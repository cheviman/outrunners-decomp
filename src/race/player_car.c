/**
 *    Name:        race/player_car.c
 *    Description: the screen's car each frame: its place, the view, the rivals' warning, the crash physics
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The screen's car, each frame: player_car_check_place, player_car_read_controls,
   player_car_build_hit_boxes, then by state: 0 sets up the race (the view's constants in screen
   eye_distance-18, the course, the car on its grid place: segment 16 + grid_segment_lists by players and
   place, road_x from grid_road_place_lists, its world position; the palette of its model,
   player_car_palettes; timer 8 frames in the demo, 149 in a race with the intro view car
   view_shift_intro/view_pitch_intro; the gears, torque curves, crash physics, with screen is_host the
   race's time left (race time_left: 40 seconds plus master_time_difficulty), the engine sound); 1 the
   countdown to the start: brake full and the wheel centred until timer runs out (others read it as the
   frames to the start), then car moving and race started; 2 driving (player_car_move): goes to 3 when
   the time is up (car time_up), to 4 at the goal (car at_goal); counts hit_guard down; on a hit (flags
   bit 2, by hit_kind): 3 a fork sign slows it for 15 frames (car slowed_frames), 0 scenery, 1 a road
   object (hit_other), 2 the other player's car (hit_other): a speed change, a push and a bounce from the
   model's crash table (car crash_table, by impact_speed or the cars' speeds and places), the hit's
   sounds, a crash animation, the CRASH text (car crash_text), immune (flags bit 3), state 5 and its
   first frame; 5 the crash plays out (player_car_crash_step); 3 time up: gas off and the model's brake
   (player_car_time_up_brakes), 180 frames, then the race is over (car race_over), GAME OVER (004771)
   with one player or race_result_start with two, 60 frames, screen flags bit 4 and DJ message 36 or 37;
   4 the goal: finished, the race's stage count up, waits for the screen's finish time (screen 410) into
   race finish_time and the race time, race finished, then drives itself (player_car_steer_to_stop) to
   its stop_position by its place (goal_stops), plays the model's arrival animation
   (player_car_arrival_animations), brakes to a stop, 90 frames, screen flags bit 4. */
