/**
 *    Name:        sound.c
 *    Description: the Z80's side of the Multi 32 sound board: its memory, its I/O ports and its
 *                 interrupt controller
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's sega/segas32.cpp (Aaron Giles): the Multi 32's sound maps and handlers.
 *    MAME's terms are in tools/reference/mame/LICENSE.
 *
 *    Memory: 0x0000-0x9FFF the start of the sound ROM; 0xA000-0xBFFF an 8 KB page of it, picked by the
 *    bank port; 0xC000-0xDFFF the MultiPCM; 0xE000-0xFFFF the RAM it shares with the V70 (0x700000
 *    there).
 *    Ports: 0x80-0x8F the YM3438; 0xA0-0xAF the bank; 0xB0-0xBF the MultiPCM's banks; 0xC0-0xCF
 *    acknowledge interrupts (odd ports) and interrupt the V70 (bit 2); 0xD0-0xD7 map the inputs to the
 *    vectors (0xD0-0xD2) and mask them (0xD3); 0xF1 a byte that reads back. The rest read 0xFF.
 */
#include "internal.h"

/* The Z80 interrupt line follows the highest vector with an unmasked input pending; vector N puts
   2 x N on the bus */
static void update_sound_irq(void)
{
	const uint8_t pending = s32_board.sound_irq_input & ~s32_board.sound_irq_control[3] & 0x07;
	int vector = 0;

	while (vector < 3 && !(pending & (1u << vector)))
		vector++;
	if (s32_board.sound_irq_changed)
		s32_board.sound_irq_changed(vector < 3, (uint8_t)(2 * (vector < 3 ? vector : 0)));
}

void s32_sound_signal_irq(int input)
{
	for (int vector = 0; vector < 3; vector++)
		if (s32_board.sound_irq_control[vector] == input)
			s32_board.sound_irq_input |= (uint8_t)(1u << vector);
	update_sound_irq();
}

void s32_sound_clear_irq(int input)
{
	for (int vector = 0; vector < 3; vector++)
		if (s32_board.sound_irq_control[vector] == input)
			s32_board.sound_irq_input &= (uint8_t)~(1u << vector);
	update_sound_irq();
}

uint8_t s32_z80_read(uint16_t address)
{
	if (address < 0xa000)
		return s32_board.sound_rom[address];
	if (address < 0xc000)
		return s32_board.sound_rom[((uint32_t)s32_board.sound_bank * 0x2000 + (address & 0x1fff)) % S32_SOUND_ROM_SIZE];
	if (address < 0xe000)
		return s32_board.chips.pcm_read(address & 0x1fff);
	return s32_board.shared_ram[address & 0x1fff];
}

void s32_z80_write(uint16_t address, uint8_t data)
{
	if (address < 0xc000)
		return;
	if (address < 0xe000)
		s32_board.chips.pcm_write(address & 0x1fff, data);
	else
		s32_board.shared_ram[address & 0x1fff] = data;
}

uint8_t s32_z80_in(uint8_t port)
{
	if ((port & 0xf0) == 0x80)
		return s32_board.chips.fm_read(port & 3);
	return port == 0xf1 ? s32_board.sound_spare : 0xff;
}

/* The MultiPCM's 4 MB in its 2 MB of address space: 0x000000-0x0FFFFF fixed, then two 512 KB windows,
   each on the page its bank picks */
uint8_t s32_pcm_sample_read(uint32_t address)
{
	uint32_t page;

	address &= 0x1fffff;
	if (address < 0x100000)
		return s32_board.pcm_rom[address];
	page = (address < 0x180000) ? (s32_board.pcm_bank & 7u) : ((s32_board.pcm_bank >> 3) & 7u);
	return s32_board.pcm_rom[(page * 0x80000 + (address & 0x7ffff)) % S32_PCM_ROM_SIZE];
}

void s32_z80_out(uint8_t port, uint8_t data)
{
	switch (port & 0xf0)
	{
		case 0x80:
			s32_board.chips.fm_write(port & 3, data);
			break;
		case 0xa0:
			s32_board.sound_bank = data & 0x3f;
			break;
		case 0xb0:
			s32_board.pcm_bank = data;
			break;
		case 0xc0:
			if (port & 1)
			{
				s32_board.sound_irq_input &= data;
				update_sound_irq();
			}
			if (port & 4)
				s32_signal_irq(S32_IRQ_SOUND);
			break;
		case 0xd0:
			if (!(port & 8))
			{
				s32_board.sound_irq_control[port & 3] = data;
				update_sound_irq();
			}
			break;
		default:
			if (port == 0xf1)
				s32_board.sound_spare = data;
			break;
	}
}
