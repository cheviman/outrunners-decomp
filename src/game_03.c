/**
 *    Name:        game_03.c
 *    Description: the game's functions, part 03 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets the engine's pitch from the revs (car revs) each frame, or silences it in the demo without sound. */
void player_car_engine_sound(void)
{
	uint32_t sound_page = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u || advertise_sound != 0u)
	{
		sound_page = (uint32_t)SCREEN(screen)->index << 8;
		mem_write8(sound_page + SOUND_ENGINE_REVS_1, CAR(player_car)->revs >> 8);
		mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, 0x20u);
		return;
	}
	sound_page = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(sound_page + SOUND_ENGINE_REVS_1, 0u);
	mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, mem_read8(sound_page + SOUND_ENGINE_REQUESTS_1) | 0x20u);
}

/* set_tyre_noise, unless the car is off the road (car off_road), where the off-road noise plays. Used by
   player_car_drive, player_car_engine and gear_indicator_update. */
void set_tyre_noise_on_road(uint8_t tyre_noise)
{
	if (CAR(player_car)->off_road == 0u)
	{
		set_tyre_noise(tyre_noise);
		return;
	}
}

/* Sets the noise of the screen's car's tyres on the sound board (byte 718 of the screen's page; the
   sound program keeps it while the game keeps writing it and keys it off 255 of its main loop's passes
   after the last): wheelspin, skids, off-road rumble. In the demo without sound it mutes the engine
   instead. Used by player_car (player_car_engine, player_car_off_road_shake, the goal) and 00664E. */
void set_tyre_noise(uint8_t tyre_noise)
{
	uint32_t sound_page = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u || advertise_sound != 0u)
	{
		mem_write8(((uint32_t)SCREEN(screen)->index << 8) + SOUND_TYRE_NOISE_1, tyre_noise);
		return;
	}
	sound_page = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(sound_page + SOUND_ENGINE_REVS_1, 0u);
	mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, mem_read8(sound_page + SOUND_ENGINE_REQUESTS_1) | 0x20u);
}

/* Fades out the screen's music with 20 (fade_out_music_at): a step every 4 x (20 + 1) of the sound
   program's ticks. */
void fade_out_music(void)
{
	uint8_t fade_delay = 0;

	fade_delay = 0x20u;
	fade_out_music_at(fade_delay);
}

/* Fades out the screen's music: R0 into byte 18 of the screen's first track in the shared sound RAM
   (700918 for screen 1, 701118 for screen 2; music_fade_addresses), which has the sound program lower
   the music's volume, from its last CC 10, by one every 4 x (R0 + 1) of its ticks; at 0 it stops the
   music's player and its MultiPCM voices (CC 50 7E). 20 from fade_out_music, 3F and 7F from
   car_radio_update, 7F from goal_update. */
void fade_out_music_at(uint8_t fade_delay)
{
	mem_write8(music_fade_addresses[SCREEN(screen)->index], fade_delay);
}

/* Sends sound command 1F straight to the screen's request byte in the shared sound RAM (700700 + 100 *
   screen, without going through the ring of requests) and mutes the car's engine (710 = 0, 712 bit 5, as
   set_tyre_noise does in the silent demo). As the game modes start. Command 1F plays song 03, then FF:
   song 03 sends CC 50 7F on channel E, which stops every MultiPCM voice of the screen, and FF clears the
   screen's eight sequence players (music and effects); FM notes go on. */
void reset_screen_sound(void)
{
	uint32_t sound_page = 0;

	sound_page = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(sound_page + SOUND_COMMAND_1, 0x1Fu);
	mem_write8(sound_page + SOUND_ENGINE_REVS_1, 0u);
	mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, mem_read8(sound_page + SOUND_ENGINE_REQUESTS_1) | 0x20u);
}

/* Returns the 12-bit angle of the vector (R0 - R2, R1 - R3). Used by player_car_drive. */
uint32_t vector_angle(uint32_t x, uint32_t y, uint32_t from_x, uint32_t from_y)
{
	uint32_t angle = 0, distance = 0, octant = 0, short_side = 0;
	uint32_t dx = 0, dy = 0, long_side = 0;

	octant = 0u;
	dx = x - from_x;
	distance = dx;
	if ((int32_t)dx < 0)
	{
		distance = 0u - distance;
		octant = (uint8_t)(octant + 1u);
	}
	dy = y - from_y;
	short_side = dy;
	if ((int32_t)dy < 0)
	{
		short_side = 0u - short_side;
		octant = (uint8_t)(octant + 2u);
	}
	if ((int32_t)distance < (int32_t)short_side)
	{
		long_side = short_side;
		short_side = distance;
		distance = long_side;
		octant = (uint8_t)(octant + 4u);
	}
	if (distance == 0u)
		return distance;
	angle = arctangents[divide_u32((short_side << 10) + (distance >> 1), distance)];
	angle = (uint16_t)(angle * octant_angle_factors[octant]);
	angle = (uint16_t)(angle + octant_base_angles[octant]);
	angle = angle & 0xFFFu;
	return angle;
}

/* Returns the angles from a point (R3, R4, R5: x, y, z) toward another (R0, R1, R2): the pitch in R0,
   the yaw in R1. Used by player_car_place_view. */
void vector_angles(uint32_t z, uint32_t from_x, uint32_t from_y, uint32_t from_z, uint32_t *p_x, uint32_t *p_y)
{
	uint32_t x = *p_x;
	uint32_t y = *p_y;
	uint32_t distance = 0, height = 0, octant = 0, pitch = 0, short_side = 0, yaw = 0, yaw_factor = 0;
	uint32_t dx = 0, dz = 0, long_side = 0, dy = 0;

	octant = 0u;
	dx = x - from_x;
	distance = dx;
	if ((int32_t)dx < 0)
	{
		distance = 0u - distance;
		octant = (uint8_t)(octant + 1u);
	}
	dz = z - from_z;
	short_side = dz;
	if ((int32_t)dz < 0)
	{
		short_side = 0u - short_side;
		octant = (uint8_t)(octant + 2u);
	}
	if ((int32_t)distance < (int32_t)short_side)
	{
		long_side = short_side;
		short_side = distance;
		distance = long_side;
		octant = (uint8_t)(octant + 4u);
	}
	if (distance == 0u)
	{
		pitch = 0u;
		height = 0u;
		*p_x = pitch;
		*p_y = height;
		return;
	}
	yaw = arctangents[divide_u32((short_side << 10) + (distance >> 1), distance)];
	yaw = (uint16_t)(yaw * octant_angle_factors[octant]);
	yaw = (uint16_t)(yaw + octant_base_angles[octant]);
	yaw = yaw & 0xFFFu;
	if ((int8_t)octant < 4)
		yaw_factor = sine_table[yaw];
	else
		yaw_factor = cosine_table[yaw];
	distance = divide_s32((distance << 14) + ((int32_t)yaw_factor >> 1), yaw_factor);
	octant = 5u;
	if ((int32_t)distance < 0)
		distance = 0u - distance;
	dy = y - from_y;
	height = dy;
	if ((int32_t)dy < 0)
	{
		height = 0u - height;
		octant = (uint8_t)(octant - 1u);
	}
	pitch = arctangents[divide_u32((height << 10) + (distance >> 1), distance)];
	pitch = (uint16_t)(pitch * octant_angle_factors[octant]);
	pitch = (uint16_t)(pitch + octant_base_angles[octant]);
	pitch = pitch & 0xFFFu;
	height = yaw;
	*p_x = pitch;
	*p_y = height;
}

/* Builds the hit boxes of the race's cars at screen + 510: how many, then for each its player, race
   record and box (player_car_write_hit_box from its segment, model, place along and across; screen
   own_hit_box the screen's own), and the screen's car's box from its own fields at screen + 5B0. */
void player_car_build_hit_boxes(void)
{
	uint16_t road_x = 0, segment = 0, segment_fraction = 0;
	uint32_t hit_box = 0, model = 0, player = 0, racer_list = 0, racer_player = 0, racers_left = 0;

	racer_list = SCREEN(screen)->host_record + 0x20u;
	racers_left = RACE(SCREEN(screen)->host_record)->racer_count;
	player = SCREEN(screen)->player;
	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
		racers_left = 1u;
	hit_box = screen + 0x510u;
	SCREEN(hit_box - 0x510u)->hit_box_count = racers_left;
	hit_box += 4u;
	do
	{
		racer_player = mem_read8(racer_list);
		racer_list++;
		if ((SCREEN(screen)->demo_flags & 0x100u) != 0u || player == racer_player)
		{
			racer_player = player;
			SCREEN(screen)->own_hit_box = hit_box;
		}
		mem_write16(hit_box, racer_player);
		r12 = RACE_COPY_1 + (uint16_t)(racer_player << 7);
		segment_fraction = mem_read16(r12 + 0x40u);
		road_x = mem_read16(r12 + 0x44u);
		segment = mem_read16(r12 + 0x42u);
		model = mem_read8(r12 + 0x49u);
		hit_box += 2u;
		hit_box = player_car_write_hit_box(segment, model, hit_box, segment_fraction, road_x);
		racers_left--;
	} while (racers_left != 0u);
	hit_box = screen + 0x5B0u;
	segment_fraction = CAR(player_car)->segment_fraction;
	road_x = (uint16_t)CAR(player_car)->road_x;
	segment = CAR(player_car)->segment;
	model = CAR(player_car)->model;
	hit_box = player_car_write_hit_box(segment, model, hit_box, segment_fraction, road_x);
}

/* Writes a car's hit box: its record, its first and last segment (4 on), and its box along and across
   from the model's size (car_hit_box_sizes). */
uint32_t player_car_write_hit_box(uint16_t segment, uint32_t model, uint32_t hit_box, uint16_t segment_fraction, uint16_t road_x)
{
	uint32_t box_size = 0;
	uint16_t along_start = 0, along_end = 0, across_start = 0;

	mem_write32(hit_box, r12);
	hit_box += 4u;
	mem_write16(hit_box, segment);
	hit_box += 2u;
	mem_write16(hit_box, segment + 4u);
	box_size = car_hit_box_sizes_ADDRESS + ((model & ~0xFFu) | (uint8_t)(model * 2u)) * 4u;
	along_start = mem_read16(box_size);
	box_size += 2u;
	hit_box += 2u;
	mem_write16(hit_box, segment_fraction + along_start);
	along_end = mem_read16(box_size);
	box_size += 2u;
	hit_box += 2u;
	mem_write16(hit_box, segment_fraction + along_end);
	across_start = mem_read16(box_size);
	hit_box += 2u;
	mem_write16(hit_box, road_x + across_start);
	hit_box += 2u;
	mem_write16(hit_box, road_x + mem_read16(box_size + 2u));
	hit_box += 2u;
	return hit_box;
}

/* road_row_hit_check with kind 3 (R27): a car that hits a fork sign is slowed for 15 frames, as off the
   road (car slowed_frames), instead of crashing. */
void fork_sign_hit_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	r27 = 3u;
	road_row_hit_check(p_along_distance, p_next_hit_box);
}

/* road_row_hit_check with kind 0 (R27): hitting an item crashes the car. Used by scenery,
   animated_scenery and flashing_scenery (and 012FBC). */
void road_row_crash_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	r27 = 0u;
	road_row_hit_check(p_along_distance, p_next_hit_box);
}

/* Whether the screen's car hits an item of the row, unless the car's object has flags bit 3 set: the
   car's box (the entry SCREEN(screen)->own_hit_box points at: its segments at +6 and +8, along the road
   +A..+C in 128ths of a segment from its first, across +E..+10) against each item's hit_box (along from
   (item - the car's first segment) * 128 + position_fraction, across from the road's width * side +
   x_offset), for the items from first_position to end_position within the car's segments. On the first
   that overlaps: the car's object gets flags bit 2 and byte 6E = R27, the kind of hit (0 a crash, 3
   slowed), and the car's hit_along and hit_across half the distances from the car's box to the item's
   (along, across). */
void road_row_hit_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	uint32_t along_distance = *p_along_distance;
	uint32_t next_hit_box = *p_next_hit_box;
	uint16_t across_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, item_across = 0, item_along = 0, spacing = 0;
	uint32_t item_hit_box = 0, item_position = 0, segment_offset = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
		next_hit_box = SCREEN(screen)->own_hit_box + 6u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_last_segment = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_along_start = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_along_end = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_across_start = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_across_end = mem_read16(next_hit_box);
		next_hit_box += 2u;
		if ((int16_t)((r12 - OBJECT(current_object)->road_row.end_position) ^ (car_last_segment - item_position)) < 0)
		{
			spacing = (uint8_t)OBJECT(current_object)->road_row.spacing;
			if (item_position < (uint16_t)r12)
			{
				do
				{
					item_position = (uint16_t)(item_position + spacing);
				} while (item_position < (uint16_t)r12);
			}
			while (item_position < car_last_segment)
			{
				segment_offset = item_position;
				item_along = (uint16_t)(item_position - r12);
				item_position = (uint16_t)(item_position + spacing);
				segment_offset = (uint16_t)(segment_offset << 4);
				segment_offset = segment_offset & 0x7FFu;
				item_hit_box = OBJECT(current_object)->road_row.hit_box;
				item_across = (uint16_t)((uint32_t)OBJECT(current_object)->side * mem_read16(screen + 0x2012u + segment_offset * 2u) + OBJECT(current_object)->x_offset);
				item_along = (uint16_t)(((uint32_t)item_along << 7) + OBJECT(current_object)->position_fraction);
				along_distance = item_along;
				item_along += ITEM_HIT_BOX(item_hit_box)->along_start;
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance + ITEM_HIT_BOX(item_hit_box - 0x2u)->along_end);
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance - car_along_start);
				across_distance = (uint16_t)(item_across + ITEM_HIT_BOX(item_hit_box - 0x4u)->across_end - car_across_start);
				if ((int16_t)(((item_across + ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start - car_across_end) ^ across_distance) & ((item_along - car_along_end) ^ along_distance)) < 0)
				{
					object = CAR(player_car)->object;
					mem_write16(object, mem_read16(object) | 4u);
					OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
					along_distance = (uint16_t)((int16_t)along_distance >> 1);
					CAR(player_car)->hit_along = along_distance;
					CAR(player_car)->hit_across = (int16_t)across_distance >> 1;
					break;
				}
			}
		}
	}
	*p_along_distance = along_distance;
	*p_next_hit_box = next_hit_box;
}

/* road_row_hit_check for the rows drawn in mirrored pairs, with kind 0 (a crash): each item's hit box is
   tested as it is and mirrored across the road's centre. Used by scenery_pair and animated_scenery_pair. */
void road_row_pair_crash_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	uint32_t along_distance = *p_along_distance;
	uint32_t next_hit_box = *p_next_hit_box;
	uint16_t across_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, item_across = 0, item_along = 0, spacing = 0;
	uint32_t item_hit_box = 0, item_position = 0, segment_offset = 0;
	uint16_t overlap_signs = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
		next_hit_box = SCREEN(screen)->own_hit_box + 6u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_last_segment = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_along_start = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_along_end = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_across_start = mem_read16(next_hit_box);
		next_hit_box += 2u;
		car_across_end = mem_read16(next_hit_box);
		next_hit_box += 2u;
		if ((int16_t)((r12 - OBJECT(current_object)->road_row.end_position) ^ (car_last_segment - item_position)) < 0)
		{
			spacing = (uint8_t)OBJECT(current_object)->road_row.spacing;
			if (item_position < (uint16_t)r12)
			{
				do
				{
					item_position = (uint16_t)(item_position + spacing);
				} while (item_position < (uint16_t)r12);
			}
			while (item_position < car_last_segment)
			{
				segment_offset = item_position;
				item_along = (uint16_t)(item_position - r12);
				item_position = (uint16_t)(item_position + spacing);
				segment_offset = (uint16_t)(segment_offset << 4);
				segment_offset = segment_offset & 0x7FFu;
				item_hit_box = OBJECT(current_object)->road_row.hit_box;
				item_across = (uint16_t)((uint32_t)OBJECT(current_object)->side * mem_read16(screen + 0x2012u + segment_offset * 2u) + OBJECT(current_object)->x_offset);
				item_along = (uint16_t)(((uint32_t)item_along << 7) + OBJECT(current_object)->position_fraction);
				along_distance = item_along;
				across_distance = (uint16_t)item_across;
				item_along += ITEM_HIT_BOX(item_hit_box)->along_start;
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance + ITEM_HIT_BOX(item_hit_box - 0x2u)->along_end);
				item_hit_box += 2u;
				item_across += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start;
				across_distance += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_end;
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - across_distance);
				along_distance = (uint16_t)(along_distance - car_along_start);
				across_distance -= car_across_start;
				item_along = (uint16_t)((item_along - car_along_end) ^ along_distance);
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - car_across_end);
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (0u - item_across - car_across_start));
				r28 = (r28 & ~0xFFFFu) | (r28 & item_along);
				overlap_signs = r28 | (((item_across - car_across_end) ^ across_distance) & item_along);
				r28 = (r28 & ~0xFFFFu) | overlap_signs;
				if ((int16_t)overlap_signs < 0)
				{
					object = CAR(player_car)->object;
					mem_write16(object, mem_read16(object) | 4u);
					OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
					along_distance = (uint16_t)((int16_t)along_distance >> 1);
					CAR(player_car)->hit_along = along_distance;
					CAR(player_car)->hit_across = (int16_t)across_distance >> 1;
					break;
				}
			}
		}
	}
	*p_along_distance = along_distance;
	*p_next_hit_box = next_hit_box;
}

/* Unused (called only by unused_0129D0, an unused handler that goes on into road_row_pair_draw's code):
   road_row_pair_crash_check with kind 0 (a crash), but the mirrored item's hit_box is only moved across
   the road's centre, not flipped: its offsets are added to the mirrored place as they are. For a box
   even about its middle the two are the same. */
void road_row_pair_crash_check_unflipped(void)
{
	uint16_t across_distance = 0, along_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, item_across = 0, item_along = 0, mirrored_across = 0, spacing = 0;
	uint32_t car_hit_box = 0, item_hit_box = 0, item_position = 0, segment_offset = 0;
	uint16_t overlap_signs = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
		car_hit_box = SCREEN(screen)->own_hit_box + 6u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(car_hit_box);
		car_hit_box += 2u;
		car_last_segment = mem_read16(car_hit_box);
		car_hit_box += 2u;
		car_along_start = mem_read16(car_hit_box);
		car_hit_box += 2u;
		car_along_end = mem_read16(car_hit_box);
		car_hit_box += 2u;
		car_across_start = mem_read16(car_hit_box);
		car_across_end = mem_read16(car_hit_box + 2u);
		if ((int16_t)((r12 - OBJECT(current_object)->road_row.end_position) ^ (car_last_segment - item_position)) >= 0)
			return;
		spacing = (uint8_t)OBJECT(current_object)->road_row.spacing;
		if (item_position < (uint16_t)r12)
		{
			do
			{
				item_position = (uint16_t)(item_position + spacing);
			} while (item_position < (uint16_t)r12);
		}
		do
		{
			if (car_last_segment <= item_position)
				return;
			segment_offset = item_position;
			item_along = (uint16_t)(item_position - r12);
			item_position = (uint16_t)(item_position + spacing);
			segment_offset = (uint16_t)(segment_offset << 4);
			item_hit_box = OBJECT(current_object)->road_row.hit_box;
			item_across = (uint16_t)((uint32_t)OBJECT(current_object)->side * mem_read16(screen + 0x2012u + (segment_offset & 0x7FFu) * 2u) + OBJECT(current_object)->x_offset);
			item_along = (uint16_t)(((uint32_t)item_along << 7) + OBJECT(current_object)->position_fraction);
			along_distance = (uint16_t)item_along;
			across_distance = (uint16_t)item_across;
			mirrored_across = (uint16_t)(0u - item_across);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - across_distance);
			item_along += ITEM_HIT_BOX(item_hit_box)->along_start;
			item_hit_box += 2u;
			along_distance += ITEM_HIT_BOX(item_hit_box - 0x2u)->along_end;
			item_hit_box += 2u;
			item_across += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start;
			mirrored_across += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start;
			item_hit_box += 2u;
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + ITEM_HIT_BOX(item_hit_box - 0x6u)->across_end);
			along_distance -= car_along_start;
			across_distance = (uint16_t)(across_distance + ITEM_HIT_BOX(item_hit_box - 0x6u)->across_end - car_across_start);
			item_along = (uint16_t)((item_along - car_along_end) ^ along_distance);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - car_across_start);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (mirrored_across - car_across_end));
			r28 = (r28 & ~0xFFFFu) | (r28 & item_along);
			overlap_signs = r28 | (((item_across - car_across_end) ^ across_distance) & item_along);
			r28 = (r28 & ~0xFFFFu) | overlap_signs;
		} while ((int16_t)overlap_signs >= 0);
		object = CAR(player_car)->object;
		mem_write16(object, mem_read16(object) | 4u);
		OBJECT(CAR(player_car)->object)->player_car.hit_kind = r27;
		CAR(player_car)->hit_along = (int16_t)along_distance >> 1;
		CAR(player_car)->hit_across = (int16_t)across_distance >> 1;
	}
}

/* Whether the car hits another car of the race (the boxes of player_car_build_hit_boxes; not while
   either is immune, flags bit 3): a hit of kind 2 with that car's player in hit_other. */
void player_car_check_car_hits(void)
{
	uint16_t car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, other_across = 0, other_along_end = 0, other_along_start = 0, other_player = 0, own_player = 0, segment_gap = 0;
	uint32_t boxes_left = 0, car_hit_box = 0, other_record = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
		return;
	r14 = screen + 0x510u;
	car_hit_box = SCREEN(screen)->own_hit_box;
	own_player = mem_read16(car_hit_box);
	car_hit_box += 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(car_hit_box);
	car_hit_box += 2u;
	car_last_segment = mem_read16(car_hit_box);
	car_hit_box += 2u;
	car_along_start = (uint16_t)(mem_read16(car_hit_box) + 1u);
	car_hit_box += 2u;
	car_along_end = mem_read16(car_hit_box);
	car_hit_box += 2u;
	car_across_start = mem_read16(car_hit_box);
	car_across_end = mem_read16(car_hit_box + 2u);
	boxes_left = SCREEN(r14 - 0x510u)->hit_box_count;
	r14 = r14 + 4u;
	if ((uint8_t)boxes_left < 2u)
		return;
	for (;;)
	{
		other_player = mem_read16(r14);
		r14 = r14 + 2u;
		if (own_player == other_player)
		{
			r14 = r14 + 0x10u;
			boxes_left--;
			if (boxes_left == 0u)
				return;
			continue;
		}
		other_record = mem_read32(r14);
		r14 = r14 + 4u;
		if ((mem_read32(other_record + 0x58u) & 8u) != 0u)
		{
			r14 = r14 + 0xCu;
			boxes_left--;
			if (boxes_left == 0u)
				return;
			continue;
		}
		other_along_start = mem_read16(r14);
		r14 = r14 + 2u;
		segment_gap = (uint16_t)(r12 - mem_read16(r14));
		r14 = r14 + 2u;
		if ((int16_t)((car_last_segment - other_along_start) ^ segment_gap) >= 0)
		{
			r14 = r14 + 8u;
			boxes_left--;
			if (boxes_left == 0u)
				break;
			continue;
		}
		other_along_start = (uint16_t)((other_along_start - r12) << 7);
		other_along_end = (uint16_t)other_along_start;
		other_along_start += mem_read16(r14);
		r14 = r14 + 2u;
		other_along_end += mem_read16(r14);
		r14 = r14 + 2u;
		other_across = mem_read16(r14);
		r14 = r14 + 2u;
		other_across = (uint16_t)((other_across - car_across_end) ^ (mem_read16(r14) - car_across_start));
		r14 = r14 + 2u;
		if ((int16_t)(other_across & ((other_along_start - car_along_end) ^ (other_along_end - car_along_start))) < 0)
		{
			object = CAR(player_car)->object;
			mem_write16(object, mem_read16(object) | 4u);
			OBJECT(CAR(player_car)->object)->player_car.hit_kind = 2u;
			OBJECT(CAR(player_car)->object)->player_car.hit_other = other_player;
		}
		boxes_left--;
		if (boxes_left == 0u)
			return;
	}
}

/* Whether the screen's car hits this traffic object (negative set), unless the car's object has flags
   bit 3 set: the car's box (the entry SCREEN(screen)->own_hit_box points at) against the object's
   hit_box (along the road from (position - the car's first segment) * 128 + position_fraction, across
   from the road's width * side + x_offset). On a hit the car's object gets flags bit 2, byte 6E = 1 (the
   kind of hit: a single object) and 68 = this object, whose speed and weight the crash (007DBD) reads. */
uint32_t traffic_hit_check(void)
{
	uint32_t negative = 0;
	uint16_t object_across = 0;
	uint32_t car_hit_box = 0, item_hit_box = 0, object = 0, object_along = 0, segment_offset = 0;
	uint16_t overlap_signs = 0;

	if ((OBJECT(CAR(player_car)->object)->flags & 8u) != 0u)
	{
		negative = 0u;
		return negative;
	}
	object_along = (uint16_t)OBJECT(current_object)->position;
	car_hit_box = SCREEN(screen)->own_hit_box + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(car_hit_box);
	car_hit_box += 4u;
	segment_offset = (uint16_t)(object_along << 4);
	item_hit_box = OBJECT(current_object)->traffic.hit_box;
	object_across = (uint16_t)((uint32_t)OBJECT(current_object)->side * mem_read16(screen + 0x2012u + (segment_offset & 0x7FFu) * 2u) + OBJECT(current_object)->x_offset);
	object_along = (uint16_t)(((object_along - r12) << 7) + OBJECT(current_object)->position_fraction);
	overlap_signs = ((object_across + ITEM_HIT_BOX(item_hit_box)->across_start - mem_read16(car_hit_box + 6u)) ^ (object_across + ITEM_HIT_BOX(item_hit_box)->across_end - mem_read16(car_hit_box + 4u))) & ((object_along + ITEM_HIT_BOX(item_hit_box)->along_start - mem_read16(car_hit_box + 2u)) ^ (object_along + ITEM_HIT_BOX(item_hit_box)->along_end - mem_read16(car_hit_box)));
	negative = overlap_signs >> 15;
	if ((int16_t)overlap_signs < 0)
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
   the stream of the stage being built (screen built_stage); 1 reads the stream's records while their
   position is not beyond the segments the road generator has built of the stage (car stage_segments):
   command 0 road_object_stream_start_object, 1 road_object_stream_stage_end, 2 road_object_stream_end, 3
   road_object_stream_fork; 2 (the stage's stream is over) waits until the generator builds the stage the
   route goes to (screen built_stage = stage), then stage_start = car built_segments (the segment the new
   stage starts at) and its stream; 3 (a fork) waits until the route's next stage is known (screen
   route_stage no longer stage), then goes on with the stream of the branch the car takes (car branch)
   from the fork's table, the same stage_start. */
void road_object_stream_read(void)
{
	uint16_t position = 0;
	uint32_t built_stage = 0, command_routine = 0, entry = 0, left_caller = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->road_object_stream.stream = road_object_streams[SCREEN(screen)->built_stage];
		return;
	case 1:
		break;
	case 2:
		if (SCREEN(screen)->built_stage != OBJECT(current_object)->road_object_stream.stage)
			return;
		OBJECT(current_object)->state--;
		built_stage = SCREEN(screen)->built_stage;
		OBJECT(current_object)->road_object_stream.stage_start = CAR(player_car)->built_segments;
		OBJECT(current_object)->road_object_stream.stream = road_object_streams[built_stage];
		break;
	case 3:
		if (SCREEN(screen)->route_stage == OBJECT(current_object)->road_object_stream.stage)
			return;
		OBJECT(current_object)->state -= 2u;
		OBJECT(current_object)->road_object_stream.stream = ROM32(OBJECT(current_object)->road_object_stream.stream + CAR(player_car)->branch * 4u);
		break;
	default:
		invalid_jump(road_object_stream_state_jumps[OBJECT(current_object)->state]);
	}
	entry = OBJECT(current_object)->road_object_stream.stream;
	for (;;)
	{
		position = (uint16_t)ROAD_OBJECT_STREAM_ENTRY(entry)->position;
		entry += 2u;
		if ((int16_t)CAR(player_car)->stage_segments < (int16_t)position)
			break;
		command_routine = road_object_stream_command_jumps[ROAD_OBJECT_STREAM_ENTRY(entry - 0x2u)->command];
		left_caller = 0u;
		entry += 2u;
		switch (command_routine)
		{
		case 0x012758u:
			entry = road_object_stream_start_object(position, entry);
			break;
		case 0x012777u:
			left_caller = road_object_stream_stage_end();
			break;
		case 0x01279Cu:
			left_caller = road_object_stream_end();
			break;
		case 0x012786u:
			left_caller = road_object_stream_fork(entry);
			break;
		default:
			invalid_jump(command_routine);
		}
		if (left_caller != 0u)
			return;
	}
}

/* Command 0 of a stream: starts an object (start_object) at the record's position plus stage_start
   (field 1C), with the record's handler and its parameter in field 18 (most often the address of the
   object's record in the ROM), and moves the stream past the record. */
uint32_t road_object_stream_start_object(uint16_t position, uint32_t entry)
{
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->position = position + OBJECT(current_object)->road_object_stream.stage_start;
	OBJECT(object)->handler = ROAD_OBJECT_STREAM_ENTRY(entry - 0x4u)->handler;
	entry += 4u;
	OBJECT(object)->unk_18 = ROAD_OBJECT_STREAM_ENTRY(entry - 0x8u)->parameter;
	entry += 4u;
	OBJECT(current_object)->road_object_stream.stream = entry;
	return entry;
}

/* Command 1: the stage's stream is over; notes the stage the route goes to next (screen route_stage) in
   stage and waits for it (state 2). */