void player_car_update(void)
{
	uint32_t overflow = 0;
	uint8_t crash_hit_sound = 0, dj_message = 0, frame_time = 0, tyre_noise = 0;
	uint16_t grid_segment = 0, view_height_offset = 0;
	uint32_t arrival_animation = 0, brake_index = 0, crash_animation = 0, distance_index = 0, draw_distances = 0, goal_stop = 0, grid_list = 0, grid_place = 0, grid_road_place = 0, hit_side = 0, impact = 0, model = 0, other_slide = 0, other_weight = 0, other_x = 0, own_slide = 0, own_speed = 0, own_weight = 0, player_time = 0, road_x = 0, screen_view_default = 0, segment_fraction = 0, segment_length = 0, segment_record = 0, side_gap = 0, speed_difference = 0, speed_loss = 0, start_record = 0;
	const struct crash_entry *crash_entry = NULL;
	struct object *hit_object = NULL, *object = NULL;
	struct race *other_record = NULL;
	struct road_segment *car_segment_record = NULL;
	uint8_t hit_guard = 0;
	uint16_t timer = 0, timer_2 = 0, timer_3 = 0, timer_4 = 0;
	uint32_t stage_count_place = 0;

	game->wheel_motor[screen->index] = 0u;
	player_car_check_place();
	player_car_read_controls();
	player_car_build_hit_boxes();
	for (;;)
	{
		switch (current_object->state)
		{
		case 0:
			screen->current_stage = screen->built_stage;
			draw_distances = screen_view_defaults[0].draw_distances;
			distance_index = RACE(screen->host_record)->racer_count;
			screen_view_default = screen_view_defaults_ADDRESS + 0x4u;
			if (game->isdn_mode != 0u)
				distance_index = 0u;
			screen->draw_distance = DRAW_DISTANCE_LIST(draw_distances)[(int32_t)distance_index];
			screen->eye_distance = SCREEN_VIEW_DEFAULT(screen_view_default - 0x4u)->eye_distance;
			screen_view_default += 2u;
			screen->sprite_origin_x = SCREEN_VIEW_DEFAULT(screen_view_default - 0x6u)->sprite_origin_x;
			screen_view_default += 2u;
			screen->sprite_origin_y = SCREEN_VIEW_DEFAULT(screen_view_default - 0x8u)->sprite_origin_y;
			screen->view_pitch_base = SCREEN_VIEW_DEFAULT(screen_view_default - 0x8u)->view_pitch_bases[player_car->model];
			player_car_set_horizon();
			current_object->state++;
			player_car->object = address_of(current_object);
			player_car_start_course();
			start_record = address_of(screen) + 0x2000u;
			player_car->window_record = start_record;
			grid_segment = 0x10u;
			player_car->window_segment = 0u;
			grid_place = screen->places[screen->player];
			grid_list = 0u;
			if ((screen->demo_flags & 0x100u) == 0u)
			{
				grid_list = RACE(screen->host_record)->racer_count;
				grid_list = (uint8_t)(grid_list - 1u);
			}
			grid_segment += GRID_SEGMENTS(grid_segment_lists[grid_list])[(int32_t)grid_place];
			player_car->segment = grid_segment;
			grid_segment = (uint16_t)(grid_segment * 0x20u);
			player_car->segment_offset = grid_segment;
			start_record = LINKED(start_record, (uint16_t)(start_record + grid_segment));
			grid_road_place = GRID_ROAD_PLACES(grid_road_place_lists[grid_list])[(int32_t)grid_place];
			player_car->road_x = (uint16_t)grid_road_place;
			player_car->segment_remaining = (uint16_t)(grid_road_place >> 16u);
			current_object->player_car.heading = ROAD_SEGMENT(start_record)->heading;
			start_record += 2u;
			current_object->player_car.pitch = ROAD_SEGMENT(start_record - 0x2u)->pitch;
			start_record += 2u;
			current_object->player_car.world_x = grid_road_place + ROAD_SEGMENT(start_record - 0x4u)->x;
			start_record += 4u;
			current_object->player_car.world_z = ROAD_SEGMENT(start_record - 0x8u)->z;
			current_object->player_car.world_y = ROAD_SEGMENT(start_record - 0x8u)->y;
			current_object->palette = player_car_palettes[player_car->model];
			current_object->player_car.timer = 8u;
			if ((screen->demo_flags & 0x100u) == 0u)
			{
				current_object->player_car.timer = 0x95u;
				player_car->view_shift_intro = 0xFA60u;
				player_car->view_pitch_intro = 0xF0u;
			}
			player_car_load_gears();
			player_car_build_torque_curves();
			player_car_place_on_road();
			player_car_set_horizon();
			player_car_extend_course();
			screen->shown_view = address_of(screen) + 0x600u;
			player_car_project_road(&r12, &r14);
			player_car_load_crash_physics();
			if (screen->is_host == 0u)
			{
				player_car_start_engine_sound();
				player_car_animate();
				player_car_publish();
				return;
			}
			RACE(screen->race)->time_left = (game->race_copies[0].master_time_difficulty_0 + 0x28u) * 0x1Eu;
			player_car_start_engine_sound();
			player_car_animate();
			player_car_publish();
			return;
		case 1:
			timer = current_object->player_car.timer;
			current_object->player_car.timer = timer - 1u;
			if (timer != 1u)
			{
				current_object->player_car.brake = 0xFFu;
				current_object->player_car.wheel = 0x80u;
				player_car_move();
				return;
			}
			current_object->state++;
			player_car->moving = 0xFFu;
			if ((screen->demo_flags & 0x100u) == 0u)
			{
				RACE(screen->race)->started = 0xFFu;
				screen->race_started = 0xFFu;
			}
		case 2:
			if (player_car->time_up == 0u)
			{
				if (player_car->at_goal == 0u)
					break;
				current_object->state = 4u;
				current_object->player_car.step = 0u;
				current_object->player_car.state_high = 0u;
				continue;
			}
			current_object->state = 3u;
			current_object->player_car.step = 0u;
			current_object->player_car.state_high = 0u;
		case 3:
			if (player_car->at_goal != 0u)
			{
				current_object->state = 4u;
				current_object->player_car.step = 0u;
				current_object->player_car.state_high = 0u;
				continue;
			}
			current_object->player_car.gas = 0u;
			brake_index = player_car->model;
			brake_index = (uint8_t)(brake_index * 2u);
			current_object->player_car.brake = player_car_time_up_brakes[(uint8_t)(brake_index + player_car->transmission)];
			switch (current_object->player_car.step)
			{
			case 0:
				current_object->player_car.step++;
				current_object->player_car.timer = 0xB4u;
				player_car_clear_hit();
			case 1:
				timer_2 = current_object->player_car.timer;
				current_object->player_car.timer = timer_2 - 1u;
				if (timer_2 != 1u)
				{
					if (player_car->speed != 0u)
					{
						player_car->race_time = RACE(screen->host_record)->race_time;
						player_car_move();
						return;
					}
					player_car_move();
					model = player_car->model;
					player_car_steer_frame(model);
					return;
				}
				current_object->player_car.step++;
				current_object->player_car.timer = 0x3Cu;
				player_car->race_over = 0xFFu;
				if (RACE(screen->host_record)->racer_count <= 1u)
				{
					object = start_object();
					object->handler = game_over_banner_show_ADDRESS;
				}
				else
				{
					object = start_object();
					object->handler = race_result_start_ADDRESS;
				}
			case 2:
				timer_3 = current_object->player_car.timer;
				current_object->player_car.timer = timer_3 - 1u;
				if (timer_3 != 1u)
				{
					player_car_move();
					model = player_car->model;
					player_car_steer_frame(model);
					return;
				}
				current_object->player_car.step++;
				screen->flags |= 0x10u;
				dj_message = 0x37u;
				if ((int8_t)screen->current_stage_level > 3)
				{
					start_dj_message(dj_message);
					player_car_move();
					model = player_car->model;
					player_car_steer_frame(model);
					return;
				}
				dj_message = 0x36u;
				start_dj_message(dj_message);
			case 3:
				player_car_move();
				model = player_car->model;
				player_car_steer_frame(model);
				return;
			}
			break;
		case 4:
			switch (current_object->player_car.step)
			{
			case 0:
				current_object->player_car.step++;
				current_object->player_car.finished = 0xFFu;
				stage_count_place = screen->race + 0x60u;
				RACE(stage_count_place - 0x60u)->stage_count++;
				current_object->player_car.stop_position = player_car->segment;
				player_car_clear_hit();
				current_object->flags |= 8u;
				player_car_move();
				return;
			case 1:
				player_time = screen->checkpoint_times_by_player[screen->player + 0x28u];
				if (player_time == 0u)
				{
					player_car_move();
					return;
				}
				RACE(screen->race)->finish_time = player_time;
				player_car->race_time = player_time * 0xAu;
				RACE(screen->race)->finished = 0xFFu;
				current_object->player_car.step++;
				goal_stop = goal_stops[screen->places[screen->player]];
				current_object->player_car.stop_position += GOAL_STOP(goal_stop)->position;
				goal_stop += 2u;
				current_object->player_car.stop_x = GOAL_STOP(goal_stop - 0x2u)->x;
				current_object->player_car.stop_speed = GOAL_STOP(goal_stop - 0x2u)->speed;
				current_object->player_car.stop_wheel = current_object->player_car.wheel;
				current_object->player_car.stop_gas = current_object->player_car.gas;
			case 2:
				if (player_car->segment < current_object->player_car.stop_position)
				{
					player_car_steer_to_stop();
					player_car_move();
					return;
				}
				current_object->player_car.step++;
				current_object->player_car.stop_speed = 0u;
				arrival_animation = player_car_arrival_animations[player_car->model];
				animation_start(arrival_animation);
			case 3:
				current_object->player_car.gas = 0x40u;
				current_object->player_car.brake = 0u;
				current_object->player_car.wheel = 0x80u;
				player_car_steer_to_stop();
				tyre_noise = car_skid_noises[player_car->model];
				set_tyre_noise(tyre_noise);
				if (player_car->speed != 0u)
				{
					segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
					player_car_drive(&segment_record, &r12, &road_x, &segment_fraction, &segment_length);
					player_car->wheel_effect_1 = 2u;
					player_car->wheel_effect_2 = 2u;
					player_car->wheel_effect_sides = 3u;
					AS16(player_car->brake_lights) = 2u;
					player_car_place_view(pointer_to(segment_record), r12, road_x, segment_fraction, segment_length);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road(&r12, &r14);
					player_car_engine_sound();
					animation_advance(0u);
					return;
				}
				current_object->player_car.step++;
				current_object->player_car.timer = 0x5Au;
			case 4:
				current_object->player_car.gas = 0u;
				current_object->player_car.brake = 0x80u;
				current_object->player_car.wheel = 0x80u;
				timer_4 = current_object->player_car.timer;
				current_object->player_car.timer = timer_4 - 1u;
				if (timer_4 != 1u)
				{
					segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
					player_car_drive(&segment_record, &r12, &road_x, &segment_fraction, &segment_length);
					player_car_place_view(pointer_to(segment_record), r12, road_x, segment_fraction, segment_length);
					player_car_move_view();
					player_car_set_horizon();
					player_car_publish();
					player_car_extend_course();
					player_car_project_road(&r12, &r14);
					player_car_engine_sound();
					animation_advance(0u);
					return;
				}
				current_object->player_car.step++;
				screen->flags |= 0x10u;
				segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
				player_car_drive(&segment_record, &r12, &road_x, &segment_fraction, &segment_length);
				player_car_place_view(pointer_to(segment_record), r12, road_x, segment_fraction, segment_length);
				player_car_move_view();
				player_car_set_horizon();
				player_car_publish();
				player_car_extend_course();
				player_car_project_road(&r12, &r14);
				player_car_engine_sound();
				animation_advance(0u);
				return;
			case 5:
				current_object->player_car.gas = 0u;
				current_object->player_car.brake = 0x80u;
				current_object->player_car.wheel = 0x80u;
				segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
				player_car_drive(&segment_record, &r12, &road_x, &segment_fraction, &segment_length);
				player_car_place_view(pointer_to(segment_record), r12, road_x, segment_fraction, segment_length);
				player_car_move_view();
				player_car_set_horizon();
				player_car_publish();
				player_car_extend_course();
				player_car_project_road(&r12, &r14);
				player_car_engine_sound();
				animation_advance(0u);
				return;
			}
			break;
		case 5:
			player_car_crash_step();
			return;
		}
		break;
	}
	if (current_object->player_car.hit_guard == 0u)
		current_object->flags &= 0xF7u;
	else
	{
		hit_guard = current_object->player_car.hit_guard;
		current_object->player_car.hit_guard = hit_guard - 1u;
		if (hit_guard == 1u)
			current_object->flags &= 0xF7u;
	}
	player_car_check_car_hits();
	if ((current_object->flags & 4u) == 0u)
	{
		player_car->race_time = RACE(screen->host_record)->race_time;
		player_car_move();
		return;
	}
	switch (current_object->player_car.hit_kind)
	{
	case 3:
		current_object->flags &= 0xFBu;
		player_car->slowed_frames = 0xFu;
		player_car->race_time = RACE(screen->host_record)->race_time;
		player_car_move();
		return;
	case 0:
		crash_entry = rom_pointer(player_car->crash_table);
		impact = current_object->player_car.impact_speed;
		if (impact >= 0xFFFFu)
			impact = 0xFFFFu;
		speed_loss = 0u - impact;
		impact = (impact >> 7) + crash_place_offsets[screen->places[screen->player]];
		if (impact >= 0x1F0u)
			impact = 0x1F0u;
		crash_entry = rom_pointer(address_of(crash_entry) + (impact & ~0xFE0Fu));
		current_object->player_car.speed_change = (int32_t)(speed_loss * crash_entry->speed_factor) >> 8;
		hit_side = (uint16_t)player_car->road_x;
		hit_side = hit_side >> 15;
		current_object->player_car.sideways_push = crash_entry->push[(int32_t)hit_side];
		current_object->player_car.bounce_speed = crash_entry->bounce_speed;
		current_object->state = 5u;
		current_object->player_car.step = 0u;
		current_object->player_car.state_high = 0u;
		current_object->flags |= 8u;
		player_car->crash_sound = crash_entry->crash_sound;
		player_car->crash_sound_delay = 0xAu;
		crash_hit_sound = (uint8_t)(crash_entry->hit_sound | ((uint32_t)crash_entry->crash_sound << 8u));
		request_sound(crash_hit_sound);
		crash_animation = crash_entry->animation[(int32_t)hit_side];
		view_height_offset = player_car_view_height();
		crash_animation = (uint16_t)(crash_animation + view_height_offset);
		current_object->animation = crash_animation;
		frame_time = SPRITE_ANIMATION(sprite_animations[crash_animation])->frame_time;
		current_object->frame_time = frame_time;
		current_object->frame_countdown = frame_time;
		current_object->frame = 0u;
		player_car->crash_text = 0xFFu;
		player_car_crash_step();
		return;
	case 1:
		hit_object = pointer_to(LINKED(address_of(screen), current_object->player_car.hit_other));
		car_segment_record = pointer_to(PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset));
		own_speed = current_object->player_car.speed;
		impact = (uint32_t)(uint16_t)hit_object->traffic.speed << 8;
		other_weight = hit_object->traffic.weight;
		side_gap = player_car->road_x * 2u;
		other_x = (hit_object->x_offset + (uint32_t)car_segment_record->half_width * hit_object->side) * 2u;
		own_slide = current_object->player_car.sideways_speed * 2u;
		other_slide = hit_object->traffic.sideways_speed * 2u;
		break;
	case 2:
		other_record = pointer_to(current_object->player_car.hit_other);
		other_record = pointer_to(RACE_COPY_1 + (uint16_t)(address_of(other_record) << 7));
		side_gap = player_car->road_x;
		other_x = other_record->road_x;
		own_slide = current_object->player_car.sideways_speed;
		other_slide = other_record->sideways_speed;
		own_speed = current_object->player_car.speed;
		impact = other_record->speed;
		own_speed = own_speed >> 1;
		impact = impact >> 1;
		other_weight = car_weights[other_record->model];
		break;
	}
	own_weight = car_weights[player_car->model];
	crash_entry = rom_pointer(player_car->crash_table);
	impact -= own_speed;
	speed_difference = impact;
	if ((int32_t)impact < 0)
		impact = (0u - impact) * 2u;
	impact = divide_s32((impact >> 8) * other_weight, own_weight) + crash_place_offsets[screen->places[screen->player]];
	if (impact >= 0x1F0u)
		impact = 0x1F0u;
	crash_entry = rom_pointer(address_of(crash_entry) + (impact & ~0xFE0Fu));
	current_object->player_car.speed_change = (int32_t)(((int32_t)(speed_difference * crash_entry->car_speed_factor) >> 8) * other_weight) >> 8;
	side_gap = side_gap - other_x + ((other_slide - own_slide) << 1);
	hit_side = (0u - side_gap) >> 31;
	side_gap = divide_s32(((int32_t)(side_gap * (int16_t)crash_entry->push[1]) >> 7) * other_weight, own_weight);
	overflow = v60_movt_overflow(side_gap, 32, 16);
	current_object->player_car.sideways_push = side_gap;
	current_object->player_car.bounce_speed = crash_entry->bounce_speed;
	current_object->state = 5u;
	current_object->player_car.step = 0u;
	current_object->player_car.state_high = 0u;
	current_object->flags |= 8u;
	player_car->crash_sound = crash_entry->crash_sound;
	player_car->crash_sound_delay = 0xAu;
	crash_hit_sound = (uint8_t)(crash_entry->hit_sound | ((uint32_t)crash_entry->crash_sound << 8u));
	request_sound(crash_hit_sound);
	crash_animation = crash_entry->animation[(int32_t)hit_side];
	view_height_offset = player_car_view_height();
	crash_animation = (uint16_t)(crash_animation + view_height_offset);
	current_object->animation = crash_animation;
	frame_time = SPRITE_ANIMATION(sprite_animations[crash_animation])->frame_time;
	current_object->frame_time = frame_time;
	current_object->frame_countdown = frame_time;
	current_object->frame = 0u;
	player_car->crash_text = 0xFFu;
	player_car_crash_step();
}

