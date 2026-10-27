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
      `current_object`, `player_car` and `screen` (named by `symbols.txt`), and R12 and R14, scratch
      registers whose last value the next function may read, still named after them (932 uses, which
      `tools/noise_report.py` counts; R27 and R28 are locals: 1,495 uses with them). The game's structures are typed (`tools/core/decompiler/pointers.py`, with `tools/game_profile.py`'s structures):
      the objects (0x100 bytes, 124 in the page of each screen's block), the screen's car (0x200 bytes at
      1000 of the block), the course's ring of segments (128 of 0x20 bytes from 2000 of the block) and the
      block of the screen a task runs (`SCREEN_1`, `SCREEN_2`), read as `OBJECT(p)->field`,
      `CAR(p)->field`, `ROAD_SEGMENT(p)->field` and `SCREEN(p)->field`, each player's race record
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
      (`model_then_animation`). Every variable has a name, and every field but 11 uses of fields still
      named after their offset (`tools/noise_report.py`, unk_fields; 79 before): the bytes of two
      families' arrays, which their start clears as a word, a halfword and two bytes (the car radio's
      `car_models`, the checkpoint board's `row_steps`: a family's field cannot be an array yet), and
      fields of objects of two families (the choice flags `car_select_entries_run` starts) or of any
      (`start_object_list`'s number in its list)
- [x] The memory the code reaches through pointers, typed: a record of the work RAM, of the program ROM
      or of the board's plain memories (the video RAM, the sprite RAM, the RAM the sound program shares) a
      local, a parameter or a field points at is a structure, so `mem_read16(projected_segment + 4u)` reads
      `PROJECTED_SEGMENT(projected_segment)->x` (each screen's ring of views and the road as it projects it,
      the sprites' depth lists and the sprite lists' entries, the text layer's cells, the cars' hit boxes,
      riders and sprays, the crash tables, the courses' starts, the coin chutes, the seats' credits and
      inputs, the lamps, the settings' copies, the bookkeeping and the course records the sound program's RAM
      keeps, the palettes' and the packed tiles' lists, the text records); a pointer that walks values of one
      width reads `NAME(p)[i]` (`OBJECT_WORDS(object_bytes)[1] = 0u`), an array inside a record by a number
      the code works out reads `SCREEN(screen)->places[player]`, and an unrolled copy or clear is the loop
      it does. The board's registers have names: a variable in its plain memories (`MACHINE_ID_TEXT_CELL`,
      `NBG0_SCROLL_X[layer]`), a device's register read and written by name or by the address the code
      works out, through the bus (`io_read8(IO_1_CNT)`, `io_write16(board_records, ...)`: 371,
      `board_registers`, not noise). 150 reads and writes still go through a computed address
      (`tools/noise_report.py`, mem_access; 2,877 before this was typed): see "What is left, and why".
      `tools/memory_census.py` says what each one reaches over the scenarios, with the game built with
      `ORUNNERS_MEMORY_CENSUS`
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
      where the code's address stands for "go on here", as the cases do. The data's text files go by
      subject, as the C does: a table goes with the C file that reads it the most
      (`assets/data/race/course.txt` holds the courses `src/race/course.c` builds the road from)
- [x] The C by subject, as the original's modules lay it out (`symbols.txt`'s file lines, a file a subject,
      some taking several stretches of the program): `src/system/` (the main program, the settings, the
      inputs, coins and bookkeeping, sound requests, the LCD, the interrupts, the test mode), `src/video/`
      (the chips, unpacking, palettes, text), `src/engine/` (the objects and their sprites), `src/race/`
      (the player's car, its physics, the course, the other cars, the hits, the race's display, the link
      between cabinets), `src/objects/` (the scenery, the traffic, the race's people, the road's surface,
      the backgrounds, the car radio), `src/scenes/` (the game modes, the attract mode, the title, the coin
      prompts, the car select, the ranking, the name entry, the route map) and `src/sound/` (the sound
      program: its main loop, the MIDI messages, the FM voices, the engine noise, the sequences), each
      file with its header and what it holds
- [x] Linux and macOS: the game builds with GCC and Clang as well as MSVC, and GitHub Actions builds it on
      every push for Linux, macOS (an .app for Apple silicon and Intel) and Windows, with no ROM set and SDL2
      inside the window port (`.github/workflows/build.yml`: a package each, the window port with its
      licenses). On Linux (Ubuntu 24.04 on WSL, GCC 13 and Clang 18) the 16 scenarios are identical to the
      reference in state, picture and sound, and the first run makes the same pack, byte for byte, from the
      `rom/` folder and from the .zip. The macOS build has not been run on a Mac yet

## What is left, and why

What the C still says the way the original's instructions do, rather than in the game's own terms. Each of
these does exactly what the original does; the counts come from `python tools/noise_report.py` (its line
names in brackets) or, for the ones it does not count, from the C as it is now.

