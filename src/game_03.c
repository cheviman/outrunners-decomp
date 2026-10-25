/**
 *    Name:        game_03.c
 *    Description: the game's functions, part 03 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets the engine's pitch from the revs (car unk_70) each frame, or silences it in the demo without
   sound. */
void player_car_engine_sound(void)
{
	uint32_t r10 = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u || advertise_sound != 0u)
	{
		r10 = (uint32_t)SCREEN(screen)->index << 8;
		mem_write8(r10 + SOUND_700710, CAR(player_car)->revs >> 8);
		mem_write8(r10 + SOUND_700712, 0x20u);
		return;
	}
	r10 = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

/* set_tyre_noise, unless the car is off the road (car unk_7C), where the off-road noise plays. Used by
   player_car_drive, player_car_engine and gear_indicator_update. */
void set_tyre_noise_on_road(uint8_t r0)
{
	if (CAR(player_car)->off_road == 0u)
	{
		set_tyre_noise(r0);
		return;
	}
}

/* Sets the noise of the screen's car's tyres on the sound board (byte 718 of the screen's page):
   wheelspin, skids, off-road rumble. In the demo without sound it mutes the engine instead. Used by
   player_car (player_car_engine, player_car_off_road_shake, the goal) and 00664E. */
void set_tyre_noise(uint8_t r0)
{
	uint32_t r10 = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u || advertise_sound != 0u)
	{
		mem_write8(((uint32_t)SCREEN(screen)->index << 8) + (SOUND_RAM + 0x718u), r0);
		return;
	}
	r10 = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

/* Fades out the screen's music, a step every 20 of the sound program's ticks (fade_out_music_at). */
void fade_out_music(void)
{
	uint8_t r0 = 0;

	r0 = 0x20u;
	fade_out_music_at(r0);
}

/* Fades out the screen's music: R0 into byte 18 of the screen's first track in the shared sound RAM
   (700918 for screen 1, 701118 for screen 2; rom_011FFC), which has the sound program lower the track's
   volume by one every R0 of its ticks and stop it at 0. 20 from fade_out_music, 3F and 7F from
   car_radio_update, 7F from goal_update. */
void fade_out_music_at(uint8_t r0)
{
	mem_write8(rom_011FFC[SCREEN(screen)->index], r0);
}

/* Sends sound command 1F straight to the screen's request byte in the shared sound RAM (700700 + 100 *
   screen, without going through the ring of requests) and mutes the car's engine (710 = 0, 712 bit 5, as
   set_tyre_noise does in the silent demo). As the game modes start. What 1F does in the sound program
   was not followed. */
void reset_screen_sound(void)
{
	uint32_t r10 = 0;

	r10 = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(r10 + SOUND_700700, 0x1Fu);
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

/* Returns the 12-bit angle of the vector (R0 - R2, R1 - R3). Used by player_car_drive. */
uint32_t vector_angle(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
	uint32_t r0_2 = 0, r1_2 = 0, r4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	r4 = 0u;
	t1 = r0 - r2;
	r0_2 = t1;
	if ((int32_t)t1 < 0)
	{
		r0_2 = 0u - r0_2;
		r4 = (uint8_t)(r4 + 1u);
	}
	t2 = r1 - r3;
	r1_2 = t2;
	if ((int32_t)t2 < 0)
	{
		r1_2 = 0u - r1_2;
		r4 = (uint8_t)(r4 + 2u);
	}
	if ((int32_t)r0_2 < (int32_t)r1_2)
	{
		t3 = r1_2;
		r1_2 = r0_2;
		r0_2 = t3;
		r4 = (uint8_t)(r4 + 4u);
	}
	if (r0_2 == 0u)
		return r0_2;
	r0_2 = rom_103400[divide_u32((r1_2 << 10) + (r0_2 >> 1), r0_2)];
	r0_2 = (uint16_t)(r0_2 * rom_0121E8[r4]);
	r0_2 = (uint16_t)(r0_2 + rom_0121D8[r4]);
	r0_2 = r0_2 & 0xFFFu;
	return r0_2;
}

/* Returns the angles from a point (R3, R4, R5: x, y, z) toward another (R0, R1, R2): the pitch in R0,
   the yaw in R1. Used by player_car_place_view. */
void vector_angles(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t *p_r0, uint32_t *p_r1)
{
	uint32_t r0 = *p_r0;
	uint32_t r1 = *p_r1;
	uint32_t r0_2 = 0, r1_2 = 0, r2_2 = 0, r3_2 = 0, r6 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	r6 = 0u;
	t1 = r0 - r3;
	r0_2 = t1;
	if ((int32_t)t1 < 0)
	{
		r0_2 = 0u - r0_2;
		r6 = (uint8_t)(r6 + 1u);
	}
	t2 = r2 - r5;
	r2_2 = t2;
	if ((int32_t)t2 < 0)
	{
		r2_2 = 0u - r2_2;
		r6 = (uint8_t)(r6 + 2u);
	}
	if ((int32_t)r0_2 < (int32_t)r2_2)
	{
		t3 = r2_2;
		r2_2 = r0_2;
		r0_2 = t3;
		r6 = (uint8_t)(r6 + 4u);
	}
	if (r0_2 == 0u)
	{
		r0_2 = 0u;
		r1_2 = 0u;
		*p_r0 = r0_2;
		*p_r1 = r1_2;
		return;
	}
	r2_2 = rom_103400[divide_u32((r2_2 << 10) + (r0_2 >> 1), r0_2)];
	r2_2 = (uint16_t)(r2_2 * rom_0121E8[r6]);
	r2_2 = (uint16_t)(r2_2 + rom_0121D8[r6]);
	r2_2 = r2_2 & 0xFFFu;
	if ((int8_t)r6 < 4)
		r3_2 = (int16_t)rom_100C00[r2_2];
	else
		r3_2 = (int16_t)rom_101400[r2_2];
	r0_2 = divide_s32((r0_2 << 14) + ((int32_t)r3_2 >> 1), r3_2);
	r6 = 5u;
	if ((int32_t)r0_2 < 0)
		r0_2 = 0u - r0_2;
	t4 = r1 - r4;
	r1_2 = t4;
	if ((int32_t)t4 < 0)
	{
		r1_2 = 0u - r1_2;
		r6 = (uint8_t)(r6 - 1u);
	}
	r0_2 = rom_103400[divide_u32((r1_2 << 10) + (r0_2 >> 1), r0_2)];
	r0_2 = (uint16_t)(r0_2 * rom_0121E8[r6]);
	r0_2 = (uint16_t)(r0_2 + rom_0121D8[r6]);
	r0_2 = r0_2 & 0xFFFu;
	r1_2 = r2_2;
	*p_r0 = r0_2;
	*p_r1 = r1_2;
}

/* Builds the hit boxes of the race's cars at screen + 510: how many, then for each its player, race
   record and box (player_car_write_hit_box from its segment, model, place along and across; screen
   unk_500 the screen's own), and the screen's car's box from its own fields at screen + 5B0. */
void player_car_build_hit_boxes(void)
{
	uint16_t r1_2 = 0, r15 = 0, r17 = 0, road_x = 0, segment = 0, segment_fraction = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0, r11 = 0, model = 0, player = 0;

	r10 = SCREEN(screen)->host_record + 0x20u;
	r0 = RACE(SCREEN(screen)->host_record)->racer_count;
	player = SCREEN(screen)->player;
	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
		r0 = 1u;
	r11 = screen + 0x510u;
	SCREEN(r11 - 0x510u)->unk_510 = r0;
	r11 += 4u;
	do
	{
		r1 = mem_read8(r10);
		r10++;
		if ((SCREEN(screen)->demo_flags & 0x100u) != 0u || player == r1)
		{
			r1 = player;
			SCREEN(screen)->unk_500 = r11;
		}
		mem_write16(r11, r1);
		r12 = 0x20FC00u + (uint16_t)(r1 << 7);
		r15 = mem_read16(r12 + 0x40u);
		r17 = mem_read16(r12 + 0x44u);
		r1_2 = mem_read16(r12 + 0x42u);
		r2 = mem_read8(r12 + 0x49u);
		r11 += 2u;
		r11 = player_car_write_hit_box(r1_2, r2, r11, r15, r17);
		r0--;
	} while (r0 != 0u);
	r11 = screen + 0x5B0u;
	segment_fraction = CAR(player_car)->segment_fraction;
	road_x = (uint16_t)CAR(player_car)->road_x;
	segment = CAR(player_car)->segment;
	model = CAR(player_car)->model;
	r11 = player_car_write_hit_box(segment, model, r11, segment_fraction, road_x);
}

/* Writes a car's hit box: its record, its first and last segment (4 on), and its box along and across
   from the model's size (rom_0126BC). */
uint32_t player_car_write_hit_box(uint16_t r1, uint32_t r2, uint32_t r11, uint16_t r15, uint16_t r17)
{
	uint32_t r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	mem_write32(r11, r12);
	r11 += 4u;
	mem_write16(r11, r1);
	r11 += 2u;
	mem_write16(r11, r1 + 4u);
	r19 = 0x126BCu + ((r2 & ~0xFFu) | (uint8_t)(r2 * 2u)) * 4u;
	t1 = mem_read16(r19);
	r19 += 2u;
	r11 += 2u;
	mem_write16(r11, r15 + t1);
	t2 = mem_read16(r19);
	r19 += 2u;
	r11 += 2u;
	mem_write16(r11, r15 + t2);
	t3 = mem_read16(r19);
	r11 += 2u;
	mem_write16(r11, r17 + t3);
	r11 += 2u;
	mem_write16(r11, r17 + mem_read16(r19 + 2u));
	r11 += 2u;
	return r11;
}

/* road_row_hit_check with kind 3 (R27): a car that hits a fork sign is slowed for 15 frames, as off the
   road (car unk_147), instead of crashing. */
void fork_sign_hit_check(uint32_t *p_r7, uint32_t *p_r15)
{
	r27 = 3u;
	road_row_hit_check(p_r7, p_r15);
}

/* road_row_hit_check with kind 0 (R27): hitting an item crashes the car. Used by scenery,
   animated_scenery and flashing_scenery (and 012FBC). */
void road_row_crash_check(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;

	r27 = 0u;
	road_row_hit_check(&r7, &r15);
	*p_r7 = r7;
	*p_r15 = r15;
}

/* Whether the screen's car hits an item of the row, unless the car's object has flags bit 3 set: the
   car's box (the entry SCREEN(screen)->unk_500 points at: its segments at +6 and +8, along the road
   +A..+C in 128ths of a segment from its first, across +E..+10) against each item's hit_box (along from
   (item - the car's first segment) * 128 + position_fraction, across from the road's width * side +
   x_offset), for the items from first_position to end_position within the car's segments. On the first
   that overlaps: the car's object gets flags bit 2 and byte 6E = R27, the kind of hit (0 a crash, 3
   slowed), and the car's unk_122 and unk_120 half the distances from the car's box to the item's (along,
   across). */
void road_row_hit_check(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;
	uint16_t r4 = 0, r6 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint32_t r0 = 0, r7_2 = 0, r10 = 0, r15_2 = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
		return;
	r0 = (uint16_t)OBJECT(current_object)->road_row.first_position;
	r15_2 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15_2);
	r15_2 += 2u;
	r13 = mem_read16(r15_2);
	r15_2 += 2u;
	r16 = mem_read16(r15_2);
	r15_2 += 2u;
	r17 = mem_read16(r15_2);
	r15_2 += 2u;
	r18 = mem_read16(r15_2);
	r15_2 += 2u;
	r19 = mem_read16(r15_2);
	r15_2 += 2u;
	if ((int16_t)((r12 - OBJECT(current_object)->road_row.end_position) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = (uint8_t)OBJECT(current_object)->road_row.spacing;
	if (r0 < (uint16_t)r12)
	{
		do
		{
			r0 = (uint16_t)(r0 + r4);
		} while (r0 < (uint16_t)r12);
	}
	do
	{
		if (r13 <= r0)
		{
			*p_r7 = r7;
			*p_r15 = r15_2;
			return;
		}
		r7_2 = r0;
		r6 = (uint16_t)(r0 - r12);
		r0 = (uint16_t)(r0 + r4);
		r7_2 = (uint16_t)(r7_2 << 4);
		r7_2 = r7_2 & 0x7FFu;
		r10 = OBJECT(current_object)->road_row.hit_box;
		r8 = (uint16_t)((uint32_t)OBJECT(current_object)->road_row.side * mem_read16(screen + 0x2012u + r7_2 * 2u) + OBJECT(current_object)->road_row.x_offset);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + OBJECT(current_object)->road_row.position_fraction);
		r7 = r6;
		r6 += ROM16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + ROM16(r10));
		r10 += 2u;
		r7 = (uint16_t)(r7 - r16);
		r9 = (uint16_t)(r8 + ROM16(r10 + 2u) - r18);
	} while ((int16_t)(((r8 + ROM16(r10) - r19) ^ r9) & ((r6 - r17) ^ r7)) >= 0);
	object = CAR(player_car)->object;
	mem_write16(object, mem_read16(object) | 4u);
	OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
	r7 = (uint16_t)((int16_t)r7 >> 1);
	CAR(player_car)->hit_along = r7;
	CAR(player_car)->hit_across = (int16_t)r9 >> 1;
	*p_r7 = r7;
	*p_r15 = r15_2;
}

/* road_row_hit_check for the rows drawn in mirrored pairs, with kind 0 (a crash): each item's hit box is
   tested as it is and mirrored across the road's centre. Used by scenery_pair and animated_scenery_pair. */
void road_row_pair_crash_check(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;
	uint16_t r4 = 0, r6 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint32_t r0 = 0, r7_2 = 0, r10 = 0, r15_2 = 0;
	uint16_t t1 = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
		return;
	r0 = (uint16_t)OBJECT(current_object)->road_row.first_position;
	r15_2 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15_2);
	r15_2 += 2u;
	r13 = mem_read16(r15_2);
	r15_2 += 2u;
	r16 = mem_read16(r15_2);
	r15_2 += 2u;
	r17 = mem_read16(r15_2);
	r15_2 += 2u;
	r18 = mem_read16(r15_2);
	r15_2 += 2u;
	r19 = mem_read16(r15_2);
	r15_2 += 2u;
	if ((int16_t)((r12 - OBJECT(current_object)->road_row.end_position) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = (uint8_t)OBJECT(current_object)->road_row.spacing;
	if (r0 < (uint16_t)r12)
	{
		do
		{
			r0 = (uint16_t)(r0 + r4);
		} while (r0 < (uint16_t)r12);
	}
	do
	{
		if (r13 <= r0)
		{
			*p_r7 = r7;
			*p_r15 = r15_2;
			return;
		}
		r7_2 = r0;
		r6 = (uint16_t)(r0 - r12);
		r0 = (uint16_t)(r0 + r4);
		r7_2 = (uint16_t)(r7_2 << 4);
		r7_2 = r7_2 & 0x7FFu;
		r10 = OBJECT(current_object)->road_row.hit_box;
		r8 = (uint16_t)((uint32_t)OBJECT(current_object)->road_row.side * mem_read16(screen + 0x2012u + r7_2 * 2u) + OBJECT(current_object)->road_row.x_offset);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + OBJECT(current_object)->road_row.position_fraction);
		r7 = r6;
		r9 = (uint16_t)r8;
		r6 += ROM16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + ROM16(r10));
		r10 += 2u;
		r8 += ROM16(r10);
		r9 += ROM16(r10 + 2u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - r9);
		r7 = (uint16_t)(r7 - r16);
		r9 -= r18;
		r6 = (uint16_t)((r6 - r17) ^ r7);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - r19);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (0u - r8 - r18));
		r28 = (r28 & ~0xFFFFu) | (r28 & r6);
		t1 = r28 | (((r8 - r19) ^ r9) & r6);
		r28 = (r28 & ~0xFFFFu) | t1;
	} while ((int16_t)t1 >= 0);
	object = CAR(player_car)->object;
	mem_write16(object, mem_read16(object) | 4u);
	OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
	r7 = (uint16_t)((int16_t)r7 >> 1);
	CAR(player_car)->hit_along = r7;
	CAR(player_car)->hit_across = (int16_t)r9 >> 1;
	*p_r7 = r7;
	*p_r15 = r15_2;
}

void sub_01248C(void)
{
	uint16_t r4 = 0, r6 = 0, r7_2 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26 = 0;
	uint32_t r0 = 0, r7 = 0, r10 = 0, r15 = 0;
	uint16_t t1 = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->flags & 8u) == 0u)
	{
		r0 = (uint16_t)(OBJECT(current_object)->unk_54 >> 16u);
		r15 = SCREEN(screen)->unk_500 + 6u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
		r15 += 2u;
		r13 = mem_read16(r15);
		r15 += 2u;
		r16 = mem_read16(r15);
		r15 += 2u;
		r17 = mem_read16(r15);
		r15 += 2u;
		r18 = mem_read16(r15);
		r19 = mem_read16(r15 + 2u);
		if ((int16_t)((r12 - OBJECT(current_object)->unk_58) ^ (r13 - r0)) >= 0)
			return;
		r4 = (uint8_t)OBJECT(current_object)->unk_10;
		if (r0 < (uint16_t)r12)
		{
			do
			{
				r0 = (uint16_t)(r0 + r4);
			} while (r0 < (uint16_t)r12);
		}
		do
		{
			if (r13 <= r0)
				return;
			r7 = r0;
			r6 = (uint16_t)(r0 - r12);
			r0 = (uint16_t)(r0 + r4);
			r7 = (uint16_t)(r7 << 4);
			r10 = OBJECT(current_object)->unk_60;
			r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(current_object)->unk_3F * mem_read16(screen + 0x2012u + (r7 & 0x7FFu) * 2u) + OBJECT(current_object)->unk_20);
			r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(current_object)->unk_28);
			r7_2 = (uint16_t)r6;
			r9 = (uint16_t)r8;
			r26 = (uint16_t)(0u - r8);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - r9);
			r6 += mem_read16(r10);
			r10 += 2u;
			r7_2 += mem_read16(r10);
			r10 += 2u;
			r8 += mem_read16(r10);
			r26 += mem_read16(r10);
			r10 += 2u;
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + mem_read16(r10));
			r7_2 -= r16;
			r9 = (uint16_t)(r9 + mem_read16(r10) - r18);
			r6 = (uint16_t)((r6 - r17) ^ r7_2);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - r18);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (r26 - r19));
			r28 = (r28 & ~0xFFFFu) | (r28 & r6);
			t1 = r28 | (((r8 - r19) ^ r9) & r6);
			r28 = (r28 & ~0xFFFFu) | t1;
		} while ((int16_t)t1 >= 0);
		object = CAR(player_car)->object;
		mem_write16(object, mem_read16(object) | 4u);
		OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
		CAR(player_car)->hit_along = (int16_t)r7_2 >> 1;
		CAR(player_car)->hit_across = (int16_t)r9 >> 1;
	}
}

/* Whether the car hits another car of the race (the boxes of player_car_build_hit_boxes; not while
   either is immune, flags bit 3): a hit of kind 2 with that car's player in hit_other. */
void player_car_check_car_hits(void)
{
	uint16_t r4 = 0, r6_2 = 0, r7 = 0, r8 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26 = 0;
	uint32_t r5 = 0, r6 = 0, r15 = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
		return;
	r14 = screen + 0x510u;
	r15 = SCREEN(screen)->unk_500;
	r4 = mem_read16(r15);
	r15 += 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
	r15 += 2u;
	r13 = mem_read16(r15);
	r15 += 2u;
	r16 = (uint16_t)(mem_read16(r15) + 1u);
	r15 += 2u;
	r17 = mem_read16(r15);
	r15 += 2u;
	r18 = mem_read16(r15);
	r19 = mem_read16(r15 + 2u);
	r5 = SCREEN(r14 - 0x510u)->unk_510;
	r14 = r14 + 4u;
	if ((uint8_t)r5 < 2u)
		return;
	for (;;)
	{
		r26 = mem_read16(r14);
		r14 = r14 + 2u;
		if (r4 == r26)
		{
			r14 = r14 + 0x10u;
			r5--;
			if (r5 == 0u)
				return;
			continue;
		}
		r6 = mem_read32(r14);
		r14 = r14 + 4u;
		if ((mem_read32(r6 + 0x58u) & 8u) != 0u)
		{
			r14 = r14 + 0xCu;
			r5--;
			if (r5 == 0u)
				return;
			continue;
		}
		r6_2 = mem_read16(r14);
		r14 = r14 + 2u;
		r8 = (uint16_t)(r12 - mem_read16(r14));
		r14 = r14 + 2u;
		if ((int16_t)((r13 - r6_2) ^ r8) >= 0)
		{
			r14 = r14 + 8u;
			r5--;
			if (r5 == 0u)
				break;
			continue;
		}
		r6_2 = (uint16_t)((r6_2 - r12) << 7);
		r7 = (uint16_t)r6_2;
		r6_2 += mem_read16(r14);
		r14 = r14 + 2u;
		r7 += mem_read16(r14);
		r14 = r14 + 2u;
		r8 = mem_read16(r14);
		r14 = r14 + 2u;
		r8 = (uint16_t)((r8 - r19) ^ (mem_read16(r14) - r18));
		r14 = r14 + 2u;
		if ((int16_t)(r8 & ((r6_2 - r17) ^ (r7 - r16))) < 0)
		{
			object = CAR(player_car)->object;
			mem_write16(object, mem_read16(object) | 4u);
			OBJECT(CAR(player_car)->object)->player_car.hit_kind = 2u;
			OBJECT(CAR(player_car)->object)->player_car.hit_other = r26;
		}
		r5--;
		if (r5 == 0u)
			return;
	}
}

/* Whether the screen's car hits this traffic object (negative set), unless the car's object has flags
   bit 3 set: the car's box (the entry SCREEN(screen)->unk_500 points at) against the object's hit_box
   (along the road from (position - the car's first segment) * 128 + position_fraction, across from the
   road's width * side + x_offset). On a hit the car's object gets flags bit 2, byte 6E = 1 (the kind of
   hit: a single object) and 68 = this object, whose speed and weight the crash (007DBD) reads. */
uint32_t traffic_hit_check(void)
{
	uint32_t negative = 0;
	uint16_t r8 = 0;
	uint32_t r6 = 0, r7 = 0, r15 = 0, hit_box = 0, object = 0;
	uint16_t t1 = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
	{
		negative = 0u;
		return negative;
	}
	r6 = (uint16_t)OBJECT(current_object)->traffic.position;
	r15 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
	r15 += 4u;
	r7 = (uint16_t)(r6 << 4);
	hit_box = OBJECT(current_object)->traffic.hit_box;
	r8 = (uint16_t)((uint32_t)OBJECT(current_object)->traffic.side * mem_read16(screen + 0x2012u + (r7 & 0x7FFu) * 2u) + OBJECT(current_object)->traffic.x_offset);
	r6 = (uint16_t)(((r6 - r12) << 7) + OBJECT(current_object)->traffic.position_fraction);
	t1 = ((r8 + ROM16(hit_box + 4u) - mem_read16(r15 + 6u)) ^ (r8 + ROM16(hit_box + 6u) - mem_read16(r15 + 4u))) & ((r6 + ROM16(hit_box) - mem_read16(r15 + 2u)) ^ (r6 + ROM16(hit_box + 2u) - mem_read16(r15)));
	negative = t1 >> 15;
	if ((int16_t)t1 < 0)
	{
		object = CAR(player_car)->object;
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->player_car.hit_kind = 1u;
		OBJECT(object)->player_car.hit_other = current_object;
		negative = 1u;
	}
	return negative;
}

/* Starts the objects along the road of the stage being built (road_object_streams). By state: 0 takes
   the stream of the stage being built (screen unk_A3); 1 reads the stream's records while their position
   is not beyond the segments the road generator has built of the stage (car unk_46): command 0
   road_object_stream_start_object, 1 road_object_stream_stage_end, 2 road_object_stream_end, 3
   road_object_stream_fork; 2 (the stage's stream is over) waits until the generator builds the stage the
   route goes to (screen unk_A3 = stage), then stage_start = car unk_AA (the segment the new stage starts
   at) and its stream; 3 (a fork) waits until the route's next stage is known (screen unk_A1 no longer
   stage), then goes on with the stream of the branch the car takes (car unk_C9) from the fork's table,
   the same stage_start. */
void road_object_stream_read(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r10 = 0, left_caller = 0;
	uint32_t target1 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->road_object_stream.stream = road_object_streams[SCREEN(screen)->unk_A3];
		return;
	case 1:
		break;
	case 2:
		if (SCREEN(screen)->unk_A3 != OBJECT(current_object)->road_object_stream.stage)
			return;
		OBJECT(current_object)->state--;
		r0_2 = SCREEN(screen)->unk_A3;
		OBJECT(current_object)->road_object_stream.stage_start = CAR(player_car)->built_segments;
		OBJECT(current_object)->road_object_stream.stream = road_object_streams[r0_2];
		break;
	case 3:
		if (SCREEN(screen)->unk_A1 == OBJECT(current_object)->road_object_stream.stage)
			return;
		OBJECT(current_object)->state -= 2u;
		OBJECT(current_object)->road_object_stream.stream = ROM32(OBJECT(current_object)->road_object_stream.stream + CAR(player_car)->branch * 4u);
		break;
	default:
		invalid_jump(rom_012708[OBJECT(current_object)->state]);
	}
	r10 = OBJECT(current_object)->road_object_stream.stream;
	for (;;)
	{
		r0 = ROM16(r10);
		r10 += 2u;
		if ((int16_t)CAR(player_car)->stage_segments < (int16_t)r0)
			break;
		r1 = rom_012748[ROM16(r10)];
		left_caller = 0u;
		r10 += 2u;
		target1 = r1;
		switch (target1)
		{
		case 0x012758u:
			r10 = road_object_stream_start_object(r0, r10);
			break;
		case 0x012777u:
			left_caller = road_object_stream_stage_end();
			break;
		case 0x01279Cu:
			left_caller = road_object_stream_end();
			break;
		case 0x012786u:
			left_caller = road_object_stream_fork(r10);
			break;
		default:
			invalid_jump(target1);
		}
		if (left_caller != 0u)
			return;
	}
}

