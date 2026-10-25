/**
 *    Name:        game_00.c
 *    Description: the game's functions, part 00 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The reset's code (000006): SBR to the vector table, the stack 21FC00, 16 bytes from 0000A4 to E00000
   and 4 from 0000B4 to E80000 (which the board does not map), the interrupt controller's registers from
   0000B8, privilege level 0 with the stack 21F400 and a delay; then clears 21E000-21E3FF, turns the link
   board off (801000), clears 21F000-21FFFF, sets the I/O chips up (init_io_chips), clears the game's
   variables (20E000-20FFFF), writes the second task's first interrupt frame (000C98 to 21FF84: its PC
   0006B0) and its stack pointers (unk_20E000, waiting_task_stack), initialise_game, lcd_start (the LCD),
   the interrupts on, the link's check (network_check), and goes on as the boot task's frame loop
   (second_task(1), screen 2's). */
void boot(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0, r11 = 0;
	uint32_t stack_100004 = 0;
	uint32_t psw1 = 0;

	load_privileged(5u, vector_table_ADDRESS);
	r10 = 0xA4u;
	r11 = 0xE00000u;
	r1 = 8u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10++;
		r11++;
		mem_write8(r11, mem_read8(r10));
		r1--;
		r10++;
		r11++;
	} while (r1 != 0u);
	mem_write8(0xE80000u, rom_0000B4);
	mem_write8(0xE80001u, rom_0000B5);
	mem_write8(0xE80002u, rom_0000B6);
	mem_write8(0xE80003u, rom_0000B7);
	r11 = INTERRUPT_CONTROL;
	r1 = 6u;
	r10 = 0xB8u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10++;
		r11++;
		mem_write8(r11, mem_read8(r10));
		r1--;
		r10++;
		r11++;
	} while (r1 != 0u);
	stack_100004 = 0u;
	psw1 = stack_100004;
	psw_write(psw1);
	load_privileged(2u, 0x210000u);
	load_privileged(3u, 0x210000u);
	load_privileged(4u, 0x210000u);
	r0 = 0x10000u;
	do
	{
		r0--;
	} while (r0 != 0u);
	r0 = 0u;
	r1 = 0u;
	r2 = 0x20u;
	r10 = 0x21E000u;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	mem_write32(0x801000u, 0u);
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	r0 = 0u;
	r1 = 0u;
	r10 = 0x21F000u;
	r2 = 0x80u;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	init_io_chips();
	r0 = 0u;
	r1 = 0u;
	r2 = 0x100u;
	r10 = unk_20E000_ADDRESS;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
	player_car = 0x219000u;
	unk_20E000 = 0x21FF84u;
	waiting_task_stack = 0x21F800u;
	initialise_game();
	lcd_start();
	mem_write8(IRQ_D00007, 0u);
	mem_write8(IRQ_D00005, 0u);
	interrupts_on();
	network_check();
	second_task(1u);
}

/* Returns the CRC-16 (polynomial 1021) of the R11 bytes at R10, worked out in the top half of a 32-bit
   register that starts at DEBDEB00, each byte entering its low byte, with eight more shifts after the
   last. Used for the EEPROM's settings (load_settings, eeprom_write_settings) and the program's
   signature (signature_checksum). */
uint32_t crc16(uint32_t r10, uint32_t r11)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0;

	r2 = 0x10210000u;
	r0 = 0xDEBDEB00u;
	do
	{
		r0 = (r0 & ~0xFFu) | mem_read8(r10);
		r1 = 8u;
		r10++;
		do
		{
			carry = r0 * 2u < r0;
			r0 = r0 * 2u;
			if (carry)
				r0 ^= r2;
			r1--;
		} while (r1 != 0u);
		r11--;
	} while (r11 != 0u);
	r1 = 8u;
	do
	{
		carry = r0 * 2u < r0;
		r0 = r0 * 2u;
		if (carry)
			r0 ^= r2;
		r1--;
	} while (r1 != 0u);
	r0 = r0 >> 16;
	return r0;
}

/* Returns the crc16 of the program's 48-byte signature text at 00010C (OUTRUNNERS EUROPA and its build
   date, 93.4.21), which the EEPROM keeps in settings_signature so that a new program is noticed
   (load_settings). */
uint32_t signature_checksum(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = 0x10Cu;
	r11 = 0x30u;
	return crc16(r10, r11);
}

/* Loads the settings from the EEPROM at start-up (eeprom_read_settings into 20E600-20E67F) and repairs
   them: a block marked SEGA gets the defaults (0004C0, marked S32A) with eeprom_write_count 100; one
   without the S32A mark the defaults; with it, a new program (signature_checksum unlike
   settings_signature) gets the defaults keeping the eeprom_write_count of a sound copy (the settings or
   their backup, each checked with its crc16); with the same program a bad copy (or a backup unlike the
   settings) is rewritten from the other, both bad the defaults. Every repair but rewriting a copy also
   resets the backup RAM's data (bookkeeping_reset); each one saves the result (eeprom_write_settings). */
void load_settings(uint32_t r0)
{
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r10 = 0, r11 = 0;

	eeprom_read_settings(r0);
	if (settings_magic == 0x41474553u)
	{
		bookkeeping_reset(r0);
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
		eeprom_write_count = 0x64u;
		backup_eeprom_write_count = 0x64u;
		r0_2 = signature_checksum();
		settings_signature = r0_2;
		r10 = eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0_2 = crc16(r10, r11);
		settings_checksum = r0_2;
		backup_settings_checksum = r0_2;
		eeprom_write_settings();
		return;
	}
	if (settings_magic != 0x41323353u)
	{
		bookkeeping_reset(r0);
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
		r0_2 = signature_checksum();
		settings_signature = r0_2;
		r10 = eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0_2 = crc16(r10, r11);
		settings_checksum = r0_2;
		backup_settings_checksum = r0_2;
		eeprom_write_settings();
		return;
	}
	r0_2 = signature_checksum();
	if (settings_signature != (uint16_t)r0_2 || settings_signature != (uint16_t)r0_2)
	{
		settings_signature = r0_2;
		bookkeeping_reset(r0_2);
		r10 = eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0_2 = crc16(r10, r11);
		r10 = settings_checksum_ADDRESS;
		if (settings_checksum == (uint16_t)r0_2)
		{
			r0_3 = mem_read16(r10 + 2u);
			move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
			eeprom_write_count = r0_3;
			backup_eeprom_write_count = r0_3;
			r0_2 = signature_checksum();
			settings_signature = r0_2;
			r10 = eeprom_write_count_ADDRESS;
			r11 = 0x3Au;
			r0_2 = crc16(r10, r11);
			settings_checksum = r0_2;
			backup_settings_checksum = r0_2;
			eeprom_write_settings();
			return;
		}
		r10 = backup_eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0 = crc16(r10, r11);
		r10 = backup_settings_checksum_ADDRESS;
		if (backup_settings_checksum != (uint16_t)r0)
		{
			bookkeeping_reset(r0);
			move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
			r0_2 = signature_checksum();
			settings_signature = r0_2;
			r10 = eeprom_write_count_ADDRESS;
			r11 = 0x3Au;
			r0_2 = crc16(r10, r11);
			settings_checksum = r0_2;
			backup_settings_checksum = r0_2;
			eeprom_write_settings();
			return;
		}
		r0_3 = mem_read16(r10 + 2u);
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
		eeprom_write_count = r0_3;
		backup_eeprom_write_count = r0_3;
		r0_2 = signature_checksum();
		settings_signature = r0_2;
		r10 = eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0_2 = crc16(r10, r11);
		settings_checksum = r0_2;
		backup_settings_checksum = r0_2;
		eeprom_write_settings();
		return;
	}
	r10 = eeprom_write_count_ADDRESS;
	r11 = 0x3Au;
	r0_2 = crc16(r10, r11);
	if (settings_checksum != (uint16_t)r0_2)
	{
		r10 = backup_eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0 = crc16(r10, r11);
		if (backup_settings_checksum == (uint16_t)r0)
		{
			move_halfwords(settings_checksum_ADDRESS, backup_settings_checksum_ADDRESS, 0x1Eu);
			eeprom_write_settings();
			return;
		}
		bookkeeping_reset(r0);
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
		r0_2 = signature_checksum();
		settings_signature = r0_2;
		r10 = eeprom_write_count_ADDRESS;
		r11 = 0x3Au;
		r0_2 = crc16(r10, r11);
		settings_checksum = r0_2;
		backup_settings_checksum = r0_2;
		eeprom_write_settings();
		return;
	}
	r10 = backup_eeprom_write_count_ADDRESS;
	r11 = 0x3Au;
	r0_2 = crc16(r10, r11);
	if (settings_checksum != (uint16_t)r0_2 || backup_settings_checksum != (uint16_t)r0_2)
	{
		move_halfwords(backup_settings_checksum_ADDRESS, settings_checksum_ADDRESS, 0x1Eu);
		eeprom_write_settings();
		return;
	}
}

/* Counts a save of the settings in eeprom_write_count, up to 10000 (the test mode warns when the
   EEPROM's limit of 10,000 writes is reached). */
void count_eeprom_write(void)
{
	eeprom_write_count++;
	if (eeprom_write_count >= 0x2710u)
		eeprom_write_count = 0x2710u;
}

/* Locks the EEPROM against writes (command 00, the 93C46's write disable) and turns the interrupts back
   on. */
void eeprom_lock(void)
{
	uint8_t r4 = 0;
	uint32_t r2 = 0, r13 = 0;

	r4 = 0u;
	eeprom_begin(&r2, &r13);
	eeprom_send_command(r2, r4, r13);
	interrupts_on();
}

/* Unused: reads the EEPROM's word R4 (its read command, R4 | 80) into R0 and turns the interrupts back
   on. */
void eeprom_read_one_word(uint8_t r4)
{
	uint32_t r2 = 0, r13 = 0, ignored = 0;

	r4 |= 0x80u;
	eeprom_begin(&r2, &r13);
	eeprom_read_word(r4, r13, &ignored, &r2);
	interrupts_on();
}

/* Unused: reads the settings from the EEPROM again (eeprom_read_settings) and turns the interrupts back
   on. */
void reload_settings(uint32_t r0)
{
	eeprom_read_settings(r0);
	interrupts_on();
}

/* Reads the EEPROM's 64 words into 20E600-20E67F (settings_magic, settings_signature, the settings from
   settings_checksum on, their backup from backup_settings_checksum on), with the interrupts off. */
void eeprom_read_settings(uint32_t r0)
{
	uint8_t r4 = 0;
	uint32_t r2 = 0, r5 = 0, r11 = 0, r13 = 0;

	r11 = settings_magic_ADDRESS;
	r4 = 0x80u;
	r5 = 0x40u;
	eeprom_begin(&r2, &r13);
	do
	{
		eeprom_read_word(r4, r13, &r0, &r2);
		r4++;
		mem_write16(r11, r0);
		r5--;
		r11 += 2u;
	} while (r5 != 0u);
}

/* Unused: writes R0 into the EEPROM's word R4 (its write command, R4 | 40) without unlocking it first,
   and turns the interrupts back on. */
void eeprom_write_one_word(uint16_t r0, uint8_t r4)
{
	uint32_t r2 = 0, r13 = 0;

	r4 |= 0x40u;
	eeprom_begin(&r2, &r13);
	eeprom_write_word(r0, r2, r4, r13);
	interrupts_on();
}

/* Saves the settings into the EEPROM after they changed (the link's, link_take_master_settings; the test
   mode's, test_mode_exit): eeprom_write_settings_counted, then eeprom_lock. */
void save_settings(void)
{
	eeprom_write_settings_counted();
	eeprom_lock();
}

/* Counts the save (count_eeprom_write) and writes the settings (eeprom_write_settings). */
void eeprom_write_settings_counted(void)
{
	count_eeprom_write();
	eeprom_write_settings();
}

/* Writes the settings into the EEPROM: their checksum (crc16 of the 3A bytes from eeprom_write_count)
   into settings_checksum, checksum and settings copied over the backup (backup_settings_checksum on),
   the EEPROM unlocked (command 30, write enable) and its 64 words written from 20E600
   (eeprom_write_word). Leaves the interrupts off and the EEPROM unlocked (save_settings locks it). */
void eeprom_write_settings(void)
{
	uint8_t r4 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r2 = 0, r5 = 0, r10 = 0, r11 = 0, r13 = 0;

	r10 = eeprom_write_count_ADDRESS;
	r11 = 0x3Au;
	r0 = crc16(r10, r11);
	settings_checksum = r0;
	move_string(settings_checksum_ADDRESS, 0x1Eu, backup_settings_checksum_ADDRESS, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	r4 = 0x30u;
	eeprom_begin(&r2, &r13);
	eeprom_send_command(r2, r4, r13);
	r10 = settings_magic_ADDRESS;
	r4 = 0x40u;
	r5 = 0x40u;
	eeprom_begin(&r2, &r13);
	do
	{
		r0_2 = mem_read16(r10);
		r10 += 2u;
		r2 = eeprom_write_word(r0_2, r2, r4, r13);
		r4++;
		r5--;
	} while (r5 != 0u);
}

/* Reads the EEPROM word of read command R4 (eeprom_send_command): 18 clock pulses collecting the data
   out (bit 7 of R12, port F of the second I/O chip), the 16-bit word into R0; then chip select off. */
void eeprom_read_word(uint8_t r4, uint32_t r13, uint32_t *p_r0, uint32_t *p_r2)
{
	uint32_t r0 = *p_r0;
	uint32_t r2 = *p_r2;
	uint32_t r3 = 0;

	r2 = eeprom_send_command(r2, r4, r13);
	r2 = (r2 & 0x3Fu) | 0x20u;
	r3 = 0x12u;
	do
	{
		r2 = eeprom_set_lines(r2, r13);
		r0 = ((r0 & ~0xFFu) | mem_read8(r12)) * 2u;
		r2 = (uint8_t)(r2 ^ 0x40u);
		r2 = eeprom_set_lines(r2, r13);
		r2 = (uint8_t)(r2 ^ 0x40u);
		r3--;
	} while (r3 != 0u);
	r2 = r2 & 0x1Fu;
	r2 = eeprom_set_lines(r2, r13);
	r0 = r0 >> 9;
	*p_r0 = r0;
	*p_r2 = (*p_r2 & 0xFFFFFF00u) | (r2 & 0xFFu);
}

/* Writes R0 into the EEPROM word of write command R4 (eeprom_send_command): its 16 bits high first, chip
   select dropped to start the write, then waits, clocking, until the data out (bit 7 of R12) says it is
   done. */
uint32_t eeprom_write_word(uint32_t r0, uint32_t r2, uint8_t r4, uint32_t r13)
{
	uint8_t r0_2 = 0;
	uint16_t r6 = 0;
	uint32_t r3 = 0;

	r6 = (uint16_t)r0;
	r2 = eeprom_send_command(r2, r4, r13);
	r6 = (uint16_t)(((uint32_t)r6 << 8) | (r6 >> 8));
	r3 = 0x10u;
	do
	{
		r0 = (r0 & ~0xFFu) | (uint8_t)r6;
		r0 = (r0 & ~0xFFu) | (r0 & 0x80u);
		r6 = (uint16_t)(((uint32_t)r6 << 1) | (r6 >> 15));
		r2 = ((r2 & ~0xFFu) | (r2 & 0x1Fu)) + 0x20u + r0;
		r2 = eeprom_set_lines(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = eeprom_set_lines(r2, r13);
		r3--;
	} while (r3 != 0u);
	r3 = 2u;
	do
	{
		r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
		r2 = eeprom_set_lines(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = eeprom_set_lines(r2, r13);
		r3--;
	} while (r3 != 0u);
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	do
	{
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | 0x20u);
		r2 = eeprom_set_lines(r2, r13);
		r0_2 = mem_read8(r12);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = eeprom_set_lines(r2, r13);
	} while ((int8_t)r0_2 >= 0);
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	r2 = eeprom_set_lines(r2, r13);
	return r2;
}

/* Sends command R4 to the EEPROM (a 93C46 on port H of the second I/O chip, R13: bit 7 data in, 6 clock,
   5 chip select): chip select, a start bit, then the 8 bits of R4 high first (two of opcode: 2 read, 1
   write, 0 with the address's top bits 3 write enable or 0 write disable; six of address), each with a
   clock pulse. */
uint32_t eeprom_send_command(uint32_t r2, uint8_t r4, uint32_t r13)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r3 = 0;

	r1 = (uint8_t)r4;
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	r2 = eeprom_set_lines(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = eeprom_set_lines(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x60u);
	r2 = eeprom_set_lines(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = eeprom_set_lines(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0xC0u);
	r2 = eeprom_set_lines(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = eeprom_set_lines(r2, r13);
	r3 = 8u;
	do
	{
		r0 = (uint8_t)r1;
		r1 = (uint8_t)(r1 * 2u);
		r2 = (r2 & ~0xFFu) | (r2 & 0x3Fu);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | (r0 & 0x80u));
		r2 = eeprom_set_lines(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = eeprom_set_lines(r2, r13);
		r3--;
	} while (r3 != 0u);
	return r2;
}

/* Starts an EEPROM access: the interrupts off, R13 the EEPROM's lines (C8000E, port H of the second I/O
   chip), R12 its data out (C8000A, port F, bit 7), R2 the lines' present value. */
void eeprom_begin(uint32_t *p_r2, uint32_t *p_r13)
{
	uint8_t r2 = 0;

	*p_r13 = 0xC8000Eu;
	r12 = 0xC8000Au;
	r2 = mem_read8(0xC8000Eu);
	interrupts_off();
	*p_r2 = (*p_r2 & 0xFFFFFF00u) | r2;
}

/* Writes R2 to the EEPROM's lines (R13), with bit 4 set too. */
uint32_t eeprom_set_lines(uint32_t r2, uint32_t r13)
{
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | 0x10u);
	mem_write8(r13, r2);
	return r2;
}

/* The two tasks' frame loops, one function in the C: second_task(0) is the second task (from 0006B0),
   which runs screen 1; second_task(1) the boot task after boot (from 000637), which first sets up the
   link's race (link_take_master_settings), the interrupt controller's timer (D00008, D0000A),
   task_swapping, each screen's first attract car (screen unk_9E from rom_000D14 by machine_count and the
   screen's car number) and the screens on (C0001C, C8001C), then runs screen 2. A frame of either:
   wait_for_frame (the VBLANK swaps the tasks meanwhile; the second task calls link_send_records before
   it with second_task_waiting set, and the boot task calls link_send_records and link_receive_records
   only while that is clear), the test mode when test_mode_on is set, link_receive_records,
   race_entries_update, race_progress_update and course_records_update (the link and the race records),
   game_frame_count + 1 (screen 1's loop), take_screen_inputs, check_game_start, the game mode's code
   (the table at 0008CC by game_mode: game_mode_power_on, game_mode_restart, game_mode_title,
   game_mode_car_presentation, game_mode_demo_race, game_mode_ranking, game_mode_play,
   entry_invitation_step (the entry countdown on a seat without a player), game_mode_isdn_connection),
   run_objects and lcd_update; with single_task set it goes on with the other screen's frame instead of
   waiting. The test mode: task_swapping off, test_mode_start, then test_mode_frame each frame while
   test_mode_on lasts; then the screens off, the variables cleared, the second task's frame written
   again, initialise_game, lcd_reset, both screens in game mode 1 with a fade in (fade 1),
   network_check_again, and the boot task's part again. */
void second_task(uint32_t entry_point)
{
	bool L_000637 = false, L_000719 = false, L_00077F = false;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;
	uint32_t target1 = 0;

	for (;;)
	{
		for (;;)
		{
			target1 = L_000719 ? 1u : L_000637 ? 1u : entry_point;
			switch (target1)
			{
			case 0x000000u:
				break;
			case 0x000001u:
				if (!L_000719)
				{
					L_000637 = false;
					link_take_master_settings();
					mem_write16(IRQ_D00008, 0x39u);
					mem_write16(IRQ_D0000A, 1u);
					unk_20E0FD = 0u;
					screen_2_vblanks = 0u;
					unk_20E0FF = 0u;
					screen_1_vblanks = 0u;
					vblanks_since_start = 0u;
					task_swapping = 0xFFu;
					r0 = rom_000D14[machine_count];
					r2 = unk_218002;
					unk_20809E = mem_read8(r0 + unk_208002);
					unk_21809E = mem_read8(r0 + r2);
					mem_write8(IO_C0001C, 6u);
					mem_write8(0xC8001Cu, 0x86u);
				}
				do
				{
					L_000719 = false;
					if (second_task_waiting == 0u)
						link_send_records();
					r2 = wait_for_frame(r2);
					L_00077F = test_mode_on != 0u;
					if (L_00077F)
						break;
					if (second_task_waiting == 0u)
						link_receive_records();
					race_entries_update();
					race_progress_update();
					course_records_update();
					screen = SCREEN_2;
					r2 = take_screen_inputs();
					r0 = SCREEN(screen)->game_mode;
					player_car = screen + 0x1000u;
					r0 = check_game_start(r0, r2);
					switch (r0)
					{
					case 0:
						game_mode_power_on();
						break;
					case 1:
						game_mode_restart();
						break;
					case 2:
						game_mode_title();
						break;
					case 3:
						game_mode_car_presentation();
						break;
					case 4:
						game_mode_demo_race();
						break;
					case 5:
						game_mode_ranking();
						break;
					case 6:
						game_mode_play();
						break;
					case 7:
						entry_invitation_step();
						break;
					case 8:
						game_mode_isdn_connection();
						break;
					default:
						invalid_jump(rom_0008CC[r0]);
					}
					run_objects();
					lcd_update();
				} while (single_task == 0u);
				if (L_00077F)
					break;
				break;
			default:
				invalid_jump(target1);
			}
			if (L_00077F)
				break;
			do
			{
				second_task_waiting = 0xFFu;
				link_send_records();
				r2 = wait_for_frame(r2);
				second_task_waiting = 0u;
				L_00077F = test_mode_on != 0u;
				if (L_00077F)
					break;
				link_receive_records();
				race_entries_update();
				race_progress_update();
				course_records_update();
				game_frame_count++;
				screen = SCREEN_1;
				r2 = take_screen_inputs();
				r0 = SCREEN(screen)->game_mode;
				player_car = screen + 0x1000u;
				r0 = check_game_start(r0, r2);
				switch (r0)
				{
				case 0:
					game_mode_power_on();
					break;
				case 1:
					game_mode_restart();
					break;
				case 2:
					game_mode_title();
					break;
				case 3:
					game_mode_car_presentation();
					break;
				case 4:
					game_mode_demo_race();
					break;
				case 5:
					game_mode_ranking();
					break;
				case 6:
					game_mode_play();
					break;
				case 7:
					entry_invitation_step();
					break;
				case 8:
					game_mode_isdn_connection();
					break;
				default:
					invalid_jump(rom_0008CC[r0]);
				}
				run_objects();
				lcd_update();
			} while (single_task == 0u);
			if (L_00077F)
				break;
			L_000719 = true;
		}
		L_00077F = false;
		task_swapping = 0u;
		wait_for_frame(r2);
		test_mode_start();
		do
		{
			wait_for_frame(r2);
			test_mode_frame();
		} while (test_mode_on != 0u);
		interrupts_off();
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r0 = 0u;
		r1 = 0u;
		r2 = 0x100u;
		r10 = unk_20E000_ADDRESS;
		do
		{
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			r2--;
		} while (r2 != 0u);
		move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
		player_car = 0x219000u;
		unk_20E000 = 0x21FF84u;
		waiting_task_stack = 0x21F800u;
		initialise_game();
		lcd_reset();
		unk_208080 = 1u;
		unk_208153 = 1u;
		unk_208094 |= 1u;
		unk_218080 = 1u;
		unk_218153 = 1u;
		unk_218094 |= 1u;
		interrupts_on();
		network_check_again();
		L_000637 = true;
	}
}

/* Each frame before the game mode's code: in the attract modes (not 0, not 6), with a credit on the seat
   (seat1_credits, seat2_credits for screen 2) or free play (free_play), blinks the START lamp (bit 2 of
   the seat's port_d_outputs, 8 frames on and 8 off by start_lamp_blink); START pressed (buttons_pressed
   bit 6) takes the credit (not with free play; and clears unk_20E0A2, unk_20E0AA), turns the lamp off
   and starts a game: game mode 6 at step 0, link_event 1, the race record's car part cleared
   (race_record_clear) and race unk_19 = FF. Returns the game mode. */
uint32_t check_game_start(uint32_t r0, uint32_t r2)
{
	uint8_t r3 = 0;
	uint32_t r0_2 = 0, index = 0;
	uint32_t t1 = 0;

	if (r0 == 0u || (uint8_t)r0 == 6u)
		return r0;
	index = SCREEN(screen)->index;
	if (free_play != 0u)
	{
		r3 = (uint8_t)(SCREEN(screen)->start_lamp_blink + 1u);
		SCREEN(screen)->start_lamp_blink = r3;
		port_d_outputs[index] = (port_d_outputs[index] & ~4u) | ((r3 & 8u) >> 1);
		if ((SCREEN(screen)->buttons_pressed & 0x40u) == 0u)
			return r0;
		if (free_play == 0u)
		{
			mem_write8(r2, mem_read8(r2) - 1u);
			mem_write8(r2 + 2u, 0u);
		}
		SCREEN(screen)->start_lamp_blink = 0u;
		t1 = port_d_outputs_ADDRESS + index;
		mem_write8(t1, mem_read8(t1) & ~4u);
		r0_2 = 6u;
		AS32(SCREEN(screen)->game_mode) = 6u;
		SCREEN(screen)->link_event = 1u;
		race_record_clear();
		RACE(SCREEN(screen)->race)->entry_request = 0xFFu;
		return r0_2;
	}
	r2 = rom_0008C4[index];
	if (mem_read8(r2) == 0u)
		return r0;
	r3 = (uint8_t)(SCREEN(screen)->start_lamp_blink + 1u);
	SCREEN(screen)->start_lamp_blink = r3;
	port_d_outputs[index] = (port_d_outputs[index] & ~4u) | ((r3 & 8u) >> 1);
	if ((SCREEN(screen)->buttons_pressed & 0x40u) == 0u)
		return r0;
	if (free_play == 0u)
	{
		mem_write8(r2, mem_read8(r2) - 1u);
		mem_write8(r2 + 2u, 0u);
	}
	SCREEN(screen)->start_lamp_blink = 0u;
	t1 = port_d_outputs_ADDRESS + index;
	mem_write8(t1, mem_read8(t1) & ~4u);
	r0_2 = 6u;
	AS32(SCREEN(screen)->game_mode) = 6u;
	SCREEN(screen)->link_event = 1u;
	race_record_clear();
	RACE(SCREEN(screen)->race)->entry_request = 0xFFu;
	return r0_2;
}

/* Waits for the next frame: polls frame_ready (set by the VBLANK, which also swaps the tasks) and clears
   it; while the screen has tile pages to unpack (screen unk_B00) it unpacks them a piece at a time
   (unpack_queued_step) between the polls. */
uint32_t wait_for_frame(uint32_t r2)
{
	for (;;)
	{
		if ((uint8_t)SCREEN(screen)->unpack_state == 0u)
		{
			do
			{
				frame_wait(frame_ready_ADDRESS);
				take_interrupts();
			} while (frame_ready == 0u);
			frame_end();
			frame_ready = 0u;
			break;
		}
		r2 = unpack_queued_step(r2);
		if (frame_ready != 0u)
		{
			frame_ready = 0u;
			break;
		}
	}
	return r2;
}

/* Sets the game up, at start-up and after the test mode: 13, 0, 0, 1F to C00074 (which the board does
   not map); for screen 2, then 1, its objects' page cleared (clear_screen_ram) and its constants
   (init_screen: index, its video registers, the view's, the courses' stage lengths at 700);
   initialise_board; then initialise_screen for screens 1 and 2. */
void initialise_game(void)
{
	uint32_t r0 = 0;

	mem_write8(IO_C00074, 0x13u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0x1Fu);
	screen = SCREEN_2;
	clear_screen_ram();
	init_screen();
	screen = SCREEN_1;
	clear_screen_ram();
	r0 = init_screen();
	initialise_board(r0);
	screen = SCREEN_1;
	initialise_screen();
	screen = SCREEN_2;
	initialise_screen();
}

/* Sets a screen up: its sound off (reset_screen_sound), no objects (clear_objects), its tilemaps'
   registers and their copies (init_mixer), three palettes uploaded (upload_boot_palettes) and the
   courses' records (load_course_records). */
void initialise_screen(void)
{
	reset_screen_sound();
	clear_objects();
	init_mixer();
	upload_boot_palettes();
	load_course_records();
}

/* Sets the board up: the I/O chips (init_io_chips), the settings (load_settings), the sprite chip and
   its RAM (init_sprite_chip, load_sprite_ram_tables), the video RAM and its control (init_video), the
   backup RAM checked (bookkeeping_check), the credits (init_coins), the sound requests
   (start_sound_program), three lists of tile pages unpacked at once (unpack_tile_pages: 109420, 109428,
   109578), then the inputs read and taken twice (read_switches_and_analog, take_screen_inputs) so that
   their previous values hold. */
void initialise_board(uint32_t r0)
{
	uint32_t r10 = 0;

	init_io_chips();
	load_settings(r0);
	init_sprite_chip();
	load_sprite_ram_tables();
	init_video();
	bookkeeping_check();
	init_coins();
	start_sound_program();
	r10 = 0x109420u;
	unpack_tile_pages(r10);
	r10 = 0x109428u;
	unpack_tile_pages(r10);
	r10 = 0x109578u;
	unpack_tile_pages(r10);
	read_switches_and_analog();
	read_switches_and_analog();
	take_screen_inputs();
	take_screen_inputs();
}

/* Reads the inputs in the VBLANK (int_vector_40; twice at start-up): the switches of both I/O chips
   (ports E and F: bits 0-1 the seats' service switches, 2-5 the coin switches, 7 a test switch) into
   switches, with previous_switches, switches_pressed and switches_released; then, on the turn of the
   screen whose task runs, half the analog inputs through the A/D converter (C00050 + 4 * index): screen
   1's turn seat 1's wheel, gas and brake and seat 2's wheel, screen 2's seat 2's gas and brake (the
   converter's second bank), each raw into analog_raw on and scaled to 0-FF between its calibration (a
   low and a high byte from 20E638, in the settings) into analog_inputs on; then the converter's bank for
   the other turn (C00060). */
uint32_t read_switches_and_analog(void)
{
	uint8_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint16_t r0_2 = 0, r7 = 0, r8 = 0;
	uint32_t r1_2 = 0, r2_2 = 0, r3_2 = 0, r4 = 0, r5 = 0, r6 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint16_t t5 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(IO_C0000A);
	r1 = (uint8_t)~t2;
	r0 = (uint8_t)(~t1 | ((((uint32_t)r1 << 4) | (r1 >> 4)) & 3u));
	t3 = mem_read8(0xC80008u);
	t4 = mem_read8(0xC8000Au);
	r3 = (uint8_t)~t4;
	r2 = (uint8_t)(~t3 | ((((uint32_t)r3 << 4) | (r3 >> 4)) & 3u));
	r1 = (r0 | r2) & 2u;
	r0 = (uint8_t)((r0 & 0xDu) | ((((r2 & 0xCu) * 2u) | (r2 & 1u)) * 2u) | ((uint32_t)r1 << 6) | (r1 >> 2));
	r1 = switches;
	switches = r0;
	previous_switches = r1;
	switches_pressed = ~r1 & r0;
	switches_released = r1 & ~r0;
	r5 = SCREEN(screen)->index;
	r5 = r5 & 1u;
	r1_2 = IO_PORTS + 0x50u + r5 * 4u;
	r2_2 = analog_raw_ADDRESS + r5 * 4u;
	r3_2 = analog_inputs_ADDRESS + r5 * 4u;
	r4 = rom_000B14[r5];
	r6 = rom_000B0C[r5];
	do
	{
		mem_write8(r1_2, 0xFFu);
		r0_2 = (((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)(mem_read8(r1_2) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) >> 7;
		mem_write8(r2_2, (uint8_t)r0_2);
		r7 = mem_read8(r6);
		r6++;
		r8 = mem_read8(r6);
		r2_2++;
		r6++;
		if (r8 < r0_2)
			r0_2 = (uint16_t)r8;
		t5 = r0_2;
		r0_2 = (uint16_t)(t5 - r7);
		if ((int16_t)t5 < (int16_t)r7)
			r0_2 = 0u;
		mem_write8(r3_2, divide_u16((uint16_t)(r0_2 * 0xFFu), (uint16_t)(r8 - r7)));
		r4--;
		r3_2++;
		r1_2 += 2u;
	} while (r4 != 0u);
	mem_write8(IO_C00060, r5 ^ 1u);
	return r6;
}

/* Packs both seats' inputs for the coming frame (the VBLANK, on screen 1's turn) into their records: the
   screen's player + 1, the buttons (port A's high bits into 0-3, its low two into 4-5, the shift; port E
   bit 4 into bit 6, START), the wheel (analog_inputs inverted), the gas and the brake: seat_1_player to
   seat_1_brake and seat_2_player to seat_2_brake. On ISDN (isdn_mode) they go to the link board instead
   (link_send_seat_inputs). */
void pack_seat_inputs(void)
{
	uint8_t r0 = 0, r1 = 0, r2 = 0;
	uint32_t r2_2 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(IO_C00000);
	r2 = (uint8_t)~t2;
	r0 = ((~t1 * 2u * 2u) & 0x40u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 0x3Fu);
	t3 = mem_read8(0xC80008u);
	t4 = mem_read8(0xC80000u);
	r2 = (uint8_t)~t4;
	r1 = ((~t3 * 2u * 2u) & 0x40u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 0x3Fu);
	r10 = analog_inputs_ADDRESS;
	if (isdn_mode != 0u)
	{
		r11 = 0x800020u;
		r2_2 = 0x208040u;
		link_send_seat_inputs(r0, r2_2, &r10, &r11);
		r2_2 = 0x218040u;
		r0 = (uint8_t)r1;
		link_send_seat_inputs(r0, r2_2, &r10, &r11);
		return;
	}
	seat_1_player = unk_208001 + 1u;
	seat_1_buttons = r0;
	seat_1_wheel = ~analog_inputs;
	r10++;
	seat_1_gas = analog_gas_1;
	r10++;
	seat_1_brake = analog_brake_1;
	seat_2_player = unk_218001 + 1u;
	seat_2_buttons = r1;
	r10++;
	seat_2_wheel = ~analog_wheel_2;
	r10++;
	seat_2_gas = analog_gas_2;
	seat_2_brake = analog_brake_2;
}

/* Never runs in the scenarios (ISDN only): writes a seat's five bytes as halfwords on the link board at
   R11 (800020 on): 0, the buttons (R0), the wheel (inverted), gas and brake from analog_inputs at R10;
   or, when its screen has a link_event (R2: screen + 40), the event and the four bytes after it, which
   it then clears (without moving R10 on, so the next seat would read this one's analog inputs). */
void link_send_seat_inputs(uint8_t r0, uint32_t r2, uint32_t *p_r10, uint32_t *p_r11)
{
	uint32_t r10 = *p_r10;
	uint32_t r11 = *p_r11;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	if (mem_read8(r2) != 0u)
	{
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2 = r2 + 1u - 4u;
		mem_write32(r2, 0u);
		mem_write8(r2 - 1u, 0u);
		*p_r11 = r11;
		return;
	}
	mem_write16(r11, 0u);
	r11 += 2u;
	mem_write16(r11, (uint8_t)r0);
	t1 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, (uint8_t)~t1);
	t2 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, t2);
	t3 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, t3);
	r11 += 2u;
	*p_r10 = r10;
	*p_r11 = r11;
}

/* queue_screen_inputs for screen 1 (seat_1_buttons, the first I/O chip) and screen 2 (seat_2_buttons,
   the second): the VBLANK on screen 2's turn. */
uint32_t queue_seat_inputs(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = SCREEN_1;
	r11 = seat_1_buttons_ADDRESS;
	r12 = 0xC00000u;
	queue_screen_inputs(r10, r11);
	r10 = SCREEN_2;
	r11 = seat_2_buttons_ADDRESS;
	r12 = 0xC80000u;
	return queue_screen_inputs(r10, r11);
}

/* Puts a screen's inputs into its ring of eight (screen + 130, at input_write_place): the seat's buttons
   (R11) with ~port B of its I/O chip (R12 + 2) in bits 8-15; and the record's wheel, gas and brake into
   the screen's wheel, gas and brake. On ISDN the record is the screen's race_copy's +2C (the inputs the
   link brought). */
uint32_t queue_screen_inputs(uint32_t r10, uint32_t r11)
{
	uint32_t r0 = 0, r2 = 0, input_write_place = 0;

	if (isdn_mode != 0u)
		r11 = SCREEN(r10)->race_copy + 0x2Cu;
	r0 = (uint8_t)~mem_read8(r12 + 2u);
	r2 = mem_read32(r11);
	r0 = (uint16_t)(r0 << 8) | (uint8_t)r2;
	input_write_place = SCREEN(r10)->input_write_place;
	mem_write32(r10 + 0x130u + input_write_place * 4u, r0);
	SCREEN(r10)->input_write_place = (input_write_place + 1u) & 7u;
	AS32(SCREEN(r10)->wheel) = r2 >> 8;
	return r0;
}

/* Takes the screen's inputs for the frame from its ring (input_read_place; both frame loops, before
   check_game_start): buttons, with previous_buttons, buttons_pressed and buttons_released (bit 6 START,
   4-5 the shift up and down, 0-3 the others, 8-15 port B). On ISDN also the link's status (race_copy
   unk_2B) into link_status, with link_status_set, link_status_cleared and link_status_changed, and,
   unless in it already, game mode 8 (game_mode_isdn_connection) while it says the line connects (bits
   2-3). */
uint32_t take_screen_inputs(void)
{
	uint8_t r0_2 = 0, r3_2 = 0, link_status = 0;
	uint32_t r0 = 0, r2 = 0, r3 = 0, buttons = 0, input_read_place = 0;
	uint8_t t1 = 0;

	input_read_place = SCREEN(screen)->input_read_place;
	r0 = mem_read32(screen + 0x130u + input_read_place * 4u);
	SCREEN(screen)->input_read_place = (input_read_place + 1u) & 7u;
	r3 = screen + 0x110u;
	buttons = SCREEN(r3 - 0x110u)->buttons;
	SCREEN(r3 - 0x110u)->buttons = r0;
	r3 += 4u;
	SCREEN(r3 - 0x114u)->previous_buttons = buttons;
	r2 = ~buttons & r0;
	r3 += 4u;
	SCREEN(r3 - 0x118u)->buttons_pressed = r2;
	SCREEN(r3 - 0x118u)->buttons_released = buttons & ~r0;
	if (isdn_mode != 0u)
	{
		r0_2 = RACE(SCREEN(screen)->race_copy)->unk_2B;
		t1 = r0_2;
		link_status = SCREEN(screen)->link_status;
		SCREEN(screen)->link_status = t1;
		r2 = (r2 & ~0xFFu) | (uint8_t)~link_status;
		r2 = (r2 & ~0xFFu) | (r2 & r0_2);
		SCREEN(screen)->link_status_set = r2;
		r3_2 = ~r0_2 & link_status;
		SCREEN(screen)->link_status_cleared = r3_2;
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | r3_2);
		SCREEN(screen)->link_status_changed = r2;
		if (SCREEN(screen)->game_mode != 8u && (r0_2 & 0xCu) != 0u)
			AS32(SCREEN(screen)->game_mode) = 8u;
	}
	return r2;
}

/* Never runs in the scenarios (ISDN only): the players on this side and on the other (race_copy unk_29,
   unk_2A) into isdn_players_this_side and isdn_players_other_side, their sum into player_count (the
   race's players) and half of it into machine_count (its cabinets). */
void isdn_count_players(void)
{
	uint8_t r0 = 0, r1 = 0;

	r0 = RACE(SCREEN(screen)->race_copy)->unk_29;
	r1 = RACE(SCREEN(screen)->race_copy)->unk_2A;
	isdn_players_this_side = r0;
	isdn_players_other_side = r1;
	r1 += r0;
	player_count = r1;
	machine_count = r1 >> 1;
}

/* Game mode 8, on ISDN only (never in the scenarios; take_screen_inputs goes to it while link_status
   says the line connects, bits 2-3), by mode_step: 0 a fade out (attract_lamps_update in 0 and 1); 1
   when it is over, no objects, the sound off, the screen's tilemaps, sprites and text off, palette
   031740, the lamps off (checkpoint_lamp, leader_lamp by index), clear_lcd, isdn_status_update started
   (the list at 000F70); 2 a fade in; 3 when it is over, 4; 4 when link_status says connected (bit 0:
   isdn_count_players) or no longer connecting (bits 2-3 clear), mode_timer 150 frames; 5 counts them
   down, then a fade out; 6 when it is over, game mode 2 (the title). */
void game_mode_isdn_connection(void)
{
	uint32_t r10 = 0, r19 = 0, link_status = 0;
	uint8_t mode_timer = 0;

	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->mode_step++;
		attract_lamps_update();
		return;
	case 1:
		if ((SCREEN(screen)->flags & 1u) != 0u)
		{
			attract_lamps_update();
			return;
		}
		SCREEN(screen)->mode_step++;
		clear_objects();
		reset_screen_sound();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		r19 = 0x31740u;
		queue_palette(r19);
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		clear_lcd();
		r10 = 0xF70u;
		start_object_list(r10);
		return;
	case 2:
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->mode_step++;
		return;
	case 3:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			SCREEN(screen)->mode_step++;
			return;
		}
		return;
	case 4:
		break;
	case 5:
		mode_timer = (uint8_t)SCREEN(screen)->mode_timer;
		AS8(SCREEN(screen)->mode_timer) = mode_timer - 1u;
		if (mode_timer == 1u)
		{
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			SCREEN(screen)->mode_step++;
		}
		return;
	case 6:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			AS32(SCREEN(screen)->game_mode) = 2u;
		return;
	default:
		invalid_jump(rom_000DAC[SCREEN(screen)->mode_step]);
	}
	link_status = SCREEN(screen)->link_status;
	if ((link_status & 1u) != 0u)
	{
		isdn_count_players();
		SCREEN(screen)->mode_step++;
		AS8(SCREEN(screen)->mode_timer) = 0x96u;
		return;
	}
	if ((link_status & 0xCu) == 0u)
	{
		SCREEN(screen)->mode_step++;
		AS8(SCREEN(screen)->mode_timer) = 0x96u;
		return;
	}
}

