/**
 *    Name:        game_01.c
 *    Description: the game's functions, part 01 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    24-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets up a flag over the car another player has chosen (animation 1B, a palette by the car, rom_006E80;
   angle: the car's place on the turntable), starts its number (choice_flag_number) and goes on to
   choice_flag_follow. */
void choice_flag_start(void)
{
	uint32_t r9 = 0, r21 = 0, car = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	r9 = 0x1Bu;
	animation_start(r9);
	car = OBJECT(current_object)->choice_flag.car;
	OBJECT(current_object)->palette = rom_006E80[car];
	OBJECT(current_object)->choice_flag.angle = car << 9;
	r21 = start_object();
	mem_write32(r21 + 4u, choice_flag_number_start_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	OBJECT(current_object)->handler = choice_flag_follow_ADDRESS;
	choice_flag_follow();
}

/* Keeps the flag over its car as the turntable turns (its angle less car_select angle, projected as
   turntable_draw does, above the cars) and draws it while car_select_entries' flags_visible says so
   (visible); not drawn behind the view. */
void choice_flag_follow(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, object = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0, t3 = 0;

	object = OBJECT(current_object)->choice_flag.entries;
	t1 = ~~OBJECT(object)->unk_12;
	OBJECT(current_object)->choice_flag.visible = t1;
	if (t1 == 0u)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		return;
	}
	r0 = OBJECT(current_object)->choice_flag.angle;
	r0 = (uint16_t)(r0 - mem_read16(OBJECT(object)->unk_14 + 0x24u));
	r0 = r0 & 0xFFFu;
	r2 = SCREEN(screen)->unk_F08 + 0x140u;
	r1 = (int32_t)((int16_t)rom_100C00[r0] * r2) >> 14;
	r3 = (int32_t)((0u - (int16_t)rom_101400[r0]) * r2) >> 14;
	r2 = 0x44u;
	r3 += 0x140u;
	r0 = 0x100000u;
	t2 = SCREEN(screen)->unk_F04;
	r3 += 0x100u;
	t3 = r3 + t2;
	overflow = v60_overflow_add32(r3, t2, 0u);
	zero = t3 == 0u;
	negative = t3 >> 31;
	r3 = t3;
	if ((negative ^ overflow) | zero)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		return;
	}
	r0 = divide_s32(r0, r3);
	r1 = r1 * r0;
	carry = (r1 >> 11) & 1u;
	r1 = (uint16_t)(((int32_t)r1 >> 12) + 0xA0u + carry);
	r2 = r2 * r0;
	carry = (r2 >> 11) & 1u;
	OBJECT(current_object)->x = r1;
	OBJECT(current_object)->y = ((int32_t)r2 >> 12) + 0x60u + carry;
	r0 = draw_object(r0);
}

/* Sets up the flag of one more player on a car that has one (on the last flag there, below), starts its
   number (choice_flag_number) and goes on to stacked_choice_flag_follow. */
void stacked_choice_flag_start(void)
{
	uint32_t r9 = 0, r21 = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEEu;
	r9 = 0x1Bu;
	animation_start(r9);
	OBJECT(current_object)->palette = rom_006E80[OBJECT(current_object)->stacked_choice_flag.car];
	r21 = start_object();
	mem_write32(r21 + 4u, choice_flag_number_start_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	OBJECT(current_object)->handler = stacked_choice_flag_follow_ADDRESS;
	stacked_choice_flag_follow();
}

/* Draws the flag 14 higher (and 3 right) than the one below it while that one is drawn (its flags bit 5
   and visible); else it is not drawn. */
void stacked_choice_flag_follow(void)
{
	uint32_t r0 = 0, object = 0;
	uint8_t t1 = 0;

	object = OBJECT(current_object)->stacked_choice_flag.below;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		t1 = ~~OBJECT(object)->unk_12;
		OBJECT(current_object)->stacked_choice_flag.visible = t1;
		if (t1 != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 3u;
			r0 = (uint16_t)(OBJECT(object)->y - 0x14u);
			OBJECT(current_object)->y = r0;
			r0 = draw_object(r0);
			return;
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
}

/* Sets up the car number of a flag's player (rom_006E90 by its car number, unk_20E030), then
   choice_flag_number_follow. */
void choice_flag_number_start(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 0xEFu;
	r9 = rom_006E90[unk_20E030[(uint8_t)(OBJECT(OBJECT(current_object)->choice_flag_number.flag)->unk_08 >> 8u)]];
	animation_start(r9);
	OBJECT(current_object)->palette = 0x27u;
	OBJECT(current_object)->handler = choice_flag_number_follow_ADDRESS;
	choice_flag_number_follow();
}

/* Draws the number on its flag (13 right, 10 up from its place) while the flag is drawn (its flags bit 5
   and visible); else it is not drawn. */
void choice_flag_number_follow(void)
{
	uint32_t r0 = 0, object = 0;
	uint8_t t1 = 0;

	object = OBJECT(current_object)->choice_flag_number.flag;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		t1 = ~~OBJECT(object)->unk_12;
		OBJECT(current_object)->choice_flag_number.visible = t1;
		if (t1 != 0u)
		{
			OBJECT(current_object)->x = OBJECT(object)->x + 0x13u;
			r0 = (uint16_t)(OBJECT(object)->y - 0x10u);
			OBJECT(current_object)->y = r0;
			r0 = draw_object(r0);
			return;
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
}

/* Places the object at a point of the car select's turntable scene and draws it: R1 across, R2 down and
   R3 in depth (the turntable's centre at 140), seen from a view that screen unk_F04 brings closer
   (car_select starts it at -300 and raises it to 0): with d = R3 + 100 + unk_F04, x = R1 * (100000 / d)
   >> 12 + A0, y = R2 * (100000 / d) >> 12 + 60, the depth layer (3C) 100 - d >> 4, the width and height
   scale (100000 / d) * the object's size (1C) >> 12. Not drawn (flags bit 5 cleared) with d under 10 or
   x outside -28..168. Used by car_attendant, turntable_car and turntable_scenery. */
uint32_t turntable_draw(uint32_t r1, uint32_t r2, uint32_t r3)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t r3_4 = 0;
	uint16_t r3_3 = 0;
	uint32_t r0 = 0, r3_2 = 0;
	uint16_t t3 = 0;
	uint32_t t1 = 0, t2 = 0;

	r0 = 0x100000u;
	t1 = SCREEN(screen)->unk_F04;
	r3 += 0x100u;
	t2 = r3 + t1;
	overflow = v60_overflow_add32(r3, t1, 0u);
	zero = t2 == 0u;
	negative = t2 >> 31;
	r3_2 = t2;
	if (((negative ^ overflow) | zero) == 0u)
	{
		r0 = divide_s32(r0, r3_2);
		t3 = (uint16_t)r3_2 >> 4;
		r3_3 = t3;
		if (t3 != 0u)
		{
			OBJECT(current_object)->depth_layer = 0u - (r3_3 - 0x100u);
			r3_4 = 0xF4u;
			r1 = r1 * r0;
			carry = (r1 >> 11) & 1u;
			r1 = ((int32_t)r1 >> 12) + 0xA0u + carry;
			if ((int32_t)r1 <= -0x28 || (int32_t)r1 >= 0x168)
			{
				OBJECT(current_object)->flags &= 0xDFu;
				return r0;
			}
			r2 = r2 * r0;
			carry = v60_sha_cy(r2, r3_4, 32);
			OBJECT(current_object)->x = r1;
			OBJECT(current_object)->y = shift_signed_by(r2, r3_4, 32) + 0x60u + carry;
			r0 = r0 * OBJECT(current_object)->unk_1C;
			carry = v60_sha_cy(r0, r3_4, 32);
			r0 = shift_signed_by(r0, r3_4, 32);
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + carry);
			OBJECT(current_object)->width_scale = r0;
			OBJECT(current_object)->height_scale = r0;
			return draw_object(r0);
		}
	}
	OBJECT(current_object)->flags &= 0xDFu;
	return r0;
}

/* Whether the player has not confirmed yet (zero set): confirming is pressing the gas pedal down (screen
   unk_101 from C0, after letting it up under 40) or START, once confirm_delay has run out. */
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
		if (SCREEN(screen)->unk_101 < 0xC0u)
		{
			if ((SCREEN(screen)->unk_118 & 0x40u) != 0u)
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
		if (SCREEN(screen)->unk_101 >= 0x40u)
		{
			if ((SCREEN(screen)->unk_118 & 0x40u) != 0u)
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
		if ((SCREEN(screen)->unk_118 & 0x40u) != 0u)
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
		invalid_jump(rom_0062B0[OBJECT(current_object)->car_select.gas_down]);
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
	uint8_t r0 = 0;
	uint32_t r9 = 0, r19 = 0, r21 = 0;

	r19 = 0x6988u + SCREEN(screen)->unk_00 * 5u * 4u;
	set_up_background(r19);
	r19 = 0x13F66Cu;
	start_palette_fades(r19);
	clear_lcd();
	SCREEN(screen)->unk_F35 = 0xFFu;
	r0 = 0xFu;
	request_sound(r0);
	r0 = 0x3Bu;
	start_dj_message(r0);
	SCREEN(screen)->unk_E2 = 0xFFu;
	r21 = start_object();
	mem_write32(r21 + 4u, entry_banner_start_ADDRESS);
	r21 = start_object();
	mem_write32(r21 + 4u, entry_car_ring_start_ADDRESS);
	OBJECT(current_object)->entry_countdown.car_ring = r21;
	r21 = start_object();
	mem_write32(r21 + 4u, entry_traffic_run_ADDRESS);
	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Du;
	r9 = 0x26Fu;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x4Eu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x7Au;
	OBJECT(current_object)->handler = entry_countdown_count_ADDRESS;
	entry_countdown_count();
}

/* Shows the entry time left (unk_20FC1A, frames): the seconds as the big number (its unk_40), the frames
   into the second to the ring of cars, and on the LCD a shrinking bar (rom_006464) with the seconds;
   when it is over clears the screen's unk_E2, sub_011FE6, and goes on to entry_countdown_done. */
void entry_countdown_count(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, r1 = 0;
	uint32_t t1 = 0;

	r0 = unk_20FC1A;
	if (r0 == 0u)
	{
		SCREEN(screen)->unk_E2 = 0u;
		sub_011FE6();
		OBJECT(current_object)->handler = entry_countdown_done_ADDRESS;
		entry_countdown_done();
		return;
	}
	t1 = r0 - 6u;
	r0 = t1;
	if ((int32_t)t1 < 0)
		r0 = 0u;
	r1 = 0u;
	divux_pair(0x1Eu, &r0, &r1);
	OBJECT(current_object)->frame = r0;
	AS8(OBJECT(OBJECT(current_object)->entry_countdown.car_ring)->unk_10) = r1;
	move_string(rom_006464[(r1 + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	r1 = 0u;
	divux_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero == 0u)
	{
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 0x30u);
		AS8(SCREEN(screen)->unk_F18) = r0;
	}
	AT8(SCREEN(screen)->unk_F18, 1) = r1 + 0x30u;
	r0 = draw_object(r0);
}

/* Does nothing (the number is no longer drawn): the entry time is over. */
void entry_countdown_done(void)
{
}

/* Sets up the WAITING YOUR ENTRY banner (sprite set 287) at the centre of the screen, then
   entry_banner_blink. */
void entry_banner_start(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	r9 = 0x287u;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x287u;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	OBJECT(current_object)->handler = entry_banner_blink_ADDRESS;
	entry_banner_blink();
}

/* Draws the banner 16 frames out of every 32. */
void entry_banner_blink(void)
{
	uint32_t r0 = 0;

	r0 = OBJECT(current_object)->entry_banner.blink;
	r0 = (uint8_t)(r0 + 1u);
	OBJECT(current_object)->entry_banner.blink = r0;
	if ((r0 & 0x10u) != 0u)
		return;
	r0 = draw_object(r0);
}

/* Sets up the ring of cars around the entry countdown with the first car's sprite (rom_0069B8), then
   entry_car_ring_draw. */
void entry_car_ring_start(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->depth_layer = 0x19Bu;
	r9 = rom_0069B8;
	OBJECT(current_object)->palette = rom_0069BA + 4u;
	animation_start(r9);
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
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t copies = 0;

	r0 = OBJECT(current_object)->entry_car_ring.copies;
	if (r0 == 0u)
	{
		r0 = OBJECT(current_object)->entry_car_ring.car;
		r0 = (uint8_t)(r0 + 1u);
		r0 = r0 & 7u;
		OBJECT(current_object)->entry_car_ring.car = r0;
		r10 = rom_0069B8_ADDRESS + r0 * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->palette = ROM16(rom_000002_ADDRESS + r10) + 4u;
		animation_start(r9);
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		return;
	}
	if ((int8_t)r0 > 0x10)
		r0 = 0x10u;
	OBJECT(current_object)->entry_car_ring.copies = r0;
	do
	{
		r0 = OBJECT(current_object)->entry_car_ring.copies;
		r0 = (r0 & 0xFu) << 8;
		r1 = (int16_t)rom_100C00[r0];
		r1 += r1 * 2u * 2u;
		carry = (r1 >> 9) & 1u;
		r1 = (uint16_t)(((int32_t)r1 >> 10) + 0xA0u + carry);
		r2 = 0u - (int16_t)rom_101400[r0];
		r0 = r2 * 2u * 2u;
		r2 += r0;
		carry = (r2 >> 9) & 1u;
		OBJECT(current_object)->x = r1;
		OBJECT(current_object)->y = ((int32_t)r2 >> 10) + 0x70u + carry;
		draw_object(r0);
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
	uint32_t r0 = 0, r21 = 0;
	uint16_t delay = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = random_number();
		if ((r0 & 0xFu) == 0u)
		{
			r21 = start_object();
			mem_write32(r21 + 4u, entry_traffic_car_drive_ADDRESS);
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
		invalid_jump(rom_00661C[OBJECT(current_object)->state]);
	}
}

/* A car coming up the road toward the view behind the entry countdown (car: a random one of the eight,
   rom_0069B8), by state: 0 sets it up and picks its way: 1 along the path at 0067B2 (scale, x and y a
   frame, path_step) seen from the front and one side (frame 6), 2 the same path mirrored (x = 96 - x)
   from the other side (frame 7); one car in 128 goes to 3 or 4 instead, the same paths, but after 45
   frames (spin_delay) its frames, its views all round, run five a frame (it spins) with
   set_tyre_noise(4) each frame; at the end of the path it requests its car's sound (rom_0067A9) and
   ends. */
void entry_traffic_car_drive(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, ignored = 0, path_step = 0;
	uint8_t t1 = 0;
	uint16_t spin_delay = 0, spin_delay_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = random_number();
		r1 = byte_swap32(r0);
		r0 &= 7u;
		OBJECT(current_object)->entry_traffic_car.car = r0;
		r10 = rom_0069B8_ADDRESS + r0 * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->palette = ROM16(rom_000002_ADDRESS + r10);
		animation_start(r9);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 3u;
		OBJECT(current_object)->frame = 6u;
		OBJECT(current_object)->state++;
		t1 = r1 & 0x7Fu;
		r1 = (r1 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			OBJECT(current_object)->entry_traffic_car.spin_delay = 0x2Du;
			OBJECT(current_object)->state += 2u;
		}
		if ((int32_t)r1 >= 0)
		{
			path_step = OBJECT(current_object)->entry_traffic_car.path_step;
			OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
			r10 = 0x67B2u + path_step * 3u * 2u;
			r0 = ROM16(r10);
			r10 += 2u;
			if (r0 != 0u)
			{
				OBJECT(current_object)->width_scale = r0;
				OBJECT(current_object)->height_scale = r0;
				OBJECT(current_object)->x = ROM16(r10);
				OBJECT(current_object)->y = ROM16(r10 + 2u);
				r0 = draw_object(r0);
				return;
			}
			r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(r0_2);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->frame = 7u;
		OBJECT(current_object)->state++;
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		r10 = 0x67B2u + path_step * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(r0_2);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->x = r0;
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	case 3:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay != 0u)
		{
			spin_delay = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay - 1u;
			if (spin_delay != 1u)
			{
				path_step = OBJECT(current_object)->entry_traffic_car.path_step;
				OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
				r10 = 0x67B2u + path_step * 3u * 2u;
				r0 = ROM16(r10);
				r10 += 2u;
				if (r0 != 0u)
				{
					OBJECT(current_object)->width_scale = r0;
					OBJECT(current_object)->height_scale = r0;
					OBJECT(current_object)->x = ROM16(r10);
					OBJECT(current_object)->y = ROM16(r10 + 2u);
					r0 = draw_object(r0);
					return;
				}
				r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
				request_sound(r0_2);
				OBJECT(current_object)->flags = 0u;
				return;
			}
		}
		ignored = animation_advance(0u);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		animation_advance(ignored);
		r0_2 = 4u;
		set_tyre_noise(r0_2);
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		r10 = 0x67B2u + path_step * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 != 0u)
		{
			OBJECT(current_object)->width_scale = r0;
			OBJECT(current_object)->height_scale = r0;
			OBJECT(current_object)->x = ROM16(r10);
			OBJECT(current_object)->y = ROM16(r10 + 2u);
			r0 = draw_object(r0);
			return;
		}
		r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(r0_2);
		OBJECT(current_object)->flags = 0u;
		return;
	case 1:
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		r10 = 0x67B2u + path_step * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 != 0u)
		{
			OBJECT(current_object)->width_scale = r0;
			OBJECT(current_object)->height_scale = r0;
			OBJECT(current_object)->x = ROM16(r10);
			OBJECT(current_object)->y = ROM16(r10 + 2u);
			r0 = draw_object(r0);
			return;
		}
		r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
		request_sound(r0_2);
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		if (OBJECT(current_object)->entry_traffic_car.spin_delay != 0u)
		{
			spin_delay_2 = OBJECT(current_object)->entry_traffic_car.spin_delay;
			OBJECT(current_object)->entry_traffic_car.spin_delay = spin_delay_2 - 1u;
			if (spin_delay_2 != 1u)
			{
				path_step = OBJECT(current_object)->entry_traffic_car.path_step;
				OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
				r10 = 0x67B2u + path_step * 3u * 2u;
				r0 = ROM16(r10);
				r10 += 2u;
				if (r0 == 0u)
				{
					r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
					request_sound(r0_2);
					OBJECT(current_object)->flags = 0u;
					return;
				}
				OBJECT(current_object)->width_scale = r0;
				OBJECT(current_object)->height_scale = r0;
				r0 = (uint16_t)(0x96u - ROM16(r10));
				OBJECT(current_object)->x = r0;
				OBJECT(current_object)->y = ROM16(r10 + 2u);
				r0 = draw_object(r0);
				return;
			}
		}
		ignored = animation_advance(0u);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		ignored = animation_advance(ignored);
		animation_advance(ignored);
		r0_2 = 4u;
		set_tyre_noise(r0_2);
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		r10 = 0x67B2u + path_step * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(r0_2);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->x = r0;
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	case 2:
		path_step = OBJECT(current_object)->entry_traffic_car.path_step;
		OBJECT(current_object)->entry_traffic_car.path_step = path_step + 1u;
		r10 = 0x67B2u + path_step * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[OBJECT(current_object)->entry_traffic_car.car];
			request_sound(r0_2);
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->x = r0;
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00665C[OBJECT(current_object)->state]);
	}
}

/* Gives the object the next palette (unk_3A, an index into the sprites' palette words, rom_032BA4) of
   its palette list (unk_18); a negative entry goes back by that many bytes, to loop. */
void step_palette(void)
{
	uint32_t r0 = 0, r10 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r0 = (int16_t)mem_read16(r10);
	r10 += 2u;
	if ((int16_t)r0 < 0)
	{
		r10 += r0;
		r0 = mem_read16(r10);
		r10 += 2u;
	}
	OBJECT(current_object)->palette = r0;
	OBJECT(current_object)->unk_18 = r10;
}

/* The screen's car, each frame: player_car_check_place, player_car_read_controls,
   player_car_build_hit_boxes, then by state: 0 sets up the race (the view's constants in screen
   unk_10-18, the course, the car on its grid place: segment 16 + rom_0091CC by players and place, road_x
   from rom_0090AC, its world position; the palette of its model, rom_009510; timer 8 frames in the demo,
   149 in a race with the intro view car unk_10C/unk_10E; the gears, torque curves, crash physics, with
   screen unk_318 the race's time left (race unk_6A: 40 seconds plus unk_20FC0B), the engine sound); 1
   the countdown to the start: brake full and the wheel centred until timer runs out (others read it as
   the frames to the start), then car unk_66 and race unk_5D; 2 driving (player_car_move): goes to 3 when
   the time is up (car unk_180), to 4 at the goal (car unk_181); counts hit_guard down; on a hit (flags
   bit 2, by hit_kind): 3 a fork sign slows it for 15 frames (car unk_147), 0 scenery, 1 a road object
   (hit_other), 2 the other player's car (hit_other): a speed change, a push and a bounce from the
   model's crash table (car unk_130, by impact_speed or the cars' speeds and places), the hit's sounds, a
   crash animation, the CRASH text (car unk_136), immune (flags bit 3), state 5; 5 the crash plays out:
   no controls, the push and the speed change die away, it bounces under gravity (sound 31 on landing),
   back to 2 when the animation ends with 15 frames of hit_guard; 3 time up: gas off and the model's
   brake (rom_009530), 180 frames, then the race is over (car unk_182), GAME OVER (004771) with one
   player or race_result_start with two, 60 frames, screen unk_94 bit 4 and DJ message 36 or 37; 4 the
   goal: finished, the race's stage count up, waits for the screen's finish time (screen 410) into race
   unk_16 and the race time, race unk_5E, then drives itself (player_car_steer_to_stop) to its
   stop_position by its place (rom_009540), plays the model's arrival animation (rom_009520), brakes to a
   stop, 90 frames, screen unk_94 bit 4. */
