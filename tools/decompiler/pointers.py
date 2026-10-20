"""
Name:        pointers.py
Description: works out which registers point at which of the game's structures, so that a memory access
             through them reads as a field: OBJECT(r20)->unk_30 instead of mem_read32(r20 + 0x30u)
Version:     0.1.0
Created:     12-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

The structures (STRUCTS) are the game objects (0xA0 bytes in work RAM, run by the object loop with R20
pointing at each) and the player blocks (0x200 bytes from 0x208400, R18 pointing at the current one).
A kind is (structure, offset): a pointer to a structure, or into it; or ("global", address): a pointer
to a variable in work RAM (R21 holds the free object stack at 0x20E900), so that what is reached through
it reads as that variable.

Fields and variables symbols.txt says point at a structure (a struct name after their type) give
their kind to what is read from them: a u32 holds the address, a u16 the low half of it, which the code
puts in the work RAM page ("(r & 0xFFFF0000) | halfword"). An access through any expression of a known
kind reads as a field, not only through a register.

A kind may also say which family's structure it points into ((struct, offset, family): object.eggman_scene
in symbols.txt), so that what is read through it reads as that family's code reads its own object (its
scoped fields, emit.py). The family comes from what symbols.txt says a field, variable, parameter or local
points at, and from the function's own name: the object a function of a family runs on (R20 and R23 when
it starts) is that family's. Two paths, callers or returns that say two families of one structure say
the structure alone.

Pointers into the program ROM (REGIONS: "rom") have a kind of their own, without a structure: an address
from 0x1000 on in the ROM, plus any offset, or a word read from a field or variable symbols.txt says points
into the ROM ("rom" after its type). What is read through one reads the ROM itself (ROM16(p)) rather than
the board's bus; rom() stops the program if such an address ever falls outside the ROM.

Forward over each function's blocks, a register keeps the kind of what it was set to:
  - another register, or one plus a constant (a pointer into the same structure);
  - "(r & 0xFFFF0000) | halfword": the game keeps structures in the 0x20xxxx page and links them with
    16-bit addresses, so this is another structure of the same kind;
  - 0x208400 plus a multiple of 0x200: a player (0x2084CC plus one: its field CC);
  - a word read from a variable that holds a pointer (GLOBAL_KINDS: the ends of the object list);
  - 0x208000 (R25) plus a word read from a table of offsets symbols.txt says lead to a structure.
Across functions: what a function finds in its registers when it starts is what its callers have there
(through calls of any kind), and what a call leaves in the registers it hands back is what the callee
has there when it returns. A caller that does not know says nothing; two that know different things make
it unknown. The context registers start as the game keeps them (CONTEXT_KINDS: R20, R22, R23 and R24 an
object, R18 a player) unless the callers say otherwise. Inside a function, where paths meet, a kind
holds only if it is the same on all of them. Repeated until it settles.

This only decides how accesses are written, never what they do: the C reads and writes the same bytes.

The structures, places and rules above are the V60 program's. Another program sets its own in the module's
names below (tools/decompile_sound.py: the Z80's sound channels in its shared RAM, 16-bit pointers).
"""
from v60 import ir

STRUCTS = {"object": 0xA0, "player": 0x200}
VARIABLES = 0x208000          # where R25 points: the game's variables
PLAYERS = (0x208400, 0x200, 3)    # first, size, count
# (struct, offset) or ("global", address) -> (struct, bits): what a field or variable points at (Program)
TARGETS = {}
# (struct, family, offset) -> (what it points at, bits): a field one family of functions uses as a pointer;
# ("global", family, address) the same for a variable of the work RAM the family uses its own way
SCOPED_TARGETS = {}
GLOBAL_FAMILY = None          # the family whose own names of the work RAM the function being analysed uses
CONTEXT_KINDS = {20: ("object", 0), 22: ("object", 0), 23: ("object", 0), 24: ("object", 0), 18: ("player", 0)}
OBJECT_REGISTERS = (20, 22, 23, 24)    # what the game keeps here it reads as objects, wherever it put them
OWN_REGISTERS = (20, 23)      # what points at the object a function runs on when it starts (the object loop's)
REGIONS = {"rom"}             # kinds that are a memory, not a structure
OFFSET = ("offset", 0)        # not a pointer: a value below 0x10000 (a halfword read, a mask...), an offset
CONFLICT = ("conflict", 0)    # callers that know different things: nothing is known
UNKNOWN = ("unknown", 0)      # a caller that does not know
ROUNDS = 40
WORK_RAM = (0x200000, 0x210000)
ADDRESS_MASK = 0xFFFFFF
POINTER_BITS = 32              # a value read at this width holds a whole address
PAGE_LINKS = True              # structures linked by the low halves of their addresses, in the work RAM page
ROM_DATA = (0x1000, 0x200000)  # where the program ROM's data can be (the vectors and first code are below)
ARRAYS = {}                    # struct -> (first, count): structures one after another, so that a pointer
                               # moved by the size of one points at the next (PLAYERS has its own rules)
STRUCT_FUNCTION = "ram"        # what gives a pointer to a structure from its address (game/support.h)
ROM_STRUCTS = set()            # structures that are records of the program ROM: read through ROM_FUNCTION
ROM_FUNCTION = "rom"           # ... and what gives a pointer to one of those (const)
CONSTANTS = False              # registers keep the constants they are set to, so that a pointer moved by one
                               # (the Z80's "ld de,12h ... add iy,de") keeps its kind
