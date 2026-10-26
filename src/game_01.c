/**
 *    Name:        game_01.c
 *    Description: the game's functions, part 01 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets up a flag over the car another player has chosen (animation 1B, a palette by the car,
   choice_flag_palettes; angle: the car's place on the turntable), starts its number (choice_flag_number)
   and goes on to choice_flag_follow. */
void choice_flag_start(void)
{
	uint32_t animation = 0, car = 0, object = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	animation = 0x1Bu;
	animation_start(animation);
	car = OBJECT(current_object)->choice_flag.car;
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
	perspective = draw_object(perspective);
}

/* Sets up the flag of one more player on a car that has one (on the last flag there, below), starts its
   number (choice_flag_number) and goes on to stacked_choice_flag_follow. */
void stacked_choice_flag_start(void)
{
	uint32_t animation = 0, object = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	animation = 0x1Bu;
	animation_start(animation);
	OBJECT(current_object)->palette = choice_flag_palettes[OBJECT(current_object)->stacked_choice_flag.car];
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
	uint32_t flag_y = 0, object = 0;
	uint8_t below_shown = 0;

	object = OBJECT(current_object)->stacked_choice_flag.below;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		below_shown = ~~OBJECT(object)->choice_flag.visible;
		OBJECT(current_object)->stacked_choice_flag.visible = below_shown;
		if (below_shown != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 3u;
			flag_y = (uint16_t)(OBJECT(object)->y - 0x14u);
			OBJECT(current_object)->y = flag_y;
			flag_y = draw_object(flag_y);
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
	uint32_t number_y = 0, object = 0;
	uint8_t flag_shown = 0;

	object = OBJECT(current_object)->choice_flag_number.flag;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		flag_shown = ~~OBJECT(object)->choice_flag.visible;
		OBJECT(current_object)->choice_flag_number.visible = flag_shown;
		if (flag_shown != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 0x13u;
			number_y = (uint16_t)(OBJECT(object)->y - 0x10u);
			OBJECT(current_object)->y = number_y;
			number_y = draw_object(number_y);
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
uint32_t turntable_draw(uint32_t across, uint32_t down, uint32_t point_depth)
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
			scale = shift_signed_by(scale, fraction_shift, 32);
			scale = (scale & ~0xFFFFu) | (uint16_t)(scale + carry);
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
	default:
		invalid_jump(car_select_gas_down_jumps[OBJECT(current_object)->car_select.gas_down]);
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
	animation = 0x26Fu;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x4Eu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x7Au;
	OBJECT(current_object)->handler = entry_countdown_count_ADDRESS;
	entry_countdown_count();
}

/* Shows the entry time left (RACE_COPY_1's entry_frames, frames): the seconds as the big number (its
   unk_40), the frames into the second to the ring of cars, and on the LCD a shrinking bar
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
	move_string(entry_countdown_lcd_texts[(remainder + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	remainder = 0u;
	divux_pair(0xAu, &time_shown, &remainder);
	zero = (uint32_t)(time_shown == 0u);
	if (zero == 0u)
	{
		time_shown = (time_shown & ~0xFFu) | (uint8_t)(time_shown + 0x30u);
		SCREEN(screen)->lcd_text_8_to_11 = (SCREEN(screen)->lcd_text_8_to_11 & 0xFFFFFF00u) | (time_shown & 0xFFu);
	}
	SCREEN(screen)->lcd_text_8_to_11 = (SCREEN(screen)->lcd_text_8_to_11 & 0xFFFF00FFu) | ((remainder + 0x30u & 0xFFu) << 8u);
	time_shown = draw_object(time_shown);
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

	blink_frames = OBJECT(current_object)->entry_banner.blink;
	blink_frames = (uint8_t)(blink_frames + 1u);
	OBJECT(current_object)->entry_banner.blink = blink_frames;
	if ((blink_frames & 0x10u) != 0u)
		return;
	blink_frames = draw_object(blink_frames);
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
	uint32_t animation = 0, copy_count = 0, ignored = 0, look = 0, ring_angle = 0, ring_car = 0, ring_x = 0, ring_y = 0, y_by_four = 0;
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
		y_by_four = ring_y * 2u * 2u;
		ring_y += y_by_four;
		carry = (ring_y >> 9) & 1u;
		OBJECT(current_object)->x = ring_x;
		OBJECT(current_object)->y = ((int32_t)ring_y >> 10) + 0x70u + carry;
		draw_object(y_by_four);
		copies = OBJECT(current_object)->entry_car_ring.copies;
		OBJECT(current_object)->entry_car_ring.copies = copies - 1u;
	} while (copies != 1u);
	ignored = animation_advance(0u);
	ignored = animation_advance(ignored);
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
	default:
		invalid_jump(entry_traffic_state_jumps[OBJECT(current_object)->state]);
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
	uint32_t animation = 0, ignored = 0, look = 0, mirrored_x = 0, path_size = 0, path_step = 0, random_swapped = 0, size = 0, traffic_car = 0, traffic_path_step = 0;
	uint8_t spin_chance = 0;
	uint16_t spin_delay = 0, spin_delay_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 3:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay != 0u)
		{
			spin_delay = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay - 1u;
			if (spin_delay == 1u)
			{
				ignored = animation_advance(0u);
				ignored = animation_advance(ignored);
				ignored = animation_advance(ignored);
				ignored = animation_advance(ignored);
				animation_advance(ignored);
				tyre_noise = 4u;
				set_tyre_noise(tyre_noise);
			}
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
				path_size = draw_object(path_size);
				return;
			}
			pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(pass_sound);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		ignored = animation_advance(0u);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		animation_advance(ignored);
		tyre_noise = 4u;
		set_tyre_noise(tyre_noise);
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
			path_size = draw_object(path_size);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
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
			path_size = draw_object(path_size);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay != 0u)
		{
			spin_delay_2 = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay_2 - 1u;
			if (spin_delay_2 == 1u)
			{
				ignored = animation_advance(0u);
				ignored = animation_advance(ignored);
				ignored = animation_advance(ignored);
				ignored = animation_advance(ignored);
				animation_advance(ignored);
				pass_sound = 4u;
				set_tyre_noise(pass_sound);
			}
			path_step = OBJECT(current_object)->entry_traffic_car.path_step;
			OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
			traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
			size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
			traffic_path_step += 2u;
			if (size == 0u)
			{
				pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
				request_sound(pass_sound);
				OBJECT(current_object)->flags = 0u;
				return;
			}
			OBJECT(current_object)->width_scale = size;
			OBJECT(current_object)->height_scale = size;
			mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
			OBJECT(current_object)->x = mirrored_x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			mirrored_x = draw_object(mirrored_x);
			return;
		}
		ignored = animation_advance(0u);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		animation_advance(ignored);
		pass_sound = 4u;
		set_tyre_noise(pass_sound);
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		traffic_path_step += 2u;
		if (size == 0u)
		{
			pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(pass_sound);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	case 2:
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		traffic_path_step += 2u;
		if (size == 0u)
		{
			pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(pass_sound);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	default:
		invalid_jump(entry_traffic_car_state_jumps[OBJECT(current_object)->state]);
	}
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
			path_size = draw_object(path_size);
			return;
		}
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->frame = 7u;
	OBJECT(current_object)->state++;
	path_step = OBJECT(current_object)->entry_traffic_car.path_step;
	OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
	traffic_path_step = traffic_path_ADDRESS + path_step * 3u * 2u;
	size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
	traffic_path_step += 2u;
	if (size == 0u)
	{
		pass_sound = car_pass_sounds[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(pass_sound);
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->width_scale = size;
	OBJECT(current_object)->height_scale = size;
	mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
	OBJECT(current_object)->x = mirrored_x;
	OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
	mirrored_x = draw_object(mirrored_x);
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
		palette_entry = mem_read16(list_place);
		list_place += 2u;
	}
	OBJECT(current_object)->palette = palette_entry;
	OBJECT(current_object)->step_palette.palette_list = list_place;
}

/* The screen's car, each frame: player_car_check_place, player_car_read_controls,
   player_car_build_hit_boxes, then by state: 0 sets up the race (the view's constants in screen
   eye_distance-18, the course, the car on its grid place: segment 16 + grid_segment_lists by players and
   place, road_x from grid_road_place_lists, its world position; the palette of its model,
   player_car_palettes; timer 8 frames in the demo, 149 in a race with the intro view car
   view_shift_intro/unk_10E; the gears, torque curves, crash physics, with screen is_host the race's time
   left (race time_left: 40 seconds plus master_time_difficulty), the engine sound); 1 the countdown to
   the start: brake full and the wheel centred until timer runs out (others read it as the frames to the
   start), then car moving and race started; 2 driving (player_car_move): goes to 3 when the time is up
   (car time_up), to 4 at the goal (car at_goal); counts hit_guard down; on a hit (flags bit 2, by
   hit_kind): 3 a fork sign slows it for 15 frames (car slowed_frames), 0 scenery, 1 a road object
   (hit_other), 2 the other player's car (hit_other): a speed change, a push and a bounce from the
   model's crash table (car crash_table, by impact_speed or the cars' speeds and places), the hit's
   sounds, a crash animation, the CRASH text (car crash_text), immune (flags bit 3), state 5; 5 the crash
   plays out: no controls, the push and the speed change die away, it bounces under gravity (sound 31 on
   landing), back to 2 when the animation ends with 15 frames of hit_guard; 3 time up: gas off and the
   model's brake (player_car_time_up_brakes), 180 frames, then the race is over (car race_over), GAME
   OVER (004771) with one player or race_result_start with two, 60 frames, screen flags bit 4 and DJ
   message 36 or 37; 4 the goal: finished, the race's stage count up, waits for the screen's finish time
   (screen 410) into race finish_time and the race time, race finished, then drives itself
   (player_car_steer_to_stop) to its stop_position by its place (goal_stops), plays the model's arrival
   animation (player_car_arrival_animations), brakes to a stop, 90 frames, screen flags bit 4. */
void player_car_update(void)
{
	bool L_0071FB = false, L_007E41 = false, L_007F31 = false;
	uint32_t overflow = 0;
	uint8_t crash_hit_sound = 0, dj_message = 0, frame_time = 0, landing_sound = 0, tyre_noise = 0;
	uint16_t grid_segment = 0;
	uint32_t arrival_animation = 0, brake_index = 0, crash_animation = 0, crash_entry = 0, crash_sound = 0, distance_index = 0, draw_distances = 0, goal_stop = 0, gravity = 0, grid_list = 0, grid_place = 0, grid_road_place = 0, hit_object = 0, hit_side = 0, ignored = 0, impact = 0, model = 0, new_bounce_speed = 0, new_height = 0, object = 0, old_height = 0, other_record = 0, other_slide = 0, other_weight = 0, other_x = 0, own_slide = 0, own_speed = 0, own_weight = 0, player_time = 0, road_x = 0, screen_view_default = 0, segment_fraction = 0, segment_length = 0, segment_record = 0, side_gap = 0, speed_difference = 0, speed_loss = 0, start_record = 0, view_height_offset = 0;
	uint8_t hit_guard = 0, crash_sound_delay = 0;
	uint16_t timer = 0, timer_2 = 0, timer_3 = 0, timer_4 = 0;
	uint32_t jump_target = 0, stage_count_place = 0;

	wheel_motor[SCREEN(screen)->index] = 0u;
	player_car_check_place();
	player_car_read_controls();
	player_car_build_hit_boxes();
	do
	{
		do
		{
			jump_target = L_0071FB ? 0x7134u : L_007F31 ? 0x70B9u : player_car_state_jumps[OBJECT(current_object)->state];
			switch (jump_target)
			{
			case 0x006F30u:
				SCREEN(screen)->current_stage = SCREEN(screen)->built_stage;
				draw_distances = screen_view_defaults[0].draw_distances;
				distance_index = RACE(SCREEN(screen)->host_record)->racer_count;
				screen_view_default = screen_view_defaults_ADDRESS + 0x4u;
				if (isdn_mode != 0u)
					distance_index = 0u;
				SCREEN(screen)->draw_distance = DRAW_DISTANCE_LIST(draw_distances)[distance_index];
				SCREEN(screen)->eye_distance = SCREEN_VIEW_DEFAULT(screen_view_default - 0x4u)->eye_distance;
				screen_view_default += 2u;
				SCREEN(screen)->sprite_origin_x = SCREEN_VIEW_DEFAULT(screen_view_default - 0x6u)->sprite_origin_x;
				screen_view_default += 2u;
				SCREEN(screen)->sprite_origin_y = SCREEN_VIEW_DEFAULT(screen_view_default - 0x8u)->sprite_origin_y;
				SCREEN(screen)->view_pitch_base = SCREEN_VIEW_DEFAULT(screen_view_default - 0x8u)->view_pitch_bases[CAR(player_car)->model];
				player_car_set_horizon();
				OBJECT(current_object)->state++;
				CAR(player_car)->object = current_object;
				player_car_start_course();
				start_record = screen + 0x2000u;
				CAR(player_car)->window_record = start_record;
				grid_segment = 0x10u;
				CAR(player_car)->window_segment = 0u;
				grid_place = mem_read8(screen + 0x320u + SCREEN(screen)->player);
				grid_list = 0u;
				if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
				{
					grid_list = RACE(SCREEN(screen)->host_record)->racer_count;
					grid_list = (uint8_t)(grid_list - 1u);
				}
				grid_segment += GRID_SEGMENTS(grid_segment_lists[grid_list])[grid_place];
				CAR(player_car)->segment = grid_segment;
				grid_segment = (uint16_t)(grid_segment * 0x20u);
				CAR(player_car)->segment_offset = grid_segment;
				start_record = (start_record & ~0xFFFFu) | (uint16_t)(start_record + grid_segment);
				grid_road_place = GRID_ROAD_PLACES(grid_road_place_lists[grid_list])[grid_place];
				CAR(player_car)->road_x = (uint16_t)grid_road_place;
				CAR(player_car)->segment_remaining = (uint16_t)(grid_road_place >> 16u);
				OBJECT(current_object)->player_car.heading = (OBJECT(start_record)->flags | ((uint32_t)OBJECT(start_record)->flags_high << 8u));
				start_record += 2u;
				OBJECT(current_object)->player_car.pitch = OBJECT(start_record - 0x2u)->unk_02;
				start_record += 2u;
				OBJECT(current_object)->player_car.world_x = grid_road_place + OBJECT(start_record - 0x4u)->handler;
				start_record += 4u;
				OBJECT(current_object)->player_car.world_z = (OBJECT(start_record - 0x8u)->slot | ((uint32_t)OBJECT(start_record - 0x8u)->unk_09 << 8u) | ((uint32_t)OBJECT(start_record - 0x8u)->unk_0A << 16u) | ((uint32_t)OBJECT(start_record - 0x8u)->unk_0B << 24u));
				OBJECT(current_object)->player_car.world_y = (OBJECT(start_record - 0x8u)->state | ((uint32_t)OBJECT(start_record - 0x8u)->unk_0D << 8u) | ((uint32_t)OBJECT(start_record - 0x8u)->unk_0E << 16u) | ((uint32_t)OBJECT(start_record - 0x8u)->unk_0F << 24u));
				OBJECT(current_object)->palette = player_car_palettes[CAR(player_car)->model];
				OBJECT(current_object)->player_car.timer = 8u;
				if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
				{
					OBJECT(current_object)->player_car.timer = 0x95u;
					CAR(player_car)->view_shift_intro = 0xFA60u;
					CAR(player_car)->view_pitch_intro = 0xF0u;
				}
				player_car_load_gears();
				player_car_build_torque_curves();
				player_car_place_on_road();
				player_car_set_horizon();
				player_car_extend_course();
				SCREEN(screen)->shown_view = screen + 0x600u;
				player_car_project_road();
				player_car_load_crash_physics();
				if (SCREEN(screen)->is_host != 0u)
					RACE(SCREEN(screen)->race)->time_left = (RACE(RACE_COPY_1)->master_time_difficulty_0 + 0x28u) * 0x1Eu;
				player_car_start_engine_sound();
				player_car_animate();
				player_car_publish();
				return;
			case 0x007088u:
				timer = OBJECT(current_object)->player_car.timer;
				OBJECT(current_object)->player_car.timer = timer - 1u;
				if (timer != 1u)
				{
					OBJECT(current_object)->player_car.brake = 0xFFu;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					player_car_move();
					return;
				}
				OBJECT(current_object)->state++;
				CAR(player_car)->moving = 0xFFu;
				if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
				{
					RACE(SCREEN(screen)->race)->started = 0xFFu;
					SCREEN(screen)->race_started = 0xFFu;
				}
			case 0x0070B9u:
				if (L_007F31 || CAR(player_car)->time_up == 0u)
				{
					if (!L_007F31)
					{
						L_0071FB = CAR(player_car)->at_goal != 0u;
						if (!L_0071FB)
						{
							if (OBJECT(current_object)->player_car.hit_guard == 0u)
								OBJECT(current_object)->flags &= 0xF7u;
							else
							{
								hit_guard = OBJECT(current_object)->player_car.hit_guard;
								OBJECT(current_object)->player_car.hit_guard = hit_guard - 1u;
								if (hit_guard == 1u)
									OBJECT(current_object)->flags &= 0xF7u;
							}
							player_car_check_car_hits();
							if ((OBJECT(current_object)->flags & 4u) == 0u)
							{
								CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
								player_car_move();
								return;
							}
							switch (OBJECT(current_object)->player_car.hit_kind)
							{
							case 3:
								OBJECT(current_object)->flags &= 0xFBu;
								CAR(player_car)->slowed_frames = 0xFu;
								CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
								player_car_move();
								return;
							case 0:
								crash_entry = CAR(player_car)->crash_table;
								impact = OBJECT(current_object)->player_car.impact_speed;
								if (impact >= 0xFFFFu)
									impact = 0xFFFFu;
								speed_loss = 0u - impact;
								impact = (impact >> 7) + crash_place_offsets[mem_read8(screen + 0x320u + SCREEN(screen)->player)];
								if (impact >= 0x1F0u)
									impact = 0x1F0u;
								crash_entry += (impact & ~0xFFFFu) | (impact & 0x1F0u);
								OBJECT(current_object)->player_car.speed_change = (int32_t)(speed_loss * mem_read16(crash_entry + 4u)) >> 8;
								hit_side = (uint16_t)CAR(player_car)->road_x;
								hit_side = hit_side >> 15;
								OBJECT(current_object)->player_car.sideways_push = mem_read16(crash_entry + 6u + hit_side * 2u);
								OBJECT(current_object)->player_car.bounce_speed = mem_read16(crash_entry + 0xAu);
								break;
							case 1:
								hit_object = (screen & ~0xFFFFu) | OBJECT(current_object)->player_car.hit_other;
								own_speed = OBJECT(current_object)->player_car.speed;
								impact = (uint32_t)(uint16_t)OBJECT(hit_object)->traffic.speed << 8;
								other_weight = OBJECT(hit_object)->traffic.weight;
								side_gap = CAR(player_car)->road_x * 2u;
								other_x = (OBJECT(hit_object)->x_offset + (uint32_t)mem_read16((((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset)) + 0x12u) * OBJECT(hit_object)->side) * 2u;
								own_slide = OBJECT(current_object)->player_car.sideways_speed * 2u;
								other_slide = OBJECT(hit_object)->traffic.sideways_speed * 2u;
								L_007E41 = true;
								break;
							case 2:
								other_record = OBJECT(current_object)->player_car.hit_other;
								other_record = RACE_COPY_1 + (uint16_t)(other_record << 7);
								side_gap = CAR(player_car)->road_x;
								other_x = (int16_t)mem_read16(other_record + 0x44u);
								own_slide = OBJECT(current_object)->player_car.sideways_speed;
								other_slide = (int16_t)mem_read16(other_record + 0x4Eu);
								own_speed = OBJECT(current_object)->player_car.speed;
								impact = mem_read16(other_record + 0x4Cu);
								own_speed = own_speed >> 1;
								impact = impact >> 1;
								other_weight = car_weights[mem_read8(other_record + 0x49u)];
								L_007E41 = true;
								break;
							default:
								invalid_jump(player_car_hit_kind_jumps[OBJECT(current_object)->player_car.hit_kind]);
							}
							if (L_007E41)
							{
								L_007E41 = false;
								own_weight = car_weights[CAR(player_car)->model];
								crash_entry = CAR(player_car)->crash_table;
								impact -= own_speed;
								speed_difference = impact;
								if ((int32_t)impact < 0)
									impact = (0u - impact) * 2u;
								impact = divide_s32((impact >> 8) * other_weight, own_weight) + crash_place_offsets[mem_read8(screen + 0x320u + SCREEN(screen)->player)];
								if (impact >= 0x1F0u)
									impact = 0x1F0u;
								crash_entry += (impact & ~0xFFFFu) | (impact & 0x1F0u);
								OBJECT(current_object)->player_car.speed_change = (int32_t)(((int32_t)(speed_difference * mem_read16(crash_entry + 0xEu)) >> 8) * other_weight) >> 8;
								side_gap = side_gap - other_x + ((other_slide - own_slide) << 1);
								hit_side = (0u - side_gap) >> 31;
								side_gap = divide_s32(((int32_t)(side_gap * (int16_t)mem_read16(crash_entry + 8u)) >> 7) * other_weight, own_weight);
								overflow = v60_movt_overflow(side_gap, 32, 16);
								OBJECT(current_object)->player_car.sideways_push = side_gap;
								OBJECT(current_object)->player_car.bounce_speed = mem_read16(crash_entry + 0xAu);
							}
							OBJECT(current_object)->state = 5u;
							OBJECT(current_object)->player_car.step = 0u;
							OBJECT(current_object)->player_car.state_high = 0u;
							OBJECT(current_object)->flags |= 8u;
							CAR(player_car)->crash_sound = mem_read8(crash_entry + 0xDu);
							CAR(player_car)->crash_sound_delay = 0xAu;
							crash_hit_sound = (uint8_t)mem_read16(crash_entry + 0xCu);
							request_sound(crash_hit_sound);
							crash_animation = mem_read16(crash_entry + hit_side * 2u);
							view_height_offset = player_car_view_height();
							crash_animation = (uint16_t)(crash_animation + view_height_offset);
							OBJECT(current_object)->animation = crash_animation;
							frame_time = SPRITE_ANIMATION(sprite_animations[crash_animation])->frame_time;
							OBJECT(current_object)->frame_time = frame_time;
							OBJECT(current_object)->frame_countdown = frame_time;
							OBJECT(current_object)->frame = 0u;
							CAR(player_car)->crash_text = 0xFFu;
						}
					}
					if (!L_0071FB)
					{
						if (CAR(player_car)->at_goal != 0u)
						{
							OBJECT(current_object)->state = 2u;
							OBJECT(current_object)->player_car.step = 0u;
							OBJECT(current_object)->player_car.state_high = 0u;
							OBJECT(current_object)->flags &= 0xFBu;
							OBJECT(current_object)->player_car.hit_guard = 0xFu;
							OBJECT(current_object)->player_car.sideways_push = 0;
							OBJECT(current_object)->player_car.bounce_speed = 0;
							OBJECT(current_object)->player_car.bounce_height = 0;
							OBJECT(current_object)->player_car.speed_change = 0;
							OBJECT(current_object)->player_car.gas = 0u;
							CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
							player_car_move();
							return;
						}
						wheel_motor[SCREEN(screen)->index] = 0x10u;
						if (CAR(player_car)->crash_sound_delay != 0u)
						{
							crash_sound_delay = CAR(player_car)->crash_sound_delay;
							CAR(player_car)->crash_sound_delay = crash_sound_delay - 1u;
							if (crash_sound_delay == 1u)
							{
								crash_sound = CAR(player_car)->crash_sound;
								if (crash_sound != 0u)
									request_sound((uint8_t)crash_sound);
							}
						}
						if (OBJECT(current_object)->frame_time == 0u)
						{
							OBJECT(current_object)->state = 2u;
							OBJECT(current_object)->player_car.step = 0u;
							OBJECT(current_object)->player_car.state_high = 0u;
							OBJECT(current_object)->flags &= 0xFBu;
							OBJECT(current_object)->player_car.hit_guard = 0xFu;
							OBJECT(current_object)->player_car.sideways_push = 0;
							OBJECT(current_object)->player_car.bounce_speed = 0;
							OBJECT(current_object)->player_car.bounce_height = 0;
							OBJECT(current_object)->player_car.speed_change = 0;
							OBJECT(current_object)->player_car.gas = 0u;
							CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
							player_car_move();
							return;
						}
						OBJECT(current_object)->player_car.gas = 0u;
						OBJECT(current_object)->player_car.brake = 0u;
						OBJECT(current_object)->player_car.wheel = 0x80u;
						OBJECT(current_object)->player_car.sideways_push = (int32_t)((uint32_t)OBJECT(current_object)->player_car.sideways_push * CAR(player_car)->push_decay) / 0x100;
						OBJECT(current_object)->player_car.speed_change = (int32_t)((uint32_t)OBJECT(current_object)->player_car.speed_change * CAR(player_car)->speed_change_decay) / 0x100;
						new_height = OBJECT(current_object)->player_car.bounce_height;
						gravity = CAR(player_car)->gravity;
						new_bounce_speed = OBJECT(current_object)->player_car.bounce_speed - gravity;
						old_height = new_height;
						new_height += new_bounce_speed;
						if ((int32_t)(old_height ^ new_height) < 0)
						{
							new_height = 0u;
							new_bounce_speed = (int32_t)((new_bounce_speed + gravity) * CAR(player_car)->bounce_damping) / -0x100;
							if (new_bounce_speed != 0u)
							{
								landing_sound = 0x31u;
								request_sound(landing_sound);
							}
						}
						OBJECT(current_object)->player_car.bounce_speed = new_bounce_speed;
						OBJECT(current_object)->player_car.bounce_height = new_height;
						CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
						player_car_move();
						return;
					}
				}
				if (L_0071FB)
					break;
				OBJECT(current_object)->state = 3u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->player_car.state_high = 0u;
			case 0x007134u:
				if (!L_0071FB)
				{
					if (CAR(player_car)->at_goal == 0u)
					{
						OBJECT(current_object)->player_car.gas = 0u;
						brake_index = CAR(player_car)->model;
						brake_index = (uint8_t)(brake_index * 2u);
						OBJECT(current_object)->player_car.brake = player_car_time_up_brakes[(uint8_t)(brake_index + CAR(player_car)->transmission)];
						switch (OBJECT(current_object)->player_car.step)
						{
						case 0:
							OBJECT(current_object)->player_car.step++;
							OBJECT(current_object)->player_car.timer = 0xB4u;
							player_car_clear_hit();
						case 1:
							timer_2 = OBJECT(current_object)->player_car.timer;
							OBJECT(current_object)->player_car.timer = timer_2 - 1u;
							if (timer_2 != 1u)
							{
								if (CAR(player_car)->speed != 0u)
								{
									CAR(player_car)->race_time = RACE(SCREEN(screen)->host_record)->race_time;
									player_car_move();
									return;
								}
								player_car_move();
								model = CAR(player_car)->model;
								player_car_steer_frame(model);
								return;
							}
							OBJECT(current_object)->player_car.step++;
							OBJECT(current_object)->player_car.timer = 0x3Cu;
							CAR(player_car)->race_over = 0xFFu;
							if (RACE(SCREEN(screen)->host_record)->racer_count <= 1u)
							{
								object = start_object();
								OBJECT(object)->handler = game_over_banner_show_ADDRESS;
							}
							else
							{
								object = start_object();
								OBJECT(object)->handler = race_result_start_ADDRESS;
							}
							timer_3 = OBJECT(current_object)->player_car.timer;
							OBJECT(current_object)->player_car.timer = timer_3 - 1u;
							if (timer_3 == 1u)
							{
								OBJECT(current_object)->player_car.step++;
								SCREEN(screen)->flags |= 0x10u;
								dj_message = 0x37u;
								if ((int8_t)SCREEN(screen)->current_stage_level <= 3)
									dj_message = 0x36u;
								start_dj_message(dj_message);
							}
							break;
						case 2:
							timer_3 = OBJECT(current_object)->player_car.timer;
							OBJECT(current_object)->player_car.timer = timer_3 - 1u;
							if (timer_3 != 1u)
							{
								player_car_move();
								model = CAR(player_car)->model;
								player_car_steer_frame(model);
								return;
							}
							OBJECT(current_object)->player_car.step++;
							SCREEN(screen)->flags |= 0x10u;
							dj_message = 0x37u;
							if ((int8_t)SCREEN(screen)->current_stage_level <= 3)
								dj_message = 0x36u;
							start_dj_message(dj_message);
							player_car_move();
							model = CAR(player_car)->model;
							player_car_steer_frame(model);
							return;
						case 3:
							break;
						default:
							invalid_jump(player_car_time_up_step_jumps[OBJECT(current_object)->player_car.step]);
						}
						player_car_move();
						model = CAR(player_car)->model;
						player_car_steer_frame(model);
						return;
					}
				}
				OBJECT(current_object)->state = 4u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->player_car.state_high = 0u;
			case 0x007200u:
				switch (OBJECT(current_object)->player_car.step)
				{
				case 0:
					OBJECT(current_object)->player_car.step++;
					OBJECT(current_object)->player_car.finished = 0xFFu;
					stage_count_place = SCREEN(screen)->race + 0x60u;
					mem_write8(stage_count_place, mem_read8(stage_count_place) + 1u);
					OBJECT(current_object)->player_car.stop_position = CAR(player_car)->segment;
					player_car_clear_hit();
					OBJECT(current_object)->flags |= 8u;
					player_car_move();
					return;
				case 1:
					player_time = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
					if (player_time == 0u)
					{
						player_car_move();
						return;
					}
					RACE(SCREEN(screen)->race)->finish_time = player_time;
					CAR(player_car)->race_time = player_time * 0xAu;
					RACE(SCREEN(screen)->race)->finished = 0xFFu;
					OBJECT(current_object)->player_car.step++;
					goal_stop = goal_stops[mem_read8(screen + 0x320u + SCREEN(screen)->player)];
					OBJECT(current_object)->player_car.stop_position += GOAL_STOP(goal_stop)->position;
					goal_stop += 2u;
					OBJECT(current_object)->player_car.stop_x = GOAL_STOP(goal_stop - 0x2u)->x;
					OBJECT(current_object)->player_car.stop_speed = GOAL_STOP(goal_stop - 0x2u)->speed;
					OBJECT(current_object)->player_car.stop_wheel = OBJECT(current_object)->player_car.wheel;
					OBJECT(current_object)->player_car.stop_gas = OBJECT(current_object)->player_car.gas;
				case 2:
					if (CAR(player_car)->segment < OBJECT(current_object)->player_car.stop_position)
					{
						player_car_steer_to_stop();
						player_car_move();
						return;
					}
					OBJECT(current_object)->player_car.step++;
					OBJECT(current_object)->player_car.stop_speed = 0u;
					arrival_animation = player_car_arrival_animations[CAR(player_car)->model];
					animation_start(arrival_animation);
				case 3:
					OBJECT(current_object)->player_car.gas = 0x40u;
					OBJECT(current_object)->player_car.brake = 0u;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					player_car_steer_to_stop();
					tyre_noise = car_skid_noises[CAR(player_car)->model];
					set_tyre_noise(tyre_noise);
					if (CAR(player_car)->speed != 0u)
					{
						segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
						player_car_drive(&segment_record, &road_x, &segment_fraction, &segment_length);
						CAR(player_car)->wheel_effect_1 = 2u;
						CAR(player_car)->wheel_effect_2 = 2u;
						CAR(player_car)->wheel_effect_sides = 3u;
						AS16(CAR(player_car)->brake_lights) = 2u;
						player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
						player_car_move_view();
						player_car_set_horizon();
						player_car_publish();
						player_car_extend_course();
						player_car_project_road();
						player_car_engine_sound();
						ignored = animation_advance(0u);
						return;
					}
					OBJECT(current_object)->player_car.step++;
					OBJECT(current_object)->player_car.timer = 0x5Au;
					OBJECT(current_object)->player_car.gas = 0u;
					OBJECT(current_object)->player_car.brake = 0x80u;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					timer_4 = OBJECT(current_object)->player_car.timer;
					OBJECT(current_object)->player_car.timer = timer_4 - 1u;
					if (timer_4 == 1u)
					{
						OBJECT(current_object)->player_car.step++;
						SCREEN(screen)->flags |= 0x10u;
					}
					segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&segment_record, &road_x, &segment_fraction, &segment_length);
					player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road();
					player_car_engine_sound();
					ignored = animation_advance(0u);
					return;
				case 4:
					OBJECT(current_object)->player_car.gas = 0u;
					OBJECT(current_object)->player_car.brake = 0x80u;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					timer_4 = OBJECT(current_object)->player_car.timer;
					OBJECT(current_object)->player_car.timer = timer_4 - 1u;
					if (timer_4 == 1u)
					{
						OBJECT(current_object)->player_car.step++;
						SCREEN(screen)->flags |= 0x10u;
					}
					segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&segment_record, &road_x, &segment_fraction, &segment_length);
					player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road();
					player_car_engine_sound();
					ignored = animation_advance(0u);
					return;
				case 5:
					OBJECT(current_object)->player_car.gas = 0u;
					OBJECT(current_object)->player_car.brake = 0x80u;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&segment_record, &road_x, &segment_fraction, &segment_length);
					player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road();
					player_car_engine_sound();
					ignored = animation_advance(0u);
					return;
				default:
					invalid_jump(player_car_goal_step_jumps[OBJECT(current_object)->player_car.step]);
				}
				break;
			case 0x007F31u:
				L_007F31 = true;
				break;
			default:
				invalid_jump(jump_target);
			}
		} while (L_0071FB);
	} while (L_007F31);
}

