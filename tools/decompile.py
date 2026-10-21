"""
Name:        decompile.py
Description: decompiles the game's V70 program into readable C, one C function per original function
Version:     0.1.0
Created:     11-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

Usage: python tools/decompile.py [rom folder or .zip] [--check] [--trace] [--out DIR] [--symbols FILE]
                                 [--variables FILE] [--split-webs] [--fields FILE]
  --check     build every function's IR and report what does not lift, without writing anything
  --trace     every function and block reports itself when it runs (game_trace in runtime/src/game.c)
  --out       where to write the C (src/ by default, the game's source; CMake reads ORUNNERS_DECOMPILED_DIR)
  --symbols   the names to use instead of symbols.txt
  --variables also list every function's local variables and the instructions that write them, in the
              terms of symbols.txt's "local" lines (for naming them)
  --split-webs  every value of a register the function reads is a variable of its own, named or not
              (for naming them: by default the unnamed ones share the register's variable)
  --fields    also list the object types' own variables the C reads through a function's own object
              without a name from its family (scoped lines), by family

Uses the same analysis as tools/recompile.py (symbols.txt, no traces), lifts every function to IR
(tools/decompiler/function.py), works out what each one takes and hands back, and writes src/: game.h
(prototypes, file by file, and the addresses of code kept in memory slots), ram.h (the game's variables,
tables, board registers and structures), the functions' files (symbols.txt's file lines lay them out, as
the original's source files were: system/boot.c, player/states.c...; game_NN.c without them) and table.c
(the code calls through memory slots reach, by address). Beside the build, in build/decompile/: index.txt (where each function is, its callers
and callees, how much of it the scenarios ran) and unnamed_addresses.txt (ROM addresses the C uses as
values that symbols.txt does not name yet). Everything it names comes from symbols.txt.
"""
import bisect
import re
import sys
import time
from collections import Counter
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import disassemble  # noqa: E402
import recompile  # noqa: E402
import orunners_rom  # noqa: E402
from decompiler import cleanup  # noqa: E402
from decompiler import conditions  # noqa: E402
from decompiler import constants  # noqa: E402
from decompiler import dataflow  # noqa: E402
from decompiler import emit  # noqa: E402
from decompiler import expressions  # noqa: E402
from decompiler import knownbits  # noqa: E402
from decompiler import memory  # noqa: E402
from decompiler import pointers  # noqa: E402
from decompiler import function as functions  # noqa: E402
from decompiler import preserved  # noqa: E402
from decompiler import signatures  # noqa: E402
from decompiler import structure  # noqa: E402
from decompiler import stack  # noqa: E402
from v60 import ir  # noqa: E402
from v60 import lift  # noqa: E402


SYMBOLS = ROOT / "symbols.txt"
REPORTS = ROOT / "build" / "decompile"    # index.txt and unnamed_addresses.txt: not part of the source
SHARED_ENTRY_MINIMUM = 64    # instructions two functions share before one of them becomes a way into the other


def load_program(source):
    """The program split into functions. A "func" line of symbols.txt in the middle of code another
    function reaches by jumping (each state of the player's update, entered through a jump table) makes
    that code a function of its own: the jumps into it become tail calls."""
    image = orunners_rom.build_program(orunners_rom.load_set(source))
    symbols = disassemble.load_symbols(SYMBOLS)
    program = recompile.Program(image, symbols, set(), set())
    reached = set().union(*program.bodies.values())
    inside = {address for address, symbol in symbols.items()
              if symbol.kind == "func" and address in reached and address not in program.entries}
    # code nothing reaches (functions the game never calls) is decompiled too, from its "func" lines
    # (the code runs in the ROM's mirrors too: an address is reached when any copy of it is)
    reached_copies = {orunners_rom.canonical_address(address) for address in reached}
    unreached = {address for address, symbol in symbols.items()
                 if symbol.kind == "func" and address not in reached and address not in reached_copies}
    live = set(program.entries)
    if inside or unreached:
        program = recompile.Program(image, symbols, set(), inside | unreached)
    program.live_entries = live
    program.unreached = unreached
    merge_entries(program)
    return program


def merge_entries(program):
    """A function that, after some code of its own, jumps into the middle of code another function has
    too (the player's update runs into the state code the player's animation has) is that code and a
    jump into the other one, which gets a second way in (ir.EntryPoint): the shared code is written once.
    tools/recompile.py keeps a copy in each, as the C it writes cannot jump into another function."""
    program.inner_entries = {}     # address -> the function it is a second way into
    program.second_entries = {}    # function -> its other ways in
    for x in sorted(program.bodies):
        body = program.bodies[x]
        for y in sorted(program.bodies):
            other = program.bodies[y]
            if y == x or x in other or len(body.keys() & other.keys()) < SHARED_ENTRY_MINIMUM:
                continue
            own = {address: instruction for address, instruction in body.items() if address not in other}
            into = {successor for instruction in own.values() for successor in program.successors(instruction)
                    if successor in other}
            if len(into) != 1:
                continue
            target = into.pop()
            program.inner_entries[target] = y
            program.second_entries.setdefault(y, []).append(target)
            program.bodies[x] = own
            break