PAGED = False                  # structures a page long, one per page: (p & FF00) | n is field n of p's (page_parts)
RUNS = set()                   # structures laid one after another wherever a pointer starts (no fixed base,
                               # unlike ARRAYS): a pointer moved past one points into the next


def link_kind(link, offset=0):
    """The kind of a pointer to what 'link' names (symbols.txt): STRUCT, STRUCT.FAMILY or a region."""
    struct, _, family = link.partition(".")
    return (struct, offset, family) if family else (struct, offset)


def family_of(kind):
    """The family of the structure a kind points into, or None."""
    return kind[2] if kind is not None and len(kind) > 2 else None


def paged(kind):
    """True for a pointer into a structure a page long whose array starts on a page (the Z80's sequence
    players): its high byte picks the structure, its low byte the field."""
    return PAGED and kind is not None and kind[0] in STRUCTS and STRUCTS[kind[0]] == 0x100 \
        and kind[0] in ARRAYS and ARRAYS[kind[0]][0] & 0xFF == 0


def is_page_mask(node):
    return isinstance(node, ir.Op) and node.name == "and" and isinstance(node.args[1], ir.Const) \
        and node.args[1].value == 0xFF00 and node.bits == 16


def page_parts(node, state, zeros):
    """(structure, field offset, the structure's address) for a pointer into a paged structure set a byte
    at a time: p & FF00 (the structure), (p & FF00) | n ("ld l,n"), (p & FF00) | (p + n) & FF ("inc l"), or
    q | n with q a structure's own address; None otherwise."""
    if is_page_mask(node):
        inner = kind_of(node.args[0], state, zeros)
        return (inner[0], 0, node) if paged(inner) else None
    if isinstance(node, ir.Op) and node.name == "shl" and node.bits == 16 and isinstance(node.args[1], ir.Const) \
            and node.args[1].value == 8:
        # the next page's structure ("inc h; ld l,0"): ((p >> 8) + n) << 8
        page = node.args[0]
        while isinstance(page, ir.Op) and page.name in ("zext", "fits", "trunc"):
            page = page.args[0]
        if isinstance(page, ir.Op) and page.name == "add" and isinstance(page.args[1], ir.Const):
            high = page.args[0]
            while isinstance(high, ir.Op) and high.name in ("zext", "fits", "trunc"):
                high = high.args[0]
            if isinstance(high, ir.Op) and high.name == "shr" and isinstance(high.args[1], ir.Const) \
                    and high.args[1].value == 8:
                inner = kind_of(high.args[0], state, zeros)
                if paged(inner):
                    return inner[0], 0, node
        return None
    if not (isinstance(node, ir.Op) and node.name == "or" and node.bits == 16):
        return None
    for kept, low in (node.args, tuple(reversed(node.args))):
        if is_page_mask(kept):
            inner = kind_of(kept.args[0], state, zeros)
            if not paged(inner):
                continue
            if isinstance(low, ir.Const) and low.value < 0x100:
                return inner[0], low.value, kept
            byte = low
            while isinstance(byte, ir.Op) and byte.name in ("zext", "fits", "trunc"):
                byte = byte.args[0]
            if isinstance(byte, ir.Op) and byte.name == "add" and isinstance(byte.args[1], ir.Const):
                moved = byte.args[0]
                while isinstance(moved, ir.Op) and moved.name in ("zext", "fits", "trunc"):
                    moved = moved.args[0]
                if moved == kept.args[0]:
                    return inner[0], (inner[1] + byte.args[1].value) & 0xFF, kept
        elif isinstance(low, ir.Const) and low.value < 0x100:
            inner = kind_of(kept, state, zeros)
            if paged(inner) and inner[1] == 0:
                return inner[0], low.value, kept
    return None


def player_constant(value):
    first, size, count = PLAYERS
    if first <= value < first + size * count:
        return "player", (value - first) % size
    for struct, (start, number) in ARRAYS.items():
        size = STRUCTS[struct]
        if start <= value < start + size * number:
            return struct, (value - start) % size
    return None


def in_work_ram(value):
    return WORK_RAM[0] <= value < WORK_RAM[1]


def merge(a, b):
    """Kinds two paths agree on. A pointer to a variable in the work RAM on one and to a structure on the
    other points at that structure: a list's head is reached as the link field of a structure that is not
    there (check_zones starts its walk of hitter_list at 0x20F016, whose field 68, hit_list_next, is
    hitter_list), and the same register then walks the structures it links."""
    found = {}
    for index, kind in a.items():
        other = b.get(index)
        if other == kind:
            found[index] = kind
        elif other is not None and kind[:2] == other[:2] and kind[0] in STRUCTS:
            found[index] = kind[:2]    # one structure, of two families (or of one and of none): of none
        elif other is not None and {kind[0], other[0]} & {"global"}:
            pointer, structure = (kind, other) if kind[0] == "global" else (other, kind)
            if structure[0] in STRUCTS and structure[1] == 0 and in_work_ram(pointer[1]):
                found[index] = structure
    return found


def gather(a, b):
    """What two callers (or returns) say together: CONFLICT where they know different things. One that
    does not know leaves what the other knows about a structure (a guess that only names fields), but not
    about a variable's address, which the C would put in place of the register: that has to be certain."""
    found = dict(a)
    for index, kind in b.items():
        old = found.get(index)
        if old is None or old == CONFLICT:
            if old is None:
                found[index] = kind
            continue
        if old == kind:
            continue
        if UNKNOWN in (old, kind):
            other = kind if old == UNKNOWN else old
            found[index] = CONFLICT if other[0] == "global" else other
        elif old[:2] == kind[:2] and old[0] in STRUCTS:
            found[index] = old[:2]    # one structure, of two families: of none
        else:
            found[index] = CONFLICT
    return found


