/**
 *    Name:        race/race_cars.c
 *    Description: the cars of the race drawn as the screen's view sees them: the body, the spray, the pieces
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the race's cars: for each player of the race, a race_car with its race record and the tables of
   its model (body, driver_places, passenger_places, sprays, tag), the two race_car_occupant objects and
   two rider_gesture objects that follow it; then ends. */
void race_cars_start(void)
{
	uint8_t rider = 0;
	uint32_t car_number = 0, cars_left = 0, driver_occupant = 0, model = 0, player = 0, race = 0, rider_places = 0;
	struct object *object = NULL, *occupant = NULL;
	struct race *record = NULL;

	race = screen->host_record + 0x1Fu;
	cars_left = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		object = start_object();
		player = RACE_ORDER_PLAYERS(race)[0];
		object->handler = race_car_draw_ADDRESS;
		object->race_car.player = player;
		record = pointer_to(race_copy_offsets[player] + GAME_VARIABLES);
		object->race_car.record = address_of(record);
		model = record->model;
		object->race_car.model = model;
		object->race_car.first_body_piece = race_car_first_body_pieces[model];
		object->race_car.first_body_animation = race_car_first_body_animations[model];
		object->race_car.driver_animation = race_car_driver_animations[model];
		object->race_car.driver_palette = race_car_driver_palettes[model];
		object->race_car.driver_frames = race_car_driver_frame_counts[model];
		object->race_car.driver_gesture_mask = race_car_driver_gesture_masks[model];
		object->race_car.passenger_animation = race_car_passenger_animations[model];
		object->race_car.passenger_palette = race_car_passenger_palettes[model];
		object->race_car.passenger_frames = race_car_passenger_frame_counts[model];
		object->race_car.passenger_gesture_mask = race_car_passenger_gesture_masks[model];
		rider_places = race_car_rider_places[model];
		object->race_car.driver_places = rider_places;
		object->race_car.passenger_places = rider_places + 4u;
		object->race_car.spray_2_animations = race_car_spray_2_animations_ADDRESS;
		object->race_car.spray_2_places = rider_places + 8u;
		object->race_car.spray_2_effect = address_of(record) + 0x52u;
		object->race_car.spray_2_side = 2u;
		object->race_car.spray_1_animations = race_car_spray_1_animations_ADDRESS;
		object->race_car.spray_1_places = rider_places + 0xCu;
		object->race_car.spray_1_effect = address_of(record) + 0x51u;
		object->race_car.spray_1_side = 1u;
		car_number = record->car_number;
		object->race_car.tag_animation = race_car_tag_animations[car_number];
		object->race_car.tag_palette = race_car_tag_palettes[model];
		object->race_car.blink_counter = race_car_blink_starts[car_number];
		object->race_car.rival = car_number - RACE(screen->race_copy)->car_number;
		r12 = LINKED(r12, (uint16_t)address_of(object));
		driver_occupant = model;
		race++;
		occupant = start_object();
		occupant->handler = race_car_driver_handlers[driver_occupant];
		occupant->race_car_occupant.car = r12;
		occupant->race_car_occupant.occupant = driver_occupant;
		occupant->race_car_occupant.body_back = 0xFFu;
		occupant = start_object();
		occupant->handler = race_car_passenger_handlers[driver_occupant];
		occupant->race_car_occupant.car = r12;
		occupant->race_car_occupant.occupant = driver_occupant + 8u;
		occupant->race_car_occupant.body_back = 0u;
		rider = (uint8_t)model;
		object = start_object();
		object->handler = rider_gesture_update_ADDRESS;
		object->rider_gesture.car = r12;
		object->rider_gesture.rider = rider;
		object->rider_gesture.model = model;
		object->rider_gesture.player = OBJECT(r12)->race_car.player;
		object = start_object();
		object->handler = rider_gesture_update_ADDRESS;
		object->rider_gesture.car = r12;
		object->rider_gesture.rider = rider + 8u;
		object->rider_gesture.model = model;
		object->rider_gesture.player = OBJECT(r12)->race_car.player;
		cars_left--;
	} while (cars_left != 0u);
	current_object->flags = 0u;
	current_object->flags_high = 0u;
}