/* One frame of the car on the road: player_car_drive, player_car_place_view, player_car_move_view,
   player_car_set_horizon, player_car_publish, player_car_extend_course, player_car_project_road,
   player_car_engine_sound and player_car_animate; no wheel effects while it crashes (state 5). */
void player_car_move(void)
{
	uint32_t road_x = 0, segment_fraction = 0, segment_length = 0, segment_record = 0;

	segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
	player_car_drive(&segment_record, &road_x, &segment_fraction, &segment_length);
	player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
	if (OBJECT(current_object)->state == 5u)
		CAR(player_car)->wheel_effect_sides = 0u;
	player_car_move_view();
	player_car_set_horizon();
	player_car_publish();
	player_car_extend_course();
	player_car_project_road();
	player_car_engine_sound();
	player_car_animate();
}

/* Drives the car by itself after the goal: gas 40, no brake, the speed (car speed) a 16th of the way to
   stop_speed each frame, and the wheel turned step by step toward stop_x across the road (centred within
   8 of it; stop_wheel keeps it). */
void player_car_steer_to_stop(void)
{
	uint32_t negative = 0, overflow = 0;
	uint8_t stop_wheel = 0;
	uint16_t distance_from_stop = 0, stop_x = 0;
	uint32_t speed = 0, speed_gap = 0;
	uint16_t road_x = 0, x_from_stop = 0;
	uint32_t stop_speed = 0;

	OBJECT(current_object)->player_car.brake = 0u;
	OBJECT(current_object)->player_car.gas = 0x40u;
	speed = CAR(player_car)->speed;
	stop_speed = OBJECT(current_object)->player_car.stop_speed;
	speed_gap = stop_speed - speed;
	if (stop_speed != speed)
		speed += (int32_t)(speed_gap * 1u) / 0x10 + (shift_signed_by(speed_gap, 0xE0u, 32) * 2u + 1u);
	CAR(player_car)->speed = speed;
	stop_wheel = OBJECT(current_object)->player_car.stop_wheel;
	stop_x = (uint16_t)OBJECT(current_object)->player_car.stop_x;
	road_x = CAR(player_car)->road_x;
	x_from_stop = road_x - stop_x;
	overflow = (uint32_t)(int16_t)road_x - (int16_t)stop_x != (uint32_t)(int16_t)(road_x - stop_x);
	negative = x_from_stop >> 15;
	distance_from_stop = x_from_stop;
	if (road_x == stop_x)
	{
		OBJECT(current_object)->player_car.stop_wheel = 0x80u;
		OBJECT(current_object)->player_car.wheel = 0x80u;
		return;
	}
	if (negative ^ overflow)
	{
		if ((int16_t)distance_from_stop >= -8)
		{
			OBJECT(current_object)->player_car.stop_wheel = 0x80u;
			OBJECT(current_object)->player_car.wheel = 0x80u;
			return;
		}
		stop_wheel--;
		if ((int8_t)stop_wheel < 0x10)
			stop_wheel = 0x10u;
		if ((int8_t)stop_wheel > 0x6F)
			stop_wheel = 0x6Fu;
		OBJECT(current_object)->player_car.stop_wheel = stop_wheel;
		OBJECT(current_object)->player_car.wheel = stop_wheel;
		return;
	}
	if ((int16_t)distance_from_stop <= 8)
	{
		OBJECT(current_object)->player_car.stop_wheel = 0x80u;
		OBJECT(current_object)->player_car.wheel = 0x80u;
		return;
	}
	stop_wheel++;
	if ((int8_t)stop_wheel > -0x31)
		stop_wheel = 0xCFu;
	if ((int8_t)stop_wheel < -0x70)
		stop_wheel = 0x90u;
	OBJECT(current_object)->player_car.stop_wheel = stop_wheel;
	OBJECT(current_object)->player_car.wheel = stop_wheel;
}

/* Keeps the race order around the screen's car: screen leader_record-6C the race records (unk_70-73 the
   players) of the leader, the last, the car just ahead and the car just behind (all itself in the demo),
   screen leader_window_segment and unk_352 the leader's race window_segment and unk_62. When its place
   (screen 320 + player, kept in place) changes during the race: sound 37 when the car it passed or that
   passed it is within 160 across, else 3A. While first, blinks one of the screen's lamps (the count,
   period and timer bytes C9-CB of its entry at checkpoint_lamp). */
void player_car_check_place(void)
{
	uint32_t zero = 0, carry = 0;
	uint8_t sound = 0;
	uint16_t distance_across = 0;
	uint32_t ahead_player = 0, behind_place = 0, behind_player = 0, lamp = 0, last_place = 0, last_player = 0, leader_player = 0, leader_race = 0, passed_race = 0, place = 0, race = 0;
	uint8_t old_place = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
	{
		leader_player = SCREEN(screen)->player;
		last_player = leader_player;
		ahead_player = leader_player;
		behind_player = leader_player;
		SCREEN(screen)->leader_player = leader_player;
		SCREEN(screen)->last_player = last_player;
		SCREEN(screen)->ahead_player = ahead_player;
		SCREEN(screen)->behind_player = behind_player;
		leader_race = race_copy_offsets[leader_player] + GAME_VARIABLES;
		SCREEN(screen)->leader_record = leader_race;
		SCREEN(screen)->last_record = race_copy_offsets[last_player] + GAME_VARIABLES;
		SCREEN(screen)->ahead_record = race_copy_offsets[ahead_player] + GAME_VARIABLES;
		SCREEN(screen)->behind_record = race_copy_offsets[behind_player] + GAME_VARIABLES;
		SCREEN(screen)->leader_window_segment = RACE(leader_race)->window_segment;
		SCREEN(screen)->leader_stage = RACE(leader_race)->stage;
		return;
	}
	race = SCREEN(screen)->host_record + 0x1Fu;
	last_place = RACE(race - 0x1Fu)->racer_count;
	last_place = (uint8_t)(last_place - 1u);
	race++;
	last_player = mem_read8(race + last_place);
	leader_player = RACE(race - 0x20u)->race_order;
	place = mem_read8(screen + 0x320u + SCREEN(screen)->player);
	if (place == 0u)
	{
		lamp = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		if (mem_read8(lamp + 0xC9u) == 0u)
		{
			mem_write8(lamp + 0xC9u, 2u);
			mem_write8(lamp + 0xCAu, 0x10u);
			mem_write8(lamp + 0xCBu, 0x10u);
		}
	}
	old_place = OBJECT(current_object)->player_car.place;
	carry = place < old_place;
	zero = (uint8_t)(place - old_place) == 0u;
	if (place != old_place)
	{
		OBJECT(current_object)->player_car.place = place;
		passed_race = SCREEN(screen)->behind_record;
		if (carry | zero)
			passed_race = SCREEN(screen)->ahead_record;
		distance_across = (uint16_t)(RACE(passed_race)->road_x - RACE(SCREEN(screen)->race_copy)->road_x);
		sound = 0x3Au;
		if ((int16_t)distance_across < 0x160 && (int16_t)distance_across > -0x160)
			sound = 0x37u;
		if (OBJECT(current_object)->state != 0u)
			request_sound(sound);
	}
	behind_place = place;
	if (place != 0u)
		place = (uint8_t)(place - 1u);
	if (behind_place != last_place)
		behind_place = (uint8_t)(behind_place + 1u);
	ahead_player = mem_read8(race + place);
	behind_player = mem_read8(race + behind_place);
	SCREEN(screen)->leader_player = leader_player;
	SCREEN(screen)->last_player = last_player;
	SCREEN(screen)->ahead_player = ahead_player;
	SCREEN(screen)->behind_player = behind_player;
	leader_race = race_copy_offsets[leader_player] + GAME_VARIABLES;
	SCREEN(screen)->leader_record = leader_race;
	SCREEN(screen)->last_record = race_copy_offsets[last_player] + GAME_VARIABLES;
	SCREEN(screen)->ahead_record = race_copy_offsets[ahead_player] + GAME_VARIABLES;
	SCREEN(screen)->behind_record = race_copy_offsets[behind_player] + GAME_VARIABLES;
	SCREEN(screen)->leader_window_segment = RACE(leader_race)->window_segment;
	SCREEN(screen)->leader_stage = RACE(leader_race)->stage;
}

/* Hands the car's state on: this frame's view into the screen's ring of eight (screen + 600, slot
   unk_D4, also race view_slot) and the view of the slot race_copy view_slot names back into the car and
   screen shown_view, so that both screens draw the view of the same frame; and the car's world position,
   animation, palette, frame, wheel effects, speed, sideways speed, flags, segments, place across the
   road and bounce into its race record (race published set), from which race_car draws it on both
   screens. Not in ISDN mode (isdn_mode), which reads them back instead. */
void player_car_publish(void)
{
	uint32_t race = 0, race_copy = 0, shown_view = 0, view_ring_slot = 0, view_slot = 0;

	race = SCREEN(screen)->race;
	race_copy = SCREEN(screen)->race_copy;
	if (isdn_mode == 0u)
	{
		view_ring_slot = SCREEN(screen)->view_ring_slot;
		view_ring_slot = (uint8_t)(view_ring_slot + 1u);
		view_ring_slot = view_ring_slot & 7u;
		SCREEN(screen)->view_ring_slot = view_ring_slot;
		RACE(race)->view_slot = view_ring_slot;
		r12 = screen + 0x600u + (uint16_t)(view_ring_slot << 5);
		view_slot = RACE(race_copy)->view_slot;
		shown_view = screen + 0x600u + (uint16_t)(view_slot << 5);
		SCREEN(screen)->shown_view = shown_view;
	}
	else
	{
		r12 = screen + 0x600u;
		shown_view = r12;
		SCREEN(screen)->shown_view = shown_view;
	}
	mem_write32(r12, CAR(player_car)->view_x);
	CAR(player_car)->view_x = mem_read32(shown_view);
	mem_write32(r12 + 4u, CAR(player_car)->view_y);
	CAR(player_car)->view_y = mem_read32(shown_view + 4u);
	mem_write32(r12 + 8u, CAR(player_car)->view_z);
	CAR(player_car)->view_z = mem_read32(shown_view + 8u);
	mem_write16(r12 + 0xCu, CAR(player_car)->view_pitch);
	CAR(player_car)->view_pitch = mem_read16(shown_view + 0xCu);
	mem_write16(r12 + 0xEu, CAR(player_car)->view_yaw);
	CAR(player_car)->view_yaw = mem_read16(shown_view + 0xEu);
	mem_write16(r12 + 0x10u, CAR(player_car)->window_offset);
	mem_write16(r12 + 0x14u, CAR(player_car)->segment_remaining);
	mem_write16(r12 + 0x12u, SCREEN(screen)->horizon_line);
	SCREEN(screen)->horizon_line = mem_read16(shown_view + 0x12u);
	if (isdn_mode == 0u)
	{
		RACE(race)->world_x = OBJECT(current_object)->player_car.world_x;
		RACE(race)->world_y = OBJECT(current_object)->player_car.world_y;
		RACE(race)->world_z = OBJECT(current_object)->player_car.world_z;
		RACE(race)->animation = OBJECT(current_object)->animation;
		RACE(race)->palette = OBJECT(current_object)->palette;
		RACE(race)->frame = OBJECT(current_object)->frame;
		RACE(race)->wheel_effect_sides = CAR(player_car)->wheel_effect_sides;
		RACE(race)->hard_acceleration = CAR(player_car)->hard_acceleration;
		RACE(race)->wheel_effect_1 = CAR(player_car)->wheel_effect_1;
		RACE(race)->wheel_effect_2 = CAR(player_car)->wheel_effect_2;
		RACE(race)->speed = OBJECT(current_object)->player_car.speed;
		RACE(race)->sideways_speed = OBJECT(current_object)->player_car.sideways_speed;
		RACE(race)->car_flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
		RACE(race)->segment_fraction = CAR(player_car)->segment_fraction;
		RACE(race)->segment = CAR(player_car)->segment;
		RACE(race)->road_x = CAR(player_car)->road_x;
		RACE(race)->window_segment = CAR(player_car)->window_segment;
		RACE(race)->stage = SCREEN(screen)->built_stage;
		RACE(race)->bounce_height = (uint16_t)OBJECT(current_object)->player_car.bounce_height;
		RACE(race)->published = 0xFFu;
	}
}

/* Clears what the screen's car hands on to the race record (player_car_publish's), for the ranking
   (game_mode_ranking): race published, the next of the ring of eight views (screen view_ring_slot, race
   view_slot, its address in screen shown_view) with the view's place and angles cleared in it and the
   car (view_x-z, view_pitch, view_yaw), the wheel effects and hard acceleration (race
   wheel_effect_sides-53), segment_fraction, segment, road_x, window_segment (race segment_fraction-46),
   window_offset, segment_remaining, the horizon (screen horizon_line), and the halfword at C8 of
   current_object (player_car_publish's bounce_height) with race bounce_height; from the ranking
   current_object is what run_objects left in it, not the car. */
void clear_published_car(void)
{
	uint32_t race = 0, view_ring_slot = 0;

	race = SCREEN(screen)->race;
	RACE(race)->published = 0u;
	view_ring_slot = SCREEN(screen)->view_ring_slot;
	view_ring_slot = (uint8_t)(view_ring_slot + 1u);
	view_ring_slot = view_ring_slot & 7u;
	SCREEN(screen)->view_ring_slot = view_ring_slot;
	RACE(race)->view_slot = view_ring_slot;
	r12 = screen + 0x600u + (uint16_t)(view_ring_slot << 5);
	SCREEN(screen)->shown_view = r12;
	CAR(player_car)->view_x = 0;
	mem_write32(r12, 0u);
	CAR(player_car)->view_y = 0;
	mem_write32(r12 + 4u, 0u);
	CAR(player_car)->view_z = 0;
	mem_write32(r12 + 8u, 0u);
	CAR(player_car)->view_pitch = 0u;
	mem_write16(r12 + 0xCu, 0u);
	CAR(player_car)->view_yaw = 0u;
	mem_write16(r12 + 0xEu, 0u);
	CAR(player_car)->wheel_effect_sides = 0u;
	RACE(race)->wheel_effect_sides = 0u;
	CAR(player_car)->hard_acceleration = 0u;
	RACE(race)->hard_acceleration = 0u;
	CAR(player_car)->wheel_effect_1 = 0u;
	RACE(race)->wheel_effect_1 = 0u;
	CAR(player_car)->wheel_effect_2 = 0u;
	RACE(race)->wheel_effect_2 = 0u;
	CAR(player_car)->segment_fraction = 0u;
	RACE(race)->segment_fraction = 0u;
	CAR(player_car)->segment = 0u;
	RACE(race)->segment = 0u;
	CAR(player_car)->road_x = 0;
	RACE(race)->road_x = 0;
	CAR(player_car)->window_segment = 0u;
	RACE(race)->window_segment = 0u;
	CAR(player_car)->window_offset = 0u;
	mem_write16(r12 + 0x10u, 0u);
	CAR(player_car)->segment_remaining = 0u;
	mem_write16(r12 + 0x14u, 0u);
	SCREEN(screen)->horizon_line = 0u;
	mem_write16(r12 + 0x12u, 0u);
	OBJECT(current_object)->unk_C8 &= 0xFFFF0000u;
	RACE(race)->bounce_height = 0;
}

/* Works out the horizon's line on the screen (screen horizon_line) from the view's tilt: the nod (car
   view_pitch_nod), the rival's and the fork camera's pulls (view_pitch_rival, view_pitch_lift), the car
   model's base pitch (screen view_pitch_base), clamped to C0-200, and the intro's (view_pitch_intro). */
void player_car_set_horizon(void)
{
	uint32_t draw_distance = 0, eye_distance = 0, tilt = 0;

	tilt = (uint16_t)(CAR(player_car)->view_pitch_nod + CAR(player_car)->view_pitch_rival + CAR(player_car)->view_pitch_lift + SCREEN(screen)->view_pitch_base);
	if ((int16_t)tilt > 0x200)
		tilt = 0x200u;
	if ((int16_t)tilt < 0xC0)
		tilt = 0xC0u;
	tilt = (uint16_t)(tilt + CAR(player_car)->view_pitch_intro) & 0xFFFu;
	eye_distance = SCREEN(screen)->eye_distance;
	draw_distance = SCREEN(screen)->draw_distance;
	SCREEN(screen)->horizon_line = divide_s32(divide_s32(sine_table[tilt] * draw_distance, (uint32_t)cosine_table[tilt]) * eye_distance, draw_distance + eye_distance) - SCREEN(screen)->sprite_origin_y + 0x100u;
}

/* Puts the car on its starting segment at the start: no way into the segment, road_x from the grid's
   place, its world position (player_car_world_position) and the view behind it (player_car_place_view). */
void player_car_place_on_road(void)
{
	uint32_t road_x = 0, segment_fraction = 0, segment_length = 0, segment_record = 0;

	CAR(player_car)->segment_fraction = 0u;
	CAR(player_car)->road_x = (uint16_t)OBJECT(current_object)->player_car.world_x;
	segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
	road_x = (uint16_t)CAR(player_car)->road_x;
	segment_length = 0x80u;
	segment_fraction = CAR(player_car)->segment_fraction;
	r12 = ((segment_record + 0x20u) & ~0xFFFFu) | ((segment_record + 0x20u) & 0xAFFFu);
	player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
	player_car_place_view(segment_record, road_x, segment_fraction, segment_length);
}

/* Ends what a hit was doing to the car: no sideways_push, speed_change, bounce, no hit pending (flags
   bit 2), and the car no longer skidding (car skidding) or slipping (unk_65). */
void player_car_clear_hit(void)
{
	OBJECT(current_object)->player_car.sideways_push = 0;
	OBJECT(current_object)->player_car.speed_change = 0;
	OBJECT(current_object)->player_car.bounce_height = 0;
	OBJECT(current_object)->player_car.bounce_speed = 0;
	OBJECT(current_object)->player_car.sideways_push = 0;
	OBJECT(current_object)->flags &= 0xFBu;
	CAR(player_car)->skidding = 0u;
	CAR(player_car)->wheel_slip = 0u;
}

/* Picks the car's animation: while a hit is pending, the one it has goes on; skidding (car skidding), a
   skid by how far it slides (slide) toward the side the wheel is turned
   (player_car_skid_animations_wheel_high, player_car_skid_animations_wheel_low by model); spinning its
   wheels while it speeds up (car wheel_slip, acceleration above 0), the model's wheelspin
   (player_car_wheelspin_animations); else its body turned by the steering (player_car_steer_frame). Each
   one in the row of the view's height (player_car_view_height); a new one starts at frame 0. */
void player_car_animate(void)
{
	uint8_t frame_time = 0, slide = 0;
	uint32_t ignored = 0, model_then_animation = 0, view_height = 0;

	model_then_animation = CAR(player_car)->model;
	if ((OBJECT(current_object)->flags & 4u) != 0u)
	{
		ignored = animation_advance(0u);
		return;
	}
	if (CAR(player_car)->skidding != 0u)
	{
		slide = (uint8_t)(uint16_t)OBJECT(current_object)->player_car.slide;
		model_then_animation = (uint16_t)(model_then_animation << 4);
		if ((int8_t)OBJECT(current_object)->player_car.wheel >= 0)
		{
			if ((int8_t)slide > 0)
				slide = (uint8_t)(0u - slide);
			model_then_animation = player_car_skid_animations_wheel_low[(model_then_animation & ~0xFFu) | (uint8_t)(model_then_animation + (0u - slide))];
		}
		else
		{
			if ((int8_t)slide < 0)
				slide = (uint8_t)(0u - slide);
			model_then_animation = player_car_skid_animations_wheel_high[(model_then_animation & ~0xFFu) | (uint8_t)(model_then_animation + slide)];
		}
		view_height = player_car_view_height();
		model_then_animation = (uint16_t)(model_then_animation + view_height);
		if (OBJECT(current_object)->animation != model_then_animation)
		{
			OBJECT(current_object)->animation = model_then_animation;
			frame_time = SPRITE_ANIMATION(sprite_animations[model_then_animation])->frame_time;
			OBJECT(current_object)->frame_time = frame_time;
			OBJECT(current_object)->frame_countdown = frame_time;
			OBJECT(current_object)->frame = 0u;
		}
		ignored = animation_advance(0u);
		return;
	}
	if (OBJECT(current_object)->player_car.acceleration <= 0 || CAR(player_car)->wheel_slip == 0u)
	{
		player_car_steer_frame(model_then_animation);
		return;
	}
	model_then_animation = player_car_wheelspin_animations[model_then_animation];
	view_height = player_car_view_height();
	model_then_animation = (uint16_t)(model_then_animation + view_height);
	if (OBJECT(current_object)->animation != model_then_animation)
	{
		OBJECT(current_object)->animation = model_then_animation;
		frame_time = SPRITE_ANIMATION(sprite_animations[model_then_animation])->frame_time;
		OBJECT(current_object)->frame_time = frame_time;
		OBJECT(current_object)->frame_countdown = frame_time;
		OBJECT(current_object)->frame = 0u;
	}
	ignored = animation_advance(0u);
}

/* The car's body as it steers: the model's animation (player_car_steer_animations) in the row of the
   view's height, its frame (steer_frame) one step a frame toward the one of the steering (the bytes
   around 0092BC: 0-8, 4 straight on). */
void player_car_steer_frame(uint32_t model)
{
	uint8_t steer_frame = 0, target_frame = 0;
	uint16_t animation = 0;
	uint32_t view_height = 0;

	animation = player_car_steer_animations[model];
	view_height = player_car_view_height();
	OBJECT(current_object)->animation = animation + view_height;
	target_frame = mem_read8(steer_frames_by_steering_ADDRESS + OBJECT(current_object)->player_car.steering);
	steer_frame = OBJECT(current_object)->player_car.steer_frame;
	if (target_frame == steer_frame)
	{
		OBJECT(current_object)->frame = steer_frame;
		OBJECT(current_object)->player_car.steer_frame = steer_frame;
		return;
	}
	if (target_frame < steer_frame)
		steer_frame--;
	else
		steer_frame++;
	OBJECT(current_object)->frame = steer_frame;
	OBJECT(current_object)->player_car.steer_frame = steer_frame;
}

/* The row of the car's animations for how steeply the view looks down on it: the view's pitch (car
   view_pitch) less B0 and less the road's pitch under the car (race_copy segment_pitch), in steps of 30,
   0 to 5 (view_height). */
uint32_t player_car_view_height(void)
{
	uint16_t highest = 0, tilt = 0;
	uint32_t view_height = 0;
	uint16_t pitch_over_road = 0;

	view_height = 0u;
	highest = 5u;
	pitch_over_road = ((CAR(player_car)->view_pitch - 0xB0u) << 4) - ((uint32_t)RACE(SCREEN(screen)->race_copy)->segment_pitch << 4);
	tilt = pitch_over_road;
	if ((int16_t)pitch_over_road >= 0)
	{
		view_height = tilt / 0x300u;
		if (highest <= view_height)
			view_height = highest;
	}
	OBJECT(current_object)->player_car.view_height = view_height;
	return view_height;
}

