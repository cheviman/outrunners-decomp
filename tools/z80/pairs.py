# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        pairs.py
Description: splits the bytes the Z80's register pairs hold apart into variables of their own
Version:     0.1.0
Created:     13-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

The IR keeps BC, DE and HL whole (tools/z80/lift.py): a byte of one is a part of it, which is how the
data flow and the signatures see them. Code that keeps two things in a pair, a loop count in B and a
register number in C, then reads as one variable changed half at a time:
"count = (count & 0xFFu) | 0x1400u". In a function that writes a byte of a pair on its own (and uses the
pair a byte at a time more than whole), that byte becomes a register of its own (B C D E H L: BYTES), so
that it is a variable:
  - a read of that byte reads its register; a read of the whole pair puts the pair together from it,
    (b << 8) | c, or (b << 8) | (bc & 0xFF) when the other byte stays in the pair;
  - a write of that byte writes its register; a write of the whole pair writes the pair and the byte;
  - where the pair crosses the function's edges, as its callees, its callers and the data flow see it, the
    byte goes back into the pair or out of it: before a call that reads that byte, and a return or tail
    call that hands the pair back, after a call that writes it, and at the start when the function takes
    the pair.
A byte the function never writes on its own stays in the pair: reading it is reading the pair (a register
number that comes in C and goes on to the callee in C). The C does the same; what the cleanup finds
dead of all that goes. tidy_bytes then takes the bytes of a pair put together back to the bytes.
"""
from decompiler import dataflow
from v60 import ir

PAIRS = {0: (11, 12), 1: (13, 14), 2: (15, 16)}    # BC, DE, HL -> (high byte, low byte) registers
BYTES = {11: "b", 12: "c", 13: "d", 14: "e", 15: "h", 16: "l"}
SHIFT = ir.const(8, 8)
MASKS = {0: 0x00FF, 1: 0xFF00}    # the bits of each part (0 the low byte, 1 the high one)


def is_pair(node, pair):
    return isinstance(node, ir.Reg) and node.index == pair


def is_const(node, value):
    return isinstance(node, ir.Const) and node.value == value


def byte_reg(pair, part):
    return ir.Reg(PAIRS[pair][0 if part == 1 else 1], 8)


def in_pair(pair, part, byte):
    """A byte as the part of a pair it is: zero-extended, shifted into the high byte for part 1."""
    value = ir.zext(byte, 16)
    return ir.op("shl", (value, SHIFT), 16) if part == 1 else value


def put_together(pair, split, parts=(0, 1)):
    """The pair as it is now, the 'split' bytes from their registers and the rest from the pair itself;
    only the 'parts' asked for (the rest 0)."""
    pieces = []
    for part in sorted(set(parts) & {0, 1}, reverse=True):
        if part in split:
            pieces.append(in_pair(pair, part, byte_reg(pair, part)))
        else:
            pieces.append(ir.op("and", (ir.Reg(pair, 16), ir.const(MASKS[part], 16)), 16))
    if not pieces:
        return ir.const(0, 16)
    value = pieces[0]
    for piece in pieces[1:]:
        value = ir.op("or", (value, piece), 16)
    return value


def count_reads(expression, pair):
    """(reads of one byte of the pair, reads of the whole pair) in an expression."""
    halves = wholes = 0
    stack = [(expression, None)]
    while stack:
        node, parent = stack.pop()
        if is_pair(node, pair):
            byte = isinstance(parent, ir.Op) and (
                (parent.name == "trunc" and parent.bits == 8)
                or (parent.name == "shr" and is_const(parent.args[1], 8))
                or (parent.name == "and" and is_const(parent.args[1], 0xFF)))
            if byte:
                halves += 1
            else:
                wholes += 1
        if isinstance(node, ir.Op):
            stack += [(arg, node) for arg in node.args]
        elif isinstance(node, ir.Load):
            stack.append((node.address, node))
    return halves, wholes


def byte_reader(pair, split):
    """A rebuild() change that reads a split byte of the pair from its register (bottom-up: the shift and
    the mask first, then the cut that takes a byte of what they gave)."""
    def change(node):
        if not isinstance(node, ir.Op):
            return None
        name, args = node.name, node.args
        if name == "shr" and is_pair(args[0], pair) and is_const(args[1], 8) and 1 in split:
            return ir.zext(byte_reg(pair, 1), node.bits)
        if name == "and" and is_pair(args[0], pair) and is_const(args[1], 0xFF) and 0 in split:
            return ir.zext(byte_reg(pair, 0), node.bits)
        if name == "trunc" and node.bits == 8:
            inner = args[0]
            if is_pair(inner, pair) and 0 in split:
                return byte_reg(pair, 0)
            if isinstance(inner, ir.Op) and inner.name == "zext" and inner.args[0].bits == 8:
                return inner.args[0]
        return None
    return change


def whole_reader(pair, split):
    """The rest of the pair's reads: the pair put together (once: what it puts in is not looked at again)."""
    def change(node):
        return put_together(pair, split) if is_pair(node, pair) else None
    return change


