"""
Name:        recompile.py
Description: translates the game's V60 program into C, one C function per V60 function
Version:     0.1.0
Created:     11-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

Usage: python tools/recompile.py [rom folder or .zip] --trace build/trace/executed.txt
                                 --calls build/trace/called.txt
Writes build/generated/: program.h, program_NN.c and entries.c. Nothing it writes may be
redistributed: it is the game's code, translated from the user's own ROM.

Code is followed at the addresses the CPU runs it from, not folded to one copy per ROM chip:
the same bytes reached through two mirrors give different PC-relative values, so they are two
pieces of C. Function names come from symbols.txt; unnamed functions are sub_ADDRESS until
someone works out what they do.

Instructions take no time. The board's events and the interrupts they raise run only in the game's
frame loop (frame_wait_loop in symbols.txt), as tools/reference/s32ref runs them with frame timing.
"""
import sys
from collections import Counter
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import disassemble  # noqa: E402
import orunners_rom  # noqa: E402
from v60 import codegen  # noqa: E402
from v60 import disassembler as dis  # noqa: E402
from v60 import emit_c  # noqa: E402
from v60 import lift  # noqa: E402

RESET_ADDRESS = 0xFFFFFFF0
FUNCTIONS_PER_FILE = 120
SHARED_MINIMUM = 8    # instructions: shared code shorter than this stays in every function that runs it


def key(address):
    return address & 0xFFFFFF


