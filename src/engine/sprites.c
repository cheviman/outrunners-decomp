/**
 *    Name:        engine/sprites.c
 *    Description: drawing an object: its draw modes, its sprites from its animation's frame, the rows of sprites along the road
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Draws the object's sprite, unless flags bit 1 is set or the screen's sprite list is full: field 3E
   picks the routine (draw_object_mode_jumps): some project the object's road position (1C) with its
   offsets (20, 24) and set fields 30 and 32 to where it lands and 3C to the road's depth layer there,
   some draw it at the screen position in 30 and 32, some copy a ready sprite entry from the address in
   field 38; then links it into the depth list of layer 3C (screen + 5000). */
uint16_t draw_object(uint32_t routine_then_layer)
{
	uint32_t depth_list = 0, layer_head = 0, left_caller = 0, sprite_entry = 0;

	if ((OBJECT(current_object)->flags & 2u) != 0u)
		return routine_then_layer;
	routine_then_layer = draw_object_mode_jumps[OBJECT(current_object)->draw_mode];
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
		return routine_then_layer;
	left_caller = 0u;
	switch (routine_then_layer)
	{
	case 0x00EF32u:
		draw_object_on_screen(&routine_then_layer, &sprite_entry);
		break;
	case 0x00F104u:
		sprite_entry = draw_object_entry(sprite_entry);
		break;
	case 0x00EB74u:
		draw_object_between_segments(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EEBFu:
		draw_object_at_car_window_segment_on_road(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EE37u:
		draw_object_at_car_window_segment(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EC1Bu:
		draw_object_from_road_edge(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F126u:
		sprite_entry = draw_object_command(sprite_entry);
		break;
	case 0x00ECCDu:
		draw_object_from_road_edge_unshifted(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F04Eu:
		draw_object_one_piece(&routine_then_layer, &sprite_entry);
		break;
	case 0x00EE75u:
		draw_object_at_segment(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F15Cu:
		draw_object_smoothed(&routine_then_layer, &sprite_entry);
		break;
	case 0x00ED82u:
		draw_object_from_road_edge_doubled(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	}
	if (left_caller != 0u)
		return routine_then_layer;
	routine_then_layer = OBJECT(current_object)->depth_layer;
	depth_list = screen + 0x5000u + routine_then_layer * 4u;
	if (DEPTH_LIST(depth_list)->head == 0u)
		DEPTH_LIST(depth_list)->tail = sprite_entry;
	r12 = SCREEN(screen)->sprite_list_last + 0x10u;
	layer_head = DEPTH_LIST(depth_list)->head;
	AS32(SPRITE_WORDS(sprite_entry)[0]) = layer_head;
	AS32(SPRITE_WORDS(sprite_entry)[6144]) = (uint16_t)(layer_head + 0x300u);
	SCREEN(screen)->sprite_list_last = sprite_entry;
	r12 = r12 >> 4;
	r12 = r12 | 0x8000u;
	DEPTH_LIST(depth_list)->head = r12;
	SCREEN(screen)->sprite_groups++;
	return routine_then_layer;
}

/* Draw mode 2: the object between the road's segment at its position (field 1C, counted from the view
   drawn: race_copy window_segment) and the next, by the fraction in field 28 (80ths), offset across and
   up by fields 20 and 24 scaled by each segment's scale (draw_object_at_scale). Its size, meant as the
   two segments' scales by the fraction, multiplies them by the second point's offsets instead (R4 and R5
   used twice): a slip in the game's code; nothing in the scenarios draws with it. */
void draw_object_between_segments(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t near_weight = 0, next_scale = 0, next_x = 0, next_y = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0, y = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)PROJECTED_SEGMENT(r12)->scale;
			if (next_scale != 0u)
			{
				x = (int32_t)(OBJECT(current_object)->x_offset * scale) >> 12;
				y = (int32_t)(OBJECT(current_object)->y_offset * scale) >> 12;
				next_x = (int32_t)(OBJECT(current_object)->x_offset * next_scale) >> 12;
				next_y = (int32_t)(OBJECT(current_object)->y_offset * next_scale) >> 12;
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				next_x = ((next_x & ~0xFFFFu) | (uint16_t)(next_x + PROJECTED_SEGMENT(r12)->x)) * position_fraction;
				x = (int32_t)(((x & ~0xFFFFu) | (uint16_t)(x + PROJECTED_SEGMENT(projected_segment)->x)) * near_weight + next_x) >> 7;
				next_y = ((next_y & ~0xFFFFu) | (uint16_t)(next_y + PROJECTED_SEGMENT(r12)->y)) * position_fraction;
				y = (int32_t)(((y & ~0xFFFFu) | (uint16_t)(y + PROJECTED_SEGMENT(projected_segment)->y)) * near_weight + next_y) >> 7;
				scale = (scale * next_x + next_scale * next_y) >> 7;
				*p_leftover = x;
				draw_object_at_scale((uint16_t)y, scale, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 5: the object on the road's surface between its segment (field 1C) and the next by the
   fraction in field 28, across at field 20 from the road's edge on side field 3F (the segment's half
   width times the side), its size the two segments' scales by the fraction shifted by scale_shift
   (draw_object_at_shifted_scale). */
void draw_object_from_road_edge(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)PROJECTED_SEGMENT(r12)->scale;
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)ROAD_SEGMENT(screen + 0x2000u + (ring_offset & 0xFFFu))->half_width * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + PROJECTED_SEGMENT(r12)->x) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)PROJECTED_SEGMENT(projected_segment)->y * near_weight + (int16_t)PROJECTED_SEGMENT(r12)->y * position_fraction) >> 7);
				scale = (scale * near_weight + next_scale * position_fraction) >> 7;
				*p_leftover = x;
				draw_object_at_shifted_scale(y, scale, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 7: as draw_object_from_road_edge, at the size of scale_shift 0 whatever the object's. */
void draw_object_from_road_edge_unshifted(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)PROJECTED_SEGMENT(r12)->scale;
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)ROAD_SEGMENT(screen + 0x2000u + (ring_offset & 0xFFFu))->half_width * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + PROJECTED_SEGMENT(r12)->x) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)PROJECTED_SEGMENT(projected_segment)->y * near_weight + (int16_t)PROJECTED_SEGMENT(r12)->y * position_fraction) >> 7);
				scale = (int32_t)(scale * near_weight + next_scale * position_fraction) >> 11;
				*p_leftover = x;
				draw_object_at_scale(y, scale, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode B (the traffic cars'): as draw_object_from_road_edge, at twice the size of
   draw_object_from_road_edge_unshifted (scale_shift 1) whatever the object's. */
void draw_object_from_road_edge_doubled(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)PROJECTED_SEGMENT(r12)->scale;
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)ROAD_SEGMENT(screen + 0x2000u + (ring_offset & 0xFFFu))->half_width * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + PROJECTED_SEGMENT(r12)->x) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)PROJECTED_SEGMENT(projected_segment)->y * near_weight + (int16_t)PROJECTED_SEGMENT(r12)->y * position_fraction) >> 7);
				scale = (int32_t)(scale * near_weight + next_scale * position_fraction) >> 10;
				*p_leftover = x;
				draw_object_at_scale(y, scale, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 4: as draw_object_at_segment, but the segment counted from the screen car's own
   window_segment instead of the view drawn (race_copy window_segment). */
void draw_object_at_car_window_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = CAR(player_car)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			x = (uint16_t)(((int32_t)(OBJECT(current_object)->x_offset * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x);
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->y);
			*p_leftover = x;
			draw_object_at_segment_scale(y, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 9: the object at the road's segment of its position (field 1C, counted from the view drawn:
   race_copy window_segment), offset across and up by fields 20 and 24 scaled by the segment's scale, its
   size that scale shifted by scale_shift; not drawn when the segment is behind or not projected. */
void draw_object_at_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			x = (uint16_t)(((int32_t)(OBJECT(current_object)->x_offset * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x);
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->y);
			scale = shift_by(scale, OBJECT(current_object)->scale_shift, 32) >> 4;
			*p_leftover = x;
			draw_object_at_scale(y, scale, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 3: as draw_object_at_car_window_segment, on the road's surface (no up offset). */
void draw_object_at_car_window_segment_on_road(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = CAR(player_car)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = PROJECTED_SEGMENT(projected_segment)->scale;
		if (scale != 0u)
		{
			x = (uint16_t)(((int32_t)(OBJECT(current_object)->x_offset * scale) >> 12) + PROJECTED_SEGMENT(projected_segment)->x);
			y = PROJECTED_SEGMENT(projected_segment)->y;
			*p_leftover = x;
			draw_object_at_segment_scale(y, projected_segment, p_leftover, p_sprite_entry, p_left_caller);
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draws the object at R0, R1 at the projected segment R11's own scale (draw_object_at_shifted_scale). */
void draw_object_at_segment_scale(uint16_t y, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;
	uint32_t scale = 0;

	scale = PROJECTED_SEGMENT(projected_segment)->scale;
	draw_object_at_shifted_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Draws the object at R0, R1 at the scale R2 shifted by its scale_shift (and >> 4)
   (draw_object_at_scale). */
void draw_object_at_shifted_scale(uint16_t y, uint32_t scale, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;

	scale = shift_by(scale, OBJECT(current_object)->scale_shift, 32) >> 4;
	draw_object_at_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Places a road object projected at R0, R1 with size R2 (both scales): x and y; off the screen (x beyond
   1A0 or -1A0, y beyond 80 or -160) back to draw_object's caller undrawn; the depth_layer of the
   projected segment R11 (+8), then put_object_sprites. */
void draw_object_at_scale(uint16_t y, uint32_t scale, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;
	uint32_t width_scale = 0;

	OBJECT(current_object)->x = x;
	OBJECT(current_object)->y = y;
	if ((int16_t)x > 0x1A0 || (int16_t)x < -0x1A0 || (int16_t)y > 0x80 || (int16_t)y < -0x160)
		*p_left_caller = 1u;
	else
	{
		width_scale = scale;
		OBJECT(current_object)->depth_layer = PROJECTED_SEGMENT(projected_segment)->depth_layer;
		put_object_sprites(y, scale, width_scale, &x, p_sprite_entry);
	}
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Draw mode 0: the object at x, y on the screen, less the screen's scroll (screen sprite_origin_x,
   sprite_origin_y), at its width_scale and height_scale (put_object_sprites). */
void draw_object_on_screen(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint16_t y = 0;
	uint32_t height_scale = 0, width_scale = 0, x = 0;

	x = (uint16_t)(OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x);
	y = (uint16_t)(OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y);
	height_scale = OBJECT(current_object)->height_scale;
	width_scale = OBJECT(current_object)->width_scale;
	put_object_sprites(y, height_scale, width_scale, &x, p_sprite_entry);
	*p_leftover = (*p_leftover & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Writes the object's frame (animation, frame) into the screen's sprite list at R10 and its copy at +
   3000, a sprite of 8 halfwords for each piece: its word flipped by flags bits 6-7, its size from
   0x122B74 by size_step scaled by R2 (height) and R3 (width), placed at R0, R1 plus its offsets scaled
   the same way (signs flipped by the flags: sprite_flip_x_signs, sprite_flip_y_signs), its palette word
   from sprite_palettes by palette; R10 past them. */
void put_object_sprites(uint16_t y, uint32_t height_scale, uint32_t width_scale, uint32_t *p_x, uint32_t *p_sprite_entry)
{
	uint16_t x = (uint16_t)*p_x;
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t attributes = 0, flip_bits = 0, frame_x = 0, frame_y = 0, sprite_height = 0, sprite_palette_word = 0, sprite_rom_offset = 0, sprite_source_size = 0, sprite_width = 0, sprite_x = 0, sprite_y = 0;
	uint32_t flip_index = 0, frame_height_scale = 0, frame_width_scale = 0, palette_words = 0, piece_count = 0, signed_height_scale = 0, signed_width_scale = 0, size_step_offset = 0;
	uint16_t source_size = 0, rom_offset = 0, palette_word = 0, height = 0, height_bits = 0, width = 0, width_bits = 0;

	frame_x = (uint16_t)x;
	frame_y = (uint16_t)y;
	frame_height_scale = height_scale;
	frame_width_scale = width_scale;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame];
	palette_words = sprite_palettes[OBJECT(current_object)->palette];
	flip_bits = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u)) & 0xC0u;
	flip_index = flip_bits;
	flip_index = flip_index >> 5;
	signed_width_scale = frame_width_scale * sprite_flip_x_signs[flip_index];
	signed_height_scale = frame_height_scale * sprite_flip_y_signs[flip_index];
	piece_count = SPRITE_FRAME(r12)->piece_count;
	r12 += 2u;
	do
	{
		attributes = SPRITE_PIECE(r12)->attributes ^ flip_bits;
		size_step_offset = OBJECT(current_object)->size_step * 3u;
		r12 += 2u;
		r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
		source_size = SPRITE_SIZE_STEP(r14)->source_size;
		r14 += 2u;
		sprite_source_size = source_size;
		rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
		r14 += 2u;
		sprite_rom_offset = rom_offset;
		palette_word = SPRITE_PALETTE_WORDS(palette_words)[0];
		palette_words += 2u;
		sprite_palette_word = palette_word;
		height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
		r14 += 2u;
		height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
		r14 += 2u;
		sprite_height = (((height * frame_height_scale) >> 8) & 0x3FFu) | height_bits;
		width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
		r14 += 2u;
		width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
		r14 += 2u;
		sprite_width = (((width * frame_width_scale) >> 8) & 0x3FFu) | width_bits | SCREEN(screen)->sprite_screen_bit;
		r12 += 2u;
		sprite_y = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset * signed_height_scale) >> 8) + frame_y);
		r12 += 2u;
		sprite_x = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset * signed_width_scale) >> 8) + frame_x);
		SPRITE_WORDS(sprite_entry)[6144] = attributes;
		SPRITE_WORDS(sprite_entry)[0] = attributes;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_source_size;
		SPRITE_WORDS(sprite_entry)[0] = sprite_source_size;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_height;
		SPRITE_WORDS(sprite_entry)[0] = sprite_height;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_width;
		SPRITE_WORDS(sprite_entry)[0] = sprite_width;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_y;
		SPRITE_WORDS(sprite_entry)[0] = sprite_y;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_x;
		SPRITE_WORDS(sprite_entry)[0] = sprite_x;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_rom_offset;
		SPRITE_WORDS(sprite_entry)[0] = sprite_rom_offset;
		sprite_entry += 2u;
		SPRITE_WORDS(sprite_entry)[6144] = sprite_palette_word;
		SPRITE_WORDS(sprite_entry)[0] = sprite_palette_word;
		sprite_entry += 2u;
		piece_count--;
		r12 += 2u;
	} while (piece_count != 0u);
	*p_x = (*p_x & 0xFFFF0000u) | attributes;
	*p_sprite_entry = sprite_entry;
}

/* Draw mode 8: the first piece of the animation's frame at its own size (no zoom, no flip), at x, y on
   the screen (less sprite_origin_x, sprite_origin_y), with the first word of its palette
   (sprite_palettes), into the sprite list and its halfway copy alike. */
void draw_object_one_piece(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t attributes = 0, palette_word = 0, rom_offset = 0, source_size = 0, sprite_height = 0, sprite_width = 0, sprite_x = 0, sprite_y = 0;
	uint32_t size_step_offset = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	attributes = SPRITE_PIECE(r12)->attributes;
	size_step_offset = OBJECT(current_object)->size_step * 3u;
	r12 += 2u;
	r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 += 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	palette_word = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0];
	r14 += 2u;
	sprite_height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 += 2u;
	sprite_height |= SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 += 2u;
	sprite_width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 += 2u;
	sprite_width = sprite_width | SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits | SCREEN(screen)->sprite_screen_bit;
	r12 += 2u;
	sprite_y = (uint16_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset + (OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y));
	r12 += 2u;
	sprite_x = (uint16_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset + (OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x));
	SPRITE_WORDS(sprite_entry)[6144] = attributes;
	SPRITE_WORDS(sprite_entry)[0] = attributes;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = source_size;
	SPRITE_WORDS(sprite_entry)[0] = source_size;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_height;
	SPRITE_WORDS(sprite_entry)[0] = sprite_height;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_width;
	SPRITE_WORDS(sprite_entry)[0] = sprite_width;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_y;
	SPRITE_WORDS(sprite_entry)[0] = sprite_y;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_x;
	SPRITE_WORDS(sprite_entry)[0] = sprite_x;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = rom_offset;
	SPRITE_WORDS(sprite_entry)[0] = rom_offset;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = palette_word;
	SPRITE_WORDS(sprite_entry)[0] = palette_word;
	sprite_entry += 2u;
	r12 += 2u;
	r14 += 2u;
	*p_leftover = (*p_leftover & 0xFFFF0000u) | attributes;
	*p_sprite_entry = sprite_entry;
}

