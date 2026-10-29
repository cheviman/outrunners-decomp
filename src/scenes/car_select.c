/**
 *    Name:        scenes/car_select.c
 *    Description: the car select: the attendant and the turntable, the cars, the transmission, the flags of the other cabinets' choices, the countdown and the entry traffic
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the car select with 630 frames (21 seconds) to choose, then car_select_run. */
void car_select_start(void)
{
	OBJECT(current_object)->car_select.time_left = 0x276u;
	OBJECT(current_object)->handler = car_select_run_ADDRESS;
	car_select_run();
}

/* The car select, by state: 0 sets the screen up (unless screen entry_countdown_shown says the entry
   countdown did: clears the LCD, the title Mega Driver, sound F, DJ message 3B), the tile layer's tables
   from 006920, the WELCOME! letters; 1 when the screen's data is unpacked (unpack_state) starts the
   eight cars on the turntable and the rest, the default car (car_select_default_cars by the player's
   place in the race), the time display (time_digits_start); 2 30 frames; 3 the screen's
   turntable_depth_offset up to 0, then the instructions (select_car_heading_show) and the car's caption
   (car_caption_show); 4 the wheel turns the turntable (turn, turn_delay, angle; car model is the car in
   front) until the gas or START confirms or time_left runs out: race model = the car, car_choice = the
   car + 1, the car's sound (car_select_car_sounds), at least 180 frames left, the transmission choice
   (transmission_choice_run); 5 the wheel picks the transmission (car transmission =
   car_select_wheel_transmissions by the wheel: 0 automatic, 1 manual) until confirmed or out of time;
   6-8 when the entry time is over (entry_over) marks the player ready (set_player_ready) and waits for
   the race's other players (players_not_ready), DJ message 3C; 9 60 frames, then fade_out_music and the
   game's next step (screen game_mode byte 2); 10 stays. Every frame from 4 on: car_select_turn_to_choice
   and car_select_update_rowscroll; time_left down by one and shown in seconds. */
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
			song_title = mega_driver_title_ADDRESS;
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
		video_registers[2] |= CAR_SELECT_TILEMAP_SETUP(setup_bits - 0x1Cu)->video_register_bits;
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.second_scroll_y_address)[0] = 0xE0u;
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
			LINE_SELECTS(first_rowselect)[0] = row_number;
			LINE_SELECTS(second_rowselect)[0] = row_number;
			LINE_SELECTS(r12)[0] = row_number;
			r12 += 2u;
			LINE_SELECTS(fourth_rowselect)[0] = row_number;
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
		move_halfwords(screen + 0x3800u, zero_block_ADDRESS, 0xE0u);
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
		default_car = car_select_default_cars[SCREEN(screen)->places[SCREEN(screen)->player]];
		CAR(player_car)->model = (uint8_t)default_car;
		CAR(player_car)->transmission = 0u;
		OBJECT(current_object)->car_select.car_chosen = 0u;
		OBJECT(current_object)->car_select.transmission_chosen = 0u;
		default_car = (uint16_t)((uint32_t)default_car << 7);
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - default_car;
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_y_address)[0] = 0u;
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
				VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - (new_angle >> 2);
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
				VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - (turntable_angle >> 2);
				car_select_skew_floor_previous();
				return;
			}
			turntable_angle += 0x20u;
			OBJECT(current_object)->car_select.angle = turntable_angle;
			VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - (turntable_angle >> 2);
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
				VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - (turntable_angle >> 2);
				car_select_skew_floor_previous();
				return;
			}
			turntable_angle += 0x20u;
			OBJECT(current_object)->car_select.angle = turntable_angle;
			VIDEO_REGISTER_WORDS(OBJECT(current_object)->car_select.scroll_x_address)[0] = 0u - (turntable_angle >> 2);
			car_select_skew_floor_next();
			return;
		}
		OBJECT(current_object)->car_select.choice_turn++;
		return;
	case 2:
		return;
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
		LINE_SCROLL_VALUES(near_scroll_place)[0] = line_scroll;
		LINE_SCROLL_VALUES(r12)[0] = line_scroll;
		r12 += 2u;
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
			line_scroll = shift_by(FLOOR_LINE_OFFSETS(line_offsets)[0], (uint8_t)fraction_shift, 16);
			LINE_SCROLL_VALUES(near_scroll_place)[0] = line_scroll;
			near_scroll_place += 2u;
			LINE_SCROLL_VALUES(r12)[0] = line_scroll;
			r12 += 2u;
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
		near_offset = (uint16_t)(FLOOR_LINE_OFFSETS(line_offsets)[0] + skew);
		far_offset = (uint16_t)(near_offset + skew);
		FLOOR_LINE_OFFSETS(line_offsets)[0] = far_offset;
		LINE_SCROLL_VALUES(near_scroll_place)[0] = shift_by(near_offset, (uint8_t)fraction_shift, 16);
		near_scroll_place += 2u;
		LINE_SCROLL_VALUES(r12)[0] = shift_by(far_offset, (uint8_t)fraction_shift, 16);
		r12 += 2u;
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
	uint32_t across = 0, animation = 0, attendant_angle = 0, down = 0, point_depth = 0, radius = 0;
	uint16_t angle_to_front = 0;

	for (;;)
	{
		switch (OBJECT(current_object)->state)
		{
		case 0:
			OBJECT(current_object)->draw_mode = 0xAu;
			animation = STANDING;
			OBJECT(current_object)->palette = 0x7Cu;
			animation_start(animation);
			OBJECT(current_object)->car_attendant.angle = OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle + 0x100u;
			OBJECT(current_object)->car_attendant.radius = 0x160u;
			OBJECT(current_object)->car_attendant.size = 0xC0u;
			OBJECT(current_object)->state++;
		case 1:
			if (((((uint32_t)CAR(player_car)->model << 9) - OBJECT(current_object)->car_attendant.angle) & 0xFFFu) == 0u)
			{
				if (OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.car_chosen == 0u)
				{
					attendant_angle = OBJECT(current_object)->car_attendant.angle;
					break;
				}
				OBJECT(current_object)->car_attendant.side_offset = 0;
				OBJECT(current_object)->state = 3u;
				continue;
			}
			animation = ATTENDANT_WALKING;
			animation_start(animation);
			OBJECT(current_object)->state++;
		case 2:
			attendant_angle = OBJECT(current_object)->car_attendant.angle;
			angle_to_front = (attendant_angle - OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.angle) << 4;
			negative = angle_to_front >> 15;
			if (angle_to_front == 0u)
			{
				animation = STANDING;
				animation_start(animation);
				OBJECT(current_object)->flags &= 0xBFu;
				OBJECT(current_object)->state--;
				break;
			}
			if (negative)
			{
				OBJECT(current_object)->flags |= 0x40u;
				attendant_angle = (uint16_t)(attendant_angle + 0x20u);
				OBJECT(current_object)->car_attendant.angle = attendant_angle;
				break;
			}
			OBJECT(current_object)->flags &= 0xBFu;
			attendant_angle = (uint16_t)(attendant_angle - 0x20u);
			OBJECT(current_object)->car_attendant.angle = attendant_angle;
			break;
		case 3:
			attendant_angle = (uint16_t)OBJECT(current_object)->car_attendant.side_offset;
			target_side = (uint16_t)((uint32_t)CAR(player_car)->transmission << 7);
			if (attendant_angle == target_side)
			{
				if (OBJECT(OBJECT(current_object)->car_attendant.car_select)->car_select.transmission_chosen == 0u)
				{
					animation = STANDING;
					if (OBJECT(current_object)->animation != STANDING)
						animation_start(animation);
					attendant_angle = (uint16_t)(attendant_angle + OBJECT(current_object)->car_attendant.angle);
					break;
				}
				OBJECT(current_object)->car_attendant.angle += OBJECT(current_object)->car_attendant.side_offset;
				animation = AUTOMATIC_POSE;
				if (CAR(player_car)->transmission != 0u)
					animation = MANUAL_POSE;
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
				animation_advance(0u);
				return;
			}
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
			animation = ATTENDANT_WALKING;
			if (OBJECT(current_object)->animation == ATTENDANT_WALKING)
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
				animation_advance(0u);
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
			animation_advance(0u);
			return;
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
			animation_advance(0u);
			return;
		}
		break;
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
	animation_advance(0u);
}

