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
- [x] The sound board in the reference: the sound program on MAME's Z80, with ymfm's YM3438 (which the
      program uses only for its timer) and MAME's MultiPCM, which plays the music and the effects
      (`s32ref --sound-wav`), the same sample for sample from one run to the next
- [x] Scenarios (`tests/controls/`): the attract mode; a game until GAME OVER; races to all ten goals of the
      map, the race time held by a poke (`race_*`, `two_players`, and the `route_*` that
      `tools/make_routes.py` writes, turning the wheel before the forks a route takes); a name typed into
      the entry; every page of the test mode and its 122 sounds. Between them they execute 16,103 addresses
      (`python tools/check_scenarios.py --reference-only`); the game has no continue
- [x] Recursive disassembly without traces (`tools/disassemble.py`, from `symbols.txt`): the vectors, the
      tables of code addresses next to the jumps that read them, the object handlers (stored in field 4,
      from lists, tables of lists and the streams of objects along each stage's road, which fork with the
      route) and a timeline: 19,708 instructions in 640 functions, no invalid or overlapping paths, and all
      16,103 addresses the scenarios execute among them
- [x] Coverage (`tools/coverage.py`): every byte of the code chips is code, data that the code or the
      tables `symbols.txt` describes point at, or fill; none is unknown. To find the last of it,
      `s32ref --rom-reads FILE` lists what a run reads of the ROM as data, by instruction. The code and the
      tables nothing uses are in `symbols.txt` too (its last section): 52 functions nothing calls are
      decompiled with the rest; 1,039 bytes of code are left out of the C, the reset vector's jump and 14
      places of dead code that jump or call into the middle of functions the game runs, whose C would cut
      those functions in two
- [x] Translation scaffold (`tools/recompile.py`): the program translated into C one instruction at a time,
      615 functions that the V60 runtime (`runtime/src/v60.c`) runs a frame at a time. `orunners_headless`
      matches the reference frame by frame in all 16 scenarios. The game runs two tasks, each on a stack
      of its own, and its VBLANK handler swaps them; their C runs on native stacks of their own
      (`runtime/src/tasks.c`)
- [x] Decompiler, first pass (`tools/decompile.py`): the program decompiled into C, in `src/`: 671 functions
      with no goto, which the game's runtime (`runtime/src/game.c`) runs a frame at a time.
      `orunners_game_headless` matches the reference frame by frame in all 16 scenarios, and
      `tools/check_structure.py` finds the C's ifs, loops and switches doing what the program's jumps do in
      every function, the ones no scenario runs included (and in the sound program's 60, with `--sound`). The VBLANK handler
      asks the runtime to swap the tasks (`swap_tasks`), and a function that returns for its caller too
      says so (`left_caller`). The registers the whole program shares are globals, a copy for each task:
      `current_object`, `player_car` and `screen` (named by `symbols.txt`), and R12, R14 and R26-R28,
      scratch registers whose last value the next function may read, still named after them (2,036 uses,
      which `tools/noise_report.py` counts). Most names are still addresses, and some of SegaSonic's (`GAME_VARIABLES`,
      `PLAYER_BLOCK`) stand where OutRunners' will go
- [x] The game's own video and sound: `runtime/src/video.c` draws both screens; the sound program,
      decompiled into C (`src/sound/`, `tools/decompile_sound.py` from `sound_symbols.txt`), runs on the
      board's time with the YM3438 (`runtime/src/ym3438.c`) and the MultiPCM (`runtime/src/multipcm.c`,
      the same as MAME's sample for sample). `orunners_game_headless` matches the reference frame by frame
      in state, picture and sound in all 16 scenarios (`python tools/check_scenarios.py --video --sound`).
      Left out of the comparisons: the V70's and the Z80's stacks, which the C keeps in no memory, and the
      copies the test mode's RAM tests leave of them in the sprite RAM (`runtime/include/s32/machine.h`)