def build_functions(program):
    builder = functions.Builder(program)
    built = {}
    failures = []
    for entry in sorted(program.entries):
        try:
            built[entry] = builder.build(entry)
        except lift.Unsupported as problem:
            failures.append((entry, str(problem)))
    fold_mirror_copies(program, built)
    return built, failures


def fold_mirror_copies(program, built):
    """A function the game calls in one copy of the ROM and unused code calls in another is one function:
    the unused code's calls go to the copy the game runs (the bookkeeping functions run at 0FFxxx, and
    code nothing calls reaches them at 09Fxxx), which keeps its name."""
    live = getattr(program, "live_entries", set())
    copies = {}
    for entry in built:
        canonical = orunners_rom.canonical_address(entry)
        if canonical is not None:
            copies.setdefault(canonical, []).append(entry)
    alias = {}
    for entries in copies.values():
        running = [entry for entry in entries if entry in live]
        if len(entries) > 1 and len(running) == 1:
            alias.update({entry: running[0] for entry in entries if entry not in live})
    for entry, kept in alias.items():
        if "_at_" in program.names[kept] and "_at_" not in program.names[entry]:
            program.names[kept] = program.names[entry]    # the plain name goes with the copy that stays
        del built[entry]
        program.entries.discard(entry)
    for function in built.values():
        for block in function.blocks.values():
            for statement in block.statements + [block.end]:
                if isinstance(statement, (ir.Call, ir.TailCall)) and statement.target in alias:
                    statement.target = alias[statement.target]


def interrupt_handlers(program):
    table = next(address for address, symbol in program.symbols.items() if symbol.name == "vector_table")
    return [program.mem.r32(table + vector * 4) & 0xFFFFFF for vector in range(0x40, 0x45)]


def open_entries(program, built, flow):
    """The functions nothing in the program reaches: no call, tail call, call table or memory slot leads to
    them (code nothing reaches calls some of the others)."""
    reached = set(interrupt_handlers(program))
    for targets in flow.slot_targets.values():
        reached |= targets
    for function in built.values():
        for block in function.blocks.values():
            for statement in block.statements + [block.end]:
                if isinstance(statement, ir.Call):
                    reached |= set(flow.call_targets(statement))
                elif isinstance(statement, ir.TailCall):
                    reached.add(statement.target & 0xFFFFFF)
    return {entry for entry in getattr(program, "unreached", ()) if entry in built and entry not in reached}


def declared_results(program, open_entries):
    """What the functions nothing calls hand back, from the "result" lines of symbols.txt: {entry: {RN}}."""
    entries = {program.names[entry]: entry for entry in program.names}
    found = {}
    for function_name, register in disassemble.load_names(SYMBOLS).results:
        entry = entries.get(function_name)
        if entry is None:
            raise SystemExit(f"symbols.txt: a result of {function_name}, which is no function")
        if entry not in open_entries:
            raise SystemExit(f"symbols.txt: a result of {function_name}, whose callers say what it hands back")
        found.setdefault(entry, set()).add(register)
    return found


def analyse(program, built):
    """Stack slots, preserved registers, R25 and the data flow summaries, in place."""
    for function in built.values():
        stack.analyse(function)
    return analyse_after_stack(program, built)


def report_flow(flow):
    outputs = flow.outputs()
    inputs = Counter()
    register_inputs = Counter()
    output_sizes = Counter()
    register_outputs = Counter()
    for entry, summary in flow.summaries.items():
        registers_in = sorted(v for v in summary.reads if v[0] == "r")
        inputs[len(registers_in)] += 1
        for variable in summary.reads:
            register_inputs[variable] += 1
        out = outputs[entry]
        output_sizes[len(out)] += 1
        for variable in out:
            register_outputs[variable] += 1
    def name(variable):
        return f"R{variable[1]}" if variable[0] == "r" else variable[1]
    print("register inputs per function: " + ", ".join(f"{k}: {v}" for k, v in sorted(inputs.items())))
    print("most read on entry: " + ", ".join(f"{name(k)} {v}" for k, v in register_inputs.most_common(16)))
    print("outputs per function: " + ", ".join(f"{k}: {v}" for k, v in sorted(output_sizes.items())))
    print("most handed back: " + ", ".join(f"{name(k)} {v}" for k, v in register_outputs.most_common(16)))


