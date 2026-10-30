/**
 *    Name:        race/hits.c
 *    Description: the angles of a vector, the hit boxes and the checks of the car against the road's objects and the traffic
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

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
	uint16_t height_2 = 0, pitch = 0;
	uint32_t distance = 0, height = 0, octant = 0, short_side = 0, yaw = 0, yaw_factor = 0;
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
		height_2 = 0u;
	}
	else
	{
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
		pitch = ((uint32_t)arctangents[divide_u32((height << 10) + (distance >> 1), distance)] * octant_angle_factors[octant] + octant_base_angles[octant]) & 0xFFFu;
		height_2 = (uint16_t)yaw;
	}
	*p_x = pitch;
	*p_y = height_2;
}

/* Builds the hit boxes of the race's cars at screen + 510: how many, then for each its player, race
   record and box (player_car_write_hit_box from its segment, model, place along and across; screen
   own_hit_box the screen's own), and the screen's car's box from its own fields at screen + 5B0. */
void player_car_build_hit_boxes(void)
{
	uint16_t road_x = 0, segment = 0, segment_fraction = 0;
	uint32_t hit_box = 0, model = 0, player = 0, racer_list = 0, racer_player = 0, racers_left = 0;
	struct race *racer_copy = NULL;

	racer_list = screen->host_record + 0x20u;
	racers_left = RACE(screen->host_record)->racer_count;
	player = screen->player;
	if ((screen->demo_flags & 0x100u) != 0u)
		racers_left = 1u;
	hit_box = address_of(screen) + 0x510u;
	SCREEN(hit_box - 0x510u)->hit_box_count = racers_left;
	hit_box += 4u;
	do
	{
		racer_player = RACE_ORDER_PLAYERS(racer_list)[0];
		racer_list++;
		if ((screen->demo_flags & 0x100u) != 0u || player == racer_player)
		{
			racer_player = player;
			screen->own_hit_box = hit_box;
		}
		CAR_HIT_BOX(hit_box)->player = racer_player;
		racer_copy = pointer_to(RACE_COPY_1 + (uint16_t)(racer_player << 7));
		segment_fraction = racer_copy->segment_fraction;
		road_x = (uint16_t)racer_copy->road_x;
		segment = racer_copy->segment;
		model = racer_copy->model;
		hit_box += 2u;
		hit_box = player_car_write_hit_box(segment, model, hit_box, address_of(racer_copy), segment_fraction, road_x);
		racers_left--;
	} while (racers_left != 0u);
	hit_box = address_of(screen) + 0x5B0u;
	segment_fraction = player_car->segment_fraction;
	road_x = (uint16_t)player_car->road_x;
	segment = player_car->segment;
	model = player_car->model;
	player_car_write_hit_box(segment, model, hit_box, address_of(racer_copy), segment_fraction, road_x);
}

/* Writes a car's hit box: its record, its first and last segment (4 on), and its box along and across
   from the model's size (car_hit_box_sizes). */
uint32_t player_car_write_hit_box(uint16_t segment, uint32_t model, uint32_t hit_box, uint32_t racer_copy, uint16_t segment_fraction, uint16_t road_x)
{
	const struct item_hit_box *box_size = NULL;
	uint16_t along_start = 0, along_end = 0, across_start = 0;

	CAR_HIT_BOX(hit_box - 0x2u)->record = racer_copy;
	hit_box += 4u;
	CAR_HIT_BOX(hit_box - 0x6u)->first_segment = segment;
	hit_box += 2u;
	CAR_HIT_BOX(hit_box - 0x8u)->last_segment = segment + 4u;
	box_size = rom_pointer(car_hit_box_sizes_ADDRESS + (uint8_t)(model * 2u) * 4u);
	along_start = box_size->along_start;
	hit_box += 2u;
	CAR_HIT_BOX(hit_box - 0xAu)->along_start = segment_fraction + along_start;
	along_end = box_size->along_end;
	hit_box += 2u;
	CAR_HIT_BOX(hit_box - 0xCu)->along_end = segment_fraction + along_end;
	across_start = box_size->across_start;
	hit_box += 2u;
	CAR_HIT_BOX(hit_box - 0xEu)->across_start = road_x + across_start;
	hit_box += 2u;
	CAR_HIT_BOX(hit_box - 0x10u)->across_end = road_x + box_size->across_end;
	hit_box += 2u;
	return hit_box;
}

