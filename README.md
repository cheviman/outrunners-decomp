# OutRunners — decompilation

A reconstruction of Sega's 1992 arcade game *OutRunners* (Sega System Multi 32) into platform-agnostic C,
not an emulator, following the method of
[segasonic-decomp](https://github.com/cheviman/segasonic-decomp): the game's program decompiled one
function at a time, checked frame by frame against MAME in picture and sound.

**Work in progress.** This repository will contain no game data. As with
[n64decomp/sm64](https://github.com/n64decomp/sm64), you bring your own ROM set (MAME's `orunners`), and
the tools verify it, extract the assets and build your own executable.

## Status

How much of the C reads as C written by hand, measured on every run of `python tools/noise_report.py`
(`clean`): 589 of the 762 functions (77.3%) and 25,884 of their 26,883 lines of code (96.3%) hold nothing
of what the table under "What is left, and why" counts. Some of that the C has to keep to do what the
original does: R12 and R14 where a value goes from one function to another through them, several fields
moved in one access as the original's one instruction moves them, a byte or halfword of a value changed
with the rest of it kept, as the original's byte and halfword instructions change it, and the memory the
memory test, the checksum and the copy reach by its address, whatever is there (`MEMORY_ROUTINES` in
`tools/game_profile.py`, each with why). Without counting those (`faithful`), 759 functions (99.6%) and
26,879 lines of code (99.99%) are clean. The goal before the ports is all of both, in that measure: the 3
functions left read memory at an address worked out from a number the analysis cannot bound (4 lines,
`mem_access` in the table).

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
      registers whose last value the next function may read: globals named after them only where a value
      goes into or out of a function, and locals with names of their own elsewhere (716 uses of the
      globals, which `tools/noise_report.py` counts, 932 before; R27 and R28 are locals: 1,495 uses with
      them). The game's structures are typed (`tools/core/decompiler/pointers.py`, with `tools/game_profile.py`'s structures):
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
- [x] The assets (`extract_assets.py`, driven by `assets.json`, which holds positions only), each with a
      name, in formats the usual tools open: the tiles as 37 tilesets by subject (the road, each course's
      backgrounds, the route map, the title...: `tilesets.txt`), the sprites as 264 sheets by subject
      (`cars/<car>/`, `scenery/<course>/`, `hud/`, `traffic/`...: `sprite_sheets.txt`), the MultiPCM's
      samples as 219 WAV files by what plays them (`pcm_samples.txt`) with the 512 sample headers as text,
      the music and the sound effects as 324 MIDI files, and the program's and the sound program's tables
      as text records with their fields' names and addresses written as the names of what they reach.
      `python extract_assets.py --check` builds the 17 chips back from them, byte for byte (the code from
      the set)
- [x] The game is not anchored to the ROM: it loads those files themselves when it starts
      (`runtime/src/assets.c`, `assets_media.c`, `assets_midi.c`), every piece in a block of memory of its
      own, with no image of the ROM, no region the size of a chip, no mirrors, banks or interleaving. The C
      reads a table by its name (`game_mode_jumps[mode]`: a pointer the loader sets, `src/table.c`'s
      `game_tables`); an address the game keeps in its variables reaches its data through the assets'
      index (`data_at`); the video draws tiles from the tilesets by their numbers and sprites from the sheets
      by their places, the MultiPCM plays the WAV files, the sound program reads its tables and rebuilds its
      sequences from the MIDI files. What the loader reads is the ROM's data byte for byte (the program's
      818 pieces and the sound program's 396), and the 16 scenarios are identical in state, picture and
      sound. The reference and the translation scaffold read the ROM set when they start, as MAME does, and
      the memory test's ROM part is the board's (`runtime/replaced.txt`: the decompiled game has no ROM chips
      to sum)
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
      licenses). On Linux (Ubuntu 24.04 on WSL, GCC 13 and Clang 18) the 16 scenarios were identical to the
      reference in state, picture and sound (with the game on its earlier assets pack; not run again on
      Linux since it loads the asset files). The macOS build has not been run on a Mac yet
