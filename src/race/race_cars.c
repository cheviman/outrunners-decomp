/**
 *    Name:        race/race_cars.c
 *    Description: the cars of the race drawn as the screen's view sees them: the body, the spray, the pieces
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
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
	uint32_t car_number = 0, cars_left = 0, driver_occupant = 0, model = 0, object = 0, occupant = 0, player = 0, race = 0, record = 0, rider_places = 0;

	race = SCREEN(screen)->host_record + 0x1Fu;
	cars_left = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		object = start_object();
		player = mem_read8(race);
		OBJECT(object)->handler = race_car_draw_ADDRESS;
		OBJECT(object)->race_car.player = player;
		record = race_copy_offsets[player] + GAME_VARIABLES;
		OBJECT(object)->race_car.record = record;
		model = RACE(record)->model;
		OBJECT(object)->race_car.model = model;
		OBJECT(object)->race_car.first_body_piece = race_car_first_body_pieces[model];
		OBJECT(object)->race_car.first_body_animation = race_car_first_body_animations[model];
		OBJECT(object)->race_car.driver_animation = race_car_driver_animations[model];
		OBJECT(object)->race_car.driver_palette = race_car_driver_palettes[model];
		OBJECT(object)->race_car.driver_frames = race_car_driver_frame_counts[model];
		OBJECT(object)->race_car.driver_gesture_mask = race_car_driver_gesture_masks[model];
		OBJECT(object)->race_car.passenger_animation = race_car_passenger_animations[model];
		OBJECT(object)->race_car.passenger_palette = race_car_passenger_palettes[model];
		OBJECT(object)->race_car.passenger_frames = race_car_passenger_frame_counts[model];
		OBJECT(object)->race_car.passenger_gesture_mask = race_car_passenger_gesture_masks[model];
		rider_places = race_car_rider_places[model];
		OBJECT(object)->race_car.driver_places = rider_places;
		OBJECT(object)->race_car.passenger_places = rider_places + 4u;
		OBJECT(object)->race_car.spray_2_animations = race_car_spray_2_animations_ADDRESS;
		OBJECT(object)->race_car.spray_2_places = rider_places + 8u;
		OBJECT(object)->race_car.spray_2_effect = record + 0x52u;
		OBJECT(object)->race_car.spray_2_side = 2u;
		OBJECT(object)->race_car.spray_1_animations = race_car_spray_1_animations_ADDRESS;
		OBJECT(object)->race_car.spray_1_places = rider_places + 0xCu;
		OBJECT(object)->race_car.spray_1_effect = record + 0x51u;
		OBJECT(object)->race_car.spray_1_side = 1u;
		car_number = RACE(record)->car_number;
		OBJECT(object)->race_car.tag_animation = race_car_tag_animations[car_number];
		OBJECT(object)->race_car.tag_palette = race_car_tag_palettes[model];
		OBJECT(object)->race_car.blink_counter = race_car_blink_starts[car_number];
		OBJECT(object)->race_car.rival = car_number - RACE(SCREEN(screen)->race_copy)->car_number;
		r12 = (r12 & ~0xFFFFu) | (uint16_t)object;
		driver_occupant = model;
		race++;
		occupant = start_object();
		OBJECT(occupant)->handler = race_car_driver_handlers[driver_occupant];
		OBJECT(occupant)->race_car_occupant.car = r12;
		OBJECT(occupant)->race_car_occupant.occupant = driver_occupant;
		OBJECT(occupant)->race_car_occupant.body_back = 0xFFu;
		occupant = start_object();
		OBJECT(occupant)->handler = race_car_passenger_handlers[driver_occupant];
		OBJECT(occupant)->race_car_occupant.car = r12;
		OBJECT(occupant)->race_car_occupant.occupant = driver_occupant + 8u;
		OBJECT(occupant)->race_car_occupant.body_back = 0u;
		rider = (uint8_t)model;
		object = start_object();
		OBJECT(object)->handler = rider_gesture_update_ADDRESS;
		OBJECT(object)->rider_gesture.car = r12;
		OBJECT(object)->rider_gesture.rider = rider;
		OBJECT(object)->rider_gesture.model = model;
		OBJECT(object)->rider_gesture.player = mem_read8(r12 + 9u);
		object = start_object();
		OBJECT(object)->handler = rider_gesture_update_ADDRESS;
		OBJECT(object)->rider_gesture.car = r12;
		OBJECT(object)->rider_gesture.rider = rider + 8u;
		OBJECT(object)->rider_gesture.model = model;
		OBJECT(object)->rider_gesture.player = mem_read8(r12 + 9u);
		cars_left--;
	} while (cars_left != 0u);
	OBJECT(current_object)->flags = 0u;
	OBJECT(current_object)->flags_high = 0u;
}

/* Draws a car of the race, the screen's own or a rival, from its race record (record), as the screen's
   view sees it: its world position less the view's through the view's matrix and scaled by its depth
   (off the screen, too near or too far: not drawn); its body (animation, frame, palette from the record,
   plus the screen's brake lights and wheel_turn), its driver and passenger (driver_places,
   passenger_places), the sprays at its wheels (race_car_draw_spray), five frames of animation 44 on its
   hard acceleration (record hard_acceleration, car hard_acceleration); over the screen's own car the
   CRASH (or Ouch!) text after a hit, over a rival its number tag, blinking. A rival behind the window of
   the road (within 256 segments) is shown small at the bottom of the screen, larger as it closes in and
   across as it is (with its tag). A body animation without wheels (race_car_body_wheel_kinds) sets
   special_body for the occupants. Once the race is over (screen flags bit 4) it becomes
   race_car_start_ending. */
