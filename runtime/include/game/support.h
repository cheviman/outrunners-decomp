/**
 *    Name:        support.h
 *    Description: what the decompiled game program uses besides memory: the processor status, the frame
 *                 loop and its interrupts, calls by address, and the V60's string and bit-string moves
 *                 and searches
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    15-10-2026
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

#include "s32/bus.h"
#include "s32/machine.h"
#include "s32/program.h"
#include "v60/arith.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Work RAM holds the game's variables, at the addresses the original program used, so its state compares
   byte for byte with the original. A variable is an lvalue of its type at its address; ram.h (written
   by tools/decompile.py) names them. The V60 is little endian and lets a halfword or word sit at any
   address, like the targets this runs on; the build turns strict aliasing off for this code. */
/* Functions that stop the game: the C compiler knows they do not come back */
#if defined(_MSC_VER)
#define GAME_NORETURN __declspec(noreturn)
#else
#define GAME_NORETURN _Noreturn
#endif

/* a structure pointer that is not in work RAM: in the program ROM (a link left at 0 in an object a
   scenario crafted), the bus reads the ROM there and drops writes, so it gets a copy of the ROM to read
   and write; anywhere else the program stops */
uint8_t *ram_outside(uint32_t address);

static inline uint8_t *ram(uint32_t address)
{
	if ((address & 0xf00000) != 0x200000)
		return ram_outside(address);
	return &s32_board.work_ram[address & 0xffff];
}

#define RAM8(address)   (*(uint8_t *)ram(address))
#define RAM16(address)  (*(uint16_t *)ram(address))
#define RAM32(address)  (*(uint32_t *)ram(address))
#define RAM8S(address)  (*(int8_t *)ram(address))
#define RAM16S(address) (*(int16_t *)ram(address))
#define RAM32S(address) (*(int32_t *)ram(address))

/* The program ROM, which the game's code also reads as data (tables): the user's ROM image, as the board
   maps it (the first 2 MB, and the first 1 MB again from 0xF00000). A pointer the code sets to the ROM
   can hold a work RAM address instead (a list the game was poked to read from RAM): the read goes there,
   as the bus would send it */
GAME_NORETURN void rom_outside(uint32_t address);

static inline const uint8_t *rom(uint32_t address)
{
	if ((address & 0xf00000) == 0xf00000)
		return &s32_board.program[address & 0xfffff];
	if ((address & 0xffffff) < 0x200000)
		return &s32_board.program[address & 0x1fffff];
	if ((address & 0xf00000) != 0x200000)
		rom_outside(address);
	return ram(address);
}

#define ROM8(address)  (*(const uint8_t *)rom(address))
#define ROM16(address) (*(const uint16_t *)rom(address))
#define ROM32(address) (*(const uint32_t *)rom(address))
#define ROM8S(address)  (*(const int8_t *)rom(address))
#define ROM16S(address) (*(const int16_t *)rom(address))
#define ROM32S(address) (*(const int32_t *)rom(address))

/* The game links its objects by the low halfword of their addresses: the object 'link' names, in the
   page of work RAM 'base' points into */
#define LINKED(base, link) (((base) & 0xFFFF0000u) | (link))

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

/* The frame loop: while the program waits for a frame, the board runs its next event; then the
   program takes the interrupts that event raised; when the wait is over, a frame is done */
void frame_wait(uint32_t frames_to_wait);
void take_interrupts(void);
void frame_end(void);

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

/* DIVX / DIVUX into a register pair: the 64-bit dividend is high:low, the quotient goes to low and the
   remainder to high */
void divx_pair(uint32_t divisor, uint32_t *low, uint32_t *high);
void divux_pair(uint32_t divisor, uint32_t *low, uint32_t *high);

#ifdef __cplusplus
}
#endif

#endif
