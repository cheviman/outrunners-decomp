/**
 *    Name:        support.h
 *    Description: what the decompiled game program uses besides memory: the processor status, the frame
 *                 loop and its interrupts, calls by address, and the V60's string and bit-string moves
 *                 and searches
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's V60 core (src/devices/cpu/v60, Farfetch'd, R. Belmont) where the program
 *    leans on the processor: its status word, interrupt entry and exit, MOVCU/MOVCFU/MOVCD,
 *    MOVBSD/MOVBSU and SCH0BSU/SCH1BSU. The condition flags are variables of the decompiled functions;
 *    'psw' keeps the rest.
 */
#ifndef GAME_SUPPORT_H
#define GAME_SUPPORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "game/assets.h"
#include "s32/bus.h"
#include "s32/machine.h"
#include "s32/program.h"
#include "v60/arith.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The game's state is its work RAM, laid out as the original program had it, so it compares byte for byte
   with the original's: ram.h's struct game (written by tools/decompile.py), which the code reaches through
   game (game->game_frame_count) and runtime/src/game.c keeps, the board reading the same bytes. A place the
   code works out by its address is the byte there (ram()). The V60 is little endian and lets a halfword or
   word sit at any address, like the targets this runs on; the build turns strict aliasing off for this
   code. */
struct game;
extern struct game *game;
/* Functions that stop the game: the C compiler knows they do not come back */
#if defined(_MSC_VER)
#define GAME_NORETURN __declspec(noreturn)
#else
#define GAME_NORETURN _Noreturn
#endif

/* A game function the board puts its own in place of (runtime/replaced.txt): tools/decompile.py writes
   its definition as BOARD_ORIGINAL(name), which keeps the original as original_name */
#define BOARD_ORIGINAL(name) original_##name

/* a structure pointer that is not in work RAM: 0, no place (NULL, which address_of takes back to 0); in
   the program ROM (a link left at 0 in an object a scenario crafted), the bus reads the ROM there and drops
   writes, so it gets a copy of the ROM to read and write; anywhere else the program stops */
uint8_t *ram_outside(uint32_t address);

static inline uint8_t *ram(uint32_t address)
{
	if ((address & 0xf00000) != 0x200000)
		return ram_outside(address);
	return &((uint8_t *)game)[address & 0x1ffff];
}

/* The address the original holds for the place a pointer the C keeps points at: in the game's state, its
   work RAM address; in a memory of the board, its address there; in a copy of the ROM ram_outside made,
   the address it copied from; none (NULL), 0. And the pointer the C keeps where the original holds an
   address: a place of the game's state or of the board's plain memories, a copy of the ROM, none for 0 */
uint32_t address_outside(const void *pointer);
void *pointer_to(uint32_t address);

static inline uint32_t address_of(const void *pointer)
{
	const uintptr_t offset = (uintptr_t)pointer - (uintptr_t)game;
	if (offset < 0x20000u)
		return 0x200000u + (uint32_t)offset;
	return address_outside(pointer);
}

#define RAM8(address)   (*(uint8_t *)ram(address))
#define RAM16(address)  (*(uint16_t *)ram(address))
#define RAM32(address)  (*(uint32_t *)ram(address))
#define RAM8S(address)  (*(int8_t *)ram(address))
#define RAM16S(address) (*(int16_t *)ram(address))
#define RAM32S(address) (*(int32_t *)ram(address))

/* The game's data (game/assets.h): its tables by name, NAME[i]; through an address the game holds,
   data_at(address), which stops the program where no piece of the data is (data_outside). A pointer the
   code sets to its data can hold a work RAM address instead (a list the game was poked to read from RAM):
   the read goes there, as the bus would send it */
GAME_NORETURN void data_outside(uint32_t address);

/* The board's memories the code uses as plain memory (symbols.txt "struct NAME SIZE board", "elements NAME TYPE
   board"), as the bus has them: the video RAM (0x300000), the sprite RAM (0x400000) and the RAM the sound
   program shares (0x700000); a pointer into one of them that the game points at the work RAM or the
   program's chips instead reads those, as the bus does (the road rows' last sprites: the sprite list,
   the one being put together in the work RAM, or 0x100000 when there are none). A structure of the
   board's anywhere else stops the program */
GAME_NORETURN void board_outside(uint32_t address);

