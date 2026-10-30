/**
 *    Name:        scenes/title.c
 *    Description: the title screen: the cars and riders, the palms, the logo, the seagull, the pictures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
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
	uint32_t background_setup = 0, objects_left = 0;
	struct object *object = NULL;
	const struct title_screen_layer_setup *title_screen_layer_setup = NULL;
	uint8_t frames = 0;
	uint32_t scroll_x_address = 0, scroll_x_address_2 = 0, second_scroll_x_address = 0, scroll_x_address_3 = 0, second_scroll_x_address_2 = 0;

	switch (current_object->state)
	{
	case 0:
		screen->route_map_signals &= 0xFFFFFF00u;
		screen->title_logo_shine = 0u;
		title_screen_layer_setup = rom_pointer(title_screen_layer_setups_ADDRESS + ((uint32_t)screen->index << 3) * 4u);
		current_object->title_screen.tile_bank_address = title_screen_layer_setup->tile_bank_address;
		current_object->title_screen.pages_address = title_screen_layer_setup->pages_address;
		current_object->title_screen.scroll_x_address = title_screen_layer_setup->scroll_x_address;
		current_object->title_screen.scroll_y_address = title_screen_layer_setup->scroll_y_address;
		current_object->title_screen.second_tile_bank_address = title_screen_layer_setup->second_tile_bank_address;
		current_object->title_screen.second_pages_address = title_screen_layer_setup->second_pages_address;
		current_object->title_screen.second_scroll_x_address = title_screen_layer_setup->second_scroll_x_address;
		current_object->title_screen.second_scroll_y_address = title_screen_layer_setup->second_scroll_y_address;
		background_setup = title_background_setups_ADDRESS + screen->index * 5u * 4u;
		set_up_background(background_setup);
		VIDEO_REGISTER_WORDS(current_object->title_screen.second_scroll_y_address)[0] = 0xFFF0u;
		VIDEO_REGISTER_WORDS(current_object->title_screen.scroll_x_address)[0] = 0x360u;
		scroll_x_address = current_object->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address)[0] += (screen->car_number * 5u) << 6;
		VIDEO_REGISTER_WORDS(current_object->title_screen.scroll_y_address)[0] = 0x72u;
		next_attract_car = screen->next_attract_car;
		screen->attract_car = next_attract_car;
		screen->next_attract_car = (next_attract_car + 1u) & 7u;
		screen->link_event_data_1 = screen->attract_car;
		screen->link_event = 5u;
		object = start_object();
		object->handler = credit_display_update_ADDRESS;
		object = start_object();
		object->handler = coin_prompt_update_ADDRESS;
		objects_left = 8u;
		object_index = 0u;
		do
		{
			object = start_object();
			object->handler = title_car_drive_ADDRESS;
			object->title_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		current_object->title_screen.frames = 0x2Du;
		current_object->state++;
		return;
	case 1:
		title_screen_start_palms();
		scroll_x_address_2 = current_object->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address_2)[0] += 8u;
		second_scroll_x_address = current_object->title_screen.second_scroll_x_address;
		VIDEO_REGISTER_WORDS(second_scroll_x_address)[0] += 0x14u;
		frames = (uint8_t)current_object->title_screen.frames;
		current_object->title_screen.frames = (current_object->title_screen.frames & 0xFF00u) | (frames - 1u & 0xFFu);
		if (frames != 1u)
			return;
		object = start_object();
		object->handler = title_logo_update_ADDRESS;
		object = start_object();
		object->handler = title_logo_copy_update_ADDRESS;
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			object->handler = title_logo_car_update_ADDRESS;
			object->title_logo_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			object->handler = title_logo_stripe_update_ADDRESS;
			object->title_logo_stripe.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		current_object->title_screen.wave_times = title_logo_car_wave_frames_ADDRESS;
		current_object->title_screen.frames = 0u;
		current_object->state++;
		return;
	case 2:
		title_screen_start_palms();
		scroll_x_address_3 = current_object->title_screen.scroll_x_address;
		VIDEO_REGISTER_WORDS(scroll_x_address_3)[0] += 8u;
		second_scroll_x_address_2 = current_object->title_screen.second_scroll_x_address;
		VIDEO_REGISTER_WORDS(second_scroll_x_address_2)[0] += 0x14u;
		current_object->title_screen.frames++;
		if (current_object->title_screen.frames != TITLE_WAVE_FRAMES(current_object->title_screen.wave_times)[0])
			return;
		objects_left = 3u;
		object_index = 0u;
		do
		{
			object = start_object();
			object->handler = title_logo_car_update_ADDRESS;
			object->title_logo_car.index = object_index;
			object_index++;
			objects_left--;
		} while (objects_left != 0u);
		current_object->title_screen.wave_times += 2u;
		return;
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
	uint32_t animation = 0, model = 0;
	struct object *object = NULL;
	const struct title_car_record *title_car_record = NULL;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		title_car_record = rom_pointer(title_car_records_ADDRESS + ((uint32_t)current_object->title_car.index << 4));
		model = title_car_record->model;
		animation = title_car_animations[model];
		animation_start(animation);
		current_object->palette = title_car_record->palette;
		current_object->width_scale = title_car_record->width_scale;
		current_object->height_scale = title_car_record->height_scale;
		current_object->title_car.position = title_car_record->position;
		current_object->y = title_car_record->y;
		current_object->title_car.speed = title_car_record->speed;
		current_object->frame = 9u;
		current_object->depth_layer = 0x18Fu;
		current_object->depth_layer += title_car_record->depth_layer;
		object = start_object();
		object->handler = title_car_passenger_update_ADDRESS;
		object->title_car_passenger.car = address_of(current_object);
		object->title_car_passenger.model = model;
		object = start_object();
		object->handler = title_car_driver_update_ADDRESS;
		object->title_car_driver.car = address_of(current_object);
		object->title_car_driver.model = model;
		current_object->state++;
		break;
	case 1:
		break;
	}
	current_object->title_car.position += current_object->title_car.speed;
	if (current_object->title_car.position >= 0x1000)
	{
		current_object->flags &= 0xDFu;
		current_object->title_car.position &= 0xFFFu;
	}
	current_object->x = current_object->title_car.position;
	current_object->x -= title_car_screen_offsets[screen->car_number];
	current_object->title_car.frames++;
	bounce = current_object->title_car.frames & 1u;
	current_object->title_car.rest_y = current_object->y;
	current_object->y -= bounce;
	draw_object(model);
	current_object->y = current_object->title_car.rest_y;
}

/* The driver of a title car (the blond man at the wheel): sets up its picture by the car's model
   (animation title_driver_animations, palette title_driver_palettes, x_offset title_driver_x_offsets,
   y_offset title_driver_y_offsets, a layer in front of the car), then is drawn with the car each frame
   (title_rider_draw). */
