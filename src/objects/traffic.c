/**
 *    Name:        objects/traffic.c
 *    Description: the traffic: its cars, convoys, people and their hits
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the traffic of the stage the race's leader is in (stage_road_object_lists). By state: 0 the
   list of the stage being built (screen built_stage), traffic_level = 8 - the number of players (byte 1F
   of the record screen host_record points at) + the game setting game_difficulty, the draw distance into
   screen traffic_draw_distance, no traffic yet (screen traffic_count); 1 (outside the attract demo,
   screen demo_flags bit 8) when the leader's stage (screen leader_stage) changes, takes its list with
   positions from where the leader was then (screen leader_window_segment); starts each record (position,
   level, handler, stop_distance, record) whose position is within the draw distance of the leader's view
   (window_segment of the race record screen leader_record), when its level is not above traffic_level
   and fewer than 32 are on the road (screen traffic_count); then writes at F84 of the screen the
   stretches of road the players can see (each player's view, window_segment of the race records listed
   after byte 1F, give or take the draw distance; a count, then each stretch's top and bottom from the
   highest), for traffic_out_of_view. */
void traffic_spawner_update(void)
{
	uint32_t negative = 0, overflow = 0;
	uint16_t object_position = 0, position = 0, spawn_position = 0, stretch_count = 0, stretch_top = 0, traffic_draw_distance = 0, view_bottom = 0, view_top = 0;
	uint32_t built_stage = 0, leader_stage = 0, racer_list = 0, racers_left = 0, stretches = 0;
	struct object *traffic_object = NULL;
	const struct traffic_spawn_entry *entry = NULL;
	uint16_t stage_start = 0, road_position = 0, level = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->traffic_spawner.stage_start = 0;
		built_stage = screen->built_stage;
		current_object->traffic_spawner.stage = built_stage;
		current_object->traffic_spawner.list = stage_road_object_lists[built_stage];
		current_object->traffic_spawner.traffic_level = (uint8_t)(8u - RACE(screen->host_record)->racer_count + game->game_difficulty);
		screen->traffic_draw_distance = screen->draw_distance >> 7;
		screen->traffic_racer_list = screen->host_record + 0x1Fu;
		screen->traffic_count = 0u;
		current_object->state++;
		return;
	case 1:
		entry = rom_pointer(current_object->traffic_spawner.list);
		if ((screen->demo_flags & 0x100u) == 0u && current_object->traffic_spawner.stage != screen->leader_stage)
		{
			current_object->traffic_spawner.stage_start = screen->leader_window_segment;
			leader_stage = screen->leader_stage;
			current_object->traffic_spawner.stage = leader_stage;
			entry = rom_pointer(stage_road_object_lists[leader_stage]);
			current_object->traffic_spawner.list = address_of(entry);
		}
		for (;;)
		{
			position = (uint16_t)entry->position;
			stage_start = current_object->traffic_spawner.stage_start;
			road_position = position + stage_start;
			overflow = (uint32_t)(int16_t)position + (int16_t)stage_start != (uint32_t)(int16_t)(position + stage_start);
			negative = road_position >> 15;
			spawn_position = road_position;
			if (negative ^ overflow)
				break;
			object_position = (uint16_t)spawn_position;
			if ((int16_t)RACE(screen->leader_record)->window_segment < (int16_t)(spawn_position - screen->traffic_draw_distance))
				break;
			level = entry->level;
			if (current_object->traffic_spawner.traffic_level < level || screen->traffic_count >= 0x20u)
			{
				entry++;
				current_object->traffic_spawner.list = address_of(entry);
				continue;
			}
			traffic_object = start_object();
			traffic_object->position = object_position;
			traffic_object->handler = entry->handler;
			traffic_object->traffic.stop_distance = entry->stop_distance;
			traffic_object->traffic.record = entry->record;
			entry++;
			current_object->traffic_spawner.list = address_of(entry);
			screen->traffic_count++;
		}
		racer_list = screen->traffic_racer_list;
		stretches = address_of(screen) + 0xF84u;
		traffic_draw_distance = screen->traffic_draw_distance;
		stretch_count = 0u;
		stretch_top = 0xFFFFu;
		racers_left = RACER_NUMBERS(racer_list)[0];
		racer_list++;
		do
		{
			view_bottom = game->race_copies[RACER_NUMBERS(racer_list)[0]].window_segment;
			view_top = (uint16_t)((uint32_t)view_bottom + traffic_draw_distance);
			view_bottom -= traffic_draw_distance;
			view_top++;
			racer_list++;
			if (view_top < stretch_top)
			{
				STRETCH_ENDS(stretches)[0] = stretch_top;
				stretches += 2u;
				STRETCH_ENDS(stretches)[0] = view_top - 1u;
				stretch_count++;
				stretches += 2u;
			}
			stretch_top = (uint16_t)view_bottom;
			racers_left--;
		} while (racers_left != 0u);
		STRETCH_ENDS(stretches)[0] = stretch_top;
		screen->visible_stretch_count = stretch_count;
		return;
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
	uint32_t line = 0;
	struct object *leader = NULL, *object = NULL;

	line = current_object->traffic.record;
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
		object->position = leader->position - spacing;
		object->handler = traffic_follower_update_ADDRESS;
		object->traffic_follower.stop_distance = leader->traffic.stop_distance;
		object->traffic_follower.record = r12;
		object->traffic_follower.leader = address_of(leader);
		leader = object;
		screen->traffic_count++;
	}
	current_object->traffic.record = line;
	current_object->handler = traffic_car_update_ADDRESS;
	traffic_car_update();
}

