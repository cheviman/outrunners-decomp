/**
 *    Name:        scenes/ranking.c
 *    Description: the ranking: the course records, OutRunner of OutRunners, its titles, banners and record lines, and the traffic passing behind them
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Loads the ten courses' records into course_records (6 bytes each: the car's model, the name's three
   letters, the time in hundredths) from the backup RAM (701F32); a course without one there (its first
   byte negative) gets its default from default_course_records (SGA, 5'00"00); the model is kept to its
   low three bits. */
void load_course_records(void)
{
	uint32_t backup_record = 0, course_record = 0, courses_left = 0;
	uint32_t model_place = 0;

	courses_left = 0xAu;
	backup_record = BOOKKEEPING_COURSE_RECORDS_ADDRESS;
	course_record = default_course_records_ADDRESS;
	r12 = course_records_ADDRESS;
	for (;;)
	{
		if ((int8_t)BACKUP_COURSE_RECORD(backup_record)->model >= 0)
		{
			COURSE_RECORD_COPY(r12)->model = BACKUP_COURSE_RECORD(backup_record)->model;
			model_place = r12;
			r12 = r12 + 1u;
			COURSE_RECORD_COPY(model_place)->model &= 7u;
			backup_record++;
			COURSE_RECORD_COPY(r12 - 0x1u)->name[0] = BACKUP_COURSE_RECORD(backup_record - 0x1u)->name[0];
			r12 = r12 + 1u;
			backup_record++;
			COURSE_RECORD_COPY(r12 - 0x2u)->name[1] = BACKUP_COURSE_RECORD(backup_record - 0x2u)->name[1];
			r12 = r12 + 1u;
			backup_record++;
			COURSE_RECORD_COPY(r12 - 0x3u)->name[2] = BACKUP_COURSE_RECORD(backup_record - 0x3u)->name[2];
			r12 = r12 + 1u;
			backup_record++;
			COURSE_RECORD_COPY(r12 - 0x4u)->time = BACKUP_COURSE_RECORD(backup_record - 0x4u)->time;
			r12 = r12 + 2u;
			course_record += 6u;
			courses_left--;
			backup_record += 2u;
			if (courses_left == 0u)
				return;
			continue;
		}
		COURSE_RECORD_COPY(r12)->model_and_name = COURSE_RECORD(course_record)->model_and_name;
		r12 = r12 + 4u;
		course_record += 4u;
		COURSE_RECORD_COPY(r12 - 0x4u)->time = COURSE_RECORD(course_record - 0x4u)->time;
		r12 = r12 + 2u;
		backup_record += 6u;
		courses_left--;
		course_record += 2u;
		if (courses_left != 0u)
			continue;
		break;
	}
}

/* The ranking in the attract mode (game mode 5, game_mode_ranking): OutRunner of OutRunners, the record
   of three courses in turn. By state: 0 starts ranking_objects (01AA01, 01A6CF, which game_mode_ranking
   has started too), ranking_background and the two halves of the title (ranking_title_left,
   ranking_title_right); 1 clears course_over; once the screen's fade is over (screen flags bit 0): after
   three courses ends, else courses_shown + 1, 60 frames (frames_left), and starts ranking_course_banner
   and ranking_record for the course screen record_course; 2 when the 60 frames are over sets course_over
   for a frame (the background fades out) and moves screen record_course to the next of the ten courses,
   back to 1. */