void title_car_driver_update(void)
{
	uint32_t animation = 0, model = 0;

	switch (current_object->state)
	{
	case 0:
		model = current_object->title_car_driver.model;
		current_object->palette = title_driver_palettes[model];
		current_object->title_car_driver.x_offset = title_driver_x_offsets[model];
		current_object->title_car_driver.y_offset = title_driver_y_offsets[model];
		animation = title_driver_animations[model];
		animation_start(animation);
		current_object->state++;
		current_object->draw_mode = 0xAu;
		current_object->depth_layer = OBJECT(current_object->title_car_driver.car)->depth_layer;
		current_object->depth_layer++;
		return;
	case 1:
		title_rider_draw();
		return;
	}
}

/* The passenger of a title car (the red-haired woman beside the driver): sets up its picture by the
   car's model (animation title_passenger_animations, palette title_passenger_palettes, x_offset
   title_passenger_x_offsets, y_offset title_passenger_y_offsets, a layer in front of the car), then is
   drawn with the car each frame (title_rider_draw). */
void title_car_passenger_update(void)
{
	uint32_t animation = 0, model = 0;

	switch (current_object->state)
	{
	case 0:
		break;
	case 1:
		title_rider_draw();
		return;
	}
	model = current_object->title_car_passenger.model;
	current_object->palette = title_passenger_palettes[model];
	current_object->title_car_passenger.x_offset = title_passenger_x_offsets[model];
	current_object->title_car_passenger.y_offset = title_passenger_y_offsets[model];
	animation = title_passenger_animations[model];
	animation_start(animation);
	current_object->state++;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = OBJECT(current_object->title_car_passenger.car)->depth_layer;
	current_object->depth_layer++;
}