/* Draws a car of the race, the screen's own or a rival, from its race record (record), as the screen's
   view sees it: its world position less the view's through the view's matrix and scaled by its depth
   (off the screen, too near or too far: not drawn); its body (animation, frame, palette from the record,
   plus the screen's brake lights and wheel_turn), its driver and passenger (driver_places,
   passenger_places), the sprays at its wheels (race_car_draw_spray), five frames of animation 44 on its
   hard acceleration (record hard_acceleration, car hard_acceleration); over the screen's own car the
   CRASH (or Ouch!) text after a hit, over a rival its number tag, blinking. A rival behind the window of
   the road, or off the screen, is race_car_draw_behind's. Its sprites go into the list of their depth
   layer (race_car_draw_link). A body animation without wheels (race_car_body_wheel_kinds) sets
   special_body for the occupants. Once the race is over (screen flags bit 4) it becomes
   race_car_start_ending. */
void race_car_draw(void)
{
	uint8_t hard_acceleration_frame = 0, wheel_effect_sides = 0;
	uint16_t crash_text_y = 0, element = 0, far_segment = 0, other_spray = 0, segment = 0, sprite_x = 0, sprite_y = 0, window_segment = 0, x_offset = 0, y_offset = 0;
	uint32_t animation = 0, blink_counter = 0, body_animation = 0, body_attributes = 0, car = 0, depth = 0, depth_layer = 0, ignored = 0, palette = 0, piece = 0, relative_y = 0, rider_places = 0, scale = 0, screen_x = 0, screen_y = 0, second_rider_frame = 0, spray = 0, sprite_entry = 0, view = 0, view_offset = 0, x_term = 0, y_term = 0;
	struct race *race = NULL;
	struct race_car_rider *first_rider = NULL, *second_rider = NULL;
	uint16_t swapped_rider = 0, swapped_spray = 0;

	current_object->race_car.blink_counter++;
	sprite_entry = screen->sprite_list_last + 0x10u;
	if (screen->sprite_list_limit <= sprite_entry)
		return;
	window_segment = player_car->window_segment;
	far_segment = (uint16_t)(window_segment + (screen->draw_distance >> 7));
	race = pointer_to(current_object->race_car.record);
	segment = race->segment;
	if (current_object->race_car.rival == 0u)
	{
		if ((screen->flags & 0x10u) != 0u)
			current_object->handler = race_car_ending_handlers[current_object->race_car.model];
		else if (far_segment <= segment)
			return;
	}
	else
	{
		if (segment <= window_segment)
		{
			race_car_draw_behind(sprite_entry, race, &r12, &r14);
			return;
		}
		if (far_segment <= segment)
			return;
	}
	car = address_of(player_car) + 0xDCu;
	r12 = (uint32_t)race->world_x - player_car->view_x;
	relative_y = (uint32_t)race->world_y - player_car->view_y;
	r14 = (uint32_t)race->world_z - player_car->view_z;
	x_term = r12 * CAR(car - 0xDCu)->view_matrix_xx;
	car += 4u;
	y_term = relative_y * CAR(car - 0xE0u)->view_matrix_xy;
	car += 4u;
	screen_x = (int32_t)(x_term + r14 * CAR(car - 0xE4u)->view_matrix_xz + y_term) >> 14;
	car += 4u;
	x_term = r12 * CAR(car - 0xE8u)->view_matrix_yx;
	car += 4u;
	y_term = relative_y * CAR(car - 0xECu)->view_matrix_yy;
	car += 4u;
	screen_y = (int32_t)(x_term + r14 * CAR(car - 0xF0u)->view_matrix_yz + y_term) >> 14;
	depth = (int32_t)(r12 * CAR(car - 0xF0u)->view_matrix_zx + r14 * CAR(car - 0xF0u)->view_matrix_zz + relative_y * CAR(car - 0xF0u)->view_matrix_zy) >> 14;
	if (depth <= 0x40u || depth >= 0x3000u)
	{
		if (current_object->race_car.rival != 0u)
		{
			race_car_draw_behind(sprite_entry, race, &r12, &r14);
			return;
		}
		depth_layer = 0x1C0u;
		scale = 0x100u;
		screen_x = 0u;
		screen_y = 0u;
	}
	else
	{
		depth_layer = depth_layers_by_depth[depth];
		scale = perspective_scales[depth];
		screen_x = (int32_t)(screen_x * scale) >> 12;
		screen_y = 0u - ((int32_t)(screen_y * scale) >> 12);
		if ((int32_t)screen_x > 0x120 || (int32_t)screen_x < -0x120 || (int32_t)screen_y > 0xF0)
		{
			if (current_object->race_car.rival != 0u)
			{
				race_car_draw_behind(sprite_entry, race, &r12, &r14);
				return;
			}
			depth_layer = 0x1C0u;
			scale = 0x100u;
			screen_x = 0u;
			screen_y = 0u;
		}
		else
			scale = (uint16_t)((int16_t)scale >> 4);
	}
	current_object->race_car.wheel_turn += race->speed;
	palette = (uint8_t)(current_object->race_car.wheel_turn >> 16u);
	palette = palette & 1u;
	if ((uint8_t)(current_object->race_car.wheel_turn >> 24u) != palette)
	{
		current_object->race_car.wheel_turn = (current_object->race_car.wheel_turn & 0xFFFFFFu) | ((palette & 0xFFu) << 24u);
		current_object->race_car.wheel_turn &= 0xFFFF0000u;
	}
	palette = (uint16_t)(palette + AS16(player_car->brake_lights));
	palette = (uint16_t)(palette + race->palette);
	r12 = race->animation;
	r14 = race->frame;
	sprite_x = (uint16_t)screen_x;
	sprite_y = (uint16_t)screen_y;
	current_object->x = screen_x;
	current_object->y = screen_y;
	current_object->width_scale = scale;
	current_object->depth_layer = depth_layer;
	race_car_put_frame(scale, sprite_x, sprite_y, &body_attributes, &sprite_entry, &r12, &palette, &r14);
	current_object->race_car.body_attributes = body_attributes;
	if (current_object->race_car.special_body == 0u)
	{
		animation = race->animation;
		body_animation = (uint16_t)(animation - current_object->race_car.first_body_animation);
		if (race_car_body_wheel_kinds[body_animation] != 0u)
		{
			player_car->crash_body_shown = 0u;
			first_rider = pointer_to(address_of(current_object) + 0x90u);
			second_rider = pointer_to(address_of(current_object) + 0xA0u);
			if ((current_object->race_car.model != 6u || race_car_body_wheel_kinds[body_animation] == 2u) && (body_attributes & 0x40u) == 0u)
			{
				swapped_rider = address_of(second_rider);
				second_rider = pointer_to(LINKED(address_of(second_rider), (uint16_t)address_of(first_rider)));
				first_rider = pointer_to(LINKED(address_of(first_rider), swapped_rider));
			}
			if (race->speed == 0u)
			{
				r14 = 0u;
				second_rider_frame = 0u;
			}
			else
			{
				r14 = first_rider->frame;
				second_rider_frame = second_rider->frame;
				if (first_rider->frames <= r14)
					r14 = 1u;
				else
					r14 = (uint8_t)(r14 + 1u);
				if (second_rider->frames <= second_rider_frame)
					second_rider_frame = 1u;
				else
					second_rider_frame = (uint8_t)(second_rider_frame + 1u);
			}
			first_rider->frame = r14;
			second_rider->frame = second_rider_frame;
			piece = SPRITE_ANIMATION(sprite_animations[animation])->frames[race->frame] + 2u;
			view = SPRITE_PIECE(piece)->attributes;
			piece += 2u;
			view = view & 0x40u;
			view = view >> 6;
			view = (uint16_t)(view * 0x34u);
			view = (uint16_t)(view + (uint16_t)(SPRITE_PIECE(piece - 0x2u)->sprite - current_object->race_car.first_body_piece) / 3u);
			current_object->race_car.view = view;
			view_offset = (uint16_t)(view << 4);
			piece += 2u;
			y_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->y_offset;
			x_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->x_offset;
			r12 = (r12 & ~0xFFu) | current_object->race_car.gesture_shown;
			if ((r12 & first_rider->gesture_mask) == 0u)
			{
				rider_places = first_rider->places;
				palette = first_rider->palette;
				r12 = first_rider->animation + view;
				sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
			}
			r12 = (r12 & ~0xFFu) | current_object->race_car.gesture_shown;
			if ((r12 & second_rider->gesture_mask) == 0u)
			{
				r14 = second_rider_frame;
				rider_places = second_rider->places;
				palette = second_rider->palette;
				r12 = second_rider->animation + view;
				sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
			}
			spray = address_of(current_object) + 0xC0u;
			other_spray = (uint16_t)(address_of(current_object) + 0xD0u);
			if ((current_object->race_car.body_attributes & 0x40u) == 0u)
			{
				swapped_spray = other_spray;
				other_spray = (uint16_t)spray;
				spray = (spray & ~0xFFFFu) | swapped_spray;
			}
			wheel_effect_sides = race->wheel_effect_sides;
			sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y, pointer_to(spray));
			spray = (spray & ~0xFFFFu) | (uint16_t)other_spray;
			sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y, pointer_to(spray));
			hard_acceleration_frame = current_object->race_car.hard_acceleration_frame;
			if (hard_acceleration_frame != 0u || race->hard_acceleration != 0u)
			{
				r14 = hard_acceleration_frame;
				hard_acceleration_frame++;
				if (hard_acceleration_frame >= 5u)
					hard_acceleration_frame = 0u;
				current_object->race_car.hard_acceleration_frame = hard_acceleration_frame;
				r12 = ACCELERATION_SMOKE;
				palette = 0x58u;
				sprite_x = (uint16_t)screen_x;
				sprite_y = (uint16_t)screen_y;
				race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &r12, &palette, &r14);
				race_car_put_sprite(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &r12, &palette, &r14);
			}
		}
		else
		{
			current_object->race_car.special_body = 0xFFu;
			if (current_object->race_car.rival == 0u)
				player_car->crash_body_shown = 0xFFu;
		}
	}
	if (screen->race_started == 0u && OBJECT(player_car->object)->player_car.timer >= 0x3Cu)
	{
		if (current_object->race_car.rival != 0u)
		{
			if (current_object->race_car.crash_text_frames == 0u)
			{
				if (player_car->crash_text == 0u)
				{
					race_car_draw_link(depth_layer, sprite_entry);
					return;
				}
				player_car->crash_text = 0u;
				current_object->race_car.crash_text_rise = 0u;
				current_object->race_car.crash_text_frames = 0x1Eu;
				current_object->race_car.crash_text_y = screen_y;
				current_object->race_car.crash_text_animation = CRASH_TEXT;
				current_object->race_car.crash_text_palette = 0x4Bu;
				if (player_car->crash_sound == 0u)
				{
					current_object->race_car.crash_text_animation = 0x30u;
					current_object->race_car.crash_text_palette = 0x4Cu;
				}
			}
			current_object->race_car.crash_text_frames--;
			r12 = current_object->race_car.crash_text_animation;
			palette = current_object->race_car.crash_text_palette;
			r14 = current_object->race_car.crash_text_rise;
			r14 = r14 >> 1;
			sprite_x = (uint16_t)screen_x;
			crash_text_y = (uint16_t)current_object->race_car.crash_text_y;
			scale = 0x100u;
			race_car_put_frame(scale, sprite_x, crash_text_y, &ignored, &sprite_entry, &r12, &palette, &r14);
			current_object->race_car.crash_text_rise++;
			current_object->race_car.crash_text_rise &= 7u;
			race_car_draw_link(depth_layer, sprite_entry);
			return;
		}
		element = SPRITE_PALETTE_WORDS(sprite_palettes[current_object->race_car.tag_palette])[0];
		r12 = 0x1Bu;
		sprite_x = (uint16_t)(screen_x - 3u);
		sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
		race_car_put_flat_sprite(element, sprite_x, sprite_y, &sprite_entry, &r12, &r14);
		r12 = current_object->race_car.tag_animation;
		sprite_y -= 0x16u;
		sprite_x += 0x15u;
		race_car_put_flat_sprite(element, sprite_x, sprite_y, &sprite_entry, &r12, &r14);
		race_car_draw_link(depth_layer, sprite_entry);
		return;
	}
	if (current_object->race_car.rival == 0u)
	{
		if (current_object->race_car.crash_text_frames == 0u)
		{
			if (player_car->crash_text == 0u)
			{
				race_car_draw_link(depth_layer, sprite_entry);
				return;
			}
			player_car->crash_text = 0u;
			current_object->race_car.crash_text_rise = 0u;
			current_object->race_car.crash_text_frames = 0x1Eu;
			current_object->race_car.crash_text_y = screen_y;
			current_object->race_car.crash_text_animation = CRASH_TEXT;
			current_object->race_car.crash_text_palette = 0x4Bu;
			if (player_car->crash_sound == 0u)
			{
				current_object->race_car.crash_text_animation = 0x30u;
				current_object->race_car.crash_text_palette = 0x4Cu;
			}
		}
		current_object->race_car.crash_text_frames--;
		r12 = current_object->race_car.crash_text_animation;
		palette = current_object->race_car.crash_text_palette;
		r14 = current_object->race_car.crash_text_rise;
		r14 = r14 >> 1;
		sprite_x = (uint16_t)screen_x;
		crash_text_y = (uint16_t)current_object->race_car.crash_text_y;
		scale = 0x100u;
		race_car_put_frame(scale, sprite_x, crash_text_y, &ignored, &sprite_entry, &r12, &palette, &r14);
		current_object->race_car.crash_text_rise++;
		current_object->race_car.crash_text_rise &= 7u;
		race_car_draw_link(depth_layer, sprite_entry);
		return;
	}
	blink_counter = current_object->race_car.blink_counter;
	if (race_car_tag_blink[blink_counter & 0x1Fu] == 0u)
	{
		race_car_draw_link(depth_layer, sprite_entry);
		return;
	}
	element = SPRITE_PALETTE_WORDS(sprite_palettes[current_object->race_car.tag_palette])[0];
	r12 = 0x1Bu;
	sprite_x = (uint16_t)(screen_x - 3u);
	sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
	race_car_put_flat_sprite(element, sprite_x, sprite_y, &sprite_entry, &r12, &r14);
	r12 = current_object->race_car.tag_animation;
	sprite_y -= 0x16u;
	sprite_x += 0x15u;
	race_car_put_flat_sprite(element, sprite_x, sprite_y, &sprite_entry, &r12, &r14);
	race_car_draw_link(depth_layer, sprite_entry);
}