/* Command 0 of a stream: starts an object (start_object) at the record's position plus stage_start
   (field 1C), with the record's handler and its parameter in field 18 (most often the address of the
   object's record in the ROM), and moves the stream past the record. */
uint32_t road_object_stream_start_object(uint16_t r0, uint32_t r10)
{
	uint32_t r21 = 0;

	r21 = start_object();
	mem_write16(r21 + 0x1Cu, r0 + OBJECT(current_object)->road_object_stream.stage_start);
	mem_write32(r21 + 4u, mem_read32(r10));
	r10 += 4u;
	mem_write32(r21 + 0x18u, mem_read32(r10));
	r10 += 4u;
	OBJECT(current_object)->road_object_stream.stream = r10;
	return r10;
}

/* Command 1: the stage's stream is over; notes the stage the route goes to next (screen unk_A1) in stage
   and waits for it (state 2). */
uint32_t road_object_stream_stage_end(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->state++;
	OBJECT(current_object)->road_object_stream.stage = SCREEN(screen)->unk_A1;
	left_caller = 1u;
	return left_caller;
}

/* Command 3, a fork: keeps the address of the table of the branches' streams in stream, the current
   stage (screen unk_A3) in stage, and waits for the branch (state 3). */
uint32_t road_object_stream_fork(uint32_t r10)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->state += 2u;
	OBJECT(current_object)->road_object_stream.stream = r10;
	OBJECT(current_object)->road_object_stream.stage = SCREEN(screen)->unk_A3;
	left_caller = 1u;
	return left_caller;
}

/* Command 2: the last stage's stream is over (the goals): ends the reader. */
uint32_t road_object_stream_end(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->unk_01 = 0u;
	left_caller = 1u;
	return left_caller;
}

/* Cuts the sprites beyond the road's crests: each frame, for the crests the road's projection found
   (screen unk_1F00 of them; their depth layers at 1F10 of the screen and their screen heights at 1F30),
   each whose layer is lower than all those before it gets a sprite clip command in its layer (draw_mode
   6: the 16-byte entry at clip_command, command 7000: the drawing area from the top of the screen down
   to clip_bottom = the crest's height + screen unk_16 - 1, across clip_left..clip_right), so the sprites
   the list draws after it (meant: what lies beyond the crest) are cut at the crest's height. */
void crest_clip_update(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, crest = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 6u;
		OBJECT(current_object)->crest_clip.clip_entry = current_object + 0x80u;
		OBJECT(current_object)->crest_clip.clip_command = 0x7000u;
		OBJECT(current_object)->crest_clip.clip_bottom = 0xDFu;
		OBJECT(current_object)->crest_clip.clip_left = 0u;
		OBJECT(current_object)->crest_clip.clip_right = 0x13Fu;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_0127FC[OBJECT(current_object)->state]);
	}
	if (SCREEN(screen)->crest_count == 0u)
		return;
	OBJECT(current_object)->crest_clip.crest = 0u;
	OBJECT(current_object)->depth_layer = 0xFFFFu;
	do
	{
		crest = OBJECT(current_object)->crest_clip.crest;
		r0 = mem_read16(screen + 0x1F10u + crest * 2u);
		if (r0 < OBJECT(current_object)->depth_layer)
		{
			OBJECT(current_object)->depth_layer = r0;
			r0_2 = (uint16_t)(mem_read16(screen + 0x1F30u + crest * 2u) + SCREEN(screen)->sprite_origin_y - 1u);
			OBJECT(current_object)->crest_clip.clip_bottom = r0_2;
			draw_object(r0_2);
		}
		OBJECT(current_object)->crest_clip.crest++;
	} while (OBJECT(current_object)->crest_clip.crest <= SCREEN(screen)->crest_count);
}

/* Static scenery along the road (trees, buildings, crowds, banners, signs): a row of identical sprites
   from its record. By state: 0 road_row_start; 1 road_row_crash_check, then road_row_draw; 2 (a record
   without a hit box) road_row_draw only. Ends when road_row_draw finds the whole row behind the view. */
void scenery_update(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 1:
		road_row_crash_check(&r7, &r15);
		road_row_draw(r7, r15);
		return;
	case 2:
		road_row_draw(0u, 0u);
		return;
	default:
		invalid_jump(rom_012888[OBJECT(current_object)->state]);
	}
}

/* Sets up a row of identical sprites along the road from its record (record): animation
   (animation_start), palette, flags bits to set (bit 6 mirrors the sprites), size_table, scale_shift,
   side, x_offset, y_offset, spacing, count and the hit box (hit_box points at it; none when its first 32
   bits are 0); first_position = position, end_position = position + spacing * (count - 1) + 1. Goes on
   to the next state, or the one after it (no hit check) without a hit box. Used by scenery,
   scenery_pair, fork_sign and overhead_scenery. */
void road_row_start(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->state++;
	r10 = OBJECT(current_object)->road_row.record;
	r9 = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->palette = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->flags |= ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.size_table = ROM8(r10);
	r10++;
	OBJECT(current_object)->scale_shift = ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.side = ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.x_offset = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.y_offset = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.spacing = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.count = ROM16(r10);
	r10 += 2u;
	if (ROM32(r10) == 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->road_row.hit_box = r10;
	animation_start(r9);
	position = (uint16_t)OBJECT(current_object)->road_row.position;
	r1 = (uint16_t)(OBJECT(current_object)->road_row.spacing * (OBJECT(current_object)->road_row.count - 1u) + 1u);
	OBJECT(current_object)->road_row.first_position = position;
	OBJECT(current_object)->road_row.end_position = position + r1;
}

/* Scenery whose palette cycles a step every frame, for flashing lights (the Christmas trees, the arches
   over the underwater tunnel, a fairground ride). By state: 0 reads its record as road_row_start does,
   except that the second and third halfwords are the address of its list of palettes (palette_cycle)
   instead of a palette; 1 road_row_crash_check; 1 and 2 the next palette from the list into palette (a
   negative entry goes back that many bytes first), then road_row_draw. */
void flashing_scenery_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r7 = 0, r9 = 0, r10 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->flashing_scenery.palette_cycle;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flashing_scenery.palette_cycle = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->flashing_scenery.size_table = ROM8(r10);
		r10++;
		OBJECT(current_object)->scale_shift = ROM8(r10);
		r10++;
		OBJECT(current_object)->flashing_scenery.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->flashing_scenery.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flashing_scenery.y_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flashing_scenery.spacing = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flashing_scenery.count = ROM16(r10);
		r10 += 2u;
		if (ROM32(r10) == 0u)
			OBJECT(current_object)->state++;
		OBJECT(current_object)->flashing_scenery.hit_box = r10;
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->flashing_scenery.position;
		r1 = (uint16_t)(OBJECT(current_object)->flashing_scenery.spacing * (OBJECT(current_object)->flashing_scenery.count - 1u) + 1u);
		OBJECT(current_object)->flashing_scenery.first_position = position;
		OBJECT(current_object)->flashing_scenery.end_position = position + r1;
		return;
	case 1:
		road_row_crash_check(&r7, &r15);
		r10 = OBJECT(current_object)->flashing_scenery.palette_cycle;
		r0 = (int16_t)ROM16(r10);
		r10 += 2u;
		if ((int16_t)r0 < 0)
		{
			r10 += r0;
			r0 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(current_object)->palette = r0;
		OBJECT(current_object)->flashing_scenery.palette_cycle = r10;
		road_row_draw(r7, r15);
		return;
	case 2:
		r10 = OBJECT(current_object)->flashing_scenery.palette_cycle;
		r0 = (int16_t)ROM16(r10);
		r10 += 2u;
		if ((int16_t)r0 < 0)
		{
			r10 += r0;
			r0 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(current_object)->palette = r0;
		OBJECT(current_object)->flashing_scenery.palette_cycle = r10;
		road_row_draw(r7, r15);
		return;
	default:
		invalid_jump(rom_012914[OBJECT(current_object)->state]);
	}
}

/* Static scenery drawn twice, at each side of the road, the second mirrored (rows of trees, houses,
   spectators). By state: 0 road_row_start; 1 road_row_pair_crash_check, then road_row_pair_draw; 2 (no
   hit box) road_row_pair_draw only. */
void scenery_pair_update(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 1:
		road_row_pair_crash_check(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_0129B8[OBJECT(current_object)->state]);
	}
	road_row_pair_draw(r7, r15);
}

/* Animated scenery high over the road, drawn from the middle of the screen instead of the road's centre
   line (screen_centered), so it does not swing with the bends: the schools of fish over the underwater
   tunnel, the cloud bands of stage 25. By state: 0 screen_centered = FF, then road_row_start (its
   records have no hit box); 1 and 2 animation_advance and road_row_draw. */
void overhead_scenery_update(void)
{
	uint32_t r7 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->overhead_scenery.screen_centered = 0xFFu;
		road_row_start();
		return;
	case 1:
	case 2:
		r7 = animation_advance(0u);
		road_row_draw(r7, 0u);
		return;
	default:
		invalid_jump(rom_012A00[OBJECT(current_object)->state]);
	}
}

/* The northern lights over stage 26 (stream 26 at 1040: two of them, with their own lists): drawn at a
   screen position (draw_mode 0) that slides in from x -160 to 160, 8 a frame (y 40, or 30 for car model
   6, car unk_60), then stays; it steps through its list of animations (aurora_next_animation, each shown
   animation_frames frames, aurora_count_down) and is drawn only every other frame (when countdown is
   odd), so it looks faint. It never ends. */
void aurora_update(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = (uint16_t)rom_012A2C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->aurora.animation_list;
		OBJECT(current_object)->aurora.animation_list = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		OBJECT(current_object)->aurora.animation_frames = ROM8(r10 + 1u);
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->x = 0xFF60u;
		if (CAR(player_car)->model != 6u)
		{
			OBJECT(current_object)->y = 0x40u;
			aurora_next_animation();
		}
		else
		{
			OBJECT(current_object)->y = 0x30u;
			aurora_next_animation();
		}
		OBJECT(current_object)->x += 8u;
		if ((int16_t)OBJECT(current_object)->x >= 0xA0)
			OBJECT(current_object)->state++;
		break;
	case 1:
		OBJECT(current_object)->x += 8u;
		if ((int16_t)OBJECT(current_object)->x >= 0xA0)
			OBJECT(current_object)->state++;
		aurora_count_down();
		r0 = (uint16_t)((r0 & ~0xFFu) | OBJECT(current_object)->aurora.countdown);
		t1 = r0 & 1u;
		r0_2 = (r0 & ~0xFFu) | t1;
		if (t1 == 0u)
			return;
		animation_advance(0u);
		r0_2 = draw_object(r0_2);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_012A2C[state]);
	}
	aurora_count_down();
	r0 = (uint16_t)((r0 & ~0xFFu) | OBJECT(current_object)->aurora.countdown);
	t1 = r0 & 1u;
	r0_2 = (r0 & ~0xFFu) | t1;
	if (t1 == 0u)
		return;
	animation_advance(0u);
	r0_2 = draw_object(r0_2);
}

/* Counts the current animation's frames down; when they run out, aurora_next_animation. */
void aurora_count_down(void)
{
	uint8_t countdown = 0;

	countdown = OBJECT(current_object)->aurora.countdown;
	OBJECT(current_object)->aurora.countdown = countdown - 1u;
	if (countdown == 1u)
	{
		aurora_next_animation();
		return;
	}
}

/* Starts the next animation of the list (animation_list; a negative entry goes back that many bytes
   first) for animation_frames frames. */
void aurora_next_animation(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	r10 = OBJECT(current_object)->aurora.animation_list;
	r0 = (int16_t)ROM16(r10);
	r10 += 2u;
	if ((int16_t)r0 < 0)
	{
		r10 += r0;
		r0 = (int16_t)mem_read16(r10);
		r10 += 2u;
	}
	r9 = r0;
	OBJECT(current_object)->aurora.animation_list = r10;
	OBJECT(current_object)->aurora.countdown = OBJECT(current_object)->aurora.animation_frames;
	animation_start(r9);
}

/* The signs on the road where it forks at the end of a stage: a line down the middle before the fork and
   a line across each branch after it. By state: 0 road_row_start; 1 fork_sign_hit_check, then
   road_row_draw; 2 road_row_draw only. A car that hits one is slowed, not crashed. */
void fork_sign_update(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 2:
		road_row_draw(0u, 0u);
		return;
	case 1:
		fork_sign_hit_check(&r7, &r15);
		road_row_draw(r7, r15);
		return;
	default:
		invalid_jump(rom_012ADC[OBJECT(current_object)->state]);
	}
}

/* Scenery that animates in place (the Ferris wheel, windmills; also the scenery of the goal lines). By
   state: 0 road_row_start_copy; 1 road_row_crash_check; 1 and 2 animation_advance and road_row_draw. */
void animated_scenery_update(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start_copy();
		return;
	case 1:
		road_row_crash_check(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_012B00[OBJECT(current_object)->state]);
	}
	r7 = animation_advance(r7);
	road_row_draw(r7, r15);
}

/* The same code as road_row_start, at another address: the first step of animated_scenery and
   animated_scenery_pair. */
void road_row_start_copy(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->state++;
	r10 = OBJECT(current_object)->road_row.record;
	r9 = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->palette = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->flags |= ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.size_table = ROM8(r10);
	r10++;
	OBJECT(current_object)->scale_shift = ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.side = ROM8(r10);
	r10++;
	OBJECT(current_object)->road_row.x_offset = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.y_offset = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.spacing = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->road_row.count = ROM16(r10);
	r10 += 2u;
	if (ROM32(r10) == 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->road_row.hit_box = r10;
	animation_start(r9);
	position = (uint16_t)OBJECT(current_object)->road_row.position;
	r1 = (uint16_t)(OBJECT(current_object)->road_row.spacing * (OBJECT(current_object)->road_row.count - 1u) + 1u);
	OBJECT(current_object)->road_row.first_position = position;
	OBJECT(current_object)->road_row.end_position = position + r1;
}

/* Animated scenery drawn twice, at each side of the road, the second mirrored (the people lining the
   start and the goal, the scenery around the forks). By state: 0 road_row_start_copy; 1
   road_row_pair_crash_check; 1 and 2 animation_advance and road_row_pair_draw. */
void animated_scenery_pair_update(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start_copy();
		return;
	case 1:
		road_row_pair_crash_check(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_012B90[OBJECT(current_object)->state]);
	}
	r7 = animation_advance(r7);
	road_row_pair_draw(r7, r15);
}

/* What walks across the road as the car comes near (the camels of the desert stages). By state: 0 reads
   the record (animation, crossing_animation, palette, flags, side, x_offset, trigger_distance, x_speed,
   draw_mode, scale_shift; y_offset -256 for animation 165), trigger_position = position -
   trigger_distance, appear_position = position - draw_distance; 1 not drawn until the view (race_copy
   unk_46) passes appear_position; 2 drawn standing (draw_object) until the car (car unk_44) reaches
   trigger_position, then crossing_animation; 3 while the car (car unk_42) has not reached it: x_offset
   += x_speed and position + 1 each frame, and while within draw_distance of the car animation_advance
   and draw_object; it ends when the car reaches it, it gets beyond draw_distance or its screen_x leaves
   -F0..130. */
void road_crosser_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, crossing_animation = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_012BBC[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->road_crosser.record;
		r9 = ROM16(r10);
		r10 += 2u;
		if (r9 == 0x165u)
			AS32(OBJECT(current_object)->road_crosser.y_offset) = (int16_t)0xFF00u;
		OBJECT(current_object)->road_crosser.crossing_animation = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->road_crosser.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->road_crosser.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->road_crosser.trigger_distance = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->road_crosser.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->road_crosser.position;
		OBJECT(current_object)->road_crosser.trigger_position = position - OBJECT(current_object)->road_crosser.trigger_distance;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->road_crosser.appear_position = position - r1;
		OBJECT(current_object)->road_crosser.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->road_crosser.appear_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->road_crosser.trigger_position)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		crossing_animation = OBJECT(current_object)->road_crosser.crossing_animation;
		animation_start(crossing_animation);
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->road_crosser.position)
		{
			OBJECT(current_object)->road_crosser.x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->road_crosser.position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->road_crosser.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->road_crosser.trigger_position)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		crossing_animation = OBJECT(current_object)->road_crosser.crossing_animation;
		animation_start(crossing_animation);
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->road_crosser.position)
		{
			OBJECT(current_object)->road_crosser.x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->road_crosser.position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->road_crosser.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 3:
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->road_crosser.position)
		{
			OBJECT(current_object)->road_crosser.x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->road_crosser.position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->road_crosser.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012BBC[state]);
	}
}

/* The kangaroos of stage 21 (Australia) that hop across the road ahead of the car. By state: 0 reads the
   record (animation, hop_animation, palette, flags, side, x_offset, trigger_distance, x_speed,
   draw_mode, scale_shift), trigger_position = position - trigger_distance, appear_position = position -
   draw_distance; 1 not drawn until the view (race_copy unk_46) passes appear_position; 2 drawn standing
   until the car (car unk_44) reaches trigger_position, then hop_animation; 3 while the car (car unk_42)
   has not reached it: x_offset += x_speed each frame and it moves along the road at the car's own pace
   (position_fraction += the car's speed, car unk_74, >> 9), animation_advance and draw_object; it ends
   when the car reaches it, when it gets beyond draw_distance or when its screen_x leaves -F0..130. */
void kangaroo_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, hop_animation = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_012CB8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->kangaroo.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->kangaroo.hop_animation = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->kangaroo.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->kangaroo.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->kangaroo.trigger_distance = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->kangaroo.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->kangaroo.position;
		OBJECT(current_object)->kangaroo.trigger_position = position - OBJECT(current_object)->kangaroo.trigger_distance;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->kangaroo.appear_position = position - r1;
		OBJECT(current_object)->kangaroo.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->kangaroo.appear_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->kangaroo.trigger_position)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		hop_animation = OBJECT(current_object)->kangaroo.hop_animation;
		animation_start(hop_animation);
		if (OBJECT(current_object)->kangaroo.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.x_offset += OBJECT(current_object)->kangaroo.x_speed;
		r0 = (uint16_t)(OBJECT(current_object)->kangaroo.position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->kangaroo.position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->kangaroo.position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->kangaroo.position_fraction = r0;
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.position_fraction = r0;
		animation_advance(0u);
		draw_object(r0);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	case 2:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->kangaroo.trigger_position)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		hop_animation = OBJECT(current_object)->kangaroo.hop_animation;
		animation_start(hop_animation);
		if (OBJECT(current_object)->kangaroo.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.x_offset += OBJECT(current_object)->kangaroo.x_speed;
		r0 = (uint16_t)(OBJECT(current_object)->kangaroo.position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->kangaroo.position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->kangaroo.position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->kangaroo.position_fraction = r0;
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.position_fraction = r0;
		animation_advance(0u);
		draw_object(r0);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	case 3:
		if (OBJECT(current_object)->kangaroo.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.x_offset += OBJECT(current_object)->kangaroo.x_speed;
		r0 = (uint16_t)(OBJECT(current_object)->kangaroo.position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->kangaroo.position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->kangaroo.position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->kangaroo.position_fraction = r0;
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->kangaroo.position_fraction = r0;
		animation_advance(0u);
		draw_object(r0);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	default:
		invalid_jump(rom_012CB8[state]);
	}
}

/* Scenery that travels across the view once the car comes near (helicopters, the roller coaster train,
   birds, fish over the tunnel, a tanker truck on a bridge). By state: 0 reads the record (animation,
   palette, flags, draw_mode, scale_shift, side, x_offset, y_offset, trigger_distance + 16, x_speed,
   y_speed, road_speed), trigger_position = position - trigger_distance; 1 not drawn until the view
   (race_copy unk_46) passes trigger_position; 1 and 2: while the car (car unk_42) has not reached it,
   each frame x_offset += x_speed, y_offset += y_speed, position += road_speed, and while within
   draw_distance of the car animation_advance and draw_object; it ends when the car reaches it, it gets
   beyond draw_distance or its screen_x leaves -F0..130. */
void moving_scenery_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_012DC4[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->moving_scenery.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		r10++;
		OBJECT(current_object)->scale_shift = ROM8(r10);
		r10++;
		OBJECT(current_object)->moving_scenery.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->moving_scenery.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->moving_scenery.y_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->moving_scenery.trigger_distance = ROM16(r10);
		OBJECT(current_object)->moving_scenery.trigger_distance += 0x10u;
		r10 += 2u;
		OBJECT(current_object)->moving_scenery.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->moving_scenery.y_speed = ROM16(r10);
		OBJECT(current_object)->moving_scenery.road_speed = ROM16(r10 + 2u);
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->moving_scenery.position;
		OBJECT(current_object)->moving_scenery.trigger_position = position - OBJECT(current_object)->moving_scenery.trigger_distance;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->moving_scenery.appear_position = position - r1;
		OBJECT(current_object)->moving_scenery.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->moving_scenery.trigger_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->moving_scenery.position)
		{
			OBJECT(current_object)->moving_scenery.x_offset += OBJECT(current_object)->moving_scenery.x_speed;
			OBJECT(current_object)->moving_scenery.y_offset += OBJECT(current_object)->moving_scenery.y_speed;
			OBJECT(current_object)->moving_scenery.position += OBJECT(current_object)->moving_scenery.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->moving_scenery.draw_distance + OBJECT(current_object)->moving_scenery.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->moving_scenery.screen_x >= -0xF0 && OBJECT(current_object)->moving_scenery.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->moving_scenery.position)
		{
			OBJECT(current_object)->moving_scenery.x_offset += OBJECT(current_object)->moving_scenery.x_speed;
			OBJECT(current_object)->moving_scenery.y_offset += OBJECT(current_object)->moving_scenery.y_speed;
			OBJECT(current_object)->moving_scenery.position += OBJECT(current_object)->moving_scenery.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->moving_scenery.draw_distance + OBJECT(current_object)->moving_scenery.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->moving_scenery.screen_x >= -0xF0 && OBJECT(current_object)->moving_scenery.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012DC4[state]);
	}
}

/* The helicopter over the waterfalls of stage 4: it swoops down, then flies off. By state: 0 reads the
   record (animation, palette, flags, draw_mode, scale_shift, side, x_offset, y_offset, trigger_distance
   + 16, x_speed, y_speed, road_speed, dive_frames, dive_y_speed), trigger_position = position -
   trigger_distance, appear_position = position - draw_distance; 1 not drawn until the view (race_copy
   unk_46) passes trigger_position, then dive_countdown = dive_frames; 2 for dive_frames frames y_offset
   += dive_y_speed and position += road_speed; 3 x_offset += x_speed, y_offset += y_speed, position +=
   road_speed; from 2 on, animation_advance and draw_object while within draw_distance of the car (car
   unk_42); it ends when the car reaches it, when it gets beyond draw_distance or when its screen_x
   leaves -F0..130. */
void swooping_helicopter_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t dive_countdown = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_012EB0[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->swooping_helicopter.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		r10++;
		OBJECT(current_object)->scale_shift = ROM8(r10);
		r10++;
		OBJECT(current_object)->swooping_helicopter.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->swooping_helicopter.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.y_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.trigger_distance = ROM16(r10);
		OBJECT(current_object)->swooping_helicopter.trigger_distance += 0x10u;
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.y_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.road_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->swooping_helicopter.dive_frames = ROM8(r10);
		OBJECT(current_object)->swooping_helicopter.dive_y_speed = ROM16(r10 + 1u);
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->swooping_helicopter.position;
		OBJECT(current_object)->swooping_helicopter.trigger_position = position - OBJECT(current_object)->swooping_helicopter.trigger_distance;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->swooping_helicopter.appear_position = position - r1;
		OBJECT(current_object)->swooping_helicopter.draw_distance = r1;
		return;
	case 1:
		if (OBJECT(current_object)->swooping_helicopter.trigger_position < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->swooping_helicopter.dive_countdown = OBJECT(current_object)->swooping_helicopter.dive_frames;
			r0 = draw_object(r0);
			return;
		}
		return;
	case 2:
		OBJECT(current_object)->swooping_helicopter.y_offset += OBJECT(current_object)->swooping_helicopter.dive_y_speed;
		dive_countdown = OBJECT(current_object)->swooping_helicopter.dive_countdown;
		OBJECT(current_object)->swooping_helicopter.dive_countdown = dive_countdown - 1u;
		if (dive_countdown != 1u)
		{
			OBJECT(current_object)->swooping_helicopter.position += OBJECT(current_object)->swooping_helicopter.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->swooping_helicopter.position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(r0);
				if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->swooping_helicopter.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->swooping_helicopter.x_offset += OBJECT(current_object)->swooping_helicopter.x_speed;
		OBJECT(current_object)->swooping_helicopter.y_offset += OBJECT(current_object)->swooping_helicopter.y_speed;
		OBJECT(current_object)->swooping_helicopter.position += OBJECT(current_object)->swooping_helicopter.road_speed;
		if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->swooping_helicopter.position) < (int16_t)CAR(player_car)->window_segment)
		{
			animation_advance(0u);
			draw_object(r0);
			if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
				return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 3:
		if (OBJECT(current_object)->swooping_helicopter.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->swooping_helicopter.x_offset += OBJECT(current_object)->swooping_helicopter.x_speed;
		OBJECT(current_object)->swooping_helicopter.y_offset += OBJECT(current_object)->swooping_helicopter.y_speed;
		OBJECT(current_object)->swooping_helicopter.position += OBJECT(current_object)->swooping_helicopter.road_speed;
		if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->swooping_helicopter.position) < (int16_t)CAR(player_car)->window_segment)
		{
			animation_advance(0u);
			draw_object(r0);
			if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
				return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012EB0[state]);
	}
}