/* Never runs in the scenarios: the ISDN connection screen's texts (game_mode_isdn_connection starts it
   from the list at 000F70), on the text layer (print_text_layer), by state: 0 by link_status: bit 0 ISDN
   SUCCESS, THIS SIDE, OTHER SIDE and TOTAL with the players (race_copy unk_29, unk_2A and their sum,
   print_hex_byte), then 1; bits 2-3 clear ISDN FAILED, then 1; else ISDN CONECTING (sic) blinking every
   8 frames (its unk_10 counting); 1 nothing more. */
void isdn_status_update(void)
{
	uint8_t r0_2 = 0, r8 = 0, r9 = 0, link_status = 0;
	uint16_t r1 = 0;
	uint32_t r0 = 0, r11 = 0, r19 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		return;
	default:
		invalid_jump(rom_000EB0[OBJECT(current_object)->state]);
	}
	link_status = SCREEN(screen)->link_status;
	if ((link_status & 1u) == 0u)
	{
		if ((link_status & 0xCu) == 0u)
		{
			OBJECT(current_object)->state++;
			r19 = rom_000F80;
			r19 = print_text_layer(r19);
			return;
		}
		r0 = (uint8_t)OBJECT(current_object)->unk_10;
		r0 = (uint8_t)(r0 + 1u);
		AS8(OBJECT(current_object)->unk_10) = r0;
		r0 = r0 >> 3;
		r19 = rom_000F78[r0 & 1u];
		r19 = print_text_layer(r19);
		return;
	}
	OBJECT(current_object)->state++;
	r19 = rom_000F84;
	print_text_layer(r19);
	r19 = rom_000F88;
	print_text_layer(r19);
	r19 = rom_000F8C;
	print_text_layer(r19);
	r19 = rom_000F90;
	print_text_layer(r19);
	r1 = 0x6000u;
	r0_2 = RACE(SCREEN(screen)->race_copy)->unk_29;
	r9 = (uint8_t)r0_2;
	r11 = TEXT_CELL(24, 12);
	print_hex_byte(r0_2, r1, r11);
	r0_2 = RACE(SCREEN(screen)->race_copy)->unk_2A;
	r8 = (uint8_t)r0_2;
	r11 = TEXT_CELL(24, 13);
	print_hex_byte(r0_2, r1, r11);
	r0_2 = (uint8_t)((uint32_t)r9 + r8);
	r11 = TEXT_CELL(24, 14);
	print_hex_byte(r0_2, r1, r11);
}

/* Game mode 2, the title (title_screen_update), by mode_step: 0 the screen cleared (as clear_screen),
   the title's palette (031E88), title_screen_update started, a fade in (fade 1), the input ring's places
   to 0 (input_write_place, input_read_place), the screen on (C0001C = 6, C8001C = 86 for screen 2); 1
   when the fade is over, mode_timer 450 frames (15 seconds); 2 counts them down, then a fade out (fade
   0); 3 when it is over, game mode 3 (the car presentation). attract_lamps_update every frame first. */
void game_mode_title(void)
{
	uint32_t r19 = 0, r21 = 0;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		clear_objects();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		r19 = rom_031E88;
		queue_palette(r19);
		r21 = start_object();
		mem_write32(r21 + 4u, title_screen_update_ADDRESS);
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->input_write_place = 0u;
		SCREEN(screen)->input_read_place = 0u;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		SCREEN(screen)->mode_step++;
		return;
	case 1:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			SCREEN(screen)->mode_timer = 0x1C2u;
			SCREEN(screen)->mode_step++;
		}
		return;
	case 2:
		mode_timer = SCREEN(screen)->mode_timer;
		SCREEN(screen)->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
		{
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			SCREEN(screen)->mode_step++;
		}
		return;
	case 3:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			AS32(SCREEN(screen)->game_mode) = 3u;
		return;
	default:
		invalid_jump(rom_001014[SCREEN(screen)->mode_step]);
	}
}

/* Unused: starts the 81 bars of a picture, an object each (start_object, handler bar_picture_bar_start)
   from the 8-byte records at 0011B1 (ended by a negative first word): its place on the screen into
   target_x, target_y and its two scales into scales. The places make two rows across the screen (y
   19-107 and 139-195); what the bars show cannot be told without drawing them. */
void bar_picture_start(void)
{
	uint32_t r19 = 0, r21 = 0;
	uint32_t t1 = 0;

	r19 = 0x11B1u;
	do
	{
		r21 = start_object();
		mem_write32(r21 + 4u, bar_picture_bar_start_ADDRESS);
		t1 = ROM32(r19 + 4u);
		mem_write32(r21 + 0x80u, ROM32(r19));
		mem_write32(r21 + 0x84u, t1);
		r19 += 8u;
	} while ((int32_t)ROM32(r19) >= 0);
}

/* Only unused code reaches it (bar_picture_start): sets up a bar of the picture: drawn on the screen
   (draw_mode 0) with animation 0 and palette 25, its width_scale and height_scale from scales, depth
   layer 3, at the middle of the screen (x A0, y 70), with a random speed_x and speed_y (-8 to 7: a byte
   of RANDOM_D80000 shifted right 4); then bar_picture_bar_fly. */
void bar_picture_bar_start(void)
{
	uint32_t r9 = 0;
	uint8_t t1 = 0, t2 = 0;

	OBJECT(current_object)->draw_mode = 0u;
	r9 = 0u;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x25u;
	AS32(OBJECT(current_object)->width_scale) = OBJECT(current_object)->bar_picture.scales;
	OBJECT(current_object)->depth_layer = 3u;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	t1 = mem_read8(RANDOM_D80000);
	OBJECT(current_object)->bar_picture.speed_x = (int16_t)(int8_t)t1 >> 4;
	t2 = mem_read8(RANDOM_D80000);
	OBJECT(current_object)->bar_picture.speed_y = (int16_t)(int8_t)t2 >> 4;
	OBJECT(current_object)->handler = bar_picture_bar_fly_ADDRESS;
}

/* Only unused code reaches it (bar_picture_start): each frame the bar's speed_x and speed_y grow by 1
   toward its place (target_x, target_y) and it moves by them, swinging about it; an axis whose next step
   lands exactly on the place is put there for that frame. When both land on the same frame it shows
   frame D and stays (bar_picture_bar_draw). Drawn every frame (draw_object). */
void bar_picture_bar_fly(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r2 = 0;
	uint16_t target_x = 0, t1 = 0, target_y = 0, t2 = 0;

	r2 = 0xFFFFFFFFu;
	r0 = (uint16_t)OBJECT(current_object)->bar_picture.speed_x;
	r1 = (uint16_t)r0;
	r0 += OBJECT(current_object)->x;
	target_x = OBJECT(current_object)->bar_picture.target_x;
	t1 = target_x - r0;
	overflow = (uint32_t)(int16_t)target_x - (int16_t)r0 != (uint32_t)(int16_t)(target_x - r0);
	zero = t1 == 0u;
	negative = t1 >> 15;
	if (target_x != r0)
	{
		r0 = 1u;
		if ((negative ^ overflow) | zero)
			r0 = 0xFFFFu;
		r0 += r1;
		OBJECT(current_object)->bar_picture.speed_x = r0;
		r0 += OBJECT(current_object)->x;
		r2 = 0u;
	}
	OBJECT(current_object)->x = r0;
	r0_2 = (uint16_t)OBJECT(current_object)->bar_picture.speed_y;
	r1 = (uint16_t)r0_2;
	r0_2 = (uint16_t)(r0_2 + OBJECT(current_object)->y);
	target_y = OBJECT(current_object)->bar_picture.target_y;
	t2 = target_y - r0_2;
	overflow = (uint32_t)(int16_t)target_y - (int16_t)r0_2 != (uint32_t)(int16_t)(target_y - r0_2);
	zero = t2 == 0u;
	negative = t2 >> 15;
	if (target_y == r0_2)
	{
		OBJECT(current_object)->y = r0_2;
		if (r2 == 0u)
		{
			r0_2 = draw_object(r0_2);
			return;
		}
		OBJECT(current_object)->handler = bar_picture_bar_draw_ADDRESS;
		OBJECT(current_object)->frame = 0xDu;
		r0_2 = draw_object(r0_2);
		return;
	}
	r0_2 = 1u;
	if ((negative ^ overflow) | zero)
		r0_2 = 0xFFFFu;
	r0_2 = (uint16_t)(r0_2 + r1);
	OBJECT(current_object)->bar_picture.speed_y = r0_2;
	r0_2 = (uint16_t)(r0_2 + OBJECT(current_object)->y);
	r2 = 0u;
	OBJECT(current_object)->y = r0_2;
	if (r2 == 0u)
	{
		r0_2 = draw_object(r0_2);
		return;
	}
	OBJECT(current_object)->handler = bar_picture_bar_draw_ADDRESS;
	OBJECT(current_object)->frame = 0xDu;
	r0_2 = draw_object(r0_2);
}

/* Only unused code reaches it (bar_picture_start): the bar in its place: draws it (draw_object) each
   frame. */
void bar_picture_bar_draw(void)
{
	uint32_t ignored = 0;

	ignored = draw_object(0u);
}

/* Game mode 0 (the first frame after start-up): game mode 2 and its first step (game_mode_title). */
void game_mode_power_on(void)
{
	AS32(SCREEN(screen)->game_mode) = 2u;
	game_mode_title();
}

/* Game mode 1 (after the ranking, game_mode_ranking, and after the test mode): game mode 2 and its first
   step (game_mode_title). */
void game_mode_restart(void)
{
	AS32(SCREEN(screen)->game_mode) = 2u;
	game_mode_title();
}

/* Game mode 3, the presentation of a car (car_presentation_update), by mode_step: 0 the screen cleared
   (as clear_screen), the palettes of the car's scene (rom_031DDC by rom_001A98 by attract_car) and
   031740, object_handlers_1A78 (credit_display, coin_prompt, car_presentation) started, a fade in; 1
   when it is over, mode_timer 180 frames (6 seconds; the staff credits code adds 1200); 2 counts them
   down, then a fade out; 3 when it is over, link_event 3 and game mode 4 (the demo race).
   attract_lamps_update every frame first. */
void game_mode_car_presentation(void)
{
	uint32_t r10 = 0, r19 = 0;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		break;
	case 1:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			SCREEN(screen)->mode_timer = 0xB4u;
			SCREEN(screen)->mode_step++;
		}
		return;
	case 2:
		mode_timer = SCREEN(screen)->mode_timer;
		SCREEN(screen)->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
		{
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			SCREEN(screen)->mode_step++;
		}
		return;
	case 3:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			SCREEN(screen)->link_event = 3u;
			AS32(SCREEN(screen)->game_mode) = 4u;
		}
		return;
	default:
		invalid_jump(rom_00146C[SCREEN(screen)->mode_step]);
	}
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	r19 = rom_031DDC[rom_001A98[SCREEN(screen)->attract_car]];
	queue_palette(r19);
	r19 = 0x31740u;
	queue_palette(r19);
	r10 = 0x1A78u;
	start_object_list(r10);
	SCREEN(screen)->fade_kind = 1u;
	SCREEN(screen)->flags |= 1u;
	SCREEN(screen)->mode_step++;
}

/* Game mode 4, the demo race of the attract mode (the car just presented, attract_car, on its stage):
   attract_lamps_update, then by mode_step demo_race_set_up, demo_race_fade_in, demo_race_start_timer,
   demo_race_count_down, demo_race_end, and but on the set-up's and the end's frames the race's own steps
   (game_mode_play by game_step, from step 10). */
void game_mode_demo_race(void)
{
	uint32_t left_caller = 0;

	attract_lamps_update();
	left_caller = 0u;
	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		left_caller = demo_race_set_up();
		break;
	case 1:
		demo_race_fade_in();
		break;
	case 2:
		demo_race_start_timer();
		break;
	case 3:
		demo_race_count_down();
		break;
	case 4:
		left_caller = demo_race_end(left_caller);
		break;
	default:
		invalid_jump(rom_001530[SCREEN(screen)->mode_step]);
	}
	if (left_caller != 0u)
		return;
	game_mode_play();
}

/* Step 0 of the demo race: demo_flags bit 8 (the race's code tells the demo by it: the car drives
   itself), the screen cleared (as clear_screen), step 1 with mode_timer 0, and game_step 10 (the demo
   race's set-up in game_mode_play, which does not run this frame). */
uint32_t demo_race_set_up(void)
{
	uint32_t left_caller = 0;

	SCREEN(screen)->demo_flags |= 0x100u;
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	SCREEN(screen)->mode_step++;
	AS8(SCREEN(screen)->mode_timer) = 0u;
	SCREEN(screen)->game_step = 0xAu;
	left_caller = 1u;
	return left_caller;
}

/* Step 1 of the demo race: after two frames (mode_timer), a fade in and step 2. */
void demo_race_fade_in(void)
{
	uint8_t t1 = 0;

	AS8(SCREEN(screen)->mode_timer)++;
	t1 = (uint8_t)SCREEN(screen)->mode_timer & 1u;
	AS8(SCREEN(screen)->mode_timer) = t1;
	if (t1 == 0u)
	{
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->mode_step++;
	}
}

/* Step 2 of the demo race: when the fade is over, mode_timer 600 frames (20 seconds) and step 3. */
void demo_race_start_timer(void)
{
	if ((SCREEN(screen)->flags & 1u) == 0u)
	{
		SCREEN(screen)->mode_timer = 0x258u;
		SCREEN(screen)->mode_step++;
	}
}

/* Step 3 of the demo race: counts mode_timer down, then a fade out and step 4. */
void demo_race_count_down(void)
{
	uint16_t mode_timer = 0;

	mode_timer = SCREEN(screen)->mode_timer;
	SCREEN(screen)->mode_timer = mode_timer - 1u;
	if (mode_timer == 1u)
	{
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->mode_step++;
	}
}

/* Step 4 of the demo race: when the fade is over, game mode 5 (the ranking) and the race record's car
   part cleared (race_record_clear), without the race's step that frame. */
uint32_t demo_race_end(uint32_t left_caller)
{
	uint32_t left_caller_2 = 0;

	if ((SCREEN(screen)->flags & 1u) != 0u)
		return left_caller;
	AS32(SCREEN(screen)->game_mode) = 5u;
	race_record_clear();
	left_caller_2 = 1u;
	return left_caller_2;
}

/* The lamp show of the attract modes (game_mode_title, game_mode_car_presentation, game_mode_demo_race,
   game_mode_ranking, game_mode_isdn_connection): every 8 video frames (vblank_counter >> 3) the next of
   32 steps of the pattern at 001607, two bytes a step and screen, into the screen's two lamp entries
   (byte 0 of checkpoint_lamp and leader_lamp by 4 * index: bits 1 and 3 of the seat's output port D,
   which the VBLANK writes). */
void attract_lamps_update(void)
{
	uint32_t r0 = 0, r1 = 0;

	r0 = vblank_counter;
	r0 = r0 >> 3;
	r0 = (uint8_t)(r0 * 2u);
	r1 = SCREEN(screen)->index;
	r0 = 0x1607u + (uint8_t)(r0 + r1) * 2u;
	r1 = (uint8_t)(r1 * 2u);
	r1 = (uint8_t)(r1 * 2u);
	checkpoint_lamp[r1] = ROM8(r0);
	leader_lamp[r1] = rom_000001[r0];
}

