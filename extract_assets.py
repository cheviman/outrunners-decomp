#!/usr/bin/env python3
"""
Name:        extract_assets.py
Description: extracts the game's assets from the user's own ROM set, driven by assets.json (as in
             n64decomp/sm64)
Version:     0.1.0
Created:     25-10-2026
Modified:    26-10-2026
Author:      Sebastian Toyos

Usage:
  python extract_assets.py [rom folder or .zip]   extract what is missing (default ROM location: rom/)
  python extract_assets.py --force [rom]          extract everything again
  python extract_assets.py --clean                delete every extracted asset
  python extract_assets.py --check [rom]          build back what can go into the ROM and compare the bytes

assets.json holds positions only: every byte comes from the user's own ROM set when the assets are
extracted, into assets/ (which, like the ROM, stays out of the repository). Each asset is in a format the
usual tools open (indexed PNG, WAV, text), and --check builds the board's regions back from the files and
compares them with the set, chip by chip.
"""
import json
import re
import shutil
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "core"))
import asset_formats  # noqa: E402
import data_formats  # noqa: E402
import orunners_rom  # noqa: E402

MANIFEST = ROOT / "assets.json"
ASSETS = ROOT / "assets"
STATE = ASSETS / ".extracted"     # manifest version, then the path of every asset extracted

REGION_BUILDERS = {
    "program": orunners_rom.build_program,
    "tiles": orunners_rom.build_tiles,
    "sprites": orunners_rom.build_sprites,
    "sound": orunners_rom.build_sound,
    "pcm": orunners_rom.build_pcm,
}
TILE_SLOTS = 16    # palettes a tile sheet's PNG holds: 16 of 16 colors


def load_manifest():
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    for asset in manifest["assets"]:
        asset["offset"] = int(asset["offset"], 0)
        asset["size"] = int(asset["size"], 0)
        if asset["region"] not in REGION_BUILDERS:
            raise SystemExit(f"assets.json: unknown region '{asset['region']}' for {asset['path']}")
    return manifest


def palette_colors(program, offset, count, alternate):
    """'count' palette entries at 'offset' of the program ROM as (red, green, blue): in the palette window's
    alternate format (the low bit of blue, green and red in bits 14-12), or as palette RAM holds them."""
    colors = []
    for index in range(count):
        value = int.from_bytes(program[offset + index * 2:offset + index * 2 + 2], "little")
        if alternate:
            red = ((value >> 12) & 1) | ((value << 1) & 0x1E)
            green = ((value >> 13) & 1) | ((value >> 3) & 0x1E)
            blue = ((value >> 14) & 1) | ((value >> 7) & 0x1E)
            value = blue << 10 | green << 5 | red
        colors.append(asset_formats.rgb555(value & 0x7FFF))
    return colors


