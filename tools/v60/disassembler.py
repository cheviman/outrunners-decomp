# license:BSD-3-Clause
# copyright-holders:Farfetch'd, R. Belmont, Sebastian Toyos
"""
Name:        disassembler.py
Description: NEC V60 instruction decoder with structured operands
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

Ported from MAME src/devices/cpu/v60/v60d.cpp. The text of every instruction matches MAME's
output exactly: tools/reference/compare_with_mame.py builds the original and diffs both.
On top of the text, every instruction carries its decoded operands and control flow,
which is what the analyzer and the C generator work with.
"""
from dataclasses import dataclass, field

REGISTER_NAMES = [
    "R0", "R1", "R2", "R3",
    "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11",
    "R12", "R13", "R14", "R15",
    "R16", "R17", "R18", "R19",
    "R20", "R21", "R22", "R23",
    "R24", "R25", "R26", "R27",
    "R28", "AP", "FP", "SP",
    "PC", "PSW", "Unk", "Unk",
    "ISP", "L0SP", "L1SP", "L2SP",
    "L3SP", "SBR", "TR", "SYCW",
    "TKCW", "PIR", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "PSW2",
    "ATBR0", "ATLR0", "ATBR1", "ATLR1",
    "ATBR2", "ATLR2", "ATBR3", "ATLR3",
    "TRMODE", "ADTR0", "ADTR1", "ADTMR0",
    "ADTMR1", "Reserved", "Reserved", "Reserved",
]

MASK32 = 0xFFFFFFFF

# How execution continues after an instruction
FLOW_NORMAL = "normal"
FLOW_JUMP = "jump"            # unconditional, never falls through
FLOW_BRANCH = "branch"        # falls through or goes to the target
FLOW_CALL = "call"            # goes to the target and comes back to the next instruction
FLOW_RETURN = "return"
FLOW_INVALID = "invalid"


@dataclass(slots=True)
class Operand:
    """A decoded operand. 'value' holds the resolved address for PC-relative modes,
    the absolute address for direct modes and the number for immediates."""
    mode: str
    size: int
    text: str
    reg: int = -1
    index_reg: int = -1
    disp: int = 0
    outer_disp: int = 0
    value: int = 0


@dataclass(slots=True)
class Instruction:
    address: int
    length: int
    mnemonic: str
    text: str
    flow: str
    operands: list = field(default_factory=list)
    target: int | None = None           # fixed target of a branch, jump or call
    target_pointer: int | None = None   # memory address the target is read from (indirect jump)


class FlatMemory:
    """Little-endian reads from a byte block; addresses are wrapped with a mask."""

    def __init__(self, data, mask):
        self.data = data
        self.mask = mask

    def r8(self, address):
        return self.data[address & self.mask]

    def r16(self, address):
        return self.r8(address) | (self.r8(address + 1) << 8)

    def r32(self, address):
        return self.r16(address) | (self.r16(address + 2) << 16)


def _s8(value):
    return value - 0x100 if value & 0x80 else value


def _s16(value):
    return value - 0x10000 if value & 0x8000 else value


def _s32(value):
    return value - 0x100000000 if value & 0x80000000 else value


def _signed_hex(value):
    return "-" + format(-value, "X") if value < 0 else format(value, "X")


def _immediate_text(value, size):
    if size == 0:
        value &= 0xFF
    elif size == 1:
        value &= 0xFFFF
    return "#" + format(value, "X")


def _read_disp(mem, address, width):
    """width 0/1/2 = 8/16/32 bits. Returns (signed displacement, bytes read)."""
    if width == 0:
        return _s8(mem.r8(address)), 1
    if width == 1:
        return _s16(mem.r16(address)), 2
    return _s32(mem.r32(address)), 4


