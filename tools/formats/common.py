"""
Name:        common.py
Description: what the format modules share: the names of symbols.txt, values as text, records of fields
             and the two ways their lines are written and read back
Version:     0.1.0
Created:     14-10-2026
Modified:    15-10-2026
Author:      Sebastian Toyos

A module of helpers only (no FORMATS: tools/data_formats.py gathers none from it).

Values: numbers are 0x.. hex or decimal, either with a sign; sprite frames read as their sheet and index
(sonic/walking[3]: the fourth frame of assets/sprites/sonic/walking.png, as sprite_sheets.txt lays the
sheets out), or frame_XXXX (the offset of the frame's record in sprite_frame_records); program addresses
read 0x0XXXXX, with the name symbols.txt gives the place (name+0xNN inside a named data piece; a
variable's name for a work RAM address): the address is what goes back, a name in its place is read too.

A record is fields (name, offset, struct code, style) that tile its bytes; its lines are written one of two
ways:
- in columns (Out, reader): a keyword, then 'field value' pairs, the names of addresses after them, the
  records of one shape lined up; comments after a record say its index or what it is for;
- labelled (labelled_table, labelled_list): a keyword, the record's label (its index), then 'field value'
  pairs; the names of addresses go in a comment at the end.
Either way 'bytes XX ...' and 'words XXXX ...' lines (hex, words little endian) keep what no record
explains, so every piece comes back byte for byte, and '#' starts a comment. Offsets the code adds to a
base other than the piece read as the address they reach; offsets from the piece's own start read +XXXX.

A piece never changes size (the code reaches every piece at a fixed address): edit the numbers of a
record, do not add or remove lines. Where symbols.txt cuts a table where the code reads a field, the piece
starts in the middle of a record: phased_formats makes a format for each field a cut may start at
(NAME_from_FIELD), whose lines hold only the fields of their record that lie in the piece.
"""
import re
import struct
from bisect import bisect_right
from decimal import ROUND_HALF_EVEN, Decimal
from pathlib import Path

import orunners_rom

ROOT = Path(__file__).resolve().parents[2]
WORK_RAM = 0x208000     # R25: the work RAM page the game's variables are offsets into


# ---- names of symbols.txt ----

_NAMES = {}


def _names():
    """{'address': {address: name} of the func, code, data and table lines, 'name': {name: address} of
    every line, 'named': their sorted addresses, 'data': the ones that are data or table lines,
    'variables': {address: name} of the var and io lines}."""
    if _NAMES:
        return _NAMES
    by_address, by_name, data, variables = {}, {}, set(), {}
    path = ROOT / "symbols.txt"
    lines = path.read_text(encoding="utf-8").splitlines() if path.exists() else []
    for line in lines:
        match = re.match(r"([0-9A-F]{6})\s+(func|code|data|table|var|io)\s+(\w+)", line)
        if not match:
            continue
        address, kind, name = int(match.group(1), 16), match.group(2), match.group(3)
        by_name.setdefault(name, address)
        if kind in ("var", "io"):
            variables.setdefault(address, name)
        elif address not in by_address:
            by_address[address] = name
            if kind in ("data", "table"):
                data.add(address)
    _NAMES.update({"address": by_address, "name": by_name, "named": sorted(by_address), "data": data,
                   "variables": variables})
    return _NAMES


def name_of(value):
    """The name of program address 'value' (mirrors folded): its own, or name+0xNN inside a named data
    piece; outside the program ROM, the variable or port symbols.txt names there; None when it has none
    (and for 0, a null pointer)."""
    if value == 0:
        return None
    address = orunners_rom.canonical_address(value) if 0 <= value <= 0xFFFFFF else None
    names = _names()
    if address is None:
        return names["variables"].get(value)
    name = names["address"].get(address)
    if name is not None:
        return name
    index = bisect_right(names["named"], address) - 1
    if index >= 0:
        base = names["named"][index]
        if base in names["data"] and address - base < 0x1000:
            return f"{names['address'][base]}+0x{address - base:X}"
    return None


def address_value(token, where):
    """A program address from '0x..' or a name (name, name+0xNN)."""
    if token.lower().startswith("0x"):
        return int(token, 16)
    name, _, offset = token.partition("+")
    names = _names()
    if name not in names["name"]:
        raise ValueError(f"{where}: unknown name '{name}'")
    return names["name"][name] + (int(offset, 16) if offset else 0)


