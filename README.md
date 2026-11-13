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
(`clean`): 642 of the 762 functions (84.3%) and 26,725 of their 27,149 lines of code (98.4%) hold nothing
of what the table under "What is left, and why" counts. Some of that the C has to keep to do what the
original does: R12 and R14 where a value goes from one object's handler to the next through them, several fields
moved in one access as the original's one instruction moves them, a byte or halfword of a value changed
with the rest of it kept, as the original's byte and halfword instructions change it, and the memory the
memory test, the checksum and the copy reach by its address, whatever is there (`MEMORY_ROUTINES` in
`tools/game_profile.py`, each with why). Without counting those (`faithful`), all 762 functions and all
27,149 lines of code are clean, the goal before the ports.

Apart from all that, the C keeps the game's state where the original had it, laid out as the board's work RAM (one structure, `struct game`, which the code reaches through `game`): structures reached by casting an address, addresses of the work RAM, of code and of tables used as values, links within a 64 KB page, memory moved and filled through its address (the memory model's row of the table under "What is left, and why"). 255 of the game's 696 functions (36.6%) and 20,821 of their 23,256 lines of code (89.5%) touch none of it; taking the rest out comes once the C no longer leans on the processor it runs on (the last entries of the list below).

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
- [x] All of it, the eleventh round, the last memory reached by address outside the memory routines: an
      index the code counts (a register whose every value is a byte or a halfword, or itself counted on by a
      constant) reads its table by name, `car_numbers[players_left - 1u]` as the car select counts the
      players down to 1 (and the sound program's channel controls, by the channel's offset); an array of a
      structure read from so many of its elements before its start as one term of the index counts in is
      that term counted from more than 0, `page_table[(page_set - 3u) * 5u + (near_scroll >> 9)]` (the road's
      page table, in `symbols.txt` the 15 words it is: five for each of page sets 3 to 5); and a table read
      from its middle by a signed number is that table from its real start, `steer_frames_by_steering[64 +
      steering]` (129 bytes from 00927C, a table's count in `symbols.txt` as a variable's: `table
      NAME[COUNT]`), which the assets cut there too: assets taken out before it are cut another way, and the
      game stops when it loads them (`the assets have no table steer_frames_by_steering`; `python
      extract_assets.py --only program_data --force` takes the data out again). The program's data in
      `assets/data` take the file of the C that reads them, the loaders' lists (`src/table.c`,
      `src/sound/tables.c`) left out: they name every table once, and had put 24 of them in a `table.txt` of
      their own. Memory reached by address 25 ->
      21, the memory routines' alone; faithful 759 -> 762 functions and 26,879 -> 26,883 lines: all of it