/* One of the eight cars on the car select's turntable (car: its model and its place, car << 9), by
   state: 0 its sprite set and palette (turntable_car_looks by car); 1 placed by its angle less the
   turntable's (car_select angle), with the frame that shows it from there (turntable_car_frames), round
   the centre (radius screen turntable_radius_offset + 140) with turntable_draw; once the car is chosen
   (car_select car_chosen) the car in front takes the palette 4 on if it is the chosen one (car model)
   and goes to 2, where draw_object draws it where it was. */
void turntable_car_show(void)
{
	uint32_t across = 0, animation = 0, car_angle = 0, down = 0, look = 0, point_depth = 0, ring_radius = 0, seen_angle = 0;
	uint16_t angle_from_front = 0;

	switch (OBJECT(current_object)->state)
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
		draw_object(0u);
		return;
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
	turntable_draw(across, down, point_depth);
}

/* A ring of scenery round the car select's turntable (one of twelve groups, from 006A18: the crowd, the
   palm trees), by state: 0 its animation, palette, size_step, size and list of items (item_list: angle,
   radius and height of each, closed by a negative angle); 1 each frame takes the turntable's angle
   (car_select angle) and draws the items on the far side (their angle less it 480..B7F) with
   turntable_draw, keeping for each its previous_place and flags bit 5 in item_places for draw mode 0A's
   halfway copy; then animation_advance. */
void turntable_scenery_show(void)
{
	uint16_t leftover = 0, size = 0;
	uint32_t across = 0, animation = 0, down = 0, group = 0, item_angle = 0, item_place = 0, point_depth = 0, ring_radius = 0, saved_place = 0;
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
	}
	item_place = OBJECT(current_object)->turntable_scenery.item_list;
	saved_place = current_object + 0x80u;
	OBJECT(current_object)->turntable_scenery.angle = OBJECT(OBJECT(current_object)->turntable_scenery.car_select)->car_select.angle;
	for (;;)
	{
		item_angle = TURNTABLE_SCENERY_ITEM(item_place)->angle;
		item_place += 2u;
		if ((int16_t)item_angle < 0)
			break;
		item_angle = (uint16_t)(item_angle - OBJECT(current_object)->turntable_scenery.angle);
		item_angle = item_angle & 0xFFFu;
		ring_radius = TURNTABLE_SCENERY_ITEM(item_place - 0x2u)->radius;
		item_place += 2u;
		if (item_angle < 0x480u || item_angle > 0xB7Fu)
		{
			item_place += 2u;
			saved_place += 2u;
			ITEM_PLACE_WORDS(saved_place)[0] = 0u;
			saved_place += 2u;
			continue;
		}
		ring_radius += SCREEN(screen)->turntable_radius_offset;
		across = (int32_t)(sine_table[item_angle] * ring_radius) >> 14;
		point_depth = (int32_t)((0u - cosine_table[item_angle]) * ring_radius) >> 14;
		down = TURNTABLE_SCENERY_ITEM(item_place - 0x4u)->height;
		point_depth += 0x140u;
		kept_saved_place = saved_place;
		OBJECT(current_object)->previous_place = LINKED(screen, ITEM_PLACE_WORDS(saved_place)[0]);
		OBJECT(current_object)->flags = (OBJECT(current_object)->flags & 0xDFu) | ITEM_PLACE_WORDS(saved_place)[1];
		kept_item_place = item_place + 2u;
		leftover = turntable_draw(across, down, point_depth);
		item_place = kept_item_place;
		saved_place = kept_saved_place;
		ITEM_PLACE_WORDS(saved_place)[0] = (uint16_t)OBJECT(current_object)->turntable_scenery.previous_place;
		leftover = (uint16_t)((leftover & ~0xFFu) | OBJECT(current_object)->flags);
		saved_place += 2u;
		ITEM_PLACE_WORDS(saved_place)[0] = leftover & ~0xDFu;
		saved_place += 2u;
	}
	animation_advance(0u);
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

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = CAR_NAME_FONT;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x18u;
		letter_index = OBJECT(current_object)->welcome_letter.index;
		place = welcome_letter_places_ADDRESS + letter_index * 4u;
		OBJECT(current_object)->x = WELCOME_LETTER_PLACE(place)->x;
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->frame = WELCOME_LETTER_PLACE(place)->letter;
		OBJECT(current_object)->welcome_letter.delay = letter_index * 2u;
		OBJECT(current_object)->welcome_letter.frames_left = 0x2Du;
		OBJECT(current_object)->welcome_letter.palette_list = welcome_letter_palette_steps_ADDRESS;
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
	uint32_t animation = 0, object = 0, piece = 0;
	uint16_t timer = 0, timer_2 = 0;

	switch (OBJECT(current_object)->state)
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
		animation = SELECT_CAR_HEADING;
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
		draw_object(0u);
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
		draw_object(0u);
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
	}
}

