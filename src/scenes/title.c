/**
 *    Name:        scenes/title.c
 *    Description: the title screen: the cars and riders, the palms, the logo, the seagull, the pictures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The title of the attract mode. By state: 0 sets it up: clears screen 5800 and 5801 (title_logo_shine,
   the logo's shine signal), keeps the addresses of the tilemaps' register copies
   (title_screen_layer_setups by screen: scroll_x_address, scroll_y_address for the first tilemap,
   second_scroll_x_address, second_scroll_y_address for the second), the background (set_up_background
   with title_background_setups by screen) and its scrolls, picks the car of the next car presentation
   (screen attract_car from next_attract_car, which steps through the eight), starts the credits line
   (credit_display), the prompt (coin_prompt) and the eight cars driving past (title_car); 1 for 45
   frames (frames): palm trees (title_screen_start_palms) and the two tilemaps scrolled by 8 and 20 a
   frame, then the logo (title_logo, title_logo_copy), three logo cars (title_logo_car) and the three
   lines across the logo (title_logo_stripe); 2 the same palms and scrolls, with three more logo cars at
   each count of wave_times (120, 240, 300, 390, 450). */
void title_screen_update(void)
{
	uint8_t next_attract_car = 0, object_index = 0;
	uint32_t background_setup = 0, object = 0, objects_left = 0, title_screen_layer_setup = 0;
	uint8_t frames = 0;
	uint32_t scroll_x_address = 0, scroll_x_address_2 = 0, second_scroll_x_address = 0, scroll_x_address_3 = 0, second_scroll_x_address_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		SCREEN(screen)->route_map_signals &= 0xFFFFFF00u;
		SCREEN(screen)->title_logo_shine = 0u;
		title_screen_layer_setup = title_screen_layer_setups_ADDRESS + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
		OBJECT(current_object)->title_screen.tile_bank_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup)->tile_bank_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.pages_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x4u)->pages_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.scroll_x_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x8u)->scroll_x_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.scroll_y_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0xCu)->scroll_y_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.second_tile_bank_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x10u)->second_tile_bank_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.second_pages_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x14u)->second_pages_address;
		title_screen_layer_setup += 4u;
		OBJECT(current_object)->title_screen.second_scroll_x_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x18u)->second_scroll_x_address;
		OBJECT(current_object)->title_screen.second_scroll_y_address = TITLE_SCREEN_LAYER_SETUP(title_screen_layer_setup - 0x18u)->second_scroll_y_address;
		background_setup = title_background_setups_ADDRESS + SCREEN(screen)->index * 5u * 4u;
		set_up_background(background_setup);
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->title_screen.second_scroll_y_address)[0] = 0xFFF0u;
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->title_screen.scroll_x_address)[0] = 0x360u;
		scroll_x_address = OBJECT(current_object)->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address)[0] += (SCREEN(screen)->car_number * 5u) << 6;
		VIDEO_REGISTER_WORDS(OBJECT(current_object)->title_screen.scroll_y_address)[0] = 0x72u;
		next_attract_car = SCREEN(screen)->next_attract_car;
		SCREEN(screen)->attract_car = next_attract_car;
		SCREEN(screen)->next_attract_car = (next_attract_car + 1u) & 7u;
		SCREEN(screen)->link_event_data_1 = SCREEN(screen)->attract_car;
		SCREEN(screen)->link_event = 5u;
		object = start_object();
		OBJECT(object)->handler = credit_display_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = coin_prompt_update_ADDRESS;
		objects_left = 8u;
		object_index = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_car_drive_ADDRESS;
			OBJECT(object)->title_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		OBJECT(current_object)->title_screen.frames = 0x2Du;
		OBJECT(current_object)->state++;
		return;
	case 1:
		title_screen_start_palms();
		scroll_x_address_2 = OBJECT(current_object)->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address_2)[0] += 8u;
		second_scroll_x_address = OBJECT(current_object)->title_screen.second_scroll_x_address;
		VIDEO_REGISTER_WORDS(second_scroll_x_address)[0] += 0x14u;
		frames = (uint8_t)OBJECT(current_object)->title_screen.frames;
		OBJECT(current_object)->title_screen.frames = (OBJECT(current_object)->title_screen.frames & 0xFF00u) | (frames - 1u & 0xFFu);
		if (frames != 1u)
			return;
		object = start_object();
		OBJECT(object)->handler = title_logo_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = title_logo_copy_update_ADDRESS;
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_car_update_ADDRESS;
			OBJECT(object)->title_logo_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_stripe_update_ADDRESS;
			OBJECT(object)->title_logo_stripe.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		OBJECT(current_object)->title_screen.wave_times = title_logo_car_wave_frames_ADDRESS;
		OBJECT(current_object)->title_screen.frames = 0u;
		OBJECT(current_object)->state++;
		return;
	case 2:
		title_screen_start_palms();
		scroll_x_address_3 = OBJECT(current_object)->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address_3)[0] += 8u;
		second_scroll_x_address_2 = OBJECT(current_object)->title_screen.second_scroll_x_address;
		VIDEO_REGISTER_WORDS(second_scroll_x_address_2)[0] += 0x14u;
		OBJECT(current_object)->title_screen.frames++;
		if (OBJECT(current_object)->title_screen.frames != TITLE_WAVE_FRAMES(OBJECT(current_object)->title_screen.wave_times)[0])
			return;
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_car_update_ADDRESS;
			OBJECT(object)->title_logo_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		OBJECT(current_object)->title_screen.wave_times += 2u;
		return;
	default:
		invalid_jump(title_screen_state_jumps[OBJECT(current_object)->state]);
	}
}