def rewrite_reads(statement, pair, split):
    ir.map_statement(statement, byte_reader(pair, split))
    ir.map_statement(statement, whole_reader(pair, split))


def rewrite(expression, pair, split):
    expression = ir.rebuild(expression, byte_reader(pair, split))
    return ir.rebuild(expression, whole_reader(pair, split))


def high_byte_of(value):
    """The byte that a value written to a pair's high half (value << 8) puts there."""
    if isinstance(value, ir.Const):
        return ir.const((value.value >> 8) & 0xFF, 8)
    if isinstance(value, ir.Op) and value.name == "shl" and is_const(value.args[1], 8):
        inner = value.args[0]
        if isinstance(inner, ir.Op) and inner.name == "zext":
            inner = inner.args[0]
        return ir.trunc(inner, 8)
    return ir.trunc(ir.op("shr", (value, SHIFT), 16), 8)


def part_of(value, part):
    """Part 0 or 1 of a 16-bit value, as a byte."""
    if part == 0:
        return ir.trunc(value, 8)
    return ir.trunc(ir.op("shr", (value, SHIFT), 16), 8)


class Splitter:
    def __init__(self, function, flow, signature):
        self.function = function
        self.flow = flow
        self.signature = signature
        self.count = 0

    def temp(self, value):
        self.count += 1
        return ir.Temp(f"t_pair_{self.count}", value.bits)

    def bytes_to_split(self, pair):
        """The parts of the pair the function writes on their own, if it uses the pair more often a byte at
        a time (writing or reading one byte) than whole (reading it as an address or a 16-bit value,
        writing it from one); an empty set otherwise."""
        written = set()
        halves_used = whole_used = 0
        for block in self.function.blocks.values():
            for statement in block.statements + [block.end]:
                if isinstance(statement, ir.SetStack) and is_pair(statement.value, pair):
                    continue    # PUSH: saved and put back whole, or a byte at a time (split_slots)
                if isinstance(statement, ir.SetReg) and statement.index == pair:
                    parts, rest, _ = dataflow.partial_write(statement)
                    parts &= {0, 1}
                    if len(parts) == 1:
                        written |= parts
                        halves_used += 1
                        expressions = [rest]
                    else:
                        # (a constant or a POP writes both bytes as well as either)
                        whole_used += not isinstance(statement.value, (ir.Const, ir.Stack))
                        expressions = [statement.value]
                else:
                    expressions = list(ir.statement_expressions(statement))
                for expression in expressions:
                    halves, wholes = count_reads(expression, pair)
                    halves_used += halves
                    whole_used += wholes
        return written if halves_used > whole_used else set()

    def unpack(self, pair, parts, origin):
        """The statements that take the given parts of the pair into their bytes' registers."""
        found = [ir.SetReg(byte_reg(pair, part).index, part_of(ir.Reg(pair, 16), part))
                 for part in sorted(parts, reverse=True)]
        for statement in found:
            statement.origin = origin
        return found

    def pack(self, pair, split, origin, parts=(0, 1)):
        """The pair with the split bytes of 'parts' put back, before what reads them; None when it reads
        none of them."""
        wanted = set(parts) & split
        if not wanted:
            return None
        if wanted == {0, 1}:
            value = put_together(pair, split, parts)
        else:
            # one byte back into the pair, the other one kept
            part = next(iter(wanted))
            value = ir.op("or", (ir.op("and", (ir.Reg(pair, 16), ir.const(MASKS[1 - part], 16)), 16),
                                 in_pair(pair, part, byte_reg(pair, part))), 16)
        statement = ir.SetReg(pair, value)
        statement.origin = origin
        return statement

    @staticmethod
    def parts_of(variables, pair):
        return {variable[2] for variable in variables if variable[0] == "r" and variable[1] == pair} & {0, 1}

    def write(self, statement, pair, split):
        """The statements that stand for a write of the pair."""
        origin = getattr(statement, "origin", None)
        parts, rest, keep = dataflow.partial_write(statement)
        parts &= {0, 1}
        if len(parts) == 1:
            part = next(iter(parts))
            if part in split:
                byte = ir.trunc(rest, 8) if part == 0 else high_byte_of(rest)
                new = [ir.SetReg(byte_reg(pair, part).index, rewrite(byte, pair, split))]
            else:
                # a byte that stays in the pair: the pair, its other byte as it was there (the split
                # register holds that byte's value)
                new = [ir.SetReg(pair, ir.op("or", (ir.op("and", (ir.Reg(pair, 16), ir.const(keep, 16)), 16),
                                                    rewrite(rest, pair, split)), 16))]
        else:
            value = rewrite(statement.value, pair, split)
            if split == {0, 1}:
                temp = self.temp(value)
                new = [ir.SetTemp(temp, value)] + [ir.SetReg(byte_reg(pair, part).index, part_of(temp, part))
                                                   for part in (1, 0)]
            else:
                temp = self.temp(value)
                new = [ir.SetTemp(temp, value), ir.SetReg(pair, temp)] + \
                    [ir.SetReg(byte_reg(pair, part).index, part_of(temp, part)) for part in sorted(split)]
        for item in new:
            item.origin = origin
        return new

    def split(self, pair, split):
        function = self.function
        results = {value[1] for value in self.signature.results if value[0] == "r"}
        params = {value[1] for value in self.signature.params if value[0] == "r"}
        for label, block in function.blocks.items():
            statements = []
            # the split bytes that still hold what the pair holds there (taken from it, and neither
            # changed since): putting them back would change nothing
            same = set()
            if label == function.entry and pair in params:
                # (where the function starts: "local" lines name its bytes by it)
                statements += self.unpack(pair, split, function.entry)
                same = set(split)
            for statement in block.statements:
                origin = getattr(statement, "origin", None)
                if isinstance(statement, ir.Call):
                    reads, writes, always = self.flow.call_effect(statement)
                    # what the callee reads, and what it may write but may leave as it was: the byte taken
                    # back from the pair afterwards has to be in the pair
                    wanted = self.parts_of(reads, pair) | (self.parts_of(writes, pair)
                                                           - self.parts_of(always, pair))
                    packed = self.pack(pair, split, origin, wanted - same)
                    if packed is not None:
                        statements.append(packed)
                        same |= wanted & split
                    rewrite_reads(statement, pair, split)
                    statements.append(statement)
                    written = self.parts_of(writes, pair) & split
                    statements += self.unpack(pair, written, origin)
                    same |= written
                    continue
                if isinstance(statement, ir.SetReg) and statement.index == pair:
                    parts = dataflow.partial_write(statement)[0] & {0, 1}
                    statements += self.write(statement, pair, split)
                    if len(parts) == 1:
                        same -= parts    # the byte's register changed, or the pair's byte did
                    elif split == {0, 1}:
                        same = set()    # both bytes written, the pair left as it was
                    else:
                        same = set(split)    # the pair and its split byte written from one value
                    continue
                rewrite_reads(statement, pair, split)
                statements.append(statement)
            end = block.end
            origin = getattr(end, "origin", None)
            packed = None
            if isinstance(end, ir.TailCall):
                reads, _, _ = self.flow.combine([end.target])
                parts = self.parts_of(reads, pair) | ({0, 1} if pair in results else set())
                packed = self.pack(pair, split, origin, parts - same)
            elif isinstance(end, (ir.Return, ir.InterruptReturn)) and pair in results:
                packed = self.pack(pair, split, origin, {0, 1} - same)
            if packed is not None:
                statements.append(packed)
            rewrite_reads(end, pair, split)
            block.statements = statements

