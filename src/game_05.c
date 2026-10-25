/**
 *    Name:        game_05.c
 *    Description: the game's functions, part 05 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
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
	uint32_t r10 = 0;

	r10 = 0x1DD14u;
	current_object = settings_magic_ADDRESS;
	do
	{
		current_object = (current_object & ~0xFFu) | ROM8(r10);
		r10++;
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
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1E0F4u;
	test_mode_draw_texts(r0, r10);
	test_mode_coin_assignments_draw(&r0, &r1);
	test_mode_draw_instructions(r0);
	r10 = 0x310000u | rom_01E200[test_mode_page_cursor];
	test_mode_set_cursor(r0, r10);
	test_mode_routine = test_mode_coin_assignments_run_ADDRESS;
	test_mode_coin_assignments_run(r1);
}

/* The coin assignments, each frame: SERVICE moves the cursor through the three items; TEST on
   COIN/CREDIT SETTING takes the next setting (test_mode_coin_assignments_next_setting), on MANUAL
   SETTING opens that page (test_mode_coin_assignments_manual_start), on EXIT leaves
   (test_mode_coin_assignments_exit); the arrow blinks. Its table's last four entries belong to the
   manual setting page, whose table starts inside this one: they are never taken from here. */
void test_mode_coin_assignments_run(uint32_t r1)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor;
		r0 = (uint8_t)(r0 + 1u);
		r1 = (r1 & ~0xFFu) | 0xCu;
		r1 = (r1 & ~0xFFu) | ((uint8_t)r1 >> 2);
		if ((int8_t)r1 <= (int8_t)r0)
			r0 = 0u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01E200[r0];
		test_mode_set_cursor(r0, r10);
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
			test_mode_coin_assignments_manual_start(r1);
			break;
		case 2:
			test_mode_coin_assignments_exit();
			break;
		case 3:
			test_mode_coin_assignments_step_coin_to_credit(r1);
			break;
		case 4:
			test_mode_coin_assignments_step_bonus_adder(r1);
			break;
		case 5:
			test_mode_coin_assignments_step_multiplier(r1);
			break;
		case 6:
			test_mode_coin_assignments_manual_exit();
			break;
		default:
			invalid_jump(rom_01E0D8[test_mode_page_cursor]);
		}
	}
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the coin/credit setting: its number (coin_setting + 1) and what it gives, from its description
   (rom_01E220): up to five lines of "n COIN(S) m CREDIT(S)", or a text (FREE PLAY, MANUAL SETTING). */
void test_mode_coin_assignments_draw(uint32_t *p_r0, uint32_t *p_r1)
{
	uint32_t zero = 0;
	uint16_t r26 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;

	current_object = settings_magic_ADDRESS;
	r10 = TEXT_CELL(32, 9);
	r1 = 0x64u;
	r2 = 2u;
	r0 = coin_setting + 1u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r11 = rom_01E220[coin_setting];
	r26 = 0u;
	r12 = TEXT_CELL(17, 10);
	r9 = 5u;
	for (;;)
	{
		r0 = mem_read8(r11);
		zero = r0 == 0u;
		r11++;
		if ((int8_t)r0 <= 0)
			break;
		r1 = (r1 & ~0xFFFFu) | 0x6400u;
		r10 = r12;
		r13 = 0x1E20Eu;
		do
		{
			mem_write16(r10, r1);
			r1 = (r1 & ~0xFFu) | ROM8(r13);
			r10 += 2u;
			r13++;
		} while ((uint8_t)r1 != 0u);
		r10 = r12;
		r1 = (r1 & ~0xFFFFu) | 0x6464u;
		r2 = 1u;
		r3 = 6u;
		test_mode_draw_count(r0, r2, r3, &r1, &r10);
		r10 = r12 + 0x10u;
		r0 = mem_read8(r11);
		r1 = (r1 & ~0xFFFFu) | 0x6464u;
		r2 = 1u;
		r3 = 8u;
		r11++;
		test_mode_draw_count(r0, r2, r3, &r1, &r10);
		r9--;
		r12 = r12 + 0x80u;
	}
	if (r9 == 0u)
	{
		*p_r0 = r0;
		*p_r1 = r1;
		return;
	}
	r10 = r12;
	do
	{
		move_string(screen, 0u, r10, 0x11u, 2u, 1u, 0u, r26, &r27, &r28);
		r9--;
		r10 += 0x80u;
	} while (r9 != 0u);
	if (zero)
	{
		*p_r0 = r0;
		*p_r1 = r1;
		return;
	}
	r0 = 0x6200u;
	for (;;)
	{
		r0 = (r0 & ~0xFFu) | mem_read8(r11);
		r11++;
		if ((uint8_t)r0 == 0u)
			break;
		mem_write16(r12, r0);
		r12 = r12 + 2u;
	}
	*p_r0 = r0;
	*p_r1 = r1;
}

/* COIN/CREDIT SETTING: the next of the 26 settings (coin_setting, after FREE PLAY back to */
void test_mode_coin_assignments_next_setting(void)
{
	uint32_t r0 = 0, r10 = 0, ignored = 0;

	do
	{
		r0 = coin_setting;
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0x1A)
			r0 = 0u;
		coin_setting = r0;
		r10 = 0x1E3E8u + r0 * 4u;
		if ((int8_t)ROM8(r10) < 0)
		{
			coins_per_credit = 1u;
			bonus_adder = 0xFFu;
			first_chute_multiplier = 1u;
			second_chute_multiplier = 1u;
			free_play = 0xFFu;
			test_mode_coin_assignments_draw(&ignored, &ignored);
			return;
		}
	} while (r0 != 0u && coins_per_credit == ROM8(r10) && bonus_adder == ROM8(r10 + 1u) && first_chute_multiplier == ROM8(r10 + 2u));
	coins_per_credit = ROM8(r10);
	r10++;
	bonus_adder = ROM8(r10);
	r10++;
	first_chute_multiplier = ROM8(r10);
	second_chute_multiplier = ROM8(r10 + 1u);
	free_play = 0u;
	test_mode_coin_assignments_draw(&ignored, &ignored);
}

/* Opens the coin assignments' MANUAL SETTING page: clears the text, draws its texts (COIN TO CREDIT,
   BONUS ADDER, MULTIPLIER, the table of credits for 1 to 9 coins, EXIT) and the setting
   (test_mode_coin_assignments_manual_draw), the instructions, puts the cursor on EXIT
   (test_mode_page_cursor 3) and goes on in test_mode_coin_assignments_manual_run. */
void test_mode_coin_assignments_manual_start(uint32_t r1)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1E13Fu;
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_coin_assignments_manual_draw(r1);
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 3u;
	r10 = 0x310000u | rom_01E20C;
	test_mode_set_cursor(r0, r10);
	test_mode_routine = test_mode_coin_assignments_manual_run_ADDRESS;
}

/* Each frame of the MANUAL SETTING page: SERVICE moves the cursor down its four lines
   (test_mode_page_cursor, wrapping), TEST changes the line's value: COIN TO CREDIT
   (test_mode_coin_assignments_step_coin_to_credit), BONUS ADDER
   (test_mode_coin_assignments_step_bonus_adder), MULTIPLIER (test_mode_coin_assignments_step_multiplier)
   or EXIT (test_mode_coin_assignments_manual_exit); then blinks the cursor. */
