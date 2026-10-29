/**
 *    Name:        scenes/name_entry.c
 *    Description: the name entry after a record: the certificate, the letters' drum, the LCD, the car, the drivers and the course's name
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

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
		if (SCREEN(screen)->places[(int32_t)player] == 0u && SCREEN(screen)->checkpoint_times_by_player[(int32_t)(player + 0x28u)] < course_record_times[course_number * 3u])
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
   lamps (button_lamps) and clears lcd_restart_frames, starts name_entry_lcd, the background
   (set_up_background with 002E4C by screen; start_palette_fades with 13F6C0), the countdown number
   (time_digits_start), name_entry_object_handlers (name_entry_car, name_entry_drivers,
   name_entry_course_name, passing_traffic) and the 32 name_entry_letter objects; open, timer 600 frames,
   wheel_angle A80, the name's text record at x F8, y 54 and the time's at x A8, y 54 (time_to_text of
   the player's time, screen 410 + 2 * player); 1 the drum spins in (wheel_angle + 20 a frame for 40
   frames, spin_frames; a letter is 80 of the turn's 1000), with the seconds left (name_entry_draw_time);
   2 the wheel turns the drum a letter at a time, 4 frames a letter (turn_steps; a turn under way turns
   back if the wheel goes the other way), name_entry_read_wheel takes the next turn when none is under
   way and name_entry_choose the letter in front, the entry and the time; 3 name_entry_store_record; 4
   nothing. Every frame it draws the time and the name (draw_text). */
void name_entry_update(void)
{
	uint8_t letter_index = 0, steps = 0;
	uint16_t new_angle = 0;
	uint32_t background_setup = 0, handler_list = 0, object = 0, palette_fades = 0, player_time = 0, text_field = 0, text_record = 0;
	uint8_t letters_left = 0;
	uint16_t spin_frames = 0;

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
		handler_list = name_entry_objects_ADDRESS;
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
		player_time = SCREEN(screen)->checkpoint_times_by_player[SCREEN(screen)->player + 0x28u];
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
		OBJECT(current_object)->name_entry.wheel_angle += 0x20u;
		spin_frames = OBJECT(current_object)->name_entry.spin_frames;
		OBJECT(current_object)->name_entry.spin_frames = spin_frames - 1u;
		if (spin_frames != 1u)
		{
			name_entry_draw_time();
			return;
		}
		OBJECT(current_object)->state++;
		break;
	case 1:
		OBJECT(current_object)->name_entry.wheel_angle += 0x20u;
		spin_frames = OBJECT(current_object)->name_entry.spin_frames;
		OBJECT(current_object)->name_entry.spin_frames = spin_frames - 1u;
		if (spin_frames != 1u)
		{
			name_entry_draw_time();
			return;
		}
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	case 3:
		name_entry_store_record();
		return;
	case 4:
		text_record = current_object + 0xA4u;
		draw_text(text_record);
		text_record = current_object + 0x9Au;
		draw_text(text_record);
		return;
	}
	steps = (uint8_t)OBJECT(current_object)->name_entry.turn_steps;
	if (steps == 0u)
	{
		name_entry_read_wheel();
		return;
	}
	if ((int8_t)steps >= 0)
	{
		if (SCREEN(screen)->wheel < 0xA0u)
		{
			OBJECT(current_object)->name_entry.turn_steps = steps - 1u;
			new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
			OBJECT(current_object)->name_entry.wheel_angle = new_angle;
			if ((uint8_t)OBJECT(current_object)->name_entry.turn_steps != 0u)
			{
				name_entry_choose();
				return;
			}
			name_entry_read_wheel();
			return;
		}
		steps -= 4u;
		OBJECT(current_object)->name_entry.turn_steps = steps + 1u;
		new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
		OBJECT(current_object)->name_entry.wheel_angle = new_angle;
		if ((uint8_t)OBJECT(current_object)->name_entry.turn_steps != 0u)
		{
			name_entry_choose();
			return;
		}
		name_entry_read_wheel();
		return;
	}
	if (SCREEN(screen)->wheel >= 0x60u)
	{
		OBJECT(current_object)->name_entry.turn_steps = steps + 1u;
		new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle - 0x20u);
		OBJECT(current_object)->name_entry.wheel_angle = new_angle;
		if ((uint8_t)OBJECT(current_object)->name_entry.turn_steps != 0u)
		{
			name_entry_choose();
			return;
		}
		name_entry_read_wheel();
		return;
	}
	steps += 4u;
	OBJECT(current_object)->name_entry.turn_steps = steps - 1u;
	new_angle = (uint16_t)(OBJECT(current_object)->name_entry.wheel_angle + 0x20u);
	OBJECT(current_object)->name_entry.wheel_angle = new_angle;
	if ((uint8_t)OBJECT(current_object)->name_entry.turn_steps != 0u)
	{
		name_entry_choose();
		return;
	}
	name_entry_read_wheel();
}