void race_car_draw(void)
{
	bool L_00978F = false, L_009874 = false, L_009AAD = false, L_009B31 = false;
	uint8_t hard_acceleration_frame = 0, wheel_effect_sides = 0;
	uint16_t crash_text_y = 0, element = 0, far_segment = 0, segment = 0, sprite_x = 0, sprite_y = 0, window_segment = 0, x_offset = 0, y_offset = 0;
	uint32_t animation = 0, blink_counter = 0, body_animation = 0, body_attributes = 0, car = 0, depth = 0, depth_layer = 0, depth_list = 0, distance_behind = 0, group = 0, ignored = 0, older_group = 0, own_race = 0, palette = 0, piece = 0, race = 0, relative_y = 0, rider_places = 0, scale = 0, screen_x = 0, screen_y = 0, second_rider_frame = 0, sprite_entry = 0, view = 0, view_offset = 0, x_term = 0, y_term = 0;
	uint16_t past_limit = 0, swapped_rider = 0, swapped_spray = 0;

	OBJECT(current_object)->race_car.blink_counter++;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
		return;
	window_segment = CAR(player_car)->window_segment;
	far_segment = (uint16_t)(window_segment + (SCREEN(screen)->draw_distance >> 7));
	race = OBJECT(current_object)->race_car.record;
	segment = RACE(race)->segment;
	if (OBJECT(current_object)->race_car.rival != 0u)
	{
		L_009B31 = segment <= window_segment;
		if (!L_009B31)
			L_00978F = true;
	}
	if (!L_009B31)
	{
		if (!L_00978F && (SCREEN(screen)->flags & 0x10u) != 0u)
			OBJECT(current_object)->handler = race_car_ending_handlers[OBJECT(current_object)->race_car.model];
		else if (far_segment <= segment)
			return;
		car = player_car + 0xDCu;
		r12 = (uint32_t)RACE(race)->world_x - CAR(player_car)->view_x;
		relative_y = (uint32_t)RACE(race)->world_y - CAR(player_car)->view_y;
		r14 = (uint32_t)RACE(race)->world_z - CAR(player_car)->view_z;
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
		if (depth > 0x40u && depth < 0x3000u)
		{
			depth_layer = depth_layers_by_depth[depth];
			scale = perspective_scales[depth];
			screen_x = (int32_t)(screen_x * scale) >> 12;
			screen_y = 0u - ((int32_t)(screen_y * scale) >> 12);
			L_009B31 = (int32_t)screen_x > 0x120 || (int32_t)screen_x < -0x120 || (int32_t)screen_y > 0xF0;
			if (!L_009B31)
			{
				scale = (uint16_t)((int16_t)scale >> 4);
				L_009874 = true;
			}
		}
	}
	if (!L_009874 && OBJECT(current_object)->race_car.rival != 0u)
	{
		OBJECT(current_object)->race_car.special_body = 0u;
		OBJECT(current_object)->race_car.gesture_shown = 0u;
		own_race = SCREEN(screen)->race_copy;
		distance_behind = RACE(own_race)->segment;
		distance_behind = (uint16_t)(distance_behind - RACE(race)->segment);
		past_limit = distance_behind - 0x100u;
		distance_behind = past_limit;
		if ((int16_t)past_limit >= 0)
			return;
		distance_behind = (uint16_t)(0u - distance_behind);
		depth_layer = 0x191u;
		sprite_x = (uint16_t)((int16_t)((RACE(race)->road_x - RACE(own_race)->road_x) * 3u) >> 2);
		if ((int16_t)sprite_x <= 0x58)
		{
			if ((int16_t)sprite_x < -0x58)
				sprite_x = 0xFFA8u;
		}
		else
			sprite_x = 0x58u;
		sprite_y = (uint16_t)((int16_t)(0u - RACE(race)->bounce_height) >> 2);
		scale = distance_behind * 3u;
		scale = (scale & ~0xFFFFu) | ((uint16_t)scale >> 3);
		r12 = RACE(race)->animation;
		palette = RACE(race)->palette;
		r14 = RACE(race)->frame;
		race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
		sprite_y = (uint16_t)((int16_t)(distance_behind * 0xFFECu) >> 8);
		scale = distance_behind;
		r12 = OBJECT(current_object)->race_car.tag_animation;
		palette = OBJECT(current_object)->race_car.tag_palette;
		r14 = 0u;
		race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
	}
	else
	{
		if (!L_009874)
		{
			depth_layer = 0x1C0u;
			scale = 0x100u;
			screen_x = 0u;
			screen_y = 0u;
		}
		OBJECT(current_object)->race_car.wheel_turn += RACE(race)->speed;
		palette = (uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 16u);
		palette = palette & 1u;
		if ((uint8_t)(OBJECT(current_object)->race_car.wheel_turn >> 24u) != palette)
		{
			OBJECT(current_object)->race_car.wheel_turn = (OBJECT(current_object)->race_car.wheel_turn & 0xFFFFFFu) | ((palette & 0xFFu) << 24u);
			OBJECT(current_object)->race_car.wheel_turn &= 0xFFFF0000u;
		}
		palette = (uint16_t)(palette + AS16(CAR(player_car)->brake_lights));
		palette = (uint16_t)(palette + RACE(race)->palette);
		r12 = RACE(race)->animation;
		r14 = RACE(race)->frame;
		sprite_x = (uint16_t)screen_x;
		sprite_y = (uint16_t)screen_y;
		OBJECT(current_object)->x = screen_x;
		OBJECT(current_object)->y = screen_y;
		OBJECT(current_object)->width_scale = scale;
		OBJECT(current_object)->depth_layer = depth_layer;
		race_car_put_frame(scale, sprite_x, sprite_y, &body_attributes, &sprite_entry, &palette);
		OBJECT(current_object)->race_car.body_attributes = body_attributes;
		if (OBJECT(current_object)->race_car.special_body == 0u)
		{
			animation = RACE(race)->animation;
			body_animation = (uint16_t)(animation - OBJECT(current_object)->race_car.first_body_animation);
			if (race_car_body_wheel_kinds[body_animation] != 0u)
			{
				CAR(player_car)->crash_body_shown = 0u;
				r27 = current_object + 0x90u;
				r28 = current_object + 0xA0u;
				if ((OBJECT(current_object)->race_car.model != 6u || race_car_body_wheel_kinds[body_animation] == 2u) && (body_attributes & 0x40u) == 0u)
				{
					swapped_rider = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | swapped_rider;
				}
				if (RACE(race)->speed == 0u)
				{
					r14 = 0u;
					second_rider_frame = 0u;
				}
				else
				{
					r14 = mem_read8(r27 + 0xAu);
					second_rider_frame = mem_read8(r28 + 0xAu);
					if (mem_read8(r27 + 0xBu) <= r14)
						r14 = 1u;
					else
						r14 = (uint8_t)(r14 + 1u);
					if (mem_read8(r28 + 0xBu) <= second_rider_frame)
						second_rider_frame = 1u;
					else
						second_rider_frame = (uint8_t)(second_rider_frame + 1u);
				}
				mem_write8(r27 + 0xAu, r14);
				mem_write8(r28 + 0xAu, second_rider_frame);
				piece = SPRITE_ANIMATION(sprite_animations[animation])->frames[RACE(race)->frame] + 2u;
				view = SPRITE_PIECE(piece)->attributes;
				piece += 2u;
				view = view & 0x40u;
				view = view >> 6;
				view = (uint16_t)(view * 0x34u);
				view = (uint16_t)(view + (uint16_t)(SPRITE_PIECE(piece - 0x2u)->sprite - OBJECT(current_object)->race_car.first_body_piece) / 3u);
				OBJECT(current_object)->race_car.view = view;
				view_offset = (uint16_t)(view << 4);
				piece += 2u;
				y_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->y_offset;
				x_offset = (uint16_t)SPRITE_PIECE(piece - 0x4u)->x_offset;
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->race_car.gesture_shown;
				if ((r12 & mem_read8(r27 + 8u)) == 0u)
				{
					rider_places = mem_read32(r27);
					palette = mem_read16(r27 + 6u);
					r12 = mem_read16(r27 + 4u) + view;
					sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
				}
				r12 = (r12 & ~0xFFu) | OBJECT(current_object)->race_car.gesture_shown;
				if ((r12 & mem_read8(r28 + 8u)) == 0u)
				{
					r14 = second_rider_frame;
					rider_places = mem_read32(r28);
					palette = mem_read16(r28 + 6u);
					r12 = mem_read16(r28 + 4u) + view;
					sprite_entry = race_car_draw_part(view_offset, x_offset, y_offset, scale, sprite_entry, rider_places, palette, (uint16_t)screen_x, (uint16_t)screen_y);
				}
				r27 = current_object + 0xC0u;
				r28 = current_object + 0xD0u;
				if ((OBJECT(current_object)->race_car.body_attributes & 0x40u) == 0u)
				{
					swapped_spray = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | swapped_spray;
				}
				wheel_effect_sides = RACE(race)->wheel_effect_sides;
				sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)r28;
				sprite_entry = race_car_draw_spray(view_offset, x_offset, y_offset, wheel_effect_sides, scale, sprite_entry, (uint16_t)screen_x, (uint16_t)screen_y);
				hard_acceleration_frame = OBJECT(current_object)->race_car.hard_acceleration_frame;
				if (hard_acceleration_frame != 0u || RACE(race)->hard_acceleration != 0u)
				{
					r14 = hard_acceleration_frame;
					hard_acceleration_frame++;
					if (hard_acceleration_frame >= 5u)
						hard_acceleration_frame = 0u;
					OBJECT(current_object)->race_car.hard_acceleration_frame = hard_acceleration_frame;
					r12 = 0x44u;
					palette = 0x58u;
					sprite_x = (uint16_t)screen_x;
					sprite_y = (uint16_t)screen_y;
					race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
					race_car_put_sprite(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
				}
			}
			else
			{
				OBJECT(current_object)->race_car.special_body = 0xFFu;
				if (OBJECT(current_object)->race_car.rival == 0u)
					CAR(player_car)->crash_body_shown = 0xFFu;
			}
		}
		do
		{
			if (L_009AAD || SCREEN(screen)->race_started != 0u || OBJECT(CAR(player_car)->object)->player_car.timer < 0x3Cu)
			{
				if (L_009AAD || OBJECT(current_object)->race_car.rival == 0u)
				{
					L_009AAD = false;
					if (OBJECT(current_object)->race_car.crash_text_frames == 0u)
					{
						if (CAR(player_car)->crash_text == 0u)
						{
							depth_list = screen + 0x5000u + depth_layer * 4u;
							if (DEPTH_LIST(depth_list)->head == 0u)
								DEPTH_LIST(depth_list)->tail = sprite_entry;
							group = SCREEN(screen)->sprite_list_last + 0x10u;
							older_group = DEPTH_LIST(depth_list)->head;
							mem_write32(sprite_entry, older_group);
							mem_write32(sprite_entry + 0x3000u, (uint16_t)(older_group + 0x300u));
							DEPTH_LIST(depth_list)->head = (group >> 4) | 0x8000u;
							SCREEN(screen)->sprite_list_last = sprite_entry;
							return;
						}
						CAR(player_car)->crash_text = 0u;
						OBJECT(current_object)->race_car.crash_text_rise = 0u;
						OBJECT(current_object)->race_car.crash_text_frames = 0x1Eu;
						OBJECT(current_object)->race_car.crash_text_y = screen_y;
						OBJECT(current_object)->race_car.crash_text_animation = 0x2Fu;
						OBJECT(current_object)->race_car.crash_text_palette = 0x4Bu;
						if (CAR(player_car)->crash_sound == 0u)
						{
							OBJECT(current_object)->race_car.crash_text_animation = 0x30u;
							OBJECT(current_object)->race_car.crash_text_palette = 0x4Cu;
						}
					}
					OBJECT(current_object)->race_car.crash_text_frames--;
					r12 = OBJECT(current_object)->race_car.crash_text_animation;
					palette = OBJECT(current_object)->race_car.crash_text_palette;
					r14 = OBJECT(current_object)->race_car.crash_text_rise;
					r14 = r14 >> 1;
					sprite_x = (uint16_t)screen_x;
					crash_text_y = (uint16_t)OBJECT(current_object)->race_car.crash_text_y;
					scale = 0x100u;
					race_car_put_frame(scale, sprite_x, crash_text_y, &ignored, &sprite_entry, &palette);
					OBJECT(current_object)->race_car.crash_text_rise++;
					OBJECT(current_object)->race_car.crash_text_rise &= 7u;
				}
				else
				{
					blink_counter = OBJECT(current_object)->race_car.blink_counter;
					if (race_car_tag_blink[blink_counter & 0x1Fu] != 0u)
					{
						element = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->race_car.tag_palette])[0];
						r12 = 0x1Bu;
						sprite_x = (uint16_t)(screen_x - 3u);
						sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
						sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
						r12 = OBJECT(current_object)->race_car.tag_animation;
						sprite_y -= 0x16u;
						sprite_x += 0x15u;
						sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
					}
				}
			}
			else
			{
				L_009AAD = OBJECT(current_object)->race_car.rival != 0u;
				if (!L_009AAD)
				{
					element = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->race_car.tag_palette])[0];
					r12 = 0x1Bu;
					sprite_x = (uint16_t)(screen_x - 3u);
					sprite_y = (uint16_t)((uint16_t)screen_y - ((0x30u * scale) >> 8));
					sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
					r12 = OBJECT(current_object)->race_car.tag_animation;
					sprite_y -= 0x16u;
					sprite_x += 0x15u;
					sprite_entry = race_car_put_flat_sprite(sprite_entry, element, sprite_x, sprite_y);
				}
			}
		} while (L_009AAD);
	}
	depth_list = screen + 0x5000u + depth_layer * 4u;
	if (DEPTH_LIST(depth_list)->head == 0u)
		DEPTH_LIST(depth_list)->tail = sprite_entry;
	group = SCREEN(screen)->sprite_list_last + 0x10u;
	older_group = DEPTH_LIST(depth_list)->head;
	mem_write32(sprite_entry, older_group);
	mem_write32(sprite_entry + 0x3000u, (uint16_t)(older_group + 0x300u));
	DEPTH_LIST(depth_list)->head = (group >> 4) | 0x8000u;
	SCREEN(screen)->sprite_list_last = sprite_entry;
}