/* The presentation of a car in the attract mode (game mode 3, object_handlers_1A78): the picture of the
   car screen unk_9C names. By state: 0 starts car_name and staff_credits_code (with this object as its
   presentation, kept in staff_credits_code), the screen's background (set_up_background with 001AA0 by
   screen), the picture (animation rom_001A88 by the car, palette 275, draw mode 0) at x A0, y 60 on
   depth layer 0; 1 steps its animation (animation_advance) and draws it. Every frame, in all states, it
   queues the next of the four palette records of rom_001AC8 (palette_step, round and round) with
   queue_palette_list, so the picture's colours cycle. State 2 (the palette only, no picture) is not set
   by its own code. */
void car_presentation_update(void)
{
	uint32_t r0 = 0, r9 = 0, r19 = 0, r21 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_001694[state];
	switch (state)
	{
	case 0:
		r21 = start_object();
		mem_write32(r21 + 4u, car_name_start_ADDRESS);
		r21 = start_object();
		mem_write32(r21 + 4u, staff_credits_code_start_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->car_presentation.staff_credits_code = r21;
		OBJECT(current_object)->state++;
		r19 = 0x1AA0u + SCREEN(screen)->index * 5u * 4u;
		set_up_background(r19);
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x60u;
		OBJECT(current_object)->depth_layer = 0u;
		r0 = SCREEN(screen)->attract_car;
		r9 = rom_001A88[r0];
		OBJECT(current_object)->palette = 0x275u;
		animation_start(r9);
		animation_advance(0u);
		draw_object(r0);
		break;
	case 1:
		animation_advance(0u);
		draw_object(r0);
		r0 = OBJECT(current_object)->car_presentation.palette_step;
		r19 = rom_001AC8[r0];
		if (r19 == 0u)
		{
			r19 = rom_001AC8[0];
			r0 = 0u;
		}
		OBJECT(current_object)->car_presentation.palette_step = r0 + 1u;
		queue_palette_list(r19);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_001694[state]);
	}
	r0 = OBJECT(current_object)->car_presentation.palette_step;
	r19 = rom_001AC8[r0];
	if (r19 == 0u)
	{
		r19 = rom_001AC8[0];
		r0 = 0u;
	}
	OBJECT(current_object)->car_presentation.palette_step = r0 + 1u;
	queue_palette_list(r19);
}

/* Starts watching for the secret code during the presentation of the cars: takes the controls held
   (staff_credits_code_read_controls), puts code_place at the start of the code (001ADC) and goes on to
   staff_credits_code_watch. */
void staff_credits_code_start(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0;

	staff_credits_code_read_controls(0u, &r0, &zero);
	OBJECT(current_object)->staff_credits_code.code_place = 0x1ADCu;
	OBJECT(current_object)->handler = staff_credits_code_watch_ADDRESS;
	staff_credits_code_watch(r0, 0u);
}

/* Watches the controls for the code (001ADC: shift down twice, shift up twice, the wheel left, right,
   left, right, the brake, the gas; a wrong one starts over); when it is complete: sound 9, the
   presentation 1200 frames longer (screen unk_88), and staff_credits_code_show. */
void staff_credits_code_watch(uint32_t r0, uint32_t r2)
{
	uint32_t zero = 0;
	uint8_t r0_2 = 0;
	uint32_t r10 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0;

	staff_credits_code_read_controls(r2, &r0, &zero);
	if (zero)
	{
		staff_credits_code_done();
		return;
	}
	r10 = OBJECT(current_object)->staff_credits_code.code_place;
	t1 = ROM8(r10);
	r10++;
	if (t1 != (uint8_t)r0)
		r10 = 0x1ADCu;
	OBJECT(current_object)->staff_credits_code.code_place = r10;
	if (ROM8(r10) != 0u)
	{
		staff_credits_code_done();
		return;
	}
	r0_2 = 9u;
	queue_sound(r0_2);
	SCREEN(screen)->mode_timer += 0x4B0u;
	t2 = OBJECT(current_object)->staff_credits_code.presentation + 0x10u;
	mem_write16(t2, mem_read16(t2) + 0x4B0u);
	OBJECT(current_object)->staff_credits_code.frames_left = 0x4B0u;
	OBJECT(current_object)->handler = staff_credits_code_show_ADDRESS;
	staff_credits_code_show();
}

/* Shows the staff credits (the texts at 001AE8, draw_text) for 1200 frames, then fade_out_music and
   staff_credits_code_done. */
void staff_credits_code_show(void)
{
	uint32_t r18 = 0, r19 = 0;
	uint16_t frames_left = 0;

	frames_left = OBJECT(current_object)->staff_credits_code.frames_left;
	OBJECT(current_object)->staff_credits_code.frames_left = frames_left - 1u;
	if (frames_left == 1u)
	{
		OBJECT(current_object)->handler = staff_credits_code_done_ADDRESS;
		fade_out_music();
		return;
	}
	r18 = 0x1AE8u;
	while ((int16_t)mem_read16(r18) >= 0)
	{
		r19 = r18;
		r18 = draw_text(r19);
	}
	staff_credits_code_done();
}

/* Does nothing: the code has been used; the object stays until the presentation's objects are cleared. */
void staff_credits_code_done(void)
{
}

/* Reads the controls as bits (0 shift up, 1 shift down, 2 the wheel right, 3 the wheel left, 4 the
   brake, 5 the gas, each with less needed to stay on), keeps them in controls_held and returns the ones
   newly on (zero set when there are none). */
void staff_credits_code_read_controls(uint32_t r2, uint32_t *p_r0, uint32_t *p_zero)
{
	uint32_t r0 = *p_r0;
	uint32_t zero = *p_zero;
	uint8_t r0_3 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2_2 = 0, r3 = 0;
	uint8_t t1 = 0;

	r0_2 = (r0 & ~0xFFu) | OBJECT(current_object)->staff_credits_code.controls_held;
	r1 = 0u;
	r2_2 = (r2 & ~0xFFu) | (uint8_t)~SCREEN(screen)->wheel;
	if ((r0_2 & 8u) != 0u)
	{
		if ((uint8_t)r2_2 < 0x40u)
			r1 |= 8u;
	}
	else if ((uint8_t)r2_2 < 0x20u)
		r1 |= 8u;
	r3 = 2u;
	r1 = staff_credits_code_control_bit(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | SCREEN(screen)->gas;
	r3 = 5u;
	r1 = staff_credits_code_control_bit(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | SCREEN(screen)->brake;
	r3 = 4u;
	r1 = staff_credits_code_control_bit(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | (uint8_t)SCREEN(screen)->buttons;
	if ((r2_2 & 0x10u) != 0u)
		r1 |= 1u;
	if ((r2_2 & 0x20u) == 0u)
	{
		OBJECT(current_object)->staff_credits_code.controls_held = r1;
		t1 = ~r0_2 & r1;
		zero = t1 == 0u;
		r0_3 = t1;
		*p_r0 = (*p_r0 & 0xFFFFFF00u) | r0_3;
		*p_zero = zero;
		return;
	}
	r1 |= 2u;
	OBJECT(current_object)->staff_credits_code.controls_held = r1;
	t1 = ~r0_2 & r1;
	zero = t1 == 0u;
	r0_3 = t1;
	*p_r0 = (*p_r0 & 0xFFFFFF00u) | r0_3;
	*p_zero = zero;
}

/* Sets bit R3 of the controls being read (R1) when the value R2 is E0 or more, or C0 or more when the
   bit was on in the controls held before (R0): staff_credits_code_read_controls' test for the wheel
   turned right, the gas and the brake. */
uint32_t staff_credits_code_control_bit(uint32_t r0, uint32_t r1, uint8_t r2, uint32_t r3)
{
	if ((r0 & shl32(1u, (int)(r3))) != 0u)
	{
		if ((uint8_t)r2 >= 0xC0u)
			r1 |= shl32(1u, (int)(r3));
	}
	else if ((uint8_t)r2 >= 0xE0u)
	{
		r1 |= shl32(1u, (int)(r3));
		return r1;
	}
	return r1;
}

/* Starts the letters of the presented car's name (rom_001CEC by screen unk_9C: MAD POWER, SPEED BUSTER,
   EASY HANDLING, SMOOTH OPERATOR, BAD BOY, ROAD MONSTER, QUICK REACTOR, WILD CHASER): a car_name_letter
   for each character, spaces too, 18 pixels apart from x 168, each 2 frames later than the one before
   (delay 15, 17...); then ends. */
void car_name_start(void)
{
	uint8_t r1 = 0;
	uint16_t r0 = 0;
	uint32_t r10 = 0, r21 = 0;

	r10 = rom_001CEC[SCREEN(screen)->attract_car];
	r0 = 0x168u;
	r1 = 0xFu;
	while (mem_read8(r10) != 0u)
	{
		r21 = start_object();
		mem_write32(r21 + 4u, car_name_letter_update_ADDRESS);
		mem_write8(r21 + 9u, mem_read8(r10));
		mem_write16(r21 + 0x30u, r0);
		r0 += 0x12u;
		mem_write8(r21 + 0x10u, r1);
		r1 += 2u;
		r10++;
	}
	OBJECT(current_object)->flags = 0u;
}

/* A letter of the presented car's name (font 346 at size A0, its colours cycling through 001D70 with
   step_palette, at y 14). By state: 0 sets itself up (its character from character, x + 8); 1 hidden
   until delay runs out; 2 slides in from the right, 24 pixels a frame for 14 frames (slide_frames); 3
   stays until 120 frames after its start (frames_left); 4 slides out to the left, 24 pixels a frame, and
   ends once x is negative. */
void car_name_letter_update(void)
{
	uint32_t r9 = 0, ignored = 0;
	uint8_t delay = 0, slide_frames = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x346u;
		animation_start(r9);
		OBJECT(current_object)->car_name_letter.palette_list = 0x1D70u;
		OBJECT(current_object)->frame = OBJECT(current_object)->car_name_letter.character;
		OBJECT(current_object)->x += 8u;
		OBJECT(current_object)->y = 0x14u;
		OBJECT(current_object)->width_scale = 0xA0u;
		OBJECT(current_object)->height_scale = 0xA0u;
		OBJECT(current_object)->car_name_letter.frames_left = 0x78u;
		OBJECT(current_object)->car_name_letter.slide_frames = 0xEu;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->car_name_letter.frames_left--;
		delay = OBJECT(current_object)->car_name_letter.delay;
		OBJECT(current_object)->car_name_letter.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x18u;
		slide_frames = OBJECT(current_object)->car_name_letter.slide_frames;
		OBJECT(current_object)->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 1:
		OBJECT(current_object)->car_name_letter.frames_left--;
		delay = OBJECT(current_object)->car_name_letter.delay;
		OBJECT(current_object)->car_name_letter.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x18u;
		slide_frames = OBJECT(current_object)->car_name_letter.slide_frames;
		OBJECT(current_object)->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		frames_left = OBJECT(current_object)->car_name_letter.frames_left;
		OBJECT(current_object)->car_name_letter.frames_left = frames_left - 1u;
		if (frames_left == 1u)
			OBJECT(current_object)->state++;
		step_palette();
		ignored = draw_object(0u);
		return;
	case 2:
		OBJECT(current_object)->x -= 0x18u;
		slide_frames = OBJECT(current_object)->car_name_letter.slide_frames;
		OBJECT(current_object)->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		frames_left = OBJECT(current_object)->car_name_letter.frames_left;
		OBJECT(current_object)->car_name_letter.frames_left = frames_left - 1u;
		if (frames_left == 1u)
			OBJECT(current_object)->state++;
		step_palette();
		ignored = draw_object(0u);
		return;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->x -= 0x18u;
		if ((int16_t)OBJECT(current_object)->x >= 0)
		{
			step_palette();
			ignored = draw_object(0u);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_0019CC[OBJECT(current_object)->state]);
	}
	frames_left = OBJECT(current_object)->car_name_letter.frames_left;
	OBJECT(current_object)->car_name_letter.frames_left = frames_left - 1u;
	if (frames_left == 1u)
		OBJECT(current_object)->state++;
	step_palette();
	ignored = draw_object(0u);
}

/* Loads the ten courses' records into course_records (6 bytes each: the car's model, the name's three
   letters, the time in hundredths) from the backup RAM (701F32); a course without one there (its first
   byte negative) gets its default from 002CC4 (SGA, 5'00"00); the model is kept to its low three bits. */
void load_course_records(void)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	r0 = 0xAu;
	r10 = SOUND_RAM + 0x1F32u;
	r11 = 0x2CC4u;
	r12 = course_records_ADDRESS;
	for (;;)
	{
		if ((int8_t)mem_read8(r10) >= 0)
		{
			mem_write8(r12, mem_read8(r10));
			t1 = r12;
			r12 = r12 + 1u;
			mem_write8(t1, mem_read8(t1) & 7u);
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write16(r12, mem_read16(r10));
			r12 = r12 + 2u;
			r11 += 6u;
			r0--;
			r10 += 2u;
			if (r0 == 0u)
				return;
			continue;
		}
		mem_write32(r12, ROM32(r11));
		r12 = r12 + 4u;
		r11 += 4u;
		mem_write16(r12, ROM16(r11));
		r12 = r12 + 2u;
		r10 += 6u;
		r0--;
		r11 += 2u;
		if (r0 != 0u)
			continue;
		break;
	}
}

/* Game mode 5, the ranking (ranking_update), after the demo race and after a game, by mode_step: 0
   demo_flags bit 8 clear, the wheel motor off (unk_20E0D0 by index), the sound off (reset_screen_sound),
   the screen cleared (as clear_screen), clear_published_car, the ranking's palette (031E80),
   object_handlers_2CB8 (credit_display, coin_prompt) and ranking_update started, mode_timer 270 frames
   (9 seconds), race game_over cleared; 1 waits until no fade runs (ranking_background fades the screen
   in); 2 counts the 270 frames down; 3 when no fade runs, game mode 1 (back to the title).
   attract_lamps_update every frame first. */
void game_mode_ranking(void)
{
	uint32_t r10 = 0, r19 = 0, r21 = 0;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		break;
	case 1:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			SCREEN(screen)->mode_step++;
		return;
	case 2:
		mode_timer = SCREEN(screen)->mode_timer;
		SCREEN(screen)->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
			SCREEN(screen)->mode_step++;
		return;
	case 3:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			AS32(SCREEN(screen)->game_mode) = 1u;
		return;
	default:
		invalid_jump(rom_001DE0[SCREEN(screen)->mode_step]);
	}
	SCREEN(screen)->demo_flags &= ~0x100u;
	unk_20E0D0[SCREEN(screen)->index] = 0u;
	reset_screen_sound();
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	clear_published_car();
	r19 = rom_031E80;
	queue_palette(r19);
	SCREEN(screen)->mode_step++;
	r10 = 0x2CB8u;
	start_object_list(r10);
	r21 = start_object();
	mem_write32(r21 + 4u, ranking_update_ADDRESS);
	SCREEN(screen)->mode_timer = 0x10Eu;
	RACE(SCREEN(screen)->race)->game_over = 0u;
}

/* Whether the screen's player has beaten the course's record (zero clear): when the race was finished
   (race_copy unk_5E), the course (the goal's stage, screen unk_A3, less 15) into record_course, and a
   record when the player came first (screen 320 + player is 0) in a time (screen 410 + 2 * player) under
   the course's (course_record_times by course). */
uint32_t course_record_beaten(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, player = 0;

	if (RACE(SCREEN(screen)->race_copy)->unk_5E != 0u)
	{
		r0 = SCREEN(screen)->unk_A3;
		r0 = (uint8_t)(r0 - 0x15u);
		SCREEN(screen)->record_course = r0;
		player = SCREEN(screen)->player;
		if (mem_read8(screen + 0x320u + player) == 0u && mem_read16(screen + 0x410u + player * 2u) < course_record_times[r0 * 3u])
		{
			zero = 1u == 0u;
			return zero;
		}
	}
	zero = 0u == 0u;
	return zero;
}

/* The name entry after a race won at the goal under the course's record time (game step 18, when
   course_record_beaten says so): the certificate (CONGRATULATIONS!, the course, BEST RUNNER OF THE
   RACE.) with the time and the name, a drum of letters to pick from. By state: 0 lights the screen's two
   lamps (unk_F20) and clears lcd_restart_frames, starts name_entry_lcd, the background
   (set_up_background with 002E4C by screen; start_palette_fades with 13F6C0), the countdown number
   (time_digits_start), object_handlers_2D58 (name_entry_car, name_entry_drivers, name_entry_course_name,
   passing_traffic) and the 32 name_entry_letter objects; open, timer 600 frames, wheel_angle A80, the
   name's text record at x F8, y 54 and the time's at x A8, y 54 (time_to_text of the player's time,
   screen 410 + 2 * player); 1 the drum spins in (wheel_angle + 20 a frame for 40 frames, spin_frames; a
   letter is 80 of the turn's 1000); 2 the wheel turns the drum a letter at a time, 4 frames a letter
   (turn_steps; a turn under way turns back if the wheel goes the other way), sooner the further it leans
   (turn_delay counts down by rom_002D6C by the wheel), and << and >> a letter each time; the letter in
   front is letter, and shows blinking at the name's next place; the gas pedal let up (under 40) then
   pressed (C0 or more), START or the music button enters it with a sound (rom_002D98 by place): the left
   arrow takes the last letter back, the right arrow enters a space, ED or the third letter ends the
   entry, as does the timer running out (the seconds left show as the countdown number,
   time_digits_draw); then open 0, an empty name or one of the names at 002D7C (SEX, XXX, XX and X's with
   spaces) becomes ---, 60 frames; 3 writes the record into the race record (unk_11 + 1, unk_16 the time,
   unk_12 the car's model, unk_13-15 the name, unk_18 the course, screen unk_A3 - 15), turns the lamps
   off and sets the game's next step (screen unk_80 byte 2 = 0x15 (step 21)); 4 nothing. Every frame it
   draws the time and the name (draw_text). */
void name_entry_update(void)
{
	bool L_00200F = false, L_00209D = false, L_0020B6 = false, L_0020FB = false, L_00212C = false, L_002130 = false, L_002133 = false;
	uint8_t r0 = 0, r1 = 0;
	uint16_t r0_3 = 0, buttons_pressed = 0, buttons_pressed_2 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r10 = 0, r19 = 0, r21 = 0, ignored = 0, letter = 0, object = 0, race = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t t1 = 0, turn_delay = 0, t2 = 0;
	uint16_t spin_frames = 0, timer = 0, timer_2 = 0;
	uint32_t target1 = 0, target2 = 0, t3 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		SCREEN(screen)->button_lamps = 0xFFFFu;
		lcd_restart_frames = 0u;
		r21 = start_object();
		mem_write32(r21 + 4u, name_entry_lcd_update_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r19 = 0x2E4Cu + SCREEN(screen)->index * 5u * 4u;
		set_up_background(r19);
		r19 = 0x13F6C0u;
		start_palette_fades(r19);
		time_digits_start();
		r10 = 0x2D58u;
		start_object_list(r10);
		r0 = 0x20u;
		do
		{
			r21 = start_object();
			mem_write32(r21 + 0x14u, current_object);
			mem_write32(r21 + 4u, name_entry_letter_update_ADDRESS);
			t1 = r0;
			r0 = (uint8_t)(t1 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t1 != 1u);
		OBJECT(current_object)->name_entry.open = 0xFFu;
		OBJECT(current_object)->name_entry.timer = 0x258u;
		OBJECT(current_object)->name_entry.spin_frames = 0x28u;
		OBJECT(current_object)->name_entry.wheel_angle = 0xA80u;
		OBJECT(current_object)->name_entry.letter = 0xFFu;
		r0_2 = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
		object = current_object + 0x9Au;
		OBJECT(object - 0x9Au)->name_entry.name_x = 0xF8u;
		object += 2u;
		OBJECT(object - 0x9Cu)->name_entry.name_y = 0x54u;
		OBJECT(object - 0x9Cu)->name_entry.name_attribute = 0xF0u;
		object = current_object + 0xA4u;
		OBJECT(object - 0xA4u)->name_entry.time_x = 0xA8u;
		object += 2u;
		OBJECT(object - 0xA6u)->name_entry.time_y = 0x54u;
		object += 2u;
		OBJECT(object - 0xA8u)->name_entry.time_attribute = 0xF0u;
		object += 2u;
		time_to_text(r0_2, object);
		OBJECT(current_object)->state++;
	case 1:
		OBJECT(current_object)->name_entry.wheel_angle += 0x20u;
		spin_frames = OBJECT(current_object)->name_entry.spin_frames;
		OBJECT(current_object)->name_entry.spin_frames = spin_frames - 1u;
		if (spin_frames != 1u)
		{
			r0_2 = OBJECT(current_object)->name_entry.timer;
			r0_2 = r0_2 / 0x1Eu;
			r0_2 = binary_to_bcd(r0_2);
			time_digits_draw(r0_2);
			r19 = current_object + 0xA4u;
			draw_text(r19);
			r19 = current_object + 0x9Au;
			ignored = draw_text(r19);
			return;
		}
		OBJECT(current_object)->state++;
	case 2:
		r0 = (uint8_t)OBJECT(current_object)->name_entry.turn_steps;
		L_00200F = r0 == 0u;
		if (!L_00200F)
		{
			if ((int8_t)r0 < 0)
			{
				if (SCREEN(screen)->wheel >= 0x60u)
				{
					OBJECT(current_object)->name_entry.turn_steps = r0 + 1u;
					r0_3 = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
				}
				else
				{
					r0 += 4u;
					OBJECT(current_object)->name_entry.turn_steps = r0 - 1u;
					r0_3 = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
				}
			}
			else if (SCREEN(screen)->wheel < 0xA0u)
			{
				OBJECT(current_object)->name_entry.turn_steps = r0 - 1u;
				r0_3 = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
			}
			else
			{
				r0 -= 4u;
				OBJECT(current_object)->name_entry.turn_steps = r0 + 1u;
				r0_3 = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
			}
			OBJECT(current_object)->name_entry.wheel_angle = r0_3;
			L_00200F = (uint8_t)OBJECT(current_object)->name_entry.turn_steps == 0u;
		}
		if (L_00200F)
		{
			r0_2 = SCREEN(screen)->wheel;
			r0_2 = r0_2 >> 4;
			r1 = rom_002D6C[r0_2];
			if (r1 != 0u)
			{
				turn_delay = OBJECT(current_object)->name_entry.turn_delay;
				OBJECT(current_object)->name_entry.turn_delay = turn_delay - r1;
				if ((int8_t)turn_delay <= (int8_t)r1)
				{
					OBJECT(current_object)->name_entry.turn_delay = 0xF;
					r1 = 4u;
					if (r0_2 >> 2 != 0u)
						r1 = (uint8_t)(0u - r1);
					OBJECT(current_object)->name_entry.turn_steps = r1;
				}
			}
			else
			{
				buttons_pressed_2 = (uint16_t)SCREEN(screen)->buttons_pressed;
				if ((buttons_pressed_2 & 0x200u) != 0u)
				{
					if ((buttons_pressed_2 & 0x400u) == 0u)
					{
						r0_2 = 0xFFu;
						r1 = 0xFu;
						turn_delay = OBJECT(current_object)->name_entry.turn_delay;
						OBJECT(current_object)->name_entry.turn_delay = turn_delay - r1;
						if ((int8_t)turn_delay <= (int8_t)r1)
						{
							OBJECT(current_object)->name_entry.turn_delay = 0xF;
							r1 = 4u;
							if (r0_2 >> 2 != 0u)
								r1 = (uint8_t)(0u - r1);
							OBJECT(current_object)->name_entry.turn_steps = r1;
						}
					}
				}
				else if ((buttons_pressed_2 & 0x400u) != 0u)
				{
					r0_2 = 0u;
					r1 = 0xFu;
					turn_delay = OBJECT(current_object)->name_entry.turn_delay;
					OBJECT(current_object)->name_entry.turn_delay = turn_delay - r1;
					if ((int8_t)turn_delay <= (int8_t)r1)
					{
						OBJECT(current_object)->name_entry.turn_delay = 0xF;
						r1 = 4u;
						if (r0_2 >> 2 != 0u)
							r1 = (uint8_t)(0u - r1);
						OBJECT(current_object)->name_entry.turn_steps = r1;
					}
				}
				else
					OBJECT(current_object)->name_entry.turn_delay = 1;
			}
		}
		OBJECT(current_object)->name_entry.letter = ((OBJECT(current_object)->name_entry.wheel_angle + 0x40u) & 0xFFFu) >> 7;
		do
		{
			target1 = L_00209D ? 0x2090u : rom_002088[OBJECT(current_object)->name_entry.gas_step];
			switch (target1)
			{
			case 0x002090u:
				if (!L_00209D && SCREEN(screen)->gas >= 0x40u)
					L_002133 = true;
				else
				{
					if (!L_00209D)
						OBJECT(current_object)->name_entry.gas_step++;
					L_00209D = false;
					if (SCREEN(screen)->gas < 0xC0u)
					{
						buttons_pressed = (uint16_t)SCREEN(screen)->buttons_pressed;
						L_0020B6 = (buttons_pressed & 0x100u) != 0u || (buttons_pressed & 0x40u) != 0u;
						if (!L_0020B6)
							L_002133 = true;
					}
					if (L_0020B6 || !L_002133)
					{
						L_0020B6 = false;
						letter = OBJECT(current_object)->name_entry.letter;
						r1_2 = OBJECT(current_object)->name_entry.place;
						do
						{
							do
							{
								target2 = L_00212C ? 0x20D4u : L_0020FB ? 0x20D4u : rom_0020C8[r1_2];
								switch (target2)
								{
								case 0x0020D4u:
									do
									{
										if (L_002130 || !L_00212C)
										{
											if (L_002130 || !L_0020FB)
											{
												if (L_002130 || letter == 0x1Eu)
												{
													L_002130 = false;
													OBJECT(current_object)->name_entry.gas_step--;
													L_002133 = true;
												}
											}
										}
										if (L_002133)
											break;
										if (L_00212C || L_0020FB || letter != 0x1Fu)
										{
											if (!L_00212C)
											{
												L_0020FB = false;
												stack_4 = letter;
												stack_8 = r1_2;
												r0 = rom_002D98[r1_2];
												request_sound(r0);
												r1_2 = stack_8;
												r0_2 = stack_4;
												t2 = r0_2;
												r0 = mem_read8(rom_002D00_ADDRESS + r0_2);
												if (t2 == 0x1Du)
													r0 = 0x20u;
												mem_write8(current_object + 0xA0u + r1_2, r0);
												r1_2 = (r1_2 & ~0xFFu) | (uint8_t)(r1_2 + 1u);
											}
											if (L_00212C || (uint8_t)r1_2 != 3u)
											{
												L_00212C = false;
												OBJECT(current_object)->name_entry.place = r1_2;
												L_002130 = true;
											}
										}
									} while (L_002130);
									if (L_002133)
										break;
									break;
								case 0x0020E2u:
									if (letter == 0x1Eu)
									{
										mem_write8(current_object + 0xA0u + r1_2, 0u);
										r1_2 = (uint8_t)(r1_2 - 1u);
										L_00212C = true;
									}
									if (L_00212C)
										break;
									L_0020FB = letter != 0x1Fu;
									if (L_0020FB)
										break;
									break;
								default:
									invalid_jump(target2);
								}
								if (L_002133)
									break;
							} while (L_00212C);
							if (L_002133)
								break;
						} while (L_0020FB);
					}
				}
				if (L_002133)
				{
					L_002133 = false;
					r1_2 = OBJECT(current_object)->name_entry.place;
					r0 = rom_002D00[OBJECT(current_object)->name_entry.letter];
					if ((game_frame_count & 4u) == 0u)
						r0 = 0u;
					mem_write8(current_object + 0xA0u + r1_2, r0);
					timer = OBJECT(current_object)->name_entry.timer;
					OBJECT(current_object)->name_entry.timer = timer - 1u;
					if (timer != 1u)
					{
						r0_2 = OBJECT(current_object)->name_entry.timer;
						r0_2 = r0_2 / 0x1Eu;
						r0_2 = binary_to_bcd(r0_2);
						time_digits_draw(r0_2);
						r19 = current_object + 0xA4u;
						draw_text(r19);
						r19 = current_object + 0x9Au;
						ignored = draw_text(r19);
						return;
					}
				}
				break;
			case 0x00209Du:
				L_00209D = true;
				break;
			default:
				invalid_jump(target1);
			}
		} while (L_00209D);
		OBJECT(current_object)->name_entry.open = 0u;
		mem_write8(current_object + 0xA0u + r1_2, 0u);
		r0_2 = OBJECT(current_object)->unk_A0;
		if (r0_2 == 0u)
			OBJECT(current_object)->unk_A0 = 0x2D2D2Du;
		else
		{
			r10 = 0x2D7Cu;
			while ((int32_t)ROM32(r10) >= 0)
			{
				t3 = ROM32(r10);
				r10 += 4u;
				if (r0_2 == t3)
				{
					OBJECT(current_object)->unk_A0 = 0x2D2D2Du;
					break;
				}
			}
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->name_entry.timer = 0x3Cu;
		OBJECT(current_object)->name_entry.spin_frames = 0u;
		timer_2 = OBJECT(current_object)->name_entry.timer;
		OBJECT(current_object)->name_entry.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			SCREEN(screen)->button_lamps = 0u;
			SCREEN(screen)->game_step = 0x15u;
			race = SCREEN(screen)->race;
			RACE(race)->record_serial++;
			RACE(race)->finish_time = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
			RACE(race)->record_model = CAR(player_car)->model;
			RACE(race)->record_name = OBJECT(current_object)->name_entry.name;
			RACE(race)->unk_14 = (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u);
			RACE(race)->record_course = SCREEN(screen)->unk_A3 - 0x15u;
			OBJECT(current_object)->state++;
		}
		break;
	case 3:
		timer_2 = OBJECT(current_object)->name_entry.timer;
		OBJECT(current_object)->name_entry.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			SCREEN(screen)->button_lamps = 0u;
			SCREEN(screen)->game_step = 0x15u;
			race = SCREEN(screen)->race;
			RACE(race)->record_serial++;
			RACE(race)->finish_time = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
			RACE(race)->record_model = CAR(player_car)->model;
			RACE(race)->record_name = OBJECT(current_object)->name_entry.name;
			RACE(race)->unk_14 = (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u);
			RACE(race)->record_course = SCREEN(screen)->unk_A3 - 0x15u;
			OBJECT(current_object)->state++;
		}
		r19 = current_object + 0xA4u;
		draw_text(r19);
		r19 = current_object + 0x9Au;
		ignored = draw_text(r19);
		return;
	case 4:
		break;
	default:
		invalid_jump(rom_001ED0[OBJECT(current_object)->state]);
	}
	r19 = current_object + 0xA4u;
	draw_text(r19);
	r19 = current_object + 0x9Au;
	ignored = draw_text(r19);
}