/* The START banner over the start line and its two posts with the signal lights (stream 0 at 20: four of
   them). By state: 0 reads the record as road_row_start does (animation, palette, flags, draw_mode,
   scale_shift, side, x_offset, y_offset, spacing, count, hit box; first_position, end_position), and
   goes on to state 2 without a hit box; 1 road_row_crash_check and draw_object (no record has a hit
   box); 2 the lights: palette from rom_01306E by the seconds to the start (the car object's unk_10, in
   frames, rounded up): C9 until the last 3 seconds, then CC, CB, and CA at the start, which goes on to
   state 3; 3 draw_object. It never ends. */
void start_gate_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_012FC8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->start_gate.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		r10++;
		OBJECT(current_object)->scale_shift = ROM8(r10);
		r10++;
		OBJECT(current_object)->start_gate.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->start_gate.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_gate.y_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_gate.spacing = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_gate.count = ROM16(r10);
		r10 += 2u;
		if (ROM32(r10) == 0u)
			OBJECT(current_object)->state++;
		OBJECT(current_object)->start_gate.hit_box = r10;
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->start_gate.position;
		r1 = (uint16_t)(OBJECT(current_object)->start_gate.spacing * (OBJECT(current_object)->start_gate.count - 1u) + 1u);
		OBJECT(current_object)->start_gate.first_position = position;
		OBJECT(current_object)->start_gate.end_position = position + r1;
		return;
	case 1:
		road_row_crash_check(&ignored, &ignored);
		r0 = draw_object(r0);
		return;
	case 2:
		r0 = OBJECT(CAR(player_car)->object)->player_car.timer;
		r0 = (uint16_t)(r0 + 0x1Du);
		r0 = r0 / 0x1Eu;
		OBJECT(current_object)->palette = rom_01306E[r0];
		if (r0 == 0u)
			OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 3:
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_012FC8[state]);
	}
}

/* The man at the start line who waves the chequered flag as the race starts (stream 0 at 20). By state:
   0 reads the record (animation, go_animation, palette, side, x_offset, y_offset), draw_mode 9,
   scale_shift 1; 1 drawn standing (06E) while the car object's unk_10 counts the frames to the start,
   then go_animation (06F, the waving); 1 and 2: ends once the car (car unk_42) reaches its position,
   else animation_advance and draw_object. */
void start_flagman_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, go_animation = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01308C[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->start_flagman.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_flagman.go_animation = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_flagman.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->start_flagman.x_offset = ROM16(r10);
		OBJECT(current_object)->start_flagman.y_offset = ROM16(r10 + 2u);
		animation_start(r9);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		return;
	case 1:
		r0 = OBJECT(CAR(player_car)->object)->player_car.timer;
		if (r0 != 0u)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		go_animation = OBJECT(current_object)->start_flagman.go_animation;
		animation_start(go_animation);
		if (OBJECT(current_object)->start_flagman.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(r0);
		return;
	case 2:
		if (OBJECT(current_object)->start_flagman.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(r0);
		return;
	default:
		invalid_jump(rom_01308C[state]);
	}
}

/* The man waving a chequered flag at each fork and at the goal. By state: 0 reads the record (animation,
   in_view_animation, palette, side, x_offset, y_offset), draw_mode 9, scale_shift 1, appear_position =
   position - draw_distance (and 58 = position - 30, never read); 1 not drawn until the view (race_copy
   unk_46) passes appear_position, then in_view_animation (06F, the waving; so 06E is never drawn); 1 and
   2: ends once the car (car unk_42) reaches its position, else animation_advance and draw_object. */
void flagman_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, in_view_animation = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_013110[state];
	switch (state)
	{
	case 0:
		r10 = OBJECT(current_object)->flagman.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flagman.in_view_animation = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flagman.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->flagman.x_offset = ROM16(r10);
		OBJECT(current_object)->flagman.y_offset = ROM16(r10 + 2u);
		animation_start(r9);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->flagman.position;
		OBJECT(current_object)->unk_58 = position - 0x1Eu;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->flagman.appear_position = position - r1;
		OBJECT(current_object)->flagman.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->flagman.appear_position)
			return;
		OBJECT(current_object)->state++;
		in_view_animation = OBJECT(current_object)->flagman.in_view_animation;
		animation_start(in_view_animation);
		if (OBJECT(current_object)->flagman.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 2:
		if (OBJECT(current_object)->flagman.position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_013110[state]);
	}
}

/* The balloons let go over the start line as the race starts (stream 0 at 24-34). By state: 0 reads the
   record (animation, palette, side, x_offset, y_offset), draw_mode 9, scale_shift 1; 1 not drawn until
   the start (the car object's unk_10 down to 0); 1 and 2: it rises faster and faster (rise_speed + 1
   each frame, y_offset - rise_speed / 2) and drifts (x_offset + 8), animation_advance and draw_object;
   it ends when the car (car unk_42) reaches its position or y_offset passes -3000 (high above). */
void start_balloon_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->start_balloon.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->start_balloon.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->start_balloon.x_offset = ROM16(r10);
		OBJECT(current_object)->start_balloon.y_offset = ROM16(r10 + 2u);
		animation_start(r9);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
			return;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->start_balloon.position <= (int16_t)CAR(player_car)->window_segment || OBJECT(current_object)->start_balloon.y_offset < -0x3000)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		r0 = (uint16_t)(OBJECT(current_object)->start_balloon.rise_speed + 1u);
		OBJECT(current_object)->start_balloon.rise_speed = r0;
		r0 = r0 >> 1;
		OBJECT(current_object)->start_balloon.y_offset -= r0;
		OBJECT(current_object)->start_balloon.x_offset += 8u;
		r0 = draw_object(r0);
		return;
	case 2:
		if (OBJECT(current_object)->start_balloon.position <= (int16_t)CAR(player_car)->window_segment || OBJECT(current_object)->start_balloon.y_offset < -0x3000)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		r0 = (uint16_t)(OBJECT(current_object)->start_balloon.rise_speed + 1u);
		OBJECT(current_object)->start_balloon.rise_speed = r0;
		r0 = r0 >> 1;
		OBJECT(current_object)->start_balloon.y_offset -= r0;
		OBJECT(current_object)->start_balloon.x_offset += 8u;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0131B0[OBJECT(current_object)->state]);
	}
}

/* A sound repeated at random along a stretch of road (the jungle of stage 3: sound 51): plays it, then
   again after 15 to 78 frames at random (random_number), until the car (car unk_44) has gone length
   segments from where it started. */
void ambient_sound_update(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0;
	uint8_t countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->ambient_sound.end_position = CAR(player_car)->segment + OBJECT(current_object)->ambient_sound.length;
		OBJECT(current_object)->state++;
		r0_2 = 0x51u;
		request_sound(r0_2);
		return;
	case 1:
		r0 = random_number();
		OBJECT(current_object)->ambient_sound.countdown = (r0 & 0x3Fu) + 0xFu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_013238[OBJECT(current_object)->state]);
	}
	if (OBJECT(current_object)->ambient_sound.end_position <= (int16_t)CAR(player_car)->segment)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	countdown = OBJECT(current_object)->ambient_sound.countdown;
	OBJECT(current_object)->ambient_sound.countdown = countdown - 1u;
	if (countdown != 1u)
		return;
	OBJECT(current_object)->state--;
	r0_2 = 0x51u;
	request_sound(r0_2);
}

/* Plays sound 23 once and ends: placed at the start of stage 30's stream, so it sounds as the road of
   stage 30 starts being built. */
void one_shot_sound_update(void)
{
	uint8_t r0 = 0;

	OBJECT(current_object)->flags = 0u;
	r0 = 0x23u;
	request_sound(r0);
}

/* The WEST BOUND / EAST BOUND panel at the bottom centre of the screen before the first fork (stream 0
   at 550): animation 39 at a screen position (draw_mode A, layer 19D), its palette cycling through
   base_palette to base_palette + 3 (a step every 4 frames, colour_count). By state: 0 sets it up below
   the screen (y 118); 1 rises 2 a frame to y DC; 1 and 2: once the car (car unk_44) passes 250 or the
   race time runs out (word 6A of the race record screen unk_308 points at), 3: goes down 2 a frame and
   ends at y 118. */
void west_east_panel_update(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x118u;
		r9 = 0x39u;
		OBJECT(current_object)->west_east_panel.base_palette = 0x67u;
		animation_start(r9);
		return;
	case 1:
		OBJECT(current_object)->y -= 2u;
		if (OBJECT(current_object)->y == 0xDCu)
			OBJECT(current_object)->state++;
		if (RACE(SCREEN(screen)->host_record)->time_left == 0u || CAR(player_car)->segment > 0x250u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->y += 2u;
			if (OBJECT(current_object)->y == 0x118u)
			{
				OBJECT(current_object)->flags = 0u;
				OBJECT(current_object)->unk_01 = 0u;
				return;
			}
		}
		r0 = OBJECT(current_object)->west_east_panel.colour_count;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = r0;
		r0 = draw_object(r0);
		return;
	case 2:
		if (RACE(SCREEN(screen)->host_record)->time_left == 0u || CAR(player_car)->segment > 0x250u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->y += 2u;
			if (OBJECT(current_object)->y == 0x118u)
			{
				OBJECT(current_object)->flags = 0u;
				OBJECT(current_object)->unk_01 = 0u;
				return;
			}
		}
		r0 = OBJECT(current_object)->west_east_panel.colour_count;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = r0;
		r0 = draw_object(r0);
		return;
	case 3:
		OBJECT(current_object)->y += 2u;
		if (OBJECT(current_object)->y == 0x118u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		r0 = OBJECT(current_object)->west_east_panel.colour_count;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = r0;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0132A4[OBJECT(current_object)->state]);
	}
}

/* Santa's sleigh and reindeer flying back and forth over the goal of stage 26, with sound 49 (sound)
   every 15 frames. By state: 0 reads the record (animation, palette, flags, side, x_offset, x_speed,
   sound, draw_mode, scale_shift; y_offset -100 for animation 3A0, -180 for 3A2), appear_position =
   position - draw_distance (and 58 = position - 100, never read); 1 not drawn until the view (race_copy
   unk_46) passes appear_position; 2 moves by x_speed (leftward) until its screen_x is below -B0, then 3:
   mirrored (flags bit 6), moves back until screen_x is above F0, then 2 again; each frame
   animation_advance and draw_object. It never ends (the race ends at the goal). */
void santa_sleigh_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t sound_countdown = 0, sound_countdown_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_013348[state];
	switch (state)
	{
	case 0:
		r10 = OBJECT(current_object)->santa_sleigh.record;
		r9 = ROM16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->santa_sleigh.y_offset = -0x180;
		if (r9 == 0x3A0u)
			OBJECT(current_object)->santa_sleigh.y_offset = -0x100;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->santa_sleigh.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->santa_sleigh.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->santa_sleigh.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->santa_sleigh.sound = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		OBJECT(current_object)->santa_sleigh.sound_countdown = 0xAu;
		animation_start(r9);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->santa_sleigh.position;
		OBJECT(current_object)->unk_58 = position - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->santa_sleigh.appear_position = position - r1;
		OBJECT(current_object)->santa_sleigh.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->santa_sleigh.appear_position)
			return;
		OBJECT(current_object)->state++;
	case 2:
		sound_countdown = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown - 1u;
		if (sound_countdown == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			r0 = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)r0);
		}
		OBJECT(current_object)->santa_sleigh.x_offset += OBJECT(current_object)->santa_sleigh.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->santa_sleigh.screen_x >= -0xB0)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		sound_countdown_2 = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown_2 - 1u;
		if (sound_countdown_2 == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			r0 = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)r0);
		}
		OBJECT(current_object)->santa_sleigh.x_offset -= OBJECT(current_object)->santa_sleigh.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->santa_sleigh.screen_x > 0xF0)
		{
			OBJECT(current_object)->state = 2u;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = draw_object(r0);
		return;
	case 3:
		sound_countdown_2 = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown_2 - 1u;
		if (sound_countdown_2 == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			r0 = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)r0);
		}
		OBJECT(current_object)->santa_sleigh.x_offset -= OBJECT(current_object)->santa_sleigh.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->santa_sleigh.screen_x > 0xF0)
		{
			OBJECT(current_object)->state = 2u;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_013348[state]);
	}
}

/* What runs back and forth in front of a goal's crowd, setting off rightward: a kangaroo (stage 21), a
   dragon dance (22), an orca leaping over the sea (28). By state: 0 reads the record (animation,
   palette, flags, side, x_offset, x_speed, draw_mode, scale_shift; y_offset -180 for animation 3A2),
   appear_position = position - draw_distance (and 58 = position - 100, never read); 1 not drawn until
   the view (race_copy unk_46) passes appear_position; 2 moves by x_speed (mirrored by the record's flags
   bit 6) until its screen_x passes B0, then 3: unmirrored, moves back until screen_x is below -F0, then
   2 again; each frame animation_advance and draw_object. It never ends. */
void goal_parade_rightward_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01345C[state];
	switch (state)
	{
	case 0:
		r10 = OBJECT(current_object)->goal_parade.record;
		r9 = ROM16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->goal_parade.y_offset = -0x180;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->goal_parade.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		animation_start(r9);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->goal_parade.position;
		OBJECT(current_object)->unk_58 = position - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - r1;
		OBJECT(current_object)->goal_parade.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags &= 0xBFu;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags &= 0xBFu;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		r0 = draw_object(r0);
		return;
	case 3:
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01345C[state]);
	}
}

/* Animated scenery at a goal that makes a sound every second: the pandas at Beijing (stage 24, sound
   47), the cheering group with banners in Japan (23, sound 4A), at the Matterhorn (27, sound 46). By
   state: 0 reads its record as road_row_start does (animation, palette, flags, size_table, scale_shift,
   side, x_offset, y_offset, spacing, count), with a sound where a hit box would be; 1 and 2: every 60
   frames (sound_countdown) the sound, animation_advance and road_row_draw. (State 3, the same without
   the sound, is never reached.) */
void sounding_scenery_update(void)
{
	uint8_t sound = 0;
	uint16_t r1 = 0, position = 0;
	uint32_t r7 = 0, r9 = 0, r10 = 0;
	uint8_t sound_countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r10 = OBJECT(current_object)->sounding_scenery.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->sounding_scenery.size_table = ROM8(r10);
		r10++;
		OBJECT(current_object)->scale_shift = ROM8(r10);
		r10++;
		OBJECT(current_object)->sounding_scenery.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->sounding_scenery.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->sounding_scenery.y_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->sounding_scenery.spacing = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->sounding_scenery.count = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->sounding_scenery.sound = ROM16(r10);
		OBJECT(current_object)->sounding_scenery.sound_countdown = 0x3Cu;
		OBJECT(current_object)->unk_60 = r10;
		animation_start(r9);
		position = (uint16_t)OBJECT(current_object)->sounding_scenery.position;
		r1 = (uint16_t)(OBJECT(current_object)->sounding_scenery.spacing * (OBJECT(current_object)->sounding_scenery.count - 1u) + 1u);
		OBJECT(current_object)->sounding_scenery.first_position = position;
		OBJECT(current_object)->sounding_scenery.end_position = position + r1;
		return;
	case 1:
	case 2:
		sound_countdown = OBJECT(current_object)->sounding_scenery.sound_countdown;
		OBJECT(current_object)->sounding_scenery.sound_countdown = sound_countdown - 1u;
		if (sound_countdown == 1u)
		{
			OBJECT(current_object)->sounding_scenery.sound_countdown = 0x3Cu;
			sound = (uint8_t)OBJECT(current_object)->sounding_scenery.sound;
			request_sound(sound);
		}
		r7 = animation_advance(0u);
		road_row_draw(r7, 0u);
		return;
	case 3:
		r7 = animation_advance(0u);
		road_row_draw(r7, 0u);
		return;
	default:
		invalid_jump(rom_01352C[OBJECT(current_object)->state]);
	}
}

/* What runs back and forth in front of a goal's crowd, setting off leftward: a kangaroo (stage 21), an
   orca (28), the bulls (29). As goal_parade_rightward_update, with the turns the other way: 2 moves by
   x_speed (negative) until its screen_x is below -B0, then 3: mirrored (flags bit 6), moves back until
   screen_x is above F0, then 2 again. It never ends. */
void goal_parade_leftward_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0135D8[state];
	switch (state)
	{
	case 0:
		r10 = OBJECT(current_object)->goal_parade.record;
		r9 = ROM16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->goal_parade.y_offset = -0x180;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->goal_parade.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		animation_start(r9);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->goal_parade.position;
		OBJECT(current_object)->unk_58 = position - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - r1;
		OBJECT(current_object)->goal_parade.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = draw_object(r0);
		return;
	case 3:
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0135D8[state]);
	}
}

/* The dragon dance in front of the crowd at the goal of stage 22 (with goal_parade_rightward's second
   dragon): as goal_parade_leftward_update, plus sound 44 on one frame in four at random (random_number).
   It never ends. */
void goal_parade_dragon_update(void)
{
	uint16_t r1 = 0, position = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r10 = OBJECT(current_object)->goal_parade.record;
		r9 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->flags |= ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.side = ROM8(r10);
		r10++;
		OBJECT(current_object)->goal_parade.x_offset = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->goal_parade.x_speed = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->draw_mode = ROM8(r10);
		OBJECT(current_object)->scale_shift = ROM8(r10 + 1u);
		animation_start(r9);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->goal_parade.position;
		OBJECT(current_object)->unk_58 = position - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - r1;
		OBJECT(current_object)->goal_parade.draw_distance = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			r0 = random_number();
			t1 = r0 & 3u;
			r0 = (r0 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0 = (r0 & ~0xFFu) | 0x44u;
				request_sound((uint8_t)r0);
			}
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			r0 = random_number();
			t1 = r0 & 3u;
			r0 = (r0 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0 = (r0 & ~0xFFu) | 0x44u;
				request_sound((uint8_t)r0);
			}
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		r0 = random_number();
		t1 = r0 & 3u;
		r0 = (r0 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0 = (r0 & ~0xFFu) | 0x44u;
			request_sound((uint8_t)r0);
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->goal_parade.x_offset += OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			r0 = random_number();
			t1 = r0 & 3u;
			r0 = (r0 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0 = (r0 & ~0xFFu) | 0x44u;
				request_sound((uint8_t)r0);
			}
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			r0 = random_number();
			t1 = r0 & 3u;
			r0 = (r0 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0 = (r0 & ~0xFFu) | 0x44u;
				request_sound((uint8_t)r0);
			}
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		r0 = random_number();
		t1 = r0 & 3u;
		r0 = (r0 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0 = (r0 & ~0xFFu) | 0x44u;
			request_sound((uint8_t)r0);
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 3:
		OBJECT(current_object)->goal_parade.x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		r0 = random_number();
		t1 = r0 & 3u;
		r0 = (r0 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0 = (r0 & ~0xFFu) | 0x44u;
			request_sound((uint8_t)r0);
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_0136A8[OBJECT(current_object)->state]);
	}
}

/* Starts the traffic of the stage the race's leader is in (object_lists_2E1F0). By state: 0 the list of
   the stage being built (screen unk_A3), traffic_level = 8 - the number of players (byte 1F of the
   record screen unk_308 points at) + the game setting game_difficulty, the draw distance into screen
   unk_FA6, no traffic yet (screen unk_FA8); 1 (outside the attract demo, screen unk_90 bit 8) when the
   leader's stage (screen unk_352) changes, takes its list with positions from where the leader was then
   (screen unk_350); starts each record (position, level, handler, stop_distance, record) whose position
   is within the draw distance of the leader's view (unk_46 of the race record screen unk_60), when its
   level is not above traffic_level and fewer than 32 are on the road (screen unk_FA8); then writes at
   F84 of the screen the stretches of road the players can see (each player's view, unk_46 of the race
   records listed after byte 1F, give or take the draw distance; a count, then each stretch's top and
   bottom from the highest), for traffic_out_of_view. */
void traffic_spawner_update(void)
{
	uint32_t negative = 0, overflow = 0;
	uint16_t r0_2 = 0, r1 = 0, r6 = 0, r7 = 0, r8 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r21 = 0, leader_stage = 0;
	uint16_t stage_start = 0, t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->traffic_spawner.stage_start = 0;
		r0 = SCREEN(screen)->unk_A3;
		OBJECT(current_object)->traffic_spawner.stage = r0;
		OBJECT(current_object)->traffic_spawner.list = object_lists_2E1F0[r0];
		OBJECT(current_object)->traffic_spawner.traffic_level = (uint8_t)(8u - RACE(SCREEN(screen)->host_record)->racer_count + game_difficulty);
		SCREEN(screen)->unk_FA6 = SCREEN(screen)->unk_12 >> 7;
		SCREEN(screen)->unk_F80 = SCREEN(screen)->host_record + 0x1Fu;
		SCREEN(screen)->unk_FA8 = 0u;
		OBJECT(current_object)->state++;
		return;
	case 1:
		r10 = OBJECT(current_object)->traffic_spawner.list;
		if ((SCREEN(screen)->demo_flags & 0x100u) == 0u && OBJECT(current_object)->traffic_spawner.stage != SCREEN(screen)->leader_stage)
		{
			OBJECT(current_object)->traffic_spawner.stage_start = SCREEN(screen)->leader_window_segment;
			leader_stage = SCREEN(screen)->leader_stage;
			OBJECT(current_object)->traffic_spawner.stage = leader_stage;
			r10 = object_lists_2E1F0[leader_stage];
			OBJECT(current_object)->traffic_spawner.list = r10;
		}
		for (;;)
		{
			r0_2 = mem_read16(r10);
			stage_start = OBJECT(current_object)->traffic_spawner.stage_start;
			t1 = r0_2 + stage_start;
			overflow = (uint32_t)(int16_t)r0_2 + (int16_t)stage_start != (uint32_t)(int16_t)(r0_2 + stage_start);
			negative = t1 >> 15;
			r0_2 = t1;
			r10 += 2u;
			if (negative ^ overflow)
				break;
			r1 = (uint16_t)r0_2;
			if ((int16_t)mem_read16(SCREEN(screen)->unk_60 + 0x46u) < (int16_t)(r0_2 - SCREEN(screen)->unk_FA6))
				break;
			t2 = mem_read16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->traffic_spawner.traffic_level < t2 || SCREEN(screen)->unk_FA8 >= 0x20u)
			{
				r10 += 0xCu;
				OBJECT(current_object)->traffic_spawner.list = r10;
				continue;
			}
			r21 = start_object();
			mem_write16(r21 + 0x1Cu, r1);
			mem_write32(r21 + 4u, mem_read32(r10));
			r10 += 4u;
			mem_write16(r21 + 0x84u, mem_read32(r10));
			r10 += 4u;
			mem_write32(r21 + 0x18u, mem_read32(r10));
			r10 += 4u;
			OBJECT(current_object)->traffic_spawner.list = r10;
			SCREEN(screen)->unk_FA8++;
		}
		r10 = SCREEN(screen)->unk_F80;
		r11 = screen + 0xF84u;
		r6 = SCREEN(screen)->unk_FA6;
		r7 = 0u;
		r8 = 0xFFFFu;
		r9 = mem_read8(r10);
		r10++;
		do
		{
			r0_2 = unk_20FC46[(uint32_t)mem_read8(r10) << 6];
			r1 = (uint16_t)((uint32_t)r0_2 + r6);
			r0_2 -= r6;
			r1++;
			r10++;
			if (r1 < r8)
			{
				mem_write16(r11, r8);
				r11 += 2u;
				mem_write16(r11, r1 - 1u);
				r7++;
				r11 += 2u;
			}
			r8 = (uint16_t)r0_2;
			r9--;
		} while (r9 != 0u);
		mem_write16(r11, r8);
		SCREEN(screen)->unk_F84 = r7;
		return;
	default:
		invalid_jump(rom_013778[OBJECT(current_object)->state]);
	}
}

/* The first vehicle of a convoy (the only one: stage 25, animation 35C followed by eight 35D): its
   record starts with the distance between them and the records of those that follow, closed by 0; starts
   each (handler 013AB7: traffic_car_update, kept live as long as the one before it is, which ends it)
   that distance behind the one before, keeping the one before in its field 14, with the same
   stop_distance, counted in screen unk_FA8; then becomes a traffic_car with the record after the list. */