def decode_operand(mem, ipc, pc, m, size):
    """Decode one addressing mode (MAME's decode_AM). Returns (Operand, length)."""
    mod = mem.r8(pc)
    reg = mod & 0x1F
    name = REGISTER_NAMES[reg]
    at = "@" if size & 0x80 else ""
    group = mod >> 5

    if m:
        if group <= 2:
            inner, width = _read_disp(mem, pc + 1, group)
            outer, _ = _read_disp(mem, pc + 1 + width, group)
            text = f"{_signed_hex(outer)}{at}[{_signed_hex(inner)}[{name}]]"
            return Operand("double_disp", size, text, reg=reg, disp=inner, outer_disp=outer), 1 + 2 * width
        if group == 3:
            return Operand("reg", size, name, reg=reg), 1
        if group == 4:
            return Operand("autoinc", size, f"{at}[{name}+]", reg=reg), 1
        if group == 5:
            return Operand("autodec", size, f"{at}[-{name}]", reg=reg), 1
        if group == 7:
            return Operand("error", size, "!ERRAM1"), 1

        # group 6: indexed modes; the first byte holds the index register
        sub = mem.r8(pc + 1)
        rx = reg
        index = REGISTER_NAMES[rx]
        rn = sub & 0x1F
        base = REGISTER_NAMES[rn]
        subgroup = sub >> 5
        if subgroup <= 2:
            disp, width = _read_disp(mem, pc + 2, subgroup)
            text = f"{index}@{_signed_hex(disp)}[{base}]" if at else f"{_signed_hex(disp)}[{base}]({index})"
            return Operand("disp_indexed", size, text, reg=rn, index_reg=rx, disp=disp), 2 + width
        if subgroup == 3:
            text = f"{index}@[{base}]" if at else f"[{base}]({index})"
            return Operand("reg_indirect_indexed", size, text, reg=rn, index_reg=rx), 2
        if subgroup <= 6:
            disp, width = _read_disp(mem, pc + 2, subgroup - 4)
            text = f"{index}@[{_signed_hex(disp)}[{base}]]" if at else f"[{_signed_hex(disp)}[{base}]]({index})"
            return Operand("disp_indirect_indexed", size, text, reg=rn, index_reg=rx, disp=disp), 2 + width

        # subgroup 7: PC-relative and absolute, indexed
        if rn in (16, 17, 18):
            disp, width = _read_disp(mem, pc + 2, rn - 16)
            value = (ipc + disp) & MASK32
            text = f"{index}@{value:X}[PC]" if at else f"{value:X}[PC]({index})"
            return Operand("pc_disp_indexed", size, text, index_reg=rx, disp=disp, value=value), 2 + width
        if rn == 19:
            value = mem.r32(pc + 2)
            text = f"{index}@{value:X}" if at else f"{value:X}({index})"
            return Operand("direct_indexed", size, text, index_reg=rx, value=value), 6
        if rn in (24, 25, 26):
            disp, width = _read_disp(mem, pc + 2, rn - 24)
            value = (ipc + disp) & MASK32
            text = f"{index}@[{value:X}[PC]]" if at else f"[{value:X}[PC]]({index})"
            return Operand("pc_disp_indirect_indexed", size, text, index_reg=rx, disp=disp, value=value), 2 + width
        if rn == 27:
            value = mem.r32(pc + 2)
            text = f"{index}@[{value:X}]" if at else f"[{value:X}]({index})"
            return Operand("direct_deferred_indexed", size, text, index_reg=rx, value=value), 6
        return Operand("error", size, "!ERRAM3"), 1

    if group <= 2:
        disp, width = _read_disp(mem, pc + 1, group)
        return Operand("disp", size, f"{_signed_hex(disp)}{at}[{name}]", reg=reg, disp=disp), 1 + width
    if group == 3:
        return Operand("reg_indirect", size, f"{at}[{name}]", reg=reg), 1
    if group <= 6:
        disp, width = _read_disp(mem, pc + 1, group - 4)
        return Operand("disp_indirect", size, f"{at}[{_signed_hex(disp)}[{name}]]", reg=reg, disp=disp), 1 + width

    # group 7: immediates, PC-relative and absolute addresses
    if reg <= 15:
        return Operand("immediate", size, _immediate_text(reg, size), value=reg), 1
    if reg in (16, 17, 18):
        disp, width = _read_disp(mem, pc + 1, reg - 16)
        value = (ipc + disp) & MASK32
        return Operand("pc_disp", size, f"{value:X}{at}[PC]", disp=disp, value=value), 1 + width
    if reg == 19:
        value = mem.r32(pc + 1)
        return Operand("direct", size, f"{at}{value:X}", value=value), 5
    if reg == 20:
        width = size & 0x7F
        if width == 0:
            value, length = mem.r8(pc + 1), 2
        elif width == 1:
            value, length = mem.r16(pc + 1), 3
        elif width == 2:
            value, length = mem.r32(pc + 1), 5
        else:
            return Operand("error", size, "!ERRAM6"), 1
        return Operand("immediate", size, _immediate_text(value, size), value=value), length
    if reg in (24, 25, 26):
        disp, width = _read_disp(mem, pc + 1, reg - 24)
        value = (ipc + disp) & MASK32
        return Operand("pc_disp_indirect", size, f"{at}[{value:X}[PC]]", disp=disp, value=value), 1 + width
    if reg == 27:
        value = mem.r32(pc + 1)
        return Operand("direct_deferred", size, f"{at}[{value:X}]", value=value), 5
    if reg in (28, 29, 30):
        inner, width = _read_disp(mem, pc + 1, reg - 28)
        outer, _ = _read_disp(mem, pc + 1 + width, reg - 28)
        value = (ipc + inner) & MASK32
        text = f"{_signed_hex(outer)}{at}[{value:X}[PC]]"
        return Operand("pc_double_disp", size, text, disp=inner, outer_disp=outer, value=value), 1 + 2 * width
    return Operand("error", size, "!ERRAM5"), 1


