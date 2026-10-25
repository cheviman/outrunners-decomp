"""
Name:        asset_formats.py
Description: converters from System 32 data formats to standard files (indexed PNG, WAV), and back
Version:     0.1.0
Created:     11-10-2026
Modified:    19-10-2026
Author:      Sebastian Toyos

Formats follow MAME's segas32 driver (BSD-3-Clause):
  - tiles: 16x16 pixels, 4 bpp, 128 bytes per tile; gfx_layout bgcharlayout in segas32.cpp
  - PCM: RF5C68 sign-magnitude bytes (bit 7 set = positive), 0xFF ends a sample; sound/rf5c68.cpp;
    the samples are written as 8-bit WAVs one byte for one (rf5c68_to_unsigned), so they go back exactly
  - sprites: rows of 32-bit words (big endian in the ROM image), 8 pens of 4 bits or 4 of 8 bits each, top
    bits first; a frame is a record of sprite_frame_records in the program ROM (symbols.txt): width in
    pixels, height (low byte) and address bits 16-19 (high nibble), bank bits (0x0800, 0x4000), address;
    the frames are laid out in sheets (sprite_sheets.txt: an animation family or an object a sheet)
PNGs keep the original pen numbers as palette indices, so they can be converted back losslessly.
"""
import bisect
import re
import struct
import zlib
from pathlib import Path

TILE_BYTES = 128
TILE_SIZE = 16

# A tile row is 8 bytes; its 16 pixels come from the bytes in this order (two pixels per byte,
# high nibble first). It is MAME's x offsets {0,4,16,20,8,12,24,28,32,36,48,52,40,44,56,60}.
TILE_ROW_ORDER = (0, 2, 1, 3, 4, 6, 5, 7)

GRAY16 = bytes(level for index in range(16) for level in (index * 17,) * 3)


def _png_chunk(kind, data):
    chunk = kind + data
    return struct.pack(">I", len(data)) + chunk + struct.pack(">I", zlib.crc32(chunk) & 0xFFFFFFFF)


def write_png_4bpp(path, width, rows, palette=GRAY16):
    """Write an indexed 4-bit PNG. 'rows' holds packed rows: width / 2 bytes, high nibble first."""
    header = struct.pack(">IIBBBBB", width, len(rows), 4, 3, 0, 0, 0)
    raw = b"".join(b"\x00" + row for row in rows)
    with open(path, "wb") as out:
        out.write(b"\x89PNG\r\n\x1a\n")
        out.write(_png_chunk(b"IHDR", header))
        out.write(_png_chunk(b"PLTE", palette))
        out.write(_png_chunk(b"IDAT", zlib.compress(raw, 9)))
        out.write(_png_chunk(b"IEND", b""))


def tile_sheet_rows(data, columns):
    """Lay 16x16x4 tiles out in a grid 'columns' tiles wide. Returns packed 4-bit rows."""
    count = len(data) // TILE_BYTES
    rows = []
    for first in range(0, count, columns):
        tiles = [data[(first + column) * TILE_BYTES:(first + column + 1) * TILE_BYTES]
                 for column in range(min(columns, count - first))]
        for y in range(TILE_SIZE):
            row = bytearray()
            for tile in tiles:
                source = tile[y * 8:y * 8 + 8]
                row += bytes(source[index] for index in TILE_ROW_ORDER)
            row += bytes(8 * (columns - len(tiles)))
            rows.append(bytes(row))
    return rows


PCM_END = 0xFF       # the RF5C68 stops (or loops) at this byte: no sample holds it
PCM_PAGE = 0x100     # samples start at a page of the ROM window


def rf5c68_to_unsigned(data):
    """Sign-magnitude bytes (bit 7 set = positive) as 8-bit unsigned WAV samples, one for one: +m is
    128 + m and -m is 127 - m, so both zeros (80h and 00h) stay apart (128 and 127, half a step from the
    middle each) and 255 is the end byte, which no sample holds."""
    return bytes(0x80 + (value & 0x7F) if value & 0x80 else 0x7F - value for value in data)


def unsigned_to_rf5c68(data):
    """rf5c68_to_unsigned's way back (255, the end byte, is taken as 254)."""
    return bytes(0x80 | min(value - 0x80, 0x7E) if value >= 0x80 else 0x7F - value for value in data)


