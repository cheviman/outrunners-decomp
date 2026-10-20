# license:BSD-3-Clause
# copyright-holders:Farfetch'd, R. Belmont, Sebastian Toyos
"""
Name:        codegen.py
Description: translates decoded V60 instructions into C that does what MAME's V60 core does
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

Every translation mirrors an op function of MAME's core (src/devices/cpu/v60, BSD-3-Clause):
operands are decoded in the same order, memory is read and written in the same order and with
the same widths, and flags come from the helpers in runtime/include/v60/ops.h. The three kinds of
operand access follow MAME's tables: am1 reads a value, am2 yields an address (or a register),
am3 writes a value. Bit-string operands (am2 "bam") yield an address plus a bit offset.
"""
from . import disassembler as dis

REGISTER_NAMES = [f"R{index}" for index in range(29)] + ["AP", "FP", "SP"]
C_TYPES = {0: "uint8_t", 1: "uint16_t", 2: "uint32_t"}
BITS = {0: 8, 1: 16, 2: 32}
SIZES = {0: 1, 1: 2, 2: 4, 3: 8}


class Unsupported(Exception):
    """An instruction or addressing mode this generator does not translate yet."""


def reg(index):
    return REGISTER_NAMES[index] if index < 32 else f"cpu.reg[{index}]"


def const(value):
    return f"0x{value & 0xFFFFFFFF:X}u"


def plus(base, offset):
    """C for base + a signed 32-bit offset, without a leading '+ 0'."""
    if offset == 0:
        return base
    if offset > 0:
        return f"{base} + 0x{offset:X}u"
    return f"{base} - 0x{-offset:X}u"


def sized(expression, dim):
    return f"({C_TYPES[dim]})({expression})"


class Location:
    """An am2 result: either a register (MAME's amflag = 1) or a memory address in a C variable."""

    def __init__(self, register=None, address=None):
        self.register = register
        self.address = address