def _register_operand(reg):
    return Operand("reg", 2, REGISTER_NAMES[reg], reg=reg)


def _length_operand(code):
    """Length field of the string and bit-field instructions: a register or a 7-bit immediate."""
    if code & 0x80:
        return _register_operand(code & 0x1F)
    return Operand("immediate", 1, _immediate_text(code, 1), value=code)


def _make(ipc, length, mnemonic, operands, flow):
    text = f"{mnemonic:<8}" + ", ".join(operand.text for operand in operands)
    return Instruction(ipc, length, mnemonic, text, flow, operands)


# Instruction formats. All of them receive pc = address of the byte after the opcode.

def _format_f1(mem, ipc, pc, mnemonic, size1, size2, flow):
    code = mem.r8(pc)
    if code & 0x20:
        operand, length = decode_operand(mem, ipc, pc + 1, code & 0x40, size1)
        operands = [operand, _register_operand(code & 0x1F)]
    else:
        operand, length = decode_operand(mem, ipc, pc + 1, code & 0x40, size2)
        operands = [_register_operand(code & 0x1F), operand]
    return _make(ipc, length + 2, mnemonic, operands, flow)


def _format_f2(mem, ipc, pc, mnemonic, size1, size2, flow):
    code = mem.r8(pc)
    first, length1 = decode_operand(mem, ipc, pc + 1, code & 0x40, size1)
    second, length2 = decode_operand(mem, ipc, pc + 1 + length1, code & 0x20, size2)
    return _make(ipc, length1 + length2 + 2, mnemonic, [first, second], flow)


def _format_f1f2(mem, ipc, pc, mnemonic, size1, size2, flow):
    if mem.r8(pc) & 0x80:
        return _format_f2(mem, ipc, pc, mnemonic, size1, size2, flow)
    return _format_f1(mem, ipc, pc, mnemonic, size1, size2, flow)


def _format_f3(mem, ipc, pc, mnemonic, size1, size2, flow):
    operand, length = decode_operand(mem, ipc, pc, mem.r8(pc - 1) & 1, size1)
    return _make(ipc, length + 1, mnemonic, [operand], flow)


def _format_f4a(mem, ipc, pc, mnemonic, size1, size2, flow):
    target = (ipc + _s8(mem.r8(pc))) & MASK32
    return Instruction(ipc, 2, mnemonic, f"{mnemonic:<8}{target:X}", flow, target=target)


def _format_f4b(mem, ipc, pc, mnemonic, size1, size2, flow):
    target = (ipc + _s16(mem.r16(pc))) & MASK32
    return Instruction(ipc, 3, mnemonic, f"{mnemonic:<8}{target:X}", flow, target=target)


def _format_f5(mem, ipc, pc, mnemonic, size1, size2, flow):
    return Instruction(ipc, 1, mnemonic, mnemonic, flow)


def _format_f6(mem, ipc, pc, mnemonic, size1, size2, flow):
    counter = _register_operand(mem.r8(pc) & 0x1F)
    target = (ipc + _s16(mem.r16(pc + 1))) & MASK32
    text = f"{mnemonic:<8}{counter.text}, {target:X}[PC]"
    return Instruction(ipc, 4, mnemonic, text, flow, [counter], target=target)


