/**
 *    Name:        objects/backgrounds.c
 *    Description: the backgrounds: their layers and animations, the palette cycles, the sunset, the backdrops
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The scenery behind the road (the race's first tilemap). By state: 0 sets it up: its register copies
   (race_background_layer_setups by screen: scroll_x_address, scroll_y_address, tile_bank_address,
   page_word_address), the stage's background (background: stage_backgrounds by screen first_stage, none
   for stage 0), its tile pages (background_tile_page_lists, loaded at once with unpack_tile_pages), tile
   bank and palettes (race_background_fade_palettes, queue_palette_list), its objects
   (race_background_start_objects) and, for stages 6, 8 and 1C, its animation
   (race_background_start_lower_animation, race_background_start_upper_animation); 1 waits for a change:
   a fork's new stage (the car's new_branch; background 2 * screen built_stage + the branch, car branch)
   or the road's events 4 and 5 (screen stage_background_change; background 2A + built_stage): screen
   background_changing set, the new background's pages queued into the other half of the pages
   (page_half; queue_tile_pages); 2 changes over as the view turns away and back (the car's view_yaw;
   change_step): when it starts its palettes, tile bank and the first page word (next_page_words), at 2A0
   (off the screen) the fades of background_leaving_fades and of the road (background_road_palettes, into
   screen road_palette_fades) and the next, the new stage's objects (screen route_stage), at 160 its
   palette fades (race_background_fade_palettes) and the next, at 60 the last, at 0 back to state 1 with
   the animation of backgrounds 5, 7, 8, 25 and 26. Each frame scrolls it: x by the car's view_yaw (a
   quarter of it, all of it during a change) less A0, y by the slope (the car's road_pitch) and screen
   horizon_line, kept between 0 and 180 less the road's top line (screen road_top_line; also into screen
   background_scroll_y). */