static inline uint8_t *board(uint32_t address)
{
	switch ((address >> 20) & 0xf)
	{
	case 0x0:
	case 0x1:
		return (uint8_t *)data_at(address);
	case 0x2:
		return ram(address);
	case 0x3:
		return &s32_board.video_ram[address & (S32_VIDEO_RAM_SIZE - 1)];
	case 0x4:
		return &s32_board.sprite_ram[address & (S32_SPRITE_RAM_SIZE - 1)];
	case 0x7:
		return &s32_board.shared_ram[address & (S32_SHARED_RAM_SIZE - 1)];
	default:
		board_outside(address);
	}
}

/* the board's variables in those memories (the "io" lines of symbols.txt there): lvalues, as the work RAM's */
#define BOARD8(address)   (*(uint8_t *)board(address))
#define BOARD16(address)  (*(uint16_t *)board(address))
#define BOARD32(address)  (*(uint32_t *)board(address))
#define BOARD8S(address)  (*(int8_t *)board(address))
#define BOARD16S(address) (*(int16_t *)board(address))
#define BOARD32S(address) (*(int32_t *)board(address))

/* A register of a device of the board (the I/O chips, the interrupt controller, the link board, the
   palettes and mixers...), by its name: the bus hands it the read or the write */
#define io_read8(address)          mem_read8(address)
#define io_read16(address)         mem_read16(address)
#define io_read32(address)         mem_read32(address)
#define io_write8(address, data)   mem_write8(address, data)
#define io_write16(address, data)  mem_write16(address, data)
#define io_write32(address, data)  mem_write32(address, data)

#define DATA8(address)  (*(const uint8_t *)data_at(address))
#define DATA16(address) (*(const uint16_t *)data_at(address))
#define DATA32(address) (*(const uint32_t *)data_at(address))
#define DATA8S(address)  (*(const int8_t *)data_at(address))
#define DATA16S(address) (*(const int16_t *)data_at(address))
#define DATA32S(address) (*(const int32_t *)data_at(address))

/* A table read with another width than its elements': 'offset' bytes into it */
#define TABLE8(table, offset)  (*(const uint8_t *)((const uint8_t *)table##_data + (offset)))
#define TABLE16(table, offset) (*(const uint16_t *)((const uint8_t *)table##_data + (offset)))
#define TABLE32(table, offset) (*(const uint32_t *)((const uint8_t *)table##_data + (offset)))

/* The game links its objects by the low halfword of their addresses: the object 'link' names, in the
   page of work RAM 'base' points into */
#define LINKED(base, link) (((base) & 0xFFFF0000u) | (link))
/* a place 'offset' bytes on from 'base' in its 64 KB page: the original adds them in 16 bits, which wrap
   there (the ring of the road's segments at screen + 0x2000u) */
#define PAGE_ADD(base, offset) (((base) & 0xFFFF0000u) | (uint16_t)((base) + (offset)))

/* A place in the 256-byte page 'base' points into, 'low' bytes from its start (another port of the I/O chip
   'base' points at) */
#define IN_PAGE(base, low) (((base) & ~0xFFu) | (low))

/* the board's memories (addresses the code computes into them) */
#define WORK_RAM          0x200000u
#define PLAYER_BLOCK(number) (0x208400u + ((number) - 1u) * 0x200u)    /* players 1-3: struct player */
#define VIDEO_RAM         0x300000u
#define TEXT_LAYER        0x31F000u    /* the text layer: 64 x 32 cells of a halfword (MAME segas32_v) */
#define TEXT_CELL(column, row) (TEXT_LAYER + ((row) * 64u + (column)) * 2u)
#define SPRITE_RAM        0x400000u
#define SPRITE_CONTROL    0x500000u
#define PALETTE_RAM       0x600000u
#define MIXER_REGISTERS   0x610000u
#define SOUND_RAM         0x700000u
#define IO_PORTS          0xC00000u
#define INTERRUPT_CONTROL 0xD00000u
#define RANDOM_GENERATOR  0xD80000u

/* A variable read with another width than its own, or a part of one */
#define AS8(variable)  (*(uint8_t *)&(variable))
#define AS16(variable) (*(uint16_t *)&(variable))
#define AS32(variable) (*(uint32_t *)&(variable))
#define AT8(variable, offset)  (*(uint8_t *)((uint8_t *)&(variable) + (offset)))
#define AT16(variable, offset) (*(uint16_t *)((uint8_t *)&(variable) + (offset)))
#define AT32(variable, offset) (*(uint32_t *)((uint8_t *)&(variable) + (offset)))

