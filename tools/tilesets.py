"""
Name:        tilesets.py
Description: the tile ROM's 16x16 tiles in tilesets by subject: writes tilesets.txt from a census of the
             tilemaps the program's data holds, draws the tilesets for extract_assets.py and builds the tile
             ROM back from them
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Usage: py tools/tilesets.py [CENSUS_DIR]   (reads rom/; writes tilesets.txt. CENSUS_DIR: the tile censuses
                                            of 'tools/palette_census.py run', to compare what the scenarios
                                            show with what the data names)

Only the tilemaps (the four NBG layers) show the tile ROM's 32768 tiles: a page of video RAM is 32 x 16
cells, a cell the tile's number (13 bits) and its flips, and the I/O chip gives each layer a bank (2 bits,
tile_banks), the number's top two bits. The game unpacks its pages into video RAM from the program ROM
(unpack's format, a page 1 KB) by lists, each entry a page number and the address of its packed data
(queue_tile_pages, unpack_tile_pages). The census reads those lists where the program keeps them, unpacks
every page and takes its tiles with the bank the code gives the layer that shows it:
  - the race's backgrounds (race_background_update): each stage's picture (stage_backgrounds, the last four
    pages of its list in background_tile_page_lists, stage_tile_banks; no_background_tile_pages for stage 0);
    the nine pages of each change of background at a fork (background b, from stage b / 2 to
    fork_next_stages[b]: pages 0-3 the stage it leaves, 4-8 the one it reaches, background_tile_banks) and of
    the changes within a stage (background 2A + the stage); the animations of stages 6, 8 and 28
    (background_lower_animation_tile_pages, background_upper_animation_tile_pages);
  - the ranking's pictures of the goals (course_backgrounds, ranking_background_tile_banks);
  - the scenes' set-ups (set_up_background's background_setup records): the title, the car presentation,
    the car select, the entry countdown, the name entry and the route map, each page with the bank of the
    tilemap whose page word holds it (the first tilemap's for the pages the route map turns to later);
  - the road (boot_tile_pages_29_to_4e, bank 0: road_surface_start) and the two pages unpacked at boot that
    no tilemap of this set shows (boot_tile_pages_5d and 5e: the FBI and EPA notices of the US sets, whose
    art is in bank 2).
The tile ROM lays its pictures out in rows of 32 tiles (a row is one palette: a cell's colour, bits 4-12, is
also its number's), so whole rows go to tilesets: a row to the subject whose tiles are in it (a course's art
rather than a scene that shows it too), a run of rows nothing uses to the subject on both sides of it, or
else to a tileset of its own under unused/, named after the subject before it.

tilesets.txt holds tile numbers and names only: 'NAME RANGE ...  # description', a tileset a line, its ranges
(FIRST-LAST, hex) in the order its PNG lays them out, 32 tiles a row. It can be edited by hand, as long as
every tile is in one tileset.
"""
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "core"))
import asset_formats  # noqa: E402
import orunners_rom  # noqa: E402

TILE_COUNT = 0x8000            # 16x16 tiles in the tile ROM
BANK_TILES = 0x2000            # tiles a bank holds: a cell's 13 bits
ROW = 32                       # tiles in a row of the tile ROM, a row of a tileset's PNG
PAGE_CELLS = 32 * 16
SLOTS = 16                     # palettes a tileset's PNG holds: 16 of 16 colours
INDEX = ROOT / "tilesets.txt"