/* One of the eight cars driving past at the bottom of the title (title_screen starts them, index 0-7).
   Sets itself up from its record (title_car_records by index: the model, whose animation
   title_car_animations gives, palette, width_scale, height_scale, position, y, speed, and its
   depth_layer less 18F; draw mode A, frame 9) and starts its driver (title_car_driver) and passenger
   (title_car_passenger); each frame drives on by speed along a strip 4096 wide that wraps (clearing
   flags bit 5 as it wraps), is drawn at the position less the screen's offset (title_car_screen_offsets
   by screen car_number) and bobs a line up on odd frames (frames; rest_y keeps its y meanwhile). */
void title_car_drive(void)
{
	uint16_t bounce = 0;
	uint32_t animation = 0, model = 0, object = 0, title_car_record = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	model = title_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		title_car_record = title_car_records_ADDRESS + ((uint32_t)OBJECT(current_object)->title_car.index << 4);
		model = TITLE_CAR_RECORD(title_car_record)->model;
		animation = title_car_animations[model];
		title_car_record += 2u;
		animation_start(animation);
		OBJECT(current_object)->palette = TITLE_CAR_RECORD(title_car_record - 0x2u)->palette;
		title_car_record += 2u;
		OBJECT(current_object)->width_scale = TITLE_CAR_RECORD(title_car_record - 0x4u)->width_scale;
		title_car_record += 2u;
		OBJECT(current_object)->height_scale = TITLE_CAR_RECORD(title_car_record - 0x6u)->height_scale;
		title_car_record += 2u;
		OBJECT(current_object)->title_car.position = TITLE_CAR_RECORD(title_car_record - 0x8u)->position;
		title_car_record += 2u;
		OBJECT(current_object)->y = TITLE_CAR_RECORD(title_car_record - 0xAu)->y;
		title_car_record += 2u;
		OBJECT(current_object)->title_car.speed = TITLE_CAR_RECORD(title_car_record - 0xCu)->speed;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x18Fu;
		OBJECT(current_object)->depth_layer += TITLE_CAR_RECORD(title_car_record - 0xCu)->depth_layer;
		object = start_object();
		OBJECT(object)->handler = title_car_passenger_update_ADDRESS;
		OBJECT(object)->title_car_passenger.car = current_object;
		OBJECT(object)->title_car_passenger.model = model;
		object = start_object();
		OBJECT(object)->handler = title_car_driver_update_ADDRESS;
		OBJECT(object)->title_car_driver.car = current_object;
		OBJECT(object)->title_car_driver.model = model;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(title_car_state_jumps[state]);
	}
	OBJECT(current_object)->title_car.position += OBJECT(current_object)->title_car.speed;
	if (OBJECT(current_object)->title_car.position >= 0x1000)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		OBJECT(current_object)->title_car.position &= 0xFFFu;
	}
	OBJECT(current_object)->x = OBJECT(current_object)->title_car.position;
	OBJECT(current_object)->x -= title_car_screen_offsets[SCREEN(screen)->car_number];
	OBJECT(current_object)->title_car.frames++;
	bounce = OBJECT(current_object)->title_car.frames & 1u;
	OBJECT(current_object)->title_car.rest_y = OBJECT(current_object)->y;
	OBJECT(current_object)->y -= bounce;
	draw_object(model);
	OBJECT(current_object)->y = OBJECT(current_object)->title_car.rest_y;
}