def check(program):
    built, failures = build_functions(program)
    stack_free = 0
    for entry, function in built.items():
        try:
            stack.analyse(function)
        except stack.StackError as problem:
            failures.append((entry, str(problem)))
            continue
        left = stack.leftovers(function)
        if left:
            failures.append((entry, f"{len(left)} statements still use SP, the first: {left[0]}"))
        else:
            stack_free += 1
    ends = Counter()
    blocks = 0
    statements = 0
    for function in built.values():
        for block in function.blocks.values():
            blocks += 1
            statements += len(block.statements)
            ends[type(block.end).__name__] += 1
            for successor in block.successors():
                if successor not in function.blocks:
                    failures.append((function.entry, f"block {block.label} jumps to {successor}, not a block"))
    print(f"functions:  {len(built)} ({stack_free} without SP once the stack is slots)")
    print(f"blocks:     {blocks}")
    print(f"statements: {statements}")
    print("block ends: " + ", ".join(f"{name} {count}" for name, count in ends.most_common()))
    print(f"problems:   {len(failures)}")
    for entry, problem in failures[:20]:
        print(f"  {entry:06X}  {problem}")
    if not failures:
        flow = analyse_after_stack(program, built)
        left = [(function.entry, statement) for function in built.values() for statement in constants.uses(function)]
        print(f"R{constants.GLOBALS_BASE} left:   {len(left)}")
        for entry, statement in left[:5]:
            print(f"  {entry:06X}  {statement}")
        kept = Counter(len(function.preserved) for function in built.values())
        print("preserved registers per function: " + ", ".join(f"{k}: {v}" for k, v in sorted(kept.items())))
        report_flow(flow)
        report_signatures(signatures.compute(flow))
        leaks = shared_through_slots(flow)
        print(f"values code reached through slots or interrupts would hand back: {len(leaks)}")
        for entry, problem in leaks[:10]:
            print(f"  {entry:06X}  {problem}")
        failures += left + leaks
    return 1 if failures else 0


def shared_through_slots(flow):
    """Code called through a memory slot, or an interrupt handler, hands nothing back in C but the
    context registers. The game must not rely on more: after such a call nothing may read a register
    the code could have written, and a handler has to put back every register it uses."""
    problems = []
    context = flow.context
    for entry in flow.functions:
        for call, live in flow.live_after_calls(entry):
            if call.slot is None:
                continue
            written = flow.combine(flow.call_targets(call))[1] - context
            leaked = (live & written) - context
            if leaked:
                problems.append((entry, f"reads {sorted(leaked)[:6]} after a call through slot {call.slot:X}"))
    for handler in flow.interrupt_handlers:
        written = {variable for variable in flow.summaries[handler].writes if variable[0] == "r"} - context
        if written:
            problems.append((handler, f"interrupt handler changes {sorted(written)[:6]}"))
    return problems


def report_signatures(found):
    def counts(sizes):
        return ", ".join(f"{k}: {v}" for k, v in sorted(Counter(sizes).items()))
    print("parameters per function: " + counts(len(signature.params) for signature in found.values()))
    print("results per function:    " + counts(len(signature.results) for signature in found.values()))
    print("zeroed inputs:           " + counts(len(signature.zeroed) for signature in found.values()))


def analyse_after_stack(program, built):
    """analyse() for functions whose stack slots are already made."""
    stack.link_caller_exits(built)
    for function in built.values():
        constants.substitute(function)
    flow = dataflow.Program(built, program.analysis, interrupt_handlers(program), signatures.CONTEXT)
    signatures.OPEN = open_entries(program, built, flow)
    flow.declared = declared_results(program, signatures.OPEN)
    flow.solve()
    while True:
        removed = sum(len(preserved.analyse(function, flow.call_writes, context=signatures.CONTEXT))
                      for function in built.values())
        if not removed:
            break
        flow.solve()
    changed = sum(conditions.recover(function, flow) for function in built.values())
    if changed:
        flow.reset()
        flow.solve()
    flow.conditions_recovered = changed
    return flow


CREATED = "12-10-2026"    # when the C was first written; the files say when it was last written too


def file_header(name, description):
    """The header every file of the project starts with, for a file this program writes."""
    return ("/**\n"
            f" *    Name:        {name}\n"
            f" *    Description: {description}\n"
            " *    Version:     0.1.0\n"
            f" *    Created:     {CREATED}\n"
            f" *    Modified:    {time.strftime('%d-%m-%Y')}\n"
            " *    Author:      Sebastian Toyos\n"
            " *\n"
            " *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by\n"
            " *    tools/decompile.py, with the names of symbols.txt.\n"
            " */\n")
FUNCTIONS_PER_FILE = 120
PART_DESCRIPTION = "the game's functions, part {number:02} (in the order of their V60 addresses)"
FILE_LINE = re.compile(r"([0-9A-F]{4,6})\s+file\s+([A-Za-z0-9_/]+)\s*(?:#\s*(.*\S))?\s*$")


