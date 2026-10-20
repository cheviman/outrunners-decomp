# license:BSD-3-Clause
# copyright-holders:Juergen Buchmueller, Sebastian Toyos
"""
Name:        lift.py
Description: translates decoded Z80 data instructions into IR statements that do what MAME's Z80 core does
Version:     0.1.0
Created:     13-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

The registers are the decompiler's: the pairs BC, DE, HL, IX, IY and SP are 16-bit registers whose
bytes are their parts (B is part 1 of BC, C part 0), A is an 8-bit register of its own, and the other
bank (EXX, EX AF,AF') is four more registers. Every memory read becomes a temporary in the order
MAME's core makes it; 16-bit reads and writes are the low byte first.

Of the flags the program can see, S, Z, P/V and C are kept, as the decompiler's s, z, ov and cy: P/V
is the overflow after arithmetic, the parity of the result after logic, shifts and rotates, BC != 0
after a block move, and the opposite of the tested bit after BIT, as MAME's pv_val makes them. H and N
only matter to DAA and to the byte PUSH AF puts on the stack; the program never uses DAA, and what
PUSH AF leaves on the stack is never read back but by POP AF. The undocumented bits 3 and 5 are not
kept either.

Control instructions (jumps, calls, returns, RST, DJNZ) are left to the caller, as are DI, EI, IM and
HALT.
"""
from v60 import ir

from . import disassembler as dis

BC, DE, HL, IX, IY, SP, A = 0, 1, 2, 3, 4, 5, 6
ALTERNATE = {BC: 7, DE: 8, HL: 9, A: 10}
WIDTHS = {BC: 16, DE: 16, HL: 16, IX: 16, IY: 16, SP: 16, A: 8, 7: 16, 8: 16, 9: 16, 10: 8}
NAMES = {BC: "bc", DE: "de", HL: "hl", IX: "ix", IY: "iy", SP: "sp", A: "a",
         7: "bc_alt", 8: "de_alt", 9: "hl_alt", 10: "a_alt"}
PAIRS = {"bc": BC, "de": DE, "hl": HL, "ix": IX, "iy": IY, "sp": SP}
BYTES = {"b": (BC, 1), "c": (BC, 0), "d": (DE, 1), "e": (DE, 0), "h": (HL, 1), "l": (HL, 0),
         "ixh": (IX, 1), "ixl": (IX, 0), "iyh": (IY, 1), "iyl": (IY, 0)}
FLAGS = ("s", "z", "ov", "cy")
ALTERNATE_FLAGS = {"s": "s_alt", "z": "z_alt", "ov": "ov_alt", "cy": "cy_alt"}

ARITHMETIC = ("add", "adc", "sub", "sbc", "and", "xor", "or", "cp")
SHIFTS = ("rlc", "rrc", "rl", "rr", "sla", "sra", "sll", "srl")


class Unsupported(Exception):
    """An instruction the lifter does not translate."""


def reg(index):
    return ir.Reg(index, WIDTHS[index])


def c16(value):
    return ir.const(value, 16)


def c8(value):
    return ir.const(value, 8)


def lnot(value):
    return ir.op("eq", (value, ir.const(0, value.bits)), 1)


def parity(value):
    """1 when the 8-bit value has an even number of bits set: P/V after logic, as MAME's pv() reads it."""
    return ir.op("parity", (value,), 1)


