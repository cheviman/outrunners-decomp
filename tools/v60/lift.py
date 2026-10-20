# license:BSD-3-Clause
# copyright-holders:Farfetch'd, R. Belmont, Sebastian Toyos
"""
Name:        lift.py
Description: translates decoded V60 data instructions into IR statements that do what MAME's V60 core does
Version:     0.1.0
Created:     11-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

The same rules as codegen.py, which this replaces for data instructions: operands are decoded in
MAME's order (am1 reads a value, am2 yields an address or names a register, am3 writes a value), every
memory read becomes a temporary in the order MAME makes it, and every flag MAME sets is set here with
an explicit value. Control instructions (branches, calls, returns) are left to the caller.
"""
from . import disassembler as dis
from . import ir

SIZES = {0: 1, 1: 2, 2: 4, 3: 8}
BITS = {0: 8, 1: 16, 2: 32}
DIMENSIONS = {"b": 0, "h": 1, "w": 2}


class Unsupported(Exception):
    """An instruction or addressing mode the lifter does not translate."""


class Location:
    """An am2 result: a register (MAME's amflag = 1) or a memory address expression."""

    def __init__(self, register=None, address=None):
        self.register = register
        self.address = address


class Lifter:
    """The IR statements of one instruction."""

    def __init__(self, mem, prefix="t"):
        self.mem = mem
        self.statements = []
        self.prefix = prefix
        self.count = 0

    def emit(self, statement):
        self.statements.append(statement)

    def temp(self, value, hint="value"):
        """Binds 'value' to a new temporary, so it is computed here, in order."""
        self.count += 1
        temp = ir.Temp(f"{self.prefix}{self.count}_{hint}", value.bits)
        self.emit(ir.SetTemp(temp, value))
        return temp

    def load(self, address, bits, hint="value"):
        return self.temp(ir.Load(address, bits), hint)

    # ---- effective addresses shared by am1 / am2 / am3 ----

    def effective_address(self, op, dim, flavor):
        scale = SIZES[dim]
        if flavor == "am3" and op.mode == "direct_deferred_indexed":
            scale = 1    # MAME's am3DirectAddressDeferredIndexed does not scale the index

        def index():
            value = ir.Reg(op.index_reg)
            return value if scale == 1 else ir.op("mul", (value, ir.const(scale)), 32)

        def plus(base, offset):
            return base if offset == 0 else ir.op("add", (base, ir.const(offset)), 32)

        mode = op.mode
        if mode == "reg_indirect":
            return ir.Reg(op.reg)
        if mode == "reg_indirect_indexed":
            return ir.op("add", (ir.Reg(op.reg), index()), 32)
        if mode == "disp":
            return plus(ir.Reg(op.reg), op.disp)
        if mode == "disp_indexed":
            return ir.op("add", (plus(ir.Reg(op.reg), op.disp), index()), 32)
        if mode in ("pc_disp", "direct"):
            return ir.const(op.value)
        if mode in ("pc_disp_indexed", "direct_indexed"):
            return ir.op("add", (ir.const(op.value), index()), 32)
        if mode == "disp_indirect":
            return self.load(plus(ir.Reg(op.reg), op.disp), 32, "pointer")
        if mode == "disp_indirect_indexed":
            return ir.op("add", (self.load(plus(ir.Reg(op.reg), op.disp), 32, "pointer"), index()), 32)
        if mode in ("pc_disp_indirect", "direct_deferred"):
            return self.load(ir.const(op.value), 32, "pointer")
        if mode in ("pc_disp_indirect_indexed", "direct_deferred_indexed"):
            return ir.op("add", (self.load(ir.const(op.value), 32, "pointer"), index()), 32)
        if mode == "double_disp":
            return plus(self.load(plus(ir.Reg(op.reg), op.disp), 32, "pointer"), op.outer_disp)
        if mode == "pc_double_disp":
            return plus(self.load(ir.const(op.value), 32, "pointer"), op.outer_disp)
        raise Unsupported(f"addressing mode {mode}")

    # ---- am1: read a value ----

    def read_value(self, op, dim, hint="source"):
        bits = BITS[dim]
        if op.mode == "reg":
            return self.temp(ir.trunc(ir.Reg(op.reg), bits), hint)
        if op.mode == "immediate":
            return ir.const(op.value, bits)
        if op.mode == "autoinc":
            value = self.load(ir.Reg(op.reg), bits, hint)
            self.emit(ir.SetReg(op.reg, ir.op("add", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
            return value
        if op.mode == "autodec":
            self.emit(ir.SetReg(op.reg, ir.op("sub", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
            return self.load(ir.Reg(op.reg), bits, hint)
        return self.load(self.effective_address(op, dim, "am1"), bits, hint)

    # ---- am2: compute an address, or name a register ----

    def address_of(self, op, dim, hint="address"):
        if op.mode == "reg":
            return Location(register=op.reg)
        if op.mode == "immediate":
            # MAME's am2Immediate hands the immediate back as if it were an address
            return Location(address=ir.const(op.value))
        if op.mode == "autoinc":
            address = self.temp(ir.Reg(op.reg), hint)
            self.emit(ir.SetReg(op.reg, ir.op("add", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
            return Location(address=address)
        if op.mode == "autodec":
            self.emit(ir.SetReg(op.reg, ir.op("sub", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
            return Location(address=self.temp(ir.Reg(op.reg), hint))
        address = self.effective_address(op, dim, "am2")
        if isinstance(address, (ir.Const, ir.Temp)):
            return Location(address=address)
        return Location(address=self.temp(address, hint))

    def bit_address_of(self, op):
        """MAME's bam2 for bit strings: (byte address, bit offset)."""
        if op.mode == "reg_indirect":
            return self.temp(ir.Reg(op.reg), "address"), ir.const(0)
        if op.mode == "disp":
            return self.temp(ir.Reg(op.reg), "address"), ir.const(op.disp)
        if op.mode == "reg_indirect_indexed":
            # MAME's bam2RegisterIndirectIndexed: the address in the base register, the bit offset in the
            # index register (any size: the move divides it into bytes)
            return self.temp(ir.Reg(op.reg), "address"), self.temp(ir.Reg(op.index_reg), "offset")
        raise Unsupported(f"bit addressing mode {op.mode}")

    # ---- am3: write a value ----

    def write_value(self, op, dim, value):
        bits = BITS[dim]
        if op.mode == "reg":
            self.store(Location(register=op.reg), dim, value)
        elif op.mode == "immediate":
            raise Unsupported("write to an immediate operand")
        elif op.mode == "autoinc":
            self.emit(ir.Store(ir.Reg(op.reg), ir.trunc(value, bits), bits))
            self.emit(ir.SetReg(op.reg, ir.op("add", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
        elif op.mode == "autodec":
            self.emit(ir.SetReg(op.reg, ir.op("sub", (ir.Reg(op.reg), ir.const(SIZES[dim])), 32)))
            self.emit(ir.Store(ir.Reg(op.reg), ir.trunc(value, bits), bits))
        else:
            address = self.effective_address(op, dim, "am3")
            self.emit(ir.Store(address, ir.trunc(value, bits), bits))

    # ---- MAME's F12LOADOP / F12STOREOP ----

    def load_location(self, location, dim, hint="value"):
        bits = BITS[dim]
        if location.register is not None:
            return self.temp(ir.trunc(ir.Reg(location.register), bits), hint)
        return self.load(location.address, bits, hint)

    def store(self, location, dim, value):
        bits = BITS[dim]
        value = ir.trunc(value, bits) if value.bits > bits else ir.zext(value, bits)
        if location.register is None:
            self.emit(ir.Store(location.address, value, bits))
            return
        index = location.register
        if bits == 32:
            self.emit(ir.SetReg(index, value))
        else:
            # SETREG8 / SETREG16 keep the upper bits of the register
            keep = ir.op("and", (ir.Reg(index), ir.const(~ir.MASKS[bits])), 32)
            self.emit(ir.SetReg(index, ir.op("or", (keep, ir.zext(value, 32)), 32)))

    # ---- MAME's F12DecodeOperands / F12DecodeFirstOperand ----

    def format_byte(self, instruction):
        return self.mem.r8(instruction.address + 1)

    def f12_operands(self, instruction, kind1, dim1, kind2, dim2):
        code = self.format_byte(instruction)
        operands = instruction.operands
        if code & 0x80:
            first = self._decode(operands[0], kind1, dim1, "source")
            second = self._decode(operands[1], kind2, dim2, "operand")
        elif code & 0x20:
            # register second operand is read before the first operand's addressing mode runs
            second = self._register_operand(code & 0x1F, kind2, dim2, "operand")
            first = self._decode(operands[0], kind1, dim1, "source")
        else:
            first = self._register_operand(code & 0x1F, kind1, dim1, "source")
            second = self._decode(operands[1], kind2, dim2, "operand")
        return first, second

    def f12_same_register(self, instruction):
        """The register both operands of a format 1/2 instruction name (xor R5, R5), or None."""
        code = self.format_byte(instruction)
        operands = instruction.operands
        if code & 0x80:
            first, second = operands[0], operands[1]
            if first.mode == "reg" and second.mode == "reg" and first.reg == second.reg:
                return first.reg
            return None
        other = operands[0] if code & 0x20 else operands[1]
        return other.reg if other.mode == "reg" and other.reg == code & 0x1F else None

    def f12_first(self, instruction, kind1, dim1):
        code = self.format_byte(instruction)
        operands = instruction.operands
        if code & 0x80 or code & 0x20:
            first = self._decode(operands[0], kind1, dim1, "source")
        else:
            first = self._register_operand(code & 0x1F, kind1, dim1, "source")
        if not code & 0x80 and code & 0x20:
            destination = dis.Operand("reg", 2, f"R{code & 0x1F}", reg=code & 0x1F)
        else:
            destination = operands[1]
        return first, destination

    def _decode(self, op, kind, dim, hint):
        if kind == "value":
            return self.read_value(op, dim, hint)
        return self.address_of(op, dim, hint)

    def _register_operand(self, index, kind, dim, hint):
        if kind == "value":
            return self.temp(ir.trunc(ir.Reg(index), BITS[dim]), hint)
        return Location(register=index)

    # ---- flag helpers ----

    def set_sz(self, result):
        for statement in ir.flags_of_result(result):
            self.emit(statement)

    def add_flags(self, a, b, carry, result):
        bits = result.bits
        self.emit(ir.SetFlag("cy", ir.op("carry_add", (a, b, carry), 1)))
        self.emit(ir.SetFlag("ov", ir.op("overflow_add", (a, b, carry), 1)))
        self.set_sz(result)

    def sub_flags(self, a, b, borrow, result):
        self.emit(ir.SetFlag("cy", ir.op("carry_sub", (a, b, borrow), 1)))
        self.emit(ir.SetFlag("ov", ir.op("overflow_sub", (a, b, borrow), 1)))
        self.set_sz(result)


def split(mnemonic):
    """'add.w' -> ('add', 2); 'mov' -> ('mov', None)."""
    if "." in mnemonic:
        name, suffix = mnemonic.split(".", 1)
        return name, DIMENSIONS.get(suffix)
    return mnemonic, None


def _helper_flag(name, args, flag):
    return ir.op(f"helper:{name}_{flag}", args, 1)


def lift_data(lifter, instruction):
    """IR for an instruction that only moves data and sets flags; False if it is not one of them."""
    name, dim = split(instruction.mnemonic)
    mnemonic = instruction.mnemonic
    emit = lifter.emit

    if name in ("add", "addc", "sub", "subc") and dim is not None:
        bits = BITS[dim]
        source, dest = lifter.f12_operands(instruction, "value", dim, "address", dim)
        value = lifter.load_location(dest, dim)
        # the carry going in is read before the instruction sets a new one
        carry = lifter.temp(ir.Flag("cy"), "carry_in") if name.endswith("c") else ir.const(0, 1)
        if name.startswith("add"):
            total = ir.op("add", (ir.op("add", (value, source), bits), ir.zext(carry, bits)), bits)
            result = lifter.temp(total, "result")
            lifter.add_flags(value, source, carry, result)
        else:
            total = ir.op("sub", (ir.op("sub", (value, source), bits), ir.zext(carry, bits)), bits)
            result = lifter.temp(total, "result")
            lifter.sub_flags(value, source, carry, result)
        lifter.store(dest, dim, result)
        return True

    if name in ("xor", "sub") and dim is not None and lifter.f12_same_register(instruction) is not None:
        # xor R, R and sub R, R give 0 whatever R holds: the register is not read
        bits = BITS[dim]
        result = ir.const(0, bits)
        if name == "sub":
            emit(ir.SetFlag("cy", ir.const(0, 1)))
        emit(ir.SetFlag("ov", ir.const(0, 1)))
        lifter.set_sz(result)
        lifter.store(Location(register=lifter.f12_same_register(instruction)), dim, result)
        return True

    if name in ("and", "or", "xor") and dim is not None:
        bits = BITS[dim]
        source, dest = lifter.f12_operands(instruction, "value", dim, "address", dim)
        value = lifter.load_location(dest, dim)
        result = lifter.temp(ir.op(name, (value, source), bits), "result")
        emit(ir.SetFlag("ov", ir.const(0, 1)))
        lifter.set_sz(result)
        lifter.store(dest, dim, result)
        return True

    if name in ("mul", "mulu") and dim is not None:
        bits = BITS[dim]
        source, dest = lifter.f12_operands(instruction, "value", dim, "address", dim)
        value = lifter.load_location(dest, dim)
        if name == "mul":
            product = ir.op("mul", (ir.sext(value, 2 * bits), ir.sext(source, 2 * bits)), 2 * bits)
        else:
            product = ir.op("mul", (ir.zext(value, 2 * bits), ir.zext(source, 2 * bits)), 2 * bits)
        product = lifter.temp(product, "product")
        result = lifter.temp(ir.trunc(product, bits), "result")
        lifter.set_sz(result)
        high = ir.op("shr", (product, ir.const(bits, 8)), 2 * bits)
        emit(ir.SetFlag("ov", ir.op("ne", (high, ir.const(0, 2 * bits)), 1)))
        lifter.store(dest, dim, result)
        return True

    if name in ("div", "divu", "rem", "remu") and dim is not None:
        bits = BITS[dim]
        source, dest = lifter.f12_operands(instruction, "value", dim, "address", dim)
        value = lifter.load_location(dest, dim)
        args = (value, source)
        if name == "div":
            emit(ir.SetFlag("ov", _helper_flag("div", args, "ov")))
        else:
            emit(ir.SetFlag("ov", ir.const(0, 1)))
        result = lifter.temp(ir.op(f"helper:{name}", args, bits), "result")
        lifter.set_sz(result)
        lifter.store(dest, dim, result)
        return True

    if name in ("shl", "sha", "rot", "rotc") and dim is not None:
        bits = BITS[dim]
        count, dest = lifter.f12_operands(instruction, "value", 0, "address", dim)
        value = lifter.load_location(dest, dim)
        args = (value, count) if name != "rotc" else (value, count, ir.Flag("cy"))
        carry = lifter.temp(_helper_flag(name, args, "cy"), "carry")
        overflow = lifter.temp(_helper_flag(name, args, "ov"), "overflow")
        result = lifter.temp(ir.op(f"helper:{name}", args, bits), "result")
        emit(ir.SetFlag("cy", carry))
        emit(ir.SetFlag("ov", overflow))
        lifter.set_sz(result)
        lifter.store(dest, dim, result)
        return True

    if mnemonic in ("set1", "clr1", "not1"):
        number, dest = lifter.f12_operands(instruction, "value", 2, "address", 2)
        value = lifter.load_location(dest, 2)
        mask = ir.op("shl", (ir.const(1), number), 32)
        tested = ir.op("ne", (ir.op("and", (value, mask), 32), ir.const(0)), 1)
        emit(ir.SetFlag("cy", tested))
        emit(ir.SetFlag("z", ir.op("eq", (ir.op("and", (value, mask), 32), ir.const(0)), 1)))
        if mnemonic == "set1":
            result = ir.op("or", (value, mask), 32)
        elif mnemonic == "clr1":
            result = ir.op("and", (value, ir.op("not", (mask,), 32)), 32)
        else:
            result = ir.op("xor", (value, mask), 32)
        lifter.store(dest, 2, lifter.temp(result, "result"))
        return True

    if name == "cmp" and dim is not None:
        bits = BITS[dim]
        first, second = lifter.f12_operands(instruction, "value", dim, "value", dim)
        difference = lifter.temp(ir.op("sub", (second, first), bits), "difference")
        lifter.sub_flags(second, first, ir.const(0, 1), difference)
        return True

    if mnemonic == "test1":
        number, value = lifter.f12_operands(instruction, "value", 2, "value", 2)
        masked = ir.op("and", (value, ir.op("shl", (ir.const(1), number), 32)), 32)
        emit(ir.SetFlag("cy", ir.op("ne", (masked, ir.const(0)), 1)))
        emit(ir.SetFlag("z", ir.op("eq", (masked, ir.const(0)), 1)))
        return True

    if name == "test" and dim is not None:
        value = lifter.read_value(instruction.operands[0], dim, "value")
        lifter.set_sz(value)
        emit(ir.SetFlag("cy", ir.const(0, 1)))
        emit(ir.SetFlag("ov", ir.const(0, 1)))
        return True

    if name in ("inc", "dec") and dim is not None:
        bits = BITS[dim]
        location = lifter.address_of(instruction.operands[0], dim)
        value = lifter.load_location(location, dim)
        one = ir.const(1, bits)
        zero = ir.const(0, 1)
        if name == "inc":
            result = lifter.temp(ir.op("add", (value, one), bits), "result")
            lifter.add_flags(value, one, zero, result)
        else:
            result = lifter.temp(ir.op("sub", (value, one), bits), "result")
            lifter.sub_flags(value, one, zero, result)
        lifter.store(location, dim, result)
        return True

    moves = {
        "mov.b": (0, 0), "mov.h": (1, 1), "mov.w": (2, 2),
        "movs.bh": (0, 1), "movs.bw": (0, 2), "movs.hw": (1, 2),
        "movz.bh": (0, 1), "movz.bw": (0, 2), "movz.hw": (1, 2),
        "movt.hb": (1, 0), "movt.wb": (2, 0), "movt.wh": (2, 1),
        "not.b": (0, 0), "not.h": (1, 1), "not.w": (2, 2),
        "setf": (0, 0), "rvbyt": (2, 2),
    }
    if mnemonic in moves:
        dim1, dim2 = moves[mnemonic]
        bits1, bits2 = BITS[dim1], BITS[dim2]
        source, dest = lifter.f12_first(instruction, "value", dim1)
        if name in ("mov", "movz"):
            value = ir.zext(source, bits2)
        elif name == "movs":
            value = ir.sext(source, bits2)
        elif name == "movt":
            value = ir.trunc(source, bits2)
            emit(ir.SetFlag("ov", _helper_flag("movt", (source, ir.const(bits2, 8)), "ov")))
        elif name == "not":
            value = lifter.temp(ir.op("not", (source,), bits1), "result")
            emit(ir.SetFlag("ov", ir.const(0, 1)))
            lifter.set_sz(value)
        elif name == "setf":
            value = ir.zext(ir.op("helper:setf", (source, ir.Flag("z"), ir.Flag("s"), ir.Flag("ov"), ir.Flag("cy")),
                                  1), 8)
        else:
            value = ir.op("helper:byte_swap", (source,), 32)
        lifter.write_value(dest, dim2, lifter.temp(value, "value") if not isinstance(value, ir.Temp) else value)
        return True

    if name == "neg" and dim is not None:
        bits = BITS[dim]
        source, dest = lifter.f12_first(instruction, "value", dim)
        result = lifter.temp(ir.op("neg", (source,), bits), "result")
        emit(ir.SetFlag("ov", ir.bit(ir.op("and", (source, result), bits), bits - 1)))
        lifter.set_sz(result)
        emit(ir.SetFlag("cy", ir.op("ne", (result, ir.const(0, bits)), 1)))
        lifter.write_value(dest, dim, result)
        return True

    if name == "movea":
        location, dest = lifter.f12_first(instruction, "address", dim)
        # MAME's quirk: a register operand yields the register number
        value = ir.const(location.register) if location.register is not None else location.address
        lifter.write_value(dest, 2, value)
        return True

    if name == "xch" and dim is not None:
        first, second = lifter.f12_operands(instruction, "address", dim, "address", dim)
        value = lifter.load_location(first, dim, "first_value")
        other = lifter.load_location(second, dim, "second_value")
        lifter.store(first, dim, other)
        lifter.store(second, dim, value)
        return True

    if mnemonic == "updpsw.w":
        value, mask = lifter.f12_operands(instruction, "value", 2, "value", 2)
        emit(ir.Effect("update_psw", (value, mask)))
        return True

    if mnemonic == "ldpr":
        source, number = lifter.f12_operands(instruction, "address", 2, "value", 2)
        value = ir.Reg(source.register) if source.register is not None else source.address
        emit(ir.Effect("load_privileged", (number, value)))
        return True

    if mnemonic in ("divx", "divux"):
        divisor, dest = lifter.f12_operands(instruction, "value", 2, "address", 3)
        register = ir.const(dest.register if dest.register is not None else 0xFFFFFFFF)
        address = dest.address if dest.register is None else ir.const(0)
        emit(ir.Effect(mnemonic, (divisor, register, address)))
        return True

    if mnemonic == "push":
        value = lifter.read_value(instruction.operands[0], 2, "value")
        emit(ir.SetReg(dis_sp(), ir.op("sub", (ir.Reg(dis_sp()), ir.const(4)), 32)))
        emit(ir.Store(ir.Reg(dis_sp()), value, 32))
        return True

    if mnemonic == "pop":
        value = lifter.load(ir.Reg(dis_sp()), 32, "value")
        emit(ir.SetReg(dis_sp(), ir.op("add", (ir.Reg(dis_sp()), ir.const(4)), 32)))
        lifter.write_value(instruction.operands[0], 2, value)
        return True

    if mnemonic in ("pushm", "popm"):
        operand = instruction.operands[0]
        if operand.mode != "immediate":
            raise Unsupported(f"{mnemonic} with a computed register list")
        mask = operand.value
        sp = dis_sp()
        if mnemonic == "pushm":
            if mask & (1 << 31):
                psw = lifter.temp(ir.op("helper:read_psw", (ir.Flag("z"), ir.Flag("s"), ir.Flag("ov"), ir.Flag("cy")),
                                        32), "psw")
                emit(ir.SetReg(sp, ir.op("sub", (ir.Reg(sp), ir.const(4)), 32)))
                emit(ir.Store(ir.Reg(sp), psw, 32))
            for index in range(30, -1, -1):
                if mask & (1 << index):
                    emit(ir.SetReg(sp, ir.op("sub", (ir.Reg(sp), ir.const(4)), 32)))
                    emit(ir.Store(ir.Reg(sp), ir.Reg(index), 32))
        else:
            for index in range(31):
                if mask & (1 << index):
                    value = lifter.load(ir.Reg(sp), 32, "value")
                    emit(ir.SetReg(sp, ir.op("add", (ir.Reg(sp), ir.const(4)), 32)))
                    emit(ir.SetReg(index, value))
            if mask & (1 << 31):
                low = lifter.load(ir.Reg(sp), 16, "psw")
                emit(ir.Effect("write_psw_low", (low,)))
                emit(ir.SetReg(sp, ir.op("add", (ir.Reg(sp), ir.const(4)), 32)))
        return True

    if mnemonic in ("movcu.b", "movcu.h", "movcfu.b", "movcfu.h", "movcd.b", "movcd.h", "movcfd.b", "movcfd.h"):
        dim = DIMENSIONS[mnemonic[-1]]
        source, source_length, dest, dest_length = instruction.operands
        source_address = lifter.address_of(source, dim, "source")
        length1 = lifter.read_value(source_length, 2, "source_length") if source_length.mode == "reg" \
            else ir.const(source_length.value)
        dest_address = lifter.address_of(dest, dim, "dest")
        length2 = lifter.read_value(dest_length, 2, "dest_length") if dest_length.mode == "reg" \
            else ir.const(dest_length.value)
        if source_address.register is not None or dest_address.register is not None:
            raise Unsupported(f"{mnemonic} with a register operand")
        fill = ir.const(1 if "f" in mnemonic.split(".")[0][3:] else 0)
        downward = ir.const(1 if mnemonic.startswith(("movcd", "movcfd")) else 0)
        emit(ir.Effect("move_string", (source_address.address, length1, dest_address.address, length2,
                                       ir.const(SIZES[dim]), fill, downward)))
        return True

    if mnemonic in ("movbsd", "movbsu"):
        source, length, dest = instruction.operands
        if "reg" in (source.mode, dest.mode):
            # a register is no bit address: the V60 cannot run this (MAME's core stops: "BAM2 - 6"); only
            # code the game never calls has it
            emit(ir.Effect("invalid_operand", (ir.const(instruction.address),)))
            return True
        source_address, source_offset = lifter.bit_address_of(source)
        bits = lifter.read_value(length, 2, "length") if length.mode == "reg" else ir.const(length.value)
        dest_address, dest_offset = lifter.bit_address_of(dest)
        helper = "move_bits_down" if mnemonic == "movbsd" else "move_bits_up"
        emit(ir.Effect(helper, (source_address, source_offset, dest_address, dest_offset, bits)))
        return True

    if mnemonic in ("extbfz", "insbfr"):
        # MAME's opEXTBFZ / opINSBFR: the 32 bits at the bit address's byte (address + offset / 8), from
        # bit offset & 7, 'length' of them (the mask (1 << length) - 1, its count taken like x86 does)
        if mnemonic == "extbfz":
            field, length, dest = instruction.operands
        else:
            source, field, length = instruction.operands
            value = lifter.read_value(source, 2, "value")
        address, offset = lifter.bit_address_of(field)
        count = lifter.read_value(length, 2, "length") if length.mode == "reg" else ir.const(length.value)
        byte = lifter.temp(ir.op("add", (address, ir.op("shr", (offset, ir.const(3, 8)), 32)), 32), "field_address")
        word = lifter.load(byte, 32, "field")
        shift = ir.op("and", (offset, ir.const(7)), 32)
        mask = ir.op("sub", (ir.op("shl", (ir.const(1), count), 32), ir.const(1)), 32)
        if mnemonic == "extbfz":
            lifter.write_value(dest, 2, lifter.temp(ir.op("and", (ir.op("shr", (word, shift), 32), mask), 32), "bits"))
        else:
            kept = ir.op("and", (word, ir.op("not", (ir.op("shl", (mask, shift), 32),), 32)), 32)
            inserted = ir.op("shl", (ir.op("and", (mask, value), 32), shift), 32)
            emit(ir.Store(byte, ir.op("or", (kept, inserted), 32), 32))
        return True

    if mnemonic in ("sch0bsu", "sch1bsu"):
        # MAME's opSCHBS: how many bits up from the bit address differ from the one searched for (the
        # length when none matches) into the destination, Z set when none matched; R28 is left at the byte
        # of the last bit looked at (the runtime's search_bits_up)
        field, length, dest = instruction.operands
        if dest.mode != "reg":
            raise Unsupported(f"{mnemonic} into memory")
        address, offset = lifter.bit_address_of(field)
        count = lifter.read_value(length, 2, "length") if length.mode == "reg" else ir.const(length.value)
        searched = ir.const(1 if mnemonic == "sch1bsu" else 0)
        emit(ir.Effect("search_bits_up", (address, offset, count, searched, ir.const(dest.reg))))
        emit(ir.SetFlag("z", ir.op("eq", (ir.Reg(dest.reg), count), 1)))
        return True

    if mnemonic == "nop":
        return True
    if mnemonic == "halt":
        emit(ir.Effect("halt"))    # the V60 waits for an interrupt there; MAME's core goes on at once
        return True

    return False


def dis_sp():
    return 31
