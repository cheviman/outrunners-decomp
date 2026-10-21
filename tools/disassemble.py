"""
Name:        disassemble.py
Description: recursive-descent disassembly of the V70 program into a listing and a coverage report
Version:     0.1.0
Created:     11-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

Usage: python tools/disassemble.py [rom folder or .zip] [--trace executed.txt ...]   (default ROM: rom/)
Writes build/listing/orunners.s and build/listing/report.txt.

--trace adds the addresses a real run executed (tools/reference/s32ref --trace) as entry points
and checks them against the static analysis: an executed address that falls inside a statically
decoded instruction means one of the two readings is wrong, and it is reported.

Starts at the entry points in symbols.txt and follows every branch, call and jump whose
target can be known without running the game. Whatever is never reached stays as data.
On top of plain flow it understands two patterns the game uses everywhere:
  - branch tables: "jmp table[PC](R0)" lands on a run of "br" instructions (with "nop" padding; the
    first may be a conditional branch, on a flag the code before the jump sets)
  - pointer tables: "mov.w table[PC](R0), R0" followed by "jsr [R0]" reads a table of addresses
  - code pointer slots: "movea.b func[PC], 1C[R20]" (or an absolute "movea.b func, 1C[R11]") stores
    a handler that "jsr [1C[R20]]" calls; "mov.w table[PC](R3), 4[R21]" stores one of a table of them
  - a table entry loaded into a register some instructions before "jsr [R0]" (OutRunners, 00EAEE), as
    long as nothing in between writes the register or calls
A stored or tabled address is accepted as code only if a trial decode of it is clean.
"""
import re
import sys
from collections import deque
from dataclasses import dataclass
from pathlib import Path
from typing import NamedTuple

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import orunners_rom  # noqa: E402
from v60 import disassembler as dis  # noqa: E402

SYMBOL_KINDS = ("func", "code", "data")
FORK = -1                       # a record kind (kinds=) that ends its array with a table of arrays
STACK_POINTER = 31
MAX_RECORDS = 4096
CODE_CHIPS = orunners_rom.PROGRAM_AREAS[:2]


@dataclass(frozen=True)
class Layout:
    """Where a data table holds code addresses (symbols.txt: record=, count=, end=, code=, lists=, pointers=)."""
    record: int                 # bytes per record
    code: tuple                 # (offset inside each record, base): a 32-bit code address when base is None,
                                # else a 16-bit offset from base
    count: int | None = None    # records per array ...
    prefixed: bool = False      # ... or: a record count comes before the records ...
    prefix_bytes: int = 2       # how long that count is: 2 (count=prefix) or 4 (count=prefix32, bytes32)
    prefix_in_bytes: bool = False  # the count is the records' length in bytes (count=bytes32)
    end: int | None = None      # ... or: the array ends with this word where the next record would start
    end_bytes: int = 2          # how long that end mark is: 2 (end=FFFF) or 4 (end=FFFFFFFF, a whole end record)
    lists: int | None = None    # the table is this many 16-bit offsets, from its own address, to such arrays
    pointers: int | None = None  # the table is this many 32-bit addresses of such arrays
    kinds: tuple | None = None   # (offset, {value: length, 0 or FORK}): a 16-bit field picks each record's
                                 # length, 0 for the record that ends the array (its own first offset + 2
                                 # bytes), FORK for one that ends it with a table of addresses of arrays
                                 # (as many as fit before the first of them) that go on from there


class Symbol(NamedTuple):
    kind: str
    name: str
    layout: Layout | None = None
    calls: str | None = None     # code: the computed call there goes to the code of this described table


def parse_code_fields(text, where):
    """'8,12' or '2:973B4': field offsets, each with the base its 16-bit offset is added to, if any"""
    fields = []
    for field in text.split(","):
        offset, _, base = field.partition(":")
        try:
            fields.append((int(offset), int(base, 16) if base else None))
        except ValueError:
            raise SystemExit(f"{where}: bad code field '{field}'") from None
    return tuple(fields)


def parse_layout(attributes, where):
    """Layout from 'key=value' attributes: sizes, offsets and counts in decimal, the end mark and bases in
    hexadecimal (an end mark of more than four digits is 32 bits long)"""
    values = {}
    for attribute in attributes:
        name, _, value = attribute.partition("=")
        if name not in ("record", "count", "end", "code", "lists", "pointers", "kinds") or not value:
            raise SystemExit(f"{where}: unknown layout attribute '{attribute}'")
        values[name] = value
    ends = ("count" in values) + ("end" in values) + ("kinds" in values)
    if "record" not in values or "code" not in values or ends != 1:
        raise SystemExit(f"{where}: a layout needs record=, code= and one of count=, end= or kinds=")
    kinds = None
    if "kinds" in values:
        offset, _, choices = values["kinds"].partition(":")
        lengths = {}
        for choice in choices.split(","):
            kind, _, length = choice.partition("=")
            lengths[int(kind, 16)] = 0 if length == "end" else FORK if length == "fork" else int(length)
        kinds = (int(offset), lengths)
    if "lists" in values and "pointers" in values:
        raise SystemExit(f"{where}: lists= and pointers= do not go together")
    prefixed = values.get("count") in ("prefix", "prefix32", "bytes32")
    return Layout(record=int(values["record"]),
                  code=parse_code_fields(values["code"], where),
                  count=int(values["count"]) if "count" in values and not prefixed else None,
                  prefixed=prefixed,
                  prefix_bytes=4 if values.get("count") in ("prefix32", "bytes32") else 2,
                  prefix_in_bytes=values.get("count") == "bytes32",
                  end=int(values["end"], 16) if "end" in values else None,
                  end_bytes=4 if len(values.get("end", "")) > 4 else 2,
                  lists=int(values["lists"]) if "lists" in values else None,
                  pointers=int(values["pointers"]) if "pointers" in values else None,
                  kinds=kinds)