void race_background_update(void)
{
	uint32_t zero = 0;
	uint8_t background_2 = 0, branch = 0, first_stage_2 = 0;
	uint16_t scroll_x = 0, scroll_y = 0, scroll_y_limit = 0, view_turn = 0;
	uint32_t animation = 0, background = 0, background_inverted = 0, first_stage = 0, index = 0, mixer_bits = 0, new_background = 0, page_count = 0, page_half = 0, page_number = 0, page_word_set = 0, palettes = 0, route_stage = 0, tile_page_list = 0;
	const uint32_t *next_page_words = NULL;
	uint32_t *mixer_address = NULL;
	const struct race_background_layer_setup *race_background_layer_setup = NULL;
	const struct road_palette_record *fades = NULL;
	const uint32_t *tile_pages = NULL;
	struct tilemap_pages *page_word_address = NULL;
	uint32_t view_turn_kept = 0;
	uint8_t first_background = 0;
	uint16_t signed_turn = 0, turn_back = 0, slope_scroll_y = 0;

	switch (current_object->state)
	{
	case 0:
		index = screen->index;
		current_object->race_background.page_half = index;
		race_background_layer_setup = rom_pointer(race_background_layer_setups_ADDRESS + index * 5u * 4u);
		current_object->race_background.scroll_x_address = race_background_layer_setup->scroll_x_address;
		current_object->race_background.scroll_y_address = race_background_layer_setup->scroll_y_address;
		current_object->race_background.tile_bank_address = race_background_layer_setup->tile_bank_address;
		page_word_address = pointer_to(race_background_layer_setup->page_word_address);
		current_object->race_background.page_word_address = address_of(page_word_address);
		page_word_address->pages = background_page_words[(uint32_t)current_object->race_background.page_half << 3];
		mixer_address = pointer_to(race_background_layer_setup->mixer_address);
		mixer_bits = mixer_address[0];
		mixer_bits = mixer_bits & ~0xFu;
		mixer_address[0] = mixer_bits | 4u;
		screen->flags |= 4u;
		background_inverted = screen->first_stage;
		if (background_inverted != 0u)
			background_inverted = (uint8_t)~stage_backgrounds[background_inverted];
		first_background = ~background_inverted;
		current_object->race_background.background = first_background;
		tile_page_list = address_of(current_object) + 0x2Cu;
		tile_pages = &no_background_tile_pages;
		if ((int8_t)first_background >= 0)
			tile_pages = rom_pointer(background_tile_page_lists[(uint8_t)~background_inverted] + 0x10u);
		r12 = tile_page_list;
		page_number = current_object->race_background.page_half * 5u * 2u + 1u;
		page_count = 4u;
		do
		{
			OBJECT_TILE_PAGES(r12)[0] = byte_swap32(page_number) | tile_pages[0];
			r12 += 4u;
			page_number++;
			page_count--;
			tile_pages++;
		} while (page_count != 0u);
		OBJECT_TILE_PAGES(r12)[0] = 0xFFFFFFFFu;
		r12 = unpack_tile_pages(tile_page_list, r12);
		TILEMAP_BANKS(current_object->race_background.tile_bank_address)->tile_bank = stage_tile_banks[screen->first_stage];
		screen->background_changing = 0u;
		screen->stage_background_change = 0u;
		screen->backdrops_leaving = 0u;
		screen->background_fading = 0xFFu;
		screen->background_at_start = 0xFFu;
		first_stage = screen->first_stage;
		race_background_start_objects(first_stage);
		palettes = no_background_palettes_ADDRESS;
		background = current_object->race_background.background;
		if ((int8_t)background >= 0)
		{
			fades = rom_pointer(background_palette_fades[background]);
			race_background_fade_palettes(address_of(fades));
			palettes = background_palettes[current_object->race_background.background];
		}
		queue_palette_list(palettes);
		current_object->state++;
		first_stage_2 = screen->first_stage;
		if (first_stage_2 == 6u)
			race_background_start_lower_animation();
		else
		{
			animation = 0u;
			if (first_stage_2 == 8u)
				race_background_start_upper_animation(animation);
			else
			{
				animation = 1u;
				if (first_stage_2 == 0x1Cu)
					race_background_start_upper_animation(animation);
			}
		}
	case 1:
		if (screen->stage_background_change == 0u)
		{
			if (player_car->new_branch == 0u)
				break;
			player_car->new_branch = 0u;
		}
		screen->background_changing = 0xFFu;
		new_background = screen->built_stage;
		if (screen->stage_background_change == 0u)
		{
			new_background = new_background * 2u;
			new_background = (new_background & ~0xFFu) | (uint8_t)(new_background + player_car->branch);
		}
		else
			new_background = (uint8_t)(new_background + 0x2Au);
		current_object->race_background.background = new_background;
		tile_page_list = address_of(current_object) + 0x2Cu;
		tile_pages = rom_pointer(background_tile_page_lists[new_background]);
		r12 = tile_page_list;
		page_half = current_object->race_background.page_half;
		page_half = page_half ^ 2u;
		current_object->race_background.page_half = page_half;
		page_half = page_half * 5u * 2u + 1u;
		page_count = 9u;
		do
		{
			OBJECT_TILE_PAGES(r12)[0] = byte_swap32(page_half) | tile_pages[0];
			r12 += 4u;
			page_half++;
			page_count--;
			tile_pages++;
		} while (page_count != 0u);
		OBJECT_TILE_PAGES(r12)[0] = 0xFFFFFFFFu;
		queue_tile_pages(tile_page_list);
		page_word_set = current_object->race_background.page_half * 2u;
		branch = screen->stage_background_branch;
		if (screen->stage_background_change != 0u)
		{
			current_object->race_background.next_page_words = background_page_words_ADDRESS + ((page_word_set & ~0xFFu) | (uint8_t)(page_word_set + branch)) * 2u * 2u * 4u;
			current_object->state++;
			current_object->race_background.change_step = 0u;
			break;
		}
		branch = player_car->branch;
		current_object->race_background.next_page_words = background_page_words_ADDRESS + ((page_word_set & ~0xFFu) | (uint8_t)(page_word_set + branch)) * 2u * 2u * 4u;
		current_object->state++;
		current_object->race_background.change_step = 0u;
		break;
	case 2:
		signed_turn = (int16_t)((uint32_t)player_car->view_yaw << 4) >> 4;
		zero = signed_turn == 0u;
		view_turn = signed_turn;
		if ((int16_t)signed_turn < 0)
		{
			turn_back = 0u - view_turn;
			zero = turn_back == 0u;
			view_turn = turn_back;
		}
		switch (current_object->race_background.change_step)
		{
		case 0:
			if (zero)
				break;
			screen->background_fading = 0xFFu;
			background = current_object->race_background.background;
			palettes = background_palettes[background];
			queue_palette_list(palettes);
			TILEMAP_BANKS(current_object->race_background.tile_bank_address)->tile_bank = background_tile_banks[background];
			next_page_words = rom_pointer(current_object->race_background.next_page_words);
			TILEMAP_PAGES(current_object->race_background.page_word_address)->pages = next_page_words[0];
			current_object->race_background.next_page_words = address_of(next_page_words + 1);
			current_object->race_background.change_step++;
			break;
		case 1:
			if ((int16_t)view_turn < 0x2A0)
				break;
			screen->backdrops_leaving = 0xFFu;
			fades = rom_pointer(background_leaving_fades[current_object->race_background.background]);
			start_palette_fades(address_of(fades));
			fades = rom_pointer(background_road_palettes[current_object->race_background.background]);
			if (fades == NULL)
			{
				next_page_words = rom_pointer(current_object->race_background.next_page_words);
				TILEMAP_PAGES(current_object->race_background.page_word_address)->pages = next_page_words[0];
				current_object->race_background.next_page_words = address_of(next_page_words + 1);
				current_object->race_background.change_step++;
				break;
			}
			screen->road_palette_fades = fades->road_fades;
			fades = fades + 1;
			start_palette_fades(address_of(fades));
			next_page_words = rom_pointer(current_object->race_background.next_page_words);
			TILEMAP_PAGES(current_object->race_background.page_word_address)->pages = next_page_words[0];
			current_object->race_background.next_page_words = address_of(next_page_words + 1);
			current_object->race_background.change_step++;
			break;
		case 2:
			view_turn_kept = view_turn;
			screen->backdrops_leaving = 0u;
			screen->background_at_start = 0u;
			route_stage = screen->route_stage;
			race_background_start_objects(route_stage);
			view_turn = (uint16_t)view_turn_kept;
			current_object->race_background.change_step++;
		case 3:
			if ((int16_t)view_turn > 0x160)
				break;
			fades = rom_pointer(background_palette_fades[current_object->race_background.background]);
			race_background_fade_palettes(address_of(fades));
			next_page_words = rom_pointer(current_object->race_background.next_page_words);
			TILEMAP_PAGES(current_object->race_background.page_word_address)->pages = next_page_words[0];
			current_object->race_background.next_page_words = address_of(next_page_words + 1);
			current_object->race_background.change_step++;
			break;
		case 4:
			if ((int16_t)view_turn > 0x60)
				break;
			next_page_words = rom_pointer(current_object->race_background.next_page_words);
			TILEMAP_PAGES(current_object->race_background.page_word_address)->pages = next_page_words[0];
			current_object->race_background.next_page_words = address_of(next_page_words + 1);
			current_object->race_background.change_step++;
			break;
		case 5:
			if (zero == 0u)
				break;
			screen->background_changing = 0u;
			screen->stage_background_change = 0u;
			current_object->state--;
			background_2 = current_object->race_background.background;
			if (background_2 == 5u)
			{
				race_background_start_lower_animation();
				break;
			}
			animation = 0u;
			if (background_2 == 7u || background_2 == 8u)
			{
				race_background_start_upper_animation(animation);
				break;
			}
			animation = 2u;
			if (background_2 == 0x25u)
			{
				race_background_start_upper_animation(animation);
				break;
			}
			animation = 1u;
			if (background_2 != 0x26u)
				break;
			race_background_start_upper_animation(animation);
			break;
		}
		break;
	}
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	VIDEO_REGISTER_WORDS(current_object->race_background.scroll_x_address)[0] = scroll_x - 0xA0u;
	slope_scroll_y = ((int16_t)sine_table[(uint32_t)(int16_t)player_car->road_pitch] >> 6) + screen->horizon_line - 1u;
	scroll_y = slope_scroll_y;
	if ((int16_t)slope_scroll_y >= 0)
	{
		scroll_y_limit = (uint16_t)(0u - screen->road_top_line + 0x180u);
		if ((int16_t)scroll_y_limit < (int16_t)scroll_y)
			scroll_y = (uint16_t)scroll_y_limit;
	}
	else
		scroll_y = 0u;
	screen->background_scroll_y = scroll_y;
	VIDEO_REGISTER_WORDS(current_object->race_background.scroll_y_address)[0] = scroll_y;
}