def _format_f7a(mem, ipc, pc, mnemonic, size1, size2, flow):
    code = mem.r8(pc)
    source, length1 = decode_operand(mem, ipc, pc + 1, code & 0x40, size1)
    source_length = _length_operand(mem.r8(pc + 1 + length1))
    dest, length2 = decode_operand(mem, ipc, pc + 2 + length1, code & 0x20, size2)
    dest_length = _length_operand(mem.r8(pc + 2 + length1 + length2))
    return _make(ipc, length1 + length2 + 4, mnemonic, [source, source_length, dest, dest_length], flow)


def _format_f7b(mem, ipc, pc, mnemonic, size1, size2, flow):
    code = mem.r8(pc)
    source, length1 = decode_operand(mem, ipc, pc + 1, code & 0x40, size1)
    source_length = _length_operand(mem.r8(pc + 1 + length1))
    dest, length2 = decode_operand(mem, ipc, pc + 2 + length1, code & 0x20, size2)
    return _make(ipc, length1 + length2 + 3, mnemonic, [source, source_length, dest], flow)


def _format_f7c(mem, ipc, pc, mnemonic, size1, size2, flow):
    code = mem.r8(pc)
    source, length1 = decode_operand(mem, ipc, pc + 1, code & 0x40, size1)
    dest, length2 = decode_operand(mem, ipc, pc + 1 + length1, code & 0x20, size2)
    extra = _length_operand(mem.r8(pc + 1 + length1 + length2))
    return _make(ipc, length1 + length2 + 3, mnemonic, [source, dest, extra], flow)


F1, F2, F1F2, F3 = _format_f1, _format_f2, _format_f1f2, _format_f3
F4A, F4B, F5, F6 = _format_f4a, _format_f4b, _format_f5, _format_f6
F7A, F7B, F7C = _format_f7a, _format_f7b, _format_f7c
N, B, J, C, R = FLOW_NORMAL, FLOW_BRANCH, FLOW_JUMP, FLOW_CALL, FLOW_RETURN


def _sized(table, opcodes, name, fmt, flow=N):
    """MAME's DEFINE_TRIPLE_OPCODE: .b/.h/.w variants."""
    for opcode, suffix, size in zip(opcodes, (".b", ".h", ".w"), (0, 1, 2)):
        table[opcode] = (name + suffix, fmt, size, size, flow)


# Main table: opcode -> (mnemonic, format, operand size 1, operand size 2, flow)
OPCODES = [None] * 256
OPCODES[0x00] = ("halt", F5, 0, 0, N)
OPCODES[0x01] = ("ldtask", F1F2, 2, 2, N)
OPCODES[0x02] = ("stpr", F1F2, 2, 2, N)
OPCODES[0x03] = ("getra", F1F2, 0, 2, N)
OPCODES[0x04] = ("getpte", F1F2, 0, 2, N)
OPCODES[0x05] = ("getate", F1F2, 0, 3, N)
OPCODES[0x08] = ("rvbit", F1F2, 0, 0, N)
OPCODES[0x09] = ("mov.b", F1F2, 0, 0, N)
OPCODES[0x0A] = ("movs.bh", F1F2, 0, 1, N)
OPCODES[0x0B] = ("movz.bh", F1F2, 0, 1, N)
OPCODES[0x0C] = ("movs.bw", F1F2, 0, 2, N)
OPCODES[0x0D] = ("movz.bw", F1F2, 0, 2, N)
OPCODES[0x10] = ("clrtlba", F5, 0, 0, N)
OPCODES[0x12] = ("ldpr", F1F2, 2, 2, N)
OPCODES[0x13] = ("updpsw.w", F1F2, 2, 2, N)
OPCODES[0x14] = ("updpte", F1F2, 0, 2, N)
OPCODES[0x15] = ("update", F1F2, 0, 3, N)
OPCODES[0x19] = ("movt.hb", F1F2, 1, 0, N)
OPCODES[0x1B] = ("mov.h", F1F2, 1, 1, N)
OPCODES[0x1C] = ("movs.hw", F1F2, 1, 2, N)
OPCODES[0x1D] = ("movz.hw", F1F2, 1, 2, N)
_sized(OPCODES, (0x20, 0x22, 0x24), "in", F1F2)
_sized(OPCODES, (0x21, 0x23, 0x25), "out", F1F2)
OPCODES[0x29] = ("movt.wb", F1F2, 2, 0, N)
OPCODES[0x2B] = ("movt.wh", F1F2, 2, 1, N)
OPCODES[0x2C] = ("rvbyt", F1F2, 2, 2, N)
OPCODES[0x2D] = ("mov.w", F1F2, 2, 2, N)
_sized(OPCODES, (0x38, 0x3A, 0x3C), "not", F1F2)
_sized(OPCODES, (0x39, 0x3B, 0x3D), "neg", F1F2)
OPCODES[0x3F] = ("mov.d", F1F2, 3, 3, N)
OPCODES[0x40] = ("movea.b", F1F2, 0, 2, N)
OPCODES[0x42] = ("movea.h", F1F2, 1, 2, N)
OPCODES[0x44] = ("movea.w", F1F2, 2, 2, N)
_sized(OPCODES, (0x41, 0x43, 0x45), "xch", F1)
OPCODES[0x47] = ("setf", F1F2, 0, 0, N)
OPCODES[0x48] = ("bsr", F4B, 0, 0, C)
OPCODES[0x49] = ("call", F1F2, 0, 2, C)
OPCODES[0x4A] = ("updpsw.h", F1F2, 2, 2, N)
OPCODES[0x4B] = ("chlvl", F1F2, 0, 0, N)
OPCODES[0x4C] = ("caxi", F1, 2, 2, N)
OPCODES[0x4D] = ("chkar", F1F2, 0, 0, N)
OPCODES[0x4E] = ("chkaw", F1F2, 0, 0, N)
OPCODES[0x4F] = ("chkae", F1F2, 0, 0, N)
_sized(OPCODES, (0x50, 0x52, 0x54), "rem", F1F2)
_sized(OPCODES, (0x51, 0x53, 0x55), "remu", F1F2)