/* Moves the view with what the car does: when a rival comes close (player_car_check_rivals, rival_step)
   it starts the RIVAL SEKKIN warning (rival_warning) and pulls the view back and up (car
   view_shift_rival to -256, unk_112 +8 a step) until the rival has gone, then lets it back; a speed gain
   of more than A0 in a frame without wheelspin sets car hard_acceleration (hard acceleration); the view
   nods (nod, car view_shift_nod and unk_102) one way with that hard acceleration and the other with hard
   braking (more than 120 a frame with the brake down), easing back; and it lags the car's rises and
   drops (car view_rise). */
void player_car_move_view(void)
{
	bool L_007ACB = false, L_007AD1 = false, L_007AFA = false;
	uint16_t climb = 0, last_negated_speed = 0, nod = 0, nod_step = 0, rise_step = 0, side_step = 0, side_target = 0, view_rise = 0, view_side = 0;
	uint32_t object = 0, rise_over_margin = 0, view_shift_rival = 0;
	uint16_t speed_drop = 0, nod_target = 0, eased_step = 0, negative_side_target = 0, positive_side_target = 0, drop_target = 0, climb_target = 0;
	uint32_t jump_target = 0, rise = 0, rise_with_margin = 0;

	do
	{
		do
		{
			jump_target = L_007AD1 ? 0x792Bu : L_007AFA ? 0x792Bu : player_car_rival_step_jumps[OBJECT(current_object)->player_car.rival_step];
			switch (jump_target)
			{
			case 0x007927u:
				OBJECT(current_object)->player_car.rival_step++;
			case 0x00792Bu:
				if (L_007AD1 || L_007AFA || OBJECT(current_object)->player_car.rival_near != 0u)
				{
					do
					{
						if (L_007ACB || L_007AD1 || !L_007AFA)
						{
							if (L_007ACB || !L_007AD1)
							{
								if (!L_007ACB)
								{
									object = start_object();
									OBJECT(object)->handler = rival_warning_update_ADDRESS;
								}
								L_007ACB = false;
								OBJECT(current_object)->player_car.rival_step = 2u;
							}
							L_007AD1 = false;
							player_car_check_rivals();
						}
						if (L_007AFA || OBJECT(current_object)->player_car.rival_near == 0u)
						{
							if (!L_007AFA)
								OBJECT(current_object)->player_car.rival_step = 3u;
							L_007AFA = false;
							player_car_check_rivals();
							L_007ACB = OBJECT(current_object)->player_car.rival_near != 0u;
							if (!L_007ACB)
							{
								view_shift_rival = CAR(player_car)->view_shift_rival;
								if (view_shift_rival == 0u)
									OBJECT(current_object)->player_car.rival_step = 1u;
								else
								{
									view_shift_rival = (uint16_t)(view_shift_rival + 0x10u);
									CAR(player_car)->view_pitch_rival -= 8u;
									CAR(player_car)->view_shift_rival = view_shift_rival;
								}
							}
						}
						else
						{
							view_shift_rival = CAR(player_car)->view_shift_rival;
							if (view_shift_rival != 0xFF00u)
							{
								view_shift_rival = (uint16_t)(view_shift_rival - 0x10u);
								CAR(player_car)->view_pitch_rival += 8u;
								CAR(player_car)->view_shift_rival = view_shift_rival;
							}
						}
					} while (L_007ACB);
				}
				break;
			case 0x007AD1u:
				L_007AD1 = true;
				break;
			case 0x007AFAu:
				L_007AFA = true;
				break;
			default:
				invalid_jump(jump_target);
			}
		} while (L_007AD1);
	} while (L_007AFA);
	CAR(player_car)->hard_acceleration = 0u;
	nod_step = (uint16_t)(0u - OBJECT(current_object)->player_car.speed);
	last_negated_speed = OBJECT(current_object)->player_car.last_negated_speed;
	OBJECT(current_object)->player_car.last_negated_speed = nod_step;
	nod_step -= last_negated_speed;
	nod = (uint16_t)OBJECT(current_object)->player_car.nod;
	if ((OBJECT(current_object)->flags & 4u) != 0u)
		nod_step = 0u;
	else if ((int16_t)nod_step < 0)
	{
		if (CAR(player_car)->wheel_slip != 0u || (int16_t)nod_step >= -0xA0)
			nod_step = 0u;
		else
		{
			nod_step = (uint16_t)(nod_step * 1u);
			CAR(player_car)->hard_acceleration = 0xFFu;
		}
	}
	else if (OBJECT(current_object)->player_car.brake_force == 0u)
		nod_step = 0u;
	else
	{
		speed_drop = nod_step;
		nod_step = (uint16_t)(speed_drop - 0x120u);
		if ((int16_t)speed_drop <= 0x120)
			nod_step = 0u;
		else
			nod_step = (uint16_t)(nod_step * 1u);
	}
	nod_target = (int16_t)(nod_step * 1u) / 5;
	nod_step = (uint16_t)(nod_target - nod);
	if (nod_target != nod)
	{
		eased_step = (int16_t)nod_step >> 3;
		nod_step = eased_step;
		if (eased_step == 0u)
			nod_step++;
	}
	if ((int16_t)nod_step < -0x20)
		nod_step = 0xFFE0u;
	if ((int16_t)nod_step > 6)
		nod_step = 6u;
	nod += nod_step;
	if ((int16_t)nod > 0x18)
		nod = 0x18u;
	if ((int16_t)nod < -0x100)
		nod = 0xFF00u;
	OBJECT(current_object)->player_car.nod = nod;
	CAR(player_car)->view_shift_nod = nod;
	if ((int16_t)nod >= 0)
		nod = (uint16_t)((int16_t)(nod * 1u) / 1);
	else
		nod = (uint16_t)((int16_t)(nod * 1u) / 1);
	CAR(player_car)->view_pitch_nod = nod;
	side_target = (uint16_t)((0u - OBJECT(current_object)->player_car.sideways_speed) << 1);
	view_side = CAR(player_car)->view_side;
	if (CAR(player_car)->skidding == 0u)
	{
		if (view_side != 0u)
		{
			if ((int16_t)view_side >= 0)
				view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
			else
				view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if ((int16_t)side_target < 0)
	{
		negative_side_target = side_target;
		side_step = (uint16_t)(negative_side_target - view_side);
		if (negative_side_target != view_side)
		{
			view_side += ((int16_t)side_step >> 2) + 1u;
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if ((int16_t)side_target > 0)
	{
		positive_side_target = side_target;
		side_step = (uint16_t)(positive_side_target - view_side);
		if (positive_side_target != view_side)
		{
			view_side += (int16_t)side_step >> 2;
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if (view_side != 0u)
	{
		if ((int16_t)view_side >= 0)
			view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
		else
			view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
		if ((int16_t)view_side > 0x40)
			view_side = 0x40u;
	}
	view_rise = (uint16_t)(int16_t)CAR(player_car)->view_rise;
	rise = (uint32_t)OBJECT(current_object)->player_car.world_y - OBJECT(current_object)->player_car.previous_world_y;
	rise_over_margin = rise + 0xCu;
	if ((int32_t)rise < -0xC)
	{
		drop_target = rise_over_margin;
		rise_step = (uint16_t)(drop_target - view_rise);
		if (drop_target == (uint16_t)view_rise)
			return;
		view_rise += ((int16_t)rise_step >> 2) + 1u;
	}
	else
	{
		rise_with_margin = rise_over_margin;
		climb = (uint16_t)(rise_with_margin - 0xCu);
		if ((int32_t)rise_with_margin > 0xC)
		{
			climb_target = climb;
			rise_step = (uint16_t)(climb_target - view_rise);
			if (climb_target == (uint16_t)view_rise)
				return;
			view_rise += (int16_t)rise_step >> 2;
		}
		else
		{
			if ((uint16_t)view_rise == 0u)
				return;
			if ((int16_t)view_rise >= 0)
				view_rise = (uint16_t)((int16_t)(view_rise * 3u) / 4);
			else
				view_rise = (uint16_t)((int16_t)(view_rise * 3u) / 4);
		}
	}
	if ((int16_t)view_rise > 8)
		view_rise = 8u;
	if ((int16_t)view_rise < -8)
		view_rise = 0xFFF8u;
	CAR(player_car)->view_rise = view_rise;
}

/* Whether a rival is close (rival_near): at speed 6000 or more (5000 to stay on), the car just ahead
   (screen ahead_record) a little slower and within reach, or the car just behind (unk_6C) a little
   faster and close; once on it stays for 30 frames (rival_hold) after the rival has gone. */
void player_car_check_rivals(void)
{
	uint32_t ahead_race = 0, gap = 0, position = 0, race = 0, speed = 0, speed_gap = 0;

	race = SCREEN(screen)->race_copy;
	ahead_race = SCREEN(screen)->ahead_record;
	r12 = SCREEN(screen)->behind_record;
	position = (RACE(race)->segment_fraction | ((uint32_t)RACE(race)->segment << 16u));
	speed = RACE(race)->speed;
	position = (position & ~0xFFFFu) | (uint16_t)(position << 9);
	if (ahead_race != race)
	{
		gap = (RACE(ahead_race)->segment_fraction | ((uint32_t)RACE(ahead_race)->segment << 16u));
		gap = ((gap & ~0xFFFFu) | (uint16_t)(gap << 9)) - position;
		speed_gap = RACE(ahead_race)->speed - speed;
		if (OBJECT(current_object)->player_car.rival_near == 0u)
		{
			if (speed < 0x6000u)
			{
				if (OBJECT(current_object)->player_car.rival_hold == 0u)
				{
					OBJECT(current_object)->player_car.rival_near = 0u;
					return;
				}
				OBJECT(current_object)->player_car.rival_hold--;
				return;
			}
			if ((int32_t)gap <= 0x18000 && (int32_t)speed_gap <= -1 && (int32_t)speed_gap >= -0x100)
			{
				OBJECT(current_object)->player_car.rival_hold = 0x1Eu;
				if (OBJECT(current_object)->player_car.rival_hold != 0u)
					OBJECT(current_object)->player_car.rival_hold--;
				OBJECT(current_object)->player_car.rival_near = 0xFFu;
				return;
			}
		}
		else
		{
			if (speed < 0x5000u)
			{
				if (OBJECT(current_object)->player_car.rival_hold == 0u)
				{
					OBJECT(current_object)->player_car.rival_near = 0u;
					return;
				}
				OBJECT(current_object)->player_car.rival_hold--;
				return;
			}
			if ((int32_t)gap <= 0x20000 && (int32_t)speed_gap <= -1 && (int32_t)speed_gap >= -0x200)
			{
				if (OBJECT(current_object)->player_car.rival_hold != 0u)
					OBJECT(current_object)->player_car.rival_hold--;
				OBJECT(current_object)->player_car.rival_near = 0xFFu;
				return;
			}
		}
	}
	if (r12 == race)
	{
		if (OBJECT(current_object)->player_car.rival_hold == 0u)
		{
			OBJECT(current_object)->player_car.rival_near = 0u;
			return;
		}
		OBJECT(current_object)->player_car.rival_hold--;
		return;
	}
	gap = (RACE(r12)->segment_fraction | ((uint32_t)RACE(r12)->segment << 16u));
	gap = ((gap & ~0xFFFFu) | (uint16_t)(gap << 9)) - position;
	speed_gap = RACE(r12)->speed - speed;
	if (OBJECT(current_object)->player_car.rival_near == 0u)
	{
		if (speed < 0x6000u || (int32_t)gap < -0x30000 || (int32_t)speed_gap > 0x2000 || (int32_t)speed_gap < 0)
		{
			if (OBJECT(current_object)->player_car.rival_hold == 0u)
			{
				OBJECT(current_object)->player_car.rival_near = 0u;
				return;
			}
			OBJECT(current_object)->player_car.rival_hold--;
			return;
		}
		OBJECT(current_object)->player_car.rival_hold = 0x1Eu;
		if (OBJECT(current_object)->player_car.rival_hold != 0u)
			OBJECT(current_object)->player_car.rival_hold--;
		OBJECT(current_object)->player_car.rival_near = 0xFFu;
		return;
	}
	if (speed < 0x5000u || (int32_t)gap < -0x40000 || (int32_t)speed_gap > 0x3000 || (int32_t)speed_gap < -0x1000)
	{
		if (OBJECT(current_object)->player_car.rival_hold == 0u)
		{
			OBJECT(current_object)->player_car.rival_near = 0u;
			return;
		}
		OBJECT(current_object)->player_car.rival_hold--;
		return;
	}
	if (OBJECT(current_object)->player_car.rival_hold != 0u)
		OBJECT(current_object)->player_car.rival_hold--;
	OBJECT(current_object)->player_car.rival_near = 0xFFu;
}

/* RIVAL SEKKIN !! (a rival approaching) at the centre of the screen: 12 frames on, 6 off, 12 on, with
   the rival_alarm, then it ends. */
void rival_warning_update(void)
{
	uint32_t ignored = 0, object = 0, text = 0;
	uint8_t off_frames = 0, on_frames = 0, blinks = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->rival_warning.blinks = 2u;
		OBJECT(current_object)->rival_warning.frames = 0xCu;
		object = start_object();
		OBJECT(object)->handler = rival_alarm_update_ADDRESS;
		break;
	case 1:
		break;
	case 2:
		off_frames = OBJECT(current_object)->rival_warning.frames;
		OBJECT(current_object)->rival_warning.frames = off_frames - 1u;
		if (off_frames == 1u)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->rival_warning.frames = 0xCu;
		}
		return;
	default:
		invalid_jump(rival_warning_state_jumps[OBJECT(current_object)->state]);
	}
	on_frames = OBJECT(current_object)->rival_warning.frames;
	OBJECT(current_object)->rival_warning.frames = on_frames - 1u;
	if (on_frames != 1u)
	{
		text = rival_warning_text_ADDRESS;
		ignored = draw_text(text);
		return;
	}
	blinks = OBJECT(current_object)->rival_warning.blinks;
	OBJECT(current_object)->rival_warning.blinks = blinks - 1u;
	if (blinks == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->rival_warning.frames = 6u;
	off_frames = OBJECT(current_object)->rival_warning.frames;
	OBJECT(current_object)->rival_warning.frames = off_frames - 1u;
	if (off_frames == 1u)
	{
		OBJECT(current_object)->state--;
		OBJECT(current_object)->rival_warning.frames = 0xCu;
	}
}

/* The sound of the rival warning: sound 2B twice, 15 frames apart. */
void rival_alarm_update(void)
{
	uint8_t sound = 0;
	uint8_t frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		sound = 0x2Bu;
		request_sound(sound);
		OBJECT(current_object)->state++;
		OBJECT(current_object)->rival_alarm.frames = 0xFu;
		return;
	case 1:
		frames = OBJECT(current_object)->rival_alarm.frames;
		OBJECT(current_object)->rival_alarm.frames = frames - 1u;
		if (frames == 1u)
		{
			sound = 0x2Bu;
			request_sound(sound);
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
		}
		return;
	default:
		invalid_jump(rival_alarm_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Loads the crash physics of the car's model (car_crash_physics): bounce_damping, speed_change_decay,
   push_decay, gravity and the address of its crash table. */
void player_car_load_crash_physics(void)
{
	uint32_t crash_physics = 0;

	crash_physics = car_crash_physics[CAR(player_car)->model];
	CAR(player_car)->bounce_damping = CRASH_PHYSICS(crash_physics)->bounce_damping;
	crash_physics += 2u;
	CAR(player_car)->speed_change_decay = CRASH_PHYSICS(crash_physics - 0x2u)->speed_change_decay;
	crash_physics += 2u;
	CAR(player_car)->push_decay = CRASH_PHYSICS(crash_physics - 0x4u)->push_decay;
	crash_physics += 2u;
	CAR(player_car)->gravity = CRASH_PHYSICS(crash_physics - 0x6u)->gravity;
	CAR(player_car)->crash_table = crash_physics + 2u;
}

/* Starts the race's cars: for each player of the race, a race_car with its race record and the tables of
   its model (body, driver_places, passenger_places, sprays, tag), the two race_car_occupant objects and
   two rider_gesture objects that follow it; then ends. */
void race_cars_start(void)
{
	uint8_t rider = 0;
	uint32_t car_number = 0, cars_left = 0, driver_occupant = 0, model = 0, object = 0, occupant = 0, player = 0, race = 0, record = 0, rider_places = 0;

	race = SCREEN(screen)->host_record + 0x1Fu;
	cars_left = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		object = start_object();
		player = mem_read8(race);
		OBJECT(object)->handler = race_car_draw_ADDRESS;
		OBJECT(object)->race_car.player = player;
		record = race_copy_offsets[player] + GAME_VARIABLES;
		OBJECT(object)->race_car.record = record;
		model = RACE(record)->model;
		OBJECT(object)->race_car.model = model;
		OBJECT(object)->race_car.first_body_piece = race_car_first_body_pieces[model];
		OBJECT(object)->race_car.first_body_animation = race_car_first_body_animations[model];
		OBJECT(object)->race_car.driver_animation = race_car_driver_animations[model];
		OBJECT(object)->race_car.driver_palette = race_car_driver_palettes[model];
		OBJECT(object)->race_car.driver_frames = race_car_driver_frame_counts[model];
		OBJECT(object)->race_car.driver_gesture_mask = race_car_driver_gesture_masks[model];
		OBJECT(object)->race_car.passenger_animation = race_car_passenger_animations[model];
		OBJECT(object)->race_car.passenger_palette = race_car_passenger_palettes[model];
		OBJECT(object)->race_car.passenger_frames = race_car_passenger_frame_counts[model];
		OBJECT(object)->race_car.passenger_gesture_mask = race_car_passenger_gesture_masks[model];
		rider_places = race_car_rider_places[model];
		OBJECT(object)->race_car.driver_places = rider_places;
		OBJECT(object)->race_car.passenger_places = rider_places + 4u;
		OBJECT(object)->race_car.spray_2_animations = race_car_spray_2_animations_ADDRESS;
		OBJECT(object)->race_car.spray_2_places = rider_places + 8u;
		OBJECT(object)->race_car.spray_2_effect = record + 0x52u;
		OBJECT(object)->race_car.spray_2_side = 2u;
		OBJECT(object)->race_car.spray_1_animations = race_car_spray_1_animations_ADDRESS;
		OBJECT(object)->race_car.spray_1_places = rider_places + 0xCu;
		OBJECT(object)->race_car.spray_1_effect = record + 0x51u;
		OBJECT(object)->race_car.spray_1_side = 1u;
		car_number = RACE(record)->car_number;
		OBJECT(object)->race_car.tag_animation = race_car_tag_animations[car_number];
		OBJECT(object)->race_car.tag_palette = race_car_tag_palettes[model];
		OBJECT(object)->race_car.blink_counter = race_car_blink_starts[car_number];
		OBJECT(object)->race_car.rival = car_number - RACE(SCREEN(screen)->race_copy)->car_number;
		r12 = (r12 & ~0xFFFFu) | (uint16_t)object;
		driver_occupant = model;
		race++;
		occupant = start_object();
		OBJECT(occupant)->handler = race_car_driver_handlers[driver_occupant];
		OBJECT(occupant)->race_car_occupant.car = r12;
		OBJECT(occupant)->race_car_occupant.occupant = driver_occupant;
		OBJECT(occupant)->race_car_occupant.body_back = 0xFFu;
		occupant = start_object();
		OBJECT(occupant)->handler = race_car_passenger_handlers[driver_occupant];
		OBJECT(occupant)->race_car_occupant.car = r12;
		OBJECT(occupant)->race_car_occupant.occupant = driver_occupant + 8u;
		OBJECT(occupant)->race_car_occupant.body_back = 0u;
		rider = (uint8_t)model;
		object = start_object();
		OBJECT(object)->handler = rider_gesture_update_ADDRESS;
		OBJECT(object)->rider_gesture.car = r12;
		OBJECT(object)->rider_gesture.rider = rider;
		OBJECT(object)->rider_gesture.model = model;
		OBJECT(object)->rider_gesture.player = mem_read8(r12 + 9u);
		object = start_object();
		OBJECT(object)->handler = rider_gesture_update_ADDRESS;
		OBJECT(object)->rider_gesture.car = r12;
		OBJECT(object)->rider_gesture.rider = rider + 8u;
		OBJECT(object)->rider_gesture.model = model;
		OBJECT(object)->rider_gesture.player = mem_read8(r12 + 9u);
		cars_left--;
	} while (cars_left != 0u);
	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->flags_high = 0u;
}

/* Draws a car of the race, the screen's own or a rival, from its race record (record), as the screen's
   view sees it: its world position less the view's through the view's matrix and scaled by its depth
   (off the screen, too near or too far: not drawn); its body (animation, frame, palette from the record,
   plus the screen's brake lights and wheel_turn), its driver and passenger (driver_places,
   passenger_places), the sprays at its wheels (race_car_draw_spray), five frames of animation 44 on its
   hard acceleration (record hard_acceleration, car hard_acceleration); over the screen's own car the
   CRASH (or Ouch!) text after a hit, over a rival its number tag, blinking. A rival behind the window of
   the road (within 256 segments) is shown small at the bottom of the screen, larger as it closes in and
   across as it is (with its tag). A body animation without wheels (race_car_body_wheel_kinds) sets
   special_body for the occupants. Once the race is over (screen flags bit 4) it becomes
   race_car_start_ending. */
void race_car_draw(void)
{
	bool L_00978F = false, L_009874 = false, L_009AAD = false, L_009B31 = false;
	uint8_t hard_acceleration_frame = 0, wheel_effect_sides = 0;
	uint16_t crash_text_y = 0, element = 0, far_segment = 0, segment = 0, sprite_x = 0, sprite_y = 0, window_segment = 0, x_offset = 0, y_offset = 0;
	uint32_t animation = 0, blink_counter = 0, body_animation = 0, body_attributes = 0, car = 0, depth = 0, depth_layer = 0, depth_list = 0, distance_behind = 0, group = 0, ignored = 0, older_group = 0, own_race = 0, palette = 0, piece = 0, race = 0, relative_y = 0, rider_places = 0, scale = 0, screen_x = 0, screen_y = 0, second_rider_frame = 0, sprite_entry = 0, view = 0, view_offset = 0, x_term = 0, y_term = 0;
	uint16_t past_limit = 0, swapped_rider = 0, swapped_spray = 0;

	OBJECT(current_object)->race_car.blink_counter++;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
		return;
	window_segment = CAR(player_car)->window_segment;
	far_segment = (uint16_t)(window_segment + (SCREEN(screen)->draw_distance >> 7));
	race = OBJECT(current_object)->race_car.record;
	segment = RACE(race)->segment;
	if (OBJECT(current_object)->race_car.rival != 0u)
	{
		L_009B31 = segment <= window_segment;
		if (!L_009B31)
			L_00978F = true;
	}
	if (!L_009B31)
	{
		if (!L_00978F && (SCREEN(screen)->flags & 0x10u) != 0u)
			OBJECT(current_object)->handler = race_car_ending_handlers[OBJECT(current_object)->race_car.model];
		else if (far_segment <= segment)
			return;
		car = player_car + 0xDCu;
		r12 = (uint32_t)RACE(race)->world_x - CAR(player_car)->view_x;
		relative_y = (uint32_t)RACE(race)->world_y - CAR(player_car)->view_y;
		r14 = (uint32_t)RACE(race)->world_z - CAR(player_car)->view_z;
		x_term = r12 * CAR(car - 0xDCu)->view_matrix_xx;
		car += 4u;
		y_term = relative_y * CAR(car - 0xE0u)->view_matrix_xy;
		car += 4u;
		screen_x = (int32_t)(x_term + r14 * CAR(car - 0xE4u)->view_matrix_xz + y_term) >> 14;
		car += 4u;
		x_term = r12 * CAR(car - 0xE8u)->view_matrix_yx;
		car += 4u;
		y_term = relative_y * CAR(car - 0xECu)->view_matrix_yy;
		car += 4u;
		screen_y = (int32_t)(x_term + r14 * CAR(car - 0xF0u)->view_matrix_yz + y_term) >> 14;
		depth = (int32_t)(r12 * CAR(car - 0xF0u)->view_matrix_zx + r14 * CAR(car - 0xF0u)->view_matrix_zz + relative_y * CAR(car - 0xF0u)->view_matrix_zy) >> 14;
		if (depth > 0x40u && depth < 0x3000u)
		{
			depth_layer = depth_layers_by_depth[depth];
			scale = perspective_scales[depth];
			screen_x = (int32_t)(screen_x * scale) >> 12;
			screen_y = 0u - ((int32_t)(screen_y * scale) >> 12);
			L_009B31 = (int32_t)screen_x > 0x120 || (int32_t)screen_x < -0x120 || (int32_t)screen_y > 0xF0;
			if (!L_009B31)
			{
				scale = (uint16_t)((int16_t)scale >> 4);
				L_009874 = true;
			}
		}
	}
	if (!L_009874 && OBJECT(current_object)->race_car.rival != 0u)
	{
		OBJECT(current_object)->race_car.special_body = 0u;
		OBJECT(current_object)->race_car.gesture_shown = 0u;
		own_race = SCREEN(screen)->race_copy;
		distance_behind = RACE(own_race)->segment;
		distance_behind = (uint16_t)(distance_behind - RACE(race)->segment);
		past_limit = distance_behind - 0x100u;
		distance_behind = past_limit;
		if ((int16_t)past_limit >= 0)
			return;
		distance_behind = (uint16_t)(0u - distance_behind);
		depth_layer = 0x191u;
		sprite_x = (uint16_t)((int16_t)((RACE(race)->road_x - RACE(own_race)->road_x) * 3u) >> 2);
		if ((int16_t)sprite_x <= 0x58)
		{
			if ((int16_t)sprite_x < -0x58)
				sprite_x = 0xFFA8u;
		}
		else
			sprite_x = 0x58u;
		sprite_y = (uint16_t)((int16_t)(0u - RACE(race)->bounce_height) >> 2);
		scale = distance_behind * 3u;
		scale = (scale & ~0xFFFFu) | ((uint16_t)scale >> 3);
		r12 = RACE(race)->animation;
		palette = RACE(race)->palette;
		r14 = RACE(race)->frame;
		race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
		sprite_y = (uint16_t)((int16_t)(distance_behind * 0xFFECu) >> 8);
		scale = distance_behind;
		r12 = OBJECT(current_object)->race_car.tag_animation;
		palette = OBJECT(current_object)->race_car.tag_palette;
		r14 = 0u;
		race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
	}
	else
	{
		if (!L_009874)
		{
			depth_layer = 0x1C0u;
			scale = 0x100u;
			screen_x = 0u;
			screen_y = 0u;
		}
		OBJECT(current_object)->race_car.wheel_turn += RACE(race)->speed;
		palette = (uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 16u);
		palette = palette & 1u;
		if ((uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 24u) != palette)
		{
			OBJECT(current_object)->race_car.wheel_turn = (OBJECT(current_object)->race_car.wheel_turn & 0xFFFFFFu) | ((palette & 0xFFu) << 24u);
			OBJECT(current_object)->race_car.wheel_turn &= 0xFFFF0000u;
		}
		palette = (uint16_t)(palette + AS16(CAR(player_car)->brake_lights));
		palette = (uint16_t)(palette + RACE(race)->palette);
		r12 = RACE(race)->animation;
		r14 = RACE(race)->frame;
		sprite_x = (uint16_t)screen_x;
		sprite_y = (uint16_t)screen_y;
		OBJECT(current_object)->x = screen_x;
		OBJECT(current_object)->y = screen_y;
		OBJECT(current_object)->width_scale = scale;
		OBJECT(current_object)->depth_layer = depth_layer;
		race_car_put_frame(scale, sprite_x, sprite_y, &body_attributes, &sprite_entry, &palette);
		OBJECT(current_object)->race_car.body_attributes = body_attributes;
		if (OBJECT(current_object)->race_car.special_body == 0u)
		{
			animation = RACE(race)->animation;
			body_animation = (uint16_t)(animation - OBJECT(current_object)->race_car.first_body_animation);
			if (race_car_body_wheel_kinds[body_animation] != 0u)
			{
				CAR(player_car)->crash_body_shown = 0u;
				r27 = current_object + 0x90u;
				r28 = current_object + 0xA0u;
				if ((OBJECT(current_object)->race_car.model != 6u || race_car_body_wheel_kinds[body_animation] == 2u) && (body_attributes & 0x40u) == 0u)
				{
					swapped_rider = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | swapped_rider;
				}
				if (RACE(race)->speed == 0u)
				{
					r14 = 0u;
					second_rider_frame = 0u;
				}
				else
				{
					r14 = mem_read8(r27 + 0xAu);
					second_rider_frame = mem_read8(r28 + 0xAu);
					if (mem_read8(r27 + 0xBu) <= r14)
						r14 = 1u;
					else
						r14 = (uint8_t)(r14 + 1u);
					if (mem_read8(r28 + 0xBu) <= second_rider_frame)
						second_rider_frame = 1u;
					else
						second_rider_frame = (uint8_t)(second_rider_frame + 1u);
				}
				mem_write8(r27 + 0xAu, r14);
				mem_write8(r28 + 0xAu, second_rider_frame);
				piece = SPRITE_ANIMATION(sprite_animations[animation])->frames[RACE(race)->frame] + 2u;
				view = SPRITE_PIECE(piece)->attributes;
				piece += 2u;
				view = view & 0x40u;
				view = view >> 6;
				view = (uint16_t)(view * 0x34u);
				view = (uint16_t)(view + (uint16_t)(SPRITE_PIECE(piece - 0x2u)->sprite - OBJECT(current_object)->race_car.first_body_piece) / 3u);
				OBJECT(current_object)->race_car.view = view;
				view_offset = (uint16_t)(view << 4);
				piece += 2u;
				y_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->y_offset;
				x_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->x_offset;
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->race_car.gesture_shown;
				if ((r12 & mem_read8(r27 + 8u)) == 0u)
				{
					rider_places = mem_read32(r27);
					palette = mem_read16(r27 + 6u);
					r12 = mem_read16(r27 + 4u) + view;
					sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
				}
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->race_car.gesture_shown;
				if ((r12 & mem_read8(r28 + 8u)) == 0u)
				{
					r14 = second_rider_frame;
					rider_places = mem_read32(r28);
					palette = mem_read16(r28 + 6u);
					r12 = mem_read16(r28 + 4u) + view;
					sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
				}
				r27 = current_object + 0xC0u;
				r28 = current_object + 0xD0u;
				if ((OBJECT(current_object)->race_car.body_attributes & 0x40u) == 0u)
				{
					swapped_spray = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | swapped_spray;
				}
				wheel_effect_sides = RACE(race)->wheel_effect_sides;
				sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)r28;
				sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y);
				hard_acceleration_frame = OBJECT(current_object)->race_car.hard_acceleration_frame;
				if (hard_acceleration_frame != 0u || RACE(race)->hard_acceleration != 0u)
				{
					r14 = hard_acceleration_frame;
					hard_acceleration_frame++;
					if (hard_acceleration_frame >= 5u)
						hard_acceleration_frame = 0u;
					OBJECT(current_object)->race_car.hard_acceleration_frame = hard_acceleration_frame;
					r12 = 0x44u;
					palette = 0x58u;
					sprite_x = (uint16_t)screen_x;
					sprite_y = (uint16_t)screen_y;
					race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
					race_car_put_sprite(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
				}
			}
			else
			{
				OBJECT(current_object)->race_car.special_body = 0xFFu;
				if (OBJECT(current_object)->race_car.rival == 0u)
					CAR(player_car)->crash_body_shown = 0xFFu;
			}
		}
		do
		{
			if (L_009AAD || SCREEN(screen)->race_started != 0u || OBJECT(CAR(player_car)->object)->player_car.timer < 0x3Cu)
			{
				if (L_009AAD || OBJECT(current_object)->race_car.rival == 0u)
				{
					L_009AAD = false;
					if (OBJECT(current_object)->race_car.crash_text_frames == 0u)
					{
						if (CAR(player_car)->crash_text == 0u)
						{
							depth_list = screen + 0x5000u + depth_layer * 4u;
							if (mem_read16(depth_list) == 0u)
								mem_write16(depth_list + 2u, sprite_entry);
							group = SCREEN(screen)->sprite_list_last + 0x10u;
							older_group = mem_read16(depth_list);
							mem_write32(sprite_entry, older_group);
							mem_write32(sprite_entry + 0x3000u, (uint16_t)(older_group + 0x300u));
							mem_write16(depth_list, (group >> 4) | 0x8000u);
							SCREEN(screen)->sprite_list_last = sprite_entry;
							return;
						}
						CAR(player_car)->crash_text = 0u;
						OBJECT(current_object)->race_car.crash_text_rise = 0u;
						OBJECT(current_object)->race_car.crash_text_frames = 0x1Eu;
						OBJECT(current_object)->race_car.crash_text_y = screen_y;
						OBJECT(current_object)->race_car.crash_text_animation = 0x2Fu;
						OBJECT(current_object)->race_car.crash_text_palette = 0x4Bu;
						if (CAR(player_car)->crash_sound == 0u)
						{
							OBJECT(current_object)->race_car.crash_text_animation = 0x30u;
							OBJECT(current_object)->race_car.crash_text_palette = 0x4Cu;
						}
					}
					OBJECT(current_object)->race_car.crash_text_frames--;
					r12 = OBJECT(current_object)->race_car.crash_text_animation;
					palette = OBJECT(current_object)->race_car.crash_text_palette;
					r14 = OBJECT(current_object)->race_car.crash_text_rise;
					r14 = r14 >> 1;
					sprite_x = (uint16_t)screen_x;
					crash_text_y = (uint16_t)OBJECT(current_object)->race_car.crash_text_y;
					scale = 0x100u;
					race_car_put_frame(scale, sprite_x, crash_text_y, &ignored, &sprite_entry, &palette);
					OBJECT(current_object)->race_car.crash_text_rise++;
					OBJECT(current_object)->race_car.crash_text_rise &= 7u;
				}
				else
				{
					blink_counter = OBJECT(current_object)->race_car.blink_counter;
					if (race_car_tag_blink[blink_counter & 0x1Fu] != 0u)
					{
						element = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->race_car.tag_palette])[0];
						r12 = 0x1Bu;
						sprite_x = (uint16_t)(screen_x - 3u);
						sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
						sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
						r12 = OBJECT(current_object)->race_car.tag_animation;
						sprite_y -= 0x16u;
						sprite_x += 0x15u;
						sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
					}
				}
			}
			else
			{
				L_009AAD = OBJECT(current_object)->race_car.rival != 0u;
				if (!L_009AAD)
				{
					element = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->race_car.tag_palette])[0];
					r12 = 0x1Bu;
					sprite_x = (uint16_t)(screen_x - 3u);
					sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
					sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
					r12 = OBJECT(current_object)->race_car.tag_animation;
					sprite_y -= 0x16u;
					sprite_x += 0x15u;
					sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
				}
			}
		} while (L_009AAD);
	}
	depth_list = screen + 0x5000u + depth_layer * 4u;
	if (mem_read16(depth_list) == 0u)
		mem_write16(depth_list + 2u, sprite_entry);
	group = SCREEN(screen)->sprite_list_last + 0x10u;
	older_group = mem_read16(depth_list);
	mem_write32(sprite_entry, older_group);
	mem_write32(sprite_entry + 0x3000u, (uint16_t)(older_group + 0x300u));
	mem_write16(depth_list, (group >> 4) | 0x8000u);
	SCREEN(screen)->sprite_list_last = sprite_entry;
}

/* Draws the spray at one side's wheels (dust, smoke, water...: the record's kind, from the race record's
   wheel effects), cycling its frames. */
uint32_t race_car_draw_spray(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint8_t wheel_effect_sides, uint32_t scale, uint32_t sprite_entry, uint16_t screen_x, uint16_t screen_y)
{
	uint32_t effect = 0, frame = 0, kind = 0, palette = 0, places = 0;

	frame = mem_read8(r27 + 0xCu);
	if (frame == 0u && (wheel_effect_sides & mem_read8(r27 + 0xDu)) == 0u)
	{
		mem_write8(r27 + 0xCu, 0u);
		mem_write8(r27 + 0xEu, 0u);
		return sprite_entry;
	}
	kind = mem_read8(r27 + 0xEu);
	effect = mem_read8(mem_read32(r27 + 8u));
	if (effect != 0u && effect != kind)
	{
		kind = effect;
		mem_write8(r27 + 0xEu, effect);
		frame = 0u;
	}
	r14 = frame;
	frame = (uint8_t)(frame + 1u);
	if (race_car_spray_frame_counts[kind] <= frame)
		frame = 0u;
	mem_write8(r27 + 0xCu, frame);
	places = mem_read32(r27 + 4u);
	r12 = mem_read16(mem_read32(r27) + kind * 2u);
	palette = race_car_spray_palettes[kind];
	return race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, places, palette, screen_x, screen_y);
}

