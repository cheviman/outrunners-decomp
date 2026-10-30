/**
 *    Name:        system/test_mode.c
 *    Description: the test mode: its menu and pages (memory, volume, input, output, sound, CRT, game, coins, network, bookkeeping, backup clear) and their drawing
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Enters the test mode (reset calls it once when test_mode_on says the test mode is on): turns the link
   board off, clears the tilemaps and the sprite lists (init_video, init_sprite_lists), clears both
   screens' pending tile unpacking (screen unpack_state), demo flag (demo_flags bit 8) and flags bit 0,
   the mixers' registers 40-4A, turns both screens on (the I/O chips' CNT), sets up the test mode's
   palettes and font (test_mode_set_up_colours), test_mode_flags 0 (INDIVIDUAL) and the cursor on EXIT,
   stops both monitors' sounds (command 1F through the sound command queue, test_mode_queue_sound and
   test_mode_queue_sound_screen_2), turns the lamps and the wheel motor off
   (checkpoint_lamp-wheel_motor[1], screen button_lamps, port_d_outputs, port_g_outputs), writes " LCD
   status OK! " on both cabinets' LCDs (the 16 bytes at F10 of each screen's block), sets
   test_mode_routine to test_mode_menu and reads the controls (test_mode_read_controls). */
uint32_t test_mode_start(void)
{
	uint16_t sound = 0;
	uint32_t lcd_place = 0, text_0_to = 0, text_12_to = 0, text_4_to = 0, text_8_to = 0;
	uint32_t port_d_outputs_0_halfword = 0, port_g_outputs_0_halfword = 0;

	io_write16(LINK_BOARD_ON, 0u);
	io_write8(EVENT_CODE, 0x1Fu);
	init_video();
	init_sprite_lists();
	game->screen_1.unpack_state = 0u;
	game->screen_1.unpack_read_place = 0u;
	game->screen_1.unpack_write_place = 0u;
	game->screen_2.unpack_state = 0u;
	game->screen_2.unpack_read_place = 0u;
	game->screen_2.unpack_write_place = 0u;
	game->screen_1.demo_flags &= ~0x100u;
	game->screen_2.demo_flags &= ~0x100u;
	game->screen_1.flags &= ~1u;
	game->screen_2.flags &= ~1u;
	io_write32(MIXER_1_OFFSET_0_RED, 0u);
	io_write32(MIXER_1_OFFSET_0_BLUE, 0u);
	io_write32(MIXER_1_OFFSET_1_GREEN, 0u);
	io_write32(MIXER_2_OFFSET_0_RED, 0u);
	io_write32(MIXER_2_OFFSET_0_BLUE, 0u);
	io_write32(MIXER_2_OFFSET_1_GREEN, 0u);
	io_write8(IO_1_CNT, 6u);
	io_write8(IO_2_CNT, 0x86u);
	test_mode_set_up_colours();
	game->test_mode_flags = 0u;
	game->test_mode_cursor = 0xCu;
	game->test_mode_sound_queue_write = 0u;
	game->test_mode_sound_queue_read = 0u;
	sound = 0x1Fu;
	test_mode_queue_sound(sound);
	test_mode_queue_sound_screen_2(sound);
	SOUND_ENGINE_REVS_1[0] = 0u;
	SOUND_ENGINE_REQUESTS_1[0] |= 0x20u;
	SOUND_ENGINE_REVS_2 = 0u;
	SOUND_ENGINE_REQUESTS_2 |= 0x20u;
	game->wheel_motor[0] = 0u;
	game->wheel_motor[1] = 0u;
	game->checkpoint_lamp[0] = 0u;
	game->checkpoint_lamp_blinks = 0u;
	game->checkpoint_lamp_period = 0u;
	game->checkpoint_lamp_timer = 0u;
	game->checkpoint_lamp_2 = 0u;
	game->checkpoint_lamp_2_blinks = 0u;
	game->checkpoint_lamp_2_period = 0u;
	game->checkpoint_lamp_2_timer = 0u;
	game->leader_lamp[0] = 0u;
	game->leader_lamp_blinks = 0u;
	game->leader_lamp_period = 0u;
	game->leader_lamp_timer = 0u;
	game->leader_lamp_2 = 0u;
	game->leader_lamp_2_blinks = 0u;
	game->leader_lamp_2_period = 0u;
	game->leader_lamp_2_timer = 0u;
	game->screen_1.button_lamps = 0u;
	game->screen_2.button_lamps = 0u;
	port_d_outputs_0_halfword = (game->port_d_outputs[0] | ((uint32_t)game->port_d_outputs[1] << 8u)) & 0xE0E1u;
	game->port_d_outputs[0] = (uint8_t)port_d_outputs_0_halfword;
	game->port_d_outputs[1] = (uint8_t)(port_d_outputs_0_halfword >> 8u);
	port_g_outputs_0_halfword = (game->port_g_outputs[0] | ((uint32_t)game->port_g_outputs[1] << 8u)) & ~0x403u;
	game->port_g_outputs[0] = (uint8_t)port_g_outputs_0_halfword;
	game->port_g_outputs[1] = (uint8_t)(port_g_outputs_0_halfword >> 8u);
	text_0_to = test_mode_lcd_text;
	text_4_to = test_mode_lcd_text_4_to_7;
	AS32(game->screen_1.lcd_text[0]) = text_0_to;
	AS32(game->screen_1.lcd_text[4]) = text_4_to;
	AS32(game->screen_2.lcd_text[0]) = text_0_to;
	AS32(game->screen_2.lcd_text[4]) = text_4_to;
	text_8_to = test_mode_lcd_text_8_to_11;
	text_12_to = test_mode_lcd_text_12_to_15;
	AS32(game->screen_1.lcd_text[8]) = text_8_to;
	AS32(game->screen_1.lcd_text[12]) = text_12_to;
	AS32(game->screen_2.lcd_text[8]) = text_8_to;
	AS32(game->screen_2.lcd_text[12]) = text_12_to;
	game->test_mode_routine = test_mode_menu_ADDRESS;
	lcd_place = 0x218F20u;
	test_mode_read_controls();
	return lcd_place;
}

/* The test mode's frame, which reset runs every video frame while the test mode is on: reads the buttons
   and the analog inputs (test_mode_read_controls), sends the sound board the next queued sound command
   (test_mode_send_sound) and runs test_mode_routine, the step of the menu or page being shown. */
void test_mode_frame(void)
{
	test_mode_read_controls();
	test_mode_send_sound();
	call_code(game->test_mode_routine);
}

/* Shows the test mode's menu, where every page returns: in CONTINUE mode (test_mode_flags bit 0) the
   cursor first moves on to the next item; clears the text layer, shows the E2PROM write count while
   SERVICE is held (test_mode_menu_show_write_count), draws TEST MODE, the heading (INDIVIDUAL or
   CONTINUE), the twelve items and SELECT BY SERVICE BUTTON AND PUSH TEST BUTTON, puts the cursor on
   test_mode_cursor (test_mode_set_cursor) and goes on to test_mode_menu_run. With the cursor on the
   heading (after test_mode_menu_toggle_continue has redrawn it) it goes straight on. */
void test_mode_menu(void)
{
	uint32_t carry = 0;
	uint16_t cell_value = 0;
	uint32_t cursor_cell = 0, heading = 0, text = 0;

	if (game->test_mode_cursor == 0u)
	{
		game->test_mode_routine = test_mode_menu_run_ADDRESS;
		test_mode_menu_run();
		return;
	}
	if ((game->test_mode_flags & 1u) != 0u)
		game->test_mode_cursor++;
	cell_value = test_mode_clear_text();
	cell_value = test_mode_menu_show_write_count(cell_value);
	text = test_mode_menu_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	carry = (game->test_mode_flags & 1u) != 0u;
	heading = carry;
	text = test_mode_menu_headings[heading];
	heading = test_mode_draw_texts((uint16_t)heading, text);
	test_mode_draw_instructions(heading);
	cursor_cell = 0x310000u | test_mode_menu_cursor_cells[game->test_mode_cursor];
	test_mode_set_cursor(heading, cursor_cell);
	game->test_mode_routine = test_mode_menu_run_ADDRESS;
	test_mode_menu_run();
}

/* The menu, each frame: SERVICE moves the cursor (test_mode_cursor) down, from EXIT back to the heading;
   TEST starts the item's page (test_mode_page_jumps: test_mode_menu_toggle_continue,
   test_mode_memory_test, test_mode_volume_adjustment, test_mode_input_test, test_mode_output_test,
   test_mode_sound_test, test_mode_crt_test, test_mode_game_assignments, test_mode_coin_assignments, then
   the network assignments, bookkeeping, backup data clear and exit pages,
   test_mode_network_assignments_start, test_mode_bookkeeping_start, test_mode_backup_data_clear_start,
   test_mode_exit); the cursor's arrow blinks (test_mode_blink_address, test_mode_blink_timer: 11 frames
   shown, 6 hidden). */