/* Draws a rider of a title car (title_car_driver, title_car_passenger) on its car: at the car's x and y
   plus its x_offset and y_offset, bobbing with the car (bit 0 of the car's frames), at the size
   title_rider_scales gives for the model. */
void title_rider_draw(void)
{
	uint32_t y = 0;

	y = current_object->title_rider.model;
	current_object->width_scale = title_rider_scales[y];
	current_object->height_scale = title_rider_scales[y];
	current_object->x = OBJECT(current_object->title_rider.car)->x + current_object->title_rider.x_offset;
	current_object->y = OBJECT(current_object->title_rider.car)->y + current_object->title_rider.y_offset - (OBJECT(current_object->title_rider.car)->title_car.frames & 1u);
	draw_object(y);
}

/* Starts the title's palm trees: a near one (title_palm_tree with near_or_far 0) every 8th frame
   (palm_frames), else a far one (1) every other frame. */
void title_screen_start_palms(void)
{
	struct object *object = NULL;

	current_object->title_screen.palm_frames = (current_object->title_screen.palm_frames & 0xFF00u) | ((uint8_t)current_object->title_screen.palm_frames + 1u & 0xFFu);
	if ((current_object->title_screen.palm_frames & 7u) == 0u)
	{
		object = start_object();
		object->handler = title_palm_tree_update_ADDRESS;
		object->title_palm_tree.near_or_far = 0u;
		return;
	}
	if ((current_object->title_screen.palm_frames & 1u) == 0u)
	{
		object = start_object();
		object->handler = title_palm_tree_update_ADDRESS;
		object->title_palm_tree.near_or_far = 1u;
		return;
	}
}

/* A palm tree passing the title from right to left (title_screen_start_palms starts them): near
   (near_or_far 0: full size, low on the screen, 48 a frame) or far (1: 3/8 of the size, along the beach,
   18 a frame); ends once off the left edge. */
void title_palm_tree_update(void)
{
	uint32_t animation = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		animation = PALM_TREE;
		animation_start(animation);
		current_object->palette = 0x6Du;
		if (current_object->title_palm_tree.near_or_far != 0u)
		{
			current_object->width_scale = 0x60u;
			current_object->height_scale = 0x60u;
			current_object->x = 0x170u;
			current_object->y = 0x96u;
			current_object->depth_layer = 0x185u;
		}
		else
		{
			current_object->width_scale = 0x100u;
			current_object->height_scale = 0x100u;
			current_object->x = 0x170u;
			current_object->y = 0xF0u;
			current_object->depth_layer = 0x194u;
		}
		current_object->state++;
		break;
	case 1:
		break;
	}
	if ((int16_t)current_object->x <= -0x30)
	{
		current_object->flags = 0u;
		return;
	}
	if (current_object->title_palm_tree.near_or_far != 0u)
	{
		current_object->x -= 0x12u;
		draw_object(0u);
		return;
	}
	current_object->x -= 0x30u;
	draw_object(0u);
}

/* The OutRunners logo of the title: slides in from the right (16 a frame for 10 frames); then starts the
   palm trees of its board (title_board_palms), the two seagulls (title_seagull) and the title's still
   pictures (title_picture: the board, SEGA, (C) SEGA 1992), flashes through the palettes at 01AC54
   (palette_list) for 6 frames and settles in palette 269 at its next frame; then shines (its animation
   again from frame 1) each time the first logo car reaches the right edge (screen title_logo_shine). */