/* Draws a part of the car (a wheel, a spray) at its offset from the car's place, scaled with the car. */
uint32_t race_car_draw_part(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint32_t scale, uint32_t sprite_entry, uint32_t places, uint32_t palette, uint16_t screen_x, uint16_t screen_y)
{
	uint32_t carry = 0;
	uint16_t sprite_x = 0, sprite_y = 0;
	uint32_t ignored = 0;

	sprite_x = (uint16_t)x_offset;
	sprite_y = (uint16_t)y_offset;
	places += view_offset;
	if (mem_read32(places) == 0u)
		return sprite_entry;
	sprite_x = (uint16_t)((sprite_x + mem_read16(places)) * scale);
	sprite_y = (uint16_t)((sprite_y + mem_read16(places + 2u)) * scale);
	carry = (sprite_x >> 7) & 1u;
	sprite_x = (uint16_t)(((int16_t)sprite_x >> 8) + carry);
	carry = (sprite_y >> 7) & 1u;
	sprite_x += screen_x;
	sprite_y = (uint16_t)(((int16_t)sprite_y >> 8) + carry + screen_y);
	race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
	return sprite_entry;
}

/* Puts the first piece of frame R14 of animation R12 with palette R13 into the sprite list, scaled by R8
   at R17, R18 (race_car_put_sprite). */
void race_car_put_frame(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_palette)
{
	uint32_t piece_attributes = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[r12])->frames[r14] + 2u;
	*p_palette = sprite_palettes[*p_palette];
	race_car_put_sprite(scale, sprite_x, sprite_y, &piece_attributes, p_sprite_entry, p_palette);
	*p_attributes = (*p_attributes & 0xFFFF0000u) | (piece_attributes & 0xFFFFu);
}

/* Writes one sprite of a frame's piece into the screen's sprite list (and its copy at + 3000), its size
   and offsets scaled by R8, at R17, R18. */
void race_car_put_sprite(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_palette)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t height_and_bits = 0, palette_word = 0, piece_attributes = 0, piece_x = 0, piece_y = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0, element = 0;

	piece_attributes = SPRITE_PIECE(r12)->attributes;
	mem_write16(sprite_entry + 0x3000u, piece_attributes);
	mem_write16(sprite_entry, piece_attributes);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	r14 = sprite_size_steps_ADDRESS + SPRITE_PIECE(r12 - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 = r14 + 2u;
	sprite_source_size = source_size;
	mem_write16(sprite_entry + 0x3000u, sprite_source_size);
	mem_write16(sprite_entry, sprite_source_size);
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	r14 = r14 + 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 = r14 + 2u;
	height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 = r14 + 2u;
	height_and_bits = (((height * scale) >> 8) & 0x3FFu) | height_bits;
	mem_write16(sprite_entry + 0x3000u, height_and_bits);
	mem_write16(sprite_entry, height_and_bits);
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 = r14 + 2u;
	width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
	r14 = r14 + 2u;
	width_and_bits = (((width * scale) >> 8) & 0x3FFu) | width_bits | SCREEN(screen)->sprite_screen_bit;
	mem_write16(sprite_entry + 0x3000u, width_and_bits);
	mem_write16(sprite_entry, width_and_bits);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	piece_y = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset * scale) >> 8) + sprite_y);
	mem_write16(sprite_entry + 0x3000u, piece_y);
	mem_write16(sprite_entry, piece_y);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	piece_x = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset * scale) >> 8) + sprite_x);
	mem_write16(sprite_entry + 0x3000u, piece_x);
	mem_write16(sprite_entry, piece_x);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_rom_offset);
	mem_write16(sprite_entry, sprite_rom_offset);
	sprite_entry += 2u;
	element = SPRITE_PALETTE_WORDS(*p_palette)[0];
	*p_palette += 2u;
	palette_word = element;
	mem_write16(sprite_entry + 0x3000u, palette_word);
	mem_write16(sprite_entry, palette_word);
	sprite_entry += 2u;
	SCREEN(screen)->sprite_groups++;
	r12 = r12 + 2u;
	*p_attributes = (*p_attributes & 0xFFFF0000u) | piece_attributes;
	*p_sprite_entry = sprite_entry;
}

/* Writes the first piece of animation R12 into the sprite list at R17, R18 at its own size (the number
   tags). */
uint32_t race_car_put_flat_sprite(uint32_t sprite_entry, uint16_t palette_word, uint16_t sprite_x, uint16_t sprite_y)
{
	uint16_t attributes = 0, height_and_bits = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[r12])->frames[0] + 2u;
	attributes = SPRITE_PIECE(r12)->attributes;
	mem_write16(sprite_entry + 0x3000u, attributes);
	mem_write16(sprite_entry, attributes);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	r14 = sprite_size_steps_ADDRESS + SPRITE_PIECE(r12 - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 = r14 + 2u;
	sprite_source_size = source_size;
	mem_write16(sprite_entry + 0x3000u, sprite_source_size);
	mem_write16(sprite_entry, sprite_source_size);
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	r14 = r14 + 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 = r14 + 2u;
	height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 = r14 + 2u;
	height_and_bits = height | height_bits;
	mem_write16(sprite_entry + 0x3000u, height_and_bits);
	mem_write16(sprite_entry, height_and_bits);
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 = r14 + 2u;
	width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
	r14 = r14 + 2u;
	width_and_bits = width | width_bits | SCREEN(screen)->sprite_screen_bit;
	mem_write16(sprite_entry + 0x3000u, width_and_bits);
	mem_write16(sprite_entry, width_and_bits);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_y);
	mem_write16(sprite_entry, sprite_y);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_x);
	mem_write16(sprite_entry, sprite_x);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_rom_offset);
	mem_write16(sprite_entry, sprite_rom_offset);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, palette_word);
	mem_write16(sprite_entry, palette_word);
	sprite_entry += 2u;
	SCREEN(screen)->sprite_groups++;
	r12 = r12 + 2u;
	return sprite_entry;
}

/* Loads the car's model and transmission: its handling record (car handling), its torque curves
   (unk_50), its gear ratios (unk_54) and top gear (unk_63), the first gear's ratio (unk_64), and the
   roadside surfaces of the first stage (player_car_next_surfaces). */
void player_car_load_gears(void)
{
	uint32_t gear_list = 0, model = 0, model_transmission = 0;

	model = CAR(player_car)->model;
	r12 = car_handlings[model];
	model_transmission = CAR(player_car)->transmission + model * 2u;
	CAR(player_car)->torque_curves = car_torque_curves[model_transmission];
	gear_list = car_gear_lists[model_transmission];
	CAR(player_car)->top_gear = GEAR_LIST(gear_list)->top_gear;
	gear_list++;
	CAR(player_car)->gear_ratios = gear_list;
	CAR(player_car)->gear_ratio = GEAR_LIST(gear_list - 0x1u)->first_ratio;
	CAR(player_car)->handling = r12;
	player_car_next_surfaces();
}

/* Builds the drive force of each gear by speed at screen + 1300 (256 values a gear), interpolating the
   model's points (car torque_curves). */
void player_car_build_torque_curves(void)
{
	uint32_t curve_entry = 0, curve_force = 0, force_step = 0, gears_left = 0, speed = 0, steps_left = 0, torque_point = 0;
	uint16_t next_speed = 0, next_force = 0;

	curve_entry = screen + 0x1300u;
	torque_point = CAR(player_car)->torque_curves;
	gears_left = CAR(player_car)->top_gear;
	gears_left = (uint8_t)(gears_left + 1u);
	do
	{
		r12 = torque_point + 4u;
		do
		{
			speed = (uint16_t)TORQUE_POINT(torque_point)->speed;
			torque_point += 2u;
			next_speed = TORQUE_POINT(r12)->speed;
			r12 = r12 + 2u;
			next_force = TORQUE_POINT(r12 - 0x2u)->force;
			r12 = r12 + 2u;
			curve_force = (uint32_t)TORQUE_POINT(torque_point - 0x2u)->force << 16;
			steps_left = next_speed - speed;
			force_step = divide_s32(((uint32_t)next_force << 16) - curve_force, steps_left);
			torque_point += 2u;
			do
			{
				mem_write16(curve_entry, curve_force >> 16);
				curve_force += force_step;
				steps_left--;
				curve_entry += 2u;
			} while (steps_left != 0u);
		} while (TORQUE_POINT(r12)->speed >= 0);
		torque_point = r12 + 4u;
		gears_left--;
	} while (gears_left != 0u);
}

/* Takes the controls: gas, brake and wheel from the screen's pedals and wheel (screen gas, unk_102,
   unk_100). In the demo (screen demo_flags bit 8) the car drives itself: no brake, the gas up to F0
   (eased off while skidding), the wheel back toward the middle when off the road or slowed, else against
   its sideways drift. */
void player_car_read_controls(void)
{
	uint8_t gas = 0, wheel = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		OBJECT(current_object)->player_car.gas = SCREEN(screen)->gas;
		OBJECT(current_object)->player_car.brake = SCREEN(screen)->brake;
		OBJECT(current_object)->player_car.wheel = SCREEN(screen)->wheel;
		return;
	}
	OBJECT(current_object)->player_car.brake = 0u;
	wheel = OBJECT(current_object)->player_car.wheel;
	if (CAR(player_car)->slowed_frames != 0u)
	{
		if (CAR(player_car)->road_x < 0)
		{
			if (wheel < 0xF8u)
				OBJECT(current_object)->player_car.wheel = wheel + 8u;
			gas = OBJECT(current_object)->player_car.gas;
			if (CAR(player_car)->skidding == 0u)
			{
				if (gas < 0xF0u)
					OBJECT(current_object)->player_car.gas = gas + 0x10u;
				return;
			}
			if (gas < 4u)
				return;
			OBJECT(current_object)->player_car.gas = gas - 4u;
			return;
		}
		if (wheel >= 8u)
			OBJECT(current_object)->player_car.wheel = wheel - 8u;
		gas = OBJECT(current_object)->player_car.gas;
		if (CAR(player_car)->skidding == 0u)
		{
			if (gas < 0xF0u)
				OBJECT(current_object)->player_car.gas = gas + 0x10u;
			return;
		}
		if (gas < 4u)
			return;
		OBJECT(current_object)->player_car.gas = gas - 4u;
		return;
	}
	if (CAR(player_car)->off_road != 0u && CAR(player_car)->model != 7u)
	{
		if (CAR(player_car)->road_x < 0)
		{
			if (wheel >= 8u)
				OBJECT(current_object)->player_car.wheel = wheel - 8u;
			gas = OBJECT(current_object)->player_car.gas;
			if (CAR(player_car)->skidding == 0u)
			{
				if (gas < 0xF0u)
					OBJECT(current_object)->player_car.gas = gas + 0x10u;
				return;
			}
			if (gas < 4u)
				return;
			OBJECT(current_object)->player_car.gas = gas - 4u;
			return;
		}
		if (wheel < 0xF8u)
			OBJECT(current_object)->player_car.wheel = wheel + 8u;
		gas = OBJECT(current_object)->player_car.gas;
		if (CAR(player_car)->skidding == 0u)
		{
			if (gas < 0xF0u)
				OBJECT(current_object)->player_car.gas = gas + 0x10u;
			return;
		}
		if (gas < 4u)
			return;
		OBJECT(current_object)->player_car.gas = gas - 4u;
		return;
	}
	if ((uint16_t)OBJECT(current_object)->player_car.sideways_speed != 0u)
	{
		if (OBJECT(current_object)->player_car.sideways_speed < 0)
		{
			if (wheel >= 8u)
				OBJECT(current_object)->player_car.wheel = wheel - 8u;
		}
		else if (wheel < 0xF8u)
			OBJECT(current_object)->player_car.wheel = wheel + 8u;
	}
	gas = OBJECT(current_object)->player_car.gas;
	if (CAR(player_car)->skidding == 0u)
	{
		if (gas < 0xF0u)
			OBJECT(current_object)->player_car.gas = gas + 0x10u;
		return;
	}
	if (gas < 4u)
		return;
	OBJECT(current_object)->player_car.gas = gas - 4u;
}

/* Moves the car for the frame: the drag (player_car_road_drag), the slope of its segment (race
   segment_pitch), the brake (no gas while braking; brake lights car brake_lights) and the gas through
   the gear's torque curve go to player_car_engine (speed, revs); the wheel steers through the model's
   table (less at low speed) into steering; the road's turn under it (previous_heading less heading)
   pushes it out (less with catch-up, car catch_up); beyond the model's grip it skids (car skidding,
   slide, tyre noise 3, smoke on both sides); sideways_speed and sideways_push move it across (car
   road_x, within the road's half width + 60); the speed along (less its sideways angle, vector_angle)
   plus a catch-up bonus by the distance to the car ahead (car catch_up_target, unk_6A; not once
   finished) moves it along, passing segments (car window_segment, unk_44, unk_40, unk_A4, unk_34,
   unk_32); then player_car_world_position. In ISDN mode (isdn_mode) the race record the link brings in
   gives all this. */
void player_car_drive(uint32_t *p_segment_record, uint32_t *p_road_x, uint32_t *p_segment_fraction, uint32_t *p_segment_length)
{
	uint32_t segment_record = *p_segment_record;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t tyre_noise = 0;
	uint16_t catch_up_target = 0, half_width = 0, max_road_x = 0, min_road_x = 0, new_sideways_speed = 0, road_half_width = 0, segment_then_offset = 0, start_road_x = 0, window_segment = 0;
	uint32_t brake_force = 0, catch_up = 0, cornering = 0, drag = 0, drift = 0, drive_force = 0, forward_speed = 0, gas = 0, gear = 0, gear_curve = 0, gear_ratio = 0, grip = 0, new_speed = 0, origin_x = 0, origin_z = 0, road_force = 0, road_x = 0, segment_fraction = 0, segment_length = 0, segment_pitch = 0, speed = 0, steering = 0, wheel = 0;
	uint16_t target_sideways_speed = 0, gap_to_ahead = 0, catch_up_change = 0;

	r12 = CAR(player_car)->handling;
	CAR(player_car)->wheel_effect_sides = 0u;
	CAR(player_car)->wheel_effect_1 = 0u;
	CAR(player_car)->wheel_effect_2 = 0u;
	if (CAR(player_car)->speed != 0u && CAR(player_car)->shift_spin != 0u)
	{
		CAR(player_car)->wheel_effect_1 = 0xBu;
		CAR(player_car)->wheel_effect_2 = 0xBu;
		CAR(player_car)->wheel_effect_sides = 3u;
	}
	drag = CAR(player_car)->speed;
	if (drag != 0u)
	{
		drag = (drag * drag) >> 16;
		start_road_x = (uint16_t)CAR(player_car)->road_x;
		road_half_width = mem_read16(segment_record + 0x12u);
		half_width = CAR_HANDLING(r12)->half_width;
		drag = player_car_road_drag(drag, start_road_x, road_half_width, half_width);
	}
	segment_pitch = mem_read16(segment_record + 2u);
	RACE(SCREEN(screen)->race)->segment_pitch = segment_pitch;
	road_force = ((int32_t)sine_table[segment_pitch] >> 1) - (drag + 0x1800u);
	gas = OBJECT(current_object)->player_car.gas;
	AS16(CAR(player_car)->brake_lights) = 0u;
	brake_force = brake_pedal_curve[OBJECT(current_object)->player_car.brake];
	if (brake_force != 0u)
	{
		gas = 0u;
		AS16(CAR(player_car)->brake_lights) = 2u;
	}
	OBJECT(current_object)->player_car.brake_force = brake_force;
	brake_force = brake_force * CAR_HANDLING(r12)->brake_factor;
	gear = CAR(player_car)->gear;
	gear_curve = screen + 0x1300u + (uint16_t)(gear << 9);
	speed = CAR(player_car)->speed;
	drive_force = (int32_t)((uint32_t)gas_pedal_curve[gas] * mem_read16(gear_curve + (speed >> 8) * 2u)) >> 6;
	gear_ratio = CAR(player_car)->gear_ratio;
	player_car_engine(road_force, drive_force, brake_force, gear_ratio);
	if (isdn_mode != 0u)
		CAR(player_car)->speed = RACE(SCREEN(screen)->race_copy)->speed;
	new_speed = CAR(player_car)->speed;
	OBJECT(current_object)->player_car.speed = new_speed;
	wheel = OBJECT(current_object)->player_car.wheel;
	steering = (int32_t)(WHEEL_STEERING(CAR_HANDLING(r12)->steering)[wheel >> 3] * 0x14u) / 0x10;
	if (new_speed <= 0xA000u)
	{
		if (new_speed > 0x2000u)
			steering = (int32_t)(steering * (new_speed + 0x6000u)) / 0x10000;
		else
			steering = (int32_t)(steering * new_speed) / 0x4000;
	}
	OBJECT(current_object)->player_car.steering = steering;
	new_sideways_speed = (uint16_t)((int16_t)((((uint32_t)OBJECT(current_object)->player_car.previous_heading << 4) - ((uint32_t)OBJECT(current_object)->player_car.heading << 4)) * 0x20u) / 0x100);
	if (OBJECT(current_object)->state == 5u)
		new_sideways_speed = (uint16_t)((int16_t)new_sideways_speed >> 4);
	new_sideways_speed = (uint16_t)divide_s32((uint32_t)(int16_t)new_sideways_speed << 8, CAR(player_car)->catch_up + 0x100u);
	CAR(player_car)->skidding = 0u;
	cornering = OBJECT(current_object)->player_car.speed * steering;
	grip = CAR_HANDLING(r12)->grip;
	if ((int32_t)grip <= (int32_t)cornering)
	{
		CAR(player_car)->skidding = 0xFFu;
		cornering = (int32_t)((cornering - grip) * CAR_HANDLING(r12)->slide_factor) / -0x400000;
		OBJECT(current_object)->player_car.slide = cornering;
		new_sideways_speed = (uint16_t)(new_sideways_speed + cornering + steering);
		if (CAR(player_car)->wheel_effect_sides == 0u)
		{
			CAR(player_car)->wheel_effect_1 = 3u;
			CAR(player_car)->wheel_effect_2 = 3u;
			CAR(player_car)->wheel_effect_sides = 3u;
		}
		tyre_noise = 3u;
		set_tyre_noise_on_road(tyre_noise);
		OBJECT(current_object)->player_car.sideways_speed = new_sideways_speed;
		drift = new_sideways_speed;
	}
	else
	{
		grip = 0u - grip;
		if ((int32_t)cornering <= (int32_t)grip)
		{
			CAR(player_car)->skidding = 0xFFu;
			cornering = (int32_t)((cornering - grip) * CAR_HANDLING(r12)->slide_factor) / -0x400000;
			OBJECT(current_object)->player_car.slide = cornering;
			new_sideways_speed = (uint16_t)(new_sideways_speed + cornering + steering);
			if (CAR(player_car)->wheel_effect_sides == 0u)
			{
				CAR(player_car)->wheel_effect_1 = 3u;
				CAR(player_car)->wheel_effect_2 = 3u;
				CAR(player_car)->wheel_effect_sides = 3u;
			}
			tyre_noise = 3u;
			set_tyre_noise_on_road(tyre_noise);
			OBJECT(current_object)->player_car.sideways_speed = new_sideways_speed;
			drift = new_sideways_speed;
		}
		else
		{
			new_sideways_speed += steering;
			if ((SCREEN(screen)->demo_flags & 0x100u) != 0u || RACE(SCREEN(screen)->race_copy)->finished != 0u)
			{
				drift = (uint16_t)OBJECT(current_object)->player_car.sideways_speed;
				target_sideways_speed = new_sideways_speed;
				new_sideways_speed = (uint16_t)(target_sideways_speed - drift);
				if (target_sideways_speed != drift)
					drift = (uint16_t)(drift + ((int16_t)(new_sideways_speed * 1u) / 4 + (shift_signed_by(new_sideways_speed, 0xF0u, 16) * 2u + 1u)));
				OBJECT(current_object)->player_car.sideways_speed = drift;
			}
			else
			{
				drift = new_sideways_speed;
				OBJECT(current_object)->player_car.sideways_speed = drift;
			}
		}
	}
	if (isdn_mode != 0u)
	{
		drift = (uint16_t)RACE(SCREEN(screen)->race_copy)->sideways_speed;
		OBJECT(current_object)->player_car.sideways_speed = drift;
	}
	drift = (uint32_t)(int16_t)drift << 8;
	speed = OBJECT(current_object)->player_car.speed;
	forward_speed = speed;
	origin_x = 0u;
	origin_z = 0u;
	drift = vector_angle(drift, speed, origin_x, origin_z);
	forward_speed = (forward_speed * cosine_table[drift]) >> 14;
	segment_fraction = CAR(player_car)->segment_progress + forward_speed;
	if (OBJECT(current_object)->player_car.finished == 0u)
	{
		catch_up_target = (uint16_t)(RACE(SCREEN(screen)->ahead_record)->segment - RACE(SCREEN(screen)->race_copy)->segment);
		if ((int16_t)catch_up_target > 0x20)
			catch_up_target = 0x20u;
		gap_to_ahead = catch_up_target;
		catch_up_target = (uint16_t)(gap_to_ahead - 1u);
		if ((int16_t)gap_to_ahead < 1)
			catch_up_target = 0u;
		CAR(player_car)->catch_up_target = catch_up_target;
		catch_up = CAR(player_car)->catch_up;
		catch_up_change = catch_up_target - catch_up;
		overflow = (uint32_t)(int16_t)catch_up_target - (int16_t)catch_up != (uint32_t)(int16_t)(catch_up_target - catch_up);
		zero = catch_up_change == 0u;
		negative = catch_up_change >> 15;
		if (catch_up_target != catch_up)
		{
			if (((negative ^ overflow) | zero) == 0u)
				catch_up = (uint16_t)(catch_up + 1u);
			else
				catch_up = (uint16_t)(catch_up - 1u);
		}
		CAR(player_car)->catch_up = catch_up;
		segment_fraction += (catch_up * forward_speed) >> 7;
	}
	if (isdn_mode != 0u)
	{
		segment_fraction = RACE(SCREEN(screen)->race_copy)->segment_fraction;
		segment_then_offset = RACE(SCREEN(screen)->race_copy)->segment;
		road_x = RACE(SCREEN(screen)->race_copy)->road_x;
		CAR(player_car)->segment_fraction = segment_fraction;
		CAR(player_car)->segment_progress = segment_fraction;
		CAR(player_car)->segment_progress = CAR(player_car)->segment_progress << 8;
		CAR(player_car)->segment = segment_then_offset;
		segment_then_offset = (segment_then_offset * 0x20u) & 0xFFFu;
		CAR(player_car)->segment_offset = segment_then_offset;
		segment_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + segment_then_offset);
		r12 = ((segment_record + 0x20u) & ~0xFFFFu) | ((segment_record + 0x20u) & 0xAFFFu);
		segment_length = mem_read16(segment_record);
		segment_length = (uint16_t)(segment_length - mem_read16(r12));
		segment_length = sine_table[segment_length & 0xFFFu] * road_x;
		carry = (segment_length >> 5) & 1u;
		segment_length = (int32_t)(((int32_t)segment_length >> 6) + 0x8000u + carry) >> 8;
		window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
		CAR(player_car)->window_segment = window_segment;
		CAR(player_car)->window_offset = (window_segment * 0x20u) & 0xFFFu;
	}
	else
	{
		road_x = (int16_t)(CAR(player_car)->road_x + OBJECT(current_object)->player_car.sideways_speed + OBJECT(current_object)->player_car.sideways_push);
		CAR(player_car)->road_x = road_x;
		for (;;)
		{
			r12 = ((segment_record + 0x20u) & ~0xFFFFu) | ((segment_record + 0x20u) & 0xAFFFu);
			segment_length = mem_read16(segment_record);
			segment_length = (uint16_t)(segment_length - mem_read16(r12));
			segment_length = sine_table[segment_length & 0xFFFu] * road_x;
			carry = (segment_length >> 5) & 1u;
			segment_length = ((int32_t)segment_length >> 6) + 0x8000u + carry;
			if ((int32_t)segment_fraction < (int32_t)segment_length)
				break;
			segment_fraction -= segment_length;
			segment_record = r12;
			CAR(player_car)->window_segment++;
			CAR(player_car)->segment++;
			CAR(player_car)->window_offset += 0x20u;
			CAR(player_car)->window_offset &= 0xAFFFu;
			CAR(player_car)->segment_offset += 0x20u;
			CAR(player_car)->segment_offset &= 0xAFFFu;
		}
		CAR(player_car)->segment_progress = segment_fraction;
		segment_fraction = (int32_t)segment_fraction >> 8;
		segment_length = (int32_t)segment_length >> 8;
		CAR(player_car)->segment_fraction = segment_fraction;
	}
	if (isdn_mode != 0u)
	{
		CAR(player_car)->road_x = road_x;
		player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
		*p_segment_record = segment_record;
		*p_road_x = road_x;
		*p_segment_fraction = segment_fraction;
		*p_segment_length = segment_length;
		return;
	}
	max_road_x = mem_read16(segment_record + 0x12u);
	min_road_x = (uint16_t)(0u - max_road_x);
	max_road_x += 0x60u;
	min_road_x -= 0x60u;
	if ((int16_t)road_x < (int16_t)max_road_x)
	{
		if ((int16_t)min_road_x < (int16_t)road_x)
		{
			player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
			*p_segment_record = segment_record;
			*p_road_x = road_x;
			*p_segment_fraction = segment_fraction;
			*p_segment_length = segment_length;
			return;
		}
		road_x = (road_x & ~0xFFFFu) | min_road_x;
		CAR(player_car)->road_x = road_x;
		player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
		*p_segment_record = segment_record;
		*p_road_x = road_x;
		*p_segment_fraction = segment_fraction;
		*p_segment_length = segment_length;
		return;
	}
	road_x = (road_x & ~0xFFFFu) | max_road_x;
	CAR(player_car)->road_x = road_x;
	player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
	*p_segment_record = segment_record;
	*p_road_x = road_x;
	*p_segment_fraction = segment_fraction;
	*p_segment_length = segment_length;
}