# the program's tables (symbols.txt)
STAGE_BACKGROUNDS = 0x017B60           # by first stage: its background (FF: none)
BACKGROUND_TILE_BANKS = 0x017B7F       # by background, at a change
STAGE_TILE_BANKS = 0x017BA9            # by first stage, at the start
NO_BACKGROUND_TILE_PAGES = 0x109684    # stage 0's four pages
BACKGROUND_TILE_PAGE_LISTS = 0x109694  # by background: the address of its nine pages (0: none)
BACKGROUNDS = 0x44                     # 2A by the forks, then 2A + a stage for the changes within one
FORK_NEXT_STAGES = 0x00E5D3            # by stage * 2 + branch
LOWER_ANIMATION_PAGES = 0x017904       # by screen: page lists (background 5, stage 6)
UPPER_ANIMATION_PAGES = 0x017988       # by animation * 2 + screen (backgrounds 7 and 8, 26, 25)
UPPER_ANIMATION_BACKGROUNDS = (0x07, 0x26, 0x25)
COURSE_BACKGROUNDS = 0x002D20          # by course (the ten goals): its background, for the ranking
RANKING_TILE_BANKS = 0x002C18          # by background
STAGES = 31
BACKGROUND_SETUPS = [                  # set_up_background's records (20 bytes): the scene, where, how many
    ("title", 0x01AD0C, 2),
    ("car_presentation", 0x001AA0, 2),
    ("entry_countdown", 0x006988, 2),
    ("name_entry", 0x002E4C, 2),
    ("route_map", 0x004868, 4),
]
CAR_SELECT_TILEMAP_SETUPS = 0x006920   # 32 bytes each, the background_setup's address at +18
ROAD_PAGES = 0x109578                  # boot_tile_pages_29_to_4e
NOTICE_PAGES = (0x109420, 0x109428)    # boot_tile_pages_5d, boot_tile_pages_5e
NOTICE_BANK = 2

# The art of each stage's pictures, by stage, as the race shows them (the goals, 21-30, by the test mode's
# ten names, AUSTRALIA to KENYA); a stage that shows another's art takes its name (six goals show copies of
# the art of stages 7-12, in the banks of the stages before them). SECOND_PICTURES: the change of background
# within a stage (Tiananmen and Guilin, Sydney and Ayers Rock).
COURSES = {
    0: "start", 1: "grand_canyon", 2: "san_francisco", 3: "jungle", 4: "niagara_falls", 5: "hawaii",
    6: "easter_island", 7: "spain", 8: "atlantic_ocean", 9: "switzerland", 10: "china", 11: "japan",
    12: "hong_kong", 13: "egypt", 14: "greece", 15: "paris", 16: "germany", 17: "germany", 18: "paris",
    19: "greece", 20: "egypt", 21: "australia", 22: "hong_kong", 23: "japan", 24: "china", 25: "russia",
    26: "northern_europe", 27: "switzerland", 28: "atlantic_ocean", 29: "spain", 30: "kenya",
}
SECOND_PICTURES = {10: "china", 21: "australia", 24: "china"}
# what each scene's own pages show (the others it shows are a course's: the title's sky and sea and the car
# select's background are courses/start's)
SCENE_NOTES = {
    "title": "the ground under the title",
    "car_presentation": "the checked backdrop of the car presentation",
    "car_select": "the two players' frames",
    "entry_countdown": ("the WELCOME OUTRUNNERS! letter and the road picture behind it, which the name entry "
                        "shows too"),
    "name_entry": "the CONGRATULATIONS! certificate",
    "route_map": "the world map, the routes and the close-up maps",
}


# ---- the census ----

def unpack(program, source):
    """The bytes unpack (src/video/unpack.c) makes of the packed data at 'source'."""
    out = bytearray()
    flags, left = program[source], 9
    source += 1

    def flag():
        nonlocal flags, left, source
        left -= 1
        if left == 0:
            flags, left = program[source], 8
            source += 1
        bit = flags >> 7
        flags = (flags << 1) & 0xFF
        return bit
    while True:
        if flag():
            out.append(program[source])
            source += 1
            continue
        if flag():
            word = program[source] | program[source + 1] << 8
            source += 2
            back = (word | ~0xFFFF) >> 3        # (back 1 to 8192)
            count = word & 7
            if count:
                count += 2
            else:
                count = program[source]
                source += 1
                if count == 0:
                    return bytes(out)
                count += 1
        else:
            count = flag() * 2
            count += flag() + 2
            back = program[source] - 256
            source += 1
        start = len(out) + back
        for index in range(count):
            out.append(out[start + index])