void player_car_update(void)
{
	bool L_0071FB = false, L_007E41 = false, L_007F31 = false;
	uint32_t overflow = 0;
	uint8_t r0_3 = 0, r9_2 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r17 = 0, r19 = 0, r21 = 0, crash_sound = 0, gravity = 0, ignored = 0, model = 0, object = 0, object_2 = 0;
	uint8_t hit_guard = 0, crash_sound_delay = 0;
	uint16_t timer = 0, timer_2 = 0, timer_3 = 0, timer_4 = 0;
	uint32_t target1 = 0, t1 = 0;

	unk_20E0D0[SCREEN(screen)->unk_00] = 0u;
	player_car_check_place();
	player_car_read_controls();
	player_car_build_hit_boxes();
	do
	{
		do
		{
			target1 = L_0071FB ? 0x7134u : L_007F31 ? 0x70B9u : rom_006F18[OBJECT(current_object)->state];
			switch (target1)
			{
			case 0x006F30u:
				SCREEN(screen)->unk_A6 = SCREEN(screen)->unk_A3;
				r0 = rom_010BC8;
				r1 = mem_read8(SCREEN(screen)->unk_308 + 0x1Fu);
				r2 = rom_010BCC_ADDRESS;
				if (unk_20E623 != 0u)
					r1 = 0u;
				SCREEN(screen)->unk_12 = mem_read16(r0 + r1 * 2u);
				SCREEN(screen)->unk_10 = ROM16(r2);
				r2 += 2u;
				SCREEN(screen)->unk_14 = ROM16(r2);
				r2 += 2u;
				SCREEN(screen)->unk_16 = ROM16(r2);
				SCREEN(screen)->unk_18 = ROM16(r2 + 2u + CAR(player_car)->model * 2u);
				player_car_set_horizon();
				OBJECT(current_object)->state++;
				CAR(player_car)->object = current_object;
				player_car_start_course();
				object = screen + 0x2000u;
				CAR(player_car)->window_record = object;
				r0_2 = 0x10u;
				CAR(player_car)->window_segment = 0u;
				r1 = mem_read8(screen + 0x320u + SCREEN(screen)->unk_01);
				r2 = 0u;
				if ((SCREEN(screen)->unk_90 & 0x100u) == 0u)
				{
					r2 = mem_read8(SCREEN(screen)->unk_308 + 0x1Fu);
					r2 = (uint8_t)(r2 - 1u);
				}
				r0_2 += mem_read16(rom_0091CC[r2] + r1 * 2u);
				CAR(player_car)->segment = r0_2;
				r0_2 = (uint16_t)(r0_2 * 0x20u);
				CAR(player_car)->segment_offset = r0_2;
				object = (object & ~0xFFFFu) | (uint16_t)(object + r0_2);
				r1 = mem_read32(rom_0090AC[r2] + r1 * 4u);
				AS32(CAR(player_car)->road_x) = r1;
				OBJECT(current_object)->player_car.heading = AS16(OBJECT(object)->flags);
				object += 2u;
				AS16(OBJECT(current_object)->unk_8C) = OBJECT(object - 0x2u)->unk_02;
				object += 2u;
				OBJECT(current_object)->player_car.world_x = r1 + OBJECT(object - 0x4u)->handler;
				object += 4u;
				OBJECT(current_object)->player_car.world_z = OBJECT(object - 0x8u)->unk_08;
				OBJECT(current_object)->player_car.world_y = AS32(OBJECT(object - 0x8u)->state);
				OBJECT(current_object)->palette = rom_009510[CAR(player_car)->model];
				OBJECT(current_object)->player_car.timer = 8u;
				if ((SCREEN(screen)->unk_90 & 0x100u) == 0u)
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
				SCREEN(screen)->unk_D0 = screen + 0x600u;
				player_car_project_road();
				player_car_load_crash_physics();
				if (SCREEN(screen)->unk_318 != 0u)
					RACE(SCREEN(screen)->race)->unk_6A = ((int8_t)unk_20FC0B[0] + 0x28u) * 0x1Eu;
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
				if ((SCREEN(screen)->unk_90 & 0x100u) == 0u)
				{
					RACE(SCREEN(screen)->race)->unk_5D = 0xFFu;
					SCREEN(screen)->unk_31C = 0xFFu;
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
								CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
								player_car_move();
								return;
							}
							switch (OBJECT(current_object)->player_car.hit_kind)
							{
							case 3:
								OBJECT(current_object)->flags &= 0xFBu;
								CAR(player_car)->slowed_frames = 0xFu;
								CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
								player_car_move();
								return;
							case 0:
								r19 = CAR(player_car)->crash_table;
								r5 = OBJECT(current_object)->player_car.impact_speed;
								if (r5 >= 0xFFFFu)
									r5 = 0xFFFFu;
								r0 = 0u - r5;
								r5 = (r5 >> 7) + rom_008026[mem_read8(screen + 0x320u + SCREEN(screen)->unk_01)];
								if (r5 >= 0x1F0u)
									r5 = 0x1F0u;
								r19 += (r5 & ~0xFFFFu) | (r5 & 0x1F0u);
								OBJECT(current_object)->player_car.speed_change = (int32_t)(r0 * mem_read16(r19 + 4u)) >> 8;
								r8 = (uint16_t)CAR(player_car)->road_x;
								r8 = r8 >> 15;
								OBJECT(current_object)->player_car.sideways_push = mem_read16(r19 + 6u + r8 * 2u);
								OBJECT(current_object)->player_car.bounce_speed = mem_read16(r19 + 0xAu);
								break;
							case 1:
								object_2 = (screen & ~0xFFFFu) | OBJECT(current_object)->player_car.hit_other;
								r4 = OBJECT(current_object)->player_car.speed;
								r5 = (uint32_t)OBJECT(object_2)->unk_2C << 8;
								r6 = OBJECT(object_2)->unk_6A;
								r0 = CAR(player_car)->road_x * 2u;
								r1 = ((int16_t)OBJECT(object_2)->unk_20 + (uint32_t)mem_read16((((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset)) + 0x12u) * (int8_t)OBJECT(object_2)->unk_3F) * 2u;
								r2 = OBJECT(current_object)->player_car.sideways_speed * 2u;
								r3 = (int16_t)OBJECT(object_2)->unk_2E * 2u;
								L_007E41 = true;
								break;
							case 2:
								r10 = OBJECT(current_object)->player_car.hit_other;
								r10 = 0x20FC00u + (uint16_t)(r10 << 7);
								r0 = CAR(player_car)->road_x;
								r1 = (int16_t)mem_read16(r10 + 0x44u);
								r2 = OBJECT(current_object)->player_car.sideways_speed;
								r3 = (int16_t)mem_read16(r10 + 0x4Eu);
								r4 = OBJECT(current_object)->player_car.speed;
								r5 = mem_read16(r10 + 0x4Cu);
								r4 = r4 >> 1;
								r5 = r5 >> 1;
								r6 = rom_008038[mem_read8(r10 + 0x49u)];
								L_007E41 = true;
								break;
							default:
								invalid_jump(rom_007D28[OBJECT(current_object)->player_car.hit_kind]);
							}
							if (L_007E41)
							{
								L_007E41 = false;
								r7 = rom_008038[CAR(player_car)->model];
								r19 = CAR(player_car)->crash_table;
								r5 -= r4;
								r4 = r5;
								if ((int32_t)r5 < 0)
									r5 = (0u - r5) * 2u;
								r5 = divide_s32((r5 >> 8) * r6, r7) + rom_008026[mem_read8(screen + 0x320u + SCREEN(screen)->unk_01)];
								if (r5 >= 0x1F0u)
									r5 = 0x1F0u;
								r19 += (r5 & ~0xFFFFu) | (r5 & 0x1F0u);
								OBJECT(current_object)->player_car.speed_change = (int32_t)(((int32_t)(r4 * mem_read16(r19 + 0xEu)) >> 8) * r6) >> 8;
								r0 = r0 - r1 + ((r3 - r2) << 1);
								r8 = (0u - r0) >> 31;
								r0 = divide_s32(((int32_t)(r0 * (int16_t)mem_read16(r19 + 8u)) >> 7) * r6, r7);
								overflow = v60_movt_overflow(r0, 32, 16);
								OBJECT(current_object)->player_car.sideways_push = r0;
								OBJECT(current_object)->player_car.bounce_speed = mem_read16(r19 + 0xAu);
							}
							OBJECT(current_object)->state = 5u;
							OBJECT(current_object)->player_car.step = 0u;
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->flags |= 8u;
							CAR(player_car)->crash_sound = mem_read8(r19 + 0xDu);
							CAR(player_car)->crash_sound_delay = 0xAu;
							r0_3 = (uint8_t)mem_read16(r19 + 0xCu);
							request_sound(r0_3);
							r9 = mem_read16(r19 + r8 * 2u);
							r1 = player_car_view_height();
							r9 = (uint16_t)(r9 + r1);
							OBJECT(current_object)->animation = r9;
							r9_2 = mem_read8(rom_034138[r9]);
							OBJECT(current_object)->player_car.frame_time = r9_2;
							OBJECT(current_object)->player_car.frame_countdown = r9_2;
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
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->flags &= 0xFBu;
							OBJECT(current_object)->player_car.hit_guard = 0xFu;
							OBJECT(current_object)->player_car.sideways_push = 0;
							OBJECT(current_object)->player_car.bounce_speed = 0;
							OBJECT(current_object)->player_car.bounce_height = 0;
							OBJECT(current_object)->player_car.speed_change = 0;
							OBJECT(current_object)->player_car.gas = 0u;
							CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
							player_car_move();
							return;
						}
						unk_20E0D0[SCREEN(screen)->unk_00] = 0x10u;
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
						if (OBJECT(current_object)->player_car.frame_time == 0u)
						{
							OBJECT(current_object)->state = 2u;
							OBJECT(current_object)->player_car.step = 0u;
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->flags &= 0xFBu;
							OBJECT(current_object)->player_car.hit_guard = 0xFu;
							OBJECT(current_object)->player_car.sideways_push = 0;
							OBJECT(current_object)->player_car.bounce_speed = 0;
							OBJECT(current_object)->player_car.bounce_height = 0;
							OBJECT(current_object)->player_car.speed_change = 0;
							OBJECT(current_object)->player_car.gas = 0u;
							CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
							player_car_move();
							return;
						}
						OBJECT(current_object)->player_car.gas = 0u;
						OBJECT(current_object)->player_car.brake = 0u;
						OBJECT(current_object)->player_car.wheel = 0x80u;
						OBJECT(current_object)->player_car.sideways_push = (int32_t)((uint32_t)OBJECT(current_object)->player_car.sideways_push * CAR(player_car)->push_decay) / 0x100;
						OBJECT(current_object)->player_car.speed_change = (int32_t)((uint32_t)OBJECT(current_object)->player_car.speed_change * CAR(player_car)->speed_change_decay) / 0x100;
						r8 = OBJECT(current_object)->player_car.bounce_height;
						gravity = CAR(player_car)->gravity;
						r9 = OBJECT(current_object)->player_car.bounce_speed - gravity;
						r7 = r8;
						r8 += r9;
						if ((int32_t)(r7 ^ r8) < 0)
						{
							r8 = 0u;
							r9 = (int32_t)((r9 + gravity) * CAR(player_car)->bounce_damping) / -0x100;
							if (r9 != 0u)
							{
								r0_3 = 0x31u;
								request_sound(r0_3);
							}
						}
						OBJECT(current_object)->player_car.bounce_speed = r9;
						OBJECT(current_object)->player_car.bounce_height = r8;
						CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
						player_car_move();
						return;
					}
				}
				if (L_0071FB)
					break;
				OBJECT(current_object)->state = 3u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->unk_0E = 0u;
				OBJECT(current_object)->unk_0F = 0u;
			case 0x007134u:
				if (!L_0071FB)
				{
					if (CAR(player_car)->at_goal == 0u)
					{
						OBJECT(current_object)->player_car.gas = 0u;
						r0 = CAR(player_car)->model;
						r0 = (uint8_t)(r0 * 2u);
						OBJECT(current_object)->player_car.brake = rom_009530[(uint8_t)(r0 + CAR(player_car)->transmission)];
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
									CAR(player_car)->race_time = mem_read32(SCREEN(screen)->unk_308 + 0x6Cu);
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
							if (mem_read8(SCREEN(screen)->unk_308 + 0x1Fu) <= 1u)
							{
								r21 = start_object();
								mem_write32(r21 + 4u, game_over_banner_show_ADDRESS);
							}
							else
							{
								r21 = start_object();
								mem_write32(r21 + 4u, race_result_start_ADDRESS);
							}
							timer_3 = OBJECT(current_object)->player_car.timer;
							OBJECT(current_object)->player_car.timer = timer_3 - 1u;
							if (timer_3 == 1u)
							{
								OBJECT(current_object)->player_car.step++;
								SCREEN(screen)->unk_94 |= 0x10u;
								r0_3 = 0x37u;
								if ((int8_t)SCREEN(screen)->unk_A4 <= 3)
									r0_3 = 0x36u;
								start_dj_message(r0_3);
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
							SCREEN(screen)->unk_94 |= 0x10u;
							r0_3 = 0x37u;
							if ((int8_t)SCREEN(screen)->unk_A4 <= 3)
								r0_3 = 0x36u;
							start_dj_message(r0_3);
							player_car_move();
							model = CAR(player_car)->model;
							player_car_steer_frame(model);
							return;
						case 3:
							break;
						default:
							invalid_jump(rom_007164[OBJECT(current_object)->player_car.step]);
						}
						player_car_move();
						model = CAR(player_car)->model;
						player_car_steer_frame(model);
						return;
					}
				}
				OBJECT(current_object)->state = 4u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->unk_0E = 0u;
				OBJECT(current_object)->unk_0F = 0u;
			case 0x007200u:
				switch (OBJECT(current_object)->player_car.step)
				{
				case 0:
					OBJECT(current_object)->player_car.step++;
					OBJECT(current_object)->player_car.finished = 0xFFu;
					t1 = SCREEN(screen)->race + 0x60u;
					mem_write8(t1, mem_read8(t1) + 1u);
					OBJECT(current_object)->player_car.stop_position = CAR(player_car)->segment;
					player_car_clear_hit();
					OBJECT(current_object)->flags |= 8u;
					player_car_move();
					return;
				case 1:
					r0 = mem_read16(screen + 0x410u + SCREEN(screen)->unk_01 * 2u);
					if (r0 == 0u)
					{
						player_car_move();
						return;
					}
					RACE(SCREEN(screen)->race)->unk_16 = r0;
					CAR(player_car)->race_time = r0 * 0xAu;
					RACE(SCREEN(screen)->race)->unk_5E = 0xFFu;
					OBJECT(current_object)->player_car.step++;
					r0 = rom_009540[mem_read8(screen + 0x320u + SCREEN(screen)->unk_01)];
					OBJECT(current_object)->player_car.stop_position += mem_read16(r0);
					r0 += 2u;
					OBJECT(current_object)->player_car.stop_x = mem_read16(r0);
					OBJECT(current_object)->player_car.stop_speed = mem_read16(r0 + 2u);
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
					r9 = rom_009520[CAR(player_car)->model];
					animation_start(r9);
				case 3:
					OBJECT(current_object)->player_car.gas = 0x40u;
					OBJECT(current_object)->player_car.brake = 0u;
					OBJECT(current_object)->player_car.wheel = 0x80u;
					player_car_steer_to_stop();
					r0_3 = rom_00ABC3[CAR(player_car)->model];
					set_tyre_noise(r0_3);
					if (CAR(player_car)->speed != 0u)
					{
						r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
						player_car_drive(&r11, &r13, &r15, &r17);
						CAR(player_car)->wheel_effect_1 = 2u;
						CAR(player_car)->wheel_effect_2 = 2u;
						CAR(player_car)->wheel_effect_sides = 3u;
						AS16(CAR(player_car)->brake_lights) = 2u;
						player_car_place_view(r11, r13, r15, r17);
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
						SCREEN(screen)->unk_94 |= 0x10u;
					}
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&r11, &r13, &r15, &r17);
					player_car_place_view(r11, r13, r15, r17);
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
						SCREEN(screen)->unk_94 |= 0x10u;
					}
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&r11, &r13, &r15, &r17);
					player_car_place_view(r11, r13, r15, r17);
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
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
					player_car_drive(&r11, &r13, &r15, &r17);
					player_car_place_view(r11, r13, r15, r17);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road();
					player_car_engine_sound();
					ignored = animation_advance(0u);
					return;
				default:
					invalid_jump(rom_00720C[OBJECT(current_object)->player_car.step]);
				}
				break;
			case 0x007F31u:
				L_007F31 = true;
				break;
			default:
				invalid_jump(target1);
			}
		} while (L_0071FB);
	} while (L_007F31);
}

/* One frame of the car on the road: player_car_drive, player_car_place_view, player_car_move_view,
   player_car_set_horizon, player_car_publish, player_car_extend_course, player_car_project_road,
   player_car_engine_sound and player_car_animate; no wheel effects while it crashes (state 5). */
void player_car_move(void)
{
	uint32_t r11 = 0, r13 = 0, r15 = 0, r17 = 0;

	r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
	player_car_drive(&r11, &r13, &r15, &r17);
	player_car_place_view(r11, r13, r15, r17);
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

/* Drives the car by itself after the goal: gas 40, no brake, the speed (car unk_74) a 16th of the way to
   stop_speed each frame, and the wheel turned step by step toward stop_x across the road (centred within
   8 of it; stop_wheel keeps it). */
void player_car_steer_to_stop(void)
{
	uint32_t negative = 0, overflow = 0;
	uint8_t r0_2 = 0;
	uint16_t r2 = 0, stop_x = 0;
	uint32_t r0 = 0, r1 = 0;
	uint16_t road_x = 0, t1 = 0;
	uint32_t stop_speed = 0;

	OBJECT(current_object)->player_car.brake = 0u;
	OBJECT(current_object)->player_car.gas = 0x40u;
	r0 = CAR(player_car)->speed;
	stop_speed = OBJECT(current_object)->player_car.stop_speed;
	r1 = stop_speed - r0;
	if (stop_speed != r0)
		r0 += (int32_t)(r1 * 1u) / 0x10 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
	CAR(player_car)->speed = r0;
	r0_2 = OBJECT(current_object)->player_car.stop_wheel;
	stop_x = (uint16_t)OBJECT(current_object)->player_car.stop_x;
	road_x = CAR(player_car)->road_x;
	t1 = road_x - stop_x;
	overflow = (uint32_t)(int16_t)road_x - (int16_t)stop_x != (uint32_t)(int16_t)(road_x - stop_x);
	negative = t1 >> 15;
	r2 = t1;
	if (road_x == stop_x)
	{
		OBJECT(current_object)->player_car.stop_wheel = 0x80u;
		OBJECT(current_object)->player_car.wheel = 0x80u;
		return;
	}
	if (negative ^ overflow)
	{
		if ((int16_t)r2 >= -8)
		{
			OBJECT(current_object)->player_car.stop_wheel = 0x80u;
			OBJECT(current_object)->player_car.wheel = 0x80u;
			return;
		}
		r0_2--;
		if ((int8_t)r0_2 < 0x10)
			r0_2 = 0x10u;
		if ((int8_t)r0_2 > 0x6F)
			r0_2 = 0x6Fu;
		OBJECT(current_object)->player_car.stop_wheel = r0_2;
		OBJECT(current_object)->player_car.wheel = r0_2;
		return;
	}
	if ((int16_t)r2 <= 8)
	{
		OBJECT(current_object)->player_car.stop_wheel = 0x80u;
		OBJECT(current_object)->player_car.wheel = 0x80u;
		return;
	}
	r0_2++;
	if ((int8_t)r0_2 > -0x31)
		r0_2 = 0xCFu;
	if ((int8_t)r0_2 < -0x70)
		r0_2 = 0x90u;
	OBJECT(current_object)->player_car.stop_wheel = r0_2;
	OBJECT(current_object)->player_car.wheel = r0_2;
}

/* Keeps the race order around the screen's car: screen unk_60-6C the race records (unk_70-73 the
   players) of the leader, the last, the car just ahead and the car just behind (all itself in the demo),
   screen unk_350 and unk_352 the leader's race unk_46 and unk_62. When its place (screen 320 + player,
   kept in place) changes during the race: sound 37 when the car it passed or that passed it is within
   160 across, else 3A. While first, blinks one of the screen's lamps (the count, period and timer bytes
   C9-CB of its entry at unk_20E0C0). */
void player_car_check_place(void)
{
	uint32_t zero = 0, carry = 0;
	uint8_t r0_2 = 0;
	uint16_t r3_2 = 0;
	uint32_t r0 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0;
	uint8_t place = 0;

	if ((SCREEN(screen)->unk_90 & 0x100u) != 0u)
	{
		r7 = SCREEN(screen)->unk_01;
		r6 = r7;
		r5 = r7;
		r3 = r7;
		SCREEN(screen)->unk_70 = r7;
		SCREEN(screen)->unk_71 = r6;
		SCREEN(screen)->unk_72 = r5;
		SCREEN(screen)->unk_73 = r3;
		r7 = rom_00E0A0[r7] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_60 = r7;
		SCREEN(screen)->unk_64 = rom_00E0A0[r6] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_68 = rom_00E0A0[r5] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_6C = rom_00E0A0[r3] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_350 = mem_read16(r7 + 0x46u);
		SCREEN(screen)->unk_352 = mem_read8(r7 + 0x62u);
		return;
	}
	r10 = SCREEN(screen)->unk_308 + 0x1Fu;
	r9 = mem_read8(r10);
	r9 = (uint8_t)(r9 - 1u);
	r10++;
	r6 = mem_read8(r10 + r9);
	r7 = mem_read8(r10);
	r4 = mem_read8(screen + 0x320u + SCREEN(screen)->unk_01);
	if (r4 == 0u)
	{
		r0 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
		if (mem_read8(r0 + 0xC9u) == 0u)
		{
			mem_write8(r0 + 0xC9u, 2u);
			mem_write8(r0 + 0xCAu, 0x10u);
			mem_write8(r0 + 0xCBu, 0x10u);
		}
	}
	place = OBJECT(current_object)->player_car.place;
	carry = r4 < place;
	zero = (uint8_t)(r4 - place) == 0u;
	if (r4 != place)
	{
		OBJECT(current_object)->player_car.place = r4;
		r3 = SCREEN(screen)->unk_6C;
		if (carry | zero)
			r3 = SCREEN(screen)->unk_68;
		r3_2 = (uint16_t)(mem_read16(r3 + 0x44u) - RACE(SCREEN(screen)->race_copy)->unk_44);
		r0_2 = 0x3Au;
		if ((int16_t)r3_2 < 0x160 && (int16_t)r3_2 > -0x160)
			r0_2 = 0x37u;
		if (OBJECT(current_object)->state != 0u)
			request_sound(r0_2);
	}
	r3 = r4;
	if (r4 != 0u)
		r4 = (uint8_t)(r4 - 1u);
	if (r3 != r9)
		r3 = (uint8_t)(r3 + 1u);
	r5 = mem_read8(r10 + r4);
	r3 = mem_read8(r10 + r3);
	SCREEN(screen)->unk_70 = r7;
	SCREEN(screen)->unk_71 = r6;
	SCREEN(screen)->unk_72 = r5;
	SCREEN(screen)->unk_73 = r3;
	r7 = rom_00E0A0[r7] + unk_20E000_ADDRESS;
	SCREEN(screen)->unk_60 = r7;
	SCREEN(screen)->unk_64 = rom_00E0A0[r6] + unk_20E000_ADDRESS;
	SCREEN(screen)->unk_68 = rom_00E0A0[r5] + unk_20E000_ADDRESS;
	SCREEN(screen)->unk_6C = rom_00E0A0[r3] + unk_20E000_ADDRESS;
	SCREEN(screen)->unk_350 = mem_read16(r7 + 0x46u);
	SCREEN(screen)->unk_352 = mem_read8(r7 + 0x62u);
}

/* Hands the car's state on: this frame's view into the screen's ring of eight (screen + 600, slot
   unk_D4, also race unk_4B) and the view of the slot race_copy unk_4B names back into the car and screen
   unk_D0, so that both screens draw the view of the same frame; and the car's world position, animation,
   palette, frame, wheel effects, speed, sideways speed, flags, segments, place across the road and
   bounce into its race record (race unk_5F set), from which race_car draws it on both screens. Not in a
   replay (unk_20E623), which reads them back instead. */
void player_car_publish(void)
{
	uint32_t r0 = 0, r13 = 0, race = 0, race_2 = 0;

	race = SCREEN(screen)->race;
	race_2 = SCREEN(screen)->race_copy;
	if (unk_20E623 == 0u)
	{
		r0 = SCREEN(screen)->unk_D4;
		r0 = (uint8_t)(r0 + 1u);
		r0 = r0 & 7u;
		SCREEN(screen)->unk_D4 = r0;
		RACE(race)->unk_4B = r0;
		r12 = screen + 0x600u + (uint16_t)(r0 << 5);
		r0 = RACE(race_2)->unk_4B;
		r13 = screen + 0x600u + (uint16_t)(r0 << 5);
		SCREEN(screen)->unk_D0 = r13;
	}
	else
	{
		r12 = screen + 0x600u;
		r13 = r12;
		SCREEN(screen)->unk_D0 = r13;
	}
	mem_write32(r12, CAR(player_car)->view_x);
	CAR(player_car)->view_x = mem_read32(r13);
	mem_write32(r12 + 4u, CAR(player_car)->view_y);
	CAR(player_car)->view_y = mem_read32(r13 + 4u);
	mem_write32(r12 + 8u, CAR(player_car)->view_z);
	CAR(player_car)->view_z = mem_read32(r13 + 8u);
	mem_write16(r12 + 0xCu, CAR(player_car)->view_pitch);
	CAR(player_car)->view_pitch = mem_read16(r13 + 0xCu);
	mem_write16(r12 + 0xEu, CAR(player_car)->view_yaw);
	CAR(player_car)->view_yaw = mem_read16(r13 + 0xEu);
	mem_write16(r12 + 0x10u, CAR(player_car)->window_offset);
	mem_write16(r12 + 0x14u, CAR(player_car)->segment_remaining);
	mem_write16(r12 + 0x12u, SCREEN(screen)->unk_B8);
	SCREEN(screen)->unk_B8 = mem_read16(r13 + 0x12u);
	if (unk_20E623 == 0u)
	{
		RACE(race)->unk_30 = OBJECT(current_object)->player_car.world_x;
		RACE(race)->unk_34 = OBJECT(current_object)->player_car.world_y;
		RACE(race)->unk_38 = OBJECT(current_object)->player_car.world_z;
		RACE(race)->unk_3C = OBJECT(current_object)->animation;
		RACE(race)->unk_3E = OBJECT(current_object)->palette;
		RACE(race)->unk_4A = OBJECT(current_object)->frame;
		RACE(race)->unk_50 = CAR(player_car)->wheel_effect_sides;
		RACE(race)->unk_53 = CAR(player_car)->hard_acceleration;
		RACE(race)->unk_51 = CAR(player_car)->wheel_effect_1;
		RACE(race)->unk_52 = CAR(player_car)->wheel_effect_2;
		RACE(race)->unk_4C = OBJECT(current_object)->player_car.speed;
		RACE(race)->unk_4E = OBJECT(current_object)->player_car.sideways_speed;
		RACE(race)->unk_58 = AS16(OBJECT(current_object)->flags);
		RACE(race)->unk_40 = CAR(player_car)->segment_fraction;
		RACE(race)->unk_42 = CAR(player_car)->segment;
		RACE(race)->unk_44 = CAR(player_car)->road_x;
		RACE(race)->unk_46 = CAR(player_car)->window_segment;
		RACE(race)->unk_62 = SCREEN(screen)->unk_A3;
		RACE(race)->unk_56 = (uint16_t)OBJECT(current_object)->player_car.bounce_height;
		RACE(race)->unk_5F = 0xFFu;
	}
}

void sub_007677(void)
{
	uint32_t r0 = 0, race = 0;

	race = SCREEN(screen)->race;
	RACE(race)->unk_5F = 0u;
	r0 = SCREEN(screen)->unk_D4;
	r0 = (uint8_t)(r0 + 1u);
	r0 = r0 & 7u;
	SCREEN(screen)->unk_D4 = r0;
	RACE(race)->unk_4B = r0;
	r12 = screen + 0x600u + (uint16_t)(r0 << 5);
	SCREEN(screen)->unk_D0 = r12;
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
	RACE(race)->unk_50 = 0u;
	CAR(player_car)->hard_acceleration = 0u;
	RACE(race)->unk_53 = 0u;
	CAR(player_car)->wheel_effect_1 = 0u;
	RACE(race)->unk_51 = 0u;
	CAR(player_car)->wheel_effect_2 = 0u;
	RACE(race)->unk_52 = 0u;
	CAR(player_car)->segment_fraction = 0u;
	RACE(race)->unk_40 = 0u;
	CAR(player_car)->segment = 0u;
	RACE(race)->unk_42 = 0u;
	CAR(player_car)->road_x = 0;
	RACE(race)->unk_44 = 0u;
	CAR(player_car)->window_segment = 0u;
	RACE(race)->unk_46 = 0u;
	CAR(player_car)->window_offset = 0u;
	mem_write16(r12 + 0x10u, 0u);
	CAR(player_car)->segment_remaining = 0u;
	mem_write16(r12 + 0x14u, 0u);
	SCREEN(screen)->unk_B8 = 0u;
	mem_write16(r12 + 0x12u, 0u);
	AS16(OBJECT(current_object)->unk_C8) = 0u;
	RACE(race)->unk_56 = 0u;
}

/* Works out the horizon's line on the screen (screen unk_B8) from the view's tilt: the nod (car
   unk_102), the rival and road-object pulls (unk_112, unk_10A), the screen's own tilt (screen unk_18),
   clamped to C0-200, and the intro's (unk_10E). */
void player_car_set_horizon(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0;

	r0 = (uint16_t)(CAR(player_car)->view_pitch_nod + CAR(player_car)->view_pitch_rival + CAR(player_car)->view_pitch_lift + SCREEN(screen)->unk_18);
	if ((int16_t)r0 > 0x200)
		r0 = 0x200u;
	if ((int16_t)r0 < 0xC0)
		r0 = 0xC0u;
	r0 = (uint16_t)(r0 + CAR(player_car)->view_pitch_intro) & 0xFFFu;
	r1 = SCREEN(screen)->unk_10;
	r2 = SCREEN(screen)->unk_12;
	SCREEN(screen)->unk_B8 = divide_s32(divide_s32((int16_t)rom_100C00[r0] * r2, (uint32_t)(int16_t)rom_101400[r0]) * r1, r2 + r1) - SCREEN(screen)->unk_16 + 0x100u;
}

/* Puts the car on its starting segment at the start: no way into the segment, road_x from the grid's
   place, its world position (player_car_world_position) and the view behind it (player_car_place_view). */
void player_car_place_on_road(void)
{
	uint32_t r11 = 0, r17 = 0, road_x = 0, segment_fraction = 0;

	CAR(player_car)->segment_fraction = 0u;
	CAR(player_car)->road_x = (uint16_t)OBJECT(current_object)->player_car.world_x;
	r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->segment_offset);
	road_x = (uint16_t)CAR(player_car)->road_x;
	r17 = 0x80u;
	segment_fraction = CAR(player_car)->segment_fraction;
	r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
	player_car_world_position(r11, road_x, segment_fraction, r17);
	player_car_place_view(r11, road_x, segment_fraction, r17);
}

