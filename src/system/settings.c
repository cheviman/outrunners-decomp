/**
 *    Name:        system/settings.c
 *    Description: the settings in the EEPROM: loading, checking and saving them, and the EEPROM's serial lines
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Loads the settings from the EEPROM at start-up (eeprom_read_settings into 20E600-20E67F) and repairs
   them: a block marked SEGA gets the defaults (0004C0, marked S32A) with eeprom_write_count 100; one
   without the S32A mark the defaults; with it, a new program (signature_checksum unlike
   settings_signature) gets the defaults keeping the eeprom_write_count of a sound copy (the settings or
   their backup, each checked with its crc16); with the same program a bad copy (or a backup unlike the
   settings) is rewritten from the other, both bad the defaults. Every repair but rewriting a copy also
   resets the backup RAM's data (bookkeeping_reset); each one saves the result (eeprom_write_settings). */
void load_settings(uint32_t checksum)
{
	uint16_t write_count = 0;
	uint32_t new_checksum = 0, settings = 0, settings_length = 0;

	eeprom_read_settings(checksum);
	if (settings_magic == 0x41474553u)
	{
		bookkeeping_reset(checksum);
		move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
		eeprom_write_count = 0x64u;
		backup_eeprom_write_count = 0x64u;
		new_checksum = signature_checksum();
		settings_signature = new_checksum;
		settings = eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		new_checksum = crc16(settings, settings_length);
		settings_checksum = new_checksum;
		backup_settings_checksum = new_checksum;
		eeprom_write_settings();
		return;
	}
	if (settings_magic != 0x41323353u)
	{
		bookkeeping_reset(checksum);
		move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
		new_checksum = signature_checksum();
		settings_signature = new_checksum;
		settings = eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		new_checksum = crc16(settings, settings_length);
		settings_checksum = new_checksum;
		backup_settings_checksum = new_checksum;
		eeprom_write_settings();
		return;
	}
	new_checksum = signature_checksum();
	if (settings_signature != (uint16_t)new_checksum || settings_signature != (uint16_t)new_checksum)
	{
		settings_signature = new_checksum;
		bookkeeping_reset(new_checksum);
		settings = eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		new_checksum = crc16(settings, settings_length);
		settings = settings_checksum_ADDRESS;
		if (settings_checksum == (uint16_t)new_checksum)
		{
			write_count = SETTINGS_COPY(settings)->write_count;
			move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
			eeprom_write_count = write_count;
			backup_eeprom_write_count = write_count;
			new_checksum = signature_checksum();
			settings_signature = new_checksum;
			settings = eeprom_write_count_ADDRESS;
			settings_length = 0x3Au;
			new_checksum = crc16(settings, settings_length);
			settings_checksum = new_checksum;
			backup_settings_checksum = new_checksum;
			eeprom_write_settings();
			return;
		}
		settings = backup_eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		checksum = crc16(settings, settings_length);
		settings = backup_settings_checksum_ADDRESS;
		if (backup_settings_checksum != (uint16_t)checksum)
		{
			bookkeeping_reset(checksum);
			move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
			new_checksum = signature_checksum();
			settings_signature = new_checksum;
			settings = eeprom_write_count_ADDRESS;
			settings_length = 0x3Au;
			new_checksum = crc16(settings, settings_length);
			settings_checksum = new_checksum;
			backup_settings_checksum = new_checksum;
			eeprom_write_settings();
			return;
		}
		write_count = SETTINGS_COPY(settings)->write_count;
		move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
		eeprom_write_count = write_count;
		backup_eeprom_write_count = write_count;
		new_checksum = signature_checksum();
		settings_signature = new_checksum;
		settings = eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		new_checksum = crc16(settings, settings_length);
		settings_checksum = new_checksum;
		backup_settings_checksum = new_checksum;
		eeprom_write_settings();
		return;
	}
	settings = eeprom_write_count_ADDRESS;
	settings_length = 0x3Au;
	new_checksum = crc16(settings, settings_length);
	if (settings_checksum != (uint16_t)new_checksum)
	{
		settings = backup_eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		checksum = crc16(settings, settings_length);
		if (backup_settings_checksum == (uint16_t)checksum)
		{
			move_halfwords(settings_checksum_ADDRESS, backup_settings_checksum_ADDRESS, 0x1Eu);
			eeprom_write_settings();
			return;
		}
		bookkeeping_reset(checksum);
		move_halfwords(settings_magic_ADDRESS, default_settings_ADDRESS, 0x40u);
		new_checksum = signature_checksum();
		settings_signature = new_checksum;
		settings = eeprom_write_count_ADDRESS;
		settings_length = 0x3Au;
		new_checksum = crc16(settings, settings_length);
		settings_checksum = new_checksum;
		backup_settings_checksum = new_checksum;
		eeprom_write_settings();
		return;
	}
	settings = backup_eeprom_write_count_ADDRESS;
	settings_length = 0x3Au;
	new_checksum = crc16(settings, settings_length);
	if (settings_checksum != (uint16_t)new_checksum || backup_settings_checksum != (uint16_t)new_checksum)
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
	uint8_t command = 0;
	uint32_t lines = 0, lines_port = 0;

	command = 0u;
	eeprom_begin(&lines, &lines_port);
	eeprom_send_command(lines, command, lines_port);
	interrupts_on();
}

