"""
Name:        program_fonts.py
Description: the program's fonts as indexed PNG images: the text layer's characters (text_font, 4 bits a
             pixel) and the unused one-bit font (one_bit_font)
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Binary formats (a fourth element "binary" in FORMATS): to_text returns the PNG's bytes, from_text takes them.

The image: the characters in a grid of 16 a row, in order (character N at column N % 16, row N // 16 of the
grid), each 8x8 pixels, so the image is 128 pixels wide and 8 * (characters / 16) high; a pixel's PNG
palette index is its pen. Any indexed PNG of 1, 2, 4 or 8 bits a pixel reads back (an image edited
elsewhere stays indexed), as long as it keeps the size and no pen goes past the format's (15, 1).

- program_text_font (text_font, 384 characters, 12288 bytes): load_text_font copies it into video RAM at
  318000, the text layer's characters. A character is 32 bytes, 4 a row (top row first), each byte two
  pixels, the left one in the high nibble: pixel (x, y) of character N is the nibble of byte
  N * 32 + y * 4 + x / 2, the high one for an even x. Written as a 4-bit PNG, 16 greys (pen 0 black).
- program_one_bit_font (one_bit_font, 128 characters, 1024 bytes): the unused load_shadowed_text_font's
  font. A character is 8 bytes, a row each (top row first), a bit a pixel, the left one in bit 7: pixel
  (x, y) of character N is bit 7 - x of byte N * 8 + y. Written as a 1-bit PNG, pen 0 black, pen 1 white.
"""
import os
import struct
import tempfile
import zlib

import asset_formats

PER_ROW = 16            # characters a row of the grid
SIZE = 8                # a character's width and height in pixels


def _chunk(kind, body):
    return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body) & 0xFFFFFFFF)


def _png(rows, bits, palette):
    """An indexed PNG of 'bits' (1 or 4) a pixel."""
    raw = bytearray()
    per_byte = 8 // bits
    for row in rows:
        raw.append(0)
        for at in range(0, len(row), per_byte):
            byte = 0
            for index in range(per_byte):
                pen = row[at + index] if at + index < len(row) else 0
                byte |= pen << (8 - bits * (index + 1))
            raw.append(byte)
    header = struct.pack(">IIBBBBB", len(rows[0]), len(rows), bits, 3, 0, 0, 0)
    return (b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", header)
            + _chunk(b"PLTE", b"".join(bytes(entry) for entry in palette))
            + _chunk(b"IDAT", zlib.compress(bytes(raw), 9)) + _chunk(b"IEND", b""))


def _pens(png):
    """The palette indices of a PNG's pixels, a list a row (asset_formats.read_png_indexed reads a file)."""
    handle, path = tempfile.mkstemp(suffix=".png")
    try:
        with os.fdopen(handle, "wb") as out:
            out.write(png)
        rows, _ = asset_formats.read_png_indexed(path)
    finally:
        os.remove(path)
    return rows


def _grid(count, pixel):
    """The rows of the image of 'count' characters, pixel(character, x, y) each."""
    height = (count + PER_ROW - 1) // PER_ROW * SIZE
    rows = [[0] * (PER_ROW * SIZE) for _ in range(height)]
    for character in range(count):
        left, top = character % PER_ROW * SIZE, character // PER_ROW * SIZE
        for y in range(SIZE):
            for x in range(SIZE):
                rows[top + y][left + x] = pixel(character, x, y)
    return rows


def _characters(rows, highest):
    """[(character, its rows)] of an image of the grid, checking its size and pens."""
    if not rows or len(rows[0]) != PER_ROW * SIZE or len(rows) % SIZE:
        raise ValueError(f"the font image is {len(rows[0]) if rows else 0}x{len(rows)}, not {PER_ROW * SIZE} wide "
                         f"and a multiple of {SIZE} high")
    if any(pen > highest for row in rows for pen in row):
        raise ValueError(f"a pixel of the font image has a pen above {highest}")
    count = len(rows) // SIZE * PER_ROW
    return [[rows[character // PER_ROW * SIZE + y][character % PER_ROW * SIZE:character % PER_ROW * SIZE + SIZE]
             for y in range(SIZE)] for character in range(count)]


# ---- text_font ----

TEXT_CHARACTER = 32     # bytes a character of the text layer


def _whole_rows(size, character):
    """The characters of the grid's whole rows that hold 'size' bytes (more than them when they make no whole
    rows: such an image does not read back to the piece, which then stays as bytes)."""
    count = -(-size // character)
    return max(PER_ROW, -(-count // PER_ROW) * PER_ROW)


def _byte(data, at):
    return data[at] if at < len(data) else 0


def text_font_to_png(data):
    def pixel(character, x, y):
        value = _byte(data, character * TEXT_CHARACTER + y * 4 + x // 2)
        return value & 0x0F if x & 1 else value >> 4
    return _png(_grid(_whole_rows(len(data), TEXT_CHARACTER), pixel), 4, [(level * 17,) * 3 for level in range(16)])


def text_font_from_png(png):
    out = bytearray()
    for character in _characters(_pens(png), 15):
        for row in character:
            out += bytes(row[x] << 4 | row[x + 1] for x in range(0, SIZE, 2))
    return bytes(out)


# ---- one_bit_font ----

ONE_BIT_CHARACTER = 8   # bytes a character of the one-bit font


def one_bit_font_to_png(data):
    def pixel(character, x, y):
        return _byte(data, character * ONE_BIT_CHARACTER + y) >> (7 - x) & 1
    return _png(_grid(_whole_rows(len(data), ONE_BIT_CHARACTER), pixel), 1, [(0, 0, 0), (255, 255, 255)])


def one_bit_font_from_png(png):
    out = bytearray()
    for character in _characters(_pens(png), 1):
        for row in character:
            out.append(sum(pen << (7 - x) for x, pen in enumerate(row)))
    return bytes(out)


FORMATS = {
    "program_text_font": (".png", text_font_to_png, text_font_from_png, "binary"),
    "program_one_bit_font": (".png", one_bit_font_to_png, one_bit_font_from_png, "binary"),
}