/* Ends what a hit was doing to the car: no sideways_push, speed_change, bounce, no hit pending (flags
   bit 2), and the car no longer skidding (car unk_67) or slipping (unk_65). */
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

/* Picks the car's animation: while a hit is pending, the one it has goes on; skidding (car unk_67), a
   skid by how far it slides (slide) toward the side the wheel is turned (rom_009310, rom_009410 by
   model); spinning its wheels while it speeds up (car unk_65, acceleration above 0), the model's
   wheelspin (rom_009300); else its body turned by the steering (player_car_steer_frame). Each one in the
   row of the view's height (player_car_view_height); a new one starts at frame 0. */
void player_car_animate(void)
{
	uint8_t r8 = 0, r9_2 = 0;
	uint32_t r1 = 0, r9 = 0, ignored = 0;

	r9 = CAR(player_car)->model;
	if ((OBJECT(current_object)->flags & 4u) != 0u)
	{
		ignored = animation_advance(0u);
		return;
	}
	if (CAR(player_car)->skidding != 0u)
	{
		r8 = (uint8_t)(uint16_t)OBJECT(current_object)->player_car.slide;
		r9 = (uint16_t)(r9 << 4);
		if ((int8_t)OBJECT(current_object)->player_car.wheel >= 0)
		{
			if ((int8_t)r8 > 0)
				r8 = (uint8_t)(0u - r8);
			r9 = rom_009410[(r9 & ~0xFFu) | (uint8_t)(r9 + (0u - r8))];
		}
		else
		{
			if ((int8_t)r8 < 0)
				r8 = (uint8_t)(0u - r8);
			r9 = rom_009310[(r9 & ~0xFFu) | (uint8_t)(r9 + r8)];
		}
		r1 = player_car_view_height();
		r9 = (uint16_t)(r9 + r1);
		if (OBJECT(current_object)->animation != r9)
		{
			OBJECT(current_object)->animation = r9;
			r9_2 = mem_read8(rom_034138[r9]);
			OBJECT(current_object)->player_car.frame_time = r9_2;
			OBJECT(current_object)->player_car.frame_countdown = r9_2;
			OBJECT(current_object)->frame = 0u;
		}
		ignored = animation_advance(0u);
		return;
	}
	if (OBJECT(current_object)->player_car.acceleration <= 0 || CAR(player_car)->wheel_slip == 0u)
	{
		player_car_steer_frame(r9);
		return;
	}
	r9 = rom_009300[r9];
	r1 = player_car_view_height();
	r9 = (uint16_t)(r9 + r1);
	if (OBJECT(current_object)->animation != r9)
	{
		OBJECT(current_object)->animation = r9;
		r9_2 = mem_read8(rom_034138[r9]);
		OBJECT(current_object)->player_car.frame_time = r9_2;
		OBJECT(current_object)->player_car.frame_countdown = r9_2;
		OBJECT(current_object)->frame = 0u;
	}
	ignored = animation_advance(0u);
}

/* The car's body as it steers: the model's animation (rom_00926C) in the row of the view's height, its
   frame (steer_frame) one step a frame toward the one of the steering (the bytes around 0092BC: 0-8, 4
   straight on). */
void player_car_steer_frame(uint32_t r9)
{
	uint8_t r0_2 = 0, r1_2 = 0;
	uint16_t r0 = 0;
	uint32_t r1 = 0;

	r0 = rom_00926C[r9];
	r1 = player_car_view_height();
	OBJECT(current_object)->animation = r0 + r1;
	r0_2 = mem_read8(0x92BCu + OBJECT(current_object)->player_car.steering);
	r1_2 = OBJECT(current_object)->player_car.steer_frame;
	if (r0_2 == r1_2)
	{
		OBJECT(current_object)->frame = r1_2;
		OBJECT(current_object)->player_car.steer_frame = r1_2;
		return;
	}
	if (r0_2 < r1_2)
		r1_2--;
	else
		r1_2++;
	OBJECT(current_object)->frame = r1_2;
	OBJECT(current_object)->player_car.steer_frame = r1_2;
}

/* The row of the car's animations for how steeply the view looks down on it: the view's pitch (car
   unk_2C) less B0 and less the road's pitch under the car (race_copy unk_54), in steps of 30, 0 to 5
   (view_height). */
uint32_t player_car_view_height(void)
{
	uint16_t r3 = 0, r4 = 0;
	uint32_t r1 = 0;
	uint16_t t1 = 0;

	r1 = 0u;
	r3 = 5u;
	t1 = ((CAR(player_car)->view_pitch - 0xB0u) << 4) - ((uint32_t)RACE(SCREEN(screen)->race_copy)->unk_54 << 4);
	r4 = t1;
	if ((int16_t)t1 >= 0)
	{
		r1 = r4 / 0x300u;
		if (r3 <= r1)
			r1 = r3;
	}
	OBJECT(current_object)->player_car.view_height = r1;
	return r1;
}

/* Moves the view with what the car does: when a rival comes close (player_car_check_rivals, rival_step)
   it starts the RIVAL SEKKIN warning (rival_warning) and pulls the view back and up (car unk_110 to
   -256, unk_112 +8 a step) until the rival has gone, then lets it back; a speed gain of more than A0 in
   a frame without wheelspin sets car unk_145 (hard acceleration); the view nods (nod, car unk_100 and
   unk_102) one way with that hard acceleration and the other with hard braking (more than 120 a frame
   with the brake down), easing back; and it lags the car's rises and drops (car unk_104). */
void player_car_move_view(void)
{
	bool L_007ACB = false, L_007AD1 = false, L_007AFA = false;
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint32_t r0_2 = 0, r21 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t7 = 0, t9 = 0;
	uint32_t target1 = 0, t6 = 0, t8 = 0;

	do
	{
		do
		{
			target1 = L_007AD1 ? 0x792Bu : L_007AFA ? 0x792Bu : rom_007917[OBJECT(current_object)->player_car.rival_step];
			switch (target1)
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
									r21 = start_object();
									mem_write32(r21 + 4u, rival_warning_update_ADDRESS);
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
								r0_2 = CAR(player_car)->view_shift_rival;
								if (r0_2 == 0u)
									OBJECT(current_object)->player_car.rival_step = 1u;
								else
								{
									r0_2 = (uint16_t)(r0_2 + 0x10u);
									CAR(player_car)->view_pitch_rival -= 8u;
									CAR(player_car)->view_shift_rival = r0_2;
								}
							}
						}
						else
						{
							r0_2 = CAR(player_car)->view_shift_rival;
							if (r0_2 != 0xFF00u)
							{
								r0_2 = (uint16_t)(r0_2 - 0x10u);
								CAR(player_car)->view_pitch_rival += 8u;
								CAR(player_car)->view_shift_rival = r0_2;
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
				invalid_jump(target1);
			}
		} while (L_007AD1);
	} while (L_007AFA);
	CAR(player_car)->hard_acceleration = 0u;
	r0 = (uint16_t)(0u - OBJECT(current_object)->player_car.speed);
	r1 = (uint16_t)(OBJECT(current_object)->unk_B0 >> 16u);
	AT16(OBJECT(current_object)->unk_B0, 2) = r0;
	r0 -= r1;
	r1 = (uint16_t)OBJECT(current_object)->player_car.nod;
	if ((OBJECT(current_object)->flags & 4u) != 0u)
		r0 = 0u;
	else if ((int16_t)r0 < 0)
	{
		if (CAR(player_car)->wheel_slip != 0u || (int16_t)r0 >= -0xA0)
			r0 = 0u;
		else
		{
			r0 = (uint16_t)(r0 * 1u);
			CAR(player_car)->hard_acceleration = 0xFFu;
		}
	}
	else if (OBJECT(current_object)->player_car.brake_force == 0u)
		r0 = 0u;
	else
	{
		t1 = r0;
		r0 = (uint16_t)(t1 - 0x120u);
		if ((int16_t)t1 <= 0x120)
			r0 = 0u;
		else
			r0 = (uint16_t)(r0 * 1u);
	}
	t2 = (int16_t)(r0 * 1u) / 5;
	r0 = (uint16_t)(t2 - r1);
	if (t2 != r1)
	{
		t3 = (int16_t)r0 >> 3;
		r0 = t3;
		if (t3 == 0u)
			r0++;
	}
	if ((int16_t)r0 < -0x20)
		r0 = 0xFFE0u;
	if ((int16_t)r0 > 6)
		r0 = 6u;
	r1 += r0;
	if ((int16_t)r1 > 0x18)
		r1 = 0x18u;
	if ((int16_t)r1 < -0x100)
		r1 = 0xFF00u;
	OBJECT(current_object)->player_car.nod = r1;
	CAR(player_car)->view_shift_nod = r1;
	if ((int16_t)r1 >= 0)
		r1 = (uint16_t)((int16_t)(r1 * 1u) / 1);
	else
		r1 = (uint16_t)((int16_t)(r1 * 1u) / 1);
	CAR(player_car)->view_pitch_nod = r1;
	r0 = (uint16_t)((0u - OBJECT(current_object)->player_car.sideways_speed) << 1);
	r1 = CAR(player_car)->view_side;
	if (CAR(player_car)->skidding == 0u)
	{
		if (r1 != 0u)
		{
			if ((int16_t)r1 >= 0)
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			else
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if ((int16_t)r0 < 0)
	{
		t4 = r0;
		r2 = (uint16_t)(t4 - r1);
		if (t4 != r1)
		{
			r1 += ((int16_t)r2 >> 2) + 1u;
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if ((int16_t)r0 > 0)
	{
		t5 = r0;
		r2 = (uint16_t)(t5 - r1);
		if (t5 != r1)
		{
			r1 += (int16_t)r2 >> 2;
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if (r1 != 0u)
	{
		if ((int16_t)r1 >= 0)
			r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		else
			r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		if ((int16_t)r1 > 0x40)
			r1 = 0x40u;
	}
	r1 = (uint16_t)(int16_t)CAR(player_car)->view_rise;
	t6 = (uint32_t)OBJECT(current_object)->player_car.world_y - OBJECT(current_object)->player_car.previous_world_y;
	r0_2 = t6 + 0xCu;
	if ((int32_t)t6 < -0xC)
	{
		t7 = r0_2;
		r2 = (uint16_t)(t7 - r1);
		if (t7 == (uint16_t)r1)
			return;
		r1 += ((int16_t)r2 >> 2) + 1u;
	}
	else
	{
		t8 = r0_2;
		r0 = (uint16_t)(t8 - 0xCu);
		if ((int32_t)t8 > 0xC)
		{
			t9 = r0;
			r2 = (uint16_t)(t9 - r1);
			if (t9 == (uint16_t)r1)
				return;
			r1 += (int16_t)r2 >> 2;
		}
		else
		{
			if ((uint16_t)r1 == 0u)
				return;
			if ((int16_t)r1 >= 0)
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			else
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		}
	}
	if ((int16_t)r1 > 8)
		r1 = 8u;
	if ((int16_t)r1 < -8)
		r1 = 0xFFF8u;
	CAR(player_car)->view_rise = r1;
}

/* Whether a rival is close (rival_near): at speed 6000 or more (5000 to stay on), the car just ahead
   (screen unk_68) a little slower and within reach, or the car just behind (unk_6C) a little faster and
   close; once on it stays for 30 frames (rival_hold) after the rival has gone. */
void player_car_check_rivals(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r11 = 0, race = 0;

	race = SCREEN(screen)->race_copy;
	r11 = SCREEN(screen)->unk_68;
	r12 = SCREEN(screen)->unk_6C;
	r0 = AS32(RACE(race)->unk_40);
	r1 = RACE(race)->unk_4C;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 9);
	if (r11 != race)
	{
		r2 = mem_read32(r11 + 0x40u);
		r2 = ((r2 & ~0xFFFFu) | (uint16_t)(r2 << 9)) - r0;
		r3 = mem_read16(r11 + 0x4Cu) - r1;
		if (OBJECT(current_object)->player_car.rival_near == 0u)
		{
			if (r1 < 0x6000u)
			{
				if (OBJECT(current_object)->player_car.rival_hold == 0u)
				{
					OBJECT(current_object)->player_car.rival_near = 0u;
					return;
				}
				OBJECT(current_object)->player_car.rival_hold--;
				return;
			}
			if ((int32_t)r2 <= 0x18000 && (int32_t)r3 <= -1 && (int32_t)r3 >= -0x100)
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
			if (r1 < 0x5000u)
			{
				if (OBJECT(current_object)->player_car.rival_hold == 0u)
				{
					OBJECT(current_object)->player_car.rival_near = 0u;
					return;
				}
				OBJECT(current_object)->player_car.rival_hold--;
				return;
			}
			if ((int32_t)r2 <= 0x20000 && (int32_t)r3 <= -1 && (int32_t)r3 >= -0x200)
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
	r2 = mem_read32(r12 + 0x40u);
	r2 = ((r2 & ~0xFFFFu) | (uint16_t)(r2 << 9)) - r0;
	r3 = mem_read16(r12 + 0x4Cu) - r1;
	if (OBJECT(current_object)->player_car.rival_near == 0u)
	{
		if (r1 < 0x6000u || (int32_t)r2 < -0x30000 || (int32_t)r3 > 0x2000 || (int32_t)r3 < 0)
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
	if (r1 < 0x5000u || (int32_t)r2 < -0x40000 || (int32_t)r3 > 0x3000 || (int32_t)r3 < -0x1000)
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
	uint32_t r19 = 0, r21 = 0, ignored = 0;
	uint8_t frames = 0, frames_2 = 0, blinks = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->rival_warning.blinks = 2u;
		OBJECT(current_object)->rival_warning.frames = 0xCu;
		r21 = start_object();
		mem_write32(r21 + 4u, rival_alarm_update_ADDRESS);
		break;
	case 1:
		break;
	case 2:
		frames = OBJECT(current_object)->rival_warning.frames;
		OBJECT(current_object)->rival_warning.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->rival_warning.frames = 0xCu;
		}
		return;
	default:
		invalid_jump(rom_007C44[OBJECT(current_object)->state]);
	}
	frames_2 = OBJECT(current_object)->rival_warning.frames;
	OBJECT(current_object)->rival_warning.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		r19 = 0x7CDBu;
		ignored = draw_text(r19);
		return;
	}
	blinks = OBJECT(current_object)->rival_warning.blinks;
	OBJECT(current_object)->rival_warning.blinks = blinks - 1u;
	if (blinks == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->rival_warning.frames = 6u;
	frames = OBJECT(current_object)->rival_warning.frames;
	OBJECT(current_object)->rival_warning.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->state--;
		OBJECT(current_object)->rival_warning.frames = 0xCu;
	}
}

/* The sound of the rival warning: sound 2B twice, 15 frames apart. */
void rival_alarm_update(void)
{
	uint8_t r0 = 0;
	uint8_t frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = 0x2Bu;
		request_sound(r0);
		OBJECT(current_object)->state++;
		OBJECT(current_object)->rival_alarm.frames = 0xFu;
		return;
	case 1:
		frames = OBJECT(current_object)->rival_alarm.frames;
		OBJECT(current_object)->rival_alarm.frames = frames - 1u;
		if (frames == 1u)
		{
			r0 = 0x2Bu;
			request_sound(r0);
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
		}
		return;
	default:
		invalid_jump(rom_007CAC[OBJECT(current_object)->state]);
	}
}

/* Loads the crash physics of the car's model (rom_00804A): bounce_damping, speed_change_decay,
   push_decay, gravity and the address of its crash table. */
void player_car_load_crash_physics(void)
{
	uint32_t r10 = 0;

	r10 = rom_00804A[CAR(player_car)->model];
	CAR(player_car)->bounce_damping = mem_read16(r10);
	r10 += 2u;
	CAR(player_car)->speed_change_decay = mem_read16(r10);
	r10 += 2u;
	CAR(player_car)->push_decay = mem_read16(r10);
	r10 += 2u;
	CAR(player_car)->gravity = mem_read16(r10);
	CAR(player_car)->crash_table = r10 + 2u;
}

/* Starts the race's cars: for each player of the race, a race_car with its race record and the tables of
   its model (body, wheels, sprays, tag), the two race_car_occupant objects and two 0141F4 objects that
   follow it; then ends. */
void race_cars_start(void)
{
	uint8_t r3_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r10 = 0, r11 = 0, r21 = 0;

	r10 = SCREEN(screen)->unk_308 + 0x1Fu;
	r0 = mem_read8(r10);
	r10++;
	do
	{
		r21 = start_object();
		r1 = mem_read8(r10);
		mem_write32(r21 + 4u, race_car_draw_ADDRESS);
		mem_write8(r21 + 9u, r1);
		r11 = rom_00E0A0[r1] + unk_20E000_ADDRESS;
		mem_write32(r21 + 0x80u, r11);
		r2 = mem_read8(r11 + 0x49u);
		mem_write8(r21 + 0xAu, r2);
		mem_write16(r21 + 0x8Cu, rom_009DCC[r2]);
		mem_write16(r21 + 0x8Eu, rom_009DDC[r2]);
		mem_write16(r21 + 0x94u, rom_009DEC[r2]);
		mem_write16(r21 + 0x96u, rom_009E1C[r2]);
		mem_write8(r21 + 0x9Bu, rom_009E3C[r2]);
		mem_write8(r21 + 0x98u, rom_009E0C[r2]);
		mem_write16(r21 + 0xA4u, rom_009DFC[r2]);
		mem_write16(r21 + 0xA6u, rom_009E2C[r2]);
		mem_write8(r21 + 0xABu, rom_009E44[r2]);
		mem_write8(r21 + 0xA8u, rom_009E14[r2]);
		r3 = rom_18D25C[r2];
		mem_write32(r21 + 0x90u, r3);
		mem_write32(r21 + 0xA0u, r3 + 4u);
		mem_write32(r21 + 0xC0u, 0x9F2Cu);
		mem_write32(r21 + 0xC4u, r3 + 8u);
		mem_write32(r21 + 0xC8u, r11 + 0x52u);
		mem_write8(r21 + 0xCDu, 2u);
		mem_write32(r21 + 0xD0u, 0x9F4Cu);
		mem_write32(r21 + 0xD4u, r3 + 0xCu);
		mem_write32(r21 + 0xD8u, r11 + 0x51u);
		mem_write8(r21 + 0xDDu, 1u);
		r1 = mem_read8(r11 + 2u);
		mem_write16(r21 + 0xBAu, rom_009F8C[r1]);
		mem_write16(r21 + 0xB8u, rom_009F7C[r2]);
		mem_write8(r21 + 0xBCu, rom_009F9C[r1]);
		mem_write8(r21 + 0xBDu, r1 - RACE(SCREEN(screen)->race_copy)->unk_02);
		r12 = (r12 & ~0xFFFFu) | (uint16_t)r21;
		r3 = r2;
		r10++;
		r21 = start_object();
		mem_write32(r21 + 4u, rom_009EBC[r3]);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3);
		mem_write8(r21 + 0xAu, 0xFFu);
		r21 = start_object();
		mem_write32(r21 + 4u, rom_009EDC[r3]);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3 + 8u);
		mem_write8(r21 + 0xAu, 0u);
		r3_2 = (uint8_t)r2;
		r21 = start_object();
		mem_write32(r21 + 4u, rider_gesture_update_ADDRESS);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3_2);
		mem_write8(r21 + 0xAu, r2);
		mem_write8(r21 + 0xBu, mem_read8(r12 + 9u));
		r21 = start_object();
		mem_write32(r21 + 4u, rider_gesture_update_ADDRESS);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3_2 + 8u);
		mem_write8(r21 + 0xAu, r2);
		mem_write8(r21 + 0xBu, mem_read8(r12 + 9u));
		r0--;
	} while (r0 != 0u);
	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->unk_01 = 0u;
}