NAME_KINDS = ("var", "io", "table", "struct", "field", "register", "scoped", "param", "result", "local", "port",
              "file", "anchor", "comment", "task")    # names for the decompiled C (load_names; file lines:
                                              # decompile.source_files)


def load_symbols(path):
    """Read symbols.txt: {canonical address: Symbol(kind, name, layout)} for the program ROM. The lines
    that name variables, hardware, tables and structures are for the decompiler (load_names)."""
    symbols = {}
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        if parts[0] in NAME_KINDS or (len(parts) > 1 and parts[1] in NAME_KINDS):
            continue
        if len(parts) < 3 or parts[1] not in SYMBOL_KINDS:
            raise SystemExit(f"{path.name}:{number}: expected 'ADDRESS KIND NAME' with KIND in {SYMBOL_KINDS}")
        calls = None
        if len(parts) == 4 and parts[1] == "code" and parts[3].startswith("calls="):
            calls = parts[3].partition("=")[2]
            parts = parts[:3]
        if len(parts) > 3 and parts[1] != "data":
            raise SystemExit(f"{path.name}:{number}: only data symbols take a layout (code ones, calls=TABLE)")
        address = orunners_rom.canonical_address(int(parts[0], 16))
        if address is None:
            raise SystemExit(f"{path.name}:{number}: {parts[0]} is not in the program ROM")
        layout = parse_layout(parts[3:], f"{path.name}:{number}") if len(parts) > 3 else None
        symbols[address] = Symbol(parts[1], parts[2], layout, calls)
    return symbols


VALUE_TYPES = ("u8", "s8", "u16", "s16", "u32", "s32")


class Names(NamedTuple):
    """The names symbols.txt gives the decompiled C besides functions and labels."""
    variables: dict     # work RAM address -> (name, type)
    hardware: dict      # address -> (name, type)
    tables: dict        # program ROM address -> (name, element type)
    structs: dict       # name -> size
    fields: dict        # (struct, offset) -> (name, type)
    registers: dict     # register number -> name, for the registers the whole program shares
    targets: dict       # (struct, offset) or ("global", address) -> (struct it points at, bits): a u32
                        # holds its address, a u16 the low half of it (a link in the work RAM page)
    scoped: dict        # (struct, family) -> {offset: (name, type)}: fields that mean one thing only in the
                        # code of one family of functions (an object type's own variables)
    params: dict        # (function name, register number) -> name of what the function takes or hands back
                        # in that register
    overlays: dict      # (struct or "global", offset or address, bits) -> (name, type): a narrower name for
                        # the first bytes of a field or variable (the low half of a 16.16 value)
    locals: dict        # (function name, address of the instruction that writes it or "entry", "rN" or "tN")
                        # -> the name of the local variable
    ports: dict         # I/O port number -> name (a processor with ports of its own: the Z80)
    param_structs: dict  # (function name, register number) -> the structure the parameter points at
    results: dict       # (function name, register number) -> name of what a function nothing calls hands back
    local_structs: dict  # (function name, WHERE, "rN") -> the structure the local variable points at
    rom_structs: set     # the structures that are records of the program ROM ('struct NAME SIZE rom'): read
                         # through rom(), never written
    scoped_targets: dict  # (struct, family, offset) -> (what it points at, bits): a scoped field that holds a
                          # pointer, as that family's code reads it
    scoped_overlays: dict  # (struct, family) -> {(offset, bits): (name, type)}: a narrower name a family gives
                           # the first bytes of one of its fields
    anchors: dict       # work RAM address -> (variable, offset): where a structure would be whose field at
                        # 'offset' is the variable (a list's head), as the code that walks the list starts there
    comments: dict      # function name -> what the C says above it (comment lines)
    tasks: set          # functions a task starts at: the runtime runs them by address (task lines)