/* The driver of a title car (the blond man at the wheel): sets up its picture by the car's model
   (animation title_driver_animations, palette title_driver_palettes, x_offset title_driver_x_offsets,
   y_offset title_driver_y_offsets, a layer in front of the car), then is drawn with the car each frame
   (title_rider_draw). */
void title_car_driver_update(void)
{
	uint32_t animation = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		model = OBJECT(current_object)->title_car_driver.model;
		OBJECT(current_object)->palette = title_driver_palettes[model];
		OBJECT(current_object)->title_car_driver.x_offset = title_driver_x_offsets[model];
		OBJECT(current_object)->title_car_driver.y_offset = title_driver_y_offsets[model];
		animation = title_driver_animations[model];
		animation_start(animation);
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = OBJECT(OBJECT(current_object)->title_car_driver.car)->depth_layer;
		OBJECT(current_object)->depth_layer++;
		return;
	case 1:
		title_rider_draw();
		return;
	default:
		invalid_jump(title_car_driver_state_jumps[OBJECT(current_object)->state]);
	}
}

/* The passenger of a title car (the red-haired woman beside the driver): sets up its picture by the
   car's model (animation title_passenger_animations, palette title_passenger_palettes, x_offset
   title_passenger_x_offsets, y_offset title_passenger_y_offsets, a layer in front of the car), then is
   drawn with the car each frame (title_rider_draw). */
void title_car_passenger_update(void)
{
	uint32_t animation = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		title_rider_draw();
		return;
	default:
		invalid_jump(title_car_passenger_state_jumps[OBJECT(current_object)->state]);
	}
	model = OBJECT(current_object)->title_car_passenger.model;
	OBJECT(current_object)->palette = title_passenger_palettes[model];
	OBJECT(current_object)->title_car_passenger.x_offset = title_passenger_x_offsets[model];
	OBJECT(current_object)->title_car_passenger.y_offset = title_passenger_y_offsets[model];
	animation = title_passenger_animations[model];
	animation_start(animation);
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = OBJECT(OBJECT(current_object)->title_car_passenger.car)->depth_layer;
	OBJECT(current_object)->depth_layer++;
}

/* Draws a rider of a title car (title_car_driver, title_car_passenger) on its car: at the car's x and y
   plus its x_offset and y_offset, bobbing with the car (bit 0 of the car's frames), at the size
   title_rider_scales gives for the model. */
void title_rider_draw(void)
{
	uint32_t model = 0, y = 0;

	model = OBJECT(current_object)->title_rider.model;
	OBJECT(current_object)->width_scale = title_rider_scales[model];
	OBJECT(current_object)->height_scale = title_rider_scales[model];
	OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->title_rider.car)->x + OBJECT(current_object)->title_rider.x_offset;
	y = (uint16_t)(OBJECT(OBJECT(current_object)->title_rider.car)->y + OBJECT(current_object)->title_rider.y_offset - (OBJECT(OBJECT(current_object)->title_rider.car)->title_car.frames & 1u));
	OBJECT(current_object)->y = y;
	y = draw_object(y);
}