/* A traffic car, drawn from the angle the player sees it at. By state: 0 traffic_car_start; 1 moves
   along the road by speed (128ths of a segment a frame, position_fraction carrying into position), then
   traffic_car_draw; once the screen's car has reached the goal (car at_goal) it ends when it is 8
   segments behind the view. */
void traffic_car_update(void)
{
	uint16_t position_fraction = 0, whole_segment = 0;

	switch (current_object->state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	}
	if (player_car->at_goal != 0u && (int16_t)(current_object->position - 8u) < (int16_t)RACE(screen->race_copy)->window_segment)
	{
		screen->traffic_count--;
		current_object->flags = 0u;
		return;
	}
	position_fraction = (uint16_t)(current_object->position_fraction + current_object->traffic.speed);
	whole_segment = 0x80u;
	if ((int16_t)position_fraction >= 0x80)
	{
		do
		{
			current_object->position++;
			position_fraction -= whole_segment;
		} while ((int16_t)whole_segment <= (int16_t)position_fraction);
	}
	traffic_car_draw(position_fraction);
}

/* Sets up a traffic car from its record (the address of its sprite record, then side, x_offset and
   speed): from the sprite record its animation (animation_start), palette, passing_sound, hit_sound,
   weight and hit_box (the sprite record's last 8 bytes); draw_mode B (the frame chosen by the angle it
   is seen from, traffic_car_draw), scale_shift FF, passed 0; goes on to state 1. */