# ---- stack slots that save a pair a byte at a time ----

# where the slots of the bytes are numbered: apart from the slot's own number, which other pushes may still
# use whole (the data flow and the C take each number for a variable of its own)
BYTE_SLOTS = -0x10000

def byte_halves(value):
    """(high byte, low byte) of a 16-bit value put together from two bytes: (b << 8) | c, or a byte of it
    still in its pair ((b << 8) | (bc & 0xFF), (bc & 0xFF00) | c); None for any other value."""
    if not (isinstance(value, ir.Op) and value.name == "or" and value.bits == 16):
        return None
    for high, low in (value.args, tuple(reversed(value.args))):
        if isinstance(high, ir.Op) and high.name == "shl" and is_const(high.args[1], 8) \
                and isinstance(high.args[0], ir.Op) and high.args[0].name == "zext" and high.args[0].args[0].bits == 8:
            high_byte = high.args[0].args[0]
        elif isinstance(high, ir.Op) and high.name == "and" and is_const(high.args[1], 0xFF00) and high.args[0].bits == 16:
            high_byte = part_of(high.args[0], 1)
        else:
            continue
        if isinstance(low, ir.Op) and low.name == "zext" and low.args[0].bits == 8:
            return high_byte, low.args[0]
        if isinstance(low, ir.Op) and low.name == "and" and is_const(low.args[1], 0xFF) and low.args[0].bits == 16:
            return high_byte, part_of(low.args[0], 0)
    return None