class Lifter:
    """The IR statements of one instruction."""

    def __init__(self, mem, prefix="t", carry=None):
        self.mem = mem
        self.statements = []
        self.prefix = prefix
        # the carry the instruction finds when one before it in its block left a constant (OR A clears it
        # before SBC HL,DE: a plain subtraction), else None
        self.known_carry = carry
        self.count = 0

    def emit(self, statement):
        self.statements.append(statement)

    def temp(self, value, hint="value"):
        self.count += 1
        temp = ir.Temp(f"{self.prefix}{self.count}_{hint}", value.bits)
        self.emit(ir.SetTemp(temp, value))
        return temp

    def load(self, address, bits=8, hint="value"):
        return self.temp(ir.Load(address, bits), hint)

    def held(self, value, hint="value"):
        """'value' as a temporary (a constant as it is): what an instruction reads that sets the flags,
        so that a branch on them can compare what was compared (decompiler/conditions.py), whatever
        happens to the registers afterwards."""
        return value if isinstance(value, (ir.Const, ir.Temp)) else self.temp(value, hint)

    def store(self, address, value):
        self.emit(ir.Store(address, value, value.bits))

    # ---- registers ----

    def read8(self, name):
        if name == "a":
            return reg(A)
        index, part = BYTES[name]
        if part == 0:
            return ir.trunc(reg(index), 8)
        return ir.trunc(ir.op("shr", (reg(index), c8(8)), 16), 8)

    def write8(self, name, value):
        if name == "a":
            self.emit(ir.SetReg(A, value))
            return
        index, part = BYTES[name]
        wide = ir.zext(value, 16)
        if part == 0:
            self.emit(ir.SetReg(index, ir.op("or", (ir.op("and", (reg(index), c16(0xFF00)), 16), wide), 16)))
        else:
            self.emit(ir.SetReg(index, ir.op("or", (ir.op("and", (reg(index), c16(0x00FF)), 16),
                                                    ir.op("shl", (wide, c8(8)), 16)), 16)))

    def read16(self, name):
        return reg(PAIRS[name])

    def write16(self, name, value):
        self.emit(ir.SetReg(PAIRS[name], value))

    # ---- operands ----

    def address(self, operand):
        """The memory address an operand names."""
        if operand.mode == "reg_indirect":
            return reg(PAIRS[operand.reg])
        if operand.mode == "indexed":
            return ir.op("add", (reg(PAIRS[operand.reg]), c16(operand.disp & 0xFFFF)), 16)
        if operand.mode == "direct":
            return c16(operand.value)
        raise Unsupported(f"no address in {operand.text}")

    def value8(self, operand, hint="value"):
        if operand.mode == "reg":
            return self.read8(operand.reg)
        if operand.mode == "immediate":
            return c8(operand.value)
        return self.load(self.address(operand), 8, hint)

    def put8(self, operand, value, address=None):
        if operand.mode == "reg":
            self.write8(operand.reg, value)
        else:
            self.store(address if address is not None else self.address(operand), value)

    def value16(self, operand, hint="value"):
        if operand.mode == "reg":
            return self.read16(operand.reg)
        if operand.mode == "immediate":
            return c16(operand.value)
        return self.load(self.address(operand), 16, hint)

    # ---- flags ----

    def flag(self, name, value):
        self.emit(ir.SetFlag(name, value))

    def sign_zero(self, result):
        self.flag("s", ir.bit(result, result.bits - 1))
        self.flag("z", lnot(result))

    # ---- instruction families ----

    def arithmetic(self, name, value):
        """ADD ADC SUB SBC AND XOR OR CP on A."""
        a = reg(A)
        if value == a and name in ("xor", "sub"):
            # XOR A and SUB A clear A whatever it holds: they read nothing
            self.flag("s", ir.const(0, 1))
            self.flag("z", ir.const(1, 1))
            self.flag("ov", ir.const(1 if name == "xor" else 0, 1))
            self.flag("cy", ir.const(0, 1))
            self.emit(ir.SetReg(A, c8(0)))
            return
        if name in ("and", "xor", "or"):
            # AND A and OR A leave A as it is: only the flags change
            result = self.temp(a, "value") if value == a else self.temp(ir.op(name, (a, value), 8), name)
            self.sign_zero(result)
            self.flag("ov", parity(result))
            self.flag("cy", ir.const(0, 1))
            if value != a:
                self.emit(ir.SetReg(A, result))
            return
        carry = (self.known_carry or ir.Flag("cy")) if name in ("adc", "sbc") else ir.const(0, 1)
        a = self.held(a, "a")
        value = self.held(value, "operand")
        if name in ("add", "adc"):
            # ADD and SUB (and CP) take no carry in: "a + b", not "a + b + 0", so that the passes see the
            # sum or the comparison they know
            whole = ir.op("add", (a, value), 8)
            result = self.temp(whole if name == "add" else ir.op("add", (whole, ir.zext(carry, 8)), 8), "sum")
            self.flag("ov", ir.op("overflow_add", (a, value, carry), 1))
            self.flag("cy", ir.op("carry_add", (a, value, carry), 1))
        else:
            whole = ir.op("sub", (a, value), 8)
            result = self.temp(whole if name != "sbc" else ir.op("sub", (whole, ir.zext(carry, 8)), 8), "difference")
            self.flag("ov", ir.op("overflow_sub", (a, value, carry), 1))
            self.flag("cy", ir.op("carry_sub", (a, value, carry), 1))
        self.sign_zero(result)
        if name != "cp":
            self.emit(ir.SetReg(A, result))

    def shifted(self, name, value):
        """The result of a CB shift or rotate of 'value', with its flags set."""
        carry_in = ir.zext(self.known_carry or ir.Flag("cy"), 8)
        if name == "rlc":
            result = ir.op("or", (ir.op("shl", (value, c8(1)), 8), ir.op("shr", (value, c8(7)), 8)), 8)
        elif name == "rrc":
            result = ir.op("or", (ir.op("shr", (value, c8(1)), 8), ir.op("shl", (value, c8(7)), 8)), 8)
        elif name == "rl":
            result = ir.op("or", (ir.op("shl", (value, c8(1)), 8), carry_in), 8)
        elif name == "rr":
            result = ir.op("or", (ir.op("shr", (value, c8(1)), 8), ir.op("shl", (carry_in, c8(7)), 8)), 8)
        elif name == "sla":
            result = ir.op("shl", (value, c8(1)), 8)
        elif name == "sra":
            result = ir.op("sar", (value, c8(1)), 8)
        elif name == "sll":
            result = ir.op("or", (ir.op("shl", (value, c8(1)), 8), c8(1)), 8)
        else:
            result = ir.op("shr", (value, c8(1)), 8)
        result = self.temp(result, name)
        self.flag("cy", ir.bit(value, 7) if name in ("rlc", "rl", "sla", "sll") else ir.bit(value, 0))
        self.sign_zero(result)
        self.flag("ov", parity(result))
        return result