def load_names(path, registers=None):
    """Read the var, io, table, struct and field lines of symbols.txt:
        ADDRESS  var    NAME  TYPE [STRUCT]  a variable in work RAM (TYPE: u8 s8 u16 s16 u32 s32)
        ADDRESS  io     NAME  TYPE       a hardware register
        ADDRESS  table  NAME  TYPE [STRUCT]  data in the program ROM the code reads by index, TYPE per element
        struct   NAME   SIZE             a structure in work RAM (SIZE in hexadecimal)
        field    STRUCT OFFSET NAME TYPE [STRUCT]  one of its fields (OFFSET in hexadecimal)
    A second var or field at the same place with another width names the first bytes of the wider one
    (x_fraction, the low half of the 16.16 x).
    A struct name after the type says the value points at one: a u32 its address, a u16 the low half of
    it (the game links its structures in the work RAM page by 16-bit addresses); "rom" after a u32 says it
    points into the program ROM. STRUCT.FAMILY (object.eggman_scene) says which family's structure it is:
    what is read through it reads as that family's code reads its own (its scoped fields).
        register RN     NAME             a register the whole program shares (a global variable in C)
        scoped   STRUCT FAMILY OFFSET NAME TYPE [STRUCT]  a field as the functions whose names start with
                                         FAMILY use it, in the structure their code runs on (a structure or
                                         "rom": what it holds points at one); with "global" for STRUCT and
                                         an address for OFFSET, work RAM as they use it
        param    FUNCTION RN NAME [STRUCT]  what a function takes or hands back in register RN (a structure:
                                         what it takes there points at one; "rom": into the program ROM)
        result   FUNCTION RN NAME [STRUCT]  what a function nothing calls hands back in register RN (no
                                         caller says it: the ones that do call get it from what they read)
        local    FUNCTION WHERE RN NAME [STRUCT]  the local variable of FUNCTION that holds what the
                                         instructions at WHERE (addresses separated by commas) write in
                                         register RN (a call: what it hands back in RN); "entry" in WHERE is
                                         what RN holds when the function starts; a structure: what it holds
                                         points at one
        local    FUNCTION WHERE TN NAME  likewise the N-th value the instruction at WHERE reads or works
                                         out on its way (t1: what a read-modify-write instruction read)
        local    FUNCTION WHERE SN NAME  the stack slot N bytes (hexadecimal) below where SP stood when the
                                         function started, by the instructions at WHERE that store to it
        ADDRESS  port   NAME             an I/O port
        ADDRESS  anchor VARIABLE OFFSET  the address a list walk starts from: a structure there would have
                                         VARIABLE (the list's head) as its field at OFFSET (the link the walk
                                         follows), so the C writes it as VARIABLE's address less OFFSET
        comment  FUNCTION TEXT           what the C says about the function, in a /* */ block above it
        task     FUNCTION                a task starts there, from an interrupt frame the game writes (the
                                         runtime runs it by its address)
    'registers' ({name: number}) lets param and local lines name a register by its name (hl, iy) on a
    processor whose registers have names; RN works on any."""
    names = Names({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, set(), {}, {}, {}, {}, set())
    registers = registers or {}

    def register_number(text):
        """The number of the register 'text' names (RN, or a name of 'registers'), or None."""
        if text.upper().startswith("R") and text[1:].isdigit():
            return int(text[1:])
        return registers.get(text.lower())

    def declare(table, key, overlay_key, name, kind, where):
        """The wider of two names at one place is the field or variable; the narrower one an overlay."""
        old = table.get(key)
        if old is None:
            table[key] = (name, kind)
            return
        if int(old[1][1:]) == int(kind[1:]):
            raise SystemExit(f"{where}: '{name}' has the place and width of '{old[0]}'")
        if int(old[1][1:]) < int(kind[1:]):
            table[key] = (name, kind)
            name, kind = old
        names.overlays[overlay_key + (int(kind[1:]),)] = (name, kind)
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        parts = line.split()
        if not parts:
            continue
        where = f"{path.name}:{number}"
        if parts[0] == "task":
            if len(parts) != 2 or not parts[1].isidentifier():
                raise SystemExit(f"{where}: expected 'task FUNCTION'")
            names.tasks.add(parts[1])
            continue
        if parts[0] == "comment":
            words = line.split(None, 2)
            if len(words) != 3 or not words[1].isidentifier():
                raise SystemExit(f"{where}: expected 'comment FUNCTION TEXT'")
            if words[1] in names.comments:
                raise SystemExit(f"{where}: {words[1]} has a comment already")
            names.comments[words[1]] = words[2]
            continue
        if parts[0] == "struct":
            if len(parts) not in (3, 4) or (len(parts) == 4 and parts[3] != "rom"):
                raise SystemExit(f"{where}: expected 'struct NAME SIZE [rom]'")
            names.structs[parts[1]] = int(parts[2], 16)
            if len(parts) == 4:
                names.rom_structs.add(parts[1])
            continue
        if parts[0] == "scoped":
            if len(parts) not in (6, 7) or parts[5] not in VALUE_TYPES:
                raise SystemExit(f"{where}: expected 'scoped STRUCT FAMILY OFFSET NAME TYPE [STRUCT]' with TYPE in "
                                 f"{VALUE_TYPES}")
            family = names.scoped.setdefault((parts[1], parts[2]), {})
            offset = int(parts[3], 16)
            if offset in family:
                # a second name for the same place: the wider is the field, the narrower its first bytes
                old = family[offset]
                if int(old[1][1:]) == int(parts[5][1:]):
                    raise SystemExit(f"{where}: '{parts[4]}' has the place and width of '{old[0]}'")
                wider, narrower = (old, (parts[4], parts[5])) if int(old[1][1:]) > int(parts[5][1:]) \
                    else ((parts[4], parts[5]), old)
                family[offset] = wider
                names.scoped_overlays.setdefault((parts[1], parts[2]), {})[(offset, int(narrower[1][1:]))] = narrower
            else:
                family[offset] = (parts[4], parts[5])
            if len(parts) == 7:
                names.scoped_targets[(parts[1], parts[2], int(parts[3], 16))] = (parts[6], int(parts[5][1:]))
            continue
        if parts[0] in ("param", "result"):
            if len(parts) not in (4, 5) or register_number(parts[2]) is None or not parts[3].isidentifier():
                raise SystemExit(f"{where}: expected '{parts[0]} FUNCTION RN NAME [STRUCT]'")
            key = (parts[1], register_number(parts[2]))
            table = names.params if parts[0] == "param" else names.results
            if table.get(key, parts[3]) != parts[3]:
                raise SystemExit(f"{where}: {parts[1]}'s {parts[2]} is '{table[key]}' already")
            table[key] = parts[3]
            if len(parts) == 5:
                names.param_structs[(parts[1], register_number(parts[2]))] = parts[4]
            continue
        if parts[0] == "local":
            spots = parts[2].split(",") if len(parts) in (5, 6) else []
            value = parts[3] if len(parts) in (5, 6) else ""
            if not re.fullmatch(r"[rt]\d+|s[0-9A-F]+", value) and register_number(value) is not None:
                value = f"r{register_number(value)}"
            if not spots or any(spot != "entry" and not re.fullmatch(r"[0-9A-Fa-f]{4,6}", spot) for spot in spots) \
                    or not re.fullmatch(r"[rt]\d+|s[0-9A-F]+", value) or not parts[4].isidentifier():
                raise SystemExit(f"{where}: expected 'local FUNCTION WHERE RN NAME [STRUCT]' or 'local FUNCTION WHERE TN NAME'")
            for spot in spots:
                key = (parts[1], spot if spot == "entry" else int(spot, 16), value)
                names.locals[key] = parts[4]
                if len(parts) == 6:
                    names.local_structs[key] = parts[5]
            continue
        if parts[0] == "register":
            if len(parts) != 3 or not parts[1].upper().startswith("R") or not parts[1][1:].isdigit():
                raise SystemExit(f"{where}: expected 'register RN NAME'")
            names.registers[int(parts[1][1:])] = parts[2]
            continue
        if parts[0] == "field":
            if len(parts) not in (5, 6) or parts[4] not in VALUE_TYPES:
                raise SystemExit(f"{where}: expected 'field STRUCT OFFSET NAME TYPE [STRUCT]' with TYPE in {VALUE_TYPES}")
            declare(names.fields, (parts[1], int(parts[2], 16)), (parts[1], int(parts[2], 16)), parts[3], parts[4],
                    where)
            if len(parts) == 6:
                names.targets[(parts[1], int(parts[2], 16))] = (parts[5], int(parts[4][1:]))
            continue
        if len(parts) >= 2 and parts[1] == "anchor":
            if len(parts) != 4 or not parts[2].isidentifier() or not re.fullmatch(r"[0-9A-Fa-f]+", parts[3]):
                raise SystemExit(f"{where}: expected 'ADDRESS anchor VARIABLE OFFSET'")
            names.anchors[int(parts[0], 16)] = (parts[2], int(parts[3], 16))
            continue
        if len(parts) >= 2 and parts[1] == "port":
            if len(parts) != 3 or not parts[2].isidentifier():
                raise SystemExit(f"{where}: expected 'ADDRESS port NAME'")
            names.ports[int(parts[0], 16)] = parts[2]
            continue
        if len(parts) < 2 or parts[1] not in ("var", "io", "table"):
            continue
        if len(parts) not in (4, 5) or parts[3] not in VALUE_TYPES or (len(parts) == 5 and parts[1] == "io"):
            raise SystemExit(f"{where}: expected 'ADDRESS {parts[1]} NAME TYPE' with TYPE in {VALUE_TYPES}")
        address = int(parts[0], 16)
        target = {"var": names.variables, "io": names.hardware, "table": names.tables}[parts[1]]
        if parts[1] == "var":
            declare(target, address, ("global", address), parts[2], parts[3], where)
        else:
            target[address] = (parts[2], parts[3])
        if len(parts) == 5:
            names.targets[("global", address)] = (parts[4], int(parts[3][1:]))
    def structure(pointed):
        """True for STRUCT or STRUCT.FAMILY."""
        struct, dot, family = pointed.partition(".")
        return struct in names.structs and (not dot or family.isidentifier())
    for (pointed, _) in list(names.targets.values()) + list(names.scoped_targets.values()):
        if not structure(pointed) and pointed != "rom":
            raise SystemExit(f"{path.name}: points at '{pointed}', which is no struct (nor 'rom')")
    for (function, _), pointed in names.param_structs.items():
        if not structure(pointed) and pointed != "rom":
            raise SystemExit(f"{path.name}: a parameter of {function} points at '{pointed}', which is no struct "
                             "(nor 'rom')")
    for (function, _, _), pointed in names.local_structs.items():
        if not structure(pointed) and pointed != "rom":
            raise SystemExit(f"{path.name}: a local of {function} points at '{pointed}', which is no struct "
                             "(nor 'rom')")
    return names


def load_trace(paths, fold_mirrors=True):
    """Read s32ref address lists: (program addresses, count of addresses outside the program ROM).

    With fold_mirrors the addresses are folded to their canonical copy; without it they are kept as
    executed, which is what the C generator needs.
    """
    addresses = set()
    outside = 0
    for path in paths:
        for line in Path(path).read_text(encoding="ascii").splitlines():
            if not line or line.startswith("#"):
                continue
            address = int(line, 16)
            canonical = orunners_rom.canonical_address(address)
            if canonical is None:
                outside += 1
            else:
                addresses.add(canonical if fold_mirrors else address & 0xFFFFFF)
    return addresses, outside


class Analysis:
    """Static analysis of the V70 program.

    With fold_mirrors (the listing) every address is folded to its canonical copy. Without it
    (the C generator) addresses are kept as the CPU runs them: the same ROM bytes reached through
    two mirrors are two separate pieces of code, because PC-relative operands differ.
    'entries' replaces the code entry points of symbols.txt: a list of (address, kind).
    """

    def __init__(self, image, symbols, trace=frozenset(), fold_mirrors=True, entries=None):
        self.image = image
        self.mem = orunners_rom.ProgramMemory(image)
        self.symbols = symbols
        self.fold = fold_mirrors
        self.entries = entries
        self.executed = trace    # addresses a real run executed
        self.trace_added = 0     # executed addresses the static analysis had not reached
        self.trace_conflicts = []  # executed addresses inside a statically decoded instruction
        self.trace_outside = 0   # executed addresses outside the program ROM (code in RAM)
        self.instructions = {}   # start address -> Instruction
        self.owner = {}          # every byte of decoded code -> start of its instruction
        self.labels = {}         # address -> "func" | "loc" | "data"
        self.invalid = []        # (address, text, where the path came from)
        self.unresolved = []     # (address, text): jumps and calls whose target needs run-time data
        self.overlaps = []       # (address, start of the instruction already covering it, origin)
        self.outside = []        # (address, target): targets outside the program ROM
        self.slots = set()       # displacements of memory slots that indirect calls read their target from
        self.pointer_writes = [] # (address of the store, stored value, slot displacement)
        self.table_writes = []   # (address of the store, table address, slot displacement): an entry of a
                                 # table of code addresses stored into a slot
        self.table_loads = {}    # while tracing: register -> the table "mov.w table[PC](Rx), reg" loaded it from
        self.rejected = []       # (candidate, address of the store): stored values that do not decode cleanly
        self.data_words = {}     # address of a pointer table entry -> address it points to
        self.described = []      # (first, stop): bytes of the tables symbols.txt describes with a layout
        self.described_targets = set()  # code addresses those tables hold
        self.table_targets = {}  # name of such a table -> the code addresses it holds
        self.jump_tables = {}    # address of "jmp table(Rx)" -> addresses of its "br" entries
        self.call_tables = {}    # address of "jsr table(Rx)" -> addresses of its "br" entries
        self.pointer_tables = {} # address of "jsr [Rx]" fed by a table -> addresses in the table
        self.return_jumps = {}   # address of a return that lands on a pushed address -> that address
        self.push_jumps = {}     # address of a jump that acts as a call (pushed return) -> return address
        self.pending = deque()

    def key(self, address):
        """The address code is known by: canonical, or as executed (24-bit) without folding."""
        canonical = orunners_rom.canonical_address(address)
        if canonical is None:
            return None
        return canonical if self.fold else address & 0xFFFFFF

    @staticmethod
    def fits(address, length):
        """True if an instruction at 'address' lies inside one ROM area (does not run off its end)."""
        start = orunners_rom.canonical_address(address)
        end = orunners_rom.canonical_address(address + length - 1)
        return start is not None and end == start + length - 1

    def add_target(self, address, kind, origin, label=True):
        key = self.key(address)
        if key is None:
            self.outside.append((origin, address))
            return
        if label and (kind == "func" or key not in self.labels):
            self.labels[key] = kind
        self.pending.append((key, origin))

    def run(self):
        if self.entries is None:
            for address, symbol in sorted(self.symbols.items()):
                if symbol.kind == "data":
                    self.labels[address] = "data"
                else:
                    self.add_target(address, "func" if symbol.kind == "func" else "loc", address)
        else:
            for address, kind in self.entries:
                self.add_target(address, kind, address)
        for address, symbol in sorted(self.symbols.items()):
            if symbol.layout is not None:
                self.described_table(address, symbol)
        # a computed call symbols.txt says goes to a described table's code (calls=): its targets
        for address, symbol in sorted(self.symbols.items()):
            if symbol.calls is not None:
                if symbol.calls not in self.table_targets:
                    raise SystemExit(f"symbols.txt: {symbol.name} calls {symbol.calls}, which is no described table")
                self.pointer_tables[address] = sorted(self.table_targets[symbol.calls])
        tried = set()
        self.settle(tried)
        if not self.executed:
            return

        # executed addresses the static analysis did not reach become entry points
        added = [address for address in sorted(self.executed)
                 if address not in self.instructions and address not in self.owner]
        for address in added:
            self.add_target(address, "loc", address, label=False)
        self.settle(tried)
        self.trace_added = sum(1 for address in added if address in self.instructions)
        # label the ones that are not just the next instruction of other executed code
        fall_through = {instruction.address + instruction.length for instruction in self.instructions.values()
                        if instruction.flow not in (dis.FLOW_JUMP, dis.FLOW_RETURN)}
        for address in added:
            if address in self.instructions and address not in fall_through:
                self.labels.setdefault(address, "loc")
        self.trace_conflicts = sorted((address, self.owner[address]) for address in self.executed
                                      if address not in self.instructions and address in self.owner)

    def described_table(self, address, symbol):
        """Follow the code addresses of a table that symbols.txt describes, and keep the bytes it spans."""
        layout = symbol.layout
        if layout.lists is not None:
            arrays = [address + self.mem.r16(address + 2 * number) for number in range(layout.lists)]
            self.described.append((address, address + 2 * layout.lists))
        elif layout.pointers is not None:
            arrays = [orunners_rom.canonical_address(self.mem.r32(address + 4 * number))
                      for number in range(layout.pointers)]
            self.described.append((address, address + 4 * layout.pointers))
            if None in arrays:
                raise SystemExit(f"symbols.txt: {symbol.name} points outside the program ROM")
        else:
            arrays = [address]
        arrays = list(arrays)
        seen_arrays = set(arrays)
        while arrays:
            array = arrays.pop(0)
            record = array
            count = layout.count
            if layout.prefixed:
                count = self.mem.r32(array) if layout.prefix_bytes == 4 else self.mem.r16(array)
                if layout.prefix_in_bytes:
                    count //= layout.record
                record += layout.prefix_bytes
            for number in range(MAX_RECORDS):
                if count is not None and number == count:
                    break
                length = layout.record
                if layout.kinds is not None:
                    kind_offset, lengths = layout.kinds
                    kind = self.mem.r16(record + kind_offset)
                    if kind not in lengths:
                        raise SystemExit(f"symbols.txt: {symbol.name} record at {record:06X} is of kind "
                                         f"{kind:X}, which its layout does not list")
                    length = lengths[kind]
                    if length == 0:
                        record += kind_offset + 2
                        break
                    if length == FORK:
                        table = record + kind_offset + 2
                        first = orunners_rom.canonical_address(self.mem.r32(table))
                        branches = (first - table) // 4 if first is not None and first > table else 0
                        if not 1 <= branches <= 8:
                            raise SystemExit(f"symbols.txt: {symbol.name} fork at {record:06X} has no table of "
                                             f"arrays after it")
                        for number in range(branches):
                            branch = orunners_rom.canonical_address(self.mem.r32(table + 4 * number))
                            if branch not in seen_arrays:
                                seen_arrays.add(branch)
                                arrays.append(branch)
                        record = table + 4 * branches
                        break
                if layout.end is not None                         and (self.mem.r32(record) if layout.end_bytes == 4 else self.mem.r16(record)) == layout.end:
                    record += layout.end_bytes
                    break
                for offset, base in layout.code:
                    if offset + (4 if base is None else 2) > length:
                        continue   # a shorter kind of record has no such field
                    value = self.mem.r32(record + offset) if base is None else base + self.mem.r16(record + offset)
                    if value == 0:
                        continue   # no handler
                    target = orunners_rom.canonical_address(value)
                    if target is None or not any(start <= target < end for start, end in CODE_CHIPS):
                        raise SystemExit(f"symbols.txt: {symbol.name} record at {record:06X} holds "
                                         f"{value:08X} at +{offset}, which is not in the code ROM")
                    if base is None:
                        self.data_words[record + offset] = target
                    self.add_target(value, "func", record + offset)
                    self.described_targets.add(value & 0xFFFFFF)
                    self.table_targets.setdefault(symbol.name, set()).add(value & 0xFFFFFF)
                record += length
            else:
                raise SystemExit(f"symbols.txt: {symbol.name} array at {array:06X} has no end in sight")
            self.described.append((array, record))

    def settle(self, tried):
        """Follow every pending path, then stored code pointers, until nothing new turns up."""
        while True:
            while self.pending:
                address, origin = self.pending.popleft()
                self.trace(address, origin)
            # stored code pointers whose slot is read by an indirect call
            found = False
            for store, table, slot in self.table_writes:
                if slot not in self.slots or (table, store) in tried:
                    continue
                tried.add((table, store))
                self.pointer_table(table, True, store)
                found = found or bool(self.pending)
            for store, value, slot in self.pointer_writes:
                candidate = self.key(value)
                if slot not in self.slots or candidate is None or (candidate, store) in tried:
                    continue
                tried.add((candidate, store))
                if candidate in self.instructions:
                    # already decoded through a branch, but the indirect call enters there too
                    self.labels[candidate] = "func"
                elif self.trial(candidate):
                    self.add_target(candidate, "func", store)
                    found = True
                else:
                    self.rejected.append((candidate, store))
            if not found:
                return

    def trial(self, start, limit=4000):
        """Decode one function's local flow without committing it: clean if no invalid or overlapping code."""
        seen = set()
        stack = [start]
        while stack and len(seen) < limit:
            address = stack.pop()
            while address not in seen and address not in self.instructions:
                if address in self.owner:
                    return False
                instruction = dis.decode(self.mem, address)
                if instruction.flow == dis.FLOW_INVALID or not self.fits(address, instruction.length):
                    return False
                seen.add(address)
                if instruction.target is not None:
                    target = self.key(instruction.target)
                    if target is None:
                        return False
                    if instruction.flow in (dis.FLOW_BRANCH, dis.FLOW_JUMP):
                        stack.append(target)
                if instruction.flow in (dis.FLOW_JUMP, dis.FLOW_RETURN):
                    break
                address += instruction.length
        return True

    def branch_table(self, base, called, origin):
        """A run of 'br' or fixed 'jmp' entries (and 'nop' padding) indexed by a register."""
        cursor = self.key(base)
        if cursor is None:
            self.outside.append((origin, base))
            return
        self.labels.setdefault(cursor, "loc")
        entries = (self.call_tables if called else self.jump_tables).setdefault(origin, [])
        first = cursor
        for _ in range(1024):
            entry = dis.decode(self.mem, cursor)
            # the first entry may branch on a flag the code before the jump always leaves the same way
            # (0996A7: "be", Z set by the test that ended the run): it falls through to the second
            conditional = cursor == first and entry.flow == dis.FLOW_BRANCH and entry.mnemonic.startswith("b") \
                and entry.target is not None
            if entry.mnemonic == "br" or (entry.mnemonic == "jmp" and entry.target is not None) or conditional:
                self.add_target(cursor, "loc", origin, label=False)
                entries.append(cursor)
                if called:
                    target = self.key(entry.target)
                    if target is not None:
                        self.labels[target] = "func"
            elif entry.mnemonic == "nop":
                self.add_target(cursor, "loc", origin, label=False)
                entries.append(cursor)
            else:
                # the last case can be plain code right after the branches (seen at 0998B8), even code one of
                # the branches leads to as well: the game uses that index (00353C's, at 003548)
                if cursor in self.instructions or self.trial(cursor):
                    self.add_target(cursor, "func" if called else "loc", origin, label=False)
                    entries.append(cursor)
                return
            cursor += entry.length

    def pointer_table(self, base, called, origin):
        """A run of 32-bit code addresses indexed by a register; ends at the first entry that is not code."""
        cursor = self.key(base)
        if cursor is None:
            self.outside.append((origin, base))
            return
        self.labels.setdefault(cursor, "data")
        targets = self.pointer_tables.setdefault(origin, [])
        first_after = None      # the nearest target after the table: a table never runs into its own code
        for _ in range(512):
            if first_after is not None and cursor >= first_after:
                return
            word = self.mem.r32(cursor)
            if word >> 24:
                return          # an address on the 24-bit bus has nothing in its top byte
            target = self.key(word)
            if target is None or not self.trial(target):
                return
            if target > cursor and (first_after is None or target < first_after):
                first_after = target
            self.data_words[cursor] = target
            targets.append(target)
            self.add_target(target, "func" if called else "loc", origin)
            cursor += 4

    def trace(self, address, origin):
        previous = None
        self.table_loads = {}
        while True:
            if address in self.instructions:
                return
            if address in self.owner:
                self.overlaps.append((address, self.owner[address], origin))
                return
            instruction = dis.decode(self.mem, address)
            last_byte = address + instruction.length - 1
            if instruction.flow == dis.FLOW_INVALID or not self.fits(address, instruction.length):
                self.invalid.append((address, instruction.text, origin))
                return
            taken = [a for a in range(address, last_byte + 1) if a in self.owner]
            if taken:
                self.overlaps.append((address, self.owner[taken[0]], origin))
                return

            self.instructions[address] = instruction
            for byte_address in range(address, last_byte + 1):
                self.owner[byte_address] = address
            self.follow(instruction, previous)
            self.note_table_load(instruction)
            if instruction.flow in (dis.FLOW_JUMP, dis.FLOW_RETURN):
                return
            previous = instruction
            address += instruction.length

    def follow(self, instruction, previous):
        kind = "func" if instruction.flow == dis.FLOW_CALL else "loc"
        first = instruction.operands[0] if instruction.operands else None
        pushed = self.pushed_return(previous)
        if instruction.flow == dis.FLOW_JUMP and instruction.target is not None and pushed is not None:
            # "movea back[PC], [-SP]" + "br func": a call to func that returns to 'back'
            self.add_target(pushed, "loc", previous.address)
            self.add_target(instruction.target, "func", instruction.address)
            self.push_jumps[instruction.address] = self.key(pushed)
        elif instruction.target is not None:
            self.add_target(instruction.target, kind, instruction.address)
        elif instruction.target_pointer is not None:
            pointer = self.key(instruction.target_pointer)
            if pointer is None:
                self.unresolved.append((instruction.address, instruction.text))
            else:
                self.labels.setdefault(pointer, "data")
                self.add_target(self.mem.r32(pointer), kind, instruction.address)
        elif instruction.mnemonic in ("jmp", "jsr") and first.mode in ("pc_disp_indexed", "direct_indexed"):
            # a jump through a table right after pushing a return address calls the entries (seen at 099881)
            called = instruction.mnemonic == "jsr" or pushed is not None
            self.branch_table(first.value, called, instruction.address)
        elif instruction.mnemonic in ("jmp", "jsr") and first.mode == "reg_indirect" \
                and (self.loads_table_entry(previous, first.reg) or first.reg in self.table_loads):
            table = previous.operands[0].value if self.loads_table_entry(previous, first.reg) \
                else self.table_loads[first.reg]
            self.pointer_table(table, instruction.mnemonic == "jsr", instruction.address)
        elif instruction.mnemonic in ("jmp", "jsr", "call"):
            self.unresolved.append((instruction.address, instruction.text))
            if first is not None and first.mode == "disp_indirect":
                self.slots.add(first.disp)

        # code pointer stores: "movea.b func[PC], slot", "movea.b func, slot" or "mov.w #func, slot"
        if len(instruction.operands) == 2 and instruction.flow == dis.FLOW_NORMAL:
            source, dest = instruction.operands
            if dest.mode == "disp":
                if instruction.mnemonic.startswith("movea") and source.mode in ("pc_disp", "direct"):
                    self.pointer_writes.append((instruction.address, source.value, dest.disp))
                elif instruction.mnemonic == "mov.w" and source.mode == "immediate" and not source.value >> 24:
                    # (a word with its top byte set is data, not a code address: "mov.w #F800F800, 4[R27]")
                    self.pointer_writes.append((instruction.address, source.value, dest.disp))
                elif instruction.mnemonic == "mov.w" and source.mode == "pc_disp_indexed":
                    self.table_writes.append((instruction.address, source.value, dest.disp))

        # "movea target[PC], [-SP]" right before a return: the return lands on target
        if instruction.flow == dis.FLOW_RETURN and pushed is not None:
            self.add_target(pushed, "loc", previous.address)
            self.return_jumps[instruction.address] = self.key(pushed)

        # any "movea target[PC], [-SP]" pushes a place a later return may land on (a loop head, say);
        # it could also be a data pointer passed on the stack, so it has to decode cleanly first
        own = self.pushed_return(instruction)
        if own is not None:
            candidate = self.key(own)
            if candidate is not None and (candidate in self.instructions or self.trial(candidate)):
                self.add_target(own, "loc", instruction.address)

    def note_table_load(self, instruction):
        """Keeps which registers hold an entry of a PC-relative table: set by "mov.w table[PC](Rx), reg",
        forgotten when the register is written again or a call may change it"""
        if instruction.flow == dis.FLOW_CALL:
            self.table_loads.clear()
            return
        if not instruction.operands:
            return
        dest = instruction.operands[-1]
        if dest.mode != "reg":
            return
        if self.loads_table_entry(instruction, dest.reg):
            self.table_loads[dest.reg] = instruction.operands[0].value
        else:
            self.table_loads.pop(dest.reg, None)

    @staticmethod
    def pushed_return(previous):
        """The address a "movea target[PC], [-SP]" pushes, or None for any other instruction."""
        if previous is None or not previous.mnemonic.startswith("movea") or len(previous.operands) != 2:
            return None
        source, dest = previous.operands
        if source.mode == "pc_disp" and dest.mode == "autodec" and dest.reg == STACK_POINTER:
            return source.value
        return None

    @staticmethod
    def loads_table_entry(previous, reg):
        """True for "mov.w table[PC](Rx), reg": a 32-bit read from a PC-relative table into reg."""
        if previous is None or previous.mnemonic != "mov.w" or len(previous.operands) != 2:
            return False
        source, dest = previous.operands
        return source.mode == "pc_disp_indexed" and dest.mode == "reg" and dest.reg == reg

    def label_name(self, address):
        if address in self.symbols:
            return self.symbols[address].name
        kind = self.labels.get(address)
        if kind is None:
            return None
        prefix = {"func": "func", "loc": "loc", "data": "data"}[kind]
        return f"{prefix}_{address:06X}"

    def target_comment(self, instruction):
        target = instruction.target
        if target is None and instruction.target_pointer is not None:
            target = instruction.target_pointer
        if target is None:
            return ""
        canonical = orunners_rom.canonical_address(target)
        name = self.label_name(canonical) if canonical is not None else None
        return f"  ; {name}" if name else ""


def write_listing(analysis, path):
    image = analysis.image
    with open(path, "w", encoding="ascii", newline="\n") as out:
        out.write("; OutRunners (orunners, World) - V70 program\n")
        out.write("; Generated by tools/disassemble.py from the user's own ROM. Do not redistribute.\n")
        for start, end in orunners_rom.PROGRAM_AREAS:
            out.write(f"\n; ======== {start:06X}-{end - 1:06X} ========\n")
            address = start
            while address < end:
                label = analysis.label_name(address)
                if label:
                    out.write(f"\n{label}:\n")
                instruction = analysis.instructions.get(address)
                if instruction:
                    raw = image[address:address + min(instruction.length, 10)].hex(" ")
                    if instruction.length > 10:
                        raw += " .."
                    comment = analysis.target_comment(instruction)
                    out.write(f"    {address:06X}  {raw:<32} {instruction.text}{comment}\n")
                    address += instruction.length
                    continue
                if address in analysis.data_words:
                    value = int.from_bytes(image[address:address + 4], "little")
                    name = analysis.label_name(analysis.data_words[address])
                    out.write(f"    {address:06X}  .long {value:08X}{'':<38} ; {name}\n")
                    address += 4
                    continue
                stop = address + 1
                while stop < end and stop - address < 16 and stop not in analysis.instructions \
                        and stop not in analysis.data_words and analysis.label_name(stop) is None:
                    stop += 1
                chunk = image[address:stop]
                text = "".join(chr(b) if 0x20 <= b < 0x7F else "." for b in chunk)
                values = ",".join(f"{b:02X}" for b in chunk)
                out.write(f"    {address:06X}  .byte {values:<47} ; {text}\n")
                address = stop


def write_report(analysis, path):
    lines = ["V70 program analysis", "====================", ""]
    total_code = 0
    total_size = 0
    for start, end in orunners_rom.PROGRAM_AREAS:
        code = sum(1 for a in range(start, end) if a in analysis.owner)
        total_code += code
        total_size += end - start
        lines.append(f"{start:06X}-{end - 1:06X}: {code:7} of {end - start:7} bytes decoded as code "
                     f"({100 * code / (end - start):5.1f}%)")
    lines.append(f"total:         {total_code:7} of {total_size:7} bytes ({100 * total_code / total_size:5.1f}%)")
    functions = sum(1 for kind in analysis.labels.values() if kind == "func")
    lines += [
        "",
        f"instructions:        {len(analysis.instructions)}",
        f"functions:           {functions}",
        f"code labels:         {sum(1 for kind in analysis.labels.values() if kind == 'loc')}",
        f"unresolved jumps:    {len(analysis.unresolved)}",
        f"invalid paths:       {len(analysis.invalid)}",
        f"overlapping paths:   {len(analysis.overlaps)}",
        f"targets outside ROM: {len(analysis.outside)}",
    ]
    if analysis.executed:
        confirmed = sum(1 for address in analysis.executed if address in analysis.instructions)
        lines += [
            "",
            f"trace: executed addresses        {len(analysis.executed)}",
            f"trace: found by static analysis  {confirmed - analysis.trace_added}",
            f"trace: added from the trace      {analysis.trace_added}",
            f"trace: conflicts                 {len(analysis.trace_conflicts)}",
            f"trace: outside the program ROM   {analysis.trace_outside}",
        ]
    sections = [
        ("Unresolved jumps and calls (target comes from registers or RAM)", analysis.unresolved,
         lambda item: f"{item[0]:06X}  {item[1]}"),
        ("Invalid instructions reached", analysis.invalid,
         lambda item: f"{item[0]:06X}  {item[1]:<30} reached from {item[2]:06X}"),
        ("Overlapping instructions", analysis.overlaps,
         lambda item: f"{item[0]:06X}  inside instruction at {item[1]:06X}, reached from {item[2]:06X}"),
        ("Targets outside the program ROM", analysis.outside,
         lambda item: f"{item[0]:06X}  -> {item[1]:08X}"),
        ("Stored code pointers rejected by the trial decode", analysis.rejected,
         lambda item: f"{item[0]:06X}  stored at {item[1]:06X}"),
        ("Executed addresses inside a statically decoded instruction", analysis.trace_conflicts,
         lambda item: f"{item[0]:06X}  inside instruction at {item[1]:06X}"),
    ]
    for title, items, render in sections:
        if items:
            lines += ["", title, "-" * len(title)]
            lines += [render(item) for item in sorted(items)]
    path.write_text("\n".join(lines) + "\n", encoding="ascii")
    return lines


def main():
    arguments = sys.argv[1:]
    trace_paths = []
    while "--trace" in arguments:
        position = arguments.index("--trace")
        if position + 1 >= len(arguments):
            raise SystemExit("Usage: python tools/disassemble.py [rom folder or .zip] [--trace executed.txt ...]")
        trace_paths.append(arguments[position + 1])
        del arguments[position:position + 2]
    source = Path(arguments[0]) if arguments else ROOT / "rom"

    image = orunners_rom.build_program(orunners_rom.load_set(source))
    executed, outside = load_trace(trace_paths)
    analysis = Analysis(image, load_symbols(ROOT / "symbols.txt"), executed)
    analysis.trace_outside = outside
    analysis.run()

    out_dir = ROOT / "build" / "listing"
    out_dir.mkdir(parents=True, exist_ok=True)
    write_listing(analysis, out_dir / "orunners.s")
    report = write_report(analysis, out_dir / "report.txt")
    # print everything up to the first detailed section (a title underlined with dashes)
    end = next((index for index in range(3, len(report) - 1) if report[index + 1].startswith("-")), len(report))
    print("\n".join(report[:end]).rstrip())
    print(f"\nListing: {out_dir / 'orunners.s'}\nReport:  {out_dir / 'report.txt'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