for _base, _fmt in ((0x60, F4A), (0x70, F4B)):
    for _offset, _name in enumerate(("bv", "bnv", "bl", "bnl", "be", "bne", "bnh", "bh",
                                     "bn", "bp", None, None, "blt", "bge", "ble", "bgt")):
        if _name:
            OPCODES[_base + _offset] = (_name, _fmt, 0, 0, B)
    OPCODES[_base + 0x0A] = ("br", _fmt, 0, 0, J)

_sized(OPCODES, (0x80, 0x82, 0x84), "add", F1F2)
_sized(OPCODES, (0x81, 0x83, 0x85), "mul", F1F2)
OPCODES[0x86] = ("mulx", F1F2, 2, 3, N)
OPCODES[0x87] = ("test1", F1F2, 2, 2, N)
_sized(OPCODES, (0x88, 0x8A, 0x8C), "or", F1F2)
OPCODES[0x89] = ("rot.b", F1F2, 0, 0, N)
OPCODES[0x8B] = ("rot.h", F1F2, 0, 1, N)
OPCODES[0x8D] = ("rot.w", F1F2, 0, 2, N)
_sized(OPCODES, (0x90, 0x92, 0x94), "addc", F1F2)
_sized(OPCODES, (0x91, 0x93, 0x95), "mulu", F1F2)
OPCODES[0x96] = ("mulux", F1F2, 2, 3, N)
OPCODES[0x97] = ("set1", F1F2, 2, 2, N)
_sized(OPCODES, (0x98, 0x9A, 0x9C), "subc", F1F2)
OPCODES[0x99] = ("rotc.b", F1F2, 0, 0, N)
OPCODES[0x9B] = ("rotc.h", F1F2, 0, 1, N)
OPCODES[0x9D] = ("rotc.w", F1F2, 0, 2, N)
_sized(OPCODES, (0xA0, 0xA2, 0xA4), "and", F1F2)
_sized(OPCODES, (0xA1, 0xA3, 0xA5), "div", F1F2)
OPCODES[0xA6] = ("divx", F1F2, 2, 3, N)
OPCODES[0xA7] = ("clr1", F1F2, 2, 2, N)
_sized(OPCODES, (0xA8, 0xAA, 0xAC), "sub", F1F2)
OPCODES[0xA9] = ("shl.b", F1F2, 0, 0, N)
OPCODES[0xAB] = ("shl.h", F1F2, 0, 1, N)
OPCODES[0xAD] = ("shl.w", F1F2, 0, 2, N)
_sized(OPCODES, (0xB0, 0xB2, 0xB4), "xor", F1F2)
_sized(OPCODES, (0xB1, 0xB3, 0xB5), "divu", F1F2)
OPCODES[0xB6] = ("divux", F1F2, 2, 3, N)
OPCODES[0xB7] = ("not1", F1F2, 2, 2, N)
_sized(OPCODES, (0xB8, 0xBA, 0xBC), "cmp", F1F2)
OPCODES[0xB9] = ("sha.b", F1F2, 0, 0, N)
OPCODES[0xBB] = ("sha.h", F1F2, 0, 1, N)
OPCODES[0xBD] = ("sha.w", F1F2, 0, 2, N)
OPCODES[0xC8] = ("brk", F5, 0, 0, N)
OPCODES[0xC9] = ("brkv", F5, 0, 0, N)
OPCODES[0xCA] = ("rsr", F5, 0, 0, R)
OPCODES[0xCB] = ("trapfl", F5, 0, 0, N)
OPCODES[0xCC] = ("dispose", F5, 0, 0, N)
OPCODES[0xCD] = ("nop", F5, 0, 0, N)