/* A letter on the name entry's drum (font 2DE; index 0-31: A-Z ! ? . and the right arrow, the left arrow
   and ED, rom_002D00); ends when the entry closes (the name entry's open is 0). By state: 0 its angle
   index * 80 round the drum, y_offset -5; 1 for 40 frames (spin_frames) y_offset + 1, the drum dropping
   into place. Every frame its place from its angle less the wheel_angle (the sine table at 100C00, the
   cosine at 101400): x_offset, depth, a width that narrows toward the drum's edges (width_factor),
   mirrored (flags bit 6) with palette 27 where it faces away (the cosine under 2000, more than 60
   degrees from the front), 34 in front, 30 for the letter in front (shown 4 frames of 8: hidden, flags
   bit 5 cleared, when bit 2 of game_frame_count is clear); drawn in perspective (100000 / (depth + 100))
   about x E0, y 40. */
void name_entry_letter_update(void)
{
	uint32_t carry = 0;
	uint16_t r2 = 0;
	uint32_t r0 = 0, r1 = 0, r2_2 = 0, r3 = 0, r9 = 0, index = 0, object = 0;
	uint16_t spin_frames = 0;
	uint32_t t1 = 0;

	object = OBJECT(current_object)->name_entry_letter.name_entry;
	if ((uint8_t)(OBJECT(object)->unk_08 >> 8u) == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x2DEu;
		animation_start(r9);
		index = OBJECT(current_object)->name_entry_letter.index;
		OBJECT(current_object)->frame = rom_002D00[index];
		OBJECT(current_object)->name_entry_letter.angle = index << 7;
		OBJECT(current_object)->name_entry_letter.y_offset = -5;
		OBJECT(current_object)->name_entry_letter.spin_frames = 0x28u;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->name_entry_letter.y_offset++;
		spin_frames = OBJECT(current_object)->name_entry_letter.spin_frames;
		OBJECT(current_object)->name_entry_letter.spin_frames = spin_frames - 1u;
		if (spin_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 1:
		OBJECT(current_object)->name_entry_letter.y_offset++;
		spin_frames = OBJECT(current_object)->name_entry_letter.spin_frames;
		OBJECT(current_object)->name_entry_letter.spin_frames = spin_frames - 1u;
		if (spin_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00221C[OBJECT(current_object)->state]);
	}
	r0 = OBJECT(current_object)->name_entry_letter.angle;
	r0 = (uint16_t)(r0 - OBJECT(object)->unk_2C);
	r0 = r0 & 0xFFFu;
	r1 = (int16_t)rom_100C00[r0];
	r3 = (int16_t)rom_101400[r0];
	r0 = r3;
	r1 = (int32_t)r1 >> 7;
	r3 = 0u - ((int32_t)r3 >> 7);
	OBJECT(current_object)->name_entry_letter.x_offset = r1;
	OBJECT(current_object)->name_entry_letter.depth = r3;
	r2 = 0x34u;
	if ((uint8_t)(OBJECT(object)->unk_08 >> 16u) == OBJECT(current_object)->name_entry_letter.index)
	{
		if ((game_frame_count & 4u) == 0u)
		{
			OBJECT(current_object)->flags &= 0xDFu;
			return;
		}
		r2 = 0x30u;
	}
	t1 = r0 - 0x2000u;
	r0 = t1;
	if ((int32_t)t1 < 0)
	{
		OBJECT(current_object)->flags |= 0x40u;
		r0 = 0u - r0;
		r2 = 0x27u;
	}
	else
	{
		OBJECT(current_object)->flags &= 0xBFu;
		r0 = r0 * 3u;
	}
	OBJECT(current_object)->name_entry_letter.width_factor = (int32_t)(r0 * 3u) >> 8;
	OBJECT(current_object)->palette = r2;
	r2_2 = OBJECT(current_object)->name_entry_letter.y_offset;
	r0 = divide_s32(0x100000u, r3 + 0x100u);
	r1 = r1 * r0;
	r2_2 = r2_2 * r0;
	carry = (r1 >> 11) & 1u;
	r1 = (uint16_t)(((int32_t)r1 >> 12) + carry);
	carry = (r2_2 >> 11) & 1u;
	r2_2 = (uint16_t)(((int32_t)r2_2 >> 12) + carry + 0x40u);
	OBJECT(current_object)->x = r1 + 0xE0u;
	OBJECT(current_object)->y = r2_2;
	OBJECT(current_object)->depth_layer = r2_2;
	carry = (r0 >> 3) & 1u;
	r0 = ((int32_t)r0 >> 4) + carry;
	OBJECT(current_object)->height_scale = r0;
	r0 = (int32_t)(r0 * OBJECT(current_object)->name_entry_letter.width_factor) >> 8;
	OBJECT(current_object)->width_scale = r0;
	r0 = draw_object(r0);
}

/* Shows the name entry on the cabinet LCD: 0 clears it (clear_lcd), shows the DJ's face (screen unk_F35
   = FF) and writes the time (7 characters of the entry's time_text) at F13; then every frame the name's
   three characters at F1C, in the LCD's codes (7F -> 7E and | -> 7F, its arrows; \ -> 65); ends when the
   entry closes. It means to write spaces after the end of the name, but tests the address (tb R10)
   instead of the character, so it never does. */
void name_entry_lcd_update(void)
{
	uint8_t r0 = 0;
	uint32_t r9 = 0, r10 = 0, r11 = 0, object = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		clear_lcd();
		SCREEN(screen)->dj_face = 0xFFu;
		move_string(OBJECT(current_object)->name_entry_lcd.name_entry + 0xAAu, 7u, screen + 0xF13u, 7u, 1u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_002304[OBJECT(current_object)->state]);
	}
	object = OBJECT(current_object)->name_entry_lcd.name_entry;
	r10 = object + 0xA0u;
	r11 = screen + 0xF1Cu;
	r9 = 3u;
	do
	{
		r0 = mem_read8(r10);
		r10++;
		if (r10 == 0u)
		{
			do
			{
				mem_write8(r11, 0x20u);
				r9--;
				r11++;
			} while (r9 != 0u);
			break;
		}
		if (r0 == 0x7Fu)
			r0 = 0x7Eu;
		if (r0 == 0x7Cu)
			r0 = 0x7Fu;
		if (r0 == 0x5Cu)
			r0 = 0x65u;
		mem_write8(r11, r0);
		r9--;
		r11++;
	} while (r9 != 0u);
	if ((uint8_t)(OBJECT(object)->unk_08 >> 8u) == 0u)
		OBJECT(current_object)->flags = 0u;
}

/* The player's car on the certificate screen (animation rom_002D9C by the car's model, car unk_60;
   palette rom_002DAC) at x 40, y C0: state 0 sets it up and runs into name_entry_drivers_update, which
   with state 1 draws it; then it draws itself every frame. */
void name_entry_car_update(void)
{
	uint32_t r0 = 0, r9 = 0, model = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_002384[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_002384[state]);
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	model = CAR(player_car)->model;
	r9 = rom_002D9C[model];
	OBJECT(current_object)->palette = rom_002DAC[model];
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation_start(r9);
	OBJECT(current_object)->x = 0x40u;
	OBJECT(current_object)->y = 0xC0u;
	OBJECT(current_object)->frame = 0u;
	name_entry_drivers_update();
}

/* The car's two drivers standing by it on the certificate screen (animation rom_002DBC by the car's
   model, car unk_60; palette rom_002DCC) at x 60, y C8: state 0 sets them up and runs into
   name_entry_course_name_update, which with state 1 draws them; then drawn every frame. */
void name_entry_drivers_update(void)
{
	uint32_t r0 = 0, r9 = 0, model = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0023D4[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0023D4[state]);
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	model = CAR(player_car)->model;
	r9 = rom_002DBC[model];
	OBJECT(current_object)->palette = rom_002DCC[model];
	OBJECT(current_object)->depth_layer = 0x19Du;
	animation_start(r9);
	OBJECT(current_object)->x = 0x60u;
	OBJECT(current_object)->y = 0xC8u;
	OBJECT(current_object)->frame = 0u;
	name_entry_course_name_update();
}

/* The course's name on the certificate (Australia Course...: animation rom_002E24 by the course, screen
   unk_A3 - 15; palette rom_002E38) at x E0, y 40: state 0 sets it up; drawn every frame. */
void name_entry_course_name_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_002424[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_002424[state]);
	}
	OBJECT(current_object)->state++;
	r0 = SCREEN(screen)->unk_A3;
	r0 = (uint8_t)(r0 - 0x15u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->palette = rom_002E38[r0];
	r9 = rom_002E24[r0];
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation_start(r9);
	OBJECT(current_object)->x = 0xE0u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->y = 0x40u;
	r0 = draw_object(r0);
}

/* The ranking in the attract mode (game mode 5, game_mode_ranking): OutRunner of OutRunners, the record
   of three courses in turn. By state: 0 starts object_handlers_2CB8 (01AA01, 01A6CF, which
   game_mode_ranking has started too), ranking_background and the two halves of the title
   (ranking_title_left, ranking_title_right); 1 clears course_over; once the screen's fade is over
   (screen unk_94 bit 0): after three courses ends, else courses_shown + 1, 60 frames (frames_left), and
   starts ranking_course_banner and ranking_record for the course screen unk_9D; 2 when the 60 frames are
   over sets course_over for a frame (the background fades out) and moves screen unk_9D to the next of
   the ten courses, back to 1. */
void ranking_update(void)
{
	uint8_t r0 = 0, record_course = 0;
	uint32_t r10 = 0, r21 = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->ranking.courses_shown = 0u;
		r10 = 0x2CB8u;
		start_object_list(r10);
		r21 = start_object();
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 4u, ranking_background_update_ADDRESS);
		r21 = start_object();
		mem_write32(r21 + 4u, ranking_title_left_update_ADDRESS);
		r21 = start_object();
		mem_write32(r21 + 4u, ranking_title_right_update_ADDRESS);
		OBJECT(current_object)->state++;
		return;
	case 1:
		OBJECT(current_object)->ranking.course_over &= 0xDFFFu;
		if ((SCREEN(screen)->flags & 1u) != 0u)
			return;
		if (OBJECT(current_object)->ranking.courses_shown >= 3u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->ranking.courses_shown++;
		OBJECT(current_object)->ranking.frames_left = 0x3Cu;
		record_course = SCREEN(screen)->record_course;
		r21 = start_object();
		mem_write32(r21 + 4u, ranking_course_banner_update_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, record_course);
		r21 = start_object();
		mem_write32(r21 + 4u, ranking_record_start_ADDRESS);
		mem_write8(r21 + 9u, record_course);
		OBJECT(current_object)->state++;
		return;
	case 2:
		frames_left = OBJECT(current_object)->ranking.frames_left;
		OBJECT(current_object)->ranking.frames_left = frames_left - 1u;
		if (frames_left != 1u)
			return;
		OBJECT(current_object)->ranking.course_over |= 0x2000u;
		r0 = (uint8_t)(SCREEN(screen)->record_course + 1u);
		if (r0 >= 0xAu)
			r0 = 0u;
		SCREEN(screen)->record_course = r0;
		OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(rom_002488[OBJECT(current_object)->state]);
	}
}

/* The picture of the course behind the ranking, while the game mode is the ranking (screen unk_80 5). By
   state: 0 courses_shown from the ranking; for the first course I/O C0001C = 6 (C8001C = 86 for screen
   2) and a fade in (screen unk_153 1, unk_94 bit 0); ranking_background_load; 1 waits while the pages
   are unpacked (screen unk_B00), then for the later courses a fade in (unk_153 3); 2 when the ranking
   says course_over: after the third course a fade out (unk_153 0) and state 4, else a fade out (unk_153
   2); 3 when the fade is over, back to 0 for the next course's picture; 4 nothing more. Every frame, in
   any game mode, the picture scrolls by 3 pixels (scroll_x - 3). */
void ranking_background_update(void)
{
	uint32_t scroll_x_address = 0;

	if (SCREEN(screen)->game_mode == 5u)
	{
		switch (OBJECT(current_object)->state)
		{
		case 0:
			OBJECT(current_object)->ranking_background.courses_shown = (uint8_t)OBJECT(OBJECT(current_object)->ranking_background.ranking)->unk_10;
			if (OBJECT(current_object)->ranking_background.courses_shown == 0u)
			{
				if (SCREEN(screen)->index != 0u)
					mem_write8(0xC8001Cu, 0x86u);
				else
					mem_write8(IO_C0001C, 6u);
				SCREEN(screen)->fade_kind = 1u;
				SCREEN(screen)->flags |= 1u;
			}
			ranking_background_load();
			OBJECT(current_object)->state++;
		case 1:
			if (SCREEN(screen)->unpack_state != 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 0u)
			{
				SCREEN(screen)->fade_kind = 3u;
				SCREEN(screen)->flags |= 1u;
			}
			OBJECT(current_object)->state++;
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->unk_02 & 0x2000u) == 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 3u)
			{
				SCREEN(screen)->fade_kind = 2u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state++;
				if ((SCREEN(screen)->flags & 1u) == 0u)
					OBJECT(current_object)->state = 0u;
			}
			else
			{
				SCREEN(screen)->fade_kind = 0u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state = 4u;
			}
			break;
		case 2:
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->unk_02 & 0x2000u) == 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 3u)
			{
				SCREEN(screen)->fade_kind = 2u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state++;
				if ((SCREEN(screen)->flags & 1u) == 0u)
					OBJECT(current_object)->state = 0u;
			}
			else
			{
				SCREEN(screen)->fade_kind = 0u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state = 4u;
			}
			scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
			mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
			return;
		case 3:
			if ((SCREEN(screen)->flags & 1u) == 0u)
				OBJECT(current_object)->state = 0u;
			scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
			mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
			return;
		case 4:
			break;
		default:
			invalid_jump(rom_002564[OBJECT(current_object)->state]);
		}
	}
	scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
	mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
}

/* The left half of the ranking's title, OutRunner of OutRunners (animation 2D2, palette 2B2) at x 0, y
   30: state 0 sets it up; drawn every frame. */
void ranking_title_left_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_002620[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B2u;
		r9 = 0x2D2u;
		animation_start(r9);
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_002620[state]);
	}
	r0 = draw_object(r0);
}

/* The right half of the ranking's title, OutRunner of OutRunners (animation 2D3, palette 2B3) at x 13F,
   y 30: state 0 sets it up; drawn every frame. */
void ranking_title_right_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00266C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B3u;
		r9 = 0x2D3u;
		animation_start(r9);
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = 0x13Fu;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00266C[state]);
	}
	r0 = draw_object(r0);
}

/* The scroll with the course's name under the ranking's title (Australia Course...: animation rom_002DFC
   by course, palette rom_002E10) at x A0, y 50, for 90 frames (frames_left). */
void ranking_course_banner_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0026B8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r0 = OBJECT(current_object)->ranking_course_banner.course;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = rom_002E10[r0];
		r9 = rom_002DFC[r0];
		animation_start(r9);
		OBJECT(current_object)->depth_layer = 0x19Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->ranking_course_banner.frames_left = 0x5Au;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_0026B8[state]);
	}
	frames_left = OBJECT(current_object)->ranking_course_banner.frames_left;
	OBJECT(current_object)->ranking_course_banner.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* Sets up the course's record line from the records at 20E300 (by course: the initials, '-' shown as
   '.', and a 0 into text, then the time as 5'00"00, time_to_text), starts a ranking_record_character for
   each of its 11 characters (index, course, itself) and ends at once. */
void ranking_record_start(void)
{
	uint8_t r1 = 0, r4 = 0;
	uint32_t r0 = 0, r3 = 0, r11 = 0, r21 = 0, object = 0;
	uint8_t t1 = 0;

	object = current_object + 0x80u;
	r3 = OBJECT(current_object)->ranking_record.course;
	r3 = (uint8_t)(r3 * 6u);
	r11 = 0x20E301u + r3;
	r4 = mem_read8(r11);
	r11++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	OBJECT(object - 0x80u)->ranking_record.text = r4;
	t1 = mem_read8(r11);
	r11++;
	r4 = t1;
	object++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	AT8(OBJECT(object - 0x81u)->unk_80, 1) = r4;
	r4 = mem_read8(r11);
	object++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	AT8(OBJECT(object - 0x82u)->unk_80, 2) = r4;
	object++;
	AT8(OBJECT(object - 0x83u)->unk_80, 3) = 0u;
	r0 = course_record_times[r3 >> 1];
	object++;
	time_to_text(r0, object);
	r0 = 0xBu;
	r1 = 0u;
	do
	{
		r21 = start_object();
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r1);
		mem_write8(r21 + 0xAu, OBJECT(current_object)->ranking_record.course);
		mem_write32(r21 + 4u, ranking_record_character_update_ADDRESS);
		r1++;
		r0--;
	} while (r0 != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* A character of a course's record line (font 2DE, palette 27): 0 its character from the record's text
   by index, at y 80 and x -A0 plus 16 pixels for each place of index; 1 slides in, 32 pixels a frame for
   7 frames (slide_steps), then starts a ranking_record_car (each of the 11 characters starts one, all at
   the same place); 2 index + 2 frames later (colour_delay) takes the colours of the record's car
   (rom_002DEC by its model); 3 58 frames after its start (frames_left) slides out to the left, 16 pixels
   a frame, and ends at x -20. */
void ranking_record_character_update(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0, course = 0, ranking_record = 0;
	uint8_t colour_delay = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00279C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_character.frames_left = 0x3Au;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x27u;
		r9 = 0x2DEu;
		animation_start(r9);
		ranking_record = OBJECT(current_object)->ranking_record_character.ranking_record;
		r0 = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->depth_layer = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->frame = mem_read8(ranking_record + 0x80u + r0);
		OBJECT(current_object)->x = rom_002D2A[r0];
		OBJECT(current_object)->y = rom_002D40[r0];
		OBJECT(current_object)->ranking_record_character.slide_steps = 0u;
	case 1:
		OBJECT(current_object)->x += 0x20u;
		OBJECT(current_object)->ranking_record_character.slide_steps++;
		if (OBJECT(current_object)->ranking_record_character.slide_steps >= 7u)
		{
			r21 = start_object();
			mem_write32(r21 + 4u, ranking_record_car_update_ADDRESS);
			mem_write8(r21 + 9u, OBJECT(current_object)->ranking_record_character.course);
			OBJECT(current_object)->state++;
		}
		r0 = (r0 & ~0xFFu) | OBJECT(current_object)->ranking_record_character.index;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 2u);
		OBJECT(current_object)->ranking_record_character.colour_delay = r0;
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			r0 = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = rom_002DEC[r0];
			OBJECT(current_object)->state++;
		}
		break;
	case 2:
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			r0 = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = rom_002DEC[r0];
			OBJECT(current_object)->state++;
		}
		frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
		OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
		if (frames_left != 1u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0xFFE0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0xFFE0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_00279C[state]);
	}
	frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
	OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->x -= 0x10u;
	if (OBJECT(current_object)->x != 0xFFE0u)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* The record holder's car under the course's record line (animation rom_002DDC by the model of the
   course's record, palette rom_002DAC; size 50, y 88): drives in from the left 16 pixels a frame to x
   108 or more, turns round (frame 9 down to 4), waits 15 frames (wait_frames), drives back left 16
   pixels a frame and ends at x 0. State 5 (drawn only) is not set by its own code. */
void ranking_record_car_update(void)
{
	uint32_t r0 = 0, r9 = 0, course = 0;
	uint16_t wait_frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00287C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_car.wait_frames = 0xFu;
		OBJECT(current_object)->draw_mode = 0xAu;
		course = OBJECT(current_object)->ranking_record_car.course;
		r0 = course_records[(uint8_t)(course * 6u)];
		r9 = rom_002DDC[r0];
		OBJECT(current_object)->palette = rom_002DAC[r0];
		animation_start(r9);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x88u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 1:
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->frame--;
		if (OBJECT(current_object)->frame != 4u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 3:
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 5:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00287C[state]);
	}
}

/* Counts the steps of the passing cars' path (0067B2: size, x, y; closed by 0) into path_steps, then
   runs passing_traffic_update. */
void passing_traffic_start(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	r10 = 0x67B2u;
	r0 = 0u;
	while (ROM16(r10) != 0u)
	{
		r0++;
		r10 += 6u;
	}
	OBJECT(current_object)->passing_traffic.path_steps = r0;
	OBJECT(current_object)->handler = passing_traffic_update_ADDRESS;
	passing_traffic_update();
}

/* Now and then a car passes on the road behind the certificate: by state, 0 one frame in 16 at random
   (random_number) starts a passing_car on the path's last step (path_steps), 1 then waits 10 frames
   (wait). */
void passing_traffic_update(void)
{
	uint32_t r0 = 0, r21 = 0;
	uint16_t wait = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = random_number();
		if ((r0 & 0xFu) == 0u)
		{
			r21 = start_object();
			mem_write32(r21 + 4u, passing_car_update_ADDRESS);
			mem_write16(r21 + 0x10u, OBJECT(current_object)->passing_traffic.path_steps);
			OBJECT(current_object)->passing_traffic.wait = 0xAu;
			OBJECT(current_object)->state++;
		}
		return;
	case 1:
		wait = OBJECT(current_object)->passing_traffic.wait;
		OBJECT(current_object)->passing_traffic.wait = wait - 1u;
		if (wait == 1u)
			OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(rom_002960[OBJECT(current_object)->state]);
	}
}

/* A car on the road behind the certificate (animation rom_002DDC by a random model, palette rom_002DAC;
   its sound, rom_0067A9 by the model, requested as it starts): state 0 picks frame C and state 1, or
   frame 1 and state 2 when the random number is negative; then a frame a step it goes from the path's
   last step (0067B2: size 370 near the bottom edge) to its first (size 5 on the horizon), at the path's
   x in state 1 or at 96 - x on the other side of the road in state 2; ends past the first step. */