/* Starts the title's palm trees: a near one (title_palm_tree with near_or_far 0) every 8th frame
   (palm_frames), else a far one (1) every other frame. */
void title_screen_start_palms(void)
{
	uint32_t object = 0;

	OBJECT(current_object)->title_screen.palm_frames = (OBJECT(current_object)->title_screen.palm_frames & 0xFF00u) | ((uint8_t)OBJECT(current_object)->title_screen.palm_frames + 1u & 0xFFu);
	if ((OBJECT(current_object)->title_screen.palm_frames & 7u) == 0u)
	{
		object = start_object();
		OBJECT(object)->handler = title_palm_tree_update_ADDRESS;
		OBJECT(object)->title_palm_tree.near_or_far = 0u;
		return;
	}
	if ((OBJECT(current_object)->title_screen.palm_frames & 1u) == 0u)
	{
		object = start_object();
		OBJECT(object)->handler = title_palm_tree_update_ADDRESS;
		OBJECT(object)->title_palm_tree.near_or_far = 1u;
		return;
	}
}

/* A palm tree passing the title from right to left (title_screen_start_palms starts them): near
   (near_or_far 0: full size, low on the screen, 48 a frame) or far (1: 3/8 of the size, along the beach,
   18 a frame); ends once off the left edge. */
void title_palm_tree_update(void)
{
	uint32_t animation = 0, state_routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = title_palm_tree_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = PALM_TREE;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x6Du;
		if (OBJECT(current_object)->title_palm_tree.near_or_far != 0u)
		{
			OBJECT(current_object)->width_scale = 0x60u;
			OBJECT(current_object)->height_scale = 0x60u;
			OBJECT(current_object)->x = 0x170u;
			OBJECT(current_object)->y = 0x96u;
			OBJECT(current_object)->depth_layer = 0x185u;
		}
		else
		{
			OBJECT(current_object)->width_scale = 0x100u;
			OBJECT(current_object)->height_scale = 0x100u;
			OBJECT(current_object)->x = 0x170u;
			OBJECT(current_object)->y = 0xF0u;
			OBJECT(current_object)->depth_layer = 0x194u;
		}
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(title_palm_tree_state_jumps[state]);
	}
	if ((int16_t)OBJECT(current_object)->x <= -0x30)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (OBJECT(current_object)->title_palm_tree.near_or_far != 0u)
	{
		OBJECT(current_object)->x -= 0x12u;
		state_routine = draw_object(state_routine);
		return;
	}
	OBJECT(current_object)->x -= 0x30u;
	state_routine = draw_object(state_routine);
}

/* The OutRunners logo of the title: slides in from the right (16 a frame for 10 frames); then starts the
   palm trees of its board (title_board_palms), the two seagulls (title_seagull) and the title's still
   pictures (title_picture: the board, SEGA, (C) SEGA 1992), flashes through the palettes at 01AC54
   (palette_list) for 6 frames and settles in palette 269 at its next frame; then shines (its animation
   again from frame 1) each time the first logo car reaches the right edge (screen title_logo_shine). */
