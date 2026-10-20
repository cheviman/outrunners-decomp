"""
Name:        conditions.py
Description: turns branches on the condition flags into comparisons of the values that set them
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

"cmp.h #0, 8[R25]; ble" sets four flags from 8[R25] - 0 and branches on (S xor OV) or Z: that is
8[R25] <= 0, signed, at 16 bits. When every flag a branch reads was set in its own block by one
instruction (a compare or subtraction, or one that leaves OV clear: test, move, logic), and nothing in
between changed what that instruction read, the branch tests the comparison itself. The flags are then
dead, and cleanup.py takes them away. A branch on flags a call sets, or that come from another block,
keeps its flags.
"""
from v60 import ir

from . import cleanup

ZERO_1 = ir.Const(0, 1)
# a branch whose flags none of the comparisons explains reads them as they were set, when what set them
# reads only temporaries and constants (the Z80's bit shifted out into the carry); the V60's keep them
DIRECT_FLAGS = False

FLAGS_READ = {"v": {"ov"}, "nv": {"ov"}, "l": {"cy"}, "nl": {"cy"}, "e": {"z"}, "ne": {"z"},
              "nh": {"cy", "z"}, "h": {"cy", "z"}, "n": {"s"}, "p": {"s"},
              "lt": {"s", "ov"}, "ge": {"s", "ov"}, "le": {"s", "ov", "z"}, "gt": {"s", "ov", "z"}}
OPPOSITE = {"v": "nv", "l": "nl", "e": "ne", "n": "p", "nh": "h", "lt": "ge", "le": "gt"}


def negation(node):
    """x for "x == 0" on a 1-bit value, else None."""
    if isinstance(node, ir.Op) and node.name == "eq" and node.args[1] == ZERO_1 and node.args[0].bits == 1:
        return node.args[0]
    return None


def is_flag(node, name):
    return isinstance(node, ir.Flag) and node.name == name


def pair(node, name, first, second):
    """True for "first <name> second" on flags, in either order."""
    if not (isinstance(node, ir.Op) and node.name == name):
        return False
    a, b = node.args
    return (is_flag(a, first) and is_flag(b, second)) or (is_flag(a, second) and is_flag(b, first))


def is_signed_less(node):
    return pair(node, "xor", "s", "ov")


def is_signed_less_equal(node):
    if not (isinstance(node, ir.Op) and node.name == "or"):
        return False
    a, b = node.args
    return (is_signed_less(a) and is_flag(b, "z")) or (is_signed_less(b) and is_flag(a, "z"))


def condition_name(node):
    """Which of the V60's conditions a branch condition built by function.flag_condition is, or None."""
    for name, flag in (("v", "ov"), ("l", "cy"), ("e", "z"), ("n", "s")):
        if is_flag(node, flag):
            return name
    inner = negation(node)
    if inner is not None:
        return OPPOSITE.get(condition_name(inner))
    if pair(node, "or", "cy", "z"):
        return "nh"
    if is_signed_less(node):
        return "lt"
    if is_signed_less_equal(node):
        return "le"
    return None


def resolve(node, temps):
    while isinstance(node, ir.Temp) and node.name in temps:
        node = temps[node.name]
    return node


def subtraction(result, temps):
    """(a, b) when 'result' was computed as a - b (with no borrow), else None."""
    value = resolve(result, temps)
    if not (isinstance(value, ir.Op) and value.name == "sub"):
        return None
    a, b = value.args
    if isinstance(a, ir.Op) and a.name == "sub" and b == ir.Const(0, b.bits):
        return tuple(a.args)
    return a, b


def sign_of(node):
    """The value whose top bit a definition of S takes, or None."""
    if isinstance(node, ir.Op) and node.name == "trunc" and node.bits == 1:
        inner = node.args[0]
        if isinstance(inner, ir.Op) and inner.name == "shr" and isinstance(inner.args[1], ir.Const):
            value = inner.args[0]
            if inner.args[1].value == value.bits - 1:
                return value
    return None


def zero_of(node):
    """The value a definition of Z compares with 0, or None."""
    if isinstance(node, ir.Op) and node.name == "eq" and isinstance(node.args[1], ir.Const) \
            and node.args[1].value == 0:
        return node.args[0]
    return None


