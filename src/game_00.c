/**
 *    Name:        game_00.c
 *    Description: the game's functions, part 00 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The reset's code (000006): SBR to the vector table, the stack 21FC00, 16 bytes from 0000A4 to E00000
   and 4 from 0000B4 (boot_e80000_bytes) to E80000 (which the board does not map), the interrupt
   controller's registers from 0000B8, privilege level 0 with the stack 21F400 and a delay; then clears
   21E000-21E3FF, turns the link board off (801000), clears 21F000-21FFFF, sets the I/O chips up
   (init_io_chips), clears the game's variables (20E000-20FFFF), writes the second task's first interrupt
   frame (000C98 to 21FF84: its PC 0006B0) and its stack pointers (waiting_task_frame,
   waiting_task_stack), initialise_game, lcd_start (the LCD), the interrupts on, the link's check
   (network_check), and goes on as the boot task's frame loop (second_task(1), screen 2's). */
void boot(void)
{
	uint32_t block_count = 0, clear_address = 0, delay_count = 0, destination = 0, fill = 0, fill_high = 0, link_off_delay = 0, pair_count = 0, source = 0;
	uint32_t psw_image = 0;
	uint32_t psw = 0;

	load_privileged(5u, vector_table_ADDRESS);
	source = 0xA4u;
	destination = 0xE00000u;
	pair_count = 8u;
	do
	{
		mem_write8(destination, mem_read8(source));
		source++;
		destination++;
		mem_write8(destination, mem_read8(source));
		pair_count--;
		source++;
		destination++;
	} while (pair_count != 0u);
	mem_write8(0xE80000u, (uint8_t)boot_e80000_bytes);
	mem_write8(0xE80001u, (uint8_t)(boot_e80000_bytes >> 8u));
	mem_write8(0xE80002u, (uint8_t)(boot_e80000_bytes >> 16u));
	mem_write8(0xE80003u, (uint8_t)(boot_e80000_bytes >> 24u));
	destination = INTERRUPT_CONTROL;
	pair_count = 6u;
	source = 0xB8u;
	do
	{
		mem_write8(destination, mem_read8(source));
		source++;
		destination++;
		mem_write8(destination, mem_read8(source));
		pair_count--;
		source++;
		destination++;
	} while (pair_count != 0u);
	psw_image = 0u;
	psw = psw_image;
	psw_write(psw);
	load_privileged(2u, 0x210000u);
	load_privileged(3u, 0x210000u);
	load_privileged(4u, 0x210000u);
	delay_count = 0x10000u;
	do
	{
		delay_count--;
	} while (delay_count != 0u);
	fill = 0u;
	fill_high = 0u;
	block_count = 0x20u;
	clear_address = 0x21E000u;
	do
	{
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		block_count--;
	} while (block_count != 0u);
	mem_write32(0x801000u, 0u);
	link_off_delay = BOARD_OFF_LOOPS;
	do
	{
		link_off_delay--;
	} while (link_off_delay != 0u);
	fill = 0u;
	fill_high = 0u;
	clear_address = 0x21F000u;
	block_count = 0x80u;
	do
	{
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		block_count--;
	} while (block_count != 0u);
	init_io_chips();
	fill = 0u;
	fill_high = 0u;
	block_count = 0x100u;
	clear_address = GAME_VARIABLES;
	do
	{
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		mem_write32(clear_address, fill);
		mem_write32(clear_address + 4u, fill_high);
		clear_address += 8u;
		block_count--;
	} while (block_count != 0u);
	move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
	player_car = 0x219000u;
	waiting_task_frame = 0x21FF84u;
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
uint32_t crc16(uint32_t bytes, uint32_t byte_count)
{
	uint32_t carry = 0;
	uint32_t bit_count = 0, crc = 0, polynomial = 0;

	polynomial = 0x10210000u;
	crc = 0xDEBDEB00u;
	do
	{
		crc = (crc & ~0xFFu) | mem_read8(bytes);
		bit_count = 8u;
		bytes++;
		do
		{
			carry = crc * 2u < crc;
			crc = crc * 2u;
			if (carry)
				crc ^= polynomial;
			bit_count--;
		} while (bit_count != 0u);
		byte_count--;
	} while (byte_count != 0u);
	bit_count = 8u;
	do
	{
		carry = crc * 2u < crc;
		crc = crc * 2u;
		if (carry)
			crc ^= polynomial;
		bit_count--;
	} while (bit_count != 0u);
	crc = crc >> 16;
	return crc;
}

/* Returns the crc16 of the program's 48-byte signature text at 00010C (OUTRUNNERS EUROPA and its build
   date, 93.4.21), which the EEPROM keeps in settings_signature so that a new program is noticed
   (load_settings). */
uint32_t signature_checksum(void)
{
	uint32_t signature_length = 0, signature_text = 0;

	signature_text = 0x10Cu;
	signature_length = 0x30u;
	return crc16(signature_text, signature_length);
}

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
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
			write_count = mem_read16(settings + 2u);
			move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
			move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
		write_count = mem_read16(settings + 2u);
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
		move_halfwords(settings_magic_ADDRESS, 0x4C0u, 0x40u);
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
		mem_write16(settings, eeprom_word);
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
	move_string(settings_checksum_ADDRESS, 0x1Eu, backup_settings_checksum_ADDRESS, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	command = 0x30u;
	eeprom_begin(&lines, &lines_port);
	eeprom_send_command(lines, command, lines_port);
	settings = settings_magic_ADDRESS;
	command = 0x40u;
	word_count = 0x40u;
	eeprom_begin(&lines, &lines_port);
	do
	{
		eeprom_word = mem_read16(settings);
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
		eeprom_word = ((eeprom_word & ~0xFFu) | mem_read8(r12)) * 2u;
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
		data_out = mem_read8(r12);
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

	*p_lines_port = 0xC8000Eu;
	r12 = 0xC8000Au;
	lines_now = mem_read8(0xC8000Eu);
	interrupts_off();
	*p_lines = (*p_lines & 0xFFFFFF00u) | lines_now;
}

/* Writes R2 to the EEPROM's lines (R13), with bit 4 set too. */
uint32_t eeprom_set_lines(uint32_t lines, uint32_t lines_port)
{
	lines = (lines & ~0xFFu) | (uint8_t)(lines | 0x10u);
	mem_write8(lines_port, lines);
	return lines;
}

/* The two tasks' frame loops, one function in the C: second_task(0) is the second task (from 0006B0),
   which runs screen 1; second_task(1) the boot task after boot (from 000637), which first sets up the
   link's race (link_take_master_settings), the interrupt controller's timer (D00008, D0000A),
   task_swapping, each screen's first attract car (screen next_attract_car from first_attract_car_lists
   by machine_count and the screen's car number) and the screens on (C0001C, C8001C), then runs screen 2.
   A frame of either: wait_for_frame (the VBLANK swaps the tasks meanwhile; the second task calls
   link_send_records before it with second_task_waiting set, and the boot task calls link_send_records
   and link_receive_records only while that is clear), the test mode when test_mode_on is set,
   link_receive_records, race_entries_update, race_progress_update and course_records_update (the link
   and the race records), game_frame_count + 1 (screen 1's loop), take_screen_inputs, check_game_start,
   the game mode's code (game_mode_jumps by game_mode: game_mode_power_on, game_mode_restart,
   game_mode_title, game_mode_car_presentation, game_mode_demo_race, game_mode_ranking, game_mode_play,
   entry_invitation_step (the entry countdown on a seat without a player), game_mode_isdn_connection),
   run_objects and lcd_update; with single_task set it goes on with the other screen's frame instead of
   waiting. The test mode: task_swapping off, test_mode_start, then test_mode_frame each frame while
   test_mode_on lasts; then the screens off, the variables cleared, the second task's frame written
   again, initialise_game, lcd_reset, both screens in game mode 1 with a fade in (fade 1),
   network_check_again, and the boot task's part again. */
void second_task(uint32_t entry_point)
{
	bool L_000637 = false, L_000719 = false, L_00077F = false;
	uint32_t attract_car_list = 0, block_count = 0, clear_address = 0, fill = 0, fill_high = 0, input_changes = 0, mode = 0;
	uint32_t jump_target = 0;

	for (;;)
	{
		for (;;)
		{
			jump_target = L_000719 ? 1u : L_000637 ? 1u : entry_point;
			switch (jump_target)
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
					screen_2_vblanks_high = 0u;
					screen_2_vblanks = 0u;
					screen_1_vblanks_high = 0u;
					screen_1_vblanks = 0u;
					vblanks_since_start = 0u;
					task_swapping = 0xFFu;
					attract_car_list = first_attract_car_lists[machine_count];
					input_changes = SCREEN(SCREEN_2)->car_number;
					SCREEN(SCREEN_1)->next_attract_car = FIRST_ATTRACT_CARS(attract_car_list)[SCREEN(SCREEN_1)->car_number];
					SCREEN(SCREEN_2)->next_attract_car = FIRST_ATTRACT_CARS(attract_car_list)[input_changes];
					mem_write8(IO_C0001C, 6u);
					mem_write8(0xC8001Cu, 0x86u);
				}
				do
				{
					L_000719 = false;
					if (second_task_waiting == 0u)
						link_send_records();
					input_changes = wait_for_frame(input_changes);
					L_00077F = test_mode_on != 0u;
					if (L_00077F)
						break;
					if (second_task_waiting == 0u)
						link_receive_records();
					race_entries_update();
					race_progress_update();
					course_records_update();
					screen = SCREEN_2;
					input_changes = take_screen_inputs();
					mode = SCREEN(screen)->game_mode;
					player_car = screen + 0x1000u;
					mode = check_game_start(mode, input_changes);
					switch (mode)
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
						invalid_jump(game_mode_jumps[mode]);
					}
					run_objects();
					lcd_update();
				} while (single_task == 0u);
				if (L_00077F)
					break;
				break;
			default:
				invalid_jump(jump_target);
			}
			if (L_00077F)
				break;
			do
			{
				second_task_waiting = 0xFFu;
				link_send_records();
				input_changes = wait_for_frame(input_changes);
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
				input_changes = take_screen_inputs();
				mode = SCREEN(screen)->game_mode;
				player_car = screen + 0x1000u;
				mode = check_game_start(mode, input_changes);
				switch (mode)
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
					invalid_jump(game_mode_jumps[mode]);
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
		wait_for_frame(input_changes);
		test_mode_start();
		do
		{
			wait_for_frame(input_changes);
			test_mode_frame();
		} while (test_mode_on != 0u);
		interrupts_off();
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		fill = 0u;
		fill_high = 0u;
		block_count = 0x100u;
		clear_address = GAME_VARIABLES;
		do
		{
			mem_write32(clear_address, fill);
			mem_write32(clear_address + 4u, fill_high);
			clear_address += 8u;
			mem_write32(clear_address, fill);
			mem_write32(clear_address + 4u, fill_high);
			clear_address += 8u;
			mem_write32(clear_address, fill);
			mem_write32(clear_address + 4u, fill_high);
			clear_address += 8u;
			mem_write32(clear_address, fill);
			mem_write32(clear_address + 4u, fill_high);
			clear_address += 8u;
			block_count--;
		} while (block_count != 0u);
		move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
		player_car = 0x219000u;
		waiting_task_frame = 0x21FF84u;
		waiting_task_stack = 0x21F800u;
		initialise_game();
		lcd_reset();
		SCREEN(SCREEN_1)->game_mode = 1u;
		SCREEN(SCREEN_1)->mode_step = 0u;
		SCREEN(SCREEN_1)->fade_kind = 1u;
		SCREEN(SCREEN_1)->flags |= 1u;
		SCREEN(SCREEN_2)->game_mode = 1u;
		SCREEN(SCREEN_2)->mode_step = 0u;
		SCREEN(SCREEN_2)->fade_kind = 1u;
		SCREEN(SCREEN_2)->flags |= 1u;
		interrupts_on();
		network_check_again();
		L_000637 = true;
	}
}

/* Each frame before the game mode's code: in the attract modes (not 0, not 6), with a credit on the seat
   (seat1_credits, seat2_credits for screen 2) or free play (free_play), blinks the START lamp (bit 2 of
   the seat's port_d_outputs, 8 frames on and 8 off by start_lamp_blink); START pressed (buttons_pressed
   bit 6) takes the credit (not with free play; and clears the bytes at 20E0A2 and 20E0AA, after each
   seat's coin units), turns the lamp off and starts a game: game mode 6 at step 0, link_event 1, the
   race record's car part cleared (race_record_clear) and race entry_request = FF. Returns the game mode. */
uint32_t check_game_start(uint32_t mode, uint32_t seat_credits)
{
	uint8_t lamp_blink = 0;
	uint32_t index = 0, play_mode = 0;
	uint32_t seat_outputs = 0;

	if (mode == 0u || (uint8_t)mode == 6u)
		return mode;
	index = SCREEN(screen)->index;
	if (free_play != 0u)
	{
		lamp_blink = (uint8_t)(SCREEN(screen)->start_lamp_blink + 1u);
		SCREEN(screen)->start_lamp_blink = lamp_blink;
		port_d_outputs[index] = (port_d_outputs[index] & ~4u) | ((lamp_blink & 8u) >> 1);
		if ((SCREEN(screen)->buttons_pressed & 0x40u) == 0u)
			return mode;
		if (free_play == 0u)
		{
			mem_write8(seat_credits, mem_read8(seat_credits) - 1u);
			mem_write8(seat_credits + 2u, 0u);
		}
		SCREEN(screen)->start_lamp_blink = 0u;
		seat_outputs = port_d_outputs_ADDRESS + index;
		mem_write8(seat_outputs, mem_read8(seat_outputs) & ~4u);
		play_mode = 6u;
		AS32(SCREEN(screen)->game_mode) = 6u;
		SCREEN(screen)->link_event = 1u;
		race_record_clear();
		RACE(SCREEN(screen)->race)->entry_request = 0xFFu;
		return play_mode;
	}
	seat_credits = seat_credits_addresses[index];
	if (mem_read8(seat_credits) == 0u)
		return mode;
	lamp_blink = (uint8_t)(SCREEN(screen)->start_lamp_blink + 1u);
	SCREEN(screen)->start_lamp_blink = lamp_blink;
	port_d_outputs[index] = (port_d_outputs[index] & ~4u) | ((lamp_blink & 8u) >> 1);
	if ((SCREEN(screen)->buttons_pressed & 0x40u) == 0u)
		return mode;
	if (free_play == 0u)
	{
		mem_write8(seat_credits, mem_read8(seat_credits) - 1u);
		mem_write8(seat_credits + 2u, 0u);
	}
	SCREEN(screen)->start_lamp_blink = 0u;
	seat_outputs = port_d_outputs_ADDRESS + index;
	mem_write8(seat_outputs, mem_read8(seat_outputs) & ~4u);
	play_mode = 6u;
	AS32(SCREEN(screen)->game_mode) = 6u;
	SCREEN(screen)->link_event = 1u;
	race_record_clear();
	RACE(SCREEN(screen)->race)->entry_request = 0xFFu;
	return play_mode;
}

/* Waits for the next frame: polls frame_ready (set by the VBLANK, which also swaps the tasks) and clears
   it; while the screen has tile pages to unpack (screen unpack_state) it unpacks them a piece at a time
   (unpack_queued_step) between the polls. */
uint32_t wait_for_frame(uint32_t copy_source)
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
		copy_source = unpack_queued_step(copy_source);
		if (frame_ready != 0u)
		{
			frame_ready = 0u;
			break;
		}
	}
	return copy_source;
}

/* Sets the game up, at start-up and after the test mode: 13, 0, 0, 1F to C00074 (which the board does
   not map); for screen 2, then 1, its objects' page cleared (clear_screen_ram) and its constants
   (init_screen: index, its video registers, the view's, the courses' stage lengths at 700);
   initialise_board; then initialise_screen for screens 1 and 2. */
void initialise_game(void)
{
	uint32_t checksum = 0;

	mem_write8(IO_C00074, 0x13u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0x1Fu);
	screen = SCREEN_2;
	clear_screen_ram();
	init_screen();
	screen = SCREEN_1;
	clear_screen_ram();
	checksum = init_screen();
	initialise_board(checksum);
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
void initialise_board(uint32_t checksum)
{
	uint32_t tile_page_list = 0;

	init_io_chips();
	load_settings(checksum);
	init_sprite_chip();
	load_sprite_ram_tables();
	init_video();
	bookkeeping_check();
	init_coins();
	start_sound_program();
	tile_page_list = boot_tile_pages_5d_ADDRESS;
	unpack_tile_pages(tile_page_list);
	tile_page_list = boot_tile_pages_5e_ADDRESS;
	unpack_tile_pages(tile_page_list);
	tile_page_list = boot_tile_pages_29_to_4e_ADDRESS;
	unpack_tile_pages(tile_page_list);
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
	uint8_t chip_1_port_f_on = 0, chip_2_port_f_on = 0, chip_2_switches = 0, new_switches = 0, old_switches = 0, test_switch = 0;
	uint16_t calibration_high = 0, calibration_low = 0, reading = 0;
	uint32_t calibration = 0, converter = 0, inputs_left = 0, raw_place = 0, scaled_place = 0, screen_turn = 0;
	uint8_t chip_1_port_e = 0, chip_1_port_f = 0, chip_2_port_e = 0, chip_2_port_f = 0;
	uint16_t capped_reading = 0;

	chip_1_port_e = mem_read8(IO_C00008);
	chip_1_port_f = mem_read8(IO_C0000A);
	chip_1_port_f_on = (uint8_t)~chip_1_port_f;
	new_switches = (uint8_t)(~chip_1_port_e | ((((uint32_t)chip_1_port_f_on << 4) | (chip_1_port_f_on >> 4)) & 3u));
	chip_2_port_e = mem_read8(0xC80008u);
	chip_2_port_f = mem_read8(0xC8000Au);
	chip_2_port_f_on = (uint8_t)~chip_2_port_f;
	chip_2_switches = (uint8_t)(~chip_2_port_e | ((((uint32_t)chip_2_port_f_on << 4) | (chip_2_port_f_on >> 4)) & 3u));
	test_switch = (new_switches | chip_2_switches) & 2u;
	new_switches = (uint8_t)((new_switches & 0xDu) | ((((chip_2_switches & 0xCu) * 2u) | (chip_2_switches & 1u)) * 2u) | ((uint32_t)test_switch << 6) | (test_switch >> 2));
	old_switches = switches;
	switches = new_switches;
	previous_switches = old_switches;
	switches_pressed = ~old_switches & new_switches;
	switches_released = old_switches & ~new_switches;
	screen_turn = SCREEN(screen)->index;
	screen_turn = screen_turn & 1u;
	converter = IO_PORTS + 0x50u + screen_turn * 4u;
	raw_place = analog_raw_ADDRESS + screen_turn * 4u;
	scaled_place = analog_inputs_ADDRESS + screen_turn * 4u;
	inputs_left = analog_input_counts[screen_turn];
	calibration = analog_calibration_addresses[screen_turn];
	do
	{
		mem_write8(converter, 0xFFu);
		reading = (((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)(mem_read8(converter) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) * 2u) & ~0xFFu) | mem_read8(converter)) >> 7;
		mem_write8(raw_place, (uint8_t)reading);
		calibration_low = mem_read8(calibration);
		calibration++;
		calibration_high = mem_read8(calibration);
		raw_place++;
		calibration++;
		if (calibration_high < reading)
			reading = (uint16_t)calibration_high;
		capped_reading = reading;
		reading = (uint16_t)(capped_reading - calibration_low);
		if ((int16_t)capped_reading < (int16_t)calibration_low)
			reading = 0u;
		mem_write8(scaled_place, divide_u16((uint16_t)(reading * 0xFFu), (uint16_t)(calibration_high - calibration_low)));
		inputs_left--;
		scaled_place++;
		converter += 2u;
	} while (inputs_left != 0u);
	mem_write8(IO_C00060, screen_turn ^ 1u);
	return calibration;
}

/* Packs both seats' inputs for the coming frame (the VBLANK, on screen 1's turn) into their records: the
   screen's player + 1, the buttons (port A's high bits into 0-3, its low two into 4-5, the shift; port E
   bit 4 into bit 6, START), the wheel (analog_inputs inverted), the gas and the brake: seat_1_player to
   seat_1_brake and seat_2_player to seat_2_brake. On ISDN (isdn_mode) they go to the link board instead
   (link_send_seat_inputs). */
void pack_seat_inputs(void)
{
	uint8_t other_seat_buttons = 0, port_a_on = 0, seat_buttons = 0;
	uint32_t analog_place = 0, link_event_place = 0, link_place = 0;
	uint8_t chip_1_port_e = 0, chip_1_port_a = 0, chip_2_port_e = 0, chip_2_port_a = 0;

	chip_1_port_e = mem_read8(IO_C00008);
	chip_1_port_a = mem_read8(IO_C00000);
	port_a_on = (uint8_t)~chip_1_port_a;
	seat_buttons = ((~chip_1_port_e * 2u * 2u) & 0x40u) | ((((uint32_t)port_a_on << 4) | (port_a_on >> 4)) & 0x3Fu);
	chip_2_port_e = mem_read8(0xC80008u);
	chip_2_port_a = mem_read8(0xC80000u);
	port_a_on = (uint8_t)~chip_2_port_a;
	other_seat_buttons = ((~chip_2_port_e * 2u * 2u) & 0x40u) | ((((uint32_t)port_a_on << 4) | (port_a_on >> 4)) & 0x3Fu);
	analog_place = analog_inputs_ADDRESS;
	if (isdn_mode != 0u)
	{
		link_place = 0x800020u;
		link_event_place = 0x208040u;
		link_send_seat_inputs(seat_buttons, link_event_place, &analog_place, &link_place);
		link_event_place = 0x218040u;
		seat_buttons = (uint8_t)other_seat_buttons;
		link_send_seat_inputs(seat_buttons, link_event_place, &analog_place, &link_place);
		return;
	}
	seat_1_player = SCREEN(SCREEN_1)->player + 1u;
	seat_1_buttons = seat_buttons;
	seat_1_wheel = ~analog_inputs;
	analog_place++;
	seat_1_gas = analog_gas_1;
	analog_place++;
	seat_1_brake = analog_brake_1;
	seat_2_player = SCREEN(SCREEN_2)->player + 1u;
	seat_2_buttons = other_seat_buttons;
	analog_place++;
	seat_2_wheel = ~analog_wheel_2;
	analog_place++;
	seat_2_gas = analog_gas_2;
	seat_2_brake = analog_brake_2;
}

/* Never runs in the scenarios (ISDN only): writes a seat's five bytes as halfwords on the link board at
   R11 (800020 on): 0, the buttons (R0), the wheel (inverted), gas and brake from analog_inputs at R10;
   or, when its screen has a link_event (R2: screen + 40), the event and the four bytes after it, which
   it then clears (without moving R10 on, so the next seat would read this one's analog inputs). */
void link_send_seat_inputs(uint8_t seat_buttons, uint32_t link_event_place, uint32_t *p_analog_place, uint32_t *p_link_place)
{
	uint32_t analog_place = *p_analog_place;
	uint32_t link_place = *p_link_place;
	uint8_t wheel_input = 0, gas_input = 0, brake_input = 0;

	if (SCREEN(link_event_place - 0x40u)->link_event != 0u)
	{
		mem_write16(link_place, SCREEN(link_event_place - 0x40u)->link_event);
		link_place += 2u;
		link_event_place++;
		mem_write16(link_place, SCREEN(link_event_place - 0x41u)->link_event_data_1);
		link_place += 2u;
		link_event_place++;
		mem_write16(link_place, SCREEN(link_event_place - 0x42u)->link_event_data_2);
		link_place += 2u;
		link_event_place++;
		mem_write16(link_place, SCREEN(link_event_place - 0x43u)->link_event_data_3);
		link_place += 2u;
		link_event_place++;
		mem_write16(link_place, SCREEN(link_event_place - 0x44u)->link_event_data_4);
		link_place += 2u;
		link_event_place = link_event_place + 1u - 4u;
		SCREEN(link_event_place - 0x41u)->link_event_data_1 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_2 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_3 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_4 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event = 0u;
	}
	else
	{
		mem_write16(link_place, 0u);
		link_place += 2u;
		mem_write16(link_place, (uint8_t)seat_buttons);
		wheel_input = mem_read8(analog_place);
		analog_place++;
		link_place += 2u;
		mem_write16(link_place, (uint8_t)~wheel_input);
		gas_input = mem_read8(analog_place);
		analog_place++;
		link_place += 2u;
		mem_write16(link_place, gas_input);
		brake_input = mem_read8(analog_place);
		analog_place++;
		link_place += 2u;
		mem_write16(link_place, brake_input);
		link_place += 2u;
	}
	*p_analog_place = analog_place;
	*p_link_place = link_place;
}

/* queue_screen_inputs for screen 1 (seat_1_buttons, the first I/O chip) and screen 2 (seat_2_buttons,
   the second): the VBLANK on screen 2's turn. */
uint32_t queue_seat_inputs(void)
{
	uint32_t seat_record = 0, seat_screen = 0;

	seat_screen = SCREEN_1;
	seat_record = seat_1_buttons_ADDRESS;
	r12 = 0xC00000u;
	queue_screen_inputs(seat_screen, seat_record);
	seat_screen = SCREEN_2;
	seat_record = seat_2_buttons_ADDRESS;
	r12 = 0xC80000u;
	return queue_screen_inputs(seat_screen, seat_record);
}

/* Puts a screen's inputs into its ring of eight (screen + 130, at input_write_place): the seat's buttons
   (R11) with ~port B of its I/O chip (R12 + 2) in bits 8-15; and the record's wheel, gas and brake into
   the screen's wheel, gas and brake. On ISDN the record is the screen's race_copy's +2C (the inputs the
   link brought). */