/* One frame of the car on the road: player_car_drive, player_car_place_view, player_car_move_view,
   player_car_set_horizon, player_car_publish, player_car_extend_course, player_car_project_road,
   player_car_engine_sound and player_car_animate; no wheel effects while it crashes (state 5). */
void player_car_move(void)
{
	uint32_t road_x = 0, segment_fraction = 0, segment_length = 0, segment_record = 0;

	segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
	player_car_drive(&segment_record, &r12, &road_x, &segment_fraction, &segment_length);
	player_car_place_view(pointer_to(segment_record), r12, road_x, segment_fraction, segment_length);
	if (current_object->state == 5u)
		player_car->wheel_effect_sides = 0u;
	player_car_move_view();
	player_car_set_horizon();
	player_car_publish();
	player_car_extend_course();
	player_car_project_road(&r12, &r14);
	player_car_engine_sound();
	player_car_animate();
}

/* Drives the car by itself after the goal: gas 40, no brake, the speed (car speed) a 16th of the way to
   stop_speed each frame, and the wheel turned step by step toward stop_x across the road (centred within
   8 of it; stop_wheel keeps it). */
void player_car_steer_to_stop(void)
{
	uint32_t negative = 0, overflow = 0;
	uint8_t stop_wheel = 0;
	uint16_t distance_from_stop = 0, stop_x = 0;
	uint32_t speed = 0, speed_gap = 0;
	uint16_t road_x = 0, x_from_stop = 0;
	uint32_t stop_speed = 0;

	current_object->player_car.brake = 0u;
	current_object->player_car.gas = 0x40u;
	speed = player_car->speed;
	stop_speed = current_object->player_car.stop_speed;
	speed_gap = stop_speed - speed;
	if (stop_speed != speed)
		speed += (int32_t)(speed_gap * 1u) / 0x10 + (shift_signed_by(speed_gap, 0xE0u, 32) * 2u + 1u);
	player_car->speed = speed;
	stop_wheel = current_object->player_car.stop_wheel;
	stop_x = (uint16_t)current_object->player_car.stop_x;
	road_x = player_car->road_x;
	x_from_stop = road_x - stop_x;
	overflow = (uint32_t)(int16_t)road_x - (int16_t)stop_x != (uint32_t)(int16_t)(road_x - stop_x);
	negative = x_from_stop >> 15;
	distance_from_stop = x_from_stop;
	if (road_x == stop_x)
	{
		current_object->player_car.stop_wheel = 0x80u;
		current_object->player_car.wheel = 0x80u;
		return;
	}
	if (negative ^ overflow)
	{
		if ((int16_t)distance_from_stop >= -8)
		{
			current_object->player_car.stop_wheel = 0x80u;
			current_object->player_car.wheel = 0x80u;
			return;
		}
		stop_wheel--;
		if ((int8_t)stop_wheel < 0x10)
			stop_wheel = 0x10u;
		if ((int8_t)stop_wheel > 0x6F)
			stop_wheel = 0x6Fu;
		current_object->player_car.stop_wheel = stop_wheel;
		current_object->player_car.wheel = stop_wheel;
		return;
	}
	if ((int16_t)distance_from_stop <= 8)
	{
		current_object->player_car.stop_wheel = 0x80u;
		current_object->player_car.wheel = 0x80u;
		return;
	}
	stop_wheel++;
	if ((int8_t)stop_wheel > -0x31)
		stop_wheel = 0xCFu;
	if ((int8_t)stop_wheel < -0x70)
		stop_wheel = 0x90u;
	current_object->player_car.stop_wheel = stop_wheel;
	current_object->player_car.wheel = stop_wheel;
}