/* A rival behind the screen's car, or off the screen (race_car_draw goes on to it): within 256 segments
   behind, small at the bottom of the screen, larger as it closes in (depth layer 191), across as it is
   on the road (the difference of road_x, at most 58 either way) and up by its bounce; its car and its
   number tag (tag_animation, tag_palette), into the list of that depth layer (race_car_draw_link), with
   special_body and gesture_shown cleared. */
void race_car_draw_behind(uint32_t sprite_entry, struct race *race, uint32_t *p_last_piece, uint32_t *p_last_size_step)
{
	uint16_t sprite_x = 0, sprite_y = 0;
	uint32_t animation = 0, depth_layer = 0, distance_behind = 0, frame = 0, ignored = 0, palette = 0, scale = 0;
	struct race *own_race = NULL;
	uint16_t past_limit = 0;

	current_object->race_car.special_body = 0u;
	current_object->race_car.gesture_shown = 0u;
	own_race = pointer_to(screen->race_copy);
	distance_behind = own_race->segment;
	distance_behind = (uint16_t)(distance_behind - race->segment);
	past_limit = distance_behind - 0x100u;
	distance_behind = past_limit;
	if ((int16_t)past_limit >= 0)
		return;
	distance_behind = (uint16_t)(0u - distance_behind);
	depth_layer = 0x191u;
	sprite_x = (uint16_t)((int16_t)((race->road_x - own_race->road_x) * 3u) >> 2);
	if ((int16_t)sprite_x <= 0x58)
	{
		if ((int16_t)sprite_x < -0x58)
			sprite_x = 0xFFA8u;
	}
	else
		sprite_x = 0x58u;
	sprite_y = (uint16_t)((int16_t)(0u - race->bounce_height) >> 2);
	scale = distance_behind * 3u;
	scale = (scale & ~0xFFFFu) | ((uint16_t)scale >> 3);
	animation = race->animation;
	palette = race->palette;
	frame = race->frame;
	race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &animation, &palette, &frame);
	sprite_y = (uint16_t)((int16_t)(distance_behind * 0xFFECu) >> 8);
	scale = distance_behind;
	*p_last_piece = current_object->race_car.tag_animation;
	palette = current_object->race_car.tag_palette;
	*p_last_size_step = 0u;
	race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, p_last_piece, &palette, p_last_size_step);
	race_car_draw_link(depth_layer, sprite_entry);
}

