"""
Name:        coverage.py
Description: byte map of the program ROM: decoded code, data the code points at, fill, and what is still unknown
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

Usage: python tools/coverage.py [rom folder or .zip] [--trace executed.txt ...]   (default ROM: rom/)
Writes build/listing/coverage.txt.

Runs the same analysis as tools/disassemble.py and gives every byte of the program image one class:
  code             decoded instructions, branch tables included
  described data   tables whose layout is known: the ones symbols.txt describes and the pointer tables
                   the analysis follows
  referenced data  from a place the code reads or takes the address of, or a data symbol, up to the
                   next known thing
  fill             runs of at least 16 bytes of 0xFF, the erased state of an EPROM
  unknown          everything else
Unknown runs are listed largest first, with two hints about what they are: how much of the run decodes
as instructions from its start, and how many 32-bit words elsewhere in the ROM hold its address.
"""
import sys
from collections import Counter, defaultdict
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import disassemble  # noqa: E402
import orunners_rom  # noqa: E402
from v60 import disassembler as dis  # noqa: E402

CODE = "code"
DESCRIBED = "described data"
DATA = "referenced data"
FILL = "fill"
UNKNOWN = "unknown"
CLASSES = (CODE, DESCRIBED, DATA, FILL, UNKNOWN)

FILL_BYTE = 0xFF
FILL_MINIMUM = 16
# Operand modes whose Operand.value is a fixed place in memory
FIXED_MODES = {"pc_disp", "pc_disp_indexed", "pc_disp_indirect", "pc_disp_indirect_indexed", "pc_double_disp",
               "direct", "direct_indexed", "direct_deferred", "direct_deferred_indexed"}
# ... and the ones among them where a jump or call reads its target from that place
INDIRECT_MODES = {"pc_disp_indirect", "pc_disp_indirect_indexed", "pc_double_disp",
                  "direct_deferred", "direct_deferred_indexed"}
CODE_CHIPS = orunners_rom.PROGRAM_AREAS[:2]


def data_references(analysis):
    """ROM places the code reads, or takes the address of, that are not code: {address: [instruction addresses]}"""
    references = defaultdict(list)
    for instruction in analysis.instructions.values():
        jumps = instruction.flow != dis.FLOW_NORMAL
        for operand in instruction.operands:
            # a jump or call names code with its operand, unless it reads the target from memory
            if operand.mode not in FIXED_MODES or (jumps and operand.mode not in INDIRECT_MODES):
                continue
            place = orunners_rom.canonical_address(operand.value)
            if place is not None and place not in analysis.owner:
                references[place].append(instruction.address)
    # data symbols count as references made by symbols.txt (no instruction address)
    for address, symbol in analysis.symbols.items():
        if symbol.kind == "data":
            references.setdefault(address, [])
    return references


def classify(analysis, references):
    """One class per byte of the program image: {address: class}, areas in PROGRAM_AREAS only"""
    image = analysis.image
    described = {entry + offset for entry in analysis.data_words for offset in range(4)}
    for first, stop in analysis.described:
        described.update(range(first, stop))
    classes = {}
    for start, end in orunners_rom.PROGRAM_AREAS:
        current = UNKNOWN
        for address in range(start, end):
            if address in analysis.owner:
                classes[address] = CODE
                current = UNKNOWN
            elif address in described:
                classes[address] = DESCRIBED
                current = UNKNOWN
            else:
                # referenced data runs from its reference to the next known thing
                if address in references:
                    current = DATA
                classes[address] = current
        # fill only replaces unknown bytes: a referenced table can hold 0xFF on purpose
        address = start
        while address < end:
            if classes[address] != UNKNOWN or image[address] != FILL_BYTE:
                address += 1
                continue
            stop = address
            while stop < end and classes[stop] == UNKNOWN and image[stop] == FILL_BYTE:
                stop += 1
            if stop - address >= FILL_MINIMUM:
                for byte_address in range(address, stop):
                    classes[byte_address] = FILL
            address = stop
    return classes


def runs_of(classes, wanted, start, end):
    """Maximal runs [first, stop) of bytes of class 'wanted' inside [start, end)"""
    runs = []
    address = start
    while address < end:
        if classes[address] != wanted:
            address += 1
            continue
        stop = address
        while stop < end and classes[stop] == wanted:
            stop += 1
        runs.append((address, stop))
        address = stop
    return runs


def sweep(analysis, start, end):
    """Bytes of [start, end) that decode as instructions, one after another, before the first invalid one"""
    address = start
    while address < end:
        instruction = dis.decode(analysis.mem, address)
        if instruction.flow == dis.FLOW_INVALID or address + instruction.length > end:
            break
        address += instruction.length
    return address - start


