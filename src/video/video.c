/**
 *    Name:        video/video.c
 *    Description: the video chips set up and cleared: the screens, the tile layers, the sprite chip and its lists, the mixer, the I/O chips
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets up the constants of the screen's block at boot: index (0 for the block at 208000, 1 at 218000)
   and by it (screen_hardware_records) sprite_screen_bit, palette_ram, mixer and io_chip; eye_distance,
   draw_distance (the second of the draw distances at 010BE2), sprite_origin_x, sprite_origin_y and
   view_pitch_base from 010BCC (010BC8 holds the draw distances' address); and stage_lengths: for each of
   the 31 roads of stage_courses (the courses player_car_start_course starts by screen first_stage) the
   sum of its sections' lengths (the first halfword of each 6-byte record; the negative ones are
   commands: -1 goes on at the address it holds, -4 is 4 bytes long, -2 and -6 end it), and
   branch_road_length (14A) after them. */
uint32_t init_screen(void)
{
	uint16_t course_length = 0, section_length = 0;
	uint32_t command_routine = 0, courses_left = 0, index = 0, length = 0, length_place = 0, screen_hardware = 0, stage_course = 0;
	const struct course_entry *course_entry = NULL;
	uint32_t command = 0;

	index = address_of(screen) >> 16;
	index = index & 1u;
	screen->index = index;
	screen_hardware = screen_hardware_records[index];
	screen->sprite_screen_bit = SCREEN_HARDWARE(screen_hardware)->sprite_screen_bit;
	screen_hardware += 2u;
	screen->palette_ram = SCREEN_HARDWARE(screen_hardware - 0x2u)->palette_ram;
	screen_hardware += 4u;
	screen->mixer = SCREEN_HARDWARE(screen_hardware - 0x6u)->mixer;
	screen->io_chip = SCREEN_HARDWARE(screen_hardware - 0x6u)->io_chip;
	screen->draw_distance = DRAW_DISTANCE_LIST(screen_view_defaults[0].draw_distances)[1];
	screen->eye_distance = screen_view_defaults[0].eye_distance;
	screen->sprite_origin_x = screen_view_defaults[0].sprite_origin_x;
	screen->sprite_origin_y = screen_view_defaults[0].sprite_origin_y;
	screen->view_pitch_base = screen_view_defaults[0].view_pitch_bases[0];
	stage_course = stage_courses_ADDRESS;
	length_place = address_of(screen) + 0x700u;
	courses_left = 0x1Fu;
	do
	{
		course_entry = rom_pointer(COURSE_POINTER(stage_course)[0]);
		section_length = 0u;
		course_length = 0u;
		stage_course += 4u;
		course_length += section_length;
		for (;;)
		{
			length = course_entry->length;
			course_entry = course_entry + 1;
			if ((int16_t)length >= 0)
			{
				course_length += length;
				continue;
			}
			command = (uint16_t)~length;
			command_routine = course_length_command_jumps[command];
			switch (command)
			{
			case 2:
			case 4:
				continue;
			case 0:
				course_entry = rom_pointer(address_of(course_entry) - 4u);
				course_entry = rom_pointer((course_entry->length | ((uint32_t)(course_entry->jump & 0xFFFFu) << 16u)));
				continue;
			case 3:
				course_entry = rom_pointer(address_of(course_entry) - 2u);
				continue;
			case 1:
			case 5:
				break;
			}
			break;
		}
		STAGE_LENGTH_LIST(length_place)[0] = course_length;
		courses_left--;
		length_place += 2u;
	} while (courses_left != 0u);
	STAGE_LENGTH_LIST(length_place)[0] = 0x14Au;
	return command_routine;
}

/* Clears the screen's work RAM at boot: E000 bytes from 8000 below its block (the objects and their
   lists) to 6000 into it. */
