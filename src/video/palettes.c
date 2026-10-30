/**
 *    Name:        video/palettes.c
 *    Description: the palettes: the boot's, the stages', the upload queue, the screen fades and the palette fades
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Uploads three blocks of colours straight into the screen's palette RAM at boot (upload_palette): 256
   colours at 9E00 (03153C), 16 at 9C00 (031740) and 2560 at 8000 (153904). */
void upload_boot_palettes(void)
{
	uint32_t colour_block = 0;

	colour_block = boot_palette_9e00_ADDRESS;
	upload_palette(colour_block);
	colour_block = black_background_palette_ADDRESS;
	upload_palette(colour_block);
	colour_block = boot_palette_8000_ADDRESS;
	upload_palette(colour_block);
}

/* Queues the palette of the stage the screen goes to (scene_palettes by screen route_stage) through
   queue_palette (the palette queue at screen + 900). Used by player_car_extend_course and the game's
   code (002E74). */
void queue_stage_load(void)
{
	uint32_t colour_block = 0;

	colour_block = scene_palettes[screen->route_stage];
	queue_palette(colour_block);
}

/* Queues a block of colours (record R19: its place in the palette, its length in bytes, the colours) for
   the screen's palette RAM, copied in the vertical blank (upload_queued_palettes). */
void queue_palette(uint32_t colour_block)
{
	uint32_t list_flag = 0;

	list_flag = 0u;
	queue_palette_upload(list_flag, colour_block);
}

/* Queues a list of colour blocks (record R19: its length in bytes, then 8-byte entries: the colours'
   address, their place in the palette, how many blocks of 16) for the screen's palette RAM, copied in
   the vertical blank (upload_queued_palettes). */
void queue_palette_list(uint32_t colour_list)
{
	uint32_t list_flag = 0;

	list_flag = 0x80000000u;
	queue_palette_upload(list_flag, colour_list);
}

/* Adds R19 (bit 31 from R8: a list, not a block) to the screen's queue of palette uploads (the count at
   palette_upload_count, the entries after it, up to 128). */
void queue_palette_upload(uint32_t list_flag, uint32_t colour_record)
{
	uint32_t upload_count = 0;
	struct screen *upload_queue = NULL;

	upload_queue = screen;
	if ((int32_t)upload_queue->palette_upload_count < 0x80)
	{
		upload_count = upload_queue->palette_upload_count + 1u;
		upload_queue->palette_uploads[(int32_t)(upload_count - 1u)] = colour_record | list_flag;
		upload_queue->palette_upload_count = upload_count;
	}
}

/* Copies the queued palette uploads (queue_palette, queue_palette_list) into the screen's palette RAM in
   the vertical blank and empties the queue. */
void upload_queued_palettes(void)
{
	uint32_t colour_record = 0, upload_queue = 0, uploads_left = 0;
	uint32_t queued_upload = 0;

	upload_queue = address_of(screen) + 0x900u;
	uploads_left = SCREEN(upload_queue - 0x900u)->palette_upload_count;
	upload_queue += 4u;
	for (;;)
	{
		queued_upload = PALETTE_UPLOAD_QUEUE(upload_queue)[0];
		colour_record = queued_upload & 0x7FFFFFFFu;
		upload_queue += 4u;
		if ((queued_upload & 0x80000000u) == 0u)
		{
			upload_palette(colour_record);
			uploads_left--;
			if (uploads_left == 0u)
				break;
			continue;
		}
		upload_palette_list(colour_record);
		uploads_left--;
		if (uploads_left == 0u)
			break;
	}
	screen->palette_upload_count = uploads_left;
}

/* Copies a block of colours (place, length in bytes, colours) into the screen's palette RAM (600000 for
   screen 1, 680000 for screen 2). */