def is_register(operand):
    return operand.mode == "reg"


def lift_data(lifter, instruction):
    """Appends the statements of a data instruction to the lifter; False for control instructions."""
    mnemonic = instruction.mnemonic
    operands = instruction.operands
    if instruction.copy_to is not None:
        raise Unsupported(f"{instruction.address:04X}: {instruction.text}")

    if mnemonic == "nop":
        return True

    if mnemonic == "ld":
        destination, source = operands
        if destination.mode == "reg" and destination.reg in PAIRS:
            lifter.write16(destination.reg, lifter.value16(source, destination.reg))
        elif source.mode == "reg" and source.reg in PAIRS:
            lifter.store(lifter.address(destination), lifter.read16(source.reg))
        elif destination.mode == "reg" and destination.reg in ("i", "r") or source.mode == "reg" and source.reg in ("i", "r"):
            raise Unsupported(f"{instruction.address:04X}: {instruction.text}")
        else:
            value = lifter.value8(source)
            lifter.put8(destination, value)
        return True

    if mnemonic in ARITHMETIC:
        # "add a,x" names A; "and x", "cp x" do not
        source = operands[-1]
        if mnemonic in ("add", "adc", "sbc") and operands[0].reg in ("hl", "ix", "iy") and len(operands) == 2:
            return lift_add16(lifter, mnemonic, operands[0].reg, source.reg)
        lifter.arithmetic(mnemonic, lifter.value8(source))
        return True

    if mnemonic in ("inc", "dec"):
        operand = operands[0]
        if operand.mode == "reg" and operand.reg in PAIRS:
            step = "add" if mnemonic == "inc" else "sub"
            lifter.write16(operand.reg, ir.op(step, (lifter.read16(operand.reg), c16(1)), 16))
            return True
        address = lifter.address(operand) if operand.mode != "reg" else None
        value = lifter.held(lifter.read8(operand.reg)) if operand.mode == "reg" else lifter.load(address, 8)
        step = "add" if mnemonic == "inc" else "sub"
        result = lifter.temp(ir.op(step, (value, c8(1)), 8), mnemonic)
        lifter.sign_zero(result)
        lifter.flag("ov", ir.op("eq", (result, c8(0x80 if mnemonic == "inc" else 0x7F)), 1))
        lifter.put8(operand, result, address)
        return True

    if mnemonic == "cpl":
        lifter.emit(ir.SetReg(A, ir.op("not", (reg(A),), 8)))
        return True

    if mnemonic == "neg":
        value = lifter.temp(reg(A), "negated")
        lifter.emit(ir.SetReg(A, c8(0)))
        lifter.arithmetic("sub", value)
        return True

    if mnemonic in ("rlca", "rrca", "rla", "rra"):
        a = lifter.temp(reg(A), "rotated")
        carry_in = ir.zext(lifter.known_carry or ir.Flag("cy"), 8)
        if mnemonic == "rlca":
            result = ir.op("or", (ir.op("shl", (a, c8(1)), 8), ir.op("shr", (a, c8(7)), 8)), 8)
        elif mnemonic == "rrca":
            result = ir.op("or", (ir.op("shr", (a, c8(1)), 8), ir.op("shl", (a, c8(7)), 8)), 8)
        elif mnemonic == "rla":
            result = ir.op("or", (ir.op("shl", (a, c8(1)), 8), carry_in), 8)
        else:
            result = ir.op("or", (ir.op("shr", (a, c8(1)), 8), ir.op("shl", (carry_in, c8(7)), 8)), 8)
        lifter.emit(ir.SetReg(A, result))
        lifter.flag("cy", ir.bit(a, 7) if mnemonic in ("rlca", "rla") else ir.bit(a, 0))
        return True

    if mnemonic in SHIFTS:
        operand = operands[0]
        address = lifter.address(operand) if operand.mode != "reg" else None
        value = lifter.read8(operand.reg) if operand.mode == "reg" else lifter.load(address, 8)
        result = lifter.shifted(mnemonic, value)
        lifter.put8(operand, result, address)
        return True

    if mnemonic in ("bit", "set", "res"):
        number = operands[0].value
        operand = operands[1]
        address = lifter.address(operand) if operand.mode != "reg" else None
        value = lifter.read8(operand.reg) if operand.mode == "reg" else lifter.load(address, 8)
        if mnemonic == "bit":
            value = lifter.held(value, "tested")
            tested = ir.bit(value, number)
            lifter.flag("z", lnot(tested))
            lifter.flag("ov", lnot(tested))
            lifter.flag("s", tested if number == 7 else ir.const(0, 1))
            return True
        mask = 1 << number
        result = ir.op("or", (value, c8(mask)), 8) if mnemonic == "set" else ir.op("and", (value, c8(~mask & 0xFF)), 8)
        lifter.put8(operand, result, address)
        return True

    if mnemonic == "ex":
        first, second = operands
        if first.reg == "de" and second.reg == "hl":
            de = lifter.temp(reg(DE), "de")
            lifter.emit(ir.SetReg(DE, reg(HL)))
            lifter.emit(ir.SetReg(HL, de))
            return True
        if first.mode == "reg_indirect" and first.reg == "sp":
            top = lifter.load(reg(SP), 16, "top")
            lifter.store(reg(SP), lifter.read16(second.reg))
            lifter.write16(second.reg, top)
            return True
        if first.reg == "af":
            a = lifter.temp(reg(A), "a")
            lifter.emit(ir.SetReg(A, reg(ALTERNATE[A])))
            lifter.emit(ir.SetReg(ALTERNATE[A], a))
            for flag, other in ALTERNATE_FLAGS.items():
                saved = lifter.temp(ir.Flag(flag), flag)
                lifter.flag(flag, ir.Flag(other))
                lifter.flag(other, saved)
            return True
        raise Unsupported(f"{instruction.address:04X}: {instruction.text}")

    if mnemonic == "exx":
        for index in (BC, DE, HL):
            saved = lifter.temp(reg(index), NAMES[index])
            lifter.emit(ir.SetReg(index, reg(ALTERNATE[index])))
            lifter.emit(ir.SetReg(ALTERNATE[index], saved))
        return True

    if mnemonic == "push":
        name = operands[0].reg
        lifter.emit(ir.SetReg(SP, ir.op("sub", (reg(SP), c16(2)), 16)))
        if name == "af":
            # A above F, as two bytes: A is saved whole, so a POP AF that puts it back is seen as one
            lifter.store(ir.op("add", (reg(SP), c16(1)), 16), reg(A))
            lifter.store(reg(SP), flags_byte())
        else:
            lifter.store(reg(SP), lifter.read16(name))
        return True

    if mnemonic == "pop":
        name = operands[0].reg
        if name == "af":
            f = lifter.load(reg(SP), 8, "f")
            lifter.emit(ir.SetReg(A, lifter.load(ir.op("add", (reg(SP), c16(1)), 16), 8, "a")))
            lifter.flag("s", ir.bit(f, 7))
            lifter.flag("z", ir.bit(f, 6))
            lifter.flag("ov", ir.bit(f, 2))
            lifter.flag("cy", ir.bit(f, 0))
        else:
            lifter.write16(name, lifter.load(reg(SP), 16, name))
        lifter.emit(ir.SetReg(SP, ir.op("add", (reg(SP), c16(2)), 16)))
        return True

    if mnemonic in ("ldi", "ldd", "ldir", "lddr"):
        step = "add" if mnemonic in ("ldi", "ldir") else "sub"
        if mnemonic in ("ldir", "lddr"):
            # the whole move: BC bytes from (HL) to (DE), then BC is 0; its time depends on BC (block.c)
            lifter.emit(ir.Effect("move_block", (reg(HL), reg(DE), reg(BC), ir.const(1 if step == "add" else 0, 1))))
            count = reg(BC)
            lifter.emit(ir.SetReg(HL, ir.op(step, (reg(HL), count), 16)))
            lifter.emit(ir.SetReg(DE, ir.op(step, (reg(DE), count), 16)))
            lifter.emit(ir.SetReg(BC, c16(0)))
            lifter.flag("ov", ir.const(0, 1))
            return True
        value = lifter.load(reg(HL), 8, "moved")
        lifter.store(reg(DE), value)
        lifter.emit(ir.SetReg(HL, ir.op(step, (reg(HL), c16(1)), 16)))
        lifter.emit(ir.SetReg(DE, ir.op(step, (reg(DE), c16(1)), 16)))
        lifter.emit(ir.SetReg(BC, ir.op("sub", (reg(BC), c16(1)), 16)))
        lifter.flag("ov", ir.op("ne", (reg(BC), c16(0)), 1))
        return True

    if mnemonic == "in":
        destination, port = operands
        if port.mode != "port" or destination.reg != "a":
            raise Unsupported(f"{instruction.address:04X}: {instruction.text}")
        # the Z80 puts A on the upper address lines; the board decodes the low byte
        lifter.write8("a", lifter.temp(ir.op("helper:port_in", (c8(port.value),), 8), "port"))
        return True

    if mnemonic == "out":
        port, source = operands
        if port.mode != "port" or source.reg != "a":
            raise Unsupported(f"{instruction.address:04X}: {instruction.text}")
        lifter.emit(ir.Effect("port_out", (c8(port.value), reg(A))))
        return True

    return False