def outward(kind):
    """What a function tells its callees and callers about a register: a pointer into the ROM, or an offset,
    stays inside the function (a constant that happens to be a ROM address is not news to anyone)."""
    return UNKNOWN if kind[0] in REGIONS or kind == OFFSET or kind[0] == "const" else kind


def held_constant(node, state):
    """A constant 'node' is or a register holds (CONSTANTS), as a Const; else 'node'."""
    if CONSTANTS and isinstance(node, ir.Reg) and state is not None:
        kind = state.get(node.index)
        if kind is not None and kind[0] == "const":
            return ir.Const(kind[1], node.bits)
    return node


def known(kinds):
    return {index: kind for index, kind in kinds.items() if kind not in (CONFLICT, UNKNOWN)}


def signed(offset):
    offset &= (1 << POINTER_BITS) - 1
    return offset - (1 << POINTER_BITS) if offset >= 1 << (POINTER_BITS - 1) else offset


def low_zeros(node, zeros):
    """How many low bits of 'node' are known to be zero ('zeros': the same for registers)."""
    if isinstance(node, ir.Const):
        value = node.value
        return (value & -value).bit_length() - 1 if value else node.bits
    if isinstance(node, ir.Reg):
        return zeros.get(node.index, 0)
    if not isinstance(node, ir.Op):
        return 0
    name, args = node.name, node.args
    if name == "shl" and isinstance(args[1], ir.Const):
        return min(node.bits, low_zeros(args[0], zeros) + args[1].value)
    if name == "mul":
        return min(node.bits, low_zeros(args[0], zeros) + low_zeros(args[1], zeros))
    if name == "and":
        return max(low_zeros(args[0], zeros), low_zeros(args[1], zeros))
    if name in ("or", "xor", "add", "sub"):
        return min(low_zeros(args[0], zeros), low_zeros(args[1], zeros))
    if name in ("zext", "trunc", "fits"):
        return min(node.bits, low_zeros(args[0], zeros))
    return 0


def kind_of(node, state, zeros):
    """The kind of the value 'node' computes, or None."""
    if isinstance(node, ir.Reg):
        return state.get(node.index)
    if isinstance(node, ir.Temp):
        return state.get(("temp", node.name))    # (PAGED only: the Z80's EX DE,HL goes through one)
    if isinstance(node, ir.Const):
        value = node.value & ADDRESS_MASK
        found = player_constant(value)
        if found is None and in_work_ram(value):
            found = ("global", value)
        if found is None and rom_address(value):
            found = ("rom", 0)
        return found
    if isinstance(node, ir.Load):
        if node.bits == POINTER_BITS:
            target = target_of(node.address, state, POINTER_BITS)
            return link_kind(target) if target is not None else None
        return None
    if not isinstance(node, ir.Op):
        return None
    name, args = node.name, node.args
    if PAGED and name in ("and", "or", "shl"):
        found = page_parts(node, state, zeros)
        if found is not None:
            return found[0], found[1]
    if CONSTANTS and name == "add":
        args = tuple(held_constant(arg, state) for arg in args)
    if name in ("add", "sub"):
        # a pointer into the ROM moved by an offset: something that cannot be an address itself (below
        # 0x10000), so that a RAM pointer plus a displacement that happens to be a ROM address stays unknown
        kinds = [kind_of(arg, state, zeros) if not isinstance(arg, ir.Const) or name == "add" else None
                 for arg in args]
        if kinds.count(("rom", 0)) == 1 and all(kind in (("rom", 0), OFFSET) or (kind is None and offset_sized(arg, state))
                                                 for kind, arg in zip(kinds, args)):
            return "rom", 0
    if name in ("add", "sub") and isinstance(args[1], ir.Const):
        delta = args[1].value if name == "add" else -args[1].value
        inner = kind_of(args[0], state, zeros)
        if inner is not None and inner[0] == "global":
            value = (inner[1] + delta) & ADDRESS_MASK
            return ("global", value) if in_work_ram(value) else None
        if inner is not None and inner[0] in STRUCTS:
            offset = signed(inner[1] + delta)
            if 0 <= offset < STRUCTS[inner[0]]:
                return (inner[0], offset) + tuple(inner[2:])
            if inner[0] in RUNS or (inner[0] in ARRAYS and offset >= 0):
                return inner[0], offset % STRUCTS[inner[0]]    # into the next (or the one before) of the array
            return None
    if name == "add" and isinstance(args[0], ir.Const) and args[0].value & ADDRESS_MASK == VARIABLES \
            and isinstance(args[1], ir.Load) and args[1].bits == 32:
        # the variables' base (R25) plus an offset read from a table symbols.txt says leads to a structure
        # (player_block_offsets: 0x400, 0x600, 0x800)
        target = target_of(args[1].address, state, 32)
        if target is not None and link_kind(target)[0] in STRUCTS:
            return link_kind(target)
    if name == "add" and isinstance(args[0], ir.Const):
        # a player's block, or a place in it: the first one's plus a multiple of the block size
        found = player_constant(args[0].value & ADDRESS_MASK)
        if found is not None and found[0] == "player" and args[0].value & ADDRESS_MASK < PLAYERS[0] + PLAYERS[1] \
                and low_zeros(args[1], zeros) >= PLAYERS[1].bit_length() - 1:
            return found
        return None
    if name == "or" and PAGE_LINKS:
        kept, low = args
        page = isinstance(kept, ir.Const) and (kept.value & 0xFFFF) == 0 and in_work_ram(kept.value)
        masked = isinstance(kept, ir.Op) and kept.name == "and" and isinstance(kept.args[1], ir.Const) \
            and kept.args[1].value == 0xFFFF0000
        if page or masked:
            # a link read from a field or variable that holds one: what it links, whatever the register
            # held before (the player's object read into the player register)
            link = low
            while isinstance(link, ir.Op) and link.name in ("zext", "fits"):
                link = link.args[0]
            if isinstance(link, ir.Load) and link.bits == 16:
                target = target_of(link.address, state, 16)
                if target is not None:
                    return link_kind(target)
        if masked and isinstance(kept.args[0], ir.Reg):
            inner = state.get(kept.args[0].index)
            if inner is not None and inner[0] in STRUCTS:
                return inner[0], 0
            if inner == ("rom", 0):
                return inner    # a pointer into the ROM moved within its page
    return None