def source_files(symbols_path, entries, address_of, part_name, part_description):
    """[(path without .c, description, entries)]: the C files the functions go to, by subject. A file
    line of the symbols ('ADDRESS file PATH  # ...') gives the functions from ADDRESS up to the next file
    line to src/PATH.c: a stretch of the program the original's linker laid out as one piece. A file may
    take several stretches (the objects of one stage, from both code chips); its functions go in the order
    of their addresses, and the comment of its first line is its description (the others say what their
    stretch holds). Without file lines, the functions are cut into parts of FUNCTIONS_PER_FILE (part_name
    and part_description take the part's number)."""
    marks = []
    for line in Path(symbols_path).read_text(encoding="utf-8").splitlines():
        match = FILE_LINE.match(line)
        if match:
            marks.append((int(match.group(1), 16), match.group(2), match.group(3) or ""))
    ordered = sorted(entries, key=address_of)
    if not marks:
        return [(part_name.format(number=number), part_description.format(number=number),
                 ordered[first:first + FUNCTIONS_PER_FILE])
                for number, first in enumerate(range(0, len(ordered), FUNCTIONS_PER_FILE))]
    marks.sort()
    starts = [address for address, _, _ in marks]
    if len(set(starts)) != len(starts):
        raise SystemExit(f"{Path(symbols_path).name}: two file lines at one address")
    descriptions = {}
    for _, path, description in marks:
        descriptions.setdefault(path, description)
    groups = {}
    for entry in ordered:
        index = bisect.bisect_right(starts, address_of(entry)) - 1
        if index < 0:
            raise SystemExit(f"{Path(symbols_path).name}: the function at {address_of(entry):06X} comes before "
                             "the first file line")
        groups.setdefault(marks[index][1], []).append(entry)
    return [(path, descriptions[path], groups[path]) for path in sorted(groups)]


def write_sources(out_dir, files, texts, include, header=None, keep=()):
    """Every file of 'files' (source_files) into out_dir, its functions' C after the header ('header', the
    program's file_header) and the include; the .c files of an earlier run go first (but in the folders of
    'keep', another program's)."""
    header = header or file_header
    for stale in sorted(out_dir.rglob("*.c")):
        if stale.relative_to(out_dir).parts[0] not in keep:
            stale.unlink()
    for folder in sorted((path for path in out_dir.rglob("*") if path.is_dir()), reverse=True):
        if folder.relative_to(out_dir).parts[0] not in keep and not any(folder.iterdir()):
            folder.rmdir()
    for path, description, entries in files:
        target = out_dir / f"{path}.c"
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(header(f"{path}.c", description) + f'#include "{include}"\n\n'
                          + "\n".join(texts[entry] for entry in entries), encoding="ascii")


def grouped_prototypes(files, prototype):
    """The functions' prototypes file by file, each file's under a comment with its path and description."""
    lines = []
    for path, description, entries in files:
        lines += [f"/* {path}.c: {description} */"] + [prototype(entry) + ";" for entry in entries] + [""]
    return lines[:-1]


def source_locations(files, texts):
    """{entry: 'path.c:line'} where each function's C starts in the files write_sources writes."""
    locations = {}
    for path, description, entries in files:
        # the header, then the #include and a blank line
        line = file_header(f"{path}.c", description).count("\n") + 2
        for entry in entries:
            locations[entry] = f"{path}.c:{line + 1}"
            line += texts[entry].count("\n") + 1
    return locations


def canonical_entry(entry):
    """Where a function is in the program ROM's own copy (code run through a mirror is folded to it)."""
    canonical = orunners_rom.canonical_address(entry)
    return entry if canonical is None else canonical


def game_files(entries):
    """The files of src/ for the game's functions (symbols.txt's file lines)."""
    return source_files(SYMBOLS, entries, canonical_entry, "game_{number:02}", PART_DESCRIPTION)
RESET_ADDRESS = 0xFFFFF0
IDENTIFIER = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
# names the runtime the decompiled code includes already uses (game/support.h, s32/bus.h, v60/arith.h)
TAKEN = {"psw", "psw_read", "psw_write", "psw_update", "load_privileged", "frame_wait", "take_interrupts",
         "frame_end", "swap_tasks", "game_context", "game_context_registers", "game_context_count", "halt", "call_code", "unknown_code", "invalid_jump", "move_string", "move_bits_down", "move_bits_up",
         "search_bits_up", "search_bytes", "divx_pair", "divux_pair", "game_code", "game_code_count", "game_entry", "main"}


def needs_return_address(built):
    """Functions that read their own return address, and the ones that tail call them (and so hand over
    their own)."""
    found = {entry for entry, function in built.items() if emit.reads_return_address(function)}
    changed = True
    while changed:
        changed = False
        for entry, function in built.items():
            if entry in found:
                continue
            if any(isinstance(block.end, ir.TailCall) and block.end.target in found
                   for block in function.blocks.values()):
                found.add(entry)
                changed = True
    return found


def c_names(program, built):
    names = {}
    for entry in built:
        name = program.names[entry]
        if not IDENTIFIER.match(name) or name in TAKEN:
            raise SystemExit(f"{entry:06X}: '{name}' cannot be a C function name")
        names[entry] = name
    if len(set(names.values())) != len(names):
        raise SystemExit("two functions share a name")
    return names


def call_results(function, found, flow):
    """For each call of 'function': the registers the C call hands back (through result pointers)."""
    results = {}
    for block in function.blocks.values():
        for statement in block.statements:
            if not isinstance(statement, ir.Call):
                continue
            registers = set()
            if statement.slot is None:
                for target in flow.call_targets(statement):
                    registers |= {value[1] for value in found[target].results if value[0] == "r"}
            results[id(statement)] = registers
    return results


