"""
Name:        compare_with_mame.py
Description: decodes every byte offset of the V60 program with our disassembler and with MAME's, and diffs them
Version:     0.1.0
Created:     11-10-2026
Modified:    13-10-2026
Author:      Sebastian Toyos

Usage: python tools/reference/compare_with_mame.py [rom folder or .zip]
Needs build/bin/oracle (built by CMake with the reference tools).
Decoding at every offset, not only at real instructions, exercises every opcode and
addressing mode that appears in the ROM bytes, including garbage.
"""
import subprocess
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))
import orunners_rom  # noqa: E402
from v60 import disassembler  # noqa: E402

ORACLE = ROOT / "build" / "bin" / ("oracle.exe" if sys.platform == "win32" else "oracle")


def our_line(mem, address):
    instruction = disassembler.decode(mem, address)
    text = instruction.text
    # MAME prints the byte after an unassigned main opcode instead of the opcode itself
    opcode = mem.r8(address)
    if instruction.flow == disassembler.FLOW_INVALID and instruction.length == 1 \
            and not (0x58 <= opcode <= 0x5F) and disassembler.OPCODES[opcode] is None:
        text = f"${mem.r8(address + 1):02X}"
    return f"{address:06X} {instruction.length} {text}"


def main():
    if not ORACLE.exists():
        raise SystemExit(f"{ORACLE} not found")
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "rom"
    image = orunners_rom.build_program(orunners_rom.load_set(source))

    work = ROOT / "build" / "reference"
    work.mkdir(parents=True, exist_ok=True)
    image_path = work / "maincpu.bin"
    image_path.write_bytes(image)
    mem = disassembler.FlatMemory(image, len(image) - 1)

    differences = 0
    compared = 0
    for start, end in orunners_rom.PROGRAM_AREAS:
        mame_path = work / f"mame_{start:06X}.txt"
        subprocess.run([str(ORACLE), str(image_path), hex(start), hex(end), str(mame_path)], check=True)
        with open(mame_path, encoding="ascii") as mame_file:
            for address, mame_line in zip(range(start, end), mame_file):
                mame_line = mame_line.rstrip("\n")
                ours = our_line(mem, address)
                compared += 1
                if ours != mame_line:
                    differences += 1
                    if differences <= 20:
                        print(f"MAME: {mame_line}\nOURS: {ours}\n")
    print(f"{compared} offsets compared, {differences} differences")
    return 0 if differences == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