def stack_reads(expressions):
    """{offset: bits} of the stack slots some of 'expressions' read."""
    found = {}
    for expression in expressions:
        for node in ir.walk(expression):
            if isinstance(node, ir.Stack):
                found[node.offset] = max(found.get(node.offset, 0), node.bits)
    return found


def split_slots(function):
    """A PUSH of a pair whose bytes are variables of their own stores the two bytes (the high one at the
    higher address, as the Z80 does), and the POPs it reaches read them back a byte at a time, so that
    each byte goes back to its variable. Done by web: the pushes and pops connected through one stack
    slot, when every push of the web saves bytes put together (a slot that saves HL whole at another
    time stays whole there)."""
    blocks = function.blocks
    halves_of = {}
    for block in blocks.values():
        for statement in block.statements:
            if isinstance(statement, ir.SetStack) and statement.bits == 16:
                found = byte_halves(statement.value)
                if found is not None:
                    halves_of[id(statement)] = found
    if not halves_of:
        return
    offsets = {statement.offset for block in blocks.values() for statement in block.statements
               if id(statement) in halves_of}

    def walk(label, state, record=None):
        """The writes reaching each slot after 'label' (and, with 'record', the reads on the way)."""
        state = dict(state)
        for statement in blocks[label].statements:
            expressions = [statement.value] if isinstance(statement, ir.SetStack) else ir.statement_expressions(statement)
            if record is not None:
                for offset, bits in stack_reads(expressions).items():
                    if offset in offsets:
                        record.append((statement, offset, bits, state[offset]))
            if isinstance(statement, ir.SetStack) and statement.offset in offsets:
                state[statement.offset] = frozenset([id(statement)])
        if record is not None:
            end = blocks[label].end
            for offset, bits in stack_reads(ir.statement_expressions(end)).items():
                if offset in offsets:
                    record.append((end, offset, bits, state[offset]))
        return state

    # the writes reaching each block, forwards ("entry": what the slot held when the function started)
    start = {function.entry: {offset: frozenset(["entry"]) for offset in offsets}}
    pending = [function.entry]
    while pending:
        label = pending.pop()
        state = walk(label, start[label])
        for successor in blocks[label].successors():
            old = start.get(successor)
            new = state if old is None else {offset: old[offset] | state[offset] for offset in offsets}
            if new != old:
                start[successor] = new
                pending.append(successor)
    reads = []
    for label in start:
        walk(label, start[label], reads)

    # webs: the writes and reads one slot connects
    parent = {}

    def find(key):
        while parent.setdefault(key, key) != key:
            parent[key] = parent[parent[key]]
            key = parent[key]
        return key
    for statement, offset, bits, writes in reads:
        for write in writes:
            parent[find(("read", id(statement), offset))] = find(("write", write))
    bad = set()
    for statement, offset, bits, writes in reads:
        if bits != 16 or any(write == "entry" or write not in halves_of for write in writes):
            bad.add(find(("read", id(statement), offset)))
    split = {key for key in parent if key[0] == "write" and find(key) not in bad and key[1] in halves_of}
    readers = {(id(statement), offset) for statement, offset, bits, writes in reads
               if find(("read", id(statement), offset)) not in bad}
    if not split:
        return

    def reader(offsets_here):
        def change(node):
            if isinstance(node, ir.Op) and node.name == "trunc" and node.bits == 8:
                inner = node.args[0]
                if isinstance(inner, ir.Stack) and inner.offset in offsets_here:
                    return ir.Stack(BYTE_SLOTS + inner.offset, 8)
                if isinstance(inner, ir.Op) and inner.name == "shr" and is_const(inner.args[1], 8) \
                        and isinstance(inner.args[0], ir.Stack) and inner.args[0].offset in offsets_here:
                    return ir.Stack(BYTE_SLOTS + inner.args[0].offset + 1, 8)
            if isinstance(node, ir.Stack) and node.offset in offsets_here and node.bits == 16:
                high = ir.zext(ir.Stack(BYTE_SLOTS + node.offset + 1, 8), 16)
                return ir.op("or", (ir.op("shl", (high, SHIFT), 16), ir.zext(ir.Stack(BYTE_SLOTS + node.offset, 8), 16)), 16)
            return None
        return change
    for block in blocks.values():
        statements = []
        for statement in block.statements:
            if ("write", id(statement)) in split:
                high, low = halves_of[id(statement)]
                origin = getattr(statement, "origin", None)
                for item in (ir.SetStack(BYTE_SLOTS + statement.offset + 1, high, 8),
                             ir.SetStack(BYTE_SLOTS + statement.offset, low, 8)):
                    item.origin = origin
                    statements.append(item)
                continue
            here = {offset for key, offset in readers if key == id(statement)}
            if here:
                ir.map_statement(statement, reader(here))
            statements.append(statement)
        here = {offset for key, offset in readers if key == id(block.end)}
        if here:
            ir.map_statement(block.end, reader(here))
        block.statements = statements