/* road_row_hit_check with kind 3 (R27): a car that hits a fork sign is slowed for 15 frames, as off the
   road (car slowed_frames), instead of crashing. */
void fork_sign_hit_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	uint8_t hit_kind = 0;

	hit_kind = 3u;
	road_row_hit_check(hit_kind, p_along_distance, p_next_hit_box);
}

/* road_row_hit_check with kind 0 (R27): hitting an item crashes the car. Used by scenery,
   animated_scenery and flashing_scenery (and 012FBC). */
void road_row_crash_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	uint8_t hit_kind = 0;

	hit_kind = 0u;
	road_row_hit_check(hit_kind, p_along_distance, p_next_hit_box);
}

/* Whether the screen's car hits an item of the row, unless the car's object has flags bit 3 set: the
   car's box (the entry screen->own_hit_box points at: its segments at +6 and +8, along the road +A..+C
   in 128ths of a segment from its first, across +E..+10) against each item's hit_box (along from (item -
   the car's first segment) * 128 + position_fraction, across from the road's width * side + x_offset),
   for the items from first_position to end_position within the car's segments. On the first that
   overlaps: the car's object gets flags bit 2 and byte 6E = R27, the kind of hit (0 a crash, 3 slowed),
   and the car's hit_along and hit_across half the distances from the car's box to the item's (along,
   across). */