void passing_car_update(void)
{
	uint8_t r0_2 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r10 = 0, step = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = random_number();
		r3 = r0;
		r0 &= 7u;
		r9 = rom_002DDC[r0];
		OBJECT(current_object)->palette = rom_002DAC[r0];
		animation_start(r9);
		r0_2 = rom_0067A9[r0];
		request_sound(r0_2);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 3u;
		OBJECT(current_object)->frame = 0xCu;
		OBJECT(current_object)->state++;
		if ((int32_t)r3 >= 0)
		{
			step = (uint16_t)OBJECT(current_object)->passing_car.step;
			t1 = step - 1u;
			r0 = t1;
			if ((int16_t)t1 < 0)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			OBJECT(current_object)->passing_car.step = r0;
			r10 = 0x67B2u + r0 * 3u * 2u;
			r0 = ROM16(r10);
			OBJECT(current_object)->width_scale = r0;
			OBJECT(current_object)->height_scale = r0;
			r10 += 2u;
			OBJECT(current_object)->x = ROM16(r10);
			OBJECT(current_object)->y = ROM16(r10 + 2u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->frame = 1u;
		OBJECT(current_object)->state++;
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		t2 = step - 1u;
		r0 = t2;
		if ((int16_t)t2 < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0_3 = ROM16(r10);
		OBJECT(current_object)->width_scale = r0_3;
		OBJECT(current_object)->height_scale = r0_3;
		r10 += 2u;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->x = r0;
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	case 1:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		t1 = step - 1u;
		r0 = t1;
		if ((int16_t)t1 < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		r10 += 2u;
		OBJECT(current_object)->x = ROM16(r10);
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	case 2:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		t2 = step - 1u;
		r0 = t2;
		if ((int16_t)t2 < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0_3 = ROM16(r10);
		OBJECT(current_object)->width_scale = r0_3;
		OBJECT(current_object)->height_scale = r0_3;
		r10 += 2u;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->x = r0;
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0029A4[OBJECT(current_object)->state]);
	}
}

/* Loads the picture of the course screen unk_9D (rom_002D20 picks it) into the screen's first tilemap:
   its register addresses for the screen (002C44, into 80-AC), the tile bank (rom_002C18), the palette
   (rom_0318CC, queue_palette_list), the four tilemap pages (rom_109694, with the screen's page numbers
   from rom_002CAC, in page_list; queue_tile_pages), start_palette_fades with rom_031B14, zoom 1:1, zoom
   centre and the second tilemap's scroll 0, the pages, the scroll's start (rom_002CB4: x 360 for screen
   1, 4A0 for screen 2; y 60), its priority 1 and the second tilemap off in the mixer (rom_002CA4: screen
   182 and 186; unk_94 bit 2); unk_20E027 = FF to send the registers. */
void ranking_background_load(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0, index = 0;
	uint32_t stack_4 = 0;

	r19 = 0x2C44u + SCREEN(screen)->index * 0xCu * 4u;
	OBJECT(current_object)->ranking_background.tile_bank_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.pages_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.scroll_x_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.scroll_y_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.zoom_x_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.zoom_y_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_x_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_y_address = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_A0 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_A4 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->ranking_background.second_scroll_x_address = mem_read32(r19);
	OBJECT(current_object)->ranking_background.second_scroll_y_address = mem_read32(r19 + 4u);
	unk_20E027 = 0xFFu;
	r0 = rom_002D20[SCREEN(screen)->record_course];
	mem_write8(OBJECT(current_object)->ranking_background.tile_bank_address, rom_002C18[r0]);
	stack_4 = rom_031B14[r0];
	r19 = rom_0318CC[r0];
	r10 = rom_109694[r0] + 0x10u;
	r11 = current_object + 0xB0u;
	r0 = rom_002CAC[SCREEN(screen)->index];
	r9 = 4u;
	do
	{
		mem_write32(r11, mem_read32(r10) | (r0 & ~0xFFFFFFu));
		r11 += 4u;
		r0 = r0 << 8;
		r9--;
		r10 += 4u;
	} while (r9 != 0u);
	mem_write32(r11, 0xFFFFFFFFu);
	r10 = current_object + 0xB0u;
	queue_tile_pages(r10);
	queue_palette_list(r19);
	r19 = stack_4;
	start_palette_fades(r19);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_x_address, 0x200u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_y_address, 0x200u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_center_x_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_center_y_address, 0u);
	mem_write16(OBJECT(current_object)->unk_A0, 0u);
	mem_write16(OBJECT(current_object)->unk_A4, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.second_scroll_x_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.second_scroll_y_address, 0u);
	index = SCREEN(screen)->index;
	mem_write32(OBJECT(current_object)->ranking_background.pages_address, byte_swap32(rom_002CAC[index]));
	mem_write16(OBJECT(current_object)->ranking_background.scroll_x_address, rom_002CB4[index]);
	mem_write16(OBJECT(current_object)->ranking_background.scroll_y_address, 0x60u);
	r10 = rom_002CA4[index];
	r0 = mem_read32(r10);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	mem_write32(r10, (r0 & ~0xFFu) | (uint8_t)(r0 | 1u));
	r10 += 4u;
	r0 = mem_read32(r10);
	mem_write32(r10, (r0 & ~0xFFFFu) | (r0 & 0xFFF0u));
	SCREEN(screen)->flags |= 4u;
}

/* Game mode 6, a game (check_game_start starts it at step 0) and the demo's race (game_mode_demo_race,
   from step 10), by game_step: 0 sound 21 (queue_sound), the screen's two lamps on (unk_F20),
   lcd_restart_frames cleared (the LCD's timer), a fade out, race game_over cleared; 1 when the fade is
   over, waits until the screen's player is in the list of the race's players (from byte 1F of the
   race_copy record of host_player), then the host itself (not on ISDN) writes 1 to the link board's
   80000A; 2 the car select: demo_flags bit 8 clear, the wheel motor off (unk_20E0D0), game_start_time
   from the backup RAM's seconds (701F10), a game counted on the seat (701F14 + 2 * index,
   bookkeeping_increment16), C00074 = rom_003463 by index, the sound off (reset_screen_sound) unless the
   entry countdown's music plays (entry_countdown_shown), clear_screen, clear_race_state, palette 031E7C,
   in_game, host_record (the host's race_copy record), the stages (screen unk_A0-A6) 0, the seat's lamps
   on (checkpoint_lamp 2, leader_lamp 8), car_select_start started, a fade in, the screen on; 3 the car
   select (car_select_run goes on to 4); 4 a fade out, entry_countdown_shown cleared, the lamps on; 5
   when the fade is over, race ready cleared, silence_and_clear_screen, the screen off (C0001C = 4), the
   stage's palette (queue_stage_load), the screen's car (object_handlers_346C: player_car_update, its
   place from screen 320 + player), step 23; 23 once the car has published its state (race_copy
   published): race_running, the race's objects (object_handlers_3474; the demo's object_handlers_34C4),
   24; 24 and 25 a frame each; 26 step 11, the input ring's places to 0, the screen on; 11 a fade in,
   step 6; 6 when it is over, 7; 7 the race, until its ending is over (flags bit 1,
   race_car_start_ending): race_running cleared, a fade out, 8; 8 when it is over, the lamps on, 9; 9 the
   text and the tilemaps off (clear_text_layer, reset_tile_layers), no objects, step 12; 12 the route
   map: short of the goal (race_copy unk_5E clear) the sound off, the song title Meaning of the Light
   (start_song_title with 003504) and sound 0B; palette 031E84, route_map_update (object_handlers_34EC),
   a fade in; 13 when it is over, 14; 14 the map until START (buttons_pressed bit 6) or route_map_update
   sets 16; 15 (nothing sets it) 16; 16 with a new course record (course_record_beaten) a fade out and
   18, else 21; 18 when the fade is over, no objects, name_entry_update started, 19; 19 a fade in, 20; 20
   the name entry (name_entry_update sets 21); 21 set_player_ready and 22; 21 and 22 once every player of
   the race is ready (players_not_ready): a fade out, fade_out_music, 17; 17 when the fade is over, the
   game's end: the time played (the backup's seconds less game_start_time) added to the seat's total
   (701F18 + 4 * index) and kept if the longest (701F20) or the shortest (701F22), a game counted at the
   stage level reached (screen unk_A4, 6 at the goal: 701F24 + 2 * it), C00074 = rom_003465 then
   rom_003467 by index, sound 1E, the sound and the tilemaps off, in_game, screen unk_31C and unk_318
   cleared, the race record's car part cleared (race_record_clear), no objects, race game_over (the
   screens in the attract modes go to the ranking, race_entries_update), the lamps off, clear_lcd, 28; 28
   game mode 5 (the ranking). The demo: 10 silence_and_clear_screen, clear_race_state, the stage and the
   car model from 0034F4 by attract_car (the stage into screen unk_A0, A1, A3, A5), host_record its own
   record at screen + 490 (demo_player_count 1, demo_players this screen's player, demo_stage), the model
   into car model and race unk_49, palette 031E88, then as 27; 27 waits until race_copy published is
   clear, then as 5. */
void game_mode_play(void)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, player = 0;
	uint32_t r0_2 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0, r21 = 0, index = 0;
	uint8_t t1 = 0;
	uint32_t flags = 0;

	switch (SCREEN(screen)->game_step)
	{
	case 0:
		r0 = 0x21u;
		queue_sound(r0);
		SCREEN(screen)->button_lamps = 0xFFFFu;
		lcd_restart_frames = 0u;
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		RACE(SCREEN(screen)->race)->game_over = 0u;
		return;
	case 1:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			r10 = rom_00E0A0[SCREEN(screen)->host_player] + 0x20E01Fu;
			r0_2 = mem_read8(r10);
			r10++;
			if (r0_2 == 0u)
				return;
			player = SCREEN(screen)->player;
			do
			{
				t1 = mem_read8(r10);
				zero = (uint8_t)(t1 - player) == 0u;
				r10++;
				r0_2--;
			} while ((r0_2 != 0u) & (t1 != player));
			if (zero)
			{
				if (SCREEN(screen)->player == SCREEN(screen)->host_player && isdn_mode == 0u)
					mem_write8(0x80000Au, 1u);
				SCREEN(screen)->game_step++;
			}
		}
		return;
	case 2:
		SCREEN(screen)->demo_flags &= ~0x100u;
		unk_20E0D0[SCREEN(screen)->index] = 0u;
		SCREEN(screen)->game_start_time = mem_read32(SOUND_701F10);
		r7 = SOUND_RAM + 0x1F14u + SCREEN(screen)->index * 2u;
		bookkeeping_increment16(r7);
		mem_write8(IO_C00074, rom_003463[SCREEN(screen)->index]);
		if (SCREEN(screen)->entry_countdown_shown == 0u)
			reset_screen_sound();
		clear_screen();
		clear_race_state();
		r19 = rom_031E7C;
		queue_palette(r19);
		SCREEN(screen)->in_game = 0xFFu;
		SCREEN(screen)->host_record = rom_00E0A0[SCREEN(screen)->host_player] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_A4 = 0u;
		SCREEN(screen)->unk_A0 = 0u;
		SCREEN(screen)->unk_A5 = 0u;
		SCREEN(screen)->unk_A3 = 0u;
		SCREEN(screen)->unk_A6 = 0u;
		SCREEN(screen)->unk_A1 = 0u;
		SCREEN(screen)->unk_A2 = 0u;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
		mem_write32(r10 + 0xC0u, 2u);
		mem_write32(r10 + 0xC8u, 8u);
		r21 = start_object();
		mem_write32(r21 + 4u, car_select_start_ADDRESS);
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		SCREEN(screen)->game_step++;
		return;
	case 3:
		return;
	case 4:
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		SCREEN(screen)->entry_countdown_shown = 0u;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
		mem_write32(r10 + 0xC0u, 2u);
		mem_write32(r10 + 0xC8u, 8u);
		return;
	case 10:
		silence_and_clear_screen();
		clear_race_state();
		r10 = 0x34F4u + SCREEN(screen)->attract_car * 2u;
		r0 = ROM8(r10);
		SCREEN(screen)->unk_A0 = r0;
		SCREEN(screen)->unk_A3 = r0;
		SCREEN(screen)->unk_A1 = r0;
		SCREEN(screen)->unk_A4 = 0u;
		SCREEN(screen)->unk_A5 = r0;
		SCREEN(screen)->unk_A2 = 0u;
		r11 = screen + 0x490u;
		SCREEN(screen)->host_record = r11;
		SCREEN(r11 - 0x490u)->demo_player_count = 1u;
		SCREEN(r11 - 0x490u)->demo_players = SCREEN(screen)->player;
		SCREEN(r11 - 0x490u)->demo_stage = r0;
		r0 = rom_000001[r10];
		CAR(player_car)->model = r0;
		RACE(SCREEN(screen)->race)->unk_49 = r0;
		SCREEN(screen)->game_step = 0x1Bu;
		r19 = rom_031E88;
		queue_palette(r19);
		if (RACE(SCREEN(screen)->race_copy)->published != 0u)
			return;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		queue_stage_load();
		r10 = 0x346Cu;
		r21 = start_object_list(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->player));
		SCREEN(screen)->game_step = 0x17u;
		return;
	case 27:
		if (RACE(SCREEN(screen)->race_copy)->published != 0u)
			return;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		queue_stage_load();
		r10 = 0x346Cu;
		r21 = start_object_list(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->player));
		SCREEN(screen)->game_step = 0x17u;
		return;
	case 5:
		if ((SCREEN(screen)->flags & 1u) != 0u)
			return;
		RACE(SCREEN(screen)->race)->ready = 0u;
		silence_and_clear_screen();
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		queue_stage_load();
		r10 = 0x346Cu;
		r21 = start_object_list(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->player));
		SCREEN(screen)->game_step = 0x17u;
		return;
	case 23:
		if (RACE(SCREEN(screen)->race_copy)->published == 0u)
			return;
		SCREEN(screen)->race_running = 0xFFu;
		r10 = 0x3474u;
		if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
			r10 = 0x34C4u;
		start_object_list(r10);
		SCREEN(screen)->game_step++;
		return;
	case 24:
	case 25:
		SCREEN(screen)->game_step++;
		return;
	case 26:
		SCREEN(screen)->game_step = 0xBu;
		SCREEN(screen)->input_write_place = 0u;
		SCREEN(screen)->input_read_place = 0u;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		return;
	case 11:
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step = 6u;
		return;
	case 6:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			SCREEN(screen)->game_step++;
		return;
	case 7:
		flags = SCREEN(screen)->flags;
		SCREEN(screen)->flags = flags & ~2u;
		if ((flags & 2u) != 0u)
		{
			SCREEN(screen)->race_running = 0u;
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			SCREEN(screen)->game_step++;
		}
		return;
	case 8:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
			mem_write32(r10 + 0xC0u, 2u);
			mem_write32(r10 + 0xC8u, 8u);
			SCREEN(screen)->game_step++;
		}
		return;
	case 9:
		clear_text_layer();
		reset_tile_layers();
		SCREEN(screen)->game_step = 0xCu;
		clear_objects();
		return;
	case 12:
		if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
		{
			reset_screen_sound();
			r10 = 0x3504u;
			start_song_title(r10);
			r0 = 0xBu;
			request_sound(r0);
		}
		r19 = rom_031E84;
		queue_palette(r19);
		r10 = 0x34ECu;
		start_object_list(r10);
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 13:
		if ((SCREEN(screen)->flags & 1u) == 0u)
			SCREEN(screen)->game_step++;
		return;
	case 14:
		if ((SCREEN(screen)->buttons_pressed & 0x40u) != 0u)
		{
			SCREEN(screen)->game_step = 0x10u;
			return;
		}
		return;
	case 15:
		SCREEN(screen)->game_step = 0x10u;
		return;
	case 16:
		zero = course_record_beaten();
		if (zero == 0u)
		{
			SCREEN(screen)->game_step = 0x12u;
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			return;
		}
		SCREEN(screen)->game_step = 0x15u;
		return;
	case 18:
		if ((SCREEN(screen)->flags & 1u) == 0u)
		{
			clear_objects();
			SCREEN(screen)->game_step++;
			r21 = start_object();
			mem_write32(r21 + 4u, name_entry_update_ADDRESS);
		}
		return;
	case 19:
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 20:
		return;
	case 21:
		set_player_ready();
		SCREEN(screen)->game_step++;
		zero = players_not_ready();
		if (zero == 0u)
		{
			SCREEN(screen)->game_step = 0x11u;
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			fade_out_music();
		}
		return;
	case 22:
		zero = players_not_ready();
		if (zero == 0u)
		{
			SCREEN(screen)->game_step = 0x11u;
			SCREEN(screen)->fade_kind = 0u;
			SCREEN(screen)->flags |= 1u;
			fade_out_music();
		}
		return;
	case 17:
		if ((SCREEN(screen)->flags & 1u) != 0u)
			return;
		r9 = mem_read32(SOUND_701F10) - SCREEN(screen)->game_start_time;
		r7 = SOUND_RAM + 0x1F18u + SCREEN(screen)->index * 4u;
		bookkeeping_add32(r7, r9);
		if (mem_read16(SOUND_701F20) < (uint16_t)r9)
		{
			r7 = SOUND_701F20;
			bookkeeping_set16(r7, (uint16_t)r9);
		}
		if ((uint16_t)r9 < mem_read16(SOUND_701F22))
		{
			r7 = SOUND_701F22;
			bookkeeping_set16(r7, (uint16_t)r9);
		}
		r6 = SCREEN(screen)->unk_A4;
		if (RACE(SCREEN(screen)->race_copy)->unk_5E != 0u)
			r6 = 6u;
		r7 = SOUND_RAM + 0x1F24u + r6 * 2u;
		bookkeeping_increment16(r7);
		index = SCREEN(screen)->index;
		mem_write8(IO_C00074, rom_003465[index]);
		mem_write8(IO_C00074, rom_003467[index]);
		r0 = 0x1Eu;
		request_sound(r0);
		reset_screen_sound();
		reset_tile_layers();
		SCREEN(screen)->race_started = 0u;
		SCREEN(screen)->in_game = 0u;
		SCREEN(screen)->is_host = 0u;
		race_record_clear();
		clear_objects();
		SCREEN(screen)->game_step = 0x1Cu;
		RACE(SCREEN(screen)->race)->game_over = 0xFFu;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		clear_lcd();
		return;
	case 28:
		AS32(SCREEN(screen)->game_mode) = 5u;
		return;
	default:
		invalid_jump(rom_002E80[SCREEN(screen)->game_step]);
	}
}

/* The screen's sound off (reset_screen_sound), then clear_screen. */
void silence_and_clear_screen(void)
{
	reset_screen_sound();
	clear_screen();
}

/* Clears what the screen shows: no objects (clear_objects), its tilemaps off in the mixer
   (reset_tile_layers), no sprites (clear_sprite_group_count, screen unk_B0) and the text layer cleared
   (clear_text_layer). */
void clear_screen(void)
{
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
}

/* Clears the race's state: the race record's car part (race_record_clear), the screen's car block and
   the 100 bytes after it (300 bytes from player_car), the race's order, places and times (screen + 330,
   160 bytes) and the ring of views (screen + 600, 100 bytes). */
void clear_race_state(void)
{
	race_record_clear();
	move_halfwords(player_car, 0x100800u, 0x180u);
	move_halfwords(screen + 0x330u, 0x100800u, 0xB0u);
	move_string(0x100800u, 0x80u, screen + 0x600u, 0x80u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* The map of the route after the race (game step 12, object_handlers_34EC); keeps "Thank you for your
   playing." scrolling on the LCD (F40 = 005014). By state: 0 copies its screen's tilemap register
   addresses (0047E8) into 80-BC, first_branch from screen unk_34A, the background (set_up_background
   with 004868 by screen and first_branch: the routes on tilemap 0 over the world on the second), the
   palette rom_031DDC[2C] (queue_palette); 1 zoom 1:1 about the middle of the screen, sea_colour black,
   30 frames (timer); 2 the world's tilemap off, sea_colour 7EE0; 3 zooms in (zoom + 20 a frame for 9
   frames, 200 being 1:1), then the close-up map (pages rom_0048B8) at its start (x scroll rom_0048D0 by
   first_branch, y 8C), starts route_map_car and the 32 route_map_landmark objects; 4
   checkpoint_legs_left from the car's unk_D0, unk_12 from screen unk_A4, stage 0; 5-14 the car drives
   the route leg by leg (leg_frames frames each): 22 frames up from the start to the first checkpoint
   (the car moves, the map stands), then 6 frames of 8 pixels sideways (first_branch picks the side), 11
   of 5, then for each stage 10 frames of 4 up or down by the branch taken (screen unk_34A + stage - 1)
   and 15 of 7 sideways; on these legs the map scrolls under the car (the object's x and y move the other
   way, for the objects on the map), screen unk_5800 telling the car its way (bits 0 down, 1 up, 2 right,
   3 left, 5 the map at its edge, 7 standing); a stage_result_line starts where each stage begins (5, 7,
   11), a route_map_sign (C.P) and stage + 1 where a checkpoint leg ends; when its count of legs (unk_12
   for the first leg of a stage, checkpoint_legs_left for the others) is 0 at a leg's start, the race
   ended on it: last_leg_progress and leg_frames are worked out from the race_copy record's unk_42, the
   car's unk_4A and the stage lengths at 700 of the screen, and unk_5800 bit 6 ends the drive after it;
   at the end (stage 6 or a cut leg) the car stands (bit 7), FINISH (route_map_sign 1) if the goal was
   reached (race_copy unk_5E), 60 frames; 15 the routes (pages rom_0048C0) at zoom 2x where the close-up
   map was, the steps of the zoom out into x and y, unk_5800 bit 4 (the car, the landmarks and the signs
   end); 16 zooms out over 9 frames to 1:1 and the whole route (x scroll rom_0048D4 by first_branch, y
   70), then the world's tilemap again (pages rom_0048C8); 17 sea_colour black, starts 00463C and an
   004240 for each leg (stage + 1, at most 6), DJ message 38, 90 frames; 18 DJ message 39 once the DJ is
   quiet (screen unk_F30); 19 when the 90 frames are over sets the game's next step (screen unk_80 byte 2
   = 0x10 (step 16)) and starts 0046BB; 20 nothing. */
void route_map_update(void)
{
	bool L_003BFC = false, L_003C2D = false;
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r19 = 0, r21 = 0, route_branches = 0, stage_lengths = 0;
	uint8_t timer = 0, timer_2 = 0, t1 = 0, timer_3 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, timer_4 = 0, timer_5 = 0;
	uint32_t target1 = 0, zoom_x_address = 0, zoom_y_address = 0, scroll_x_address = 0, scroll_x_address_2 = 0, scroll_y_address = 0, scroll_x_address_3 = 0, scroll_y_address_2 = 0, scroll_x_address_4 = 0, scroll_y_address_3 = 0, scroll_x_address_5 = 0, scroll_x_address_6 = 0, scroll_x_address_7 = 0, scroll_x_address_8 = 0, scroll_y_address_4 = 0, scroll_x_address_9 = 0, scroll_y_address_5 = 0, scroll_x_address_10 = 0, scroll_x_address_11 = 0, scroll_x_address_12 = 0, zoom_x_address_2 = 0, zoom_y_address_2 = 0, scroll_x_address_13 = 0, scroll_y_address_6 = 0, second_scroll_x_address = 0, second_scroll_y_address = 0;

	if ((SCREEN(screen)->lcd_scroll | SCREEN(screen)->lcd_scroll_request) == 0u)
		SCREEN(screen)->lcd_scroll_request = 0x5014u;
	do
	{
		do
		{
			do
			{
				do
				{
					do
					{
						do
						{
							do
							{
								do
								{
									target1 = L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003C2D ? 0x383Au : rom_003540[OBJECT(current_object)->state];
									switch (target1)
									{
									case 0x003594u:
										SCREEN(screen)->unk_5800 = 0u;
										r19 = 0x47E8u + ((uint32_t)SCREEN(screen)->index << 4) * 4u;
										OBJECT(current_object)->route_map.pages_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.scroll_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.scroll_y_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.zoom_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.zoom_y_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.zoom_center_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.zoom_center_y_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.register_scroll_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.register_scroll_y_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_A4 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.second_pages_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.second_scroll_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.second_scroll_y_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_x_address = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_y_address = mem_read32(r19);
										AS32(OBJECT(current_object)->unk_BC) = mem_read32(r19 + 4u);
										r0 = SCREEN(screen)->index * 5u * 2u;
										route_branches = SCREEN(screen)->route_branches;
										OBJECT(current_object)->route_map.first_branch = route_branches;
										r19 = 0x4868u + (r0 + route_branches * 5u) * 4u;
										set_up_background(r19);
										r19 = rom_031E8C;
										queue_palette(r19);
										OBJECT(current_object)->state++;
										return;
									case 0x003644u:
										OBJECT(current_object)->route_map.sea_colour_address = rom_0048D8[SCREEN(screen)->index];
										mem_write16(OBJECT(current_object)->route_map.sea_colour_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_x_address, 0xA0u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_y_address, 0x70u);
										mem_write16(OBJECT(current_object)->route_map.scroll_x_address, 0x180u);
										mem_write16(OBJECT(current_object)->route_map.scroll_y_address, 0x70u);
										mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, 0x8C00u);
										mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, 0x70u);
										mem_write16(OBJECT(current_object)->route_map.second_scroll_x_address, 0xF8u);
										mem_write16(OBJECT(current_object)->route_map.second_scroll_y_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.second_register_scroll_x_address, 0xF8u);
										mem_write16(OBJECT(current_object)->route_map.second_register_scroll_y_address, 0u);
										unk_20E027 = 0xFFu;
										OBJECT(current_object)->route_map.timer = 0x1Eu;
										OBJECT(current_object)->state++;
										return;
									case 0x0036C0u:
										timer = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer - 1u;
										if (timer == 1u)
										{
											mem_write32(OBJECT(current_object)->route_map.second_pages_address, 0u);
											mem_write16(OBJECT(current_object)->route_map.sea_colour_address, 0x7EE0u);
											OBJECT(current_object)->route_map.timer = 9u;
											OBJECT(current_object)->state++;
										}
										return;
									case 0x0036DFu:
										zoom_x_address = OBJECT(current_object)->route_map.zoom_x_address;
										mem_write16(zoom_x_address, mem_read16(zoom_x_address) + 0x20u);
										zoom_y_address = OBJECT(current_object)->route_map.zoom_y_address;
										mem_write16(zoom_y_address, mem_read16(zoom_y_address) + 0x20u);
										unk_20E027 = 0xFFu;
										timer_2 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_2 - 1u;
										if (timer_2 != 1u)
											return;
										mem_write32(OBJECT(current_object)->route_map.pages_address, rom_0048B8[SCREEN(screen)->index]);
										mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_x_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_y_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.scroll_x_address, rom_0048D0[OBJECT(current_object)->route_map.first_branch]);
										mem_write16(OBJECT(current_object)->route_map.scroll_y_address, 0x8Cu);
										mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
										mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
										unk_20E027 = 0xFFu;
										SCREEN(screen)->skip_scroll_update = 0xFFu;
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_car_update_ADDRESS);
										mem_write8(r21 + 9u, CAR(player_car)->model);
										mem_write32(r21 + 0x14u, current_object);
										SCREEN(screen)->unk_5800 |= 0x80u;
										OBJECT(current_object)->x = 0xA0u;
										OBJECT(current_object)->y = 0x70u;
										r1 = 0x20u;
										r0_2 = 0u;
										do
										{
											r21 = start_object();
											mem_write32(r21 + 4u, route_map_landmark_update_ADDRESS);
											mem_write8(r21 + 9u, r0_2);
											mem_write32(r21 + 0x14u, current_object);
											r0_2++;
											r1--;
										} while (r1 != 0u);
										OBJECT(current_object)->state++;
										return;
									case 0x0037A7u:
										OBJECT(current_object)->unk_12 = SCREEN(screen)->unk_A4;
										OBJECT(current_object)->route_map.checkpoint_legs_left = CAR(player_car)->forks_taken;
										OBJECT(current_object)->route_map.stage = 0u;
										OBJECT(current_object)->state++;
										return;
									case 0x0037BEu:
										SCREEN(screen)->unk_5800 &= ~0x80u;
										r21 = start_object();
										mem_write32(r21 + 4u, stage_result_line_draw_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->route_map.stage);
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0x16;
											OBJECT(current_object)->state++;
										}
										else
										{
											mem_write8(r21 + 0xAu, 1u);
											r0 = mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7));
											stage_lengths = SCREEN(screen)->stage_lengths;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(r0 << 3, stage_lengths);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(((r0 * 5u) << 2) + r0 * 2u, stage_lengths);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x00383Au:
										if (!L_003BFC)
										{
											if (!L_003BFC)
											{
												if (!L_003BFC)
												{
													if (!L_003BFC)
													{
														if (!L_003BFC)
														{
															if (!L_003BFC)
															{
																if (!L_003BFC)
																{
																	if (!L_003C2D)
																	{
																		t1 = OBJECT(current_object)->route_map.leg_frames - 1u;
																		OBJECT(current_object)->route_map.leg_frames = t1;
																		if ((int8_t)t1 >= 0)
																		{
																			SCREEN(screen)->unk_5800 |= 2u;
																			return;
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
										if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003C2D || (SCREEN(screen)->unk_5800 & 0x40u) != 0u)
										{
											if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || !L_003C2D)
											{
												SCREEN(screen)->unk_5800 |= 0x80u;
												if (RACE(SCREEN(screen)->race_copy)->unk_5E != 0u)
												{
													r21 = start_object();
													mem_write32(r21 + 4u, route_map_sign_update_ADDRESS);
													mem_write32(r21 + 0x14u, current_object);
													mem_write8(r21 + 9u, 1u);
												}
												OBJECT(current_object)->route_map.timer = 0x3Cu;
												OBJECT(current_object)->state = 0xFu;
											}
											timer_3 = OBJECT(current_object)->route_map.timer;
											OBJECT(current_object)->route_map.timer = timer_3 - 1u;
											if (timer_3 == 1u)
											{
												mem_write32(OBJECT(current_object)->route_map.pages_address, rom_0048C0[SCREEN(screen)->index]);
												mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x400u);
												mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x400u);
												scroll_x_address = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 0x180u);
												scroll_x_address_2 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_2, mem_read16(scroll_x_address_2) - ((uint32_t)OBJECT(current_object)->route_map.first_branch << 8));
												scroll_y_address = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address, mem_read16(scroll_y_address) - 0xF7u);
												scroll_x_address_3 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_3, (int16_t)mem_read16(scroll_x_address_3) >> 1);
												scroll_y_address_2 = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address_2, (int16_t)mem_read16(scroll_y_address_2) >> 1);
												mem_write16(OBJECT(current_object)->route_map.zoom_center_x_address, 0xA0u);
												mem_write16(OBJECT(current_object)->route_map.zoom_center_y_address, 0x70u);
												scroll_x_address_4 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_4, mem_read16(scroll_x_address_4) + 0x1D0u);
												scroll_y_address_3 = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address_3, mem_read16(scroll_y_address_3) + 0xA3u);
												mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
												mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
												OBJECT(current_object)->x = rom_0048D4[OBJECT(current_object)->route_map.first_branch];
												OBJECT(current_object)->y = 0x70u;
												OBJECT(current_object)->x -= mem_read16(OBJECT(current_object)->route_map.scroll_x_address);
												OBJECT(current_object)->y -= mem_read16(OBJECT(current_object)->route_map.scroll_y_address);
												OBJECT(current_object)->x = (int16_t)OBJECT(current_object)->x >> 3;
												OBJECT(current_object)->y = (int16_t)OBJECT(current_object)->y >> 3;
												unk_20E027 = 0xFFu;
												SCREEN(screen)->skip_scroll_update = 0xFFu;
												SCREEN(screen)->unk_5800 |= 0x10u;
												OBJECT(current_object)->route_map.timer = 9u;
												OBJECT(current_object)->state++;
											}
											return;
										}
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_sign_update_ADDRESS);
										mem_write32(r21 + 0x14u, current_object);
										mem_write8(r21 + 9u, 0u);
										SCREEN(screen)->unk_5800 &= ~2u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003877u:
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = start_object();
										mem_write32(r21 + 4u, stage_result_line_draw_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->route_map.stage);
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											mem_write8(r21 + 0xAu, 1u);
										if (OBJECT(current_object)->unk_12 != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 6;
											OBJECT(current_object)->state++;
										}
										else
										{
											r2 = screen + 0x700u;
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - mem_read16(r2 + SCREEN(screen)->unk_A6 * 2u);
											r3 = SCREEN(r2 - 0x700u)->unk_73E;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(r0 << 2, r3);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(r0 * 3u * 2u, r3);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003908u:
										t2 = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = t2;
										if ((int8_t)t2 >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												scroll_x_address_5 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_5, mem_read16(scroll_x_address_5) - 8u);
												OBJECT(current_object)->x += 8u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											scroll_x_address_6 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_6, mem_read16(scroll_x_address_6) + 8u);
											OBJECT(current_object)->x -= 8u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->unk_12--;
										OBJECT(current_object)->state++;
									case 0x00394Eu:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xB;
											OBJECT(current_object)->state++;
										}
										else
										{
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											r1 = mem_read16(screen + 0x700u + SCREEN(screen)->unk_A6 * 2u);
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(r0 << 2, r1);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(r0 * 5u * 2u + r0, r1);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x0039B4u:
										t3 = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = t3;
										if ((int8_t)t3 >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												scroll_x_address_7 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_7, mem_read16(scroll_x_address_7) - 5u);
												OBJECT(current_object)->x += 5u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											scroll_x_address_8 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_8, mem_read16(scroll_x_address_8) + 5u);
											OBJECT(current_object)->x -= 5u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_sign_update_ADDRESS);
										mem_write32(r21 + 0x14u, current_object);
										mem_write8(r21 + 9u, 0u);
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003A14u:
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = start_object();
										mem_write32(r21 + 4u, stage_result_line_draw_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->route_map.stage);
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											mem_write8(r21 + 0xAu, 1u);
										if (OBJECT(current_object)->unk_12 != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xA;
											OBJECT(current_object)->state++;
										}
										else
										{
											r2 = screen + 0x700u;
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - mem_read16(r2 + SCREEN(screen)->unk_A6 * 2u);
											r3 = SCREEN(r2 - 0x700u)->unk_73E;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(r0 << 2, r3);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(r0 * 5u * 2u, r3);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003AA5u:
										t4 = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = t4;
										if ((int8_t)t4 >= 0)
										{
											if (mem_read8(screen + 0x34Au + (OBJECT(current_object)->route_map.stage - 1u)) != 0u)
											{
												if (OBJECT(current_object)->route_map.first_branch != 0u)
												{
													SCREEN(screen)->unk_5800 |= 1u;
													scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
													mem_write16(scroll_y_address_4, mem_read16(scroll_y_address_4) + 4u);
													scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
													mem_write16(scroll_x_address_9, mem_read16(scroll_x_address_9) - 1u);
													OBJECT(current_object)->y -= 4u;
													OBJECT(current_object)->x++;
													return;
												}
												SCREEN(screen)->unk_5800 |= 2u;
												scroll_y_address_5 = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address_5, mem_read16(scroll_y_address_5) - 4u);
												scroll_x_address_10 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_10, mem_read16(scroll_x_address_10) + 1u);
												OBJECT(current_object)->y += 4u;
												OBJECT(current_object)->x--;
												return;
											}
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->unk_5800 |= 2u;
												scroll_y_address_5 = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address_5, mem_read16(scroll_y_address_5) - 4u);
												scroll_x_address_10 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_10, mem_read16(scroll_x_address_10) + 1u);
												OBJECT(current_object)->y += 4u;
												OBJECT(current_object)->x--;
												return;
											}
											SCREEN(screen)->unk_5800 |= 1u;
											scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
											mem_write16(scroll_y_address_4, mem_read16(scroll_y_address_4) + 4u);
											scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_9, mem_read16(scroll_x_address_9) - 1u);
											OBJECT(current_object)->y -= 4u;
											OBJECT(current_object)->x++;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFFCu;
										OBJECT(current_object)->unk_12--;
										OBJECT(current_object)->state++;
									case 0x003B16u:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xF;
											OBJECT(current_object)->state++;
										}
										else
										{
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											r1 = mem_read16(screen + 0x700u + SCREEN(screen)->unk_A6 * 2u);
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(r0 << 3, r1);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(r0 * 3u * 5u, r1);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003B77u:
										t5 = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = t5;
										if ((int8_t)t5 >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												if ((int16_t)mem_read16(OBJECT(current_object)->route_map.scroll_x_address) <= 0x10)
												{
													SCREEN(screen)->unk_5800 |= 0x20u;
													return;
												}
												scroll_x_address_11 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_11, mem_read16(scroll_x_address_11) - 7u);
												OBJECT(current_object)->x += 7u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											if ((int16_t)mem_read16(OBJECT(current_object)->route_map.scroll_x_address) >= 0x2A0)
											{
												SCREEN(screen)->unk_5800 |= 0x20u;
												return;
											}
											scroll_x_address_12 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_12, mem_read16(scroll_x_address_12) + 7u);
											OBJECT(current_object)->x -= 7u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										if (OBJECT(current_object)->route_map.stage != 6u)
										{
											r21 = start_object();
											mem_write32(r21 + 4u, route_map_sign_update_ADDRESS);
											mem_write32(r21 + 0x14u, current_object);
											mem_write8(r21 + 9u, 0u);
											OBJECT(current_object)->state = 0xBu;
											return;
										}
										L_003BFC = true;
										break;
									case 0x003C2Du:
										L_003C2D = true;
										break;
									case 0x003CF1u:
										zoom_x_address_2 = OBJECT(current_object)->route_map.zoom_x_address;
										mem_write16(zoom_x_address_2, mem_read16(zoom_x_address_2) - 0x40u);
										zoom_y_address_2 = OBJECT(current_object)->route_map.zoom_y_address;
										mem_write16(zoom_y_address_2, mem_read16(zoom_y_address_2) - 0x40u);
										scroll_x_address_13 = OBJECT(current_object)->route_map.scroll_x_address;
										mem_write16(scroll_x_address_13, mem_read16(scroll_x_address_13) + OBJECT(current_object)->x);
										scroll_y_address_6 = OBJECT(current_object)->route_map.scroll_y_address;
										mem_write16(scroll_y_address_6, mem_read16(scroll_y_address_6) + OBJECT(current_object)->y);
										unk_20E027 = 0xFFu;
										timer_4 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_4 - 1u;
										if (timer_4 == 1u)
										{
											mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x200u);
											mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x200u);
											mem_write16(OBJECT(current_object)->route_map.scroll_x_address, rom_0048D4[OBJECT(current_object)->route_map.first_branch]);
											mem_write16(OBJECT(current_object)->route_map.scroll_y_address, 0x70u);
											mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
											mem_write32(OBJECT(current_object)->route_map.second_pages_address, rom_0048C8[SCREEN(screen)->index]);
											mem_write16(OBJECT(current_object)->route_map.second_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.second_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
											second_scroll_x_address = OBJECT(current_object)->route_map.second_scroll_x_address;
											mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) - 0x88u);
											second_scroll_y_address = OBJECT(current_object)->route_map.second_scroll_y_address;
											mem_write16(second_scroll_y_address, mem_read16(second_scroll_y_address) - 0x70u);
											mem_write16(OBJECT(current_object)->route_map.second_register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.second_scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.second_register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.second_scroll_y_address));
											unk_20E027 = 0xFFu;
											OBJECT(current_object)->state++;
										}
										return;
									case 0x003D94u:
										mem_write16(OBJECT(current_object)->route_map.sea_colour_address, 0u);
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_direction_sign_show_ADDRESS);
										r1 = OBJECT(current_object)->route_map.stage;
										if (r1 != 6u)
											r1++;
										r0_2 = 0u;
										do
										{
											r21 = start_object();
											mem_write32(r21 + 4u, route_map_leg_show_ADDRESS);
											mem_write8(r21 + 9u, r0_2);
											mem_write8(r21 + 0xAu, OBJECT(current_object)->route_map.last_leg_progress);
											mem_write32(r21 + 0x14u, current_object);
											r0_2++;
											r1--;
										} while (r1 != 0u);
										r0_2 = 0x38u;
										start_dj_message(r0_2);
										OBJECT(current_object)->route_map.timer = 0x5Au;
										OBJECT(current_object)->state++;
										return;
									case 0x003DE9u:
										if (SCREEN(screen)->dj_mouth_shapes == 0u)
										{
											r0_2 = 0x39u;
											start_dj_message(r0_2);
											OBJECT(current_object)->state++;
										}
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_wait_banner_show_ADDRESS);
										OBJECT(current_object)->state++;
										return;
									case 0x003DFCu:
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										r21 = start_object();
										mem_write32(r21 + 4u, route_map_wait_banner_show_ADDRESS);
										OBJECT(current_object)->state++;
										return;
									case 0x003E1Au:
										return;
									default:
										invalid_jump(target1);
									}
								} while (L_003BFC);
							} while (L_003BFC);
						} while (L_003BFC);
					} while (L_003BFC);
				} while (L_003BFC);
			} while (L_003BFC);
		} while (L_003BFC);
	} while (L_003C2D);
}