def offset_sized(node, state=None):
    """True for a value that is surely below 0x10000: a constant, a byte or halfword (read, extended or
    cut), one masked to 16 bits, a register that holds one (OFFSET), or such a value times a small
    constant."""
    if isinstance(node, ir.Const):
        return node.value < 0x10000 or node.value >= 0xFFFF0000    # a small displacement either way
    if isinstance(node, ir.Reg):
        kind = state.get(node.index) if state is not None else None
        return kind == OFFSET or (kind is not None and kind[0] == "const")
    if isinstance(node, ir.Load):
        return node.bits <= 16
    if not isinstance(node, ir.Op):
        return False
    name, args = node.name, node.args
    if name in ("zext", "fits", "trunc"):
        return (args[0].bits if name == "zext" else node.bits) <= 16 or offset_sized(args[0], state)
    if name == "and":
        return any(isinstance(arg, ir.Const) and arg.value < 0x10000 for arg in args)
    if name in ("mul", "shl") and isinstance(args[1], ir.Const) and args[1].value <= (8 if name == "mul" else 3):
        return offset_sized(args[0], state)
    if name == "or":
        return all(offset_sized(arg, state) for arg in args)
    if name in ("shr", "sar") and isinstance(args[1], ir.Const):
        return args[1].value >= 16 or offset_sized(args[0], state)
    if name in ("add", "sub"):
        return all(offset_sized(arg, state) for arg in args)    # an offset into a record or a table, either way
    return node.bits <= 16


def rom_address(value):
    """An address in the program ROM that can be data (the vectors and the first code are below 0x1000)."""
    return ROM_DATA[0] <= value < ROM_DATA[1]


def constant_base(address):
    """The constant of "constant + rest", or of a constant address."""
    if isinstance(address, ir.Const):
        return address.value & ADDRESS_MASK
    if isinstance(address, ir.Op) and address.name == "add":
        for arg in address.args:
            if isinstance(arg, ir.Const):
                return arg.value & ADDRESS_MASK
    return None


def target_of(address, state, bits):
    """What a value of 'bits' read at 'address' points at (TARGETS, SCOPED_TARGETS: STRUCT, STRUCT.FAMILY
    or a region), or None."""
    hit = access(address, state)
    if hit is not None and hit[0] in REGIONS:
        hit = None    # a table in the ROM: what symbols.txt says its elements lead to, by its address
    if hit is not None and hit[0] != "global":
        key = (hit[0], hit[1])
        scoped = SCOPED_TARGETS.get((hit[0], hit[4], hit[1])) if hit[4] is not None else None
        if scoped is not None:
            return scoped[0] if scoped[1] == bits else None    # the family's own use of the field
    else:
        base = constant_base(hit[1] if hit is not None else address)
        key = ("global", base) if base is not None else None
        scoped = SCOPED_TARGETS.get(("global", GLOBAL_FAMILY, base)) if GLOBAL_FAMILY is not None else None
        if scoped is not None:
            return scoped[0] if scoped[1] == bits else None    # the family's own use of the variable
    found = TARGETS.get(key) if key is not None else None
    return found[0] if found is not None and found[1] == bits else None


def access(address, state):
    """(struct, offset of the field, base register, how far into the struct the base points, the family of
    the structure or None) for a memory access at 'address', or None. Through a pointer to a variable:
    ("global", the address with the variable's address in place of the register)."""
    globals_ = {index: kind[1] for index, kind in state.items() if kind[0] == "global"}
    if globals_:
        def change(node):
            if isinstance(node, ir.Reg) and node.index in globals_:
                return ir.Const(globals_[node.index])
            return None
        rebuilt = ir.rebuild(address, change)
        if rebuilt != address:
            folded = ir.rebuild(rebuilt, fold)
            return ("global", folded)
    whole = kind_of(address, state, {})
    if whole is not None and whole[0] in REGIONS:
        return whole[0], 0, address, 0, None
    if PAGED:
        found = page_parts(address, state, {})
        if found is not None:
            return found[0], found[1], found[2], 0, None
    if isinstance(address, ir.Reg):
        kind = state.get(address.index)
        return (kind[0], kind[1], address, kind[1], family_of(kind)) if kind is not None and kind[0] in STRUCTS \
            else None
    base, displacement = address, 0
    if isinstance(address, ir.Op) and address.name in ("add", "sub") and isinstance(address.args[1], ir.Const):
        base = address.args[0]
        displacement = address.args[1].value if address.name == "add" else -address.args[1].value
    if isinstance(base, ir.Reg):
        kind = state.get(base.index)
    elif isinstance(base, ir.Op) and (base.name == "or" or base.name == "add" and isinstance(base.args[0], ir.Const)):
        kind = kind_of(base, state, {})
    elif isinstance(base, ir.Load):
        kind = kind_of(base, state, {})    # a pointer read where it is used (22[4[R20]])
    else:
        kind = None
    if kind is None or kind[0] in ("global", "const") or kind[0] in REGIONS or kind == OFFSET:
        return None
    offset = signed(kind[1] + displacement)
    if 0 <= offset < STRUCTS[kind[0]]:
        return kind[0], offset, base, kind[1], family_of(kind)
    return None


