/**
 *    Name:        scenes/ranking.c
 *    Description: the ranking: the course records, OutRunner of OutRunners, its titles, banners and record lines, and the traffic passing behind them
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
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
uint32_t load_course_records(void)
{
	uint32_t backup_record = 0, courses_left = 0;
	const struct course_record *course_record = NULL;
	struct course_record_copy *record_copy = NULL;
	uint32_t model_place = 0;

	courses_left = 0xAu;
	backup_record = BOOKKEEPING_COURSE_RECORDS_ADDRESS;
	course_record = rom_pointer(default_course_records_ADDRESS);
	record_copy = pointer_to(course_records_ADDRESS);
	for (;;)
	{
		if ((int8_t)BACKUP_COURSE_RECORD(backup_record)->model >= 0)
		{
			record_copy->model = BACKUP_COURSE_RECORD(backup_record)->model;
			model_place = address_of(record_copy);
			COURSE_RECORD_COPY(model_place)->model &= 7u;
			backup_record++;
			record_copy->name[0] = BACKUP_COURSE_RECORD(backup_record - 0x1u)->name[0];
			backup_record++;
			record_copy->name[1] = BACKUP_COURSE_RECORD(backup_record - 0x2u)->name[1];
			backup_record++;
			record_copy->name[2] = BACKUP_COURSE_RECORD(backup_record - 0x3u)->name[2];
			backup_record++;
			record_copy->time = BACKUP_COURSE_RECORD(backup_record - 0x4u)->time;
			record_copy++;
			course_record++;
			courses_left--;
			backup_record += 2u;
			if (courses_left == 0u)
				return address_of(record_copy);
			continue;
		}
		record_copy->model_and_name = course_record->model_and_name;
		record_copy->time = course_record->time;
		record_copy++;
		backup_record += 6u;
		courses_left--;
		course_record++;
		if (courses_left != 0u)
			continue;
		break;
	}
	return address_of(record_copy);
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
	uint32_t handler_list = 0;
	struct object *object = NULL;
	uint16_t frames_left = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->ranking.courses_shown = 0u;
		handler_list = ranking_objects_ADDRESS;
		start_object_list(handler_list);
		object = start_object();
		object->ranking_background.ranking = address_of(current_object);
		object->handler = ranking_background_update_ADDRESS;
		object = start_object();
		object->handler = ranking_title_left_update_ADDRESS;
		object = start_object();
		object->handler = ranking_title_right_update_ADDRESS;
		current_object->state++;
		return;
	case 1:
		current_object->ranking.course_over &= 0xDFFFu;
		if ((screen->flags & 1u) != 0u)
			return;
		if (current_object->ranking.courses_shown >= 3u)
		{
			current_object->flags = 0u;
			return;
		}
		current_object->ranking.courses_shown++;
		current_object->ranking.frames_left = 0x3Cu;
		record_course = screen->record_course;
		object = start_object();
		object->handler = ranking_course_banner_update_ADDRESS;
		object->ranking_course_banner.ranking = address_of(current_object);
		object->ranking_course_banner.course = record_course;
		object = start_object();
		object->handler = ranking_record_start_ADDRESS;
		object->ranking_record.course = record_course;
		current_object->state++;
		return;
	case 2:
		frames_left = current_object->ranking.frames_left;
		current_object->ranking.frames_left = frames_left - 1u;
		if (frames_left != 1u)
			return;
		current_object->ranking.course_over |= 0x2000u;
		next_course = (uint8_t)(screen->record_course + 1u);
		if (next_course >= 0xAu)
			next_course = 0u;
		screen->record_course = next_course;
		current_object->state--;
		return;
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

	if (screen->game_mode == 5u)
	{
		for (;;)
		{
			switch (current_object->state)
			{
			case 0:
				current_object->ranking_background.courses_shown = OBJECT(current_object->ranking_background.ranking)->ranking.courses_shown;
				if (current_object->ranking_background.courses_shown == 0u)
				{
					if (screen->index != 0u)
						io_write8(IO_2_CNT, 0x86u);
					else
						io_write8(IO_1_CNT, 6u);
					screen->fade_kind = 1u;
					screen->flags |= 1u;
				}
				ranking_background_load();
				current_object->state++;
			case 1:
				if (screen->unpack_state != 0u)
					break;
				if (current_object->ranking_background.courses_shown != 0u)
				{
					screen->fade_kind = 3u;
					screen->flags |= 1u;
				}
				current_object->state++;
				if ((OBJECT(current_object->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
					break;
				if (current_object->ranking_background.courses_shown != 3u)
				{
					screen->fade_kind = 2u;
					screen->flags |= 1u;
					current_object->state++;
					if ((screen->flags & 1u) != 0u)
						break;
					current_object->state = 0u;
					break;
				}
				screen->fade_kind = 0u;
				screen->flags |= 1u;
				current_object->state = 4u;
				continue;
			case 2:
				if ((OBJECT(current_object->ranking_background.ranking)->ranking.course_over & 0x2000u) == 0u)
					break;
				if (current_object->ranking_background.courses_shown != 3u)
				{
					screen->fade_kind = 2u;
					screen->flags |= 1u;
					current_object->state++;
					if ((screen->flags & 1u) != 0u)
						break;
					current_object->state = 0u;
					break;
				}
				screen->fade_kind = 0u;
				screen->flags |= 1u;
				current_object->state = 4u;
				continue;
			case 3:
				if ((screen->flags & 1u) == 0u)
					current_object->state = 0u;
				scroll_x_address = current_object->ranking_background.scroll_x_address;
				VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
				return;
			case 4:
				break;
			}
			break;
		}
	}
	scroll_x_address = current_object->ranking_background.scroll_x_address;
	VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 3u;
}

/* The left half of the ranking's title, OutRunner of OutRunners (animation 2D2, palette 2B2) at x 0, y
   30: state 0 sets it up; drawn every frame. */
