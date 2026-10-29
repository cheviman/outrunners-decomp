/**
 *    Name:        system/main.c
 *    Description: the machine's main program: the boot and its checks, the second task, the frame wait, the start of a game, a screen and the board, the reset
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
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
		mem_write8(destination, BOOT_REGISTER_VALUES(source)[0]);
		source++;
		destination++;
		mem_write8(destination, BOOT_REGISTER_VALUES(source)[0]);
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
		io_write8(destination, BOOT_REGISTER_VALUES(source)[0]);
		source++;
		destination++;
		io_write8(destination, BOOT_REGISTER_VALUES(source)[0]);
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
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		block_count--;
	} while (block_count != 0u);
	io_write32(LINK_BOARD_ON, 0u);
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
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
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
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		CLEARED_RAM_WORDS(clear_address)[0] = fill;
		CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
		clear_address += 8u;
		block_count--;
	} while (block_count != 0u);
	move_halfwords(WORK_RAM + 0x1FF84u, second_task_start_frame_ADDRESS, 0x3Eu);
	player_car = 0x219000u;
	waiting_task_frame = 0x21FF84u;
	waiting_task_stack = 0x21F800u;
	initialise_game();
	lcd_start();
	io_write8(IRQ_ACKNOWLEDGE, 0u);
	io_write8(IRQ_D00005, 0u);
	interrupts_on();
	network_check();
	second_task(1u);
}

/* Returns the CRC-16 (polynomial 1021) of the R11 bytes at R10, worked out in the top half of a 32-bit
   register that starts at DEBDEB00, each byte entering its low byte, with eight more shifts after the
   last. Used for the EEPROM's settings (load_settings, eeprom_write_settings) and the program's
   signature (signature_checksum). */
uint16_t crc16(uint32_t bytes, uint32_t byte_count)
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
uint16_t signature_checksum(void)
{
	uint32_t signature_length = 0, signature_text = 0;

	signature_text = signature_text_ADDRESS;
	signature_length = 0x30u;
	return crc16(signature_text, signature_length);
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
					io_write16(IRQ_TIMER_0, 0x39u);
					io_write16(IRQ_TIMER_1, 1u);
					screen_2_vblanks_high = 0u;
					screen_2_vblanks = 0u;
					screen_1_vblanks_high = 0u;
					screen_1_vblanks = 0u;
					vblanks_since_start = 0u;
					task_swapping = 0xFFu;
					attract_car_list = first_attract_car_lists[machine_count];
					input_changes = SCREEN(SCREEN_2)->car_number;
					SCREEN(SCREEN_1)->next_attract_car = FIRST_ATTRACT_CARS(attract_car_list)[SCREEN(SCREEN_1)->car_number];
					SCREEN(SCREEN_2)->next_attract_car = FIRST_ATTRACT_CARS(attract_car_list)[(int32_t)input_changes];
					io_write8(IO_1_CNT, 6u);
					io_write8(IO_2_CNT, 0x86u);
				}
				do
				{
					L_000719 = false;
					if (second_task_waiting == 0u)
						link_send_records();
					wait_for_frame(input_changes);
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
					}
					run_objects();
					lcd_update();
				} while (single_task == 0u);
				if (L_00077F)
					break;
				break;
			}
			if (L_00077F)
				break;
			do
			{
				second_task_waiting = 0xFFu;
				link_send_records();
				wait_for_frame(input_changes);
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
		io_write8(IO_1_CNT, 4u);
		io_write8(IO_2_CNT, 0x80u);
		fill = 0u;
		fill_high = 0u;
		block_count = 0x100u;
		clear_address = GAME_VARIABLES;
		do
		{
			CLEARED_RAM_WORDS(clear_address)[0] = fill;
			CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
			clear_address += 8u;
			CLEARED_RAM_WORDS(clear_address)[0] = fill;
			CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
			clear_address += 8u;
			CLEARED_RAM_WORDS(clear_address)[0] = fill;
			CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
			clear_address += 8u;
			CLEARED_RAM_WORDS(clear_address)[0] = fill;
			CLEARED_RAM_WORDS(clear_address)[1] = fill_high;
			clear_address += 8u;
			block_count--;
		} while (block_count != 0u);
		move_halfwords(WORK_RAM + 0x1FF84u, second_task_start_frame_ADDRESS, 0x3Eu);
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

	if (mode == 0u || mode == 6u)
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
			SEAT_CREDITS(seat_credits)->credits--;
			SEAT_CREDITS(seat_credits)->bonus_units = 0u;
		}
		SCREEN(screen)->start_lamp_blink = 0u;
		port_d_outputs[index] &= ~4u;
		play_mode = 6u;
		AS32(SCREEN(screen)->game_mode) = 6u;
		SCREEN(screen)->link_event = 1u;
		race_record_clear();
		RACE(SCREEN(screen)->race)->entry_request = 0xFFu;
		return play_mode;
	}
	seat_credits = seat_credits_addresses[index];
	if (SEAT_CREDITS(seat_credits)->credits == 0u)
		return mode;
	lamp_blink = (uint8_t)(SCREEN(screen)->start_lamp_blink + 1u);
	SCREEN(screen)->start_lamp_blink = lamp_blink;
	port_d_outputs[index] = (port_d_outputs[index] & ~4u) | ((lamp_blink & 8u) >> 1);
	if ((SCREEN(screen)->buttons_pressed & 0x40u) == 0u)
		return mode;
	if (free_play == 0u)
	{
		SEAT_CREDITS(seat_credits)->credits--;
		SEAT_CREDITS(seat_credits)->bonus_units = 0u;
	}
	SCREEN(screen)->start_lamp_blink = 0u;
	port_d_outputs[index] &= ~4u;
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

	io_write8(EVENT_CODE, 0x13u);
	io_write8(EVENT_CODE, 0u);
	io_write8(EVENT_CODE, 0u);
	io_write8(EVENT_CODE, 0x1Fu);
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

/* The V70's reset vector, at the top of the program's address space: a jump to boot. */
void reset(void)
{
	boot();
}