void title_logo_update(void)
{
	uint32_t animation = 0, object = 0, object_index = 0, objects_left = 0, palette_list = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	object_index = title_logo_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = TITLE_LOGO;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x26Au;
		OBJECT(current_object)->x = 0x142u;
		OBJECT(current_object)->y = 0x47u;
		OBJECT(current_object)->depth_layer = 0x199u;
		OBJECT(current_object)->title_logo.frames = 0xBu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		frames = OBJECT(current_object)->title_logo.frames;
		OBJECT(current_object)->title_logo.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->state++;
			OBJECT(current_object)->palette = 0x269u;
			animation_advance(0u);
			object_index = draw_object(object_index);
			return;
		}
		palette_list = OBJECT(current_object)->title_logo.palette_list;
		OBJECT(current_object)->palette = TITLE_LOGO_PALETTES(palette_list)[0];
		OBJECT(current_object)->title_logo.palette_list = palette_list + 2u;
		object_index = draw_object(object_index);
		return;
	case 3:
		if (SCREEN(screen)->title_logo_shine != 0u)
		{
			OBJECT(current_object)->frame = 1u;
			OBJECT(current_object)->frame_time = 2u;
			OBJECT(current_object)->frame_countdown = 2;
		}
		animation_advance(0u);
		object_index = draw_object(object_index);
		return;
	default:
		invalid_jump(title_logo_state_jumps[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo.frames;
	OBJECT(current_object)->title_logo.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		OBJECT(current_object)->x -= 0x10u;
		object_index = draw_object(object_index);
		return;
	}
	object = start_object();
	OBJECT(object)->handler = title_board_palms_update_ADDRESS;
	objects_left = 2u;
	object_index &= ~0xFFu;
	do
	{
		object = start_object();
		OBJECT(object)->handler = title_seagull_fly_ADDRESS;
		OBJECT(object)->title_seagull.index = object_index;
		object_index = (object_index & ~0xFFu) | (uint8_t)(object_index + 1u);
		objects_left--;
	} while (objects_left != 0u);
	objects_left = 3u;
	object_index &= ~0xFFu;
	do
	{
		object = start_object();
		OBJECT(object)->handler = title_picture_draw_ADDRESS;
		OBJECT(object)->title_picture.index = object_index;
		object_index = (object_index & ~0xFFu) | (uint8_t)(object_index + 1u);
		objects_left--;
	} while (objects_left != 0u);
	OBJECT(current_object)->title_logo.palette_list = title_logo_flash_palettes_ADDRESS;
	OBJECT(current_object)->title_logo.frames = 6u;
	OBJECT(current_object)->state++;
	object_index = draw_object(object_index);
}

/* A copy of the logo in other colours (cyan) that slides in from the left as the logo (title_logo)
   slides in from the right, meets it, flashes through the palettes at 01AC62 (palette_list) and ends. */
