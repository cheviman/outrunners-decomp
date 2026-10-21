# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        function.py
Description: the sound program's functions as control-flow graphs of IR blocks, with the time they take
Version:     0.1.0
Created:     13-10-2026
Modified:    21-10-2026
Author:      Sebastian Toyos

Takes the functions tools/z80/analysis.py splits the program into, lifts every instruction to IR
(tools/z80/lift.py for data, here for control), and keeps the sound board's timing model (see
runtime/src/sound_cpu.c): every basic block starts with Effect("z80_block", cycles, fetches), where the
board runs what is due (the YM3438 timers, an interrupt the Z80 would take), the block's time passes and its
opcode fetches count the refresh register up (LD A,R reads it: Effect("refresh_read", k, A), k the block's
fetches up to it); a
condition that sends a JR, DJNZ, CALL or RET the long way adds its extra cycles on that way
(Effect("z80_cycles")). A block ends after every call, so a callee starts when its caller's time says.
DI and EI become Effects; EI as the last instruction of a block delays interrupts by that block start,
as the Z80 delays them by one instruction.

The main loop's top (the "loop" line of sound_symbols.txt) is where the program waits for the board:
every way into it returns to the board, which calls it again while the board's time is ahead.
"""
from decompiler.function import Block, Function
from v60 import ir

from . import disassembler as dis
from . import lift
from . import timing

CONDITIONS = ("nz", "z", "nc", "c", "po", "pe", "p", "m")


def condition(name):
    """IR condition of a condition code, from the flags."""
    flags = {"z": ir.Flag("z"), "c": ir.Flag("cy"), "pe": ir.Flag("ov"), "m": ir.Flag("s")}
    if name in flags:
        return flags[name]
    return lift.lnot({"nz": flags["z"], "nc": flags["c"], "po": flags["pe"], "p": flags["m"]}[name])


def is_stack_swap(instruction):
    return instruction.mnemonic == "ex" and instruction.operands[0].mode == "reg_indirect" \
        and instruction.operands[0].reg == "sp"


def leaders(program):
    """Where blocks start, the same for every function and for the reference's timing (s32ref reads them):
    entries, every target, and the instruction after anything that branches, jumps, returns or calls."""
    found = set(program.entries)
    for address, instruction in program.analysis.instructions.items():
        following = (address + instruction.length) & 0xFFFF
        for target in program.successors(instruction):
            if target != following:
                found.add(target)
        if instruction.flow in (dis.FLOW_BRANCH, dis.FLOW_JUMP, dis.FLOW_RETURN, dis.FLOW_CALL):
            found.add(following)
    return found & set(program.analysis.instructions)


class Builder:
    """Lifts the functions of a tools/z80/analysis.Program."""

    def __init__(self, program):
        self.program = program
        self.entries = program.entries
        self.leaders = leaders(program)
        self.loop = next((address for address, symbol in program.symbols.items() if symbol.kind == "loop"), None)

    def build(self, entry):
        body = self.program.bodies[entry]
        function = Function(entry, self.program.names[entry])
        self.extra_blocks = []
        order = sorted(body)
        current = None
        for position, address in enumerate(order):
            instruction = body[address]
            if current is not None and address in self.leaders:
                if current.end is None:
                    current.end = self.transfer(entry, address)
                current = None
            if current is None:
                current = Block(address)
                current.statements.append(ir.Effect("z80_block", (ir.const(0, 32), ir.const(0, 32))))
                function.blocks[address] = current
            current.addresses.append(address)
            before, extra = len(current.statements), len(self.extra_blocks)
            self.lift(function, current, instruction, body)
            for statement in current.statements[before:]:
                statement.origin = address
            for block in self.extra_blocks[extra:]:
                for statement in block.statements:
                    statement.origin = address
            if current.end is not None and not hasattr(current.end, "origin"):
                current.end.origin = address
            following = (address + instruction.length) & 0xFFFF
            if current.end is None and (position + 1 >= len(order) or order[position + 1] != following
                                        or following in self.entries):
                current.end = self.transfer(entry, following)
                current.end.origin = address
            if current.end is not None:
                current = None
        for block in function.blocks.values():
            block.statements[0] = ir.Effect("z80_block", (ir.const(self.block_cycles(body, block), 32),
                                                          ir.const(self.block_fetches(block), 32)))
            block.statements[0].origin = block.addresses[0]
        for block in self.extra_blocks:
            function.blocks[block.label] = block
        self.close_transfers(function)
        return function

    def block_cycles(self, body, block):
        """The block's time: every instruction, a conditional one the short way."""
        return sum(timing.cycles(body[address])[1] for address in block.addresses)

    def block_fetches(self, block):
        """The opcode fetches of the block's instructions (timing.fetches)."""
        return sum(timing.fetches(self.program.mem.r8(address)) for address in block.addresses)

    def transfer(self, entry, target):
        """How execution gets to 'target': back to the board at the loop's top, a jump in this function, or
        into another one."""
        if target == self.loop:
            return ir.Return()
        if target != entry and target in self.entries:
            return ir.TailCall(target)
        return ir.Goto(target)

    def close_transfers(self, function):
        """Branch targets in other functions get a block of their own with the tail call."""
        def resolve(target, origin):
            if target in function.blocks:
                return target
            if target == self.loop:
                label = ("wait", target)
                if label not in function.blocks:
                    function.blocks[label] = Block(label, end=ir.Return())
                return label
            if target in self.entries:
                label = ("tail", target)
                if label not in function.blocks:
                    function.blocks[label] = Block(label, end=ir.TailCall(target))
                    function.blocks[label].end.origin = origin
                return label
            raise lift.Unsupported(f"function {function.entry:04X}: no code at {target:04X}")

        for block in list(function.blocks.values()):
            end = block.end
            origin = block.addresses[-1] if block.addresses else None
            if isinstance(end, ir.Goto):
                end.target = resolve(end.target, origin)
            elif isinstance(end, ir.Branch):
                end.target = resolve(end.target, origin)
                end.otherwise = resolve(end.otherwise, origin)
            elif isinstance(end, ir.Switch):
                end.cases = {case: resolve(target, origin) for case, target in end.cases.items()}

    def longer_way(self, instruction, label, statements, end):
        """A block for the way a condition makes longer: its extra cycles, then where it goes."""
        taken, short = timing.cycles(instruction)
        block = Block(label, [ir.Effect("z80_cycles", (ir.const(taken - short, 32),)), *statements], end)
        self.extra_blocks.append(block)
        return label

    def swapped_back(self, instruction, body):
        """True for either of two EX (SP),rr in a row in the same block (the program's delays)."""
        if not hasattr(self, "_pairs") or self._pairs[0] is not body:
            pairs = set()
            order = sorted(body)
            position = 0
            while position + 1 < len(order):
                first, second = body[order[position]], body[order[position + 1]]
                if is_stack_swap(first) and is_stack_swap(second) and first.text == second.text \
                        and second.address == (first.address + first.length) & 0xFFFF \
                        and second.address not in self.leaders:
                    pairs |= {first.address, second.address}
                    position += 2
                else:
                    position += 1
            self._pairs = (body, pairs)
        return instruction.address in self._pairs[1]

    @staticmethod
    def carry_left(statements):
        """The constant the carry holds after 'statements' (the block so far), or None: the last value
        set, unless a call or an effect comes after it."""
        for statement in reversed(statements):
            if isinstance(statement, ir.SetFlag) and statement.name == "cy":
                return statement.value if isinstance(statement.value, ir.Const) else None
            if isinstance(statement, (ir.Call, ir.Effect)):
                return None
        return None

    def lift(self, function, block, instruction, body):
        address = instruction.address
        mnemonic = instruction.mnemonic
        following = (address + instruction.length) & 0xFFFF
        statements = block.statements

        if self.swapped_back(instruction, body):
            return    # EX (SP),HL twice: nothing changes, only time passes (the block counts it)
        if mnemonic == "ld" and [operand.reg for operand in instruction.operands] == ["a", "r"]:
            # the refresh register: the fetches up to the block's start, and the block's own up to here
            fetched = self.block_fetches(block)
            statements.append(ir.Effect("refresh_read", (ir.const(fetched, 32), ir.const(lift.A, 8))))
            value = lift.reg(lift.A)
            statements.append(ir.SetFlag("z", ir.op("eq", (value, lift.c8(0)), 1)))
            statements.append(ir.SetFlag("s", ir.op("ne", (ir.op("and", (value, lift.c8(0x80)), 8), lift.c8(0)), 1)))
            statements.append(ir.SetFlag("ov", ir.op("helper:interrupts_enabled", (), 1)))
            return
        lifter = lift.Lifter(self.program.mem, prefix=f"t{address:04X}_", carry=self.carry_left(statements))
        if lift.lift_data(lifter, instruction):
            statements += lifter.statements
            return

        target = instruction.target
        code = instruction.condition
        if mnemonic == "di":
            statements.append(ir.Effect("interrupts_off"))
            return
        if mnemonic == "ei":
            last = following in self.leaders or following not in body
            statements.append(ir.Effect("interrupts_on", (ir.const(1 if last else 0, 1),)))
            return
        if mnemonic == "im":
            if instruction.operands[0].value != 1:
                raise lift.Unsupported(f"{address:04X}: {instruction.text}")
            return
        if mnemonic in ("jp", "jr") and code is None and target is not None:
            block.end = self.transfer(function.entry, target)
            return
        if mnemonic in ("jp", "jr") and code is not None:
            taken = target
            if mnemonic == "jr":
                taken = self.longer_way(instruction, ("taken", address), [], ir.Goto(target))
            block.end = ir.Branch(condition(code), taken, following)
            return
        if mnemonic == "jp":
            table = self.program.analysis.jump_tables.get(address)
            if not table:
                raise lift.Unsupported(f"{address:04X}: {instruction.text} with no table")
            register = lift.PAIRS[instruction.operands[0].reg]
            block.end = ir.Switch(lift.reg(register), {case: case for case in table})
            return
        if mnemonic == "djnz":
            lifter.write8("b", ir.op("sub", (lifter.read8("b"), lift.c8(1)), 8))
            statements += lifter.statements
            nonzero = ir.op("ne", (lifter.read8("b"), lift.c8(0)), 1)
            taken = self.longer_way(instruction, ("taken", address), [], ir.Goto(target))
            block.end = ir.Branch(nonzero, taken, following)
            return
        if mnemonic in ("call", "rst"):
            call = ir.Call(target=target, return_address=following)
            if code is None:
                statements.append(call)
                block.end = ir.Goto(following)
            else:
                taken = self.longer_way(instruction, ("call", address), [call], ir.Goto(following))
                block.end = ir.Branch(condition(code), taken, following)
            return
        if mnemonic == "ret":
            if code is None:
                block.end = ir.Return()
            else:
                taken = self.longer_way(instruction, ("return", address), [], ir.Return())
                block.end = ir.Branch(condition(code), taken, following)
            return
        if mnemonic == "reti":
            # the handler enables interrupts right before it (EI), so copying IFF2 to IFF1 changes nothing
            block.end = ir.Return()
            return
        raise lift.Unsupported(f"{address:04X}: {instruction.text}")