uint32_t queue_screen_inputs(uint32_t seat_screen, uint32_t seat_record)
{
	uint32_t input_write_place = 0, inputs = 0, seat_inputs = 0;

	if (isdn_mode != 0u)
		seat_record = SCREEN(seat_screen)->race_copy + 0x2Cu;
	inputs = (uint8_t)~mem_read8(r12 + 2u);
	seat_inputs = mem_read32(seat_record);
	inputs = (uint16_t)(inputs << 8) | (uint8_t)seat_inputs;
	input_write_place = SCREEN(seat_screen)->input_write_place;
	mem_write32(seat_screen + 0x130u + input_write_place * 4u, inputs);
	SCREEN(seat_screen)->input_write_place = (input_write_place + 1u) & 7u;
	AS32(SCREEN(seat_screen)->wheel) = seat_inputs >> 8;
	return inputs;
}

/* Takes the screen's inputs for the frame from its ring (input_read_place; both frame loops, before
   check_game_start): buttons, with previous_buttons, buttons_pressed and buttons_released (bit 6 START,
   4-5 the shift up and down, 0-3 the others, 8-15 port B). On ISDN also the link's status (race_copy
   link_status) into link_status, with link_status_set, link_status_cleared and link_status_changed, and,
   unless in it already, game mode 8 (game_mode_isdn_connection) while it says the line connects (bits
   2-3). */
uint32_t take_screen_inputs(void)
{
	uint8_t new_link_status = 0, old_link_status = 0, status_cleared = 0;
	uint32_t buttons = 0, buttons_place = 0, input_changes = 0, input_read_place = 0, inputs = 0;
	uint8_t new_status = 0;

	input_read_place = SCREEN(screen)->input_read_place;
	inputs = mem_read32(screen + 0x130u + input_read_place * 4u);
	SCREEN(screen)->input_read_place = (input_read_place + 1u) & 7u;
	buttons_place = screen + 0x110u;
	buttons = SCREEN(buttons_place - 0x110u)->buttons;
	SCREEN(buttons_place - 0x110u)->buttons = inputs;
	buttons_place += 4u;
	SCREEN(buttons_place - 0x114u)->previous_buttons = buttons;
	input_changes = ~buttons & inputs;
	buttons_place += 4u;
	SCREEN(buttons_place - 0x118u)->buttons_pressed = input_changes;
	SCREEN(buttons_place - 0x118u)->buttons_released = buttons & ~inputs;
	if (isdn_mode != 0u)
	{
		new_link_status = RACE(SCREEN(screen)->race_copy)->link_status;
		new_status = new_link_status;
		old_link_status = SCREEN(screen)->link_status;
		SCREEN(screen)->link_status = new_status;
		input_changes = (input_changes & ~0xFFu) | (uint8_t)~old_link_status;
		input_changes = (input_changes & ~0xFFu) | (input_changes & new_link_status);
		SCREEN(screen)->link_status_set = input_changes;
		status_cleared = ~new_link_status & old_link_status;
		SCREEN(screen)->link_status_cleared = status_cleared;
		input_changes = (input_changes & ~0xFFu) | (uint8_t)(input_changes | status_cleared);
		SCREEN(screen)->link_status_changed = input_changes;
		if (SCREEN(screen)->game_mode != 8u && (new_link_status & 0xCu) != 0u)
			AS32(SCREEN(screen)->game_mode) = 8u;
	}
	return input_changes;
}

/* Never runs in the scenarios (ISDN only): the players on this side and on the other (race_copy
   players_this_side, unk_2A) into isdn_players_this_side and isdn_players_other_side, their sum into
   player_count (the race's players) and half of it into machine_count (its cabinets). */
void isdn_count_players(void)
{
	uint8_t player_sum = 0, players_this_side = 0;

	players_this_side = RACE(SCREEN(screen)->race_copy)->players_this_side;
	player_sum = RACE(SCREEN(screen)->race_copy)->players_other_side;
	isdn_players_this_side = players_this_side;
	isdn_players_other_side = player_sum;
	player_sum += players_this_side;
	player_count = player_sum;
	machine_count = player_sum >> 1;
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
	uint32_t colour_block = 0, handler_list = 0, lamp_place = 0, link_status = 0;
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
		colour_block = black_background_palette_ADDRESS;
		queue_palette(colour_block);
		lamp_place = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		mem_write32(lamp_place + 0xC0u, 0u);
		mem_write32(lamp_place + 0xC8u, 0u);
		clear_lcd();
		handler_list = 0xF70u;
		start_object_list(handler_list);
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
		SCREEN(screen)->mode_timer = (SCREEN(screen)->mode_timer & 0xFF00u) | (mode_timer - 1u & 0xFFu);
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
		invalid_jump(isdn_connection_step_jumps[SCREEN(screen)->mode_step]);
	}
	link_status = SCREEN(screen)->link_status;
	if ((link_status & 1u) != 0u)
	{
		isdn_count_players();
		SCREEN(screen)->mode_step++;
		SCREEN(screen)->mode_timer = (SCREEN(screen)->mode_timer & 0xFF00u) | 0x96u;
		return;
	}
	if ((link_status & 0xCu) == 0u)
	{
		SCREEN(screen)->mode_step++;
		SCREEN(screen)->mode_timer = (SCREEN(screen)->mode_timer & 0xFF00u) | 0x96u;
		return;
	}
}

/* Never runs in the scenarios: the ISDN connection screen's texts (game_mode_isdn_connection starts it
   from the list at 000F70), on the text layer (print_text_layer), by state: 0 by link_status: bit 0 ISDN
   SUCCESS, THIS SIDE, OTHER SIDE and TOTAL with the players (race_copy players_this_side, unk_2A and
   their sum, print_hex_byte), then 1; bits 2-3 clear ISDN FAILED, then 1; else ISDN CONECTING (sic)
   blinking every 8 frames (its unk_10 counting); 1 nothing more. */
void isdn_status_update(void)
{
	uint8_t link_status = 0, other_side_count = 0, players_other_side = 0, players_this_side = 0, this_side_count = 0, total_players = 0;
	uint16_t cell = 0;
	uint32_t blink_frames = 0, cell_place = 0, text_record = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		return;
	default:
		invalid_jump(isdn_status_state_jumps[OBJECT(current_object)->state]);
	}
	link_status = SCREEN(screen)->link_status;
	if ((link_status & 1u) == 0u)
	{
		if ((link_status & 0xCu) == 0u)
		{
			OBJECT(current_object)->state++;
			text_record = isdn_failed_text;
			text_record = print_text_layer(text_record);
			return;
		}
		blink_frames = OBJECT(current_object)->isdn_status.blink_counter;
		blink_frames = (uint8_t)(blink_frames + 1u);
		OBJECT(current_object)->isdn_status.blink_counter = blink_frames;
		blink_frames = blink_frames >> 3;
		text_record = isdn_connecting_texts[blink_frames & 1u];
		text_record = print_text_layer(text_record);
		return;
	}
	OBJECT(current_object)->state++;
	text_record = isdn_success_text;
	print_text_layer(text_record);
	text_record = isdn_this_side_text;
	print_text_layer(text_record);
	text_record = isdn_other_side_text;
	print_text_layer(text_record);
	text_record = isdn_total_text;
	print_text_layer(text_record);
	cell = 0x6000u;
	players_this_side = RACE(SCREEN(screen)->race_copy)->players_this_side;
	this_side_count = (uint8_t)players_this_side;
	cell_place = TEXT_CELL(24, 12);
	print_hex_byte(players_this_side, cell, cell_place);
	players_other_side = RACE(SCREEN(screen)->race_copy)->players_other_side;
	other_side_count = (uint8_t)players_other_side;
	cell_place = TEXT_CELL(24, 13);
	print_hex_byte(players_other_side, cell, cell_place);
	total_players = (uint8_t)((uint32_t)this_side_count + other_side_count);
	cell_place = TEXT_CELL(24, 14);
	print_hex_byte(total_players, cell, cell_place);
}

/* Game mode 2, the title (title_screen_update), by mode_step: 0 the screen cleared (as clear_screen),
   the title's palette (title_palette), title_screen_update started, a fade in (fade 1), the input ring's
   places to 0 (input_write_place, input_read_place), the screen on (C0001C = 6, C8001C = 86 for screen
   2); 1 when the fade is over, mode_timer 450 frames (15 seconds); 2 counts them down, then a fade out
   (fade 0); 3 when it is over, game mode 3 (the car presentation). attract_lamps_update every frame
   first. */
void game_mode_title(void)
{
	uint32_t colour_block = 0, object = 0;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (SCREEN(screen)->mode_step)
	{
	case 0:
		clear_objects();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		colour_block = title_palette;
		queue_palette(colour_block);
		object = start_object();
		OBJECT(object)->handler = title_screen_update_ADDRESS;
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
		invalid_jump(title_step_jumps[SCREEN(screen)->mode_step]);
	}
}

/* Unused: starts the 81 bars of a picture, an object each (start_object, handler bar_picture_bar_start)
   from the 8-byte records of bar_picture_bars (ended by a negative first word): its place on the screen
   into target_x, target_y and its two scales into scales. The places make two rows across the screen (y
   19-107 and 139-195); what the bars show cannot be told without drawing them. */
void bar_picture_start(void)
{
	uint32_t bar = 0, object = 0;
	uint32_t scales = 0, target_x_long = 0;

	bar = bar_picture_bars_ADDRESS;
	do
	{
		object = start_object();
		OBJECT(object)->handler = bar_picture_bar_start_ADDRESS;
		scales = BAR_PICTURE_BAR(bar)->scales;
		target_x_long = BAR_PICTURE_BAR(bar)->target;
		OBJECT(object)->bar_picture.target_x = (uint16_t)target_x_long;
		OBJECT(object)->bar_picture.target_y = (uint16_t)(target_x_long >> 16u);
		OBJECT(object)->bar_picture.scales = scales;
		bar += 8u;
	} while ((int32_t)BAR_PICTURE_BAR(bar)->target >= 0);
}

/* Only unused code reaches it (bar_picture_start): sets up a bar of the picture: drawn on the screen
   (draw_mode 0) with animation 0 and palette 25, its width_scale and height_scale from scales, depth
   layer 3, at the middle of the screen (x A0, y 70), with a random speed_x and speed_y (-8 to 7: a byte
   of RANDOM_D80000 shifted right 4); then bar_picture_bar_fly. */
void bar_picture_bar_start(void)
{
	uint32_t animation = 0;
	uint8_t random_x = 0, random_y = 0;
	uint32_t width_scale_long = 0;

	OBJECT(current_object)->draw_mode = 0u;
	animation = 0u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x25u;
	width_scale_long = OBJECT(current_object)->bar_picture.scales;
	OBJECT(current_object)->width_scale = (uint16_t)width_scale_long;
	OBJECT(current_object)->height_scale = (uint16_t)(width_scale_long >> 16u);
	OBJECT(current_object)->depth_layer = 3u;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	random_x = mem_read8(RANDOM_D80000);
	OBJECT(current_object)->bar_picture.speed_x = (int16_t)(int8_t)random_x >> 4;
	random_y = mem_read8(RANDOM_D80000);
	OBJECT(current_object)->bar_picture.speed_y = (int16_t)(int8_t)random_y >> 4;
	OBJECT(current_object)->handler = bar_picture_bar_fly_ADDRESS;
}

/* Only unused code reaches it (bar_picture_start): each frame the bar's speed_x and speed_y grow by 1
   toward its place (target_x, target_y) and it moves by them, swinging about it; an axis whose next step
   lands exactly on the place is put there for that frame. When both land on the same frame it shows
   frame D and stays (bar_picture_bar_draw). Drawn every frame (draw_object). */
void bar_picture_bar_fly(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0;
	uint16_t old_x_speed = 0, old_y_speed = 0, x_motion = 0;
	uint32_t landed = 0, y_motion = 0;
	uint16_t target_x = 0, x_distance = 0, target_y = 0, y_distance = 0;

	landed = 0xFFFFFFFFu;
	x_motion = (uint16_t)OBJECT(current_object)->bar_picture.speed_x;
	old_x_speed = (uint16_t)x_motion;
	x_motion += OBJECT(current_object)->x;
	target_x = OBJECT(current_object)->bar_picture.target_x;
	x_distance = target_x - x_motion;
	overflow = (uint32_t)(int16_t)target_x - (int16_t)x_motion != (uint32_t)(int16_t)(target_x - x_motion);
	zero = x_distance == 0u;
	negative = x_distance >> 15;
	if (target_x != x_motion)
	{
		x_motion = 1u;
		if ((negative ^ overflow) | zero)
			x_motion = 0xFFFFu;
		x_motion += old_x_speed;
		OBJECT(current_object)->bar_picture.speed_x = x_motion;
		x_motion += OBJECT(current_object)->x;
		landed = 0u;
	}
	OBJECT(current_object)->x = x_motion;
	y_motion = (uint16_t)OBJECT(current_object)->bar_picture.speed_y;
	old_y_speed = (uint16_t)y_motion;
	y_motion = (uint16_t)(y_motion + OBJECT(current_object)->y);
	target_y = OBJECT(current_object)->bar_picture.target_y;
	y_distance = target_y - y_motion;
	overflow = (uint32_t)(int16_t)target_y - (int16_t)y_motion != (uint32_t)(int16_t)(target_y - y_motion);
	zero = y_distance == 0u;
	negative = y_distance >> 15;
	if (target_y == y_motion)
	{
		OBJECT(current_object)->y = y_motion;
		if (landed == 0u)
		{
			y_motion = draw_object(y_motion);
			return;
		}
		OBJECT(current_object)->handler = bar_picture_bar_draw_ADDRESS;
		OBJECT(current_object)->frame = 0xDu;
		y_motion = draw_object(y_motion);
		return;
	}
	y_motion = 1u;
	if ((negative ^ overflow) | zero)
		y_motion = 0xFFFFu;
	y_motion = (uint16_t)(y_motion + old_y_speed);
	OBJECT(current_object)->bar_picture.speed_y = y_motion;
	y_motion = (uint16_t)(y_motion + OBJECT(current_object)->y);
	landed = 0u;
	OBJECT(current_object)->y = y_motion;
	if (landed == 0u)
	{
		y_motion = draw_object(y_motion);
		return;
	}
	OBJECT(current_object)->handler = bar_picture_bar_draw_ADDRESS;
	OBJECT(current_object)->frame = 0xDu;
	y_motion = draw_object(y_motion);
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
   (as clear_screen), the palettes of the car's scene (scene_palettes by attract_car_scene_palettes by
   attract_car) and 031740, object_handlers_1A78 (credit_display, coin_prompt, car_presentation) started,
   a fade in; 1 when it is over, mode_timer 180 frames (6 seconds; the staff credits code adds 1200); 2
   counts them down, then a fade out; 3 when it is over, link_event 3 and game mode 4 (the demo race).
   attract_lamps_update every frame first. */
void game_mode_car_presentation(void)
{
	uint32_t colour_block = 0, handler_list = 0;
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
		invalid_jump(car_presentation_step_jumps[SCREEN(screen)->mode_step]);
	}
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	colour_block = scene_palettes[attract_car_scene_palettes[SCREEN(screen)->attract_car]];
	queue_palette(colour_block);
	colour_block = black_background_palette_ADDRESS;
	queue_palette(colour_block);
	handler_list = 0x1A78u;
	start_object_list(handler_list);
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
		invalid_jump(demo_race_step_jumps[SCREEN(screen)->mode_step]);
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
	SCREEN(screen)->mode_timer &= 0xFF00u;
	SCREEN(screen)->game_step = 0xAu;
	left_caller = 1u;
	return left_caller;
}

/* Step 1 of the demo race: after two frames (mode_timer), a fade in and step 2. */
void demo_race_fade_in(void)
{
	uint8_t frame_parity = 0;

	SCREEN(screen)->mode_timer = (SCREEN(screen)->mode_timer & 0xFF00u) | ((uint8_t)SCREEN(screen)->mode_timer + 1u & 0xFFu);
	frame_parity = (uint8_t)SCREEN(screen)->mode_timer & 1u;
	SCREEN(screen)->mode_timer = (SCREEN(screen)->mode_timer & 0xFF00u) | (frame_parity & 0xFFu);
	if (frame_parity == 0u)
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
   32 steps of the pattern attract_lamp_steps, two bytes a step and screen, into the screen's two lamp
   entries (byte 0 of checkpoint_lamp and leader_lamp by 4 * index: bits 1 and 3 of the seat's output
   port D, which the VBLANK writes). */
void attract_lamps_update(void)
{
	uint32_t attract_lamp_step = 0, screen_index = 0;

	attract_lamp_step = vblank_counter;
	attract_lamp_step = attract_lamp_step >> 3;
	attract_lamp_step = (uint8_t)(attract_lamp_step * 2u);
	screen_index = SCREEN(screen)->index;
	attract_lamp_step = attract_lamp_steps_ADDRESS + (uint8_t)(attract_lamp_step + screen_index) * 2u;
	screen_index = (uint8_t)(screen_index * 2u);
	screen_index = (uint8_t)(screen_index * 2u);
	checkpoint_lamp[screen_index] = ATTRACT_LAMP_STEP(attract_lamp_step)->checkpoint;
	leader_lamp[screen_index] = ATTRACT_LAMP_STEP(attract_lamp_step)->leader;
}

/* The presentation of a car in the attract mode (game mode 3, object_handlers_1A78): the picture of the
   car screen attract_car names. By state: 0 starts car_name and staff_credits_code (with this object as
   its presentation, kept in staff_credits_code), the screen's background (set_up_background with 001AA0
   by screen), the picture (animation attract_car_picture_animations by the car, palette 275, draw mode
   0) at x A0, y 60 on depth layer 0; 1 steps its animation (animation_advance) and draws it. Every
   frame, in all states, it queues the next of the four palette records of car_presentation_palette_cycle
   (palette_step, round and round) with queue_palette_list, so the picture's colours cycle. State 2 (the
   palette only, no picture) is not set by its own code. */
void car_presentation_update(void)
{
	uint32_t animation = 0, background_setup = 0, colour_list = 0, object = 0, palette_index = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = car_presentation_state_jumps[state];
	switch (state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = car_name_start_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = staff_credits_code_start_ADDRESS;
		OBJECT(object)->staff_credits_code.presentation = current_object;
		OBJECT(current_object)->car_presentation.staff_credits_code = object;
		OBJECT(current_object)->state++;
		background_setup = 0x1AA0u + SCREEN(screen)->index * 5u * 4u;
		set_up_background(background_setup);
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x60u;
		OBJECT(current_object)->depth_layer = 0u;
		routine = SCREEN(screen)->attract_car;
		animation = attract_car_picture_animations[routine];
		OBJECT(current_object)->palette = 0x275u;
		animation_start(animation);
		animation_advance(0u);
		draw_object(routine);
		break;
	case 1:
		animation_advance(0u);
		draw_object(routine);
		palette_index = OBJECT(current_object)->car_presentation.palette_step;
		colour_list = car_presentation_palette_cycle[palette_index];
		if (colour_list == 0u)
		{
			colour_list = car_presentation_palette_cycle[0];
			palette_index = 0u;
		}
		OBJECT(current_object)->car_presentation.palette_step = palette_index + 1u;
		queue_palette_list(colour_list);
		return;
	case 2:
		break;
	default:
		invalid_jump(car_presentation_state_jumps[state]);
	}
	palette_index = OBJECT(current_object)->car_presentation.palette_step;
	colour_list = car_presentation_palette_cycle[palette_index];
	if (colour_list == 0u)
	{
		colour_list = car_presentation_palette_cycle[0];
		palette_index = 0u;
	}
	OBJECT(current_object)->car_presentation.palette_step = palette_index + 1u;
	queue_palette_list(colour_list);
}

/* Starts watching for the secret code during the presentation of the cars: takes the controls held
   (staff_credits_code_read_controls), puts code_place at the start of the code (001ADC) and goes on to
   staff_credits_code_watch. */
void staff_credits_code_start(void)
{
	uint32_t zero = 0;
	uint32_t new_controls = 0;

	staff_credits_code_read_controls(0u, &new_controls, &zero);
	OBJECT(current_object)->staff_credits_code.code_place = staff_credits_code_sequence_ADDRESS;
	OBJECT(current_object)->handler = staff_credits_code_watch_ADDRESS;
	staff_credits_code_watch(new_controls, 0u);
}

/* Watches the controls for the code (staff_credits_code_sequence: shift down twice, shift up twice, the
   wheel left, right, left, right, the brake, the gas; a wrong one starts over); when it is complete:
   sound 9, the presentation 1200 frames longer (screen mode_timer), and staff_credits_code_show. */
void staff_credits_code_watch(uint32_t new_controls, uint32_t control_reading)
{
	uint32_t zero = 0;
	uint8_t sound_code = 0;
	uint32_t next_control = 0;
	uint8_t element = 0;
	uint32_t presentation_timer = 0;

	staff_credits_code_read_controls(control_reading, &new_controls, &zero);
	if (zero)
	{
		staff_credits_code_done();
		return;
	}
	next_control = OBJECT(current_object)->staff_credits_code.code_place;
	element = STAFF_CREDITS_CODE_CONTROLS(next_control)[0];
	next_control++;
	if (element != (uint8_t)new_controls)
		next_control = staff_credits_code_sequence_ADDRESS;
	OBJECT(current_object)->staff_credits_code.code_place = next_control;
	if (STAFF_CREDITS_CODE_CONTROLS(next_control)[0] != 0u)
	{
		staff_credits_code_done();
		return;
	}
	sound_code = 9u;
	queue_sound(sound_code);
	SCREEN(screen)->mode_timer += 0x4B0u;
	presentation_timer = OBJECT(current_object)->staff_credits_code.presentation + 0x10u;
	mem_write16(presentation_timer, mem_read16(presentation_timer) + 0x4B0u);
	OBJECT(current_object)->staff_credits_code.frames_left = 0x4B0u;
	OBJECT(current_object)->handler = staff_credits_code_show_ADDRESS;
	staff_credits_code_show();
}

/* Shows the staff credits (the texts at 001AE8, draw_text) for 1200 frames, then fade_out_music and
   staff_credits_code_done. */
void staff_credits_code_show(void)
{
	uint32_t next_text = 0, text_record = 0;
	uint16_t frames_left = 0;

	frames_left = OBJECT(current_object)->staff_credits_code.frames_left;
	OBJECT(current_object)->staff_credits_code.frames_left = frames_left - 1u;
	if (frames_left == 1u)
	{
		OBJECT(current_object)->handler = staff_credits_code_done_ADDRESS;
		fade_out_music();
		return;
	}
	next_text = 0x1AE8u;
	while ((int16_t)mem_read16(next_text) >= 0)
	{
		text_record = next_text;
		next_text = draw_text(text_record);
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
void staff_credits_code_read_controls(uint32_t control_reading, uint32_t *p_new_controls, uint32_t *p_zero)
{
	uint32_t new_controls = *p_new_controls;
	uint32_t zero = *p_zero;
	uint8_t newly_on = 0;
	uint32_t control_bit = 0, controls_now = 0, held_before = 0, reading = 0;
	uint8_t pressed_controls = 0;

	held_before = (new_controls & ~0xFFu) | OBJECT(current_object)->staff_credits_code.controls_held;
	controls_now = 0u;
	reading = (control_reading & ~0xFFu) | (uint8_t)~SCREEN(screen)->wheel;
	if ((held_before & 8u) != 0u)
	{
		if ((uint8_t)reading < 0x40u)
			controls_now |= 8u;
	}
	else if ((uint8_t)reading < 0x20u)
		controls_now |= 8u;
	control_bit = 2u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, (uint8_t)reading, control_bit);
	reading = (reading & ~0xFFu) | SCREEN(screen)->gas;
	control_bit = 5u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, (uint8_t)reading, control_bit);
	reading = (reading & ~0xFFu) | SCREEN(screen)->brake;
	control_bit = 4u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, (uint8_t)reading, control_bit);
	reading = (reading & ~0xFFu) | (uint8_t)SCREEN(screen)->buttons;
	if ((reading & 0x10u) != 0u)
		controls_now |= 1u;
	if ((reading & 0x20u) != 0u)
		controls_now |= 2u;
	OBJECT(current_object)->staff_credits_code.controls_held = controls_now;
	pressed_controls = ~held_before & controls_now;
	zero = pressed_controls == 0u;
	newly_on = pressed_controls;
	*p_new_controls = (*p_new_controls & 0xFFFFFF00u) | newly_on;
	*p_zero = zero;
}

/* Sets bit R3 of the controls being read (R1) when the value R2 is E0 or more, or C0 or more when the
   bit was on in the controls held before (R0): staff_credits_code_read_controls' test for the wheel
   turned right, the gas and the brake. */
uint32_t staff_credits_code_control_bit(uint32_t held_before, uint32_t controls_now, uint8_t reading, uint32_t control_bit)
{
	if ((held_before & shl32(1u, (int)(control_bit))) != 0u)
	{
		if ((uint8_t)reading >= 0xC0u)
			controls_now |= shl32(1u, (int)(control_bit));
	}
	else if ((uint8_t)reading >= 0xE0u)
	{
		controls_now |= shl32(1u, (int)(control_bit));
		return controls_now;
	}
	return controls_now;
}

/* Starts the letters of the presented car's name (attract_car_names by screen attract_car: MAD POWER,
   SPEED BUSTER, EASY HANDLING, SMOOTH OPERATOR, BAD BOY, ROAD MONSTER, QUICK REACTOR, WILD CHASER): a
   car_name_letter for each character, spaces too, 18 pixels apart from x 168, each 2 frames later than
   the one before (delay 15, 17...); then ends. */
