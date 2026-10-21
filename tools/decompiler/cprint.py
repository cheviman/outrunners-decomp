"""
Name:        cprint.py
Description: prints IR expressions as plain C, with casts and parentheses only where the value needs them
Version:     0.1.0
Created:     11-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

An IR value has a width; C has its integer types and promotions. Every printed value says whether it
equals the IR value exactly (no bits set above its width, never negative) and what C type it has: uint32_t,
uint64_t, or int (what uint8_t and uint16_t promote to). Most operators only need their operands right in
the low bits (addition, logic, a left shift: "modular"), so a value is truncated only where a comparison,
a right shift, a division or a function needs it exact. Assignments do the rest: every variable has the
type of its width. Arithmetic that could overflow an int is done in uint32_t.
"""
import re

from v60 import ir

from . import cpu

UNSIGNED = {8: "uint8_t", 16: "uint16_t", 32: "uint32_t", 64: "uint64_t"}
SIGNED = {8: "int8_t", 16: "int16_t", 32: "int32_t", 64: "int64_t"}
PORT_NAMES = {}    # I/O port -> its name, for a processor with ports (tools/decompile_sound.py)

# C precedence: higher binds tighter
PRIMARY, UNARY, MULTIPLY, ADD, SHIFT, RELATION, EQUALITY = 16, 15, 13, 12, 11, 10, 9
BIT_AND, BIT_XOR, BIT_OR, LOGICAL_AND, LOGICAL_OR, CONDITIONAL = 8, 7, 6, 5, 4, 3
BITWISE = {BIT_AND, BIT_XOR, BIT_OR}


PART = re.compile(r"A([ST])(8|16|32)\((.*?)(?:, (\d+))?\)$")
INSIDE = set()    # the AS/AT accesses (their C) that lie inside the field or variable they are written over


def part(bits, name, offset, whole):
    """C of an access of 'bits' at byte 'offset' of 'name', a field or variable of 'whole' bits that has no
    name for that part of it: AS8(x) for the first byte, AT8(x, 1) for the second. An access that reaches
    past the end of it (the 32 bits from a halfword on, two fields copied at once) is written the same way,
    but only the ones inside it read as a part of its value (part_read)."""
    text = f"AS{bits}({name})" if offset == 0 else f"AT{bits}({name}, {offset})"
    if offset * 8 + bits <= whole and bits < whole:
        INSIDE.add(text)
    return text


def part_read(text):
    """A read of a part of a wider field or variable that has no name of its own (AS8(x): its first byte,
    AT8(x, 1): its second) as C: (uint8_t)x, (uint8_t)(x >> 8u); None for any other text, and for an access
    that reaches past the end of the field or variable (it reads more than its value)."""
    match = PART.match(text)
    if match is None or text.count("(") != text.count(")") or text not in INSIDE:
        return None
    kind, bits, inner, offset = match.groups()
    # the macro's argument is everything up to its last ', N)' (or ')'): it may hold calls and commas
    inner = text[4 if bits == "8" else 5:-1]
    if kind == "T":
        inner, offset = inner.rsplit(", ", 1)
        return f"(uint{bits}_t)({inner} >> {int(offset) * 8}u)"
    return f"(uint{bits}_t){inner}"


class Value:
    """Printed C for an IR value. 'signed' is the width of a value the C already holds as a signed number
    of that width (a signed field, a sign extension), 0 otherwise: a signed comparison needs no cast."""
    __slots__ = ("text", "prec", "exact", "ctype", "signed")

    def __init__(self, text, prec, exact, ctype, signed=0):
        self.text = text
        self.prec = prec
        self.exact = exact
        self.ctype = ctype
        self.signed = signed


def inverted_mask(value, bits):
    """'~0x4000u' for an and mask that is shorter written as the bits it clears, or None."""
    mask = (1 << bits) - 1
    value &= mask
    cleared = ~value & ((1 << 64) - 1 if bits == 64 else 0xFFFFFFFF)
    if bits < 32:
        cleared &= mask
    if not cleared or cleared == mask:
        return None
    if len(f"{cleared:X}") >= len(f"{value:X}"):
        return None
    return f"~{literal(cleared, 64 if bits == 64 else 32)}"