/* Puts the sprites a car has just written (from the screen's sprite_list_last on, up to sprite_entry) at
   the head of the list of its depth layer (screen + 5000 + 4 * depth_layer; its tail too when the list
   was empty), and moves sprite_list_last on to sprite_entry. */
void race_car_draw_link(uint32_t depth_layer, uint32_t sprite_entry)
{
	uint32_t group = 0, older_group = 0;
	struct depth_list *depth_list = NULL;

	depth_list = pointer_to(address_of(screen) + 0x5000u + depth_layer * 4u);
	if (depth_list->head == 0u)
		depth_list->tail = sprite_entry;
	group = screen->sprite_list_last + 0x10u;
	older_group = depth_list->head;
	AS32(SPRITE_WORDS(sprite_entry)[0]) = older_group;
	AS32(SPRITE_WORDS(sprite_entry)[6144]) = (uint16_t)(older_group + 0x300u);
	depth_list->head = (group >> 4) | 0x8000u;
	screen->sprite_list_last = sprite_entry;
}

/* Draws the spray at one side's wheels (dust, smoke, water...: the record's kind, from the race record's
   wheel effects), cycling its frames. */
uint32_t race_car_draw_spray(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint8_t wheel_effect_sides, uint32_t scale, uint32_t sprite_entry, uint16_t screen_x, uint16_t screen_y, struct race_car_spray *spray)
{
	uint32_t effect = 0, frame = 0, kind = 0, palette = 0, places = 0;

	frame = spray->frame;
	if (frame == 0u && (wheel_effect_sides & spray->side) == 0u)
	{
		spray->frame = 0u;
		spray->kind = 0u;
		return sprite_entry;
	}
	kind = spray->kind;
	effect = WHEEL_EFFECT_VALUES(spray->effect)[0];
	if (effect != 0u && effect != kind)
	{
		kind = effect;
		spray->kind = effect;
		frame = 0u;
	}
	r14 = frame;
	frame = (uint8_t)(frame + 1u);
	if (race_car_spray_frame_counts[kind] <= frame)
		frame = 0u;
	spray->frame = frame;
	places = spray->places;
	r12 = SPRAY_ANIMATION_NUMBERS(spray->animations)[(int32_t)kind];
	palette = race_car_spray_palettes[kind];
	return race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, places, palette, screen_x, screen_y);
}