class Program:
    """The analysed program, split into functions ready to be written as C."""

    def __init__(self, image, symbols, executed, called):
        self.image = image
        self.mem = orunners_rom.ProgramMemory(image)
        self.symbols = symbols

        entries = [(RESET_ADDRESS, "func")]
        table = next(address for address, symbol in symbols.items() if symbol.name == "vector_table")
        vector_targets = [self.mem.r32(table + vector * 4) for vector in range(256)]
        default_handler = Counter(vector_targets).most_common(1)[0][0]
        for vector in (2, 0x40, 0x41, 0x42, 0x43, 0x44):
            entries.append((vector_targets[vector], "func"))
        entries.append((default_handler, "func"))
        entries += [(address, "func") for address in sorted(called)]
        # named functions become C functions where the trace shows them running at that very address
        entries += [(address, "func") for address, symbol in sorted(symbols.items())
                    if symbol.kind == "func" and address in executed]

        self.analysis = disassemble.Analysis(image, symbols, executed, fold_mirrors=False, entries=entries)
        self.analysis.run()
        problems = self.analysis.invalid + self.analysis.overlaps + self.analysis.trace_conflicts
        if problems:
            raise SystemExit(f"The analysis is not clean: {problems[:5]}")

        instructions = self.analysis.instructions
        self.entries = {address for address, kind in self.analysis.labels.items()
                        if kind == "func" and address in instructions}
        self.entries.add(key(RESET_ADDRESS))
        for table_entries in self.analysis.call_tables.values():
            self.entries.update(table_entries)
        # the code a table of addresses calls; the code a jump through one goes to (099C22) stays in its function
        for origin, targets in self.analysis.pointer_tables.items():
            if instructions[origin].flow == dis.FLOW_CALL:
                self.entries.update(targets)

        self.bodies = {}
        for entry in sorted(self.entries):
            self.bodies[entry] = self.walk(entry)
        # code that no function reaches (it would only run through computed jumps) gets its own entry
        covered = set().union(*self.bodies.values())
        self.orphans = sorted(address for address in instructions if address not in covered)
        while True:
            covered = set().union(*self.bodies.values())
            starts = [address for address in self.orphans if address not in covered]
            if not starts:
                break
            self.entries.add(starts[0])
            self.bodies[starts[0]] = self.walk(starts[0])
        self.split_shared_code()

        self.names = self.choose_names()
        self.frame_hooks = self.frame_loop_hooks()

    def split_shared_code(self):
        """Code that two functions both run into (an entry that falls into another's code, entries that
        share their last part) becomes a function of its own, which they reach with a tail call: it is
        written once. A shared ending shorter than SHARED_MINIMUM instructions stays copied. Where the
        shared code runs back into the code that led to it (a jump into the middle of a loop), the split
        would make the loop a chain of calls that never return, so those functions keep their own copies."""
        original = set(self.entries)
        rejected = set()
        while True:
            owners = {}
            for entry, body in self.bodies.items():
                for address in body:
                    owners.setdefault(address, set()).add(entry)
            starts = set()
            for entry, body in self.bodies.items():
                for address, instruction in body.items():
                    for successor in self.successors(instruction):
                        if successor in body and len(owners[successor]) > 1 \
                                and owners[successor] != owners[address]:
                            starts.add(successor)
            starts -= rejected
            # a shared ending of a few instructions reads better copied than called
            small = {start for start in starts if len(self.walk(start)) < SHARED_MINIMUM}
            rejected |= small
            starts -= small
            if not starts:
                return
            self.entries |= starts
            self.bodies = {entry: self.walk(entry) for entry in sorted(self.entries)}
            looping = {entry for component in self.tail_call_cycles() for entry in component} - original
            if looping:
                rejected |= looping
                self.entries -= looping
                self.bodies = {entry: self.walk(entry) for entry in sorted(self.entries)}

    def tail_call_cycles(self):
        """Groups of functions that run into each other in a circle without returning."""
        edges = {}
        for entry, body in self.bodies.items():
            for instruction in body.values():
                for successor in self.successors(instruction):
                    if successor in self.entries and successor != entry:
                        edges.setdefault(entry, set()).add(successor)
        index, low, on_stack, stack, found = {}, {}, set(), [], []
        counter = 0
        for root in sorted(self.entries):
            if root in index:
                continue
            work = [(root, iter(sorted(edges.get(root, ()))))]
            index[root] = low[root] = counter
            counter += 1
            stack.append(root)
            on_stack.add(root)
            while work:
                node, children = work[-1]
                advanced = False
                for child in children:
                    if child not in index:
                        index[child] = low[child] = counter
                        counter += 1
                        stack.append(child)
                        on_stack.add(child)
                        work.append((child, iter(sorted(edges.get(child, ())))))
                        advanced = True
                        break
                    if child in on_stack:
                        low[node] = min(low[node], index[child])
                if advanced:
                    continue
                work.pop()
                if work:
                    parent = work[-1][0]
                    low[parent] = min(low[parent], low[node])
                if low[node] == index[node]:
                    component = []
                    while True:
                        member = stack.pop()
                        on_stack.discard(member)
                        component.append(member)
                        if member == node:
                            break
                    if len(component) > 1:
                        found.append(component)
        return found

    def successors(self, instruction):
        """Addresses execution can continue at inside the same function."""
        flow = instruction.flow
        following = []
        if flow in (dis.FLOW_NORMAL, dis.FLOW_BRANCH, dis.FLOW_CALL):
            following.append(key(instruction.address + instruction.length))
        if flow == dis.FLOW_BRANCH and instruction.target is not None:
            following.append(key(instruction.target))
        if flow == dis.FLOW_JUMP:
            if instruction.address in self.analysis.push_jumps:
                following.append(self.analysis.push_jumps[instruction.address])
            elif instruction.target is not None:
                following.append(key(instruction.target))
            following += self.analysis.jump_tables.get(instruction.address, [])
            following += self.analysis.pointer_tables.get(instruction.address, [])
        if flow == dis.FLOW_RETURN and instruction.address in self.analysis.return_jumps:
            following.append(self.analysis.return_jumps[instruction.address])
        pushed = self.continuation(instruction)
        if pushed is not None:
            following.append(pushed)
        return following

    def continuation(self, instruction):
        """For "movea back[PC], [-SP]": 'back', if it was decoded as code (a place returns may land on)."""
        pushed = disassemble.Analysis.pushed_return(instruction)
        if pushed is None or key(pushed) not in self.analysis.instructions:
            return None
        return key(pushed)

    def walk(self, entry):
        body = {}
        stack = [entry]
        while stack:
            address = stack.pop()
            if address in body or (address != entry and address in self.entries):
                continue
            instruction = self.analysis.instructions.get(address)
            if instruction is None:
                raise SystemExit(f"No decoded instruction at {address:06X} (function {entry:06X})")
            body[address] = instruction
            stack += self.successors(instruction)
        return body

    def frame_loop_hooks(self):
        """C to put before instructions of the frame loop: {address: lines}."""
        named = {symbol.name: address for address, symbol in self.symbols.items()}
        for name in ("frame_wait_loop", "frame_wait_done"):
            if name not in named:
                raise SystemExit(f"symbols.txt: no '{name}'")
        loop = self.analysis.instructions.get(named["frame_wait_loop"])
        if loop is None or not loop.operands:
            raise SystemExit("symbols.txt: frame_wait_loop is not an instruction that reads the frame counter")
        block = codegen.Block(self.mem)
        counter = block.address_of(loop.operands[0], 0, "frames_to_wait")
        if counter.register is not None:
            raise SystemExit("symbols.txt: frame_wait_loop reads a register, not the frame counter")
        wait = block.lines + [f"v60_frame_wait({counter.address});"]
        resume = loop.address + loop.length
        return {
            loop.address: wait if len(wait) == 1 else ["{"] + [f"\t{line}" for line in wait] + ["}"],
            resume: [f"v60_take_interrupts({codegen.const(resume)});"],
            named["frame_wait_done"]: ["v60_frame_end();"],
        }

    def choose_names(self):
        named = {}
        for address, symbol in self.symbols.items():
            if symbol.kind in ("func", "code"):
                named[address] = symbol.name
        names = {}
        used = set()
        # functions at their canonical address keep the plain name; mirror copies get the address added
        for entry in sorted(self.entries, key=lambda a: (orunners_rom.canonical_address(a) != a, a)):
            name = named.get(orunners_rom.canonical_address(entry))
            if name is None:
                name = f"sub_{entry:06X}"
            elif name in used:
                name = f"{name}_at_{entry:06X}"
            used.add(name)
            names[entry] = name
        return names