/* Draws the spray at one side's wheels (dust, smoke, water...: the record's kind, from the race record's
   wheel effects), cycling its frames. */
uint32_t race_car_draw_spray(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint8_t wheel_effect_sides, uint32_t scale, uint32_t sprite_entry, uint16_t screen_x, uint16_t screen_y)
{
	uint32_t effect = 0, frame = 0, kind = 0, palette = 0, places = 0;

	frame = mem_read8(r27 + 0xCu);
	if (frame == 0u && (wheel_effect_sides & mem_read8(r27 + 0xDu)) == 0u)
	{
		mem_write8(r27 + 0xCu, 0u);
		mem_write8(r27 + 0xEu, 0u);
		return sprite_entry;
	}
	kind = mem_read8(r27 + 0xEu);
	effect = mem_read8(mem_read32(r27 + 8u));
	if (effect != 0u && effect != kind)
	{
		kind = effect;
		mem_write8(r27 + 0xEu, effect);
		frame = 0u;
	}
	r14 = frame;
	frame = (uint8_t)(frame + 1u);
	if (race_car_spray_frame_counts[kind] <= frame)
		frame = 0u;
	mem_write8(r27 + 0xCu, frame);
	places = mem_read32(r27 + 4u);
	r12 = mem_read16(mem_read32(r27) + kind * 2u);
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
	race_car_put_frame(scale, sprite_x, sprite_y, &ignored, &sprite_entry, &palette);
	return sprite_entry;
}

