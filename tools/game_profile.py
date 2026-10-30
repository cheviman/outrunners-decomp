"""
Name:        game_profile.py
Description: what the shared decompiler core (tools/core) needs to know of OutRunners: where the game is,
             its ROM set, and its settings
Version:     0.1.0
Created:     25-10-2026
Modified:    30-10-2026
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
                       (0x20FB00, "races", "race", 2), (0x20FC00, "race_copies", "race", 8),
                       (0x210000, "object_area_2", "object_area", 1), (0x218000, "screen_2", "screen", 1),
                       (0x21E000, "isdn_races", "race", 8), (0x21F000, "stacks", None, 0x1000))
memory.NOTES = {
    "variables": ["/* the game's variables in work RAM, by their addresses (the fields of struct game, at the end);",
                  "   SCREEN_1 and SCREEN_2 are the blocks of the two screens, which the code reaches through screen",
                  "   (R25 of the task that runs each) */"],
    "state": ["/* The game's state: the work RAM (128 KB at 200000 on the board), laid out as the original had it,",
              "   so that each frame's state compares byte for byte with the original's. The code reaches it",
              "   through game: game->game_frame_count; the runtime keeps one (runtime/src/game.c). The stacks are",
              "   the V70's, which the decompiled program does not use */"],
    "scoped": ["/* work RAM as one family of functions uses it (their scratch variables) */"],
    "tables": ["/* data in the program ROM the code reads */"],
    "hardware": ["/* the board's registers (read and written through the bus: io_read8, io_write8...) */"],
    "board": ["/* the board's memories the code uses as plain memory (the video RAM, the sprite RAM, the RAM the",
              "   sound program shares), by name: lvalues through board() */"],
}
memory.PLAIN_HARDWARE = ("VIDEO", "SPRITE_RAM", "SOUND")
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