/* Keeps the race order around the screen's car: screen leader_record-6C the race records (leader_player
   to behind_player the players) of the leader, the last, the car just ahead and the car just behind (all
   itself in the demo), screen leader_window_segment and leader_stage the leader's race window_segment
   and stage. When its place (screen 320 + player, kept in place) changes during the race: sound 37 when
   the car it passed or that passed it is within 160 across, else 3A. While first, blinks one of the
   screen's lamps (the count, period and timer bytes C9-CB of its entry at checkpoint_lamp). */
void player_car_check_place(void)
{
	uint32_t zero = 0, carry = 0;
	uint8_t sound = 0;
	uint16_t distance_across = 0;
	uint32_t ahead_player = 0, behind_place = 0, behind_player = 0, last_place = 0, last_player = 0, leader_player = 0, place = 0, race = 0;
	struct race *leader_race = NULL, *passed_race = NULL;
	struct screen_lamps *lamp = NULL;
	uint8_t old_place = 0;

	if ((screen->demo_flags & 0x100u) != 0u)
	{
		leader_player = screen->player;
		last_player = leader_player;
		ahead_player = leader_player;
		behind_player = leader_player;
		screen->leader_player = leader_player;
		screen->last_player = last_player;
		screen->ahead_player = ahead_player;
		screen->behind_player = behind_player;
		leader_race = pointer_to(race_copy_offsets[leader_player] + GAME_VARIABLES);
		screen->leader_record = address_of(leader_race);
		screen->last_record = race_copy_offsets[last_player] + GAME_VARIABLES;
		screen->ahead_record = race_copy_offsets[ahead_player] + GAME_VARIABLES;
		screen->behind_record = race_copy_offsets[behind_player] + GAME_VARIABLES;
		screen->leader_window_segment = leader_race->window_segment;
		screen->leader_stage = leader_race->stage;
		return;
	}
	race = screen->host_record + 0x1Fu;
	last_place = RACE(race - 0x1Fu)->racer_count;
	last_place = (uint8_t)(last_place - 1u);
	race++;
	last_player = RACE_ORDER_PLAYERS(race)[(int32_t)last_place];
	leader_player = RACE_ORDER_PLAYERS(race)[0];
	place = screen->places[screen->player];
	if (place == 0u)
	{
		lamp = pointer_to(GAME_VARIABLES + screen->index * 4u);
		if (lamp->leader_blinks == 0u)
		{
			lamp->leader_blinks = 2u;
			lamp->leader_period = 0x10u;
			lamp->leader_timer = 0x10u;
		}
	}
	old_place = current_object->player_car.place;
	carry = place < old_place;
	zero = (uint8_t)(place - old_place) == 0u;
	if (place != old_place)
	{
		current_object->player_car.place = place;
		passed_race = pointer_to(screen->behind_record);
		if (carry | zero)
			passed_race = pointer_to(screen->ahead_record);
		distance_across = (uint16_t)(passed_race->road_x - RACE(screen->race_copy)->road_x);
		sound = 0x3Au;
		if ((int16_t)distance_across < 0x160 && (int16_t)distance_across > -0x160)
			sound = 0x37u;
		if (current_object->state != 0u)
			request_sound(sound);
	}
	behind_place = place;
	if (place != 0u)
		place = (uint8_t)(place - 1u);
	if (behind_place != last_place)
		behind_place = (uint8_t)(behind_place + 1u);
	ahead_player = RACE_ORDER_PLAYERS(race)[(int32_t)place];
	behind_player = RACE_ORDER_PLAYERS(race)[(int32_t)behind_place];
	screen->leader_player = leader_player;
	screen->last_player = last_player;
	screen->ahead_player = ahead_player;
	screen->behind_player = behind_player;
	leader_race = pointer_to(race_copy_offsets[leader_player] + GAME_VARIABLES);
	screen->leader_record = address_of(leader_race);
	screen->last_record = race_copy_offsets[last_player] + GAME_VARIABLES;
	screen->ahead_record = race_copy_offsets[ahead_player] + GAME_VARIABLES;
	screen->behind_record = race_copy_offsets[behind_player] + GAME_VARIABLES;
	screen->leader_window_segment = leader_race->window_segment;
	screen->leader_stage = leader_race->stage;
}

/* Hands the car's state on: this frame's view into the screen's ring of eight (screen + 600, slot
   view_ring_slot, also race view_slot) and the view of the slot race_copy view_slot names back into the
   car and screen shown_view, so that both screens draw the view of the same frame; and the car's world
   position, animation, palette, frame, wheel effects, speed, sideways speed, flags, segments, place
   across the road and bounce into its race record (race published set), from which race_car draws it on
   both screens. Not in ISDN mode (isdn_mode), which reads them back instead. */
struct view *player_car_publish(void)
{
	uint32_t view_ring_slot = 0, view_slot = 0;
	struct race *race = NULL, *race_copy = NULL;
	struct view *shown_view = NULL, *view = NULL;