def u32(program, address):
    return struct.unpack_from("<I", program, address)[0]


def page_list(program, address):
    """[(page number, packed data's address)] of a list for queue_tile_pages, up to its negative entry."""
    entries = []
    while not u32(program, address) & 0x80000000:
        entry = u32(program, address)
        entries.append((entry >> 24, entry & 0xFFFFFF))
        address += 4
    return entries


def census(program):
    """[(subject, kind, use, packed page's address, bank)]: every tilemap page the program's data names, with
    the bank its tiles are shown with. kind: 'course' (the art of a stage's pictures), 'scene', 'road',
    'notices'; use: what names it, (what, number): ('stage', its number) for a stage's picture,
    ('background', b) for a change of background, ('lower animation', 0), ('upper animation', n),
    ('ranking', 0), ('set-up', address), ('boot', the list's address)."""
    uses = []
    lists = {}
    for background in range(BACKGROUNDS):
        address = u32(program, BACKGROUND_TILE_PAGE_LISTS + 4 * background) & 0xFFFFFF
        if address:
            lists[background] = [u32(program, address + 4 * page) & 0xFFFFFF for page in range(9)]
    for stage in range(STAGES):
        background = program[STAGE_BACKGROUNDS + stage]
        pages = ([u32(program, NO_BACKGROUND_TILE_PAGES + 4 * page) & 0xFFFFFF for page in range(4)]
                 if background == 0xFF else lists[background][4:8])
        for page in pages:
            uses.append((COURSES[stage], "course", ("stage", stage), page, program[STAGE_TILE_BANKS + stage]))
    for background, pages in lists.items():
        bank = program[BACKGROUND_TILE_BANKS + background]
        if background < 0x2A:
            leaves = background >> 1
            arriving = COURSES[program[FORK_NEXT_STAGES + background]]
        else:
            leaves = background - 0x2A
            arriving = SECOND_PICTURES[leaves]
        for index, page in enumerate(pages):
            subject = COURSES[leaves] if index < 4 else arriving
            uses.append((subject, "course", ("background", background), page, bank))
    for screen in range(2):
        for _, page in page_list(program, u32(program, LOWER_ANIMATION_PAGES + 4 * screen)):
            uses.append((COURSES[6], "course", ("lower animation", 0), page,
                         program[BACKGROUND_TILE_BANKS + 0x05]))
    for animation, background in enumerate(UPPER_ANIMATION_BACKGROUNDS):
        reaches = program[FORK_NEXT_STAGES + background]
        for screen in range(2):
            pages = page_list(program, u32(program, UPPER_ANIMATION_PAGES + 4 * (animation * 2 + screen)))
            for _, page in pages:
                uses.append((COURSES[reaches], "course", ("upper animation", animation), page,
                             program[BACKGROUND_TILE_BANKS + background]))
    for course in range(10):
        background = program[COURSE_BACKGROUNDS + course]
        for page in lists[background][4:8]:
            uses.append((COURSES[program[FORK_NEXT_STAGES + background]], "course", ("ranking", 0), page,
                         program[RANKING_TILE_BANKS + background]))
    setups = [(scene, address + 20 * index) for scene, address, count in BACKGROUND_SETUPS
              for index in range(count)]
    setups += [("car_select", u32(program, CAR_SELECT_TILEMAP_SETUPS + 32 * index + 0x18) & 0xFFFFFF)
               for index in range(2)]
    for scene, address in setups:
        pages, second_pages, bank, second_bank, _, _, tile_pages = struct.unpack_from("<IIBBBBI", program, address)
        # (a page in the second tilemap's page word alone takes its bank; the rest, the first one's)
        second = ({(second_pages >> shift) & 0xFF for shift in (0, 8, 16, 24)}
                  - {(pages >> shift) & 0xFF for shift in (0, 8, 16, 24)})
        for number, page in page_list(program, tile_pages & 0xFFFFFF):
            uses.append((scene, "scene", ("set-up", address), page, second_bank if number in second else bank))
    for _, page in page_list(program, ROAD_PAGES):
        uses.append(("road", "road", ("boot", ROAD_PAGES), page, 0))
    for address in NOTICE_PAGES:
        for _, page in page_list(program, address):
            uses.append(("notices", "notices", ("boot", address), page, NOTICE_BANK))
    return uses


