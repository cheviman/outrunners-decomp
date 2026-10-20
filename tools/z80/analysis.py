# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        analysis.py
Description: the Z80 sound program, found from its entry points and split into functions
Version:     0.1.0
Created:     13-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

Starts at the reset (0000) and at the interrupt handler (0038: the board's Z80 runs in interrupt mode
1, and nothing is wired to its NMI), follows every jump, branch and call, and through the jump tables
sound_symbols.txt describes. Every call target is a function, as is every "func" line; a function's
body is the code reachable from its entry without going through another function's entry (running
into one is a tail call). The program lives in 0000-9FFF: the banked page at A000-BFFF only ever
holds data. Code the reset and the interrupt never reach (routines nothing calls) comes in through its
"func" lines: such a function is in 'unreached'.

Program has the attributes the decompiler takes from tools/recompile.py's: entries, bodies, names,
symbols, mem, successors(), and an analysis with instructions and jump_tables; and 'reached', the
addresses the program runs, and 'unreached', the functions outside them.
"""
from dataclasses import dataclass, field
from pathlib import Path

from . import disassembler as dis

ENTRIES = {0x0000: "func", 0x0038: "func"}
CODE_END = 0xA000
KINDS = ("func", "code", "loop", "data", "jumps", "via")


@dataclass
class Symbol:
    kind: str
    name: str
    count: int = 0          # jumps: entries in the table
    table: str = ""         # via: the table the jump uses


def load_symbols(path):
    """sound_symbols.txt: {address: Symbol}. Lines of other kinds (names for the decompiler) are skipped."""
    symbols = {}
    for number, line in enumerate(Path(path).read_text(encoding="utf-8").splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        if len(parts) < 3 or parts[1] not in KINDS:
            continue
        where = f"{Path(path).name}:{number}"
        try:
            address = int(parts[0], 16)
        except ValueError:
            raise SystemExit(f"{where}: '{parts[0]}' is not an address") from None
        if parts[1] == "jumps":
            if len(parts) != 4 or not parts[3].isdigit():
                raise SystemExit(f"{where}: expected 'ADDRESS jumps NAME COUNT'")
            symbols[address] = Symbol("jumps", parts[2], count=int(parts[3]))
        elif parts[1] == "via":
            symbols[address] = Symbol("via", f"via_{address:04X}", table=parts[2])
        else:
            symbols[address] = Symbol(parts[1], parts[2])
    return symbols


@dataclass
class Analysis:
    instructions: dict = field(default_factory=dict)     # address -> dis.Instruction
    jump_tables: dict = field(default_factory=dict)      # address of a JP (rr) -> its targets
    call_tables: dict = field(default_factory=dict)
    pointer_tables: dict = field(default_factory=dict)
    return_jumps: dict = field(default_factory=dict)
    push_jumps: dict = field(default_factory=dict)
    pointer_writes: list = field(default_factory=list)
    described_targets: dict = field(default_factory=dict)


class Program:
    """The analysed sound program, split into functions."""

    def __init__(self, image, symbols):
        self.image = image
        self.mem = dis.FlatMemory(image, 0xFFFF)
        self.symbols = symbols
        self.analysis = Analysis()
        tables = {symbol.name: address for address, symbol in symbols.items() if symbol.kind == "jumps"}
        for address, symbol in symbols.items():
            if symbol.kind != "via":
                continue
            if symbol.table not in tables:
                raise SystemExit(f"sound_symbols.txt: {address:04X} uses '{symbol.table}', which is not a jumps table")
            start = tables[symbol.table]
            count = symbols[start].count
            self.analysis.jump_tables[address] = list(dict.fromkeys(self.mem.r16(start + 2 * index)
                                                                    for index in range(count)))

        self.entries = set(ENTRIES) | {address for address, symbol in symbols.items() if symbol.kind in ("func", "loop")}
        self.decode_all()
        self.entries |= {instruction.target for instruction in self.analysis.instructions.values()
                         if instruction.flow == dis.FLOW_CALL and instruction.target is not None}
        # code nothing reaches from the reset and the interrupt is decompiled too, from its "func" lines
        self.reached = self.reachable()
        self.unreached = {entry for entry in self.entries if entry not in self.reached}
        self.bodies = {entry: self.walk(entry) for entry in sorted(self.entries)}
        self.split_shared_code()
        self.names = self.choose_names()

    def reachable(self):
        """The addresses the program runs from the reset and the interrupt handler: every jump, branch, call
        and jump table followed."""
        found = set()
        work = list(ENTRIES)
        while work:
            address = work.pop()
            if address in found:
                continue
            found.add(address)
            instruction = self.analysis.instructions[address]
            work += self.successors(instruction)
            if instruction.flow == dis.FLOW_CALL and instruction.target is not None:
                work.append(instruction.target)
        return found

    def split_shared_code(self):
        """Code two functions both run into (one jumps into the middle of the other) becomes a function of
        its own, which both reach with a tail call: it is written once (also when one of them is code the
        program never reaches)."""
        while True:
            owners = {}
            for entry, body in self.bodies.items():
                for address in body:
                    owners.setdefault(address, set()).add(entry)
            starts = {successor for body in self.bodies.values() for address, instruction in body.items()
                      for successor in self.successors(instruction)
                      if successor in body and len(owners[successor]) > 1 and owners[successor] != owners[address]}
            if not starts:
                return
            self.entries |= starts
            self.bodies = {entry: self.walk(entry) for entry in sorted(self.entries)}

    def decode_all(self):
        work = sorted(self.entries)
        instructions = self.analysis.instructions
        while work:
            address = work.pop()
            while address not in instructions:
                if address >= CODE_END:
                    raise SystemExit(f"code at {address:04X}, outside the program")
                instruction = dis.decode(self.mem, address)
                if instruction.flow == dis.FLOW_INVALID:
                    raise SystemExit(f"{address:04X}: not an instruction")
                instructions[address] = instruction
                if instruction.target is not None and instruction.flow in (dis.FLOW_CALL, dis.FLOW_JUMP, dis.FLOW_BRANCH):
                    work.append(instruction.target)
                work += self.analysis.jump_tables.get(address, [])
                if instruction.flow == dis.FLOW_JUMP and instruction.target is None \
                        and address not in self.analysis.jump_tables:
                    raise SystemExit(f"{address:04X}: {instruction.text} goes where sound_symbols.txt does not say")
                if instruction.flow in (dis.FLOW_JUMP, dis.FLOW_RETURN):
                    break
                address = (address + instruction.length) & 0xFFFF
        # an instruction that starts inside another one
        for address, instruction in instructions.items():
            for inside in range(address + 1, address + instruction.length):
                if inside in instructions:
                    raise SystemExit(f"{inside:04X} is inside the instruction at {address:04X}")

    def successors(self, instruction):
        """Addresses execution can continue at inside the same function."""
        following = []
        flow = instruction.flow
        after = (instruction.address + instruction.length) & 0xFFFF
        if flow in (dis.FLOW_NORMAL, dis.FLOW_BRANCH, dis.FLOW_CALL):
            following.append(after)
        if flow in (dis.FLOW_BRANCH, dis.FLOW_JUMP) and instruction.target is not None:
            following.append(instruction.target)
        following += self.analysis.jump_tables.get(instruction.address, [])
        return following

    def walk(self, entry):
        body = {}
        stack = [entry]
        while stack:
            address = stack.pop()
            if address in body or (address != entry and address in self.entries):
                continue
            body[address] = self.analysis.instructions[address]
            stack += self.successors(body[address])
        return body

    def choose_names(self):
        named = {address: symbol.name for address, symbol in self.symbols.items() if symbol.kind in ("func", "code", "loop")}
        return {entry: named.get(entry, f"sound_{entry:04X}") for entry in sorted(self.entries)}


def load_program(rom_image, symbols_path):
    """The sound program from the first 64 KB of the sound region."""
    return Program(rom_image[:0x10000], load_symbols(symbols_path))