	race = pointer_to(screen->race);
	race_copy = pointer_to(screen->race_copy);
	if (game->isdn_mode == 0u)
	{
		view_ring_slot = screen->view_ring_slot;
		view_ring_slot = (uint8_t)(view_ring_slot + 1u);
		view_ring_slot = view_ring_slot & 7u;
		screen->view_ring_slot = view_ring_slot;
		race->view_slot = view_ring_slot;
		view = pointer_to(address_of(screen) + 0x600u + (uint16_t)(view_ring_slot << 5));
		view_slot = race_copy->view_slot;
		shown_view = pointer_to(address_of(screen) + 0x600u + (uint16_t)(view_slot << 5));
		screen->shown_view = address_of(shown_view);
	}
	else
	{
		view = &screen->views[0];
		shown_view = view;
		screen->shown_view = address_of(shown_view);
	}
	view->x = player_car->view_x;
	player_car->view_x = shown_view->x;
	view->y = player_car->view_y;
	player_car->view_y = shown_view->y;
	view->z = player_car->view_z;
	player_car->view_z = shown_view->z;
	view->pitch = player_car->view_pitch;
	player_car->view_pitch = shown_view->pitch;
	view->yaw = player_car->view_yaw;
	player_car->view_yaw = shown_view->yaw;
	view->window_offset = player_car->window_offset;
	view->segment_remaining = player_car->segment_remaining;
	view->horizon_line = screen->horizon_line;
	screen->horizon_line = shown_view->horizon_line;
	if (game->isdn_mode == 0u)
	{
		race->world_x = current_object->player_car.world_x;
		race->world_y = current_object->player_car.world_y;
		race->world_z = current_object->player_car.world_z;
		race->animation = current_object->animation;
		race->palette = current_object->palette;
		race->frame = current_object->frame;
		race->wheel_effect_sides = player_car->wheel_effect_sides;
		race->hard_acceleration = player_car->hard_acceleration;
		race->wheel_effect_1 = player_car->wheel_effect_1;
		race->wheel_effect_2 = player_car->wheel_effect_2;
		race->speed = current_object->player_car.speed;
		race->sideways_speed = current_object->player_car.sideways_speed;
		race->car_flags = (current_object->flags | ((uint32_t)current_object->flags_high << 8u));
		race->segment_fraction = player_car->segment_fraction;
		race->segment = player_car->segment;
		race->road_x = player_car->road_x;
		race->window_segment = player_car->window_segment;
		race->stage = screen->built_stage;
		race->bounce_height = (uint16_t)current_object->player_car.bounce_height;
		race->published = 0xFFu;
	}
	return view;
}

/* Clears what the screen's car hands on to the race record (player_car_publish's), for the ranking
   (game_mode_ranking): race published, the next of the ring of eight views (screen view_ring_slot, race
   view_slot, its address in screen shown_view) with the view's place and angles cleared in it and the
   car (view_x-z, view_pitch, view_yaw), the wheel effects and hard acceleration (race
   wheel_effect_sides-53), segment_fraction, segment, road_x, window_segment (race segment_fraction-46),
   window_offset, segment_remaining, the horizon (screen horizon_line), and the halfword at C8 of
   current_object (player_car_publish's bounce_height) with race bounce_height; from the ranking
   current_object is what run_objects left in it, not the car. */
struct view *clear_published_car(void)
{
	uint32_t view_ring_slot = 0;
	struct race *race = NULL;
	struct view *view = NULL;

	race = pointer_to(screen->race);
	race->published = 0u;
	view_ring_slot = screen->view_ring_slot;
	view_ring_slot = (uint8_t)(view_ring_slot + 1u);
	view_ring_slot = view_ring_slot & 7u;
	screen->view_ring_slot = view_ring_slot;
	race->view_slot = view_ring_slot;
	view = pointer_to(address_of(screen) + 0x600u + (uint16_t)(view_ring_slot << 5));
	screen->shown_view = address_of(view);
	player_car->view_x = 0;
	view->x = 0u;
	player_car->view_y = 0;
	view->y = 0u;
	player_car->view_z = 0;
	view->z = 0u;
	player_car->view_pitch = 0u;
	view->pitch = 0u;
	player_car->view_yaw = 0u;
	view->yaw = 0u;
	player_car->wheel_effect_sides = 0u;
	race->wheel_effect_sides = 0u;
	player_car->hard_acceleration = 0u;
	race->hard_acceleration = 0u;
	player_car->wheel_effect_1 = 0u;
	race->wheel_effect_1 = 0u;
	player_car->wheel_effect_2 = 0u;
	race->wheel_effect_2 = 0u;
	player_car->segment_fraction = 0u;
	race->segment_fraction = 0u;
	player_car->segment = 0u;
	race->segment = 0u;
	player_car->road_x = 0;
	race->road_x = 0;
	player_car->window_segment = 0u;
	race->window_segment = 0u;
	player_car->window_offset = 0u;
	view->window_offset = 0u;
	player_car->segment_remaining = 0u;
	view->segment_remaining = 0u;
	screen->horizon_line = 0u;
	view->horizon_line = 0u;
	current_object->player_car.bounce_height &= 0xFFFF0000u;
	race->bounce_height = 0;
	return view;
}

/* Works out the horizon's line on the screen (screen horizon_line) from the view's tilt: the nod (car
   view_pitch_nod), the rival's and the fork camera's pulls (view_pitch_rival, view_pitch_lift), the car
   model's base pitch (screen view_pitch_base), clamped to C0-200, and the intro's (view_pitch_intro). */
void player_car_set_horizon(void)
{
	uint32_t draw_distance = 0, eye_distance = 0, tilt = 0;

	tilt = (uint16_t)(player_car->view_pitch_nod + player_car->view_pitch_rival + player_car->view_pitch_lift + screen->view_pitch_base);
	if ((int16_t)tilt > 0x200)
		tilt = 0x200u;
	if ((int16_t)tilt < 0xC0)
		tilt = 0xC0u;
	tilt = (uint16_t)(tilt + player_car->view_pitch_intro) & 0xFFFu;
	eye_distance = screen->eye_distance;
	draw_distance = screen->draw_distance;
	screen->horizon_line = divide_s32(divide_s32(sine_table[tilt] * draw_distance, (uint32_t)cosine_table[tilt]) * eye_distance, draw_distance + eye_distance) - screen->sprite_origin_y + 0x100u;
}

/* Puts the car on its starting segment at the start: no way into the segment, road_x from the grid's
   place, its world position (player_car_world_position) and the view behind it (player_car_place_view). */
void player_car_place_on_road(void)
{
	uint32_t next_segment = 0, road_x = 0, segment_fraction = 0, segment_length = 0, segment_record = 0;

	player_car->segment_fraction = 0u;
	player_car->road_x = (uint16_t)current_object->player_car.world_x;
	segment_record = PAGE_ADD(address_of(screen) + 0x2000u, player_car->segment_offset);
	road_x = (uint16_t)player_car->road_x;
	segment_length = 0x80u;
	segment_fraction = player_car->segment_fraction;
	next_segment = (segment_record + 0x20u) & ~0x5000u;
	player_car_world_position(pointer_to(segment_record), pointer_to(next_segment), road_x, segment_fraction, segment_length);
	player_car_place_view(pointer_to(segment_record), next_segment, road_x, segment_fraction, segment_length);
}

/* Ends what a hit was doing to the car: no sideways_push, speed_change, bounce, no hit pending (flags
   bit 2), and the car no longer skidding (car skidding) or slipping (wheel_slip). */
void player_car_clear_hit(void)
{
	current_object->player_car.sideways_push = 0;
	current_object->player_car.speed_change = 0;
	current_object->player_car.bounce_height = 0;
	current_object->player_car.bounce_speed = 0;
	current_object->player_car.sideways_push = 0;
	current_object->flags &= 0xFBu;
	player_car->skidding = 0u;
	player_car->wheel_slip = 0u;
}

/* Picks the car's animation: while a hit is pending, the one it has goes on; skidding (car skidding), a
   skid by how far it slides (slide) toward the side the wheel is turned
   (player_car_skid_animations_wheel_high, player_car_skid_animations_wheel_low by model); spinning its
   wheels while it speeds up (car wheel_slip, acceleration above 0), the model's wheelspin
   (player_car_wheelspin_animations); else its body turned by the steering (player_car_steer_frame). Each
   one in the row of the view's height (player_car_view_height); a new one starts at frame 0. */