/* Draw mode 1: copies a ready 16-byte sprite entry, from the address in fields 38-3B (animation and
   palette read as one 32-bit address), into the sprite list and its halfway copy, with the screen's bit
   in its fourth word (sprite_screen_bit). Not run in the scenarios. */
uint32_t draw_object_entry(uint32_t sprite_entry)
{
	move_halfwords(sprite_entry, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	move_halfwords(sprite_entry + 0x3000u, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	SPRITE_WORDS(sprite_entry)[3] |= SCREEN(screen)->sprite_screen_bit;
	SPRITE_WORDS(sprite_entry)[6147] |= SCREEN(screen)->sprite_screen_bit;
	sprite_entry += 0x10u;
	return sprite_entry;
}

/* Draw mode 6: copies a ready 16-byte entry from the address in fields 38-3B into the sprite list and
   its halfway copy as it is, without the screen's bit: for list commands such as crest_clip_update's
   clip. */
uint32_t draw_object_command(uint32_t sprite_entry)
{
	move_halfwords(sprite_entry, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	move_halfwords(sprite_entry + 0x3000u, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	sprite_entry += 0x10u;
	return sprite_entry;
}

/* Draw mode A: every piece of the animation's frame at x, y on the screen (less sprite_origin_x,
   sprite_origin_y), zoomed by width_scale and height_scale, flipped by flags bits 6-7 (with bit 7 the
   pieces' offsets are negated, smoothed_flip_signs), each with its palette word; the halfway copy
   (+3000) is drawn from the averages of this frame's x, y, width_scale and height_scale and the last
   frame's, kept in the ring of sprite places (next_sprite_place, 16 bytes a record; the object's field
   50 points at its record) when flags bit 5 says it was drawn the frame before. Keeps this frame's
   values in a new record and sets flags bit 5. */
void draw_object_smoothed(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t entry_word = 0, flip_bits = 0, halfway_width = 0, halfway_x = 0, halfway_y = 0, piece_height_bits = 0, piece_width_bits = 0, sprite_rom_offset = 0, sprite_screen_bit = 0, sprite_source_size = 0;
	uint32_t copy_entry = 0, flip_index = 0, halfway_height_scale = 0, halfway_width_scale = 0, height_scale = 0, offset_sign = 0, offset_x = 0, offset_y = 0, palette_words = 0, piece_height = 0, piece_width = 0, previous_place = 0, size_step_offset = 0, sprite_place = 0, width_scale = 0, x = 0, y = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0, palette_word = 0;
	uint32_t flags = 0, flags_long = 0;

	previous_place = OBJECT(current_object)->previous_place;
	sprite_place = SCREEN(screen)->next_sprite_place;
	x = (uint32_t)(int16_t)OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x;
	y = (uint32_t)(int16_t)OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y;
	width_scale = OBJECT(current_object)->width_scale;
	height_scale = OBJECT(current_object)->height_scale;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->flags_upper << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->flags_upper = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) == 0u)
	{
		halfway_x = (uint16_t)x;
		halfway_y = (uint16_t)y;
		halfway_width_scale = width_scale;
		halfway_height_scale = height_scale;
	}
	else
	{
		halfway_height_scale = SPRITE_PLACE(previous_place)->height;
		previous_place += 2u;
		halfway_width_scale = SPRITE_PLACE(previous_place - 0x2u)->width;
		previous_place += 2u;
		halfway_x = (uint16_t)((int32_t)((int16_t)SPRITE_PLACE(previous_place - 0x4u)->x + x) >> 1);
		halfway_y = (uint16_t)((int32_t)((int16_t)SPRITE_PLACE(previous_place - 0x4u)->y + y) >> 1);
		halfway_width_scale = (halfway_width_scale + width_scale) >> 1;
		halfway_height_scale = (halfway_height_scale + height_scale) >> 1;
	}
	OBJECT(current_object)->previous_place = sprite_place;
	SPRITE_PLACE(sprite_place)->height = height_scale;
	sprite_place += 2u;
	SPRITE_PLACE(sprite_place - 0x2u)->width = width_scale;
	sprite_place += 2u;
	SPRITE_PLACE(sprite_place - 0x4u)->y = y;
	sprite_place += 2u;
	SPRITE_PLACE(sprite_place - 0x6u)->x = x;
	sprite_place += 0xAu;
	SCREEN(screen)->next_sprite_place = LINKED(sprite_place, sprite_place & 0xEFFFu);
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame];
	palette_words = sprite_palettes[OBJECT(current_object)->palette];
	flip_bits = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u)) & 0xC0u;
	flip_index = flip_bits;
	offset_sign = smoothed_flip_signs[flip_index >> 5];
	previous_place = SPRITE_FRAME(r12)->piece_count;
	copy_entry = sprite_entry + 0x3000u;
	r12 += 2u;
	do
	{
		entry_word = SPRITE_PIECE(r12)->attributes ^ flip_bits;
		SPRITE_WORDS(sprite_entry)[0] = entry_word;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = entry_word;
		copy_entry += 2u;
		size_step_offset = OBJECT(current_object)->size_step * 3u;
		r12 += 2u;
		r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
		source_size = SPRITE_SIZE_STEP(r14)->source_size;
		r14 += 2u;
		sprite_source_size = source_size;
		SPRITE_WORDS(sprite_entry)[0] = sprite_source_size;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = sprite_source_size;
		copy_entry += 2u;
		rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
		r14 += 2u;
		sprite_rom_offset = rom_offset;
		height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
		r14 += 2u;
		piece_height = height;
		height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
		r14 += 2u;
		piece_height_bits = height_bits;
		SPRITE_WORDS(sprite_entry)[0] = (((piece_height * height_scale) >> 8) & 0x3FFu) | piece_height_bits;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = (((piece_height * halfway_height_scale) >> 8) & 0x3FFu) | piece_height_bits;
		copy_entry += 2u;
		width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
		r14 += 2u;
		piece_width = width;
		halfway_width = ((piece_width * halfway_width_scale) >> 8) & 0x3FFu;
		width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
		r14 += 2u;
		piece_width_bits = width_bits;
		piece_width = (((piece_width * width_scale) >> 8) & 0x3FFu) | piece_width_bits;
		halfway_width |= piece_width_bits;
		sprite_screen_bit = SCREEN(screen)->sprite_screen_bit;
		SPRITE_WORDS(sprite_entry)[0] = piece_width | sprite_screen_bit;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = halfway_width | sprite_screen_bit;
		copy_entry += 2u;
		r12 += 2u;
		offset_y = SPRITE_PIECE(r12 - 0x4u)->y_offset * offset_sign;
		SPRITE_WORDS(sprite_entry)[0] = ((int32_t)(offset_y * height_scale) >> 8) + y;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = (uint32_t)((int32_t)(offset_y * halfway_height_scale) >> 8) + halfway_y;
		copy_entry += 2u;
		r12 += 2u;
		offset_x = SPRITE_PIECE(r12 - 0x6u)->x_offset * offset_sign;
		SPRITE_WORDS(sprite_entry)[0] = ((int32_t)(offset_x * width_scale) >> 8) + x;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = (uint32_t)((int32_t)(offset_x * halfway_width_scale) >> 8) + halfway_x;
		copy_entry += 2u;
		SPRITE_WORDS(sprite_entry)[0] = sprite_rom_offset;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = sprite_rom_offset;
		copy_entry += 2u;
		palette_word = SPRITE_PALETTE_WORDS(palette_words)[0];
		palette_words += 2u;
		entry_word = palette_word;
		SPRITE_WORDS(sprite_entry)[0] = entry_word;
		sprite_entry += 2u;
		SPRITE_WORDS(copy_entry)[0] = entry_word;
		copy_entry += 2u;
		previous_place--;
		r12 += 2u;
	} while (previous_place != 0u);
	*p_leftover = x;
	*p_sprite_entry = sprite_entry;
}