def tile_users(program):
    """{tile: {(subject, kind)}} and {(subject, kind): {use}}: who uses each tile, by the census."""
    pages = {}
    users, whats = {}, {}
    for subject, kind, use, page, bank in census(program):
        if page not in pages:
            data = unpack(program, page)
            pages[page] = [data[index] | data[index + 1] << 8 for index in range(0, PAGE_CELLS * 2, 2)]
        for cell in pages[page]:
            users.setdefault(bank * BANK_TILES + (cell & 0x1FFF), set()).add((subject, kind))
        whats.setdefault((subject, kind), set()).add(use)
    return users, whats


def shown_tiles(census_dir):
    """{tile: Counter of scenarios} the tile censuses of palette_census.py's run saw (the board takes a tile
    number modulo the tile ROM's size)."""
    shown = {}
    for path in sorted(Path(census_dir).glob("tiles_*.txt")):
        with open(path, encoding="ascii") as lines:
            for line in lines:
                tile = int(line[:line.index(" ")], 16) % TILE_COUNT
                shown.setdefault(tile, Counter())[path.stem[6:]] += 1
    return shown


# ---- the tilesets ----

KIND_ORDER = {"course": 0, "notices": 1, "road": 2, "scene": 3}


def row_owners(users, tiles_rom):
    """The subject of each row of 32 tiles: (subject, kind) or None for a row nothing uses. A course's art
    comes before the road, the notices and the scenes that show it too; among equals, the one with the most
    tiles in the row. Blank tiles do not count where the row has others (every page of a bank has the
    blank tile at its start)."""
    blank = bytes(asset_formats.TILE_BYTES)
    owners = []
    for row in range(TILE_COUNT // ROW):
        tiles = [tile for tile in range(row * ROW, (row + 1) * ROW) if tile in users]
        drawn = [tile for tile in tiles if tile_bytes(tiles_rom, [tile]) != blank]
        counts = Counter(user for tile in (drawn or tiles) for user in users[tile])
        owners.append(min(counts, key=lambda user: (KIND_ORDER[user[1]], -counts[user], user)) if counts else None)
    return owners


def ranges_of(tiles):
    """Contiguous [first, last] runs of sorted tile numbers."""
    runs = []
    for tile in tiles:
        if runs and tile == runs[-1][1] + 1:
            runs[-1][1] = tile
        else:
            runs.append([tile, tile])
    return runs


def folder_of(subject, kind):
    return {"course": "courses/", "scene": "scenes/"}.get(kind, "") + subject


def numbers(values, digits=None):
    """'1', '1 and 2', '1, 2 and 3' (hex with 'digits')."""
    texts = [f"{value:0{digits}X}" if digits else str(value) for value in sorted(values)]
    return texts[0] if len(texts) == 1 else ", ".join(texts[:-1]) + " and " + texts[-1]


def describe(subject, kind, uses):
    """What names a subject's pages, in words."""
    by = {}
    for what, number in uses:
        by.setdefault(what, set()).add(number)
    if kind == "course":
        stages = by.get("stage", set())
        parts = [("stage " if len(stages) == 1 else "stages ") + numbers(stages) if stages else "no stage's start"]
        if "background" in by:
            parts.append(("backgrounds " if len(by["background"]) > 1 else "background ")
                         + numbers(by["background"], 2))
        if "lower animation" in by:
            parts.append("the lower animation")
        if "upper animation" in by:
            parts.append(("upper animations " if len(by["upper animation"]) > 1 else "upper animation ")
                         + numbers(by["upper animation"]))
        if "ranking" in by:
            parts.append("the ranking")
        return ", ".join(parts)
    if kind == "scene":
        return SCENE_NOTES[subject] + " (set_up_background's records at " + numbers(by["set-up"], 6) + ")"
    if kind == "road":
        return "the road's pages (boot_tile_pages_29_to_4e)"
    return ("pages 5D and 5E (boot_tile_pages_5d, boot_tile_pages_5e): the FBI and EPA notices, which no "
            "tilemap of this set shows")


def partition(program, tiles_rom):
    """[(name, [(first, last)], description)] in the order of their first tile: every row of the tile ROM in
    one tileset."""
    users, whats = tile_users(program)
    owners = row_owners(users, tiles_rom)
    rows = len(owners)
    # runs of rows nothing uses: to the subject on both sides of them, or a tileset of their own
    unused_runs = []
    row = 0
    while row < rows:
        if owners[row] is not None:
            row += 1
            continue
        start = row
        while row < rows and owners[row] is None:
            row += 1
        before = owners[start - 1] if start else None
        after = owners[row] if row < rows else None
        if before is not None and before == after and (start * ROW) // BANK_TILES == (row * ROW - 1) // BANK_TILES:
            owners[start:row] = [before] * (row - start)
        else:
            unused_runs.append((start, row, before))
    sets = {}
    for row, owner in enumerate(owners):
        if owner is not None:
            sets.setdefault(folder_of(*owner), []).extend(range(row * ROW, (row + 1) * ROW))
    descriptions = {}
    for (subject, kind), uses in whats.items():
        name = folder_of(subject, kind)
        if name in sets:
            used = sum(1 for tile in sets[name] if (subject, kind) in users.get(tile, ()))
            descriptions[name] = (describe(subject, kind, uses) + f": {len(sets[name])} tiles, "
                                  f"{len(sets[name]) - used} in no page")
    blank = bytes(asset_formats.TILE_BYTES)
    row_bytes = ROW * asset_formats.TILE_BYTES
    rows_like = {}
    for row in range(rows):
        rows_like.setdefault(tiles_rom[row * row_bytes:(row + 1) * row_bytes], []).append(row)
    taken = Counter()
    for start, end, before in unused_runs:
        # the rows all blank apart from the others, each run a tileset named after the subject before it
        row = start
        while row < end:
            empty_rows = tiles_rom[row * row_bytes:(row + 1) * row_bytes] == blank * ROW
            first = row
            while row < end and (tiles_rom[row * row_bytes:(row + 1) * row_bytes] == blank * ROW) == empty_rows:
                row += 1
            tiles = list(range(first * ROW, row * ROW))
            base = "unused/" + ("blank_" if empty_rows else "") + ("after_" + before[0] if before else "start")
            taken[base] += 1
            name = base if taken[base] == 1 else f"{base}_{taken[base]}"
            empty = sum(1 for tile in tiles if tile_bytes(tiles_rom, [tile]) == blank)
            solid = sum(1 for tile in tiles if len(set(tile_bytes(tiles_rom, [tile]))) == 1) - empty
            note = f"{len(tiles)} tiles no page names: {empty} blank"
            if not empty_rows:
                note += f", {solid} of one pen, {len(tiles) - empty - solid} drawn"
                # (rows equal to rows a subject has, in another bank: the banks' copies of one picture)
                copies = {folder_of(*owners[other]) for line in range(first, row)
                          if tiles_rom[line * row_bytes:(line + 1) * row_bytes] != blank * ROW
                          for other in rows_like[tiles_rom[line * row_bytes:(line + 1) * row_bytes]]
                          if owners[other] is not None}
                if copies:
                    note += "; rows equal to rows of " + ", ".join(sorted(copies))
            sets[name] = tiles
            descriptions[name] = note
    found = []
    for name, tiles in sets.items():
        found.append((name, [tuple(run) for run in ranges_of(sorted(tiles))], descriptions.get(name, "")))
    found.sort(key=lambda entry: entry[1][0][0])
    return found


def write_index(path, found):
    lines = ["# The tile ROM's 32768 16x16 tiles in tilesets by subject (tools/tilesets.py writes it from the",
             "# census of the tilemaps the program's data holds; it can be edited by hand). A line a tileset:",
             "# 'NAME FIRST-LAST ...  # description', its tiles by number (hex: the bank, 0-3, in the top two of",
             "# 15 bits, as the tilemaps show them), in the order its PNG lays them out, 32 a row. Every tile is",
             "# in one tileset; 'in no page' are tiles no tilemap page of the program names."]
    for name, ranges, description in found:
        text = " ".join(f"{first:04X}-{last:04X}" if first != last else f"{first:04X}" for first, last in ranges)
        lines.append(f"{name} {text}" + (f"  # {description}" if description else ""))
    path.write_text("\n".join(lines) + "\n", encoding="ascii")


LINE = re.compile(r"^([a-z0-9_/]+)((?:\s+[0-9A-F]{4}(?:-[0-9A-F]{4})?)+)\s*(?:#.*)?$")


def load_index(path):
    """[(name, [tile numbers in PNG order])] of tilesets.txt; every tile number in exactly one."""
    sets = []
    seen = {}
    for number, line in enumerate(Path(path).read_text(encoding="ascii").splitlines(), 1):
        if not line.strip() or line.startswith("#"):
            continue
        match = LINE.match(line)
        if not match:
            raise SystemExit(f"{path}:{number}: not a tileset line")
        tiles = []
        for part in match.group(2).split():
            first, _, last = part.partition("-")
            tiles.extend(range(int(first, 16), int(last or first, 16) + 1))
        for tile in tiles:
            if tile >= TILE_COUNT or tile in seen:
                raise SystemExit(f"{path}:{number}: tile {tile:04X} "
                                 + ("past the tile ROM" if tile >= TILE_COUNT else f"already in {seen[tile]}"))
            seen[tile] = match.group(1)
        sets.append((match.group(1), tiles))
    if len(seen) < TILE_COUNT:
        missing = [tile for tile in range(TILE_COUNT) if tile not in seen]
        raise SystemExit(f"{path}: {len(missing)} tiles in no tileset, from {missing[0]:04X}")
    return sets


def tile_bytes(tiles_rom, tiles):
    size = asset_formats.TILE_BYTES
    return b"".join(tiles_rom[tile * size:(tile + 1) * size] for tile in tiles)


def extract(asset, tiles_rom, program, folder, palette_colors):
    """Every tileset of the index as an 8-bit indexed PNG of 32 tiles a row in colour: the 16 palettes its
    tiles are shown with the most (tile_palettes.json), a tile's pixels at its palette's slot * 16 + pen, so
    that index & 15 is the pen (pen 0 see-through; a tile whose palette is not among them, at its pen); and
    the index beside them (tilesets.txt), which the game reads them by. Returns the number of files written."""
    known = json.loads((ROOT / asset["palettes"]).read_text(encoding="ascii"))
    formats = known.get("formats", {})
    palettes = {int(tile, 16): int(offset, 0) for source in ("drawn", "nearby", "chosen")
                for tile, offset in known.get(source, {}).items()}
    files = 0
    for name, tiles in load_index(ROOT / asset["index"]):
        used = Counter(palettes[tile] for tile in tiles if tile in palettes)
        slots = [offset for offset, _ in used.most_common(SLOTS)]
        colours = []
        for offset in slots:
            alternate = formats.get(f"0x{offset:06X}", "alternate") == "alternate"
            colours += palette_colors(program, offset, 16, alternate)
        colours += [(level * 17,) * 3 for level in range(16)] * (SLOTS - len(slots))
        rows = asset_formats.tile_sheet_pens(tile_bytes(tiles_rom, tiles), ROW)
        for index, tile in enumerate(tiles):
            slot = slots.index(palettes[tile]) if palettes.get(tile) in slots else 0
            if slot:
                top, left = (index // ROW) * 16, (index % ROW) * 16
                for y in range(top, top + 16):
                    rows[y][left:left + 16] = [slot * 16 + pen for pen in rows[y][left:left + 16]]
        target = folder / f"{name}.png"
        target.parent.mkdir(parents=True, exist_ok=True)
        asset_formats.write_png_indexed(target, rows, colours, 8,
                                        alphas=[0 if index % 16 == 0 else 255 for index in range(SLOTS * 16)])
        files += 1
    (folder / "tilesets.txt").write_text((ROOT / asset["index"]).read_text(encoding="ascii"), encoding="ascii")
    return files + 1


def build(asset):
    """The tile ROM from the tilesets' PNGs (the low 4 bits of each index are the pen), by the index."""
    out = bytearray(asset["size"])
    for name, tiles in load_index(ROOT / asset["index"]):
        rows, _ = asset_formats.read_png_indexed(ROOT / asset["path"] / f"{name}.png")
        data = asset_formats.tiles_from_sheet(rows, ROW, len(tiles))
        for index, tile in enumerate(tiles):
            out[tile * asset_formats.TILE_BYTES:(tile + 1) * asset_formats.TILE_BYTES] = \
                data[index * asset_formats.TILE_BYTES:(index + 1) * asset_formats.TILE_BYTES]
    return bytes(out)


def pack_pieces(asset):
    """[(name, offset in the tile ROM, bytes)]: each tileset's ranges, for the assets pack."""
    rom = build(asset)
    pieces = []
    for name, tiles in load_index(ROOT / asset["index"]):
        for first, last in ranges_of(tiles):
            pieces.append((f"{name}[{first:04X}]", first * asset_formats.TILE_BYTES,
                           rom[first * asset_formats.TILE_BYTES:(last + 1) * asset_formats.TILE_BYTES]))
    return pieces


def main():
    if len(sys.argv) > 2:
        raise SystemExit(__doc__.split("Usage:")[1].split("\n\n")[0])
    rom = orunners_rom.load_set(ROOT / "rom")
    program = orunners_rom.build_program(rom)
    tiles_rom = orunners_rom.build_tiles(rom)
    found = partition(program, tiles_rom)
    write_index(INDEX, found)
    users, _ = tile_users(program)
    unused = [entry for entry in found if entry[0].startswith("unused/")]
    unused_tiles = sum(last - first + 1 for _, ranges, _ in unused for first, last in ranges)
    print(f"written: {INDEX}")
    print(f"  {len(found)} tilesets, {len(found) - len(unused)} by subject; {len(users)} tiles in the pages, "
          f"{TILE_COUNT - len(users)} in none; {unused_tiles} in {len(unused)} unused tilesets")
    if len(sys.argv) == 2:
        shown = shown_tiles(sys.argv[1])
        only_shown = sorted(set(shown) - set(users))
        blank = sum(1 for tile in only_shown if tile_bytes(tiles_rom, [tile]) == bytes(asset_formats.TILE_BYTES))
        print(f"  the scenarios show {len(shown)} tiles: {len(set(shown) & set(users))} in the pages, "
              f"{len(only_shown)} not ({blank} blank); in the pages and not shown: {len(set(users) - set(shown))}")
        for first, last in ranges_of(only_shown):
            print(f"    shown, in no page: {first:04X}-{last:04X}")
        for name, ranges, _ in found:
            tiles = [tile for first, last in ranges for tile in range(first, last + 1)]
            scenarios = Counter(scenario for tile in tiles for scenario in shown.get(tile, ()))
            print(f"  {name}: {sum(1 for tile in tiles if tile in shown)} of {len(tiles)} tiles shown, in "
                  + (", ".join(sorted(scenarios)) if scenarios else "no scenario"))


if __name__ == "__main__":
    main()
