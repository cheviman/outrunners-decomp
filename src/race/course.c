/**
 *    Name:        race/course.c
 *    Description: the course: its segments built ahead of the car, the road projected for the view
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the race's course (stage_courses by screen first_stage, car course): clears the ring of
   segments, the first road width, and builds the segments as far as the view reaches (screen
   draw_distance >> 7, prebuilt_segments); the first stage into the stage history. */
void player_car_start_course(void)
{
	uint16_t ring_mask = 0, width_code = 0;
	uint32_t course = 0;

	move_string(zero_block_ADDRESS, 0x200u, screen + 0x2000u, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
	course = stage_courses[SCREEN(screen)->first_stage];
	CAR(player_car)->course = course;
	width_code = COURSE_START(course)->width_code;
	CAR(player_car)->width_code = (uint8_t)width_code;
	CAR(player_car)->road_width = width_code * 0xB0u;
	CAR(player_car)->prebuilt_segments = SCREEN(screen)->draw_distance >> 7;
	ring_mask = 0xFFFu;
	do
	{
		player_car_build_segment(ring_mask);
	} while ((int16_t)CAR(player_car)->built_segments < (int16_t)CAR(player_car)->prebuilt_segments);
	SCREEN(screen)->current_stage_level = SCREEN(screen)->built_stage_level;
	CAR(player_car)->stage_history = SCREEN(screen)->route_stage;
}

/* Keeps the course built 104 segments past the window's start (window_record, build_end_offset). At a
   fork the building waits (at_fork) until the checkpoint is passed (screen forks_decided): then the
   branch taken (screen 349 + the fork) goes on: branch, the course of that branch (car course), its
   stage (screen route_stage, stage indicator A5), the stage and branch histories and forks_taken, the
   stage's data queued (queue_stage_load) and the branch_arrow. */
void player_car_extend_course(void)
{
	uint8_t branch_stages = 0;
	uint16_t ring_mask = 0, run_width = 0, window_offset = 0;
	uint32_t branch = 0, course = 0, course_place = 0, forks_reached = 0, forks_taken = 0, object = 0, race = 0, run_length = 0;

	ring_mask = 0xFFFu;
	if (CAR(player_car)->at_fork == 0u)
	{
		window_offset = CAR(player_car)->window_offset;
		CAR(player_car)->window_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + window_offset);
		CAR(player_car)->build_end_offset = (window_offset + 0xD00u) & 0xFFFu;
		if (CAR(player_car)->build_offset != CAR(player_car)->build_end_offset)
		{
			do
			{
				if (CAR(player_car)->at_fork != 0u)
					break;
				player_car_build_segment(ring_mask);
			} while (CAR(player_car)->build_offset != CAR(player_car)->build_end_offset);
		}
		return;
	}
	forks_reached = CAR(player_car)->forks_reached;
	if (SCREEN(screen)->forks_decided < forks_reached)
		return;
	course = CAR(player_car)->course;
	branch = mem_read8(screen + 0x349u + forks_reached);
	CAR(player_car)->branch = branch;
	branch_stages = COURSE_FORK(course)->branch_stages[branch];
	CAR(player_car)->course = COURSE_FORK(course)->branch_courses[branch];
	SCREEN(screen)->route_stage = branch_stages;
	CAR(player_car)->new_branch = 0xFFu;
	SCREEN(screen)->route_stage_level++;
	forks_taken = CAR(player_car)->forks_taken;
	mem_write8(player_car + 0xD7u + forks_taken, branch);
	forks_taken = (uint8_t)(forks_taken + 1u);
	mem_write8(player_car + 0xD1u + forks_taken, branch_stages);
	CAR(player_car)->forks_taken = forks_taken;
	CAR(player_car)->at_fork = 0u;
	queue_stage_load();
	object = start_object();
	OBJECT(object)->handler = branch_arrow_update_ADDRESS;
	for (;;)
	{
		CAR(player_car)->course_records_read++;
		course_place = CAR(player_car)->course;
		run_length = (int16_t)ROM16(course_place);
		course_place += 2u;
		if ((int16_t)run_length > 0)
			break;
		switch (0u - run_length)
		{
		case 0:
			OBJECT(current_object)->state = 3u;
			OBJECT(current_object)->player_car.step = 0u;
			OBJECT(current_object)->player_car.state_high = 0u;
			return;
		case 6:
			object = start_object();
			OBJECT(object)->handler = goal_update_ADDRESS;
			OBJECT(object)->position = CAR(player_car)->built_segments;
			CAR(player_car)->course = course_place;
			continue;
		case 1:
			CAR(player_car)->course = ROM32(course_place);
			continue;
		case 2:
			race = SCREEN(screen)->race;
			CAR(player_car)->forks_reached++;
			RACE(race)->stage_count++;
			CAR(player_car)->course = course_place;
			CAR(player_car)->at_fork = 0xFFu;
			return;
		case 3:
			CAR(player_car)->pitch_angle = 0u;
			CAR(player_car)->course = course_place + 4u;
			continue;
		case 4:
			SCREEN(screen)->built_stage_level = ROM8(course_place);
			course_place++;
			SCREEN(screen)->built_stage = ROM8(course_place);
			CAR(player_car)->stage_segments = 0xFFFFu;
			CAR(player_car)->course = course_place + 1u;
			CAR(player_car)->stage_start = CAR(player_car)->built_segments;
			continue;
		case 5:
			CAR(player_car)->heading_angle = 0u;
			CAR(player_car)->course = course_place + 4u;
			continue;
		default:
			invalid_jump(course_command_jumps[0u - run_length]);
		}
	}
	CAR(player_car)->run_left = run_length;
	CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
	course_place++;
	CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
	course_place++;
	run_width = ROM8(course_place);
	course_place++;
	CAR(player_car)->road_event = ROM8(course_place);
	CAR(player_car)->course = course_place + 1u;
	CAR(player_car)->width_code = (uint8_t)run_width;
	run_width = (uint16_t)(run_width * 0xB0u);
	CAR(player_car)->road_width_target = run_width;
	CAR(player_car)->road_width_step = (int16_t)(run_width - CAR(player_car)->road_width) >> 6;
	player_car_build_segment(ring_mask);
}