/* Draws a part of the car (a wheel, a spray) at its offset from the car's place, scaled with the car. */
uint32_t race_car_draw_part(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint32_t scale, uint32_t sprite_entry, uint32_t places, uint32_t palette, uint16_t screen_x, uint16_t screen_y)
{
	uint32_t carry = 0;
	uint16_t sprite_x = 0, sprite_y = 0;
	uint32_t ignored = 0;

	sprite_x = (uint16_t)x_offset;
	sprite_y = (uint16_t)y_offset;
	places += view_offset;
	if ((CAR_PART_PLACE(places)->x | CAR_PART_PLACE(places)->y) == 0u)
		return sprite_entry;
	sprite_x = (uint16_t)((sprite_x + CAR_PART_PLACE(places)->x) * scale);
	sprite_y = (uint16_t)((sprite_y + CAR_PART_PLACE(places)->y) * scale);
	carry = (sprite_x >> 7) & 1u;
	sprite_x = (uint16_t)(((int16_t)sprite_x >> 8) + carry);
	carry = (sprite_y >> 7) & 1u;
	sprite_x += screen_x;
	sprite_y = (uint16_t)(((int16_t)sprite_y >> 8) + carry + screen_y);
	race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &r12, &palette, &r14);
	return sprite_entry;
}