void traffic_car_start_convoy(void)
{
	uint16_t r0 = 0;
	uint32_t r1 = 0, r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(current_object)->traffic.record;
	r0 = (uint16_t)ROM32(r10);
	r11 = current_object;
	r10 += 4u;
	for (;;)
	{
		r12 = ROM32(r10);
		r10 += 4u;
		if (r12 == 0u)
			break;
		r21 = start_object();
		r1 = mem_read16(r11 + 0x1Cu);
		r1 = (uint16_t)(r1 - r0);
		mem_write16(r21 + 0x1Cu, r1);
		mem_write32(r21 + 4u, traffic_follower_update_ADDRESS);
		mem_write16(r21 + 0x84u, mem_read16(r11 + 0x84u));
		mem_write32(r21 + 0x18u, r12);
		mem_write32(r21 + 0x14u, r11);
		r11 = r21;
		SCREEN(screen)->unk_FA8++;
	}
	OBJECT(current_object)->traffic.record = r10;
	OBJECT(current_object)->handler = traffic_car_update_ADDRESS;
	traffic_car_update(r1);
}

/* A traffic car, drawn from the angle the player sees it at. By state: 0 traffic_car_start; 1 moves
   along the road by speed (128ths of a segment a frame, position_fraction carrying into position), then
   traffic_car_draw; once the screen's car has reached the goal (car unk_181) it ends when it is 8
   segments behind the view. */
void traffic_car_update(uint32_t r1)
{
	uint32_t r0 = 0, r1_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_0138F8[state];
	switch (state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_0138F8[state]);
	}
	if (CAR(player_car)->at_goal == 0u)
	{
		r0 = (r0 & ~0xFFFFu) | OBJECT(current_object)->traffic.position_fraction;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->traffic.speed);
		r1_2 = (r1 & ~0xFFFFu) | 0x80u;
		if ((int16_t)r1_2 <= (int16_t)r0)
		{
			do
			{
				OBJECT(current_object)->traffic.position++;
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1_2);
			} while ((int16_t)r1_2 <= (int16_t)r0);
		}
		traffic_car_draw(r0, r1_2);
		return;
	}
	r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->traffic.position;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
	if ((int16_t)r0 < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	r0 = (r0 & ~0xFFFFu) | OBJECT(current_object)->traffic.position_fraction;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->traffic.speed);
	r1_2 = (r1 & ~0xFFFFu) | 0x80u;
	if ((int16_t)r1_2 <= (int16_t)r0)
	{
		do
		{
			OBJECT(current_object)->traffic.position++;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1_2);
		} while ((int16_t)r1_2 <= (int16_t)r0);
	}
	traffic_car_draw(r0, r1_2);
}

/* Sets up a traffic car from its record (the address of its sprite record, then side, x_offset and
   speed): from the sprite record its animation (animation_start), palette, passing_sound, hit_sound,
   weight and hit_box (the sprite record's last 8 bytes); draw_mode B (the frame chosen by the angle it
   is seen from, traffic_car_draw), scale_shift FF, passed 0; goes on to state 1. */
void traffic_car_start(void)
{
	uint32_t r9 = 0, r10 = 0, r11 = 0;
	uint8_t t3 = 0, t4 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0;

	r10 = OBJECT(current_object)->traffic.record;
	r11 = ROM32(r10);
	t1 = mem_read16(r11);
	r11 += 2u;
	r9 = t1;
	OBJECT(current_object)->draw_mode = 0xBu;
	t2 = mem_read16(r11);
	r11 += 2u;
	OBJECT(current_object)->palette = t2;
	OBJECT(current_object)->scale_shift = 0xFFu;
	t3 = mem_read8(r11);
	r11++;
	OBJECT(current_object)->traffic.passing_sound = t3;
	t4 = mem_read8(r11);
	r11++;
	OBJECT(current_object)->traffic.hit_sound = t4;
	t5 = mem_read16(r11);
	OBJECT(current_object)->traffic.weight = t5;
	OBJECT(current_object)->traffic.hit_box = r11 + 2u;
	r10 += 4u;
	OBJECT(current_object)->traffic.side = (uint8_t)ROM16(r10);
	r10 += 2u;
	AS32(OBJECT(current_object)->traffic.x_offset) = (int16_t)ROM16(r10);
	OBJECT(current_object)->traffic.speed = ROM16(r10 + 2u);
	OBJECT(current_object)->traffic.passed = 0u;
	OBJECT(current_object)->state++;
	animation_start(r9);
}

/* Moves and draws a traffic car: keeps the position_fraction; ends it (taking it off the count, screen
   unk_FA8) when traffic_out_of_view; plays passing_sound whenever the screen's car (race_copy unk_42)
   passes it or it passes the car (passed); while it is within the draw distance ahead of the view
   (race_copy unk_46) picks its frame by the angle the player sees it from: a column (4-1B) from the
   difference between the road's direction (+0 of the road's segment record) at the car and at it, plus a
   quarter of its screen_x, and a row (0-3, 20 frames apart) from the difference in height (+2) and the
   car's pitch (car unk_2C), shaken for 20 frames after a hit (shake, rom_013D45); traffic_hit_check, and
   on a hit traffic_start_hit_sound and shake = 1; draw_object. */
void traffic_car_draw(uint32_t r0, uint32_t r1)
{
	uint32_t zero = 0, negative = 0, carry = 0;
	uint8_t r2_2 = 0, passing_sound = 0;
	uint16_t r1_3 = 0, r2 = 0, r3 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r10 = 0, race = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t t1 = 0;

	OBJECT(current_object)->traffic.position_fraction = r0;
	r0_2 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->traffic.position;
	zero = traffic_out_of_view((uint16_t)r0_2);
	if (zero)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	race = SCREEN(screen)->race_copy;
	r1_2 = (r1 & ~0xFFFFu) | (uint16_t)r0_2;
	r1_2 = (r1_2 & ~0xFFFFu) | (uint16_t)(r1_2 - SCREEN(screen)->unk_FA6);
	r2 = RACE(race)->unk_42;
	if (OBJECT(current_object)->traffic.passed != 0u)
	{
		if ((int16_t)r2 < (int16_t)r0_2)
		{
			OBJECT(current_object)->traffic.passed = ~OBJECT(current_object)->traffic.passed;
			stack_4 = r0_2;
			stack_8 = r1_2;
			passing_sound = OBJECT(current_object)->traffic.passing_sound;
			request_sound(passing_sound);
			r1_2 = stack_8;
			r0_2 = stack_4;
		}
	}
	else if ((int16_t)r0_2 < (int16_t)r2)
	{
		OBJECT(current_object)->traffic.passed = ~OBJECT(current_object)->traffic.passed;
		stack_4 = r0_2;
		stack_8 = r1_2;
		passing_sound = OBJECT(current_object)->traffic.passing_sound;
		request_sound(passing_sound);
		r1_2 = stack_8;
		r0_2 = stack_4;
	}
	r2 = RACE(race)->unk_46;
	if ((int16_t)r0_2 < (int16_t)r2 || (int16_t)r2 <= (int16_t)r1_2)
		return;
	r10 = screen + 0x2000u + CAR(player_car)->segment_offset;
	r0_2 = mem_read16(r10);
	r2 = mem_read16(r10 + 2u);
	r10 = screen + 0x2000u + (((uint32_t)(uint16_t)OBJECT(current_object)->traffic.position << 5) & 0xFFFu);
	r3 = mem_read16(r10 + 2u);
	r0_2 = (uint16_t)((int16_t)((r0_2 << 4) - ((uint32_t)mem_read16(r10) << 4)) >> 6);
	r1_3 = (uint16_t)((int16_t)OBJECT(current_object)->traffic.screen_x >> 2);
	carry = (uint16_t)(r0_2 + r1_3) < r0_2;
	r0_2 = (uint16_t)(r0_2 + r1_3);
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 0x10u + carry);
	if ((int8_t)r0_2 >= 4)
	{
		if ((int8_t)r0_2 > 0x1B)
			r0_2 = (r0_2 & ~0xFFu) | 0x1Bu;
	}
	else
		r0_2 = (r0_2 & ~0xFFu) | 4u;
	r2 = (uint16_t)(((uint32_t)r2 << 4) - ((uint32_t)r3 << 4) + ((int16_t)((uint32_t)CAR(player_car)->view_pitch << 4) >> 2));
	carry = (r2 >> 9) & 1u;
	t1 = ((int16_t)r2 >> 10) + carry;
	r2_2 = t1;
	if ((int8_t)t1 >= 0)
	{
		if ((int8_t)r2_2 > 3)
			r2_2 = 3u;
	}
	else
		r2_2 = 0u;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + ((uint32_t)r2_2 << 5));
	if (OBJECT(current_object)->traffic.shake != 0u)
	{
		r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + rom_013D45[OBJECT(current_object)->traffic.shake]);
		OBJECT(current_object)->traffic.shake++;
		if (OBJECT(current_object)->traffic.shake == 0x15u)
			OBJECT(current_object)->traffic.shake = 0u;
	}
	OBJECT(current_object)->frame = r0_2;
	negative = traffic_hit_check();
	if (negative)
	{
		traffic_start_hit_sound();
		OBJECT(current_object)->traffic.shake = 1u;
	}
	r0_2 = draw_object(r0_2);
}

/* A vehicle in a line of road traffic behind a 0138EB leader (traffic_car_start_convoy starts the line
   from the leader's list: each one the list's spacing behind the one before it, with its own record):
   moves and draws as the leader's family does (traffic_car_update), but lives exactly as long as the one
   before it (leader): when that one has ended it ends too (screen unk_FA8, the count of the traffic,
   less 1). The only line in the road's lists is in list 25 of object_lists_2E1F0 (a leader of animation
   35C and eight of 35D); the scenarios start it only for a frame or two. */
void traffic_follower_update(void)
{
	if (OBJECT(OBJECT(current_object)->traffic_follower.leader)->flags == 0u)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	traffic_car_update(0u);
	if (OBJECT(current_object)->flags == 0u)
	{
		OBJECT(current_object)->flags = 1u;
		SCREEN(screen)->unk_FA8++;
	}
}

/* The vehicle a road animal pulls (traffic_figure_start_team starts it behind a 013BA3 leader from the
   leader's list): the covered wagons behind the horses (list 1 of object_lists_2E1F0), the sleighs
   behind the reindeer (lists 25 and 26). Moves and draws as the 013AF9 traffic does
   (traffic_stopping_car_update), and lives exactly as long as the one before it (leader): when that one
   has ended it ends too (screen unk_FA8 less 1). */
void pulled_vehicle_update(void)
{
	if (OBJECT(OBJECT(current_object)->pulled_vehicle.leader)->flags == 0u)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	traffic_stopping_car_update();
	if (OBJECT(current_object)->flags == 0u)
	{
		OBJECT(current_object)->flags = 1u;
		SCREEN(screen)->unk_FA8++;
	}
}

/* A traffic car that slows to a stop: as traffic_car_update, but at each segment it counts stop_distance
   down and, once it has run out (stopping), its speed drops by one a segment to 0. The cars of stages 2
   and 12, and what the traffic_figure teams pull (013AD8 runs it). */
void traffic_stopping_car_update(void)
{
	uint32_t r0 = 0, r1 = 0;
	uint16_t stop_distance = 0, speed = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_013B04[state];
	switch (state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_013B04[state]);
	}
	if (CAR(player_car)->at_goal != 0u)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->traffic.position;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
		if ((int16_t)r0 < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
	}
	r0 = (r0 & ~0xFFFFu) | OBJECT(current_object)->traffic.position_fraction;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->traffic.speed);
	r1 = 0x80u;
	if ((int16_t)r0 < (int16_t)r1)
	{
		traffic_car_draw(r0, r1);
		return;
	}
	OBJECT(current_object)->traffic.position++;
	if (OBJECT(current_object)->traffic.stopping == 0u)
	{
		stop_distance = OBJECT(current_object)->traffic.stop_distance;
		OBJECT(current_object)->traffic.stop_distance = stop_distance - 1u;
		if (stop_distance == 1u)
			OBJECT(current_object)->traffic.stopping = 1u;
	}
	else
	{
		speed = OBJECT(current_object)->traffic.speed;
		OBJECT(current_object)->traffic.speed = speed - 1u;
		if ((int16_t)speed <= 1)
			OBJECT(current_object)->traffic.speed = 0;
	}
	for (;;)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1);
		if ((int16_t)r0 < (int16_t)r1)
			break;
		OBJECT(current_object)->traffic.position++;
	}
	traffic_car_draw(r0, r1);
}

/* The leader of a team (the horses pulling a covered wagon in stage 1, the reindeer pulling a sleigh in
   stages 25 and 26): its record starts with the distance and the records of what it pulls, closed by 0;
   starts each (handler 013AD8: traffic_stopping_car_update, kept live as long as the one before it is,
   which ends it) that distance behind the one before (its field 14 the one before, the same
   stop_distance, counted in screen unk_FA8), then becomes a traffic_figure with the record after the
   list. */
void traffic_figure_start_team(void)
{
	uint16_t r0 = 0;
	uint32_t r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(current_object)->traffic.record;
	r0 = (uint16_t)ROM32(r10);
	r11 = current_object;
	r10 += 4u;
	for (;;)
	{
		r12 = ROM32(r10);
		r10 += 4u;
		if (r12 == 0u)
			break;
		r21 = start_object();
		mem_write16(r21 + 0x1Cu, mem_read16(r11 + 0x1Cu) - r0);
		mem_write32(r21 + 4u, pulled_vehicle_update_ADDRESS);
		mem_write16(r21 + 0x84u, mem_read16(r11 + 0x84u));
		mem_write32(r21 + 0x18u, r12);
		mem_write32(r21 + 0x14u, r11);
		r11 = r21;
		SCREEN(screen)->unk_FA8++;
	}
	OBJECT(current_object)->traffic.record = r10;
	OBJECT(current_object)->handler = traffic_figure_update_ADDRESS;
	traffic_figure_update();
}

/* Traffic drawn from a single view (draw_mode 5) instead of by angle: the horses of the covered wagons
   (stage 1), the reindeer (stages 25, 26) and the figures of stages 10 and 24 (animation 185) and 30
   (258, 071). By state: 0 reads its record as traffic_car_start does (draw_mode 4, scale_shift 2 and
   y_offset -2C0 for animation 165; scale_shift 0 for 258 and 185, else 1); 1 once the screen's car has
   reached the goal (car unk_181) ends when 8 segments behind the view; moves along the road by speed,
   slowing to a stop once stop_distance has run out (stopping); ends when traffic_out_of_view; while
   within the draw distance ahead of the view: traffic_hit_check when it has a hit box
   (traffic_start_hit_sound on a hit), draw_object and animation_advance. */
void traffic_figure_update(void)
{
	uint32_t zero = 0, negative = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0, race = 0;
	uint16_t stop_distance = 0, speed = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r10 = OBJECT(current_object)->traffic.record;
		r11 = ROM32(r10);
		r9 = mem_read16(r11);
		r10 += 4u;
		r11 += 2u;
		if (r9 != 0x165u)
		{
			if (r9 == 0x258u || r9 == 0x185u)
				OBJECT(current_object)->scale_shift = 0u;
			else
				OBJECT(current_object)->scale_shift = 1u;
			OBJECT(current_object)->palette = mem_read16(r11);
			OBJECT(current_object)->draw_mode = 5u;
			r11 += 2u;
			OBJECT(current_object)->traffic.passing_sound = mem_read8(r11);
			r11++;
			OBJECT(current_object)->traffic.hit_sound = mem_read8(r11);
			r11++;
			OBJECT(current_object)->traffic.weight = mem_read16(r11);
			OBJECT(current_object)->traffic.hit_box = r11 + 2u;
			OBJECT(current_object)->traffic.side = (uint8_t)ROM16(r10);
			r10 += 2u;
			AS32(OBJECT(current_object)->traffic.x_offset) = (int16_t)ROM16(r10);
			OBJECT(current_object)->traffic.speed = ROM16(r10 + 2u);
			OBJECT(current_object)->state++;
			animation_start(r9);
			return;
		}
		OBJECT(current_object)->scale_shift = 2u;
		OBJECT(current_object)->palette = mem_read16(r11);
		OBJECT(current_object)->draw_mode = 4u;
		r11 += 2u;
		OBJECT(current_object)->traffic.passing_sound = mem_read8(r11);
		r11++;
		OBJECT(current_object)->traffic.hit_sound = mem_read8(r11);
		r11++;
		OBJECT(current_object)->traffic.weight = mem_read16(r11);
		OBJECT(current_object)->traffic.hit_box = r11 + 2u;
		OBJECT(current_object)->traffic.side = (uint8_t)ROM16(r10);
		r10 += 2u;
		AS32(OBJECT(current_object)->traffic.x_offset) = (int16_t)ROM16(r10);
		OBJECT(current_object)->traffic.speed = ROM16(r10 + 2u);
		AS32(OBJECT(current_object)->traffic.y_offset) = (int16_t)0xFD40u;
		OBJECT(current_object)->state++;
		animation_start(r9);
		return;
	case 1:
		if (CAR(player_car)->at_goal != 0u && (int16_t)(OBJECT(current_object)->traffic.position - 8u) < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
		r0 = (uint16_t)(OBJECT(current_object)->traffic.position_fraction + OBJECT(current_object)->traffic.speed);
		r1 = 0x80u;
		if ((int16_t)r0 >= 0x80)
		{
			do
			{
				OBJECT(current_object)->traffic.position++;
				if (OBJECT(current_object)->traffic.stopping == 0u)
				{
					stop_distance = OBJECT(current_object)->traffic.stop_distance;
					OBJECT(current_object)->traffic.stop_distance = stop_distance - 1u;
					if (stop_distance == 1u)
						OBJECT(current_object)->traffic.stopping = 1u;
				}
				else
				{
					speed = OBJECT(current_object)->traffic.speed;
					OBJECT(current_object)->traffic.speed = speed - 1u;
					if ((int16_t)speed <= 1)
						OBJECT(current_object)->traffic.speed = 0;
				}
				r0 -= r1;
			} while ((int16_t)r1 <= (int16_t)r0);
		}
		OBJECT(current_object)->traffic.position_fraction = r0;
		r0_2 = (uint16_t)OBJECT(current_object)->traffic.position;
		zero = traffic_out_of_view((uint16_t)r0_2);
		if (zero)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
		race = SCREEN(screen)->race_copy;
		r1 = (uint16_t)(r0_2 - SCREEN(screen)->unk_FA6);
		if ((int16_t)r0_2 < (int16_t)RACE(race)->unk_46 || (int16_t)RACE(race)->unk_46 <= (int16_t)r1)
			return;
		if (ROM32(OBJECT(current_object)->traffic.hit_box) != 0u)
		{
			negative = traffic_hit_check();
			if (negative)
				traffic_start_hit_sound();
		}
		draw_object(r0_2);
		ignored = animation_advance(0u);
		return;
	default:
		invalid_jump(rom_013BB0[OBJECT(current_object)->state]);
	}
}

/* When the traffic object has a hit_sound, starts an object (013D15) that plays it 15 frames later. */
void traffic_start_hit_sound(void)
{
	uint32_t r21 = 0;

	if (OBJECT(current_object)->traffic.hit_sound != 0u)
	{
		r21 = start_object();
		mem_write32(r21 + 4u, traffic_hit_sound_wait_ADDRESS);
		mem_write8(r21 + 0x10u, 0xFu);
		mem_write8(r21 + 0x12u, OBJECT(current_object)->traffic.hit_sound);
	}
}

/* Plays a road object's own sound 15 frames after the car has hit it (traffic_start_hit_sound starts it
   with the sound of the object's record): counts delay down, then requests sound (request_sound) and
   ends. */
void traffic_hit_sound_wait(void)
{
	uint8_t sound = 0;
	uint8_t delay = 0;

	delay = OBJECT(current_object)->traffic_hit_sound.delay;
	OBJECT(current_object)->traffic_hit_sound.delay = delay - 1u;
	if (delay == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		sound = OBJECT(current_object)->traffic_hit_sound.sound;
		request_sound(sound);
		return;
	}
}

/* Whether road position R0 is outside every stretch of road the race's players see (zero set: the
   traffic there is removed): the stretches traffic_spawner writes at F84 of the screen (a count, then
   each stretch's top and bottom). */
uint32_t traffic_out_of_view(uint16_t r0)
{
	uint32_t zero = 0;
	uint32_t r9 = 0, r10 = 0;
	uint16_t t1 = 0, t2 = 0;

	r10 = screen + 0xF84u;
	r9 = SCREEN(r10 - 0xF84u)->unk_F84;
	r10 += 2u;
	do
	{
		t1 = mem_read16(r10);
		r10 += 2u;
		if (t1 < (uint16_t)r0)
			break;
		t2 = mem_read16(r10);
		r10 += 2u;
		if (t2 < (uint16_t)r0)
		{
			zero = 1u == 0u;
			return zero;
		}
		r9--;
	} while (r9 != 0u);
	zero = 0u == 0u;
	return zero;
}

/* Starts the other branch of the road off to the right (placed on the left branch's stream: the car took
   the left way): direction -1, then fork_branch_draw. */
void fork_branch_start_right(void)
{
	OBJECT(current_object)->handler = fork_branch_draw_ADDRESS;
	OBJECT(current_object)->fork_branch.direction = -1;
	fork_branch_draw();
}

/* Starts the other branch of the road off to the left (on the right branch's stream): direction 1, then
   fork_branch_draw. */
void fork_branch_start_left(void)
{
	OBJECT(current_object)->handler = fork_branch_draw_ADDRESS;
	OBJECT(current_object)->fork_branch.direction = 1;
	fork_branch_draw();
}

/* Draws the branch of the road the car did not take, after a fork, as a strip of sprites curving away to
   one side (direction) along the 40 segments from position: for each segment in view (the road's
   per-segment records from the view, screen + 3000) the piece of road (fork_branch_sprite) goes at the
   road's screen x plus a curve growing with the distance from start_position (rom_101400) times the
   road's scale times direction, the pieces between segments filled in, into the screen's sprite list and
   its segment's depth list, until the strip leaves the screen. Ends once the view (race_copy unk_46)
   passes end_position. */