uint32_t road_object_stream_stage_end(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->state++;
	OBJECT(current_object)->road_object_stream.stage = SCREEN(screen)->route_stage;
	left_caller = 1u;
	return left_caller;
}

/* Command 3, a fork: keeps the address of the table of the branches' streams in stream, the current
   stage (screen built_stage) in stage, and waits for the branch (state 3). */
uint32_t road_object_stream_fork(uint32_t branch_streams)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->state += 2u;
	OBJECT(current_object)->road_object_stream.stream = branch_streams;
	OBJECT(current_object)->road_object_stream.stage = SCREEN(screen)->built_stage;
	left_caller = 1u;
	return left_caller;
}

/* Command 2: the last stage's stream is over (the goals): ends the reader. */
uint32_t road_object_stream_end(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->flags_high = 0u;
	left_caller = 1u;
	return left_caller;
}

/* Cuts the sprites beyond the road's crests: each frame, for the crests the road's projection found
   (screen crest_count of them; their depth layers at 1F10 of the screen and their screen heights at
   1F30), each whose layer is lower than all those before it gets a sprite clip command in its layer
   (draw_mode 6: the 16-byte entry at clip_command, command 7000: the drawing area from the top of the
   screen down to clip_bottom = the crest's height + screen sprite_origin_y - 1, across
   clip_left..clip_right), so the sprites the list draws after it (meant: what lies beyond the crest) are
   cut at the crest's height. */
void crest_clip_update(void)
{
	uint16_t depth_layer = 0;
	uint32_t clip_bottom = 0, crest = 0;

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
		invalid_jump(crest_clip_state_jumps[OBJECT(current_object)->state]);
	}
	if (SCREEN(screen)->crest_count == 0u)
		return;
	OBJECT(current_object)->crest_clip.crest = 0u;
	OBJECT(current_object)->depth_layer = 0xFFFFu;
	do
	{
		crest = OBJECT(current_object)->crest_clip.crest;
		depth_layer = mem_read16(screen + 0x1F10u + crest * 2u);
		if (depth_layer < OBJECT(current_object)->depth_layer)
		{
			OBJECT(current_object)->depth_layer = depth_layer;
			clip_bottom = (uint16_t)(mem_read16(screen + 0x1F30u + crest * 2u) + SCREEN(screen)->sprite_origin_y - 1u);
			OBJECT(current_object)->crest_clip.clip_bottom = clip_bottom;
			draw_object(clip_bottom);
		}
		OBJECT(current_object)->crest_clip.crest++;
	} while (OBJECT(current_object)->crest_clip.crest <= SCREEN(screen)->crest_count);
}

/* Static scenery along the road (trees, buildings, crowds, banners, signs): a row of identical sprites
   from its record. By state: 0 road_row_start; 1 road_row_crash_check, then road_row_draw; 2 (a record
   without a hit box) road_row_draw only. Ends when road_row_draw finds the whole row behind the view. */
void scenery_update(void)
{
	uint32_t along_distance = 0, next_hit_box = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 1:
		road_row_crash_check(&along_distance, &next_hit_box);
		road_row_draw(along_distance, next_hit_box);
		return;
	case 2:
		road_row_draw(0u, 0u);
		return;
	default:
		invalid_jump(scenery_state_jumps[OBJECT(current_object)->state]);
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
	uint16_t position = 0, row_length = 0;
	uint32_t animation = 0, road_row_record = 0;

	OBJECT(current_object)->state++;
	road_row_record = OBJECT(current_object)->road_row.record;
	animation = ROAD_ROW_RECORD(road_row_record)->animation;
	road_row_record += 2u;
	OBJECT(current_object)->palette = ROAD_ROW_RECORD(road_row_record - 0x2u)->palette;
	road_row_record += 2u;
	OBJECT(current_object)->flags |= ROAD_ROW_RECORD(road_row_record - 0x4u)->flags;
	road_row_record++;
	OBJECT(current_object)->road_row.size_table = ROAD_ROW_RECORD(road_row_record - 0x5u)->size_table;
	road_row_record++;
	OBJECT(current_object)->scale_shift = ROAD_ROW_RECORD(road_row_record - 0x6u)->scale_shift;
	road_row_record++;
	OBJECT(current_object)->side = ROAD_ROW_RECORD(road_row_record - 0x7u)->side;
	road_row_record++;
	OBJECT(current_object)->x_offset = ROAD_ROW_RECORD(road_row_record - 0x8u)->x_offset;
	road_row_record += 2u;
	OBJECT(current_object)->y_offset = ROAD_ROW_RECORD(road_row_record - 0xAu)->y_offset;
	road_row_record += 2u;
	OBJECT(current_object)->road_row.spacing = ROAD_ROW_RECORD(road_row_record - 0xCu)->spacing;
	road_row_record += 2u;
	OBJECT(current_object)->road_row.count = ROAD_ROW_RECORD(road_row_record - 0xEu)->count;
	road_row_record += 2u;
	if (ROAD_ROW_RECORD(road_row_record - 0x10u)->hit_box_along == 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->road_row.hit_box = road_row_record;
	animation_start(animation);
	position = (uint16_t)OBJECT(current_object)->position;
	row_length = (uint16_t)(OBJECT(current_object)->road_row.spacing * (OBJECT(current_object)->road_row.count - 1u) + 1u);
	OBJECT(current_object)->road_row.first_position = position;
	OBJECT(current_object)->road_row.end_position = position + row_length;
}

/* Scenery whose palette cycles a step every frame, for flashing lights (the Christmas trees, the arches
   over the underwater tunnel, a fairground ride). By state: 0 reads its record as road_row_start does,
   except that the second and third halfwords are the address of its list of palettes (palette_cycle)
   instead of a palette; 1 road_row_crash_check; 1 and 2 the next palette from the list into palette (a
   negative entry goes back that many bytes first), then road_row_draw. */
void flashing_scenery_update(void)
{
	uint16_t position = 0, row_length = 0;
	uint32_t along_distance = 0, animation = 0, element = 0, next_hit_box = 0, palettes = 0, record = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		record = OBJECT(current_object)->flashing_scenery.palette_cycle;
		animation = FLASHING_SCENERY_RECORD(record)->animation;
		record += 2u;
		OBJECT(current_object)->flashing_scenery.palette_cycle = FLASHING_SCENERY_RECORD(record - 0x2u)->palettes;
		record += 4u;
		OBJECT(current_object)->flags |= FLASHING_SCENERY_RECORD(record - 0x6u)->flags;
		record++;
		OBJECT(current_object)->flashing_scenery.size_table = FLASHING_SCENERY_RECORD(record - 0x7u)->size_table;
		record++;
		OBJECT(current_object)->scale_shift = FLASHING_SCENERY_RECORD(record - 0x8u)->scale_shift;
		record++;
		OBJECT(current_object)->side = FLASHING_SCENERY_RECORD(record - 0x9u)->side;
		record++;
		OBJECT(current_object)->x_offset = FLASHING_SCENERY_RECORD(record - 0xAu)->x_offset;
		record += 2u;
		OBJECT(current_object)->y_offset = FLASHING_SCENERY_RECORD(record - 0xCu)->y_offset;
		record += 2u;
		OBJECT(current_object)->flashing_scenery.spacing = FLASHING_SCENERY_RECORD(record - 0xEu)->spacing;
		record += 2u;
		OBJECT(current_object)->flashing_scenery.count = FLASHING_SCENERY_RECORD(record - 0x10u)->count;
		record += 2u;
		if (FLASHING_SCENERY_RECORD(record - 0x12u)->hit_box_along == 0u)
			OBJECT(current_object)->state++;
		OBJECT(current_object)->flashing_scenery.hit_box = record;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		row_length = (uint16_t)(OBJECT(current_object)->flashing_scenery.spacing * (OBJECT(current_object)->flashing_scenery.count - 1u) + 1u);
		OBJECT(current_object)->flashing_scenery.first_position = position;
		OBJECT(current_object)->flashing_scenery.end_position = position + row_length;
		return;
	case 1:
		road_row_crash_check(&along_distance, &next_hit_box);
		palettes = OBJECT(current_object)->flashing_scenery.palette_cycle;
		element = FLASHING_SCENERY_PALETTES(palettes)[0];
		palettes += 2u;
		if ((int16_t)element < 0)
		{
			palettes += element;
			element = (uint16_t)FLASHING_SCENERY_PALETTES(palettes)[0];
			palettes += 2u;
		}
		OBJECT(current_object)->palette = element;
		OBJECT(current_object)->flashing_scenery.palette_cycle = palettes;
		road_row_draw(along_distance, next_hit_box);
		return;
	case 2:
		palettes = OBJECT(current_object)->flashing_scenery.palette_cycle;
		element = FLASHING_SCENERY_PALETTES(palettes)[0];
		palettes += 2u;
		if ((int16_t)element < 0)
		{
			palettes += element;
			element = (uint16_t)FLASHING_SCENERY_PALETTES(palettes)[0];
			palettes += 2u;
		}
		OBJECT(current_object)->palette = element;
		OBJECT(current_object)->flashing_scenery.palette_cycle = palettes;
		road_row_draw(along_distance, next_hit_box);
		return;
	default:
		invalid_jump(flashing_scenery_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Static scenery drawn twice, at each side of the road, the second mirrored (rows of trees, houses,
   spectators). By state: 0 road_row_start; 1 road_row_pair_crash_check, then road_row_pair_draw; 2 (no
   hit box) road_row_pair_draw only. */
void scenery_pair_update(void)
{
	uint32_t along_distance = 0, next_hit_box = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 1:
		road_row_pair_crash_check(&along_distance, &next_hit_box);
		break;
	case 2:
		break;
	default:
		invalid_jump(scenery_pair_state_jumps[OBJECT(current_object)->state]);
	}
	road_row_pair_draw(along_distance, next_hit_box);
}

/* Animated scenery high over the road, drawn from the middle of the screen instead of the road's centre
   line (screen_centered), so it does not swing with the bends: the schools of fish over the underwater
   tunnel, the cloud bands of stage 25. By state: 0 screen_centered = FF, then road_row_start (its
   records have no hit box); 1 and 2 animation_advance and road_row_draw. */
void overhead_scenery_update(void)
{
	uint32_t animation_order = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->overhead_scenery.screen_centered = 0xFFu;
		road_row_start();
		return;
	case 1:
	case 2:
		animation_order = animation_advance(0u);
		road_row_draw(animation_order, 0u);
		return;
	default:
		invalid_jump(overhead_scenery_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The northern lights over stage 26 (stream 26 at 1040: two of them, with their own lists): drawn at a
   screen position (draw_mode 0) that slides in from x -160 to 160, 8 a frame (y 40, or 30 for car model
   6, car model), then stays; it steps through its list of animations (aurora_next_animation, each shown
   animation_frames frames, aurora_count_down) and is drawn only every other frame (when countdown is
   odd), so it looks faint. It never ends. */
void aurora_update(void)
{
	uint16_t countdown = 0, routine = 0;
	uint32_t odd_countdown = 0, record = 0;
	uint8_t odd = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = (uint16_t)aurora_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		record = OBJECT(current_object)->aurora.animation_list;
		OBJECT(current_object)->aurora.animation_list = AURORA_RECORD(record)->animations;
		record += 4u;
		OBJECT(current_object)->palette = AURORA_RECORD(record - 0x4u)->palette;
		record += 2u;
		OBJECT(current_object)->flags |= AURORA_RECORD(record - 0x6u)->flags;
		OBJECT(current_object)->aurora.animation_frames = AURORA_RECORD(record - 0x6u)->animation_frames;
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
		countdown = (uint16_t)((routine & ~0xFFu) | OBJECT(current_object)->aurora.countdown);
		odd = countdown & 1u;
		odd_countdown = (countdown & ~0xFFu) | odd;
		if (odd == 0u)
			return;
		animation_advance(0u);
		odd_countdown = draw_object(odd_countdown);
		return;
	case 2:
		break;
	default:
		invalid_jump(aurora_state_jumps[state]);
	}
	aurora_count_down();
	countdown = (uint16_t)((routine & ~0xFFu) | OBJECT(current_object)->aurora.countdown);
	odd = countdown & 1u;
	odd_countdown = (countdown & ~0xFFu) | odd;
	if (odd == 0u)
		return;
	animation_advance(0u);
	odd_countdown = draw_object(odd_countdown);
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
	uint32_t animation = 0, animations = 0, element = 0;

	animations = OBJECT(current_object)->aurora.animation_list;
	element = AURORA_ANIMATIONS(animations)[0];
	animations += 2u;
	if ((int16_t)element < 0)
	{
		animations += element;
		element = AURORA_ANIMATIONS(animations)[0];
		animations += 2u;
	}
	animation = element;
	OBJECT(current_object)->aurora.animation_list = animations;
	OBJECT(current_object)->aurora.countdown = OBJECT(current_object)->aurora.animation_frames;
	animation_start(animation);
}

/* The signs on the road where it forks at the end of a stage: a line down the middle before the fork and
   a line across each branch after it. By state: 0 road_row_start; 1 fork_sign_hit_check, then
   road_row_draw; 2 road_row_draw only. A car that hits one is slowed, not crashed. */
void fork_sign_update(void)
{
	uint32_t along_distance = 0, next_hit_box = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start();
		return;
	case 2:
		road_row_draw(0u, 0u);
		return;
	case 1:
		fork_sign_hit_check(&along_distance, &next_hit_box);
		road_row_draw(along_distance, next_hit_box);
		return;
	default:
		invalid_jump(fork_sign_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Scenery that animates in place (the Ferris wheel, windmills; also the scenery of the goal lines). By
   state: 0 road_row_start_copy; 1 road_row_crash_check; 1 and 2 animation_advance and road_row_draw. */
void animated_scenery_update(void)
{
	uint32_t along_distance = 0, next_hit_box = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start_copy();
		return;
	case 1:
		road_row_crash_check(&along_distance, &next_hit_box);
		break;
	case 2:
		break;
	default:
		invalid_jump(animated_scenery_state_jumps[OBJECT(current_object)->state]);
	}
	along_distance = animation_advance(along_distance);
	road_row_draw(along_distance, next_hit_box);
}

/* The same code as road_row_start, at another address: the first step of animated_scenery and
   animated_scenery_pair. */
void road_row_start_copy(void)
{
	uint16_t position = 0, row_length = 0;
	uint32_t animation = 0, road_row_record = 0;

	OBJECT(current_object)->state++;
	road_row_record = OBJECT(current_object)->road_row.record;
	animation = ROAD_ROW_RECORD(road_row_record)->animation;
	road_row_record += 2u;
	OBJECT(current_object)->palette = ROAD_ROW_RECORD(road_row_record - 0x2u)->palette;
	road_row_record += 2u;
	OBJECT(current_object)->flags |= ROAD_ROW_RECORD(road_row_record - 0x4u)->flags;
	road_row_record++;
	OBJECT(current_object)->road_row.size_table = ROAD_ROW_RECORD(road_row_record - 0x5u)->size_table;
	road_row_record++;
	OBJECT(current_object)->scale_shift = ROAD_ROW_RECORD(road_row_record - 0x6u)->scale_shift;
	road_row_record++;
	OBJECT(current_object)->side = ROAD_ROW_RECORD(road_row_record - 0x7u)->side;
	road_row_record++;
	OBJECT(current_object)->x_offset = ROAD_ROW_RECORD(road_row_record - 0x8u)->x_offset;
	road_row_record += 2u;
	OBJECT(current_object)->y_offset = ROAD_ROW_RECORD(road_row_record - 0xAu)->y_offset;
	road_row_record += 2u;
	OBJECT(current_object)->road_row.spacing = ROAD_ROW_RECORD(road_row_record - 0xCu)->spacing;
	road_row_record += 2u;
	OBJECT(current_object)->road_row.count = ROAD_ROW_RECORD(road_row_record - 0xEu)->count;
	road_row_record += 2u;
	if (ROAD_ROW_RECORD(road_row_record - 0x10u)->hit_box_along == 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->road_row.hit_box = road_row_record;
	animation_start(animation);
	position = (uint16_t)OBJECT(current_object)->position;
	row_length = (uint16_t)(OBJECT(current_object)->road_row.spacing * (OBJECT(current_object)->road_row.count - 1u) + 1u);
	OBJECT(current_object)->road_row.first_position = position;
	OBJECT(current_object)->road_row.end_position = position + row_length;
}

/* Animated scenery drawn twice, at each side of the road, the second mirrored (the people lining the
   start and the goal, the scenery around the forks). By state: 0 road_row_start_copy; 1
   road_row_pair_crash_check; 1 and 2 animation_advance and road_row_pair_draw. */
void animated_scenery_pair_update(void)
{
	uint32_t along_distance = 0, next_hit_box = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_row_start_copy();
		return;
	case 1:
		road_row_pair_crash_check(&along_distance, &next_hit_box);
		break;
	case 2:
		break;
	default:
		invalid_jump(animated_scenery_pair_state_jumps[OBJECT(current_object)->state]);
	}
	along_distance = animation_advance(along_distance);
	road_row_pair_draw(along_distance, next_hit_box);
}

/* What walks across the road as the car comes near (the camels of the desert stages). By state: 0 reads
   the record (animation, crossing_animation, palette, flags, side, x_offset, trigger_distance, x_speed,
   draw_mode, scale_shift; y_offset -256 for animation 165), trigger_position = position -
   trigger_distance, appear_position = position - draw_distance; 1 not drawn until the view (race_copy
   window_segment) passes appear_position; 2 drawn standing (draw_object) until the car (car segment)
   reaches trigger_position, then crossing_animation; 3 while the car (car window_segment) has not
   reached it: x_offset += x_speed and position + 1 each frame, and while within draw_distance of the car
   animation_advance and draw_object; it ends when the car reaches it, it gets beyond draw_distance or
   its screen_x leaves -F0..130. */
void road_crosser_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, crossing_animation = 0, road_crosser_record = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = road_crosser_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		road_crosser_record = OBJECT(current_object)->road_crosser.record;
		animation = ROAD_CROSSER_RECORD(road_crosser_record)->animation;
		road_crosser_record += 2u;
		if (animation == 0x165u)
			AS32(OBJECT(current_object)->road_crosser.y_offset) = (int16_t)0xFF00u;
		OBJECT(current_object)->road_crosser.crossing_animation = ROAD_CROSSER_RECORD(road_crosser_record - 0x2u)->crossing_animation;
		road_crosser_record += 2u;
		OBJECT(current_object)->palette = ROAD_CROSSER_RECORD(road_crosser_record - 0x4u)->palette;
		road_crosser_record += 2u;
		OBJECT(current_object)->flags |= ROAD_CROSSER_RECORD(road_crosser_record - 0x6u)->flags;
		road_crosser_record++;
		OBJECT(current_object)->side = ROAD_CROSSER_RECORD(road_crosser_record - 0x7u)->side;
		road_crosser_record++;
		OBJECT(current_object)->x_offset = ROAD_CROSSER_RECORD(road_crosser_record - 0x8u)->x_offset;
		road_crosser_record += 2u;
		OBJECT(current_object)->road_crosser.trigger_distance = ROAD_CROSSER_RECORD(road_crosser_record - 0xAu)->trigger_distance;
		road_crosser_record += 2u;
		OBJECT(current_object)->road_crosser.x_speed = ROAD_CROSSER_RECORD(road_crosser_record - 0xCu)->x_speed;
		road_crosser_record += 2u;
		OBJECT(current_object)->draw_mode = ROAD_CROSSER_RECORD(road_crosser_record - 0xEu)->draw_mode;
		OBJECT(current_object)->scale_shift = ROAD_CROSSER_RECORD(road_crosser_record - 0xEu)->scale_shift;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->road_crosser.trigger_position = position - OBJECT(current_object)->road_crosser.trigger_distance;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->road_crosser.appear_position = position - draw_distance;
		OBJECT(current_object)->road_crosser.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->road_crosser.appear_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->road_crosser.trigger_position)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		crossing_animation = OBJECT(current_object)->road_crosser.crossing_animation;
		animation_start(crossing_animation);
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->position)
		{
			OBJECT(current_object)->x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->road_crosser.trigger_position)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		crossing_animation = OBJECT(current_object)->road_crosser.crossing_animation;
		animation_start(crossing_animation);
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->position)
		{
			OBJECT(current_object)->x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 3:
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->position)
		{
			OBJECT(current_object)->x_offset += OBJECT(current_object)->road_crosser.x_speed;
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->road_crosser.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	default:
		invalid_jump(road_crosser_state_jumps[state]);
	}
}

/* The kangaroos of stage 21 (Australia) that hop across the road ahead of the car. By state: 0 reads the
   record (animation, hop_animation, palette, flags, side, x_offset, trigger_distance, x_speed,
   draw_mode, scale_shift), trigger_position = position - trigger_distance, appear_position = position -
   draw_distance; 1 not drawn until the view (race_copy window_segment) passes appear_position; 2 drawn
   standing until the car (car segment) reaches trigger_position, then hop_animation; 3 while the car
   (car window_segment) has not reached it: x_offset += x_speed each frame and it moves along the road at
   the car's own pace (position_fraction += the car's speed, car speed, >> 9), animation_advance and
   draw_object; it ends when the car reaches it, when it gets beyond draw_distance or when its screen_x
   leaves -F0..130. */
void kangaroo_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, hop_animation = 0, position_fraction = 0, road_crosser_record = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = kangaroo_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		road_crosser_record = OBJECT(current_object)->kangaroo.record;
		animation = ROAD_CROSSER_RECORD(road_crosser_record)->animation;
		road_crosser_record += 2u;
		OBJECT(current_object)->kangaroo.hop_animation = ROAD_CROSSER_RECORD(road_crosser_record - 0x2u)->crossing_animation;
		road_crosser_record += 2u;
		OBJECT(current_object)->palette = ROAD_CROSSER_RECORD(road_crosser_record - 0x4u)->palette;
		road_crosser_record += 2u;
		OBJECT(current_object)->flags |= ROAD_CROSSER_RECORD(road_crosser_record - 0x6u)->flags;
		road_crosser_record++;
		OBJECT(current_object)->side = ROAD_CROSSER_RECORD(road_crosser_record - 0x7u)->side;
		road_crosser_record++;
		OBJECT(current_object)->x_offset = ROAD_CROSSER_RECORD(road_crosser_record - 0x8u)->x_offset;
		road_crosser_record += 2u;
		OBJECT(current_object)->kangaroo.trigger_distance = ROAD_CROSSER_RECORD(road_crosser_record - 0xAu)->trigger_distance;
		road_crosser_record += 2u;
		OBJECT(current_object)->kangaroo.x_speed = ROAD_CROSSER_RECORD(road_crosser_record - 0xCu)->x_speed;
		road_crosser_record += 2u;
		OBJECT(current_object)->draw_mode = ROAD_CROSSER_RECORD(road_crosser_record - 0xEu)->draw_mode;
		OBJECT(current_object)->scale_shift = ROAD_CROSSER_RECORD(road_crosser_record - 0xEu)->scale_shift;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->kangaroo.trigger_position = position - OBJECT(current_object)->kangaroo.trigger_distance;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->kangaroo.appear_position = position - draw_distance;
		OBJECT(current_object)->kangaroo.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->kangaroo.appear_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->kangaroo.trigger_position)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		hop_animation = OBJECT(current_object)->kangaroo.hop_animation;
		animation_start(hop_animation);
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->kangaroo.x_speed;
		position_fraction = (uint16_t)(OBJECT(current_object)->position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)position_fraction >= 0x80)
		{
			position_fraction = (uint16_t)(position_fraction - 0x80u);
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->position_fraction = position_fraction;
				animation_advance(0u);
				draw_object(position_fraction);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->position_fraction = position_fraction;
		animation_advance(0u);
		draw_object(position_fraction);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		return;
	case 2:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->kangaroo.trigger_position)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		hop_animation = OBJECT(current_object)->kangaroo.hop_animation;
		animation_start(hop_animation);
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->kangaroo.x_speed;
		position_fraction = (uint16_t)(OBJECT(current_object)->position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)position_fraction >= 0x80)
		{
			position_fraction = (uint16_t)(position_fraction - 0x80u);
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->position_fraction = position_fraction;
				animation_advance(0u);
				draw_object(position_fraction);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->position_fraction = position_fraction;
		animation_advance(0u);
		draw_object(position_fraction);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		return;
	case 3:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->kangaroo.x_speed;
		position_fraction = (uint16_t)(OBJECT(current_object)->position_fraction + (CAR(player_car)->speed >> 9));
		if ((int16_t)position_fraction >= 0x80)
		{
			position_fraction = (uint16_t)(position_fraction - 0x80u);
			OBJECT(current_object)->position++;
			if ((int16_t)(0u - OBJECT(current_object)->kangaroo.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				OBJECT(current_object)->position_fraction = position_fraction;
				animation_advance(0u);
				draw_object(position_fraction);
				if (OBJECT(current_object)->kangaroo.screen_x >= -0xF0 && OBJECT(current_object)->kangaroo.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->position_fraction = position_fraction;
		animation_advance(0u);
		draw_object(position_fraction);
		if (OBJECT(current_object)->kangaroo.screen_x < -0xF0 || OBJECT(current_object)->kangaroo.screen_x > 0x130)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		return;
	default:
		invalid_jump(kangaroo_state_jumps[state]);
	}
}

/* Scenery that travels across the view once the car comes near (helicopters, the roller coaster train,
   birds, fish over the tunnel, a tanker truck on a bridge). By state: 0 reads the record (animation,
   palette, flags, draw_mode, scale_shift, side, x_offset, y_offset, trigger_distance + 16, x_speed,
   y_speed, road_speed), trigger_position = position - trigger_distance; 1 not drawn until the view
   (race_copy window_segment) passes trigger_position; 1 and 2: while the car (car window_segment) has
   not reached it, each frame x_offset += x_speed, y_offset += y_speed, position += road_speed, and while
   within draw_distance of the car animation_advance and draw_object; it ends when the car reaches it, it
   gets beyond draw_distance or its screen_x leaves -F0..130. */
void moving_scenery_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, moving_scenery_record = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = moving_scenery_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		moving_scenery_record = OBJECT(current_object)->moving_scenery.record;
		animation = MOVING_SCENERY_RECORD(moving_scenery_record)->animation;
		moving_scenery_record += 2u;
		OBJECT(current_object)->palette = MOVING_SCENERY_RECORD(moving_scenery_record - 0x2u)->palette;
		moving_scenery_record += 2u;
		OBJECT(current_object)->flags |= MOVING_SCENERY_RECORD(moving_scenery_record - 0x4u)->flags;
		moving_scenery_record++;
		OBJECT(current_object)->draw_mode = MOVING_SCENERY_RECORD(moving_scenery_record - 0x5u)->draw_mode;
		moving_scenery_record++;
		OBJECT(current_object)->scale_shift = MOVING_SCENERY_RECORD(moving_scenery_record - 0x6u)->scale_shift;
		moving_scenery_record++;
		OBJECT(current_object)->side = MOVING_SCENERY_RECORD(moving_scenery_record - 0x7u)->side;
		moving_scenery_record++;
		OBJECT(current_object)->x_offset = MOVING_SCENERY_RECORD(moving_scenery_record - 0x8u)->x_offset;
		moving_scenery_record += 2u;
		OBJECT(current_object)->y_offset = MOVING_SCENERY_RECORD(moving_scenery_record - 0xAu)->y_offset;
		moving_scenery_record += 2u;
		OBJECT(current_object)->moving_scenery.trigger_distance = MOVING_SCENERY_RECORD(moving_scenery_record - 0xCu)->trigger_distance;
		OBJECT(current_object)->moving_scenery.trigger_distance += 0x10u;
		moving_scenery_record += 2u;
		OBJECT(current_object)->moving_scenery.x_speed = MOVING_SCENERY_RECORD(moving_scenery_record - 0xEu)->x_speed;
		moving_scenery_record += 2u;
		OBJECT(current_object)->moving_scenery.y_speed = MOVING_SCENERY_RECORD(moving_scenery_record - 0x10u)->y_speed;
		OBJECT(current_object)->moving_scenery.road_speed = MOVING_SCENERY_RECORD(moving_scenery_record - 0x10u)->road_speed;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->moving_scenery.trigger_position = position - OBJECT(current_object)->moving_scenery.trigger_distance;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->moving_scenery.appear_position = position - draw_distance;
		OBJECT(current_object)->moving_scenery.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->moving_scenery.trigger_position)
			return;
		OBJECT(current_object)->state++;
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->position)
		{
			OBJECT(current_object)->x_offset += OBJECT(current_object)->moving_scenery.x_speed;
			OBJECT(current_object)->y_offset += OBJECT(current_object)->moving_scenery.y_speed;
			OBJECT(current_object)->position += OBJECT(current_object)->moving_scenery.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->moving_scenery.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->moving_scenery.screen_x >= -0xF0 && OBJECT(current_object)->moving_scenery.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->window_segment < OBJECT(current_object)->position)
		{
			OBJECT(current_object)->x_offset += OBJECT(current_object)->moving_scenery.x_speed;
			OBJECT(current_object)->y_offset += OBJECT(current_object)->moving_scenery.y_speed;
			OBJECT(current_object)->position += OBJECT(current_object)->moving_scenery.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->moving_scenery.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->moving_scenery.screen_x >= -0xF0 && OBJECT(current_object)->moving_scenery.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	default:
		invalid_jump(moving_scenery_state_jumps[state]);
	}
}

/* The helicopter over the waterfalls of stage 4: it swoops down, then flies off. By state: 0 reads the
   record (animation, palette, flags, draw_mode, scale_shift, side, x_offset, y_offset, trigger_distance
   + 16, x_speed, y_speed, road_speed, dive_frames, dive_y_speed), trigger_position = position -
   trigger_distance, appear_position = position - draw_distance; 1 not drawn until the view (race_copy
   window_segment) passes trigger_position, then dive_countdown = dive_frames; 2 for dive_frames frames
   y_offset += dive_y_speed and position += road_speed; 3 x_offset += x_speed, y_offset += y_speed,
   position += road_speed; from 2 on, animation_advance and draw_object while within draw_distance of the
   car (car window_segment); it ends when the car reaches it, when it gets beyond draw_distance or when
   its screen_x leaves -F0..130. */
