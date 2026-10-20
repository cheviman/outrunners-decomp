"""
Name:        cleanup.py
Description: removes the statements whose results nothing reads (flags no branch tests, values overwritten
             before use), once every function's signature is known
Version:     0.1.0
Created:     11-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

Liveness runs backwards over each function with the data flow's own rules (demanded bits, what calls
read and write), from what is live where the function ends: its results, and the context registers,
which are global variables in C. A statement goes when none of what it writes is live after it, and it
then asks for nothing, so values only dead code reads go too. Memory writes, calls and effects always
stay. A memory read whose value nothing needs goes as well, unless its address is a constant outside the
plain memories (ROM, RAM, video, sprites, palette): reading the board's I/O, interrupt controller or
random number generator does things.

What a call passes by value is worked out as the original does it even where the callee's use of it does
not matter to this caller (point_to_screen works out a screen x the caller drops, from the projected x
the caller loads into R0 for it): the C writes the argument, so it has to be what the code passes, not
whatever the register held before. Where that takes a register this function's own callers do not hand
it, the C reads the 0 its variable starts with, as for every input no caller sets.
"""
from v60 import ir

from . import cpu

from . import dataflow
from . import signatures

# address >> 20 of the areas where a read has no side effect
PLAIN_MEMORY = {0x0, 0x1, 0x2, 0x3, 0x4, 0x6, 0x7, 0xF}


def variables_of(values):
    """Data flow variables of C values (("r", n) or ("f", name))."""
    found = set()
    for value in values:
        if value[0] == "r":
            found |= dataflow.register_parts(value[1], dataflow.FULL)
        else:
            found.add(value)
    return found


def has_effect(value):
    """True if evaluating 'value' reads a place where reading does something."""
    for node in ir.walk(value):
        if isinstance(node, ir.Load):
            address = node.address
            if isinstance(address, ir.Const) and ((address.value & 0xFFFFFF) >> 20) not in PLAIN_MEMORY:
                return True
    return False


class Liveness:
    """Liveness of one function, with dead statements left out of it."""

    def __init__(self, flow, function, exit_live, passed=None):
        self.flow = flow
        self.function = function
        self.exit_live = exit_live
        self.passed = passed or {}    # id(call or tail call) -> what it reads to pass by value, live anyway

    def needed(self, statement, live, temps):
        if isinstance(statement, ir.SetTemp):
            if statement.temp.name in temps:
                return True
            if has_effect(statement.value):
                temps[statement.temp.name] = dataflow.width(statement.temp.bits)
                return True
            return False
        if isinstance(statement, ir.SetReg):
            if not cpu.kept(statement.index):
                return True
            parts, _, _ = dataflow.partial_write(statement)
            return any(("r", statement.index, part) in live for part in parts)
        if isinstance(statement, ir.SetFlag):
            return ("f", statement.name) in live
        if isinstance(statement, ir.SetStack):
            return ("s", statement.offset) in live
        return True

    def end_live(self, block, live_in):
        end = block.end
        live = set()
        for successor in block.successors():
            live |= live_in.get(successor, set())
        if isinstance(end, (ir.Return, ir.TailCall, ir.InterruptReturn)):
            live |= self.exit_live
        out = set(live)
        temps = {}
        self.flow.transfer(end, live, temps, end=True)
        live |= self.passed.get(id(end), set())
        return live, temps, out

    def backwards(self, block, live_in, keep=None):
        """Live variables at the start of 'block'. With 'keep', records the statements to keep, each with the
        variables live after it, and (under ("end", id(block))) what is live when the block is left."""
        live, temps, out = self.end_live(block, live_in)
        if keep is not None:
            keep[("end", id(block))] = out
        for statement in reversed(block.statements):
            if not self.needed(statement, live, temps):
                continue
            if keep is not None:
                keep[id(statement)] = set(live)
            self.flow.transfer(statement, live, temps)
            live |= self.passed.get(id(statement), set())
        return live

    def solve(self):
        blocks = self.function.blocks
        live_in = {label: set() for label in blocks}
        order = list(blocks)
        changed = True
        while changed:
            changed = False
            for label in reversed(order):
                live = self.backwards(blocks[label], live_in)
                if live != live_in[label]:
                    live_in[label] = live
                    changed = True
        return live_in


def context_live(flow, entry, context):
    """The parts of the global registers live where a function ends: all of them, but of the scratch ones
    (signatures.SCRATCH_CONTEXT) only the bits later code may read without setting them, and those its
    callers read after calling it."""
    live = set()
    handed = flow.exit_live.get(entry, frozenset())
    for index in context:
        live |= dataflow.register_parts(index, signatures.SCRATCH_CONTEXT.get(index, dataflow.FULL))
        live |= {variable for variable in handed if variable[0] == "r" and variable[1] == index}
    return live


def passed_by_value(flow, function, arguments):
    """{id(call or tail call): the parts of the registers it passes by value that its callee reads} for the
    calls of 'function' to one known function; 'arguments' gives each callee's by-value registers."""
    found = {}
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            if isinstance(statement, ir.Call) and statement.slot is None and statement.target is not None:
                target = statement.target & 0xFFFFFF
            elif isinstance(statement, ir.TailCall):
                target = statement.target
            else:
                continue
            summary = flow.summaries.get(target)
            registers = arguments.get(target)
            if summary is None or not registers:
                continue
            # the whole of what the C passes: every part up to the highest one the callee reads (its parameter
            # is that wide), not only the parts it reads (add_to_touchable_list reads only R0's high half)
            parts = set()
            for index in registers:
                read = [variable[2] for variable in summary.reads if variable[0] == "r" and variable[1] == index]
                if read:
                    parts |= {("r", index, part) for part in range(max(read) + 1)}
            if parts:
                found[id(statement)] = parts
    return found


def eliminate(flow, function, results, context, arguments=None):
    """Removes the dead statements of 'function' in place. 'results' are its C results, 'context' the
    registers that are global variables, 'arguments' {callee: the registers it takes by value} (see the
    module's notes). Returns {id(statement): variables live after it} for the statements that stay, and
    the variables live at the start. Where it ends, a result it returns is live whole: the caller takes
    the whole variable, even where the original only wrote part of the register and the caller's other
    bits go through it ("and.b #FF, R0"). Results it hands back through pointers are copied back only in
    the parts the function may write (emit.py), so only those are live."""
    if len(results) > 1:
        written = flow.summaries[function.entry].writes
        exit_live = {variable for variable in written if (variable[0], variable[1]) in set(results)}
    else:
        exit_live = variables_of(results)
    exit_live |= context_live(flow, function.entry, context)
    liveness = Liveness(flow, function, exit_live, passed_by_value(flow, function, arguments) if arguments else {})
    live_in = liveness.solve()
    after = {}
    for label, block in function.blocks.items():
        keep = {}
        liveness.backwards(block, live_in, keep)
        block.statements = [statement for statement in block.statements if id(statement) in keep]
        after.update(keep)
    return after, live_in[function.entry]