# ---- reading the bytes of pairs put together as the bytes ----

def byte_of(node):
    """The byte a zero-extended byte or a constant below 100h is, or None."""
    if isinstance(node, ir.Op) and node.name == "zext" and node.args[0].bits == 8:
        return node.args[0]
    if isinstance(node, ir.Const) and node.value < 0x100:
        return ir.const(node.value, 8)
    return None


def halves(node):
    """(high, low) bytes of "(high << 8) | low" (bytes zero-extended to 16 bits, or constants), or None."""
    if not (isinstance(node, ir.Op) and node.name == "or" and node.bits == 16):
        return None
    for shifted, low in (node.args, tuple(reversed(node.args))):
        if isinstance(shifted, ir.Op) and shifted.name == "shl" and is_const(shifted.args[1], 8) \
                and byte_of(shifted.args[0]) is not None and byte_of(low) is not None:
            return byte_of(shifted.args[0]), byte_of(low)
        if isinstance(shifted, ir.Const) and not shifted.value & 0xFF and byte_of(low) is not None:
            return ir.const(shifted.value >> 8, 8), byte_of(low)
    return None


def word_part(node):
    """(word, part) when 'node' is a byte of a 16-bit word in its place: (word & 0xFF), the low byte cut
    and widened, or (word >> 8) << 8, the high byte cut, widened and shifted back; else None."""
    if not isinstance(node, ir.Op) or node.bits != 16:
        return None
    name, args = node.name, node.args
    if name == "and" and isinstance(args[1], ir.Const) and args[0].bits == 16:
        if args[1].value == 0xFF:
            return args[0], 0
        if args[1].value == 0xFF00:
            return args[0], 1
    if name == "zext" and isinstance(args[0], ir.Op) and args[0].name in ("trunc", "fits") and args[0].bits == 8 \
            and args[0].args[0].bits == 16:
        return args[0].args[0], 0
    if name == "shl" and is_const(args[1], 8):
        inner = args[0]
        if isinstance(inner, ir.Op) and inner.name == "zext" and isinstance(inner.args[0], ir.Op) \
                and inner.args[0].name in ("trunc", "fits"):
            inner = inner.args[0].args[0]
        if isinstance(inner, ir.Op) and inner.name == "shr" and is_const(inner.args[1], 8) and inner.args[0].bits == 16:
            return inner.args[0], 1
    return None


