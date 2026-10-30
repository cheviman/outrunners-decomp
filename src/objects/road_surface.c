/**
 *    Name:        objects/road_surface.c
 *    Description: the road's surface drawn line by line, and its tiles' changes
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The road of the race, drawn on its tilemap with line scroll and line select. By state: 0
   road_surface_start; then each frame this frame's lines (road_surface_lines) and, from state 2, those
   of the frame in between (road_surface_halfway_lines); where the first segment's page set
   (segment_page_set) is 0 it starts the stage's palette fades (start_palette_fades with screen
   road_palette_fades; state 3), and at the next one that is not takes it as page_set and fades back
   (road_surface_fade_back; state 2). Each frame the tilemap's page word (at page_register) is the page
   table's word for page_set and near_scroll (five a set). At each new segment (segment past
   last_segment) acts on the segment's event when it changes (after the first 30 segments, event_delay):
   4 and 5 change the background within the stage (screen stage_background_change = FF,
   stage_background_branch = 0 or 1, for race_background), 1 and 2 turn the two road_tiles_change flags
   (0A, 0B) on and off, 3 both; a flag turned on starts a road_tiles_change (the Golden Gate Bridge in
   race_to_goal.txt, frames 7659-8325). */
void road_surface_update(void)
{
	uint32_t event = 0, fraction_shift = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, ignored_5 = 0, ignored_6 = 0, ignored_7 = 0, max_depth = 0, near_scroll = 0, previous_depth = 0, previous_line = 0, previous_x = 0, road_palette_fades = 0, rounding = 0, stripe_count = 0;
	struct object *object = NULL;
	uint8_t first_flag = 0, first_tiles_flag = 0, second_tiles_flag = 0;

	switch (current_object->state)
	{
	case 0:
		road_surface_start();
		current_object->road_surface.event_delay = 0x1Eu;
		current_object->state++;
		return;
	case 1:
		road_surface_lines(&ignored, &ignored_2, &ignored_3, &r12, &ignored_4, &r14, &ignored_5, &ignored_6, &ignored_7);
		current_object->road_surface.points_buffer ^= 0x60u;
		current_object->state++;
		if (current_object->road_surface.segment_page_set == 0u)
		{
			road_palette_fades = screen->road_palette_fades;
			start_palette_fades(road_palette_fades);
			current_object->state++;
		}
		break;
	case 2:
		road_surface_lines(&previous_line, &previous_x, &previous_depth, &r12, &stripe_count, &r14, &fraction_shift, &rounding, &max_depth);
		road_surface_halfway_lines(previous_line, previous_x, previous_depth, stripe_count, (uint8_t)fraction_shift, rounding, (uint16_t)max_depth, &r12, &r14);
		if (current_object->road_surface.segment_page_set == 0u)
		{
			road_palette_fades = screen->road_palette_fades;
			start_palette_fades(road_palette_fades);
			current_object->state++;
		}
		break;
	case 3:
		road_surface_lines(&previous_line, &previous_x, &previous_depth, &r12, &stripe_count, &r14, &fraction_shift, &rounding, &max_depth);
		road_surface_halfway_lines(previous_line, previous_x, previous_depth, stripe_count, (uint8_t)fraction_shift, rounding, (uint16_t)max_depth, &r12, &r14);
		if (current_object->road_surface.segment_page_set != 0u)
		{
			current_object->road_surface.page_set = current_object->road_surface.segment_page_set;
			road_palette_fades = screen->road_palette_fades;
			road_surface_fade_back(road_palette_fades);
			current_object->state--;
		}
		break;
	}
	near_scroll = current_object->road_surface.near_scroll;
	near_scroll = (uint16_t)(near_scroll + 0x4FFu);
	PAGE_SELECT(current_object->road_surface.page_register)[0] = current_object->road_surface.page_table[(int32_t)((near_scroll >> 9) + (current_object->road_surface.page_set - 3u) * 5u)];
	if (current_object->road_surface.segment <= current_object->road_surface.last_segment)
		return;
	current_object->road_surface.last_segment = current_object->road_surface.segment;
	if (current_object->road_surface.event_delay != 0u)
	{
		current_object->road_surface.last_event = current_object->road_surface.event;
		current_object->road_surface.event_delay--;
		return;
	}
	event = current_object->road_surface.event;
	if (current_object->road_surface.last_event == event)
		return;
	current_object->road_surface.last_event = event;
	switch (event)
	{
	case 4:
		screen->stage_background_change = 0xFFu;
		screen->stage_background_branch = 0u;
		return;
	case 5:
		screen->stage_background_change = 0xFFu;
		screen->stage_background_branch = 1u;
		return;
	case 0:
		return;
	case 1:
		first_flag = ~current_object->road_surface.first_tiles_flag;
		current_object->road_surface.first_tiles_flag = first_flag;
		if (first_flag == 0u)
			return;
		road_surface_start_tiles_change();
		return;
	case 3:
		first_tiles_flag = ~current_object->road_surface.first_tiles_flag;
		current_object->road_surface.first_tiles_flag = first_tiles_flag;
		if (first_tiles_flag != 0u)
			road_surface_start_tiles_change();
		second_tiles_flag = ~current_object->road_surface.second_tiles_flag;
		current_object->road_surface.second_tiles_flag = second_tiles_flag;
		if (second_tiles_flag == 0u)
			return;
		object = start_object();
		object->road_tiles_change.table_half = 1u;
		object->road_tiles_change.road = address_of(current_object);
		object->road_tiles_change.flag = address_of(current_object) + 0xBu;
		object->road_tiles_change.data = road_tiles_change_second_data_ADDRESS;
		object->handler = road_tiles_change_update_ADDRESS;
		return;
	case 2:
		second_tiles_flag = ~current_object->road_surface.second_tiles_flag;
		current_object->road_surface.second_tiles_flag = second_tiles_flag;
		if (second_tiles_flag == 0u)
			return;
		object = start_object();
		object->road_tiles_change.table_half = 1u;
		object->road_tiles_change.road = address_of(current_object);
		object->road_tiles_change.flag = address_of(current_object) + 0xBu;
		object->road_tiles_change.data = road_tiles_change_second_data_ADDRESS;
		object->handler = road_tiles_change_update_ADDRESS;
		return;
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
	uint32_t line_select_table = 0, mixer_address = 0, mixer_bits = 0, palettes = 0;
	const struct road_surface_layer_setup *road_surface_layer_setup = NULL;

	road_surface_layer_setup = rom_pointer(road_surface_layer_setups_ADDRESS + ((uint32_t)screen->index << 3) * 4u);
	video_bits = game->video_registers[2] & road_surface_layer_setup->video_mask;
	game->video_registers[2] = video_bits | road_surface_layer_setup->video_bits;
	VIDEO_REGISTER_WORDS(road_surface_layer_setup->scroll_x_address)[0] = 0x160u;
	VIDEO_REGISTER_WORDS(road_surface_layer_setup->scroll_y_address)[0] = 0u;
	game->send_video_registers = 0xFFu;
	TILE_BANK_VALUES(road_surface_layer_setup->tile_bank_address)[0] = 0u;
	current_object->road_surface.page_register = road_surface_layer_setup->page_register;
	mixer_address = road_surface_layer_setup->mixer_address;
	mixer_bits = MIXER_VALUE_PAIRS(mixer_address)[0];
	mixer_bits = mixer_bits & ~0xFu;
	MIXER_VALUE_PAIRS(mixer_address)[0] = mixer_bits | 8u;
	screen->flags |= 4u;
	current_object->road_surface.line_scroll_table = road_surface_layer_setup->line_scroll_table;
	line_select_table = road_surface_layer_setup->line_select_table;
	current_object->road_surface.line_select_table = line_select_table;
	move_halfwords(line_select_table - 0x1C0u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0x640u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0xE40u, zero_block_ADDRESS, 0xE0u);
	move_halfwords(line_select_table + 0x1640u, zero_block_ADDRESS, 0xE0u);
	current_object->road_surface.page_set = stage_road_page_sets[screen->first_stage];
	current_object->road_surface.points_buffer = 0u;
	current_object->road_surface.last_event = 0u;
	current_object->road_surface.last_segment = 0u;
	current_object->road_surface.first_tiles_flag = 0u;
	current_object->road_surface.second_tiles_flag = 0u;
	move_halfwords(address_of(current_object) + 0x18u, road_surface_page_words_ADDRESS, 0x1Eu);
	palettes = stage_road_palettes[screen->first_stage];
	screen->road_palette_fades = ROAD_PALETTE_LISTS(palettes)[0];
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
void road_surface_lines(uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_select_place, uint32_t *p_stripe_count, uint32_t *p_stripe, uint32_t *p_fraction_shift, uint32_t *p_rounding, uint32_t *p_max_depth)
{
	uint32_t previous_line = *p_previous_line;
	uint32_t previous_x = *p_previous_x;
	uint32_t previous_depth = *p_previous_depth;
	uint32_t stripe = *p_stripe;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t fraction_shift = 0;
	uint16_t line_select = 0, max_depth = 0;
	uint32_t depth_step = 0, ignored = 0, ignored_2 = 0, line = 0, line_count = 0, point_depth = 0, point_entry = 0, point_line = 0, point_x = 0, rounding = 0, scroll_place = 0, segment = 0, select_place = 0, skip_state = 0, sprite_origin_y = 0, table_offset = 0, view_segment = 0, x_step = 0;
	uint32_t scroll_bottom = 0;
	uint16_t segment_scale = 0, remaining_scale = 0;
	uint32_t line_reached = 0, line_reached_first = 0, pairs_left = 0;

	view_segment = address_of(screen) + 0x3000u;
	table_offset = (uint8_t)(game->video_registers[2] >> 8u);
	table_offset = (uint8_t)(table_offset + 8u);
	table_offset = (table_offset & 0xCu) << 7;
	scroll_place = current_object->road_surface.line_scroll_table + table_offset * 4u;
	scroll_bottom = scroll_place;
	select_place = current_object->road_surface.line_select_table + table_offset * 4u;
	segment = RACE(screen->race_copy)->window_segment;
	stripe &= ~0xFFFFu;
	fraction_shift = 0xF4u;
	rounding = 0x800u;
	sprite_origin_y = (int16_t)screen->sprite_origin_y;
	max_depth = 0xFFu;
	skip_state = 1u;
	point_entry = address_of(screen) + 0x3800u + current_object->road_surface.points_buffer * 2u * 4u;
	line = 0xE0u;
	while (PROJECTED_SEGMENT(view_segment)->scale == 0u)
	{
		view_segment += 0x10u;
		segment++;
	}
	current_object->road_surface.segment_page_set = PROJECTED_SEGMENT(view_segment)->width_code;
	current_object->road_surface.event = PROJECTED_SEGMENT(view_segment)->road_event;
	current_object->road_surface.segment = segment;
	ROAD_POINTS(point_entry)[0] = segment;
	segment = 0u - (segment & ~0xFFFCu);
	overflow = v60_overflow_add32(segment, 2u, 0u);
	zero = segment + 2u == 0u;
	negative = (segment + 2u) >> 31;
	segment += 2u;
	point_entry += 2u;
	if ((negative ^ overflow) | zero)
	{
		segment += 2u;
		stripe |= 0x100u;
	}
	for (;;)
	{
		segment_scale = PROJECTED_SEGMENT(view_segment)->scale;
		view_segment += 2u;
		if (segment_scale != 0u)
		{
			point_depth = PROJECTED_SEGMENT(view_segment - 0x2u)->width >> 2;
			ROAD_POINTS(point_entry)[0] = point_depth;
			point_entry += 2u;
			view_segment += 2u;
			point_x = 0u - (int16_t)PROJECTED_SEGMENT(view_segment - 0x4u)->x;
			ROAD_POINTS(point_entry)[0] = point_x;
			point_entry += 2u;
			view_segment += 2u;
			point_line = (int16_t)PROJECTED_SEGMENT(view_segment - 0x6u)->y + sprite_origin_y;
			ROAD_POINTS(point_entry)[0] = point_line;
			point_entry += 2u;
			view_segment += 0xAu;
			if (skip_state == 0u)
			{
				line_reached = line;
				line_count = line_reached - point_line;
				if ((int32_t)line_reached <= (int32_t)point_line)
				{
					previous_line = point_line;
					previous_x = point_x;
					previous_depth = point_depth;
					segment--;
					if (segment != 0u)
						continue;
					segment = 2u;
					stripe = stripe ^ 0x100u;
					continue;
				}
				if (line_count != 1u)
				{
					road_surface_fill_lines(line, point_line, point_x, point_depth, (uint16_t)stripe, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &ignored, &ignored_2, &scroll_place, &select_place, &zero);
					if (zero == 0u)
					{
						for (;;)
						{
							remaining_scale = PROJECTED_SEGMENT(view_segment)->scale;
							view_segment += 2u;
							if (remaining_scale == 0u)
								break;
							ROAD_POINTS(point_entry)[0] = PROJECTED_SEGMENT(view_segment - 0x2u)->width >> 2;
							point_entry += 2u;
							view_segment += 2u;
							ROAD_POINTS(point_entry)[0] = 0u - PROJECTED_SEGMENT(view_segment - 0x4u)->x;
							point_entry += 2u;
							view_segment += 2u;
							point_line = (int16_t)PROJECTED_SEGMENT(view_segment - 0x6u)->y + sprite_origin_y;
							ROAD_POINTS(point_entry)[0] = point_line;
							point_entry += 2u;
							view_segment += 0xAu;
						}
						break;
					}
				}
			}
			else
			{
				line_reached_first = line;
				line_count = line_reached_first - point_line;
				if ((int32_t)line_reached_first <= (int32_t)point_line)
				{
					skip_state = 0u;
					current_object->road_surface.skipped_line = 0u;
					previous_line = point_line;
					previous_x = point_x;
					previous_depth = point_depth;
					segment--;
					if (segment != 0u)
						continue;
					segment = 2u;
					stripe = stripe ^ 0x100u;
					continue;
				}
				if ((int32_t)skip_state < 0)
				{
					if (line_count != 1u)
					{
						current_object->road_surface.skipped_x = previous_x;
						current_object->road_surface.skipped_depth = previous_depth;
						road_surface_fill_lines(line, point_line, point_x, point_depth, (uint16_t)stripe, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &x_step, &depth_step, &scroll_place, &select_place, &zero);
						current_object->road_surface.skipped_x_step = x_step;
						current_object->road_surface.skipped_depth_step = depth_step;
						skip_state = 0u;
						if (zero == 0u)
						{
							for (;;)
							{
								remaining_scale = PROJECTED_SEGMENT(view_segment)->scale;
								view_segment += 2u;
								if (remaining_scale == 0u)
									break;
								ROAD_POINTS(point_entry)[0] = PROJECTED_SEGMENT(view_segment - 0x2u)->width >> 2;
								point_entry += 2u;
								view_segment += 2u;
								ROAD_POINTS(point_entry)[0] = 0u - PROJECTED_SEGMENT(view_segment - 0x4u)->x;
								point_entry += 2u;
								view_segment += 2u;
								point_line = (int16_t)PROJECTED_SEGMENT(view_segment - 0x6u)->y + sprite_origin_y;
								ROAD_POINTS(point_entry)[0] = point_line;
								point_entry += 2u;
								view_segment += 0xAu;
							}
							break;
						}
					}
				}
				else
				{
					line_count = (line_count - 1u) * 2u;
					scroll_place -= line_count;
					select_place -= line_count;
					current_object->road_surface.skipped_line = point_line;
					current_object->road_surface.skipped_scroll_place = scroll_place;
					current_object->road_surface.skipped_select_place = select_place;
					current_object->road_surface.skipped_stripe = stripe;
					skip_state = 0xFFFFFFFFu;
				}
			}
			scroll_place -= 2u;
			LINE_SCROLL_VALUES(scroll_place)[0] = point_x;
			line_select = (uint16_t)point_depth;
			if ((int16_t)max_depth < (int16_t)line_select)
				line_select = (uint16_t)max_depth;
			select_place -= 2u;
			LINE_SELECTS(select_place)[0] = line_select + stripe;
			line = point_line;
			if (line == 0u)
			{
				for (;;)
				{
					remaining_scale = PROJECTED_SEGMENT(view_segment)->scale;
					view_segment += 2u;
					if (remaining_scale == 0u)
						break;
					ROAD_POINTS(point_entry)[0] = PROJECTED_SEGMENT(view_segment - 0x2u)->width >> 2;
					point_entry += 2u;
					view_segment += 2u;
					ROAD_POINTS(point_entry)[0] = 0u - PROJECTED_SEGMENT(view_segment - 0x4u)->x;
					point_entry += 2u;
					view_segment += 2u;
					point_line = (int16_t)PROJECTED_SEGMENT(view_segment - 0x6u)->y + sprite_origin_y;
					ROAD_POINTS(point_entry)[0] = point_line;
					point_entry += 2u;
					view_segment += 0xAu;
				}
				break;
			}
			previous_line = point_line;
			previous_x = point_x;
			previous_depth = point_depth;
			segment--;
			if (segment != 0u)
				continue;
			segment = 2u;
			stripe = stripe ^ 0x100u;
			continue;
		}
		pairs_left = shift_by(line, max_depth, 32);
		carry = v60_shl_cy(line, max_depth, 32);
		zero = pairs_left == 0u;
		line = pairs_left;
		if (carry)
		{
			select_place -= 2u;
			LINE_SELECTS(select_place)[0] = 0u;
			if (zero)
				break;
		}
		line++;
		for (;;)
		{
			select_place -= 4u;
			line--;
			if (((line != 0u) & (LINE_SELECT_PAIRS(select_place)[0] != 0u)) == 0u)
				break;
			LINE_SELECT_PAIRS(select_place)[0] = 0u;
		}
		break;
	}
	screen->road_top_line = point_line;
	ROAD_POINTS(point_entry)[0] = 0xFFFFu;
	road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &previous_x, &previous_depth, &select_place, &stripe);
	current_object->road_surface.near_scroll = LINE_SCROLL_VALUES(scroll_bottom)[-1];
	*p_previous_line = previous_line;
	*p_previous_x = previous_x;
	*p_previous_depth = previous_depth;
	*p_select_place = select_place;
	*p_stripe_count = segment;
	*p_stripe = stripe;
	*p_fraction_shift = fraction_shift;
	*p_rounding = rounding;
	*p_max_depth = (*p_max_depth & 0xFFFF0000u) | max_depth;
}

/* Works out the road of the frame in between: the same lines from the average of this frame's points and
   the last frame's (the two halves of the points buffer, matched by segment), into the next tables;
   flips points_buffer. */
void road_surface_halfway_lines(uint32_t previous_line, uint32_t previous_x, uint32_t previous_depth, uint32_t stripe_count, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_select_place, uint32_t *p_stripe)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t line_select = 0, segment = 0;
	uint32_t depth_step = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, last_frame_points = 0, line = 0, line_count = 0, point_depth = 0, point_line = 0, point_x = 0, points = 0, scroll_place = 0, select_place = 0, skip_state = 0, table_offset = 0, x_step = 0;
	uint16_t last_frame_segment = 0, last_depth = 0, last_x = 0, last_line = 0;
	uint32_t line_reached = 0, line_reached_first = 0, pairs_left = 0;

	table_offset = (uint8_t)(game->video_registers[2] >> 8u);
	table_offset = (uint8_t)(table_offset + 4u);
	table_offset = (table_offset & 0xCu) << 7;
	scroll_place = current_object->road_surface.line_scroll_table + table_offset * 4u;
	select_place = current_object->road_surface.line_select_table + table_offset * 4u;
	*p_stripe &= ~0xFFFFu;
	skip_state = 1u;
	line = 0xE0u;
	points = current_object->road_surface.points_buffer;
	last_frame_points = points ^ 0x60u;
	current_object->road_surface.points_buffer = last_frame_points;
	points = address_of(screen) + 0x3800u + points * 2u * 4u;
	last_frame_points = address_of(screen) + 0x3800u + last_frame_points * 2u * 4u;
	segment = ROAD_POINTS(points)[0];
	last_frame_segment = ROAD_POINTS(last_frame_points)[0];
	last_frame_points += 2u;
	stripe_count = (stripe_count & ~0xFFFFu) | last_frame_segment;
	points += 2u;
	if ((uint16_t)stripe_count != segment)
	{
		if ((uint16_t)stripe_count < segment)
		{
			do
			{
				last_frame_points += 6u;
				stripe_count = (stripe_count & ~0xFFFFu) | (uint16_t)(stripe_count + 1u);
			} while ((uint16_t)stripe_count < segment);
		}
		else
		{
			do
			{
				points += 6u;
				segment++;
			} while (segment < (uint16_t)stripe_count);
		}
	}
	stripe_count = 0u - (stripe_count & ~0xFFFCu);
	overflow = v60_overflow_add32(stripe_count, 2u, 0u);
	zero = stripe_count + 2u == 0u;
	negative = (stripe_count + 2u) >> 31;
	stripe_count += 2u;
	if ((negative ^ overflow) | zero)
	{
		stripe_count += 2u;
		*p_stripe |= 0x100u;
	}
	while ((int16_t)(ROAD_POINTS(points)[0] | ROAD_POINTS(last_frame_points)[0]) >= 0)
	{
		last_depth = ROAD_POINTS(last_frame_points)[0];
		last_frame_points += 2u;
		point_depth = shift_by((uint32_t)ROAD_POINTS(points)[0] + last_depth, max_depth, 32);
		points += 2u;
		last_x = ROAD_POINTS(last_frame_points)[0];
		last_frame_points += 2u;
		point_x = shift_signed_by((uint32_t)(int16_t)ROAD_POINTS(points)[0] + (int16_t)last_x, max_depth, 32);
		points += 2u;
		last_line = ROAD_POINTS(last_frame_points)[0];
		last_frame_points += 2u;
		point_line = shift_signed_by((uint32_t)(int16_t)ROAD_POINTS(points)[0] + (int16_t)last_line, max_depth, 32);
		points += 2u;
		if (skip_state == 0u)
		{
			line_reached = line;
			line_count = line_reached - point_line;
			if ((int32_t)point_line < (int32_t)line_reached)
			{
				if (line_count != 1u)
				{
					road_surface_fill_lines(line, point_line, point_x, point_depth, (uint16_t)*p_stripe, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &ignored, &ignored_2, &scroll_place, &select_place, &zero);
					if (zero == 0u)
					{
						*p_select_place = select_place;
						road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
						return;
					}
				}
				scroll_place -= 2u;
				LINE_SCROLL_VALUES(scroll_place)[0] = point_x;
				line_select = (uint16_t)point_depth;
				if ((int16_t)max_depth < (int16_t)line_select)
					line_select = (uint16_t)max_depth;
				select_place -= 2u;
				LINE_SELECTS(select_place)[0] = line_select + *p_stripe;
				line = point_line;
				if (line == 0u)
				{
					*p_select_place = select_place;
					road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
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
						LINE_SCROLL_VALUES(scroll_place)[0] = point_x;
						line_select = (uint16_t)point_depth;
						if ((int16_t)max_depth < (int16_t)line_select)
							line_select = (uint16_t)max_depth;
						select_place -= 2u;
						LINE_SELECTS(select_place)[0] = line_select + *p_stripe;
						line = point_line;
						if (line == 0u)
						{
							*p_select_place = select_place;
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
							return;
						}
					}
					else
					{
						current_object->road_surface.skipped_x = previous_x;
						current_object->road_surface.skipped_depth = previous_depth;
						road_surface_fill_lines(line, point_line, point_x, point_depth, (uint16_t)*p_stripe, fraction_shift, rounding, max_depth, &previous_line, &previous_x, &previous_depth, &x_step, &depth_step, &scroll_place, &select_place, &zero);
						current_object->road_surface.skipped_x_step = x_step;
						current_object->road_surface.skipped_depth_step = depth_step;
						skip_state = 0u;
						if (zero == 0u)
						{
							*p_select_place = select_place;
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
							return;
						}
						scroll_place -= 2u;
						LINE_SCROLL_VALUES(scroll_place)[0] = point_x;
						line_select = (uint16_t)point_depth;
						if ((int16_t)max_depth < (int16_t)line_select)
							line_select = (uint16_t)max_depth;
						select_place -= 2u;
						LINE_SELECTS(select_place)[0] = line_select + *p_stripe;
						line = point_line;
						if (line == 0u)
						{
							*p_select_place = select_place;
							road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
							return;
						}
					}
				}
				else
				{
					line_count = (line_count - 1u) * 2u;
					scroll_place -= line_count;
					select_place -= line_count;
					current_object->road_surface.skipped_line = point_line;
					current_object->road_surface.skipped_scroll_place = scroll_place;
					current_object->road_surface.skipped_select_place = select_place;
					current_object->road_surface.skipped_stripe = *p_stripe;
					skip_state = 0xFFFFFFFFu;
					scroll_place -= 2u;
					LINE_SCROLL_VALUES(scroll_place)[0] = point_x;
					line_select = (uint16_t)point_depth;
					if ((int16_t)max_depth < (int16_t)line_select)
						line_select = (uint16_t)max_depth;
					select_place -= 2u;
					LINE_SELECTS(select_place)[0] = line_select + *p_stripe;
					line = point_line;
					if (line == 0u)
					{
						*p_select_place = select_place;
						road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
						return;
					}
				}
			}
			else
			{
				skip_state = 0u;
				current_object->road_surface.skipped_line = 0u;
			}
		}
		previous_line = point_line;
		previous_x = point_x;
		previous_depth = point_depth;
		stripe_count--;
		if (stripe_count != 0u)
			continue;
		stripe_count = 2u;
		*p_stripe = *p_stripe ^ 0x100u;
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
			select_place -= 4u;
			line--;
			if (((line != 0u) & (LINE_SELECT_PAIRS(select_place)[0] != 0u)) == 0u)
				break;
			LINE_SELECT_PAIRS(select_place)[0] = 0u;
		}
		*p_select_place = select_place;
		road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
		return;
	}
	select_place -= 2u;
	LINE_SELECTS(select_place)[0] = 0u;
	if (zero)
	{
		*p_select_place = select_place;
		road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
		return;
	}
	line++;
	for (;;)
	{
		select_place -= 4u;
		line--;
		if (((line != 0u) & (LINE_SELECT_PAIRS(select_place)[0] != 0u)) == 0u)
			break;
		LINE_SELECT_PAIRS(select_place)[0] = 0u;
	}
	*p_select_place = select_place;
	road_surface_fill_skipped_lines(fraction_shift, rounding, max_depth, &ignored_3, &ignored_4, p_select_place, p_stripe);
}