- [x] A window port (`orunners`, `ports/sdl2/`): both screens, the sound, the keyboard and game controllers;
      a control script played in the window gives the reference's frames (`coin_start`: state, picture
      and sound)
- [ ] Names, types and structures for the C (`symbols.txt` and `sound_symbols.txt`); the assets, and the
      ROM out of every executable (today the build still writes the ROM's regions into them); other ports

## Getting started

Requirements: Python 3.10 or newer, CMake 3.20 or newer and a C11 / C++17 compiler (Visual Studio Build
Tools on Windows). The window port needs SDL2 2.32: on Windows its development package for Visual C++
(`SDL2-devel-2.32.10-VC.zip`) unpacked in `build/downloads/sdl2/`, elsewhere the system's.

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

   `tests/controls/` holds the control scripts (format in `runtime/include/s32/script.h`). With
   `--sound-wav FILE` the sound board runs too and what it plays is written as a WAV file. The attract
   mode, with the settings a new EEPROM gets, plays no note; a game does (`tests/controls/coin_start.txt`).

4. Run every scenario on the reference, translate the program into C from what they execute, build that
   translation (`orunners_headless`) and compare it with the reference, frame by frame:

   ```bash
   python tools/check_scenarios.py --reference-only
   ```

   ```bash
   python tools/recompile.py --trace build/scenarios/*/executed.txt --calls build/scenarios/*/called.txt
   ```

   ```bash
   cmake -S . -B build/cmake -DORUNNERS_SCAFFOLD=ON
   ```

   ```bash
   cmake --build build/cmake --config Release
   ```

   ```bash
   python tools/check_scenarios.py --translated --native-only
   ```

   The translation goes to `build/generated/` and stays out of the repository: it is the game's code.

5. The decompiled program is in `src/`, and the build makes `orunners_game_headless` from it. Compare it
   with the reference (the first command of step 4 writes the reference's results):

   ```bash
   python tools/check_scenarios.py --native-only
   ```

   `python tools/decompile.py` writes `src/` again, from the ROM and `symbols.txt`, and
   `python tools/decompile_sound.py` the sound program's C (`src/sound/`), from the ROM and
   `sound_symbols.txt`, with the timing model the reference runs the sound program on
   (`build/sound/timing.txt`). With the picture and the sound:

   ```bash
   python tools/check_scenarios.py --video --sound
   ```

   (this runs the reference again: with `--sound` its Z80 keeps the game's time, block by block).

6. Play it in a window:

   ```bash
   build/bin/orunners
   ```

   The two screens side by side, the cabinet's two seats. Arrows: the wheel, gas and brake; Q and A:
   shift up and down; space: the DJ, `[` and `]`: the songs; 5 and 6: coins; 1 and 2: starts; 9: service;
   F2: test; F11: full screen; Escape: quit. A game controller per player: the left stick is the wheel,
   the triggers the pedals, the shoulders the gears. `--script FILE` plays a control script instead
   (`--fast` without waiting for the frames' time), and `--hashes`, `--video-hashes` and
   `--sound-hashes` write the same hashes as the reference.

## Tools

`tools/` starts as a copy of segasonic-decomp's tools at commit `c329452`: the disassemblers and lifters
of the V60 (which decode the V70's code as they are) and the Z80, the decompiler, the checks, and the
reference tools built on MAME's cores. They are adapted to the Multi 32 and to OutRunners as each step
needs them: `orunners_rom.py` replaced `sonic_rom.py`, and the decompiler's settings (the registers the
program shares, R24 as the base of its variables, the object handler's field) are OutRunners'. Some of
SegaSonic's names stay in them until `symbols.txt` gives OutRunners' own. SegaSonic's own data formats and
stage scripts were left out.

## Legal

- This project is licensed under the BSD 3-Clause License; see `LICENSE`.
- OutRunners is a trademark of SEGA. This project is not affiliated with or endorsed by SEGA.