void test_mode_menu_run(void)
{
	uint32_t cursor = 0, cursor_cell = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		cursor = game->test_mode_cursor;
		cursor = (uint8_t)(cursor + 1u);
		if ((int8_t)cursor >= 0xD)
			cursor = 0u;
		game->test_mode_cursor = cursor;
		cursor_cell = 0x310000u | test_mode_menu_cursor_cells[cursor];
		test_mode_set_cursor(cursor, cursor_cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_page_jumps[game->test_mode_cursor];
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* While SERVICE is held as the menu is drawn: E2PROM WRITE COUNT and the count (eeprom_write_count, 5
   digits) at the bottom of the menu. */
uint16_t test_mode_menu_show_write_count(uint16_t cell_value)
{
	uint32_t attribute = 0, cell = 0, digits = 0, text = 0, write_count = 0;

	if ((game->test_mode_buttons & 1u) == 0u)
		return cell_value;
	text = test_mode_write_count_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	cell = TEXT_CELL(27, 26);
	write_count = game->eeprom_write_count;
	attribute = 0x64u;
	digits = 5u;
	test_mode_draw_number(digits, &write_count, &attribute, &cell);
	return write_count;
}

/* The menu's heading: TEST switches between INDIVIDUAL and CONTINUE (test_mode_flags bit 0) and redraws
   it, then back to test_mode_menu. In CONTINUE mode each return to the menu moves the cursor to the next
   item, so the pages run one after another with TEST alone. */
void test_mode_menu_toggle_continue(void)
{
	uint32_t carry = 0;
	uint32_t heading = 0, text = 0;
	uint32_t mode_flags = 0, test_mode_flags_long = 0;

	mode_flags = (game->test_mode_flags | ((uint32_t)game->test_mode_cursor << 8u) | ((uint32_t)game->test_mode_page_cursor << 16u) | ((uint32_t)(game->test_mode_routine & 0xFFu) << 24u));
	carry = (mode_flags & 1u) != 0u;
	test_mode_flags_long = mode_flags ^ 1u;
	game->test_mode_flags = (uint8_t)test_mode_flags_long;
	game->test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	game->test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	game->test_mode_routine = (game->test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	heading = 0u - (0xFFFFFFFFu + carry);
	text = test_mode_menu_headings[heading];
	test_mode_draw_texts((uint16_t)heading, text);
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* The MEMORY TEST, all at once with the interrupts off and the sound program held in its wait loop
   (sound RAM 1FFF = 1): each chip's result shows TEST while it is tested, then GOOD or BAD
   (memory_test_results, test_mode_memory_test_draw). The program ROM's two halves by
   test_mode_memory_test_checksum against the sums at 03FFF8-03FFFF (IC37, IC40: 000000-03FFF7; IC36,
   IC39: 100000-1FEFFF); the video RAM (IC60, IC58, IC57, IC54) and the sprite RAM (IC61, IC67, IC68,
   IC70) by 4-bit lanes (test_mode_memory_test_nibbles); the palette RAMs' four 16 KB blocks by byte
   (IC1, IC3, IC2, IC4, IC120, IC121, IC122, IC123; test_mode_memory_test_halfwords); the work RAM
   (IC115-IC118, test_mode_memory_test_words) and the sound RAM (IC21, test_mode_memory_test_bytes) each
   saved in the sprite RAM and put back. The tilemaps, sprite lists and palettes it overwrites are set up
   again (init_video, init_sprite_lists, test_mode_set_up_colours). IC70 always shows GOOD: the test of
   its lane has no branch to BAD. Ends with the blinking PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt) and test_mode_memory_test_run. */
void test_mode_memory_test(void)
{
	uint8_t blank_result = 0, chip_result = 0;
	uint32_t cell_value = 0, failures = 0, memory_start = 0, odd_check = 0, rom_check = 0;
	uint8_t video_lane_0_failures = 0, video_lane_1_failures = 0, sprite_lane_0_failures = 0, sprite_lane_1_failures = 0;
	uint16_t low_rom_check = 0, high_rom_check = 0, video_lane_2_failures = 0, sprite_lane_2_failures = 0, sprite_lane_3_failures = 0, work_byte_1_failures = 0;
	uint32_t work_byte_2_failures = 0, work_byte_3_failures = 0;

	interrupts_off();
	cell_value = test_mode_clear_text();
	SOUND_PROGRAM_HOLD = 1u;
	current_object = pointer_to(memory_test_results_ADDRESS);
	blank_result = 0u;
	set_bytes(memory_test_results_ADDRESS, 0x19u, blank_result);
	AS16(game->memory_test_results[0]) = 0x101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	memory_start = 0u;
	rom_check = LOW_ROM_TESTED_BYTES;
	test_mode_memory_test_checksum(memory_start, &rom_check, &odd_check);
	low_rom_check = rom_check;
	rom_check = (rom_check & ~0xFFu) | 2u;
	if (ic37_checksum != low_rom_check)
		rom_check = (rom_check & ~0xFFu) | 3u;
	game->memory_test_results[0] = rom_check;
	rom_check = (rom_check & ~0xFFu) | 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (ic40_checksum != odd_check)
		rom_check = (rom_check & ~0xFFu) | 3u;
	game->memory_test_results[1] = rom_check;
	current_object = pointer_to(address_of(current_object) + 1u);
	AS16(game->memory_test_results[2]) = 0x101u;
	test_mode_memory_test_draw(&rom_check, &r12);
	memory_start = 0x100000u;
	rom_check = HIGH_ROM_TESTED_BYTES;
	test_mode_memory_test_checksum(memory_start, &rom_check, &odd_check);
	high_rom_check = rom_check;
	rom_check = (rom_check & ~0xFFu) | 2u;
	if (ic36_checksum != high_rom_check)
		rom_check = (rom_check & ~0xFFu) | 3u;
	game->memory_test_results[2] = rom_check;
	rom_check = (rom_check & ~0xFFu) | 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (ic39_checksum != odd_check)
		rom_check = (rom_check & ~0xFFu) | 3u;
	game->memory_test_results[3] = rom_check;
	current_object = pointer_to(address_of(current_object) + 1u);
	AS32(game->memory_test_results[4]) = 0x1010101u;
	test_mode_memory_test_draw(&rom_check, &r12);
	memory_start = 0x300000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_nibbles(failures, memory_start);
	video_lane_0_failures = failures & 0xFu;
	chip_result = 2u;
	if (video_lane_0_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[4] = chip_result;
	video_lane_1_failures = failures & 0xF0u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (video_lane_1_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[5] = chip_result;
	video_lane_2_failures = failures & 0xF00u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (video_lane_2_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[6] = chip_result;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if ((failures & ~0xFFFu) != 0u)
		chip_result = 3u;
	game->memory_test_results[7] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	cell_value = init_video();
	AS32(game->memory_test_results[8]) = 0x1010101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	memory_start = 0x400000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_nibbles(failures, memory_start);
	sprite_lane_0_failures = failures & 0xFu;
	chip_result = 2u;
	if (sprite_lane_0_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[8] = chip_result;
	sprite_lane_1_failures = failures & 0xF0u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (sprite_lane_1_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[9] = chip_result;
	sprite_lane_2_failures = failures & 0xF00u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (sprite_lane_2_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[10] = chip_result;
	sprite_lane_3_failures = failures & ~0xFFFu;
	failures = sprite_lane_3_failures;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	game->memory_test_results[11] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	init_sprite_lists();
	AS16(game->memory_test_results[12]) = 0x101u;
	test_mode_memory_test_draw(&failures, &r12);
	memory_start = 0x600000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[12] = chip_result;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if ((failures & ~0xFFu) != 0u)
		chip_result = 3u;
	game->memory_test_results[13] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	cell_value = test_mode_set_up_colours();
	AS16(game->memory_test_results[14]) = 0x101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	memory_start = PALETTE_RAM + 0x4000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[14] = chip_result;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if ((failures & ~0xFFu) != 0u)
		chip_result = 3u;
	game->memory_test_results[15] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	cell_value = test_mode_set_up_colours();
	AS16(game->memory_test_results[16]) = 0x101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	memory_start = 0x680000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[16] = chip_result;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if ((failures & ~0xFFu) != 0u)
		chip_result = 3u;
	game->memory_test_results[17] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	cell_value = test_mode_set_up_colours();
	AS16(game->memory_test_results[18]) = 0x101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	memory_start = 0x684000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[18] = chip_result;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if ((failures & ~0xFFu) != 0u)
		chip_result = 3u;
	game->memory_test_results[19] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	cell_value = test_mode_set_up_colours();
	AS32(game->memory_test_results[20]) = 0x1010101u;
	test_mode_memory_test_draw(&cell_value, &r12);
	move_halfwords(SPRITE_RAM, WORK_RAM, 0x10000u);
	memory_start = 0x200000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_words(failures, memory_start);
	move_halfwords(WORK_RAM, SPRITE_RAM, 0x10000u);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[20] = chip_result;
	work_byte_1_failures = failures & 0xFF00u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (work_byte_1_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[21] = chip_result;
	work_byte_2_failures = failures & 0xFF0000u;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (work_byte_2_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[22] = chip_result;
	work_byte_3_failures = failures & ~0xFFFFFFu;
	failures = work_byte_3_failures;
	chip_result = 2u;
	current_object = pointer_to(address_of(current_object) + 1u);
	if (work_byte_3_failures != 0u)
		chip_result = 3u;
	game->memory_test_results[23] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	init_sprite_lists();
	game->memory_test_results[24] = 1u;
	test_mode_memory_test_draw(&failures, &r12);
	move_halfwords(SPRITE_RAM, SOUND_RAM, 0x1000u);
	memory_start = 0x700000u;
	failures = 0x1FFFu;
	failures = test_mode_memory_test_bytes(failures, memory_start);
	move_halfwords(SOUND_RAM, SPRITE_RAM, 0x1000u);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	game->memory_test_results[24] = chip_result;
	current_object = pointer_to(address_of(current_object) + 1u);
	init_sprite_lists();
	test_mode_memory_test_draw(&failures, &r12);
	SOUND_PROGRAM_HOLD = 0u;
	interrupts_on();
	test_mode_show_exit_prompt();
	game->test_mode_routine = test_mode_memory_test_run_ADDRESS;
	test_mode_memory_test_run((uint16_t)failures);
}

/* The memory test's results, each frame: TEST goes back to the menu; PUSH TEST BUTTON TO EXIT blinks
   (test_mode_blink_prompt). */
void test_mode_memory_test_run(uint16_t character)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Draws the memory test's page (MEMORY TEST, 837-8890 <ROM> and its four chips, 837-8676 <RAM> and its
   21, NOW TESTING) and each chip's result from memory_test_results beside its name: ---- (not yet),
   TEST, GOOD or BAD. */
void test_mode_memory_test_draw(uint32_t *p_cell_value, uint32_t *p_text_place)
{
	uint16_t cell_value = (uint16_t)*p_cell_value;
	uint16_t cell_contents = 0;
	uint32_t result_cells = 0, result_place = 0, results_left = 0, text = 0, text_place = 0;
	const struct memory_test_result_text *result_text = NULL;
	uint8_t colour = 0, character = 0, character_1 = 0, character_2 = 0;
	uint16_t cell = 0;

	text = memory_test_page_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	result_place = memory_test_results_ADDRESS;
	result_cells = memory_test_result_cells_ADDRESS;
	text_place = TEXT_CELL(0, 0);
	results_left = 0x19u;
	do
	{
		result_text = rom_pointer(memory_test_result_texts_ADDRESS + CHIP_TEST_RESULTS(result_place)[0] * 3u * 2u);
		colour = result_text->colour;
		cell = TEST_MODE_CELLS(result_cells)[0];
		result_cells += 2u;
		text_place = LINKED(text_place, cell);
		character = result_text->character_0;
		cell_contents = (uint16_t)((uint16_t)((uint32_t)colour << 8) | character);
		TEXT_LAYER_CELLS(text_place)[0] = cell_contents;
		text_place += 2u;
		character_1 = result_text->character_1;
		cell_contents = (uint16_t)((cell_contents & ~0xFFu) | character_1);
		TEXT_LAYER_CELLS(text_place)[0] = cell_contents;
		text_place += 2u;
		character_2 = result_text->character_2;
		cell_contents = (uint16_t)((cell_contents & ~0xFFu) | character_2);
		TEXT_LAYER_CELLS(text_place)[0] = cell_contents;
		text_place += 2u;
		cell_contents = (uint16_t)((cell_contents & ~0xFFu) | result_text->character_3);
		TEXT_LAYER_CELLS(text_place)[0] = cell_contents;
		text_place += 2u;
		results_left--;
		result_place++;
	} while (results_left != 0u);
	*p_cell_value = cell_contents;
	*p_text_place = text_place;
}

/* Sums a stretch of the program ROM as two 16-bit checks, one of the even halfwords (the chip of the low
   half of each 32-bit word: IC37 or IC36) and one of the odd (IC40 or IC39): each halfword is xored in,
   the sum shifted right by one and xored with 8810 when a 1 falls out (a CRC); both start at 1. */
void BOARD_ORIGINAL(test_mode_memory_test_checksum)(uint32_t memory_start, uint32_t *p_byte_count, uint32_t *p_odd_check)
{
	uint32_t byte_count = *p_byte_count;
	uint32_t carry = 0;
	uint8_t shift = 0;
	uint16_t even_sum = 0, odd_sum = 0, polynomial = 0;
	uint32_t pairs_left = 0;

	pairs_left = byte_count >> 2;
	even_sum = 1u;
	odd_sum = 1u;
	shift = 0xFFu;
	polynomial = 0x8810u;
	do
	{
		even_sum ^= PROGRAM_HALFWORDS(memory_start)[0];
		carry = v60_shl_cy(even_sum, shift, 16);
		even_sum = shift_by(even_sum, shift, 16);
		memory_start += 2u;
		if (carry)
			even_sum ^= polynomial;
		odd_sum ^= PROGRAM_HALFWORDS(memory_start)[0];
		carry = v60_shl_cy(odd_sum, shift, 16);
		odd_sum = shift_by(odd_sum, shift, 16);
		memory_start += 2u;
		if (carry)
			odd_sum ^= polynomial;
		pairs_left--;
	} while (pairs_left != 0u);
	*p_byte_count = (*p_byte_count & 0xFFFF0000u) | even_sum;
	*p_odd_check = (*p_odd_check & 0xFFFF0000u) | odd_sum;
}

/* Tests RAM 32 bits at a time (the work RAM, in place: it takes its return address off the stack into a
   register and jumps back through it, so that it needs no stack in the RAM it overwrites): 8 passes,
   each writing a walking pattern (01010101 the first pass, one bit further each pass, turned by a few
   bits from word to word by the word's address) and reading it back; returns the bits found wrong (each
   mismatch's expected ^ read, xored together), a byte per chip; NOW TESTING blinks once a pass
   (test_mode_memory_test_blink). */
uint32_t test_mode_memory_test_words(uint32_t byte_count, uint32_t memory_start)
{
	uint32_t carry = 0;
	uint8_t rotation = 0;
	uint32_t address_bits = 0, checked_address = 0, expected = 0, failures = 0, pattern = 0, tested_address = 0, word_count = 0, words_left = 0;
	uint32_t low_17_bits = 0, low_14_bits = 0, low_11_bits = 0, low_8_bits = 0, found = 0, read_low_17_bits = 0, read_low_14_bits = 0, read_low_11_bits = 0, read_low_8_bits = 0;

	word_count = byte_count >> 2;
	pattern = 0x1010101u;
	failures = 0u;
	do
	{
		tested_address = memory_start;
		expected = pattern;
		words_left = word_count;
		do
		{
			mem_write32(tested_address, expected);
			tested_address += 4u;
			rotation = 6u;
			low_17_bits = tested_address << 15;
			address_bits = low_17_bits;
			if (low_17_bits != 0u)
			{
				rotation--;
				low_14_bits = address_bits << 3;
				address_bits = low_14_bits;
				if (low_14_bits != 0u)
				{
					rotation--;
					low_11_bits = address_bits << 3;
					address_bits = low_11_bits;
					if (low_11_bits != 0u)
					{
						rotation--;
						low_8_bits = address_bits << 3;
						address_bits = low_8_bits;
						if (low_8_bits != 0u)
						{
							rotation--;
							if (address_bits << 3 != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 32);
			words_left--;
		} while (words_left != 0u);
		checked_address = memory_start;
		expected = pattern;
		words_left = word_count;
		do
		{
			found = mem_read32(checked_address);
			checked_address += 4u;
			if (found != expected)
				failures = failures ^ expected ^ mem_read32(checked_address - 4u);
			rotation = 6u;
			read_low_17_bits = checked_address << 15;
			address_bits = read_low_17_bits;
			if (read_low_17_bits != 0u)
			{
				rotation--;
				read_low_14_bits = address_bits << 3;
				address_bits = read_low_14_bits;
				if (read_low_14_bits != 0u)
				{
					rotation--;
					read_low_11_bits = address_bits << 3;
					address_bits = read_low_11_bits;
					if (read_low_11_bits != 0u)
					{
						rotation--;
						read_low_8_bits = address_bits << 3;
						address_bits = read_low_8_bits;
						if (read_low_8_bits != 0u)
						{
							rotation--;
							if (address_bits << 3 != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 32);
			words_left--;
		} while (words_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pattern, 1u, 32);
		pattern = (pattern << 1) | (pattern >> 31);
	} while (carry == 0u);
	return failures;
}

/* test_mode_memory_test_words 16 bits at a time (the palette RAMs: a chip for each byte), with the
   pattern 0101; returns the bits found wrong. */
uint16_t test_mode_memory_test_halfwords(uint32_t size, uint32_t memory_start)
{
	uint32_t carry = 0;
	uint8_t rotation = 0;
	uint16_t address_bits = 0, expected = 0, failures = 0, pattern = 0;
	uint32_t checked_address = 0, halfword_count = 0, halfwords_left = 0, tested_address = 0;
	uint16_t low_13_bits = 0, low_10_bits = 0, low_7_bits = 0, found = 0, read_low_13_bits = 0, read_low_10_bits = 0, read_low_7_bits = 0;

	halfword_count = size >> 1;
	pattern = 0x101u;
	failures = 0u;
	do
	{
		tested_address = memory_start;
		expected = (uint16_t)pattern;
		halfwords_left = halfword_count;
		do
		{
			io_write16(tested_address, expected);
			tested_address += 2u;
			address_bits = (uint16_t)tested_address;
			rotation = 6u;
			if ((uint16_t)address_bits != 0u)
			{
				rotation--;
				low_13_bits = (uint32_t)address_bits << 3;
				address_bits = low_13_bits;
				if (low_13_bits != 0u)
				{
					rotation--;
					low_10_bits = (uint32_t)address_bits << 3;
					address_bits = low_10_bits;
					if (low_10_bits != 0u)
					{
						rotation--;
						low_7_bits = (uint32_t)address_bits << 3;
						address_bits = low_7_bits;
						if (low_7_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		checked_address = memory_start;
		expected = (uint16_t)pattern;
		halfwords_left = halfword_count;
		do
		{
			found = io_read16(checked_address);
			checked_address += 2u;
			if (found != expected)
				failures = failures ^ expected ^ io_read16(checked_address - 2u);
			address_bits = (uint16_t)checked_address;
			rotation = 6u;
			if ((uint16_t)address_bits != 0u)
			{
				rotation--;
				read_low_13_bits = (uint32_t)address_bits << 3;
				address_bits = read_low_13_bits;
				if (read_low_13_bits != 0u)
				{
					rotation--;
					read_low_10_bits = (uint32_t)address_bits << 3;
					address_bits = read_low_10_bits;
					if (read_low_10_bits != 0u)
					{
						rotation--;
						read_low_7_bits = (uint32_t)address_bits << 3;
						address_bits = read_low_7_bits;
						if (read_low_7_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pattern, 1u, 16);
		pattern = (uint16_t)(((uint32_t)pattern << 1) | (pattern >> 15));
	} while (carry == 0u);
	return failures;
}

/* test_mode_memory_test_words a byte at a time (the sound RAM, all but its last byte, which holds the
   sound program), with the pattern 01; returns the bits found wrong. */
uint16_t test_mode_memory_test_bytes(uint32_t size_then_wrong_bits, uint32_t ram_start)
{
	uint32_t carry = 0;
	uint8_t pass_pattern = 0, pattern = 0, turn = 0;
	uint16_t address_bits = 0;
	uint32_t byte_count = 0, bytes_left = 0, tested_address = 0;
	uint8_t read_byte = 0;
	uint16_t address_low_15 = 0, address_low_12 = 0, address_low_9 = 0, address_low = 0, checked_address_low_15 = 0, checked_address_low_12 = 0, checked_address_low_9 = 0, checked_address_low = 0;

	byte_count = size_then_wrong_bits;
	pass_pattern = 1u;
	size_then_wrong_bits &= ~0xFFu;
	do
	{
		tested_address = ram_start;
		pattern = (uint8_t)pass_pattern;
		bytes_left = byte_count;
		do
		{
			TESTED_SOUND_RAM_BYTES(tested_address)[0] = pattern;
			tested_address++;
			turn = 6u;
			address_low_15 = tested_address * 2u;
			address_bits = address_low_15;
			if (address_low_15 != 0u)
			{
				turn--;
				address_low_12 = (uint32_t)address_bits << 3;
				address_bits = address_low_12;
				if (address_low_12 != 0u)
				{
					turn--;
					address_low_9 = (uint32_t)address_bits << 3;
					address_bits = address_low_9;
					if (address_low_9 != 0u)
					{
						turn--;
						address_low = (uint32_t)address_bits << 3;
						address_bits = address_low;
						if (address_low != 0u)
						{
							turn--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								turn--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		tested_address = ram_start;
		pattern = (uint8_t)pass_pattern;
		bytes_left = byte_count;
		do
		{
			read_byte = TESTED_SOUND_RAM_BYTES(tested_address)[0];
			tested_address++;
			if (read_byte != pattern)
			{
				size_then_wrong_bits = size_then_wrong_bits ^ pattern;
				size_then_wrong_bits = size_then_wrong_bits ^ TESTED_SOUND_RAM_BYTES(tested_address)[-1];
			}
			turn = 6u;
			checked_address_low_15 = tested_address * 2u;
			address_bits = checked_address_low_15;
			if (checked_address_low_15 != 0u)
			{
				turn--;
				checked_address_low_12 = (uint32_t)address_bits << 3;
				address_bits = checked_address_low_12;
				if (checked_address_low_12 != 0u)
				{
					turn--;
					checked_address_low_9 = (uint32_t)address_bits << 3;
					address_bits = checked_address_low_9;
					if (checked_address_low_9 != 0u)
					{
						turn--;
						checked_address_low = (uint32_t)address_bits << 3;
						address_bits = checked_address_low;
						if (checked_address_low != 0u)
						{
							turn--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								turn--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pass_pattern, 1u, 8);
		pass_pattern = (uint8_t)(((uint32_t)pass_pattern << 1) | (pass_pattern >> 7));
	} while (carry == 0u);
	return size_then_wrong_bits;
}

/* test_mode_memory_test_words for RAM of 4-bit chips, 16 bits at a time (the video RAM and the sprite
   RAM): 4 passes with the pattern 1111 (a bit per chip); returns the bits found wrong, a nibble per
   chip. */
uint16_t test_mode_memory_test_nibbles(uint32_t size, uint32_t ram_start)
{
	uint32_t carry = 0;
	uint8_t turn = 0;
	uint16_t pass_pattern = 0, pattern = 0, wrong_bits = 0;
	uint32_t address_bits = 0, checked_address = 0, halfword_count = 0, halfwords_left = 0, tested_address = 0;
	uint16_t read_halfword = 0;
	uint32_t address_low_17 = 0, address_low_15 = 0, address_low_13 = 0, address_low_11 = 0, address_low_9 = 0, address_low_7 = 0, address_low = 0, checked_address_low_17 = 0, checked_address_low_15 = 0, checked_address_low_13 = 0, checked_address_low_11 = 0, checked_address_low_9 = 0, checked_address_low_7 = 0, checked_address_low = 0;

	halfword_count = size >> 1;
	pass_pattern = 0x1111u;
	wrong_bits = 0u;
	do
	{
		tested_address = ram_start;
		pattern = (uint16_t)pass_pattern;
		halfwords_left = halfword_count;
		do
		{
			mem_write16(tested_address, pattern);
			tested_address += 2u;
			turn = 9u;
			address_low_17 = tested_address << 15;
			address_bits = address_low_17;
			if (address_low_17 != 0u)
			{
				turn--;
				address_low_15 = address_bits * 2u * 2u;
				address_bits = address_low_15;
				if (address_low_15 != 0u)
				{
					turn--;
					address_low_13 = address_bits * 2u * 2u;
					address_bits = address_low_13;
					if (address_low_13 != 0u)
					{
						turn--;
						address_low_11 = address_bits * 2u * 2u;
						address_bits = address_low_11;
						if (address_low_11 != 0u)
						{
							turn--;
							address_low_9 = address_bits * 2u * 2u;
							address_bits = address_low_9;
							if (address_low_9 != 0u)
							{
								turn--;
								address_low_7 = address_bits * 2u * 2u;
								address_bits = address_low_7;
								if (address_low_7 != 0u)
								{
									turn--;
									address_low = address_bits * 2u * 2u;
									address_bits = address_low;
									if (address_low != 0u)
									{
										turn--;
										if (address_bits * 2u * 2u != 0u)
											turn--;
									}
								}
							}
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		checked_address = ram_start;
		pattern = (uint16_t)pass_pattern;
		halfwords_left = halfword_count;
		do
		{
			read_halfword = mem_read16(checked_address);
			checked_address += 2u;
			if (read_halfword != pattern)
				wrong_bits = wrong_bits ^ pattern ^ mem_read16(checked_address - 2u);
			turn = 9u;
			checked_address_low_17 = checked_address << 15;
			address_bits = checked_address_low_17;
			if (checked_address_low_17 != 0u)
			{
				turn--;
				checked_address_low_15 = address_bits * 2u * 2u;
				address_bits = checked_address_low_15;
				if (checked_address_low_15 != 0u)
				{
					turn--;
					checked_address_low_13 = address_bits * 2u * 2u;
					address_bits = checked_address_low_13;
					if (checked_address_low_13 != 0u)
					{
						turn--;
						checked_address_low_11 = address_bits * 2u * 2u;
						address_bits = checked_address_low_11;
						if (checked_address_low_11 != 0u)
						{
							turn--;
							checked_address_low_9 = address_bits * 2u * 2u;
							address_bits = checked_address_low_9;
							if (checked_address_low_9 != 0u)
							{
								turn--;
								checked_address_low_7 = address_bits * 2u * 2u;
								address_bits = checked_address_low_7;
								if (checked_address_low_7 != 0u)
								{
									turn--;
									checked_address_low = address_bits * 2u * 2u;
									address_bits = checked_address_low;
									if (checked_address_low != 0u)
									{
										turn--;
										if (address_bits * 2u * 2u != 0u)
											turn--;
									}
								}
							}
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pass_pattern, 1u, 16);
		pass_pattern = (uint16_t)(((uint32_t)pass_pattern << 1) | (pass_pattern >> 15));
	} while (carry == 0u);
	return wrong_bits;
}

/* Shows or hides NOW TESTING (each of its cells xored with its character in palette 66): the RAM tests
   call it after each pass. */
void test_mode_memory_test_blink(void)
{
	uint16_t character = 0;
	uint32_t next_cell = 0, now_testing = 0;
	uint32_t cell = 0;

	character = 0x6600u;
	next_cell = TEXT_CELL(14, 25);
	now_testing = memory_test_now_testing_text_ADDRESS;
	do
	{
		character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(now_testing)[0]);
		cell = next_cell;
		next_cell += 2u;
		TEXT_LAYER_CELLS(cell)[0] ^= character;
		now_testing++;
	} while (TEST_MODE_CHARACTERS(now_testing)[0] != 0u);
}

/* Starts the VOLUME ADJUSTMENT, the calibration of the wheels' and pedals' potentiometers: draws ADJUST
   and CANCEL with the cursor on CANCEL (test_mode_page_cursor 1), the six controls' names (MONITOR A and
   B: STEERING WHEEL, ACCELERATOR, BRAKE) and the prompt; then test_mode_volume_adjustment_select. */
void test_mode_volume_adjustment(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = volume_adjustment_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	text = volume_adjustment_controls_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	game->test_mode_page_cursor = 1u;
	cell = 0x310000u | volume_adjustment_cancel_cell;
	character = test_mode_set_cursor(character, cell);
	game->test_mode_routine = test_mode_volume_adjustment_select_ADDRESS;
	test_mode_volume_adjustment_select(character);
}

/* Each frame: the bars (test_mode_volume_adjustment_draw); SERVICE moves the cursor between ADJUST and
   CANCEL; TEST on CANCEL goes back to the menu; TEST on ADJUST starts the adjustment: ADJUSTING, PUSH
   TEST BUTTON TO EXIT, each control's range (volume_ranges) narrowed to where it is now, then
   test_mode_volume_adjustment_adjust. */
void test_mode_volume_adjustment_select(uint32_t character)
{
	uint8_t position = 0;
	uint16_t drawn_character = 0;
	uint32_t analog_input = 0, cell = 0, controls_left = 0, page_cursor = 0, text = 0;
	struct volume_range *range = NULL;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	test_mode_volume_adjustment_draw(&character, &r12);
	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor ^ 1u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | volume_adjustment_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) == 0u)
	{
		blink_timer = game->test_mode_blink_timer - 1u;
		game->test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			game->test_mode_blink_timer = 5;
			blink_cell = game->test_mode_blink_address;
			cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
			TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
			if (cursor_character != 0u)
				game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
		}
		return;
	}
	if (game->test_mode_page_cursor == 0u)
	{
		drawn_character = test_mode_clear_text();
		text = volume_adjusting_text_ADDRESS;
		drawn_character = test_mode_draw_texts(drawn_character, text);
		text = volume_adjustment_controls_text_ADDRESS;
		test_mode_draw_texts(drawn_character, text);
		test_mode_show_exit_prompt();
		range = pointer_to(volume_ranges_ADDRESS);
		analog_input = test_mode_analog_inputs_ADDRESS;
		controls_left = 6u;
		do
		{
			position = (uint8_t)(ANALOG_POSITIONS(analog_input)[0] & ~1u);
			range->low = position;
			range->high = position | 1u;
			controls_left--;
			analog_input++;
			range++;
		} while (controls_left != 0u);
		game->test_mode_routine = test_mode_volume_adjustment_adjust_ADDRESS;
		return;
	}
	game->test_mode_routine = test_mode_menu_ADDRESS;
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* The adjustment, each frame: each control's range (volume_ranges) widens to take its position
   (test_mode_analog_inputs), as the operator turns the wheels and presses the pedals all the way; the
   bars; TEST marks the settings changed (test_mode_flags bit 1) and goes back to the menu; PUSH TEST
   BUTTON TO EXIT blinks. */
void test_mode_volume_adjustment_adjust(void)
{
	uint32_t analog_input = 0, controls_left = 0, position = 0;
	struct volume_range *range = NULL;

	analog_input = test_mode_analog_inputs_ADDRESS;
	range = pointer_to(volume_ranges_ADDRESS);
	controls_left = 6u;
	do
	{
		position = ANALOG_POSITIONS(analog_input)[0];
		analog_input++;
		if (position < range->low)
			range->low = position;
		if (range->high < position)
			range->high = position;
		range++;
		controls_left--;
	} while (controls_left != 0u);
	test_mode_volume_adjustment_draw(&position, &r12);
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		game->test_mode_flags |= 2u;
		game->test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt((uint16_t)position);
}

/* Draws the six controls' bars (32 cells, a cell for 8 steps): the range of volume_ranges in green and
   the control's position (test_mode_analog_inputs) in red. */
void test_mode_volume_adjustment_draw(uint32_t *p_character, uint32_t *p_last_analog_input)
{
	uint16_t character = (uint16_t)*p_character;
	uint16_t range_character = 0;
	uint32_t analog_input = 0, bar_cells = 0, calibration = 0, cell = 0, controls_left = 0, position_step = 0, range_length = 0, range_start = 0, text = 0;
	uint16_t range_cell_a = 0, range_cell_b = 0, position_cell = 0;

	text = volume_adjustment_bars_text_ADDRESS;
	test_mode_draw_texts(character, text);
	cell = TEXT_CELL(0, 0);
	bar_cells = volume_adjustment_bar_cells_ADDRESS;
	calibration = volume_ranges_ADDRESS;
	controls_left = 3u;
	do
	{
		range_start = ANALOG_CALIBRATION_BYTES(calibration)[0];
		range_start = range_start >> 3;
		calibration++;
		range_length = ANALOG_CALIBRATION_BYTES(calibration)[0];
		range_length = range_length >> 3;
		range_length = (uint8_t)(range_length - range_start);
		range_length = (uint8_t)(range_length + 1u);
		range_cell_a = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = LINKED(cell, range_cell_a);
		cell += range_start * 2u;
		range_character = 0x6403u;
		calibration++;
		do
		{
			TEXT_LAYER_CELLS(cell)[0] = range_character;
			range_length--;
			cell += 2u;
		} while (range_length != 0u);
		controls_left--;
	} while (controls_left != 0u);
	controls_left = 3u;
	do
	{
		range_start = ANALOG_CALIBRATION_BYTES(calibration)[0];
		range_start = range_start >> 3;
		calibration++;
		range_length = ANALOG_CALIBRATION_BYTES(calibration)[0];
		range_length = range_length >> 3;
		range_length = (uint8_t)(range_length - range_start);
		range_length = (uint8_t)(range_length + 1u);
		range_cell_b = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = LINKED(cell, range_cell_b);
		cell += range_start * 2u;
		range_character = 0x6403u;
		calibration++;
		do
		{
			TEXT_LAYER_CELLS(cell)[0] = range_character;
			range_length--;
			cell += 2u;
		} while (range_length != 0u);
		controls_left--;
	} while (controls_left != 0u);
	bar_cells = volume_adjustment_bar_cells_ADDRESS;
	analog_input = test_mode_analog_inputs_ADDRESS;
	controls_left = 6u;
	do
	{
		position_step = ANALOG_POSITIONS(analog_input)[0] >> 3;
		position_cell = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = LINKED(cell, position_cell);
		TEXT_LAYER_CELLS(cell)[(int32_t)position_step] = 0x6203u;
		controls_left--;
		analog_input++;
	} while (controls_left != 0u);
	*p_character = position_step;
	*p_last_analog_input = analog_input;
}

/* Starts the INPUT TEST: clears the text layer, draws the inputs' names and the blinking PUSH TEST
   BUTTON TO EXIT, then test_mode_input_test_run. */
void test_mode_input_test(void)
{
	uint16_t character = 0;
	uint32_t text = 0;

	character = test_mode_clear_text();
	text = input_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_show_exit_prompt();
	game->test_mode_routine = test_mode_input_test_run_ADDRESS;
	test_mode_input_test_run(character);
}

/* The input test, each frame: the inputs (test_mode_input_test_draw); TEST goes on to
   test_mode_input_test_release, so that the press shows as TEST On before the page ends; PUSH TEST
   BUTTON TO EXIT blinks (test_mode_blink_prompt). */
void test_mode_input_test_run(uint32_t character)
{
	test_mode_input_test_draw(&character, &r12, &r14);
	if ((game->test_mode_buttons_pressed & 2u) == 0u)
	{
		test_mode_blink_prompt((uint16_t)character);
		return;
	}
	game->test_mode_routine = test_mode_input_test_release_ADDRESS;
	test_mode_blink_prompt((uint16_t)character);
}

/* The input test once TEST is pressed: the inputs each frame until TEST is let go, then back to the
   menu. */
void test_mode_input_test_release(void)
{
	uint32_t test_switches = 0;

	test_mode_input_test_draw(&test_switches, &r12, &r14);
	if ((game->test_mode_buttons_released & 2u) == 0u)
	{
		test_mode_blink_prompt((uint16_t)test_switches);
		return;
	}
	game->test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt((uint16_t)test_switches);
}

/* Draws the inputs of both monitors (cabinets): the wheel, accelerator and brake as the game sees them
   through the calibration (test_mode_analog_inputs clamped to volume_ranges and scaled to 0-FF), as a
   mark on a bar of 16 cells and a hexadecimal number; SHIFT UP, SHIFT DOWN, START, DJ/MUSIC, <<, >>,
   COIN and SERVICE On or Off, read from each I/O chip's ports (active low); TEST On when either
   monitor's is pressed. */
void test_mode_input_test_draw(uint32_t *p_character, uint32_t *p_last_port, uint32_t *p_last_bar_cells)
{
	uint16_t character = (uint16_t)*p_character;
	uint32_t carry = 0;
	uint8_t colour = 0, test_switches_b = 0;
	uint16_t range_high = 0, range_low = 0, test_switches = 0;
	uint32_t analog_input = 0, bar_cells = 0, bit = 0, cell = 0, controls_left = 0, digit_count = 0, item = 0, port_a = 0, port_b = 0, position = 0, text = 0;
	struct volume_range *range = NULL;

	text = input_test_bars_text_ADDRESS;
	test_mode_draw_texts(character, text);
	cell = TEXT_CELL(0, 0);
	item = input_test_items_ADDRESS;
	analog_input = test_mode_analog_inputs_ADDRESS;
	range = pointer_to(volume_ranges_ADDRESS);
	bar_cells = input_test_bar_cells_ADDRESS;
	controls_left = 6u;
	do
	{
		position = ANALOG_POSITIONS(analog_input)[0];
		range_low = range->low;
		analog_input++;
		if (position < range_low)
			position = range_low;
		range_high = range->high;
		range++;
		if (range_high < position)
			position = range_high;
		position = (uint8_t)(position - range_low);
		range_high = (uint8_t)(range_high - range_low);
		range_low = (uint16_t)position;
		position = (uint16_t)(position << 8);
		position = (uint16_t)(position - range_low);
		position = divide_u16(position, range_high);
		cell = LINKED(cell, TEST_MODE_CELLS(bar_cells)[0]);
		TEXT_LAYER_CELLS(cell)[(uint8_t)position >> 4] = 0x6403u;
		colour = 0x64u;
		digit_count = 2u;
		cell = LINKED(cell, AS16(INPUT_TEST_BYTES(item)[0]));
		bar_cells += 2u;
		item += 2u;
		cell = test_mode_draw_hex_number(position, colour, digit_count, cell);
		controls_left--;
	} while (controls_left != 0u);
	port_a = IO_1_PORT_A;
	port_b = 0xC80000u;
	do
	{
		port_a = IN_PAGE(port_a, INPUT_TEST_BYTES(item)[0]);
		item++;
		bit = INPUT_TEST_BYTES(item)[0];
		carry = (io_read32(port_a) & shl32(1u, (int)(bit))) != 0u;
		item++;
		cell = LINKED(cell, AS16(INPUT_TEST_BYTES(item)[0]));
		AS32(TEXT_LAYER_CELLS(cell)[0]) = input_test_on_off_cells[carry];
		port_b = IN_PAGE(port_b, (uint8_t)port_a);
		carry = (io_read32(port_b) & shl32(1u, (int)(bit))) != 0u;
		item += 2u;
		cell = LINKED(cell, AS16(INPUT_TEST_BYTES(item)[0]));
		AS32(TEXT_LAYER_CELLS(cell)[0]) = input_test_on_off_cells[carry];
		item += 2u;
	} while ((int16_t)AS16(INPUT_TEST_BYTES(item)[0]) >= 0);
	port_a = IN_PAGE(port_a, 8u);
	port_b = IN_PAGE(port_b, (uint8_t)port_a);
	test_switches = io_read8(port_a);
	port_a = IN_PAGE(port_a, 0xAu);
	test_switches = test_switches & (io_read8(port_a) >> 4);
	test_switches_b = io_read8(port_b) & (io_read8(IN_PAGE(port_b, (uint8_t)port_a)) >> 4);
	carry = (test_switches & 1u) != 0u;
	item += 2u;
	cell = LINKED(cell, AS16(INPUT_TEST_BYTES(item)[0]));
	AS32(TEXT_LAYER_CELLS(cell)[0]) = input_test_on_off_cells[carry];
	carry = (test_switches_b & 1u) != 0u;
	item += 2u;
	cell = LINKED(cell, AS16(INPUT_TEST_BYTES(item)[0]));
	AS32(TEXT_LAYER_CELLS(cell)[0]) = input_test_on_off_cells[carry];
	test_switches = test_switches & test_switches_b;
	carry = (test_switches & 2u) != 0u;
	AS32(TEXT_LAYER_CELLS(LINKED(cell, AS16(INPUT_TEST_BYTES(item)[2])))[0]) = input_test_on_off_cells[carry];
	*p_character = test_switches;
	*p_last_port = port_a;
	*p_last_bar_cells = bar_cells;
}

/* Starts the OUTPUT TEST: the ten outputs of the two monitors (cabinets) with their state
   (test_mode_output_test_draw), EXIT and the prompt, the cursor on EXIT (test_mode_page_cursor 10); then
   test_mode_output_test_run. */
void test_mode_output_test(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = output_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	game->test_mode_page_cursor = 0xAu;
	cell = 0x310000u | output_test_exit_cell;
	test_mode_set_cursor(character, cell);
	r12 = test_mode_output_test_draw();
	game->test_mode_routine = test_mode_output_test_run_ADDRESS;
	test_mode_output_test_run();
}

/* The output test, each frame: TEST turns the output under the cursor on and letting go of it turns it
   off again (test_mode_output_test_toggle both times), so it is on while TEST is held; TEST on EXIT runs
   test_mode_output_test_exit; SERVICE, while TEST is not held, moves the cursor through the eleven
   items; the arrow blinks. */
void test_mode_output_test_run(void)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		switch (game->test_mode_page_cursor)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			test_mode_output_test_toggle();
			break;
		case 10:
			test_mode_output_test_exit();
			break;
		}
		r12 = test_mode_output_test_draw();
	}
	if ((game->test_mode_buttons_released & 2u) != 0u)
	{
		page_cursor = game->test_mode_page_cursor;
		if (page_cursor != 0xAu)
		{
			switch (page_cursor)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				test_mode_output_test_toggle();
				break;
			case 10:
				test_mode_output_test_exit();
				break;
			}
			r12 = test_mode_output_test_draw();
		}
	}
	if ((game->test_mode_buttons_pressed & 1u) == 0u || (game->test_mode_buttons & 2u) != 0u)
	{
		blink_timer = game->test_mode_blink_timer - 1u;
		game->test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			game->test_mode_blink_timer = 5;
			blink_cell = game->test_mode_blink_address;
			cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
			TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
			if (cursor_character != 0u)
				game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
		}
		return;
	}
	test_mode_clear_cursor();
	page_cursor = game->test_mode_page_cursor;
	page_cursor = (uint8_t)(page_cursor + 1u);
	if ((int8_t)page_cursor >= 0xB)
		page_cursor = 0u;
	game->test_mode_page_cursor = page_cursor;
	cell = 0x310000u | output_test_cursor_cells[page_cursor];
	test_mode_set_cursor(page_cursor, cell);
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws On or Off for the ten outputs, from their bits in the game's output bytes: monitor A's CHECK
   POINT and RACE LEADER lamps (port_d_outputs bits 1 and 3), the steering wheel's motor (port_d_outputs
   bit 4), the DJ/MUSIC and << >> lamps (port_g_outputs bits 0 and 1); monitor B's the same bits of
   20E093 and 20E095. */
uint32_t test_mode_output_test_draw(void)
{
	uint32_t carry = 0;
	uint32_t cell = 0, output_bits = 0;
	const struct output_test_output *output = NULL;

	cell = TEXT_CELL(0, 0);
	output = rom_pointer(output_test_outputs_ADDRESS);
	do
	{
		cell = LINKED(cell, output->cell);
		output_bits = output->variable;
		output_bits += 0x20E000u;
		carry = (OUTPUT_BITS(output_bits)[0] & shl32(1u, (int)(output->bit))) != 0u;
		AS32(TEXT_LAYER_CELLS(cell)[0]) = output_test_on_off_cells[carry];
		output++;
	} while (output->cell != 0u);
	return output_bits;
}

/* Turns the output under the cursor (test_mode_page_cursor) on or off: flips its bit in the game's
   output bytes (the list at 01D5C0). */
void test_mode_output_test_toggle(void)
{
	uint32_t output_variable = 0;
	const struct output_test_output *output = NULL;

	output = rom_pointer(output_test_outputs_ADDRESS + 0x2u + game->test_mode_page_cursor * 4u - 2u);
	output_variable = output->variable + GAME_VARIABLES;
	OUTPUT_BITS(output_variable)[0] ^= shl32(1u, (int)(output->bit));
}

/* The output test's EXIT: turns the lamps and the wheel motor off (port_d_outputs &= E0E1,
   port_g_outputs &= FBFC: monitor B's DJ/MUSIC and << >> bits, 20E095 bits 0-1, are not among them) and
   goes back to the menu. */
void test_mode_output_test_exit(void)
{
	uint32_t port_d_outputs_0_halfword = 0, port_g_outputs_0_halfword = 0;

	port_d_outputs_0_halfword = (game->port_d_outputs[0] | ((uint32_t)game->port_d_outputs[1] << 8u)) & 0xE0E1u;
	game->port_d_outputs[0] = (uint8_t)port_d_outputs_0_halfword;
	game->port_d_outputs[1] = (uint8_t)(port_d_outputs_0_halfword >> 8u);
	port_g_outputs_0_halfword = (game->port_g_outputs[0] | ((uint32_t)game->port_g_outputs[1] << 8u)) & ~0x403u;
	game->port_g_outputs[0] = (uint8_t)port_g_outputs_0_halfword;
	game->port_g_outputs[1] = (uint8_t)(port_g_outputs_0_halfword >> 8u);
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the SOUND TEST: MONITOR A No., MONITOR B No. (the numbers of the next sounds, both 0:
   sound_test_number_a, sound_test_number_b), EXIT with the cursor on it (test_mode_page_cursor 2) and
   the prompt; then test_mode_sound_test_run. */
void test_mode_sound_test(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = sound_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	game->test_mode_page_cursor = 2u;
	cell = 0x310000u | sound_test_exit_cell;
	test_mode_set_cursor(character, cell);
	game->sound_test_number_a = 0u;
	game->sound_test_number_b = 0u;
	test_mode_sound_test_draw();
	game->test_mode_routine = test_mode_sound_test_run_ADDRESS;
	test_mode_sound_test_run();
}

/* The sound test, each frame: SERVICE moves the cursor through the three items; TEST stops both
   monitors' sounds (command 1F to each), then plays monitor A's next sound
   (test_mode_sound_test_play_a), monitor B's (test_mode_sound_test_play_b) or goes back to the menu
   (test_mode_sound_test_exit), and redraws the numbers; the arrow blinks. */
void test_mode_sound_test_run(void)
{
	uint16_t stop_request = 0;
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		if ((int8_t)page_cursor >= 3)
			page_cursor = 0u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | sound_test_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		stop_request = 0x1Fu;
		test_mode_queue_sound(stop_request);
		test_mode_queue_sound_screen_2(stop_request);
		switch (game->test_mode_page_cursor)
		{
		case 0:
			test_mode_sound_test_play_a();
			break;
		case 1:
			test_mode_sound_test_play_b();
			break;
		case 2:
			test_mode_sound_test_exit();
			break;
		}
		test_mode_sound_test_draw();
	}
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws the two monitors' sound numbers (sound_test_number_a, sound_test_number_b), 3 digits each. */
void test_mode_sound_test_draw(void)
{
	uint32_t cell = 0, colour = 0, digit_count = 0, number = 0;

	cell = TEXT_CELL(28, 11);
	number = game->sound_test_number_a;
	colour = 0x64u;
	digit_count = 3u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
	cell = (cell & ~0xFFFFu) | 0xF6B8u;
	number = game->sound_test_number_b;
	colour = 0x64u;
	digit_count = 3u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
}

/* Plays monitor A's sound number sound_test_number_a from the list of 121 sound codes at 01D7C6
   (test_mode_queue_sound) and steps to the next; from number 40 on it first sends command D6 (DA from
   66, D8 from 6B), which selects sound set 6 (A, 8): the music parts' templates and the screen's
   MultiPCM sample bank; past the list's end (its closing 0) the number goes back to 0 without a sound. */
void test_mode_sound_test_play_a(void)
{
	uint16_t sound_set_request = 0;
	uint32_t sound_code = 0, sound_number = 0;
	uint32_t saved_sound_number = 0;

	sound_number = game->sound_test_number_a;
	if (sound_number < 0x40u)
	{
		sound_code = sound_test_codes[sound_number];
		if (sound_code == 0u)
		{
			game->sound_test_number_a = 0u;
			return;
		}
		game->sound_test_number_a = sound_number + 1u;
		test_mode_queue_sound((uint16_t)sound_code);
		return;
	}
	sound_set_request = 0xD6u;
	if (sound_number >= 0x66u)
	{
		sound_set_request = 0xDAu;
		if (sound_number >= 0x6Bu)
			sound_set_request = 0xD8u;
	}
	saved_sound_number = sound_number;
	test_mode_queue_sound(sound_set_request);
	sound_number = saved_sound_number;
	sound_code = sound_test_codes[sound_number];
	if (sound_code == 0u)
	{
		game->sound_test_number_a = 0u;
		return;
	}
	game->sound_test_number_a = sound_number + 1u;
	test_mode_queue_sound((uint16_t)sound_code);
}

/* test_mode_sound_test_play_a for monitor B: sound_test_number_b, sent through
   test_mode_queue_sound_screen_2. */
void test_mode_sound_test_play_b(void)
{
	uint16_t sound_set_request = 0;
	uint32_t sound_code = 0, sound_number = 0;
	uint32_t saved_sound_number = 0;

	sound_number = game->sound_test_number_b;
	if (sound_number < 0x40u)
	{
		sound_code = sound_test_codes[sound_number];
		if (sound_code == 0u)
		{
			game->sound_test_number_b = 0u;
			return;
		}
		game->sound_test_number_b = sound_number + 1u;
		test_mode_queue_sound_screen_2((uint16_t)sound_code);
		return;
	}
	sound_set_request = 0xD6u;
	if (sound_number >= 0x66u)
	{
		sound_set_request = 0xDAu;
		if (sound_number >= 0x6Bu)
			sound_set_request = 0xD8u;
	}
	saved_sound_number = sound_number;
	test_mode_queue_sound_screen_2(sound_set_request);
	sound_number = saved_sound_number;
	sound_code = sound_test_codes[sound_number];
	if (sound_code == 0u)
	{
		game->sound_test_number_b = 0u;
		return;
	}
	game->sound_test_number_b = sound_number + 1u;
	test_mode_queue_sound_screen_2((uint16_t)sound_code);
}

/* The sound test's EXIT: back to the menu. */
void test_mode_sound_test_exit(void)
{
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the C.R.T. TEST's first page: a black background (the background colour of both palettes,
   609C00 and 689C00), C.R.T. TEST 1/2, the colour bars (test_mode_crt_test_draw_bars) and the blinking
   PUSH TEST BUTTON TO CONTINUE; then test_mode_crt_test_bars_run. */
void test_mode_crt_test(void)
{
	uint16_t character = 0;
	uint32_t text = 0;

	character = test_mode_clear_text();
	io_write16(BACKGROUND_COLOUR_1, 0u);
	io_write16(BACKGROUND_COLOUR_2, 0u);
	text = crt_test_first_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_crt_test_draw_bars(character);
	test_mode_show_continue_prompt();
	game->test_mode_routine = test_mode_crt_test_bars_run_ADDRESS;
	test_mode_crt_test_bars_run(character);
}

/* The colour bars, each frame: TEST goes on to the second page (test_mode_crt_test_grid); the message
   blinks. */
void test_mode_crt_test_bars_run(uint16_t character)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_crt_test_grid_ADDRESS;
	test_mode_blink_prompt(character);
}

/* The C.R.T. test's second page: the crosshatch (test_mode_crt_test_draw_grid), C.R.T. TEST 2/2 and the
   blinking PUSH TEST BUTTON TO EXIT; then test_mode_crt_test_grid_run. */
void test_mode_crt_test_grid(void)
{
	uint16_t character = 0;
	uint32_t text = 0;

	character = test_mode_crt_test_draw_grid(0u);
	text = crt_test_second_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_show_exit_prompt();
	game->test_mode_routine = test_mode_crt_test_grid_run_ADDRESS;
	test_mode_crt_test_grid_run(character);
}

/* The crosshatch, each frame: TEST clears the text layer, gives the background back its blue (733) and
   goes back to the menu; the message blinks. */
void test_mode_crt_test_grid_run(uint16_t character)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		character = test_mode_clear_text();
		io_write16(BACKGROUND_COLOUR_1, 0x733u);
		io_write16(BACKGROUND_COLOUR_2, 0x733u);
		game->test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt(character);
}

/* Draws the first page on the text layer: a frame around the screen and four bars of 32 steps
   (test_mode_crt_test_draw_bar, characters 8001-8004: red, green, blue and white). */
uint16_t test_mode_crt_test_draw_bars(uint32_t character)
{
	uint16_t drawn_character = 0;
	uint32_t cell = 0, cells_left = 0, rows_left = 0;

	cell = TEXT_CELL(1, 0);
	drawn_character = 0x6C15u;
	cells_left = 0x26u;
	do
	{
		TEXT_LAYER_CELLS(cell)[1728] = drawn_character;
		TEXT_LAYER_CELLS(cell)[0] = drawn_character;
		cell += 2u;
		cells_left--;
	} while (cells_left != 0u);
	cell = LINKED(cell, 0xF080u);
	drawn_character++;
	rows_left = 0x1Au;
	do
	{
		TEXT_LAYER_CELLS(cell)[39] = drawn_character;
		TEXT_LAYER_CELLS(cell)[0] = drawn_character;
		rows_left--;
		cell += 0x80u;
	} while (rows_left != 0u);
	cell = LINKED(cell, 0xF000u);
	drawn_character++;
	TEXT_LAYER_CELLS(cell)[0] = drawn_character;
	drawn_character++;
	TEXT_LAYER_CELLS(cell)[39] = drawn_character;
	drawn_character++;
	TEXT_LAYER_CELLS(cell)[1728] = drawn_character;
	TEXT_LAYER_CELLS(cell)[1767] = drawn_character + 1u;
	cell = LINKED(cell, 0xF208u);
	drawn_character = 0x8001u;
	cell = test_mode_crt_test_draw_bar(drawn_character, cell);
	drawn_character = 0x8002u;
	cell = test_mode_crt_test_draw_bar(drawn_character, cell);
	drawn_character = 0x8003u;
	cell = test_mode_crt_test_draw_bar(drawn_character, cell);
	drawn_character = 0x8004u;
	test_mode_crt_test_draw_bar(drawn_character, cell);
	return drawn_character;
}

/* Draws one colour bar: 5 rows of 32 cells of character R0, each column with the next palette (the cell
   + 200), so that the colour steps up across it; returns the place 5 rows down for the next bar. */
uint32_t test_mode_crt_test_draw_bar(uint16_t character, uint32_t cell)
{
	uint16_t column_character = 0;
	uint32_t column_cell = 0, columns_left = 0;

	column_cell = cell;
	column_character = (uint16_t)character;
	columns_left = 0x20u;
	do
	{
		TEXT_LAYER_CELLS(column_cell)[256] = column_character;
		TEXT_LAYER_CELLS(column_cell)[192] = column_character;
		TEXT_LAYER_CELLS(column_cell)[128] = column_character;
		TEXT_LAYER_CELLS(column_cell)[64] = column_character;
		TEXT_LAYER_CELLS(column_cell)[0] = column_character;
		column_character += 0x200u;
		columns_left--;
		column_cell += 2u;
	} while (columns_left != 0u);
	cell += 0x280u;
	return cell;
}

/* Draws the crosshatch of the second page over the whole text layer (characters 6C1B-6C1F: the edges and
   the inside), and blanks the cell after the exit message. */
uint16_t test_mode_crt_test_draw_grid(uint32_t character)
{
	uint16_t drawn_character = 0, far_edge_character = 0;
	uint32_t cell = 0, cells_left = 0, cells_left_in_row = 0, row_cell = 0, rows_left = 0;

	cell = TEXT_CELL(0, 1);
	drawn_character = 0x6C1Bu;
	far_edge_character = 0x6C1Cu;
	rows_left = 0x1Au;
	do
	{
		TEXT_LAYER_CELLS(cell)[39] = far_edge_character;
		TEXT_LAYER_CELLS(cell)[0] = drawn_character;
		rows_left--;
		cell += 0x80u;
	} while (rows_left != 0u);
	cell = LINKED(cell, 0xF002u);
	far_edge_character++;
	drawn_character = (uint16_t)far_edge_character;
	far_edge_character++;
	cells_left = 0x26u;
	do
	{
		TEXT_LAYER_CELLS(cell)[1728] = far_edge_character;
		TEXT_LAYER_CELLS(cell)[0] = drawn_character;
		cell += 2u;
		cells_left--;
	} while (cells_left != 0u);
	cell = LINKED(cell, 0xF082u);
	drawn_character = (uint16_t)(far_edge_character + 1u);
	rows_left = 0x1Au;
	do
	{
		row_cell = cell;
		cells_left_in_row = 0x26u;
		do
		{
			TEXT_LAYER_CELLS(row_cell)[0] = drawn_character;
			row_cell += 2u;
			cells_left_in_row--;
		} while (cells_left_in_row != 0u);
		rows_left--;
		cell += 0x80u;
	} while (rows_left != 0u);
	TEXT_LAYER_CELLS(LINKED(cell, 0xFCC0u))[0] = 0u;
	return drawn_character;
}

/* Starts the GAME ASSIGNMENTS: the settings' names and values (test_mode_game_assignments_draw), EXIT,
   the prompt, the cursor on EXIT (test_mode_page_cursor 9); then test_mode_game_assignments_run. */
void test_mode_game_assignments(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, character_2 = 0, text = 0;

	character = test_mode_clear_text();
	text = game_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	test_mode_game_assignments_draw(&character_2, &r12);
	test_mode_draw_instructions(character_2);
	game->test_mode_page_cursor = 9u;
	cell = 0x310000u | game_assignments_exit_cell;
	test_mode_set_cursor(character_2, cell);
	game->test_mode_routine = test_mode_game_assignments_run_ADDRESS;
	test_mode_game_assignments_run();
}

/* The game assignments, each frame: SERVICE moves the cursor through the ten items; TEST changes the
   setting under it: the game difficulty (test_mode_game_assignments_difficulty), a stage's time
   (test_mode_game_assignments_time), the steering wheel reaction (test_mode_game_assignments_steering),
   the advertise sound (test_mode_game_assignments_advertise), or EXIT (test_mode_game_assignments_exit);
   the arrow blinks. */
void test_mode_game_assignments_run(void)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		if ((int8_t)page_cursor >= 0xA)
			page_cursor = 0u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | game_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = pointer_to(settings_magic_ADDRESS);
		switch (game->test_mode_page_cursor)
		{
		case 0:
			test_mode_game_assignments_difficulty();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			test_mode_game_assignments_time();
			break;
		case 7:
			test_mode_game_assignments_steering();
			break;
		case 8:
			test_mode_game_assignments_advertise();
			break;
		case 9:
			test_mode_game_assignments_exit();
			break;
		}
	}
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws the game settings: game_difficulty + 1 (1-8) and its bar from EASY to HARD (two cells a level),
   the six stages' time_difficulty (NORMAL, or +n SEC / -n SEC), and steering_wheel_reaction and
   advertise_sound (bit 0) as On or Off. */
void test_mode_game_assignments_draw(uint32_t *p_character, uint32_t *p_difficulty_seconds)
{
	uint16_t grey = 0, sign_character = 0;
	uint32_t advertise_on = 0, bar_cells_left = 0, cell = 0, colour = 0, difficulty_seconds = 0, digit_count = 0, number = 0, row_cell = 0, seconds = 0, stages_left = 0, steering_on = 0, text = 0;
	uint32_t bar_length = 0;

	current_object = pointer_to(settings_magic_ADDRESS);
	cell = TEXT_CELL(32, 5);
	colour = 0x64u;
	digit_count = 2u;
	number = game->game_difficulty + 1u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
	cell = LINKED(cell, 0xF3A8u);
	bar_length = game->game_difficulty * 2u;
	bar_cells_left = bar_length;
	if (bar_length != 0u)
	{
		do
		{
			TEXT_LAYER_CELLS(cell)[0] = (TEXT_LAYER_CELLS(cell)[0] & 0x1FFu) | 0x6400u;
			cell += 2u;
			bar_cells_left--;
		} while (bar_cells_left != 0u);
	}
	grey = 0x60u;
	test_mode_colour_line(grey, cell);
	row_cell = TEXT_CELL(28, 10);
	difficulty_seconds = address_of(current_object) + 0x32u;
	stages_left = 6u;
	do
	{
		cell = row_cell;
		seconds = TIME_DIFFICULTY_SECONDS(difficulty_seconds)[0];
		difficulty_seconds++;
		if (seconds == 0u)
		{
			TEXT_LAYER_CELLS(cell)[0] = 0x644Eu;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = 0x644Fu;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = 0x6452u;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = 0x644Du;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = 0x6441u;
			TEXT_LAYER_CELLS(cell)[1] = 0x644Cu;
		}
		else
		{
			sign_character = 0x642Bu;
			if ((int8_t)seconds < 0)
			{
				sign_character = 0x642Du;
				seconds = (uint8_t)(0u - seconds);
			}
			TEXT_LAYER_CELLS(cell)[0] = sign_character;
			colour = 0x64u;
			digit_count = 2u;
			cell += 2u;
			test_mode_draw_number(digit_count, &seconds, &colour, &cell);
			cell += 4u;
			TEXT_LAYER_CELLS(cell)[0] = 0x6653u;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = 0x6645u;
			TEXT_LAYER_CELLS(cell)[1] = 0x6643u;
		}
		stages_left--;
		row_cell += 0x80u;
	} while (stages_left != 0u);
	steering_on = game->steering_wheel_reaction;
	steering_on = steering_on & 1u;
	text = steering_wheel_reaction_texts[steering_on];
	test_mode_draw_texts((uint16_t)steering_on, text);
	advertise_on = game->advertise_sound;
	advertise_on = advertise_on & 1u;
	text = advertise_sound_texts[advertise_on];
	advertise_on = test_mode_draw_texts((uint16_t)advertise_on, text);
	*p_character = advertise_on;
	*p_difficulty_seconds = difficulty_seconds;
}

/* GAME DIFFICULTY: game_difficulty one level harder, after the eighth back to the first; redraws. */
void test_mode_game_assignments_difficulty(void)
{
	uint32_t ignored = 0;

	game->game_difficulty = (game->game_difficulty + 1u) & 7u;
	test_mode_game_assignments_draw(&ignored, &r12);
}

/* TIME DIFFICULTY: the time_difficulty of the stage under the cursor (items 1-6: PRE-STAGE, STAGE 1-4,
   FINAL STAGE) one second more, from +5 back to -5; redraws. */
void test_mode_game_assignments_time(void)
{
	uint8_t seconds = 0;
	uint32_t ignored = 0, stage_item = 0;

	stage_item = game->test_mode_page_cursor;
	seconds = (uint8_t)(game->time_difficulty[stage_item - 1u] + 1u);
	if ((int8_t)seconds > 5)
		seconds = 0xFBu;
	game->time_difficulty[stage_item - 1u] = seconds;
	test_mode_game_assignments_draw(&ignored, &r12);
}

/* STEERING WHEEL REACTION: steering_wheel_reaction on or off (1 or 0); redraws. */
void test_mode_game_assignments_steering(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = game->steering_wheel_reaction > 0u;
	game->steering_wheel_reaction = 1u - carry;
	test_mode_game_assignments_draw(&ignored, &r12);
}

/* ADVERTISE SOUND: advertise_sound on or off (1 or 0); redraws. */
void test_mode_game_assignments_advertise(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = game->advertise_sound > 0u;
	game->advertise_sound = 1u - carry;
	test_mode_game_assignments_draw(&ignored, &r12);
}

/* The game assignments' EXIT: compares game_difficulty, time_difficulty, steering_wheel_reaction and
   advertise_sound with their backup copy 3C bytes on (backup_settings_checksum on, the copy of the
   settings the boot code keeps); if any changed, marks the settings changed (test_mode_flags bit 1);
   back to the menu. */
void test_mode_game_assignments_exit(void)
{
	uint32_t setting_places = 0;

	setting_places = game_assignments_setting_places_ADDRESS;
	current_object = pointer_to(settings_magic_ADDRESS);
	do
	{
		current_object = pointer_to(IN_PAGE(address_of(current_object), TEST_MODE_SETTING_PLACES(setting_places)[0]));
		setting_places++;
		if ((uint8_t)address_of(current_object) == 0u)
		{
			game->test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (BACKED_UP_SETTING(address_of(current_object))->backup == BACKED_UP_SETTING(address_of(current_object))->value);
	game->test_mode_flags |= 2u;
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the COIN ASSIGNMENTS with the cursor on EXIT (test_mode_page_cursor 2):
   test_mode_coin_assignments_show. */
void test_mode_coin_assignments(void)
{
	game->test_mode_page_cursor = 2u;
	test_mode_coin_assignments_show();
}

/* Draws the coin assignments page (COIN/CREDIT SETTING with the setting,
   test_mode_coin_assignments_draw; MANUAL SETTING; EXIT; the prompt) with the cursor where
   test_mode_page_cursor says: on EXIT when the page starts, on MANUAL SETTING when that page
   (test_mode_coin_assignments_manual_start) returns here; then test_mode_coin_assignments_run. */
void test_mode_coin_assignments_show(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, character_2 = 0, colours = 0, text = 0;

	character = test_mode_clear_text();
	text = coin_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	test_mode_coin_assignments_draw(&character_2, &colours, &r12);
	test_mode_draw_instructions(character_2);
	cell = 0x310000u | coin_assignments_cursor_cells[game->test_mode_page_cursor];
	test_mode_set_cursor(character_2, cell);
	game->test_mode_routine = test_mode_coin_assignments_run_ADDRESS;
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

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		item_count = (item_count & ~0xFFu) | 0xCu;
		item_count = (item_count & ~0xFFu) | ((uint8_t)item_count >> 2);
		if ((int8_t)item_count <= (int8_t)page_cursor)
			page_cursor = 0u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | coin_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = pointer_to(settings_magic_ADDRESS);
		switch (game->test_mode_page_cursor)
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
		}
	}
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws the coin/credit setting: its number (coin_setting + 1) and what it gives, from its description
   (coin_setting_descriptions): up to five lines of "n COIN(S) m CREDIT(S)", or a text (FREE PLAY, MANUAL
   SETTING). */
void test_mode_coin_assignments_draw(uint32_t *p_character, uint32_t *p_colours, uint32_t *p_line_cell)
{
	uint32_t zero = 0;
	uint16_t blank_cell = 0;
	uint32_t cell = 0, colours = 0, credits = 0, description = 0, description_byte = 0, digit_count = 0, line_cell = 0, line_text = 0, lines_left = 0, number = 0, plural_offset = 0;

	current_object = pointer_to(settings_magic_ADDRESS);
	cell = TEXT_CELL(32, 9);
	colours = 0x64u;
	digit_count = 2u;
	number = game->coin_setting + 1u;
	test_mode_draw_number(digit_count, &number, &colours, &cell);
	description = coin_setting_descriptions[game->coin_setting];
	blank_cell = 0u;
	line_cell = TEXT_CELL(17, 10);
	lines_left = 5u;
	for (;;)
	{
		description_byte = COIN_SETTING_DESCRIPTION(description)[0];
		zero = description_byte == 0u;
		description++;
		if ((int8_t)description_byte <= 0)
			break;
		colours = (colours & ~0xFFFFu) | 0x6400u;
		cell = line_cell;
		line_text = coin_assignments_line_text_ADDRESS;
		do
		{
			TEXT_LAYER_CELLS(cell)[0] = colours;
			colours = (colours & ~0xFFu) | TEST_MODE_CHARACTERS(line_text)[0];
			cell += 2u;
			line_text++;
		} while ((uint8_t)colours != 0u);
		cell = line_cell;
		colours = (colours & ~0xFFFFu) | 0x6464u;
		digit_count = 1u;
		plural_offset = 6u;
		test_mode_draw_count(description_byte, digit_count, plural_offset, &colours, &cell);
		cell = line_cell + 0x10u;
		credits = COIN_SETTING_DESCRIPTION(description)[0];
		colours = (colours & ~0xFFFFu) | 0x6464u;
		digit_count = 1u;
		plural_offset = 8u;
		description++;
		test_mode_draw_count(credits, digit_count, plural_offset, &colours, &cell);
		lines_left--;
		line_cell += 0x80u;
	}
	if (lines_left != 0u)
	{
		cell = line_cell;
		do
		{
			set_halfwords(cell, 0x11u, blank_cell);
			lines_left--;
			cell += 0x80u;
		} while (lines_left != 0u);
		if (zero == 0u)
		{
			description_byte = 0x6200u;
			for (;;)
			{
				description_byte = (description_byte & ~0xFFu) | COIN_SETTING_DESCRIPTION(description)[0];
				description++;
				if ((uint8_t)description_byte == 0u)
					break;
				TEXT_LAYER_CELLS(line_cell)[0] = description_byte;
				line_cell += 2u;
			}
		}
	}
	*p_character = description_byte;
	*p_colours = colours;
	*p_line_cell = line_cell;
}

/* COIN/CREDIT SETTING: the next of the 26 settings (coin_setting, after FREE PLAY back to */
void test_mode_coin_assignments_next_setting(void)
{
	uint32_t ignored = 0, ignored_2 = 0, setting_number = 0;
	const struct coin_setting_record *setting = NULL;

	do
	{
		setting_number = game->coin_setting;
		setting_number = (uint8_t)(setting_number + 1u);
		if ((int8_t)setting_number >= 0x1A)
			setting_number = 0u;
		game->coin_setting = setting_number;
		setting = rom_pointer(coin_setting_records_ADDRESS + setting_number * 4u);
		if ((int8_t)setting->coins < 0)
		{
			game->coins_per_credit = 1u;
			game->bonus_adder = 0xFFu;
			game->first_chute_multiplier = 1u;
			game->second_chute_multiplier = 1u;
			game->free_play = 0xFFu;
			test_mode_coin_assignments_draw(&ignored, &ignored_2, &r12);
			return;
		}
	} while (setting_number != 0u && game->coins_per_credit == setting->coins && game->bonus_adder == setting->bonus && game->first_chute_multiplier == setting->first_multiplier);
	game->coins_per_credit = setting->coins;
	game->bonus_adder = setting->bonus;
	game->first_chute_multiplier = setting->first_multiplier;
	game->second_chute_multiplier = setting->second_multiplier;
	game->free_play = 0u;
	test_mode_coin_assignments_draw(&ignored, &ignored_2, &r12);
}

/* Opens the coin assignments' MANUAL SETTING page: clears the text, draws its texts (COIN TO CREDIT,
   BONUS ADDER, MULTIPLIER, the table of credits for 1 to 9 coins, EXIT) and the setting
   (test_mode_coin_assignments_manual_draw), the instructions, puts the cursor on EXIT
   (test_mode_page_cursor 3) and goes on in test_mode_coin_assignments_manual_run. */
void test_mode_coin_assignments_manual_start(uint32_t item_count)
{
	uint16_t character = 0, coins_for_credit = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = coin_manual_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	coins_for_credit = test_mode_coin_assignments_manual_draw(item_count);
	test_mode_draw_instructions(coins_for_credit);
	game->test_mode_page_cursor = 3u;
	cell = 0x310000u | coin_manual_exit_cell;
	test_mode_set_cursor(coins_for_credit, cell);
	game->test_mode_routine = test_mode_coin_assignments_manual_run_ADDRESS;
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

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		item_count = 0x10u;
		item_count = item_count >> 2;
		if ((int8_t)item_count <= (int8_t)page_cursor)
			page_cursor = 0u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | coin_manual_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = pointer_to(settings_magic_ADDRESS);
		switch (game->test_mode_page_cursor)
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
		}
	}
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws the manual coin setting: coins_per_credit (N COIN(S) 1 CREDIT), the bonus_adder (N COINS GIVE 1
   EXTRA COIN, NO BONUS ADDER, or FREE PLAY when negative), the first_chute_multiplier (1 COIN COUNTS AS
   N COIN(S)), and the table of what 1 to 9 coins give: each coin counts as the multiplier, the bonus
   adds a coin for every bonus_adder counted (the rest carried on), and the credits (at most 9) show as
   two digits (test_mode_draw_two_digits) with what is left over as a fraction of a credit in the font's
   fraction characters. */
uint16_t test_mode_coin_assignments_manual_draw(uint32_t item_count)
{
	uint32_t bonus_rest = 0, cell = 0, coins_for_bonus = 0, coins_for_credit = 0, counted_coins = 0, credits = 0, digit_count = 0, leftover_coins = 0, multiplier = 0, numerator = 0, plural_offset = 0, rows_left = 0, shown_bonus = 0, shown_multiplier = 0, text = 0, toward_bonus = 0;
	uint32_t saved_leftover_coins = 0;

	current_object = pointer_to(settings_magic_ADDRESS);
	cell = TEXT_CELL(23, 8);
	coins_for_credit = game->coins_per_credit;
	item_count = (item_count & ~0xFFFFu) | 0x6464u;
	digit_count = 1u;
	plural_offset = 6u;
	test_mode_draw_count(coins_for_credit, digit_count, plural_offset, &item_count, &cell);
	shown_bonus = game->bonus_adder;
	if ((int8_t)shown_bonus > 0)
	{
		cell = LINKED(cell, 0xF51Eu);
		item_count = 0x64u;
		digit_count = 1u;
		test_mode_draw_number(digit_count, &shown_bonus, &item_count, &cell);
		text = bonus_adder_text_ADDRESS;
		test_mode_draw_texts((uint16_t)shown_bonus, text);
	}
	else if ((int8_t)shown_bonus < 0)
	{
		text = bonus_free_play_text_ADDRESS;
		test_mode_draw_texts((uint16_t)shown_bonus, text);
	}
	else
	{
		text = no_bonus_adder_text_ADDRESS;
		test_mode_draw_texts((uint16_t)shown_bonus, text);
	}
	cell = TEXT_CELL(32, 12);
	shown_multiplier = game->first_chute_multiplier;
	item_count = (item_count & ~0xFFFFu) | 0x6464u;
	digit_count = 1u;
	plural_offset = 6u;
	test_mode_draw_count(shown_multiplier, digit_count, plural_offset, &item_count, &cell);
	cell = LINKED(cell, 0xF784u);
	coins_for_credit = game->coins_per_credit;
	coins_for_bonus = (int8_t)game->bonus_adder;
	multiplier = game->first_chute_multiplier;
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
			TEXT_LAYER_CELLS(cell)[0] = numerator + 0x6A7Fu;
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = coins_for_credit + 0x6A86u;
			cell += 2u;
		}
		else
		{
			AS32(TEXT_LAYER_CELLS(cell)[0]) = 0x200020u;
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
		TEXT_LAYER_CELLS(cell)[0] = number + 0x6A30u;
		cell += 2u;
	}
	else
	{
		TEXT_LAYER_CELLS(cell)[0] = 0x20u;
		cell += 2u;
	}
	TEXT_LAYER_CELLS(cell)[0] = ones + 0x6A30u;
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
	item_count = (item_count & ~0xFFu) | RAM8(settings_magic_ADDRESS + setting_offset);
	item_count = (item_count & ~0xFFu) | (uint8_t)(item_count + 1u);
	if ((uint8_t)item_count >= 0xAu)
		item_count = (item_count & ~0xFFu) | 1u;
	RAM8(settings_magic_ADDRESS + setting_offset) = item_count;
	if (game->free_play == 0u)
		game->coin_setting = 0x1Au;
	test_mode_coin_assignments_manual_draw(item_count);
}

/* BONUS ADDER: steps bonus_adder through none (0), 2 to 9 and back to none; from free play (FF) it goes
   to none and turns free_play off; the coin setting becomes the manual one (coin_setting 1A); redraws. */
void test_mode_coin_assignments_step_bonus_adder(uint32_t item_count)
{
	uint8_t bonus = 0;
	uint8_t next_bonus = 0;

	bonus = game->bonus_adder;
	if (bonus == 0u)
		bonus++;
	next_bonus = bonus + 1u;
	bonus = next_bonus;
	if (next_bonus == 0u)
		game->free_play = 0u;
	if (bonus >= 0xAu)
		bonus = 0u;
	game->bonus_adder = bonus;
	game->coin_setting = 0x1Au;
	test_mode_coin_assignments_manual_draw(item_count);
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
	game->test_mode_page_cursor = 1u;
	game->test_mode_routine = test_mode_coin_assignments_show_ADDRESS;
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
		current_object = pointer_to(IN_PAGE(address_of(current_object), TEST_MODE_SETTING_PLACES(setting_places)[0]));
		setting_places++;
		if ((uint8_t)address_of(current_object) == 0u)
		{
			game->test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (BACKED_UP_SETTING(address_of(current_object))->backup == BACKED_UP_SETTING(address_of(current_object))->value);
	game->test_mode_flags |= 2u;
	r12 = bookkeeping_clear_credits();
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* Opens the NETWORK ASSIGNMENTS page: its texts, with PRIVILEGE MODE and CABINET ID hidden (colour 6E)
   when the cabinet is stand_alone; test_mode_privilege_mode from link_master; the values
   (test_mode_network_assignments_draw), the instructions, the cursor on EXIT; goes on in
   test_mode_network_assignments_run. */
void test_mode_network_assignments_start(void)
{
	uint16_t character = 0, hidden_colour = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = network_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	current_object = pointer_to(settings_magic_ADDRESS);
	if (game->stand_alone != 0u)
	{
		cell = TEXT_CELL(8, 12);
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, cell);
		cell = TEXT_CELL(8, 14);
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, cell);
	}
	game->test_mode_privilege_mode = game->link_master & 1u;
	character = test_mode_network_assignments_draw();
	test_mode_draw_instructions(character);
	game->test_mode_page_cursor = 3u;
	cell = 0x310000u | network_assignments_exit_cell;
	test_mode_set_cursor(character, cell);
	game->test_mode_routine = test_mode_network_assignments_run_ADDRESS;
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

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = game->test_mode_page_cursor;
		if (game->stand_alone == 0u)
		{
			page_cursor = (uint8_t)(page_cursor + 1u);
			if ((int8_t)page_cursor >= 4)
				page_cursor = 0u;
		}
		else
			page_cursor = page_cursor ^ 3u;
		game->test_mode_page_cursor = page_cursor;
		cell = 0x310000u | network_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = pointer_to(settings_magic_ADDRESS);
		switch (game->test_mode_page_cursor)
		{
		case 0:
			test_mode_network_assignments_toggle_communication();
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
		}
	}
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* Draws the network settings: COMMUNICATION NETWORK or STAND-ALONE (stand_alone), PRIVILEGE MODE SLAVE
   or MASTER (test_mode_privilege_mode), CABINET ID 1-4 (cabinet_id + 1); the last two hidden (colour 6E)
   when stand-alone. */
uint16_t test_mode_network_assignments_draw(void)
{
	uint16_t cabinet_number_then_colour_2 = 0;
	uint32_t cabinet_number_then_colour = 0, cell = 0, colour = 0, digit_count = 0, privilege = 0, stand_alone_bit = 0, text = 0;

	current_object = pointer_to(settings_magic_ADDRESS);
	stand_alone_bit = game->stand_alone;
	stand_alone_bit = stand_alone_bit & 1u;
	text = communication_texts[stand_alone_bit];
	test_mode_draw_texts((uint16_t)stand_alone_bit, text);
	privilege = game->test_mode_privilege_mode;
	text = privilege_mode_texts[privilege];
	test_mode_draw_texts((uint16_t)privilege, text);
	cell = TEXT_CELL(33, 14);
	colour = 0x64u;
	digit_count = 1u;
	cabinet_number_then_colour = game->cabinet_id + 1u;
	test_mode_draw_number(digit_count, &cabinet_number_then_colour, &colour, &cell);
	if (game->stand_alone != 0u)
	{
		cell = TEXT_CELL(28, 12);
		cabinet_number_then_colour_2 = 0x6Eu;
		test_mode_colour_line(cabinet_number_then_colour_2, cell);
		cell = TEXT_CELL(33, 14);
		cabinet_number_then_colour = 0x6Eu;
		cabinet_number_then_colour = test_mode_colour_line((uint16_t)cabinet_number_then_colour, cell);
	}
	return cabinet_number_then_colour;
}

/* COMMUNICATION: switches stand_alone (NETWORK, STAND-ALONE), puts the privilege mode back to MASTER
   (link_master 1, isdn_mode 0, test_mode_privilege_mode 1), shows or hides the PRIVILEGE MODE and
   CABINET ID labels and redraws the values. */
void test_mode_network_assignments_toggle_communication(void)
{
	uint32_t carry = 0;
	uint16_t stand_alone_then_colour = 0;
	uint32_t cell = 0;
	uint32_t saved_colour = 0;

	carry = game->stand_alone > 0u;
	game->stand_alone = 1u - carry;
	game->link_master = 1u;
	game->isdn_mode = 0u;
	game->test_mode_privilege_mode = 1u;
	stand_alone_then_colour = 0x60u;
	if (1u != carry)
		stand_alone_then_colour = 0x6Eu;
	cell = TEXT_CELL(8, 12);
	saved_colour = stand_alone_then_colour;
	test_mode_colour_line(stand_alone_then_colour, cell);
	cell = TEXT_CELL(8, 14);
	stand_alone_then_colour = (uint16_t)saved_colour;
	test_mode_colour_line(stand_alone_then_colour, cell);
	test_mode_network_assignments_draw();
}

/* PRIVILEGE MODE: switches test_mode_privilege_mode between SLAVE and MASTER and sets link_master and
   isdn_mode from it (01E6E5: SLAVE 0 and 0, MASTER 1 and 0; the table's third pair, ISDN, 0 and 1,
   cannot be reached); redraws. */
void test_mode_network_assignments_toggle_privilege_mode(void)
{
	uint32_t privilege = 0;
	const struct privilege_mode_setting *setting = NULL;
	uint8_t stepped_privilege = 0;

	privilege = game->test_mode_privilege_mode;
	stepped_privilege = privilege - 1u;
	privilege = stepped_privilege;
	if ((int8_t)stepped_privilege < 0)
		privilege = 1u;
	game->test_mode_privilege_mode = privilege;
	setting = rom_pointer(privilege_mode_settings_ADDRESS + privilege * 2u);
	game->link_master = setting->master;
	game->isdn_mode = setting->isdn;
	test_mode_network_assignments_draw();
}

/* CABINET ID: steps cabinet_id through 0-3 (shown 1-4); redraws. */
void test_mode_network_assignments_step_cabinet_id(void)
{
	game->cabinet_id = (game->cabinet_id + 1u) & 3u;
	test_mode_network_assignments_draw();
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
		current_object = pointer_to(IN_PAGE(address_of(current_object), TEST_MODE_SETTING_PLACES(setting_places)[0]));
		setting_places++;
		if ((uint8_t)address_of(current_object) == 0u)
		{
			game->test_mode_routine = test_mode_menu_ADDRESS;
			return;
		}
	} while (BACKED_UP_SETTING(address_of(current_object))->backup == BACKED_UP_SETTING(address_of(current_object))->value);
	game->test_mode_flags |= 2u;
	game->test_mode_routine = test_mode_menu_ADDRESS;
}

/* Opens the first BOOKKEEPING page (1/2): its texts, its numbers (test_mode_bookkeeping_draw_totals) and
   the blinking PUSH TEST BUTTON TO CONTINUE (test_mode_show_continue_prompt); goes on in
   test_mode_bookkeeping_wait. */
void test_mode_bookkeeping_start(void)
{
	uint16_t character = 0;
	uint32_t text = 0;

	character = test_mode_clear_text();
	text = bookkeeping_first_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_bookkeeping_draw_totals();
	test_mode_show_continue_prompt();
	game->test_mode_routine = test_mode_bookkeeping_wait_ADDRESS;
	test_mode_bookkeeping_wait(character);
}

/* Blinks the prompt (test_mode_blink_prompt) until TEST, then the second page
   (test_mode_bookkeeping_second_page_start). */
void test_mode_bookkeeping_wait(uint16_t character)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_bookkeeping_second_page_start_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Opens the second BOOKKEEPING page (2/2): its texts, the time histogram and the course records
   (test_mode_bookkeeping_draw_histogram) and the blinking PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt); goes on in test_mode_bookkeeping_second_page_wait. */
void test_mode_bookkeeping_second_page_start(void)
{
	uint16_t character = 0;
	uint32_t character_2 = 0, text = 0;

	character = test_mode_clear_text();
	text = bookkeeping_second_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	test_mode_bookkeeping_draw_histogram(&character_2, &r12);
	test_mode_show_exit_prompt();
	game->test_mode_routine = test_mode_bookkeeping_second_page_wait_ADDRESS;
	test_mode_bookkeeping_second_page_wait((uint16_t)character_2);
}

/* Blinks the prompt until TEST, then back to the menu. */
void test_mode_bookkeeping_second_page_wait(uint16_t character)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Draws the first bookkeeping page's numbers from the bookkeeping in the sound board's shared RAM
   (701F00 on): COIN CHUTE A and B (the coins of each seat's first chute, 701F06 and 701F0A) and TOTAL
   COIN(S); COIN CREDIT(S) (701F02) and SERVICE CREDIT(S) (701F04) and their TOTAL; NUMBER OF GAME(S) A
   and B (701F14, 701F16); TOTAL TIME (701F10, counted by count_total_time) and GAME PLAY TIME A and B
   (701F18, 701F1C) in days, hours, minutes and seconds (test_mode_draw_duration); once a game was
   played, AVERAGE GAME TIME (the play times over the games), LONGEST (701F20) and SHORTEST (701F22) in
   minutes and seconds (test_mode_draw_minutes_seconds). */
uint16_t test_mode_bookkeeping_draw_totals(void)
{
	uint32_t average_time = 0, cell = 0, coin_credits = 0, coin_sum = 0, coins_a = 0, coins_b = 0, colours = 0, credit_sum = 0, digit_count = 0, games_a = 0, games_b = 0, longest_time = 0, play_time_a = 0, play_time_b = 0, plural_offset = 0, service_credits = 0, shortest_time = 0, time_cell = 0, total_coins = 0, total_credits = 0, total_games = 0, total_time = 0;
	uint32_t game_count = 0;

	current_object = pointer_to(BOOKKEEPING_CHECKSUM_ADDRESS);
	cell = TEXT_CELL(26, 5);
	coins_a = BOOKKEEPING_CHUTE_A_COINS;
	coin_sum = coins_a;
	colours = 0x64u;
	digit_count = 5u;
	test_mode_draw_number(digit_count, &coins_a, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF334u;
	coins_b = BOOKKEEPING_BLOCK(address_of(current_object))->chute_b_coins;
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
	coin_credits = BOOKKEEPING_BLOCK(address_of(current_object))->coin_credits;
	credit_sum = coin_credits;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(coin_credits, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF534u;
	service_credits = BOOKKEEPING_BLOCK(address_of(current_object))->service_credits;
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
	games_a = BOOKKEEPING_BLOCK(address_of(current_object))->games_a;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(games_a, digit_count, plural_offset, &colours, &cell);
	cell = (cell & ~0xFFFFu) | 0xF734u;
	games_b = BOOKKEEPING_BLOCK(address_of(current_object))->games_b;
	colours = (colours & ~0xFFFFu) | 0x646Au;
	digit_count = 5u;
	plural_offset = 0xFFFFFFFDu;
	test_mode_draw_count(games_b, digit_count, plural_offset, &colours, &cell);
	time_cell = TEXT_CELL(20, 16);
	total_time = BOOKKEEPING_BLOCK(address_of(current_object))->total_time;
	time_cell = test_mode_draw_duration(total_time, time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xF8A8u;
	play_time_a = BOOKKEEPING_BLOCK(address_of(current_object))->play_time_a;
	time_cell = test_mode_draw_duration(play_time_a, time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xF928u;
	play_time_b = BOOKKEEPING_BLOCK(address_of(current_object))->play_time_b;
	time_cell = test_mode_draw_duration(play_time_b, time_cell);
	games_a = BOOKKEEPING_BLOCK(address_of(current_object))->games_a;
	game_count = BOOKKEEPING_BLOCK(address_of(current_object))->games_b + games_a;
	total_games = game_count;
	if (game_count == 0u)
		return games_a;
	time_cell = (time_cell & ~0xFFFFu) | 0xFA34u;
	average_time = divide_u32(BOOKKEEPING_BLOCK(address_of(current_object))->play_time_a + BOOKKEEPING_BLOCK(address_of(current_object))->play_time_b, total_games);
	test_mode_draw_minutes_seconds(&average_time, &time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xFAB4u;
	longest_time = BOOKKEEPING_BLOCK(address_of(current_object))->longest_play;
	test_mode_draw_minutes_seconds(&longest_time, &time_cell);
	time_cell = (time_cell & ~0xFFFFu) | 0xFB34u;
	shortest_time = BOOKKEEPING_BLOCK(address_of(current_object))->shortest_play;
	test_mode_draw_minutes_seconds(&shortest_time, &time_cell);
	return shortest_time;
}

/* Draws the second bookkeeping page's numbers: the TIME HISTOGRAM, how far the games got (seven counts
   from 701F24: PRE-STAGE, STAGE 1-5, COMPLETE), each with a bar of up to 20 blocks scaled to the
   largest; and OutRunner of OutRunners, the record of each of the ten goals (AUSTRALIA to KENYA; six
   bytes each from 701F32: the car, none when negative, shown by its colour, the three letters of the
   name and the time in hundredths, shown as minutes, seconds and hundredths). */
void test_mode_bookkeeping_draw_histogram(uint32_t *p_last_character, uint32_t *p_cell)
{
	uint32_t carry = 0;
	uint16_t character = 0, stage_games = 0, zero_character = 0;
	uint32_t bar_blocks = 0, bar_cell = 0, bar_scale = 0, car_then_time = 0, cell = 0, colour = 0, colour_name = 0, digit_count = 0, divisor = 0, drawn_counts = 0, games = 0, largest_count = 0, number_cell = 0, record = 0, records_left = 0, rest_time = 0, row_cell = 0, stage_counts = 0, stages_left = 0;
	const struct time_digit_step *digit_step = NULL;
	uint32_t rounded_blocks = 0;

	current_object = pointer_to(BOOKKEEPING_CHECKSUM_ADDRESS);
	stage_counts = BOOKKEEPING_HISTOGRAM_ADDRESS;
	largest_count = 0u;
	stages_left = 7u;
	do
	{
		stage_games = STAGE_GAME_COUNTS(stage_counts)[0];
		stage_counts += 2u;
		if (largest_count < stage_games)
			largest_count = stage_games;
		stages_left--;
	} while (stages_left != 0u);
	bar_scale = 0u;
	if (largest_count != 0u)
		bar_scale = divide_u32(0x50000000u, largest_count);
	drawn_counts = address_of(current_object) + 0x24u;
	cell = TEXT_CELL(13, 5);
	stages_left = 7u;
	do
	{
		games = STAGE_GAME_COUNTS(drawn_counts)[0];
		bar_blocks = games;
		number_cell = cell;
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
			bar_cell = cell + 0xAu;
			do
			{
				TEXT_LAYER_CELLS(bar_cell)[0] = 0x6403u;
				bar_cell += 2u;
				bar_blocks--;
			} while (bar_blocks != 0u);
		}
		stages_left--;
		cell += 0x80u;
	} while (stages_left != 0u);
	row_cell = TEXT_CELL(19, 14);
	record = address_of(current_object) + 0x32u;
	records_left = 0xAu;
	do
	{
		car_then_time = BACKUP_COURSE_RECORD(record)->model;
		record++;
		if ((int8_t)car_then_time < 0)
			record += 5u;
		else
		{
			cell = row_cell;
			colour_name = car_colour_names_ADDRESS + car_then_time * 3u * 2u;
			character = (uint16_t)(0x6400u | TEST_MODE_CHARACTERS(colour_name)[0]);
			TEXT_LAYER_CELLS(cell)[0] = character;
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			colour_name++;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[0]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(colour_name)[1]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			character = (uint16_t)((character & ~0xFFu) | BACKUP_COURSE_RECORD(record - 0x1u)->name[0]);
			cell += 4u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			record++;
			character = (uint16_t)((character & ~0xFFu) | BACKUP_COURSE_RECORD(record - 0x2u)->name[1]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			record++;
			character = (uint16_t)((character & ~0xFFu) | BACKUP_COURSE_RECORD(record - 0x3u)->name[2]);
			cell += 2u;
			TEXT_LAYER_CELLS(cell)[0] = character;
			record++;
			car_then_time = BACKUP_COURSE_RECORD(record - 0x4u)->time;
			zero_character = 0x6430u;
			digit_step = rom_pointer(time_digit_steps_ADDRESS);
			record += 2u;
			cell += 4u;
			do
			{
				rest_time = 0u;
				divisor = digit_step->divisor;
				divux_pair(divisor, &car_then_time, &rest_time);
				TEXT_LAYER_CELLS(cell)[0] = car_then_time + zero_character;
				cell += digit_step->cell_step;
				car_then_time = rest_time;
				digit_step++;
			} while ((int32_t)digit_step->divisor >= 0);
		}
		records_left--;
		row_cell += 0x80u;
	} while (records_left != 0u);
	*p_last_character = car_then_time;
	*p_cell = cell;
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
	seconds = STORED_DURATION(duration - 0x3u)->days * 0x18u;
	duration--;
	seconds = (seconds + STORED_DURATION(duration - 0x2u)->hours) * 0x3Cu;
	duration--;
	seconds = (seconds + STORED_DURATION(duration - 0x1u)->minutes) * 0x3Cu + STORED_DURATION(duration - 0x1u)->seconds;
	return seconds;
}

/* Unused: writes a time of R0 seconds at R10 as two bytes: the seconds (R0 mod 60), then the minutes (R0
   / 60). */
void test_mode_split_minutes_seconds(uint32_t seconds, struct stored_duration *stored_time)
{
	uint32_t remainder = 0;

	remainder = 0u;
	divux_pair(SECONDS_PER_MINUTE, &seconds, &remainder);
	stored_time->seconds = remainder;
	stored_time->minutes = seconds;
}

/* Opens the BACKUP DATA CLEAR page: YES (CLEAR) and NO (CANCEL), the instructions, the cursor on NO;
   goes on in test_mode_backup_data_clear_run. */
void test_mode_backup_data_clear_start(void)
{
	uint16_t character = 0;
	uint32_t cell = 0, text = 0;

	character = test_mode_clear_text();
	text = backup_data_clear_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	game->test_mode_page_cursor = 1u;
	cell = 0x310000u | backup_data_clear_no_cell;
	test_mode_set_cursor(character, cell);
	game->test_mode_routine = test_mode_backup_data_clear_run_ADDRESS;
	test_mode_backup_data_clear_run();
}

/* Each frame of the BACKUP DATA CLEAR page: SERVICE moves the cursor between YES and NO; TEST on NO goes
   back to the menu; TEST on YES leaves the cursor on, clears the bookkeeping (bookkeeping_reset: all of
   it back to its defaults at 011458, zeros but SHORTEST GAME TIME FFFF and no course records, the
   credits too), shows COMPLETED, hides SELECT BY SERVICE BUTTON and blinks PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt) until TEST (test_mode_backup_data_clear_wait). */
void test_mode_backup_data_clear_run(void)
{
	uint16_t hidden_colour = 0;
	uint32_t cell = 0, character = 0, instructions_cell = 0, text = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((game->test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		character = game->test_mode_page_cursor ^ 1u;
		game->test_mode_page_cursor = character;
		cell = 0x310000u | backup_data_clear_cursor_cells[character];
		test_mode_set_cursor(character, cell);
	}
	if ((game->test_mode_buttons_pressed & 2u) == 0u)
	{
		blink_timer = game->test_mode_blink_timer - 1u;
		game->test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			game->test_mode_blink_timer = 5;
			blink_cell = game->test_mode_blink_address;
			cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
			TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
			if (cursor_character != 0u)
				game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
		}
		return;
	}
	if (game->test_mode_page_cursor == 0u)
	{
		TEXT_LAYER_CELLS(game->test_mode_blink_address)[0] = 0x6A7Fu;
		character = bookkeeping_reset(character);
		text = backup_data_clear_completed_text_ADDRESS;
		test_mode_draw_texts((uint16_t)character, text);
		instructions_cell = 0x310000u | test_mode_instructions_text;
		hidden_colour = 0x6Eu;
		test_mode_colour_line(hidden_colour, instructions_cell);
		test_mode_show_exit_prompt();
		game->test_mode_routine = test_mode_backup_data_clear_wait_ADDRESS;
		return;
	}
	game->test_mode_routine = test_mode_menu_ADDRESS;
	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		game->test_mode_blink_timer = 5;
		blink_cell = game->test_mode_blink_address;
		cursor_character = TEXT_LAYER_CELLS(blink_cell)[0] ^ 0x6A7Fu;
		TEXT_LAYER_CELLS(blink_cell)[0] = cursor_character;
		if (cursor_character != 0u)
			game->test_mode_blink_timer = (uint32_t)game->test_mode_blink_timer << 1;
	}
}

/* After the clear: blinks the prompt until TEST, then back to the menu. */
void test_mode_backup_data_clear_wait(void)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
		game->test_mode_routine = test_mode_menu_ADDRESS;
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
	uint16_t character = 0;
	uint32_t text = 0;
	uint32_t flags_before = 0, test_mode_flags_long = 0;

	flags_before = (game->test_mode_flags | ((uint32_t)game->test_mode_cursor << 8u) | ((uint32_t)game->test_mode_page_cursor << 16u) | ((uint32_t)(game->test_mode_routine & 0xFFu) << 24u));
	test_mode_flags_long = flags_before & ~2u;
	game->test_mode_flags = (uint8_t)test_mode_flags_long;
	game->test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	game->test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	game->test_mode_routine = (game->test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	if ((flags_before & 2u) != 0u)
	{
		save_settings();
		if (game->eeprom_write_count < EEPROM_WRITE_LIMIT)
		{
			game->test_mode_on = 0u;
			io_write8(IO_1_CNT, 4u);
			io_write8(IO_2_CNT, 0x80u);
			return;
		}
		character = test_mode_clear_text();
		text = eeprom_caution_text_ADDRESS;
		test_mode_draw_texts(character, text);
		test_mode_show_exit_prompt();
		game->test_mode_routine = test_mode_exit_caution_wait_ADDRESS;
		return;
	}
	game->test_mode_on = 0u;
	io_write8(IO_1_CNT, 4u);
	io_write8(IO_2_CNT, 0x80u);
}

/* Blinks PUSH TEST BUTTON TO EXIT under the CAUTION until TEST, then leaves the test mode as
   test_mode_exit does. */
void test_mode_exit_caution_wait(void)
{
	if ((game->test_mode_buttons_pressed & 2u) != 0u)
	{
		game->test_mode_on = 0u;
		io_write8(IO_1_CNT, 4u);
		io_write8(IO_2_CNT, 0x80u);
		return;
	}
	test_mode_blink_prompt(0u);
}

/* Sets up the test mode's colours and tiles on both screens: the whole palette in the background's
   colour (733), the text palettes (01F304 for screen 1, 01F506 for screen 2: the text colours 60 grey,
   62 red, 64 green, 66 yellow, 68 cyan, 6A white, 6E the background's), 32 palettes after them whose
   colours 1-4 are red, green, blue and white at levels 0-31, and four solid tiles of colours 1-4
   (318020): what the C.R.T. test's colour bars are drawn with. */
uint16_t test_mode_set_up_colours(void)
{
	uint16_t background_colour = 0, level = 0, level_colour = 0, white = 0;
	uint32_t halfword_count = 0, ignored = 0, levels_left = 0, palette_place = 0, rows_left = 0, screen_1_colour = 0, screen_2_colour = 0, tile_place = 0, tile_row = 0, tile_step = 0, tiles_left = 0;

	background_colour = 0x733u;
	set_halfwords(PALETTE_RAM + 0x8000u, 0x4000u, background_colour);
	set_halfwords(0x688000u, 0x4000u, background_colour);
	halfword_count = test_mode_palettes_1_bytes;
	halfword_count = halfword_count >> 1;
	move_halfwords(PALETTE_RAM + 0x9E00u, test_mode_palettes_1_ADDRESS, halfword_count);
	halfword_count = test_mode_palettes_2_bytes;
	halfword_count = halfword_count >> 1;
	move_string(test_mode_palettes_2_ADDRESS, halfword_count, 0x689E00u, halfword_count, 2u, 0u, 0u, 0u, &palette_place, &ignored);
	level = 0u;
	levels_left = 0x20u;
	palette_place = (palette_place & ~0x8000u) + 2u;
	do
	{
		screen_2_colour = palette_place;
		screen_1_colour = screen_2_colour & ~0x80000u;
		level_colour = (uint16_t)level;
		io_write16(screen_1_colour, level_colour);
		io_write16(screen_2_colour, level_colour);
		white = (uint16_t)level_colour;
		level_colour = (uint16_t)((uint32_t)level_colour << 5);
		screen_1_colour += 2u;
		io_write16(screen_1_colour, level_colour);
		screen_2_colour += 2u;
		io_write16(screen_2_colour, level_colour);
		white += level_colour;
		level_colour = (uint16_t)((uint32_t)level_colour << 5);
		screen_1_colour += 2u;
		io_write16(screen_1_colour, level_colour);
		screen_2_colour += 2u;
		io_write16(screen_2_colour, level_colour);
		white += level_colour;
		io_write16(screen_1_colour + 2u, white);
		io_write16(screen_2_colour + 2u, white);
		level++;
		levels_left--;
		palette_place += 0x20u;
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
			TILE_PIXEL_ROWS(tile_place)[0] = tile_row;
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
	test_mode_draw_texts((uint16_t)character, text);
}

/* Makes PUSH TEST BUTTON TO EXIT (the text record at 01F2BC, row 25) the prompt test_mode_blink_prompt
   blinks (test_mode_blink_address), from the next frame. */
void test_mode_show_exit_prompt(void)
{
	game->test_mode_blink_address = exit_prompt_text_ADDRESS;
	game->test_mode_blink_timer = 0;
}

/* Makes PUSH TEST BUTTON TO CONTINUE (the text record at 01F2DE) the prompt test_mode_blink_prompt
   blinks, from the next frame. */
void test_mode_show_continue_prompt(void)
{
	game->test_mode_blink_address = continue_prompt_text_ADDRESS;
	game->test_mode_blink_timer = 0;
}

/* Blinks the prompt whose text record test_mode_blink_address holds: when test_mode_blink_timer runs
   out, draws it (shown 21 frames) if its first cell is empty, else erases it (hidden 11 frames). */
void test_mode_blink_prompt(uint16_t character)
{
	uint32_t cell = 0, text = 0;
	uint8_t blink_timer = 0;

	blink_timer = game->test_mode_blink_timer - 1u;
	game->test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer >= 0)
		return;
	text = game->test_mode_prompt_text;
	cell = 0x310000u | AS16(TEST_MODE_TEXT_BYTES(text)[0]);
	if (TEXT_LAYER_CELLS(cell)[0] == 0u)
	{
		test_mode_draw_texts(character, text);
		game->test_mode_blink_timer = 0x14;
		return;
	}
	do
	{
		TEXT_LAYER_CELLS(cell)[0] = 0u;
		cell += 2u;
	} while (TEXT_LAYER_CELLS(cell)[0] != 0u);
	game->test_mode_blink_timer = 0xA;
}

/* Puts the cursor at text cell R10, the cell two before a menu line: test_mode_blink_address = R10 (the
   cursor, character 7F, blinks there: the menu loops' tail at 01F0AD), test_mode_blink_timer 0, and the
   line in white (6A, test_mode_colour_line). */
uint16_t test_mode_set_cursor(uint32_t character, uint32_t cell)
{
	uint32_t colour = 0;

	game->test_mode_blink_address = cell;
	game->test_mode_blink_timer = 0;
	cell += 4u;
	colour = 0x6Au;
	return test_mode_colour_line((uint16_t)colour, cell);
}

/* Takes the cursor off its line: clears its cell (test_mode_blink_address) and puts the line back in
   grey (60). */
void test_mode_clear_cursor(void)
{
	uint32_t cell = 0, colour = 0;

	cell = game->test_mode_blink_address;
	TEXT_LAYER_CELLS(cell)[0] = 0u;
	cell += 4u;
	colour = 0x60u;
	test_mode_colour_line((uint16_t)colour, cell);
}

/* Draws the text record at R10 on the text layer: lines of a cell (halfword), characters and colour
   changes (a negative byte: its complement is the colour, the cells' high byte), each closed by 0; a 0
   cell ends it. */
uint16_t test_mode_draw_texts(uint16_t character, uint32_t text)
{
	uint32_t cell = 0;

	cell = TEXT_CELL(0, 0);
	for (;;)
	{
		cell = LINKED(cell, AS16(TEST_MODE_TEXT_BYTES(text)[0]));
		text += 2u;
		if ((uint16_t)cell == 0u)
			break;
		for (;;)
		{
			character = (uint16_t)((character & ~0xFFu) | TEST_MODE_TEXT_BYTES(text)[0]);
			text++;
			if ((uint8_t)character == 0u)
				break;
			if ((int8_t)character >= 0)
			{
				TEXT_LAYER_CELLS(cell)[0] = character;
				cell += 2u;
				continue;
			}
			character = (uint16_t)((uint32_t)(uint8_t)~character << 8);
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
	TEXT_LAYER_CELLS(*p_cell)[(int32_t)plural_offset] = colours;
	colours = byte_swap32(colours << 8);
	*p_colours = colours;
	test_mode_draw_number(digit_count, &number, p_colours, p_cell);
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
		TEXT_LAYER_CELLS(cell)[0] = digit;
		if (*p_number == 0u)
		{
			for (;;)
			{
				digit_count--;
				if (digit_count == 0u)
					break;
				cell -= 2u;
				TEXT_LAYER_CELLS(cell)[0] = 0u;
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
	zero_character = (uint16_t)((uint16_t)((uint32_t)colour << 8) | 0x30u);
	for (;;)
	{
		digit = 0u;
		divux_pair(0x10u, &number, &digit);
		digit = (uint16_t)(digit + zero_character);
		if ((int8_t)digit > 0x39)
			digit = (digit & ~0xFFu) | (uint8_t)(digit + 7u);
		cell -= 2u;
		TEXT_LAYER_CELLS(cell)[0] = digit;
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
		TEXT_LAYER_CELLS(cell)[0] = 0u;
	}
	return cell;
}

/* Gives the text from cell R10 up to the next empty cell the colour R0 (60 grey, 6A white, 6E
   hidden...). */
uint16_t test_mode_colour_line(uint16_t colour, uint32_t cell)
{
	uint16_t character = 0;

	colour = (uint16_t)((uint32_t)colour << 8);
	for (;;)
	{
		character = TEXT_LAYER_CELLS(cell)[0] & ~0xE00u;
		TEXT_LAYER_CELLS(cell)[0] = character;
		if (character == 0u)
			break;
		TEXT_LAYER_CELLS(cell)[0] |= colour;
		cell += 2u;
	}
	return colour;
}

/* Clears the text layer (30 rows of 64 cells). */
uint16_t test_mode_clear_text(void)
{
	uint16_t blank_cell = 0, cell_count = 0;

	cell_count = 0x780u;
	blank_cell = 0u;
	set_halfwords(TEXT_CELL(0, 0), 0x780u, blank_cell);
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

	port_e_a = io_read8(IO_1_PORT_E);
	port_e_b = io_read8(IO_2_PORT_E);
	port_f_a = io_read8(IO_1_PORT_F);
	port_f_b = io_read8(IO_2_PORT_F);
	port_f_pressed = (uint8_t)(~port_f_a | ~port_f_b);
	buttons = ((~port_e_a | ~port_e_b) & 0x33u) | ((((uint32_t)port_f_pressed << 4) | (port_f_pressed >> 4)) & 3u);
	previous_buttons = game->test_mode_buttons;
	game->test_mode_buttons = buttons;
	game->test_mode_buttons_previous = previous_buttons;
	game->test_mode_buttons_pressed = ~previous_buttons & buttons;
	game->test_mode_buttons_released = previous_buttons & ~buttons;
	flags_before = (game->test_mode_flags | ((uint32_t)game->test_mode_cursor << 8u) | ((uint32_t)game->test_mode_page_cursor << 16u) | ((uint32_t)(game->test_mode_routine & 0xFFu) << 24u));
	carry = (flags_before & 4u) != 0u;
	test_mode_flags_long = flags_before ^ 4u;
	game->test_mode_flags = (uint8_t)test_mode_flags_long;
	game->test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	game->test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	game->test_mode_routine = (game->test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	bank_then_count = carry;
	converter_input = ANALOG_INPUTS + bank_then_count * 4u;
	analog_input = test_mode_analog_inputs_ADDRESS + bank_then_count * 4u;
	bank_then_count = (uint8_t)(0u - bank_then_count);
	bank_then_count = (uint8_t)(bank_then_count + 2u);
	bank_then_count = (uint8_t)(bank_then_count * 2u);
	do
	{
		reading = 0u;
		io_write8(converter_input, 0xFFu);
		bits_left = 8u;
		do
		{
			converter_bits = io_read8(converter_input);
			carry = (uint8_t)(converter_bits * 2u) < converter_bits;
			reading = (uint8_t)(reading * 2u + carry);
			bits_left--;
		} while (bits_left != 0u);
		ANALOG_POSITIONS(analog_input)[0] = reading;
		bank_then_count--;
		analog_input++;
		converter_input += 2u;
	} while (bank_then_count != 0u);
	carry = (game->test_mode_flags & 4u) != 0u;
	io_write8(ANALOG_BANK, carry);
}

/* Sends the test mode's next queued sound request (test_mode_sound_queue, test_mode_sound_queue_read) to
   the sound CPU, one a frame: to screen 2's command byte (SOUND_COMMAND_2) when the request has its high
   byte set, else to screen 1's (SOUND_COMMAND_1). */
void test_mode_send_sound(void)
{
	uint8_t inverted_command = 0;
	uint32_t read_index = 0;
	uint16_t inverted_request = 0;

	read_index = game->test_mode_sound_queue_read;
	if (game->test_mode_sound_queue_write == read_index)
		return;
	inverted_request = ~game->test_mode_sound_queue[read_index];
	inverted_command = (uint8_t)inverted_request;
	if ((int16_t)inverted_request >= 0)
		SOUND_COMMAND_2 = ~inverted_command;
	else
		SOUND_COMMAND_1[0] = ~inverted_command;
	game->test_mode_sound_queue_read = (read_index + 1u) & 0xFu;
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

	write_index = game->test_mode_sound_queue_write;
	game->test_mode_sound_queue[write_index] = request;
	game->test_mode_sound_queue_write = (write_index + 1u) & 0xFu;
}