void traffic_car_start(void)
{
	uint32_t start_animation = 0, traffic_sprite_record = 0;
	const struct traffic_record *record = NULL;
	uint8_t passing_sound = 0, hit_sound = 0;
	uint16_t animation = 0, palette = 0, weight = 0;

	record = rom_pointer(current_object->traffic.record);
	traffic_sprite_record = record->sprite;
	animation = TRAFFIC_SPRITE_RECORD(traffic_sprite_record)->animation;
	traffic_sprite_record += 2u;
	start_animation = animation;
	current_object->draw_mode = 0xBu;
	palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
	traffic_sprite_record += 2u;
	current_object->palette = palette;
	current_object->scale_shift = 0xFFu;
	passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
	traffic_sprite_record++;
	current_object->traffic.passing_sound = passing_sound;
	hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
	traffic_sprite_record++;
	current_object->traffic.hit_sound = hit_sound;
	weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
	current_object->traffic.weight = weight;
	current_object->traffic.hit_box = traffic_sprite_record + 2u;
	current_object->side = (uint8_t)record->side;
	AS32(current_object->traffic.x_offset) = record->x_offset;
	current_object->traffic.speed = record->speed;
	current_object->traffic.passed = 0u;
	current_object->state++;
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
void traffic_car_draw(uint16_t position_fraction)
{
	uint32_t zero = 0, negative = 0, carry = 0;
	uint8_t passing_sound = 0, row = 0;
	uint16_t height_difference = 0, object_height = 0, position = 0, screen_x_quarter = 0, seen_from = 0, segment = 0, window_segment = 0;
	uint32_t frame = 0;
	struct race *race = NULL;
	struct road_segment *car_segment_record = NULL, *segment_record = NULL;
	uint32_t saved_position = 0, saved_seen_from = 0;
	uint8_t raw_row = 0;

	current_object->position_fraction = position_fraction;
	position = (uint16_t)current_object->position;
	zero = traffic_out_of_view(position);
	if (zero)
	{
		screen->traffic_count--;
		current_object->flags = 0u;
		return;
	}
	race = pointer_to(screen->race_copy);
	seen_from = (uint16_t)(position - screen->traffic_draw_distance);
	segment = race->segment;
	if (current_object->traffic.passed != 0u)
	{
		if ((int16_t)segment < (int16_t)position)
		{
			current_object->traffic.passed = ~current_object->traffic.passed;
			saved_position = position;
			saved_seen_from = seen_from;
			passing_sound = current_object->traffic.passing_sound;
			request_sound(passing_sound);
			seen_from = (uint16_t)saved_seen_from;
			position = (uint16_t)saved_position;
		}
	}
	else if ((int16_t)position < (int16_t)segment)
	{
		current_object->traffic.passed = ~current_object->traffic.passed;
		saved_position = position;
		saved_seen_from = seen_from;
		passing_sound = current_object->traffic.passing_sound;
		request_sound(passing_sound);
		seen_from = (uint16_t)saved_seen_from;
		position = (uint16_t)saved_position;
	}
	window_segment = race->window_segment;
	if ((int16_t)position < (int16_t)window_segment || (int16_t)window_segment <= (int16_t)seen_from)
		return;
	car_segment_record = pointer_to(address_of(screen) + 0x2000u + player_car->segment_offset);
	frame = car_segment_record->heading;
	height_difference = car_segment_record->pitch;
	segment_record = pointer_to(address_of(screen) + 0x2000u + (((uint32_t)(uint16_t)current_object->position << 5) & 0xFFFu));
	object_height = segment_record->pitch;
	frame = (uint16_t)((int16_t)((frame << 4) - ((uint32_t)segment_record->heading << 4)) >> 6);
	screen_x_quarter = (uint16_t)((int16_t)current_object->traffic.screen_x >> 2);
	carry = (uint16_t)(frame + screen_x_quarter) < frame;
	frame = (uint8_t)(frame + screen_x_quarter + 0x10u + carry);
	if ((int8_t)frame >= 4)
	{
		if ((int8_t)frame > 0x1B)
			frame = 0x1Bu;
	}
	else
		frame = 4u;
	height_difference = (uint16_t)(((uint32_t)height_difference << 4) - ((uint32_t)object_height << 4) + ((int16_t)((uint32_t)player_car->view_pitch << 4) >> 2));
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
	frame = (uint8_t)(frame + ((uint32_t)row << 5));
	if (current_object->traffic.shake != 0u)
	{
		frame = (uint8_t)(frame + traffic_shake_frame_offsets[current_object->traffic.shake]);
		current_object->traffic.shake++;
		if (current_object->traffic.shake == 0x15u)
			current_object->traffic.shake = 0u;
	}
	current_object->frame = frame;
	negative = traffic_hit_check();
	if (negative)
	{
		traffic_start_hit_sound();
		current_object->traffic.shake = 1u;
	}
	draw_object(frame);
}

/* A vehicle in a line of road traffic behind a 0138EB leader (traffic_car_start_convoy starts the line
   from the leader's list: each one the list's spacing behind the one before it, with its own record):
   moves and draws as the leader's family does (traffic_car_update), but lives exactly as long as the one
   before it (leader): when that one has ended it ends too (screen traffic_count, the count of the
   traffic, less 1). The only line in the road's lists is in list 25 of stage_road_object_lists (a leader
   of animation 35C and eight of 35D); the scenarios start it only for a frame or two. */
void traffic_follower_update(void)
{
	if (OBJECT(current_object->traffic_follower.leader)->flags == 0u)
	{
		screen->traffic_count--;
		current_object->flags = 0u;
		return;
	}
	traffic_car_update();
	if (current_object->flags == 0u)
	{
		current_object->flags = 1u;
		screen->traffic_count++;
	}
}

/* The vehicle a road animal pulls (traffic_figure_start_team starts it behind a 013BA3 leader from the
   leader's list): the covered wagons behind the horses (list 1 of stage_road_object_lists), the sleighs
   behind the reindeer (lists 25 and 26). Moves and draws as the 013AF9 traffic does
   (traffic_stopping_car_update), and lives exactly as long as the one before it (leader): when that one
   has ended it ends too (screen traffic_count less 1). */
void pulled_vehicle_update(void)
{
	if (OBJECT(current_object->pulled_vehicle.leader)->flags == 0u)
	{
		screen->traffic_count--;
		current_object->flags = 0u;
		return;
	}
	traffic_stopping_car_update();
	if (current_object->flags == 0u)
	{
		current_object->flags = 1u;
		screen->traffic_count++;
	}
}

/* A traffic car that slows to a stop: as traffic_car_update, but at each segment it counts stop_distance
   down and, once it has run out (stopping), its speed drops by one a segment to 0. The cars of stages 2
   and 12, and what the traffic_figure teams pull (013AD8 runs it). */
void traffic_stopping_car_update(void)
{
	uint16_t position_fraction = 0, whole_segment = 0;
	uint16_t stop_distance = 0, speed = 0;

	switch (current_object->state)
	{
	case 0:
		traffic_car_start();
		return;
	case 1:
		break;
	}
	if (player_car->at_goal != 0u && (int16_t)(current_object->position - 8u) < (int16_t)RACE(screen->race_copy)->window_segment)
	{
		screen->traffic_count--;
		current_object->flags = 0u;
		return;
	}
	position_fraction = (uint16_t)(current_object->position_fraction + current_object->traffic.speed);
	whole_segment = 0x80u;
	if ((int16_t)position_fraction < 0x80)
	{
		traffic_car_draw(position_fraction);
		return;
	}
	current_object->position++;
	if (current_object->traffic.stopping == 0u)
	{
		stop_distance = current_object->traffic.stop_distance;
		current_object->traffic.stop_distance = stop_distance - 1u;
		if (stop_distance == 1u)
			current_object->traffic.stopping = 1u;
	}
	else
	{
		speed = current_object->traffic.speed;
		current_object->traffic.speed = speed - 1u;
		if ((int16_t)speed <= 1)
			current_object->traffic.speed = 0;
	}
	for (;;)
	{
		position_fraction -= whole_segment;
		if ((int16_t)position_fraction < (int16_t)whole_segment)
			break;
		current_object->position++;
	}
	traffic_car_draw(position_fraction);
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
	uint32_t line = 0;
	struct object *leader = NULL, *object = NULL;

	line = current_object->traffic.record;
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
		object->position = leader->position - spacing;
		object->handler = pulled_vehicle_update_ADDRESS;
		object->pulled_vehicle.stop_distance = leader->traffic.stop_distance;
		object->pulled_vehicle.record = r12;
		object->pulled_vehicle.leader = address_of(leader);
		leader = object;
		screen->traffic_count++;
	}
	current_object->traffic.record = line;
	current_object->handler = traffic_figure_update_ADDRESS;
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
	uint16_t position = 0, position_fraction = 0, seen_from = 0, whole_segment = 0;
	uint32_t animation = 0, traffic_sprite_record = 0;
	struct race *race = NULL;
	const struct traffic_record *record = NULL;
	uint16_t stop_distance = 0, speed = 0;

	switch (current_object->state)
	{
	case 0:
		record = rom_pointer(current_object->traffic.record);
		traffic_sprite_record = record->sprite;
		animation = TRAFFIC_SPRITE_RECORD(traffic_sprite_record)->animation;
		traffic_sprite_record += 2u;
		if (animation != SWORDFISH)
		{
			if (animation == ELEPHANT || animation == CYCLIST)
				current_object->scale_shift = 0u;
			else
				current_object->scale_shift = 1u;
			current_object->palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
			current_object->draw_mode = 5u;
			traffic_sprite_record += 2u;
			current_object->traffic.passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
			traffic_sprite_record++;
			current_object->traffic.hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
			traffic_sprite_record++;
			current_object->traffic.weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
			current_object->traffic.hit_box = traffic_sprite_record + 2u;
			current_object->side = (uint8_t)record->side;
			AS32(current_object->traffic.x_offset) = record->x_offset;
			current_object->traffic.speed = record->speed;
			current_object->state++;
			animation_start(animation);
			return;
		}
		current_object->scale_shift = 2u;
		current_object->palette = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x2u)->palette;
		current_object->draw_mode = 4u;
		traffic_sprite_record += 2u;
		current_object->traffic.passing_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x4u)->passing_sound;
		traffic_sprite_record++;
		current_object->traffic.hit_sound = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x5u)->hit_sound;
		traffic_sprite_record++;
		current_object->traffic.weight = TRAFFIC_SPRITE_RECORD(traffic_sprite_record - 0x6u)->weight;
		current_object->traffic.hit_box = traffic_sprite_record + 2u;
		current_object->side = (uint8_t)record->side;
		AS32(current_object->traffic.x_offset) = record->x_offset;
		current_object->traffic.speed = record->speed;
		AS32(current_object->traffic.y_offset) = (int16_t)0xFD40u;
		current_object->state++;
		animation_start(animation);
		return;
	case 1:
		if (player_car->at_goal != 0u && (int16_t)(current_object->position - 8u) < (int16_t)RACE(screen->race_copy)->window_segment)
		{
			screen->traffic_count--;
			current_object->flags = 0u;
			return;
		}
		position_fraction = (uint16_t)(current_object->position_fraction + current_object->traffic.speed);
		whole_segment = 0x80u;
		if ((int16_t)position_fraction >= 0x80)
		{
			do
			{
				current_object->position++;
				if (current_object->traffic.stopping == 0u)
				{
					stop_distance = current_object->traffic.stop_distance;
					current_object->traffic.stop_distance = stop_distance - 1u;
					if (stop_distance == 1u)
						current_object->traffic.stopping = 1u;
				}
				else
				{
					speed = current_object->traffic.speed;
					current_object->traffic.speed = speed - 1u;
					if ((int16_t)speed <= 1)
						current_object->traffic.speed = 0;
				}
				position_fraction -= whole_segment;
			} while ((int16_t)whole_segment <= (int16_t)position_fraction);
		}
		current_object->position_fraction = position_fraction;
		position = (uint16_t)current_object->position;
		zero = traffic_out_of_view(position);
		if (zero)
		{
			screen->traffic_count--;
			current_object->flags = 0u;
			return;
		}
		race = pointer_to(screen->race_copy);
		seen_from = (uint16_t)(position - screen->traffic_draw_distance);
		if ((int16_t)position < (int16_t)race->window_segment || (int16_t)race->window_segment <= (int16_t)seen_from)
			return;
		if ((ITEM_HIT_BOX(current_object->traffic.hit_box)->along_start | ITEM_HIT_BOX(current_object->traffic.hit_box)->along_end) != 0u)
		{
			negative = traffic_hit_check();
			if (negative)
				traffic_start_hit_sound();
		}
		draw_object(position);
		animation_advance(0u);
		return;
	}
}

