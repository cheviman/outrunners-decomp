/**
 *    Name:        system/coins.c
 *    Description: the coins and credits, and the bookkeeping
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets up the coins: chute_entry_offset 14 (the two chutes shared: the second's coins counted as the
   first's, on its meter) when not on free play and both chutes give the same units
   (first_chute_multiplier = second_chute_multiplier), else 0; the seats' credits back from the
   bookkeeping (701F74-701F7A, bytes); and the low four bits of I/O chip 1's port F, inverted, into
   second_io_chip_port_f (nothing reads it). */
void init_coins(void)
{
	uint8_t chip_2_port_f = 0;

	chip_2_port_f = io_read8(IO_2_PORT_F);
	second_io_chip_port_f = ~chip_2_port_f & 0xFu;
	chute_entry_offset = 0u;
	if (free_play == 0u && second_chute_multiplier == first_chute_multiplier)
		chute_entry_offset = 0x14u;
	seat1_credits = (uint8_t)BOOKKEEPING_SEAT_1_CREDITS;
	seat1_bookkeeping_byte = (uint8_t)BOOKKEEPING_SEAT_1_BYTE;
	seat2_credits = (uint8_t)BOOKKEEPING_SEAT_2_CREDITS;
	seat2_bookkeeping_byte = (uint8_t)BOOKKEEPING_SEAT_2_BYTE;
}

/* The coins and service credits, in the vertical blank unless on free play (free_play): queues each
   seat's pending coin sound (4E, seat1_coin_sounds, seat2_coin_sounds) and service sound (50,
   seat1_service_sounds, seat2_service_sounds), one each vertical blank; SERVICE newly pressed
   (switches_pressed bits 0-1) gives that seat a credit (add_service_credit), counted in the
   bookkeeping's service credits (701F04); a coin chute let go (switches_released bits 2-3 seat 1's two
   chutes, 4-5 seat 2's) counts its coin (insert_coin, entries at 0114D4) and the credits it gives in the
   bookkeeping (701F02); then drives the coin meters (pulse_coin_meter: each seat's first, and its second
   unless the chutes are shared, chute_entry_offset). */
void update_coins(void)
{
	uint8_t meter_bit = 0, pressed_switches = 0, released_switches = 0, sound = 0;
	uint16_t credits_given = 0;
	uint32_t chute_entry = 0, coin_meter = 0, counter = 0, seat_credits = 0, service_sounds = 0;
	uint32_t saved_screen = 0;
	uint8_t released_chutes = 0;

	if (free_play != 0u)
		return;
	saved_screen = screen;
	if (seat1_coin_sounds != 0u)
	{
		sound = 0x4Eu;
		screen = SCREEN_1;
		queue_sound(sound);
		seat1_coin_sounds--;
	}
	if (seat2_coin_sounds != 0u)
	{
		sound = 0x4Eu;
		screen = SCREEN_2;
		queue_sound(sound);
		seat2_coin_sounds--;
	}
	if (seat1_service_sounds != 0u)
	{
		sound = 0x50u;
		screen = SCREEN_1;
		queue_sound(sound);
		seat1_service_sounds--;
	}
	if (seat2_service_sounds != 0u)
	{
		sound = 0x50u;
		screen = SCREEN_2;
		queue_sound(sound);
		seat2_service_sounds--;
	}
	screen = saved_screen;
	pressed_switches = switches_pressed;
	credits_given = 0u;
	if ((pressed_switches & 1u) != 0u)
	{
		seat_credits = seat1_credits_ADDRESS;
		service_sounds = seat1_service_sounds_ADDRESS;
		r12 = SCREEN_1;
		credits_given = add_service_credit(credits_given, seat_credits, service_sounds);
		counter = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(counter, credits_given);
	}
	credits_given = 0u;
	if ((pressed_switches & 2u) != 0u)
	{
		seat_credits = seat2_credits_ADDRESS;
		service_sounds = seat2_service_sounds_ADDRESS;
		r12 = SCREEN_2;
		credits_given = add_service_credit(credits_given, seat_credits, service_sounds);
		counter = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(counter, credits_given);
	}
	released_chutes = switches_released & 0x3Cu;
	released_switches = released_chutes;
	if (released_chutes != 0u)
	{
		credits_given = 0u;
		r12 = SCREEN_1;
		chute_entry = seat1_first_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		chute_entry = seat1_second_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		r12 = SCREEN_2;
		chute_entry = seat2_first_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		chute_entry = seat2_second_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		counter = SOUND_RAM + 0x1F02u;
		bookkeeping_add16(counter, credits_given);
	}
	if (chute_entry_offset == 0u)
	{
		coin_meter = 0x20E083u;
		meter_bit = 1u;
		r12 = port_d_outputs_ADDRESS;
		pulse_coin_meter(meter_bit, coin_meter);
	}
	coin_meter = coin_meters_ADDRESS;
	meter_bit = 0u;
	r12 = port_d_outputs_ADDRESS;
	pulse_coin_meter(meter_bit, coin_meter);
	if (chute_entry_offset == 0u)
	{
		coin_meter = 0x20E089u;
		meter_bit = 1u;
		r12 = port_d_outputs_ADDRESS + 0x1u;
		pulse_coin_meter(meter_bit, coin_meter);
	}
	coin_meter = 0x20E086u;
	meter_bit = 0u;
	r12 = port_d_outputs_ADDRESS + 0x1u;
	pulse_coin_meter(meter_bit, coin_meter);
}