void swooping_helicopter_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, routine = 0, swooping_helicopter_record = 0;
	uint8_t dive_countdown = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = swooping_helicopter_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		swooping_helicopter_record = OBJECT(current_object)->swooping_helicopter.record;
		animation = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record)->animation;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->palette = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x2u)->palette;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->flags |= SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x4u)->flags;
		swooping_helicopter_record++;
		OBJECT(current_object)->draw_mode = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x5u)->draw_mode;
		swooping_helicopter_record++;
		OBJECT(current_object)->scale_shift = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x6u)->scale_shift;
		swooping_helicopter_record++;
		OBJECT(current_object)->side = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x7u)->side;
		swooping_helicopter_record++;
		OBJECT(current_object)->x_offset = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x8u)->x_offset;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->y_offset = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0xAu)->y_offset;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->swooping_helicopter.trigger_distance = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0xCu)->trigger_distance;
		OBJECT(current_object)->swooping_helicopter.trigger_distance += 0x10u;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->swooping_helicopter.x_speed = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0xEu)->x_speed;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->swooping_helicopter.y_speed = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x10u)->y_speed;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->swooping_helicopter.road_speed = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x12u)->road_speed;
		swooping_helicopter_record += 2u;
		OBJECT(current_object)->swooping_helicopter.dive_frames = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x14u)->dive_frames;
		OBJECT(current_object)->swooping_helicopter.dive_y_speed = SWOOPING_HELICOPTER_RECORD(swooping_helicopter_record - 0x14u)->dive_y_speed;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->swooping_helicopter.trigger_position = position - OBJECT(current_object)->swooping_helicopter.trigger_distance;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->swooping_helicopter.appear_position = position - draw_distance;
		OBJECT(current_object)->swooping_helicopter.draw_distance = draw_distance;
		return;
	case 1:
		if (OBJECT(current_object)->swooping_helicopter.trigger_position < (int16_t)RACE(SCREEN(screen)->race_copy)->window_segment)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->swooping_helicopter.dive_countdown = OBJECT(current_object)->swooping_helicopter.dive_frames;
			routine = draw_object(routine);
			return;
		}
		return;
	case 2:
		OBJECT(current_object)->y_offset += OBJECT(current_object)->swooping_helicopter.dive_y_speed;
		dive_countdown = OBJECT(current_object)->swooping_helicopter.dive_countdown;
		OBJECT(current_object)->swooping_helicopter.dive_countdown = dive_countdown - 1u;
		if (dive_countdown != 1u)
		{
			OBJECT(current_object)->position += OBJECT(current_object)->swooping_helicopter.road_speed;
			if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
			{
				animation_advance(0u);
				draw_object(routine);
				if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
					return;
			}
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->swooping_helicopter.x_speed;
		OBJECT(current_object)->y_offset += OBJECT(current_object)->swooping_helicopter.y_speed;
		OBJECT(current_object)->position += OBJECT(current_object)->swooping_helicopter.road_speed;
		if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
		{
			animation_advance(0u);
			draw_object(routine);
			if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
				return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 3:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->swooping_helicopter.x_speed;
		OBJECT(current_object)->y_offset += OBJECT(current_object)->swooping_helicopter.y_speed;
		OBJECT(current_object)->position += OBJECT(current_object)->swooping_helicopter.road_speed;
		if ((int16_t)(0u - OBJECT(current_object)->swooping_helicopter.draw_distance + OBJECT(current_object)->position) < (int16_t)CAR(player_car)->window_segment)
		{
			animation_advance(0u);
			draw_object(routine);
			if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
				return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	default:
		invalid_jump(swooping_helicopter_state_jumps[state]);
	}
}

/* The START banner over the start line and its two posts with the signal lights (stream 0 at 20: four of
   them). By state: 0 reads the record as road_row_start does (animation, palette, flags, draw_mode,
   scale_shift, side, x_offset, y_offset, spacing, count, hit box; first_position, end_position), and
   goes on to state 2 without a hit box; 1 road_row_crash_check and draw_object (no record has a hit
   box); 2 the lights: palette from start_gate_light_palettes by the seconds to the start (the car
   object's timer, in frames, rounded up): C9 until the last 3 seconds, then CC, CB, and CA at the start,
   which goes on to state 3; 3 draw_object. It never ends. */
void start_gate_update(void)
{
	uint16_t position = 0, row_length = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, seconds_left = 0, start_gate_record = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	seconds_left = start_gate_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		start_gate_record = OBJECT(current_object)->start_gate.record;
		animation = START_GATE_RECORD(start_gate_record)->animation;
		start_gate_record += 2u;
		OBJECT(current_object)->palette = START_GATE_RECORD(start_gate_record - 0x2u)->palette;
		start_gate_record += 2u;
		OBJECT(current_object)->flags |= START_GATE_RECORD(start_gate_record - 0x4u)->flags;
		start_gate_record++;
		OBJECT(current_object)->draw_mode = START_GATE_RECORD(start_gate_record - 0x5u)->draw_mode;
		start_gate_record++;
		OBJECT(current_object)->scale_shift = START_GATE_RECORD(start_gate_record - 0x6u)->scale_shift;
		start_gate_record++;
		OBJECT(current_object)->side = START_GATE_RECORD(start_gate_record - 0x7u)->side;
		start_gate_record++;
		OBJECT(current_object)->x_offset = START_GATE_RECORD(start_gate_record - 0x8u)->x_offset;
		start_gate_record += 2u;
		OBJECT(current_object)->y_offset = START_GATE_RECORD(start_gate_record - 0xAu)->y_offset;
		start_gate_record += 2u;
		OBJECT(current_object)->start_gate.spacing = START_GATE_RECORD(start_gate_record - 0xCu)->spacing;
		start_gate_record += 2u;
		OBJECT(current_object)->start_gate.count = START_GATE_RECORD(start_gate_record - 0xEu)->count;
		start_gate_record += 2u;
		if (START_GATE_RECORD(start_gate_record - 0x10u)->hit_box_along == 0u)
			OBJECT(current_object)->state++;
		OBJECT(current_object)->start_gate.hit_box = start_gate_record;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		row_length = (uint16_t)(OBJECT(current_object)->start_gate.spacing * (OBJECT(current_object)->start_gate.count - 1u) + 1u);
		OBJECT(current_object)->start_gate.first_position = position;
		OBJECT(current_object)->start_gate.end_position = position + row_length;
		return;
	case 1:
		road_row_crash_check(&ignored, &ignored_2);
		seconds_left = draw_object(seconds_left);
		return;
	case 2:
		seconds_left = OBJECT(CAR(player_car)->object)->player_car.timer;
		seconds_left = (uint16_t)(seconds_left + 0x1Du);
		seconds_left = seconds_left / 0x1Eu;
		OBJECT(current_object)->palette = start_gate_light_palettes[seconds_left];
		if (seconds_left == 0u)
			OBJECT(current_object)->state++;
		seconds_left = draw_object(seconds_left);
		return;
	case 3:
		seconds_left = draw_object(seconds_left);
		return;
	default:
		invalid_jump(start_gate_state_jumps[state]);
	}
}

/* The man at the start line who waves the chequered flag as the race starts (stream 0 at 20). By state:
   0 reads the record (animation, go_animation, palette, side, x_offset, y_offset), draw_mode 9,
   scale_shift 1; 1 drawn standing (06E) while the car object's timer counts the frames to the start,
   then go_animation (06F, the waving); 1 and 2: ends once the car (car window_segment) reaches its
   position, else animation_advance and draw_object. */
void start_flagman_update(void)
{
	uint32_t animation = 0, flagman_record = 0, frames_to_start = 0, go_animation = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	frames_to_start = start_flagman_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		flagman_record = OBJECT(current_object)->start_flagman.record;
		animation = FLAGMAN_RECORD(flagman_record)->animation;
		flagman_record += 2u;
		OBJECT(current_object)->start_flagman.go_animation = FLAGMAN_RECORD(flagman_record - 0x2u)->waving_animation;
		flagman_record += 2u;
		OBJECT(current_object)->palette = FLAGMAN_RECORD(flagman_record - 0x4u)->palette;
		flagman_record += 2u;
		OBJECT(current_object)->side = FLAGMAN_RECORD(flagman_record - 0x6u)->side;
		flagman_record++;
		OBJECT(current_object)->x_offset = FLAGMAN_RECORD(flagman_record - 0x7u)->x_offset;
		OBJECT(current_object)->y_offset = FLAGMAN_RECORD(flagman_record - 0x7u)->y_offset;
		animation_start(animation);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		return;
	case 1:
		frames_to_start = OBJECT(CAR(player_car)->object)->player_car.timer;
		if (frames_to_start != 0u)
		{
			frames_to_start = draw_object(frames_to_start);
			return;
		}
		OBJECT(current_object)->state++;
		go_animation = OBJECT(current_object)->start_flagman.go_animation;
		animation_start(go_animation);
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(frames_to_start);
		return;
	case 2:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(frames_to_start);
		return;
	default:
		invalid_jump(start_flagman_state_jumps[state]);
	}
}

/* The man waving a chequered flag at each fork and at the goal. By state: 0 reads the record (animation,
   in_view_animation, palette, side, x_offset, y_offset), draw_mode 9, scale_shift 1, appear_position =
   position - draw_distance (and 58 = position - 30, never read); 1 not drawn until the view (race_copy
   window_segment) passes appear_position, then in_view_animation (06F, the waving; so 06E is never
   drawn); 1 and 2: ends once the car (car window_segment) reaches its position, else animation_advance
   and draw_object. */
void flagman_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, flagman_record = 0, in_view_animation = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = flagman_state_jumps[state];
	switch (state)
	{
	case 0:
		flagman_record = OBJECT(current_object)->flagman.record;
		animation = FLAGMAN_RECORD(flagman_record)->animation;
		flagman_record += 2u;
		OBJECT(current_object)->flagman.in_view_animation = FLAGMAN_RECORD(flagman_record - 0x2u)->waving_animation;
		flagman_record += 2u;
		OBJECT(current_object)->palette = FLAGMAN_RECORD(flagman_record - 0x4u)->palette;
		flagman_record += 2u;
		OBJECT(current_object)->side = FLAGMAN_RECORD(flagman_record - 0x6u)->side;
		flagman_record++;
		OBJECT(current_object)->x_offset = FLAGMAN_RECORD(flagman_record - 0x7u)->x_offset;
		OBJECT(current_object)->y_offset = FLAGMAN_RECORD(flagman_record - 0x7u)->y_offset;
		animation_start(animation);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->flagman.trigger_position = position - 0x1Eu;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->flagman.appear_position = position - draw_distance;
		OBJECT(current_object)->flagman.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->flagman.appear_position)
			return;
		OBJECT(current_object)->state++;
		in_view_animation = OBJECT(current_object)->flagman.in_view_animation;
		animation_start(in_view_animation);
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	case 2:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(flagman_state_jumps[state]);
	}
}

/* The balloons let go over the start line as the race starts (stream 0 at 24-34). By state: 0 reads the
   record (animation, palette, side, x_offset, y_offset), draw_mode 9, scale_shift 1; 1 not drawn until
   the start (the car object's timer down to 0); 1 and 2: it rises faster and faster (rise_speed + 1 each
   frame, y_offset - rise_speed / 2) and drifts (x_offset + 8), animation_advance and draw_object; it
   ends when the car (car window_segment) reaches its position or y_offset passes -3000 (high above). */
void start_balloon_update(void)
{
	uint32_t animation = 0, rise_speed = 0, start_balloon_record = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		start_balloon_record = OBJECT(current_object)->start_balloon.record;
		animation = START_BALLOON_RECORD(start_balloon_record)->animation;
		start_balloon_record += 2u;
		OBJECT(current_object)->palette = START_BALLOON_RECORD(start_balloon_record - 0x2u)->palette;
		start_balloon_record += 2u;
		OBJECT(current_object)->side = START_BALLOON_RECORD(start_balloon_record - 0x4u)->side;
		start_balloon_record++;
		OBJECT(current_object)->x_offset = START_BALLOON_RECORD(start_balloon_record - 0x5u)->x_offset;
		OBJECT(current_object)->y_offset = START_BALLOON_RECORD(start_balloon_record - 0x5u)->y_offset;
		animation_start(animation);
		OBJECT(current_object)->draw_mode = 9u;
		OBJECT(current_object)->scale_shift = 1u;
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
			return;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment || OBJECT(current_object)->y_offset < -0x3000)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		rise_speed = (uint16_t)(OBJECT(current_object)->start_balloon.rise_speed + 1u);
		OBJECT(current_object)->start_balloon.rise_speed = rise_speed;
		rise_speed = rise_speed >> 1;
		OBJECT(current_object)->y_offset -= rise_speed;
		OBJECT(current_object)->x_offset += 8u;
		rise_speed = draw_object(rise_speed);
		return;
	case 2:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment || OBJECT(current_object)->y_offset < -0x3000)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		rise_speed = (uint16_t)(OBJECT(current_object)->start_balloon.rise_speed + 1u);
		OBJECT(current_object)->start_balloon.rise_speed = rise_speed;
		rise_speed = rise_speed >> 1;
		OBJECT(current_object)->y_offset -= rise_speed;
		OBJECT(current_object)->x_offset += 8u;
		rise_speed = draw_object(rise_speed);
		return;
	default:
		invalid_jump(start_balloon_state_jumps[OBJECT(current_object)->state]);
	}
}

/* A sound repeated at random along a stretch of road (the jungle of stage 3: sound 51): plays it, then
   again after 15 to 78 frames at random (random_number), until the car (car segment) has gone length
   segments from where it started. */
void ambient_sound_update(void)
{
	uint8_t sound = 0;
	uint32_t random_bits = 0;
	uint8_t countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->ambient_sound.end_position = CAR(player_car)->segment + OBJECT(current_object)->ambient_sound.length;
		OBJECT(current_object)->state++;
		sound = 0x51u;
		request_sound(sound);
		return;
	case 1:
		random_bits = random_number();
		OBJECT(current_object)->ambient_sound.countdown = (random_bits & 0x3Fu) + 0xFu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(ambient_sound_state_jumps[OBJECT(current_object)->state]);
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
	sound = 0x51u;
	request_sound(sound);
}

/* Plays sound 23 once and ends: placed at the start of stage 30's stream, so it sounds as the road of
   stage 30 starts being built. */
void one_shot_sound_update(void)
{
	uint8_t sound = 0;

	OBJECT(current_object)->flags = 0u;
	sound = 0x23u;
	request_sound(sound);
}

/* The WEST BOUND / EAST BOUND panel at the bottom centre of the screen before the first fork (stream 0
   at 550): animation 39 at a screen position (draw_mode A, layer 19D), its palette cycling through
   base_palette to base_palette + 3 (a step every 4 frames, colour_count). By state: 0 sets it up below
   the screen (y 118); 1 rises 2 a frame to y DC; 1 and 2: once the car (car segment) passes 250 or the
   race time runs out (word 6A of the race record screen host_record points at), 3: goes down 2 a frame
   and ends at y 118. */
void west_east_panel_update(void)
{
	uint32_t animation = 0, palette = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x118u;
		animation = 0x39u;
		OBJECT(current_object)->west_east_panel.base_palette = 0x67u;
		animation_start(animation);
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
				OBJECT(current_object)->flags_high = 0u;
				return;
			}
		}
		palette = OBJECT(current_object)->west_east_panel.colour_count;
		palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = palette;
		palette = (uint16_t)(((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = palette;
		palette = draw_object(palette);
		return;
	case 2:
		if (RACE(SCREEN(screen)->host_record)->time_left == 0u || CAR(player_car)->segment > 0x250u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->y += 2u;
			if (OBJECT(current_object)->y == 0x118u)
			{
				OBJECT(current_object)->flags = 0u;
				OBJECT(current_object)->flags_high = 0u;
				return;
			}
		}
		palette = OBJECT(current_object)->west_east_panel.colour_count;
		palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = palette;
		palette = (uint16_t)(((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = palette;
		palette = draw_object(palette);
		return;
	case 3:
		OBJECT(current_object)->y += 2u;
		if (OBJECT(current_object)->y == 0x118u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		palette = OBJECT(current_object)->west_east_panel.colour_count;
		palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
		OBJECT(current_object)->west_east_panel.colour_count = palette;
		palette = (uint16_t)(((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette);
		OBJECT(current_object)->palette = palette;
		palette = draw_object(palette);
		return;
	default:
		invalid_jump(west_east_panel_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Santa's sleigh and reindeer flying back and forth over the goal of stage 26, with sound 49 (sound)
   every 15 frames. By state: 0 reads the record (animation, palette, flags, side, x_offset, x_speed,
   sound, draw_mode, scale_shift; y_offset -100 for animation 3A0, -180 for 3A2), appear_position =
   position - draw_distance (and 58 = position - 100, never read); 1 not drawn until the view (race_copy
   window_segment) passes appear_position; 2 moves by x_speed (leftward) until its screen_x is below -B0,
   then 3: mirrored (flags bit 6), moves back until screen_x is above F0, then 2 again; each frame
   animation_advance and draw_object. It never ends (the race ends at the goal). */
void santa_sleigh_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, ignored = 0, santa_sleigh_record = 0, sound = 0;
	uint8_t sound_countdown = 0, sound_countdown_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	sound = santa_sleigh_state_jumps[state];
	switch (state)
	{
	case 0:
		santa_sleigh_record = OBJECT(current_object)->santa_sleigh.record;
		animation = SANTA_SLEIGH_RECORD(santa_sleigh_record)->animation;
		santa_sleigh_record += 2u;
		if (animation == 0x3A2u)
			OBJECT(current_object)->y_offset = -0x180;
		if (animation == 0x3A0u)
			OBJECT(current_object)->y_offset = -0x100;
		OBJECT(current_object)->palette = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0x2u)->palette;
		santa_sleigh_record += 2u;
		OBJECT(current_object)->flags |= SANTA_SLEIGH_RECORD(santa_sleigh_record - 0x4u)->flags;
		santa_sleigh_record++;
		OBJECT(current_object)->side = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0x5u)->side;
		santa_sleigh_record++;
		OBJECT(current_object)->x_offset = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0x6u)->x_offset;
		santa_sleigh_record += 2u;
		OBJECT(current_object)->santa_sleigh.x_speed = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0x8u)->x_speed;
		santa_sleigh_record += 2u;
		OBJECT(current_object)->santa_sleigh.sound = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0xAu)->sound;
		santa_sleigh_record += 2u;
		OBJECT(current_object)->draw_mode = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0xCu)->draw_mode;
		OBJECT(current_object)->scale_shift = SANTA_SLEIGH_RECORD(santa_sleigh_record - 0xCu)->scale_shift;
		OBJECT(current_object)->santa_sleigh.sound_countdown = 0xAu;
		animation_start(animation);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->santa_sleigh.trigger_position = position - 0x64u;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->santa_sleigh.appear_position = position - draw_distance;
		OBJECT(current_object)->santa_sleigh.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->santa_sleigh.appear_position)
			return;
		OBJECT(current_object)->state++;
	case 2:
		sound_countdown = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown - 1u;
		if (sound_countdown == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			sound = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)sound);
		}
		OBJECT(current_object)->x_offset += OBJECT(current_object)->santa_sleigh.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->santa_sleigh.screen_x >= -0xB0)
		{
			sound = draw_object(sound);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		sound_countdown_2 = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown_2 - 1u;
		if (sound_countdown_2 == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			sound = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)sound);
		}
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->santa_sleigh.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->santa_sleigh.screen_x > 0xF0)
		{
			OBJECT(current_object)->state = 2u;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		sound = draw_object(sound);
		return;
	case 3:
		sound_countdown_2 = OBJECT(current_object)->santa_sleigh.sound_countdown;
		OBJECT(current_object)->santa_sleigh.sound_countdown = sound_countdown_2 - 1u;
		if (sound_countdown_2 == 1u)
		{
			OBJECT(current_object)->santa_sleigh.sound_countdown = 0xFu;
			sound = OBJECT(current_object)->santa_sleigh.sound;
			request_sound((uint8_t)sound);
		}
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->santa_sleigh.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->santa_sleigh.screen_x > 0xF0)
		{
			OBJECT(current_object)->state = 2u;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		sound = draw_object(sound);
		return;
	default:
		invalid_jump(santa_sleigh_state_jumps[state]);
	}
}

/* What runs back and forth in front of a goal's crowd, setting off rightward: a kangaroo (stage 21), a
   dragon dance (22), an orca leaping over the sea (28). By state: 0 reads the record (animation,
   palette, flags, side, x_offset, x_speed, draw_mode, scale_shift; y_offset -180 for animation 3A2),
   appear_position = position - draw_distance (and 58 = position - 100, never read); 1 not drawn until
   the view (race_copy window_segment) passes appear_position; 2 moves by x_speed (mirrored by the
   record's flags bit 6) until its screen_x passes B0, then 3: unmirrored, moves back until screen_x is
   below -F0, then 2 again; each frame animation_advance and draw_object. It never ends. */
void goal_parade_rightward_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, goal_parade_record = 0, ignored = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = goal_parade_rightward_state_jumps[state];
	switch (state)
	{
	case 0:
		goal_parade_record = OBJECT(current_object)->goal_parade.record;
		animation = GOAL_PARADE_RECORD(goal_parade_record)->animation;
		goal_parade_record += 2u;
		if (animation == 0x3A2u)
			OBJECT(current_object)->y_offset = -0x180;
		OBJECT(current_object)->palette = GOAL_PARADE_RECORD(goal_parade_record - 0x2u)->palette;
		goal_parade_record += 2u;
		OBJECT(current_object)->flags |= GOAL_PARADE_RECORD(goal_parade_record - 0x4u)->flags;
		goal_parade_record++;
		OBJECT(current_object)->side = GOAL_PARADE_RECORD(goal_parade_record - 0x5u)->side;
		goal_parade_record++;
		OBJECT(current_object)->x_offset = GOAL_PARADE_RECORD(goal_parade_record - 0x6u)->x_offset;
		goal_parade_record += 2u;
		OBJECT(current_object)->goal_parade.x_speed = GOAL_PARADE_RECORD(goal_parade_record - 0x8u)->x_speed;
		goal_parade_record += 2u;
		OBJECT(current_object)->draw_mode = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->draw_mode;
		OBJECT(current_object)->scale_shift = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->scale_shift;
		animation_start(animation);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->goal_parade.trigger_position = position - 0x64u;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - draw_distance;
		OBJECT(current_object)->goal_parade.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags &= 0xBFu;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		routine = draw_object(routine);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags &= 0xBFu;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		routine = draw_object(routine);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(goal_parade_rightward_state_jumps[state]);
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
	uint16_t position = 0, row_length = 0;
	uint32_t animation = 0, animation_order = 0, sounding_scenery_record = 0;
	uint8_t sound_countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		sounding_scenery_record = OBJECT(current_object)->sounding_scenery.record;
		animation = SOUNDING_SCENERY_RECORD(sounding_scenery_record)->animation;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->palette = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x2u)->palette;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->flags |= SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x4u)->flags;
		sounding_scenery_record++;
		OBJECT(current_object)->sounding_scenery.size_table = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x5u)->size_table;
		sounding_scenery_record++;
		OBJECT(current_object)->scale_shift = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x6u)->scale_shift;
		sounding_scenery_record++;
		OBJECT(current_object)->side = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x7u)->side;
		sounding_scenery_record++;
		OBJECT(current_object)->x_offset = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x8u)->x_offset;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->y_offset = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0xAu)->y_offset;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->sounding_scenery.spacing = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0xCu)->spacing;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->sounding_scenery.count = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0xEu)->count;
		sounding_scenery_record += 2u;
		OBJECT(current_object)->sounding_scenery.sound = SOUNDING_SCENERY_RECORD(sounding_scenery_record - 0x10u)->sound;
		OBJECT(current_object)->sounding_scenery.sound_countdown = 0x3Cu;
		OBJECT(current_object)->sounding_scenery.sound_entry = sounding_scenery_record;
		animation_start(animation);
		position = (uint16_t)OBJECT(current_object)->position;
		row_length = (uint16_t)(OBJECT(current_object)->sounding_scenery.spacing * (OBJECT(current_object)->sounding_scenery.count - 1u) + 1u);
		OBJECT(current_object)->sounding_scenery.first_position = position;
		OBJECT(current_object)->sounding_scenery.end_position = position + row_length;
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
		animation_order = animation_advance(0u);
		road_row_draw(animation_order, 0u);
		return;
	case 3:
		animation_order = animation_advance(0u);
		road_row_draw(animation_order, 0u);
		return;
	default:
		invalid_jump(sounding_scenery_state_jumps[OBJECT(current_object)->state]);
	}
}

/* What runs back and forth in front of a goal's crowd, setting off leftward: a kangaroo (stage 21), an
   orca (28), the bulls (29). As goal_parade_rightward_update, with the turns the other way: 2 moves by
   x_speed (negative) until its screen_x is below -B0, then 3: mirrored (flags bit 6), moves back until
   screen_x is above F0, then 2 again. It never ends. */
void goal_parade_leftward_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, goal_parade_record = 0, ignored = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = goal_parade_leftward_state_jumps[state];
	switch (state)
	{
	case 0:
		goal_parade_record = OBJECT(current_object)->goal_parade.record;
		animation = GOAL_PARADE_RECORD(goal_parade_record)->animation;
		goal_parade_record += 2u;
		if (animation == 0x3A2u)
			OBJECT(current_object)->y_offset = -0x180;
		OBJECT(current_object)->palette = GOAL_PARADE_RECORD(goal_parade_record - 0x2u)->palette;
		goal_parade_record += 2u;
		OBJECT(current_object)->flags |= GOAL_PARADE_RECORD(goal_parade_record - 0x4u)->flags;
		goal_parade_record++;
		OBJECT(current_object)->side = GOAL_PARADE_RECORD(goal_parade_record - 0x5u)->side;
		goal_parade_record++;
		OBJECT(current_object)->x_offset = GOAL_PARADE_RECORD(goal_parade_record - 0x6u)->x_offset;
		goal_parade_record += 2u;
		OBJECT(current_object)->goal_parade.x_speed = GOAL_PARADE_RECORD(goal_parade_record - 0x8u)->x_speed;
		goal_parade_record += 2u;
		OBJECT(current_object)->draw_mode = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->draw_mode;
		OBJECT(current_object)->scale_shift = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->scale_shift;
		animation_start(animation);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->goal_parade.trigger_position = position - 0x64u;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - draw_distance;
		OBJECT(current_object)->goal_parade.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		routine = draw_object(routine);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		ignored = animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		routine = draw_object(routine);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(goal_parade_leftward_state_jumps[state]);
	}
}

/* The dragon dance in front of the crowd at the goal of stage 22 (with goal_parade_rightward's second
   dragon): as goal_parade_leftward_update, plus sound 44 on one frame in four at random (random_number).
   It never ends. */
