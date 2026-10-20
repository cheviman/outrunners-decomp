# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        emit_c.py
Description: writes IR expressions and statements as C
Version:     0.1.0
Created:     11-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

C carries every value of up to 32 bits as uint32_t (64-bit ones as uint64_t) and casts to the
operator's width where the width matters, so the C does exactly what the IR says on every compiler.
Registers and flags are printed through 'names': by default the global register file of the
generated program (R0..R28, AP, FP, SP, cpu.reg[n]) and the flags in cpu.z, cpu.s, cpu.ov, cpu.cy.
"""
from . import ir

UNSIGNED = {1: "uint32_t", 8: "uint8_t", 16: "uint16_t", 32: "uint32_t", 64: "uint64_t"}
SIGNED = {8: "int8_t", 16: "int16_t", 32: "int32_t", 64: "int64_t"}
CARRIER = {1: "uint32_t", 8: "uint32_t", 16: "uint32_t", 32: "uint32_t", 64: "uint64_t"}
REGISTER_NAMES = [f"R{index}" for index in range(29)] + ["AP", "FP", "SP"]


class Names:
    """How registers, flags and temporaries are spelled in C."""

    def register(self, index):
        return REGISTER_NAMES[index] if index < 32 else f"cpu.reg[{index}]"

    def flag(self, name):
        return f"cpu.{name}"

    def temp(self, temp):
        return temp.name


def literal(value, bits):
    if bits == 64:
        return f"0x{value:X}ull"
    return f"0x{value:X}u"


class Printer:
    def __init__(self, names=None):
        self.names = names or Names()

    # ---- expressions ----

    def expr(self, node):
        """C for 'node', a value of CARRIER[node.bits] whose bits above node.bits are clear."""
        if isinstance(node, ir.Const):
            return literal(node.value, node.bits)
        if isinstance(node, ir.Reg):
            return self.names.register(node.index)
        if isinstance(node, ir.Flag):
            return self.names.flag(node.name)
        if isinstance(node, ir.Temp):
            return self.names.temp(node)
        if isinstance(node, ir.Load):
            return f"mem_read{node.bits}({self.expr(node.address)})"
        if isinstance(node, ir.Op):
            return self.operator(node)
        raise TypeError(f"not an expression: {node!r}")

    def wide(self, node):
        """'node' as a value of at least 32 bits, safe for C arithmetic without promotion to int."""
        text = self.expr(node)
        if node.bits == 64:
            return text
        if isinstance(node, (ir.Const, ir.Reg, ir.Temp)):
            return text
        return f"(uint32_t)({text})"

    def signed(self, node, bits):
        """'node' read as a signed value of 'bits' bits, in a C signed type."""
        return f"({SIGNED[bits]})({UNSIGNED[bits]})({self.expr(node)})"

    def cut(self, text, bits):
        """Truncate C text to 'bits' and give it back in its carrier."""
        if bits in (32, 64):
            return f"({UNSIGNED[bits]})({text})"
        if bits == 1:
            return f"(({text}) & 1u)"
        return f"(uint32_t)({UNSIGNED[bits]})({text})"

    def operator(self, node):
        name = node.name
        args = node.args
        bits = node.bits
        if name in ("add", "sub", "mul", "and", "or", "xor"):
            symbol = {"add": "+", "sub": "-", "mul": "*", "and": "&", "or": "|", "xor": "^"}[name]
            left = self.wide(args[0]) if bits <= 32 else f"(uint64_t){self.expr(args[0])}"
            right = self.expr(args[1]) if bits <= 32 else f"(uint64_t){self.expr(args[1])}"
            return self.cut(f"{left} {symbol} {right}", bits)
        if name == "not":
            return self.cut(f"~{self.wide(args[0])}", bits)
        if name == "neg":
            return self.cut(f"0u - {self.wide(args[0])}", bits)
        if name in ("shl", "shr"):
            helper = {("shl", 64): "shl64", ("shr", 64): "shr64"}.get((name, bits), f"{name}32")
            return self.cut(f"{helper}({self.expr(args[0])}, (int)({self.expr(args[1])}))", bits)
        if name == "sar":
            return self.cut(f"(uint32_t)sar32({self.signed(args[0], bits)}, (int)({self.expr(args[1])}))", bits)
        if name in ("eq", "ne"):
            symbol = "==" if name == "eq" else "!="
            return f"(uint32_t)({self.expr(args[0])} {symbol} {self.expr(args[1])})"
        if name in ("lt", "le"):
            width = args[0].bits
            symbol = "<" if name == "lt" else "<="
            return f"(uint32_t)({self.signed(args[0], width)} {symbol} {self.signed(args[1], width)})"
        if name in ("ult", "ule"):
            symbol = "<" if name == "ult" else "<="
            return f"(uint32_t)({self.expr(args[0])} {symbol} {self.expr(args[1])})"
        if name == "zext":
            return self.expr(args[0]) if bits <= 32 else f"(uint64_t){self.expr(args[0])}"
        if name == "trunc":
            return self.cut(self.expr(args[0]), bits)
        if name == "sext":
            source_bits = args[1].value
            return self.cut(f"({UNSIGNED[64] if bits == 64 else 'uint32_t'})({SIGNED[64] if bits == 64 else 'int32_t'})"
                            f"{self.signed(args[0], source_bits)}", bits)
        if name == "select":
            return f"(({self.expr(args[0])}) ? {self.expr(args[1])} : {self.expr(args[2])})"
        if name in ("carry_add", "overflow_add", "carry_sub", "overflow_sub"):
            width = args[0].bits
            return f"v60_{name}{width}({self.expr(args[0])}, {self.expr(args[1])}, {self.expr(args[2])})"
        if name.startswith("helper:"):
            return self.helper(name[7:], args, bits)
        raise ValueError(f"unknown operator {name}")

    def helper(self, name, args, bits):
        values = [self.expr(arg) for arg in args]
        if name in ("div", "divu", "rem", "remu"):
            return f"v60_{name}_value{bits}({values[0]}, {values[1]})"
        if name == "div_ov":
            width = args[0].bits
            return f"v60_div_overflow{width}({values[0]}, {values[1]})"
        if name in ("shl", "sha", "rot"):
            return f"v60_{name}_value({values[0]}, {values[1]}, {bits})"
        if name in ("shl_cy", "shl_ov", "sha_cy", "sha_ov", "rot_cy", "rot_ov"):
            width = args[0].bits
            return f"v60_{name}({values[0]}, {values[1]}, {width})"
        if name == "rotc":
            return f"v60_rotc_value({values[0]}, {values[1]}, {values[2]}, {bits})"
        if name in ("rotc_cy", "rotc_ov"):
            width = args[0].bits
            return f"v60_{name}({values[0]}, {values[1]}, {values[2]}, {width})"
        if name == "movt_ov":
            return f"v60_movt_overflow({values[0]}, {args[0].bits}, {args[1].value})"
        if name == "setf":
            return f"v60_condition({values[0]}, {values[1]}, {values[2]}, {values[3]}, {values[4]})"
        if name == "byte_swap":
            return f"v60_byte_swap({values[0]})"
        if name == "read_psw":
            return "v60_read_psw()"
        raise ValueError(f"unknown helper {name}")

    # ---- statements ----

    def statement(self, node):
        if isinstance(node, ir.SetTemp):
            return f"const {CARRIER[node.temp.bits]} {self.names.temp(node.temp)} = {self.expr(node.value)};"
        if isinstance(node, ir.SetReg):
            return f"{self.names.register(node.index)} = {self.expr(node.value)};"
        if isinstance(node, ir.SetFlag):
            return f"{self.names.flag(node.name)} = (uint8_t)({self.expr(node.value)});"
        if isinstance(node, ir.Store):
            return f"mem_write{node.bits}({self.expr(node.address)}, ({UNSIGNED[node.bits]})({self.expr(node.value)}));"
        if isinstance(node, ir.Effect):
            return self.effect(node)
        raise TypeError(f"not a statement: {node!r}")

    def effect(self, node):
        values = [self.expr(arg) for arg in node.args]
        name = node.name
        if name == "update_psw":
            value, mask = values
            return f"v60_write_psw((v60_read_psw() & ~({mask} & 0xFFFFFFu)) | ({value} & 0xFFFFFFu & {mask}));"
        if name == "load_privileged":
            return f"v60_load_privileged({values[0]}, {values[1]});"
        if name in ("divx", "divux"):
            return f"v60_{name}({values[0]}, (int)(int32_t){values[1]}, {values[2]});"
        if name == "write_psw_low":
            return f"v60_write_psw((v60_read_psw() & 0xFFFF0000u) | {values[0]});"
        if name == "move_string":
            source, length1, dest, length2, size, fill, downward = values
            return (f"v60_move_string({source}, {length1}, {dest}, {length2}, (int){size}, "
                    f"{fill} != 0, {downward} != 0);")
        if name in ("move_bits_down", "move_bits_up"):
            return f"v60_{name}({', '.join(values)});"
        if name == "halt":
            return "/* nothing: MAME's core does not wait on halt */"
        raise ValueError(f"unknown effect {name}")
