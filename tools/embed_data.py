"""
Name:        embed_data.py
Description: writes the game's data as C for the executable to carry: the board's ROM regions, built
             from the player's ROM set
Version:     0.1.0
Created:     13-10-2026
Modified:    19-10-2026
Author:      Sebastian Toyos

Usage: python tools/embed_data.py [ROM folder or .zip] [output folder]
       (default: rom/, and build/cmake/data; a folder holding the set's .zip is read through the .zip)

Writes program.c, tiles.c, sprites.c, sound.c and pcm.c, the arrays runtime/include/game/data.h
declares, laid out as the regions tools/orunners_rom.py builds. (OutRunners has no extracted assets yet:
the data is the ROM set's as it is.) CMake runs it when the ROM set changes; the C it writes stays in the
build folder, as the ROM never goes into the repository.
"""
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import orunners_rom  # noqa: E402

# file -> (array, builder)
REGIONS = {
    "program.c": ("game_program_rom", orunners_rom.build_program),
    "tiles.c": ("game_tile_rom", orunners_rom.build_tiles),
    "sprites.c": ("game_sprite_rom", orunners_rom.build_sprites),
    "sound.c": ("game_sound_rom", orunners_rom.build_sound),
    "pcm.c": ("game_pcm_rom", orunners_rom.build_pcm),
}
BYTES_PER_LINE = 32


def find_set(source):
    """The ROM set at 'source', or in the one .zip of a folder that has none of its files loose."""
    found, _ = orunners_rom.identify_set(source)
    if not found and source.is_dir():
        for archive in sorted(source.glob("*.zip")):
            found, _ = orunners_rom.identify_set(archive)
            if found:
                source = archive
                break
    return orunners_rom.load_set(source)


def write_array(path, name, data):
    lines = [
        f"/* {name}: written by tools/embed_data.py from the player's ROM set; not for the repository */",
        "#include \"game/data.h\"",
        "",
        f"const uint8_t {name}[{len(data)}] = {{",
    ]
    for start in range(0, len(data), BYTES_PER_LINE):
        lines.append(",".join(f"0x{byte:02x}" for byte in data[start:start + BYTES_PER_LINE]) + ",")
    lines.append("};")
    path.write_text("\n".join(lines) + "\n", encoding="ascii")


def main():
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "rom"
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "build" / "cmake" / "data"
    if not source.exists():
        raise SystemExit(f"{source}: no such ROM folder or file")
    rom = find_set(source)
    out_dir.mkdir(parents=True, exist_ok=True)
    for file_name, (name, builder) in REGIONS.items():
        data = builder(rom)
        write_array(out_dir / file_name, name, data)
        print(f"  {out_dir / file_name}  ({name}, {len(data)} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