/* State 2's reading of the wheel for the drum's next turn (name_entry_update goes on to it when no turn
   is under way, or when one has just ended): the wheel in sixteenths gives a turn rate
   (name_entry_turn_rates: none in the middle, where << and >> turn it at rate F, one way or the other,
   and with neither turn_delay goes to 1); turn_delay counts down by it, and when it runs out (back to F)
   a turn of 4 steps starts (turn_steps 4, or -4 the other way). Then name_entry_choose. */
void name_entry_read_wheel(void)
{
	uint8_t new_steps = 0, turn_rate = 0;
	uint16_t turn_buttons = 0;
	uint32_t wheel_position = 0;
	uint8_t turn_delay = 0;

	wheel_position = SCREEN(screen)->wheel;
	wheel_position = wheel_position >> 4;
	turn_rate = name_entry_turn_rates[wheel_position];
	if (turn_rate == 0u)
	{
		turn_buttons = (uint16_t)SCREEN(screen)->buttons_pressed;
		if ((turn_buttons & 0x200u) != 0u)
		{
			if ((turn_buttons & 0x400u) != 0u)
			{
				name_entry_choose();
				return;
			}
			wheel_position = 0xFFu;
		}
		else
		{
			if ((turn_buttons & 0x400u) == 0u)
			{
				OBJECT(current_object)->name_entry.turn_delay = 1;
				name_entry_choose();
				return;
			}
			wheel_position = 0u;
		}
		turn_rate = 0xFu;
	}
	turn_delay = OBJECT(current_object)->name_entry.turn_delay;
	OBJECT(current_object)->name_entry.turn_delay = turn_delay - turn_rate;
	if ((int8_t)turn_rate < (int8_t)turn_delay)
	{
		name_entry_choose();
		return;
	}
	OBJECT(current_object)->name_entry.turn_delay = 0xF;
	new_steps = 4u;
	if (wheel_position >> 2 != 0u)
		new_steps = (uint8_t)(0u - new_steps);
	OBJECT(current_object)->name_entry.turn_steps = new_steps;
	name_entry_choose();
}

/* The rest of state 2 (name_entry_update and name_entry_read_wheel go on to it): the letter in front of
   the pointer (letter, from wheel_angle); the gas pedal let up (under 40) then pressed (C0 or more),
   START or the music button enters it with a sound (name_entry_place_sounds by place): the left arrow
   takes the last letter back, the right arrow enters a space, ED or a third letter ends the name
   (name_entry_finish); the letter in front blinks at the name's next place; the timer counts down, the
   name ends when it runs out (name_entry_finish), and the seconds left show (name_entry_draw_time). */