def fold(node):
    """Constant additions in an address."""
    if isinstance(node, ir.Op) and node.name == "add":
        left, right = node.args
        if isinstance(left, ir.Const) and isinstance(right, ir.Const):
            return ir.Const((left.value + right.value) & 0xFFFFFFFF, node.bits)
        if isinstance(left, ir.Op) and left.name == "add" and isinstance(right, ir.Const) \
                and isinstance(left.args[0], ir.Const):
            return ir.op("add", (ir.Const((left.args[0].value + right.value) & 0xFFFFFFFF), left.args[1]), node.bits)
    return None


class Program:
    """The kinds of every function's registers, across calls."""

    def __init__(self, functions, flow, signatures, call_results, context, targets=None, hints=None, site_hints=None,
                 scoped_targets=None, families=None, global_families=None):
        TARGETS.clear()
        TARGETS.update(targets or {})
        SCOPED_TARGETS.clear()
        SCOPED_TARGETS.update(scoped_targets or {})
        self.families = families or {}       # entry -> the family of the object the function runs on (by its name)
        self.global_families = global_families or {}    # entry -> the family of its own names of the work RAM
        self.hints = hints or {}             # entry -> {register: kind}: what symbols.txt says a parameter is
        self.site_hints = site_hints or {}   # (instruction address, register) -> kind: what symbols.txt says a
                                             # local variable written there is
        self.functions = functions
        self.flow = flow
        self.signatures = signatures
        self.call_results = call_results     # entry -> {id(call statement): registers it hands back}
        self.context = set(context)
        self.entry_kinds = {}                # entry -> kinds every caller agrees on
        self.exit_kinds = {}                 # entry -> kinds of what it hands back, at every return
        self.changed = set()

    # ---- across functions ----

    def run(self):
        pending = set(self.functions)
        for _ in range(ROUNDS):
            if not pending:
                break
            self.changed = set()
            for entry in sorted(pending):
                self.analyse(entry, record=None)
            pending = self.changed
        fields, accesses = {}, []
        for entry in self.functions:
            found = {}
            self.analyse(entry, record=(found, accesses))
            fields[entry] = found
        return fields, accesses

    def tell_callee(self, target, state):
        state = {index: outward(kind) for index, kind in state.items()}
        merged = gather(self.entry_kinds.get(target, {}), state)
        if merged != self.entry_kinds.get(target):
            self.entry_kinds[target] = merged
            self.changed.add(target)

    def tell_callers(self, entry, kinds):
        kinds = {index: outward(kind) for index, kind in kinds.items()}
        merged = gather(self.exit_kinds.get(entry, {}), kinds)
        if merged == self.exit_kinds.get(entry):
            return
        self.exit_kinds[entry] = merged
        # every function that calls it has to look again
        self.changed |= self.callers.get(entry, set())

    @property
    def callers(self):
        if not hasattr(self, "_callers"):
            found = {}
            for entry, function in self.functions.items():
                for block in function.blocks.values():
                    for statement in block.statements:
                        if isinstance(statement, ir.Call):
                            for target in self.flow.call_targets(statement):
                                found.setdefault(target, set()).add(entry)
                    if isinstance(block.end, ir.TailCall):
                        found.setdefault(block.end.target, set()).add(entry)
            self._callers = found
        return self._callers

    # ---- one function ----

    def told(self, target, state):
        """What a caller tells a callee about its registers: the parameters and the context registers, as it
        knows them. A parameter the callee only hands back as it found it (its data flow summary's base, and
        what it works out its other results from, do not read it) tells only of the structures the program
        keeps everywhere (CONTEXT_KINDS: objects, players): a record one caller leaves in R11 is not what
        allocate_object finds there when no object is free, called from anywhere else."""
        summary = self.flow.summaries.get(target)
        common = {kind[0] for kind in CONTEXT_KINDS.values()}
        found = {index: state.get(index, UNKNOWN) for index in self.context}
        for value in self.signatures[target].params:
            if value[0] != "r":
                continue
            kind = state.get(value[1], UNKNOWN)
            if summary is not None and kind[0] in STRUCTS and kind[0] not in common:
                reads = set(summary.base)
                for key, passed in summary.passes.items():
                    if key != value:
                        reads |= passed
                if not any(variable[0] == "r" and variable[1] == value[1] for variable in reads):
                    kind = UNKNOWN
            found[value[1]] = kind
        return found

    def handed_back(self, entry):
        """Registers a function changes for its caller: its results, and the context registers it writes."""
        results = {value[1] for value in self.signatures[entry].results if value[0] == "r"}
        written = {variable[1] for variable in self.flow.summaries[entry].writes if variable[0] == "r"}
        return results | (written & self.context)

    def after_call(self, targets, state, context_only=False):
        """The kinds after a call that may go to any of 'targets': a register a target writes has the kind
        that target leaves in it, one it does not write keeps the caller's. Through a slot only the
        context registers come back."""
        affected = set()
        for target in targets:
            affected |= self.handed_back(target)
        if context_only:
            affected &= self.context
        for index in affected:
            said = {}
            for target in targets:
                if index in self.handed_back(target):
                    kind = self.exit_kinds.get(target, {}).get(index, UNKNOWN)
                else:
                    kind = state.get(index, UNKNOWN)
                said = gather(said, {index: kind})
            kind = known(said).get(index)
            if kind is None and index in OBJECT_REGISTERS:
                kind = ("object", 0)    # the object registers hold objects after a call, whatever the callee did
            if kind is None:
                state.pop(index, None)
            else:
                state[index] = kind

    def step(self, statement, state, zeros, record):
        if PAGED and isinstance(statement, ir.SetTemp):
            kind = kind_of(statement.value, state, zeros)
            if kind is None:
                state.pop(("temp", statement.temp.name), None)
            else:
                state[("temp", statement.temp.name)] = kind
            return
        if isinstance(statement, ir.SetReg):
            kind = kind_of(statement.value, state, zeros)
            if CONSTANTS and isinstance(statement.value, ir.Const) and kind is None:
                kind = ("const", statement.value.value & ADDRESS_MASK)
            if kind is None and offset_sized(statement.value, state):
                kind = OFFSET
            hinted = self.site_hints.get((getattr(statement, "origin", None), statement.index))
            if hinted is not None:
                kind = hinted
            if statement.index in OBJECT_REGISTERS and (kind is None or kind[0] == "global") \
                    and not (kind is None and isinstance(statement.value, ir.Const)):
                # whatever the object registers are set to is used as an object, except an address outside
                # the work RAM (test_bookkeeping walks the bookkeeping block at 701F02 with R20)
                kind = ("object", 0)
            zeros[statement.index] = low_zeros(statement.value, zeros)
            if kind is None:
                state.pop(statement.index, None)
            else:
                state[statement.index] = kind
        elif isinstance(statement, ir.Call):
            targets = self.flow.call_targets(statement)
            if record is None:
                for target in targets:
                    # a call through a slot passes nothing but the context registers
                    told = {index: state.get(index, UNKNOWN) for index in self.context} if statement.slot is not None \
                        else self.told(target, state)
                    self.tell_callee(target, told)
            self.after_call(targets, state, context_only=statement.slot is not None)
            # what symbols.txt says a call hands back points at ("local" lines name it by the call)
            origin = getattr(statement, "origin", None)
            if origin is not None and statement.slot is None:
                for target in targets:
                    for index in self.handed_back(target):
                        hinted = self.site_hints.get((origin, index))
                        if hinted is not None:
                            state[index] = hinted
            for index in list(zeros):
                if index not in state:
                    zeros.pop(index, None)
        elif isinstance(statement, ir.Effect):
            written = {variable[1] for variable in self.flow.statement_writes(statement) if variable[0] == "r"}
            for index in written:
                state.pop(index, None)
                zeros.pop(index, None)

    def note(self, statement, state, record):
        found, accesses = record
        if isinstance(statement, ir.Store):
            hit = access(statement.address, state)
            if hit is not None and hit[0] in REGIONS:
                hit = None    # nothing writes the ROM: whatever this is, it stays as it was
            if hit is not None:
                if hit[0] == "global":
                    hit = hit + (statement.bits,)
                else:
                    accesses.append((hit[0], hit[1], statement.bits))
                found[id(statement)] = hit
        for expression in ir.statement_expressions(statement):
            for node in ir.walk(expression):
                if isinstance(node, ir.Load):
                    hit = access(node.address, state)
                    if hit is not None and hit[0] in REGIONS:
                        found[id(node)] = hit
                        continue
                    if hit is not None:
                        if hit[0] == "global":
                            hit = hit + (node.bits,)
                        else:
                            accesses.append((hit[0], hit[1], node.bits))
                        found[id(node)] = hit

    def analyse(self, entry, record):
        global GLOBAL_FAMILY
        GLOBAL_FAMILY = self.global_families.get(entry)
        function = self.functions[entry]
        blocks = function.blocks
        told = self.entry_kinds.get(entry, {})
        # the object registers stay objects when callers disagree (one passes a block a sprite lives in)
        state = {index: kind for index, kind in CONTEXT_KINDS.items()
                 if told.get(index) != CONFLICT or index in OBJECT_REGISTERS}
        state.update(known(told))
        state.update(self.hints.get(entry, {}))
        own = self.families.get(entry)
        if own is not None:
            # the object the function runs on is its family's, whatever its callers say
            for index in OWN_REGISTERS:
                kind = state.get(index)
                if kind is not None and kind[:2] == ("object", 0):
                    state[index] = ("object", 0, own)
        start = {entry: state}
        pending = [entry]
        while pending:
            label = pending.pop()
            state = dict(start[label])
            zeros = {}
            for statement in blocks[label].statements:
                self.step(statement, state, zeros, None)
            for successor in blocks[label].successors():
                if successor not in start:
                    start[successor] = dict(state)
                    pending.append(successor)
                    continue
                merged = merge(start[successor], state)
                if merged != start[successor]:
                    start[successor] = merged
                    pending.append(successor)
        exits = None
        for label, state in start.items():
            state = dict(state)
            zeros = {}
            block = blocks[label]
            for statement in block.statements:
                if record is not None:
                    self.note(statement, state, record)
                self.step(statement, state, zeros, record)
            end = block.end
            if record is not None:
                self.note(end, state, record)
            if isinstance(end, ir.TailCall):
                if record is None:
                    self.tell_callee(end.target, self.told(end.target, state))
                self.after_call([end.target], state)
            if isinstance(end, (ir.Return, ir.TailCall, ir.InterruptReturn)):
                kinds = {index: state.get(index, UNKNOWN) for index in self.handed_back(entry)}
                exits = kinds if exits is None else gather(exits, kinds)
        if record is None and exits is not None:
            self.tell_callers(entry, exits)