void road_row_hit_check(uint8_t hit_kind, uint32_t *p_along_distance, uint32_t *p_next_hit_box)
{
	uint32_t along_distance = *p_along_distance;
	uint32_t next_hit_box = *p_next_hit_box;
	uint16_t across_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, first_segment = 0, item_across = 0, item_along = 0, spacing = 0;
	uint32_t item_hit_box = 0, item_position = 0, segment_offset = 0;
	uint32_t object = 0;

	if ((OBJECT(player_car->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)current_object->road_row.first_position;
		next_hit_box = screen->own_hit_box + 6u;
		first_segment = CAR_HIT_BOX(next_hit_box - 0x6u)->first_segment;
		next_hit_box += 2u;
		car_last_segment = CAR_HIT_BOX(next_hit_box - 0x8u)->last_segment;
		next_hit_box += 2u;
		car_along_start = CAR_HIT_BOX(next_hit_box - 0xAu)->along_start;
		next_hit_box += 2u;
		car_along_end = CAR_HIT_BOX(next_hit_box - 0xCu)->along_end;
		next_hit_box += 2u;
		car_across_start = CAR_HIT_BOX(next_hit_box - 0xEu)->across_start;
		next_hit_box += 2u;
		car_across_end = CAR_HIT_BOX(next_hit_box - 0x10u)->across_end;
		next_hit_box += 2u;
		if ((int16_t)((first_segment - current_object->road_row.end_position) ^ (car_last_segment - item_position)) < 0)
		{
			spacing = (uint8_t)current_object->road_row.spacing;
			if (item_position < first_segment)
			{
				do
				{
					item_position = (uint16_t)(item_position + spacing);
				} while (item_position < first_segment);
			}
			while (item_position < car_last_segment)
			{
				segment_offset = item_position;
				item_along = (uint16_t)(item_position - first_segment);
				item_position = (uint16_t)(item_position + spacing);
				segment_offset = (uint16_t)(segment_offset << 4);
				segment_offset = segment_offset & 0x7FFu;
				item_hit_box = current_object->road_row.hit_box;
				item_across = (uint16_t)((uint32_t)current_object->side * ROAD_SEGMENT(address_of(screen) + 0x2000u + segment_offset * 2u)->half_width + current_object->x_offset);
				item_along = (uint16_t)(((uint32_t)item_along << 7) + current_object->position_fraction);
				along_distance = item_along;
				item_along += ITEM_HIT_BOX(item_hit_box)->along_start;
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance + ITEM_HIT_BOX(item_hit_box - 0x2u)->along_end);
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance - car_along_start);
				across_distance = (uint16_t)(item_across + ITEM_HIT_BOX(item_hit_box - 0x4u)->across_end - car_across_start);
				if ((int16_t)(((item_across + ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start - car_across_end) ^ across_distance) & ((item_along - car_along_end) ^ along_distance)) < 0)
				{
					object = player_car->object;
					OBJECT(object)->flags |= 4u;
					OBJECT(player_car->object)->player_car.hit_kind = hit_kind;
					along_distance = (uint16_t)((int16_t)along_distance >> 1);
					player_car->hit_along = along_distance;
					player_car->hit_across = (int16_t)across_distance >> 1;
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
	uint8_t hit_kind = 0;
	uint16_t across_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, first_segment = 0, item_across = 0, item_along = 0, mirrored_across_start = 0, spacing = 0;
	uint32_t item_hit_box = 0, item_position = 0, segment_offset = 0;
	uint32_t object = 0;

	hit_kind = 0u;
	if ((OBJECT(player_car->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)current_object->road_row.first_position;
		next_hit_box = screen->own_hit_box + 6u;
		first_segment = CAR_HIT_BOX(next_hit_box - 0x6u)->first_segment;
		next_hit_box += 2u;
		car_last_segment = CAR_HIT_BOX(next_hit_box - 0x8u)->last_segment;
		next_hit_box += 2u;
		car_along_start = CAR_HIT_BOX(next_hit_box - 0xAu)->along_start;
		next_hit_box += 2u;
		car_along_end = CAR_HIT_BOX(next_hit_box - 0xCu)->along_end;
		next_hit_box += 2u;
		car_across_start = CAR_HIT_BOX(next_hit_box - 0xEu)->across_start;
		next_hit_box += 2u;
		car_across_end = CAR_HIT_BOX(next_hit_box - 0x10u)->across_end;
		next_hit_box += 2u;
		if ((int16_t)((first_segment - current_object->road_row.end_position) ^ (car_last_segment - item_position)) < 0)
		{
			spacing = (uint8_t)current_object->road_row.spacing;
			if (item_position < first_segment)
			{
				do
				{
					item_position = (uint16_t)(item_position + spacing);
				} while (item_position < first_segment);
			}
			while (item_position < car_last_segment)
			{
				segment_offset = item_position;
				item_along = (uint16_t)(item_position - first_segment);
				item_position = (uint16_t)(item_position + spacing);
				segment_offset = (uint16_t)(segment_offset << 4);
				segment_offset = segment_offset & 0x7FFu;
				item_hit_box = current_object->road_row.hit_box;
				item_across = (uint16_t)((uint32_t)current_object->side * ROAD_SEGMENT(address_of(screen) + 0x2000u + segment_offset * 2u)->half_width + current_object->x_offset);
				item_along = (uint16_t)(((uint32_t)item_along << 7) + current_object->position_fraction);
				along_distance = item_along;
				across_distance = (uint16_t)item_across;
				item_along += ITEM_HIT_BOX(item_hit_box)->along_start;
				item_hit_box += 2u;
				along_distance = (uint16_t)(along_distance + ITEM_HIT_BOX(item_hit_box - 0x2u)->along_end);
				item_hit_box += 2u;
				item_across += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_start;
				across_distance += ITEM_HIT_BOX(item_hit_box - 0x4u)->across_end;
				mirrored_across_start = (uint16_t)(0u - across_distance);
				along_distance = (uint16_t)(along_distance - car_along_start);
				across_distance -= car_across_start;
				item_along = (uint16_t)((item_along - car_along_end) ^ along_distance);
				if ((int16_t)((((mirrored_across_start - car_across_end) ^ (0u - item_across - car_across_start)) & item_along) | (((item_across - car_across_end) ^ across_distance) & item_along)) < 0)
				{
					object = player_car->object;
					OBJECT(object)->flags |= 4u;
					OBJECT(player_car->object)->player_car.hit_kind = hit_kind;
					along_distance = (uint16_t)((int16_t)along_distance >> 1);
					player_car->hit_along = along_distance;
					player_car->hit_across = (int16_t)across_distance >> 1;
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
	uint8_t hit_kind = 0;
	uint16_t across_distance = 0, along_distance = 0, car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, item_across = 0, item_along = 0, mirrored_across = 0, mirrored_across_end = 0, spacing = 0;
	uint32_t item_position = 0, segment_offset = 0;
	struct car_hit_box *car_hit_box = NULL;
	const struct item_hit_box *item_hit_box = NULL;
	uint32_t object = 0;

	hit_kind = 0u;
	if ((OBJECT(player_car->object)->flags & 8u) == 0u)
	{
		item_position = (uint16_t)current_object->road_row.first_position;
		car_hit_box = pointer_to(screen->own_hit_box);
		r12 = (r12 & ~0xFFFFu) | car_hit_box->first_segment;
		car_last_segment = car_hit_box->last_segment;
		car_along_start = car_hit_box->along_start;
		car_along_end = car_hit_box->along_end;
		car_across_start = car_hit_box->across_start;
		car_across_end = car_hit_box->across_end;
		if ((int16_t)((r12 - current_object->road_row.end_position) ^ (car_last_segment - item_position)) >= 0)
			return;
		spacing = (uint8_t)current_object->road_row.spacing;
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
			item_hit_box = rom_pointer(current_object->road_row.hit_box);
			item_across = (uint16_t)((uint32_t)current_object->side * ROAD_SEGMENT(address_of(screen) + 0x2000u + (segment_offset & 0x7FFu) * 2u)->half_width + current_object->x_offset);
			item_along = (uint16_t)(((uint32_t)item_along << 7) + current_object->position_fraction);
			along_distance = (uint16_t)item_along;
			across_distance = (uint16_t)item_across;
			mirrored_across = (uint16_t)(0u - item_across);
			item_along += item_hit_box->along_start;
			along_distance += item_hit_box->along_end;
			item_across += item_hit_box->across_start;
			mirrored_across += item_hit_box->across_start;
			mirrored_across_end = (uint16_t)(0u - across_distance + item_hit_box->across_end);
			along_distance -= car_along_start;
			across_distance = (uint16_t)(across_distance + item_hit_box->across_end - car_across_start);
			item_along = (uint16_t)((item_along - car_along_end) ^ along_distance);
		} while ((int16_t)((((mirrored_across_end - car_across_start) ^ (mirrored_across - car_across_end)) & item_along) | (((item_across - car_across_end) ^ across_distance) & item_along)) >= 0);
		object = player_car->object;
		OBJECT(object)->flags |= 4u;
		OBJECT(player_car->object)->player_car.hit_kind = hit_kind;
		player_car->hit_along = (int16_t)along_distance >> 1;
		player_car->hit_across = (int16_t)across_distance >> 1;
	}
}

/* Whether the car hits another car of the race (the boxes of player_car_build_hit_boxes; not while
   either is immune, flags bit 3): a hit of kind 2 with that car's player in hit_other. */
void player_car_check_car_hits(void)
{
	uint16_t car_across_end = 0, car_across_start = 0, car_along_end = 0, car_along_start = 0, car_last_segment = 0, first_segment = 0, other_across = 0, other_along_end = 0, other_along_start = 0, other_player = 0, own_player = 0, segment_gap = 0;
	uint32_t boxes_left = 0, other_hit_box = 0;
	struct car_hit_box *car_hit_box = NULL;
	struct race *other_record = NULL;
	uint32_t object = 0;

	if ((OBJECT(player_car->object)->flags & 8u) != 0u)
		return;
	other_hit_box = address_of(screen) + 0x510u;
	car_hit_box = pointer_to(screen->own_hit_box);
	own_player = car_hit_box->player;
	first_segment = car_hit_box->first_segment;
	car_last_segment = car_hit_box->last_segment;
	car_along_start = (uint16_t)(car_hit_box->along_start + 1u);
	car_along_end = car_hit_box->along_end;
	car_across_start = car_hit_box->across_start;
	car_across_end = car_hit_box->across_end;
	boxes_left = SCREEN(other_hit_box - 0x510u)->hit_box_count;
	other_hit_box += 4u;
	if ((uint8_t)boxes_left < 2u)
		return;
	for (;;)
	{
		other_player = CAR_HIT_BOX(other_hit_box)->player;
		other_hit_box += 2u;
		if (own_player == other_player)
		{
			other_hit_box += 0x10u;
			boxes_left--;
			if (boxes_left == 0u)
				return;
			continue;
		}
		other_record = pointer_to(CAR_HIT_BOX(other_hit_box - 0x2u)->record);
		other_hit_box += 4u;
		if ((other_record->car_flags & 8u) != 0u)
		{
			other_hit_box += 0xCu;
			boxes_left--;
			if (boxes_left == 0u)
				return;
			continue;
		}
		other_along_start = CAR_HIT_BOX(other_hit_box - 0x6u)->first_segment;
		other_hit_box += 2u;
		segment_gap = (uint16_t)(first_segment - CAR_HIT_BOX(other_hit_box - 0x8u)->last_segment);
		other_hit_box += 2u;
		if ((int16_t)((car_last_segment - other_along_start) ^ segment_gap) >= 0)
		{
			other_hit_box += 8u;
			boxes_left--;
			if (boxes_left == 0u)
				break;
			continue;
		}
		other_along_start = (uint16_t)((uint32_t)(other_along_start - first_segment) << 7);
		other_along_end = (uint16_t)other_along_start;
		other_along_start += CAR_HIT_BOX(other_hit_box - 0xAu)->along_start;
		other_hit_box += 2u;
		other_along_end += CAR_HIT_BOX(other_hit_box - 0xCu)->along_end;
		other_hit_box += 2u;
		other_across = CAR_HIT_BOX(other_hit_box - 0xEu)->across_start;
		other_hit_box += 2u;
		other_across = (uint16_t)((other_across - car_across_end) ^ (CAR_HIT_BOX(other_hit_box - 0x10u)->across_end - car_across_start));
		other_hit_box += 2u;
		if ((int16_t)(other_across & ((other_along_start - car_along_end) ^ (other_along_end - car_along_start))) < 0)
		{
			object = player_car->object;
			OBJECT(object)->flags |= 4u;
			OBJECT(player_car->object)->player_car.hit_kind = 2u;
			OBJECT(player_car->object)->player_car.hit_other = other_player;
		}
		boxes_left--;
		if (boxes_left == 0u)
			return;
	}
}

/* Whether the screen's car hits this traffic object (negative set), unless the car's object has flags
   bit 3 set: the car's box (the entry screen->own_hit_box points at) against the object's hit_box (along
   the road from (position - the car's first segment) * 128 + position_fraction, across from the road's
   width * side + x_offset). On a hit the car's object gets flags bit 2, byte 6E = 1 (the kind of hit: a
   single object) and 68 = this object, whose speed and weight the crash (007DBD) reads. */
uint32_t traffic_hit_check(void)
{
	uint32_t negative = 0;
	uint16_t first_segment = 0, object_across = 0;
	uint32_t object_along = 0, segment_offset = 0;
	struct car_hit_box *car_hit_box = NULL;
	const struct item_hit_box *item_hit_box = NULL;
	struct object *object = NULL;
	uint16_t overlap_signs = 0;

	if ((OBJECT(player_car->object)->flags & 8u) != 0u)
	{
		negative = 0u;
		return negative;
	}
	object_along = (uint16_t)current_object->position;
	car_hit_box = pointer_to(screen->own_hit_box);
	first_segment = car_hit_box->first_segment;
	segment_offset = (uint16_t)(object_along << 4);
	item_hit_box = rom_pointer(current_object->traffic.hit_box);
	object_across = (uint16_t)((uint32_t)current_object->side * ROAD_SEGMENT(address_of(screen) + 0x2000u + (segment_offset & 0x7FFu) * 2u)->half_width + current_object->x_offset);
	object_along = (uint16_t)(((object_along - first_segment) << 7) + current_object->position_fraction);
	overlap_signs = ((object_across + item_hit_box->across_start - car_hit_box->across_end) ^ (object_across + item_hit_box->across_end - car_hit_box->across_start)) & ((object_along + item_hit_box->along_start - car_hit_box->along_end) ^ (object_along + item_hit_box->along_end - car_hit_box->along_start));
	negative = overlap_signs >> 15;
	if ((int16_t)overlap_signs < 0)
	{
		object = pointer_to(player_car->object);
		object->flags |= 4u;
		object->player_car.hit_kind = 1u;
		object->player_car.hit_other = address_of(current_object);
		negative = 1u;
	}
	return negative;
}