void car_name_start(void)
{
	uint8_t letter_delay = 0;
	uint16_t letter_x = 0;
	uint32_t letter_place = 0, object = 0;

	letter_place = attract_car_names[SCREEN(screen)->attract_car];
	letter_x = 0x168u;
	letter_delay = 0xFu;
	while (CAR_NAME_LETTERS(letter_place)[0] != 0u)
	{
		object = start_object();
		OBJECT(object)->handler = car_name_letter_update_ADDRESS;
		OBJECT(object)->car_name_letter.character = CAR_NAME_LETTERS(letter_place)[0];
		OBJECT(object)->x = letter_x;
		letter_x += 0x12u;
		OBJECT(object)->car_name_letter.delay = letter_delay;
		letter_delay += 2u;
		letter_place++;
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
	uint32_t animation = 0, ignored = 0;
	uint8_t delay = 0, slide_frames = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x346u;
		animation_start(animation);
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
		invalid_jump(car_name_letter_state_jumps[OBJECT(current_object)->state]);
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
   byte negative) gets its default from default_course_records (SGA, 5'00"00); the model is kept to its
   low three bits. */
void load_course_records(void)
{
	uint32_t backup_record = 0, course_record = 0, courses_left = 0;
	uint32_t model_place = 0;

	courses_left = 0xAu;
	backup_record = SOUND_RAM + 0x1F32u;
	course_record = default_course_records_ADDRESS;
	r12 = course_records_ADDRESS;
	for (;;)
	{
		if ((int8_t)mem_read8(backup_record) >= 0)
		{
			mem_write8(r12, mem_read8(backup_record));
			model_place = r12;
			r12 = r12 + 1u;
			mem_write8(model_place, mem_read8(model_place) & 7u);
			backup_record++;
			mem_write8(r12, mem_read8(backup_record));
			r12 = r12 + 1u;
			backup_record++;
			mem_write8(r12, mem_read8(backup_record));
			r12 = r12 + 1u;
			backup_record++;
			mem_write8(r12, mem_read8(backup_record));
			r12 = r12 + 1u;
			backup_record++;
			mem_write16(r12, mem_read16(backup_record));
			r12 = r12 + 2u;
			course_record += 6u;
			courses_left--;
			backup_record += 2u;
			if (courses_left == 0u)
				return;
			continue;
		}
		mem_write32(r12, AS32(COURSE_RECORD(course_record)->model));
		r12 = r12 + 4u;
		course_record += 4u;
		mem_write16(r12, COURSE_RECORD(course_record - 0x4u)->time);
		r12 = r12 + 2u;
		backup_record += 6u;
		courses_left--;
		course_record += 2u;
		if (courses_left != 0u)
			continue;
		break;
	}
}

/* Game mode 5, the ranking (ranking_update), after the demo race and after a game, by mode_step: 0
   demo_flags bit 8 clear, the wheel motor off (wheel_motor by index), the sound off
   (reset_screen_sound), the screen cleared (as clear_screen), clear_published_car, the ranking's palette
   (ranking_palette), object_handlers_2CB8 (credit_display, coin_prompt) and ranking_update started,
   mode_timer 270 frames (9 seconds), race game_over cleared; 1 waits until no fade runs
   (ranking_background fades the screen in); 2 counts the 270 frames down; 3 when no fade runs, game mode
   1 (back to the title). attract_lamps_update every frame first. */
void game_mode_ranking(void)
{
	uint32_t colour_block = 0, handler_list = 0, object = 0;
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
		invalid_jump(ranking_step_jumps[SCREEN(screen)->mode_step]);
	}
	SCREEN(screen)->demo_flags &= ~0x100u;
	wheel_motor[SCREEN(screen)->index] = 0u;
	reset_screen_sound();
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	clear_published_car();
	colour_block = ranking_palette;
	queue_palette(colour_block);
	SCREEN(screen)->mode_step++;
	handler_list = 0x2CB8u;
	start_object_list(handler_list);
	object = start_object();
	OBJECT(object)->handler = ranking_update_ADDRESS;
	SCREEN(screen)->mode_timer = 0x10Eu;
	RACE(SCREEN(screen)->race)->game_over = 0u;
}

/* Whether the screen's player has beaten the course's record (zero clear): when the race was finished
   (race_copy finished), the course (the goal's stage, screen built_stage, less 15) into record_course,
   and a record when the player came first (screen 320 + player is 0) in a time (screen 410 + 2 * player)
   under the course's (course_record_times by course). */
uint32_t course_record_beaten(void)
{
	uint32_t zero = 0;
	uint32_t course_number = 0, player = 0;

	if (RACE(SCREEN(screen)->race_copy)->finished != 0u)
	{
		course_number = SCREEN(screen)->built_stage;
		course_number = (uint8_t)(course_number - 0x15u);
		SCREEN(screen)->record_course = course_number;
		player = SCREEN(screen)->player;
		if (mem_read8(screen + 0x320u + player) == 0u && mem_read16(screen + 0x410u + player * 2u) < course_record_times[course_number * 3u])
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
   (turn_delay counts down by name_entry_turn_rates by the wheel), and << and >> a letter each time; the
   letter in front is letter, and shows blinking at the name's next place; the gas pedal let up (under
   40) then pressed (C0 or more), START or the music button enters it with a sound
   (name_entry_place_sounds by place): the left arrow takes the last letter back, the right arrow enters
   a space, ED or the third letter ends the entry, as does the timer running out (the seconds left show
   as the countdown number, time_digits_draw); then open 0, an empty name or one of the names of
   name_entry_rejected_names (SEX, XXX, XX and X's with spaces) becomes ---, 60 frames; 3 writes the
   record into the race record (unk_11 + 1, unk_16 the time, unk_12 the car's model, unk_13-15 the name,
   unk_18 the course, screen built_stage - 15), turns the lamps off and sets the game's next step (screen
   game_mode byte 2 = 0x15 (step 21)); 4 nothing. Every frame it draws the time and the name (draw_text). */
void name_entry_update(void)
{
	bool L_00200F = false, L_00209D = false, L_0020B6 = false, L_0020FB = false, L_00212C = false, L_002130 = false, L_002133 = false;
	uint8_t letter_character = 0, letter_index = 0, new_steps = 0, sound_code = 0, steps = 0, turn_rate = 0;
	uint16_t buttons_pressed = 0, new_angle = 0, turn_buttons = 0;
	uint32_t background_setup = 0, entered_name = 0, handler_list = 0, ignored_2 = 0, letter = 0, name_place = 0, object = 0, palette_fades = 0, player_time = 0, race = 0, rejected_name = 0, remaining_time = 0, saved_letter = 0, text_field = 0, text_record = 0, wheel_position = 0;
	uint32_t kept_letter = 0, kept_place = 0;
	uint8_t letters_left = 0, turn_delay = 0, entered_letter = 0;
	uint16_t spin_frames = 0, timer = 0, timer_2 = 0;
	uint32_t jump_target = 0, jump_target_2 = 0, element = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		SCREEN(screen)->button_lamps = 0xFFFFu;
		lcd_restart_frames = 0u;
		object = start_object();
		OBJECT(object)->handler = name_entry_lcd_update_ADDRESS;
		OBJECT(object)->name_entry_lcd.name_entry = current_object;
		background_setup = 0x2E4Cu + SCREEN(screen)->index * 5u * 4u;
		set_up_background(background_setup);
		palette_fades = name_entry_palette_fades_ADDRESS;
		start_palette_fades(palette_fades);
		time_digits_start();
		handler_list = 0x2D58u;
		start_object_list(handler_list);
		letter_index = 0x20u;
		do
		{
			object = start_object();
			OBJECT(object)->name_entry_letter.name_entry = current_object;
			OBJECT(object)->handler = name_entry_letter_update_ADDRESS;
			letters_left = letter_index;
			letter_index = (uint8_t)(letters_left - 1u);
			OBJECT(object)->name_entry_letter.index = letter_index;
		} while (letters_left != 1u);
		OBJECT(current_object)->name_entry.open = 0xFFu;
		OBJECT(current_object)->name_entry.timer = 0x258u;
		OBJECT(current_object)->name_entry.spin_frames = 0x28u;
		OBJECT(current_object)->name_entry.wheel_angle = 0xA80u;
		OBJECT(current_object)->name_entry.letter = 0xFFu;
		player_time = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
		text_field = current_object + 0x9Au;
		OBJECT(text_field - 0x9Au)->name_entry.name_x = 0xF8u;
		text_field += 2u;
		OBJECT(text_field - 0x9Cu)->name_entry.name_y = 0x54u;
		OBJECT(text_field - 0x9Cu)->name_entry.name_attribute = 0xF0u;
		text_field = current_object + 0xA4u;
		OBJECT(text_field - 0xA4u)->name_entry.time_x = 0xA8u;
		text_field += 2u;
		OBJECT(text_field - 0xA6u)->name_entry.time_y = 0x54u;
		text_field += 2u;
		OBJECT(text_field - 0xA8u)->name_entry.time_attribute = 0xF0u;
		text_field += 2u;
		time_to_text(player_time, text_field);
		OBJECT(current_object)->state++;
	case 1:
		OBJECT(current_object)->name_entry.wheel_angle += 0x20u;
		spin_frames = OBJECT(current_object)->name_entry.spin_frames;
		OBJECT(current_object)->name_entry.spin_frames = spin_frames - 1u;
		if (spin_frames != 1u)
		{
			remaining_time = OBJECT(current_object)->name_entry.timer;
			remaining_time = remaining_time / 0x1Eu;
			remaining_time = binary_to_bcd(remaining_time);
			time_digits_draw(remaining_time);
			text_record = current_object + 0xA4u;
			draw_text(text_record);
			text_record = current_object + 0x9Au;
			ignored_2 = draw_text(text_record);
			return;
		}
		OBJECT(current_object)->state++;
	case 2:
		steps = (uint8_t)OBJECT(current_object)->name_entry.turn_steps;
		L_00200F = steps == 0u;
		if (!L_00200F)
		{
			if ((int8_t)steps < 0)
			{
				if (SCREEN(screen)->wheel >= 0x60u)
				{
					OBJECT(current_object)->name_entry.turn_steps = steps + 1u;
					new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
				}
				else
				{
					steps += 4u;
					OBJECT(current_object)->name_entry.turn_steps = steps - 1u;
					new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
				}
			}
			else if (SCREEN(screen)->wheel < 0xA0u)
			{
				OBJECT(current_object)->name_entry.turn_steps = steps - 1u;
				new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
			}
			else
			{
				steps -= 4u;
				OBJECT(current_object)->name_entry.turn_steps = steps + 1u;
				new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
			}
			OBJECT(current_object)->name_entry.wheel_angle = new_angle;
			L_00200F = (uint8_t)OBJECT(current_object)->name_entry.turn_steps == 0u;
		}
		if (L_00200F)
		{
			wheel_position = SCREEN(screen)->wheel;
			wheel_position = wheel_position >> 4;
			turn_rate = name_entry_turn_rates[wheel_position];
			if (turn_rate != 0u)
			{
				turn_delay = OBJECT(current_object)->name_entry.turn_delay;
				OBJECT(current_object)->name_entry.turn_delay = turn_delay - turn_rate;
				if ((int8_t)turn_delay <= (int8_t)turn_rate)
				{
					OBJECT(current_object)->name_entry.turn_delay = 0xF;
					new_steps = 4u;
					if (wheel_position >> 2 != 0u)
						new_steps = (uint8_t)(0u - new_steps);
					OBJECT(current_object)->name_entry.turn_steps = new_steps;
				}
			}
			else
			{
				turn_buttons = (uint16_t)SCREEN(screen)->buttons_pressed;
				if ((turn_buttons & 0x200u) != 0u)
				{
					if ((turn_buttons & 0x400u) == 0u)
					{
						wheel_position = 0xFFu;
						turn_rate = 0xFu;
						turn_delay = OBJECT(current_object)->name_entry.turn_delay;
						OBJECT(current_object)->name_entry.turn_delay = turn_delay - turn_rate;
						if ((int8_t)turn_delay <= (int8_t)turn_rate)
						{
							OBJECT(current_object)->name_entry.turn_delay = 0xF;
							new_steps = 4u;
							if (wheel_position >> 2 != 0u)
								new_steps = (uint8_t)(0u - new_steps);
							OBJECT(current_object)->name_entry.turn_steps = new_steps;
						}
					}
				}
				else if ((turn_buttons & 0x400u) != 0u)
				{
					wheel_position = 0u;
					turn_rate = 0xFu;
					turn_delay = OBJECT(current_object)->name_entry.turn_delay;
					OBJECT(current_object)->name_entry.turn_delay = turn_delay - turn_rate;
					if ((int8_t)turn_delay <= (int8_t)turn_rate)
					{
						OBJECT(current_object)->name_entry.turn_delay = 0xF;
						new_steps = 4u;
						if (wheel_position >> 2 != 0u)
							new_steps = (uint8_t)(0u - new_steps);
						OBJECT(current_object)->name_entry.turn_steps = new_steps;
					}
				}
				else
					OBJECT(current_object)->name_entry.turn_delay = 1;
			}
		}
		OBJECT(current_object)->name_entry.letter = ((OBJECT(current_object)->name_entry.wheel_angle + 0x40u) & 0xFFFu) >> 7;
		do
		{
			jump_target = L_00209D ? 0x2090u : name_entry_gas_step_jumps[OBJECT(current_object)->name_entry.gas_step];
			switch (jump_target)
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
						name_place = OBJECT(current_object)->name_entry.place;
						do
						{
							do
							{
								jump_target_2 = L_00212C ? 0x20D4u : L_0020FB ? 0x20D4u : name_entry_place_jumps[name_place];
								switch (jump_target_2)
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
												kept_letter = letter;
												kept_place = name_place;
												sound_code = name_entry_place_sounds[name_place];
												request_sound(sound_code);
												name_place = kept_place;
												saved_letter = kept_letter;
												entered_letter = saved_letter;
												letter_character = mem_read8(name_entry_letters_ADDRESS + saved_letter);
												if (entered_letter == 0x1Du)
													letter_character = 0x20u;
												mem_write8(current_object + 0xA0u + name_place, letter_character);
												name_place = (name_place & ~0xFFu) | (uint8_t)(name_place + 1u);
											}
											if (L_00212C || (uint8_t)name_place != 3u)
											{
												L_00212C = false;
												OBJECT(current_object)->name_entry.place = name_place;
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
										mem_write8(current_object + 0xA0u + name_place, 0u);
										name_place = (uint8_t)(name_place - 1u);
										L_00212C = true;
									}
									if (L_00212C)
										break;
									L_0020FB = letter != 0x1Fu;
									if (L_0020FB)
										break;
									break;
								default:
									invalid_jump(jump_target_2);
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
					name_place = OBJECT(current_object)->name_entry.place;
					letter_character = name_entry_letters[OBJECT(current_object)->name_entry.letter];
					if ((game_frame_count & 4u) == 0u)
						letter_character = 0u;
					mem_write8(current_object + 0xA0u + name_place, letter_character);
					timer = OBJECT(current_object)->name_entry.timer;
					OBJECT(current_object)->name_entry.timer = timer - 1u;
					if (timer != 1u)
					{
						remaining_time = OBJECT(current_object)->name_entry.timer;
						remaining_time = remaining_time / 0x1Eu;
						remaining_time = binary_to_bcd(remaining_time);
						time_digits_draw(remaining_time);
						text_record = current_object + 0xA4u;
						draw_text(text_record);
						text_record = current_object + 0x9Au;
						ignored_2 = draw_text(text_record);
						return;
					}
				}
				break;
			case 0x00209Du:
				L_00209D = true;
				break;
			default:
				invalid_jump(jump_target);
			}
		} while (L_00209D);
		OBJECT(current_object)->name_entry.open = 0u;
		mem_write8(current_object + 0xA0u + name_place, 0u);
		entered_name = AS32(OBJECT(current_object)->name_entry.name);
		if (entered_name == 0u)
			AS32(OBJECT(current_object)->name_entry.name) = 0x2D2D2Du;
		else
		{
			rejected_name = name_entry_rejected_names_ADDRESS;
			while ((int32_t)REJECTED_NAMES(rejected_name)[0] >= 0)
			{
				element = REJECTED_NAMES(rejected_name)[0];
				rejected_name += 4u;
				if (entered_name == element)
				{
					AS32(OBJECT(current_object)->name_entry.name) = 0x2D2D2Du;
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
			RACE(race)->record_name_rest = (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u);
			RACE(race)->record_course = SCREEN(screen)->built_stage - 0x15u;
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
			RACE(race)->record_name_rest = (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u);
			RACE(race)->record_course = SCREEN(screen)->built_stage - 0x15u;
			OBJECT(current_object)->state++;
		}
		text_record = current_object + 0xA4u;
		draw_text(text_record);
		text_record = current_object + 0x9Au;
		ignored_2 = draw_text(text_record);
		return;
	case 4:
		break;
	default:
		invalid_jump(name_entry_state_jumps[OBJECT(current_object)->state]);
	}
	text_record = current_object + 0xA4u;
	draw_text(text_record);
	text_record = current_object + 0x9Au;
	ignored_2 = draw_text(text_record);
}

/* A letter on the name entry's drum (font 2DE; index 0-31: A-Z ! ? . and the right arrow, the left arrow
   and ED, name_entry_letters); ends when the entry closes (the name entry's open is 0). By state: 0 its
   angle index * 80 round the drum, y_offset -5; 1 for 40 frames (spin_frames) y_offset + 1, the drum
   dropping into place. Every frame its place from its angle less the wheel_angle (sine_table,
   cosine_table): x_offset, depth, a width that narrows toward the drum's edges (width_factor), mirrored
   (flags bit 6) with palette 27 where it faces away (the cosine under 2000, more than 60 degrees from
   the front), 34 in front, 30 for the letter in front (shown 4 frames of 8: hidden, flags bit 5 cleared,
   when bit 2 of game_frame_count is clear); drawn in perspective (100000 / (depth + 100)) about x E0, y
   40. */
void name_entry_letter_update(void)
{
	uint32_t carry = 0;
	uint16_t letter_palette = 0;
	uint32_t animation = 0, front_cosine = 0, index = 0, letter_angle = 0, letter_depth = 0, letter_scale = 0, letter_x = 0, letter_y = 0, object = 0, width_base = 0;
	uint16_t spin_frames = 0;
	uint32_t toward_front = 0;

	object = OBJECT(current_object)->name_entry_letter.name_entry;
	if (OBJECT(object)->name_entry.open == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = 0x2DEu;
		animation_start(animation);
		index = OBJECT(current_object)->name_entry_letter.index;
		OBJECT(current_object)->frame = name_entry_letters[index];
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
		invalid_jump(name_entry_letter_state_jumps[OBJECT(current_object)->state]);
	}
	letter_angle = OBJECT(current_object)->name_entry_letter.angle;
	letter_angle = (uint16_t)(letter_angle - OBJECT(object)->name_entry.wheel_angle);
	letter_angle = letter_angle & 0xFFFu;
	letter_x = sine_table[letter_angle];
	letter_depth = cosine_table[letter_angle];
	front_cosine = letter_depth;
	letter_x = (int32_t)letter_x >> 7;
	letter_depth = 0u - ((int32_t)letter_depth >> 7);
	OBJECT(current_object)->name_entry_letter.x_offset = letter_x;
	OBJECT(current_object)->name_entry_letter.depth = letter_depth;
	letter_palette = 0x34u;
	if (OBJECT(object)->name_entry.letter == OBJECT(current_object)->name_entry_letter.index)
	{
		if ((game_frame_count & 4u) == 0u)
		{
			OBJECT(current_object)->flags &= 0xDFu;
			return;
		}
		letter_palette = 0x30u;
	}
	toward_front = front_cosine - 0x2000u;
	width_base = toward_front;
	if ((int32_t)toward_front < 0)
	{
		OBJECT(current_object)->flags |= 0x40u;
		width_base = 0u - width_base;
		letter_palette = 0x27u;
	}
	else
	{
		OBJECT(current_object)->flags &= 0xBFu;
		width_base = width_base * 3u;
	}
	OBJECT(current_object)->name_entry_letter.width_factor = (int32_t)(width_base * 3u) >> 8;
	OBJECT(current_object)->palette = letter_palette;
	letter_y = OBJECT(current_object)->name_entry_letter.y_offset;
	letter_scale = divide_s32(0x100000u, letter_depth + 0x100u);
	letter_x = letter_x * letter_scale;
	letter_y = letter_y * letter_scale;
	carry = (letter_x >> 11) & 1u;
	letter_x = (uint16_t)(((int32_t)letter_x >> 12) + carry);
	carry = (letter_y >> 11) & 1u;
	letter_y = (uint16_t)(((int32_t)letter_y >> 12) + carry + 0x40u);
	OBJECT(current_object)->x = letter_x + 0xE0u;
	OBJECT(current_object)->y = letter_y;
	OBJECT(current_object)->depth_layer = letter_y;
	carry = (letter_scale >> 3) & 1u;
	letter_scale = ((int32_t)letter_scale >> 4) + carry;
	OBJECT(current_object)->height_scale = letter_scale;
	letter_scale = (int32_t)(letter_scale * OBJECT(current_object)->name_entry_letter.width_factor) >> 8;
	OBJECT(current_object)->width_scale = letter_scale;
	letter_scale = draw_object(letter_scale);
}

/* Shows the name entry on the cabinet LCD: 0 clears it (clear_lcd), shows the DJ's face (screen dj_face
   = FF) and writes the time (7 characters of the entry's time_text) at F13; then every frame the name's
   three characters at F1C, in the LCD's codes (7F -> 7E and | -> 7F, its arrows; \ -> 65); ends when the
   entry closes. It means to write spaces after the end of the name, but tests the address (tb R10)
   instead of the character, so it never does. */
void name_entry_lcd_update(void)
{
	uint8_t lcd_character = 0;
	uint32_t characters_left = 0, lcd_place = 0, name_place = 0, object = 0;

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
		invalid_jump(name_entry_lcd_state_jumps[OBJECT(current_object)->state]);
	}
	object = OBJECT(current_object)->name_entry_lcd.name_entry;
	name_place = object + 0xA0u;
	lcd_place = screen + 0xF1Cu;
	characters_left = 3u;
	do
	{
		lcd_character = mem_read8(name_place);
		name_place++;
		if (name_place == 0u)
		{
			do
			{
				mem_write8(lcd_place, 0x20u);
				characters_left--;
				lcd_place++;
			} while (characters_left != 0u);
			break;
		}
		if (lcd_character == 0x7Fu)
			lcd_character = 0x7Eu;
		if (lcd_character == 0x7Cu)
			lcd_character = 0x7Fu;
		if (lcd_character == 0x5Cu)
			lcd_character = 0x65u;
		mem_write8(lcd_place, lcd_character);
		characters_left--;
		lcd_place++;
	} while (characters_left != 0u);
	if (OBJECT(object)->name_entry.open == 0u)
		OBJECT(current_object)->flags = 0u;
}

/* The player's car on the certificate screen (animation certificate_car_animations by the car's model,
   car model; palette car_model_palettes) at x 40, y C0: state 0 sets it up and runs into
   name_entry_drivers_update, which with state 1 draws it; then it draws itself every frame. */
void name_entry_car_update(void)
{
	uint32_t animation = 0, model = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = name_entry_car_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(name_entry_car_state_jumps[state]);
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	model = CAR(player_car)->model;
	animation = certificate_car_animations[model];
	OBJECT(current_object)->palette = car_model_palettes[model];
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation_start(animation);
	OBJECT(current_object)->x = 0x40u;
	OBJECT(current_object)->y = 0xC0u;
	OBJECT(current_object)->frame = 0u;
	name_entry_drivers_update();
}

/* The car's two drivers standing by it on the certificate screen (animation
   certificate_driver_animations by the car's model, car model; palette certificate_driver_palettes) at x
   60, y C8: state 0 sets them up and runs into name_entry_course_name_update, which with state 1 draws
   them; then drawn every frame. */
void name_entry_drivers_update(void)
{
	uint32_t animation = 0, model = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = name_entry_drivers_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(name_entry_drivers_state_jumps[state]);
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	model = CAR(player_car)->model;
	animation = certificate_driver_animations[model];
	OBJECT(current_object)->palette = certificate_driver_palettes[model];
	OBJECT(current_object)->depth_layer = 0x19Du;
	animation_start(animation);
	OBJECT(current_object)->x = 0x60u;
	OBJECT(current_object)->y = 0xC8u;
	OBJECT(current_object)->frame = 0u;
	name_entry_course_name_update();
}

/* The course's name on the certificate (Australia Course...: animation certificate_course_animations by
   the course, screen built_stage - 15; palette certificate_course_palettes) at x E0, y 40: state 0 sets
   it up; drawn every frame. */
void name_entry_course_name_update(void)
{
	uint32_t animation = 0, course_number = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = name_entry_course_name_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(name_entry_course_name_state_jumps[state]);
	}
	OBJECT(current_object)->state++;
	course_number = SCREEN(screen)->built_stage;
	course_number = (uint8_t)(course_number - 0x15u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->palette = certificate_course_palettes[course_number];
	animation = certificate_course_animations[course_number];
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation_start(animation);
	OBJECT(current_object)->x = 0xE0u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->y = 0x40u;
	course_number = draw_object(course_number);
}

/* The ranking in the attract mode (game mode 5, game_mode_ranking): OutRunner of OutRunners, the record
   of three courses in turn. By state: 0 starts object_handlers_2CB8 (01AA01, 01A6CF, which
   game_mode_ranking has started too), ranking_background and the two halves of the title
   (ranking_title_left, ranking_title_right); 1 clears course_over; once the screen's fade is over
   (screen flags bit 0): after three courses ends, else courses_shown + 1, 60 frames (frames_left), and
   starts ranking_course_banner and ranking_record for the course screen record_course; 2 when the 60
   frames are over sets course_over for a frame (the background fades out) and moves screen record_course
   to the next of the ten courses, back to 1. */
void ranking_update(void)
{
	uint8_t next_course = 0, record_course = 0;
	uint32_t handler_list = 0, object = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->ranking.courses_shown = 0u;
		handler_list = 0x2CB8u;
		start_object_list(handler_list);
		object = start_object();
		OBJECT(object)->ranking_background.ranking = current_object;
		OBJECT(object)->handler = ranking_background_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = ranking_title_left_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = ranking_title_right_update_ADDRESS;
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
		object = start_object();
		OBJECT(object)->handler = ranking_course_banner_update_ADDRESS;
		OBJECT(object)->ranking_course_banner.ranking = current_object;
		OBJECT(object)->ranking_course_banner.course = record_course;
		object = start_object();
		OBJECT(object)->handler = ranking_record_start_ADDRESS;
		OBJECT(object)->ranking_record.course = record_course;
		OBJECT(current_object)->state++;
		return;
	case 2:
		frames_left = OBJECT(current_object)->ranking.frames_left;
		OBJECT(current_object)->ranking.frames_left = frames_left - 1u;
		if (frames_left != 1u)
			return;
		OBJECT(current_object)->ranking.course_over |= 0x2000u;
		next_course = (uint8_t)(SCREEN(screen)->record_course + 1u);
		if (next_course >= 0xAu)
			next_course = 0u;
		SCREEN(screen)->record_course = next_course;
		OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(ranking_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The picture of the course behind the ranking, while the game mode is the ranking (screen game_mode 5).
   By state: 0 courses_shown from the ranking; for the first course I/O C0001C = 6 (C8001C = 86 for
   screen 2) and a fade in (screen fade_kind 1, unk_94 bit 0); ranking_background_load; 1 waits while the
   pages are unpacked (screen unpack_state), then for the later courses a fade in (unk_153 3); 2 when the
   ranking says course_over: after the third course a fade out (unk_153 0) and state 4, else a fade out
   (unk_153 2); 3 when the fade is over, back to 0 for the next course's picture; 4 nothing more. Every
   frame, in any game mode, the picture scrolls by 3 pixels (scroll_x - 3). */
void ranking_background_update(void)
{
	uint32_t scroll_x_address = 0;

	if (SCREEN(screen)->game_mode == 5u)
	{
		switch (OBJECT(current_object)->state)
		{
		case 0:
			OBJECT(current_object)->ranking_background.courses_shown = OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.courses_shown;
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
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
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
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
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
			invalid_jump(ranking_background_state_jumps[OBJECT(current_object)->state]);
		}
	}
	scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
	mem_write16(scroll_x_address, mem_read16(scroll_x_address) - 3u);
}

/* The left half of the ranking's title, OutRunner of OutRunners (animation 2D2, palette 2B2) at x 0, y
   30: state 0 sets it up; drawn every frame. */
void ranking_title_left_update(void)
{
	uint32_t animation = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = ranking_title_left_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B2u;
		animation = 0x2D2u;
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(ranking_title_left_state_jumps[state]);
	}
	routine = draw_object(routine);
}

/* The right half of the ranking's title, OutRunner of OutRunners (animation 2D3, palette 2B3) at x 13F,
   y 30: state 0 sets it up; drawn every frame. */
void ranking_title_right_update(void)
{
	uint32_t animation = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = ranking_title_right_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B3u;
		animation = 0x2D3u;
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = 0x13Fu;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(ranking_title_right_state_jumps[state]);
	}
	routine = draw_object(routine);
}

/* The scroll with the course's name under the ranking's title (Australia Course...: animation
   ranking_course_banner_animations by course, palette ranking_course_banner_palettes) at x A0, y 50, for
   90 frames (frames_left). */
void ranking_course_banner_update(void)
{
	uint32_t animation = 0, course_number = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	course_number = ranking_course_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		course_number = OBJECT(current_object)->ranking_course_banner.course;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = ranking_course_banner_palettes[course_number];
		animation = ranking_course_banner_animations[course_number];
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->ranking_course_banner.frames_left = 0x5Au;
		break;
	case 1:
		break;
	default:
		invalid_jump(ranking_course_banner_state_jumps[state]);
	}
	frames_left = OBJECT(current_object)->ranking_course_banner.frames_left;
	OBJECT(current_object)->ranking_course_banner.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		course_number = draw_object(course_number);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* Sets up the course's record line from the records at 20E300 (by course: the initials, '-' shown as
   '.', and a 0 into text, then the time as 5'00"00, time_to_text), starts a ranking_record_character for
   each of its 11 characters (index, course, itself) and ends at once. */
void ranking_record_start(void)
{
	uint8_t character_index = 0, initial = 0;
	uint32_t characters_left = 0, initial_place = 0, object = 0, record_offset = 0, record_time = 0, text_place = 0;
	uint8_t second_initial = 0;

	text_place = current_object + 0x80u;
	record_offset = OBJECT(current_object)->ranking_record.course;
	record_offset = (uint8_t)(record_offset * 6u);
	initial_place = 0x20E301u + record_offset;
	initial = mem_read8(initial_place);
	initial_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x80u)->ranking_record.text = initial;
	second_initial = mem_read8(initial_place);
	initial_place++;
	initial = second_initial;
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	AT8(OBJECT(text_place - 0x81u)->unk_80, 1) = initial;
	initial = mem_read8(initial_place);
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	AT8(OBJECT(text_place - 0x82u)->unk_80, 2) = initial;
	text_place++;
	AT8(OBJECT(text_place - 0x83u)->unk_80, 3) = 0u;
	record_time = course_record_times[record_offset >> 1];
	text_place++;
	time_to_text(record_time, text_place);
	characters_left = 0xBu;
	character_index = 0u;
	do
	{
		object = start_object();
		OBJECT(object)->ranking_record_character.ranking_record = current_object;
		OBJECT(object)->ranking_record_character.index = character_index;
		OBJECT(object)->ranking_record_character.course = OBJECT(current_object)->ranking_record.course;
		OBJECT(object)->handler = ranking_record_character_update_ADDRESS;
		character_index++;
		characters_left--;
	} while (characters_left != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* A character of a course's record line (font 2DE, palette 27): 0 its character from the record's text
   by index, at y 80 and x -A0 plus 16 pixels for each place of index; 1 slides in, 32 pixels a frame for
   7 frames (slide_steps), then starts a ranking_record_car (each of the 11 characters starts one, all at
   the same place); 2 index + 2 frames later (colour_delay) takes the colours of the record's car
   (ranking_record_character_palettes by its model); 3 58 frames after its start (frames_left) slides out
   to the left, 16 pixels a frame, and ends at x -20. */
void ranking_record_character_update(void)
{
	uint32_t animation = 0, character_index = 0, course = 0, object = 0, ranking_record = 0;
	uint8_t colour_delay = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	character_index = ranking_record_character_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_character.frames_left = 0x3Au;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x27u;
		animation = 0x2DEu;
		animation_start(animation);
		ranking_record = OBJECT(current_object)->ranking_record_character.ranking_record;
		character_index = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->depth_layer = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->frame = mem_read8(ranking_record + 0x80u + character_index);
		OBJECT(current_object)->x = ranking_record_character_xs[character_index];
		OBJECT(current_object)->y = ranking_record_character_ys[character_index];
		OBJECT(current_object)->ranking_record_character.slide_steps = 0u;
	case 1:
		OBJECT(current_object)->x += 0x20u;
		OBJECT(current_object)->ranking_record_character.slide_steps++;
		if (OBJECT(current_object)->ranking_record_character.slide_steps >= 7u)
		{
			object = start_object();
			OBJECT(object)->handler = ranking_record_car_update_ADDRESS;
			OBJECT(object)->ranking_record_car.course = OBJECT(current_object)->ranking_record_character.course;
			OBJECT(current_object)->state++;
		}
		character_index = (character_index & ~0xFFu) | OBJECT(current_object)->ranking_record_character.index;
		character_index = (character_index & ~0xFFu) | (uint8_t)(character_index + 2u);
		OBJECT(current_object)->ranking_record_character.colour_delay = character_index;
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			character_index = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = ranking_record_character_palettes[character_index];
			OBJECT(current_object)->state++;
		}
		break;
	case 2:
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			character_index = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = ranking_record_character_palettes[character_index];
			OBJECT(current_object)->state++;
		}
		frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
		OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
		if (frames_left != 1u)
		{
			character_index = draw_object(character_index);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0xFFE0u)
		{
			character_index = draw_object(character_index);
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
			character_index = draw_object(character_index);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(ranking_record_character_state_jumps[state]);
	}
	frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
	OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		character_index = draw_object(character_index);
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->x -= 0x10u;
	if (OBJECT(current_object)->x != 0xFFE0u)
	{
		character_index = draw_object(character_index);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* The record holder's car under the course's record line (animation car_model_turn_animations by the
   model of the course's record, palette car_model_palettes; size 50, y 88): drives in from the left 16
   pixels a frame to x 108 or more, turns round (frame 9 down to 4), waits 15 frames (wait_frames),
   drives back left 16 pixels a frame and ends at x 0. State 5 (drawn only) is not set by its own code. */
void ranking_record_car_update(void)
{
	uint32_t animation = 0, car_model = 0, course = 0;
	uint16_t wait_frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	car_model = ranking_record_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_car.wait_frames = 0xFu;
		OBJECT(current_object)->draw_mode = 0xAu;
		course = OBJECT(current_object)->ranking_record_car.course;
		car_model = course_records[(uint8_t)(course * 6u)];
		animation = car_model_turn_animations[car_model];
		OBJECT(current_object)->palette = car_model_palettes[car_model];
		animation_start(animation);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x88u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 1:
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 2:
		OBJECT(current_object)->frame--;
		if (OBJECT(current_object)->frame != 4u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 3:
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 5:
		car_model = draw_object(car_model);
		return;
	default:
		invalid_jump(ranking_record_car_state_jumps[state]);
	}
}

/* Counts the steps of the passing cars' path (traffic_path: size, x, y; closed by 0) into path_steps,
   then runs passing_traffic_update. */
void passing_traffic_start(void)
{
	uint8_t step_count = 0;
	uint32_t traffic_path_step = 0;

	traffic_path_step = traffic_path_ADDRESS;
	step_count = 0u;
	while (TRAFFIC_PATH_STEP(traffic_path_step)->size != 0u)
	{
		step_count++;
		traffic_path_step += 6u;
	}
	OBJECT(current_object)->passing_traffic.path_steps = step_count;
	OBJECT(current_object)->handler = passing_traffic_update_ADDRESS;
	passing_traffic_update();
}

/* Now and then a car passes on the road behind the certificate: by state, 0 one frame in 16 at random
   (random_number) starts a passing_car on the path's last step (path_steps), 1 then waits 10 frames
   (wait). */
void passing_traffic_update(void)
{
	uint32_t object = 0, random = 0;
	uint16_t wait = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		random = random_number();
		if ((random & 0xFu) == 0u)
		{
			object = start_object();
			OBJECT(object)->handler = passing_car_update_ADDRESS;
			OBJECT(object)->passing_car.step = OBJECT(current_object)->passing_traffic.path_steps;
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
		invalid_jump(passing_traffic_state_jumps[OBJECT(current_object)->state]);
	}
}

/* A car on the road behind the certificate (animation car_model_turn_animations by a random model,
   palette car_model_palettes; its sound, car_pass_sounds by the model, requested as it starts): state 0
   picks frame C and state 1, or frame 1 and state 2 when the random number is negative; then a frame a
   step it goes from the path's last step (traffic_path: size 370 near the bottom edge) to its first
   (size 5 on the horizon), at the path's x in state 1 or at 96 - x on the other side of the road in
   state 2; ends past the first step. */
void passing_car_update(void)
{
	uint8_t pass_sound = 0;
	uint16_t size = 0;
	uint32_t animation = 0, car_model = 0, mirrored_x = 0, path_size = 0, random_side = 0, step = 0, step_index = 0, traffic_path_step = 0;
	uint16_t near_step = 0, far_step = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		car_model = random_number();
		random_side = car_model;
		car_model &= 7u;
		animation = car_model_turn_animations[car_model];
		OBJECT(current_object)->palette = car_model_palettes[car_model];
		animation_start(animation);
		pass_sound = car_pass_sounds[car_model];
		request_sound(pass_sound);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 3u;
		OBJECT(current_object)->frame = 0xCu;
		OBJECT(current_object)->state++;
		if ((int32_t)random_side >= 0)
		{
			step = (uint16_t)OBJECT(current_object)->passing_car.step;
			near_step = step - 1u;
			step_index = near_step;
			if ((int16_t)near_step < 0)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			OBJECT(current_object)->passing_car.step = step_index;
			traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
			path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
			OBJECT(current_object)->width_scale = path_size;
			OBJECT(current_object)->height_scale = path_size;
			traffic_path_step += 2u;
			OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			path_size = draw_object(path_size);
			return;
		}
		OBJECT(current_object)->frame = 1u;
		OBJECT(current_object)->state++;
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		traffic_path_step += 2u;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	case 1:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		near_step = step - 1u;
		step_index = near_step;
		if ((int16_t)near_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = path_size;
		OBJECT(current_object)->height_scale = path_size;
		traffic_path_step += 2u;
		OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		path_size = draw_object(path_size);
		return;
	case 2:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		traffic_path_step += 2u;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	default:
		invalid_jump(passing_car_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Loads the picture of the course screen record_course (course_backgrounds picks it) into the screen's
   first tilemap: its register addresses for the screen (ranking_background_register_sets, into 80-AC),
   the tile bank (ranking_background_tile_banks), the palette (background_palettes, queue_palette_list),
   the four tilemap pages (background_tile_page_lists, with the screen's page numbers from
   ranking_background_page_numbers, in page_list; queue_tile_pages), start_palette_fades with
   background_palette_fades, zoom 1:1, zoom centre and the second tilemap's scroll 0, the pages, the
   scroll's start (ranking_background_scroll_xs: x 360 for screen 1, 4A0 for screen 2; y 60), its
   priority 1 and the second tilemap off in the mixer (ranking_mixer_addresses: screen 182 and 186;
   unk_94 bit 2); send_video_registers = FF to send the registers. */
void ranking_background_load(void)
{
	uint32_t background_number = 0, background_pages = 0, colour_list = 0, index = 0, mixer_place = 0, mixer_priority = 0, page_numbers = 0, page_place = 0, pages_to_copy = 0, palette_fades = 0, queued_pages = 0, ranking_background_registers = 0;
	uint32_t saved_fades = 0;

	ranking_background_registers = ranking_background_register_sets_ADDRESS + SCREEN(screen)->index * 0xCu * 4u;
	OBJECT(current_object)->ranking_background.tile_bank_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers)->tile_bank_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.pages_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x4u)->pages_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x8u)->scroll_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0xCu)->scroll_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x10u)->zoom_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x14u)->zoom_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x18u)->zoom_center_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x1Cu)->zoom_center_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.cleared_scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x20u)->cleared_scroll_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.cleared_scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x24u)->cleared_scroll_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.second_scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x28u)->second_scroll_x_address;
	OBJECT(current_object)->ranking_background.second_scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x28u)->second_scroll_y_address;
	send_video_registers = 0xFFu;
	background_number = course_backgrounds[SCREEN(screen)->record_course];
	mem_write8(OBJECT(current_object)->ranking_background.tile_bank_address, ranking_background_tile_banks[background_number]);
	saved_fades = background_palette_fades[background_number];
	colour_list = background_palettes[background_number];
	background_pages = background_tile_page_lists[background_number] + 0x10u;
	page_place = current_object + 0xB0u;
	page_numbers = ranking_background_page_numbers[SCREEN(screen)->index];
	pages_to_copy = 4u;
	do
	{
		mem_write32(page_place, mem_read32(background_pages) | (page_numbers & ~0xFFFFFFu));
		page_place += 4u;
		page_numbers = page_numbers << 8;
		pages_to_copy--;
		background_pages += 4u;
	} while (pages_to_copy != 0u);
	mem_write32(page_place, 0xFFFFFFFFu);
	queued_pages = current_object + 0xB0u;
	queue_tile_pages(queued_pages);
	queue_palette_list(colour_list);
	palette_fades = saved_fades;
	start_palette_fades(palette_fades);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_x_address, 0x200u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_y_address, 0x200u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_center_x_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.zoom_center_y_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.cleared_scroll_x_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.cleared_scroll_y_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.second_scroll_x_address, 0u);
	mem_write16(OBJECT(current_object)->ranking_background.second_scroll_y_address, 0u);
	index = SCREEN(screen)->index;
	mem_write32(OBJECT(current_object)->ranking_background.pages_address, byte_swap32(ranking_background_page_numbers[index]));
	mem_write16(OBJECT(current_object)->ranking_background.scroll_x_address, ranking_background_scroll_xs[index]);
	mem_write16(OBJECT(current_object)->ranking_background.scroll_y_address, 0x60u);
	mixer_place = ranking_mixer_addresses[index];
	mixer_priority = mem_read32(mixer_place);
	mixer_priority = (mixer_priority & ~0xFFFFu) | (mixer_priority & 0xFFF0u);
	mem_write32(mixer_place, (mixer_priority & ~0xFFu) | (uint8_t)(mixer_priority | 1u));
	mixer_place += 4u;
	mixer_priority = mem_read32(mixer_place);
	mem_write32(mixer_place, (mixer_priority & ~0xFFFFu) | (mixer_priority & 0xFFF0u));
	SCREEN(screen)->flags |= 4u;
}