void test_mode_coin_assignments_manual_run(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor;
		r0 = (uint8_t)(r0 + 1u);
		r1 = 0x10u;
		r1 = r1 >> 2;
		if ((int8_t)r1 <= (int8_t)r0)
			r0 = 0u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01E206[r0];
		test_mode_set_cursor(r0, r10);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		switch (test_mode_page_cursor)
		{
		case 0:
			test_mode_coin_assignments_step_coin_to_credit(r1);
			break;
		case 1:
			test_mode_coin_assignments_step_bonus_adder(r1);
			break;
		case 2:
			test_mode_coin_assignments_step_multiplier(r1);
			break;
		case 3:
			test_mode_coin_assignments_manual_exit();
			break;
		default:
			invalid_jump(rom_01E0E4[test_mode_page_cursor]);
		}
	}
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the manual coin setting: coins_per_credit (N COIN(S) 1 CREDIT), the bonus_adder (N COINS GIVE 1
   EXTRA COIN, NO BONUS ADDER, or FREE PLAY when negative), the first_chute_multiplier (1 COIN COUNTS AS
   N COIN(S)), and the table of what 1 to 9 coins give: each coin counts as the multiplier, the bonus
   adds a coin for every bonus_adder counted (the rest carried on), and the credits (at most 9) show as
   two digits (test_mode_draw_two_digits) with what is left over as a fraction of a credit in the font's
   fraction characters. */