def source_of(defs, temps, used):
    """Where the flags in 'used' come from: (result Z and S describe, (a, b) when they come from a - b,
    OV known clear, CY known clear), or None when they do not come from one place."""
    result = None
    if used & {"z", "s"}:
        candidates = set()
        if "z" in used:
            candidates.add(zero_of(defs["z"]))
        if "s" in used:
            candidates.add(sign_of(defs["s"]))
        if None in candidates or len(candidates) != 1:
            return None
        result = candidates.pop()
    compared = subtraction(result, temps) if result is not None else None
    overflow_clear = carry_clear = False
    for flag, helper in (("ov", "overflow_sub"), ("cy", "carry_sub")):
        if flag not in used:
            continue
        definition = defs[flag]
        if definition == ZERO_1:
            if flag == "ov":
                overflow_clear = True
            else:
                carry_clear = True
            continue
        if isinstance(definition, ir.Op) and definition.name == helper and definition.args[2] == ZERO_1:
            operands = tuple(definition.args[:2])
            if result is None and compared is None:
                compared = operands
            elif compared is None or tuple(compared) != operands:
                return None
            continue
        return None
    return result, compared, overflow_clear, carry_clear


def comparison(name, source, checked=False):
    """The comparison condition 'name' tests, given where its flags come from; None if there is none.
    The values compared have to be temporaries or constants, which still hold at the branch, unless
    'checked' says what they read is known not to change before it."""
    result, compared, overflow_clear, carry_clear = source
    for value in ([result] if result is not None else []) + list(compared or ()):
        reads, memory = reads_of(value)
        if (reads or memory) and not checked:
            return None
    if compared is not None and not overflow_clear and not carry_clear:
        a, b = compared
        table = {"e": ("eq", a, b), "ne": ("ne", a, b), "lt": ("lt", a, b), "ge": ("le", b, a),
                 "le": ("le", a, b), "gt": ("lt", b, a), "l": ("ult", a, b), "nl": ("ule", b, a),
                 "nh": ("ule", a, b), "h": ("ult", b, a)}
        if name in table:
            operator, left, right = table[name]
            return ir.op(operator, (left, right), 1)
    if result is None:
        return None
    zero = ir.Const(0, result.bits)
    if name in ("e", "ne"):
        return ir.op("eq" if name == "e" else "ne", (result, zero), 1)
    if name == "n" or (name == "lt" and overflow_clear):
        return ir.op("lt", (result, zero), 1)
    if name == "p" or (name == "ge" and overflow_clear):
        return ir.op("le", (zero, result), 1)
    if name == "le" and overflow_clear:
        return ir.op("le", (result, zero), 1)
    if name == "gt" and overflow_clear:
        return ir.op("lt", (zero, result), 1)
    if name == "nh" and carry_clear:
        return ir.op("eq", (result, zero), 1)
    if name == "h" and carry_clear:
        return ir.op("ne", (result, zero), 1)
    return None


def reads_of(expression):
    """Registers, flags and stack slots an expression reads itself (not through temporaries, which are
    bound once), and whether it reads memory."""
    found = set()
    memory = False
    for node in ir.walk(expression):
        if isinstance(node, ir.Reg):
            found.add(("r", node.index))
        elif isinstance(node, ir.Flag):
            found.add(("f", node.name))
        elif isinstance(node, ir.Stack):
            found.add(("s", node.offset))
        elif isinstance(node, ir.Load):
            memory = True
    return found, memory


def written_by(statement, flow):
    """Registers, flags and stack slots a statement may write, and whether it may write memory."""
    if isinstance(statement, ir.SetReg):
        return {("r", statement.index)}, False
    if isinstance(statement, ir.SetFlag):
        return {("f", statement.name)}, False
    if isinstance(statement, ir.SetStack):
        return {("s", statement.offset)}, False
    if isinstance(statement, ir.Store):
        return set(), True
    if isinstance(statement, (ir.Call, ir.Effect)):
        return {(variable[0], variable[1]) for variable in flow.statement_writes(statement)}, True
    return set(), False


