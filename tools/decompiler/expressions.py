"""
Name:        expressions.py
Description: makes the statements of a function read as expressions: temporaries go into the place that
             uses them, constants fold, and register writes that keep bits nobody reads write the value
Version:     0.1.0
Created:     11-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

The lifter binds every value an instruction reads or computes to a temporary, in the order MAME's core
does things. Most are used once, right after. A temporary goes into its one use when nothing between the
two changes what it reads: no write to a register, flag or stack slot it reads, and no memory write, call
or effect if it reads memory. A read of the board's I/O stays a statement of its own, in its place.
Cheap values (a constant, a register, a stack slot, a part of one) can be copied into several uses.

"mov.b X, R2" keeps bits 8-31 of R2: R2 = (R2 & 0xFFFFFF00) | X. When nothing reads those bits before
they are written again, it is just R2 = X.
"""
from v60 import ir

from . import cpu

from . import cleanup
from . import dataflow


# ---- simplifying one expression ----

def mask_of(bits):
    return ir.MASKS[bits]


def simplify(node):
    """'node' with constants folded and operations that do nothing taken out (bottom-up)."""
    return ir.rebuild(node, simplify_node)


def simplify_node(node):
    """simplify_one until it has nothing more to do; None if it had nothing to do at all."""
    result = None
    while True:
        new = simplify_one(node)
        if new is None:
            return result
        result = node = new


def narrowed_operand(node, bits):
    """'node' at 'bits' when it is a value of that width sign- or zero-extended (or a constant), else None."""
    if isinstance(node, ir.Const):
        return ir.Const(node.value & mask_of(bits), bits)
    if isinstance(node, ir.Op) and node.name in ("sext", "zext") and node.args[0].bits == bits:
        return node.args[0]
    return None