void upload_palette(uint32_t colour_block)
{
	uint32_t blocks_left = 0, palette_address = 0;
	uint32_t colours_0_place = 0, colours_2 = 0, colours_4_place = 0, colours_6 = 0, colours_8_place = 0, colours_10 = 0, colours_12_place = 0, colours_14 = 0;

	palette_address = screen->index;
	palette_address = (palette_address << 19) | (palette_address >> 13) | 0x600000u | PALETTE_BLOCK(colour_block)->place;
	colour_block += 2u;
	blocks_left = PALETTE_BLOCK(colour_block - 0x2u)->length;
	blocks_left = blocks_left >> 5;
	colour_block += 2u;
	do
	{
		colours_0_place = palette_address;
		palette_address += 8u;
		colours_2 = AS32(PALETTE_COLOURS(colour_block)[2]);
		io_write32(colours_0_place, AS32(PALETTE_COLOURS(colour_block)[0]));
		io_write32(colours_0_place + 4u, colours_2);
		colour_block += 8u;
		colours_4_place = palette_address;
		palette_address += 8u;
		colours_6 = AS32(PALETTE_COLOURS(colour_block)[2]);
		io_write32(colours_4_place, AS32(PALETTE_COLOURS(colour_block)[0]));
		io_write32(colours_4_place + 4u, colours_6);
		colour_block += 8u;
		colours_8_place = palette_address;
		palette_address += 8u;
		colours_10 = AS32(PALETTE_COLOURS(colour_block)[2]);
		io_write32(colours_8_place, AS32(PALETTE_COLOURS(colour_block)[0]));
		io_write32(colours_8_place + 4u, colours_10);
		colour_block += 8u;
		colours_12_place = palette_address;
		palette_address += 8u;
		colours_14 = AS32(PALETTE_COLOURS(colour_block)[2]);
		io_write32(colours_12_place, AS32(PALETTE_COLOURS(colour_block)[0]));
		io_write32(colours_12_place + 4u, colours_14);
		blocks_left--;
		colour_block += 8u;
	} while (blocks_left != 0u);
}

/* Copies a list of colour blocks (the list's length in bytes, then the address, place and number of
   32-byte blocks of each) into the screen's palette RAM. */
void upload_palette_list(uint32_t colour_list)
{
	uint32_t blocks_left = 0, colour_source = 0, entries_left = 0, palette_address = 0;
	uint32_t colours_2 = 0, colours_6 = 0, colours_10 = 0, colours_14 = 0;

	palette_address = screen->index;
	palette_address = (palette_address << 19) | (palette_address >> 13) | 0x600000u;
	entries_left = PALETTE_BLOCK_LIST(colour_list)->length >> 3;
	colour_list += 4u;
	do
	{
		colour_source = PALETTE_BLOCK_ENTRY(colour_list)->colours;
		colour_list += 4u;
		palette_address = LINKED(palette_address, PALETTE_BLOCK_ENTRY(colour_list - 0x4u)->place);
		colour_list += 2u;
		blocks_left = PALETTE_BLOCK_ENTRY(colour_list - 0x6u)->blocks;
		colour_list += 2u;
		do
		{
			colours_2 = AS32(PALETTE_COLOURS(colour_source)[2]);
			io_write32(palette_address, AS32(PALETTE_COLOURS(colour_source)[0]));
			io_write32(palette_address + 4u, colours_2);
			palette_address += 8u;
			colours_6 = AS32(PALETTE_COLOURS(colour_source)[6]);
			io_write32(palette_address, AS32(PALETTE_COLOURS(colour_source)[4]));
			io_write32(palette_address + 4u, colours_6);
			palette_address += 8u;
			colours_10 = AS32(PALETTE_COLOURS(colour_source)[10]);
			io_write32(palette_address, AS32(PALETTE_COLOURS(colour_source)[8]));
			io_write32(palette_address + 4u, colours_10);
			palette_address += 8u;
			colours_14 = AS32(PALETTE_COLOURS(colour_source)[14]);
			io_write32(palette_address, AS32(PALETTE_COLOURS(colour_source)[12]));
			io_write32(palette_address + 4u, colours_14);
			blocks_left--;
			palette_address += 8u;
		} while (blocks_left != 0u);
		entries_left--;
	} while (entries_left != 0u);
}

/* Steps the screen's fade while the screen's flags bit 0 is set (vblank_update_mixer, every other
   vertical blank): a fade asked for by fade_kind (0 to black, 1 from black, 2 to white, 3 from white, 4
   to a red tint, 5 back from it; records at 01173A) sets fade_level, fade_step, fade_end and
   fade_channels and fade_kind 6 (under way); then each time the level moves a step (6 bits: 20 is -32,
   black; 1F is +31, white) and goes into the mixer's colour offset 0 in mixer_values (registers 40-44:
   red, green, blue, those fade_channels has; register 3E's high byte C0), which vblank_update_mixer
   copies to the mixer; flags bit 0 is cleared when the level reaches fade_end. */