- [x] Toward the 99%: a jump through a table of code is a switch with the cases the table holds and no
      `default:`, as the original checks no index (`--checks` writes a build that stops on an index past
      the table, where the 16 scenarios run as well); R12 and R14 are locals wherever their value stays
      within a function; what every call passes and every way back hands back is followed bit by bit
      across functions (`knownbits.across`), so a byte every caller passes as a byte is a byte's sum; a
      function returns no more of its result than its callers read, and a register a function only hands
      back as it was is no argument its caller has to work out where it drops what comes back; the sound
      program puts no byte back into the pair it was taken from unchanged, and a pointer moved within its
      256-byte page reads `IN_PAGE(queue_pointer, write_index)`. Partial writes 439 -> 245, casts 2,856 ->
      2,796; `tools/check_structure.py` finds every function's control flow as the program's jumps
- [x] Toward the 99%, the second round: what a function reads to work out R12 and R14 is followed as what
      it hands back is, so a call that writes them no longer reads all its callee may read (`draw_object`'s
      R0); the bits known zero across functions start from all of them and are taken back until nothing
      changes, which finds a value that goes round between calls (`insert_coin`'s credits); a bit tested by
      a constant number reads that bit alone; what a function hands back through pointers is worked out
      only as wide as its callers read it; stack slots are followed a part at a time, as registers are (a
      register pushed whole and popped for its low byte). Memory by a number the code works out reads as the
      arrays it is: in the screen's block and the objects' area (`route_branches[forks_reached - 1]`,
      where the code counts from 1; `checkpoint_orders[checkpoint_offset * 4u + pass_player]`), the race
      records' copies by player (`RACE(RACE_COPY_1)[player].segment`), the road's ring of segments by a
      byte offset (`ROAD_SEGMENT(screen + 0x2000u + offset)->half_width`), the record after the one a
      pointer is in, and a field an `inc.b` reaches through an index (`row_steps[row_player]`). Partial
      writes 245 -> 180, memory reached by address 149 -> 82, casts 2,796 -> 2,749; the 16 scenarios
      identical in state, picture and sound, and `tools/check_structure.py` 0 problems
- [x] Toward the 99%, the third round: the data the code walks through one pointer at several widths reads
      as the bytes of a named table (`COURSE_BYTES(course_place)[0]`, `AS16(INPUT_TEST_BYTES(item)[0])`,
      the test mode's texts), and a fork's table of the road objects' streams as its own elements: no data
      is read through its ROM address any more (26 before). The objects' fields every family numbers
      alike have names of their own (`number`, `car`, `car_colour`: no field is named after its offset, 5
      before). What a search or a string move leaves in a register points where `symbols.txt` says
      (`OBJECT_SLOT_FLAGS(free_flag)[0]`, `LCD_CHARACTERS(text_end)[0]`); a register can point at a
      variable (the test mode's R20 at its settings: `time_difficulty[stage_item - 1u]`); the screens'
      pages of the sound RAM by the screen's index; a word written at an element of an array of halfwords
      is `AS32` of it (`AS32(video_registers[32 + index * 2u])`, not an address). A case of a switch that
      goes on to the start of another goes just before it and falls through into it, as C cases do
      (`lap_timer_update`, no control flag left). Memory reached by address 82 -> 53
- [x] Toward the 99%, the fourth round: a state machine's cases that go on into another case are written as
      C cases that fall through into it, stop where another case starts, leave by break and copy a small
      case they jump to (`song_title_show` has no control flag left, `player_car_move_view` 2 of 3,
      `car_radio_update` 5 of 7); the bits known zero go through what a function pushes and pops (the
      sound test's codes and the name entry's letters read as their tables, `sound_test_codes[sound_number]`).
      Memory reached by address 53 -> 48, partial writes 180 -> 174, casts 2,747 -> 2,732
- [x] Toward the 99%, the fifth round, the control flags: a state machine's cases inside a loop are written
      the same way, and a case that falls through into the next ones takes their code where it is copied
      (`checkpoint_board_update`); a jump into a state's case right after storing its number is the switch
      running again, `state = 0xFu; continue;`, where that writes the C shorter (`route_map_update`, the
      car select's attendant); a switch in a loop whose cases go round again or leave it ends with a
      break (`init_screen`'s course walk); where flags are still left, the head of a loop around another
      one, a small block several jump to and a larger piece of a loop entered at two places are copied
      (`player_car_move_view`; `palette_fade_step` 3 of 9); the station's DJ, which four states of the
      radio jump to, is a function of its own (`car_radio_talk`, a `func` line, out of `car_radio_update`).
      The functions named have no flag left but `palette_fade_step`. Control flags 15 functions and 59
      flags -> 10 and 38, ROM addresses printed as numbers 4 -> 0
- [x] Toward the 99%, the sixth round, the results written back: a function that hands results back
      through pointers is also written with one way out at its end where the short ways to it meet the
      others there, which the C keeps when it writes results back before fewer returns, or as few in
      fewer lines (`playlist_command`'s commands are cases that break, `vector_angles` an if and an else);
      a call whose result is the function's own as it ends (the original's jump on into the callee) gets
      the caller's pointer, `*p_leftover = x; draw_object_at_scale(..., p_leftover, ...);` (the seven
      `draw_object_*` routines); a copy of a function's code keeps the variables of the base a field
      access adds up (`ROAD_SEGMENT(screen + 0x2000u + (ring_offset & 0xFFFu))`), which kept the copies
      with one way out from being taken. Results written back before an early return 64 -> 13
- [x] Toward the 99%, the seventh round, control flags and memory by address: the ways of a case that goes
      on into the next one meet where that one starts (`race_background_update`, no flag left:
      `if (stage_background_change == 0u) { if (new_branch == 0u) break; new_branch = 0u; }`); the switch
      runs again for a state's case also where its block does something before it switches (split off), where
      the state is stored with the fields after it in one word and where other fields of the record are stored
      after it (`player_car_update`, 1 flag of 3). A pointer the code keeps on the stack keeps its type
      (`CAR_SELECT_TILEMAP_SETUP(setup_bits - 0x1Cu)->video_register_bits`, the road surface's line scroll,
      the sound program's voices and sequence players); a typed local stays a variable where its one use is
      a memory access (`ROAD_SEGMENT(car_segment_record)->half_width`, `TEXT_LAYER_CELLS(cursor_cell)[0]`);
      the road rows' last sprites are `SPRITE_WORDS` (the board's memories read the work RAM or the
      program's chips where the game points there, as the bus does); a device's place moved by a constant,
      or with its low byte replaced, is still the device (the link board's flag, the input test's ports).
      Control flags 10 functions and 38 flags -> 9 and 35, memory reached by address 48 -> 27
- [x] Toward the 99%, the eighth round, a part of one value put into another: a part a write keeps of a
      register that holds the same constant on every way into the block is that constant (`unpack`'s offset
      back, `0xFFFF0000u | AS16(PACKED_BYTES(source)[0])`; the test mode's minus sign, `0x642Du`); a
      parameter `symbols.txt` says points at a structure is a pointer where the function only puts an
      address together from it (`sequence_player = IN_PAGE(player, 0x1Bu)`); a pointer moved to another
      place of its 256-byte page says so with `IN_PAGE` (the input test's ports of an I/O chip,
      `r12 = IN_PAGE(port_a, 0xAu)`; the test mode's settings walked by their places). A small tail written
      again where it is jumped to does not take the one way back with it (`player_car_drive` writes its
      results once, at its end), and the objects' lists are cleared through a pointer to blocks of eight
      bytes (`OBJECT_LIST_BLOCK(list_block)->low`). The crash of the screen's car, state 5 of
      `player_car_update`, which a hit also runs into the frame it starts, is a function of its own
      (`player_car_crash_step`, a `func` line: `player_car_update` has no flag left). Mixed partial writes
      37 -> 28 lines, in 22 -> 16 functions; results written back before an early return 13 -> 9; memory
      reached by address 27 -> 25; control flags 9 functions and 35 flags -> 8 and 34
- [x] Toward all of it, the ninth round, the control flags: none is left. The values a register can hold
      where the code reads it are followed from the constants the functions and their callers set and from
      the program ROM's tables (`ranges.py`), so a bit tested by a register's number reads only the bits
      those numbers reach (the link board's self-test, 4 or 5 chips; the staff credits code's controls; the
      coin chutes' switches); a loop the code enters at more than two places gets a copy of the rest of the
      round those ways start (`palette_fade_step`: the scan for the first colour still fading goes on into
      the loop of steps at its red, green or blue); a loop whose ways out run through code of their own
      goes on where they meet; a test made again with nothing changed between goes straight to its side
      (`race_car_draw` tests the rival flag twice); a short way out several blocks of a loop lead to, a
      small loop of its own too, is written where each is (`road_surface_lines`' step to the next segment and
      its copy of the points left, so that the ways meet where it draws the line); and a function can go
      back to its caller at an exit (`exit` lines: `second_task_frames`, both screens' frames, returns when
      the test mode starts, as C cannot break out of two loops). Large pieces several ways run into are
      functions of their own (`func` lines): `player_car_engine_wheelspin`, `_grip`, `_standing`, `_revs`;
      `race_car_draw_behind`, `race_car_draw_link`; `name_entry_read_wheel`, `name_entry_choose`,
      `name_entry_finish`, `name_entry_store_record`, `name_entry_draw_time`; and in the sound program,
      `cut_open_hi_hat`, `take_voice` and `release_note` for each screen. Control flags 8 functions and 34
      flags -> 0; results written back before an early return 9 -> 0; mixed partial writes 28 -> 23 lines, in
      16 -> 12 functions
- [x] Toward all of it, the tenth round, the mixed partial writes: none is left. A partial write whose kept
      part something reads afterwards changes a part of the one value the register holds, so the C writes it
      in place, on one variable: `random_number`'s state and the number it hands back, the turntable's
      leftover with the flags in its low byte, the riders' gestures `race_car_draw` puts in R12's low byte,
      the memory test's results in the low byte of its ROM check, `road_surface_halfway_lines`' stripe count
      with the last frame's segment in its low half, the sound program's register pairs (a count in B, a
      register in C). Not where the code reads memory through one of them: a pointer moved within its page is
      a pointer of its own (`IN_PAGE`, the input test's ports). A value goes into the rest a partial write
      keeps, or a partial write into what reads that rest too, only where the part written or the rest is
      zero, so each of the original's steps where both carry something is a line of its own (the A/D
      converter's bits shifted in one at a time, `read_switches_and_analog`; the EEPROM's, `eeprom_read_word`;
      a pair of x turned round, then its low half written, `road_row_pair_draw`). And a call passes only the
      parts of a register its callee reads: `multiply_bytes` clears the low byte of its value first thing, so
      its callers put the level in the high byte alone (`level_product = (uint32_t)note_level << 8`). Mixed
      partial writes 23 -> 0 lines, in 12 -> 0 functions; partial writes 150 -> 148; R12 and R14's uses
      717 -> 720 (the riders' gestures, in R12 as the original keeps them)

## What is left, and why

What the C still says the way the original's instructions do, rather than in the game's own terms. Each of
these does exactly what the original does; the counts come from `python tools/noise_report.py` (its line
names in brackets) or, for the ones it does not count, from the C as it is now.

| What | Count | Why |
|---|---|---|
| R12 and R14 named after the register (`context_registers`) | 720 uses | Where a value stays within a function (worked out and used up before it ends, across calls that do not touch the register), it is a local with a name of its own (`symbols.txt`'s local lines); the global is what the function finds in the register, what a call leaves there or reads, and what is left at its end (932 uses before). A byte the code puts in such a register and keeps the rest of is that global with a part changed: the riders' gestures, which `race_car_draw` writes into R12's low byte twice (3 uses, a local of their own with R12's rest in it before the tenth round). Scratch registers whose last value the next handler reads: `race_cars_start` puts the car's address together from R12's high half, which the handler before it left (the screen's page), and the road rows' and the scenery's drawing keep the height of the last size step in R14 and skip reloading it when the object's step is the same. The C keeps them as globals, a copy for each task. Locals with that value passed from handler to handler through a variable of their own would take some 600 copies at the handlers' starts and ends (measured: 146 handlers read R12 as they start, 163 leave it), more than the uses they would take away. R27 and R28 are locals: the one function that would read them before setting them, `player_car_project_road`, does so only if its projection pattern skipped every segment, and it starts with a 1 (`symbols.txt`'s `unread` lines) |
| Memory reached through a computed address (`mem_access`) | 25 | The routines that work on memory by its address, whatever is there (`MEMORY_ROUTINES` in `tools/game_profile.py`, faithful): the memory test (6), `unrolled_copy`, a copy from any address to any other (8), what boot writes where the board maps nothing (E00000, E80000: 6), the checksum of the settings (1). And 4 more, one by one: the car numbers one before the array by a count that goes down (`car_select_entries_run`, 2: from the players' count, which the analysis cannot bound), the road's page table read by page sets counted from 3 (`road_surface_update`: from 24 bytes before the object, 1), a table read by a signed number (the body's frame by the steering, 1: the data it reads around its address is another piece of the assets); 149 before the second round, 48 before the seventh |
| Several fields read or written at once, `AS16`/`AS32` (`wide_access`) | 206 | The original moves them in one instruction: the sprite lists' halfwords two at a time (54: a list's jump, and its copy in the other half of the list 0x3000 bytes on, `SPRITE_WORDS(p)[6144]`), the screen's block (69: the LCD's 16 characters four at a time, `AS32(SCREEN(SCREEN_1)->lcd_text[4])`; the game mode with its steps, the memory test's results two at a time...), palette colours 64 bits at a time (16), a halfword or a word of the data the code walks a byte at a time (the input test's list, the course's records, the test mode's texts: 13), two text cells at once (7), objects' fields (5), the projected segments copied whole (4), the car's brake lights (4), and 34 more (two halfword video registers at once, the unpacker's halfwords, a course record's model and name, the seats' bookkeeping bytes...) |
| Jumps through a table with an index it does not have (`invalid_jump`) | 0 | The original jumps through its tables of code without checking the index, and so does the C: its switch has the cases the table holds and no `default:` (191 before). `python tools/decompile.py --checks` (and `decompile_sound.py --checks`) writes the C for a build where an index the table does not hold stops the game with the address the original would have jumped to (`invalid_jump`, 192 places); the 16 scenarios are identical in that build too. Three functions end in such a switch whose every case returns, which compilers warn about (MSVC's C4715) |
| Results written back before an early return (`copy_backs`) | 0 | A function hands back what it leaves in registers through pointers (`p_leftover`): kept in a local, it is written back as the function ends, once where the ways out meet at the end (`vector_angles`), straight through the pointer where the function only passes it on, and before the call that hands it on (`*p_leftover = x; draw_object_at_scale(..., p_leftover, ...);`). None left: `road_surface_lines`' 9, at three returns out of the loops its control flags left, went with them in the ninth round (64 before the sixth round) |
| Functions with `bool` control flags (`control_variables`) | 0 | Where the original's jumps join paths that no nesting of ifs and loops can, a flag would say which way to go instead of a goto (59 functions had them before the sixth round). None is left: the decompiler writes what C can say: a small block several jump to written for each, a jump into a state's case right after storing its number written as the switch running again (`state = 3u; continue;`), a larger piece of a loop entered at two places copied for the second way in, a large piece several states jump to is a function of its own (`car_radio_talk`, `player_car_crash_step` and the ninth round's: `func` lines), a loop entered at more than two places gets a copy of the rest of the round those ways start (`palette_fade_step`), a test made again with nothing changed between goes straight to its side (`race_car_draw`), a short way out several blocks of a loop lead to is written where each is (`road_surface_lines`), and code two nested loops both leave to is where a function of their own goes back to its caller (`second_task_frames`, an `exit` line) |
| Partial writes of a variable, `(x & ~0xFFFFu) \| ...` (`partial_writes`) | 148 | Each changes a part of a value and keeps the rest of the same value, as the original's byte and halfword instructions do (`colour_count`'s `inc.b`, `palette = (palette & ~0xFFu) \| (uint8_t)(palette + 1u)`; a pointer moved within its 64 KB page), which the faithful measure takes as the original's. None puts a part of one value into another (`mixed_partial_writes`: 23 lines, in 12 functions, before the tenth round): where something reads the rest a write keeps, the write changes a part of the one value the register holds, and the C writes it on that one variable, a step a line where the part written and the rest both carry something: two halves the code keeps in one register and turns round (the random numbers' state, `random_number`; a pair of x in `road_row_pair_draw`), bits shifted in one at a time (the A/D converter's, `read_switches_and_analog`; the EEPROM's, `eeprom_read_word`), a cell's attribute and character (`test_mode_bookkeeping_draw_histogram`), the Z80's register pair BC with a count in B and a register in C (`set_voice_modulation_screen_1` and `_2`), a result the code puts in the low byte of what a call left (the turntable's leftover and the item's flags, the memory test's results), the riders' gestures in R12 (`race_car_draw`), the last frame's segment in the low half of `road_surface_halfway_lines`' stripe count. A call passes only the parts of a register its callee reads, so what a caller would put in a part the callee clears first goes (`multiply_bytes`' callers: `level_product = (uint32_t)note_level << 8`). A bit tested by a register's number reads only the bits the numbers that register can hold reach (`ranges.py`: the link board's self-test, the staff credits code's controls, the coin chutes' switches from their table in the ROM), so a byte read into the register before such a test keeps nothing of what was there. The V70 writes the low byte or halfword of a 32-bit register and keeps the rest; the C does the same where the rest is read later: a pair packed in one register (`draw_text_at`'s position, x in the low halfword and y in the high one), halfword sums on a register whose high half goes on into a product or a store, a result the callers' variables take whole through a pointer. What every call passes and every way back hands back is followed bit by bit across functions, a function returns only what its callers read of its result, and a register a function only hands back is worked out only where its caller reads it afterwards, so those need none (439 before). A pointer moved within its 64 KB page (the next entry of a list, a link the code reads: `chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail)`) says so with `LINKED`, as the objects' links do (82), and one within its 256-byte page with `IN_PAGE` (the sound program's MIDI queue's next byte, a sequence player's field; the game's ports of an I/O chip) |
| Data read through its ROM address (`rom_reads`) | 0 | The course's stream, the input test's list and the test mode's texts read as the bytes of their tables, a halfword or a word of them with `AS16`/`AS32`; a fork's table of the road objects' streams as its own elements (26 before) |
| Tables read by their address (`rom_table_reads`) | 0 | Both programs read their tables by name: a pointer each that the loader sets (`src/table.c`'s `game_tables`, `src/sound/tables.c`'s `sound_tables`); 847 before |
| ROM addresses printed as numbers (`raw_rom_addresses`) | 0 | Every ROM address the code uses as a value has a name; the last four were `car_radio_update`'s switch going on at its own jump targets, which now runs the switch again or calls `car_radio_talk` |
| Fields named after their offset (`unk_fields`) | 0 | The objects' fields 09, 0A and 0B that the code that starts an object sets alike for any family: `number` (its place in its start list, a choice flag's player), `car`, `car_colour` (5 before) |
| A function the board puts its own in place of (`board_replaced`) | 1 | The memory test's sums of the ROM chips (`runtime/replaced.txt`): the game has no ROM chips to sum, so the board hands back the sums the program keeps for them (at 03FFF8 and 03FFFC), as good chips give |
| Work RAM addresses printed as numbers | 29 uses | The tasks' stacks and frames (`0x21F800u`, `0x21FF84u`), places in the screens' blocks (screen 2's car `0x219000u`, the sprite places `0x20C000u`), `car_numbers` less one and the coin meters after the first: the decompiler names a constant only when it is a variable's own address |
| Unused names in `src/ram.h` | 50 | Places of the screens' blocks and the race records that the code reaches by their own address, which the C reads as fields, and two places of the ROM (`rom_000001`, `rom_000002`): each named after its address (`unk_208001`), and nothing uses the names |
| Data without a name of its own | 186 pieces, 1 to 6 bytes | The `data_XXXXXX` sections of `assets/data/`: the alignment between the code and the tables (zeros), the V60's `nop`s at 000000-000005, and dead code bytes in two places (00E8D9, after `halt`s) that the code reaches by no way. Every table has a name, and every piece with a structure a format of its own |
| Board registers without a name | 6 uses | The interrupt controller's register 5 (`IRQ_D00005`, which MAME does not know either) and the bytes boot writes to E00000 and E80000-E80003, where the board maps nothing |
| Calls through a slot that holds a function's address | 11 wrappers | `src/table.c`: how `call_code()` runs a function from a code address it reads (`call_0006B0` runs `second_task`), named after that address |

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

2. Build the tools into `build/bin/` (the build takes the assets out of the ROM set into `assets/`, which
   the game loads from the repository's folder or from beside its executable; no executable holds the
   ROM):

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

   The game keeps its EEPROM (the settings, `orunners.eeprom`) and the part of the sound board's RAM it
   keeps on the battery (the bookkeeping and the ten course records, `orunners.bookkeeping`) beside the
   executable on Windows, and on Linux and macOS in the user's folder for it (`~/.local/share/OutRunners`,
   `~/Library/Application Support/OutRunners`), as the executable's may not be written. It loads its
   assets from the folder `assets` beside the executable (beside the .app on macOS), in that folder of the
   user's, or in the current one (`--assets FOLDER` picks another). The packages the CI makes hold the
   window port alone: until the game can take the assets out of a ROM set itself, the folder comes from
   `extract_assets.py`.

## The assets

`python extract_assets.py` takes the assets out of the ROM set into `assets/` (`--force` again, `--clean`
removes them, `--check` builds the chips back from them and compares, `--only TYPE` takes one type of
asset); `assets.json` says what goes where, in positions only, and the files beside it say how the assets
are cut and named, in positions, numbers and names only: `tilesets.txt` (the tile numbers of each
tileset), `sprite_sheets.txt` (the piece records of each sheet and row), `pcm_samples.txt` (where each
sample is), `tile_palettes.json` and `sprite_palettes.json` (the palettes they are drawn with, from a census
of the scenarios: `tools/palette_census.py run DIR` and `resolve DIR`), `read_crossings.txt` (the names
inside a table the original reads across, `tools/read_crossings.py` from the reference's census of its data
reads). The game reads the assets themselves, so each folder carries what it needs to (a list, an index,
the names its texts use). The formats:

- `assets/tilesets/`: a tileset a subject (`road.png`, `courses/japan.png`, `scenes/route_map.png`),
  8-bit indexed PNGs of 32 16x16 tiles a row, in the 16 palettes the tileset's tiles are shown with the
  most (a pixel's index & 15 is its pen); `tilesets.txt` beside them says which tile numbers each holds.
- `assets/sprites/`: a sheet a subject, a row an animation; 4-bit sheets hold up to 8 palettes, 32 entries
  each (the pens the board draws, then the same pens where it does not); a JSON beside each says where
  each piece is and where it goes in the sprite chips; `sheets.txt` lists them.
- `assets/sound/samples/`: the MultiPCM's samples as 8-bit WAV files by what plays them (`engine/`,
  `tyres/`, `speech/`, `drums/<song>/`, `instruments/<song>/`, `effects/`...), with a `smpl` loop where the
  sample loops; `headers.txt`, the 512 sample headers with their fields by name and the sample each one
  plays in each sound set; `samples.txt`, where each sample is.
- `assets/sound/data/`: the music (`music/`), the effects (`effects/`), the DJ's voices (`voices/`) and the
  sound sets' commands (`controls/`) as Standard MIDI Files, a tick a tick of the sound program
  (`tools/formats/sound_music.py` says how a sequence becomes a file and back); the sound program's
  tables as text, by subject (`chips.txt`, `tuning.txt`, `instruments.txt`, `engine.txt`, `commands.txt`).
- `assets/data/`: the program's data as text files by subject, the C's subjects
  (`assets/data/race/course.txt`): a table goes with the C file that reads it the most. A file has a
  section a piece (cut where `symbols.txt` names a place and where the C reads a table; a name inside a
  table is no cut, `layout.json`'s `inside`), and `layout.json` has the pieces in order; the code is left
  out (the C replaces it).

The tables are text in one format (`tools/core/formats/typed.py`), which the game's loader reads as it is:
a `layout` line names the columns (`layout road_row_record 24 animation:u16 palette:u16 flags:u8 ...`),
then a record a line; `record` lines give the kinds of the records a list mixes, each line starting with
its kind's name. Values: numbers (hex unsigned, decimal signed), addresses as the names of what they reach
(`game_mode_title`, `isdn_total_text+0x4`), texts in quotes, colours as `#RRGGBB`, a value split in two as
`HIGH:LOW` (an FM channel's block and frequency number, `4:1EE`). `names.txt` beside the data gives the
names' addresses.

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
censuses, cuts and formats (`tilesets.py`, `pcm_samples.py`, `sprite_sheets.py`, `palette_census.py`,
`read_crossings.py`, `formats/`).

## Legal

- This project is licensed under the BSD 3-Clause License; see `LICENSE`.
- The board's runtime (`runtime/`) follows the behavior of MAME's System 32 and Multi 32 driver, its sound
  chips and ymfm's YM3438, BSD 3-Clause, whose terms are in `tools/core/system32/reference/mame/LICENSE`
  and `ymfm/LICENSE` beside it.
- The window port links SDL2 (zlib License); the packages the CI makes carry it inside the executable, with
  its terms and the ones above in `licenses/`.
- OutRunners is a trademark of SEGA. This project is not affiliated with or endorsed by SEGA.
