"""
Name:        game_profile.py
Description: what the shared decompiler core (tools/core) needs to know of OutRunners: where the game is,
             its ROM set, and its settings
Version:     0.1.0
Created:     25-10-2026
Modified:    06-11-2026
Author:      Sebastian Toyos

The core (arcade-decomp: the decompiler, the V60 and Z80 frontends, the analysis, the checks) is the same
for every game; this module gives it OutRunners': the repository's root, the ROM set's module
(orunners_rom: the regions as the board has them), the names the generated files carry, and the
decompiler's settings for the V70 program, which it puts in the core's modules when it is imported: the
structures (the objects, 0x100 bytes; the screen's car, at 1000 of the screen's block; the block of the
screen a task runs, R25; the objects' area below it), the fixed ones (the two screens' blocks, the race
records), the registers the whole program shares and what they hold, the game's variables (from R24,
0x20E000), the object handler's slot, and the sound board's chip for the Z80 program (its MultiPCM).
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "core"))

import orunners_rom as rom  # noqa: E402
from decompiler import constants
from decompiler import dataflow
from decompiler import function
from decompiler import memory
from decompiler import pointers
from decompiler import signatures

ROOT = Path(__file__).resolve().parents[1]
GAME = "OutRunners (arcade, Sega System Multi 32)"      # in the generated files' headers
ROM_SET_TITLE = "OutRunners (orunners, World)"
CPU = "V70"
BINARY = "orunners"                             # its executables: orunners_game_headless, orunners_headless
GUARD = "ORUNNERS"                           # the generated headers' include guards: ORUNNERS_GAME_H...

# the program's structures, registers and variables (decompiler/pointers.py, memory.py, signatures.py)
pointers.STRUCTS = {"object": 0x100, "car": 0x200, "screen": 0x6000, "object_area": 0x8000}
pointers.VARIABLES = 0x20E000
pointers.PLAYERS = (0, 0, 0)
pointers.CONTEXT_KINDS = {20: ("object", 0), 23: ("car", 0), 25: ("screen", 0)}
pointers.OBJECT_REGISTERS = (20,)
pointers.OWN_REGISTERS = (20,)
pointers.HANDLER_FIELD = ("object", 4)
pointers.VARIABLE_BLOCKS = {0x20E600: 0x80,      # the settings, which the test mode walks with R20
                            0x20E716: 0x19}      # the memory test's results, which it writes through R20
pointers.WORK_RAM = (0x200000, 0x220000)
# the board's devices (runtime/src/machine.c): the sprite control, the two screens' palettes and mixers, the
# link board (its memory is a byte on each halfword), the two I/O chips, the interrupt controller, the
# random number generator
pointers.DEVICE_AREAS = ((0x500000, 0x500010), (0x600000, 0x700000), (0x800000, 0x801004),
                         (0xC00000, 0xD00010), (0xD80000, 0xD80010))
pointers.BASE_KINDS = {0x208000: ("screen", 0), 0x218000: ("screen", 0)}
pointers.PAGE_LINK_KINDS = {"screen": ("object", 0)}
pointers.NESTED = {"screen": ((0x1000, 0x200, "car"),)
                  + tuple((0x2000 + 0x20 * n, 0x20, "road_segment") for n in range(128)),    # the course's ring
                   "object_area": ((0x100, 0x100, "object"), (0x200, 0x100, "object_list"),
                                   (0x300, 0x100, "object_list"))
                   + tuple((0x400 + 0x100 * n, 0x100, "object") for n in range(124))}
pointers.BELOW = {"screen": (0x8000, "object_area")}
pointers.INSTANCES = {0x208000: ("screen", 0x6000), 0x218000: ("screen", 0x6000), 0x20FB00: ("race", 0x80),
                      0x20FB80: ("race", 0x80), 0x20FC00: ("race", 0x80), 0x20FC80: ("race", 0x80)}
# records the code reaches by a number from a fixed address, whose places have names of their own: each screen's
# page of the sound RAM, by the screen's index (set_tyre_noise)
pointers.NUMBERED_RECORDS = {0x700700: "screen_sound_page"}
memory.WORK_RAM = (0x200000, 0x220000)
memory.VARIABLES = 0x20E000
# the program's data is the game's assets, loaded by name (runtime/src/assets.c): its tables are pointers the
# loader sets (NAME_data), and an address the code holds reaches its data through data_at()
memory.LOADED_TABLES = True
memory.ROM_MACRO, memory.ROM_FUNCTION = "DATA", "data_at"
pointers.ROM_FUNCTION = "data_at"
SOUND_LOADED_TABLES = True    # the sound program's tables too: sound/support.h's sound_tables
memory.PLAYER_FIELDS = ()
memory.BUS_ONLY = []
memory.CANONICAL = rom.canonical_address
memory.NAMED_COPIES = False     # a constant in a mirror of the code ROM may be no address (the memory test's counts)
memory.NAMED_BASES = {0x208000: "SCREEN_1", 0x218000: "SCREEN_2", 0x20FB00: "RACE_1", 0x20FB80: "RACE_2",
                      0x20FC00: "RACE_COPY_1", 0x20FC80: "RACE_COPY_2"}
# the game's state, struct game, which the C reaches through game (game->game_frame_count): the work RAM, a
# 64 KB page for each screen, its objects' area below its block; the game's variables after screen 1's block,
# then the race records: this cabinet's two players', and the copies of every player's (eight with the link,
# whose records link_receive_records brings in); after screen 2's the race records the ISDN link brings in,
# and the stacks
memory.STATE = "game"
memory.STATE_BLOCKS = ((0x200000, "object_area_1", "object_area", 1), (0x208000, "screen_1", "screen", 1),
                       (0x20E080, "coin_meters", "coin_meter", 4),
                       (0x20E600, "eeprom", "eeprom_contents", 1),
                       (0x20FB00, "races", "race", 2), (0x20FC00, "race_copies", "race", 8),
                       (0x210000, "object_area_2", "object_area", 1), (0x218000, "screen_2", "screen", 1),
                       (0x21E000, "isdn_races", "race", 8), (0x21F000, "stacks", None, 0x1000))
# a copy whose ends are each one exact thing of its size is typed C (memset from zero_block, the ROM's 400 bytes of
# zeros; an assignment between two of the same type)
memory.TYPED_MOVES = True
# a place of the state is a C pointer from where it is made to where it is read: locals that only carry it,
# parameters whose callers all pass one (phase 2, typed pointers)
memory.TYPED_POINTERS = True
memory.ZERO_BLOCK = (0x100800, 0x100C00)
# the game's variables the code reaches only by name, never by its address (63 of them: none an array the code indexes
# past, none sharing bytes with another name, none read with the next ones at once; the ones it writes by their
# address stay in the work RAM: the settings the EEPROM is read into, the screens' VBLANK counts, the analog inputs,
# the coin sounds, the tile banks, and the seats' bookkeeping bytes, read as the halfword the backup keeps) live in
# struct world in the host's memory. The work RAM keeps their places, where world_project puts them for the state's
# hash each frame, and where they are while code reaches memory by its address (the memory test of the work RAM in
# place, with the interrupts off; crc16; unrolled_copy; the runtime's string moves: world_ram_begin and world_ram_end)
memory.HOST = "world"
memory.HOST_VARIABLES = frozenset(("waiting_task_stack", "vblanks_since_start", "debug_timer_rounds",
                                   "sprite_list_buffers", "sprite_lists_set_up", "single_task", "game_frame_count",
                                   "send_video_registers", "task_swapping", "second_task_waiting",
                                   "timer_interrupts_left", "vblank_counter", "cleared_on_race_over", "car_numbers",
                                   "link_wait_frames_start", "total_time_count", "player_count", "link_off",
                                   "link_failed", "link_wait_frames", "network_check_blink_counter",
                                   "isdn_players_this_side", "isdn_players_other_side", "link_wait_count",
                                   "chute_entry_offset", "seat1_coin_units", "seat2_coin_units", "wheel_motor",
                                   "switches", "previous_switches", "switches_pressed", "switches_released",
                                   "second_io_chip_port_f", "seat_1_player", "seat_1_wheel", "seat_1_gas",
                                   "seat_1_brake", "seat_2_player", "seat_2_wheel", "seat_2_gas", "seat_2_brake",
                                   "lcd_send_buffer_1", "lcd_send_buffer_2", "lcd_restart_frames", "lcd_send_step",
                                   "lcd_busy", "lcd_init_step", "test_mode_on", "test_mode_flags", "test_mode_cursor",
                                   "test_mode_page_cursor", "test_mode_routine", "test_mode_buttons",
                                   "test_mode_buttons_previous", "test_mode_buttons_pressed",
                                   "test_mode_buttons_released", "test_mode_blink_timer", "sound_test_number_a",
                                   "sound_test_number_b", "test_mode_sound_queue_write", "test_mode_sound_queue_read",
                                   "test_mode_sound_queue", "test_mode_privilege_mode"))
memory.HOST_SYNCED = frozenset(("test_mode_memory_test_words", "crc16", "unrolled_copy"))
memory.CODE_FIELDS = frozenset(("object.handler", "test_mode_routine"))
memory.CODE_TABLES = True
memory.NOTES = {
    "variables": ["/* the game's variables in work RAM, by their addresses (the fields of struct game, at the end);",
                  "   SCREEN_1 and SCREEN_2 are the blocks of the two screens, which the code reaches through screen",
                  "   (R25 of the task that runs each) */"],
    "state": ["/* The game's state: the work RAM (128 KB at 200000 on the board), laid out as the original had it,",
              "   so that each frame's state compares byte for byte with the original's. The code reaches it",
              "   through game: game->game_frame_count; the runtime keeps one (runtime/src/game.c). The stacks are",
              "   the V70's, which the decompiled program does not use */"],
    "host": ["/* the game's variables the code reaches only by name, in the host's memory (their places in the work",
             "   RAM, struct game, are where world_project puts them) */"],
    "scoped": ["/* work RAM as one family of functions uses it (their scratch variables) */"],
    "tables": ["/* data in the program ROM the code reads */"],
    "hardware": ["/* the board's registers (read and written through the bus: io_read8, io_write8...) */"],
    "board": ["/* the board's memories the code uses as plain memory (the video RAM, the sprite RAM, the RAM the",
              "   sound program shares), by name: lvalues through board() */"],
}
memory.PLAIN_HARDWARE = ("VIDEO", "SPRITE_RAM", "SOUND")
# the video RAM and the sprite RAM are 16-bit words in the host's order (runtime/include/s32/machine.h): a byte
# of the board's memories through board_byte, which finds its lane (game/support.h)
memory.BOARD_BYTE_FUNCTION = pointers.BOARD_BYTE_FUNCTION = "board_byte"
# the routines that work on memory by its address, whatever is there (noise_report.py: their mem_access is
# the original's): function -> why
MEMORY_ROUTINES = {
    "test_mode_memory_test_words": "the memory test: writes and reads back every word of a memory it is given",
    "test_mode_memory_test_nibbles": "the memory test: every halfword of a memory it is given, a nibble at a time",
    "test_mode_memory_test_bytes": "the memory test: every byte of a memory it is given",
    "crc16": "a checksum of the bytes at any address it is given",
    "unrolled_copy": "copies any number of bytes from any address to any other",
    "boot": "writes the register values at 0000A4 to E00000 and E80000, which this board does not map",
}
# the census of the program's access widths (width_census.py): the board's memories it looks at, as the bus
# decodes them (first address, past the last, its name, the size that repeats in that range; the program's data
# by rom.canonical_address), and the routines whose accesses say nothing of how the game lays out what they
# reach: the memory test writes and reads back every element at one width, and a copy moves the bytes in order
WIDTH_MEMORIES = ((0x200000, 0x300000, "work RAM", 0x20000), (0x300000, 0x400000, "video RAM", 0x20000),
                  (0x400000, 0x500000, "sprite RAM", 0x20000), (0x700000, 0x800000, "sound RAM", 0x2000))
WIDTH_ROUTINES = ("test_mode_memory_test_words", "test_mode_memory_test_nibbles", "test_mode_memory_test_bytes",
                  "unrolled_copy")
# (R27 and R28 are not: no function reads what another left in them; player_car_project_road could, were
# its projection pattern to skip every segment, but segment_projection_pattern starts with a 1)
signatures.CONTEXT = (12, 14, 20, 23, 25, 32)
signatures.INITIAL = {32: 0x21F400}
signatures.SCRATCH_CONTEXT = {12: 0xFFFF0000, 14: 0xFFFFFFFF}
signatures.CONSTANT = {24: 0x20E000}
constants.GLOBALS_BASE = 24
constants.GLOBALS_VALUE = 0x20E000
dataflow.HANDLER_SLOTS = (0x4,)
function.INTERRUPTS_BEFORE_POLL = True    # the board's events, and their interrupts, come before the poll
function.FRAME_COUNTER = "frame_ready"

# the sound program's board (z80/emit.py): its banked window and its sound chip
SOUND_HARDWARE = [(0xA000, 0xC000, "SAMPLE_BANK"), (0xC000, 0xE000, "MULTIPCM")]
SOUND_NOTES = {
    "variables": ["/* the sound program's variables, in the RAM it shares with the V70 (E000-FFFF) */"],
    "scoped": ["/* shared RAM as one family of functions uses it */"],
    "tables": ["/* data in the program ROM the code reads */"],
    "hardware": ["/* the banked page of the sound ROM and the MultiPCM (read and written through the bus) */"],
}
