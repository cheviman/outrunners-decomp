# license:BSD-3-Clause
# copyright-holders:Juergen Buchmueller, Sebastian Toyos
"""
Name:        disassembler.py
Description: Zilog Z80 instruction decoder with structured operands
Version:     0.1.0
Created:     13-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

Ported from MAME src/devices/cpu/z80/z80dasm.cpp. The text of every instruction matches MAME's
output exactly: tools/reference/compare_z80_with_mame.py builds the original and diffs both, over
the sound program and over every opcode of every prefix. On top of the text, every instruction
carries its decoded operands and control flow, which is what the tools that analyze the sound
program work with.

The tables keep MAME's argument templates. In them A is a jump or call address, B an 8-bit
immediate, N a 16-bit immediate, O the target of a relative jump, P an I/O port number, V a
restart vector, W a memory address, I the index register (ix or iy), X the index register plus a
displacement that follows the opcode, and Y the same with the displacement read before the opcode
(DDCB and FDCB instructions). Everything else is printed as is.

Control flow:
- JP, JR and CALL with a condition (nz, z, nc, c, po, pe, p, m) carry it in Instruction.condition.
  A conditional jump is a branch; DJNZ is a branch too, with no condition code (it tests B).
- A conditional call is still a call: execution comes back to the next instruction either way.
- RET cc is a branch-like return: flow FLOW_BRANCH, no target, mnemonic 'ret' and its condition.
  It either falls through or leaves the routine, so a walker follows the next instruction and
  treats the taken side as a return. RET, RETI and RETN are FLOW_RETURN.
- RST is a call to its vector. JP (HL), JP (IX) and JP (IY) are jumps with no static target.
- HALT is normal: the Z80 resumes at the next instruction once an interrupt has been served.
"""
from dataclasses import dataclass, field

# How execution continues after an instruction (the same values as tools/v60/disassembler.py)
FLOW_NORMAL = "normal"
FLOW_JUMP = "jump"            # unconditional, never falls through
FLOW_BRANCH = "branch"        # falls through or goes to the target (RET cc: falls through or returns)
FLOW_CALL = "call"            # goes to the target and comes back to the next instruction
FLOW_RETURN = "return"
FLOW_INVALID = "invalid"

MASK16 = 0xFFFF

REGISTERS_8 = ("b", "c", "d", "e", "h", "l", "(hl)", "a")
CONDITIONS = ("nz", "z", "nc", "c", "po", "pe", "p", "m")
CONDITIONAL_MNEMONICS = ("jp", "jr", "call", "ret")

# Names an operand can have once the template is expanded
REGISTER_NAMES = {
    "a", "b", "c", "d", "e", "h", "l", "i", "r", "ixh", "ixl", "iyh", "iyl",
    "af", "af'", "bc", "de", "hl", "sp", "ix", "iy",
}
REGISTER_INDIRECT = {"(bc)", "(de)", "(hl)", "(sp)", "(ix)", "(iy)"}
# Template letters followed by a 16-bit word, and the operand they make
WORD_MODES = {"A": "address", "N": "immediate", "W": "direct"}

FLOWS = {
    "jp": FLOW_JUMP, "jr": FLOW_JUMP, "djnz": FLOW_BRANCH, "call": FLOW_CALL, "rst": FLOW_CALL,
    "ret": FLOW_RETURN, "reti": FLOW_RETURN, "retn": FLOW_RETURN,
}


@dataclass(slots=True)
class Operand:
    """A decoded operand. Modes:
    reg           a register: a-l, i, r, ixh-iyl, af, af', bc, de, hl, sp, ix, iy ('reg')
    reg_indirect  memory at a register pair: (bc), (de), (hl), (sp); for JP (HL)/(IX)/(IY), the
                  register itself is the target ('reg')
    indexed       memory at ix or iy plus a signed displacement ('reg', 'disp')
    immediate     a constant: 8 or 16 bits, a bit number, an interrupt mode, the 0 of OUT (C),0 ('value')
    direct        memory at a fixed address ('value')
    port          an I/O port given as a constant ('value')
    port_c        the I/O port in register C; the Z80 puts B on the upper address lines ('reg')
    address       the target of JP, JR, DJNZ, CALL or RST, a 16-bit Z80 address ('value')
    """
    mode: str
    text: str
    reg: str = ""
    disp: int = 0
    value: int = 0