/* Puts the first piece of frame R14 of animation R12 with palette R13 into the sprite list, scaled by R8
   at R17, R18 (race_car_put_sprite). */
void race_car_put_frame(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_palette)
{
	uint32_t piece_attributes = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[r12])->frames[r14] + 2u;
	*p_palette = sprite_palettes[*p_palette];
	race_car_put_sprite(scale, sprite_x, sprite_y, &piece_attributes, p_sprite_entry, p_palette);
	*p_attributes = (*p_attributes & 0xFFFF0000u) | (piece_attributes & 0xFFFFu);
}

/* Writes one sprite of a frame's piece into the screen's sprite list (and its copy at + 3000), its size
   and offsets scaled by R8, at R17, R18. */
void race_car_put_sprite(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_palette)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t height_and_bits = 0, palette_word = 0, piece_attributes = 0, piece_x = 0, piece_y = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0, element = 0;

	piece_attributes = SPRITE_PIECE(r12)->attributes;
	mem_write16(sprite_entry + 0x3000u, piece_attributes);
	mem_write16(sprite_entry, piece_attributes);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	r14 = sprite_size_steps_ADDRESS + SPRITE_PIECE(r12 - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 = r14 + 2u;
	sprite_source_size = source_size;
	mem_write16(sprite_entry + 0x3000u, sprite_source_size);
	mem_write16(sprite_entry, sprite_source_size);
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	r14 = r14 + 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 = r14 + 2u;
	height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 = r14 + 2u;
	height_and_bits = (((height * scale) >> 8) & 0x3FFu) | height_bits;
	mem_write16(sprite_entry + 0x3000u, height_and_bits);
	mem_write16(sprite_entry, height_and_bits);
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 = r14 + 2u;
	width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
	r14 = r14 + 2u;
	width_and_bits = (((width * scale) >> 8) & 0x3FFu) | width_bits | SCREEN(screen)->sprite_screen_bit;
	mem_write16(sprite_entry + 0x3000u, width_and_bits);
	mem_write16(sprite_entry, width_and_bits);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	piece_y = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset * scale) >> 8) + sprite_y);
	mem_write16(sprite_entry + 0x3000u, piece_y);
	mem_write16(sprite_entry, piece_y);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	piece_x = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset * scale) >> 8) + sprite_x);
	mem_write16(sprite_entry + 0x3000u, piece_x);
	mem_write16(sprite_entry, piece_x);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_rom_offset);
	mem_write16(sprite_entry, sprite_rom_offset);
	sprite_entry += 2u;
	element = SPRITE_PALETTE_WORDS(*p_palette)[0];
	*p_palette += 2u;
	palette_word = element;
	mem_write16(sprite_entry + 0x3000u, palette_word);
	mem_write16(sprite_entry, palette_word);
	sprite_entry += 2u;
	SCREEN(screen)->sprite_groups++;
	r12 = r12 + 2u;
	*p_attributes = (*p_attributes & 0xFFFF0000u) | piece_attributes;
	*p_sprite_entry = sprite_entry;
}