/* Drives coin meter R1 (its bit in the port D outputs at R12, port_d_outputs: bit 0 a seat's first
   chute's meter, bit 1 its second's) from its record R7 in coin_meters (frames on, frames off, pulses
   owed): when idle with a pulse owed, the bit goes on for 6 vertical blanks and off for 6. Then writes
   the credits into the bookkeeping and its backup (write_bookkeeping_credits) with the checksum. */
void pulse_coin_meter(uint8_t meter_bit, uint32_t coin_meter)
{
	uint32_t meter_frames = 0;
	uint8_t frames_on = 0;

	meter_frames = COIN_METER(coin_meter)->frames;
	coin_meter += 2u;
	if (meter_frames == 0u)
	{
		if (COIN_METER(coin_meter - 0x2u)->pulses_owed != 0u)
		{
			COIN_METER(coin_meter - 0x2u)->pulses_owed--;
			PORT_OUTPUT_BITS(r12)[0] |= shift_by(1u, meter_bit, 8);
			COIN_METER(coin_meter - 0x2u)->frames = 0x606u;
		}
		r12 = BOOKKEEPING_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		bookkeeping_update_checksum((uint16_t)meter_frames);
		r12 = BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		return;
	}
	if ((uint8_t)meter_frames == 0u)
	{
		coin_meter--;
		COIN_METER(coin_meter - 0x1u)->frames_off--;
		r12 = BOOKKEEPING_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		bookkeeping_update_checksum((uint16_t)meter_frames);
		r12 = BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		return;
	}
	frames_on = COIN_METER(coin_meter - 0x2u)->frames_on;
	COIN_METER(coin_meter - 0x2u)->frames_on = frames_on - 1u;
	if (frames_on != 1u)
	{
		r12 = BOOKKEEPING_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		bookkeeping_update_checksum((uint16_t)meter_frames);
		r12 = BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS;
		write_bookkeeping_credits();
		return;
	}
	PORT_OUTPUT_BITS(r12)[0] &= rotate_by(0xFEu, meter_bit, 8);
	r12 = BOOKKEEPING_SEAT_1_CREDITS_ADDRESS;
	write_bookkeeping_credits();
	bookkeeping_update_checksum((uint16_t)meter_frames);
	r12 = BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS;
	write_bookkeeping_credits();
}

/* Writes the seats' credits (the halfwords at seat1_credits, seat1_bookkeeping_byte, seat2_credits,
   seat2_bookkeeping_byte) at R12: 701F74 in the bookkeeping or 701FF0 in its backup. */
void write_bookkeeping_credits(void)
{
	BOOKKEEPING_HALFWORDS(r12)[0] = (seat1_credits | ((uint32_t)seat1_coin_units << 8u));
	r12 += 2u;
	BOOKKEEPING_HALFWORDS(r12)[0] = AS16(seat1_bookkeeping_byte);
	r12 += 2u;
	BOOKKEEPING_HALFWORDS(r12)[0] = (seat2_credits | ((uint32_t)seat2_coin_units << 8u));
	r12 += 2u;
	BOOKKEEPING_HALFWORDS(r12)[0] = AS16(seat2_bookkeeping_byte);
	r12 += 2u;
}

/* A service credit for the seat whose credits are at R10 (seat1_credits or seat2_credits): one credit
   more, up to 9 (at 9 its coin units are cleared; beyond, nothing); a credit given brings the screen R12
   to the title from the attract mode (show_title_on_credit), asks for the seat's service sound (R11) and
   counts one in R9, which it returns. Not run in the scenarios. */