/* Fills the road's lines between two of its points, from the lower one up: the line scroll steps evenly
   from one point's x to the other's and the line select from one's depth to the other's (at most R18),
   plus R14 (100 on alternate pairs of segments); returns the two steps. */
void road_surface_fill_lines(uint32_t line, uint32_t point_line, uint32_t point_x, uint32_t point_depth, uint16_t stripe, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_x_step, uint32_t *p_depth_step, uint32_t *p_line_scroll_place, uint32_t *p_select_place, uint32_t *p_zero)
{
	uint32_t previous_line = *p_previous_line;
	uint32_t previous_x = *p_previous_x;
	uint32_t previous_depth = *p_previous_depth;
	uint32_t line_scroll_place = *p_line_scroll_place;
	uint32_t select_place = *p_select_place;
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
			LINE_SCROLL_VALUES(line_scroll_place)[0] = shift_signed_by(previous_x, (uint8_t)fraction_shift, 32);
			line_select = (uint16_t)shift_signed_by(previous_depth, (uint8_t)fraction_shift, 32);
			if ((int16_t)max_depth < (int16_t)line_select)
				line_select = (uint16_t)max_depth;
			select_place -= 2u;
			LINE_SELECTS(select_place)[0] = line_select + stripe;
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
			LINE_SCROLL_VALUES(line_scroll_place)[0] = shift_signed_by(previous_x, (uint8_t)fraction_shift, 32);
			line_select = (uint16_t)shift_signed_by(previous_depth, (uint8_t)fraction_shift, 32);
			if ((int16_t)max_depth < (int16_t)line_select)
				line_select = (uint16_t)max_depth;
			select_place -= 2u;
			LINE_SELECTS(select_place)[0] = line_select + stripe;
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
	*p_select_place = select_place;
	*p_zero = zero;
}