class Block:
    """The C statements of one instruction, with readable, unique temporaries."""

    def __init__(self, mem):
        self.mem = mem
        self.lines = []
        self.names = {}

    def emit(self, line):
        self.lines.append(line)

    def temp(self, ctype, hint, expression):
        count = self.names.get(hint, 0)
        self.names[hint] = count + 1
        name = hint if count == 0 else f"{hint}{count + 1}"
        self.emit(f"{ctype} {name} = {expression};")
        return name

    # ---- effective addresses shared by am1 / am2 / am3 ----

    def effective_address(self, op, dim, flavor):
        """C for the address of a memory operand; pointer reads are emitted as statements."""
        scale = SIZES[dim]
        if flavor == "am3" and op.mode == "direct_deferred_indexed":
            scale = 1    # MAME's am3DirectAddressDeferredIndexed does not scale the index

        def index():
            return reg(op.index_reg) if scale == 1 else f"{reg(op.index_reg)} * {scale}u"

        mode = op.mode
        if mode == "reg_indirect":
            return reg(op.reg)
        if mode == "reg_indirect_indexed":
            return f"{reg(op.reg)} + {index()}"
        if mode == "disp":
            return plus(reg(op.reg), op.disp)
        if mode == "disp_indexed":
            return f"{plus(reg(op.reg), op.disp)} + {index()}"
        if mode == "pc_disp":
            return const(op.value)
        if mode == "pc_disp_indexed":
            return f"{const(op.value)} + {index()}"
        if mode == "direct":
            return const(op.value)
        if mode == "direct_indexed":
            return f"{const(op.value)} + {index()}"
        if mode == "disp_indirect":
            return self.temp("const uint32_t", "pointer", f"mem_read32({plus(reg(op.reg), op.disp)})")
        if mode == "disp_indirect_indexed":
            pointer = self.temp("const uint32_t", "pointer", f"mem_read32({plus(reg(op.reg), op.disp)})")
            return f"{pointer} + {index()}"
        if mode == "pc_disp_indirect":
            return self.temp("const uint32_t", "pointer", f"mem_read32({const(op.value)})")
        if mode == "pc_disp_indirect_indexed":
            pointer = self.temp("const uint32_t", "pointer", f"mem_read32({const(op.value)})")
            return f"{pointer} + {index()}"
        if mode == "double_disp":
            pointer = self.temp("const uint32_t", "pointer", f"mem_read32({plus(reg(op.reg), op.disp)})")
            return plus(pointer, op.outer_disp)
        if mode == "pc_double_disp":
            pointer = self.temp("const uint32_t", "pointer", f"mem_read32({const(op.value)})")
            return plus(pointer, op.outer_disp)
        if mode == "direct_deferred":
            return self.temp("const uint32_t", "pointer", f"mem_read32({const(op.value)})")
        if mode == "direct_deferred_indexed":
            pointer = self.temp("const uint32_t", "pointer", f"mem_read32({const(op.value)})")
            return f"{pointer} + {index()}"
        raise Unsupported(f"addressing mode {mode}")

    # ---- am1: read a value ----

    def read_value(self, op, dim, hint="source"):
        if op.mode == "reg":
            return self.temp("const uint32_t", hint, sized(reg(op.reg), dim))
        if op.mode == "immediate":
            mask = {0: 0xFF, 1: 0xFFFF, 2: 0xFFFFFFFF}[dim]
            return const(op.value & mask)
        if op.mode == "autoinc":
            value = self.temp("const uint32_t", hint, f"mem_read{BITS[dim]}({reg(op.reg)})")
            self.emit(f"{reg(op.reg)} += {SIZES[dim]};")
            return value
        if op.mode == "autodec":
            self.emit(f"{reg(op.reg)} -= {SIZES[dim]};")
            return self.temp("const uint32_t", hint, f"mem_read{BITS[dim]}({reg(op.reg)})")
        address = self.effective_address(op, dim, "am1")
        return self.temp("const uint32_t", hint, f"mem_read{BITS[dim]}({address})")

    # ---- am2: compute an address, or name a register ----

    def address_of(self, op, dim, hint="address"):
        if op.mode == "reg":
            return Location(register=op.reg)
        if op.mode == "immediate":
            # MAME's am2Immediate hands the immediate back as if it were an address
            return Location(address=const(op.value))
        if op.mode == "autoinc":
            address = self.temp("const uint32_t", hint, reg(op.reg))
            self.emit(f"{reg(op.reg)} += {SIZES[dim]};")
            return Location(address=address)
        if op.mode == "autodec":
            self.emit(f"{reg(op.reg)} -= {SIZES[dim]};")
            return Location(address=self.temp("const uint32_t", hint, reg(op.reg)))
        expression = self.effective_address(op, dim, "am2")
        if expression.startswith("0x") and expression.endswith("u") and " " not in expression:
            return Location(address=expression)
        return Location(address=self.temp("const uint32_t", hint, expression))

    def bit_address_of(self, op):
        """MAME's bam2 for bit strings: (byte address C, bit offset C)."""
        if op.mode == "reg_indirect":
            return self.temp("const uint32_t", "address", reg(op.reg)), "0u"
        if op.mode == "disp":
            return self.temp("const uint32_t", "address", reg(op.reg)), const(op.disp)
        raise Unsupported(f"bit addressing mode {op.mode}")

    # ---- am3: write a value ----

    def write_value(self, op, dim, value):
        if op.mode == "reg":
            self.store(Location(register=op.reg), dim, value)
        elif op.mode == "immediate":
            raise Unsupported("write to an immediate operand")
        elif op.mode == "autoinc":
            self.emit(f"mem_write{BITS[dim]}({reg(op.reg)}, {sized(value, dim)});")
            self.emit(f"{reg(op.reg)} += {SIZES[dim]};")
        elif op.mode == "autodec":
            self.emit(f"{reg(op.reg)} -= {SIZES[dim]};")
            self.emit(f"mem_write{BITS[dim]}({reg(op.reg)}, {sized(value, dim)});")
        else:
            address = self.effective_address(op, dim, "am3")
            self.emit(f"mem_write{BITS[dim]}({address}, {sized(value, dim)});")

    # ---- MAME's F12LOADOP / F12STOREOP ----

    def load(self, location, dim, hint="value"):
        if location.register is not None:
            return self.temp(C_TYPES[dim], hint, sized(reg(location.register), dim))
        return self.temp(C_TYPES[dim], hint, f"mem_read{BITS[dim]}({location.address})")

    def store(self, location, dim, value):
        if location.register is not None:
            if dim == 0:
                self.emit(f"reg_write8({location.register}, {value});")
            elif dim == 1:
                self.emit(f"reg_write16({location.register}, {value});")
            else:
                self.emit(f"{reg(location.register)} = {value};")
        else:
            self.emit(f"mem_write{BITS[dim]}({location.address}, {sized(value, dim)});")

    # ---- MAME's F12DecodeOperands / F12DecodeFirstOperand / F12WriteSecondOperand ----

    def format_byte(self, instruction):
        return self.mem.r8(instruction.address + 1)

    def f12_operands(self, instruction, kind1, dim1, kind2, dim2):
        """Both operands of a format 1/2 instruction, decoded in MAME's order."""
        code = self.format_byte(instruction)
        operands = instruction.operands
        if code & 0x80:
            first = self._decode(operands[0], kind1, dim1, "source")
            second = self._decode(operands[1], kind2, dim2, "address")
        elif code & 0x20:
            # register second operand is read before the first operand's addressing mode runs
            second = self._register_operand(code & 0x1F, kind2, dim2, "address")
            first = self._decode(operands[0], kind1, dim1, "source")
        else:
            first = self._register_operand(code & 0x1F, kind1, dim1, "source")
            second = self._decode(operands[1], kind2, dim2, "address")
        return first, second

    def f12_first(self, instruction, kind1, dim1):
        """First operand of a move-like instruction, and the operand the result goes to."""
        code = self.format_byte(instruction)
        operands = instruction.operands
        if code & 0x80 or code & 0x20:
            first = self._decode(operands[0], kind1, dim1, "source")
        else:
            first = self._register_operand(code & 0x1F, kind1, dim1, "source")
        if not code & 0x80 and code & 0x20:
            destination = dis.Operand("reg", 2, reg(code & 0x1F), reg=code & 0x1F)
        else:
            destination = operands[1]
        return first, destination

    def _decode(self, op, kind, dim, hint):
        if kind == "value":
            return self.read_value(op, dim, hint)
        return self.address_of(op, dim, hint)

    def _register_operand(self, index, kind, dim, hint):
        if kind == "value":
            return self.temp("const uint32_t", hint, sized(reg(index), dim))
        return Location(register=index)


