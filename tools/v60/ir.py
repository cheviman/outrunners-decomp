# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        ir.py
Description: intermediate representation of V60 code: expressions over registers, flags, memory and
             temporaries, and the statements that change them
Version:     0.1.0
Created:     11-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

Every value is an unsigned integer of a known width in bits (1 for flags and conditions, 8, 16, 32,
64); a value narrower than its C carrier never has bits set above its width. Operators are pure, so
an expression can be moved or shared as long as the statements it reads from stay in order; memory
reads are only ever bound to a temporary first, in the order MAME's core makes them.
"""
from dataclasses import dataclass, field

MASKS = {1: 0x1, 8: 0xFF, 16: 0xFFFF, 32: 0xFFFFFFFF, 64: 0xFFFFFFFFFFFFFFFF}
FLAGS = ("z", "s", "ov", "cy")


# ---- expressions ----

@dataclass(frozen=True)
class Const:
    value: int
    bits: int = 32

    def __post_init__(self):
        object.__setattr__(self, "value", self.value & MASKS[self.bits])


@dataclass(frozen=True)
class Reg:
    """A whole 32-bit register: 0-28, AP (29), FP (30), SP (31), or a privileged one (36 + n)."""
    index: int
    bits: int = 32


@dataclass(frozen=True)
class Flag:
    name: str
    bits: int = 1


@dataclass(frozen=True)
class EntryPoint:
    """In a function with more than one way in: which one this call took (0 the first)."""
    bits: int = 32


@dataclass(frozen=True)
class Temp:
    """A value bound by a SetTemp statement of the same instruction or function."""
    name: str
    bits: int


@dataclass(frozen=True)
class Control:
    """A variable only the C has, true or false, that steers its flow where the jumps of the code cross
    (decompiler/gotos.py): no register or flag of the CPU."""
    name: str
    bits: int = 1


@dataclass(frozen=True)
class Load:
    """Only ever the value of a SetTemp: memory is read where the statement stands."""
    address: object
    bits: int


@dataclass(frozen=True)
class Op:
    """A pure operator. 'bits' is the width of the result.

    add sub mul and or xor        result truncated to bits
    not neg                       one operand
    shl shr sar                   counts are masked like x86 does (5 bits, or 6 for 64-bit values)
    eq ne lt le ult ule           comparisons, 1 bit; lt and le are signed at the operands' width
    zext sext trunc               to 'bits'; sext takes the width to extend from as a Const second operand
    select                        (condition, if true, if false)
    carry_add overflow_add        flags of a + b + carry_in at the operands' width (operands: a, b, carry_in)
    carry_sub overflow_sub        flags of a - b - borrow_in, likewise
    helper:NAME                   one of the MAME-exact helpers of runtime/include/v60/ops.h (C emitter)
    """
    name: str
    args: tuple
    bits: int


# ---- statements ----

@dataclass
class SetTemp:
    temp: Temp
    value: object


@dataclass
class SetReg:
    index: int
    value: object


@dataclass
class SetFlag:
    name: str
    value: object


@dataclass
class Store:
    address: object
    value: object
    bits: int


@dataclass
class Effect:
    """A call with side effects the IR does not spell out (string and bit-string moves, PSW writes...)."""
    name: str
    args: tuple = field(default_factory=tuple)


@dataclass
class Call:
    """A call that comes back to the next statement: to the function at 'target', or to the code at a
    computed 'address', one of 'choices' when the analysis knows them (call and pointer tables), or one
    of the addresses the program stores in memory slot 'slot' (the displacement of "jsr [slot[Rn]]")."""
    target: int = None
    address: object = None
    choices: tuple = ()
    return_address: int = 0
    slot: int = None


# ---- how a block ends ----

@dataclass
class Goto:
    target: int


@dataclass
class Branch:
    condition: object
    target: int
    otherwise: int


@dataclass
class Switch:
    """A jump through a table: 'address' is computed, 'cases' maps each possible address to a block."""
    address: object
    cases: dict


@dataclass
class Return:
    pass


@dataclass
class InterruptReturn:
    """RETIS: back from an interrupt, with the PSW it saved; 'frame' bytes of stack are dropped."""
    frame: object


@dataclass
class TailCall:
    """Execution goes on in another function, which returns for this one; 'entry', when it is not the
    function's first instruction, is the way into it (decompile.py: functions with two ways in)."""
    target: int
    entry: int = None


