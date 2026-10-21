"""
Name:        knownbits.py
Description: follows which bits of each variable are known to be zero, and simplifies what that makes
             pointless: masks that clear nothing, casts that cut nothing
Version:     0.1.0
Created:     12-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

"mov.b [R18], R0" loads a byte into R0: in C, r0 = mem_read8(...), and bits 8-31 of r0 are zero from
then on. So a later "(r0 & 0xFFFF0000u) | x" is x, and "(uint16_t)r0" is r0. This runs on the C the way
emit.py writes it, after everything else: variables start at 0, a call changes only the results it
hands back (and the global context registers), and where paths meet a bit is known zero only if it is
on every path.

A cast that cuts nothing becomes Op("fits", (value,), bits): the value, already within that width.
The index of a table read ("base + r0 * 2") known to fit in 16 bits says so the same way,
zext(fits(r0, 16)), so that memory.py takes it for an index into the table rather than a pointer.
"""
from v60 import ir

from . import cpu

FULL = 0xFFFFFFFF


def width_mask(bits):
    return ir.MASKS[bits] if bits <= 64 else (1 << bits) - 1


def possible(mask_known_zero, bits):
    """Bits that may be set, given those known zero."""
    return ~mask_known_zero & width_mask(bits)


def highest(value):
    """Mask of every bit up to the highest one set."""
    return (1 << value.bit_length()) - 1 if value else 0


