"""
Name:        data_formats.py
Description: the program ROM's and the sound program's data as readable files by subject, and back, byte
             for byte
Version:     0.1.0
Created:     14-10-2026
Modified:    25-10-2026
Author:      Sebastian Toyos

The data of the program ROM (the data area, 0x100000-0x17FFFF, and what the code chips hold besides the
code, at 0x000000-0x01FFFF and 0x080000-0x09FFFF: the tables, palettes, scripts, level data the code
reads) is cut where symbols.txt names a place into pieces, the code left out (the C replaces it; the build
takes those bytes from the ROM). Each piece is written in a format that says what it is, and goes to the
text file of its subject as a section (stages/stage_08.txt holds stage 8's spawn lists, palettes,
tilemaps and its objects' tables): data_formats.json lists the files, each with its description and its
pieces' formats. A piece no file lists goes with the one before it; one with no format is written as 16-bit
words in hex (bytes when its size is odd), which goes back just as well; a run of one byte (padding) is
written in layout.json. The sound program's chip goes the same way, cut where sound_symbols.txt names a
place (sound_formats.json: the driver's tables by subject, and the music, the effects and the voices as
MIDI files, a file each in their folders). Each format is a pair: to_text(bytes) -> text, and
from_text(text) -> bytes, with from_text(to_text(data)) == data.

The program ROM is the V60's and the sound program the Z80's: both little endian.
"""
import json
import re
import struct


# ---- the formats ----

def words_to_text(data):
    """16-bit words, eight a line, hex."""
    words = struct.unpack(f"<{len(data) // 2}H", data)
    return "".join(" ".join(f"{word:04X}" for word in words[at:at + 8]) + "\n" for at in range(0, len(words), 8))


def words_from_text(text):
    return b"".join(struct.pack("<H", int(word, 16)) for word in text.split())


def bytes_to_text(data):
    """Bytes, sixteen a line, hex."""
    return "".join(" ".join(f"{byte:02X}" for byte in data[at:at + 16]) + "\n" for at in range(0, len(data), 16))


def bytes_from_text(text):
    return bytes(int(byte, 16) for byte in text.split())