/* A piece of the car select's instructions ("Turn your", the steering wheel, "Then foot the", the
   accelerator; piece_animation, palette and place from select_car_heading), by state: 0 starts its
   animation; 1 drawn and animated until the car is chosen (car_select car_chosen); 2 rises 2 a frame for
   16 frames (timer), over the transmission choice; 3 drawn and animated until the transmission is chosen
   (transmission_chosen), when it ends. */
void select_instruction_show(void)
{
	uint32_t piece_animation = 0;
	uint16_t timer = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		piece_animation = OBJECT(current_object)->select_instruction.piece_animation;
		animation_start(piece_animation);
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.car_chosen == 0u)
		{
			draw_object(0u);
			animation_advance(0u);
			return;
		}
		OBJECT(current_object)->select_instruction.timer = 0x10u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.car_chosen == 0u)
		{
			draw_object(0u);
			animation_advance(0u);
			return;
		}
		OBJECT(current_object)->select_instruction.timer = 0x10u;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y -= 2u;
		timer = OBJECT(current_object)->select_instruction.timer;
		OBJECT(current_object)->select_instruction.timer = timer - 1u;
		if (timer != 1u)
		{
			draw_object(0u);
			animation_advance(0u);
			return;
		}
		OBJECT(current_object)->state++;
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(0u);
		animation_advance(0u);
		return;
	case 2:
		break;
	case 3:
		if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(0u);
		animation_advance(0u);
		return;
	}
	OBJECT(current_object)->y -= 2u;
	timer = OBJECT(current_object)->select_instruction.timer;
	OBJECT(current_object)->select_instruction.timer = timer - 1u;
	if (timer != 1u)
	{
		draw_object(0u);
		animation_advance(0u);
		return;
	}
	OBJECT(current_object)->state++;
	if (OBJECT(OBJECT(current_object)->select_instruction.car_select)->car_select.transmission_chosen != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(0u);
	animation_advance(0u);
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
		panels_y = VIDEO_REGISTER_WORDS(second_scroll_y_address)[0];
		VIDEO_REGISTER_WORDS(second_scroll_y_address)[0] = panels_y - 0x10u;
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
			VIDEO_REGISTER_WORDS(second_scroll_x_address)[0] += slide_step;
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
		VIDEO_REGISTER_WORDS(second_scroll_x_address)[0] += slide_step;
		slide_frames = OBJECT(current_object)->transmission_choice.slide_frames;
		OBJECT(current_object)->transmission_choice.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			OBJECT(current_object)->state++;
		break;
	case 4:
		break;
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
	uint32_t animation = 0, object = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = transmission_label_start_automatic_ADDRESS;
		OBJECT(object)->transmission_label.panel = current_object;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		animation = AUTOMATIC_PANEL;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->x_offset = 0x54;
		OBJECT(current_object)->y_offset = -0x6C;
		OBJECT(current_object)->automatic_panel.palette_list = transmission_panel_palette_steps_ADDRESS;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
		draw_object(0u);
		return;
	}
	if (CAR(player_car)->transmission != 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->automatic_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
		draw_object(0u);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->automatic_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
	OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
	draw_object(0u);
}

/* The MANUAL TRANSMISSION title of the right panel of the transmission choice, by state
   (manual_panel_state_jumps): 0 starts its label (transmission_label_start_manual) and sets it up; 1
   cycles its colours (step_palette from palette_list) while the wheel picks manual (car transmission 1),
   else palette 2AE; drawn at x_offset, y_offset less the choice's x, y (transmission_choice), which
   slide it. Nothing sets state 2 (drawing only). */
void manual_panel_show(void)
{
	uint32_t animation = 0, object = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		object = start_object();
		OBJECT(object)->handler = transmission_label_start_manual_ADDRESS;
		OBJECT(object)->transmission_label.panel = current_object;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0xF0u;
		animation = MANUAL_PANEL;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2A8u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->x_offset = 0xEC;
		OBJECT(current_object)->y_offset = -0x6C;
		OBJECT(current_object)->manual_panel.palette_list = transmission_panel_palette_steps_ADDRESS;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
		draw_object(0u);
		return;
	}
	if (CAR(player_car)->transmission == 0u)
	{
		OBJECT(current_object)->palette = 0x2AEu;
		object = OBJECT(current_object)->manual_panel.choice;
		OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
		OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
		draw_object(0u);
		return;
	}
	step_palette();
	object = OBJECT(current_object)->manual_panel.choice;
	OBJECT(current_object)->x = 0u - OBJECT(object)->x + OBJECT(current_object)->x_offset;
	OBJECT(current_object)->y = 0u - OBJECT(object)->y + OBJECT(current_object)->y_offset;
	draw_object(0u);
}

/* Sets up the label of the automatic transmission's panel (sprite set 2C1, 24 below the panel's place;
   the frames show NOVICE there), then transmission_label_follow. */
void transmission_label_start_automatic(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	animation = NOVICE_LABEL;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x2A8u;
	OBJECT(current_object)->x_offset = 0;
	OBJECT(current_object)->y_offset = 0x24;
	OBJECT(current_object)->handler = transmission_label_follow_ADDRESS;
	transmission_label_follow();
}