def pointers_to(analysis, targets):
    """32-bit words outside code (any alignment) whose value is one of 'targets', through any mirror"""
    image = analysis.image
    found = defaultdict(list)
    for start, end in orunners_rom.PROGRAM_AREAS:
        for address in range(start, end - 3):
            if address in analysis.owner:
                continue
            value = int.from_bytes(image[address:address + 4], "little")
            if value >= 0x1000000:
                continue
            target = orunners_rom.canonical_address(value)
            if target in targets:
                found[target].append(address)
    return found


def write_coverage(analysis, references, classes, path):
    image = analysis.image
    lines = ["Program ROM coverage", "====================", ""]
    header = f"{'area':<14}" + "".join(f"{name:>16}" for name in CLASSES) + f"{'total':>10}"
    lines.append(header)
    totals = Counter()
    for start, end in orunners_rom.PROGRAM_AREAS:
        counts = Counter(classes[address] for address in range(start, end))
        if sum(counts.values()) != end - start:
            raise SystemExit(f"coverage: {start:06X}-{end - 1:06X} classes do not add up to the area size")
        totals.update(counts)
        lines.append(f"{start:06X}-{end - 1:06X} " + "".join(f"{counts[name]:16}" for name in CLASSES)
                     + f"{end - start:10}")
    lines.append(f"{'total':<14}" + "".join(f"{totals[name]:16}" for name in CLASSES)
                 + f"{sum(totals.values()):10}")
    image_size = sum(end - start for start, end in orunners_rom.PROGRAM_AREAS)
    if sum(totals.values()) != image_size:
        raise SystemExit("coverage: classes do not add up to the program image size")

    unknown = []
    for start, end in CODE_CHIPS:
        unknown += runs_of(classes, UNKNOWN, start, end)
    pointers = pointers_to(analysis, {first for first, _ in unknown})
    rows = []
    for first, stop in unknown:
        decoded = sweep(analysis, first, stop)
        rows.append((first, stop, decoded, len(pointers.get(first, []))))
    as_code = [row for row in rows if row[2] == row[1] - row[0]]
    lines += [
        "",
        f"places the code or symbols.txt marks as data: {len(references)}",
        f"unknown runs in the code chips: {len(rows)}, {sum(stop - first for first, stop, _, _ in rows)} bytes",
        f"  decoding as instructions from start to end: {len(as_code)} runs, "
        f"{sum(stop - first for first, stop, _, _ in as_code)} bytes",
        f"  whose address is held by a word elsewhere: "
        f"{sum(1 for row in rows if row[3])} runs",
    ]
    title = "Unknown runs in the code chips, largest first"
    lines += ["", title, "-" * len(title),
              f"{'first':<7} {'last':<7} {'bytes':>6} {'decodes':>8} {'pointers':>8}  first bytes"]
    for first, stop, decoded, held in sorted(rows, key=lambda row: (row[0] - row[1], row[0])):
        verdict = "all" if decoded == stop - first else str(decoded)
        raw = image[first:min(stop, first + 12)].hex(" ")
        lines.append(f"{first:06X}  {stop - 1:06X}  {stop - first:6} {verdict:>8} {held:>8}  {raw}")
    path.write_text("\n".join(lines) + "\n", encoding="ascii")
    return lines


def main():
    arguments = sys.argv[1:]
    trace_paths = []
    while "--trace" in arguments:
        position = arguments.index("--trace")
        if position + 1 >= len(arguments):
            raise SystemExit("Usage: python tools/coverage.py [rom folder or .zip] [--trace executed.txt ...]")
        trace_paths.append(arguments[position + 1])
        del arguments[position:position + 2]
    source = Path(arguments[0]) if arguments else ROOT / "rom"

    image = orunners_rom.build_program(orunners_rom.load_set(source))
    executed, _ = disassemble.load_trace(trace_paths)
    analysis = disassemble.Analysis(image, disassemble.load_symbols(ROOT / "symbols.txt"), executed)
    analysis.run()
    references = data_references(analysis)
    classes = classify(analysis, references)

    out_dir = ROOT / "build" / "listing"
    out_dir.mkdir(parents=True, exist_ok=True)
    lines = write_coverage(analysis, references, classes, out_dir / "coverage.txt")
    end = next(index for index, line in enumerate(lines) if line.startswith("Unknown runs in the code chips"))
    print("\n".join(lines[:end]).rstrip())
    print(f"\nCoverage: {out_dir / 'coverage.txt'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