def definitions(block, flow, stop=None):
    """The flags set in the block that still hold, at its end (or before statement number 'stop'), the
    expression they were set to (nothing in between changed what it reads), and the block's
    temporaries."""
    defs = {}
    temps = {}
    for statement in block.statements[:stop]:
        if isinstance(statement, ir.SetTemp):
            temps[statement.temp.name] = statement.value
            continue
        written, memory = written_by(statement, flow)
        for flag in list(defs):
            reads, reads_memory = reads_of(defs[flag])
            if reads & written or (reads_memory and memory) or ("f", flag) in written:
                del defs[flag]
        if isinstance(statement, ir.SetFlag) and ("f", statement.name) not in reads_of(statement.value)[0]:
            defs[statement.name] = statement.value
    return defs, temps


def shifted_out(definition):
    """For the carry of a shift by a constant (SHL, SHA): (value, number of the bit it holds), or None."""
    if not (isinstance(definition, ir.Op) and definition.name in ("helper:shl_cy", "helper:sha_cy")):
        return None
    value, count = definition.args
    if not isinstance(count, ir.Const):
        return None
    shift = count.value & 0xFF
    if shift >= 0x80:
        shift -= 0x100
    bits = value.bits
    if 0 < -shift <= bits:
        return value, -shift - 1         # to the right: the last bit out is bit n - 1
    if 0 < shift <= bits:
        return value, bits - shift       # to the left: bit bits - n
    return None


def rewrite_condition(condition, defs, temps, checked=False):
    if isinstance(condition, ir.Op) and condition.name == "and" and condition.bits == 1:
        # DBcc: the counter is not zero, and the condition holds
        left, right = condition.args
        new = rewrite_condition(right, defs, temps, checked)
        return condition if new is right else ir.op("and", (left, new), 1)
    name = condition_name(condition)
    if name is None:
        return condition
    used = FLAGS_READ[name]
    if not used <= set(defs):
        return condition
    if name in ("l", "nl"):
        found = shifted_out(defs["cy"])
        if found is not None:
            value, number = found
            reads, memory = reads_of(value)
            if checked or not (reads or memory):
                bit = ir.op("and", (value, ir.Const(1 << number, value.bits)), value.bits)
                return ir.op("ne" if name == "l" else "eq", (bit, ir.Const(0, value.bits)), 1)
    source = source_of(defs, temps, used)
    new = comparison(name, source, checked) if source is not None else None
    if new is None and DIRECT_FLAGS:
        new = substituted(condition, defs, used, checked)
    return condition if new is None else new


def substituted(condition, defs, used, checked):
    """'condition' with each flag it reads replaced by what the flag was set to, or None when that reads
    something that may have changed since."""
    for flag in used:
        reads, memory = reads_of(defs[flag])
        if (reads or memory) and not checked:
            return None
    return ir.rebuild(condition, lambda node: defs[node.name] if isinstance(node, ir.Flag) else None)


def resolve_all(expression, values):
    """'expression' with its temporaries replaced by the values they were bound to; None if one of them
    no longer holds."""
    missing = []

    def change(node):
        if isinstance(node, ir.Temp):
            if node.name not in values:
                missing.append(node.name)
                return None
            return values[node.name]
        return None
    rebuilt = ir.rebuild(expression, change)
    return None if missing else rebuilt


def carried(block, flow):
    """The flags 'block' leaves set, as expressions over registers and constants (no temporaries), that
    hold at its end: what they read is not written after they were set. None stands for nothing."""
    values = {}
    flags = {}
    for statement in block.statements:
        if isinstance(statement, ir.SetTemp):
            resolved = resolve_all(statement.value, values)
            # a value read from memory can be read again later, as long as nothing writes memory in
            # between and reading it does nothing else (cleanup.has_effect: the board's I/O)
            if resolved is not None and not cleanup.has_effect(resolved):
                values[statement.temp.name] = resolved
            continue
        if isinstance(statement, ir.SetFlag):
            resolved = resolve_all(statement.value, values)
            written, _ = written_by(statement, flow)
            for name in list(flags):
                if ("f", name) in written or written & reads_of(flags[name])[0]:
                    del flags[name]
            if resolved is not None and ("f", statement.name) not in reads_of(resolved)[0]:
                flags[statement.name] = resolved
            continue
        written, writes_memory = written_by(statement, flow)
        for table in (values, flags):
            for name in list(table):
                reads, memory = reads_of(table[name])
                if reads & written or (memory and writes_memory) or (table is flags and ("f", name) in written):
                    del table[name]
    return flags


