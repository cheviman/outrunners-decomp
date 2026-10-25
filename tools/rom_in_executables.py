"""
Name:        rom_in_executables.py
Description: looks for the ROM set's content in the built executables: none of the game's may hold any
Version:     0.1.0
Created:     25-10-2026
Modified:    25-10-2026
Author:      Sebastian Toyos

Usage: py tools/rom_in_executables.py [EXECUTABLE ...]   (default: the game's in build/bin)

Every 32-byte stretch of each region of the ROM set (rom/), at every 32nd byte, is looked for in the
executables; a stretch any binary could hold (almost all one byte, up to three runs of two bytes, or bytes or
16-bit words in an arithmetic progression) is left out. Prints, for each executable, how many stretches of
each region it holds; the game's must hold none (the reference tools read the ROM set when they start, and
hold none either).
"""
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import orunners_rom  # noqa: E402

SPAN = 32
GAME = ["orunners_game_headless.exe", "orunners.exe"]


def trivial(chunk):
    """Almost one byte over and over (all but 4 of them: a run of padding with a value at an edge), up to three
    runs of two bytes (zeros, ones, zeros), or bytes or little-endian 16-bit words in an arithmetic progression."""
    if max(chunk.count(value) for value in set(chunk)) >= len(chunk) - 4:
        return True
    if len(set(chunk)) == 2 and sum(1 for index in range(1, len(chunk)) if chunk[index] != chunk[index - 1]) <= 2:
        return True
    steps = {(chunk[index + 1] - chunk[index]) & 0xFF for index in range(len(chunk) - 1)}
    if len(steps) == 1:
        return True
    words = struct.unpack(f"<{len(chunk) // 2}H", chunk)
    return len({(words[index + 1] - words[index]) & 0xFFFF for index in range(len(words) - 1)}) == 1


def main():
    paths = [Path(argument) for argument in sys.argv[1:]] or \
        [ROOT / "build" / "bin" / name for name in GAME if (ROOT / "build" / "bin" / name).exists()]
    rom = orunners_rom.load_set(ROOT / "rom")
    regions = {"program": orunners_rom.build_program(rom), "tiles": orunners_rom.build_tiles(rom),
               "sprites": orunners_rom.build_sprites(rom), "sound": orunners_rom.build_sound(rom),
               "pcm": orunners_rom.build_pcm(rom)}
    failed = False
    for path in paths:
        data = path.read_bytes()
        windows = {data[at:at + SPAN] for at in range(len(data) - SPAN + 1)}
        found = {}
        for name, region in regions.items():
            count = 0
            for at in range(0, len(region) - SPAN + 1, SPAN):
                chunk = region[at:at + SPAN]
                if not trivial(chunk) and chunk in windows:
                    count += 1
            found[name] = count
        failed |= any(found.values())
        print(f"{path.name}: " + ", ".join(f"{name} {count}" for name, count in found.items())
              + " stretches of the ROM set")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