/* Fills the stretch of lines road_surface_lines passed over (from line skipped_line, at the table places
   it kept) by going on from the kept point with the kept steps down to line DF; nothing when there is
   none. */
void road_surface_fill_skipped_lines(uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_select_place, uint32_t *p_stripe)
{
	uint16_t line_select = 0;
	uint32_t line_count = 0, scroll_place = 0, skipped_depth_step = 0, skipped_line = 0, x_step = 0;

	skipped_line = current_object->road_surface.skipped_line;
	if (skipped_line == 0u)
		return;
	*p_previous_x = (current_object->road_surface.skipped_x << 12) + rounding;
	*p_previous_depth = (current_object->road_surface.skipped_depth << 12) + rounding;
	x_step = current_object->road_surface.skipped_x_step;
	skipped_depth_step = current_object->road_surface.skipped_depth_step;
	scroll_place = current_object->road_surface.skipped_scroll_place;
	*p_select_place = current_object->road_surface.skipped_select_place;
	*p_stripe = current_object->road_surface.skipped_stripe;
	line_count = 0xDFu - skipped_line;
	if (0xDFu == skipped_line)
	{
		return;
	}
	do
	{
		*p_previous_x += x_step;
		*p_previous_depth += skipped_depth_step;
		LINE_SCROLL_VALUES(scroll_place)[0] = shift_signed_by(*p_previous_x, (uint8_t)fraction_shift, 32);
		line_select = (uint16_t)shift_signed_by(*p_previous_depth, (uint8_t)fraction_shift, 32);
		scroll_place += 2u;
		if ((int16_t)max_depth < (int16_t)line_select)
		{
			line_select = (uint16_t)max_depth;
			x_step = 0u;
		}
		LINE_SELECTS(*p_select_place)[0] = line_select + *p_stripe;
		*p_select_place += 2u;
		line_count--;
	} while (line_count != 0u);
}