/* Draws a car of the race, the screen's own or a rival, from its race record (record), as the screen's
   view sees it: its world position less the view's through the view's matrix and scaled by its depth
   (off the screen, too near or too far: not drawn); its body (animation, frame, palette from the record,
   plus the screen's brake lights and wheel_turn), its wheels, the sprays at its wheels
   (race_car_draw_spray), five frames of animation 44 on its hard acceleration (record unk_53, car
   unk_145); over the screen's own car the CRASH (or Ouch!) text after a hit, over a rival its number
   tag, blinking. A rival behind the window of the road (within 256 segments) is shown small at the
   bottom of the screen, larger as it closes in and across as it is (with its tag). A body animation
   without wheels (rom_009E4C) sets special_body for the occupants. Once the race is over (screen unk_94
   bit 4) it becomes race_car_start_ending. */
void race_car_draw(void)
{
	bool L_00978F = false, L_009874 = false, L_009AAD = false, L_009B31 = false;
	uint8_t r0_2 = 0, r7_2 = 0;
	uint16_t r1 = 0, r2 = 0, r5 = 0, r6 = 0, r13_2 = 0, r17_2 = 0, r18 = 0, crash_text_y = 0, window_segment = 0;
	uint32_t r0 = 0, r1_2 = 0, r2_2 = 0, r3 = 0, r4 = 0, r5_2 = 0, r6_2 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, blink_counter = 0, car = 0, ignored = 0, race = 0, race_2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	OBJECT(current_object)->race_car.blink_counter++;
	r10 = SCREEN(screen)->unk_C0 + 0x10u;
	if (SCREEN(screen)->unk_C4 <= r10)
		return;
	window_segment = CAR(player_car)->window_segment;
	r1 = (uint16_t)(window_segment + (SCREEN(screen)->unk_12 >> 7));
	race = OBJECT(current_object)->race_car.record;
	r2 = RACE(race)->unk_42;
	if (OBJECT(current_object)->race_car.rival != 0u)
	{
		L_009B31 = r2 <= window_segment;
		if (!L_009B31)
			L_00978F = true;
	}
	if (!L_009B31)
	{
		if (!L_00978F && (SCREEN(screen)->unk_94 & 0x10u) != 0u)
			OBJECT(current_object)->handler = rom_009FC4[OBJECT(current_object)->race_car.model];
		else if (r1 <= r2)
			return;
		car = player_car + 0xDCu;
		r12 = RACE(race)->unk_30 - CAR(player_car)->view_x;
		r13 = RACE(race)->unk_34 - CAR(player_car)->view_y;
		r14 = RACE(race)->unk_38 - CAR(player_car)->view_z;
		r2_2 = r12 * CAR(car - 0xDCu)->view_matrix_xx;
		car += 4u;
		r3 = r13 * CAR(car - 0xE0u)->view_matrix_xy;
		car += 4u;
		r15 = (int32_t)(r2_2 + r14 * CAR(car - 0xE4u)->view_matrix_xz + r3) >> 14;
		car += 4u;
		r2_2 = r12 * CAR(car - 0xE8u)->view_matrix_yx;
		car += 4u;
		r3 = r13 * CAR(car - 0xECu)->view_matrix_yy;
		car += 4u;
		r16 = (int32_t)(r2_2 + r14 * CAR(car - 0xF0u)->view_matrix_yz + r3) >> 14;
		r17 = (int32_t)(r12 * CAR(car - 0xF0u)->view_matrix_zx + r14 * CAR(car - 0xF0u)->view_matrix_zz + r13 * CAR(car - 0xF0u)->view_matrix_zy) >> 14;
		if (r17 > 0x40u && r17 < 0x3000u)
		{
			r9 = rom_1A0340[r17];
			r8 = rom_19A340[r17];
			r15 = (int32_t)(r15 * r8) >> 12;
			r16 = 0u - ((int32_t)(r16 * r8) >> 12);
			L_009B31 = (int32_t)r15 > 0x120 || (int32_t)r15 < -0x120 || (int32_t)r16 > 0xF0;
			if (!L_009B31)
			{
				r8 = (uint16_t)((int16_t)r8 >> 4);
				L_009874 = true;
			}
		}
	}
	if (!L_009874 && OBJECT(current_object)->race_car.rival != 0u)
	{
		OBJECT(current_object)->race_car.special_body = 0u;
		OBJECT(current_object)->unk_E1 = 0u;
		race_2 = SCREEN(screen)->race_copy;
		r7 = RACE(race_2)->unk_42;
		r7 = (uint16_t)(r7 - RACE(race)->unk_42);
		t1 = r7 - 0x100u;
		r7 = t1;
		if ((int16_t)t1 >= 0)
			return;
		r7 = (uint16_t)(0u - r7);
		r9 = 0x191u;
		r17_2 = (uint16_t)((int16_t)((RACE(race)->unk_44 - RACE(race_2)->unk_44) * 3u) >> 2);
		if ((int16_t)r17_2 <= 0x58)
		{
			if ((int16_t)r17_2 < -0x58)
				r17_2 = 0xFFA8u;
		}
		else
			r17_2 = 0x58u;
		r18 = (uint16_t)((int16_t)(0u - RACE(race)->unk_56) >> 2);
		r8 = r7 * 3u;
		r8 = (r8 & ~0xFFFFu) | ((uint16_t)r8 >> 3);
		r12 = RACE(race)->unk_3C;
		r13 = RACE(race)->unk_3E;
		r14 = RACE(race)->unk_4A;
		race_car_put_frame(r8, r17_2, r18, &ignored, &r10, &r13);
		r18 = (uint16_t)((int16_t)(r7 * 0xFFECu) >> 8);
		r8 = r7;
		r12 = OBJECT(current_object)->race_car.tag_animation;
		r13 = OBJECT(current_object)->race_car.tag_palette;
		r14 = 0u;
		race_car_put_frame(r8, r17_2, r18, &ignored, &r10, &r13);
	}
	else
	{
		if (!L_009874)
		{
			r9 = 0x1C0u;
			r8 = 0x100u;
			r15 = 0u;
			r16 = 0u;
		}
		OBJECT(current_object)->race_car.wheel_turn += RACE(race)->unk_4C;
		r13 = (uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 16u);
		r13 = r13 & 1u;
		if ((uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 24u) != r13)
		{
			AT8(OBJECT(current_object)->race_car.wheel_turn, 3) = r13;
			AS16(OBJECT(current_object)->race_car.wheel_turn) = 0u;
		}
		r13 = (uint16_t)(r13 + AS16(CAR(player_car)->brake_lights));
		r13 = (uint16_t)(r13 + RACE(race)->unk_3E);
		r12 = RACE(race)->unk_3C;
		r14 = RACE(race)->unk_4A;
		r17_2 = (uint16_t)r15;
		r18 = (uint16_t)r16;
		OBJECT(current_object)->x = r15;
		OBJECT(current_object)->y = r16;
		OBJECT(current_object)->width_scale = r8;
		OBJECT(current_object)->depth_layer = r9;
		race_car_put_frame(r8, r17_2, r18, &r0, &r10, &r13);
		OBJECT(current_object)->unk_84 = r0;
		if (OBJECT(current_object)->race_car.special_body == 0u)
		{
			r2_2 = RACE(race)->unk_3C;
			r1_2 = (uint16_t)(r2_2 - (uint16_t)(OBJECT(current_object)->unk_8C >> 16u));
			if (rom_009E4C[r1_2] != 0u)
			{
				CAR(player_car)->unk_137 = 0u;
				r27 = current_object + 0x90u;
				r28 = current_object + 0xA0u;
				if ((OBJECT(current_object)->race_car.model != 6u || rom_009E4C[r1_2] == 2u) && (r0 & 0x40u) == 0u)
				{
					t2 = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | t2;
				}
				if (RACE(race)->unk_4C == 0u)
				{
					r14 = 0u;
					r7 = 0u;
				}
				else
				{
					r14 = mem_read8(r27 + 0xAu);
					r7 = mem_read8(r28 + 0xAu);
					if (mem_read8(r27 + 0xBu) <= r14)
						r14 = 1u;
					else
						r14 = (uint8_t)(r14 + 1u);
					if (mem_read8(r28 + 0xBu) <= r7)
						r7 = 1u;
					else
						r7 = (uint8_t)(r7 + 1u);
				}
				mem_write8(r27 + 0xAu, r14);
				mem_write8(r28 + 0xAu, r7);
				r11 = mem_read32(rom_034138[r2_2] + 4u + RACE(race)->unk_4A * 4u) + 2u;
				r3 = mem_read16(r11);
				r11 += 2u;
				r3 = r3 & 0x40u;
				r3 = r3 >> 6;
				r3 = (uint16_t)(r3 * 0x34u);
				r3 = (uint16_t)(r3 + (uint16_t)(mem_read16(r11) - (uint16_t)OBJECT(current_object)->unk_8C) / 3u);
				AT8(OBJECT(current_object)->unk_E2, 1) = r3;
				r4 = (uint16_t)(r3 << 4);
				r11 += 2u;
				r6 = mem_read16(r11);
				r5 = mem_read16(r11 + 2u);
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->unk_E1;
				if ((r12 & mem_read8(r27 + 8u)) == 0u)
				{
					r11 = mem_read32(r27);
					r13 = mem_read16(r27 + 6u);
					r12 = mem_read16(r27 + 4u) + r3;
					r10 = race_car_draw_part(r4, r5, r6, r8, r10, r11, r13, (uint16_t)r15, (uint16_t)r16);
				}
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->unk_E1;
				if ((r12 & mem_read8(r28 + 8u)) == 0u)
				{
					r14 = r7;
					r11 = mem_read32(r28);
					r13 = mem_read16(r28 + 6u);
					r12 = mem_read16(r28 + 4u) + r3;
					r10 = race_car_draw_part(r4, r5, r6, r8, r10, r11, r13, (uint16_t)r15, (uint16_t)r16);
				}
				r27 = current_object + 0xC0u;
				r28 = current_object + 0xD0u;
				if ((OBJECT(current_object)->unk_84 & 0x40u) == 0u)
				{
					t3 = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | t3;
				}
				r7_2 = RACE(race)->unk_50;
				r10 = race_car_draw_spray(r4, r5, r6, r7_2, r8, r10, (uint16_t)r15, (uint16_t)r16);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)r28;
				r10 = race_car_draw_spray(r4, r5, r6, r7_2, r8, r10, (uint16_t)r15, (uint16_t)r16);
				r0_2 = OBJECT(current_object)->unk_BF;
				if (r0_2 != 0u || RACE(race)->unk_53 != 0u)
				{
					r14 = r0_2;
					r0_2++;
					if (r0_2 >= 5u)
						r0_2 = 0u;
					OBJECT(current_object)->unk_BF = r0_2;
					r12 = 0x44u;
					r13 = 0x58u;
					r17_2 = (uint16_t)r15;
					r18 = (uint16_t)r16;
					race_car_put_frame(r8, r17_2, r18, &ignored, &r10, &r13);
					race_car_put_sprite(r8, r17_2, r18, &ignored, &r10, &r13);
				}
			}
			else
			{
				OBJECT(current_object)->race_car.special_body = 0xFFu;
				if (OBJECT(current_object)->race_car.rival == 0u)
					CAR(player_car)->unk_137 = 0xFFu;
			}
		}
		do
		{
			if (L_009AAD || SCREEN(screen)->unk_31C != 0u || OBJECT(CAR(player_car)->object)->player_car.timer < 0x3Cu)
			{
				if (L_009AAD || OBJECT(current_object)->race_car.rival == 0u)
				{
					L_009AAD = false;
					if (OBJECT(current_object)->race_car.crash_text_frames == 0u)
					{
						if (CAR(player_car)->crash_text == 0u)
						{
							r4 = screen + 0x5000u + r9 * 4u;
							if (mem_read16(r4) == 0u)
								mem_write16(r4 + 2u, r10);
							r5_2 = SCREEN(screen)->unk_C0 + 0x10u;
							r6_2 = mem_read16(r4);
							mem_write32(r10, r6_2);
							mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
							mem_write16(r4, (r5_2 >> 4) | 0x8000u);
							SCREEN(screen)->unk_C0 = r10;
							return;
						}
						CAR(player_car)->crash_text = 0u;
						OBJECT(current_object)->race_car.crash_text_rise = 0u;
						OBJECT(current_object)->race_car.crash_text_frames = 0x1Eu;
						OBJECT(current_object)->race_car.crash_text_y = r16;
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
					r13 = OBJECT(current_object)->race_car.crash_text_palette;
					r14 = OBJECT(current_object)->race_car.crash_text_rise;
					r14 = r14 >> 1;
					r17_2 = (uint16_t)r15;
					crash_text_y = (uint16_t)OBJECT(current_object)->race_car.crash_text_y;
					r8 = 0x100u;
					race_car_put_frame(r8, r17_2, crash_text_y, &ignored, &r10, &r13);
					OBJECT(current_object)->race_car.crash_text_rise++;
					OBJECT(current_object)->race_car.crash_text_rise &= 7u;
				}
				else
				{
					blink_counter = OBJECT(current_object)->race_car.blink_counter;
					if (rom_009FA4[blink_counter & 0x1Fu] != 0u)
					{
						r13_2 = mem_read16(rom_032BA4[OBJECT(current_object)->race_car.tag_palette]);
						r12 = 0x1Bu;
						r17_2 = (uint16_t)(r15 - 3u);
						r18 = (uint16_t)((uint16_t)r16 - ((0x30u * r8) >> 8));
						r10 = race_car_put_flat_sprite(r10, r13_2, r17_2, r18);
						r12 = OBJECT(current_object)->race_car.tag_animation;
						r18 -= 0x16u;
						r17_2 += 0x15u;
						r10 = race_car_put_flat_sprite(r10, r13_2, r17_2, r18);
					}
				}
			}
			else
			{
				L_009AAD = OBJECT(current_object)->race_car.rival != 0u;
				if (!L_009AAD)
				{
					r13_2 = mem_read16(rom_032BA4[OBJECT(current_object)->race_car.tag_palette]);
					r12 = 0x1Bu;
					r17_2 = (uint16_t)(r15 - 3u);
					r18 = (uint16_t)((uint16_t)r16 - ((0x30u * r8) >> 8));
					r10 = race_car_put_flat_sprite(r10, r13_2, r17_2, r18);
					r12 = OBJECT(current_object)->race_car.tag_animation;
					r18 -= 0x16u;
					r17_2 += 0x15u;
					r10 = race_car_put_flat_sprite(r10, r13_2, r17_2, r18);
				}
			}
		} while (L_009AAD);
	}
	r4 = screen + 0x5000u + r9 * 4u;
	if (mem_read16(r4) == 0u)
		mem_write16(r4 + 2u, r10);
	r5_2 = SCREEN(screen)->unk_C0 + 0x10u;
	r6_2 = mem_read16(r4);
	mem_write32(r10, r6_2);
	mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
	mem_write16(r4, (r5_2 >> 4) | 0x8000u);
	SCREEN(screen)->unk_C0 = r10;
}

/* Draws the spray at one side's wheels (dust, smoke, water...: the record's kind, from the race record's
   wheel effects), cycling its frames. */
uint32_t race_car_draw_spray(uint32_t r4, uint16_t r5, uint16_t r6, uint8_t r7, uint32_t r8, uint32_t r10, uint16_t r15, uint16_t r16)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r11 = 0, r13 = 0;

	r1 = mem_read8(r27 + 0xCu);
	if (r1 == 0u && (r7 & mem_read8(r27 + 0xDu)) == 0u)
	{
		mem_write8(r27 + 0xCu, 0u);
		mem_write8(r27 + 0xEu, 0u);
		return r10;
	}
	r0 = mem_read8(r27 + 0xEu);
	r2 = mem_read8(mem_read32(r27 + 8u));
	if (r2 != 0u && r2 != r0)
	{
		r0 = r2;
		mem_write8(r27 + 0xEu, r2);
		r1 = 0u;
	}
	r14 = r1;
	r1 = (uint8_t)(r1 + 1u);
	if (rom_009F6C[r0] <= r1)
		r1 = 0u;
	mem_write8(r27 + 0xCu, r1);
	r11 = mem_read32(r27 + 4u);
	r12 = mem_read16(mem_read32(r27) + r0 * 2u);
	r13 = rom_009F0C[r0];
	return race_car_draw_part(r4, r5, r6, r8, r10, r11, r13, r15, r16);
}

/* Draws a part of the car (a wheel, a spray) at its offset from the car's place, scaled with the car. */
uint32_t race_car_draw_part(uint32_t r4, uint16_t r5, uint16_t r6, uint32_t r8, uint32_t r10, uint32_t r11, uint32_t r13, uint16_t r15, uint16_t r16)
{
	uint32_t carry = 0;
	uint16_t r17 = 0, r18 = 0;
	uint32_t ignored = 0;

	r17 = (uint16_t)r5;
	r18 = (uint16_t)r6;
	r11 += r4;
	if (mem_read32(r11) == 0u)
		return r10;
	r17 = (uint16_t)((r17 + mem_read16(r11)) * r8);
	r18 = (uint16_t)((r18 + mem_read16(r11 + 2u)) * r8);
	carry = (r17 >> 7) & 1u;
	r17 = (uint16_t)(((int16_t)r17 >> 8) + carry);
	carry = (r18 >> 7) & 1u;
	r17 += r15;
	r18 = (uint16_t)(((int16_t)r18 >> 8) + carry + r16);
	race_car_put_frame(r8, r17, r18, &ignored, &r10, &r13);
	return r10;
}

/* Puts the first piece of frame R14 of animation R12 with palette R13 into the sprite list, scaled by R8
   at R17, R18 (race_car_put_sprite). */
void race_car_put_frame(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13)
{
	uint32_t r0 = 0;

	r12 = mem_read32(rom_034138[r12] + 4u + r14 * 4u) + 2u;
	*p_r13 = rom_032BA4[*p_r13];
	race_car_put_sprite(r8, r17, r18, &r0, p_r10, p_r13);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

/* Writes one sprite of a frame's piece into the screen's sprite list (and its copy at + 3000), its size
   and offsets scaled by R8, at R17, R18. */
void race_car_put_sprite(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13)
{
	uint32_t r10 = *p_r10;
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	r0 = mem_read16(r12);
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = 0x122B74u + mem_read16(r12) * 4u;
	t1 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t1;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t2 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t2;
	t3 = ROM16(r14);
	r14 = r14 + 2u;
	t4 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = (((t3 * r8) >> 8) & 0x3FFu) | t4;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	t5 = ROM16(r14);
	r14 = r14 + 2u;
	t6 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = (((t5 * r8) >> 8) & 0x3FFu) | t6 | SCREEN(screen)->unk_1A;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	r12 = r12 + 2u;
	r2 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r8) >> 8) + r18);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	r12 = r12 + 2u;
	r2 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r8) >> 8) + r17);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t7 = mem_read16(*p_r13);
	*p_r13 += 2u;
	r2 = t7;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	SCREEN(screen)->unk_B0++;
	r12 = r12 + 2u;
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0;
	*p_r10 = r10;
}

/* Writes the first piece of animation R12 into the sprite list at R17, R18 at its own size (the number
   tags). */
uint32_t race_car_put_flat_sprite(uint32_t r10, uint16_t r13, uint16_t r17, uint16_t r18)
{
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;

	r12 = mem_read32(rom_034138[r12] + 4u) + 2u;
	r0 = mem_read16(r12);
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = 0x122B74u + mem_read16(r12) * 4u;
	t1 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t1;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t2 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t2;
	t3 = ROM16(r14);
	r14 = r14 + 2u;
	t4 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = t3 | t4;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	t5 = ROM16(r14);
	r14 = r14 + 2u;
	t6 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = t5 | t6 | SCREEN(screen)->unk_1A;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r18);
	mem_write16(r10, r18);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r17);
	mem_write16(r10, r17);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r13);
	mem_write16(r10, r13);
	r10 += 2u;
	SCREEN(screen)->unk_B0++;
	r12 = r12 + 2u;
	return r10;
}

/* Loads the car's model and transmission: its handling record (car unk_58), its torque curves (unk_50),
   its gear ratios (unk_54) and top gear (unk_63), the first gear's ratio (unk_64), and the roadside
   surfaces of the first stage (player_car_next_surfaces). */
void player_car_load_gears(void)
{
	uint32_t r1 = 0, model = 0;

	model = CAR(player_car)->model;
	r12 = rom_00ADCC[model];
	r1 = CAR(player_car)->transmission + model * 2u;
	CAR(player_car)->torque_curves = rom_00B0C0[r1];
	r1 = rom_00B04C[r1];
	CAR(player_car)->top_gear = mem_read8(r1);
	r1++;
	CAR(player_car)->gear_ratios = r1;
	CAR(player_car)->gear_ratio = mem_read8(r1);
	CAR(player_car)->handling = r12;
	player_car_next_surfaces();
}

/* Builds the drive force of each gear by speed at screen + 1300 (256 values a gear), interpolating the
   model's points (car unk_50). */
void player_car_build_torque_curves(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r10 = screen + 0x1300u;
	r11 = CAR(player_car)->torque_curves;
	r9 = CAR(player_car)->top_gear;
	r9 = (uint8_t)(r9 + 1u);
	do
	{
		r12 = r11 + 4u;
		do
		{
			r0 = ROM16(r11);
			r11 += 2u;
			t1 = ROM16(r12);
			r12 = r12 + 2u;
			t2 = ROM16(r12);
			r12 = r12 + 2u;
			r1 = (uint32_t)ROM16(r11) << 16;
			r2 = t1 - r0;
			r3 = divide_s32(((uint32_t)t2 << 16) - r1, r2);
			r11 += 2u;
			do
			{
				mem_write16(r10, r1 >> 16);
				r1 += r3;
				r2--;
				r10 += 2u;
			} while (r2 != 0u);
		} while ((int16_t)ROM16(r12) >= 0);
		r11 = r12 + 4u;
		r9--;
	} while (r9 != 0u);
}

/* Takes the controls: gas, brake and wheel from the screen's pedals and wheel (screen unk_101, unk_102,
   unk_100). In the demo (screen unk_90 bit 8) the car drives itself: no brake, the gas up to F0 (eased
   off while skidding), the wheel back toward the middle when off the road or slowed, else against its
   sideways drift. */
