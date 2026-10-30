"""
Name:        sprite_sheets.py
Description: writes sprite_sheets.txt, the map of the sprite ROM's pieces into sheets that
             extract_assets.py draws: a sheet per family of objects (or stretch of animations), a row per
             animation
Version:     0.1.0
Created:     25-10-2026
Modified:    30-10-2026
Author:      Sebastian Toyos

Usage: py tools/sprite_sheets.py          (reads rom/ and src/; writes sprite_sheets.txt)

The sprite ROM's graphics are cut by the piece records at 0x122B74 (sprite_records in assets.json: 12
bytes each, a piece's size and place in the sprite ROM). An object shows an animation of rom_034138: a
record whose frames are lists of pieces, each piece naming a record (and the next ones, the same piece
smaller, by the object's size_step). A record goes to the sheet of the first animation that reaches it,
a record none reaches to the sheet of the record before it. An animation goes to the family whose code
starts it with a constant (animation_start after "r9 = N", or N written to the object's animation); the
rest, by their number, to sheets of their own stretch of animations.

The file holds record numbers and names only: 'NNNN sheet PATH # description' starts a sheet at record
NNNN (up to the next sheet line; a sheet may take several stretches), 'NNNN row NAME' a row. It can be
edited by hand: the sheets follow it.
"""
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import orunners_rom  # noqa: E402

ANIMATIONS = 0x034138      # rom_034138: the animations' records
RECORDS = 0x122B74         # the sprite pieces' records
RECORD = 12
STRETCH = 64               # animations of no known family: a sheet a stretch of this many


def animations(program):
    """[the address of each animation's record], in number order."""
    found = []
    at = ANIMATIONS
    while not found or at < min(found):
        address = struct.unpack_from("<I", program, at)[0]
        if not 0x1000 <= address < len(program):
            break
        found.append(address)
        at += 4
    return found


def pieces_of(program, record, with_flags=False):
    """The piece records an animation's frames name (their numbers, size_step 0); with_flags: (number, the
    piece's first sprite word) pairs (bit 9: 8 bits a pen)."""
    numbers = []
    for index in range(1024):
        entry = struct.unpack_from("<i", program, record + 4 + index * 4)[0]
        if entry <= 0:
            break
        count = struct.unpack_from("<H", program, entry)[0]
        for piece in range(min(count, 256)):
            word0, size_index = struct.unpack_from("<HH", program, entry + 2 + piece * 8)
            numbers.append((size_index // 3, word0) if with_flags else size_index // 3)
    return numbers


def record_count(program):
    count = 0
    while RECORDS + (count + 1) * RECORD <= len(program):
        size = struct.unpack_from("<H", program, RECORDS + count * RECORD)[0]
        if size & 0xFF == 0 or size >> 8 == 0:
            break
        count += 1
    return count


def families_by_animation():
    """{animation number: family} from the C: a constant put in r9 before animation_start, or written to
    an object's animation field, in a function of a family (its name up to the scoped family symbols.txt
    knows, else the function's name)."""
    symbols = (ROOT / "symbols.txt").read_text(encoding="utf-8")
    families = sorted({family for family in re.findall(r"^scoped\s+object\s+(\w+)\s", symbols, re.M)},
                      key=len, reverse=True)
    found = {}
    function_line = re.compile(r"^(?:static )?\w[\w *]*?\b(\w+)\(.*\)$")
    for path in sorted(path for path in (ROOT / "src").rglob("*.c") if "sound" not in path.parts):
        current, constant = None, None
        for line in path.read_text(encoding="utf-8").splitlines():
            match = function_line.match(line)
            if match:
                name = match.group(1)
                current = next((family for family in families if name == family or name.startswith(family + "_")),
                               name)
                constant = None
                continue
            match = re.search(r"\br9 = 0x([0-9A-F]+)u;", line) or re.search(r"\br9 = (\d+)u;", line)
            if match:
                constant = int(match.group(1), 16 if "0x" in match.group(0) else 10)
            if "animation_start(r9)" in line and constant is not None and current:
                found.setdefault(constant, current)
            match = re.search(r"->animation = 0x([0-9A-F]+)u;", line) or re.search(r"->animation = (\d+)u;", line)
            if match and current:
                found.setdefault(int(match.group(1), 16 if "0x" in match.group(0) else 10), current)
    return found


def main():
    program = orunners_rom.build_program(orunners_rom.load_set(ROOT / "rom"))
    count = record_count(program)
    owners = {}
    for number, record in enumerate(animations(program)):
        for piece in pieces_of(program, record):
            if piece < count:
                owners.setdefault(piece, number)
    names = families_by_animation()
    lines = ["# The sprite ROM's pieces in sheets (tools/sprite_sheets.py writes it; it can be edited by hand):",
             "# 'NNNN sheet PATH # description' starts a sheet at piece record NNNN, up to the next sheet line",
             "# (a sheet may take several stretches); 'NNNN row NAME' starts a row. Records by number (the",
             "# records at 0x122B74, 12 bytes each)."]
    sheet = row = None
    owner = None
    for number in range(count):
        owner = owners.get(number, owner)
        if owner is None:
            wanted_sheet, wanted_row = "unreached/before_animations", "records"
        else:
            family = names.get(owner)
            wanted_sheet = (f"{family}/{family}" if family
                            else f"animations/animations_{owner // STRETCH * STRETCH:03X}")
            wanted_row = f"animation_{owner:03X}"
        if wanted_sheet != sheet:
            description = (f"the pieces of {names[owner]}'s animations" if owner is not None and owner in names
                           else "the pieces of animations no constant in the code names")
            lines.append(f"{number:04X} sheet {wanted_sheet}  # {description}")
            sheet, row = wanted_sheet, wanted_row
        elif wanted_row != row:
            lines.append(f"{number:04X} row {wanted_row}")
            row = wanted_row
    (ROOT / "sprite_sheets.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
    sheets = {line.split()[2] for line in lines if " sheet " in line}
    print(f"{count} records, {len(owners)} reached by an animation; {len(names)} animations of a known family; "
          f"{len(sheets)} sheets")


if __name__ == "__main__":
    main()