def next_address(first, second):
    """True if 'second' is the address after 'first'."""
    if isinstance(first, ir.Const) and isinstance(second, ir.Const):
        return second.value == (first.value + 1) & 0xFFFF
    if isinstance(second, ir.Op) and second.name == "add" and second.args[0] == first and is_const(second.args[1], 1):
        return True
    # (ix+n) and (ix+n+1)
    return isinstance(first, ir.Op) and first.name == "add" and isinstance(second, ir.Op) and second.name == "add" \
        and first.args[0] == second.args[0] and isinstance(first.args[1], ir.Const) \
        and isinstance(second.args[1], ir.Const) and second.args[1].value == (first.args[1].value + 1) & 0xFFFF


def kept_low_byte(node):
    """The pair of "(high << 8) | (pair & 0xFF)", a split high byte put back over the low byte its pair
    kept (an RR B before LD A,C), or None: its low byte is the pair's."""
    if not (isinstance(node, ir.Op) and node.name == "or" and node.bits == 16):
        return None
    for high, low in (node.args, tuple(reversed(node.args))):
        if isinstance(high, ir.Op) and high.name == "shl" and is_const(high.args[1], 8) \
                and isinstance(low, ir.Op) and low.name == "and" and is_const(low.args[1], 0xFF) \
                and low.args[0].bits == 16:
            return low.args[0]
    return None


def tidy_change(node):
    """A byte of a pair put together is the byte it was put together from (the low byte of a split high
    byte put back over its pair: the pair's low byte, kept_low_byte); a word put together from the
    two bytes after one another in memory (LD E,(HL); INC HL; LD D,(HL)) is the word read there; what SBC A,A
    leaves (A - A - carry) is 0 - carry."""
    if not isinstance(node, ir.Op):
        return None
    name, args = node.name, node.args
    found = halves(node)
    if found is not None and all(isinstance(byte, ir.Load) for byte in found) \
            and next_address(found[1].address, found[0].address):
        return ir.Load(found[1].address, 16)
    if name == "or" and node.bits == 16:
        # the high byte and the low byte of one word put together again: the word
        sources = [word_part(arg) for arg in args]
        if None not in sources and {part for _, part in sources} == {0, 1} and sources[0][0] == sources[1][0]:
            return sources[0][0]
    if name == "sub" and isinstance(args[0], ir.Op) and args[0].name == "sub" \
            and args[0].args[0] == args[0].args[1] and not any(isinstance(item, ir.Load) for item in ir.walk(args[0])):
        return ir.op("sub", (ir.const(0, node.bits), args[1]), node.bits)
    if name == "trunc" and node.bits == 8:
        inner = args[0]
        found = halves(inner)
        if found is not None:
            return found[1]
        low = kept_low_byte(inner)
        if low is not None:
            return ir.trunc(low, 8)
        if isinstance(inner, ir.Op) and inner.name == "shr" and is_const(inner.args[1], 8):
            found = halves(inner.args[0])
            if found is not None:
                return found[0]
        if isinstance(inner, ir.Op) and inner.name == "zext" and inner.args[0].bits == 8:
            return inner.args[0]
    if name == "shr" and is_const(args[1], 8):
        found = halves(args[0])
        if found is not None:
            return ir.zext(found[0], node.bits)
    if name == "and" and is_const(args[1], 0xFF):
        found = halves(args[0])
        if found is not None:
            return ir.zext(found[1], node.bits)
    return None


def tidy_bytes(function, after=None):
    """tidy_change over the function, again: the passes that put expressions into one another bring
    bytes of pairs put together back (a POP BC, then DJNZ). A pair put together of which only one byte is
    live afterwards ('after': cleanup.py's liveness) is that byte."""
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            if after is not None and isinstance(statement, ir.SetReg) and statement.index in PAIRS:
                found = halves(statement.value)
                if found is not None:
                    live = {variable[2] for variable in after.get(id(statement), ())
                            if variable[0] == "r" and variable[1] == statement.index}
                    if live == {0}:
                        statement.value = ir.zext(found[1], 16)
                    elif live == {1}:
                        statement.value = in_pair(statement.index, 1, found[0])
            ir.map_statement(statement, tidy_change)
        block.statements, seconds = joined_stores(block.statements)
        if after is not None:
            # a store that stands for two: what is live after it is what was live after the second
            for joined, (first, second) in seconds.items():
                after[joined] = after.get(id(second), set())
                # (the two stores are gone: their ids may come back for other nodes)
                after.pop(id(first), None)
                after.pop(id(second), None)