void player_car_read_controls(void)
{
	uint8_t gas = 0, wheel = 0;

	if ((SCREEN(screen)->unk_90 & 0x100u) == 0u)
	{
		OBJECT(current_object)->player_car.gas = SCREEN(screen)->unk_101;
		OBJECT(current_object)->player_car.brake = SCREEN(screen)->unk_102;
		OBJECT(current_object)->player_car.wheel = SCREEN(screen)->unk_100;
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

/* Moves the car for the frame: the drag (player_car_road_drag), the slope of its segment (race unk_54),
   the brake (no gas while braking; brake lights car unk_7E) and the gas through the gear's torque curve
   go to player_car_engine (speed, revs); the wheel steers through the model's table (less at low speed)
   into steering; the road's turn under it (previous_heading less heading) pushes it out (less with
   catch-up, car unk_6A); beyond the model's grip it skids (car unk_67, slide, tyre noise 3, smoke on
   both sides); sideways_speed and sideways_push move it across (car unk_38, within the road's half width
   + 60); the speed along (less its sideways angle, vector_angle) plus a catch-up bonus by the distance
   to the car ahead (car unk_68, unk_6A; not once finished) moves it along, passing segments (car unk_42,
   unk_44, unk_40, unk_A4, unk_34, unk_32); then player_car_world_position. In a replay (unk_20E623) the
   race record gives all this. */
void player_car_drive(uint32_t *p_r11, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r17)
{
	uint32_t r11 = *p_r11;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t r0_2 = 0;
	uint16_t r2 = 0, r3 = 0, r6_2 = 0, r7_2 = 0, r8_2 = 0, road_x = 0;
	uint32_t r0 = 0, r1 = 0, r2_2 = 0, r3_2 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r13 = 0, r15 = 0, r17 = 0, gear = 0, gear_ratio = 0, speed = 0, speed_2 = 0, wheel = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

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
	r0 = CAR(player_car)->speed;
	if (r0 != 0u)
	{
		r0 = (r0 * r0) >> 16;
		road_x = (uint16_t)CAR(player_car)->road_x;
		r2 = mem_read16(r11 + 0x12u);
		r3 = ROM16(r12 + 0x1Au);
		r0 = player_car_road_drag(r0, road_x, r2, r3);
	}
	r7 = mem_read16(r11 + 2u);
	RACE(SCREEN(screen)->race)->unk_54 = r7;
	r7 = ((int32_t)(int16_t)rom_100C00[r7] >> 1) - (r0 + 0x1800u);
	r8 = OBJECT(current_object)->player_car.gas;
	AS16(CAR(player_car)->brake_lights) = 0u;
	r9 = rom_00ACCB[OBJECT(current_object)->player_car.brake];
	if (r9 != 0u)
	{
		r8 = 0u;
		AS16(CAR(player_car)->brake_lights) = 2u;
	}
	OBJECT(current_object)->player_car.brake_force = r9;
	r9 = r9 * ROM32(r12 + 0x1Cu);
	gear = CAR(player_car)->gear;
	r2_2 = screen + 0x1300u + (uint16_t)(gear << 9);
	speed = CAR(player_car)->speed;
	r8 = (int32_t)((uint32_t)(int8_t)rom_00ABCB[r8] * mem_read16(r2_2 + (speed >> 8) * 2u)) >> 6;
	gear_ratio = CAR(player_car)->gear_ratio;
	player_car_engine(r7, r8, r9, gear_ratio);
	if (unk_20E623 != 0u)
		CAR(player_car)->speed = RACE(SCREEN(screen)->race_copy)->unk_4C;
	speed_2 = CAR(player_car)->speed;
	OBJECT(current_object)->player_car.speed = speed_2;
	wheel = OBJECT(current_object)->player_car.wheel;
	r0 = (int32_t)((int8_t)mem_read8(ROM32(r12) + (wheel >> 3)) * 0x14u) / 0x10;
	if (speed_2 <= 0xA000u)
	{
		if (speed_2 > 0x2000u)
			r0 = (int32_t)(r0 * (speed_2 + 0x6000u)) / 0x10000;
		else
			r0 = (int32_t)(r0 * speed_2) / 0x4000;
	}
	OBJECT(current_object)->player_car.steering = r0;
	r8_2 = (uint16_t)((int16_t)((((uint32_t)OBJECT(current_object)->player_car.previous_heading << 4) - ((uint32_t)OBJECT(current_object)->player_car.heading << 4)) * 0x20u) / 0x100);
	if (OBJECT(current_object)->state == 5u)
		r8_2 = (uint16_t)((int16_t)r8_2 >> 4);
	r8_2 = (uint16_t)divide_s32((uint32_t)(int16_t)r8_2 << 8, CAR(player_car)->catch_up + 0x100u);
	CAR(player_car)->skidding = 0u;
	r1 = OBJECT(current_object)->player_car.speed * r0;
	r2_2 = ROM32(r12 + 0x20u);
	if ((int32_t)r2_2 <= (int32_t)r1)
	{
		CAR(player_car)->skidding = 0xFFu;
		r1 = (int32_t)((r1 - r2_2) * ROM32(r12 + 0x24u)) / -0x400000;
		OBJECT(current_object)->player_car.slide = r1;
		r8_2 = (uint16_t)(r8_2 + r1 + r0);
		if (CAR(player_car)->wheel_effect_sides == 0u)
		{
			CAR(player_car)->wheel_effect_1 = 3u;
			CAR(player_car)->wheel_effect_2 = 3u;
			CAR(player_car)->wheel_effect_sides = 3u;
		}
		r0_2 = 3u;
		set_tyre_noise_on_road(r0_2);
		OBJECT(current_object)->player_car.sideways_speed = r8_2;
		r0 = r8_2;
	}
	else
	{
		r2_2 = 0u - r2_2;
		if ((int32_t)r1 <= (int32_t)r2_2)
		{
			CAR(player_car)->skidding = 0xFFu;
			r1 = (int32_t)((r1 - r2_2) * ROM32(r12 + 0x24u)) / -0x400000;
			OBJECT(current_object)->player_car.slide = r1;
			r8_2 = (uint16_t)(r8_2 + r1 + r0);
			if (CAR(player_car)->wheel_effect_sides == 0u)
			{
				CAR(player_car)->wheel_effect_1 = 3u;
				CAR(player_car)->wheel_effect_2 = 3u;
				CAR(player_car)->wheel_effect_sides = 3u;
			}
			r0_2 = 3u;
			set_tyre_noise_on_road(r0_2);
			OBJECT(current_object)->player_car.sideways_speed = r8_2;
			r0 = r8_2;
		}
		else
		{
			r8_2 += r0;
			if ((SCREEN(screen)->unk_90 & 0x100u) != 0u || RACE(SCREEN(screen)->race_copy)->unk_5E != 0u)
			{
				r0 = (uint16_t)OBJECT(current_object)->player_car.sideways_speed;
				t1 = r8_2;
				r8_2 = (uint16_t)(t1 - r0);
				if (t1 != r0)
					r0 = (uint16_t)(r0 + ((int16_t)(r8_2 * 1u) / 4 + (shift_signed_by(r8_2, 0xF0u, 16) * 2u + 1u)));
				OBJECT(current_object)->player_car.sideways_speed = r0;
			}
			else
			{
				r0 = r8_2;
				OBJECT(current_object)->player_car.sideways_speed = r0;
			}
		}
	}
	if (unk_20E623 != 0u)
	{
		r0 = RACE(SCREEN(screen)->race_copy)->unk_4E;
		OBJECT(current_object)->player_car.sideways_speed = r0;
	}
	r0 = (uint32_t)(int16_t)r0 << 8;
	speed = OBJECT(current_object)->player_car.speed;
	r9 = speed;
	r2_2 = 0u;
	r3_2 = 0u;
	r0 = vector_angle(r0, speed, r2_2, r3_2);
	r9 = (r9 * (int16_t)rom_101400[r0]) >> 14;
	r15 = CAR(player_car)->segment_progress + r9;
	if (OBJECT(current_object)->player_car.finished == 0u)
	{
		r7_2 = (uint16_t)(mem_read16(SCREEN(screen)->unk_68 + 0x42u) - RACE(SCREEN(screen)->race_copy)->unk_42);
		if ((int16_t)r7_2 > 0x20)
			r7_2 = 0x20u;
		t2 = r7_2;
		r7_2 = (uint16_t)(t2 - 1u);
		if ((int16_t)t2 < 1)
			r7_2 = 0u;
		CAR(player_car)->catch_up_target = r7_2;
		r6 = CAR(player_car)->catch_up;
		t3 = r7_2 - r6;
		overflow = (uint32_t)(int16_t)r7_2 - (int16_t)r6 != (uint32_t)(int16_t)(r7_2 - r6);
		zero = t3 == 0u;
		negative = t3 >> 15;
		if (r7_2 != r6)
		{
			if (((negative ^ overflow) | zero) == 0u)
				r6 = (uint16_t)(r6 + 1u);
			else
				r6 = (uint16_t)(r6 - 1u);
		}
		CAR(player_car)->catch_up = r6;
		r15 += (r6 * r9) >> 7;
	}
	if (unk_20E623 != 0u)
	{
		r15 = RACE(SCREEN(screen)->race_copy)->unk_40;
		r6_2 = RACE(SCREEN(screen)->race_copy)->unk_42;
		r13 = (int16_t)RACE(SCREEN(screen)->race_copy)->unk_44;
		CAR(player_car)->segment_fraction = r15;
		CAR(player_car)->segment_progress = r15;
		CAR(player_car)->segment_progress = CAR(player_car)->segment_progress << 8;
		CAR(player_car)->segment = r6_2;
		r6_2 = (r6_2 * 0x20u) & 0xFFFu;
		CAR(player_car)->segment_offset = r6_2;
		r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + r6_2);
		r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
		r17 = mem_read16(r11);
		r17 = (uint16_t)(r17 - mem_read16(r12));
		r17 = (int16_t)rom_100C00[r17 & 0xFFFu] * r13;
		carry = (r17 >> 5) & 1u;
		r17 = (int32_t)(((int32_t)r17 >> 6) + 0x8000u + carry) >> 8;
		r6_2 = RACE(SCREEN(screen)->race_copy)->unk_46;
		CAR(player_car)->window_segment = r6_2;
		CAR(player_car)->window_offset = (r6_2 * 0x20u) & 0xFFFu;
	}
	else
	{
		r13 = (int16_t)(CAR(player_car)->road_x + OBJECT(current_object)->player_car.sideways_speed + OBJECT(current_object)->player_car.sideways_push);
		CAR(player_car)->road_x = r13;
		for (;;)
		{
			r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
			r17 = mem_read16(r11);
			r17 = (uint16_t)(r17 - mem_read16(r12));
			r17 = (int16_t)rom_100C00[r17 & 0xFFFu] * r13;
			carry = (r17 >> 5) & 1u;
			r17 = ((int32_t)r17 >> 6) + 0x8000u + carry;
			if ((int32_t)r15 < (int32_t)r17)
				break;
			r15 -= r17;
			r11 = r12;
			CAR(player_car)->window_segment++;
			CAR(player_car)->segment++;
			CAR(player_car)->window_offset += 0x20u;
			CAR(player_car)->window_offset &= 0xAFFFu;
			CAR(player_car)->segment_offset += 0x20u;
			CAR(player_car)->segment_offset &= 0xAFFFu;
		}
		CAR(player_car)->segment_progress = r15;
		r15 = (int32_t)r15 >> 8;
		r17 = (int32_t)r17 >> 8;
		CAR(player_car)->segment_fraction = r15;
	}
	if (unk_20E623 != 0u)
	{
		CAR(player_car)->road_x = r13;
		player_car_world_position(r11, r13, r15, r17);
		*p_r11 = r11;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r17 = r17;
		return;
	}
	r6_2 = mem_read16(r11 + 0x12u);
	r7_2 = (uint16_t)(0u - r6_2);
	r6_2 += 0x60u;
	r7_2 -= 0x60u;
	if ((int16_t)r13 < (int16_t)r6_2)
	{
		if ((int16_t)r7_2 < (int16_t)r13)
		{
			player_car_world_position(r11, r13, r15, r17);
			*p_r11 = r11;
			*p_r13 = r13;
			*p_r15 = r15;
			*p_r17 = r17;
			return;
		}
		r13 = (r13 & ~0xFFFFu) | r7_2;
		CAR(player_car)->road_x = r13;
		player_car_world_position(r11, r13, r15, r17);
		*p_r11 = r11;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r17 = r17;
		return;
	}
	r13 = (r13 & ~0xFFFFu) | r6_2;
	CAR(player_car)->road_x = r13;
	player_car_world_position(r11, r13, r15, r17);
	*p_r11 = r11;
	*p_r13 = r13;
	*p_r15 = r15;
	*p_r17 = r17;
}

/* Places the car in the world from its segment: world_x and world_z from the segment's position and the
   car's place across and along it turned by the road's heading and pitch (rotate_offset), world_y the
   road's height there plus bounce_height (the old one to previous_world_y); the rest of the segment (car
   unk_3A) and the road's heading under the car (heading, the old one to previous_heading). */
void player_car_world_position(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17)
{
	uint16_t heading = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r18 = 0, world_y = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	if (unk_20E623 == 0u)
	{
		r0 = 0u;
		r1 = r13 << 7;
		r2 = r15 << 7;
		r4 = mem_read16(r11 + 2u);
		r3 = mem_read16(r11);
		rotate_offset(r0, r3, r4, &r1, &r2);
		OBJECT(current_object)->player_car.world_x = (int32_t)(r1 * 2u + (mem_read32(r11 + 4u) << 8)) >> 8;
		OBJECT(current_object)->player_car.world_z = (int32_t)(r2 * 2u + (mem_read32(r11 + 8u) << 8)) >> 8;
		r18 = r17 - r15;
		r1 = divide_s32(mem_read32(r12 + 0xCu) * r15 + mem_read32(r11 + 0xCu) * r18, r17) + OBJECT(current_object)->player_car.bounce_height;
	}
	else
	{
		OBJECT(current_object)->player_car.world_x = RACE(SCREEN(screen)->race_copy)->unk_30;
		r1 = RACE(SCREEN(screen)->race_copy)->unk_34 + (int16_t)RACE(SCREEN(screen)->race_copy)->unk_56;
		OBJECT(current_object)->player_car.world_z = RACE(SCREEN(screen)->race_copy)->unk_38;
		r18 = r17 - r15;
	}
	t1 = r1;
	world_y = OBJECT(current_object)->player_car.world_y;
	OBJECT(current_object)->player_car.world_y = t1;
	OBJECT(current_object)->player_car.previous_world_y = world_y;
	CAR(player_car)->segment_remaining = r18;
	r0 = (uint16_t)((uint32_t)mem_read16(r11) << 4);
	r1 = (uint16_t)((uint32_t)mem_read16(r12) << 4);
	r2 = r0;
	switch (r2 >> 14)
	{
	case 0:
	case 3:
		r0 = (int16_t)r0;
		r1 = (int16_t)r1;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(rom_00A5F4[r2 >> 14]);
	}
	t2 = ((int32_t)divide_s32(r0 * r18 + r1 * r15, r17) >> 4) & 0xFFFu;
	heading = OBJECT(current_object)->player_car.heading;
	OBJECT(current_object)->player_car.heading = t2;
	OBJECT(current_object)->player_car.previous_heading = heading;
}

/* Places the view behind the car: a point on the road some way back of the car (the nod, the intro, the
   rival and road-object pulls: car unk_100, unk_10C, unk_110, unk_108) and across (unk_106) and above
   (unk_104) it (view_target_x/y/z), looking toward the road twelve segments on (vector_angles: car
   unk_2E the yaw, unk_30 the road's pitch; car unk_2C the pitch with the view's tilt), the eye screen
   unk_10 back from the target along those angles (angles_to_offset) in car unk_20-28. */
void player_car_place_view(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17)
{
	uint16_t r2_2 = 0, r8 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r9 = 0, r18 = 0;

	r0 = (uint32_t)(int16_t)CAR(player_car)->view_shift_rival + (int16_t)CAR(player_car)->view_shift_lift;
	if ((int32_t)r0 >= 0x30)
		r0 = 0x30u;
	if ((int32_t)r0 <= -0xC0)
		r0 = 0xFFFFFF40u;
	r0 = r0 + (int16_t)CAR(player_car)->view_shift_intro + (int16_t)CAR(player_car)->view_shift_nod;
	r9 = ((int32_t)r0 >> 2) & ~0x1Fu;
	r11 += r9;
	r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 | 0x2000u);
	r11 = (r11 & ~0xFFFFu) | (r11 & 0xAFFFu);
	r12 = r12 + r9;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x2000u);
	r15 += r0 & 0x7Fu;
	r0 = 0u;
	r1 = (r13 + (int16_t)CAR(player_car)->view_side) << 7;
	r2 = r15 << 7;
	r4 = mem_read16(r11 + 2u);
	r3 = mem_read16(r11);
	rotate_offset(r0, r3, r4, &r1, &r2);
	OBJECT(current_object)->player_car.view_target_x = ((int32_t)(r1 * 2u) >> 8) + mem_read32(r11 + 4u);
	OBJECT(current_object)->player_car.view_target_z = ((int32_t)(r2 * 2u) >> 8) + mem_read32(r11 + 8u);
	r18 = r17 - r15;
	OBJECT(current_object)->player_car.view_target_y = divide_s32(mem_read32(((r12 & ~0xFFFFu) | (r12 & 0xAFFFu)) + 0xCu) * r15 + mem_read32(r11 + 0xCu) * r18, r17) + (int16_t)CAR(player_car)->view_rise;
	r12 = ((r11 + 0x180u) & ~0xFFFFu) | ((r11 + 0x180u) & 0xAFFFu);
	r0 = mem_read32(r12 + 4u);
	r1 = mem_read32(r12 + 0xCu);
	r2 = mem_read32(r12 + 8u);
	r3 = mem_read32(r11 + 4u);
	r4 = mem_read32(r11 + 0xCu);
	r5 = mem_read32(r11 + 8u);
	vector_angles(r2, r3, r4, r5, &r0, &r1);
	r11 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
	r12 = ((r11 + 0x180u) & ~0xFFFFu) | ((r11 + 0x180u) & 0xAFFFu);
	r8 = (uint16_t)r0;
	r9 = r1;
	r0 = mem_read32(r12 + 4u);
	r1 = mem_read32(r12 + 0xCu);
	r2 = mem_read32(r12 + 8u);
	r3 = mem_read32(r11 + 4u);
	r4 = mem_read32(r11 + 0xCu);
	r5 = mem_read32(r11 + 8u);
	vector_angles(r2, r3, r4, r5, &r0, &r1);
	r0 = (uint16_t)(r0 << 4);
	r1 = (uint16_t)(r1 << 4);
	r8 = (uint16_t)((uint32_t)r8 << 4);
	r9 = (uint16_t)(r9 << 4);
	r2 = r1;
	switch (r2 >> 14)
	{
	case 0:
	case 3:
		r1 = (int16_t)r1;
		r9 = (int16_t)r9;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(rom_00A798[r2 >> 14]);
	}
	r1 = ((int32_t)divide_s32(r1 * r15 + r9 * r18, r17) >> 4) & 0xFFFu;
	r0 = ((int32_t)divide_s32((int16_t)r0 * r15 + (int16_t)r8 * r18, r17) >> 4) & 0xFFFu;
	CAR(player_car)->view_yaw = r1;
	CAR(player_car)->road_pitch = r0;
	r2_2 = (uint16_t)(CAR(player_car)->view_pitch_nod + CAR(player_car)->view_pitch_rival + CAR(player_car)->view_pitch_lift + SCREEN(screen)->unk_18);
	if ((int16_t)r2_2 > 0x200)
		r2_2 = 0x200u;
	if ((int16_t)r2_2 < 0xC0)
		r2_2 = 0xC0u;
	r0 = (uint16_t)(r0 + r2_2 + CAR(player_car)->view_pitch_intro) & 0xFFFu;
	CAR(player_car)->view_pitch = r0;
	r2 = SCREEN(screen)->unk_10;
	r0 = (uint16_t)(r0 + 0x800u);
	r0 = r0 & 0xFFFu;
	angles_to_offset(r0, r1, r2, &r3, &r4, &r5);
	r4 += OBJECT(current_object)->player_car.view_target_y;
	r5 += OBJECT(current_object)->player_car.view_target_z;
	CAR(player_car)->view_x = r3 + OBJECT(current_object)->player_car.view_target_x;
	CAR(player_car)->view_y = r4;
	CAR(player_car)->view_z = r5;
}

/* The engine and the speed: the force (brake, slope and drag, drive; acceleration) changes the speed
   (car unk_74, also impact_speed), with any speed_change from a hit, up to the top speed of the gear
   (its ratio, car unk_64); a force past the model's grip spins the wheels, and hard braking locks them
   (car unk_65: tyre noise 1 or the model's skid noise, smoke); the revs (car unk_70) follow the speed
   through the gear's ratio, or the gas when the car stands; car unk_66 says it moves. */