void step_screen_fade(void)
{
	uint32_t carry = 0;
	uint8_t channels = 0;
	uint16_t level = 0;
	uint32_t channels_left = 0, mixer_register = 0;
	const struct screen_fade_start *fade_start = NULL;

	switch (screen->fade_kind)
	{
	case 0:
		fade_start = rom_pointer(screen_fade_starts_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 1:
		fade_start = rom_pointer(screen_fade_from_black_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 2:
		fade_start = rom_pointer(screen_fade_to_white_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 3:
		fade_start = rom_pointer(screen_fade_from_white_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 4:
		fade_start = rom_pointer(screen_fade_to_red_tint_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 5:
		fade_start = rom_pointer(screen_fade_from_red_tint_ADDRESS);
		screen->fade_level = fade_start->level;
		screen->fade_step = fade_start->step;
		screen->fade_end = fade_start->end;
		screen->fade_channels = fade_start->channels;
		screen->fade_kind = 6u;
		break;
	case 6:
		break;
	}
	level = (uint8_t)(screen->fade_level + screen->fade_step) & 0x3Fu;
	screen->fade_level = (uint8_t)level;
	mixer_register = address_of(screen) + 0x19Fu;
	SCREEN(mixer_register - 0x19Fu)->colour_offset_mode = 0xC0u;
	channels = screen->fade_channels;
	channels_left = 3u;
	mixer_register++;
	do
	{
		carry = (uint8_t)(channels * 2u) < channels;
		channels = (uint8_t)(channels * 2u);
		if (carry)
		{
			MIXER_COLOUR_OFFSETS(mixer_register)[0] = level;
			mixer_register += 2u;
		}
		channels_left--;
	} while (channels_left != 0u);
	if (level == screen->fade_end)
		screen->flags &= ~1u;
}

/* Starts a palette_fade for each 12-byte record of list R19 (a 32-bit length in bytes, then records:
   from, to, palette offset, blocks, period). */
void start_palette_fades(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint16_t palette_offset = 0;
	uint32_t fades_left = 0, from = 0, to = 0;
	uint32_t fade_count = 0;

	fade_count = PALETTE_FADE_LIST(fade_list)->length / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count != 0u)
	{
		do
		{
			from = PALETTE_FADE_ENTRY(fade_list)->from;
			to = PALETTE_FADE_ENTRY(fade_list)->to;
			fade_list += 8u;
			palette_offset = PALETTE_FADE_ENTRY(fade_list - 0x8u)->palette_offset;
			fade_list += 2u;
			blocks = PALETTE_FADE_ENTRY(fade_list - 0xAu)->blocks;
			fade_list++;
			period = PALETTE_FADE_ENTRY(fade_list - 0xBu)->period;
			fade_list++;
			start_palette_fade(blocks, period, from, to, palette_offset);
			fades_left--;
		} while (fades_left != 0u);
	}
}

/* Starts the palette fades of list R19 backwards (start_palette_fade from each record's second palette
   to its first): the road's colours back after a change of page set. */
void road_surface_fade_back(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint16_t palette_offset = 0;
	uint32_t fades_left = 0, from = 0, to = 0;
	uint32_t fade_count = 0;

	fade_count = PALETTE_FADE_LIST(fade_list)->length / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count != 0u)
	{
		do
		{
			to = PALETTE_FADE_ENTRY(fade_list)->from;
			fade_list += 4u;
			from = PALETTE_FADE_ENTRY(fade_list - 0x4u)->to;
			fade_list += 4u;
			palette_offset = PALETTE_FADE_ENTRY(fade_list - 0x8u)->palette_offset;
			fade_list += 2u;
			blocks = PALETTE_FADE_ENTRY(fade_list - 0xAu)->blocks;
			fade_list++;
			period = PALETTE_FADE_ENTRY(fade_list - 0xBu)->period;
			fade_list++;
			start_palette_fade(blocks, period, from, to, palette_offset);
			fades_left--;
		} while (fades_left != 0u);
	}
}

/* Starts the palette fades of list R19 (start_palette_fade for each record) and a
   race_background_fade_watch that keeps them; with an empty list clears screen background_fading (the
   background's palettes are changing) at once. */
void race_background_fade_palettes(uint32_t fade_list)
{
	uint8_t blocks = 0, period = 0;
	uint16_t palette_offset = 0;
	uint32_t fade_object = 0, fade_slot = 0, fades_left = 0, from = 0, to = 0;
	struct object *object = NULL;
	uint32_t fade_count = 0;

	fade_count = PALETTE_FADE_LIST(fade_list)->length / 0xCu;
	fades_left = fade_count;
	fade_list += 4u;
	if (fade_count == 0u)
	{
		screen->background_fading = 0u;
		return;
	}
	object = start_object();
	object->handler = race_background_fade_watch_update_ADDRESS;
	object->race_background_fade_watch.background = address_of(current_object);
	object->race_background_fade_watch.count = fades_left;
	fade_slot = address_of(object) + 0x80u;
	do
	{
		from = PALETTE_FADE_ENTRY(fade_list)->from;
		to = PALETTE_FADE_ENTRY(fade_list)->to;
		fade_list += 8u;
		palette_offset = PALETTE_FADE_ENTRY(fade_list - 0x8u)->palette_offset;
		fade_list += 2u;
		blocks = PALETTE_FADE_ENTRY(fade_list - 0xAu)->blocks;
		fade_list++;
		period = PALETTE_FADE_ENTRY(fade_list - 0xBu)->period;
		fade_list++;
		fade_object = address_of(start_palette_fade(blocks, period, from, to, palette_offset));
		BACKGROUND_FADES(fade_slot)[0] = fade_object;
		fades_left--;
		fade_slot += 4u;
	} while (fades_left != 0u);
}

/* Waits for the background's palette fades (fades, count of them) to end, forgetting each as it ends
   (its flags bit 0 clear); then clears screen background_fading (the background's palettes are changing)
   and ends. */
void race_background_fade_watch_update(void)
{
	uint8_t all_flags = 0, fade_flags = 0;
	uint32_t fade_slot = 0, fades_left = 0;
	struct object *fade = NULL;

	all_flags = 0u;
	fade_slot = address_of(current_object) + 0x80u;
	fades_left = current_object->race_background_fade_watch.count;
	do
	{
		fade = pointer_to(BACKGROUND_FADES(fade_slot)[0]);
		fade_slot += 4u;
		if (fade != NULL)
		{
			fade_flags = (uint8_t)(fade->flags | ((uint32_t)fade->flags_high << 8u) | ((uint32_t)fade->flags_upper << 16u));
			all_flags |= fade_flags;
			if ((fade_flags & 1u) == 0u)
				BACKGROUND_FADES(fade_slot)[-1] = 0u;
		}
		fades_left--;
	} while (fades_left != 0u);
	if ((all_flags & 1u) == 0u)
	{
		screen->background_fading = 0u;
		current_object->flags = 0u;
	}
}

/* Starts a palette_fade of 16 colours from the palette at R10 to the one at R11, for palette offset R12
   (R0 blocks of 16 in its upload record), a step every R1 frames, the first after (screen
   palette_fades_started mod R1) + 1 (palette_fades_started counts the fades started); returns the
   object. */
struct object *start_palette_fade(uint8_t blocks, uint8_t period, uint32_t from, uint32_t to, uint16_t palette_offset)
{
	uint8_t phase = 0;
	struct object *object = NULL;

	object = start_object();
	object->handler = palette_fade_prepare_ADDRESS;
	object->palette_fade.upload_length = 8u;
	object->palette_fade.upload_colours = address_of(object) + 0x34u;
	object->palette_fade.palette_offset = palette_offset;
	object->palette_fade.blocks = blocks;
	phase = remainder_u8(screen->palette_fades_started, period);
	screen->palette_fades_started++;
	object->palette_fade.delay = phase + 1u;
	object->palette_fade.period = period;
	object->palette_fade.from = from;
	object->palette_fade.to = to;
	object->palette_fade.colour_place = address_of(object) + 0x34u;
	object->palette_fade.difference_place = address_of(object) + 0x54u;
	object->palette_fade.colours_left = 0x10u;
	return object;
}

/* Prepares a palette fade, one colour a frame for its 16 (colours_left): the colour of the first palette
   (from) into its colours (34 on) and its red, green and blue (five bits each) less those of the second
   (to) into its differences (54 on); then palette_fade_step. */
void palette_fade_prepare(void)
{
	uint16_t from_colour = 0, to_colour = 0;
	uint32_t difference_place = 0, from = 0, to = 0;
	uint8_t colours_left = 0;

	from = current_object->palette_fade.from;
	to = current_object->palette_fade.to;
	r12 = current_object->palette_fade.colour_place;
	difference_place = current_object->palette_fade.difference_place;
	from_colour = PALETTE_COLOURS(from)[0];
	to_colour = PALETTE_COLOURS(to)[0];
	PALETTE_FADE_COLOURS(r12)[0] = from_colour;
	r12 += 2u;
	PALETTE_FADE_DIFFERENCES(difference_place)[0] = (from_colour & 0x1Fu) - (to_colour & 0x1Fu);
	difference_place++;
	from_colour = from_colour >> 5;
	to_colour = to_colour >> 5;
	PALETTE_FADE_DIFFERENCES(difference_place)[0] = (from_colour & 0x1Fu) - (to_colour & 0x1Fu);
	difference_place++;
	PALETTE_FADE_DIFFERENCES(difference_place)[0] = ((from_colour >> 5) & 0x1Fu) - ((to_colour >> 5) & 0x1Fu);
	difference_place++;
	colours_left = current_object->palette_fade.colours_left;
	current_object->palette_fade.colours_left = colours_left - 1u;
	from += 2u;
	to += 2u;
	if (colours_left == 1u)
	{
		current_object->handler = palette_fade_step_ADDRESS;
		palette_fade_step();
		return;
	}
	current_object->palette_fade.from = from;
	current_object->palette_fade.to = to;
	current_object->palette_fade.colour_place = r12;
	current_object->palette_fade.difference_place = difference_place;
}

/* Every period frames (delay; start_palette_fade staggers the first) moves each of the 16 colours one
   step of each of red, green and blue toward the second palette (by its differences, counted down to 0)
   and queues them (queue_palette_list with its record at 28: 8 bytes, the colours, palette_offset,
   blocks); ends when no colour moved, or when the screen fades out (screen flags bit 0, with fade_kind
   at most 5 and even). */
void palette_fade_step(void)
{
	uint8_t fade_kind = 0;
	uint16_t colour_step = 0;
	uint32_t colour_place = 0, colours_left = 0, difference_place = 0, upload = 0;
	uint8_t delay = 0;

	if ((screen->flags & 1u) != 0u)
	{
		fade_kind = screen->fade_kind;
		if ((int8_t)fade_kind <= 5 && (fade_kind & 1u) == 0u)
		{
			current_object->flags = 0u;
			return;
		}
	}
	delay = current_object->palette_fade.delay;
	current_object->palette_fade.delay = delay - 1u;
	if (delay != 1u)
		return;
	current_object->palette_fade.delay = current_object->palette_fade.period;
	difference_place = address_of(current_object) + 0x54u;
	colour_place = address_of(current_object) + 0x34u;
	colour_step = 0u;
	colours_left = 0x10u;
	for (;;)
	{
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
			colour_step++;
			difference_place++;
			if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
			{
				PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
				colour_step += 0x20u;
				difference_place++;
			}
			else if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
			{
				PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
				colour_step -= 0x20u;
				difference_place++;
			}
			else
				difference_place++;
		}
		else if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
			colour_step--;
			difference_place++;
			if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
			{
				PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
				colour_step += 0x20u;
				difference_place++;
			}
			else if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
			{
				PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
				colour_step -= 0x20u;
				difference_place++;
			}
			else
				difference_place++;
		}
		else
		{
			difference_place++;
			if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
			{
				PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
				colour_step += 0x20u;
				difference_place++;
			}
			else
			{
				if (PALETTE_FADE_DIFFERENCES(difference_place)[0] <= 0)
				{
					difference_place++;
					if (PALETTE_FADE_DIFFERENCES(difference_place)[0] >= 0)
					{
						if (PALETTE_FADE_DIFFERENCES(difference_place)[0] <= 0)
						{
							difference_place++;
							colour_place += 2u;
							colours_left--;
							if (colours_left != 0u)
								continue;
							current_object->flags = 0u;
							return;
						}
						PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
						colour_step -= 0x400u;
						difference_place++;
						break;
					}
					PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
					colour_step += 0x400u;
					difference_place++;
					break;
				}
				PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
				colour_step -= 0x20u;
				difference_place++;
			}
		}
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
			colour_step += 0x400u;
			difference_place++;
			break;
		}
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
			colour_step -= 0x400u;
			difference_place++;
			break;
		}
		difference_place++;
		break;
	}
	for (;;)
	{
		PALETTE_FADE_COLOURS(colour_place)[0] += colour_step;
		colours_left--;
		colour_place += 2u;
		if (colours_left == 0u)
			break;
		colour_step = 0u;
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
			colour_step++;
			difference_place++;
		}
		else if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
			colour_step--;
			difference_place++;
		}
		else
			difference_place++;
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
			colour_step += 0x20u;
			difference_place++;
		}
		else if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
			colour_step -= 0x20u;
			difference_place++;
		}
		else
			difference_place++;
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] < 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]++;
			colour_step += 0x400u;
			difference_place++;
			continue;
		}
		if (PALETTE_FADE_DIFFERENCES(difference_place)[0] > 0)
		{
			PALETTE_FADE_DIFFERENCES(difference_place)[0]--;
			colour_step -= 0x400u;
			difference_place++;
			continue;
		}
		difference_place++;
	}
	upload = address_of(current_object) + 0x28u;
	queue_palette_list(upload);
}