/* Puts the first piece of frame R14 of animation R12 with palette R13 into the sprite list, scaled by R8
   at R17, R18 (race_car_put_sprite). */
void race_car_put_frame(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_palette, uint32_t *p_last_size_step)
{
	uint32_t piece_attributes = 0;

	*p_last_piece = SPRITE_ANIMATION(sprite_animations[(int32_t)*p_last_piece])->frames[(int32_t)*p_last_size_step] + 2u;
	*p_palette = sprite_palettes[*p_palette];
	race_car_put_sprite(scale, sprite_x, sprite_y, &piece_attributes, p_sprite_entry, p_last_piece, p_palette, p_last_size_step);
	*p_attributes = (*p_attributes & 0xFFFF0000u) | (piece_attributes & 0xFFFFu);
}

/* Writes one sprite of a frame's piece into the screen's sprite list (and its copy at + 3000), its size
   and offsets scaled by R8, at R17, R18. */
void race_car_put_sprite(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_palette, uint32_t *p_last_size_step)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint32_t last_piece = *p_last_piece;
	uint16_t height_and_bits = 0, palette_word = 0, piece_attributes = 0, piece_x = 0, piece_y = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint32_t size_step = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0, element = 0;

	piece_attributes = SPRITE_PIECE(last_piece)->attributes;
	SPRITE_WORDS(sprite_entry)[6144] = piece_attributes;
	SPRITE_WORDS(sprite_entry)[0] = piece_attributes;
	sprite_entry += 2u;
	last_piece += 2u;
	size_step = sprite_size_steps_ADDRESS + SPRITE_PIECE(last_piece - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(size_step)->source_size;
	size_step += 2u;
	sprite_source_size = source_size;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_source_size;
	SPRITE_WORDS(sprite_entry)[0] = sprite_source_size;
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(size_step - 0x2u)->rom_offset;
	size_step += 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(size_step - 0x4u)->height;
	size_step += 2u;
	height_bits = SPRITE_SIZE_STEP(size_step - 0x6u)->height_bits;
	size_step += 2u;
	height_and_bits = (((height * scale) >> 8) & 0x3FFu) | height_bits;
	SPRITE_WORDS(sprite_entry)[6144] = height_and_bits;
	SPRITE_WORDS(sprite_entry)[0] = height_and_bits;
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(size_step - 0x8u)->width;
	size_step += 2u;
	width_bits = SPRITE_SIZE_STEP(size_step - 0xAu)->width_bits;
	size_step += 2u;
	width_and_bits = (((width * scale) >> 8) & 0x3FFu) | width_bits | screen->sprite_screen_bit;
	SPRITE_WORDS(sprite_entry)[6144] = width_and_bits;
	SPRITE_WORDS(sprite_entry)[0] = width_and_bits;
	sprite_entry += 2u;
	last_piece += 2u;
	piece_y = (uint16_t)(((int32_t)(SPRITE_PIECE(last_piece - 0x4u)->y_offset * scale) >> 8) + sprite_y);
	SPRITE_WORDS(sprite_entry)[6144] = piece_y;
	SPRITE_WORDS(sprite_entry)[0] = piece_y;
	sprite_entry += 2u;
	last_piece += 2u;
	piece_x = (uint16_t)(((int32_t)(SPRITE_PIECE(last_piece - 0x6u)->x_offset * scale) >> 8) + sprite_x);
	SPRITE_WORDS(sprite_entry)[6144] = piece_x;
	SPRITE_WORDS(sprite_entry)[0] = piece_x;
	sprite_entry += 2u;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_rom_offset;
	SPRITE_WORDS(sprite_entry)[0] = sprite_rom_offset;
	sprite_entry += 2u;
	element = SPRITE_PALETTE_WORDS(*p_palette)[0];
	*p_palette += 2u;
	palette_word = element;
	SPRITE_WORDS(sprite_entry)[6144] = palette_word;
	SPRITE_WORDS(sprite_entry)[0] = palette_word;
	sprite_entry += 2u;
	screen->sprite_groups++;
	last_piece += 2u;
	*p_attributes = (*p_attributes & 0xFFFF0000u) | piece_attributes;
	*p_sprite_entry = sprite_entry;
	*p_last_piece = last_piece;
	*p_last_size_step = size_step;
}