uint32_t test_mode_coin_assignments_manual_draw(uint32_t r1)
{
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	current_object = settings_magic_ADDRESS;
	r10 = TEXT_CELL(23, 8);
	r0 = coins_per_credit;
	r1_2 = (r1 & ~0xFFFFu) | 0x6464u;
	r2 = 1u;
	r3 = 6u;
	test_mode_draw_count(r0, r2, r3, &r1_2, &r10);
	r0 = bonus_adder;
	if ((int8_t)r0 > 0)
	{
		r10 = (r10 & ~0xFFFFu) | 0xF51Eu;
		r1_2 = 0x64u;
		r2 = 1u;
		test_mode_draw_number(r2, &r0, &r1_2, &r10);
		r10 = 0x1E3CBu;
		test_mode_draw_texts(r0, r10);
	}
	else if ((int8_t)r0 < 0)
	{
		r10 = 0x1E3BCu;
		test_mode_draw_texts(r0, r10);
	}
	else
	{
		r10 = 0x1E39Eu;
		test_mode_draw_texts(r0, r10);
	}
	r10 = TEXT_CELL(32, 12);
	r0 = first_chute_multiplier;
	r1_2 = (r1_2 & ~0xFFFFu) | 0x6464u;
	r2 = 1u;
	r3 = 6u;
	test_mode_draw_count(r0, r2, r3, &r1_2, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF784u;
	r0 = coins_per_credit;
	r1_2 = (int8_t)bonus_adder;
	r2 = first_chute_multiplier;
	r3 = 0u;
	r4 = 0u;
	r9 = 9u;
	do
	{
		r3 += r2;
		if ((int32_t)r1_2 > 0)
		{
			r4 += r2;
			r5 = 0u;
			divux_pair(r1_2, &r4, &r5);
			r3 += r4;
			r4 = r5;
		}
		r5 = r3;
		r6 = 0u;
		divux_pair(r0, &r5, &r6);
		if ((uint8_t)r5 >= 9u)
		{
			r5 = (r5 & ~0xFFu) | 9u;
			r6 = 0u;
		}
		stack_4 = r6;
		r10 = test_mode_draw_two_digits(r5, r10);
		r5 = stack_4;
		if (r5 != 0u)
		{
			mem_write16(r10, r5 + 0x6A7Fu);
			r10 += 2u;
			mem_write16(r10, r0 + 0x6A86u);
			r10 += 2u;
		}
		else
		{
			mem_write32(r10, 0x200020u);
			r10 += 4u;
		}
		r9--;
	} while (r9 != 0u);
	return r0;
}

/* Draws R5 (below 100) at text cell R10 as two digits in white (6A), a blank for a 0 tens digit, and
   returns the cell after them. Used by test_mode_coin_assignments_manual_draw. */
uint32_t test_mode_draw_two_digits(uint32_t r5, uint32_t r10)
{
	uint32_t zero = 0;
	uint32_t r6 = 0;

	r6 = 0u;
	divux_pair(0xAu, &r5, &r6);
	zero = (uint32_t)(r5 == 0u);
	if (zero == 0u)
	{
		mem_write16(r10, r5 + 0x6A30u);
		r10 += 2u;
	}
	else
	{
		mem_write16(r10, 0x20u);
		r10 += 2u;
	}
	mem_write16(r10, r6 + 0x6A30u);
	r10 += 2u;
	return r10;
}

/* COIN TO CREDIT: test_mode_coin_assignments_step_value with coins_per_credit. */
void test_mode_coin_assignments_step_coin_to_credit(uint32_t r1)
{
	uint32_t r0 = 0;

	r0 = 0x10u;
	test_mode_coin_assignments_step_value(r0, r1);
}

/* Steps the coin setting R0 bytes into the settings (coins_per_credit or first_chute_multiplier) from 1
   to 9 and round; unless free play is on, the coin setting becomes the manual one (coin_setting 1A);
   redraws (test_mode_coin_assignments_manual_draw). */
void test_mode_coin_assignments_step_value(uint32_t r0, uint32_t r1)
{
	uint32_t r1_2 = 0, ignored = 0;

	r1_2 = (r1 & ~0xFFu) | mem_read8(current_object + r0);
	r1_2 = (r1_2 & ~0xFFu) | (uint8_t)(r1_2 + 1u);
	if ((uint8_t)r1_2 >= 0xAu)
		r1_2 = (r1_2 & ~0xFFu) | 1u;
	mem_write8(current_object + r0, r1_2);
	if (free_play == 0u)
		coin_setting = 0x1Au;
	ignored = test_mode_coin_assignments_manual_draw(r1_2);
}

/* BONUS ADDER: steps bonus_adder through none (0), 2 to 9 and back to none; from free play (FF) it goes
   to none and turns free_play off; the coin setting becomes the manual one (coin_setting 1A); redraws. */
void test_mode_coin_assignments_step_bonus_adder(uint32_t r1)
{
	uint8_t r0 = 0;
	uint32_t ignored = 0;
	uint8_t t1 = 0;

	r0 = bonus_adder;
	if (r0 == 0u)
		r0++;
	t1 = r0 + 1u;
	r0 = t1;
	if (t1 == 0u)
		free_play = 0u;
	if (r0 >= 0xAu)
		r0 = 0u;
	bonus_adder = r0;
	coin_setting = 0x1Au;
	ignored = test_mode_coin_assignments_manual_draw(r1);
}

/* MULTIPLIER: test_mode_coin_assignments_step_value with first_chute_multiplier (1 COIN COUNTS AS N
   COIN). */
void test_mode_coin_assignments_step_multiplier(uint32_t r1)
{
	uint32_t r0 = 0;

	r0 = 0x11u;
	test_mode_coin_assignments_step_value(r0, r1);
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
	uint32_t r10 = 0;

	r10 = 0x1E44Du;
	do
	{
		current_object = (current_object & ~0xFFu) | ROM8(r10);
		r10++;
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
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1E62Cu;
	test_mode_draw_texts(r0, r10);
	current_object = settings_magic_ADDRESS;
	if (stand_alone != 0u)
	{
		r10 = TEXT_CELL(8, 12);
		r0 = 0x6Eu;
		test_mode_colour_line(r0, r10);
		r10 = TEXT_CELL(8, 14);
		r0 = 0x6Eu;
		test_mode_colour_line(r0, r10);
	}
	test_mode_privilege_mode = link_master & 1u;
	r0 = test_mode_network_assignments_draw();
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 3u;
	r10 = 0x310000u | rom_01E682;
	test_mode_set_cursor(r0, r10);
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
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor;
		if (stand_alone == 0u)
		{
			r0 = (uint8_t)(r0 + 1u);
			if ((int8_t)r0 >= 4)
				r0 = 0u;
		}
		else
			r0 = r0 ^ 3u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01E67C[r0];
		test_mode_set_cursor(r0, r10);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		r0 = test_mode_page_cursor;
		switch (r0)
		{
		case 0:
			test_mode_network_assignments_toggle_communication(r0);
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
			invalid_jump(rom_01E61C[r0]);
		}
	}
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the network settings: COMMUNICATION NETWORK or STAND-ALONE (stand_alone), PRIVILEGE MODE SLAVE
   or MASTER (test_mode_privilege_mode), CABINET ID 1-4 (cabinet_id + 1); the last two hidden (colour 6E)
   when stand-alone. */
uint32_t test_mode_network_assignments_draw(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;

	current_object = settings_magic_ADDRESS;
	r0 = stand_alone;
	r0 = r0 & 1u;
	r10 = rom_01E684[r0];
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_privilege_mode;
	r10 = rom_01E68C[r0];
	test_mode_draw_texts(r0, r10);
	r10 = TEXT_CELL(33, 14);
	r1 = 0x64u;
	r2 = 1u;
	r0 = cabinet_id + 1u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	if (stand_alone != 0u)
	{
		r10 = TEXT_CELL(28, 12);
		r0 = (r0 & ~0xFFu) | 0x6Eu;
		r0 = test_mode_colour_line(r0, r10);
		r10 = TEXT_CELL(33, 14);
		r0 = (r0 & ~0xFFu) | 0x6Eu;
		r0 = test_mode_colour_line(r0, r10);
	}
	return r0;
}

/* COMMUNICATION: switches stand_alone (NETWORK, STAND-ALONE), puts the privilege mode back to MASTER
   (link_master 1, isdn_mode 0, test_mode_privilege_mode 1), shows or hides the PRIVILEGE MODE and
   CABINET ID labels and redraws the values. */
void test_mode_network_assignments_toggle_communication(uint32_t r0)
{
	uint32_t carry = 0;
	uint32_t r0_2 = 0, r10 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;

	carry = stand_alone > 0u;
	r0_2 = (r0 & ~0xFFu) | 1u;
	t1 = r0_2;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(t1 - carry);
	stand_alone = r0_2;
	link_master = 1u;
	isdn_mode = 0u;
	test_mode_privilege_mode = 1u;
	r0_2 = (r0_2 & ~0xFFu) | 0x60u;
	if (t1 != carry)
		r0_2 = (r0_2 & ~0xFFu) | 0x6Eu;
	r10 = TEXT_CELL(8, 12);
	stack_4 = r0_2;
	test_mode_colour_line(r0_2, r10);
	r10 = TEXT_CELL(8, 14);
	r0_2 = stack_4;
	test_mode_colour_line(r0_2, r10);
	ignored = test_mode_network_assignments_draw();
}

/* PRIVILEGE MODE: switches test_mode_privilege_mode between SLAVE and MASTER and sets link_master and
   isdn_mode from it (01E6E5: SLAVE 0 and 0, MASTER 1 and 0; the table's third pair, ISDN, 0 and 1,
   cannot be reached); redraws. */
void test_mode_network_assignments_toggle_privilege_mode(void)
{
	uint32_t r0 = 0, r10 = 0, ignored = 0;
	uint8_t t1 = 0;

	r0 = test_mode_privilege_mode;
	t1 = r0 - 1u;
	r0 = t1;
	if ((int8_t)t1 < 0)
		r0 = 1u;
	test_mode_privilege_mode = r0;
	r10 = 0x1E6E5u + r0 * 2u;
	link_master = ROM8(r10);
	isdn_mode = ROM8(r10 + 1u);
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
	uint32_t r10 = 0;

	r10 = 0x1E6E0u;
	do
	{
		current_object = (current_object & ~0xFFu) | ROM8(r10);
		r10++;
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
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1EA5Cu;
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_bookkeeping_draw_totals();
	test_mode_show_continue_prompt();
	test_mode_routine = test_mode_bookkeeping_wait_ADDRESS;
	test_mode_bookkeeping_wait((uint16_t)r0);
}

/* Blinks the prompt (test_mode_blink_prompt) until TEST, then the second page
   (test_mode_bookkeeping_second_page_start). */
void test_mode_bookkeeping_wait(uint16_t r0)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_bookkeeping_second_page_start_ADDRESS;
	test_mode_blink_prompt(r0);
}

/* Opens the second BOOKKEEPING page (2/2): its texts, the time histogram and the course records
   (test_mode_bookkeeping_draw_histogram) and the blinking PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt); goes on in test_mode_bookkeeping_second_page_wait. */
void test_mode_bookkeeping_second_page_start(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1EBB9u;
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_bookkeeping_draw_histogram();
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_bookkeeping_second_page_wait_ADDRESS;
	test_mode_bookkeeping_second_page_wait((uint16_t)r0);
}

/* Blinks the prompt until TEST, then back to the menu. */
void test_mode_bookkeeping_second_page_wait(uint16_t r0)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(r0);
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
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	current_object = SOUND_701F00;
	r10 = TEXT_CELL(26, 5);
	r0 = mem_read16(SOUND_701F06);
	r4 = r0;
	r1 = 0x64u;
	r2 = 5u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF334u;
	r0 = mem_read16(current_object + 0xAu);
	r4 += r0;
	r1 = 0x64u;
	r2 = 5u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF3B2u;
	r0 = r4;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 6u;
	r3 = 0xFFFFFFFEu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF4B4u;
	r0 = mem_read16(current_object + 2u);
	r4 = r0;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF534u;
	r0 = mem_read16(current_object + 4u);
	r4 += r0;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF5B2u;
	r0 = r4;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 6u;
	r3 = 0xFFFFFFFEu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF6B4u;
	r0 = mem_read16(current_object + 0x14u);
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF734u;
	r0 = mem_read16(current_object + 0x16u);
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	test_mode_draw_count(r0, r2, r3, &r1, &r10);
	r11 = TEXT_CELL(20, 16);
	r0 = mem_read32(current_object + 0x10u);
	r11 = test_mode_draw_duration(r0, r11);
	r11 = (r11 & ~0xFFFFu) | 0xF8A8u;
	r0 = mem_read32(current_object + 0x18u);
	r11 = test_mode_draw_duration(r0, r11);
	r11 = (r11 & ~0xFFFFu) | 0xF928u;
	r0 = mem_read32(current_object + 0x1Cu);
	r11 = test_mode_draw_duration(r0, r11);
	r0 = mem_read16(current_object + 0x14u);
	t1 = mem_read16(current_object + 0x16u) + r0;
	r1 = t1;
	if (t1 == 0u)
		return r0;
	r11 = (r11 & ~0xFFFFu) | 0xFA34u;
	r0 = divide_u32(mem_read32(current_object + 0x18u) + mem_read32(current_object + 0x1Cu), r1);
	test_mode_draw_minutes_seconds(&r0, &r11);
	r11 = (r11 & ~0xFFFFu) | 0xFAB4u;
	r0 = mem_read16(current_object + 0x20u);
	test_mode_draw_minutes_seconds(&r0, &r11);
	r11 = (r11 & ~0xFFFFu) | 0xFB34u;
	r0 = mem_read16(current_object + 0x22u);
	test_mode_draw_minutes_seconds(&r0, &r11);
	return r0;
}

/* Draws the second bookkeeping page's numbers: the TIME HISTOGRAM, how far the games got (seven counts
   from 701F24: PRE-STAGE, STAGE 1-5, COMPLETE), each with a bar of up to 20 blocks scaled to the
   largest; and OutRunner of OutRunners, the record of each of the ten goals (AUSTRALIA to KENYA; six
   bytes each from 701F32: the car, none when negative, shown by its colour, the three letters of the
   name and the time in hundredths, shown as minutes, seconds and hundredths). */
uint32_t test_mode_bookkeeping_draw_histogram(void)
{
	uint32_t carry = 0;
	uint16_t r0_2 = 0, r1 = 0, r3 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint32_t t1 = 0;

	current_object = SOUND_701F00;
	r10 = SOUND_RAM + 0x1F24u;
	r0 = 0u;
	r9 = 7u;
	do
	{
		r1 = mem_read16(r10);
		r10 += 2u;
		if (r0 < r1)
			r0 = r1;
		r9--;
	} while (r9 != 0u);
	r4 = 0u;
	if (r0 != 0u)
		r4 = divide_u32(0x50000000u, r0);
	r11 = current_object + 0x24u;
	r12 = TEXT_CELL(13, 5);
	r9 = 7u;
	do
	{
		r0 = mem_read16(r11);
		r5 = r0;
		r10 = r12;
		r1_2 = 0x64u;
		r2 = 5u;
		r11 += 2u;
		test_mode_draw_number(r2, &r0, &r1_2, &r10);
		r5 = r5 * r4;
		carry = (r5 >> 25) & 1u;
		t1 = (r5 >> 26) + carry;
		r5 = t1;
		if (t1 != 0u)
		{
			r10 = r12 + 0xAu;
			do
			{
				mem_write16(r10, 0x6403u);
				r10 += 2u;
				r5--;
			} while (r5 != 0u);
		}
		r9--;
		r12 = r12 + 0x80u;
	} while (r9 != 0u);
	r10 = TEXT_CELL(19, 14);
	r11 = current_object + 0x32u;
	r9 = 0xAu;
	do
	{
		r0 = mem_read8(r11);
		r11++;
		if ((int8_t)r0 < 0)
			r11 += 5u;
		else
		{
			r12 = r10;
			r13 = 0x1ED94u + r0 * 3u * 2u;
			r0_2 = (uint16_t)(0x6400u | ROM8(r13));
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13 + 1u));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | mem_read8(r11));
			r12 = r12 + 4u;
			mem_write16(r12, r0_2);
			r11++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | mem_read8(r11));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r11++;
			r12 = r12 + 2u;
			mem_write16(r12, (r0_2 & ~0xFFu) | mem_read8(r11));
			r11++;
			r0 = mem_read16(r11);
			r3 = 0x6430u;
			r13 = 0x1E970u;
			r11 += 2u;
			r12 = r12 + 4u;
			do
			{
				r1_2 = 0u;
				r2 = ROM32(r13);
				r13 += 4u;
				divux_pair(r2, &r0, &r1_2);
				mem_write16(r12, r0 + r3);
				r12 = r12 + ROM32(r13);
				r0 = r1_2;
				r13 += 4u;
			} while ((int32_t)ROM32(r13) >= 0);
		}
		r9--;
		r10 += 0x80u;
	} while (r9 != 0u);
	return r0;
}

