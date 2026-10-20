"""
Name:        dataflow.py
Description: what each function reads, writes and hands back in registers and flags, across the program
Version:     0.1.0
Created:     11-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

Variables are the parts of the V60 registers (("r", n, part): part 0 is bits 0-7, part 1 bits 8-15,
part 2 bits 16-31, since byte and halfword instructions change only the low part of a register), the
four flags (("f", name)) and a function's stack slots (("s", offset)), which stack.py made of its pushes
and pops. Temporaries never outlive the statements of one instruction; inside a block they carry the
bits their readers ask for back to the expressions that compute them.

Liveness is by demanded bits: a statement only needs the parts of its inputs that can reach the bits
read later, so "mov.b X, R3" followed by byte uses of R3 does not make the rest of R3 an input.

For every function a summary says which variables it may read before writing them (its inputs), which
it may write, and which it writes on every path (so a call can be taken to set them). Summaries are
computed until nothing changes, which also covers recursion. A computed call goes to the functions the
analysis knows can be its target. Then each call is looked at from the caller's side: what it reads
afterwards, out of what the callee may write, is what the callee hands back. That is live where the
callee returns, so the summaries are worked out again with it, until both settle: a function that may
leave a value it hands back as it found it reads that value from its caller.

A call reads what its callee reads for itself (Summary.base), and, for each value it hands back that the
caller reads afterwards, what that value is made of (Summary.passes): a caller that does not read a value
afterwards does not hand the callee its own copy of it, even when the callee may leave it as it was.

Many functions answer in R0 whether they did their job (allocating an object, say) and write their other
results only when they did; their callers test R0 right away ("tb R0", or "test.b R0" and a branch on
zero). A summary also keeps what a function always writes on its exits where the low byte, the low
halfword or the whole of R0 ends up zero, and where it does not, so the caller's side of such a test
takes only the matching results as written.
"""
from v60 import ir

from . import cpu

PARTS = ((0, 0x000000FF), (1, 0x0000FF00), (2, 0xFFFF0000))
FULL = 0xFFFFFFFF
ZERO_MASKS = (0xFF, 0xFFFF, FULL)                 # what a test of R0 looks at: byte, halfword, word


def configure():
    """The tables of the processor in use (cpu.current): the parts of the registers kept as variables (SP
    is gone), the flags, the effects interrupts may run in."""
    global KEPT, REGISTER_PARTS, FLAGS, TRACKED, INTERRUPT_EFFECTS
    KEPT = frozenset(cpu.current.registers)
    REGISTER_PARTS = [("r", index, part) for index in cpu.current.registers for part, _ in PARTS]
    FLAGS = [("f", name) for name in cpu.current.flags]
    TRACKED = frozenset(REGISTER_PARTS + FLAGS)
    INTERRUPT_EFFECTS = cpu.current.interrupt_effects


configure()


def width(bits):
    return ir.MASKS[bits] if bits <= 64 else (1 << bits) - 1


def below(mask):
    """Every bit up to the highest bit of 'mask': what an addition needs of its operands."""
    return (1 << mask.bit_length()) - 1 if mask else 0


def register_parts(index, mask):
    return {("r", index, part) for part, bits in PARTS if mask & bits}


def parts_mask(variables, index):
    mask = 0
    for part, bits in PARTS:
        if ("r", index, part) in variables:
            mask |= bits
    return mask


def shifted_right(mask, count, bits, arithmetic):
    """The bits of a value shifted right by 'count' that reach 'mask': an arithmetic shift fills the top
    with the sign bit."""
    needed = (mask << count) & width(bits)
    if arithmetic and mask & ~(width(bits) >> min(count, bits)):
        needed |= 1 << (bits - 1)
    return needed