void title_logo_copy_update(void)
{
	uint32_t animation = 0, palette_list = 0, state_routine = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = title_logo_copy_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = TITLE_LOGO_COPY;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x26Du;
		OBJECT(current_object)->x = 2u;
		OBJECT(current_object)->y = 0x47u;
		OBJECT(current_object)->depth_layer = 0x199u;
		OBJECT(current_object)->title_logo_copy.frames = 0xBu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		frames = OBJECT(current_object)->title_logo_copy.frames;
		OBJECT(current_object)->title_logo_copy.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		palette_list = OBJECT(current_object)->title_logo_copy.palette_list;
		OBJECT(current_object)->palette = TITLE_LOGO_PALETTES(palette_list)[0];
		OBJECT(current_object)->title_logo_copy.palette_list = palette_list + 2u;
		state_routine = draw_object(state_routine);
		return;
	default:
		invalid_jump(title_logo_copy_state_jumps[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo_copy.frames;
	OBJECT(current_object)->title_logo_copy.frames = frames_2 - 1u;
	if (frames_2 == 1u)
	{
		OBJECT(current_object)->title_logo_copy.palette_list = title_logo_copy_flash_palettes_ADDRESS;
		OBJECT(current_object)->title_logo_copy.frames = 6u;
		OBJECT(current_object)->state++;
		state_routine = draw_object(state_routine);
		return;
	}
	OBJECT(current_object)->x += 0x10u;
	state_routine = draw_object(state_routine);
}

/* A small car racing across the logo along one of its lines (title_screen starts three at a time, index
   0-2): after 1, 5 or 9 frames by index, one of the eight cars at random (title_car_animations,
   title_logo_car_palettes) at a quarter of its size at the left edge; 16 a frame to the right edge; the
   first of the three then makes the logo, its board's palm trees and the lines shine (screen
   title_logo_shine for a frame), and it ends. */
void title_logo_car_update(void)
{
	uint32_t animation = 0, model = 0, place = 0, place_offset = 0, state_routine = 0;
	uint8_t delay = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = title_logo_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->title_logo_car.delay = ((uint32_t)OBJECT(current_object)->title_logo_car.index << 2) + 1u;
		OBJECT(current_object)->state++;
		delay = OBJECT(current_object)->title_logo_car.delay;
		OBJECT(current_object)->title_logo_car.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->draw_mode = 0xAu;
		model = random_number();
		model &= 7u;
		animation = title_car_animations[model];
		animation_start(animation);
		OBJECT(current_object)->palette = title_logo_car_palettes[model];
		place_offset = (uint32_t)OBJECT(current_object)->title_logo_car.index << 2;
		place = title_logo_car_places_ADDRESS + place_offset;
		OBJECT(current_object)->x = TITLE_PLACE(place)->x;
		OBJECT(current_object)->y = TITLE_PLACE(place)->y;
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Au;
		OBJECT(current_object)->state++;
		place_offset = draw_object(place_offset);
		return;
	case 1:
		delay = OBJECT(current_object)->title_logo_car.delay;
		OBJECT(current_object)->title_logo_car.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->draw_mode = 0xAu;
		model = random_number();
		model &= 7u;
		animation = title_car_animations[model];
		animation_start(animation);
		OBJECT(current_object)->palette = title_logo_car_palettes[model];
		place_offset = (uint32_t)OBJECT(current_object)->title_logo_car.index << 2;
		place = title_logo_car_places_ADDRESS + place_offset;
		OBJECT(current_object)->x = TITLE_PLACE(place)->x;
		OBJECT(current_object)->y = TITLE_PLACE(place)->y;
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Au;
		OBJECT(current_object)->state++;
		place_offset = draw_object(place_offset);
		return;
	case 2:
		OBJECT(current_object)->x += 0x10u;
		if ((int16_t)OBJECT(current_object)->x <= 0x148)
		{
			state_routine = draw_object(state_routine);
			return;
		}
		if (OBJECT(current_object)->title_logo_car.index == 0u)
			SCREEN(screen)->title_logo_shine = 1u;
		OBJECT(current_object)->state++;
		state_routine = draw_object(state_routine);
		return;
	case 3:
		SCREEN(screen)->title_logo_shine = 0u;
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(title_logo_car_state_jumps[state]);
	}
}

/* One of the three thin coloured lines across the logo that the logo cars race along (index 0-2): after
   1, 5 or 9 frames by index it slides in from the left (16 a frame for 19 frames), then shines with the
   logo (its animation from frame 0 when screen title_logo_shine is set). */
void title_logo_stripe_update(void)
{
	uint32_t animation = 0, place = 0, place_offset = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	place_offset = title_logo_stripe_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->title_logo_stripe.frames = ((uint32_t)OBJECT(current_object)->title_logo_stripe.index << 2) + 1u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		frames = OBJECT(current_object)->title_logo_stripe.frames;
		OBJECT(current_object)->title_logo_stripe.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			OBJECT(current_object)->state++;
			place_offset = draw_object(place_offset);
			return;
		}
		OBJECT(current_object)->x += 0x10u;
		place_offset = draw_object(place_offset);
		return;
	case 3:
		if (SCREEN(screen)->title_logo_shine != 0u)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->frame_time = 2u;
			OBJECT(current_object)->frame_countdown = 2;
		}
		animation_advance(0u);
		place_offset = draw_object(place_offset);
		return;
	default:
		invalid_jump(title_logo_stripe_state_jumps[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo_stripe.frames;
	OBJECT(current_object)->title_logo_stripe.frames = frames_2 - 1u;
	if (frames_2 != 1u)
		return;
	OBJECT(current_object)->draw_mode = 0xAu;
	animation = TITLE_LOGO_STRIPE;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x270u;
	place_offset = (uint32_t)OBJECT(current_object)->title_logo_stripe.index << 2;
	place = title_logo_stripe_places_ADDRESS + place_offset;
	OBJECT(current_object)->x = TITLE_PLACE(place)->x;
	OBJECT(current_object)->y = TITLE_PLACE(place)->y;
	OBJECT(current_object)->depth_layer = 0x198u;
	OBJECT(current_object)->title_logo_stripe.frames = 0x14u;
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->title_logo_stripe.frames;
	OBJECT(current_object)->title_logo_stripe.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		place_offset = draw_object(place_offset);
		return;
	}
	OBJECT(current_object)->x += 0x10u;
	place_offset = draw_object(place_offset);
}