void fork_branch_draw(void)
{
	uint16_t r0_2 = 0, r9 = 0, end_position = 0, position = 0, start_position = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r13 = 0, r15 = 0, r18 = 0, r19 = 0, r21 = 0, r26 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->fork_branch.position;
		OBJECT(current_object)->fork_branch.start_position = position;
		OBJECT(current_object)->fork_branch.end_position = position + 0x28u;
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_013DF0[OBJECT(current_object)->state]);
	}
	r9 = RACE(SCREEN(screen)->race_copy)->unk_46;
	start_position = (uint16_t)OBJECT(current_object)->fork_branch.start_position;
	end_position = (uint16_t)OBJECT(current_object)->fork_branch.end_position;
	if (end_position <= r9)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	r19 = screen + 0x2FF0u;
	do
	{
		r19 += 0x10u;
	} while (mem_read16(r19) == 0u);
	if (mem_read16(r19) == 0u)
		return;
	r27 = r19;
	r27 = r27 & 0xFFFu;
	r27 = r27 >> 4;
	r27 = (uint16_t)(r27 + r9);
	r19 -= 0x10u;
	do
	{
		r19 += 0x10u;
		if (mem_read16(r19) == 0u)
			return;
		r26 = r19 & 0xFFFu;
		r26 = r26 >> 4;
		r26 = (uint16_t)(r26 + r9);
		t1 = r26 - start_position;
		r26 = t1;
	} while ((int16_t)t1 < 0 || (int16_t)mem_read16(r19 + 0x16u) >= 2);
	r0 = (int16_t)mem_read16(r19 + 6u);
	r18 = 0u;
	if (start_position <= r27)
		r18 = 2u - r0;
	r2 = (int16_t)mem_read16(r19 + 4u);
	r4 = mem_read16(r19);
	if (r4 >= 0x1FF8u)
		r4 = 0x1FF8u;
	r26 = (uint16_t)(r26 << 3);
	r2 += (uint32_t)((int32_t)(((int32_t)(((int16_t)rom_101400[(uint16_t)(r26 + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * r4) >> 12) * OBJECT(current_object)->fork_branch.direction;
	fork_branch_sprite(&r13, &r15);
	for (;;)
	{
		r19 += 0x10u;
		if (mem_read16(r19) == 0u)
			break;
		r1 = (int16_t)mem_read16(r19 + 6u);
		if ((int16_t)r0 <= (int16_t)r1)
			continue;
		r3 = (int16_t)mem_read16(r19 + 4u);
		r21 = mem_read16(r19 + 8u);
		r5 = mem_read16(r19);
		if (r5 >= 0x1FF8u)
			r5 = 0x1FF8u;
		r26 = r19 & 0xFFFu;
		r26 = r26 >> 4;
		r26 = (uint16_t)(r26 + r9);
		if (end_position < r26)
			break;
		r26 = (uint16_t)(r26 - start_position);
		r26 = (uint16_t)(r26 << 3);
		r3 += (uint32_t)((int32_t)(((int32_t)(((int16_t)rom_101400[(uint16_t)(r26 + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * r5) >> 12) * OBJECT(current_object)->fork_branch.direction;
		if ((int32_t)r3 > 0x280 || (int32_t)r3 < -0x280)
			break;
		r10 = SCREEN(screen)->sprite_list_last + 0x10u;
		r8 = r0 - r1;
		r18 += r8;
		if ((int32_t)(r18 - 1u) < 0)
			continue;
		if (r18 == 1u)
		{
			if (SCREEN(screen)->sprite_list_limit <= r10)
				break;
			r18 = 0u;
			mem_write32(r10 + 0x3000u, 0u);
			mem_write32(r10, r12);
			r10 += 4u;
			mem_write16(r10, r13);
			r10 += 2u;
			mem_write16(r10, ((r4 * 0x370u) >> 12) | r14);
			r10 += 2u;
			mem_write16(r10, r1);
			r10 += 2u;
			mem_write16(r10, r3);
			r10 += 2u;
			mem_write32(r10, r15);
			r10 += 4u;
		}
		else
		{
			r6 = r8;
			r7 = 0u;
			r0_2 = (uint16_t)r1;
			do
			{
				if ((int16_t)r0_2 < 2)
				{
					if (SCREEN(screen)->sprite_list_limit <= r10)
						return;
					mem_write32(r10 + 0x3000u, 0u);
					mem_write32(r10, r12);
					r10 += 4u;
					mem_write16(r10, r13);
					r27 = r5;
					r27 = r27 * r8;
					r27 = r27 + r4 * r7;
					r27 = divide_s32(r27, r6);
					r27 = r27 * 0x370u;
					r27 = r27 >> 12;
					r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 | r14);
					r10 += 2u;
					mem_write16(r10, r27);
					r10 += 2u;
					mem_write16(r10, r0_2);
					r0_2++;
					r27 = r3;
					r27 = r27 * r8;
					r27 = r27 + r2 * r7;
					r27 = divide_s32(r27, r6);
					r10 += 2u;
					mem_write16(r10, r27);
					r10 += 2u;
					mem_write32(r10, r15);
					r10 += 4u;
				}
				r8--;
				r18--;
				r7++;
			} while (r18 != 0u);
		}
		r0 = r1;
		r2 = r3;
		r4 = r5;
		r27 = screen + 0x5000u + r21 * 4u;
		if (mem_read16(r27) == 0u)
			mem_write16(r27 + 2u, r10);
		r26 = SCREEN(screen)->sprite_list_last + 0x10u;
		r28 = mem_read16(r27);
		mem_write32(r10, r28);
		r28 = (uint16_t)(r28 + 0x300u);
		mem_write32(r10 + 0x3000u, r28);
		SCREEN(screen)->sprite_list_last = r10;
		mem_write16(r27, (r26 >> 4) | 0x8000u);
		SCREEN(screen)->sprite_groups++;
	}
}

/* Builds the sprite words of the branch's road piece for fork_branch_draw: the first frame of animation
   74 (its size and place in the sprite ROM, the size steps at 0x122B74) and the palette of the stage
   being built (rom_0140C8 by screen unk_A3, into rom_032BA4). */
void fork_branch_sprite(uint32_t *p_r13, uint32_t *p_r15)
{
	uint16_t r13 = 0;
	uint32_t r26 = 0;

	r26 = mem_read32(rom_034308 + 4u);
	r27 = (r27 & ~0xFFFFu) | mem_read16(r26 + 2u);
	r26 = 0x122B74u + mem_read16(r26 + 4u) * 4u;
	r12 = ((uint32_t)ROM16(r26) << 16) | (uint16_t)r27;
	r26 += 2u;
	r27 = (r27 & ~0xFFFFu) | ROM16(r26);
	r26 += 4u;
	r13 = 1u | ROM16(r26);
	r14 = ROM16(r26 + 4u);
	r14 = r14 | SCREEN(screen)->sprite_screen_bit;
	*p_r15 = ((uint32_t)mem_read16(rom_032BA4[rom_0140C8[SCREEN(screen)->unk_A3]]) << 16) | (uint16_t)r27;
	*p_r13 = (*p_r13 & 0xFFFF0000u) | r13;
}

/* One of the two people in a race car (rom_01440E, rom_01442E by occupant), seen only while the car
   shows an animation of its own (its special_body: a crash): then drawn at the car's place and size
   until its animation ends, when the second one lets the car go back to its body. */
void race_car_occupant_update(void)
{
	uint32_t r0 = 0, r9 = 0, object = 0, occupant = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0u;
		occupant = OBJECT(current_object)->race_car_occupant.occupant;
		r9 = rom_01440E[occupant];
		OBJECT(current_object)->palette = rom_01442E[occupant];
		animation_start(r9);
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		if (OBJECT(object)->race_car.special_body == 0u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y = OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
		OBJECT(current_object)->frame = 0u;
		OBJECT(current_object)->unk_41 = 1u;
		OBJECT(current_object)->unk_42 = 1u;
		break;
	case 1:
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		if (OBJECT(object)->race_car.special_body == 0u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y = OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
		OBJECT(current_object)->frame = 0u;
		OBJECT(current_object)->unk_41 = 1u;
		OBJECT(current_object)->unk_42 = 1u;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_014114[OBJECT(current_object)->state]);
	}
	object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
	AS8(OBJECT(object)->unk_CC) = 0u;
	OBJECT(object)->unk_DC = 0u;
	if (OBJECT(object)->race_car.special_body == 0u)
	{
		animation_advance(0u);
		if (OBJECT(current_object)->unk_41 == 0u)
		{
			object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
			OBJECT(current_object)->state--;
			OBJECT(object)->race_car.special_body = OBJECT(current_object)->race_car_occupant.body_back;
			return;
		}
		return;
	}
	OBJECT(current_object)->depth_layer = OBJECT(object)->depth_layer;
	r0 = OBJECT(object)->width_scale;
	if (r0 > 0x100u)
		r0 = 0x100u;
	OBJECT(current_object)->width_scale = r0;
	OBJECT(current_object)->height_scale = r0;
	OBJECT(current_object)->x = OBJECT(object)->x + SCREEN(screen)->sprite_origin_x;
	if (OBJECT(object)->race_car.rival != 0u)
		OBJECT(current_object)->y = ((int32_t)((int16_t)RACE(OBJECT(object)->race_car.record)->unk_56 * r0) >> 8) + OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
	draw_object(r0);
	animation_advance(0u);
	if (OBJECT(current_object)->unk_41 == 0u)
	{
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		OBJECT(current_object)->state--;
		OBJECT(object)->race_car.special_body = OBJECT(current_object)->race_car_occupant.body_back;
		return;
	}
}

/* A rider's gesture on another racer's car (00974C; race_cars_start starts one for its driver, rider =
   model, and one for its passenger, model + 8; only some have a gesture, rom_01444E, the others end at
   once): when the car moves up a place in the race (rider_gesture_check_place) it plays its gesture once
   (the driver raises an arm, a finger up) in place of the rider, and while the car can be seen from the
   angle and the screen's car is racing (rider_gesture_check_view) it is drawn on the car
   (rider_gesture_place) at the car's size and a layer in front; then waits for the next time. */
void rider_gesture_update(void)
{
	uint32_t zero = 0;
	uint8_t r9_2 = 0;
	uint32_t r0 = 0, r9 = 0, object = 0, rider = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0u;
		rider = OBJECT(current_object)->rider_gesture.rider;
		r9 = rom_01444E[rider];
		if (r9 == 0u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->palette = rom_01442E[rider];
		animation_start(r9);
		object = (screen & ~0xFFFFu) | OBJECT(current_object)->rider_gesture.car;
		r9_2 = 0xFFu;
		zero = rider_gesture_check_place(r9_2, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->unk_41 = 1u;
			OBJECT(current_object)->unk_42 = 1u;
			OBJECT(object)->unk_E1 = 0xFFu;
		}
		return;
	case 1:
		object = (screen & ~0xFFFFu) | OBJECT(current_object)->rider_gesture.car;
		r9_2 = 0xFFu;
		zero = rider_gesture_check_place(r9_2, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->unk_41 = 1u;
			OBJECT(current_object)->unk_42 = 1u;
			OBJECT(object)->unk_E1 = 0xFFu;
		}
		return;
	case 2:
		object = (screen & ~0xFFFFu) | OBJECT(current_object)->rider_gesture.car;
		zero = rider_gesture_check_view(object);
		if (zero)
		{
			animation_advance(0u);
			if (OBJECT(current_object)->unk_41 == 0u)
			{
				object = (screen & ~0xFFFFu) | OBJECT(current_object)->rider_gesture.car;
				OBJECT(current_object)->state--;
				OBJECT(object)->unk_E1 = 0u;
				return;
			}
			return;
		}
		OBJECT(current_object)->depth_layer = OBJECT(object)->depth_layer;
		OBJECT(current_object)->depth_layer++;
		r0 = OBJECT(object)->width_scale;
		if (r0 > 0x100u)
			r0 = 0x100u;
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		rider_gesture_place((uint16_t)r0, object);
		draw_object(r0);
		animation_advance(0u);
		if (OBJECT(current_object)->unk_41 == 0u)
		{
			object = (screen & ~0xFFFFu) | OBJECT(current_object)->rider_gesture.car;
			OBJECT(current_object)->state--;
			OBJECT(object)->unk_E1 = 0u;
			return;
		}
		return;
	default:
		invalid_jump(rom_014200[OBJECT(current_object)->state]);
	}
}

/* Whether the gesture shows (zero clear): only while the screen's car object is in state 2 or 4
   (rom_0143A0) and the car is seen from one of the angles that show it (rom_0143A6 by the car's view,
   the high byte of its E2); otherwise clears the car's E1 (the gesture's flag); then
   rider_gesture_check_place. */
uint32_t rider_gesture_check_view(uint32_t r10)
{
	uint32_t zero = 0;
	uint32_t r9 = 0;
	uint8_t t1 = 0;

	r9 = rom_0143A0[OBJECT(CAR(player_car)->object)->state];
	r9 = r9 & rom_0143A6[(uint8_t)(OBJECT(r10)->unk_E2 >> 8u)];
	if (r9 == 0u)
	{
		t1 = OBJECT(r10)->unk_E1 & r9;
		zero = t1 == 0u;
		OBJECT(r10)->unk_E1 = t1;
		return zero;
	}
	return rider_gesture_check_place((uint8_t)r9, r10);
}

/* Notes the racer's place in the race (screen + 320 + player) in its car's E2 and sets the car's E1 (the
   gesture's flag) when it is better than the last one; returns E1 masked with R9 (zero set when there is
   no gesture to show). */
uint32_t rider_gesture_check_place(uint8_t r9, uint32_t r10)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint8_t t1 = 0;

	r0 = mem_read8(screen + 0x320u + OBJECT(current_object)->rider_gesture.player);
	r1 = (uint8_t)OBJECT(r10)->unk_E2;
	AS8(OBJECT(r10)->unk_E2) = r0;
	if (r0 < r1)
		OBJECT(r10)->unk_E1 = 0xFFu;
	t1 = OBJECT(r10)->unk_E1 & r9;
	zero = t1 == 0u;
	OBJECT(r10)->unk_E1 = t1;
	return zero;
}

/* Places the gesture on its car: the car's x and y (plus the screen's scroll, screen unk_14 and unk_16)
   and the rider's offset, rom_014360 and rom_014380 by rider plus the seat's offset for the car's view
   (rom_18D25C by model, the view in the high byte of the car's E2, the driver's or the passenger's),
   scaled by R0 (the car's width_scale). */
void rider_gesture_place(uint16_t r0, uint32_t r10)
{
	uint16_t r6 = 0, r7 = 0, r9 = 0;
	uint32_t r2 = 0, r3 = 0, r11 = 0, model = 0;

	r3 = OBJECT(current_object)->rider_gesture.rider;
	r6 = rom_014360[r3];
	r7 = rom_014380[r3];
	model = OBJECT(current_object)->rider_gesture.model;
	r2 = (uint8_t)(OBJECT(r10)->unk_E2 >> 8u);
	r3 = (uint8_t)(r3 - model);
	r11 = rom_18D25C[model] + (uint16_t)(r2 << 4) + (r3 >> 1);
	r9 = (uint16_t)(OBJECT(r10)->y + SCREEN(screen)->sprite_origin_y + ((int16_t)((uint32_t)(r7 + mem_read16(r11 + 2u)) * r0) >> 8));
	OBJECT(current_object)->x = OBJECT(r10)->x + SCREEN(screen)->sprite_origin_x + ((int16_t)((uint32_t)(r6 + mem_read16(r11)) * r0) >> 8);
	OBJECT(current_object)->y = r9;
}

/* The opening of a race (not in the demo): the view comes down from high above the start line to behind
   the car, taking the car's unk_10C up by 18 and unk_10E down by 3 a step until it is 0 (80 steps from
   FA60 and F0; camera_step), while the opening's sounds play (race_opening_sounds_play; sound_step);
   ends when both are done. */
void race_opening_update(void)
{
	uint16_t view_pitch_intro = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
	case 1:
		break;
	case 2:
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_01447C[OBJECT(current_object)->state]);
	}
	switch (OBJECT(current_object)->race_opening.sound_step)
	{
	case 0:
		race_opening_sounds_start();
		break;
	case 1:
		race_opening_sounds_play();
		break;
	case 2:
		race_opening_sounds_done();
		break;
	default:
		invalid_jump(rom_0144A9[OBJECT(current_object)->race_opening.sound_step]);
	}
	switch (OBJECT(current_object)->race_opening.camera_step)
	{
	case 0:
		OBJECT(current_object)->race_opening.camera_step++;
		return;
	case 1:
		CAR(player_car)->view_shift_intro += 0x12u;
		view_pitch_intro = CAR(player_car)->view_pitch_intro;
		CAR(player_car)->view_pitch_intro = view_pitch_intro - 3u;
		if (view_pitch_intro != 3u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->race_opening.camera_step++;
		return;
	case 2:
		return;
	default:
		invalid_jump(rom_0144EC[OBJECT(current_object)->race_opening.camera_step]);
	}
}

/* Starts the opening's sounds: the first at count 1, the list at 014661. */
void race_opening_sounds_start(void)
{
	OBJECT(current_object)->race_opening.sound_step++;
	OBJECT(current_object)->race_opening.next_count = rom_014660;
	OBJECT(current_object)->race_opening.sound_list = 0x14661u;
}

/* Counts the opening's steps (count) and requests each sound of the list at its count (sound_list:
   sounds 2D, 24, 0A, 21, 2E, 2D, 24 at the counts 1, 20, 65, 120, 150, 160, 190); after the last goes on
   to race_opening_sounds_done. */
void race_opening_sounds_play(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	OBJECT(current_object)->race_opening.count++;
	if (OBJECT(current_object)->race_opening.count < OBJECT(current_object)->race_opening.next_count)
	{
		race_opening_sounds_done();
		return;
	}
	r10 = OBJECT(current_object)->race_opening.sound_list;
	r0 = ROM8(r10);
	r10++;
	request_sound(r0);
	if (ROM8(r10) != 0u)
	{
		OBJECT(current_object)->race_opening.next_count = ROM8(r10);
		OBJECT(current_object)->race_opening.sound_list = r10 + 1u;
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->race_opening.sound_step++;
	race_opening_sounds_done();
}

/* Does nothing: the opening's sounds are over. */
void race_opening_sounds_done(void)
{
}

/* The sounds as the car nears the end of a stage (before each fork's checkpoint and each goal): sound
   24, then sound 53 20 frames later (the list at 01466F: a delay, then sounds and delays closed by 0);
   when the race record's stage count (race_copy unk_60) changes (the checkpoint or the goal passed),
   sound 24 if this screen's player leads the race (screen unk_01 = unk_70), else 28, and it ends. */
void stage_end_sounds_update(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;
	uint8_t countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->stage_end_sounds.stage = RACE(SCREEN(screen)->race_copy)->stage_count;
		r10 = 0x1466Fu;
		OBJECT(current_object)->stage_end_sounds.countdown = ROM8(r10);
		OBJECT(current_object)->stage_end_sounds.sound_list = r10 + 1u;
		return;
	case 1:
		if (OBJECT(current_object)->stage_end_sounds.stage != RACE(SCREEN(screen)->race_copy)->stage_count)
		{
			OBJECT(current_object)->state++;
			return;
		}
		countdown = OBJECT(current_object)->stage_end_sounds.countdown;
		OBJECT(current_object)->stage_end_sounds.countdown = countdown - 1u;
		if (countdown != 1u)
			return;
		r10 = OBJECT(current_object)->stage_end_sounds.sound_list;
		r0 = ROM8(r10);
		r10++;
		request_sound(r0);
		if (ROM8(r10) != 0u)
		{
			OBJECT(current_object)->stage_end_sounds.countdown = ROM8(r10);
			OBJECT(current_object)->stage_end_sounds.sound_list = r10 + 1u;
			return;
		}
		OBJECT(current_object)->state++;
		return;
	case 2:
		if (OBJECT(current_object)->stage_end_sounds.stage == RACE(SCREEN(screen)->race_copy)->stage_count)
			return;
		r0 = 0x24u;
		if (SCREEN(screen)->player != SCREEN(screen)->unk_70)
			r0 = 0x28u;
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		request_sound(r0);
		return;
	default:
		invalid_jump(rom_014520[OBJECT(current_object)->state]);
	}
}

/* Raises and tilts the view before each fork (256 segments before it), so the fork is seen from above.
   By state: 0 waits until the car (race_copy unk_42) reaches its position; 1 each frame the camera's
   height (car unk_10A) up by lift_step to lift_height (1E0 less the car model's height, rom_010BD2 by
   race_copy unk_49; screen unk_F0F = FF when it gets there: the radio's checkpoint message) and its
   pitch (car unk_108) down by tilt_step; 2 holds until the car reaches lower_position (224 segments on);
   3 lowers it back the same way and ends. Does nothing while car unk_137 is set. */
void fork_camera_update(void)
{
	uint16_t r0_2 = 0;
	uint32_t r0 = 0;
	uint16_t lift_step = 0, view_pitch_lift = 0;

	if (CAR(player_car)->unk_137 != 0u)
		return;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (RACE(SCREEN(screen)->race_copy)->unk_42 < (uint16_t)OBJECT(current_object)->fork_camera.position)
			return;
		OBJECT(current_object)->state++;
		r0 = RACE(SCREEN(screen)->race_copy)->unk_49;
		OBJECT(current_object)->fork_camera.lift_height = 0u - rom_010BD2[r0] + 0x1E0u;
		OBJECT(current_object)->fork_camera.lift_step = rom_014674[r0];
		OBJECT(current_object)->fork_camera.tilt_step = rom_014684[r0];
		OBJECT(current_object)->fork_camera.lower_position = OBJECT(current_object)->fork_camera.position + 0xE0u;
		CAR(player_car)->view_pitch_lift = 0u;
		CAR(player_car)->view_shift_lift = 0u;
		return;
	case 1:
		r0_2 = (uint16_t)(CAR(player_car)->view_pitch_lift + OBJECT(current_object)->fork_camera.lift_step);
		if ((int16_t)OBJECT(current_object)->fork_camera.lift_height <= (int16_t)r0_2)
		{
			OBJECT(current_object)->state++;
			r0_2 = OBJECT(current_object)->fork_camera.lift_height;
			SCREEN(screen)->unk_F0F = 0xFFu;
		}
		CAR(player_car)->view_pitch_lift = r0_2;
		CAR(player_car)->view_shift_lift -= OBJECT(current_object)->fork_camera.tilt_step;
		return;
	case 2:
		if (RACE(SCREEN(screen)->race_copy)->unk_42 < OBJECT(current_object)->fork_camera.lower_position)
			return;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_0145A0[OBJECT(current_object)->state]);
	}
	lift_step = OBJECT(current_object)->fork_camera.lift_step;
	view_pitch_lift = CAR(player_car)->view_pitch_lift;
	r0_2 = (uint16_t)(view_pitch_lift - lift_step);
	if ((int16_t)lift_step < (int16_t)view_pitch_lift)
	{
		CAR(player_car)->view_pitch_lift = r0_2;
		CAR(player_car)->view_shift_lift += OBJECT(current_object)->fork_camera.tilt_step;
		return;
	}
	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->unk_01 = 0u;
	CAR(player_car)->view_pitch_lift = 0u;
	CAR(player_car)->view_shift_lift = 0u;
}

/* The end of the race for the screen's car: starts the objects of its ending (object_lists_14A20 by its
   model and whether it won, or object_list_14A60 when it did not finish); when they are done (state 2)
   sound 4F and raises screen unk_94 bit 1, which game step 7 waits for ( the music stopped without a
   finish). */
void race_car_start_ending(void)
{
	bool L_0146F4 = false;
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r10 = 0, r21 = 0, player = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		r0 = OBJECT(current_object)->race_car.model;
		r0 = (uint8_t)(r0 * 2u);
		OBJECT(current_object)->x += 0xA0u;
		OBJECT(current_object)->y += 0xDEu;
		player = SCREEN(screen)->player;
		for (;;)
		{
			if (L_0146F4 || RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
			{
				if (!L_0146F4)
				{
					r0 = object_list_14A60;
					r10 = 0x14A64u;
				}
				do
				{
					r21 = start_object();
					mem_write32(r21 + 4u, mem_read32(r10));
					r10 += 4u;
					mem_write32(r21 + 0x80u, mem_read32(r10));
					mem_write32(r21 + 0x14u, current_object);
					r10 += 4u;
					r0--;
				} while (r0 != 0u);
				return;
			}
			if (mem_read8(screen + 0x320u + player) != 0u)
				r0 = (uint8_t)(r0 + 1u);
			r10 = object_lists_14A20[r0];
			r0 = mem_read32(r10);
			r10 += 4u;
			L_0146F4 = true;
		}
	case 1:
	case 3:
		break;
	case 2:
		OBJECT(current_object)->state++;
		r0_2 = 0x4Fu;
		request_sound(r0_2);
		SCREEN(screen)->flags |= 2u;
		SCREEN(screen)->flags &= 0xFFFFFFEFu;
		if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
		{
			fade_out_music();
			return;
		}
		break;
	default:
		invalid_jump(rom_0146A0[OBJECT(current_object)->state]);
	}
}

/* The player's car in the goal scene: its animation, palette and frame from the race record (race_copy
   unk_3C, unk_3E, unk_4A), its layer, place and scale from the scene object (scene: the car's drawing
   object, 00974C's, whose handler is 014694 at the goal); then goal_actor_start. */
void goal_actor_car_start(void)
{
	uint32_t r9 = 0, object = 0;

	object = OBJECT(current_object)->goal_actor.scene;
	r9 = RACE(SCREEN(screen)->race_copy)->unk_3C;
	OBJECT(current_object)->animation = r9;
	animation_start(r9);
	OBJECT(current_object)->palette = RACE(SCREEN(screen)->race_copy)->unk_3E;
	OBJECT(current_object)->frame = RACE(SCREEN(screen)->race_copy)->unk_4A;
	OBJECT(current_object)->depth_layer = OBJECT(object)->depth_layer;
	OBJECT(current_object)->x = OBJECT(object)->x;
	OBJECT(current_object)->y = OBJECT(object)->y;
	OBJECT(current_object)->width_scale = OBJECT(object)->width_scale;
	OBJECT(current_object)->height_scale = OBJECT(object)->width_scale;
	goal_actor_start();
}

/* The driver in the goal scene: goal_actor_rider_place with the scene object's first rider record (+90). */
void goal_actor_driver_start(void)
{
	uint32_t scene = 0;

	scene = OBJECT(current_object)->goal_actor.scene;
	r12 = scene + 0x90u;
	goal_actor_rider_place(scene);
}

/* The passenger in the goal scene: goal_actor_rider_place with the scene object's second rider record
   (+A0). */
void goal_actor_passenger_start(void)
{
	uint32_t scene = 0;

	scene = OBJECT(current_object)->goal_actor.scene;
	r12 = scene + 0xA0u;
	goal_actor_rider_place(scene);
}

/* Places a rider in the scene's car: from the car's current frame (the car's animation and frame from
   race_copy unk_3C, unk_4A) the view it is drawn from (bit 6 of the frame's attributes picks the
   mirrored set) and the seat's offset; from the rider record (R12: a table of offsets by view, the
   animation of the first view, the palette, the frame) the rider's animation, palette and frame; scaled
   by the scene object's scale onto its place, in its layer; then goal_actor_start. */
void goal_actor_rider_place(uint32_t r10)
{
	uint32_t carry = 0;
	uint32_t r3 = 0, r5 = 0, r6 = 0, r9 = 0, r11 = 0, race = 0, width_scale = 0;

	OBJECT(current_object)->palette = mem_read16(r12 + 6u);
	OBJECT(current_object)->depth_layer = OBJECT(r10)->depth_layer;
	race = SCREEN(screen)->race_copy;
	r11 = mem_read32(rom_034138[RACE(race)->unk_3C] + 4u + RACE(race)->unk_4A * 4u) + 2u;
	r3 = mem_read16(r11);
	r11 += 2u;
	r3 = r3 & 0x40u;
	r3 = r3 >> 6;
	r3 = (uint16_t)(r3 * 0x34u);
	r3 = (uint16_t)(r3 + (uint16_t)(mem_read16(r11) - (uint16_t)OBJECT(r10)->unk_8C) / 3u);
	r11 += 2u;
	r6 = mem_read16(r11);
	r5 = mem_read16(r11 + 2u);
	r11 = mem_read32(r12) + (r3 << 4);
	r5 = (uint16_t)(r5 + mem_read16(r11));
	r6 = (uint16_t)(r6 + mem_read16(r11 + 2u));
	r9 = mem_read16(r12 + 4u) + r3;
	OBJECT(current_object)->animation = r9;
	animation_start(r9);
	OBJECT(current_object)->frame = mem_read8(r12 + 0xAu);
	width_scale = OBJECT(r10)->width_scale;
	OBJECT(current_object)->width_scale = width_scale;
	OBJECT(current_object)->height_scale = width_scale;
	r5 = (int16_t)r5 * width_scale;
	r6 = (int16_t)r6 * width_scale;
	carry = (r5 >> 7) & 1u;
	r5 = (uint16_t)(((int32_t)r5 >> 8) + carry);
	carry = (r6 >> 7) & 1u;
	r6 = (uint16_t)(((int32_t)r6 >> 8) + carry + OBJECT(r10)->y);
	OBJECT(current_object)->x = r5 + OBJECT(r10)->x;
	OBJECT(current_object)->y = r6;
	goal_actor_start();
}

/* Sets an actor of the goal scene to a screen position (draw_mode 0) and goes on to goal_actor_run. */
void goal_actor_start(void)
{
	OBJECT(current_object)->handler = goal_actor_run_ADDRESS;
	OBJECT(current_object)->draw_mode = 0u;
	goal_actor_run();
}

/* Plays the actor's script (script; step is where this frame's step began), a step a frame: a halfword
   of flags, from bit 15: wait (counter to the next halfword, drawn), repeat from step (until the counter
   passes the next halfword), animation_advance, frame =, x +=, x =, y +=, y =, scale += and = (34, 36),
   layer += and =, a new animation, palette =, a sound (request_sound); with bit 0 clear the step ends
   and the actor is drawn (draw_object); with bit 0 set a command (rom_014994): 0 tells the scene object
   it is over (its state = 2), 1 ends the actor, 2 ends the step undrawn, 3 flips it (flags bit 6), 4 car
   unk_34 + 4000, 5 a repeat whose count grows by 8 for each place behind (rom_014A16 by screen + 320 +
   unk_01). */
void goal_actor_run(void)
{
	uint32_t carry = 0;
	uint8_t r9_2 = 0;
	uint16_t r5 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0, script = 0;
	uint16_t t1 = 0, t2 = 0;
	uint32_t t3 = 0, t4 = 0, t5 = 0;

	r10 = OBJECT(current_object)->goal_actor.script;
	for (;;)
	{
		r5 = ROM16(r10);
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		r10 += 2u;
		if (carry)
		{
			OBJECT(current_object)->goal_actor.counter++;
			t1 = ROM16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->goal_actor.counter <= t1)
			{
				r0 = draw_object(r0);
				return;
			}
			OBJECT(current_object)->goal_actor.counter = 0u;
			OBJECT(current_object)->goal_actor.script = r10;
			continue;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->goal_actor.counter++;
			t2 = ROM16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->goal_actor.counter <= t2)
			{
				r10 = OBJECT(current_object)->goal_actor.step;
				OBJECT(current_object)->goal_actor.script = r10;
				continue;
			}
			OBJECT(current_object)->goal_actor.counter = 0u;
			OBJECT(current_object)->goal_actor.script = r10;
			continue;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
			ignored = animation_advance(ignored);
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->frame = (uint8_t)ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->x += ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->x = ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->y += ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->y = ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->width_scale += ROM16(r10);
			OBJECT(current_object)->height_scale += ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->width_scale = ROM16(r10);
			OBJECT(current_object)->height_scale = ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->depth_layer += ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->depth_layer = ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			r9 = ROM16(r10);
			OBJECT(current_object)->animation = r9;
			r9_2 = mem_read8(rom_034138[r9]);
			OBJECT(current_object)->goal_actor.frame_time = r9_2;
			OBJECT(current_object)->goal_actor.frame_countdown = r9_2;
			OBJECT(current_object)->frame = 0u;
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->palette = ROM16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			r0 = ROM16(r10);
			r10 += 2u;
			request_sound((uint8_t)r0);
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		if (carry == 0u)
		{
			t3 = r10;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = t3;
			OBJECT(current_object)->goal_actor.step = script;
			r0 = draw_object(r0);
			return;
		}
		t4 = ROM16(r10);
		r0 = rom_014994[t4];
		r10 += 2u;
		switch (t4)
		{
		case 0:
			OBJECT(OBJECT(current_object)->goal_actor.scene)->state = 2u;
			t3 = r10;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = t3;
			OBJECT(current_object)->goal_actor.step = script;
			r0 = draw_object(r0);
			return;
		case 1:
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		case 2:
			t5 = r10;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = t5;
			OBJECT(current_object)->goal_actor.step = script;
			return;
		case 3:
			OBJECT(current_object)->flags ^= 0x40u;
			t3 = r10;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = t3;
			OBJECT(current_object)->goal_actor.step = script;
			r0 = draw_object(r0);
			return;
		case 4:
			AS16(CAR(player_car)->segment_progress) += 0x4000u;
			t3 = r10;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = t3;
			OBJECT(current_object)->goal_actor.step = script;
			r0 = draw_object(r0);
			return;
		case 5:
			break;
		default:
			invalid_jump(rom_014994[t4]);
		}
		OBJECT(current_object)->goal_actor.counter++;
		r0 = (uint16_t)(ROM16(r10) + ((uint32_t)rom_014A16[mem_read8(screen + 0x320u + SCREEN(screen)->player)] << 3));
		r10 += 2u;
		if (OBJECT(current_object)->goal_actor.counter <= r0)
		{
			r10 = OBJECT(current_object)->goal_actor.step;
			OBJECT(current_object)->goal_actor.script = r10;
			continue;
		}
		OBJECT(current_object)->goal_actor.counter = 0u;
		OBJECT(current_object)->goal_actor.script = r10;
	}
}

/* The road of the race, drawn on its tilemap with line scroll and line select. By state: 0
   road_surface_start; then each frame this frame's lines (road_surface_lines) and, from state 2, those
   of the frame in between (road_surface_halfway_lines); where the first segment's page set
   (segment_page_set) is 0 it starts the stage's palette fades (start_palette_fades with screen unk_208;
   state 3), and at the next one that is not takes it as page_set and fades back (road_surface_fade_back;
   state 2). Each frame the tilemap's page word (at page_register) is the page table's word for page_set
   and near_scroll (five a set). At each new segment (segment past last_segment) acts on the segment's
   event when it changes (after the first 30 segments, event_delay): 4 and 5 change the background within
   the stage (screen unk_203 = FF, unk_204 = 0 or 1, for race_background), 1 and 2 turn the two
   road_tiles_change flags (0A, 0B) on and off, 3 both; a flag turned on starts a road_tiles_change (the
   Golden Gate Bridge in race_to_goal.txt, frames 7659-8325). */
void road_surface_update(void)
{
	uint32_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r13 = 0, r15 = 0, r16 = 0, r18 = 0, r19 = 0, r21 = 0, event = 0, ignored = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_surface_start();
		OBJECT(current_object)->road_surface.event_delay = 0x1Eu;
		OBJECT(current_object)->state++;
		return;
	case 1:
		road_surface_lines(&ignored, &ignored, &ignored, &ignored, &ignored, &ignored, &ignored);
		OBJECT(current_object)->road_surface.points_buffer ^= 0x60u;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->road_surface.segment_page_set == 0u)
		{
			r19 = SCREEN(screen)->unk_208;
			start_palette_fades(r19);
			OBJECT(current_object)->state++;
		}
		break;
	case 2:
		road_surface_lines(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		road_surface_halfway_lines(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(current_object)->road_surface.segment_page_set == 0u)
		{
			r19 = SCREEN(screen)->unk_208;
			start_palette_fades(r19);
			OBJECT(current_object)->state++;
		}
		break;
	case 3:
		road_surface_lines(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		road_surface_halfway_lines(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(current_object)->road_surface.segment_page_set != 0u)
		{
			OBJECT(current_object)->road_surface.page_set = OBJECT(current_object)->road_surface.segment_page_set;
			r19 = SCREEN(screen)->unk_208;
			road_surface_fade_back(r19);
			OBJECT(current_object)->state--;
		}
		break;
	default:
		invalid_jump(rom_016B0C[OBJECT(current_object)->state]);
	}
	r0 = OBJECT(current_object)->road_surface.near_scroll;
	r0 = (uint16_t)(r0 + 0x4FFu);
	mem_write32(OBJECT(current_object)->road_surface.page_register, mem_read32(current_object - 0x24u + ((r0 >> 9) + OBJECT(current_object)->road_surface.page_set * 5u) * 4u));
	if (OBJECT(current_object)->road_surface.segment <= OBJECT(current_object)->road_surface.last_segment)
		return;
	OBJECT(current_object)->road_surface.last_segment = OBJECT(current_object)->road_surface.segment;
	if (OBJECT(current_object)->road_surface.event_delay != 0u)
	{
		OBJECT(current_object)->road_surface.last_event = OBJECT(current_object)->road_surface.event;
		OBJECT(current_object)->road_surface.event_delay--;
		return;
	}
	event = OBJECT(current_object)->road_surface.event;
	if (OBJECT(current_object)->road_surface.last_event == event)
		return;
	OBJECT(current_object)->road_surface.last_event = event;
	switch (event)
	{
	case 4:
		SCREEN(screen)->unk_203 = 0xFFu;
		SCREEN(screen)->unk_204 = 0u;
		return;
	case 5:
		SCREEN(screen)->unk_203 = 0xFFu;
		SCREEN(screen)->unk_204 = 1u;
		return;
	case 0:
		return;
	case 1:
		t1 = ~OBJECT(current_object)->road_surface.first_tiles_flag;
		OBJECT(current_object)->road_surface.first_tiles_flag = t1;
		if (t1 == 0u)
			return;
		road_surface_start_tiles_change();
		return;
	case 3:
		t2 = ~OBJECT(current_object)->road_surface.first_tiles_flag;
		OBJECT(current_object)->road_surface.first_tiles_flag = t2;
		if (t2 != 0u)
			road_surface_start_tiles_change();
		t3 = ~OBJECT(current_object)->road_surface.second_tiles_flag;
		OBJECT(current_object)->road_surface.second_tiles_flag = t3;
		if (t3 == 0u)
			return;
		r21 = start_object();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 0x3Cu, current_object + 0xBu);
		mem_write32(r21 + 0x38u, 0x174B4u);
		mem_write32(r21 + 4u, road_tiles_change_update_ADDRESS);
		return;
	case 2:
		t3 = ~OBJECT(current_object)->road_surface.second_tiles_flag;
		OBJECT(current_object)->road_surface.second_tiles_flag = t3;
		if (t3 == 0u)
			return;
		r21 = start_object();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 0x3Cu, current_object + 0xBu);
		mem_write32(r21 + 0x38u, 0x174B4u);
		mem_write32(r21 + 4u, road_tiles_change_update_ADDRESS);
		return;
	default:
		invalid_jump(rom_016BBC[event]);
	}
}

/* Sets up the road's tilemap from the screen's list at 01710C: its bits in unk_20E204, its scroll (160,
   0), tile bank, mixer bits, page_register, the line tables in the video RAM (line_scroll_table,
   line_select_table) with the line select's start from 100800, the page set of the stage (rom_03CC80 by
   screen unk_A0), the page table (from 01714C) and the stage's palettes (rom_031C38 by stage: the fades'
   list into screen unk_208, the rest queued with queue_palette_list). */
void road_surface_start(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r10 = 0, r11 = 0, r19 = 0;

	r10 = 0x1710Cu + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
	r0 = unk_20E204 & ROM16(r10);
	r10 += 2u;
	unk_20E204 = r0 | ROM16(r10);
	r10 += 2u;
	mem_write16(ROM32(r10), 0x160u);
	r10 += 4u;
	mem_write16(ROM32(r10), 0u);
	unk_20E027 = 0xFFu;
	r10 += 4u;
	mem_write8(ROM32(r10), 0u);
	r10 += 4u;
	OBJECT(current_object)->road_surface.page_register = ROM32(r10);
	r10 += 4u;
	r11 = ROM32(r10);
	r0_2 = mem_read32(r11);
	r0_2 = (r0_2 & ~0xFFFFu) | (r0_2 & 0xFFF0u);
	mem_write32(r11, (r0_2 & ~0xFFu) | (uint8_t)(r0_2 | 8u));
	SCREEN(screen)->flags |= 4u;
	r10 += 4u;
	OBJECT(current_object)->road_surface.line_scroll_table = ROM32(r10);
	r10 = ROM32(r10 + 4u);
	OBJECT(current_object)->road_surface.line_select_table = r10;
	move_halfwords(r10 - 0x1C0u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x640u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0xE40u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x1640u, 0x100800u, 0xE0u);
	OBJECT(current_object)->road_surface.page_set = rom_03CC80[SCREEN(screen)->unk_A0];
	OBJECT(current_object)->road_surface.points_buffer = 0u;
	OBJECT(current_object)->road_surface.last_event = 0u;
	OBJECT(current_object)->road_surface.last_segment = 0u;
	OBJECT(current_object)->road_surface.first_tiles_flag = 0u;
	OBJECT(current_object)->road_surface.second_tiles_flag = 0u;
	move_string(0x1714Cu, 0x1Eu, current_object + 0x18u, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	r19 = rom_031C38[SCREEN(screen)->unk_A0];
	SCREEN(screen)->unk_208 = mem_read32(r19);
	r19 += 4u;
	queue_palette_list(r19);
}

/* Works out this frame's road: from the segments in view (screen + 3000, 16 bytes each, the first at the
   view's position race_copy unk_46) the first one's page set and event (+C, +D) and number (segment),
   each segment's point (+2 depth, +4 x, +6 y) into the points buffer (screen + 3800 by points_buffer),
   and between points the lines of the line scroll (the road's x) and line select (its depth, every other
   pair of segments in the picture's second half: the stripes) tables, from the bottom line up
   (road_surface_fill_lines); a stretch it passes over is filled after (road_surface_fill_skipped_lines);
   the farthest point's line into screen unk_200 and the bottom line's scroll into near_scroll. */
void road_surface_lines(uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r16, uint32_t *p_r18)
{
	uint32_t r4 = *p_r4;
	uint32_t r5 = *p_r5;
	uint32_t r6 = *p_r6;
	bool L_016E09 = false, L_016E73 = false, leave_loop = false, leave_loop_2 = false, leave_loop_3 = false, leave_loop_4 = false, leave_loop_5 = false, next_round = false;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t r0_2 = 0, r18 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r19 = 0, r26 = 0, ignored = 0, sprite_origin_y = 0;
	uint32_t stack_4 = 0;
	uint16_t t1 = 0, t3 = 0;
	uint32_t t2 = 0, t4 = 0, t5 = 0;

	r10 = screen + 0x3000u;
	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 8u);
	r0 = (r0 & 0xCu) << 7;
	r11 = OBJECT(current_object)->road_surface.line_scroll_table + r0 * 4u;
	stack_4 = r11;
	r12 = OBJECT(current_object)->road_surface.line_select_table + r0 * 4u;
	r13 = RACE(SCREEN(screen)->race_copy)->unk_46;
	r14 = r14 & ~0xFFFFu;
	r15 = 0xFFFFFFF4u;
	r16 = 0x800u;
	sprite_origin_y = (int16_t)SCREEN(screen)->sprite_origin_y;
	r18 = 0xFFu;
	r19 = 1u;
	r26 = screen + 0x3800u + OBJECT(current_object)->road_surface.points_buffer * 2u * 4u;
	r0 = 0xE0u;
	while (mem_read16(r10) == 0u)
	{
		r10 += 0x10u;
		r13++;
	}
	OBJECT(current_object)->road_surface.segment_page_set = mem_read8(r10 + 0xCu);
	OBJECT(current_object)->road_surface.event = mem_read8(r10 + 0xDu);
	OBJECT(current_object)->road_surface.segment = r13;
	mem_write16(r26, r13);
	r13 = 0u - ((r13 & ~0xFFFFu) | (r13 & 3u));
	overflow = v60_overflow_add32(r13, 2u, 0u);
	zero = r13 + 2u == 0u;
	negative = (r13 + 2u) >> 31;
	r13 += 2u;
	r26 += 2u;
	if ((negative ^ overflow) | zero)
	{
		r13 += 2u;
		r14 = r14 | 0x100u;
	}
	for (;;)
	{
		t1 = mem_read16(r10);
		r10 += 2u;
		if (t1 == 0u)
			break;
		r3 = mem_read16(r10) >> 2;
		mem_write16(r26, r3);
		r26 += 2u;
		r10 += 2u;
		r2 = 0u - (int16_t)mem_read16(r10);
		mem_write16(r26, r2);
		r26 += 2u;
		r10 += 2u;
		r1 = (int16_t)mem_read16(r10) + sprite_origin_y;
		mem_write16(r26, r1);
		r26 += 2u;
		r10 += 0xAu;
		next_round = false;
		for (;;)
		{
			leave_loop = false;
			do
			{
				leave_loop_2 = false;
				do
				{
					leave_loop_3 = false;
					do
					{
						if (L_016E09 || L_016E73 || L_016E09 || L_016E09 || r19 == 0u)
						{
							if (!L_016E09)
							{
								if (!L_016E73)
								{
									if (!L_016E09)
									{
										if (!L_016E09)
										{
											t2 = r0;
											r9 = t2 - r1;
											if ((int32_t)t2 <= (int32_t)r1)
											{
												r4 = r1;
												r5 = r2;
												r6 = r3;
												r13--;
												if (r13 != 0u)
												{
													next_round = true;
													leave_loop = true;
													leave_loop_2 = true;
													leave_loop_3 = true;
													break;
												}
												r13 = 2u;
												r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
												next_round = true;
												leave_loop = true;
												leave_loop_2 = true;
												leave_loop_3 = true;
												break;
											}
										}
									}
								}
							}
							leave_loop_4 = false;
							for (;;)
							{
								leave_loop_5 = false;
								do
								{
									if (L_016E73 || L_016E09 || L_016E09 || L_016E73 || L_016E09 || L_016E09 || r9 == 1u)
									{
										if (!L_016E73)
										{
											if (L_016E09 || L_016E09 || !L_016E73)
											{
												L_016E09 = false;
												r11 -= 2u;
												mem_write16(r11, r2);
												r0_2 = (uint16_t)r3;
												if ((int16_t)r18 < (int16_t)r0_2)
													r0_2 = (uint16_t)r18;
												r12 = r12 - 2u;
												mem_write16(r12, r0_2 + r14);
												r0 = r1;
											}
										}
										if (L_016E73 || L_016E73 || r0 == 0u)
										{
											for (;;)
											{
												t3 = mem_read16(r10);
												r10 += 2u;
												if (t3 == 0u)
													break;
												mem_write16(r26, mem_read16(r10) >> 2);
												r26 += 2u;
												r10 += 2u;
												mem_write16(r26, 0u - mem_read16(r10));
												r26 += 2u;
												r10 += 2u;
												r1 = (int16_t)mem_read16(r10) + sprite_origin_y;
												mem_write16(r26, r1);
												r26 += 2u;
												r10 += 0xAu;
											}
											SCREEN(screen)->unk_200 = r1;
											mem_write16(r26, 0xFFFFu);
											road_surface_fill_skipped_lines((uint8_t)r15, r16, r18, &r5, &r6);
											OBJECT(current_object)->road_surface.near_scroll = mem_read16(stack_4 - 2u);
											*p_r4 = r4;
											*p_r5 = r5;
											*p_r6 = r6;
											*p_r13 = r13;
											*p_r15 = r15;
											*p_r16 = r16;
											*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
											return;
										}
										r4 = r1;
										r5 = r2;
										r6 = r3;
										r13--;
										if (r13 != 0u)
										{
											next_round = true;
											leave_loop = true;
											leave_loop_2 = true;
											leave_loop_3 = true;
											leave_loop_4 = true;
											leave_loop_5 = true;
											break;
										}
										r13 = 2u;
										r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
										next_round = true;
										leave_loop = true;
										leave_loop_2 = true;
										leave_loop_3 = true;
										leave_loop_4 = true;
										leave_loop_5 = true;
										break;
									}
									road_surface_fill_lines(r0, r1, r2, r3, (uint8_t)r15, r16, r18, &r4, &r5, &r6, &ignored, &ignored, &r11, &zero);
									L_016E73 = zero == 0u;
								} while (L_016E73);
								if (leave_loop_5)
									break;
								L_016E09 = true;
							}
							if (leave_loop_4)
								break;
						}
						t4 = r0;
						r9 = t4 - r1;
						if ((int32_t)t4 <= (int32_t)r1)
						{
							r19 = 0u;
							OBJECT(current_object)->road_surface.skipped_line = 0u;
							r4 = r1;
							r5 = r2;
							r6 = r3;
							r13--;
							if (r13 != 0u)
							{
								next_round = true;
								leave_loop = true;
								leave_loop_2 = true;
								leave_loop_3 = true;
								break;
							}
							r13 = 2u;
							r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
							next_round = true;
							leave_loop = true;
							leave_loop_2 = true;
							leave_loop_3 = true;
							break;
						}
						if ((int32_t)r19 < 0)
						{
							L_016E09 = r9 == 1u;
							if (!L_016E09)
							{
								OBJECT(current_object)->road_surface.skipped_x = r5;
								OBJECT(current_object)->road_surface.skipped_depth = r6;
								road_surface_fill_lines(r0, r1, r2, r3, (uint8_t)r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
								OBJECT(current_object)->road_surface.skipped_x_step = r7;
								OBJECT(current_object)->road_surface.skipped_depth_step = r8;
								r19 = 0u;
								L_016E73 = zero == 0u;
								if (!L_016E73)
									L_016E09 = true;
							}
						}
					} while (L_016E09);
					if (leave_loop_3)
						break;
				} while (L_016E73);
				if (leave_loop_2)
					break;
			} while (L_016E09);
			if (leave_loop)
				break;
			r9 = (r9 - 1u) * 2u;
			r11 -= r9;
			r12 = r12 - r9;
			OBJECT(current_object)->road_surface.skipped_line = r1;
			OBJECT(current_object)->road_surface.skipped_scroll_place = r11;
			OBJECT(current_object)->road_surface.skipped_select_place = r12;
			OBJECT(current_object)->road_surface.skipped_stripe = r14;
			r19 = 0xFFFFFFFFu;
			L_016E09 = true;
		}
		if (next_round)
			continue;
	}
	t5 = shift_by(r0, r18, 32);
	carry = v60_shl_cy(r0, r18, 32);
	zero = t5 == 0u;
	r0 = t5;
	if (carry == 0u)
	{
		r0++;
		for (;;)
		{
			r12 = r12 - 4u;
			r0--;
			if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		SCREEN(screen)->unk_200 = r1;
		mem_write16(r26, 0xFFFFu);
		road_surface_fill_skipped_lines((uint8_t)r15, r16, r18, &r5, &r6);
		OBJECT(current_object)->road_surface.near_scroll = mem_read16(stack_4 - 2u);
		*p_r4 = r4;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r16 = r16;
		*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		SCREEN(screen)->unk_200 = r1;
		mem_write16(r26, 0xFFFFu);
		road_surface_fill_skipped_lines((uint8_t)r15, r16, r18, &r5, &r6);
		OBJECT(current_object)->road_surface.near_scroll = mem_read16(stack_4 - 2u);
		*p_r4 = r4;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r16 = r16;
		*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
		return;
	}
	r0++;
	for (;;)
	{
		r12 = r12 - 4u;
		r0--;
		if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	SCREEN(screen)->unk_200 = r1;
	mem_write16(r26, 0xFFFFu);
	road_surface_fill_skipped_lines((uint8_t)r15, r16, r18, &r5, &r6);
	OBJECT(current_object)->road_surface.near_scroll = mem_read16(stack_4 - 2u);
	*p_r4 = r4;
	*p_r5 = r5;
	*p_r6 = r6;
	*p_r13 = r13;
	*p_r15 = r15;
	*p_r16 = r16;
	*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
}

/* Works out the road of the frame in between: the same lines from the average of this frame's points and
   the last frame's (the two halves of the points buffer, matched by segment), into the next tables;
   flips points_buffer. */
void road_surface_halfway_lines(uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r13, uint8_t r15, uint32_t r16, uint16_t r18)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t r0_2 = 0, r26 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r11 = 0, r13_2 = 0, r19 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint32_t t5 = 0, t6 = 0, t7 = 0;

	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 4u);
	r0 = (r0 & 0xCu) << 7;
	r11 = OBJECT(current_object)->road_surface.line_scroll_table + r0 * 4u;
	r12 = OBJECT(current_object)->road_surface.line_select_table + r0 * 4u;
	r14 = r14 & ~0xFFFFu;
	r19 = 1u;
	r0 = 0xE0u;
	r27 = OBJECT(current_object)->road_surface.points_buffer;
	r28 = r27;
	r28 = r28 ^ 0x60u;
	OBJECT(current_object)->road_surface.points_buffer = r28;
	r27 = r27 * 2u;
	r28 = r28 * 2u;
	r27 = screen + 0x3800u + r27 * 4u;
	r28 = screen + 0x3800u + r28 * 4u;
	r26 = mem_read16(r27);
	t1 = mem_read16(r28);
	r28 = r28 + 2u;
	r13_2 = (r13 & ~0xFFFFu) | t1;
	r27 = r27 + 2u;
	if ((uint16_t)r13_2 != r26)
	{
		if ((uint16_t)r13_2 < r26)
		{
			do
			{
				r28 = r28 + 6u;
				r13_2 = (r13_2 & ~0xFFFFu) | (uint16_t)(r13_2 + 1u);
			} while ((uint16_t)r13_2 < r26);
		}
		else
		{
			do
			{
				r27 = r27 + 6u;
				r26++;
			} while (r26 < (uint16_t)r13_2);
		}
	}
	r13_2 = 0u - ((r13_2 & ~0xFFFFu) | (r13_2 & 3u));
	overflow = v60_overflow_add32(r13_2, 2u, 0u);
	zero = r13_2 + 2u == 0u;
	negative = (r13_2 + 2u) >> 31;
	r13_2 += 2u;
	if ((negative ^ overflow) | zero)
	{
		r13_2 += 2u;
		r14 = r14 | 0x100u;
	}
	while ((int16_t)(mem_read16(r27) | mem_read16(r28)) >= 0)
	{
		t2 = mem_read16(r28);
		r28 = r28 + 2u;
		r3 = shift_by((uint32_t)mem_read16(r27) + t2, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t3 = mem_read16(r28);
		r28 = r28 + 2u;
		r2 = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)t3, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t4 = mem_read16(r28);
		r28 = r28 + 2u;
		r1 = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)t4, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		if (r19 == 0u)
		{
			t5 = r0;
			r9 = t5 - r1;
			if ((int32_t)r1 < (int32_t)t5)
			{
				if (r9 != 1u)
				{
					road_surface_fill_lines(r0, r1, r2, r3, r15, r16, r18, &r4, &r5, &r6, &ignored, &ignored, &r11, &zero);
					if (zero == 0u)
					{
						road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
						return;
					}
				}
				r11 -= 2u;
				mem_write16(r11, r2);
				r0_2 = (uint16_t)r3;
				if ((int16_t)r18 < (int16_t)r0_2)
					r0_2 = (uint16_t)r18;
				r12 = r12 - 2u;
				mem_write16(r12, r0_2 + r14);
				r0 = r1;
				if (r0 == 0u)
				{
					road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
					return;
				}
			}
		}
		else
		{
			t6 = r0;
			r9 = t6 - r1;
			if ((int32_t)r1 < (int32_t)t6)
			{
				if ((int32_t)r19 < 0)
				{
					if (r9 == 1u)
					{
						r11 -= 2u;
						mem_write16(r11, r2);
						r0_2 = (uint16_t)r3;
						if ((int16_t)r18 < (int16_t)r0_2)
							r0_2 = (uint16_t)r18;
						r12 = r12 - 2u;
						mem_write16(r12, r0_2 + r14);
						r0 = r1;
						if (r0 == 0u)
						{
							road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
							return;
						}
					}
					else
					{
						OBJECT(current_object)->road_surface.skipped_x = r5;
						OBJECT(current_object)->road_surface.skipped_depth = r6;
						road_surface_fill_lines(r0, r1, r2, r3, r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
						OBJECT(current_object)->road_surface.skipped_x_step = r7;
						OBJECT(current_object)->road_surface.skipped_depth_step = r8;
						r19 = 0u;
						if (zero == 0u)
						{
							road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
							return;
						}
						r11 -= 2u;
						mem_write16(r11, r2);
						r0_2 = (uint16_t)r3;
						if ((int16_t)r18 < (int16_t)r0_2)
							r0_2 = (uint16_t)r18;
						r12 = r12 - 2u;
						mem_write16(r12, r0_2 + r14);
						r0 = r1;
						if (r0 == 0u)
						{
							road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
							return;
						}
					}
				}
				else
				{
					r9 = (r9 - 1u) * 2u;
					r11 -= r9;
					r12 = r12 - r9;
					OBJECT(current_object)->road_surface.skipped_line = r1;
					OBJECT(current_object)->road_surface.skipped_scroll_place = r11;
					OBJECT(current_object)->road_surface.skipped_select_place = r12;
					OBJECT(current_object)->road_surface.skipped_stripe = r14;
					r19 = 0xFFFFFFFFu;
					r11 -= 2u;
					mem_write16(r11, r2);
					r0_2 = (uint16_t)r3;
					if ((int16_t)r18 < (int16_t)r0_2)
						r0_2 = (uint16_t)r18;
					r12 = r12 - 2u;
					mem_write16(r12, r0_2 + r14);
					r0 = r1;
					if (r0 == 0u)
					{
						road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
						return;
					}
				}
			}
			else
			{
				r19 = 0u;
				OBJECT(current_object)->road_surface.skipped_line = 0u;
			}
		}
		r4 = r1;
		r5 = r2;
		r6 = r3;
		r13_2--;
		if (r13_2 != 0u)
			continue;
		r13_2 = 2u;
		r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
	}
	t7 = shift_by(r0, (uint8_t)r18, 32);
	carry = v60_shl_cy(r0, (uint8_t)r18, 32);
	zero = t7 == 0u;
	r0 = t7;
	if (carry == 0u)
	{
		r0++;
		for (;;)
		{
			r12 = r12 - 4u;
			r0--;
			if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
		return;
	}
	r0++;
	for (;;)
	{
		r12 = r12 - 4u;
		r0--;
		if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	road_surface_fill_skipped_lines(r15, r16, r18, &r5, &r6);
}

/* Fills the road's lines between two of its points, from the lower one up: the line scroll steps evenly
   from one point's x to the other's and the line select from one's depth to the other's (at most R18),
   plus R14 (100 on alternate pairs of segments); returns the two steps. */
void road_surface_fill_lines(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3, uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r7, uint32_t *p_r8, uint32_t *p_r11, uint32_t *p_zero)
{
	uint32_t r4 = *p_r4;
	uint32_t r5 = *p_r5;
	uint32_t r6 = *p_r6;
	uint32_t zero = *p_zero;
	uint16_t r0_2 = 0;
	uint32_t r4_2 = 0, r7 = 0, r8 = 0, r9 = 0;
	uint32_t t1 = 0, t2 = 0;

	r9 = r4 - r1;
	r7 = divide_s32((r5 - r2) << 12, r9);
	r5 = (r5 << 12) + r16;
	r8 = divide_s32((r6 - r3) << 12, r9);
	r6 = (r6 << 12) + r16;
	if ((int32_t)r1 < 0)
	{
		r9 += r1;
		t1 = r4;
		r4_2 = t1 - r0;
		if ((int32_t)r0 < (int32_t)t1)
		{
			r9 -= r4_2;
			r5 -= r7 * r4_2;
			r6 -= r8 * r4_2;
		}
		do
		{
			r5 -= r7;
			r6 -= r8;
			*p_r11 -= 2u;
			mem_write16(*p_r11, shift_signed_by(r5, (uint8_t)r15, 32));
			r0_2 = (uint16_t)shift_signed_by(r6, (uint8_t)r15, 32);
			if ((int16_t)r18 < (int16_t)r0_2)
				r0_2 = (uint16_t)r18;
			r12 = r12 - 2u;
			mem_write16(r12, r0_2 + r14);
			r9--;
		} while (r9 != 0u);
		zero = 1u == 0u;
		*p_r4 = r4_2;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r7 = r7;
		*p_r8 = r8;
		*p_zero = zero;
		return;
	}
	r9--;
	t2 = r4;
	r4_2 = t2 - r0;
	if ((int32_t)r0 < (int32_t)t2)
	{
		r9 -= r4_2;
		r5 -= r7 * r4_2;
		r6 -= r8 * r4_2;
	}
	do
	{
		r5 -= r7;
		r6 -= r8;
		*p_r11 -= 2u;
		mem_write16(*p_r11, shift_signed_by(r5, (uint8_t)r15, 32));
		r0_2 = (uint16_t)shift_signed_by(r6, (uint8_t)r15, 32);
		if ((int16_t)r18 < (int16_t)r0_2)
			r0_2 = (uint16_t)r18;
		r12 = r12 - 2u;
		mem_write16(r12, r0_2 + r14);
		r9--;
	} while (r9 != 0u);
	zero = 0u == 0u;
	*p_r4 = r4_2;
	*p_r5 = r5;
	*p_r6 = r6;
	*p_r7 = r7;
	*p_r8 = r8;
	*p_zero = zero;
}

/* Fills the stretch of lines road_surface_lines passed over (from line skipped_line, at the table places
   it kept) by going on from the kept point with the kept steps down to line DF; nothing when there is
   none. */
void road_surface_fill_skipped_lines(uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r5, uint32_t *p_r6)
{
	uint16_t r0 = 0;
	uint32_t r7 = 0, r9 = 0, r11 = 0, skipped_depth_step = 0, skipped_line = 0;

	skipped_line = OBJECT(current_object)->road_surface.skipped_line;
	if (skipped_line == 0u)
		return;
	*p_r5 = (OBJECT(current_object)->road_surface.skipped_x << 12) + r16;
	*p_r6 = (OBJECT(current_object)->road_surface.skipped_depth << 12) + r16;
	r7 = OBJECT(current_object)->road_surface.skipped_x_step;
	skipped_depth_step = OBJECT(current_object)->road_surface.skipped_depth_step;
	r11 = OBJECT(current_object)->road_surface.skipped_scroll_place;
	r12 = OBJECT(current_object)->road_surface.skipped_select_place;
	r14 = OBJECT(current_object)->road_surface.skipped_stripe;
	r9 = 0xDFu - skipped_line;
	if (0xDFu == skipped_line)
	{
		return;
	}
	do
	{
		*p_r5 += r7;
		*p_r6 += skipped_depth_step;
		mem_write16(r11, shift_signed_by(*p_r5, (uint8_t)r15, 32));
		r0 = (uint16_t)shift_signed_by(*p_r6, (uint8_t)r15, 32);
		r11 += 2u;
		if ((int16_t)r18 < (int16_t)r0)
		{
			r0 = (uint16_t)r18;
			r7 = 0u;
		}
		mem_write16(r12, r0 + r14);
		r12 = r12 + 2u;
		r9--;
	} while (r9 != 0u);
}

/* Starts the road_tiles_change of event 1 (its data at 017444, the even bytes of the page table) with
   the road's first flag (0A). */
void road_surface_start_tiles_change(void)
{
	uint32_t r21 = 0;

	r21 = start_object();
	mem_write8(r21 + 9u, 0u);
	mem_write32(r21 + 0x14u, current_object);
	mem_write32(r21 + 0x3Cu, current_object + 0xAu);
	mem_write32(r21 + 0x38u, 0x17444u);
	mem_write32(r21 + 4u, road_tiles_change_update_ADDRESS);
}

/* Changes the road's tiles for a stretch of road (road_surface starts it when an event turns one of its
   flags on; the Golden Gate Bridge in race_to_goal.txt): copies the four tile pages the road shows
   (source_pages to target_pages, a page a frame), turns its half of the road's page table (table_half:
   the even or odd bytes) to the copies, copies the new look into the pages by the road's page set (16
   halfwords of each a frame for 16 frames) and turns the table back; when the road turns its flag off
   (flag) it changes them back the same way and ends; its data list (data: 017444 for the first flag,
   0174B4 for the second) holds the pages and the bytes for the table. */
void road_tiles_change_update(void)
{
	uint32_t r0 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, data = 0, step = 0;
	uint8_t t1 = 0, pages_left = 0, step_2 = 0, t2 = 0, pages_left_2 = 0, step_3 = 0;

	data = OBJECT(current_object)->road_tiles_change.data;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r0 = OBJECT(OBJECT(current_object)->road_tiles_change.road)->unk_0D;
		OBJECT(current_object)->road_tiles_change.page_set = r0;
		r12 = data - 0xCu + r0 * 4u;
		r13 = data + 0x18u + SCREEN(screen)->index * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)ROM8(r12) << 8) * 4u);
			t1 = ROM8(r13);
			r13++;
			mem_write32(r11, 0x300000u + ((uint32_t)t1 << 8) * 4u);
			r11 += 4u;
			r9--;
			r12 = r12 + 1u;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0u;
		OBJECT(current_object)->road_tiles_change.pages_left = 4u;
		OBJECT(current_object)->state++;
	case 1:
		step = OBJECT(current_object)->road_tiles_change.step;
		move_string(mem_read32(current_object + 0x18u + step * 4u), 0x200u, mem_read32(current_object + 0x28u + step * 4u), 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->road_tiles_change.step++;
		pages_left = OBJECT(current_object)->road_tiles_change.pages_left;
		OBJECT(current_object)->road_tiles_change.pages_left = pages_left - 1u;
		if (pages_left != 1u)
			return;
		r10 = data + 0x20u + SCREEN(screen)->index * 5u * 2u;
		r11 = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		r9 = 3u;
		do
		{
			r12 = r10;
			r8 = 0xAu;
			do
			{
				mem_write8(r11, ROM8(r12));
				r8--;
				r12 = r12 + 1u;
				r11 += 2u;
			} while (r8 != 0u);
			r9--;
		} while (r9 != 0u);
		r10 = current_object + 0x18u;
		r11 = data + OBJECT(current_object)->road_tiles_change.page_set * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)ROM8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0x10u;
		OBJECT(current_object)->state++;
		return;
	case 2:
		r0 = 0x10u;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		step_2 = OBJECT(current_object)->road_tiles_change.step;
		OBJECT(current_object)->road_tiles_change.step = step_2 - 1u;
		if (step_2 != 1u)
			return;
		r10 = data + 0x34u;
		r11 = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, ROM8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->state++;
	case 3:
		if (mem_read8(OBJECT(current_object)->road_tiles_change.flag) != 0u)
			return;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r0 = OBJECT(OBJECT(current_object)->road_tiles_change.road)->unk_0D;
		OBJECT(current_object)->road_tiles_change.page_set = r0;
		r12 = data + r0 * 4u;
		r13 = data + 0x18u + SCREEN(screen)->index * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)ROM8(r12) << 8) * 4u);
			t2 = ROM8(r13);
			r13++;
			mem_write32(r11, 0x300000u + ((uint32_t)t2 << 8) * 4u);
			r11 += 4u;
			r9--;
			r12 = r12 + 1u;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0u;
		OBJECT(current_object)->road_tiles_change.pages_left = 4u;
		OBJECT(current_object)->state++;
	case 4:
		step = OBJECT(current_object)->road_tiles_change.step;
		move_string(mem_read32(current_object + 0x18u + step * 4u), 0x200u, mem_read32(current_object + 0x28u + step * 4u), 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->road_tiles_change.step++;
		pages_left_2 = OBJECT(current_object)->road_tiles_change.pages_left;
		OBJECT(current_object)->road_tiles_change.pages_left = pages_left_2 - 1u;
		if (pages_left_2 != 1u)
			return;
		r10 = data + 0x20u + SCREEN(screen)->index * 5u * 2u;
		r11 = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		r9 = 3u;
		do
		{
			r12 = r10;
			r8 = 0xAu;
			do
			{
				mem_write8(r11, ROM8(r12));
				r8--;
				r12 = r12 + 1u;
				r11 += 2u;
			} while (r8 != 0u);
			r9--;
		} while (r9 != 0u);
		r10 = current_object + 0x18u;
		r11 = data - 0xCu + OBJECT(current_object)->road_tiles_change.page_set * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)ROM8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0x10u;
		OBJECT(current_object)->state++;
		return;
	case 5:
		r0 = 0x10u;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		step_3 = OBJECT(current_object)->road_tiles_change.step;
		OBJECT(current_object)->road_tiles_change.step = step_3 - 1u;
		if (step_3 != 1u)
			return;
		r10 = data + 0x52u;
		r11 = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, ROM8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_0171E0[OBJECT(current_object)->state]);
	}
}