/* Loads the tiles of the background's lower animation (queue_tile_pages with
   background_lower_animation_tile_pages by screen) and starts it (race_background_lower_animation). */
void race_background_start_lower_animation(void)
{
	uint32_t tile_pages = 0;
	struct object *object = NULL;

	tile_pages = background_lower_animation_tile_pages[screen->index];
	queue_tile_pages(tile_pages);
	object = start_object();
	object->race_background_lower_animation.delay = 2u;
	object->race_background_lower_animation.step = screen->index;
	object->race_background_lower_animation.page_word_address = current_object->race_background.page_word_address;
	object->handler = race_background_lower_animation_wait_ADDRESS;
}

/* Waits until the animation's tiles are loaded (screen unpack_state), then goes on to
   race_background_lower_animation_cycle. */
void race_background_lower_animation_wait(void)
{
	if (screen->unpack_state != 0u)
		return;
	current_object->handler = race_background_lower_animation_cycle_ADDRESS;
	race_background_lower_animation_cycle();
}

/* Every 2 frames (delay) puts the next of four page pairs (background_lower_animation_page_pairs; step
   goes two on, keeping to the screen's own) into the second half of the background's page word (the
   lower pages); ends when a change of background starts (screen background_changing). */
void race_background_lower_animation_cycle(void)
{
	uint32_t step = 0;
	uint8_t delay = 0;

	if (screen->background_changing != 0u)
	{
		current_object->flags = 0u;
		return;
	}
	delay = current_object->race_background_lower_animation.delay;
	current_object->race_background_lower_animation.delay = delay - 1u;
	if (delay == 1u)
	{
		current_object->race_background_lower_animation.delay = 2u;
		step = current_object->race_background_lower_animation.step;
		step = (uint8_t)(step + 2u);
		step = step & 7u;
		current_object->race_background_lower_animation.step = step;
		TILEMAP_PAGES(current_object->race_background_lower_animation.page_word_address)->lower_pages = background_lower_animation_page_pairs[step];
	}
}

/* Loads the tiles of upper animation R0 (queue_tile_pages with background_upper_animation_tile_pages by
   R0 and screen) and starts it (race_background_upper_animation). */
void race_background_start_upper_animation(uint32_t animation)
{
	uint32_t tile_pages = 0;
	struct object *object = NULL;

	animation = animation * 2u;
	tile_pages = background_upper_animation_tile_pages[(uint8_t)(animation + screen->index)];
	queue_tile_pages(tile_pages);
	object = start_object();
	object->race_background_upper_animation.delay = 3u;
	object->race_background_upper_animation.step = screen->index;
	object->race_background_upper_animation.page_word_address = current_object->race_background.page_word_address;
	object->handler = race_background_upper_animation_wait_ADDRESS;
}

/* Waits until the animation's tiles are loaded (screen unpack_state), then goes on to
   race_background_upper_animation_cycle. */
void race_background_upper_animation_wait(void)
{
	if (screen->unpack_state != 0u)
		return;
	current_object->handler = race_background_upper_animation_cycle_ADDRESS;
	race_background_upper_animation_cycle();
}

/* Every 3 frames (delay) puts the next of three page pairs (background_upper_animation_page_pairs) into
   the first half of the background's page word (the upper pages: the schools of fish over the underwater
   tunnel swim); ends when a change of background starts (screen background_changing). */