/* Builds the next segment of the road from the course (car course): at the end of a run reads the next
   records: 0 the course ends (the car's state 3), 1 a jump to another part, 2 a fork (forks_reached, the
   race's stage count, at_fork), 3 and 5 the pitch and the heading back to 0, 4 a new stage (screen
   built_stage_level, built_stage; stage_segments, stage_start), 6 the goal (a goal object at this
   segment), else a run (its length, pitch and heading steps, road width, a byte for the segments). The
   segment: heading and pitch turned by the run's steps, the position moved along them (next_x/y/z), the
   width eased toward the run's; written at build_offset, then built_segments and stage_segments count
   up. */
void player_car_build_segment(uint16_t ring_mask)
{
	uint16_t road_width = 0, run_width = 0;
	uint32_t course_place = 0, heading_cosine = 0, heading_sine = 0, object = 0, pitch_cosine = 0, pitch_sine = 0, race = 0, ring_segment = 0, run_length = 0;
	uint16_t run_left = 0;

	for (;;)
	{
		run_left = CAR(player_car)->run_left;
		CAR(player_car)->run_left = run_left - 1u;
		if ((int16_t)run_left >= 1)
			break;
		for (;;)
		{
			CAR(player_car)->course_records_read++;
			course_place = CAR(player_car)->course;
			run_length = (int16_t)ROM16(course_place);
			course_place += 2u;
			if ((int16_t)run_length > 0)
				break;
			switch (0u - run_length)
			{
			case 0:
				OBJECT(current_object)->state = 3u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->player_car.state_high = 0u;
				return;
			case 6:
				object = start_object();
				OBJECT(object)->handler = goal_update_ADDRESS;
				OBJECT(object)->position = CAR(player_car)->built_segments;
				CAR(player_car)->course = course_place;
				continue;
			case 1:
				CAR(player_car)->course = ROM32(course_place);
				continue;
			case 2:
				race = SCREEN(screen)->race;
				CAR(player_car)->forks_reached++;
				RACE(race)->stage_count++;
				CAR(player_car)->course = course_place;
				CAR(player_car)->at_fork = 0xFFu;
				return;
			case 3:
				CAR(player_car)->pitch_angle = 0u;
				CAR(player_car)->course = course_place + 4u;
				continue;
			case 4:
				SCREEN(screen)->built_stage_level = ROM8(course_place);
				course_place++;
				SCREEN(screen)->built_stage = ROM8(course_place);
				CAR(player_car)->stage_segments = 0xFFFFu;
				CAR(player_car)->course = course_place + 1u;
				CAR(player_car)->stage_start = CAR(player_car)->built_segments;
				continue;
			case 5:
				CAR(player_car)->heading_angle = 0u;
				CAR(player_car)->course = course_place + 4u;
				continue;
			default:
				invalid_jump(course_command_jumps[0u - run_length]);
			}
		}
		CAR(player_car)->run_left = run_length;
		CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
		course_place++;
		CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
		course_place++;
		run_width = ROM8(course_place);
		course_place++;
		CAR(player_car)->road_event = ROM8(course_place);
		CAR(player_car)->course = course_place + 1u;
		CAR(player_car)->width_code = (uint8_t)run_width;
		run_width = (uint16_t)(run_width * 0xB0u);
		CAR(player_car)->road_width_target = run_width;
		CAR(player_car)->road_width_step = (int16_t)(run_width - CAR(player_car)->road_width) >> 6;
	}
	CAR(player_car)->width_changing = 0xFFu;
	if ((uint16_t)CAR(player_car)->road_width_step == 0u)
		CAR(player_car)->width_changing = 0u;
	else if (CAR(player_car)->road_width_step < 0)
	{
		CAR(player_car)->road_width += CAR(player_car)->road_width_step;
		if (CAR(player_car)->road_width <= CAR(player_car)->road_width_target)
		{
			CAR(player_car)->road_width = CAR(player_car)->road_width_target;
			CAR(player_car)->road_width_step = 0;
			CAR(player_car)->width_changing = 0u;
		}
	}
	else
	{
		CAR(player_car)->road_width += CAR(player_car)->road_width_step;
		if (CAR(player_car)->road_width_target <= CAR(player_car)->road_width)
		{
			CAR(player_car)->road_width = CAR(player_car)->road_width_target;
			CAR(player_car)->road_width_step = 0;
			CAR(player_car)->width_changing = 0u;
		}
	}
	CAR(player_car)->heading_angle = (((uint32_t)(int16_t)CAR(player_car)->heading_step << 8) + CAR(player_car)->heading_angle) & 0xFFFFFFFu;
	CAR(player_car)->pitch_angle = (((uint32_t)(int16_t)CAR(player_car)->pitch_step << 8) + CAR(player_car)->pitch_angle) & 0xFFFFFFFu;
	pitch_cosine = (uint16_t)(CAR(player_car)->pitch_angle >> 16u);
	heading_cosine = (uint16_t)(CAR(player_car)->heading_angle >> 16u);
	pitch_sine = sine_table[pitch_cosine];
	pitch_cosine = cosine_table[pitch_cosine];
	heading_sine = sine_table[heading_cosine];
	heading_cosine = cosine_table[heading_cosine];
	CAR(player_car)->next_x_fine += (int32_t)(heading_sine * pitch_cosine) >> 13;
	CAR(player_car)->next_y_fine -= pitch_sine << 1;
	CAR(player_car)->next_z_fine += (int32_t)(pitch_cosine * heading_cosine) >> 13;
	CAR(player_car)->next_x = CAR(player_car)->next_x_fine;
	CAR(player_car)->next_x = (int32_t)CAR(player_car)->next_x >> 8;
	CAR(player_car)->next_y = CAR(player_car)->next_y_fine;
	CAR(player_car)->next_y = (int32_t)CAR(player_car)->next_y >> 8;
	CAR(player_car)->next_z = CAR(player_car)->next_z_fine;
	CAR(player_car)->next_z = (int32_t)CAR(player_car)->next_z >> 8;
	ring_segment = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->build_offset);
	ROAD_SEGMENT(ring_segment)->heading = (uint16_t)(CAR(player_car)->heading_angle >> 16u);
	ROAD_SEGMENT(ring_segment)->pitch = (uint16_t)(CAR(player_car)->pitch_angle >> 16u);
	ROAD_SEGMENT(ring_segment)->x = CAR(player_car)->next_x;
	ROAD_SEGMENT(ring_segment)->z = CAR(player_car)->next_z;
	ROAD_SEGMENT(ring_segment)->y = CAR(player_car)->next_y;
	ROAD_SEGMENT(ring_segment)->road_event = CAR(player_car)->road_event;
	road_width = CAR(player_car)->road_width;
	ROAD_SEGMENT(ring_segment)->width = road_width;
	ROAD_SEGMENT(ring_segment)->half_width = road_width >> 1;
	if (CAR(player_car)->width_changing == 0u)
		ROAD_SEGMENT(ring_segment)->width_code = CAR(player_car)->width_code;
	else
		ROAD_SEGMENT(ring_segment)->width_code = 0u;
	CAR(player_car)->build_offset = (ring_segment + 0x20u) & ring_mask;
	CAR(player_car)->built_segments++;
	CAR(player_car)->stage_segments++;
}

