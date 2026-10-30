/**
 *    Name:        race/course.c
 *    Description: the course: its segments built ahead of the car, the road projected for the view
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
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

	move_halfwords(address_of(screen) + 0x2000u, zero_block_ADDRESS, 0x200u);
	course = stage_courses[screen->first_stage];
	player_car->course = course;
	width_code = COURSE_START(course)->width_code;
	player_car->width_code = (uint8_t)width_code;
	player_car->road_width = width_code * 0xB0u;
	player_car->prebuilt_segments = screen->draw_distance >> 7;
	ring_mask = 0xFFFu;
	do
	{
		player_car_build_segment(ring_mask);
	} while ((int16_t)player_car->built_segments < (int16_t)player_car->prebuilt_segments);
	screen->current_stage_level = screen->built_stage_level;
	player_car->stage_history[0] = screen->route_stage;
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
	uint32_t branch = 0, course = 0, course_place = 0, forks_reached = 0, forks_taken = 0, run_length = 0;
	struct object *object = NULL;
	struct race *race = NULL;

	ring_mask = 0xFFFu;
	if (player_car->at_fork == 0u)
	{
		window_offset = player_car->window_offset;
		player_car->window_record = PAGE_ADD(address_of(screen) + 0x2000u, window_offset);
		player_car->build_end_offset = (window_offset + 0xD00u) & 0xFFFu;
		if (player_car->build_offset != player_car->build_end_offset)
		{
			do
			{
				if (player_car->at_fork != 0u)
					break;
				player_car_build_segment(ring_mask);
			} while (player_car->build_offset != player_car->build_end_offset);
		}
		return;
	}
	forks_reached = player_car->forks_reached;
	if (screen->forks_decided < forks_reached)
		return;
	course = player_car->course;
	branch = screen->route_branches[(int32_t)(forks_reached - 1u)];
	player_car->branch = branch;
	branch_stages = COURSE_FORK(course)->branch_stages[(int32_t)branch];
	player_car->course = COURSE_FORK(course)->branch_courses[(int32_t)branch];
	screen->route_stage = branch_stages;
	player_car->new_branch = 0xFFu;
	screen->route_stage_level++;
	forks_taken = player_car->forks_taken;
	player_car->branch_history[(int32_t)forks_taken] = branch;
	forks_taken = (uint8_t)(forks_taken + 1u);
	player_car->stage_history[(int32_t)forks_taken] = branch_stages;
	player_car->forks_taken = forks_taken;
	player_car->at_fork = 0u;
	queue_stage_load();
	object = start_object();
	object->handler = branch_arrow_update_ADDRESS;
	for (;;)
	{
		player_car->course_records_read++;
		course_place = player_car->course;
		run_length = (int16_t)AS16(COURSE_BYTES(course_place)[0]);
		course_place += 2u;
		if ((int16_t)run_length > 0)
			break;
		switch (0u - run_length)
		{
		case 0:
			current_object->state = 3u;
			current_object->player_car.step = 0u;
			current_object->player_car.state_high = 0u;
			return;
		case 6:
			object = start_object();
			object->handler = goal_update_ADDRESS;
			object->position = player_car->built_segments;
			player_car->course = course_place;
			continue;
		case 1:
			player_car->course = AS32(COURSE_BYTES(course_place)[0]);
			continue;
		case 2:
			race = pointer_to(screen->race);
			player_car->forks_reached++;
			race->stage_count++;
			player_car->course = course_place;
			player_car->at_fork = 0xFFu;
			return;
		case 3:
			player_car->pitch_angle = 0u;
			player_car->course = course_place + 4u;
			continue;
		case 4:
			screen->built_stage_level = COURSE_BYTES(course_place)[0];
			course_place++;
			screen->built_stage = COURSE_BYTES(course_place)[0];
			player_car->stage_segments = 0xFFFFu;
			player_car->course = course_place + 1u;
			player_car->stage_start = player_car->built_segments;
			continue;
		case 5:
			player_car->heading_angle = 0u;
			player_car->course = course_place + 4u;
			continue;
		}
	}
	player_car->run_left = run_length;
	player_car->pitch_step = (uint32_t)(int8_t)COURSE_BYTES(course_place)[0] << 8;
	course_place++;
	player_car->heading_step = (uint32_t)(int8_t)COURSE_BYTES(course_place)[0] << 8;
	course_place++;
	run_width = COURSE_BYTES(course_place)[0];
	course_place++;
	player_car->road_event = COURSE_BYTES(course_place)[0];
	player_car->course = course_place + 1u;
	player_car->width_code = (uint8_t)run_width;
	run_width = (uint16_t)(run_width * 0xB0u);
	player_car->road_width_target = run_width;
	player_car->road_width_step = (int16_t)(run_width - player_car->road_width) >> 6;
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
	uint32_t course_place = 0, heading_cosine = 0, heading_sine = 0, pitch_cosine = 0, pitch_sine = 0, run_length = 0;
	struct object *object = NULL;
	struct race *race = NULL;
	struct road_segment *ring_segment = NULL;
	uint16_t run_left = 0;

	for (;;)
	{
		run_left = player_car->run_left;
		player_car->run_left = run_left - 1u;
		if ((int16_t)run_left >= 1)
			break;
		for (;;)
		{
			player_car->course_records_read++;
			course_place = player_car->course;
			run_length = (int16_t)AS16(COURSE_BYTES(course_place)[0]);
			course_place += 2u;
			if ((int16_t)run_length > 0)
				break;
			switch (0u - run_length)
			{
			case 0:
				current_object->state = 3u;
				current_object->player_car.step = 0u;
				current_object->player_car.state_high = 0u;
				return;
			case 6:
				object = start_object();
				object->handler = goal_update_ADDRESS;
				object->position = player_car->built_segments;
				player_car->course = course_place;
				continue;
			case 1:
				player_car->course = AS32(COURSE_BYTES(course_place)[0]);
				continue;
			case 2:
				race = pointer_to(screen->race);
				player_car->forks_reached++;
				race->stage_count++;
				player_car->course = course_place;
				player_car->at_fork = 0xFFu;
				return;
			case 3:
				player_car->pitch_angle = 0u;
				player_car->course = course_place + 4u;
				continue;
			case 4:
				screen->built_stage_level = COURSE_BYTES(course_place)[0];
				course_place++;
				screen->built_stage = COURSE_BYTES(course_place)[0];
				player_car->stage_segments = 0xFFFFu;
				player_car->course = course_place + 1u;
				player_car->stage_start = player_car->built_segments;
				continue;
			case 5:
				player_car->heading_angle = 0u;
				player_car->course = course_place + 4u;
				continue;
			}
		}
		player_car->run_left = run_length;
		player_car->pitch_step = (uint32_t)(int8_t)COURSE_BYTES(course_place)[0] << 8;
		course_place++;
		player_car->heading_step = (uint32_t)(int8_t)COURSE_BYTES(course_place)[0] << 8;
		course_place++;
		run_width = COURSE_BYTES(course_place)[0];
		course_place++;
		player_car->road_event = COURSE_BYTES(course_place)[0];
		player_car->course = course_place + 1u;
		player_car->width_code = (uint8_t)run_width;
		run_width = (uint16_t)(run_width * 0xB0u);
		player_car->road_width_target = run_width;
		player_car->road_width_step = (int16_t)(run_width - player_car->road_width) >> 6;
	}
	player_car->width_changing = 0xFFu;
	if ((uint16_t)player_car->road_width_step == 0u)
		player_car->width_changing = 0u;
	else if (player_car->road_width_step < 0)
	{
		player_car->road_width += player_car->road_width_step;
		if (player_car->road_width <= player_car->road_width_target)
		{
			player_car->road_width = player_car->road_width_target;
			player_car->road_width_step = 0;
			player_car->width_changing = 0u;
		}
	}
	else
	{
		player_car->road_width += player_car->road_width_step;
		if (player_car->road_width_target <= player_car->road_width)
		{
			player_car->road_width = player_car->road_width_target;
			player_car->road_width_step = 0;
			player_car->width_changing = 0u;
		}
	}
	player_car->heading_angle = (((uint32_t)(int16_t)player_car->heading_step << 8) + player_car->heading_angle) & 0xFFFFFFFu;
	player_car->pitch_angle = (((uint32_t)(int16_t)player_car->pitch_step << 8) + player_car->pitch_angle) & 0xFFFFFFFu;
	pitch_cosine = (uint16_t)(player_car->pitch_angle >> 16u);
	heading_cosine = (uint16_t)(player_car->heading_angle >> 16u);
	pitch_sine = sine_table[pitch_cosine];
	pitch_cosine = cosine_table[pitch_cosine];
	heading_sine = sine_table[heading_cosine];
	heading_cosine = cosine_table[heading_cosine];
	player_car->next_x_fine += (int32_t)(heading_sine * pitch_cosine) >> 13;
	player_car->next_y_fine -= pitch_sine << 1;
	player_car->next_z_fine += (int32_t)(pitch_cosine * heading_cosine) >> 13;
	player_car->next_x = player_car->next_x_fine;
	player_car->next_x = (int32_t)player_car->next_x >> 8;
	player_car->next_y = player_car->next_y_fine;
	player_car->next_y = (int32_t)player_car->next_y >> 8;
	player_car->next_z = player_car->next_z_fine;
	player_car->next_z = (int32_t)player_car->next_z >> 8;
	ring_segment = pointer_to(PAGE_ADD(address_of(screen) + 0x2000u, player_car->build_offset));
	ring_segment->heading = (uint16_t)(player_car->heading_angle >> 16u);
	ring_segment->pitch = (uint16_t)(player_car->pitch_angle >> 16u);
	ring_segment->x = player_car->next_x;
	ring_segment->z = player_car->next_z;
	ring_segment->y = player_car->next_y;
	ring_segment->road_event = player_car->road_event;
	road_width = player_car->road_width;
	ring_segment->width = road_width;
	ring_segment->half_width = road_width >> 1;
	if (player_car->width_changing == 0u)
		ring_segment->width_code = player_car->width_code;
	else
		ring_segment->width_code = 0u;
	player_car->build_offset = (address_of(ring_segment) + 0x20u) & ring_mask;
	player_car->built_segments++;
	player_car->stage_segments++;
}

/* Projects the road for the view of screen shown_view (player_car_publish): its rotation (view_matrix_*,
   from the view's pitch and yaw), then each segment from the window's start out to the view's distance
   turned into the view and scaled by its depth into the screen's list at screen + 3000 (scale, width, x,
   y, the segment's byte), segments behind the view or too far left out. */
