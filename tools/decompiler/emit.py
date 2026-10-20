"""
Name:        emit.py
Description: writes the decompiled functions as C: one C function per V60 function, registers as C
             variables, results through pointers
Version:     0.1.0
Created:     11-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

Every function becomes "void name(parameters)", or returns the one value it hands back. Its parameters
are the values it takes (signatures.py), by value, then a pointer for each result: the function copies
the caller's value in when it starts and out when it returns, so a result it leaves alone on some path
keeps the caller's value, as the register did. Parameters and results take the names symbols.txt gives
them ("param" lines), and the callers' variables that go in or come back take the same names. The
context registers are global variables; the other registers, the flags, the stack slots and the
temporaries are local variables (variables.py), named for what they hold where something says it. A
function that reads its own return address (it hands it to a new object as the code to run next) takes
it as a parameter, and every call passes the address after the call, which is a constant. Calls
through memory slots and interrupts go through call_code(), which finds the C function for a V60
address in a table (game/support.h); a call through a table of code is a switch on the index, one call
per entry, and an entry that is only a jump to another function calls that one.

Memory is named as much as symbols.txt allows: variables, tables, fields of the game's structures (with
the family views of each object type), board registers; an access that covers several of them at once
is written per field where it means the same (a constant stored, the same fields copied, a test that
they are all 0), and bit operations on part of a field on the field that holds them. Addresses the code
uses as values print as NAME_ADDRESS, TEXT_CELL(column, row) or a place in one of the board's memories.
The control flow is the tree of structure.py; every value is computed at its V60 width, with the casts
that make the C exact on any compiler.
"""
import re

from v60 import ir

from . import cpu
from . import cprint
from . import dataflow
from . import memory
from . import pointers
from . import structure
from . import variables

class EmitError(Exception):
    pass


def register_name(index):
    return cpu.current.register_names.get(index, f"r{index}")


def flag_name(flag):
    return cpu.current.flag_names[flag]


def register_bits(index):
    return cpu.current.register_bits.get(index, 32)
PSW_INTERRUPTS = 1 << 18    # the processor status bit that lets interrupts in
# words of C and of the runtime (game/support.h) a local variable must not be called
GLOBAL_WORDS = set("""auto break case char const continue default do double else enum extern float for goto if
inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void
volatile while bool true false main psw ram rom entry_point return_address carry zero negative overflow
LINKED OBJECT PLAYER RAM8 RAM16 RAM32 ROM8 ROM16 ROM32 AS8 AS16 AS32 AT8 AT16 AT32 s32_board game_code
interrupts_on interrupts_off divide_s8 divide_s16 divide_s32 divide_u8 divide_u16 divide_u32 remainder_s8
remainder_s16 remainder_s32 remainder_u8 remainder_u16 remainder_u32 shift_by shift_signed_by rotate_by
rotate_with_carry_by byte_swap32 mem_read8 mem_read16 mem_read32 mem_write8 mem_write16 mem_write32
call_code unknown_code invalid_jump invalid_operand frame_wait take_interrupts frame_end halt move_string game_trace game_trace_block
GAME_VARIABLES PLAYER_BLOCK WORK_RAM VIDEO_RAM SPRITE_RAM SPRITE_CONTROL PALETTE_RAM MIXER_REGISTERS SOUND_RAM IO_PORTS
INTERRUPT_CONTROL RANDOM_GENERATOR TEXT_LAYER TEXT_CELL RAM8S RAM16S RAM32S ROM8S ROM16S ROM32S move_bytes move_halfwords move_bytes_backward
move_halfwords_backward set_bytes set_halfwords move_bits_down move_bits_up search_bits_up divx_pair divux_pair""".split())


# the board's memories, as game/support.h names them (addresses the code computes into them)
REGIONS = [(0x300000, 0x320000, "VIDEO_RAM"), (0x400000, 0x420000, "SPRITE_RAM"),
           (0x500000, 0x500010, "SPRITE_CONTROL"), (0x600000, 0x610000, "PALETTE_RAM"),
           (0x610000, 0x620000, "MIXER_REGISTERS"), (0x700000, 0x702000, "SOUND_RAM"), (0xC00000, 0xC00080, "IO_PORTS"),
           (0xD00000, 0xD00010, "INTERRUPT_CONTROL"), (0xD80000, 0xD80010, "RANDOM_GENERATOR")]
BOARD_MEMORIES = {0x200000} | {start for start, _, _ in REGIONS}    # where the board's memories start


def value_name(value):
    """C name of a signature value: ("r", n) or ("f", name)."""
    return register_name(value[1]) if value[0] == "r" else flag_name(value[1])


def ordered(values):
    return sorted(values, key=lambda value: (0, value[1]) if value[0] == "r" else (1, cpu.current.flags.index(value[1])))


def label_name(label):
    """L_0012AB for a block at that address; L_start_002FF1, L_exit_00C755_2... for the ones the
    decompiler makes."""
    if isinstance(label, tuple):
        return "L_" + "_".join(label_name(part)[2:] if isinstance(part, tuple)
                               else f"{part:06X}" if isinstance(part, int) else str(part) for part in label)
    return f"L_{label:06X}"


def variable_order(name):
    """r2 before r2_2 before r10, then the ones named for what they hold."""
    register, _, number = name[1:].partition("_")
    if name.startswith("r") and register.isdigit() and (not number or number.isdigit()):
        return 0, int(register), int(number or 1), ""
    return 1, 0, 0, name


def label_order(label):
    return (1, label[1], label[0]) if isinstance(label, tuple) else (0, label, "")


def reads_return_address(function):
    for block in function.blocks.values():
        for statement in block.statements + [block.end]:
            for expression in ir.statement_expressions(statement):
                for node in ir.walk(expression):
                    if isinstance(node, ir.Stack) and node.offset == 0:
                        return True
    return False


def stride_of(offset):
    """(index, stride) when 'offset' is index * stride: x + x * 2 is 3x, x + x is 2x."""
    if isinstance(offset, ir.Op):
        name, args = offset.name, offset.args
        if name == "add" and args[0] == args[1]:
            return args[0], 2
        if name == "add" and isinstance(args[1], ir.Op) and args[1].name == "mul" and args[1].args[0] == args[0] \
                and isinstance(args[1].args[1], ir.Const):
            return args[0], args[1].args[1].value + 1
        if name == "mul" and isinstance(args[1], ir.Const):
            return args[0], args[1].value
        if name == "shl" and isinstance(args[1], ir.Const):
            return args[0], 1 << args[1].value
    return None