/* Draws a time of R0 seconds at text cell R11 as days (5 digits), hours, minutes and seconds (2 digits
   each, a cell apart, for the D H M S of the text) and returns the cell after it. */
uint32_t test_mode_draw_duration(uint32_t r0, uint32_t r11)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	r1 = 0u;
	divux_pair(0x15180u, &r0, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 5u;
	r10 = r11;
	r11 += 0xCu;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r0_2 = stack_4;
	r1 = 0u;
	divux_pair(0xE10u, &r0_2, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	r11 += 6u;
	test_mode_draw_number(r2, &r0_2, &r1, &r10);
	r0_2 = stack_4;
	test_mode_draw_minutes_seconds(&r0_2, &r11);
	return r11;
}

/* Draws a time of R0 seconds at text cell R11 as minutes and seconds, 2 digits each a cell apart; R11
   moves to the seconds. */
void test_mode_draw_minutes_seconds(uint32_t *p_r0, uint32_t *p_r11)
{
	uint32_t r11 = *p_r11;
	uint32_t r1 = 0, r2 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	r1 = 0u;
	divux_pair(0x3Cu, p_r0, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	r11 += 6u;
	test_mode_draw_number(r2, p_r0, &r1, &r10);
	*p_r0 = stack_4;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	test_mode_draw_number(r2, p_r0, &r1, &r10);
	*p_r11 = r11;
}

/* Unused: turns a duration kept as five bytes at R10 (seconds, minutes and hours a byte each, then days
   a halfword) into seconds: ((days * 24 + hours) * 60 + minutes) * 60 + seconds, in R0 (R1 the carry). */
uint32_t test_mode_duration_to_seconds(uint32_t r10)
{
	uint32_t r0 = 0;

	r10 = r10 + 5u - 2u;
	r0 = mem_read16(r10) * 0x18u;
	r10--;
	r0 = (r0 + mem_read8(r10)) * 0x3Cu;
	r10--;
	r0 = (r0 + mem_read8(r10)) * 0x3Cu + mem_read8(r10 - 1u);
	return r0;
}

/* Unused: writes a time of R0 seconds at R10 as two bytes: the seconds (R0 mod 60), then the minutes (R0
   / 60). */
void test_mode_split_minutes_seconds(uint32_t r0, uint32_t r10)
{
	uint32_t r1 = 0;

	r1 = 0u;
	divux_pair(0x3Cu, &r0, &r1);
	mem_write8(r10, r1);
	mem_write8(r10 + 1u, r0);
}

/* Opens the BACKUP DATA CLEAR page: YES (CLEAR) and NO (CANCEL), the instructions, the cursor on NO;
   goes on in test_mode_backup_data_clear_run. */
void test_mode_backup_data_clear_start(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1EE79u;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 1u;
	r10 = 0x310000u | rom_01EEB0;
	test_mode_set_cursor(r0, r10);
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
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor ^ 1u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01EEAE[r0];
		test_mode_set_cursor(r0, r10);
	}
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		t1 = test_mode_blink_timer - 1u;
		test_mode_blink_timer = t1;
		if ((int8_t)t1 < 0)
		{
			test_mode_blink_timer = 5;
			t2 = test_mode_blink_address;
			t3 = mem_read16(t2) ^ 0x6A7Fu;
			mem_write16(t2, t3);
			if (t3 != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	if (test_mode_page_cursor == 0u)
	{
		mem_write16(test_mode_blink_address, 0x6A7Fu);
		r0 = bookkeeping_reset(r0);
		r10 = 0x1EEB2u;
		test_mode_draw_texts(r0, r10);
		r10 = 0x310000u | rom_01F282;
		r0 = 0x6Eu;
		test_mode_colour_line(r0, r10);
		test_mode_show_exit_prompt();
		test_mode_routine = test_mode_backup_data_clear_wait_ADDRESS;
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
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
	uint32_t r0 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = AS32(test_mode_flags);
	AS32(test_mode_flags) = t1 & ~2u;
	if ((t1 & 2u) != 0u)
	{
		save_settings();
		if (eeprom_write_count < 0x2710u)
		{
			test_mode_on = 0u;
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			return;
		}
		r0 = test_mode_clear_text();
		r10 = 0x1EF12u;
		test_mode_draw_texts(r0, r10);
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
	uint16_t r0_2 = 0, r1 = 0, r2 = 0, r26 = 0;
	uint32_t r0 = 0, r1_2 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;

	r26 = 0x733u;
	set_halfwords(PALETTE_RAM + 0x8000u, 0x4000u, r26);
	set_halfwords(0x688000u, 0x4000u, r26);
	r0 = rom_01F302;
	r0 = r0 >> 1;
	move_halfwords(PALETTE_RAM + 0x9E00u, 0x1F304u, r0);
	r0 = rom_01F504;
	r0 = r0 >> 1;
	move_string(0x1F506u, r0, 0x689E00u, r0, 2u, 0u, 0u, 0u, &r27, &r28);
	r27 = r27 & ~0x8000u;
	r0_2 = 0u;
	r9 = 0x20u;
	r27 = r27 + 2u;
	do
	{
		r10 = r27;
		r11 = r10 & ~0x80000u;
		r1 = (uint16_t)r0_2;
		mem_write16(r11, r1);
		mem_write16(r10, r1);
		r2 = (uint16_t)r1;
		r1 = (uint16_t)((uint32_t)r1 << 5);
		r11 += 2u;
		mem_write16(r11, r1);
		r10 += 2u;
		mem_write16(r10, r1);
		r2 += r1;
		r1 = (uint16_t)((uint32_t)r1 << 5);
		r11 += 2u;
		mem_write16(r11, r1);
		r10 += 2u;
		mem_write16(r10, r1);
		r2 += r1;
		mem_write16(r11 + 2u, r2);
		mem_write16(r10 + 2u, r2);
		r0_2++;
		r9--;
		r27 = r27 + 0x20u;
	} while (r9 != 0u);
	r10 = VIDEO_RAM + 0x18020u;
	r0 = 0u;
	r1_2 = 0x11111111u;
	r9 = 4u;
	do
	{
		r0 += r1_2;
		r8 = 8u;
		do
		{
			mem_write32(r10, r0);
			r10 += 4u;
			r8--;
		} while (r8 != 0u);
		r9--;
	} while (r9 != 0u);
	return r0;
}

/* Draws SELECT BY SERVICE BUTTON AND PUSH TEST BUTTON at the bottom of the screen. */
void test_mode_draw_instructions(uint32_t r0)
{
	uint32_t r10 = 0;

	r10 = rom_01F282_ADDRESS;
	r0 = test_mode_draw_texts(r0, r10);
}

/* Makes PUSH TEST BUTTON TO EXIT (the text record at 01F2BC, row 25) the prompt test_mode_blink_prompt
   blinks (test_mode_blink_address), from the next frame. */
void test_mode_show_exit_prompt(void)
{
	test_mode_blink_address = 0x1F2BCu;
	test_mode_blink_timer = 0;
}

/* Makes PUSH TEST BUTTON TO CONTINUE (the text record at 01F2DE) the prompt test_mode_blink_prompt
   blinks, from the next frame. */
void test_mode_show_continue_prompt(void)
{
	test_mode_blink_address = 0x1F2DEu;
	test_mode_blink_timer = 0;
}

/* Blinks the prompt whose text record test_mode_blink_address holds: when test_mode_blink_timer runs
   out, draws it (shown 21 frames) if its first cell is empty, else erases it (hidden 11 frames). */
void test_mode_blink_prompt(uint32_t r0)
{
	uint32_t r10 = 0, r11 = 0;
	uint8_t t1 = 0;

	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 >= 0)
		return;
	r10 = test_mode_blink_address;
	r11 = 0x310000u | mem_read16(r10);
	if (mem_read16(r11) == 0u)
	{
		test_mode_draw_texts(r0, r10);
		test_mode_blink_timer = 0x14;
		return;
	}
	do
	{
		mem_write16(r11, 0u);
		r11 += 2u;
	} while (mem_read16(r11) != 0u);
	test_mode_blink_timer = 0xA;
}

/* Puts the cursor at text cell R10, the cell two before a menu line: test_mode_blink_address = R10 (the
   cursor, character 7F, blinks there: the menu loops' tail at 01F0AD), test_mode_blink_timer 0, and the
   line in white (6A, test_mode_colour_line). */
uint32_t test_mode_set_cursor(uint32_t r0, uint32_t r10)
{
	uint32_t r0_2 = 0;

	test_mode_blink_address = r10;
	test_mode_blink_timer = 0;
	r10 += 4u;
	r0_2 = (r0 & ~0xFFu) | 0x6Au;
	return test_mode_colour_line(r0_2, r10);
}

/* Takes the cursor off its line: clears its cell (test_mode_blink_address) and puts the line back in
   grey (60). */
void test_mode_clear_cursor(void)
{
	uint32_t r0 = 0, r10 = 0;

	r10 = test_mode_blink_address;
	mem_write16(r10, 0u);
	r10 += 4u;
	r0 = 0x60u;
	r0 = test_mode_colour_line(r0, r10);
}

/* Draws the text record at R10 on the text layer: lines of a cell (halfword), characters and colour
   changes (a negative byte: its complement is the colour, the cells' high byte), each closed by 0; a 0
   cell ends it. */
uint32_t test_mode_draw_texts(uint32_t r0, uint32_t r10)
{
	uint32_t r11 = 0;

	r11 = TEXT_CELL(0, 0);
	for (;;)
	{
		r11 = (r11 & ~0xFFFFu) | mem_read16(r10);
		r10 += 2u;
		if ((uint16_t)r11 == 0u)
			break;
		for (;;)
		{
			r0 = (r0 & ~0xFFu) | mem_read8(r10);
			r10++;
			if ((uint8_t)r0 == 0u)
				break;
			if ((int8_t)r0 >= 0)
			{
				mem_write16(r11, r0);
				r11 += 2u;
				continue;
			}
			r0 = (r0 & ~0xFFu) | (uint8_t)~r0;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 8);
		}
	}
	return r0;
}

/* Draws the number R0 as test_mode_draw_number does, and R3 cells from R10 an S (or a blank for 1 or
   less) for the plural of the word beside it (COIN(S), CREDIT(S), GAME(S)); R1 holds the S's colour (the
   word's) in its low byte and the digits' in its second. */
void test_mode_draw_count(uint32_t r0, uint32_t r2, uint32_t r3, uint32_t *p_r1, uint32_t *p_r10)
{
	uint32_t r1 = *p_r1;

	r1 = r1 << 8;
	if (r0 <= 1u)
	{
		mem_write16(*p_r10 + r3 * 2u, r1);
		r1 = byte_swap32(r1 << 8);
		test_mode_draw_number(r2, &r0, &r1, p_r10);
		*p_r1 = r1;
		return;
	}
	r1 |= 0x53u;
	mem_write16(*p_r10 + r3 * 2u, r1);
	r1 = byte_swap32(r1 << 8);
	test_mode_draw_number(r2, &r0, &r1, p_r10);
	*p_r1 = r1;
}

/* Draws the number R0 in decimal in R2 cells ending at text cell R10 + 2 * R2, right-aligned in the
   colour in R1's low byte (the cells' high byte), the unused cells before it cleared. */
void test_mode_draw_number(uint32_t r2, uint32_t *p_r0, uint32_t *p_r1, uint32_t *p_r10)
{
	uint8_t r1 = (uint8_t)*p_r1;
	uint32_t r10 = *p_r10;
	uint16_t r3 = 0;
	uint32_t r1_2 = 0;

	r10 = r10 + r2 + r2;
	r3 = (uint16_t)((uint16_t)((uint32_t)(uint8_t)r1 << 8) | 0x30u);
	for (;;)
	{
		r1_2 = 0u;
		divux_pair(0xAu, p_r0, &r1_2);
		r1_2 = (r1_2 & ~0xFFFFu) | (uint16_t)(r1_2 + r3);
		r10 -= 2u;
		mem_write16(r10, r1_2);
		if (*p_r0 == 0u)
			break;
		r2--;
		if (r2 == 0u)
		{
			*p_r1 = r1_2;
			*p_r10 = r10;
			return;
		}
	}
	for (;;)
	{
		r2--;
		if (r2 == 0u)
			break;
		r10 -= 2u;
		mem_write16(r10, 0u);
	}
	*p_r1 = r1_2;
	*p_r10 = r10;
}

/* test_mode_draw_number in hexadecimal (digits 0-9, A-F). */
uint32_t test_mode_draw_hex_number(uint32_t r0, uint8_t r1, uint32_t r2, uint32_t r10)
{
	uint16_t r3 = 0;
	uint32_t r1_2 = 0;

	r10 = r10 + r2 + r2;
	r3 = (uint16_t)((uint16_t)((uint32_t)(uint8_t)r1 << 8) | 0x30u);
	for (;;)
	{
		r1_2 = 0u;
		divux_pair(0x10u, &r0, &r1_2);
		r1_2 = (uint16_t)(r1_2 + r3);
		if ((int8_t)r1_2 > 0x39)
			r1_2 = (r1_2 & ~0xFFu) | (uint8_t)(r1_2 + 7u);
		r10 -= 2u;
		mem_write16(r10, r1_2);
		if (r0 == 0u)
			break;
		r2--;
		if (r2 == 0u)
			return r10;
	}
	for (;;)
	{
		r2--;
		if (r2 == 0u)
			break;
		r10 -= 2u;
		mem_write16(r10, 0u);
	}
	return r10;
}

/* Gives the text from cell R10 up to the next empty cell the colour R0 (60 grey, 6A white, 6E
   hidden...). */
uint32_t test_mode_colour_line(uint32_t r0, uint32_t r10)
{
	uint16_t t1 = 0;

	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 8);
	for (;;)
	{
		t1 = mem_read16(r10) & ~0xE00u;
		mem_write16(r10, t1);
		if (t1 == 0u)
			break;
		mem_write16(r10, mem_read16(r10) | r0);
		r10 += 2u;
	}
	return r0;
}

/* Clears the text layer (30 rows of 64 cells). */
uint32_t test_mode_clear_text(void)
{
	uint16_t r26 = 0;
	uint32_t r0 = 0;

	r0 = 0x780u;
	r26 = 0u;
	move_string(screen, 0u, TEXT_CELL(0, 0), 0x780u, 2u, 1u, 0u, r26, &r27, &r28);
	return r0;
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
	uint8_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint32_t r0_2 = 0, r2_2 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint32_t t5 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(0xC80008u);
	t3 = mem_read8(IO_C0000A);
	t4 = mem_read8(0xC8000Au);
	r2 = (uint8_t)(~t3 | ~t4);
	r0 = ((~t1 | ~t2) & 0x33u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 3u);
	r1 = test_mode_buttons;
	test_mode_buttons = r0;
	test_mode_buttons_previous = r1;
	test_mode_buttons_pressed = ~r1 & r0;
	test_mode_buttons_released = r1 & ~r0;
	t5 = AS32(test_mode_flags);
	carry = (t5 & 4u) != 0u;
	AS32(test_mode_flags) = t5 ^ 4u;
	r0_2 = carry;
	r10 = IO_PORTS + 0x50u + r0_2 * 4u;
	r11 = test_mode_analog_inputs_ADDRESS + r0_2 * 4u;
	r0_2 = (uint8_t)(0u - r0_2);
	r0_2 = (uint8_t)(r0_2 + 2u);
	r0_2 = (uint8_t)(r0_2 * 2u);
	do
	{
		r1 = 0u;
		mem_write8(r10, 0xFFu);
		r2_2 = 8u;
		do
		{
			r3 = mem_read8(r10);
			carry = (uint8_t)(r3 * 2u) < r3;
			r1 = (uint8_t)(r1 * 2u + carry);
			r2_2--;
		} while (r2_2 != 0u);
		mem_write8(r11, r1);
		r0_2--;
		r11++;
		r10 += 2u;
	} while (r0_2 != 0u);
	carry = (test_mode_flags & 4u) != 0u;
	mem_write8(IO_C00060, carry);
}

/* Sends the test mode's next queued sound request (test_mode_sound_queue, test_mode_sound_queue_read) to
   the sound CPU, one a frame: to screen 2's command byte (SOUND_COMMAND_2) when the request has its high
   byte set, else to screen 1's (SOUND_COMMAND_1). */
void test_mode_send_sound(void)
{
	uint8_t r1 = 0;
	uint32_t r0 = 0;
	uint16_t t1 = 0;

	r0 = test_mode_sound_queue_read;
	if (test_mode_sound_queue_write == r0)
		return;
	t1 = ~test_mode_sound_queue[r0];
	r1 = (uint8_t)t1;
	if ((int16_t)t1 >= 0)
		mem_write8(SOUND_COMMAND_2, ~r1);
	else
		mem_write8(SOUND_COMMAND_1, ~r1);
	test_mode_sound_queue_read = (r0 + 1u) & 0xFu;
}

/* test_mode_queue_sound for screen 2's sound CPU commands (the high byte set). */
void test_mode_queue_sound_screen_2(uint16_t r0)
{
	r0 |= 0xFF00u;
	test_mode_queue_sound(r0);
}

/* Puts sound request R0 in the test mode's ring of 16 (test_mode_sound_queue at
   test_mode_sound_queue_write) for test_mode_send_sound: screen 1's, or screen 2's with the high byte
   set (test_mode_queue_sound_screen_2). */
void test_mode_queue_sound(uint16_t r0)
{
	uint32_t r1 = 0;

	r1 = test_mode_sound_queue_write;
	test_mode_sound_queue[r1] = r0;
	test_mode_sound_queue_write = (r1 + 1u) & 0xFu;
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
	uint32_t r0 = 0, r6 = 0;
	uint32_t saved_r28 = 0, saved_r27 = 0, saved_screen = 0, saved_r12 = 0, saved_screen_2 = 0;

	saved_r28 = r28;
	saved_r27 = r27;
	saved_screen = screen;
	saved_r12 = r12;
	vblank_counter++;
	vblanks_since_start++;
	mem_write16(IRQ_D00008, rom_03F82A);
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
		r0 = tile_banks[0];
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | tile_banks[1]);
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | tile_banks[2]);
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | tile_banks[3]);
		mem_write8(IO_C0000E, (r0 << 6) | (r0 >> 2));
	}
	finish_sprite_list();
	send_sound_requests();
	if (mem_read8(screen + 0xA7u) == 0u)
		vblank_update_scrolls();
	mem_write8(screen + 0xA7u, 0u);
	if (test_mode_on == 0u)
	{
		r6 = read_switches_and_analog();
		switch (mem_read8(screen))
		{
		case 0:
			pack_seat_inputs();
			break;
		case 1:
			r0 = queue_seat_inputs();
			break;
		default:
			invalid_jump(rom_03FAC4[mem_read8(screen)]);
		}
		check_test_button(r0);
		if (free_play == 0u)
			update_coins(r6);
	}
	update_lamps();
	lcd_refresh();
	count_total_time();
	frame_ready = 0xFFu;
	if (single_task == 0u && ((test_mode_on != 0u && mem_read8(screen) == 0u) || task_swapping != 0u))
		swap_tasks();
	r0 = rom_03FACC[mem_read8(screen)];
	mem_write8(r0, mem_read8(r0) + 1u);
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
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;

	r0 = SCREEN(screen)->index * 2u;
	mem_write32(VIDEO_RAM + 0x1FF10u + r0 * 4u, RAM32(video_registers_ADDRESS + 0x10u + r0 * 4u));
	mem_write32(VIDEO_RAM + 0x1FF14u + r0 * 4u, RAM32(video_registers_ADDRESS + 0x14u + r0 * 4u));
	r1 = 0u - r0 + 2u;
	r2 = RAM32(video_registers_ADDRESS + 0x10u + r1 * 4u);
	r3 = scroll_targets[r1];
	RAM32(video_registers_ADDRESS + 0x10u + r1 * 4u) = r3;
	r3 = r3 << 6;
	mem_write32(VIDEO_RAM + 0x1FF10u + r1 * 4u, (((int32_t)((r2 << 6) - r3) >> 1) + r3) >> 6);
	r2 = RAM32(video_registers_ADDRESS + 0x14u + r1 * 4u);
	r3 = scroll_targets[1 + r1];
	RAM32(video_registers_ADDRESS + 0x14u + r1 * 4u) = r3;
	r3 = r3 << 7;
	mem_write32(VIDEO_RAM + 0x1FF14u + r1 * 4u, (((int32_t)((r2 << 7) - r3) >> 1) + r3) >> 7);
	mem_write32(VIDEO_RAM + 0x1FF20u + r0 * 4u, RAM32(video_registers_ADDRESS + 0x20u + r0 * 4u));
	mem_write32(VIDEO_RAM + 0x1FF24u + r0 * 4u, RAM32(video_registers_ADDRESS + 0x24u + r0 * 4u));
	r2 = RAM32(video_registers_ADDRESS + 0x20u + r1 * 4u);
	r3 = scroll_targets[4 + r1];
	RAM32(video_registers_ADDRESS + 0x20u + r1 * 4u) = r3;
	r3 = r3 << 6;
	mem_write32(VIDEO_RAM + 0x1FF20u + r1 * 4u, (((int32_t)((r2 << 6) - r3) >> 1) + r3) >> 6);
	r2 = RAM32(video_registers_ADDRESS + 0x24u + r1 * 4u);
	r3 = scroll_targets[5 + r1];
	RAM32(video_registers_ADDRESS + 0x24u + r1 * 4u) = r3;
	r3 = r3 << 7;
	mem_write32(VIDEO_RAM + 0x1FF24u + r1 * 4u, (((int32_t)((r2 << 7) - r3) >> 1) + r3) >> 7);
	r0_2 = (uint8_t)(video_registers[2] >> 8u);
	mem_write8(VIDEO_31FF05, r0_2);
	AT8(video_registers[2], 1) = (r0_2 + 4u) & 0xECu;
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
	uint8_t t1 = 0;

	t1 = timer_interrupts_left;
	timer_interrupts_left = t1 - 1u;
	if (t1 != 1u)
		mem_write16(IRQ_D00008, rom_03F82C);
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
	uint16_t r1 = 0;
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (isdn_mode == 0u)
	{
		mem_write8(IRQ_D00007, 0xEFu);
		return;
	}
	link_wait_count++;
	r1 = 1u;
	t1 = mem_read8(0x800010u);
	r0 = t1;
	mem_write8(0x800010u, 0u);
	if (r0 < 4u)
	{
		switch (r0)
		{
		case 3:
			link_isdn_receive_records();
			r1 = 0x168u;
			link_wait_count = 0u;
			break;
		case 0:
		case 1:
		case 2:
			break;
		default:
			invalid_jump(rom_03F85C[r0]);
		}
	}
	mem_write16(IRQ_D0000A, r1);
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
void check_test_button(uint32_t r0)
{
	if ((((r0 & ~0xFFu) | switches_pressed) & 0x80u) != 0u)
		test_mode_on = 0xFFu;
}