uint16_t add_service_credit(uint16_t credits_given, uint32_t seat_credits, uint32_t service_sounds)
{
	uint32_t zero = 0;
	uint8_t credits = 0;

	SEAT_CREDITS(seat_credits)->credits++;
	credits = SEAT_CREDITS(seat_credits)->credits;
	zero = (uint8_t)(credits - 9u) == 0u;
	if (credits >= 9u)
	{
		SEAT_CREDITS(seat_credits)->bonus_units = 0u;
		SEAT_CREDITS(seat_credits)->coin_units = 0u;
		if (zero == 0u)
		{
			SEAT_CREDITS(seat_credits)->credits = 9u;
			return credits_given;
		}
	}
	show_title_on_credit();
	SEAT_SOUND_COUNTS(service_sounds)[0] = 1u;
	credits_given = (uint8_t)(credits_given + 1u);
	return credits_given;
}

/* A coin from the chute of entry R19 (when its bit is in R6): takes the entry's variant
   (chute_entry_offset: with the chutes shared, the second counts as the first), writes its byte to I/O
   C00074, owes the chute's coin meter a pulse (coin_meters) and counts the coin in the bookkeeping
   (701F06-701F0C, bookkeeping_increment16); with fewer than 9 credits: the screen's coin sound, and the
   chute's coin units (first_chute_multiplier or second_chute_multiplier) go toward a bonus unit (the
   credit record's third byte; one more unit each bonus_adder, when set) and toward a credit (its second
   byte, seat1_coin_units: a credit each coins_per_credit, up to 9, show_title_on_credit). Returns R9
   plus the credits given. */
uint16_t insert_coin(uint8_t released_switches, uint16_t credits_given, uint32_t chute_entry)
{
	uint8_t bonus_units = 0, chute_units = 0, credit_units = 0, units_per_credit = 0;
	uint32_t coin_sounds = 0, counter = 0, seat_credits = 0, units_per_bonus = 0;
	uint32_t switch_bit = 0, owed_pulses = 0;

	switch_bit = COIN_CHUTE(chute_entry)->switch_bit;
	chute_entry += 4u;
	if ((released_switches & shl32(1u, (int)(switch_bit))) == 0u)
		return credits_given;
	coin_sounds = COIN_CHUTE(chute_entry - 0x4u)->coin_sounds;
	chute_entry = chute_entry + 4u + chute_entry_offset;
	seat_credits = COIN_CHUTE_VARIANT(chute_entry)->credits;
	io_write8(EVENT_CODE, COIN_CHUTE_VARIANT(chute_entry)->event_code);
	owed_pulses = COIN_CHUTE_VARIANT(chute_entry)->pulses_owed;
	COIN_METER(owed_pulses - 0x2u)->pulses_owed++;
	counter = COIN_CHUTE_VARIANT(chute_entry)->counter;
	bookkeeping_increment16(counter);
	if (SEAT_CREDITS(seat_credits)->credits >= 9u)
		return credits_given;
	SEAT_SOUND_COUNTS(coin_sounds)[0]++;
	chute_units = CHUTE_UNIT_SETTINGS(COIN_CHUTE_VARIANT(chute_entry)->coin_units)[0];
	units_per_bonus = bonus_adder;
	if (units_per_bonus != 0u)
	{
		bonus_units = (uint8_t)(SEAT_CREDITS(seat_credits)->bonus_units + chute_units);
		while (units_per_bonus <= bonus_units)
		{
			SEAT_CREDITS(seat_credits)->coin_units++;
			bonus_units -= units_per_bonus;
		}
		SEAT_CREDITS(seat_credits)->bonus_units = bonus_units;
	}
	units_per_credit = coins_per_credit;
	credit_units = (uint8_t)(SEAT_CREDITS(seat_credits)->coin_units + chute_units);
	do
	{
		if (credit_units < units_per_credit)
		{
			SEAT_CREDITS(seat_credits)->coin_units = credit_units;
			return credits_given;
		}
		SEAT_CREDITS(seat_credits)->credits++;
		show_title_on_credit();
		credits_given = (uint8_t)(credits_given + 1u);
		credit_units -= units_per_credit;
	} while (SEAT_CREDITS(seat_credits)->credits < 9u);
	credit_units = 0u;
	SEAT_CREDITS(seat_credits)->bonus_units = 0u;
	SEAT_CREDITS(seat_credits)->coin_units = credit_units;
	return credits_given;
}

