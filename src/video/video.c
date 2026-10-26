/**
 *    Name:        video/video.c
 *    Description: the video chips set up and cleared: the screens, the tile layers, the sprite chip and its lists, the mixer, the I/O chips
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
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
	bool L_013DB6 = false;
	uint16_t course_length = 0;
	uint32_t command_routine = 0, courses_left = 0, index = 0, length_place = 0, screen_hardware = 0, section_length = 0, stage_course = 0;
	uint32_t command = 0;

	index = screen >> 16;
	index = index & 1u;
	SCREEN(screen)->index = index;
	screen_hardware = screen_hardware_records[index];
	SCREEN(screen)->sprite_screen_bit = SCREEN_HARDWARE(screen_hardware)->sprite_screen_bit;
	screen_hardware += 2u;
	SCREEN(screen)->palette_ram = SCREEN_HARDWARE(screen_hardware - 0x2u)->palette_ram;
	screen_hardware += 4u;
	SCREEN(screen)->mixer = SCREEN_HARDWARE(screen_hardware - 0x6u)->mixer;
	SCREEN(screen)->io_chip = SCREEN_HARDWARE(screen_hardware - 0x6u)->io_chip;
	SCREEN(screen)->draw_distance = DRAW_DISTANCE_LIST(screen_view_defaults[0].draw_distances)[1];
	SCREEN(screen)->eye_distance = screen_view_defaults[0].eye_distance;
	SCREEN(screen)->sprite_origin_x = screen_view_defaults[0].sprite_origin_x;
	SCREEN(screen)->sprite_origin_y = screen_view_defaults[0].sprite_origin_y;
	SCREEN(screen)->view_pitch_base = screen_view_defaults[0].view_pitch_bases[0];
	stage_course = stage_courses_ADDRESS;
	length_place = screen + 0x700u;
	courses_left = 0x1Fu;
	do
	{
		r12 = COURSE_POINTER(stage_course)[0];
		section_length = 0u;
		course_length = 0u;
		stage_course += 4u;
		for (;;)
		{
			course_length += section_length;
			for (;;)
			{
				section_length = mem_read16(r12);
				r12 = r12 + 6u;
				if ((int16_t)section_length >= 0)
					break;
				command = (uint16_t)~section_length;
				command_routine = course_length_command_jumps[command];
				switch (command)
				{
				case 2:
				case 4:
					continue;
				case 0:
					r12 = mem_read32(r12 - 4u);
					continue;
				case 3:
					r12 = r12 - 2u;
					continue;
				case 1:
				case 5:
					L_013DB6 = true;
					break;
				default:
					invalid_jump(course_length_command_jumps[command]);
				}
				if (L_013DB6)
					break;
			}
			if (L_013DB6)
				break;
		}
		L_013DB6 = false;
		mem_write16(length_place, course_length);
		courses_left--;
		length_place += 2u;
	} while (courses_left != 0u);
	mem_write16(length_place, 0x14Au);
	return command_routine;
}

/* Clears the screen's work RAM at boot: E000 bytes from 8000 below its block (the objects and their
   lists) to 6000 into it. */
void clear_screen_ram(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = screen - 0x8000u;
	blocks_left = 0x38u;
	block_halfwords = 0x200u;
	do
	{
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
		block += 0x400u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Sets up video RAM: cleared (clear_video_ram), the text font (load_text_font), the video control
   register 31FF00 (0800; 0A00 flipped, flip_screen) and the registers 31FF02-31FF8F from 010B12, with
   their copy at video_registers, the four tile layers' scroll targets (scroll_targets-20E2AF) cleared,
   then reset_tile_layers. */
uint32_t init_video(void)
{
	clear_video_ram();
	load_text_font();
	mem_write16(VIDEO_31FF00, video_control_default);
	video_registers[0] = video_control_default;
	if (flip_screen != 0u)
	{
		mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) | 0x200u);
		video_registers[0] |= 0x200u;
	}
	move_halfwords(scroll_targets_ADDRESS, zero_block_ADDRESS, 0x10u);
	move_halfwords(VIDEO_RAM + 0x1FF02u, video_register_defaults_ADDRESS, 0x47u);
	move_string(video_register_defaults_ADDRESS, 0x47u, video_registers_ADDRESS + 0x2u, 0x47u, 2u, 0u, 0u, 0u, &r27, &r28);
	return reset_tile_layers();
}