/* Steps the object's animation: nothing while frame_time is 0; when frame_countdown runs out (back to
   frame_time), the next frame. A frame's entry in the record (32 bits at +4 + frame * 4) is the address
   of its data when positive, else an order: 0 holds the frame before (frame_time 0), -1 goes back to
   frame 0, -2 starts the animation whose number is the next entry. */
uint32_t animation_advance(uint32_t leftover)
{
	uint32_t frame = 0, next_animation = 0, order_routine = 0, sprite_animation = 0;
	uint8_t frame_countdown = 0;
	uint32_t order = 0;

	if (OBJECT(current_object)->frame_time == 0u)
		return leftover;
	frame_countdown = OBJECT(current_object)->frame_countdown;
	OBJECT(current_object)->frame_countdown = frame_countdown - 1u;
	if ((int8_t)frame_countdown > 1)
		return leftover;
	OBJECT(current_object)->frame_countdown = OBJECT(current_object)->frame_time;
	OBJECT(current_object)->frame++;
	frame = OBJECT(current_object)->frame;
	sprite_animation = sprite_animations[OBJECT(current_object)->animation];
	if (SPRITE_ANIMATION(sprite_animation)->frames[(int32_t)frame] > 0)
		return leftover;
	order = 0u - SPRITE_ANIMATION(sprite_animation)->frames[(int32_t)frame];
	order_routine = animation_order_jumps[order];
	switch (order)
	{
	case 0:
		OBJECT(current_object)->frame_time = 0u;
		OBJECT(current_object)->frame--;
		return order_routine;
	case 2:
		next_animation = SPRITE_ANIMATION(sprite_animation)->frames[(int32_t)(frame + 1u)];
		animation_start(next_animation);
		return order_routine;
	case 1:
		OBJECT(current_object)->frame = 0u;
		return order_routine;
	}
}

