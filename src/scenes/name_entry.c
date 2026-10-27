/**
 *    Name:        scenes/name_entry.c
 *    Description: the name entry after a record: the certificate, the letters' drum, the LCD, the car, the drivers and the course's name
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
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
   frames, spin_frames; a letter is 80 of the turn's 1000); 2 the wheel turns the drum a letter at a
   time, 4 frames a letter (turn_steps; a turn under way turns back if the wheel goes the other way),
   sooner the further it leans (turn_delay counts down by name_entry_turn_rates by the wheel), and << and
   >> a letter each time; the letter in front is letter, and shows blinking at the name's next place; the
   gas pedal let up (under 40) then pressed (C0 or more), START or the music button enters it with a
   sound (name_entry_place_sounds by place): the left arrow takes the last letter back, the right arrow
   enters a space, ED or the third letter ends the entry, as does the timer running out (the seconds left
   show as the countdown number, time_digits_draw); then open 0, an empty name or one of the names of
   name_entry_rejected_names (SEX, XXX, XX and X's with spaces) becomes ---, 60 frames; 3 writes the
   record into the race record (record_serial + 1, finish_time the time, unk_12 the car's model,
   unk_13-15 the name, record_course the course, screen built_stage - 15), turns the lamps off and sets
   the game's next step (screen game_mode byte 2 = 0x15 (step 21)); 4 nothing. Every frame it draws the
   time and the name (draw_text). */
void name_entry_update(void)
{
	bool L_00200F = false, L_00209D = false, L_0020B6 = false, L_0020FB = false, L_00212C = false, L = false, L_002133 = false;
	uint8_t letter_character = 0, letter_index = 0, new_steps = 0, sound_code = 0, steps = 0, turn_rate = 0;
	uint16_t buttons_pressed = 0, new_angle = 0, turn_buttons = 0;
	uint32_t background_setup = 0, entered_name = 0, handler_list = 0, ignored = 0, letter = 0, name_place = 0, object = 0, palette_fades = 0, player_time = 0, race = 0, rejected_name = 0, remaining_time = 0, saved_letter = 0, text_field = 0, text_record = 0, wheel_position = 0;
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
			ignored = draw_text(text_record);
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
										if (L || !L_00212C)
										{
											if (L || !L_0020FB)
											{
												if (L || letter == 0x1Eu)
												{
													L = false;
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
												L = true;
											}
										}
									} while (L);
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
						ignored = draw_text(text_record);
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
		entered_name = OBJECT(current_object)->name_entry.name_word;
		if (entered_name == 0u)
			OBJECT(current_object)->name_entry.name_word = 0x2D2D2Du;
		else
		{
			rejected_name = name_entry_rejected_names_ADDRESS;
			while ((int32_t)REJECTED_NAMES(rejected_name)[0] >= 0)
			{
				element = REJECTED_NAMES(rejected_name)[0];
				rejected_name += 4u;
				if (entered_name == element)
				{
					OBJECT(current_object)->name_entry.name_word = 0x2D2D2Du;
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
			RACE(race)->finish_time = SCREEN(screen)->checkpoint_times_by_player[SCREEN(screen)->player + 0x28u];
			RACE(race)->record_model = CAR(player_car)->model;
			RACE(race)->record_name = OBJECT(current_object)->name_entry.name;
			RACE(race)->record_name_rest = (uint16_t)(OBJECT(current_object)->name_entry.name_word >> 8u);
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
			RACE(race)->finish_time = SCREEN(screen)->checkpoint_times_by_player[SCREEN(screen)->player + 0x28u];
			RACE(race)->record_model = CAR(player_car)->model;
			RACE(race)->record_name = OBJECT(current_object)->name_entry.name;
			RACE(race)->record_name_rest = (uint16_t)(OBJECT(current_object)->name_entry.name_word >> 8u);
			RACE(race)->record_course = SCREEN(screen)->built_stage - 0x15u;
			OBJECT(current_object)->state++;
		}
		text_record = current_object + 0xA4u;
		draw_text(text_record);
		text_record = current_object + 0x9Au;
		ignored = draw_text(text_record);
		return;
	case 4:
		break;
	default:
		invalid_jump(name_entry_state_jumps[OBJECT(current_object)->state]);
	}
	text_record = current_object + 0xA4u;
	draw_text(text_record);
	text_record = current_object + 0x9Au;
	ignored = draw_text(text_record);
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
		move_bytes(screen + 0xF13u, OBJECT(current_object)->name_entry_lcd.name_entry + 0xAAu, 7u);
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
