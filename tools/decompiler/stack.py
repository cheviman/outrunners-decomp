"""
Name:        stack.py
Description: follows the V60 stack pointer through each function and turns stack memory into slots
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

The game passes nothing on the stack: it saves registers there (PUSH, PUSHM) and the V60 keeps return
addresses there. Knowing how far SP is from where it stood when the function started, every push,
pop and [SP] access becomes a numbered slot (ir.Stack / ir.SetStack) that data flow treats like a
register, and SP itself drops out. Slot 0 holds the caller's return address. A call leaves SP where
it was. Code that sets SP to an address of its own (boot) starts a new stack there. A function that
drops its own return address before returning (add.w #4, SP; rsr) returns from its caller as well.
"""
import copy

from v60 import ir

from . import cpu

NEW_STACK = -0x100000    # slot offsets after code sets SP itself: far from anything the caller's stack holds
MEMORY_SP = 32           # the register SP becomes in a function whose stack stays in memory (V60 only)


class StackError(Exception):
    pass


def offset_of(expression, sp, temps=None):
    """The stack offset 'expression' computes when SP is 'sp' bytes from the start, or None.
    'temps' maps temporaries already known to hold stack addresses to their offsets."""
    bits = cpu.current.address_bits
    if isinstance(expression, ir.Reg) and expression.index == cpu.current.sp:
        return sp
    if isinstance(expression, ir.Temp) and temps:
        return temps.get(expression.name)
    if isinstance(expression, ir.Op) and expression.name in ("add", "sub") and expression.bits == bits:
        base, amount = expression.args
        if isinstance(amount, ir.Const):
            inner = offset_of(base, sp, temps)
            if inner is not None:
                delta = amount.value if amount.value < 1 << (bits - 1) else amount.value - (1 << bits)
                return inner + delta if expression.name == "add" else inner - delta
    return None


def slot_reader(sp):
    """A rebuild() change that turns reads of stack memory into slots."""
    def change(node):
        if isinstance(node, ir.Load):
            offset = offset_of(node.address, sp)
            if offset is not None:
                return ir.Stack(offset, node.bits)
        return None
    return change


def mentions_sp(expression):
    return any(isinstance(node, ir.Reg) and node.index == cpu.current.sp for node in ir.walk(expression))


def analyse(function):
    """Rewrites 'function' in place; returns the SP offset at the start of each block. A function whose
    stack addresses serve as values (an address into the frame handed on, SP set to a register that walked
    the frame) keeps its stack in memory instead: SP becomes MEMORY_SP, a register the whole program
    shares, and pushes, pops and [SP] accesses go to memory, as on the V60 (code the game never calls is
    all that does it)."""
    if MEMORY_SP not in cpu.current.registers:
        return slots(function)
    saved = copy.deepcopy(function.blocks)
    try:
        found = slots(function)
        if not leftovers(function):
            return found
    except StackError:
        pass
    function.blocks = saved
    function.exits_caller = False
    function.memory_stack = True

    def change(node):
        if isinstance(node, ir.Reg) and node.index == cpu.current.sp:
            return ir.Reg(MEMORY_SP, node.bits)
        return None
    for block in function.blocks.values():
        for index, statement in enumerate(block.statements):
            if isinstance(statement, ir.SetReg) and statement.index == cpu.current.sp:
                block.statements[index] = statement = ir.SetReg(MEMORY_SP, statement.value)
            ir.map_statement(statement, change)
        ir.map_statement(block.end, change)
    return {label: 0 for label in function.blocks}


def slots(function):
    """The stack as slots: see analyse()."""
    function.exits_caller = False
    stacks = 0
    start = {function.entry: 0}
    pending = [function.entry]
    done = set()
    while pending:
        label = pending.pop()
        if label in done:
            continue
        done.add(label)
        block = function.blocks[label]
        sp = start[label]
        statements = []
        temps = {}
        for statement in block.statements:
            if isinstance(statement, ir.SetTemp):
                offset = offset_of(statement.value, sp, temps)
                if offset is not None:
                    # a stack address held for a moment: followed as an offset, the temporary goes away
                    temps[statement.temp.name] = offset
                    continue
            if isinstance(statement, ir.SetReg) and statement.index == cpu.current.sp:
                new = offset_of(statement.value, sp, temps)
                if new is None:
                    if not isinstance(statement.value, ir.Const):
                        raise StackError(f"{function.name}: SP set to {statement.value}")
                    stacks += 1    # the code's own stack: a fresh range of slots
                    new = NEW_STACK * stacks
                sp = new
                continue
            if isinstance(statement, ir.Effect) and statement.name == "return_from_interrupt":
                # RETIS to code the function pushed: the PC it pops is the jump, the PSW takes effect
                frame = statement.args[0]
                if not isinstance(frame, ir.Const):
                    raise StackError(f"{function.name}: RETIS with a computed frame")
                statements.append(ir.Effect("write_psw", (ir.Stack(sp + 4),)))
                sp += 8 + frame.value
                continue
            if isinstance(statement, ir.Store):
                offset = offset_of(statement.address, sp)
                if offset is not None:
                    value = ir.rebuild(statement.value, slot_reader(sp))
                    slot = ir.SetStack(offset, value, statement.bits)
                    if hasattr(statement, "origin"):
                        slot.origin = statement.origin    # the instruction that stores it ("local" lines)
                    statements.append(slot)
                    continue
            ir.map_statement(statement, slot_reader(sp))
            statements.append(statement)
        block.statements = statements
        ir.map_statement(block.end, slot_reader(sp))
        # a stack address that still serves as a value (the flags of "add.w #4, SP") has none in C: it
        # becomes a marker the C writer refuses, so it has to turn out unused
        def opaque(node):
            if isinstance(node, ir.Temp) and node.name in temps:
                return ir.op("stack_address", (ir.const(temps[node.name] & 0xFFFFFFFF),), cpu.current.address_bits)
            return None
        for statement in statements + [block.end]:
            ir.map_statement(statement, opaque)
        if isinstance(block.end, ir.Return) and sp == cpu.current.return_address_bytes:
            function.exits_caller = True
        elif isinstance(block.end, (ir.Return, ir.InterruptReturn)) and sp != 0 and sp != NEW_STACK * stacks:
            # (code that set up a stack of its own may hand over to its caller with that stack in place: the
            # Z80's reset gives the board its main loop)
            raise StackError(f"{function.name}: returns with SP {sp:+d} from where it started")
        for successor in block.successors():
            if successor in start and start[successor] != sp:
                raise StackError(f"{function.name}: SP {start[successor]} and {sp} meet at {successor}")
            start[successor] = sp
            pending.append(successor)
    return start


def leftovers(function):
    """Statements that still read or set SP after analyse()."""
    found = []
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            if isinstance(statement, ir.SetReg) and statement.index == cpu.current.sp:
                found.append(statement)
            elif any(mentions_sp(expression) for expression in ir.statement_expressions(statement)):
                found.append(statement)
    return found
