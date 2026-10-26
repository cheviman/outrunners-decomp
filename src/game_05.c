/**
 *    Name:        game_05.c
 *    Description: the game's functions, part 05 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* ADVERTISE SOUND: advertise_sound on or off (1 or 0); redraws. */
void test_mode_game_assignments_advertise(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = advertise_sound > 0u;
	advertise_sound = 1u - carry;
	ignored = test_mode_game_assignments_draw();
}

/* The game assignments' EXIT: compares game_difficulty, time_difficulty, steering_wheel_reaction and
   advertise_sound with their backup copy 3C bytes on (backup_settings_checksum on, the copy of the
   settings the boot code keeps); if any changed, marks the settings changed (test_mode_flags bit 1);
   back to the menu. */
void test_mode_game_assignments_exit(void)
{
	uint32_t setting_places = 0;

	setting_places = game_assignments_setting_places_ADDRESS;
	current_object = settings_magic_ADDRESS;
	do
	{
		current_object = (current_object & ~0xFFu) | TEST_MODE_SETTING_PLACES(setting_places)[0];
		setting_places++;
		if ((uint8_t)current_object == 0u)
		{
			test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (mem_read8(current_object + 0x3Cu) == mem_read8(current_object));
	test_mode_flags |= 2u;
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the COIN ASSIGNMENTS with the cursor on EXIT (test_mode_page_cursor 2):
   test_mode_coin_assignments_show. */
void test_mode_coin_assignments(void)
{
	test_mode_page_cursor = 2u;
	test_mode_coin_assignments_show();
}

/* Draws the coin assignments page (COIN/CREDIT SETTING with the setting,
   test_mode_coin_assignments_draw; MANUAL SETTING; EXIT; the prompt) with the cursor where
   test_mode_page_cursor says: on EXIT when the page starts, on MANUAL SETTING when that page
   (test_mode_coin_assignments_manual_start) returns here; then test_mode_coin_assignments_run. */
void test_mode_coin_assignments_show(void)
{
	uint32_t cell = 0, character = 0, colours = 0, text = 0;

	character = test_mode_clear_text();
	text = coin_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	test_mode_coin_assignments_draw(&character, &colours);
	test_mode_draw_instructions(character);
	cell = 0x310000u | coin_assignments_cursor_cells[test_mode_page_cursor];
	test_mode_set_cursor(character, cell);
	test_mode_routine = test_mode_coin_assignments_run_ADDRESS;
	test_mode_coin_assignments_run(colours);
}

/* The coin assignments, each frame: SERVICE moves the cursor through the three items; TEST on
   COIN/CREDIT SETTING takes the next setting (test_mode_coin_assignments_next_setting), on MANUAL
   SETTING opens that page (test_mode_coin_assignments_manual_start), on EXIT leaves
   (test_mode_coin_assignments_exit); the arrow blinks. Its table's last four entries belong to the
   manual setting page, whose table starts inside this one: they are never taken from here. */
void test_mode_coin_assignments_run(uint32_t item_count)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		item_count = (item_count & ~0xFFu) | 0xCu;
		item_count = (item_count & ~0xFFu) | ((uint8_t)item_count >> 2);
		if ((int8_t)item_count <= (int8_t)page_cursor)
			page_cursor = 0u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | coin_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		switch (test_mode_page_cursor)
		{
		case 0:
			test_mode_coin_assignments_next_setting();
			break;
		case 1:
			test_mode_coin_assignments_manual_start(item_count);
			break;
		case 2:
			test_mode_coin_assignments_exit();
			break;
		case 3:
			test_mode_coin_assignments_step_coin_to_credit(item_count);
			break;
		case 4:
			test_mode_coin_assignments_step_bonus_adder(item_count);
			break;
		case 5:
			test_mode_coin_assignments_step_multiplier(item_count);
			break;
		case 6:
			test_mode_coin_assignments_manual_exit();
			break;
		default:
			invalid_jump(test_mode_coin_assignments_item_jumps[test_mode_page_cursor]);
		}
	}
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the coin/credit setting: its number (coin_setting + 1) and what it gives, from its description
   (coin_setting_descriptions): up to five lines of "n COIN(S) m CREDIT(S)", or a text (FREE PLAY, MANUAL
   SETTING). */
void test_mode_coin_assignments_draw(uint32_t *p_character, uint32_t *p_colours)
{
	uint32_t zero = 0;
	uint16_t blank_cell = 0;
	uint32_t cell = 0, colours = 0, credits = 0, description = 0, description_byte = 0, digit_count = 0, line_text = 0, lines_left = 0, number = 0, plural_offset = 0;

	current_object = settings_magic_ADDRESS;
	cell = TEXT_CELL(32, 9);
	colours = 0x64u;
	digit_count = 2u;
	number = coin_setting + 1u;
	test_mode_draw_number(digit_count, &number, &colours, &cell);
	description = coin_setting_descriptions[coin_setting];
	blank_cell = 0u;
	r12 = TEXT_CELL(17, 10);
	lines_left = 5u;
	for (;;)
	{
		description_byte = mem_read8(description);
		zero = description_byte == 0u;
		description++;
		if ((int8_t)description_byte <= 0)
			break;
		colours = (colours & ~0xFFFFu) | 0x6400u;
		cell = r12;
		line_text = coin_assignments_line_text_ADDRESS;
		do
		{
			mem_write16(cell, colours);
			colours = (colours & ~0xFFu) | TEST_MODE_CHARACTERS(line_text)[0];
			cell += 2u;
			line_text++;
		} while ((uint8_t)colours != 0u);
		cell = r12;
		colours = (colours & ~0xFFFFu) | 0x6464u;
		digit_count = 1u;
		plural_offset = 6u;
		test_mode_draw_count(description_byte, digit_count, plural_offset, &colours, &cell);
		cell = r12 + 0x10u;
		credits = mem_read8(description);
		colours = (colours & ~0xFFFFu) | 0x6464u;
		digit_count = 1u;
		plural_offset = 8u;
		description++;
		test_mode_draw_count(credits, digit_count, plural_offset, &colours, &cell);
		lines_left--;
		r12 = r12 + 0x80u;
	}
	if (lines_left != 0u)
	{
		cell = r12;
		do
		{
			move_string(screen, 0u, cell, 0x11u, 2u, 1u, 0u, blank_cell, &r27, &r28);
			lines_left--;
			cell += 0x80u;
		} while (lines_left != 0u);
		if (zero == 0u)
		{
			description_byte = 0x6200u;
			for (;;)
			{
				description_byte = (description_byte & ~0xFFu) | mem_read8(description);
				description++;
				if ((uint8_t)description_byte == 0u)
					break;
				mem_write16(r12, description_byte);
				r12 = r12 + 2u;
			}
		}
	}
	*p_character = description_byte;
	*p_colours = colours;
}

/* COIN/CREDIT SETTING: the next of the 26 settings (coin_setting, after FREE PLAY back to */
void test_mode_coin_assignments_next_setting(void)
{
	uint32_t ignored = 0, ignored_2 = 0, setting = 0, setting_number = 0;

	do
	{
		setting_number = coin_setting;
		setting_number = (uint8_t)(setting_number + 1u);
		if ((int8_t)setting_number >= 0x1A)
			setting_number = 0u;
		coin_setting = setting_number;
		setting = coin_setting_records_ADDRESS + setting_number * 4u;
		if ((int8_t)COIN_SETTING_RECORD(setting)->coins < 0)
		{
			coins_per_credit = 1u;
			bonus_adder = 0xFFu;
			first_chute_multiplier = 1u;
			second_chute_multiplier = 1u;
			free_play = 0xFFu;
			test_mode_coin_assignments_draw(&ignored, &ignored_2);
			return;
		}
	} while (setting_number != 0u && coins_per_credit == COIN_SETTING_RECORD(setting)->coins && bonus_adder == COIN_SETTING_RECORD(setting)->bonus && first_chute_multiplier == COIN_SETTING_RECORD(setting)->first_multiplier);
	coins_per_credit = COIN_SETTING_RECORD(setting)->coins;
	setting++;
	bonus_adder = COIN_SETTING_RECORD(setting - 0x1u)->bonus;
	setting++;
	first_chute_multiplier = COIN_SETTING_RECORD(setting - 0x2u)->first_multiplier;
	second_chute_multiplier = COIN_SETTING_RECORD(setting - 0x2u)->second_multiplier;
	free_play = 0u;
	test_mode_coin_assignments_draw(&ignored, &ignored_2);
}

/* Opens the coin assignments' MANUAL SETTING page: clears the text, draws its texts (COIN TO CREDIT,
   BONUS ADDER, MULTIPLIER, the table of credits for 1 to 9 coins, EXIT) and the setting
   (test_mode_coin_assignments_manual_draw), the instructions, puts the cursor on EXIT
   (test_mode_page_cursor 3) and goes on in test_mode_coin_assignments_manual_run. */
void test_mode_coin_assignments_manual_start(uint32_t item_count)
{
	uint32_t cell = 0, character = 0, coins_for_credit = 0, text = 0;

	character = test_mode_clear_text();
	text = coin_manual_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	coins_for_credit = test_mode_coin_assignments_manual_draw(item_count);
	test_mode_draw_instructions(coins_for_credit);
	test_mode_page_cursor = 3u;
	cell = 0x310000u | coin_manual_exit_cell;
	test_mode_set_cursor(coins_for_credit, cell);
	test_mode_routine = test_mode_coin_assignments_manual_run_ADDRESS;
}

/* Each frame of the MANUAL SETTING page: SERVICE moves the cursor down its four lines
   (test_mode_page_cursor, wrapping), TEST changes the line's value: COIN TO CREDIT
   (test_mode_coin_assignments_step_coin_to_credit), BONUS ADDER
   (test_mode_coin_assignments_step_bonus_adder), MULTIPLIER (test_mode_coin_assignments_step_multiplier)
   or EXIT (test_mode_coin_assignments_manual_exit); then blinks the cursor. */
void test_mode_coin_assignments_manual_run(void)
{
	uint32_t cell = 0, item_count = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		item_count = 0x10u;
		item_count = item_count >> 2;
		if ((int8_t)item_count <= (int8_t)page_cursor)
			page_cursor = 0u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | coin_manual_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		switch (test_mode_page_cursor)
		{
		case 0:
			test_mode_coin_assignments_step_coin_to_credit(item_count);
			break;
		case 1:
			test_mode_coin_assignments_step_bonus_adder(item_count);
			break;
		case 2:
			test_mode_coin_assignments_step_multiplier(item_count);
			break;
		case 3:
			test_mode_coin_assignments_manual_exit();
			break;
		default:
			invalid_jump(test_mode_coin_assignments_manual_item_jumps[test_mode_page_cursor]);
		}
	}
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the manual coin setting: coins_per_credit (N COIN(S) 1 CREDIT), the bonus_adder (N COINS GIVE 1
   EXTRA COIN, NO BONUS ADDER, or FREE PLAY when negative), the first_chute_multiplier (1 COIN COUNTS AS
   N COIN(S)), and the table of what 1 to 9 coins give: each coin counts as the multiplier, the bonus
   adds a coin for every bonus_adder counted (the rest carried on), and the credits (at most 9) show as
   two digits (test_mode_draw_two_digits) with what is left over as a fraction of a credit in the font's
   fraction characters. */
uint32_t test_mode_coin_assignments_manual_draw(uint32_t item_count)
{
	uint32_t bonus_rest = 0, cell = 0, coins_for_bonus = 0, coins_for_credit = 0, colours = 0, counted_coins = 0, credits = 0, digit_count = 0, leftover_coins = 0, multiplier = 0, numerator = 0, plural_offset = 0, rows_left = 0, shown_bonus = 0, shown_multiplier = 0, text = 0, toward_bonus = 0;
	uint32_t saved_leftover_coins = 0;

	current_object = settings_magic_ADDRESS;
	cell = TEXT_CELL(23, 8);
	coins_for_credit = coins_per_credit;
	colours = (item_count & ~0xFFFFu) | 0x6464u;
	digit_count = 1u;
	plural_offset = 6u;
	test_mode_draw_count(coins_for_credit, digit_count, plural_offset, &colours, &cell);
	shown_bonus = bonus_adder;
	if ((int8_t)shown_bonus > 0)
	{
		cell = (cell & ~0xFFFFu) | 0xF51Eu;
		colours = 0x64u;
		digit_count = 1u;
		test_mode_draw_number(digit_count, &shown_bonus, &colours, &cell);
		text = bonus_adder_text_ADDRESS;
		test_mode_draw_texts(shown_bonus, text);
	}
	else if ((int8_t)shown_bonus < 0)
	{
		text = bonus_free_play_text_ADDRESS;
		test_mode_draw_texts(shown_bonus, text);
	}
	else
	{
		text = no_bonus_adder_text_ADDRESS;
		test_mode_draw_texts(shown_bonus, text);
	}
	cell = TEXT_CELL(32, 12);
	shown_multiplier = first_chute_multiplier;
	colours = (colours & ~0xFFFFu) | 0x6464u;
	digit_count = 1u;
	plural_offset = 6u;
	test_mode_draw_count(shown_multiplier, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF784u;
	coins_for_credit = coins_per_credit;
	coins_for_bonus = (int8_t)bonus_adder;
	multiplier = first_chute_multiplier;
	counted_coins = 0u;
	toward_bonus = 0u;
	rows_left = 9u;
	do
	{
		counted_coins += multiplier;
		if ((int32_t)coins_for_bonus > 0)
		{
			toward_bonus += multiplier;
			bonus_rest = 0u;
			divux_pair(coins_for_bonus, &toward_bonus, &bonus_rest);
			counted_coins += toward_bonus;
			toward_bonus = bonus_rest;
		}
		credits = counted_coins;
		leftover_coins = 0u;
		divux_pair(coins_for_credit, &credits, &leftover_coins);
		if ((uint8_t)credits >= 9u)
		{
			credits = (credits & ~0xFFu) | 9u;
			leftover_coins = 0u;
		}
		saved_leftover_coins = leftover_coins;
		cell = test_mode_draw_two_digits(credits, cell);
		numerator = saved_leftover_coins;
		if (numerator != 0u)
		{
			mem_write16(cell, numerator + 0x6A7Fu);
			cell += 2u;
			mem_write16(cell, coins_for_credit + 0x6A86u);
			cell += 2u;
		}
		else
		{
			mem_write32(cell, 0x200020u);
			cell += 4u;
		}
		rows_left--;
	} while (rows_left != 0u);
	return coins_for_credit;
}

/* Draws R5 (below 100) at text cell R10 as two digits in white (6A), a blank for a 0 tens digit, and
   returns the cell after them. Used by test_mode_coin_assignments_manual_draw. */
uint32_t test_mode_draw_two_digits(uint32_t number, uint32_t cell)
{
	uint32_t zero = 0;
	uint32_t ones = 0;

	ones = 0u;
	divux_pair(0xAu, &number, &ones);
	zero = (uint32_t)(number == 0u);
	if (zero == 0u)
	{
		mem_write16(cell, number + 0x6A30u);
		cell += 2u;
	}
	else
	{
		mem_write16(cell, 0x20u);
		cell += 2u;
	}
	mem_write16(cell, ones + 0x6A30u);
	cell += 2u;
	return cell;
}

/* COIN TO CREDIT: test_mode_coin_assignments_step_value with coins_per_credit. */
void test_mode_coin_assignments_step_coin_to_credit(uint32_t item_count)
{
	uint32_t setting_offset = 0;

	setting_offset = 0x10u;
	test_mode_coin_assignments_step_value(setting_offset, item_count);
}

/* Steps the coin setting R0 bytes into the settings (coins_per_credit or first_chute_multiplier) from 1
   to 9 and round; unless free play is on, the coin setting becomes the manual one (coin_setting 1A);
   redraws (test_mode_coin_assignments_manual_draw). */
void test_mode_coin_assignments_step_value(uint32_t setting_offset, uint32_t item_count)
{
	uint32_t ignored = 0, setting = 0;

	setting = (item_count & ~0xFFu) | mem_read8(current_object + setting_offset);
	setting = (setting & ~0xFFu) | (uint8_t)(setting + 1u);
	if ((uint8_t)setting >= 0xAu)
		setting = (setting & ~0xFFu) | 1u;
	mem_write8(current_object + setting_offset, setting);
	if (free_play == 0u)
		coin_setting = 0x1Au;
	ignored = test_mode_coin_assignments_manual_draw(setting);
}

/* BONUS ADDER: steps bonus_adder through none (0), 2 to 9 and back to none; from free play (FF) it goes
   to none and turns free_play off; the coin setting becomes the manual one (coin_setting 1A); redraws. */
void test_mode_coin_assignments_step_bonus_adder(uint32_t item_count)
{
	uint8_t bonus = 0;
	uint32_t ignored = 0;
	uint8_t next_bonus = 0;

	bonus = bonus_adder;
	if (bonus == 0u)
		bonus++;
	next_bonus = bonus + 1u;
	bonus = next_bonus;
	if (next_bonus == 0u)
		free_play = 0u;
	if (bonus >= 0xAu)
		bonus = 0u;
	bonus_adder = bonus;
	coin_setting = 0x1Au;
	ignored = test_mode_coin_assignments_manual_draw(item_count);
}

/* MULTIPLIER: test_mode_coin_assignments_step_value with first_chute_multiplier (1 COIN COUNTS AS N
   COIN). */
void test_mode_coin_assignments_step_multiplier(uint32_t item_count)
{
	uint32_t setting_offset = 0;

	setting_offset = 0x11u;
	test_mode_coin_assignments_step_value(setting_offset, item_count);
}

/* EXIT of the MANUAL SETTING page: back to the coin assignments page, its cursor on MANUAL SETTING
   (test_mode_page_cursor 1). */
void test_mode_coin_assignments_manual_exit(void)
{
	test_mode_page_cursor = 1u;
	test_mode_routine = test_mode_coin_assignments_show_ADDRESS;
}

/* EXIT of the coin assignments page: when coins_per_credit, first_chute_multiplier, bonus_adder,
   free_play or coin_setting differ from the copy last written to the EEPROM (3C bytes on), marks the
   settings changed (test_mode_flags bit 1, written at the test mode's EXIT) and clears the credits kept
   in the shared RAM (bookkeeping_clear_credits); back to the menu. */
void test_mode_coin_assignments_exit(void)
{
	uint32_t setting_places = 0;

	setting_places = coin_assignments_setting_places_ADDRESS;
	do
	{
		current_object = (current_object & ~0xFFu) | TEST_MODE_SETTING_PLACES(setting_places)[0];
		setting_places++;
		if ((uint8_t)current_object == 0u)
		{
			test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (mem_read8(current_object + 0x3Cu) == mem_read8(current_object));
	test_mode_flags |= 2u;
	bookkeeping_clear_credits();
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* Opens the NETWORK ASSIGNMENTS page: its texts, with PRIVILEGE MODE and CABINET ID hidden (colour 6E)
   when the cabinet is stand_alone; test_mode_privilege_mode from link_master; the values
   (test_mode_network_assignments_draw), the instructions, the cursor on EXIT; goes on in
   test_mode_network_assignments_run. */
void test_mode_network_assignments_start(void)
{
	uint32_t cell = 0, character = 0, hidden_colour = 0, text = 0;

	character = test_mode_clear_text();
	text = network_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	current_object = settings_magic_ADDRESS;
	if (stand_alone != 0u)
	{
		cell = TEXT_CELL(8, 12);
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, cell);
		cell = TEXT_CELL(8, 14);
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, cell);
	}
	test_mode_privilege_mode = link_master & 1u;
	character = test_mode_network_assignments_draw();
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 3u;
	cell = 0x310000u | network_assignments_exit_cell;
	test_mode_set_cursor(character, cell);
	test_mode_routine = test_mode_network_assignments_run_ADDRESS;
	test_mode_network_assignments_run();
}

/* Each frame of the NETWORK ASSIGNMENTS page: SERVICE moves the cursor (between COMMUNICATION and EXIT
   only when stand_alone, else down all four lines), TEST runs its line: COMMUNICATION
   (test_mode_network_assignments_toggle_communication), PRIVILEGE MODE
   (test_mode_network_assignments_toggle_privilege_mode), CABINET ID
   (test_mode_network_assignments_step_cabinet_id) or EXIT (test_mode_network_assignments_exit); then
   blinks the cursor. */
void test_mode_network_assignments_run(void)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor;
		if (stand_alone == 0u)
		{
			page_cursor = (uint8_t)(page_cursor + 1u);
			if ((int8_t)page_cursor >= 4)
				page_cursor = 0u;
		}
		else
			page_cursor = page_cursor ^ 3u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | network_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		page_cursor = test_mode_page_cursor;
		switch (page_cursor)
		{
		case 0:
			test_mode_network_assignments_toggle_communication(page_cursor);
			break;
		case 1:
			test_mode_network_assignments_toggle_privilege_mode();
			break;
		case 2:
			test_mode_network_assignments_step_cabinet_id();
			break;
		case 3:
			test_mode_network_assignments_exit();
			break;
		default:
			invalid_jump(test_mode_network_assignments_item_jumps[page_cursor]);
		}
	}
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the network settings: COMMUNICATION NETWORK or STAND-ALONE (stand_alone), PRIVILEGE MODE SLAVE
   or MASTER (test_mode_privilege_mode), CABINET ID 1-4 (cabinet_id + 1); the last two hidden (colour 6E)
   when stand-alone. */
uint32_t test_mode_network_assignments_draw(void)
{
	uint32_t cabinet_number_then_colour = 0, cell = 0, colour = 0, digit_count = 0, privilege = 0, stand_alone_bit = 0, text = 0;

	current_object = settings_magic_ADDRESS;
	stand_alone_bit = stand_alone;
	stand_alone_bit = stand_alone_bit & 1u;
	text = communication_texts[stand_alone_bit];
	test_mode_draw_texts(stand_alone_bit, text);
	privilege = test_mode_privilege_mode;
	text = privilege_mode_texts[privilege];
	test_mode_draw_texts(privilege, text);
	cell = TEXT_CELL(33, 14);
	colour = 0x64u;
	digit_count = 1u;
	cabinet_number_then_colour = cabinet_id + 1u;
	test_mode_draw_number(digit_count, &cabinet_number_then_colour, &colour, &cell);
	if (stand_alone != 0u)
	{
		cell = TEXT_CELL(28, 12);
		cabinet_number_then_colour = (cabinet_number_then_colour & ~0xFFu) | 0x6Eu;
		cabinet_number_then_colour = test_mode_colour_line(cabinet_number_then_colour, cell);
		cell = TEXT_CELL(33, 14);
		cabinet_number_then_colour = (cabinet_number_then_colour & ~0xFFu) | 0x6Eu;
		cabinet_number_then_colour = test_mode_colour_line(cabinet_number_then_colour, cell);
	}
	return cabinet_number_then_colour;
}

/* COMMUNICATION: switches stand_alone (NETWORK, STAND-ALONE), puts the privilege mode back to MASTER
   (link_master 1, isdn_mode 0, test_mode_privilege_mode 1), shows or hides the PRIVILEGE MODE and
   CABINET ID labels and redraws the values. */
void test_mode_network_assignments_toggle_communication(uint32_t page_cursor)
{
	uint32_t carry = 0;
	uint32_t cell = 0, ignored = 0, stand_alone_then_colour = 0;
	uint32_t saved_colour = 0;
	uint8_t stand_alone_setting = 0;

	carry = stand_alone > 0u;
	stand_alone_then_colour = (page_cursor & ~0xFFu) | 1u;
	stand_alone_setting = stand_alone_then_colour;
	stand_alone_then_colour = (stand_alone_then_colour & ~0xFFu) | (uint8_t)(stand_alone_setting - carry);
	stand_alone = stand_alone_then_colour;
	link_master = 1u;
	isdn_mode = 0u;
	test_mode_privilege_mode = 1u;
	stand_alone_then_colour = (stand_alone_then_colour & ~0xFFu) | 0x60u;
	if (stand_alone_setting != carry)
		stand_alone_then_colour = (stand_alone_then_colour & ~0xFFu) | 0x6Eu;
	cell = TEXT_CELL(8, 12);
	saved_colour = stand_alone_then_colour;
	test_mode_colour_line(stand_alone_then_colour, cell);
	cell = TEXT_CELL(8, 14);
	stand_alone_then_colour = saved_colour;
	test_mode_colour_line(stand_alone_then_colour, cell);
	ignored = test_mode_network_assignments_draw();
}

/* PRIVILEGE MODE: switches test_mode_privilege_mode between SLAVE and MASTER and sets link_master and
   isdn_mode from it (01E6E5: SLAVE 0 and 0, MASTER 1 and 0; the table's third pair, ISDN, 0 and 1,
   cannot be reached); redraws. */
void test_mode_network_assignments_toggle_privilege_mode(void)
{
	uint32_t ignored = 0, privilege = 0, setting = 0;
	uint8_t stepped_privilege = 0;

	privilege = test_mode_privilege_mode;
	stepped_privilege = privilege - 1u;
	privilege = stepped_privilege;
	if ((int8_t)stepped_privilege < 0)
		privilege = 1u;
	test_mode_privilege_mode = privilege;
	setting = privilege_mode_settings_ADDRESS + privilege * 2u;
	link_master = PRIVILEGE_MODE_SETTING(setting)->master;
	isdn_mode = PRIVILEGE_MODE_SETTING(setting)->isdn;
	ignored = test_mode_network_assignments_draw();
}

/* CABINET ID: steps cabinet_id through 0-3 (shown 1-4); redraws. */
void test_mode_network_assignments_step_cabinet_id(void)
{
	uint32_t ignored = 0;

	cabinet_id = (cabinet_id + 1u) & 3u;
	ignored = test_mode_network_assignments_draw();
}

/* EXIT of the NETWORK ASSIGNMENTS page: when stand_alone, link_master, isdn_mode or cabinet_id differ
   from the copy last written to the EEPROM, marks the settings changed (test_mode_flags bit 1); back to
   the menu. */
void test_mode_network_assignments_exit(void)
{
	uint32_t setting_places = 0;

	setting_places = network_assignments_setting_places_ADDRESS;
	do
	{
		current_object = (current_object & ~0xFFu) | TEST_MODE_SETTING_PLACES(setting_places)[0];
		setting_places++;
		if ((uint8_t)current_object == 0u)
		{
			test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (mem_read8(current_object + 0x3Cu) == mem_read8(current_object));
	test_mode_flags |= 2u;
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* Opens the first BOOKKEEPING page (1/2): its texts, its numbers (test_mode_bookkeeping_draw_totals) and
   the blinking PUSH TEST BUTTON TO CONTINUE (test_mode_show_continue_prompt); goes on in
   test_mode_bookkeeping_wait. */
void test_mode_bookkeeping_start(void)
{
	uint32_t character = 0, text = 0;

	character = test_mode_clear_text();
	text = bookkeeping_first_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_bookkeeping_draw_totals();
	test_mode_show_continue_prompt();
	test_mode_routine = test_mode_bookkeeping_wait_ADDRESS;
	test_mode_bookkeeping_wait((uint16_t)character);
}

/* Blinks the prompt (test_mode_blink_prompt) until TEST, then the second page
   (test_mode_bookkeeping_second_page_start). */
void test_mode_bookkeeping_wait(uint16_t character)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_bookkeeping_second_page_start_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Opens the second BOOKKEEPING page (2/2): its texts, the time histogram and the course records
   (test_mode_bookkeeping_draw_histogram) and the blinking PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt); goes on in test_mode_bookkeeping_second_page_wait. */
void test_mode_bookkeeping_second_page_start(void)
{
	uint32_t character = 0, text = 0;

	character = test_mode_clear_text();
	text = bookkeeping_second_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_bookkeeping_draw_histogram();
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_bookkeeping_second_page_wait_ADDRESS;
	test_mode_bookkeeping_second_page_wait((uint16_t)character);
}

/* Blinks the prompt until TEST, then back to the menu. */
void test_mode_bookkeeping_second_page_wait(uint16_t character)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Draws the first bookkeeping page's numbers from the bookkeeping in the sound board's shared RAM
   (701F00 on): COIN CHUTE A and B (the coins of each seat's first chute, 701F06 and 701F0A) and TOTAL
   COIN(S); COIN CREDIT(S) (701F02) and SERVICE CREDIT(S) (701F04) and their TOTAL; NUMBER OF GAME(S) A
   and B (701F14, 701F16); TOTAL TIME (701F10, counted by count_total_time) and GAME PLAY TIME A and B
   (701F18, 701F1C) in days, hours, minutes and seconds (test_mode_draw_duration); once a game was
   played, AVERAGE GAME TIME (the play times over the games), LONGEST (701F20) and SHORTEST (701F22) in
   minutes and seconds (test_mode_draw_minutes_seconds). */
uint32_t test_mode_bookkeeping_draw_totals(void)
{
	uint32_t average_time = 0, cell = 0, coin_credits = 0, coin_sum = 0, coins_a = 0, coins_b = 0, colours = 0, credit_sum = 0, digit_count = 0, games_a = 0, games_b = 0, longest_time = 0, play_time_a = 0, play_time_b = 0, plural_offset = 0, service_credits = 0, shortest_time = 0, time_cell = 0, total_coins = 0, total_credits = 0, total_games = 0, total_time = 0;
	uint32_t game_count = 0;

	current_object = SOUND_701F00;
	cell = TEXT_CELL(26, 5);
	coins_a = mem_read16(SOUND_701F06);
	coin_sum = coins_a;
	colours = 0x64u;
	digit_count = 5u;
	test_mode_draw_number(digit_count, &coins_a, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF334u;
	coins_b = mem_read16(current_object + 0xAu);
	coin_sum += coins_b;
	colours = 0x64u;
	digit_count = 5u;
	test_mode_draw_number(digit_count, &coins_b, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF3B2u;
	total_coins = coin_sum;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 6u;
	plural_offset = 0xFFFFFFFEu;
	test_mode_draw_count(total_coins, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF4B4u;
	coin_credits = mem_read16(current_object + 2u);
	credit_sum = coin_credits;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(coin_credits, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF534u;
	service_credits = mem_read16(current_object + 4u);
	credit_sum += service_credits;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(service_credits, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF5B2u;
	total_credits = credit_sum;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 6u;
	plural_offset = 0xFFFFFFFEu;
	test_mode_draw_count(total_credits, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF6B4u;
	games_a = mem_read16(current_object + 0x14u);
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(games_a, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF734u;
	games_b = mem_read16(current_object + 0x16u);
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(games_b, digit_count, plural_offset, &colours, &cell);
	time_cell = TEXT_CELL(20, 16);
	total_time = mem_read32(current_object + 0x10u);
	time_cell = test_mode_draw_duration(total_time, time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xF8A8u;
	play_time_a = mem_read32(current_object + 0x18u);
	time_cell = test_mode_draw_duration(play_time_a, time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xF928u;
	play_time_b = mem_read32(current_object + 0x1Cu);
	time_cell = test_mode_draw_duration(play_time_b, time_cell);
	games_a = mem_read16(current_object + 0x14u);
	game_count = mem_read16(current_object + 0x16u) + games_a;
	total_games = game_count;
	if (game_count == 0u)
		return games_a;
	time_cell = (time_cell & ~0xFFFFu) | 0xFA34u;
	average_time = divide_u32(mem_read32(current_object + 0x18u) + mem_read32(current_object + 0x1Cu), total_games);
	test_mode_draw_minutes_seconds(&average_time, &time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xFAB4u;
	longest_time = mem_read16(current_object + 0x20u);
	test_mode_draw_minutes_seconds(&longest_time, &time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xFB34u;
	shortest_time = mem_read16(current_object + 0x22u);
	test_mode_draw_minutes_seconds(&shortest_time, &time_cell);
	return shortest_time;
}

/* Draws the second bookkeeping page's numbers: the TIME HISTOGRAM, how far the games got (seven counts
   from 701F24: PRE-STAGE, STAGE 1-5, COMPLETE), each with a bar of up to 20 blocks scaled to the
   largest; and OutRunner of OutRunners, the record of each of the ten goals (AUSTRALIA to KENYA; six
   bytes each from 701F32: the car, none when negative, shown by its colour, the three letters of the
   name and the time in hundredths, shown as minutes, seconds and hundredths). */
uint32_t test_mode_bookkeeping_draw_histogram(void)
{
	uint32_t carry = 0;
	uint16_t character = 0, stage_games = 0, zero_character = 0;
	uint32_t bar_blocks = 0, bar_cell = 0, bar_scale = 0, car_then_time = 0, colour = 0, colour_name = 0, digit_count = 0, digit_step = 0, divisor = 0, drawn_counts = 0, games = 0, largest_count = 0, number_cell = 0, record = 0, records_left = 0, rest_time = 0, row_cell = 0, stage_counts = 0, stages_left = 0;
	uint32_t rounded_blocks = 0;

	current_object = SOUND_701F00;
	stage_counts = SOUND_RAM + 0x1F24u;
	largest_count = 0u;
	stages_left = 7u;
	do
	{
		stage_games = mem_read16(stage_counts);
		stage_counts += 2u;
		if (largest_count < stage_games)
			largest_count = stage_games;
		stages_left--;
	} while (stages_left != 0u);
	bar_scale = 0u;
	if (largest_count != 0u)
		bar_scale = divide_u32(0x50000000u, largest_count);
	drawn_counts = current_object + 0x24u;
	r12 = TEXT_CELL(13, 5);
	stages_left = 7u;
	do
	{
		games = mem_read16(drawn_counts);
		bar_blocks = games;
		number_cell = r12;
		colour = 0x64u;
		digit_count = 5u;
		drawn_counts += 2u;
		test_mode_draw_number(digit_count, &games, &colour, &number_cell);
		bar_blocks = bar_blocks * bar_scale;
		carry = (bar_blocks >> 25) & 1u;
		rounded_blocks = (bar_blocks >> 26) + carry;
		bar_blocks = rounded_blocks;
		if (rounded_blocks != 0u)
		{
			bar_cell = r12 + 0xAu;
			do
			{
				mem_write16(bar_cell, 0x6403u);
				bar_cell += 2u;
				bar_blocks--;
			} while (bar_blocks != 0u);
		}
		stages_left--;
		r12 = r12 + 0x80u;
	} while (stages_left != 0u);
	row_cell = TEXT_CELL(19, 14);
	record = current_object + 0x32u;
	records_left = 0xAu;
	do
	{
		car_then_time = mem_read8(record);
		record++;
		if ((int8_t)car_then_time < 0)
			record += 5u;
		else
		{
			r12 = row_cell;
			colour_name = car_colour_names_ADDRESS + car_then_time * 3u * 2u;
			character = (uint16_t)(0x6400u | TEST_MODE_CHARACTERS(colour_name)[0]);
			mem_write16(r12, character);
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			r12 = r12 + 2u;
			mem_write16(r12, character);
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			r12 = r12 + 2u;
			mem_write16(r12, character);
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			r12 = r12 + 2u;
			mem_write16(r12, character);
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			r12 = r12 + 2u;
			mem_write16(r12, character);
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[1]);
			r12 = r12 + 2u;
			mem_write16(r12, character);
			character = (uint16_t)((character & ~0xFFu) | mem_read8(record));
			r12 = r12 + 4u;
			mem_write16(r12, character);
			record++;
			character = (uint16_t)((character & ~0xFFu) | mem_read8(record));
			r12 = r12 + 2u;
			mem_write16(r12, character);
			record++;
			r12 = r12 + 2u;
			mem_write16(r12, (character & ~0xFFu) | mem_read8(record));
			record++;
			car_then_time = mem_read16(record);
			zero_character = 0x6430u;
			digit_step = time_digit_steps_ADDRESS;
			record += 2u;
			r12 = r12 + 4u;
			do
			{
				rest_time = 0u;
				divisor = TIME_DIGIT_STEP(digit_step)->divisor;
				digit_step += 4u;
				divux_pair(divisor, &car_then_time, &rest_time);
				mem_write16(r12, car_then_time + zero_character);
				r12 = r12 + TIME_DIGIT_STEP(digit_step - 0x4u)->cell_step;
				car_then_time = rest_time;
				digit_step += 4u;
			} while ((int32_t)TIME_DIGIT_STEP(digit_step)->divisor >= 0);
		}
		records_left--;
		row_cell += 0x80u;
	} while (records_left != 0u);
	return car_then_time;
}

/* Draws a time of R0 seconds at text cell R11 as days (5 digits), hours, minutes and seconds (2 digits
   each, a cell apart, for the D H M S of the text) and returns the cell after it. */
uint32_t test_mode_draw_duration(uint32_t seconds, uint32_t cell)
{
	uint32_t colour = 0, digit_count = 0, number_cell = 0, remainder = 0, seconds_of_day = 0, seconds_of_hour = 0;
	uint32_t saved_remainder = 0;

	remainder = 0u;
	divux_pair(SECONDS_PER_DAY, &seconds, &remainder);
	saved_remainder = remainder;
	colour = 0x64u;
	digit_count = 5u;
	number_cell = cell;
	cell += 0xCu;
	test_mode_draw_number(digit_count, &seconds, &colour, &number_cell);
	seconds_of_day = saved_remainder;
	remainder = 0u;
	divux_pair(SECONDS_PER_HOUR, &seconds_of_day, &remainder);
	saved_remainder = remainder;
	colour = 0x64u;
	digit_count = 2u;
	number_cell = cell;
	cell += 6u;
	test_mode_draw_number(digit_count, &seconds_of_day, &colour, &number_cell);
	seconds_of_hour = saved_remainder;
	test_mode_draw_minutes_seconds(&seconds_of_hour, &cell);
	return cell;
}

/* Draws a time of R0 seconds at text cell R11 as minutes and seconds, 2 digits each a cell apart; R11
   moves to the seconds. */
void test_mode_draw_minutes_seconds(uint32_t *p_seconds, uint32_t *p_cell)
{
	uint32_t cell = *p_cell;
	uint32_t colour = 0, digit_count = 0, number_cell = 0, remainder = 0;
	uint32_t saved_remainder = 0;

	remainder = 0u;
	divux_pair(SECONDS_PER_MINUTE, p_seconds, &remainder);
	saved_remainder = remainder;
	colour = 0x64u;
	digit_count = 2u;
	number_cell = cell;
	cell += 6u;
	test_mode_draw_number(digit_count, p_seconds, &colour, &number_cell);
	*p_seconds = saved_remainder;
	colour = 0x64u;
	digit_count = 2u;
	number_cell = cell;
	test_mode_draw_number(digit_count, p_seconds, &colour, &number_cell);
	*p_cell = cell;
}

/* Unused: turns a duration kept as five bytes at R10 (seconds, minutes and hours a byte each, then days
   a halfword) into seconds: ((days * 24 + hours) * 60 + minutes) * 60 + seconds, in R0 (R1 the carry). */
uint32_t test_mode_duration_to_seconds(uint32_t duration)
{
	uint32_t seconds = 0;

	duration = duration + 5u - 2u;
	seconds = mem_read16(duration) * 0x18u;
	duration--;
	seconds = (seconds + mem_read8(duration)) * 0x3Cu;
	duration--;
	seconds = (seconds + mem_read8(duration)) * 0x3Cu + mem_read8(duration - 1u);
	return seconds;
}

/* Unused: writes a time of R0 seconds at R10 as two bytes: the seconds (R0 mod 60), then the minutes (R0
   / 60). */
void test_mode_split_minutes_seconds(uint32_t seconds, uint32_t stored_time)
{
	uint32_t remainder = 0;

	remainder = 0u;
	divux_pair(SECONDS_PER_MINUTE, &seconds, &remainder);
	mem_write8(stored_time, remainder);
	mem_write8(stored_time + 1u, seconds);
}

/* Opens the BACKUP DATA CLEAR page: YES (CLEAR) and NO (CANCEL), the instructions, the cursor on NO;
   goes on in test_mode_backup_data_clear_run. */
void test_mode_backup_data_clear_start(void)
{
	uint32_t cell = 0, character = 0, text = 0;

	character = test_mode_clear_text();
	text = backup_data_clear_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 1u;
	cell = 0x310000u | backup_data_clear_no_cell;
	test_mode_set_cursor(character, cell);
	test_mode_routine = test_mode_backup_data_clear_run_ADDRESS;
	test_mode_backup_data_clear_run();
}

/* Each frame of the BACKUP DATA CLEAR page: SERVICE moves the cursor between YES and NO; TEST on NO goes
   back to the menu; TEST on YES leaves the cursor on, clears the bookkeeping (bookkeeping_reset: all of
   it back to its defaults at 011458, zeros but SHORTEST GAME TIME FFFF and no course records, the
   credits too), shows COMPLETED, hides SELECT BY SERVICE BUTTON and blinks PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt) until TEST (test_mode_backup_data_clear_wait). */
void test_mode_backup_data_clear_run(void)
{
	uint32_t cell = 0, character = 0, hidden_colour = 0, instructions_cell = 0, text = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		character = test_mode_page_cursor ^ 1u;
		test_mode_page_cursor = character;
		cell = 0x310000u | backup_data_clear_cursor_cells[character];
		test_mode_set_cursor(character, cell);
	}
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		blink_timer = test_mode_blink_timer - 1u;
		test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			test_mode_blink_timer = 5;
			blink_cell = test_mode_blink_address;
			cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
			mem_write16(blink_cell, cursor_character);
			if (cursor_character != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	if (test_mode_page_cursor == 0u)
	{
		mem_write16(test_mode_blink_address, 0x6A7Fu);
		character = bookkeeping_reset(character);
		text = backup_data_clear_completed_text_ADDRESS;
		test_mode_draw_texts(character, text);
		instructions_cell = 0x310000u | test_mode_instructions_text;
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, instructions_cell);
		test_mode_show_exit_prompt();
		test_mode_routine = test_mode_backup_data_clear_wait_ADDRESS;
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* After the clear: blinks the prompt until TEST, then back to the menu. */
void test_mode_backup_data_clear_wait(void)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(0u);
}

/* EXIT: when a page changed a setting (test_mode_flags bit 1, cleared here) writes the settings to the
   EEPROM (save_settings, which counts eeprom_write_count up to 10000); while eeprom_write_count is below
   10000, or with nothing to write, leaves the test mode: test_mode_on 0 and the I/O chips' CNT registers
   4 and 80 (the test mode's start wrote 6 and 86: bit 1, cleared now, turns a chip's screen output on,
   runtime/src/io.c); at 10000 shows the CAUTION that the EEPROM has been written 10,000 times and waits
   for TEST first (test_mode_exit_caution_wait). */
void test_mode_exit(void)
{
	uint32_t character = 0, text = 0;
	uint32_t flags_before = 0, test_mode_flags_long = 0;

	flags_before = (test_mode_flags | ((uint32_t)test_mode_cursor << 8u) | ((uint32_t)test_mode_page_cursor << 16u) | ((uint32_t)(test_mode_routine & 0xFFu) << 24u));
	test_mode_flags_long = flags_before & ~2u;
	test_mode_flags = (uint8_t)test_mode_flags_long;
	test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	test_mode_routine = (test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	if ((flags_before & 2u) != 0u)
	{
		save_settings();
		if (eeprom_write_count < EEPROM_WRITE_LIMIT)
		{
			test_mode_on = 0u;
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			return;
		}
		character = test_mode_clear_text();
		text = eeprom_caution_text_ADDRESS;
		test_mode_draw_texts(character, text);
		test_mode_show_exit_prompt();
		test_mode_routine = test_mode_exit_caution_wait_ADDRESS;
		return;
	}
	test_mode_on = 0u;
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
}

/* Blinks PUSH TEST BUTTON TO EXIT under the CAUTION until TEST, then leaves the test mode as
   test_mode_exit does. */
void test_mode_exit_caution_wait(void)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		test_mode_on = 0u;
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		return;
	}
	test_mode_blink_prompt(0u);
}

/* Sets up the test mode's colours and tiles on both screens: the whole palette in the background's
   colour (733), the text palettes (01F304 for screen 1, 01F506 for screen 2: the text colours 60 grey,
   62 red, 64 green, 66 yellow, 68 cyan, 6A white, 6E the background's), 32 palettes after them whose
   colours 1-4 are red, green, blue and white at levels 0-31, and four solid tiles of colours 1-4
   (318020): what the C.R.T. test's colour bars are drawn with. */
uint32_t test_mode_set_up_colours(void)
{
	uint16_t background_colour = 0, level = 0, level_colour = 0, white = 0;
	uint32_t halfword_count = 0, levels_left = 0, rows_left = 0, screen_1_colour = 0, screen_2_colour = 0, tile_place = 0, tile_row = 0, tile_step = 0, tiles_left = 0;

	background_colour = 0x733u;
	set_halfwords(PALETTE_RAM + 0x8000u, 0x4000u, background_colour);
	set_halfwords(0x688000u, 0x4000u, background_colour);
	halfword_count = test_mode_palettes_1_bytes;
	halfword_count = halfword_count >> 1;
	move_halfwords(PALETTE_RAM + 0x9E00u, test_mode_palettes_1_ADDRESS, halfword_count);
	halfword_count = test_mode_palettes_2_bytes;
	halfword_count = halfword_count >> 1;
	move_string(test_mode_palettes_2_ADDRESS, halfword_count, 0x689E00u, halfword_count, 2u, 0u, 0u, 0u, &r27, &r28);
	r27 = r27 & ~0x8000u;
	level = 0u;
	levels_left = 0x20u;
	r27 = r27 + 2u;
	do
	{
		screen_2_colour = r27;
		screen_1_colour = screen_2_colour & ~0x80000u;
		level_colour = (uint16_t)level;
		mem_write16(screen_1_colour, level_colour);
		mem_write16(screen_2_colour, level_colour);
		white = (uint16_t)level_colour;
		level_colour = (uint16_t)((uint32_t)level_colour << 5);
		screen_1_colour += 2u;
		mem_write16(screen_1_colour, level_colour);
		screen_2_colour += 2u;
		mem_write16(screen_2_colour, level_colour);
		white += level_colour;
		level_colour = (uint16_t)((uint32_t)level_colour << 5);
		screen_1_colour += 2u;
		mem_write16(screen_1_colour, level_colour);
		screen_2_colour += 2u;
		mem_write16(screen_2_colour, level_colour);
		white += level_colour;
		mem_write16(screen_1_colour + 2u, white);
		mem_write16(screen_2_colour + 2u, white);
		level++;
		levels_left--;
		r27 = r27 + 0x20u;
	} while (levels_left != 0u);
	tile_place = VIDEO_RAM + 0x18020u;
	tile_row = 0u;
	tile_step = 0x11111111u;
	tiles_left = 4u;
	do
	{
		tile_row += tile_step;
		rows_left = 8u;
		do
		{
			mem_write32(tile_place, tile_row);
			tile_place += 4u;
			rows_left--;
		} while (rows_left != 0u);
		tiles_left--;
	} while (tiles_left != 0u);
	return tile_row;
}

/* Draws SELECT BY SERVICE BUTTON AND PUSH TEST BUTTON at the bottom of the screen. */
void test_mode_draw_instructions(uint32_t character)
{
	uint32_t text = 0;

	text = test_mode_instructions_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
}

/* Makes PUSH TEST BUTTON TO EXIT (the text record at 01F2BC, row 25) the prompt test_mode_blink_prompt
   blinks (test_mode_blink_address), from the next frame. */
void test_mode_show_exit_prompt(void)
{
	test_mode_blink_address = exit_prompt_text_ADDRESS;
	test_mode_blink_timer = 0;
}

/* Makes PUSH TEST BUTTON TO CONTINUE (the text record at 01F2DE) the prompt test_mode_blink_prompt
   blinks, from the next frame. */
void test_mode_show_continue_prompt(void)
{
	test_mode_blink_address = continue_prompt_text_ADDRESS;
	test_mode_blink_timer = 0;
}

/* Blinks the prompt whose text record test_mode_blink_address holds: when test_mode_blink_timer runs
   out, draws it (shown 21 frames) if its first cell is empty, else erases it (hidden 11 frames). */
void test_mode_blink_prompt(uint32_t character)
{
	uint32_t cell = 0, text = 0;
	uint8_t blink_timer = 0;

	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer >= 0)
		return;
	text = test_mode_blink_address;
	cell = 0x310000u | mem_read16(text);
	if (mem_read16(cell) == 0u)
	{
		test_mode_draw_texts(character, text);
		test_mode_blink_timer = 0x14;
		return;
	}
	do
	{
		mem_write16(cell, 0u);
		cell += 2u;
	} while (mem_read16(cell) != 0u);
	test_mode_blink_timer = 0xA;
}

/* Puts the cursor at text cell R10, the cell two before a menu line: test_mode_blink_address = R10 (the
   cursor, character 7F, blinks there: the menu loops' tail at 01F0AD), test_mode_blink_timer 0, and the
   line in white (6A, test_mode_colour_line). */
uint32_t test_mode_set_cursor(uint32_t character, uint32_t cell)
{
	uint32_t colour = 0;

	test_mode_blink_address = cell;
	test_mode_blink_timer = 0;
	cell += 4u;
	colour = (character & ~0xFFu) | 0x6Au;
	return test_mode_colour_line(colour, cell);
}

/* Takes the cursor off its line: clears its cell (test_mode_blink_address) and puts the line back in
   grey (60). */
void test_mode_clear_cursor(void)
{
	uint32_t cell = 0, colour = 0;

	cell = test_mode_blink_address;
	mem_write16(cell, 0u);
	cell += 4u;
	colour = 0x60u;
	colour = test_mode_colour_line(colour, cell);
}

/* Draws the text record at R10 on the text layer: lines of a cell (halfword), characters and colour
   changes (a negative byte: its complement is the colour, the cells' high byte), each closed by 0; a 0
   cell ends it. */
uint32_t test_mode_draw_texts(uint32_t character, uint32_t text)
{
	uint32_t cell = 0;

	cell = TEXT_CELL(0, 0);
	for (;;)
	{
		cell = (cell & ~0xFFFFu) | mem_read16(text);
		text += 2u;
		if ((uint16_t)cell == 0u)
			break;
		for (;;)
		{
			character = (character & ~0xFFu) | mem_read8(text);
			text++;
			if ((uint8_t)character == 0u)
				break;
			if ((int8_t)character >= 0)
			{
				mem_write16(cell, character);
				cell += 2u;
				continue;
			}
			character = (character & ~0xFFu) | (uint8_t)~character;
			character = (character & ~0xFFFFu) | (uint16_t)(character << 8);
		}
	}
	return character;
}

/* Draws the number R0 as test_mode_draw_number does, and R3 cells from R10 an S (or a blank for 1 or
   less) for the plural of the word beside it (COIN(S), CREDIT(S), GAME(S)); R1 holds the S's colour (the
   word's) in its low byte and the digits' in its second. */
void test_mode_draw_count(uint32_t number, uint32_t digit_count, uint32_t plural_offset, uint32_t *p_colours, uint32_t *p_cell)
{
	uint32_t colours = *p_colours;

	colours = colours << 8;
	if (number > 1u)
		colours |= 0x53u;
	mem_write16(*p_cell + plural_offset * 2u, colours);
	colours = byte_swap32(colours << 8);
	test_mode_draw_number(digit_count, &number, &colours, p_cell);
	*p_colours = colours;
}

/* Draws the number R0 in decimal in R2 cells ending at text cell R10 + 2 * R2, right-aligned in the
   colour in R1's low byte (the cells' high byte), the unused cells before it cleared. */
void test_mode_draw_number(uint32_t digit_count, uint32_t *p_number, uint32_t *p_colour, uint32_t *p_cell)
{
	uint8_t colour = (uint8_t)*p_colour;
	uint32_t cell = *p_cell;
	uint16_t zero_character = 0;
	uint32_t digit = 0;

	cell = cell + digit_count + digit_count;
	zero_character = (uint16_t)((uint16_t)((uint32_t)(uint8_t)colour << 8) | 0x30u);
	do
	{
		digit = 0u;
		divux_pair(0xAu, p_number, &digit);
		digit = (digit & ~0xFFFFu) | (uint16_t)(digit + zero_character);
		cell -= 2u;
		mem_write16(cell, digit);
		if (*p_number == 0u)
		{
			for (;;)
			{
				digit_count--;
				if (digit_count == 0u)
					break;
				cell -= 2u;
				mem_write16(cell, 0u);
			}
			break;
		}
		digit_count--;
	} while (digit_count != 0u);
	*p_colour = digit;
	*p_cell = cell;
}

/* test_mode_draw_number in hexadecimal (digits 0-9, A-F). */
uint32_t test_mode_draw_hex_number(uint32_t number, uint8_t colour, uint32_t digit_count, uint32_t cell)
{
	uint16_t zero_character = 0;
	uint32_t digit = 0;

	cell = cell + digit_count + digit_count;
	zero_character = (uint16_t)((uint16_t)((uint32_t)(uint8_t)colour << 8) | 0x30u);
	for (;;)
	{
		digit = 0u;
		divux_pair(0x10u, &number, &digit);
		digit = (uint16_t)(digit + zero_character);
		if ((int8_t)digit > 0x39)
			digit = (digit & ~0xFFu) | (uint8_t)(digit + 7u);
		cell -= 2u;
		mem_write16(cell, digit);
		if (number == 0u)
			break;
		digit_count--;
		if (digit_count == 0u)
			return cell;
	}
	for (;;)
	{
		digit_count--;
		if (digit_count == 0u)
			break;
		cell -= 2u;
		mem_write16(cell, 0u);
	}
	return cell;
}

/* Gives the text from cell R10 up to the next empty cell the colour R0 (60 grey, 6A white, 6E
   hidden...). */
uint32_t test_mode_colour_line(uint32_t colour, uint32_t cell)
{
	uint16_t character = 0;

	colour = (colour & ~0xFFFFu) | (uint16_t)(colour << 8);
	for (;;)
	{
		character = mem_read16(cell) & ~0xE00u;
		mem_write16(cell, character);
		if (character == 0u)
			break;
		mem_write16(cell, mem_read16(cell) | colour);
		cell += 2u;
	}
	return colour;
}

/* Clears the text layer (30 rows of 64 cells). */
uint32_t test_mode_clear_text(void)
{
	uint16_t blank_cell = 0;
	uint32_t cell_count = 0;

	cell_count = 0x780u;
	blank_cell = 0u;
	move_string(screen, 0u, TEXT_CELL(0, 0), 0x780u, 2u, 1u, 0u, blank_cell, &r27, &r28);
	return cell_count;
}

/* The test mode's controls, each frame: test_mode_buttons (SERVICE bit 0, TEST bit 1, START bit 4 and
   bit 5 of input port E, of either seat; bits 4-5 of port F count as SERVICE and TEST too),
   test_mode_buttons_previous, test_mode_buttons_pressed and test_mode_buttons_released; and the analog
   inputs into test_mode_analog_inputs, alternately (test_mode_flags bit 2, which also sets the
   converter's input bank): the first four (seat A's wheel, gas and brake, seat B's wheel) one frame,
   seat B's gas and brake the next. */
void test_mode_read_controls(void)
{
	uint32_t carry = 0;
	uint8_t buttons = 0, converter_bits = 0, port_f_pressed = 0, previous_buttons = 0, reading = 0;
	uint32_t analog_input = 0, bank_then_count = 0, bits_left = 0, converter_input = 0;
	uint8_t port_e_a = 0, port_e_b = 0, port_f_a = 0, port_f_b = 0;
	uint32_t flags_before = 0, test_mode_flags_long = 0;

	port_e_a = mem_read8(IO_C00008);
	port_e_b = mem_read8(0xC80008u);
	port_f_a = mem_read8(IO_C0000A);
	port_f_b = mem_read8(0xC8000Au);
	port_f_pressed = (uint8_t)(~port_f_a | ~port_f_b);
	buttons = ((~port_e_a | ~port_e_b) & 0x33u) | ((((uint32_t)port_f_pressed << 4) | (port_f_pressed >> 4)) & 3u);
	previous_buttons = test_mode_buttons;
	test_mode_buttons = buttons;
	test_mode_buttons_previous = previous_buttons;
	test_mode_buttons_pressed = ~previous_buttons & buttons;
	test_mode_buttons_released = previous_buttons & ~buttons;
	flags_before = (test_mode_flags | ((uint32_t)test_mode_cursor << 8u) | ((uint32_t)test_mode_page_cursor << 16u) | ((uint32_t)(test_mode_routine & 0xFFu) << 24u));
	carry = (flags_before & 4u) != 0u;
	test_mode_flags_long = flags_before ^ 4u;
	test_mode_flags = (uint8_t)test_mode_flags_long;
	test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	test_mode_routine = (test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	bank_then_count = carry;
	converter_input = IO_PORTS + 0x50u + bank_then_count * 4u;
	analog_input = test_mode_analog_inputs_ADDRESS + bank_then_count * 4u;
	bank_then_count = (uint8_t)(0u - bank_then_count);
	bank_then_count = (uint8_t)(bank_then_count + 2u);
	bank_then_count = (uint8_t)(bank_then_count * 2u);
	do
	{
		reading = 0u;
		mem_write8(converter_input, 0xFFu);
		bits_left = 8u;
		do
		{
			converter_bits = mem_read8(converter_input);
			carry = (uint8_t)(converter_bits * 2u) < converter_bits;
			reading = (uint8_t)(reading * 2u + carry);
			bits_left--;
		} while (bits_left != 0u);
		mem_write8(analog_input, reading);
		bank_then_count--;
		analog_input++;
		converter_input += 2u;
	} while (bank_then_count != 0u);
	carry = (test_mode_flags & 4u) != 0u;
	mem_write8(IO_C00060, carry);
}

/* Sends the test mode's next queued sound request (test_mode_sound_queue, test_mode_sound_queue_read) to
   the sound CPU, one a frame: to screen 2's command byte (SOUND_COMMAND_2) when the request has its high
   byte set, else to screen 1's (SOUND_COMMAND_1). */
void test_mode_send_sound(void)
{
	uint8_t inverted_command = 0;
	uint32_t read_index = 0;
	uint16_t inverted_request = 0;

	read_index = test_mode_sound_queue_read;
	if (test_mode_sound_queue_write == read_index)
		return;
	inverted_request = ~test_mode_sound_queue[read_index];
	inverted_command = (uint8_t)inverted_request;
	if ((int16_t)inverted_request >= 0)
		mem_write8(SOUND_COMMAND_2, ~inverted_command);
	else
		mem_write8(SOUND_COMMAND_1, ~inverted_command);
	test_mode_sound_queue_read = (read_index + 1u) & 0xFu;
}

/* test_mode_queue_sound for screen 2's sound CPU commands (the high byte set). */
void test_mode_queue_sound_screen_2(uint16_t request)
{
	request |= 0xFF00u;
	test_mode_queue_sound(request);
}

/* Puts sound request R0 in the test mode's ring of 16 (test_mode_sound_queue at
   test_mode_sound_queue_write) for test_mode_send_sound: screen 1's, or screen 2's with the high byte
   set (test_mode_queue_sound_screen_2). */
void test_mode_queue_sound(uint16_t request)
{
	uint32_t write_index = 0;

	write_index = test_mode_sound_queue_write;
	test_mode_sound_queue[write_index] = request;
	test_mode_sound_queue_write = (write_index + 1u) & 0xFu;
}

/* The vertical blank interrupt (IRQ 0), every video frame: counts vblank_counter and
   vblanks_since_start, starts timer 0 for the LCD (int_vector_43, 34 times: timer_interrupts_left); when
   it interrupted screen 2's task: both screens' mixers and palettes (vblank_update_mixer), the video
   registers (all 72 halfwords from video_registers when send_video_registers asks for them, which it
   clears, else the 8 at video_registers[16] and the 8 at video_registers[32]) and the external tile
   banks of chip 0's port H (2 bits each of tile_banks-tile_banks[3]); then for the interrupted task's
   screen the sprite list (finish_sprite_list), send_sound_requests, and vblank_update_scrolls unless the
   screen's skip_scroll_update is set (cleared here); outside the test mode the inputs
   (read_switches_and_analog), pack_seat_inputs (screen 1) or queue_seat_inputs (screen 2),
   check_test_button and, without free play, the coins (update_coins); then update_lamps, the LCD
   (lcd_refresh), count_total_time; sets frame_ready; swaps the tasks (task_swap) unless single_task is
   set, when task_swapping is set or when the test mode is on and screen 1's task is running; counts the
   frame for the running task's screen (screen_1_vblanks, screen_2_vblanks). */
void int_vector_40(void)
{
	uint32_t calibration = 0, screen_vblank_counter = 0, tile_bank_bits = 0;
	uint32_t saved_r28 = 0, saved_r27 = 0, saved_screen = 0, saved_r12 = 0, saved_screen_2 = 0;

	saved_r28 = r28;
	saved_r27 = r27;
	saved_screen = screen;
	saved_r12 = r12;
	vblank_counter++;
	vblanks_since_start++;
	mem_write16(IRQ_D00008, vblank_timer_period);
	timer_interrupts_left = 0x22u;
	debug_timer_rounds--;
	if (SCREEN(screen)->index != 0u)
	{
		saved_screen_2 = screen;
		screen = SCREEN_1;
		vblank_update_mixer();
		screen = SCREEN_2;
		vblank_update_mixer();
		screen = saved_screen_2;
		if (send_video_registers == 0u)
		{
			move_halfwords(VIDEO_RAM + 0x1FF20u, video_registers_ADDRESS + 0x20u, 8u);
			move_halfwords(VIDEO_RAM + 0x1FF40u, video_registers_ADDRESS + 0x40u, 8u);
		}
		else
		{
			send_video_registers = 0u;
			move_halfwords(VIDEO_31FF00, video_registers_ADDRESS, 0x48u);
		}
		tile_bank_bits = tile_banks[0];
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[1]);
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[2]);
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[3]);
		mem_write8(IO_C0000E, (tile_bank_bits << 6) | (tile_bank_bits >> 2));
	}
	finish_sprite_list();
	send_sound_requests();
	if (mem_read8(screen + 0xA7u) == 0u)
		vblank_update_scrolls();
	mem_write8(screen + 0xA7u, 0u);
	if (test_mode_on == 0u)
	{
		calibration = read_switches_and_analog();
		switch (mem_read8(screen))
		{
		case 0:
			pack_seat_inputs();
			break;
		case 1:
			tile_bank_bits = queue_seat_inputs();
			break;
		default:
			invalid_jump(vblank_seat_inputs_jumps[mem_read8(screen)]);
		}
		check_test_button(tile_bank_bits);
		if (free_play == 0u)
			update_coins(calibration);
	}
	update_lamps();
	lcd_refresh();
	count_total_time();
	frame_ready = 0xFFu;
	if (single_task == 0u && ((test_mode_on != 0u && mem_read8(screen) == 0u) || task_swapping != 0u))
		swap_tasks();
	screen_vblank_counter = screen_vblank_counters[mem_read8(screen)];
	mem_write8(screen_vblank_counter, mem_read8(screen_vblank_counter) + 1u);
	r12 = saved_r12;
	screen = saved_screen;
	r27 = saved_r27;
	r28 = saved_r28;
	mem_write8(IRQ_D00007, 0xFEu);
}

/* In the vertical blank, a screen's mixer and colours: while the screen's unk_94 bit 0 is set, a step of
   its colour offsets (step_screen_fade) into the mixer's colour offset registers (3E-4B); unk_94 bit 2
   (cleared) sends the mixer settings at 160 of the screen (registers 00-3D and 4C-4F); unk_94 bit 3
   (cleared) makes the set at 1B0 the current one and sends it; then the queued palettes
   (upload_queued_palettes). */
void vblank_update_mixer(void)
{
	uint32_t mixer = 0;
	uint32_t flags = 0, flags_2 = 0;

	if ((SCREEN(screen)->flags & 1u) != 0u)
	{
		step_screen_fade();
		move_string(screen + 0x19Eu, 7u, SCREEN(screen)->mixer + 0x3Eu, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	}
	flags = SCREEN(screen)->flags;
	SCREEN(screen)->flags = flags & ~4u;
	if ((flags & 4u) != 0u)
	{
		mixer = SCREEN(screen)->mixer;
		move_halfwords(mixer, screen + 0x160u, 0x1Fu);
		move_string(screen + 0x1ACu, 2u, mixer + 0x4Cu, 2u, 2u, 0u, 0u, 0u, &r27, &r28);
	}
	flags_2 = SCREEN(screen)->flags;
	SCREEN(screen)->flags = flags_2 & ~8u;
	if ((flags_2 & 8u) != 0u)
	{
		mixer = SCREEN(screen)->mixer;
		move_halfwords(screen + 0x160u, screen + 0x1B0u, 0x1Fu);
		move_halfwords(screen + 0x1ACu, screen + 0x1FCu, 2u);
		move_halfwords(mixer, screen + 0x1B0u, 0x1Fu);
		move_string(screen + 0x1FCu, 2u, mixer + 0x4Cu, 2u, 2u, 0u, 0u, 0u, &r27, &r28);
	}
	if (SCREEN(screen)->palette_upload_count != 0u)
		upload_queued_palettes();
}

/* In the vertical blank, the tilemaps' scrolls (two per screen): those of the screen whose task was
   running from video_registers[8] and video_registers[16] as they are; those of the other screen
   half-way between the values they have (video_registers[8], video_registers[16] by screen) and the new
   ones (scroll_targets, scroll_targets[4]), which become their values: the in-between frame of the
   screen whose task did not run (each task's scrolls change every other frame); and the byte at 31FF05
   from video_registers[2]'s second byte, which steps by 4 (bits 2-3). */
void vblank_update_scrolls(void)
{
	uint8_t control_byte = 0;
	uint32_t new_scroll = 0, old_scroll = 0, other_layer = 0, running_layer = 0;

	running_layer = SCREEN(screen)->index * 2u;
	mem_write32(VIDEO_RAM + 0x1FF10u + running_layer * 4u, RAM32(video_registers_ADDRESS + 0x10u + running_layer * 4u));
	mem_write32(VIDEO_RAM + 0x1FF14u + running_layer * 4u, RAM32(video_registers_ADDRESS + 0x14u + running_layer * 4u));
	other_layer = 0u - running_layer + 2u;
	old_scroll = RAM32(video_registers_ADDRESS + 0x10u + other_layer * 4u);
	new_scroll = scroll_targets[other_layer];
	RAM32(video_registers_ADDRESS + 0x10u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 6;
	mem_write32(VIDEO_RAM + 0x1FF10u + other_layer * 4u, (((int32_t)((old_scroll << 6) - new_scroll) >> 1) + new_scroll) >> 6);
	old_scroll = RAM32(video_registers_ADDRESS + 0x14u + other_layer * 4u);
	new_scroll = scroll_targets[1 + other_layer];
	RAM32(video_registers_ADDRESS + 0x14u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 7;
	mem_write32(VIDEO_RAM + 0x1FF14u + other_layer * 4u, (((int32_t)((old_scroll << 7) - new_scroll) >> 1) + new_scroll) >> 7);
	mem_write32(VIDEO_RAM + 0x1FF20u + running_layer * 4u, RAM32(video_registers_ADDRESS + 0x20u + running_layer * 4u));
	mem_write32(VIDEO_RAM + 0x1FF24u + running_layer * 4u, RAM32(video_registers_ADDRESS + 0x24u + running_layer * 4u));
	old_scroll = RAM32(video_registers_ADDRESS + 0x20u + other_layer * 4u);
	new_scroll = scroll_targets[4 + other_layer];
	RAM32(video_registers_ADDRESS + 0x20u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 6;
	mem_write32(VIDEO_RAM + 0x1FF20u + other_layer * 4u, (((int32_t)((old_scroll << 6) - new_scroll) >> 1) + new_scroll) >> 6);
	old_scroll = RAM32(video_registers_ADDRESS + 0x24u + other_layer * 4u);
	new_scroll = scroll_targets[5 + other_layer];
	RAM32(video_registers_ADDRESS + 0x24u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 7;
	mem_write32(VIDEO_RAM + 0x1FF24u + other_layer * 4u, (((int32_t)((old_scroll << 7) - new_scroll) >> 1) + new_scroll) >> 7);
	control_byte = (uint8_t)(video_registers[2] >> 8u);
	mem_write8(VIDEO_31FF05, control_byte);
	video_registers[2] = (video_registers[2] & 0xFFu) | (((control_byte + 4u) & 0xECu & 0xFFu) << 8u);
}

/* The end of the vertical blank (IRQ 1): only acknowledged. */
void int_vector_41(void)
{
	mem_write8(IRQ_D00007, 0xFDu);
}

/* The sound CPU's interrupt (IRQ 2): only acknowledged. The sound program never raises it. */
void int_vector_42(void)
{
	mem_write8(IRQ_D00007, 0xFBu);
}

/* Timer 0 (IRQ 3), which int_vector_40 starts each frame at its shortest period: sends the LCD its next
   piece (lcd_send_next_nibble) and starts the timer again until timer_interrupts_left runs out, 34 times
   a frame. When it runs out it tests the screen's byte 1 and test_mode_on but goes the same way; the
   code after them (unused_03F7D8: counts debug_timer_rounds and shows it on the text layer) is never
   reached. */
void int_vector_43(void)
{
	uint8_t interrupts_left = 0;

	interrupts_left = timer_interrupts_left;
	timer_interrupts_left = interrupts_left - 1u;
	if (interrupts_left != 1u)
		mem_write16(IRQ_D00008, lcd_timer_period);
	lcd_send_next_nibble();
	mem_write8(IRQ_D00007, 0xF7u);
}

/* Timer 1 (IRQ 4), the link board's: outside the ISDN mode (isdn_mode) only acknowledged, which leaves
   the timer stopped; in it counts link_wait_count and takes the kind of the link board's message (its
   byte at 800010, cleared as it is read): 3, a message, is copied out of the board
   (link_isdn_receive_records: 800600 on to 21E000), link_wait_count cleared and the timer started at
   168; any other kind polls again at the shortest period. link_unused_case is a fourth case (the
   controls, pack_seat_inputs, then 96) its table does not list. */
void int_vector_44(void)
{
	uint16_t timer_period = 0;
	uint32_t message_kind = 0;
	uint8_t received_kind = 0;

	if (isdn_mode == 0u)
	{
		mem_write8(IRQ_D00007, 0xEFu);
		return;
	}
	link_wait_count++;
	timer_period = 1u;
	received_kind = mem_read8(0x800010u);
	message_kind = received_kind;
	mem_write8(0x800010u, 0u);
	if (message_kind < 4u)
	{
		switch (message_kind)
		{
		case 3:
			link_isdn_receive_records();
			timer_period = 0x168u;
			link_wait_count = 0u;
			break;
		case 0:
		case 1:
		case 2:
			break;
		default:
			invalid_jump(link_message_kind_jumps[message_kind]);
		}
	}
	mem_write16(IRQ_D0000A, timer_period);
	mem_write8(IRQ_D00007, 0xEFu);
}

/* The vectors no interrupt uses (250 of the 256): halt instructions, on into int_vector_02's. */
void int_default(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	int_vector_02();
}

/* The NMI: halt instructions, then on into check_test_button (whose return is not an interrupt's). */
void int_vector_02(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	check_test_button(0u);
}

/* Turns the test mode on (test_mode_on FF) when TEST has just been pressed (bit 7 of switches_pressed,
   the inputs newly pressed); the boot task's loop then runs the test mode. */
void check_test_button(uint32_t leftover)
{
	if ((((leftover & ~0xFFu) | switches_pressed) & 0x80u) != 0u)
		test_mode_on = 0xFFu;
}

/* In the vertical blank, sends the screen's next sound request to the sound CPU: one from its ring of 32
   (queue_sound's: sound_queue_read, sound_queue_write, the requests after them) to its command byte
   (SOUND_COMMAND_1 for screen 1, SOUND_COMMAND_2 for screen 2), and one from its second ring at 840
   (second_sound_queue_read, second_sound_queue_write; only the unused request_second_sound fills it) to
   the byte 8 after it, which the sound program never reads. */
void send_sound_requests(void)
{
	uint32_t command_offset = 0, queue = 0, second_queue = 0, second_sound_queue_read = 0, sound_queue_read = 0;
	uint8_t sound_queue_write = 0, second_sound_queue_write = 0;

	command_offset = (uint32_t)SCREEN(screen)->index << 8;
	queue = screen + 0x800u;
	sound_queue_read = SCREEN(queue - 0x800u)->sound_queue_read;
	queue++;
	sound_queue_write = SCREEN(queue - 0x801u)->sound_queue_write;
	queue++;
	if (sound_queue_read != sound_queue_write)
	{
		mem_write8(command_offset + SOUND_COMMAND_1, mem_read8(queue + sound_queue_read));
		SCREEN(queue - 0x802u)->sound_queue_read = (sound_queue_read + 1u) & 0x1Fu;
	}
	second_queue = screen + 0x840u;
	second_sound_queue_read = SCREEN(second_queue - 0x840u)->second_sound_queue_read;
	second_queue++;
	second_sound_queue_write = SCREEN(second_queue - 0x841u)->second_sound_queue_write;
	second_queue++;
	if (second_sound_queue_read != second_sound_queue_write)
	{
		mem_write8(command_offset + SOUND_SECOND_COMMAND_1, mem_read8(second_queue + second_sound_queue_read));
		SCREEN(second_queue - 0x842u)->second_sound_queue_read = (second_sound_queue_read + 1u) & 0x1Fu;
	}
}

/* Counts the bookkeeping's TOTAL TIME (701F10, seconds, kept with its copy and checksum), outside the
   test mode only: total_time_count gains 17 every vertical blank and a second passes at each 1000 (2000
   for every 73rd second). */
void count_total_time(void)
{
	uint32_t time_count = 0, total_time = 0;

	if (test_mode_on != 0u)
		return;
	time_count = (uint16_t)(total_time_count + 0x11u);
	if (time_count < 0x3E8u)
	{
		total_time_count = time_count;
		return;
	}
	total_time = SOUND_701F10;
	if (mem_read32(SOUND_701F10) % 0x49u != 0u)
	{
		total_time_count = time_count - 0x3E8u;
		if (mem_read32(total_time) != 0xFFFFFFFFu)
		{
			mem_write32(total_time, mem_read32(total_time) + 1u);
			bookkeeping_update_checksum(time_count);
			mem_write32(total_time + 0x7Cu, mem_read32(total_time + 0x7Cu) + 1u);
		}
		return;
	}
	if (time_count < 0x7D0u)
	{
		total_time_count = time_count;
		return;
	}
	time_count = (uint16_t)(time_count - 0x3E8u);
	total_time_count = time_count - 0x3E8u;
	if (mem_read32(total_time) != 0xFFFFFFFFu)
	{
		mem_write32(total_time, mem_read32(total_time) + 1u);
		bookkeeping_update_checksum(time_count);
		mem_write32(total_time + 0x7Cu, mem_read32(total_time + 0x7Cu) + 1u);
	}
}

/* In the vertical blank, the cabinet's lamps: blinks each seat's CHECK POINT lamp (output port D bit 1:
   checkpoint_lamp, checkpoint_lamp_2) and RACE LEADER lamp (bit 3: leader_lamp, leader_lamp_2), each
   with a blinks, a period and a timer byte after it: while blinks is not 0 the timer counts the vertical
   blanks and at 0 toggles the lamp, starts again from period and takes one off blinks; writes output
   port D of each I/O chip from port_d_outputs or port_d_outputs[1] with those lamps and, when the seat
   is not in the attract mode (screen demo_flags bit 8) and the STEERING WHEEL REACTION setting
   (steering_wheel_reaction) is on, the wheel motor's bits (wheel_motor by seat); then the music and
   arrows lamps (update_button_lamps). */
void update_lamps(void)
{
	uint8_t port_d = 0;
	uint32_t port_g = 0, screen_block = 0;
	uint8_t checkpoint_timer = 0, leader_timer = 0, checkpoint_2_timer = 0, leader_2_timer = 0;

	if (checkpoint_lamp_blinks != 0u)
	{
		checkpoint_timer = checkpoint_lamp_timer;
		checkpoint_lamp_timer = checkpoint_timer - 1u;
		if (checkpoint_timer == 1u)
		{
			checkpoint_lamp_timer = checkpoint_lamp_period;
			checkpoint_lamp[0] ^= 2u;
			checkpoint_lamp_blinks--;
		}
	}
	if (leader_lamp_blinks != 0u)
	{
		leader_timer = leader_lamp_timer;
		leader_lamp_timer = leader_timer - 1u;
		if (leader_timer == 1u)
		{
			leader_lamp_timer = leader_lamp_period;
			leader_lamp[0] ^= 8u;
			leader_lamp_blinks--;
		}
	}
	port_d = port_d_outputs[0] | checkpoint_lamp[0] | leader_lamp[0];
	if ((SCREEN(SCREEN_1)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		port_d |= wheel_motor[0];
	mem_write8(IO_C00006, port_d);
	if (checkpoint_lamp_2_blinks != 0u)
	{
		checkpoint_2_timer = checkpoint_lamp_2_timer;
		checkpoint_lamp_2_timer = checkpoint_2_timer - 1u;
		if (checkpoint_2_timer == 1u)
		{
			checkpoint_lamp_2_timer = checkpoint_lamp_2_period;
			checkpoint_lamp_2 ^= 2u;
			checkpoint_lamp_2_blinks--;
		}
	}
	if (leader_lamp_2_blinks != 0u)
	{
		leader_2_timer = leader_lamp_2_timer;
		leader_lamp_2_timer = leader_2_timer - 1u;
		if (leader_2_timer == 1u)
		{
			leader_lamp_2_timer = leader_lamp_2_period;
			leader_lamp_2 ^= 8u;
			leader_lamp_2_blinks--;
		}
	}
	port_d = port_d_outputs[1] | checkpoint_lamp_2 | leader_lamp_2;
	if ((SCREEN(SCREEN_2)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		port_d |= wheel_motor[1];
	mem_write8(0xC80006u, port_d);
	screen_block = SCREEN_1;
	port_g = port_g_outputs_ADDRESS;
	update_button_lamps(screen_block, port_g);
	screen_block = SCREEN_2;
	port_g = port_g_outputs_ADDRESS + 0x1u;
	update_button_lamps(screen_block, port_g);
}

/* A screen's two button lamps, in its port G image (R11: port_g_outputs or port_g_outputs[1], which the
   LCD's output sends): the DJ/MUSIC lamp (bit 0) by the low byte of button_lamps and the << >> lamp (bit
   1) by its high byte: 0 off, negative on, n blinking every n vertical blanks (music_lamp_timer,
   arrows_lamp_timer), each mode taken when it changes (music_lamp_mode, arrows_lamp_mode). */
void update_button_lamps(uint32_t screen_block, uint32_t port_g)
{
	uint8_t arrows_mode = 0, button_lamps = 0, lamps = 0;
	uint8_t music_lamp_timer = 0, arrows_lamp_timer = 0;

	lamps = mem_read8(port_g);
	button_lamps = (uint8_t)SCREEN(screen_block)->button_lamps;
	if (button_lamps != SCREEN(screen_block)->music_lamp_mode)
	{
		SCREEN(screen_block)->music_lamp_mode = button_lamps;
		if ((int8_t)button_lamps > 0)
		{
			lamps ^= 1u;
			SCREEN(screen_block)->music_lamp_timer = button_lamps;
		}
		else if (button_lamps == 0u)
			lamps &= ~1u;
		else
			lamps |= 1u;
	}
	else if ((int8_t)button_lamps > 0)
	{
		music_lamp_timer = SCREEN(screen_block)->music_lamp_timer;
		SCREEN(screen_block)->music_lamp_timer = music_lamp_timer - 1u;
		if (music_lamp_timer == 1u)
		{
			lamps ^= 1u;
			SCREEN(screen_block)->music_lamp_timer = button_lamps;
		}
	}
	arrows_mode = (uint8_t)(SCREEN(screen_block)->button_lamps >> 8u);
	if (arrows_mode == SCREEN(screen_block)->arrows_lamp_mode)
	{
		if ((int8_t)arrows_mode > 0)
		{
			arrows_lamp_timer = SCREEN(screen_block)->arrows_lamp_timer;
			SCREEN(screen_block)->arrows_lamp_timer = arrows_lamp_timer - 1u;
			if (arrows_lamp_timer == 1u)
			{
				lamps ^= 2u;
				SCREEN(screen_block)->arrows_lamp_timer = arrows_mode;
			}
		}
		mem_write8(port_g, lamps);
		return;
	}
	SCREEN(screen_block)->arrows_lamp_mode = arrows_mode;
	if ((int8_t)arrows_mode > 0)
	{
		lamps ^= 2u;
		SCREEN(screen_block)->arrows_lamp_timer = arrows_mode;
		mem_write8(port_g, lamps);
		return;
	}
	if (arrows_mode == 0u)
	{
		lamps &= ~2u;
		mem_write8(port_g, lamps);
		return;
	}
	lamps |= 2u;
	mem_write8(port_g, lamps);
}

/* The V70's reset vector, at the top of the program's address space: a jump to boot. */
void reset(void)
{
	boot();
}
