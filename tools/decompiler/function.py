"""
Name:        function.py
Description: the game's functions as control-flow graphs of IR blocks, built from the analysed program
Version:     0.1.0
Created:     11-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

Takes the functions tools/recompile.py splits the program into and lifts every instruction to IR
(tools/v60/lift.py for data, here for control). The V60's tricks with its stack become plain control
flow: "movea back[PC], [-SP]" followed by a jump into other code, or by the start of another
function, is a call that comes back to 'back'; a jump table entered that way is a table of calls
(the analysis makes its entries functions); a return that lands on a pushed address is a jump.
"""
from dataclasses import dataclass, field

from v60 import disassembler as dis
from v60 import ir
from v60 import lift


def flag_condition(name):
    """IR condition of a conditional branch ('e' for be, 'lt' for blt...), from the flags."""
    z, s, ov, cy = (ir.Flag(flag) for flag in ("z", "s", "ov", "cy"))

    def lnot(value):
        return ir.op("eq", (value, ir.const(0, 1)), 1)

    def xor(a, b):
        return ir.op("xor", (a, b), 1)

    def lor(a, b):
        return ir.op("or", (a, b), 1)

    table = {
        "v": ov, "nv": lnot(ov), "l": cy, "nl": lnot(cy), "e": z, "ne": lnot(z),
        "nh": lor(cy, z), "h": lnot(lor(cy, z)), "n": s, "p": lnot(s),
        "lt": xor(s, ov), "ge": lnot(xor(s, ov)), "le": lor(xor(s, ov), z), "gt": lnot(lor(xor(s, ov), z)),
    }
    return table[name]


CONDITION_NAMES = ("v", "nv", "l", "nl", "e", "ne", "nh", "h", "n", "p", "lt", "ge", "le", "gt")


@dataclass
class Block:
    label: object                                  # address of its first instruction, or ("tail", target)
    statements: list = field(default_factory=list)
    end: object = None
    addresses: list = field(default_factory=list)  # its instructions, for comments and diagnostics

    def successors(self):
        end = self.end
        if isinstance(end, ir.Goto):
            return [end.target]
        if isinstance(end, ir.Branch):
            return [end.target, end.otherwise]
        if isinstance(end, ir.Switch):
            return list(dict.fromkeys(end.cases.values()))
        return []


@dataclass
class Function:
    entry: int
    name: str
    blocks: dict = field(default_factory=dict)     # label -> Block, the entry first