/* Starts the road_tiles_change of event 1 (its data road_tiles_change_first_data, the even bytes of the
   page table) with the road's first flag (0A). */
void road_surface_start_tiles_change(void)
{
	struct object *object = NULL;

	object = start_object();
	object->road_tiles_change.table_half = 0u;
	object->road_tiles_change.road = address_of(current_object);
	object->road_tiles_change.flag = address_of(current_object) + 0xAu;
	object->road_tiles_change.data = road_tiles_change_first_data_ADDRESS;
	object->handler = road_tiles_change_update_ADDRESS;
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
	uint32_t byte_count = 0, data = 0, halfword_count = 0, page_count = 0, page_numbers = 0, page_set = 0, row_bytes = 0, row_count = 0, source_end = 0, source_slot = 0, step = 0, table_place = 0, table_source = 0, target_end = 0, target_page_numbers = 0, target_slot = 0;
	uint8_t target_page = 0, pages_left = 0, step_2 = 0, target_page_back = 0, pages_left_2 = 0, step_3 = 0;

	data = current_object->road_tiles_change.data;
	switch (current_object->state)
	{
	case 0:
		source_slot = address_of(current_object) + 0x18u;
		target_slot = address_of(current_object) + 0x28u;
		page_set = OBJECT(current_object->road_tiles_change.road)->road_surface.page_set;
		current_object->road_tiles_change.page_set = page_set;
		r12 = data - 0xCu + page_set * 4u;
		target_page_numbers = data + 0x18u + screen->index * 4u;
		page_count = 4u;
		do
		{
			TILE_PAGE_PLACES(source_slot)[0] = 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(r12)[0] << 8) * 4u;
			target_page = ROAD_TILES_CHANGE_BYTES(target_page_numbers)[0];
			target_page_numbers++;
			TILE_PAGE_PLACES(target_slot)[0] = 0x300000u + ((uint32_t)target_page << 8) * 4u;
			target_slot += 4u;
			page_count--;
			r12++;
			source_slot += 4u;
		} while (page_count != 0u);
		current_object->road_tiles_change.step = 0u;
		current_object->road_tiles_change.pages_left = 4u;
		current_object->state++;
	case 1:
		step = current_object->road_tiles_change.step;
		move_halfwords(current_object->road_tiles_change.target_pages[(int32_t)step], current_object->road_tiles_change.source_pages[(int32_t)step], 0x200u);
		current_object->road_tiles_change.step++;
		pages_left = current_object->road_tiles_change.pages_left;
		current_object->road_tiles_change.pages_left = pages_left - 1u;
		if (pages_left != 1u)
			return;
		table_source = data + 0x20u + screen->index * 5u * 2u;
		table_place = current_object->road_tiles_change.road + 0x18u + current_object->road_tiles_change.table_half;
		row_count = 3u;
		do
		{
			r12 = table_source;
			row_bytes = 0xAu;
			do
			{
				ROAD_PAGE_TABLE_BYTES(table_place)[0] = ROAD_TILES_CHANGE_BYTES(r12)[0];
				row_bytes--;
				r12++;
				table_place += 2u;
			} while (row_bytes != 0u);
			row_count--;
		} while (row_count != 0u);
		source_slot = address_of(current_object) + 0x18u;
		page_numbers = data + current_object->road_tiles_change.page_set * 4u;
		page_count = 4u;
		do
		{
			TILE_PAGE_PLACES(source_slot)[0] = 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(page_numbers)[0] << 8) * 4u;
			page_count--;
			page_numbers++;
			source_slot += 4u;
		} while (page_count != 0u);
		current_object->road_tiles_change.step = 0x10u;
		current_object->state++;
		return;
	case 2:
		halfword_count = 0x10u;
		source_slot = address_of(current_object) + 0x18u;
		target_slot = address_of(current_object) + 0x28u;
		page_count = 4u;
		do
		{
			r12 = TILE_PAGE_PLACES(source_slot)[0];
			move_string(r12, halfword_count, TILE_PAGE_PLACES(target_slot)[0], halfword_count, 2u, 0u, 0u, 0u, &target_end, &source_end);
			TILE_PAGE_PLACES(source_slot)[0] = source_end;
			TILE_PAGE_PLACES(target_slot)[0] = target_end;
			page_count--;
			source_slot += 4u;
			target_slot += 4u;
		} while (page_count != 0u);
		step_2 = current_object->road_tiles_change.step;
		current_object->road_tiles_change.step = step_2 - 1u;
		if (step_2 != 1u)
			return;
		table_source = data + 0x34u;
		table_place = current_object->road_tiles_change.road + 0x18u + current_object->road_tiles_change.table_half;
		byte_count = 0x1Eu;
		do
		{
			ROAD_PAGE_TABLE_BYTES(table_place)[0] = ROAD_TILES_CHANGE_BYTES(table_source)[0];
			byte_count--;
			table_source++;
			table_place += 2u;
		} while (byte_count != 0u);
		current_object->state++;
	case 3:
		if (TILES_CHANGE_FLAGS(current_object->road_tiles_change.flag)[0] != 0u)
			return;
		source_slot = address_of(current_object) + 0x18u;
		target_slot = address_of(current_object) + 0x28u;
		page_set = OBJECT(current_object->road_tiles_change.road)->road_surface.page_set;
		current_object->road_tiles_change.page_set = page_set;
		r12 = data + page_set * 4u;
		target_page_numbers = data + 0x18u + screen->index * 4u;
		page_count = 4u;
		do
		{
			TILE_PAGE_PLACES(source_slot)[0] = 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(r12)[0] << 8) * 4u;
			target_page_back = ROAD_TILES_CHANGE_BYTES(target_page_numbers)[0];
			target_page_numbers++;
			TILE_PAGE_PLACES(target_slot)[0] = 0x300000u + ((uint32_t)target_page_back << 8) * 4u;
			target_slot += 4u;
			page_count--;
			r12++;
			source_slot += 4u;
		} while (page_count != 0u);
		current_object->road_tiles_change.step = 0u;
		current_object->road_tiles_change.pages_left = 4u;
		current_object->state++;
	case 4:
		step = current_object->road_tiles_change.step;
		move_halfwords(current_object->road_tiles_change.target_pages[(int32_t)step], current_object->road_tiles_change.source_pages[(int32_t)step], 0x200u);
		current_object->road_tiles_change.step++;
		pages_left_2 = current_object->road_tiles_change.pages_left;
		current_object->road_tiles_change.pages_left = pages_left_2 - 1u;
		if (pages_left_2 != 1u)
			return;
		table_source = data + 0x20u + screen->index * 5u * 2u;
		table_place = current_object->road_tiles_change.road + 0x18u + current_object->road_tiles_change.table_half;
		row_count = 3u;
		do
		{
			r12 = table_source;
			row_bytes = 0xAu;
			do
			{
				ROAD_PAGE_TABLE_BYTES(table_place)[0] = ROAD_TILES_CHANGE_BYTES(r12)[0];
				row_bytes--;
				r12++;
				table_place += 2u;
			} while (row_bytes != 0u);
			row_count--;
		} while (row_count != 0u);
		source_slot = address_of(current_object) + 0x18u;
		page_numbers = data - 0xCu + current_object->road_tiles_change.page_set * 4u;
		page_count = 4u;
		do
		{
			TILE_PAGE_PLACES(source_slot)[0] = 0x300000u + ((uint32_t)ROAD_TILES_CHANGE_BYTES(page_numbers)[0] << 8) * 4u;
			page_count--;
			page_numbers++;
			source_slot += 4u;
		} while (page_count != 0u);
		current_object->road_tiles_change.step = 0x10u;
		current_object->state++;
		return;
	case 5:
		halfword_count = 0x10u;
		source_slot = address_of(current_object) + 0x18u;
		target_slot = address_of(current_object) + 0x28u;
		page_count = 4u;
		do
		{
			r12 = TILE_PAGE_PLACES(source_slot)[0];
			move_string(r12, halfword_count, TILE_PAGE_PLACES(target_slot)[0], halfword_count, 2u, 0u, 0u, 0u, &target_end, &source_end);
			TILE_PAGE_PLACES(source_slot)[0] = source_end;
			TILE_PAGE_PLACES(target_slot)[0] = target_end;
			page_count--;
			source_slot += 4u;
			target_slot += 4u;
		} while (page_count != 0u);
		step_3 = current_object->road_tiles_change.step;
		current_object->road_tiles_change.step = step_3 - 1u;
		if (step_3 != 1u)
			return;
		table_source = data + 0x52u;
		table_place = current_object->road_tiles_change.road + 0x18u + current_object->road_tiles_change.table_half;
		byte_count = 0x1Eu;
		do
		{
			ROAD_PAGE_TABLE_BYTES(table_place)[0] = ROAD_TILES_CHANGE_BYTES(table_source)[0];
			byte_count--;
			table_source++;
			table_place += 2u;
		} while (byte_count != 0u);
		current_object->flags = 0u;
		return;
	}
}