/* The scenery behind the road (the race's first tilemap). By state: 0 sets it up: its register copies
   (017A50 by screen: scroll_x_address, scroll_y_address, page_word_address, tile_bank_address), the
   stage's background (background: rom_017B60 by screen unk_A0, none for stage 0), its tile pages
   (rom_109694, loaded at once with unpack_tile_pages), tile bank and palettes
   (race_background_fade_palettes, queue_palette_list), its objects (race_background_start_objects) and,
   for stages 6, 8 and 1C, its animation (race_background_start_lower_animation,
   race_background_start_upper_animation); 1 waits for a change: a fork's new stage (the car's unk_CA;
   background 2 * screen unk_A3 + the branch, car unk_C9) or the road's events 4 and 5 (screen unk_203;
   background 2A + unk_A3): screen unk_202 set, the new background's pages queued into the other half of
   the pages (page_half; queue_tile_pages); 2 changes over as the view turns away and back (the car's
   unk_2E; change_step): when it starts its palettes, tile bank and the first page word
   (next_page_words), at 2A0 (off the screen) the fades of rom_0319F0 and of the road (rom_031CB8, into
   screen unk_208) and the next, the new stage's objects (screen unk_A1), at 160 its palette fades
   (race_background_fade_palettes) and the next, at 60 the last, at 0 back to state 1 with the animation
   of backgrounds 5, 7, 8, 25 and 26. Each frame scrolls it: x by the car's unk_2E (a quarter of it, all
   of it during a change) less A0, y by the slope (the car's unk_30) and screen unk_B8, kept between 0
   and 180 less the road's top line (screen unk_200; also into screen unk_20C). */