void title_logo_update(void)
{
	uint8_t object_index = 0;
	uint32_t animation = 0, object_index_2 = 0, objects_left = 0, palette_list = 0;
	struct object *object = NULL;
	uint8_t frames = 0, frames_2 = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		animation = TITLE_LOGO;
		animation_start(animation);
		current_object->palette = 0x26Au;
		current_object->x = 0x142u;
		current_object->y = 0x47u;
		current_object->depth_layer = 0x199u;
		current_object->title_logo.frames = 0xBu;
		current_object->state++;
		break;
	case 1:
		break;
	case 2:
		frames = current_object->title_logo.frames;
		current_object->title_logo.frames = frames - 1u;
		if (frames == 1u)
		{
			current_object->state++;
			current_object->palette = 0x269u;
			animation_advance(0u);
			draw_object(0u);
			return;
		}
		palette_list = current_object->title_logo.palette_list;
		current_object->palette = TITLE_LOGO_PALETTES(palette_list)[0];
		current_object->title_logo.palette_list = palette_list + 2u;
		draw_object(0u);
		return;
	case 3:
		if (screen->title_logo_shine != 0u)
		{
			current_object->frame = 1u;
			current_object->frame_time = 2u;
			current_object->frame_countdown = 2;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
	}
	frames_2 = current_object->title_logo.frames;
	current_object->title_logo.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		current_object->x -= 0x10u;
		draw_object(0u);
		return;
	}
	object = start_object();
	object->handler = title_board_palms_update_ADDRESS;
	objects_left = 2u;
	object_index = 0u;
	do
	{
		object = start_object();
		object->handler = title_seagull_fly_ADDRESS;
		object->title_seagull.index = object_index;
		object_index++;
		objects_left--;
	} while (objects_left != 0u);
	objects_left = 3u;
	object_index_2 = 0u;
	do
	{
		object = start_object();
		object->handler = title_picture_draw_ADDRESS;
		object->title_picture.index = object_index_2;
		object_index_2 = (uint8_t)(object_index_2 + 1u);
		objects_left--;
	} while (objects_left != 0u);
	current_object->title_logo.palette_list = title_logo_flash_palettes_ADDRESS;
	current_object->title_logo.frames = 6u;
	current_object->state++;
	draw_object(object_index_2);
}

/* A copy of the logo in other colours (cyan) that slides in from the left as the logo (title_logo)
   slides in from the right, meets it, flashes through the palettes at 01AC62 (palette_list) and ends. */
void title_logo_copy_update(void)
{
	uint32_t animation = 0, palette_list = 0;
	uint8_t frames = 0, frames_2 = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		animation = TITLE_LOGO_COPY;
		animation_start(animation);
		current_object->palette = 0x26Du;
		current_object->x = 2u;
		current_object->y = 0x47u;
		current_object->depth_layer = 0x199u;
		current_object->title_logo_copy.frames = 0xBu;
		current_object->state++;
		break;
	case 1:
		break;
	case 2:
		frames = current_object->title_logo_copy.frames;
		current_object->title_logo_copy.frames = frames - 1u;
		if (frames == 1u)
		{
			current_object->flags = 0u;
			return;
		}
		palette_list = current_object->title_logo_copy.palette_list;
		current_object->palette = TITLE_LOGO_PALETTES(palette_list)[0];
		current_object->title_logo_copy.palette_list = palette_list + 2u;
		draw_object(0u);
		return;
	}
	frames_2 = current_object->title_logo_copy.frames;
	current_object->title_logo_copy.frames = frames_2 - 1u;
	if (frames_2 == 1u)
	{
		current_object->title_logo_copy.palette_list = title_logo_copy_flash_palettes_ADDRESS;
		current_object->title_logo_copy.frames = 6u;
		current_object->state++;
		draw_object(0u);
		return;
	}
	current_object->x += 0x10u;
	draw_object(0u);
}

/* A small car racing across the logo along one of its lines (title_screen starts three at a time, index
   0-2): after 1, 5 or 9 frames by index, one of the eight cars at random (title_car_animations,
   title_logo_car_palettes) at a quarter of its size at the left edge; 16 a frame to the right edge; the
   first of the three then makes the logo, its board's palm trees and the lines shine (screen
   title_logo_shine for a frame), and it ends. */