void ranking_update(void)
{
	uint8_t next_course = 0, record_course = 0;
	uint32_t handler_list = 0, object = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->ranking.courses_shown = 0u;
		handler_list = ranking_objects_ADDRESS;
		start_object_list(handler_list);
		object = start_object();
		OBJECT(object)->ranking_background.ranking = current_object;
		OBJECT(object)->handler = ranking_background_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = ranking_title_left_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = ranking_title_right_update_ADDRESS;
		OBJECT(current_object)->state++;
		return;
	case 1:
		OBJECT(current_object)->ranking.course_over &= 0xDFFFu;
		if ((SCREEN(screen)->flags & 1u) != 0u)
			return;
		if (OBJECT(current_object)->ranking.courses_shown >= 3u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->ranking.courses_shown++;
		OBJECT(current_object)->ranking.frames_left = 0x3Cu;
		record_course = SCREEN(screen)->record_course;
		object = start_object();
		OBJECT(object)->handler = ranking_course_banner_update_ADDRESS;
		OBJECT(object)->ranking_course_banner.ranking = current_object;
		OBJECT(object)->ranking_course_banner.course = record_course;
		object = start_object();
		OBJECT(object)->handler = ranking_record_start_ADDRESS;
		OBJECT(object)->ranking_record.course = record_course;
		OBJECT(current_object)->state++;
		return;
	case 2:
		frames_left = OBJECT(current_object)->ranking.frames_left;
		OBJECT(current_object)->ranking.frames_left = frames_left - 1u;
		if (frames_left != 1u)
			return;
		OBJECT(current_object)->ranking.course_over |= 0x2000u;
		next_course = (uint8_t)(SCREEN(screen)->record_course + 1u);
		if (next_course >= 0xAu)
			next_course = 0u;
		SCREEN(screen)->record_course = next_course;
		OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(ranking_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The picture of the course behind the ranking, while the game mode is the ranking (screen game_mode 5).
   By state: 0 courses_shown from the ranking; for the first course I/O C0001C = 6 (C8001C = 86 for
   screen 2) and a fade in (screen fade_kind 1, flags bit 0); ranking_background_load; 1 waits while the
   pages are unpacked (screen unpack_state), then for the later courses a fade in (fade_kind 3); 2 when
   the ranking says course_over: after the third course a fade out (fade_kind 0) and state 4, else a fade
   out (fade_kind 2); 3 when the fade is over, back to 0 for the next course's picture; 4 nothing more.
   Every frame, in any game mode, the picture scrolls by 3 pixels (scroll_x - 3). */
void ranking_background_update(void)
{
	uint32_t scroll_x_address = 0;

	if (SCREEN(screen)->game_mode == 5u)
	{
		switch (OBJECT(current_object)->state)
		{
		case 0:
			OBJECT(current_object)->ranking_background.courses_shown = OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.courses_shown;
			if (OBJECT(current_object)->ranking_background.courses_shown == 0u)
			{
				if (SCREEN(screen)->index != 0u)
					io_write8(IO_2_CNT, 0x86u);
				else
					io_write8(IO_1_CNT, 6u);
				SCREEN(screen)->fade_kind = 1u;
				SCREEN(screen)->flags |= 1u;
			}
			ranking_background_load();
			OBJECT(current_object)->state++;
		case 1:
			if (SCREEN(screen)->unpack_state != 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 0u)
			{
				SCREEN(screen)->fade_kind = 3u;
				SCREEN(screen)->flags |= 1u;
			}
			OBJECT(current_object)->state++;
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 3u)
			{
				SCREEN(screen)->fade_kind = 2u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state++;
				if ((SCREEN(screen)->flags & 1u) == 0u)
					OBJECT(current_object)->state = 0u;
			}
			else
			{
				SCREEN(screen)->fade_kind = 0u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state = 4u;
			}
			break;
		case 2:
			if ((OBJECT(OBJECT(current_object)->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
			{
				scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
				VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
				return;
			}
			if (OBJECT(current_object)->ranking_background.courses_shown != 3u)
			{
				SCREEN(screen)->fade_kind = 2u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state++;
				if ((SCREEN(screen)->flags & 1u) == 0u)
					OBJECT(current_object)->state = 0u;
			}
			else
			{
				SCREEN(screen)->fade_kind = 0u;
				SCREEN(screen)->flags |= 1u;
				OBJECT(current_object)->state = 4u;
			}
			scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
			VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
			return;
		case 3:
			if ((SCREEN(screen)->flags & 1u) == 0u)
				OBJECT(current_object)->state = 0u;
			scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
			VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
			return;
		case 4:
			break;
		default:
			invalid_jump(ranking_background_state_jumps[OBJECT(current_object)->state]);
		}
	}
	scroll_x_address = OBJECT(current_object)->ranking_background.scroll_x_address;
	VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
}

/* The left half of the ranking's title, OutRunner of OutRunners (animation 2D2, palette 2B2) at x 0, y
   30: state 0 sets it up; drawn every frame. */
void ranking_title_left_update(void)
{
	uint32_t animation = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = ranking_title_left_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B2u;
		animation = 0x2D2u;
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(ranking_title_left_state_jumps[state]);
	}
	routine = draw_object(routine);
}

/* The right half of the ranking's title, OutRunner of OutRunners (animation 2D3, palette 2B3) at x 13F,
   y 30: state 0 sets it up; drawn every frame. */
void ranking_title_right_update(void)
{
	uint32_t animation = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = ranking_title_right_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x2B3u;
		animation = 0x2D3u;
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = 0x13Fu;
		OBJECT(current_object)->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(ranking_title_right_state_jumps[state]);
	}
	routine = draw_object(routine);
}

/* The scroll with the course's name under the ranking's title (Australia Course...: animation
   ranking_course_banner_animations by course, palette ranking_course_banner_palettes) at x A0, y 50, for
   90 frames (frames_left). */
void ranking_course_banner_update(void)
{
	uint32_t animation = 0, course_number = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	course_number = ranking_course_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		course_number = OBJECT(current_object)->ranking_course_banner.course;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = ranking_course_banner_palettes[course_number];
		animation = ranking_course_banner_animations[course_number];
		animation_start(animation);
		OBJECT(current_object)->depth_layer = 0x19Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x50u;
		OBJECT(current_object)->ranking_course_banner.frames_left = 0x5Au;
		break;
	case 1:
		break;
	default:
		invalid_jump(ranking_course_banner_state_jumps[state]);
	}
	frames_left = OBJECT(current_object)->ranking_course_banner.frames_left;
	OBJECT(current_object)->ranking_course_banner.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		course_number = draw_object(course_number);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* Sets up the course's record line from the records at 20E300 (by course: the initials, '-' shown as
   '.', and a 0 into text, then the time as 5'00"00, time_to_text), starts a ranking_record_character for
   each of its 11 characters (index, course, itself) and ends at once. */
void ranking_record_start(void)
{
	uint8_t character_index = 0, initial = 0;
	uint32_t characters_left = 0, initial_place = 0, object = 0, record_offset = 0, record_time = 0, text_place = 0;
	uint8_t second_initial = 0;

	text_place = current_object + 0x80u;
	record_offset = OBJECT(current_object)->ranking_record.course;
	record_offset = (uint8_t)(record_offset * 6u);
	initial_place = 0x20E301u + record_offset;
	initial = COURSE_RECORD_BYTES(initial_place)[0];
	initial_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x80u)->ranking_record.text = initial;
	second_initial = COURSE_RECORD_BYTES(initial_place)[0];
	initial_place++;
	initial = second_initial;
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x81u)->list_data = (OBJECT(text_place - 0x81u)->list_data & 0xFFFF00FFu) | ((initial & 0xFFu) << 8u);
	initial = COURSE_RECORD_BYTES(initial_place)[0];
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x82u)->list_data = (OBJECT(text_place - 0x82u)->list_data & 0xFF00FFFFu) | ((initial & 0xFFu) << 16u);
	text_place++;
	OBJECT(text_place - 0x83u)->list_data &= 0xFFFFFFu;
	record_time = course_record_times[record_offset >> 1];
	text_place++;
	time_to_text(record_time, text_place);
	characters_left = 0xBu;
	character_index = 0u;
	do
	{
		object = start_object();
		OBJECT(object)->ranking_record_character.ranking_record = current_object;
		OBJECT(object)->ranking_record_character.index = character_index;
		OBJECT(object)->ranking_record_character.course = OBJECT(current_object)->ranking_record.course;
		OBJECT(object)->handler = ranking_record_character_update_ADDRESS;
		character_index++;
		characters_left--;
	} while (characters_left != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* A character of a course's record line (font 2DE, palette 27): 0 its character from the record's text
   by index, at y 80 and x -A0 plus 16 pixels for each place of index; 1 slides in, 32 pixels a frame for
   7 frames (slide_steps), then starts a ranking_record_car (each of the 11 characters starts one, all at
   the same place); 2 index + 2 frames later (colour_delay) takes the colours of the record's car
   (ranking_record_character_palettes by its model); 3 58 frames after its start (frames_left) slides out
   to the left, 16 pixels a frame, and ends at x -20. */
void ranking_record_character_update(void)
{
	uint32_t animation = 0, character_index = 0, course = 0, object = 0, ranking_record = 0;
	uint8_t colour_delay = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	character_index = ranking_record_character_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_character.frames_left = 0x3Au;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->palette = 0x27u;
		animation = 0x2DEu;
		animation_start(animation);
		ranking_record = OBJECT(current_object)->ranking_record_character.ranking_record;
		character_index = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->depth_layer = OBJECT(current_object)->ranking_record_character.index;
		OBJECT(current_object)->frame = mem_read8(ranking_record + 0x80u + character_index);
		OBJECT(current_object)->x = ranking_record_character_xs[character_index];
		OBJECT(current_object)->y = ranking_record_character_ys[character_index];
		OBJECT(current_object)->ranking_record_character.slide_steps = 0u;
	case 1:
		OBJECT(current_object)->x += 0x20u;
		OBJECT(current_object)->ranking_record_character.slide_steps++;
		if (OBJECT(current_object)->ranking_record_character.slide_steps >= 7u)
		{
			object = start_object();
			OBJECT(object)->handler = ranking_record_car_update_ADDRESS;
			OBJECT(object)->ranking_record_car.course = OBJECT(current_object)->ranking_record_character.course;
			OBJECT(current_object)->state++;
		}
		character_index = (character_index & ~0xFFu) | OBJECT(current_object)->ranking_record_character.index;
		character_index = (character_index & ~0xFFu) | (uint8_t)(character_index + 2u);
		OBJECT(current_object)->ranking_record_character.colour_delay = character_index;
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			character_index = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = ranking_record_character_palettes[character_index];
			OBJECT(current_object)->state++;
		}
		break;
	case 2:
		colour_delay = OBJECT(current_object)->ranking_record_character.colour_delay;
		OBJECT(current_object)->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			course = OBJECT(current_object)->ranking_record_character.course;
			character_index = course_records[(uint8_t)(course * 6u)];
			OBJECT(current_object)->palette = ranking_record_character_palettes[character_index];
			OBJECT(current_object)->state++;
		}
		frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
		OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
		if (frames_left != 1u)
		{
			character_index = draw_object(character_index);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0xFFE0u)
		{
			character_index = draw_object(character_index);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0xFFE0u)
		{
			character_index = draw_object(character_index);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(ranking_record_character_state_jumps[state]);
	}
	frames_left = OBJECT(current_object)->ranking_record_character.frames_left;
	OBJECT(current_object)->ranking_record_character.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		character_index = draw_object(character_index);
		return;
	}
	OBJECT(current_object)->state++;
	OBJECT(current_object)->x -= 0x10u;
	if (OBJECT(current_object)->x != 0xFFE0u)
	{
		character_index = draw_object(character_index);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* The record holder's car under the course's record line (animation car_model_turn_animations by the
   model of the course's record, palette car_model_palettes; size 50, y 88): drives in from the left 16
   pixels a frame to x 108 or more, turns round (frame 9 down to 4), waits 15 frames (wait_frames),
   drives back left 16 pixels a frame and ends at x 0. State 5 (drawn only) is not set by its own code. */
void ranking_record_car_update(void)
{
	uint32_t animation = 0, car_model = 0, course = 0;
	uint16_t wait_frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	car_model = ranking_record_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->ranking_record_car.wait_frames = 0xFu;
		OBJECT(current_object)->draw_mode = 0xAu;
		course = OBJECT(current_object)->ranking_record_car.course;
		car_model = course_records[(uint8_t)(course * 6u)];
		animation = car_model_turn_animations[car_model];
		OBJECT(current_object)->palette = car_model_palettes[car_model];
		animation_start(animation);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0x88u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 1:
		OBJECT(current_object)->x += 0x10u;
		if (OBJECT(current_object)->x < 0x108u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 2:
		OBJECT(current_object)->frame--;
		if (OBJECT(current_object)->frame != 4u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 3:
		wait_frames = OBJECT(current_object)->ranking_record_car.wait_frames;
		OBJECT(current_object)->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->state++;
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 4:
		OBJECT(current_object)->x -= 0x10u;
		if (OBJECT(current_object)->x != 0u)
		{
			car_model = draw_object(car_model);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	case 5:
		car_model = draw_object(car_model);
		return;
	default:
		invalid_jump(ranking_record_car_state_jumps[state]);
	}
}

/* Counts the steps of the passing cars' path (traffic_path: size, x, y; closed by 0) into path_steps,
   then runs passing_traffic_update. */
void passing_traffic_start(void)
{
	uint8_t step_count = 0;
	uint32_t traffic_path_step = 0;

	traffic_path_step = traffic_path_ADDRESS;
	step_count = 0u;
	while (TRAFFIC_PATH_STEP(traffic_path_step)->size != 0u)
	{
		step_count++;
		traffic_path_step += 6u;
	}
	OBJECT(current_object)->passing_traffic.path_steps = step_count;
	OBJECT(current_object)->handler = passing_traffic_update_ADDRESS;
	passing_traffic_update();
}

/* Now and then a car passes on the road behind the certificate: by state, 0 one frame in 16 at random
   (random_number) starts a passing_car on the path's last step (path_steps), 1 then waits 10 frames
   (wait). */
void passing_traffic_update(void)
{
	uint32_t object = 0, random = 0;
	uint16_t wait = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		random = random_number();
		if ((random & 0xFu) == 0u)
		{
			object = start_object();
			OBJECT(object)->handler = passing_car_update_ADDRESS;
			OBJECT(object)->passing_car.step = OBJECT(current_object)->passing_traffic.path_steps;
			OBJECT(current_object)->passing_traffic.wait = 0xAu;
			OBJECT(current_object)->state++;
		}
		return;
	case 1:
		wait = OBJECT(current_object)->passing_traffic.wait;
		OBJECT(current_object)->passing_traffic.wait = wait - 1u;
		if (wait == 1u)
			OBJECT(current_object)->state--;
		return;
	default:
		invalid_jump(passing_traffic_state_jumps[OBJECT(current_object)->state]);
	}
}

/* A car on the road behind the certificate (animation car_model_turn_animations by a random model,
   palette car_model_palettes; its sound, car_pass_sounds by the model, requested as it starts): state 0
   picks frame C and state 1, or frame 1 and state 2 when the random number is negative; then a frame a
   step it goes from the path's last step (traffic_path: size 370 near the bottom edge) to its first
   (size 5 on the horizon), at the path's x in state 1 or at 96 - x on the other side of the road in
   state 2; ends past the first step. */
void passing_car_update(void)
{
	uint8_t pass_sound = 0;
	uint16_t size = 0;
	uint32_t animation = 0, car_model = 0, mirrored_x = 0, path_size = 0, random_side = 0, step = 0, step_index = 0, traffic_path_step = 0;
	uint16_t near_step = 0, far_step = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		car_model = random_number();
		random_side = car_model;
		car_model &= 7u;
		animation = car_model_turn_animations[car_model];
		OBJECT(current_object)->palette = car_model_palettes[car_model];
		animation_start(animation);
		pass_sound = car_pass_sounds[car_model];
		request_sound(pass_sound);
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 3u;
		OBJECT(current_object)->frame = 0xCu;
		OBJECT(current_object)->state++;
		if ((int32_t)random_side >= 0)
		{
			step = (uint16_t)OBJECT(current_object)->passing_car.step;
			near_step = step - 1u;
			step_index = near_step;
			if ((int16_t)near_step < 0)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			OBJECT(current_object)->passing_car.step = step_index;
			traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
			path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
			OBJECT(current_object)->width_scale = path_size;
			OBJECT(current_object)->height_scale = path_size;
			traffic_path_step += 2u;
			OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
			OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
			path_size = draw_object(path_size);
			return;
		}
		OBJECT(current_object)->frame = 1u;
		OBJECT(current_object)->state++;
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		traffic_path_step += 2u;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	case 1:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		near_step = step - 1u;
		step_index = near_step;
		if ((int16_t)near_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		path_size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = path_size;
		OBJECT(current_object)->height_scale = path_size;
		traffic_path_step += 2u;
		OBJECT(current_object)->x = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		path_size = draw_object(path_size);
		return;
	case 2:
		step = (uint16_t)OBJECT(current_object)->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->passing_car.step = step_index;
		traffic_path_step = traffic_path_ADDRESS + step_index * 3u * 2u;
		size = TRAFFIC_PATH_STEP(traffic_path_step)->size;
		OBJECT(current_object)->width_scale = size;
		OBJECT(current_object)->height_scale = size;
		traffic_path_step += 2u;
		mirrored_x = (uint16_t)(0x96u - TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->x);
		OBJECT(current_object)->x = mirrored_x;
		OBJECT(current_object)->y = TRAFFIC_PATH_STEP(traffic_path_step - 0x2u)->y;
		mirrored_x = draw_object(mirrored_x);
		return;
	default:
		invalid_jump(passing_car_state_jumps[OBJECT(current_object)->state]);
	}
}

/* Loads the picture of the course screen record_course (course_backgrounds picks it) into the screen's
   first tilemap: its register addresses for the screen (ranking_background_register_sets, into 80-AC),
   the tile bank (ranking_background_tile_banks), the palette (background_palettes, queue_palette_list),
   the four tilemap pages (background_tile_page_lists, with the screen's page numbers from
   ranking_background_page_numbers, in page_list; queue_tile_pages), start_palette_fades with
   background_palette_fades, zoom 1:1, zoom centre and the second tilemap's scroll 0, the pages, the
   scroll's start (ranking_background_scroll_xs: x 360 for screen 1, 4A0 for screen 2; y 60), its
   priority 1 and the second tilemap off in the mixer (ranking_mixer_addresses: screen 182 and 186; flags
   bit 2); send_video_registers = FF to send the registers. */
void ranking_background_load(void)
{
	uint32_t background_number = 0, background_pages = 0, colour_list = 0, index = 0, mixer_place = 0, mixer_priority = 0, page_numbers = 0, page_place = 0, pages_to_copy = 0, palette_fades = 0, queued_pages = 0, ranking_background_registers = 0;
	uint32_t saved_fades = 0;

	ranking_background_registers = ranking_background_register_sets_ADDRESS + SCREEN(screen)->index * 0xCu * 4u;
	OBJECT(current_object)->ranking_background.tile_bank_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers)->tile_bank_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.pages_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x4u)->pages_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x8u)->scroll_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0xCu)->scroll_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x10u)->zoom_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x14u)->zoom_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x18u)->zoom_center_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.zoom_center_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x1Cu)->zoom_center_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.cleared_scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x20u)->cleared_scroll_x_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.cleared_scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x24u)->cleared_scroll_y_address;
	ranking_background_registers += 4u;
	OBJECT(current_object)->ranking_background.second_scroll_x_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x28u)->second_scroll_x_address;
	OBJECT(current_object)->ranking_background.second_scroll_y_address = RANKING_BACKGROUND_REGISTERS(ranking_background_registers - 0x28u)->second_scroll_y_address;
	send_video_registers = 0xFFu;
	background_number = course_backgrounds[SCREEN(screen)->record_course];
	TILE_BANK_VALUES(OBJECT(current_object)->ranking_background.tile_bank_address)[0] = ranking_background_tile_banks[background_number];
	saved_fades = background_palette_fades[background_number];
	colour_list = background_palettes[background_number];
	background_pages = background_tile_page_lists[background_number] + 0x10u;
	page_place = current_object + 0xB0u;
	page_numbers = ranking_background_page_numbers[SCREEN(screen)->index];
	pages_to_copy = 4u;
	do
	{
		OBJECT_TILE_PAGES(page_place)[0] = TILE_PAGE_ENTRIES(background_pages)[0] | (page_numbers & ~0xFFFFFFu);
		page_place += 4u;
		page_numbers = page_numbers << 8;
		pages_to_copy--;
		background_pages += 4u;
	} while (pages_to_copy != 0u);
	OBJECT_TILE_PAGES(page_place)[0] = 0xFFFFFFFFu;
	queued_pages = current_object + 0xB0u;
	queue_tile_pages(queued_pages);
	queue_palette_list(colour_list);
	palette_fades = saved_fades;
	start_palette_fades(palette_fades);
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.zoom_x_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.zoom_y_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.zoom_center_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.zoom_center_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.cleared_scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.cleared_scroll_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.second_scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.second_scroll_y_address)[0] = 0u;
	index = SCREEN(screen)->index;
	PAGE_SELECT(OBJECT(current_object)->ranking_background.pages_address)[0] = byte_swap32(ranking_background_page_numbers[index]);
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.scroll_x_address)[0] = ranking_background_scroll_xs[index];
	VIDEO_REGISTER_WORDS(OBJECT(current_object)->ranking_background.scroll_y_address)[0] = 0x60u;
	mixer_place = ranking_mixer_addresses[index];
	mixer_priority = MIXER_VALUE_PAIRS(mixer_place)[0];
	mixer_priority = (mixer_priority & ~0xFFFFu) | (mixer_priority & 0xFFF0u);
	MIXER_VALUE_PAIRS(mixer_place)[0] = (mixer_priority & ~0xFFu) | (uint8_t)(mixer_priority | 1u);
	mixer_place += 4u;
	mixer_priority = MIXER_VALUE_PAIRS(mixer_place)[0];
	MIXER_VALUE_PAIRS(mixer_place)[0] = (mixer_priority & ~0xFFFFu) | (mixer_priority & 0xFFF0u);
	SCREEN(screen)->flags |= 4u;
}