void race_background_update(void)
{
	bool L_017685 = false;
	uint32_t zero = 0;
	uint8_t r1 = 0, background_2 = 0;
	uint16_t r0_2 = 0, r1_2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0, background = 0, index = 0, next_page_words = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0, t3 = 0, t4 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_017530[state];
	switch (state)
	{
	case 0:
		index = SCREEN(screen)->index;
		OBJECT(current_object)->race_background.page_half = index;
		r10 = 0x17A50u + index * 5u * 4u;
		OBJECT(current_object)->race_background.scroll_x_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->race_background.scroll_y_address = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->race_background.tile_bank_address = ROM32(r10);
		r10 += 4u;
		r11 = ROM32(r10);
		OBJECT(current_object)->race_background.page_word_address = r11;
		mem_write32(r11, rom_017AE0[(uint32_t)OBJECT(current_object)->race_background.page_half << 3]);
		r10 = ROM32(r10 + 4u);
		r0 = mem_read32(r10);
		r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
		mem_write32(r10, (r0 & ~0xFFu) | (uint8_t)(r0 | 4u));
		SCREEN(screen)->flags |= 4u;
		r0 = SCREEN(screen)->unk_A0;
		if (r0 != 0u)
			r0 = (uint8_t)~rom_017B60[r0];
		t1 = ~r0;
		OBJECT(current_object)->race_background.background = t1;
		r10 = current_object + 0x2Cu;
		r11 = 0x109684u;
		if ((int8_t)t1 >= 0)
			r11 = rom_109694[(uint8_t)~r0] + 0x10u;
		r12 = r10;
		r0 = OBJECT(current_object)->race_background.page_half * 5u * 2u + 1u;
		r9 = 4u;
		do
		{
			mem_write32(r12, byte_swap32(r0) | mem_read32(r11));
			r0++;
			r9--;
			r11 += 4u;
			r12 = r12 + 4u;
		} while (r9 != 0u);
		mem_write32(r12, 0xFFFFFFFFu);
		unpack_tile_pages(r10);
		mem_write8(OBJECT(current_object)->race_background.tile_bank_address, rom_017BA9[SCREEN(screen)->unk_A0]);
		SCREEN(screen)->unk_202 = 0u;
		SCREEN(screen)->unk_203 = 0u;
		SCREEN(screen)->unk_205 = 0u;
		SCREEN(screen)->unk_206 = 0xFFu;
		SCREEN(screen)->unk_207 = 0xFFu;
		r0 = SCREEN(screen)->unk_A0;
		race_background_start_objects(r0);
		r19 = 0x13F9D4u;
		background = OBJECT(current_object)->race_background.background;
		if ((int8_t)background >= 0)
		{
			r19 = rom_031B14[background];
			race_background_fade_palettes(r19);
			r19 = rom_0318CC[OBJECT(current_object)->race_background.background];
		}
		queue_palette_list(r19);
		OBJECT(current_object)->state++;
		r1 = SCREEN(screen)->unk_A0;
		if (r1 == 6u)
			race_background_start_lower_animation();
		else
		{
			r0 = 0u;
			if (r1 == 8u)
				race_background_start_upper_animation(r0);
			else
			{
				r0 = 1u;
				if (r1 == 0x1Cu)
					race_background_start_upper_animation(r0);
			}
		}
	case 1:
		if (SCREEN(screen)->unk_203 != 0u)
			L_017685 = true;
		else if (CAR(player_car)->unk_CA != 0u)
		{
			CAR(player_car)->unk_CA = 0u;
			L_017685 = true;
		}
		if (L_017685)
		{
			SCREEN(screen)->unk_202 = 0xFFu;
			r0 = SCREEN(screen)->unk_A3;
			if (SCREEN(screen)->unk_203 == 0u)
			{
				r0 = r0 * 2u;
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + CAR(player_car)->branch);
			}
			else
				r0 = (uint8_t)(r0 + 0x2Au);
			OBJECT(current_object)->race_background.background = r0;
			r10 = current_object + 0x2Cu;
			r11 = rom_109694[r0];
			r12 = r10;
			r0 = OBJECT(current_object)->race_background.page_half;
			r0 = r0 ^ 2u;
			OBJECT(current_object)->race_background.page_half = r0;
			r0 = r0 * 5u * 2u + 1u;
			r9 = 9u;
			do
			{
				mem_write32(r12, byte_swap32(r0) | mem_read32(r11));
				r0++;
				r9--;
				r11 += 4u;
				r12 = r12 + 4u;
			} while (r9 != 0u);
			mem_write32(r12, 0xFFFFFFFFu);
			queue_tile_pages(r10);
			r0 = OBJECT(current_object)->race_background.page_half * 2u;
			r1 = SCREEN(screen)->unk_204;
			if (SCREEN(screen)->unk_203 == 0u)
				r1 = CAR(player_car)->branch;
			OBJECT(current_object)->race_background.next_page_words = rom_017AE0_ADDRESS + ((r0 & ~0xFFu) | (uint8_t)(r0 + r1)) * 2u * 2u * 4u;
			OBJECT(current_object)->state++;
			OBJECT(current_object)->race_background.change_step = 0u;
		}
		break;
	case 2:
		r0 = (r0 & ~0xFFFFu) | CAR(player_car)->view_yaw;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 4);
		t2 = (int16_t)r0 >> 4;
		zero = t2 == 0u;
		r0 = (r0 & ~0xFFFFu) | t2;
		if ((int16_t)t2 < 0)
		{
			t3 = 0u - r0;
			zero = t3 == 0u;
			r0 = (r0 & ~0xFFFFu) | t3;
		}
		switch (OBJECT(current_object)->race_background.change_step)
		{
		case 0:
			if (zero == 0u)
			{
				SCREEN(screen)->unk_206 = 0xFFu;
				background = OBJECT(current_object)->race_background.background;
				r19 = rom_0318CC[background];
				queue_palette_list(r19);
				mem_write8(OBJECT(current_object)->race_background.tile_bank_address, rom_017B7F[background]);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 1:
			if ((int16_t)r0 >= 0x2A0)
			{
				SCREEN(screen)->unk_205 = 0xFFu;
				r19 = rom_0319F0[OBJECT(current_object)->race_background.background];
				start_palette_fades(r19);
				r19 = rom_031CB8[OBJECT(current_object)->race_background.background];
				if (r19 == 0u)
				{
					next_page_words = OBJECT(current_object)->race_background.next_page_words;
					mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
					OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
					OBJECT(current_object)->race_background.change_step++;
				}
				else
				{
					SCREEN(screen)->unk_208 = mem_read32(r19);
					r19 += 4u;
					start_palette_fades(r19);
					next_page_words = OBJECT(current_object)->race_background.next_page_words;
					mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
					OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
					OBJECT(current_object)->race_background.change_step++;
				}
			}
			break;
		case 2:
			stack_4 = r0;
			SCREEN(screen)->unk_205 = 0u;
			SCREEN(screen)->unk_207 = 0u;
			r0 = SCREEN(screen)->unk_A1;
			race_background_start_objects(r0);
			r0 = stack_4;
			OBJECT(current_object)->race_background.change_step++;
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(current_object)->race_background.background];
				race_background_fade_palettes(r19);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 3:
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(current_object)->race_background.background];
				race_background_fade_palettes(r19);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 4:
			if ((int16_t)r0 <= 0x60)
			{
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, ROM32(next_page_words));
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 5:
			if (zero)
			{
				SCREEN(screen)->unk_202 = 0u;
				SCREEN(screen)->unk_203 = 0u;
				OBJECT(current_object)->state--;
				background_2 = OBJECT(current_object)->race_background.background;
				if (background_2 == 5u)
					race_background_start_lower_animation();
				else
				{
					r0 = 0u;
					if (background_2 == 7u || background_2 == 8u)
						race_background_start_upper_animation(r0);
					else
					{
						r0 = 2u;
						if (background_2 == 0x25u)
						{
							race_background_start_upper_animation(r0);
							r0_2 = CAR(player_car)->view_yaw;
							if (SCREEN(screen)->unk_202 == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(current_object)->race_background.scroll_x_address, r0_2 - 0xA0u);
							t4 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->unk_B8 - 1u;
							r0_2 = t4;
							if ((int16_t)t4 >= 0)
							{
								r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							SCREEN(screen)->unk_20C = r0_2;
							mem_write16(OBJECT(current_object)->race_background.scroll_y_address, r0_2);
							return;
						}
						r0 = 1u;
						if (background_2 == 0x26u)
						{
							race_background_start_upper_animation(r0);
							r0_2 = CAR(player_car)->view_yaw;
							if (SCREEN(screen)->unk_202 == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(current_object)->race_background.scroll_x_address, r0_2 - 0xA0u);
							t4 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->unk_B8 - 1u;
							r0_2 = t4;
							if ((int16_t)t4 >= 0)
							{
								r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							SCREEN(screen)->unk_20C = r0_2;
							mem_write16(OBJECT(current_object)->race_background.scroll_y_address, r0_2);
							return;
						}
					}
				}
			}
			break;
		default:
			invalid_jump(rom_017738[OBJECT(current_object)->race_background.change_step]);
		}
		break;
	default:
		invalid_jump(rom_017530[state]);
	}
	r0_2 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0_2 = r0_2 >> 2;
	mem_write16(OBJECT(current_object)->race_background.scroll_x_address, r0_2 - 0xA0u);
	t4 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->unk_B8 - 1u;
	r0_2 = t4;
	if ((int16_t)t4 >= 0)
	{
		r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
		if ((int16_t)r1_2 < (int16_t)r0_2)
			r0_2 = (uint16_t)r1_2;
	}
	else
		r0_2 = 0u;
	SCREEN(screen)->unk_20C = r0_2;
	mem_write16(OBJECT(current_object)->race_background.scroll_y_address, r0_2);
}