def simplify_one(node):
    if not isinstance(node, ir.Op):
        return None
    name, args, bits = node.name, node.args, node.bits
    constants = [arg.value if isinstance(arg, ir.Const) else None for arg in args]
    if name in ("add", "sub", "and", "or", "xor") and all(value is not None for value in constants):
        a, b = constants
        value = {"add": a + b, "sub": a - b, "and": a & b, "or": a | b, "xor": a ^ b}[name]
        return ir.Const(value & mask_of(bits), bits)
    if name in ("zext", "trunc") and constants[0] is not None:
        return ir.Const(constants[0] & mask_of(bits), bits)
    if name in ("shl", "shr", "sar", "mul") and constants[0] is not None and constants[1] is not None \
            and bits <= 64:
        value, count = constants
        mask = mask_of(bits)
        if name == "mul":
            return ir.Const((value * count) & mask, bits)
        if count < bits:
            if name == "shl":
                return ir.Const((value << count) & mask, bits)
            if name == "shr":
                return ir.Const((value & mask) >> count, bits)
            signed = value - (1 << bits) if (value >> (bits - 1)) & 1 else value
            return ir.Const((signed >> count) & mask, bits)
    if name in ("neg", "not") and constants[0] is not None:
        value = -constants[0] if name == "neg" else ~constants[0]
        return ir.Const(value & mask_of(bits), bits)
    if name in ("add", "sub", "or", "xor") and constants[1] == 0 and args[0].bits == bits:
        return args[0]
    if name in ("add", "or", "xor") and constants[0] == 0 and args[1].bits == bits:
        return args[1]
    if name == "and" and constants[1] is not None and constants[1] == mask_of(bits) and args[0].bits == bits:
        return args[0]
    if name == "and" and constants[1] == 0:
        return ir.Const(0, bits)
    # an index scaled the way the V60 code does it: x + x is x * 2, x + x * 2 is x * 3
    if name == "add" and args[0] == args[1] and args[0].bits == bits:
        return ir.op("mul", (args[0], ir.Const(2, bits)), bits)
    if name == "add" and isinstance(args[1], ir.Op) and args[1].name == "mul" and args[1].args[0] == args[0] \
            and isinstance(args[1].args[1], ir.Const) and args[1].bits == bits and args[0].bits == bits:
        return ir.op("mul", (args[0], ir.Const((args[1].args[1].value + 1) & mask_of(bits), bits)), bits)
    if name == "add" and constants[1] is not None:
        inner = args[0]
        if isinstance(inner, ir.Op) and inner.name == "add" and inner.bits == bits \
                and isinstance(inner.args[1], ir.Const):
            return ir.op("add", (inner.args[0], ir.Const((inner.args[1].value + constants[1]) & mask_of(bits), bits)),
                         bits)
        if isinstance(inner, ir.Op) and inner.name == "add" and inner.bits == bits \
                and isinstance(inner.args[0], ir.Const):
            return ir.op("add", (ir.Const((inner.args[0].value + constants[1]) & mask_of(bits), bits), inner.args[1]),
                         bits)
    if name == "trunc":
        inner = args[0]
        if isinstance(inner, ir.Op) and inner.name == "mul" and inner.bits == 2 * bits:
            # the low half of a widened product is the product at the narrow width, signed or not
            narrowed = [narrowed_operand(arg, bits) for arg in inner.args]
            if None not in narrowed:
                return ir.op("mul", tuple(narrowed), bits)
        if isinstance(inner, ir.Op) and inner.name == "trunc":
            return ir.trunc(inner.args[0], bits)
        if isinstance(inner, ir.Op) and inner.name == "zext" and inner.args[0].bits >= bits:
            return ir.trunc(inner.args[0], bits)
        if isinstance(inner, ir.Op) and inner.name == "zext" and inner.args[0].bits < bits:
            return ir.zext(inner.args[0], bits)
    if name == "zext":
        inner = args[0]
        if isinstance(inner, ir.Op) and inner.name == "zext":
            return ir.zext(inner.args[0], bits)
    if name in ("helper:shl", "helper:sha") and constants[1] is not None and args[0].bits == bits:
        # SHL / SHA by a constant: the count is a signed byte, negative to the right
        count = signed_count(constants[1])
        if count == 0:
            return args[0]
        if 0 < count < bits:
            return ir.op("shl", (args[0], ir.Const(count, 8)), bits)
        if -bits < count < 0:
            return ir.op("shr" if name == "helper:shl" else "sar", (args[0], ir.Const(-count, 8)), bits)
    if name in ("helper:shl_cy", "helper:sha_cy") and constants[1] is not None:
        # the carry of a shift by a constant is the last bit shifted out
        width = args[0].bits
        count = signed_count(constants[1])
        if count == 0:
            return ir.Const(0, bits)
        if 0 < count <= width:
            return bit_of(args[0], width - count)
        if -width <= count < 0:
            return bit_of(args[0], -count - 1)
    if name in ("helper:shl_ov", "helper:rot_ov"):
        return ir.Const(0, bits)
    if name == "helper:rot" and constants[1] is not None and args[0].bits == bits:
        # a rotation by a constant, as C writes one: (x << n) | (x >> (width - n))
        count = signed_count(constants[1]) % bits
        if count == 0:
            return args[0]
        return ir.op("or", (ir.op("shl", (args[0], ir.Const(count, 8)), bits),
                            ir.op("shr", (args[0], ir.Const(bits - count, 8)), bits)), bits)
    if name in ("helper:divu", "helper:remu") and constants[1]:
        # by a constant that is not 0: C's division (the V60 leaves the value as it was when dividing by 0)
        return ir.op("udiv" if name == "helper:divu" else "urem", args, bits)
    if name in ("helper:div", "helper:rem") and constants[1] and constants[1] != mask_of(args[1].bits):
        # by a constant that is neither 0 nor -1 (MIN / -1 overflows): C's signed division, which truncates
        return ir.op("sdiv" if name == "helper:div" else "srem", args, bits)
    if name in ("carry_sub", "carry_add", "overflow_sub", "overflow_add") and constants[2] == 0:
        width = args[0].bits
        if name == "carry_sub":
            return ir.op("ult", (args[0], args[1]), 1)    # the borrow
        if name == "carry_add" and args[0] == args[1]:
            return ir.op("lt", (args[0], ir.Const(0, width)), 1)    # x + x carries the top bit out
        if name == "carry_add":
            return ir.op("ult", (ir.op("add", (args[0], args[1]), width), args[0]), 1)
        if name == "overflow_sub" and constants[1] == 1:
            return ir.op("eq", (args[0], ir.Const(1 << (width - 1), width)), 1)
        if name == "overflow_add" and constants[1] == 1:
            return ir.op("eq", (args[0], ir.Const(mask_of(width) >> 1, width)), 1)
        if name in ("overflow_sub", "overflow_add") and width in (8, 16):
            # the difference (or sum) of the two as signed numbers is not what fits in the width
            operation = "sub" if name == "overflow_sub" else "add"
            exact = ir.op(operation, (ir.sext(args[0], 32), ir.sext(args[1], 32)), 32)
            kept = ir.sext(ir.op(operation, (args[0], args[1]), width), 32)
            return ir.op("ne", (exact, kept), 1)
    if name == "or":
        # (x & 0xFFFFFF00) | (uint8_t)x is x
        for keep, low in ((args[0], args[1]), (args[1], args[0])):
            if isinstance(keep, ir.Op) and keep.name == "and" and isinstance(keep.args[1], ir.Const) \
                    and keep.args[0].bits == bits:
                kept = low_part(low)
                if kept is not None and kept[0] == keep.args[0] \
                        and keep.args[1].value == mask_of(bits) & ~mask_of(kept[1]):
                    return keep.args[0]
    return None