/* Game mode 6, a game (check_game_start starts it at step 0) and the demo's race (game_mode_demo_race,
   from step 10), by game_step: 0 sound 21 (queue_sound), the screen's two lamps on (unk_F20),
   lcd_restart_frames cleared (the LCD's timer), a fade out, race game_over cleared; 1 when the fade is
   over, waits until the screen's player is in the list of the race's players (from byte 1F of the
   race_copy record of host_player), then the host itself (not on ISDN) writes 1 to the link board's
   80000A; 2 the car select: demo_flags bit 8 clear, the wheel motor off (wheel_motor), game_start_time
   from the backup RAM's seconds (701F10), a game counted on the seat (701F14 + 2 * index,
   bookkeeping_increment16), C00074 = game_start_c00074_bytes by index, the sound off
   (reset_screen_sound) unless the entry countdown's music plays (entry_countdown_shown), clear_screen,
   clear_race_state, palette car_select_palette, in_game, host_record (the host's race_copy record), the
   stages (screen first_stage-A6) 0, the seat's lamps on (checkpoint_lamp 2, leader_lamp 8),
   car_select_start started, a fade in, the screen on; 3 the car select (car_select_run goes on to 4); 4
   a fade out, entry_countdown_shown cleared, the lamps on; 5 when the fade is over, race ready cleared,
   silence_and_clear_screen, the screen off (C0001C = 4), the stage's palette (queue_stage_load), the
   screen's car (object_handlers_346C: player_car_update, its place from screen 320 + player), step 23;
   23 once the car has published its state (race_copy published): race_running, the race's objects
   (object_handlers_3474; the demo's object_handlers_34C4), 24; 24 and 25 a frame each; 26 step 11, the
   input ring's places to 0, the screen on; 11 a fade in, step 6; 6 when it is over, 7; 7 the race, until
   its ending is over (flags bit 1, race_car_start_ending): race_running cleared, a fade out, 8; 8 when
   it is over, the lamps on, 9; 9 the text and the tilemaps off (clear_text_layer, reset_tile_layers), no
   objects, step 12; 12 the route map: short of the goal (race_copy finished clear) the sound off, the
   song title Meaning of the Light (start_song_title with 003504) and sound 0B; palette
   route_map_palette, route_map_update (object_handlers_34EC), a fade in; 13 when it is over, 14; 14 the
   map until START (buttons_pressed bit 6) or route_map_update sets 16; 15 (nothing sets it) 16; 16 with
   a new course record (course_record_beaten) a fade out and 18, else 21; 18 when the fade is over, no
   objects, name_entry_update started, 19; 19 a fade in, 20; 20 the name entry (name_entry_update sets
   21); 21 set_player_ready and 22; 21 and 22 once every player of the race is ready (players_not_ready):
   a fade out, fade_out_music, 17; 17 when the fade is over, the game's end: the time played (the
   backup's seconds less game_start_time) added to the seat's total (701F18 + 4 * index) and kept if the
   longest (701F20) or the shortest (701F22), a game counted at the stage level reached (screen
   current_stage_level, 6 at the goal: 701F24 + 2 * it), C00074 = game_end_c00074_bytes then
   game_end_c00074_next_bytes by index, sound 1E, the sound and the tilemaps off, in_game, screen
   race_started and unk_318 cleared, the race record's car part cleared (race_record_clear), no objects,
   race game_over (the screens in the attract modes go to the ranking, race_entries_update), the lamps
   off, clear_lcd, 28; 28 game mode 5 (the ranking). The demo: 10 silence_and_clear_screen,
   clear_race_state, the stage and the car model from attract_demo_races by attract_car (the stage into
   screen first_stage, A1, A3, A5), host_record its own record at screen + 490 (demo_player_count 1,
   demo_players this screen's player, demo_stage), the model into car model and race model, palette
   title_palette, then as 27; 27 waits until race_copy published is clear, then as 5. */
void game_mode_play(void)
{
	uint32_t zero = 0;
	uint8_t model = 0, player = 0, sound_code = 0, stage = 0;
	uint32_t bookkeeping_entry = 0, car = 0, colour_block = 0, demo_race_choice = 0, demo_record = 0, handler_list = 0, index = 0, lamp_place = 0, object = 0, race = 0, racers_left = 0, song_title = 0, stage_level = 0, time_played = 0;
	uint8_t racer = 0;
	uint32_t flags = 0;

	switch (SCREEN(screen)->game_step)
	{
	case 0:
		sound_code = 0x21u;
		queue_sound(sound_code);
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
			race = race_copy_offsets[SCREEN(screen)->host_player] + 0x20E01Fu;
			racers_left = RACE(race - 0x1Fu)->racer_count;
			race++;
			if (racers_left == 0u)
				return;
			player = SCREEN(screen)->player;
			do
			{
				racer = mem_read8(race);
				zero = (uint8_t)(racer - player) == 0u;
				race++;
				racers_left--;
			} while ((racers_left != 0u) & (racer != player));
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
		wheel_motor[SCREEN(screen)->index] = 0u;
		SCREEN(screen)->game_start_time = mem_read32(SOUND_701F10);
		bookkeeping_entry = SOUND_RAM + 0x1F14u + SCREEN(screen)->index * 2u;
		bookkeeping_increment16(bookkeeping_entry);
		mem_write8(IO_C00074, game_start_c00074_bytes[SCREEN(screen)->index]);
		if (SCREEN(screen)->entry_countdown_shown == 0u)
			reset_screen_sound();
		clear_screen();
		clear_race_state();
		colour_block = car_select_palette;
		queue_palette(colour_block);
		SCREEN(screen)->in_game = 0xFFu;
		SCREEN(screen)->host_record = race_copy_offsets[SCREEN(screen)->host_player] + GAME_VARIABLES;
		SCREEN(screen)->current_stage_level = 0u;
		SCREEN(screen)->first_stage = 0u;
		SCREEN(screen)->route_stage_level = 0u;
		SCREEN(screen)->built_stage = 0u;
		SCREEN(screen)->current_stage = 0u;
		SCREEN(screen)->route_stage = 0u;
		SCREEN(screen)->built_stage_level = 0u;
		lamp_place = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		mem_write32(lamp_place + 0xC0u, 2u);
		mem_write32(lamp_place + 0xC8u, 8u);
		object = start_object();
		OBJECT(object)->handler = car_select_start_ADDRESS;
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
		lamp_place = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		mem_write32(lamp_place + 0xC0u, 2u);
		mem_write32(lamp_place + 0xC8u, 8u);
		return;
	case 10:
		silence_and_clear_screen();
		clear_race_state();
		demo_race_choice = attract_demo_races_ADDRESS + SCREEN(screen)->attract_car * 2u;
		stage = DEMO_RACE_CHOICE(demo_race_choice)->stage;
		SCREEN(screen)->first_stage = stage;
		SCREEN(screen)->built_stage = stage;
		SCREEN(screen)->route_stage = stage;
		SCREEN(screen)->current_stage_level = 0u;
		SCREEN(screen)->route_stage_level = stage;
		SCREEN(screen)->built_stage_level = 0u;
		demo_record = screen + 0x490u;
		SCREEN(screen)->host_record = demo_record;
		SCREEN(demo_record - 0x490u)->demo_player_count = 1u;
		SCREEN(demo_record - 0x490u)->demo_players = SCREEN(screen)->player;
		SCREEN(demo_record - 0x490u)->demo_stage = stage;
		model = DEMO_RACE_CHOICE(demo_race_choice)->model;
		CAR(player_car)->model = model;
		RACE(SCREEN(screen)->race)->model = model;
		SCREEN(screen)->game_step = 0x1Bu;
		colour_block = title_palette;
		queue_palette(colour_block);
		if (RACE(SCREEN(screen)->race_copy)->published != 0u)
			return;
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		queue_stage_load();
		handler_list = 0x346Cu;
		car = start_object_list(handler_list);
		OBJECT(car)->player_car.place = mem_read8(screen + 0x320u + SCREEN(screen)->player);
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
		handler_list = 0x346Cu;
		car = start_object_list(handler_list);
		OBJECT(car)->player_car.place = mem_read8(screen + 0x320u + SCREEN(screen)->player);
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
		handler_list = 0x346Cu;
		car = start_object_list(handler_list);
		OBJECT(car)->player_car.place = mem_read8(screen + 0x320u + SCREEN(screen)->player);
		SCREEN(screen)->game_step = 0x17u;
		return;
	case 23:
		if (RACE(SCREEN(screen)->race_copy)->published == 0u)
			return;
		SCREEN(screen)->race_running = 0xFFu;
		handler_list = 0x3474u;
		if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
			handler_list = 0x34C4u;
		start_object_list(handler_list);
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
			lamp_place = GAME_VARIABLES + SCREEN(screen)->index * 4u;
			mem_write32(lamp_place + 0xC0u, 2u);
			mem_write32(lamp_place + 0xC8u, 8u);
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
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
		{
			reset_screen_sound();
			song_title = 0x3504u;
			start_song_title(song_title);
			sound_code = 0xBu;
			request_sound(sound_code);
		}
		colour_block = route_map_palette;
		queue_palette(colour_block);
		handler_list = 0x34ECu;
		start_object_list(handler_list);
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
			object = start_object();
			OBJECT(object)->handler = name_entry_update_ADDRESS;
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
		time_played = mem_read32(SOUND_701F10) - SCREEN(screen)->game_start_time;
		bookkeeping_entry = SOUND_RAM + 0x1F18u + SCREEN(screen)->index * 4u;
		bookkeeping_add32(bookkeeping_entry, time_played);
		if (mem_read16(SOUND_701F20) < (uint16_t)time_played)
		{
			bookkeeping_entry = SOUND_701F20;
			bookkeeping_set16(bookkeeping_entry, (uint16_t)time_played);
		}
		if ((uint16_t)time_played < mem_read16(SOUND_701F22))
		{
			bookkeeping_entry = SOUND_701F22;
			bookkeeping_set16(bookkeeping_entry, (uint16_t)time_played);
		}
		stage_level = SCREEN(screen)->current_stage_level;
		if (RACE(SCREEN(screen)->race_copy)->finished != 0u)
			stage_level = 6u;
		bookkeeping_entry = SOUND_RAM + 0x1F24u + stage_level * 2u;
		bookkeeping_increment16(bookkeeping_entry);
		index = SCREEN(screen)->index;
		mem_write8(IO_C00074, game_end_c00074_bytes[index]);
		mem_write8(IO_C00074, game_end_c00074_next_bytes[index]);
		sound_code = 0x1Eu;
		request_sound(sound_code);
		reset_screen_sound();
		reset_tile_layers();
		SCREEN(screen)->race_started = 0u;
		SCREEN(screen)->in_game = 0u;
		SCREEN(screen)->is_host = 0u;
		race_record_clear();
		clear_objects();
		SCREEN(screen)->game_step = 0x1Cu;
		RACE(SCREEN(screen)->race)->game_over = 0xFFu;
		lamp_place = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		mem_write32(lamp_place + 0xC0u, 0u);
		mem_write32(lamp_place + 0xC8u, 0u);
		clear_lcd();
		return;
	case 28:
		AS32(SCREEN(screen)->game_mode) = 5u;
		return;
	default:
		invalid_jump(game_step_jumps[SCREEN(screen)->game_step]);
	}
}