class Layouts:
    """The fields of each structure: those symbols.txt names (with their types), then one for every other
    offset the code reads or writes, with the width it uses most (unk_XX). An access with another width,
    or inside a wider field, says so in the C. symbols.txt may name a part of a wider field as a field of
    its own (the whole part of a 16.16 position): the struct holds both in a union."""

    TYPES = {"u8": (8, False), "s8": (8, True), "u16": (16, False), "s16": (16, True),
             "u32": (32, False), "s32": (32, True)}

    def __init__(self, accesses, names=None):
        """'accesses': iterable of (struct, offset, bits); 'names': disassemble.Names."""
        declared = {}
        if names is not None:
            for (struct, offset), (name, kind) in names.fields.items():
                declared.setdefault(struct, {})[offset] = (name, *self.TYPES[kind])
        counts = {}
        for struct, offset, bits in accesses:
            counts.setdefault(struct, {}).setdefault(offset, {}).setdefault(bits, 0)
            counts[struct][offset][bits] += 1
        self.fields = {}
        self.names = {}
        self.signed = {}
        self.scoped = {}    # struct -> family -> {offset: (name, bits, signed)}
        self.views = {}     # struct -> family -> the member that holds the family's view
        self.overlays = {}  # struct -> {(offset, bits): (name, signed)}: a narrower name for a field's first bytes
        self.scoped_overlays = {}    # struct -> family -> {(offset, bits): (name, signed)}: the same in a family's view
        if names is not None:
            for (struct, offset, bits), (name, kind) in names.overlays.items():
                if struct != "global":
                    self.overlays.setdefault(struct, {})[(offset, bits)] = (name, self.TYPES[kind][1])
            for (struct, family), found in names.scoped_overlays.items():
                self.scoped_overlays.setdefault(struct, {})[family] = {
                    key: (name, self.TYPES[kind][1]) for key, (name, kind) in found.items()}
        if names is not None:
            for (struct, family), found in names.scoped.items():
                self.scoped.setdefault(struct, {})[family] = {offset: (name, *self.TYPES[kind])
                                                              for offset, (name, kind) in found.items()}
        for struct in set(counts) | set(declared):
            fields, names_, signed = {}, {}, {}
            for offset, (name, bits, is_signed) in declared.get(struct, {}).items():
                fields[offset] = bits
                names_[offset] = name
                signed[offset] = is_signed
            covered = {offset + byte for offset, bits in fields.items() for byte in range(bits // 8)}
            offsets = counts.get(struct, {})
            for offset in sorted(offsets):
                if offset in covered:
                    continue    # a named field, or inside a wider field before it
                widths = offsets[offset]
                bits = max(widths, key=lambda width: (widths[width], width))
                while bits > 8 and any(offset + byte in covered for byte in range(bits // 8)):
                    bits //= 2
                if offset + bits // 8 > STRUCTS[struct]:
                    bits = 8
                fields[offset] = bits
                names_[offset] = f"unk_{offset:02X}"
                signed[offset] = False
                covered |= {offset + byte for byte in range(bits // 8)}
            self.fields[struct] = fields
            self.names[struct] = names_
            self.signed[struct] = signed
        # names the preprocessor would take (the variables, tables and registers are macros)
        macros = set()
        if names is not None:
            for found in (names.variables, names.tables, names.hardware, names.overlays):
                macros |= {name for name, _ in found.values()}
        for struct, families in self.scoped.items():
            taken = set(self.names.get(struct, {}).values()) | macros
            self.views[struct] = {family: family if family not in taken else f"{family}_view" for family in families}

    def scoped_field(self, struct, family, offset):
        """(view name, field name, field offset, field bits, signed) of the family's field holding 'offset',
        or None."""
        fields = self.scoped.get(struct, {}).get(family, {})
        for start in range(offset, offset - 4, -1):
            found = fields.get(start)
            if found is not None and start + found[1] // 8 > offset:
                return self.views[struct][family], found[0], start, found[1], found[2]
        return None

    def overlay(self, struct, offset, bits):
        """(name, signed) of the overlay of 'bits' at 'offset', or None."""
        return self.overlays.get(struct, {}).get((offset, bits))

    def scoped_overlay(self, struct, family, offset, bits):
        """(name, signed) of the family's overlay of 'bits' at 'offset', or None."""
        return self.scoped_overlays.get(struct, {}).get(family, {}).get((offset, bits))

    def families(self, struct):
        return sorted(self.scoped.get(struct, {}))

    def region(self, struct):
        """Where the families' views start in the structure (at the start of the field the first of them
        falls in), or None."""
        offsets = [offset for fields in self.scoped.get(struct, {}).values() for offset in fields]
        if not offsets:
            return None
        start = min(offsets)
        for offset, bits, _ in self.members(struct):
            if offset < start < offset + bits // 8:
                return offset
        return start

    def lookup(self, struct, offset):
        """(field offset, field bits) of the narrowest field holding 'offset', or None."""
        fields = self.fields.get(struct, {})
        for start in range(offset, offset - 4, -1):
            bits = fields.get(start)
            if bits is not None and start + bits // 8 > offset:
                return start, bits
        return None

    def members(self, struct, fields=None):
        """The fields ({offset: bits}, the structure's by default) as C members: (offset, bits, [parts]) for
        each outermost field, the parts being the fields inside it, as members of their own (a part may
        have parts). A field that starts inside another and runs past its end has no place in C."""
        fields = self.fields.get(struct, {}) if fields is None else fields
        outer = []
        holding = []    # the fields the next one may lie inside, the outermost first
        for offset in sorted(fields, key=lambda start: (start, -fields[start])):
            bits = fields[offset]
            member = (offset, bits, [])
            while holding and offset >= holding[-1][0] + holding[-1][1] // 8:
                holding.pop()
            if holding:
                if offset + bits // 8 > holding[-1][0] + holding[-1][1] // 8:
                    raise ValueError(f"{struct}: the field at {offset:X} runs past the end of the one at "
                                     f"{holding[-1][0]:X}")
                holding[-1][2].append(member)
            else:
                outer.append(member)
            holding.append(member)
        return outer

    def member_lines(self, struct, members, start, end, pad, names=None, signed=None, overlays=None):
        """C members for the fields 'members' (see members()), from offset 'start' to 'end'; 'names',
        'signed' ({offset: ...}) and 'overlays' ({(offset, bits): (name, signed)}) are the structure's by
        default."""
        generic = names is None
        names = self.names[struct] if names is None else names
        signed = self.signed[struct] if signed is None else signed
        if overlays is None:
            overlays = self.overlays.get(struct, {}) if generic else {}

        def kind(offset, bits):
            return f"int{bits}_t" if signed[offset] else f"uint{bits}_t"

        def lay(members, start, end, pad):
            lines = []
            position = start
            for offset, bits, parts in members:
                if offset > position:
                    lines.append(f"{pad}uint8_t pad_{position:02X}[0x{offset - position:X}];")
                own = [(found_bits, found) for (found_offset, found_bits), found in sorted(overlays.items())
                       if found_offset == offset and found_bits < bits]
                if not parts and not own:
                    lines.append(f"{pad}{kind(offset, bits)} {names[offset]};")
                else:
                    # a field with parts or overlays of its own: views of the same bytes
                    lines += [f"{pad}union", f"{pad}{{", f"{pad}\t{kind(offset, bits)} {names[offset]};"]
                    for overlay_bits, (overlay_name, overlay_signed) in own:
                        overlay_kind = f"int{overlay_bits}_t" if overlay_signed else f"uint{overlay_bits}_t"
                        lines.append(f"{pad}\t{overlay_kind} {overlay_name};")
                    if parts:
                        lines += [f"{pad}\tstruct", f"{pad}\t{{"]
                        lines += lay(parts, offset, offset, pad + "\t\t")
                        lines.append(f"{pad}\t}};")
                    lines.append(f"{pad}}};")
                position = offset + bits // 8
            if position < end:
                lines.append(f"{pad}uint8_t pad_{position:02X}[0x{end - position:X}];")
            return lines
        return lay(members, start, end, pad)

    def member_type(self, struct, offset, bits):
        return f"int{bits}_t" if self.signed[struct][offset] else f"uint{bits}_t"

    def declarations(self):
        lines = ["#pragma pack(push, 1)"]
        for struct, size in STRUCTS.items():
            fields = self.fields.get(struct, {})
            if struct not in self.names:
                self.names[struct], self.signed[struct] = {}, {}
            lines.append(f"struct {struct}")
            lines.append("{")
            members = self.members(struct)
            region = self.region(struct)
            if region is None:
                lines += self.member_lines(struct, members, 0, size, "\t")
            else:
                if any(offset < region < offset + bits // 8 for offset, bits, _ in members):
                    raise ValueError(f"{struct}: a field runs into the families' views at {region:X}")
                lines += self.member_lines(struct, [m for m in members if m[0] < region], 0, region, "\t")
                # the object types' own variables: what every object keeps there, and each family's view
                lines += ["\tunion", "\t{", "\t\tstruct", "\t\t{"]
                lines += self.member_lines(struct, [m for m in members if m[0] >= region], region, size, "\t\t\t")
                lines.append("\t\t};")
                for family in self.families(struct):
                    fields = self.scoped[struct][family]
                    widths = {offset: bits for offset, (_, bits, _) in fields.items()}
                    view = self.members(struct, widths)
                    if max(o + b // 8 for o, b in widths.items()) > size:
                        raise ValueError(f"{struct} {family}: fields run past the end")
                    lines += ["\t\tstruct", "\t\t{"]
                    lines += self.member_lines(struct, view, region, region, "\t\t\t",
                                               {offset: found[0] for offset, found in fields.items()},
                                               {offset: found[2] for offset, found in fields.items()},
                                               self.scoped_overlays.get(struct, {}).get(family, {}))
                    lines.append(f"\t\t}} {self.views[struct][family]};")
                lines.append("\t};")
            lines.append("};")
            lines.append("")
        lines.append("#pragma pack(pop)")
        lines.append("")
        for struct, size in STRUCTS.items():
            what = "the program ROM's records" if struct in ROM_STRUCTS else "the game's blocks"
            lines.append(f"_Static_assert(sizeof(struct {struct}) == 0x{size:X}, "
                         f"\"struct {struct} has the size of {what}\");")
        lines.append("")
        for struct in STRUCTS:
            if struct in ROM_STRUCTS:
                lines.append(f"#define {struct.upper()}(address) ((const struct {struct} *){ROM_FUNCTION}(address))")
            else:
                lines.append(f"#define {struct.upper()}(address) ((struct {struct} *){STRUCT_FUNCTION}(address))")
        for struct, (first, count) in ARRAYS.items():
            lines.append(f"#define {struct.upper()}_ADDRESS(n) (0x{first:X}u + (n) * 0x{STRUCTS[struct]:X}u)    "
                         f"/* the {count} of them, from 0 */")
        return lines