def signed_count(value):
    """A shift count as the V60 reads it: its low byte, signed."""
    count = value & 0xFF
    return count - 0x100 if count >= 0x80 else count


def bit_of(value, position):
    return ir.trunc(ir.op("shr", (value, ir.Const(position, 8)), value.bits), 1) if position else ir.trunc(value, 1)


def low_part(node):
    """(x, n) when 'node' is the low n bits of x, zero-extended; None otherwise."""
    if isinstance(node, ir.Op) and node.name == "zext":
        inner = node.args[0]
        if isinstance(inner, ir.Op) and inner.name in ("trunc", "fits"):
            return inner.args[0], inner.bits
    return None


# ---- partial register writes ----

def narrow_partial_writes(function, after, whole=()):
    """R = (R & keep) | rest becomes R = rest where the kept bits are dead; 'after' is what cleanup.py
    found live after each statement. The registers in 'whole' are global variables in C: what they hold
    stays as the original leaves it, a pointer with its page (current_object keeps its 0x20 when the code
    only sets the link in its low half), dead bits or not."""
    changed = 0
    for block in function.blocks.values():
        for statement in block.statements:
            if not isinstance(statement, ir.SetReg) or not cpu.kept(statement.index) or statement.index in whole:
                continue
            parts, rest, keep = dataflow.partial_write(statement)
            if not keep or id(statement) not in after:
                continue
            live = after[id(statement)]
            kept = {("r", statement.index, part) for part, bits in dataflow.PARTS if keep & bits}
            if not kept & live:
                statement.value = rest
                changed += 1
    return changed


# ---- temporaries into their uses ----

def direct_reads(expression):
    """Registers, flags and stack slots an expression reads itself, whether it reads memory, and whether
    it reads a place where reading does something."""
    found = set()
    memory = False
    effect = False
    for node in ir.walk(expression):
        if isinstance(node, ir.Reg):
            found.add(("r", node.index))
        elif isinstance(node, ir.Flag):
            found.add(("f", node.name))
        elif isinstance(node, ir.Stack):
            found.add(("s", node.offset))
        elif isinstance(node, ir.Load):
            memory = True
            if cleanup.has_effect(node):
                effect = True
    return found, memory, effect


def writes_of(statement, flow):
    """What a statement may write: registers, flags and stack slots, and whether it writes memory."""
    if isinstance(statement, ir.SetReg):
        return {("r", statement.index)}, False
    if isinstance(statement, ir.SetFlag):
        return {("f", statement.name)}, False
    if isinstance(statement, ir.SetStack):
        return {("s", statement.offset)}, False
    if isinstance(statement, ir.SetTemp):
        return set(), False
    if isinstance(statement, ir.Store):
        return set(), True
    if isinstance(statement, (ir.Call, ir.Effect)):
        return {(variable[0], variable[1]) for variable in flow.statement_writes(statement)}, True
    return set(), False


def cheap(expression):
    """A value worth copying into several places."""
    if isinstance(expression, (ir.Const, ir.Reg, ir.Flag, ir.Stack, ir.Temp)):
        return True
    if isinstance(expression, ir.Op) and expression.name in ("trunc", "zext") and \
            isinstance(expression.args[0], (ir.Reg, ir.Stack, ir.Temp, ir.Flag)):
        return True
    # an address: a pointer and a displacement ("r20 + 0x30u" for a read-modify-write of a field)
    if isinstance(expression, ir.Op) and expression.name == "add" and isinstance(expression.args[1], ir.Const) and \
            isinstance(expression.args[0], (ir.Reg, ir.Stack, ir.Temp)):
        return True
    return False


