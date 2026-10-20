"""
Name:        render_state.py
Description: renders the text layer and the tilemap pages of a System 32 state dump (s32ref --dump-dir) to PNG
Version:     0.1.0
Created:     11-10-2026
Modified:    11-10-2026
Author:      Sebastian Toyos

Usage: python tools/render_state.py <dump dir> [ROM folder or .zip]   (default: rom/)

A development view, not the game's renderer: no scrolling, zoom, priorities or blending.
Formats follow MAME's segas32_v.cpp (BSD-3-Clause):
  - text layer: 64x32 cells of 8x8 pixels; cell = color (bits 9-15) and char (bits 0-8);
    chars are 4-bit packed, high nibble first, read from video RAM itself (32 bytes each)
  - tilemap pages: 32x16 cells of 16x16 tiles; cell = flips (14-15), color (4-12), tile (0-12)
  - palette entries are xBBBBBGGGGGRRRRR; a layer adds its palette base from the mixer registers
"""
import struct
import sys
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import orunners_rom  # noqa: E402

TILE_ROW_ORDER = (0, 2, 1, 3, 4, 6, 5, 7)


def write_png_rgb(path, width, height, pixels):
    """pixels: bytes of width * height * 3."""
    def chunk(kind, data):
        body = kind + data
        return struct.pack(">I", len(data)) + body + struct.pack(">I", zlib.crc32(body) & 0xFFFFFFFF)
    stride = width * 3
    raw = b"".join(b"\x00" + pixels[y * stride:(y + 1) * stride] for y in range(height))
    with open(path, "wb") as out:
        out.write(b"\x89PNG\r\n\x1a\n")
        out.write(chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)))
        out.write(chunk(b"IDAT", zlib.compress(raw, 6)))
        out.write(chunk(b"IEND", b""))


def rgb(entry):
    return bytes((((entry >> 0) & 0x1F) << 3, ((entry >> 5) & 0x1F) << 3, ((entry >> 10) & 0x1F) << 3))


def render_text_layer(video, palette, mixer):
    control = struct.unpack_from("<H", video, 0x1FF5C)[0]
    page = ((control >> 4) & 0x1F) << 11          # word offset of the 64x32 cell map
    bank = (control & 7) << 9
    palette_base = (mixer[0x20 // 2] & 0x00F0) << 6
    width, height = 512, 256
    pixels = bytearray(width * height * 3)
    for row in range(32):
        for column in range(64):
            cell = struct.unpack_from("<H", video, (page + row * 64 + column) * 2)[0]
            char = (bank | (cell & 0x1FF)) & 0xFFF
            color = (cell >> 9) & 0x7F
            source = char * 32
            for y in range(8):
                line = video[source + y * 4:source + y * 4 + 4]
                for x in range(8):
                    value = line[x // 2]
                    pen = (value >> 4) if x % 2 == 0 else (value & 0x0F)
                    if pen == 0:
                        continue
                    entry = palette[(palette_base + color * 16 + pen) & 0x3FFF]
                    offset = ((row * 8 + y) * width + column * 8 + x) * 3
                    pixels[offset:offset + 3] = rgb(entry)
    return width, height, bytes(pixels)


def render_tile_pages(video, palette, tiles, pages):
    """Each page is 32x16 tiles of 16x16; pages are laid out 4 per row. Color 0 drawn as black."""
    per_row = 4
    width = per_row * 512
    height = ((len(pages) + per_row - 1) // per_row) * 256
    pixels = bytearray(width * height * 3)
    for slot, page in enumerate(pages):
        page_x = (slot % per_row) * 512
        page_y = (slot // per_row) * 256
        for row in range(16):
            for column in range(32):
                cell = struct.unpack_from("<H", video, (page * 512 + row * 32 + column) * 2)[0]
                tile = cell & 0x1FFF
                color = (cell >> 4) & 0x1FF
                flip_x = cell & 0x4000
                flip_y = cell & 0x8000
                source = tile * 128
                for y in range(16):
                    line = tiles[source + y * 8:source + y * 8 + 8]
                    ordered = bytes(line[i] for i in TILE_ROW_ORDER)
                    target_y = page_y + row * 16 + (15 - y if flip_y else y)
                    for x in range(16):
                        value = ordered[x // 2]
                        pen = (value >> 4) if x % 2 == 0 else (value & 0x0F)
                        if pen == 0:
                            continue
                        entry = palette[(color * 16 + pen) & 0x3FFF]
                        target_x = page_x + column * 16 + (15 - x if flip_x else x)
                        offset = (target_y * width + target_x) * 3
                        pixels[offset:offset + 3] = rgb(entry)
    return width, height, bytes(pixels)


def main():
    if len(sys.argv) < 2:
        raise SystemExit("Usage: python tools/render_state.py <dump dir> [ROM folder or .zip]")
    dump = Path(sys.argv[1])
    rom_set = Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "rom"
    video = (dump / "video_ram.bin").read_bytes()
    palette = struct.unpack("<16384H", (dump / "palette.bin").read_bytes())
    mixer = struct.unpack("<64H", (dump / "mixer.bin").read_bytes())

    width, height, pixels = render_text_layer(video, palette, mixer)
    write_png_rgb(dump / "text_layer.png", width, height, pixels)
    print(f"  {dump / 'text_layer.png'}")

    # pages referenced by the NBG0-NBG3 page select registers (0x31FF40-0x31FF4E)
    selects = struct.unpack_from("<8H", video, 0x1FF40)
    pages = sorted({(value >> shift) & 0x7F for value in selects for shift in (0, 8)})
    if rom_set.exists():
        tiles = orunners_rom.build_tiles(orunners_rom.load_set(rom_set))
        width, height, pixels = render_tile_pages(video, palette, tiles, pages)
        write_png_rgb(dump / "tile_pages.png", width, height, pixels)
        print(f"  {dump / 'tile_pages.png'}  (pages {', '.join(str(page) for page in pages)})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