void ranking_title_left_update(void)
{
	uint32_t animation = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->draw_mode = 0xAu;
		current_object->palette = 0x2B2u;
		animation = RANKING_TITLE_LEFT;
		animation_start(animation);
		current_object->depth_layer = 0x19Eu;
		current_object->x = 0u;
		current_object->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		draw_object(0u);
		return;
	}
	draw_object(0u);
}

/* The right half of the ranking's title, OutRunner of OutRunners (animation 2D3, palette 2B3) at x 13F,
   y 30: state 0 sets it up; drawn every frame. */
void ranking_title_right_update(void)
{
	uint32_t animation = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->draw_mode = 0xAu;
		current_object->palette = 0x2B3u;
		animation = RANKING_TITLE_RIGHT;
		animation_start(animation);
		current_object->depth_layer = 0x19Du;
		current_object->x = 0x13Fu;
		current_object->y = 0x30u;
		break;
	case 1:
		break;
	case 2:
		draw_object(0u);
		return;
	}
	draw_object(0u);
}

/* The scroll with the course's name under the ranking's title (Australia Course...: animation
   ranking_course_banner_animations by course, palette ranking_course_banner_palettes) at x A0, y 50, for
   90 frames (frames_left). */
void ranking_course_banner_update(void)
{
	uint32_t animation = 0, course_number = 0;
	uint16_t frames_left = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		course_number = current_object->ranking_course_banner.course;
		current_object->draw_mode = 0xAu;
		current_object->palette = ranking_course_banner_palettes[course_number];
		animation = ranking_course_banner_animations[course_number];
		animation_start(animation);
		current_object->depth_layer = 0x19Cu;
		current_object->x = 0xA0u;
		current_object->y = 0x50u;
		current_object->ranking_course_banner.frames_left = 0x5Au;
		break;
	case 1:
		break;
	}
	frames_left = current_object->ranking_course_banner.frames_left;
	current_object->ranking_course_banner.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		draw_object(course_number);
		return;
	}
	current_object->flags = 0u;
}

/* Sets up the course's record line from the records at 20E300 (by course: the initials, '-' shown as
   '.', and a 0 into text, then the time as 5'00"00, time_to_text), starts a ranking_record_character for
   each of its 11 characters (index, course, itself) and ends at once. */