@dataclass(slots=True)
class Instruction:
    address: int
    length: int
    mnemonic: str
    text: str
    flow: str
    operands: list = field(default_factory=list)
    target: int | None = None       # fixed target of a jump, branch or call
    condition: str | None = None    # condition code of JP, JR, CALL or RET cc
    copy_to: str | None = None      # DDCB/FDCB: register that also receives the result ('b=' in MAME)


class FlatMemory:
    """Little-endian reads from a byte block; addresses are wrapped with a mask (0xFFFF: the Z80 view)."""

    def __init__(self, data, mask):
        self.data = data
        self.mask = mask

    def r8(self, address):
        return self.data[address & self.mask]

    def r16(self, address):
        return self.r8(address) | (self.r8(address + 1) << 8)


def _s8(value):
    return value - 0x100 if value & 0x80 else value


def _split(entries):
    """'mnemonic arguments' -> (mnemonic, argument template or None); unassigned entries stay None."""
    table = []
    for entry in entries:
        if entry is None:
            table.append(None)
        elif " " in entry:
            table.append(tuple(entry.split(" ", 1)))
        else:
            table.append((entry, None))
    return table


# Arithmetic and logic on A, opcodes 0x80-0xBF: the operation is in bits 3-5, the register in bits 0-2
ALU = ("add a,", "adc a,", "sub ", "sbc a,", "and ", "xor ", "or ", "cp ")
# CB prefix: rotations and shifts in 0x00-0x3F, then bit, res and set
SHIFTS = ("rlc", "rrc", "rl", "rr", "sla", "sra", "sll", "srl")
BIT_OPERATIONS = ("bit", "res", "set")

# Unprefixed opcodes. CB, DD, ED and FD are prefixes and never reach the table.
_main = [None] * 256
_main[0x00:0x40] = [
    "nop", "ld bc,N", "ld (bc),a", "inc bc", "inc b", "dec b", "ld b,B", "rlca",
    "ex af,af'", "add hl,bc", "ld a,(bc)", "dec bc", "inc c", "dec c", "ld c,B", "rrca",
    "djnz O", "ld de,N", "ld (de),a", "inc de", "inc d", "dec d", "ld d,B", "rla",
    "jr O", "add hl,de", "ld a,(de)", "dec de", "inc e", "dec e", "ld e,B", "rra",
    "jr nz,O", "ld hl,N", "ld (W),hl", "inc hl", "inc h", "dec h", "ld h,B", "daa",
    "jr z,O", "add hl,hl", "ld hl,(W)", "dec hl", "inc l", "dec l", "ld l,B", "cpl",
    "jr nc,O", "ld sp,N", "ld (W),a", "inc sp", "inc (hl)", "dec (hl)", "ld (hl),B", "scf",
    "jr c,O", "add hl,sp", "ld a,(W)", "dec sp", "inc a", "dec a", "ld a,B", "ccf",
]
for _op in range(0x40, 0x80):
    _main[_op] = f"ld {REGISTERS_8[(_op >> 3) & 7]},{REGISTERS_8[_op & 7]}"
_main[0x76] = "halt"
for _op in range(0x80, 0xC0):
    _main[_op] = ALU[(_op >> 3) & 7] + REGISTERS_8[_op & 7]
_main[0xC0:0x100] = [
    "ret nz", "pop bc", "jp nz,A", "jp A", "call nz,A", "push bc", "add a,B", "rst V",
    "ret z", "ret", "jp z,A", None, "call z,A", "call A", "adc a,B", "rst V",
    "ret nc", "pop de", "jp nc,A", "out (P),a", "call nc,A", "push de", "sub B", "rst V",
    "ret c", "exx", "jp c,A", "in a,(P)", "call c,A", None, "sbc a,B", "rst V",
    "ret po", "pop hl", "jp po,A", "ex (sp),hl", "call po,A", "push hl", "and B", "rst V",
    "ret pe", "jp (hl)", "jp pe,A", "ex de,hl", "call pe,A", None, "xor B", "rst V",
    "ret p", "pop af", "jp p,A", "di", "call p,A", "push af", "or B", "rst V",
    "ret m", "ld sp,hl", "jp m,A", "ei", "call m,A", None, "cp B", "rst V",
]
MAIN = _split(_main)

# CB prefix
_cb = []
for _op in range(256):
    _register = REGISTERS_8[_op & 7]
    if _op < 0x40:
        _cb.append(f"{SHIFTS[_op >> 3]} {_register}")
    else:
        _cb.append(f"{BIT_OPERATIONS[(_op >> 6) - 1]} {(_op >> 3) & 7},{_register}")