/* A credit on the screen R12 during the attract mode (game modes 1-5, the low byte of its game_mode)
   brings it to the title: game_mode = 2 (game mode 2, game_mode_title, from its first step). */
void show_title_on_credit(void)
{
	if ((int8_t)SCREEN(r12)->game_mode > 0 && (int8_t)SCREEN(r12)->game_mode < 6)
		AS32(SCREEN(r12)->game_mode) = 2u;
}

/* Adds one to the 16-bit bookkeeping counter at R7 (and its backup's, 7C on) unless it is at FFFF, with
   the checksum: the coins by chute (insert_coin), the games by seat and the races' results
   (game_mode_play). */
void bookkeeping_increment16(uint32_t counter)
{
	if (BOOKKEEPING_COUNT16(counter)->value != 0xFFFFu)
	{
		BOOKKEEPING_COUNT16(counter)->value++;
		bookkeeping_update_checksum(0u);
		BOOKKEEPING_COUNT16(counter)->backup++;
	}
}

/* Unused: adds one to the 8-bit bookkeeping counter at R7 (and its backup's, 7C on) unless it is at FF,
   with the checksum (bookkeeping_update_checksum). */
void bookkeeping_increment8(uint32_t counter_address)
{
	if (BOOKKEEPING_COUNT8(counter_address)->value != 0xFFu)
	{
		BOOKKEEPING_COUNT8(counter_address)->value++;
		bookkeeping_update_checksum(0u);
		BOOKKEEPING_COUNT8(counter_address)->backup++;
	}
}

/* Adds R9 to the 16-bit bookkeeping counter at R7 (the credits from coins, 701F02, and the service
   credits, 701F04: update_coins) and to its backup's (7C on), with the checksum; past FFFF it stays at
   FFFF (and the backup's is left as it was: the code writes R7 twice). */
void bookkeeping_add16(uint32_t counter, uint16_t amount)
{
	uint32_t carry = 0;
	uint16_t count = 0;

	count = BOOKKEEPING_COUNT16(counter)->value;
	carry = (uint16_t)(count + amount) < count;
	BOOKKEEPING_COUNT16(counter)->value = count + amount;
	if (carry)
	{
		BOOKKEEPING_COUNT16(counter)->value = 0xFFFFu;
		bookkeeping_update_checksum(0u);
		BOOKKEEPING_COUNT16(counter)->value = 0xFFFFu;
		return;
	}
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT16(counter)->backup += amount;
}

/* Adds R9 to the 32-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum; past
   FFFFFFFF it stays there (the backup's left as it was): the play time by seat (game_mode_play). */
void bookkeeping_add32(uint32_t counter, uint32_t amount)
{
	uint32_t carry = 0;
	uint32_t count = 0;

	count = BOOKKEEPING_COUNT32(counter)->value;
	carry = count + amount < count;
	BOOKKEEPING_COUNT32(counter)->value = count + amount;
	if (carry)
	{
		BOOKKEEPING_COUNT32(counter)->value = 0xFFFFFFFFu;
		bookkeeping_update_checksum(0u);
		BOOKKEEPING_COUNT32(counter)->value = 0xFFFFFFFFu;
		return;
	}
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT32(counter)->backup += amount;
}

/* Unused: adds R9 to the 8-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum;
   past FF it stays at FF (and the backup's is left as it was: the code writes R7 twice), as
   bookkeeping_add16 does. */
void bookkeeping_add8(uint32_t counter, uint8_t amount)
{
	uint32_t carry = 0;
	uint8_t count = 0;

	count = BOOKKEEPING_COUNT8(counter)->value;
	carry = (uint8_t)(count + amount) < count;
	BOOKKEEPING_COUNT8(counter)->value = count + amount;
	if (carry)
	{
		BOOKKEEPING_COUNT8(counter)->value = 0xFFu;
		bookkeeping_update_checksum(0u);
		BOOKKEEPING_COUNT8(counter)->value = 0xFFu;
		return;
	}
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT8(counter)->backup += amount;
}

/* Sets the 16-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum: the longest
   and shortest play (701F20, 701F22; game_mode_play). */
void bookkeeping_set16(uint32_t counter, uint16_t amount)
{
	BOOKKEEPING_COUNT16(counter)->value = amount;
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT16(counter)->backup = amount;
}