void player_car_animate(void)
{
	uint8_t frame_time = 0, slide = 0;
	uint16_t view_height = 0;
	uint32_t model_then_animation = 0;

	model_then_animation = player_car->model;
	if ((current_object->flags & 4u) != 0u)
	{
		animation_advance(0u);
		return;
	}
	if (player_car->skidding != 0u)
	{
		slide = (uint8_t)(uint16_t)current_object->player_car.slide;
		model_then_animation = (uint16_t)(model_then_animation << 4);
		if ((int8_t)current_object->player_car.wheel >= 0)
		{
			if ((int8_t)slide > 0)
				slide = (uint8_t)(0u - slide);
			model_then_animation = player_car_skid_animations_wheel_low[(model_then_animation & ~0xFFu) | (uint8_t)(model_then_animation + (0u - slide))];
		}
		else
		{
			if ((int8_t)slide < 0)
				slide = (uint8_t)(0u - slide);
			model_then_animation = player_car_skid_animations_wheel_high[(model_then_animation & ~0xFFu) | (uint8_t)(model_then_animation + slide)];
		}
		view_height = player_car_view_height();
		model_then_animation = (uint16_t)(model_then_animation + view_height);
		if (current_object->animation != model_then_animation)
		{
			current_object->animation = model_then_animation;
			frame_time = SPRITE_ANIMATION(sprite_animations[model_then_animation])->frame_time;
			current_object->frame_time = frame_time;
			current_object->frame_countdown = frame_time;
			current_object->frame = 0u;
		}
		animation_advance(0u);
		return;
	}
	if (current_object->player_car.acceleration <= 0 || player_car->wheel_slip == 0u)
	{
		player_car_steer_frame(model_then_animation);
		return;
	}
	model_then_animation = player_car_wheelspin_animations[model_then_animation];
	view_height = player_car_view_height();
	model_then_animation = (uint16_t)(model_then_animation + view_height);
	if (current_object->animation != model_then_animation)
	{
		current_object->animation = model_then_animation;
		frame_time = SPRITE_ANIMATION(sprite_animations[model_then_animation])->frame_time;
		current_object->frame_time = frame_time;
		current_object->frame_countdown = frame_time;
		current_object->frame = 0u;
	}
	animation_advance(0u);
}

/* The car's body as it steers: the model's animation (player_car_steer_animations) in the row of the
   view's height, its frame (steer_frame) one step a frame toward the one of the steering
   (steer_frames_by_steering, from its middle: 0 full left to 8 full right, 4 straight on). */
void player_car_steer_frame(uint32_t model)
{
	uint8_t steer_frame = 0, target_frame = 0;
	uint16_t animation = 0, view_height = 0;

	animation = player_car_steer_animations[model];
	view_height = player_car_view_height();
	current_object->animation = animation + view_height;
	target_frame = steer_frames_by_steering[64 + current_object->player_car.steering];
	steer_frame = current_object->player_car.steer_frame;
	if (target_frame == steer_frame)
	{
		current_object->frame = steer_frame;
		current_object->player_car.steer_frame = steer_frame;
		return;
	}
	if (target_frame < steer_frame)
		steer_frame--;
	else
		steer_frame++;
	current_object->frame = steer_frame;
	current_object->player_car.steer_frame = steer_frame;
}

/* The row of the car's animations for how steeply the view looks down on it: the view's pitch (car
   view_pitch) less B0 and less the road's pitch under the car (race_copy segment_pitch), in steps of 30,
   0 to 5 (view_height). */
uint16_t player_car_view_height(void)
{
	uint16_t highest = 0, tilt = 0, view_height = 0;
	uint16_t pitch_over_road = 0;

	view_height = 0u;
	highest = 5u;
	pitch_over_road = ((player_car->view_pitch - 0xB0u) << 4) - ((uint32_t)RACE(screen->race_copy)->segment_pitch << 4);
	tilt = pitch_over_road;
	if ((int16_t)pitch_over_road >= 0)
	{
		view_height = tilt / 0x300u;
		if (highest <= view_height)
			view_height = (uint16_t)highest;
	}
	current_object->player_car.view_height = (uint8_t)view_height;
	return view_height;
}

/* Moves the view with what the car does: when a rival comes close (player_car_check_rivals, rival_step)
   it starts the RIVAL SEKKIN warning (rival_warning) and pulls the view back and up (car
   view_shift_rival to -256, view_pitch_rival +8 a step) until the rival has gone, then lets it back; a
   speed gain of more than A0 in a frame without wheelspin sets car hard_acceleration (hard
   acceleration); the view nods (nod, car view_shift_nod and view_pitch_nod) one way with that hard
   acceleration and the other with hard braking (more than 120 a frame with the brake down), easing back;
   and it lags the car's rises and drops (car view_rise). */