void race_background_upper_animation_cycle(void)
{
	uint32_t step = 0;
	uint8_t delay = 0;

	if (screen->background_changing != 0u)
	{
		current_object->flags = 0u;
		return;
	}
	delay = current_object->race_background_upper_animation.delay;
	current_object->race_background_upper_animation.delay = delay - 1u;
	if (delay != 1u)
		return;
	current_object->race_background_upper_animation.delay = 3u;
	step = current_object->race_background_upper_animation.step;
	step = (uint8_t)(step + 2u);
	if ((int8_t)step >= 6)
		step = (uint8_t)(step - 6u);
	current_object->race_background_upper_animation.step = step;
	TILEMAP_PAGES(current_object->race_background_upper_animation.page_word_address)->upper_pages = background_upper_animation_page_pairs[step];
}

/* Sets up the screen's two tilemaps (0 and 2 for screen 1, 1 and 3 for screen 2; their register copies
   listed at background_register_lists by screen) from the 20-byte record background_setup (R19): their
   pages (two words), their tile banks (two bytes) and their priorities in the mixer (two bytes, the low
   four bits of mixer registers 22 and 26, 24 and 28 for screen 2; 0 is off), zoom 1:1, zoom centre and
   scrolls 0, the second tilemap's row scroll and row select off (video_registers[2]), the registers and
   the mixer to be sent (send_video_registers, screen flags bit 2); then queues the pages of the list at
   the record's +C (queue_tile_pages) and the palette record at +10 (queue_palette_list). Used by
   car_presentation, name_entry, route_map, car_select_run, entry_countdown_start and
   title_screen_update. */