class FunctionWriter:
    """Writes one V60 function as one C function."""

    def __init__(self, program, entry):
        self.program = program
        self.entry = entry
        self.body = program.bodies[entry]
        self.labels = set()
        # addresses this function pushes as return addresses: a return may come back to them
        self.continuations = sorted({address for address in
                                     (program.continuation(instruction) for instruction in self.body.values())
                                     if address is not None and address in self.body})

    def resume(self):
        """C, one line, for after a return or a tail call: back to a pushed continuation, or out."""
        checks = []
        for address in self.continuations:
            self.labels.add(address)
            checks.append(f"if (cpu.pc == {codegen.const(address)}) goto L_{address:06X};")
        return " ".join(checks + ["return;"])

    def transfer(self, target):
        """C that sends execution to 'target': a goto inside this function, or a tail call."""
        target = key(target)
        if target != self.entry and target in self.program.entries:
            return f"{{ {self.program.names[target]}(); {self.resume()} }}"
        if target not in self.body:
            raise codegen.Unsupported(f"jump to {target:06X}, outside function {self.entry:06X}")
        self.labels.add(target)
        return f"goto L_{target:06X};"

    def call(self, target_expression, static_target, next_address, address):
        if static_target is not None and key(static_target) in self.program.entries:
            invoke = f"{self.program.names[key(static_target)]}();"
        else:
            invoke = f"v60_call({target_expression});"
        return [f"push32({codegen.const(next_address)});", invoke,
                f"v60_check_return({codegen.const(next_address)}, {codegen.const(address)});"]

    def instruction_lines(self, instruction):
        block = codegen.Block(self.program.mem)
        address = instruction.address
        mnemonic = instruction.mnemonic
        next_address = address + instruction.length
        lines = []

        pushed = self.program.analysis.push_jumps.get(address)
        lifter = lift.Lifter(self.program.mem)
        if lift.lift_data(lifter, instruction):
            printer = emit_c.Printer()
            lines = [printer.statement(statement) for statement in lifter.statements]
            if not lines:
                return ["/* nothing to do */"]
        elif pushed is not None and mnemonic in ("br", "jmp"):
            # the code pushed its own return address before jumping: a call that comes back to 'pushed'
            return [f"{self.program.names[key(instruction.target)]}();",
                    f"v60_check_return({codegen.const(pushed)}, {codegen.const(address)});",
                    self.transfer(pushed)]
        elif mnemonic == "br":
            return [self.transfer(instruction.target)]
        elif mnemonic in codegen.CONDITIONS:
            return [f"if ({codegen.CONDITIONS[mnemonic]})", f"\t{self.transfer(instruction.target)}"]
        elif mnemonic in codegen.DECREMENT_CONDITIONS:
            counter = codegen.reg(instruction.operands[0].reg)
            condition = codegen.DECREMENT_CONDITIONS[mnemonic]
            test = f"{counter} != 0" if condition is None else f"{counter} != 0 && {condition}"
            return [f"{counter} -= 1;", f"if ({test})", f"\t{self.transfer(instruction.target)}"]
        elif mnemonic == "tb":
            counter = codegen.reg(instruction.operands[0].reg)
            return [f"if ({counter} == 0)", f"\t{self.transfer(instruction.target)}"]
        elif mnemonic == "bsr":
            lines = self.call(None, instruction.target, next_address, address)
        elif mnemonic == "jsr":
            location = block.address_of(instruction.operands[0], 0, "target")
            if location.register is not None:
                raise codegen.Unsupported("jsr to a register")
            lines = block.lines + self.call(location.address, instruction.target, next_address, address)
        elif mnemonic == "jmp":
            return self.jump(block, instruction)
        elif mnemonic == "rsr":
            return ["cpu.pc = pop32();"] + self.leave(instruction)
        elif mnemonic == "retis":
            frame = block.read_value(instruction.operands[0], 1, "frame")
            block.emit("cpu.pc = pop32();")
            block.temp("const uint32_t", "psw", "pop32()")
            block.emit(f"SP += {frame};")
            block.emit("v60_write_psw(psw);")
            return ["{"] + [f"\t{line}" for line in block.lines] + ["}"] + self.leave(instruction)
        else:
            raise codegen.Unsupported(f"instruction {mnemonic}")

        if len(lines) == 1:
            return lines
        return ["{"] + [f"\t{line}" for line in lines] + ["}"]

    def jump(self, block, instruction):
        address = instruction.address
        if instruction.target is not None:
            return [self.transfer(instruction.target)]
        location = block.address_of(instruction.operands[0], 0, "target")
        if location.register is not None:
            raise codegen.Unsupported("jmp to a register")
        lines = ["{"] + [f"\t{line}" for line in block.lines]
        table = self.program.analysis.jump_tables.get(address)
        if table:
            lines.append(f"\tswitch ({location.address})")
            lines.append("\t{")
            for entry in table:
                lines.append(f"\t\tcase {codegen.const(entry)}: {self.transfer(entry)}")
            lines.append(f"\t\tdefault: v60_unknown_jump({location.address}, {codegen.const(address)}); return;")
            lines.append("\t}")
        else:
            lines.append(f"\tv60_call({location.address});")
            lines.append(f"\t{self.resume()}")
        lines.append("}")
        return lines

    def leave(self, instruction):
        """After a return: back to the caller, or on to the address the code pushed for itself."""
        target = self.program.analysis.return_jumps.get(instruction.address)
        if target is None:
            return [self.resume()]
        return [f"if (cpu.pc != {codegen.const(target)})",
                f"\tv60_unknown_jump(cpu.pc, {codegen.const(instruction.address)});",
                self.transfer(target)]

    def write(self):
        order = [self.entry] + sorted(address for address in self.body if address != self.entry)
        chunks = []
        for position, address in enumerate(order):
            instruction = self.body[address]
            lines = [f"/* {address:06X}  {instruction.text} */"]
            lines += self.program.frame_hooks.get(address, [])
            lines += self.instruction_lines(instruction)
            if instruction.flow in (dis.FLOW_NORMAL, dis.FLOW_BRANCH, dis.FLOW_CALL):
                following = key(address + instruction.length)
                upcoming = order[position + 1] if position + 1 < len(order) else None
                if following != upcoming or following in self.program.entries:
                    lines.append(self.transfer(following))
            chunks.append((address, lines))

        text = [f"void {self.program.names[self.entry]}(void)", "{"]
        for address, lines in chunks:
            if address in self.labels:
                text.append(f"L_{address:06X}:")
            text += [f"\t{line}" for line in lines]
        text.append("}")
        return "\n".join(text)


