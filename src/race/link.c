/**
 *    Name:        race/link.c
 *    Description: the link between the cabinets: the ISDN connection, the network check at boot, the race's entries and progress, the course records, the records sent and received
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Never runs in the scenarios (ISDN only): the players on this side and on the other (race_copy
   players_this_side, players_other_side) into isdn_players_this_side and isdn_players_other_side, their
   sum into player_count (the race's players) and half of it into machine_count (its cabinets). */
void isdn_count_players(void)
{
	uint8_t player_sum = 0, players_this_side = 0;

	players_this_side = RACE(SCREEN(screen)->race_copy)->players_this_side;
	player_sum = RACE(SCREEN(screen)->race_copy)->players_other_side;
	game->isdn_players_this_side = players_this_side;
	game->isdn_players_other_side = player_sum;
	player_sum += players_this_side;
	game->player_count = player_sum;
	game->machine_count = player_sum >> 1;
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
	uint8_t link_status = 0;
	uint32_t colour_block = 0, handler_list = 0, lamp_place = 0;
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
		SCREEN_LAMPS(lamp_place)->checkpoint_all = 0u;
		SCREEN_LAMPS(lamp_place)->leader_all = 0u;
		clear_lcd();
		handler_list = isdn_connection_objects_ADDRESS;
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
   SUCCESS, THIS SIDE, OTHER SIDE and TOTAL with the players (race_copy players_this_side,
   players_other_side and their sum, print_hex_byte), then 1; bits 2-3 clear ISDN FAILED, then 1; else
   ISDN CONECTING (sic) blinking every 8 frames (its blink_counter counting); 1 nothing more. */
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
	}
	link_status = SCREEN(screen)->link_status;
	if ((link_status & 1u) == 0u)
	{
		if ((link_status & 0xCu) == 0u)
		{
			OBJECT(current_object)->state++;
			text_record = isdn_failed_text;
			print_text_layer(text_record);
			return;
		}
		blink_frames = OBJECT(current_object)->isdn_status.blink_counter;
		blink_frames = (uint8_t)(blink_frames + 1u);
		OBJECT(current_object)->isdn_status.blink_counter = blink_frames;
		blink_frames = blink_frames >> 3;
		text_record = isdn_connecting_texts[blink_frames & 1u];
		print_text_layer(text_record);
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

/* The network check after the test mode: network_check, or in ISDN mode (isdn_mode) the link board off
   for a moment, the link's variables (from machine_count), the board's memory at 800004 and 800600, this
   machine's two race records and all the players' records cleared, then network_check_isdn. */
void network_check_again(void)
{
	uint32_t delay = 0;

	if (game->isdn_mode == 0u)
	{
		network_check();
		return;
	}
	io_write16(LINK_BOARD_ON, 0u);
	r12 = 0x800000u;
	delay = BOARD_OFF_LOOPS;
	do
	{
		delay--;
	} while (delay != 0u);
	move_halfwords(machine_count_ADDRESS, zero_block_ADDRESS, 0x20u);
	move_halfwords(r12 + 4u, zero_block_ADDRESS, 0x10u);
	move_halfwords(r12 + 0x600u, zero_block_ADDRESS, 0x400u);
	move_halfwords(RACE_1, zero_block_ADDRESS, 0x80u);
	move_halfwords(RACE_COPY_1, zero_block_ADDRESS, 0x200u);
	network_check_isdn();
}

/* The network check at boot: the link board off (801000), the link's variables, the board's first bytes
   and all the race records cleared; with the STAND-ALONE setting (stand_alone into link_off) only
   network_check_stand_alone. Else it shows NETWORK CHECK (MASTER, SLAVE or ISDN) and BOARD CHECK, tests
   the board's memory (network_check_board_memory), writes its greeting V70 and turns the board on, waits
   for it (network_check_wait_board), checks the board's answer Z80 (network_check_greeting), waits
   again, shows the board's self-test (network_check_four_ics); in ISDN mode goes on to
   network_check_isdn; else gives the board this machine's role (link_master at 800004), waits, shows the
   self-test again (network_check_five_ics), waits for the board's flag (801002 bit 7) with CHECK NETWORK
   STATUS blinking, gives it this machine's number (800002 to 800E20) and FF at 800006, shows CHECK TRANS
   END until the board's 800008 is set (link_lost if 80000C is), and goes on to network_check_join. The
   test button (test_mode_on) gives it up at any wait (link_failed FF). */
void network_check(void)
{
	uint32_t left_caller = 0, text = 0;
	uint8_t board_status = 0;
	uint32_t board_flags_greeted = 0, board_flags_role = 0, board_flags_status = 0;

	io_write16(LINK_BOARD_ON, 0u);
	r12 = 0x800000u;
	move_halfwords(machine_count_ADDRESS, zero_block_ADDRESS, 0x20u);
	move_halfwords(r12, zero_block_ADDRESS, 8u);
	move_halfwords(RACE_1, zero_block_ADDRESS, 0x80u);
	move_halfwords(RACE_COPY_1, zero_block_ADDRESS, 0x200u);
	game->link_off = game->stand_alone;
	if (game->link_off != 0u)
	{
		network_check_stand_alone();
		return;
	}
	io_write8(LINK_BOARD_ON, 0u);
	text = network_check_master_text_ADDRESS;
	if (game->link_master == 0u)
		text = network_check_slave_text_ADDRESS;
	if (game->isdn_mode != 0u)
		text = network_check_isdn_text_ADDRESS;
	print_text_layer(text);
	text = board_check_text_ADDRESS;
	print_text_layer(text);
	left_caller = 0u;
	left_caller = network_check_board_memory(left_caller);
	if (left_caller != 0u)
		return;
	io_write16(r12, 0x56u);
	io_write16(r12 + 2u, 0x37u);
	io_write16(r12 + 4u, 0x30u);
	io_write8(r12 + 0x14u, 0u);
	io_write16(LINK_BOARD_FLAGS, 0u);
	io_write16(LINK_BOARD_ON, 1u);
	left_caller = 0u;
	left_caller = network_check_wait_board(left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	left_caller = network_check_greeting(left_caller);
	if (left_caller != 0u)
		return;
	board_flags_greeted = io_read32(LINK_BOARD_FLAGS);
	io_write32(LINK_BOARD_FLAGS, board_flags_greeted | 1u);
	wait_for_frame();
	left_caller = 0u;
	left_caller = network_check_wait_board(left_caller);
	if (left_caller != 0u)
		return;
	io_write8(r12 + 0xCu, 0u);
	left_caller = 0u;
	left_caller = network_check_four_ics(left_caller);
	if (left_caller != 0u)
		return;
	if (game->isdn_mode != 0u)
	{
		network_check_isdn();
		return;
	}
	io_write8(r12 + 4u, game->link_master);
	board_flags_role = io_read32(LINK_BOARD_FLAGS);
	io_write32(LINK_BOARD_FLAGS, board_flags_role & ~1u);
	wait_for_frame();
	left_caller = 0u;
	left_caller = network_check_wait_board(left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	left_caller = network_check_five_ics(left_caller);
	if (left_caller != 0u)
		return;
	board_flags_status = io_read32(LINK_BOARD_FLAGS);
	io_write32(LINK_BOARD_FLAGS, board_flags_status | 1u);
	do
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
		{
			game->link_failed = 0xFFu;
			return;
		}
		current_object = check_network_status_text_ADDRESS;
		network_check_blink_text();
		board_status = io_read8(LINK_BOARD_FLAGS);
	} while ((int8_t)board_status >= 0);
	network_check_clear_status_line();
	io_write8(r12 + 0xE20u, io_read8(r12 + 2u));
	io_write8(r12 + 6u, 0xFFu);
	text = check_trans_end_text_ADDRESS;
	print_text_layer(text);
	do
	{
		if (game->test_mode_on != 0u)
		{
			game->link_failed = 0xFFu;
			return;
		}
		if (io_read8(r12 + 0xCu) != 0u)
		{
			link_lost();
			return;
		}
	} while (io_read8(r12 + 8u) == 0u);
	network_check_clear_status_line();
	game->link_wait_frames = 0u;
	network_check_join();
}

/* Takes the link from the board: machine_count from its byte at 800000, player_count twice it, this
   machine's number from 800002, the board's flag 801002 set; with no machine on the link (and not in
   ISDN mode) network_check_stand_alone; else the two screens' car numbers from the cabinet ID
   (cabinet_id * 2 and + 1, into screen car_number and race car_number), then network_check_result with
   this machine's number. */
void network_check_join(void)
{
	uint8_t car_number = 0, count = 0;
	uint32_t machine = 0;

	count = io_read8(r12);
	game->machine_count = count;
	count = (uint8_t)(count * 2u);
	game->player_count = count;
	machine = io_read8(r12 + 2u);
	io_write8(r12 + 0x1002u, 1u);
	if (game->isdn_mode == 0u && count < 2u)
	{
		network_check_stand_alone();
		return;
	}
	car_number = (uint8_t)(game->cabinet_id * 2u);
	game->screen_1.car_number = car_number;
	game->races[0].car_number = car_number;
	car_number++;
	game->screen_2.car_number = car_number;
	game->races[1].car_number = car_number;
	network_check_result(machine);
}

/* The network check's ISDN mode: the link board off for a moment, 1 into its byte at 800014 (0 in
   network mode), its flag clear, the board on, network_check_wait_isdn, then network_check_join. */
void network_check_isdn(void)
{
	uint32_t delay = 0, left_caller = 0;

	io_write16(LINK_BOARD_ON, 0u);
	delay = BOARD_OFF_LOOPS;
	do
	{
		delay--;
	} while (delay != 0u);
	io_write8(r12 + 0x14u, 1u);
	io_write8(LINK_BOARD_FLAGS, 0u);
	io_write8(LINK_BOARD_ON, 1u);
	left_caller = 0u;
	left_caller = network_check_wait_isdn(left_caller);
	if (left_caller != 0u)
		return;
	network_check_join();
}

/* The link has failed (the board's error byte 80000C, seen by network_check, link_receive_records or
   link_send_records): link_failed 1, then network_check_stand_alone (which shows COMMUNICATION FAILED.
   for 128 frames). */
void link_lost(void)
{
	game->link_failed = 1u;
	network_check_stand_alone();
}

/* Sets the machine up alone: link_off, one machine and two players (machine_count, player_count), car
   numbers 0 and 1 for the two screens (screen and race car_number), then network_check_result as machine
   1. */
void network_check_stand_alone(void)
{
	uint32_t machine = 0;

	game->link_off = 1u;
	machine = 1u;
	game->player_count = 2u;
	game->machine_count = 1u;
	game->screen_1.car_number = 0u;
	game->races[0].car_number = 0u;
	game->screen_2.car_number = 1u;
	game->races[1].car_number = 1u;
	network_check_result(machine);
}

/* Sets up the machine's two players from its number R0 (1-4): both race records' machine, their players
   ((R0 - 1) * 2 and + 1, also screen player), the screens' race_copy (race_copy_offsets by player:
   20FC00 + 80 each) and race (20FB00, 20FB80); machine 1 also puts its settings (advertise_sound,
   steering_wheel_reaction, coins_per_credit, first_chute_multiplier, bonus_adder, coin_setting,
   free_play, game_difficulty, time_difficulty[0-5]) at 03-10 of its first record (the master_ fields;
   second_chute_multiplier is not sent) and its course records (the backup's, 701F32) at 30. Then NETWORK
   CHECK and the result for 128 frames: COMMUNICATION FAILED. (link_failed), THIS MACHINE IS ISDN MODE.,
   THIS MACHINE IS STAND-ALONE., or SLAVE / MASTER CONTROLLER (the board's 800004) with COMMUNICATION
   SUCCESS. and THIS MACHINE ID IS n (the board's 800002); the text cleared. */
void network_check_result(uint32_t machine)
{
	uint32_t frames_left = 0, race = 0, second_player = 0, text = 0;
	uint32_t saved_frames_left = 0;
	uint8_t isdn_machine_id = 0, board_master = 0, slave_machine_id = 0, master_machine_id = 0;

	game->races[0].machine = machine;
	game->races[1].machine = machine;
	machine = (uint8_t)(machine - 1u);
	machine = (uint8_t)(machine * 2u);
	game->races[0].player = machine;
	second_player = machine + 1u;
	game->races[1].player = second_player;
	game->screen_1.player = machine;
	game->screen_2.player = second_player;
	game->screen_1.race_copy = race_copy_offsets[machine] + GAME_VARIABLES;
	game->screen_2.race_copy = race_copy_offsets[second_player] + GAME_VARIABLES;
	race = RACE_1;
	game->screen_1.race = RACE_1;
	game->screen_2.race = RACE_2;
	if (machine == 0u)
	{
		RACE(race)->master_advertise_sound = game->advertise_sound;
		RACE(race)->master_steering_wheel_reaction = game->steering_wheel_reaction;
		RACE(race)->master_coins_per_credit = game->coins_per_credit;
		RACE(race)->master_first_chute_multiplier = game->first_chute_multiplier;
		RACE(race)->master_bonus_adder = game->bonus_adder;
		RACE(race)->master_coin_setting = game->coin_setting;
		RACE(race)->master_free_play = game->free_play;
		RACE(race)->master_game_difficulty = game->game_difficulty;
		RACE(race)->master_time_difficulty_0 = game->time_difficulty[0];
		RACE(race)->master_time_difficulty_1 = game->time_difficulty[1];
		RACE(race)->master_time_difficulty_2_5 = ((uint8_t)game->time_difficulty[2] | ((uint32_t)(uint8_t)game->time_difficulty[3] << 8u) | ((uint32_t)(uint8_t)game->time_difficulty[4] << 16u) | ((uint32_t)(uint8_t)game->time_difficulty[5] << 24u));
		move_halfwords(race + 0x30u, BOOKKEEPING_COURSE_RECORDS_ADDRESS, 0x1Eu);
	}
	clear_text_layer();
	text = network_check_text_ADDRESS;
	print_text_layer(text);
	if (game->link_failed != 0u)
	{
		text = communication_failed_text_ADDRESS;
		print_text_layer(text);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			wait_for_frame();
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	if (game->isdn_mode != 0u)
	{
		text = isdn_machine_text_ADDRESS;
		print_text_layer(text);
		text = communication_success_text_ADDRESS;
		print_text_layer(text);
		text = machine_id_text_ADDRESS;
		print_text_layer(text);
		isdn_machine_id = io_read8(LINK_MACHINE_NUMBER);
		MACHINE_ID_TEXT_CELL = (isdn_machine_id + 0x30u) | 0x6000u;
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			wait_for_frame();
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	if (game->link_off != 0u)
	{
		text = stand_alone_machine_text_ADDRESS;
		print_text_layer(text);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			wait_for_frame();
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	board_master = io_read8(LINK_MASTER);
	if (board_master == 0u)
	{
		text = slave_machine_text_ADDRESS;
		print_text_layer(text);
		text = communication_success_text_ADDRESS;
		print_text_layer(text);
		text = machine_id_text_ADDRESS;
		print_text_layer(text);
		slave_machine_id = io_read8(LINK_MACHINE_NUMBER);
		MACHINE_ID_TEXT_CELL = (slave_machine_id + 0x30u) | 0x6000u;
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			wait_for_frame();
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	text = master_machine_text_ADDRESS;
	print_text_layer(text);
	text = communication_success_text_ADDRESS;
	print_text_layer(text);
	text = machine_id_text_ADDRESS;
	print_text_layer(text);
	master_machine_id = io_read8(LINK_MACHINE_NUMBER);
	MACHINE_ID_TEXT_CELL = (master_machine_id + 0x30u) | 0x6000u;
	frames_left = 0x80u;
	do
	{
		saved_frames_left = frames_left;
		wait_for_frame();
		frames_left = saved_frames_left - 1u;
	} while (frames_left != 0u);
	clear_text_layer();
}

/* Tests the link board's memory (network_check_memory_test over 7FF of its bytes); on a fault the
   screens off, CONNECTION ERROR and network_check_error. */
uint32_t network_check_board_memory(uint32_t left_caller)
{
	uint8_t outcome = 0;
	uint32_t board_memory = 0, count_then_faults = 0, error_texts = 0;

	board_memory = 0x800000u;
	count_then_faults = 0x7FFu;
	count_then_faults = network_check_memory_test(count_then_faults, board_memory);
	outcome = 2u;
	if (count_then_faults != 0u)
		outcome = 3u;
	if (outcome == 3u)
	{
		io_write8(IO_1_CNT, 4u);
		io_write8(IO_2_CNT, 0x80u);
		error_texts = connection_error_texts_ADDRESS;
		return network_check_error(error_texts);
	}
	return left_caller;
}

/* A memory test: eight passes (seed 01, 02, ... 80), each writing a pattern from the seed into R1 bytes
   (one a halfword) from R10, reading them back and gathering the bits that differ into R0's low byte (0:
   no fault); CHECK DUALPORT RAM flips on the status line after each pass
   (network_check_toggle_memory_text). */
uint8_t network_check_memory_test(uint32_t count, uint32_t memory)
{
	uint32_t carry = 0;
	uint8_t faults = 0, pattern = 0, rotation = 0, seed = 0;
	uint16_t address_bits = 0;
	uint32_t byte_count = 0, bytes_left = 0, cell = 0;
	uint8_t byte_read = 0;
	uint16_t written_low_15_bits = 0, written_low_12_bits = 0, written_low_9_bits = 0, written_low_6_bits = 0, read_low_15_bits = 0, read_low_12_bits = 0, read_low_9_bits = 0, read_low_6_bits = 0;

	byte_count = count;
	seed = 1u;
	faults = 0u;
	do
	{
		cell = memory;
		pattern = (uint8_t)seed;
		bytes_left = byte_count;
		do
		{
			io_write16(cell, pattern);
			cell += 2u;
			rotation = 6u;
			written_low_15_bits = cell * 2u;
			address_bits = written_low_15_bits;
			if (written_low_15_bits != 0u)
			{
				rotation--;
				written_low_12_bits = (uint32_t)address_bits << 3;
				address_bits = written_low_12_bits;
				if (written_low_12_bits != 0u)
				{
					rotation--;
					written_low_9_bits = (uint32_t)address_bits << 3;
					address_bits = written_low_9_bits;
					if (written_low_9_bits != 0u)
					{
						rotation--;
						written_low_6_bits = (uint32_t)address_bits << 3;
						address_bits = written_low_6_bits;
						if (written_low_6_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, rotation, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		cell = memory;
		pattern = (uint8_t)seed;
		bytes_left = byte_count;
		do
		{
			byte_read = io_read8(cell);
			cell++;
			if (byte_read != pattern)
				faults = faults ^ pattern ^ io_read8(cell - 1u);
			cell++;
			rotation = 6u;
			read_low_15_bits = cell * 2u;
			address_bits = read_low_15_bits;
			if (read_low_15_bits != 0u)
			{
				rotation--;
				read_low_12_bits = (uint32_t)address_bits << 3;
				address_bits = read_low_12_bits;
				if (read_low_12_bits != 0u)
				{
					rotation--;
					read_low_9_bits = (uint32_t)address_bits << 3;
					address_bits = read_low_9_bits;
					if (read_low_9_bits != 0u)
					{
						rotation--;
						read_low_6_bits = (uint32_t)address_bits << 3;
						address_bits = read_low_6_bits;
						if (read_low_6_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, rotation, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		network_check_toggle_memory_text();
		carry = v60_rot_cy(seed, 1u, 8);
		seed = (uint8_t)(((uint32_t)seed << 1) | (seed >> 7));
	} while (carry == 0u);
	return faults;
}

/* Waits for the link board's flag (801002 bit 7), WAIT blinking, for up to E10 frames (two minutes,
   link_wait_frames): then the screens off, COMMUNICATION TIME OUT and network_check_error; the test
   button gives the check up (link_failed FF); the status line cleared after. */
uint32_t network_check_wait_board(uint32_t left_caller)
{
	uint32_t error_texts = 0, left_caller_2 = 0;
	uint16_t frames_left = 0;
	uint32_t board_flags = 0;

	game->link_wait_frames = 0xE10u;
	do
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
		{
			game->link_failed = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		frames_left = game->link_wait_frames;
		game->link_wait_frames = frames_left - 1u;
		if (frames_left == 1u)
		{
			io_write8(IO_1_CNT, 4u);
			io_write8(IO_2_CNT, 0x80u);
			error_texts = communication_time_out_texts_ADDRESS;
			return network_check_error(error_texts);
		}
		board_flags = io_read32(LINK_BOARD_FLAGS);
	} while ((board_flags & 0x80u) == 0u);
	network_check_clear_status_line();
	return left_caller;
}

/* ISDN mode: waits, WAIT blinking, until the link board's byte 800000 (its machine count) is set, then
   60 frames more (link_wait_frames); the test button gives the check up (link_failed FF); the status
   line cleared after. */
uint32_t network_check_wait_isdn(uint32_t left_caller)
{
	uint32_t left_caller_2 = 0;
	uint8_t board_machines = 0;
	uint16_t frames_left = 0;

	do
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
		{
			game->link_failed = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		board_machines = io_read8(0x800000u);
	} while (board_machines == 0u);
	game->link_wait_frames = 0x3Cu;
	do
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
		{
			game->link_failed = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		frames_left = game->link_wait_frames;
		game->link_wait_frames = frames_left - 1u;
	} while (frames_left != 1u);
	network_check_clear_status_line();
	return left_caller;
}

/* Unused: a busy wait: counts 3000 down to 0 and returns. It sits between network_check_wait_isdn and
   network_check_greeting. */
void network_check_delay(void)
{
	uint32_t delay = 0;

	delay = 0x3000u;
	do
	{
		delay--;
	} while (delay != 0u);
}

/* Checks the greetings on the link board: V70 (this program's, at 800000) and Z80 (the board's answer,
   at 800010); otherwise the screens off, CONNECTION ERROR and network_check_error. */
uint32_t network_check_greeting(uint32_t left_caller)
{
	uint32_t answer_letters = 0, board_answer = 0, error_texts = 0;

	board_answer = 0x10u + r12;
	answer_letters = ((((uint32_t)(uint8_t)io_read16(board_answer) << 8) | (uint8_t)io_read16(board_answer + 2u)) << 8) | (uint8_t)io_read16(board_answer + 4u);
	if ((((((uint32_t)(uint8_t)io_read16(r12) << 8) | (uint8_t)io_read16(r12 + 2u)) << 8) | (uint8_t)io_read16(r12 + 4u)) != 0x563730u || answer_letters != 0x5A3830u)
	{
		io_write8(IO_1_CNT, 4u);
		io_write8(IO_2_CNT, 0x80u);
		error_texts = connection_error_texts_ADDRESS;
		return network_check_error(error_texts);
	}
	return left_caller;
}

/* The link board's self-test (its byte 80000C), the second time: clear, IC17, IC23, IC11, IC10 and IC9
   GOOD (network_check_show_ics); else the screens off, COMMUNICATION BOARD ERROR and
   network_check_four_ics, which, the byte still set, shows four of the ICs and halts there (the five
   after it only show if the byte clears meanwhile), then network_check_halt. */
uint32_t network_check_five_ics(uint32_t left_caller)
{
	uint8_t self_test = 0;
	uint32_t error_texts = 0, ic_count = 0, left_caller_2 = 0;

	self_test = io_read8(r12 + 0xCu);
	if (self_test == 0u)
	{
		ic_count = 5u;
		network_check_show_ics(ic_count, self_test);
		return left_caller;
	}
	io_write8(IO_1_CNT, 4u);
	io_write8(IO_2_CNT, 0x80u);
	clear_text_layer();
	error_texts = communication_board_error_texts_ADDRESS;
	print_text_layer_list(error_texts);
	left_caller_2 = 0u;
	left_caller_2 = network_check_four_ics(left_caller_2);
	if (left_caller_2 != 0u)
		return left_caller_2;
	ic_count = 5u;
	self_test = io_read8(r12 + 0xCu);
	network_check_show_ics(ic_count, self_test);
	io_write8(IO_1_CNT, 6u);
	io_write8(IO_2_CNT, 0x86u);
	return network_check_halt();
}

/* The link board's self-test (its byte 80000C), the first time: clear, IC17, IC23, IC11 and IC10 GOOD
   (network_check_show_ics); else the screens off, COMMUNICATION BOARD ERROR with each of the four GOOD
   or BAD, the screens on, and network_check_halt. */
uint32_t network_check_four_ics(uint32_t left_caller)
{
	uint8_t self_test = 0;
	uint32_t error_texts = 0, ic_count = 0;

	self_test = io_read8(r12 + 0xCu);
	if (self_test == 0u)
	{
		ic_count = 4u;
		network_check_show_ics(ic_count, self_test);
		return left_caller;
	}
	io_write8(IO_1_CNT, 4u);
	io_write8(IO_2_CNT, 0x80u);
	clear_text_layer();
	error_texts = communication_board_error_texts_ADDRESS;
	print_text_layer_list(error_texts);
	self_test = io_read8(r12 + 0xCu);
	ic_count = 4u;
	network_check_show_ics(ic_count, self_test);
	io_write8(IO_1_CNT, 6u);
	io_write8(IO_2_CNT, 0x86u);
	return network_check_halt();
}

/* Shows R0 of the link board's ICs (IC17, IC23, IC11, IC10, IC9), each GOOD or BAD by its bit in R2
   (link_board_ic_texts). */
void network_check_show_ics(uint32_t ic_count, uint8_t self_test)
{
	uint32_t ic = 0, text = 0, text_index = 0;

	ic = 0u;
	do
	{
		text_index = (uint8_t)(ic * 2u);
		if ((self_test & shl32(1u, (int)(ic))) != 0u)
			text_index = (uint8_t)(text_index + 1u);
		text = link_board_ic_texts[text_index];
		print_text_layer(text);
		ic = (uint8_t)(ic + 1u);
		ic_count--;
	} while (ic_count != 0u);
}

/* Blinks a text record (at current_object: its cell, attribute and characters) by flipping it into the
   text layer every 16th call (network_check_blink_counter): WAIT, CHECK NETWORK STATUS, NETWORK ERROR. */
void network_check_blink_text(void)
{
	uint16_t character = 0;
	uint32_t cell = 0;

	game->network_check_blink_counter++;
	if ((game->network_check_blink_counter & 0xFu) != 0u)
		return;
	cell = 0x310000u | 0xF000u | TEXT_LAYER_RECORD(current_object)->cell;
	current_object = current_object + 2u;
	character = (uint16_t)((uint32_t)TEXT_LAYER_RECORD(current_object - 0x2u)->attribute << 8);
	current_object = current_object + 1u;
	for (;;)
	{
		character = (uint16_t)((character & ~0xFFu) | TEXT_CHARACTERS(current_object)[0]);
		current_object = current_object + 1u;
		if ((uint8_t)character == 0u)
			break;
		TEXT_LAYER_CELLS(cell)[0] ^= character;
		cell += 2u;
	}
}

/* Clears row 24 of the text layer, where the network check's waits blink their texts. */
void network_check_clear_status_line(void)
{
	uint32_t cell = 0, cells_left = 0;

	cell = TEXT_CELL(0, 24);
	cells_left = 0x28u;
	do
	{
		TEXT_LAYER_CELLS(cell)[0] = 0u;
		cells_left--;
		cell += 2u;
	} while (cells_left != 0u);
}

/* Flips CHECK DUALPORT RAM on or off on the status line (row 24), once each pass of
   network_check_memory_test. */
void network_check_toggle_memory_text(void)
{
	uint16_t character = 0;
	uint32_t memory_text = 0, next_cell = 0;
	uint32_t cell = 0;

	character = 0x6000u;
	next_cell = TEXT_CELL(8, 24);
	memory_text = check_dualport_ram_characters_ADDRESS;
	do
	{
		character = (uint16_t)((character & ~0xFFu) | TEXT_CHARACTERS(memory_text)[0]);
		cell = next_cell;
		next_cell += 2u;
		TEXT_LAYER_CELLS(cell)[0] ^= character;
		memory_text++;
	} while (TEXT_CHARACTERS(memory_text)[0] != 0u);
}

/* Shows the error R19 (a text list: CONNECTION ERROR, COMMUNICATION TIME OUT...) on a cleared text layer
   with the screens on, then halts like network_check_halt. */
uint32_t network_check_error(uint32_t error_texts)
{
	uint32_t left_caller = 0;

	clear_text_layer();
	print_text_layer_list(error_texts);
	io_write8(IO_1_CNT, 6u);
	io_write8(IO_2_CNT, 0x86u);
	for (;;)
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
			break;
		current_object = network_error_text_ADDRESS;
		network_check_blink_text();
	}
	game->link_failed = 0xFFu;
	left_caller = 1u;
	return left_caller;
}

/* Blinks NETWORK ERROR until the test button (test_mode_on) is pressed; then link_failed FF and the
   network check is given up (it returns past its caller). */
uint32_t network_check_halt(void)
{
	uint32_t left_caller = 0;

	for (;;)
	{
		wait_for_frame();
		if (game->test_mode_on != 0u)
			break;
		current_object = network_error_text_ADDRESS;
		network_check_blink_text();
	}
	game->link_failed = 0xFFu;
	left_caller = 1u;
	return left_caller;
}

/* After the network check (unless the test button is down): sends and receives the race records
   (link_send_records, 5 frames, link_receive_records; in ISDN mode once the board's 8006E1 is set,
   link_isdn_receive_records), keeps each player's car number (race car_number into car_numbers), takes
   the first player's (the master's) course records into the backup (701F32 with its checksum, 011418,
   and the copy at 701FAE) and into course_records (001D84), and adopts its settings (record +03-10 into
   advertise_sound, steering_wheel_reaction, coins_per_credit, first_chute_multiplier, bonus_adder,
   coin_setting, free_play, game_difficulty, time_difficulty[0-5]; second_chute_multiplier is not taken;
   011018 and 01139E when any of the coin settings changes, all saved by 00036B when any changes). */
void link_take_master_settings(void)
{
	uint16_t record_size = 0;
	uint32_t car_number_slot = 0, coins_changed = 0, players_left = 0, race = 0, record_car_number = 0;
	uint8_t isdn_records_ready = 0;
	uint32_t time_difficulty_2_long = 0;

	if (game->test_mode_on != 0u)
		return;
	link_send_records();
	wait_for_frame();
	wait_for_frame();
	wait_for_frame();
	wait_for_frame();
	wait_for_frame();
	for (;;)
	{
		wait_for_frame();
		if (game->isdn_mode == 0u)
			break;
		isdn_records_ready = io_read8(LINK_ISDN_RECORDS_READY);
		if (isdn_records_ready != 0u)
		{
			link_isdn_receive_records();
			break;
		}
	}
	link_receive_records();
	record_car_number = 0x20FC02u;
	car_number_slot = car_numbers_ADDRESS;
	players_left = game->player_count;
	record_size = 0x80u;
	do
	{
		PLAYER_CAR_NUMBERS(car_number_slot)[0] = RACE(record_car_number - 0x2u)->car_number;
		record_car_number = LINKED(record_car_number, (uint16_t)(record_car_number + record_size));
		car_number_slot++;
		players_left--;
	} while (players_left != 0u);
	move_halfwords(BOOKKEEPING_COURSE_RECORDS_ADDRESS, WORK_RAM + 0xFC30u, 0x1Eu);
	bookkeeping_update_checksum((uint16_t)players_left);
	move_halfwords(SOUND_RAM + 0x1FAEu, WORK_RAM + 0xFC30u, 0x1Eu);
	r12 = load_course_records();
	race = RACE_COPY_1;
	coins_changed = 0u;
	if (game->coin_setting != game->race_copies[0].master_coin_setting || game->coins_per_credit != RACE(race)->master_coins_per_credit || game->first_chute_multiplier != RACE(race)->master_first_chute_multiplier || game->bonus_adder != RACE(race)->master_bonus_adder || game->free_play != RACE(race)->master_free_play)
	{
		coins_changed = 0xFFFFFFFFu;
		game->coin_setting = RACE(race)->master_coin_setting;
		game->coins_per_credit = RACE(race)->master_coins_per_credit;
		game->first_chute_multiplier = RACE(race)->master_first_chute_multiplier;
		game->bonus_adder = RACE(race)->master_bonus_adder;
		game->free_play = RACE(race)->master_free_play;
		init_coins();
		r12 = bookkeeping_clear_credits();
	}
	if (game->advertise_sound == RACE(race)->master_advertise_sound && game->steering_wheel_reaction == RACE(race)->master_steering_wheel_reaction && game->game_difficulty == RACE(race)->master_game_difficulty && ((uint8_t)game->time_difficulty[0] | ((uint32_t)(uint8_t)game->time_difficulty[1] << 8u)) == ((uint8_t)RACE(race)->master_time_difficulty_0 | ((uint32_t)RACE(race)->master_time_difficulty_1 << 8u)) && ((uint8_t)game->time_difficulty[2] | ((uint32_t)(uint8_t)game->time_difficulty[3] << 8u) | ((uint32_t)(uint8_t)game->time_difficulty[4] << 16u) | ((uint32_t)(uint8_t)game->time_difficulty[5] << 24u)) == RACE(race)->master_time_difficulty_2_5 && coins_changed == 0u)
		return;
	game->advertise_sound = RACE(race)->master_advertise_sound;
	game->steering_wheel_reaction = RACE(race)->master_steering_wheel_reaction;
	game->game_difficulty = RACE(race)->master_game_difficulty;
	game->time_difficulty[0] = RACE(race)->master_time_difficulty_0;
	game->time_difficulty[1] = RACE(race)->master_time_difficulty_1;
	time_difficulty_2_long = RACE(race)->master_time_difficulty_2_5;
	game->time_difficulty[2] = (uint8_t)time_difficulty_2_long;
	game->time_difficulty[3] = (uint8_t)(time_difficulty_2_long >> 8u);
	game->time_difficulty[4] = (uint8_t)(time_difficulty_2_long >> 16u);
	game->time_difficulty[5] = (uint8_t)(time_difficulty_2_long >> 24u);
	save_settings();
}

/* The entries to a race, each frame. Player 0's screen keeps them in its record: once every entry is
   taken (it means to wait for each record's entry_serial_seen, but compares player 0's own each time), a
   player's entry_request not yet accepted (accepted_players) is accepted: entrant = that player,
   entry_serial + 1, and the first one starts entry_frames at 600 (entry_frames_by_machines), cut to 40
   once every player is in; entry_frames counting down to 0 closes the entries (entrant FF, entry_serial
   + 1, accepted_players cleared, 1 to the link board's 80000A). Every screen: when some player's race is
   over (race game_over) an idle screen (game steps 1-5) goes to step 5 (cleared_on_race_over cleared),
   and while player 0's entry_frames runs to step 7 (entry_invitation_step); each new entry_serial of
   player 0's record (entry_serial_seen) adds its entrant to the screen's entrants (entrant_count), or
   the close clears them (and entry_accepted); the screen's own player accepted: its entry_request
   cleared, entry_accepted, the first entrant into host_player and is_host when that is itself; the host
   keeps racer_count and race_order in its record from the entrants. */
uint32_t race_entries_update(uint32_t entry_place)
{
	uint32_t zero = 0;
	uint8_t entrant = 0, entrants = 0, entry_serial = 0;
	uint16_t next_entry_frames = 0, record_size = 0;
	uint32_t entrant_count = 0, entrant_list = 0, entry_frames = 0, machines = 0, other_race = 0, player = 0, players_left = 0, race = 0, race_2 = 0, request = 0;
	uint8_t game_over = 0;
	uint16_t old_entry_frames = 0;
	uint32_t entrants_2 = 0, race_order_long = 0;

	race = SCREEN(screen)->race;
	race_2 = RACE_COPY_1;
	if (SCREEN(screen)->player == 0u)
	{
		players_left = game->player_count;
		record_size = 0x80u;
		entry_serial = RACE(race)->entry_serial;
		other_race = race_2 + 0x1Cu;
		entry_place = screen + 0x300u;
		entry_frames = RACE(race)->entry_frames;
		do
		{
			players_left--;
		} while ((players_left != 0u) & (RACE(other_race - 0x1Cu)->entry_serial_seen == entry_serial));
		if (RACE(other_race - 0x1Cu)->entry_serial_seen == entry_serial)
		{
			if (entry_frames == 0u)
			{
				machines = 0u;
				if (game->stand_alone == 0u)
					machines = game->machine_count;
				next_entry_frames = entry_frames_by_machines[machines];
				request = race_2 + 0x19u;
				players_left = game->player_count;
				player = 0u;
				for (;;)
				{
					if (SCREEN(entry_place - 0x300u)->accepted_players[(int32_t)player] != 0u || RACE(request - 0x19u)->entry_request == 0u)
					{
						player = (uint8_t)(player + 1u);
						request = LINKED(request, (uint16_t)(request + record_size));
						players_left--;
						if (players_left == 0u)
							break;
						continue;
					}
					SCREEN(entry_place - 0x300u)->accepted_players[(int32_t)player] = 0xFFu;
					RACE(race)->entrant = player;
					RACE(race)->entry_serial++;
					entry_frames = next_entry_frames;
					if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != game->player_count || entry_frames <= 0x28u)
						break;
					entry_frames = 0x28u;
					break;
				}
			}
			else
			{
				old_entry_frames = entry_frames;
				entry_frames = (uint16_t)(old_entry_frames - 1u);
				next_entry_frames = (uint16_t)entry_frames;
				if (old_entry_frames != 1u)
				{
					request = race_2 + 0x19u;
					players_left = game->player_count;
					player = 0u;
					for (;;)
					{
						if (SCREEN(entry_place - 0x300u)->accepted_players[(int32_t)player] != 0u || RACE(request - 0x19u)->entry_request == 0u)
						{
							player = (uint8_t)(player + 1u);
							request = LINKED(request, (uint16_t)(request + record_size));
							players_left--;
							if (players_left == 0u)
								break;
							continue;
						}
						SCREEN(entry_place - 0x300u)->accepted_players[(int32_t)player] = 0xFFu;
						RACE(race)->entrant = player;
						RACE(race)->entry_serial++;
						entry_frames = next_entry_frames;
						if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != game->player_count || entry_frames <= 0x28u)
							break;
						entry_frames = 0x28u;
						break;
					}
				}
				else
				{
					RACE(race)->entry_serial++;
					RACE(race)->entrant = 0xFFu;
					AS32(SCREEN(entry_place - 0x300u)->accepted_players[0]) = 0u;
					entry_place += 4u;
					AS32(SCREEN(entry_place - 0x304u)->accepted_players[4]) = 0u;
					io_write8(LINK_ENTRIES_CLOSED, 1u);
					entry_place += 4u;
				}
			}
		}
		RACE(race)->entry_frames = entry_frames;
	}
	players_left = game->player_count;
	other_race = race_2 + 0x28u;
	do
	{
		game_over = RACE(other_race - 0x28u)->game_over;
		zero = game_over == 0u;
		other_race += 0x80u;
		players_left--;
	} while ((players_left != 0u) & (game_over == 0u));
	if (zero)
	{
		if (RACE(race_2)->entry_frames != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	else if (SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
	{
		AS32(SCREEN(screen)->game_mode) = 5u;
		game->cleared_on_race_over = 0u;
		if (RACE(race_2)->entry_frames != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	if (RACE(race_2)->entry_serial == RACE(race)->entry_serial_seen)
		return entry_place;
	RACE(race)->entry_serial_seen = RACE(race_2)->entry_serial;
	entrant = RACE(race_2)->entrant;
	if ((int8_t)entrant < 0)
	{
		SCREEN(screen)->entrant_count = 0u;
		SCREEN(screen)->entry_accepted = 0u;
		return entry_place;
	}
	entrant_list = screen + 0x30Fu;
	entrant_count = SCREEN(entrant_list - 0x30Fu)->entrant_count;
	entrant_count = (uint8_t)(entrant_count + 1u);
	SCREEN(entrant_list - 0x30Fu)->entrant_count = entrant_count;
	SCREEN(entrant_list - 0x30Fu)->entrants[(int32_t)(entrant_count - 1u)] = entrant;
	if (SCREEN(screen)->player == entrant)
	{
		RACE(race)->entry_request = 0u;
		SCREEN(screen)->entry_accepted = 0xFFu;
		entrants = SCREEN(entrant_list - 0x30Fu)->entrants[0];
		SCREEN(screen)->host_player = entrants;
		if (SCREEN(screen)->player == entrants)
			SCREEN(screen)->is_host = 0xFFu;
	}
	if (SCREEN(screen)->entry_accepted != 0u && SCREEN(screen)->is_host != 0u)
	{
		entry_place = race + 0x1Fu;
		RACE(entry_place - 0x1Fu)->racer_count = SCREEN(entrant_list - 0x30Fu)->entrant_count;
		entry_place++;
		entrant_list++;
		entrants_2 = AS32(SCREEN(entrant_list - 0x310u)->entrants[4]);
		race_order_long = AS32(SCREEN(entrant_list - 0x310u)->entrants[0]);
		RACE(entry_place - 0x20u)->race_order = (uint8_t)race_order_long;
		RACE(entry_place - 0x20u)->race_order_1 = (uint8_t)(race_order_long >> 8u);
		RACE(entry_place - 0x20u)->race_order_2 = (uint8_t)(race_order_long >> 16u);
		RACE(entry_place - 0x20u)->race_order_3 = (uint8_t)(race_order_long >> 24u);
		RACE(entry_place - 0x20u)->race_order_4_7 = entrants_2;
		entry_place += 8u;
	}
	return entry_place;
}

/* The race's progress, each frame of a race (screen in_game). The host (is_host) runs it in its record:
   once every racer has taken the last pass (pass_seen = pass_count), a racer whose stage_count changed
   (against counted_stages) has passed a checkpoint (stage_count - 1): pass_player, pass_checkpoint,
   pass_order (counted_passes), pass_branch (the side of the road it was on: race road_x bit 15),
   pass_time (race_time in hundredths plus the order), pass_count + 1; the first to pass a checkpoint
   before the goal adds the time of the stage its branch leads to (stage_start_times by
   fork_next_stages[stage * 2 + branch]) plus the setting for that checkpoint (master_time_difficulty) to
   time_left. Once the race has started (race_started) race_time + 34 and time_left - 1 a frame, and one
   pass of sorting race_order: the finished (race finished) first by finish_time, then by distance (race
   segment_fraction-43). Every screen from the host's record (host_record): the leader's segment and
   stage (leader_window_segment, leader_stage), each player's place (places, its own in place); each new
   pass: pass_seen (screen and race), checkpoints_passed, checkpoint_passes, the order and the time by
   player and by order (checkpoint_orders, checkpoint_order, checkpoint_times_by_player,
   checkpoint_times); the first car past a fork decides the route: forks_decided + 1, its branch in
   route_branches and the stage it leads to in route_stages. */
void race_progress_update(uint32_t *p_host_record, uint32_t *p_racer_copy)
{
	uint32_t racer_copy = *p_racer_copy;
	uint8_t branch = 0, host_pass_count = 0, pass_branch = 0, pass_count = 0, place = 0, player = 0;
	uint16_t order = 0, pass_time = 0;
	uint32_t ahead_player = 0, ahead_race = 0, checked_race = 0, checkpoint = 0, checkpoint_offset = 0, comparisons_left = 0, counted_stages = 0, fork_index = 0, host_order = 0, host_racer_count = 0, next_player = 0, order_entry = 0, order_place = 0, pass_checkpoint = 0, pass_order = 0, pass_player = 0, passing_race = 0, race = 0, race_2 = 0, race_3 = 0, racer = 0, racer_count = 0, racers_left = 0, sort_place = 0;
	uint8_t racer_total = 0;
	uint32_t swapped_player = 0;

	race = SCREEN(screen)->race;
	race_2 = SCREEN(screen)->host_record;
	if (SCREEN(screen)->in_game != 0u)
	{
		if (SCREEN(screen)->is_host != 0u)
		{
			host_order = race_2 + 0x1Fu;
			host_racer_count = RACE(host_order - 0x1Fu)->racer_count;
			host_pass_count = RACE(race)->pass_count;
			host_order++;
			order_entry = host_order;
			racers_left = host_racer_count;
			do
			{
				checked_race = race_copy_offsets[RACE_ORDER_PLAYERS(order_entry)[0]] + GAME_VARIABLES;
				racers_left--;
				order_entry++;
			} while ((racers_left != 0u) & (RACE(checked_race)->pass_seen == host_pass_count));
			if (RACE(checked_race)->pass_seen == host_pass_count)
			{
				order_entry = host_order;
				counted_stages = screen + 0x330u;
				racers_left = host_racer_count;
				do
				{
					racer = RACE_ORDER_PLAYERS(order_entry)[0];
					passing_race = race_copy_offsets[racer] + GAME_VARIABLES;
					racers_left--;
					order_entry++;
				} while ((racers_left != 0u) & (SCREEN(counted_stages - 0x330u)->counted_stages[(int32_t)racer] == RACE(passing_race)->stage_count));
				if (SCREEN(counted_stages - 0x330u)->counted_stages[(int32_t)racer] != RACE(passing_race)->stage_count)
				{
					checkpoint = RACE(passing_race)->stage_count;
					SCREEN(counted_stages - 0x330u)->counted_stages[(int32_t)racer] = checkpoint;
					RACE(race)->pass_player = racer;
					checkpoint = (uint8_t)(checkpoint - 1u);
					RACE(race)->pass_checkpoint = checkpoint;
					order = SCREEN(screen)->counted_passes[(int32_t)checkpoint];
					SCREEN(screen)->counted_passes[(int32_t)checkpoint]++;
					RACE(race)->pass_order = (uint8_t)order;
					branch = (uint8_t)((uint16_t)RACE(passing_race)->road_x >> 15);
					RACE(race)->pass_branch = branch;
					RACE(race)->pass_time = RACE(race)->race_time / 0xAu + order;
					RACE(race)->pass_count++;
					if (order == 0u && checkpoint != 5u)
					{
						fork_index = RACE(passing_race)->stage;
						fork_index = (uint8_t)(fork_index * 2u);
						fork_index = (uint8_t)(fork_index + branch);
						RACE(race)->time_left += (int8_t)(stage_start_times[fork_next_stages[fork_index]] + game->master_time_difficulty[checkpoint]) * 0x1Eu;
					}
				}
			}
			if (SCREEN(screen)->race_started != 0u)
			{
				RACE(race)->race_time += 0x22u;
				if (RACE(race)->time_left != 0u)
					RACE(race)->time_left--;
			}
			sort_place = race + 0x1Fu;
			racer_count = RACE(sort_place - 0x1Fu)->racer_count;
			racer_total = racer_count;
			comparisons_left = (uint8_t)(racer_total - 1u);
			sort_place++;
			if ((int8_t)racer_total > 1)
			{
				ahead_player = RACE(sort_place - 0x20u)->race_order;
				ahead_race = race_copy_offsets[ahead_player] + GAME_VARIABLES;
				sort_place++;
				if (SCREEN(screen)->race_started != 0u)
				{
					do
					{
						next_player = RACE_ORDER_PLAYERS(sort_place)[0];
						race_3 = race_copy_offsets[next_player] + GAME_VARIABLES;
						sort_place++;
						if (RACE(ahead_race)->finished != 0u)
						{
							if (RACE(race_3)->finished == 0u || RACE(ahead_race)->finish_time <= RACE(race_3)->finish_time)
							{
								swapped_player = next_player;
								next_player = ahead_player;
								ahead_player = swapped_player;
								ahead_race = race_3;
							}
						}
						else if ((RACE(race_3)->segment_fraction | ((uint32_t)RACE(race_3)->segment << 16u)) <= (RACE(ahead_race)->segment_fraction | ((uint32_t)RACE(ahead_race)->segment << 16u)))
						{
							swapped_player = next_player;
							next_player = ahead_player;
							ahead_player = swapped_player;
							ahead_race = race_3;
						}
						RACE_ORDER_PLAYERS(sort_place)[-2] = next_player;
						comparisons_left--;
					} while (comparisons_left != 0u);
					RACE_ORDER_PLAYERS(sort_place)[-1] = ahead_player;
				}
			}
		}
		order_place = race_2 + 0x1Fu;
		racers_left = RACE(order_place - 0x1Fu)->racer_count;
		place = 0u;
		order_place++;
		racer = RACE(order_place - 0x20u)->race_order;
		player = SCREEN(screen)->player;
		racer_copy = race_copy_offsets[racer] + GAME_VARIABLES;
		SCREEN(screen)->leader_window_segment = RACE(racer_copy)->window_segment;
		SCREEN(screen)->leader_stage = RACE(racer_copy)->stage;
		order_place++;
		do
		{
			SCREEN(screen)->places[(int32_t)racer] = place;
			if (racer == player)
				SCREEN(screen)->place = place;
			racer = RACE_ORDER_PLAYERS(order_place)[0];
			place++;
			racers_left--;
			order_place++;
		} while (racers_left != 0u);
		if (SCREEN(screen)->pass_seen != RACE(race_2)->pass_count)
		{
			pass_count = RACE(race_2)->pass_count;
			SCREEN(screen)->pass_seen = pass_count;
			RACE(race)->pass_seen = pass_count;
			pass_player = RACE(race_2)->pass_player;
			pass_checkpoint = RACE(race_2)->pass_checkpoint;
			pass_order = RACE(race_2)->pass_order;
			pass_branch = RACE(race_2)->pass_branch;
			pass_time = RACE(race_2)->pass_time;
			checkpoint_offset = pass_checkpoint * 2u;
			SCREEN(screen)->checkpoints_passed[(int32_t)pass_player]++;
			SCREEN(screen)->checkpoint_passes[(int32_t)pass_checkpoint]++;
			SCREEN(screen)->checkpoint_orders[(int32_t)(checkpoint_offset * 4u + pass_player)] = pass_order;
			SCREEN(screen)->checkpoint_order[(int32_t)(checkpoint_offset * 4u + pass_order)] = pass_player;
			checkpoint_offset = checkpoint_offset * 2u;
			SCREEN(screen)->checkpoint_times_by_player[(int32_t)(checkpoint_offset * 2u + pass_player)] = pass_time;
			SCREEN(screen)->checkpoint_times[(int32_t)(checkpoint_offset * 2u + pass_order)] = pass_time;
			if (pass_order == 0u && pass_checkpoint != 5u)
			{
				SCREEN(screen)->forks_decided++;
				SCREEN(screen)->route_branches[(int32_t)pass_checkpoint] = pass_branch;
				fork_index = RACE(GAME_VARIABLES + race_copy_offsets[pass_player])->stage;
				fork_index = (uint8_t)(fork_index * 2u);
				SCREEN(screen)->route_stages[(int32_t)pass_checkpoint] = fork_next_stages[(uint8_t)(fork_index + pass_branch)];
			}
		}
	}
	*p_host_record = race_2;
	*p_racer_copy = racer_copy;
}

/* Game step 7, an idle seat while another one's game takes entries (race_entries_update sends it here),
   by screen game_mode's byte 2: 0 screen fade_kind = 0 and flags bit 0; 1 once that bit has cleared: the
   seat's sound channel (012004), the screen's objects (011C60), layers (01070C), sprite count (010889)
   and text layer (01076A) set afresh, its race record's race part cleared (race_record_clear), not the
   demo (demo_flags bit 8), the wheel motor off (wheel_motor), both of the seat's lamps blinking (bytes
   C0-C3 and C8-CB of its entry at checkpoint_lamp); 2 starts entry_invitation_object_handlers
   (entry_countdown_start, credit_display_update, coin_prompt_update): the invitation to join with the
   entry time left; 3 the seat's screen on (its I/O chip's CNT, 6 or 86), fade_kind = 1, flags bit 0; 4
   once player 0's entry_frames is over: the sound channel again (012004), step 2, the lamps off, the LCD
   cleared (clear_lcd). */
void entry_invitation_step(void)
{
	uint32_t index = 0, lamp = 0, object_list = 0;

	switch (SCREEN(screen)->game_step)
	{
	case 0:
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 1:
		break;
	case 2:
		SCREEN(screen)->game_step++;
		object_list = entry_invitation_objects_ADDRESS;
		start_object_list(object_list);
		return;
	case 3:
		if (SCREEN(screen)->index != 0u)
			io_write8(IO_2_CNT, 0x86u);
		else
			io_write8(IO_1_CNT, 6u);
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 4:
		if (game->race_copies[0].entry_frames != 0u)
			return;
		reset_screen_sound();
		AS32(SCREEN(screen)->game_mode) = 2u;
		lamp = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		SCREEN_LAMPS(lamp)->checkpoint_all = 0u;
		SCREEN_LAMPS(lamp)->leader_all = 0u;
		clear_lcd();
		return;
	}
	if ((SCREEN(screen)->flags & 1u) == 0u)
	{
		SCREEN(screen)->game_step++;
		reset_screen_sound();
		clear_objects();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		race_record_clear();
		SCREEN(screen)->demo_flags &= ~0x100u;
		index = SCREEN(screen)->index;
		game->wheel_motor[index] = 0u;
		lamp = GAME_VARIABLES + index * 4u;
		SCREEN_LAMPS(lamp)->checkpoint = 2u;
		SCREEN_LAMPS(lamp)->leader = 8u;
		SCREEN_LAMPS(lamp)->checkpoint_blinks = 0xFFu;
		SCREEN_LAMPS(lamp)->checkpoint_period = 0x10u;
		SCREEN_LAMPS(lamp)->checkpoint_timer = 1u;
		SCREEN_LAMPS(lamp)->leader_blinks = 0xFFu;
		SCREEN_LAMPS(lamp)->leader_period = 0x10u;
		SCREEN_LAMPS(lamp)->leader_timer = 1u;
	}
}

/* Clears bytes 30-7F of the screen's race record (the race's part: the car's published state, the
   checkpoint passes, the times), from the zeros at 100800. */
void race_record_clear(void)
{
	move_halfwords(SCREEN(screen)->race + 0x30u, zero_block_ADDRESS, 0x28u);
}

/* Marks the screen's player ready in its race record (ready), after its selections. */
void set_player_ready(void)
{
	RACE(SCREEN(screen)->race)->ready = 0xFFu;
}

/* Whether a player of the race has not marked itself ready yet (zero set): byte 5C of each one's
   race_copy record. */
uint32_t players_not_ready(void)
{
	uint32_t zero = 0;
	uint32_t race = 0, racers_left = 0;
	uint8_t ready = 0;

	race = SCREEN(screen)->host_record + 0x1Fu;
	racers_left = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		ready = RACE(RACE_COPY_1 + race_copy_player_offsets[RACE_ORDER_PLAYERS(race)[0]])->ready;
		zero = ready == 0u;
		racers_left--;
		race++;
	} while ((racers_left != 0u) & (ready != 0u));
	return zero;
}

/* Takes the players' new course records, each frame: when a player's record_serial changes (against
   name_entries_seen), its time (finish_time) on its course (record_course) at or under the course's
   record (course_records, 6 bytes a course: model, the three letters of the name, the time) replaces it
   with its record_model and record_name (the first letter) and record_name_rest (the other two), also in
   the two copies in the backup (701F32 with its checksum, 011418, and 701FAE). */
uint32_t course_records_update(uint32_t record_entry)
{
	uint16_t finish_time = 0;
	uint32_t backup_record = 0, ignored = 0, players_left = 0, race = 0, record_offset = 0, seen_serials = 0;
	uint8_t seen_serial = 0;

	seen_serials = screen + 0x328u;
	players_left = game->player_count;
	race = RACE_2;
	do
	{
		race += 0x80u;
		seen_serial = SEEN_RECORD_SERIALS(seen_serials)[0];
		seen_serials++;
		if (RACE(race)->record_serial != seen_serial)
		{
			SEEN_RECORD_SERIALS(seen_serials)[-1] = RACE(race)->record_serial;
			finish_time = RACE(race)->finish_time;
			record_offset = RACE(race)->record_course;
			record_offset = (uint8_t)(record_offset * 6u);
			record_entry = course_records_ADDRESS + record_offset;
			if (finish_time <= COURSE_RECORD_ENTRY(record_entry)->time)
			{
				COURSE_RECORD_ENTRY(record_entry)->time = finish_time;
				COURSE_RECORD_ENTRY(record_entry)->model = RACE(race)->record_model;
				COURSE_RECORD_ENTRY(record_entry)->name = RACE(race)->record_name;
				COURSE_RECORD_ENTRY(record_entry)->name_rest = RACE(race)->record_name_rest;
				backup_record = BOOKKEEPING_COURSE_RECORDS_ADDRESS + record_offset;
				AS32(BACKUP_COURSE_RECORD(backup_record)->model) = (COURSE_RECORD_ENTRY(record_entry)->model | ((uint32_t)COURSE_RECORD_ENTRY(record_entry)->name << 8u) | ((uint32_t)COURSE_RECORD_ENTRY(record_entry)->name_rest << 16u));
				BACKUP_COURSE_RECORD(backup_record)->time = COURSE_RECORD_ENTRY(record_entry)->time;
				bookkeeping_update_checksum((uint16_t)ignored);
				backup_record = SOUND_RAM + 0x1FAEu + record_offset;
				AS32(BACKUP_COURSE_RECORD(backup_record)->model) = (COURSE_RECORD_ENTRY(record_entry)->model | ((uint32_t)COURSE_RECORD_ENTRY(record_entry)->name << 8u) | ((uint32_t)COURSE_RECORD_ENTRY(record_entry)->name_rest << 16u));
				BACKUP_COURSE_RECORD(backup_record)->time = COURSE_RECORD_ENTRY(record_entry)->time;
			}
		}
		players_left--;
	} while (players_left != 0u);
	return record_entry;
}

/* Takes the race records of all the players into 20FC00 (80 bytes each, by player), each frame: alone
   (link_off, link_failed) this machine's own two (20FB00, 20FB80); in ISDN mode the eight from 21E000;
   on the link the eight from the link board (link_copy_records_in from 800020, or 800720 when its byte
   80000E is set), or link_lost when its error byte 80000C is set. */
void link_receive_records(void)
{
	uint32_t destination = 0, pairs_left = 0, record_count = 0, source = 0;
	uint8_t board_error = 0, second_buffer = 0;
	uint32_t block_F0 = 0, bytes_F4 = 0, block_F8 = 0, bytes_FC = 0;

	if (game->isdn_mode == 0u)
	{
		if (game->link_failed == 0u && game->link_off == 0u)
		{
			board_error = io_read8(LINK_BOARD_ERROR);
			if (board_error != 0u)
			{
				link_lost();
				return;
			}
			source = 0x800020u;
			destination = RACE_COPY_1;
			record_count = 8u;
			second_buffer = io_read8(LINK_SECOND_BUFFER);
			if (second_buffer == 0u)
			{
				link_copy_records_in(record_count, source, destination);
				return;
			}
			source += 0x700u;
			link_copy_records_in(record_count, source, destination);
			return;
		}
		source = RACE_1;
		destination = RACE_COPY_1;
		pairs_left = 1u;
	}
	else
	{
		source = 0x21E000u;
		destination = RACE_COPY_1;
		pairs_left = 4u;
	}
	do
	{
		for (uint32_t repeat = 0; repeat < 31u; repeat++)
		{
			block_F0 = destination;
			destination += 8u;
			bytes_F4 = RACE_RECORD_WORDS(source)[1];
			RACE_RECORD_WORDS(block_F0)[0] = RACE_RECORD_WORDS(source)[0];
			RACE_RECORD_WORDS(block_F0)[1] = bytes_F4;
			source += 8u;
		}
		block_F8 = destination;
		destination += 8u;
		bytes_FC = RACE_RECORD_WORDS(source)[1];
		RACE_RECORD_WORDS(block_F8)[0] = RACE_RECORD_WORDS(source)[0];
		RACE_RECORD_WORDS(block_F8)[1] = bytes_FC;
		pairs_left--;
		source += 8u;
	} while (pairs_left != 0u);
}

/* ISDN mode: copies the eight race records from the link board (800600) to 21E000, where
   link_receive_records takes them; the link interrupt (int_vector_44) does it on the board's command 3. */
void link_isdn_receive_records(void)
{
	uint32_t board_records = 0, race_records = 0, record_count = 0;

	board_records = 0x800600u;
	race_records = 0x21E000u;
	record_count = 8u;
	link_copy_records_in(record_count, board_records, race_records);
}

/* Copies R1 race records from the link board's memory at R10 (a byte in each halfword) to R11: their
   first 70 bytes, 80 apart. */
void link_copy_records_in(uint32_t record_count, uint32_t board_records, uint32_t race_records)
{
	do
	{
		for (uint32_t repeat = 0; repeat < 111u; repeat++)
		{
			RACE_RECORD_BYTES(race_records)[0] = (uint8_t)io_read16(board_records);
			race_records++;
			board_records += 2u;
		}
		RACE_RECORD_BYTES(race_records)[0] = (uint8_t)io_read16(board_records);
		race_records += 0x11u;
		record_count--;
		board_records += 2u;
	} while (record_count != 0u);
}

/* Sends this machine's two race records to the link board, each frame (not in ISDN mode, nor alone):
   their first 70 bytes to 800E20 and 800F00 (a byte in each halfword), then FF to its byte 800006;
   link_lost when its error byte 80000C is set. link_wait_frames from link_wait_frames_start (which
   nothing sets). */
void link_send_records(void)
{
	uint32_t board_records = 0, race = 0, record_count = 0;
	uint8_t board_error = 0;

	if (game->isdn_mode == 0u)
	{
		game->link_wait_frames = game->link_wait_frames_start;
		if (game->link_failed != 0u || game->link_off != 0u)
			return;
		board_error = io_read8(LINK_BOARD_ERROR);
		if (board_error != 0u)
		{
			link_lost();
			return;
		}
		race = RACE_COPY_1;
		board_records = 0x800FE0u;
		record_count = 2u;
		do
		{
			race = race - 0x10u - 1u;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x6Fu)->race_time >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x6Eu)->race_time >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x6Du)->race_time >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x6Cu)->race_time);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x6Bu)->time_left >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x6Au)->time_left);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x69u)->pass_time >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x68u)->pass_time);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x67u)->pass_branch);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x66u)->pass_order);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x65u)->pass_checkpoint);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x64u)->pass_player);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x63u)->pass_count);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x62u)->stage);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x61u)->pass_seen);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x60u)->stage_count);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Fu)->published);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Eu)->finished);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Du)->started);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Cu)->ready);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Bu)->link_pad_5B);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5Au)->link_pad_5A);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x59u)->car_flags >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x58u)->car_flags);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x57u)->bounce_height >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x56u)->bounce_height);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x55u)->segment_pitch >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x54u)->segment_pitch);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x53u)->hard_acceleration);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x52u)->wheel_effect_2);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x51u)->wheel_effect_1);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x50u)->wheel_effect_sides);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x4Fu)->sideways_speed >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x4Eu)->sideways_speed);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x4Du)->speed >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x4Cu)->speed);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x4Bu)->view_slot);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x4Au)->frame);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x49u)->model);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x48u)->car_choice);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x47u)->window_segment >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x46u)->window_segment);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x45u)->road_x >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x44u)->road_x);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x43u)->segment >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x42u)->segment);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x41u)->segment_fraction >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x40u)->segment_fraction);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x3Fu)->palette >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x3Eu)->palette);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x3Du)->animation >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x3Cu)->animation);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x3Bu)->world_z >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x3Au)->world_z >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x39u)->world_z >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x38u)->world_z);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x37u)->world_y >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x36u)->world_y >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x35u)->world_y >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x34u)->world_y);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x33u)->world_x >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x32u)->world_x >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x31u)->world_x >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x30u)->world_x);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Fu)->link_brake);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Eu)->link_gas);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Du)->link_wheel);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Cu)->link_buttons);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Bu)->link_status);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2Au)->players_other_side);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x29u)->players_this_side);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x28u)->game_over);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x27u)->race_order_4_7 >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x26u)->race_order_4_7 >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x25u)->race_order_4_7 >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x24u)->race_order_4_7);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x23u)->race_order_3);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x22u)->race_order_2);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x21u)->race_order_1);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x20u)->race_order);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x1Fu)->racer_count);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x1Eu)->entrant);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x1Du)->entry_serial);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x1Cu)->entry_serial_seen);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x1Bu)->entry_frames >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x1Au)->entry_frames);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x19u)->entry_request);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x18u)->record_course);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x17u)->finish_time >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x16u)->finish_time);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x15u)->record_name_rest >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0x14u)->record_name_rest);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x13u)->record_name);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x12u)->record_model);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x11u)->record_serial);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0x10u)->master_time_difficulty_2_5 >> 24u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0xFu)->master_time_difficulty_2_5 >> 16u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)(RACE(race - 0xEu)->master_time_difficulty_2_5 >> 8u));
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0xDu)->master_time_difficulty_2_5);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0xCu)->master_time_difficulty_1);
			race--;
			board_records -= 2u;
			io_write16(board_records, (uint8_t)RACE(race - 0xBu)->master_time_difficulty_0);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0xAu)->master_game_difficulty);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x9u)->master_free_play);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x8u)->master_coin_setting);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x7u)->master_bonus_adder);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x6u)->master_first_chute_multiplier);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x5u)->master_coins_per_credit);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x4u)->master_steering_wheel_reaction);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x3u)->master_advertise_sound);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x2u)->car_number);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race - 0x1u)->player);
			race--;
			board_records -= 2u;
			io_write16(board_records, RACE(race)->machine);
			record_count--;
		} while (record_count != 0u);
		io_write8(LINK_RECORDS_SENT, 0xFFu);
	}
}
