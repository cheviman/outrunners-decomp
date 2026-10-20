# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        compare_z80_with_mame.py
Description: decodes every byte offset of the Z80 sound program with our disassembler and with MAME's, and diffs them
Version:     0.1.0
Created:     13-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

Usage: python tools/reference/compare_z80_with_mame.py [rom folder or .zip]
Needs build/bin/oracle_z80 (built by CMake with the reference tools).
The sound program, epr-15785 (the first 0x40000 bytes of the sound region), is decoded as one linear
image: both sides read the bytes that follow each offset in the image, wrapping at its end, and print
the offset; relative jump targets are 16-bit Z80 addresses, so every line is what the Z80 sees at
offset & 0xFFFF. A second image, built here, holds every opcode of every prefix followed by operand
bytes, so that the encodings the sound program never uses are compared too.
"""
import subprocess
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import orunners_rom  # noqa: E402
from z80 import disassembler  # noqa: E402

ORACLE = ROOT / "build" / "bin" / ("oracle_z80.exe" if sys.platform == "win32" else "oracle_z80")
PROGRAM_SIZE = 0x40000   # epr-15785.ic36


def opcode_image():
    """Every opcode after every prefix (none, CB, ED, DD, FD, DD CB d, FD CB d), each followed by operand
    bytes; once with small positive values and once with values that need a sign. Padded with NOPs to a
    power of two, as the oracle wants."""
    sequences = []
    for operands in (b"\x05\x34\x12", b"\x80\xff\x7f"):
        for op in range(256):
            sequences.append(bytes([op]) + operands)
            sequences.append(bytes([0xCB, op]))
            sequences.append(bytes([0xED, op]) + operands)
            for prefix in (0xDD, 0xFD):
                sequences.append(bytes([prefix, op]) + operands)
                sequences.append(bytes([prefix, 0xCB, operands[0], op]))
    image = b"".join(sequences)
    size = 1 << (len(image) - 1).bit_length()
    return image + bytes(size - len(image))


def our_line(mem, offset):
    instruction = disassembler.decode(mem, offset)
    return f"{offset:05X} {instruction.length} {instruction.text}"


def compare(name, image, work):
    """Decode every offset of the image on both sides; returns the number of differences."""
    image_path = work / f"z80_{name}.bin"
    image_path.write_bytes(image)
    mame_path = work / f"mame_z80_{name}.txt"
    subprocess.run([str(ORACLE), str(image_path), "0", hex(len(image)), str(mame_path)], check=True)
    mem = disassembler.FlatMemory(image, len(image) - 1)

    differences = 0
    compared = 0
    with open(mame_path, encoding="ascii") as mame_file:
        for offset, mame_line in zip(range(len(image)), mame_file):
            mame_line = mame_line.rstrip("\n")
            ours = our_line(mem, offset)
            compared += 1
            if ours != mame_line:
                differences += 1
                if differences <= 20:
                    print(f"MAME: {mame_line}\nOURS: {ours}\n")
    if compared != len(image):
        print(f"{name}: MAME printed {compared} lines for {len(image)} offsets")
        differences += len(image) - compared
    print(f"{name}: {compared} offsets compared, {differences} differences")
    return differences


def main():
    if not ORACLE.exists():
        raise SystemExit(f"{ORACLE} not found")
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "rom"
    program = orunners_rom.build_sound(orunners_rom.load_set(source))[:PROGRAM_SIZE]

    work = ROOT / "build" / "reference"
    work.mkdir(parents=True, exist_ok=True)
    differences = compare("program", program, work)
    differences += compare("opcodes", opcode_image(), work)
    return 0 if differences == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