def word_of_byte(value, part):
    """The 16-bit value whose low (part 0) or high (part 1) byte 'value' is, or None."""
    if not (isinstance(value, ir.Op) and value.name in ("trunc", "fits") and value.bits == 8):
        return None
    inner = value.args[0]
    if part == 0:
        return inner if inner.bits == 16 else None
    if isinstance(inner, ir.Op) and inner.name == "shr" and is_const(inner.args[1], 8) and inner.args[0].bits == 16:
        return inner.args[0]
    return None


def joined_stores(statements):
    """Two stores in a row of the low and the high byte of one word at an address and the next ("ld (ix+n),l;
    ld (ix+n+1),h") as the word stored: z80_write16 writes the two bytes in that order too. Returns the
    statements and {id of a joined store: the second store it stands for}."""
    found = []
    seconds = {}
    for statement in statements:
        previous = found[-1] if found else None
        # (through a pointer only: two variables in a row, or two registers of the RF5C68, stay two)
        if isinstance(statement, ir.Store) and statement.bits == 8 and isinstance(previous, ir.Store) \
                and previous.bits == 8 and not isinstance(previous.address, ir.Const) \
                and next_address(previous.address, statement.address):
            word = word_of_byte(previous.value, 0)
            if word is not None and word == word_of_byte(statement.value, 1):
                joined = ir.Store(previous.address, word, 16)
                if hasattr(previous, "origin"):
                    joined.origin = previous.origin
                found[-1] = joined
                seconds[id(joined)] = (previous, statement)
                continue
        found.append(statement)
    return found, seconds


def pure(node):
    """True for an expression of registers, stack slots, temporaries and constants only."""
    return not any(isinstance(item, ir.Load) for item in ir.walk(node))


def leaves(value):
    """The registers, stack slots and flags an expression reads."""
    found = set()
    for node in ir.walk(value):
        if isinstance(node, ir.Reg):
            found.add(("r", node.index))
        elif isinstance(node, ir.Stack):
            found.add(("s", node.offset))
        elif isinstance(node, ir.Flag):
            found.add(("f", node.name))
    return found


def written_by(statement):
    """The registers, stack slots and flags a statement writes; None for a call (anything but the stack)."""
    if isinstance(statement, ir.SetReg):
        return {("r", statement.index)}
    if isinstance(statement, ir.SetStack):
        return {("s", statement.offset)}
    if isinstance(statement, ir.SetFlag):
        return {("f", statement.name)}
    if isinstance(statement, ir.Call):
        return None
    return set()


def inlined_temp(statement):
    """True for a temporary tidy() puts into what reads it: the pairs' own, and what a POP reads from the
    slots of bytes a PUSH saved (split_slots), when they read no memory."""
    if not isinstance(statement, ir.SetTemp) or not pure(statement.value):
        return False
    return statement.temp.name.startswith("t_pair_") or any(
        isinstance(node, ir.Stack) and node.offset <= BYTE_SLOTS // 2 for node in ir.walk(statement.value))


def tidy(function):
    """The pairs' own temporaries go into what reads them, where they only put bytes together, and the
    bytes read from a pair put together become the bytes themselves. A temporary whose inputs change
    before a later read stays, from there on."""
    for block in function.blocks.values():
        values = {}
        originals = {}
        statements = []

        def inline(node, values=values):
            if isinstance(node, ir.Temp) and node.name in values:
                return values[node.name]
            return None
        for statement in block.statements:
            ir.map_statement(statement, inline)
            written = written_by(statement)
            for name in list(values):
                inputs = leaves(values[name])
                if written is None and any(kind != "s" for kind, _ in inputs) or written and inputs & written:
                    statements.append(originals.pop(name))
                    del values[name]
            if inlined_temp(statement):
                values[statement.temp.name] = statement.value
                originals[statement.temp.name] = statement
                continue
            statements.append(statement)
        ir.map_statement(block.end, inline)
        block.statements = statements
        for statement in block.statements + [block.end]:
            ir.map_statement(statement, tidy_change)


def split(function, flow, signature):
    """Splits, in place, the bytes of pairs 'function' writes on their own; returns the pairs it split."""
    splitter = Splitter(function, flow, signature)
    done = []
    for pair in PAIRS:
        parts = splitter.bytes_to_split(pair)
        if parts:
            splitter.split(pair, parts)
            done.append(pair)
    if done:
        tidy(function)
        split_slots(function)
        tidy(function)
    return done