CB = _split(_cb)

# DDCB/FDCB: always on (ix+d)/(iy+d); register fields other than (hl) also copy the result to that
# register, which MAME prints as 'b=' (it does the same for BIT, which writes nothing)
_indexed_cb = []
for _op in range(256):
    _copy = "" if _op & 7 == 6 else REGISTERS_8[_op & 7] + "="
    if _op < 0x40:
        _indexed_cb.append(f"{SHIFTS[_op >> 3]} {_copy}Y")
    else:
        _indexed_cb.append(f"{BIT_OPERATIONS[(_op >> 6) - 1]} {_copy}{(_op >> 3) & 7},Y")
INDEXED_CB = _split(_indexed_cb)

# ED prefix; '*' marks the undocumented copies of NEG
_ed = [None] * 256
_ed[0x40:0x80] = [
    "in b,(c)", "out (c),b", "sbc hl,bc", "ld (W),bc", "neg", "retn", "im 0", "ld i,a",
    "in c,(c)", "out (c),c", "adc hl,bc", "ld bc,(W)", "neg *", "reti", "im 0", "ld r,a",
    "in d,(c)", "out (c),d", "sbc hl,de", "ld (W),de", "neg *", "retn", "im 1", "ld a,i",
    "in e,(c)", "out (c),e", "adc hl,de", "ld de,(W)", "neg *", "reti", "im 2", "ld a,r",
    "in h,(c)", "out (c),h", "sbc hl,hl", "ld (W),hl", "neg *", "retn", "im 0", "rrd (hl)",
    "in l,(c)", "out (c),l", "adc hl,hl", "ld hl,(W)", "neg *", "reti", "im 0", "rld (hl)",
    "in 0,(c)", "out (c),0", "sbc hl,sp", "ld (W),sp", "neg *", "retn", "im 1", None,
    "in a,(c)", "out (c),a", "adc hl,sp", "ld sp,(W)", "neg *", "reti", "im 2", None,
]
_ed[0xA0:0xA4] = ["ldi", "cpi", "ini", "outi"]
_ed[0xA8:0xAC] = ["ldd", "cpd", "ind", "outd"]
_ed[0xB0:0xB4] = ["ldir", "cpir", "inir", "otir"]
_ed[0xB8:0xBC] = ["lddr", "cpdr", "indr", "otdr"]
ED = _split(_ed)

# DD/FD prefix: the opcodes that use HL, H, L or (HL), with ix/iy in their place
_indexed = [None] * 256
for _op, _text in (
        (0x09, "add I,bc"), (0x19, "add I,de"), (0x21, "ld I,N"), (0x22, "ld (W),I"), (0x23, "inc I"),
        (0x24, "inc Ih"), (0x25, "dec Ih"), (0x26, "ld Ih,B"), (0x29, "add I,I"), (0x2A, "ld I,(W)"),
        (0x2B, "dec I"), (0x2C, "inc Il"), (0x2D, "dec Il"), (0x2E, "ld Il,B"), (0x34, "inc X"),
        (0x35, "dec X"), (0x36, "ld X,B"), (0x39, "add I,sp"), (0xE1, "pop I"), (0xE3, "ex (sp),I"),
        (0xE5, "push I"), (0xE9, "jp (I)"), (0xF9, "ld sp,I")):
    _indexed[_op] = _text
_halves = {"h": "Ih", "l": "Il"}
for _op in range(0x40, 0x80):
    _dest, _source = REGISTERS_8[(_op >> 3) & 7], REGISTERS_8[_op & 7]
    if _source == "(hl)" and _dest != "(hl)":
        _indexed[_op] = f"ld {_dest},X"         # with (ix+d), h and l are still h and l
    elif _dest == "(hl)" and _source != "(hl)":
        _indexed[_op] = f"ld X,{_source}"
    elif _dest in _halves or _source in _halves:
        _indexed[_op] = f"ld {_halves.get(_dest, _dest)},{_halves.get(_source, _source)}"
for _op in range(0x80, 0xC0):
    _source = {"h": "Ih", "l": "Il", "(hl)": "X"}.get(REGISTERS_8[_op & 7])
    if _source:
        _indexed[_op] = ALU[(_op >> 3) & 7] + _source
INDEXED = _split(_indexed)