- [x] After it, the small things: the names the decompiler made up for memory that its accesses turned out
      to be something else (44 variables `unk_` in the screens' blocks, the tables `rom_000001` and
      `rom_000002`) are no longer declared, nor loaded; a function that returns a value and ends in a switch
      whose every case returns says that nothing goes on past it (`unreachable()`), so no compiler warns;
      and a place so many bytes on from a pointer in its 64 KB page, added in 16 bits as the original does, is
      `PAGE_ADD` (the ring of the road's segments: partial writes 148 -> 140)
- [x] The twelfth round, R12 and R14 as global variables only where they have to be. What a function leaves
      in them is live where it ends as far as its callers read it, and where a boundary function ends (an
      object's handler, an interrupt, a task: the code that runs next reads what it leaves); a part of the
      global a function only takes through stays as it was. In the functions that neither call through a
      memory slot, nor wait for an interrupt, nor only take a value through to the next handler, nor call
      one of those, they are registers like the rest: parameters and results named in `symbols.txt` (61
      `param` lines: the drawing's `last_piece` and `last_size_step`, the road's `select_place` and
      `stripe`, the EEPROM's `eeprom_port`...), and locals (`first_segment`, `handling`, `next_segment`).
      R12 and R14's uses 720 -> 347, in 104 -> 74 functions; partial writes 140 -> 132; clean 592 -> 601
      functions
- [x] The board's memory model counted (`python tools/noise_report.py`): the structures the code reaches by
      casting an address of the work RAM, the variables at fixed addresses, the addresses of the work RAM,
      of code and of tables used as values, the places worked out within a page, and the memory moved,
      filled or searched through its address, which nothing counted before (106: `move_halfwords`,
      `move_string`, `set_bytes`...), with how much of the game's C touches none of it. No name made up of an
      address or an offset is left: 17 functions' comments gave fields by their offset, from before the
      fields had names (`the car object's unk_10`, its `timer`), `struct object`'s view of every object
      declared 64 such fields that no code reads (now padding), and one place of screen 2's block went by a
      name made up of its address that the meter did not see (`made_up_names` 86 -> 0)
- [x] The game's state as one structure, the first step out of the board's memory model: `struct game`
      (`src/ram.h`) is the work RAM laid out as the original had it, its blocks by name (`object_area_1`,
      `screen_1`, `races`, `race_copies`, `object_area_2`, `screen_2`, `isdn_races`, `stacks`) and its
      variables at their places, the names that share bytes in a union (`checkpoint_lamp[8]` and the names
      of its bytes: each screen's lamp a record of four). The code reaches it through `game`
      (`game->game_frame_count++`); the runtime keeps one world's (`runtime/src/game.c`) and the board
      reads the same bytes, so each frame's state still compares byte for byte with the original's. The
      arrays the code indexes have their counts in `symbols.txt` (`course_records[60]`, the ten records of
      six bytes), so that no index runs past one, and a screen's block is the 0x6000 bytes its fields take
      (the game's variables follow screen 1's). Variables at fixed addresses (`ram_variables`) 139 -> 0
- [x] The state's blocks by name where the code reaches them at their fixed addresses: a field of a
      screen's block, of a player's race record or of a copy of one is the block's field
      (`game->screen_1.unpack_state`, `game->races[0].car_number`), and a record the code counts from the
      first copy by a player's number its element (`game->race_copies[player].model`): the copies are
      eight, every player's with the link (`link_receive_records` brings in the other cabinets'). Structures
      reached by casting an address (`ram_structures`) 9,034 -> 8,920, addresses of the work RAM used as
      values 296 -> 182
- [x] Pointers, first part: the registers the whole program shares that point at a structure are C
      pointers (`struct object *current_object`, `struct car *player_car`, `struct screen *screen`), read
      as such (`current_object->state++`), set from a block of the state or a structure inside one by name
      (`screen = &game->screen_1`, `player_car = &screen->car`), and the runtime keeps a copy of them for
      each task. The structures inside a screen's block and an objects' area are members of theirs
      (`symbols.txt`'s `member` lines: the screen's ring of eight `views`, its `car`, the course's ring of
      128 `road_segments`, the `projected_segments`, the 416 `depth_layers`; the area's `spare_object`,
      `object_lists` and 124 `objects`). Where the C still needs the address the original holds there
      (a field that keeps one, a link in the page, a block copy), `address_of(screen)` gives it, and
      `pointer_to(address)` the pointer for one, into any memory the original's register points into: the
      test mode's R20 walks the bookkeeping in the sound board's RAM, and the text drawing's walks the ROM's
      texts, through copies `ram_outside` keeps the addresses of (`pointer_conversions`: 413). Structures
      reached by casting an address 8,920 -> 2,121; out of the memory model 145 -> 235 functions
- [x] Pointers, second part: a function's own variables, its parameters and what it hands back are C
      pointers where they point at the start of one structure of the state wherever the code reaches a
      field through them (`struct object *object = NULL; ... object->flags`, `struct object
      *start_object(void)`, `draw_object_at_scale(..., struct projected_segment *projected_segment, ...)`),
      the signatures decided from each function's own code before any call to it is written; a caller
      that holds an address passes `pointer_to(address)`, and a pointer compared with 0 is `NULL`
      (`object != NULL`). Structures reached by casting an address 2,121 -> 1,173; the conversions 413 ->
      706, where each new pointer meets what is still an address: the fields that keep one, the links
      in the page, the parameters of a function whose code reaches no field through them
- [x] Pointers into the program ROM's records the same way, read in place in the piece of the program's
      data (`const struct sprite_animation *sprite_animation = NULL; ... sprite_animation->frames`):
      `rom_pointer(address)` gives the one for an address the code holds, and `address_of` finds the
      address back from the piece. Tables reached through an address (`table_addresses`) 1,283 -> 1,163;
      the conversions 706 -> 822 (`rom_pointer` counted with them), most of them a field or a table that
      keeps a ROM address (`rom_pointer(player_car->surface_entry_1)`, `rom_pointer(stage_courses[i])`)
- [x] A variable that walks its record (it reads a field and moves on to the next, as the V60's `[R10+]`
      does) as a pointer to the record it is in: `size_step->height`, not
      `SPRITE_SIZE_STEP(size_step - 0x4u)->height`; its steps within the record go, and one onto another
      record moves it by records (`sprite_place++`, `race--`). Structures reached by casting an address
      1,173 -> 935, tables reached through an address 1,163 -> 779; the conversions 822 -> 900, one where
      such a pointer gets its value (`size_step = rom_pointer(...)`) in place of a cast at each field;
      326 lines of C less
- [x] Pointers to arrays of plain values the same way (`const int16_t *palette_list; palette_list[0];
      palette_list++`), made from the table, the variable or the field they start at
      (`stage_end_sound_cues`, `&game->memory_test_results[0]`,
      `&current_object->road_tiles_change.source_pages[0]`); and a walking pointer read where it points
      past the start of its record, as the address of the field there
      (`address_of(&road_row_record->hit_box_along)`). Structures reached by casting an address 935 -> 804,
      tables reached through an address 779 -> 566; the conversions 900 -> 1,060, most of them a pointer
      made from the address a field keeps, or given back to it
- [x] What the C leans on of the processor it runs on, counted (`python tools/width_census.py`, from
      `s32ref --access-widths` over the 16 scenarios, whose states stay the same with it): the reference
      follows the work RAM as a big-endian processor running the C would hold it, the C keeping the V70's
      places and widths (each value written goes there in that order, a string move takes its bytes along, a
      bit instruction changes the byte its bit is in), and of the 2,209 million reads it compares, 257 places
      and widths read another value there, in 128 functions: `flags` written a byte at a time and read with
      the byte after it, palette colours written by halfwords and read by words, a halfword's low byte. The
      program's data is read at more than one width at 4,867 places, in 10 functions (the crash physics'
      records a halfword and its high byte, the palette fades' lists); 22 accesses of the work RAM and 2,290
      of the data are at an address out of line with their width, in 10 and 27 functions (the unpacker's
      halfwords at odd addresses, the road objects' records, the course records). Left out: the stacks, the
      memory test and the copies (`WIDTH_ROUTINES` in `tools/game_profile.py`), and the ROM's sums, which the
      board replaces
- [x] The board's memories in one order on both sides: each keeps its values in the host's order, so that
      the C reads them as the processor it runs on holds them (`runtime/include/s32/machine.h`). The work RAM
      and the RAM the sound program shares keep each value at the width the programs reach it (the bus takes
      them before its 16-bit lanes, as the values they hold); the video RAM and the sprite RAM, which the
      video reads by 16-bit words, keep words, as MAME does: the byte at an even address is the word's low
      one (`S32_BYTE_LANE`: 0 on a little-endian processor, 1 on a big-endian one) and a 32-bit access is the
      two words in order. The video reads them so (`video_word`, `video_byte`, the sprites' graphics in
      sprite RAM by their bytes), the state's hash takes the words as the V70 holds them, and the game's
      bytes of those memories go through `board_byte` (`BOARD8`, and the byte views `ram.h` declares:
      `UNPACKED_BYTES`, the sound commands). On a little-endian processor that is the V70's order, byte for
      byte: the 16 scenarios identical in state, picture and sound. The video's memories reached 32 bits at a
      time are the next entry's; the work RAM's hash and the sound board's RAM the window port saves still
      need each value's width on a big-endian processor
- [x] Each access of several elements as the elements it moves: where the original reaches from an element
      of an array on into the next ones in one access, the C reaches each (the decompiler's emitter, as it
      already did for named fields: `element_run` in `decompiler/cprint.py`, which covering() and tiling()
      take), so that each element takes its part of the value whatever order the processor keeps their bytes
      in, and none is reached at an address out of line with its width. A store is each element's part
      (`SPRITE_WORDS(sprite_entry)[0] = (uint16_t)layer_head; SPRITE_WORDS(sprite_entry)[1] = (uint16_t)(layer_head >> 16u);`,
      and the other half 0 where the value is one halfword: `[0] = (uint16_t)(older_group + 0x300u); [1] = 0u;`),
      a constant each element's (`game->memory_test_results[4] = 1u;` four times), a read the elements put
      together (`game->video_registers[8 + layer * 2u] | (uint32_t)game->video_registers[9 + layer * 2u] << 16u`,
      the course's stream `course_place[0] | (uint32_t)course_place[1] << 8u`), and a copy element for element
      (`race_order = entrants[0]`...). The views of the video's memories that the code reaches 32 bits at a
      time are its 16-bit words, as the video reads them (`TEXT_LAYER_FONT_WORDS`, `TILE_PIXEL_WORDS`,
      `BITMAP_LAYER_WORDS`, `LINE_SELECTS` for two lines' selects, the scroll registers `NBG0_SCROLL_X`...: at
      31FF10 a layer's 16.16 scroll as the fraction's word, which the chip does not read, and the pixels'), so
      a 32-bit value goes there as its two words, the low one first, and a copy word for word
      (`NBG0_SCROLL_X[running_layer * 2u] = game->video_registers[8 + running_layer * 2u];` and the next).
      `wide_access` 204 -> 38, clean 602 -> 624 functions (81.9%) and 97.7% ->
      98.3% of the lines; the uses of R12 347 -> 349, as the two stores of r12 into a sprite entry are its two
      halves now, and the memory model's casts go up where each element repeats the one its access goes
      through (`SCREEN(entry_place - 0x300u)->accepted_players[0]` to `[3]`, `INPUT_TEST_BYTES(item)[0] |
      ... INPUT_TEST_BYTES(item)[1] << 8u`: `ram_structures` 804 -> 813, `table_addresses` 566 -> 586,
      `page_links` 96 -> 97), which go with the casts when the state leaves the memory model. The 16
      scenarios identical in state, picture and sound
- [x] A big-endian build, and the program's data in the host's order: `cmake/powerpc-linux-gnu.cmake` builds
      the headless game for 32-bit PowerPC Linux, which `qemu-ppc` runs (Debian and Ubuntu's
      gcc-powerpc-linux-gnu and qemu-user), with no warning. The loader puts each value of the data in the
      host's order at the width the asset gives it, and keeps where each one lies (`host_order` in
      `runtime/src/assets.c`: a 24-bit value and the byte after it are one 32-bit word, as the C reads
      `dj_messages` and `radio_songs`); the bus takes the data's bytes as the chips hold them
      (`data_byte_at`), and a string move between memories that keep the host's order copies their bytes as
      they are (`move_element` in `runtime/src/game.c`: a text of the data into the LCD's). On the PC the
      same, byte for byte: the 16 scenarios identical. Checked against the census (`ORUNNERS_DUMP_UNITS` writes
      where the loader lays out each value, `width_census.py report` compares the original's reads with it):
      33 tables were read in parts of the values their assets lay out (145,969 reads, in 44 functions),
      where an asset types its data otherwise than the C reads it. Three are typed now, each part by what the
      code reads in it (`symbols.txt`): the records after `sprite_list_switches`' pointers as
      `sprite_list_switch_records` (`struct sprite_list_switch`, by their fields), the packed tile pages
      inside `background_tile_page_lists`' piece as `background_packed_tile_pages` (bytes, which the unpacker
      reads), and the bytes boot writes to the interrupt controller as `boot_interrupt_control_bytes`. Then
      the extractor follows the typed pointers (`pointed_segments` in `tools/core/formats/typed.py`): what a
      table of addresses, an array of them or a record's field points at, as `symbols.txt` says (`table NAME
      u32 STRUCT`, `elements NAME u32 STRUCT`, `field STRUCT OFFSET NAME u32 STRUCT`), is laid out in its
      piece as that structure, and its own pointers followed the same way: a record takes its size (the
      fields that fit, where the piece ends first), an array of values and a record whose array goes on past
      it up to what comes next, a table of addresses ends where the first record it points at in its own
      piece starts, and the rest of the piece keeps its own type from the piece's start (`car_handlings`'
      records by their fields, `steering` to `slide_factor`). An array of addresses past a record is followed
      up to its first mark (`sprite_animation`'s `frames`, each a `sprite_frame`, then 0, -1 or -2), and a
      record followed by records of another structure says so (`field STRUCT OFFSET NAME STRUCT2[]`, which
      only the assets read: a `palette_block_list`'s length, then its `palette_block_entry`s; a
      `palette_fade_list`'s, then its `palette_fade_entry`s, whose `from` and `to` are colours; a
      `road_palette_record`'s road fades, then the fades it starts), and the records the formats that walk
      their own pieces read (the stages' background objects: `palette_cycle_record`, `sunset_fade_record`)
      have their pointers followed too. The assets build the ROM back byte for byte. What is left of the data
      read across its values: 4,528 reads in 17 tables (the crash physics, the torque curves, the roadside
      surfaces, a few palettes and texts). On PowerPC, `coin_start` runs 608 frames with the sound's samples
      identical all the way (the sound RAM's hash is the host's own: its values at the programs' widths) and
      the picture up to frame 139, where the work RAM shows: the palette RAM is the same there, but the
      mixer's fade and a few sprite and tilemap words are not, written in the work RAM at one width and read
      at another (the census's first part); it stopped at frame 608, where the sound program read its data at
      0 (the next entry)
- [x] Each value at its own width on a big-endian processor, found by comparing the state there with the
      PC's (`tools/state_diff.py`, over the headless game's `--dump-frame` dumps of one frame on both): each
      place `symbols.txt` names is compared as a value of its width (a variable, a field of the state's
      structures, a family's field where no other family has bytes of it at another place or width), the
      bytes nothing names by 32-bit words in either order, and a value that holds the PC's bytes as they
      are says so (bytes moved there a halfword at a time: the backup's course records in a race record the
      link sends). What it found, each fixed in `symbols.txt` so that the decompiler writes each field at
      its own width:
      a wide store over bytes with no name, which now have one: the game mode's 32-bit writes clear the
      byte after `game_step` (`game_mode_spare`), the controls' the byte after `brake` (`controls_spare`),
      the traffic's and a road crosser's offsets their sign in the halfword after (`x_offset_sign`,
      `y_offset_sign`: the object code reads the offsets as 16 bits), and the sound program's drum time-out
      the byte after it (`drum_timeout_spare`); the brake lights are a halfword.
      Names of two widths at one place, a union that the host lays out in its own order and a big-endian
      processor aliases the other way round: the wide name goes where its bytes have names of their own
      (`sprite_place`'s `size` and `place`, `coin_meter`'s `frames`, `screen_lamps`' `checkpoint_all` and
      `leader_all`, `road_segment`'s `width_code_and_event`, `tilemap_pages`' `pages`, `sprite_text_record`'s
      `place`, the sound program's `position` and `playlist`), and a byte written into a register is a part
      of it (`colour_offset_select`, mixer register 3E, whose high byte the fade sets).
      A view at another width than what it reaches: the mixer's copies (`mixer_register_copies`, halfwords
      the code reads two at a time), the scroll targets (`scroll_targets[16]`, halfwords), a tilemap's page
      word in the registers' copy (`page_select`, the road's each frame), the sprite places' blank words, and
      a palette list's length (`length` and `length_high`: a fade's own list in its object has only the low
      half written, `upload_palette_list` reads both).
      The program's data laid out at another width than the code reads it: the tile pages' lists, the
      background setups (`background_setup`'s `tile_pages`, the car select's, and four tables by screen the
      code reached by number: `car_presentation_backgrounds`, `name_entry_backgrounds`,
      `route_map_backgrounds`, `entry_countdown_backgrounds`), the LCD's texts a move copies
      (`lcd_text_characters`), the transmission choice's and the car presentation's palette lists, a stage's
      road palettes (`stage_road_palette_record`: the road's fades, then a palette list to upload, where a
      change of background's record has a fade list), the crash table after each crash physics record, and
      the arrays a pointer reaches: a torque curve's points, a model's rider places, a stage's roadside
      surfaces (`u32 STRUCT[]`, which the extractor lays out as records up to what comes next; the C reads
      STRUCT). The census's data read across its values: 4,528
      reads in 17 tables -> 1,169 in 7.
      Besides the names: the sound program's emitter splits a store over named fields as the V60's does
      (`SEQUENCE_PLAYER(player)->position_low = (uint8_t)position;` and the high byte); the Z80's bus takes
      the sound program's data as its chip holds it (`assets_sound_byte`, `s32_board.sound_byte_at`), as the
      V60's does (`data_byte_at`), where the C's tables read it in the host's order; and the sound's hash
      takes each sample low byte first, as the PC holds it (a big-endian processor's bytes in memory gave
      another hash for the same sound). `wide_access` 38 -> 8, clean 624 -> 642 functions (84.3%); the
      memory model's casts go up where a value goes into its fields through the same cast
      (`TILEMAP_PAGES(...)->upper_pages` and `->lower_pages`, `PAGE_SELECT`, `SPRITE_PLACE`, `COIN_METER`:
      `ram_structures` 813 -> 832), where the code reads the course's entries a byte at a time (`COURSE_BYTES`, 6) and names
      the background tables it gave as numbers (4: `table_addresses` 586 -> 595), and in `road_row_draw`,
      where the split stores made the decompiler copy the loop's step into the branch that skips an item
      rather than the end into the loop (`page_links` 97 -> 102, `pointer_conversions` 1,060 -> 1,062). On
      PowerPC `coin_start` runs its 2,401 frames, the sound identical all the way and the picture up to frame
      2,058 (139 and 608 before): from 2,059 two clip entries of the sprite list and screen 2's tyre noise (a
      byte of the RAM the sound program shares) differ, which the code writes from data the work RAM does not
      keep; on the PC the 16 scenarios identical
- [x] `coin_start` identical on PowerPC, its 2,401 frames, picture and sound, and none of the program's data
      the scenarios reach read across the values the loader lays out (the census: 1,169 reads in 7 tables
      -> 0; the bytes read wider, 4,223 in 10 tables -> 4,190 in 8). Frame 2,059's clip entries: draw modes
      1 and 6 take the address of a ready entry of the sprite list from the object's fields 38-3B, which
      `crest_clip_update` writes as one word (`clip_entry`) and the C read as the two halfwords the other
      objects keep there (`animation | palette << 16`), the halves a big-endian processor holds the other
      way round; each is now a family of its one function with a field of its own
      (`current_object->draw_object_command.entry`). The data, each typed in `symbols.txt` or in its format
      as the code reads it: the hit boxes of the road rows', the flashing scenery's and the start gate's
      records four halfwords (`hit_box u16[4]`, 812 records: two 32-bit words before, of which the code that
      starts the object tests the first, now `(hit_box[0] | hit_box[1]) == 0u`); the end of a traffic list
      its position and level; the endings' lists and their actors' scripts pieces of their own
      (`ending_actors`, `goal_actor_scripts`: halfwords, 32-bit words before); `scene_palettes`' entries
      colour blocks (`palette_block`: the four blocks in the table's own piece laid out as place, length and
      colours); the test mode's instructions and two settings' values as texts
      (`test_mode_instructions_text`, `game_assignments_value_texts`). The extractor's walk of the typed
      pointers follows the records of a list that follows a record too (a road palette record's fades, a
      stage's palette list), and a list that says its length holds that many bytes of records, where the
      walk went on up to the first record that points nowhere: past a list's end it took a block's place
      and count for an address (`0x00012040`) and laid colours out over other tables (`music_fade_addresses`'
      second entry, `demo_sounds`, the endings' lists, 92 bytes of `road_row_size_shift_lists`), and the
      palettes only such lists point at stayed bytes of a fade list (22 more palette lists and 185 more
      runs of colours laid out as their own). The loader's layout before and after, compared value by value
      (`ORUNNERS_DUMP_UNITS`), changes nowhere else, and the assets build the ROM back byte for byte. The
      cars' hit boxes are members of `struct screen` (`car_hit_boxes`, `player_car_hit_box`), which
      `state_diff.py` compares by their fields: at `coin_start`'s last frame the state on PowerPC is the
      PC's but for the backup's checksum at 701F00 (the next entry). On the PC the 16
      scenarios identical; `casts` 3,027 -> 3,023, the rest of the counts as they were
- [x] The scenarios that cover the code on PowerPC: six of them (`coin_start`, `game_over`, `test_mode`,
      `two_players`, `name_entry`, `sound_test`: 127,066 of the 16's 563,396 frames) run 98.6% of the
      addresses the 16 run, the V70's and the Z80's (their `executed.txt`), and take 20 minutes at once
      under qemu, picture and sound hashed every frame (23 ms a frame; `coin_start` 68 seconds). What they
      found, each a value written at one width and read at another, which the PC does not show:
      a fork's two courses, 32-bit addresses the code takes from the course's stream, which the C reads
      everywhere else a byte at a time: the bytes they are (`course_fork` `branch_courses`, put together as
      the stream's other values are); the checkpoint board's car images, a word each (the animation and the
      palette) that the row's drawing read as two halfwords of a structure of its own: the two halves of
      the word (`(uint16_t)car_sprite[0]`, `car_sprite[0] >> 16u`); the road's page table, whose words
      `road_tiles_change_update` writes a byte at a time: bytes, four of them put together where
      `road_surface_update` takes a page word (the decompiler writes an access by its number that runs on
      over the elements of a family's array as those elements, as it did for a record's); the cells of the
      test mode's texts and of the input test's items, halfwords in a stream the C reads a byte at a time:
      a column type of the typed format, `b16`, a 16-bit value the loader lays out as its two bytes; the
      last items of the turntable's scenery, bytes at the end of the groups' piece: a piece of their own
      (`turntable_scenery_items`); the route map's signals, a word whose low half had a second name of 16
      bits that the C cleared bits through (on a big-endian processor a union's narrower member is the
      other half): one name, the whole word (`screen->route_map_signals &= 0xFFFFFFF3u`; the unused viewer
      of the Japanese texts keeps its number in its low half); the time difficulty the master cabinet
      sends, two bytes and a word in the race record and six bytes by another name over it: six bytes
      (`race->master_time_difficulty[5]`), found by reading, as the scenarios leave them all 0. And one in
      the tests themselves: the scripts hold the race's time up
      with a byte poked into the high half of `time_left`, which a memory that keeps its values in its
      processor's order holds in the other byte: a script's `poke` may name the halfword its byte is one
      of (`poke 20FB6B 0A 20FB6A`). The census lists the texts' cells with the bytes the original reads
      wider (4,190 reads in 8 tables -> 4,421 in 30: the C reads each as its bytes); `casts` 3,023 ->
      3,038, the bytes put together. With these the six are identical on PowerPC, picture and sound, all
      of their frames; on the PC the 16 scenarios identical, and the assets build the ROM back
- [x] The 16 scenarios identical on PowerPC, picture and sound, all of their 563,396 frames: the other ten
      (`attract`, `race_left`, `race_right`, `race_to_goal` and the six `route_*`: 436,330 frames, which
      run the 1.4% of the addresses the six do not) needed nothing more, 25 minutes at once under qemu
      (`python tools/big_endian_scenarios.py`). `python tools/select_scenarios.py --big-endian` keeps that
      state as verified, so that a change runs again on PowerPC only the scenarios it can reach
- [x] Nothing read at an address out of line with its type's alignment: a build with `-fsanitize=alignment`
      (`-DORUNNERS_ALIGNMENT_CHECK=ON`, GCC or Clang) runs the 16 scenarios with no report, picture and sound
      identical. Of the accesses the census finds out of line (2,290 to the program's data in 27 functions,
      22 to the work RAM in 10), the C already read all but five places a byte at a time or as members of the
      packed structures, which the compiler reads at any address. The five, in three functions: two lists of
      halfwords at an odd address in the middle of `road_object_streams`' piece, the first of
      `flashing_scenery_palettes` (021041) and the second of `aurora_animations` (02C481), which the code
      reads there whole: `elements ... unaligned` in `symbols.txt`, which the C reads through a type of
      alignment 1 (`const unaligned_int16 *palettes`, `game/support.h`: GCC and Clang read it a byte at a time
      where the processor needs it, as PowerPC's `-mstrict-align` shows); and the output test's bits, which
      `test1` and `not1` test and flip in the 32 bits from an output's byte (20E092 to 20E095): the four
      bytes they are (`port_output_bits`), read and written in their order as the V70 does, where the C took
      a word at an address out of line, which on a big-endian processor also tested and flipped a bit of
      another byte (`test_mode_output_test_toggle` too, which no scenario runs). `ram_structures` 832 -> 835
      and `casts` 3,038 -> 3,048, the bytes put together
- [ ] After the decompilation, before the state leaves the board's memory model (Sebas, 05-11-2026): a C that
      does not lean on the processor it runs on, so that the game builds for one unlike the PC's: big endian,
      and taking a halfword or a word only at an address of its width (the Saturn's SH-2). The C counts on a
      little-endian processor that takes any address, as the V70 does (`runtime/include/game/support.h`
      says so, and the build turns strict aliasing off): the board reads its memories in the V70's order, a
      byte at a time (the bus's `load16`, the video's `video_word` and `sprite_word`), where the game's C
      writes them in the host's (`BOARD8`, `BOARD16`, `BOARD32`: 32 uses) and keeps its state, which the bus
      reads too, in the host's as well; the loader puts the program's data into its pieces in the V70's
      order (`put` in `runtime/src/assets.c`), which the C reads in place in the host's (`data_at`,
      `rom_pointer`: 145 and 228 uses); several fields are read or written in one access (`AS16`, `AS32`:
      the 204 of `wide_access` under "What is left, and why"), whose halves a big-endian processor takes the
      other way round; and the structures are packed at the V70's offsets (`#pragma pack(1)`), and some of
      what the code reads is at an address that is not a multiple of its width. The steps, each checked by
      the 16 scenarios: what leans on the host counted (`width_census.py`), the board's memories in one order
      on both sides, the accesses of several elements as the elements, none of the program's data read across
      a value the loader lays out, each value at its own width wherever the 16 scenarios reach it on PowerPC,
      all of their frames, picture and sound, and nothing read at an address out of line with its type's
      alignment, are done (the entries above). Left: the work RAM's hash and what the window port saves by
      each value's width, so that the state compares in the V70's order on any processor (the scenarios on
      PowerPC compare the picture and the sound; `tools/state_diff.py` found the state of `coin_start`'s last
      frame the PC's but for the backup's checksum). It is done when a big-endian build runs the 16 scenarios
      under QEMU with the state (in the V70's order), the picture and the sound identical. The tasks' stacks are the
      port's: `runtime/src/tasks.c` takes fibers on Windows and ucontext elsewhere, and a machine with no
      system switches them its own way (`host/tasks.h`). The V60's arithmetic (a division by 0, a shift by a
      computed count), the status word and R12 and R14 are plain C that does what the V70 does on any
      processor, and so are the addresses the state holds (the entry below), only slower than pointers
- [ ] After the decompilation (Sebas, 28-10-2026): the game's state out of the board's memory model. The C
      keeps it laid out as the board's work RAM (128 KB at 200000), at the places the original had it: one
      structure since the first step (`struct game`, its variables `game->game_frame_count`), its
      structures are reached by casting an address (`OBJECT`, `SCREEN`, `CAR`, `RACE`: 9,034 uses), its
      pointers are 32-bit addresses (`current_object`, `player_car`, `screen`, a screen's block at 208000 or
      218000), a link is an address's low halfword in its 64 KB page (`LINKED`: the objects' and depth lists)
      and an object's handler a code address that `call_code` looks up in `game_code` (206 uses of a
      function's address or of `call_code` in `src/`; `python tools/noise_report.py` counts all of these). In readable C that is noise, and the ports never go back to the board. What
      moves is the work RAM: the video RAM, the sprite RAM, the palettes and mixers, the link board's memory
      and the sound board's RAM are how the game talks to the board, which the ports' video and sound replace,
      separately. A structure moves into the host's memory, with real pointers and plain C variables, once
      every function that touches it is C for good: subsystem by subsystem, not all at the end, since until
      then the decompiler writes the C from the original's code and each frame's work RAM is compared with the
      original's on MAME's V70 (`s32_state_hash`). It opens fields added to the game's structures, more than
      one world in a process (a quick match's server and client, replays), sizes that grow, other processors
      with no addresses to translate, and a debugger that shows objects by their fields. It needs the 542
      tables the loader sets by name (`game_tables`) as constant arrays with their pointers translated, as
      many hold addresses of data or code: `stage_courses` the courses' streams, whose command -1 goes on at
      the address it holds; the tile page lists, a page number in an entry's top byte and its packed data's
      24-bit address below; the road objects' streams, each object's handler and record. What is saved keeps
      its format, with a reader of its own, so what was saved before still opens: the EEPROM's 64 words (the
      settings and their backup: `orunners.eeprom`), and the bookkeeping and the ten course records in the
      sound board's RAM from 701F00 (the backup RAM, with a checksum and a copy), which the window port does
      not keep yet. The game's behaviour stays the original's: the engine's fixed limits (124 objects a
      screen, the road's ring of 128 segments, 416 depth layers, sprite lists that fill up) are another change
- [ ] After the decompilation (Sebas, 29-10-2026): races between machines in the ports. The game links up to
      four cabinets of two seats each, eight players (`player_count` twice `machine_count`), through the
      communication board it reaches at 800000: one of them is the master (the test mode's NETWORK
      ASSIGNMENTS: PRIVILEGE MODE, CABINET ID); `network_check` tests the board at boot,
      `link_take_master_settings` hands the master's settings and course records to the others, and every
      frame each machine sends its players' race records and takes the others' (`link_send_records`,
      `link_receive_records`), which each screen draws the other cars from. There is also an ISDN mode
      between two places (`isdn_count_players`: the players on this side and on the other), and a cabinet
      alone runs STAND-ALONE. The runtime has the board as MAME has it with no other cabinet on the line (the
      link stays offline), so no scenario links two machines and none runs the ISDN mode. A port links
      machines with a board of its own in place of the communication board: each running game a cabinet, the
      board's memory carried over the network. First scenarios that link two machines, checked against the
      original, so that the link's code is verified like the rest; more than one cabinet in one process needs
      the game's state out of the board's memory model (the entry above)

## What is left, and why

What the C still says the way the original's instructions do, rather than in the game's own terms. Each of
these does exactly what the original does; the counts come from `python tools/noise_report.py` (its line
names in brackets) or, for the ones it does not count, from the C as it is now.

| What | Count | Why |
|---|---|---|
| R12 and R14 named after the register (`context_registers`) | 349 uses | Scratch registers whose last value the next handler reads: `race_cars_start` puts the car's address together from R12's high half, which the handler before it left (the screen's page), and the road rows' and the scenery's drawing keep the height of the last size step in R14 and skip reloading it when the object's step is the same. The C keeps them as globals, a copy for each task, only in the functions where they have to be (74, 104 before the twelfth round): the boundary functions (an object's handler, an interrupt, a task), the two that call through a memory slot (`run_objects`, `test_mode_frame`), `wait_for_frame` (the vertical blank's interrupt writes R12), the ones a value only goes through on its way to the next handler, and their callers. In the others they are registers like the rest: parameters and results with names of their own (`race_car_put_sprite(..., &last_piece, ..., &last_size_step)`, `eeprom_read_word(command, eeprom_port, ...)`), and locals; a value stays a local where nothing reads it after the function (the hit boxes' `first_segment`), and the C keeps the parts of the global a function only takes through (a halfword it writes keeps the page above it). 932 uses before the eighth round, 720 before the twelfth. R27 and R28 are locals: the one function that would read them before setting them, `player_car_project_road`, does so only if its projection pattern skipped every segment, and it starts with a 1 (`symbols.txt`'s `unread` lines) |
| Memory reached through a computed address (`mem_access`) | 21 | The routines that work on memory by its address, whatever is there (`MEMORY_ROUTINES` in `tools/game_profile.py`, faithful): the memory test (6), `unrolled_copy`, a copy from any address to any other (8), what boot writes where the board maps nothing (E00000, E80000: 6), the checksum of the settings (1). Nothing else: the last 4 went in the eleventh round (the car numbers one before the array by a count that goes down, the road's page table by page sets counted from 3, the body's frame by the steering from the middle of its table); 149 before the second round, 48 before the seventh |
| Several fields read or written at once, `AS16`/`AS32` (`wide_access`) | 8 | The original moves them in one instruction; where the access runs on from an element of an array into the next ones, or over named fields, the C moves each as its own (the entries under "Status"; 204, then 38 before): what is left moves bytes as they are or writes zeros, which comes out the same on a processor of either order: a projected segment's width code, road event and depth layer copied onto the next one's (4), the seats' bookkeeping bytes read as the halfword the backup keeps (2), and their credits in the backup cleared (2) |
| Jumps through a table with an index it does not have (`invalid_jump`) | 0 | The original jumps through its tables of code without checking the index, and so does the C: its switch has the cases the table holds and no `default:` (191 before). `python tools/decompile.py --checks` (and `decompile_sound.py --checks`) writes the C for a build where an index the table does not hold stops the game with the address the original would have jumped to (`invalid_jump`, 192 places); the 16 scenarios are identical in that build too. Three functions end in such a switch whose every case returns: the C says nothing goes on past it (`unreachable()`, C23's), as nothing does in the original |
| Results written back before an early return (`copy_backs`) | 0 | A function hands back what it leaves in registers through pointers (`p_leftover`): kept in a local, it is written back as the function ends, once where the ways out meet at the end (`vector_angles`), straight through the pointer where the function only passes it on, and before the call that hands it on (`*p_leftover = x; draw_object_at_scale(..., p_leftover, ...);`). None left: `road_surface_lines`' 9, at three returns out of the loops its control flags left, went with them in the ninth round (64 before the sixth round) |
| Functions with `bool` control flags (`control_variables`) | 0 | Where the original's jumps join paths that no nesting of ifs and loops can, a flag would say which way to go instead of a goto (59 functions had them before the sixth round). None is left: the decompiler writes what C can say: a small block several jump to written for each, a jump into a state's case right after storing its number written as the switch running again (`state = 3u; continue;`), a larger piece of a loop entered at two places copied for the second way in, a large piece several states jump to is a function of its own (`car_radio_talk`, `player_car_crash_step` and the ninth round's: `func` lines), a loop entered at more than two places gets a copy of the rest of the round those ways start (`palette_fade_step`), a test made again with nothing changed between goes straight to its side (`race_car_draw`), a short way out several blocks of a loop lead to is written where each is (`road_surface_lines`), and code two nested loops both leave to is where a function of their own goes back to its caller (`second_task_frames`, an `exit` line) |
| Partial writes of a variable, `(x & ~0xFFFFu) \| ...` (`partial_writes`) | 130 | Each changes a part of a value and keeps the rest of the same value, as the original's byte and halfword instructions do (`colour_count`'s `inc.b`, `palette = (palette & ~0xFFu) \| (uint8_t)(palette + 1u)`; a pointer moved within its 64 KB page), which the faithful measure takes as the original's. None puts a part of one value into another (`mixed_partial_writes`: 23 lines, in 12 functions, before the tenth round): where something reads the rest a write keeps, the write changes a part of the one value the register holds, and the C writes it on that one variable, a step a line where the part written and the rest both carry something: two halves the code keeps in one register and turns round (the random numbers' state, `random_number`; a pair of x in `road_row_pair_draw`), bits shifted in one at a time (the A/D converter's, `read_switches_and_analog`; the EEPROM's, `eeprom_read_word`), a cell's attribute and character (`test_mode_bookkeeping_draw_histogram`), the Z80's register pair BC with a count in B and a register in C (`set_voice_modulation_screen_1` and `_2`), a result the code puts in the low byte of what a call left (the turntable's leftover and the item's flags, the memory test's results), the riders' gestures in R12 (`race_car_draw`), the last frame's segment in the low half of `road_surface_halfway_lines`' stripe count. A call passes only the parts of a register its callee reads, so what a caller would put in a part the callee clears first goes (`multiply_bytes`' callers: `level_product = (uint32_t)note_level << 8`). A bit tested by a register's number reads only the bits the numbers that register can hold reach (`ranges.py`: the link board's self-test, the staff credits code's controls, the coin chutes' switches from their table in the ROM), so a byte read into the register before such a test keeps nothing of what was there. The V70 writes the low byte or halfword of a 32-bit register and keeps the rest; the C does the same where the rest is read later: a pair packed in one register (`draw_text_at`'s position, x in the low halfword and y in the high one), halfword sums on a register whose high half goes on into a product or a store, a result the callers' variables take whole through a pointer. What every call passes and every way back hands back is followed bit by bit across functions, a function returns only what its callers read of its result, and a register a function only hands back is worked out only where its caller reads it afterwards, so those need none (439 before). A pointer moved within its 64 KB page (the next entry of a list, a link the code reads: `chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail)`) says so with `LINKED`, as the objects' links do (77), and a place so many bytes on from a pointer in its page, added in 16 bits as the original does, with `PAGE_ADD` (the ring of the road's segments, `PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset)`: 10), and one within its 256-byte page with `IN_PAGE` (the sound program's MIDI queue's next byte, a sequence player's field; the game's ports of an I/O chip) |
| Data read through its ROM address (`rom_reads`) | 0 | The course's stream, the input test's list and the test mode's texts read as the bytes of their tables, a halfword or a word of them with `AS16`/`AS32`; a fork's table of the road objects' streams as its own elements (26 before) |
| Tables read by their address (`rom_table_reads`) | 0 | Both programs read their tables by name: a pointer each that the loader sets (`src/table.c`'s `game_tables`, `src/sound/tables.c`'s `sound_tables`); 847 before |
| ROM addresses printed as numbers (`raw_rom_addresses`) | 0 | Every ROM address the code uses as a value has a name; the last four were `car_radio_update`'s switch going on at its own jump targets, which now runs the switch again or calls `car_radio_talk` |
| Fields named after their offset (`unk_fields`) | 0 | The objects' fields 09, 0A and 0B that the code that starts an object sets alike for any family: `number` (its place in its start list, a choice flag's player), `car`, `car_colour` (5 before) |
| A function the board puts its own in place of (`board_replaced`) | 1 | The memory test's sums of the ROM chips (`runtime/replaced.txt`): the game has no ROM chips to sum, so the board hands back the sums the program keeps for them (at 03FFF8 and 03FFFC), as good chips give |
| Work RAM addresses printed as numbers | 25 uses | The tasks' stacks and frames (`0x21F800u`, `0x21FF84u`), places in the screens' blocks (the sprite places `0x20C000u`, one past screen 2's LCD text `0x218F20u`, where `test_mode_start` leaves R12) and the coin meters after the first: the decompiler names a constant only when it is a variable's own address. The memory model's `ram_addresses` counts them |
| Names made up of an address or an offset (`unk_names`, `made_up_names`) | 0 | None in the code, none in a comment, none declared for nothing: 17 functions' comments gave fields by their offset, written before the fields had names (`the car object's unk_10`, its `timer`), `struct object`'s view of every object declared 64 fields named after their offset that no code reads, as the families' views name those bytes (86 before, now padding), and `test_mode_start` gave a place of screen 2's block by a name made up of its address (`unk_218F20_ADDRESS`), now its number |
| The game's state in the board's memory model (`ram_structures`, `ram_variables`, `ram_addresses`, `code_addresses`, `table_addresses`, `page_links`, `ram_blocks`, `pointer_conversions`) | 835, 0, 154, 206, 595, 102, 106 and 1,062 | The game's state is one structure, `struct game`, laid out as the board's work RAM (128 KB at 200000), at the places the original had it, so that each frame's state compares byte for byte with the original's; the code reaches its variables as its fields (`game->game_frame_count`; 139 macros over fixed addresses before), and its blocks where the code reaches them at their fixed addresses (`game->screen_1.unpack_state`); the registers the whole program shares, and a function's own variables, parameters and results, that point at a structure are C pointers (`current_object->state`, `object->flags`). A field that holds an address is still read by casting it (`RACE(screen->race_copy)->stage_count`), the other pointers are 32-bit addresses, the C turns a pointer back into the address the original holds where it still needs it (`address_of(screen)`) and makes one from an address it holds (`pointer_to`, `rom_pointer`), an object's handler is a code address that `call_code` runs, the tables are reached through the addresses they hold (`data_at`), a link is an address's low halfword in its 64 KB page (`LINKED`), and memory is moved, filled and searched through its address as the V60's string instructions do (`move_halfwords(object, zero_block_ADDRESS, 0x80u)` clears an object; the others clear lists, race records and tables, copy the settings, the LCD's texts, the sprite entries and the video and mixer registers' values, and the memory test keeps the work RAM in the sprite RAM while it tests it). Taking the game's state out of that model, with real pointers and plain C variables, comes once the C no longer leans on the processor it runs on; the board's own memories (the video and sprite RAM, the palettes, the sound and link boards' RAM) stay, being how the game talks to the board |
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
`verify_rom.py`, `render_state.py`, `memory_census.py`, `width_census.py`, `state_diff.py`,
`select_scenarios.py`, `big_endian_scenarios.py`) run the core's. A change to the decompiler or those
tools goes to arcade-decomp, checked on OutRunners, and comes here as a newer commit of the submodule
(`git submodule update --remote tools/core`). The reference the scenarios compare the game with is the core's
too (`tools/core/system32/reference/`: `s32ref` on MAME's cores, the disassemblers' oracles, `pcmcheck`),
which CMake builds through its `reference.cmake` with OutRunners' data library. `memory_census.py run`
counts what each mem_read and mem_write of the C reaches over the scenarios, on the headless game built
with `ORUNNERS_MEMORY_CENSUS=ON` into `build/bin-census` (`ORUNNERS_BIN_DIR`); `report` sums it into
`build/census/memory.txt`. `width_census.py run` runs the scenarios on the reference with
`--access-widths`, which lists every access the original makes of memory as data and follows the work RAM as
a big-endian processor running the C would hold it; `report` writes `build/access_widths/report.txt`: the
reads that processor gives another value, the program's data read at more than one width, and the accesses
out of line with their width, each by the place and the function. `state_diff.py LITTLE_DIR BIG_DIR`
compares the headless game's `--dump-frame` dumps of one frame on the PC and on a big-endian processor:
each place `symbols.txt` names as a value of its width, the rest by words.

`select_scenarios.py` says which scenarios a change has to run again, so that a correction does not take the
16 every time:

```bash
python tools/select_scenarios.py
python tools/select_scenarios.py --run
python tools/select_scenarios.py --big-endian --run
```

It compares the working tree with the state last verified, a copy in `build/verified/` that `--save` writes
(after a full `check_scenarios.py`) and that a `--run` that ends identical writes again; without one, with the
last commit (`--base REV` for another). `src/` is compared function by function, comments and layout apart: a
function is the same when its tokens are, or when they differ only in names, its own locals renamed one for
one or a name `symbols.txt` renames; a macro, a prototype, a table's entry and each structure's members, by
offset and type, are compared too, and a function that names one that changed counts as changed. A changed
function needs the scenarios whose reference run executed an address of it (`executed.txt`,
`sound_executed.txt`; a function's addresses go from its `func` line to the next). The game's loaded data
(the headless game's dumps: every piece's bytes, and where the loader lays out the values wider than a byte,
which matters to a big-endian processor alone) needs the scenarios in which the original read an address that
changed (`build/access_widths/`, which `width_census.py run` writes). The runtime, the headless port, the
build's files, the sound's timing model, the sound program's data and the pictures and samples are every
scenario's; a control script is its own scenario's, whose reference runs again. Each scenario is listed with
why. `--run` runs them (`check_scenarios.py NAMES --native-only --video --sound`); `--cover` picks, of those,
the shortest that between them run every function that changed, a first check that leaves the state as it
was. What it does not look at: the reference and the compiler (a change to either is a full
`check_scenarios.py --video --sound`), and what a scenario executes is what its last reference run did.

`--big-endian` does the same for the big-endian build, which has a verified state of its own, through
`big_endian_scenarios.py [NAME ...]`: the scripts on `build/bin-ppc`'s game under `qemu-ppc` (through WSL on
Windows), each frame's picture and sound samples compared with the reference's in `build/scenarios/`.

The rest of `tools/` is
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