void ranking_record_start(void)
{
	uint8_t character_index = 0, initial = 0;
	uint32_t characters_left = 0, initial_place = 0, record_offset = 0, record_time = 0, text_place = 0;
	struct object *object = NULL;
	uint8_t second_initial = 0;

	text_place = address_of(current_object) + 0x80u;
	record_offset = current_object->ranking_record.course;
	record_offset = (uint8_t)(record_offset * 6u);
	initial_place = course_records_ADDRESS + 0x1u + record_offset;
	initial = COURSE_RECORD_BYTES(initial_place)[0];
	initial_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x80u)->ranking_record.text[0] = initial;
	second_initial = COURSE_RECORD_BYTES(initial_place)[0];
	initial_place++;
	initial = second_initial;
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x81u)->ranking_record.text[1] = initial;
	initial = COURSE_RECORD_BYTES(initial_place)[0];
	text_place++;
	if (initial == 0x2Du)
		initial = 0x2Eu;
	OBJECT(text_place - 0x82u)->ranking_record.text[2] = initial;
	text_place++;
	OBJECT(text_place - 0x83u)->ranking_record.text[3] = 0u;
	record_time = game->course_record_times[record_offset >> 1];
	text_place++;
	time_to_text(record_time, text_place);
	characters_left = 0xBu;
	character_index = 0u;
	do
	{
		object = start_object();
		object->ranking_record_character.ranking_record = address_of(current_object);
		object->ranking_record_character.index = character_index;
		object->ranking_record_character.course = current_object->ranking_record.course;
		object->handler = ranking_record_character_update_ADDRESS;
		character_index++;
		characters_left--;
	} while (characters_left != 0u);
	current_object->flags = 0u;
}

/* A character of a course's record line (font 2DE, palette 27): 0 its character from the record's text
   by index, at y 80 and x -A0 plus 16 pixels for each place of index; 1 slides in, 32 pixels a frame for
   7 frames (slide_steps), then starts a ranking_record_car (each of the 11 characters starts one, all at
   the same place); 2 index + 2 frames later (colour_delay) takes the colours of the record's car
   (ranking_record_character_palettes by its model); 3 58 frames after its start (frames_left) slides out
   to the left, 16 pixels a frame, and ends at x -20. */
void ranking_record_character_update(void)
{
	uint32_t animation = 0, character_index = 0;
	struct object *object = NULL;
	uint8_t colour_delay = 0;
	uint16_t frames_left = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->ranking_record_character.frames_left = 0x3Au;
		current_object->draw_mode = 0xAu;
		current_object->palette = 0x27u;
		animation = NAME_ENTRY_FONT;
		animation_start(animation);
		object = pointer_to(current_object->ranking_record_character.ranking_record);
		character_index = current_object->ranking_record_character.index;
		current_object->depth_layer = current_object->ranking_record_character.index;
		current_object->frame = object->ranking_record.text[(int32_t)character_index];
		current_object->x = ranking_record_character_xs[character_index];
		current_object->y = ranking_record_character_ys[character_index];
		current_object->ranking_record_character.slide_steps = 0u;
	case 1:
		current_object->x += 0x20u;
		current_object->ranking_record_character.slide_steps++;
		if (current_object->ranking_record_character.slide_steps >= 7u)
		{
			object = start_object();
			object->handler = ranking_record_car_update_ADDRESS;
			object->ranking_record_car.course = current_object->ranking_record_character.course;
			current_object->state++;
		}
		current_object->ranking_record_character.colour_delay = current_object->ranking_record_character.index + 2u;
		colour_delay = current_object->ranking_record_character.colour_delay;
		current_object->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			character_index = current_object->ranking_record_character.course;
			current_object->palette = ranking_record_character_palettes[game->course_records[(uint8_t)(character_index * 6u)]];
			current_object->state++;
		}
		break;
	case 2:
		colour_delay = current_object->ranking_record_character.colour_delay;
		current_object->ranking_record_character.colour_delay = colour_delay - 1u;
		if (colour_delay == 1u)
		{
			character_index = current_object->ranking_record_character.course;
			current_object->palette = ranking_record_character_palettes[game->course_records[(uint8_t)(character_index * 6u)]];
			current_object->state++;
		}
		frames_left = current_object->ranking_record_character.frames_left;
		current_object->ranking_record_character.frames_left = frames_left - 1u;
		if (frames_left != 1u)
		{
			draw_object(character_index);
			return;
		}
		current_object->state++;
		current_object->x -= 0x10u;
		if (current_object->x != 0xFFE0u)
		{
			draw_object(character_index);
			return;
		}
		current_object->flags = 0u;
		return;
	case 3:
		break;
	case 4:
		current_object->x -= 0x10u;
		if (current_object->x != 0xFFE0u)
		{
			draw_object(character_index);
			return;
		}
		current_object->flags = 0u;
		return;
	}
	frames_left = current_object->ranking_record_character.frames_left;
	current_object->ranking_record_character.frames_left = frames_left - 1u;
	if (frames_left != 1u)
	{
		draw_object(character_index);
		return;
	}
	current_object->state++;
	current_object->x -= 0x10u;
	if (current_object->x != 0xFFE0u)
	{
		draw_object(character_index);
		return;
	}
	current_object->flags = 0u;
}