def temp_uses(statement, name):
    count = 0
    for expression in ir.statement_expressions(statement):
        for node in ir.walk(expression):
            if isinstance(node, ir.Temp) and node.name == name:
                count += 1
    return count


def inline_block(block, flow):
    """One pass over a block; returns True if a temporary went into its uses."""
    statements = block.statements
    everything = statements + [block.end]
    for index, statement in enumerate(statements):
        if not isinstance(statement, ir.SetTemp):
            continue
        name = statement.temp.name
        value = statement.value
        reads, memory, effect = direct_reads(value)
        if effect:
            continue
        uses = [(position, temp_uses(other, name)) for position, other in enumerate(everything)
                if position > index]
        uses = [(position, count) for position, count in uses if count]
        total = sum(count for _, count in uses)
        if total == 0:
            continue
        if total > 1 and not cheap(value):
            continue
        last = uses[-1][0]
        movable = True
        for between in everything[index + 1:last]:
            written, writes_memory = writes_of(between, flow) if between is not block.end else (set(), False)
            if reads & written or (memory and writes_memory):
                movable = False
                break
        if not movable:
            continue
        replacement = value

        def change(node, name=name, replacement=replacement):
            return replacement if isinstance(node, ir.Temp) and node.name == name else None
        for position, _ in uses:
            ir.map_statement(everything[position], change)
        del statements[index]
        return True
    return False


def inline_temporaries(function, flow):
    changed = 0
    for block in function.blocks.values():
        while inline_block(block, flow):
            changed += 1
    return changed


def simplify_function(function):
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            ir.map_statement(statement, simplify_node)


def sink_increments(function):
    """"t = mem[r]; r = r + k; x = t" becomes "t = mem[r]; x = t; r = r + k": a register moved by a
    constant goes after the statements that follow it as long as they do not touch it, so that what it
    read goes straight where it is used (the V60's [R+] reads and writes)."""
    moved = 0
    for block in function.blocks.values():
        statements = block.statements
        position = 0
        while position < len(statements) - 1:
            statement = statements[position]
            if isinstance(statement, ir.SetReg) and isinstance(statement.value, ir.Op) \
                    and statement.value.name in ("add", "sub") and statement.value.args[0] == ir.Reg(statement.index) \
                    and isinstance(statement.value.args[1], ir.Const):
                following = statements[position + 1]
                if isinstance(following, (ir.SetTemp, ir.SetReg, ir.SetFlag, ir.Store)) \
                        and not (isinstance(following, ir.SetReg) and following.index == statement.index) \
                        and register_reads(following, statement.index) == 0:
                    statements[position], statements[position + 1] = following, statement
                    moved += 1
                    position += 1
                    continue
            position += 1
    return moved


def tidy(function, flow, after, whole=()):
    """All of the above, in place ('whole' as for narrow_partial_writes)."""
    narrow_partial_writes(function, after, whole)
    simplify_function(function)
    sink_increments(function)
    inline_temporaries(function, flow)
    simplify_function(function)


# ---- registers into their one use ----

def register_reads(statement, index):
    """How many times a statement's expressions read register 'index'."""
    count = 0
    for expression in ir.statement_expressions(statement):
        for node in ir.walk(expression):
            if isinstance(node, ir.Reg) and node.index == index:
                count += 1
    return count


def implicit_reads(statement, flow, index):
    """True if a call or effect reads the register without an expression saying so (a parameter)."""
    if isinstance(statement, ir.Call):
        reads = flow.call_effect(statement)[0]
    elif isinstance(statement, ir.Effect):
        reads = flow.effect(statement)[0]
    else:
        return False
    return any(variable[0] == "r" and variable[1] == index for variable in reads)


def writes_register(statement, flow, index):
    written, _ = writes_of(statement, flow)
    return ("r", index) in written