void set_up_background(uint32_t background_setup)
{
	uint32_t mixer_bits = 0, palette_list = 0, tile_pages = 0;
	const struct background_registers *background_registers = NULL;
	uint32_t *mixer_address = NULL;
	struct tilemap_banks *tile_banks_address = NULL;
	struct tilemap_pages *pages_address = NULL;
	uint8_t tile_bank = 0, second_tile_bank = 0;
	uint32_t pages = 0, second_pages = 0;

	background_registers = rom_pointer(background_register_lists[screen->index]);
	pages_address = pointer_to(background_registers->pages_address);
	pages = BACKGROUND_SETUP(background_setup)->pages;
	background_setup += 4u;
	pages_address->pages = pages;
	second_pages = BACKGROUND_SETUP(background_setup - 0x4u)->second_pages;
	background_setup += 4u;
	pages_address->second_pages = second_pages;
	tile_banks_address = pointer_to(background_registers->tile_banks_address);
	tile_bank = BACKGROUND_SETUP(background_setup - 0x8u)->tile_bank;
	background_setup++;
	tile_banks_address->tile_bank = tile_bank;
	second_tile_bank = BACKGROUND_SETUP(background_setup - 0x9u)->second_tile_bank;
	background_setup++;
	tile_banks_address->second_tile_bank = second_tile_bank;
	VIDEO_REGISTER_WORDS(background_registers->zoom_x_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(background_registers->zoom_y_address)[0] = 0x200u;
	VIDEO_REGISTER_WORDS(background_registers->zoom_centre_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(background_registers->zoom_centre_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(background_registers->scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(background_registers->scroll_y_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(background_registers->second_scroll_x_address)[0] = 0u;
	VIDEO_REGISTER_WORDS(background_registers->second_scroll_y_address)[0] = 0u;
	game->video_registers[2] &= background_registers->video_mask;
	game->send_video_registers = 0xFFu;
	mixer_address = pointer_to(background_registers->mixer_address);
	mixer_bits = mixer_address[0];
	mixer_bits = mixer_bits & ~0xFu;
	mixer_address[0] = mixer_bits | BACKGROUND_SETUP(background_setup - 0xAu)->priority;
	mixer_address++;
	mixer_bits = mixer_address[0];
	mixer_bits = mixer_bits & ~0xFu;
	background_setup++;
	mixer_address[0] = mixer_bits | BACKGROUND_SETUP(background_setup - 0xBu)->second_priority;
	screen->flags |= 4u;
	background_setup++;
	tile_pages = BACKGROUND_SETUP(background_setup - 0xCu)->tile_pages;
	queue_tile_pages(tile_pages);
	palette_list = BACKGROUND_SETUP(background_setup - 0xCu)->palette_list;
	queue_palette_list(palette_list);
}

/* Starts the background objects of stage R0 (stage_background_object_lists; R0 + 1F during a change
   within the stage, screen stage_background_change), each with its data (field 80) and this object (14). */
void race_background_start_objects(uint32_t stage)
{
	uint32_t object_list = 0, objects_left = 0;
	struct object *object = NULL;
	uint32_t entry_count = 0;

	if (screen->stage_background_change != 0u)
		stage = (uint8_t)(stage + 0x1Fu);
	object_list = stage_background_object_lists[stage];
	entry_count = OBJECT_START_LIST(object_list)->count >> 3;
	objects_left = entry_count;
	object_list += 4u;
	if (entry_count != 0u)
	{
		do
		{
			object = start_object();
			object->handler = OBJECT_LIST_PAIR(object_list)->handler;
			object_list += 4u;
			object->list_data = OBJECT_LIST_PAIR(object_list - 0x4u)->data;
			object->parent = address_of(current_object);
			objects_left--;
			object_list += 4u;
		} while (objects_left != 0u);
	}
}

/* Cycles palettes of the stage's scenery (stage_background_object_lists): after delay frames, every
   step_frames frames queues the next palette set of its list (palette_list; a negative entry goes back
   that many bytes) with queue_palette_list, for frames_left frames (or for good when it is 0); it waits
   while the stage's background is changing (screen background_fading) and ends when it changes again
   (running). */
void palette_cycle_update(void)
{
	uint16_t step_frames = 0;
	uint32_t palettes = 0;
	const struct palette_cycle_record *palette_cycle_record = NULL;
	const uint32_t *palette_list = NULL;
	uint16_t delay = 0, frames_left = 0, step_countdown = 0;

	switch (current_object->state)
	{
	case 0:
		palette_cycle_record = rom_pointer(current_object->palette_cycle.data);
		current_object->palette_cycle.delay = palette_cycle_record->delay;
		current_object->palette_cycle.frames_left = palette_cycle_record->frames_left;
		current_object->palette_cycle.palette_list = palette_cycle_record->palette_list;
		step_frames = (uint16_t)palette_cycle_record->step_frames;
		current_object->palette_cycle.step_frames = step_frames;
		current_object->palette_cycle.step_countdown = step_frames;
		current_object->palette_cycle.running = 0u;
		current_object->state++;
		if (current_object->palette_cycle.delay != 0u)
		{
			delay = current_object->palette_cycle.delay;
			current_object->palette_cycle.delay = delay - 1u;
			if (delay != 1u)
				return;
		}
		current_object->state++;
		if (current_object->palette_cycle.frames_left != 0u)
		{
			frames_left = current_object->palette_cycle.frames_left;
			current_object->palette_cycle.frames_left = frames_left - 1u;
			if (frames_left == 1u)
			{
				current_object->flags = 0u;
				return;
			}
		}
		else
			current_object->state++;
		break;
	case 1:
		delay = current_object->palette_cycle.delay;
		current_object->palette_cycle.delay = delay - 1u;
		if (delay != 1u)
			return;
		current_object->state++;
		if (current_object->palette_cycle.frames_left != 0u)
		{
			frames_left = current_object->palette_cycle.frames_left;
			current_object->palette_cycle.frames_left = frames_left - 1u;
			if (frames_left == 1u)
			{
				current_object->flags = 0u;
				return;
			}
		}
		else
			current_object->state++;
		break;
	case 2:
		frames_left = current_object->palette_cycle.frames_left;
		current_object->palette_cycle.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			current_object->flags = 0u;
			return;
		}
		break;
	case 3:
		break;
	}
	switch (current_object->palette_cycle.running)
	{
	case 0:
		if (screen->background_fading != 0u)
			return;
		current_object->palette_cycle.running++;
		break;
	case 1:
		break;
	}
	if (screen->background_fading != 0u)
	{
		current_object->flags = 0u;
		return;
	}
	step_countdown = current_object->palette_cycle.step_countdown;
	current_object->palette_cycle.step_countdown = step_countdown - 1u;
	if (step_countdown != 1u)
		return;
	current_object->palette_cycle.step_countdown = current_object->palette_cycle.step_frames;
	palette_list = rom_pointer(current_object->palette_cycle.palette_list);
	palettes = palette_list[0];
	palette_list++;
	if ((int32_t)palettes < 0)
	{
		palette_list = rom_pointer(address_of(palette_list) + palettes);
		palettes = palette_list[0];
		palette_list++;
	}
	current_object->palette_cycle.palette_list = address_of(palette_list);
	queue_palette_list(palettes);
}

/* Fades the colours of the stage toward evening after a while (stage_background_object_lists: Hawaii's
   sunset after 900 frames, stage 26's dusk after 300 and its night after 600): waits count frames (not
   while the background changes, screen background_fading), then starts a palette fade
   (start_palette_fade) for each entry of its list (fade_list: from, to, the palette, the speed), keeps
   them in fades and their number in count, and goes on to sunset_fade_wait. */
void sunset_fade_start(void)
{
	uint8_t blocks = 0, period = 0;
	uint32_t fade = 0, fade_object = 0, fades_left = 0, from = 0, to = 0;
	uint32_t *fade_slot = NULL;
	const struct sunset_fade_record *sunset_fade_record = NULL;
	uint32_t count = 0, fade_count = 0;

	switch (current_object->state)
	{
	case 0:
		sunset_fade_record = rom_pointer(current_object->sunset_fade.data);
		current_object->sunset_fade.count = sunset_fade_record->delay;
		current_object->sunset_fade.fade_list = sunset_fade_record->fade_list;
		current_object->state++;
		if ((uint16_t)current_object->sunset_fade.count != 0u)
		{
			count = current_object->sunset_fade.count;
			current_object->sunset_fade.count = count - 1u;
			if (count != 1u)
				return;
		}
		current_object->state++;
		break;
	case 1:
		count = current_object->sunset_fade.count;
		current_object->sunset_fade.count = count - 1u;
		if (count != 1u)
			return;
		current_object->state++;
		break;
	case 2:
		break;
	}
	if (screen->background_fading != 0u)
		return;
	fade = current_object->sunset_fade.fade_list;
	fade_count = PALETTE_FADE_LIST(fade)->length / 0xCu;
	fades_left = fade_count;
	fade += 4u;
	if (fade_count == 0u)
	{
		current_object->flags = 0u;
		return;
	}
	current_object->sunset_fade.count = fades_left;
	fade_slot = &current_object->sunset_fade.fades;
	do
	{
		from = PALETTE_FADE_ENTRY(fade)->from;
		to = PALETTE_FADE_ENTRY(fade)->to;
		fade += 8u;
		r12 = (r12 & ~0xFFFFu) | PALETTE_FADE_ENTRY(fade - 0x8u)->palette_offset;
		fade += 2u;
		blocks = PALETTE_FADE_ENTRY(fade - 0xAu)->blocks;
		fade++;
		period = PALETTE_FADE_ENTRY(fade - 0xBu)->period;
		fade++;
		fade_object = address_of(start_palette_fade(blocks, period, from, to, (uint16_t)r12));
		fade_slot[0] = fade_object;
		fades_left--;
		fade_slot++;
	} while (fades_left != 0u);
	current_object->handler = sunset_fade_wait_ADDRESS;
}

/* Waits for its palette fades to finish, then ends; if the stage's background starts changing (screen
   background_fading) it stops them and ends. */
void sunset_fade_wait(void)
{
	uint8_t all_flags = 0, fade_flags = 0;
	uint32_t fades_left = 0;
	uint32_t *fade_slot = NULL;
	struct object *fade = NULL;

	all_flags = 0u;
	fade_slot = &current_object->sunset_fade.fades;
	fades_left = current_object->sunset_fade.count;
	do
	{
		fade = pointer_to(fade_slot[0]);
		fade_slot++;
		if (fade != NULL)
		{
			fade_flags = (uint8_t)(fade->flags | ((uint32_t)fade->flags_high << 8u) | ((uint32_t)fade->flags_upper << 16u));
			all_flags |= fade_flags;
			if ((fade_flags & 1u) == 0u)
				fade_slot[-1] = 0u;
		}
		fades_left--;
	} while (fades_left != 0u);
	if ((all_flags & 1u) == 0u)
		current_object->flags = 0u;
	if (screen->background_fading == 0u)
		return;
	fade_slot = &current_object->sunset_fade.fades;
	fades_left = current_object->sunset_fade.count;
	do
	{
		fade = pointer_to(fade_slot[0]);
		fade_slot++;
		if (fade != NULL)
			fade->flags = 0u;
		fades_left--;
	} while (fades_left != 0u);
	current_object->flags = 0u;
}

/* Unused object handler: sets up a sprite on the background as backdrop_near_start does (from its data
   the animation, palette, panorama_x, height and half_width; draw_mode A, layer 1; straight to state 1
   when screen background_at_start is set); then backdrop_far_update. */
void backdrop_far_start(void)
{
	uint32_t animation = 0;
	const struct backdrop_record *backdrop_record = NULL;

	if (screen->background_at_start != 0u)
		current_object->state++;
	current_object->backdrop.leaving = 0u;
	backdrop_record = rom_pointer(current_object->backdrop.data);
	animation = backdrop_record->animation;
	current_object->palette = backdrop_record->palette;
	current_object->backdrop.panorama_x = backdrop_record->panorama_x;
	current_object->backdrop.height = backdrop_record->height;
	current_object->backdrop.half_width = backdrop_record->half_width;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = 1u;
	animation_start(animation);
	current_object->handler = backdrop_far_update_ADDRESS;
	backdrop_far_update();
}

/* Only unused code reaches it (backdrop_far_start): keeps the sprite on the background's panorama at the
   panorama's own speed (x from the car's view_yaw, a quarter of it until screen background_changing is
   set, less panorama_x), y from the background's height (screen background_scroll_y) and height. By
   state: 0 waits until it is off the screen; 1 draw_object while on the screen; off it, it ends once
   leaving. */
void backdrop_far_update(void)
{
	uint32_t zero = 0;
	uint32_t scroll_x = 0;

	current_object->backdrop.leaving |= screen->backdrops_leaving;
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	current_object->x = 0u - (((scroll_x - current_object->backdrop.panorama_x) & 0x3FFu) - 0x2A0u);
	switch (current_object->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		current_object->state++;
		break;
	case 1:
		break;
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		current_object->y = 0u - (screen->background_scroll_y - 0xFFu - current_object->backdrop.height);
		draw_object(scroll_x);
		return;
	}
	if (current_object->backdrop.leaving != 0u)
		current_object->flags = 0u;
}

/* Sets up a sprite on the background a little nearer than the panorama (the hill of the Acropolis with
   the Parthenon, stages 14 and 19): from its data the animation, palette, panorama_x, height and
   half_width; draw_mode A, layer 1; straight to state 1 when the background is being set up (screen
   background_at_start); then backdrop_near_update. */
void backdrop_near_start(void)
{
	uint32_t animation = 0;
	const struct backdrop_record *backdrop_record = NULL;

	if (screen->background_at_start != 0u)
		current_object->state++;
	current_object->backdrop.leaving = 0u;
	backdrop_record = rom_pointer(current_object->backdrop.data);
	animation = backdrop_record->animation;
	current_object->palette = backdrop_record->palette;
	current_object->backdrop.panorama_x = backdrop_record->panorama_x;
	current_object->backdrop.height = backdrop_record->height;
	current_object->backdrop.half_width = backdrop_record->half_width;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = 1u;
	animation_start(animation);
	current_object->handler = backdrop_near_update_ADDRESS;
	backdrop_near_update();
}

/* Keeps the sprite on the background, moving with its panorama a quarter faster (nearer): x from the
   car's heading (car view_yaw, a quarter of it until screen background_changing is set) less panorama_x,
   y from the background's height (screen background_scroll_y) and height, but not below screen
   road_top_line's line. By state: 0 waits until it is off the screen (so it does not pop up in view); 1
   draw_object while on the screen; off it, it ends once leaving. */
void backdrop_near_update(void)
{
	uint32_t zero = 0;
	uint16_t height = 0, scroll_x = 0, y_change = 0;
	uint32_t minus_y = 0;

	current_object->backdrop.leaving |= screen->backdrops_leaving;
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	scroll_x = (scroll_x - current_object->backdrop.panorama_x) & 0x3FFu;
	current_object->x = 0u - (scroll_x + (scroll_x >> 2) - 0x320u);
	switch (current_object->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		current_object->state++;
		break;
	case 1:
		break;
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (current_object->backdrop.leaving != 0u)
			current_object->flags = 0u;
		return;
	}
	height = current_object->backdrop.height;
	minus_y = (uint16_t)(screen->background_scroll_y - 0xFFu - height);
	height -= screen->road_top_line;
	if ((int16_t)height < (int16_t)minus_y)
		minus_y = height;
	y_change = (uint16_t)(0u - minus_y - current_object->y);
	if ((int16_t)y_change > 1 || (int16_t)y_change < -1)
		current_object->y = 0u - minus_y;
	draw_object(minus_y);
}

/* Sets up a landmark on the horizon that grows as the car approaches (Tiananmen in stage 10, the castle
   of stages 16 and 17...): as backdrop_near_start, layer 2, and approach_start = the car's segment (car
   window_segment) less the distance in its data; then backdrop_landmark_update. */
void backdrop_landmark_start(void)
{
	uint32_t animation = 0;
	const struct backdrop_record *backdrop_record = NULL;

	if (screen->background_at_start != 0u)
		current_object->state++;
	current_object->backdrop_landmark.leaving = 0u;
	backdrop_record = rom_pointer(current_object->backdrop_landmark.data);
	animation = backdrop_record->animation;
	current_object->palette = backdrop_record->palette;
	current_object->backdrop_landmark.panorama_x = backdrop_record->panorama_x;
	current_object->backdrop_landmark.height = backdrop_record->height;
	current_object->backdrop_landmark.half_width = backdrop_record->half_width;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = 2u;
	animation_start(animation);
	current_object->backdrop_landmark.approach_start = 0u - backdrop_record->approach_distance + player_car->window_segment;
	current_object->handler = backdrop_landmark_update_ADDRESS;
	backdrop_landmark_update();
}

/* Keeps the landmark on the background's panorama (as backdrop_near_update, a quarter faster than the
   panorama) and grows it with the distance travelled: its scale (width_scale, height_scale) follows the
   sine of (car window_segment - approach_start) / 2 (sine_table) up to full size (100, state 2). By
   state: 0 waits until it is off the screen; 1 and 2 drawn while on the screen; off it, it ends once
   leaving. */
void backdrop_landmark_update(void)
{
	uint32_t zero = 0;
	uint16_t height = 0, scale = 0, scroll_x = 0, y_change = 0;
	uint32_t approach = 0, minus_y = 0;

	current_object->backdrop_landmark.leaving |= screen->backdrops_leaving;
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	scroll_x = (scroll_x - current_object->backdrop_landmark.panorama_x) & 0x3FFu;
	current_object->x = 0u - (scroll_x + (scroll_x >> 2) - 0x320u);
	switch (current_object->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		current_object->state++;
		approach = player_car->window_segment;
		approach = (uint16_t)(approach - current_object->backdrop_landmark.approach_start);
		scale = (uint16_t)sine_table[approach >> 1] >> 6;
		if ((int16_t)scale >= 0x100)
		{
			scale = 0x100u;
			current_object->state++;
		}
		current_object->width_scale = scale;
		current_object->height_scale = scale;
		break;
	case 1:
		approach = player_car->window_segment;
		approach = (uint16_t)(approach - current_object->backdrop_landmark.approach_start);
		scale = (uint16_t)sine_table[approach >> 1] >> 6;
		if ((int16_t)scale >= 0x100)
		{
			scale = 0x100u;
			current_object->state++;
		}
		current_object->width_scale = scale;
		current_object->height_scale = scale;
		break;
	case 2:
		break;
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (current_object->backdrop_landmark.leaving != 0u)
			current_object->flags = 0u;
		return;
	}
	height = current_object->backdrop_landmark.height;
	minus_y = (uint16_t)(screen->background_scroll_y - 0xFFu - height);
	height -= screen->road_top_line;
	if ((int16_t)height < (int16_t)minus_y)
		minus_y = height;
	y_change = (uint16_t)(0u - minus_y - current_object->y);
	if ((int16_t)y_change > 1 || (int16_t)y_change < -1)
		current_object->y = 0u - minus_y;
	draw_object(minus_y);
}

/* Unused object handler: sets up a sprite on the background as backdrop_far_start does, with
   other_palette 120; then backdrop_flashing_update. */
void backdrop_flashing_start(void)
{
	uint32_t animation = 0;
	const struct backdrop_record *backdrop_record = NULL;

	if (screen->background_at_start != 0u)
		current_object->state++;
	current_object->backdrop_flashing.leaving = 0u;
	backdrop_record = rom_pointer(current_object->backdrop_flashing.data);
	animation = backdrop_record->animation;
	current_object->palette = backdrop_record->palette;
	current_object->backdrop_flashing.panorama_x = backdrop_record->panorama_x;
	current_object->backdrop_flashing.height = backdrop_record->height;
	current_object->backdrop_flashing.half_width = backdrop_record->half_width;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = 1u;
	animation_start(animation);
	current_object->backdrop_flashing.other_palette = 0x120u;
	current_object->handler = backdrop_flashing_update_ADDRESS;
	backdrop_flashing_update();
}

/* Only unused code reaches it (backdrop_flashing_start): as backdrop_far_update (on the panorama at its
   speed), animated (animation_advance), and each frame its palette swaps with other_palette, so it
   flashes between its own palette and palette 120. */
void backdrop_flashing_update(void)
{
	uint32_t zero = 0;
	uint16_t scroll_x = 0;
	uint32_t minus_y = 0;
	uint16_t other_palette = 0;

	current_object->backdrop_flashing.leaving |= screen->backdrops_leaving;
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	current_object->x = 0u - (((scroll_x - current_object->backdrop_flashing.panorama_x) & 0x3FFu) - 0x2A0u);
	animation_advance(0u);
	other_palette = current_object->backdrop_flashing.other_palette;
	minus_y = current_object->palette;
	current_object->palette = other_palette;
	current_object->backdrop_flashing.other_palette = minus_y;
	switch (current_object->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		current_object->state++;
		break;
	case 1:
		break;
	}
	zero = backdrop_off_screen();
	if (zero == 0u)
	{
		current_object->y = 0u - (screen->background_scroll_y - 0xFFu - current_object->backdrop_flashing.height);
		draw_object(minus_y);
		return;
	}
	if (current_object->backdrop_flashing.leaving != 0u)
		current_object->flags = 0u;
}

/* Sets up a sprite on the background that is drawn every other frame, so it looks faint (the pyramids of
   stages 13 and 20, the spray of stage 4's falls): as backdrop_near_start, layer 1, frame_count 0; then
   backdrop_translucent_update. */
void backdrop_translucent_start(void)
{
	uint32_t animation = 0;
	const struct backdrop_record *backdrop_record = NULL;

	if (screen->background_at_start != 0u)
		current_object->state++;
	current_object->backdrop_translucent.leaving = 0u;
	backdrop_record = rom_pointer(current_object->backdrop_translucent.data);
	animation = backdrop_record->animation;
	current_object->palette = backdrop_record->palette;
	current_object->backdrop_translucent.panorama_x = backdrop_record->panorama_x;
	current_object->backdrop_translucent.height = backdrop_record->height;
	current_object->backdrop_translucent.half_width = backdrop_record->half_width;
	current_object->draw_mode = 0xAu;
	current_object->depth_layer = 1u;
	animation_start(animation);
	current_object->backdrop_translucent.frame_count = 0u;
	current_object->handler = backdrop_translucent_update_ADDRESS;
	backdrop_translucent_update();
}

/* Keeps the sprite on the background's panorama at the panorama's own speed (as backdrop_far_update),
   animates it and draws it only on even frames (frame_count), so it looks faint. By state: 0 waits until
   it is off the screen; 1 drawn while on the screen; off it, it ends once leaving. */
void backdrop_translucent_update(void)
{
	uint32_t zero = 0;
	uint32_t scroll_x = 0;

	current_object->backdrop_translucent.leaving |= screen->backdrops_leaving;
	scroll_x = player_car->view_yaw;
	if (screen->background_changing == 0u)
		scroll_x = scroll_x >> 2;
	current_object->x = 0u - (((scroll_x - current_object->backdrop_translucent.panorama_x) & 0x3FFu) - 0x2A0u);
	animation_advance(0u);
	current_object->backdrop_translucent.frame_count++;
	switch (current_object->state)
	{
	case 0:
		zero = backdrop_off_screen();
		if (zero == 0u)
			return;
		current_object->state++;
		break;
	case 1:
		break;
	}
	zero = backdrop_off_screen();
	if (zero)
	{
		if (current_object->backdrop_translucent.leaving != 0u)
			current_object->flags = 0u;
		return;
	}
	if ((current_object->backdrop_translucent.frame_count & 1u) != 0u)
		return;
	current_object->y = 0u - (screen->background_scroll_y - 0xFFu - current_object->backdrop_translucent.height);
	draw_object(scroll_x);
}

/* Whether the background sprite is wholly off the screen (zero set; it clears flags bit 5 then): its x
   (field 30) more than half_width beyond the screen's edges (0..140). */
uint32_t backdrop_off_screen(void)
{
	uint32_t zero = 0;
	uint16_t centre_x = 0, half_screen = 0, half_width = 0, minus_right_edge = 0;

	half_screen = 0xA0u;
	centre_x = (uint16_t)(current_object->x - 0xA0u);
	half_width = current_object->backdrop.half_width;
	minus_right_edge = (uint16_t)(0u - centre_x - half_width);
	if ((int16_t)(centre_x - half_width) > 0xA0 || (int16_t)half_screen < (int16_t)minus_right_edge)
	{
		current_object->flags &= 0xDFu;
		zero = 0u == 0u;
		return zero;
	}
	zero = 1u == 0u;
	return zero;
}
