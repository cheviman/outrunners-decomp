"""
Name:        program_walk.py
Description: reads a piece of the program's data by what points into it: a table of addresses at its start,
             the lists they reach, the records the lists' entries point at; for the format modules
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

A module of helpers only (no FORMATS: data_formats gathers none from it).

A piece that holds lists and the records they point at (road_object_streams, stage_road_object_lists,
stage_background_object_lists...) is no table: its parts are where its own addresses say. A Walk starts at the
places a format gives it, reads each part with the reader of its kind (which may want more places: the records
an entry points at), and writes the parts in the order of their places, each under the 'record' lines of the
typed format (tools/core/formats/typed.py), whose from_text reads it back. A place inside the piece is written
NAME+0xNN (NAME the piece's name: the typed format has no labels), the first line of each part says its own;
what no part covers, and a part that would run over another, stays as 'bytes' lines, so the piece goes back
byte for byte whatever its readers make of it.
"""
import struct
from bisect import bisect_right

from formats import common, typed


def nearest_place(value):
    """A program ROM address as NAME+0xNN after the named place before it (however far), or None: for the
    values a format knows to be addresses (typed.name_for names only the ones near a name)."""
    if not 0x400 <= value < 0x200000:
        return None
    names = common._names()
    index = bisect_right(names["named"], value) - 1
    if index < 0:
        return None
    base = names["named"][index]
    text = names["address"][base] + (f"+0x{value - base:X}" if value != base else "")
    return text if common.address_value(text, text) == value else None


def value_text(value, kind):
    """A value as the typed format writes it: unsigned in hex, signed in decimal."""
    if kind[0] == "s":
        return str(value)
    return f"0x{value:0{typed.SIZES[kind] * 2}X}"


class Walk:
    def __init__(self, data, piece):
        typed.format_for(piece)     # (symbols.txt's structures, for record_columns)
        self.data = data
        self.piece = piece
        self.base = common.address_value(piece, piece)
        self.parts = {}             # offset -> (size, [(cells, comment)])
        self.kinds = {}             # offset -> the kind of the part there
        self.queue = []

    # ---- places ----

    def offset_of(self, value):
        """The offset in the piece of address 'value', or None when it is outside."""
        return value - self.base if self.base <= value < self.base + len(self.data) else None

    def place(self, value):
        """An address as a name: another place's own name, NAME+0xNN inside this piece, NAME+0xNN after the
        named place before it; 0 as 0; else in hex."""
        if value == 0:
            return "0"
        name = typed.name_for(value)
        offset = self.offset_of(value)
        if offset is not None and (name is None or name == self.piece or name.startswith(self.piece + "+")):
            return self.piece if offset == 0 else f"{self.piece}+0x{offset:X}"
        return name or nearest_place(value) or f"0x{value:08X}"

    def want(self, value, kind):
        """Reads the part at address 'value' (if it is inside the piece) as 'kind'."""
        offset = self.offset_of(value)
        if offset is not None:
            self.queue.append((offset, kind))

    def want_offset(self, offset, kind):
        self.queue.append((offset, kind))

    # ---- reading ----

    def u16(self, offset):
        return struct.unpack_from("<H", self.data, offset)[0]

    def s16(self, offset):
        return struct.unpack_from("<h", self.data, offset)[0]

    def u32(self, offset):
        return struct.unpack_from("<I", self.data, offset)[0]

    def fits(self, offset, size):
        return 0 <= offset and offset + size <= len(self.data)

    def run(self, readers):
        """Reads every part wanted, by its kind: readers[kind](walk, offset) -> (size, rows) or None (no such
        part there: its bytes stay as they are)."""
        done = set()
        while self.queue:
            offset, kind = self.queue.pop(0)
            if (offset, kind) in done or offset in self.parts:
                continue
            done.add((offset, kind))
            found = readers[kind](self, offset)
            if found is not None and found[0] > 0:
                self.parts[offset] = found
                self.kinds[offset] = kind

    def write(self, out, gap_reader=None):
        """The parts in the order of their offsets into the writer, the bytes between them as 'bytes' lines;
        or, when gap_reader(walk, start, end, kind of the part before) reads them (as rows), those rows."""
        at, kind = 0, None

        def gap(end):
            rows = gap_reader(self, at, end, kind) if gap_reader is not None and end > at else None
            if rows is None:
                out.raw(self.data[at:end])
                return
            for index, (cells, comment) in enumerate(rows):
                out.row(cells, (f"+0x{at:X} " if index == 0 else "") + (comment or "") or None)
        for offset in sorted(self.parts):
            size, rows = self.parts[offset]
            if offset < at:
                continue        # inside the part before it (its bytes are written there)
            if offset > at:
                gap(offset)
            for index, (cells, comment) in enumerate(rows):
                if index == 0:
                    comment = f"+0x{offset:X}" + (f" {comment}" if comment else "")
                out.row(cells, comment)
            at, kind = offset + size, self.kinds.get(offset)
        gap(len(self.data))


def address_table(keyword, kind):
    """The reader of a table of addresses of parts of 'kind' in the piece (the streams, the lists by stage...):
    as many as come before the first part they reach, a line each ('keyword' and the place, its index after it)."""
    def read(walk, offset):
        rows, at, first = [], offset, len(walk.data)
        while at < first and walk.fits(at, 4) and walk.offset_of(walk.u32(at)) is not None:
            target = walk.u32(at)
            walk.want(target, kind)
            first = min(first, walk.offset_of(target))
            rows.append(([keyword, walk.place(target)], f"[{(at - offset) // 4}]"))
            at += 4
        return (at - offset, rows) if rows else None
    return read


# ---- records as symbols.txt's structures lay them out ----

def record_columns(structure):
    """[(name, offset, value type)] of a structure of symbols.txt, or None."""
    typed.format_for(structure)
    layout = typed._LAYOUTS.get(structure)
    return None if layout is None else layout[1]


def record_size(structure):
    return typed.element_size(structure)


def declare(out, keyword, structure, extra=()):
    """The 'record' line of a structure's records; 'extra' columns (column texts) after its own."""
    columns = record_columns(structure)
    out.record(keyword, record_size(structure) + sum(typed.SIZES[text.split(":")[1].split("=")[0]] for text in extra),
               [typed.column(name, kind) for name, _, kind in columns] + list(extra))


def record_cells(walk, offset, structure, pointers=()):
    """The cells of the structure's record at 'offset': its values, the u32 columns named in 'pointers' (the
    ones that hold addresses) as places."""
    cells = []
    for name, at, kind in record_columns(structure):
        value = struct.unpack_from("<" + typed.VALUE_TYPES[kind], walk.data, offset + at)[0]
        if kind == "u32" and name in pointers:
            cells.append(walk.place(value))
        else:
            cells.append(value_text(value, kind))
    return cells