/* The record holder's car under the course's record line (animation car_model_turn_animations by the
   model of the course's record, palette car_model_palettes; size 50, y 88): drives in from the left 16
   pixels a frame to x 108 or more, turns round (frame 9 down to 4), waits 15 frames (wait_frames),
   drives back left 16 pixels a frame and ends at x 0. State 5 (drawn only) is not set by its own code. */
void ranking_record_car_update(void)
{
	uint32_t animation = 0, car_model = 0, course = 0;
	uint16_t wait_frames = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->ranking_record_car.wait_frames = 0xFu;
		current_object->draw_mode = 0xAu;
		course = current_object->ranking_record_car.course;
		car_model = game->course_records[(uint8_t)(course * 6u)];
		animation = car_model_turn_animations[car_model];
		current_object->palette = car_model_palettes[car_model];
		animation_start(animation);
		current_object->x = 0u;
		current_object->y = 0x88u;
		current_object->height_scale = 0x50u;
		current_object->width_scale = 0x50u;
		current_object->frame = 9u;
		current_object->depth_layer = 0x19Eu;
		current_object->x += 0x10u;
		if (current_object->x < 0x108u)
		{
			draw_object(car_model);
			return;
		}
		current_object->state++;
		draw_object(car_model);
		return;
	case 1:
		current_object->x += 0x10u;
		if (current_object->x < 0x108u)
		{
			draw_object(car_model);
			return;
		}
		current_object->state++;
		draw_object(car_model);
		return;
	case 2:
		current_object->frame--;
		if (current_object->frame != 4u)
		{
			draw_object(car_model);
			return;
		}
		current_object->state++;
		wait_frames = current_object->ranking_record_car.wait_frames;
		current_object->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			draw_object(0u);
			return;
		}
		current_object->state++;
		current_object->x -= 0x10u;
		if (current_object->x != 0u)
		{
			draw_object(car_model);
			return;
		}
		current_object->flags = 0u;
		return;
	case 3:
		wait_frames = current_object->ranking_record_car.wait_frames;
		current_object->ranking_record_car.wait_frames = wait_frames - 1u;
		if (wait_frames != 1u)
		{
			draw_object(0u);
			return;
		}
		current_object->state++;
		current_object->x -= 0x10u;
		if (current_object->x != 0u)
		{
			draw_object(car_model);
			return;
		}
		current_object->flags = 0u;
		return;
	case 4:
		current_object->x -= 0x10u;
		if (current_object->x != 0u)
		{
			draw_object(car_model);
			return;
		}
		current_object->flags = 0u;
		return;
	case 5:
		draw_object(car_model);
		return;
	}
}

/* Counts the steps of the passing cars' path (traffic_path: size, x, y; closed by 0) into path_steps,
   then runs passing_traffic_update. */
