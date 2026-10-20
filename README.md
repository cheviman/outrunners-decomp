# OutRunners — decompilation

A reconstruction of Sega's 1992 arcade game *OutRunners* (Sega System Multi 32) into platform-agnostic C,
not an emulator, following the method of
[segasonic-decomp](https://github.com/cheviman/segasonic-decomp): the game's program decompiled one
function at a time, checked frame by frame against MAME in picture and sound.

**Work in progress.** This repository will contain no game data. As with
[n64decomp/sm64](https://github.com/n64decomp/sm64), you bring your own ROM set (MAME's `orunners`), and
the tools verify it, extract the assets and build your own executable.

## Status

- [x] ROM set verification (MAME set `orunners`, World), by CRC32 and SHA-1 of every file
- [x] Multi 32 board model in C (`runtime/`): memory map, interrupt controller and timers, the two I/O
      chips, EEPROM, the A/D converter of the wheels and pedals, and the link board with no other cabinet
      on the line
- [x] Reference run of the original program on MAME's V70 core against that model (`s32ref`), with MAME's
      Multi 32 video drawing both screens: it boots, runs the attract mode and plays a scripted game, the
      same frame by frame from one run to the next
- [ ] The sound board in the reference (Z80, YM3438, MultiPCM)
- [ ] Scenarios, the static analysis, the decompiler, the game's own video and sound, assets, a port

## Getting started

Requirements: Python 3.10 or newer, CMake 3.20 or newer and a C11 / C++17 compiler (Visual Studio Build
Tools on Windows).

1. Put your MAME `orunners.zip`, or the files inside it, in `rom/`, and check it:

   ```bash
   python tools/verify_rom.py
   ```

2. Build the tools into `build/bin/` (the build writes the ROM regions into the executables):

   ```bash
   cmake -S . -B build/cmake
   ```

   ```bash
   cmake --build build/cmake --config Release
   ```

3. Run the original program on the reference, here the attract mode for a minute, with a frame of both
   screens every ten seconds as BMP:

   ```bash
   build/bin/s32ref --script tests/controls/attract.txt --video-dump 600 --video-dump 1200 --video-dir build/frames
   ```

   `tests/controls/` holds the control scripts (format in `runtime/include/s32/script.h`).

## Tools

`tools/` starts as a copy of segasonic-decomp's tools at commit `c329452`: the disassemblers and lifters
of the V60 (which decode the V70's code as they are) and the Z80, the decompiler, the checks, and the
reference tools built on MAME's cores. They still describe SegaSonic's board and game (`sonic_rom.py`, the
decompiler's settings) and are being adapted to the Multi 32 and to OutRunners. SegaSonic's own data
formats and stage scripts were left out.

## Legal

- This project is licensed under the BSD 3-Clause License; see `LICENSE`.
- OutRunners is a trademark of SEGA. This project is not affiliated with or endorsed by SEGA.