void player_car_engine(uint32_t r7, uint32_t r8, uint32_t r9, uint32_t r19)
{
	bool L_00A8A3 = false, L_00A8E6 = false, L_00A914 = false, L_00A9E9 = false;
	uint32_t negative = 0, overflow = 0;
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r18 = 0, speed = 0;
	uint32_t speed_change = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, speed_change_2 = 0, t6 = 0, t7 = 0, t8 = 0;

	r9 = (int32_t)(r9 + r7 + r8) >> 6;
	OBJECT(current_object)->player_car.acceleration = r9;
	r18 = (r19 * 0xFFFFu) >> 8;
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
								if (L_00A8E6 || L_00A914 || L_00A914 || L_00A914 || (int32_t)mem_read32(r12 + 0xCu) <= (int32_t)r9)
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
													r0 = 1u;
													set_tyre_noise_on_road(r0);
												}
												r9 = mem_read32(r12 + 0x14u);
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
									speed_change = OBJECT(current_object)->player_car.speed_change;
									t1 = speed + speed_change;
									overflow = v60_overflow_add32(speed, speed_change, 0u);
									negative = t1 >> 31;
									r0_2 = t1;
									if (negative ^ overflow)
										r0_2 = 0u;
									r1 = r0_2 + r9;
									if ((int32_t)r1 <= (int32_t)r18)
									{
										if ((int32_t)r1 < 0)
											r1 = 0u;
										t2 = r1;
										r1 = t2 - r0_2;
										if (t2 != r0_2)
											r0_2 += (int32_t)(r1 * 1u) / 2 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
									}
									else
									{
										t3 = r18;
										r1 = t3 - r0_2;
										if (t3 != r0_2)
											r0_2 += (int32_t)(r1 * 1u) / 0x1E + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
										if (r0_2 >= 0xFFFFu)
											r0_2 = 0xFFFFu;
									}
									CAR(player_car)->speed = r0_2;
									OBJECT(current_object)->player_car.impact_speed = r0_2;
									if (r0_2 == 0u)
									{
										CAR(player_car)->moving = 0u;
										r0_2 = (uint32_t)OBJECT(current_object)->player_car.gas << 8;
										r1 = CAR(player_car)->revs;
										t4 = r0_2;
										r0_2 = t4 - r1;
										if (t4 != r1)
											r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
										if (r1 >= 0xFFFFu)
											r1 = 0xFFFFu;
										CAR(player_car)->revs = r1;
										return;
									}
									CAR(player_car)->moving = 0xFFu;
									r1 = CAR(player_car)->revs;
									t5 = divide_u32(r0_2 << 8, r19);
									r0_2 = t5 - r1;
									if (t5 != r1)
										r1 += (int32_t)(r0_2 * 1u) / 0xA + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
									if (r1 >= 0xFFFFu)
										r1 = 0xFFFFu;
									CAR(player_car)->revs = r1;
									return;
								}
							}
							if (L_00A9E9 || CAR(player_car)->speed == 0u || (int32_t)r9 > -0x380)
							{
								CAR(player_car)->wheel_slip = 0u;
								speed = CAR(player_car)->speed;
								speed_change_2 = OBJECT(current_object)->player_car.speed_change;
								t6 = speed + speed_change_2;
								overflow = v60_overflow_add32(speed, speed_change_2, 0u);
								negative = t6 >> 31;
								r0_2 = t6;
								if (negative ^ overflow)
									r0_2 = 0u;
								r1 = r0_2 + r9;
								if ((int32_t)r1 <= (int32_t)r18)
								{
									if ((int32_t)r1 < 0)
										r1 = 0u;
									t7 = r1;
									r1 = t7 - r0_2;
									if (t7 != r0_2)
										r0_2 += (int32_t)(r1 * 1u) / 2 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
								}
								else
								{
									t8 = r18;
									r1 = t8 - r0_2;
									if (t8 != r0_2)
										r0_2 += (int32_t)(r1 * 1u) / 0x1E + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
									if (r0_2 >= 0xFFFFu)
										r0_2 = 0xFFFFu;
								}
								CAR(player_car)->speed = r0_2;
								OBJECT(current_object)->player_car.impact_speed = r0_2;
								if (r0_2 == 0u)
								{
									CAR(player_car)->moving = 0u;
									r0_2 = (uint32_t)OBJECT(current_object)->player_car.gas << 8;
									r1 = CAR(player_car)->revs;
									t4 = r0_2;
									r0_2 = t4 - r1;
									if (t4 != r1)
										r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
									if (r1 >= 0xFFFFu)
										r1 = 0xFFFFu;
									CAR(player_car)->revs = r1;
									return;
								}
								CAR(player_car)->moving = 0xFFu;
								r0_2 = divide_u32(r0_2 << 8, r19);
								r1 = CAR(player_car)->revs;
								t4 = r0_2;
								r0_2 = t4 - r1;
								if (t4 != r1)
									r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
								if (r1 >= 0xFFFFu)
									r1 = 0xFFFFu;
								CAR(player_car)->revs = r1;
								return;
							}
							CAR(player_car)->wheel_slip = 0xFFu;
							L_00A8A3 = true;
						}
						else
						{
							L_00A8E6 = (int32_t)mem_read32(r12 + 0x10u) <= (int32_t)r9;
							if (!L_00A8E6)
							{
								L_00A9E9 = CAR(player_car)->speed == 0u || (int32_t)r9 > -0x480;
								if (!L_00A9E9)
									L_00A8A3 = true;
							}
						}
					} while (L_00A8E6);
				} while (L_00A9E9);
				if (L_00A8A3)
				{
					L_00A8A3 = false;
					L_00A914 = SCREEN(screen)->unk_102 == 0u;
					if (!L_00A914)
					{
						if (CAR(player_car)->skidding == 0u)
						{
							r0 = rom_00ABC3[CAR(player_car)->model];
							set_tyre_noise(r0);
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
void angles_to_offset(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t *p_r3, uint32_t *p_r4, uint32_t *p_r5)
{
	uint32_t r6 = 0;

	r6 = (int16_t)rom_101400[r0];
	*p_r3 = (int32_t)(((int32_t)((int16_t)rom_100C00[r1] * r6) >> 14) * r2) >> 14;
	*p_r4 = 0u - ((int32_t)((int16_t)rom_100C00[r0] * r2) >> 14);
	*p_r5 = (int32_t)(((int32_t)((int16_t)rom_101400[r1] * r6) >> 14) * r2) >> 14;
}

void sub_00AB24(void)
{
	sub_00AB46();
}

void sub_00AB36(void)
{
	sub_00AB46();
}

void sub_00AB46(void)
{
}

/* Takes the roadside surfaces of the stage (screen unk_A3): the two lists of rom_00B78C for it (car
   unk_148, unk_14C), from the segment where the stage starts (car unk_4E) to the next one's (unk_4C). */
void player_car_next_surfaces(void)
{
	uint16_t r5 = 0;
	uint32_t r7 = 0;

	r7 = SCREEN(screen)->unk_A3;
	CAR(player_car)->surface_stage = r7;
	r5 = (uint16_t)(mem_read16(screen + 0x700u + r7 * 2u) + SCREEN(screen)->unk_73E);
	CAR(player_car)->surface_start = CAR(player_car)->surface_next_start;
	CAR(player_car)->surface_next_start += r5;
	r7 = 0xB78Cu + (uint8_t)(r7 * 2u) * 4u;
	CAR(player_car)->surface_entry_1 = ROM32(r7);
	CAR(player_car)->surface_entry_2 = ROM32(r7 + 4u);
}

/* The drag on the car: speed squared, times the model's off-road factor while slowed (car unk_147) or
   off the road (car unk_7C, with the off-road shake); and the roadside surfaces under its wheels (car
   unk_148, unk_14C) give the effects at each side (car unk_140: bit 0 with unk_141, bit 1 with unk_142). */
uint32_t player_car_road_drag(uint32_t r0, uint16_t r1, uint16_t r2, uint16_t r3)
{
	uint32_t carry = 0;
	uint16_t r4 = 0, r5 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0;
	uint32_t r1_2 = 0, r9 = 0, r13 = 0, segment = 0, surface_start = 0;

	r13 = CAR(player_car)->surface_entry_1;
	r14 = CAR(player_car)->surface_entry_2;
	if ((int16_t)ROM16(r13) < 0 && (int16_t)ROM16(r14) < 0 && CAR(player_car)->surface_next_start <= CAR(player_car)->segment)
	{
		player_car_next_surfaces();
		r13 = CAR(player_car)->surface_entry_1;
		r14 = CAR(player_car)->surface_entry_2;
	}
	segment = CAR(player_car)->segment;
	surface_start = CAR(player_car)->surface_start;
	if (ROM16(r13) + surface_start <= segment)
	{
		r13 += 8u;
		CAR(player_car)->surface_entry_1 = r13;
	}
	if (ROM16(r14) + surface_start <= segment)
	{
		r14 = r14 + 8u;
		CAR(player_car)->surface_entry_2 = r14;
	}
	if (CAR(player_car)->slowed_frames != 0u)
	{
		CAR(player_car)->slowed_frames--;
		r0 = (r0 * mem_read32(r12 + 8u)) >> 4;
		player_car_off_road_shake();
		return r0;
	}
	r4 = (uint16_t)(r1 - r3);
	r9 = 0u;
	r3 = (uint16_t)(r3 + r1 - 0x8000u);
	r4 -= 0x8000u;
	carry = (uint16_t)(0x8000u + r2) < r3;
	r1_2 = carry;
	carry = r4 < (uint16_t)(0u - r2 - 0x8000u);
	CAR(player_car)->off_road = 0u;
	if ((uint8_t)(r1_2 + carry) != 0u)
	{
		CAR(player_car)->off_road = 0xFFu;
		r0 = (r0 * mem_read32(r12 + 8u)) >> 4;
		player_car_off_road_shake();
	}
	r15 = ROM16(r13 + 2u);
	r16 = ROM16(r13 + 4u);
	r17 = ROM16(r14 + 2u);
	r18 = ROM16(r14 + 4u);
	r5 = (uint16_t)(r3 - r2);
	r3 += r2;
	carry = r15 < r5;
	r1_2 = (uint8_t)(r9 * 2u + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r16 < r5;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r3 < r17;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r3 < r18;
	switch ((uint8_t)(r1_2 + r9 + carry))
	{
	case 0:
		sub_00B65F();
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
		sub_00B68A();
		break;
	case 2:
	case 6:
	case 10:
	case 14:
		sub_00B67C();
		break;
	case 4:
	case 12:
		sub_00B66E(r13);
		break;
	case 8:
		sub_00B660(r13);
		break;
	case 16:
		sub_00B698();
		break;
	case 17:
	case 19:
		sub_00B6C3();
		break;
	case 18:
		sub_00B6B5();
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 28:
	case 29:
	case 30:
	case 31:
		sub_00B6A7(r13);
		break;
	case 24:
	case 25:
	case 26:
	case 27:
		sub_00B699(r13);
		break;
	default:
		invalid_jump(rom_00B70C[(uint8_t)(r1_2 + r9 + carry)]);
	}
	r5 = (uint16_t)(r4 - r2);
	r4 += r2;
	carry = r15 < r5;
	r1_2 = (uint8_t)(r9 * 2u + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r16 < r5;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r4 < r17;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r4 < r18;
	switch ((uint8_t)(r1_2 + r9 + carry))
	{
	case 0:
		sub_00B698();
		return r0;
	case 8:
	case 9:
	case 10:
	case 11:
		sub_00B699(r13);
		return r0;
	case 4:
	case 5:
	case 6:
	case 7:
	case 12:
	case 13:
	case 14:
	case 15:
		sub_00B6A7(r13);
		return r0;
	case 2:
		sub_00B6B5();
		return r0;
	case 1:
	case 3:
		sub_00B6C3();
		return r0;
	default:
		invalid_jump(rom_00B74C[(uint8_t)(r1_2 + r9 + carry)]);
	}
}

void sub_00B65F(void)
{
}

void sub_00B660(uint32_t r13)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = mem_read8(r13 + 6u);
}

void sub_00B66E(uint32_t r13)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = mem_read8(r13 + 7u);
}

void sub_00B67C(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = mem_read8(r14 + 6u);
}

void sub_00B68A(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = mem_read8(r14 + 7u);
}

void sub_00B698(void)
{
}

void sub_00B699(uint32_t r13)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = mem_read8(r13 + 6u);
}

void sub_00B6A7(uint32_t r13)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = mem_read8(r13 + 7u);
}

void sub_00B6B5(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = mem_read8(r14 + 6u);
}

void sub_00B6C3(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = mem_read8(r14 + 7u);
}

/* Off the road: the model's shake to the screen's bits of output port D (unk_20E0D0, the wheel motor's;
   rom_00B704) and its off-road noise (rom_00B6FC). */
void player_car_off_road_shake(void)
{
	uint8_t r0 = 0;
	uint32_t model = 0;

	model = CAR(player_car)->model;
	unk_20E0D0[SCREEN(screen)->unk_00] = rom_00B704[model];
	r0 = rom_00B6FC[model];
	set_tyre_noise(r0);
}

/* The rev meter, the bar left of the speed at the bottom right: state 0 sets it up (animation 6, at F2,
   D6), 1 draws the frame of the revs (car unk_70 >> 11, 32 steps). */
void rev_meter_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00BFCC[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		OBJECT(current_object)->frame = CAR(player_car)->revs >> 11;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00BFCC[state]);
	}
	OBJECT(current_object)->state++;
	r9 = 6u;
	OBJECT(current_object)->palette = 0x44u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF2u;
	OBJECT(current_object)->y = 0xD6u;
	animation_start(r9);
}

/* The speed in km/h at the bottom right: state 0 sets it up (digits of animation 12, units_x, digits_y)
   and clears the screen's maximum (unk_F52), sum (unk_F54) and count (unk_F58); 1 each frame the car's
   speed / 88 into screen unk_F50, and while moving adds it to the sum, counts it and keeps the maximum
   (for the LCD's pages), then draws its digits from the units leftwards. */
void speedometer_update(void)
{
	uint32_t r0 = 0, r9 = 0, r18 = 0, r19 = 0;
	uint32_t stack_4 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C024[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r9 = 0x12u;
		OBJECT(current_object)->palette = 0x48u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->speedometer.units_x = 0x118u;
		OBJECT(current_object)->speedometer.digits_y = 0xCEu;
		SCREEN(screen)->unk_F54 = 0u;
		SCREEN(screen)->unk_F58 = 0u;
		SCREEN(screen)->unk_F52 = 0u;
		animation_start(r9);
		return;
	case 1:
		r18 = OBJECT(CAR(player_car)->object)->player_car.speed;
		SCREEN(screen)->unk_F50 = r18;
		if (r18 != 0u)
		{
			r18 = r18 / 0x88u;
			SCREEN(screen)->unk_F50 = r18;
			SCREEN(screen)->unk_F54 += r18;
			SCREEN(screen)->unk_F58++;
			if ((int16_t)SCREEN(screen)->unk_F52 < (int16_t)r18)
				SCREEN(screen)->unk_F52 = r18;
		}
		OBJECT(current_object)->x = OBJECT(current_object)->speedometer.units_x;
		OBJECT(current_object)->y = OBJECT(current_object)->speedometer.digits_y;
		do
		{
			r19 = 0u;
			divx_pair(0xAu, &r18, &r19);
			OBJECT(current_object)->frame = r19;
			stack_4 = r18;
			r0 = draw_object(r0);
			r18 = stack_4;
			OBJECT(current_object)->x -= 8u;
		} while ((uint16_t)r18 != 0u);
		return;
	default:
		invalid_jump(rom_00C024[state]);
	}
}

/* The km/h after the speed: state 0 sets it up (animation 10 at 127, CE), 1 draws it. */
void speed_unit_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C0D4[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00C0D4[state]);
	}
	OBJECT(current_object)->state++;
	r9 = 0x10u;
	OBJECT(current_object)->palette = 0x47u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x127u;
	OBJECT(current_object)->y = 0xCEu;
	animation_start(r9);
}

/* The gear at the bottom right, and the shifting: state 0 sets it up (rom_00D018 by the top gear: AUTO
   for the automatic). 1 each frame: the shift buttons (screen unk_118 bits 4 up, 5 down) change the gear
   within 0 and the top gear; shifting up at revs F000 or more, or down into revs past FFFF, spins the
   wheels 30 frames (car unk_144, tyre noise 1); the gear's ratio to car unk_64, its frame drawn. Below
   the top gear with the revs at the limit (FF00) and the car moving, after 150 frames starts the
   shift_up_hint once. Once the race is over (car unk_181 or unk_182), 2 only draws. */
void gear_indicator_update(void)
{
	uint8_t r0_2 = 0, r2 = 0;
	uint32_t r0 = 0, r1 = 0, r2_2 = 0, r9 = 0, r21 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C118[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r9 = rom_00D018[CAR(player_car)->top_gear];
		OBJECT(current_object)->palette = 0x45u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0x114u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(r9);
		return;
	case 1:
		if (CAR(player_car)->shift_spin != 0u)
		{
			CAR(player_car)->shift_spin--;
			r0_2 = 1u;
			set_tyre_noise_on_road(r0_2);
		}
		r0 = CAR(player_car)->top_gear;
		r1 = CAR(player_car)->gear;
		r2 = (uint8_t)SCREEN(screen)->unk_118;
		if ((r2 & 0x10u) != 0u && r1 != r0)
		{
			r1 = (uint8_t)(r1 + 1u);
			if (CAR(player_car)->revs >= 0xF000u)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		if ((r2 & 0x20u) != 0u && r1 != 0u)
		{
			r2_2 = ROM8(CAR(player_car)->gear_ratios + r1);
			r1 = (uint8_t)(r1 - 1u);
			if (divide_u32(CAR(player_car)->revs * r2_2, ROM8(CAR(player_car)->gear_ratios + r1)) >= 0xFFFFu)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		CAR(player_car)->gear = r1;
		CAR(player_car)->gear_ratio = ROM8(CAR(player_car)->gear_ratios + r1);
		OBJECT(current_object)->frame = r1;
		if (r1 == r0)
		{
			if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
				OBJECT(current_object)->state++;
			r0 = draw_object(r0);
			return;
		}
		r0 = 0u;
		if (CAR(player_car)->revs >= 0xFF00u && CAR(player_car)->speed != 0u)
		{
			r0 = (uint16_t)(OBJECT(current_object)->gear_indicator.redline_frames + 1u);
			if (OBJECT(current_object)->gear_indicator.hint_started == 0u && r0 >= 0x96u)
			{
				OBJECT(current_object)->gear_indicator.hint_started = 0xFFu;
				r21 = start_object();
				mem_write32(r21 + 4u, shift_up_hint_update_ADDRESS);
				mem_write32(r21 + 0x14u, current_object);
				mem_write8(r21 + 9u, r1);
			}
		}
		OBJECT(current_object)->gear_indicator.redline_frames = r0;
		if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
			OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 2:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00C118[state]);
	}
}

/* A hint to shift up, blinking at the bottom centre (animation E): state 0 sets it up; 1 blinks it (4
   frames of 8) while the car stays in the gear it started in with the revs at FC00 or more, then ends
   and lets the gear indicator start it again. */
void shift_up_hint_update(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = (uint16_t)rom_00C23C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r9 = 0xEu;
		OBJECT(current_object)->palette = 0x46u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(r9);
		return;
	case 1:
		if (CAR(player_car)->gear != OBJECT(current_object)->shift_up_hint.gear || CAR(player_car)->revs < 0xFC00u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.hint_started = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.redline_frames = 0u;
			return;
		}
		r0_2 = (r0 & ~0xFFu) | OBJECT(current_object)->shift_up_hint.blink;
		r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 1u);
		OBJECT(current_object)->shift_up_hint.blink = r0_2;
		if ((r0_2 & 4u) != 0u)
			return;
		r0_2 = draw_object(r0_2);
		return;
	default:
		invalid_jump(rom_00C23C[state]);
	}
}

/* The number of cars after the place at the bottom left (/2): ends at once with one player; state 0 sets
   it up (animation 5, frame the players less one) and starts the place_number; 1 draws it, and when the
   screen's place (screen unk_31F) changes passes the new one to the place_number (its place, state 2). */
void place_count_update(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0, object = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C2B8[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		r0 = (r0 & ~0xFFu) | SCREEN(screen)->unk_31F;
		if (OBJECT(current_object)->place_count.place != (uint8_t)r0)
		{
			OBJECT(current_object)->place_count.place = r0;
			object = OBJECT(current_object)->place_count.number;
			OBJECT(object)->place_number.place = r0;
			OBJECT(object)->state = 2u;
		}
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00C2B8[state]);
	}
	r0 = (r0 & ~0xFFu) | mem_read8(SCREEN(screen)->unk_308 + 0x1Fu);
	t1 = r0;
	r0 = (r0 & ~0xFFu) | (uint8_t)(t1 - 1u);
	if (t1 == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	r9 = 5u;
	OBJECT(current_object)->palette = 0x43u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->x = 0x2Au;
	OBJECT(current_object)->y = 0xD8u;
	stack_4 = r0;
	animation_start(r9);
	OBJECT(current_object)->frame = stack_4;
	r0 = (r0 & ~0xFFu) | SCREEN(screen)->unk_31F;
	OBJECT(current_object)->place_count.place = r0;
	r21 = start_object();
	mem_write32(r21 + 4u, place_number_update_ADDRESS);
	mem_write8(r21 + 9u, r0);
	OBJECT(current_object)->place_count.number = r21;
	OBJECT(current_object)->state++;
	r0 = (r0 & ~0xFFu) | SCREEN(screen)->unk_31F;
	if (OBJECT(current_object)->place_count.place != (uint8_t)r0)
	{
		OBJECT(current_object)->place_count.place = r0;
		object = OBJECT(current_object)->place_count.number;
		OBJECT(object)->place_number.place = r0;
		OBJECT(object)->state = 2u;
	}
	r0 = draw_object(r0);
}

/* The screen's place, 1st or 2nd, at the bottom left: state 0 sets it up (animation 4, the place's frame
   and colour); 1 draws it; 2 a new place: twice the size and blinking for 40 frames, 4 shrinking back
   into place in 8, then 5 draws it. */
void place_number_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t1 = 0, t2 = 0, frames = 0, blink_timer = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C350[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		r9 = 4u;
		animation_start(r9);
		t1 = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = t1;
		r0 = 0x41u;
		if (t1 != 0u)
			r0 = 0x42u;
		OBJECT(current_object)->palette = r0;
		OBJECT(current_object)->x = 0x1Cu;
		OBJECT(current_object)->y = 0xD8u;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 2:
		t2 = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = t2;
		r0 = 0x41u;
		if (t2 != 0u)
			r0 = 0x42u;
		OBJECT(current_object)->palette = r0;
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
		r0 = draw_object(r0);
		return;
	case 1:
	case 5:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00C350[state]);
	}
	blink_timer = OBJECT(current_object)->place_number.blink_timer;
	OBJECT(current_object)->place_number.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		OBJECT(current_object)->place_number.blink_timer = 4u;
		OBJECT(current_object)->flags ^= 2u;
	}
	frames_2 = OBJECT(current_object)->place_number.frames;
	OBJECT(current_object)->place_number.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		r0 = draw_object(r0);
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
	r0 = draw_object(r0);
}

/* The lap time after LAP at the top right: state 0 sets it up (the label, the text record and the
   checkpoints passed); 1 shows the race time since the last checkpoint (car unk_04 / 10 less car unk_08,
   time_to_text); when the screen's car passes a checkpoint (screen 480 + player) the time it took
   (screen 3C0 records) starts the next lap (car unk_08) and stays shown in another colour for 180 frames
   (split_frames; state 2), and C00074, which the board does not map, gets 8 + 2 * screen + (1 - the
   stage's parity); after the last (rom_00D0C0) 3 keeps the last lap's time. */
void lap_timer_update(void)
{
	bool L_00C4B3 = false;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r19 = 0, checkpoint_time = 0, object = 0;
	uint16_t split_frames = 0;
	uint32_t state = 0, target1 = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C44C[state];
	do
	{
		target1 = L_00C4B3 ? 0xC4B3u : rom_00C44C[state];
		switch (target1)
		{
		case 0x00C45Cu:
			r9 = 0x2Du;
			OBJECT(current_object)->palette = 0x4Au;
			OBJECT(current_object)->draw_mode = 8u;
			OBJECT(current_object)->depth_layer = 0x19Eu;
			OBJECT(current_object)->x = 0xE0u;
			OBJECT(current_object)->y = 0x16u;
			animation_start(r9);
			object = current_object + 0x80u;
			OBJECT(object - 0x80u)->lap_timer.text_x = 0xF8u;
			object += 2u;
			OBJECT(object - 0x82u)->lap_timer.text_y = 0xEu;
			OBJECT(object - 0x82u)->lap_timer.text_attribute = 0x90u;
			OBJECT(current_object)->lap_timer.checkpoints = RACE(SCREEN(screen)->race_copy)->unk_60;
			OBJECT(current_object)->state++;
			return;
		case 0x00C4B3u:
			r19 = current_object + 0x80u;
			r10 = r19 + 6u;
			r0 = SCREEN(screen)->unk_01;
			r1 = mem_read8(screen + 0x480u + r0);
			if (OBJECT(current_object)->lap_timer.checkpoints == r1)
			{
				r0 = CAR(player_car)->race_time / 0xAu - CAR(player_car)->checkpoint_time;
				time_to_text(r0, r10);
				draw_text(r19);
				r0 = draw_object(r0);
				return;
			}
			OBJECT(current_object)->lap_timer.checkpoints = r1;
			r1 = (uint8_t)(r1 - 1u);
			OBJECT(current_object)->state = rom_00D0C0[r1];
			r1 = (uint8_t)(r1 << 3);
			r0 = mem_read16(screen + 0x3C0u + (uint8_t)(r1 + r0) * 2u);
			checkpoint_time = CAR(player_car)->checkpoint_time;
			CAR(player_car)->checkpoint_time = r0;
			r0 -= checkpoint_time;
			time_to_text(r0, r10);
			OBJECT(current_object)->lap_timer.text_attribute = 0xB0u;
			OBJECT(current_object)->lap_timer.split_frames = 0xB4u;
			r0 = (r0 & ~0xFFu) | SCREEN(screen)->unk_00;
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 * 2u);
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + (~SCREEN(screen)->unk_A4 & 1u));
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 8u);
			mem_write8(IO_C00074, r0);
		case 0x00C53Bu:
			split_frames = OBJECT(current_object)->lap_timer.split_frames;
			OBJECT(current_object)->lap_timer.split_frames = split_frames - 1u;
			if (split_frames != 1u)
			{
				r19 = current_object + 0x80u;
				draw_text(r19);
				r0 = draw_object(r0);
				return;
			}
			OBJECT(current_object)->lap_timer.text_attribute = 0x90u;
			OBJECT(current_object)->state--;
			L_00C4B3 = true;
			break;
		case 0x00C541u:
			r19 = current_object + 0x80u;
			draw_text(r19);
			r0 = draw_object(r0);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_00C4B3);
}

void sub_00C552(void)
{
}

/* Sets up the STAGE label at the top left of the race screen (sprite set 2E), then stage_indicator_draw. */
void stage_indicator_start(void)
{
	uint32_t r9 = 0;

	r9 = 0x2Eu;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x20u;
	OBJECT(current_object)->y = 0x16u;
	animation_start(r9);
	OBJECT(current_object)->handler = stage_indicator_draw_ADDRESS;
	stage_indicator_draw();
}

/* Draws the stage's name after the label (rom_00D034 by the forks taken, screen unk_A5: PRE-STAGE, 1 to
   4, FINAL STAGE) and the label itself. */
void stage_indicator_draw(void)
{
	uint32_t r0 = 0, r19 = 0;

	r0 = SCREEN(screen)->unk_A5;
	r19 = rom_00D034[r0];
	draw_text(r19);
	r0 = draw_object(r0);
}

/* The start's messages in the middle of the screen: state 0 shows YOU'RE IN CAR NUMBER n! (animation 28F
   + the screen's car number, screen unk_02, with DJ message 21 + it); when the start's countdown (the
   car object's unk_10) is at 30 frames, Ready (animation 31, DJ message 31); at 0, Go! (animation 32, DJ
   message 32), 30 frames, then it ends. */