/* Sets up the label of the manual transmission's panel (sprite set 2C9, its frame from
   car_gears_caption_frames by the car's model; the frames show 5 SPEED there for the car coin_start
   takes), then transmission_label_follow. */
void transmission_label_start_manual(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xF0u;
	animation = GEARS_LABEL;
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
	uint32_t object = 0;

	object = OBJECT(current_object)->transmission_label.panel;
	if ((OBJECT(object)->flags & 1u) == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->x = OBJECT(object)->x + OBJECT(current_object)->x_offset;
	OBJECT(current_object)->y = OBJECT(object)->y + OBJECT(current_object)->y_offset;
	OBJECT(current_object)->palette = OBJECT(object)->palette;
	draw_object(0u);
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
		animation = CAR_CAPTION_LETTERS;
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
		draw_object(car_model);
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
		draw_object(car_model);
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
		animation = CAR_GEARS_CAPTION;
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
		draw_object(car_model);
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
		draw_object(car_model);
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
	}
}

/* The car select's message once the choices are made, by state: 0 sets up WAITING FOR CHALLENGERS! JUST
   A MOMENT PLEASE (animation 28B); 1 drawn 20 frames (timer); 2 hidden 10; once the entry time is over
   (car_select entry_over) JUST A MOMENT PLEASE (animation 28D) instead: 3 drawn 20 frames, 4 hidden 10. */