/* Places the car in the world from its segment: world_x and world_z from the segment's position and the
   car's place across and along it turned by the road's heading and pitch (rotate_offset), world_y the
   road's height there plus bounce_height (the old one to previous_world_y); the rest of the segment (car
   segment_remaining) and the road's heading under the car (heading, the old one to previous_heading). */
void player_car_world_position(uint32_t segment_record, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length)
{
	uint16_t previous_heading = 0;
	uint32_t heading_end = 0, heading_start = 0, height = 0, offset_x = 0, offset_y = 0, offset_z = 0, previous_world_y = 0, quadrant_heading = 0, segment_heading = 0, segment_pitch = 0, segment_remaining = 0;
	uint16_t heading = 0;
	uint32_t world_y = 0;

	if (isdn_mode == 0u)
	{
		offset_y = 0u;
		offset_x = road_x << 7;
		offset_z = segment_fraction << 7;
		segment_pitch = mem_read16(segment_record + 2u);
		segment_heading = mem_read16(segment_record);
		rotate_offset(offset_y, segment_heading, segment_pitch, &offset_x, &offset_z);
		OBJECT(current_object)->player_car.world_x = (int32_t)(offset_x * 2u + (mem_read32(segment_record + 4u) << 8)) >> 8;
		OBJECT(current_object)->player_car.world_z = (int32_t)(offset_z * 2u + (mem_read32(segment_record + 8u) << 8)) >> 8;
		segment_remaining = segment_length - segment_fraction;
		height = divide_s32(mem_read32(r12 + 0xCu) * segment_fraction + mem_read32(segment_record + 0xCu) * segment_remaining, segment_length) + OBJECT(current_object)->player_car.bounce_height;
	}
	else
	{
		OBJECT(current_object)->player_car.world_x = RACE(SCREEN(screen)->race_copy)->world_x;
		height = (uint32_t)RACE(SCREEN(screen)->race_copy)->world_y + RACE(SCREEN(screen)->race_copy)->bounce_height;
		OBJECT(current_object)->player_car.world_z = RACE(SCREEN(screen)->race_copy)->world_z;
		segment_remaining = segment_length - segment_fraction;
	}
	world_y = height;
	previous_world_y = OBJECT(current_object)->player_car.world_y;
	OBJECT(current_object)->player_car.world_y = world_y;
	OBJECT(current_object)->player_car.previous_world_y = previous_world_y;
	CAR(player_car)->segment_remaining = segment_remaining;
	heading_start = (uint16_t)((uint32_t)mem_read16(segment_record) << 4);
	heading_end = (uint16_t)((uint32_t)mem_read16(r12) << 4);
	quadrant_heading = heading_start;
	switch (quadrant_heading >> 14)
	{
	case 0:
	case 3:
		heading_start = (int16_t)heading_start;
		heading_end = (int16_t)heading_end;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(player_car_heading_quadrant_jumps[quadrant_heading >> 14]);
	}
	heading = ((int32_t)divide_s32(heading_start * segment_remaining + heading_end * segment_fraction, segment_length) >> 4) & 0xFFFu;
	previous_heading = OBJECT(current_object)->player_car.heading;
	OBJECT(current_object)->player_car.heading = heading;
	OBJECT(current_object)->player_car.previous_heading = previous_heading;
}

/* Places the view behind the car: a point on the road some way back of the car (the nod, the intro, the
   rival and road-object pulls: car view_shift_nod, unk_10C, unk_110, unk_108) and across (unk_106) and
   above (unk_104) it (view_target_x/y/z), looking toward the road twelve segments on (vector_angles: car
   view_yaw the yaw, unk_30 the road's pitch; car view_pitch the pitch with the view's tilt), the eye
   screen eye_distance back from the target along those angles (angles_to_offset) in car view_x-28. */
void player_car_place_view(uint32_t segment_record, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length)
{
	uint16_t pitch_here = 0, tilt = 0;
	uint32_t eye_distance = 0, eye_x = 0, eye_y = 0, eye_z = 0, far_z = 0, near_x = 0, near_y = 0, near_z = 0, offset_x = 0, offset_y = 0, offset_z = 0, pitch = 0, quadrant_yaw = 0, segment_heading = 0, segment_pitch = 0, segment_remaining = 0, shift_offset = 0, view_shift = 0, yaw = 0, yaw_here = 0;

	view_shift = (uint32_t)(int16_t)CAR(player_car)->view_shift_rival + (int16_t)CAR(player_car)->view_shift_lift;
	if ((int32_t)view_shift >= 0x30)
		view_shift = 0x30u;
	if ((int32_t)view_shift <= -0xC0)
		view_shift = 0xFFFFFF40u;
	view_shift = view_shift + (int16_t)CAR(player_car)->view_shift_intro + (int16_t)CAR(player_car)->view_shift_nod;
	shift_offset = ((int32_t)view_shift >> 2) & ~0x1Fu;
	segment_record += shift_offset;
	segment_record = (segment_record & ~0xFFFFu) | (uint16_t)(segment_record | 0x2000u);
	segment_record = (segment_record & ~0xFFFFu) | (segment_record & 0xAFFFu);
	r12 = r12 + shift_offset;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x2000u);
	segment_fraction += view_shift & 0x7Fu;
	offset_y = 0u;
	offset_x = (road_x + (int16_t)CAR(player_car)->view_side) << 7;
	offset_z = segment_fraction << 7;
	segment_pitch = mem_read16(segment_record + 2u);
	segment_heading = mem_read16(segment_record);
	rotate_offset(offset_y, segment_heading, segment_pitch, &offset_x, &offset_z);
	OBJECT(current_object)->player_car.view_target_x = ((int32_t)(offset_x * 2u) >> 8) + mem_read32(segment_record + 4u);
	OBJECT(current_object)->player_car.view_target_z = ((int32_t)(offset_z * 2u) >> 8) + mem_read32(segment_record + 8u);
	segment_remaining = segment_length - segment_fraction;
	OBJECT(current_object)->player_car.view_target_y = divide_s32(mem_read32(((r12 & ~0xFFFFu) | (r12 & 0xAFFFu)) + 0xCu) * segment_fraction + mem_read32(segment_record + 0xCu) * segment_remaining, segment_length) + (int16_t)CAR(player_car)->view_rise;
	r12 = ((segment_record + 0x180u) & ~0xFFFFu) | ((segment_record + 0x180u) & 0xAFFFu);
	pitch = mem_read32(r12 + 4u);
	yaw = mem_read32(r12 + 0xCu);
	far_z = mem_read32(r12 + 8u);
	near_x = mem_read32(segment_record + 4u);
	near_y = mem_read32(segment_record + 0xCu);
	near_z = mem_read32(segment_record + 8u);
	vector_angles(far_z, near_x, near_y, near_z, &pitch, &yaw);
	segment_record = ((segment_record + 0x20u) & ~0xFFFFu) | ((segment_record + 0x20u) & 0xAFFFu);
	r12 = ((segment_record + 0x180u) & ~0xFFFFu) | ((segment_record + 0x180u) & 0xAFFFu);
	pitch_here = (uint16_t)pitch;
	yaw_here = yaw;
	pitch = mem_read32(r12 + 4u);
	yaw = mem_read32(r12 + 0xCu);
	far_z = mem_read32(r12 + 8u);
	near_x = mem_read32(segment_record + 4u);
	near_y = mem_read32(segment_record + 0xCu);
	near_z = mem_read32(segment_record + 8u);
	vector_angles(far_z, near_x, near_y, near_z, &pitch, &yaw);
	pitch = (uint16_t)(pitch << 4);
	yaw = (uint16_t)(yaw << 4);
	pitch_here = (uint16_t)((uint32_t)pitch_here << 4);
	yaw_here = (uint16_t)(yaw_here << 4);
	quadrant_yaw = yaw;
	switch (quadrant_yaw >> 14)
	{
	case 0:
	case 3:
		yaw = (int16_t)yaw;
		yaw_here = (int16_t)yaw_here;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(player_car_view_yaw_quadrant_jumps[quadrant_yaw >> 14]);
	}
	yaw = ((int32_t)divide_s32(yaw * segment_fraction + yaw_here * segment_remaining, segment_length) >> 4) & 0xFFFu;
	pitch = ((int32_t)divide_s32((int16_t)pitch * segment_fraction + (int16_t)pitch_here * segment_remaining, segment_length) >> 4) & 0xFFFu;
	CAR(player_car)->view_yaw = yaw;
	CAR(player_car)->road_pitch = pitch;
	tilt = (uint16_t)(CAR(player_car)->view_pitch_nod + CAR(player_car)->view_pitch_rival + CAR(player_car)->view_pitch_lift + SCREEN(screen)->view_pitch_base);
	if ((int16_t)tilt > 0x200)
		tilt = 0x200u;
	if ((int16_t)tilt < 0xC0)
		tilt = 0xC0u;
	pitch = (uint16_t)(pitch + tilt + CAR(player_car)->view_pitch_intro) & 0xFFFu;
	CAR(player_car)->view_pitch = pitch;
	eye_distance = SCREEN(screen)->eye_distance;
	pitch = (uint16_t)(pitch + 0x800u);
	pitch = pitch & 0xFFFu;
	angles_to_offset(pitch, yaw, eye_distance, &eye_x, &eye_y, &eye_z);
	eye_y += OBJECT(current_object)->player_car.view_target_y;
	eye_z += OBJECT(current_object)->player_car.view_target_z;
	CAR(player_car)->view_x = eye_x + OBJECT(current_object)->player_car.view_target_x;
	CAR(player_car)->view_y = eye_y;
	CAR(player_car)->view_z = eye_z;
}

/* The engine and the speed: the force (brake, slope and drag, drive; acceleration) changes the speed
   (car speed, also impact_speed), with any speed_change from a hit, up to the top speed of the gear (its
   ratio, car gear_ratio); a force past the model's grip spins the wheels, and hard braking locks them
   (car wheel_slip: tyre noise 1 or the model's skid noise, smoke); the revs (car revs) follow the speed
   through the gear's ratio, or the gas when the car stands; car moving says it moves. */
void player_car_engine(uint32_t road_force, uint32_t drive_force, uint32_t force, uint32_t gear_ratio)
{
	bool L_00A8A3 = false, L_00A8E6 = false, L_00A914 = false, L_00A9E9 = false;
	uint32_t negative = 0, overflow = 0;
	uint8_t tyre_noise = 0;
	uint32_t revs = 0, revs_change = 0, speed = 0, speed_step = 0, speed_then_revs = 0, top_speed = 0;
	uint32_t spin_speed_change = 0, spin_speed = 0, spin_target_speed = 0, spin_top_speed = 0, target_revs = 0, spin_target_revs = 0, grip_speed_change = 0, grip_speed = 0, grip_target_speed = 0, grip_top_speed = 0;

	force = (int32_t)(force + road_force + drive_force) >> 6;
	OBJECT(current_object)->player_car.acceleration = force;
	top_speed = (gear_ratio * 0xFFFFu) >> 8;
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
						if (L_00A8E6 || L_00A9E9 || L_00A914 || L_00A914 || L_00A914 || CAR(player_car)->wheel_slip == 0u)
						{
							if (L_00A8E6 || !L_00A9E9)
							{
								if (L_00A8E6 || L_00A914 || L_00A914 || L_00A914 || CAR_HANDLING(r12)->spin_start_force <= (int32_t)force)
								{
									if (L_00A8E6 || !L_00A914)
									{
										if (L_00A8E6 || !L_00A914)
										{
											if (L_00A8E6 || !L_00A914)
											{
												if (!L_00A8E6)
													CAR(player_car)->wheel_slip = 0xFFu;
												if (CAR(player_car)->skidding == 0u)
												{
													tyre_noise = 1u;
													set_tyre_noise_on_road(tyre_noise);
												}
												force = CAR_HANDLING(r12)->spin_drive_force;
												if (CAR(player_car)->wheel_effect_sides == 0u)
												{
													CAR(player_car)->wheel_effect_1 = 1u;
													CAR(player_car)->wheel_effect_2 = 1u;
													CAR(player_car)->wheel_effect_sides = 3u;
												}
											}
										}
									}
									speed = CAR(player_car)->speed;
									spin_speed_change = OBJECT(current_object)->player_car.speed_change;
									spin_speed = speed + spin_speed_change;
									overflow = v60_overflow_add32(speed, spin_speed_change, 0u);
									negative = spin_speed >> 31;
									speed_then_revs = spin_speed;
									if (negative ^ overflow)
										speed_then_revs = 0u;
									speed_step = speed_then_revs + force;
									if ((int32_t)speed_step <= (int32_t)top_speed)
									{
										if ((int32_t)speed_step < 0)
											speed_step = 0u;
										spin_target_speed = speed_step;
										speed_step = spin_target_speed - speed_then_revs;
										if (spin_target_speed != speed_then_revs)
											speed_then_revs += (int32_t)(speed_step * 1u) / 2 + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
									}
									else
									{
										spin_top_speed = top_speed;
										speed_step = spin_top_speed - speed_then_revs;
										if (spin_top_speed != speed_then_revs)
											speed_then_revs += (int32_t)(speed_step * 1u) / 0x1E + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
										if (speed_then_revs >= 0xFFFFu)
											speed_then_revs = 0xFFFFu;
									}
									CAR(player_car)->speed = speed_then_revs;
									OBJECT(current_object)->player_car.impact_speed = speed_then_revs;
									if (speed_then_revs == 0u)
									{
										CAR(player_car)->moving = 0u;
										speed_then_revs = (uint32_t)OBJECT(current_object)->player_car.gas << 8;
										revs = CAR(player_car)->revs;
										target_revs = speed_then_revs;
										revs_change = target_revs - revs;
										if (target_revs != revs)
											revs += (int32_t)(revs_change * 1u) / 3 + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
										if (revs >= 0xFFFFu)
											revs = 0xFFFFu;
										CAR(player_car)->revs = revs;
										return;
									}
									CAR(player_car)->moving = 0xFFu;
									revs = CAR(player_car)->revs;
									spin_target_revs = divide_u32(speed_then_revs << 8, gear_ratio);
									revs_change = spin_target_revs - revs;
									if (spin_target_revs != revs)
										revs += (int32_t)(revs_change * 1u) / 0xA + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
									if (revs >= 0xFFFFu)
										revs = 0xFFFFu;
									CAR(player_car)->revs = revs;
									return;
								}
							}
							if (L_00A9E9 || CAR(player_car)->speed == 0u || (int32_t)force > -0x380)
							{
								CAR(player_car)->wheel_slip = 0u;
								speed = CAR(player_car)->speed;
								grip_speed_change = OBJECT(current_object)->player_car.speed_change;
								grip_speed = speed + grip_speed_change;
								overflow = v60_overflow_add32(speed, grip_speed_change, 0u);
								negative = grip_speed >> 31;
								speed_then_revs = grip_speed;
								if (negative ^ overflow)
									speed_then_revs = 0u;
								speed_step = speed_then_revs + force;
								if ((int32_t)speed_step <= (int32_t)top_speed)
								{
									if ((int32_t)speed_step < 0)
										speed_step = 0u;
									grip_target_speed = speed_step;
									speed_step = grip_target_speed - speed_then_revs;
									if (grip_target_speed != speed_then_revs)
										speed_then_revs += (int32_t)(speed_step * 1u) / 2 + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
								}
								else
								{
									grip_top_speed = top_speed;
									speed_step = grip_top_speed - speed_then_revs;
									if (grip_top_speed != speed_then_revs)
										speed_then_revs += (int32_t)(speed_step * 1u) / 0x1E + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
									if (speed_then_revs >= 0xFFFFu)
										speed_then_revs = 0xFFFFu;
								}
								CAR(player_car)->speed = speed_then_revs;
								OBJECT(current_object)->player_car.impact_speed = speed_then_revs;
								if (speed_then_revs == 0u)
								{
									CAR(player_car)->moving = 0u;
									speed_then_revs = (uint32_t)OBJECT(current_object)->player_car.gas << 8;
									revs = CAR(player_car)->revs;
									target_revs = speed_then_revs;
									revs_change = target_revs - revs;
									if (target_revs != revs)
										revs += (int32_t)(revs_change * 1u) / 3 + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
									if (revs >= 0xFFFFu)
										revs = 0xFFFFu;
									CAR(player_car)->revs = revs;
									return;
								}
								CAR(player_car)->moving = 0xFFu;
								speed_then_revs = divide_u32(speed_then_revs << 8, gear_ratio);
								revs = CAR(player_car)->revs;
								target_revs = speed_then_revs;
								revs_change = target_revs - revs;
								if (target_revs != revs)
									revs += (int32_t)(revs_change * 1u) / 3 + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
								if (revs >= 0xFFFFu)
									revs = 0xFFFFu;
								CAR(player_car)->revs = revs;
								return;
							}
							CAR(player_car)->wheel_slip = 0xFFu;
							L_00A8A3 = true;
						}
						else
						{
							L_00A8E6 = CAR_HANDLING(r12)->spin_end_force <= (int32_t)force;
							if (!L_00A8E6)
							{
								L_00A9E9 = CAR(player_car)->speed == 0u || (int32_t)force > -0x480;
								if (!L_00A9E9)
									L_00A8A3 = true;
							}
						}
					} while (L_00A8E6);
				} while (L_00A9E9);
				if (L_00A8A3)
				{
					L_00A8A3 = false;
					L_00A914 = SCREEN(screen)->brake == 0u;
					if (!L_00A914)
					{
						if (CAR(player_car)->skidding == 0u)
						{
							tyre_noise = car_skid_noises[CAR(player_car)->model];
							set_tyre_noise(tyre_noise);
						}
						L_00A914 = CAR(player_car)->wheel_effect_sides != 0u;
						if (!L_00A914)
						{
							CAR(player_car)->wheel_effect_1 = 2u;
							CAR(player_car)->wheel_effect_2 = 2u;
							CAR(player_car)->wheel_effect_sides = 3u;
							L_00A914 = true;
						}
					}
				}
			} while (L_00A914);
		} while (L_00A914);
	} while (L_00A914);
}

/* Returns the offset of length R2 along pitch R0 and yaw R1 (R3, R4, R5: x, y, z). Used by
   player_car_place_view for the eye behind the view's target. */
void angles_to_offset(uint32_t pitch, uint32_t yaw, uint32_t distance, uint32_t *p_offset_x, uint32_t *p_offset_y, uint32_t *p_offset_z)
{
	uint32_t pitch_cosine = 0;

	pitch_cosine = cosine_table[pitch];
	*p_offset_x = (int32_t)(((int32_t)(sine_table[yaw] * pitch_cosine) >> 14) * distance) >> 14;
	*p_offset_y = 0u - ((int32_t)(sine_table[pitch] * distance) >> 14);
	*p_offset_z = (int32_t)(((int32_t)(cosine_table[yaw] * pitch_cosine) >> 14) * distance) >> 14;
}

/* Unused (only unk_00AB04 lists it, for octants 1, 2, 5 and 6 of an angle): the distance along angle R0
   that has the x distance from R11's x (+4) to the object's field 20 (that distance / the sine of R0),
   turned to angle R1 by distance_offset_at_angle. Its C has no body: nothing reads the results (R2, R3). */
void distance_offset_from_x(void)
{
	distance_offset_at_angle();
}

/* Unused (only unk_00AB04 lists it, for octants 0, 3, 4 and 7 of an angle): the distance along angle R0
   that has the z distance from R11's z (+8) to the object's field 28 (that distance / the cosine of R0),
   turned to angle R1 by distance_offset_at_angle. Its C has no body: nothing reads the results (R2, R3). */
void distance_offset_from_z(void)
{
	distance_offset_at_angle();
}

/* Unused (the end of distance_offset_from_x and distance_offset_from_z): R2 / R6 as a length along angle
   R1, its sine part into R2 and its cosine part into R3. Its C has no body: nothing reads the results. */
void distance_offset_at_angle(void)
{
}

/* Takes the roadside surfaces of the stage (screen built_stage): the two lists of
   stage_roadside_surface_lists for it (car surface_entry_1, unk_14C), from the segment where the stage
   starts (car surface_start) to the next one's (unk_4C). */
void player_car_next_surfaces(void)
{
	uint16_t stage_length = 0;
	uint32_t stage_then_surfaces = 0;

	stage_then_surfaces = SCREEN(screen)->built_stage;
	CAR(player_car)->surface_stage = stage_then_surfaces;
	stage_length = (uint16_t)(mem_read16(screen + 0x700u + stage_then_surfaces * 2u) + SCREEN(screen)->branch_road_length);
	CAR(player_car)->surface_start = CAR(player_car)->surface_next_start;
	CAR(player_car)->surface_next_start += stage_length;
	stage_then_surfaces = stage_roadside_surface_lists_ADDRESS + (uint8_t)(stage_then_surfaces * 2u) * 4u;
	CAR(player_car)->surface_entry_1 = STAGE_ROADSIDE_SURFACES(stage_then_surfaces)->first;
	CAR(player_car)->surface_entry_2 = STAGE_ROADSIDE_SURFACES(stage_then_surfaces)->second;
}

/* The drag on the car: speed squared, times the model's off-road factor while slowed (car slowed_frames)
   or off the road (car off_road, with the off-road shake); and the roadside surfaces under its wheels
   (car surface_entry_1, unk_14C) give the effects at each side (car wheel_effect_sides: bit 0 with
   unk_141, bit 1 with unk_142). */