| What | Count | Why |
|---|---|---|
| R12 and R14 named after the register (`context_registers`) | 932 uses | Scratch registers whose last value the next handler reads: `race_cars_start` puts the car's address together from R12's high half, which the handler before it left (the screen's page), and the road rows' and the scenery's drawing keep the height of the last size step in R14 and skip reloading it when the object's step is the same. The C keeps them as globals, a copy for each task. Locals with that value passed from handler to handler through a variable of their own would take some 600 copies at the handlers' starts and ends (measured: 146 handlers read R12 as they start, 163 leave it), more than the uses they would take away. R27 and R28 are locals: the one function that would read them before setting them, `player_car_project_road`, does so only if its projection pattern skipped every segment, and it starts with a 1 (`symbols.txt`'s `unread` lines) |
| Memory reached through a computed address (`mem_access`) | 150 | An element picked by a number through a pointer the code works out first and keeps (`checkpoint_board_update`'s rows: `inc.b` through a place it worked out), or a table indexed by two numbers (the checkpoints' orders, by checkpoint and by player): 36. One pointer that reaches two memories or two layouts: the road rows' last sprites (the ROM's blank places or the work RAM's ring), the memory test, the CRC of the settings, the test mode's texts (halfwords and bytes in one walk): 24. Code nothing calls, so nothing says what its pointers reach: `unrolled_copy`'s loop (a copy from any address to any other), two of the test mode's: 15. A place one before an array or past the record a pointer is in (`screen + 0x349u + forks_reached`, the next projected segment): 14. What a string move or a search leaves in a register: 10. The road's ring of segments by a byte offset: 8. The race records' copies by the player's number (`0x20FC42u + (player << 7)`): 7. The addresses boot writes where the board maps nothing (E00000, E80000): 6. Stack slots: 5. And 25 more, one by one (a table read by an index the analysis cannot bound, the slots below a screen's block, the test mode's settings through R20 plus an offset) |
| Several fields read or written at once, `AS16`/`AS32` (`wide_access`) | 145 | The original moves them in one instruction: the sprite lists' halfwords two at a time (54: a list's jump, and its copy in the other half of the list 0x3000 bytes on, `SPRITE_WORDS(p)[6144]`), the screen's block (35: the game mode with its steps, the LCD's text four characters at a time, the memory test's results two at a time...), palette colours 64 bits at a time (16), two text cells at once (7), objects' fields (5), the projected segments copied whole (4), the car's brake lights (4), and 10 more (the unpacker's halfwords, a course record's model and name, the seats' bookkeeping bytes...) |
| Jumps through a table with an index it does not have (`invalid_jump`) | 193 | The original jumps through its tables of code without checking the index; the C's switch has a `default:` for the indexes the table does not hold, where `invalid_jump` stops the game with the address the original would have jumped to |
| Results written back before an early return (`copy_backs`) | 40 | A function hands back what it leaves in registers through pointers (`p_leftover`); where a caller may pass one variable for two of them, writing straight through the pointer would not be the same, so the result is kept in a local and written back at each way out |
| Functions with `bool` control flags (`control_variables`) | 17 functions, 45 flags | Where the original's jumps join paths that no nesting of ifs and loops can, a flag says which way to go instead of a goto; each is named after the code it leads to (`L_017685`, 478 uses) |
| Partial writes of a variable, `(x & ~0xFFFFu) \| ...` (`partial_writes`) | 502 | The V70 writes the low byte or halfword of a 32-bit register and keeps the rest; the C does the same where the rest is read later |
| Data read through its ROM address (`rom_reads`) | 26 | The course's stream, whose commands have operands of different widths behind one pointer (16), the input test's list, which one pointer walks through two layouts (9), and the road objects' stream after a fork (1) |
| Tables read over the pack's copy of the data (`rom_table_reads`) | 847 | The tables are in the assets pack at their own addresses, which the code keeps in RAM and in other tables, so a table is read by its name over that copy (`src/ram.h`: `rom(...)`) |
| ROM addresses printed as numbers (`raw_rom_addresses`) | 2 | The switches that go on through the original's own jump target in `car_radio_update` and `song_title_show`, where the address stands for "go on here", as the cases do |
| Fields named after their offset (`unk_fields`) | 5 | Fields of objects of two families (`car_select_entries_run`'s choice flags, `coin_prompt_update`'s) or of any (`start_object_list`'s number in its list) |
| A function the board puts its own in place of (`board_replaced`) | 1 | The memory test's sums of the ROM chips (`runtime/replaced.txt`): the game has no ROM chips to sum, so the board hands back the sums the program keeps for them (at 03FFF8 and 03FFFC), as good chips give |
| Work RAM addresses printed as numbers | 37 uses | The tasks' stacks and frames (`0x21F800u`, `0x21FF84u`), places in the screens' blocks (screen 2's car `0x219000u`, the sprite places `0x20C000u`), the race copies' fields by player (`0x20FC42u + (player << 7)`), `car_numbers` less one and the coin meters after the first: the decompiler names a constant only when it is a variable's own address |
| Unused names in `src/ram.h` | 50 | Places of the screens' blocks and the race records that the code reaches by their own address, which the C reads as fields, and two places of the ROM (`rom_000001`, `rom_000002`): each named after its address (`unk_208001`), and nothing uses the names |
| Data without a name of its own | 7 named after their address, 193 stretches | Unused tables named after their address (`unk_00183B` and six more in `symbols.txt`), and the `data_XXXXXX` sections of `assets/data/`, the stretches between the named tables |
| Board registers without a name | 6 uses | The interrupt controller's register 5 (`IRQ_D00005`, which MAME does not know either) and the bytes boot writes to E00000 and E80000-E80003, where the board maps nothing |
| Calls through a slot that holds a function's address | 14 wrappers | `src/table.c`: how `call_code()` runs a function from a code address it reads (`call_0006B0` runs `second_task`), named after that address |

## Getting started

Requirements: Python 3.10 or newer, CMake 3.20 or newer and a C11 / C++17 compiler (Visual Studio Build
Tools on Windows, GCC or Clang on Linux, Xcode's command line tools on macOS). The window port needs SDL2
2.30 or newer: on Windows its development package for Visual C++ (`SDL2-devel-2.32.10-VC.zip`) unpacked in
`build/downloads/sdl2/`, elsewhere the system's (`libsdl2-dev` on Debian and Ubuntu); or, with
`-DORUNNERS_STATIC_SDL2=ON`, SDL2 2.32.10 built from its source into the executable (CMake fetches it from
GitHub), as the packages are. On Linux and macOS the commands below run with `python3`.

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
   the triggers the pedals, the shoulders the gears. `--script FILE` plays a control script instead,
   from a blank EEPROM it does not keep (`--fast` without waiting for the frames' time), and `--hashes`,
   `--video-hashes` and `--sound-hashes` write the same hashes as the reference.

   The game keeps its EEPROM (the settings and the bookkeeping, `orunners.eeprom`) beside the executable
   on Windows, and on Linux and macOS in the user's folder for it (`~/.local/share/OutRunners`,
   `~/Library/Application Support/OutRunners`), as the executable's may not be written. It loads
   `orunners.assets` from beside the executable or from that folder; with neither, its first run makes
   the pack in that folder from your ROM set, `rom/` or `orunners.zip`, beside the executable (beside the
   .app on macOS) or in that folder. The packages the CI makes hold the window port alone, so that is
   how a player's first run goes.

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
- `assets/data/` and `assets/sound/data/`: the program's and the sound program's data as text files by
  subject, the C's subjects (`assets/data/race/course.txt`, `assets/sound/data/voices.txt`): a table goes
  with the C file that reads it the most, a piece no C file reads with the code it lies in, and one past
  the code with the piece before it. A file has a section a piece (cut where `symbols.txt` names a place
  and where the C reads a table), and `layout.json` has the pieces in order; the code is left out (the C
  replaces it).

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
`disassemble.py`, `check_structure.py`, `noise_report.py`, `coverage.py`, `check_scenarios.py`,
`verify_rom.py`, `render_state.py`, `memory_census.py`) run the core's. A change to the decompiler or those
tools goes to arcade-decomp, checked on OutRunners, and comes here as a newer commit of the submodule
(`git submodule update --remote tools/core`). The reference the scenarios compare the game with is the core's
too (`tools/core/system32/reference/`: `s32ref` on MAME's cores, the disassemblers' oracles, `pcmcheck`),
which CMake builds through its `reference.cmake` with OutRunners' data library. `memory_census.py run`
counts what each mem_read and mem_write of the C reaches over the scenarios, on the headless game built
with `ORUNNERS_MEMORY_CENSUS=ON` into `build/bin-census` (`ORUNNERS_BIN_DIR`); `report` sums it into
`build/census/memory.txt`. The rest of `tools/` is
OutRunners' own: the ROM set (`orunners_rom.py`), the scenarios' routes (`make_routes.py`), the assets'
census and sheets, and the pack.

## Legal

- This project is licensed under the BSD 3-Clause License; see `LICENSE`.
- The board's runtime (`runtime/`) follows the behavior of MAME's System 32 and Multi 32 driver, its sound
  chips and ymfm's YM3438, BSD 3-Clause, whose terms are in `tools/core/system32/reference/mame/LICENSE`
  and `ymfm/LICENSE` beside it.
- The window port links SDL2 (zlib License); the packages the CI makes carry it inside the executable, with
  its terms and the ones above in `licenses/`.
- OutRunners is a trademark of SEGA. This project is not affiliated with or endorsed by SEGA.