/* Processor status word: interrupt enable (bit 18), stacks and levels; the flags (bits 0-3) are local
   variables of the code, merged in when the program reads the whole word */
extern uint32_t psw;

#define PSW_INTERRUPTS (1u << 18)    /* maskable interrupts enabled */

uint32_t psw_read(uint32_t z, uint32_t s, uint32_t ov, uint32_t cy);
void psw_write(uint32_t value);
uint32_t psw_update(uint32_t current, uint32_t value, uint32_t mask);   /* UPDPSW; gives the new word */

/* The program turns the interrupts off around what they must not cut in on (the EEPROM, the protection) */
static inline void interrupts_off(void)
{
	psw_update(psw, 0, PSW_INTERRUPTS);
}

static inline void interrupts_on(void)
{
	psw_update(psw, PSW_INTERRUPTS, PSW_INTERRUPTS);
}

/* Arithmetic the code does with values it computes, the way the game's processor does it:
   - a division by 0 leaves the dividend as it was, and so does the one quotient that does not fit (the
     lowest number divided by -1); by a constant the C divides with / and %;
   - a shift or rotation by a computed count takes the count's low byte as signed: left when positive,
     right when negative; 'bits' is the width of the value */
static inline uint32_t divide_s8(uint32_t a, uint32_t b) { return v60_div_value8(a, b); }
static inline uint32_t divide_s16(uint32_t a, uint32_t b) { return v60_div_value16(a, b); }
static inline uint32_t divide_s32(uint32_t a, uint32_t b) { return v60_div_value32(a, b); }
static inline uint32_t divide_u8(uint32_t a, uint32_t b) { return v60_divu_value8(a, b); }
static inline uint32_t divide_u16(uint32_t a, uint32_t b) { return v60_divu_value16(a, b); }
static inline uint32_t divide_u32(uint32_t a, uint32_t b) { return v60_divu_value32(a, b); }
static inline uint32_t remainder_s8(uint32_t a, uint32_t b) { return v60_rem_value8(a, b); }
static inline uint32_t remainder_s16(uint32_t a, uint32_t b) { return v60_rem_value16(a, b); }
static inline uint32_t remainder_s32(uint32_t a, uint32_t b) { return v60_rem_value32(a, b); }
static inline uint32_t remainder_u8(uint32_t a, uint32_t b) { return v60_remu_value8(a, b); }
static inline uint32_t remainder_u16(uint32_t a, uint32_t b) { return v60_remu_value16(a, b); }
static inline uint32_t remainder_u32(uint32_t a, uint32_t b) { return v60_remu_value32(a, b); }
static inline uint32_t shift_by(uint32_t value, uint32_t count, int bits) { return v60_shl_value(value, count, bits); }
static inline uint32_t shift_signed_by(uint32_t value, uint32_t count, int bits) { return v60_sha_value(value, count, bits); }
static inline uint32_t rotate_by(uint32_t value, uint32_t count, int bits) { return v60_rot_value(value, count, bits); }
static inline uint32_t rotate_with_carry_by(uint32_t value, uint32_t count, uint32_t carry, int bits)
{
	return v60_rotc_value(value, count, carry, bits);
}
static inline uint32_t byte_swap32(uint32_t value) { return v60_byte_swap(value); }

/* LDPR: the privileged registers (the system base register says where the interrupt vectors are) */
void load_privileged(uint32_t number, uint32_t value);

/* The frame loop: while the program waits for a frame (polling 'frame_ready', which an interrupt sets), the
   board runs its next event; then the program takes the interrupts that event raised; when the wait is
   over, a frame is done */
void frame_wait(uint32_t frame_ready);
void take_interrupts(void);
void frame_end(void);

/* The game's two tasks: the VBLANK handler asks for the other one (03F5FC), and the runtime takes it up
   when the handler is done, as the V70 returns into it. Each task keeps its own copy of the registers the
   whole program shares (game_context, written by tools/decompile.py) */