/* The screen's sound off (reset_screen_sound), then clear_screen. */
void silence_and_clear_screen(void)
{
	reset_screen_sound();
	clear_screen();
}

/* Clears what the screen shows: no objects (clear_objects), its tilemaps off in the mixer
   (reset_tile_layers), no sprites (clear_sprite_group_count, screen sprite_groups) and the text layer
   cleared (clear_text_layer). */
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
	move_halfwords(player_car, zero_block_ADDRESS, 0x180u);
	move_halfwords(screen + 0x330u, zero_block_ADDRESS, 0xB0u);
	move_string(zero_block_ADDRESS, 0x80u, screen + 0x600u, 0x80u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* The map of the route after the race (game step 12, object_handlers_34EC); keeps "Thank you for your
   playing." scrolling on the LCD (F40 = 005014). By state: 0 copies its screen's tilemap register
   addresses (route_map_register_sets) into 80-BC, first_branch from screen route_branches, the
   background (set_up_background with 004868 by screen and first_branch: the routes on tilemap 0 over the
   world on the second), the palette route_map_background_palette (scene_palettes[2C]) (queue_palette); 1
   zoom 1:1 about the middle of the screen, sea_colour black, 30 frames (timer); 2 the world's tilemap
   off, sea_colour 7EE0; 3 zooms in (zoom + 20 a frame for 9 frames, 200 being 1:1), then the close-up
   map (pages route_map_close_up_pages) at its start (x scroll route_map_start_scroll_xs by first_branch,
   y 8C), starts route_map_car and the 32 route_map_landmark objects; 4 checkpoint_legs_left from the
   car's unk_D0, unk_12 from screen current_stage_level, stage 0; 5-14 the car drives the route leg by
   leg (leg_frames frames each): 22 frames up from the start to the first checkpoint (the car moves, the
   map stands), then 6 frames of 8 pixels sideways (first_branch picks the side), 11 of 5, then for each
   stage 10 frames of 4 up or down by the branch taken (screen route_branches + stage - 1) and 15 of 7
   sideways; on these legs the map scrolls under the car (the object's x and y move the other way, for
   the objects on the map), screen route_map_signals telling the car its way (bits 0 down, 1 up, 2 right,
   3 left, 5 the map at its edge, 7 standing); a stage_result_line starts where each stage begins (5, 7,
   11), a route_map_sign (C.P) and stage + 1 where a checkpoint leg ends; when its count of legs (unk_12
   for the first leg of a stage, checkpoint_legs_left for the others) is 0 at a leg's start, the race
   ended on it: last_leg_progress and leg_frames are worked out from the race_copy record's unk_42, the
   car's unk_4A and the stage lengths at 700 of the screen, and unk_5800 bit 6 ends the drive after it;
   at the end (stage 6 or a cut leg) the car stands (bit 7), FINISH (route_map_sign 1) if the goal was
   reached (race_copy finished), 60 frames; 15 the routes (pages route_map_routes_pages) at zoom 2x where
   the close-up map was, the steps of the zoom out into x and y, unk_5800 bit 4 (the car, the landmarks
   and the signs end); 16 zooms out over 9 frames to 1:1 and the whole route (x scroll
   route_map_whole_scroll_xs by first_branch, y 70), then the world's tilemap again (pages
   route_map_world_pages); 17 sea_colour black, starts 00463C and an 004240 for each leg (stage + 1, at
   most 6), DJ message 38, 90 frames; 18 DJ message 39 once the DJ is quiet (screen dj_mouth_shapes); 19
   when the 90 frames are over sets the game's next step (screen game_mode byte 2 = 0x10 (step 16)) and
   starts 0046BB; 20 nothing. */
void route_map_update(void)
{
	bool L_003BFC = false, L_003C2D = false;
	uint8_t dj_message = 0, landmark_index = 0, leg_number = 0;
	uint32_t background_setup = 0, branch_road_length = 0, colour_block = 0, landmarks_left = 0, leg_distance = 0, legs_left = 0, lengths_place = 0, object = 0, route_branches = 0, route_map_registers = 0, setup_base = 0, stage_length = 0, stage_lengths = 0;
	uint8_t timer = 0, timer_2 = 0, start_leg_frames = 0, timer_3 = 0, turn_leg_frames = 0, approach_leg_frames = 0, branch_leg_frames = 0, stage_leg_frames = 0, timer_4 = 0, timer_5 = 0;
	uint32_t jump_target = 0, zoom_x_address = 0, zoom_y_address = 0, scroll_x_address = 0, scroll_x_address_2 = 0, scroll_y_address = 0, scroll_x_address_3 = 0, scroll_y_address_2 = 0, scroll_x_address_4 = 0, scroll_y_address_3 = 0, scroll_x_address_5 = 0, scroll_x_address_6 = 0, scroll_x_address_7 = 0, scroll_x_address_8 = 0, scroll_y_address_4 = 0, scroll_x_address_9 = 0, scroll_y_address_5 = 0, scroll_x_address_10 = 0, scroll_x_address_11 = 0, scroll_x_address_12 = 0, zoom_x_address_2 = 0, zoom_y_address_2 = 0, scroll_x_address_13 = 0, scroll_y_address_6 = 0, second_scroll_x_address = 0, second_scroll_y_address = 0;

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
									jump_target = L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003C2D ? 0x383Au : route_map_state_jumps[OBJECT(current_object)->state];
									switch (jump_target)
									{
									case 0x003594u:
										SCREEN(screen)->route_map_signals = 0u;
										route_map_registers = route_map_register_sets_ADDRESS + ((uint32_t)SCREEN(screen)->index << 4) * 4u;
										OBJECT(current_object)->route_map.pages_address = ROUTE_MAP_REGISTERS(route_map_registers)->pages_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x4u)->scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x8u)->scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0xCu)->zoom_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x10u)->zoom_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_center_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x14u)->zoom_center_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_center_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x18u)->zoom_center_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.register_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x1Cu)->register_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.register_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x20u)->register_scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.tile_bank_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x24u)->tile_bank_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_pages_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x28u)->second_pages_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x2Cu)->second_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x30u)->second_scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x34u)->second_register_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x38u)->second_register_scroll_y_address;
										OBJECT(current_object)->route_map.second_tile_bank_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x38u)->second_tile_bank_address;
										setup_base = SCREEN(screen)->index * 5u * 2u;
										route_branches = SCREEN(screen)->route_branches;
										OBJECT(current_object)->route_map.first_branch = route_branches;
										background_setup = 0x4868u + (setup_base + route_branches * 5u) * 4u;
										set_up_background(background_setup);
										colour_block = route_map_background_palette;
										queue_palette(colour_block);
										OBJECT(current_object)->state++;
										return;
									case 0x003644u:
										OBJECT(current_object)->route_map.sea_colour_address = route_map_sea_colour_addresses[SCREEN(screen)->index];
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
										send_video_registers = 0xFFu;
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
										send_video_registers = 0xFFu;
										timer_2 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_2 - 1u;
										if (timer_2 != 1u)
											return;
										mem_write32(OBJECT(current_object)->route_map.pages_address, route_map_close_up_pages[SCREEN(screen)->index]);
										mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x200u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_x_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.zoom_center_y_address, 0u);
										mem_write16(OBJECT(current_object)->route_map.scroll_x_address, route_map_start_scroll_xs[OBJECT(current_object)->route_map.first_branch]);
										mem_write16(OBJECT(current_object)->route_map.scroll_y_address, 0x8Cu);
										mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
										mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
										send_video_registers = 0xFFu;
										SCREEN(screen)->skip_scroll_update = 0xFFu;
										object = start_object();
										OBJECT(object)->handler = route_map_car_update_ADDRESS;
										OBJECT(object)->route_map_car.model = CAR(player_car)->model;
										OBJECT(object)->route_map_car.route_map = current_object;
										SCREEN(screen)->route_map_signals |= 0x80u;
										OBJECT(current_object)->x = 0xA0u;
										OBJECT(current_object)->y = 0x70u;
										landmarks_left = 0x20u;
										landmark_index = 0u;
										do
										{
											object = start_object();
											OBJECT(object)->handler = route_map_landmark_update_ADDRESS;
											OBJECT(object)->route_map_landmark.index_then_half_rate = landmark_index;
											OBJECT(object)->route_map_landmark.route_map = current_object;
											landmark_index++;
											landmarks_left--;
										} while (landmarks_left != 0u);
										OBJECT(current_object)->state++;
										return;
									case 0x0037A7u:
										OBJECT(current_object)->route_map.first_legs_left = SCREEN(screen)->current_stage_level;
										OBJECT(current_object)->route_map.checkpoint_legs_left = CAR(player_car)->forks_taken;
										OBJECT(current_object)->route_map.stage = 0u;
										OBJECT(current_object)->state++;
										return;
									case 0x0037BEu:
										SCREEN(screen)->route_map_signals &= ~0x80u;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0x16;
											OBJECT(current_object)->state++;
										}
										else
										{
											OBJECT(object)->stage_result_line.last = 1u;
											leg_distance = mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7));
											stage_lengths = SCREEN(screen)->stage_lengths;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 3, stage_lengths);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(((leg_distance * 5u) << 2) + leg_distance * 2u, stage_lengths);
											SCREEN(screen)->route_map_signals |= 0x40u;
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
																		start_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
																		OBJECT(current_object)->route_map.leg_frames = start_leg_frames;
																		if ((int8_t)start_leg_frames >= 0)
																		{
																			SCREEN(screen)->route_map_signals |= 2u;
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
										if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003C2D || (SCREEN(screen)->route_map_signals & 0x40u) != 0u)
										{
											if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || !L_003C2D)
											{
												SCREEN(screen)->route_map_signals |= 0x80u;
												if (RACE(SCREEN(screen)->race_copy)->finished != 0u)
												{
													object = start_object();
													OBJECT(object)->handler = route_map_sign_update_ADDRESS;
													OBJECT(object)->route_map_sign.route_map = current_object;
													OBJECT(object)->route_map_sign.kind = 1u;
												}
												OBJECT(current_object)->route_map.timer = 0x3Cu;
												OBJECT(current_object)->state = 0xFu;
											}
											timer_3 = OBJECT(current_object)->route_map.timer;
											OBJECT(current_object)->route_map.timer = timer_3 - 1u;
											if (timer_3 == 1u)
											{
												mem_write32(OBJECT(current_object)->route_map.pages_address, route_map_routes_pages[SCREEN(screen)->index]);
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
												OBJECT(current_object)->x = route_map_whole_scroll_xs[OBJECT(current_object)->route_map.first_branch];
												OBJECT(current_object)->y = 0x70u;
												OBJECT(current_object)->x -= mem_read16(OBJECT(current_object)->route_map.scroll_x_address);
												OBJECT(current_object)->y -= mem_read16(OBJECT(current_object)->route_map.scroll_y_address);
												OBJECT(current_object)->x = (int16_t)OBJECT(current_object)->x >> 3;
												OBJECT(current_object)->y = (int16_t)OBJECT(current_object)->y >> 3;
												send_video_registers = 0xFFu;
												SCREEN(screen)->skip_scroll_update = 0xFFu;
												SCREEN(screen)->route_map_signals |= 0x10u;
												OBJECT(current_object)->route_map.timer = 9u;
												OBJECT(current_object)->state++;
											}
											return;
										}
										object = start_object();
										OBJECT(object)->handler = route_map_sign_update_ADDRESS;
										OBJECT(object)->route_map_sign.route_map = current_object;
										OBJECT(object)->route_map_sign.kind = 0u;
										SCREEN(screen)->route_map_signals &= ~2u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003877u:
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											OBJECT(object)->stage_result_line.last = 1u;
										if (OBJECT(current_object)->route_map.first_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 6;
											OBJECT(current_object)->state++;
										}
										else
										{
											lengths_place = screen + 0x700u;
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - mem_read16(lengths_place + SCREEN(screen)->current_stage * 2u);
											branch_road_length = SCREEN(lengths_place - 0x700u)->branch_road_length;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, branch_road_length);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 3u * 2u, branch_road_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003908u:
										turn_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = turn_leg_frames;
										if ((int8_t)turn_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												scroll_x_address_5 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_5, mem_read16(scroll_x_address_5) - 8u);
												OBJECT(current_object)->x += 8u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											scroll_x_address_6 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_6, mem_read16(scroll_x_address_6) + 8u);
											OBJECT(current_object)->x -= 8u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.first_legs_left--;
										OBJECT(current_object)->state++;
									case 0x00394Eu:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xB;
											OBJECT(current_object)->state++;
										}
										else
										{
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											stage_length = mem_read16(screen + 0x700u + SCREEN(screen)->current_stage * 2u);
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, stage_length);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 5u * 2u + leg_distance, stage_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x0039B4u:
										approach_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = approach_leg_frames;
										if ((int8_t)approach_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												scroll_x_address_7 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_7, mem_read16(scroll_x_address_7) - 5u);
												OBJECT(current_object)->x += 5u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											scroll_x_address_8 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_8, mem_read16(scroll_x_address_8) + 5u);
											OBJECT(current_object)->x -= 5u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = route_map_sign_update_ADDRESS;
										OBJECT(object)->route_map_sign.route_map = current_object;
										OBJECT(object)->route_map_sign.kind = 0u;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003A14u:
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											OBJECT(object)->stage_result_line.last = 1u;
										if (OBJECT(current_object)->route_map.first_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xA;
											OBJECT(current_object)->state++;
										}
										else
										{
											lengths_place = screen + 0x700u;
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - mem_read16(lengths_place + SCREEN(screen)->current_stage * 2u);
											branch_road_length = SCREEN(lengths_place - 0x700u)->branch_road_length;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, branch_road_length);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 5u * 2u, branch_road_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003AA5u:
										branch_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = branch_leg_frames;
										if ((int8_t)branch_leg_frames >= 0)
										{
											if (mem_read8(screen + 0x34Au + (OBJECT(current_object)->route_map.stage - 1u)) != 0u)
											{
												if (OBJECT(current_object)->route_map.first_branch != 0u)
												{
													SCREEN(screen)->route_map_signals |= 1u;
													scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
													mem_write16(scroll_y_address_4, mem_read16(scroll_y_address_4) + 4u);
													scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
													mem_write16(scroll_x_address_9, mem_read16(scroll_x_address_9) - 1u);
													OBJECT(current_object)->y -= 4u;
													OBJECT(current_object)->x++;
													return;
												}
												SCREEN(screen)->route_map_signals |= 2u;
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
												SCREEN(screen)->route_map_signals |= 2u;
												scroll_y_address_5 = OBJECT(current_object)->route_map.scroll_y_address;
												mem_write16(scroll_y_address_5, mem_read16(scroll_y_address_5) - 4u);
												scroll_x_address_10 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_10, mem_read16(scroll_x_address_10) + 1u);
												OBJECT(current_object)->y += 4u;
												OBJECT(current_object)->x--;
												return;
											}
											SCREEN(screen)->route_map_signals |= 1u;
											scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
											mem_write16(scroll_y_address_4, mem_read16(scroll_y_address_4) + 4u);
											scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_9, mem_read16(scroll_x_address_9) - 1u);
											OBJECT(current_object)->y -= 4u;
											OBJECT(current_object)->x++;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFFCu;
										OBJECT(current_object)->route_map.first_legs_left--;
										OBJECT(current_object)->state++;
									case 0x003B16u:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xF;
											OBJECT(current_object)->state++;
										}
										else
										{
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											stage_length = mem_read16(screen + 0x700u + SCREEN(screen)->current_stage * 2u);
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 3, stage_length);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 3u * 5u, stage_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003B77u:
										stage_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = stage_leg_frames;
										if ((int8_t)stage_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												if ((int16_t)mem_read16(OBJECT(current_object)->route_map.scroll_x_address) <= 0x10)
												{
													SCREEN(screen)->route_map_signals |= 0x20u;
													return;
												}
												scroll_x_address_11 = OBJECT(current_object)->route_map.scroll_x_address;
												mem_write16(scroll_x_address_11, mem_read16(scroll_x_address_11) - 7u);
												OBJECT(current_object)->x += 7u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											if ((int16_t)mem_read16(OBJECT(current_object)->route_map.scroll_x_address) >= 0x2A0)
											{
												SCREEN(screen)->route_map_signals |= 0x20u;
												return;
											}
											scroll_x_address_12 = OBJECT(current_object)->route_map.scroll_x_address;
											mem_write16(scroll_x_address_12, mem_read16(scroll_x_address_12) + 7u);
											OBJECT(current_object)->x -= 7u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										if (OBJECT(current_object)->route_map.stage != 6u)
										{
											object = start_object();
											OBJECT(object)->handler = route_map_sign_update_ADDRESS;
											OBJECT(object)->route_map_sign.route_map = current_object;
											OBJECT(object)->route_map_sign.kind = 0u;
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
										send_video_registers = 0xFFu;
										timer_4 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_4 - 1u;
										if (timer_4 == 1u)
										{
											mem_write16(OBJECT(current_object)->route_map.zoom_x_address, 0x200u);
											mem_write16(OBJECT(current_object)->route_map.zoom_y_address, 0x200u);
											mem_write16(OBJECT(current_object)->route_map.scroll_x_address, route_map_whole_scroll_xs[OBJECT(current_object)->route_map.first_branch]);
											mem_write16(OBJECT(current_object)->route_map.scroll_y_address, 0x70u);
											mem_write16(OBJECT(current_object)->route_map.register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
											mem_write32(OBJECT(current_object)->route_map.second_pages_address, route_map_world_pages[SCREEN(screen)->index]);
											mem_write16(OBJECT(current_object)->route_map.second_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.second_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.scroll_y_address));
											second_scroll_x_address = OBJECT(current_object)->route_map.second_scroll_x_address;
											mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) - 0x88u);
											second_scroll_y_address = OBJECT(current_object)->route_map.second_scroll_y_address;
											mem_write16(second_scroll_y_address, mem_read16(second_scroll_y_address) - 0x70u);
											mem_write16(OBJECT(current_object)->route_map.second_register_scroll_x_address, mem_read16(OBJECT(current_object)->route_map.second_scroll_x_address));
											mem_write16(OBJECT(current_object)->route_map.second_register_scroll_y_address, mem_read16(OBJECT(current_object)->route_map.second_scroll_y_address));
											send_video_registers = 0xFFu;
											OBJECT(current_object)->state++;
										}
										return;
									case 0x003D94u:
										mem_write16(OBJECT(current_object)->route_map.sea_colour_address, 0u);
										object = start_object();
										OBJECT(object)->handler = route_map_direction_sign_show_ADDRESS;
										legs_left = OBJECT(current_object)->route_map.stage;
										if (legs_left != 6u)
											legs_left++;
										leg_number = 0u;
										do
										{
											object = start_object();
											OBJECT(object)->handler = route_map_leg_show_ADDRESS;
											OBJECT(object)->route_map_leg.leg = leg_number;
											OBJECT(object)->route_map_leg.progress = OBJECT(current_object)->route_map.last_leg_progress;
											OBJECT(object)->route_map_leg.map = current_object;
											leg_number++;
											legs_left--;
										} while (legs_left != 0u);
										dj_message = 0x38u;
										start_dj_message(dj_message);
										OBJECT(current_object)->route_map.timer = 0x5Au;
										OBJECT(current_object)->state++;
										return;
									case 0x003DE9u:
										if (SCREEN(screen)->dj_mouth_shapes == 0u)
										{
											dj_message = 0x39u;
											start_dj_message(dj_message);
											OBJECT(current_object)->state++;
										}
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										object = start_object();
										OBJECT(object)->handler = route_map_wait_banner_show_ADDRESS;
										OBJECT(current_object)->state++;
										return;
									case 0x003DFCu:
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										object = start_object();
										OBJECT(object)->handler = route_map_wait_banner_show_ADDRESS;
										OBJECT(current_object)->state++;
										return;
									case 0x003E1Au:
										return;
									default:
										invalid_jump(jump_target);
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

/* The player's car on the route map (animation route_map_car_animations by the car's model, palette
   route_map_car_palettes, size 50): 0 at x 8C, y C3 with frame C; 1 until the route map's stage is not
   0, drives up 4 pixels and right 1 a frame unless the map says it stands (screen route_map_signals bit
   7); 2 stays where it is while the map scrolls under it, its frame by the way the map says it goes (bit
   2 frame 9, bit 3 frame 4, bit 0 frame 6, bit 1 frame C), moving itself 7 pixels a frame sideways when
   the map is at its edge (bit 5); ends when the map zooms out (bit 4). Its depth layer follows its y
   (map_depth_from_y). */
void route_map_car_update(void)
{
	uint32_t animation = 0, car_model = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		car_model = OBJECT(current_object)->route_map_car.model;
		animation = route_map_car_animations[car_model];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_car_palettes[car_model];
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->x = 0x8Cu;
		OBJECT(current_object)->y = 0xC3u;
		OBJECT(current_object)->frame = 0xCu;
		map_depth_from_y();
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if (OBJECT(OBJECT(current_object)->route_map_car.route_map)->route_map.stage != 0u)
		{
			OBJECT(current_object)->state++;
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 0x80u) != 0u)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->y -= 4u;
		OBJECT(current_object)->x++;
		map_depth_from_y();
		routine = draw_object(routine);
		return;
	case 2:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 0x80u) != 0u)
		{
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 4u) != 0u)
		{
			OBJECT(current_object)->frame = 9u;
			if ((SCREEN(screen)->route_map_signals & 0x20u) != 0u)
				OBJECT(current_object)->x += 7u;
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 8u) != 0u)
		{
			OBJECT(current_object)->frame = 4u;
			if ((SCREEN(screen)->route_map_signals & 0x20u) != 0u)
			{
				OBJECT(current_object)->x -= 7u;
				map_depth_from_y();
				routine = draw_object(routine);
				return;
			}
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 1u) == 0u)
		{
			if ((SCREEN(screen)->route_map_signals & 2u) == 0u)
			{
				map_depth_from_y();
				routine = draw_object(routine);
				return;
			}
			OBJECT(current_object)->frame = 0xCu;
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->frame = 6u;
		map_depth_from_y();
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_car_state_jumps[state]);
	}
}

/* A sign on the route map, by kind: 0 C.P where the car passed a checkpoint (animation 2F9), 1 FINISH at
   the goal (2FA); palette 2CF, depth layer 195. State 0: x_offset and y_offset are the middle of the
   screen (A0, 70 less 8) less the map's place, where the car is; kind 1 goes to state 2; 1 follows the
   map (its x and y plus the offsets) and turns (animation_advance); 2 FINISH at the goal's place on the
   map (the map's x plus route_map_goal_xs by the goal, screen current_stage - 15; y 5C), drawn 4 frames
   of 8 (blink); both end when the map zooms out (screen route_map_signals bit 4). */
void route_map_sign_update(void)
{
	uint32_t animation = 0, goal = 0, sign_kind = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	sign_kind = route_map_sign_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		sign_kind = OBJECT(current_object)->route_map_sign.kind;
		animation = route_map_sign_animations[sign_kind];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_sign_palettes[sign_kind];
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
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		sign_kind = draw_object(sign_kind);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		sign_kind = draw_object(sign_kind);
		return;
	case 2:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		goal = SCREEN(screen)->current_stage - 0x15u;
		OBJECT(current_object)->route_map_sign.x_offset = route_map_goal_xs[goal];
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y = 0x5Cu;
		OBJECT(current_object)->route_map_sign.blink--;
		if ((OBJECT(current_object)->route_map_sign.blink & 4u) == 0u)
			return;
		goal = draw_object(goal);
		return;
	default:
		invalid_jump(route_map_sign_state_jumps[state]);
	}
}

/* One of the 32 animated pictures on the close-up route map (people, animals and vehicles by the
   landmarks): 0 from its record (route_map_landmarks: 16 * the index in 9, the second set of 32 (+ 200)
   when screen route_branches is 1): animation, palette, size, x_offset and y_offset from the map's
   place, then field 9 from the record (set for one record: it then shows only every other frame); 1
   follows the map (its x and y plus the offsets), animation_advance and draw_object, with field 9 set
   only when toggle comes round; ends when the map zooms out (screen route_map_signals bit 4). Its depth
   layer follows its y (map_depth_from_y). */