/* The player's car on the route map (animation rom_0048E0 by the car's model, palette rom_0048F0, size
   50): 0 at x 8C, y C3 with frame C; 1 until the route map's stage is not 0, drives up 4 pixels and
   right 1 a frame unless the map says it stands (screen unk_5800 bit 7); 2 stays where it is while the
   map scrolls under it, its frame by the way the map says it goes (bit 2 frame 9, bit 3 frame 4, bit 0
   frame 6, bit 1 frame C), moving itself 7 pixels a frame sideways when the map is at its edge (bit 5);
   ends when the map zooms out (bit 4). Its depth layer follows its y (map_depth_from_y). */
void route_map_car_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_003E28[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r0 = OBJECT(current_object)->route_map_car.model;
		r9 = rom_0048E0[r0];
		animation_start(r9);
		OBJECT(current_object)->palette = rom_0048F0[r0];
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->x = 0x8Cu;
		OBJECT(current_object)->y = 0xC3u;
		OBJECT(current_object)->frame = 0xCu;
		map_depth_from_y();
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if (OBJECT(OBJECT(current_object)->route_map_car.route_map)->unk_28 != 0u)
		{
			OBJECT(current_object)->state++;
			r0 = draw_object(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 0x80u) != 0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->y -= 4u;
		OBJECT(current_object)->x++;
		map_depth_from_y();
		r0 = draw_object(r0);
		return;
	case 2:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 0x80u) != 0u)
		{
			map_depth_from_y();
			r0 = draw_object(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 4u) != 0u)
		{
			OBJECT(current_object)->frame = 9u;
			if ((SCREEN(screen)->unk_5800 & 0x20u) != 0u)
				OBJECT(current_object)->x += 7u;
			map_depth_from_y();
			r0 = draw_object(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 8u) != 0u)
		{
			OBJECT(current_object)->frame = 4u;
			if ((SCREEN(screen)->unk_5800 & 0x20u) != 0u)
			{
				OBJECT(current_object)->x -= 7u;
				map_depth_from_y();
				r0 = draw_object(r0);
				return;
			}
			map_depth_from_y();
			r0 = draw_object(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 1u) == 0u)
		{
			if ((SCREEN(screen)->unk_5800 & 2u) == 0u)
			{
				map_depth_from_y();
				r0 = draw_object(r0);
				return;
			}
			OBJECT(current_object)->frame = 0xCu;
			map_depth_from_y();
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->frame = 6u;
		map_depth_from_y();
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_003E28[state]);
	}
}

/* A sign on the route map, by kind: 0 C.P where the car passed a checkpoint (animation 2F9), 1 FINISH at
   the goal (2FA); palette 2CF, depth layer 195. State 0: x_offset and y_offset are the middle of the
   screen (A0, 70 less 8) less the map's place, where the car is; kind 1 goes to state 2; 1 follows the
   map (its x and y plus the offsets) and turns (animation_advance); 2 FINISH at the goal's place on the
   map (the map's x plus rom_004042 by the goal, screen unk_A6 - 15; y 5C), drawn 4 frames of 8 (blink);
   both end when the map zooms out (screen unk_5800 bit 4). */
void route_map_sign_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_003F34[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r0 = OBJECT(current_object)->route_map_sign.kind;
		r9 = rom_00403A[r0];
		animation_start(r9);
		OBJECT(current_object)->palette = rom_00403E[r0];
		OBJECT(current_object)->width_scale = 0x100u;
		OBJECT(current_object)->height_scale = 0x100u;
		OBJECT(current_object)->route_map_sign.x_offset = 0xA0;
		OBJECT(current_object)->route_map_sign.y_offset = 0x70;
		OBJECT(current_object)->route_map_sign.x_offset -= OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->route_map_sign.y_offset -= OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->route_map_sign.y_offset -= 8u;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		OBJECT(current_object)->depth_layer = 0x195u;
		if (OBJECT(current_object)->route_map_sign.kind == 1u)
		{
			OBJECT(current_object)->state = 2u;
			return;
		}
		OBJECT(current_object)->state++;
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 2:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = SCREEN(screen)->unk_A6 - 0x15u;
		OBJECT(current_object)->route_map_sign.x_offset = rom_004042[r0];
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y = 0x5Cu;
		OBJECT(current_object)->route_map_sign.blink--;
		if ((OBJECT(current_object)->route_map_sign.blink & 4u) == 0u)
			return;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_003F34[state]);
	}
}

/* One of the 32 animated pictures on the close-up route map (people, animals and vehicles by the
   landmarks): 0 from its record (004C14 + 16 * the index in 9, the second set of 32 (+ 200) when screen
   unk_34A is 1): animation, palette, size, x_offset and y_offset from the map's place, then field 9 from
   the record (set for one record: it then shows only every other frame); 1 follows the map (its x and y
   plus the offsets), animation_advance and draw_object, with field 9 set only when toggle comes round;
   ends when the map zooms out (screen unk_5800 bit 4). Its depth layer follows its y (map_depth_from_y). */
void route_map_landmark_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_004064[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r0 = ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4) + ((uint32_t)SCREEN(screen)->route_branches << 9);
		r10 = 0x4C14u + r0;
		r9 = mem_read16(r10);
		r10 += 2u;
		animation_start(r9);
		OBJECT(current_object)->palette = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->width_scale = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->height_scale = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->route_map_landmark.x_offset = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->route_map_landmark.y_offset = mem_read16(r10);
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		AT8(OBJECT(current_object)->unk_08, 1) = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->state++;
		map_depth_from_y();
		r0 = draw_object(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
		{
			OBJECT(current_object)->route_map_landmark.toggle++;
			t1 = OBJECT(current_object)->route_map_landmark.toggle & 1u;
			OBJECT(current_object)->route_map_landmark.toggle = t1;
			if (t1 != 0u)
				return;
		}
		map_depth_from_y();
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_004064[state]);
	}
}

/* Sets the object's depth layer (3C) from its y: (y + D0) >> 3, at most 19F, so that what stands lower
   on the route map is drawn in front. Used by route_map_car and route_map_landmark. */
void map_depth_from_y(void)
{
	OBJECT(current_object)->depth_layer = OBJECT(current_object)->y;
	OBJECT(current_object)->depth_layer += 0xD0u;
	OBJECT(current_object)->depth_layer = OBJECT(current_object)->depth_layer >> 3;
	if (OBJECT(current_object)->depth_layer == 0u)
	{
		OBJECT(current_object)->depth_layer = 0u;
		return;
	}
	if ((int16_t)OBJECT(current_object)->depth_layer >= 0x1A0)
	{
		OBJECT(current_object)->depth_layer = 0x19Fu;
		return;
	}
}

/* A line of the player's places by stage on the route map, with two players or more (ends at once with
   one): every frame, the stage's label (004BA4 by stage: PRESTG, STAGE1-4, RESULT) at x A8, y 40 + 16 *
   stage, and at x E8 the player's place at that stage (004BDA by screen 360 + player + 8 * stage:
   1st-8th); the last line says Retire instead, unless it is RESULT (the goal reached), where the
   player's final place (place_address) is drawn big (animation 4, draw mode 8, at x F0, y B0, size 160;
   palette 41 for 1st, 42 for the others). */
void stage_result_line_draw(void)
{
	uint16_t r5 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r18 = 0, player = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;

	player = SCREEN(screen)->player;
	if (RACE(SCREEN(screen)->host_record)->racer_count <= 1u)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	OBJECT(current_object)->stage_result_line.place_address = screen + 0x320u + player;
	r18 = 0x4BA4u + OBJECT(current_object)->stage_result_line.stage * 3u * 3u;
	r5 = mem_read16(r18);
	r3 = (0x40u + (OBJECT(current_object)->stage_result_line.stage << 4)) << 16;
	r3 |= (uint16_t)(r3 + 0xA8u);
	r18 += 2u;
	stack_4 = r3;
	draw_text_at(r3, r5, r18);
	r3 = (stack_4 & ~0xFFFFu) | (uint16_t)(stack_4 + 0x40u);
	player = SCREEN(screen)->player;
	r0 = mem_read8(screen + 0x360u + player + (OBJECT(current_object)->stage_result_line.stage << 3)) * 3u;
	if (OBJECT(current_object)->stage_result_line.last == 0u)
	{
		r18 = 0x4BDAu + r0 * 2u;
		r5 = ROM16(r18);
		r18 += 2u;
		draw_text_at(r3, r5, r18);
		return;
	}
	if (OBJECT(current_object)->stage_result_line.stage != 5u)
	{
		r5 = rom_004C0A;
		r18 = 0x4C0Cu;
		draw_text_at(r3, r5, r18);
		return;
	}
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF0u;
	OBJECT(current_object)->y = 0xB0u;
	OBJECT(current_object)->width_scale = 0x160u;
	OBJECT(current_object)->height_scale = 0x160u;
	r9 = 4u;
	animation_start(r9);
	t1 = ~~mem_read8(OBJECT(current_object)->stage_result_line.place_address);
	OBJECT(current_object)->frame = t1;
	r0 = 0x41u;
	if (t1 != 0u)
		r0 = 0x42u;
	OBJECT(current_object)->palette = r0;
	r0 = draw_object(r0);
}

/* A leg of the route the player drove, drawn in red over the map shown after the race (003520), by
   state: 0 sets it up by its number (leg): 0 from the start to the first fork, 1 on from there by the
   direction taken (screen unk_34A: 0 east, 1 west, the map's 09), 2-5 by the branch taken at each fork
   (screen unk_34A + leg - 1) from the place of its stage (rom_004900, rom_00492A by screen unk_343 + leg
   - 1), the map shifted by the direction (rom_004954) and 6B down; a leg driven already shows whole
   (frame 7, or B for 2-5), the leg the car is on (the map's unk_28) up to the frame before its progress,
   with the car on it (route_map_leg_car); legs 1-5 start the CP sign at their start
   (route_map_checkpoint), and leg 5, when the race reached the goal (race_copy unk_5E), also the FINISH
   sign and the car at its end; 1 draws it. */