void passing_traffic_start(void)
{
	uint8_t step_count = 0;
	const struct traffic_path_step *traffic_path_step = NULL;

	traffic_path_step = rom_pointer(traffic_path_ADDRESS);
	step_count = 0u;
	while (traffic_path_step->size != 0u)
	{
		step_count++;
		traffic_path_step = traffic_path_step + 1;
	}
	current_object->passing_traffic.path_steps = step_count;
	current_object->handler = passing_traffic_update_ADDRESS;
	passing_traffic_update();
}

/* Now and then a car passes on the road behind the certificate: by state, 0 one frame in 16 at random
   (random_number) starts a passing_car on the path's last step (path_steps), 1 then waits 10 frames
   (wait). */
void passing_traffic_update(void)
{
	uint32_t random = 0;
	struct object *object = NULL;
	uint16_t wait = 0;

	switch (current_object->state)
	{
	case 0:
		random = random_number();
		if ((random & 0xFu) == 0u)
		{
			object = start_object();
			object->handler = passing_car_update_ADDRESS;
			object->passing_car.step = current_object->passing_traffic.path_steps;
			current_object->passing_traffic.wait = 0xAu;
			current_object->state++;
		}
		return;
	case 1:
		wait = current_object->passing_traffic.wait;
		current_object->passing_traffic.wait = wait - 1u;
		if (wait == 1u)
			current_object->state--;
		return;
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
	uint32_t animation = 0, car_model = 0, mirrored_x = 0, path_size = 0, random_side = 0, step = 0, step_index = 0;
	const struct traffic_path_step *traffic_path_step = NULL;
	uint16_t near_step = 0, far_step = 0;

	switch (current_object->state)
	{
	case 0:
		car_model = random_number();
		random_side = car_model;
		car_model &= 7u;
		animation = car_model_turn_animations[car_model];
		current_object->palette = car_model_palettes[car_model];
		animation_start(animation);
		pass_sound = car_pass_sounds[car_model];
		request_sound(pass_sound);
		current_object->draw_mode = 0xAu;
		current_object->depth_layer = 3u;
		current_object->frame = 0xCu;
		current_object->state++;
		if ((int32_t)random_side >= 0)
		{
			step = (uint16_t)current_object->passing_car.step;
			near_step = step - 1u;
			step_index = near_step;
			if ((int16_t)near_step < 0)
			{
				current_object->flags = 0u;
				return;
			}
			current_object->passing_car.step = step_index;
			traffic_path_step = rom_pointer(traffic_path_ADDRESS + step_index * 3u * 2u);
			path_size = traffic_path_step->size;
			current_object->width_scale = path_size;
			current_object->height_scale = path_size;
			current_object->x = traffic_path_step->x;
			current_object->y = traffic_path_step->y;
			draw_object(path_size);
			return;
		}
		current_object->frame = 1u;
		current_object->state++;
		step = (uint16_t)current_object->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			current_object->flags = 0u;
			return;
		}
		current_object->passing_car.step = step_index;
		traffic_path_step = rom_pointer(traffic_path_ADDRESS + step_index * 3u * 2u);
		mirrored_x = traffic_path_step->size;
		current_object->width_scale = mirrored_x;
		current_object->height_scale = mirrored_x;
		current_object->x = 0x96u - traffic_path_step->x;
		current_object->y = traffic_path_step->y;
		draw_object(mirrored_x);
		return;
	case 1:
		step = (uint16_t)current_object->passing_car.step;
		near_step = step - 1u;
		step_index = near_step;
		if ((int16_t)near_step < 0)
		{
			current_object->flags = 0u;
			return;
		}
		current_object->passing_car.step = step_index;
		traffic_path_step = rom_pointer(traffic_path_ADDRESS + step_index * 3u * 2u);
		path_size = traffic_path_step->size;
		current_object->width_scale = path_size;
		current_object->height_scale = path_size;
		current_object->x = traffic_path_step->x;
		current_object->y = traffic_path_step->y;
		draw_object(path_size);
		return;
	case 2:
		step = (uint16_t)current_object->passing_car.step;
		far_step = step - 1u;
		step_index = far_step;
		if ((int16_t)far_step < 0)
		{
			current_object->flags = 0u;
			return;
		}
		current_object->passing_car.step = step_index;
		traffic_path_step = rom_pointer(traffic_path_ADDRESS + step_index * 3u * 2u);
		mirrored_x = traffic_path_step->size;
		current_object->width_scale = mirrored_x;
		current_object->height_scale = mirrored_x;
		current_object->x = 0x96u - traffic_path_step->x;
		current_object->y = traffic_path_step->y;
		draw_object(mirrored_x);
		return;
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
	uint32_t background_number = 0, background_pages = 0, colour_list = 0, index = 0, mixer_place = 0, mixer_priority = 0, page_numbers = 0, page_place = 0, pages_to_copy = 0, palette_fades = 0, queued_pages = 0;
	const struct ranking_background_registers *ranking_background_registers = NULL;
	uint32_t saved_fades = 0;

	ranking_background_registers = rom_pointer(ranking_background_register_sets_ADDRESS + screen->index * 0xCu * 4u);
	current_object->ranking_background.tile_bank_address = ranking_background_registers->tile_bank_address;
	current_object->ranking_background.pages_address = ranking_background_registers->pages_address;
	current_object->ranking_background.scroll_x_address = ranking_background_registers->scroll_x_address;
	current_object->ranking_background.scroll_y_address = ranking_background_registers->scroll_y_address;
	current_object->ranking_background.zoom_x_address = ranking_background_registers->zoom_x_address;
	current_object->ranking_background.zoom_y_address = ranking_background_registers->zoom_y_address;
	current_object->ranking_background.zoom_center_x_address = ranking_background_registers->zoom_center_x_address;
	current_object->ranking_background.zoom_center_y_address = ranking_background_registers->zoom_center_y_address;
	current_object->ranking_background.cleared_scroll_x_address = ranking_background_registers->cleared_scroll_x_address;
	current_object->ranking_background.cleared_scroll_y_address = ranking_background_registers->cleared_scroll_y_address;
	current_object->ranking_background.second_scroll_x_address = ranking_background_registers->second_scroll_x_address;
	current_object->ranking_background.second_scroll_y_address = ranking_background_registers->second_scroll_y_address;
	game->send_video_registers = 0xFFu;
	background_number = course_backgrounds[screen->record_course];
	TILE_BANK_VALUES(current_object->ranking_background.tile_bank_address)[0] = ranking_background_tile_banks[background_number];
	saved_fades = background_palette_fades[background_number];
	colour_list = background_palettes[background_number];
	background_pages = background_tile_page_lists[background_number] + 0x10u;
	page_place = address_of(current_object) + 0xB0u;
	page_numbers = ranking_background_page_numbers[screen->index];
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
	queued_pages = address_of(current_object) + 0xB0u;
	queue_tile_pages(queued_pages);
	queue_palette_list(colour_list);
	palette_fades = saved_fades;
	start_palette_fades(palette_fades);
	VIDEO_REGISTER_WORDS(current_object->ranking_background.zoom_x_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.zoom_y_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.zoom_center_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.zoom_center_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.cleared_scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.cleared_scroll_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.second_scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(current_object->ranking_background.second_scroll_y_address)[0] = 0u;
	index = screen->index;
	PAGE_SELECT(current_object->ranking_background.pages_address)[0] = byte_swap32(ranking_background_page_numbers[index]);
	VIDEO_REGISTER_WORDS(current_object->ranking_background.scroll_x_address)[0] = ranking_background_scroll_xs[index];
	VIDEO_REGISTER_WORDS(current_object->ranking_background.scroll_y_address)[0] = 0x60u;
	mixer_place = ranking_mixer_addresses[index];
	mixer_priority = MIXER_VALUE_PAIRS(mixer_place)[0];
	mixer_priority = mixer_priority & ~0xFu;
	MIXER_VALUE_PAIRS(mixer_place)[0] = mixer_priority | 1u;
	mixer_place += 4u;
	mixer_priority = MIXER_VALUE_PAIRS(mixer_place)[0];
	MIXER_VALUE_PAIRS(mixer_place)[0] = mixer_priority & ~0xFu;
	screen->flags |= 4u;
}