/* Unused: reads the EEPROM's word R4 (its read command, R4 | 80) into R0 and turns the interrupts back
   on. */
void eeprom_read_one_word(uint8_t command)
{
	uint32_t ignored = 0, lines = 0, lines_port = 0;

	command |= 0x80u;
	eeprom_begin(&lines, &lines_port);
	eeprom_read_word(command, lines_port, &ignored, &lines);
	interrupts_on();
}

/* Unused: reads the settings from the EEPROM again (eeprom_read_settings) and turns the interrupts back
   on. */
void reload_settings(uint32_t eeprom_word)
{
	eeprom_read_settings(eeprom_word);
	interrupts_on();
}

/* Reads the EEPROM's 64 words into 20E600-20E67F (settings_magic, settings_signature, the settings from
   settings_checksum on, their backup from backup_settings_checksum on), with the interrupts off. */
void eeprom_read_settings(uint32_t eeprom_word)
{
	uint8_t command = 0;
	uint32_t lines = 0, lines_port = 0, settings = 0, word_count = 0;

	settings = settings_magic_ADDRESS;
	command = 0x80u;
	word_count = 0x40u;
	eeprom_begin(&lines, &lines_port);
	do
	{
		eeprom_read_word(command, lines_port, &eeprom_word, &lines);
		command++;
		SETTINGS_WORDS(settings)[0] = eeprom_word;
		word_count--;
		settings += 2u;
	} while (word_count != 0u);
}

/* Unused: writes R0 into the EEPROM's word R4 (its write command, R4 | 40) without unlocking it first,
   and turns the interrupts back on. */
void eeprom_write_one_word(uint16_t eeprom_word, uint8_t command)
{
	uint32_t lines = 0, lines_port = 0;

	command |= 0x40u;
	eeprom_begin(&lines, &lines_port);
	eeprom_write_word(eeprom_word, lines, command, lines_port);
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
	uint8_t command = 0;
	uint16_t eeprom_word = 0;
	uint32_t checksum = 0, lines = 0, lines_port = 0, settings = 0, settings_length = 0, word_count = 0;

	settings = eeprom_write_count_ADDRESS;
	settings_length = 0x3Au;
	checksum = crc16(settings, settings_length);
	settings_checksum = checksum;
	move_halfwords(backup_settings_checksum_ADDRESS, settings_checksum_ADDRESS, 0x1Eu);
	command = 0x30u;
	eeprom_begin(&lines, &lines_port);
	eeprom_send_command(lines, command, lines_port);
	settings = settings_magic_ADDRESS;
	command = 0x40u;
	word_count = 0x40u;
	eeprom_begin(&lines, &lines_port);
	do
	{
		eeprom_word = SETTINGS_WORDS(settings)[0];
		settings += 2u;
		lines = eeprom_write_word(eeprom_word, lines, command, lines_port);
		command++;
		word_count--;
	} while (word_count != 0u);
}

/* Reads the EEPROM word of read command R4 (eeprom_send_command): 18 clock pulses collecting the data
   out (bit 7 of R12, port F of the second I/O chip), the 16-bit word into R0; then chip select off. */
void eeprom_read_word(uint8_t command, uint32_t lines_port, uint32_t *p_eeprom_word, uint32_t *p_lines)
{
	uint32_t eeprom_word = *p_eeprom_word;
	uint32_t lines = *p_lines;
	uint32_t pulse_count = 0;

	lines = eeprom_send_command(lines, command, lines_port);
	lines = (lines & 0x3Fu) | 0x20u;
	pulse_count = 0x12u;
	do
	{
		lines = eeprom_set_lines(lines, lines_port);
		eeprom_word = ((eeprom_word & ~0xFFu) | io_read8(r12)) * 2u;
		lines = (uint8_t)(lines ^ 0x40u);
		lines = eeprom_set_lines(lines, lines_port);
		lines = (uint8_t)(lines ^ 0x40u);
		pulse_count--;
	} while (pulse_count != 0u);
	lines = lines & 0x1Fu;
	lines = eeprom_set_lines(lines, lines_port);
	eeprom_word = eeprom_word >> 9;
	*p_eeprom_word = eeprom_word;
	*p_lines = (*p_lines & 0xFFFFFF00u) | (lines & 0xFFu);
}

