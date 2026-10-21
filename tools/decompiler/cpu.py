"""
Name:        cpu.py
Description: what the decompiler's passes need to know about the processor whose code they work on
Version:     0.1.0
Created:     13-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

The passes were written for the V60 and work on any code lifted to the same IR: a processor is its
registers (which are kept as variables, which one is the stack pointer, how wide an address is), its
flags, how big a return address is on its stack, the effects where interrupts may run, and the register
a function's yes-or-no answer usually comes back in. use() makes a profile the current one and lets the
modules that keep tables of it build them again; V60 is the one in use until then.
"""
from dataclasses import dataclass


@dataclass(frozen=True)
class Profile:
    name: str
    registers: tuple            # the registers kept as variables (the stack pointer is not one)
    sp: int                     # the stack pointer
    address_bits: int           # of an address, a pointer and a stack slot holding one
    flags: tuple                # the flags the IR keeps
    return_address_bytes: int   # what a call pushes
    interrupt_effects: tuple    # Effects where interrupt handlers may run
    result_register: object     # where functions answer whether they did their job (None: no such habit)
    register_bits: dict         # the width of each register kept as a variable
    register_names: dict        # the C name of a register, where it is not rN
    flag_names: dict            # the C name of each flag
    variable_names: dict        # the base name of a register's variables, where it is not rN
    invalid_jump: str           # what a jump through a table calls for an entry the table does not have


V60 = Profile(
    name="v60",
    registers=tuple(range(31)) + (32, 33),    # 32: SP where the stack stays in memory (stack.MEMORY_SP);
                                              # 33: whether a call returned for its caller too (stack.LEFT_CALLER)
    sp=31,
    address_bits=32,
    flags=("z", "s", "ov", "cy"),
    return_address_bytes=4,
    interrupt_effects=("frame_wait", "take_interrupts"),
    result_register=0,
    register_bits={index: 32 for index in tuple(range(31)) + (32, 33)},
    register_names={29: "ap", 30: "fp", 33: "left_caller"},
    flag_names={"z": "zero", "s": "negative", "ov": "overflow", "cy": "carry"},
    variable_names={33: "left_caller"},
    invalid_jump="invalid_jump",
)

# tools/z80/lift.py: BC DE HL IX IY are 0-4, SP 5, A 6, and the other bank's BC DE HL A 7-10; B C D E H L
# 11-16 where a function uses a pair's bytes apart (tools/z80/pairs.py)
Z80 = Profile(
    name="z80",
    registers=(0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16),
    sp=5,
    address_bits=16,
    flags=("z", "s", "ov", "cy", "z_alt", "s_alt", "ov_alt", "cy_alt"),
    return_address_bytes=2,
    interrupt_effects=("z80_block",),
    result_register=None,
    register_bits={0: 16, 1: 16, 2: 16, 3: 16, 4: 16, 6: 8, 7: 16, 8: 16, 9: 16, 10: 8,
                   11: 8, 12: 8, 13: 8, 14: 8, 15: 8, 16: 8},
    register_names={0: "bc", 1: "de", 2: "hl", 3: "ix", 4: "iy", 5: "sp", 6: "a",
                    7: "bc_alt", 8: "de_alt", 9: "hl_alt", 10: "a_alt",
                    11: "b", 12: "c", 13: "d", 14: "e", 15: "h", 16: "l"},
    flag_names={"z": "zero", "s": "negative", "ov": "overflow", "cy": "carry",
                "z_alt": "zero_alt", "s_alt": "negative_alt", "ov_alt": "overflow_alt", "cy_alt": "carry_alt"},
    variable_names={0: "bc", 1: "de", 2: "hl", 3: "ix", 4: "iy", 6: "a", 7: "bc_alt", 8: "de_alt", 9: "hl_alt",
                    10: "a_alt", 11: "b", 12: "c", 13: "d", 14: "e", 15: "h", 16: "l"},
    invalid_jump="sound_invalid_jump",
)

current = V60


def variable_name(index):
    """The base name of the variables a register's values become, where nothing better names them."""
    return current.variable_names.get(index, f"r{index}")


def kept(index):
    """True for a register the passes keep as a variable."""
    return index in current.registers


def use(profile):
    """Makes 'profile' the processor the passes work on."""
    global current
    current = profile
    from . import dataflow
    dataflow.configure()