void player_car_move_view(void)
{
	uint16_t climb = 0, last_negated_speed = 0, nod = 0, nod_step = 0, rise_step = 0, side_step = 0, side_target = 0, view_rise = 0, view_side = 0;
	uint32_t rise_over_margin = 0, view_shift_rival = 0;
	struct object *object = NULL;
	uint16_t speed_drop = 0, nod_target = 0, eased_step = 0, negative_side_target = 0, positive_side_target = 0, drop_target = 0, climb_target = 0;
	uint32_t rise = 0, rise_with_margin = 0;

	for (;;)
	{
		switch (current_object->player_car.rival_step)
		{
		case 0:
			current_object->player_car.rival_step++;
			if (current_object->player_car.rival_near != 0u)
			{
				object = start_object();
				object->handler = rival_warning_update_ADDRESS;
				current_object->player_car.rival_step = 2u;
				continue;
			}
			break;
		case 1:
			if (current_object->player_car.rival_near != 0u)
			{
				object = start_object();
				object->handler = rival_warning_update_ADDRESS;
				current_object->player_car.rival_step = 2u;
				continue;
			}
			break;
		case 2:
			player_car_check_rivals();
			if (current_object->player_car.rival_near != 0u)
			{
				view_shift_rival = player_car->view_shift_rival;
				if (view_shift_rival == 0xFF00u)
					break;
				view_shift_rival = (uint16_t)(view_shift_rival - 0x10u);
				player_car->view_pitch_rival += 8u;
				player_car->view_shift_rival = view_shift_rival;
				break;
			}
			current_object->player_car.rival_step = 3u;
			player_car_check_rivals();
			if (current_object->player_car.rival_near != 0u)
			{
				current_object->player_car.rival_step = 2u;
				continue;
			}
			view_shift_rival = player_car->view_shift_rival;
			if (view_shift_rival == 0u)
			{
				current_object->player_car.rival_step = 1u;
				break;
			}
			view_shift_rival = (uint16_t)(view_shift_rival + 0x10u);
			player_car->view_pitch_rival -= 8u;
			player_car->view_shift_rival = view_shift_rival;
			break;
		case 3:
			player_car_check_rivals();
			if (current_object->player_car.rival_near == 0u)
			{
				view_shift_rival = player_car->view_shift_rival;
				if (view_shift_rival == 0u)
				{
					current_object->player_car.rival_step = 1u;
					break;
				}
				view_shift_rival = (uint16_t)(view_shift_rival + 0x10u);
				player_car->view_pitch_rival -= 8u;
				player_car->view_shift_rival = view_shift_rival;
				break;
			}
			current_object->player_car.rival_step = 2u;
			continue;
		}
		break;
	}
	player_car->hard_acceleration = 0u;
	nod_step = (uint16_t)(0u - current_object->player_car.speed);
	last_negated_speed = current_object->player_car.last_negated_speed;
	current_object->player_car.last_negated_speed = nod_step;
	nod_step -= last_negated_speed;
	nod = (uint16_t)current_object->player_car.nod;
	if ((current_object->flags & 4u) != 0u)
		nod_step = 0u;
	else if ((int16_t)nod_step < 0)
	{
		if (player_car->wheel_slip != 0u || (int16_t)nod_step >= -0xA0)
			nod_step = 0u;
		else
		{
			nod_step = (uint16_t)(nod_step * 1u);
			player_car->hard_acceleration = 0xFFu;
		}
	}
	else if (current_object->player_car.brake_force == 0u)
		nod_step = 0u;
	else
	{
		speed_drop = nod_step;
		nod_step = (uint16_t)(speed_drop - 0x120u);
		if ((int16_t)speed_drop <= 0x120)
			nod_step = 0u;
		else
			nod_step = (uint16_t)(nod_step * 1u);
	}
	nod_target = (int16_t)(nod_step * 1u) / 5;
	nod_step = (uint16_t)(nod_target - nod);
	if (nod_target != nod)
	{
		eased_step = (int16_t)nod_step >> 3;
		nod_step = eased_step;
		if (eased_step == 0u)
			nod_step++;
	}
	if ((int16_t)nod_step < -0x20)
		nod_step = 0xFFE0u;
	if ((int16_t)nod_step > 6)
		nod_step = 6u;
	nod += nod_step;
	if ((int16_t)nod > 0x18)
		nod = 0x18u;
	if ((int16_t)nod < -0x100)
		nod = 0xFF00u;
	current_object->player_car.nod = nod;
	player_car->view_shift_nod = nod;
	if ((int16_t)nod >= 0)
		nod = (uint16_t)((int16_t)(nod * 1u) / 1);
	else
		nod = (uint16_t)((int16_t)(nod * 1u) / 1);
	player_car->view_pitch_nod = nod;
	side_target = (uint16_t)((0u - current_object->player_car.sideways_speed) << 1);
	view_side = player_car->view_side;
	if (player_car->skidding == 0u)
	{
		if (view_side != 0u)
		{
			if ((int16_t)view_side >= 0)
				view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
			else
				view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if ((int16_t)side_target < 0)
	{
		negative_side_target = side_target;
		side_step = (uint16_t)(negative_side_target - view_side);
		if (negative_side_target != view_side)
		{
			view_side += ((int16_t)side_step >> 2) + 1u;
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if ((int16_t)side_target > 0)
	{
		positive_side_target = side_target;
		side_step = (uint16_t)(positive_side_target - view_side);
		if (positive_side_target != view_side)
		{
			view_side += (int16_t)side_step >> 2;
			if ((int16_t)view_side > 0x40)
				view_side = 0x40u;
		}
	}
	else if (view_side != 0u)
	{
		if ((int16_t)view_side >= 0)
			view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
		else
			view_side = (uint16_t)((int16_t)(view_side * 3u) / 4);
		if ((int16_t)view_side > 0x40)
			view_side = 0x40u;
	}
	view_rise = (uint16_t)(int16_t)player_car->view_rise;
	rise = (uint32_t)current_object->player_car.world_y - current_object->player_car.previous_world_y;
	rise_over_margin = rise + 0xCu;
	if ((int32_t)rise < -0xC)
	{
		drop_target = rise_over_margin;
		rise_step = (uint16_t)(drop_target - view_rise);
		if (drop_target == (uint16_t)view_rise)
			return;
		view_rise += ((int16_t)rise_step >> 2) + 1u;
	}
	else
	{
		rise_with_margin = rise_over_margin;
		climb = (uint16_t)(rise_with_margin - 0xCu);
		if ((int32_t)rise_with_margin > 0xC)
		{
			climb_target = climb;
			rise_step = (uint16_t)(climb_target - view_rise);
			if (climb_target == (uint16_t)view_rise)
				return;
			view_rise += (int16_t)rise_step >> 2;
		}
		else
		{
			if ((uint16_t)view_rise == 0u)
				return;
			if ((int16_t)view_rise >= 0)
				view_rise = (uint16_t)((int16_t)(view_rise * 3u) / 4);
			else
				view_rise = (uint16_t)((int16_t)(view_rise * 3u) / 4);
		}
	}
	if ((int16_t)view_rise > 8)
		view_rise = 8u;
	if ((int16_t)view_rise < -8)
		view_rise = 0xFFF8u;
	player_car->view_rise = view_rise;
}

/* Whether a rival is close (rival_near): at speed 6000 or more (5000 to stay on), the car just ahead
   (screen ahead_record) a little slower and within reach, or the car just behind (behind_record) a
   little faster and close; once on it stays for 30 frames (rival_hold) after the rival has gone. */
void player_car_check_rivals(void)
{
	uint32_t gap = 0, position = 0, speed = 0, speed_gap = 0;
	struct race *ahead_race = NULL, *race = NULL, *race_2 = NULL;

	race = pointer_to(screen->race_copy);
	ahead_race = pointer_to(screen->ahead_record);
	race_2 = pointer_to(screen->behind_record);
	position = (race->segment_fraction | ((uint32_t)race->segment << 16u));
	speed = race->speed;
	position = (position & ~0xFFFFu) | (uint16_t)(position << 9);
	if (address_of(ahead_race) != address_of(race))
	{
		gap = (ahead_race->segment_fraction | ((uint32_t)ahead_race->segment << 16u));
		gap = ((gap & ~0xFFFFu) | (uint16_t)(gap << 9)) - position;
		speed_gap = ahead_race->speed - speed;
		if (current_object->player_car.rival_near == 0u)
		{
			if (speed < 0x6000u)
			{
				if (current_object->player_car.rival_hold == 0u)
				{
					current_object->player_car.rival_near = 0u;
					return;
				}
				current_object->player_car.rival_hold--;
				return;
			}
			if ((int32_t)gap <= 0x18000 && (int32_t)speed_gap <= -1 && (int32_t)speed_gap >= -0x100)
			{
				current_object->player_car.rival_hold = 0x1Eu;
				if (current_object->player_car.rival_hold != 0u)
					current_object->player_car.rival_hold--;
				current_object->player_car.rival_near = 0xFFu;
				return;
			}
		}
		else
		{
			if (speed < 0x5000u)
			{
				if (current_object->player_car.rival_hold == 0u)
				{
					current_object->player_car.rival_near = 0u;
					return;
				}
				current_object->player_car.rival_hold--;
				return;
			}
			if ((int32_t)gap <= 0x20000 && (int32_t)speed_gap <= -1 && (int32_t)speed_gap >= -0x200)
			{
				if (current_object->player_car.rival_hold != 0u)
					current_object->player_car.rival_hold--;
				current_object->player_car.rival_near = 0xFFu;
				return;
			}
		}
	}
	if (address_of(race_2) == address_of(race))
	{
		if (current_object->player_car.rival_hold == 0u)
		{
			current_object->player_car.rival_near = 0u;
			return;
		}
		current_object->player_car.rival_hold--;
		return;
	}
	gap = (race_2->segment_fraction | ((uint32_t)race_2->segment << 16u));
	gap = ((gap & ~0xFFFFu) | (uint16_t)(gap << 9)) - position;
	speed_gap = race_2->speed - speed;
	if (current_object->player_car.rival_near == 0u)
	{
		if (speed < 0x6000u || (int32_t)gap < -0x30000 || (int32_t)speed_gap > 0x2000 || (int32_t)speed_gap < 0)
		{
			if (current_object->player_car.rival_hold == 0u)
			{
				current_object->player_car.rival_near = 0u;
				return;
			}
			current_object->player_car.rival_hold--;
			return;
		}
		current_object->player_car.rival_hold = 0x1Eu;
		if (current_object->player_car.rival_hold != 0u)
			current_object->player_car.rival_hold--;
		current_object->player_car.rival_near = 0xFFu;
		return;
	}
	if (speed < 0x5000u || (int32_t)gap < -0x40000 || (int32_t)speed_gap > 0x3000 || (int32_t)speed_gap < -0x1000)
	{
		if (current_object->player_car.rival_hold == 0u)
		{
			current_object->player_car.rival_near = 0u;
			return;
		}
		current_object->player_car.rival_hold--;
		return;
	}
	if (current_object->player_car.rival_hold != 0u)
		current_object->player_car.rival_hold--;
	current_object->player_car.rival_near = 0xFFu;
}

/* RIVAL SEKKIN !! (a rival approaching) at the centre of the screen: 12 frames on, 6 off, 12 on, with
   the rival_alarm, then it ends. */
void rival_warning_update(void)
{
	uint32_t text = 0;
	struct object *object = NULL;
	uint8_t off_frames = 0, on_frames = 0, blinks = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->rival_warning.blinks = 2u;
		current_object->rival_warning.frames = 0xCu;
		object = start_object();
		object->handler = rival_alarm_update_ADDRESS;
		break;
	case 1:
		break;
	case 2:
		off_frames = current_object->rival_warning.frames;
		current_object->rival_warning.frames = off_frames - 1u;
		if (off_frames == 1u)
		{
			current_object->state--;
			current_object->rival_warning.frames = 0xCu;
		}
		return;
	}
	on_frames = current_object->rival_warning.frames;
	current_object->rival_warning.frames = on_frames - 1u;
	if (on_frames != 1u)
	{
		text = rival_warning_text_ADDRESS;
		draw_text(rom_pointer(text));
		return;
	}
	blinks = current_object->rival_warning.blinks;
	current_object->rival_warning.blinks = blinks - 1u;
	if (blinks == 1u)
	{
		current_object->flags = 0u;
		current_object->flags_high = 0u;
		return;
	}
	current_object->state++;
	current_object->rival_warning.frames = 6u;
	off_frames = current_object->rival_warning.frames;
	current_object->rival_warning.frames = off_frames - 1u;
	if (off_frames == 1u)
	{
		current_object->state--;
		current_object->rival_warning.frames = 0xCu;
	}
}

/* The sound of the rival warning: sound 2B twice, 15 frames apart. */
void rival_alarm_update(void)
{
	uint8_t sound = 0;
	uint8_t frames = 0;

	switch (current_object->state)
	{
	case 0:
		sound = 0x2Bu;
		request_sound(sound);
		current_object->state++;
		current_object->rival_alarm.frames = 0xFu;
		return;
	case 1:
		frames = current_object->rival_alarm.frames;
		current_object->rival_alarm.frames = frames - 1u;
		if (frames == 1u)
		{
			sound = 0x2Bu;
			request_sound(sound);
			current_object->flags = 0u;
			current_object->flags_high = 0u;
		}
		return;
	}
}

/* Loads the crash physics of the car's model (car_crash_physics): bounce_damping, speed_change_decay,
   push_decay, gravity and the address of its crash table. */
void player_car_load_crash_physics(void)
{
	uint32_t crash_physics = 0;

	crash_physics = car_crash_physics[player_car->model];
	player_car->bounce_damping = CRASH_PHYSICS(crash_physics)->bounce_damping;
	crash_physics += 2u;
	player_car->speed_change_decay = CRASH_PHYSICS(crash_physics - 0x2u)->speed_change_decay;
	crash_physics += 2u;
	player_car->push_decay = CRASH_PHYSICS(crash_physics - 0x4u)->push_decay;
	crash_physics += 2u;
	player_car->gravity = CRASH_PHYSICS(crash_physics - 0x6u)->gravity;
	player_car->crash_table = crash_physics + 2u;
}

/* The crash playing out, each frame of player_car_update's state 5 and the frame a hit starts it: at the
   goal (car at_goal) back to 2 at once; else the wheel motor at 10, the hit's sound once its delay (car
   crash_sound_delay) runs out, no controls, the push and the speed change dying away (car push_decay,
   speed_change_decay), a bounce under gravity (car gravity, bounce_damping; sound 31 on landing), and
   back to 2 with 15 frames of hit_guard when the crash animation ends (frame_time 0). */
void player_car_crash_step(void)
{
	uint8_t landing_sound = 0;
	uint32_t crash_sound = 0, gravity = 0, new_bounce_speed = 0, new_height = 0, old_height = 0;
	uint8_t crash_sound_delay = 0;

	if (player_car->at_goal != 0u)
	{
		current_object->state = 2u;
		current_object->player_car.step = 0u;
		current_object->player_car.state_high = 0u;
		current_object->flags &= 0xFBu;
		current_object->player_car.hit_guard = 0xFu;
		current_object->player_car.sideways_push = 0;
		current_object->player_car.bounce_speed = 0;
		current_object->player_car.bounce_height = 0;
		current_object->player_car.speed_change = 0;
		current_object->player_car.gas = 0u;
		player_car->race_time = RACE(screen->host_record)->race_time;
		player_car_move();
		return;
	}
	game->wheel_motor[screen->index] = 0x10u;
	if (player_car->crash_sound_delay != 0u)
	{
		crash_sound_delay = player_car->crash_sound_delay;
		player_car->crash_sound_delay = crash_sound_delay - 1u;
		if (crash_sound_delay == 1u)
		{
			crash_sound = player_car->crash_sound;
			if (crash_sound != 0u)
				request_sound((uint8_t)crash_sound);
		}
	}
	if (current_object->frame_time == 0u)
	{
		current_object->state = 2u;
		current_object->player_car.step = 0u;
		current_object->player_car.state_high = 0u;
		current_object->flags &= 0xFBu;
		current_object->player_car.hit_guard = 0xFu;
		current_object->player_car.sideways_push = 0;
		current_object->player_car.bounce_speed = 0;
		current_object->player_car.bounce_height = 0;
		current_object->player_car.speed_change = 0;
		current_object->player_car.gas = 0u;
		player_car->race_time = RACE(screen->host_record)->race_time;
		player_car_move();
		return;
	}
	current_object->player_car.gas = 0u;
	current_object->player_car.brake = 0u;
	current_object->player_car.wheel = 0x80u;
	current_object->player_car.sideways_push = (int32_t)((uint32_t)current_object->player_car.sideways_push * player_car->push_decay) / 0x100;
	current_object->player_car.speed_change = (int32_t)((uint32_t)current_object->player_car.speed_change * player_car->speed_change_decay) / 0x100;
	new_height = current_object->player_car.bounce_height;
	gravity = player_car->gravity;
	new_bounce_speed = current_object->player_car.bounce_speed - gravity;
	old_height = new_height;
	new_height += new_bounce_speed;
	if ((int32_t)(old_height ^ new_height) < 0)
	{
		new_height = 0u;
		new_bounce_speed = (int32_t)((new_bounce_speed + gravity) * player_car->bounce_damping) / -0x100;
		if (new_bounce_speed != 0u)
		{
			landing_sound = 0x31u;
			request_sound(landing_sound);
		}
	}
	current_object->player_car.bounce_speed = new_bounce_speed;
	current_object->player_car.bounce_height = new_height;
	player_car->race_time = RACE(screen->host_record)->race_time;
	player_car_move();
}