void start_message_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C5A8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x80u;
		r9 = SCREEN(screen)->unk_02;
		r9 = (uint16_t)(r9 + 0x28Fu);
		animation_start(r9);
		OBJECT(current_object)->palette = CAR(player_car)->model + 0x289u;
		r0 = (uint8_t)(0x21u + SCREEN(screen)->unk_02);
		start_dj_message((uint8_t)r0);
		OBJECT(current_object)->state++;
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			r0 = draw_object(r0);
			return;
		}
		r9 = 0x31u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x4Cu;
		r0 = 0x31u;
		start_dj_message((uint8_t)r0);
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			r0 = draw_object(r0);
			return;
		}
		r9 = 0x31u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x4Cu;
		r0 = 0x31u;
		start_dj_message((uint8_t)r0);
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
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_00C5A8[state]);
	}
	if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
	{
		r0 = draw_object(r0);
		return;
	}
	r9 = 0x32u;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x4Bu;
	OBJECT(current_object)->start_message.frames = 0x1Eu;
	r0 = 0x32u;
	start_dj_message((uint8_t)r0);
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->start_message.frames;
	OBJECT(current_object)->start_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	r0 = draw_object(r0);
}

/* The time left in seconds at the top centre, and the time up: state 0 waits for the race's time
   (unk_308's +6A), takes it into car unk_12 and sets the digits up (time_digits_start); 1 shows it
   during the start's countdown; 2 follows the race's time (until the screen's car has finished,
   race_copy unk_5E) and starts EXTEND TIME at each checkpoint (time_left_display_check_extension); at 0
   the time is up (car unk_180) and 3 shows 0, until time comes back (the car drives on: its state 2).
   The digits flip as they change (time_digits_draw). */
void time_left_display_update(void)
{
	uint32_t r0 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = mem_read16(SCREEN(screen)->unk_308 + 0x6Au);
		if (r0 == 0u)
			return;
		CAR(player_car)->time_left = r0;
		OBJECT(current_object)->state++;
		time_digits_start();
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->state != 1u)
		{
			OBJECT(current_object)->state++;
			time_left_display_check_extension();
			if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
				CAR(player_car)->time_left = mem_read16(SCREEN(screen)->unk_308 + 0x6Au);
		}
		break;
	case 2:
		time_left_display_check_extension();
		if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
			CAR(player_car)->time_left = mem_read16(SCREEN(screen)->unk_308 + 0x6Au);
		break;
	case 3:
		time_left_display_check_extension();
		r0 = mem_read16(SCREEN(screen)->unk_308 + 0x6Au);
		if (r0 != 0u)
		{
			CAR(player_car)->time_left = r0;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->unk_0E = 0u;
			OBJECT(CAR(player_car)->object)->unk_0F = 0u;
		}
		r0 = binary_to_bcd(r0);
		time_digits_draw(r0);
		return;
	default:
		invalid_jump(rom_00C66C[OBJECT(current_object)->state]);
	}
	r0 = CAR(player_car)->time_left;
	if (r0 == 0u)
	{
		OBJECT(current_object)->state++;
		CAR(player_car)->time_up = 0xFFu;
		time_left_display_check_extension();
		r0 = mem_read16(SCREEN(screen)->unk_308 + 0x6Au);
		if (r0 != 0u)
		{
			CAR(player_car)->time_left = r0;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->unk_0E = 0u;
			OBJECT(CAR(player_car)->object)->unk_0F = 0u;
		}
		r0 = binary_to_bcd(r0);
		time_digits_draw(r0);
		return;
	}
	r0 = r0 / 0x1Eu;
	r0 = binary_to_bcd(r0);
	time_digits_draw(r0);
}

/* Sets up the three digits of the time at the top centre (animation 3 at A9, 11), each to open from
   nothing at its first value. Used by time_left_display, car_select_run and 001EC2. */
void time_digits_start(void)
{
	uint32_t r9 = 0, object = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->unk_80 = SPRITE_RAM + 0x1u;
	object += 4u;
	OBJECT(object - 0x84u)->unk_84 = 0u;
	OBJECT(object - 0x84u)->unk_86 = 0u;
	OBJECT(object - 0x84u)->unk_87 = 0u;
	object += 4u;
	OBJECT(object - 0x88u)->unk_88 = SPRITE_RAM + 0x1u;
	object += 4u;
	OBJECT(object - 0x8Cu)->unk_8C = 0u;
	object += 4u;
	OBJECT(object - 0x90u)->unk_90 = SPRITE_RAM + 0x1u;
	OBJECT(object - 0x90u)->unk_94 = 0u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 0u;
	r9 = 3u;
	OBJECT(current_object)->palette = 0x40u;
	OBJECT(current_object)->unk_20 = 0xA9u;
	OBJECT(current_object)->y = 0x11u;
	animation_start(r9);
}

/* Draws the three digits of R0 (binary-coded decimal), the units at the right: a digit that changes
   squeezes shut and opens again as the new one; leading zeros are left out. */