/* In the vertical blank, sends the screen's next sound request to the sound CPU: one from its ring of 32
   (queue_sound's: sound_queue_read, sound_queue_write, the requests after them) to its command byte
   (SOUND_COMMAND_1 for screen 1, SOUND_COMMAND_2 for screen 2), and one from its second ring at 840
   (second_sound_queue_read, second_sound_queue_write; only the unused request_second_sound fills it) to
   the byte 8 after it, which the sound program never reads. */
void send_sound_requests(void)
{
	uint32_t r0 = 0, r10 = 0, second_sound_queue_read = 0, sound_queue_read = 0;
	uint8_t sound_queue_write = 0, second_sound_queue_write = 0;

	r10 = (uint32_t)SCREEN(screen)->index << 8;
	r0 = screen + 0x800u;
	sound_queue_read = SCREEN(r0 - 0x800u)->sound_queue_read;
	r0++;
	sound_queue_write = SCREEN(r0 - 0x801u)->sound_queue_write;
	r0++;
	if (sound_queue_read != sound_queue_write)
	{
		mem_write8(r10 + SOUND_COMMAND_1, mem_read8(r0 + sound_queue_read));
		SCREEN(r0 - 0x802u)->sound_queue_read = (sound_queue_read + 1u) & 0x1Fu;
	}
	r0 = screen + 0x840u;
	second_sound_queue_read = SCREEN(r0 - 0x840u)->second_sound_queue_read;
	r0++;
	second_sound_queue_write = SCREEN(r0 - 0x841u)->second_sound_queue_write;
	r0++;
	if (second_sound_queue_read != second_sound_queue_write)
	{
		mem_write8(r10 + SOUND_SECOND_COMMAND_1, mem_read8(r0 + second_sound_queue_read));
		SCREEN(r0 - 0x842u)->second_sound_queue_read = (second_sound_queue_read + 1u) & 0x1Fu;
	}
}

