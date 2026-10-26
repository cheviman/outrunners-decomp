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
      `current_object`, `player_car` and `screen` (named by `symbols.txt`), and R12, R14, R27 and R28,
      scratch registers whose last value the next function may read, still named after them (1,495 uses,
      which `tools/noise_report.py` counts). The game's structures are typed (`tools/core/decompiler/pointers.py`, with `tools/game_profile.py`'s structures):
      the objects (0x100 bytes, 124 in the page of each screen's block), the screen's car (0x200 bytes at
      1000 of the block) and the block of the screen a task runs (`SCREEN_1`, `SCREEN_2`), read as
      `OBJECT(p)->field`, `CAR(p)->field` and `SCREEN(p)->field`, each player's race record
      (`RACE(p)`, which `symbols.txt` declares with the fields that point at it) and the objects' area
      below each screen's block (`OBJECT_AREA(screen - 0x8000u)`: the lists of the objects, their slots);
      what the code reaches by its own address in the screens' blocks and the race records reads as their
      fields (`SCREEN(SCREEN_1)->game_mode`, `RACE(RACE_COPY_1)->entry_frames`), an object whose handler
      is set to a family's function as that family's, and the variables' arrays by element
      (`time_difficulty[1]`, `video_registers[16]`). The objects' code is
      named and commented, family by family (the scenery and traffic along the road, the player's car and
      the race display, the radio and its DJ, the title, attract and select screens, the route map, the
      name entry), with each family's own fields, and so is the rest of the game's code: the boot and the
      two tasks' frame loops, the game's modes and steps, the race's logic and the link between cabinets,
      the sprite lists and the board's services (sound requests, palettes, text, the EEPROM, coins and
      bookkeeping), the test mode and the interrupt hooks, with the work RAM variables and screen fields
      they use; the code nothing that runs reaches is named and commented as well ("Unused:"). The sound
      program is named in `sound_symbols.txt`: one driver for both screens, each with its page of commands,
      eight sequence players and a queue of MIDI messages, played on 24 MultiPCM voices and the YM3438's
      six channels, and the engine and tyre noise the game drives through the page (the game's side of the
      shared RAM has the same names). Every parameter, result and local variable is named by what it holds
      (`symbols.txt`'s param, result and local lines: 2,100 were named after a register and 565 values the
      decompiler works out once were numbered), and so are the values a string move fills with; where a
      partial write chains two values of a register into one variable, its name says both
      (`model_then_animation`). Every variable has a name; 79 uses of fields are still named after
      their offset (`tools/noise_report.py`, unk_fields): the bytes that code reading or writing several
      fields at once covers with no field name (the object's word above `flags_high` the most), the memory
      test walking the RAM byte by byte with
      the object register, pointers that may be either of two families, the bytes of a family's arrays,
      and two bytes of the race record nothing reads
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
- [x] The assets (`extract_assets.py`, driven by `assets.json`, which holds positions only): the tiles as
      32 indexed PNG sheets in the palettes the game shows them with (`tile_palettes.json`, from a census
      of the scenarios), the sprites as 144 sheets of the piece records at 0x122B74 (`sprite_sheets.txt`,
      `sprite_palettes.json`), the MultiPCM's sample headers as text and its samples as WAV files (the
      fixed megabyte and the six pages the sound sets bank in), and the program's and the sound program's
      data as text, the code left out. `python extract_assets.py --check` builds the 17 chips back from
      them, byte for byte (the code from the set)
- [x] No executable carries the ROM: the game loads its assets pack (`orunners.assets`, which
      `tools/make_pack.py` writes from the assets; on a first run the game writes it from the player's ROM
      set itself, `--extract`), the reference and the translation scaffold read the ROM set when they
      start, and the memory test's ROM part is the board's (`runtime/replaced.txt`: the decompiled game has
      no ROM chips to sum). `tools/rom_in_executables.py` finds no stretch of the ROM in any executable,
      and the 16 scenarios are identical in state, picture and sound with the game on its pack