void route_map_landmark_update(void)
{
	uint32_t animation = 0, landmark = 0, record_offset = 0, routine = 0;
	uint8_t frame_toggle = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_landmark_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		record_offset = ((uint32_t)OBJECT(current_object)->route_map_landmark.index_then_half_rate << 4) + ((uint32_t)SCREEN(screen)->route_branches << 9);
		landmark = route_map_landmarks_ADDRESS + record_offset;
		animation = ROUTE_MAP_LANDMARK_RECORD(landmark)->animation;
		landmark += 2u;
		animation_start(animation);
		OBJECT(current_object)->palette = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x2u)->palette;
		landmark += 2u;
		OBJECT(current_object)->width_scale = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x4u)->width_scale;
		landmark += 2u;
		OBJECT(current_object)->height_scale = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x6u)->height_scale;
		landmark += 2u;
		OBJECT(current_object)->route_map_landmark.x_offset = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x8u)->x_offset;
		landmark += 2u;
		OBJECT(current_object)->route_map_landmark.y_offset = ROUTE_MAP_LANDMARK_RECORD(landmark - 0xAu)->y_offset;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		OBJECT(current_object)->route_map_landmark.index_then_half_rate = (uint8_t)ROUTE_MAP_LANDMARK_RECORD(landmark - 0xAu)->half_rate;
		OBJECT(current_object)->state++;
		map_depth_from_y();
		record_offset = draw_object(record_offset);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		if (OBJECT(current_object)->route_map_landmark.index_then_half_rate != 0u)
		{
			OBJECT(current_object)->route_map_landmark.toggle++;
			frame_toggle = OBJECT(current_object)->route_map_landmark.toggle & 1u;
			OBJECT(current_object)->route_map_landmark.toggle = frame_toggle;
			if (frame_toggle != 0u)
				return;
		}
		map_depth_from_y();
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_landmark_state_jumps[state]);
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
   one): every frame, the stage's label (stage_result_labels by stage: PRESTG, STAGE1-4, RESULT) at x A8,
   y 40 + 16 * stage, and at x E8 the player's place at that stage (stage_result_places by screen 360 +
   player + 8 * stage: 1st-8th); the last line says Retire (stage_result_retire) instead, unless it is
   RESULT (the goal reached), where the player's final place (place_address) is drawn big (animation 4,
   draw mode 8, at x F0, y B0, size 160; palette 41 for 1st, 42 for the others). */
void stage_result_line_draw(void)
{
	uint16_t attribute = 0;
	uint32_t animation = 0, place_index = 0, place_palette = 0, place_text = 0, player = 0, retire_text = 0, stage_result_text = 0, text_place = 0;
	uint32_t saved_position = 0;
	uint8_t final_place = 0;

	player = SCREEN(screen)->player;
	if (RACE(SCREEN(screen)->host_record)->racer_count <= 1u)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	OBJECT(current_object)->stage_result_line.place_address = screen + 0x320u + player;
	stage_result_text = stage_result_labels_ADDRESS + OBJECT(current_object)->stage_result_line.stage * 3u * 3u;
	attribute = STAGE_RESULT_TEXT(stage_result_text)->attribute;
	text_place = (0x40u + (OBJECT(current_object)->stage_result_line.stage << 4)) << 16;
	text_place |= (uint16_t)(text_place + 0xA8u);
	stage_result_text += 2u;
	saved_position = text_place;
	draw_text_at(text_place, attribute, stage_result_text);
	text_place = (saved_position & ~0xFFFFu) | (uint16_t)(saved_position + 0x40u);
	player = SCREEN(screen)->player;
	place_index = mem_read8(screen + 0x360u + player + (OBJECT(current_object)->stage_result_line.stage << 3)) * 3u;
	if (OBJECT(current_object)->stage_result_line.last == 0u)
	{
		place_text = stage_result_places_ADDRESS + place_index * 2u;
		attribute = STAGE_RESULT_PLACE_TEXT(place_text)->attribute;
		place_text += 2u;
		draw_text_at(text_place, attribute, place_text);
		return;
	}
	if (OBJECT(current_object)->stage_result_line.stage != 5u)
	{
		attribute = stage_result_retire[0].attribute;
		retire_text = stage_result_retire_ADDRESS + 0x2u;
		draw_text_at(text_place, attribute, retire_text);
		return;
	}
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF0u;
	OBJECT(current_object)->y = 0xB0u;
	OBJECT(current_object)->width_scale = 0x160u;
	OBJECT(current_object)->height_scale = 0x160u;
	animation = 4u;
	animation_start(animation);
	final_place = ~~mem_read8(OBJECT(current_object)->stage_result_line.place_address);
	OBJECT(current_object)->frame = final_place;
	place_palette = 0x41u;
	if (final_place != 0u)
		place_palette = 0x42u;
	OBJECT(current_object)->palette = place_palette;
	place_palette = draw_object(place_palette);
}

/* A leg of the route the player drove, drawn in red over the map shown after the race (003520), by
   state: 0 sets it up by its number (leg): 0 from the start to the first fork, 1 on from there by the
   direction taken (screen route_branches: 0 east, 1 west, the map's 09), 2-5 by the branch taken at each
   fork (screen route_branches + leg - 1) from the place of its stage (route_map_stage_xs,
   route_map_stage_ys by screen route_stages + leg - 2), the map shifted by the direction
   (route_map_direction_x_shifts) and 6B down; a leg driven already shows whole (frame 7, or B for 2-5),
   the leg the car is on (the map's unk_28) up to the frame before its progress, with the car on it
   (route_map_leg_car); legs 1-5 start the CP sign at their start (route_map_checkpoint), and leg 5, when
   the race reached the goal (race_copy finished), also the FINISH sign and the car at its end; 1 draws
   it. */
void route_map_leg_show(void)
{
	uint32_t animation = 0, fork_index = 0, leg_stage = 0, object = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_leg_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_leg_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->palette = 0x2D1u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->depth_layer = 0x18Eu;
	switch (OBJECT(current_object)->route_map_leg.leg)
	{
	case 0:
		animation = 0x306u;
		animation_start(animation);
		OBJECT(current_object)->x = 0xFFF7u;
		OBJECT(current_object)->y = 0x2Bu;
		OBJECT(current_object)->depth_layer++;
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
			OBJECT(current_object)->frame = 7u;
		else
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
		}
		break;
	case 1:
		animation = route_map_first_fork_leg_animations[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
		animation_start(animation);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0u;
		object = start_object();
		OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
		OBJECT(object)->route_map_checkpoint.leg = current_object;
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
		{
			OBJECT(current_object)->frame = 7u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
		object = start_object();
		OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
		OBJECT(object)->route_map_leg_car.leg = current_object;
		OBJECT(current_object)->frame--;
		if ((int8_t)OBJECT(current_object)->frame < 0)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		fork_index = OBJECT(current_object)->route_map_leg.leg - 1u;
		animation = route_map_fork_leg_animations[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch * 2u + mem_read8(screen + 0x34Au + fork_index)];
		animation_start(animation);
		leg_stage = mem_read8(screen + 0x343u + fork_index);
		OBJECT(current_object)->x = route_map_stage_xs[leg_stage];
		OBJECT(current_object)->y = route_map_stage_ys[leg_stage];
		object = start_object();
		OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
		OBJECT(object)->route_map_checkpoint.leg = current_object;
		OBJECT(current_object)->depth_layer += OBJECT(current_object)->route_map_leg.leg;
		if (OBJECT(current_object)->route_map_leg.leg != 5u)
		{
			if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
			{
				OBJECT(current_object)->frame = 0xBu;
				OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
			{
				OBJECT(current_object)->frame = 0u;
				OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
		}
		else
		{
			object = start_object();
			OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
			OBJECT(object)->route_map_checkpoint.leg = current_object;
			OBJECT(object)->route_map_checkpoint.kind = 1u;
			OBJECT(current_object)->route_map_leg.progress = 0xBu;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame = 0xBu;
		}
		break;
	default:
		invalid_jump(route_map_leg_jumps[OBJECT(current_object)->route_map_leg.leg]);
	}
	OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
	OBJECT(current_object)->y += 0x6Bu;
	OBJECT(current_object)->state++;
}

/* The player's car on the map after the race, where the red line of its leg ends, by state: 0 sets it
   up: the sprite of its model (route_map_car_animations, route_map_car_palettes by car model) at a
   quarter size, its place and frame (which way it faces) from a record by the leg's progress
   (route_map_start_leg_car_places for leg 0, route_map_first_fork_leg_car_places for leg 1 by the
   direction, route_map_fork_leg_car_places for legs 2-5 by the direction and the branch, from the
   stage's place), shifted like the legs; 1 draws it. */
void route_map_leg_car_show(void)
{
	uint32_t animation = 0, fork_index = 0, leg_stage = 0, model = 0, place = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_leg_car_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_leg_car_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	model = CAR(player_car)->model;
	animation = route_map_car_animations[model];
	animation_start(animation);
	OBJECT(current_object)->palette = route_map_car_palettes[model];
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	OBJECT(current_object)->depth_layer = 0x194u;
	switch (OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg)
	{
	case 0:
		place = route_map_start_leg_car_places_ADDRESS + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3);
		OBJECT(current_object)->x = ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y = ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	case 1:
		place = route_map_first_fork_leg_car_places_ADDRESS + (((uint32_t)SCREEN(screen)->route_branches << 6) + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3));
		OBJECT(current_object)->x = ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y = ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	case 2:
	case 3:
	case 4:
	case 5:
		fork_index = OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg - 1u;
		place = route_map_fork_leg_car_places_ADDRESS + (((SCREEN(screen)->route_branches * 3u) << 6) + ((mem_read8(screen + 0x34Au + fork_index) * 3u) << 5) + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3));
		leg_stage = mem_read8(screen + 0x343u + fork_index);
		OBJECT(current_object)->x = route_map_stage_xs[leg_stage];
		OBJECT(current_object)->y = route_map_stage_ys[leg_stage];
		OBJECT(current_object)->x += ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y += ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	default:
		invalid_jump(route_map_leg_car_leg_jumps[OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg]);
	}
}

/* Unused: ends the object (flags 0). It follows the code of route_map_leg_car_show's last state
   (draw_object), whose state table lists only states 0 and 1. */
void route_map_leg_car_end(void)
{
	OBJECT(current_object)->flags = 0u;
}

/* A sign on the map after the race, by state: 0 sets it up by its kind (route_map_sign_animations,
   route_map_sign_palettes): 0 the CP sign at the start of its leg (the fork it leaves from), 1 the
   FINISH sign beside the goal end of the last leg (route_map_finish_x_offsets,
   route_map_finish_y_offsets by the direction and the branch); 1 the CP sign turns (animation_advance)
   and is drawn; 2 FINISH blinks, drawn 4 frames out of 8 (blink). */
void route_map_checkpoint_show(void)
{
	uint32_t animation = 0, finish_index = 0, kind = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_checkpoint_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		kind = OBJECT(current_object)->route_map_checkpoint.kind;
		animation = route_map_sign_animations[kind];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_sign_palettes[kind];
		OBJECT(current_object)->width_scale = 0x100u;
		OBJECT(current_object)->height_scale = 0x100u;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->y;
		OBJECT(current_object)->y -= 6u;
		OBJECT(current_object)->depth_layer = 0x194u;
		if (OBJECT(current_object)->route_map_checkpoint.kind == 1u)
		{
			finish_index = SCREEN(screen)->route_branches * 2u + mem_read8(screen + 0x34Au + (OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->route_map_leg.leg - 1u));
			OBJECT(current_object)->x += route_map_finish_x_offsets[finish_index];
			OBJECT(current_object)->y += route_map_finish_y_offsets[finish_index];
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->state++;
		return;
	case 1:
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	case 2:
		OBJECT(current_object)->route_map_checkpoint.blink--;
		if ((OBJECT(current_object)->route_map_checkpoint.blink & 4u) == 0u)
			return;
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_checkpoint_state_jumps[state]);
	}
}

/* The WEST BOUND / EAST BOUND sign at the foot of the map after the race: a globe between the two,
   placed by the direction (screen route_branches) so that only the half of the way taken shows, by
   state: 0 sets it up below the screen; 1 rises 2 a frame to DC; 1 and 2 cycle its colours through four
   palettes from palette_base, one every 4 frames (colour_timer), and draw it. */
void route_map_direction_sign_show(void)
{
	uint32_t animation = 0, colour_cycle = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = route_map_direction_x_shifts[SCREEN(screen)->route_branches];
		OBJECT(current_object)->y = 0x118u;
		animation = 0x39u;
		OBJECT(current_object)->route_map_direction_sign.palette_base = 0x67u;
		animation_start(animation);
		return;
	case 1:
		OBJECT(current_object)->y -= 2u;
		if (OBJECT(current_object)->y == 0xDCu)
			OBJECT(current_object)->state++;
		colour_cycle = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		colour_cycle = (colour_cycle & ~0xFFu) | (uint8_t)(colour_cycle + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = colour_cycle;
		colour_cycle = (uint16_t)(((colour_cycle >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = colour_cycle;
		colour_cycle = draw_object(colour_cycle);
		return;
	case 2:
		colour_cycle = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		colour_cycle = (colour_cycle & ~0xFFu) | (uint8_t)(colour_cycle + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = colour_cycle;
		colour_cycle = (uint16_t)(((colour_cycle >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = colour_cycle;
		colour_cycle = draw_object(colour_cycle);
		return;
	default:
		invalid_jump(route_map_direction_sign_state_jumps[OBJECT(current_object)->state]);
	}
}

/* JUST A MOMENT PLEASE over the map after the race, while the game waits to go on, by state: 0 waits 32
   frames (timer), then sets it up at the centre of the screen, tiny; 1 grows to full size in 15 frames;
   2 drawn 15 frames; 3 hidden 5 frames, back to 2. */
void route_map_wait_banner_show(void)
{
	uint32_t animation = 0, routine = 0;
	uint8_t wait_count = 0, timer = 0, timer_2 = 0, timer_3 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_wait_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->route_map_wait_banner.timer++;
		wait_count = OBJECT(current_object)->route_map_wait_banner.timer & 0x1Fu;
		OBJECT(current_object)->route_map_wait_banner.timer = wait_count;
		if (wait_count == 0u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			animation = 0x28Du;
			animation_start(animation);
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
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->width_scale += 0x12u;
		OBJECT(current_object)->height_scale += 0x12u;
		routine = draw_object(routine);
		return;
	case 2:
		timer_2 = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 5u;
			OBJECT(current_object)->state++;
			routine = draw_object(routine);
			return;
		}
		routine = draw_object(routine);
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
		invalid_jump(route_map_wait_banner_state_jumps[state]);
	}
}

/* GAME OVER at the centre of the screen when a one-player race ends short of the goal, by state: 0 sets
   it up, tiny; 0 and 1 grow it to full size in 15 frames (timer); 2 draws it. */
void game_over_banner_show(void)
{
	uint32_t animation = 0, routine = 0;
	uint8_t timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = game_over_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = 0x2F4u;
		animation_start(animation);
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
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(game_over_banner_state_jumps[state]);
	}
	timer = OBJECT(current_object)->game_over_banner.timer;
	OBJECT(current_object)->game_over_banner.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(current_object)->state++;
		routine = draw_object(routine);
		return;
	}
	OBJECT(current_object)->width_scale += 0x12u;
	OBJECT(current_object)->height_scale += 0x12u;
	routine = draw_object(routine);
}

/* Starts the car select with 630 frames (21 seconds) to choose, then car_select_run. */
void car_select_start(void)
{
	OBJECT(current_object)->car_select.time_left = 0x276u;
	OBJECT(current_object)->handler = car_select_run_ADDRESS;
	car_select_run();
}

/* The car select, by state: 0 sets the screen up (unless screen entry_countdown_shown says the entry
   countdown did: clears the LCD, the title Mega Driver, sound F, DJ message 3B), the tile layer's tables
   from 006920, the WELCOME! letters; 1 when the screen's data is unpacked (unk_B00) starts the eight
   cars on the turntable and the rest, the default car (car_select_default_cars by the player's place in
   the race), the time display (time_digits_start); 2 30 frames; 3 the screen's unk_F04 up to 0, then the
   instructions (select_car_heading_show) and the car's caption (car_caption_show); 4 the wheel turns the
   turntable (turn, turn_delay, angle; car model is the car in front) until the gas or START confirms or
   time_left runs out: race model = the car, unk_48 = the car + 1, the car's sound
   (car_select_car_sounds), at least 180 frames left, the transmission choice (transmission_choice_run);
   5 the wheel picks the transmission (car transmission = car_select_wheel_transmissions by the wheel: 0
   automatic, 1 manual) until confirmed or out of time; 6-8 when the entry time is over (entry_over)
   marks the player ready (set_player_ready) and waits for the race's other players (players_not_ready),
   DJ message 3C; 9 60 frames, then fade_out_music and the game's next step (screen game_mode byte 2); 10
   stays. Every frame from 4 on: car_select_turn_to_choice and car_select_update_rowscroll; time_left
   down by one and shown in seconds. */
void car_select_run(void)
{
	uint32_t zero = 0;
	uint8_t car_index = 0, car_sound = 0, dj_message = 0, group_index = 0, new_turn = 0, sound_code = 0, turn_now = 0;
	uint16_t default_car = 0, new_angle = 0, row_number = 0;
	uint32_t background = 0, colour_block = 0, first_rowselect = 0, fourth_rowselect = 0, model = 0, object = 0, race = 0, remaining_time = 0, rows_to_fill = 0, rowscroll_address = 0, second_rowselect = 0, setup = 0, song_title = 0, wheel = 0, wheel_position = 0;
	uint32_t setup_bits = 0;
	uint8_t cars_left = 0, groups_left = 0, turn_rate = 0, turn_delay = 0;
	uint16_t timer = 0, timer_2 = 0;
	uint32_t depth_offset = 0;

	if (RACE(RACE_COPY_1)->entry_frames == 0u)
		OBJECT(current_object)->car_select.entry_over = 0xFFu;
	OBJECT(current_object)->car_select.time_left--;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (SCREEN(screen)->entry_countdown_shown == 0u)
		{
			clear_lcd();
			SCREEN(screen)->dj_face = 0xFFu;
			song_title = 0x6EB6u;
			start_song_title(song_title);
			sound_code = 0xFu;
			request_sound(sound_code);
			dj_message = 0x3Bu;
			start_dj_message(dj_message);
		}
		setup = car_select_tilemap_setups_ADDRESS + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
		OBJECT(current_object)->car_select.scroll_x_address = CAR_SELECT_TILEMAP_SETUP(setup)->scroll_x_address;
		setup += 4u;
		OBJECT(current_object)->car_select.scroll_y_address = CAR_SELECT_TILEMAP_SETUP(setup - 0x4u)->scroll_y_address;
		setup += 4u;
		OBJECT(current_object)->car_select.second_scroll_x_address = CAR_SELECT_TILEMAP_SETUP(setup - 0x8u)->second_scroll_x_address;
		setup += 4u;
		OBJECT(current_object)->car_select.second_scroll_y_address = CAR_SELECT_TILEMAP_SETUP(setup - 0xCu)->second_scroll_y_address;
		setup += 4u;
		OBJECT(current_object)->car_select.rowscroll_address = CAR_SELECT_TILEMAP_SETUP(setup - 0x10u)->rowscroll_address;
		setup += 4u;
		OBJECT(current_object)->car_select.rowselect_address = CAR_SELECT_TILEMAP_SETUP(setup - 0x14u)->rowselect_address;
		setup += 4u;
		background = CAR_SELECT_TILEMAP_SETUP(setup - 0x18u)->background;
		setup_bits = setup + 4u;
		set_up_background(background);
		video_registers[2] |= mem_read16(setup_bits);
		mem_write16(OBJECT(current_object)->car_select.second_scroll_y_address, 0xE0u);
		object = start_object();
		OBJECT(object)->handler = welcome_letter_start_ADDRESS;
		OBJECT(current_object)->state++;
		first_rowselect = OBJECT(current_object)->car_select.rowselect_address;
		second_rowselect = first_rowselect + 0x800u;
		r12 = first_rowselect + 0x1000u;
		fourth_rowselect = first_rowselect + 0x1800u;
		row_number = 0x98u;
		rows_to_fill = 0xE0u;
		do
		{
			mem_write16(first_rowselect, row_number);
			mem_write16(second_rowselect, row_number);
			mem_write16(r12, row_number);
			r12 = r12 + 2u;
			mem_write16(fourth_rowselect, row_number);
			fourth_rowselect += 2u;
			row_number++;
			rows_to_fill--;
			first_rowselect += 2u;
			second_rowselect += 2u;
		} while (rows_to_fill != 0u);
		rowscroll_address = OBJECT(current_object)->car_select.rowscroll_address;
		move_halfwords(rowscroll_address, zero_block_ADDRESS, 0xE0u);
		move_halfwords(rowscroll_address + 0x800u, zero_block_ADDRESS, 0xE0u);
		move_halfwords(rowscroll_address + 0x1000u, zero_block_ADDRESS, 0xE0u);
		move_halfwords(rowscroll_address + 0x1800u, zero_block_ADDRESS, 0xE0u);
		move_string(zero_block_ADDRESS, 0xE0u, screen + 0x3800u, 0xE0u, 2u, 0u, 0u, 0u, &r27, &r28);
		return;
	case 1:
		if (SCREEN(screen)->unpack_state != 0u)
			return;
		object = start_object();
		OBJECT(object)->handler = car_attendant_run_ADDRESS;
		OBJECT(object)->car_attendant.car_select = current_object;
		object = start_object();
		OBJECT(object)->handler = car_select_entries_run_ADDRESS;
		OBJECT(object)->car_select_entries.car_select = current_object;
		car_index = 8u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = turntable_car_show_ADDRESS;
			OBJECT(object)->turntable_car.car_select = current_object;
			cars_left = car_index;
			car_index = (uint8_t)(cars_left - 1u);
			OBJECT(object)->turntable_car.car = car_index;
		} while (cars_left != 1u);
		group_index = 0xCu;
		do
		{
			object = start_object();
			OBJECT(object)->handler = turntable_scenery_show_ADDRESS;
			OBJECT(object)->turntable_scenery.car_select = current_object;
			groups_left = group_index;
			group_index = (uint8_t)(groups_left - 1u);
			OBJECT(object)->turntable_scenery.group = group_index;
		} while (groups_left != 1u);
		default_car = car_select_default_cars[mem_read8(screen + 0x320u + SCREEN(screen)->player)];
		CAR(player_car)->model = (uint8_t)default_car;
		CAR(player_car)->transmission = 0u;
		OBJECT(current_object)->car_select.car_chosen = 0u;
		OBJECT(current_object)->car_select.transmission_chosen = 0u;
		default_car = (uint16_t)((uint32_t)default_car << 7);
		mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - default_car);
		mem_write16(OBJECT(current_object)->car_select.scroll_y_address, 0u);
		OBJECT(current_object)->car_select.angle = (uint32_t)default_car << 2;
		OBJECT(current_object)->car_select.top_scroll = 0u;
		OBJECT(current_object)->car_select.turn = 0;
		OBJECT(current_object)->car_select.turn_delay = 1;
		OBJECT(current_object)->car_select.rowscroll_copies = 0u;
		OBJECT(current_object)->car_select.gas_down = 0u;
		OBJECT(current_object)->car_select.confirm_delay = 0x1Eu;
		time_digits_start();
		SCREEN(screen)->turntable_depth_offset = -0x300;
		SCREEN(screen)->turntable_radius_offset = 0;
		OBJECT(current_object)->car_select.timer = 0x1Eu;
		OBJECT(current_object)->state++;
		colour_block = car_select_palette;
		queue_palette(colour_block);
		zero = car_select_wait_confirm();
		timer = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer - 1u;
		if (timer == 1u)
			OBJECT(current_object)->state++;
		remaining_time = OBJECT(current_object)->car_select.time_left;
		remaining_time = remaining_time / 0x1Eu;
		remaining_time = binary_to_bcd(remaining_time);
		time_digits_draw(remaining_time);
		return;
	case 2:
		zero = car_select_wait_confirm();
		timer = OBJECT(current_object)->car_select.timer;
		OBJECT(current_object)->car_select.timer = timer - 1u;
		if (timer == 1u)
			OBJECT(current_object)->state++;
		remaining_time = OBJECT(current_object)->car_select.time_left;
		remaining_time = remaining_time / 0x1Eu;
		remaining_time = binary_to_bcd(remaining_time);
		time_digits_draw(remaining_time);
		return;
	case 3:
		zero = car_select_wait_confirm();
		depth_offset = SCREEN(screen)->turntable_depth_offset + 0x40u;
		SCREEN(screen)->turntable_depth_offset = depth_offset;
		if (depth_offset != 0u)
		{
			remaining_time = OBJECT(current_object)->car_select.time_left;
			remaining_time = remaining_time / 0x1Eu;
			remaining_time = binary_to_bcd(remaining_time);
			time_digits_draw(remaining_time);
			return;
		}
		OBJECT(current_object)->state++;
		object = start_object();
		OBJECT(object)->handler = select_car_heading_show_ADDRESS;
		OBJECT(object)->select_car_heading.car_select = current_object;
		object = start_object();
		OBJECT(object)->handler = car_caption_show_ADDRESS;
		OBJECT(object)->car_caption.car_select = current_object;
		remaining_time = OBJECT(current_object)->car_select.time_left;
		remaining_time = remaining_time / 0x1Eu;
		remaining_time = binary_to_bcd(remaining_time);
		time_digits_draw(remaining_time);
		return;
	case 4:
		if (OBJECT(current_object)->car_select.time_left != 0u)
		{
			turn_now = (uint8_t)OBJECT(current_object)->car_select.turn;
			if (turn_now == 0u)
			{
				wheel_position = SCREEN(screen)->wheel;
				wheel_position = wheel_position >> 5;
				turn_rate = car_select_turn_delays[wheel_position];
				turn_delay = OBJECT(current_object)->car_select.turn_delay;
				OBJECT(current_object)->car_select.turn_delay = turn_delay - turn_rate;
				if ((int8_t)turn_delay <= (int8_t)turn_rate)
				{
					OBJECT(current_object)->car_select.turn_delay = 0xF;
					new_turn = 0x10u;
					if (wheel_position >> 2 != 0u)
						new_turn = (uint8_t)(0u - new_turn);
					OBJECT(current_object)->car_select.turn = new_turn;
				}
			}
			else
			{
				if ((int8_t)turn_now < 0)
				{
					if (SCREEN(screen)->wheel >= 0x60u)
					{
						OBJECT(current_object)->car_select.turn = turn_now + 1u;
						car_select_skew_floor_previous();
						new_angle = (uint16_t)(OBJECT(current_object)->car_select.angle - 0x20u);
					}
					else
					{
						turn_now += 0x10u;
						OBJECT(current_object)->car_select.turn = turn_now - 1u;
						car_select_skew_floor_next();
						new_angle = (uint16_t)(OBJECT(current_object)->car_select.angle + 0x20u);
					}
				}
				else if (SCREEN(screen)->wheel < 0xA0u)
				{
					OBJECT(current_object)->car_select.turn = turn_now - 1u;
					car_select_skew_floor_next();
					new_angle = (uint16_t)(OBJECT(current_object)->car_select.angle + 0x20u);
				}
				else
				{
					turn_now -= 0x10u;
					OBJECT(current_object)->car_select.turn = turn_now + 1u;
					car_select_skew_floor_previous();
					new_angle = (uint16_t)(OBJECT(current_object)->car_select.angle - 0x20u);
				}
				OBJECT(current_object)->car_select.angle = new_angle;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (new_angle >> 2));
				if ((uint8_t)OBJECT(current_object)->car_select.turn == 0u)
				{
					wheel_position = SCREEN(screen)->wheel;
					wheel_position = wheel_position >> 5;
					turn_rate = car_select_turn_delays[wheel_position];
					turn_delay = OBJECT(current_object)->car_select.turn_delay;
					OBJECT(current_object)->car_select.turn_delay = turn_delay - turn_rate;
					if ((int8_t)turn_delay <= (int8_t)turn_rate)
					{
						OBJECT(current_object)->car_select.turn_delay = 0xF;
						new_turn = 0x10u;
						if (wheel_position >> 2 != 0u)
							new_turn = (uint8_t)(0u - new_turn);
						OBJECT(current_object)->car_select.turn = new_turn;
					}
				}
			}
			CAR(player_car)->model = ((OBJECT(current_object)->car_select.angle + 0x100u) & 0xFFFu) >> 9;
			zero = car_select_wait_confirm();
			if (zero)
			{
				car_select_turn_to_choice();
				car_select_update_rowscroll();
				remaining_time = OBJECT(current_object)->car_select.time_left;
				remaining_time = remaining_time / 0x1Eu;
				remaining_time = binary_to_bcd(remaining_time);
				time_digits_draw(remaining_time);
				return;
			}
		}
		OBJECT(current_object)->car_select.car_chosen = 0xFFu;
		race = SCREEN(screen)->race;
		model = CAR(player_car)->model;
		RACE(race)->model = model;
		RACE(race)->car_choice = model + 1u;
		OBJECT(current_object)->car_select.confirm_delay = 0x1Eu;
		car_sound = car_select_car_sounds[model];
		request_sound(car_sound);
		if ((int16_t)OBJECT(current_object)->car_select.time_left < 0xB4)
			OBJECT(current_object)->car_select.time_left = 0xB4u;
		object = start_object();
		OBJECT(object)->handler = transmission_choice_run_ADDRESS;
		OBJECT(object)->transmission_choice.car_select = current_object;
		OBJECT(current_object)->state++;
		car_select_turn_to_choice();
		car_select_update_rowscroll();
		remaining_time = OBJECT(current_object)->car_select.time_left;
		remaining_time = remaining_time / 0x1Eu;
		remaining_time = binary_to_bcd(remaining_time);
		time_digits_draw(remaining_time);
		return;
	case 5:
		if (OBJECT(current_object)->car_select.time_left != 0u)
		{
			wheel = SCREEN(screen)->wheel;
			CAR(player_car)->transmission = car_select_wheel_transmissions[wheel >> 5];
			zero = car_select_wait_confirm();
			if (zero)
			{
				car_select_turn_to_choice();
				car_select_update_rowscroll();
				remaining_time = OBJECT(current_object)->car_select.time_left;
				remaining_time = remaining_time / 0x1Eu;
				remaining_time = binary_to_bcd(remaining_time);
				time_digits_draw(remaining_time);
				return;
			}
		}
		object = start_object();
		OBJECT(object)->handler = waiting_banner_show_ADDRESS;
		OBJECT(object)->waiting_banner.car_select = current_object;
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
		dj_message = 0x3Cu;
		start_dj_message_if_quiet(dj_message);
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
		dj_message = 0x3Cu;
		start_dj_message_if_quiet(dj_message);
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
		dj_message = 0x3Cu;
		start_dj_message_if_quiet(dj_message);
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
		invalid_jump(car_select_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Once the car is chosen (car_chosen), turns the turntable the short way round until the chosen car (car
   model) is in front, 20 a frame, skewing the floor as it goes. */
void car_select_turn_to_choice(void)
{
	uint32_t negative = 0;
	uint16_t turntable_angle = 0;
	uint16_t angle_to_choice = 0;

	switch (OBJECT(current_object)->car_select.choice_turn)
	{
	case 0:
		if (OBJECT(current_object)->car_select.car_chosen == 0u)
			return;
		OBJECT(current_object)->car_select.choice_turn++;
		turntable_angle = OBJECT(current_object)->car_select.angle;
		angle_to_choice = (((uint32_t)CAR(player_car)->model << 9) - turntable_angle) << 4;
		negative = angle_to_choice >> 15;
		if (angle_to_choice != 0u)
		{
			if (negative)
			{
				turntable_angle -= 0x20u;
				OBJECT(current_object)->car_select.angle = turntable_angle;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (turntable_angle >> 2));
				car_select_skew_floor_previous();
				return;
			}
			turntable_angle += 0x20u;
			OBJECT(current_object)->car_select.angle = turntable_angle;
			mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (turntable_angle >> 2));
			car_select_skew_floor_next();
			return;
		}
		OBJECT(current_object)->car_select.choice_turn++;
		return;
	case 1:
		turntable_angle = OBJECT(current_object)->car_select.angle;
		angle_to_choice = (((uint32_t)CAR(player_car)->model << 9) - turntable_angle) << 4;
		negative = angle_to_choice >> 15;
		if (angle_to_choice != 0u)
		{
			if (negative)
			{
				turntable_angle -= 0x20u;
				OBJECT(current_object)->car_select.angle = turntable_angle;
				mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (turntable_angle >> 2));
				car_select_skew_floor_previous();
				return;
			}
			turntable_angle += 0x20u;
			OBJECT(current_object)->car_select.angle = turntable_angle;
			mem_write16(OBJECT(current_object)->car_select.scroll_x_address, 0u - (turntable_angle >> 2));
			car_select_skew_floor_next();
			return;
		}
		OBJECT(current_object)->car_select.choice_turn++;
		return;
	case 2:
		return;
	default:
		invalid_jump(car_select_choice_turn_jumps[OBJECT(current_object)->car_select.choice_turn]);
	}
}

