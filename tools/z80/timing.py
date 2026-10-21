# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        timing.py
Description: how many clock cycles each Z80 instruction takes, as MAME's core counts them
Version:     0.1.0
Created:     13-10-2026
Modified:    21-10-2026
Author:      Sebastian Toyos

cycles(instruction) is (taken, not taken) for the instructions whose time depends on a condition
(JR cc, DJNZ, CALL cc, RET cc), and the same number twice for the others. LDIR and LDDR give the time of
their last round; each round before it takes 21 (runtime/src/sound_cpu.c adds them). These are the
Zilog timings MAME's core keeps (an opcode fetch 4, a memory or argument access 3, a port access 4,
plus its internal cycles); tools/reference/sound measures the core and compares.
"""
from . import disassembler as dis

ALU = ("add", "adc", "sub", "sbc", "and", "xor", "or", "cp")
SHIFTS = ("rlc", "rrc", "rl", "rr", "sla", "sra", "sll", "srl")
INDEX_BYTES = ("ixh", "ixl", "iyh", "iyl")
SIXTEEN = ("bc", "de", "hl", "sp", "ix", "iy", "af")


def is_pair(operand):
    return operand.mode == "reg" and operand.reg in SIXTEEN


def is_index(operand):
    return operand.reg in ("ix", "iy") or operand.reg in INDEX_BYTES


def memory_cost(operand):
    """What reading an 8-bit operand adds to an ALU or load: (hl) 3, (ix+d) 15 (prefix, displacement,
    address computation), an index register half 4 (its prefix)."""
    if operand.mode == "reg_indirect":
        return 3
    if operand.mode == "indexed":
        return 15
    if operand.mode == "reg" and operand.reg in INDEX_BYTES:
        return 4
    return 0


PREFIXES = (0xCB, 0xDD, 0xED, 0xFD)


def fetches(first_byte):
    """How many opcode fetches (M1 cycles) an instruction makes, each of which counts the refresh register
    R up, as MAME's core does: one, and one more when it starts with a prefix (DD CB d op makes two: the
    displacement and the opcode after CB are argument reads). LDIR and LDDR fetch both bytes again on
    every round, and an interrupt counts R up once (runtime/src/sound_cpu.c)."""
    return 2 if first_byte in PREFIXES else 1


def cycles(instruction):
    """(cycles when a condition holds, cycles when it does not)."""
    m = instruction.mnemonic
    ops = instruction.operands
    both = None

    if m in ("nop", "halt", "di", "ei", "exx", "cpl", "rlca", "rrca", "rla", "rra", "daa", "scf", "ccf"):
        both = 4
    elif m == "ex":
        if ops[0].mode == "reg_indirect":
            both = 23 if ops[1].reg in ("ix", "iy") else 19
        else:
            both = 4
    elif m == "ld":
        destination, source = ops
        if is_pair(destination) or is_pair(source):
            pair = destination if is_pair(destination) else source
            other = source if pair is destination else destination
            if destination.reg == "sp" and source.mode == "reg":
                both = 10 if source.reg in ("ix", "iy") else 6
            elif other.mode == "immediate":
                both = 14 if pair.reg in ("ix", "iy") else 10
            elif pair.reg == "hl":
                both = 16
            else:
                both = 20
        elif destination.reg in ("i", "r") or source.reg in ("i", "r"):
            both = 9
        elif destination.mode == "direct" or source.mode == "direct":
            both = 13
        elif destination.mode == "indexed" or source.mode == "indexed":
            both = 19
        elif destination.mode == "reg_indirect" and source.mode == "immediate":
            both = 10
        elif destination.mode == "reg_indirect" or source.mode == "reg_indirect":
            both = 7
        elif source.mode == "immediate":
            both = 11 if is_index(destination) else 7
        elif is_index(destination) or is_index(source):
            both = 8
        else:
            both = 4
    elif m in ALU:
        if len(ops) == 2 and is_pair(ops[0]):
            if m == "add":
                both = 15 if ops[0].reg in ("ix", "iy") else 11
            else:
                both = 15
        else:
            source = ops[-1]
            both = 7 if source.mode == "immediate" else 4 + memory_cost(source)
    elif m in ("inc", "dec"):
        operand = ops[0]
        if is_pair(operand):
            both = 10 if operand.reg in ("ix", "iy") else 6
        elif operand.mode == "reg_indirect":
            both = 11
        elif operand.mode == "indexed":
            both = 23
        else:
            both = 8 if is_index(operand) else 4
    elif m == "neg" or m == "im":
        both = 8
    elif m in SHIFTS or m in ("set", "res"):
        operand = ops[-1]
        both = 23 if operand.mode == "indexed" else (15 if operand.mode == "reg_indirect" else 8)
    elif m == "bit":
        operand = ops[-1]
        both = 20 if operand.mode == "indexed" else (12 if operand.mode == "reg_indirect" else 8)
    elif m in ("ldi", "ldd", "cpi", "cpd", "ini", "ind", "outi", "outd", "ldir", "lddr"):
        both = 16
    elif m == "push":
        both = 15 if ops[0].reg in ("ix", "iy") else 11
    elif m == "pop":
        both = 14 if ops[0].reg in ("ix", "iy") else 10
    elif m == "in" or m == "out":
        port = ops[1] if m == "in" else ops[0]
        both = 11 if port.mode == "port" else 12
    elif m == "jp":
        if ops[0].mode == "reg_indirect":
            both = 8 if ops[0].reg in ("ix", "iy") else 4
        else:
            both = 10
    elif m == "jr":
        return (12, 7) if instruction.condition else (12, 12)
    elif m == "djnz":
        return (13, 8)
    elif m == "call":
        return (17, 10) if instruction.condition else (17, 17)
    elif m == "ret":
        return (11, 5) if instruction.condition else (10, 10)
    elif m in ("reti", "retn"):
        both = 14
    elif m == "rst":
        both = 11
    if both is None:
        raise ValueError(f"{instruction.address:04X}: no timing for {instruction.text}")
    return both, both