/* Counts the bookkeeping's TOTAL TIME (701F10, seconds, kept with its copy and checksum), outside the
   test mode only: total_time_count gains 17 every vertical blank and a second passes at each 1000 (2000
   for every 73rd second). */
void count_total_time(void)
{
	uint32_t r0 = 0, r7 = 0;

	if (test_mode_on != 0u)
		return;
	r0 = (uint16_t)(total_time_count + 0x11u);
	if (r0 < 0x3E8u)
	{
		total_time_count = r0;
		return;
	}
	r7 = SOUND_701F10;
	if (mem_read32(SOUND_701F10) % 0x49u != 0u)
	{
		total_time_count = r0 - 0x3E8u;
		if (mem_read32(r7) != 0xFFFFFFFFu)
		{
			mem_write32(r7, mem_read32(r7) + 1u);
			bookkeeping_update_checksum(r0);
			mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + 1u);
		}
		return;
	}
	if (r0 < 0x7D0u)
	{
		total_time_count = r0;
		return;
	}
	r0 = (uint16_t)(r0 - 0x3E8u);
	total_time_count = r0 - 0x3E8u;
	if (mem_read32(r7) != 0xFFFFFFFFu)
	{
		mem_write32(r7, mem_read32(r7) + 1u);
		bookkeeping_update_checksum(r0);
		mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + 1u);
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
	uint8_t r0 = 0;
	uint32_t r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (checkpoint_lamp_blinks != 0u)
	{
		t1 = checkpoint_lamp_timer;
		checkpoint_lamp_timer = t1 - 1u;
		if (t1 == 1u)
		{
			checkpoint_lamp_timer = checkpoint_lamp_period;
			checkpoint_lamp[0] ^= 2u;
			checkpoint_lamp_blinks--;
		}
	}
	if (leader_lamp_blinks != 0u)
	{
		t2 = leader_lamp_timer;
		leader_lamp_timer = t2 - 1u;
		if (t2 == 1u)
		{
			leader_lamp_timer = leader_lamp_period;
			leader_lamp[0] ^= 8u;
			leader_lamp_blinks--;
		}
	}
	r0 = port_d_outputs[0] | checkpoint_lamp[0] | leader_lamp[0];
	if ((SCREEN(SCREEN_1)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		r0 |= wheel_motor[0];
	mem_write8(IO_C00006, r0);
	if (checkpoint_lamp_2_blinks != 0u)
	{
		t3 = checkpoint_lamp_2_timer;
		checkpoint_lamp_2_timer = t3 - 1u;
		if (t3 == 1u)
		{
			checkpoint_lamp_2_timer = checkpoint_lamp_2_period;
			checkpoint_lamp_2 ^= 2u;
			checkpoint_lamp_2_blinks--;
		}
	}
	if (leader_lamp_2_blinks != 0u)
	{
		t4 = leader_lamp_2_timer;
		leader_lamp_2_timer = t4 - 1u;
		if (t4 == 1u)
		{
			leader_lamp_2_timer = leader_lamp_2_period;
			leader_lamp_2 ^= 8u;
			leader_lamp_2_blinks--;
		}
	}
	r0 = port_d_outputs[1] | checkpoint_lamp_2 | leader_lamp_2;
	if ((SCREEN(SCREEN_2)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		r0 |= wheel_motor[1];
	mem_write8(0xC80006u, r0);
	r10 = SCREEN_1;
	r11 = port_g_outputs_ADDRESS;
	update_button_lamps(r10, r11);
	r10 = SCREEN_2;
	r11 = port_g_outputs_ADDRESS + 0x1u;
	update_button_lamps(r10, r11);
}

/* A screen's two button lamps, in its port G image (R11: port_g_outputs or port_g_outputs[1], which the
   LCD's output sends): the DJ/MUSIC lamp (bit 0) by the low byte of button_lamps and the << >> lamp (bit
   1) by its high byte: 0 off, negative on, n blinking every n vertical blanks (music_lamp_timer,
   arrows_lamp_timer), each mode taken when it changes (music_lamp_mode, arrows_lamp_mode). */
void update_button_lamps(uint32_t r10, uint32_t r11)
{
	uint8_t r0 = 0, r1 = 0, button_lamps = 0;
	uint8_t music_lamp_timer = 0, arrows_lamp_timer = 0;

	r0 = mem_read8(r11);
	button_lamps = (uint8_t)SCREEN(r10)->button_lamps;
	if (button_lamps != SCREEN(r10)->music_lamp_mode)
	{
		SCREEN(r10)->music_lamp_mode = button_lamps;
		if ((int8_t)button_lamps > 0)
		{
			r0 ^= 1u;
			SCREEN(r10)->music_lamp_timer = button_lamps;
		}
		else if (button_lamps == 0u)
			r0 &= ~1u;
		else
			r0 |= 1u;
	}
	else if ((int8_t)button_lamps > 0)
	{
		music_lamp_timer = SCREEN(r10)->music_lamp_timer;
		SCREEN(r10)->music_lamp_timer = music_lamp_timer - 1u;
		if (music_lamp_timer == 1u)
		{
			r0 ^= 1u;
			SCREEN(r10)->music_lamp_timer = button_lamps;
		}
	}
	r1 = (uint8_t)(SCREEN(r10)->button_lamps >> 8u);
	if (r1 == SCREEN(r10)->arrows_lamp_mode)
	{
		if ((int8_t)r1 > 0)
		{
			arrows_lamp_timer = SCREEN(r10)->arrows_lamp_timer;
			SCREEN(r10)->arrows_lamp_timer = arrows_lamp_timer - 1u;
			if (arrows_lamp_timer == 1u)
			{
				r0 ^= 2u;
				SCREEN(r10)->arrows_lamp_timer = r1;
			}
		}
		mem_write8(r11, r0);
		return;
	}
	SCREEN(r10)->arrows_lamp_mode = r1;
	if ((int8_t)r1 > 0)
	{
		r0 ^= 2u;
		SCREEN(r10)->arrows_lamp_timer = r1;
		mem_write8(r11, r0);
		return;
	}
	if (r1 == 0u)
	{
		r0 &= ~2u;
		mem_write8(r11, r0);
		return;
	}
	r0 |= 2u;
	mem_write8(r11, r0);
}

/* The V70's reset vector, at the top of the program's address space: a jump to boot. */
void reset(void)
{
	boot();
}
