"""
Name:        preserved.py
Description: finds registers a function saves on the stack and puts back before every return, and
             removes the saving and the putting back
Version:     0.1.0
Created:     11-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

A register is preserved when, on every path to every return, it holds again the value it had when the
function started. A register that becomes a variable of each C function (a parameter, a result or a
local) cannot have its caller's value changed by the callee, so the push that saved it and the pop that
put it back are dropped; the register stays free for the function's own use in between, and the
function no longer counts as writing it (function.preserved, read by dataflow.py). A context register
is a global variable in C, seen by every function the callee calls in between: its save and restore
only go when nothing writes it in between, and otherwise stay as a saved copy in the C. Works on the
stack slots stack.py makes, before data flow.
"""
from v60 import ir

from . import cpu


def analyse(function, call_writes, registers=None, context=()):
    """Registers the function preserves; their saves and restores are removed from 'function' and added
    to function.preserved. 'call_writes(call)' gives the variables a call may write, from the callees'
    summaries; 'context' lists the registers that are global variables in C."""
    if not hasattr(function, "preserved"):
        function.preserved = set()
    found = []
    for register in registers if registers is not None else cpu.current.registers:
        if register in function.preserved or not mentions_register(function, register):
            continue
        traced = trace(function, register, call_writes, register in context)
        if traced is None:
            continue
        found.append(register)
        if any(read_after(function, label, register, traced[1]) for label in function.blocks):
            # the function uses what it puts back: the copy stays in its C, its callers' value is safe
            continue
        removed = set().union(*traced)
        for block in function.blocks.values():
            block.statements = [statement for statement in block.statements if id(statement) not in removed]
    function.preserved |= set(found)
    return found


def mentions_register(function, register):
    for block in function.blocks.values():
        for statement in block.statements:
            if isinstance(statement, ir.SetReg) and statement.index == register:
                return True
    return False


def writes_register(statement, register, call_writes):
    """True if a statement other than a restore may change the register."""
    if isinstance(statement, ir.SetReg):
        return statement.index == register
    if isinstance(statement, ir.Effect) and statement.name in ("divx", "divux"):
        target = statement.args[1]
        return isinstance(target, ir.Const) and register in (target.value, target.value + 1)
    if isinstance(statement, ir.Effect) and statement.name in ("move_string", "move_bits_down", "move_bits_up", "search_bytes"):
        return register in (27, 28)
    if isinstance(statement, ir.Effect) and statement.name == "search_bits_up":
        return register in (statement.args[4].value, 28)
    if isinstance(statement, ir.Effect) and statement.name == "refresh_read":
        return register == statement.args[1].value
    if isinstance(statement, ir.Call):
        return ("r", register) in call_writes(statement)
    return False


def run(block, state, register, call_writes):
    """(intact, slots holding the entry value, what the block does to the register) at the end of
    'block', from 'state' at its start."""
    intact, slots = state
    slots = set(slots)
    size = cpu.current.register_bits.get(register, 32) // 8    # bytes of the slot that keeps the register
    found = {"saves": set(), "restores": set(), "feeders": set(), "saving_slots": set(), "written": False}
    temps = {}
    for statement in block.statements:
        if isinstance(statement, ir.SetTemp):
            temps[statement.temp.name] = statement
        if isinstance(statement, ir.SetStack):
            value, feeder = statement.value, None
            if isinstance(value, ir.Temp) and value.name in temps:
                # "push R0" reads the register into a temporary first (OutRunners' interrupt handlers)
                feeder = temps[value.name]
                value = feeder.value
            if isinstance(value, ir.Reg) and value.index == register and intact and statement.bits == value.bits:
                slots.add(statement.offset)
                found["saves"].add(id(statement))
                found["saving_slots"].add(statement.offset)
                if feeder is not None:
                    found["feeders"].add(id(feeder))
                continue
            end = statement.offset + statement.bits // 8
            slots -= {offset for offset in slots if offset < end and statement.offset < offset + size}
        if isinstance(statement, ir.SetReg) and statement.index == register:
            value, feeder = statement.value, None
            if isinstance(value, ir.Temp) and value.name in temps:
                feeder = temps[value.name]
                value = feeder.value
            if isinstance(value, ir.Stack) and value.offset in slots and value.bits == size * 8:
                intact = True
                found["restores"].add(id(statement))
                if feeder is not None:
                    found["feeders"].add(id(feeder))
                continue
        if writes_register(statement, register, call_writes):
            intact = False
            found["written"] = True
    return intact, frozenset(slots), found