/* Loads the tiles of the background's lower animation (queue_tile_pages with rom_017904 by screen) and
   starts it (race_background_lower_animation). */
void race_background_start_lower_animation(void)
{
	uint32_t r10 = 0, r21 = 0;

	r10 = rom_017904[SCREEN(screen)->index];
	queue_tile_pages(r10);
	r21 = start_object();
	mem_write8(r21 + 0x10u, 2u);
	mem_write8(r21 + 0x12u, SCREEN(screen)->index);
	mem_write32(r21 + 0x20u, OBJECT(current_object)->race_background.page_word_address);
	mem_write32(r21 + 4u, race_background_lower_animation_wait_ADDRESS);
}

/* Waits until the animation's tiles are loaded (screen unk_B00), then goes on to
   race_background_lower_animation_cycle. */
void race_background_lower_animation_wait(void)
{
	if (SCREEN(screen)->unpack_state != 0u)
		return;
	OBJECT(current_object)->handler = race_background_lower_animation_cycle_ADDRESS;
	race_background_lower_animation_cycle();
}

/* Every 2 frames (delay) puts the next of four page pairs (rom_01790C; step goes two on, keeping to the
   screen's own) into the second half of the background's page word (the lower pages); ends when a change
   of background starts (screen unk_202). */
void race_background_lower_animation_cycle(void)
{
	uint32_t r0 = 0;
	uint8_t delay = 0;

	if (SCREEN(screen)->unk_202 != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	delay = OBJECT(current_object)->race_background_lower_animation.delay;
	OBJECT(current_object)->race_background_lower_animation.delay = delay - 1u;
	if (delay == 1u)
	{
		OBJECT(current_object)->race_background_lower_animation.delay = 2u;
		r0 = OBJECT(current_object)->race_background_lower_animation.step;
		r0 = (uint8_t)(r0 + 2u);
		r0 = r0 & 7u;
		OBJECT(current_object)->race_background_lower_animation.step = r0;
		mem_write16(OBJECT(current_object)->race_background_lower_animation.page_word_address + 2u, rom_01790C[r0]);
	}
}

/* Loads the tiles of upper animation R0 (queue_tile_pages with rom_017988 by R0 and screen) and starts
   it (race_background_upper_animation). */
void race_background_start_upper_animation(uint32_t r0)
{
	uint32_t r10 = 0, r21 = 0;

	r0 = r0 * 2u;
	r10 = rom_017988[(r0 & ~0xFFu) | (uint8_t)(r0 + SCREEN(screen)->index)];
	queue_tile_pages(r10);
	r21 = start_object();
	mem_write8(r21 + 0x10u, 3u);
	mem_write8(r21 + 0x12u, SCREEN(screen)->index);
	mem_write32(r21 + 0x20u, OBJECT(current_object)->race_background.page_word_address);
	mem_write32(r21 + 4u, race_background_upper_animation_wait_ADDRESS);
}

/* Waits until the animation's tiles are loaded (screen unk_B00), then goes on to
   race_background_upper_animation_cycle. */
void race_background_upper_animation_wait(void)
{
	if (SCREEN(screen)->unpack_state != 0u)
		return;
	OBJECT(current_object)->handler = race_background_upper_animation_cycle_ADDRESS;
	race_background_upper_animation_cycle();
}

/* Every 3 frames (delay) puts the next of three page pairs (rom_0179A0) into the first half of the
   background's page word (the upper pages: the schools of fish over the underwater tunnel swim); ends
   when a change of background starts (screen unk_202). */
void race_background_upper_animation_cycle(void)
{
	uint32_t r0 = 0;
	uint8_t delay = 0;

	if (SCREEN(screen)->unk_202 != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	delay = OBJECT(current_object)->race_background_upper_animation.delay;
	OBJECT(current_object)->race_background_upper_animation.delay = delay - 1u;
	if (delay != 1u)
		return;
	OBJECT(current_object)->race_background_upper_animation.delay = 3u;
	r0 = OBJECT(current_object)->race_background_upper_animation.step;
	r0 = (uint8_t)(r0 + 2u);
	if ((int8_t)r0 >= 6)
		r0 = (uint8_t)(r0 - 6u);
	OBJECT(current_object)->race_background_upper_animation.step = r0;
	mem_write16(OBJECT(current_object)->race_background_upper_animation.page_word_address, rom_0179A0[r0]);
}

/* Sets up the screen's two tilemaps (0 and 2 for screen 1, 1 and 3 for screen 2; their register copies
   listed at rom_017A78 by screen) from the 20-byte record R19: their pages (two words), their tile banks
   (two bytes) and their priorities in the mixer (two bytes, the low four bits of mixer registers 22 and
   26, 24 and 28 for screen 2; 0 is off), zoom 1:1, zoom centre and scrolls 0, the second tilemap's row
   scroll and row select off (unk_20E204), the registers and the mixer to be sent (unk_20E027, screen
   unk_94 bit 2); then queues the pages of the list at the record's +C (queue_tile_pages) and the palette
   record at +10 (queue_palette_list). Used by car_presentation, name_entry, route_map, car_select_run,
   entry_countdown_start and title_screen_update. */
void set_up_background(uint32_t r19)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0, r19_2 = 0;
	uint8_t t3 = 0, t4 = 0;
	uint32_t t1 = 0, t2 = 0;

	r10 = rom_017A78[SCREEN(screen)->index];
	r11 = mem_read32(r10);
	t1 = mem_read32(r19);
	r19 += 4u;
	mem_write32(r11, t1);
	t2 = mem_read32(r19);
	r19 += 4u;
	mem_write32(r11 + 8u, t2);
	r10 += 4u;
	r11 = mem_read32(r10);
	t3 = mem_read8(r19);
	r19++;
	mem_write8(r11, t3);
	t4 = mem_read8(r19);
	r19++;
	mem_write8(r11 + 2u, t4);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0x200u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0x200u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	unk_20E204 &= mem_read32(r10);
	unk_20E027 = 0xFFu;
	r11 = mem_read32(r10 + 4u);
	r0 = mem_read32(r11);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	mem_write32(r11, (r0 & ~0xFFu) | (uint8_t)(r0 | mem_read8(r19)));
	r11 += 4u;
	r0 = mem_read32(r11);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	r19++;
	mem_write32(r11, (r0 & ~0xFFu) | (uint8_t)(r0 | mem_read8(r19)));
	SCREEN(screen)->flags |= 4u;
	r19++;
	r10 = mem_read32(r19);
	queue_tile_pages(r10);
	r19_2 = mem_read32(r19 + 4u);
	queue_palette_list(r19_2);
}

/* Starts the background objects of stage R0 (object_lists_181B8; R0 + 1F during a change within the
   stage, screen unk_203), each with its data (field 80) and this object (14). */
void race_background_start_objects(uint32_t r0)
{
	uint32_t r0_2 = 0, r10 = 0, r21 = 0;
	uint32_t t1 = 0;

	if (SCREEN(screen)->unk_203 != 0u)
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 0x1Fu);
	r10 = object_lists_181B8[r0];
	t1 = mem_read32(r10) >> 3;
	r0_2 = t1;
	r10 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r21 = start_object();
			mem_write32(r21 + 4u, mem_read32(r10));
			r10 += 4u;
			mem_write32(r21 + 0x80u, mem_read32(r10));
			mem_write32(r21 + 0x14u, current_object);
			r0_2--;
			r10 += 4u;
		} while (r0_2 != 0u);
	}
}

/* Cycles palettes of the stage's scenery (object_lists_181B8): after delay frames, every step_frames
   frames queues the next palette set of its list (palette_list; a negative entry goes back that many
   bytes) with queue_palette_list, for frames_left frames (or for good when it is 0); it waits while the
   stage's background is changing (screen unk_206) and ends when it changes again (running). */
void palette_cycle_update(void)
{
	uint16_t r0 = 0;
	uint32_t r10 = 0, r19 = 0;
	uint16_t delay = 0, frames_left = 0, step_countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		r10 = OBJECT(current_object)->palette_cycle.data;
		OBJECT(current_object)->palette_cycle.delay = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette_cycle.frames_left = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->palette_cycle.palette_list = ROM32(r10);
		r0 = (uint16_t)ROM32(r10 + 4u);
		OBJECT(current_object)->palette_cycle.step_frames = r0;
		OBJECT(current_object)->palette_cycle.step_countdown = r0;
		OBJECT(current_object)->palette_cycle.running = 0u;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->palette_cycle.delay != 0u)
		{
			delay = OBJECT(current_object)->palette_cycle.delay;
			OBJECT(current_object)->palette_cycle.delay = delay - 1u;
			if (delay != 1u)
				return;
		}
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->palette_cycle.frames_left != 0u)
		{
			frames_left = OBJECT(current_object)->palette_cycle.frames_left;
			OBJECT(current_object)->palette_cycle.frames_left = frames_left - 1u;
			if (frames_left == 1u)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
		}
		else
			OBJECT(current_object)->state++;
		break;
	case 1:
		delay = OBJECT(current_object)->palette_cycle.delay;
		OBJECT(current_object)->palette_cycle.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->palette_cycle.frames_left != 0u)
		{
			frames_left = OBJECT(current_object)->palette_cycle.frames_left;
			OBJECT(current_object)->palette_cycle.frames_left = frames_left - 1u;
			if (frames_left == 1u)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
		}
		else
			OBJECT(current_object)->state++;
		break;
	case 2:
		frames_left = OBJECT(current_object)->palette_cycle.frames_left;
		OBJECT(current_object)->palette_cycle.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_017C08[OBJECT(current_object)->state]);
	}
	switch (OBJECT(current_object)->palette_cycle.running)
	{
	case 0:
		if (SCREEN(screen)->unk_206 != 0u)
			return;
		OBJECT(current_object)->palette_cycle.running++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017C68[OBJECT(current_object)->palette_cycle.running]);
	}
	if (SCREEN(screen)->unk_206 != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	step_countdown = OBJECT(current_object)->palette_cycle.step_countdown;
	OBJECT(current_object)->palette_cycle.step_countdown = step_countdown - 1u;
	if (step_countdown != 1u)
		return;
	OBJECT(current_object)->palette_cycle.step_countdown = OBJECT(current_object)->palette_cycle.step_frames;
	r10 = OBJECT(current_object)->palette_cycle.palette_list;
	r19 = ROM32(r10);
	r10 += 4u;
	if ((int32_t)r19 < 0)
	{
		r10 += r19;
		r19 = mem_read32(r10);
		r10 += 4u;
	}
	OBJECT(current_object)->palette_cycle.palette_list = r10;
	queue_palette_list(r19);
}

/* Fades the colours of the stage toward evening after a while (object_lists_181B8: Hawaii's sunset after
   900 frames, stage 26's dusk after 300 and its night after 600): waits count frames (not while the
   background changes, screen unk_206), then starts a palette fade (start_palette_fade) for each entry of
   its list (fade_list: from, to, the palette, the speed), keeps them in fades and their number in count,
   and goes on to sunset_fade_wait. */
void sunset_fade_start(void)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r17 = 0, r18 = 0, r19 = 0, r21 = 0, data = 0;
	uint32_t count = 0, t1 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		data = OBJECT(current_object)->sunset_fade.data;
		OBJECT(current_object)->sunset_fade.count = ROM32(data);
		OBJECT(current_object)->sunset_fade.fade_list = ROM32(data + 4u);
		OBJECT(current_object)->state++;
		if ((uint16_t)OBJECT(current_object)->sunset_fade.count != 0u)
		{
			count = OBJECT(current_object)->sunset_fade.count;
			OBJECT(current_object)->sunset_fade.count = count - 1u;
			if (count != 1u)
				return;
		}
		OBJECT(current_object)->state++;
		break;
	case 1:
		count = OBJECT(current_object)->sunset_fade.count;
		OBJECT(current_object)->sunset_fade.count = count - 1u;
		if (count != 1u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017CB8[OBJECT(current_object)->state]);
	}
	if (SCREEN(screen)->unk_206 != 0u)
		return;
	r19 = OBJECT(current_object)->sunset_fade.fade_list;
	t1 = ROM32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->sunset_fade.count = r18;
	r17 = current_object + 0x18u;
	do
	{
		r10 = ROM32(r19);
		r11 = ROM32(r19 + 4u);
		r19 += 8u;
		r12 = (r12 & ~0xFFFFu) | ROM16(r19);
		r19 += 2u;
		r0 = ROM8(r19);
		r19++;
		r1 = ROM8(r19);
		r19++;
		r21 = start_palette_fade(r0, r1, r10, r11);
		mem_write32(r17, r21);
		r18--;
		r17 += 4u;
	} while (r18 != 0u);
	OBJECT(current_object)->handler = sunset_fade_wait_ADDRESS;
}

/* Waits for its palette fades to finish, then ends; if the stage's background starts changing (screen
   unk_206) it stops them and ends. */
void sunset_fade_wait(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = current_object + 0x18u;
	r9 = OBJECT(current_object)->sunset_fade.count;
	do
	{
		r1 = mem_read32(r10);
		r10 += 4u;
		if (r1 != 0u)
		{
			r1 = mem_read32(r1);
			r0 |= r1;
			if ((r1 & 1u) == 0u)
				mem_write32(r10 - 4u, 0u);
		}
		r9--;
	} while (r9 != 0u);
	if ((r0 & 1u) == 0u)
		OBJECT(current_object)->flags = 0u;
	if (SCREEN(screen)->unk_206 == 0u)
		return;
	r10 = current_object + 0x18u;
	r9 = OBJECT(current_object)->sunset_fade.count;
	do
	{
		r1 = mem_read32(r10);
		r10 += 4u;
		if (r1 != 0u)
			mem_write8(r1, 0u);
		r9--;
	} while (r9 != 0u);
	OBJECT(current_object)->flags = 0u;
}

void sub_017D70(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->palette = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(r9);
	OBJECT(current_object)->handler = sub_017DB0_ADDRESS;
	sub_017DB0();
}

void sub_017DB0(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->x = 0u - (((r0 - OBJECT(current_object)->unk_20) & 0x3FFu) - 0x2A0u);
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017DE4[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->unk_24);
		OBJECT(current_object)->y = 0u - r0_2;
		r0_2 = draw_object(r0_2);
		return;
	}
	if (OBJECT(current_object)->unk_0D != 0u)
		OBJECT(current_object)->flags = 0u;
}

/* Sets up a sprite on the background a little nearer than the panorama (the hill of the Acropolis with
   the Parthenon, stages 14 and 19): from its data the animation, palette, panorama_x, height and
   half_width; draw_mode A, layer 1; straight to state 1 when the background is being set up (screen
   unk_207); then backdrop_near_update. */
void backdrop_near_start(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop.leaving = 0u;
	r10 = OBJECT(current_object)->backdrop.data;
	r9 = ROM32(r10);
	r10 += 4u;
	OBJECT(current_object)->palette = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop.panorama_x = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop.height = ROM16(r10);
	OBJECT(current_object)->backdrop.half_width = ROM16(r10 + 2u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(r9);
	OBJECT(current_object)->handler = backdrop_near_update_ADDRESS;
	backdrop_near_update();
}

/* Keeps the sprite on the background, moving with its panorama a quarter faster (nearer): x from the
   car's heading (car unk_2E, a quarter of it until screen unk_202 is set) less panorama_x, y from the
   background's height (screen unk_20C) and height, but not below screen unk_200's line. By state: 0
   waits until it is off the screen (so it does not pop up in view); 1 draw_object while on the screen;
   off it, it ends once leaving. */
void backdrop_near_update(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->backdrop.leaving |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(current_object)->backdrop.panorama_x) & 0x3FFu;
	OBJECT(current_object)->x = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017E98[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (OBJECT(current_object)->backdrop.leaving != 0u)
			OBJECT(current_object)->flags = 0u;
		return;
	}
	r1 = OBJECT(current_object)->backdrop.height;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - r1);
	r1 -= SCREEN(screen)->unk_200;
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(current_object)->y);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(current_object)->y = 0u - r0_2;
	r0_2 = draw_object(r0_2);
}

/* Sets up a landmark on the horizon that grows as the car approaches (Tiananmen in stage 10, the castle
   of stages 16 and 17...): as backdrop_near_start, layer 2, and approach_start = the car's segment (car
   unk_42) less the distance in its data; then backdrop_landmark_update. */
void backdrop_landmark_start(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop_landmark.leaving = 0u;
	r10 = OBJECT(current_object)->backdrop_landmark.data;
	r9 = ROM32(r10);
	r10 += 4u;
	OBJECT(current_object)->palette = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop_landmark.panorama_x = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop_landmark.height = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop_landmark.half_width = ROM16(r10);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 2u;
	animation_start(r9);
	OBJECT(current_object)->backdrop_landmark.approach_start = 0u - ROM32(r10 + 2u) + CAR(player_car)->window_segment;
	OBJECT(current_object)->handler = backdrop_landmark_update_ADDRESS;
	backdrop_landmark_update();
}

/* Keeps the landmark on the background's panorama (as backdrop_near_update, the same speed as the
   panorama) and grows it with the distance travelled: its scale (width_scale, height_scale) follows the
   sine of (car unk_42 - approach_start) / 2 (rom_100C00) up to full size (100, state 2). By state: 0
   waits until it is off the screen; 1 and 2 drawn while on the screen; off it, it ends once leaving. */
void backdrop_landmark_update(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->backdrop_landmark.leaving |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(current_object)->backdrop_landmark.panorama_x) & 0x3FFu;
	OBJECT(current_object)->x = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		r0_2 = CAR(player_car)->window_segment;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->backdrop_landmark.approach_start);
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		break;
	case 1:
		r0_2 = CAR(player_car)->window_segment;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->backdrop_landmark.approach_start);
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->width_scale = r0;
		OBJECT(current_object)->height_scale = r0;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017F7C[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (OBJECT(current_object)->backdrop_landmark.leaving != 0u)
			OBJECT(current_object)->flags = 0u;
		return;
	}
	r1 = OBJECT(current_object)->backdrop_landmark.height;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - r1);
	r1 -= SCREEN(screen)->unk_200;
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(current_object)->y);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(current_object)->y = 0u - r0_2;
	r0_2 = draw_object(r0_2);
}

void sub_018009(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->palette = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(r9);
	OBJECT(current_object)->unk_10 = 0x120u;
	OBJECT(current_object)->handler = sub_018050_ADDRESS;
	sub_018050();
}

void sub_018050(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, palette = 0;
	uint32_t r0_2 = 0;
	uint16_t t1 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->x = 0u - (((r0 - OBJECT(current_object)->unk_20) & 0x3FFu) - 0x2A0u);
	animation_advance(0u);
	t1 = OBJECT(current_object)->unk_10;
	palette = OBJECT(current_object)->palette;
	OBJECT(current_object)->palette = t1;
	OBJECT(current_object)->unk_10 = palette;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_018094[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->unk_24);
		OBJECT(current_object)->y = 0u - r0_2;
		r0_2 = draw_object(r0_2);
		return;
	}
	if (OBJECT(current_object)->unk_0D != 0u)
		OBJECT(current_object)->flags = 0u;
}

/* Sets up a sprite on the background that is drawn every other frame, so it looks faint (the pyramids of
   stages 13 and 20, the spray of stage 4's falls): as backdrop_near_start, layer 1, frame_count 0; then
   backdrop_translucent_update. */
void backdrop_translucent_start(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop_translucent.leaving = 0u;
	r10 = OBJECT(current_object)->backdrop_translucent.data;
	r9 = ROM32(r10);
	r10 += 4u;
	OBJECT(current_object)->palette = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop_translucent.panorama_x = ROM16(r10);
	r10 += 2u;
	OBJECT(current_object)->backdrop_translucent.height = ROM16(r10);
	OBJECT(current_object)->backdrop_translucent.half_width = ROM16(r10 + 2u);
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(r9);
	OBJECT(current_object)->backdrop_translucent.frame_count = 0u;
	OBJECT(current_object)->handler = backdrop_translucent_update_ADDRESS;
	backdrop_translucent_update();
}

/* Keeps the sprite on the background's panorama (as backdrop_landmark_update), animates it and draws it
   only on even frames (frame_count), so it looks faint. By state: 0 waits until it is off the screen; 1
   drawn while on the screen; off it, it ends once leaving. */
void backdrop_translucent_update(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->backdrop_translucent.leaving |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->x = 0u - (((r0 - OBJECT(current_object)->backdrop_translucent.panorama_x) & 0x3FFu) - 0x2A0u);
	animation_advance(0u);
	OBJECT(current_object)->backdrop_translucent.frame_count++;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01814C[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (OBJECT(current_object)->backdrop_translucent.leaving != 0u)
			OBJECT(current_object)->flags = 0u;
		return;
	}
	if ((OBJECT(current_object)->backdrop_translucent.frame_count & 1u) != 0u)
		return;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->backdrop_translucent.height);
	OBJECT(current_object)->y = 0u - r0_2;
	r0_2 = draw_object(r0_2);
}

/* Whether the background sprite is wholly off the screen (zero set; it clears flags bit 5 then): its x
   (field 30) more than half_width beyond the screen's edges (0..140). */
uint32_t backdrop_off_screen(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0, r2 = 0, half_width = 0;

	r0 = 0xA0u;
	r1 = (uint16_t)(OBJECT(current_object)->x - 0xA0u);
	half_width = OBJECT(current_object)->backdrop.half_width;
	r2 = (uint16_t)(0u - r1 - half_width);
	if ((int16_t)(r1 - half_width) > 0xA0 || (int16_t)r0 < (int16_t)r2)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		zero = 0u == 0u;
		return zero;
	}
	zero = 1u == 0u;
	return zero;
}

/* Starts a palette_fade for each 12-byte record of list R19 (a 32-bit length in bytes, then records:
   from, to, palette offset, blocks, period). */
void start_palette_fades(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r18 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r10 = mem_read32(r19);
			r11 = mem_read32(r19 + 4u);
			r19 += 8u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
			r19 += 2u;
			r0 = mem_read8(r19);
			r19++;
			r1 = mem_read8(r19);
			r19++;
			start_palette_fade(r0, r1, r10, r11);
			r18--;
		} while (r18 != 0u);
	}
}

/* Starts the palette fades of list R19 backwards (start_palette_fade from each record's second palette
   to its first): the road's colours back after a change of page set. */
void road_surface_fade_back(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r18 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r11 = mem_read32(r19);
			r19 += 4u;
			r10 = mem_read32(r19);
			r19 += 4u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
			r19 += 2u;
			r0 = mem_read8(r19);
			r19++;
			r1 = mem_read8(r19);
			r19++;
			start_palette_fade(r0, r1, r10, r11);
			r18--;
		} while (r18 != 0u);
	}
}

/* Starts the palette fades of list R19 (start_palette_fade for each record) and a
   race_background_fade_watch that keeps them; with an empty list clears screen unk_206 (the background's
   palettes are changing) at once. */
void race_background_fade_palettes(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r17 = 0, r18 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 == 0u)
	{
		SCREEN(screen)->unk_206 = 0u;
		return;
	}
	r21 = start_object();
	mem_write32(r21 + 4u, race_background_fade_watch_update_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	mem_write32(r21 + 0x10u, r18);
	r17 = r21 + 0x80u;
	do
	{
		r10 = mem_read32(r19);
		r11 = mem_read32(r19 + 4u);
		r19 += 8u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
		r19 += 2u;
		r0 = mem_read8(r19);
		r19++;
		r1 = mem_read8(r19);
		r19++;
		r21 = start_palette_fade(r0, r1, r10, r11);
		mem_write32(r17, r21);
		r18--;
		r17 += 4u;
	} while (r18 != 0u);
}