void time_digits_draw(uint32_t r0)
{
	bool L_00C73B = false;
	uint32_t negative = 0;
	uint8_t r1 = 0, r2 = 0;
	uint16_t r3 = 0;
	uint32_t r4 = 0, r18 = 0, r19 = 0;
	uint32_t stack_4 = 0, stack_8 = 0, stack_C = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	OBJECT(current_object)->x = OBJECT(current_object)->unk_20;
	r18 = current_object + 0x80u;
	r19 = 3u;
	do
	{
		r1 = r0 & 0xFu;
		t1 = mem_read8(r18);
		negative = t1 >> 7;
		r18++;
		r2 = mem_read8(r18);
		r18++;
		r3 = mem_read16(r18);
		r18 += 6u;
		r4 = r0;
		do
		{
			if (L_00C73B || t1 != 0u)
			{
				if (L_00C73B || negative == 0u)
				{
					L_00C73B = false;
					r4 = mem_read32(r18 - 4u);
					t2 = r3;
					r3 = (uint16_t)(t2 - 0x40u);
					mem_write16(r18 - 6u, r3);
					if (t2 != 0x40u)
					{
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->frame = r2;
							OBJECT(current_object)->width_scale = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							draw_object(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
					else
					{
						mem_write8(r18 - 8u, 0xFFu);
						mem_write8(r18 - 7u, r1);
						mem_write32(r18 - 4u, r0);
					}
				}
				else
				{
					r3 += 0x40u;
					mem_write16(r18 - 6u, r3);
					if ((uint8_t)r3 != 0u)
					{
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->frame = r2;
							OBJECT(current_object)->width_scale = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							draw_object(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
					else
					{
						mem_write8(r18 - 8u, 0u);
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->frame = r2;
							OBJECT(current_object)->width_scale = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							draw_object(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
				}
			}
			else
			{
				if (r2 != r1)
				{
					mem_write8(r18 - 8u, 1u);
					L_00C73B = true;
				}
				if (!L_00C73B)
				{
					if (r19 == 3u || r4 != 0u)
					{
						OBJECT(current_object)->frame = r2;
						OBJECT(current_object)->width_scale = r3;
						stack_4 = r0;
						stack_8 = r18;
						stack_C = r19;
						draw_object(r0);
						r19 = stack_C;
						r18 = stack_8;
						r0 = stack_4;
					}
				}
			}
		} while (L_00C73B);
		r0 = r0 >> 4;
		OBJECT(current_object)->x -= 0x12u;
		r19--;
	} while (r19 != 0u);
}

/* Starts the EXTEND TIME message once for each checkpoint passed (screen unk_349, counted in car
   unk_CD). */
void time_left_display_check_extension(void)
{
	uint32_t r21 = 0;

	if (CAR(player_car)->extensions_shown < SCREEN(screen)->unk_349)
	{
		CAR(player_car)->extensions_shown = SCREEN(screen)->unk_349;
		r21 = start_object();
		mem_write32(r21 + 4u, extend_time_message_start_ADDRESS);
	}
}

/* Sets up the EXTEND TIME message under the time (animation 33) for 90 frames, blinks one of the
   screen's cabinet lamps 16 times (bytes C1-C3 of its entry at unk_20E0C0), then
   extend_time_message_blink. */
void extend_time_message_start(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Fu;
	r9 = 0x33u;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x3Fu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x30u;
	OBJECT(current_object)->extend_time_message.frames = 0x5Au;
	OBJECT(current_object)->extend_time_message.blink = 0u;
	OBJECT(current_object)->handler = extend_time_message_blink_ADDRESS;
	r0 = SCREEN(screen)->unk_00;
	r10 = unk_20E000_ADDRESS + r0 * 4u;
	mem_write8(r10 + 0xC3u, 1u);
	mem_write8(r10 + 0xC2u, 0x10u);
	mem_write8(r10 + 0xC1u, 0x10u);
	extend_time_message_blink(r0);
}

/* Blinks EXTEND TIME (4 frames on, 4 off) until its frames run out. */
void extend_time_message_blink(uint32_t r0)
{
	uint32_t r0_2 = 0;
	uint8_t frames = 0;

	frames = OBJECT(current_object)->extend_time_message.frames;
	OBJECT(current_object)->extend_time_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	r0_2 = (r0 & ~0xFFu) | OBJECT(current_object)->extend_time_message.blink;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 1u);
	OBJECT(current_object)->extend_time_message.blink = r0_2;
	if ((r0_2 & 4u) != 0u)
		return;
	r0_2 = draw_object(r0_2);
}

/* The start of a stage's road (position 0 of streams 1-30): when the car (car unk_44) reaches it, the
   stage the car is in becomes the one whose road is being built (screen unk_A4 = unk_A2, its level;
   unk_A6 = unk_A3, its number; car unk_4A = unk_48, the segment it starts at), and it ends. In the
   attract demo (screen unk_90 bit 8) it ends at once. */
void stage_entry_update(void)
{
	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	default:
		invalid_jump(rom_00C880[OBJECT(current_object)->state]);
	}
	if ((SCREEN(screen)->unk_90 & 0x100u) != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (OBJECT(current_object)->stage_entry.position <= (int16_t)CAR(player_car)->segment)
	{
		SCREEN(screen)->unk_A4 = SCREEN(screen)->unk_A2;
		SCREEN(screen)->unk_A6 = SCREEN(screen)->unk_A3;
		CAR(player_car)->reached_stage_start = CAR(player_car)->stage_start;
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
}

void sub_00C8B5(void)
{
}

/* The goal line and what follows it: state 0 waits until the screen's car reaches its segment
   (position), then marks the car at the goal (car unk_181); 1 when the car's finish is recorded (race
   unk_5E) sets up Congratulations! (animation 2F5) and the TOTAL TIME (the text at D08E and the screen's
   finish time, screen 410, through time_to_text), DJ message 34 when first, else 35, the screen's sound
   byte to 7F; 2 waits; 3 after 120 frames draws them for 60, then the title Dream Flying
   (start_song_title) and sound 10; 4 240 more frames of both, or with two players 5 only the TOTAL TIME
   beside the race_result; then it ends. */
void goal_update(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r19 = 0, r21 = 0, ignored = 0, object = 0;
	uint16_t timer = 0, timer_2 = 0, timer_3 = 0, timer_4 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_00C8C4[state];
	switch (state)
	{
	case 0:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->goal.position)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->state++;
	case 1:
		if (RACE(SCREEN(screen)->race)->unk_5E == 0u)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Cu;
		r9 = 0x2F5u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x4Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x90u;
		object = current_object + 0x80u;
		OBJECT(object - 0x80u)->goal.text_x = 0xB0u;
		object += 2u;
		OBJECT(object - 0x82u)->goal.text_y = 0x30u;
		object += 2u;
		OBJECT(object - 0x84u)->goal.text_attribute = 0x90u;
		r0 = mem_read16(screen + 0x410u + SCREEN(screen)->unk_01 * 2u);
		object += 2u;
		time_to_text(r0, object);
		r0_2 = 0x34u;
		if (SCREEN(screen)->unk_31F != 0u)
			r0_2 = 0x35u;
		start_dj_message(r0_2);
		r0 = 0x7Fu;
		sub_011FEA((uint8_t)r0);
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
		timer_2 = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(r0);
		r19 = 0xD08Eu;
		draw_text(r19);
		r19 = current_object + 0x80u;
		ignored = draw_text(r19);
		return;
	case 5:
		timer_3 = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer_3 - 1u;
		if (timer_3 != 1u)
		{
			r19 = 0xD08Eu;
			draw_text(r19);
			r19 = current_object + 0x80u;
			ignored = draw_text(r19);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_00C8C4[state]);
	}
	timer_4 = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = timer_4 - 1u;
	if (timer_4 != 1u)
	{
		draw_object(r0);
		r19 = 0xD08Eu;
		draw_text(r19);
		r19 = current_object + 0x80u;
		ignored = draw_text(r19);
		return;
	}
	r10 = 0xD0AEu;
	start_song_title(r10);
	r0 = (r0 & ~0xFFu) | 0x10u;
	request_sound((uint8_t)r0);
	if (mem_read8(SCREEN(screen)->unk_308 + 0x1Fu) != 1u)
	{
		r21 = start_object();
		mem_write32(r21 + 4u, race_result_start_ADDRESS);
		OBJECT(current_object)->state++;
	}
	OBJECT(current_object)->goal.timer = 0xF0u;
	OBJECT(current_object)->state++;
	timer_2 = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = timer_2 - 1u;
	if (timer_2 == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(r0);
	r19 = 0xD08Eu;
	draw_text(r19);
	r19 = current_object + 0x80u;
	ignored = draw_text(r19);
}

/* With two players, the result at the end of the race: the screen's place in big letters (animation 2F8:
   1st, 2nd) at the bottom for 240 frames (race_result_hold), and the verdict above it
   (race_result_verdict_start). */
void race_result_start(void)
{
	uint16_t r0 = 0;
	uint32_t r9 = 0, r21 = 0;
	uint8_t t1 = 0;

	r21 = start_object();
	mem_write32(r21 + 4u, race_result_verdict_start_ADDRESS);
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	r9 = 0x2F8u;
	animation_start(r9);
	t1 = ~~SCREEN(screen)->unk_31F;
	OBJECT(current_object)->frame = t1;
	r0 = 0x41u;
	if (t1 != 0u)
		r0 = 0x42u;
	OBJECT(current_object)->palette = r0;
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
	uint32_t r9 = 0;
	uint8_t t1 = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	t1 = ~~SCREEN(screen)->unk_31F;
	OBJECT(current_object)->frame = t1;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	if (t1 != 0u)
	{
		r9 = 0x2F7u;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x42u;
		OBJECT(current_object)->race_result.frames = 0xF0u;
		OBJECT(current_object)->handler = race_result_hold_ADDRESS;
		race_result_hold();
		return;
	}
	r9 = 0x2F6u;
	animation_start(r9);
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
   between arrows of that way: state 0 sets it up (animation 34 or 35 by the branch, car unk_C9; DJ
   message 33 when first, else 3A, unless one is playing) for 57 segments of the race (leave_segment); 1
   cycles its colours (palette_list); 2 then grows and drops out of the screen. Ends when the race is
   over. */
void branch_arrow_update(void)
{
	uint8_t r0 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0_3 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r0 = 0x3Au;
		if (SCREEN(screen)->unk_31F == 0u)
			r0 = 0x33u;
		start_dj_message_if_quiet(r0);
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Du;
		r10 = 0xCB98u;
		r9 = 0x34u;
		if (CAR(player_car)->branch != 0u)
		{
			r10 = 0xCBACu;
			r9 = 0x35u;
		}
		SCREEN(screen)->unk_F40 = r10;
		animation_start(r9);
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD0u;
		OBJECT(current_object)->branch_arrow.palette_list = 0xD0A0u;
		OBJECT(current_object)->branch_arrow.leave_segment = RACE(SCREEN(screen)->race_copy)->unk_42 + 0x39u;
		OBJECT(current_object)->state++;
	case 1:
		if (CAR(player_car)->race_over != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_42 < (int16_t)OBJECT(current_object)->branch_arrow.leave_segment)
		{
			r10 = OBJECT(current_object)->branch_arrow.palette_list;
			r0_3 = (int16_t)ROM16(r10);
			r10 += 2u;
			if ((int16_t)r0_3 < 0)
			{
				r10 += r0_3;
				r0_3 = mem_read16(r10);
				r10 += 2u;
			}
			OBJECT(current_object)->palette = r0_3;
			OBJECT(current_object)->branch_arrow.palette_list = r10;
			r0_3 = draw_object(r0_3);
			return;
		}
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00CAC8[OBJECT(current_object)->state]);
	}
	r0_2 = (uint16_t)(OBJECT(current_object)->y + 3u);
	if ((int16_t)r0_2 > 0x140)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->y = r0_2;
	r0_2 = (uint16_t)(OBJECT(current_object)->width_scale + 6u);
	OBJECT(current_object)->width_scale = r0_2;
	OBJECT(current_object)->height_scale = r0_2;
	r10 = OBJECT(current_object)->branch_arrow.palette_list;
	r0_3 = (int16_t)ROM16(r10);
	r10 += 2u;
	if ((int16_t)r0_3 < 0)
	{
		r10 += r0_3;
		r0_3 = mem_read16(r10);
		r10 += 2u;
	}
	OBJECT(current_object)->palette = r0_3;
	OBJECT(current_object)->branch_arrow.palette_list = r10;
	r0_3 = draw_object(r0_3);
}

/* The times at a checkpoint, in rows under the top line: state 0 takes each player's car image
   (rom_00CFC8 by the car chosen); 1 waits until the screen's car passes a checkpoint (screen 480 +
   player); 2 then shows a row for each car that has passed it, in the order they did (screen 390): the
   first with its time, the others with how far behind the first they were (BEHIND); each row slides in
   from the right, the screen's own blinks while it waits for the others, and all slide out once the race
   has gone C0 segments on (leave_segment) or is over; when the last has gone, back to 1. */
void checkpoint_board_update(void)
{
	bool L_00CCDE = false, L_00CCFC = false;
	uint8_t r0_2 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r21 = 0, checkpoint = 0, leave_segment = 0, object = 0;
	uint8_t blink_timer = 0, rows_left = 0;
	uint16_t t1 = 0, t4 = 0;
	uint32_t target1 = 0, t2 = 0, t3 = 0, t5 = 0, t6 = 0, t7 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r10 = SCREEN(screen)->unk_308 + 0x1Fu;
		r9 = mem_read8(r10);
		r10++;
		do
		{
			r0 = mem_read8(r10);
			mem_write32(current_object + 0x80u + r0 * 4u, rom_00CFC8[mem_read8(0x20FC49u + (r0 << 5) * 4u)]);
			r10++;
			r9--;
		} while (r9 != 0u);
		r10 = screen + 0x480u + SCREEN(screen)->unk_01;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = mem_read8(r10);
		OBJECT(current_object)->checkpoint_board.checkpoint_count = r10;
		OBJECT(current_object)->checkpoint_board.rows_shown = 0u;
		OBJECT(current_object)->state++;
		r0_2 = mem_read8(OBJECT(current_object)->checkpoint_board.checkpoint_count);
		if (OBJECT(current_object)->checkpoint_board.checkpoints_seen == r0_2)
			return;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = r0_2;
		OBJECT(current_object)->checkpoint_board.checkpoint = r0_2 - 1u;
		OBJECT(current_object)->checkpoint_board.leave_segment = CAR(player_car)->segment + 0xC0u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		r0_2 = mem_read8(OBJECT(current_object)->checkpoint_board.checkpoint_count);
		if (OBJECT(current_object)->checkpoint_board.checkpoints_seen == r0_2)
			return;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = r0_2;
		OBJECT(current_object)->checkpoint_board.checkpoint = r0_2 - 1u;
		OBJECT(current_object)->checkpoint_board.leave_segment = CAR(player_car)->segment + 0xC0u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00CBCC[OBJECT(current_object)->state]);
	}
	checkpoint = OBJECT(current_object)->checkpoint_board.checkpoint;
	OBJECT(current_object)->checkpoint_board.row = 0u;
	OBJECT(current_object)->checkpoint_board.rows_left = mem_read8(screen + 0x35Au + checkpoint);
	OBJECT(current_object)->checkpoint_board.passing_order = screen + 0x390u + checkpoint * 2u * 4u;
	do
	{
		r21 = mem_read8(OBJECT(current_object)->checkpoint_board.passing_order);
		do
		{
			do
			{
				target1 = L_00CCFC ? 0xCCECu : L_00CCFC ? 0xCCECu : rom_00CC70[mem_read8(current_object + 0xF0u + r21)];
				switch (target1)
				{
				case 0x00CC84u:
					OBJECT(current_object)->checkpoint_board.rows_shown++;
					r0 = mem_read16(screen + 0x3C0u + (((uint32_t)OBJECT(current_object)->checkpoint_board.checkpoint << 3) + r21) * 2u);
					r1 = r21;
					if (OBJECT(current_object)->checkpoint_board.row != 0u)
					{
						t1 = r0 - OBJECT(current_object)->checkpoint_board.leader_time;
						r0 = t1;
						if ((int16_t)t1 < 0)
							r0 = 0u;
						checkpoint_board_gap_time(r0, r1);
					}
					else
					{
						OBJECT(current_object)->checkpoint_board.leader_time = r0;
						checkpoint_board_leader_time(r0, r1);
					}
					r0 = SCREEN(screen)->unk_01;
					if (r0 != r21)
					{
						L_00CCDE = mem_read8(current_object + 0xF0u + r0) != 0u;
						if (!L_00CCDE)
						{
							mem_write16(current_object + 0xA0u + r21 * 2u, 0u);
							mem_write8(current_object + 0xF0u + r21, 2u);
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
					mem_write16(current_object + 0xA0u + r21 * 2u, 0x100u);
					t2 = current_object + 0xF0u + r21;
					mem_write8(t2, mem_read8(t2) + 1u);
				case 0x00CCECu:
					if (!L_00CCFC)
					{
						if (!L_00CCFC)
						{
							t3 = current_object + 0xA0u + r21 * 2u;
							t4 = mem_read16(t3);
							mem_write16(t3, t4 - 0x10u);
						}
					}
					if (!L_00CCFC && !L_00CCFC && t4 != 0x10u)
						checkpoint_board_draw_row(r21);
					else
					{
						if (!L_00CCFC)
						{
							if (!L_00CCFC)
							{
								t5 = current_object + 0xF0u + r21;
								mem_write8(t5, mem_read8(t5) + 1u);
							}
						}
						L_00CCFC = false;
						if (OBJECT(current_object)->checkpoint_board.leave_segment == 0u || CAR(player_car)->race_over != 0u)
						{
							t6 = current_object + 0xF0u + r21;
							mem_write8(t6, mem_read8(t6) + 1u);
							r0_3 = (uint16_t)(mem_read16(current_object + 0xA0u + r21 * 2u) - 0x10u);
							mem_write16(current_object + 0xA0u + r21 * 2u, r0_3);
							if (r0_3 != 0xFF00u)
								checkpoint_board_draw_row(r21);
							else
							{
								OBJECT(current_object)->checkpoint_board.rows_shown--;
								t7 = current_object + 0xF0u + r21;
								mem_write8(t7, mem_read8(t7) + 1u);
								checkpoint_board_draw_row(r21);
							}
						}
						else if (SCREEN(screen)->unk_01 != r21)
							checkpoint_board_draw_row(r21);
						else
						{
							r0 = OBJECT(current_object)->checkpoint_board.blink_on;
							blink_timer = OBJECT(current_object)->checkpoint_board.blink_timer;
							OBJECT(current_object)->checkpoint_board.blink_timer = blink_timer - 1u;
							if (blink_timer == 1u)
							{
								r0 = r0 ^ 4u;
								OBJECT(current_object)->checkpoint_board.blink_on = r0;
								OBJECT(current_object)->checkpoint_board.blink_timer = 4u + r0;
							}
							if (r0 != 0u)
								checkpoint_board_draw_row(r21);
						}
					}
					break;
				case 0x00CCFCu:
					L_00CCFC = true;
					break;
				case 0x00CD32u:
					r0_3 = (uint16_t)(mem_read16(current_object + 0xA0u + r21 * 2u) - 0x10u);
					mem_write16(current_object + 0xA0u + r21 * 2u, r0_3);
					if (r0_3 == 0xFF00u)
					{
						OBJECT(current_object)->checkpoint_board.rows_shown--;
						t7 = current_object + 0xF0u + r21;
						mem_write8(t7, mem_read8(t7) + 1u);
					}
					checkpoint_board_draw_row(r21);
					break;
				case 0x00CD55u:
					break;
				default:
					invalid_jump(target1);
				}
			} while (L_00CCFC);
		} while (L_00CCFC);
		OBJECT(current_object)->checkpoint_board.passing_order++;
		OBJECT(current_object)->checkpoint_board.row++;
		rows_left = OBJECT(current_object)->checkpoint_board.rows_left;
		OBJECT(current_object)->checkpoint_board.rows_left = rows_left - 1u;
	} while (rows_left != 1u);
	leave_segment = OBJECT(current_object)->checkpoint_board.leave_segment;
	if (leave_segment != 0u && (int16_t)leave_segment <= (int16_t)RACE(SCREEN(screen)->race_copy)->unk_42)
		OBJECT(current_object)->checkpoint_board.leave_segment = 0u;
	if (OBJECT(current_object)->checkpoint_board.rows_shown == 0u)
	{
		object = current_object + 0xF0u;
		OBJECT(object - 0xF0u)->unk_F0 = 0u;
		OBJECT(object - 0xF0u)->unk_F2 = 0u;
		OBJECT(object - 0xF0u)->unk_F4 = 0u;
		OBJECT(object - 0xF0u)->unk_F6 = 0u;
		OBJECT(object - 0xF0u)->unk_F7 = 0u;
		OBJECT(current_object)->state--;
	}
}

/* Draws row unk_12 of the board for player R21, slid by its offset: the car number, the car's small
   image, the time text (draw_text_at; another colour after the first row) and, after the first, BEHIND. */
void checkpoint_board_draw_row(uint32_t r21)
{
	uint16_t r5 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r10 = 0, r18 = 0, row = 0;

	r10 = 0xCF88u + OBJECT(current_object)->checkpoint_board.row * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->x = r0;
	OBJECT(current_object)->y = ROM16(r10 + 2u);
	r9 = rom_00CFE8[mem_read8(unk_20E030_ADDRESS + r21)];
	OBJECT(current_object)->palette = 0x27u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(r9);
	draw_object(r0);
	r10 = 0xCF68u + OBJECT(current_object)->checkpoint_board.row * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->x = r0;
	OBJECT(current_object)->y = ROM16(r10 + 2u);
	r10 = current_object + 0x80u + r21 * 4u;
	r9 = mem_read16(r10);
	OBJECT(current_object)->palette = mem_read16(r10 + 2u);
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->draw_mode = 0u;
	animation_start(r9);
	OBJECT(current_object)->frame = 4u;
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	draw_object(r0);
	row = OBJECT(current_object)->checkpoint_board.row;
	r3 = rom_00CFF8[row];
	r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + mem_read16(current_object + 0xA0u + r21 * 2u));
	r5 = 0xC0u;
	if (row != 0u)
		r5 = 0x90u;
	r18 = current_object + 0xB0u + r21 * 2u * 4u;
	draw_text_at(r3, r5, r18);
	row = OBJECT(current_object)->checkpoint_board.row;
	if (row == 0u)
		return;
	r10 = 0xCFA8u + row * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->x = r0;
	OBJECT(current_object)->y = ROM16(r10 + 2u);
	r9 = 0x36u;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(r9);
	r0 = draw_object(r0);
}

/* Writes the first car's time R0 as M'SS"HH into player R1's row text. */
void checkpoint_board_leader_time(uint32_t r0, uint32_t r1)
{
	uint32_t r10 = 0;

	r10 = current_object + 0xB0u + r1 * 2u * 4u;
	time_to_text(r0, r10);
}

void sub_00CEA4(uint32_t r0, uint32_t r10)
{
	r0 = r0 / 0xAu;
	time_to_text(r0, r10);
}

/* Writes the time R0 (hundredths of a second) as M'SS"HH, closed by 0, at R10. Used by lap_timer, goal,
   checkpoint_board, 001EC2 and 00270C. */
void time_to_text(uint32_t r0, uint32_t r10)
{
	uint32_t r1 = 0;

	r10 += 7u;
	mem_write8(r10, 0u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r10--;
	mem_write8(r10, 0x22u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(6u, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r10--;
	mem_write8(r10, 0x27u);
	mem_write8(r10 - 1u, r0 + 0x30u);
}

/* Writes how far behind the first player R1 passed (R0, hundredths) into its row text, as time_to_text
   does but with spaces for zero minutes and zero tens of seconds. */
void checkpoint_board_gap_time(uint32_t r0, uint32_t r1)
{
	uint32_t zero = 0;
	uint8_t r2 = 0;
	uint32_t r1_2 = 0, r10 = 0;

	r10 = current_object + 0xB7u + r1 * 2u * 4u;
	r2 = 0x30u;
	mem_write8(r10, 0u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	r10--;
	mem_write8(r10, r1_2 + 0x30u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	r10--;
	mem_write8(r10, r1_2 + 0x30u);
	r10--;
	mem_write8(r10, 0x22u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	zero = (uint32_t)(r0 == 0u);
	if (zero == 0u)
	{
		r10--;
		mem_write8(r10, r1_2 + r2);
		r1_2 = 0u;
		divx_pair(6u, &r0, &r1_2);
		zero = (uint32_t)(r0 == 0u);
		if (zero)
		{
			r10--;
			mem_write8(r10, r1_2 + r2);
			mem_write16(r10 - 2u, 0x2020u);
			return;
		}
		r10--;
		mem_write8(r10, r1_2 + r2);
		r10--;
		mem_write8(r10, 0x27u);
		mem_write8(r10 - 1u, r0 + r2);
		return;
	}
	r10--;
	mem_write8(r10, r1_2 + r2);
	r10--;
	mem_write8(r10, 0x20u);
	mem_write16(r10 - 2u, 0x2020u);
}

void sub_00D0C8(void)
{
	uint32_t r1 = 0;

	if (unk_20E623 == 0u)
	{
		sub_00D13A();
		return;
	}
	mem_write16(0x801000u, 0u);
	r12 = 0x800000u;
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	move_halfwords(unk_20E040_ADDRESS, 0x100800u, 0x20u);
	move_halfwords(r12 + 4u, 0x100800u, 0x10u);
	move_halfwords(r12 + 0x600u, 0x100800u, 0x400u);
	move_halfwords(unk_20FB00_ADDRESS, 0x100800u, 0x80u);
	move_string(0x100800u, 0x200u, 0x20FC00u, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
	sub_00D2BE();
}

void sub_00D13A(void)
{
	uint32_t r2 = 0, r19 = 0, ignored = 0, left_caller = 0;
	uint8_t t4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	mem_write16(0x801000u, 0u);
	r12 = 0x800000u;
	move_halfwords(unk_20E040_ADDRESS, 0x100800u, 0x20u);
	move_halfwords(r12, 0x100800u, 8u);
	move_halfwords(unk_20FB00_ADDRESS, 0x100800u, 0x80u);
	move_halfwords(WORK_RAM + 0xFC00u, 0x100800u, 0x200u);
	unk_20E042 = unk_20E620;
	if (unk_20E042 != 0u)
	{
		sub_00D2F0();
		return;
	}
	mem_write8(0x801000u, 0u);
	r19 = 0xE10Cu;
	if (unk_20E621 == 0u)
		r19 = 0xE128u;
	if (unk_20E623 != 0u)
		r19 = 0xE143u;
	sub_011874(r19);
	r19 = 0xE15Eu;
	sub_011874(r19);
	left_caller = 0u;
	left_caller = sub_00D524(left_caller);
	if (left_caller != 0u)
		return;
	mem_write16(r12, 0x56u);
	mem_write16(r12 + 2u, 0x37u);
	mem_write16(r12 + 4u, 0x30u);
	mem_write8(r12 + 0x14u, 0u);
	mem_write16(0x801002u, 0u);
	mem_write16(0x801000u, 1u);
	left_caller = 0u;
	sub_00D5E0(&ignored, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	sub_00D664(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	t1 = mem_read32(0x801002u);
	mem_write32(0x801002u, t1 | 1u);
	r2 = sub_0008F0(r2);
	left_caller = 0u;
	sub_00D5E0(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	mem_write8(r12 + 0xCu, 0u);
	left_caller = 0u;
	sub_00D6BB(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	if (unk_20E623 != 0u)
	{
		sub_00D2BE();
		return;
	}
	mem_write8(r12 + 4u, unk_20E621);
	t2 = mem_read32(0x801002u);
	mem_write32(0x801002u, t2 & ~1u);
	r2 = sub_0008F0(r2);
	left_caller = 0u;
	sub_00D5E0(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	left_caller = sub_00D6AD(r2, left_caller);
	if (left_caller != 0u)
		return;
	t3 = mem_read32(0x801002u);
	mem_write32(0x801002u, t3 | 1u);
	do
	{
		r2 = sub_0008F0(r2);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			return;
		}
		current_object = 0xE1B6u;
		sub_00D6EC();
		t4 = mem_read8(0x801002u);
	} while ((int8_t)t4 >= 0);
	sub_00D716();
	mem_write8(r12 + 0xE20u, mem_read8(r12 + 2u));
	mem_write8(r12 + 6u, 0xFFu);
	r19 = 0xE1D6u;
	sub_011874(r19);
	do
	{
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			return;
		}
		if (mem_read8(r12 + 0xCu) != 0u)
		{
			sub_00D2EB();
			return;
		}
	} while (mem_read8(r12 + 8u) == 0u);
	sub_00D716();
	unk_20E046 = 0u;
	sub_00D298();
}

void sub_00D298(void)
{
	uint8_t r1 = 0, r2 = 0;
	uint32_t r0 = 0;

	r1 = mem_read8(r12);
	unk_20E040 = r1;
	r1 = (uint8_t)(r1 * 2u);
	unk_20E041 = r1;
	r0 = mem_read8(r12 + 2u);
	mem_write8(r12 + 0x1002u, 1u);
	if (unk_20E623 == 0u && r1 < 2u)
	{
		sub_00D2F0();
		return;
	}
	r2 = (uint8_t)(unk_20E622 * 2u);
	unk_208002 = r2;
	unk_20FB02 = r2;
	r2++;
	unk_218002 = r2;
	unk_20FB82 = r2;
	sub_00D341(r0);
}

void sub_00D2BE(void)
{
	uint32_t r1 = 0, left_caller = 0;

	mem_write16(0x801000u, 0u);
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	mem_write8(r12 + 0x14u, 1u);
	mem_write8(0x801002u, 0u);
	mem_write8(0x801000u, 1u);
	left_caller = 0u;
	left_caller = sub_00D612(left_caller);
	if (left_caller != 0u)
		return;
	sub_00D298();
}

void sub_00D2EB(void)
{
	unk_20E043 = 1u;
	sub_00D2F0();
}

void sub_00D2F0(void)
{
	uint32_t r0 = 0;

	unk_20E042 = 1u;
	r0 = 1u;
	unk_20E041 = 2u;
	unk_20E040 = 1u;
	unk_208002 = 0u;
	unk_20FB02 = 0u;
	unk_218002 = 1u;
	unk_20FB82 = 1u;
	sub_00D341(r0);
}

void sub_00D341(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r19 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	unk_20FB00 = r0;
	unk_20FB80 = r0;
	r0 = (r0 & ~0xFFu) | (uint8_t)(r0 - 1u);
	r0 = (r0 & ~0xFFu) | (uint8_t)(r0 * 2u);
	unk_20FB01 = r0;
	r1 = r0 + 1u;
	unk_20FB81 = r1;
	unk_208001 = r0;
	unk_218001 = r1;
	unk_208020 = rom_00E0A0[r0] + unk_20E000_ADDRESS;
	unk_218020 = rom_00E0A0[r1] + unk_20E000_ADDRESS;
	r19 = unk_20FB00_ADDRESS;
	unk_208024 = unk_20FB00_ADDRESS;
	unk_218024 = unk_20FB80_ADDRESS;
	if ((uint8_t)r0 == 0u)
	{
		unk_20FB03 = unk_20E60D;
		unk_20FB04 = unk_20E624;
		unk_20FB05 = unk_20E610;
		unk_20FB06 = unk_20E611;
		unk_20FB07 = unk_20E613;
		unk_20FB08 = unk_20E614;
		unk_20FB09 = unk_20E615;
		unk_20FB0A = unk_20E630;
		unk_20FB0B = unk_20E632;
		unk_20FB0D = unk_20E634;
		move_halfwords(r19 + 0x30u, SOUND_RAM + 0x1F32u, 0x1Eu);
	}
	sub_01076A();
	r19 = 0xDF84u;
	sub_011874(r19);
	if (unk_20E043 != 0u)
	{
		r19 = 0xDFCEu;
		sub_011874(r19);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	if (unk_20E623 != 0u)
	{
		r19 = 0xE055u;
		sub_011874(r19);
		r19 = 0xDF98u;
		sub_011874(r19);
		r19 = 0xDFB4u;
		sub_011874(r19);
		t1 = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (t1 + 0x30u) | 0x6000u);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	if (unk_20E042 != 0u)
	{
		r19 = 0xDFEAu;
		sub_011874(r19);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	t2 = mem_read8(0x800004u);
	if (t2 == 0u)
	{
		r19 = 0xE034u;
		sub_011874(r19);
		r19 = 0xDF98u;
		sub_011874(r19);
		r19 = 0xDFB4u;
		sub_011874(r19);
		t3 = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (t3 + 0x30u) | 0x6000u);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	r19 = 0xE00Cu;
	sub_011874(r19);
	r19 = 0xDF98u;
	sub_011874(r19);
	r19 = 0xDFB4u;
	sub_011874(r19);
	t4 = mem_read8(0x800002u);
	mem_write16(VIDEO_31F736, (t4 + 0x30u) | 0x6000u);
	r0_2 = 0x80u;
	do
	{
		stack_4 = r0_2;
		ignored = sub_0008F0(ignored);
		r0_2 = stack_4 - 1u;
	} while (r0_2 != 0u);
	sub_01076A();
}

uint32_t sub_00D524(uint32_t left_caller)
{
	uint8_t r1 = 0;
	uint32_t r0 = 0, r10 = 0, r19 = 0, ignored = 0, left_caller_2 = 0;

	r10 = 0x800000u;
	r0 = 0x7FFu;
	r0 = sub_00D544(r0, r10);
	r1 = 2u;
	if ((uint8_t)r0 != 0u)
		r1 = 3u;
	if (r1 == 3u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r19 = 0xE422u;
		sub_00D7CB(r19, &ignored, &left_caller_2);
		return left_caller_2;
	}
	return left_caller;
}

uint32_t sub_00D544(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r2 = 0, r3 = 0, r6 = 0;
	uint16_t r5 = 0;
	uint32_t r1 = 0, r4 = 0, r7 = 0;
	uint8_t t5 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;

	r1 = r0;
	r2 = 1u;
	r0 &= ~0xFFu;
	do
	{
		r7 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			mem_write16(r7, r3);
			r7 += 2u;
			r6 = 6u;
			t1 = r7 * 2u;
			r5 = t1;
			if (t1 != 0u)
			{
				r6--;
				t2 = (uint32_t)r5 << 3;
				r5 = t2;
				if (t2 != 0u)
				{
					r6--;
					t3 = (uint32_t)r5 << 3;
					r5 = t3;
					if (t3 != 0u)
					{
						r6--;
						t4 = (uint32_t)r5 << 3;
						r5 = t4;
						if (t4 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 8);
			r4--;
		} while (r4 != 0u);
		r7 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			t5 = mem_read8(r7);
			r7++;
			if (t5 != r3)
			{
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ r3);
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ mem_read8(r7 - 1u));
			}
			r7++;
			r6 = 6u;
			t6 = r7 * 2u;
			r5 = t6;
			if (t6 != 0u)
			{
				r6--;
				t7 = (uint32_t)r5 << 3;
				r5 = t7;
				if (t7 != 0u)
				{
					r6--;
					t8 = (uint32_t)r5 << 3;
					r5 = t8;
					if (t8 != 0u)
					{
						r6--;
						t9 = (uint32_t)r5 << 3;
						r5 = t9;
						if (t9 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 8);
			r4--;
		} while (r4 != 0u);
		sub_00D72D();
		carry = v60_rot_cy(r2, 1u, 8);
		r2 = (uint8_t)(((uint32_t)r2 << 1) | (r2 >> 7));
	} while (carry == 0u);
	return r0;
}

void sub_00D5E0(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r19 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0;

	unk_20E046 = 0xE10u;
	do
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			*p_left_caller = 1u;
			return;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t1 = unk_20E046;
		unk_20E046 = t1 - 1u;
		if (t1 == 1u)
		{
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			r19 = 0xE484u;
			sub_00D7CB(r19, p_r2, p_left_caller);
			return;
		}
		t2 = mem_read32(0x801002u);
	} while ((t2 & 0x80u) == 0u);
	sub_00D716();
}

uint32_t sub_00D612(uint32_t left_caller)
{
	uint32_t ignored = 0, left_caller_2 = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	do
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t1 = mem_read8(0x800000u);
	} while (t1 == 0u);
	unk_20E046 = 0x3Cu;
	do
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t2 = unk_20E046;
		unk_20E046 = t2 - 1u;
	} while (t2 != 1u);
	sub_00D716();
	return left_caller;
}

void sub_00D658(void)
{
	uint32_t r0 = 0;

	r0 = 0x3000u;
	do
	{
		r0--;
	} while (r0 != 0u);
}

void sub_00D664(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r0 = 0, r19 = 0;

	r0 = 0x10u + r12;
	*p_r2 = ((((uint32_t)(uint8_t)mem_read16(r0) << 8) | (uint8_t)mem_read16(r0 + 2u)) << 8) | (uint8_t)mem_read16(r0 + 4u);
	if ((((((uint32_t)(uint8_t)mem_read16(r12) << 8) | (uint8_t)mem_read16(r12 + 2u)) << 8) | (uint8_t)mem_read16(r12 + 4u)) != 0x563730u || *p_r2 != 0x5A3830u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r19 = 0xE422u;
		sub_00D7CB(r19, p_r2, p_left_caller);
		return;
	}
}

uint32_t sub_00D6AD(uint32_t r2, uint32_t left_caller)
{
	uint32_t r0 = 0, r2_2 = 0, r19 = 0, ignored = 0, left_caller_2 = 0;

	r2_2 = (r2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)r2_2 == 0u)
	{
		r0 = 5u;
		sub_00D6C7(r0, r2_2);
		return left_caller;
	}
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
	sub_01076A();
	r19 = 0xE464u;
	sub_011868(r19);
	left_caller_2 = 0u;
	sub_00D6BB(&r2_2, &left_caller_2);
	if (left_caller_2 != 0u)
		return left_caller_2;
	r0 = 5u;
	r2_2 = (r2_2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	sub_00D6C7(r0, r2_2);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	sub_00D884(&ignored, &left_caller_2);
	return left_caller_2;
}

void sub_00D6BB(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r2 = *p_r2;
	uint32_t r0 = 0, r2_2 = 0, r19 = 0, left_caller_2 = 0;

	r2_2 = (r2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)r2_2 == 0u)
	{
		r0 = 4u;
		sub_00D6C7(r0, r2_2);
		*p_r2 = r2_2;
		return;
	}
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
	sub_01076A();
	r19 = 0xE464u;
	sub_011868(r19);
	r2_2 = (r2_2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	r0 = 4u;
	sub_00D6C7(r0, r2_2);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	sub_00D884(&r2_2, &left_caller_2);
	*p_r2 = r2_2;
	*p_left_caller = left_caller_2;
}

void sub_00D6C7(uint32_t r0, uint32_t r2)
{
	uint32_t r1 = 0, r3 = 0, r19 = 0;

	r1 = 0u;
	do
	{
		r3 = (uint8_t)(r1 * 2u);
		if ((r2 & shl32(1u, (int)(r1))) != 0u)
			r3 = (uint8_t)(r3 + 1u);
		r19 = rom_00E4D0[r3];
		sub_011874(r19);
		r1 = (uint8_t)(r1 + 1u);
		r0--;
	} while (r0 != 0u);
}

void sub_00D6EC(void)
{
	uint16_t r18 = 0;
	uint32_t r19 = 0;

	unk_20E048++;
	if ((unk_20E048 & 0xFu) != 0u)
		return;
	r19 = 0x310000u | 0xF000u | AS16(OBJECT(current_object)->flags);
	current_object = current_object + 2u;
	r18 = (uint16_t)((uint32_t)(uint8_t)OBJECT(current_object - 0x2u)->unk_02 << 8);
	current_object = current_object + 1u;
	for (;;)
	{
		r18 = (uint16_t)((r18 & ~0xFFu) | mem_read8(current_object));
		current_object = current_object + 1u;
		if ((uint8_t)r18 == 0u)
			break;
		mem_write16(r19, mem_read16(r19) ^ r18);
		r19 += 2u;
	}
}

void sub_00D716(void)
{
	uint32_t r0 = 0, r1 = 0;

	r0 = TEXT_CELL(0, 24);
	r1 = 0x28u;
	do
	{
		mem_write16(r0, 0u);
		r1--;
		r0 += 2u;
	} while (r1 != 0u);
}

void sub_00D72D(void)
{
	uint16_t r26 = 0;
	uint32_t t1 = 0;

	r26 = 0x6000u;
	r27 = TEXT_CELL(8, 24);
	r28 = 0xE17Au;
	do
	{
		r26 = (uint16_t)((r26 & ~0xFFu) | ROM8(r28));
		t1 = r27;
		r27 = r27 + 2u;
		mem_write16(t1, mem_read16(t1) ^ r26);
		r28 = r28 + 1u;
	} while (ROM8(r28) != 0u);
}

void sub_00D7CB(uint32_t r19, uint32_t *p_r2, uint32_t *p_left_caller)
{
	sub_01076A();
	sub_011868(r19);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	for (;;)
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
			break;
		current_object = 0xE40Eu;
		sub_00D6EC();
	}
	unk_20E043 = 0xFFu;
	*p_left_caller = 1u;
}

void sub_00D884(uint32_t *p_r2, uint32_t *p_left_caller)
{
	for (;;)
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
			break;
		current_object = 0xE40Eu;
		sub_00D6EC();
	}
	unk_20E043 = 0xFFu;
	*p_left_caller = 1u;
}

void sub_00D8A5(void)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0;
	uint8_t t1 = 0;

	if (unk_20E700 != 0u)
		return;
	sub_00E8DF();
	ignored = sub_0008F0(0u);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	for (;;)
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E623 == 0u)
			break;
		t1 = mem_read8(0x8006E1u);
		if (t1 != 0u)
		{
			sub_00E638();
			break;
		}
	}
	sub_00E620();
	r10 = 0x20FC02u;
	r11 = unk_20E030_ADDRESS;
	r0 = unk_20E041;
	r1 = 0x80u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10 = (r10 & ~0xFFFFu) | (uint16_t)(r10 + r1);
		r11++;
		r0--;
	} while (r0 != 0u);
	move_halfwords(SOUND_RAM + 0x1F32u, WORK_RAM + 0xFC30u, 0x1Eu);
	sub_011418(r0);
	move_string(0x20FC30u, 0x1Eu, SOUND_RAM + 0x1FAEu, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	sub_001D84();
	r10 = 0x20FC00u;
	r9 = 0u;
	if (unk_20E614 != unk_20FC08 || unk_20E610 != unk_20FC05 || unk_20E611 != unk_20FC06 || unk_20E613 != unk_20FC07 || unk_20E615 != unk_20FC09)
	{
		r9 = 0xFFFFFFFFu;
		unk_20E614 = unk_20FC08;
		unk_20E610 = unk_20FC05;
		unk_20E611 = unk_20FC06;
		unk_20E613 = unk_20FC07;
		unk_20E615 = unk_20FC09;
		sub_011018();
		sub_01139E();
	}
	if (unk_20E60D == unk_20FC03 && unk_20E624 == unk_20FC04 && unk_20E630 == unk_20FC0A && unk_20E632 == AS16(unk_20FC0B[0]) && unk_20E634 == unk_20FC0D && r9 == 0u)
		return;
	unk_20E60D = unk_20FC03;
	unk_20E624 = unk_20FC04;
	unk_20E630 = unk_20FC0A;
	unk_20E632 = AS16(unk_20FC0B[0]);
	unk_20E634 = unk_20FC0D;
	sub_00036B();
}
