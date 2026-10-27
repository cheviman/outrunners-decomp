"""
Name:        read_crossings.py
Description: where the original program's reads of its data run on from one named place into the next:
             the census of the reference's data reads (s32ref --rom-reads over the scenarios), as the names
             no piece of the assets may start at
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Usage: python tools/read_crossings.py [CENSUS_FOLDER]   (build/rom_reads by default)

Each file of the census lists, for every instruction that reads the program's ROM as data, the runs of
bytes it read ("PC FIRST-LAST"). Two ways a table goes on past a name:
- a run that goes from one named place into the next: the code reads the two as one table (the test mode's
  pages of text);
- an instruction that reads a table at a fixed base by an index ("mov.b 17B7F[PC](R0)", "movs.hw
  100C00(R1)") and reaches past the next named place: what it reads is that table, and the places it
  passes are names inside it (background_tile_banks, read by background up to 66, on into
  stage_tile_banks; the sine table on into the cosine one).
Either way the second name is a name inside the first table, and the assets keep them in one piece. The
memory test's sums of the chips (the board replaces them, runtime/replaced.txt) read the whole ROM and are
left out. Writes read_crossings.txt in the repository: the addresses and names, no byte of the ROM.
"""
import re
import sys
from bisect import bisect_right
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT / "tools"), str(ROOT / "tools" / "core")]
import data_formats  # noqa: E402

REPLACED_READERS = {0x01C9A3, 0x01C9AE}    # test_mode_memory_test_checksum's loop: the board's sums instead
OUTPUT = ROOT / "read_crossings.txt"


def places():
    """[(address, name)] of the program's named places, as the extractor cuts its data"""
    found = data_formats.pieces(ROOT / "symbols.txt", 0, 0x200000)
    header = (ROOT / "src" / "ram.h").read_text(encoding="ascii")
    addresses = dict(re.findall(r"^#define (\w+)_ADDRESS 0x([0-9A-F]+)u", header, re.M))
    found += [(int(addresses[name], 16), name) for name in re.findall(r"^extern const void \*(\w+)_data;", header, re.M)]
    return sorted(set(found))


INDEXED_MODES = ("pc_disp_indexed", "direct_indexed")    # a fixed base and an index register


def table_bases(image):
    """{PC: the fixed bases its operands index from} of the instructions the census names, decoded"""
    from v60 import disassembler as dis
    memory = dis.FlatMemory(image, 0xFFFFFF)
    found = {}

    def bases(pc):
        if pc not in found:
            try:
                instruction = dis.decode(memory, pc)
                found[pc] = [operand.value for operand in instruction.operands if operand.mode in INDEXED_MODES]
            except Exception:
                found[pc] = []
        return found[pc]
    return bases


def main():
    import orunners_rom
    census = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "build" / "rom_reads"
    named = places()
    starts = [address for address, _ in named]
    bases = table_bases(orunners_rom.build_program(orunners_rom.load_set(ROOT / "rom")))
    crossings = set()
    reach = {}    # PC -> the farthest byte it reads
    for path in sorted(census.glob("*.txt")):
        for line in path.read_text(encoding="ascii").splitlines():
            pc, span = line.split()
            if int(pc, 16) in REPLACED_READERS:
                continue
            first, last = (int(value, 16) for value in span.split("-"))
            reach[int(pc, 16)] = max(reach.get(int(pc, 16), 0), last)
            # the named places strictly inside the run: each one the run reads on into
            at = bisect_right(starts, first)
            while at < len(named) and named[at][0] <= last:
                crossings.add(named[at])
                at += 1
    # the named places between a fixed base and the farthest byte an instruction reads from it
    for pc, last in reach.items():
        for base in bases(pc):
            if not base <= last < base + 0x10000:
                continue
            at = bisect_right(starts, base)
            while at < len(named) and named[at][0] <= last:
                crossings.add(named[at])
                at += 1
    lines = ["# ADDRESS NAME: named places the original's reads run into from the place before (tools/read_crossings.py,",
             "# from the census of its data reads over the scenarios): names inside a table, which the assets",
             "# keep in the piece of the table"]
    lines += [f"{address:06X} {name}" for address, name in sorted(crossings)]
    OUTPUT.write_text("\n".join(lines) + "\n", encoding="ascii")
    print(f"{len(crossings)} names inside tables the code reads across")
    return 0


if __name__ == "__main__":
    sys.exit(main())
