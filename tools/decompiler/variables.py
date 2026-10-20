"""
Name:        variables.py
Description: splits each register of a function into the variables it really holds (one per web of
             definitions and uses), each with the width its uses need
Version:     0.1.0
Created:     12-10-2026
Modified:    15-10-2026
Author:      Sebastian Toyos

Hand-written assembly reuses a register for one thing after another. A definition (an assignment, a call
that hands the register back, the function's start) and the uses it reaches make a web; webs that reach a
common use are one variable, and so are the values an assignment works out from the register itself
(r19 += 2 as it walks a list, the low byte counted up), as long as it does not only look something up
with it (r0 = table[r0] is a new value) or keep the rest of it (mov.b #9, R0 puts a new value, a sound
number, in the low byte of whatever R0 held). A call that hands a register back through a pointer takes the caller's
variable in and out, so what reaches the call and what it hands back are one web, unless what the
register held does not matter to the call: the callee always writes the whole register and never reads
what it held (queue_sprite_at_depth's bucket head), or it writes every part of it the caller reads
afterwards and only hands the rest back as it was (animate's flag in the low byte of R7, when the caller
looks at nothing else of R7). Then what it hands back starts a web of its own. The web that holds the function's parameter or result has the name
symbols.txt gives it, or the register's (r0); the others are r0_2, r0_3...

A variable is as wide as the bits anyone reads from it: when only the low byte of a web is ever live
(cleanup.py's liveness by parts), it is a uint8_t; when only the low halfword, a uint16_t. The C behaves
the same, because no use looks at the bits the narrower type drops.

Webs with a name (below) are variables by that name. What calls write in registers and nothing reads
(the frame animate hands back to a caller that only wants to know whether the animation ended) goes to
one variable of the function, ignored, whatever the register: the call needs somewhere to write it.
The rest share one variable per register and width, the register itself (r5): values nothing says
more about. To name those (decompile.py --split-webs), each web the function reads (in its own
expressions, as a parameter of a call, or as its result) is instead a variable of its own: one register
holds the screen y, then whether the object went off screen, then a sound, and each is a variable
(r5, r5_2...) that a "local" line can name by the instructions that write it.

The context registers are global variables and stay as they are. A web the caller can say more about
(hint: what its values are, an object the code reads fields through, a field it was read from, the
parameter it is passed as) is named for it, with a number when the name repeats: object, object_2,
kind; webs with the same name and width share a variable. symbols.txt can name any of them ("local" lines): by the address of an instruction that writes it,
which stays where it is whatever this module makes of the webs.
"""
from v60 import ir

from . import cpu

from . import dataflow

FULL_PARTS = (0, 1, 2)


def outside_loads(expression):
    """The nodes of an expression, without going into the addresses of the memory it reads."""
    stack = [expression]
    while stack:
        node = stack.pop()
        yield node
        if isinstance(node, ir.Op):
            stack.extend(node.args)


class Union:
    def __init__(self):
        self.parent = {}

    def find(self, item):
        self.parent.setdefault(item, item)
        while self.parent[item] != item:
            self.parent[item] = self.parent[self.parent[item]]
            item = self.parent[item]
        return item

    def join(self, a, b):
        a, b = self.find(a), self.find(b)
        if a != b:
            self.parent[b] = a