# ---- values ----

SIZES = {"B": 1, "b": 1, "H": 2, "h": 2, "I": 4, "i": 4}


def number(token, where):
    """A number: 0x.. hex, else decimal; either may carry a sign."""
    try:
        sign = -1 if token.startswith("-") else 1
        body = token.lstrip("+-")
        if body.lower().startswith("0x"):
            return sign * int(body[2:], 16)
        return sign * int(body, 10)
    except ValueError:
        raise ValueError(f"{where}: '{token}' is not a number") from None


_SHEETS = {}


def _sheets():
    """{'name': {frame number: 'sheet[index]'}, 'number': the reverse}: where sprite_sheets.txt puts each
    frame of assets.json's sprite asset (assets/sprites/SHEET.png, the index-th frame of the sheet)."""
    if _SHEETS:
        return _SHEETS
    import json
    import asset_formats
    names = {}
    manifest, sheets_map = ROOT / "assets.json", ROOT / "sprite_sheets.txt"
    if manifest.exists() and sheets_map.exists():
        assets = json.loads(manifest.read_text(encoding="utf-8"))["assets"]
        asset = next((asset for asset in assets if asset["type"] == "sprite_frames"), None)
        if asset is not None:
            numbers = [index * asset_formats.SPRITE_RECORD for index in range(asset["count"])]
            _, where = asset_formats.load_sprite_sheets(sheets_map, numbers)
            names = {number: f"{path}[{index}]" for number, (path, index) in where.items()}
    _SHEETS.update({"name": names, "number": {name: number for number, name in names.items()}})
    return _SHEETS


def frame_text(value):
    """A sprite frame word: the frame's sheet and its index there (sonic/walking[3]: the fourth frame of
    assets/sprites/sonic/walking.png), frame_XXXX for a frame no sheet holds (the offset of its record in
    sprite_frame_records, 8 bytes a record), else 0xXXXX."""
    if value % 8:
        return f"0x{value:04X}"
    return _sheets()["name"].get(value, f"frame_{value:04X}")


def frame_value(token, where):
    if token.endswith("]"):
        if token not in _sheets()["number"]:
            raise ValueError(f"{where}: '{token}' is no frame of a sprite sheet")
        return _sheets()["number"][token]
    if token.startswith("frame_"):
        try:
            return int(token[6:], 16)
        except ValueError:
            raise ValueError(f"{where}: '{token}' is not a sprite frame") from None
    return number(token, where)


def fixed_text(raw):
    """A 16.16 value as the shortest decimal that comes back to it."""
    exact = Decimal(raw) / 65536
    for places in range(7):
        text = str(exact.quantize(Decimal(1).scaleb(-places), rounding=ROUND_HALF_EVEN))
        if fixed_value(text) == raw:
            return text
    return str(exact)


def fixed_value(text):
    return int((Decimal(text) * 65536).to_integral_value(rounding=ROUND_HALF_EVEN))


# ---- fields and records ----

DECORATED = ("ptr", "off")      # styles whose value may be followed by a name