/* The palm trees at the right end of the logo's board: grow in from 3/4 of their size (8 frames), then
   sway each time the logo shines (screen title_logo_shine). */
void title_board_palms_update(void)
{
	uint32_t animation = 0, state_routine = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = title_board_palms_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = TITLE_BOARD_PALMS;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x272u;
		OBJECT(current_object)->width_scale = 0xC0u;
		OBJECT(current_object)->height_scale = 0xC0u;
		OBJECT(current_object)->x = 0xE2u;
		OBJECT(current_object)->y = 0x4Eu;
		OBJECT(current_object)->depth_layer = 0x198u;
		OBJECT(current_object)->title_board_palms.frames = 9u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		if (SCREEN(screen)->title_logo_shine != 0u)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->frame_time = 3u;
			OBJECT(current_object)->frame_countdown = 6;
		}
		animation_advance(0u);
		state_routine = draw_object(state_routine);
		return;
	default:
		invalid_jump(title_board_palms_state_jumps[state]);
	}
	frames = OBJECT(current_object)->title_board_palms.frames;
	OBJECT(current_object)->title_board_palms.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		state_routine = draw_object(state_routine);
		return;
	}
	OBJECT(current_object)->width_scale += 8u;
	OBJECT(current_object)->height_scale += 8u;
	state_routine = draw_object(state_routine);
}

/* One of the two seagulls over the title (index 0, 1): flies up out of the logo for 8 frames growing to
   full size, then circles about that place in a figure of eight (the sine of x_angle, a turn in 128
   frames, and of y_angle, twice as fast), flapping (its animation). */