# ---- the operations ----

ARITHMETIC = {
    "add": lambda bits, v, s: f"v60_add{bits}({v}, (uint{bits}_t){s}, 0)",
    "addc": lambda bits, v, s: f"v60_add{bits}({v}, (uint{bits}_t){s}, cpu.cy)",
    "sub": lambda bits, v, s: f"v60_sub{bits}({v}, (uint{bits}_t){s}, 0)",
    "subc": lambda bits, v, s: f"v60_sub{bits}({v}, (uint{bits}_t){s}, cpu.cy)",
    "and": lambda bits, v, s: f"(uint{bits}_t)v60_logic{bits}({v} & {s})",
    "or": lambda bits, v, s: f"(uint{bits}_t)v60_logic{bits}({v} | {s})",
    "xor": lambda bits, v, s: f"(uint{bits}_t)v60_logic{bits}({v} ^ {s})",
    "mul": lambda bits, v, s: f"v60_mul{bits}({v}, {s})",
    "mulu": lambda bits, v, s: f"v60_mulu{bits}({v}, {s})",
    "div": lambda bits, v, s: f"v60_div{bits}({v}, {s})",
    "divu": lambda bits, v, s: f"v60_divu{bits}({v}, {s})",
    "rem": lambda bits, v, s: f"v60_rem{bits}({v}, {s})",
    "remu": lambda bits, v, s: f"v60_remu{bits}({v}, {s})",
}

# Shifts and rotates read an 8-bit count and modify an operand of the size in their name
SHIFTS = {"shl": "v60_shl", "sha": "v60_sha", "rot": "v60_rot", "rotc": "v60_rotc"}

SINGLE_BITS = {"set1": "v60_set1", "clr1": "v60_clr1", "not1": "v60_not1"}

DIMENSIONS = {"b": 0, "h": 1, "w": 2}

CONDITIONS = {
    "bv": "cpu.ov", "bnv": "!cpu.ov",
    "bl": "cpu.cy", "bnl": "!cpu.cy",
    "be": "cpu.z", "bne": "!cpu.z",
    "bnh": "(cpu.cy | cpu.z)", "bh": "!(cpu.cy | cpu.z)",
    "bn": "cpu.s", "bp": "!cpu.s",
    "blt": "(cpu.s ^ cpu.ov)", "bge": "!(cpu.s ^ cpu.ov)",
    "ble": "((cpu.s ^ cpu.ov) | cpu.z)", "bgt": "!((cpu.s ^ cpu.ov) | cpu.z)",
}

DECREMENT_CONDITIONS = {
    "dbv": "cpu.ov", "dbnv": "!cpu.ov", "dbl": "cpu.cy", "dbnl": "!cpu.cy",
    "dbe": "cpu.z", "dbne": "!cpu.z", "dbnh": "(cpu.cy | cpu.z)", "dbh": "!(cpu.cy | cpu.z)",
    "dbn": "cpu.s", "dbp": "!cpu.s", "dblt": "(cpu.s ^ cpu.ov)", "dbge": "!(cpu.s ^ cpu.ov)",
    "dble": "((cpu.s ^ cpu.ov) | cpu.z)", "dbgt": "!((cpu.s ^ cpu.ov) | cpu.z)", "dbr": None,
}