# 0xD0-0xFF: format F3, bit 0 of the opcode selects the addressing mode set
for _opcode, _entry in (
        (0xD0, ("dec.b", F3, 0, 0, N)), (0xD2, ("dec.h", F3, 1, 1, N)), (0xD4, ("dec.w", F3, 2, 2, N)),
        (0xD6, ("jmp", F3, 0, 0, J)), (0xD8, ("inc.b", F3, 0, 0, N)), (0xDA, ("inc.h", F3, 1, 1, N)),
        (0xDC, ("inc.w", F3, 2, 2, N)), (0xDE, ("prepare", F3, 2, 0, N)), (0xE0, ("tasi", F3, 0, 0, N)),
        (0xE2, ("ret", F3, 2, 0, R)), (0xE4, ("popm", F3, 2, 0, N)), (0xE6, ("pop", F3, 2, 0, N)),
        (0xE8, ("jsr", F3, 0, 0, C)), (0xEA, ("retiu", F3, 1, 0, R)), (0xEC, ("pushm", F3, 2, 0, N)),
        (0xEE, ("push", F3, 2, 0, N)), (0xF0, ("test.b", F3, 0, 0, N)), (0xF2, ("test.h", F3, 1, 1, N)),
        (0xF4, ("test.w", F3, 2, 2, N)), (0xF6, ("getpsw", F3, 2, 0, N)), (0xF8, ("trap", F3, 0, 0, N)),
        (0xFA, ("retis", F3, 1, 0, R)), (0xFC, ("sttask", F3, 2, 0, N)), (0xFE, ("clrtlb", F3, 0, 0, N))):
    OPCODES[_opcode] = _entry
    OPCODES[_opcode + 1] = _entry

# 0x58-0x5F: the second byte (bits 0-4) selects the instruction
SUBTABLES = {code: [None] * 32 for code in range(0x58, 0x60)}
for _sub, _name in ((0x00, "cmpc"), (0x01, "cmpcf"), (0x02, "cmpcs"), (0x08, "movcu"), (0x09, "movcd"),
                    (0x0A, "movcfu"), (0x0B, "movcfd"), (0x0C, "movcs")):
    SUBTABLES[0x58][_sub] = (_name + ".b", F7A, 0, 0, N)
    SUBTABLES[0x5A][_sub] = (_name + ".h", F7A, 1, 1, N)
for _sub, _name in ((0x18, "schcu"), (0x19, "schcd"), (0x1A, "skpcu"), (0x1B, "skpcd")):
    SUBTABLES[0x58][_sub] = (_name + ".b", F7B, 0, 0, N)
    SUBTABLES[0x5A][_sub] = (_name + ".h", F7B, 1, 1, N)
SUBTABLES[0x59][0x00] = ("adddc", F7C, 0, 0, N)
SUBTABLES[0x59][0x01] = ("subdc", F7C, 0, 0, N)
SUBTABLES[0x59][0x02] = ("subrdc", F7C, 0, 0, N)
SUBTABLES[0x59][0x10] = ("cvtd.pz", F7C, 0, 1, N)
SUBTABLES[0x59][0x18] = ("cvtd.zp", F7C, 1, 0, N)
for _sub, _name in ((0x00, "sch0bsu"), (0x01, "sch0bsd"), (0x02, "sch1bsu"), (0x03, "sch1bsd")):
    SUBTABLES[0x5B][_sub] = (_name, F7B, 0x80, 2, N)