def reads_register(statement, register):
    return any(isinstance(node, ir.Reg) and node.index == register
               for expression in ir.statement_expressions(statement) for node in ir.walk(expression))


def read_after(function, label, register, restores):
    """True if, after a restore in block 'label', a statement reads the register before the function
    returns or writes it again."""
    statements = function.blocks[label].statements
    positions = [index for index, statement in enumerate(statements) if id(statement) in restores]
    if not positions:
        return False
    seen = set()
    work = [(label, positions[0] + 1)]
    while work:
        current, start = work.pop()
        if (current, start) in seen:
            continue
        seen.add((current, start))
        block = function.blocks[current]
        for statement in block.statements[start:]:
            if id(statement) in restores:
                continue
            if reads_register(statement, register) or isinstance(statement, ir.Call):
                return True
            if isinstance(statement, ir.SetReg) and statement.index == register:
                break
        else:
            if isinstance(block.end, ir.TailCall) or reads_register(block.end, register):
                return True
            if not isinstance(block.end, (ir.Return, ir.InterruptReturn)):
                work += [(successor, 0) for successor in block.successors()]
    return False


def trace(function, register, call_writes, is_context=False):
    """(saves, restores, the temporaries the restores read the slots through) as sets of statement ids
    if 'register' is preserved, else None.

    Forward over the blocks with a state per path: does the register hold its entry value, and which
    slots hold it. The save is a SetStack of the whole register while it is untouched; a restore is a
    SetReg from such a slot, straight or through the temporary a pop reads it into. Every return has to
    see the entry value. A slot that saved it must not be read for anything else.
    """
    blocks = function.blocks
    # forward to a fixed point: the register is intact where it is on every path, a slot holds it where
    # it does on every path
    start = {function.entry: (True, frozenset())}
    pending = [function.entry]
    while pending:
        label = pending.pop()
        intact, slots, _ = run(blocks[label], start[label], register, call_writes)
        for successor in blocks[label].successors():
            new = (intact, slots)
            if successor in start:
                old_intact, old_slots = start[successor]
                new = (old_intact and intact, old_slots & slots)
                if new == start[successor]:
                    continue
            start[successor] = new
            pending.append(successor)

    saves, restores, feeders = set(), set(), set()
    saving_slots = set()
    written_between = False
    for label, state in start.items():
        block = blocks[label]
        intact, _, found = run(block, state, register, call_writes)
        saves |= found["saves"]
        restores |= found["restores"]
        feeders |= found["feeders"]
        saving_slots |= found["saving_slots"]
        written_between = written_between or found["written"]
        end = block.end
        if isinstance(end, (ir.Return, ir.InterruptReturn)) and not intact:
            return None
        if isinstance(end, ir.ComputedJump):
            return None
        if isinstance(end, ir.TailCall) and (not intact or ("r", register) in call_writes(ir.Call(target=end.target))):
            return None
    if not saves or (is_context and written_between):
        return None
    # the saving slots and the temporaries of the restores serve nothing else
    feeder_names = set()
    for block in blocks.values():
        for statement in block.statements:
            if id(statement) in feeders:
                feeder_names.add(statement.temp.name)
    for block in blocks.values():
        for statement in block.statements + [block.end]:
            if id(statement) in restores or id(statement) in feeders or id(statement) in saves:
                continue
            for expression in ir.statement_expressions(statement):
                for node in ir.walk(expression):
                    if isinstance(node, ir.Stack) and node.offset in saving_slots:
                        return None
                    if isinstance(node, ir.Temp) and node.name in feeder_names:
                        return None
    return saves, restores, feeders