def flags_byte():
    """F as PUSH AF puts it on the stack: the kept flags where F has them."""
    f = c8(0)
    for flag, bit in (("s", 7), ("z", 6), ("ov", 2), ("cy", 0)):
        f = ir.op("or", (f, ir.op("shl", (ir.zext(ir.Flag(flag), 8), c8(bit)), 8)), 8)
    return f


def lift_add16(lifter, mnemonic, destination, source):
    """ADD HL/IX/IY,rr keeps S, Z and P/V; ADC and SBC HL,rr set them from the 16-bit result."""
    left = lifter.read16(destination)
    right = lifter.read16(source)
    if mnemonic != "add":
        left, right = lifter.held(left, "left"), lifter.held(right, "right")
    if mnemonic == "add":
        result = lifter.temp(ir.op("add", (left, right), 16), "sum")
        lifter.flag("cy", ir.op("carry_add", (left, right, ir.const(0, 1)), 1))
        lifter.write16(destination, result)
        return True
    carry = lifter.known_carry or ir.Flag("cy")
    if mnemonic == "adc":
        result = lifter.temp(ir.op("add", (ir.op("add", (left, right), 16), ir.zext(carry, 16)), 16), "sum")
        lifter.flag("ov", ir.op("overflow_add", (left, right, carry), 1))
        lifter.flag("cy", ir.op("carry_add", (left, right, carry), 1))
    else:
        result = lifter.temp(ir.op("sub", (ir.op("sub", (left, right), 16), ir.zext(carry, 16)), 16), "difference")
        lifter.flag("ov", ir.op("overflow_sub", (left, right, carry), 1))
        lifter.flag("cy", ir.op("carry_sub", (left, right, carry), 1))
    lifter.sign_zero(result)
    lifter.write16(destination, result)
    return True