def demanded(node, mask, out, temps):
    """Adds to 'out' the variables 'node' needs to produce the bits in 'mask'; temporaries go to 'temps'
    (name -> bits wanted of them). A temporary the expression names is wanted even when none of its bits
    are (and.h #FF of a value whose bits 8-15 are all that is read later): the statement that reads it
    stays, so the one that sets it has to stay too."""
    if not mask:
        for leaf in ir.walk(node):
            if isinstance(leaf, ir.Temp):
                temps.setdefault(leaf.name, 0)
        return
    if isinstance(node, ir.Reg):
        if node.index in KEPT:
            out |= register_parts(node.index, mask)
        return
    if isinstance(node, ir.Flag):
        out.add(("f", node.name))
        return
    if isinstance(node, ir.Stack):
        out.add(("s", node.offset))
        return
    if isinstance(node, ir.Temp):
        temps[node.name] = temps.get(node.name, 0) | mask
        return
    if isinstance(node, ir.Load):
        demanded(node.address, FULL, out, temps)
        return
    if not isinstance(node, ir.Op):
        return
    name, args = node.name, node.args
    if name == "trunc" or name == "zext":
        demanded(args[0], mask & width(min(node.bits, args[0].bits)), out, temps)
    elif name == "sext":
        source = args[1].value
        needed = mask & width(source)
        if mask & ~width(source):
            needed |= 1 << (source - 1)
        demanded(args[0], needed, out, temps)
    elif name == "and":
        left, right = args
        demanded(left, mask & right.value if isinstance(right, ir.Const) else mask, out, temps)
        demanded(right, mask & left.value if isinstance(left, ir.Const) else mask, out, temps)
    elif name in ("or", "xor", "not"):
        for arg in args:
            demanded(arg, mask, out, temps)
    elif name in ("add", "sub", "mul", "neg"):
        for arg in args:
            demanded(arg, below(mask) & width(arg.bits), out, temps)
    elif name == "shl" and isinstance(args[1], ir.Const):
        demanded(args[0], (mask >> args[1].value) & width(args[0].bits), out, temps)
    elif name == "shr" and isinstance(args[1], ir.Const):
        demanded(args[0], (mask << args[1].value) & width(args[0].bits), out, temps)
    elif name == "sar" and isinstance(args[1], ir.Const):
        demanded(args[0], shifted_right(mask, args[1].value, args[0].bits, True), out, temps)
    elif name in ("helper:shl", "helper:sha") and isinstance(args[1], ir.Const) and args[0].bits == node.bits:
        # SHL / SHA by a constant: the count is a signed byte, negative to the right
        count = args[1].value & 0xFF
        if count >= 0x80:
            count -= 0x100
        if count >= 0:
            needed = (mask >> count) & width(args[0].bits)
        else:
            needed = shifted_right(mask, -count, args[0].bits, name == "helper:sha")
        demanded(args[0], needed, out, temps)
    elif name == "select":
        demanded(args[0], FULL, out, temps)
        demanded(args[1], mask, out, temps)
        demanded(args[2], mask, out, temps)
    else:
        for arg in args:
            demanded(arg, width(arg.bits), out, temps)


def expression_reads(expression):
    """Everything an expression mentions, whole (for reports)."""
    found = set()
    for node in ir.walk(expression):
        if isinstance(node, ir.Reg) and node.index in KEPT:
            found |= register_parts(node.index, FULL)
        elif isinstance(node, ir.Flag):
            found.add(("f", node.name))
        elif isinstance(node, ir.Stack):
            found.add(("s", node.offset))
    return found


def partial_write(statement):
    """For "R = (R & keep) | rest": (parts written, rest, keep); for any other SetReg: (all parts, value, 0)."""
    value = statement.value
    if isinstance(value, ir.Op) and value.name == "or":
        kept, rest = value.args
        if isinstance(kept, ir.Op) and kept.name == "and" and isinstance(kept.args[0], ir.Reg) \
                and kept.args[0].index == statement.index and isinstance(kept.args[1], ir.Const):
            keep = kept.args[1].value
            if all((keep & bits) in (0, bits) for _, bits in PARTS):
                return {part for part, bits in PARTS if not keep & bits}, rest, keep
    return {part for part, _ in PARTS}, value, 0