def _literal_operand(text):
    """Operand for a template piece with no bytes behind it; None for a mark like the '*' of NEG."""
    if text in REGISTER_NAMES:
        return Operand("reg", text, reg=text)
    if text in REGISTER_INDIRECT:
        return Operand("reg_indirect", text, reg=text[1:-1])
    if text == "(c)":
        return Operand("port_c", text, reg="c")
    if text.isdigit():
        return Operand("immediate", text, value=int(text))
    return None


def _expand(mem, instruction, template, pos, opcode, index, displacement):
    """Expand an argument template the way MAME does, left to right, reading the operand bytes at pos.
    Fills in the operands, the condition and the copy register; returns (text, address after the operands)."""
    pieces = []
    for piece in template.split(","):
        copy = ""
        if len(piece) > 2 and piece[1] == "=":
            copy, piece = piece[:2], piece[2:]
        text = ""
        mode = None
        value = 0
        for char in piece:
            if char in WORD_MODES:
                value = mem.r8(pos) | (mem.r8(pos + 1) << 8)
                pos += 2
                text += f"${value:04X}"
                mode = WORD_MODES[char]
            elif char in "BP":
                value = mem.r8(pos)
                pos += 1
                text += f"${value:02X}"
                mode = "immediate" if char == "B" else "port"
            elif char == "O":
                value = (instruction.address + _s8(mem.r8(pos)) + 2) & MASK16
                pos += 1
                text += f"${value:04X}"
                mode = "address"
            elif char == "V":
                value = opcode & 0x38
                text += f"${value:02X}"
                mode = "address"
            elif char in "XY":
                if char == "X":
                    displacement = _s8(mem.r8(pos))
                    pos += 1
                sign = "-" if displacement < 0 else "+"
                text += f"({index}{sign}${abs(displacement):02x})"
                mode = "indexed"
            elif char == "I":
                text += index
            else:
                text += char

        if mode is None:
            operand = None
            if text in CONDITIONS and instruction.mnemonic in CONDITIONAL_MNEMONICS:
                instruction.condition = text
            else:
                operand = _literal_operand(text)
        elif mode == "indexed":
            operand = Operand("indexed", text, reg=index, disp=displacement)
        else:
            operand = Operand(mode, text, value=value)
        if operand is not None:
            instruction.operands.append(operand)
        if copy and instruction.mnemonic != "bit":
            instruction.copy_to = copy[0]
        pieces.append(copy + text)
    return ",".join(pieces), pos


def decode(mem, address):
    """Decode the instruction at 'address'. Never raises: undecodable bytes come back as FLOW_INVALID.

    The bytes are read from mem at address, address + 1 and so on, unmasked: the memory object maps
    them (FlatMemory with mask 0xFFFF wraps them as the Z80 does). Targets are 16-bit Z80 addresses.
    Like MAME, an ED, DD or FD prefix that does not start a known instruction comes back alone as
    'db $ed' (one byte long), and decoding goes on at the next byte.
    """
    opcode = mem.r8(address)
    pos = address + 1
    index = ""
    displacement = 0

    if opcode == 0xCB:
        opcode = mem.r8(pos)
        pos += 1
        entry = CB[opcode]
    elif opcode == 0xED:
        entry = ED[mem.r8(pos)]
        pos += 1
    elif opcode in (0xDD, 0xFD):
        index = "ix" if opcode == 0xDD else "iy"
        second = mem.r8(pos)
        pos += 1
        if second == 0xCB:
            # DD CB d op: the displacement comes before the opcode
            displacement = _s8(mem.r8(pos))
            entry = INDEXED_CB[mem.r8(pos + 1)]
            pos += 2
        else:
            entry = INDEXED[second]
    else:
        entry = MAIN[opcode]

    if entry is None:
        text = f"${opcode:02x}"
        operands = [Operand("immediate", text, value=opcode)]
        return Instruction(address, 1, "db", f"db   {text}", FLOW_INVALID, operands)

    mnemonic, template = entry
    instruction = Instruction(address, 0, mnemonic, mnemonic, FLOWS.get(mnemonic, FLOW_NORMAL))
    if template is not None:
        arguments, pos = _expand(mem, instruction, template, pos, opcode, index, displacement)
        instruction.text = f"{mnemonic:<4} {arguments}"
    instruction.length = pos - address

    if instruction.condition and instruction.flow in (FLOW_JUMP, FLOW_RETURN):
        instruction.flow = FLOW_BRANCH
    if instruction.flow in (FLOW_JUMP, FLOW_BRANCH, FLOW_CALL):
        destination = instruction.operands[-1] if instruction.operands else None
        if destination is not None and destination.mode == "address":
            instruction.target = destination.value
    return instruction