void route_map_leg_show(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r21 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00424C[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00424C[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->palette = 0x2D1u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->depth_layer = 0x18Eu;
	switch (OBJECT(current_object)->route_map_leg.leg)
	{
	case 0:
		r9 = 0x306u;
		animation_start(r9);
		OBJECT(current_object)->x = 0xFFF7u;
		OBJECT(current_object)->y = 0x2Bu;
		OBJECT(current_object)->depth_layer++;
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_28)
			OBJECT(current_object)->frame = 7u;
		else
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			r21 = start_object();
			mem_write32(r21 + 4u, route_map_leg_car_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
		}
		break;
	case 1:
		r9 = rom_004958[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
		animation_start(r9);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0u;
		r21 = start_object();
		mem_write32(r21 + 4u, route_map_checkpoint_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_28)
		{
			OBJECT(current_object)->frame = 7u;
			OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
		r21 = start_object();
		mem_write32(r21 + 4u, route_map_leg_car_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->frame--;
		if ((int8_t)OBJECT(current_object)->frame < 0)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		r1 = OBJECT(current_object)->route_map_leg.leg - 1u;
		r9 = rom_00495C[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u) * 2u + mem_read8(screen + 0x34Au + r1)];
		animation_start(r9);
		r0 = mem_read8(screen + 0x343u + r1);
		OBJECT(current_object)->x = rom_004900[r0];
		OBJECT(current_object)->y = rom_00492A[r0];
		r21 = start_object();
		mem_write32(r21 + 4u, route_map_checkpoint_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->depth_layer += OBJECT(current_object)->route_map_leg.leg;
		if (OBJECT(current_object)->route_map_leg.leg != 5u)
		{
			if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_28)
			{
				OBJECT(current_object)->frame = 0xBu;
				OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			r21 = start_object();
			mem_write32(r21 + 4u, route_map_leg_car_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			r21 = start_object();
			mem_write32(r21 + 4u, route_map_leg_car_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
			{
				OBJECT(current_object)->frame = 0u;
				OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
		}
		else
		{
			r21 = start_object();
			mem_write32(r21 + 4u, route_map_checkpoint_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			mem_write8(r21 + 0xAu, 1u);
			OBJECT(current_object)->route_map_leg.progress = 0xBu;
			r21 = start_object();
			mem_write32(r21 + 4u, route_map_leg_car_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->frame = 0xBu;
		}
		break;
	default:
		invalid_jump(rom_004280[OBJECT(current_object)->route_map_leg.leg]);
	}
	OBJECT(current_object)->x += rom_004954[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg.map)->unk_08 >> 8u)];
	OBJECT(current_object)->y += 0x6Bu;
	OBJECT(current_object)->state++;
}

/* The player's car on the map after the race, where the red line of its leg ends, by state: 0 sets it
   up: the sprite of its model (rom_0048E0, rom_0048F0 by car unk_60) at a quarter size, its place and
   frame (which way it faces) from a record by the leg's progress (004964 for leg 0, 0049A4 for leg 1 by
   the direction, 004A24 for legs 2-5 by the direction and the branch, from the stage's place), shifted
   like the legs; 1 draws it. */
void route_map_leg_car_show(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, model = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00442C[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00442C[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	model = CAR(player_car)->model;
	r9 = rom_0048E0[model];
	animation_start(r9);
	OBJECT(current_object)->palette = rom_0048F0[model];
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	OBJECT(current_object)->depth_layer = 0x194u;
	switch ((uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 8u))
	{
	case 0:
		r10 = 0x4964u + ((uint32_t)(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 16u) << 3);
		OBJECT(current_object)->x = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->y = ROM16(r10);
		OBJECT(current_object)->x += rom_004954[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROM16(r10 + 2u);
		OBJECT(current_object)->state++;
		return;
	case 1:
		r10 = 0x49A4u + (((uint32_t)SCREEN(screen)->route_branches << 6) + ((uint32_t)(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 16u) << 3));
		OBJECT(current_object)->x = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->y = mem_read16(r10);
		OBJECT(current_object)->x += rom_004954[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->state++;
		return;
	case 2:
	case 3:
	case 4:
	case 5:
		r1 = (uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 8u) - 1u;
		r10 = 0x4A24u + (((SCREEN(screen)->route_branches * 3u) << 6) + ((mem_read8(screen + 0x34Au + r1) * 3u) << 5) + ((uint32_t)(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 16u) << 3));
		r0 = mem_read8(screen + 0x343u + r1);
		OBJECT(current_object)->x = rom_004900[r0];
		OBJECT(current_object)->y = rom_00492A[r0];
		OBJECT(current_object)->x += mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->y += mem_read16(r10);
		OBJECT(current_object)->x += rom_004954[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->state++;
		return;
	default:
		invalid_jump(rom_004474[(uint8_t)(OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->unk_08 >> 8u)]);
	}
}

/* Unused: ends the object (flags 0). It follows the code of route_map_leg_car_show's last state
   (draw_object), whose state table lists only states 0 and 1. */
void route_map_leg_car_end(void)
{
	OBJECT(current_object)->flags = 0u;
}

/* A sign on the map after the race, by state: 0 sets it up by its kind (rom_00403A, rom_00403E): 0 the
   CP sign at the start of its leg (the fork it leaves from), 1 the FINISH sign beside the goal end of
   the last leg (rom_00462C, rom_004634 by the direction and the branch); 1 the CP sign turns
   (animation_advance) and is drawn; 2 FINISH blinks, drawn 4 frames out of 8 (blink). */
void route_map_checkpoint_show(void)
{
	uint32_t r0 = 0, r9 = 0, kind = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_004588[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		kind = OBJECT(current_object)->route_map_checkpoint.kind;
		r9 = rom_00403A[kind];
		animation_start(r9);
		OBJECT(current_object)->palette = rom_00403E[kind];
		OBJECT(current_object)->width_scale = 0x100u;
		OBJECT(current_object)->height_scale = 0x100u;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->y;
		OBJECT(current_object)->y -= 6u;
		OBJECT(current_object)->depth_layer = 0x194u;
		if (OBJECT(current_object)->route_map_checkpoint.kind == 1u)
		{
			r0 = SCREEN(screen)->route_branches * 2u + mem_read8(screen + 0x34Au + ((uint8_t)(OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->unk_08 >> 8u) - 1u));
			OBJECT(current_object)->x += rom_00462C[r0];
			OBJECT(current_object)->y += rom_004634[r0];
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->state++;
		return;
	case 1:
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->route_map_checkpoint.blink--;
		if ((OBJECT(current_object)->route_map_checkpoint.blink & 4u) == 0u)
			return;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_004588[state]);
	}
}

/* The WEST BOUND / EAST BOUND sign at the foot of the map after the race: a globe between the two,
   placed by the direction (screen unk_34A) so that only the half of the way taken shows, by state: 0
   sets it up below the screen; 1 rises 2 a frame to DC; 1 and 2 cycle its colours through four palettes
   from palette_base, one every 4 frames (colour_timer), and draw it. */
void route_map_direction_sign_show(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = rom_004954[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y = 0x118u;
		r9 = 0x39u;
		OBJECT(current_object)->route_map_direction_sign.palette_base = 0x67u;
		animation_start(r9);
		return;
	case 1:
		OBJECT(current_object)->y -= 2u;
		if (OBJECT(current_object)->y == 0xDCu)
			OBJECT(current_object)->state++;
		r0 = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = r0;
		r0 = draw_object(r0);
		return;
	case 2:
		r0 = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = r0;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_004648[OBJECT(current_object)->state]);
	}
}

/* JUST A MOMENT PLEASE over the map after the race, while the game waits to go on, by state: 0 waits 32
   frames (timer), then sets it up at the centre of the screen, tiny; 1 grows to full size in 15 frames;
   2 drawn 15 frames; 3 hidden 5 frames, back to 2. */
void route_map_wait_banner_show(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t1 = 0, timer = 0, timer_2 = 0, timer_3 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0046C8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->route_map_wait_banner.timer++;
		t1 = OBJECT(current_object)->route_map_wait_banner.timer & 0x1Fu;
		OBJECT(current_object)->route_map_wait_banner.timer = t1;
		if (t1 == 0u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			r9 = 0x28Du;
			animation_start(r9);
			OBJECT(current_object)->palette = 0u;
			OBJECT(current_object)->width_scale = 4u;
			OBJECT(current_object)->height_scale = 4u;
			OBJECT(current_object)->x = 0xA0u;
			OBJECT(current_object)->y = 0x70u;
			OBJECT(current_object)->depth_layer = 0x19Fu;
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state++;
		}
		return;
	case 1:
		timer = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state++;
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->width_scale += 0x12u;
		OBJECT(current_object)->height_scale += 0x12u;
		r0 = draw_object(r0);
		return;
	case 2:
		timer_2 = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 5u;
			OBJECT(current_object)->state++;
			r0 = draw_object(r0);
			return;
		}
		r0 = draw_object(r0);
		return;
	case 3:
		timer_3 = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer_3 - 1u;
		if (timer_3 == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state--;
			return;
		}
		return;
	default:
		invalid_jump(rom_0046C8[state]);
	}
}

/* GAME OVER at the centre of the screen when a one-player race ends short of the goal, by state: 0 sets
   it up, tiny; 0 and 1 grow it to full size in 15 frames (timer); 2 draws it. */
void game_over_banner_show(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00477C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x2F4u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x2Au;
		OBJECT(current_object)->width_scale = 4u;
		OBJECT(current_object)->height_scale = 4u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x70u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->game_over_banner.timer = 0xFu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00477C[state]);
	}
	timer = OBJECT(current_object)->game_over_banner.timer;
	OBJECT(current_object)->game_over_banner.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->width_scale += 0x12u;
	OBJECT(current_object)->height_scale += 0x12u;
	r0 = draw_object(r0);
}

/* Starts the car select with 630 frames (21 seconds) to choose, then car_select_run. */
void car_select_start(void)
{
	OBJECT(current_object)->car_select.time_left = 0x276u;
	OBJECT(current_object)->handler = car_select_run_ADDRESS;
	car_select_run();
}

/* The car select, by state: 0 sets the screen up (unless screen unk_E2 says the entry countdown did:
   clears the LCD, the title Mega Driver, sound F, DJ message 3B), the tile layer's tables from 006920,
   the WELCOME! letters; 1 when the screen's data is unpacked (unk_B00) starts the eight cars on the
   turntable and the rest, the default car (rom_0069B0 by the player's place in the race), the time
   display (time_digits_start); 2 30 frames; 3 the screen's unk_F04 up to 0, then the instructions
   (select_car_heading_show) and the car's caption (car_caption_show); 4 the wheel turns the turntable
   (turn, turn_delay, angle; car unk_60 is the car in front) until the gas or START confirms or time_left
   runs out: race unk_49 = the car, unk_48 = the car + 1, the car's sound (rom_006DEE), at least 180
   frames left, the transmission choice (transmission_choice_run); 5 the wheel picks the transmission
   (car unk_61 = rom_006DA4 by the wheel: 0 automatic, 1 manual) until confirmed or out of time; 6-8 when
   the entry time is over (entry_over) marks the player ready (set_player_ready) and waits for the race's
   other players (players_not_ready), DJ message 3C; 9 60 frames, then fade_out_music and the game's next
   step (screen unk_80 byte 2); 10 stays. Every frame from 4 on: car_select_turn_to_choice and
   car_select_update_rowscroll; time_left down by one and shown in seconds. */
void car_select_run(void)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0_3 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r19 = 0, r21 = 0, model = 0, race = 0, rowscroll_address = 0, wheel = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0, t2 = 0, t4 = 0, turn_delay = 0;
	uint16_t timer = 0, timer_2 = 0;
	uint32_t t3 = 0;

	if (unk_20FC1A == 0u)
		OBJECT(current_object)->car_select.entry_over = 0xFFu;
	OBJECT(current_object)->car_select.time_left--;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (SCREEN(screen)->entry_countdown_shown == 0u)
		{
			clear_lcd();
			SCREEN(screen)->dj_face = 0xFFu;
			r10 = 0x6EB6u;
			start_song_title(r10);
			r0 = 0xFu;
			request_sound(r0);
			r0 = 0x3Bu;
			start_dj_message(r0);
		}
		r10 = 0x6920u + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
		OBJECT(current_object)->car_select.scroll_x_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->car_select.scroll_y_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->car_select.second_scroll_x_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->car_select.second_scroll_y_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->car_select.rowscroll_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->car_select.rowselect_address = ROM32(r10);
		r10 += 4u;
		r19 = ROM32(r10);
		stack_4 = r10 + 4u;
		set_up_background(r19);
		unk_20E204 |= mem_read16(stack_4);
		mem_write16(OBJECT(current_object)->car_select.second_scroll_y_address, 0xE0u);
		r21 = start_object();
		mem_write32(r21 + 4u, welcome_letter_start_ADDRESS);
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->car_select.rowselect_address;
		r11 = r10 + 0x800u;
		r12 = r10 + 0x1000u;
		r13 = r10 + 0x1800u;
		r0_2 = 0x98u;
		r9 = 0xE0u;
		do
		{
			mem_write16(r10, r0_2);
			mem_write16(r11, r0_2);
			mem_write16(r12, r0_2);
			r12 = r12 + 2u;
			mem_write16(r13, r0_2);
			r13 += 2u;
			r0_2++;
			r9--;
			r10 += 2u;
			r11 += 2u;
		} while (r9 != 0u);
		rowscroll_address = OBJECT(current_object)->car_select.rowscroll_address;
		move_halfwords(rowscroll_address, 0x100800u, 0xE0u);
		move_halfwords(rowscroll_address + 0x800u, 0x100800u, 0xE0u);
		move_halfwords(rowscroll_address + 0x1000u, 0x100800u, 0xE0u);
		move_halfwords(rowscroll_address + 0x1800u, 0x100800u, 0xE0u);
		move_string(0x100800u, 0xE0u, screen + 0x3800u, 0xE0u, 2u, 0u, 0u, 0u, &r27, &r28);
		return;
	case 1:
		if (SCREEN(screen)->unpack_state != 0u)
			return;
		r21 = start_object();
		mem_write32(r21 + 4u, car_attendant_run_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = start_object();
		mem_write32(r21 + 4u, car_select_entries_run_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r0 = 8u;
		do
		{
			r21 = start_object();
			mem_write32(r21 + 4u, turntable_car_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			t1 = r0;
			r0 = (uint8_t)(t1 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t1 != 1u);
		r0 = 0xCu;
		do
		{
			r21 = start_object();
			mem_write32(r21 + 4u, turntable_scenery_show_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			t2 = r0;
			r0 = (uint8_t)(t2 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t2 != 1u);
		r0_2 = rom_0069B0[mem_read8(screen + 0x320u + SCREEN(screen)->player)];
		CAR(player_car)->model = (uint8_t)r0_2;
		CAR(player_car)->transmission = 0u;
		OBJECT(current_object)->car_select.car_chosen = 0u;
		OBJECT(current_object)->car_select.transmission_chosen = 0u;
		r0_2 = (uint16_t)((uint32_t)r0_2 << 7);
		mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - r0_2);
		mem_write16(OBJECT(current_object)->car_select.scroll_y_address, 0u);
		OBJECT(current_object)->car_select.angle = (uint32_t)r0_2 << 2;
		OBJECT(current_object)->car_select.top_scroll = 0u;
		OBJECT(current_object)->car_select.turn = 0;
		OBJECT(current_object)->car_select.turn_delay = 1;
		OBJECT(current_object)->car_select.rowscroll_copies = 0u;
		OBJECT(current_object)->car_select.gas_down = 0u;
		OBJECT(current_object)->car_select.confirm_delay = 0x1Eu;
		time_digits_start();
		SCREEN(screen)->unk_F04 = 0xFFFFFD00u;
		SCREEN(screen)->unk_F08 = 0u;
		OBJECT(current_object)->car_select.timer = 0x1Eu;
		OBJECT(current_object)->state++;
		r19 = rom_031E7C;
		queue_palette(r19);
		zero = car_select_wait_confirm();
		timer = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer - 1u;
		if (timer == 1u)
			OBJECT(current_object)->state++;
		r0_3 = OBJECT(current_object)->car_select.time_left;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = binary_to_bcd(r0_3);
		time_digits_draw(r0_3);
		return;
	case 2:
		zero = car_select_wait_confirm();
		timer = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer - 1u;
		if (timer == 1u)
			OBJECT(current_object)->state++;
		r0_3 = OBJECT(current_object)->car_select.time_left;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = binary_to_bcd(r0_3);
		time_digits_draw(r0_3);
		return;
	case 3:
		zero = car_select_wait_confirm();
		t3 = SCREEN(screen)->unk_F04 + 0x40u;
		SCREEN(screen)->unk_F04 = t3;
		if (t3 != 0u)
		{
			r0_3 = OBJECT(current_object)->car_select.time_left;
			r0_3 = r0_3 / 0x1Eu;
			r0_3 = binary_to_bcd(r0_3);
			time_digits_draw(r0_3);
			return;
		}
		OBJECT(current_object)->state++;
		r21 = start_object();
		mem_write32(r21 + 4u, select_car_heading_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = start_object();
		mem_write32(r21 + 4u, car_caption_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r0_3 = OBJECT(current_object)->car_select.time_left;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = binary_to_bcd(r0_3);
		time_digits_draw(r0_3);
		return;
	case 4:
		if (OBJECT(current_object)->car_select.time_left != 0u)
		{
			r0 = (uint8_t)OBJECT(current_object)->car_select.turn;
			if (r0 == 0u)
			{
				r0_3 = SCREEN(screen)->wheel;
				r0_3 = r0_3 >> 5;
				t4 = rom_006D9C[r0_3];
				turn_delay = OBJECT(current_object)->car_select.turn_delay;
				OBJECT(current_object)->car_select.turn_delay = turn_delay - t4;
				if ((int8_t)turn_delay <= (int8_t)t4)
				{
					OBJECT(current_object)->car_select.turn_delay = 0xF;
					r1 = 0x10u;
					if (r0_3 >> 2 != 0u)
						r1 = (uint8_t)(0u - r1);
					OBJECT(current_object)->car_select.turn = r1;
				}
			}
			else
			{
				if ((int8_t)r0 < 0)
				{
					if (SCREEN(screen)->wheel >= 0x60u)
					{
						OBJECT(current_object)->car_select.turn = r0 + 1u;
						car_select_skew_floor_previous();
						r0_2 = (uint16_t)(OBJECT(current_object)->car_select.angle - 0x20u);
					}
					else
					{
						r0 += 0x10u;
						OBJECT(current_object)->car_select.turn = r0 - 1u;
						car_select_skew_floor_next();
						r0_2 = (uint16_t)(OBJECT(current_object)->car_select.angle + 0x20u);
					}
				}
				else if (SCREEN(screen)->wheel < 0xA0u)
				{
					OBJECT(current_object)->car_select.turn = r0 - 1u;
					car_select_skew_floor_next();
					r0_2 = (uint16_t)(OBJECT(current_object)->car_select.angle + 0x20u);
				}
				else
				{
					r0 -= 0x10u;
					OBJECT(current_object)->car_select.turn = r0 + 1u;
					car_select_skew_floor_previous();
					r0_2 = (uint16_t)(OBJECT(current_object)->car_select.angle - 0x20u);
				}
				OBJECT(current_object)->car_select.angle = r0_2;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (r0_2 >> 2));
				if ((uint8_t)OBJECT(current_object)->car_select.turn == 0u)
				{
					r0_3 = SCREEN(screen)->wheel;
					r0_3 = r0_3 >> 5;
					t4 = rom_006D9C[r0_3];
					turn_delay = OBJECT(current_object)->car_select.turn_delay;
					OBJECT(current_object)->car_select.turn_delay = turn_delay - t4;
					if ((int8_t)turn_delay <= (int8_t)t4)
					{
						OBJECT(current_object)->car_select.turn_delay = 0xF;
						r1 = 0x10u;
						if (r0_3 >> 2 != 0u)
							r1 = (uint8_t)(0u - r1);
						OBJECT(current_object)->car_select.turn = r1;
					}
				}
			}
			CAR(player_car)->model = ((OBJECT(current_object)->car_select.angle + 0x100u) & 0xFFFu) >> 9;
			zero = car_select_wait_confirm();
			if (zero)
			{
				car_select_turn_to_choice();
				car_select_update_rowscroll();
				r0_3 = OBJECT(current_object)->car_select.time_left;
				r0_3 = r0_3 / 0x1Eu;
				r0_3 = binary_to_bcd(r0_3);
				time_digits_draw(r0_3);
				return;
			}
		}
		OBJECT(current_object)->car_select.car_chosen = 0xFFu;
		race = SCREEN(screen)->race;
		model = CAR(player_car)->model;
		RACE(race)->unk_49 = model;
		RACE(race)->unk_48 = model + 1u;
		OBJECT(current_object)->car_select.confirm_delay = 0x1Eu;
		r0 = rom_006DEE[model];
		request_sound(r0);
		if ((int16_t)OBJECT(current_object)->car_select.time_left < 0xB4)
			OBJECT(current_object)->car_select.time_left = 0xB4u;
		r21 = start_object();
		mem_write32(r21 + 4u, transmission_choice_run_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->state++;
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		r0_3 = OBJECT(current_object)->car_select.time_left;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = binary_to_bcd(r0_3);
		time_digits_draw(r0_3);
		return;
	case 5:
		if (OBJECT(current_object)->car_select.time_left != 0u)
		{
			wheel = SCREEN(screen)->wheel;
			CAR(player_car)->transmission = rom_006DA4[wheel >> 5];
			zero = car_select_wait_confirm();
			if (zero)
			{
				car_select_turn_to_choice();
				car_select_update_rowscroll();
				r0_3 = OBJECT(current_object)->car_select.time_left;
				r0_3 = r0_3 / 0x1Eu;
				r0_3 = binary_to_bcd(r0_3);
				time_digits_draw(r0_3);
				return;
			}
		}
		r21 = start_object();
		mem_write32(r21 + 4u, waiting_banner_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->car_select.transmission_chosen = 0xFFu;
		OBJECT(current_object)->state++;
	case 6:
		OBJECT(current_object)->car_select.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->car_select.entry_over == 0u)
		{
			car_select_turn_to_choice();
			car_select_update_rowscroll();
			return;
		}
		set_player_ready();
		OBJECT(current_object)->state++;
		zero = players_not_ready();
		if (zero)
		{
			car_select_turn_to_choice();
			car_select_update_rowscroll();
			return;
		}
		r0 = 0x3Cu;
		start_dj_message_if_quiet(r0);
		OBJECT(current_object)->state++;
		timer_2 = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			fade_out_music();
			SCREEN(screen)->game_step++;
			OBJECT(current_object)->state++;
		}
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		return;
	case 7:
		if (OBJECT(current_object)->car_select.entry_over == 0u)
		{
			car_select_turn_to_choice();
			car_select_update_rowscroll();
			return;
		}
		set_player_ready();
		OBJECT(current_object)->state++;
		zero = players_not_ready();
		if (zero)
		{
			car_select_turn_to_choice();
			car_select_update_rowscroll();
			return;
		}
		r0 = 0x3Cu;
		start_dj_message_if_quiet(r0);
		OBJECT(current_object)->state++;
		timer_2 = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			fade_out_music();
			SCREEN(screen)->game_step++;
			OBJECT(current_object)->state++;
		}
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		return;
	case 8:
		zero = players_not_ready();
		if (zero)
		{
			car_select_turn_to_choice();
			car_select_update_rowscroll();
			return;
		}
		r0 = 0x3Cu;
		start_dj_message_if_quiet(r0);
		OBJECT(current_object)->state++;
		timer_2 = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			fade_out_music();
			SCREEN(screen)->game_step++;
			OBJECT(current_object)->state++;
		}
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		return;
	case 9:
		timer_2 = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			fade_out_music();
			SCREEN(screen)->game_step++;
			OBJECT(current_object)->state++;
		}
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		return;
	case 10:
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		return;
	default:
		invalid_jump(rom_005058[OBJECT(current_object)->state]);
	}
}

/* Once the car is chosen (car_chosen), turns the turntable the short way round until the chosen car (car
   unk_60) is in front, 20 a frame, skewing the floor as it goes. */
void car_select_turn_to_choice(void)
{
	uint32_t negative = 0;
	uint16_t r1 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->car_select.choice_turn)
	{
	case 0:
		if (OBJECT(current_object)->car_select.car_chosen == 0u)
			return;
		OBJECT(current_object)->car_select.choice_turn++;
		r1 = OBJECT(current_object)->car_select.angle;
		t1 = (((uint32_t)CAR(player_car)->model << 9) - r1) << 4;
		negative = t1 >> 15;
		if (t1 != 0u)
		{
			if (negative)
			{
				r1 -= 0x20u;
				OBJECT(current_object)->car_select.angle = r1;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (r1 >> 2));
				car_select_skew_floor_previous();
				return;
			}
			r1 += 0x20u;
			OBJECT(current_object)->car_select.angle = r1;
			mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (r1 >> 2));
			car_select_skew_floor_next();
			return;
		}
		OBJECT(current_object)->car_select.choice_turn++;
		return;
	case 1:
		r1 = OBJECT(current_object)->car_select.angle;
		t1 = (((uint32_t)CAR(player_car)->model << 9) - r1) << 4;
		negative = t1 >> 15;
		if (t1 != 0u)
		{
			if (negative)
			{
				r1 -= 0x20u;
				OBJECT(current_object)->car_select.angle = r1;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (r1 >> 2));
				car_select_skew_floor_previous();
				return;
			}
			r1 += 0x20u;
			OBJECT(current_object)->car_select.angle = r1;
			mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (r1 >> 2));
			car_select_skew_floor_next();
			return;
		}
		OBJECT(current_object)->car_select.choice_turn++;
		return;
	case 2:
		return;
	default:
		invalid_jump(rom_0053EC[OBJECT(current_object)->car_select.choice_turn]);
	}
}

/* Writes the line scroll of the tile layer for the next frames: the top 104 lines all at top_scroll,
   which moves one more each frame, and while rowscroll_copies says, the floor's 120 lines from their
   offsets at 3800 of the screen. */
void car_select_update_rowscroll(void)
{
	uint8_t r1_2 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;

	r0 = (uint8_t)(unk_20E204 >> 8u);
	r1 = r0;
	r0 = (uint8_t)(r0 + 4u);
	r1 = (uint8_t)(r1 + 8u);
	r11 = OBJECT(current_object)->car_select.rowscroll_address + ((r0 & 0xCu) << 7) * 4u;
	r12 = OBJECT(current_object)->car_select.rowscroll_address + ((r1 & 0xCu) << 7) * 4u;
	r0_2 = (uint16_t)(OBJECT(current_object)->car_select.top_scroll + 1u);
	OBJECT(current_object)->car_select.top_scroll = r0_2;
	r9 = 0x68u;
	do
	{
		mem_write16(r11, r0_2);
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		r9--;
		r11 += 2u;
	} while (r9 != 0u);
	switch (OBJECT(current_object)->car_select.rowscroll_copies)
	{
	case 1:
	case 2:
		r1_2 = 0xFAu;
		r10 = screen + 0x3800u;
		r9 = 0x78u;
		do
		{
			r0_2 = shift_by(mem_read16(r10), (uint8_t)r1_2, 16);
			mem_write16(r11, r0_2);
			r11 += 2u;
			mem_write16(r12, r0_2);
			r12 = r12 + 2u;
			r9--;
			r10 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->car_select.rowscroll_copies--;
		break;
	case 3:
		OBJECT(current_object)->car_select.rowscroll_copies--;
		return;
	case 0:
		break;
	default:
		invalid_jump(rom_00548C[OBJECT(current_object)->car_select.rowscroll_copies]);
	}
}

/* Skews the turntable's floor for a step toward the next car (car_select_skew_floor with 6). */
void car_select_skew_floor_next(void)
{
	uint16_t r0 = 0;

	r0 = 6u;
	car_select_skew_floor(r0);
}

/* Skews the turntable's floor for a step toward the previous car (car_select_skew_floor with -6). */
void car_select_skew_floor_previous(void)
{
	uint16_t r0 = 0;

	r0 = 0xFFFAu;
	car_select_skew_floor(r0);
}

/* Adds a step that grows line by line (by R0) to the floor's 120 line offsets at 3800 of the screen,
   writes them to the next two pages of the line scroll and has car_select_update_rowscroll copy them to
   the others (rowscroll_copies). */
void car_select_skew_floor(uint16_t r0)
{
	uint8_t r4 = 0;
	uint16_t r1_2 = 0, r2_2 = 0, r3 = 0;
	uint32_t r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0, rowscroll_address = 0;

	r1 = (uint8_t)(unk_20E204 >> 8u);
	r2 = r1;
	r1 = (uint8_t)(r1 + 4u);
	r2 = (uint8_t)(r2 + 8u);
	rowscroll_address = OBJECT(current_object)->car_select.rowscroll_address;
	r11 = rowscroll_address + 0xD0u + ((r1 & 0xCu) << 7) * 4u;
	r12 = rowscroll_address + 0xD0u + ((r2 & 0xCu) << 7) * 4u;
	r10 = screen + 0x3800u;
	r3 = 0u;
	r4 = 0xFAu;
	r9 = 0x78u;
	do
	{
		r3 += r0;
		r1_2 = (uint16_t)(mem_read16(r10) + r3);
		r2_2 = (uint16_t)(r1_2 + r3);
		mem_write16(r10, r2_2);
		mem_write16(r11, shift_by(r1_2, (uint8_t)r4, 16));
		r11 += 2u;
		mem_write16(r12, shift_by(r2_2, (uint8_t)r4, 16));
		r12 = r12 + 2u;
		r9--;
		r10 += 2u;
	} while (r9 != 0u);
	OBJECT(current_object)->car_select.rowscroll_copies = 3u;
}

/* The man who stands by the car in front on the car select's turntable, by state (rom_005534): 0 sets
   him up half a car round from the front (angle, radius, size); 1 stands beside the car in front (car
   unk_60) until it changes, then 2 walks round the turntable, 20 a frame, to the front, facing his way
   (flags bit 6), and stands again; once the car is chosen (car_select car_chosen), 3 walks to one side
   of the car or the other by the transmission the wheel picks (side_offset to car unk_61 * 80) and
   stands there; once it is chosen (transmission_chosen), 4 strikes the pose for it (animation 267
   automatic, 268 manual). Each frame he is drawn at his angle (plus side_offset) less the turntable's
   (car_select angle) with turntable_draw, and animated. */
void car_attendant_run(void)
{
	bool L_0055F8 = false;
	uint32_t negative = 0;
	uint16_t r1 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r3 = 0, r9 = 0, ignored = 0, radius = 0;
	uint16_t t1 = 0;
	uint32_t target1 = 0;

	do
	{
		target1 = L_0055F8 ? 0x557Du : rom_005534[OBJECT(current_object)->state];
		switch (target1)
		{
		case 0x005548u:
			OBJECT(current_object)->draw_mode = 0xAu;
			r9 = 0x269u;
			OBJECT(current_object)->palette = 0x7Cu;
			animation_start(r9);
			OBJECT(current_object)->car_attendant.angle = OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24 + 0x100u;
			OBJECT(current_object)->car_attendant.radius = 0x160u;
			OBJECT(current_object)->car_attendant.size = 0xC0u;
			OBJECT(current_object)->state++;
		case 0x00557Du:
			if (L_0055F8 || ((((uint32_t)CAR(player_car)->model << 9) - OBJECT(current_object)->car_attendant.angle) & 0xFFFu) == 0u)
			{
				if (!L_0055F8)
				{
					if ((uint8_t)OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_2C == 0u)
					{
						r0 = OBJECT(current_object)->car_attendant.angle;
						r0 = (uint16_t)(r0 - 0x40u);
						r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
						r0 = r0 & 0xFFFu;
						r1_2 = (int16_t)rom_100C00[r0];
						r3 = 0u - (int16_t)rom_101400[r0];
						radius = OBJECT(current_object)->car_attendant.radius;
						r1_2 = (int32_t)(r1_2 * radius) >> 14;
						r2 = 0x6Cu;
						r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
						turntable_draw(r1_2, r2, r3);
						ignored = animation_advance(0u);
						return;
					}
					OBJECT(current_object)->car_attendant.side_offset = 0;
					OBJECT(current_object)->state = 3u;
				}
				r0 = (uint16_t)OBJECT(current_object)->car_attendant.side_offset;
				r1 = (uint16_t)((uint32_t)CAR(player_car)->transmission << 7);
				if (r0 == r1)
				{
					if ((uint8_t)(OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_2C >> 8u) == 0u)
					{
						r9 = 0x269u;
						if (OBJECT(current_object)->animation != 0x269u)
							animation_start(r9);
						r0 = (uint16_t)(r0 + OBJECT(current_object)->car_attendant.angle);
						r0 = (uint16_t)(r0 - 0x40u);
						r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
						r0 = r0 & 0xFFFu;
						r1_2 = (int16_t)rom_100C00[r0];
						r3 = 0u - (int16_t)rom_101400[r0];
						radius = OBJECT(current_object)->car_attendant.radius;
						r1_2 = (int32_t)(r1_2 * radius) >> 14;
						r2 = 0x6Cu;
						r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
						turntable_draw(r1_2, r2, r3);
						ignored = animation_advance(0u);
						return;
					}
					OBJECT(current_object)->car_attendant.angle += OBJECT(current_object)->car_attendant.side_offset;
					r9 = 0x267u;
					if (CAR(player_car)->transmission != 0u)
						r9 = 0x268u;
					animation_start(r9);
					OBJECT(current_object)->state++;
					r0 = OBJECT(current_object)->car_attendant.angle;
					r0 = (uint16_t)(r0 - 0x40u);
					r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
					r0 = r0 & 0xFFFu;
					r1_2 = (int16_t)rom_100C00[r0];
					r3 = 0u - (int16_t)rom_101400[r0];
					radius = OBJECT(current_object)->car_attendant.radius;
					r1_2 = (int32_t)(r1_2 * radius) >> 14;
					r2 = 0x6Cu;
					r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
					turntable_draw(r1_2, r2, r3);
					ignored = animation_advance(0u);
					return;
				}
				if ((int16_t)r1 < (int16_t)r0)
				{
					OBJECT(current_object)->flags &= 0xBFu;
					r0 = (uint16_t)(r0 - 0x20u);
				}
				else
				{
					OBJECT(current_object)->flags |= 0x40u;
					r0 = (uint16_t)(r0 + 0x20u);
				}
				OBJECT(current_object)->car_attendant.side_offset = r0;
				r9 = 0x266u;
				if (OBJECT(current_object)->animation == 0x266u)
				{
					r0 = (uint16_t)(r0 + OBJECT(current_object)->car_attendant.angle);
					r0 = (uint16_t)(r0 - 0x40u);
					r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
					r0 = r0 & 0xFFFu;
					r1_2 = (int16_t)rom_100C00[r0];
					r3 = 0u - (int16_t)rom_101400[r0];
					radius = OBJECT(current_object)->car_attendant.radius;
					r1_2 = (int32_t)(r1_2 * radius) >> 14;
					r2 = 0x6Cu;
					r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
					turntable_draw(r1_2, r2, r3);
					ignored = animation_advance(0u);
					return;
				}
				animation_start(r9);
				r0 = (uint16_t)(r0 + OBJECT(current_object)->car_attendant.angle);
				r0 = (uint16_t)(r0 - 0x40u);
				r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
				r0 = r0 & 0xFFFu;
				r1_2 = (int16_t)rom_100C00[r0];
				r3 = 0u - (int16_t)rom_101400[r0];
				radius = OBJECT(current_object)->car_attendant.radius;
				r1_2 = (int32_t)(r1_2 * radius) >> 14;
				r2 = 0x6Cu;
				r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
				turntable_draw(r1_2, r2, r3);
				ignored = animation_advance(0u);
				return;
			}
			r9 = 0x266u;
			animation_start(r9);
			OBJECT(current_object)->state++;
		case 0x0055A6u:
			r0 = OBJECT(current_object)->car_attendant.angle;
			t1 = (r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24) << 4;
			negative = t1 >> 15;
			if (t1 == 0u)
			{
				r9 = 0x269u;
				animation_start(r9);
				OBJECT(current_object)->flags &= 0xBFu;
				OBJECT(current_object)->state--;
			}
			else if (negative)
			{
				OBJECT(current_object)->flags |= 0x40u;
				r0 = (uint16_t)(r0 + 0x20u);
				OBJECT(current_object)->car_attendant.angle = r0;
			}
			else
			{
				OBJECT(current_object)->flags &= 0xBFu;
				r0 = (uint16_t)(r0 - 0x20u);
				OBJECT(current_object)->car_attendant.angle = r0;
			}
			break;
		case 0x0055F8u:
			L_0055F8 = true;
			break;
		case 0x00566Au:
			r0 = OBJECT(current_object)->car_attendant.angle;
			r0 = (uint16_t)(r0 - 0x40u);
			r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
			r0 = r0 & 0xFFFu;
			r1_2 = (int16_t)rom_100C00[r0];
			r3 = 0u - (int16_t)rom_101400[r0];
			radius = OBJECT(current_object)->car_attendant.radius;
			r1_2 = (int32_t)(r1_2 * radius) >> 14;
			r2 = 0x6Cu;
			r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
			turntable_draw(r1_2, r2, r3);
			ignored = animation_advance(0u);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_0055F8);
	r0 = (uint16_t)(r0 - 0x40u);
	r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->car_attendant.car_select)->unk_24);
	r0 = r0 & 0xFFFu;
	r1_2 = (int16_t)rom_100C00[r0];
	r3 = 0u - (int16_t)rom_101400[r0];
	radius = OBJECT(current_object)->car_attendant.radius;
	r1_2 = (int32_t)(r1_2 * radius) >> 14;
	r2 = 0x6Cu;
	r3 = ((int32_t)(r3 * radius) >> 14) + 0x140u;
	turntable_draw(r1_2, r2, r3);
	ignored = animation_advance(0u);
}

/* One of the eight cars on the car select's turntable (car: its model and its place, car << 9), by
   state: 0 its sprite set and palette (rom_0069B8 by car); 1 placed by its angle less the turntable's
   (car_select angle), with the frame that shows it from there (rom_0069D8), round the centre (radius
   screen unk_F08 + 140) with turntable_draw; once the car is chosen (car_select car_chosen) the car in
   front takes the palette 4 on if it is the chosen one (car unk_60) and goes to 2, where draw_object
   draws it where it was. */
void turntable_car_show(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint16_t t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0056C0[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r10 = rom_0069B8_ADDRESS + OBJECT(current_object)->turntable_car.car * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->palette = ROM16(rom_000002_ADDRESS + r10);
		animation_start(r9);
		OBJECT(current_object)->turntable_car.size = 0x100u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0056C0[state]);
	}
	r0 = OBJECT(current_object)->turntable_car.car;
	r0 = (uint16_t)(r0 << 9);
	r0 = (uint16_t)(r0 - OBJECT(OBJECT(current_object)->turntable_car.car_select)->unk_24);
	t1 = r0 & 0xFFFu;
	r0 = t1;
	if (t1 == 0u && (uint8_t)OBJECT(OBJECT(current_object)->turntable_car.car_select)->unk_2C != 0u)
	{
		if (OBJECT(current_object)->turntable_car.car == CAR(player_car)->model)
			OBJECT(current_object)->palette += 4u;
		OBJECT(current_object)->state++;
	}
	OBJECT(current_object)->frame = rom_0069D8[r0 >> 6];
	r2 = SCREEN(screen)->unk_F08 + 0x140u;
	r1 = (int32_t)((int16_t)rom_100C00[r0] * r2) >> 14;
	r3 = (int32_t)((0u - (int16_t)rom_101400[r0]) * r2) >> 14;
	r2 = 0x70u;
	r3 += 0x140u;
	ignored = turntable_draw(r1, r2, r3);
}

/* A ring of scenery round the car select's turntable (one of twelve groups, from 006A18: the crowd, the
   palm trees), by state: 0 its animation, palette, size_step, size and list of items (item_list: angle,
   radius and height of each, closed by a negative angle); 1 each frame takes the turntable's angle
   (car_select angle) and draws the items on the far side (their angle less it 480..B7F) with
   turntable_draw, keeping for each its previous_place and flags bit 5 in item_places for draw mode 0A's
   halfway copy; then animation_advance. */
void turntable_scenery_show(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0;
	uint32_t stack_4 = 0, stack_8 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r10 = 0x6A18u + OBJECT(current_object)->turntable_scenery.group * 2u * 4u;
		r9 = ROM16(r10);
		r10 += 2u;
		animation_start(r9);
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		r0 = ROM16(r10);
		OBJECT(current_object)->size_step = r0 >> 12;
		OBJECT(current_object)->turntable_scenery.size = r0 & 0xFFFu;
		OBJECT(current_object)->turntable_scenery.item_list = r10 + 2u + ROM16(r10 + 2u);
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_005770[OBJECT(current_object)->state]);
	}
	r10 = OBJECT(current_object)->turntable_scenery.item_list;
	r11 = current_object + 0x80u;
	OBJECT(current_object)->turntable_scenery.angle = OBJECT(OBJECT(current_object)->turntable_scenery.car_select)->unk_24;
	for (;;)
	{
		r0_2 = mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
			break;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->turntable_scenery.angle);
		r0_2 = r0_2 & 0xFFFu;
		r2 = mem_read16(r10);
		r10 += 2u;
		if (r0_2 < 0x480u || r0_2 > 0xB7Fu)
		{
			r10 += 2u;
			r11 += 2u;
			mem_write16(r11, 0u);
			r11 += 2u;
			continue;
		}
		r2 += SCREEN(screen)->unk_F08;
		r1 = (int32_t)((int16_t)rom_100C00[r0_2] * r2) >> 14;
		r3 = (int32_t)((0u - (int16_t)rom_101400[r0_2]) * r2) >> 14;
		r2 = (int16_t)mem_read16(r10);
		r3 += 0x140u;
		stack_4 = r11;
		OBJECT(current_object)->turntable_scenery.previous_place = (screen & ~0xFFFFu) | mem_read16(r11);
		OBJECT(current_object)->flags = (OBJECT(current_object)->flags & 0xDFu) | mem_read16(r11 + 2u);
		stack_8 = r10 + 2u;
		r0_2 = turntable_draw(r1, r2, r3);
		r10 = stack_8;
		r11 = stack_4;
		mem_write16(r11, (uint16_t)OBJECT(current_object)->turntable_scenery.previous_place);
		r0 = (uint16_t)((r0_2 & ~0xFFu) | OBJECT(current_object)->flags);
		r11 += 2u;
		mem_write16(r11, (r0 & ~0xFFu) | (r0 & 0x20u));
		r11 += 2u;
	}
	ignored = animation_advance(0u);
}