def pcm_sample_starts(chip, headers, count):
    """{offset in the sound region: [sample numbers]} from the 'count' pointers of pcm_sample_headers at
    'headers' in the sound program's chip: a header's byte 0 goes to port B0 (the bank's high bits), byte 1
    to port A0 (its low 6 bits), byte 2 is the page where the sample starts in the A000-BFFF window; the
    board's bank is A0 | (B0 bit 2) << 6 | (B0 bits 0-1) << 7, 2000h bytes each."""
    starts = {}
    for number in range(count):
        header = chip[headers + number * 2] | chip[headers + number * 2 + 1] << 8
        high, low, page = chip[header:header + 3]
        bank = (low & 0x3F) | (high & 0x04) << 4 | (high & 0x03) << 7
        starts.setdefault(bank * 0x2000 + (page * PCM_PAGE & 0x1FFF), []).append(number)
    return starts


def pcm_pieces(data, starts):
    """The samples and what else the PCM area holds, [(offset, bytes, sample numbers)]: each sample from
    its start up to the end byte (or the next sample's start), then every other run of bytes that is not
    the end byte (sample numbers []: data no header points at). Offsets are the area's; what the pieces
    leave is end bytes."""
    pieces = []
    taken = bytearray(len(data))
    ordered = sorted(starts)
    for index, start in enumerate(ordered):
        limit = ordered[index + 1] if index + 1 < len(ordered) else len(data)
        end = data.find(PCM_END, start, limit)
        end = limit if end < 0 else end
        pieces.append((start, data[start:end], starts[start]))
        taken[start:end] = b"\x01" * (end - start)
    at = 0
    while at < len(data):
        if taken[at] or data[at] == PCM_END:
            at += 1
            continue
        start = at
        while at < len(data) and not taken[at] and data[at] != PCM_END:
            at += 1
        pieces.append((start, data[start:at], []))
    return sorted(pieces, key=lambda piece: piece[0])


def write_wav(path, samples, rate, bits=16):
    """Write mono PCM: 16-bit signed little endian, or 8-bit unsigned."""
    size = bits // 8
    with open(path, "wb") as out:
        out.write(b"RIFF" + struct.pack("<I", 36 + len(samples)) + b"WAVE")
        out.write(b"fmt " + struct.pack("<IHHIIHH", 16, 1, 1, rate, rate * size, size, bits))
        out.write(b"data" + struct.pack("<I", len(samples)) + samples)