/* Writes R0 into the EEPROM word of write command R4 (eeprom_send_command): its 16 bits high first, chip
   select dropped to start the write, then waits, clocking, until the data out (bit 7 of R12) says it is
   done. */
uint32_t eeprom_write_word(uint32_t eeprom_word, uint32_t lines, uint8_t command, uint32_t lines_port)
{
	uint8_t data_out = 0;
	uint16_t word_bits = 0;
	uint32_t pulse_count = 0;

	word_bits = (uint16_t)eeprom_word;
	lines = eeprom_send_command(lines, command, lines_port);
	word_bits = (uint16_t)(((uint32_t)word_bits << 8) | (word_bits >> 8));
	pulse_count = 0x10u;
	do
	{
		eeprom_word = (eeprom_word & ~0xFFu) | (uint8_t)word_bits;
		eeprom_word = (eeprom_word & ~0xFFu) | (eeprom_word & 0x80u);
		word_bits = (uint16_t)(((uint32_t)word_bits << 1) | (word_bits >> 15));
		lines = ((lines & ~0xFFu) | (lines & 0x1Fu)) + 0x20u + eeprom_word;
		lines = eeprom_set_lines(lines, lines_port);
		lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
		lines = eeprom_set_lines(lines, lines_port);
		pulse_count--;
	} while (pulse_count != 0u);
	pulse_count = 2u;
	do
	{
		lines = (lines & ~0xFFu) | (lines & 0x1Fu);
		lines = eeprom_set_lines(lines, lines_port);
		lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
		lines = eeprom_set_lines(lines, lines_port);
		pulse_count--;
	} while (pulse_count != 0u);
	lines = (lines & ~0xFFu) | (lines & 0x1Fu);
	do
	{
		lines = (lines & ~0xFFu) | (uint8_t)(lines | 0x20u);
		lines = eeprom_set_lines(lines, lines_port);
		data_out = io_read8(r12);
		lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
		lines = eeprom_set_lines(lines, lines_port);
	} while ((int8_t)data_out >= 0);
	lines = (lines & ~0xFFu) | (lines & 0x1Fu);
	lines = eeprom_set_lines(lines, lines_port);
	return lines;
}

/* Sends command R4 to the EEPROM (a 93C46 on port H of the second I/O chip, R13: bit 7 data in, 6 clock,
   5 chip select): chip select, a start bit, then the 8 bits of R4 high first (two of opcode: 2 read, 1
   write, 0 with the address's top bits 3 write enable or 0 write disable; six of address), each with a
   clock pulse. */
uint32_t eeprom_send_command(uint32_t lines, uint8_t command, uint32_t lines_port)
{
	uint8_t command_bits = 0, sent_bits = 0;
	uint32_t pulse_count = 0;

	command_bits = (uint8_t)command;
	lines = (lines & ~0xFFu) | (lines & 0x1Fu);
	lines = eeprom_set_lines(lines, lines_port);
	lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
	lines = eeprom_set_lines(lines, lines_port);
	lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x60u);
	lines = eeprom_set_lines(lines, lines_port);
	lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
	lines = eeprom_set_lines(lines, lines_port);
	lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0xC0u);
	lines = eeprom_set_lines(lines, lines_port);
	lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
	lines = eeprom_set_lines(lines, lines_port);
	pulse_count = 8u;
	do
	{
		sent_bits = (uint8_t)command_bits;
		command_bits = (uint8_t)(command_bits * 2u);
		lines = (lines & ~0xFFu) | (lines & 0x3Fu);
		lines = (lines & ~0xFFu) | (uint8_t)(lines | (sent_bits & 0x80u));
		lines = eeprom_set_lines(lines, lines_port);
		lines = (lines & ~0xFFu) | (uint8_t)(lines ^ 0x40u);
		lines = eeprom_set_lines(lines, lines_port);
		pulse_count--;
	} while (pulse_count != 0u);
	return lines;
}

/* Starts an EEPROM access: the interrupts off, R13 the EEPROM's lines (C8000E, port H of the second I/O
   chip), R12 its data out (C8000A, port F, bit 7), R2 the lines' present value. */
void eeprom_begin(uint32_t *p_lines, uint32_t *p_lines_port)
{
	uint8_t lines_now = 0;

	*p_lines_port = IO_2_PORT_H;
	r12 = IO_2_PORT_F;
	lines_now = io_read8(IO_2_PORT_H);
	interrupts_off();
	*p_lines = (*p_lines & 0xFFFFFF00u) | lines_now;
}

/* Writes R2 to the EEPROM's lines (R13), with bit 4 set too. */
uint32_t eeprom_set_lines(uint32_t lines, uint32_t lines_port)
{
	lines = (lines & ~0xFFu) | (uint8_t)(lines | 0x10u);
	io_write8(lines_port, lines);
	return lines;
}