/* Starts animation R9 (sprite_animations[R9] is its record) at its first frame: frame_time and
   frame_countdown from the record's first byte, size_step 0, and fields 34 and 36 to 100 (the width and
   height zoom of the screen-position drawing, draw_object_on_screen: 100 the sprite's own size). */
void animation_start(uint32_t animation)
{
	uint8_t frame_time = 0;

	OBJECT(current_object)->animation = animation;
	frame_time = SPRITE_ANIMATION(sprite_animations[(int32_t)animation])->frame_time;
	OBJECT(current_object)->frame_time = frame_time;
	OBJECT(current_object)->frame_countdown = frame_time;
	OBJECT(current_object)->size_step = 0u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->frame = 0u;
}

/* Finishes the screen's sprite list in the vertical blank (int_vector_40): picks a record of
   sprite_list_switches by the screen (index) and sprite_list_buffers; when groups were drawn
   (sprite_groups), chains the depth lists from layer 0 to 415 (depth_lists: the list's start entry jumps
   to the first layer's newest group, each layer's tail to the next layer's head; the heads cleared), in
   the list and its halfway copy, and ends the chain with the record's word (a jump on to sprite entry 3,
   screen 2's list, after screen 1's; entry 4, the end, after screen 2's); points sprite RAM entries 2
   and 3 (the jumps to each screen's list) at the halfway copy of the list just finished and at the other
   screen's finished list; then the next buffer (sprite_list_buffers, sprite_list_last,
   sprite_list_limit) and sprite_groups 0. */