/* Unused: sets the 32-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set32(uint32_t counter, uint32_t amount)
{
	BOOKKEEPING_COUNT32(counter)->value = amount;
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT32(counter)->backup = amount;
}

/* Unused: sets the 8-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set8(uint32_t counter, uint8_t amount)
{
	BOOKKEEPING_COUNT8(counter)->value = amount;
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_COUNT8(counter)->backup = amount;
}

/* Clears the credits kept in the bookkeeping (701F74-701F7B) and in its backup (701FF0-701FF7), with the
   checksum. Not run in the scenarios. */
void bookkeeping_clear_credits(void)
{
	AS32(BOOKKEEPING_SEAT_1_CREDITS) = 0u;
	AS32(BOOKKEEPING_SEAT_2_CREDITS) = 0u;
	bookkeeping_update_checksum(0u);
	BOOKKEEPING_BACKUP_SEAT_1_CREDITS = 0u;
	BOOKKEEPING_BACKUP_SEAT_2_CREDITS = 0u;
	r12 = SOUND_RAM + 0x1FF8u;
}

/* Checks the bookkeeping at boot: when the block (701F02) matches the checksum (701F00), copies it over
   the backup (701F7E); else when the backup matches, the backup over the block; else resets both
   (bookkeeping_reset). */
void bookkeeping_check(void)
{
	uint16_t stored_checksum = 0;
	uint32_t block = 0, checksum = 0, destination = 0, source = 0;

	stored_checksum = BOOKKEEPING_CHECKSUM;
	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(0u, block);
	if (checksum == stored_checksum)
	{
		source = SOUND_RAM + 0x1F02u;
		destination = SOUND_RAM + 0x1F7Eu;
		bookkeeping_copy(checksum, source, destination);
		return;
	}
	block = SOUND_RAM + 0x1F7Eu;
	checksum = bookkeeping_checksum(checksum, block);
	if (checksum == stored_checksum)
	{
		destination = SOUND_RAM + 0x1F02u;
		source = SOUND_RAM + 0x1F7Eu;
		bookkeeping_copy(checksum, source, destination);
		return;
	}
	bookkeeping_reset(checksum);
}

/* Resets the bookkeeping: the defaults at 011458 into the block (701F02), then the block into its backup
   (701F7E), with the checksum. */
uint16_t bookkeeping_reset(uint32_t checksum)
{
	uint32_t destination = 0, source = 0;

	source = bookkeeping_defaults_ADDRESS;
	destination = SOUND_RAM + 0x1F02u;
	bookkeeping_copy(checksum, source, destination);
	source = SOUND_RAM + 0x1F02u;
	destination = SOUND_RAM + 0x1F7Eu;
	return bookkeeping_copy(checksum, source, destination);
}

/* Copies 3E halfwords from R2 over R3 (the bookkeeping and its backup at 701F7E, either way, or the
   defaults), then bookkeeping_update_checksum. */
uint16_t bookkeeping_copy(uint32_t checksum, uint32_t source, uint32_t destination)
{
	move_halfwords(destination, source, 0x3Eu);
	return bookkeeping_update_checksum((uint16_t)checksum);
}

/* Writes the checksum of the bookkeeping (701F02-701F7D in the shared sound RAM) into 701F00, after a
   change to it; returns it. */
uint16_t bookkeeping_update_checksum(uint16_t checksum)
{
	uint32_t block = 0;

	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(checksum, block);
	BOOKKEEPING_CHECKSUM = checksum;
	return checksum;
}

/* Unused: checks the bookkeeping block (701F02) against its checksum (701F00) and resets both it and its
   backup (bookkeeping_reset) when they differ; unlike bookkeeping_check it does not try the backup. */
void bookkeeping_check_block(void)
{
	uint32_t block = 0, checksum = 0;

	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(0u, block);
	if (BOOKKEEPING_CHECKSUM != checksum)
	{
		bookkeeping_reset(checksum);
		return;
	}
}

/* Returns the checksum of the 3E halfwords at R2 (the bookkeeping or its backup): from 1, each halfword
   XORed in, then a shift right by one, with 8810 XORed in when the bit shifted out was 1. */
uint16_t bookkeeping_checksum(uint32_t checksum, uint32_t block)
{
	uint32_t carry = 0;
	uint16_t sum = 0;
	uint32_t halfwords_left = 0;

	sum = 1u;
	halfwords_left = 0x3Eu;
	do
	{
		sum ^= BOOKKEEPING_HALFWORDS(block)[0];
		carry = sum & 1u;
		sum = sum >> 1;
		block += 2u;
		if (carry)
			sum ^= 0x8810u;
		halfwords_left--;
	} while (halfwords_left != 0u);
	return sum;
}