def write_index(out_dir, program, built, flow, names, texts, entries):
    """index.txt: for every function, where its C is, its size, who calls it and what it calls, the
    named things its C mentions, and whether a scenario of tests/controls ran it (build/scenarios)."""
    callers = {entry: set() for entry in built}
    callees = {entry: set() for entry in built}
    for entry, function in built.items():
        for block in function.blocks.values():
            targets = []
            for statement in block.statements:
                if isinstance(statement, ir.Call):
                    try:
                        targets += flow.call_targets(statement) if statement.slot is None else []
                    except ValueError:
                        pass
            if isinstance(block.end, ir.TailCall):
                targets.append(block.end.target)
            for target in targets:
                if target in built:
                    callees[entry].add(target)
                    callers[target].add(entry)
    executed = set()
    traces = sorted((ROOT / "build" / "scenarios").glob("*/executed.txt"))
    if traces:
        found, _ = disassemble.load_trace(traces, fold_mirrors=False)
        executed = {address & 0xFFFFFF for address in found}
    words = re.compile(r"\b(unk_2[0-9A-F]{5}|rom_[0-9A-F]{6}|[A-Z_]+_[0-9A-F]{6}|[a-z][a-z0-9_]+)\b")
    lines = ["# function  name  instructions  ran-in-a-scenario  file:line",
             "#   calls: ...   called by: ...   mentions: variables, tables, registers, fields"]
    locations = source_locations(game_files(entries), texts)
    for entry in entries:
        body = program.bodies[entry]
        ran = sum(1 for address in body if address in executed)
        lines.append(f"{entry:06X}  {names[entry]}  {len(body)}  {ran}/{len(body)}  {locations[entry]}")
        lines.append("    calls: " + " ".join(sorted(names[target] for target in callees[entry])))
        lines.append("    called by: " + " ".join(sorted(names[caller] for caller in callers[entry])))
        mentioned = sorted({word for word in words.findall(texts[entry])
                            if word.startswith(("unk_", "rom_")) or word.isupper() or "->" in word})
        fields = sorted(set(re.findall(r"(?:OBJECT|PLAYER)\([^)]*\)->(\w+)", texts[entry])))
        lines.append("    mentions: " + " ".join(mentioned + ["->" + field for field in fields]))
    (out_dir / "index.txt").write_text("\n".join(lines) + "\n", encoding="ascii")


def code_table(program, flow):
    """Addresses call_code() may get: what calls through memory slots find there, the interrupt handlers,
    where tasks start (symbols.txt's task lines)."""
    addresses = set(interrupt_handlers(program))
    for targets in flow.slot_targets.values():
        addresses |= targets
    tasks = disassemble.load_names(SYMBOLS).tasks
    entries = {name: entry for entry, name in program.names.items()}
    for name in sorted(tasks):
        if name not in entries:
            raise SystemExit(f"symbols.txt: task {name}, which is no function")
        addresses.add(entries[name])
    return sorted(address for address in addresses if address in flow.functions)


def trampolines(built):
    """Functions that only jump to another one (the "br" entries of jump tables) -> (that function, the
    way into it)."""
    found = {}
    for entry, function in built.items():
        if len(function.blocks) != 1:
            continue
        block = next(iter(function.blocks.values()))
        if not block.statements and isinstance(block.end, ir.TailCall) and block.end.target & 0xFFFFFF != entry:
            found[entry] = (block.end.target & 0xFFFFFF, block.end.entry)
    return found


def family_accesses(built, names, known):
    """The functions' reads and writes of memory, as memory.accesses_of: (those of the whole program, {family:
    those that index a table at work RAM a family of functions names its own way (scoped global)})."""
    families = {family: set(found) for (struct, family), found in known.scoped.items() if struct == "global"}
    shared = []
    own = {}
    for entry, function in built.items():
        name = names[entry]
        family = max((family for family in families if name == family or name.startswith(family + "_")),
                     key=len, default=None)
        for access in memory.accesses_of([function]):
            parts = memory.based(access[0])
            if family is not None and parts is not None and parts[0] in families[family]:
                own.setdefault(family, []).append(access)
            else:
                shared.append(access)
    return shared, own