uint32_t player_car_road_drag(uint32_t drag, uint16_t road_x, uint16_t road_half_width, uint16_t half_width)
{
	uint32_t carry = 0;
	uint16_t beyond_edge = 0, inner_edge_1 = 0, inner_edge_2 = 0, low_side = 0, outer_edge_1 = 0, outer_edge_2 = 0;
	uint32_t empty_bits = 0, roadside_surface = 0, segment = 0, surface_bits = 0, surface_start = 0;

	roadside_surface = CAR(player_car)->surface_entry_1;
	r14 = CAR(player_car)->surface_entry_2;
	if (ROADSIDE_SURFACE(roadside_surface)->end < 0 && ROADSIDE_SURFACE(r14)->end < 0 && CAR(player_car)->surface_next_start <= CAR(player_car)->segment)
	{
		player_car_next_surfaces();
		roadside_surface = CAR(player_car)->surface_entry_1;
		r14 = CAR(player_car)->surface_entry_2;
	}
	segment = CAR(player_car)->segment;
	surface_start = CAR(player_car)->surface_start;
	if ((uint16_t)ROADSIDE_SURFACE(roadside_surface)->end + surface_start <= segment)
	{
		roadside_surface += 8u;
		CAR(player_car)->surface_entry_1 = roadside_surface;
	}
	if ((uint16_t)ROADSIDE_SURFACE(r14)->end + surface_start <= segment)
	{
		r14 = r14 + 8u;
		CAR(player_car)->surface_entry_2 = r14;
	}
	if (CAR(player_car)->slowed_frames != 0u)
	{
		CAR(player_car)->slowed_frames--;
		drag = (drag * CAR_HANDLING(r12)->off_road_drag) >> 4;
		player_car_off_road_shake();
		return drag;
	}
	low_side = (uint16_t)(road_x - half_width);
	empty_bits = 0u;
	half_width = (uint16_t)(half_width + road_x - 0x8000u);
	low_side -= 0x8000u;
	carry = (uint16_t)(0x8000u + road_half_width) < half_width;
	surface_bits = carry;
	carry = low_side < (uint16_t)(0u - road_half_width - 0x8000u);
	CAR(player_car)->off_road = 0u;
	if ((uint8_t)(surface_bits + carry) != 0u)
	{
		CAR(player_car)->off_road = 0xFFu;
		drag = (drag * CAR_HANDLING(r12)->off_road_drag) >> 4;
		player_car_off_road_shake();
	}
	inner_edge_1 = ROADSIDE_SURFACE(roadside_surface)->inner_edge;
	outer_edge_1 = ROADSIDE_SURFACE(roadside_surface)->outer_edge;
	inner_edge_2 = ROADSIDE_SURFACE(r14)->inner_edge;
	outer_edge_2 = ROADSIDE_SURFACE(r14)->outer_edge;
	beyond_edge = (uint16_t)(half_width - road_half_width);
	half_width += road_half_width;
	carry = inner_edge_1 < beyond_edge;
	surface_bits = (uint8_t)(empty_bits * 2u + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = outer_edge_1 < beyond_edge;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = half_width < inner_edge_2;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = half_width < outer_edge_2;
	switch ((uint8_t)(surface_bits + empty_bits + carry))
	{
	case 0:
		wheel_effect_1_none();
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
		wheel_effect_1_surface_2_outer();
		break;
	case 2:
	case 6:
	case 10:
	case 14:
		wheel_effect_1_surface_2_inner();
		break;
	case 4:
	case 12:
		wheel_effect_1_surface_1_outer(roadside_surface);
		break;
	case 8:
		wheel_effect_1_surface_1_inner(roadside_surface);
		break;
	case 16:
		wheel_effect_2_none();
		break;
	case 17:
	case 19:
		wheel_effect_2_surface_2_outer();
		break;
	case 18:
		wheel_effect_2_surface_2_inner();
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 28:
	case 29:
	case 30:
	case 31:
		wheel_effect_2_surface_1_outer(roadside_surface);
		break;
	case 24:
	case 25:
	case 26:
	case 27:
		wheel_effect_2_surface_1_inner(roadside_surface);
		break;
	default:
		invalid_jump(wheel_effect_1_jumps[(uint8_t)(surface_bits + empty_bits + carry)]);
	}
	beyond_edge = (uint16_t)(low_side - road_half_width);
	low_side += road_half_width;
	carry = inner_edge_1 < beyond_edge;
	surface_bits = (uint8_t)(empty_bits * 2u + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = outer_edge_1 < beyond_edge;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = low_side < inner_edge_2;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = low_side < outer_edge_2;
	switch ((uint8_t)(surface_bits + empty_bits + carry))
	{
	case 0:
		wheel_effect_2_none();
		return drag;
	case 8:
	case 9:
	case 10:
	case 11:
		wheel_effect_2_surface_1_inner(roadside_surface);
		return drag;
	case 4:
	case 5:
	case 6:
	case 7:
	case 12:
	case 13:
	case 14:
	case 15:
		wheel_effect_2_surface_1_outer(roadside_surface);
		return drag;
	case 2:
		wheel_effect_2_surface_2_inner();
		return drag;
	case 1:
	case 3:
		wheel_effect_2_surface_2_outer();
		return drag;
	default:
		invalid_jump(wheel_effect_2_jumps[(uint8_t)(surface_bits + empty_bits + carry)]);
	}
}

/* The wheel at road_x plus the model's half track (handling +1A) is within the bounds of both surface
   entries: no effect (player_car_road_drag's pick, wheel_effect_1_jumps). */
void wheel_effect_1_none(void)
{
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+6) of surface_entry_1 past its inner bound
   (+2): the wheel at road_x plus the half track is past it and no other bound (player_car_road_drag's
   pick, wheel_effect_1_jumps). */
void wheel_effect_1_surface_1_inner(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(surface)->inner_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+7) of surface_entry_1 past its outer bound
   (+4), when the wheel at road_x plus the half track is past it and not past the bounds of
   surface_entry_2. */
void wheel_effect_1_surface_1_outer(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(surface)->outer_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+6) of surface_entry_2 past its inner bound
   (+2), when the wheel at road_x plus the half track is past it but not past its outer bound. */
void wheel_effect_1_surface_2_inner(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(r14)->inner_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+7) of surface_entry_2 past its outer bound
   (+4): for the wheel at road_x plus the half track it comes before every other. */
void wheel_effect_1_surface_2_outer(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(r14)->outer_effect;
}

/* The wheel at road_x less the model's half track is within the bounds of both surface entries: no
   effect (player_car_road_drag's pick, wheel_effect_2_jumps). */
void wheel_effect_2_none(void)
{
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+6) of surface_entry_1 past its inner bound
   (+2), when the wheel at road_x less the half track is past it but not past its outer bound. */
void wheel_effect_2_surface_1_inner(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(surface)->inner_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+7) of surface_entry_1 past its outer bound
   (+4): for the wheel at road_x less the half track it comes before every other. */
void wheel_effect_2_surface_1_outer(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(surface)->outer_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+6) of surface_entry_2 past its inner bound
   (+2), when the wheel at road_x less the half track passes no bound of surface_entry_1 nor the outer
   one of surface_entry_2. */
void wheel_effect_2_surface_2_inner(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(r14)->inner_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+7) of surface_entry_2 past its outer bound
   (+4), when the wheel at road_x less the half track passes no bound of surface_entry_1. */
void wheel_effect_2_surface_2_outer(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(r14)->outer_effect;
}

/* Off the road: the model's shake to the screen's bits of output port D (wheel_motor, the wheel motor's;
   car_model_off_road_shakes) and its off-road noise (car_model_off_road_noises). */
void player_car_off_road_shake(void)
{
	uint8_t tyre_noise = 0;
	uint32_t model = 0;

	model = CAR(player_car)->model;
	wheel_motor[SCREEN(screen)->index] = car_model_off_road_shakes[model];
	tyre_noise = car_model_off_road_noises[model];
	set_tyre_noise(tyre_noise);
}

/* The rev meter, the bar left of the speed at the bottom right: state 0 sets it up (animation 6, at F2,
   D6), 1 draws the frame of the revs (car revs >> 11, 32 steps). */
void rev_meter_update(void)
{
	uint32_t animation = 0, state_code = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_code = rev_meter_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		OBJECT(current_object)->frame = CAR(player_car)->revs >> 11;
		state_code = draw_object(state_code);
		return;
	default:
		invalid_jump(rev_meter_state_jumps[state]);
	}
	OBJECT(current_object)->state++;
	animation = 6u;
	OBJECT(current_object)->palette = 0x44u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF2u;
	OBJECT(current_object)->y = 0xD6u;
	animation_start(animation);
}

/* The speed in km/h at the bottom right: state 0 sets it up (digits of animation 12, units_x, digits_y)
   and clears the screen's maximum (unk_F52), sum (unk_F54) and count (unk_F58); 1 each frame the car's
   speed / 88 into screen shown_speed, and while moving adds it to the sum, counts it and keeps the
   maximum (for the LCD's pages), then draws its digits from the units leftwards. */
void speedometer_update(void)
{
	uint32_t animation = 0, digit = 0, speed_kmh = 0, state_code = 0;
	uint32_t saved_speed_kmh = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_code = speedometer_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = 0x12u;
		OBJECT(current_object)->palette = 0x48u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->speedometer.units_x = 0x118u;
		OBJECT(current_object)->speedometer.digits_y = 0xCEu;
		SCREEN(screen)->speed_sum = 0u;
		SCREEN(screen)->speed_samples = 0u;
		SCREEN(screen)->maximum_speed = 0;
		animation_start(animation);
		return;
	case 1:
		speed_kmh = OBJECT(CAR(player_car)->object)->player_car.speed;
		SCREEN(screen)->shown_speed = speed_kmh;
		if (speed_kmh != 0u)
		{
			speed_kmh = speed_kmh / 0x88u;
			SCREEN(screen)->shown_speed = speed_kmh;
			SCREEN(screen)->speed_sum += speed_kmh;
			SCREEN(screen)->speed_samples++;
			if (SCREEN(screen)->maximum_speed < (int16_t)speed_kmh)
				SCREEN(screen)->maximum_speed = speed_kmh;
		}
		OBJECT(current_object)->x = OBJECT(current_object)->speedometer.units_x;
		OBJECT(current_object)->y = OBJECT(current_object)->speedometer.digits_y;
		do
		{
			digit = 0u;
			divx_pair(0xAu, &speed_kmh, &digit);
			OBJECT(current_object)->frame = digit;
			saved_speed_kmh = speed_kmh;
			state_code = draw_object(state_code);
			speed_kmh = saved_speed_kmh;
			OBJECT(current_object)->x -= 8u;
		} while ((uint16_t)speed_kmh != 0u);
		return;
	default:
		invalid_jump(speedometer_state_jumps[state]);
	}
}

/* The km/h after the speed: state 0 sets it up (animation 10 at 127, CE), 1 draws it. */
void speed_unit_update(void)
{
	uint32_t animation = 0, state_code = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_code = speed_unit_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		state_code = draw_object(state_code);
		return;
	default:
		invalid_jump(speed_unit_state_jumps[state]);
	}
	OBJECT(current_object)->state++;
	animation = 0x10u;
	OBJECT(current_object)->palette = 0x47u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x127u;
	OBJECT(current_object)->y = 0xCEu;
	animation_start(animation);
}

/* The gear at the bottom right, and the shifting: state 0 sets it up (gear_indicator_animations by the
   top gear: AUTO for the automatic). 1 each frame: the shift buttons (screen buttons_pressed bits 4 up,
   5 down) change the gear within 0 and the top gear; shifting up at revs F000 or more, or down into revs
   past FFFF, spins the wheels 30 frames (car shift_spin, tyre noise 1); the gear's ratio to car
   gear_ratio, its frame drawn. Below the top gear with the revs at the limit (FF00) and the car moving,
   after 150 frames starts the shift_up_hint once. Once the race is over (car at_goal or unk_182), 2 only
   draws. */
void gear_indicator_update(void)
{
	uint8_t buttons_pressed = 0, tyre_noise = 0;
	uint32_t animation = 0, gear = 0, object = 0, old_ratio = 0, state_code = 0, top_gear_then_frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_code = gear_indicator_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = gear_indicator_animations[CAR(player_car)->top_gear];
		OBJECT(current_object)->palette = 0x45u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0x114u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(animation);
		return;
	case 1:
		if (CAR(player_car)->shift_spin != 0u)
		{
			CAR(player_car)->shift_spin--;
			tyre_noise = 1u;
			set_tyre_noise_on_road(tyre_noise);
		}
		top_gear_then_frames = CAR(player_car)->top_gear;
		gear = CAR(player_car)->gear;
		buttons_pressed = (uint8_t)SCREEN(screen)->buttons_pressed;
		if ((buttons_pressed & 0x10u) != 0u && gear != top_gear_then_frames)
		{
			gear = (uint8_t)(gear + 1u);
			if (CAR(player_car)->revs >= 0xF000u)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		if ((buttons_pressed & 0x20u) != 0u && gear != 0u)
		{
			old_ratio = CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[gear];
			gear = (uint8_t)(gear - 1u);
			if (divide_u32(CAR(player_car)->revs * old_ratio, CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[gear]) >= 0xFFFFu)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		CAR(player_car)->gear = gear;
		CAR(player_car)->gear_ratio = CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[gear];
		OBJECT(current_object)->frame = gear;
		if (gear == top_gear_then_frames)
		{
			if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
				OBJECT(current_object)->state++;
			top_gear_then_frames = draw_object(top_gear_then_frames);
			return;
		}
		top_gear_then_frames = 0u;
		if (CAR(player_car)->revs >= 0xFF00u && CAR(player_car)->speed != 0u)
		{
			top_gear_then_frames = (uint16_t)(OBJECT(current_object)->gear_indicator.redline_frames + 1u);
			if (OBJECT(current_object)->gear_indicator.hint_started == 0u && top_gear_then_frames >= 0x96u)
			{
				OBJECT(current_object)->gear_indicator.hint_started = 0xFFu;
				object = start_object();
				OBJECT(object)->handler = shift_up_hint_update_ADDRESS;
				OBJECT(object)->shift_up_hint.indicator = current_object;
				OBJECT(object)->shift_up_hint.gear = gear;
			}
		}
		OBJECT(current_object)->gear_indicator.redline_frames = top_gear_then_frames;
		if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
			OBJECT(current_object)->state++;
		top_gear_then_frames = draw_object(top_gear_then_frames);
		return;
	case 2:
		state_code = draw_object(state_code);
		return;
	default:
		invalid_jump(gear_indicator_state_jumps[state]);
	}
}

/* A hint to shift up, blinking at the bottom centre (animation E): state 0 sets it up; 1 blinks it (4
   frames of 8) while the car stays in the gear it started in with the revs at FC00 or more, then ends
   and lets the gear indicator start it again. */
void shift_up_hint_update(void)
{
	uint16_t state_code = 0;
	uint32_t animation = 0, blink = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_code = (uint16_t)shift_up_hint_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = 0xEu;
		OBJECT(current_object)->palette = 0x46u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(animation);
		return;
	case 1:
		if (CAR(player_car)->gear != OBJECT(current_object)->shift_up_hint.gear || CAR(player_car)->revs < 0xFC00u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.hint_started = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.redline_frames = 0u;
			return;
		}
		blink = (state_code & ~0xFFu) | OBJECT(current_object)->shift_up_hint.blink;
		blink = (blink & ~0xFFu) | (uint8_t)(blink + 1u);
		OBJECT(current_object)->shift_up_hint.blink = blink;
		if ((blink & 4u) != 0u)
			return;
		blink = draw_object(blink);
		return;
	default:
		invalid_jump(shift_up_hint_state_jumps[state]);
	}
}

/* The number of cars after the place at the bottom left (/2): ends at once with one player; state 0 sets
   it up (animation 5, frame the players less one) and starts the place_number; 1 draws it, and when the
   screen's place (screen place) changes passes the new one to the place_number (its place, state 2). */
void place_count_update(void)
{
	uint32_t animation = 0, count_frame = 0, initial_place = 0, object = 0, place = 0;
	uint32_t saved_count_frame = 0;
	uint8_t racer_count = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	place = place_count_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		initial_place = (place & ~0xFFu) | SCREEN(screen)->place;
		if (OBJECT(current_object)->place_count.place != (uint8_t)initial_place)
		{
			OBJECT(current_object)->place_count.place = initial_place;
			object = OBJECT(current_object)->place_count.number;
			OBJECT(object)->place_number.place = initial_place;
			OBJECT(object)->state = 2u;
		}
		initial_place = draw_object(initial_place);
		return;
	default:
		invalid_jump(place_count_state_jumps[state]);
	}
	count_frame = (place & ~0xFFu) | RACE(SCREEN(screen)->host_record)->racer_count;
	racer_count = count_frame;
	count_frame = (count_frame & ~0xFFu) | (uint8_t)(racer_count - 1u);
	if (racer_count == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	animation = 5u;
	OBJECT(current_object)->palette = 0x43u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->x = 0x2Au;
	OBJECT(current_object)->y = 0xD8u;
	saved_count_frame = count_frame;
	animation_start(animation);
	OBJECT(current_object)->frame = saved_count_frame;
	place = (count_frame & ~0xFFu) | SCREEN(screen)->place;
	OBJECT(current_object)->place_count.place = place;
	object = start_object();
	OBJECT(object)->handler = place_number_update_ADDRESS;
	OBJECT(object)->place_number.place = place;
	OBJECT(current_object)->place_count.number = object;
	OBJECT(current_object)->state++;
	initial_place = (place & ~0xFFu) | SCREEN(screen)->place;
	if (OBJECT(current_object)->place_count.place != (uint8_t)initial_place)
	{
		OBJECT(current_object)->place_count.place = initial_place;
		object = OBJECT(current_object)->place_count.number;
		OBJECT(object)->place_number.place = initial_place;
		OBJECT(object)->state = 2u;
	}
	initial_place = draw_object(initial_place);
}

/* The screen's place, 1st or 2nd, at the bottom left: state 0 sets it up (animation 4, the place's frame
   and colour); 1 draws it; 2 a new place: twice the size and blinking for 40 frames, 4 shrinking back
   into place in 8, then 5 draws it. */
void place_number_update(void)
{
	uint32_t animation = 0, new_palette = 0, palette = 0;
	uint8_t place = 0, new_place = 0, frames = 0, blink_timer = 0, frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	new_palette = place_number_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 4u;
		animation_start(animation);
		place = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = place;
		palette = 0x41u;
		if (place != 0u)
			palette = 0x42u;
		OBJECT(current_object)->palette = palette;
		OBJECT(current_object)->x = 0x1Cu;
		OBJECT(current_object)->y = 0xD8u;
		OBJECT(current_object)->state++;
		palette = draw_object(palette);
		return;
	case 2:
		new_place = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = new_place;
		new_palette = 0x41u;
		if (new_place != 0u)
			new_palette = 0x42u;
		OBJECT(current_object)->palette = new_palette;
		OBJECT(current_object)->x = 0x2Cu;
		OBJECT(current_object)->y = 0xD8u;
		OBJECT(current_object)->width_scale = 0x200u;
		OBJECT(current_object)->height_scale = 0x200u;
		OBJECT(current_object)->place_number.frames = 0x28u;
		OBJECT(current_object)->place_number.blink_timer = 4u;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->x -= 2u;
		OBJECT(current_object)->width_scale -= 0x20u;
		OBJECT(current_object)->height_scale -= 0x20u;
		frames = OBJECT(current_object)->place_number.frames;
		OBJECT(current_object)->place_number.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0u;
			OBJECT(current_object)->state++;
		}
		new_palette = draw_object(new_palette);
		return;
	case 1:
	case 5:
		new_palette = draw_object(new_palette);
		return;
	default:
		invalid_jump(place_number_state_jumps[state]);
	}
	blink_timer = OBJECT(current_object)->place_number.blink_timer;
	OBJECT(current_object)->place_number.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		OBJECT(current_object)->place_number.blink_timer = 4u;
		OBJECT(current_object)->flags ^= 2u;
	}
	frames_left = OBJECT(current_object)->place_number.frames;
	OBJECT(current_object)->place_number.frames = frames_left - 1u;
	if (frames_left != 1u)
	{
		new_palette = draw_object(new_palette);
		return;
	}
	OBJECT(current_object)->flags &= 0xFDu;
	OBJECT(current_object)->flags &= 0xDFu;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->place_number.frames = 8u;
	OBJECT(current_object)->state++;
	OBJECT(current_object)->x -= 2u;
	OBJECT(current_object)->width_scale -= 0x20u;
	OBJECT(current_object)->height_scale -= 0x20u;
	frames = OBJECT(current_object)->place_number.frames;
	OBJECT(current_object)->place_number.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->state++;
	}
	new_palette = draw_object(new_palette);
}

/* The lap time after LAP at the top right: state 0 sets it up (the label, the text record and the
   checkpoints passed); 1 shows the race time since the last checkpoint (car race_time / 10 less car
   checkpoint_time, time_to_text); when the screen's car passes a checkpoint (screen 480 + player) the
   time it took (screen 3C0 records) starts the next lap (car checkpoint_time) and stays shown in another
   colour for 180 frames (split_frames; state 2), and C00074, which the board does not map, gets 8 + 2 *
   screen + (1 - the stage's parity); after the last (lap_timer_checkpoint_states) 3 keeps the last lap's
   time. */
void lap_timer_update(void)
{
	bool L_00C4B3 = false;
	uint32_t animation = 0, checkpoint_signal = 0, checkpoint_time = 0, checkpoints = 0, lap_time = 0, object = 0, player = 0, text = 0, time_text = 0;
	uint16_t split_frames = 0;
	uint32_t state = 0, jump_target = 0;

	state = OBJECT(current_object)->state;
	checkpoint_signal = lap_timer_state_jumps[state];
	do
	{
		jump_target = L_00C4B3 ? 0xC4B3u : lap_timer_state_jumps[state];
		switch (jump_target)
		{
		case 0x00C45Cu:
			animation = 0x2Du;
			OBJECT(current_object)->palette = 0x4Au;
			OBJECT(current_object)->draw_mode = 8u;
			OBJECT(current_object)->depth_layer = 0x19Eu;
			OBJECT(current_object)->x = 0xE0u;
			OBJECT(current_object)->y = 0x16u;
			animation_start(animation);
			object = current_object + 0x80u;
			OBJECT(object - 0x80u)->lap_timer.text_x = 0xF8u;
			object += 2u;
			OBJECT(object - 0x82u)->lap_timer.text_y = 0xEu;
			OBJECT(object - 0x82u)->lap_timer.text_attribute = 0x90u;
			OBJECT(current_object)->lap_timer.checkpoints = RACE(SCREEN(screen)->race_copy)->stage_count;
			OBJECT(current_object)->state++;
			return;
		case 0x00C4B3u:
			for (;;)
			{
				text = current_object + 0x80u;
				time_text = text + 6u;
				player = SCREEN(screen)->player;
				checkpoints = mem_read8(screen + 0x480u + player);
				if (OBJECT(current_object)->lap_timer.checkpoints == checkpoints)
					break;
				OBJECT(current_object)->lap_timer.checkpoints = checkpoints;
				checkpoints = (uint8_t)(checkpoints - 1u);
				OBJECT(current_object)->state = lap_timer_checkpoint_states[checkpoints];
				checkpoints = (uint8_t)(checkpoints << 3);
				lap_time = mem_read16(screen + 0x3C0u + (uint8_t)(checkpoints + player) * 2u);
				checkpoint_time = CAR(player_car)->checkpoint_time;
				CAR(player_car)->checkpoint_time = lap_time;
				lap_time -= checkpoint_time;
				time_to_text(lap_time, time_text);
				OBJECT(current_object)->lap_timer.text_attribute = 0xB0u;
				OBJECT(current_object)->lap_timer.split_frames = 0xB4u;
				checkpoint_signal = (lap_time & ~0xFFu) | SCREEN(screen)->index;
				checkpoint_signal = (checkpoint_signal & ~0xFFu) | (uint8_t)(checkpoint_signal * 2u);
				checkpoint_signal = (checkpoint_signal & ~0xFFu) | (uint8_t)(checkpoint_signal + (~SCREEN(screen)->current_stage_level & 1u));
				checkpoint_signal = (checkpoint_signal & ~0xFFu) | (uint8_t)(checkpoint_signal + 8u);
				mem_write8(IO_C00074, checkpoint_signal);
				split_frames = OBJECT(current_object)->lap_timer.split_frames;
				OBJECT(current_object)->lap_timer.split_frames = split_frames - 1u;
				if (split_frames != 1u)
				{
					text = current_object + 0x80u;
					draw_text(text);
					checkpoint_signal = draw_object(checkpoint_signal);
					return;
				}
				OBJECT(current_object)->lap_timer.text_attribute = 0x90u;
				OBJECT(current_object)->state--;
			}
			lap_time = CAR(player_car)->race_time / 0xAu - CAR(player_car)->checkpoint_time;
			time_to_text(lap_time, time_text);
			draw_text(text);
			lap_time = draw_object(lap_time);
			return;
		case 0x00C53Bu:
			split_frames = OBJECT(current_object)->lap_timer.split_frames;
			OBJECT(current_object)->lap_timer.split_frames = split_frames - 1u;
			if (split_frames != 1u)
			{
				text = current_object + 0x80u;
				draw_text(text);
				checkpoint_signal = draw_object(checkpoint_signal);
				return;
			}
			OBJECT(current_object)->lap_timer.text_attribute = 0x90u;
			OBJECT(current_object)->state--;
			L_00C4B3 = true;
			break;
		case 0x00C541u:
			text = current_object + 0x80u;
			draw_text(text);
			checkpoint_signal = draw_object(checkpoint_signal);
			return;
		default:
			invalid_jump(jump_target);
		}
	} while (L_00C4B3);
}

/* Unused: does nothing (a lone return after lap_timer_update's last jump to draw_object). */
void lap_timer_spare_return(void)
{
}

/* Sets up the STAGE label at the top left of the race screen (sprite set 2E), then stage_indicator_draw. */
void stage_indicator_start(void)
{
	uint32_t animation = 0;

	animation = 0x2Eu;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x20u;
	OBJECT(current_object)->y = 0x16u;
	animation_start(animation);
	OBJECT(current_object)->handler = stage_indicator_draw_ADDRESS;
	stage_indicator_draw();
}

/* Draws the stage's name after the label (stage_level_texts by the forks taken, screen
   route_stage_level: PRE-STAGE, 1 to 4, FINAL STAGE) and the label itself. */
void stage_indicator_draw(void)
{
	uint32_t stage_level = 0, text = 0;

	stage_level = SCREEN(screen)->route_stage_level;
	text = stage_level_texts[stage_level];
	draw_text(text);
	stage_level = draw_object(stage_level);
}

/* The start's messages in the middle of the screen: state 0 shows YOU'RE IN CAR NUMBER n! (animation 28F
   + the screen's car number, screen car_number, with DJ message 21 + it); when the start's countdown
   (the car object's unk_10) is at 30 frames, Ready (animation 31, DJ message 31); at 0, Go! (animation
   32, DJ message 32), 30 frames, then it ends. */
void start_message_update(void)
{
	uint32_t animation = 0, dj_message = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	dj_message = start_message_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x80u;
		animation = SCREEN(screen)->car_number;
		animation = (uint16_t)(animation + 0x28Fu);
		animation_start(animation);
		OBJECT(current_object)->palette = CAR(player_car)->model + 0x289u;
		dj_message = (uint8_t)(0x21u + SCREEN(screen)->car_number);
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			dj_message = draw_object(dj_message);
			return;
		}
		animation = 0x31u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		dj_message = 0x31u;
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			dj_message = draw_object(dj_message);
			return;
		}
		animation = 0x31u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		dj_message = 0x31u;
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	case 3:
		frames = OBJECT(current_object)->start_message.frames;
		OBJECT(current_object)->start_message.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		dj_message = draw_object(dj_message);
		return;
	default:
		invalid_jump(start_message_state_jumps[state]);
	}
	if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
	{
		dj_message = draw_object(dj_message);
		return;
	}
	animation = 0x32u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x4Bu;
	OBJECT(current_object)->start_message.frames = 0x1Eu;
	dj_message = 0x32u;
	start_dj_message((uint8_t)dj_message);
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->start_message.frames;
	OBJECT(current_object)->start_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	dj_message = draw_object(dj_message);
}

/* The time left in seconds at the top centre, and the time up: state 0 waits for the race's time
   (unk_308's +6A), takes it into car time_left and sets the digits up (time_digits_start); 1 shows it
   during the start's countdown; 2 follows the race's time (until the screen's car has finished,
   race_copy finished) and starts EXTEND TIME at each checkpoint (time_left_display_check_extension); at
   0 the time is up (car time_up) and 3 shows 0, until time comes back (the car drives on: its state 2).
   The digits flip as they change (time_digits_draw). */
void time_left_display_update(void)
{
	uint32_t shown_time = 0, time_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		time_left = RACE(SCREEN(screen)->host_record)->time_left;
		if (time_left == 0u)
			return;
		CAR(player_car)->time_left = time_left;
		OBJECT(current_object)->state++;
		time_digits_start();
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->state != 1u)
		{
			OBJECT(current_object)->state++;
			time_left_display_check_extension();
			if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
				CAR(player_car)->time_left = RACE(SCREEN(screen)->host_record)->time_left;
		}
		break;
	case 2:
		time_left_display_check_extension();
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
			CAR(player_car)->time_left = RACE(SCREEN(screen)->host_record)->time_left;
		break;
	case 3:
		time_left_display_check_extension();
		shown_time = RACE(SCREEN(screen)->host_record)->time_left;
		if (shown_time != 0u)
		{
			CAR(player_car)->time_left = shown_time;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->player_car.state_high = 0u;
		}
		shown_time = binary_to_bcd(shown_time);
		time_digits_draw(shown_time);
		return;
	default:
		invalid_jump(time_left_display_state_jumps[OBJECT(current_object)->state]);
	}
	shown_time = CAR(player_car)->time_left;
	if (shown_time == 0u)
	{
		OBJECT(current_object)->state++;
		CAR(player_car)->time_up = 0xFFu;
		time_left_display_check_extension();
		shown_time = RACE(SCREEN(screen)->host_record)->time_left;
		if (shown_time != 0u)
		{
			CAR(player_car)->time_left = shown_time;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->player_car.state_high = 0u;
		}
		shown_time = binary_to_bcd(shown_time);
		time_digits_draw(shown_time);
		return;
	}
	shown_time = shown_time / 0x1Eu;
	shown_time = binary_to_bcd(shown_time);
	time_digits_draw(shown_time);
}