/* Writes the first piece of animation R12 into the sprite list at R17, R18 at its own size (the number
   tags). */
uint32_t race_car_put_flat_sprite(uint32_t sprite_entry, uint16_t palette_word, uint16_t sprite_x, uint16_t sprite_y)
{
	uint16_t attributes = 0, height_and_bits = 0, sprite_rom_offset = 0, sprite_source_size = 0, width_and_bits = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[r12])->frames[0] + 2u;
	attributes = SPRITE_PIECE(r12)->attributes;
	mem_write16(sprite_entry + 0x3000u, attributes);
	mem_write16(sprite_entry, attributes);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	r14 = sprite_size_steps_ADDRESS + SPRITE_PIECE(r12 - 0x2u)->sprite * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 = r14 + 2u;
	sprite_source_size = source_size;
	mem_write16(sprite_entry + 0x3000u, sprite_source_size);
	mem_write16(sprite_entry, sprite_source_size);
	sprite_entry += 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	r14 = r14 + 2u;
	sprite_rom_offset = rom_offset;
	height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 = r14 + 2u;
	height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 = r14 + 2u;
	height_and_bits = height | height_bits;
	mem_write16(sprite_entry + 0x3000u, height_and_bits);
	mem_write16(sprite_entry, height_and_bits);
	sprite_entry += 2u;
	width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 = r14 + 2u;
	width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
	r14 = r14 + 2u;
	width_and_bits = width | width_bits | SCREEN(screen)->sprite_screen_bit;
	mem_write16(sprite_entry + 0x3000u, width_and_bits);
	mem_write16(sprite_entry, width_and_bits);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_y);
	mem_write16(sprite_entry, sprite_y);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_x);
	mem_write16(sprite_entry, sprite_x);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_rom_offset);
	mem_write16(sprite_entry, sprite_rom_offset);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, palette_word);
	mem_write16(sprite_entry, palette_word);
	sprite_entry += 2u;
	SCREEN(screen)->sprite_groups++;
	r12 = r12 + 2u;
	return sprite_entry;
}
