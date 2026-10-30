/**
 *    Name:        objects/scenery.c
 *    Description: the road's objects: the streams that start them along the course, the scenery, the signs, the start gate and flagmen, the goal parades, the fork's branch
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

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
		OBJECT(current_object)->road_object_stream.stream = ROAD_OBJECT_STREAM_FORKS(OBJECT(current_object)->road_object_stream.stream)[CAR(player_car)->branch];
		break;
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
	OBJECT(object)->parameter = ROAD_OBJECT_STREAM_ENTRY(entry - 0x8u)->parameter;
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
	uint32_t crest = 0;

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
	}
	if (SCREEN(screen)->crest_count == 0u)
		return;
	OBJECT(current_object)->crest_clip.crest = 0u;
	OBJECT(current_object)->depth_layer = 0xFFFFu;
	do
	{
		crest = OBJECT(current_object)->crest_clip.crest;
		depth_layer = SCREEN(screen)->crest_layers[(int32_t)crest];
		if (depth_layer < OBJECT(current_object)->depth_layer)
		{
			OBJECT(current_object)->depth_layer = depth_layer;
			OBJECT(current_object)->crest_clip.clip_bottom = SCREEN(screen)->crest_heights[(int32_t)crest] + SCREEN(screen)->sprite_origin_y - 1u;
			draw_object(depth_layer);
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
		road_row_draw(along_distance, next_hit_box, &r12, &r14);
		return;
	case 2:
		road_row_draw(0u, 0u, &r12, &r14);
		return;
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
		road_row_draw(along_distance, next_hit_box, &r12, &r14);
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
		road_row_draw(along_distance, next_hit_box, &r12, &r14);
		return;
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
	}
	road_row_pair_draw(along_distance, next_hit_box, &r12, &r14);
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
		road_row_draw(animation_order, 0u, &r12, &r14);
		return;
	}
}

/* The northern lights over stage 26 (stream 26 at 1040: two of them, with their own lists): drawn at a
   screen position (draw_mode 0) that slides in from x -160 to 160, 8 a frame (y 40, or 30 for car model
   6, car model), then stays; it steps through its list of animations (aurora_next_animation, each shown
   animation_frames frames, aurora_count_down) and is drawn only every other frame (when countdown is
   odd), so it looks faint. It never ends. */
void aurora_update(void)
{
	uint32_t record = 0;

	switch (OBJECT(current_object)->state)
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
		if ((OBJECT(current_object)->aurora.countdown & 1u) == 0u)
			return;
		animation_advance(0u);
		draw_object(0u);
		return;
	case 2:
		break;
	}
	aurora_count_down();
	if ((OBJECT(current_object)->aurora.countdown & 1u) == 0u)
		return;
	animation_advance(0u);
	draw_object(0u);
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
		road_row_draw(0u, 0u, &r12, &r14);
		return;
	case 1:
		fork_sign_hit_check(&along_distance, &next_hit_box);
		road_row_draw(along_distance, next_hit_box, &r12, &r14);
		return;
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
	}
	along_distance = animation_advance(along_distance);
	road_row_draw(along_distance, next_hit_box, &r12, &r14);
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
	}
	along_distance = animation_advance(along_distance);
	road_row_pair_draw(along_distance, next_hit_box, &r12, &r14);
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
	uint32_t animation = 0, crossing_animation = 0, road_crosser_record = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		road_crosser_record = OBJECT(current_object)->road_crosser.record;
		animation = ROAD_CROSSER_RECORD(road_crosser_record)->animation;
		road_crosser_record += 2u;
		if (animation == SWORDFISH)
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
			draw_object(0u);
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
				draw_object(0u);
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
			draw_object(0u);
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
				draw_object(0u);
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
				draw_object(0u);
				if (OBJECT(current_object)->road_crosser.screen_x >= -0xF0 && OBJECT(current_object)->road_crosser.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
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
	uint16_t draw_distance = 0, position = 0, position_fraction = 0;
	uint32_t animation = 0, hop_animation = 0, road_crosser_record = 0;

	switch (OBJECT(current_object)->state)
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
			draw_object(0u);
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
			position_fraction -= 0x80u;
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
			draw_object(0u);
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
			position_fraction -= 0x80u;
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
			position_fraction -= 0x80u;
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
	uint32_t animation = 0, moving_scenery_record = 0;

	switch (OBJECT(current_object)->state)
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
				draw_object(0u);
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
				draw_object(0u);
				if (OBJECT(current_object)->moving_scenery.screen_x >= -0xF0 && OBJECT(current_object)->moving_scenery.screen_x <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
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
	uint32_t animation = 0, swooping_helicopter_record = 0;
	uint8_t dive_countdown = 0;

	switch (OBJECT(current_object)->state)
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
			draw_object(0u);
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
				draw_object(0u);
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
			draw_object(0u);
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
			draw_object(0u);
			if (OBJECT(current_object)->swooping_helicopter.screen_x >= -0xF0 && OBJECT(current_object)->swooping_helicopter.screen_x <= 0x130)
				return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
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

	switch (OBJECT(current_object)->state)
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
		draw_object(0u);
		return;
	case 2:
		seconds_left = OBJECT(CAR(player_car)->object)->player_car.timer;
		seconds_left = (uint16_t)(seconds_left + 0x1Du);
		seconds_left = seconds_left / 0x1Eu;
		OBJECT(current_object)->palette = start_gate_light_palettes[seconds_left];
		if (seconds_left == 0u)
			OBJECT(current_object)->state++;
		draw_object(seconds_left);
		return;
	case 3:
		draw_object(seconds_left);
		return;
	}
}