def through(flags, block, flow):
    """'flags' after the statements of 'block' (before its end)."""
    flags = dict(flags)
    for statement in block.statements:
        if isinstance(statement, ir.SetTemp):
            continue
        written, writes_memory = written_by(statement, flow)
        for name in list(flags):
            reads, memory = reads_of(flags[name])
            if ("f", name) in written or written & reads or (memory and writes_memory):
                del flags[name]
    return flags


SETF_CONDITIONS = ("v", "nv", "l", "nl", "e", "ne", "nh", "h", "n", "p", None, None, "lt", "ge", "le", "gt")


def flag_expression(name):
    """The branch condition function.flag_condition builds for 'name', from the flags."""
    z, s, ov, cy = (ir.Flag(flag) for flag in ("z", "s", "ov", "cy"))

    def lnot(value):
        return ir.op("eq", (value, ZERO_1), 1)
    table = {"v": ov, "nv": lnot(ov), "l": cy, "nl": lnot(cy), "e": z, "ne": lnot(z),
             "nh": ir.op("or", (cy, z), 1), "h": lnot(ir.op("or", (cy, z), 1)), "n": s, "p": lnot(s),
             "lt": ir.op("xor", (s, ov), 1), "ge": lnot(ir.op("xor", (s, ov), 1)),
             "le": ir.op("or", (ir.op("xor", (s, ov), 1), z), 1),
             "gt": lnot(ir.op("or", (ir.op("xor", (s, ov), 1), z), 1))}
    return table[name]


def recover_setf(block, flow):
    """SETF (a register set to 1 when a condition holds) reads the comparison too. Returns how many."""
    changed = 0
    for index, statement in enumerate(block.statements):
        found = [node for expression in ir.statement_expressions(statement) for node in ir.walk(expression)
                 if isinstance(node, ir.Op) and node.name == "helper:setf"]
        if not found:
            continue
        defs, temps = definitions(block, flow, index)

        def change(node, defs=defs, temps=temps):
            code = resolve(node.args[0], temps) if isinstance(node, ir.Op) and node.name == "helper:setf" else None
            if isinstance(code, ir.Const):
                name = SETF_CONDITIONS[code.value & 0xF]
                if name is None:
                    return ir.Const(1 if code.value & 0xF == 10 else 0, node.bits)
                condition = flag_expression(name)
                new = rewrite_condition(condition, defs, temps)
                if new is not condition:
                    return ir.zext(new, node.bits)
            return None
        before = repr(statement)
        ir.map_statement(statement, change)
        if repr(statement) != before:
            changed += 1
    return changed


def recover(function, flow):
    """Rewrites the branch conditions of 'function' in place; returns how many changed. A branch on
    flags its own block set reads the comparison from there; one on flags the block before it set
    (when that is the only way in, and nothing in between changes what the comparison reads) from
    that block."""
    changed = 0
    predecessors = {}
    for label, block in function.blocks.items():
        for successor in block.successors():
            predecessors.setdefault(successor, []).append(label)
    for label, block in function.blocks.items():
        changed += recover_setf(block, flow)
        end = block.end
        if not isinstance(end, ir.Branch):
            continue
        defs, temps = definitions(block, flow)
        new = rewrite_condition(end.condition, defs, temps)
        if new is end.condition:
            before = predecessors.get(label, [])
            if len(before) == 1 and before[0] != label and label != function.entry:
                flags = through(carried(function.blocks[before[0]], flow), block, flow)
                own = {flag for flag in FLAGS_READ.get(condition_name(end.condition) or "", ())}
                if flags and not any(flag in defs for flag in own):
                    new = rewrite_condition(end.condition, flags, {}, checked=True)
        if new is not end.condition:
            end.condition = new
            changed += 1
    return changed