def narrow_source(node):
    """How many bits a value really has: a zero extension of a halfword has 16 (a product of it and a
    small constant cannot overflow an int)."""
    while isinstance(node, ir.Op) and node.name in ("zext", "fits"):
        node = node.args[0]
    return node.bits


def literal(value, bits):
    if bits == 64:
        return f"0x{value:X}ull"
    return f"{value}u" if value < 10 else f"0x{value:X}u"


def signed_literal(value, bits):
    """A constant read as a signed number of 'bits' bits."""
    if value >= 1 << (bits - 1):
        value -= 1 << bits
    if bits == 64:
        return f"{value}ll"
    if value == -(1 << 31):
        return "INT32_MIN"
    return str(value) if -10 < value < 10 else (f"0x{value:X}" if value >= 0 else f"-0x{-value:X}")


class Printer:
    """Prints expressions through 'names': register(index), flag(name), temp(temp) -> (C name, bits),
    slot(node) -> (C name, bits)."""

    def __init__(self, names):
        self.names = names

    # ---- what callers ask for ----

    def any(self, node):
        """The value right in its low node.bits bits."""
        return self.emit(node)

    def exact(self, node):
        value = self.emit(node)
        if value.exact:
            return value
        bits = node.bits
        if bits == 1:
            return Value(f"{self.wrap(value, BIT_AND)} & 1u", BIT_AND, True, "u32" if value.ctype == "u32" else value.ctype)
        return Value(f"({UNSIGNED[bits]}){self.wrap(value, UNARY)}", UNARY, True,
                     "u64" if bits == 64 else ("u32" if bits == 32 else "int"))

    def unsigned(self, node):
        """Exact, in an unsigned C type at least 32 bits wide (safe to shift, multiply, divide)."""
        value = self.exact(node)
        if value.ctype in ("u32", "u64"):
            return value
        return Value(f"(uint32_t){self.wrap(value, UNARY)}", UNARY, True, "u32")

    def text(self, node, exact=False):
        return (self.exact(node) if exact else self.any(node)).text

    def condition(self, node):
        """A branch condition: the parentheses of "if" come from the caller."""
        return self.exact(node).text

    # ---- helpers ----

    @staticmethod
    def wrap(value, prec):
        """The value's text, in parentheses if it binds looser than 'prec' (or mixes with bitwise ops)."""
        if value.prec < prec or (prec in BITWISE and value.prec != prec and value.prec < UNARY):
            return f"({value.text})"
        return value.text

    @staticmethod
    def operand(value):
        """A shifted value: in parentheses when it is itself an operation (a + b << 1 reads wrong)."""
        return value.text if value.prec >= UNARY else f"({value.text})"

    @staticmethod
    def joined(left, right):
        if "u64" in (left.ctype, right.ctype):
            return "u64"
        if "u32" in (left.ctype, right.ctype):
            return "u32"
        return "int"

    # ---- the cases ----

    def emit(self, node):
        if isinstance(node, ir.Const):
            return Value(literal(node.value, node.bits), PRIMARY, True, "u64" if node.bits == 64 else "u32")
        if isinstance(node, ir.Reg):
            name, bits = self.names.register_node(node)
            return Value(name, PRIMARY, True, "u32" if bits == 32 else "int")
        if isinstance(node, ir.Flag):
            return Value(self.names.flag(node.name), PRIMARY, True, "u32")
        if isinstance(node, ir.Control):
            return Value(self.names.control(node.name), PRIMARY, True, "int")
        if isinstance(node, ir.Temp):
            name, bits = self.names.temp(node)
            return Value(name, PRIMARY, True, "u64" if bits == 64 else ("u32" if bits == 32 else "int"))
        if isinstance(node, ir.Stack):
            name, bits = self.names.slot(node)
            return Value(name, PRIMARY, True, "u32" if bits == 32 else "int")
        if isinstance(node, ir.Load):
            ctype = "u64" if node.bits == 64 else ("u32" if node.bits == 32 else "int")
            named = self.names.memory(node)
            if named is not None:
                text, signed = named
                part = part_read(text)
                if part is not None:
                    return Value(part, UNARY, True, ctype)
                if signed:
                    return Value(text, PRIMARY, False, "int", node.bits)
                return Value(text, PRIMARY, True, ctype)
            register = self.names.board_register(node.address.value & 0xFFFFFF) \
                if isinstance(node.address, ir.Const) else None
            address = register or self.any(node.address).text
            return Value(f"mem_read{node.bits}({address})", PRIMARY, True, ctype)
        if isinstance(node, ir.Op):
            return self.operator(node)
        raise TypeError(f"not an expression: {node!r}")

    def operator(self, node):
        name, args, bits = node.name, node.args, node.bits
        if name == "trunc":
            # the low bits are the value; exact() cuts it where that matters
            value = self.any(args[0])
            return Value(value.text, value.prec, False, value.ctype)
        if name == "fits":
            # known to be within 'bits' already (knownbits.py): nothing to cut
            value = self.exact(args[0])
            return Value(value.text, value.prec, True, value.ctype)
        if name == "zext":
            value = self.exact(args[0])
            if bits == 64 and value.ctype != "u64":
                return Value(f"(uint64_t){self.wrap(value, UNARY)}", UNARY, True, "u64")
            return Value(value.text, value.prec, True, value.ctype)
        if name == "sext":
            source = args[1].value
            value = self.any(args[0])
            text = value.text if value.signed == source else f"({SIGNED[source]}){self.wrap(value, UNARY)}"
            prec = value.prec if value.signed == source else UNARY
            if bits == 64:
                return Value(f"(uint64_t)(int64_t){text}", UNARY, True, "u64")
            return Value(text, prec, False, "int", source)
        if name in ("add", "sub", "mul"):
            return self.arithmetic(node)
        if name in ("and", "or", "xor"):
            return self.bitwise(node)
        if name == "not":
            value = self.any(args[0])
            if value.ctype == "int" and bits >= 32:
                value = Value(f"(uint32_t){self.wrap(value, UNARY)}", UNARY, value.exact, "u32")
            ctype = value.ctype
            return Value(f"~{self.wrap(value, UNARY)}", UNARY, bits in (32, 64) and ctype in ("u32", "u64"), ctype)
        if name == "neg":
            # 0u - x rather than -x: the same in unsigned arithmetic, and no compiler warns about it
            value = self.any(args[0])
            zero = "0ull" if bits == 64 else "0u"
            return Value(f"{zero} - {self.wrap(value, ADD + 1)}", ADD, bits in (32, 64),
                         "u64" if bits == 64 or value.ctype == "u64" else "u32")
        if name in ("shl", "shr", "sar"):
            return self.shift(node)
        if name in ("eq", "ne"):
            if isinstance(args[0], ir.Control) and args[1] == ir.Const(0, 1):
                # a control variable is true or false: "!found", "found"
                value = self.emit(args[0])
                return Value(f"!{value.text}", UNARY, True, "int") if name == "eq" else value
            left, right = self.exact(args[0]), self.exact(args[1])
            symbol = "==" if name == "eq" else "!="
            return Value(f"{self.wrap(left, RELATION)} {symbol} {self.wrap(right, RELATION)}", EQUALITY, True, "int")
        if name in ("lt", "le"):
            return self.signed_compare(node)
        if name in ("ult", "ule"):
            left_node, right_node = args
            ranged = self.range_check(name, left_node, right_node)
            if ranged is not None:
                return ranged
            # unsigned: 1 <= x is x != 0, and x < 1 is x == 0
            if name == "ule" and left_node == ir.Const(1, left_node.bits) and not isinstance(right_node, ir.Const):
                return Value(f"{self.wrap(self.exact(right_node), RELATION)} != 0u", EQUALITY, True, "int")
            if name == "ult" and right_node == ir.Const(1, right_node.bits) and not isinstance(left_node, ir.Const):
                return Value(f"{self.wrap(self.exact(left_node), RELATION)} == 0u", EQUALITY, True, "int")
            left, right = self.exact(left_node), self.exact(right_node)
            if isinstance(left_node, ir.Const) and not isinstance(right_node, ir.Const):
                # constant first reads worse: 3 < x is x > 3
                symbol = ">" if name == "ult" else ">="
                return Value(f"{self.wrap(right, SHIFT)} {symbol} {self.wrap(left, SHIFT)}", RELATION, True, "int")
            symbol = "<" if name == "ult" else "<="
            return Value(f"{self.wrap(left, SHIFT)} {symbol} {self.wrap(right, SHIFT)}", RELATION, True, "int")
        if name in ("lor", "land"):
            # || and &&: the second test only runs when the first does not decide, as the jumps did
            symbol, prec = ("||", LOGICAL_OR) if name == "lor" else ("&&", LOGICAL_AND)
            parts = []
            for arg in args:
                value = self.exact(arg)
                mixed = value.prec in (LOGICAL_AND, LOGICAL_OR) and value.prec != prec
                parts.append(f"({value.text})" if mixed or value.prec < prec else value.text)
            return Value(f" {symbol} ".join(parts), prec, True, "int")
        if name == "select":
            condition, yes, no = self.exact(args[0]), self.exact(args[1]), self.exact(args[2])
            return Value(f"{self.wrap(condition, CONDITIONAL + 1)} ? {self.wrap(yes, CONDITIONAL + 1)} : "
                         f"{self.wrap(no, CONDITIONAL)}", CONDITIONAL, True, self.joined(yes, no))
        if name in ("udiv", "urem"):
            # by a constant that is not 0 (expressions.py)
            left = self.exact(args[0])
            symbol = "/" if name == "udiv" else "%"
            return Value(f"{self.wrap(left, MULTIPLY)} {symbol} {literal(args[1].value, args[1].bits)}",
                         MULTIPLY, True, left.ctype)
        if name in ("sdiv", "srem"):
            width = args[0].bits
            left = f"({SIGNED[width]}){self.wrap(self.any(args[0]), UNARY)}"
            symbol = "/" if name == "sdiv" else "%"
            return Value(f"{left} {symbol} {signed_literal(args[1].value, width)}", MULTIPLY, False,
                         "u64" if width == 64 else "int")
        if name in ("carry_add", "overflow_add", "carry_sub", "overflow_sub"):
            width = args[0].bits
            values = ", ".join(self.exact(arg).text for arg in args)
            return Value(f"{cpu.current.name}_{name}{width}({values})", PRIMARY, True, "u32")
        if name == "parity":
            # 1 when the byte has an even number of bits set (the Z80's P/V after logic)
            return Value(f"parity8({self.exact(args[0]).text})", PRIMARY, True, "u32")
        if name.startswith("helper:"):
            return self.helper(name[7:], args, bits)
        raise ValueError(f"unknown operator {name}")

    def arithmetic(self, node):
        name, (left_node, right_node), bits = node.name, node.args, node.bits
        left, right = self.any(left_node), self.any(right_node)
        ctype = self.joined(left, right)
        small = name == "mul" and isinstance(right_node, ir.Const) and right_node.value < 0x8000 \
            and narrow_source(left_node) <= 16
        if ctype == "int" and (bits >= 32 or name == "mul") and not small:
            # in uint32_t: a product, or anything at 32 bits, could overflow an int
            left = Value(f"(uint32_t){self.wrap(left, UNARY)}", UNARY, left.exact, "u32")
            ctype = "u32"
        symbol, prec = {"add": ("+", ADD), "sub": ("-", ADD), "mul": ("*", MULTIPLY)}[name]
        if name in ("add", "sub") and isinstance(right_node, ir.Const) and bits <= 64:
            # adding 0xFFFFFFFA is taking 6 away: the same in unsigned arithmetic, and it reads as it means
            value = right_node.value & ((1 << bits) - 1)
            if value >> (bits - 1) and (1 << bits) - value < 0x10000:
                symbol = "-" if name == "add" else "+"
                right = Value(literal((1 << bits) - value, bits), PRIMARY, True, right.ctype)
        right_text = self.wrap(right, prec + 1)    # a - (b - c)
        text = f"{self.wrap(left, prec)} {symbol} {right_text}"
        exact = (bits == 32 and ctype == "u32") or (bits == 64 and ctype == "u64")
        return Value(text, prec, exact, ctype)

    def bitwise(self, node):
        name, (left_node, right_node), bits = node.name, node.args, node.bits
        if name == "or" and bits == 32 and isinstance(left_node, ir.Op) and left_node.name == "and" \
                and left_node.args[1] == ir.Const(0xFFFF0000, 32):
            link = right_node
            while isinstance(link, ir.Op) and link.name in ("zext", "fits"):
                link = link.args[0]
            if isinstance(link, ir.Load) and link.bits == 16 and self.names.link(link):
                # an object reached through a link field: LINKED(base, link) in game/support.h
                base = self.any(left_node.args[0])
                return Value(f"LINKED({base.text}, {self.any(link).text})", PRIMARY, True, "u32")
        if name == "or" and bits == 32 and ir.Const(0x200000, 32) in (left_node, right_node):
            link = right_node if left_node == ir.Const(0x200000, 32) else left_node
            while isinstance(link, ir.Op) and link.name in ("zext", "fits"):
                link = link.args[0]
            if isinstance(link, ir.Load) and link.bits == 16 and self.names.link(link):
                # an object reached through a link field from no object: in the work RAM's page
                return Value(f"LINKED(WORK_RAM, {self.any(link).text})", PRIMARY, True, "u32")
        left, right = self.any(left_node), self.any(right_node)
        symbol, prec = {"and": ("&", BIT_AND), "or": ("|", BIT_OR), "xor": ("^", BIT_XOR)}[name]
        if name == "and" and isinstance(right_node, ir.Const) and (bits in (32, 64) or left.exact):
            # a mask that clears a few bits reads as those bits: x & ~0x4000u
            cleared = inverted_mask(right_node.value, bits)
            if cleared is not None:
                right = Value(cleared, UNARY, True, "u64" if bits == 64 else "u32")
        ctype = self.joined(left, right)
        if name == "and":
            exact = left.exact or right.exact or (bits == 32 and ctype == "u32")
        else:
            exact = (left.exact and right.exact) or (bits == 32 and ctype == "u32")
        return Value(f"{self.wrap(left, prec)} {symbol} {self.wrap(right, prec)}", prec, exact, ctype)

    def shift(self, node):
        name, (value_node, count_node), bits = node.name, node.args, node.bits
        constant = count_node.value if isinstance(count_node, ir.Const) else None
        limit = 64 if bits == 64 else 32
        if name == "shl":
            value = self.any(value_node)
            if value.ctype == "int":
                value = Value(f"(uint32_t){self.wrap(value, UNARY)}", UNARY, value.exact, "u32")
            if constant is not None and constant < limit:
                return Value(f"{self.operand(value)} << {constant}", SHIFT, bits == limit, value.ctype)
            helper = "shl64" if bits == 64 else "shl32"
            count = self.exact(count_node).text
            return Value(f"{helper}({value.text}, (int)({count}))", PRIMARY, bits == limit, value.ctype)
        if name == "shr":
            value = self.exact(value_node)
            if constant is not None and constant < limit:
                return Value(f"{self.operand(value)} >> {constant}", SHIFT, True, value.ctype)
            helper = "shr64" if bits == 64 else "shr32"
            value = self.unsigned(value_node)
            count = self.exact(count_node).text
            return Value(f"{helper}({value.text}, (int)({count}))", PRIMARY, True, value.ctype)
        # sar: the value read as signed at its width
        value = self.any(value_node)
        signed = f"({SIGNED[bits]}){self.wrap(value, UNARY)}"
        if constant is not None and constant < bits:
            return Value(f"{signed} >> {constant}", SHIFT, False, "u64" if bits == 64 else "int")
        count = self.exact(count_node).text
        return Value(f"sar32({signed}, (int)({count}))", PRIMARY, False, "int")

    def range_check(self, name, left_node, right_node):
        """x - a < b in unsigned arithmetic is the range test a <= x < a + b (and x - a >= b the test
        that x is outside it), when x is a variable or a field and the range is below the sign bit of
        its width, where the signed and unsigned readings of x agree. None when it is not that."""
        inside = isinstance(right_node, ir.Const)
        difference, limit = (left_node, right_node) if inside else (right_node, left_node)
        if not isinstance(limit, ir.Const) or not isinstance(difference, ir.Op) or difference.name != "sub":
            return None
        value, start = difference.args
        if not isinstance(start, ir.Const) or isinstance(value, ir.Const):
            return None
        bits = difference.bits
        simple = value
        while isinstance(simple, ir.Op) and simple.name in ("zext", "sext", "trunc", "fits"):
            simple = simple.args[0]
        if not isinstance(simple, (ir.Reg, ir.Load, ir.Temp)) or bits > 32:
            return None
        low = start.value & ((1 << bits) - 1)
        count = limit.value & ((1 << bits) - 1)
        if inside:
            count += name == "ule"      # x - a <= b: b + 1 values
        else:
            count += name == "ult"      # b < x - a: outside the b + 1 values from a
        if low == 0 or count == 0 or low + count > 1 << (bits - 1):
            return None
        # a variable or field of that width as C has it: signed or not, the range (all of it below the
        # sign bit) tells the same
        natural = isinstance(value, (ir.Load, ir.Temp)) and value.bits == bits
        printed = self.any(value) if natural else self.exact(value)
        text = self.wrap(printed, SHIFT)
        if printed.signed or printed.ctype == "int":
            first, last = signed_literal(low, bits), signed_literal(low + count - 1, bits)
        else:
            first, last = literal(low, bits), literal(low + count - 1, bits)
        if inside:
            return Value(f"{text} >= {first} && {text} <= {last}", LOGICAL_AND, True, "int")
        return Value(f"{text} < {first} || {text} > {last}", LOGICAL_OR, True, "int")

    def constant_name(self, node):
        """The name a constant compared as a signed number has (a table's address), or None."""
        return None

    def signed_compare(self, node):
        name, (left_node, right_node) = node.name, node.args
        width = left_node.bits
        symbol = "<" if name == "lt" else "<="

        def side(operand):
            if isinstance(operand, ir.Const):
                named = self.constant_name(operand)
                if named is not None:
                    return f"({SIGNED[width]}){named}"    # (an address compared as a signed number)
                return signed_literal(operand.value, width)
            value = self.any(operand)
            if value.signed == width:
                return self.wrap(value, SHIFT)    # already that signed type
            return f"({SIGNED[width]}){self.wrap(value, UNARY)}"
        left, right = side(left_node), side(right_node)
        if isinstance(left_node, ir.Const) and not isinstance(right_node, ir.Const):
            # constant first reads worse: 0 <= x is x >= 0
            flipped = ">" if name == "lt" else ">="
            return Value(f"{right} {flipped} {left}", RELATION, True, "int")
        return Value(f"{left} {symbol} {right}", RELATION, True, "int")

    def helper(self, name, args, bits):
        values = [self.exact(arg).text for arg in args]
        if name in ("div", "divu", "rem", "remu"):
            word = {"div": "divide_s", "divu": "divide_u", "rem": "remainder_s", "remu": "remainder_u"}[name]
            return Value(f"{word}{bits}({values[0]}, {values[1]})", PRIMARY, True, "u32")
        if name == "div_ov":
            return Value(f"v60_div_overflow{args[0].bits}({values[0]}, {values[1]})", PRIMARY, True, "u32")
        if name in ("shl", "sha", "rot"):
            word = {"shl": "shift_by", "sha": "shift_signed_by", "rot": "rotate_by"}[name]
            return Value(f"{word}({values[0]}, {values[1]}, {bits})", PRIMARY, True, "u32")
        if name in ("shl_cy", "shl_ov", "sha_cy", "sha_ov", "rot_cy", "rot_ov"):
            return Value(f"v60_{name}({values[0]}, {values[1]}, {args[0].bits})", PRIMARY, True, "u32")
        if name == "rotc":
            return Value(f"rotate_with_carry_by({values[0]}, {values[1]}, {values[2]}, {bits})", PRIMARY, True, "u32")
        if name in ("rotc_cy", "rotc_ov"):
            return Value(f"v60_{name}({values[0]}, {values[1]}, {values[2]}, {args[0].bits})", PRIMARY, True, "u32")
        if name == "movt_ov":
            return Value(f"v60_movt_overflow({values[0]}, {args[0].bits}, {args[1].value})", PRIMARY, True, "u32")
        if name == "setf":
            return Value(f"v60_condition({', '.join(values)})", PRIMARY, True, "u32")
        if name == "byte_swap":
            return Value(f"byte_swap32({values[0]})", PRIMARY, True, "u32")
        if name == "read_psw":
            return Value(f"psw_read({', '.join(values)})", PRIMARY, True, "u32")
        if name == "port_in":
            port = PORT_NAMES.get(args[0].value) if isinstance(args[0], ir.Const) else None
            return Value(f"port_in({port or values[0]})", PRIMARY, True, "int")
        if name == "interrupts_enabled":
            return Value("interrupts_enabled()", PRIMARY, True, "int")
        raise ValueError(f"unknown helper {name}")