/* Starts the eight letters of WELCOME! (itself the first), then welcome_letter_show. */
void welcome_letter_start(void)
{
	uint32_t r0 = 0, r21 = 0;

	r0 = 7u;
	do
	{
		r21 = start_object();
		mem_write32(r21 + 4u, welcome_letter_show_ADDRESS);
		mem_write8(r21 + 9u, r0);
		r0--;
	} while (r0 != 0u);
	OBJECT(current_object)->welcome_letter.index = r0;
	OBJECT(current_object)->handler = welcome_letter_show_ADDRESS;
	welcome_letter_show();
}

/* Shows its letter of WELCOME! (font 346): appears 2 frames after the one before it, cycles its colours
   (step_palette, from 006DCC) and ends 45 frames after the start. */
void welcome_letter_show(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t frames_left = 0, t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_005890[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x346u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x18u;
		r0 = OBJECT(current_object)->welcome_letter.index;
		r10 = 0x6DACu + r0 * 4u;
		OBJECT(current_object)->x = ROM16(r10);
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->frame = ROM8(rom_000002_ADDRESS + r10);
		r0 = (uint16_t)(r0 * 2u);
		OBJECT(current_object)->welcome_letter.delay = r0;
		OBJECT(current_object)->welcome_letter.frames_left = 0x2Du;
		OBJECT(current_object)->welcome_letter.palette_list = 0x6DCCu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		frames_left = OBJECT(current_object)->welcome_letter.frames_left;
		OBJECT(current_object)->welcome_letter.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(r0);
		step_palette();
		return;
	default:
		invalid_jump(rom_005890[state]);
	}
	OBJECT(current_object)->welcome_letter.frames_left--;
	t1 = OBJECT(current_object)->welcome_letter.delay - 1u;
	OBJECT(current_object)->welcome_letter.delay = t1;
	if ((int16_t)t1 >= 0)
		return;
	OBJECT(current_object)->state++;
	frames_left = OBJECT(current_object)->welcome_letter.frames_left;
	OBJECT(current_object)->welcome_letter.frames_left = frames_left - 1u;
	if (frames_left == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(r0);
	step_palette();
}

/* "To Select A Car," at the top of the car select, by state: 0 starts the four pieces of the
   instructions under it (select_instruction, records at 006DF6) and sets it up; 1 drawn 20 frames
   (timer); 2 hidden 10 frames; it ends as soon as the car is chosen (car_select car_chosen). */
void select_car_heading_show(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint16_t timer = 0, timer_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_005914[state];
	switch (state)
	{
	case 0:
		r10 = 0x6DF6u;
		while (ROM16(r10) != 0u)
		{
			r21 = start_object();
			mem_write32(r21 + 4u, select_instruction_show_ADDRESS);
			mem_write32(r21 + 0x14u, OBJECT(current_object)->select_car_heading.car_select);
			mem_write16(r21 + 0x18u, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x3Au, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x30u, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x32u, ROM16(r10));
			r10 += 2u;
		}
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x2B5u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x2A6u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x30u;
		OBJECT(current_object)->select_car_heading.timer = 0x14u;
		OBJECT(current_object)->state++;
		if ((uint8_t)OBJECT(OBJECT(current_object)->select_car_heading.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer - 1u;
		if (timer == 1u)
		{
			AS8(OBJECT(current_object)->select_car_heading.timer) = 0xAu;
			OBJECT(current_object)->state++;
		}
		r0 = draw_object(r0);
		return;
	case 1:
		if ((uint8_t)OBJECT(OBJECT(current_object)->select_car_heading.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer - 1u;
		if (timer == 1u)
		{
			AS8(OBJECT(current_object)->select_car_heading.timer) = 0xAu;
			OBJECT(current_object)->state++;
		}
		r0 = draw_object(r0);
		return;
	case 2:
		if ((uint8_t)OBJECT(OBJECT(current_object)->select_car_heading.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer_2 = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			AS8(OBJECT(current_object)->select_car_heading.timer) = 0x14u;
			OBJECT(current_object)->state--;
		}
		return;
	default:
		invalid_jump(rom_005914[state]);
	}
}

/* A piece of the car select's instructions ("Turn your", the steering wheel, "Then foot the", the
   accelerator; piece_animation, palette and place from select_car_heading), by state: 0 starts its
   animation; 1 drawn and animated until the car is chosen (car_select car_chosen); 2 rises 2 a frame for
   16 frames (timer), over the transmission choice; 3 drawn and animated until the transmission is chosen
   (transmission_chosen), when it ends. */
void select_instruction_show(void)
{
	uint32_t r0 = 0, ignored = 0, piece_animation = 0;
	uint16_t timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0059C4[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		piece_animation = OBJECT(current_object)->select_instruction.piece_animation;
		animation_start(piece_animation);
		OBJECT(current_object)->state++;
		if ((uint8_t)OBJECT(OBJECT(current_object)->select_instruction.car_select)->unk_2C == 0u)
		{
			draw_object(r0);
			ignored = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->select_instruction.timer = 0x10u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if ((uint8_t)OBJECT(OBJECT(current_object)->select_instruction.car_select)->unk_2C == 0u)
		{
			draw_object(r0);
			ignored = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->select_instruction.timer = 0x10u;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y -= 2u;
		timer = OBJECT(current_object)->select_instruction.timer;
		OBJECT(current_object)->select_instruction.timer = timer - 1u;
		if (timer != 1u)
		{
			draw_object(r0);
			ignored = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->state++;
		if ((uint8_t)(OBJECT(OBJECT(current_object)->select_instruction.car_select)->unk_2C >> 8u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(r0);
		ignored = animation_advance(0u);
		return;
	case 2:
		break;
	case 3:
		if ((uint8_t)(OBJECT(OBJECT(current_object)->select_instruction.car_select)->unk_2C >> 8u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(r0);
		ignored = animation_advance(0u);
		return;
	default:
		invalid_jump(rom_0059C4[state]);
	}
	OBJECT(current_object)->y -= 2u;
	timer = OBJECT(current_object)->select_instruction.timer;
	OBJECT(current_object)->select_instruction.timer = timer - 1u;
	if (timer != 1u)
	{
		draw_object(r0);
		ignored = animation_advance(0u);
		return;
	}
	OBJECT(current_object)->state++;
	if ((uint8_t)(OBJECT(OBJECT(current_object)->select_instruction.car_select)->unk_2C >> 8u) != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(r0);
	ignored = animation_advance(0u);
}

/* The transmission choice of the car select: the AUTOMATIC and MANUAL panels (on the second tile layer,
   with the sprites automatic_panel and manual_panel following x and y), by state: 0 starts the two
   panels; 1 slides them down into view, 10 a frame for 14 frames (y, car_select
   second_scroll_y_address); 2 when the wheel picks the other transmission (car unk_61 against
   shown_transmission) queues the panels' colours for it (queue_palette_list 13F998 automatic, 13F95C
   manual); when it is chosen (car_select transmission_chosen) starts the driver couple (driver_couple)
   and 3 slides the panels aside, 10 a frame for 10 frames (slide_step, slide_frames; x,
   second_scroll_x_address), the chosen one to the half the couple leaves free; 4 stays. From 2 on, every
   frame, the colours of the transmission picked or chosen cycle: the next palette list of rom_006E30
   (automatic, automatic_cycle) or rom_006E44 (manual, manual_cycle) every two frames. */
void transmission_choice_run(void)
{
	uint8_t r0 = 0;
	uint16_t slide_step = 0;
	uint32_t r19 = 0, r21 = 0, object = 0, transmission = 0;
	uint8_t slide_frames = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0, t3 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r21 = start_object();
		mem_write32(r21 + 4u, automatic_panel_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = start_object();
		mem_write32(r21 + 4u, manual_panel_show_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->transmission_choice.shown_transmission = 0xFFu;
		OBJECT(current_object)->state++;
	case 1:
		object = OBJECT(current_object)->transmission_choice.car_select;
		OBJECT(current_object)->y -= 0x10u;
		t1 = OBJECT(object)->unk_AC;
		t2 = mem_read16(t1);
		mem_write16(t1, t2 - 0x10u);
		if (t2 != 0x10u)
			return;
		OBJECT(current_object)->state++;
	case 2:
		if ((uint8_t)(OBJECT(OBJECT(current_object)->transmission_choice.car_select)->unk_2C >> 8u) != 0u)
		{
			OBJECT(current_object)->transmission_choice.slide_step = 0x10;
			r0 = 0u;
			if (CAR(player_car)->transmission == 0u)
			{
				OBJECT(current_object)->transmission_choice.slide_step = (int8_t)0xF0u;
				r0 = 1u;
			}
			OBJECT(current_object)->transmission_choice.slide_frames = 0xAu;
			r21 = start_object();
			mem_write32(r21 + 4u, driver_couple_start_ADDRESS);
			mem_write8(r21 + 9u, r0);
			OBJECT(current_object)->state++;
			object = OBJECT(current_object)->transmission_choice.car_select;
			slide_step = (uint16_t)OBJECT(current_object)->transmission_choice.slide_step;
			OBJECT(current_object)->x += slide_step;
			t3 = OBJECT(object)->unk_A8;
			mem_write16(t3, mem_read16(t3) + slide_step);
			slide_frames = OBJECT(current_object)->transmission_choice.slide_frames;
			OBJECT(current_object)->transmission_choice.slide_frames = slide_frames - 1u;
			if (slide_frames == 1u)
				OBJECT(current_object)->state++;
		}
		else
		{
			transmission = CAR(player_car)->transmission;
			if (OBJECT(current_object)->transmission_choice.shown_transmission != transmission)
			{
				OBJECT(current_object)->transmission_choice.shown_transmission = transmission;
				r19 = 0x13F998u;
				if (transmission != 0u)
					r19 = 0x13F95Cu;
				queue_palette_list(r19);
			}
		}
		break;
	case 3:
		object = OBJECT(current_object)->transmission_choice.car_select;
		slide_step = (uint16_t)OBJECT(current_object)->transmission_choice.slide_step;
		OBJECT(current_object)->x += slide_step;
		t3 = OBJECT(object)->unk_A8;
		mem_write16(t3, mem_read16(t3) + slide_step);
		slide_frames = OBJECT(current_object)->transmission_choice.slide_frames;
		OBJECT(current_object)->transmission_choice.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 4:
		break;
	default:
		invalid_jump(rom_005A28[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission == 0u)
	{
		r19 = rom_006E30[OBJECT(current_object)->transmission_choice.automatic_cycle >> 1];
		if (r19 == 0u)
		{
			OBJECT(current_object)->transmission_choice.automatic_cycle = 0u;
			r19 = rom_006E30[0];
		}
		OBJECT(current_object)->transmission_choice.automatic_cycle++;
		queue_palette_list(r19);
		return;
	}
	r19 = rom_006E44[OBJECT(current_object)->transmission_choice.manual_cycle >> 1];
	if (r19 == 0u)
	{
		OBJECT(current_object)->transmission_choice.manual_cycle = 0u;
		r19 = rom_006E44[0];
	}
	OBJECT(current_object)->transmission_choice.manual_cycle++;
	queue_palette_list(r19);
}

/* The AUTOMATIC TRANSMISSION title of the left panel of the transmission choice, by state (rom_005B50):
   0 starts its label (transmission_label_start_automatic) and sets it up; 1 cycles its colours
   (step_palette from palette_list) while the wheel picks automatic (car unk_61 0), else palette 2AE;
   drawn at x_offset, y_offset less the choice's x, y (transmission_choice), which slide it. Nothing sets
   state 2 (drawing only). */
void automatic_panel_show(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0, object = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r21 = start_object();
		mem_write32(r21 + 4u, transmission_label_start_automatic_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		r9 = 0x2BDu;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->automatic_panel.x_offset = 0x54;
		OBJECT(current_object)->automatic_panel.y_offset = -0x6C;
		OBJECT(current_object)->automatic_panel.palette_list = 0x6E18u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->automatic_panel.x_offset;
		r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->automatic_panel.y_offset);
		OBJECT(current_object)->y = r0;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_005B50[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission != 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->automatic_panel.x_offset;
		r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->automatic_panel.y_offset);
		OBJECT(current_object)->y = r0;
		r0 = draw_object(r0);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->automatic_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->automatic_panel.x_offset;
	r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->automatic_panel.y_offset);
	OBJECT(current_object)->y = r0;
	r0 = draw_object(r0);
}

/* The MANUAL TRANSMISSION title of the right panel of the transmission choice, by state (rom_005BE4): 0
   starts its label (transmission_label_start_manual) and sets it up; 1 cycles its colours (step_palette
   from palette_list) while the wheel picks manual (car unk_61 1), else palette 2AE; drawn at x_offset,
   y_offset less the choice's x, y (transmission_choice), which slide it. Nothing sets state 2 (drawing
   only). */
void manual_panel_show(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0, object = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r21 = start_object();
		mem_write32(r21 + 4u, transmission_label_start_manual_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		r9 = 0x2BEu;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->manual_panel.x_offset = 0xEC;
		OBJECT(current_object)->manual_panel.y_offset = -0x6C;
		OBJECT(current_object)->manual_panel.palette_list = 0x6E18u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->manual_panel.x_offset;
		r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->manual_panel.y_offset);
		OBJECT(current_object)->y = r0;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_005BE4[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission == 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->manual_panel.x_offset;
		r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->manual_panel.y_offset);
		OBJECT(current_object)->y = r0;
		r0 = draw_object(r0);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->manual_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->manual_panel.x_offset;
	r0 = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->manual_panel.y_offset);
	OBJECT(current_object)->y = r0;
	r0 = draw_object(r0);
}

/* Sets up the label of the automatic transmission's panel (sprite set 2C1, 24 below the panel's place;
   the frames show NOVICE there), then transmission_label_follow. */
void transmission_label_start_automatic(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	r9 = 0x2C1u;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x2A8u;
	OBJECT(current_object)->transmission_label.x_offset = 0;
	OBJECT(current_object)->transmission_label.y_offset = 0x24;
	OBJECT(current_object)->handler = transmission_label_follow_ADDRESS;
	transmission_label_follow();
}

/* Sets up the label of the manual transmission's panel (sprite set 2C9, its unk_40 from rom_006E78 by
   the car's model; the frames show 5 SPEED there for the car coin_start takes), then
   transmission_label_follow. */
void transmission_label_start_manual(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	r9 = 0x2C9u;
	animation_start(r9);
	OBJECT(current_object)->frame = rom_006E78[CAR(player_car)->model];
	OBJECT(current_object)->palette = 0x2A8u;
	OBJECT(current_object)->transmission_label.x_offset = -0x18;
	OBJECT(current_object)->transmission_label.y_offset = 0x5C;
	OBJECT(current_object)->handler = transmission_label_follow_ADDRESS;
	transmission_label_follow();
}

/* Keeps the label on its panel (the panel's place plus its offsets, the panel's palette) and draws it;
   ends when the panel ends. */
void transmission_label_follow(void)
{
	uint32_t r0 = 0, object = 0;

	object = OBJECT(current_object)->transmission_label.panel;
	if ((OBJECT(object)->flags & 1u) == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->x = OBJECT(object)->x + OBJECT(current_object)->transmission_label.x_offset;
	r0 = (uint16_t)(OBJECT(object)->y + OBJECT(current_object)->transmission_label.y_offset);
	OBJECT(current_object)->y = r0;
	OBJECT(current_object)->palette = OBJECT(object)->palette;
	r0 = draw_object(r0);
}

/* Sets up the couple who drive the chosen car (a man and a woman, by the car's model: 006E58) on the
   left (x 50) after automatic (left_side) or on the right (F0) after manual, the half the panels slid
   away from; draw_object then draws it as its handler. */
void driver_couple_start(void)
{
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0xEFu;
	r10 = 0x6E58u + CAR(player_car)->model * 4u;
	r9 = ROM16(r10);
	OBJECT(current_object)->palette = ROM16(rom_000002_ADDRESS + r10);
	animation_start(r9);
	OBJECT(current_object)->x = 0xF0u;
	if (OBJECT(current_object)->driver_couple.left_side != 0u)
		OBJECT(current_object)->x = 0x50u;
	OBJECT(current_object)->y = 0xD0u;
	OBJECT(current_object)->handler = draw_object_ADDRESS;
}

/* The caption of the car in front on the car select (EASY HANDLING, SMOOTH OPERATOR...: animation 26E,
   frame car unk_60, palette rom_006DDE), by state: 0 starts the gearboxes under it (car_gears_caption)
   and sets it up; 1 drawn 10 frames (timer); 2 hidden 10 frames; a new car in front (shown_car) shows it
   at once, for 10 frames more; it ends when the car is chosen (car_select car_chosen). */
void car_caption_show(void)
{
	uint8_t model = 0;
	uint32_t r0 = 0, r9 = 0, r21 = 0;
	uint16_t timer = 0, timer_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r21 = start_object();
		mem_write32(r21 + 4u, car_gears_caption_show_ADDRESS);
		mem_write32(r21 + 0x14u, OBJECT(current_object)->car_caption.car_select);
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x26Eu;
		animation_start(r9);
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xC4u;
		OBJECT(current_object)->car_caption.timer = 0xAu;
		OBJECT(current_object)->state++;
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = CAR(player_car)->model;
		if (OBJECT(current_object)->car_caption.shown_car != r0)
		{
			OBJECT(current_object)->car_caption.shown_car = r0;
			OBJECT(current_object)->car_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_caption.timer;
		OBJECT(current_object)->car_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = r0;
		OBJECT(current_object)->palette = rom_006DDE[r0];
		r0 = draw_object(r0);
		return;
	case 1:
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = CAR(player_car)->model;
		if (OBJECT(current_object)->car_caption.shown_car != r0)
		{
			OBJECT(current_object)->car_caption.shown_car = r0;
			OBJECT(current_object)->car_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_caption.timer;
		OBJECT(current_object)->car_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = r0;
		OBJECT(current_object)->palette = rom_006DDE[r0];
		r0 = draw_object(r0);
		return;
	case 2:
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_caption.shown_car == model)
		{
			timer_2 = OBJECT(current_object)->car_caption.timer;
			OBJECT(current_object)->car_caption.timer = timer_2 - 1u;
			if (timer_2 == 1u)
			{
				OBJECT(current_object)->car_caption.shown_car = model;
				OBJECT(current_object)->car_caption.timer = 0xAu;
				OBJECT(current_object)->state--;
			}
			return;
		}
		OBJECT(current_object)->car_caption.shown_car = model;
		OBJECT(current_object)->car_caption.timer = 0xAu;
		OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(rom_005D68[OBJECT(current_object)->state]);
	}
}

/* The gearboxes of the car in front under its caption (Auto/5 Speed...: animation 2D1, frame rom_006E78
   by car unk_60, the manual box's), blinking like car_caption: 1 drawn 10 frames (timer), 2 hidden 10,
   at once for a new car (shown_car); it ends when the car is chosen (car_select car_chosen). */
void car_gears_caption_show(void)
{
	uint8_t model = 0;
	uint32_t r0 = 0, r9 = 0;
	uint16_t timer = 0, timer_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x2D1u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x2AFu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD4u;
		OBJECT(current_object)->car_gears_caption.shown_car = 0xFFu;
		OBJECT(current_object)->state++;
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = CAR(player_car)->model;
		if (OBJECT(current_object)->car_gears_caption.shown_car != r0)
		{
			OBJECT(current_object)->car_gears_caption.shown_car = r0;
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_gears_caption.timer;
		OBJECT(current_object)->car_gears_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = rom_006E78[r0];
		r0 = draw_object(r0);
		return;
	case 1:
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = CAR(player_car)->model;
		if (OBJECT(current_object)->car_gears_caption.shown_car != r0)
		{
			OBJECT(current_object)->car_gears_caption.shown_car = r0;
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_gears_caption.timer;
		OBJECT(current_object)->car_gears_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = rom_006E78[r0];
		r0 = draw_object(r0);
		return;
	case 2:
		if ((uint8_t)OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->unk_2C != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_gears_caption.shown_car == model)
		{
			timer_2 = OBJECT(current_object)->car_gears_caption.timer;
			OBJECT(current_object)->car_gears_caption.timer = timer_2 - 1u;
			if (timer_2 == 1u)
			{
				OBJECT(current_object)->car_gears_caption.shown_car = model;
				OBJECT(current_object)->car_gears_caption.timer = 0xAu;
				OBJECT(current_object)->state--;
			}
			return;
		}
		OBJECT(current_object)->car_gears_caption.shown_car = model;
		OBJECT(current_object)->car_gears_caption.timer = 0xAu;
		OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(rom_005E1C[OBJECT(current_object)->state]);
	}
}

/* The car select's message once the choices are made, by state: 0 sets up WAITING FOR CHALLENGERS! JUST
   A MOMENT PLEASE (animation 28B); 1 drawn 20 frames (timer); 2 hidden 10; once the entry time is over
   (car_select entry_over) JUST A MOMENT PLEASE (animation 28D) instead: 3 drawn 20 frames, 4 hidden 10. */
void waiting_banner_show(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t timer = 0, timer_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_005EC0[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 0x28Bu;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x287u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x88u;
		OBJECT(current_object)->waiting_banner.timer = 0x14u;
		OBJECT(current_object)->state++;
		if ((uint8_t)OBJECT(OBJECT(current_object)->waiting_banner.car_select)->unk_B0 != 0u)
		{
			r9 = 0x28Du;
			animation_start(r9);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
		}
		timer = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		r0 = draw_object(r0);
		return;
	case 1:
		if ((uint8_t)OBJECT(OBJECT(current_object)->waiting_banner.car_select)->unk_B0 != 0u)
		{
			r9 = 0x28Du;
			animation_start(r9);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
		}
		timer = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		r0 = draw_object(r0);
		return;
	case 3:
		timer = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		r0 = draw_object(r0);
		return;
	case 2:
		if ((uint8_t)OBJECT(OBJECT(current_object)->waiting_banner.car_select)->unk_B0 != 0u)
		{
			r9 = 0x28Du;
			animation_start(r9);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
			timer = OBJECT(current_object)->waiting_banner.timer;
			OBJECT(current_object)->waiting_banner.timer = timer - 1u;
			if (timer == 1u)
			{
				OBJECT(current_object)->waiting_banner.timer = 0xAu;
				OBJECT(current_object)->state++;
			}
			r0 = draw_object(r0);
			return;
		}
		timer_2 = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state--;
		}
		return;
	case 4:
		timer_2 = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state--;
		}
		return;
	default:
		invalid_jump(rom_005EC0[state]);
	}
}

/* The other players on the car select, by state: 0 its list of the race's players (race_order: the
   count, then the players); 1 when the LCD is not scrolling a text (screen unk_F44) writes Entry: on it
   with _ at each player's car number (car_numbers); then every frame: this screen's number blinks with _
   every 8 frames (own_number, digit_toggle, blink_timer); each other player not yet marked (a bit each
   from 40, the frame field) gets its number on the LCD and, once it has chosen a car (byte 48 of its
   race_copy record, which car_select writes), a flag over that car on the turntable (choice_flag, kept
   in car_flags by car; one more player on the same car gets a stacked_choice_flag on the last one) and
   its bit; flags_visible follows the blink pattern rom_006EA0 (frames >> 1). */
void car_select_entries_run(void)
{
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r21 = 0;
	uint8_t blink_timer = 0;
	uint32_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->car_select_entries.race_order = SCREEN(screen)->host_record + 0x1Fu;
		AS8(OBJECT(current_object)->car_select_entries.shown_players) = 0u;
		OBJECT(current_object)->car_select_entries.frames = 0u;
		OBJECT(current_object)->state++;
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		r9 = player_count;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + r9), 0x5Fu);
			r9--;
		} while (r9 != 0u);
		r0 = car_numbers[SCREEN(screen)->player];
		OBJECT(current_object)->car_select_entries.own_number = r0;
		OBJECT(current_object)->car_select_entries.digit_toggle = (r0 + 0x31u) ^ 0x5Fu;
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		r9 = player_count;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + r9), 0x5Fu);
			r9--;
		} while (r9 != 0u);
		r0 = car_numbers[SCREEN(screen)->player];
		OBJECT(current_object)->car_select_entries.own_number = r0;
		OBJECT(current_object)->car_select_entries.digit_toggle = (r0 + 0x31u) ^ 0x5Fu;
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_005F5C[OBJECT(current_object)->state]);
	}
	blink_timer = OBJECT(current_object)->car_select_entries.blink_timer;
	OBJECT(current_object)->car_select_entries.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		t1 = screen + 0xF18u + OBJECT(current_object)->car_select_entries.own_number;
		mem_write8(t1, mem_read8(t1) ^ OBJECT(current_object)->car_select_entries.digit_toggle);
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
	}
	r10 = OBJECT(current_object)->car_select_entries.race_order;
	r8 = mem_read8(r10);
	r10++;
	do
	{
		r0_2 = mem_read8(r10);
		r10++;
		if (SCREEN(screen)->player != r0_2 && (OBJECT(current_object)->car_select_entries.shown_players & shl32(1u, (int)(r0_2))) == 0u)
		{
			r1 = car_numbers[r0_2];
			mem_write8(screen + 0xF18u + r1, r1 + 0x31u);
			t2 = mem_read8(0x20FC48u + (r0_2 << 7)) - 1u;
			r1 = t2;
			if ((int32_t)t2 >= 0)
			{
				r11 = mem_read32(current_object + 0x80u + r1 * 4u);
				r21 = start_object();
				if (r11 == 0u)
				{
					mem_write32(r21 + 4u, choice_flag_start_ADDRESS);
					mem_write32(r21 + 0x18u, r21);
					mem_write32(r21 + 0x14u, current_object);
					mem_write32(current_object + 0x80u + r1 * 4u, r21);
				}
				else
				{
					mem_write32(r21 + 4u, stacked_choice_flag_start_ADDRESS);
					mem_write32(r21 + 0x14u, mem_read32(r11 + 0x18u));
					mem_write32(r11 + 0x18u, r21);
				}
				mem_write8(r21 + 9u, r0_2);
				mem_write8(r21 + 0xAu, r1);
				OBJECT(current_object)->car_select_entries.shown_players |= shl32(1u, (int)(r0_2));
			}
		}
		r8--;
	} while (r8 != 0u);
	r0_2 = OBJECT(current_object)->car_select_entries.frames;
	r0_2 = (uint8_t)(r0_2 + 1u);
	OBJECT(current_object)->car_select_entries.frames = r0_2;
	r0_2 = r0_2 >> 1;
	OBJECT(current_object)->car_select_entries.flags_visible = rom_006EA0[r0_2 & 0xFu];
}
