"""
Name:        constants.py
Description: replaces R25, the base of the game's variables, with its value at each point
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

Boot sets R25 to 0x208000 and the game reaches its variables as displacements from it. The only other
value it ever gets is a constant too: the memory test points it at video RAM (0x308000) while it checks
work RAM, calls nothing meanwhile, and sets it back before it returns. So every function starts with
R25 = 0x208000, R25 is followed through the function as a constant (with the stack slots that hold a
copy of it, for the VBLANK handler that saves every register and puts them back), every read becomes
that constant and every write goes away. Runs on the stack slots stack.py makes.
"""
from v60 import ir

GLOBALS_BASE = 25
GLOBALS_VALUE = 0x208000


class ConstantError(Exception):
    pass


def known(expression, temps, slots):
    """The value of an expression made of constants, temporaries bound to them and 32-bit stack slots
    holding a known value, or None."""
    if isinstance(expression, ir.Const):
        return expression.value
    if isinstance(expression, ir.Temp):
        value = temps.get(expression.name)
        return known(value, temps, slots) if value is not None else None
    if isinstance(expression, ir.Stack) and expression.bits == 32:
        return slots.get(expression.offset)
    if isinstance(expression, ir.Op) and expression.name in ("zext", "trunc"):
        inner = known(expression.args[0], temps, slots)
        return inner & ir.MASKS[expression.bits] if inner is not None else None
    return None


def overlapping(slots, offset, bits):
    """Slots a store of 'bits' at 'offset' changes."""
    end = offset + bits // 8
    return [start for start in slots if start < end and offset < start + 4]


def run(function, block, state, register, rewrite):
    """The register's value and the known slots at the end of 'block', from 'state' at its start. With
    'rewrite', reads of the register become the constant and writes to it go."""
    value, slots = state
    slots = dict(slots)
    temps = {}
    statements = []
    for statement in block.statements:
        def change(node, value=value):
            return ir.Const(value) if isinstance(node, ir.Reg) and node.index == register else None
        if rewrite:
            ir.map_statement(statement, change)
        if isinstance(statement, ir.SetTemp):
            temps[statement.temp.name] = ir.rebuild(statement.value, change)
        elif isinstance(statement, ir.SetStack):
            for start in overlapping(slots, statement.offset, statement.bits):
                del slots[start]
            if statement.bits == 32:
                stored = known(ir.rebuild(statement.value, change), temps, slots)
                if stored is not None:
                    slots[statement.offset] = stored
        elif isinstance(statement, ir.Call) and value != GLOBALS_VALUE:
            raise ConstantError(f"{function.name}: calls out with R{register} = {value:X}")
        elif isinstance(statement, ir.SetReg) and statement.index == register:
            new = known(ir.rebuild(statement.value, change), temps, slots)
            if new is None:
                raise ConstantError(f"{function.name}: R{register} set to {statement.value}")
            value = new
            continue
        statements.append(statement)
    if rewrite:
        block.statements = statements
        ir.map_statement(block.end, lambda node: (
            ir.Const(value) if isinstance(node, ir.Reg) and node.index == register else None))
    return value, frozenset(slots.items())


def substitute(function, register=GLOBALS_BASE, value_at_entry=GLOBALS_VALUE):
    """Rewrites 'function' in place, or raises ConstantError if the register gets a value that is not
    known, two values meet, or the function calls out or leaves with a value other than the one it
    started with."""
    blocks = function.blocks
    start = {function.entry: (value_at_entry, frozenset())}
    pending = [function.entry]
    while pending:
        label = pending.pop()
        value, slots = run(function, blocks[label], start[label], register, rewrite=False)
        end = blocks[label].end
        if isinstance(end, (ir.Return, ir.InterruptReturn, ir.TailCall, ir.ComputedJump)) and value != value_at_entry:
            raise ConstantError(f"{function.name}: leaves with R{register} = {value:X}")
        for successor in blocks[label].successors():
            if successor in start:
                old_value, old_slots = start[successor]
                if old_value != value:
                    raise ConstantError(f"{function.name}: R{register} is {old_value:X} and {value:X} at {successor:X}")
                new = (value, old_slots & slots)
                if new == start[successor]:
                    continue
                start[successor] = new
            else:
                start[successor] = (value, slots)
            pending.append(successor)
    for label, state in start.items():
        run(function, blocks[label], state, register, rewrite=True)


def uses(function, register=GLOBALS_BASE):
    """Statements of 'function' that still read or write the register."""
    found = []
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            if isinstance(statement, ir.SetReg) and statement.index == register:
                found.append(statement)
                continue
            for expression in ir.statement_expressions(statement):
                if any(isinstance(node, ir.Reg) and node.index == register for node in ir.walk(expression)):
                    found.append(statement)
                    break
    return found