def inline_register_block(block, flow, after, context):
    """One pass: a register set and then read once, and dead after that read, goes into the reading
    statement. Returns True if something changed."""
    statements = block.statements
    everything = statements + [block.end]
    for index, statement in enumerate(statements):
        if not isinstance(statement, ir.SetReg) or not cpu.kept(statement.index) or statement.index in context:
            continue
        register = statement.index
        value = statement.value
        reads, memory, effect = direct_reads(value)
        if effect:
            continue
        use = None
        for position in range(index + 1, len(everything)):
            other = everything[position]
            if position < len(statements) and implicit_reads(other, flow, register):
                break
            count = register_reads(other, register)
            if count:
                if use is not None or (count > 1 and not cheap(value)):
                    use = None
                    break
                use = position
                if position == len(statements) or writes_register(other, flow, register):
                    break
                continue
            if position < len(statements) and writes_register(other, flow, register):
                break
        if use is None:
            continue
        user = everything[use]
        # dead after its use: the use writes it again, or nothing reads it later
        if use < len(statements) and not writes_register(user, flow, register):
            live = after.get(id(user))
            if live is None or any(variable[0] == "r" and variable[1] == register for variable in live):
                continue
        if use == len(statements):
            # read by the block end: it must not be live in the blocks that follow
            if any(variable[0] == "r" and variable[1] == register for variable in block_out_live(block, after)):
                continue
        movable = True
        for between in everything[index + 1:use]:
            written, writes_memory = writes_of(between, flow)
            if reads & written or (memory and writes_memory):
                movable = False
                break
        if not movable:
            continue

        def change(node, register=register, value=value):
            return value if isinstance(node, ir.Reg) and node.index == register else None
        ir.map_statement(user, change)
        del statements[index]
        return True
    return False


def block_out_live(block, after):
    """What is live at the end of a block: recorded after its last statement, by the block end's own
    reads; unknown (everything) when there is none."""
    return after.get(("end", id(block)), {("r", index, part) for index in cpu.current.registers for part in range(3)})


def inline_registers(function, flow, after, context):
    changed = 0
    for block in function.blocks.values():
        while inline_register_block(block, flow, after, set(context)):
            changed += 1
    return changed


# ---- the index of a jump table ----

def scaled_register(value, register):
    """True for register * k written as x + x * 2, x + x, x * k or x << n."""
    if not isinstance(value, ir.Op):
        return False
    name, args = value.name, value.args
    mine = ir.Reg(register)
    if name == "add" and args[0] == mine and (args[1] == mine or (
            isinstance(args[1], ir.Op) and args[1].name == "mul" and args[1].args[0] == mine
            and isinstance(args[1].args[1], ir.Const))):
        return True
    return name in ("mul", "shl") and args[0] == mine and isinstance(args[1], ir.Const)


def switch_values(function, flow):
    """"r0 = state; r0 = r0 * 3; jump to base + r0" (or call through a table so): the jump goes to
    base + state * 3, so that the switch reads the state (emit.switch_index). The register's writes in
    the block are put into each other back to one that does not read it; only when what that reads still
    holds at the jump."""
    changed = 0
    for block in function.blocks.values():
        statements = block.statements
        places = [(len(statements), block.end)] if isinstance(block.end, ir.Switch) else []
        places += [(position, statement) for position, statement in enumerate(statements)
                   if isinstance(statement, ir.Call) and statement.choices and statement.address is not None]
        for position, holder in places:
            address = holder.address
            if isinstance(holder, ir.Switch) and table_read_index(statements, position, holder, flow):
                changed += 1
                continue
            if not isinstance(address, ir.Op) or address.name != "add":
                continue
            left, right = address.args
            base, other = (left, right) if isinstance(left, ir.Const) else (right, left)
            if not isinstance(base, ir.Const) or not isinstance(other, ir.Reg):
                continue
            value = value_before(statements, position, other.index, flow)
            if value is None or not scaled(value):
                continue
            holder.address = ir.op("add", (base, value), 32)
            changed += 1
    return changed