void swap_tasks(void);
typedef struct
{
	void *variable;     /* the C's global for it */
	uint8_t pointer;    /* a pointer into the game's state, where the original holds its address; else a
	                       32-bit number */
} game_register;
extern const game_register game_context[];
extern const uint8_t game_context_registers[];  /* the V70 register each one is */
extern const size_t game_context_count;

/* HALT: the V60 stops until an interrupt comes. MAME's core goes on with the next instruction at once,
   and so does the game here (the program's only halts start int_default and int_vector_02, the handlers
   of the interrupts it does not expect, where two checks for states that should not happen jump too:
   build_sprite_list's and delete_object_and_children's) */
static inline void halt(void)
{
}

/* Calls through addresses the program keeps in memory (object handlers, interrupt hooks) */
typedef struct game_code_entry
{
	uint32_t address;
	void (*function)(void);
} game_code_entry;

extern const game_code_entry game_code[];      /* sorted by address; written by tools/decompile.py */
extern const size_t game_code_count;
void game_entry(void);                          /* the code at the reset vector */

void call_code(uint32_t address);
GAME_NORETURN void unknown_code(uint32_t address);    /* a jump or call to an address no code was found at */
/* the default of a switch on a jump table's index: the original checks nothing, and an index past the table
   would jump to 'address', which no case handles */
GAME_NORETURN void invalid_jump(uint32_t address);
GAME_NORETURN void invalid_operand(uint32_t address); /* an instruction the V60 cannot run (code the game never calls) */

/* With tools/decompile.py --trace, every function calls game_trace when it starts (the calls go to stderr)
   and every block of code game_trace_block (the V60 addresses of the blocks that ran are written to
   blocks_run.txt when the program ends) */
void game_trace(uint32_t address);
void game_trace_block(uint32_t address);

/* MOVCU / MOVCFU / MOVCD: 'fill' is the fill value (R26); they leave in 'dest_end' and 'source_end'
   (R27, R28) the addresses past the last element moved */
void move_string(uint32_t source, uint32_t source_length, uint32_t dest, uint32_t dest_length, uint32_t element_size,
                 uint32_t fill_rest, uint32_t downward, uint32_t fill, uint32_t *dest_end, uint32_t *source_end);

/* the string moves the game makes most, when nothing reads the end addresses: 'count' elements copied
   from the first one up, or from the last one down (MOVCD), and 'count' elements set to 'value' */
void move_bytes(uint32_t dest, uint32_t source, uint32_t count);
void move_halfwords(uint32_t dest, uint32_t source, uint32_t count);
void move_bytes_backward(uint32_t dest, uint32_t source, uint32_t count);
void move_halfwords_backward(uint32_t dest, uint32_t source, uint32_t count);
void set_bytes(uint32_t dest, uint32_t count, uint32_t value);
void set_halfwords(uint32_t dest, uint32_t count, uint32_t value);

/* MOVBSD / MOVBSU: bit strings, the offsets already split into byte address and bit */
void move_bits_down(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length,
                    uint32_t *dest_end, uint32_t *source_end);
void move_bits_up(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length,
                  uint32_t *dest_end, uint32_t *source_end);

/* SCH0BSU / SCH1BSU: how many of the 'length' bits up from the bit string's start come before the first
   one equal to 'bit' ('length' when none is); 'work' (R28) is left at the byte of the last bit looked at */
uint32_t search_bits_up(uint32_t address, uint32_t offset, uint32_t length, uint32_t bit, uint32_t *work);
/* SCHCU.B / SKPCU.B: the first of 'length' bytes from 'address' that is (until_equal) or is not 'value';
   its index goes to *index ('length' when there is none) and its address to *found */
void search_bytes(uint32_t address, uint32_t length, uint32_t value, uint32_t until_equal, uint32_t *index,
	uint32_t *found);

/* DIVX / DIVUX into a register pair: the 64-bit dividend is high:low, the quotient goes to low and the
   remainder to high */
void divx_pair(uint32_t divisor, uint32_t *low, uint32_t *high);
void divux_pair(uint32_t divisor, uint32_t *low, uint32_t *high);

#ifdef __cplusplus
}
#endif

/* where the C never goes: past a switch whose every case returns, on an index the original's table does not
   have, which it never gives (C23's unreachable()) */
#ifndef unreachable
#if defined(_MSC_VER)
#define unreachable() __assume(0)
#else
#define unreachable() __builtin_unreachable()
#endif
#endif

#endif