def read_wav_unsigned(path):
    """A mono PCM WAV (8-bit unsigned, or 16-bit signed: its high byte) as 8-bit unsigned samples, and
    its rate."""
    data = Path(path).read_bytes()
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise ValueError(f"{path}: not a WAV file")
    at, rate, bits, channels, samples = 12, None, None, None, None
    while at + 8 <= len(data):
        kind, size = data[at:at + 4], struct.unpack("<I", data[at + 4:at + 8])[0]
        body = data[at + 8:at + 8 + size]
        if kind == b"fmt ":
            form, channels, rate = struct.unpack("<HHI", body[:8])
            bits = struct.unpack("<H", body[14:16])[0]
            if form != 1:
                raise ValueError(f"{path}: not PCM")
        elif kind == b"data":
            samples = body
        at += 8 + size + (size & 1)
    if samples is None or bits not in (8, 16) or channels != 1:
        raise ValueError(f"{path}: not a mono 8 or 16-bit PCM WAV")
    if bits == 16:
        samples = bytes((value >> 8) + 0x80 for (value,) in struct.iter_unpack("<h", samples[:len(samples) // 2 * 2]))
    return samples, rate


# ---- sprites ----

SPRITE_RECORD = 8    # bytes of a frame record in the program ROM


def sprite_frames(program, offset, count):
    """The frame records at 'offset' in the program ROM image (V60 order: little endian), in record order:
    [{"number": the frame's sprite_frame (record offset), "width", "height", "word": where its graphics start
    in the sprite ROM (32-bit words, bank << 20 | address), "bits": 4 or 8 a pen, "words": its length}].
    The bits per pen are not in the record (the object drawing it says them): they come from the layout,
    each frame's graphics running up to the next one's."""
    frames = []
    for index in range(count):
        width, height, bank_bits, address = struct.unpack_from("<HHHH", program, offset + index * SPRITE_RECORD)
        bank = (bank_bits & 0x0800) >> 11 | (bank_bits & 0x4000) >> 13
        frames.append({"number": index * SPRITE_RECORD, "width": width, "height": height & 0xFF,
                       "word": bank << 20 | (height & 0xF000) << 4 | address})
    ordered = sorted(frames, key=lambda frame: frame["word"])
    for position, frame in enumerate(ordered):
        following = ordered[position + 1]["word"] if position + 1 < len(ordered) else None
        words4 = (frame["width"] >> 3) * frame["height"]
        words8 = (frame["width"] >> 2) * frame["height"]
        frame["bits"] = 8 if following is not None and following - frame["word"] == words8 else 4
        frame["words"] = words8 if frame["bits"] == 8 else words4
    return frames


PIECE_RECORD = 12   # bytes of an OutRunners sprite piece record (0x122B74)


def sprite_records(program, offset, count, bits_of):
    """OutRunners' piece records at 'offset' of the program ROM image (little endian), in record order, as
    frames: [{"number": the record's number, "width" (pixels), "height", "word": where its graphics start in
    the sprite ROM (32-bit words, bank << 20 | address), "bits", "words": its length}]. A record holds the
    sprite words a piece puts in the list: the size (rows in the high byte, 32-bit words a row in the low),
    the address's low 16 bits, then the height and width words (scale and the address's bits 16-19, the
    bank in bits 13 and 15). 'bits_of' gives the bits a pen of each record (the pieces that draw it say)."""
    frames = []
    for index in range(count):
        size, address, _, height_word, _, width_word = struct.unpack_from("<6H", program, offset + index * PIECE_RECORD)
        bank = (width_word & 0x2000) >> 13 | (width_word & 0x8000) >> 14
        bits = bits_of(index)
        words = size & 0xFF
        frames.append({"number": index, "width": words * (32 // bits), "height": size >> 8,
                       "word": bank << 20 | (height_word & 0xF000) << 4 | address, "bits": bits,
                       "words": words * (size >> 8)})
    return frames


def sprite_pens(sprites, frame):
    """The frame's pens: one list a row, (width >> 3) words of 8 pens (4 bits) or (width >> 2) of 4 (8 bits)."""
    bits = frame["bits"]
    per_word = 32 // bits
    words = frame["width"] >> (3 if bits == 4 else 2)
    rows = []
    for row in range(frame["height"]):
        pens = []
        for index in range(words):
            at = (frame["word"] + row * words + index) * 4
            value = struct.unpack_from(">I", sprites, at)[0]
            pens += [(value >> (32 - bits * (pen + 1))) & ((1 << bits) - 1) for pen in range(per_word)]
        rows.append(pens)
    return rows


def pens_to_words(rows, bits):
    """Rows of pens back to the sprite ROM's bytes (big-endian 32-bit words)."""
    per_word = 32 // bits
    out = bytearray()
    for pens in rows:
        for first in range(0, len(pens), per_word):
            value = 0
            for pen in pens[first:first + per_word]:
                value = value << bits | pen
            out += struct.pack(">I", value)
    return bytes(out)


def rgb555(color):
    """A System 32 palette entry (red in bits 0-4, green 5-9, blue 10-14) as 8-bit red, green, blue."""
    return tuple(((color >> shift) & 0x1F) * 255 // 31 for shift in (0, 5, 10))


def sprite_drawn(rows, bits, opaque=False):
    """Which pens of a frame the board draws (sprite list, palette not indirect): never pen 0; and unless
    the frame is drawn opaque (sprite entry bit 8), not the first or last pen of a word when it is all ones
    (the end code), and nothing after a word that ends with the end code (runtime/src/video.c,
    draw_sprite_row and put_sprite_pixel). Rows of booleans."""
    if opaque:
        return [[pen != 0 for pen in row] for row in rows]
    per_word = 32 // bits
    end = (1 << bits) - 1
    drawn = []
    for row in rows:
        flags = []
        ended = False
        for first in range(0, len(row), per_word):
            for index, pen in enumerate(row[first:first + per_word]):
                edge = index == 0 or index == per_word - 1
                flags.append(not ended and pen != 0 and not (edge and pen == end))
            if row[first + per_word - 1] == end:
                ended = True
        drawn.append(flags)
    return drawn


def write_png_indexed(path, rows, palette, bits, transparent=None, alphas=None):
    """An indexed PNG of 'bits' (4 or 8) a pixel: 'rows' of palette indices, 'palette' of (r, g, b) entries;
    'transparent' is an index shown as see-through, or 'alphas' gives each entry's opacity."""
    width = len(rows[0]) if rows else 0
    header = struct.pack(">IIBBBBB", width, len(rows), bits, 3, 0, 0, 0)
    raw = bytearray()
    for row in rows:
        raw.append(0)
        if bits == 8:
            raw += bytes(row)
        else:
            raw += bytes(row[index] << 4 | (row[index + 1] if index + 1 < len(row) else 0)
                         for index in range(0, len(row), 2))
    with open(path, "wb") as out:
        out.write(b"\x89PNG\r\n\x1a\n")
        out.write(_png_chunk(b"IHDR", header))
        out.write(_png_chunk(b"PLTE", b"".join(bytes(entry) for entry in palette)))
        if alphas is not None:
            out.write(_png_chunk(b"tRNS", bytes(alphas)))
        elif transparent is not None:
            out.write(_png_chunk(b"tRNS", bytes(255 if index != transparent else 0 for index in range(len(palette)))))
        out.write(_png_chunk(b"IDAT", zlib.compress(bytes(raw), 9)))
        out.write(_png_chunk(b"IEND", b""))


def _paeth(left, up, up_left):
    estimate = left + up - up_left
    near_left, near_up, near_up_left = abs(estimate - left), abs(estimate - up), abs(estimate - up_left)
    if near_left <= near_up and near_left <= near_up_left:
        return left
    return up if near_up <= near_up_left else up_left


def read_png_indexed(path):
    """The palette indices of an indexed PNG (1, 2, 4 or 8 bits a pixel, any filter, not interlaced), one
    list a row, and its palette [(r, g, b)]. An image edited elsewhere comes back as indices, as long as it
    stays indexed."""
    data = open(path, "rb").read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: not a PNG")
    at = 8
    idat = bytearray()
    palette = []
    header = None
    while at < len(data):
        length, kind = struct.unpack_from(">I4s", data, at)
        body = data[at + 8:at + 8 + length]
        if kind == b"IHDR":
            header = struct.unpack(">IIBBBBB", body)
        elif kind == b"PLTE":
            palette = [tuple(body[index:index + 3]) for index in range(0, len(body), 3)]
        elif kind == b"IDAT":
            idat += body
        at += 12 + length
    width, height, bits, color_type, _, _, interlace = header
    if color_type != 3 or interlace:
        raise ValueError(f"{path}: not an indexed, non-interlaced PNG")
    raw = zlib.decompress(bytes(idat))
    stride = (width * bits + 7) // 8
    rows = []
    previous = bytearray(stride)
    at = 0
    for _ in range(height):
        kind = raw[at]
        line = bytearray(raw[at + 1:at + 1 + stride])
        at += 1 + stride
        for index in range(stride):
            left = line[index - 1] if index else 0
            up = previous[index]
            up_left = previous[index - 1] if index else 0
            if kind == 1:
                line[index] = (line[index] + left) & 0xFF
            elif kind == 2:
                line[index] = (line[index] + up) & 0xFF
            elif kind == 3:
                line[index] = (line[index] + (left + up) // 2) & 0xFF
            elif kind == 4:
                line[index] = (line[index] + _paeth(left, up, up_left)) & 0xFF
        previous = line
        per_byte = 8 // bits
        indices = []
        for byte in line:
            for pixel in range(per_byte):
                indices.append(byte >> (8 - bits * (pixel + 1)) & ((1 << bits) - 1))
        rows.append(indices[:width])
    return rows, palette


# ---- sprite sheets ----

SHEET_LINE = re.compile(r"([0-9A-F]{4})\s+(sheet|row)\s+([A-Za-z0-9_/]+)\s*(?:#\s*(.*\S))?\s*$")
SHEET_WIDTH = 1024   # pixels a row of a sheet runs to before it wraps
SHEET_GAP = 2        # see-through pixels between frames
SHEET_SLOTS = 8      # palettes a 4-bit sheet holds, 32 entries each: pens 0-15 drawn, then not drawn
NOT_DRAWN = 16       # a 4-bit frame's pen p where the board does not draw it is entry slot * 32 + 16 + p


def load_sprite_sheets(path, numbers):
    """The sheets the map at 'path' (sprite_sheets.txt) makes of the frames 'numbers':
    ({sheet path: {"description", "frames": [numbers in order], "rows": [[name, [numbers]]]}},
     {frame number: (sheet path, its index in the sheet)}). A 'FRAME sheet PATH' line gives the frames from
    FRAME up to the next sheet line to PATH (a sheet may take several stretches); a 'FRAME row NAME' line
    starts a row there, as does every stretch; the comment of a sheet's first line is its description.
    Frames the map leaves out are in neither."""
    sheets_marks, rows_marks = [], {}
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        match = SHEET_LINE.match(line)
        if not match:
            continue
        frame = int(match.group(1), 16)
        if match.group(2) == "sheet":
            sheets_marks.append((frame, match.group(3), match.group(4) or ""))
        else:
            rows_marks[frame] = match.group(3)
    sheets_marks.sort()
    starts = [frame for frame, _, _ in sheets_marks]
    sheets, where = {}, {}
    for number in sorted(numbers):
        at = bisect.bisect_right(starts, number) - 1
        if at < 0:
            continue
        _, sheet_path, description = sheets_marks[at]
        sheet = sheets.setdefault(sheet_path, {"description": description, "frames": [], "rows": []})
        if number in rows_marks or number == starts[at] or not sheet["rows"]:
            name = rows_marks.get(number) or (sheet["rows"][-1][0] if sheet["rows"] else sheet_path.rsplit("/", 1)[-1])
            sheet["rows"].append([name, []])
        sheet["rows"][-1][1].append(number)
        where[number] = (sheet_path, len(sheet["frames"]))
        sheet["frames"].append(number)
    return sheets, where


def sheet_layout(rows, sizes):
    """Where each frame goes in its sheet, {number: (x, y)}, and the sheet's (width, height): a row's frames
    left to right SHEET_GAP apart, wrapping at SHEET_WIDTH, the rows top to bottom."""
    placed, width, y = {}, 1, 0
    for _, numbers in rows:
        x = line_height = 0
        for number in numbers:
            frame_width, frame_height = sizes[number]
            if x and x + frame_width > SHEET_WIDTH:
                y, x, line_height = y + line_height + SHEET_GAP, 0, 0
            placed[number] = (x, y)
            x += frame_width + SHEET_GAP
            line_height = max(line_height, frame_height)
            width = max(width, x - SHEET_GAP)
        y += line_height + SHEET_GAP
    return placed, (width, max(y - SHEET_GAP, 1))


def build_sprite_rom(folder, size):
    """The sprite ROM image (32-bit big-endian words, 'size' bytes) from the sheets extract_assets.py wrote
    to 'folder' (each NAME.png with NAME.json beside it): each frame cut from its sheet and its pens put
    where its graphics go, 0xFF everywhere else, as the chips leave it."""
    import json
    image = bytearray(b"\xff" * size)
    for described in sorted(folder.rglob("*.json")):
        sheet = json.loads(described.read_text(encoding="ascii"))
        rows, _ = read_png_indexed(described.with_suffix(".png"))
        # a 4-bit frame's pen is index & 15 (index // 32 its palette slot, 16-31 of a slot not drawn)
        mask = (1 << sheet["bits"]) - 1
        for frame in sheet["frames"]:
            x, y = frame["x"], frame["y"]
            pens = [[pen & mask for pen in rows[y + line][x:x + frame["width"]]] for line in range(frame["height"])]
            data = pens_to_words(pens, sheet["bits"])
            at = int(frame["word"], 16) * 4
            image[at:at + len(data)] = data
    return bytes(image)


# ---- tiles back ----

def tile_sheet_pens(data, columns):
    """The tiles of a sheet (tile_sheet_rows' layout) as rows of pens, one int a pixel."""
    rows = []
    for packed in tile_sheet_rows(data, columns):
        rows.append([pen for byte in packed for pen in (byte >> 4, byte & 0x0F)])
    return rows


def tiles_from_sheet(rows, columns, count):
    """The tile ROM bytes of 'count' tiles from a sheet's rows of pens (only the low 4 bits of each index are
    the pen): the reverse of tile_sheet_rows."""
    out = bytearray(count * TILE_BYTES)
    for tile in range(count):
        top = (tile // columns) * TILE_SIZE
        left = (tile % columns) * TILE_SIZE
        for y in range(TILE_SIZE):
            pens = rows[top + y][left:left + TILE_SIZE]
            packed = bytes((pens[index] & 0x0F) << 4 | (pens[index + 1] & 0x0F) for index in range(0, TILE_SIZE, 2))
            for position, source in enumerate(TILE_ROW_ORDER):
                out[tile * TILE_BYTES + y * 8 + source] = packed[position]
    return bytes(out)