void clear_screen_ram(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = address_of(screen) - 0x8000u;
	blocks_left = 0x38u;
	block_halfwords = 0x200u;
	do
	{
		move_halfwords(block, zero_block_ADDRESS, block_halfwords);
		block += 0x400u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Sets up video RAM: cleared (clear_video_ram), the text font (load_text_font), the video control
   register 31FF00 (0800; 0A00 flipped, flip_screen) and the registers 31FF02-31FF8F from 010B12, with
   their copy at video_registers, the four tile layers' scroll targets (scroll_targets-20E2AF) cleared,
   then reset_tile_layers. */
uint16_t init_video(void)
{
	clear_video_ram();
	load_text_font();
	VIDEO_CONTROL = video_control_default;
	game->video_registers[0] = video_control_default;
	if (game->flip_screen != 0u)
	{
		VIDEO_CONTROL |= 0x200u;
		game->video_registers[0] |= 0x200u;
	}
	move_halfwords(scroll_targets_ADDRESS, zero_block_ADDRESS, 0x10u);
	move_halfwords(VIDEO_RAM + 0x1FF02u, video_register_defaults_ADDRESS, 0x47u);
	move_halfwords(video_registers_ADDRESS + 0x2u, video_register_defaults_ADDRESS, 0x47u);
	return reset_tile_layers();
}

/* Puts the screen's two tile layers (NBG0 and NBG2 for screen 1, NBG1 and NBG3 for screen 2) back at
   page 0 and scroll 0: their page selects (video_registers[32], video_registers[36]) and scroll targets
   (scroll_targets, scroll_targets[4]) cleared, and bits 12-15 of the video control register (31FF00,
   video_registers) cleared. Used as the game modes start. */
uint16_t reset_tile_layers(void)
{
	uint32_t index = 0;

	VIDEO_CONTROL &= 0xFFFu;
	game->video_registers[0] &= 0xFFFu;
	index = screen->index;
	AS32(game->video_registers[32 + index * 2u]) = 0u;
	AS32(game->video_registers[36 + index * 2u]) = 0u;
	game->scroll_targets[index * 2u] = 0u;
	game->scroll_targets[1 + index * 2u] = 0u;
	game->scroll_targets[4 + index * 2u] = 0u;
	game->scroll_targets[5 + index * 2u] = 0u;
	return index;
}

/* Clears video RAM below its registers (300000-31FEFF): the tilemap pages, the text layer and its font,
   the bitmap and the line tables. */
void clear_video_ram(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = 0x300000u;
	blocks_left = 0x1FFu;
	block_halfwords = 0x80u;
	do
	{
		move_halfwords(block, zero_block_ADDRESS, block_halfwords);
		block += 0x100u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Clears the text layer's first 30 rows (64 cells each, 31F000-31FEFF). */
void clear_text_layer(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = TEXT_CELL(0, 0);
	blocks_left = 0xFu;
	block_halfwords = 0x80u;
	do
	{
		move_halfwords(block, zero_block_ADDRESS, block_halfwords);
		block += 0x100u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Unused: clears the tilemap pages from the one at R9 (its offset in video RAM) down to the first, 400
   bytes apart (clear_tile_page each). */
void clear_tile_pages(uint16_t last_page)
{
	uint16_t next_page = 0, page = 0;
	uint16_t cleared_page = 0;

	next_page = (uint16_t)last_page;
	do
	{
		page = (uint16_t)next_page;
		clear_tile_page(page);
		cleared_page = next_page;
		next_page = (uint16_t)(cleared_page - 0x400u);
	} while ((int16_t)cleared_page >= 0x400);
}

/* Only unused code reaches it (clear_tile_pages): clears the tilemap page at R9 (300000 + R9: 200
   halfwords copied from the zeros at 100800). */
void clear_tile_page(uint16_t page)
{
	move_halfwords(0x300000u | page, zero_block_ADDRESS, 0x200u);
}

/* Sets up the sprite chip at boot: its control registers 500000-50000C from 010B0A-010B0F (rendering
   every frame), the buffer's flip bits inverted when the screen is flipped (flip_screen), then
   init_sprite_lists. */
void init_sprite_chip(void)
{
	uint16_t control_flip = 0;
	uint32_t control_register = 0, control_values = 0;

	io_write16(0x500000u, sprite_control_mode);
	control_register = SPRITE_CONTROL + 0x4u;
	control_flip = sprite_control_flip;
	control_values = sprite_control_rest_ADDRESS;
	if (game->flip_screen != 0u)
		control_flip = control_flip ^ 3u;
	io_write16(control_register, control_flip);
	control_register += 2u;
	io_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	io_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	io_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	io_write16(control_register + 2u, SPRITE_CONTROL_VALUES(control_values)[1]);
	init_sprite_lists();
}

/* Sets up the sprite lists: sprite RAM entries 0-4 (0 clips to the whole screen, 1 sets the offset
   sprites can add, x A0 y DE (sprite_origin_x, sprite_origin_y), and jumps to 2; 2 and 3, the jumps to
   screen 1's and screen 2's lists, and 4 an end), an end at the start of the eight list buffers (404000,
   40A000, 414000, 41A000 and their halfway copies 3000 on), each screen's list at its first buffer
   (sprite_list_last 404000 and 414000, sprite_list_buffers 0), its ring of sprite places at 20C000 and
   21C000 (next_sprite_place), sprite_groups 0, and sprite_lists_set_up = 1 (nothing reads it). */
void init_sprite_lists(void)
{
	move_halfwords(SPRITE_RAM, sprite_ram_start_entries_ADDRESS, 0x28u);
	game->screen_1.sprite_list_last = SPRITE_LIST_1_A_ADDRESS;
	game->screen_2.sprite_list_last = SPRITE_LIST_2_A_ADDRESS;
	game->screen_1.next_sprite_place = 0x20C000u;
	game->screen_2.next_sprite_place = 0x21C000u;
	SPRITE_LIST_1_A = 0xFFFFu;
	SPRITE_LIST_1_A_HALFWAY = 0xFFFFu;
	SPRITE_LIST_1_B = 0xFFFFu;
	SPRITE_LIST_1_B_HALFWAY = 0xFFFFu;
	SPRITE_LIST_2_A = 0xFFFFu;
	SPRITE_LIST_2_A_HALFWAY = 0xFFFFu;
	SPRITE_LIST_2_B = 0xFFFFu;
	SPRITE_LIST_2_B_HALFWAY = 0xFFFFu;
	game->sprite_lists_set_up = 1u;
	game->sprite_list_buffers = 0u;
	game->screen_1.sprite_groups &= 0xFF00u;
	game->screen_2.sprite_groups &= 0xFF00u;
}

/* Sets the count of sprite groups drawn this frame (sprite_groups) to 0: unless something is drawn after
   it, the vertical blank (finish_sprite_list) ends the screen's list at its start, without sprites. Used
   as the game modes start. */
void clear_sprite_group_count(void)
{
	screen->sprite_groups = 0u;
}

/* Sets the screen's mixer (registers 00-4E at mixer) and its two copies in the screen's block
   (mixer_values, mixer_defaults) to the defaults at 010AB6, with the other screen's two tile layers
   turned off (010B06: NBG1 and NBG3 on screen 1, NBG0 and NBG2 on screen 2; the two screens share video
   RAM, each showing two of the four layers). */
void init_mixer(void)
{
	uint32_t first_layer = 0, mixer = 0, second_layer = 0;
	const struct hidden_layer_pair *hidden_layer_pair = NULL;

	mixer = screen->mixer;
	move_halfwords_backward(mixer, mixer_register_defaults_ADDRESS, 0x28u);
	move_halfwords(address_of(screen) + 0x160u, mixer_register_defaults_ADDRESS, 0x28u);
	move_halfwords(address_of(screen) + 0x1B0u, mixer_register_defaults_ADDRESS, 0x28u);
	hidden_layer_pair = rom_pointer(hidden_layer_pairs_ADDRESS + screen->index * 2u);
	first_layer = hidden_layer_pair->first_layer;
	second_layer = hidden_layer_pair->second_layer;
	io_write16(mixer + first_layer * 2u, 0u);
	io_write16(mixer + second_layer * 2u, 0u);
	screen->mixer_values[(int32_t)first_layer] = 0u;
	screen->mixer_values[(int32_t)second_layer] = 0u;
	screen->mixer_defaults[(int32_t)first_layer] = 0u;
	screen->mixer_defaults[(int32_t)second_layer] = 0u;
}

/* Sets up the two I/O chips at boot: ports D, G and H outputs and cleared (the coin meters, lamps, LCD,
   the tile bank and the EEPROM's lines), each chip's screen on and the sound CPU out of reset (CNT 6 and
   86), and the first bank of the wheels' and pedals' A/D inputs (C00060). */
void init_io_chips(void)
{
	io_write8(IO_1_CNT, 6u);
	io_write8(IO_1_DIRECTION, 0xC8u);
	io_write8(IO_1_PORT_D, 0u);
	io_write8(IO_1_PORT_G, 0u);
	io_write8(IO_1_PORT_H, 0u);
	io_write8(IO_2_CNT, 0x86u);
	io_write8(IO_2_DIRECTION, 0xC8u);
	io_write8(IO_2_PORT_D, 0u);
	io_write8(IO_2_PORT_G, 0u);
	io_write8(IO_2_PORT_H, 0u);
	io_write8(ANALOG_BANK, 0u);
}

/* Copies data into sprite RAM outside the lists at boot: 1E0 bytes from 12297C to 410000, 2400 bytes
   from 107020 to 401000, and twelve tables of sixteen halfwords from 010C44 to 403400 (laid out as the
   indirect colour tables sprites can take from sprite RAM). */
void load_sprite_ram_tables(void)
{
	move_halfwords(SPRITE_RAM + 0x10000u, solid_colour_blocks_ADDRESS, 0xF0u);
	move_halfwords(SPRITE_RAM + 0x1000u, indirect_colour_tables_ADDRESS, 0x1200u);
	move_halfwords(SPRITE_RAM + 0x3400u, last_colour_tables_ADDRESS, 0xC0u);
}

/* Unused: meant to copy R0 & 3F8 bytes from R10 to R11 by jumping into a run of 8-byte moves at 0109B4,
   but the jump (200 less half the count from the run's start) is worked out for 128 moves and the run
   has 64: it copies the count less 200 bytes, nothing for 200, and for less it jumps past the run into
   the data at 010AB5. */
void unrolled_copy(uint16_t byte_count, uint32_t source, uint32_t destination)
{
	uint32_t move_destination = 0, second_word = 0, second_word_64 = 0;

	switch (0u - (((byte_count & 0x3F8u) >> 1) - 0x200u))
	{
	case 0x0u:
		break;
	}
	for (uint32_t repeat = 0; repeat < 63u; repeat++)
	{
		move_destination = destination;
		destination += 8u;
		second_word = mem_read32(source + 4u);
		mem_write32(move_destination, mem_read32(source));
		mem_write32(move_destination + 4u, second_word);
		source += 8u;
	}
	second_word_64 = mem_read32(source + 4u);
	mem_write32(destination, mem_read32(source));
	mem_write32(destination + 4u, second_word_64);
}