/* The man at the start line who waves the chequered flag as the race starts (stream 0 at 20). By state:
   0 reads the record (animation, go_animation, palette, side, x_offset, y_offset), draw_mode 9,
   scale_shift 1; 1 drawn standing (06E) while the car object's timer counts the frames to the start,
   then go_animation (06F, the waving); 1 and 2: ends once the car (car window_segment) reaches its
   position, else animation_advance and draw_object. */
void start_flagman_update(void)
{
	uint32_t animation = 0, flagman_record = 0, go_animation = 0;

	switch (OBJECT(current_object)->state)
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
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
		{
			draw_object(0u);
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
		draw_object(0u);
		return;
	case 2:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
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
	uint32_t animation = 0, flagman_record = 0, in_view_animation = 0;

	switch (OBJECT(current_object)->state)
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
		draw_object(0u);
		return;
	case 2:
		if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->window_segment)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
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
		OBJECT(current_object)->y_offset -= rise_speed >> 1;
		OBJECT(current_object)->x_offset += 8u;
		draw_object(rise_speed);
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
		OBJECT(current_object)->y_offset -= rise_speed >> 1;
		OBJECT(current_object)->x_offset += 8u;
		draw_object(rise_speed);
		return;
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
		animation = WEST_EAST_PANEL;
		OBJECT(current_object)->west_east_panel.base_palette = 0x67u;
		animation_start(animation);
		return;
	case 1:
		OBJECT(current_object)->y -= 2u;
		if (OBJECT(current_object)->y == 0xDCu)
			OBJECT(current_object)->state++;
		if (RACE(SCREEN(screen)->host_record)->time_left != 0u && CAR(player_car)->segment <= 0x250u)
		{
			palette = OBJECT(current_object)->west_east_panel.colour_count;
			palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
			OBJECT(current_object)->west_east_panel.colour_count = palette;
			OBJECT(current_object)->palette = ((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y += 2u;
		if (OBJECT(current_object)->y != 0x118u)
		{
			palette = OBJECT(current_object)->west_east_panel.colour_count;
			palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
			OBJECT(current_object)->west_east_panel.colour_count = palette;
			OBJECT(current_object)->palette = ((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 2:
		if (RACE(SCREEN(screen)->host_record)->time_left != 0u && CAR(player_car)->segment <= 0x250u)
		{
			palette = OBJECT(current_object)->west_east_panel.colour_count;
			palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
			OBJECT(current_object)->west_east_panel.colour_count = palette;
			OBJECT(current_object)->palette = ((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->y += 2u;
		if (OBJECT(current_object)->y != 0x118u)
		{
			palette = OBJECT(current_object)->west_east_panel.colour_count;
			palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
			OBJECT(current_object)->west_east_panel.colour_count = palette;
			OBJECT(current_object)->palette = ((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	case 3:
		OBJECT(current_object)->y += 2u;
		if (OBJECT(current_object)->y != 0x118u)
		{
			palette = OBJECT(current_object)->west_east_panel.colour_count;
			palette = (palette & ~0xFFu) | (uint8_t)(palette + 1u);
			OBJECT(current_object)->west_east_panel.colour_count = palette;
			OBJECT(current_object)->palette = ((palette >> 2) & 3u) + OBJECT(current_object)->west_east_panel.base_palette;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
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

	switch (OBJECT(current_object)->state)
	{
	case 0:
		santa_sleigh_record = OBJECT(current_object)->santa_sleigh.record;
		animation = SANTA_SLEIGH_RECORD(santa_sleigh_record)->animation;
		santa_sleigh_record += 2u;
		if (animation == ORCA)
			OBJECT(current_object)->y_offset = -0x180;
		if (animation == SANTA_SLEIGH)
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
		animation_advance(0u);
		if (OBJECT(current_object)->santa_sleigh.screen_x >= -0xB0)
		{
			draw_object(sound);
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
		draw_object(sound);
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
		draw_object(sound);
		return;
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
	uint32_t animation = 0, goal_parade_record = 0, ignored = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		goal_parade_record = OBJECT(current_object)->goal_parade.record;
		animation = GOAL_PARADE_RECORD(goal_parade_record)->animation;
		goal_parade_record += 2u;
		if (animation == ORCA)
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
		animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			draw_object(0u);
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
		draw_object(0u);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xB0)
		{
			draw_object(0u);
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
		draw_object(0u);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x < -0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags |= 0x40u;
		}
		draw_object(0u);
		return;
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
		road_row_draw(animation_order, 0u, &r12, &r14);
		return;
	case 3:
		animation_order = animation_advance(0u);
		road_row_draw(animation_order, 0u, &r12, &r14);
		return;
	}
}

/* What runs back and forth in front of a goal's crowd, setting off leftward: a kangaroo (stage 21), an
   orca (28), the bulls (29). As goal_parade_rightward_update, with the turns the other way: 2 moves by
   x_speed (negative) until its screen_x is below -B0, then 3: mirrored (flags bit 6), moves back until
   screen_x is above F0, then 2 again. It never ends. */
void goal_parade_leftward_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, goal_parade_record = 0, ignored = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		goal_parade_record = OBJECT(current_object)->goal_parade.record;
		animation = GOAL_PARADE_RECORD(goal_parade_record)->animation;
		goal_parade_record += 2u;
		if (animation == ORCA)
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
		animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			draw_object(0u);
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
		draw_object(0u);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(0u);
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			draw_object(0u);
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
		draw_object(0u);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		animation_advance(ignored);
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		draw_object(0u);
		return;
	}
}

/* The dragon dance in front of the crowd at the goal of stage 22 (with goal_parade_rightward's second
   dragon): as goal_parade_leftward_update, plus sound 44 on one frame in four at random (random_number).
   It never ends. */
void goal_parade_dragon_update(void)
{
	uint16_t draw_distance = 0, position = 0;
	uint32_t animation = 0, goal_parade_record = 0, random_bits = 0;

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
			if ((random_bits & 3u) == 0u)
			{
				random_bits = 0x44u;
				request_sound((uint8_t)random_bits);
			}
			animation_advance(0u);
			draw_object(random_bits);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			random_bits = random_number();
			if ((random_bits & 3u) == 0u)
			{
				random_bits = 0x44u;
				request_sound((uint8_t)random_bits);
			}
			animation_advance(0u);
			draw_object(random_bits);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		random_bits = random_number();
		if ((random_bits & 3u) == 0u)
		{
			random_bits = 0x44u;
			request_sound((uint8_t)random_bits);
		}
		animation_advance(0u);
		draw_object(random_bits);
		return;
	case 2:
		OBJECT(current_object)->x_offset += OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x >= -0xB0)
		{
			random_bits = random_number();
			if ((random_bits & 3u) == 0u)
			{
				random_bits = 0x44u;
				request_sound((uint8_t)random_bits);
			}
			animation_advance(0u);
			draw_object(random_bits);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->flags |= 0x40u;
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x <= 0xF0)
		{
			random_bits = random_number();
			if ((random_bits & 3u) == 0u)
			{
				random_bits = 0x44u;
				request_sound((uint8_t)random_bits);
			}
			animation_advance(0u);
			draw_object(random_bits);
			return;
		}
		OBJECT(current_object)->state--;
		OBJECT(current_object)->flags &= 0xBFu;
		random_bits = random_number();
		if ((random_bits & 3u) == 0u)
		{
			random_bits = 0x44u;
			request_sound((uint8_t)random_bits);
		}
		animation_advance(0u);
		draw_object(random_bits);
		return;
	case 3:
		OBJECT(current_object)->x_offset -= OBJECT(current_object)->goal_parade.x_speed;
		if (OBJECT(current_object)->goal_parade.screen_x > 0xF0)
		{
			OBJECT(current_object)->state--;
			OBJECT(current_object)->flags &= 0xBFu;
		}
		random_bits = random_number();
		if ((random_bits & 3u) == 0u)
		{
			random_bits = 0x44u;
			request_sound((uint8_t)random_bits);
		}
		animation_advance(0u);
		draw_object(random_bits);
		return;
	}
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
	uint16_t end_position = 0, position = 0, row_y = 0, segment_position = 0, start_position = 0, window_segment = 0;
	uint32_t branch_distance = 0, depth_layer = 0, depth_list = 0, first_sprite = 0, height_word = 0, last_scale = 0, last_x = 0, last_y = 0, layer_head = 0, offset_palette = 0, piece_scale = 0, piece_x = 0, piece_y = 0, row_span = 0, rows_done = 0, rows_left = 0, rows_to_fill = 0, segment = 0, sprite = 0, view_record = 0;
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
	} while (PROJECTED_SEGMENT(view_record)->scale == 0u);
	if (PROJECTED_SEGMENT(view_record)->scale == 0u)
		return;
	segment_position = (uint16_t)(((view_record & 0xFFFu) >> 4) + window_segment);
	view_record -= 0x10u;
	do
	{
		view_record += 0x10u;
		if (PROJECTED_SEGMENT(view_record)->scale == 0u)
			return;
		segment = view_record & 0xFFFu;
		segment = segment >> 4;
		segment = (uint16_t)(segment + window_segment);
		distance_from_start = segment - start_position;
		branch_distance = distance_from_start;
	} while ((int16_t)distance_from_start < 0 || (int16_t)PROJECTED_SEGMENT(view_record)[1].y >= 2);
	last_y = (int16_t)PROJECTED_SEGMENT(view_record)->y;
	rows_to_fill = 0u;
	if (start_position <= segment_position)
		rows_to_fill = 2u - last_y;
	last_x = (int16_t)PROJECTED_SEGMENT(view_record)->x;
	last_scale = PROJECTED_SEGMENT(view_record)->scale;
	if (last_scale >= 0x1FF8u)
		last_scale = 0x1FF8u;
	branch_distance = (uint16_t)(branch_distance << 3);
	last_x += (uint32_t)((int32_t)(((int32_t)((cosine_table[(uint16_t)(branch_distance + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * last_scale) >> 12) * OBJECT(current_object)->fork_branch.direction;
	fork_branch_sprite(&r12, &height_word, &r14, &offset_palette);
	for (;;)
	{
		view_record += 0x10u;
		if (PROJECTED_SEGMENT(view_record)->scale == 0u)
			break;
		piece_y = (int16_t)PROJECTED_SEGMENT(view_record)->y;
		if ((int16_t)last_y <= (int16_t)piece_y)
			continue;
		piece_x = (int16_t)PROJECTED_SEGMENT(view_record)->x;
		depth_layer = PROJECTED_SEGMENT(view_record)->depth_layer;
		piece_scale = PROJECTED_SEGMENT(view_record)->scale;
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
			AS32(SPRITE_WORDS(sprite)[6144]) = 0u;
			AS32(SPRITE_WORDS(sprite)[0]) = r12;
			sprite += 4u;
			SPRITE_WORDS(sprite)[0] = height_word;
			sprite += 2u;
			SPRITE_WORDS(sprite)[0] = ((last_scale * 0x370u) >> 12) | r14;
			sprite += 2u;
			SPRITE_WORDS(sprite)[0] = piece_y;
			sprite += 2u;
			SPRITE_WORDS(sprite)[0] = piece_x;
			sprite += 2u;
			AS32(SPRITE_WORDS(sprite)[0]) = offset_palette;
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
					AS32(SPRITE_WORDS(sprite)[6144]) = 0u;
					AS32(SPRITE_WORDS(sprite)[0]) = r12;
					sprite += 4u;
					SPRITE_WORDS(sprite)[0] = height_word;
					sprite += 2u;
					SPRITE_WORDS(sprite)[0] = ((divide_s32(piece_scale * rows_left + last_scale * rows_done, row_span) * 0x370u) >> 12) | r14;
					sprite += 2u;
					SPRITE_WORDS(sprite)[0] = row_y;
					row_y++;
					sprite += 2u;
					SPRITE_WORDS(sprite)[0] = divide_s32(piece_x * rows_left + last_x * rows_done, row_span);
					sprite += 2u;
					AS32(SPRITE_WORDS(sprite)[0]) = offset_palette;
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
		depth_list = screen + 0x5000u + depth_layer * 4u;
		if (DEPTH_LIST(depth_list)->head == 0u)
			DEPTH_LIST(depth_list)->tail = sprite;
		first_sprite = SCREEN(screen)->sprite_list_last + 0x10u;
		layer_head = DEPTH_LIST(depth_list)->head;
		AS32(SPRITE_WORDS(sprite)[0]) = layer_head;
		AS32(SPRITE_WORDS(sprite)[6144]) = (uint16_t)(layer_head + 0x300u);
		SCREEN(screen)->sprite_list_last = sprite;
		DEPTH_LIST(depth_list)->head = (first_sprite >> 4) | 0x8000u;
		SCREEN(screen)->sprite_groups++;
	}
}

/* Builds the sprite words of the branch's road piece for fork_branch_draw: the first frame of animation
   74 (its size and place in the sprite ROM, the size steps at 0x122B74) and the palette of the stage
   being built (fork_branch_stage_palettes by screen built_stage, into sprite_palettes). */
void fork_branch_sprite(uint32_t *p_size_and_attributes, uint32_t *p_height_word, uint32_t *p_width_bits, uint32_t *p_offset_palette)
{
	uint16_t height_word = 0, piece_attributes = 0, rom_offset = 0;
	uint32_t frames = 0, size_step = 0, width_bits = 0;

	frames = SPRITE_ANIMATION(fork_branch_piece_animation[0])->frames[0];
	piece_attributes = SPRITE_FRAME(frames)->piece_words[0];
	size_step = sprite_size_steps_ADDRESS + SPRITE_FRAME(frames)->piece_words[1] * 4u;
	*p_size_and_attributes = ((uint32_t)SPRITE_SIZE_STEP(size_step)->source_size << 16) | piece_attributes;
	size_step += 2u;
	rom_offset = SPRITE_SIZE_STEP(size_step - 0x2u)->rom_offset;
	size_step += 4u;
	height_word = 1u | SPRITE_SIZE_STEP(size_step - 0x6u)->height_bits;
	width_bits = SPRITE_SIZE_STEP(size_step - 0x6u)->width_bits;
	width_bits = width_bits | SCREEN(screen)->sprite_screen_bit;
	*p_offset_palette = ((uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[fork_branch_stage_palettes[SCREEN(screen)->built_stage]])[0] << 16) | rom_offset;
	*p_height_word = (*p_height_word & 0xFFFF0000u) | height_word;
	*p_width_bits = width_bits;
}