void player_car_project_road(uint32_t *p_matrix_yx, uint32_t *p_matrix_yz)
{
	uint32_t carry = 0;
	uint16_t crest_layer = 0, crest_y = 0, previous_y = 0, screen_top_y = 0;
	uint32_t car = 0, draw_distance = 0, fixed_point_shift = 0, matrix_xx = 0, matrix_xz = 0, matrix_yx = 0, matrix_yy = 0, matrix_yz = 0, matrix_zx = 0, matrix_zy = 0, matrix_zz = 0, near_limit = 0, previous_projected = 0, projection_pattern = 0, scale = 0, segment_depth = 0, segment_y = 0, segments_left = 0, to_view_x = 0, to_view_y = 0, to_view_z = 0, unrotated_z = 0, view_pitch = 0, view_yaw = 0, x = 0;
	struct projected_segment *projected_segment = NULL;
	struct road_segment *ring_segment = NULL;
	uint8_t projected = 0;
	uint16_t last_y = 0;
	uint32_t copied_x_y = 0, copied_road_bytes = 0, width_code_halfword = 0, width_code_halfword_2 = 0;

	view_pitch = VIEW(screen->shown_view)->pitch;
	view_yaw = VIEW(screen->shown_view)->yaw;
	matrix_zy = sine_table[view_pitch];
	matrix_zx = sine_table[view_yaw];
	matrix_yy = cosine_table[view_pitch];
	matrix_xx = cosine_table[view_yaw];
	fixed_point_shift = 0xFFFFFFF2u;
	matrix_xz = 0u - matrix_zx;
	matrix_yx = matrix_zy * matrix_zx;
	carry = (matrix_yx >> 13) & 1u;
	matrix_yx = ((int32_t)matrix_yx >> 14) + carry;
	matrix_yz = matrix_zy * matrix_xx;
	carry = (matrix_yz >> 13) & 1u;
	matrix_yz = ((int32_t)matrix_yz >> 14) + carry;
	matrix_zx = matrix_zx * matrix_yy;
	carry = (matrix_zx >> 13) & 1u;
	matrix_zx = ((int32_t)matrix_zx >> 14) + carry;
	matrix_zy = 0u - matrix_zy;
	matrix_zz = matrix_yy * matrix_xx;
	carry = (matrix_zz >> 13) & 1u;
	matrix_zz = ((int32_t)matrix_zz >> 14) + carry;
	car = address_of(player_car) + 0xDCu;
	CAR(car - 0xDCu)->view_matrix_xx = matrix_xx;
	car += 4u;
	CAR(car - 0xE0u)->view_matrix_xy = 0;
	car += 4u;
	CAR(car - 0xE4u)->view_matrix_xz = matrix_xz;
	car += 4u;
	CAR(car - 0xE8u)->view_matrix_yx = matrix_yx;
	car += 4u;
	CAR(car - 0xECu)->view_matrix_yy = matrix_yy;
	car += 4u;
	CAR(car - 0xF0u)->view_matrix_yz = matrix_yz;
	car += 4u;
	CAR(car - 0xF4u)->view_matrix_zx = matrix_zx;
	car += 4u;
	CAR(car - 0xF8u)->view_matrix_zy = matrix_zy;
	CAR(car - 0xF8u)->view_matrix_zz = matrix_zz;
	to_view_x = 0u - VIEW(screen->shown_view)->x;
	to_view_y = 0u - VIEW(screen->shown_view)->y;
	to_view_z = 0u - VIEW(screen->shown_view)->z;
	near_limit = screen->eye_distance;
	near_limit = near_limit >> 1;
	draw_distance = screen->draw_distance;
	screen_top_y = (uint16_t)(0u - screen->sprite_origin_y);
	ring_segment = pointer_to(address_of(screen) + 0x1FE0u + VIEW(screen->shown_view)->window_offset);
	projected_segment = pointer_to(address_of(screen) + 0x2FF0u);
	segments_left = draw_distance >> 7;
	screen->crest_count = 0u;
	previous_y = 0x7FFFu;
	screen->crest_heights[0] = screen_top_y + 0xE0u;
	projection_pattern = segment_projection_pattern_ADDRESS;
	for (;;)
	{
		ring_segment = pointer_to((address_of(ring_segment) + 0x20u) & ~0x5000u);
		projected = ROAD_PROJECTION_PATTERN(projection_pattern)[0];
		projection_pattern++;
		projected_segment = projected_segment + 1;
		if (projected == 0u)
		{
			previous_projected = address_of(projected_segment) - 0x10u;
			copied_x_y = (PROJECTED_SEGMENT(previous_projected)->x | ((uint32_t)PROJECTED_SEGMENT(previous_projected)->y << 16u));
			projected_segment->scale = PROJECTED_SEGMENT(previous_projected)->scale;
			projected_segment->width = PROJECTED_SEGMENT(previous_projected)->width;
			projected_segment->x = (uint16_t)copied_x_y;
			projected_segment->y = (uint16_t)(copied_x_y >> 16u);
			previous_projected += 8u;
			copied_road_bytes = AS32(PROJECTED_SEGMENT(previous_projected - 0x8u)->width_code);
			AS32(projected_segment->depth_layer) = AS32(PROJECTED_SEGMENT(previous_projected - 0x8u)->depth_layer);
			AS32(projected_segment->width_code) = copied_road_bytes;
			width_code_halfword = ring_segment->width_code_and_event;
			projected_segment->width_code = (uint8_t)width_code_halfword;
			projected_segment->road_event = (uint8_t)(width_code_halfword >> 8u);
			segments_left--;
			if (segments_left != 0u)
				continue;
			projected_segment[1].scale = 0u;
			note_road_crest((uint16_t)segment_y, (uint16_t)segment_depth);
			break;
		}
		x = to_view_x + ring_segment->x;
		segment_y = to_view_y + ring_segment->y;
		segment_depth = to_view_z + ring_segment->z;
		unrotated_z = segment_depth;
		segment_depth = (int32_t)(segment_depth * matrix_zz + x * matrix_zx + segment_y * matrix_zy + fixed_point_shift) >> 14;
		segment_y = (int32_t)(segment_y * matrix_yy + x * matrix_yx + unrotated_z * matrix_yz + fixed_point_shift) >> 14;
		x = (int32_t)(x * matrix_xx + unrotated_z * matrix_xz + fixed_point_shift) >> 14;
		if ((int32_t)draw_distance <= (int32_t)segment_depth || (int32_t)segment_depth <= (int32_t)near_limit)
		{
			projected_segment->scale = 0u;
			segments_left--;
			if (segments_left != 0u)
				continue;
			projected_segment[1].scale = 0u;
			crest_y = (uint16_t)previous_y;
			crest_layer = 3u;
			note_road_crest(crest_y, crest_layer);
			break;
		}
		scale = perspective_scales[segment_depth];
		projected_segment->scale = scale;
		x = x * scale;
		segment_y = segment_y * scale;
		carry = (x >> 11) & 1u;
		x = ((int32_t)x >> 12) + carry;
		carry = (segment_y >> 11) & 1u;
		segment_y = ((int32_t)segment_y >> 12) + carry;
		scale = scale * ring_segment->width;
		carry = (scale >> 11) & 1u;
		projected_segment->width = ((int32_t)scale >> 12) + carry;
		width_code_halfword_2 = ring_segment->width_code_and_event;
		projected_segment->width_code = (uint8_t)width_code_halfword_2;
		projected_segment->road_event = (uint8_t)(width_code_halfword_2 >> 8u);
		projected_segment->x = x;
		segment_y = (uint16_t)(0u - segment_y);
		projected_segment->y = segment_y;
		segment_depth = depth_layers_by_depth[segment_depth];
		projected_segment->depth_layer = segment_depth;
		last_y = previous_y;
		previous_y = (uint16_t)segment_y;
		if ((int16_t)last_y < (int16_t)segment_y)
		{
			note_road_crest((uint16_t)segment_y, (uint16_t)segment_depth);
			segments_left--;
			if (segments_left != 0u)
				continue;
			projected_segment[1].scale = 0u;
			note_road_crest((uint16_t)segment_y, (uint16_t)segment_depth);
			break;
		}
		segments_left--;
		if (segments_left == 0u)
		{
			projected_segment[1].scale = 0u;
			note_road_crest((uint16_t)segment_y, (uint16_t)segment_depth);
			break;
		}
	}
	*p_matrix_yx = matrix_yx;
	*p_matrix_yz = matrix_yz;
}

/* Notes a crest of the road while player_car_project_road projects it, for crest_clip_update: when it is
   the first, or higher on the screen (R27) than the last one noted, crest_count goes up, its depth layer
   R28 goes into crest_layers (at the old count, with 3 in the next place) and its height into
   crest_heights (at the new count; entry 0 is the bottom of the screen). */
void note_road_crest(uint16_t crest_y, uint16_t crest_layer)
{
	uint32_t crest_count = 0;

	crest_count = screen->crest_count;
	if (crest_count == 0u || (int16_t)crest_y < (int16_t)screen->crest_heights[(int32_t)crest_count])
	{
		screen->crest_count++;
		screen->crest_layers[(int32_t)crest_count] = crest_layer;
		screen->crest_layers[(int32_t)(crest_count + 1u)] = 3u;
		screen->crest_heights[(int32_t)(crest_count + 1u)] = crest_y;
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