class KnownBits:
    def __init__(self, function, results, context, call_results):
        self.function = function
        self.results = {value[1] for value in results if value[0] == "r"}
        self.context = set(context)
        self.call_results = call_results    # call statement -> registers it hands back

    # ---- one expression ----

    def zero(self, node, state, temps):
        """Bits of 'node' known to be zero (within its width)."""
        bits = node.bits
        mask = width_mask(bits)
        if isinstance(node, ir.Const):
            return ~node.value & mask
        if isinstance(node, ir.Reg):
            return state.get(node.index, 0) & mask
        if isinstance(node, ir.Temp):
            return temps.get(node.name, 0) & mask
        if not isinstance(node, ir.Op):
            return 0
        name, args = node.name, node.args
        if name == "zext":
            inner = args[0]
            return (self.zero(inner, state, temps) | (mask & ~width_mask(inner.bits))) & mask
        if name in ("trunc", "fits"):
            return self.zero(args[0], state, temps) & mask
        if name == "and":
            return (self.zero(args[0], state, temps) | self.zero(args[1], state, temps)) & mask
        if name in ("or", "xor"):
            return self.zero(args[0], state, temps) & self.zero(args[1], state, temps) & mask
        if name in ("add", "mul"):
            left = possible(self.zero(args[0], state, temps), args[0].bits)
            right = possible(self.zero(args[1], state, temps), args[1].bits)
            if name == "add":
                top = highest(left | right)
                reach = (top << 1) | 1 if top else 0
            else:
                reach = highest(left) * highest(right)
                reach = highest(reach)
            return ~reach & mask if left or right else mask
        if name == "shl" and isinstance(args[1], ir.Const) and args[1].value < bits:
            count = args[1].value
            return ((self.zero(args[0], state, temps) << count) | ((1 << count) - 1)) & mask
        if name == "shr" and isinstance(args[1], ir.Const) and args[1].value < bits:
            count = args[1].value
            return ((self.zero(args[0], state, temps) >> count) | (mask & ~(mask >> count))) & mask
        if name == "select":
            return self.zero(args[1], state, temps) & self.zero(args[2], state, temps) & mask
        return 0

    def mark_index(self, address, state, temps):
        """"base + index" (the index maybe scaled) with the index known to fit in 16 bits, marked so."""
        if not isinstance(address, ir.Op) or address.name != "add":
            return address
        left, right = address.args
        if isinstance(left, ir.Const) == isinstance(right, ir.Const):
            return address
        marked = self.mark(right if isinstance(left, ir.Const) else left, state, temps)
        if isinstance(left, ir.Const):
            return address if marked is right else ir.Op("add", (left, marked), address.bits)
        return address if marked is left else ir.Op("add", (marked, right), address.bits)

    def mark(self, index, state, temps):
        if isinstance(index, ir.Op) and index.name in ("mul", "shl") and len(index.args) == 2:
            first, second = index.args
            if isinstance(second, ir.Const):
                inner = self.mark(first, state, temps)
                return index if inner is first else ir.Op(index.name, (inner, second), index.bits)
            if index.name == "mul" and isinstance(first, ir.Const):
                inner = self.mark(second, state, temps)
                return index if inner is second else ir.Op(index.name, (first, inner), index.bits)
            return index
        if isinstance(index, (ir.Reg, ir.Temp)) and index.bits == 32:
            if not possible(self.zero(index, state, temps), 32) & ~0xFFFF:
                return ir.Op("zext", (ir.Op("fits", (index,), 16),), 32)
        return index

    def rewrite(self, node, state, temps):
        """'node' with what known zeros make pointless taken out."""
        return ir.rebuild(node, self.changer(state, temps))

    def changer(self, state, temps):
        """The rebuild() change that takes out what known zeros make pointless."""
        def change(current):
            if isinstance(current, ir.Load):
                address = self.mark_index(current.address, state, temps)
                return None if address is current.address else ir.Load(address, current.bits)
            if not isinstance(current, ir.Op):
                return None
            name, args, bits = current.name, current.args, current.bits
            if name == "and" and isinstance(args[1], ir.Const):
                may = possible(self.zero(args[0], state, temps), args[0].bits)
                if not may & args[1].value:
                    return ir.Const(0, bits)
                if not may & ~args[1].value & width_mask(bits):
                    return args[0]
            if name == "trunc" and args[0].bits <= 32:
                may = possible(self.zero(args[0], state, temps), args[0].bits)
                if not may & ~width_mask(bits):
                    return ir.Op("fits", (args[0],), bits)
            if name in ("or", "xor") and args[0] == ir.Const(0, bits):
                return args[1]
            if name in ("or", "xor") and args[1] == ir.Const(0, bits):
                return args[0]
            return None
        return change

    # ---- statements ----

    def step(self, statement, state, temps, rewrite):
        """State after 'statement'; with 'rewrite', its expressions are simplified first."""
        if rewrite:
            ir.map_statement(statement, self.changer(state, temps))
            if isinstance(statement, ir.Store):
                statement.address = self.mark_index(statement.address, state, temps)
        if isinstance(statement, ir.SetTemp):
            temps[statement.temp.name] = self.zero(statement.value, state, temps)
        elif isinstance(statement, ir.SetReg):
            state[statement.index] = self.zero(statement.value, state, temps)
        elif isinstance(statement, ir.Call):
            for index in self.call_results.get(id(statement), ()):
                state[index] = 0
            for index in self.context:
                state[index] = 0
        elif isinstance(statement, ir.Effect):
            for index in (27, 28) if statement.name in ("move_string", "move_bits_down", "move_bits_up", "search_bytes") else ():
                state[index] = 0
            if statement.name == "refresh_read":
                state[statement.args[1].value] = 0
            if statement.name == "search_bits_up":
                state[statement.args[4].value] = 0
                state[28] = 0
            if statement.name in ("divx", "divux"):
                register = statement.args[1]
                if isinstance(register, ir.Const) and register.value < 29:
                    state[register.value] = 0
                    state[register.value + 1] = 0
            for index in self.context:
                state[index] = 0
        return state

    def run(self):
        blocks = self.function.blocks
        entry = self.function.entry
        start = {entry: self.entry_state()}
        pending = [entry]
        while pending:
            label = pending.pop()
            state = dict(start[label])
            temps = {}
            for statement in blocks[label].statements:
                self.step(statement, state, temps, False)
            for successor in blocks[label].successors():
                if successor not in start:
                    start[successor] = dict(state)
                    pending.append(successor)
                    continue
                merged = {index: start[successor].get(index, 0) & state.get(index, 0)
                          for index in set(start[successor]) | set(state)}
                if merged != start[successor]:
                    start[successor] = merged
                    pending.append(successor)
        for label, state in start.items():
            state = dict(state)
            temps = {}
            block = blocks[label]
            for statement in block.statements:
                self.step(statement, state, temps, True)
            ir.map_statement(block.end, self.changer(state, temps))

    def entry_state(self):
        """Every local starts at 0 (all bits known zero); parameters, results and globals are unknown."""
        state = {index: FULL for index in cpu.current.registers}
        for index in self.results | self.context | self.params:
            state[index] = 0
        return state


def simplify(function, signature, context, call_results):
    analysis = KnownBits(function, signature.results, context, call_results)
    analysis.params = {value[1] for value in signature.params if value[0] == "r"}
    analysis.run()