/* Sets up the three digits of the time at the top centre (animation 3 at A9, 11), each to open from
   nothing at its first value. Used by time_left_display, car_select_run and 001EC2. */
void time_digits_start(void)
{
	uint32_t animation = 0, object = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->time_digits.units_flip = 1;
	OBJECT(object - 0x80u)->time_digits.units_digit = 0u;
	OBJECT(object - 0x80u)->time_digits.units_width = 0x40u;
	object += 4u;
	OBJECT(object - 0x84u)->time_digits.units_remaining = 0u;
	object += 4u;
	OBJECT(object - 0x88u)->time_digits.tens_flip = 1;
	OBJECT(object - 0x88u)->time_digits.tens_digit = 0u;
	OBJECT(object - 0x88u)->time_digits.tens_width = 0x40u;
	object += 4u;
	OBJECT(object - 0x8Cu)->time_digits.tens_remaining = 0u;
	object += 4u;
	OBJECT(object - 0x90u)->time_digits.hundreds_flip = 1;
	OBJECT(object - 0x90u)->time_digits.hundreds_digit = 0u;
	OBJECT(object - 0x90u)->time_digits.hundreds_width = 0x40u;
	OBJECT(object - 0x90u)->time_digits.hundreds_remaining = 0u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 0u;
	animation = 3u;
	OBJECT(current_object)->palette = 0x40u;
	OBJECT(current_object)->time_digits.units_x = 0xA9u;
	OBJECT(current_object)->y = 0x11u;
	animation_start(animation);
}

/* Draws the three digits of R0 (binary-coded decimal), the units at the right: a digit that changes
   squeezes shut and opens again as the new one; leading zeros are left out. */
void time_digits_draw(uint32_t digits)
{
	uint32_t negative = 0;
	uint8_t digit = 0, shown_digit = 0;
	uint16_t flip_width = 0;
	uint32_t digit_record = 0, digits_left = 0, remaining_digits = 0;
	uint32_t saved_digits = 0, saved_digit_record = 0, saved_digits_left = 0;
	uint8_t flip = 0;
	uint16_t closing_width = 0;

	OBJECT(current_object)->x = OBJECT(current_object)->time_digits.units_x;
	digit_record = current_object + 0x80u;
	digits_left = 3u;
	do
	{
		digit = digits & 0xFu;
		flip = mem_read8(digit_record);
		negative = flip >> 7;
		digit_record++;
		shown_digit = mem_read8(digit_record);
		digit_record++;
		flip_width = mem_read16(digit_record);
		digit_record += 6u;
		remaining_digits = digits;
		if (flip != 0u)
		{
			if (negative == 0u)
			{
				remaining_digits = mem_read32(digit_record - 4u);
				closing_width = flip_width;
				flip_width = (uint16_t)(closing_width - 0x40u);
				mem_write16(digit_record - 6u, flip_width);
				if (closing_width != 0x40u)
				{
					if (digits_left == 3u || remaining_digits != 0u)
					{
						OBJECT(current_object)->frame = shown_digit;
						OBJECT(current_object)->width_scale = flip_width;
						saved_digits = digits;
						saved_digit_record = digit_record;
						saved_digits_left = digits_left;
						draw_object(digits);
						digits_left = saved_digits_left;
						digit_record = saved_digit_record;
						digits = saved_digits;
					}
				}
				else
				{
					mem_write8(digit_record - 8u, 0xFFu);
					mem_write8(digit_record - 7u, digit);
					mem_write32(digit_record - 4u, digits);
				}
			}
			else
			{
				flip_width += 0x40u;
				mem_write16(digit_record - 6u, flip_width);
				if ((uint8_t)flip_width != 0u)
				{
					if (digits_left == 3u || remaining_digits != 0u)
					{
						OBJECT(current_object)->frame = shown_digit;
						OBJECT(current_object)->width_scale = flip_width;
						saved_digits = digits;
						saved_digit_record = digit_record;
						saved_digits_left = digits_left;
						draw_object(digits);
						digits_left = saved_digits_left;
						digit_record = saved_digit_record;
						digits = saved_digits;
					}
				}
				else
				{
					mem_write8(digit_record - 8u, 0u);
					if (digits_left == 3u || remaining_digits != 0u)
					{
						OBJECT(current_object)->frame = shown_digit;
						OBJECT(current_object)->width_scale = flip_width;
						saved_digits = digits;
						saved_digit_record = digit_record;
						saved_digits_left = digits_left;
						draw_object(digits);
						digits_left = saved_digits_left;
						digit_record = saved_digit_record;
						digits = saved_digits;
					}
				}
			}
		}
		else if (shown_digit == digit)
		{
			if (digits_left == 3u || remaining_digits != 0u)
			{
				OBJECT(current_object)->frame = shown_digit;
				OBJECT(current_object)->width_scale = flip_width;
				saved_digits = digits;
				saved_digit_record = digit_record;
				saved_digits_left = digits_left;
				draw_object(digits);
				digits_left = saved_digits_left;
				digit_record = saved_digit_record;
				digits = saved_digits;
			}
		}
		else
		{
			mem_write8(digit_record - 8u, 1u);
			remaining_digits = mem_read32(digit_record - 4u);
			closing_width = flip_width;
			flip_width = (uint16_t)(closing_width - 0x40u);
			mem_write16(digit_record - 6u, flip_width);
			if (closing_width != 0x40u)
			{
				if (digits_left == 3u || remaining_digits != 0u)
				{
					OBJECT(current_object)->frame = shown_digit;
					OBJECT(current_object)->width_scale = flip_width;
					saved_digits = digits;
					saved_digit_record = digit_record;
					saved_digits_left = digits_left;
					draw_object(digits);
					digits_left = saved_digits_left;
					digit_record = saved_digit_record;
					digits = saved_digits;
				}
			}
			else
			{
				mem_write8(digit_record - 8u, 0xFFu);
				mem_write8(digit_record - 7u, digit);
				mem_write32(digit_record - 4u, digits);
			}
		}
		digits = digits >> 4;
		OBJECT(current_object)->x -= 0x12u;
		digits_left--;
	} while (digits_left != 0u);
}

/* Starts the EXTEND TIME message once for each checkpoint passed (screen forks_decided, counted in car
   extensions_shown). */
void time_left_display_check_extension(void)
{
	uint32_t object = 0;

	if (CAR(player_car)->extensions_shown < SCREEN(screen)->forks_decided)
	{
		CAR(player_car)->extensions_shown = SCREEN(screen)->forks_decided;
		object = start_object();
		OBJECT(object)->handler = extend_time_message_start_ADDRESS;
	}
}

/* Sets up the EXTEND TIME message under the time (animation 33) for 90 frames, blinks one of the
   screen's cabinet lamps 16 times (bytes C1-C3 of its entry at checkpoint_lamp), then
   extend_time_message_blink. */
void extend_time_message_start(void)
{
	uint32_t animation = 0, index = 0, lamp = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Fu;
	animation = 0x33u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x3Fu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x30u;
	OBJECT(current_object)->extend_time_message.frames = 0x5Au;
	OBJECT(current_object)->extend_time_message.blink = 0u;
	OBJECT(current_object)->handler = extend_time_message_blink_ADDRESS;
	index = SCREEN(screen)->index;
	lamp = GAME_VARIABLES + index * 4u;
	mem_write8(lamp + 0xC3u, 1u);
	mem_write8(lamp + 0xC2u, 0x10u);
	mem_write8(lamp + 0xC1u, 0x10u);
	extend_time_message_blink(index);
}

/* Blinks EXTEND TIME (4 frames on, 4 off) until its frames run out. */
void extend_time_message_blink(uint32_t leftover)
{
	uint32_t blink = 0;
	uint8_t frames = 0;

	frames = OBJECT(current_object)->extend_time_message.frames;
	OBJECT(current_object)->extend_time_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	blink = (leftover & ~0xFFu) | OBJECT(current_object)->extend_time_message.blink;
	blink = (blink & ~0xFFu) | (uint8_t)(blink + 1u);
	OBJECT(current_object)->extend_time_message.blink = blink;
	if ((blink & 4u) != 0u)
		return;
	blink = draw_object(blink);
}

/* The start of a stage's road (position 0 of streams 1-30): when the car (car segment) reaches it, the
   stage the car is in becomes the one whose road is being built (screen current_stage_level = unk_A2,
   its level; unk_A6 = unk_A3, its number; car reached_stage_start = unk_48, the segment it starts at),
   and it ends. In the attract demo (screen demo_flags bit 8) it ends at once. */
void stage_entry_update(void)
{
	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	default:
		invalid_jump(stage_entry_state_jumps[OBJECT(current_object)->state]);
	}
	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->segment)
	{
		SCREEN(screen)->current_stage_level = SCREEN(screen)->built_stage_level;
		SCREEN(screen)->current_stage = SCREEN(screen)->built_stage;
		CAR(player_car)->reached_stage_start = CAR(player_car)->stage_start;
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
}

/* Unused: does nothing (a second return after stage_entry_update's). */
void stage_entry_spare_return(void)
{
}

/* The goal line and what follows it: state 0 waits until the screen's car reaches its segment
   (position), then marks the car at the goal (car at_goal); 1 when the car's finish is recorded (race
   finished) sets up Congratulations! (animation 2F5) and the TOTAL TIME (the text at D08E and the
   screen's finish time, screen 410, through time_to_text), DJ message 34 when first, else 35, the
   screen's sound byte to 7F; 2 waits; 3 after 120 frames draws them for 60, then the title Dream Flying
   (start_song_title) and sound 10; 4 240 more frames of both, or with two players 5 only the TOTAL TIME
   beside the race_result; then it ends. */
void goal_update(void)
{
	uint8_t dj_message = 0;
	uint32_t animation = 0, finish_time = 0, ignored_2 = 0, object = 0, result_object = 0, song_title = 0, sound_code = 0, text = 0;
	uint16_t timer = 0, title_timer = 0, result_timer = 0, show_timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	sound_code = goal_state_jumps[state];
	switch (state)
	{
	case 0:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->position)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->state++;
	case 1:
		if (RACE(SCREEN(screen)->race)->finished == 0u)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Cu;
		animation = 0x2F5u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x90u;
		object = current_object + 0x80u;
		OBJECT(object - 0x80u)->goal.text_x = 0xB0u;
		object += 2u;
		OBJECT(object - 0x82u)->goal.text_y = 0x30u;
		object += 2u;
		OBJECT(object - 0x84u)->goal.text_attribute = 0x90u;
		finish_time = mem_read16(screen + 0x410u + SCREEN(screen)->player * 2u);
		object += 2u;
		time_to_text(finish_time, object);
		dj_message = 0x34u;
		if (SCREEN(screen)->place != 0u)
			dj_message = 0x35u;
		start_dj_message(dj_message);
		sound_code = 0x7Fu;
		fade_out_music_at((uint8_t)sound_code);
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer - 1u;
		if (timer != 1u)
			return;
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer - 1u;
		if (timer != 1u)
			return;
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	case 4:
		title_timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = title_timer - 1u;
		if (title_timer == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(sound_code);
		text = total_time_text_ADDRESS;
		draw_text(text);
		text = current_object + 0x80u;
		ignored_2 = draw_text(text);
		return;
	case 5:
		result_timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = result_timer - 1u;
		if (result_timer != 1u)
		{
			text = total_time_text_ADDRESS;
			draw_text(text);
			text = current_object + 0x80u;
			ignored_2 = draw_text(text);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(goal_state_jumps[state]);
	}
	show_timer = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = show_timer - 1u;
	if (show_timer != 1u)
	{
		draw_object(sound_code);
		text = total_time_text_ADDRESS;
		draw_text(text);
		text = current_object + 0x80u;
		ignored_2 = draw_text(text);
		return;
	}
	song_title = dream_flying_song_title_ADDRESS;
	start_song_title(song_title);
	sound_code = (sound_code & ~0xFFu) | 0x10u;
	request_sound((uint8_t)sound_code);
	if (RACE(SCREEN(screen)->host_record)->racer_count != 1u)
	{
		result_object = start_object();
		OBJECT(result_object)->handler = race_result_start_ADDRESS;
		OBJECT(current_object)->state++;
	}
	OBJECT(current_object)->goal.timer = 0xF0u;
	OBJECT(current_object)->state++;
	title_timer = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = title_timer - 1u;
	if (title_timer == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(sound_code);
	text = total_time_text_ADDRESS;
	draw_text(text);
	text = current_object + 0x80u;
	ignored_2 = draw_text(text);
}

/* With two players, the result at the end of the race: the screen's place in big letters (animation 2F8:
   1st, 2nd) at the bottom for 240 frames (race_result_hold), and the verdict above it
   (race_result_verdict_start). */
void race_result_start(void)
{
	uint16_t palette = 0;
	uint32_t animation = 0, object = 0;
	uint8_t place = 0;

	object = start_object();
	OBJECT(object)->handler = race_result_verdict_start_ADDRESS;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation = 0x2F8u;
	animation_start(animation);
	place = ~~SCREEN(screen)->place;
	OBJECT(current_object)->frame = place;
	palette = 0x41u;
	if (place != 0u)
		palette = 0x42u;
	OBJECT(current_object)->palette = palette;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0xC8u;
	OBJECT(current_object)->race_result.frames = 0xF0u;
	OBJECT(current_object)->handler = race_result_hold_ADDRESS;
	race_result_hold();
}

/* Draws the place or Loser until its frames run out. */
void race_result_hold(void)
{
	uint32_t ignored = 0;
	uint16_t frames = 0;

	frames = OBJECT(current_object)->race_result.frames;
	OBJECT(current_object)->race_result.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	ignored = draw_object(0u);
}

/* The verdict over the place: Winner (animation 2F6), blinking (race_result_verdict_blink), for the
   first, Loser (2F7) for the other, 240 frames. */
void race_result_verdict_start(void)
{
	uint32_t animation = 0;
	uint8_t place = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	place = ~~SCREEN(screen)->place;
	OBJECT(current_object)->frame = place;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	if (place != 0u)
	{
		animation = 0x2F7u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x42u;
		OBJECT(current_object)->race_result.frames = 0xF0u;
		OBJECT(current_object)->handler = race_result_hold_ADDRESS;
		race_result_hold();
		return;
	}
	animation = 0x2F6u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x41u;
	OBJECT(current_object)->race_result.frames = 0xF0u;
	OBJECT(current_object)->handler = race_result_verdict_blink_ADDRESS;
	race_result_verdict_blink();
}

/* Blinks Winner (4 frames on, 4 off) until its frames run out. */
void race_result_verdict_blink(void)
{
	uint32_t ignored = 0;
	uint16_t frames = 0;

	frames = OBJECT(current_object)->race_result.frames;
	OBJECT(current_object)->race_result.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if ((OBJECT(current_object)->race_result.frames & 4u) == 0u)
		return;
	ignored = draw_object(0u);
}

/* The arrow of the way taken at a fork, at the bottom centre, with CHECK POINT scrolling on the LCD
   between arrows of that way: state 0 sets it up (animation 34 or 35 by the branch, car branch; DJ
   message 33 when first, else 3A, unless one is playing) for 57 segments of the race (leave_segment); 1
   cycles its colours (palette_list); 2 then grows and drops out of the screen. Ends when the race is
   over. */
void branch_arrow_update(void)
{
	uint8_t dj_message = 0;
	uint16_t new_scale = 0, new_y = 0;
	uint32_t animation = 0, lcd_text = 0, palette = 0, palette_list = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		dj_message = 0x3Au;
		if (SCREEN(screen)->place == 0u)
			dj_message = 0x33u;
		start_dj_message_if_quiet(dj_message);
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Du;
		lcd_text = check_point_lcd_text_branch_0_ADDRESS;
		animation = 0x34u;
		if (CAR(player_car)->branch != 0u)
		{
			lcd_text = check_point_lcd_text_branch_1_ADDRESS;
			animation = 0x35u;
		}
		SCREEN(screen)->lcd_scroll_request = lcd_text;
		animation_start(animation);
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD0u;
		OBJECT(current_object)->branch_arrow.palette_list = branch_arrow_palette_steps_ADDRESS;
		OBJECT(current_object)->branch_arrow.leave_segment = RACE(SCREEN(screen)->race_copy)->segment + 0x39u;
		OBJECT(current_object)->state++;
	case 1:
		if (CAR(player_car)->race_over != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->segment < (int16_t)OBJECT(current_object)->branch_arrow.leave_segment)
		{
			palette_list = OBJECT(current_object)->branch_arrow.palette_list;
			palette = OBJECT_PALETTE_STEPS(palette_list)[0];
			palette_list += 2u;
			if ((int16_t)palette < 0)
			{
				palette_list += palette;
				palette = mem_read16(palette_list);
				palette_list += 2u;
			}
			OBJECT(current_object)->palette = palette;
			OBJECT(current_object)->branch_arrow.palette_list = palette_list;
			palette = draw_object(palette);
			return;
		}
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(branch_arrow_state_jumps[OBJECT(current_object)->state]);
	}
	new_y = (uint16_t)(OBJECT(current_object)->y + 3u);
	if ((int16_t)new_y > 0x140)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->y = new_y;
	new_scale = (uint16_t)(OBJECT(current_object)->width_scale + 6u);
	OBJECT(current_object)->width_scale = new_scale;
	OBJECT(current_object)->height_scale = new_scale;
	palette_list = OBJECT(current_object)->branch_arrow.palette_list;
	palette = OBJECT_PALETTE_STEPS(palette_list)[0];
	palette_list += 2u;
	if ((int16_t)palette < 0)
	{
		palette_list += palette;
		palette = mem_read16(palette_list);
		palette_list += 2u;
	}
	OBJECT(current_object)->palette = palette;
	OBJECT(current_object)->branch_arrow.palette_list = palette_list;
	palette = draw_object(palette);
}

/* The times at a checkpoint, in rows under the top line: state 0 takes each player's car image
   (checkpoint_board_car_sprites by the car chosen); 1 waits until the screen's car passes a checkpoint
   (screen 480 + player); 2 then shows a row for each car that has passed it, in the order they did
   (screen 390): the first with its time, the others with how far behind the first they were (BEHIND);
   each row slides in from the right, the screen's own blinks while it waits for the others, and all
   slide out once the race has gone C0 segments on (leave_segment) or is over; when the last has gone,
   back to 1. */
void checkpoint_board_update(void)
{
	bool L_00CCDE = false, L_00CCFC = false;
	uint8_t checkpoints_passed = 0;
	uint16_t row_offset = 0;
	uint32_t blink_on = 0, checkpoint = 0, checkpoint_count = 0, leave_segment = 0, object = 0, player = 0, race = 0, racer = 0, racers_left = 0, row_player = 0, row_time = 0, screen_player = 0;
	uint8_t blink_timer = 0, rows_left = 0;
	uint16_t gap = 0, offset_before = 0;
	uint32_t jump_target = 0, started_step = 0, sliding_offset = 0, arrived_step = 0, leaving_step = 0, gone_step = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		race = SCREEN(screen)->host_record + 0x1Fu;
		racers_left = RACE(race - 0x1Fu)->racer_count;
		race++;
		do
		{
			racer = mem_read8(race);
			mem_write32(current_object + 0x80u + racer * 4u, checkpoint_board_car_sprites[mem_read8(0x20FC49u + (racer << 5) * 4u)]);
			race++;
			racers_left--;
		} while (racers_left != 0u);
		checkpoint_count = screen + 0x480u + SCREEN(screen)->player;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = mem_read8(checkpoint_count);
		OBJECT(current_object)->checkpoint_board.checkpoint_count = checkpoint_count;
		OBJECT(current_object)->checkpoint_board.rows_shown = 0u;
		OBJECT(current_object)->state++;
		checkpoints_passed = mem_read8(OBJECT(current_object)->checkpoint_board.checkpoint_count);
		if (OBJECT(current_object)->checkpoint_board.checkpoints_seen == checkpoints_passed)
			return;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = checkpoints_passed;
		OBJECT(current_object)->checkpoint_board.checkpoint = checkpoints_passed - 1u;
		OBJECT(current_object)->checkpoint_board.leave_segment = CAR(player_car)->segment + 0xC0u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		checkpoints_passed = mem_read8(OBJECT(current_object)->checkpoint_board.checkpoint_count);
		if (OBJECT(current_object)->checkpoint_board.checkpoints_seen == checkpoints_passed)
			return;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = checkpoints_passed;
		OBJECT(current_object)->checkpoint_board.checkpoint = checkpoints_passed - 1u;
		OBJECT(current_object)->checkpoint_board.leave_segment = CAR(player_car)->segment + 0xC0u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(checkpoint_board_state_jumps[OBJECT(current_object)->state]);
	}
	checkpoint = OBJECT(current_object)->checkpoint_board.checkpoint;
	OBJECT(current_object)->checkpoint_board.row = 0u;
	OBJECT(current_object)->checkpoint_board.rows_left = mem_read8(screen + 0x35Au + checkpoint);
	OBJECT(current_object)->checkpoint_board.passing_order = screen + 0x390u + checkpoint * 2u * 4u;
	do
	{
		row_player = mem_read8(OBJECT(current_object)->checkpoint_board.passing_order);
		do
		{
			do
			{
				jump_target = L_00CCFC ? 0xCCECu : L_00CCFC ? 0xCCECu : checkpoint_board_row_state_jumps[mem_read8(current_object + 0xF0u + row_player)];
				switch (jump_target)
				{
				case 0x00CC84u:
					OBJECT(current_object)->checkpoint_board.rows_shown++;
					row_time = mem_read16(screen + 0x3C0u + (((uint32_t)OBJECT(current_object)->checkpoint_board.checkpoint << 3) + row_player) * 2u);
					player = row_player;
					if (OBJECT(current_object)->checkpoint_board.row != 0u)
					{
						gap = row_time - OBJECT(current_object)->checkpoint_board.leader_time;
						row_time = gap;
						if ((int16_t)gap < 0)
							row_time = 0u;
						checkpoint_board_gap_time(row_time, player);
					}
					else
					{
						OBJECT(current_object)->checkpoint_board.leader_time = row_time;
						checkpoint_board_leader_time(row_time, player);
					}
					screen_player = SCREEN(screen)->player;
					if (screen_player != row_player)
					{
						L_00CCDE = mem_read8(current_object + 0xF0u + screen_player) != 0u;
						if (!L_00CCDE)
						{
							mem_write16(current_object + 0xA0u + row_player * 2u, 0u);
							mem_write8(current_object + 0xF0u + row_player, 2u);
							L_00CCFC = true;
						}
					}
					if (!L_00CCDE)
					{
						if (L_00CCFC)
							break;
						OBJECT(current_object)->checkpoint_board.blink_on = 4u;
						OBJECT(current_object)->checkpoint_board.blink_timer = 8u;
					}
					L_00CCDE = false;
					mem_write16(current_object + 0xA0u + row_player * 2u, 0x100u);
					started_step = current_object + 0xF0u + row_player;
					mem_write8(started_step, mem_read8(started_step) + 1u);
				case 0x00CCECu:
					if (!L_00CCFC)
					{
						if (!L_00CCFC)
						{
							sliding_offset = current_object + 0xA0u + row_player * 2u;
							offset_before = mem_read16(sliding_offset);
							mem_write16(sliding_offset, offset_before - 0x10u);
						}
					}
					if (!L_00CCFC && !L_00CCFC && offset_before != 0x10u)
						checkpoint_board_draw_row(row_player);
					else
					{
						if (!L_00CCFC)
						{
							if (!L_00CCFC)
							{
								arrived_step = current_object + 0xF0u + row_player;
								mem_write8(arrived_step, mem_read8(arrived_step) + 1u);
							}
						}
						L_00CCFC = false;
						if (OBJECT(current_object)->checkpoint_board.leave_segment == 0u || CAR(player_car)->race_over != 0u)
						{
							leaving_step = current_object + 0xF0u + row_player;
							mem_write8(leaving_step, mem_read8(leaving_step) + 1u);
							row_offset = (uint16_t)(mem_read16(current_object + 0xA0u + row_player * 2u) - 0x10u);
							mem_write16(current_object + 0xA0u + row_player * 2u, row_offset);
							if (row_offset != 0xFF00u)
								checkpoint_board_draw_row(row_player);
							else
							{
								OBJECT(current_object)->checkpoint_board.rows_shown--;
								gone_step = current_object + 0xF0u + row_player;
								mem_write8(gone_step, mem_read8(gone_step) + 1u);
								checkpoint_board_draw_row(row_player);
							}
						}
						else if (SCREEN(screen)->player != row_player)
							checkpoint_board_draw_row(row_player);
						else
						{
							blink_on = OBJECT(current_object)->checkpoint_board.blink_on;
							blink_timer = OBJECT(current_object)->checkpoint_board.blink_timer;
							OBJECT(current_object)->checkpoint_board.blink_timer = blink_timer - 1u;
							if (blink_timer == 1u)
							{
								blink_on = blink_on ^ 4u;
								OBJECT(current_object)->checkpoint_board.blink_on = blink_on;
								OBJECT(current_object)->checkpoint_board.blink_timer = 4u + blink_on;
							}
							if (blink_on != 0u)
								checkpoint_board_draw_row(row_player);
						}
					}
					break;
				case 0x00CCFCu:
					L_00CCFC = true;
					break;
				case 0x00CD32u:
					row_offset = (uint16_t)(mem_read16(current_object + 0xA0u + row_player * 2u) - 0x10u);
					mem_write16(current_object + 0xA0u + row_player * 2u, row_offset);
					if (row_offset == 0xFF00u)
					{
						OBJECT(current_object)->checkpoint_board.rows_shown--;
						gone_step = current_object + 0xF0u + row_player;
						mem_write8(gone_step, mem_read8(gone_step) + 1u);
					}
					checkpoint_board_draw_row(row_player);
					break;
				case 0x00CD55u:
					break;
				default:
					invalid_jump(jump_target);
				}
			} while (L_00CCFC);
		} while (L_00CCFC);
		OBJECT(current_object)->checkpoint_board.passing_order++;
		OBJECT(current_object)->checkpoint_board.row++;
		rows_left = OBJECT(current_object)->checkpoint_board.rows_left;
		OBJECT(current_object)->checkpoint_board.rows_left = rows_left - 1u;
	} while (rows_left != 1u);
	leave_segment = OBJECT(current_object)->checkpoint_board.leave_segment;
	if (leave_segment != 0u && (int16_t)leave_segment <= (int16_t)RACE(SCREEN(screen)->race_copy)->segment)
		OBJECT(current_object)->checkpoint_board.leave_segment = 0u;
	if (OBJECT(current_object)->checkpoint_board.rows_shown == 0u)
	{
		object = current_object + 0xF0u;
		AS32(OBJECT(object - 0xF0u)->checkpoint_board.row_steps) = 0u;
		OBJECT(object - 0xF0u)->unk_F4 = 0u;
		OBJECT(object - 0xF0u)->unk_F6 = 0u;
		OBJECT(object - 0xF0u)->unk_F7 = 0u;
		OBJECT(current_object)->state--;
	}
}

/* Draws row unk_12 of the board for player R21, slid by its offset: the car number, the car's small
   image, the time text (draw_text_at; another colour after the first row) and, after the first, BEHIND. */