def write_c(program, out_dir, trace=False, variables_path=None, split_webs=False, fields_path=None):
    built, failures = build_functions(program)
    if failures:
        raise SystemExit(f"{len(failures)} functions do not lift, the first: {failures[0][0]:06X} {failures[0][1]}")
    flow = analyse(program, built)
    found = signatures.compute(flow)
    names = c_names(program, built)

    before = sum(len(block.statements) for function in built.values() for block in function.blocks.values())
    # what each function takes by value (as emit.py's by_value): its callers work out what they pass it as
    # the original does
    arguments = {}
    for entry, signature in found.items():
        registers = {value[1] for value in signature.params - signature.results if value[0] == "r"}
        returned = next(iter(signature.results)) if len(signature.results) == 1 else None
        if returned is not None and returned[0] == "r" and returned in signature.params:
            registers.add(returned[1])
        arguments[entry] = registers
    whole = set(signatures.CONTEXT) - set(signatures.SCRATCH_CONTEXT)    # global variables in C
    live_after = {}
    live_at_start = {}
    for entry, function in built.items():
        after, start = cleanup.eliminate(flow, function, found[entry].results, signatures.CONTEXT, arguments)
        expressions.tidy(function, flow, after, whole)
        results_of_calls = call_results(function, found, flow)
        if expressions.propagate_constants(function, results_of_calls, signatures.CONTEXT):
            after, start = cleanup.eliminate(flow, function, found[entry].results, signatures.CONTEXT, arguments)
            expressions.tidy(function, flow, after, whole)
        if expressions.inline_registers(function, flow, after, whole):
            expressions.simplify_function(function)
        if expressions.switch_values(function, flow):
            after, start = cleanup.eliminate(flow, function, found[entry].results, signatures.CONTEXT, arguments)
            expressions.tidy(function, flow, after, whole)
        knownbits.simplify(function, found[entry], signatures.CONTEXT, results_of_calls)
        expressions.simplify_function(function)
        live_after[entry] = after
        live_at_start[entry] = start
    known = disassemble.load_names(SYMBOLS)
    # the structures: the object and the player, and any other symbols.txt declares (flood_fill_record)
    pointers.STRUCTS.update(known.structs)
    pointers.ROM_STRUCTS = set(known.rom_structs)
    # records of their own (not the object's or the player's blocks) come one after another: the flood
    # fill's queue is walked a record at a time
    pointers.RUNS = set(known.structs) - {"object", "player"}
    # what symbols.txt says a parameter, or a local variable where an instruction writes it, points at
    entries_by_name = {program.names[entry]: entry for entry in built}
    hints = {}
    for (function, register), struct in known.param_structs.items():
        if function in entries_by_name:
            hints.setdefault(entries_by_name[function], {})[register] = pointers.link_kind(struct)
    site_hints = {}
    for (function, spot, value), struct in known.local_structs.items():
        if not value.startswith("r"):
            continue    # (a temporary or a stack slot: the structure is for registers only)
        if spot == "entry" and function in entries_by_name:
            hints.setdefault(entries_by_name[function], {})[int(value[1:])] = pointers.link_kind(struct)
        elif spot != "entry":
            site_hints[(spot, int(value[1:]))] = pointers.link_kind(struct)
    # the object a function of a family runs on is that family's: the longest family (scoped object lines)
    # whose name starts the function's (fire_front_chase is fire_front's), as emit.py reads its fields
    object_families = sorted({family for (struct, family) in known.scoped if struct == "object"}, key=len, reverse=True)
    global_names = sorted({family for (struct, family) in known.scoped if struct == "global"}, key=len, reverse=True)
    families, global_families = {}, {}
    for entry in built:
        name = program.names[entry]
        family = next((family for family in object_families if name == family or name.startswith(family + "_")), None)
        if family is not None:
            families[entry] = family
        family = next((family for family in global_names if name == family or name.startswith(family + "_")), None)
        if family is not None:
            global_families[entry] = family    # as emit.global_family_of
    typing = pointers.Program(built, flow, found,
                              {entry: call_results(function, found, flow) for entry, function in built.items()},
                              signatures.CONTEXT, known.targets, hints, site_hints, known.scoped_targets, families,
                              global_families)
    fields, accesses = typing.run()
    through = [(hit[1], hit[2]) for found_fields in fields.values() for hit in found_fields.values()
               if hit[0] == "global"]
    data_names = {address: symbol.name for address, symbol in program.symbols.items() if symbol.kind == "data"}
    shared, own = family_accesses(built, names, known)
    named = memory.Memory(known, shared + through, data_names, own)
    always = {entry: set(summary.always) for entry, summary in flow.summaries.items()}
    emitter = emit.Emitter(built, found, names, signatures.CONTEXT, needs_return_address(built), named, always,
                           program.mem.r32, trace)
    emitter.layouts = pointers.Layouts(accesses, known)
    emitter.register_names = dict(known.registers)
    emitter.writes = {entry: set(summary.writes) for entry, summary in flow.summaries.items()}
    emitter.second_entries = dict(program.second_entries)
    table = code_table(program, flow)
    emitter.code_addresses = {address: names[address] + "_ADDRESS" for address in table
                              if address >= 0x1000 and address in names}
    emitter.flow = flow
    emitter.split_webs = split_webs
    emitter.trampolines = trampolines(built)
    emitter.data_labels = {address: name for address, name in data_names.items() if address not in named.tables}
    emitter.code_labels = {address: symbol.name for address, symbol in program.symbols.items() if symbol.kind == "code"}
    emitter.comments = dict(known.comments)
    entries_by_name = {name: entry for entry, name in names.items()}
    unknown = sorted(function for function, _ in known.params if function not in entries_by_name)
    if unknown:
        print(f"symbols.txt names parameters of {len(unknown)} functions that are not in the program: "
              + ", ".join(unknown[:5]))
    emitter.param_names = {(entries_by_name[function], register): name
                           for (function, register), name in known.params.items() if function in entries_by_name}
    emitter.result_names = {(entries_by_name[function], register): name
                            for (function, register), name in known.results.items() if function in entries_by_name}
    # a local is found by the instruction that writes it, wherever that code now is; what a register holds
    # when a function starts, by the function
    emitter.local_sites = {(spot, value): name for (function, spot, value), name in known.locals.items()
                           if spot != "entry"}
    emitter.local_names = {(entries_by_name[function], spot, value): name
                           for (function, spot, value), name in known.locals.items()
                           if spot == "entry" and function in entries_by_name}
    unknown = sorted({function for function, spot, _ in known.locals if function not in entries_by_name})
    if unknown:
        print(f"symbols.txt names locals of {len(unknown)} functions that are not in the program (the "
              "instructions still find them): " + ", ".join(unknown[:5]))

    texts = {}
    problems = []
    # every function's variables first: a call casts what it passes to the width of the callee's parameter
    for entry, function in built.items():
        if emitter.by_pointer(entry):    # only where a copy can leave out results that come back unchanged
            structure.split_small_exits(function, (live_after[entry], fields[entry]))
        structure.split_loop_entries(function, (live_after[entry], fields[entry]))
    writers = {entry: emit.FunctionWriter(emitter, function, live_after[entry], fields[entry], live_at_start[entry])
               for entry, function in built.items()}
    for entry, writer in writers.items():
        try:
            texts[entry] = writer.write()
        except emit.EmitError as problem:
            problems.append((entry, str(problem)))
    # a result some call gives the same variable as another one keeps a copy of its own: written again
    emitter.shared_results = emit.shared_results(texts.values())
    for entry in texts:
        if emitter.shared_results.get(emitter.names.get(entry)):
            texts[entry] = writers[entry].write()
    if variables_path is not None:
        write_variables(variables_path, writers, names, texts)
    if fields_path is not None:
        lines = ["# FAMILY (or function NAME)  OFFSET  BITS  FUNCTIONS: object fields the C reads through the",
                 "# function's own object without a name from its family (symbols.txt scoped lines)"]
        for (family, offset, bits), users in sorted(emitter.missing_fields.items()):
            lines.append(f"{family}  {offset:02X}  {bits}  {' '.join(sorted(users))}")
        Path(fields_path).write_text("\n".join(lines) + "\n", encoding="ascii")
    left = sum(len(block.statements) for function in built.values() for block in function.blocks.values())
    removed = before - left
    if problems:
        for entry, problem in problems[:20]:
            print(f"  {entry:06X}  {problem}")
        raise SystemExit(f"{len(problems)} functions could not be written")

    out_dir.mkdir(parents=True, exist_ok=True)
    # a jump to another function needs no C of its own: its callers call that one (code reached through
    # memory slots keeps it, for table.c)
    entries = sorted(entry for entry in built
                     if entry not in emitter.trampolines or entry in table or entry == RESET_ADDRESS)
    context = ", ".join(emitter.context_name(index) for index in signatures.CONTEXT)
    defined = ", ".join(emitter.context_name(index) + (f" = 0x{signatures.INITIAL[index]:X}u" if index in signatures.INITIAL else "")
                        for index in signatures.CONTEXT)
    context_pointers = ", ".join("&" + emitter.context_name(index) for index in signatures.CONTEXT)
    context_numbers = ", ".join(str(index) for index in signatures.CONTEXT)
    files = game_files(entries)
    prototypes = grouped_prototypes(files, emitter.prototype)
    addresses = [f"#define {name} 0x{address:06X}u" for address, name in sorted(emitter.code_addresses.items())]
    (out_dir / "game.h").write_text(
        file_header("game.h", "prototypes of the game's functions and the addresses of the code it keeps in "
                    "memory slots")
        + "#ifndef ORUNNERS_GAME_H\n#define ORUNNERS_GAME_H\n\n#include \"game/support.h\"\n#include \"ram.h\"\n\n"
        f"/* registers the whole program shares */\nextern uint32_t {context};\n\n"
        "/* where the code the program keeps in memory slots (objects' handlers, hooks) is, as the game's own\n"
        "   addresses: what it stores and compares, and what call_code() runs */\n"
        + "\n".join(addresses) + "\n\n"
        + "\n".join(prototypes) + "\n\n#endif\n", encoding="ascii")
    labels = [f"#define {name}_ADDRESS 0x{address:06X}u" for address, name in sorted(emitter.data_labels.items())]
    (out_dir / "ram.h").write_text(
        file_header("ram.h", "the game's variables, tables, board registers and structures")
        + "#ifndef ORUNNERS_RAM_H\n#define ORUNNERS_RAM_H\n\n"
        + "\n".join(named.declarations()) + "\n\n/* data in the program ROM symbols.txt labels */\n"
        + "\n".join(labels) + "\n\n/* the game's structures in work RAM */\n"
        + "\n".join(emitter.layouts.declarations()) + "\n\n#endif\n", encoding="ascii")
    write_sources(out_dir, files, texts, "game.h", keep=("sound",))

    wrappers = []
    rows = []
    for address in table:
        wrapper = emitter.wrapper(address)
        if wrapper is None:
            rows.append(f"\t{{ 0x{address:06X}u, {names[address]} }},")
        else:
            wrappers.append(wrapper)
            rows.append(f"\t{{ 0x{address:06X}u, call_{address:06X} }},")
    if emitter.wrapper(RESET_ADDRESS) is not None:
        raise SystemExit("the reset code takes parameters")
    entry_text = f"/* the code at the reset vector */\nvoid game_entry(void)\n{{\n\t{names[RESET_ADDRESS]}();\n}}\n"
    (out_dir / "table.c").write_text(
        file_header("table.c", "the registers the whole program shares, the code calls through memory "
                    "slots reach (by address), and the reset entry")
        + '#include "game.h"\n\n'
        f"uint32_t {defined};\n\n"
        "/* the same, for the runtime to keep a copy for each of the game's tasks */\n"
        f"uint32_t *const game_context[] = {{ {context_pointers} }};\n"
        f"const uint8_t game_context_registers[] = {{ {context_numbers} }};\n"
        f"const size_t game_context_count = {len(signatures.CONTEXT)};\n\n" + "\n".join(wrappers) + "\n"
        "/* the code calls through memory slots and interrupts can reach, by address */\n"
        "const game_code_entry game_code[] =\n{\n" + "\n".join(rows) + "\n};\n\n"
        f"const size_t game_code_count = {len(rows)};\n\n" + entry_text, encoding="ascii")
    branches = sum(isinstance(block.end, ir.Branch) for function in built.values() for block in function.blocks.values())
    gotos = sum(text.count("goto ") for text in texts.values())
    with_gotos = sum(1 for text in texts.values() if "goto " in text)
    print(f"gotos:      {gotos}, in {with_gotos} functions")
    # the reports of the game's source go beside the build; those of another copy, with it
    reports = REPORTS if out_dir.resolve() == (ROOT / "src").resolve() else out_dir
    reports.mkdir(parents=True, exist_ok=True)
    write_index(reports, program, built, flow, names, texts, entries)
    raw = [f"{address:06X}  {' '.join(sorted(users))}" for address, users in sorted(emitter.raw_addresses.items())]
    (reports / "unnamed_addresses.txt").write_text(
        "# ROM addresses the C uses as values that symbols.txt does not name (table or data lines), and the\n"
        "# functions that use them\n" + "\n".join(raw) + "\n", encoding="ascii")
    print(f"unnamed:    {len(raw)} ROM addresses used as values")
    print(f"functions:  {len(entries)} in {out_dir}")
    print(f"statements: {left} ({removed} dead ones removed)")
    print(f"branches:   {branches}, {flow.conditions_recovered} of them now compare values instead of flags")
    print(f"code table: {len(rows)} addresses ({len(wrappers)} through wrappers)")
    return 0