void title_logo_car_update(void)
{
	uint32_t animation = 0, model = 0;
	const struct title_place *place = NULL;
	uint8_t delay = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->title_logo_car.delay = ((uint32_t)current_object->title_logo_car.index << 2) + 1u;
		current_object->state++;
		delay = current_object->title_logo_car.delay;
		current_object->title_logo_car.delay = delay - 1u;
		if (delay != 1u)
			return;
		current_object->draw_mode = 0xAu;
		model = random_number();
		model &= 7u;
		animation = title_car_animations[model];
		animation_start(animation);
		current_object->palette = title_logo_car_palettes[model];
		place = rom_pointer(title_logo_car_places_ADDRESS + ((uint32_t)current_object->title_logo_car.index << 2));
		current_object->x = place->x;
		current_object->y = place->y;
		current_object->width_scale = 0x40u;
		current_object->height_scale = 0x40u;
		current_object->frame = 9u;
		current_object->depth_layer = 0x19Au;
		current_object->state++;
		draw_object(model);
		return;
	case 1:
		delay = current_object->title_logo_car.delay;
		current_object->title_logo_car.delay = delay - 1u;
		if (delay != 1u)
			return;
		current_object->draw_mode = 0xAu;
		model = random_number();
		model &= 7u;
		animation = title_car_animations[model];
		animation_start(animation);
		current_object->palette = title_logo_car_palettes[model];
		place = rom_pointer(title_logo_car_places_ADDRESS + ((uint32_t)current_object->title_logo_car.index << 2));
		current_object->x = place->x;
		current_object->y = place->y;
		current_object->width_scale = 0x40u;
		current_object->height_scale = 0x40u;
		current_object->frame = 9u;
		current_object->depth_layer = 0x19Au;
		current_object->state++;
		draw_object(model);
		return;
	case 2:
		current_object->x += 0x10u;
		if ((int16_t)current_object->x <= 0x148)
		{
			draw_object(0u);
			return;
		}
		if (current_object->title_logo_car.index == 0u)
			screen->title_logo_shine = 1u;
		current_object->state++;
		draw_object(0u);
		return;
	case 3:
		screen->title_logo_shine = 0u;
		current_object->flags = 0u;
		return;
	}
}

/* One of the three thin coloured lines across the logo that the logo cars race along (index 0-2): after
   1, 5 or 9 frames by index it slides in from the left (16 a frame for 19 frames), then shines with the
   logo (its animation from frame 0 when screen title_logo_shine is set). */
void title_logo_stripe_update(void)
{
	uint32_t animation = 0;
	const struct title_place *place = NULL;
	uint8_t frames = 0, frames_2 = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->title_logo_stripe.frames = ((uint32_t)current_object->title_logo_stripe.index << 2) + 1u;
		current_object->state++;
		break;
	case 1:
		break;
	case 2:
		frames = current_object->title_logo_stripe.frames;
		current_object->title_logo_stripe.frames = frames - 1u;
		if (frames == 1u)
		{
			current_object->draw_mode = 0xAu;
			current_object->state++;
			draw_object(0u);
			return;
		}
		current_object->x += 0x10u;
		draw_object(0u);
		return;
	case 3:
		if (screen->title_logo_shine != 0u)
		{
			current_object->frame = 0u;
			current_object->frame_time = 2u;
			current_object->frame_countdown = 2;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
	}
	frames_2 = current_object->title_logo_stripe.frames;
	current_object->title_logo_stripe.frames = frames_2 - 1u;
	if (frames_2 != 1u)
		return;
	current_object->draw_mode = 0xAu;
	animation = TITLE_LOGO_STRIPE;
	animation_start(animation);
	current_object->palette = 0x270u;
	place = rom_pointer(title_logo_stripe_places_ADDRESS + ((uint32_t)current_object->title_logo_stripe.index << 2));
	current_object->x = place->x;
	current_object->y = place->y;
	current_object->depth_layer = 0x198u;
	current_object->title_logo_stripe.frames = 0x14u;
	current_object->state++;
	frames = current_object->title_logo_stripe.frames;
	current_object->title_logo_stripe.frames = frames - 1u;
	if (frames == 1u)
	{
		current_object->draw_mode = 0xAu;
		current_object->state++;
		draw_object(0u);
		return;
	}
	current_object->x += 0x10u;
	draw_object(0u);
}

/* The palm trees at the right end of the logo's board: grow in from 3/4 of their size (8 frames), then
   sway each time the logo shines (screen title_logo_shine). */
void title_board_palms_update(void)
{
	uint32_t animation = 0;
	uint8_t frames = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		animation = TITLE_BOARD_PALMS;
		animation_start(animation);
		current_object->palette = 0x272u;
		current_object->width_scale = 0xC0u;
		current_object->height_scale = 0xC0u;
		current_object->x = 0xE2u;
		current_object->y = 0x4Eu;
		current_object->depth_layer = 0x198u;
		current_object->title_board_palms.frames = 9u;
		current_object->state++;
		break;
	case 1:
		break;
	case 2:
		if (screen->title_logo_shine != 0u)
		{
			current_object->frame = 0u;
			current_object->frame_time = 3u;
			current_object->frame_countdown = 6;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
	}
	frames = current_object->title_board_palms.frames;
	current_object->title_board_palms.frames = frames - 1u;
	if (frames == 1u)
	{
		current_object->draw_mode = 0xAu;
		current_object->state++;
		draw_object(0u);
		return;
	}
	current_object->width_scale += 8u;
	current_object->height_scale += 8u;
	draw_object(0u);
}