/* Writes the line scroll of the tile layer for the next frames: the top 104 lines all at top_scroll,
   which moves one more each frame, and while rowscroll_copies says, the floor's 120 lines from their
   offsets at 3800 of the screen. */
void car_select_update_rowscroll(void)
{
	uint8_t fraction_shift = 0;
	uint16_t line_scroll = 0;
	uint32_t far_page = 0, line_offsets = 0, lines_left = 0, near_page = 0, near_scroll_place = 0;

	near_page = (uint8_t)(video_registers[2] >> 8u);
	far_page = near_page;
	near_page = (uint8_t)(near_page + 4u);
	far_page = (uint8_t)(far_page + 8u);
	near_scroll_place = OBJECT(current_object)->car_select.rowscroll_address + ((near_page & 0xCu) << 7) * 4u;
	r12 = OBJECT(current_object)->car_select.rowscroll_address + ((far_page & 0xCu) << 7) * 4u;
	line_scroll = (uint16_t)(OBJECT(current_object)->car_select.top_scroll + 1u);
	OBJECT(current_object)->car_select.top_scroll = line_scroll;
	lines_left = 0x68u;
	do
	{
		mem_write16(near_scroll_place, line_scroll);
		mem_write16(r12, line_scroll);
		r12 = r12 + 2u;
		lines_left--;
		near_scroll_place += 2u;
	} while (lines_left != 0u);
	switch (OBJECT(current_object)->car_select.rowscroll_copies)
	{
	case 1:
	case 2:
		fraction_shift = 0xFAu;
		line_offsets = screen + 0x3800u;
		lines_left = 0x78u;
		do
		{
			line_scroll = shift_by(mem_read16(line_offsets), (uint8_t)fraction_shift, 16);
			mem_write16(near_scroll_place, line_scroll);
			near_scroll_place += 2u;
			mem_write16(r12, line_scroll);
			r12 = r12 + 2u;
			lines_left--;
			line_offsets += 2u;
		} while (lines_left != 0u);
		OBJECT(current_object)->car_select.rowscroll_copies--;
		break;
	case 3:
		OBJECT(current_object)->car_select.rowscroll_copies--;
		return;
	case 0:
		break;
	default:
		invalid_jump(car_select_rowscroll_copies_jumps[OBJECT(current_object)->car_select.rowscroll_copies]);
	}
}

/* Skews the turntable's floor for a step toward the next car (car_select_skew_floor with 6). */
void car_select_skew_floor_next(void)
{
	uint16_t skew_step = 0;

	skew_step = 6u;
	car_select_skew_floor(skew_step);
}

/* Skews the turntable's floor for a step toward the previous car (car_select_skew_floor with -6). */
void car_select_skew_floor_previous(void)
{
	uint16_t skew_step = 0;

	skew_step = 0xFFFAu;
	car_select_skew_floor(skew_step);
}

/* Adds a step that grows line by line (by R0) to the floor's 120 line offsets at 3800 of the screen,
   writes them to the next two pages of the line scroll and has car_select_update_rowscroll copy them to
   the others (rowscroll_copies). */
void car_select_skew_floor(uint16_t skew_step)
{
	uint8_t fraction_shift = 0;
	uint16_t far_offset = 0, near_offset = 0, skew = 0;
	uint32_t far_page = 0, line_offsets = 0, lines_left = 0, near_page = 0, near_scroll_place = 0, rowscroll_address = 0;

	near_page = (uint8_t)(video_registers[2] >> 8u);
	far_page = near_page;
	near_page = (uint8_t)(near_page + 4u);
	far_page = (uint8_t)(far_page + 8u);
	rowscroll_address = OBJECT(current_object)->car_select.rowscroll_address;
	near_scroll_place = rowscroll_address + 0xD0u + ((near_page & 0xCu) << 7) * 4u;
	r12 = rowscroll_address + 0xD0u + ((far_page & 0xCu) << 7) * 4u;
	line_offsets = screen + 0x3800u;
	skew = 0u;
	fraction_shift = 0xFAu;
	lines_left = 0x78u;
	do
	{
		skew += skew_step;
		near_offset = (uint16_t)(mem_read16(line_offsets) + skew);
		far_offset = (uint16_t)(near_offset + skew);
		mem_write16(line_offsets, far_offset);
		mem_write16(near_scroll_place, shift_by(near_offset, (uint8_t)fraction_shift, 16));
		near_scroll_place += 2u;
		mem_write16(r12, shift_by(far_offset, (uint8_t)fraction_shift, 16));
		r12 = r12 + 2u;
		lines_left--;
		line_offsets += 2u;
	} while (lines_left != 0u);
	OBJECT(current_object)->car_select.rowscroll_copies = 3u;
}

/* The man who stands by the car in front on the car select's turntable, by state
   (car_attendant_state_jumps): 0 sets him up half a car round from the front (angle, radius, size); 1
   stands beside the car in front (car model) until it changes, then 2 walks round the turntable, 20 a
   frame, to the front, facing his way (flags bit 6), and stands again; once the car is chosen
   (car_select car_chosen), 3 walks to one side of the car or the other by the transmission the wheel
   picks (side_offset to car transmission * 80) and stands there; once it is chosen
   (transmission_chosen), 4 strikes the pose for it (animation 267 automatic, 268 manual). Each frame he
   is drawn at his angle (plus side_offset) less the turntable's (car_select angle) with turntable_draw,
   and animated. */
void car_attendant_run(void)
{
	uint32_t negative = 0;
	uint16_t target_side = 0;
	uint32_t across = 0, animation = 0, attendant_angle = 0, down = 0, ignored_2 = 0, point_depth = 0, radius = 0;
	uint16_t angle_to_front = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = 0x269u;
		OBJECT(current_object)->palette = 0x7Cu;
		animation_start(animation);
		OBJECT(current_object)->car_attendant.angle = OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle + 0x100u;
		OBJECT(current_object)->car_attendant.radius = 0x160u;
		OBJECT(current_object)->car_attendant.size = 0xC0u;
		OBJECT(current_object)->state++;
	case 1:
		if (((((uint32_t)CAR(player_car)->model << 9) - OBJECT(current_object)->car_attendant.angle) & 0xFFFu) != 0u)
		{
			animation = 0x266u;
			animation_start(animation);
			OBJECT(current_object)->state++;
			attendant_angle = OBJECT(current_object)->car_attendant.angle;
			angle_to_front = (attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle) << 4;
			negative = angle_to_front >> 15;
			if (angle_to_front == 0u)
			{
				animation = 0x269u;
				animation_start(animation);
				OBJECT(current_object)->flags &= 0xBFu;
				OBJECT(current_object)->state--;
			}
			else if (negative)
			{
				OBJECT(current_object)->flags |= 0x40u;
				attendant_angle = (uint16_t)(attendant_angle + 0x20u);
				OBJECT(current_object)->car_attendant.angle = attendant_angle;
			}
			else
			{
				OBJECT(current_object)->flags &= 0xBFu;
				attendant_angle = (uint16_t)(attendant_angle - 0x20u);
				OBJECT(current_object)->car_attendant.angle = attendant_angle;
			}
			attendant_angle = (uint16_t)(attendant_angle - 0x40u);
			attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
			attendant_angle = attendant_angle & 0xFFFu;
			across = sine_table[attendant_angle];
			point_depth = 0u - cosine_table[attendant_angle];
			radius = OBJECT(current_object)->car_attendant.radius;
			across = (int32_t)(across * radius) >> 14;
			down = 0x6Cu;
			point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
			turntable_draw(across, down, point_depth);
			ignored_2 = animation_advance(0u);
			return;
		}
		if (OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.car_chosen == 0u)
		{
			attendant_angle = OBJECT(current_object)->car_attendant.angle;
			attendant_angle = (uint16_t)(attendant_angle - 0x40u);
			attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
			attendant_angle = attendant_angle & 0xFFFu;
			across = sine_table[attendant_angle];
			point_depth = 0u - cosine_table[attendant_angle];
			radius = OBJECT(current_object)->car_attendant.radius;
			across = (int32_t)(across * radius) >> 14;
			down = 0x6Cu;
			point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
			turntable_draw(across, down, point_depth);
			ignored_2 = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->car_attendant.side_offset = 0;
		OBJECT(current_object)->state = 3u;
		break;
	case 2:
		attendant_angle = OBJECT(current_object)->car_attendant.angle;
		angle_to_front = (attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle) << 4;
		negative = angle_to_front >> 15;
		if (angle_to_front == 0u)
		{
			animation = 0x269u;
			animation_start(animation);
			OBJECT(current_object)->flags &= 0xBFu;
			OBJECT(current_object)->state--;
		}
		else if (negative)
		{
			OBJECT(current_object)->flags |= 0x40u;
			attendant_angle = (uint16_t)(attendant_angle + 0x20u);
			OBJECT(current_object)->car_attendant.angle = attendant_angle;
		}
		else
		{
			OBJECT(current_object)->flags &= 0xBFu;
			attendant_angle = (uint16_t)(attendant_angle - 0x20u);
			OBJECT(current_object)->car_attendant.angle = attendant_angle;
		}
		attendant_angle = (uint16_t)(attendant_angle - 0x40u);
		attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
		attendant_angle = attendant_angle & 0xFFFu;
		across = sine_table[attendant_angle];
		point_depth = 0u - cosine_table[attendant_angle];
		radius = OBJECT(current_object)->car_attendant.radius;
		across = (int32_t)(across * radius) >> 14;
		down = 0x6Cu;
		point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
		turntable_draw(across, down, point_depth);
		ignored_2 = animation_advance(0u);
		return;
	case 3:
		break;
	case 4:
		attendant_angle = OBJECT(current_object)->car_attendant.angle;
		attendant_angle = (uint16_t)(attendant_angle - 0x40u);
		attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
		attendant_angle = attendant_angle & 0xFFFu;
		across = sine_table[attendant_angle];
		point_depth = 0u - cosine_table[attendant_angle];
		radius = OBJECT(current_object)->car_attendant.radius;
		across = (int32_t)(across * radius) >> 14;
		down = 0x6Cu;
		point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
		turntable_draw(across, down, point_depth);
		ignored_2 = animation_advance(0u);
		return;
	default:
		invalid_jump(car_attendant_state_jumps[OBJECT(current_object)->state]);
	}
	attendant_angle = (uint16_t)OBJECT(current_object)->car_attendant.side_offset;
	target_side = (uint16_t)((uint32_t)CAR(player_car)->transmission << 7);
	if (attendant_angle != target_side)
	{
		if ((int16_t)target_side < (int16_t)attendant_angle)
		{
			OBJECT(current_object)->flags &= 0xBFu;
			attendant_angle = (uint16_t)(attendant_angle - 0x20u);
		}
		else
		{
			OBJECT(current_object)->flags |= 0x40u;
			attendant_angle = (uint16_t)(attendant_angle + 0x20u);
		}
		OBJECT(current_object)->car_attendant.side_offset = attendant_angle;
		animation = 0x266u;
		if (OBJECT(current_object)->animation != 0x266u)
			animation_start(animation);
		attendant_angle = (uint16_t)(attendant_angle + OBJECT(current_object)->car_attendant.angle);
		attendant_angle = (uint16_t)(attendant_angle - 0x40u);
		attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
		attendant_angle = attendant_angle & 0xFFFu;
		across = sine_table[attendant_angle];
		point_depth = 0u - cosine_table[attendant_angle];
		radius = OBJECT(current_object)->car_attendant.radius;
		across = (int32_t)(across * radius) >> 14;
		down = 0x6Cu;
		point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
		turntable_draw(across, down, point_depth);
		ignored_2 = animation_advance(0u);
		return;
	}
	if (OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.transmission_chosen != 0u)
	{
		OBJECT(current_object)->car_attendant.angle += OBJECT(current_object)->car_attendant.side_offset;
		animation = 0x267u;
		if (CAR(player_car)->transmission != 0u)
			animation = 0x268u;
		animation_start(animation);
		OBJECT(current_object)->state++;
		attendant_angle = OBJECT(current_object)->car_attendant.angle;
		attendant_angle = (uint16_t)(attendant_angle - 0x40u);
		attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
		attendant_angle = attendant_angle & 0xFFFu;
		across = sine_table[attendant_angle];
		point_depth = 0u - cosine_table[attendant_angle];
		radius = OBJECT(current_object)->car_attendant.radius;
		across = (int32_t)(across * radius) >> 14;
		down = 0x6Cu;
		point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
		turntable_draw(across, down, point_depth);
		ignored_2 = animation_advance(0u);
		return;
	}
	animation = 0x269u;
	if (OBJECT(current_object)->animation == 0x269u)
	{
		attendant_angle = (uint16_t)(attendant_angle + OBJECT(current_object)->car_attendant.angle);
		attendant_angle = (uint16_t)(attendant_angle - 0x40u);
		attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
		attendant_angle = attendant_angle & 0xFFFu;
		across = sine_table[attendant_angle];
		point_depth = 0u - cosine_table[attendant_angle];
		radius = OBJECT(current_object)->car_attendant.radius;
		across = (int32_t)(across * radius) >> 14;
		down = 0x6Cu;
		point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
		turntable_draw(across, down, point_depth);
		ignored_2 = animation_advance(0u);
		return;
	}
	animation_start(animation);
	attendant_angle = (uint16_t)(attendant_angle + OBJECT(current_object)->car_attendant.angle);
	attendant_angle = (uint16_t)(attendant_angle - 0x40u);
	attendant_angle = (uint16_t)(attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle);
	attendant_angle = attendant_angle & 0xFFFu;
	across = sine_table[attendant_angle];
	point_depth = 0u - cosine_table[attendant_angle];
	radius = OBJECT(current_object)->car_attendant.radius;
	across = (int32_t)(across * radius) >> 14;
	down = 0x6Cu;
	point_depth = ((int32_t)(point_depth * radius) >> 14) + 0x140u;
	turntable_draw(across, down, point_depth);
	ignored_2 = animation_advance(0u);
}

/* One of the eight cars on the car select's turntable (car: its model and its place, car << 9), by
   state: 0 its sprite set and palette (turntable_car_looks by car); 1 placed by its angle less the
   turntable's (car_select angle), with the frame that shows it from there (turntable_car_frames), round
   the centre (radius screen turntable_radius_offset + 140) with turntable_draw; once the car is chosen
   (car_select car_chosen) the car in front takes the palette 4 on if it is the chosen one (car model)
   and goes to 2, where draw_object draws it where it was. */
void turntable_car_show(void)
{
	uint32_t across = 0, animation = 0, car_angle = 0, down = 0, ignored = 0, look = 0, point_depth = 0, ring_radius = 0, routine = 0, seen_angle = 0;
	uint16_t angle_from_front = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = turntable_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		look = turntable_car_looks_ADDRESS + OBJECT(current_object)->turntable_car.car * 4u;
		animation = ANIMATION_PALETTE(look)->animation;
		OBJECT(current_object)->palette = ANIMATION_PALETTE(look)->palette;
		animation_start(animation);
		OBJECT(current_object)->turntable_car.size = 0x100u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(turntable_car_state_jumps[state]);
	}
	car_angle = OBJECT(current_object)->turntable_car.car;
	car_angle = (uint16_t)(car_angle << 9);
	car_angle = (uint16_t)(car_angle - OBJECT(OBJECT(current_object)->turntable_car.car_select)->car_select.angle);
	angle_from_front = car_angle & 0xFFFu;
	seen_angle = angle_from_front;
	if (angle_from_front == 0u && OBJECT(OBJECT(current_object)->turntable_car.car_select)->car_select.car_chosen != 0u)
	{
		if (OBJECT(current_object)->turntable_car.car == CAR(player_car)->model)
			OBJECT(current_object)->palette += 4u;
		OBJECT(current_object)->state++;
	}
	OBJECT(current_object)->frame = turntable_car_frames[seen_angle >> 6];
	ring_radius = SCREEN(screen)->turntable_radius_offset + 0x140u;
	across = (int32_t)(sine_table[seen_angle] * ring_radius) >> 14;
	point_depth = (int32_t)((0u - cosine_table[seen_angle]) * ring_radius) >> 14;
	down = 0x70u;
	point_depth += 0x140u;
	ignored = turntable_draw(across, down, point_depth);
}

/* A ring of scenery round the car select's turntable (one of twelve groups, from 006A18: the crowd, the
   palm trees), by state: 0 its animation, palette, size_step, size and list of items (item_list: angle,
   radius and height of each, closed by a negative angle); 1 each frame takes the turntable's angle
   (car_select angle) and draws the items on the far side (their angle less it 480..B7F) with
   turntable_draw, keeping for each its previous_place and flags bit 5 in item_places for draw mode 0A's
   halfway copy; then animation_advance. */
void turntable_scenery_show(void)
{
	uint16_t item_flags = 0, size = 0;
	uint32_t across = 0, animation = 0, down = 0, group = 0, ignored = 0, item_angle = 0, item_place = 0, leftover = 0, point_depth = 0, ring_radius = 0, saved_place = 0;
	uint32_t kept_saved_place = 0, kept_item_place = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		group = turntable_scenery_groups_ADDRESS + OBJECT(current_object)->turntable_scenery.group * 2u * 4u;
		animation = TURNTABLE_SCENERY_GROUP(group)->animation;
		group += 2u;
		animation_start(animation);
		OBJECT(current_object)->palette = TURNTABLE_SCENERY_GROUP(group - 0x2u)->palette;
		group += 2u;
		size = TURNTABLE_SCENERY_GROUP(group - 0x4u)->size;
		OBJECT(current_object)->size_step = size >> 12;
		OBJECT(current_object)->turntable_scenery.size = size & 0xFFFu;
		OBJECT(current_object)->turntable_scenery.item_list = group + 2u + TURNTABLE_SCENERY_GROUP(group - 0x4u)->item_list_offset;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(turntable_scenery_state_jumps[OBJECT(current_object)->state]);
	}
	item_place = OBJECT(current_object)->turntable_scenery.item_list;
	saved_place = current_object + 0x80u;
	OBJECT(current_object)->turntable_scenery.angle = OBJECT(OBJECT(current_object)->turntable_scenery.car_select)->car_select.angle;
	for (;;)
	{
		item_angle = mem_read16(item_place);
		item_place += 2u;
		if ((int16_t)item_angle < 0)
			break;
		item_angle = (uint16_t)(item_angle - OBJECT(current_object)->turntable_scenery.angle);
		item_angle = item_angle & 0xFFFu;
		ring_radius = mem_read16(item_place);
		item_place += 2u;
		if (item_angle < 0x480u || item_angle > 0xB7Fu)
		{
			item_place += 2u;
			saved_place += 2u;
			mem_write16(saved_place, 0u);
			saved_place += 2u;
			continue;
		}
		ring_radius += SCREEN(screen)->turntable_radius_offset;
		across = (int32_t)(sine_table[item_angle] * ring_radius) >> 14;
		point_depth = (int32_t)((0u - cosine_table[item_angle]) * ring_radius) >> 14;
		down = (int16_t)mem_read16(item_place);
		point_depth += 0x140u;
		kept_saved_place = saved_place;
		OBJECT(current_object)->previous_place = (screen & ~0xFFFFu) | mem_read16(saved_place);
		OBJECT(current_object)->flags = (OBJECT(current_object)->flags & 0xDFu) | mem_read16(saved_place + 2u);
		kept_item_place = item_place + 2u;
		leftover = turntable_draw(across, down, point_depth);
		item_place = kept_item_place;
		saved_place = kept_saved_place;
		mem_write16(saved_place, (uint16_t)OBJECT(current_object)->turntable_scenery.previous_place);
		item_flags = (uint16_t)((leftover & ~0xFFu) | OBJECT(current_object)->flags);
		saved_place += 2u;
		mem_write16(saved_place, (item_flags & ~0xFFu) | (item_flags & 0x20u));
		saved_place += 2u;
	}
	ignored = animation_advance(0u);
}