# ---- R0 zero tests ----

def constant(expression, temps):
    """The value of an expression made only of constants (through temporaries bound to them), or None."""
    if isinstance(expression, ir.Const):
        return expression
    if isinstance(expression, ir.Temp):
        value = temps.get(expression.name)
        return constant(value, temps) if value is not None else None
    if isinstance(expression, ir.Op) and expression.name in ("zext", "trunc"):
        inner = constant(expression.args[0], temps)
        return ir.Const(inner.value, expression.bits) if inner is not None else None
    return None


def zero_state_after_write(statement, state, temps):
    """R0's (byte, halfword, word) zero-ness after a write to it, from before it; None when not known."""
    _, rest, keep = partial_write(statement)
    rest = constant(rest, temps)
    if rest is None:
        return (None, None, None)
    value = rest.value
    byte_zero = value & 0xFF == 0
    if keep == 0:
        return (byte_zero, value & 0xFFFF == 0, value == 0)
    old_byte, old_half, old_word = state
    if keep == 0xFFFFFF00:
        half = False if not byte_zero else (True if old_half else None)
        word = False if not byte_zero else (True if old_word else None)
        return (byte_zero, half, word)
    half_zero = value & 0xFFFF == 0
    word = False if not half_zero else (True if old_word else None)
    return (byte_zero, half_zero, word)


def refine(state, mask, zero):
    """R0's zero-ness once a test of its 'mask' bits said 'zero'."""
    byte, half, word = state
    if mask == 0xFF:
        byte = zero
        if not zero:
            half, word = False, False
    elif mask == 0xFFFF:
        half = zero
        if zero:
            byte = True
        else:
            word = False
    else:
        word = zero
        if zero:
            byte, half = True, True
    return (byte, half, word)


def known(state, mask):
    return state[ZERO_MASKS.index(mask)]


def r0_view(expression, definitions):
    """The width of R0 'expression' stands for (8, 16, 32), following temporaries; None if it is not R0."""
    while isinstance(expression, ir.Temp) and expression.name in definitions:
        expression = definitions[expression.name]
    result = cpu.current.result_register
    if isinstance(expression, ir.Reg) and expression.index == result:
        return expression.bits
    if isinstance(expression, ir.Op) and expression.name == "trunc" and isinstance(expression.args[0], ir.Reg) \
            and expression.args[0].index == result:
        return expression.bits
    return None


def zero_test(block):
    """If 'block' ends by branching on whether low bits of R0 are zero: (mask, taken edge means zero,
    index of the first statement of the test, so that everything from there on is the test), else None."""
    end = block.end
    if not isinstance(end, ir.Branch) or cpu.current.result_register is None:
        return None
    condition = end.condition
    statements = block.statements
    if isinstance(condition, ir.Op) and condition.name == "eq" and isinstance(condition.args[1], ir.Const) \
            and condition.args[1].value == 0 and isinstance(condition.args[0], ir.Reg) \
            and condition.args[0].index == cpu.current.result_register:
        return FULL, True, len(statements)    # tb R0
    if isinstance(condition, ir.Op) and condition.name in ("eq", "ne") and isinstance(condition.args[1], ir.Const) \
            and condition.args[1].value == 0:
        # a test of R0 that conditions.py turned into a comparison: the statements of the test are still there
        start = len(statements)
        while start > 0 and isinstance(statements[start - 1], (ir.SetTemp, ir.SetFlag)):
            start -= 1
        definitions = {statement.temp.name: statement.value for statement in statements[start:]
                       if isinstance(statement, ir.SetTemp)}
        bits = r0_view(condition.args[0], definitions)
        if bits is None:
            return None
        return width(bits), condition.name == "eq", start
    if isinstance(condition, ir.Flag) and condition.name == "z":
        taken_zero = True
    elif isinstance(condition, ir.Op) and condition.name == "eq" and isinstance(condition.args[0], ir.Flag) \
            and condition.args[0].name == "z" and isinstance(condition.args[1], ir.Const) \
            and condition.args[1].value == 0:
        taken_zero = False
    else:
        return None
    definitions = {}
    for statement in statements:
        if isinstance(statement, ir.SetTemp):
            definitions[statement.temp.name] = statement.value
    for index in range(len(statements) - 1, -1, -1):
        statement = statements[index]
        if not isinstance(statement, (ir.SetTemp, ir.SetFlag)):
            return None
        if isinstance(statement, ir.SetFlag) and statement.name == "z":
            value = statement.value
            if not (isinstance(value, ir.Op) and value.name == "eq" and isinstance(value.args[1], ir.Const)
                    and value.args[1].value == 0):
                return None
            bits = r0_view(value.args[0], definitions)
            if bits is None:
                return None
            start = index
            while start > 0 and isinstance(statements[start - 1], (ir.SetTemp, ir.SetFlag)):
                start -= 1
            return width(bits), taken_zero, start
    return None