def palette_to_text(data):
    """Palette entries as the game keeps them (the palette window's alternate format: the low bit of blue,
    green and red in bits 14-12, the rest 4:4:4), one a line: #RRGGBB (5 bits a component, scaled to 8),
    and '+' where bit 15 is set; sixteen entries a block, a blank line between blocks."""
    lines = []
    for index, (value,) in enumerate(struct.iter_unpack("<H", data[:len(data) // 2 * 2])):
        red = ((value >> 12) & 1) | ((value << 1) & 0x1E)
        green = ((value >> 13) & 1) | ((value >> 3) & 0x1E)
        blue = ((value >> 14) & 1) | ((value >> 7) & 0x1E)
        if index and index % 16 == 0:
            lines.append("")
        lines.append(f"#{red * 255 // 31:02X}{green * 255 // 31:02X}{blue * 255 // 31:02X}" + ("+" if value & 0x8000 else ""))
    text = "\n".join(lines) + "\n"
    if len(data) % 2:
        text += f"byte {data[-1]:02X}\n"
    return text


def palette_from_text(text):
    out = bytearray()
    for line in text.split("\n"):
        line = line.strip()
        if not line:
            continue
        if line.startswith("byte "):
            out.append(int(line[5:], 16))
            continue
        red, green, blue = (round(int(line[at:at + 2], 16) * 31 / 255) for at in (1, 3, 5))
        value = (0x8000 if line.endswith("+") else 0) | (blue & 1) << 14 | (green & 1) << 13 | (red & 1) << 12 \
            | (blue & 0x1E) << 7 | (green & 0x1E) << 3 | (red & 0x1E) >> 1
        out += struct.pack("<H", value)
    return bytes(out)


FILL_RUN = 16    # runs of one byte this long or longer are written as 'fill'


def fill_to_text(data):
    """Padding: runs of one byte as 'fill XX COUNT' (COUNT in decimal), the bytes between them as
    'bytes XX XX ...' lines, sixteen a line."""
    lines = ["# 'fill XX COUNT': COUNT bytes XX (count in decimal); other bytes as 'bytes' lines"]
    loose = bytearray()

    def flush():
        lines.extend("bytes " + " ".join(f"{byte:02X}" for byte in loose[at:at + 16]) for at in range(0, len(loose), 16))
        loose.clear()
    at = 0
    while at < len(data):
        end = at
        while end < len(data) and data[end] == data[at]:
            end += 1
        if end - at >= FILL_RUN:
            flush()
            lines.append(f"fill {data[at]:02X} {end - at}")
        else:
            loose += data[at:end]
        at = end
    flush()
    return "\n".join(lines) + "\n"


def fill_from_text(text):
    out = bytearray()
    for line in text.split("\n"):
        words = line.split()
        if not words or words[0].startswith("#"):
            continue
        if words[0] == "bytes":
            out += bytes(int(word, 16) for word in words[1:])
        elif words[0] == "fill" and len(words) == 3:
            out += bytes([int(words[1], 16)]) * int(words[2])
        else:
            raise ValueError(f"not a fill line: {line.strip()}")
    return bytes(out)


FORMATS = {
    "words": (".txt", words_to_text, words_from_text),
    "bytes": (".txt", bytes_to_text, bytes_from_text),
    "fill": (".txt", fill_to_text, fill_from_text),
    "palette": (".pal.txt", palette_to_text, palette_from_text),
}


def is_binary(kind):
    """A format whose files are not text (a standard file format, such as a MIDI file): FORMATS gives it
    a fourth element, "binary"; its to_text returns bytes and its from_text takes them."""
    return FORMATS[kind][3:] == ("binary",)


def plain_format(data):
    """The format of a piece data_formats.json gives none: a fill when it is one byte over and over, else
    16-bit words (bytes when its size is odd)."""
    if len(data) >= 16 and data.count(data[0]) == len(data):
        return "fill"
    return "bytes" if len(data) % 2 else "words"


def _gather():
    """The formats of tools/formats/, one module a family of data (a module with no FORMATS holds helpers)."""
    import importlib
    import pkgutil
    import formats
    for module in pkgutil.iter_modules(formats.__path__):
        FORMATS.update(getattr(importlib.import_module(f"formats.{module.name}"), "FORMATS", {}))


_gather()


# ---- the pieces ----

def pieces(symbols_path, start, end):
    """[(address, name)] of the places symbols.txt names in start..end, the first one at 'start'."""
    found = {}
    for line in symbols_path.read_text(encoding="utf-8").splitlines():
        match = re.match(r"([0-9A-F]{6})\s+(data|table)\s+(\w+)", line)
        if match:
            address = int(match.group(1), 16)
            if start <= address < end:
                found.setdefault(address, match.group(3))
    if start not in found:
        found[start] = f"data_{start:06X}"
    return sorted(found.items())


_v60_instructions = {}


def v60_code(image, symbols_path, start, end):
    """[(start, end)] of the V60 program's code in start..end (what the static analysis reaches from
    symbols.txt, as tools/disassemble.py finds it in the program image): the C replaces it, so the asset
    does not hold it."""
    if not _v60_instructions:
        import disassemble
        analysis = disassemble.Analysis(image, disassemble.load_symbols(symbols_path))
        analysis.run()
        _v60_instructions.update({address: instruction.length for address, instruction in analysis.instructions.items()})
    spans = []
    for address in sorted(address for address in _v60_instructions if start <= address < end):
        finish = address + _v60_instructions[address]
        if spans and spans[-1][1] == address:
            spans[-1][1] = finish
        else:
            spans.append([address, finish])
    return [tuple(span) for span in spans]


SOUND_WINDOW = 0xA000    # the Z80 sees the start of the sound program's chip below here, a bank of it above


def sound_pieces(symbols_path, size):
    """[(offset in the chip, name)] of the places sound_symbols.txt names in the sound program's chip: its
    table, data and jumps lines below A000 (Z80 addresses, the chip's own offsets there) and its banked lines
    (offsets in the chip: bank * 2000h + the address in the A000-BFFF window - A000h)."""
    found = {}
    for line in symbols_path.read_text(encoding="utf-8").splitlines():
        match = re.match(r"([0-9A-F]{4,6})\s+(data|table|jumps|banked)\s+(\w+)", line)
        if not match:
            continue
        offset = int(match.group(1), 16)
        if (offset < SOUND_WINDOW) == (match.group(2) != "banked") and offset < size:
            found.setdefault(offset, match.group(3))
    found.setdefault(0, "data_000000")
    return sorted(found.items())


def z80_code(chip, symbols_path):
    """[(start, end)] of the sound program's code (what the Z80 analysis reaches from sound_symbols.txt's
    entries): the C replaces it, so the asset does not hold it. Its jump tables are data: the driver reads
    them (read_table_word) before it jumps."""
    from z80 import analysis
    program = analysis.load_program(chip, symbols_path)
    instructions = program.analysis.instructions
    lengths = {address: instruction.length for address, instruction in instructions.items()}
    spans = []
    for address in sorted(lengths):
        end = address + lengths[address]
        if spans and spans[-1][1] == address:
            spans[-1][1] = end
        else:
            spans.append([address, end])
    return [tuple(span) for span in spans]


SECTION = re.compile(r"======== (\w+) .*========$")
SECTION_NOTE = ["# A section a piece of the ROM, in the order of their addresses, each under its line",
                "#   ======== NAME (ADDRESS, format) ========",
                "# layout.json says where each piece goes back; a piece keeps its size."]


def load_formats(formats_path):
    """({piece name: format}, {piece name: file}, {file: description}) from a formats file: {"files": {file:
    {"description", "pieces": {name: format or null}}}}, the pieces grouped in files by subject (a piece no file
    lists goes with the one before it); or {"formats": {name: format}}, a file a piece."""
    if not formats_path.exists():
        return {}, {}, {}
    document = json.loads(formats_path.read_text(encoding="utf-8"))
    if "files" not in document:
        return document.get("formats", {}), {}, {}
    chosen, groups, descriptions = {}, {}, {}
    for group, listing in document["files"].items():
        descriptions[group] = listing.get("description", "")
        for name, kind in listing["pieces"].items():
            groups[name] = group
            if kind:
                chosen[name] = kind
    return chosen, groups, descriptions


def _wrap(text, prefix="# ", width=110):
    lines, line = [], prefix
    for word in text.split():
        if len(line) + len(word) + 1 > width and line != prefix:
            lines.append(line.rstrip())
            line = prefix
        line += word + " "
    return lines + [line.rstrip()]


def export(region, base, places, formats_path, folder, kept=(), movable=None, default_group=None):
    """Every piece of 'region' (which starts at address 'base') into 'folder', and layout.json, the pieces in
    order. 'places' are [(address, name)] where pieces start (pieces() for the program ROM); 'kept' are (start,
    end) ranges the asset does not hold (code): pieces of their own that build() takes from the ROM; 'movable',
    (start, end), a range whose pieces build() may lay out anew (see build()). The formats file (load_formats)
    gives each piece its format and its file: a text file by subject holds its pieces as sections; a piece
    in a binary format is a file of its own in the folder of its subject; a run of one byte (padding) is
    written in layout.json. 'default_group' (an address -> a file's name) groups the pieces no file of the
    formats file takes. Returns how many files."""
    chosen, groups, descriptions = load_formats(formats_path)
    cuts = dict(places)
    if base not in cuts:
        cuts[base] = f"data_{base:06X}"
    kept_starts = {start for start, _ in kept}
    for start, finish in kept:
        cuts[start] = f"code_{start:06X}"
        cuts.setdefault(finish, f"data_{finish:06X}")
    inside_kept = {address for address in cuts for start, finish in kept if start < address < finish}
    ordered = sorted((address, name) for address, name in cuts.items()
                     if base <= address < base + len(region) and address not in inside_kept)
    # the file of each piece: its own, else the one of the piece before it (the first ones: the one after)
    group_of = [None if address in kept_starts else groups.get(name) for address, name in ordered]
    if default_group is not None:
        group_of = [None if address in kept_starts else group or default_group(address)
                    for (address, _), group in zip(ordered, group_of)]
        groups = groups or {"": None}
    if groups:
        held = [index for index, (address, _) in enumerate(ordered) if address not in kept_starts]
        for previous, index in zip([None] + held, held):
            if group_of[index] is None and previous is not None:
                group_of[index] = group_of[previous]
        for following, index in zip(held[1:] + [None], held):
            if group_of[index] is None and following is not None:
                group_of[index] = group_of[following]
    folder.mkdir(parents=True, exist_ok=True)
    layout, sections, files = [], {}, set()
    for index, (address, name) in enumerate(ordered):
        end = ordered[index + 1][0] if index + 1 < len(ordered) else base + len(region)
        data = region[address - base:end - base]
        entry = {"address": f"{address:06X}", "name": name, "size": len(data)}
        if address in kept_starts:
            layout.append(dict(entry, format="kept"))
            continue
        kind = chosen.get(name)
        if kind not in FORMATS:
            kind = plain_format(data)
        suffix, to_text, from_text = FORMATS[kind][:3]
        text = to_text(data)
        if from_text(text) != data:    # a format that does not fit this piece: keep it as bytes
            kind = "bytes"
            suffix, to_text, _ = FORMATS[kind]
            text = to_text(data)
        group = group_of[index]
        if groups and data and data.count(data[0]) == len(data) and len(data) >= FILL_RUN:
            layout.append(dict(entry, format="fill", fill=f"{data[0]:02X} {len(data)}"))
            continue
        if group is None:
            file_name = f"{address:06X}_{name}{suffix}"
        elif isinstance(text, bytes) or any(SECTION.match(line) for line in text.split("\n")):
            file_name = f"{group}/{name}{suffix}"
        else:
            file_name = f"{group}.txt"
            sections.setdefault(file_name, []).append((name, address, kind, text))
            layout.append(dict(entry, format=kind, file=file_name, section=name,
                               **({} if text.endswith("\n") else {"newline": False})))
            continue
        target = folder / file_name
        target.parent.mkdir(parents=True, exist_ok=True)
        if isinstance(text, bytes):
            target.write_bytes(text)
        else:
            target.write_text(text, encoding="ascii")
        files.add(file_name)
        layout.append(dict(entry, format=kind, file=file_name))
    for file_name, members in sections.items():
        group = file_name[:-4]
        lines = (_wrap(f"{file_name}: {descriptions[group]}") if descriptions.get(group) else []) + SECTION_NOTE
        for name, address, kind, text in members:
            body = text.split("\n")
            if text.endswith("\n"):
                body = body[:-1]
            lines += ["", f"======== {name} ({address:06X}, {kind}) ========"] + body
        target = folder / file_name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text("\n".join(lines) + "\n", encoding="ascii")
        files.add(file_name)
    document = {"base": f"{base:06X}", "pieces": layout}
    if movable:
        document["movable"] = [f"{movable[0]:06X}", f"{movable[1]:06X}"]
    (folder / "layout.json").write_text(json.dumps(document, indent=1) + "\n", encoding="ascii")
    return len(files) + 1


def read_sections(path):
    """{piece name: its text} of a file of sections export() wrote."""
    sections, name, body = {}, None, []
    for line in path.read_text(encoding="ascii").split("\n"):
        match = SECTION.match(line)
        if match:
            if name is not None:
                sections[name] = body
            name, body = match.group(1), []
        elif name is not None:
            body.append(line)
    if name is not None:
        sections[name] = body
    # a blank line goes before every header, and the file ends with a newline: one blank line ends each body
    return {name: body[:-1] if body and body[-1] == "" else body for name, body in sections.items()}


PLACES = {}    # while build() reads the pieces: where this build lays out each one, by its name


def build_pieces(folder, size, original=None):
    """The region back from 'folder', as [(piece, its offset in the region, its bytes)]: every piece's file,
    in the order layout.json gives; the kept pieces (code) from 'original', the region as the ROM has it. A piece keeps its size (the code reaches it at a
    fixed address), except in the layout's 'movable' range: there the pieces are laid out one after another
    whatever their sizes, and the last one, a fill, takes what room is left up to the range's end; the
    formats that point at a piece by its name (PLACES) find it where it went."""
    layout = json.loads((folder / "layout.json").read_text(encoding="ascii"))
    base = int(layout["base"], 16)
    movable = [int(value, 16) for value in layout.get("movable", ())]
    pieces = layout["pieces"]
    addresses = [int(piece["address"], 16) for piece in pieces]

    sections = {}

    def text_of(piece):
        path = folder / piece["file"]
        if "section" not in piece:
            return path.read_bytes() if is_binary(piece["format"]) else path.read_text(encoding="ascii")
        if path not in sections:
            sections[path] = read_sections(path)
        if piece["section"] not in sections[path]:
            raise ValueError(f"{piece['file']}: no section {piece['section']}")
        body = sections[path][piece["section"]]
        return "\n".join(body) + ("" if piece.get("newline", True) is False else "\n")

    def read_all():
        found = []
        for piece, address in zip(pieces, addresses):
            if piece["format"] == "kept":
                found.append(original[address - base:address - base + piece["size"]])
                continue
            if "fill" in piece:
                value, count = piece["fill"].split()
                found.append(bytes([int(value, 16)]) * int(count))
                continue
            _, _, from_text = FORMATS[piece["format"]][:3]
            found.append(from_text(text_of(piece)))
        return found

    PLACES.clear()
    PLACES.update({piece["name"]: address for piece, address in zip(pieces, addresses)})
    try:
        datas = read_all()
        if movable:
            moving = [index for index, address in enumerate(addresses) if movable[0] <= address < movable[1]]
            at = movable[0]
            for index in moving[:-1]:
                addresses[index] = at
                at += len(datas[index])
            last = moving[-1]
            if pieces[last]["format"] != "fill" or at > movable[1]:
                raise ValueError(f"{folder}: the pieces from {movable[0]:06X} make {at - movable[0]} bytes, "
                                 f"more than the {movable[1] - movable[0]} up to {movable[1]:06X}")
            addresses[last] = at
            datas[last] = datas[last][:1] * (movable[1] - at)
            for index in moving:
                pieces[index]["size"] = len(datas[index])
            PLACES.update({piece["name"]: address for piece, address in zip(pieces, addresses)})
            sizes = [len(data) for data in datas]
            datas = read_all()
            datas[last] = datas[last][:1] * (movable[1] - at)
            if [len(data) for data in datas] != sizes:
                raise ValueError(f"{folder}: a piece changed its size when the places moved")
    finally:
        PLACES.clear()
    total = 0
    for piece, data in zip(pieces, datas):
        if len(data) != piece["size"]:
            # the code reaches every piece at a fixed address: one that grows or shrinks moves the rest
            raise ValueError(f"{piece.get('file', piece['name'])}: {piece['name']} is {len(data)} bytes, not {piece['size']}")
        total += len(data)
    if total != size:
        raise ValueError(f"{folder}: the pieces make {total} bytes, not {size}")
    return [(piece, address - base, data) for piece, address, data in zip(pieces, addresses, datas)]


def build(folder, size, original=None):
    """The region back from 'folder' (build_pieces' pieces, one after another)."""
    return b"".join(data for _, _, data in build_pieces(folder, size, original))