for _sub, _name in ((0x08, "movbsu"), (0x09, "movbsd"), (0x0A, "notbsu"), (0x0B, "notbsd"),
                    (0x10, "andbsu"), (0x11, "andbsd"), (0x12, "andnbsu"), (0x13, "andnbsd"),
                    (0x14, "orbsu"), (0x15, "orbsd"), (0x16, "ornbsu"), (0x17, "ornbsd"),
                    (0x18, "xorbsu"), (0x19, "xorbsd"), (0x1A, "xornbsu"), (0x1B, "xornbsd")):
    SUBTABLES[0x5B][_sub] = (_name, F7B, 0x80, 0x80, N)
for _code, _suffix in ((0x5C, ".s"), (0x5E, ".l")):
    for _sub, _name in ((0x00, "cmpf"), (0x08, "movf"), (0x09, "negf"), (0x0A, "absf"),
                        (0x18, "addf"), (0x19, "subf"), (0x1A, "mulf"), (0x1B, "divf")):
        SUBTABLES[_code][_sub] = (_name + _suffix, F2, 2, 2, N)
    SUBTABLES[_code][0x10] = ("sclf" + _suffix, F2, 1, 2, N)
for _sub, _name in ((0x00, "cmpbfs"), (0x01, "cmpbfz"), (0x02, "cmpbfl"),
                    (0x08, "extbfs"), (0x09, "extbfz"), (0x0A, "extbfl")):
    SUBTABLES[0x5D][_sub] = (_name, F7B, 0x82, 2, N)
SUBTABLES[0x5D][0x18] = ("insbfr", F7C, 2, 0x82, N)
SUBTABLES[0x5D][0x19] = ("insbfl", F7C, 2, 0x82, N)
SUBTABLES[0x5F][0x00] = ("cvt.ws", F2, 2, 0, N)
SUBTABLES[0x5F][0x01] = ("cvt.sw", F2, 0, 2, N)
SUBTABLES[0x5F][0x08] = ("cvt.ls", F2, 1, 0, N)
SUBTABLES[0x5F][0x09] = ("cvt.lw", F2, 1, 2, N)
SUBTABLES[0x5F][0x10] = ("cvt.sl", F2, 0, 1, N)
SUBTABLES[0x5F][0x11] = ("cvt.wl", F2, 2, 1, N)

# 0xC6/0xC7: decrement and branch; the condition lives in bits 5-7 of the second byte
DECREMENT_BRANCHES = {
    0xC6: ("dbv", "dbl", "dbe", "dbnh", "dbn", "dbr", "dblt", "dble"),
    0xC7: ("dbnv", "dbnl", "dbne", "dbh", "dbp", "tb", "dbge", "dbgt"),
}


def decode(mem, address):
    """Decode the instruction at 'address'. Never raises: undecodable bytes come back as FLOW_INVALID.

    Note: for an unassigned main opcode MAME prints the byte that follows it ('$%02X' of pc + 1);
    this decoder prints the opcode itself. compare_with_mame.py accounts for that.
    """
    opcode = mem.r8(address)
    pc = address + 1

    if 0x58 <= opcode <= 0x5F:
        entry = SUBTABLES[opcode][mem.r8(pc) & 0x1F]
    elif opcode in DECREMENT_BRANCHES:
        name = DECREMENT_BRANCHES[opcode][mem.r8(pc) >> 5]
        return _format_f6(mem, address, pc, name, 0, 0, B)
    else:
        entry = OPCODES[opcode]
    if entry is None:
        return Instruction(address, 1, f"${opcode:02X}", f"${opcode:02X}", FLOW_INVALID)

    mnemonic, fmt, size1, size2, flow = entry
    instruction = fmt(mem, address, pc, mnemonic, size1, size2, flow)
    if any(operand.mode == "error" for operand in instruction.operands):
        instruction.flow = FLOW_INVALID
        return instruction

    # jmp / jsr / call: the target is the effective address of the first operand
    if mnemonic in ("jmp", "jsr", "call") and instruction.operands:
        destination = instruction.operands[0]
        if destination.mode in ("pc_disp", "direct"):
            instruction.target = destination.value
        elif destination.mode in ("pc_disp_indirect", "direct_deferred"):
            instruction.target_pointer = destination.value
    return instruction