@dataclass
class ComputedJump:
    """A jump to code whose address only the running program knows."""
    address: object


@dataclass(frozen=True)
class ReturnAddress:
    """The address the caller pushed: what the V60 finds at [SP] when a function starts."""
    bits: int = 32


@dataclass(frozen=True)
class Stack:
    """A stack slot: 'offset' bytes from where SP stood when the function started (0 holds the
    return address the caller pushed), read with the width it was written with."""
    offset: int
    bits: int = 32


@dataclass
class SetStack:
    offset: int
    value: object
    bits: int = 32


# ---- walking expressions ----

def children(node):
    if isinstance(node, Load):
        return (node.address,)
    if isinstance(node, Op):
        return node.args
    return ()


def walk(node):
    """'node' and every expression inside it."""
    stack = [node]
    while stack:
        current = stack.pop()
        yield current
        stack.extend(children(current))


def rebuild(node, change):
    """A copy of 'node' with 'change' applied bottom-up; 'change' returns a replacement or None."""
    if isinstance(node, Load):
        address = rebuild(node.address, change)
        node = node if address is node.address else Load(address, node.bits)
    elif isinstance(node, Op):
        args = tuple(rebuild(arg, change) for arg in node.args)
        if any(new is not old for new, old in zip(args, node.args)):
            node = Op(node.name, args, node.bits)
    replaced = change(node)
    return node if replaced is None else replaced


def statement_expressions(statement):
    """The expressions a statement reads."""
    if isinstance(statement, SetTemp):
        return [statement.value]
    if isinstance(statement, (SetReg, SetFlag, SetStack)):
        return [statement.value]
    if isinstance(statement, Store):
        return [statement.address, statement.value]
    if isinstance(statement, Effect):
        return list(statement.args)
    if isinstance(statement, Call):
        return [statement.address] if statement.address is not None else []
    if isinstance(statement, Branch):
        return [statement.condition]
    if isinstance(statement, Switch):
        return [statement.address]
    if isinstance(statement, ComputedJump):
        return [statement.address]
    if isinstance(statement, InterruptReturn):
        return [statement.frame]
    return []


def map_statement(statement, change):
    """Applies 'change' (see rebuild) to every expression a statement reads, in place."""
    def apply(value):
        return rebuild(value, change)
    if isinstance(statement, (SetTemp, SetReg, SetFlag, SetStack)):
        statement.value = apply(statement.value)
    elif isinstance(statement, Store):
        statement.address = apply(statement.address)
        statement.value = apply(statement.value)
    elif isinstance(statement, Effect):
        statement.args = tuple(apply(arg) for arg in statement.args)
    elif isinstance(statement, Call):
        if statement.address is not None:
            statement.address = apply(statement.address)
    elif isinstance(statement, Branch):
        statement.condition = apply(statement.condition)
    elif isinstance(statement, (Switch, ComputedJump)):
        statement.address = apply(statement.address)
    elif isinstance(statement, InterruptReturn):
        statement.frame = apply(statement.frame)


# ---- building helpers ----

def const(value, bits=32):
    return Const(value, bits)


def op(name, args, bits):
    return Op(name, tuple(args), bits)


def zext(value, bits):
    if value.bits == bits:
        return value
    if isinstance(value, Const):
        return Const(value.value, bits)
    return Op("zext", (value,), bits)


def trunc(value, bits):
    if value.bits == bits:
        return value
    if isinstance(value, Const):
        return Const(value.value, bits)
    return Op("trunc", (value,), bits)


def sext(value, bits):
    """Sign-extend 'value' (of its own width) to 'bits'."""
    if value.bits == bits:
        return value
    return Op("sext", (value, Const(value.bits, 8)), bits)


def bit(value, number):
    """Bit 'number' of value, as a 1-bit value."""
    return trunc(Op("shr", (value, Const(number, 8)), value.bits), 1) if number else trunc(value, 1)


def flags_of_result(result):
    """Z and S of a result, the way MAME's SetSZ macros compute them."""
    return [SetFlag("z", Op("eq", (result, Const(0, result.bits)), 1)),
            SetFlag("s", bit(result, result.bits - 1))]