void waiting_banner_show(void)
{
	uint32_t animation = 0;
	uint8_t timer = 0, timer_2 = 0;

	for (;;)
	{
		switch (OBJECT(current_object)->state)
		{
		case 0:
			OBJECT(current_object)->draw_mode = 0u;
			OBJECT(current_object)->depth_layer = 0x19Eu;
			animation = WAITING_BANNER;
			animation_start(animation);
			OBJECT(current_object)->palette = 0x287u;
			OBJECT(current_object)->x = 0xA0u;
			OBJECT(current_object)->y = 0x88u;
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state++;
			if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over == 0u)
			{
				timer = OBJECT(current_object)->waiting_banner.timer;
				OBJECT(current_object)->waiting_banner.timer = timer - 1u;
				if (timer == 1u)
				{
					OBJECT(current_object)->waiting_banner.timer = 0xAu;
					OBJECT(current_object)->state++;
				}
				draw_object(0u);
				return;
			}
			animation = WAIT_A_MINUTE_BANNER;
			animation_start(animation);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
			continue;
		case 1:
			if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over == 0u)
			{
				timer = OBJECT(current_object)->waiting_banner.timer;
				OBJECT(current_object)->waiting_banner.timer = timer - 1u;
				if (timer == 1u)
				{
					OBJECT(current_object)->waiting_banner.timer = 0xAu;
					OBJECT(current_object)->state++;
				}
				draw_object(0u);
				return;
			}
			animation = WAIT_A_MINUTE_BANNER;
			animation_start(animation);
			OBJECT(current_object)->waiting_banner.timer = 0x14u;
			OBJECT(current_object)->state = 3u;
			continue;
		case 3:
			timer = OBJECT(current_object)->waiting_banner.timer;
			OBJECT(current_object)->waiting_banner.timer = timer - 1u;
			if (timer == 1u)
			{
				OBJECT(current_object)->waiting_banner.timer = 0xAu;
				OBJECT(current_object)->state++;
			}
			draw_object(0u);
			return;
		case 2:
			if (OBJECT(OBJECT(current_object)->waiting_banner.car_select)->car_select.entry_over != 0u)
			{
				animation = WAIT_A_MINUTE_BANNER;
				animation_start(animation);
				OBJECT(current_object)->waiting_banner.timer = 0x14u;
				OBJECT(current_object)->state = 3u;
				continue;
			}
			break;
		case 4:
			break;
		}
		break;
	}
	timer_2 = OBJECT(current_object)->waiting_banner.timer;
	OBJECT(current_object)->waiting_banner.timer = timer_2 - 1u;
	if (timer_2 == 1u)
	{
		OBJECT(current_object)->waiting_banner.timer = 0x14u;
		OBJECT(current_object)->state--;
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
	uint32_t car_flag = 0, chosen_car = 0, flag_blink_frames = 0, ignored = 0, lcd_text_end = 0, object = 0, own_number = 0, players_left = 0, racer = 0, racer_car_number = 0, racer_place = 0, racers_left = 0;
	uint8_t blink_timer = 0;
	uint32_t car_choice = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->car_select_entries.race_order = SCREEN(screen)->host_record + 0x1Fu;
		OBJECT(current_object)->car_select_entries.shown_players &= 0xFFFFFF00u;
		OBJECT(current_object)->car_select_entries.frames = 0u;
		OBJECT(current_object)->state++;
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		move_string(entry_lcd_text_ADDRESS, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &lcd_text_end, &ignored);
		players_left = player_count;
		do
		{
			LCD_CHARACTERS(lcd_text_end)[mem_read8(0x20E02Fu + players_left)] = 0x5Fu;
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
		move_string(entry_lcd_text_ADDRESS, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &lcd_text_end, &ignored);
		players_left = player_count;
		do
		{
			LCD_CHARACTERS(lcd_text_end)[mem_read8(0x20E02Fu + players_left)] = 0x5Fu;
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
	}
	blink_timer = OBJECT(current_object)->car_select_entries.blink_timer;
	OBJECT(current_object)->car_select_entries.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		own_number = OBJECT(current_object)->car_select_entries.own_number;
		SCREEN(screen)->lcd_text[(int32_t)(own_number + 8u)] ^= OBJECT(current_object)->car_select_entries.digit_toggle;
		OBJECT(current_object)->car_select_entries.blink_timer = 8u;
	}
	racer_place = OBJECT(current_object)->car_select_entries.race_order;
	racers_left = RACER_NUMBERS(racer_place)[0];
	racer_place++;
	do
	{
		racer = RACER_NUMBERS(racer_place)[0];
		racer_place++;
		if (SCREEN(screen)->player != racer && (OBJECT(current_object)->car_select_entries.shown_players & shl32(1u, (int)(racer))) == 0u)
		{
			racer_car_number = car_numbers[racer];
			SCREEN(screen)->lcd_text[(int32_t)(racer_car_number + 8u)] = racer_car_number + 0x31u;
			car_choice = RACE(RACE_COPY_1)[racer].car_choice - 1u;
			chosen_car = car_choice;
			if ((int32_t)car_choice >= 0)
			{
				car_flag = OBJECT(current_object)->car_select_entries.car_flags[(int32_t)chosen_car];
				object = start_object();
				if (car_flag == 0u)
				{
					OBJECT(object)->handler = choice_flag_start_ADDRESS;
					OBJECT(object)->choice_flag.top_flag = object;
					OBJECT(object)->choice_flag.entries = current_object;
					OBJECT(current_object)->car_select_entries.car_flags[(int32_t)chosen_car] = object;
				}
				else
				{
					OBJECT(object)->handler = stacked_choice_flag_start_ADDRESS;
					OBJECT(object)->stacked_choice_flag.below = OBJECT(car_flag)->choice_flag.top_flag;
					OBJECT(car_flag)->choice_flag.top_flag = object;
				}
				OBJECT(object)->number = racer;
				OBJECT(object)->car = chosen_car;
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

/* Sets up a flag over the car another player has chosen (animation 1B, a palette by the car,
   choice_flag_palettes; angle: the car's place on the turntable), starts its number (choice_flag_number)
   and goes on to choice_flag_follow. */
void choice_flag_start(void)
{
	uint32_t animation = 0, car = 0, object = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	animation = CHOICE_FLAG;
	animation_start(animation);
	car = OBJECT(current_object)->car;
	OBJECT(current_object)->palette = choice_flag_palettes[car];
	OBJECT(current_object)->choice_flag.angle = car << 9;
	object = start_object();
	OBJECT(object)->handler = choice_flag_number_start_ADDRESS;
	OBJECT(object)->choice_flag_number.flag = current_object;
	OBJECT(current_object)->handler = choice_flag_follow_ADDRESS;
	choice_flag_follow();
}

/* Keeps the flag over its car as the turntable turns (its angle less car_select angle, projected as
   turntable_draw does, above the cars) and draws it while car_select_entries' flags_visible says so
   (visible); not drawn behind the view. */
void choice_flag_follow(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint32_t flag_depth = 0, flag_x = 0, flag_y = 0, object = 0, perspective = 0, ring_radius = 0, seen_angle = 0;
	uint8_t flags_shown = 0;
	uint32_t turntable_depth_offset = 0, view_distance = 0;

	object = OBJECT(current_object)->choice_flag.entries;
	flags_shown = ~~OBJECT(object)->car_select_entries.flags_visible;
	OBJECT(current_object)->choice_flag.visible = flags_shown;
	if (flags_shown == 0u)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		return;
	}
	seen_angle = OBJECT(current_object)->choice_flag.angle;
	seen_angle = (uint16_t)(seen_angle - OBJECT(OBJECT(object)->car_select_entries.car_select)->car_select.angle);
	seen_angle = seen_angle & 0xFFFu;
	ring_radius = SCREEN(screen)->turntable_radius_offset + 0x140u;
	flag_x = (int32_t)(sine_table[seen_angle] * ring_radius) >> 14;
	flag_depth = (int32_t)((0u - cosine_table[seen_angle]) * ring_radius) >> 14;
	flag_y = 0x44u;
	flag_depth += 0x140u;
	perspective = 0x100000u;
	turntable_depth_offset = SCREEN(screen)->turntable_depth_offset;
	flag_depth += 0x100u;
	view_distance = flag_depth + turntable_depth_offset;
	overflow = v60_overflow_add32(flag_depth, turntable_depth_offset, 0u);
	zero = view_distance == 0u;
	negative = view_distance >> 31;
	flag_depth = view_distance;
	if ((negative ^ overflow) | zero)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		return;
	}
	perspective = divide_s32(perspective, flag_depth);
	flag_x = flag_x * perspective;
	carry = (flag_x >> 11) & 1u;
	flag_x = (uint16_t)(((int32_t)flag_x >> 12) + 0xA0u + carry);
	flag_y = flag_y * perspective;
	carry = (flag_y >> 11) & 1u;
	OBJECT(current_object)->x = flag_x;
	OBJECT(current_object)->y = ((int32_t)flag_y >> 12) + 0x60u + carry;
	draw_object(perspective);
}

/* Sets up the flag of one more player on a car that has one (on the last flag there, below), starts its
   number (choice_flag_number) and goes on to stacked_choice_flag_follow. */
void stacked_choice_flag_start(void)
{
	uint32_t animation = 0, object = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	animation = CHOICE_FLAG;
	animation_start(animation);
	OBJECT(current_object)->palette = choice_flag_palettes[OBJECT(current_object)->car];
	object = start_object();
	OBJECT(object)->handler = choice_flag_number_start_ADDRESS;
	OBJECT(object)->choice_flag_number.flag = current_object;
	OBJECT(current_object)->handler = stacked_choice_flag_follow_ADDRESS;
	stacked_choice_flag_follow();
}

/* Draws the flag 14 higher (and 3 right) than the one below it while that one is drawn (its flags bit 5
   and visible); else it is not drawn. */
void stacked_choice_flag_follow(void)
{
	uint32_t object = 0;
	uint8_t below_shown = 0;

	object = OBJECT(current_object)->stacked_choice_flag.below;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		below_shown = ~~OBJECT(object)->choice_flag.visible;
		OBJECT(current_object)->stacked_choice_flag.visible = below_shown;
		if (below_shown != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 3u;
			OBJECT(current_object)->y = OBJECT(object)->y - 0x14u;
			draw_object(0u);
			return;
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
}

/* Sets up the car number of a flag's player (choice_flag_number_animations by its car number,
   car_numbers), then choice_flag_number_follow. */
void choice_flag_number_start(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEFu;
	animation = choice_flag_number_animations[car_numbers[OBJECT(OBJECT(current_object)->choice_flag_number.flag)->choice_flag.player]];
	animation_start(animation);
	OBJECT(current_object)->palette = 0x27u;
	OBJECT(current_object)->handler = choice_flag_number_follow_ADDRESS;
	choice_flag_number_follow();
}

/* Draws the number on its flag (13 right, 10 up from its place) while the flag is drawn (its flags bit 5
   and visible); else it is not drawn. */
void choice_flag_number_follow(void)
{
	uint32_t object = 0;
	uint8_t flag_shown = 0;

	object = OBJECT(current_object)->choice_flag_number.flag;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		flag_shown = ~~OBJECT(object)->choice_flag.visible;
		OBJECT(current_object)->choice_flag_number.visible = flag_shown;
		if (flag_shown != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 0x13u;
			OBJECT(current_object)->y = OBJECT(object)->y - 0x10u;
			draw_object(0u);
			return;
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
}

/* Places the object at a point of the car select's turntable scene and draws it: R1 across, R2 down and
   R3 in depth (the turntable's centre at 140), seen from a view that screen turntable_depth_offset takes
   back (car_select starts it at -300 and raises it to 0, so the scene shrinks into its place): with d =
   R3 + 100 + turntable_depth_offset, x = R1 * (100000 / d) >> 12 + A0, y = R2 * (100000 / d) >> 12 + 60,
   the depth layer (3C) 100 - d >> 4, the width and height scale (100000 / d) * the object's size (1C) >>
   12. Not drawn (flags bit 5 cleared) with d under 10 or x outside -28..168. Used by car_attendant,
   turntable_car and turntable_scenery. */
uint16_t turntable_draw(uint32_t across, uint32_t down, uint32_t point_depth)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t fraction_shift = 0;
	uint16_t distance_layer = 0;
	uint32_t scale = 0, view_distance = 0;
	uint16_t distance_sixteenths = 0;
	uint32_t turntable_depth_offset = 0, distance_sum = 0;

	scale = 0x100000u;
	turntable_depth_offset = SCREEN(screen)->turntable_depth_offset;
	point_depth += 0x100u;
	distance_sum = point_depth + turntable_depth_offset;
	overflow = v60_overflow_add32(point_depth, turntable_depth_offset, 0u);
	zero = distance_sum == 0u;
	negative = distance_sum >> 31;
	view_distance = distance_sum;
	if (((negative ^ overflow) | zero) == 0u)
	{
		scale = divide_s32(scale, view_distance);
		distance_sixteenths = (uint16_t)view_distance >> 4;
		distance_layer = distance_sixteenths;
		if (distance_sixteenths != 0u)
		{
			OBJECT(current_object)->depth_layer = 0u - (distance_layer - 0x100u);
			fraction_shift = 0xF4u;
			across = across * scale;
			carry = (across >> 11) & 1u;
			across = ((int32_t)across >> 12) + 0xA0u + carry;
			if ((int32_t)across <= -0x28 || (int32_t)across >= 0x168)
			{
				OBJECT(current_object)->flags &= 0xDFu;
				return scale;
			}
			down = down * scale;
			carry = v60_sha_cy(down, fraction_shift, 32);
			OBJECT(current_object)->x = across;
			OBJECT(current_object)->y = shift_signed_by(down, fraction_shift, 32) + 0x60u + carry;
			scale = scale * OBJECT(current_object)->turntable.size;
			carry = v60_sha_cy(scale, fraction_shift, 32);
			scale = (uint16_t)(shift_signed_by(scale, fraction_shift, 32) + carry);
			OBJECT(current_object)->width_scale = scale;
			OBJECT(current_object)->height_scale = scale;
			return draw_object(scale);
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
	return scale;
}

/* Whether the player has not confirmed yet (zero set): confirming is pressing the gas pedal down (screen
   gas from C0, after letting it up under 40) or START, once confirm_delay has run out. */
uint32_t car_select_wait_confirm(void)
{
	uint32_t zero = 0;
	uint32_t confirm_delay = 0;

	confirm_delay = OBJECT(current_object)->car_select.confirm_delay;
	if (confirm_delay != 0u)
		OBJECT(current_object)->car_select.confirm_delay = confirm_delay - 1u;
	switch (OBJECT(current_object)->car_select.gas_down)
	{
	case 0:
		if (SCREEN(screen)->gas < 0xC0u)
		{
			if ((SCREEN(screen)->buttons_pressed & 0x40u) != 0u)
			{
				if (OBJECT(current_object)->car_select.confirm_delay == 0u)
				{
					zero = 1u == 0u;
					return zero;
				}
			}
			zero = 0u == 0u;
			return zero;
		}
		OBJECT(current_object)->car_select.gas_down++;
		if (OBJECT(current_object)->car_select.confirm_delay != 0u)
		{
			zero = 0u == 0u;
			return zero;
		}
		zero = 1u == 0u;
		return zero;
	case 1:
		if (SCREEN(screen)->gas >= 0x40u)
		{
			if ((SCREEN(screen)->buttons_pressed & 0x40u) != 0u)
			{
				if (OBJECT(current_object)->car_select.confirm_delay == 0u)
				{
					zero = 1u == 0u;
					return zero;
				}
			}
			zero = 0u == 0u;
			return zero;
		}
		OBJECT(current_object)->car_select.gas_down--;
		if ((SCREEN(screen)->buttons_pressed & 0x40u) != 0u)
		{
			if (OBJECT(current_object)->car_select.confirm_delay == 0u)
			{
				zero = 1u == 0u;
				return zero;
			}
		}
		zero = 0u == 0u;
		return zero;
	}
}

/* Sets up the invitation to join, on a seat with no player while the entry time of a game runs: the
   screen (set_up_background, start_palette_fades), clears the LCD (clear_lcd) with the DJ's face, the
   music (sound F, Mega Driver) and DJ message 3B, marks the screen (unk_E2) so the car select does not
   start them again, starts the WAITING YOUR ENTRY banner (entry_banner), the ring of cars
   (entry_car_ring, in car_ring) and the traffic behind it (entry_traffic_run), and goes on to
   entry_countdown_count. */
void entry_countdown_start(void)
{
	uint8_t dj_message = 0, sound_code = 0;
	uint32_t animation = 0, background_setup = 0, object = 0, palette_fades = 0;

	background_setup = 0x6988u + SCREEN(screen)->index * 5u * 4u;
	set_up_background(background_setup);
	palette_fades = entry_countdown_palette_fades_ADDRESS;
	start_palette_fades(palette_fades);
	clear_lcd();
	SCREEN(screen)->dj_face = 0xFFu;
	sound_code = 0xFu;
	request_sound(sound_code);
	dj_message = 0x3Bu;
	start_dj_message(dj_message);
	SCREEN(screen)->entry_countdown_shown = 0xFFu;
	object = start_object();
	OBJECT(object)->handler = entry_banner_start_ADDRESS;
	object = start_object();
	OBJECT(object)->handler = entry_car_ring_start_ADDRESS;
	OBJECT(current_object)->entry_countdown.car_ring = object;
	object = start_object();
	OBJECT(object)->handler = entry_traffic_run_ADDRESS;
	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Du;
	animation = ENTRY_COUNTDOWN_DIGITS;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x4Eu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x7Au;
	OBJECT(current_object)->handler = entry_countdown_count_ADDRESS;
	entry_countdown_count();
}

/* Shows the entry time left (RACE_COPY_1's entry_frames, frames): the seconds as the big number (its
   frame), the frames into the second to the ring of cars, and on the LCD a shrinking bar
   (entry_countdown_lcd_texts) with the seconds; when it is over clears the screen's unk_E2,
   fade_out_music, and goes on to entry_countdown_done. */
void entry_countdown_count(void)
{
	uint32_t zero = 0;
	uint32_t entry_frames = 0, remainder = 0, time_shown = 0;
	uint32_t frames_shown = 0;

	entry_frames = RACE(RACE_COPY_1)->entry_frames;
	if (entry_frames == 0u)
	{
		SCREEN(screen)->entry_countdown_shown = 0u;
		fade_out_music();
		OBJECT(current_object)->handler = entry_countdown_done_ADDRESS;
		entry_countdown_done();
		return;
	}
	frames_shown = entry_frames - 6u;
	time_shown = frames_shown;
	if ((int32_t)frames_shown < 0)
		time_shown = 0u;
	remainder = 0u;
	divux_pair(0x1Eu, &time_shown, &remainder);
	OBJECT(current_object)->frame = time_shown;
	OBJECT(OBJECT(current_object)->entry_countdown.car_ring)->entry_car_ring.copies = remainder;
	move_halfwords(screen + 0xF12u, entry_countdown_lcd_texts[(remainder + 2u) / 5u], 7u);
	remainder = 0u;
	divux_pair(0xAu, &time_shown, &remainder);
	zero = (uint32_t)(time_shown == 0u);
	if (zero == 0u)
		SCREEN(screen)->lcd_text[8] = time_shown + 0x30u;
	SCREEN(screen)->lcd_text[9] = remainder + 0x30u;
	draw_object(time_shown);
}

/* Does nothing (the number is no longer drawn): the entry time is over. */
void entry_countdown_done(void)
{
}

/* Sets up the WAITING YOUR ENTRY banner (sprite set 287) at the centre of the screen, then
   entry_banner_blink. */
void entry_banner_start(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	animation = 0x287u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x287u;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	OBJECT(current_object)->handler = entry_banner_blink_ADDRESS;
	entry_banner_blink();
}

/* Draws the banner 16 frames out of every 32. */
void entry_banner_blink(void)
{
	uint32_t blink_frames = 0;

	blink_frames = (uint8_t)(OBJECT(current_object)->entry_banner.blink + 1u);
	OBJECT(current_object)->entry_banner.blink = blink_frames;
	if ((blink_frames & 0x10u) != 0u)
		return;
	draw_object(blink_frames);
}

/* Sets up the ring of cars around the entry countdown with the first car's sprite (turntable_car_looks),
   then entry_car_ring_draw. */
void entry_car_ring_start(void)
{
	uint32_t animation = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Bu;
	animation = turntable_car_looks[0].animation;
	OBJECT(current_object)->palette = turntable_car_looks[0].palette + 4u;
	animation_start(animation);
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	OBJECT(current_object)->handler = entry_car_ring_draw_ADDRESS;
	entry_car_ring_draw();
}

/* Draws a ring of as many copies of a car around the countdown as frames have gone in the second
   (copies, set by entry_countdown_count, at most 16); with a new second, the next of the eight cars
   (car). */
void entry_car_ring_draw(void)
{
	uint32_t carry = 0;
	uint32_t animation = 0, copy_count = 0, ignored = 0, look = 0, ring_angle = 0, ring_car = 0, ring_x = 0, ring_y = 0;
	uint8_t copies = 0;

	copy_count = OBJECT(current_object)->entry_car_ring.copies;
	if (copy_count == 0u)
	{
		ring_car = OBJECT(current_object)->entry_car_ring.car;
		ring_car = (uint8_t)(ring_car + 1u);
		ring_car = ring_car & 7u;
		OBJECT(current_object)->entry_car_ring.car = ring_car;
		look = turntable_car_looks_ADDRESS + ring_car * 4u;
		animation = ANIMATION_PALETTE(look)->animation;
		OBJECT(current_object)->palette = ANIMATION_PALETTE(look)->palette + 4u;
		animation_start(animation);
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		return;
	}
	if ((int8_t)copy_count > 0x10)
		copy_count = 0x10u;
	OBJECT(current_object)->entry_car_ring.copies = copy_count;
	do
	{
		ring_angle = OBJECT(current_object)->entry_car_ring.copies;
		ring_angle = (ring_angle & 0xFu) << 8;
		ring_x = sine_table[ring_angle];
		ring_x += ring_x * 2u * 2u;
		carry = (ring_x >> 9) & 1u;
		ring_x = (uint16_t)(((int32_t)ring_x >> 10) + 0xA0u + carry);
		ring_y = 0u - cosine_table[ring_angle];
		ring_y += ring_y * 2u * 2u;
		carry = (ring_y >> 9) & 1u;
		OBJECT(current_object)->x = ring_x;
		OBJECT(current_object)->y = ((int32_t)ring_y >> 10) + 0x70u + carry;
		draw_object(ring_angle);
		copies = OBJECT(current_object)->entry_car_ring.copies;
		OBJECT(current_object)->entry_car_ring.copies = copies - 1u;
	} while (copies != 1u);
	animation_advance(0u);
	animation_advance(ignored);
}

/* Sends cars up the road behind the entry countdown (entry_traffic_car), by state: 0 one chance in 16
   each frame to start one; 1 then waits 10 frames (delay). */
void entry_traffic_run(void)
{
	uint32_t object = 0, random = 0;
	uint16_t delay = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		random = random_number();
		if ((random & 0xFu) == 0u)
		{
			object = start_object();
			OBJECT(object)->handler = entry_traffic_car_drive_ADDRESS;
			OBJECT(current_object)->entry_traffic.delay = 0xAu;
			OBJECT(current_object)->state++;
		}
		return;
	case 1:
		delay = OBJECT(current_object)->entry_traffic.delay;
		OBJECT(current_object)->entry_traffic.delay = delay - 1u;
		if (delay == 1u)
			OBJECT(current_object)->state--;
		return;
	}
}

/* A car coming up the road toward the view behind the entry countdown (car: a random one of the eight,
   turntable_car_looks), by state: 0 sets it up and picks its way: 1 along the path at 0067B2 (scale, x
   and y a frame, path_step) seen from the front and one side (frame 6), 2 the same path mirrored (x = 96
   - x) from the other side (frame 7); one car in 128 goes to 3 or 4 instead, the same paths, but after
   45 frames (spin_delay) its frames, its views all round, run five a frame (it spins) with
   set_tyre_noise(4) each frame; at the end of the path it requests its car's sound (car_pass_sounds) and
   ends. */
void entry_traffic_car_drive(void)
{
	uint8_t pass_sound = 0, tyre_noise = 0;
	uint32_t animation = 0, ignored = 0, look = 0, mirrored_x = 0, path_size = 0, path_step = 0, random_swapped = 0, traffic_car = 0, traffic_path_step = 0;
	uint8_t spin_chance = 0;
	uint16_t spin_delay = 0, spin_delay_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 3:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay == 0u)
		{
			animation_advance(0u);
			animation_advance(ignored);
			animation_advance(ignored);
			animation_advance(ignored);
			animation_advance(ignored);
			tyre_noise = 4u;
			set_tyre_noise(tyre_noise);
		}
		else
		{
			spin_delay = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay - 1u;
			if (spin_delay == 1u)
			{
				animation_advance(0u);
				animation_advance(ignored);
				animation_advance(ignored);
				animation_advance(ignored);
				animation_advance(ignored);
				tyre_noise = 4u;
				set_tyre_noise(tyre_noise);
			}
		}
	case 1:
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
		path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		traffic_path_step += 2u;
		if (path_size != 0u)
		{
			OBJECT(current_object)->width_scale = path_size;
			OBJECT(current_object)->height_scale = path_size;
			OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			draw_object(path_size);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay == 0u)
		{
			animation_advance(0u);
			animation_advance(ignored);
			animation_advance(ignored);
			animation_advance(ignored);
			animation_advance(ignored);
			pass_sound = 4u;
			set_tyre_noise(pass_sound);
		}
		else
		{
			spin_delay_2 = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay_2 - 1u;
			if (spin_delay_2 == 1u)
			{
				animation_advance(0u);
				animation_advance(ignored);
				animation_advance(ignored);
				animation_advance(ignored);
				animation_advance(ignored);
				pass_sound = 4u;
				set_tyre_noise(pass_sound);
			}
		}
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
		mirrored_x = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		traffic_path_step += 2u;
		if (mirrored_x != 0u)
		{
			OBJECT(current_object)->width_scale = mirrored_x;
			OBJECT(current_object)->height_scale = mirrored_x;
			OBJECT(current_object)->x = 0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			draw_object(mirrored_x);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	case 0:
		traffic_car = random_number();
		random_swapped = byte_swap32(traffic_car);
		traffic_car &= 7u;
		OBJECT(current_object)->entry_traffic_car.car = traffic_car;
		look = turntable_car_looks_ADDRESS + traffic_car * 4u;
		animation = ANIMATION_PALETTE(look)->animation;
		OBJECT(current_object)->palette = ANIMATION_PALETTE(look)->palette;
		animation_start(animation);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 3u;
		OBJECT(current_object)->frame = 6u;
		OBJECT(current_object)->state++;
		spin_chance = random_swapped & 0x7Fu;
		random_swapped = (random_swapped & ~0xFFu) | spin_chance;
		if (spin_chance == 0u)
		{
			OBJECT(current_object)->entry_traffic_car.spin_delay = 0x2Du;
			OBJECT(current_object)->state += 2u;
		}
		if ((int32_t)random_swapped >= 0)
		{
			path_step = OBJECT(current_object)->entry_traffic_car.path_step;
			OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
			traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
			path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
			traffic_path_step += 2u;
			if (path_size != 0u)
			{
				OBJECT(current_object)->width_scale = path_size;
				OBJECT(current_object)->height_scale = path_size;
				OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
				OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
				draw_object(path_size);
				return;
			}
			pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(pass_sound);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->frame = 7u;
		OBJECT(current_object)->state++;
	case 2:
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
		mirrored_x = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		traffic_path_step += 2u;
		if (mirrored_x != 0u)
		{
			OBJECT(current_object)->width_scale = mirrored_x;
			OBJECT(current_object)->height_scale = mirrored_x;
			OBJECT(current_object)->x = 0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			draw_object(mirrored_x);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	}
}

/* Gives the object the next palette (palette, an index into the sprites' palette words, sprite_palettes)
   of its palette_list; a negative entry goes back by that many bytes, to loop. */
void step_palette(void)
{
	uint32_t list_place = 0, palette_entry = 0;

	list_place = OBJECT(current_object)->step_palette.palette_list;
	palette_entry = OBJECT_PALETTE_STEPS(list_place)[0];
	list_place += 2u;
	if ((int16_t)palette_entry < 0)
	{
		list_place += palette_entry;
		palette_entry = (uint16_t)OBJECT_PALETTE_STEPS(list_place)[0];
		list_place += 2u;
	}
	OBJECT(current_object)->palette = palette_entry;
	OBJECT(current_object)->step_palette.palette_list = list_place;
}