class Builder:
    """Lifts the functions of a recompile.Program."""

    def __init__(self, program):
        self.program = program
        self.analysis = program.analysis
        self.mem = program.mem
        self.entries = program.entries
        # the frame loop: where the board runs its events and the CPU its interrupts (cpu.h)
        named = {symbol.name: address for address, symbol in program.symbols.items()}
        self.frame_wait_loop = named["frame_wait_loop"]
        self.frame_wait_done = named["frame_wait_done"]
        loop = self.analysis.instructions[self.frame_wait_loop]
        self.frame_resume = self.frame_wait_loop + loop.length

    def build_all(self):
        return {entry: self.build(entry) for entry in sorted(self.entries)}

    # ---- one function ----

    def build(self, entry):
        body = self.program.bodies[entry]
        function = Function(entry, self.program.names[entry])
        self.extra_blocks = []
        leaders = self.leaders(entry, body)
        order = sorted(body)
        current = None
        for position, address in enumerate(order):
            instruction = body[address]
            if current is not None and address in leaders:
                if current.end is None:
                    current.end = ir.Goto(address)
                current = None
            if current is None:
                current = Block(address)
                function.blocks[address] = current
            current.addresses.append(address)
            before, extra = len(current.statements), len(self.extra_blocks)
            self.lift(function, current, instruction, body)
            # where each statement comes from: symbols.txt names a local by the instruction that writes it
            for statement in current.statements[before:]:
                statement.origin = address
            for block in self.extra_blocks[extra:]:
                for statement in block.statements:
                    statement.origin = address
            if current.end is not None and not hasattr(current.end, "origin"):
                current.end.origin = address
            following = address + instruction.length
            if current.end is None and (position + 1 >= len(order) or order[position + 1] != following):
                # execution runs off this stretch of code into the next function
                current.end = self.transfer(entry, following)
                current.end.origin = address
            if current.end is not None:
                current = None
        for block in self.extra_blocks:
            function.blocks[block.label] = block
        self.close_transfers(function)
        function.blocks = {entry: function.blocks[entry],
                           **{label: block for label, block in function.blocks.items() if label != entry}}
        second = getattr(self.program, "second_entries", {}).get(entry)
        if second:
            self.add_entry_switch(function, second)
        return function

    @staticmethod
    def add_entry_switch(function, second):
        """A function with more ways in than its first instruction starts by going to the one it was
        entered by (EntryPoint: 0 the first, then 'second' in order)."""
        entry = function.entry
        start = ("start", entry)

        def moved(target):
            return start if target == entry else target
        for block in function.blocks.values():
            end = block.end
            if isinstance(end, ir.Goto):
                end.target = moved(end.target)
            elif isinstance(end, ir.Branch):
                end.target, end.otherwise = moved(end.target), moved(end.otherwise)
            elif isinstance(end, ir.Switch):
                end.cases = {case: moved(target) for case, target in end.cases.items()}
        first = function.blocks.pop(entry)
        first.label = start
        cases = {0: start}
        for number, address in enumerate(second, 1):
            cases[number] = address
        function.blocks = {entry: Block(entry, end=ir.Switch(ir.EntryPoint(), cases)), start: first,
                           **function.blocks}

    def leaders(self, entry, body):
        leaders = {entry}
        for address, instruction in body.items():
            following = address + instruction.length
            for target in self.program.successors(instruction):
                if target != following:
                    leaders.add(target)
            if instruction.flow in (dis.FLOW_BRANCH, dis.FLOW_JUMP, dis.FLOW_RETURN):
                leaders.add(following)
        return leaders & set(body)

    def transfer(self, entry, target):
        """How execution gets to 'target': a jump in this function, or into another one."""
        if target != entry and target in self.entries:
            return ir.TailCall(target)
        inner = getattr(self.program, "inner_entries", {}).get(target)
        if inner is not None and target not in self.program.bodies[entry]:
            return ir.TailCall(inner, entry=target)
        return ir.Goto(target)

    def close_transfers(self, function):
        """Branch and switch targets in other functions get a block of their own with the tail call (its
        origin, for the names of what that call hands back, is the instruction that jumps there first)."""
        def resolve(target, origin):
            if target in function.blocks:
                return target
            if target in self.entries:
                label = ("tail", target)
                if label not in function.blocks:
                    function.blocks[label] = Block(label, end=ir.TailCall(target))
                    function.blocks[label].end.origin = origin
                return label
            inner = getattr(self.program, "inner_entries", {}).get(target)
            if inner is not None:
                label = ("tail", target)
                if label not in function.blocks:
                    function.blocks[label] = Block(label, end=ir.TailCall(inner, entry=target))
                    function.blocks[label].end.origin = origin
                return label
            raise lift.Unsupported(f"function {function.entry:06X}: no code at {target:06X}")

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

    def pushed(self, instruction):
        """The address a "movea back[PC], [-SP]" pushes, if it is code: where a later return comes back."""
        return self.program.continuation(instruction)

    def frame_hooks(self, statements, instruction):
        address = instruction.address
        if address == self.frame_wait_loop:
            lifter = lift.Lifter(self.mem, prefix=f"w{address:06X}_")
            counter = lifter.address_of(instruction.operands[0], 0, "frames_to_wait")
            statements += lifter.statements
            statements.append(ir.Effect("frame_wait", (counter.address,)))
        elif address == self.frame_resume:
            statements.append(ir.Effect("take_interrupts", (ir.const(address),)))
        elif address == self.frame_wait_done:
            statements.append(ir.Effect("frame_end", ()))

    def lift(self, function, block, instruction, body):
        address = instruction.address
        mnemonic = instruction.mnemonic
        following = address + instruction.length
        statements = block.statements
        self.frame_hooks(statements, instruction)
        previous = body.get(block.addresses[-2]) if len(block.addresses) > 1 else None
        back = self.pushed(previous) if previous is not None else None

        pushed = self.pushed(instruction)
        if pushed is not None:
            next_instruction = body.get(following)
            if next_instruction is None and following in self.entries:
                # pushing 'pushed' and running into another function: a call that comes back
                statements.append(ir.Call(target=following, return_address=pushed))
                block.end = ir.Goto(pushed)
                return
            if next_instruction is not None and next_instruction.mnemonic in ("br", "jmp"):
                return    # the push belongs to the call the jump makes

        lifter = lift.Lifter(self.mem, prefix=f"t{address:06X}_")
        if lift.lift_data(lifter, instruction):
            statements += lifter.statements
            return

        target = instruction.target
        if back is not None and mnemonic in ("br", "jmp") and target is not None:
            statements.append(ir.Call(target=target, return_address=back))
            block.end = ir.Goto(back)
            return
        if mnemonic == "br" or (mnemonic == "jmp" and target is not None):
            block.end = self.transfer(function.entry, target)
            return
        if mnemonic.startswith("b") and mnemonic[1:] in CONDITION_NAMES:
            block.end = ir.Branch(flag_condition(mnemonic[1:]), target, following)
            return
        if mnemonic.startswith("db"):
            counter = instruction.operands[0].reg
            statements.append(ir.SetReg(counter, ir.op("sub", (ir.Reg(counter), ir.const(1)), 32)))
            nonzero = ir.op("ne", (ir.Reg(counter), ir.const(0)), 1)
            condition = nonzero if mnemonic == "dbr" else ir.op("and", (nonzero, flag_condition(mnemonic[2:])), 1)
            block.end = ir.Branch(condition, target, following)
            return
        if mnemonic == "tb":
            counter = instruction.operands[0].reg
            block.end = ir.Branch(ir.op("eq", (ir.Reg(counter), ir.const(0)), 1), target, following)
            return
        if mnemonic == "bsr":
            statements.append(ir.Call(target=target, return_address=following))
            return
        if mnemonic == "jsr":
            self.call(lifter, instruction, following)
            statements += lifter.statements
            return
        if mnemonic == "jmp":
            self.jump(block, lifter, instruction, back)
            return
        if mnemonic == "rsr":
            landing = self.analysis.return_jumps.get(address)
            block.end = ir.Goto(landing) if landing is not None else ir.Return()
            return
        if mnemonic == "retis":
            landing = self.analysis.return_jumps.get(address)
            frame = lifter.read_value(instruction.operands[0], 1, "frame")
            statements += lifter.statements
            if landing is not None:
                # boot's way into its main part: the PSW and the address it pushed take effect
                statements.append(ir.Effect("return_from_interrupt", (frame,)))
                block.end = ir.Goto(landing)
            else:
                block.end = ir.InterruptReturn(frame)
            return
        raise lift.Unsupported(f"{address:06X}: {mnemonic}")

    def exits_caller(self, entry):
        """True for code that drops its own return address and returns: add.w #4, SP then rsr."""
        instruction = self.analysis.instructions.get(entry)
        while instruction is not None and instruction.mnemonic == "br":
            instruction = self.analysis.instructions.get(instruction.target)
        if instruction is None or instruction.mnemonic != "add.w":
            return False
        source, dest = instruction.operands
        following = self.analysis.instructions.get(instruction.address + instruction.length)
        return (source.mode == "immediate" and source.value == 4 and dest.mode == "reg" and dest.reg == 31
                and following is not None and following.mnemonic == "rsr")

    def call(self, lifter, instruction, following):
        location = lifter.address_of(instruction.operands[0], 0, "target")
        if location.register is not None:
            raise lift.Unsupported(f"{instruction.address:06X}: jsr to a register")
        choices = tuple(self.analysis.call_tables.get(instruction.address, ())) \
            or tuple(self.analysis.pointer_tables.get(instruction.address, ()))
        operand = instruction.operands[0]
        slot = operand.disp if operand.mode == "disp_indirect" else None
        if instruction.target is not None:
            lifter.emit(ir.Call(target=instruction.target, return_address=following))
        else:
            lifter.emit(ir.Call(address=location.address, choices=choices, return_address=following, slot=slot))

    def jump(self, block, lifter, instruction, back):
        location = lifter.address_of(instruction.operands[0], 0, "target")
        if location.register is not None:
            raise lift.Unsupported(f"{instruction.address:06X}: jmp to a register")
        block.statements += lifter.statements
        calls = self.analysis.call_tables.get(instruction.address)
        if back is not None and calls:
            # the pushed return address makes the table a table of calls that come back to 'back'; an entry
            # that drops that address and returns ends the function instead (seen at 0998B8)
            cases = {}
            for case in calls:
                label = ("call", case)
                if self.exits_caller(case):
                    self.extra_blocks.append(Block(label, end=ir.Return()))
                else:
                    self.extra_blocks.append(Block(label, [ir.Call(target=case, return_address=back)], ir.Goto(back)))
                cases[case] = label
            block.end = ir.Switch(location.address, cases)
            return
        table = self.analysis.jump_tables.get(instruction.address) or self.analysis.pointer_tables.get(instruction.address)
        if table:
            # a run of branches, or a table of code addresses the jump reads its target from (099C22)
            block.end = ir.Switch(location.address, {case: case for case in table})
            return
        block.end = ir.ComputedJump(location.address)
