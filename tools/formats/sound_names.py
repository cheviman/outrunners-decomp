"""
Name:        sound_names.py
Description: the names of sound_symbols.txt for the sound program's data formats: where a named place is in
             the sound ROM, and the name of a place
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

A module of helpers only (no FORMATS: data_formats gathers none from it).

The sound ROM (epr-15550, 512 KB) as the Z80 sees it: 0000-9FFF is the start of the chip, A000-BFFF an 8 KB
bank of it (port ROM_BANK). sound_symbols.txt names the pieces of data with 'data', 'table' and 'jumps' lines
(Z80 addresses below A000, which are the chip's own offsets there) and 'banked' lines (offsets in the chip,
A000 and up), and the code with 'func', 'code' and 'loop' lines.

A place is written as the name of the piece it is in: NAME at the piece's start, NAME+0xNN inside it; a code
address by its label. While data_formats builds the chip back, a name is where that build lays the piece out
(data_formats.PLACES); else where sound_symbols.txt puts it. A number (0x....) is read too. resolve() is the
name reader the typed format's from_text takes; write_names() writes names.txt beside the pieces, every name
and its offset, which is all the game needs to read them.
"""
import re
from bisect import bisect_right
from pathlib import Path

SYMBOLS = Path(__file__).resolve().parents[2] / "sound_symbols.txt"
WINDOW = 0xA000         # below: the chip as the Z80 sees it; A000-BFFF: a bank of it
BANK_SIZE = 0x2000

_NAMES = {}


def load(path=None):
    """Reads the names again (from 'path' when given)."""
    global SYMBOLS
    if path is not None:
        SYMBOLS = Path(path)
    _NAMES.clear()


def _names():
    """{'starts': sorted offsets where a piece or the code starts, 'at': {offset: name, None for code},
    'code': {address: label}, 'place': {name: offset}}."""
    if _NAMES:
        return _NAMES
    at, code, place = {}, {}, {}
    lines = SYMBOLS.read_text(encoding="utf-8").splitlines() if SYMBOLS.exists() else []
    for line in lines:
        match = re.match(r"([0-9A-F]{4,6})\s+(data|table|jumps|banked|func|code|loop)\s+(\w+)", line)
        if not match:
            continue
        offset, kind, name = int(match.group(1), 16), match.group(2), match.group(3)
        if kind in ("func", "code", "loop"):
            code.setdefault(offset, name)
            at.setdefault(offset, None)
        elif (kind == "banked") == (offset >= WINDOW):
            at[offset] = name
        else:
            continue
        place.setdefault(name, offset)
    _NAMES.update({"starts": sorted(at), "at": at, "code": code, "place": place})
    return _NAMES


def name_of(offset):
    """The name of the place 'offset' of the chip: a code label, NAME or NAME+0xNN; None when it is inside
    the code, or before any name."""
    names = _names()
    if offset in names["code"]:
        return names["code"][offset]
    index = bisect_right(names["starts"], offset) - 1
    if index < 0:
        return None
    start = names["starts"][index]
    name = names["at"][start]
    if name is None:
        return None
    return name if start == offset else f"{name}+0x{offset - start:X}"


def place_of(text):
    """The offset in the chip of a place written as name_of writes it, or as a number."""
    if re.fullmatch(r"[-+]?(0x[0-9A-Fa-f]+|\d+)", text):
        return int(text, 0)
    name, _, offset = text.partition("+")
    import data_formats
    if name in data_formats.PLACES:
        start = data_formats.PLACES[name]
    elif name in _names()["place"]:
        start = _names()["place"][name]
    else:
        raise ValueError(f"no place named {name}")
    return start + (int(offset, 0) if offset else 0)


def resolve(text):
    """place_of for typed.from_text: the offset of a name, or None when there is no such name."""
    try:
        return place_of(text)
    except ValueError:
        return None


def write_names(folder):
    """names.txt beside the sound program's data: the offset in the chip of every name sound_symbols.txt
    gives (the pieces of data, the code's functions and labels), which a piece may give for a place; the game
    reads it to read those pieces. Returns how many files."""
    lines = ["# NAME OFFSET: the names a piece of assets/sound/data may give for a place (sound_symbols.txt), and",
             "# the place's offset in the sound program's chip (512 KB; NAME+0xNN is that offset plus NN)"]
    lines += [f"{name} {offset:06X}" for name, offset in sorted(_names()["place"].items())]
    (Path(folder) / "names.txt").write_text("\n".join(lines) + "\n", encoding="ascii")
    return 1


def pointer_text(value):
    """A 16-bit pointer of the fixed part of the chip (a Z80 address below A000) as a name, or as 0xNNNN
    when it has none."""
    name = name_of(value) if value < WINDOW else None
    return name if name else f"0x{value:04X}"


def pointer_value(text):
    """A pointer pointer_text wrote, back to its 16 bits."""
    value = place_of(text)
    if not 0 <= value <= 0xFFFF:
        raise ValueError(f"{text} is not a 16-bit address")
    return value