void name_entry_choose(void)
{
	uint8_t letter_character = 0, sound_code = 0;
	uint16_t buttons_pressed = 0;
	uint32_t letter = 0, name_place = 0, saved_letter = 0;
	uint32_t kept_letter = 0, kept_place = 0;
	uint8_t entered_letter = 0;
	uint16_t timer = 0;

	OBJECT(current_object)->name_entry.letter = ((OBJECT(current_object)->name_entry.wheel_angle + 0x40u) & 0xFFFu) >> 7;
	switch (OBJECT(current_object)->name_entry.gas_step)
	{
	case 0:
		if (SCREEN(screen)->gas >= 0x40u)
			break;
		OBJECT(current_object)->name_entry.gas_step++;
	case 1:
		if (SCREEN(screen)->gas < 0xC0u)
		{
			buttons_pressed = (uint16_t)SCREEN(screen)->buttons_pressed;
			if ((buttons_pressed & 0x100u) == 0u && (buttons_pressed & 0x40u) == 0u)
				break;
		}
		letter = OBJECT(current_object)->name_entry.letter;
		name_place = OBJECT(current_object)->name_entry.place;
		switch (name_place)
		{
		case 0:
			if (letter == 0x1Eu)
			{
				OBJECT(current_object)->name_entry.gas_step--;
				name_place = OBJECT(current_object)->name_entry.place;
				letter_character = name_entry_letters[OBJECT(current_object)->name_entry.letter];
				if ((game_frame_count & 4u) == 0u)
					letter_character = 0u;
				OBJECT(current_object)->name_entry.name[(int32_t)name_place] = letter_character;
				timer = OBJECT(current_object)->name_entry.timer;
				OBJECT(current_object)->name_entry.timer = timer - 1u;
				if (timer != 1u)
				{
					name_entry_draw_time();
					return;
				}
				name_entry_finish(name_place);
				return;
			}
			if (letter == 0x1Fu)
			{
				name_entry_finish(name_place);
				return;
			}
			kept_letter = letter;
			kept_place = name_place;
			sound_code = name_entry_place_sounds[name_place];
			request_sound(sound_code);
			name_place = kept_place;
			saved_letter = kept_letter;
			entered_letter = saved_letter;
			letter_character = name_entry_letters[saved_letter];
			if (entered_letter == 0x1Du)
				letter_character = 0x20u;
			OBJECT(current_object)->name_entry.name[(int32_t)name_place] = letter_character;
			name_place = (uint8_t)(name_place + 1u);
			if (name_place == 3u)
			{
				name_entry_finish(name_place);
				return;
			}
			break;
		case 1:
		case 2:
			if (letter == 0x1Eu)
			{
				OBJECT(current_object)->name_entry.name[(int32_t)name_place] = 0u;
				name_place = (uint8_t)(name_place - 1u);
				break;
			}
			if (letter == 0x1Fu)
			{
				name_entry_finish(name_place);
				return;
			}
			kept_letter = letter;
			kept_place = name_place;
			sound_code = name_entry_place_sounds[name_place];
			request_sound(sound_code);
			name_place = kept_place;
			saved_letter = kept_letter;
			entered_letter = saved_letter;
			letter_character = name_entry_letters[saved_letter];
			if (entered_letter == 0x1Du)
				letter_character = 0x20u;
			OBJECT(current_object)->name_entry.name[(int32_t)name_place] = letter_character;
			name_place = (uint8_t)(name_place + 1u);
			if (name_place == 3u)
			{
				name_entry_finish(name_place);
				return;
			}
			break;
		}
		OBJECT(current_object)->name_entry.place = name_place;
		OBJECT(current_object)->name_entry.gas_step--;
		break;
	}
	name_place = OBJECT(current_object)->name_entry.place;
	letter_character = name_entry_letters[OBJECT(current_object)->name_entry.letter];
	if ((game_frame_count & 4u) == 0u)
		letter_character = 0u;
	OBJECT(current_object)->name_entry.name[(int32_t)name_place] = letter_character;
	timer = OBJECT(current_object)->name_entry.timer;
	OBJECT(current_object)->name_entry.timer = timer - 1u;
	if (timer != 1u)
	{
		name_entry_draw_time();
		return;
	}
	name_entry_finish(name_place);
}

/* The end of the name (name_entry_choose goes on to it: ED, the third letter, or the time up): open 0,
   the name closed at its place; an empty name, or one of name_entry_rejected_names (SEX, XXX, XX and X's
   with spaces), becomes ---; state 3 with 60 frames (timer), and its first frame
   (name_entry_store_record). */
void name_entry_finish(uint32_t name_place)
{
	uint32_t entered_name = 0, rejected_name = 0;
	uint32_t element = 0;

	OBJECT(current_object)->name_entry.open = 0u;
	OBJECT(current_object)->name_entry.name[(int32_t)name_place] = 0u;
	entered_name = (OBJECT(current_object)->name_entry.name[0] | ((uint32_t)OBJECT(current_object)->name_entry.name[1] << 8u) | ((uint32_t)OBJECT(current_object)->name_entry.name[2] << 16u) | ((uint32_t)OBJECT(current_object)->name_entry.name[3] << 24u));
	if (entered_name == 0u)
	{
		OBJECT(current_object)->name_entry.name[0] = 0x2Du;
		OBJECT(current_object)->name_entry.name[1] = 0x2Du;
		OBJECT(current_object)->name_entry.name[2] = 0x2Du;
		OBJECT(current_object)->name_entry.name[3] = 0u;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->name_entry.timer = 0x3Cu;
		OBJECT(current_object)->name_entry.spin_frames = 0u;
		name_entry_store_record();
		return;
	}
	rejected_name = name_entry_rejected_names_ADDRESS;
	do
	{
		if ((int32_t)REJECTED_NAMES(rejected_name)[0] < 0)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->name_entry.timer = 0x3Cu;
			OBJECT(current_object)->name_entry.spin_frames = 0u;
			name_entry_store_record();
			return;
		}
		element = REJECTED_NAMES(rejected_name)[0];
		rejected_name += 4u;
	} while (entered_name != element);
	OBJECT(current_object)->name_entry.name[0] = 0x2Du;
	OBJECT(current_object)->name_entry.name[1] = 0x2Du;
	OBJECT(current_object)->name_entry.name[2] = 0x2Du;
	OBJECT(current_object)->name_entry.name[3] = 0u;
	OBJECT(current_object)->state++;
	OBJECT(current_object)->name_entry.timer = 0x3Cu;
	OBJECT(current_object)->name_entry.spin_frames = 0u;
	name_entry_store_record();
}