void finish_sprite_list(void)
{
	uint16_t copy_distance = 0, depth_list_size = 0;
	uint32_t chain_tail = 0, cleared_head = 0, depth_list = 0, empty_head = 0, layer_head = 0, layer_octets = 0, sprite_list_switch = 0, switch_index = 0;
	uint16_t new_head = 0, new_head_2 = 0, new_head_3 = 0, new_head_4 = 0, new_head_5 = 0, new_head_6 = 0, new_head_7 = 0, new_head_8 = 0;

	switch_index = SCREEN(screen)->index;
	switch_index = (uint8_t)(switch_index * 2u);
	switch_index = switch_index | (uint8_t)sprite_list_buffers;
	switch_index = (uint8_t)(switch_index * 2u);
	sprite_list_switch = sprite_list_switches[switch_index | (uint8_t)(sprite_list_buffers >> 8u)];
	chain_tail = SPRITE_LIST_SWITCH(sprite_list_switch)->list;
	sprite_list_switch += 4u;
	if (SCREEN(screen)->sprite_groups != 0u)
	{
		depth_list = screen + 0x5000u;
		layer_octets = 0x34u;
		empty_head = 0u;
		cleared_head = 0u;
		copy_distance = 0x300u;
		depth_list_size = 4u;
		do
		{
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_2 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_2;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_3 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_3;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_4 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_4;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_5 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_5;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_6 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_6;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_7 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_7;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			if (DEPTH_LIST(depth_list)->head != 0u)
			{
				new_head_8 = cleared_head;
				layer_head = DEPTH_LIST(depth_list)->head;
				DEPTH_LIST(depth_list)->head = new_head_8;
				AS32(SPRITE_WORDS(chain_tail)[0]) = layer_head;
				AS32(SPRITE_WORDS(chain_tail)[6144]) = (uint16_t)(layer_head + copy_distance);
				chain_tail = LINKED(chain_tail, DEPTH_LIST(depth_list)->tail);
				cleared_head = empty_head;
			}
			depth_list = LINKED(depth_list, (uint16_t)(depth_list + depth_list_size));
			layer_octets--;
		} while (layer_octets != 0u);
	}
	SPRITE_WORDS(chain_tail)[0] = SPRITE_LIST_SWITCH(sprite_list_switch - 0x4u)->end_word;
	SPRITE_WORDS(chain_tail)[6144] = SPRITE_LIST_SWITCH(sprite_list_switch - 0x4u)->end_word;
	sprite_list_switch += 2u;
	SPRITE_SCREEN_1_LIST_JUMP = SPRITE_LIST_SWITCH(sprite_list_switch - 0x6u)->entry_2_jump;
	sprite_list_switch += 2u;
	SPRITE_SCREEN_2_LIST_JUMP = SPRITE_LIST_SWITCH(sprite_list_switch - 0x8u)->entry_3_jump;
	sprite_list_switch += 2u;
	sprite_list_buffers = SPRITE_LIST_SWITCH(sprite_list_switch - 0xAu)->next_buffers;
	sprite_list_switch += 2u;
	SCREEN(screen)->sprite_list_last = SPRITE_LIST_SWITCH(sprite_list_switch - 0xCu)->next_list;
	SCREEN(screen)->sprite_list_limit = SPRITE_LIST_SWITCH(sprite_list_switch - 0xCu)->next_limit;
	SCREEN(screen)->sprite_groups = 0u;
}

/* Draws the row's items in view: ends the object once end_position is not past the view's start
   (race_copy window_segment); nothing while first_position is beyond the draw distance
   (SCREEN(screen)->draw_distance >> 7 segments); moves first_position up to the first item in view; for
   each item (spacing apart, to the draw distance) where the road has a scale: x = (the road's width *
   side + x_offset) * scale >> 12 plus the road's screen x unless screen_centered, y = y_offset * scale
   >> 12 plus the road's screen y; off the screen (x beyond -1E0..1E0, y beyond -160..80) it is skipped,
   else the scale shifted by scale_shift picks the size (size_table: a size step and a shift) and a
   sprite of the animation's frame with its palette (flags bits 6-7 flip it) goes into the screen's
   sprite list (sprite_list_last; the copy at +3000 is the halfway point from the item's sprite of the
   frame before, previous_sprites) and the depth list of its road segment (screen + 5000). Sets flags bit
   5 and keeps where its sprites went in previous_sprites. Used by scenery, fork_sign, animated_scenery,
   flashing_scenery and overhead_scenery (and 01351E). */