/* Starts the eight letters of WELCOME! (itself the first), then welcome_letter_show. */
void welcome_letter_start(void)
{
	uint32_t letter_index = 0, object = 0;

	letter_index = 7u;
	do
	{
		object = start_object();
		OBJECT(object)->handler = welcome_letter_show_ADDRESS;
		OBJECT(object)->welcome_letter.index = letter_index;
		letter_index--;
	} while (letter_index != 0u);
	OBJECT(current_object)->welcome_letter.index = letter_index;
	OBJECT(current_object)->handler = welcome_letter_show_ADDRESS;
	welcome_letter_show();
}

/* Shows its letter of WELCOME! (font 346): appears 2 frames after the one before it, cycles its colours
   (step_palette, from 006DCC) and ends 45 frames after the start. */
void welcome_letter_show(void)
{
	uint32_t animation = 0, letter_index = 0, place = 0;
	uint16_t frames_left = 0, delay_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	letter_index = welcome_letter_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x346u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x18u;
		letter_index = OBJECT(current_object)->welcome_letter.index;
		place = welcome_letter_places_ADDRESS + letter_index * 4u;
		OBJECT(current_object)->x = WELCOME_LETTER_PLACE(place)->x;
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->frame = WELCOME_LETTER_PLACE(place)->letter;
		letter_index = (uint16_t)(letter_index * 2u);
		OBJECT(current_object)->welcome_letter.delay = letter_index;
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
		draw_object(letter_index);
		step_palette();
		return;
	default:
		invalid_jump(welcome_letter_state_jumps[state]);
	}
	OBJECT(current_object)->welcome_letter.frames_left--;
	delay_left = OBJECT(current_object)->welcome_letter.delay - 1u;
	OBJECT(current_object)->welcome_letter.delay = delay_left;
	if ((int16_t)delay_left >= 0)
		return;
	OBJECT(current_object)->state++;
	frames_left = OBJECT(current_object)->welcome_letter.frames_left;
	OBJECT(current_object)->welcome_letter.frames_left = frames_left - 1u;
	if (frames_left == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(letter_index);
	step_palette();
}

/* "To Select A Car," at the top of the car select, by state: 0 starts the four pieces of the
   instructions under it (select_instruction, records at 006DF6) and sets it up; 1 drawn 20 frames
   (timer); 2 hidden 10 frames; it ends as soon as the car is chosen (car_select car_chosen). */
void select_car_heading_show(void)
{
	uint32_t animation = 0, object = 0, piece = 0, routine = 0;
	uint16_t timer = 0, timer_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = select_car_heading_state_jumps[state];
	switch (state)
	{
	case 0:
		piece = select_instruction_pieces_ADDRESS;
		while (SELECT_INSTRUCTION_PIECE(piece)->animation != 0u)
		{
			object = start_object();
			OBJECT(object)->handler = select_instruction_show_ADDRESS;
			OBJECT(object)->select_instruction.car_select = OBJECT(current_object)->select_car_heading.car_select;
			OBJECT(object)->select_instruction.piece_animation = SELECT_INSTRUCTION_PIECE(piece)->animation;
			piece += 2u;
			OBJECT(object)->palette = SELECT_INSTRUCTION_PIECE(piece - 0x2u)->palette;
			piece += 2u;
			OBJECT(object)->x = SELECT_INSTRUCTION_PIECE(piece - 0x4u)->x;
			piece += 2u;
			OBJECT(object)->y = SELECT_INSTRUCTION_PIECE(piece - 0x6u)->y;
			piece += 2u;
		}
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x2B5u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2A6u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x30u;
		OBJECT(current_object)->select_car_heading.timer = 0x14u;
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->select_car_heading.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->select_car_heading.timer = (OBJECT(current_object)->select_car_heading.timer & 0xFF00u) | 0xAu;
			OBJECT(current_object)->state++;
		}
		routine = draw_object(routine);
		return;
	case 1:
		if (OBJECT(OBJECT(current_object)->select_car_heading.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->select_car_heading.timer = (OBJECT(current_object)->select_car_heading.timer & 0xFF00u) | 0xAu;
			OBJECT(current_object)->state++;
		}
		routine = draw_object(routine);
		return;
	case 2:
		if (OBJECT(OBJECT(current_object)->select_car_heading.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		timer_2 = OBJECT(current_object)->select_car_heading.timer;
		OBJECT(current_object)->select_car_heading.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->select_car_heading.timer = (OBJECT(current_object)->select_car_heading.timer & 0xFF00u) | 0x14u;
			OBJECT(current_object)->state--;
		}
		return;
	default:
		invalid_jump(select_car_heading_state_jumps[state]);
	}
}

/* A piece of the car select's instructions ("Turn your", the steering wheel, "Then foot the", the
   accelerator; piece_animation, palette and place from select_car_heading), by state: 0 starts its
   animation; 1 drawn and animated until the car is chosen (car_select car_chosen); 2 rises 2 a frame for
   16 frames (timer), over the transmission choice; 3 drawn and animated until the transmission is chosen
   (transmission_chosen), when it ends. */
void select_instruction_show(void)
{
	uint32_t ignored = 0, piece_animation = 0, routine = 0;
	uint16_t timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = select_instruction_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		piece_animation = OBJECT(current_object)->select_instruction.piece_animation;
		animation_start(piece_animation);
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.car_chosen == 0u)
		{
			draw_object(routine);
			ignored = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->select_instruction.timer = 0x10u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.car_chosen == 0u)
		{
			draw_object(routine);
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
			draw_object(routine);
			ignored = animation_advance(0u);
			return;
		}
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(routine);
		ignored = animation_advance(0u);
		return;
	case 2:
		break;
	case 3:
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(routine);
		ignored = animation_advance(0u);
		return;
	default:
		invalid_jump(select_instruction_state_jumps[state]);
	}
	OBJECT(current_object)->y -= 2u;
	timer = OBJECT(current_object)->select_instruction.timer;
	OBJECT(current_object)->select_instruction.timer = timer - 1u;
	if (timer != 1u)
	{
		draw_object(routine);
		ignored = animation_advance(0u);
		return;
	}
	OBJECT(current_object)->state++;
	if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(routine);
	ignored = animation_advance(0u);
}

/* The transmission choice of the car select: the AUTOMATIC and MANUAL panels (on the second tile layer,
   with the sprites automatic_panel and manual_panel following x and y), by state: 0 starts the two
   panels; 1 slides them down into view, 10 a frame for 14 frames (y, car_select
   second_scroll_y_address); 2 when the wheel picks the other transmission (car transmission against
   shown_transmission) queues the panels' colours for it (queue_palette_list 13F998 automatic, 13F95C
   manual); when it is chosen (car_select transmission_chosen) starts the driver couple (driver_couple)
   and 3 slides the panels aside, 10 a frame for 10 frames (slide_step, slide_frames; x,
   second_scroll_x_address), the chosen one to the half the couple leaves free; 4 stays. From 2 on, every
   frame, the colours of the transmission picked or chosen cycle: the next palette list of
   automatic_panel_palette_lists (automatic, automatic_cycle) or manual_panel_palette_lists (manual,
   manual_cycle) every two frames. */
void transmission_choice_run(void)
{
	uint8_t couple_left_side = 0;
	uint16_t slide_step = 0;
	uint32_t colour_list = 0, object = 0, transmission = 0;
	uint8_t slide_frames = 0;
	uint16_t panels_y = 0;
	uint32_t second_scroll_y_address = 0, second_scroll_x_address = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = automatic_panel_show_ADDRESS;
		OBJECT(object)->automatic_panel.choice = current_object;
		object = start_object();
		OBJECT(object)->handler = manual_panel_show_ADDRESS;
		OBJECT(object)->manual_panel.choice = current_object;
		OBJECT(current_object)->transmission_choice.shown_transmission = 0xFFu;
		OBJECT(current_object)->state++;
	case 1:
		object = OBJECT(current_object)->transmission_choice.car_select;
		OBJECT(current_object)->y -= 0x10u;
		second_scroll_y_address = OBJECT(object)->car_select.second_scroll_y_address;
		panels_y = mem_read16(second_scroll_y_address);
		mem_write16(second_scroll_y_address, panels_y - 0x10u);
		if (panels_y != 0x10u)
			return;
		OBJECT(current_object)->state++;
	case 2:
		if (OBJECT(OBJECT(current_object)->transmission_choice.car_select)->car_select.transmission_chosen != 0u)
		{
			OBJECT(current_object)->transmission_choice.slide_step = 0x10;
			couple_left_side = 0u;
			if (CAR(player_car)->transmission == 0u)
			{
				OBJECT(current_object)->transmission_choice.slide_step = (int8_t)0xF0u;
				couple_left_side = 1u;
			}
			OBJECT(current_object)->transmission_choice.slide_frames = 0xAu;
			object = start_object();
			OBJECT(object)->handler = driver_couple_start_ADDRESS;
			OBJECT(object)->driver_couple.left_side = couple_left_side;
			OBJECT(current_object)->state++;
			object = OBJECT(current_object)->transmission_choice.car_select;
			slide_step = (uint16_t)OBJECT(current_object)->transmission_choice.slide_step;
			OBJECT(current_object)->x += slide_step;
			second_scroll_x_address = OBJECT(object)->car_select.second_scroll_x_address;
			mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) + slide_step);
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
				colour_list = automatic_picked_palettes_ADDRESS;
				if (transmission != 0u)
					colour_list = manual_picked_palettes_ADDRESS;
				queue_palette_list(colour_list);
			}
		}
		break;
	case 3:
		object = OBJECT(current_object)->transmission_choice.car_select;
		slide_step = (uint16_t)OBJECT(current_object)->transmission_choice.slide_step;
		OBJECT(current_object)->x += slide_step;
		second_scroll_x_address = OBJECT(object)->car_select.second_scroll_x_address;
		mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) + slide_step);
		slide_frames = OBJECT(current_object)->transmission_choice.slide_frames;
		OBJECT(current_object)->transmission_choice.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 4:
		break;
	default:
		invalid_jump(transmission_choice_state_jumps[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission == 0u)
	{
		colour_list = automatic_panel_palette_lists[OBJECT(current_object)->transmission_choice.automatic_cycle >> 1];
		if (colour_list == 0u)
		{
			OBJECT(current_object)->transmission_choice.automatic_cycle = 0u;
			colour_list = automatic_panel_palette_lists[0];
		}
		OBJECT(current_object)->transmission_choice.automatic_cycle++;
		queue_palette_list(colour_list);
		return;
	}
	colour_list = manual_panel_palette_lists[OBJECT(current_object)->transmission_choice.manual_cycle >> 1];
	if (colour_list == 0u)
	{
		OBJECT(current_object)->transmission_choice.manual_cycle = 0u;
		colour_list = manual_panel_palette_lists[0];
	}
	OBJECT(current_object)->transmission_choice.manual_cycle++;
	queue_palette_list(colour_list);
}

/* The AUTOMATIC TRANSMISSION title of the left panel of the transmission choice, by state
   (automatic_panel_state_jumps): 0 starts its label (transmission_label_start_automatic) and sets it up;
   1 cycles its colours (step_palette from palette_list) while the wheel picks automatic (car
   transmission 0), else palette 2AE; drawn at x_offset, y_offset less the choice's x, y
   (transmission_choice), which slide it. Nothing sets state 2 (drawing only). */
void automatic_panel_show(void)
{
	uint32_t animation = 0, object = 0, panel_y = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = transmission_label_start_automatic_ADDRESS;
		OBJECT(object)->transmission_label.panel = current_object;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		animation = 0x2BDu;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->x_offset = 0x54;
		OBJECT(current_object)->y_offset = -0x6C;
		OBJECT(current_object)->automatic_panel.palette_list = 0x6E18u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
		OBJECT(current_object)->y = panel_y;
		panel_y = draw_object(panel_y);
		return;
	default:
		invalid_jump(automatic_panel_state_jumps[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission != 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
		OBJECT(current_object)->y = panel_y;
		panel_y = draw_object(panel_y);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->automatic_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
	panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
	OBJECT(current_object)->y = panel_y;
	panel_y = draw_object(panel_y);
}

/* The MANUAL TRANSMISSION title of the right panel of the transmission choice, by state
   (manual_panel_state_jumps): 0 starts its label (transmission_label_start_manual) and sets it up; 1
   cycles its colours (step_palette from palette_list) while the wheel picks manual (car transmission 1),
   else palette 2AE; drawn at x_offset, y_offset less the choice's x, y (transmission_choice), which
   slide it. Nothing sets state 2 (drawing only). */
void manual_panel_show(void)
{
	uint32_t animation = 0, object = 0, panel_y = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = transmission_label_start_manual_ADDRESS;
		OBJECT(object)->transmission_label.panel = current_object;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		animation = 0x2BEu;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->x_offset = 0xEC;
		OBJECT(current_object)->y_offset = -0x6C;
		OBJECT(current_object)->manual_panel.palette_list = 0x6E18u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
		OBJECT(current_object)->y = panel_y;
		panel_y = draw_object(panel_y);
		return;
	default:
		invalid_jump(manual_panel_state_jumps[OBJECT(current_object)->state]);
	}
	if (CAR(player_car)->transmission == 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
		OBJECT(current_object)->y = panel_y;
		panel_y = draw_object(panel_y);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->manual_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
	panel_y = (uint16_t)(0u - OBJECT(object)->y + OBJECT(current_object)->y_offset);
	OBJECT(current_object)->y = panel_y;
	panel_y = draw_object(panel_y);
}

/* Sets up the label of the automatic transmission's panel (sprite set 2C1, 24 below the panel's place;
   the frames show NOVICE there), then transmission_label_follow. */
void transmission_label_start_automatic(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	animation = 0x2C1u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x2A8u;
	OBJECT(current_object)->x_offset = 0;
	OBJECT(current_object)->y_offset = 0x24;
	OBJECT(current_object)->handler = transmission_label_follow_ADDRESS;
	transmission_label_follow();
}

/* Sets up the label of the manual transmission's panel (sprite set 2C9, its unk_40 from
   car_gears_caption_frames by the car's model; the frames show 5 SPEED there for the car coin_start
   takes), then transmission_label_follow. */
void transmission_label_start_manual(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	animation = 0x2C9u;
	animation_start(animation);
	OBJECT(current_object)->frame = car_gears_caption_frames[CAR(player_car)->model];
	OBJECT(current_object)->palette = 0x2A8u;
	OBJECT(current_object)->x_offset = -0x18;
	OBJECT(current_object)->y_offset = 0x5C;
	OBJECT(current_object)->handler = transmission_label_follow_ADDRESS;
	transmission_label_follow();
}

/* Keeps the label on its panel (the panel's place plus its offsets, the panel's palette) and draws it;
   ends when the panel ends. */
void transmission_label_follow(void)
{
	uint32_t label_y = 0, object = 0;

	object = OBJECT(current_object)->transmission_label.panel;
	if ((OBJECT(object)->flags & 1u) == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->x = OBJECT(object)->x + OBJECT(current_object)->x_offset;
	label_y = (uint16_t)(OBJECT(object)->y + OBJECT(current_object)->y_offset);
	OBJECT(current_object)->y = label_y;
	OBJECT(current_object)->palette = OBJECT(object)->palette;
	label_y = draw_object(label_y);
}

/* Sets up the couple who drive the chosen car (a man and a woman, by the car's model: 006E58) on the
   left (x 50) after automatic (left_side) or on the right (F0) after manual, the half the panels slid
   away from; draw_object then draws it as its handler. */
void driver_couple_start(void)
{
	uint32_t animation = 0, look = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0xEFu;
	look = driver_couple_looks_ADDRESS + CAR(player_car)->model * 4u;
	animation = ANIMATION_PALETTE(look)->animation;
	OBJECT(current_object)->palette = ANIMATION_PALETTE(look)->palette;
	animation_start(animation);
	OBJECT(current_object)->x = 0xF0u;
	if (OBJECT(current_object)->driver_couple.left_side != 0u)
		OBJECT(current_object)->x = 0x50u;
	OBJECT(current_object)->y = 0xD0u;
	OBJECT(current_object)->handler = draw_object_ADDRESS;
}

/* The caption of the car in front on the car select (EASY HANDLING, SMOOTH OPERATOR...: animation 26E,
   frame car model, palette car_caption_palettes), by state: 0 starts the gearboxes under it
   (car_gears_caption) and sets it up; 1 drawn 10 frames (timer); 2 hidden 10 frames; a new car in front
   (shown_car) shows it at once, for 10 frames more; it ends when the car is chosen (car_select
   car_chosen). */
void car_caption_show(void)
{
	uint8_t model = 0;
	uint32_t animation = 0, car_model = 0, object = 0;
	uint16_t timer = 0, timer_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = car_gears_caption_show_ADDRESS;
		OBJECT(object)->car_gears_caption.car_select = OBJECT(current_object)->car_caption.car_select;
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x26Eu;
		animation_start(animation);
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xC4u;
		OBJECT(current_object)->car_caption.timer = 0xAu;
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->car_caption.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		car_model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_caption.shown_car != car_model)
		{
			OBJECT(current_object)->car_caption.shown_car = car_model;
			OBJECT(current_object)->car_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_caption.timer;
		OBJECT(current_object)->car_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = car_model;
		OBJECT(current_object)->palette = car_caption_palettes[car_model];
		car_model = draw_object(car_model);
		return;
	case 1:
		if (OBJECT(OBJECT(current_object)->car_caption.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		car_model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_caption.shown_car != car_model)
		{
			OBJECT(current_object)->car_caption.shown_car = car_model;
			OBJECT(current_object)->car_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_caption.timer;
		OBJECT(current_object)->car_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = car_model;
		OBJECT(current_object)->palette = car_caption_palettes[car_model];
		car_model = draw_object(car_model);
		return;
	case 2:
		if (OBJECT(OBJECT(current_object)->car_caption.car_select)->car_select.car_chosen != 0u)
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
		invalid_jump(car_caption_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The gearboxes of the car in front under its caption (Auto/5 Speed...: animation 2D1, frame
   car_gears_caption_frames by car model, the manual box's), blinking like car_caption: 1 drawn 10 frames
   (timer), 2 hidden 10, at once for a new car (shown_car); it ends when the car is chosen (car_select
   car_chosen). */
void car_gears_caption_show(void)
{
	uint8_t model = 0;
	uint32_t animation = 0, car_model = 0;
	uint16_t timer = 0, timer_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x2D1u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2AFu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD4u;
		OBJECT(current_object)->car_gears_caption.shown_car = 0xFFu;
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		car_model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_gears_caption.shown_car != car_model)
		{
			OBJECT(current_object)->car_gears_caption.shown_car = car_model;
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_gears_caption.timer;
		OBJECT(current_object)->car_gears_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = car_gears_caption_frames[car_model];
		car_model = draw_object(car_model);
		return;
	case 1:
		if (OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->car_select.car_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		car_model = CAR(player_car)->model;
		if (OBJECT(current_object)->car_gears_caption.shown_car != car_model)
		{
			OBJECT(current_object)->car_gears_caption.shown_car = car_model;
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
		}
		timer = OBJECT(current_object)->car_gears_caption.timer;
		OBJECT(current_object)->car_gears_caption.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->car_gears_caption.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->frame = car_gears_caption_frames[car_model];
		car_model = draw_object(car_model);
		return;
	case 2:
		if (OBJECT(OBJECT(current_object)->car_gears_caption.car_select)->car_select.car_chosen != 0u)
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
		invalid_jump(car_gears_caption_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The car select's message once the choices are made, by state: 0 sets up WAITING FOR CHALLENGERS! JUST
   A MOMENT PLEASE (animation 28B); 1 drawn 20 frames (timer); 2 hidden 10; once the entry time is over
   (car_select entry_over) JUST A MOMENT PLEASE (animation 28D) instead: 3 drawn 20 frames, 4 hidden 10. */
void waiting_banner_show(void)
{
	uint32_t animation = 0, routine = 0;
	uint8_t timer = 0, timer_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = waiting_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 0x28Bu;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x287u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x88u;
		OBJECT(current_object)->waiting_banner.timer = 0x14u;
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over != 0u)
		{
			animation = 0x28Du;
			animation_start(animation);
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
		routine = draw_object(routine);
		return;
	case 1:
		if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over != 0u)
		{
			animation = 0x28Du;
			animation_start(animation);
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
		routine = draw_object(routine);
		return;
	case 3:
		timer = OBJECT(current_object)->waiting_banner.timer;
		OBJECT(current_object)->waiting_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->waiting_banner.timer = 0xAu;
			OBJECT(current_object)->state++;
		}
		routine = draw_object(routine);
		return;
	case 2:
		if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over != 0u)
		{
			animation = 0x28Du;
			animation_start(animation);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
			timer = OBJECT(current_object)->waiting_banner.timer;
			OBJECT(current_object)->waiting_banner.timer = timer - 1u;
			if (timer == 1u)
			{
				OBJECT(current_object)->waiting_banner.timer = 0xAu;
				OBJECT(current_object)->state++;
			}
			routine = draw_object(routine);
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
		invalid_jump(waiting_banner_state_jumps[state]);
	}
}

/* The other players on the car select, by state: 0 its list of the race's players (race_order: the
   count, then the players); 1 when the LCD is not scrolling a text (screen lcd_scroll) writes Entry: on
   it with _ at each player's car number (car_numbers); then every frame: this screen's number blinks
   with _ every 8 frames (own_number, digit_toggle, blink_timer); each other player not yet marked (a bit
   each from 40, the frame field) gets its number on the LCD and, once it has chosen a car (byte 48 of
   its race_copy record, which car_select writes), a flag over that car on the turntable (choice_flag,
   kept in car_flags by car; one more player on the same car gets a stacked_choice_flag on the last one)
   and its bit; flags_visible follows the blink pattern car_select_entries_flag_blink (frames >> 1). */
void car_select_entries_run(void)
{
	uint8_t own_car_number = 0;
	uint32_t car_flag = 0, chosen_car = 0, flag_blink_frames = 0, object = 0, players_left = 0, racer = 0, racer_car_number = 0, racer_place = 0, racers_left = 0;
	uint8_t blink_timer = 0;
	uint32_t own_digit_place = 0, car_choice = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->car_select_entries.race_order = SCREEN(screen)->host_record + 0x1Fu;
		OBJECT(current_object)->car_select_entries.shown_players &= 0xFFFFFF00u;
		OBJECT(current_object)->car_select_entries.frames = 0u;
		OBJECT(current_object)->state++;
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		players_left = player_count;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + players_left), 0x5Fu);
			players_left--;
		} while (players_left != 0u);
		own_car_number = car_numbers[SCREEN(screen)->player];
		OBJECT(current_object)->car_select_entries.own_number = own_car_number;
		OBJECT(current_object)->car_select_entries.digit_toggle = (own_car_number + 0x31u) ^ 0x5Fu;
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		players_left = player_count;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + players_left), 0x5Fu);
			players_left--;
		} while (players_left != 0u);
		own_car_number = car_numbers[SCREEN(screen)->player];
		OBJECT(current_object)->car_select_entries.own_number = own_car_number;
		OBJECT(current_object)->car_select_entries.digit_toggle = (own_car_number + 0x31u) ^ 0x5Fu;
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(car_select_entries_state_jumps[OBJECT(current_object)->state]);
	}
	blink_timer = OBJECT(current_object)->car_select_entries.blink_timer;
	OBJECT(current_object)->car_select_entries.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		own_digit_place = screen + 0xF18u + OBJECT(current_object)->car_select_entries.own_number;
		mem_write8(own_digit_place, mem_read8(own_digit_place) ^ OBJECT(current_object)->car_select_entries.digit_toggle);
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
	}
	racer_place = OBJECT(current_object)->car_select_entries.race_order;
	racers_left = mem_read8(racer_place);
	racer_place++;
	do
	{
		racer = mem_read8(racer_place);
		racer_place++;
		if (SCREEN(screen)->player != racer && (OBJECT(current_object)->car_select_entries.shown_players & shl32(1u, (int)(racer))) == 0u)
		{
			racer_car_number = car_numbers[racer];
			mem_write8(screen + 0xF18u + racer_car_number, racer_car_number + 0x31u);
			car_choice = mem_read8(0x20FC48u + (racer << 7)) - 1u;
			chosen_car = car_choice;
			if ((int32_t)car_choice >= 0)
			{
				car_flag = mem_read32(current_object + 0x80u + chosen_car * 4u);
				object = start_object();
				if (car_flag == 0u)
				{
					OBJECT(object)->handler = choice_flag_start_ADDRESS;
					OBJECT(object)->choice_flag.top_flag = object;
					OBJECT(object)->choice_flag.entries = current_object;
					mem_write32(current_object + 0x80u + chosen_car * 4u, object);
				}
				else
				{
					OBJECT(object)->handler = stacked_choice_flag_start_ADDRESS;
					OBJECT(object)->stacked_choice_flag.below = mem_read32(car_flag + 0x18u);
					mem_write32(car_flag + 0x18u, object);
				}
				OBJECT(object)->unk_09 = racer;
				OBJECT(object)->unk_0A = chosen_car;
				OBJECT(current_object)->car_select_entries.shown_players |= shl32(1u, (int)(racer));
			}
		}
		racers_left--;
	} while (racers_left != 0u);
	flag_blink_frames = OBJECT(current_object)->car_select_entries.frames;
	flag_blink_frames = (uint8_t)(flag_blink_frames + 1u);
	OBJECT(current_object)->car_select_entries.frames = flag_blink_frames;
	flag_blink_frames = flag_blink_frames >> 1;
	OBJECT(current_object)->car_select_entries.flags_visible = car_select_entries_flag_blink[flag_blink_frames & 0xFu];
}