- [x] The C reads the program's data by name: the records of the program ROM are structures
      (`ROAD_ROW_RECORD(road_row_record)->animation`, `SPRITE_SIZE_STEP(size_step)->source_size`, the
      car's `car_handlings[model]`), the lists they lead to are typed
      (`FLASHING_SCENERY_PALETTES(palettes)[0]`), and every table has a name: none
      is named after its address any more (`tools/noise_report.py`, rom_names: 709 uses before), and 26
      reads still go through a ROM address (rom_reads, 598 before). Those are the course's stream, whose
      commands have operands of different widths behind one pointer (16, `player_car_extend_course` and
      `player_car_build_segment`); the input test's list, which one pointer walks through two layouts in a
      row (9); and the road objects' stream after a fork, read through the object's field in one operand
      (1). Every ROM address the code uses as a value has a name (a data or table line, or a constant
      line for a number that only looks like one: 101 before) but two (raw_rom_addresses): the switches
      that go on through the original's own jump target in `car_radio_update` and `song_title_show`,
      where the code's address stands for "go on here", as the cases do. The data's text files are
      grouped by 64 KB of the ROM until they are cut where the tables start
- [ ] Other ports

## Getting started

Requirements: Python 3.10 or newer, CMake 3.20 or newer and a C11 / C++17 compiler (Visual Studio Build
Tools on Windows). The window port needs SDL2 2.32: on Windows its development package for Visual C++
(`SDL2-devel-2.32.10-VC.zip`) unpacked in `build/downloads/sdl2/`, elsewhere the system's.

The decompiler, the analysis tools and the asset formats are a project of their own,
[arcade-decomp](https://github.com/cheviman/arcade-decomp), which this repository keeps in `tools/core/` as
a git submodule: clone it with the submodule (`git submodule update --init` in a clone made without it).
The build needs it to take the assets out of your ROM set:

```bash
git clone --recursive https://github.com/cheviman/outrunners-decomp
```

1. Put your MAME `orunners.zip`, or the files inside it, in `rom/`, and check it:

   ```bash
   python tools/verify_rom.py
   ```

2. Build the tools into `build/bin/` (the build takes the assets out of the ROM set into `assets/` and
   packs them into `build/bin/orunners.assets`, which the game loads; no executable holds the ROM):

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

## The assets

`python extract_assets.py` takes the assets out of the ROM set into `assets/` (`--force` again, `--clean`
removes them, `--check` builds the chips back from them and compares); `assets.json` says what goes where,
in positions only, and `tile_palettes.json`, `sprite_palettes.json` and `sprite_sheets.txt` (positions and
record numbers too) say which palette each tile and sprite piece is shown with and how the pieces are
grouped into sheets. `tools/palette_census.py run DIR` and `resolve DIR` write those files again from the
scenarios. The formats:

- `assets/tiles/`: indexed PNG sheets of 1024 16x16 tiles, 16 palettes each (a pixel's index & 15 is its
  pen); a JSON beside each says every tile's palette.
- `assets/sprites/`: indexed PNG sheets, a family of objects (or a stretch of animations) each, a row an
  animation; 4-bit sheets hold up to 8 palettes, 32 entries each (the pens the board draws, then the same
  pens where it does not); a JSON beside each says where each piece goes back in the sprite ROM.
- `assets/sound/pcm_headers.txt` and `assets/sound/samples/`: the MultiPCM's 512 sample headers as text,
  and its samples as 8-bit WAV files, one byte a sample: `common.wav` and `bank_2.wav`-`bank_7.wav`, each
  with a JSON of the samples the headers start in it.
- `assets/data/` and `assets/sound/data/`: the program's and the sound program's data as text files, a
  section a piece (cut where `symbols.txt` names a place and where the C reads a table), and
  `layout.json`, the pieces in order; the code is left out (the C replaces it).

`python tools/make_pack.py` packs them for the game (`--check` compares what the pack puts in the board's
memory with the set, the code left out; `--layout` writes `runtime/src/pack_layout.c`, the pieces without
their bytes, which the first run fills from the player's set).

## Tools

`tools/core/` is [arcade-decomp](https://github.com/cheviman/arcade-decomp), the decompiler and analysis
tools OutRunners shares with [segasonic-decomp](https://github.com/cheviman/segasonic-decomp) (a git
submodule): the V60/V70 and Z80 decoders and lifters, the static analysis, the decompiler, the scaffold's
translator, the checks and the data and asset formats. `tools/game_profile.py` tells the core about
OutRunners (its ROM set, `orunners_rom.py`; its structures, the registers the program shares, its board),
and `tools/`'s scripts of the same names (`decompile.py`, `decompile_sound.py`, `recompile.py`,
`disassemble.py`, `check_structure.py`, `noise_report.py`, `coverage.py`) run the core's. A change to the
decompiler itself goes to arcade-decomp, checked on both games, and comes here as a newer commit of the
submodule (`git submodule update --remote tools/core`). The rest of
`tools/` is OutRunners' own: the ROM set, the scenarios' runner, the assets' census and sheets, the pack,
and the reference tools built on MAME's cores (`tools/reference/`).

## Legal

- This project is licensed under the BSD 3-Clause License; see `LICENSE`.
- OutRunners is a trademark of SEGA. This project is not affiliated with or endorsed by SEGA.