/* State 3 of name_entry_update (and the frame name_entry_finish ends the name): after 60 frames (timer)
   writes the record into the race record (record_serial + 1, finish_time the player's time, record_model
   the car's model, record_name and record_name_rest the name, record_course the course: screen
   built_stage - 15), turns the lamps off (button_lamps), sets the game's next step (game_step 21) and
   state 4; every frame it draws the time and the name (draw_text). */
void name_entry_store_record(void)
{
	uint32_t race = 0, text_record = 0;
	uint16_t timer = 0;

	timer = OBJECT(current_object)->name_entry.timer;
	OBJECT(current_object)->name_entry.timer = timer - 1u;
	if (timer == 1u)
	{
		SCREEN(screen)->button_lamps = 0u;
		SCREEN(screen)->game_step = 0x15u;
		race = SCREEN(screen)->race;
		RACE(race)->record_serial++;
		RACE(race)->finish_time = SCREEN(screen)->checkpoint_times_by_player[SCREEN(screen)->player + 0x28u];
		RACE(race)->record_model = CAR(player_car)->model;
		RACE(race)->record_name = OBJECT(current_object)->name_entry.name[0];
		RACE(race)->record_name_rest = (OBJECT(current_object)->name_entry.name[1] | ((uint32_t)OBJECT(current_object)->name_entry.name[2] << 8u));
		RACE(race)->record_course = SCREEN(screen)->built_stage - 0x15u;
		OBJECT(current_object)->state++;
	}
	text_record = current_object + 0xA4u;
	draw_text(text_record);
	text_record = current_object + 0x9Au;
	draw_text(text_record);
}

/* The seconds left as the countdown number (timer frames / 30 in BCD, time_digits_draw), then the time
   and the name (draw_text): each frame of states 1 and 2. */
void name_entry_draw_time(void)
{
	uint32_t remaining_time = 0, text_record = 0;

	remaining_time = OBJECT(current_object)->name_entry.timer;
	remaining_time = remaining_time / 0x1Eu;
	remaining_time = binary_to_bcd(remaining_time);
	time_digits_draw(remaining_time);
	text_record = current_object + 0xA4u;
	draw_text(text_record);
	text_record = current_object + 0x9Au;
	draw_text(text_record);
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
		animation = NAME_ENTRY_FONT;
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
	OBJECT(current_object)->width_scale = (int32_t)(letter_scale * OBJECT(current_object)->name_entry_letter.width_factor) >> 8;
	draw_object(letter_scale);
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
		move_bytes(screen + 0xF13u, OBJECT(current_object)->name_entry_lcd.name_entry + 0xAAu, 7u);
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	}
	object = OBJECT(current_object)->name_entry_lcd.name_entry;
	name_place = object + 0xA0u;
	lcd_place = screen + 0xF1Cu;
	characters_left = 3u;
	do
	{
		lcd_character = NAME_LETTERS(name_place)[0];
		name_place++;
		if (name_place == 0u)
		{
			do
			{
				LCD_CHARACTERS(lcd_place)[0] = 0x20u;
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
		LCD_CHARACTERS(lcd_place)[0] = lcd_character;
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
	uint32_t animation = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		draw_object(0u);
		return;
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
	uint32_t animation = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		draw_object(0u);
		return;
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
	uint32_t animation = 0, course_number = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		draw_object(0u);
		return;
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
	draw_object(course_number);
}