class Variables:
    """Webs of one function. 'call_info(statement)' gives (registers a call reads as arguments, registers it
    hands back); 'end_reads(end)' the registers a block end reads (results, a tail call's arguments)."""

    def __init__(self, function, context, call_info, end_reads, entry_registers, after, live_in, hint=None,
                 entry_names=None, chosen=None, really_reads=None, fresh=None, split=False, passes_only=None,
                 reads_input=None):
        self.function = function
        self.context = set(context)
        self.call_info = call_info
        self.end_reads = end_reads
        self.entry_registers = entry_registers    # parameters and results: defined when the function starts
        self.after = after                        # cleanup.py: variables live after each statement
        self.live_in = live_in                    # ... and at the function's start
        self.webs = Union()
        self.uses = {}        # id(Reg node) -> definitions reaching it
        self.implicit = {}    # (id(statement), register) -> definitions reaching it
        self.defined = {}     # (id(statement), register) -> its definition
        self.through_pointer = set()    # definitions a call or effect makes, through a pointer to the variable
        self.hint = hint                # (this, web root, its items) -> a name for it, or None
        self.entry_names = entry_names or {}    # register -> name of the parameter or result it holds
        self.chosen = chosen or {}      # (address of a definition or "entry", "rN") -> symbols.txt's name
        # (statement, register) -> True if a call, an effect or a block end reads the register for what it
        # does (a parameter, a result), not only to hand it back as it was
        self.really_reads = really_reads or (lambda statement, register: True)
        # call -> the registers it hands back that its callee always writes whole and never reads
        self.fresh = fresh or (lambda statement: set())
        self.split = split    # every web the function reads is a variable of its own (for naming them)
        # (call, register) -> True if the callee reads what the register held only to hand it back unchanged
        self.passes_only = passes_only or (lambda statement, register: False)
        # (call, register it hands back) -> True if what the caller's register held matters to the call: the
        # callee reads it for itself, or may hand back as it was some part the caller reads afterwards
        self.reads_input = reads_input or (lambda statement, register: True)
        self.run()

    # ---- definitions and uses of one statement ----

    def scratch(self, index):
        return cpu.kept(index) and index not in self.context

    def statement_defs(self, statement):
        """Registers a statement defines."""
        if isinstance(statement, ir.SetReg):
            return [statement.index] if self.scratch(statement.index) else []
        if isinstance(statement, (ir.Call, ir.TailCall)):
            return [index for index in self.call_info(statement)[1] if self.scratch(index)]
        if isinstance(statement, ir.Effect):
            if statement.name in ("move_string", "move_bits_down", "move_bits_up"):
                return [index for index in (27, 28) if self.scratch(index)]
            if statement.name == "search_bits_up":
                return [index for index in (statement.args[4].value, 28) if self.scratch(index)]
            if statement.name in ("divx", "divux") and isinstance(statement.args[1], ir.Const):
                low = statement.args[1].value
                return [index for index in (low, low + 1) if self.scratch(index)]
        return []

    def implicit_uses(self, statement):
        """Registers a statement reads without an expression naming them."""
        if isinstance(statement, ir.Call):
            arguments, handed_back = self.call_info(statement)
            # a result is copied in by the callee: the caller's value is read too, unless the callee always
            # writes all of it first, or what it may leave as it was is nothing the caller reads afterwards
            return [index for index in set(arguments) | (set(handed_back) - self.new_values(statement))
                    if self.scratch(index)]
        if isinstance(statement, ir.Effect) and statement.name in ("divx", "divux") \
                and isinstance(statement.args[1], ir.Const):
            low = statement.args[1].value
            return [index for index in (low, low + 1) if self.scratch(index)]
        if isinstance(statement, (ir.Return, ir.TailCall, ir.InterruptReturn)):
            reads = set(self.end_reads(statement))
            if isinstance(statement, ir.TailCall):
                reads -= self.new_values(statement)    # the function it goes on in writes them, whatever they held
            return [index for index in reads if self.scratch(index)]
        return []

    def new_values(self, statement):
        """The registers a call hands back as values of their own: what they held before does not matter to
        it (see the module's notes)."""
        arguments, handed_back = self.call_info(statement)
        return self.fresh(statement) | {index for index in handed_back
                                        if index not in arguments and not self.reads_input(statement, index)}

    def step(self, statement, state, record):
        if record:
            for expression in ir.statement_expressions(statement):
                for node in ir.walk(expression):
                    if isinstance(node, ir.Reg) and self.scratch(node.index):
                        reaching = state.get(node.index, frozenset())
                        self.uses[id(node)] = reaching
                        self.link(reaching)
            for index in self.implicit_uses(statement):
                reaching = state.get(index, frozenset())
                self.implicit[(id(statement), index)] = reaching
                self.link(reaching)
        for index in self.statement_defs(statement):
            definition = ("def", id(statement), index)
            state[index] = frozenset([definition])
            if record:
                self.defined[(id(statement), index)] = definition
                self.webs.find(definition)
                if isinstance(statement, ir.SetReg):
                    # worked out from the register itself: the same variable, changed (in a partial write,
                    # the part it writes has to come from the register too)
                    _, written, _ = dataflow.partial_write(statement)
                    for node in outside_loads(written):
                        if isinstance(node, ir.Reg) and node.index == index:
                            for previous in self.uses.get(id(node), ()):
                                self.webs.join(previous, definition)
                if not isinstance(statement, ir.SetReg):
                    self.through_pointer.add(definition)
                if isinstance(statement, (ir.Call, ir.TailCall)) and index in self.call_info(statement)[1] \
                        or (isinstance(statement, ir.Effect) and statement.name in ("divx", "divux")):
                    # the same variable goes in (copied in, or divided) and comes out
                    for previous in self.implicit.get((id(statement), index), ()):
                        self.webs.join(previous, definition)

    def link(self, definitions):
        definitions = list(definitions)
        for other in definitions[1:]:
            self.webs.join(definitions[0], other)

    def run(self):
        blocks = self.function.blocks
        entry = self.function.entry
        registers = set()
        for block in blocks.values():
            for statement in block.statements + [block.end]:
                for expression in ir.statement_expressions(statement):
                    for node in ir.walk(expression):
                        if isinstance(node, ir.Reg) and self.scratch(node.index):
                            registers.add(node.index)
                registers |= set(self.statement_defs(statement)) | set(self.implicit_uses(statement))
        self.registers = registers
        start_state = {index: frozenset([("entry", index)]) for index in registers}
        start = {entry: start_state}
        pending = [entry]
        while pending:
            label = pending.pop()
            state = dict(start[label])
            for statement in blocks[label].statements:
                self.step(statement, state, False)
            for successor in blocks[label].successors():
                old = start.get(successor)
                if old is None:
                    start[successor] = dict(state)
                    pending.append(successor)
                    continue
                merged = {index: old.get(index, frozenset()) | state.get(index, frozenset())
                          for index in set(old) | set(state)}
                if merged != old:
                    start[successor] = merged
                    pending.append(successor)
        self.block_start = start
        for label, state in start.items():
            state = dict(state)
            block = blocks[label]
            for statement in block.statements:
                self.step(statement, state, True)
            self.step(block.end, state, True)
        for index in registers:
            self.webs.find(("entry", index))
        self.read = self.read_definitions()
        self.name_webs()

    def read_definitions(self):
        """The definitions something reads: an expression of the function, a call that takes the register
        as a parameter, an effect, the function's end that hands it back."""
        found = set()
        for definitions in self.uses.values():
            found |= definitions
        for (statement_id, index), definitions in self.implicit.items():
            if self.really_reads(self.statement_of(statement_id), index):
                found |= definitions
        return found

    # ---- names and widths ----

    def name_webs(self):
        """Each web: its C name and width."""
        members = {}
        for item in list(self.webs.parent):
            members.setdefault(self.webs.find(item), []).append(item)
        self.names = {}
        self.widths = {}
        used_roots = set()
        for definitions in self.uses.values():
            used_roots |= {self.webs.find(definition) for definition in definitions}
        for definitions in self.implicit.values():
            used_roots |= {self.webs.find(definition) for definition in definitions}
        for (statement_id, index), definition in self.defined.items():
            used_roots.add(self.webs.find(definition))
        by_register = {}
        for root, items in members.items():
            if root not in used_roots:
                continue    # nothing reads it (a result the function always writes before it reads it)
            register = root[2] if root[0] == "def" else root[1]
            by_register.setdefault(register, []).append((root, items))
        # the parameters' names first: the prototype has them as they are
        for register in sorted(by_register):
            if register in self.entry_registers and register in self.entry_names:
                self.unused(self.entry_names[register])
        ignored = {}    # by the register's width: a call writes the whole register through its pointer
        for register, webs in by_register.items():
            # the web with the parameter or result keeps the parameter's name; the rest by where they
            # start. Webs of one register never hold their values at the same time (each read is where the
            # register held what it reads), so webs can share a variable: the ones with the same name and
            # width, and the ones the function never reads
            webs.sort(key=lambda web: (not any(item == ("entry", register) for item in web[1])
                                       or register not in self.entry_registers, self.first_position(web[1])))
            own = self.entry_names.get(register) if register in self.entry_registers else None
            by_width = {}
            own_taken = False
            for root, items in webs:
                width = self.width(register, items)
                entry = any(item == ("entry", register) for item in items) and register in self.entry_registers
                if not entry and self.chosen_name(register, items) is None and self.ignored(items):
                    whole = cpu.current.register_bits.get(register, 32)
                    if whole not in ignored:
                        ignored[whole] = self.unused("ignored" if whole == 32 else f"ignored{whole}")
                    self.names[root] = ignored[whole]
                    self.widths[root] = whole
                    continue
                named = None if entry else self.chosen_name(register, items)
                if named is None and not entry and self.hint is not None:
                    named = self.hint(self, root, items)
                if entry or (named is not None and named == own):
                    # the parameter or result, and what is called like it: its variable
                    key = (width, "own")
                    if key not in by_width and own_taken:
                        key = (width, None, root)    # the parameter's name went to another width
                elif named is not None:
                    key = (width, named)
                else:
                    key = (width, None, root if self.split and any(item in self.read for item in items)
                           else "plain")
                if key not in by_width:
                    if key[1] == "own":
                        by_width[key] = own if own is not None else self.unused(cpu.variable_name(register))    # reserved above
                        own_taken = True
                    elif key[1] is None:
                        by_width[key] = self.unused(cpu.variable_name(register))
                    else:
                        by_width[key] = self.unused(named)
                self.names[root] = by_width[key]
                self.widths[root] = width

    def ignored(self, items):
        """True for a web nothing reads whose every value is what a call hands back without reading what
        the register held for anything but handing it back (its fresh outputs, and what it may leave as it
        was), or what a string move leaves: any variable can take it, so one does for all of them."""
        if any(item in self.read for item in items):
            return False
        for item in items:
            if item[0] == "entry" and item[1] not in self.entry_registers:
                continue    # what the register held when the function started, which no caller hands it
            if item[0] != "def":
                return False
            statement = self.statement_of(item[1])
            if isinstance(statement, ir.TailCall) and item[2] in set(self.end_reads(ir.Return())):
                return False    # what the function it goes on in hands back is this one's result
            if isinstance(statement, (ir.Call, ir.TailCall)):
                if item[2] not in self.new_values(statement) and not self.passes_only(statement, item[2]):
                    return False
            elif not (isinstance(statement, ir.Effect) and statement.name in ("move_string", "move_bits_down",
                                                                                 "move_bits_up", "search_bits_up")):
                return False
        return True

    def sites(self, register, items):
        """Where a web's values come from: (address of the instruction, or "entry", "rN", how) for each of
        its definitions, as "local" lines name them; 'how' is "entry", "set" (an assignment), "call" (what
        a call hands back) or "effect" (a string move, a division)."""
        found = []
        for item in items:
            if item[0] == "entry":
                found.append(("entry", f"r{register}", "entry"))    # as "local ... entry rN" lines name it
                continue
            statement = self.statement_of(item[1])
            origin = getattr(statement, "origin", None)
            if origin is not None:
                how = "set" if isinstance(statement, ir.SetReg) \
                    else "call" if isinstance(statement, (ir.Call, ir.TailCall)) else "effect"
                found.append((origin, f"r{item[2]}", how))
        return found

    def chosen_name(self, register, items):
        """The name symbols.txt gives the web, by any of its definitions, or None."""
        for spot, value, _ in self.sites(register, items):
            name = self.chosen.get((spot, value))
            if name is not None:
                return name
        return None

    def variable_sites(self):
        """{C name of a variable: the definitions of its webs} (decompile.py --variables)."""
        members = {}
        for item in list(self.webs.parent):
            members.setdefault(self.webs.find(item), []).append(item)
        found = {}
        for root, name in self.names.items():
            register = root[2] if root[0] == "def" else root[1]
            found.setdefault(name, []).extend(self.sites(register, members.get(root, [root])))
        return found

    def unused(self, name):
        """'name', or 'name_2', 'name_3'... if a variable of the function already has it."""
        taken = getattr(self, "_taken", None)
        if taken is None:
            taken = self._taken = set()
        found, number = name, 1
        while found in taken:
            number += 1
            found = f"{name}_{number}"
        taken.add(found)
        return found

    def statement_of(self, statement_id):
        if not hasattr(self, "_by_id"):
            self._by_id = {id(statement): statement for statement in self.all_statements()}
            for block in self.function.blocks.values():
                self._by_id[id(block.end)] = block.end
        return self._by_id.get(statement_id)

    def first_position(self, items):
        order = {id(statement): position for position, statement in enumerate(self.all_statements())}
        positions = [order.get(item[1], 1 << 30) for item in items if item[0] == "def"]
        return min(positions) if positions else -1

    def all_statements(self):
        if not hasattr(self, "_statements"):
            self._statements = [statement for block in self.function.blocks.values() for statement in block.statements]
        return self._statements

    def width(self, register, items):
        """Bits of the web anyone reads: 8, 16 or 32, at most the register's own. A variable a call or an
        effect writes (through a pointer, or as a return value) keeps the whole register."""
        whole = cpu.current.register_bits.get(register, 32)
        if any(item in self.through_pointer for item in items):
            return whole
        parts = set()
        for item in items:
            if item[0] == "entry":
                live = self.live_in
            else:
                live = self.after.get(item[1])
                if live is None:
                    return whole
            parts |= {variable[2] for variable in live if variable[0] == "r" and variable[1] == register}
        if 2 in parts:
            return min(32, whole)
        if 1 in parts:
            return min(16, whole)
        return 8

    # ---- what the writer asks ----

    def name_of(self, definitions, register):
        if not definitions:
            return cpu.variable_name(register)
        root = self.webs.find(next(iter(definitions)))
        return self.names.get(root, cpu.variable_name(register))

    def use_name(self, node):
        definitions = self.uses.get(id(node))
        return self.name_of(definitions, node.index) if definitions is not None else None

    def implicit_name(self, statement, register):
        definitions = self.implicit.get((id(statement), register))
        return self.name_of(definitions, register) if definitions is not None else None

    def def_name(self, statement, register):
        definition = self.defined.get((id(statement), register))
        return self.name_of([definition], register) if definition is not None else None

    def declarations(self):
        """(name, width) of every variable, the entry webs (parameters, results) included."""
        found = {}
        for root, name in self.names.items():
            found[name] = self.widths[root]
        return found

    def unchanged(self, statement, register):
        """True if 'statement' (a block end) reads the register as the function found it."""
        return self.implicit.get((id(statement), register)) == frozenset([("entry", register)])

    def entry_live(self, register):
        """True if the function reads what the register held when it started."""
        return any(variable[0] == "r" and variable[1] == register for variable in self.live_in)

    def entry_name(self, register):
        """The variable holding what the register held when the function started, or None if nothing
        reads that."""
        root = self.webs.find(("entry", register))
        return self.names.get(root)