void checkpoint_board_draw_row(uint32_t player)
{
	uint16_t text_attribute = 0;
	uint32_t animation = 0, car_sprite = 0, place = 0, row = 0, row_x = 0, text_x = 0, time_text = 0;

	place = checkpoint_board_number_places_ADDRESS + OBJECT(current_object)->checkpoint_board.row * 4u;
	row_x = (uint16_t)(CHECKPOINT_BOARD_PLACE(place)->x + mem_read16(current_object + 0xA0u + player * 2u));
	OBJECT(current_object)->x = row_x;
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	animation = checkpoint_board_number_animations[mem_read8(car_numbers_ADDRESS + player)];
	OBJECT(current_object)->palette = 0x27u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(animation);
	draw_object(row_x);
	place = checkpoint_board_car_places_ADDRESS + OBJECT(current_object)->checkpoint_board.row * 4u;
	row_x = (uint16_t)(CHECKPOINT_BOARD_PLACE(place)->x + mem_read16(current_object + 0xA0u + player * 2u));
	OBJECT(current_object)->x = row_x;
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	car_sprite = current_object + 0x80u + player * 4u;
	animation = mem_read16(car_sprite);
	OBJECT(current_object)->palette = mem_read16(car_sprite + 2u);
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->draw_mode = 0u;
	animation_start(animation);
	OBJECT(current_object)->frame = 4u;
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	draw_object(row_x);
	row = OBJECT(current_object)->checkpoint_board.row;
	text_x = checkpoint_board_time_places[row];
	text_x = (text_x & ~0xFFFFu) | (uint16_t)(text_x + mem_read16(current_object + 0xA0u + player * 2u));
	text_attribute = 0xC0u;
	if (row != 0u)
		text_attribute = 0x90u;
	time_text = current_object + 0xB0u + player * 2u * 4u;
	draw_text_at(text_x, text_attribute, time_text);
	row = OBJECT(current_object)->checkpoint_board.row;
	if (row == 0u)
		return;
	place = checkpoint_board_behind_places_ADDRESS + row * 4u;
	row_x = (uint16_t)(CHECKPOINT_BOARD_PLACE(place)->x + mem_read16(current_object + 0xA0u + player * 2u));
	OBJECT(current_object)->x = row_x;
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	animation = 0x36u;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(animation);
	row_x = draw_object(row_x);
}

/* Writes the first car's time R0 as M'SS"HH into player R1's row text. */
void checkpoint_board_leader_time(uint32_t time, uint32_t player)
{
	uint32_t time_text = 0;

	time_text = current_object + 0xB0u + player * 2u * 4u;
	time_to_text(time, time_text);
}

/* Unused: time_to_text for a time in thousandths of a second: R0 / 10 (into hundredths), then
   time_to_text at R10 (it runs on into it). */
void time_to_text_thousandths(uint32_t time, uint32_t text)
{
	time = time / 0xAu;
	time_to_text(time, text);
}

/* Writes the time R0 (hundredths of a second) as M'SS"HH, closed by 0, at R10. Used by lap_timer, goal,
   checkpoint_board, 001EC2 and 00270C. */
void time_to_text(uint32_t time, uint32_t text)
{
	uint32_t digit = 0;

	text += 7u;
	mem_write8(text, 0u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	text--;
	mem_write8(text, 0x22u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	digit = 0u;
	divx_pair(6u, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	text--;
	mem_write8(text, 0x27u);
	mem_write8(text - 1u, time + 0x30u);
}

/* Writes how far behind the first player R1 passed (R0, hundredths) into its row text, as time_to_text
   does but with spaces for zero minutes and zero tens of seconds. */
void checkpoint_board_gap_time(uint32_t time, uint32_t player)
{
	uint32_t zero = 0;
	uint8_t zero_char = 0;
	uint32_t digit = 0, text = 0;

	text = current_object + 0xB7u + player * 2u * 4u;
	zero_char = 0x30u;
	mem_write8(text, 0u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	mem_write8(text, digit + 0x30u);
	text--;
	mem_write8(text, 0x22u);
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	zero = (uint32_t)(time == 0u);
	if (zero == 0u)
	{
		text--;
		mem_write8(text, digit + zero_char);
		digit = 0u;
		divx_pair(6u, &time, &digit);
		zero = (uint32_t)(time == 0u);
		if (zero)
		{
			text--;
			mem_write8(text, digit + zero_char);
			mem_write16(text - 2u, 0x2020u);
			return;
		}
		text--;
		mem_write8(text, digit + zero_char);
		text--;
		mem_write8(text, 0x27u);
		mem_write8(text - 1u, time + zero_char);
		return;
	}
	text--;
	mem_write8(text, digit + zero_char);
	text--;
	mem_write8(text, 0x20u);
	mem_write16(text - 2u, 0x2020u);
}

/* The network check after the test mode: network_check, or in ISDN mode (isdn_mode) the link board off
   for a moment, the link's variables (from machine_count), the board's memory at 800004 and 800600, this
   machine's two race records and all the players' records cleared, then network_check_isdn. */
void network_check_again(void)
{
	uint32_t delay = 0;

	if (isdn_mode == 0u)
	{
		network_check();
		return;
	}
	mem_write16(0x801000u, 0u);
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
	move_string(zero_block_ADDRESS, 0x200u, RACE_COPY_1, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
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
	uint32_t board_reply = 0, ignored = 0, left_caller = 0, text = 0;
	uint8_t board_status = 0;
	uint32_t board_flags_greeted = 0, board_flags_role = 0, board_flags_status = 0;

	mem_write16(0x801000u, 0u);
	r12 = 0x800000u;
	move_halfwords(machine_count_ADDRESS, zero_block_ADDRESS, 0x20u);
	move_halfwords(r12, zero_block_ADDRESS, 8u);
	move_halfwords(RACE_1, zero_block_ADDRESS, 0x80u);
	move_halfwords(RACE_COPY_1, zero_block_ADDRESS, 0x200u);
	link_off = stand_alone;
	if (link_off != 0u)
	{
		network_check_stand_alone();
		return;
	}
	mem_write8(0x801000u, 0u);
	text = network_check_master_text_ADDRESS;
	if (link_master == 0u)
		text = network_check_slave_text_ADDRESS;
	if (isdn_mode != 0u)
		text = network_check_isdn_text_ADDRESS;
	print_text_layer(text);
	text = board_check_text_ADDRESS;
	print_text_layer(text);
	left_caller = 0u;
	left_caller = network_check_board_memory(left_caller);
	if (left_caller != 0u)
		return;
	mem_write16(r12, 0x56u);
	mem_write16(r12 + 2u, 0x37u);
	mem_write16(r12 + 4u, 0x30u);
	mem_write8(r12 + 0x14u, 0u);
	mem_write16(0x801002u, 0u);
	mem_write16(0x801000u, 1u);
	left_caller = 0u;
	network_check_wait_board(&ignored, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	network_check_greeting(&board_reply, &left_caller);
	if (left_caller != 0u)
		return;
	board_flags_greeted = mem_read32(0x801002u);
	mem_write32(0x801002u, board_flags_greeted | 1u);
	board_reply = wait_for_frame(board_reply);
	left_caller = 0u;
	network_check_wait_board(&board_reply, &left_caller);
	if (left_caller != 0u)
		return;
	mem_write8(r12 + 0xCu, 0u);
	left_caller = 0u;
	network_check_four_ics(&board_reply, &left_caller);
	if (left_caller != 0u)
		return;
	if (isdn_mode != 0u)
	{
		network_check_isdn();
		return;
	}
	mem_write8(r12 + 4u, link_master);
	board_flags_role = mem_read32(0x801002u);
	mem_write32(0x801002u, board_flags_role & ~1u);
	board_reply = wait_for_frame(board_reply);
	left_caller = 0u;
	network_check_wait_board(&board_reply, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	left_caller = network_check_five_ics(board_reply, left_caller);
	if (left_caller != 0u)
		return;
	board_flags_status = mem_read32(0x801002u);
	mem_write32(0x801002u, board_flags_status | 1u);
	do
	{
		board_reply = wait_for_frame(board_reply);
		if (test_mode_on != 0u)
		{
			link_failed = 0xFFu;
			return;
		}
		current_object = check_network_status_text_ADDRESS;
		network_check_blink_text();
		board_status = mem_read8(0x801002u);
	} while ((int8_t)board_status >= 0);
	network_check_clear_status_line();
	mem_write8(r12 + 0xE20u, mem_read8(r12 + 2u));
	mem_write8(r12 + 6u, 0xFFu);
	text = check_trans_end_text_ADDRESS;
	print_text_layer(text);
	do
	{
		if (test_mode_on != 0u)
		{
			link_failed = 0xFFu;
			return;
		}
		if (mem_read8(r12 + 0xCu) != 0u)
		{
			link_lost();
			return;
		}
	} while (mem_read8(r12 + 8u) == 0u);
	network_check_clear_status_line();
	link_wait_frames = 0u;
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

	count = mem_read8(r12);
	machine_count = count;
	count = (uint8_t)(count * 2u);
	player_count = count;
	machine = mem_read8(r12 + 2u);
	mem_write8(r12 + 0x1002u, 1u);
	if (isdn_mode == 0u && count < 2u)
	{
		network_check_stand_alone();
		return;
	}
	car_number = (uint8_t)(cabinet_id * 2u);
	SCREEN(SCREEN_1)->car_number = car_number;
	RACE(RACE_1)->car_number = car_number;
	car_number++;
	SCREEN(SCREEN_2)->car_number = car_number;
	RACE(RACE_2)->car_number = car_number;
	network_check_result(machine);
}

/* The network check's ISDN mode: the link board off for a moment, 1 into its byte at 800014 (0 in
   network mode), its flag clear, the board on, network_check_wait_isdn, then network_check_join. */
void network_check_isdn(void)
{
	uint32_t delay = 0, left_caller = 0;

	mem_write16(0x801000u, 0u);
	delay = BOARD_OFF_LOOPS;
	do
	{
		delay--;
	} while (delay != 0u);
	mem_write8(r12 + 0x14u, 1u);
	mem_write8(0x801002u, 0u);
	mem_write8(0x801000u, 1u);
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
	link_failed = 1u;
	network_check_stand_alone();
}

/* Sets the machine up alone: link_off, one machine and two players (machine_count, player_count), car
   numbers 0 and 1 for the two screens (screen and race car_number), then network_check_result as machine
   1. */
void network_check_stand_alone(void)
{
	uint32_t machine = 0;

	link_off = 1u;
	machine = 1u;
	player_count = 2u;
	machine_count = 1u;
	SCREEN(SCREEN_1)->car_number = 0u;
	RACE(RACE_1)->car_number = 0u;
	SCREEN(SCREEN_2)->car_number = 1u;
	RACE(RACE_2)->car_number = 1u;
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
	uint32_t frames_left = 0, ignored = 0, race = 0, second_player = 0, text = 0;
	uint32_t saved_frames_left = 0;
	uint8_t isdn_machine_id = 0, board_master = 0, slave_machine_id = 0, master_machine_id = 0;

	RACE(RACE_1)->machine = machine;
	RACE(RACE_2)->machine = machine;
	machine = (machine & ~0xFFu) | (uint8_t)(machine - 1u);
	machine = (machine & ~0xFFu) | (uint8_t)(machine * 2u);
	RACE(RACE_1)->player = machine;
	second_player = machine + 1u;
	RACE(RACE_2)->player = second_player;
	SCREEN(SCREEN_1)->player = machine;
	SCREEN(SCREEN_2)->player = second_player;
	SCREEN(SCREEN_1)->race_copy = race_copy_offsets[machine] + GAME_VARIABLES;
	SCREEN(SCREEN_2)->race_copy = race_copy_offsets[second_player] + GAME_VARIABLES;
	race = RACE_1;
	SCREEN(SCREEN_1)->race = RACE_1;
	SCREEN(SCREEN_2)->race = RACE_2;
	if ((uint8_t)machine == 0u)
	{
		RACE(race)->master_advertise_sound = advertise_sound;
		RACE(race)->master_steering_wheel_reaction = steering_wheel_reaction;
		RACE(race)->master_coins_per_credit = coins_per_credit;
		RACE(race)->master_first_chute_multiplier = first_chute_multiplier;
		RACE(race)->master_bonus_adder = bonus_adder;
		RACE(race)->master_coin_setting = coin_setting;
		RACE(race)->master_free_play = free_play;
		RACE(race)->master_game_difficulty = game_difficulty;
		RACE(race)->master_time_difficulty_0 = time_difficulty[0];
		RACE(race)->unk_0C = time_difficulty[1];
		RACE(race)->master_time_difficulty_2_5 = ((uint8_t)time_difficulty[2] | ((uint32_t)(uint8_t)time_difficulty[3] << 8u) | ((uint32_t)(uint8_t)time_difficulty[4] << 16u) | ((uint32_t)(uint8_t)time_difficulty[5] << 24u));
		move_halfwords(race + 0x30u, SOUND_RAM + 0x1F32u, 0x1Eu);
	}
	clear_text_layer();
	text = network_check_text_ADDRESS;
	print_text_layer(text);
	if (link_failed != 0u)
	{
		text = communication_failed_text_ADDRESS;
		print_text_layer(text);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			ignored = wait_for_frame(ignored);
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	if (isdn_mode != 0u)
	{
		text = isdn_machine_text_ADDRESS;
		print_text_layer(text);
		text = communication_success_text_ADDRESS;
		print_text_layer(text);
		text = machine_id_text_ADDRESS;
		print_text_layer(text);
		isdn_machine_id = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (isdn_machine_id + 0x30u) | 0x6000u);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			ignored = wait_for_frame(ignored);
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	if (link_off != 0u)
	{
		text = stand_alone_machine_text_ADDRESS;
		print_text_layer(text);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			ignored = wait_for_frame(ignored);
			frames_left = saved_frames_left - 1u;
		} while (frames_left != 0u);
		clear_text_layer();
		return;
	}
	board_master = mem_read8(0x800004u);
	if (board_master == 0u)
	{
		text = slave_machine_text_ADDRESS;
		print_text_layer(text);
		text = communication_success_text_ADDRESS;
		print_text_layer(text);
		text = machine_id_text_ADDRESS;
		print_text_layer(text);
		slave_machine_id = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (slave_machine_id + 0x30u) | 0x6000u);
		frames_left = 0x80u;
		do
		{
			saved_frames_left = frames_left;
			ignored = wait_for_frame(ignored);
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
	master_machine_id = mem_read8(0x800002u);
	mem_write16(VIDEO_31F736, (master_machine_id + 0x30u) | 0x6000u);
	frames_left = 0x80u;
	do
	{
		saved_frames_left = frames_left;
		ignored = wait_for_frame(ignored);
		frames_left = saved_frames_left - 1u;
	} while (frames_left != 0u);
	clear_text_layer();
}

/* Tests the link board's memory (network_check_memory_test over 7FF of its bytes); on a fault the
   screens off, CONNECTION ERROR and network_check_error. */
uint32_t network_check_board_memory(uint32_t left_caller)
{
	uint8_t outcome = 0;
	uint32_t board_memory = 0, count_then_faults = 0, error_texts = 0, ignored = 0, left_caller_2 = 0;

	board_memory = 0x800000u;
	count_then_faults = 0x7FFu;
	count_then_faults = network_check_memory_test(count_then_faults, board_memory);
	outcome = 2u;
	if ((uint8_t)count_then_faults != 0u)
		outcome = 3u;
	if (outcome == 3u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		error_texts = connection_error_texts_ADDRESS;
		network_check_error(error_texts, &ignored, &left_caller_2);
		return left_caller_2;
	}
	return left_caller;
}

/* A memory test: eight passes (seed 01, 02, ... 80), each writing a pattern from the seed into R1 bytes
   (one a halfword) from R10, reading them back and gathering the bits that differ into R0's low byte (0:
   no fault); CHECK DUALPORT RAM flips on the status line after each pass
   (network_check_toggle_memory_text). */
uint32_t network_check_memory_test(uint32_t count_then_faults, uint32_t memory)
{
	uint32_t carry = 0;
	uint8_t pattern = 0, rotation = 0, seed = 0;
	uint16_t address_bits = 0;
	uint32_t byte_count = 0, bytes_left = 0, cell = 0;
	uint8_t byte_read = 0;
	uint16_t written_low_15_bits = 0, written_low_12_bits = 0, written_low_9_bits = 0, written_low_6_bits = 0, read_low_15_bits = 0, read_low_12_bits = 0, read_low_9_bits = 0, read_low_6_bits = 0;

	byte_count = count_then_faults;
	seed = 1u;
	count_then_faults &= ~0xFFu;
	do
	{
		cell = memory;
		pattern = (uint8_t)seed;
		bytes_left = byte_count;
		do
		{
			mem_write16(cell, pattern);
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
			byte_read = mem_read8(cell);
			cell++;
			if (byte_read != pattern)
			{
				count_then_faults = (count_then_faults & ~0xFFu) | (uint8_t)(count_then_faults ^ pattern);
				count_then_faults = (count_then_faults & ~0xFFu) | (uint8_t)(count_then_faults ^ mem_read8(cell - 1u));
			}
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
	return count_then_faults;
}

/* Waits for the link board's flag (801002 bit 7), WAIT blinking, for up to E10 frames (two minutes,
   link_wait_frames): then the screens off, COMMUNICATION TIME OUT and network_check_error; the test
   button gives the check up (link_failed FF); the status line cleared after. */
void network_check_wait_board(uint32_t *p_board_reply, uint32_t *p_left_caller)
{
	uint32_t error_texts = 0;
	uint16_t frames_left = 0;
	uint32_t board_flags = 0;

	link_wait_frames = 0xE10u;
	do
	{
		*p_board_reply = wait_for_frame(*p_board_reply);
		if (test_mode_on != 0u)
		{
			link_failed = 0xFFu;
			*p_left_caller = 1u;
			return;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		frames_left = link_wait_frames;
		link_wait_frames = frames_left - 1u;
		if (frames_left == 1u)
		{
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			error_texts = communication_time_out_texts_ADDRESS;
			network_check_error(error_texts, p_board_reply, p_left_caller);
			return;
		}
		board_flags = mem_read32(0x801002u);
	} while ((board_flags & 0x80u) == 0u);
	network_check_clear_status_line();
}

/* ISDN mode: waits, WAIT blinking, until the link board's byte 800000 (its machine count) is set, then
   60 frames more (link_wait_frames); the test button gives the check up (link_failed FF); the status
   line cleared after. */
uint32_t network_check_wait_isdn(uint32_t left_caller)
{
	uint32_t ignored = 0, left_caller_2 = 0;
	uint8_t board_machines = 0;
	uint16_t frames_left = 0;

	do
	{
		ignored = wait_for_frame(ignored);
		if (test_mode_on != 0u)
		{
			link_failed = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		board_machines = mem_read8(0x800000u);
	} while (board_machines == 0u);
	link_wait_frames = 0x3Cu;
	do
	{
		ignored = wait_for_frame(ignored);
		if (test_mode_on != 0u)
		{
			link_failed = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = wait_text_ADDRESS;
		network_check_blink_text();
		frames_left = link_wait_frames;
		link_wait_frames = frames_left - 1u;
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
void network_check_greeting(uint32_t *p_board_reply, uint32_t *p_left_caller)
{
	uint32_t board_answer = 0, error_texts = 0;

	board_answer = 0x10u + r12;
	*p_board_reply = ((((uint32_t)(uint8_t)mem_read16(board_answer) << 8) | (uint8_t)mem_read16(board_answer + 2u)) << 8) | (uint8_t)mem_read16(board_answer + 4u);
	if ((((((uint32_t)(uint8_t)mem_read16(r12) << 8) | (uint8_t)mem_read16(r12 + 2u)) << 8) | (uint8_t)mem_read16(r12 + 4u)) != 0x563730u || *p_board_reply != 0x5A3830u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		error_texts = connection_error_texts_ADDRESS;
		network_check_error(error_texts, p_board_reply, p_left_caller);
		return;
	}
}

/* The link board's self-test (its byte 80000C), the second time: clear, IC17, IC23, IC11, IC10 and IC9
   GOOD (network_check_show_ics); else the screens off, COMMUNICATION BOARD ERROR and
   network_check_four_ics, which, the byte still set, shows four of the ICs and halts there (the five
   after it only show if the byte clears meanwhile), then network_check_halt. */
uint32_t network_check_five_ics(uint32_t board_reply, uint32_t left_caller)
{
	uint32_t error_texts = 0, ic_count = 0, ignored = 0, left_caller_2 = 0, self_test = 0;

	self_test = (board_reply & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)self_test == 0u)
	{
		ic_count = 5u;
		network_check_show_ics(ic_count, self_test);
		return left_caller;
	}
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
	clear_text_layer();
	error_texts = communication_board_error_texts_ADDRESS;
	print_text_layer_list(error_texts);
	left_caller_2 = 0u;
	network_check_four_ics(&self_test, &left_caller_2);
	if (left_caller_2 != 0u)
		return left_caller_2;
	ic_count = 5u;
	self_test = (self_test & ~0xFFu) | mem_read8(r12 + 0xCu);
	network_check_show_ics(ic_count, self_test);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	network_check_halt(&ignored, &left_caller_2);
	return left_caller_2;
}

/* The link board's self-test (its byte 80000C), the first time: clear, IC17, IC23, IC11 and IC10 GOOD
   (network_check_show_ics); else the screens off, COMMUNICATION BOARD ERROR with each of the four GOOD
   or BAD, the screens on, and network_check_halt. */
void network_check_four_ics(uint32_t *p_board_reply, uint32_t *p_left_caller)
{
	uint32_t board_reply = *p_board_reply;
	uint32_t error_texts = 0, ic_count = 0, self_test = 0;

	self_test = (board_reply & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)self_test != 0u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		clear_text_layer();
		error_texts = communication_board_error_texts_ADDRESS;
		print_text_layer_list(error_texts);
		self_test = (self_test & ~0xFFu) | mem_read8(r12 + 0xCu);
		ic_count = 4u;
		network_check_show_ics(ic_count, self_test);
		mem_write8(IO_C0001C, 6u);
		mem_write8(0xC8001Cu, 0x86u);
		network_check_halt(&self_test, p_left_caller);
	}
	else
	{
		ic_count = 4u;
		network_check_show_ics(ic_count, self_test);
	}
	*p_board_reply = self_test;
}

/* Shows R0 of the link board's ICs (IC17, IC23, IC11, IC10, IC9), each GOOD or BAD by its bit in R2
   (link_board_ic_texts). */
void network_check_show_ics(uint32_t ic_count, uint32_t self_test)
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

	network_check_blink_counter++;
	if ((network_check_blink_counter & 0xFu) != 0u)
		return;
	cell = 0x310000u | 0xF000u | (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
	current_object = current_object + 2u;
	character = (uint16_t)((uint32_t)(uint8_t)OBJECT(current_object - 0x2u)->unk_02 << 8);
	current_object = current_object + 1u;
	for (;;)
	{
		character = (uint16_t)((character & ~0xFFu) | mem_read8(current_object));
		current_object = current_object + 1u;
		if ((uint8_t)character == 0u)
			break;
		mem_write16(cell, mem_read16(cell) ^ character);
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
		mem_write16(cell, 0u);
		cells_left--;
		cell += 2u;
	} while (cells_left != 0u);
}

/* Flips CHECK DUALPORT RAM on or off on the status line (row 24), once each pass of
   network_check_memory_test. */
void network_check_toggle_memory_text(void)
{
	uint16_t character = 0;
	uint32_t cell = 0;

	character = 0x6000u;
	r27 = TEXT_CELL(8, 24);
	r28 = check_dualport_ram_characters_ADDRESS;
	do
	{
		character = (uint16_t)((character & ~0xFFu) | TEXT_CHARACTERS(r28)[0]);
		cell = r27;
		r27 = r27 + 2u;
		mem_write16(cell, mem_read16(cell) ^ character);
		r28 = r28 + 1u;
	} while (TEXT_CHARACTERS(r28)[0] != 0u);
}

/* Shows the error R19 (a text list: CONNECTION ERROR, COMMUNICATION TIME OUT...) on a cleared text layer
   with the screens on, then halts like network_check_halt. */
void network_check_error(uint32_t error_texts, uint32_t *p_board_reply, uint32_t *p_left_caller)
{
	clear_text_layer();
	print_text_layer_list(error_texts);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	for (;;)
	{
		*p_board_reply = wait_for_frame(*p_board_reply);
		if (test_mode_on != 0u)
			break;
		current_object = network_error_text_ADDRESS;
		network_check_blink_text();
	}
	link_failed = 0xFFu;
	*p_left_caller = 1u;
}

/* Blinks NETWORK ERROR until the test button (test_mode_on) is pressed; then link_failed FF and the
   network check is given up (it returns past its caller). */
void network_check_halt(uint32_t *p_board_reply, uint32_t *p_left_caller)
{
	for (;;)
	{
		*p_board_reply = wait_for_frame(*p_board_reply);
		if (test_mode_on != 0u)
			break;
		current_object = network_error_text_ADDRESS;
		network_check_blink_text();
	}
	link_failed = 0xFFu;
	*p_left_caller = 1u;
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
	uint32_t car_number_slot = 0, coins_changed = 0, ignored = 0, players_left = 0, race = 0, record_car_number = 0;
	uint8_t isdn_records_ready = 0;
	uint32_t time_difficulty_2_long = 0;

	if (test_mode_on != 0u)
		return;
	link_send_records();
	ignored = wait_for_frame(0u);
	ignored = wait_for_frame(ignored);
	ignored = wait_for_frame(ignored);
	ignored = wait_for_frame(ignored);
	ignored = wait_for_frame(ignored);
	for (;;)
	{
		ignored = wait_for_frame(ignored);
		if (isdn_mode == 0u)
			break;
		isdn_records_ready = mem_read8(0x8006E1u);
		if (isdn_records_ready != 0u)
		{
			link_isdn_receive_records();
			break;
		}
	}
	link_receive_records();
	record_car_number = 0x20FC02u;
	car_number_slot = car_numbers_ADDRESS;
	players_left = player_count;
	record_size = 0x80u;
	do
	{
		mem_write8(car_number_slot, mem_read8(record_car_number));
		record_car_number = (record_car_number & ~0xFFFFu) | (uint16_t)(record_car_number + record_size);
		car_number_slot++;
		players_left--;
	} while (players_left != 0u);
	move_halfwords(SOUND_RAM + 0x1F32u, WORK_RAM + 0xFC30u, 0x1Eu);
	bookkeeping_update_checksum(players_left);
	move_string(0x20FC30u, 0x1Eu, SOUND_RAM + 0x1FAEu, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	load_course_records();
	race = RACE_COPY_1;
	coins_changed = 0u;
	if (coin_setting != RACE(RACE_COPY_1)->master_coin_setting || coins_per_credit != RACE(race)->master_coins_per_credit || first_chute_multiplier != RACE(race)->master_first_chute_multiplier || bonus_adder != RACE(race)->master_bonus_adder || free_play != RACE(race)->master_free_play)
	{
		coins_changed = 0xFFFFFFFFu;
		coin_setting = RACE(race)->master_coin_setting;
		coins_per_credit = RACE(race)->master_coins_per_credit;
		first_chute_multiplier = RACE(race)->master_first_chute_multiplier;
		bonus_adder = RACE(race)->master_bonus_adder;
		free_play = RACE(race)->master_free_play;
		init_coins();
		bookkeeping_clear_credits();
	}
	if (advertise_sound == RACE(race)->master_advertise_sound && steering_wheel_reaction == RACE(race)->master_steering_wheel_reaction && game_difficulty == RACE(race)->master_game_difficulty && ((uint8_t)time_difficulty[0] | ((uint32_t)(uint8_t)time_difficulty[1] << 8u)) == ((uint8_t)RACE(race)->master_time_difficulty_0 | ((uint32_t)RACE(race)->unk_0C << 8u)) && ((uint8_t)time_difficulty[2] | ((uint32_t)(uint8_t)time_difficulty[3] << 8u) | ((uint32_t)(uint8_t)time_difficulty[4] << 16u) | ((uint32_t)(uint8_t)time_difficulty[5] << 24u)) == RACE(race)->master_time_difficulty_2_5 && coins_changed == 0u)
		return;
	advertise_sound = RACE(race)->master_advertise_sound;
	steering_wheel_reaction = RACE(race)->master_steering_wheel_reaction;
	game_difficulty = RACE(race)->master_game_difficulty;
	time_difficulty[0] = RACE(race)->master_time_difficulty_0;
	time_difficulty[1] = RACE(race)->unk_0C;
	time_difficulty_2_long = RACE(race)->master_time_difficulty_2_5;
	time_difficulty[2] = (uint8_t)time_difficulty_2_long;
	time_difficulty[3] = (uint8_t)(time_difficulty_2_long >> 8u);
	time_difficulty[4] = (uint8_t)(time_difficulty_2_long >> 16u);
	time_difficulty[5] = (uint8_t)(time_difficulty_2_long >> 24u);
	save_settings();
}