class Summary:
    def __init__(self):
        self.reads = set()
        self.writes = set()
        self.always = set(TRACKED)   # shrinks: written on every path that returns
        # written on every exit where R0's (mask) bits are zero, or not: (mask, zero) -> set, None if no such exit
        self.always_when = {}
        self.base = set()      # what it reads with nothing live where it ends
        self.passes = {}       # ("r", n) or ("f", name) it hands back -> what it reads for that value

    def key(self):
        outcomes = tuple(sorted((mask, zero, frozenset(value) if value is not None else None)
                                for (mask, zero), value in self.always_when.items()))
        passes = tuple(sorted((key, frozenset(value)) for key, value in self.passes.items()))
        return (frozenset(self.reads), frozenset(self.writes), frozenset(self.always), outcomes,
                frozenset(self.base), passes)


class Program:
    """Data flow over the decompiler's functions."""

    def __init__(self, functions, analysis, interrupt_handlers, context=()):
        self.functions = functions
        self.analysis = analysis
        self.interrupt_handlers = [entry for entry in interrupt_handlers if entry in functions]
        # the registers the whole program shares: global variables in C (signatures.py)
        self.context = frozenset(variable for index in context for variable in register_parts(index, FULL))
        self.summaries = {entry: Summary() for entry in functions}
        self.slot_targets = self.code_pointer_slots()
        # inputs that start at 0 in C (signatures.py): not read from the caller, so not live at its calls
        self.zeroed = {}
        # live where each function ends: what it hands back to its callers (solve)
        self.exit_live = {}
        self.declared = {}    # entry -> register numbers a function nothing calls hands back (symbols.txt)

    def code_pointer_slots(self):
        """For each memory slot calls read their target from (by displacement): the functions the program
        stores there. Object handlers go to 1C; 9C holds the next one, which the object later moves to 1C;
        the tables symbols.txt describes hold handlers too. A stored 0 is no handler."""
        slots = {}
        for _, value, slot in self.analysis.pointer_writes:
            key = value & 0xFFFFFF
            if value and key in self.functions:
                slots.setdefault(slot, set()).add(key)
        handlers = slots.setdefault(0x1C, set())
        handlers |= slots.get(0x9C, set())
        handlers |= {target for target in self.analysis.described_targets if target in self.functions}
        return slots

    # ---- calls ----

    def call_targets(self, call):
        if call.target is not None:
            return [call.target & 0xFFFFFF]
        if call.choices:
            return [choice for choice in call.choices if choice in self.functions]
        if call.slot is not None and call.slot in self.slot_targets:
            return sorted(self.slot_targets[call.slot])
        raise ValueError(f"a computed call without known targets: {call}")

    def combine(self, targets):
        """Reads and writes of a call that may go to any of 'targets'; 'always' only if every one does."""
        reads, writes = set(), set()
        always = None
        for target in targets:
            summary = self.summaries.get(target)
            if summary is None:
                continue
            reads |= summary.reads
            writes |= summary.writes
            always = set(summary.always) if always is None else always & summary.always
        return reads, writes, always or set()

    def call_effect(self, call):
        """(reads, writes, always) of a call, as the caller sees them. Code called through a memory slot gets
        nothing from its caller and hands nothing back (signatures.py): only the context registers, global
        variables in C, pass between them."""
        reads, writes, always = self.combine(self.call_targets(call))
        if call.slot is None:
            return reads, writes, always
        return set(self.context), writes & self.context, set()

    def call_reads(self, call, live_after):
        """What a call reads, given what is live after it (see the module's notes)."""
        if call.slot is not None:
            return set(self.context)
        return self.target_reads(self.call_targets(call), live_after)

    def target_reads(self, targets, live_after):
        reads = set()
        for target in targets:
            summary = self.summaries.get(target)
            if summary is None:
                continue
            reads |= summary.base
            for variable in live_after & summary.writes:
                found = summary.passes.get((variable[0], variable[1]))
                # a value not handed back yet (the outputs still grow): everything it may read
                reads |= summary.reads if found is None else found
        return reads

    def call_writes(self, call):
        return {(v[0], v[1]) if v[0] == "r" else v for v in self.call_effect(call)[1]}

    def outcome_kills(self, call, mask, zero):
        """What a call certainly writes on its exits where R0's 'mask' bits are zero ('zero') or not."""
        if call.slot is not None:
            return set()
        always = None
        for target in self.call_targets(call):
            summary = self.summaries.get(target)
            if summary is None:
                continue
            outcome = summary.always_when.get((mask, zero))
            if outcome is None:
                outcome = summary.always
            always = set(outcome) if always is None else always & outcome
        return always or set()

    # ---- one statement, backwards: live after -> live before ----

    def transfer(self, statement, live, temps, end=False):
        """Liveness through one statement (or a block end when 'end'); updates 'live' and 'temps' in place.
        Returns (variables it may write, variables it always writes)."""
        if end:
            return self.transfer_end(statement, live, temps)
        if isinstance(statement, ir.SetTemp):
            wanted = temps.pop(statement.temp.name, 0)
            demanded(statement.value, wanted, live, temps)
            return set(), set()
        if isinstance(statement, ir.SetReg):
            index = statement.index
            if not cpu.kept(index):
                demanded(statement.value, FULL, live, temps)
                return set(), set()
            parts, value, _ = partial_write(statement)
            written = {("r", index, part) for part in parts}
            needed = parts_mask(live & written, index)
            live -= written
            demanded(value, needed, live, temps)
            return written, written
        if isinstance(statement, ir.SetFlag):
            variable = ("f", statement.name)
            if variable in live:
                live.discard(variable)
                demanded(statement.value, 1, live, temps)
            return {variable}, {variable}
        if isinstance(statement, ir.SetStack):
            variable = ("s", statement.offset)
            if variable in live:
                live.discard(variable)
                demanded(statement.value, width(statement.bits), live, temps)
            return {variable}, {variable}
        if isinstance(statement, ir.Store):
            demanded(statement.address, FULL, live, temps)
            demanded(statement.value, width(statement.bits), live, temps)
            return set(), set()
        if isinstance(statement, ir.Call):
            reads = self.call_reads(statement, live)
            _, writes, always = self.call_effect(statement)
            live -= always
            live |= reads
            if statement.address is not None:
                demanded(statement.address, FULL, live, temps)
            return writes, always
        if isinstance(statement, ir.Effect):
            reads, writes, always = self.effect(statement)
            live -= always
            live |= reads
            for arg in statement.args:
                demanded(arg, width(arg.bits), live, temps)
            return writes, always
        raise TypeError(f"not a statement: {statement!r}")

    def transfer_end(self, end, live, temps):
        if isinstance(end, ir.TailCall):
            reads = self.target_reads([end.target], live)
            _, writes, always = self.combine([end.target])
            live -= always
            live |= reads
            return writes, always
        for expression in ir.statement_expressions(end):
            demanded(expression, FULL, live, temps)
        return set(), set()

    def effect(self, statement):
        """(reads, writes, always) of an effect beyond its arguments."""
        name = statement.name
        flags = set(FLAGS)
        if name == "update_psw":
            mask = statement.args[1]
            if isinstance(mask, ir.Const) and not mask.value & 0xF:
                return set(), set(), set()    # only control bits (interrupts on or off): the flags stay
            return flags, flags, flags
        if name in ("write_psw_low", "write_psw"):
            return set(), flags, flags
        if name in ("divx", "divux"):
            register = statement.args[1]
            written = {("f", "s"), ("f", "z")}
            if isinstance(register, ir.Const) and register.value < 30:
                pair = register_parts(register.value, FULL) | register_parts(register.value + 1, FULL)
                return pair, written | pair, written | pair
            return set(), written, written
        if name == "move_string":
            pair = register_parts(27, FULL) | register_parts(28, FULL)
            return register_parts(26, FULL), pair, pair
        if name in ("move_bits_down", "move_bits_up"):
            pair = register_parts(27, FULL) | register_parts(28, FULL)
            return set(), pair, set()
        if name == "search_bits_up":
            # the count goes to its register; R28 only changes when the length is not 0
            count = register_parts(statement.args[4].value, FULL)
            return set(), count | register_parts(28, FULL), count
        if name in INTERRUPT_EFFECTS:
            # the handlers interrupt the code without taking anything from it or leaving anything in its
            # registers, which they save and put back; they share the context registers
            _, writes, _ = self.combine(self.interrupt_handlers)
            return set(self.context), writes & self.context, set()
        return set(), set(), set()

    def statement_writes(self, statement):
        if isinstance(statement, ir.Call):
            return self.call_effect(statement)[1]
        if isinstance(statement, ir.Effect):
            return self.effect(statement)[1]
        return set()

    # ---- one function ----

    def split_call(self, block):
        """For a block that ends "call; test of R0; branch": (index of the call, test) or None."""
        test = zero_test(block)
        if test is None:
            return None
        _, _, start = test
        if start == 0 or not isinstance(block.statements[start - 1], ir.Call):
            return None
        return start - 1, test

    def block_liveness(self, block, live_in, record_calls, calls, exit_live=frozenset()):
        """Live variables at the start of 'block' given those of its successors, and 'exit_live' where the
        function ends."""
        temps = {}
        split = self.split_call(block)
        if split is None:
            live = set()
            for successor in block.successors():
                live |= live_in[successor]
            if isinstance(block.end, (ir.Return, ir.TailCall, ir.InterruptReturn)):
                live |= exit_live
            self.transfer(block.end, live, temps, end=True)
            statements = block.statements
        else:
            call_index, (mask, taken_zero, _) = split
            call = block.statements[call_index]
            live = set()
            after = set()
            for successor, zero in ((block.end.target, taken_zero), (block.end.otherwise, not taken_zero)):
                edge = set(live_in[successor])
                edge_temps = {}
                self.transfer(block.end, edge, edge_temps, end=True)
                for statement in reversed(block.statements[call_index + 1:]):
                    self.transfer(statement, edge, edge_temps)
                after |= edge
                edge -= self.outcome_kills(call, mask, zero)
                live |= edge
            if record_calls:
                calls[id(call)] = (call, after)
            live |= self.call_reads(call, after)
            if call.address is not None:
                demanded(call.address, FULL, live, temps)
            statements = block.statements[:call_index]
        for statement in reversed(statements):
            if record_calls and isinstance(statement, ir.Call):
                calls[id(statement)] = (statement, set(live))
            self.transfer(statement, live, temps)
        return live

    def liveness(self, entry, record_calls=False, exit_live=None):
        """Live variables at each block start; with 'record_calls', also [(call, live after it)]. 'exit_live'
        is what is live where the function ends (by default what it hands back)."""
        function = self.functions[entry]
        blocks = function.blocks
        live_in = {label: set() for label in blocks}
        calls = {}
        if exit_live is None:
            exit_live = self.exit_live.get(entry, frozenset())
        changed = True
        while changed:
            changed = False
            for label in reversed(list(blocks)):
                live = self.block_liveness(blocks[label], live_in, record_calls, calls, exit_live)
                if live != live_in[label]:
                    live_in[label] = live
                    changed = True
        return live_in, list(calls.values())

    def zero_state_after(self, block, state):
        """R0's zero-ness at the end of a block, from its zero-ness at the start."""
        temps = {}
        for statement in block.statements:
            if isinstance(statement, ir.SetTemp):
                temps[statement.temp.name] = statement.value
            elif isinstance(statement, ir.SetReg) and statement.index == cpu.current.result_register:
                state = zero_state_after_write(statement, state, temps)
            elif isinstance(statement, (ir.Call, ir.Effect)):
                if any(v[0] == "r" and v[1] == 0 for v in self.statement_writes(statement)):
                    state = (None, None, None)
        return state

    def outcome_always(self, entry, kills):
        """{(mask, zero): written on every exit where R0's mask bits are zero, or not}; forward over (block,
        R0 zero-ness), so paths that tested R0 keep apart when they meet again."""
        function = self.functions[entry]
        blocks = function.blocks
        start = (function.entry, (None, None, None))
        facts = {start: set()}
        pending = [start]
        when = {}
        while pending:
            key = pending.pop()
            label, state = key
            written = facts[key]
            block = blocks[label]
            out_state = self.zero_state_after(block, state)
            out_written = written | kills[label]
            end = block.end
            if not block.successors():
                # a tail call that may write R0 ends the function with the callee's outcome, and with what
                # the callee writes on it
                callee = self.summaries.get(end.target) if isinstance(end, ir.TailCall) else None
                handed = callee is not None and any(v[0] == "r" and v[1] == 0 for v in callee.writes)
                for mask in ZERO_MASKS:
                    for zero in (True, False):
                        fact = None if handed else known(out_state, mask)
                        if fact is None or fact == zero:
                            exit_written = out_written
                            if handed:
                                outcome = callee.always_when.get((mask, zero))
                                exit_written = out_written | (callee.always if outcome is None else outcome)
                            current = when.get((mask, zero))
                            when[(mask, zero)] = set(exit_written) if current is None else current & exit_written
                continue
            test = zero_test(block)
            if test is not None:
                mask, taken_zero, _ = test
                edges = [(end.target, refine(out_state, mask, taken_zero)),
                         (end.otherwise, refine(out_state, mask, not taken_zero))]
            else:
                edges = [(successor, out_state) for successor in block.successors()]
            for successor, edge_state in edges:
                successor_key = (successor, edge_state)
                current = facts.get(successor_key)
                new = set(out_written) if current is None else current & out_written
                if current is None or new != current:
                    facts[successor_key] = new
                    pending.append(successor_key)
        return when

    def solve_function(self, entry):
        """New summary of one function from its callees' current summaries."""
        function = self.functions[entry]
        blocks = function.blocks
        live_in, _ = self.liveness(entry)
        writes, kills = {}, {}
        for label, block in blocks.items():
            written, killed = set(), set()
            scratch_live, temps = set(), {}
            for statement in block.statements:
                may, always = self.transfer(statement, scratch_live, temps)
                written |= may
                killed |= always
            may, always = self.transfer(block.end, scratch_live, temps, end=True)
            written |= may
            killed |= always
            writes[label], kills[label] = written, killed

        # written on every path to an exit, forwards
        always_in = {label: None for label in blocks}
        always_in[function.entry] = set()
        changed = True
        while changed:
            changed = False
            for label in blocks:
                if always_in[label] is None:
                    continue
                out = always_in[label] | kills[label]
                for successor in blocks[label].successors():
                    current = always_in[successor]
                    new = set(out) if current is None else current & out
                    if new != current:
                        always_in[successor] = new
                        changed = True
        exits = [always_in[label] | kills[label] for label, block in blocks.items()
                 if not block.successors() and always_in[label] is not None]

        summary = Summary()
        zeroed = self.zeroed.get(entry, ())
        # a register the function saves and puts back is a variable of its own in C: the caller's stays
        kept = {variable for variable in TRACKED if variable[0] == "r" and variable[1] in getattr(function, "preserved", ())}
        summary.reads = {variable for variable in live_in[function.entry] if variable[0] in ("r", "f")
                         and (variable[0], variable[1]) not in zeroed}
        summary.writes = {variable for written in writes.values() for variable in written
                          if variable[0] in ("r", "f")} - kept
        summary.always = (set.intersection(*exits) & TRACKED if exits else set(TRACKED)) - kept
        summary.always_when = {key: value & TRACKED - kept for key, value in self.outcome_always(entry, kills).items()}

        def entry_reads(exit_live):
            found, _ = self.liveness(entry, exit_live=exit_live)
            return {variable for variable in found[function.entry] if variable[0] in ("r", "f")
                    and (variable[0], variable[1]) not in zeroed}
        handed = self.exit_live.get(entry, frozenset())
        summary.base = entry_reads(frozenset()) if handed else set(summary.reads)
        if summary.base != summary.reads:
            for key in {(variable[0], variable[1]) for variable in handed}:
                summary.passes[key] = entry_reads(frozenset(v for v in handed if (v[0], v[1]) == key))
        return summary

    def solve_summaries(self):
        """Summaries of every function, repeated until none changes."""
        rounds = 0
        changed = True
        while changed:
            changed = False
            rounds += 1
            for entry in self.functions:
                new = self.solve_function(entry)
                if new.key() != self.summaries[entry].key():
                    self.summaries[entry] = new
                    changed = True
        return rounds

    def reset(self):
        """Forgets the summaries and outputs, for functions that changed so that they read less."""
        self.summaries = {entry: Summary() for entry in self.functions}
        self.exit_live = {}

    def solve(self):
        """Summaries and outputs together. What a function hands back is live where it returns, so a
        function reads what it may hand back unchanged ("and.b #FF, R0" leaves R0 as it was: the caller's
        R0 goes through it). Outputs only grow, until they settle."""
        while True:
            self.solve_summaries()
            outputs = self.outputs()
            if all(outputs[entry] <= self.exit_live.get(entry, frozenset()) for entry in self.functions):
                return
            for entry in self.functions:
                self.exit_live[entry] = frozenset(self.exit_live.get(entry, frozenset()) | outputs[entry])

    def live_after_calls(self, entry):
        return self.liveness(entry, record_calls=True)[1]

    def outputs(self):
        """For each function: the variables some caller reads after calling it, out of what it writes. A call
        through a memory slot takes nothing back (signatures.py)."""
        handed = {entry: set() for entry in self.functions}
        for entry in self.functions:
            for call, live in self.live_after_calls(entry):
                if call.slot is not None:
                    continue
                for target in self.call_targets(call):
                    if target in handed:
                        handed[target] |= live & self.summaries[target].writes
        # what a function nothing calls hands back: symbols.txt says it ("result" lines)
        for entry, registers in self.declared.items():
            if entry in handed:
                handed[entry] |= {variable for variable in self.summaries[entry].writes
                                  if variable[0] == "r" and variable[1] in registers}
        # a tail call hands the callee's results to this function's callers
        changed = True
        while changed:
            changed = False
            for entry, function in self.functions.items():
                for block in function.blocks.values():
                    if isinstance(block.end, ir.TailCall) and block.end.target in handed:
                        new = handed[entry] & self.summaries[block.end.target].writes
                        if not new <= handed[block.end.target]:
                            handed[block.end.target] |= new
                            changed = True
        return handed