/* Projects the road for the view of screen shown_view (player_car_publish): its rotation (view_matrix_*,
   from the view's pitch and yaw), then each segment from the window's start out to the view's distance
   turned into the view and scaled by its depth into the screen's list at screen + 3000 (scale, width, x,
   y, the segment's byte), segments behind the view or too far left out. */
void player_car_project_road(void)
{
	uint32_t carry = 0;
	uint16_t previous_y = 0, screen_top_y = 0;
	uint32_t car = 0, draw_distance = 0, fixed_point_shift = 0, matrix_xx = 0, matrix_xz = 0, matrix_yy = 0, matrix_zx = 0, matrix_zy = 0, matrix_zz = 0, near_limit = 0, previous_projected = 0, projected_segment = 0, projection_pattern = 0, ring_segment = 0, scale = 0, segments_left = 0, to_view_x = 0, to_view_y = 0, to_view_z = 0, unrotated_z = 0, view_pitch = 0, view_yaw = 0, x = 0;
	uint8_t projected = 0;
	uint16_t last_y = 0;
	uint32_t copied_x_y = 0, copied_road_bytes = 0, width_code_halfword = 0, width_code_halfword_2 = 0;

	view_pitch = VIEW(SCREEN(screen)->shown_view)->pitch;
	view_yaw = VIEW(SCREEN(screen)->shown_view)->yaw;
	matrix_zy = sine_table[view_pitch];
	matrix_zx = sine_table[view_yaw];
	matrix_yy = cosine_table[view_pitch];
	matrix_xx = cosine_table[view_yaw];
	fixed_point_shift = 0xFFFFFFF2u;
	matrix_xz = 0u - matrix_zx;
	r12 = matrix_zy * matrix_zx;
	carry = (r12 >> 13) & 1u;
	r12 = ((int32_t)r12 >> 14) + carry;
	r14 = matrix_zy * matrix_xx;
	carry = (r14 >> 13) & 1u;
	r14 = ((int32_t)r14 >> 14) + carry;
	matrix_zx = matrix_zx * matrix_yy;
	carry = (matrix_zx >> 13) & 1u;
	matrix_zx = ((int32_t)matrix_zx >> 14) + carry;
	matrix_zy = 0u - matrix_zy;
	matrix_zz = matrix_yy * matrix_xx;
	carry = (matrix_zz >> 13) & 1u;
	matrix_zz = ((int32_t)matrix_zz >> 14) + carry;
	car = player_car + 0xDCu;
	CAR(car - 0xDCu)->view_matrix_xx = matrix_xx;
	car += 4u;
	CAR(car - 0xE0u)->view_matrix_xy = 0;
	car += 4u;
	CAR(car - 0xE4u)->view_matrix_xz = matrix_xz;
	car += 4u;
	CAR(car - 0xE8u)->view_matrix_yx = r12;
	car += 4u;
	CAR(car - 0xECu)->view_matrix_yy = matrix_yy;
	car += 4u;
	CAR(car - 0xF0u)->view_matrix_yz = r14;
	car += 4u;
	CAR(car - 0xF4u)->view_matrix_zx = matrix_zx;
	car += 4u;
	CAR(car - 0xF8u)->view_matrix_zy = matrix_zy;
	CAR(car - 0xF8u)->view_matrix_zz = matrix_zz;
	to_view_x = 0u - VIEW(SCREEN(screen)->shown_view)->x;
	to_view_y = 0u - VIEW(SCREEN(screen)->shown_view)->y;
	to_view_z = 0u - VIEW(SCREEN(screen)->shown_view)->z;
	near_limit = SCREEN(screen)->eye_distance;
	near_limit = near_limit >> 1;
	draw_distance = SCREEN(screen)->draw_distance;
	screen_top_y = (uint16_t)(0u - SCREEN(screen)->sprite_origin_y);
	ring_segment = screen + 0x1FE0u + VIEW(SCREEN(screen)->shown_view)->window_offset;
	projected_segment = screen + 0x2FF0u;
	segments_left = draw_distance >> 7;
	SCREEN(screen)->crest_count = 0u;
	previous_y = 0x7FFFu;
	SCREEN(screen)->crest_heights = screen_top_y + 0xE0u;
	projection_pattern = segment_projection_pattern_ADDRESS;
	for (;;)
	{
		ring_segment = ((ring_segment + 0x20u) & ~0xFFFFu) | ((ring_segment + 0x20u) & 0xAFFFu);
		projected = ROAD_PROJECTION_PATTERN(projection_pattern)[0];
		projection_pattern++;
		projected_segment += 0x10u;
		if (projected == 0u)
		{
			previous_projected = projected_segment - 0x10u;
			copied_x_y = (PROJECTED_SEGMENT(previous_projected)->x | ((uint32_t)PROJECTED_SEGMENT(previous_projected)->y << 16u));
			PROJECTED_SEGMENT(projected_segment)->scale = PROJECTED_SEGMENT(previous_projected)->scale;
			PROJECTED_SEGMENT(projected_segment)->width = PROJECTED_SEGMENT(previous_projected)->width;
			PROJECTED_SEGMENT(projected_segment)->x = (uint16_t)copied_x_y;
			PROJECTED_SEGMENT(projected_segment)->y = (uint16_t)(copied_x_y >> 16u);
			previous_projected += 8u;
			copied_road_bytes = AS32(PROJECTED_SEGMENT(previous_projected - 0x8u)->width_code);
			AS32(PROJECTED_SEGMENT(projected_segment)->depth_layer) = AS32(PROJECTED_SEGMENT(previous_projected - 0x8u)->depth_layer);
			AS32(PROJECTED_SEGMENT(projected_segment)->width_code) = copied_road_bytes;
			width_code_halfword = mem_read16(ring_segment + 0x14u);
			PROJECTED_SEGMENT(projected_segment)->width_code = (uint8_t)width_code_halfword;
			PROJECTED_SEGMENT(projected_segment)->road_event = (uint8_t)(width_code_halfword >> 8u);
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		x = to_view_x + mem_read32(ring_segment + 4u);
		r27 = to_view_y;
		r27 = r27 + mem_read32(ring_segment + 0xCu);
		r28 = to_view_z;
		r28 = r28 + mem_read32(ring_segment + 8u);
		unrotated_z = r28;
		r28 = r28 * matrix_zz;
		r28 = r28 + x * matrix_zx;
		r28 = r28 + r27 * matrix_zy;
		r28 = r28 + fixed_point_shift;
		r28 = (int32_t)r28 >> 14;
		r27 = r27 * matrix_yy;
		r27 = r27 + x * r12;
		r27 = r27 + unrotated_z * r14;
		r27 = r27 + fixed_point_shift;
		r27 = (int32_t)r27 >> 14;
		x = (int32_t)(x * matrix_xx + unrotated_z * matrix_xz + fixed_point_shift) >> 14;
		if ((int32_t)draw_distance <= (int32_t)r28 || (int32_t)r28 <= (int32_t)near_limit)
		{
			PROJECTED_SEGMENT(projected_segment)->scale = 0u;
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				r27 = (r27 & ~0xFFFFu) | previous_y;
				r28 = (r28 & ~0xFFFFu) | 3u;
				note_road_crest();
				return;
			}
			continue;
		}
		scale = perspective_scales[r28];
		PROJECTED_SEGMENT(projected_segment)->scale = scale;
		x = x * scale;
		r27 = r27 * scale;
		carry = (x >> 11) & 1u;
		x = ((int32_t)x >> 12) + carry;
		carry = (r27 >> 11) & 1u;
		r27 = (int32_t)r27 >> 12;
		r27 = r27 + carry;
		scale = scale * mem_read16(ring_segment + 0x10u);
		carry = (scale >> 11) & 1u;
		PROJECTED_SEGMENT(projected_segment)->width = ((int32_t)scale >> 12) + carry;
		width_code_halfword_2 = mem_read16(ring_segment + 0x14u);
		PROJECTED_SEGMENT(projected_segment)->width_code = (uint8_t)width_code_halfword_2;
		PROJECTED_SEGMENT(projected_segment)->road_event = (uint8_t)(width_code_halfword_2 >> 8u);
		PROJECTED_SEGMENT(projected_segment)->x = x;
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(0u - r27);
		PROJECTED_SEGMENT(projected_segment)->y = r27;
		r28 = depth_layers_by_depth[r28];
		PROJECTED_SEGMENT(projected_segment)->depth_layer = r28;
		last_y = previous_y;
		previous_y = (uint16_t)r27;
		if ((int16_t)last_y < (int16_t)r27)
		{
			note_road_crest();
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		segments_left--;
		if (segments_left == 0u)
			break;
	}
	mem_write16(projected_segment + 0x10u, 0u);
	note_road_crest();
}

/* Notes a crest of the road while player_car_project_road projects it, for crest_clip_update: when it is
   the first, or higher on the screen (R27) than the last one noted, crest_count goes up, its depth layer
   R28 goes into crest_layers (at the old count, with 3 in the next place) and its height into
   crest_heights (at the new count; entry 0 is the bottom of the screen). */
void note_road_crest(void)
{
	uint32_t crest_count = 0;

	crest_count = SCREEN(screen)->crest_count;
	if (crest_count == 0u || (int16_t)r27 < (int16_t)mem_read16(screen + 0x1F30u + crest_count * 2u))
	{
		SCREEN(screen)->crest_count++;
		mem_write16(screen + 0x1F10u + crest_count * 2u, r28);
		mem_write16(screen + 0x1F12u + crest_count * 2u, 3u);
		mem_write16(screen + 0x1F32u + crest_count * 2u, r27);
	}
}

/* Turns an offset (R1 across, R2 along, R0 up) by a heading (R3) and a pitch (R4) into the world's axes.
   Used by player_car_world_position and player_car_place_view. */
void rotate_offset(uint32_t y, uint32_t road_heading, uint32_t road_pitch, uint32_t *p_x, uint32_t *p_z)
{
	uint32_t x = *p_x;
	uint32_t z = *p_z;
	uint32_t heading_cosine = 0, heading_sine = 0, minus_x = 0, pitch_cosine = 0, pitch_sine = 0, y_copy = 0;

	pitch_sine = sine_table[road_pitch];
	pitch_cosine = cosine_table[road_pitch];
	heading_cosine = cosine_table[road_heading];
	heading_sine = sine_table[road_heading];
	y_copy = y;
	minus_x = 0u - x;
	x = (int32_t)(x * heading_cosine + ((int32_t)(y_copy * heading_sine) >> 14) * pitch_sine + ((int32_t)(z * heading_sine) >> 14) * pitch_cosine) >> 14;
	z = (int32_t)(((int32_t)(z * heading_cosine) >> 14) * pitch_cosine + (minus_x * heading_sine + ((int32_t)(y_copy * heading_cosine) >> 14) * pitch_sine)) >> 14;
	*p_x = x;
	*p_z = z;
}