/* One of the two seagulls over the title (index 0, 1): flies up out of the logo for 8 frames growing to
   full size, then circles about that place in a figure of eight (the sine of x_angle, a turn in 128
   frames, and of y_angle, twice as fast), flapping (its animation). */
void title_seagull_fly(void)
{
	uint16_t y_wave = 0;
	uint32_t angle = 0, animation = 0;
	const struct title_place *place = NULL;
	uint8_t frames = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		animation = TITLE_SEAGULL;
		animation_start(animation);
		current_object->palette = 0x272u;
		current_object->width_scale = 0xC0u;
		current_object->height_scale = 0xC0u;
		place = rom_pointer(title_seagull_places_ADDRESS + ((uint32_t)current_object->title_seagull.index << 2));
		current_object->x = place->x;
		current_object->y = place->y;
		current_object->depth_layer = 0x198u;
		current_object->title_seagull.frames = 9u;
		current_object->state++;
		frames = current_object->title_seagull.frames;
		current_object->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			current_object->draw_mode = 0xAu;
			angle = (uint16_t)((uint32_t)current_object->title_seagull.index << 11);
			current_object->title_seagull.x_angle = angle;
			current_object->title_seagull.y_angle = angle;
			current_object->title_seagull.centre_x = current_object->x;
			current_object->title_seagull.centre_y = current_object->y;
			current_object->state++;
			animation_advance(0u);
			draw_object(angle);
			return;
		}
		current_object->width_scale += 8u;
		current_object->height_scale += 8u;
		current_object->x--;
		current_object->y -= 5u;
		animation_advance(0u);
		draw_object(0u);
		return;
	case 1:
		frames = current_object->title_seagull.frames;
		current_object->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			current_object->draw_mode = 0xAu;
			angle = (uint16_t)((uint32_t)current_object->title_seagull.index << 11);
			current_object->title_seagull.x_angle = angle;
			current_object->title_seagull.y_angle = angle;
			current_object->title_seagull.centre_x = current_object->x;
			current_object->title_seagull.centre_y = current_object->y;
			current_object->state++;
			animation_advance(0u);
			draw_object(angle);
			return;
		}
		current_object->width_scale += 8u;
		current_object->height_scale += 8u;
		current_object->x--;
		current_object->y -= 5u;
		animation_advance(0u);
		draw_object(0u);
		return;
	case 2:
		current_object->title_seagull.x_angle += 0x20u;
		if (current_object->title_seagull.x_angle >= 0x1000)
			current_object->title_seagull.x_angle -= 0x1000u;
		current_object->title_seagull.y_angle += 0x40u;
		if (current_object->title_seagull.y_angle >= 0x1000)
			current_object->title_seagull.y_angle -= 0x1000u;
		current_object->x = current_object->title_seagull.centre_x;
		current_object->y = current_object->title_seagull.centre_y;
		y_wave = (uint16_t)((int16_t)sine_table[(uint32_t)current_object->title_seagull.y_angle] >> 12);
		current_object->x += (int16_t)sine_table[current_object->title_seagull.x_angle & 0xFFFu] >> 9;
		current_object->y += y_wave;
		animation_advance(0u);
		draw_object(0u);
		return;
	}
}

/* One of the title's still pictures (title_logo starts them): by index, from its record at 01AC70
   (animation, palette, width_scale, height_scale, x, y, depth_layer): 0 (C) SEGA 1992, 1 the oval board
   behind the logo, 2 the SEGA logo. */
void title_picture_draw(void)
{
	uint32_t animation = 0;
	const struct picture_record *picture = NULL;

	switch (current_object->state)
	{
	case 1:
		draw_object(0u);
		return;
	case 0:
		break;
	}
	current_object->draw_mode = 0u;
	picture = rom_pointer(title_pictures_ADDRESS + ((uint32_t)current_object->title_picture.index << 4));
	animation = picture->animation;
	animation_start(animation);
	current_object->palette = picture->palette;
	current_object->width_scale = picture->width_scale;
	current_object->height_scale = picture->height_scale;
	current_object->x = picture->x;
	current_object->y = picture->y;
	current_object->depth_layer = picture->depth_layer;
	current_object->state++;
	draw_object(0u);
}