/* Puts the screen's two tile layers (NBG0 and NBG2 for screen 1, NBG1 and NBG3 for screen 2) back at
   page 0 and scroll 0: their page selects (video_registers[32], video_registers[36]) and scroll targets
   (scroll_targets, scroll_targets[4]) cleared, and bits 12-15 of the video control register (31FF00,
   video_registers) cleared. Used as the game modes start. */
uint32_t reset_tile_layers(void)
{
	uint32_t index = 0;
	uint32_t first_scroll_target = 0, second_scroll_target = 0;

	mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) & 0xFFFu);
	video_registers[0] &= 0xFFFu;
	index = SCREEN(screen)->index;
	RAM32(video_registers_ADDRESS + 0x40u + index * 4u) = 0u;
	RAM32(video_registers_ADDRESS + 0x48u + index * 4u) = 0u;
	first_scroll_target = scroll_targets_ADDRESS + index * 8u;
	mem_write32(first_scroll_target, 0u);
	mem_write32(first_scroll_target + 4u, 0u);
	second_scroll_target = scroll_targets_ADDRESS + 0x10u + index * 8u;
	mem_write32(second_scroll_target, 0u);
	mem_write32(second_scroll_target + 4u, 0u);
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
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
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
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
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
	move_string(zero_block_ADDRESS, 0x200u, 0x300000u | (uint16_t)page, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Sets up the sprite chip at boot: its control registers 500000-50000C from 010B0A-010B0F (rendering
   every frame), the buffer's flip bits inverted when the screen is flipped (flip_screen), then
   init_sprite_lists. */
void init_sprite_chip(void)
{
	uint16_t control_flip = 0;
	uint32_t control_register = 0, control_values = 0;

	mem_write16(SPRITE_CONTROL_500000, sprite_control_mode);
	control_register = SPRITE_CONTROL + 0x4u;
	control_flip = sprite_control_flip;
	control_values = sprite_control_rest_ADDRESS;
	if (flip_screen != 0u)
		control_flip = control_flip ^ 3u;
	mem_write16(control_register, control_flip);
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	mem_write16(control_register + 2u, SPRITE_CONTROL_VALUES(control_values)[1]);
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
	move_string(sprite_ram_start_entries_ADDRESS, 0x28u, 0x400000u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	SCREEN(SCREEN_1)->sprite_list_last = SPRITE_RAM_404000;
	SCREEN(SCREEN_2)->sprite_list_last = SPRITE_RAM_414000;
	SCREEN(SCREEN_1)->next_sprite_place = 0x20C000u;
	SCREEN(SCREEN_2)->next_sprite_place = 0x21C000u;
	mem_write16(SPRITE_RAM_404000, 0xFFFFu);
	mem_write16(SPRITE_RAM_407000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40D000, 0xFFFFu);
	mem_write16(SPRITE_RAM_414000, 0xFFFFu);
	mem_write16(SPRITE_RAM_417000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41D000, 0xFFFFu);
	sprite_lists_set_up = 1u;
	sprite_list_buffers = 0u;
	SCREEN(SCREEN_1)->sprite_groups &= 0xFF00u;
	SCREEN(SCREEN_2)->sprite_groups &= 0xFF00u;
}

/* Sets the count of sprite groups drawn this frame (sprite_groups) to 0: unless something is drawn after
   it, the vertical blank (finish_sprite_list) ends the screen's list at its start, without sprites. Used
   as the game modes start. */
void clear_sprite_group_count(void)
{
	SCREEN(screen)->sprite_groups = 0u;
}

/* Sets the screen's mixer (registers 00-4E at mixer) and its two copies in the screen's block
   (mixer_values, mixer_defaults) to the defaults at 010AB6, with the other screen's two tile layers
   turned off (010B06: NBG1 and NBG3 on screen 1, NBG0 and NBG2 on screen 2; the two screens share video
   RAM, each showing two of the four layers). */
void init_mixer(void)
{
	uint32_t first_layer = 0, hidden_layer_pair = 0, mixer = 0, second_layer = 0;

	mixer = SCREEN(screen)->mixer;
	move_halfwords_backward(mixer, mixer_register_defaults_ADDRESS, 0x28u);
	move_halfwords(screen + 0x160u, mixer_register_defaults_ADDRESS, 0x28u);
	move_string(mixer_register_defaults_ADDRESS, 0x28u, screen + 0x1B0u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	hidden_layer_pair = hidden_layer_pairs_ADDRESS + SCREEN(screen)->index * 2u;
	first_layer = HIDDEN_LAYER_PAIR(hidden_layer_pair)->first_layer;
	second_layer = HIDDEN_LAYER_PAIR(hidden_layer_pair)->second_layer;
	mem_write16(mixer + first_layer * 2u, 0u);
	mem_write16(mixer + second_layer * 2u, 0u);
	mem_write16(screen + 0x160u + first_layer * 2u, 0u);
	mem_write16(screen + 0x160u + second_layer * 2u, 0u);
	mem_write16(screen + 0x1B0u + first_layer * 2u, 0u);
	mem_write16(screen + 0x1B0u + second_layer * 2u, 0u);
}

/* Sets up the two I/O chips at boot: ports D, G and H outputs and cleared (the coin meters, lamps, LCD,
   the tile bank and the EEPROM's lines), each chip's screen on and the sound CPU out of reset (CNT 6 and
   86), and the first bank of the wheels' and pedals' A/D inputs (C00060). */
void init_io_chips(void)
{
	mem_write8(IO_C0001C, 6u);
	mem_write8(IO_C0001E, 0xC8u);
	mem_write8(IO_C00006, 0u);
	mem_write8(IO_C0000C, 0u);
	mem_write8(IO_C0000E, 0u);
	mem_write8(0xC8001Cu, 0x86u);
	mem_write8(0xC8001Eu, 0xC8u);
	mem_write8(0xC80006u, 0u);
	mem_write8(0xC8000Cu, 0u);
	mem_write8(0xC8000Eu, 0u);
	mem_write8(IO_C00060, 0u);
}

/* Copies data into sprite RAM outside the lists at boot: 1E0 bytes from 12297C to 410000, 2400 bytes
   from 107020 to 401000, and twelve tables of sixteen halfwords from 010C44 to 403400 (laid out as the
   indirect colour tables sprites can take from sprite RAM). */
void load_sprite_ram_tables(void)
{
	move_halfwords(SPRITE_RAM + 0x10000u, solid_colour_blocks_ADDRESS, 0xF0u);
	move_halfwords(SPRITE_RAM + 0x1000u, indirect_colour_tables_ADDRESS, 0x1200u);
	move_string(last_colour_tables_ADDRESS, 0xC0u, SPRITE_RAM + 0x3400u, 0xC0u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: meant to copy R0 & 3F8 bytes from R10 to R11 by jumping into a run of 8-byte moves at 0109B4,
   but the jump (200 less half the count from the run's start) is worked out for 128 moves and the run
   has 64: it copies the count less 200 bytes, nothing for 200, and for less it jumps past the run into
   the data at 010AB5. */
void unrolled_copy(uint16_t byte_count, uint32_t source, uint32_t destination)
{
	uint32_t move_destination = 0, second_word = 0, move_destination_2 = 0, second_word_2 = 0, move_destination_3 = 0, second_word_3 = 0, move_destination_4 = 0, second_word_4 = 0, move_destination_5 = 0, second_word_5 = 0, move_destination_6 = 0, second_word_6 = 0, move_destination_7 = 0, second_word_7 = 0, move_destination_8 = 0, second_word_8 = 0, move_destination_9 = 0, second_word_9 = 0, move_destination_10 = 0, second_word_10 = 0, move_destination_11 = 0, second_word_11 = 0, move_destination_12 = 0, second_word_12 = 0, move_destination_13 = 0, second_word_13 = 0, move_destination_14 = 0, second_word_14 = 0, move_destination_15 = 0, second_word_15 = 0, move_destination_16 = 0, second_word_16 = 0, move_destination_17 = 0, second_word_17 = 0, move_destination_18 = 0, second_word_18 = 0, move_destination_19 = 0, second_word_19 = 0, move_destination_20 = 0, second_word_20 = 0, move_destination_21 = 0, second_word_21 = 0, move_destination_22 = 0, second_word_22 = 0, move_destination_23 = 0, second_word_23 = 0, move_destination_24 = 0, second_word_24 = 0, move_destination_25 = 0, second_word_25 = 0, move_destination_26 = 0, second_word_26 = 0, move_destination_27 = 0, second_word_27 = 0, move_destination_28 = 0, second_word_28 = 0, move_destination_29 = 0, second_word_29 = 0, move_destination_30 = 0, second_word_30 = 0, move_destination_31 = 0, second_word_31 = 0, move_destination_32 = 0, second_word_32 = 0, move_destination_33 = 0, second_word_33 = 0, move_destination_34 = 0, second_word_34 = 0, move_destination_35 = 0, second_word_35 = 0, move_destination_36 = 0, second_word_36 = 0, move_destination_37 = 0, second_word_37 = 0, move_destination_38 = 0, second_word_38 = 0, move_destination_39 = 0, second_word_39 = 0, move_destination_40 = 0, second_word_40 = 0, move_destination_41 = 0, second_word_41 = 0, move_destination_42 = 0, second_word_42 = 0, move_destination_43 = 0, second_word_43 = 0, move_destination_44 = 0, second_word_44 = 0, move_destination_45 = 0, second_word_45 = 0, move_destination_46 = 0, second_word_46 = 0, move_destination_47 = 0, second_word_47 = 0, move_destination_48 = 0, second_word_48 = 0, move_destination_49 = 0, second_word_49 = 0, move_destination_50 = 0, second_word_50 = 0, move_destination_51 = 0, second_word_51 = 0, move_destination_52 = 0, second_word_52 = 0, move_destination_53 = 0, second_word_53 = 0, move_destination_54 = 0, second_word_54 = 0, move_destination_55 = 0, second_word_55 = 0, move_destination_56 = 0, second_word_56 = 0, move_destination_57 = 0, second_word_57 = 0, move_destination_58 = 0, second_word_58 = 0, move_destination_59 = 0, second_word_59 = 0, move_destination_60 = 0, second_word_60 = 0, move_destination_61 = 0, second_word_61 = 0, move_destination_62 = 0, second_word_62 = 0, move_destination_63 = 0, second_word_63 = 0, second_word_64 = 0;

	switch (0u - (((byte_count & 0x3F8u) >> 1) - 0x200u))
	{
	case 0x0u:
		break;
	default:
		invalid_jump(0x109B4u + (0u - (((byte_count & 0x3F8u) >> 1) - 0x200u)));
	}
	move_destination = destination;
	destination += 8u;
	second_word = mem_read32(source + 4u);
	mem_write32(move_destination, mem_read32(source));
	mem_write32(move_destination + 4u, second_word);
	source += 8u;
	move_destination_2 = destination;
	destination += 8u;
	second_word_2 = mem_read32(source + 4u);
	mem_write32(move_destination_2, mem_read32(source));
	mem_write32(move_destination_2 + 4u, second_word_2);
	source += 8u;
	move_destination_3 = destination;
	destination += 8u;
	second_word_3 = mem_read32(source + 4u);
	mem_write32(move_destination_3, mem_read32(source));
	mem_write32(move_destination_3 + 4u, second_word_3);
	source += 8u;
	move_destination_4 = destination;
	destination += 8u;
	second_word_4 = mem_read32(source + 4u);
	mem_write32(move_destination_4, mem_read32(source));
	mem_write32(move_destination_4 + 4u, second_word_4);
	source += 8u;
	move_destination_5 = destination;
	destination += 8u;
	second_word_5 = mem_read32(source + 4u);
	mem_write32(move_destination_5, mem_read32(source));
	mem_write32(move_destination_5 + 4u, second_word_5);
	source += 8u;
	move_destination_6 = destination;
	destination += 8u;
	second_word_6 = mem_read32(source + 4u);
	mem_write32(move_destination_6, mem_read32(source));
	mem_write32(move_destination_6 + 4u, second_word_6);
	source += 8u;
	move_destination_7 = destination;
	destination += 8u;
	second_word_7 = mem_read32(source + 4u);
	mem_write32(move_destination_7, mem_read32(source));
	mem_write32(move_destination_7 + 4u, second_word_7);
	source += 8u;
	move_destination_8 = destination;
	destination += 8u;
	second_word_8 = mem_read32(source + 4u);
	mem_write32(move_destination_8, mem_read32(source));
	mem_write32(move_destination_8 + 4u, second_word_8);
	source += 8u;
	move_destination_9 = destination;
	destination += 8u;
	second_word_9 = mem_read32(source + 4u);
	mem_write32(move_destination_9, mem_read32(source));
	mem_write32(move_destination_9 + 4u, second_word_9);
	source += 8u;
	move_destination_10 = destination;
	destination += 8u;
	second_word_10 = mem_read32(source + 4u);
	mem_write32(move_destination_10, mem_read32(source));
	mem_write32(move_destination_10 + 4u, second_word_10);
	source += 8u;
	move_destination_11 = destination;
	destination += 8u;
	second_word_11 = mem_read32(source + 4u);
	mem_write32(move_destination_11, mem_read32(source));
	mem_write32(move_destination_11 + 4u, second_word_11);
	source += 8u;
	move_destination_12 = destination;
	destination += 8u;
	second_word_12 = mem_read32(source + 4u);
	mem_write32(move_destination_12, mem_read32(source));
	mem_write32(move_destination_12 + 4u, second_word_12);
	source += 8u;
	move_destination_13 = destination;
	destination += 8u;
	second_word_13 = mem_read32(source + 4u);
	mem_write32(move_destination_13, mem_read32(source));
	mem_write32(move_destination_13 + 4u, second_word_13);
	source += 8u;
	move_destination_14 = destination;
	destination += 8u;
	second_word_14 = mem_read32(source + 4u);
	mem_write32(move_destination_14, mem_read32(source));
	mem_write32(move_destination_14 + 4u, second_word_14);
	source += 8u;
	move_destination_15 = destination;
	destination += 8u;
	second_word_15 = mem_read32(source + 4u);
	mem_write32(move_destination_15, mem_read32(source));
	mem_write32(move_destination_15 + 4u, second_word_15);
	source += 8u;
	move_destination_16 = destination;
	destination += 8u;
	second_word_16 = mem_read32(source + 4u);
	mem_write32(move_destination_16, mem_read32(source));
	mem_write32(move_destination_16 + 4u, second_word_16);
	source += 8u;
	move_destination_17 = destination;
	destination += 8u;
	second_word_17 = mem_read32(source + 4u);
	mem_write32(move_destination_17, mem_read32(source));
	mem_write32(move_destination_17 + 4u, second_word_17);
	source += 8u;
	move_destination_18 = destination;
	destination += 8u;
	second_word_18 = mem_read32(source + 4u);
	mem_write32(move_destination_18, mem_read32(source));
	mem_write32(move_destination_18 + 4u, second_word_18);
	source += 8u;
	move_destination_19 = destination;
	destination += 8u;
	second_word_19 = mem_read32(source + 4u);
	mem_write32(move_destination_19, mem_read32(source));
	mem_write32(move_destination_19 + 4u, second_word_19);
	source += 8u;
	move_destination_20 = destination;
	destination += 8u;
	second_word_20 = mem_read32(source + 4u);
	mem_write32(move_destination_20, mem_read32(source));
	mem_write32(move_destination_20 + 4u, second_word_20);
	source += 8u;
	move_destination_21 = destination;
	destination += 8u;
	second_word_21 = mem_read32(source + 4u);
	mem_write32(move_destination_21, mem_read32(source));
	mem_write32(move_destination_21 + 4u, second_word_21);
	source += 8u;
	move_destination_22 = destination;
	destination += 8u;
	second_word_22 = mem_read32(source + 4u);
	mem_write32(move_destination_22, mem_read32(source));
	mem_write32(move_destination_22 + 4u, second_word_22);
	source += 8u;
	move_destination_23 = destination;
	destination += 8u;
	second_word_23 = mem_read32(source + 4u);
	mem_write32(move_destination_23, mem_read32(source));
	mem_write32(move_destination_23 + 4u, second_word_23);
	source += 8u;
	move_destination_24 = destination;
	destination += 8u;
	second_word_24 = mem_read32(source + 4u);
	mem_write32(move_destination_24, mem_read32(source));
	mem_write32(move_destination_24 + 4u, second_word_24);
	source += 8u;
	move_destination_25 = destination;
	destination += 8u;
	second_word_25 = mem_read32(source + 4u);
	mem_write32(move_destination_25, mem_read32(source));
	mem_write32(move_destination_25 + 4u, second_word_25);
	source += 8u;
	move_destination_26 = destination;
	destination += 8u;
	second_word_26 = mem_read32(source + 4u);
	mem_write32(move_destination_26, mem_read32(source));
	mem_write32(move_destination_26 + 4u, second_word_26);
	source += 8u;
	move_destination_27 = destination;
	destination += 8u;
	second_word_27 = mem_read32(source + 4u);
	mem_write32(move_destination_27, mem_read32(source));
	mem_write32(move_destination_27 + 4u, second_word_27);
	source += 8u;
	move_destination_28 = destination;
	destination += 8u;
	second_word_28 = mem_read32(source + 4u);
	mem_write32(move_destination_28, mem_read32(source));
	mem_write32(move_destination_28 + 4u, second_word_28);
	source += 8u;
	move_destination_29 = destination;
	destination += 8u;
	second_word_29 = mem_read32(source + 4u);
	mem_write32(move_destination_29, mem_read32(source));
	mem_write32(move_destination_29 + 4u, second_word_29);
	source += 8u;
	move_destination_30 = destination;
	destination += 8u;
	second_word_30 = mem_read32(source + 4u);
	mem_write32(move_destination_30, mem_read32(source));
	mem_write32(move_destination_30 + 4u, second_word_30);
	source += 8u;
	move_destination_31 = destination;
	destination += 8u;
	second_word_31 = mem_read32(source + 4u);
	mem_write32(move_destination_31, mem_read32(source));
	mem_write32(move_destination_31 + 4u, second_word_31);
	source += 8u;
	move_destination_32 = destination;
	destination += 8u;
	second_word_32 = mem_read32(source + 4u);
	mem_write32(move_destination_32, mem_read32(source));
	mem_write32(move_destination_32 + 4u, second_word_32);
	source += 8u;
	move_destination_33 = destination;
	destination += 8u;
	second_word_33 = mem_read32(source + 4u);
	mem_write32(move_destination_33, mem_read32(source));
	mem_write32(move_destination_33 + 4u, second_word_33);
	source += 8u;
	move_destination_34 = destination;
	destination += 8u;
	second_word_34 = mem_read32(source + 4u);
	mem_write32(move_destination_34, mem_read32(source));
	mem_write32(move_destination_34 + 4u, second_word_34);
	source += 8u;
	move_destination_35 = destination;
	destination += 8u;
	second_word_35 = mem_read32(source + 4u);
	mem_write32(move_destination_35, mem_read32(source));
	mem_write32(move_destination_35 + 4u, second_word_35);
	source += 8u;
	move_destination_36 = destination;
	destination += 8u;
	second_word_36 = mem_read32(source + 4u);
	mem_write32(move_destination_36, mem_read32(source));
	mem_write32(move_destination_36 + 4u, second_word_36);
	source += 8u;
	move_destination_37 = destination;
	destination += 8u;
	second_word_37 = mem_read32(source + 4u);
	mem_write32(move_destination_37, mem_read32(source));
	mem_write32(move_destination_37 + 4u, second_word_37);
	source += 8u;
	move_destination_38 = destination;
	destination += 8u;
	second_word_38 = mem_read32(source + 4u);
	mem_write32(move_destination_38, mem_read32(source));
	mem_write32(move_destination_38 + 4u, second_word_38);
	source += 8u;
	move_destination_39 = destination;
	destination += 8u;
	second_word_39 = mem_read32(source + 4u);
	mem_write32(move_destination_39, mem_read32(source));
	mem_write32(move_destination_39 + 4u, second_word_39);
	source += 8u;
	move_destination_40 = destination;
	destination += 8u;
	second_word_40 = mem_read32(source + 4u);
	mem_write32(move_destination_40, mem_read32(source));
	mem_write32(move_destination_40 + 4u, second_word_40);
	source += 8u;
	move_destination_41 = destination;
	destination += 8u;
	second_word_41 = mem_read32(source + 4u);
	mem_write32(move_destination_41, mem_read32(source));
	mem_write32(move_destination_41 + 4u, second_word_41);
	source += 8u;
	move_destination_42 = destination;
	destination += 8u;
	second_word_42 = mem_read32(source + 4u);
	mem_write32(move_destination_42, mem_read32(source));
	mem_write32(move_destination_42 + 4u, second_word_42);
	source += 8u;
	move_destination_43 = destination;
	destination += 8u;
	second_word_43 = mem_read32(source + 4u);
	mem_write32(move_destination_43, mem_read32(source));
	mem_write32(move_destination_43 + 4u, second_word_43);
	source += 8u;
	move_destination_44 = destination;
	destination += 8u;
	second_word_44 = mem_read32(source + 4u);
	mem_write32(move_destination_44, mem_read32(source));
	mem_write32(move_destination_44 + 4u, second_word_44);
	source += 8u;
	move_destination_45 = destination;
	destination += 8u;
	second_word_45 = mem_read32(source + 4u);
	mem_write32(move_destination_45, mem_read32(source));
	mem_write32(move_destination_45 + 4u, second_word_45);
	source += 8u;
	move_destination_46 = destination;
	destination += 8u;
	second_word_46 = mem_read32(source + 4u);
	mem_write32(move_destination_46, mem_read32(source));
	mem_write32(move_destination_46 + 4u, second_word_46);
	source += 8u;
	move_destination_47 = destination;
	destination += 8u;
	second_word_47 = mem_read32(source + 4u);
	mem_write32(move_destination_47, mem_read32(source));
	mem_write32(move_destination_47 + 4u, second_word_47);
	source += 8u;
	move_destination_48 = destination;
	destination += 8u;
	second_word_48 = mem_read32(source + 4u);
	mem_write32(move_destination_48, mem_read32(source));
	mem_write32(move_destination_48 + 4u, second_word_48);
	source += 8u;
	move_destination_49 = destination;
	destination += 8u;
	second_word_49 = mem_read32(source + 4u);
	mem_write32(move_destination_49, mem_read32(source));
	mem_write32(move_destination_49 + 4u, second_word_49);
	source += 8u;
	move_destination_50 = destination;
	destination += 8u;
	second_word_50 = mem_read32(source + 4u);
	mem_write32(move_destination_50, mem_read32(source));
	mem_write32(move_destination_50 + 4u, second_word_50);
	source += 8u;
	move_destination_51 = destination;
	destination += 8u;
	second_word_51 = mem_read32(source + 4u);
	mem_write32(move_destination_51, mem_read32(source));
	mem_write32(move_destination_51 + 4u, second_word_51);
	source += 8u;
	move_destination_52 = destination;
	destination += 8u;
	second_word_52 = mem_read32(source + 4u);
	mem_write32(move_destination_52, mem_read32(source));
	mem_write32(move_destination_52 + 4u, second_word_52);
	source += 8u;
	move_destination_53 = destination;
	destination += 8u;
	second_word_53 = mem_read32(source + 4u);
	mem_write32(move_destination_53, mem_read32(source));
	mem_write32(move_destination_53 + 4u, second_word_53);
	source += 8u;
	move_destination_54 = destination;
	destination += 8u;
	second_word_54 = mem_read32(source + 4u);
	mem_write32(move_destination_54, mem_read32(source));
	mem_write32(move_destination_54 + 4u, second_word_54);
	source += 8u;
	move_destination_55 = destination;
	destination += 8u;
	second_word_55 = mem_read32(source + 4u);
	mem_write32(move_destination_55, mem_read32(source));
	mem_write32(move_destination_55 + 4u, second_word_55);
	source += 8u;
	move_destination_56 = destination;
	destination += 8u;
	second_word_56 = mem_read32(source + 4u);
	mem_write32(move_destination_56, mem_read32(source));
	mem_write32(move_destination_56 + 4u, second_word_56);
	source += 8u;
	move_destination_57 = destination;
	destination += 8u;
	second_word_57 = mem_read32(source + 4u);
	mem_write32(move_destination_57, mem_read32(source));
	mem_write32(move_destination_57 + 4u, second_word_57);
	source += 8u;
	move_destination_58 = destination;
	destination += 8u;
	second_word_58 = mem_read32(source + 4u);
	mem_write32(move_destination_58, mem_read32(source));
	mem_write32(move_destination_58 + 4u, second_word_58);
	source += 8u;
	move_destination_59 = destination;
	destination += 8u;
	second_word_59 = mem_read32(source + 4u);
	mem_write32(move_destination_59, mem_read32(source));
	mem_write32(move_destination_59 + 4u, second_word_59);
	source += 8u;
	move_destination_60 = destination;
	destination += 8u;
	second_word_60 = mem_read32(source + 4u);
	mem_write32(move_destination_60, mem_read32(source));
	mem_write32(move_destination_60 + 4u, second_word_60);
	source += 8u;
	move_destination_61 = destination;
	destination += 8u;
	second_word_61 = mem_read32(source + 4u);
	mem_write32(move_destination_61, mem_read32(source));
	mem_write32(move_destination_61 + 4u, second_word_61);
	source += 8u;
	move_destination_62 = destination;
	destination += 8u;
	second_word_62 = mem_read32(source + 4u);
	mem_write32(move_destination_62, mem_read32(source));
	mem_write32(move_destination_62 + 4u, second_word_62);
	source += 8u;
	move_destination_63 = destination;
	destination += 8u;
	second_word_63 = mem_read32(source + 4u);
	mem_write32(move_destination_63, mem_read32(source));
	mem_write32(move_destination_63 + 4u, second_word_63);
	source += 8u;
	second_word_64 = mem_read32(source + 4u);
	mem_write32(destination, mem_read32(source));
	mem_write32(destination + 4u, second_word_64);
}