/* Writes the first piece of animation R12 into the sprite list at R17, R18 at its own size (the number
   tags). */
void race_car_put_flat_sprite(uint16_t palette_word, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_last_size_step)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint32_t last_piece = *p_last_piece;
	uint16_t attributes = 0, height_and_bits = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint32_t piece = 0, size_step = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0;

	piece = SPRITE_ANIMATION(sprite_animations[(int32_t)last_piece])->frames[0] + 2u;
	attributes = SPRITE_PIECE(piece)->attributes;
	SPRITE_WORDS(sprite_entry)[6144] = attributes;
	SPRITE_WORDS(sprite_entry)[0] = attributes;
	sprite_entry += 2u;
	piece += 2u;
	size_step = sprite_size_steps_ADDRESS + SPRITE_PIECE(piece - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(size_step)->source_size;
	size_step += 2u;
	sprite_source_size = source_size;
	SPRITE_WORDS(sprite_entry)[6144] = sprite_source_size;
	SPRITE_WORDS(sprite_entry)[0] = sprite_source_size;
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(size_step - 0x2u)->rom_offset;
	size_step += 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(size_step - 0x4u)->height;
	size_step += 2u;
	height_bits = SPRITE_SIZE_STEP(size_step - 0x6u)->height_bits;
	size_step += 2u;
	height_and_bits = height | height_bits;
	SPRITE_WORDS(sprite_entry)[6144] = height_and_bits;
	SPRITE_WORDS(sprite_entry)[0] = height_and_bits;
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(size_step - 0x8u)->width;
	size_step += 2u;
	width_bits = SPRITE_SIZE_STEP(size_step - 0xAu)->width_bits;
	size_step += 2u;
	width_and_bits = width | width_bits | screen->sprite_screen_bit;
	SPRITE_WORDS(sprite_entry)[6144] = width_and_bits;
	SPRITE_WORDS(sprite_entry)[0] = width_and_bits;
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
	SPRITE_WORDS(sprite_entry)[6144] = palette_word;
	SPRITE_WORDS(sprite_entry)[0] = palette_word;
	sprite_entry += 2u;
	screen->sprite_groups++;
	piece += 2u;
	*p_sprite_entry = sprite_entry;
	*p_last_piece = piece;
	*p_last_size_step = size_step;
}