def write_variables(path, writers, names, texts):
    """Every local variable of the C, by function: its name and where its values come from ("entry", or
    the address of an instruction that writes it, and the register or the instruction's temporary)."""
    lines = ["# FUNCTION  C NAME  WHERE:VALUE[:HOW] ...: the local variables of the decompiled C and the instructions",
             "# that write them, as symbols.txt's \"local FUNCTION WHERE VALUE NAME\" lines name them; HOW is call",
             "# for what a call hands back, effect for what a string move or a division leaves"]
    for entry, writer in sorted(writers.items()):
        text = texts.get(entry, "")
        for name, sites in sorted(writer.variable_sites().items()):
            if not re.search(rf"\b{re.escape(name)}\b", text):
                continue
            spots = " ".join(f"{spot if spot == 'entry' else format(spot, '06X')}:{value}"
                             + (f":{how}" if how in ("call", "effect") else "")
                             for spot, value, how in sorted(set(sites), key=lambda site: (str(site[0]), site[1])))
            lines.append(f"{names[entry]}  {name}  {spots}")
    Path(path).write_text("\n".join(lines) + "\n", encoding="ascii")


def main():
    arguments = sys.argv[1:]
    checking = "--check" in arguments
    tracing = "--trace" in arguments
    global SYMBOLS
    out_dir = ROOT / "src"
    if "--out" in arguments:
        position = arguments.index("--out")
        out_dir = Path(arguments[position + 1])
        del arguments[position:position + 2]
    if "--symbols" in arguments:
        position = arguments.index("--symbols")
        SYMBOLS = Path(arguments[position + 1])
        del arguments[position:position + 2]
    variables_path = None
    if "--variables" in arguments:
        position = arguments.index("--variables")
        variables_path = Path(arguments[position + 1])
        del arguments[position:position + 2]
    split_webs = "--split-webs" in arguments
    fields_path = None
    if "--fields" in arguments:
        position = arguments.index("--fields")
        fields_path = Path(arguments[position + 1])
        del arguments[position:position + 2]
    arguments = [argument for argument in arguments if argument not in ("--check", "--trace", "--split-webs")]
    program = load_program(Path(arguments[0]) if arguments else ROOT / "rom")
    if checking:
        return check(program)
    return write_c(program, out_dir, tracing, variables_path, split_webs, fields_path)


if __name__ == "__main__":
    sys.exit(main())