def split(mnemonic):
    """'add.w' -> ('add', 2); 'mov' -> ('mov', None)."""
    if "." in mnemonic:
        name, suffix = mnemonic.split(".", 1)
        return name, DIMENSIONS.get(suffix)
    return mnemonic, None


def translate_data(block, instruction):
    """Instructions that only move data and set flags. Returns False if the mnemonic is not one of them."""
    name, dim = split(instruction.mnemonic)
    mnemonic = instruction.mnemonic

    if name in ARITHMETIC and dim is not None:
        source, dest = block.f12_operands(instruction, "value", dim, "address", dim)
        value = block.load(dest, dim)
        block.store(dest, dim, ARITHMETIC[name](BITS[dim], value, source))
        return True

    if name in SHIFTS and dim is not None:
        count, dest = block.f12_operands(instruction, "value", 0, "address", dim)
        value = block.load(dest, dim)
        block.store(dest, dim, f"({C_TYPES[dim]}){SHIFTS[name]}({value}, {count}, {BITS[dim]})")
        return True

    if mnemonic in SINGLE_BITS:
        bit, dest = block.f12_operands(instruction, "value", 2, "address", 2)
        value = block.load(dest, 2)
        block.store(dest, 2, f"{SINGLE_BITS[mnemonic]}({value}, {bit})")
        return True

    if name == "cmp" and dim is not None:
        first, second = block.f12_operands(instruction, "value", dim, "value", dim)
        bits = BITS[dim]
        block.emit(f"v60_sub{bits}((uint{bits}_t){second}, (uint{bits}_t){first}, 0);")
        return True

    if mnemonic == "test1":
        bit, value = block.f12_operands(instruction, "value", 2, "value", 2)
        block.emit(f"v60_bit_test({value}, {bit});")
        return True

    if name == "test" and dim is not None:
        value = block.read_value(instruction.operands[0], dim, "value")
        block.emit(f"v60_test{BITS[dim]}({value});")
        return True

    if name in ("inc", "dec") and dim is not None:
        location = block.address_of(instruction.operands[0], dim)
        value = block.load(location, dim)
        operation = "add" if name == "inc" else "sub"
        block.store(location, dim, f"v60_{operation}{BITS[dim]}({value}, 1, 0)")
        return True

    moves = {
        "mov.b": (0, 0, lambda v: v),
        "mov.h": (1, 1, lambda v: v),
        "mov.w": (2, 2, lambda v: v),
        "movs.bh": (0, 1, lambda v: f"(uint16_t)(int8_t)({v} & 0xFF)"),
        "movs.bw": (0, 2, lambda v: f"(uint32_t)(int32_t)(int8_t)({v} & 0xFF)"),
        "movs.hw": (1, 2, lambda v: f"(uint32_t)(int32_t)(int16_t)({v} & 0xFFFF)"),
        "movz.bh": (0, 1, lambda v: v),
        "movz.bw": (0, 2, lambda v: v),
        "movz.hw": (1, 2, lambda v: v),
        "movt.hb": (1, 0, lambda v: f"v60_movt({v}, 16, 8)"),
        "movt.wb": (2, 0, lambda v: f"v60_movt({v}, 32, 8)"),
        "movt.wh": (2, 1, lambda v: f"v60_movt({v}, 32, 16)"),
        "not.b": (0, 0, lambda v: f"v60_logic8(~{v})"),
        "not.h": (1, 1, lambda v: f"v60_logic16(~{v})"),
        "not.w": (2, 2, lambda v: f"v60_logic32(~{v})"),
        "setf": (0, 0, lambda v: f"v60_setf({v})"),
        "rvbyt": (2, 2, lambda v: f"v60_byte_swap({v})"),
    }
    if mnemonic in moves:
        dim1, dim2, convert = moves[mnemonic]
        source, dest = block.f12_first(instruction, "value", dim1)
        value = block.temp(C_TYPES[dim2], "value", sized(convert(source), dim2))
        block.write_value(dest, dim2, value)
        return True

    if name == "neg" and dim is not None:
        source, dest = block.f12_first(instruction, "value", dim)
        signed = {0: "int8_t", 1: "int16_t", 2: "int32_t"}[dim]
        value = block.temp(C_TYPES[dim], "value", f"v60_sub{BITS[dim]}(0, ({signed}){source}, 0)")
        block.emit(f"cpu.cy = {value} != 0;")
        block.write_value(dest, dim, value)
        return True

    if name == "movea":
        location, dest = block.f12_first(instruction, "address", dim)
        # MAME's quirk: a register operand yields the register number
        value = const(location.register) if location.register is not None else location.address
        block.write_value(dest, 2, value)
        return True

    if name == "xch" and dim is not None:
        first, second = block.f12_operands(instruction, "address", dim, "address", dim)
        value = block.load(first, dim, "first_value")
        other = block.load(second, dim, "second_value")
        block.store(first, dim, other)
        block.store(second, dim, value)
        return True

    if mnemonic == "updpsw.w":
        value, mask = block.f12_operands(instruction, "value", 2, "value", 2)
        block.emit(f"v60_write_psw((v60_read_psw() & ~({mask} & 0xFFFFFFu)) | ({value} & 0xFFFFFFu & {mask}));")
        return True

    if mnemonic == "ldpr":
        source, number = block.f12_operands(instruction, "address", 2, "value", 2)
        value = reg(source.register) if source.register is not None else source.address
        block.emit(f"v60_load_privileged({number}, {value});")
        return True

    if mnemonic in ("divx", "divux"):
        divisor, dest = block.f12_operands(instruction, "value", 2, "address", 3)
        register = dest.register if dest.register is not None else -1
        address = dest.address if dest.register is None else "0"
        block.emit(f"v60_{mnemonic}({divisor}, {register}, {address});")
        return True

    if mnemonic == "push":
        value = block.read_value(instruction.operands[0], 2, "value")
        block.emit(f"push32({value});")
        return True

    if mnemonic == "pop":
        value = block.temp("const uint32_t", "value", "pop32()")
        block.write_value(instruction.operands[0], 2, value)
        return True

    if mnemonic in ("pushm", "popm"):
        op = instruction.operands[0]
        if op.mode != "immediate":
            raise Unsupported(f"{mnemonic} with a computed register list")
        mask = op.value
        if mnemonic == "pushm":
            if mask & (1 << 31):
                block.emit("push32(v60_read_psw());")
            for index in range(30, -1, -1):
                if mask & (1 << index):
                    block.emit(f"push32({reg(index)});")
        else:
            for index in range(31):
                if mask & (1 << index):
                    block.emit(f"{reg(index)} = pop32();")
            if mask & (1 << 31):
                block.emit("v60_write_psw((v60_read_psw() & 0xFFFF0000u) | mem_read16(SP));")
                block.emit("SP += 4;")
        return True

    if mnemonic in ("movcu.b", "movcu.h", "movcfu.b", "movcfu.h", "movcd.b", "movcd.h", "movcfd.b", "movcfd.h"):
        dim = DIMENSIONS[mnemonic[-1]]
        source, source_length, dest, dest_length = instruction.operands
        source_address = block.address_of(source, dim, "source")
        length1 = block.read_value(source_length, 2, "source_length") if source_length.mode == "reg" \
            else const(source_length.value)
        dest_address = block.address_of(dest, dim, "dest")
        length2 = block.read_value(dest_length, 2, "dest_length") if dest_length.mode == "reg" \
            else const(dest_length.value)
        if source_address.register is not None or dest_address.register is not None:
            raise Unsupported(f"{mnemonic} with a register operand")
        fill = "true" if "f" in mnemonic.split(".")[0][3:] else "false"
        downward = "true" if mnemonic.startswith(("movcd", "movcfd")) else "false"
        block.emit(f"v60_move_string({source_address.address}, {length1}, {dest_address.address}, {length2}, "
                   f"{SIZES[dim]}, {fill}, {downward});")
        return True

    if mnemonic in ("movbsd", "movbsu"):
        source, length, dest = instruction.operands
        source_address, source_offset = block.bit_address_of(source)
        bits = block.read_value(length, 2, "length") if length.mode == "reg" else const(length.value)
        dest_address, dest_offset = block.bit_address_of(dest)
        helper = "v60_move_bits_down" if mnemonic == "movbsd" else "v60_move_bits_up"
        block.emit(f"{helper}({source_address}, {source_offset}, {dest_address}, {dest_offset}, {bits});")
        return True

    if mnemonic in ("nop", "halt"):
        block.emit("/* nothing: MAME's core does not wait on halt */" if mnemonic == "halt" else "/* nop */")
        return True

    return False