HEADER = """/*
 * Generated by tools/recompile.py from the user's own ROM. Do not redistribute.
 * SegaSonic The Hedgehog (sonic, Japan rev. C): V60 program translated to C.
 */
"""


def main():
    arguments = sys.argv[1:]
    options = {"--trace": [], "--calls": []}
    for option in options:
        while option in arguments:
            position = arguments.index(option)
            options[option].append(arguments[position + 1])
            del arguments[position:position + 2]
    source = Path(arguments[0]) if arguments else ROOT / "rom"

    image = orunners_rom.build_program(orunners_rom.load_set(source))
    symbols = disassemble.load_symbols(ROOT / "symbols.txt")
    executed, _ = disassemble.load_trace(options["--trace"], fold_mirrors=False)
    called, _ = disassemble.load_trace(options["--calls"], fold_mirrors=False)
    program = Program(image, symbols, executed, called)

    out_dir = ROOT / "build" / "generated"
    out_dir.mkdir(parents=True, exist_ok=True)
    for old in out_dir.glob("program_*.c"):
        old.unlink()

    entries = sorted(program.entries)
    prototypes = [f"void {program.names[entry]}(void);" for entry in entries]
    (out_dir / "program.h").write_text(
        HEADER + "#ifndef SONIC_PROGRAM_H\n#define SONIC_PROGRAM_H\n\n" + "\n".join(prototypes) + "\n\n#endif\n",
        encoding="ascii")

    failures = []
    for number, first in enumerate(range(0, len(entries), FUNCTIONS_PER_FILE)):
        functions = []
        for entry in entries[first:first + FUNCTIONS_PER_FILE]:
            try:
                functions.append(FunctionWriter(program, entry).write())
            except codegen.Unsupported as problem:
                failures.append((entry, str(problem)))
        text = HEADER + '#include "v60/registers.h"\n#include "v60/ops.h"\n#include "v60/arith.h"\n#include "program.h"\n\n'
        (out_dir / f"program_{number:02}.c").write_text(text + "\n\n".join(functions) + "\n", encoding="ascii")

    table = [f"\t{{ {codegen.const(entry)}, {program.names[entry]} }}," for entry in entries]
    (out_dir / "entries.c").write_text(
        HEADER + '#include "v60/code.h"\n#include "program.h"\n\nconst v60_code_entry v60_code_entries[] =\n{\n'
        + "\n".join(table) + f"\n}};\n\nconst size_t v60_code_entry_count = {len(entries)};\n", encoding="ascii")

    instructions = sum(len(body) for body in program.bodies.values())
    print(f"functions:      {len(entries)}")
    print(f"instructions:   {instructions} written ({len(program.analysis.instructions)} distinct)")
    print(f"orphan starts:  {len(program.orphans)}")
    print(f"not translated: {len(failures)}")
    for entry, problem in failures[:20]:
        print(f"  {entry:06X}  {problem}")
    print(f"Output: {out_dir}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