def switch_index(value, keys, read32):
    """For a jump through a table: (the index the table is read with, {case address: [index numbers]}),
    or None. Two kinds of table: code at base + index * stride (a run of branches), and code addresses
    read from base + index * 4 (read32 reads the program ROM)."""
    if isinstance(value, ir.Op) and value.name == "add":
        left, right = value.args
        base, offset = (left, right) if isinstance(left, ir.Const) else (right, left)
        if isinstance(base, ir.Const):
            found = stride_of(offset)
            if found is not None:
                index, stride = found
                numbers = {}
                for key in keys:
                    distance = key - (base.value & 0xFFFFFF)
                    if distance < 0 or distance % stride:
                        return None
                    numbers[key] = [distance // stride]
                return index, numbers
    if isinstance(value, ir.Load) and value.bits == 32:
        address = value.address
        if isinstance(address, ir.Op) and address.name == "add":
            left, right = address.args
            base, offset = (left, right) if isinstance(left, ir.Const) else (right, left)
            found = stride_of(offset) if isinstance(base, ir.Const) else None
            if found is not None and found[1] == 4:
                index, _ = found
                numbers = {key: [] for key in keys}
                for number in range(1024):
                    target = read32((base.value & 0xFFFFFF) + number * 4) & 0xFFFFFF
                    if target not in numbers:
                        break
                    numbers[target].append(number)
                if all(numbers.values()):
                    return index, numbers
    return None


def offset_switch(value, keys):
    """For code at base + offset with an offset that is no index times a stride: (the offset, {case address:
    its offset}), or None."""
    if isinstance(value, ir.Op) and value.name == "add":
        left, right = value.args
        base, offset = (left, right) if isinstance(left, ir.Const) else (right, left)
        if isinstance(base, ir.Const) and not isinstance(offset, ir.Const):
            start = base.value & 0xFFFFFF
            if all(key >= start for key in keys):
                return offset, {key: key - start for key in keys}
    return None


class Names:
    """How the printer spells the variables of one function."""

    def __init__(self, writer):
        self.writer = writer

    def register(self, index):
        return self.writer.use(("r", index))

    def register_node(self, node):
        """(C name, bits of the variable) for a register an expression reads."""
        return self.writer.register_read(node)

    def flag(self, name):
        return self.writer.use(("f", name))

    def control(self, name):
        return self.writer.control(name)

    def temp(self, temp):
        return self.writer.temp_name(temp), temp.bits

    def slot(self, node):
        return self.writer.slot_name(node), (32 if node.offset == 0 else self.writer.slot_bits(node))

    def link(self, load):
        """True if 'load' reads a field or variable that holds the low half of an object's address."""
        field = self.writer.fields.get(id(load))
        if field is not None and field[0] in pointers.STRUCTS:
            family = self.writer.field_family(field)
            found = pointers.SCOPED_TARGETS.get((field[0], family, field[1])) if family is not None else None
            if found is None:
                found = pointers.TARGETS.get((field[0], field[1]))
        elif isinstance(load.address, ir.Const):
            found = pointers.TARGETS.get(("global", load.address.value & 0xFFFFFF))
        else:
            found = None
        return found is not None and found[1] == 16

    def memory(self, load):
        """(C text, signed) for memory with a name, or None."""
        found = self.writer.memory(load.address, load.bits, id(load))
        return found[:2] if found is not None else None

    def board_register(self, address):
        """Name of the board register at a constant address, or None."""
        return self.writer.emitter.memory.register(address)


class Printer(cprint.Printer):
    def __init__(self, writer):
        super().__init__(Names(writer))
        self.writer = writer

    def constant_name(self, node):
        return self.writer.rom_address_name(node)

    def emit(self, node):
        if isinstance(node, ir.Op) and node.name == "stack_address":
            raise EmitError(f"{self.writer.function.name}: a stack address is used as a value")
        if isinstance(node, ir.EntryPoint):
            return cprint.Value("entry_point", cprint.PRIMARY, True, "u32")
        if isinstance(node, ir.Op) and node.name in ("eq", "ne") and node.args[1] == ir.Const(0, node.args[1].bits):
            test = self.writer.bit_test(node.args[0]) or self.writer.all_zero(node.args[0])
            if test is not None:
                return cprint.Value(f"({test}) {'==' if node.name == 'eq' else '!='} 0u", cprint.EQUALITY, True, "int")
        if isinstance(node, ir.Const) and node.bits == cpu.current.address_bits and id(node) not in self.writer.numbers():
            if node.value in self.writer.emitter.code_addresses and id(node) not in self.writer.sum_operands():
                # the address of code the program stores in a slot (an object's handler): its name
                return cprint.Value(self.writer.emitter.code_addresses[node.value], cprint.PRIMARY, True, "u32")
            named = self.writer.emitter.address_text(node.value)
            if named is not None:
                return cprint.Value(named[0], named[1], True, "u32")
            if 0x10000 <= node.value < 0x200000 and node.value & 0xFFFF and id(node) not in self.writer.sum_operands() \
                    or 0x100000 <= node.value < 0x200000 and node.value & 0xFFFF:
                # an address in the ROM nothing names yet
                self.writer.emitter.raw_addresses.setdefault(node.value, set()).add(self.writer.function.name)
        return super().emit(node)


SELF_ASSIGNMENT = re.compile(r"([A-Za-z_][A-Za-z0-9_]*) = \1;")
REPEAT_MINIMUM = 8    # a group of statements written this many times in a row or more becomes a loop (rerolled)


def rerolled(lines):
    """Runs of one group of simple statements written again and again (the sound driver's 256 LDI in a row)
    as a counted loop: the same statements, run the same number of times."""
    def simple(line):
        return line.endswith(";") and not line.startswith(("return", "goto", "break", "continue", "for ", "if "))
    found = []
    position = 0
    while position < len(lines):
        best = None
        for size in range(1, 9):
            group = lines[position:position + size]
            if len(group) < size or not all(simple(line) for line in group):
                break
            count = 1
            while lines[position + count * size:position + (count + 1) * size] == group:
                count += 1
            if count >= REPEAT_MINIMUM and (best is None or count * size > best[0] * best[1]):
                best = (count, size)
        if best is None:
            found.append(lines[position])
            position += 1
            continue
        count, size = best
        found += [f"for (uint32_t repeat = 0; repeat < {count}u; repeat++)", "{"]
        found += ["\t" + line for line in lines[position:position + size]] + ["}"]
        position += count * size
    return found


def shared_results(texts):
    """For the C of every function ('texts': its text by anything), the result pointers of each function
    that some call of it gives the same variable as another one ("wait_for_frame(&ignored, &ignored, ...)",
    or a pointer it was itself given twice): {function name: {"p_released", ...}}."""
    params = {}
    for text in texts:
        for match in re.finditer(r"^\w[\w ]*?\b(\w+)\(([^)\n]*)\)\n\{", text, re.M):
            params[match.group(1)] = [part.split()[-1].lstrip("*") for part in match.group(2).split(", ")
                                      if part != "void"]
    shared = {}
    for text in texts:
        for match in re.finditer(r"\b(\w+)\(", text):
            names = params.get(match.group(1))
            if not names:
                continue
            arguments, depth, start, index = [], 0, match.end(), match.end()
            while index < len(text):
                char = text[index]
                if char == "(":
                    depth += 1
                elif char == ")":
                    if depth == 0:
                        arguments.append(text[start:index].strip())
                        break
                    depth -= 1
                elif char == "," and depth == 0:
                    arguments.append(text[start:index].strip())
                    start = index + 1
                index += 1
            if len(arguments) != len(names):
                continue    # (the definition itself, or a prototype)
            pointers = [argument for argument in arguments if re.fullmatch(r"&\w+|p_\w+", argument)]
            for argument, name in zip(arguments, names):
                if argument in pointers and pointers.count(argument) > 1 and name.startswith("p_"):
                    shared.setdefault(match.group(1), set()).add(name)
    return shared


def through_pointer(line, name, param):
    """'line' with the variable 'name' read and set through the result pointer p_'param': &name is the
    pointer, name is *p_param ((*p_param)++, (*p_param)->...)."""
    def replace(match):
        if match.group(1):
            return f"p_{param}"
        after = line[match.end():]
        text = f"(*p_{param})" if after.startswith(("++", "--", "[", "->", ".")) else f"*p_{param}"
        return " " + text if line[:match.start()].endswith(("/", "*")) else text
    return re.sub(rf"(&)?(?<![\w.>]){name}\b", replace, line)


def comment_lines(text, width=105):
    """'text' as a /* */ block of lines at most 'width' wide, the lines after the first indented to its text."""
    lines, line = [], "/*"
    for word in text.split():
        if len(line) + 1 + len(word) > width and line.strip() not in ("/*", ""):
            lines.append(line)
            line = "  "
        line += " " + word
    lines.append(line + " */")
    return lines


CONTROL_MARK = "\x01"    # around a control variable's name while the body is written (name_controls)
CONTROL_PATTERN = "\x01([^\x01]*)\x01"


class FunctionWriter:
    def __init__(self, emitter, function, after, fields=None, live_in=frozenset()):
        self.emitter = emitter
        self.fields = fields or {}       # pointers.py: memory accesses that are fields of a structure
        self.function = function
        self.entry = function.entry
        self.signature = emitter.signatures[function.entry]
        self.after = after
        self.printer = Printer(self)
        self.temps = {}
        self.temp_bits = {}
        self.slots = {}
        self.slot_widths = {}
        self.signed_pieces = {}    # C of the fields and variables tiling() found -> signed
        self.used = set()
        self.referenced = set()
        self.extra = 0
        self.controls = {}         # control variable of gotos.py -> its C name (control)
        self.forwarded = set()     # variables of results whose caller's pointer goes straight on (forward_results)
        self.not_copied = set()    # results some way out of the function does not copy back (copy_out)
        self.written = None        # the body as the tree gives it, before forward_results (write)
        self.returned = emitter.returned(function.entry)
        self.results = emitter.by_pointer(function.entry)
        self.params = emitter.by_value(function.entry)
        self.has_results = bool(self.results) or self.returned is not None
        self.current = None
        entry_registers = {value[1] for value in self.params + self.results if value[0] == "r"}
        # the object family this function belongs to (symbols.txt scoped fields), and where the registers
        # still point at the object its code runs on
        self.family = emitter.family_of(function.entry, "object")
        self.global_family = emitter.global_family_of(function.entry)
        self.own = self.own_object()
        entry_names = {value[1]: emitter.param_name(function.entry, value)
                       for value in self.params + self.results if value[0] == "r"}
        # symbols.txt's names for locals, by the instruction that writes them ("local" lines)
        self.chosen = {(spot, value): name for (entry, spot, value), name in emitter.local_names.items()
                       if entry == function.entry and emitter.local_name(name) is not None}
        self.chosen.update(self.emitter.local_sites_free())
        self.vars = variables.Variables(function, emitter.context, self.call_registers, self.end_registers,
                                        entry_registers, after, live_in, self.local_hint,
                                        {register: name for register, name in entry_names.items()
                                         if name != register_name(register)},
                                        {key: name for key, name in self.chosen.items() if key[1][0] == "r"},
                                        self.really_reads, self.fresh_outputs, emitter.split_webs,
                                        self.passes_only, self.reads_input)
        self.widths = self.vars.declarations()
        self.handled = self.handler_families()
        emitter.param_widths[function.entry] = {
            value[1]: self.widths.get(self.vars.entry_name(value[1]) or "", 32)
            for value in self.params if value[0] == "r"}

    def statement_reading(self, node):
        """id of the statement or block end whose expressions hold 'node' (the one being written when the
        node is in none)."""
        if not hasattr(self, "_readers"):
            self._readers = {}
            for block in self.function.blocks.values():
                for statement in block.statements + [block.end]:
                    for expression in ir.statement_expressions(statement):
                        for inner in ir.walk(expression):
                            self._readers.setdefault(id(inner), id(statement))
        return self._readers.get(id(node), id(self.current))

    def handler_families(self):
        """{definition: family} for the objects this function gives a handler of some family (a spawner
        setting up the object it made): their fields are that family's. An object is what one assignment
        puts in a register (variables.py's definitions, and context_reaching's for the registers the whole
        program shares: the new object a spawner makes current_object), so that one variable may hold a saw
        blade and then the sparks it throws. A handler of no family says nothing (group_leader_wait after
        group_leader_move); handlers of two families, no family."""
        conflict = object()
        found = {}
        for block in self.function.blocks.values():
            for statement in block.statements:
                hit = self.fields.get(id(statement))
                if not isinstance(statement, ir.Store) or hit is None or hit[:2] != ("object", 0x1C) \
                        or not isinstance(hit[2], ir.Reg) or hit[3] or not isinstance(statement.value, ir.Const):
                    continue
                target = self.emitter.names.get(statement.value.value & 0xFFFFFF)
                family = self.emitter.family_of(statement.value.value & 0xFFFFFF, "object") if target else None
                if family is None:
                    continue
                for definition in self.reaching(hit[2]):
                    found[definition] = family if found.get(definition, family) == family else conflict
        return {definition: family for definition, family in found.items() if family is not conflict}

    def reaching(self, node):
        """The definitions that reach register node 'node': variables.py's, or context_reaching's for a
        register the whole program shares."""
        if node.index in self.emitter.context:
            return self.context_reaching().get(id(node), frozenset())
        return self.vars.uses.get(id(node), frozenset()) if hasattr(self, "vars") else frozenset()

    def context_reaching(self):
        """{id(register node): the definitions that reach it} for the registers the whole program shares
        (global variables in C, which variables.py leaves alone): assignments, calls that may change the
        register, ("entry", n)."""
        if hasattr(self, "_context_reaching"):
            return self._context_reaching
        context = set(self.emitter.context)
        uses = {}

        def step(statement, state, record):
            if record:
                for expression in ir.statement_expressions(statement):
                    for node in ir.walk(expression):
                        if isinstance(node, ir.Reg) and node.index in context:
                            uses[id(node)] = state.get(node.index, frozenset())
            if isinstance(statement, ir.SetReg) and statement.index in context:
                state[statement.index] = frozenset([("def", id(statement))])
            elif isinstance(statement, (ir.Call, ir.Effect)):
                for variable in self.emitter.flow.statement_writes(statement):
                    if variable[0] == "r" and variable[1] in context:
                        state[variable[1]] = frozenset([("def", id(statement))])

        blocks = self.function.blocks
        start = {self.function.entry: {index: frozenset([("entry", index)]) for index in context}}
        pending = [self.function.entry]
        while pending:
            label = pending.pop()
            state = dict(start[label])
            for statement in blocks[label].statements:
                step(statement, state, False)
            for successor in blocks[label].successors():
                old = start.get(successor)
                merged = dict(state) if old is None else \
                    {index: old.get(index, frozenset()) | state.get(index, frozenset()) for index in context}
                if merged != old:
                    start[successor] = merged
                    pending.append(successor)
        for label, state in start.items():
            state = dict(state)
            for statement in blocks[label].statements + [blocks[label].end]:
                step(statement, state, True)
        self._context_reaching = uses
        return uses

    def field_family(self, field, statement=None):
        """The family whose scoped fields an access through a pointer reads ('field': pointers.py's note of
        the access): the function's own for its object or the family of the handler it gives the object
        (family_through), else the family symbols.txt says the pointer leads to (object.eggman_scene); None
        if none."""
        struct, _, base, base_offset, family = field
        found = self.family_through(base, statement) if not base_offset and struct == "object" else None
        return found if found is not None else family

    def family_through(self, base, statement=None):
        """The family whose fields an access through register node 'base' (in 'statement', the one being
        written by default) reads: the function's own for its object, or the family of the handler the
        function gives the object; None if none."""
        if not isinstance(base, ir.Reg):
            return None
        # the statement the register is read in (a condition is written apart from the block it ends)
        where = id(statement) if statement is not None else self.statement_reading(base)
        if self.family is not None and base.index in self.own.get(where, frozenset()):
            return self.family
        if not getattr(self, "handled", None):
            return None
        # the family of the handler given to the object every definition that reaches here puts there
        families = {self.handled.get(definition) for definition in self.reaching(base)}
        return families.pop() if len(families) == 1 else None

    # ---- registers: the variables of variables.py ----

    def call_registers(self, statement):
        """(registers a call or tail call reads as arguments, registers it hands back) in C."""
        if isinstance(statement, ir.TailCall):
            targets = [statement.target]
        elif statement.slot is not None:
            return (), ()
        elif statement.target is not None:
            targets = [statement.target & 0xFFFFFF]
        else:
            targets = list(statement.choices)
        arguments, handed = set(), set()
        for target in targets:
            target = self.emitter.resolve(target)[0]
            if target not in self.emitter.signatures:
                continue
            arguments |= {value[1] for value in self.emitter.by_value(target) if value[0] == "r"}
            handed |= {value[1] for value in self.emitter.by_pointer(target) if value[0] == "r"}
            returned = self.emitter.returned(target)
            if returned is not None and returned[0] == "r":
                handed.add(returned[1])
        return arguments, handed

    def fresh_outputs(self, statement):
        """The registers a call hands back (through pointers, or as the value it returns) that every function
        it may call writes whole on every way back and never reads: what the caller's variable held does
        not go in."""
        if isinstance(statement, ir.TailCall):
            targets = [statement.target]
        elif statement.slot is not None:
            return set()
        else:
            targets = [statement.target & 0xFFFFFF] if statement.target is not None else list(statement.choices)
        found = None
        for target in targets:
            target = self.emitter.resolve(target)[0]
            signature = self.emitter.signatures.get(target)
            summary = self.emitter.flow.summaries.get(target)
            if signature is None or summary is None:
                return set()
            zeroed = self.emitter.flow.zeroed.get(target, set())
            values = list(self.emitter.by_pointer(target))
            returned = self.emitter.returned(target)
            if returned is not None and returned not in self.emitter.by_value(target):
                values.append(returned)
            fresh = {value[1] for value in values
                     if value[0] == "r" and value not in signature.params and value not in zeroed
                     and not any(variable[0] == "r" and variable[1] == value[1] for variable in summary.reads)
                     and all(("r", value[1], part) in summary.always for part in (0, 1, 2))}
            found = fresh if found is None else found & fresh
        return found or set()

    def reads_itself(self, target, register):
        """True if a function reads the register for what it does: for what it writes to memory or calls
        (its data flow summary's base, what it reads with nothing live where it ends) or for the other
        values it hands back (their passes), not only for what it hands back in the register itself
        (allocate_object leaves R11 as it found it when no object is free)."""
        summary = self.emitter.flow.summaries.get(self.emitter.resolve(target)[0])
        if summary is None:
            return True
        reads = set(summary.base)
        for key, found in summary.passes.items():
            if key != ("r", register):
                reads |= found
        return any(variable[0] == "r" and variable[1] == register for variable in reads)

    def passes_only(self, statement, register):
        """True if a call hands the register back and every function it may call reads what it held only
        to hand it back as it was: not for what it does (reads_itself), not an input the C starts at 0."""
        if isinstance(statement, ir.TailCall):
            targets = [statement.target]
        elif statement.slot is not None:
            return False
        else:
            targets = [statement.target & 0xFFFFFF] if statement.target is not None else list(statement.choices)
        for target in targets:
            target = self.emitter.resolve(target)[0]
            signature = self.emitter.signatures.get(target)
            if signature is None or (("r", register) in signature.params and self.reads_itself(target, register)) \
                    or ("r", register) in self.emitter.flow.zeroed.get(target, set()):
                return False
        return bool(targets)

    def reads_input(self, statement, register):
        """True if what the caller's register held matters to a call that hands the register back: the
        callee reads it for itself, or some part of it this function reads afterwards may come back as it
        was (a callee that leaves it so on some way back, or one of the functions a computed call may go to
        that does not touch it): the register is live before the call, by the data flow's own rules."""
        flow = self.emitter.flow
        if isinstance(statement, ir.TailCall):
            # what is live after a tail call is what is live where the function ends
            if not hasattr(self, "_ending"):
                self._ending = {id(block.end): block for block in self.function.blocks.values()}
            block = self._ending.get(id(statement))
            live = self.after.get(("end", id(block))) if block is not None else None
            if live is None:
                return True
            _, _, always = flow.combine([statement.target])
            before = (set(live) - always) | flow.target_reads([statement.target], live)
        else:
            live = self.after.get(id(statement))
            if live is None:
                return True
            _, _, always = flow.call_effect(statement)
            before = (set(live) - always) | flow.call_reads(statement, live)
        return any(variable[0] == "r" and variable[1] == register for variable in before)

    def really_reads(self, statement, register):
        """True if 'statement' reads the register for what it does: a call or tail call that takes it as a
        parameter it reads for what it does (reads_itself), the function's end (or a tail call) handing it
        back as the function's result, an effect. A call that only hands the register back, where it may
        leave it as it was, does not."""
        if isinstance(statement, (ir.Return, ir.InterruptReturn)) or not isinstance(statement, (ir.Call, ir.TailCall)):
            return True
        if isinstance(statement, ir.TailCall) and register in self.end_registers(ir.Return()):
            return True
        if isinstance(statement, ir.TailCall):
            targets = [statement.target]
        elif statement.slot is not None:
            return False
        elif statement.target is not None:
            targets = [statement.target & 0xFFFFFF]
        else:
            targets = list(statement.choices)
        for target in targets:
            signature = self.emitter.signatures.get(self.emitter.resolve(target)[0])
            if signature is not None and ("r", register) in signature.params and self.reads_itself(target, register):
                return True
        return False

    def end_registers(self, end):
        """Registers a block end reads: the function's results, and a tail call's arguments and results."""
        own = {value[1] for value in self.results if value[0] == "r"}
        if self.returned is not None and self.returned[0] == "r":
            own.add(self.returned[1])
        if isinstance(end, ir.TailCall):
            arguments, handed = self.call_registers(end)
            return own | arguments | handed
        if isinstance(end, ir.Return):
            return own
        return set()

    def register_read(self, node):
        if node.index in self.emitter.context:
            return self.emitter.context_name(node.index), 32
        name = self.vars.use_name(node) or cpu.variable_name(node.index)
        return name, self.widths.get(name, 32)

    def register_value(self, index):
        """The variable a call or the end of the function reads for a register."""
        if index in self.emitter.context:
            return self.emitter.context_name(index)
        return self.vars.def_name(self.current, index) or self.vars.implicit_name(self.current, index) \
            or cpu.variable_name(index)

    def register_set(self, index):
        """The variable a statement writes for a register."""
        if index in self.emitter.context:
            return self.emitter.context_name(index)
        return self.vars.def_name(self.current, index) or cpu.variable_name(index)

    def implicit_value(self, index):
        """The variable a call reads as an argument (before the call writes anything)."""
        if index in self.emitter.context:
            return self.emitter.context_name(index)
        return self.vars.implicit_name(self.current, index) or cpu.variable_name(index)

    # ---- names ----

    def use(self, value):
        """C name of a register or flag, noting it as used."""
        if value[0] == "r" and value[1] in self.emitter.context:
            return self.emitter.context_name(value[1])
        self.used.add(value)
        return value_name(value)

    def temp_name(self, temp):
        if temp.name not in self.temps:
            named = self.chosen_temp(temp)
            if named is None:
                named = self.temp_hint(temp)
            if named is None:
                self.unnamed_temps = getattr(self, "unnamed_temps", 0) + 1
                named = f"t{self.unnamed_temps}"
            self.temps[temp.name] = named
            self.temp_bits[temp.name] = temp.bits
        return self.temps[temp.name]

    def chosen_temp(self, temp):
        """symbols.txt's name for a temporary (a "local" line with tN), when no other variable has it."""
        found = re.fullmatch(r"t([0-9A-F]{4,6})_(\d+)_\w+", temp.name)
        if found is None:
            return None
        named = self.chosen.get((int(found.group(1), 16), f"t{found.group(2)}"))
        if named is None or named in self.vars.declarations() or named in self.temps.values():
            return None
        return self.vars.unused(named)

    def variable_sites(self):
        """{C name of a local: where its values come from, as "local" lines name them}."""
        found = self.vars.variable_sites()
        for temp, name in self.temps.items():
            spot = re.fullmatch(r"t([0-9A-F]{4,6})_(\d+)_\w+", temp)
            if spot is not None:
                found.setdefault(name, []).append((int(spot.group(1), 16), f"t{spot.group(2)}", "temp"))
        # the stack slots: by the instructions that store to them ("s" and the slot's distance below the
        # function's start)
        for offset, name in self.slots.items():
            for statement in self.slot_stores(offset):
                origin = getattr(statement, "origin", None)
                if origin is not None:
                    found.setdefault(name, []).append((origin, f"s{-offset:X}", "set"))
        return found

    def slot_stores(self, offset):
        return [statement for block in self.function.blocks.values() for statement in block.statements
                if isinstance(statement, ir.SetStack) and statement.offset == offset]

    def slot_chosen(self, offset):
        """The name symbols.txt's "local" lines give a stack slot by an instruction that stores to it, or
        None."""
        for statement in self.slot_stores(offset):
            name = self.emitter.local_sites.get((getattr(statement, "origin", None), f"s{-offset:X}"))
            if name is not None:
                return self.vars.unused(name)
        return None

    def read_field_name(self, load, statement=None):
        """The name of the whole field 'load' reads (the family's name for it when the function's own
        object is read), or None."""
        hit = self.fields.get(id(load))
        if hit is None or hit[0] not in pointers.STRUCTS:
            return None
        layouts = self.emitter.layouts
        found = layouts.lookup(hit[0], hit[1])
        exact = found == (hit[1], load.bits)
        family = self.field_family(hit, statement)
        if family is not None:
            scoped = layouts.scoped_field(hit[0], family, hit[1])
            if scoped is not None:
                if scoped[2] == hit[1] and scoped[3] == load.bits:
                    return scoped[1]
                overlay = layouts.scoped_overlay(hit[0], family, hit[1], load.bits) if scoped[2] == hit[1] else None
                if overlay is not None:
                    return overlay[0]
                if not exact or (hit[1] - scoped[2]) * 8 + load.bits <= scoped[3]:
                    return None    # a part of the family's field (field_access)
        overlay = layouts.overlay(hit[0], hit[1], load.bits)
        if overlay is not None and not exact:
            return overlay[0]
        if found is None or found[0] != hit[1]:
            return None
        name = layouts.names[hit[0]][found[0]]
        return None if name.startswith(("unk_", "var_")) else name

    def temp_hint(self, temp):
        """A name for a temporary that holds a field it was read from (the value before the code changes
        it: "wait_timer = OBJECT(...)->wait_timer; OBJECT(...)->wait_timer = wait_timer - 1u")."""
        if not hasattr(self, "_temp_values"):
            self._temp_values = {statement.temp.name: (statement.value, statement)
                                 for block in self.function.blocks.values()
                                 for statement in block.statements if isinstance(statement, ir.SetTemp)}
        value, statement = self._temp_values.get(temp.name, (None, None))
        while isinstance(value, ir.Op) and value.name in ("zext", "fits", "trunc", "sext"):
            value = value.args[0]
        name = self.read_field_name(value, statement) if isinstance(value, ir.Load) else None
        if name is None or self.emitter.local_name(name) is None:
            return None
        return self.vars.unused(name)

    def slot_hint(self, offset):
        """saved_NAME for a stack slot every store of which saves the same named thing (a context register,
        a variable, a field of the function's object), to put it back later; None otherwise."""
        saved = set()
        for block in self.function.blocks.values():
            for statement in block.statements:
                if not isinstance(statement, ir.SetStack) or statement.offset != offset:
                    continue
                value = statement.value
                while isinstance(value, ir.Op) and value.name in ("zext", "trunc", "fits"):
                    value = value.args[0]
                if isinstance(value, ir.Reg) and value.index in self.emitter.context:
                    saved.add(self.emitter.context_name(value.index))
                elif isinstance(value, ir.Load):
                    writing = self.current
                    self.current = statement
                    try:
                        found = self.memory(value.address, value.bits, id(value))
                    finally:
                        self.current = writing
                    text = found[0] if found is not None else None
                    match = None if text is None else re.fullmatch(r"(?:\w+\(\w+\)->(?:\w+\.)?)?(\w+)", text)
                    saved.add(match.group(1) if match else None)
                elif isinstance(value, ir.Reg) and self.emitter.saved_locals:
                    # a local variable pushed to get it back later (the Z80's PUSH HL ... POP HL)
                    writing = self.current
                    self.current = statement
                    try:
                        saved.add(self.register_read(value)[0])
                    finally:
                        self.current = writing
                else:
                    saved.add(None)
        if len(saved) != 1 or None in saved:
            return None
        name = self.vars.unused("saved_" + saved.pop())
        return self.emitter.local_name(name)

    def slot_name(self, node):
        if node.offset == 0:
            if self.entry not in self.emitter.needs_return_address:
                raise EmitError(f"{self.function.name}: reads its return address")
            return "return_address"
        if node.offset > 0:
            raise EmitError(f"{self.function.name}: reads its caller's stack at {node.offset:+d}")
        name = self.slots.get(node.offset) or self.slot_chosen(node.offset) or self.slot_hint(node.offset)             or f"stack_{-node.offset:X}"
        self.slots[node.offset] = name
        bits = self.slot_widths.setdefault(node.offset, node.bits)
        if bits != node.bits:
            raise EmitError(f"{self.function.name}: stack slot {node.offset} used at {bits} and {node.bits} bits")
        return name

    def slot_bits(self, node):
        return self.slot_widths.get(node.offset, node.bits)

    def memory(self, address, bits, key):
        """(C text, signed, an lvalue in work RAM) for an access of 'bits' at 'address' that has a name:
        a variable, a table, or a field of a structure the address points into ('key' finds the access
        in pointers.py's notes)."""
        if bits not in memory.TYPES:
            return None
        field = self.fields.get(key)
        if field is not None and field[0] in pointers.REGIONS:
            # through a pointer into the ROM: a table with a name, or the data read straight
            named = self.emitter.memory.access(address, bits, self.exact)
            if named is not None:
                return named
            return f"{memory.ROM_MACRO}{bits}({self.printer.any(field[2]).text})", False, False
        if field is not None and field[0] == "global":
            # through a pointer to a variable: as the variable
            own = self.own_variable(field[1], bits)
            return own if own is not None else self.emitter.memory.access(field[1], bits, self.exact)
        if field is not None:
            text, signed = self.field_access(field, bits)
            return text, signed, True
        value = memory.constant_address(address)
        if value is not None:
            own = self.own_variable(value, bits)
            if own is not None:
                return own
        own = self.own_element(address, bits)
        if own is not None:
            return own
        return self.emitter.memory.access(address, bits, self.exact)

    def own_element(self, address, bits):
        """(C, signed, True) for an element of a table in work RAM the function's family names its own way
        ("scoped global" of the elements' type), read or written by index; None for any other access."""
        items = self.emitter.memory.scoped.get(self.global_family) if self.global_family else None
        parts = memory.based(address) if items else None
        if parts is None:
            return None
        item = items.get(parts[0])
        index = memory.element_index(parts[1], bits)
        if item is None or not item.indexed or item.bits != bits or index is None:
            return None
        return f"{item.name}[{self.exact(index)}]", item.signed, True

    def own_variable(self, address, bits):
        """(C, signed, True) for work RAM the function's family names its own way ("scoped global"): the
        variable, or the part of it a narrower access reads (AS8(tornado_kind) for its low byte); None
        when no such variable holds the whole access."""
        items = self.emitter.memory.scoped.get(self.global_family) if self.global_family else None
        if not items:
            return None
        item = items.get(address)
        if item is not None and item.bits == bits:
            return (f"{item.name}[0]" if item.indexed else item.name), item.signed, True
        overlay = self.emitter.memory.scoped_overlays.get(self.global_family, {}).get((address, bits))
        if overlay is not None:
            return overlay.name, overlay.signed, True
        for start, item in items.items():
            if start <= address and address + bits // 8 <= start + item.bits // 8 and bits < item.bits:
                name = f"{item.name}[0]" if item.indexed else item.name
                return cprint.part(bits, name, address - start, item.bits), False, True
        return None

    def family_splits(self, statement):
        """True for a store through an object of a family whose own field at that place is narrower than
        the store: the family's fields say more than a name the structure has for all those bytes (the
        timer objects' started and step, not the flags' low half)."""
        field = self.fields.get(id(statement))
        if field is None or field[0] not in pointers.STRUCTS:
            return False
        family = self.field_family(field)
        scoped = self.emitter.layouts.scoped_field(field[0], family, field[1]) if family is not None else None
        return scoped is not None and scoped[2] == field[1] and scoped[3] < statement.bits

    def split_store(self, statement):
        """A store over several named fields or variables at once, as one assignment each, when it writes
        a constant or copies the same fields from elsewhere: AS16(OBJECT(o)->sprite_offset_x) = 0xA00u is
        sprite_offset_x = 0 and sprite_offset_y = 0xA. None when the store is not that."""
        pieces = self.tiling(statement.address, statement.bits, id(statement))
        if pieces is None or len(pieces) < 2:
            return None
        value = statement.value
        while isinstance(value, ir.Op) and value.name in ("trunc", "fits") and value.bits == statement.bits:
            value = value.args[0]
        if isinstance(value, ir.Const):
            lines = []
            for start, bits, text in pieces:
                part = (value.value >> (start * 8)) & ((1 << bits) - 1)
                signed = self.signed_pieces.get(text, False)
                lines.append(f"{text} = {cprint.signed_literal(part, bits) if signed else cprint.literal(part, bits)};")
            return lines
        if isinstance(value, ir.Load) and value.bits == statement.bits:
            sources = self.tiling(value.address, value.bits, id(value))
            if sources is not None and [piece[:2] for piece in sources] == [piece[:2] for piece in pieces]:
                return [f"{target} = {source};" for (_, _, target), (_, _, source) in zip(pieces, sources)]
        return None

    def tiling(self, address, bits, key):
        """The named fields or variables an access of 'bits' at 'address' covers exactly, one after the
        other: [(byte in the access, bits, C)], or None."""
        size = bits // 8
        pieces = []
        field = self.fields.get(key)
        if field is not None and field[0] in pointers.STRUCTS:
            struct, offset, base, base_offset, kind_family = field
            layouts = self.emitter.layouts
            family = self.field_family(field)
            position = offset
            while position < offset + size:
                scoped = layouts.scoped_field(struct, family, position) if family else None
                if scoped is not None:
                    if scoped[2] != position:
                        return None
                    piece = scoped[3]
                else:
                    piece = layouts.fields.get(struct, {}).get(position)
                if piece is None or position + piece // 8 > offset + size:
                    return None
                text, signed = self.field_access((struct, position, base, base_offset, kind_family), piece)
                self.signed_pieces[text] = signed
                pieces.append((position - offset, piece, text))
                position += piece // 8
            return pieces
        start = memory.constant_address(address)
        if field is not None or start is None:
            return None
        position = start
        while position < start + size:
            piece = self.variable_at(position)
            if piece is None or position + piece[1] // 8 > start + size or memory.bus_only(position, piece[1]):
                return None
            self.signed_pieces[piece[0]] = piece[2]
            pieces.append((position - start, piece[1], piece[0]))
            position += piece[1] // 8
        return pieces

    def variable_at(self, address):
        """(C, bits, signed) of the variable that starts at a work RAM address, or of the element of an
        array that does; None if none does."""
        variables = self.emitter.memory.variables
        scoped = self.emitter.memory.scoped.get(self.global_family) if self.global_family else None
        if scoped and address in scoped:
            item = scoped[address]
            return (f"{item.name}[0]" if item.indexed else item.name), item.bits, item.signed
        item = variables.get(address)
        if item is not None:
            return (f"{item.name}[0]" if item.indexed else item.name), item.bits, item.signed
        for start in range(address - 1, address - 0x40, -1):
            item = variables.get(start)
            if item is None:
                continue
            if item.indexed and (address - start) % (item.bits // 8) == 0:
                return f"{item.name}[{(address - start) // (item.bits // 8)}]", item.bits, item.signed
            return None
        return None

    def local_hint(self, variables, root, items):
        """A name for a local variable from what it holds: an object or a player the code reads fields
        through, else the field all its values are read from. None when nothing says."""
        if not hasattr(self, "_through"):
            # the structures each web points at, from the fields read and written through it
            self._through = {}
            for hit in self.fields.values():
                if hit[0] in pointers.STRUCTS and isinstance(hit[2], ir.Reg):
                    for definition in variables.uses.get(id(hit[2]), ()):
                        self._through.setdefault(variables.webs.find(definition), set()).add(hit[0])
        # a player's block set from its address: by the player's number (player_2), not the order
        first, size, count = pointers.PLAYERS
        blocks = set()
        for item in items:
            statement = variables.statement_of(item[1]) if item[0] == "def" else None
            if not isinstance(statement, ir.SetReg) or not isinstance(statement.value, ir.Const) \
                    or not first <= statement.value.value < first + size * count \
                    or (statement.value.value - first) % size:
                blocks = None
                break
            blocks.add((statement.value.value - first) // size + 1)
        if blocks is not None and len(blocks) == 1:
            return self.emitter.local_name(f"player_{blocks.pop()}")
        structs = self._through.get(root, set())
        if len(structs) == 1:
            return self.emitter.local_name(next(iter(structs)))
        sources = set()
        for item in items:
            if item[0] != "def":
                sources = None
                break
            statement = variables.statement_of(item[1])
            if not isinstance(statement, ir.SetReg):
                sources = None
                break
            value = statement.value
            while isinstance(value, ir.Op) and value.name in ("zext", "fits", "trunc", "sext"):
                value = value.args[0]
            name = self.read_field_name(value, statement) if isinstance(value, ir.Load) else None
            if name is None:
                sources = None
                break
            sources.add(name)
        if sources is not None and len(sources) == 1:
            return self.emitter.local_name(sources.pop())
        return self.passed_name(variables, root, items)

    def passed_name(self, variables, root, items):
        """The name of the parameter or result a web is passed as, or gets back from, in the calls that
        take it (symbols.txt), when they all have one and agree; also the function's own result it hands
        back. What a call hands back counts only where the function reads it, and a call that only hands
        the register back (it may leave it as it was) does not count at all: the dust offset R0 holds
        before queue_sprite_at_depth, which leaves a bucket head there that nothing reads, is no
        bucket_head."""
        if not hasattr(self, "_statement_ids"):
            self._statement_ids = {}
            for block in self.function.blocks.values():
                for statement in block.statements + [block.end]:
                    self._statement_ids[id(statement)] = statement
        found = set()
        register = root[2] if root[0] == "def" else root[1]
        definitions = set(items)
        # what the function itself does with the values says more than what it passes them on as: a web
        # its own expressions read takes no parameter's name (the tens digit build_ranking_display works
        # out is no piece_count because spawn_object_with_pieces finds it in R1 next)
        read_here = any(reaching & definitions for reaching in variables.uses.values())
        # and what it puts in the register itself, more than what a call hands back in it
        own_values = any(item[0] == "entry" or not isinstance(self._statement_ids.get(item[1]), (ir.Call, ir.TailCall))
                         for item in items)
        for (statement_id, index), reaching in variables.implicit.items():
            if index != register or not reaching & definitions:
                continue
            statement = self._statement_ids.get(statement_id)
            if not self.really_reads(statement, index):
                continue    # a call that only hands the register back says nothing about what it held
            if read_here and not isinstance(statement, ir.Return):
                continue
            found |= self.names_at(statement, index)
        left = set()    # what calls that do not read the register leave in it, beside the function's own values
        for item in items:
            if item[0] != "def":
                continue
            statement = self._statement_ids.get(item[1])
            if own_values and isinstance(statement, (ir.Call, ir.TailCall)) \
                    and not self.really_reads(statement, register):
                left |= self.names_at(statement, register)
                continue
            if item in variables.read and not own_values:
                # what a call hands back, when the function reads it
                found |= self.names_at(statement, register)
        # the function's own value, and what a call leaves in the same register (boulder_roll's sound number,
        # then whatever find_floor leaves in R0): no one name for both, unless the call calls it what the rest
        # does (check_hit hands back the previous object of the list check_hits walks, as check_hits does)
        if left and left != found:
            return None
        # a call that takes it without a name for it (a value only passed on, a leftover) says nothing
        # either: the name of one call must not spread through chains of leftovers
        if None in found or len(found) != 1:
            return None
        return self.emitter.local_name(found.pop())

    def names_at(self, statement, register):
        """Names symbols.txt gives 'register' where 'statement' passes it on: a call's parameters or
        results, a tail call's, and the function's own result at a return."""
        if isinstance(statement, ir.Return):
            if any(value[0] == "r" and value[1] == register for value in self.results) \
                    or (self.returned is not None and self.returned == ("r", register)):
                return {self.emitter.param_names.get((self.entry, register))}
            return set()
        if isinstance(statement, ir.TailCall):
            targets = [statement.target & 0xFFFFFF]
        elif isinstance(statement, ir.Call) and statement.slot is None:
            targets = [statement.target & 0xFFFFFF] if statement.target is not None else list(statement.choices)
        else:
            return set()
        return {self.emitter.param_names.get((self.emitter.resolve(target)[0], register)) for target in targets}

    def own_object(self):
        """{id(statement): registers that point at the object the function's code runs on, before it}: the
        object loop's R20 and R23 when the function starts, and copies of them."""
        blocks = self.function.blocks
        start = {self.function.entry: frozenset((20, 23))}
        pending = [self.function.entry]
        before = {}
        while pending:
            label = pending.pop()
            current = set(start[label])
            block = blocks[label]
            for statement in block.statements + [block.end]:
                before[id(statement)] = frozenset(current)
                if isinstance(statement, ir.SetReg):
                    value = statement.value
                    if isinstance(value, ir.Reg) and value.index in current:
                        current.add(statement.index)
                    else:
                        current.discard(statement.index)
                elif isinstance(statement, (ir.Call, ir.Effect)):
                    before_call = set(current)
                    current -= {variable[1] for variable in self.emitter.flow.statement_writes(statement)
                                if variable[0] == "r"}
                    restored = self.emitter.restores_current(statement.target & 0xFFFFFF) \
                        if isinstance(statement, ir.Call) and statement.target is not None else (None, None)
                    # the object loop's cursor as it was on some way out, and the current object set from it
                    # (current_object = object_cursor at the end) or left as it was
                    if restored[1] == "kept" and 23 in before_call:
                        current.add(23)
                        if restored[0] == "cursor" or (restored[0] == "mixed" and 20 in before_call):
                            current.add(20)
            for successor in block.successors():
                merged = frozenset(current) if successor not in start else start[successor] & frozenset(current)
                if merged != start.get(successor):
                    start[successor] = merged
                    pending.append(successor)
        return before

    def field_lvalue(self, field, bits):
        return self.field_access(field, bits)[0]

    def field_access(self, field, bits):
        """(C, signed) for an access of 'bits' to a field: by the name of the family's own field when the
        function's object is accessed, or the structure's; a name that holds exactly those bytes first."""
        struct, offset, base, base_offset, _ = field
        family = self.field_family(field)
        layouts = self.emitter.layouts
        if base_offset and isinstance(base, ir.Op) and base.name == "add" and isinstance(base.args[0], ir.Const):
            # a constant place in the structure plus the rest: the structure's own address plus the rest
            base = ir.op("add", (ir.Const((base.args[0].value - base_offset) & 0xFFFFFFFF), base.args[1]), 32)
            base_offset = 0
        start = self.printer.any(base).text
        pointer = f"{struct.upper()}({start})" if not base_offset else \
            f"{struct.upper()}({start} - 0x{base_offset:X}u)"
        found = layouts.lookup(struct, offset)
        if found is None:
            raise EmitError(f"{self.function.name}: no field of {struct} at {offset:X}")
        field_offset, field_bits = found
        name = f"{pointer}->{layouts.names[struct][field_offset]}"
        overlay = layouts.overlay(struct, offset, bits) if field_offset == offset else None
        if field_offset == offset and field_bits == bits:
            generic = name, layouts.signed[struct][offset], True
        elif overlay is not None and field_bits != bits:
            generic = f"{pointer}->{overlay[0]}", overlay[1], True
        else:
            generic = cprint.part(bits, name, offset - field_offset, field_bits), False, False
        if family is not None:
            scoped = layouts.scoped_field(struct, family, offset)
            if scoped is not None:
                view, scoped_name, scoped_offset, scoped_bits, scoped_signed = scoped
                text = f"{pointer}->{view}.{scoped_name}"
                if scoped_offset == offset and scoped_bits == bits:
                    if field_offset == offset and field_bits == bits and layouts.names[struct][offset] == scoped_name \
                            and layouts.signed[struct][offset] == scoped_signed:
                        return generic[:2]    # the structure's own field, which the family only says more about
                    return text, scoped_signed
                overlay = layouts.scoped_overlay(struct, family, offset, bits) if scoped_offset == offset else None
                if overlay is not None:
                    return f"{pointer}->{view}.{overlay[0]}", overlay[1]
                # bytes of the family's field: a part of it, whatever the structure calls them; an access
                # that reaches past it is the structure's if the structure has a name for all of it
                if generic[2] and (offset - scoped_offset) * 8 + bits > scoped_bits:
                    return generic[:2]
                return cprint.part(bits, text, offset - scoped_offset, scoped_bits), False
        self.note_missing(struct, offset, bits, base, base_offset, generic)
        return generic[:2]

    def note_missing(self, struct, offset, bits, base, base_offset, generic):
        """Notes an access to the object types' own variables (from where the families' views start) that
        no family names: through the function's own object, under its family (or the function's name
        when it has none) (decompile.py --fields)."""
        region = self.emitter.layouts.region(struct)
        if struct != "object" or base_offset or region is None or offset < region or not isinstance(base, ir.Reg):
            return
        if not generic[0].split("->")[-1].startswith(("var_", "unk_")) and generic[2]:
            return    # a generic field with a name and the access's width
        where = self.statement_reading(base)
        if base.index not in self.own.get(where, frozenset()):
            return
        key = (self.family or f"function {self.function.name}", offset, bits)
        self.emitter.missing_fields.setdefault(key, set()).add(self.function.name)

    def fresh(self, stem):
        self.extra += 1
        name = f"{stem}{self.extra}"
        self.temps[name] = name
        self.temp_bits[name] = 32
        return name

    def goto(self, label):
        self.referenced.add(label)
        return f"goto {self.emitter.label_text(label)};"

    def control_name(self, label):
        """The name of the control variable for the jumps to 'label' (gotos.py): the label's name without the
        words it starts with that the function's name has ("flood_fill_scan_row_found" in flood_fill_scan_row
        is found, "flood_fill_skip_parent_left_pairs" in flood_fill_scan_row_skip_parent left_pairs)."""
        words = self.emitter.label_text(label).split("_")
        own = set(self.function.name.split("_"))
        while len(words) > 1 and words[0] in own and words[1][:1].isalpha():
            words.pop(0)
        return "_".join(words)

    def control(self, name):
        """A control variable in the body being written: a mark that name_controls() replaces once the body
        shows every other name the function uses."""
        self.controls.setdefault(name, None)
        return CONTROL_MARK + name + CONTROL_MARK

    def forward_results(self, body):
        """'body' with the results the function only hands on: a register it takes and hands back by pointer
        whose variable the function itself never reads or sets, only passes to calls by pointer and copies back
        as they left it ("update(&count); ... *p_count = count; return;"), goes straight through: the caller's
        pointer is passed on (update(p_count)), with no copy of its own and nothing to copy back at each
        return. A result the ways out copy back from the variables of the register's values (and that comes in
        in its own), joined into one, that the function uses no more than twice for each copy back, is that
        variable: the function works on it through its pointer ("*p_count += 2u;"), with no copy in or back.
        A way out the register reaches unchanged copies nothing back either way. Neither is
        done for a result some call hands the same variable as another one (results nobody reads all go to
        the caller's 'ignored'): writing one through its pointer would change the other. The names of those
        variables go into self.forwarded (declarations leaves them out)."""
        self.forwarded = set()
        shared = self.emitter.shared_results.get(self.emitter.names.get(self.entry, self.function.name), set())
        for value in self.results:
            if value[0] != "r":
                continue
            param = self.emitter.param_name(self.entry, value)
            if f"p_{param}" in shared:
                continue
            entry = self.vars.entry_name(value[1]) if self.vars.entry_live(value[1]) else None
            if entry is not None:
                # copying back the value as it came in, where nothing else reads or sets it, does nothing
                same = rf"\t+\*p_{param} = {entry};"
                rest = [line for line in body if not re.fullmatch(same, line)]
                if len(rest) < len(body) and not any(re.search(rf"(?<![\w.>]){entry}\b", line) for line in rest):
                    body = rest
                    self.forwarded.add(entry)
                    continue
            copies = [line for line in body if re.fullmatch(rf"\t+\*p_{param} = [^;]*;", line)]
            sources = [re.fullmatch(rf"\t+\*p_{param} = (\w+);", line) for line in copies]
            if not copies or None in sources:
                continue    # (none, or a part of the register)
            sources = [found.group(1) for found in sources]
            # the variables of the values of the register the ways out hand back, and the one it comes in in,
            # are one variable (they never hold a value at the same time)
            names = sorted(set(sources) | ({entry} if entry is not None else set()), key=sources.count, reverse=True)
            name = entry if entry is not None else names[0]
            if any(self.widths.get(each, 32) != 32 or each in self.params for each in names):
                continue
            rest = [line for line in body if line not in copies]
            for other in names:
                if other != name:
                    rest = [re.sub(rf"(?<![\w.>]){other}\b", name, line) for line in rest]
            uses = sum(len(re.findall(rf"(?<![\w.>]){name}\b", line)) for line in rest)
            if uses == 0:
                continue
            pointed = sum(len(re.findall(rf"&{name}\b", line)) for line in rest)
            if uses == pointed:
                body = [re.sub(rf"&{name}\b", f"p_{param}", line) for line in rest]
            elif value not in self.not_copied and uses - pointed <= 2 * len(copies):
                # (a variable the function works on much more than it copies back keeps its own copy)
                body = [through_pointer(line, name, param) for line in rest]
            else:
                continue
            self.forwarded.update(names)
        return body

    def name_controls(self, body):
        """'body' with the control variables named: each its own name, apart from every other name the
        function and the program use."""
        if not self.controls:
            return body
        text = "\n".join([self.prototype()] + body)
        taken = set(re.findall(r"[A-Za-z_]\w*", re.sub(CONTROL_PATTERN, " ", text)))
        for name in sorted(self.controls):
            found, number = name, 2
            while found in taken or self.emitter.local_name(found) is None:
                found, number = f"{name}_{number}", number + 1
            self.controls[name] = found
            taken.add(found)
        return [re.sub(CONTROL_PATTERN, lambda match: self.controls[match.group(1)], line) for line in body]

    # ---- statements ----

    def expr(self, node):
        """C for a value that only needs to be right in its low bits (an assignment narrows it)."""
        return self.printer.any(node).text

    def exact(self, node):
        """C for a value that has to be exactly the IR value (an argument, a condition)."""
        return self.printer.exact(node).text

    def live_flags(self, statement):
        live = self.after.get(id(statement), set())
        return [flag for flag in cpu.current.flags if ("f", flag) in live]

    def flags_from(self, value, flags):
        shifts = {"z": 0, "s": 1, "ov": 2, "cy": 3}
        lines = []
        for flag in flags:
            shift = shifts[flag]
            source = value if shift == 0 else f"({value} >> {shift})"
            lines.append(f"{self.use(('f', flag))} = {source} & 1u;")
        return lines

    def statement(self, statement):
        self.current = statement
        if isinstance(statement, ir.SetTemp):
            # a temporary narrower than what it takes says the cut, as a store does (stored)
            value = self.exact(statement.value) if statement.temp.bits == 1 else \
                self.stored(statement.value, statement.temp.bits)
            return [f"{self.temp_name(statement.temp)} = {value};"]
        if isinstance(statement, ir.SetReg):
            if statement.index not in cpu.current.registers:
                raise EmitError(f"{self.function.name}: writes register {statement.index}")
            name = self.register_set(statement.index)
            changed = self.changed_in_place(statement, name)
            if changed is not None:
                return [changed]
            return [f"{name} = {self.narrowed(statement.value, self.widths.get(name, 32))};"]
        if isinstance(statement, ir.SetFlag):
            return [f"{self.use(('f', statement.name))} = {self.exact(statement.value)};"]
        if isinstance(statement, ir.SetStack):
            if statement.offset >= 0:
                raise EmitError(f"{self.function.name}: writes its caller's stack at {statement.offset:+d}")
            name = self.slot_name(ir.Stack(statement.offset, statement.bits))
            return [f"{name} = {self.stored(statement.value, statement.bits)};"]
        if isinstance(statement, ir.Store):
            value = statement.value
            target = self.memory(statement.address, statement.bits, id(statement))
            address = memory.constant_address(statement.address)
            if target is not None and target[2] and (address is None or not memory.bus_only(address, statement.bits)):
                if target[0].startswith("AS") or self.family_splits(statement):
                    split = self.split_store(statement)
                    if split is not None:
                        return split
                return [self.assignment(target[0], statement, target[1])]
            text = self.stored(value, statement.bits)
            where = self.emitter.memory.register(address) if address is not None else None
            return [f"mem_write{statement.bits}({where or self.expr(statement.address)}, {text});"]
        if isinstance(statement, ir.Effect):
            return self.effect(statement)
        if isinstance(statement, ir.Call):
            return self.call(statement)
        raise EmitError(f"{self.function.name}: cannot write {statement!r}")

    def stored(self, value, bits):
        """C for a value written to memory of 'bits' bits: a variable or a halfword that is wider than the
        destination (but narrower than an int, which converts silently) says the cut with a cast."""
        inner = value
        while isinstance(inner, ir.Op) and inner.name in ("zext", "fits", "trunc"):
            inner = inner.args[0]
        if isinstance(inner, ir.Reg) and inner.index not in self.emitter.context:
            width = self.widths.get(self.register_read(inner)[0], 32)
        elif isinstance(inner, ir.Load):
            width = inner.bits
        else:
            width = 32
        if bits < width < 32:
            return f"({cprint.UNSIGNED[bits]})" + self.printer.wrap(self.printer.any(value), cprint.UNARY)
        return self.expr(value)

    def narrowed(self, value, bits):
        """C for a value assigned to a variable of 'bits' bits: the bits above are dead, so a constant
        drops them and a wider value says the cut with a cast."""
        if bits >= 32:
            return self.expr(value)
        if isinstance(value, ir.Const):
            if bits == cpu.current.address_bits and value.bits == bits:
                return self.expr(value)    # a variable as wide as an address may hold one: named if it is
            return cprint.literal(value.value & ir.MASKS[bits], bits)
        inner = value
        while isinstance(inner, ir.Op) and inner.name in ("zext", "fits", "trunc"):
            if inner.name == "trunc" and inner.bits <= bits:
                return self.exact(inner)    # already cut to the variable's width or less
            inner = inner.args[0]
        if inner.bits <= bits:
            return self.expr(value)
        return f"({cprint.UNSIGNED[bits]})" + self.printer.wrap(self.printer.any(value), cprint.UNARY)

    def assignment(self, lvalue, statement, signed=False):
        """'lvalue = value;', or 'lvalue op= x;' when the value is the lvalue itself and something."""
        value = statement.value
        while isinstance(value, ir.Op) and value.name in ("trunc", "fits") and value.bits == statement.bits:
            value = value.args[0]
        if isinstance(value, ir.Op) and value.name in ("add", "sub", "and", "or", "xor") and value.bits == statement.bits:
            left, right = value.args

            def reads_target(node):
                return isinstance(node, ir.Load) and node.address == statement.address and node.bits == statement.bits
            if not reads_target(left) and value.name != "sub" and reads_target(right):
                left, right = right, left
            if reads_target(left):
                if lvalue.startswith(("AS", "AT")) and value.name in ("and", "or", "xor") and isinstance(right, ir.Const):
                    better = self.whole_bits(statement.address, statement.bits, id(statement), right.value,
                                             0xFF if value.name == "and" else 0)
                    if better is not None:
                        text, constant, bits = better
                        symbol = {"and": "&=", "or": "|=", "xor": "^="}[value.name]
                        return f"{text} {symbol} {cprint.literal(constant, bits)};"
                return self.compound(lvalue, value.name, right)
        constant = statement.value
        while isinstance(constant, ir.Op) and constant.name in ("trunc", "fits") and constant.bits == statement.bits:
            constant = constant.args[0]
        if signed and isinstance(constant, ir.Const) and statement.bits == 32 and self.rom_address_name(constant):
            return f"{lvalue} = {self.rom_address_name(constant)};"
        if signed and isinstance(constant, ir.Const):
            # a signed field or variable gets the number it holds: -4, not 0xFCu
            return f"{lvalue} = {cprint.signed_literal(constant.value & ((1 << statement.bits) - 1), statement.bits)};"
        return f"{lvalue} = {self.stored(statement.value, statement.bits)};"

    def all_zero(self, node):
        """'a | b' for a test that several named fields or variables read at once are all 0
        (AS16(floor_slope) == 0u is (floor_slope | floor_type) == 0u); None when it is not that."""
        while isinstance(node, ir.Op) and node.name in ("zext", "fits"):
            node = node.args[0]
        if not isinstance(node, ir.Load):
            return None
        found = self.memory(node.address, node.bits, id(node))
        if found is None or not found[0].startswith(("AS", "AT")):
            return None
        pieces = self.tiling(node.address, node.bits, id(node))
        if pieces is None or len(pieces) < 2:
            return None
        return " | ".join(text for _, _, text in pieces)

    def bit_test(self, node):
        """'x & mask' for a test of bits of a part of a named field or variable (AS8(flags) & 4u), on the
        whole field or variable that holds them (flags & 4u); None when it is not that."""
        if not isinstance(node, ir.Op) or node.name != "and":
            return None
        load, mask = node.args
        if isinstance(load, ir.Const):
            load, mask = mask, load
        while isinstance(load, ir.Op) and load.name in ("zext", "fits", "trunc"):
            load = load.args[0]
        if not isinstance(load, ir.Load) or not isinstance(mask, ir.Const):
            return None
        found = self.memory(load.address, load.bits, id(load))
        if found is None or not found[0].startswith(("AS", "AT")):
            return None
        better = self.whole_bits(load.address, load.bits, id(load), mask.value & ((1 << load.bits) - 1), 0)
        if better is None:
            return None
        text, constant, bits = better
        return f"{text} & {cprint.literal(constant, bits)}"

    def whole_bits(self, address, bits, key, constant, fill):
        """For an operation with 'constant' on the access of 'bits' at 'address': the named field or
        variable that holds every byte it changes or tests (a byte of the constant other than 'fill'),
        and the constant for it (its other bytes 'fill'): (C, constant, bits), or None."""
        size = bits // 8
        touched = [index for index in range(size) if (constant >> (index * 8)) & 0xFF != fill]
        if not touched:
            return None
        found = self.exact_holder(address, bits, key, touched[0], touched[-1] + 1)
        if found is None:
            return None
        text, before, width = found
        value = 0
        for index in range(width // 8):
            byte = index - before
            part = (constant >> (byte * 8)) & 0xFF if 0 <= byte < size else fill
            value |= part << (index * 8)
        return text, value, width

    def exact_holder(self, address, bits, key, low, high):
        """The narrowest field or variable with a name of its own (no AS/AT) that holds bytes [low, high)
        of the access of 'bits' at 'address': (C, how many bytes it starts before the access, its bits),
        or None."""
        field = self.fields.get(key)
        if field is not None and field[0] in pointers.STRUCTS:
            struct, offset, base, base_offset, kind_family = field
            layouts = self.emitter.layouts
            family = self.field_family(field)
            candidates = set()
            for start in range(offset + low, offset + low - 4, -1):
                width = layouts.fields.get(struct, {}).get(start)
                if width is not None:
                    candidates.add((start, width))
                for (overlay_start, overlay_bits) in layouts.overlays.get(struct, {}):
                    if overlay_start == start:
                        candidates.add((start, overlay_bits))
                if family is not None:
                    scoped = layouts.scoped_field(struct, family, start)
                    if scoped is not None:
                        candidates.add((scoped[2], scoped[3]))
                    for (overlay_start, overlay_bits) in layouts.scoped_overlays.get(struct, {}).get(family, {}):
                        if overlay_start == start:
                            candidates.add((start, overlay_bits))
            for start, width in sorted(candidates, key=lambda candidate: candidate[1]):
                if start <= offset + low and offset + high <= start + width // 8:
                    text = self.field_lvalue((struct, start, base, base_offset, kind_family), width)
                    if not text.startswith(("AS", "AT")):
                        return text, offset - start, width
            return None
        value = memory.constant_address(address)
        if field is not None or value is None:
            return None
        named = self.emitter.memory
        candidates = []
        scoped = named.scoped.get(self.global_family) if self.global_family else {}
        for start in range(value + low, value + low - 4, -1):
            if start in scoped:
                candidates.append((scoped[start].bits, start, scoped[start].name))
                continue
            item = named.variables.get(memory.own_copy(start))
            if item is not None and not item.indexed:
                candidates.append((item.bits, start, item.name))
            for width in (8, 16, 32):
                overlay = named.overlays.get((start, width))
                if overlay is not None:
                    candidates.append((width, start, overlay.name))
        for width, start, name in sorted(candidates):
            if start <= value + low and value + high <= start + width // 8 and not memory.bus_only(start, width):
                return name, value - start, width
        return None

    def rom_address_name(self, node):
        """NAME_ADDRESS for a constant that is the address of a table, data or code of the program ROM with
        a name, where it can only be that: not a whole 16.16 number (0x80000 is 8.0) nor one below 0x10000,
        nor what the code masks, shifts or multiplies. A field that holds a number (z, speed_x) can hold
        such an address too ("z = ending_dust_cloud_frames_ADDRESS"), as can a base an offset is added to
        ("palette_list += sign_palette_lists_ADDRESS"). None otherwise."""
        if not isinstance(node, ir.Const) or node.bits != cpu.current.address_bits or id(node) in self.numbers():
            return None
        value = node.value
        if value < 0x10000 or not value & 0xFFFF or not memory.program_rom(value):
            return None
        named = self.emitter.address_text(value)
        return named[0] if named is not None else self.emitter.code_addresses.get(value)

    def compound(self, lvalue, operation, right):
        """'lvalue op= right;', or ++ and --; adding 0xFFFFFFFA takes 6 away, as the expressions say it."""
        named = self.rom_address_name(right) if operation in ("add", "sub") else None
        if named is not None:
            return f"{lvalue} {'+=' if operation == 'add' else '-='} {named};"
        if operation in ("add", "sub") and isinstance(right, ir.Const):
            bits = right.bits
            value = right.value & ((1 << bits) - 1)
            if value >> (bits - 1) and (1 << bits) - value < 0x10000:
                operation = "sub" if operation == "add" else "add"
                value = (1 << bits) - value
            if value == 1:
                return f"{lvalue}{'++' if operation == 'add' else '--'};"
            return f"{lvalue} {'+=' if operation == 'add' else '-='} {cprint.literal(value, bits)};"
        symbol = {"add": "+=", "sub": "-=", "and": "&=", "or": "|=", "xor": "^="}[operation]
        if operation == "and" and isinstance(right, ir.Const):
            cleared = cprint.inverted_mask(right.value, right.bits)
            if cleared is not None:
                return f"{lvalue} &= {cleared};"
        return f"{lvalue} {symbol} {self.expr(right)};"

    def numbers(self):
        """ids of the constants that are numbers whatever their value: what the code masks, shifts,
        multiplies, divides or adds with, and whole 16.16 values (0x80000 is 8.0 and the address of
        spawn_objects_by_handler) outside an object's handler, and but for a board memory's start the
        function puts in a variable it reads or writes memory through (ram_cursor = VIDEO_RAM)."""
        if hasattr(self, "_numbers"):
            return self._numbers
        found = set()
        summed = set()
        pointing = self.address_variables()
        numeric = {"and", "or", "xor", "mul", "mulu", "shl", "shr", "sar", "div", "divu", "rem",
                   "remu", "udiv", "urem", "sdiv", "srem"}
        for block in self.function.blocks.values():
            for statement in block.statements + [block.end]:
                for expression in ir.statement_expressions(statement):
                    for node in ir.walk(expression):
                        if isinstance(node, ir.Op) and (node.name in numeric or node.name.startswith("helper:")):
                            found |= {id(arg) for arg in node.args if isinstance(arg, ir.Const)}
                        elif isinstance(node, ir.Op) and node.name in ("add", "sub"):
                            summed |= {id(arg) for arg in node.args if isinstance(arg, ir.Const)}
                        elif isinstance(node, ir.Const) and not node.value & 0xFFFF:
                            hit = self.fields.get(id(statement))
                            if isinstance(statement, ir.Store) and statement.value is node                                     and hit is not None and hit[:2] == ("object", 0x1C):
                                continue
                            if isinstance(statement, ir.SetReg) and statement.value is node                                     and node.value in BOARD_MEMORIES                                     and self.vars.def_name(statement, statement.index) in pointing:
                                continue
                            found.add(id(node))
        self._numbers = found
        self._summed = summed
        return found

    def address_variables(self):
        """The variables the function reads or writes memory through (in the address of a load or a store)."""
        found = set()
        for block in self.function.blocks.values():
            for statement in block.statements + [block.end]:
                addresses = [statement.address] if isinstance(statement, ir.Store) else []
                for expression in ir.statement_expressions(statement):
                    addresses += [node.address for node in ir.walk(expression) if isinstance(node, ir.Load)]
                for address in addresses:
                    for node in ir.walk(address):
                        if isinstance(node, ir.Reg) and node.index not in self.emitter.context:
                            found.add(self.vars.use_name(node))
        found.discard(None)
        return found

    def sum_operands(self):
        """ids of the constants added or taken away: a table's address plus an offset can be one, but code
        is not reached that way."""
        self.numbers()
        return self._summed

    def changed_in_place(self, statement, name):
        """'name op= x;' for an assignment of the variable's own value and something, or None."""
        value = statement.value
        while isinstance(value, ir.Op) and value.name in ("trunc", "fits", "zext"):
            value = value.args[0]
        if not isinstance(value, ir.Op) or value.name not in ("add", "sub", "and", "or", "xor"):
            return None
        width = self.widths.get(name, 32)
        if value.bits < width:
            return None    # the operation wraps at fewer bits than the variable holds

        def own(node):
            while isinstance(node, ir.Op) and node.name in ("zext", "fits", "trunc") and node.bits >= width:
                node = node.args[0]
            return isinstance(node, ir.Reg) and node.index == statement.index and node.bits >= width \
                and self.vars.use_name(node) == name
        left, right = value.args
        if not own(left) and value.name != "sub" and own(right):
            left, right = right, left
        if not own(left):
            return None
        return self.compound(name, value.name, right)

    def effect(self, statement):
        name = statement.name
        args = statement.args
        if name == "frame_wait":
            return [f"frame_wait({self.expr(args[0])});"]
        if name == "take_interrupts":
            return ["take_interrupts();"]
        if name == "frame_end":
            return ["frame_end();"]
        if name == "halt":
            return ["halt();"]
        if name == "invalid_operand":
            return [f"invalid_operand(0x{args[0].value:06X}u);"]
        if name == "load_privileged":
            return [f"load_privileged({self.expr(args[0])}, {self.expr(args[1])});"]
        if name == "write_psw":
            value = self.fresh("psw")
            return [f"{value} = {self.expr(args[0])};", f"psw_write({value});"] \
                + self.flags_from(value, self.live_flags(statement))
        if name == "update_psw":
            mask = args[1]
            if isinstance(mask, ir.Const) and not mask.value & 0xF:
                # only control bits change: the flags stay as they are
                if mask.value == PSW_INTERRUPTS and isinstance(args[0], ir.Const):
                    return ["interrupts_on();" if args[0].value & PSW_INTERRUPTS else "interrupts_off();"]
                return [f"psw_update(psw, {self.expr(args[0])}, {self.expr(mask)});"]
            value = self.fresh("psw")
            flags = ", ".join(self.use(("f", flag)) for flag in cpu.current.flags)
            return [f"{value} = psw_update(psw_read({flags}), {self.expr(args[0])}, {self.expr(args[1])});"] \
                + self.flags_from(value, self.live_flags(statement))
        if name == "move_string":
            shorter = self.string_move(statement)
            if shorter is not None:
                return [shorter]
            source, length1, dest, length2, size, fill, downward = (self.exact(arg) for arg in args)
            return [f"move_string({source}, {length1}, {dest}, {length2}, {size}, {fill}, {downward}, "
                    f"{self.use(('r', 26))}, &{self.register_set(27)}, &{self.register_set(28)});"]
        if name in ("move_bits_down", "move_bits_up"):
            values = ", ".join(self.exact(arg) for arg in args)
            return [f"{name}({values}, &{self.register_set(27)}, &{self.register_set(28)});"]
        if name == "search_bits_up":
            values = ", ".join(self.exact(arg) for arg in args[:4])
            return [f"{self.register_set(args[4].value)} = {name}({values}, &{self.register_set(28)});"]
        if name in ("divx", "divux"):
            divisor, register, _ = args
            if not isinstance(register, ir.Const) or register.value >= 29:
                raise EmitError(f"{self.function.name}: {name} into memory")
            low = self.register_set(register.value)
            high = self.register_set(register.value + 1)
            lines = [f"{name}_pair({self.exact(divisor)}, &{low}, &{high});"]
            live = self.after.get(id(statement), set())
            if ("f", "s") in live:
                lines.append(f"{self.use(('f', 's'))} = {low} >> 31;")
            if ("f", "z") in live:
                lines.append(f"{self.use(('f', 'z'))} = (uint32_t)({low} == 0u);")
            return lines
        raise EmitError(f"{self.function.name}: effect {name}")

    def string_move(self, statement):
        """A MOVCU/MOVCD that copies or fills whole elements, when nothing reads the end addresses it leaves
        in R27 and R28, as one of support.h's move_* and set_* (or None)."""
        source, length1, dest, length2, size, fill, downward = statement.args
        if not all(isinstance(arg, ir.Const) for arg in (size, fill, downward)) or size.value not in (1, 2):
            return None
        if any(variable[0] == "r" and variable[1] in (27, 28) for variable in self.after.get(id(statement), set())):
            return None
        unit = "bytes" if size.value == 1 else "halfwords"
        if length1 == length2:
            # the same count both ways: a copy (MOVCD from the last element down, for overlapping moves)
            backward = "_backward" if downward.value else ""
            return f"move_{unit}{backward}({self.address_argument(dest)}, {self.address_argument(source)}, " \
                   f"{self.exact(length1)});"
        if fill.value and not downward.value and length1 == ir.Const(0, length1.bits):
            return f"set_{unit}({self.address_argument(dest)}, {self.exact(length2)}, {self.use(('r', 26))});"
        return None

    def address_argument(self, node):
        """C for what a helper takes as an address: a constant one by its name, or as a place in the work
        RAM or one of the board's memories (WORK_RAM + 0x8000u), even with low bits all zero."""
        if isinstance(node, ir.Const):
            named = self.emitter.address_text(node.value)
            if named is not None:
                return named[0]
            if memory.work_ram(node.value & 0xFFFFFF) and node.value == node.value & 0xFFFFFF:
                offset = node.value - 0x200000
                return "WORK_RAM" if not offset else f"WORK_RAM + 0x{offset:X}u"
        return self.exact(node)

    def arguments(self, target, return_address, entry=None):
        values = []
        second = self.emitter.second_entries.get(target)
        if second:
            # which way into it: 0 its first instruction, then the others in order
            values.append("0u" if entry is None else f"{second.index(entry) + 1}u")
        if target in self.emitter.needs_return_address:
            if return_address is None:
                values.append("return_address")
            else:
                values.append(f"0x{return_address:06X}u")
        widths = self.emitter.param_widths.get(target, {})
        for value in self.emitter.by_value(target):
            if value[0] != "r":
                values.append(self.use(value))
                continue
            if self.never_set(value[1]):
                # nothing in this function sets the register, and it is no parameter of its own: 0
                values.append("0u")
                continue
            name = self.implicit_value(value[1])
            width = widths.get(value[1], 32)
            # a parameter narrower than the variable takes its low bits, and says so
            values.append(name if self.widths.get(name, 32) <= width else f"({cprint.UNSIGNED[width]}){name}")
        for value in self.emitter.by_pointer(target):
            values.append("&" + (self.register_set(value[1]) if value[0] == "r" else self.use(value)))
        return ", ".join(values)

    def never_set(self, index):
        """True if the call being written reads a register this function never sets and does not take:
        its variable still holds the 0 it starts with."""
        if index in self.emitter.context:
            return False
        reaching = self.vars.implicit.get((id(self.current), index))
        return reaching == frozenset([("entry", index)]) and index not in self.vars.entry_registers

    def call_expression(self, target, return_address, entry=None):
        target, entry = self.emitter.resolve(target, entry)
        if target not in self.emitter.names:
            raise EmitError(f"{self.function.name}: calls {target:06X}, which is no function")
        return f"{self.emitter.names[target]}({self.arguments(target, return_address, entry)})"

    def call_text(self, target, return_address, entry=None):
        """The call as a statement; the value it returns goes to its variable."""
        returned = self.emitter.returned(self.emitter.resolve(target, entry)[0])
        call = self.call_expression(target, return_address, entry)
        if returned is None:
            return f"{call};"
        if returned[0] == "r" and isinstance(self.current, ir.Call):
            live = self.after.get(id(self.current))
            if live is not None and not any(variable[0] == "r" and variable[1] == returned[1] for variable in live):
                return f"{call};"    # nothing reads what it returns
        target_name = self.register_set(returned[1]) if returned[0] == "r" else self.use(returned)
        return f"{target_name} = {call};"

    def call(self, statement):
        if statement.target is not None:
            return [self.call_text(statement.target & 0xFFFFFF, statement.return_address)]
        if statement.slot is not None:
            return [f"call_code({self.expr(statement.address)});"]
        if not statement.choices:
            raise EmitError(f"{self.function.name}: a computed call with no known targets")
        indexed = switch_index(statement.address, statement.choices, self.emitter.read32)
        if indexed is not None:
            # a call through a table: on the index the table is read with
            index, numbers = indexed
            lines = [f"switch ({self.exact(index)})", "{"]
            for choice in sorted(statement.choices, key=lambda choice: min(numbers[choice])):
                lines += [f"case {number}:" for number in sorted(numbers[choice])]
                lines += ["\t" + self.call_text(choice, statement.return_address), "\tbreak;"]
            lines += ["default:", f"\t{cpu.current.invalid_jump}({self.expr(statement.address)});", "}"]
            return lines
        offsets = offset_switch(statement.address, statement.choices)
        if offsets is not None:
            # into a table of code: on the offset into it
            offset, positions = offsets
            lines = [f"switch ({self.exact(offset)})", "{"]
            for choice in sorted(statement.choices, key=lambda choice: positions[choice]):
                lines += [f"case 0x{positions[choice]:X}u:", "\t" + self.call_text(choice, statement.return_address),
                          "\tbreak;"]
            lines += ["default:", f"\t{cpu.current.invalid_jump}({self.expr(statement.address)});", "}"]
            return lines
        target = self.fresh("target")
        lines = [f"{target} = {self.expr(statement.address)};", f"switch ({target})", "{"]
        for choice in statement.choices:
            lines += [f"case 0x{choice:06X}u:", "\t" + self.call_text(choice, statement.return_address), "\tbreak;"]
        lines += ["default:", f"\t{cpu.current.invalid_jump}({target});", "}"]
        return lines

    def ending(self, end):
        """Lines for a block end that leaves the function."""
        self.current = end
        if isinstance(end, ir.Return):
            return self.copy_out() + [self.return_text()]
        if isinstance(end, ir.TailCall):
            if self.returned is not None and self.emitter.returned(self.emitter.resolve(end.target, end.entry)[0])                     == self.returned:
                self.not_copied.update(self.results)
                return [f"return {self.call_expression(end.target, None, end.entry)};"]
            return [self.call_text(end.target, None, end.entry)] + self.copy_out() + [self.return_text()]
        if isinstance(end, ir.InterruptReturn):
            if not (isinstance(end.frame, ir.Const) and end.frame.value == 0):
                raise EmitError(f"{self.function.name}: RETIS drops stack")
            if self.has_results:
                raise EmitError(f"{self.function.name}: an interrupt handler with results")
            return ["return;"]
        raise EmitError(f"{self.function.name}: block end {end!r}")

    def copy_out(self):
        """Back through the result pointers: of a register, only the parts the function may write; the
        caller's other bits stay as they were."""
        lines = []
        for value in self.results:
            name, text = self.emitter.param_name(self.entry, value), self.out_value(value)
            if value[0] == "r" and isinstance(self.current, ir.Return) and self.vars.unchanged(self.current, value[1]):
                # (nothing on the way here wrote the register: through its pointer nothing would be written either)
                continue    # as it came in: the caller's variable already holds it (after a tail call it may not)
            if value[0] == "r":
                mask = self.emitter.written_mask(self.entry, value[1])
                if not mask:
                    self.not_copied.add(value)
                    continue    # it never writes it (a result only for the others it shares a signature with)
                if mask != 0xFFFFFFFF:
                    width = self.widths.get(text, 32)
                    part = text if width <= mask.bit_length() else f"({text} & 0x{mask:X}u)"
                    lines.append(f"*p_{name} = (*p_{name} & 0x{~mask & 0xFFFFFFFF:X}u) | {part};")
                    continue
            lines.append(f"*p_{name} = {text};")
        return lines

    def out_value(self, value):
        return self.register_value(value[1]) if value[0] == "r" else self.use(value)

    def return_text(self):
        return f"return {self.out_value(self.returned)};" if self.returned is not None else "return;"

    # ---- the tree of structure.py ----

    def nodes(self, nodes, depth):
        lines = []
        for position, node in enumerate(nodes):
            lines += self.node(node, depth, position == len(nodes) - 1)
        return lines

    def node(self, node, depth, last):
        pad = "\t" * depth
        if isinstance(node, structure.Label):
            if node.label not in self.labels:
                return []
            return ["\t" * (depth - 1) + f"{self.emitter.label_text(node.label)}:" + (";" if last else "")]
        if isinstance(node, structure.Code):
            lines = []
            if self.emitter.trace and isinstance(node.label, int):
                lines.append(f"game_trace_block(0x{node.label:06X}u);")
            for statement in node.statements:
                # an assignment of a variable to itself (two registers that hold one variable) does nothing
                lines += [line for line in self.statement(statement) if not SELF_ASSIGNMENT.fullmatch(line)]
            if self.emitter.reroll:
                lines = rerolled(lines)
            return [pad + line for line in lines]
        if isinstance(node, structure.End):
            return [pad + line for line in self.ending(node.end)]
        if isinstance(node, structure.Break):
            return [pad + "break;"]
        if isinstance(node, structure.Continue):
            return [pad + "continue;"]
        if isinstance(node, structure.Goto):
            return [pad + f"goto {self.emitter.label_text(node.label)};"]
        if isinstance(node, structure.Mark):
            value = node.value
            text = ("true" if value.value else "false") if isinstance(value, ir.Const) else self.exact(value)
            return [pad + f"{self.control(node.name)} = {text};"]
        if isinstance(node, structure.If):
            return self.if_lines(node, depth)
        if isinstance(node, structure.Loop):
            body = self.braced(node.body, depth)
            if node.kind == "while":
                return [pad + f"while ({self.exact(node.condition)})"] + body
            if node.kind == "do":
                body[-1] = pad + f"}} while ({self.exact(node.condition)});"
                return [pad + "do"] + body
            return [pad + "for (;;)"] + body
        if isinstance(node, structure.Switch) and isinstance(node.value, ir.EntryPoint):
            # the way the function was entered by (its callers pass it)
            lines = [pad + "switch (entry_point)", pad + "{"]
            for values, body in node.cases:
                lines += [pad + f"case {value}:" for value in values]
                lines += self.nodes(body, depth + 1)
            return lines + [pad + "}"]
        if isinstance(node, structure.Switch):
            indexed = switch_index(node.value, [value for values, _ in node.cases for value in values],
                                   self.emitter.read32)
            if indexed is not None:
                index, numbers = indexed
                lines = [pad + f"switch ({self.exact(index)})", pad + "{"]
                for values, body in node.cases:
                    for number in sorted(n for value in values for n in numbers[value]):
                        lines.append(pad + f"case {number}:")
                    lines += self.nodes(body, depth + 1)
                lines += [pad + "default:", pad + f"\t{cpu.current.invalid_jump}({self.expr(node.value)});", pad + "}"]
                return lines
            offsets = offset_switch(node.value, [value for values, _ in node.cases for value in values])
            if offsets is not None:
                offset, positions = offsets
                lines = [pad + f"switch ({self.exact(offset)})", pad + "{"]
                for values, body in node.cases:
                    lines += [pad + f"case 0x{positions[value]:X}u:" for value in values]
                    lines += self.nodes(body, depth + 1)
                lines += [pad + "default:", pad + f"\t{cpu.current.invalid_jump}({self.expr(node.value)});", pad + "}"]
                return lines
            target = self.fresh("target")
            lines = [pad + f"{target} = {self.expr(node.value)};", pad + f"switch ({target})", pad + "{"]
            for values, body in node.cases:
                lines += [pad + f"case 0x{value:06X}u:" for value in values]
                lines += self.nodes(body, depth + 1)
            lines += [pad + "default:", pad + f"\t{cpu.current.invalid_jump}({target});", pad + "}"]
            return lines
        raise EmitError(f"{self.function.name}: tree node {node!r}")

    def if_lines(self, node, depth):
        pad = "\t" * depth
        lines = [pad + f"if ({self.exact(node.condition)})"]
        # an if inside an if with an else takes braces, or the else would belong to the inner one
        needs_braces = bool(node.otherwise) and any(isinstance(inner, structure.If) for inner in node.then)
        lines += self.braced(node.then, depth) if needs_braces else self.body(node.then, depth)
        if node.otherwise:
            if len(node.otherwise) == 1 and isinstance(node.otherwise[0], structure.If):
                chained = self.if_lines(node.otherwise[0], depth)
                lines.append(pad + "else " + chained[0].lstrip("\t"))
                lines += chained[1:]
            else:
                lines.append(pad + "else")
                lines += self.body(node.otherwise, depth)
        return lines

    def body(self, nodes, depth):
        """The body of an if: one simple statement as it is, anything else in braces."""
        inner = self.nodes(nodes, depth + 1)
        simple = len(inner) == 1 and not inner[0].lstrip("\t").startswith(("if ", "for ", "while ", "do", "switch"))
        return inner if simple else self.wrap_braces(inner, depth)

    def braced(self, nodes, depth):
        return self.wrap_braces(self.nodes(nodes, depth + 1), depth)

    @staticmethod
    def wrap_braces(inner, depth):
        pad = "\t" * depth
        return [pad + "{"] + inner + [pad + "}"]

    # ---- the whole function ----

    def prototype(self):
        return self.emitter.prototype(self.entry)

    def write(self):
        """The function's C. Written again (once emitter.shared_results is known), only its last steps run."""
        if self.written is None:
            tree, self.labels = structure.structure(self.function, self.control_name)
            self.written = self.name_controls(self.nodes(tree, 1))
        body = self.forward_results(list(self.written))
        if body and body[-1] == "\treturn;":
            body.pop()    # running off the end returns as well
        said = self.emitter.comments.get(self.emitter.names.get(self.entry, self.function.name))
        text = (comment_lines(said) if said else []) + [self.prototype(), "{"]
        # the names the code mentions, fields (after "->" or ".") aside: an unused variable z is not
        # declared because some OBJECT(...)->z is read
        declarations = self.declarations(set(re.findall(r"(?<![\w.>])[A-Za-z_]\w*", "\n".join(body))))
        text += ["\t" + line for line in declarations]
        if self.emitter.trace:
            text.append(f"\tgame_trace(0x{self.entry:06X}u);")
        if declarations:
            text.append("")
        text += body
        text.append("}")
        return "\n".join(text) + "\n"

    def declarations(self, words):
        """The function's variables; of the ones that start at 0, only those its code ('words') mentions."""
        lines = []
        params = set(self.params)
        declared = {self.emitter.param_name(self.entry, value) for value in self.params}
        for value in self.results:
            if value[0] != "r":
                lines.append(f"uint32_t {value_name(value)} = *p_{value_name(value)};")
                declared.add(value_name(value))
                continue
            if not self.vars.entry_live(value[1]):
                continue    # always written before it is read: an ordinary variable
            name = self.vars.entry_name(value[1])
            if name in self.forwarded:
                continue    # (the caller's pointer goes straight on: forward_results)
            width = self.widths.get(name, 32)
            cast = "" if width == 32 else f"({cprint.UNSIGNED[width]})"
            lines.append(f"{cprint.UNSIGNED[width]} {name} = {cast}*p_{self.emitter.param_name(self.entry, value)};")
            declared.add(name)
        if self.controls:
            lines.append("bool " + ", ".join(f"{name} = false" for name in sorted(self.controls.values())) + ";")
        flags = ordered(value for value in self.used if value[0] == "f" and value not in params
                        and value not in self.results and value_name(value) in words)
        if flags:
            lines.append("uint32_t " + ", ".join(f"{value_name(value)} = 0" for value in flags) + ";")
        for bits in (8, 16, 32):
            names = sorted((name for name, width in self.widths.items()
                            if width == bits and name not in declared and name in words), key=variable_order)
            if names:
                lines.append(f"{cprint.UNSIGNED[bits]} " + ", ".join(f"{name} = 0" for name in names) + ";")
        for bits in (8, 16, 32):
            names = [name for offset, name in sorted(self.slots.items(), reverse=True)
                     if self.slot_widths[offset] == bits and name in words]
            if names:
                lines.append(f"{cprint.UNSIGNED[bits]} " + ", ".join(f"{name} = 0" for name in names) + ";")
        for bits in (8, 16, 32, 64):
            names = [name for key, name in self.temps.items()
                     if max(self.temp_bits[key], 32 if self.temp_bits[key] == 1 else 0) == bits and name in words]
            if names:
                lines.append(f"{cprint.UNSIGNED[bits]} " + ", ".join(f"{name} = 0" for name in names) + ";")
        return lines


class Emitter:
    def __init__(self, functions, signatures, names, context, needs_return_address, memory_, always, read32,
                 trace=False):
        self.memory = memory_
        self.read32 = read32    # reads the program ROM (switch tables)
        self.register_names = {}    # context register -> its name (symbols.txt)
        self.param_widths = {}      # entry -> {register: width of the parameter} (FunctionWriter)
        self.second_entries = {}    # entry -> its other ways in (decompile.py), taken by entry_point
        self.code_addresses = {}    # address of code calls through memory slots reach -> NAME_ADDRESS
        self.trampolines = {}       # entry -> (function, way in) of a function that only jumps to another
        self.param_names = {}       # (entry, register) -> the name symbols.txt gives a parameter or result
        self.result_names = {}      # (entry, register) -> the name a "result" line gives what a function
                                    # nothing calls hands back
        self.local_names = {}       # (entry, "entry", "rN") -> the name symbols.txt gives what a register
                                    # holds when the function starts
        self.local_sites = {}       # (address, "rN" or "tN") -> the name symbols.txt gives what it writes
        self.split_webs = False     # every web a function reads its own variable (variables.py)
        self.missing_fields = {}    # (family or "function NAME", offset, bits) -> functions (note_missing)
        self.data_labels = {}       # program ROM address -> name of the data label symbols.txt puts there
        self.code_labels = {}       # address -> name of the code label symbols.txt puts there
        self.comments = {}          # function name -> what the C says above it (symbols.txt comment lines)
        self.shared_results = {}    # function name -> its result pointers some call gives the same variable as
                                    # another one (shared_results)
        self.raw_addresses = {}     # ROM address printed as a number -> the functions that use it
        self.writes = {}            # entry -> what a function may write (dataflow summaries)
        self.always = always    # entry -> what a function writes on every path (dataflow summaries)
        self.functions = functions
        self.signatures = signatures
        self.names = names
        self.context = context
        self.needs_return_address = needs_return_address
        self.trace = trace    # every function reports itself to game_trace() when it starts
        self.saved_locals = False    # a stack slot that saves a local variable is saved_NAME (slot_hint)
        self.reroll = False    # runs of one group of statements written as a loop (rerolled)

    def context_name(self, index):
        """The C name of a register the whole program shares."""
        return self.register_names.get(index, register_name(index))

    def after_call(self, target, written, current, cursor):
        """restores_current's (R20, R23) states after a call to 'target' (None: not known) that may write
        the registers 'written'."""
        if not written & {20, 23}:
            return current, cursor
        inner = self.restores_current(target) if target is not None else (None, None)
        if 23 in written:
            cursor = cursor if inner[1] == "kept" else frozenset(["other"])
        if 20 in written:
            if inner[0] == "cursor":
                current = frozenset(["cursor"]) if cursor == frozenset(["same"]) else frozenset(["other"])
            elif inner[0] == "mixed":
                current = current | {"cursor"}
            else:
                current = frozenset(["other"])
        return current, cursor

    def restores_current(self, entry):
        """How a function leaves R20 (the current object) and R23 (the object loop's cursor) to its caller,
        for the names of the fields the caller reads through them afterwards: (R20, R23). R23 is "kept"
        when some way out leaves it as it came in (delete_if_off_screen changes it only when it deletes
        the object, after which its callers give the object up); R20 is "cursor" when every such way out
        sets it from R23 (mov.w R23, R20, as spawn_objects ends), "mixed" when some leave it as it came
        in or restore it."""
        if not hasattr(self, "_restores"):
            self._restores = {}
        if entry in self._restores:
            return self._restores[entry]
        self._restores[entry] = (None, None)    # a call back into itself: nothing known
        function = self.functions.get(entry)
        if function is None:
            return (None, None)
        # forward over the blocks: what R20 may hold ("same" as it came in, "cursor" = R23, "other"), and
        # whether R23 may have changed; stack slots keep what was saved in them
        start = {function.entry: (frozenset(["same"]), frozenset(["same"]))}
        pending = [function.entry]
        exits = set()
        while pending:
            label = pending.pop()
            current, cursor = start[label]
            saved = {}
            block = function.blocks[label]
            for statement in block.statements:
                if isinstance(statement, ir.SetStack) and statement.value in (ir.Reg(20), ir.Reg(23)):
                    saved[statement.offset] = current if statement.value == ir.Reg(20) else cursor
                elif isinstance(statement, ir.SetReg) and statement.index in (20, 23):
                    value = statement.value
                    if isinstance(value, ir.Stack) and value.offset in saved:
                        state = saved[value.offset]
                    elif statement.index == 20 and value == ir.Reg(23):
                        state = frozenset(["cursor"]) if cursor == frozenset(["same"]) else frozenset(["other"])
                    else:
                        state = frozenset(["other"])
                    if statement.index == 20:
                        current = state
                    else:
                        cursor = state
                elif isinstance(statement, (ir.Call, ir.Effect)):
                    written = {variable[1] for variable in self.flow.statement_writes(statement) if variable[0] == "r"}
                    target = statement.target & 0xFFFFFF \
                        if isinstance(statement, ir.Call) and statement.target is not None else None
                    current, cursor = self.after_call(target, written, current, cursor)
            if isinstance(block.end, ir.Return):
                exits.add((current, cursor))
            elif isinstance(block.end, ir.TailCall):
                # the function it goes on in returns for it: as a call and a return
                summary = self.flow.summaries.get(block.end.target)
                written = {variable[1] for variable in summary.writes if variable[0] == "r"} \
                    if summary is not None else {20, 23}
                exits.add(self.after_call(block.end.target, written, current, cursor))
            elif isinstance(block.end, ir.InterruptReturn):
                exits.add((frozenset(["other"]), frozenset(["other"])))
            for successor in block.successors():
                old = start.get(successor, (frozenset(), frozenset()))
                new = (old[0] | current, old[1] | cursor)
                if new != old:
                    start[successor] = new
                    pending.append(successor)
        kept = [current for current, cursor in exits if "same" in cursor]
        if not kept:
            result = (None, None)
        else:
            found = set().union(*kept)
            if found == {"cursor"}:
                result = ("cursor", "kept")
            elif found - {"other"}:
                result = ("mixed", "kept")
            else:
                result = (None, "kept")
        self._restores[entry] = result
        return result

    def label_text(self, label):
        """The C label for a block: the name symbols.txt gives the code there ("code" lines), when nothing
        global has it, else L_ and its address."""
        if isinstance(label, int):
            name = self.code_labels.get(memory.own_copy(label & 0xFFFFFF))
            if name is not None and self.local_name(name) is not None:
                return name
        if isinstance(label, tuple) and len(label) == 2 and label[0] == "start" and label[1] in self.names:
            # the first instruction of a function with two ways in (the other one jumps back to it)
            name = f"{self.names[label[1]]}_start"
            if self.local_name(name) is not None:
                return name
        return label_name(label)

    def address_text(self, value):
        """(C, precedence) for a constant that is an address with a name: a variable or table (its
        NAME_ADDRESS), a data label, a cell of the text layer, a board register, a place in one of the
        board's memories; or None."""
        address = value & 0xFFFFFF
        if address != value:
            return None
        if memory.work_ram(address):
            if address == memory.VARIABLES:
                return "GAME_VARIABLES", cprint.PRIMARY
            first, size, count = pointers.PLAYERS
            if first <= address < first + size * count and not (address - first) % size:
                return f"PLAYER_BLOCK({(address - first) // size + 1})", cprint.PRIMARY
            for struct, (start, number) in pointers.ARRAYS.items():
                size = pointers.STRUCTS[struct]
                if start <= address < start + size * number and not (address - start) % size:
                    return f"{struct.upper()}_ADDRESS({(address - start) // size})", cprint.PRIMARY
            anchor = self.memory.anchors.get(address)
            if anchor is not None:
                # the list's head, less the offset of the link it is reached through
                return f"{anchor[0]}_ADDRESS - 0x{anchor[1]:X}u", cprint.ADD
            item = self.memory.variables.get(address)
            if item is None and address == 0x200000:
                return "WORK_RAM", cprint.PRIMARY
            return (f"{item.name}_ADDRESS", cprint.PRIMARY) if item is not None else None
        if memory.program_rom(address):
            if address < memory.ROM_NAMES_FROM:
                return None
            canonical = memory.own_copy(address)
            item = self.memory.tables.get(canonical)
            if item is not None:
                return f"{item.name}_ADDRESS", cprint.PRIMARY
            label = self.data_labels.get(canonical)
            return (f"{label}_ADDRESS", cprint.PRIMARY) if label is not None else None
        register = self.memory.register(address)
        if register is not None:
            return register, cprint.PRIMARY
        if 0x31F000 <= address < 0x31FF00 and not address & 1:
            cell = (address - 0x31F000) // 2
            return f"TEXT_CELL({cell % 64}, {cell // 64})", cprint.PRIMARY
        for start, end, region in REGIONS:
            if start <= address < end:
                offset = address - start
                return (region, cprint.PRIMARY) if not offset else (f"{region} + 0x{offset:X}u", cprint.ADD)
        return None

    def resolve(self, target, entry=None):
        """The function a call to 'target' runs, and the way into it: a function that is only a jump to
        another one (an entry of a table of "br" instructions) hands the call on."""
        seen = set()
        while entry is None and target in self.trampolines and target not in seen:
            seen.add(target)
            target, entry = self.trampolines[target]
        return target, entry

    def returned(self, entry):
        """The one value a function hands back, as its return value; None if it has none or several."""
        results = self.signatures[entry].results
        return next(iter(results)) if len(results) == 1 else None

    def param_name(self, entry, value):
        """C name of a parameter or result of a function: the one symbols.txt gives the register, when no
        global takes it, else the register's."""
        if value[0] == "r":
            name = self.param_names.get((entry, value[1]))
            if name is None:
                # (no param line: what a local line calls the register where the function starts, or what a
                # result line calls what it hands back there)
                name = self.local_names.get((entry, "entry", f"r{value[1]}"),
                                            self.result_names.get((entry, value[1])))
            if name is not None and self.local_name(name) is not None:
                return name
        return value_name(value)

    def local_sites_free(self):
        """local_sites without the names something global has."""
        if not hasattr(self, "_local_sites_free"):
            self._local_sites_free = {key: name for key, name in self.local_sites.items()
                                      if self.local_name(name) is not None}
        return self._local_sites_free

    def local_name(self, name):
        """'name' for a local variable, unless something global has it already (a macro would take its
        place): then None."""
        if not hasattr(self, "_globals"):
            found = set(self.names.values()) | set(self.register_names.values()) | set(cprint.UNSIGNED.values())
            found |= set(cprint.SIGNED.values()) | GLOBAL_WORDS
            for items in (self.memory.variables, self.memory.tables, self.memory.hardware, self.memory.overlays):
                for item in items.values():
                    found |= {item.name, item.name + "_ADDRESS"}
            for items in list(self.memory.scoped.values()) + list(self.memory.scoped_overlays.values()):
                for item in items.values():
                    found |= {item.name, item.name + "_ADDRESS"}
            self._globals = found
        return None if name in self._globals or not name.isidentifier() else name

    def global_family_of(self, entry):
        """The family whose own names for the work RAM ("scoped global") 'entry' uses, or None."""
        name = self.names[entry]
        found = None
        for family in self.memory.scoped:
            if (name == family or name.startswith(family + "_")) and (found is None or len(family) > len(found)):
                found = family
        return found

    def family_of(self, entry, struct):
        """The family of functions 'entry' belongs to for the scoped fields of 'struct': the longest one
        whose name starts its name (fire_front_chase is fire_front), or None."""
        name = self.names[entry]
        found = None
        for family in self.layouts.families(struct):
            if (name == family or name.startswith(family + "_")) and (found is None or len(family) > len(found)):
                found = family
        return found

    def written_mask(self, entry, register):
        """The bits of a register a function may write."""
        mask = 0
        for variable in self.writes.get(entry, set()):
            if variable[0] == "r" and variable[1] == register:
                mask |= dict(dataflow.PARTS)[variable[2]]
        return mask

    def always_written(self, entry, value):
        written = self.always.get(entry, set())
        if value[0] == "f":
            return value in written
        return all(("r", value[1], part) in written for part in range(3))

    def by_value(self, entry):
        """What a function takes by value: its parameters, and the value it returns when it may return
        it unchanged (a register it does not always write)."""
        signature = self.signatures[entry]
        params = set(signature.params - signature.results)
        returned = self.returned(entry)
        if returned is not None and (returned in signature.params or not self.always_written(entry, returned)):
            params.add(returned)
        return ordered(params)

    def by_pointer(self, entry):
        """The results a function hands back through pointers (when it has more than one)."""
        if self.returned(entry) is not None:
            return []
        return ordered(self.signatures[entry].results)

    def prototype(self, entry):
        params = []
        if self.second_entries.get(entry):
            params.append("uint32_t entry_point")
        if entry in self.needs_return_address:
            params.append("uint32_t return_address")
        widths = self.param_widths.get(entry, {})
        for value in self.by_value(entry):
            width = widths.get(value[1], register_bits(value[1])) if value[0] == "r" else 32
            params.append(f"{cprint.UNSIGNED[width]} {self.param_name(entry, value)}")
        for value in self.by_pointer(entry):
            width = register_bits(value[1]) if value[0] == "r" else 32
            params.append(f"{cprint.UNSIGNED[width]} *p_{self.param_name(entry, value)}")
        returned = self.returned(entry)
        kind = "void" if returned is None else cprint.UNSIGNED[register_bits(returned[1]) if returned[0] == "r" else 32]
        return f"{kind} {self.names[entry]}({', '.join(params) if params else 'void'})"

    def wrapper(self, entry):
        """A function of no parameters that calls 'entry' with zeros, for call_code()."""
        if entry in self.needs_return_address:
            raise EmitError(f"{self.names[entry]}: called through a slot, but reads its return address")
        by_value, by_pointer = self.by_value(entry), self.by_pointer(entry)
        if not by_value and not by_pointer and self.returned(entry) is None and not self.second_entries.get(entry):
            return None
        lines = comment_lines(f"How call_code() runs {self.names[entry]} from a slot that holds its address: with 0 "
                              "for what it takes, and what it hands back dropped.")
        lines += [f"static void call_{entry:06X}(void)", "{"]
        if by_pointer:
            lines.append("\tuint32_t " + ", ".join(f"{value_name(value)} = 0" for value in by_pointer) + ";")
        values = ["0"] * len(by_value) + [f"&{value_name(value)}" for value in by_pointer]
        if self.second_entries.get(entry):
            values.insert(0, "0u")
        lines.append(f"\t{self.names[entry]}({', '.join(values)});")
        lines.append("}")
        return "\n".join(lines) + "\n"
