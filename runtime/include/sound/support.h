/**
 *    Name:        support.h
 *    Description: what the decompiled sound program uses besides its own C: its memory, the board's ports,
 *                 and the sound board's timing model
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    15-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The program's memory is the board's (s32/machine.h): SOUND_RAM8/16 is the RAM the Z80 shares with the
 *    V60 (Z80 E000-FFFF), SOUND_ROM8/16 the sound ROM's first 40 KB, and a computed address goes over the
 *    Z80's bus (z80_read8 and the rest: the banked page, the RF5C68). port_in and port_out are its I/O
 *    instructions.
 *
 *    The timing model (runtime/src/sound_cpu.c): every basic block of the original starts with
 *    z80_block(cycles). There the board runs what is due by the Z80's time (the YM3438 timers) and the Z80
 *    takes an interrupt if one is pending and it lets them in; then the block's accesses happen at that
 *    time, and its cycles pass. z80_cycles() adds the extra cycles of a condition that takes the longer
 *    way. EI that ends a block lets interrupts in from the block after the next start, as the Z80 waits
 *    one instruction.
 */
#ifndef SOUND_SUPPORT_H
#define SOUND_SUPPORT_H

#include "s32/machine.h"

#include <stdbool.h>
#include <stdint.h>

#define SOUND_RAM8(address)  (s32_board.shared_ram[(address) & 0x1fff])
#define SOUND_RAM16(address) (*(uint16_t *)&s32_board.shared_ram[(address) & 0x1fff])
#define SOUND_ROM8(address)  (s32_board.sound_rom[(address)])
#define SOUND_ROM16(address) ((uint16_t)(SOUND_ROM8(address) | SOUND_ROM8((address) + 1) << 8))
#define SOUND_RAM8S(address)  (*(int8_t *)&SOUND_RAM8(address))
#define SOUND_RAM16S(address) (*(int16_t *)&SOUND_RAM8(address))

/* Where an address of the shared RAM or the program ROM is: the program's arrays and structures
   (sound.h) are laid over these bytes, so the state compares byte for byte with the reference */
#define sound_ram_at(address) ((void *)&s32_board.shared_ram[(address) & 0x1fff])
#define sound_rom_at(address) ((const void *)&s32_board.sound_rom[(address)])

/* A part of a variable or field read on its own: the variable's first bytes, or bytes further in */
#define AS8(variable)  (*(uint8_t *)&(variable))
#define AS16(variable) (*(uint16_t *)&(variable))
#define AT8(variable, offset)  (*(uint8_t *)((uint8_t *)&(variable) + (offset)))
#define AT16(variable, offset) (*(uint16_t *)((uint8_t *)&(variable) + (offset)))

uint8_t z80_read8(uint16_t address);
uint16_t z80_read16(uint16_t address);
void z80_write8(uint16_t address, uint8_t value);
void z80_write16(uint16_t address, uint16_t value);
uint8_t port_in(uint8_t port);
void port_out(uint8_t port, uint8_t value);

/* LDIR and LDDR: 'count' bytes (0 is 65536) from 'source' to 'destination', upward or downward; every
   round but the last takes 21 cycles more */
void move_block(uint16_t source, uint16_t destination, uint16_t count, bool upward);

/* A jump through a table to an entry the table does not have: the original would run whatever is there */
void sound_invalid_jump(uint32_t address);    /* the default of a switch on a jump table's index */

void z80_block(int cycles);
void z80_cycles(int cycles);
void interrupts_on(void);
void interrupts_on_after_block(void);
void interrupts_off(void);

/* 1 when the byte has an even number of bits set: the Z80's P/V after logic */
static inline uint32_t parity8(uint32_t value)
{
	value &= 0xff;
	value ^= value >> 4;
	value ^= value >> 2;
	value ^= value >> 1;
	return (~value) & 1u;
}

/* The flags of a + b + carry and a - b - borrow at 8 and 16 bits, where the program keeps them */
static inline uint32_t z80_carry_add8(uint32_t a, uint32_t b, uint32_t c) { return ((a & 0xff) + (b & 0xff) + c) >> 8 & 1u; }
static inline uint32_t z80_carry_sub8(uint32_t a, uint32_t b, uint32_t c) { return ((a & 0xff) - (b & 0xff) - c) >> 8 & 1u; }
static inline uint32_t z80_carry_add16(uint32_t a, uint32_t b, uint32_t c) { return ((a & 0xffff) + (b & 0xffff) + c) >> 16 & 1u; }
static inline uint32_t z80_carry_sub16(uint32_t a, uint32_t b, uint32_t c) { return ((a & 0xffff) - (b & 0xffff) - c) >> 16 & 1u; }
static inline uint32_t z80_overflow_add8(uint32_t a, uint32_t b, uint32_t c)
{
	const uint32_t result = a + b + c;
	return ((a ^ result) & (b ^ result)) >> 7 & 1u;
}
static inline uint32_t z80_overflow_sub8(uint32_t a, uint32_t b, uint32_t c)
{
	const uint32_t result = a - b - c;
	return ((a ^ b) & (a ^ result)) >> 7 & 1u;
}
static inline uint32_t z80_overflow_add16(uint32_t a, uint32_t b, uint32_t c)
{
	const uint32_t result = a + b + c;
	return ((a ^ result) & (b ^ result)) >> 15 & 1u;
}
static inline uint32_t z80_overflow_sub16(uint32_t a, uint32_t b, uint32_t c)
{
	const uint32_t result = a - b - c;
	return ((a ^ b) & (a ^ result)) >> 15 & 1u;
}

#endif