def table_read_index(statements, position, switch, flow):
    """"r0 = table[index]; ...; jump to r0" (a table of code addresses, 099C1C): the index is kept in a
    temporary where the table is read, and the jump goes to table + index * 4 read with it, so that the
    switch reads the index (emit.switch_index) even when what the index was worked out from changes before
    the jump. True if it did that."""
    address = switch.address
    if not isinstance(address, ir.Reg):
        return False
    for index in range(position - 1, -1, -1):
        statement = statements[index]
        if isinstance(statement, ir.SetReg) and statement.index == address.index:
            break
        if writes_register(statement, flow, address.index):
            return False
    else:
        return False
    load = statement.value
    if not isinstance(load, ir.Load) or load.bits != 32 or not isinstance(load.address, ir.Op) \
            or load.address.name != "add":
        return False
    left, right = load.address.args
    base, offset = (left, right) if isinstance(left, ir.Const) else (right, left)
    if not isinstance(base, ir.Const) or not isinstance(offset, ir.Op) or offset.name != "mul" \
            or offset.args[1] != ir.const(4) or isinstance(offset.args[0], ir.Temp):
        return False
    # named as the instruction's own temporaries are (t0 of the table read: "local" lines name it)
    origin = getattr(statement, "origin", None)
    name = f"t{origin:06X}_0_index" if origin is not None else f"s{base.value & 0xFFFFFF:06X}_index"
    temp = ir.Temp(name, offset.args[0].bits)
    read = ir.Load(ir.op("add", (base, ir.op("mul", (temp, ir.const(4)), 32)), 32), 32)
    setting = ir.SetTemp(temp, offset.args[0])
    if origin is not None:
        setting.origin = origin
    statements.insert(index, setting)
    statement.value = read
    switch.address = read
    return True


def reads_register(expression, register):
    return any(isinstance(node, ir.Reg) and node.index == register for node in ir.walk(expression))


def value_before(statements, position, register, flow):
    """What 'register' holds just before statements[position], written with what still holds there: its
    writes in the block put into each other, back to one that does not read it. None if it cannot be."""
    value = ir.Reg(register)
    earliest = position
    for index in range(position - 1, -1, -1):
        statement = statements[index]
        if not isinstance(statement, ir.SetReg) or statement.index != register:
            if writes_register(statement, flow, register):
                return None
            continue
        written = statement.value

        def change(node, register=register, written=written):
            return written if isinstance(node, ir.Reg) and node.index == register else None
        value = ir.rebuild(value, change)
        earliest = index
        if not reads_register(value, register):
            break
    if earliest == position or reads_register(value, register):
        return None
    reads, memory, effect = direct_reads(value)
    if effect:
        return None
    for statement in statements[earliest + 1:position]:
        written, writes_memory = writes_of(statement, flow)
        if (reads & written) or (memory and writes_memory):
            return None
    return value


def scaled(value):
    """True for index * stride written as x + x * k, x + x, x * k or x << n."""
    if not isinstance(value, ir.Op):
        return False
    name, args = value.name, value.args
    if name == "add" and args[0] == args[1]:
        return True
    if name == "add" and isinstance(args[1], ir.Op) and args[1].name == "mul" and args[1].args[0] == args[0] \
            and isinstance(args[1].args[1], ir.Const):
        return True
    return name in ("mul", "shl") and isinstance(args[1], ir.Const)


# ---- constants in registers ----

def propagate_constants(function, call_results, context):
    """Inside each block, a register set to a constant reads as that constant until something writes it
    (a call that hands it back, an effect, or the global context registers after any call). Returns how
    many reads changed."""
    changed = 0
    for block in function.blocks.values():
        known = {}

        def change(node, known=known):
            if isinstance(node, ir.Reg) and node.index in known:
                return known[node.index]
            return None
        for statement in block.statements:
            before = repr(statement)
            if known:
                ir.map_statement(statement, change)
                ir.map_statement(statement, simplify_node)
            if repr(statement) != before:
                changed += 1
            if isinstance(statement, ir.SetReg):
                if isinstance(statement.value, ir.Const):
                    known[statement.index] = statement.value
                else:
                    known.pop(statement.index, None)
            elif isinstance(statement, ir.Call):
                for index in set(call_results.get(id(statement), ())) | set(context):
                    known.pop(index, None)
            elif isinstance(statement, ir.Effect):
                for index in (27, 28) if statement.name in ("move_string", "move_bits_down", "move_bits_up", "search_bytes") else ():
                    known.pop(index, None)
                if statement.name == "refresh_read":
                    known.pop(statement.args[1].value, None)
                if statement.name == "search_bits_up":
                    known.pop(statement.args[4].value, None)
                    known.pop(28, None)
                if statement.name in ("divx", "divux"):
                    register = statement.args[1]
                    if isinstance(register, ir.Const):
                        known.pop(register.value, None)
                        known.pop(register.value + 1, None)
                for index in context:
                    known.pop(index, None)
        if known:
            before = repr(block.end)
            ir.map_statement(block.end, change)
            ir.map_statement(block.end, simplify_node)
            if repr(block.end) != before:
                changed += 1
    return changed