def extract_tile_sheet(asset, data, program, path, columns):
    """A sheet of 16x16 tiles as an 8-bit indexed PNG in color: the 16 palettes its tiles are shown with
    the most (tile_palettes.json), 16 colors each, a tile's pixels at its palette's slot * 16 + pen (pen 0
    see-through), so that index & 15 is the pen; and NAME.json beside it, each tile's palette."""
    first = asset["offset"] // asset_formats.TILE_BYTES
    count = len(data) // asset_formats.TILE_BYTES
    known = json.loads((ROOT / asset["palettes"]).read_text(encoding="ascii"))
    formats = known.get("formats", {})
    palettes = {int(tile, 16): (int(offset, 0), source)
                for source in ("drawn", "nearby", "chosen") for tile, offset in known[source].items()}
    used = Counter(palettes[first + tile][0] for tile in range(count) if first + tile in palettes)
    slots = [offset for offset, _ in used.most_common(TILE_SLOTS)]
    colors = []
    for offset in slots:
        colors += palette_colors(program, offset, 16, formats.get(f"0x{offset:06X}", "alternate") == "alternate")
    colors += [(level * 17,) * 3 for level in range(16)] * (TILE_SLOTS - len(slots))
    rows = asset_formats.tile_sheet_pens(data, columns)
    listing = []
    for tile in range(count):
        offset, source = palettes.get(first + tile, (None, "none"))
        slot = slots.index(offset) if offset in slots else None
        top = (tile // columns) * asset_formats.TILE_SIZE
        left = (tile % columns) * asset_formats.TILE_SIZE
        if slot:    # (slot 0: the pens are the indices already)
            for y in range(top, top + asset_formats.TILE_SIZE):
                rows[y][left:left + asset_formats.TILE_SIZE] = \
                    [slot * 16 + pen for pen in rows[y][left:left + asset_formats.TILE_SIZE]]
        listing.append({"tile": f"{first + tile:04X}", "palette": f"0x{offset:06X}" if offset is not None else None,
                        "palette_from": source, "slot": slot})
    alphas = [0 if index % 16 == 0 else 255 for index in range(TILE_SLOTS * 16)]
    asset_formats.write_png_indexed(path, rows, colors, 8, alphas=alphas)
    path.with_suffix(".json").write_text(json.dumps({
        "notes": ["Each tile's palette: where its colors are in the program ROM ('palette_from': 'drawn' in the "
                  "scenarios, 'nearby': a tile no scenario shows, given its neighbour's, or 'chosen' by hand), and "
                  "the slot of the sheet's 16 palettes it is drawn with (null: not among them, drawn with the "
                  "first). Index & 15 of a pixel is the pen."],
        "tiles": listing}, indent=1) + "\n", encoding="ascii")
    return 2


SHEET_NOTES = ["Each piece goes back into the sprite ROM at 'word' (32-bit words, bank << 20 | address), cut from "
               "the sheet at x, y (width, height in pixels); 'record' is its number among the piece records at "
               "0x122B74. 'rows' are the sheet's animations, by the index of their first piece.",
               "4-bit sheets: up to 8 palettes, 32 entries each ('palettes', by slot): a pixel is slot * 32 + its pen "
               "where the board draws it, slot * 32 + 16 + its pen where it does not, shown see-through: pen 0 and, "
               "unless the piece is 'opaque' (the game draws it so: the end code is a color), the end code 15 at a "
               "word's edges and what follows a word that ends with it. 8-bit sheets: the pixel is the pen; pen 0 "
               "is see-through, and so is pen 255 where the sheet's pieces are not opaque (it is their end code, "
               "which the board does not draw).",
               "'palettes' are where the colours are in the program ROM; a piece's 'palette_from' says how its "
               "palette was found: 'drawn' in the scenarios, 'nearby' (a piece no scenario draws, given its "
               "neighbour's) or 'chosen' by hand (sprite_palettes.json)."]


def sheet_parts(sheet_path, sheet, by_number, palettes, slots_most):
    """A sheet as parts that each fit a PNG: one kind of pieces (4 or 8 bits a pen) and at most 'slots_most'
    palettes for the 4-bit ones (1 for 8-bit): [(path, rows)], the first part keeping the sheet's path."""
    parts, current, slots, bits = [], [], [], None
    for name, numbers in sheet["rows"]:
        row = []
        for number in numbers:
            frame = by_number[number]
            palette = palettes.get(number, (None, "none"))[0]
            fits = bits in (None, frame["bits"]) and (palette in slots or len(slots) < (slots_most if frame["bits"] == 4 else 1))
            if not fits:
                if row:
                    current.append([name, row])
                parts.append(current)
                current, row, slots, bits = [], [], [], None
            bits = frame["bits"]
            if palette not in slots:
                slots.append(palette)
            row.append(number)
        if row:
            current.append([name, row])
    if current:
        parts.append(current)
    return [(sheet_path if index == 0 else f"{sheet_path}_{index + 1}", rows) for index, rows in enumerate(parts)]


def extract_sprites(asset, sprites, program, path):
    """The sprite ROM's pieces as sheets (sprite_sheets.txt): a PNG per family of objects or stretch of
    animations, in folders, each with a JSON beside it that says where each piece is in it and where it goes
    back in the sprite ROM; what no piece covers as small sheets of their own."""
    known = json.loads((ROOT / asset["palettes"]).read_text(encoding="ascii"))
    eight = {int(number, 16) for number in known.get("bits8", [])}
    frames = asset_formats.sprite_records(program, int(asset["records"], 0), asset["count"],
                                          lambda number: 8 if number in eight else 4)
    by_number = {frame["number"]: frame for frame in frames}
    formats = known.get("formats", {})
    palettes = {int(number, 16): (int(offset, 0), source)
                for source in ("drawn", "nearby", "chosen") for number, offset in known.get(source, {}).items()}
    opaque = {int(number, 16) for numbers in known.get("opaque", {}).values() for number in numbers}
    sheets, where = asset_formats.load_sprite_sheets(ROOT / asset["sheets"], list(by_number))
    left_out = [number for number in by_number if number not in where]
    if left_out:
        raise SystemExit(f"{asset['sheets']}: records in no sheet: "
                         + ", ".join(f"{number:04X}" for number in left_out[:12]) + (" ..." if len(left_out) > 12 else ""))

    def colors_at(offset, count):
        if offset is None:
            return [(level * 255 // (count - 1),) * 3 for level in range(count)]
        return palette_colors(program, offset, count, formats.get(f"0x{offset:06X}", "alternate") == "alternate")

    files = 0
    covered = bytearray(len(sprites) // 4)
    for whole_path, sheet in sheets.items():
        for sheet_path, rows_of_part in sheet_parts(whole_path, sheet, by_number, palettes, asset_formats.SHEET_SLOTS):
            members = [by_number[number] for _, numbers in rows_of_part for number in numbers]
            bits = members[0]["bits"]
            slots = list(dict.fromkeys(palettes.get(frame["number"], (None, "none"))[0] for frame in members))
            pens = {frame["number"]: asset_formats.sprite_pens(sprites, frame) for frame in members}
            sizes = {number: (len(rows[0]) if rows else 0, len(rows)) for number, rows in pens.items()}
            placed, (width, height) = asset_formats.sheet_layout(rows_of_part, sizes)
            see_through = asset_formats.NOT_DRAWN if bits == 4 else 0
            canvas = [[see_through] * width for _ in range(height)]
            listing = []
            end_code_drawn = set()      # an 8-bit sheet's pen 255: whether the board draws it
            for frame in members:
                number = frame["number"]
                offset, source = palettes.get(number, (None, "none"))
                slot = slots.index(offset)
                x, y = placed[number]
                rows = pens[number]
                drawn = asset_formats.sprite_drawn(rows, bits, number in opaque)
                if bits == 8:
                    end_code_drawn |= {shown for row, flags in zip(rows, drawn)
                                       for pen, shown in zip(row, flags) if pen == 255}
                for line, row in enumerate(rows):
                    if bits == 4:
                        canvas[y + line][x:x + len(row)] = [slot * 32 + pen + (0 if shown else asset_formats.NOT_DRAWN)
                                                            for pen, shown in zip(row, drawn[line])]
                    else:
                        canvas[y + line][x:x + len(row)] = row
                listing.append({"index": len(listing), "record": f"{number:04X}", "x": x, "y": y,
                                "width": sizes[number][0], "height": sizes[number][1], "word": f"{frame['word']:06X}",
                                "slot": slot, "palette_from": source, "opaque": number in opaque})
                covered[frame["word"]:frame["word"] + frame["words"]] = b"\x01" * frame["words"]
            if bits == 4:
                colors, alphas = [], []
                for offset in slots:
                    shades = colors_at(offset, 16)
                    colors += shades + shades
                    alphas += [255] * 16 + [0] * 16
                options = {"alphas": alphas}
            else:
                colors = colors_at(slots[0], 256)
                # (a sheet with pens 255 drawn and not drawn shows them all see-through: the bytes are the same)
                hidden = (0,) if end_code_drawn == {True} else (0, 255)
                options = {"alphas": [0 if index in hidden else 255 for index in range(256)]}
            target = path / f"{sheet_path}.png"
            target.parent.mkdir(parents=True, exist_ok=True)
            asset_formats.write_png_indexed(target, canvas, colors, 8, **options)
            firsts, index = [], 0
            for name, numbers in rows_of_part:
                firsts.append({"row": name, "first": index, "pieces": len(numbers)})
                index += len(numbers)
            target.with_suffix(".json").write_text(json.dumps({
                "sheet": sheet_path, "description": sheet["description"], "notes": SHEET_NOTES, "bits": bits,
                "palettes": [f"0x{offset:06X}" if offset is not None else None for offset in slots],
                "rows": firsts, "frames": listing}, indent=1) + "\n", encoding="ascii")
            files += 2
    # what no piece covers and is not empty (0xFF): sheets of 8-pixel-wide 4-bit strips in unused/
    word = 0
    while word < len(covered):
        if covered[word] or sprites[word * 4:word * 4 + 4] == b"\xff\xff\xff\xff":
            word += 1
            continue
        start = word
        while word < len(covered) and not covered[word] and sprites[word * 4:word * 4 + 4] != b"\xff\xff\xff\xff":
            word += 1
        strip = {"number": None, "width": 8, "height": word - start, "word": start, "bits": 4}
        rows = asset_formats.sprite_pens(sprites, strip)
        name = f"unused/graphics_strip_{start:06X}"
        target = path / f"{name}.png"
        target.parent.mkdir(parents=True, exist_ok=True)
        asset_formats.write_png_indexed(target, rows, [(level * 17,) * 3 for level in range(16)], 8)
        target.with_suffix(".json").write_text(json.dumps({
            "sheet": name, "description": "graphics no piece record points at",
            "notes": ["Pens of the sprite ROM no piece covers, 8 a row: they go back at 'word' (32-bit words)."],
            "bits": 4, "palettes": [None], "rows": [{"row": "strip", "first": 0, "pieces": 1}],
            "frames": [{"index": 0, "record": None, "x": 0, "y": 0, "width": 8, "height": word - start,
                        "word": f"{start:06X}", "slot": 0, "palette_from": "none"}]}, indent=1) + "\n", encoding="ascii")
        files += 2
    return files


# The MultiPCM's sample headers: 12 bytes each at 12 x its number, as runtime/src/multipcm.c reads them
PCM_HEADER = 12
PCM_FIELDS = (("start", 0, 3), ("loop", 3, 2), ("end", 5, 2), ("vibrato", 7, 1), ("attack_decay1", 8, 1),
              ("decay_level_decay2", 9, 1), ("key_rate_release", 10, 1), ("amplitude", 11, 1))
PCM_WINDOW = 0x80000     # the size of a page of samples, and of each of the chip's two windows
PCM_FIXED = 0x100000     # the chip's addresses below this are the region's own; above, the windows'


def pcm_headers(pcm):
    """The sample headers at the start of the PCM region: [{field: value}], their number by position."""
    count = int(json.loads(MANIFEST.read_text(encoding="utf-8"))["pcm_headers"], 0)
    headers = []
    for number in range(count):
        raw = pcm[number * PCM_HEADER:(number + 1) * PCM_HEADER]
        headers.append({name: int.from_bytes(raw[at:at + size], "big") for name, at, size in PCM_FIELDS})
    return headers


def pcm_header_text(headers):
    lines = ["# The MultiPCM's sample headers, one a line by number (hex): start (its address in the chip's 2 MB,",
             "# the format in its top bits; from 100000 the screen's window of the page its sound set picks),",
             "# loop and end (the chip plays to 10000 - end, then goes back to loop), then the envelope and the",
             "# LFOs as the chip takes them (runtime/src/multipcm.c)."]
    for number, header in enumerate(headers):
        lines.append(f"{number:03X} " + " ".join(f"{name}={header[name]:0{size * 2}X}" for name, _, size in PCM_FIELDS))
    return "\n".join(lines) + "\n"


def pcm_headers_from_text(path):
    out = bytearray()
    for line in path.read_text(encoding="ascii").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        fields = dict(part.split("=") for part in line.split()[1:])
        for name, _, size in PCM_FIELDS:
            out += int(fields[name], 16).to_bytes(size, "big")
    return bytes(out)


def pcm_samples_in(headers, start, size):
    """The samples of the headers that lie in the region's bytes start..start + size as the chip reads them:
    [(offset into those bytes, header numbers, loop, length)]. The fixed part is the region's own; a page is
    what a window (A for headers below 100, B from 100) shows when a sound set puts that page there."""
    found = {}
    for number, header in enumerate(headers):
        address = header["start"] & 0x1FFFFF
        length = 0x10000 - header["end"]
        if address < PCM_FIXED:
            offset = address - start if start < PCM_FIXED else None
        else:
            offset = (address & (PCM_WINDOW - 1)) if start >= PCM_FIXED else None
        if offset is not None and 0 <= offset < size:
            found.setdefault((offset, header["loop"], length), []).append(number)
    return [(offset, numbers, loop, length) for (offset, loop, length), numbers in sorted(found.items())]


def extract_pcm_bank(asset, data, pcm, path):
    """A part of the PCM region as an 8-bit WAV (the chip's signed bytes, + 128) and NAME.json beside it:
    the samples the headers find in it, and the sound sets that put it in a window."""
    path.parent.mkdir(parents=True, exist_ok=True)
    asset_formats.write_wav(path, bytes(byte ^ 0x80 for byte in data), asset["rate"], bits=8)
    samples = pcm_samples_in(pcm_headers(pcm), asset["offset"], asset["size"])
    path.with_suffix(".json").write_text(json.dumps({
        "notes": ["One WAV sample a byte of the PCM region, from 'offset' (the chip's signed sample + 128). The "
                  "samples are where the sample headers (pcm_headers.txt) start them, by sample offset in this WAV: "
                  "their header numbers, loop and length (in samples).",
                  "The rate is nominal: the sound program sets the pitch of every note."],
        "offset": f"0x{asset['offset']:06X}",
        "sound_sets": asset.get("sound_sets", []),
        "samples": [{"at": offset, "headers": [f"{number:03X}" for number in numbers], "loop": loop, "length": length}
                    for offset, numbers, loop, length in samples]}, indent=1) + "\n", encoding="ascii")
    return 2


def rom_tables(start, end):
    """[(address, name)] of the program ROM's tables the C reads by name (src/ram.h: NAME is rom(NAME_ADDRESS))
    in start..end: places where data pieces start, besides symbols.txt's data and table lines."""
    header = (ROOT / "src" / "ram.h").read_text(encoding="ascii")
    addresses = dict(re.findall(r"^#define (\w+)_ADDRESS 0x([0-9A-F]+)u", header, re.M))
    found = []
    for name in re.findall(r"^#define (\w+) \(\(const \w+ \*\)rom\(", header, re.M):
        address = int(addresses[name], 16)
        if start <= address < end:
            found.append((address, name))
    return found


def subjects_of(places, symbols_path, sources, folder, code_size):
    """The file of each data piece by its subject, as the C has the code (symbols_path's file lines): a
    piece the C names takes the subject of the C file that names it the most; another one inside the code's
    chip (below code_size), the subject of the stretch of code it lies in (the original's linker laid the
    tables out beside the code that reads them); the rest go with the piece before them
    (data_formats.export). 'folder' is the C's folder the subjects are in (src/, src/sound/). Returns an
    address -> file name function for export's default_group."""
    stretches = sorted((int(address, 16), path) for address, path in
                       re.findall(r"^([0-9A-F]{4,6})\s+file\s+(\S+)", symbols_path.read_text(encoding="utf-8"), re.M))
    names = {name for _, name in places}
    uses = {}
    for source in sources:
        text = source.read_text(encoding="ascii", errors="replace")
        subject = source.relative_to(folder).with_suffix("").as_posix()
        for name, count in Counter(re.findall(r"\b(\w+?)(?:_ADDRESS)?\b", text)).items():
            if name in names:
                uses.setdefault(name, Counter())[subject] += count
    by_address = {address: uses[name].most_common(1)[0][0] for address, name in places if name in uses}

    def subject(address):
        if address in by_address:
            return by_address[address]
        inside = [path for start, path in stretches if start <= address]
        return inside[-1] if inside and address < code_size else None
    return subject


def extract(asset, data, region_of):
    path = ROOT / asset["path"]
    kind = asset["type"]
    if kind == "program_data":
        start, end = asset["offset"], asset["offset"] + asset["size"]
        places = data_formats.pieces(ROOT / "symbols.txt", start, end) + rom_tables(start, end)
        code = data_formats.v60_code(region_of("program"), ROOT / "symbols.txt", start, end)
        # the chips' mirrors are no data of their own: kept, as the code is
        mirrors = [tuple(int(value, 0) for value in pair) for pair in asset.get("mirrors", [])]
        game_c = sorted(source for source in (ROOT / "src").rglob("*.c") if "sound" not in source.parts)
        return data_formats.export(data, start, places, ROOT / asset["formats"], path, sorted(code + mirrors),
                                   default_group=subjects_of(places, ROOT / "symbols.txt", game_c, ROOT / "src",
                                                             0x40000))
    if kind == "sound_data":
        symbols = ROOT / asset["symbols"]
        places = data_formats.sound_pieces(symbols, asset["size"])
        sound_c = sorted((ROOT / "src" / "sound").glob("*.c"))
        return data_formats.export(data, asset["offset"], places, ROOT / asset["formats"], path,
                                   data_formats.z80_code(data, symbols),
                                   default_group=subjects_of(places, symbols, sound_c, ROOT / "src" / "sound",
                                                             0xA000))
    if kind == "sprite_records":
        return extract_sprites(asset, data, region_of("program"), path)
    if kind == "pcm_headers":
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(pcm_header_text(pcm_headers(region_of("pcm"))), encoding="ascii")
        return 1
    if kind == "pcm_bank":
        return extract_pcm_bank(asset, data, region_of("pcm"), path)
    if kind == "tiles_16x16x4":
        path.parent.mkdir(parents=True, exist_ok=True)
        columns = asset["columns"]
        if "palettes" not in asset:
            asset_formats.write_png_4bpp(path, columns * asset_formats.TILE_SIZE,
                                         asset_formats.tile_sheet_rows(data, columns))
            return 1
        return extract_tile_sheet(asset, data, region_of("program"), path, columns)
    raise SystemExit(f"assets.json: unknown type '{kind}' for {asset['path']}")


def remove(relative_path):
    path = ROOT / relative_path
    if path.is_dir():
        shutil.rmtree(path)
    elif path.exists():
        path.unlink()
    side = path.with_suffix(".json")
    if path.suffix in (".png", ".wav") and side.exists():
        side.unlink()
    # drop the folders the asset leaves empty, up to assets/
    folder = path.parent
    while folder != ASSETS and ASSETS in folder.parents and folder.is_dir() and not any(folder.iterdir()):
        folder.rmdir()
        folder = folder.parent


def read_state():
    """Manifest version and asset paths of the last extraction"""
    if not STATE.exists():
        return None, []
    lines = STATE.read_text(encoding="utf-8").splitlines()
    return (lines[0].strip(), [line for line in lines[1:] if line]) if lines else (None, [])


def write_state(version, paths):
    STATE.parent.mkdir(parents=True, exist_ok=True)
    STATE.write_text("\n".join([version] + paths) + "\n", encoding="utf-8")


# each takes the asset and the bytes the ROM has where it goes (what the asset leaves out comes from there)
REBUILDERS = {
    "program_data": lambda asset, original: data_formats.build(ROOT / asset["path"], asset["size"], original),
    "sound_data": lambda asset, original: data_formats.build(ROOT / asset["path"], asset["size"], original),
    "sprite_records": lambda asset, original: asset_formats.build_sprite_rom(ROOT / asset["path"], asset["size"]),
    "pcm_headers": lambda asset, original: pcm_headers_from_text(ROOT / asset["path"]),
    "pcm_bank": lambda asset, original: bytes(byte ^ 0x80 for byte in
                                              asset_formats.read_wav_unsigned(ROOT / asset["path"])[0]),
    "tiles_16x16x4": lambda asset, original: asset_formats.tiles_from_sheet(
        asset_formats.read_png_indexed(ROOT / asset["path"])[0], asset["columns"],
        asset["size"] // asset_formats.TILE_BYTES),
}


def assets_ready(manifest):
    """Whether assets/ holds this manifest's extraction, every asset of it."""
    version, _ = read_state()
    return version == str(manifest["manifest_version"]) and all((ROOT / asset["path"]).exists()
                                                                 for asset in manifest["assets"])


def regions_from_assets(rom, manifest):
    """The board's regions with every asset built from its files over the ROM's bytes (the ROM's stay
    where no asset goes yet, and for what an asset leaves out): {region name: bytes}."""
    regions = {name: bytearray(builder(rom)) for name, builder in REGION_BUILDERS.items()}
    for asset in manifest["assets"]:
        start, end = asset["offset"], asset["offset"] + asset["size"]
        region = regions[asset["region"]]
        region[start:end] = REBUILDERS[asset["type"]](asset, bytes(region[start:end]))
    return {name: bytes(region) for name, region in regions.items()}


def check(manifest, rom):
    """Builds every asset from its extracted files and compares the bytes with the ROM's; then the whole
    set, chip by chip, as the regions built from the assets give it back."""
    regions = {}
    failed = 0
    for asset in manifest["assets"]:
        if asset["region"] not in regions:
            regions[asset["region"]] = REGION_BUILDERS[asset["region"]](rom)
        original = regions[asset["region"]][asset["offset"]:asset["offset"] + asset["size"]]
        built = REBUILDERS[asset["type"]](asset, original)
        if built == original:
            print(f"  {asset['path']}: identical ({len(built)} bytes)")
            continue
        failed += 1
        first = next(index for index, (a, b) in enumerate(zip(built, original)) if a != b)
        print(f"  {asset['path']}: differs from byte {first:#x} of the region")
    covered = Counter()
    for asset in manifest["assets"]:
        covered[asset["region"]] += asset["size"]
    sizes = {name: len(builder(rom)) for name, builder in REGION_BUILDERS.items()}
    print("  the regions the assets cover: " + ", ".join(
        f"{name} {covered[name] * 100 // sizes[name]}%" for name in REGION_BUILDERS))
    chips = orunners_rom.chips_from_regions(regions_from_assets(rom, manifest))
    differ = sorted(name for name in rom if chips[name] != rom[name])
    print(f"  the ROM set from the assets: {len(rom) - len(differ)} of {len(rom)} chips identical"
          + (f"; differ: {', '.join(differ)}" if differ else ""))
    return 1 if failed or differ else 0


def main():
    arguments = sys.argv[1:]
    manifest = load_manifest()
    listed = [asset["path"] for asset in manifest["assets"]]
    saved_version, saved_paths = read_state()
    if "--clean" in arguments:
        for path in dict.fromkeys(listed + saved_paths):
            remove(path)
        STATE.unlink(missing_ok=True)
        print("Extracted assets removed")
        return 0

    force = "--force" in arguments
    checking = "--check" in arguments
    arguments = [argument for argument in arguments if argument not in ("--force", "--check")]
    source = Path(arguments[0]) if arguments else ROOT / "rom"
    if checking:
        return check(manifest, orunners_rom.load_set(source))
    version = str(manifest["manifest_version"])
    up_to_date = saved_version == version
    # assets an earlier manifest listed and this one no longer does
    stale = [path for path in saved_paths if path not in listed]
    for path in stale:
        remove(path)
        print(f"  removed {path}")
    todo = [asset for asset in manifest["assets"]
            if force or not up_to_date or not (ROOT / asset["path"]).exists()]
    if not todo:
        if stale:
            write_state(version, listed)
        print("Assets are up to date")
        return 0

    rom = orunners_rom.load_set(source)
    regions = {}

    def region_of(name):
        if name not in regions:
            regions[name] = REGION_BUILDERS[name](rom)
        return regions[name]
    files = 0
    for asset in todo:
        remove(asset["path"])
        data = region_of(asset["region"])[asset["offset"]:asset["offset"] + asset["size"]]
        files += extract(asset, data, region_of)
        print(f"  {asset['path']}")
    write_state(version, listed)
    print(f"{len(todo)} assets extracted ({files} files)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