/* When the traffic object has a hit_sound, starts an object (013D15) that plays it 15 frames later. */
void traffic_start_hit_sound(void)
{
	struct object *object = NULL;

	if (current_object->traffic.hit_sound != 0u)
	{
		object = start_object();
		object->handler = traffic_hit_sound_wait_ADDRESS;
		object->traffic_hit_sound.delay = 0xFu;
		object->traffic_hit_sound.sound = current_object->traffic.hit_sound;
	}
}

/* Plays a road object's own sound 15 frames after the car has hit it (traffic_start_hit_sound starts it
   with the sound of the object's record): counts delay down, then requests sound (request_sound) and
   ends. */
void traffic_hit_sound_wait(void)
{
	uint8_t sound = 0;
	uint8_t delay = 0;

	delay = current_object->traffic_hit_sound.delay;
	current_object->traffic_hit_sound.delay = delay - 1u;
	if (delay == 1u)
	{
		current_object->flags = 0u;
		sound = current_object->traffic_hit_sound.sound;
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

	stretch = address_of(screen) + 0xF84u;
	stretches_left = SCREEN(stretch - 0xF84u)->visible_stretch_count;
	stretch += 2u;
	do
	{
		stretch_top = STRETCH_ENDS(stretch)[0];
		stretch += 2u;
		if (stretch_top < position)
			break;
		stretch_bottom = STRETCH_ENDS(stretch)[0];
		stretch += 2u;
		if (stretch_bottom < position)
		{
			zero = 1u == 0u;
			return zero;
		}
		stretches_left--;
	} while (stretches_left != 0u);
	zero = 0u == 0u;
	return zero;
}