void road_row_draw(uint32_t rom_offset_and_palette, uint32_t piece_width)
{
	uint16_t places_mask = 0, previous_word = 0, sprite_width = 0, view_end = 0, x = 0, y = 0;
	uint32_t attributes_and_size = 0, blank_place = 0, depth_list = 0, first_position = 0, item_count = 0, item_position = 0, items_left = 0, layer_head = 0, palette_word = 0, positions_moved = 0, previous_sprites = 0, projected_segment = 0, projected_step = 0, ring_segment = 0, ring_step = 0, size = 0, size_index = 0, size_steps = 0, size_table = 0, spacing = 0, sprite_entry = 0, sprite_place = 0, sprite_size_step = 0, sprite_then_height_bits = 0, window_then_width_bits = 0;
	uint16_t items_in_view = 0;
	uint32_t flags = 0, flags_long = 0, previous_first_position = 0;

	item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	item_count = (uint16_t)OBJECT(current_object)->road_row.end_position;
	window_then_width_bits = RACE(SCREEN(screen)->race_copy)->window_segment;
	view_end = (uint16_t)(window_then_width_bits + (SCREEN(screen)->draw_distance >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)item_count <= (int16_t)window_then_width_bits)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	if ((int16_t)view_end <= (int16_t)item_position)
		return;
	while ((int16_t)item_position < (int16_t)window_then_width_bits)
	{
		item_position = (uint16_t)(item_position + spacing);
	}
	if ((int16_t)view_end < (int16_t)item_count)
		item_count = view_end;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = item_position;
	item_count = (uint16_t)(item_count - item_position);
	item_count = (uint16_t)(item_count + spacing);
	item_count = (uint16_t)(item_count - 1u);
	items_in_view = divide_u16(item_count, spacing);
	item_count = items_in_view;
	if ((int16_t)items_in_view <= 0)
		return;
	previous_sprites = 0x100000u;
	sprite_place = SCREEN(screen)->next_sprite_place;
	places_mask = 0xEFFFu;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->flags_upper << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->flags_upper = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) != 0u)
	{
		previous_sprites = OBJECT(current_object)->road_row.previous_sprites;
		previous_first_position = first_position;
		positions_moved = previous_first_position - item_position;
		if (previous_first_position != item_position)
		{
			previous_sprites -= divide_s32(positions_moved, spacing) << 3;
			previous_sprites = LINKED(previous_sprites, previous_sprites & places_mask);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = sprite_place;
	ring_segment = screen + 0x2000u + ((item_position << 5) & 0xFFFu);
	projected_segment = screen + 0x3000u + ((item_position - window_then_width_bits) << 4);
	projected_step = spacing << 4;
	ring_step = spacing << 5;
	items_left = item_count;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	palette_word = (uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0] << 16;
	attributes_and_size = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
	attributes_and_size = attributes_and_size & 0xC0u;
	attributes_and_size = attributes_and_size ^ SPRITE_PIECE(r12)->attributes;
	r12 += 2u;
	sprite_then_height_bits = SPRITE_PIECE(r12 - 0x2u)->sprite;
	size_steps = sprite_size_steps_ADDRESS + sprite_then_height_bits * 4u;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 += 2u;
	for (;;)
	{
		size = PROJECTED_SEGMENT(projected_segment)->scale;
		if (size != 0u && size <= 0x1200u)
		{
			x = (uint16_t)((int32_t)(((uint32_t)ROAD_SEGMENT(ring_segment)->half_width * OBJECT(current_object)->side + OBJECT(current_object)->x_offset) * size) >> 12);
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * size) >> 12) + PROJECTED_SEGMENT(projected_segment)->y);
			if (OBJECT(current_object)->road_row.screen_centered == 0u)
				x += PROJECTED_SEGMENT(projected_segment)->x;
			if ((int16_t)x > 0x1E0 || (int16_t)x < -0x1E0 || (int16_t)y > 0x80 || (int16_t)y < -0x160)
			{
				SPRITE_PLACE(sprite_place)->place = 0xF800F800u;
				ring_segment += ring_step;
				projected_segment += projected_step;
				ring_segment = LINKED(ring_segment, ring_segment & 0xAFFFu);
				sprite_place = LINKED(sprite_place, (uint16_t)(sprite_place + 0x10u));
				previous_sprites = LINKED(previous_sprites, (uint16_t)(previous_sprites + 0x10u));
				sprite_place = LINKED(sprite_place, sprite_place & places_mask);
				previous_sprites = LINKED(previous_sprites, previous_sprites & places_mask);
				items_left--;
				if (items_left != 0u)
					continue;
				SCREEN(screen)->sprite_list_last = sprite_entry;
				break;
			}
			size = shift_by(size, OBJECT(current_object)->scale_shift, 32);
			size_index = size;
			if (size_index >= 0x1000u)
				size_index = 0xFFFu;
			size_index = size_index >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			size = shift_by(size, (uint8_t)ROAD_ROW_SIZE_SHIFTS(road_row_size_shift_lists[size_table])[(int32_t)size_index], 32);
			sprite_size_step = ROAD_ROW_SIZE_STEPS(road_row_size_step_lists[size_table])[(int32_t)size_index];
			if (sprite_size_step != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = sprite_size_step;
				sprite_size_step = size_steps + sprite_size_step * 3u * 4u;
				attributes_and_size = (attributes_and_size & 0xFFFFu) | ((uint32_t)SPRITE_SIZE_STEP(sprite_size_step)->source_size << 16);
				sprite_size_step += 2u;
				rom_offset_and_palette = SPRITE_SIZE_STEP(sprite_size_step - 0x2u)->rom_offset;
				sprite_size_step += 2u;
				r14 = SPRITE_SIZE_STEP(sprite_size_step - 0x4u)->height;
				sprite_size_step += 2u;
				sprite_then_height_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x6u)->height_bits;
				sprite_size_step += 2u;
				piece_width = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width;
				window_then_width_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width_bits | SCREEN(screen)->sprite_screen_bit;
				rom_offset_and_palette |= palette_word;
			}
			sprite_width = (uint16_t)((size * piece_width) >> 8);
			size = ((size * r14) >> 8) & 0x3FFu;
			sprite_width &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
				break;
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = attributes_and_size;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = attributes_and_size;
			SPRITE_PLACE(sprite_place)->rom_offset = rom_offset_and_palette;
			sprite_entry += 4u;
			if (SPRITE_WORDS(previous_sprites)[4] == (uint16_t)rom_offset_and_palette)
			{
				previous_word = SPRITE_WORDS(previous_sprites)[0];
				SPRITE_PLACE(sprite_place)->height = size;
				SPRITE_WORDS(sprite_entry)[6144] = ((uint16_t)(previous_word + size) >> 1) | sprite_then_height_bits;
				SPRITE_WORDS(sprite_entry)[0] = size | sprite_then_height_bits;
				previous_word = SPRITE_WORDS(previous_sprites)[1];
				SPRITE_PLACE(sprite_place)->width = sprite_width;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = ((uint16_t)(previous_word + sprite_width) >> 1) | window_then_width_bits;
				SPRITE_WORDS(sprite_entry)[0] = sprite_width | window_then_width_bits;
				previous_word = SPRITE_WORDS(previous_sprites)[2];
				SPRITE_PLACE(sprite_place)->y = y;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = (int16_t)(previous_word + y) >> 1;
				SPRITE_WORDS(sprite_entry)[0] = y;
				previous_word = SPRITE_WORDS(previous_sprites)[3];
				SPRITE_PLACE(sprite_place)->x = x;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = (int16_t)(previous_word + x) >> 1;
				SPRITE_WORDS(sprite_entry)[0] = x;
				sprite_entry += 2u;
			}
			else
			{
				SPRITE_PLACE(sprite_place)->height = size;
				size = size | sprite_then_height_bits;
				SPRITE_WORDS(sprite_entry)[6144] = size;
				SPRITE_WORDS(sprite_entry)[0] = size;
				SPRITE_PLACE(sprite_place)->width = sprite_width;
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = sprite_width;
				SPRITE_WORDS(sprite_entry)[0] = sprite_width;
				SPRITE_PLACE(sprite_place)->y = y;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = y;
				SPRITE_WORDS(sprite_entry)[0] = y;
				SPRITE_PLACE(sprite_place)->x = x;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6144] = x;
				SPRITE_WORDS(sprite_entry)[0] = x;
				sprite_entry += 2u;
			}
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = rom_offset_and_palette;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = rom_offset_and_palette;
			depth_list = screen + 0x5000u + PROJECTED_SEGMENT(projected_segment)->depth_layer * 4u;
			sprite_entry += 4u;
			if (DEPTH_LIST(depth_list)->head == 0u)
				DEPTH_LIST(depth_list)->tail = sprite_entry;
			layer_head = DEPTH_LIST(depth_list)->head;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = layer_head;
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = (uint16_t)(layer_head + 0x300u);
			DEPTH_LIST(depth_list)->head = ((sprite_entry - 0x10u) >> 4) | 0x8000u;
			SCREEN(screen)->sprite_groups++;
			sprite_entry += 0x10u;
		}
		ring_segment += ring_step;
		projected_segment += projected_step;
		ring_segment = LINKED(ring_segment, ring_segment & 0xAFFFu);
		sprite_place = LINKED(sprite_place, (uint16_t)(sprite_place + 0x10u));
		previous_sprites = LINKED(previous_sprites, (uint16_t)(previous_sprites + 0x10u));
		sprite_place = LINKED(sprite_place, sprite_place & places_mask);
		previous_sprites = LINKED(previous_sprites, previous_sprites & places_mask);
		items_left--;
		if (items_left == 0u)
		{
			SCREEN(screen)->sprite_list_last = sprite_entry;
			break;
		}
	}
	blank_place = blank_sprite_places;
	SPRITE_PLACE(sprite_place)->size = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	SCREEN(screen)->next_sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
}