class Field:
    """One field of a record: its name, its offset in the record, its struct code and how it reads: 'd'
    decimal, 'x' hex, 'frame' a sprite frame, 'ptr' a program address (u32), ('off', base) a u16 offset
    from 'base' written as the address it reaches, 'rel' a u16 offset from the start of the piece (+XXXX),
    'fixed' a 16.16 value as a decimal (as many places as it takes to come back exactly), ('index', size) a
    byte offset into a table of 'size'-byte records written as the record number (+0xNN when it falls
    inside one)."""

    def __init__(self, name, offset, code, style="d"):
        self.name, self.offset, self.code, self.style = name, offset, code, style
        self.size = SIZES[code]
        self.kind = style[0] if isinstance(style, tuple) else style

    def value(self, data, at):
        """The field's value in the record at data[at:]."""
        return struct.unpack_from("<" + self.code, data, at + self.offset)[0]

    def shown(self, data, at):
        """(the field's value as text, the name of the place it reaches or None), the record at data[at:]."""
        value = self.value(data, at)
        if self.kind == "x":
            return f"0x{value:0{self.size * 2}X}", None
        if self.kind == "frame":
            return frame_text(value), None
        if self.kind == "rel":
            return f"+{value:04X}", None
        if self.kind == "fixed":
            return fixed_text(value), None
        if self.kind == "index":
            size = self.style[1]
            return (str(value // size) if value % size == 0 else f"+0x{value:X}"), None
        if self.kind in DECORATED:
            address = value if self.kind == "ptr" else self.style[1] + value
            return f"0x{address:06X}", name_of(address)
        return str(value), None

    def text(self, data, at):
        """The value as text with the name of the place it reaches after it."""
        value, name = self.shown(data, at)
        return f"{value} {name}" if name else value

    def pack(self, token, where):
        if self.kind == "frame":
            value = frame_value(token, where)
        elif self.kind == "rel":
            value = int(token.lstrip("+"), 16) if re.fullmatch(r"\+?[0-9A-Fa-f]+", token) else number(token, where)
        elif self.kind == "fixed":
            try:
                value = fixed_value(token)
            except ArithmeticError:
                raise ValueError(f"{where}: '{token}' is not a number") from None
        elif self.kind == "index":
            value = number(token[1:], where) if token.startswith("+") else number(token, where) * self.style[1]
        elif self.kind in DECORATED:
            value = address_value(token, where)
            if self.kind == "off":
                value -= self.style[1]
                if not 0 <= value <= 0xFFFF:
                    raise ValueError(f"{where}: {self.name} {token} is out of reach of 0x{self.style[1]:06X}")
        else:
            value = number(token, where)
        bits = self.size * 8
        if not -(1 << (bits - 1)) <= value < (1 << bits):
            raise ValueError(f"{where}: {self.name} {token} does not fit in {bits} bits")
        return (value & ((1 << bits) - 1)).to_bytes(self.size, "little")


class Rec:
    """A record: a keyword and fields (name, offset, struct code, style) that, with the 'fixed' bytes
    ({offset: bytes}, what the record must hold there, not written), tile its 'size' bytes. A 'phased'
    record may come cut by its piece: a line then holds the run of its fields that lies in the piece."""

    def __init__(self, keyword, size, fields, fixed=None, phased=False):
        self.keyword, self.size, self.phased = keyword, size, phased
        self.fields = [Field(*field) for field in fields]
        self.fixed = fixed or {}
        self.by_name = {field.name: field for field in self.fields}
        covered = bytearray(size)
        for field in self.fields:
            covered[field.offset:field.offset + field.size] = b"\1" * field.size
        for offset, value in self.fixed.items():
            covered[offset:offset + len(value)] = b"\1" * len(value)
        if len(covered) != size or not all(covered):
            raise ValueError(f"record '{keyword}': its fields do not tile its {size} bytes")

    def fits(self, data, at):
        return 0 <= at and at + self.size <= len(data) and \
            all(data[at + offset:at + offset + len(value)] == value for offset, value in self.fixed.items())

    def tiles(self, lo, hi):
        """Whether whole fields cover the record's bytes lo..hi."""
        at = lo
        for field in self.fields:
            if field.offset == at and at + field.size <= hi:
                at += field.size
        return at == hi

    def within(self, lo=0, hi=None):
        """The fields that lie in the record's bytes lo..hi."""
        hi = self.size if hi is None else hi
        return [field for field in self.fields if lo <= field.offset and field.offset + field.size <= hi]

    def pairs(self, data, at, lo=0, hi=None):
        """[(name, value text)] of the fields within the record's bytes lo..hi (the record at data[at:])."""
        return [(field.name, field.text(data, at)) for field in self.within(lo, hi)]

    def pack(self, tokens, where):
        values = {}
        index = 0
        while index < len(tokens):
            name = tokens[index]
            field = self.by_name.get(name)
            if field is None:
                raise ValueError(f"{where}: no field '{name}' in a '{self.keyword}' line")
            if index + 1 >= len(tokens):
                raise ValueError(f"{where}: '{name}' without a value")
            if name in values:
                raise ValueError(f"{where}: '{name}' twice")
            values[name] = tokens[index + 1]
            index += 2
            if field.kind in DECORATED and index < len(tokens) and tokens[index] not in self.by_name \
                    and values[name].lower().startswith("0x"):
                index += 1      # the name after an address: a reminder, not read
        missing = [field.name for field in self.fields if field.name not in values]
        if not self.phased or not missing:
            if missing:
                raise ValueError(f"{where}: '{missing[0]}' is missing")
            out = bytearray(self.size)
            for offset, value in self.fixed.items():
                out[offset:offset + len(value)] = value
            for field in self.fields:
                out[field.offset:field.offset + field.size] = field.pack(values[field.name], where)
            return bytes(out)
        # a record its piece cuts: the run of fields the line holds, one after the other
        fields = sorted((self.by_name[name] for name in values), key=lambda field: field.offset)
        out = bytearray()
        for index, field in enumerate(fields):
            if index and field.offset != fields[index - 1].offset + fields[index - 1].size:
                raise ValueError(f"{where}: a field is missing before '{field.name}'")
            out += field.pack(values[field.name], where)
        return bytes(out)


def run_of(keyword, *fields, phased=False):
    """A record whose fields (name, struct code[, style]) follow one another; without a style a field
    reads decimal when its code is signed (lower case), hex when it is not."""
    laid = []
    at = 0
    for name, code, *style in fields:
        laid.append((name, at, code, style[0] if style else ("d" if code.islower() else "x")))
        at += SIZES[code]
    return Rec(keyword, at, laid, phased=phased)


def u16_at(data, at):
    return struct.unpack_from("<H", data, at)[0] if at + 2 <= len(data) else None


def s16_at(data, at):
    return struct.unpack_from("<h", data, at)[0] if at + 2 <= len(data) else None


# ---- records in columns: writing, in the order of the bytes ----

def _is_number(text):
    return bool(text) and (text[0].isdigit() or text[0] in "-+") and " " not in text


class Out:
    """Builds a piece's text: records read at the current position, comments, and what no record
    explains as 'words' / 'bytes' lines."""

    def __init__(self, data, header=()):
        self.data = data
        self.at = 0
        self.items = [("comment", line) for line in header]

    def comment(self, text):
        self.items.append(("comment", text))

    def blank(self):
        self.items.append(("blank",))

    def rec(self, rec, comment=None):
        """The record at the current position, when it fits there; returns whether it did."""
        if not rec.fits(self.data, self.at):
            return False
        self.items.append(("rec", rec.keyword, rec.pairs(self.data, self.at), comment))
        self.at += rec.size
        return True

    def part(self, rec, lo, hi, comment=None):
        """The fields lo..hi of a phased record, at the current position."""
        self.items.append(("rec", rec.keyword, rec.pairs(self.data, self.at - lo, lo, hi), comment))
        self.at += hi - lo

    def special(self, line, size):
        """A line written by the format itself for the next 'size' bytes."""
        self.items.append(("line", line))
        self.at += size

    def raw(self, end=None, comment=None, words=True):
        """The bytes up to 'end' (the end of the piece) as words (bytes when odd or not 'words')."""
        end = len(self.data) if end is None else end
        chunk = self.data[self.at:end]
        if not chunk:
            return
        if comment:
            self.comment(comment)
        if words and len(chunk) % 2 == 0:
            values = [f"{value:04X}" for (value,) in struct.iter_unpack("<H", chunk)]
            self.items += [("line", "words " + " ".join(values[at:at + 8])) for at in range(0, len(values), 8)]
        else:
            values = [f"{value:02X}" for value in chunk]
            self.items += [("line", "bytes " + " ".join(values[at:at + 16])) for at in range(0, len(values), 16)]
        self.at = end

    def done(self, leftover="no reader of this piece reads these bytes: kept as they are"):
        if self.at < len(self.data):
            self.raw(comment=leftover)
        lines = []
        index = 0
        while index < len(self.items):
            item = self.items[index]
            if item[0] == "comment":
                lines.append(f"# {item[1]}" if item[1] else "#")
                index += 1
                continue
            if item[0] == "blank":
                lines.append("")
                index += 1
                continue
            if item[0] == "line":
                lines.append(item[1])
                index += 1
                continue
            # a run of records of the same shape: values in columns
            shape = (item[1], tuple(name for name, _ in item[2]))
            group = []
            while index < len(self.items) and self.items[index][0] == "rec" \
                    and (self.items[index][1], tuple(name for name, _ in self.items[index][2])) == shape:
                group.append(self.items[index])
                index += 1
            widths = [max(len(record[2][column][1]) for record in group) for column in range(len(shape[1]))]
            texts = []
            for _, keyword, pairs, comment in group:
                parts = [keyword]
                for column, (name, value) in enumerate(pairs):
                    last = column == len(pairs) - 1
                    if _is_number(value):
                        shown = value.rjust(widths[column])
                    else:
                        shown = value if last else value.ljust(widths[column])
                    parts.append(f"{name} {shown}")
                texts.append(("  ".join(parts), comment))
            width = max(len(text) for text, _ in texts)
            lines += [(text.ljust(width) + f"  # {comment}") if comment else text for text, comment in texts]
        return "".join(line.rstrip() + "\n" for line in lines)


# ---- records in columns: reading, one parser for every format ----

def tokens_of(line):
    """The tokens of a line up to its comment: words, and quoted strings kept whole (with their quotes)."""
    tokens = []
    at = 0
    while at < len(line):
        char = line[at]
        if char.isspace():
            at += 1
            continue
        if char == "#":
            break
        if char == '"':
            end = at + 1
            while end < len(line) and line[end] != '"':
                end += 2 if line[end] == "\\" else 1
            tokens.append(line[at:end + 1])
            at = end + 1
            continue
        end = at
        while end < len(line) and not line[end].isspace():
            end += 1
        tokens.append(line[at:end])
        at = end
    return tokens


def reader(*recs, specials=None):
    """from_text for a format whose lines are these records, 'bytes'/'words' lines and the 'specials'
    ({keyword: function(tokens, where) -> bytes})."""
    table = {}
    for rec in recs:
        if table.get(rec.keyword, rec) is not rec:
            raise ValueError(f"two records take the keyword '{rec.keyword}'")
        table[rec.keyword] = rec
    specials = specials or {}

    def from_text(text):
        out = bytearray()
        for line_number, line in enumerate(text.split("\n"), 1):
            tokens = tokens_of(line)
            if not tokens:
                continue
            keyword, where = tokens[0], f"line {line_number}"
            try:
                if keyword == "bytes":
                    out += bytes(int(token, 16) for token in tokens[1:])
                elif keyword == "words":
                    out += b"".join(struct.pack("<H", int(token, 16)) for token in tokens[1:])
                elif keyword in specials:
                    out += specials[keyword](tokens[1:], where)
                elif keyword in table:
                    out += table[keyword].pack(tokens[1:], where)
                else:
                    raise ValueError(f"{where}: '{keyword}' is not a line this file takes")
            except (ValueError, struct.error, OverflowError) as error:
                message = str(error)
                raise ValueError(message if message.startswith("line ") else f"{where}: {message}") from None
        return bytes(out)

    return from_text


def fmt(to_text, *recs, specials=None, suffix=".txt"):
    """A format: (suffix, to_text, the reader of these records and specials)."""
    return (suffix, to_text, reader(*recs, specials=specials))


# ---- records in columns: common shapes ----

def table_text(header, rec, count=None, comment=None):
    """to_text of a table of records (as many as fit, or 'count'), then what follows as hex;
    'comment(index)' gives each record's comment."""
    def to_text(data):
        out = Out(data, header)
        index = 0
        while (count is None or index < count) and out.rec(rec, comment(index) if comment else None):
            index += 1
        return out.done()
    return to_text


def phased_text(header, rec, phase, comment=None):
    """to_text of a table of 'rec' records cut by its pieces: the piece starts 'phase' bytes into a record;
    each line holds the fields of its record that lie in the piece (what does not split into whole fields
    stays as hex)."""
    def to_text(data):
        out = Out(data, header)
        position = -(phase % rec.size)
        while position < len(data):
            lo, hi = max(0, -position), min(rec.size, len(data) - position)
            index = (position + phase) // rec.size
            if rec.tiles(lo, hi):
                out.part(rec, lo, hi, comment(index) if comment else None)
            else:
                out.raw(position + hi)
            position += rec.size
        return out.done()
    return to_text


def phased_formats(name, rec, make):
    """{format name: make(phase)} for a table that symbols.txt cuts where the code reads a field: one
    format for each field a piece may start at, 'name' for the whole records and 'name_from_FIELD' for
    a piece that starts at FIELD."""
    return {name if field.offset == 0 else f"{name}_from_{field.name}": make(field.offset) for field in rec.fields}


def offset_lists_text(header, count, keyword, label, read_item, offset_field="list"):
    """to_text of 'count' u16 offsets from the start of the piece ('KEYWORD list +XXXX', the comment says
    whose: 'label' and the index), then what they reach, read one item at a time by read_item(out) (which
    returns False when nothing fits); a comment marks where each offset lands. Returns (to_text, the
    offsets' record)."""
    offsets_rec = Rec(keyword, 2, [(offset_field, 0, "H", "rel")])

    def to_text(data):
        out = Out(data, header)
        targets = {}
        for index in range(count):
            if out.at + 2 > len(data):
                break
            targets.setdefault(struct.unpack_from("<H", data, out.at)[0], []).append(index)
            out.rec(offsets_rec, f"{label} {index:02X}")
        while out.at < len(data):
            if out.at in targets:
                out.blank()
                users = targets[out.at]
                out.comment(f"+{out.at:04X}: {label}" + ("s " if len(users) > 1 else " ")
                            + ", ".join(f"{user:02X}" for user in users))
            if not read_item(out):
                break
        return out.done()

    return to_text, offsets_rec


def end_rec(keyword="end", value=b"\xff\xff"):
    """A terminator: the bytes 'value', written as the keyword alone."""
    return Rec(keyword, len(value), [], fixed={0: value})


END = end_rec()
POINT = Rec("point", 4, [("x", 0, "h"), ("y", 2, "h")])     # a pair of signed words: x, y


def single(keyword, code, style="d", header=()):
    """A piece that is one value: 'KEYWORD value value'."""
    rec = Rec(keyword, SIZES[code], [("value", 0, code, style)])

    def to_text(data):
        out = Out(data, header)
        out.rec(rec)
        return out.done()
    return fmt(to_text, rec)


def indexed(header, rec, label=None, count=None):
    """A table of records, one a line, the comment naming each one's index ('label' and the index in hex,
    or label(index) when it is a function)."""
    def comment(index):
        if label is None:
            return f"{index:02X}"
        return label(index) if callable(label) else f"{label} {index:02X}"
    return fmt(table_text(header, rec, count, comment), rec)


def byte_table(header, keyword, field, label, style="d", code="B"):
    """A table of single bytes ('code' B, or b for signed ones), one a line ('KEYWORD FIELD value')."""
    return indexed(header, Rec(keyword, 1, [(field, 0, code, style)]), label)


# ---- animations of frames inside records in columns: 'frames last N: FRAME ...' ----

def counted_words(out, keyword, show, limit=0x40):
    """A word with the last index N (its low byte; the high byte 0), then N + 1 words: one line
    'KEYWORD last N: w w ...'. Returns whether it fitted."""
    data = out.data
    header = u16_at(data, out.at)
    if header is None or header > limit or out.at + 2 + (header + 1) * 2 > len(data):
        return False
    words = struct.unpack_from(f"<{header + 1}H", data, out.at + 2)
    out.special(f"{keyword} last {header}: " + " ".join(show(word) for word in words), 2 + (header + 1) * 2)
    return True


def frames_line(out):
    """An animation of frames (the last step, then the frames) at the current position, as one line."""
    return counted_words(out, "frames", frame_text)


def frames_special(tokens, where):
    """'frames last N: FRAME ...' -> the header word N and the frame words."""
    if len(tokens) < 2 or tokens[0] != "last" or not tokens[1].endswith(":"):
        raise ValueError(f"{where}: 'frames last N: FRAME ...' expected")
    out = struct.pack("<H", number(tokens[1][:-1], where) & 0xFFFF)
    return out + b"".join(struct.pack("<H", frame_value(token, where) & 0xFFFF) for token in tokens[2:])


# ---- labelled records ----

KEPT = "past what the code reads here: kept as it is"


def content(text):
    """(line number, tokens) of the lines that hold something, comments left out."""
    for line_number, line in enumerate(text.split("\n"), 1):
        line = line.split("#", 1)[0].strip()
        if line:
            yield line_number, line.split()


def raw(tokens, where):
    """The bytes of a 'bytes XX ...' or 'words XXXX ...' line, or None when it is neither."""
    if tokens[0] not in ("bytes", "words"):
        return None
    try:
        if tokens[0] == "bytes":
            return bytes(int(token, 16) for token in tokens[1:])
        return b"".join(struct.pack("<H", int(token, 16)) for token in tokens[1:])
    except (ValueError, struct.error):
        raise ValueError(f"{where}: a {tokens[0]} line holds something that is not hex") from None


def hex_lines(data, note=None):
    """What no reader explains: words (bytes when the run is odd), with a note before them."""
    if not data:
        return []
    lines = [f"# {note}"] if note else []
    if len(data) % 2 == 0:
        words = [f"{word:04X}" for word in struct.unpack(f"<{len(data) // 2}H", data)]
        lines += ["words " + " ".join(words[at:at + 8]) for at in range(0, len(words), 8)]
    else:
        lines += ["bytes " + " ".join(f"{byte:02X}" for byte in data[at:at + 16]) for at in range(0, len(data), 16)]
    return lines


def join(header, lines):
    """The header as comment lines, then the lines."""
    return "".join(f"# {line}\n" if line else "#\n" for line in header) + "".join(line + "\n" for line in lines)


def labelled_line(rec, data, at, label=None, lo=0, hi=None, keyword=None):
    """'KEYWORD LABEL  field value  field value  # names' for the fields of the record at data[at:] that
    lie in its bytes lo..hi; the names of the places addresses reach go in the comment."""
    pairs, names = [], []
    for field in rec.within(lo, hi):
        value, name = field.shown(data, at)
        pairs.append(f"{field.name} {value}")
        if name:
            names.append(name)
    keyword = rec.keyword if keyword is None else keyword
    head = f"{keyword} {label}" if label is not None else keyword
    return f"{head}  {'  '.join(pairs)}" + (f"  # {', '.join(names)}" if names else "")


def record_tokens(tokens):
    """The tokens of a labelled line after its keyword, without its label (a lone first token)."""
    rest = tokens[1:]
    return rest[1:] if len(rest) % 2 else rest


def labelled_table(rec, header, count=None):
    """A table of records, one a line ('KEYWORD NN field value ...'), as many as fit (or 'count'); what
    follows is kept as hex. Returns (to_text, from_text)."""

    def to_text(data):
        lines = []
        total = len(data) // rec.size if count is None else min(count, len(data) // rec.size)
        for index in range(total):
            lines.append(labelled_line(rec, data, index * rec.size, f"{index:02X}"))
        lines += hex_lines(data[total * rec.size:], KEPT)
        return join(header, lines)

    def from_text(text):
        out = bytearray()
        for line_number, tokens in content(text):
            where = f"line {line_number}"
            found = raw(tokens, where)
            if found is not None:
                out += found
            elif tokens[0] == rec.keyword:
                out += rec.pack(record_tokens(tokens), where)
            else:
                raise ValueError(f"{where}: '{tokens[0]}' is not a line this file takes")
        return bytes(out)

    return to_text, from_text


def labelled_list(rec, header, end):
    """Records one after the other up to the 'end' bytes (written 'end'); what follows them is kept as
    hex. Returns (to_text, from_text)."""

    def to_text(data):
        lines = []
        at = 0
        while at < len(data):
            if data[at:at + len(end)] == end:
                lines.append("end")
                at += len(end)
                lines += hex_lines(data[at:], "after the end: no reader gets here")
                at = len(data)
                break
            if at + rec.size > len(data):
                break
            lines.append(labelled_line(rec, data, at))
            at += rec.size
        lines += hex_lines(data[at:], KEPT)
        return join(header, lines)

    def from_text(text):
        out = bytearray()
        for line_number, tokens in content(text):
            where = f"line {line_number}"
            found = raw(tokens, where)
            if found is not None:
                out += found
            elif tokens == ["end"]:
                out += end
            elif tokens[0] == rec.keyword:
                out += rec.pack(tokens[1:], where)
            else:
                raise ValueError(f"{where}: '{tokens[0]}' is not a line this file takes")
        return bytes(out)

    return to_text, from_text