void title_seagull_fly(void)
{
	uint16_t y_wave = 0;
	uint32_t angle = 0, animation = 0, place = 0, place_offset = 0, x_angle = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	place_offset = title_seagull_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = TITLE_SEAGULL;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x272u;
		OBJECT(current_object)->width_scale = 0xC0u;
		OBJECT(current_object)->height_scale = 0xC0u;
		place_offset = (uint32_t)OBJECT(current_object)->title_seagull.index << 2;
		place = title_seagull_places_ADDRESS + place_offset;
		OBJECT(current_object)->x = TITLE_PLACE(place)->x;
		OBJECT(current_object)->y = TITLE_PLACE(place)->y;
		OBJECT(current_object)->depth_layer = 0x198u;
		OBJECT(current_object)->title_seagull.frames = 9u;
		OBJECT(current_object)->state++;
		frames = OBJECT(current_object)->title_seagull.frames;
		OBJECT(current_object)->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			angle = (uint16_t)((uint32_t)OBJECT(current_object)->title_seagull.index << 11);
			OBJECT(current_object)->title_seagull.x_angle = angle;
			OBJECT(current_object)->title_seagull.y_angle = angle;
			OBJECT(current_object)->title_seagull.centre_x = OBJECT(current_object)->x;
			OBJECT(current_object)->title_seagull.centre_y = OBJECT(current_object)->y;
			OBJECT(current_object)->state++;
			animation_advance(0u);
			angle = draw_object(angle);
			return;
		}
		OBJECT(current_object)->width_scale += 8u;
		OBJECT(current_object)->height_scale += 8u;
		OBJECT(current_object)->x--;
		OBJECT(current_object)->y -= 5u;
		animation_advance(0u);
		place_offset = draw_object(place_offset);
		return;
	case 1:
		frames = OBJECT(current_object)->title_seagull.frames;
		OBJECT(current_object)->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			angle = (uint16_t)((uint32_t)OBJECT(current_object)->title_seagull.index << 11);
			OBJECT(current_object)->title_seagull.x_angle = angle;
			OBJECT(current_object)->title_seagull.y_angle = angle;
			OBJECT(current_object)->title_seagull.centre_x = OBJECT(current_object)->x;
			OBJECT(current_object)->title_seagull.centre_y = OBJECT(current_object)->y;
			OBJECT(current_object)->state++;
			animation_advance(0u);
			angle = draw_object(angle);
			return;
		}
		OBJECT(current_object)->width_scale += 8u;
		OBJECT(current_object)->height_scale += 8u;
		OBJECT(current_object)->x--;
		OBJECT(current_object)->y -= 5u;
		animation_advance(0u);
		place_offset = draw_object(place_offset);
		return;
	case 2:
		OBJECT(current_object)->title_seagull.x_angle += 0x20u;
		if (OBJECT(current_object)->title_seagull.x_angle >= 0x1000)
			OBJECT(current_object)->title_seagull.x_angle -= 0x1000u;
		OBJECT(current_object)->title_seagull.y_angle += 0x40u;
		if (OBJECT(current_object)->title_seagull.y_angle >= 0x1000)
			OBJECT(current_object)->title_seagull.y_angle -= 0x1000u;
		OBJECT(current_object)->x = OBJECT(current_object)->title_seagull.centre_x;
		OBJECT(current_object)->y = OBJECT(current_object)->title_seagull.centre_y;
		x_angle = OBJECT(current_object)->title_seagull.x_angle & 0xFFFu;
		y_wave = (uint16_t)((int16_t)sine_table[(uint32_t)OBJECT(current_object)->title_seagull.y_angle] >> 12);
		OBJECT(current_object)->x += (int16_t)sine_table[x_angle] >> 9;
		OBJECT(current_object)->y += y_wave;
		animation_advance(0u);
		x_angle = draw_object(x_angle);
		return;
	default:
		invalid_jump(title_seagull_state_jumps[state]);
	}
}

/* One of the title's still pictures (title_logo starts them): by index, from its record at 01AC70
   (animation, palette, width_scale, height_scale, x, y, depth_layer): 0 (C) SEGA 1992, 1 the oval board
   behind the logo, 2 the SEGA logo. */
void title_picture_draw(void)
{
	uint32_t animation = 0, picture = 0, picture_offset = 0, state_routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = title_picture_state_jumps[state];
	switch (state)
	{
	case 1:
		state_routine = draw_object(state_routine);
		return;
	case 0:
		break;
	default:
		invalid_jump(title_picture_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	picture_offset = (uint32_t)OBJECT(current_object)->title_picture.index << 4;
	picture = title_pictures_ADDRESS + picture_offset;
	animation = PICTURE_RECORD(picture)->animation;
	picture += 2u;
	animation_start(animation);
	OBJECT(current_object)->palette = PICTURE_RECORD(picture - 0x2u)->palette;
	picture += 2u;
	OBJECT(current_object)->width_scale = PICTURE_RECORD(picture - 0x4u)->width_scale;
	picture += 2u;
	OBJECT(current_object)->height_scale = PICTURE_RECORD(picture - 0x6u)->height_scale;
	picture += 2u;
	OBJECT(current_object)->x = PICTURE_RECORD(picture - 0x8u)->x;
	picture += 2u;
	OBJECT(current_object)->y = PICTURE_RECORD(picture - 0xAu)->y;
	OBJECT(current_object)->depth_layer = PICTURE_RECORD(picture - 0xAu)->depth_layer;
	OBJECT(current_object)->state++;
	picture_offset = draw_object(picture_offset);
}