/* road_row_draw for the rows drawn in mirrored pairs: each item twice, at the same distance either side
   of the road's centre, the second with sprite attribute bits 43 toggled (bit 6 flips it, bits 0-1 swap
   its X alignment); there is no off-screen test and no screen_centered. Used by scenery_pair and
   animated_scenery_pair. */
void road_row_pair_draw(uint32_t rom_offset_and_palette, uint32_t piece_width)
{
	uint16_t halfway_word = 0, mirror_bits = 0, offset_x = 0, places_mask = 0, sprite_width = 0, view_end = 0, y = 0;
	uint32_t attributes_and_size = 0, blank_place = 0, depth_list = 0, first_position = 0, item_count = 0, item_position = 0, items_left = 0, layer_head = 0, pair_x = 0, palette_word = 0, positions_moved = 0, previous_sprites = 0, projected_segment = 0, projected_step = 0, ring_segment = 0, ring_step = 0, size = 0, size_index = 0, size_steps = 0, size_table = 0, spacing = 0, sprite_entry = 0, sprite_place = 0, sprite_size_step = 0, sprite_then_height_bits = 0, window_then_width_bits = 0;
	uint16_t items_in_view = 0;
	uint32_t flags = 0, flags_long = 0, previous_first_position = 0;

	mirror_bits = 0x43u;
	item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	item_count = (uint16_t)OBJECT(current_object)->road_row.end_position;
	window_then_width_bits = RACE(SCREEN(screen)->race_copy)->window_segment;
	view_end = (uint16_t)(window_then_width_bits + (SCREEN(screen)->draw_distance >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)item_count <= (int16_t)window_then_width_bits)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	if ((int16_t)view_end <= (int16_t)item_position)
		return;
	while ((int16_t)item_position < (int16_t)window_then_width_bits)
	{
		item_position = (uint16_t)(item_position + spacing);
	}
	if ((int16_t)view_end < (int16_t)item_count)
		item_count = view_end;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = item_position;
	item_count = (uint16_t)(item_count - item_position);
	item_count = (uint16_t)(item_count + spacing);
	item_count = (uint16_t)(item_count - 1u);
	items_in_view = divide_u16(item_count, spacing);
	item_count = items_in_view;
	if ((int16_t)items_in_view <= 0)
		return;
	previous_sprites = 0x100000u;
	sprite_place = SCREEN(screen)->next_sprite_place;
	places_mask = 0xEFFFu;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->flags_upper << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->flags_upper = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) != 0u)
	{
		previous_sprites = OBJECT(current_object)->road_row.previous_sprites;
		previous_first_position = first_position;
		positions_moved = previous_first_position - item_position;
		if (previous_first_position != item_position)
		{
			previous_sprites -= divide_s32(positions_moved, spacing) << 3;
			previous_sprites = LINKED(previous_sprites, previous_sprites & places_mask);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = sprite_place;
	ring_segment = screen + 0x2000u + ((item_position << 5) & 0xFFFu);
	projected_segment = screen + 0x3000u + ((item_position - window_then_width_bits) << 4);
	projected_step = spacing << 4;
	ring_step = spacing << 5;
	items_left = item_count;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	palette_word = (uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0] << 16;
	attributes_and_size = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
	attributes_and_size = attributes_and_size & 0xC0u;
	attributes_and_size = attributes_and_size ^ SPRITE_PIECE(r12)->attributes;
	r12 += 2u;
	sprite_then_height_bits = SPRITE_PIECE(r12 - 0x2u)->sprite;
	size_steps = sprite_size_steps_ADDRESS + sprite_then_height_bits * 4u;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 += 2u;
	for (;;)
	{
		size = PROJECTED_SEGMENT(projected_segment)->scale;
		if (size != 0u && size <= 0x1200u)
		{
			pair_x = (int32_t)(((uint32_t)ROAD_SEGMENT(ring_segment)->half_width * OBJECT(current_object)->side + OBJECT(current_object)->x_offset) * size) >> 12;
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * size) >> 12) + PROJECTED_SEGMENT(projected_segment)->y);
			offset_x = (uint16_t)pair_x;
			pair_x = (pair_x & ~0xFFFFu) | (uint16_t)(pair_x + PROJECTED_SEGMENT(projected_segment)->x);
			pair_x = (pair_x << 16) | (pair_x >> 16);
			pair_x = (pair_x & ~0xFFFFu) | (uint16_t)(0u - offset_x);
			pair_x = (pair_x & ~0xFFFFu) | (uint16_t)(pair_x + PROJECTED_SEGMENT(projected_segment)->x);
			size = shift_by(size, OBJECT(current_object)->scale_shift, 32);
			size_index = size;
			if (size_index >= 0x1000u)
				size_index = 0xFFFu;
			size_index = size_index >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			size = shift_by(size, (uint8_t)ROAD_ROW_SIZE_SHIFTS(road_row_size_shift_lists[size_table])[(int32_t)size_index], 32);
			sprite_size_step = ROAD_ROW_SIZE_STEPS(road_row_size_step_lists[size_table])[(int32_t)size_index];
			if (sprite_size_step != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = sprite_size_step;
				sprite_size_step = size_steps + sprite_size_step * 3u * 4u;
				attributes_and_size = (attributes_and_size & 0xFFFFu) | ((uint32_t)SPRITE_SIZE_STEP(sprite_size_step)->source_size << 16);
				sprite_size_step += 2u;
				rom_offset_and_palette = SPRITE_SIZE_STEP(sprite_size_step - 0x2u)->rom_offset;
				sprite_size_step += 2u;
				r14 = SPRITE_SIZE_STEP(sprite_size_step - 0x4u)->height;
				sprite_size_step += 2u;
				sprite_then_height_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x6u)->height_bits;
				sprite_size_step += 2u;
				piece_width = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width;
				window_then_width_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width_bits | SCREEN(screen)->sprite_screen_bit;
				rom_offset_and_palette |= palette_word;
			}
			sprite_width = (uint16_t)((size * piece_width) >> 8);
			size = ((size * r14) >> 8) & 0x3FFu;
			sprite_width &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
				break;
			attributes_and_size = attributes_and_size ^ mirror_bits;
			AS32(SPRITE_WORDS(sprite_entry)[6152]) = attributes_and_size;
			AS32(SPRITE_WORDS(sprite_entry)[8]) = attributes_and_size;
			attributes_and_size = attributes_and_size ^ mirror_bits;
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = attributes_and_size;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = attributes_and_size;
			SPRITE_PLACE(sprite_place)->rom_offset = rom_offset_and_palette;
			sprite_entry += 4u;
			if (SPRITE_WORDS(previous_sprites)[4] == (uint16_t)rom_offset_and_palette)
			{
				halfway_word = SPRITE_WORDS(previous_sprites)[0];
				SPRITE_PLACE(sprite_place)->height = size;
				halfway_word = ((uint16_t)(halfway_word + size) >> 1) | sprite_then_height_bits;
				size = size | sprite_then_height_bits;
				SPRITE_WORDS(sprite_entry)[6152] = halfway_word;
				SPRITE_WORDS(sprite_entry)[8] = size;
				SPRITE_WORDS(sprite_entry)[6144] = halfway_word;
				SPRITE_WORDS(sprite_entry)[0] = size;
				halfway_word = SPRITE_WORDS(previous_sprites)[1];
				SPRITE_PLACE(sprite_place)->width = sprite_width;
				halfway_word = ((uint16_t)(halfway_word + sprite_width) >> 1) | window_then_width_bits;
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = halfway_word;
				SPRITE_WORDS(sprite_entry)[8] = sprite_width;
				SPRITE_WORDS(sprite_entry)[6144] = halfway_word;
				SPRITE_WORDS(sprite_entry)[0] = sprite_width;
				halfway_word = SPRITE_WORDS(previous_sprites)[2];
				SPRITE_PLACE(sprite_place)->y = y;
				halfway_word = (uint16_t)((int16_t)(halfway_word + y) >> 1);
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = halfway_word;
				SPRITE_WORDS(sprite_entry)[8] = y;
				SPRITE_WORDS(sprite_entry)[6144] = halfway_word;
				SPRITE_WORDS(sprite_entry)[0] = y;
				halfway_word = SPRITE_WORDS(previous_sprites)[5];
				SPRITE_PLACE(sprite_place)->mirror_x = pair_x;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = (int16_t)(halfway_word + pair_x) >> 1;
				SPRITE_WORDS(sprite_entry)[8] = pair_x;
				pair_x = (pair_x << 16) | (pair_x >> 16);
				halfway_word = SPRITE_WORDS(previous_sprites)[3];
				SPRITE_PLACE(sprite_place)->x = pair_x;
				SPRITE_WORDS(sprite_entry)[6144] = (int16_t)(halfway_word + pair_x) >> 1;
				SPRITE_WORDS(sprite_entry)[0] = pair_x;
				sprite_entry += 2u;
			}
			else
			{
				SPRITE_PLACE(sprite_place)->height = size;
				size = size | sprite_then_height_bits;
				SPRITE_WORDS(sprite_entry)[6152] = size;
				SPRITE_WORDS(sprite_entry)[8] = size;
				SPRITE_WORDS(sprite_entry)[6144] = size;
				SPRITE_WORDS(sprite_entry)[0] = size;
				SPRITE_PLACE(sprite_place)->width = sprite_width;
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = sprite_width;
				SPRITE_WORDS(sprite_entry)[8] = sprite_width;
				SPRITE_WORDS(sprite_entry)[6144] = sprite_width;
				SPRITE_WORDS(sprite_entry)[0] = sprite_width;
				SPRITE_PLACE(sprite_place)->y = y;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = y;
				SPRITE_WORDS(sprite_entry)[8] = y;
				SPRITE_WORDS(sprite_entry)[6144] = y;
				SPRITE_WORDS(sprite_entry)[0] = y;
				SPRITE_PLACE(sprite_place)->mirror_x = pair_x;
				sprite_entry += 2u;
				SPRITE_WORDS(sprite_entry)[6152] = pair_x;
				SPRITE_WORDS(sprite_entry)[8] = pair_x;
				pair_x = (pair_x << 16) | (pair_x >> 16);
				SPRITE_PLACE(sprite_place)->x = pair_x;
				SPRITE_WORDS(sprite_entry)[6144] = pair_x;
				SPRITE_WORDS(sprite_entry)[0] = pair_x;
				sprite_entry += 2u;
			}
			AS32(SPRITE_WORDS(sprite_entry)[6152]) = rom_offset_and_palette;
			AS32(SPRITE_WORDS(sprite_entry)[8]) = rom_offset_and_palette;
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = rom_offset_and_palette;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = rom_offset_and_palette;
			sprite_entry += 0x14u;
			depth_list = screen + 0x5000u + PROJECTED_SEGMENT(projected_segment)->depth_layer * 4u;
			if (DEPTH_LIST(depth_list)->head == 0u)
				DEPTH_LIST(depth_list)->tail = sprite_entry;
			layer_head = DEPTH_LIST(depth_list)->head;
			AS32(SPRITE_WORDS(sprite_entry)[0]) = layer_head;
			AS32(SPRITE_WORDS(sprite_entry)[6144]) = (uint16_t)(layer_head + 0x300u);
			DEPTH_LIST(depth_list)->head = ((sprite_entry - 0x20u) >> 4) | 0x8000u;
			SCREEN(screen)->sprite_groups += 2u;
			sprite_entry += 0x10u;
		}
		ring_segment += ring_step;
		projected_segment += projected_step;
		ring_segment = LINKED(ring_segment, ring_segment & 0xAFFFu);
		sprite_place = LINKED(sprite_place, (uint16_t)(sprite_place + 0x10u));
		previous_sprites = LINKED(previous_sprites, (uint16_t)(previous_sprites + 0x10u));
		sprite_place = LINKED(sprite_place, sprite_place & places_mask);
		previous_sprites = LINKED(previous_sprites, previous_sprites & places_mask);
		items_left--;
		if (items_left == 0u)
		{
			SCREEN(screen)->sprite_list_last = sprite_entry;
			break;
		}
	}
	blank_place = blank_sprite_places;
	SPRITE_PLACE(sprite_place)->size = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
	SPRITE_PLACE_WORDS(sprite_place)[0] = blank_place;
	SCREEN(screen)->next_sprite_place = (sprite_place + 4u) & (places_mask | 0xFFFF0000u);
}