void goal_parade_dragon_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, goal_parade_record = 0, random_bits = 0, sound = 0;
	uint8_t chance = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		goal_parade_record = OBJECT(current_object)->goal_parade.record;
		animation = GOAL_PARADE_RECORD(goal_parade_record)->animation;
		goal_parade_record += 2u;
		OBJECT(current_object)->palette = GOAL_PARADE_RECORD(goal_parade_record - 0x2u)->palette;
		goal_parade_record += 2u;
		OBJECT(current_object)->flags |= GOAL_PARADE_RECORD(goal_parade_record - 0x4u)->flags;
		goal_parade_record++;
		OBJECT(current_object)->side = GOAL_PARADE_RECORD(goal_parade_record - 0x5u)->side;
		goal_parade_record++;
		OBJECT(current_object)->x_offset = GOAL_PARADE_RECORD(goal_parade_record - 0x6u)->x_offset;
		goal_parade_record += 2u;
		OBJECT(current_object)->goal_parade.x_speed = GOAL_PARADE_RECORD(goal_parade_record - 0x8u)->x_speed;
		goal_parade_record += 2u;
		OBJECT(current_object)->draw_mode = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->draw_mode;
		OBJECT(current_object)->scale_shift = GOAL_PARADE_RECORD(goal_parade_record - 0xAu)->scale_shift;
		animation_start(animation);
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->goal_parade.trigger_position = position - 0x64u;
		draw_distance = SCREEN(screen)->draw_distance >> 7;
		OBJECT(current_object)->goal_parade.appear_position = position - draw_distance;
		OBJECT(current_object)->goal_parade.draw_distance = draw_distance;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->window_segment <= OBJECT(current_object)->goal_parade.appear_position)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			random_bits = random_number();
			chance = random_bits & 3u;
			sound = (random_bits & ~0xFFu) | chance;
			if (chance == 0u)
			{
				sound = (sound & ~0xFFu) | 0x44u;
				request_sound((uint8_t)sound);
			}
			animation_advance(0u);
			sound = draw_object(sound);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			random_bits = random_number();
			chance = random_bits & 3u;
			sound = (random_bits & ~0xFFu) | chance;
			if (chance == 0u)
			{
				sound = (sound & ~0xFFu) | 0x44u;
				request_sound((uint8_t)sound);
			}
			animation_advance(0u);
			sound = draw_object(sound);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		random_bits = random_number();
		chance = random_bits & 3u;
		sound = (random_bits & ~0xFFu) | chance;
		if (chance == 0u)
		{
			sound = (sound & ~0xFFu) | 0x44u;
			request_sound((uint8_t)sound);
		}
		animation_advance(0u);
		sound = draw_object(sound);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			random_bits = random_number();
			chance = random_bits & 3u;
			sound = (random_bits & ~0xFFu) | chance;
			if (chance == 0u)
			{
				sound = (sound & ~0xFFu) | 0x44u;
				request_sound((uint8_t)sound);
			}
			animation_advance(0u);
			sound = draw_object(sound);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			random_bits = random_number();
			chance = random_bits & 3u;
			sound = (random_bits & ~0xFFu) | chance;
			if (chance == 0u)
			{
				sound = (sound & ~0xFFu) | 0x44u;
				request_sound((uint8_t)sound);
			}
			animation_advance(0u);
			sound = draw_object(sound);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		random_bits = random_number();
		chance = random_bits & 3u;
		sound = (random_bits & ~0xFFu) | chance;
		if (chance == 0u)
		{
			sound = (sound & ~0xFFu) | 0x44u;
			request_sound((uint8_t)sound);
		}
		animation_advance(0u);
		sound = draw_object(sound);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		random_bits = random_number();
		chance = random_bits & 3u;
		sound = (random_bits & ~0xFFu) | chance;
		if (chance == 0u)
		{
			sound = (sound & ~0xFFu) | 0x44u;
			request_sound((uint8_t)sound);
		}
		animation_advance(0u);
		sound = draw_object(sound);
		return;
	default:
		invalid_jump(goal_parade_dragon_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Starts the traffic of the stage the race's leader is in (object_lists_2E1F0). By state: 0 the list of
   the stage being built (screen built_stage), traffic_level = 8 - the number of players (byte 1F of the
   record screen host_record points at) + the game setting game_difficulty, the draw distance into screen
   traffic_draw_distance, no traffic yet (screen traffic_count); 1 (outside the attract demo, screen
   demo_flags bit 8) when the leader's stage (screen leader_stage) changes, takes its list with positions
   from where the leader was then (screen leader_window_segment); starts each record (position, level,
   handler, stop_distance, record) whose position is within the draw distance of the leader's view
   (window_segment of the race record screen leader_record), when its level is not above traffic_level
   and fewer than 32 are on the road (screen traffic_count); then writes at F84 of the screen the
   stretches of road the players can see (each player's view, window_segment of the race records listed
   after byte 1F, give or take the draw distance; a count, then each stretch's top and bottom from the
   highest), for traffic_out_of_view. */
void traffic_spawner_update(void)
{
	uint32_t negative = 0, overflow = 0;
	uint16_t object_position = 0, position = 0, spawn_position = 0, stretch_count = 0, stretch_top = 0, traffic_draw_distance = 0, view_bottom = 0, view_top = 0;
	uint32_t built_stage = 0, entry = 0, leader_stage = 0, racer_list = 0, racers_left = 0, stretches = 0, traffic_object = 0;
	uint16_t stage_start = 0, road_position = 0, level = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->traffic_spawner.stage_start = 0;
		built_stage = SCREEN(screen)->built_stage;
		OBJECT(current_object)->traffic_spawner.stage = built_stage;
		OBJECT(current_object)->traffic_spawner.list = object_lists_2E1F0[built_stage];
		OBJECT(current_object)->traffic_spawner.traffic_level = (uint8_t)(8u - RACE(SCREEN(screen)->host_record)->racer_count + game_difficulty);
		SCREEN(screen)->traffic_draw_distance = SCREEN(screen)->draw_distance >> 7;
		SCREEN(screen)->traffic_racer_list = SCREEN(screen)->host_record + 0x1Fu;
		SCREEN(screen)->traffic_count = 0u;
		OBJECT(current_object)->state++;
		return;
	case 1:
		entry = OBJECT(current_object)->traffic_spawner.list;
		if ((SCREEN(screen)->demo_flags & 0x100u) == 0u && OBJECT(current_object)->traffic_spawner.stage != SCREEN(screen)->leader_stage)
		{
			OBJECT(current_object)->traffic_spawner.stage_start = SCREEN(screen)->leader_window_segment;
			leader_stage = SCREEN(screen)->leader_stage;
			OBJECT(current_object)->traffic_spawner.stage = leader_stage;
			entry = object_lists_2E1F0[leader_stage];
			OBJECT(current_object)->traffic_spawner.list = entry;
		}
		for (;;)
		{
			position = (uint16_t)TRAFFIC_SPAWN_ENTRY(entry)->position;
			stage_start = OBJECT(current_object)->traffic_spawner.stage_start;
			road_position = position + stage_start;
			overflow = (uint32_t)(int16_t)position + (int16_t)stage_start != (uint32_t)(int16_t)(position + stage_start);
			negative = road_position >> 15;
			spawn_position = road_position;
			entry += 2u;
			if (negative ^ overflow)
				break;
			object_position = (uint16_t)spawn_position;
			if ((int16_t)RACE(SCREEN(screen)->leader_record)->window_segment < (int16_t)(spawn_position - SCREEN(screen)->traffic_draw_distance))
				break;
			level = TRAFFIC_SPAWN_ENTRY(entry - 0x2u)->level;
			entry += 2u;
			if (OBJECT(current_object)->traffic_spawner.traffic_level < level || SCREEN(screen)->traffic_count >= 0x20u)
			{
				entry += 0xCu;
				OBJECT(current_object)->traffic_spawner.list = entry;
				continue;
			}
			traffic_object = start_object();
			OBJECT(traffic_object)->position = object_position;
			OBJECT(traffic_object)->handler = TRAFFIC_SPAWN_ENTRY(entry - 0x4u)->handler;
			entry += 4u;
			OBJECT(traffic_object)->traffic.stop_distance = TRAFFIC_SPAWN_ENTRY(entry - 0x8u)->stop_distance;
			entry += 4u;
			OBJECT(traffic_object)->traffic.record = TRAFFIC_SPAWN_ENTRY(entry - 0xCu)->record;
			entry += 4u;
			OBJECT(current_object)->traffic_spawner.list = entry;
			SCREEN(screen)->traffic_count++;
		}
		racer_list = SCREEN(screen)->traffic_racer_list;
		stretches = screen + 0xF84u;
		traffic_draw_distance = SCREEN(screen)->traffic_draw_distance;
		stretch_count = 0u;
		stretch_top = 0xFFFFu;
		racers_left = mem_read8(racer_list);
		racer_list++;
		do
		{
			view_bottom = race_copy_window_segments[(uint32_t)mem_read8(racer_list) << 6];
			view_top = (uint16_t)((uint32_t)view_bottom + traffic_draw_distance);
			view_bottom -= traffic_draw_distance;
			view_top++;
			racer_list++;
			if (view_top < stretch_top)
			{
				mem_write16(stretches, stretch_top);
				stretches += 2u;
				mem_write16(stretches, view_top - 1u);
				stretch_count++;
				stretches += 2u;
			}
			stretch_top = (uint16_t)view_bottom;
			racers_left--;
		} while (racers_left != 0u);
		mem_write16(stretches, stretch_top);
		SCREEN(screen)->visible_stretch_count = stretch_count;
		return;
	default:
		invalid_jump(traffic_spawner_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The first vehicle of a convoy (the only one: stage 25, animation 35C followed by eight 35D): its
   record starts with the distance between them and the records of those that follow, closed by 0; starts
   each (handler 013AB7: traffic_car_update, kept live as long as the one before it is, which ends it)
   that distance behind the one before, keeping the one before in its field 14, with the same
   stop_distance, counted in screen traffic_count; then becomes a traffic_car with the record after the
   list. */
void traffic_car_start_convoy(void)
{
	uint16_t spacing = 0;
	uint32_t follower_position = 0, leader = 0, line = 0, object = 0;

	line = OBJECT(current_object)->traffic.record;
	spacing = (uint16_t)TRAFFIC_LINE_RECORD(line)->spacing;
	leader = current_object;
	line += 4u;
	for (;;)
	{
		r12 = TRAFFIC_LINE_FOLLOWERS(line)[0];
		line += 4u;
		if (r12 == 0u)
			break;
		object = start_object();
		follower_position = (uint16_t)OBJECT(leader)->position;
		follower_position = (uint16_t)(follower_position - spacing);
		OBJECT(object)->position = follower_position;
		OBJECT(object)->handler = traffic_follower_update_ADDRESS;
		OBJECT(object)->traffic_follower.stop_distance = OBJECT(leader)->traffic.stop_distance;
		OBJECT(object)->traffic_follower.record = r12;
		OBJECT(object)->traffic_follower.leader = leader;
		leader = object;
		SCREEN(screen)->traffic_count++;
	}
	OBJECT(current_object)->traffic.record = line;
	OBJECT(current_object)->handler = traffic_car_update_ADDRESS;
	traffic_car_update(follower_position);
}

/* A traffic car, drawn from the angle the player sees it at. By state: 0 traffic_car_start; 1 moves
   along the road by speed (128ths of a segment a frame, position_fraction carrying into position), then
   traffic_car_draw; once the screen's car has reached the goal (car at_goal) it ends when it is 8
   segments behind the view. */
void traffic_car_update(uint32_t follower_position)
{
	uint32_t position_fraction = 0, trailing_position = 0, whole_segment = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	trailing_position = traffic_car_state_jumps[state];
	switch (state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	default:
		invalid_jump(traffic_car_state_jumps[state]);
	}
	if (CAR(player_car)->at_goal == 0u)
	{
		position_fraction = (trailing_position & ~0xFFFFu) | OBJECT(current_object)->position_fraction;
		position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction + OBJECT(current_object)->traffic.speed);
		whole_segment = (follower_position & ~0xFFFFu) | 0x80u;
		if ((int16_t)whole_segment <= (int16_t)position_fraction)
		{
			do
			{
				OBJECT(current_object)->position++;
				position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction - whole_segment);
			} while ((int16_t)whole_segment <= (int16_t)position_fraction);
		}
		traffic_car_draw(position_fraction, whole_segment);
		return;
	}
	trailing_position = (trailing_position & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->position;
	trailing_position = (trailing_position & ~0xFFFFu) | (uint16_t)(trailing_position - 8u);
	if ((int16_t)trailing_position < (int16_t)RACE(SCREEN(screen)->race_copy)->window_segment)
	{
		SCREEN(screen)->traffic_count--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	position_fraction = (trailing_position & ~0xFFFFu) | OBJECT(current_object)->position_fraction;
	position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction + OBJECT(current_object)->traffic.speed);
	whole_segment = (follower_position & ~0xFFFFu) | 0x80u;
	if ((int16_t)whole_segment <= (int16_t)position_fraction)
	{
		do
		{
			OBJECT(current_object)->position++;
			position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction - whole_segment);
		} while ((int16_t)whole_segment <= (int16_t)position_fraction);
	}
	traffic_car_draw(position_fraction, whole_segment);
}

/* Sets up a traffic car from its record (the address of its sprite record, then side, x_offset and
   speed): from the sprite record its animation (animation_start), palette, passing_sound, hit_sound,
   weight and hit_box (the sprite record's last 8 bytes); draw_mode B (the frame chosen by the angle it
   is seen from, traffic_car_draw), scale_shift FF, passed 0; goes on to state 1. */
void traffic_car_start(void)
{
	uint32_t record = 0, start_animation = 0, traffic_sprite_record = 0;
	uint8_t passing_sound = 0, hit_sound = 0;
	uint16_t animation = 0, palette = 0, weight = 0;

	record = OBJECT(current_object)->traffic.record;
	traffic_sprite_record = TRAFFIC_RECORD(record)->sprite;
	animation = TRAFFIC_SPRITE_RECORD(traffic_sprite_record)->animation;
	traffic_sprite_record += 2u;
	start_animation = animation;
	OBJECT(current_object)->draw_mode = 0xBu;
	palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
	traffic_sprite_record += 2u;
	OBJECT(current_object)->palette = palette;
	OBJECT(current_object)->scale_shift = 0xFFu;
	passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
	traffic_sprite_record++;
	OBJECT(current_object)->traffic.passing_sound = passing_sound;
	hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
	traffic_sprite_record++;
	OBJECT(current_object)->traffic.hit_sound = hit_sound;
	weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
	OBJECT(current_object)->traffic.weight = weight;
	OBJECT(current_object)->traffic.hit_box = traffic_sprite_record + 2u;
	record += 4u;
	OBJECT(current_object)->side = (uint8_t)TRAFFIC_RECORD(record - 0x4u)->side;
	record += 2u;
	AS32(OBJECT(current_object)->traffic.x_offset) = TRAFFIC_RECORD(record - 0x6u)->x_offset;
	OBJECT(current_object)->traffic.speed = TRAFFIC_RECORD(record - 0x6u)->speed;
	OBJECT(current_object)->traffic.passed = 0u;
	OBJECT(current_object)->state++;
	animation_start(start_animation);
}

/* Moves and draws a traffic car: keeps the position_fraction; ends it (taking it off the count, screen
   traffic_count) when traffic_out_of_view; plays passing_sound whenever the screen's car (race_copy
   segment) passes it or it passes the car (passed); while it is within the draw distance ahead of the
   view (race_copy window_segment) picks its frame by the angle the player sees it from: a column (4-1B)
   from the difference between the road's direction (+0 of the road's segment record) at the car and at
   it, plus a quarter of its screen_x, and a row (0-3, 20 frames apart) from the difference in height
   (+2) and the car's pitch (car view_pitch), shaken for 20 frames after a hit (shake,
   traffic_shake_frame_offsets); traffic_hit_check, and on a hit traffic_start_hit_sound and shake = 1;
   draw_object. */
void traffic_car_draw(uint32_t position_fraction, uint32_t whole_segment)
{
	uint32_t zero = 0, negative = 0, carry = 0;
	uint8_t passing_sound = 0, row = 0;
	uint16_t height_difference = 0, object_height = 0, screen_x_quarter = 0, segment = 0, window_segment = 0;
	uint32_t car_segment_record = 0, frame = 0, position = 0, race = 0, seen_from = 0, segment_record = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t raw_row = 0;

	OBJECT(current_object)->position_fraction = position_fraction;
	position = (position_fraction & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->position;
	zero = traffic_out_of_view((uint16_t)position);
	if (zero)
	{
		SCREEN(screen)->traffic_count--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	race = SCREEN(screen)->race_copy;
	seen_from = (whole_segment & ~0xFFFFu) | (uint16_t)position;
	seen_from = (seen_from & ~0xFFFFu) | (uint16_t)(seen_from - SCREEN(screen)->traffic_draw_distance);
	segment = RACE(race)->segment;
	if (OBJECT(current_object)->traffic.passed != 0u)
	{
		if ((int16_t)segment < (int16_t)position)
		{
			OBJECT(current_object)->traffic.passed = ~OBJECT(current_object)->traffic.passed;
			stack_4 = position;
			stack_8 = seen_from;
			passing_sound = OBJECT(current_object)->traffic.passing_sound;
			request_sound(passing_sound);
			seen_from = stack_8;
			position = stack_4;
		}
	}
	else if ((int16_t)position < (int16_t)segment)
	{
		OBJECT(current_object)->traffic.passed = ~OBJECT(current_object)->traffic.passed;
		stack_4 = position;
		stack_8 = seen_from;
		passing_sound = OBJECT(current_object)->traffic.passing_sound;
		request_sound(passing_sound);
		seen_from = stack_8;
		position = stack_4;
	}
	window_segment = RACE(race)->window_segment;
	if ((int16_t)position < (int16_t)window_segment || (int16_t)window_segment <= (int16_t)seen_from)
		return;
	car_segment_record = screen + 0x2000u + CAR(player_car)->segment_offset;
	frame = mem_read16(car_segment_record);
	height_difference = mem_read16(car_segment_record + 2u);
	segment_record = screen + 0x2000u + (((uint32_t)(uint16_t)OBJECT(current_object)->position << 5) & 0xFFFu);
	object_height = mem_read16(segment_record + 2u);
	frame = (uint16_t)((int16_t)((frame << 4) - ((uint32_t)mem_read16(segment_record) << 4)) >> 6);
	screen_x_quarter = (uint16_t)((int16_t)OBJECT(current_object)->traffic.screen_x >> 2);
	carry = (uint16_t)(frame + screen_x_quarter) < frame;
	frame = (uint16_t)(frame + screen_x_quarter);
	frame = (frame & ~0xFFu) | (uint8_t)(frame + 0x10u + carry);
	if ((int8_t)frame >= 4)
	{
		if ((int8_t)frame > 0x1B)
			frame = (frame & ~0xFFu) | 0x1Bu;
	}
	else
		frame = (frame & ~0xFFu) | 4u;
	height_difference = (uint16_t)(((uint32_t)height_difference << 4) - ((uint32_t)object_height << 4) + ((int16_t)((uint32_t)CAR(player_car)->view_pitch << 4) >> 2));
	carry = (height_difference >> 9) & 1u;
	raw_row = ((int16_t)height_difference >> 10) + carry;
	row = raw_row;
	if ((int8_t)raw_row >= 0)
	{
		if ((int8_t)row > 3)
			row = 3u;
	}
	else
		row = 0u;
	frame = (frame & ~0xFFu) | (uint8_t)(frame + ((uint32_t)row << 5));
	if (OBJECT(current_object)->traffic.shake != 0u)
	{
		frame = (frame & ~0xFFu) | (uint8_t)(frame + traffic_shake_frame_offsets[OBJECT(current_object)->traffic.shake]);
		OBJECT(current_object)->traffic.shake++;
		if (OBJECT(current_object)->traffic.shake == 0x15u)
			OBJECT(current_object)->traffic.shake = 0u;
	}
	OBJECT(current_object)->frame = frame;
	negative = traffic_hit_check();
	if (negative)
	{
		traffic_start_hit_sound();
		OBJECT(current_object)->traffic.shake = 1u;
	}
	frame = draw_object(frame);
}

/* A vehicle in a line of road traffic behind a 0138EB leader (traffic_car_start_convoy starts the line
   from the leader's list: each one the list's spacing behind the one before it, with its own record):
   moves and draws as the leader's family does (traffic_car_update), but lives exactly as long as the one
   before it (leader): when that one has ended it ends too (screen traffic_count, the count of the
   traffic, less 1). The only line in the road's lists is in list 25 of object_lists_2E1F0 (a leader of
   animation 35C and eight of 35D); the scenarios start it only for a frame or two. */
void traffic_follower_update(void)
{
	if (OBJECT(OBJECT(current_object)->traffic_follower.leader)->flags == 0u)
	{
		SCREEN(screen)->traffic_count--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	traffic_car_update(0u);
	if (OBJECT(current_object)->flags == 0u)
	{
		OBJECT(current_object)->flags = 1u;
		SCREEN(screen)->traffic_count++;
	}
}

/* The vehicle a road animal pulls (traffic_figure_start_team starts it behind a 013BA3 leader from the
   leader's list): the covered wagons behind the horses (list 1 of object_lists_2E1F0), the sleighs
   behind the reindeer (lists 25 and 26). Moves and draws as the 013AF9 traffic does
   (traffic_stopping_car_update), and lives exactly as long as the one before it (leader): when that one
   has ended it ends too (screen traffic_count less 1). */
void pulled_vehicle_update(void)
{
	if (OBJECT(OBJECT(current_object)->pulled_vehicle.leader)->flags == 0u)
	{
		SCREEN(screen)->traffic_count--;
		OBJECT(current_object)->flags = 0u;
		return;
	}
	traffic_stopping_car_update();
	if (OBJECT(current_object)->flags == 0u)
	{
		OBJECT(current_object)->flags = 1u;
		SCREEN(screen)->traffic_count++;
	}
}

/* A traffic car that slows to a stop: as traffic_car_update, but at each segment it counts stop_distance
   down and, once it has run out (stopping), its speed drops by one a segment to 0. The cars of stages 2
   and 12, and what the traffic_figure teams pull (013AD8 runs it). */
void traffic_stopping_car_update(void)
{
	uint32_t position_fraction = 0, trailing_position = 0, whole_segment = 0;
	uint16_t stop_distance = 0, speed = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	trailing_position = traffic_stopping_car_state_jumps[state];
	switch (state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	default:
		invalid_jump(traffic_stopping_car_state_jumps[state]);
	}
	if (CAR(player_car)->at_goal != 0u)
	{
		trailing_position = (trailing_position & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->position;
		trailing_position = (trailing_position & ~0xFFFFu) | (uint16_t)(trailing_position - 8u);
		if ((int16_t)trailing_position < (int16_t)RACE(SCREEN(screen)->race_copy)->window_segment)
		{
			SCREEN(screen)->traffic_count--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
	}
	position_fraction = (trailing_position & ~0xFFFFu) | OBJECT(current_object)->position_fraction;
	position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction + OBJECT(current_object)->traffic.speed);
	whole_segment = 0x80u;
	if ((int16_t)position_fraction < (int16_t)whole_segment)
	{
		traffic_car_draw(position_fraction, whole_segment);
		return;
	}
	OBJECT(current_object)->position++;
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
		position_fraction = (position_fraction & ~0xFFFFu) | (uint16_t)(position_fraction - whole_segment);
		if ((int16_t)position_fraction < (int16_t)whole_segment)
			break;
		OBJECT(current_object)->position++;
	}
	traffic_car_draw(position_fraction, whole_segment);
}

/* The leader of a team (the horses pulling a covered wagon in stage 1, the reindeer pulling a sleigh in
   stages 25 and 26): its record starts with the distance and the records of what it pulls, closed by 0;
   starts each (handler 013AD8: traffic_stopping_car_update, kept live as long as the one before it is,
   which ends it) that distance behind the one before (its field 14 the one before, the same
   stop_distance, counted in screen traffic_count), then becomes a traffic_figure with the record after
   the list. */
void traffic_figure_start_team(void)
{
	uint16_t spacing = 0;
	uint32_t leader = 0, line = 0, object = 0;

	line = OBJECT(current_object)->traffic.record;
	spacing = (uint16_t)TRAFFIC_LINE_RECORD(line)->spacing;
	leader = current_object;
	line += 4u;
	for (;;)
	{
		r12 = TRAFFIC_LINE_FOLLOWERS(line)[0];
		line += 4u;
		if (r12 == 0u)
			break;
		object = start_object();
		OBJECT(object)->position = OBJECT(leader)->position - spacing;
		OBJECT(object)->handler = pulled_vehicle_update_ADDRESS;
		OBJECT(object)->pulled_vehicle.stop_distance = OBJECT(leader)->traffic.stop_distance;
		OBJECT(object)->pulled_vehicle.record = r12;
		OBJECT(object)->pulled_vehicle.leader = leader;
		leader = object;
		SCREEN(screen)->traffic_count++;
	}
	OBJECT(current_object)->traffic.record = line;
	OBJECT(current_object)->handler = traffic_figure_update_ADDRESS;
	traffic_figure_update();
}

/* Traffic drawn from a single view (draw_mode 5) instead of by angle: the horses of the covered wagons
   (stage 1), the reindeer (stages 25, 26) and the figures of stages 10 and 24 (animation 185) and 30
   (258, 071). By state: 0 reads its record as traffic_car_start does (draw_mode 4, scale_shift 2 and
   y_offset -2C0 for animation 165; scale_shift 0 for 258 and 185, else 1); 1 once the screen's car has
   reached the goal (car at_goal) ends when 8 segments behind the view; moves along the road by speed,
   slowing to a stop once stop_distance has run out (stopping); ends when traffic_out_of_view; while
   within the draw distance ahead of the view: traffic_hit_check when it has a hit box
   (traffic_start_hit_sound on a hit), draw_object and animation_advance. */
void traffic_figure_update(void)
{
	uint32_t zero = 0, negative = 0;
	uint16_t position_fraction = 0, seen_from = 0, whole_segment = 0;
	uint32_t animation = 0, ignored = 0, position = 0, race = 0, record = 0, traffic_sprite_record = 0;
	uint16_t stop_distance = 0, speed = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		record = OBJECT(current_object)->traffic.record;
		traffic_sprite_record = TRAFFIC_RECORD(record)->sprite;
		animation = TRAFFIC_SPRITE_RECORD(traffic_sprite_record)->animation;
		record += 4u;
		traffic_sprite_record += 2u;
		if (animation != 0x165u)
		{
			if (animation == 0x258u || animation == 0x185u)
				OBJECT(current_object)->scale_shift = 0u;
			else
				OBJECT(current_object)->scale_shift = 1u;
			OBJECT(current_object)->palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
			OBJECT(current_object)->draw_mode = 5u;
			traffic_sprite_record += 2u;
			OBJECT(current_object)->traffic.passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
			traffic_sprite_record++;
			OBJECT(current_object)->traffic.hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
			traffic_sprite_record++;
			OBJECT(current_object)->traffic.weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
			OBJECT(current_object)->traffic.hit_box = traffic_sprite_record + 2u;
			OBJECT(current_object)->side = (uint8_t)TRAFFIC_RECORD(record - 0x4u)->side;
			record += 2u;
			AS32(OBJECT(current_object)->traffic.x_offset) = TRAFFIC_RECORD(record - 0x6u)->x_offset;
			OBJECT(current_object)->traffic.speed = TRAFFIC_RECORD(record - 0x6u)->speed;
			OBJECT(current_object)->state++;
			animation_start(animation);
			return;
		}
		OBJECT(current_object)->scale_shift = 2u;
		OBJECT(current_object)->palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
		OBJECT(current_object)->draw_mode = 4u;
		traffic_sprite_record += 2u;
		OBJECT(current_object)->traffic.passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
		traffic_sprite_record++;
		OBJECT(current_object)->traffic.hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
		traffic_sprite_record++;
		OBJECT(current_object)->traffic.weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
		OBJECT(current_object)->traffic.hit_box = traffic_sprite_record + 2u;
		OBJECT(current_object)->side = (uint8_t)TRAFFIC_RECORD(record - 0x4u)->side;
		record += 2u;
		AS32(OBJECT(current_object)->traffic.x_offset) = TRAFFIC_RECORD(record - 0x6u)->x_offset;
		OBJECT(current_object)->traffic.speed = TRAFFIC_RECORD(record - 0x6u)->speed;
		AS32(OBJECT(current_object)->traffic.y_offset) = (int16_t)0xFD40u;
		OBJECT(current_object)->state++;
		animation_start(animation);
		return;
	case 1:
		if (CAR(player_car)->at_goal != 0u && (int16_t)(OBJECT(current_object)->position - 8u) < (int16_t)RACE(SCREEN(screen)->race_copy)->window_segment)
		{
			SCREEN(screen)->traffic_count--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
		position_fraction = (uint16_t)(OBJECT(current_object)->position_fraction + OBJECT(current_object)->traffic.speed);
		whole_segment = 0x80u;
		if ((int16_t)position_fraction >= 0x80)
		{
			do
			{
				OBJECT(current_object)->position++;
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
				position_fraction -= whole_segment;
			} while ((int16_t)whole_segment <= (int16_t)position_fraction);
		}
		OBJECT(current_object)->position_fraction = position_fraction;
		position = (uint16_t)OBJECT(current_object)->position;
		zero = traffic_out_of_view((uint16_t)position);
		if (zero)
		{
			SCREEN(screen)->traffic_count--;
			OBJECT(current_object)->flags = 0u;
			return;
		}
		race = SCREEN(screen)->race_copy;
		seen_from = (uint16_t)(position - SCREEN(screen)->traffic_draw_distance);
		if ((int16_t)position < (int16_t)RACE(race)->window_segment || (int16_t)RACE(race)->window_segment <= (int16_t)seen_from)
			return;
		if ((ITEM_HIT_BOX(OBJECT(current_object)->traffic.hit_box)->along_start | ITEM_HIT_BOX(OBJECT(current_object)->traffic.hit_box)->along_end) != 0u)
		{
			negative = traffic_hit_check();
			if (negative)
				traffic_start_hit_sound();
		}
		draw_object(position);
		ignored = animation_advance(0u);
		return;
	default:
		invalid_jump(traffic_figure_state_jumps[OBJECT(current_object)->state]);
	}
}

/* When the traffic object has a hit_sound, starts an object (013D15) that plays it 15 frames later. */
void traffic_start_hit_sound(void)
{
	uint32_t object = 0;

	if (OBJECT(current_object)->traffic.hit_sound != 0u)
	{
		object = start_object();
		OBJECT(object)->handler = traffic_hit_sound_wait_ADDRESS;
		OBJECT(object)->traffic_hit_sound.delay = 0xFu;
		OBJECT(object)->traffic_hit_sound.sound = OBJECT(current_object)->traffic.hit_sound;
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
uint32_t traffic_out_of_view(uint16_t position)
{
	uint32_t zero = 0;
	uint32_t stretch = 0, stretches_left = 0;
	uint16_t stretch_top = 0, stretch_bottom = 0;

	stretch = screen + 0xF84u;
	stretches_left = SCREEN(stretch - 0xF84u)->visible_stretch_count;
	stretch += 2u;
	do
	{
		stretch_top = mem_read16(stretch);
		stretch += 2u;
		if (stretch_top < (uint16_t)position)
			break;
		stretch_bottom = mem_read16(stretch);
		stretch += 2u;
		if (stretch_bottom < (uint16_t)position)
		{
			zero = 1u == 0u;
			return zero;
		}
		stretches_left--;
	} while (stretches_left != 0u);
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
   road's screen x plus a curve growing with the distance from start_position (cosine_table) times the
   road's scale times direction, the pieces between segments filled in, into the screen's sprite list and
   its segment's depth list, until the strip leaves the screen. Ends once the view (race_copy
   window_segment) passes end_position. */
void fork_branch_draw(void)
{
	uint16_t end_position = 0, position = 0, row_y = 0, start_position = 0, window_segment = 0;
	uint32_t branch_distance = 0, depth_layer = 0, first_sprite = 0, height_word = 0, last_scale = 0, last_x = 0, last_y = 0, offset_palette = 0, piece_scale = 0, piece_x = 0, piece_y = 0, row_span = 0, rows_done = 0, rows_left = 0, rows_to_fill = 0, segment = 0, sprite = 0, view_record = 0;
	uint16_t distance_from_start = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		position = (uint16_t)OBJECT(current_object)->position;
		OBJECT(current_object)->fork_branch.start_position = position;
		OBJECT(current_object)->fork_branch.end_position = position + 0x28u;
		return;
	case 1:
		break;
	default:
		invalid_jump(fork_branch_state_jumps[OBJECT(current_object)->state]);
	}
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	start_position = (uint16_t)OBJECT(current_object)->fork_branch.start_position;
	end_position = (uint16_t)OBJECT(current_object)->fork_branch.end_position;
	if (end_position <= window_segment)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	view_record = screen + 0x2FF0u;
	do
	{
		view_record += 0x10u;
	} while (mem_read16(view_record) == 0u);
	if (mem_read16(view_record) == 0u)
		return;
	r27 = view_record;
	r27 = r27 & 0xFFFu;
	r27 = r27 >> 4;
	r27 = (uint16_t)(r27 + window_segment);
	view_record -= 0x10u;
	do
	{
		view_record += 0x10u;
		if (mem_read16(view_record) == 0u)
			return;
		segment = view_record & 0xFFFu;
		segment = segment >> 4;
		segment = (uint16_t)(segment + window_segment);
		distance_from_start = segment - start_position;
		branch_distance = distance_from_start;
	} while ((int16_t)distance_from_start < 0 || (int16_t)mem_read16(view_record + 0x16u) >= 2);
	last_y = (int16_t)mem_read16(view_record + 6u);
	rows_to_fill = 0u;
	if (start_position <= r27)
		rows_to_fill = 2u - last_y;
	last_x = (int16_t)mem_read16(view_record + 4u);
	last_scale = mem_read16(view_record);
	if (last_scale >= 0x1FF8u)
		last_scale = 0x1FF8u;
	branch_distance = (uint16_t)(branch_distance << 3);
	last_x += (uint32_t)((int32_t)(((int32_t)((cosine_table[(uint16_t)(branch_distance + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * last_scale) >> 12) * OBJECT(current_object)->fork_branch.direction;
	fork_branch_sprite(&height_word, &offset_palette);
	for (;;)
	{
		view_record += 0x10u;
		if (mem_read16(view_record) == 0u)
			break;
		piece_y = (int16_t)mem_read16(view_record + 6u);
		if ((int16_t)last_y <= (int16_t)piece_y)
			continue;
		piece_x = (int16_t)mem_read16(view_record + 4u);
		depth_layer = mem_read16(view_record + 8u);
		piece_scale = mem_read16(view_record);
		if (piece_scale >= 0x1FF8u)
			piece_scale = 0x1FF8u;
		segment = view_record & 0xFFFu;
		segment = segment >> 4;
		segment = (uint16_t)(segment + window_segment);
		if (end_position < segment)
			break;
		segment = (uint16_t)(segment - start_position);
		segment = (uint16_t)(segment << 3);
		piece_x += (uint32_t)((int32_t)(((int32_t)((cosine_table[(uint16_t)(segment + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * piece_scale) >> 12) * OBJECT(current_object)->fork_branch.direction;
		if ((int32_t)piece_x > 0x280 || (int32_t)piece_x < -0x280)
			break;
		sprite = SCREEN(screen)->sprite_list_last + 0x10u;
		rows_left = last_y - piece_y;
		rows_to_fill += rows_left;
		if ((int32_t)(rows_to_fill - 1u) < 0)
			continue;
		if (rows_to_fill == 1u)
		{
			if (SCREEN(screen)->sprite_list_limit <= sprite)
				break;
			rows_to_fill = 0u;
			mem_write32(sprite + 0x3000u, 0u);
			mem_write32(sprite, r12);
			sprite += 4u;
			mem_write16(sprite, height_word);
			sprite += 2u;
			mem_write16(sprite, ((last_scale * 0x370u) >> 12) | r14);
			sprite += 2u;
			mem_write16(sprite, piece_y);
			sprite += 2u;
			mem_write16(sprite, piece_x);
			sprite += 2u;
			mem_write32(sprite, offset_palette);
			sprite += 4u;
		}
		else
		{
			row_span = rows_left;
			rows_done = 0u;
			row_y = (uint16_t)piece_y;
			do
			{
				if ((int16_t)row_y < 2)
				{
					if (SCREEN(screen)->sprite_list_limit <= sprite)
						return;
					mem_write32(sprite + 0x3000u, 0u);
					mem_write32(sprite, r12);
					sprite += 4u;
					mem_write16(sprite, height_word);
					r27 = piece_scale;
					r27 = r27 * rows_left;
					r27 = r27 + last_scale * rows_done;
					r27 = divide_s32(r27, row_span);
					r27 = r27 * 0x370u;
					r27 = r27 >> 12;
					r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 | r14);
					sprite += 2u;
					mem_write16(sprite, r27);
					sprite += 2u;
					mem_write16(sprite, row_y);
					row_y++;
					r27 = piece_x;
					r27 = r27 * rows_left;
					r27 = r27 + last_x * rows_done;
					r27 = divide_s32(r27, row_span);
					sprite += 2u;
					mem_write16(sprite, r27);
					sprite += 2u;
					mem_write32(sprite, offset_palette);
					sprite += 4u;
				}
				rows_left--;
				rows_to_fill--;
				rows_done++;
			} while (rows_to_fill != 0u);
		}
		last_y = piece_y;
		last_x = piece_x;
		last_scale = piece_scale;
		r27 = screen + 0x5000u + depth_layer * 4u;
		if (mem_read16(r27) == 0u)
			mem_write16(r27 + 2u, sprite);
		first_sprite = SCREEN(screen)->sprite_list_last + 0x10u;
		r28 = mem_read16(r27);
		mem_write32(sprite, r28);
		r28 = (uint16_t)(r28 + 0x300u);
		mem_write32(sprite + 0x3000u, r28);
		SCREEN(screen)->sprite_list_last = sprite;
		mem_write16(r27, (first_sprite >> 4) | 0x8000u);
		SCREEN(screen)->sprite_groups++;
	}
}

/* Builds the sprite words of the branch's road piece for fork_branch_draw: the first frame of animation
   74 (its size and place in the sprite ROM, the size steps at 0x122B74) and the palette of the stage
   being built (fork_branch_stage_palettes by screen built_stage, into sprite_palettes). */
void fork_branch_sprite(uint32_t *p_height_word, uint32_t *p_offset_palette)
{
	uint16_t height_word = 0;
	uint32_t frames = 0, size_step = 0;

	frames = SPRITE_ANIMATION(fork_branch_piece_animation[0])->frames[0];
	r27 = (r27 & ~0xFFFFu) | mem_read16(frames + 2u);
	size_step = sprite_size_steps_ADDRESS + mem_read16(frames + 4u) * 4u;
	r12 = ((uint32_t)SPRITE_SIZE_STEP(size_step)->source_size << 16) | (uint16_t)r27;
	size_step += 2u;
	r27 = (r27 & ~0xFFFFu) | SPRITE_SIZE_STEP(size_step - 0x2u)->rom_offset;
	size_step += 4u;
	height_word = 1u | SPRITE_SIZE_STEP(size_step - 0x6u)->height_bits;
	r14 = SPRITE_SIZE_STEP(size_step - 0x6u)->width_bits;
	r14 = r14 | SCREEN(screen)->sprite_screen_bit;
	*p_offset_palette = ((uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[fork_branch_stage_palettes[SCREEN(screen)->built_stage]])[0] << 16) | (uint16_t)r27;
	*p_height_word = (*p_height_word & 0xFFFF0000u) | height_word;
}

/* One of the two people in a race car (race_car_occupant_animations, race_car_rider_palettes by
   occupant), seen only while the car shows an animation of its own (its special_body: a crash): then
   drawn at the car's place and size until its animation ends, when the second one lets the car go back
   to its body. */
void race_car_occupant_update(void)
{
	uint32_t animation = 0, object = 0, occupant = 0, scale = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0u;
		occupant = OBJECT(current_object)->race_car_occupant.occupant;
		animation = race_car_occupant_animations[occupant];
		OBJECT(current_object)->palette = race_car_rider_palettes[occupant];
		animation_start(animation);
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		if (OBJECT(object)->race_car.special_body == 0u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y = OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
		OBJECT(current_object)->frame = 0u;
		OBJECT(current_object)->frame_time = 1u;
		OBJECT(current_object)->frame_countdown = 1;
		break;
	case 1:
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		if (OBJECT(object)->race_car.special_body == 0u)
			return;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y = OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
		OBJECT(current_object)->frame = 0u;
		OBJECT(current_object)->frame_time = 1u;
		OBJECT(current_object)->frame_countdown = 1;
		break;
	case 2:
		break;
	default:
		invalid_jump(race_car_occupant_state_jumps[OBJECT(current_object)->state]);
	}
	object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
	OBJECT(object)->race_car.spray_2_frame = 0u;
	OBJECT(object)->race_car.spray_1_frame = 0u;
	if (OBJECT(object)->race_car.special_body == 0u)
	{
		animation_advance(0u);
		if (OBJECT(current_object)->frame_time == 0u)
		{
			object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
			OBJECT(current_object)->state--;
			OBJECT(object)->race_car.special_body = OBJECT(current_object)->race_car_occupant.body_back;
			return;
		}
		return;
	}
	OBJECT(current_object)->depth_layer = OBJECT(object)->depth_layer;
	scale = OBJECT(object)->width_scale;
	if (scale > 0x100u)
		scale = 0x100u;
	OBJECT(current_object)->width_scale = scale;
	OBJECT(current_object)->height_scale = scale;
	OBJECT(current_object)->x = OBJECT(object)->x + SCREEN(screen)->sprite_origin_x;
	if (OBJECT(object)->race_car.rival != 0u)
		OBJECT(current_object)->y = ((int32_t)(RACE(OBJECT(object)->race_car.record)->bounce_height * scale) >> 8) + OBJECT(object)->y + SCREEN(screen)->sprite_origin_y;
	draw_object(scale);
	animation_advance(0u);
	if (OBJECT(current_object)->frame_time == 0u)
	{
		object = LINKED(screen, OBJECT(current_object)->race_car_occupant.car);
		OBJECT(current_object)->state--;
		OBJECT(object)->race_car.special_body = OBJECT(current_object)->race_car_occupant.body_back;
		return;
	}
}

/* A rider's gesture on another racer's car (00974C; race_cars_start starts one for its driver, rider =
   model, and one for its passenger, model + 8; only some have a gesture, rider_gesture_animations, the
   others end at once): when the car moves up a place in the race (rider_gesture_check_place) it plays
   its gesture once (the driver raises an arm, a finger up) in place of the rider, and while the car can
   be seen from the angle and the screen's car is racing (rider_gesture_check_view) it is drawn on the
   car (rider_gesture_place) at the car's size and a layer in front; then waits for the next time. */
void rider_gesture_update(void)
{
	uint32_t zero = 0;
	uint8_t visible_mask = 0;
	uint32_t animation = 0, object = 0, rider = 0, scale = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0u;
		rider = OBJECT(current_object)->rider_gesture.rider;
		animation = rider_gesture_animations[rider];
		if (animation == 0u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->palette = race_car_rider_palettes[rider];
		animation_start(animation);
		object = LINKED(screen, OBJECT(current_object)->rider_gesture.car);
		visible_mask = 0xFFu;
		zero = rider_gesture_check_place(visible_mask, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->frame_time = 1u;
			OBJECT(current_object)->frame_countdown = 1;
			OBJECT(object)->race_car.gesture_shown = 0xFFu;
		}
		return;
	case 1:
		object = LINKED(screen, OBJECT(current_object)->rider_gesture.car);
		visible_mask = 0xFFu;
		zero = rider_gesture_check_place(visible_mask, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->frame_time = 1u;
			OBJECT(current_object)->frame_countdown = 1;
			OBJECT(object)->race_car.gesture_shown = 0xFFu;
		}
		return;
	case 2:
		object = LINKED(screen, OBJECT(current_object)->rider_gesture.car);
		zero = rider_gesture_check_view(object);
		if (zero)
		{
			animation_advance(0u);
			if (OBJECT(current_object)->frame_time == 0u)
			{
				object = LINKED(screen, OBJECT(current_object)->rider_gesture.car);
				OBJECT(current_object)->state--;
				OBJECT(object)->race_car.gesture_shown = 0u;
				return;
			}
			return;
		}
		OBJECT(current_object)->depth_layer = OBJECT(object)->depth_layer;
		OBJECT(current_object)->depth_layer++;
		scale = OBJECT(object)->width_scale;
		if (scale > 0x100u)
			scale = 0x100u;
		OBJECT(current_object)->width_scale = scale;
		OBJECT(current_object)->height_scale = scale;
		rider_gesture_place((uint16_t)scale, object);
		draw_object(scale);
		animation_advance(0u);
		if (OBJECT(current_object)->frame_time == 0u)
		{
			object = LINKED(screen, OBJECT(current_object)->rider_gesture.car);
			OBJECT(current_object)->state--;
			OBJECT(object)->race_car.gesture_shown = 0u;
			return;
		}
		return;
	default:
		invalid_jump(rider_gesture_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Whether the gesture shows (zero clear): only while the screen's car object is in state 2 or 4
   (rider_gesture_car_states) and the car is seen from one of the angles that show it
   (rider_gesture_views by the car's view); otherwise clears the car's gesture_shown; then
   rider_gesture_check_place. */
uint32_t rider_gesture_check_view(uint32_t car)
{
	uint32_t zero = 0;
	uint32_t visible_mask = 0;
	uint8_t gesture_shown = 0;

	visible_mask = rider_gesture_car_states[OBJECT(CAR(player_car)->object)->state];
	visible_mask = visible_mask & rider_gesture_views[OBJECT(car)->race_car.view];
	if (visible_mask == 0u)
	{
		gesture_shown = OBJECT(car)->race_car.gesture_shown & visible_mask;
		zero = gesture_shown == 0u;
		OBJECT(car)->race_car.gesture_shown = gesture_shown;
		return zero;
	}
	return rider_gesture_check_place((uint8_t)visible_mask, car);
}

/* Notes the racer's place in the race (screen + 320 + player) in its car's last_place and sets its
   gesture_shown when it is better than the last one; returns gesture_shown masked with R9 (zero set when
   there is no gesture to show). */
uint32_t rider_gesture_check_place(uint8_t visible_mask, uint32_t car)
{
	uint32_t zero = 0;
	uint8_t last_place = 0, place = 0;
	uint8_t gesture_shown = 0;

	place = mem_read8(screen + 0x320u + OBJECT(current_object)->rider_gesture.player);
	last_place = OBJECT(car)->race_car.last_place;
	OBJECT(car)->race_car.last_place = place;
	if (place < last_place)
		OBJECT(car)->race_car.gesture_shown = 0xFFu;
	gesture_shown = OBJECT(car)->race_car.gesture_shown & visible_mask;
	zero = gesture_shown == 0u;
	OBJECT(car)->race_car.gesture_shown = gesture_shown;
	return zero;
}

/* Places the gesture on its car: the car's x and y (plus the screen's scroll, screen sprite_origin_x and
   sprite_origin_y) and the rider's offset, rider_gesture_x_offsets and rider_gesture_y_offsets by rider
   plus the seat's offset for the car's view (race_car_rider_places by model, by the car's view, the
   driver's or the passenger's), scaled by R0 (the car's width_scale). */
void rider_gesture_place(uint16_t scale, uint32_t car)
{
	uint16_t x_offset = 0, y = 0, y_offset = 0;
	uint32_t model = 0, rider = 0, seat_place = 0, view = 0;

	rider = OBJECT(current_object)->rider_gesture.rider;
	x_offset = rider_gesture_x_offsets[rider];
	y_offset = rider_gesture_y_offsets[rider];
	model = OBJECT(current_object)->rider_gesture.model;
	view = OBJECT(car)->race_car.view;
	rider = (uint8_t)(rider - model);
	seat_place = race_car_rider_places[model] + (uint16_t)(view << 4) + (rider >> 1);
	y = (uint16_t)(OBJECT(car)->y + SCREEN(screen)->sprite_origin_y + ((int16_t)((uint32_t)(y_offset + mem_read16(seat_place + 2u)) * scale) >> 8));
	OBJECT(current_object)->x = OBJECT(car)->x + SCREEN(screen)->sprite_origin_x + ((int16_t)((uint32_t)(x_offset + mem_read16(seat_place)) * scale) >> 8);
	OBJECT(current_object)->y = y;
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
		OBJECT(current_object)->flags_high = 0u;
		return;
	default:
		invalid_jump(race_opening_state_jumps[OBJECT(current_object)->state]);
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
		invalid_jump(race_opening_sound_step_jumps[OBJECT(current_object)->race_opening.sound_step]);
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
		invalid_jump(race_opening_camera_step_jumps[OBJECT(current_object)->race_opening.camera_step]);
	}
}

/* Starts the opening's sounds: the first at count 1, the rest of race_opening_sound_cues. */
void race_opening_sounds_start(void)
{
	OBJECT(current_object)->race_opening.sound_step++;
	OBJECT(current_object)->race_opening.next_count = race_opening_sound_cues[0].time;
	OBJECT(current_object)->race_opening.sound_list = race_opening_sound_cues_ADDRESS + 0x1u;
}

/* Counts the opening's steps (count) and requests each sound of the list at its count (sound_list:
   sounds 2D, 24, 0A, 21, 2E, 2D, 24 at the counts 1, 20, 65, 120, 150, 160, 190); after the last goes on
   to race_opening_sounds_done. */
void race_opening_sounds_play(void)
{
	uint8_t sound = 0;
	uint32_t sound_cue = 0;

	OBJECT(current_object)->race_opening.count++;
	if (OBJECT(current_object)->race_opening.count < OBJECT(current_object)->race_opening.next_count)
	{
		race_opening_sounds_done();
		return;
	}
	sound_cue = OBJECT(current_object)->race_opening.sound_list;
	sound = SOUND_CUE(sound_cue - 0x1u)->sound;
	sound_cue++;
	request_sound(sound);
	if (SOUND_CUE(sound_cue)->time != 0u)
	{
		OBJECT(current_object)->race_opening.next_count = SOUND_CUE(sound_cue)->time;
		OBJECT(current_object)->race_opening.sound_list = sound_cue + 1u;
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
   when the race record's stage count (race_copy stage_count) changes (the checkpoint or the goal
   passed), sound 24 if this screen's player leads the race (screen player = unk_70), else 28, and it
   ends. */
void stage_end_sounds_update(void)
{
	uint8_t end_sound = 0, sound = 0;
	uint32_t sound_cue = 0;
	uint8_t countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->stage_end_sounds.stage = RACE(SCREEN(screen)->race_copy)->stage_count;
		sound_cue = stage_end_sound_cues_ADDRESS;
		OBJECT(current_object)->stage_end_sounds.countdown = SOUND_CUE(sound_cue)->time;
		OBJECT(current_object)->stage_end_sounds.sound_list = sound_cue + 1u;
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
		sound_cue = OBJECT(current_object)->stage_end_sounds.sound_list;
		sound = SOUND_CUE(sound_cue - 0x1u)->sound;
		sound_cue++;
		request_sound(sound);
		if (SOUND_CUE(sound_cue)->time != 0u)
		{
			OBJECT(current_object)->stage_end_sounds.countdown = SOUND_CUE(sound_cue)->time;
			OBJECT(current_object)->stage_end_sounds.sound_list = sound_cue + 1u;
			return;
		}
		OBJECT(current_object)->state++;
		return;
	case 2:
		if (OBJECT(current_object)->stage_end_sounds.stage == RACE(SCREEN(screen)->race_copy)->stage_count)
			return;
		end_sound = 0x24u;
		if (SCREEN(screen)->player != SCREEN(screen)->leader_player)
			end_sound = 0x28u;
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		request_sound(end_sound);
		return;
	default:
		invalid_jump(stage_end_sounds_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Raises and tilts the view before each fork (256 segments before it), so the fork is seen from above.
   By state: 0 waits until the car (race_copy segment) reaches its position; 1 each frame the view's
   pitch lift (car view_pitch_lift) up by lift_step to lift_height (1E0 less the car model's
   view_pitch_base, screen_view_defaults view_pitch_bases by race_copy model; screen nearing_fork = FF
   when it gets there: the radio's checkpoint message) and its shift lift (car view_shift_lift) down by
   tilt_step; 2 holds until the car reaches lower_position (224 segments on); 3 lowers it back the same
   way and ends. Does nothing while car crash_body_shown is set. */
void fork_camera_update(void)
{
	uint16_t next_pitch_lift = 0;
	uint32_t model = 0;
	uint16_t lift_step = 0, view_pitch_lift = 0;

	if (CAR(player_car)->crash_body_shown != 0u)
		return;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (RACE(SCREEN(screen)->race_copy)->segment < (uint16_t)OBJECT(current_object)->position)
			return;
		OBJECT(current_object)->state++;
		model = RACE(SCREEN(screen)->race_copy)->model;
		OBJECT(current_object)->fork_camera.lift_height = 0u - screen_view_defaults[0].view_pitch_bases[model] + 0x1E0u;
		OBJECT(current_object)->fork_camera.lift_step = fork_camera_lift_steps[model];
		OBJECT(current_object)->fork_camera.tilt_step = fork_camera_tilt_steps[model];
		OBJECT(current_object)->fork_camera.lower_position = OBJECT(current_object)->position + 0xE0u;
		CAR(player_car)->view_pitch_lift = 0u;
		CAR(player_car)->view_shift_lift = 0u;
		return;
	case 1:
		next_pitch_lift = (uint16_t)(CAR(player_car)->view_pitch_lift + OBJECT(current_object)->fork_camera.lift_step);
		if ((int16_t)OBJECT(current_object)->fork_camera.lift_height <= (int16_t)next_pitch_lift)
		{
			OBJECT(current_object)->state++;
			next_pitch_lift = OBJECT(current_object)->fork_camera.lift_height;
			SCREEN(screen)->nearing_fork = 0xFFu;
		}
		CAR(player_car)->view_pitch_lift = next_pitch_lift;
		CAR(player_car)->view_shift_lift -= OBJECT(current_object)->fork_camera.tilt_step;
		return;
	case 2:
		if (RACE(SCREEN(screen)->race_copy)->segment < OBJECT(current_object)->fork_camera.lower_position)
			return;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	default:
		invalid_jump(fork_camera_state_jumps[OBJECT(current_object)->state]);
	}
	lift_step = OBJECT(current_object)->fork_camera.lift_step;
	view_pitch_lift = CAR(player_car)->view_pitch_lift;
	next_pitch_lift = (uint16_t)(view_pitch_lift - lift_step);
	if ((int16_t)lift_step < (int16_t)view_pitch_lift)
	{
		CAR(player_car)->view_pitch_lift = next_pitch_lift;
		CAR(player_car)->view_shift_lift += OBJECT(current_object)->fork_camera.tilt_step;
		return;
	}
	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->flags_high = 0u;
	CAR(player_car)->view_pitch_lift = 0u;
	CAR(player_car)->view_shift_lift = 0u;
}

/* The end of the race for the screen's car: starts the objects of its ending (object_lists_14A20 by its
   model and whether it won, or object_list_14A60 when it did not finish); when they are done (state 2)
   sound 4F and raises screen flags bit 1, which game step 7 waits for ( the music stopped without a
   finish). */
void race_car_start_ending(void)
{
	uint8_t sound = 0;
	uint32_t actor = 0, actor_count = 0, actor_list = 0, ending = 0, player = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		ending = OBJECT(current_object)->race_car.model;
		ending = (uint8_t)(ending * 2u);
		OBJECT(current_object)->x += 0xA0u;
		OBJECT(current_object)->y += 0xDEu;
		player = SCREEN(screen)->player;
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
		{
			actor_count = object_list_14A60;
			actor_list = unfinished_ending_actors_ADDRESS;
		}
		else
		{
			if (mem_read8(screen + 0x320u + player) != 0u)
				ending = (uint8_t)(ending + 1u);
			actor_list = object_lists_14A20[ending];
			actor_count = mem_read32(actor_list);
			actor_list += 4u;
		}
		do
		{
			actor = start_object();
			OBJECT(actor)->handler = mem_read32(actor_list);
			actor_list += 4u;
			OBJECT(actor)->goal_actor.script = mem_read32(actor_list);
			OBJECT(actor)->goal_actor.scene = current_object;
			actor_list += 4u;
			actor_count--;
		} while (actor_count != 0u);
		break;
	case 1:
	case 3:
		break;
	case 2:
		OBJECT(current_object)->state++;
		sound = 0x4Fu;
		request_sound(sound);
		SCREEN(screen)->flags |= 2u;
		SCREEN(screen)->flags &= 0xFFFFFFEFu;
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
		{
			fade_out_music();
			return;
		}
		break;
	default:
		invalid_jump(race_car_ending_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The player's car in the goal scene: its animation, palette and frame from the race record (race_copy
   animation, unk_3E, unk_4A), its layer, place and scale from the scene object (scene: the car's drawing
   object, 00974C's, whose handler is 014694 at the goal); then goal_actor_start. */
void goal_actor_car_start(void)
{
	uint32_t animation = 0, object = 0;

	object = OBJECT(current_object)->goal_actor.scene;
	animation = RACE(SCREEN(screen)->race_copy)->animation;
	OBJECT(current_object)->animation = animation;
	animation_start(animation);
	OBJECT(current_object)->palette = RACE(SCREEN(screen)->race_copy)->palette;
	OBJECT(current_object)->frame = RACE(SCREEN(screen)->race_copy)->frame;
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
   race_copy animation, unk_4A) the view it is drawn from (bit 6 of the frame's attributes picks the
   mirrored set) and the seat's offset; from the rider record (R12: a table of offsets by view, the
   animation of the first view, the palette, the frame) the rider's animation, palette and frame; scaled
   by the scene object's scale onto its place, in its layer; then goal_actor_start. */
void goal_actor_rider_place(uint32_t scene)
{
	uint32_t carry = 0;
	uint32_t animation = 0, piece = 0, race = 0, view = 0, view_offsets = 0, width_scale = 0, x_offset = 0, y = 0;

	OBJECT(current_object)->palette = mem_read16(r12 + 6u);
	OBJECT(current_object)->depth_layer = OBJECT(scene)->depth_layer;
	race = SCREEN(screen)->race_copy;
	piece = SPRITE_ANIMATION(sprite_animations[RACE(race)->animation])->frames[RACE(race)->frame] + 2u;
	view = mem_read16(piece);
	piece += 2u;
	view = view & 0x40u;
	view = view >> 6;
	view = (uint16_t)(view * 0x34u);
	view = (uint16_t)(view + (uint16_t)(mem_read16(piece) - OBJECT(scene)->race_car.first_body_piece) / 3u);
	piece += 2u;
	y = mem_read16(piece);
	x_offset = mem_read16(piece + 2u);
	view_offsets = mem_read32(r12) + (view << 4);
	x_offset = (uint16_t)(x_offset + mem_read16(view_offsets));
	y = (uint16_t)(y + mem_read16(view_offsets + 2u));
	animation = mem_read16(r12 + 4u) + view;
	OBJECT(current_object)->animation = animation;
	animation_start(animation);
	OBJECT(current_object)->frame = mem_read8(r12 + 0xAu);
	width_scale = OBJECT(scene)->width_scale;
	OBJECT(current_object)->width_scale = width_scale;
	OBJECT(current_object)->height_scale = width_scale;
	x_offset = (int16_t)x_offset * width_scale;
	y = (int16_t)y * width_scale;
	carry = (x_offset >> 7) & 1u;
	x_offset = (uint16_t)(((int32_t)x_offset >> 8) + carry);
	carry = (y >> 7) & 1u;
	y = (uint16_t)(((int32_t)y >> 8) + carry + OBJECT(scene)->y);
	OBJECT(current_object)->x = x_offset + OBJECT(scene)->x;
	OBJECT(current_object)->y = y;
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
   and the actor is drawn (draw_object); with bit 0 set a command (goal_actor_command_jumps): 0 tells the
   scene object it is over (its state = 2), 1 ends the actor, 2 ends the step undrawn, 3 flips it (flags
   bit 6), 4 car segment_progress + 4000, 5 a repeat whose count grows by 8 for each place behind
   (goal_actor_place_waits by screen + 320 + unk_01). */
void goal_actor_run(void)
{
	uint32_t carry = 0;
	uint8_t frame_time = 0;
	uint16_t step_flags = 0;
	uint32_t animation = 0, ignored = 0, script = 0, script_word = 0, step_value = 0;
	uint16_t wait_count = 0, repeat_count = 0;
	uint32_t next_step = 0, command = 0, step_end = 0;

	script_word = OBJECT(current_object)->goal_actor.script;
	for (;;)
	{
		step_flags = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		script_word += 2u;
		if (carry)
		{
			OBJECT(current_object)->goal_actor.counter++;
			wait_count = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			if (OBJECT(current_object)->goal_actor.counter <= wait_count)
			{
				step_value = draw_object(step_value);
				return;
			}
			OBJECT(current_object)->goal_actor.counter = 0u;
			OBJECT(current_object)->goal_actor.script = script_word;
			continue;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->goal_actor.counter++;
			repeat_count = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			if (OBJECT(current_object)->goal_actor.counter <= repeat_count)
			{
				script_word = OBJECT(current_object)->goal_actor.step;
				OBJECT(current_object)->goal_actor.script = script_word;
				continue;
			}
			OBJECT(current_object)->goal_actor.counter = 0u;
			OBJECT(current_object)->goal_actor.script = script_word;
			continue;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
			ignored = animation_advance(ignored);
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->frame = (uint8_t)GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->x += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->x = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->y += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->y = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->width_scale += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			OBJECT(current_object)->height_scale += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->width_scale = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			OBJECT(current_object)->height_scale = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->depth_layer += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->depth_layer = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			animation = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			OBJECT(current_object)->animation = animation;
			frame_time = SPRITE_ANIMATION(sprite_animations[animation])->frame_time;
			OBJECT(current_object)->frame_time = frame_time;
			OBJECT(current_object)->frame_countdown = frame_time;
			OBJECT(current_object)->frame = 0u;
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			OBJECT(current_object)->palette = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			step_value = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			request_sound((uint8_t)step_value);
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		if (carry == 0u)
		{
			next_step = script_word;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = next_step;
			OBJECT(current_object)->goal_actor.step = script;
			step_value = draw_object(step_value);
			return;
		}
		command = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
		step_value = goal_actor_command_jumps[command];
		script_word += 2u;
		switch (command)
		{
		case 0:
			OBJECT(OBJECT(current_object)->goal_actor.scene)->state = 2u;
			next_step = script_word;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = next_step;
			OBJECT(current_object)->goal_actor.step = script;
			step_value = draw_object(step_value);
			return;
		case 1:
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		case 2:
			step_end = script_word;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = step_end;
			OBJECT(current_object)->goal_actor.step = script;
			return;
		case 3:
			OBJECT(current_object)->flags ^= 0x40u;
			next_step = script_word;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = next_step;
			OBJECT(current_object)->goal_actor.step = script;
			step_value = draw_object(step_value);
			return;
		case 4:
			CAR(player_car)->segment_progress = (CAR(player_car)->segment_progress & 0xFFFF0000u) | ((uint16_t)CAR(player_car)->segment_progress + 0x4000u & 0xFFFFu);
			next_step = script_word;
			script = OBJECT(current_object)->goal_actor.script;
			OBJECT(current_object)->goal_actor.script = next_step;
			OBJECT(current_object)->goal_actor.step = script;
			step_value = draw_object(step_value);
			return;
		case 5:
			break;
		default:
			invalid_jump(goal_actor_command_jumps[command]);
		}
		OBJECT(current_object)->goal_actor.counter++;
		step_value = (uint16_t)(GOAL_ACTOR_SCRIPT_WORDS(script_word)[0] + ((uint32_t)goal_actor_place_waits[mem_read8(screen + 0x320u + SCREEN(screen)->player)] << 3));
		script_word += 2u;
		if (OBJECT(current_object)->goal_actor.counter <= step_value)
		{
			script_word = OBJECT(current_object)->goal_actor.step;
			OBJECT(current_object)->goal_actor.script = script_word;
			continue;
		}
		OBJECT(current_object)->goal_actor.counter = 0u;
		OBJECT(current_object)->goal_actor.script = script_word;
	}
}

/* The road of the race, drawn on its tilemap with line scroll and line select. By state: 0
   road_surface_start; then each frame this frame's lines (road_surface_lines) and, from state 2, those
   of the frame in between (road_surface_halfway_lines); where the first segment's page set
   (segment_page_set) is 0 it starts the stage's palette fades (start_palette_fades with screen
   road_palette_fades; state 3), and at the next one that is not takes it as page_set and fades back
   (road_surface_fade_back; state 2). Each frame the tilemap's page word (at page_register) is the page
   table's word for page_set and near_scroll (five a set). At each new segment (segment past
   last_segment) acts on the segment's event when it changes (after the first 30 segments, event_delay):
   4 and 5 change the background within the stage (screen stage_background_change = FF, unk_204 = 0 or 1,
   for race_background), 1 and 2 turn the two road_tiles_change flags (0A, 0B) on and off, 3 both; a flag
   turned on starts a road_tiles_change (the Golden Gate Bridge in race_to_goal.txt, frames 7659-8325). */
void road_surface_update(void)
{
	uint32_t event = 0, fraction_shift = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, ignored_5 = 0, ignored_6 = 0, ignored_7 = 0, max_depth = 0, near_scroll = 0, object = 0, previous_depth = 0, previous_line = 0, previous_x = 0, road_palette_fades = 0, rounding = 0, stripe_count = 0;
	uint8_t first_flag = 0, first_tiles_flag = 0, second_tiles_flag = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		road_surface_start();
		OBJECT(current_object)->road_surface.event_delay = 0x1Eu;
		OBJECT(current_object)->state++;
		return;
	case 1:
		road_surface_lines(&ignored, &ignored_2, &ignored_3, &ignored_4, &ignored_5, &ignored_6, &ignored_7);
		OBJECT(current_object)->road_surface.points_buffer ^= 0x60u;
		OBJECT(current_object)->state++;
		if (OBJECT(current_object)->road_surface.segment_page_set == 0u)
		{
			road_palette_fades = SCREEN(screen)->road_palette_fades;
			start_palette_fades(road_palette_fades);
			OBJECT(current_object)->state++;
		}
		break;
	case 2:
		road_surface_lines(&previous_line, &previous_x, &previous_depth, &stripe_count, &fraction_shift, &rounding, &max_depth);
		road_surface_halfway_lines(previous_line, previous_x, previous_depth, stripe_count, (uint8_t)fraction_shift, rounding, (uint16_t)max_depth);
		if (OBJECT(current_object)->road_surface.segment_page_set == 0u)
		{
			road_palette_fades = SCREEN(screen)->road_palette_fades;
			start_palette_fades(road_palette_fades);
			OBJECT(current_object)->state++;
		}
		break;
	case 3:
		road_surface_lines(&previous_line, &previous_x, &previous_depth, &stripe_count, &fraction_shift, &rounding, &max_depth);
		road_surface_halfway_lines(previous_line, previous_x, previous_depth, stripe_count, (uint8_t)fraction_shift, rounding, (uint16_t)max_depth);
		if (OBJECT(current_object)->road_surface.segment_page_set != 0u)
		{
			OBJECT(current_object)->road_surface.page_set = OBJECT(current_object)->road_surface.segment_page_set;
			road_palette_fades = SCREEN(screen)->road_palette_fades;
			road_surface_fade_back(road_palette_fades);
			OBJECT(current_object)->state--;
		}
		break;
	default:
		invalid_jump(road_surface_state_jumps[OBJECT(current_object)->state]);
	}
	near_scroll = OBJECT(current_object)->road_surface.near_scroll;
	near_scroll = (uint16_t)(near_scroll + 0x4FFu);
	mem_write32(OBJECT(current_object)->road_surface.page_register, mem_read32(current_object - 0x24u + ((near_scroll >> 9) + OBJECT(current_object)->road_surface.page_set * 5u) * 4u));
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
		SCREEN(screen)->stage_background_change = 0xFFu;
		SCREEN(screen)->stage_background_branch = 0u;
		return;
	case 5:
		SCREEN(screen)->stage_background_change = 0xFFu;
		SCREEN(screen)->stage_background_branch = 1u;
		return;
	case 0:
		return;
	case 1:
		first_flag = ~OBJECT(current_object)->road_surface.first_tiles_flag;
		OBJECT(current_object)->road_surface.first_tiles_flag = first_flag;
		if (first_flag == 0u)
			return;
		road_surface_start_tiles_change();
		return;
	case 3:
		first_tiles_flag = ~OBJECT(current_object)->road_surface.first_tiles_flag;
		OBJECT(current_object)->road_surface.first_tiles_flag = first_tiles_flag;
		if (first_tiles_flag != 0u)
			road_surface_start_tiles_change();
		second_tiles_flag = ~OBJECT(current_object)->road_surface.second_tiles_flag;
		OBJECT(current_object)->road_surface.second_tiles_flag = second_tiles_flag;
		if (second_tiles_flag == 0u)
			return;
		object = start_object();
		OBJECT(object)->road_tiles_change.table_half = 1u;
		OBJECT(object)->road_tiles_change.road = current_object;
		OBJECT(object)->road_tiles_change.flag = current_object + 0xBu;
		OBJECT(object)->road_tiles_change.data = road_tiles_change_second_data_ADDRESS;
		OBJECT(object)->handler = road_tiles_change_update_ADDRESS;
		return;
	case 2:
		second_tiles_flag = ~OBJECT(current_object)->road_surface.second_tiles_flag;
		OBJECT(current_object)->road_surface.second_tiles_flag = second_tiles_flag;
		if (second_tiles_flag == 0u)
			return;
		object = start_object();
		OBJECT(object)->road_tiles_change.table_half = 1u;
		OBJECT(object)->road_tiles_change.road = current_object;
		OBJECT(object)->road_tiles_change.flag = current_object + 0xBu;
		OBJECT(object)->road_tiles_change.data = road_tiles_change_second_data_ADDRESS;
		OBJECT(object)->handler = road_tiles_change_update_ADDRESS;
		return;
	default:
		invalid_jump(road_surface_event_jumps[event]);
	}
}

/* Sets up the road's tilemap from the screen's road_surface_layer_setups record: its bits in
   video_registers[2], its scroll (160, 0), tile bank, mixer bits, page_register, the line tables in the
   video RAM (line_scroll_table, line_select_table) with the line select's start from 100800, the page
   set of the stage (stage_road_page_sets by screen first_stage), the page table
   (road_surface_page_words) and the stage's palettes (stage_road_palettes by stage: the fades' list into
   screen road_palette_fades, the rest queued with queue_palette_list). */
void road_surface_start(void)
{
	uint16_t video_bits = 0;
	uint32_t line_select_table = 0, mixer_address = 0, mixer_bits = 0, palettes = 0, road_surface_layer_setup = 0;

	road_surface_layer_setup = road_surface_layer_setups_ADDRESS + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
	video_bits = video_registers[2] & ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup)->video_mask;
	road_surface_layer_setup += 2u;
	video_registers[2] = video_bits | ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x2u)->video_bits;
	road_surface_layer_setup += 2u;
	mem_write16(ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x4u)->scroll_x_address, 0x160u);
	road_surface_layer_setup += 4u;
	mem_write16(ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x8u)->scroll_y_address, 0u);
	send_video_registers = 0xFFu;
	road_surface_layer_setup += 4u;
	mem_write8(ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0xCu)->tile_bank_address, 0u);
	road_surface_layer_setup += 4u;
	OBJECT(current_object)->road_surface.page_register = ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x10u)->page_register;
	road_surface_layer_setup += 4u;
	mixer_address = ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x14u)->mixer_address;
	mixer_bits = mem_read32(mixer_address);
	mixer_bits = (mixer_bits & ~0xFFFFu) | (mixer_bits & 0xFFF0u);
	mem_write32(mixer_address, (mixer_bits & ~0xFFu) | (uint8_t)(mixer_bits | 8u));
	SCREEN(screen)->flags |= 4u;
	road_surface_layer_setup += 4u;
	OBJECT(current_object)->road_surface.line_scroll_table = ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x18u)->line_scroll_table;
	line_select_table = ROAD_SURFACE_LAYER_SETUP(road_surface_layer_setup - 0x18u)->line_select_table;
	OBJECT(current_object)->road_surface.line_select_table = line_select_table;
	move_halfwords(line_select_table - 0x1C0u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0x640u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0xE40u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0x1640u, zero_block_ADDRESS, 0xE0u);
	OBJECT(current_object)->road_surface.page_set = stage_road_page_sets[SCREEN(screen)->first_stage];
	OBJECT(current_object)->road_surface.points_buffer = 0u;
	OBJECT(current_object)->road_surface.last_event = 0u;
	OBJECT(current_object)->road_surface.last_segment = 0u;
	OBJECT(current_object)->road_surface.first_tiles_flag = 0u;
	OBJECT(current_object)->road_surface.second_tiles_flag = 0u;
	move_string(road_surface_page_words_ADDRESS, 0x1Eu, current_object + 0x18u, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	palettes = stage_road_palettes[SCREEN(screen)->first_stage];
	SCREEN(screen)->road_palette_fades = mem_read32(palettes);
	palettes += 4u;
	queue_palette_list(palettes);
}

/* Works out this frame's road: from the segments in view (screen + 3000, 16 bytes each, the first at the
   view's position race_copy window_segment) the first one's page set and event (+C, +D) and number
   (segment), each segment's point (+2 depth, +4 x, +6 y) into the points buffer (screen + 3800 by
   points_buffer), and between points the lines of the line scroll (the road's x) and line select (its
   depth, every other pair of segments in the picture's second half: the stripes) tables, from the bottom
   line up (road_surface_fill_lines); a stretch it passes over is filled after
   (road_surface_fill_skipped_lines); the farthest point's line into screen road_top_line and the bottom
   line's scroll into near_scroll. */
void road_surface_lines(uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_stripe_count, uint32_t *p_fraction_shift, uint32_t *p_rounding, uint32_t *p_max_depth)
{
	bool L_016E09 = false, L_016E73 = false, leave_loop = false, leave_loop_2 = false, leave_loop_3 = false, leave_loop_4 = false, leave_loop_5 = false, next_round = false;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t line_select = 0, max_depth = 0;
	uint32_t depth_step = 0, ignored = 0, ignored_2 = 0, line = 0, line_count = 0, point_depth = 0, point_entry = 0, point_line = 0, point_x = 0, scroll_place = 0, segment = 0, skip_state = 0, sprite_origin_y = 0, table_offset = 0, view_segment = 0, x_step = 0;
	uint32_t scroll_bottom = 0;
	uint16_t segment_scale = 0, remaining_scale = 0;
	uint32_t line_reached = 0, line_reached_first = 0, pairs_left = 0;

	view_segment = screen + 0x3000u;
	table_offset = (uint8_t)(video_registers[2] >> 8u);
	table_offset = (uint8_t)(table_offset + 8u);
	table_offset = (table_offset & 0xCu) << 7;
	scroll_place = OBJECT(current_object)->road_surface.line_scroll_table + table_offset * 4u;
	scroll_bottom = scroll_place;
	r12 = OBJECT(current_object)->road_surface.line_select_table + table_offset * 4u;
	segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	r14 = r14 & ~0xFFFFu;
	*p_fraction_shift = 0xFFFFFFF4u;
	*p_rounding = 0x800u;
	sprite_origin_y = (int16_t)SCREEN(screen)->sprite_origin_y;
	max_depth = 0xFFu;
	skip_state = 1u;
	point_entry = screen + 0x3800u + OBJECT(current_object)->road_surface.points_buffer * 2u * 4u;
	line = 0xE0u;
	while (mem_read16(view_segment) == 0u)
	{
		view_segment += 0x10u;
		segment++;
	}
	OBJECT(current_object)->road_surface.segment_page_set = mem_read8(view_segment + 0xCu);
	OBJECT(current_object)->road_surface.event = mem_read8(view_segment + 0xDu);
	OBJECT(current_object)->road_surface.segment = segment;
	mem_write16(point_entry, segment);
	segment = 0u - ((segment & ~0xFFFFu) | (segment & 3u));
	overflow = v60_overflow_add32(segment, 2u, 0u);
	zero = segment + 2u == 0u;
	negative = (segment + 2u) >> 31;
	segment += 2u;
	point_entry += 2u;
	if ((negative ^ overflow) | zero)
	{
		segment += 2u;
		r14 = r14 | 0x100u;
	}
	for (;;)
	{
		segment_scale = mem_read16(view_segment);
		view_segment += 2u;
		if (segment_scale == 0u)
			break;
		point_depth = mem_read16(view_segment) >> 2;
		mem_write16(point_entry, point_depth);
		point_entry += 2u;
		view_segment += 2u;
		point_x = 0u - (int16_t)mem_read16(view_segment);
		mem_write16(point_entry, point_x);
		point_entry += 2u;
		view_segment += 2u;
		point_line = (int16_t)mem_read16(view_segment) + sprite_origin_y;
		mem_write16(point_entry, point_line);
		point_entry += 2u;
		view_segment += 0xAu;
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
						if (L_016E09 || L_016E73 || L_016E09 || L_016E09 || skip_state == 0u)
						{
							if (!L_016E09)
							{
								if (!L_016E73)
								{
									if (!L_016E09)
									{
										if (!L_016E09)
										{
											line_reached = line;
											line_count = line_reached - point_line;
											if ((int32_t)line_reached <= (int32_t)point_line)
											{
												*p_previous_line = point_line;
												*p_previous_x = point_x;
												*p_previous_depth = point_depth;
												segment--;
												if (segment != 0u)
												{
													next_round = true;
													leave_loop = true;
													leave_loop_2 = true;
													leave_loop_3 = true;
													break;
												}
												segment = 2u;
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
									if (L_016E73 || L_016E09 || L_016E09 || L_016E73 || L_016E09 || L_016E09 || line_count == 1u)
									{
										if (!L_016E73)
										{
											if (L_016E09 || L_016E09 || !L_016E73)
											{
												L_016E09 = false;
												scroll_place -= 2u;
												mem_write16(scroll_place, point_x);
												line_select = (uint16_t)point_depth;
												if ((int16_t)max_depth < (int16_t)line_select)
													line_select = (uint16_t)max_depth;
												r12 = r12 - 2u;
												mem_write16(r12, line_select + r14);
												line = point_line;
											}
										}
										if (L_016E73 || L_016E73 || line == 0u)
										{
											for (;;)
											{
												remaining_scale = mem_read16(view_segment);
												view_segment += 2u;
												if (remaining_scale == 0u)
													break;
												mem_write16(point_entry, mem_read16(view_segment) >> 2);
												point_entry += 2u;
												view_segment += 2u;
												mem_write16(point_entry, 0u - mem_read16(view_segment));
												point_entry += 2u;
												view_segment += 2u;
												point_line = (int16_t)mem_read16(view_segment) + sprite_origin_y;
												mem_write16(point_entry, point_line);
												point_entry += 2u;
												view_segment += 0xAu;
											}
											SCREEN(screen)->road_top_line = point_line;
											mem_write16(point_entry, 0xFFFFu);
											road_surface_fill_skipped_lines((uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_x, p_previous_depth);
											OBJECT(current_object)->road_surface.near_scroll = mem_read16(scroll_bottom - 2u);
											*p_stripe_count = segment;
											*p_max_depth = (*p_max_depth & 0xFFFF0000u) | max_depth;
											return;
										}
										*p_previous_line = point_line;
										*p_previous_x = point_x;
										*p_previous_depth = point_depth;
										segment--;
										if (segment != 0u)
										{
											next_round = true;
											leave_loop = true;
											leave_loop_2 = true;
											leave_loop_3 = true;
											leave_loop_4 = true;
											leave_loop_5 = true;
											break;
										}
										segment = 2u;
										r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
										next_round = true;
										leave_loop = true;
										leave_loop_2 = true;
										leave_loop_3 = true;
										leave_loop_4 = true;
										leave_loop_5 = true;
										break;
									}
									road_surface_fill_lines(line, point_line, point_x, point_depth, (uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_line, p_previous_x, p_previous_depth, &ignored, &ignored_2, &scroll_place, &zero);
									L_016E73 = zero == 0u;
								} while (L_016E73);
								if (leave_loop_5)
									break;
								L_016E09 = true;
							}
							if (leave_loop_4)
								break;
						}
						line_reached_first = line;
						line_count = line_reached_first - point_line;
						if ((int32_t)line_reached_first <= (int32_t)point_line)
						{
							skip_state = 0u;
							OBJECT(current_object)->road_surface.skipped_line = 0u;
							*p_previous_line = point_line;
							*p_previous_x = point_x;
							*p_previous_depth = point_depth;
							segment--;
							if (segment != 0u)
							{
								next_round = true;
								leave_loop = true;
								leave_loop_2 = true;
								leave_loop_3 = true;
								break;
							}
							segment = 2u;
							r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
							next_round = true;
							leave_loop = true;
							leave_loop_2 = true;
							leave_loop_3 = true;
							break;
						}
						if ((int32_t)skip_state < 0)
						{
							L_016E09 = line_count == 1u;
							if (!L_016E09)
							{
								OBJECT(current_object)->road_surface.skipped_x = *p_previous_x;
								OBJECT(current_object)->road_surface.skipped_depth = *p_previous_depth;
								road_surface_fill_lines(line, point_line, point_x, point_depth, (uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_line, p_previous_x, p_previous_depth, &x_step, &depth_step, &scroll_place, &zero);
								OBJECT(current_object)->road_surface.skipped_x_step = x_step;
								OBJECT(current_object)->road_surface.skipped_depth_step = depth_step;
								skip_state = 0u;
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
			line_count = (line_count - 1u) * 2u;
			scroll_place -= line_count;
			r12 = r12 - line_count;
			OBJECT(current_object)->road_surface.skipped_line = point_line;
			OBJECT(current_object)->road_surface.skipped_scroll_place = scroll_place;
			OBJECT(current_object)->road_surface.skipped_select_place = r12;
			OBJECT(current_object)->road_surface.skipped_stripe = r14;
			skip_state = 0xFFFFFFFFu;
			L_016E09 = true;
		}
		if (next_round)
			continue;
	}
	pairs_left = shift_by(line, max_depth, 32);
	carry = v60_shl_cy(line, max_depth, 32);
	zero = pairs_left == 0u;
	line = pairs_left;
	if (carry == 0u)
	{
		line++;
		for (;;)
		{
			r12 = r12 - 4u;
			line--;
			if (((line != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		SCREEN(screen)->road_top_line = point_line;
		mem_write16(point_entry, 0xFFFFu);
		road_surface_fill_skipped_lines((uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_x, p_previous_depth);
		OBJECT(current_object)->road_surface.near_scroll = mem_read16(scroll_bottom - 2u);
		*p_stripe_count = segment;
		*p_max_depth = (*p_max_depth & 0xFFFF0000u) | max_depth;
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		SCREEN(screen)->road_top_line = point_line;
		mem_write16(point_entry, 0xFFFFu);
		road_surface_fill_skipped_lines((uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_x, p_previous_depth);
		OBJECT(current_object)->road_surface.near_scroll = mem_read16(scroll_bottom - 2u);
		*p_stripe_count = segment;
		*p_max_depth = (*p_max_depth & 0xFFFF0000u) | max_depth;
		return;
	}
	line++;
	for (;;)
	{
		r12 = r12 - 4u;
		line--;
		if (((line != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	SCREEN(screen)->road_top_line = point_line;
	mem_write16(point_entry, 0xFFFFu);
	road_surface_fill_skipped_lines((uint8_t)*p_fraction_shift, *p_rounding, max_depth, p_previous_x, p_previous_depth);
	OBJECT(current_object)->road_surface.near_scroll = mem_read16(scroll_bottom - 2u);
	*p_stripe_count = segment;
	*p_max_depth = (*p_max_depth & 0xFFFF0000u) | max_depth;
}

/* Works out the road of the frame in between: the same lines from the average of this frame's points and
   the last frame's (the two halves of the points buffer, matched by segment), into the next tables;
   flips points_buffer. */
void road_surface_halfway_lines(uint32_t previous_line, uint32_t previous_x, uint32_t previous_depth, uint32_t stripe_count, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t line_select = 0, segment = 0;
	uint32_t depth_step = 0, ignored = 0, ignored_2 = 0, last_segment = 0, line = 0, line_count = 0, point_depth = 0, point_line = 0, point_x = 0, scroll_place = 0, skip_state = 0, table_offset = 0, x_step = 0;
	uint16_t last_frame_segment = 0, last_depth = 0, last_x = 0, last_line = 0;
	uint32_t line_reached = 0, line_reached_first = 0, pairs_left = 0;

	table_offset = (uint8_t)(video_registers[2] >> 8u);
	table_offset = (uint8_t)(table_offset + 4u);
	table_offset = (table_offset & 0xCu) << 7;
	scroll_place = OBJECT(current_object)->road_surface.line_scroll_table + table_offset * 4u;
	r12 = OBJECT(current_object)->road_surface.line_select_table + table_offset * 4u;
	r14 = r14 & ~0xFFFFu;
	skip_state = 1u;
	line = 0xE0u;
	r27 = OBJECT(current_object)->road_surface.points_buffer;
	r28 = r27;
	r28 = r28 ^ 0x60u;
	OBJECT(current_object)->road_surface.points_buffer = r28;
	r27 = r27 * 2u;
	r28 = r28 * 2u;
	r27 = screen + 0x3800u + r27 * 4u;
	r28 = screen + 0x3800u + r28 * 4u;
	segment = mem_read16(r27);
	last_frame_segment = mem_read16(r28);
	r28 = r28 + 2u;
	last_segment = (stripe_count & ~0xFFFFu) | last_frame_segment;
	r27 = r27 + 2u;
	if ((uint16_t)last_segment != segment)
	{
		if ((uint16_t)last_segment < segment)
		{
			do
			{
				r28 = r28 + 6u;
				last_segment = (last_segment & ~0xFFFFu) | (uint16_t)(last_segment + 1u);
			} while ((uint16_t)last_segment < segment);
		}
		else
		{
			do
			{
				r27 = r27 + 6u;
				segment++;
			} while (segment < (uint16_t)last_segment);
		}
	}
	last_segment = 0u - ((last_segment & ~0xFFFFu) | (last_segment & 3u));
	overflow = v60_overflow_add32(last_segment, 2u, 0u);
	zero = last_segment + 2u == 0u;
	negative = (last_segment + 2u) >> 31;
	last_segment += 2u;
	if ((negative ^ overflow) | zero)
	{
		last_segment += 2u;
		r14 = r14 | 0x100u;
	}
	while ((int16_t)(mem_read16(r27) | mem_read16(r28)) >= 0)
	{
		last_depth = mem_read16(r28);
		r28 = r28 + 2u;
		point_depth = shift_by((uint32_t)mem_read16(r27) + last_depth, (uint8_t)max_depth, 32);
		r27 = r27 + 2u;
		last_x = mem_read16(r28);
		r28 = r28 + 2u;
		point_x = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)last_x, (uint8_t)max_depth, 32);
		r27 = r27 + 2u;
		last_line = mem_read16(r28);
		r28 = r28 + 2u;
		point_line = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)last_line, (uint8_t)max_depth, 32);
		r27 = r27 + 2u;
		if (skip_state == 0u)
		{
			line_reached = line;
			line_count = line_reached - point_line;
			if ((int32_t)point_line < (int32_t)line_reached)
			{
				if (line_count != 1u)
				{
					road_surface_fill_lines(line, point_line, point_x, point_depth, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &ignored, &ignored_2, &scroll_place, &zero);
					if (zero == 0u)
					{
						road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
						return;
					}
				}
				scroll_place -= 2u;
				mem_write16(scroll_place, point_x);
				line_select = (uint16_t)point_depth;
				if ((int16_t)max_depth < (int16_t)line_select)
					line_select = (uint16_t)max_depth;
				r12 = r12 - 2u;
				mem_write16(r12, line_select + r14);
				line = point_line;
				if (line == 0u)
				{
					road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
					return;
				}
			}
		}
		else
		{
			line_reached_first = line;
			line_count = line_reached_first - point_line;
			if ((int32_t)point_line < (int32_t)line_reached_first)
			{
				if ((int32_t)skip_state < 0)
				{
					if (line_count == 1u)
					{
						scroll_place -= 2u;
						mem_write16(scroll_place, point_x);
						line_select = (uint16_t)point_depth;
						if ((int16_t)max_depth < (int16_t)line_select)
							line_select = (uint16_t)max_depth;
						r12 = r12 - 2u;
						mem_write16(r12, line_select + r14);
						line = point_line;
						if (line == 0u)
						{
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
							return;
						}
					}
					else
					{
						OBJECT(current_object)->road_surface.skipped_x = previous_x;
						OBJECT(current_object)->road_surface.skipped_depth = previous_depth;
						road_surface_fill_lines(line, point_line, point_x, point_depth, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &x_step, &depth_step, &scroll_place, &zero);
						OBJECT(current_object)->road_surface.skipped_x_step = x_step;
						OBJECT(current_object)->road_surface.skipped_depth_step = depth_step;
						skip_state = 0u;
						if (zero == 0u)
						{
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
							return;
						}
						scroll_place -= 2u;
						mem_write16(scroll_place, point_x);
						line_select = (uint16_t)point_depth;
						if ((int16_t)max_depth < (int16_t)line_select)
							line_select = (uint16_t)max_depth;
						r12 = r12 - 2u;
						mem_write16(r12, line_select + r14);
						line = point_line;
						if (line == 0u)
						{
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
							return;
						}
					}
				}
				else
				{
					line_count = (line_count - 1u) * 2u;
					scroll_place -= line_count;
					r12 = r12 - line_count;
					OBJECT(current_object)->road_surface.skipped_line = point_line;
					OBJECT(current_object)->road_surface.skipped_scroll_place = scroll_place;
					OBJECT(current_object)->road_surface.skipped_select_place = r12;
					OBJECT(current_object)->road_surface.skipped_stripe = r14;
					skip_state = 0xFFFFFFFFu;
					scroll_place -= 2u;
					mem_write16(scroll_place, point_x);
					line_select = (uint16_t)point_depth;
					if ((int16_t)max_depth < (int16_t)line_select)
						line_select = (uint16_t)max_depth;
					r12 = r12 - 2u;
					mem_write16(r12, line_select + r14);
					line = point_line;
					if (line == 0u)
					{
						road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
						return;
					}
				}
			}
			else
			{
				skip_state = 0u;
				OBJECT(current_object)->road_surface.skipped_line = 0u;
			}
		}
		previous_line = point_line;
		previous_x = point_x;
		previous_depth = point_depth;
		last_segment--;
		if (last_segment != 0u)
			continue;
		last_segment = 2u;
		r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
	}
	pairs_left = shift_by(line, (uint8_t)max_depth, 32);
	carry = v60_shl_cy(line, (uint8_t)max_depth, 32);
	zero = pairs_left == 0u;
	line = pairs_left;
	if (carry == 0u)
	{
		line++;
		for (;;)
		{
			r12 = r12 - 4u;
			line--;
			if (((line != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
		return;
	}
	line++;
	for (;;)
	{
		r12 = r12 - 4u;
		line--;
		if (((line != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth);
}

/* Fills the road's lines between two of its points, from the lower one up: the line scroll steps evenly
   from one point's x to the other's and the line select from one's depth to the other's (at most R18),
   plus R14 (100 on alternate pairs of segments); returns the two steps. */
void road_surface_fill_lines(uint32_t line, uint32_t point_line, uint32_t point_x, uint32_t point_depth, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_x_step, uint32_t *p_depth_step, uint32_t *p_line_scroll_place, uint32_t *p_zero)
{
	uint32_t previous_line = *p_previous_line;
	uint32_t previous_x = *p_previous_x;
	uint32_t previous_depth = *p_previous_depth;
	uint32_t line_scroll_place = *p_line_scroll_place;
	uint32_t zero = *p_zero;
	uint16_t line_select = 0;
	uint32_t depth_step = 0, line_count = 0, lines_done = 0, x_step = 0;
	uint32_t lower_line = 0, lower_line_clipped = 0;

	line_count = previous_line - point_line;
	x_step = divide_s32((previous_x - point_x) << 12, line_count);
	previous_x = (previous_x << 12) + rounding;
	depth_step = divide_s32((previous_depth - point_depth) << 12, line_count);
	previous_depth = (previous_depth << 12) + rounding;
	if ((int32_t)point_line < 0)
	{
		line_count += point_line;
		lower_line = previous_line;
		lines_done = lower_line - line;
		if ((int32_t)line < (int32_t)lower_line)
		{
			line_count -= lines_done;
			previous_x -= x_step * lines_done;
			previous_depth -= depth_step * lines_done;
		}
		do
		{
			previous_x -= x_step;
			previous_depth -= depth_step;
			line_scroll_place -= 2u;
			mem_write16(line_scroll_place, shift_signed_by(previous_x, (uint8_t)fraction_shift, 32));
			line_select = (uint16_t)shift_signed_by(previous_depth, (uint8_t)fraction_shift, 32);
			if ((int16_t)max_depth < (int16_t)line_select)
				line_select = (uint16_t)max_depth;
			r12 = r12 - 2u;
			mem_write16(r12, line_select + r14);
			line_count--;
		} while (line_count != 0u);
		zero = 1u == 0u;
	}
	else
	{
		line_count--;
		lower_line_clipped = previous_line;
		lines_done = lower_line_clipped - line;
		if ((int32_t)line < (int32_t)lower_line_clipped)
		{
			line_count -= lines_done;
			previous_x -= x_step * lines_done;
			previous_depth -= depth_step * lines_done;
		}
		do
		{
			previous_x -= x_step;
			previous_depth -= depth_step;
			line_scroll_place -= 2u;
			mem_write16(line_scroll_place, shift_signed_by(previous_x, (uint8_t)fraction_shift, 32));
			line_select = (uint16_t)shift_signed_by(previous_depth, (uint8_t)fraction_shift, 32);
			if ((int16_t)max_depth < (int16_t)line_select)
				line_select = (uint16_t)max_depth;
			r12 = r12 - 2u;
			mem_write16(r12, line_select + r14);
			line_count--;
		} while (line_count != 0u);
		zero = 0u == 0u;
	}
	*p_previous_line = lines_done;
	*p_previous_x = previous_x;
	*p_previous_depth = previous_depth;
	*p_x_step = x_step;
	*p_depth_step = depth_step;
	*p_line_scroll_place = line_scroll_place;
	*p_zero = zero;
}

/* Fills the stretch of lines road_surface_lines passed over (from line skipped_line, at the table places
   it kept) by going on from the kept point with the kept steps down to line DF; nothing when there is
   none. */
void road_surface_fill_skipped_lines(uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_x, uint32_t *p_previous_depth)
{
	uint16_t line_select = 0;
	uint32_t line_count = 0, scroll_place = 0, skipped_depth_step = 0, skipped_line = 0, x_step = 0;

	skipped_line = OBJECT(current_object)->road_surface.skipped_line;
	if (skipped_line == 0u)
		return;
	*p_previous_x = (OBJECT(current_object)->road_surface.skipped_x << 12) + rounding;
	*p_previous_depth = (OBJECT(current_object)->road_surface.skipped_depth << 12) + rounding;
	x_step = OBJECT(current_object)->road_surface.skipped_x_step;
	skipped_depth_step = OBJECT(current_object)->road_surface.skipped_depth_step;
	scroll_place = OBJECT(current_object)->road_surface.skipped_scroll_place;
	r12 = OBJECT(current_object)->road_surface.skipped_select_place;
	r14 = OBJECT(current_object)->road_surface.skipped_stripe;
	line_count = 0xDFu - skipped_line;
	if (0xDFu == skipped_line)
	{
		return;
	}
	do
	{
		*p_previous_x += x_step;
		*p_previous_depth += skipped_depth_step;
		mem_write16(scroll_place, shift_signed_by(*p_previous_x, (uint8_t)fraction_shift, 32));
		line_select = (uint16_t)shift_signed_by(*p_previous_depth, (uint8_t)fraction_shift, 32);
		scroll_place += 2u;
		if ((int16_t)max_depth < (int16_t)line_select)
		{
			line_select = (uint16_t)max_depth;
			x_step = 0u;
		}
		mem_write16(r12, line_select + r14);
		r12 = r12 + 2u;
		line_count--;
	} while (line_count != 0u);
}

/* Starts the road_tiles_change of event 1 (its data road_tiles_change_first_data, the even bytes of the
   page table) with the road's first flag (0A). */
void road_surface_start_tiles_change(void)
{
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->road_tiles_change.table_half = 0u;
	OBJECT(object)->road_tiles_change.road = current_object;
	OBJECT(object)->road_tiles_change.flag = current_object + 0xAu;
	OBJECT(object)->road_tiles_change.data = road_tiles_change_first_data_ADDRESS;
	OBJECT(object)->handler = road_tiles_change_update_ADDRESS;
}

/* Changes the road's tiles for a stretch of road (road_surface starts it when an event turns one of its
   flags on; the Golden Gate Bridge in race_to_goal.txt): copies the four tile pages the road shows
   (source_pages to target_pages, a page a frame), turns its half of the road's page table (table_half:
   the even or odd bytes) to the copies, copies the new look into the pages by the road's page set (16
   halfwords of each a frame for 16 frames) and turns the table back; when the road turns its flag off
   (flag) it changes them back the same way and ends; its data list (data: road_tiles_change_first_data
   for the first flag, road_tiles_change_second_data for the second) holds the pages and the bytes for
   the table. */
void road_tiles_change_update(void)
{
	uint32_t byte_count = 0, data = 0, halfword_count = 0, page_count = 0, page_numbers = 0, page_set = 0, row_bytes = 0, row_count = 0, source_slot = 0, step = 0, table_place = 0, table_source = 0, target_page_numbers = 0, target_slot = 0;
	uint8_t target_page = 0, pages_left = 0, step_2 = 0, target_page_back = 0, pages_left_2 = 0, step_3 = 0;

	data = OBJECT(current_object)->road_tiles_change.data;
	switch (OBJECT(current_object)->state)
	{
	case 0:
		source_slot = current_object + 0x18u;
		target_slot = current_object + 0x28u;
		page_set = OBJECT(OBJECT(current_object)->road_tiles_change.road)->road_surface.page_set;
		OBJECT(current_object)->road_tiles_change.page_set = page_set;
		r12 = data - 0xCu + page_set * 4u;
		target_page_numbers = data + 0x18u + SCREEN(screen)->index * 4u;
		page_count = 4u;
		do
		{
			mem_write32(source_slot, 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(r12)[0] << 8) * 4u);
			target_page = ROAD_TILES_CHANGE_BYTES(target_page_numbers)[0];
			target_page_numbers++;
			mem_write32(target_slot, 0x300000u + ((uint32_t)target_page << 8) * 4u);
			target_slot += 4u;
			page_count--;
			r12 = r12 + 1u;
			source_slot += 4u;
		} while (page_count != 0u);
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
		table_source = data + 0x20u + SCREEN(screen)->index * 5u * 2u;
		table_place = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		row_count = 3u;
		do
		{
			r12 = table_source;
			row_bytes = 0xAu;
			do
			{
				mem_write8(table_place, ROAD_TILES_CHANGE_BYTES(r12)[0]);
				row_bytes--;
				r12 = r12 + 1u;
				table_place += 2u;
			} while (row_bytes != 0u);
			row_count--;
		} while (row_count != 0u);
		source_slot = current_object + 0x18u;
		page_numbers = data + OBJECT(current_object)->road_tiles_change.page_set * 4u;
		page_count = 4u;
		do
		{
			mem_write32(source_slot, 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(page_numbers)[0] << 8) * 4u);
			page_count--;
			page_numbers++;
			source_slot += 4u;
		} while (page_count != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0x10u;
		OBJECT(current_object)->state++;
		return;
	case 2:
		halfword_count = 0x10u;
		source_slot = current_object + 0x18u;
		target_slot = current_object + 0x28u;
		page_count = 4u;
		do
		{
			r12 = mem_read32(source_slot);
			move_string(r12, halfword_count, mem_read32(target_slot), halfword_count, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(source_slot, r28);
			mem_write32(target_slot, r27);
			page_count--;
			source_slot += 4u;
			target_slot += 4u;
		} while (page_count != 0u);
		step_2 = OBJECT(current_object)->road_tiles_change.step;
		OBJECT(current_object)->road_tiles_change.step = step_2 - 1u;
		if (step_2 != 1u)
			return;
		table_source = data + 0x34u;
		table_place = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		byte_count = 0x1Eu;
		do
		{
			mem_write8(table_place, ROAD_TILES_CHANGE_BYTES(table_source)[0]);
			byte_count--;
			table_source++;
			table_place += 2u;
		} while (byte_count != 0u);
		OBJECT(current_object)->state++;
	case 3:
		if (mem_read8(OBJECT(current_object)->road_tiles_change.flag) != 0u)
			return;
		source_slot = current_object + 0x18u;
		target_slot = current_object + 0x28u;
		page_set = OBJECT(OBJECT(current_object)->road_tiles_change.road)->road_surface.page_set;
		OBJECT(current_object)->road_tiles_change.page_set = page_set;
		r12 = data + page_set * 4u;
		target_page_numbers = data + 0x18u + SCREEN(screen)->index * 4u;
		page_count = 4u;
		do
		{
			mem_write32(source_slot, 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(r12)[0] << 8) * 4u);
			target_page_back = ROAD_TILES_CHANGE_BYTES(target_page_numbers)[0];
			target_page_numbers++;
			mem_write32(target_slot, 0x300000u + ((uint32_t)target_page_back << 8) * 4u);
			target_slot += 4u;
			page_count--;
			r12 = r12 + 1u;
			source_slot += 4u;
		} while (page_count != 0u);
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
		table_source = data + 0x20u + SCREEN(screen)->index * 5u * 2u;
		table_place = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		row_count = 3u;
		do
		{
			r12 = table_source;
			row_bytes = 0xAu;
			do
			{
				mem_write8(table_place, ROAD_TILES_CHANGE_BYTES(r12)[0]);
				row_bytes--;
				r12 = r12 + 1u;
				table_place += 2u;
			} while (row_bytes != 0u);
			row_count--;
		} while (row_count != 0u);
		source_slot = current_object + 0x18u;
		page_numbers = data - 0xCu + OBJECT(current_object)->road_tiles_change.page_set * 4u;
		page_count = 4u;
		do
		{
			mem_write32(source_slot, 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(page_numbers)[0] << 8) * 4u);
			page_count--;
			page_numbers++;
			source_slot += 4u;
		} while (page_count != 0u);
		OBJECT(current_object)->road_tiles_change.step = 0x10u;
		OBJECT(current_object)->state++;
		return;
	case 5:
		halfword_count = 0x10u;
		source_slot = current_object + 0x18u;
		target_slot = current_object + 0x28u;
		page_count = 4u;
		do
		{
			r12 = mem_read32(source_slot);
			move_string(r12, halfword_count, mem_read32(target_slot), halfword_count, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(source_slot, r28);
			mem_write32(target_slot, r27);
			page_count--;
			source_slot += 4u;
			target_slot += 4u;
		} while (page_count != 0u);
		step_3 = OBJECT(current_object)->road_tiles_change.step;
		OBJECT(current_object)->road_tiles_change.step = step_3 - 1u;
		if (step_3 != 1u)
			return;
		table_source = data + 0x52u;
		table_place = OBJECT(current_object)->road_tiles_change.road + 0x18u + OBJECT(current_object)->road_tiles_change.table_half;
		byte_count = 0x1Eu;
		do
		{
			mem_write8(table_place, ROAD_TILES_CHANGE_BYTES(table_source)[0]);
			byte_count--;
			table_source++;
			table_place += 2u;
		} while (byte_count != 0u);
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(road_tiles_change_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The scenery behind the road (the race's first tilemap). By state: 0 sets it up: its register copies
   (race_background_layer_setups by screen: scroll_x_address, scroll_y_address, tile_bank_address,
   page_word_address), the stage's background (background: stage_backgrounds by screen first_stage, none
   for stage 0), its tile pages (background_tile_page_lists, loaded at once with unpack_tile_pages), tile
   bank and palettes (race_background_fade_palettes, queue_palette_list), its objects
   (race_background_start_objects) and, for stages 6, 8 and 1C, its animation
   (race_background_start_lower_animation, race_background_start_upper_animation); 1 waits for a change:
   a fork's new stage (the car's unk_CA; background 2 * screen built_stage + the branch, car branch) or
   the road's events 4 and 5 (screen stage_background_change; background 2A + unk_A3): screen
   background_changing set, the new background's pages queued into the other half of the pages
   (page_half; queue_tile_pages); 2 changes over as the view turns away and back (the car's unk_2E;
   change_step): when it starts its palettes, tile bank and the first page word (next_page_words), at 2A0
   (off the screen) the fades of background_leaving_fades and of the road (background_road_palettes, into
   screen road_palette_fades) and the next, the new stage's objects (screen route_stage), at 160 its
   palette fades (race_background_fade_palettes) and the next, at 60 the last, at 0 back to state 1 with
   the animation of backgrounds 5, 7, 8, 25 and 26. Each frame scrolls it: x by the car's unk_2E (a
   quarter of it, all of it during a change) less A0, y by the slope (the car's unk_30) and screen
   horizon_line, kept between 0 and 180 less the road's top line (screen road_top_line; also into screen
   background_scroll_y). */
void race_background_update(void)
{
	bool L_017685 = false;
	uint32_t zero = 0;
	uint8_t background_2 = 0, branch = 0, first_stage_2 = 0;
	uint16_t scroll_x = 0, scroll_y = 0, scroll_y_limit = 0;
	uint32_t animation = 0, background = 0, background_inverted = 0, fades = 0, first_stage = 0, index = 0, mixer_address = 0, mixer_bits = 0, new_background = 0, next_page_words = 0, page_count = 0, page_half = 0, page_number = 0, page_word_address = 0, page_word_set = 0, palettes = 0, race_background_layer_setup = 0, route_stage = 0, state_routine = 0, tile_page_list = 0, tile_pages = 0, view_turn = 0;
	uint32_t view_turn_kept = 0;
	uint8_t first_background = 0;
	uint16_t signed_turn = 0, turn_back = 0, slope_scroll_y = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = race_background_state_jumps[state];
	switch (state)
	{
	case 0:
		index = SCREEN(screen)->index;
		OBJECT(current_object)->race_background.page_half = index;
		race_background_layer_setup = race_background_layer_setups_ADDRESS + index * 5u * 4u;
		OBJECT(current_object)->race_background.scroll_x_address = RACE_BACKGROUND_LAYER_SETUP(race_background_layer_setup)->scroll_x_address;
		race_background_layer_setup += 4u;
		OBJECT(current_object)->race_background.scroll_y_address = RACE_BACKGROUND_LAYER_SETUP(race_background_layer_setup - 0x4u)->scroll_y_address;
		race_background_layer_setup += 4u;
		OBJECT(current_object)->race_background.tile_bank_address = RACE_BACKGROUND_LAYER_SETUP(race_background_layer_setup - 0x8u)->tile_bank_address;
		race_background_layer_setup += 4u;
		page_word_address = RACE_BACKGROUND_LAYER_SETUP(race_background_layer_setup - 0xCu)->page_word_address;
		OBJECT(current_object)->race_background.page_word_address = page_word_address;
		mem_write32(page_word_address, background_page_words[(uint32_t)OBJECT(current_object)->race_background.page_half << 3]);
		mixer_address = RACE_BACKGROUND_LAYER_SETUP(race_background_layer_setup - 0xCu)->mixer_address;
		mixer_bits = mem_read32(mixer_address);
		mixer_bits = (mixer_bits & ~0xFFFFu) | (mixer_bits & 0xFFF0u);
		mem_write32(mixer_address, (mixer_bits & ~0xFFu) | (uint8_t)(mixer_bits | 4u));
		SCREEN(screen)->flags |= 4u;
		background_inverted = SCREEN(screen)->first_stage;
		if (background_inverted != 0u)
			background_inverted = (uint8_t)~stage_backgrounds[background_inverted];
		first_background = ~background_inverted;
		OBJECT(current_object)->race_background.background = first_background;
		tile_page_list = current_object + 0x2Cu;
		tile_pages = no_background_tile_pages_ADDRESS;
		if ((int8_t)first_background >= 0)
			tile_pages = background_tile_page_lists[(uint8_t)~background_inverted] + 0x10u;
		r12 = tile_page_list;
		page_number = OBJECT(current_object)->race_background.page_half * 5u * 2u + 1u;
		page_count = 4u;
		do
		{
			mem_write32(r12, byte_swap32(page_number) | mem_read32(tile_pages));
			page_number++;
			page_count--;
			tile_pages += 4u;
			r12 = r12 + 4u;
		} while (page_count != 0u);
		mem_write32(r12, 0xFFFFFFFFu);
		unpack_tile_pages(tile_page_list);
		mem_write8(OBJECT(current_object)->race_background.tile_bank_address, stage_tile_banks[SCREEN(screen)->first_stage]);
		SCREEN(screen)->background_changing = 0u;
		SCREEN(screen)->stage_background_change = 0u;
		SCREEN(screen)->backdrops_leaving = 0u;
		SCREEN(screen)->background_fading = 0xFFu;
		SCREEN(screen)->background_at_start = 0xFFu;
		first_stage = SCREEN(screen)->first_stage;
		race_background_start_objects(first_stage);
		palettes = no_background_palettes_ADDRESS;
		background = OBJECT(current_object)->race_background.background;
		if ((int8_t)background >= 0)
		{
			fades = background_palette_fades[background];
			race_background_fade_palettes(fades);
			palettes = background_palettes[OBJECT(current_object)->race_background.background];
		}
		queue_palette_list(palettes);
		OBJECT(current_object)->state++;
		first_stage_2 = SCREEN(screen)->first_stage;
		if (first_stage_2 == 6u)
			race_background_start_lower_animation();
		else
		{
			animation = 0u;
			if (first_stage_2 == 8u)
				race_background_start_upper_animation(animation);
			else
			{
				animation = 1u;
				if (first_stage_2 == 0x1Cu)
					race_background_start_upper_animation(animation);
			}
		}
	case 1:
		if (SCREEN(screen)->stage_background_change != 0u)
			L_017685 = true;
		else if (CAR(player_car)->new_branch != 0u)
		{
			CAR(player_car)->new_branch = 0u;
			L_017685 = true;
		}
		if (L_017685)
		{
			SCREEN(screen)->background_changing = 0xFFu;
			new_background = SCREEN(screen)->built_stage;
			if (SCREEN(screen)->stage_background_change == 0u)
			{
				new_background = new_background * 2u;
				new_background = (new_background & ~0xFFu) | (uint8_t)(new_background + CAR(player_car)->branch);
			}
			else
				new_background = (uint8_t)(new_background + 0x2Au);
			OBJECT(current_object)->race_background.background = new_background;
			tile_page_list = current_object + 0x2Cu;
			tile_pages = background_tile_page_lists[new_background];
			r12 = tile_page_list;
			page_half = OBJECT(current_object)->race_background.page_half;
			page_half = page_half ^ 2u;
			OBJECT(current_object)->race_background.page_half = page_half;
			page_half = page_half * 5u * 2u + 1u;
			page_count = 9u;
			do
			{
				mem_write32(r12, byte_swap32(page_half) | mem_read32(tile_pages));
				page_half++;
				page_count--;
				tile_pages += 4u;
				r12 = r12 + 4u;
			} while (page_count != 0u);
			mem_write32(r12, 0xFFFFFFFFu);
			queue_tile_pages(tile_page_list);
			page_word_set = OBJECT(current_object)->race_background.page_half * 2u;
			branch = SCREEN(screen)->stage_background_branch;
			if (SCREEN(screen)->stage_background_change == 0u)
				branch = CAR(player_car)->branch;
			OBJECT(current_object)->race_background.next_page_words = background_page_words_ADDRESS + ((page_word_set & ~0xFFu) | (uint8_t)(page_word_set + branch)) * 2u * 2u * 4u;
			OBJECT(current_object)->state++;
			OBJECT(current_object)->race_background.change_step = 0u;
		}
		break;
	case 2:
		view_turn = (state_routine & ~0xFFFFu) | CAR(player_car)->view_yaw;
		view_turn = (view_turn & ~0xFFFFu) | (uint16_t)(view_turn << 4);
		signed_turn = (int16_t)view_turn >> 4;
		zero = signed_turn == 0u;
		view_turn = (view_turn & ~0xFFFFu) | signed_turn;
		if ((int16_t)signed_turn < 0)
		{
			turn_back = 0u - view_turn;
			zero = turn_back == 0u;
			view_turn = (view_turn & ~0xFFFFu) | turn_back;
		}
		switch (OBJECT(current_object)->race_background.change_step)
		{
		case 0:
			if (zero == 0u)
			{
				SCREEN(screen)->background_fading = 0xFFu;
				background = OBJECT(current_object)->race_background.background;
				palettes = background_palettes[background];
				queue_palette_list(palettes);
				mem_write8(OBJECT(current_object)->race_background.tile_bank_address, background_tile_banks[background]);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 1:
			if ((int16_t)view_turn >= 0x2A0)
			{
				SCREEN(screen)->backdrops_leaving = 0xFFu;
				fades = background_leaving_fades[OBJECT(current_object)->race_background.background];
				start_palette_fades(fades);
				fades = background_road_palettes[OBJECT(current_object)->race_background.background];
				if (fades == 0u)
				{
					next_page_words = OBJECT(current_object)->race_background.next_page_words;
					mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
					OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
					OBJECT(current_object)->race_background.change_step++;
				}
				else
				{
					SCREEN(screen)->road_palette_fades = mem_read32(fades);
					fades += 4u;
					start_palette_fades(fades);
					next_page_words = OBJECT(current_object)->race_background.next_page_words;
					mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
					OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
					OBJECT(current_object)->race_background.change_step++;
				}
			}
			break;
		case 2:
			view_turn_kept = view_turn;
			SCREEN(screen)->backdrops_leaving = 0u;
			SCREEN(screen)->background_at_start = 0u;
			route_stage = SCREEN(screen)->route_stage;
			race_background_start_objects(route_stage);
			view_turn = view_turn_kept;
			OBJECT(current_object)->race_background.change_step++;
			if ((int16_t)view_turn <= 0x160)
			{
				fades = background_palette_fades[OBJECT(current_object)->race_background.background];
				race_background_fade_palettes(fades);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 3:
			if ((int16_t)view_turn <= 0x160)
			{
				fades = background_palette_fades[OBJECT(current_object)->race_background.background];
				race_background_fade_palettes(fades);
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 4:
			if ((int16_t)view_turn <= 0x60)
			{
				next_page_words = OBJECT(current_object)->race_background.next_page_words;
				mem_write32(OBJECT(current_object)->race_background.page_word_address, BACKGROUND_PAGE_WORD_STEPS(next_page_words)[0]);
				OBJECT(current_object)->race_background.next_page_words = next_page_words + 4u;
				OBJECT(current_object)->race_background.change_step++;
			}
			break;
		case 5:
			if (zero)
			{
				SCREEN(screen)->background_changing = 0u;
				SCREEN(screen)->stage_background_change = 0u;
				OBJECT(current_object)->state--;
				background_2 = OBJECT(current_object)->race_background.background;
				if (background_2 == 5u)
					race_background_start_lower_animation();
				else
				{
					animation = 0u;
					if (background_2 == 7u || background_2 == 8u)
						race_background_start_upper_animation(animation);
					else
					{
						animation = 2u;
						if (background_2 == 0x25u)
						{
							race_background_start_upper_animation(animation);
							scroll_x = CAR(player_car)->view_yaw;
							if (SCREEN(screen)->background_changing == 0u)
								scroll_x = scroll_x >> 2;
							mem_write16(OBJECT(current_object)->race_background.scroll_x_address, scroll_x - 0xA0u);
							slope_scroll_y = ((int16_t)sine_table[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->horizon_line - 1u;
							scroll_y = slope_scroll_y;
							if ((int16_t)slope_scroll_y >= 0)
							{
								scroll_y_limit = (uint16_t)(0u - SCREEN(screen)->road_top_line + 0x180u);
								if ((int16_t)scroll_y_limit < (int16_t)scroll_y)
									scroll_y = (uint16_t)scroll_y_limit;
							}
							else
								scroll_y = 0u;
							SCREEN(screen)->background_scroll_y = scroll_y;
							mem_write16(OBJECT(current_object)->race_background.scroll_y_address, scroll_y);
							return;
						}
						animation = 1u;
						if (background_2 == 0x26u)
						{
							race_background_start_upper_animation(animation);
							scroll_x = CAR(player_car)->view_yaw;
							if (SCREEN(screen)->background_changing == 0u)
								scroll_x = scroll_x >> 2;
							mem_write16(OBJECT(current_object)->race_background.scroll_x_address, scroll_x - 0xA0u);
							slope_scroll_y = ((int16_t)sine_table[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->horizon_line - 1u;
							scroll_y = slope_scroll_y;
							if ((int16_t)slope_scroll_y >= 0)
							{
								scroll_y_limit = (uint16_t)(0u - SCREEN(screen)->road_top_line + 0x180u);
								if ((int16_t)scroll_y_limit < (int16_t)scroll_y)
									scroll_y = (uint16_t)scroll_y_limit;
							}
							else
								scroll_y = 0u;
							SCREEN(screen)->background_scroll_y = scroll_y;
							mem_write16(OBJECT(current_object)->race_background.scroll_y_address, scroll_y);
							return;
						}
					}
				}
			}
			break;
		default:
			invalid_jump(race_background_change_step_jumps[OBJECT(current_object)->race_background.change_step]);
		}
		break;
	default:
		invalid_jump(race_background_state_jumps[state]);
	}
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	mem_write16(OBJECT(current_object)->race_background.scroll_x_address, scroll_x - 0xA0u);
	slope_scroll_y = ((int16_t)sine_table[(uint32_t)(int16_t)CAR(player_car)->road_pitch] >> 6) + SCREEN(screen)->horizon_line - 1u;
	scroll_y = slope_scroll_y;
	if ((int16_t)slope_scroll_y >= 0)
	{
		scroll_y_limit = (uint16_t)(0u - SCREEN(screen)->road_top_line + 0x180u);
		if ((int16_t)scroll_y_limit < (int16_t)scroll_y)
			scroll_y = (uint16_t)scroll_y_limit;
	}
	else
		scroll_y = 0u;
	SCREEN(screen)->background_scroll_y = scroll_y;
	mem_write16(OBJECT(current_object)->race_background.scroll_y_address, scroll_y);
}

/* Loads the tiles of the background's lower animation (queue_tile_pages with
   background_lower_animation_tile_pages by screen) and starts it (race_background_lower_animation). */
void race_background_start_lower_animation(void)
{
	uint32_t object = 0, tile_pages = 0;

	tile_pages = background_lower_animation_tile_pages[SCREEN(screen)->index];
	queue_tile_pages(tile_pages);
	object = start_object();
	OBJECT(object)->race_background_lower_animation.delay = 2u;
	OBJECT(object)->race_background_lower_animation.step = SCREEN(screen)->index;
	OBJECT(object)->race_background_lower_animation.page_word_address = OBJECT(current_object)->race_background.page_word_address;
	OBJECT(object)->handler = race_background_lower_animation_wait_ADDRESS;
}

/* Waits until the animation's tiles are loaded (screen unpack_state), then goes on to
   race_background_lower_animation_cycle. */
void race_background_lower_animation_wait(void)
{
	if (SCREEN(screen)->unpack_state != 0u)
		return;
	OBJECT(current_object)->handler = race_background_lower_animation_cycle_ADDRESS;
	race_background_lower_animation_cycle();
}

/* Every 2 frames (delay) puts the next of four page pairs (background_lower_animation_page_pairs; step
   goes two on, keeping to the screen's own) into the second half of the background's page word (the
   lower pages); ends when a change of background starts (screen background_changing). */
void race_background_lower_animation_cycle(void)
{
	uint32_t step = 0;
	uint8_t delay = 0;

	if (SCREEN(screen)->background_changing != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	delay = OBJECT(current_object)->race_background_lower_animation.delay;
	OBJECT(current_object)->race_background_lower_animation.delay = delay - 1u;
	if (delay == 1u)
	{
		OBJECT(current_object)->race_background_lower_animation.delay = 2u;
		step = OBJECT(current_object)->race_background_lower_animation.step;
		step = (uint8_t)(step + 2u);
		step = step & 7u;
		OBJECT(current_object)->race_background_lower_animation.step = step;
		mem_write16(OBJECT(current_object)->race_background_lower_animation.page_word_address + 2u, background_lower_animation_page_pairs[step]);
	}
}

/* Loads the tiles of upper animation R0 (queue_tile_pages with background_upper_animation_tile_pages by
   R0 and screen) and starts it (race_background_upper_animation). */
void race_background_start_upper_animation(uint32_t animation)
{
	uint32_t object = 0, tile_pages = 0;

	animation = animation * 2u;
	tile_pages = background_upper_animation_tile_pages[(animation & ~0xFFu) | (uint8_t)(animation + SCREEN(screen)->index)];
	queue_tile_pages(tile_pages);
	object = start_object();
	OBJECT(object)->race_background_upper_animation.delay = 3u;
	OBJECT(object)->race_background_upper_animation.step = SCREEN(screen)->index;
	OBJECT(object)->race_background_upper_animation.page_word_address = OBJECT(current_object)->race_background.page_word_address;
	OBJECT(object)->handler = race_background_upper_animation_wait_ADDRESS;
}

/* Waits until the animation's tiles are loaded (screen unpack_state), then goes on to
   race_background_upper_animation_cycle. */
void race_background_upper_animation_wait(void)
{
	if (SCREEN(screen)->unpack_state != 0u)
		return;
	OBJECT(current_object)->handler = race_background_upper_animation_cycle_ADDRESS;
	race_background_upper_animation_cycle();
}

/* Every 3 frames (delay) puts the next of three page pairs (background_upper_animation_page_pairs) into
   the first half of the background's page word (the upper pages: the schools of fish over the underwater
   tunnel swim); ends when a change of background starts (screen background_changing). */
void race_background_upper_animation_cycle(void)
{
	uint32_t step = 0;
	uint8_t delay = 0;

	if (SCREEN(screen)->background_changing != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	delay = OBJECT(current_object)->race_background_upper_animation.delay;
	OBJECT(current_object)->race_background_upper_animation.delay = delay - 1u;
	if (delay != 1u)
		return;
	OBJECT(current_object)->race_background_upper_animation.delay = 3u;
	step = OBJECT(current_object)->race_background_upper_animation.step;
	step = (uint8_t)(step + 2u);
	if ((int8_t)step >= 6)
		step = (uint8_t)(step - 6u);
	OBJECT(current_object)->race_background_upper_animation.step = step;
	mem_write16(OBJECT(current_object)->race_background_upper_animation.page_word_address, background_upper_animation_page_pairs[step]);
}

/* Sets up the screen's two tilemaps (0 and 2 for screen 1, 1 and 3 for screen 2; their register copies
   listed at background_register_lists by screen) from the 20-byte record background_setup (R19): their
   pages (two words), their tile banks (two bytes) and their priorities in the mixer (two bytes, the low
   four bits of mixer registers 22 and 26, 24 and 28 for screen 2; 0 is off), zoom 1:1, zoom centre and
   scrolls 0, the second tilemap's row scroll and row select off (video_registers[2]), the registers and
   the mixer to be sent (send_video_registers, screen flags bit 2); then queues the pages of the list at
   the record's +C (queue_tile_pages) and the palette record at +10 (queue_palette_list). Used by
   car_presentation, name_entry, route_map, car_select_run, entry_countdown_start and
   title_screen_update. */
void set_up_background(uint32_t background_setup)
{
	uint32_t background_registers = 0, mixer_address = 0, mixer_bits = 0, pages_address = 0, palette_list = 0, tile_banks_address = 0, tile_pages = 0;
	uint8_t tile_bank = 0, second_tile_bank = 0;
	uint32_t pages = 0, second_pages = 0;

	background_registers = background_register_lists[SCREEN(screen)->index];
	pages_address = BACKGROUND_REGISTERS(background_registers)->pages_address;
	pages = BACKGROUND_SETUP(background_setup)->pages;
	background_setup += 4u;
	mem_write32(pages_address, pages);
	second_pages = BACKGROUND_SETUP(background_setup - 0x4u)->second_pages;
	background_setup += 4u;
	mem_write32(pages_address + 8u, second_pages);
	background_registers += 4u;
	tile_banks_address = BACKGROUND_REGISTERS(background_registers - 0x4u)->tile_banks_address;
	tile_bank = BACKGROUND_SETUP(background_setup - 0x8u)->tile_bank;
	background_setup++;
	mem_write8(tile_banks_address, tile_bank);
	second_tile_bank = BACKGROUND_SETUP(background_setup - 0x9u)->second_tile_bank;
	background_setup++;
	mem_write8(tile_banks_address + 2u, second_tile_bank);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x8u)->zoom_x_address, 0x200u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0xCu)->zoom_y_address, 0x200u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x10u)->zoom_centre_x_address, 0u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x14u)->zoom_centre_y_address, 0u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x18u)->scroll_x_address, 0u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x1Cu)->scroll_y_address, 0u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x20u)->second_scroll_x_address, 0u);
	background_registers += 4u;
	mem_write16(BACKGROUND_REGISTERS(background_registers - 0x24u)->second_scroll_y_address, 0u);
	background_registers += 4u;
	video_registers[2] &= BACKGROUND_REGISTERS(background_registers - 0x28u)->video_mask;
	send_video_registers = 0xFFu;
	mixer_address = BACKGROUND_REGISTERS(background_registers - 0x28u)->mixer_address;
	mixer_bits = mem_read32(mixer_address);
	mixer_bits = (mixer_bits & ~0xFFFFu) | (mixer_bits & 0xFFF0u);
	mem_write32(mixer_address, (mixer_bits & ~0xFFu) | (uint8_t)(mixer_bits | BACKGROUND_SETUP(background_setup - 0xAu)->priority));
	mixer_address += 4u;
	mixer_bits = mem_read32(mixer_address);
	mixer_bits = (mixer_bits & ~0xFFFFu) | (mixer_bits & 0xFFF0u);
	background_setup++;
	mem_write32(mixer_address, (mixer_bits & ~0xFFu) | (uint8_t)(mixer_bits | BACKGROUND_SETUP(background_setup - 0xBu)->second_priority));
	SCREEN(screen)->flags |= 4u;
	background_setup++;
	tile_pages = BACKGROUND_SETUP(background_setup - 0xCu)->tile_pages;
	queue_tile_pages(tile_pages);
	palette_list = BACKGROUND_SETUP(background_setup - 0xCu)->palette_list;
	queue_palette_list(palette_list);
}

/* Starts the background objects of stage R0 (object_lists_181B8; R0 + 1F during a change within the
   stage, screen stage_background_change), each with its data (field 80) and this object (14). */
void race_background_start_objects(uint32_t stage)
{
	uint32_t object = 0, object_list = 0, objects_left = 0;
	uint32_t entry_count = 0;

	if (SCREEN(screen)->stage_background_change != 0u)
		stage = (stage & ~0xFFu) | (uint8_t)(stage + 0x1Fu);
	object_list = object_lists_181B8[stage];
	entry_count = mem_read32(object_list) >> 3;
	objects_left = entry_count;
	object_list += 4u;
	if (entry_count != 0u)
	{
		do
		{
			object = start_object();
			OBJECT(object)->handler = mem_read32(object_list);
			object_list += 4u;
			OBJECT(object)->unk_80 = mem_read32(object_list);
			OBJECT(object)->unk_14 = current_object;
			objects_left--;
			object_list += 4u;
		} while (objects_left != 0u);
	}
}

/* Cycles palettes of the stage's scenery (object_lists_181B8): after delay frames, every step_frames
   frames queues the next palette set of its list (palette_list; a negative entry goes back that many
   bytes) with queue_palette_list, for frames_left frames (or for good when it is 0); it waits while the
   stage's background is changing (screen background_fading) and ends when it changes again (running). */
void palette_cycle_update(void)
{
	uint16_t step_frames = 0;
	uint32_t palette_cycle_record = 0, palette_list = 0, palettes = 0;
	uint16_t delay = 0, frames_left = 0, step_countdown = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		palette_cycle_record = OBJECT(current_object)->palette_cycle.data;
		OBJECT(current_object)->palette_cycle.delay = PALETTE_CYCLE_RECORD(palette_cycle_record)->delay;
		palette_cycle_record += 2u;
		OBJECT(current_object)->palette_cycle.frames_left = PALETTE_CYCLE_RECORD(palette_cycle_record - 0x2u)->frames_left;
		palette_cycle_record += 2u;
		OBJECT(current_object)->palette_cycle.palette_list = PALETTE_CYCLE_RECORD(palette_cycle_record - 0x4u)->palette_list;
		step_frames = (uint16_t)PALETTE_CYCLE_RECORD(palette_cycle_record - 0x4u)->step_frames;
		OBJECT(current_object)->palette_cycle.step_frames = step_frames;
		OBJECT(current_object)->palette_cycle.step_countdown = step_frames;
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
		invalid_jump(palette_cycle_state_jumps[OBJECT(current_object)->state]);
	}
	switch (OBJECT(current_object)->palette_cycle.running)
	{
	case 0:
		if (SCREEN(screen)->background_fading != 0u)
			return;
		OBJECT(current_object)->palette_cycle.running++;
		break;
	case 1:
		break;
	default:
		invalid_jump(palette_cycle_running_jumps[OBJECT(current_object)->palette_cycle.running]);
	}
	if (SCREEN(screen)->background_fading != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	step_countdown = OBJECT(current_object)->palette_cycle.step_countdown;
	OBJECT(current_object)->palette_cycle.step_countdown = step_countdown - 1u;
	if (step_countdown != 1u)
		return;
	OBJECT(current_object)->palette_cycle.step_countdown = OBJECT(current_object)->palette_cycle.step_frames;
	palette_list = OBJECT(current_object)->palette_cycle.palette_list;
	palettes = PALETTE_CYCLE_STEPS(palette_list)[0];
	palette_list += 4u;
	if ((int32_t)palettes < 0)
	{
		palette_list += palettes;
		palettes = mem_read32(palette_list);
		palette_list += 4u;
	}
	OBJECT(current_object)->palette_cycle.palette_list = palette_list;
	queue_palette_list(palettes);
}

/* Fades the colours of the stage toward evening after a while (object_lists_181B8: Hawaii's sunset after
   900 frames, stage 26's dusk after 300 and its night after 600): waits count frames (not while the
   background changes, screen background_fading), then starts a palette fade (start_palette_fade) for
   each entry of its list (fade_list: from, to, the palette, the speed), keeps them in fades and their
   number in count, and goes on to sunset_fade_wait. */
void sunset_fade_start(void)
{
	uint8_t blocks = 0, period = 0;
	uint32_t fade = 0, fade_object = 0, fade_slot = 0, fades_left = 0, from = 0, sunset_fade_record = 0, to = 0;
	uint32_t count = 0, fade_count = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		sunset_fade_record = OBJECT(current_object)->sunset_fade.data;
		OBJECT(current_object)->sunset_fade.count = SUNSET_FADE_RECORD(sunset_fade_record)->delay;
		OBJECT(current_object)->sunset_fade.fade_list = SUNSET_FADE_RECORD(sunset_fade_record)->fade_list;
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
		invalid_jump(sunset_fade_state_jumps[OBJECT(current_object)->state]);
	}
	if (SCREEN(screen)->background_fading != 0u)
		return;
	fade = OBJECT(current_object)->sunset_fade.fade_list;
	fade_count = PALETTE_FADE_LIST(fade)->length / 0xCu;
	fades_left = fade_count;
	fade += 4u;
	if (fade_count == 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->sunset_fade.count = fades_left;
	fade_slot = current_object + 0x18u;
	do
	{
		from = PALETTE_FADE_ENTRY(fade)->from;
		to = PALETTE_FADE_ENTRY(fade)->to;
		fade += 8u;
		r12 = (r12 & ~0xFFFFu) | PALETTE_FADE_ENTRY(fade - 0x8u)->palette_offset;
		fade += 2u;
		blocks = PALETTE_FADE_ENTRY(fade - 0xAu)->blocks;
		fade++;
		period = PALETTE_FADE_ENTRY(fade - 0xBu)->period;
		fade++;
		fade_object = start_palette_fade(blocks, period, from, to);
		mem_write32(fade_slot, fade_object);
		fades_left--;
		fade_slot += 4u;
	} while (fades_left != 0u);
	OBJECT(current_object)->handler = sunset_fade_wait_ADDRESS;
}

/* Waits for its palette fades to finish, then ends; if the stage's background starts changing (screen
   background_fading) it stops them and ends. */
void sunset_fade_wait(void)
{
	uint32_t all_flags = 0, fade = 0, fade_flags = 0, fade_slot = 0, fades_left = 0;

	all_flags = 0u;
	fade_slot = current_object + 0x18u;
	fades_left = OBJECT(current_object)->sunset_fade.count;
	do
	{
		fade = mem_read32(fade_slot);
		fade_slot += 4u;
		if (fade != 0u)
		{
			fade_flags = mem_read32(fade);
			all_flags |= fade_flags;
			if ((fade_flags & 1u) == 0u)
				mem_write32(fade_slot - 4u, 0u);
		}
		fades_left--;
	} while (fades_left != 0u);
	if ((all_flags & 1u) == 0u)
		OBJECT(current_object)->flags = 0u;
	if (SCREEN(screen)->background_fading == 0u)
		return;
	fade_slot = current_object + 0x18u;
	fades_left = OBJECT(current_object)->sunset_fade.count;
	do
	{
		fade = mem_read32(fade_slot);
		fade_slot += 4u;
		if (fade != 0u)
			mem_write8(fade, 0u);
		fades_left--;
	} while (fades_left != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* Unused object handler: sets up a sprite on the background as backdrop_near_start does (from its data
   the animation, palette, panorama_x, height and half_width; draw_mode A, layer 1; straight to state 1
   when screen background_at_start is set); then backdrop_far_update. */
void backdrop_far_start(void)
{
	uint32_t animation = 0, backdrop_record = 0;

	if (SCREEN(screen)->background_at_start != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop.leaving = 0u;
	backdrop_record = OBJECT(current_object)->backdrop.data;
	animation = BACKDROP_RECORD(backdrop_record)->animation;
	backdrop_record += 4u;
	OBJECT(current_object)->palette = BACKDROP_RECORD(backdrop_record - 0x4u)->palette;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop.panorama_x = BACKDROP_RECORD(backdrop_record - 0x6u)->panorama_x;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop.height = BACKDROP_RECORD(backdrop_record - 0x8u)->height;
	OBJECT(current_object)->backdrop.half_width = BACKDROP_RECORD(backdrop_record - 0x8u)->half_width;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(animation);
	OBJECT(current_object)->handler = backdrop_far_update_ADDRESS;
	backdrop_far_update();
}

/* Only unused code reaches it (backdrop_far_start): keeps the sprite on the background's panorama at the
   panorama's own speed (x from the car's view_yaw, a quarter of it until screen background_changing is
   set, less panorama_x), y from the background's height (screen background_scroll_y) and height. By
   state: 0 waits until it is off the screen; 1 draw_object while on the screen; off it, it ends once
   leaving. */
void backdrop_far_update(void)
{
	uint32_t zero = 0;
	uint16_t scroll_x = 0;
	uint32_t minus_y = 0;

	OBJECT(current_object)->backdrop.leaving |= SCREEN(screen)->backdrops_leaving;
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	OBJECT(current_object)->x = 0u - (((scroll_x - OBJECT(current_object)->backdrop.panorama_x) & 0x3FFu) - 0x2A0u);
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
		invalid_jump(backdrop_far_state_jumps[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		minus_y = (uint16_t)(SCREEN(screen)->background_scroll_y - 0xFFu - OBJECT(current_object)->backdrop.height);
		OBJECT(current_object)->y = 0u - minus_y;
		minus_y = draw_object(minus_y);
		return;
	}
	if (OBJECT(current_object)->backdrop.leaving != 0u)
		OBJECT(current_object)->flags = 0u;
}

/* Sets up a sprite on the background a little nearer than the panorama (the hill of the Acropolis with
   the Parthenon, stages 14 and 19): from its data the animation, palette, panorama_x, height and
   half_width; draw_mode A, layer 1; straight to state 1 when the background is being set up (screen
   background_at_start); then backdrop_near_update. */
void backdrop_near_start(void)
{
	uint32_t animation = 0, backdrop_record = 0;

	if (SCREEN(screen)->background_at_start != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop.leaving = 0u;
	backdrop_record = OBJECT(current_object)->backdrop.data;
	animation = BACKDROP_RECORD(backdrop_record)->animation;
	backdrop_record += 4u;
	OBJECT(current_object)->palette = BACKDROP_RECORD(backdrop_record - 0x4u)->palette;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop.panorama_x = BACKDROP_RECORD(backdrop_record - 0x6u)->panorama_x;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop.height = BACKDROP_RECORD(backdrop_record - 0x8u)->height;
	OBJECT(current_object)->backdrop.half_width = BACKDROP_RECORD(backdrop_record - 0x8u)->half_width;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(animation);
	OBJECT(current_object)->handler = backdrop_near_update_ADDRESS;
	backdrop_near_update();
}

/* Keeps the sprite on the background, moving with its panorama a quarter faster (nearer): x from the
   car's heading (car view_yaw, a quarter of it until screen background_changing is set) less panorama_x,
   y from the background's height (screen background_scroll_y) and height, but not below screen
   road_top_line's line. By state: 0 waits until it is off the screen (so it does not pop up in view); 1
   draw_object while on the screen; off it, it ends once leaving. */
void backdrop_near_update(void)
{
	uint32_t zero = 0;
	uint16_t height = 0, scroll_x = 0, y_change = 0;
	uint32_t minus_y = 0;

	OBJECT(current_object)->backdrop.leaving |= SCREEN(screen)->backdrops_leaving;
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	scroll_x = (scroll_x - OBJECT(current_object)->backdrop.panorama_x) & 0x3FFu;
	OBJECT(current_object)->x = 0u - (scroll_x + (scroll_x >> 2) - 0x320u);
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
		invalid_jump(backdrop_near_state_jumps[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (OBJECT(current_object)->backdrop.leaving != 0u)
			OBJECT(current_object)->flags = 0u;
		return;
	}
	height = OBJECT(current_object)->backdrop.height;
	minus_y = (uint16_t)(SCREEN(screen)->background_scroll_y - 0xFFu - height);
	height -= SCREEN(screen)->road_top_line;
	if ((int16_t)height < (int16_t)minus_y)
		minus_y = height;
	y_change = (uint16_t)(0u - minus_y - OBJECT(current_object)->y);
	if ((int16_t)y_change > 1 || (int16_t)y_change < -1)
		OBJECT(current_object)->y = 0u - minus_y;
	minus_y = draw_object(minus_y);
}

/* Sets up a landmark on the horizon that grows as the car approaches (Tiananmen in stage 10, the castle
   of stages 16 and 17...): as backdrop_near_start, layer 2, and approach_start = the car's segment (car
   window_segment) less the distance in its data; then backdrop_landmark_update. */
void backdrop_landmark_start(void)
{
	uint32_t animation = 0, backdrop_record = 0;

	if (SCREEN(screen)->background_at_start != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop_landmark.leaving = 0u;
	backdrop_record = OBJECT(current_object)->backdrop_landmark.data;
	animation = BACKDROP_RECORD(backdrop_record)->animation;
	backdrop_record += 4u;
	OBJECT(current_object)->palette = BACKDROP_RECORD(backdrop_record - 0x4u)->palette;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_landmark.panorama_x = BACKDROP_RECORD(backdrop_record - 0x6u)->panorama_x;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_landmark.height = BACKDROP_RECORD(backdrop_record - 0x8u)->height;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_landmark.half_width = BACKDROP_RECORD(backdrop_record - 0xAu)->half_width;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 2u;
	animation_start(animation);
	OBJECT(current_object)->backdrop_landmark.approach_start = 0u - BACKDROP_RECORD(backdrop_record - 0xAu)->approach_distance + CAR(player_car)->window_segment;
	OBJECT(current_object)->handler = backdrop_landmark_update_ADDRESS;
	backdrop_landmark_update();
}

/* Keeps the landmark on the background's panorama (as backdrop_near_update, a quarter faster than the
   panorama) and grows it with the distance travelled: its scale (width_scale, height_scale) follows the
   sine of (car window_segment - approach_start) / 2 (sine_table) up to full size (100, state 2). By
   state: 0 waits until it is off the screen; 1 and 2 drawn while on the screen; off it, it ends once
   leaving. */
void backdrop_landmark_update(void)
{
	uint32_t zero = 0;
	uint16_t height = 0, scale = 0, scroll_x = 0, y_change = 0;
	uint32_t approach = 0, minus_y = 0;

	OBJECT(current_object)->backdrop_landmark.leaving |= SCREEN(screen)->backdrops_leaving;
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	scroll_x = (scroll_x - OBJECT(current_object)->backdrop_landmark.panorama_x) & 0x3FFu;
	OBJECT(current_object)->x = 0u - (scroll_x + (scroll_x >> 2) - 0x320u);
	switch (OBJECT(current_object)->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		OBJECT(current_object)->state++;
		approach = CAR(player_car)->window_segment;
		approach = (uint16_t)(approach - OBJECT(current_object)->backdrop_landmark.approach_start);
		scale = (uint16_t)sine_table[approach >> 1] >> 6;
		if ((int16_t)scale >= 0x100)
		{
			scale = 0x100u;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->width_scale = scale;
		OBJECT(current_object)->height_scale = scale;
		break;
	case 1:
		approach = CAR(player_car)->window_segment;
		approach = (uint16_t)(approach - OBJECT(current_object)->backdrop_landmark.approach_start);
		scale = (uint16_t)sine_table[approach >> 1] >> 6;
		if ((int16_t)scale >= 0x100)
		{
			scale = 0x100u;
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->width_scale = scale;
		OBJECT(current_object)->height_scale = scale;
		break;
	case 2:
		break;
	default:
		invalid_jump(backdrop_landmark_state_jumps[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (OBJECT(current_object)->backdrop_landmark.leaving != 0u)
			OBJECT(current_object)->flags = 0u;
		return;
	}
	height = OBJECT(current_object)->backdrop_landmark.height;
	minus_y = (uint16_t)(SCREEN(screen)->background_scroll_y - 0xFFu - height);
	height -= SCREEN(screen)->road_top_line;
	if ((int16_t)height < (int16_t)minus_y)
		minus_y = height;
	y_change = (uint16_t)(0u - minus_y - OBJECT(current_object)->y);
	if ((int16_t)y_change > 1 || (int16_t)y_change < -1)
		OBJECT(current_object)->y = 0u - minus_y;
	minus_y = draw_object(minus_y);
}

/* Unused object handler: sets up a sprite on the background as backdrop_far_start does, with
   other_palette 120; then backdrop_flashing_update. */
void backdrop_flashing_start(void)
{
	uint32_t animation = 0, backdrop_record = 0;

	if (SCREEN(screen)->background_at_start != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop_flashing.leaving = 0u;
	backdrop_record = OBJECT(current_object)->backdrop_flashing.data;
	animation = BACKDROP_RECORD(backdrop_record)->animation;
	backdrop_record += 4u;
	OBJECT(current_object)->palette = BACKDROP_RECORD(backdrop_record - 0x4u)->palette;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_flashing.panorama_x = BACKDROP_RECORD(backdrop_record - 0x6u)->panorama_x;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_flashing.height = BACKDROP_RECORD(backdrop_record - 0x8u)->height;
	OBJECT(current_object)->backdrop_flashing.half_width = BACKDROP_RECORD(backdrop_record - 0x8u)->half_width;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(animation);
	OBJECT(current_object)->backdrop_flashing.other_palette = 0x120u;
	OBJECT(current_object)->handler = backdrop_flashing_update_ADDRESS;
	backdrop_flashing_update();
}

/* Only unused code reaches it (backdrop_flashing_start): as backdrop_far_update (on the panorama at its
   speed), animated (animation_advance), and each frame its palette swaps with other_palette, so it
   flashes between its own palette and palette 120. */
void backdrop_flashing_update(void)
{
	uint32_t zero = 0;
	uint16_t palette = 0, scroll_x = 0;
	uint32_t minus_y = 0;
	uint16_t other_palette = 0;

	OBJECT(current_object)->backdrop_flashing.leaving |= SCREEN(screen)->backdrops_leaving;
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	OBJECT(current_object)->x = 0u - (((scroll_x - OBJECT(current_object)->backdrop_flashing.panorama_x) & 0x3FFu) - 0x2A0u);
	animation_advance(0u);
	other_palette = OBJECT(current_object)->backdrop_flashing.other_palette;
	palette = OBJECT(current_object)->palette;
	OBJECT(current_object)->palette = other_palette;
	OBJECT(current_object)->backdrop_flashing.other_palette = palette;
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
		invalid_jump(backdrop_flashing_state_jumps[OBJECT(current_object)->state]);
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		minus_y = (uint16_t)(SCREEN(screen)->background_scroll_y - 0xFFu - OBJECT(current_object)->backdrop_flashing.height);
		OBJECT(current_object)->y = 0u - minus_y;
		minus_y = draw_object(minus_y);
		return;
	}
	if (OBJECT(current_object)->backdrop_flashing.leaving != 0u)
		OBJECT(current_object)->flags = 0u;
}

/* Sets up a sprite on the background that is drawn every other frame, so it looks faint (the pyramids of
   stages 13 and 20, the spray of stage 4's falls): as backdrop_near_start, layer 1, frame_count 0; then
   backdrop_translucent_update. */
void backdrop_translucent_start(void)
{
	uint32_t animation = 0, backdrop_record = 0;

	if (SCREEN(screen)->background_at_start != 0u)
		OBJECT(current_object)->state++;
	OBJECT(current_object)->backdrop_translucent.leaving = 0u;
	backdrop_record = OBJECT(current_object)->backdrop_translucent.data;
	animation = BACKDROP_RECORD(backdrop_record)->animation;
	backdrop_record += 4u;
	OBJECT(current_object)->palette = BACKDROP_RECORD(backdrop_record - 0x4u)->palette;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_translucent.panorama_x = BACKDROP_RECORD(backdrop_record - 0x6u)->panorama_x;
	backdrop_record += 2u;
	OBJECT(current_object)->backdrop_translucent.height = BACKDROP_RECORD(backdrop_record - 0x8u)->height;
	OBJECT(current_object)->backdrop_translucent.half_width = BACKDROP_RECORD(backdrop_record - 0x8u)->half_width;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = 1u;
	animation_start(animation);
	OBJECT(current_object)->backdrop_translucent.frame_count = 0u;
	OBJECT(current_object)->handler = backdrop_translucent_update_ADDRESS;
	backdrop_translucent_update();
}

/* Keeps the sprite on the background's panorama at the panorama's own speed (as backdrop_far_update),
   animates it and draws it only on even frames (frame_count), so it looks faint. By state: 0 waits until
   it is off the screen; 1 drawn while on the screen; off it, it ends once leaving. */
void backdrop_translucent_update(void)
{
	uint32_t zero = 0;
	uint16_t scroll_x = 0;
	uint32_t minus_y = 0;

	OBJECT(current_object)->backdrop_translucent.leaving |= SCREEN(screen)->backdrops_leaving;
	scroll_x = CAR(player_car)->view_yaw;
	if (SCREEN(screen)->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	OBJECT(current_object)->x = 0u - (((scroll_x - OBJECT(current_object)->backdrop_translucent.panorama_x) & 0x3FFu) - 0x2A0u);
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
		invalid_jump(backdrop_translucent_state_jumps[OBJECT(current_object)->state]);
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
	minus_y = (uint16_t)(SCREEN(screen)->background_scroll_y - 0xFFu - OBJECT(current_object)->backdrop_translucent.height);
	OBJECT(current_object)->y = 0u - minus_y;
	minus_y = draw_object(minus_y);
}

/* Whether the background sprite is wholly off the screen (zero set; it clears flags bit 5 then): its x
   (field 30) more than half_width beyond the screen's edges (0..140). */
uint32_t backdrop_off_screen(void)
{
	uint32_t zero = 0;
	uint16_t centre_x = 0, half_screen = 0, half_width = 0, minus_right_edge = 0;

	half_screen = 0xA0u;
	centre_x = (uint16_t)(OBJECT(current_object)->x - 0xA0u);
	half_width = OBJECT(current_object)->backdrop.half_width;
	minus_right_edge = (uint16_t)(0u - centre_x - half_width);
	if ((int16_t)(centre_x - half_width) > 0xA0 || (int16_t)half_screen < (int16_t)minus_right_edge)
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
void start_palette_fades(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint32_t fades_left = 0, from = 0, to = 0;
	uint32_t fade_count = 0;

	fade_count = mem_read32(fade_list) / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count != 0u)
	{
		do
		{
			from = mem_read32(fade_list);
			to = mem_read32(fade_list + 4u);
			fade_list += 8u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(fade_list);
			fade_list += 2u;
			blocks = mem_read8(fade_list);
			fade_list++;
			period = mem_read8(fade_list);
			fade_list++;
			start_palette_fade(blocks, period, from, to);
			fades_left--;
		} while (fades_left != 0u);
	}
}

/* Starts the palette fades of list R19 backwards (start_palette_fade from each record's second palette
   to its first): the road's colours back after a change of page set. */
void road_surface_fade_back(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint32_t fades_left = 0, from = 0, to = 0;
	uint32_t fade_count = 0;

	fade_count = mem_read32(fade_list) / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count != 0u)
	{
		do
		{
			to = mem_read32(fade_list);
			fade_list += 4u;
			from = mem_read32(fade_list);
			fade_list += 4u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(fade_list);
			fade_list += 2u;
			blocks = mem_read8(fade_list);
			fade_list++;
			period = mem_read8(fade_list);
			fade_list++;
			start_palette_fade(blocks, period, from, to);
			fades_left--;
		} while (fades_left != 0u);
	}
}

/* Starts the palette fades of list R19 (start_palette_fade for each record) and a
   race_background_fade_watch that keeps them; with an empty list clears screen background_fading (the
   background's palettes are changing) at once. */
void race_background_fade_palettes(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint32_t fade_object = 0, fade_slot = 0, fades_left = 0, from = 0, object = 0, to = 0;
	uint32_t fade_count = 0;

	fade_count = mem_read32(fade_list) / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count == 0u)
	{
		SCREEN(screen)->background_fading = 0u;
		return;
	}
	object = start_object();
	OBJECT(object)->handler = race_background_fade_watch_update_ADDRESS;
	OBJECT(object)->race_background_fade_watch.background = current_object;
	OBJECT(object)->race_background_fade_watch.count = fades_left;
	fade_slot = object + 0x80u;
	do
	{
		from = mem_read32(fade_list);
		to = mem_read32(fade_list + 4u);
		fade_list += 8u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(fade_list);
		fade_list += 2u;
		blocks = mem_read8(fade_list);
		fade_list++;
		period = mem_read8(fade_list);
		fade_list++;
		fade_object = start_palette_fade(blocks, period, from, to);
		mem_write32(fade_slot, fade_object);
		fades_left--;
		fade_slot += 4u;
	} while (fades_left != 0u);
}
