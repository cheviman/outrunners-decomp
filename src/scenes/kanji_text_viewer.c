/**
 *    Name:        scenes/kanji_text_viewer.c
 *    Description: a viewer of kanji texts nothing starts (unused)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Unused object handler: shows the Japanese texts of the table at 01B254 (1000 of them) by number, a
   three-digit number chosen with the buttons. By state: 0 stops the tile unpacking (unpack_state), sets
   up video RAM (init_video) with the tilemaps off and the bitmap layer on at 4 bits a pixel, scroll 0,
   palette FF (video_registers bit B cleared, video_registers[1] = F, video_registers[68],
   video_registers[69], video_registers[70]: words 00, 02, 88, 8A and 8C of the video registers' copy,
   sent as send_video_registers asks), the number 0 (screen route_map_signals-5806), the cursor (x) on
   the units; 1 buttons_pressed bits 3 and 2 move the cursor over the three digits, bits 1 and 0 step the
   digit under it (0-9, wrapping); on screen 1 the number is written at text cell (5,10)
   (kanji_text_viewer_draw_number) with the cursor's digit blinking as '*' (blink_count), and the text
   drawn on the bitmap a line at a time (kanji_text_viewer_draw_line) from 16 rows and 128 pixels in, 16
   rows a line; 2 draws the text only; 3 nothing (nothing sets states 2 and 3). */
void kanji_text_viewer_update(void)
{
	uint16_t text_number = 0;
	uint32_t cell = 0, cursor_cell = 0, line = 0, line_place = 0, text = 0, text_ended = 0;
	uint32_t video_registers_0_long = 0;

	switch (current_object->state)
	{
	case 0:
		screen->unpack_state &= 0xFF00u;
		init_video();
		video_registers_0_long = (game->video_registers[0] | ((uint32_t)game->video_registers[1] << 16u)) & ~0x800u;
		game->video_registers[0] = (uint16_t)video_registers_0_long;
		game->video_registers[1] = (uint16_t)(video_registers_0_long >> 16u);
		game->video_registers[1] = 0xFu;
		game->video_registers[70] = 0xFFu;
		game->video_registers[68] = 0u;
		game->video_registers[69] = 0u;
		game->send_video_registers = 0xFFu;
		screen->kanji_text_number = 0u;
		screen->kanji_text_units = 0;
		screen->kanji_text_tens = 0;
		screen->kanji_text_hundreds = 0;
		current_object->x = 0xEu;
		current_object->state++;
		return;
	case 1:
		if ((screen->buttons_pressed & 8u) != 0u)
			current_object->x -= 2u;
		if ((screen->buttons_pressed & 4u) != 0u)
			current_object->x += 2u;
		if ((int16_t)current_object->x < 0xA)
			current_object->x = 0xEu;
		if ((int16_t)current_object->x > 0xE)
			current_object->x = 0xAu;
		if (current_object->x == 0xAu)
		{
			if ((screen->buttons_pressed & 2u) != 0u)
				screen->kanji_text_hundreds++;
			if ((screen->buttons_pressed & 1u) != 0u)
				screen->kanji_text_hundreds--;
			if (screen->kanji_text_hundreds < 0)
				screen->kanji_text_hundreds = 9;
			if (screen->kanji_text_hundreds > 9)
				screen->kanji_text_hundreds = 0;
		}
		else if (current_object->x == 0xCu)
		{
			if ((screen->buttons_pressed & 2u) != 0u)
				screen->kanji_text_tens++;
			if ((screen->buttons_pressed & 1u) != 0u)
				screen->kanji_text_tens--;
			if (screen->kanji_text_tens < 0)
				screen->kanji_text_tens = 9;
			if (screen->kanji_text_tens > 9)
				screen->kanji_text_tens = 0;
		}
		else
		{
			if ((screen->buttons_pressed & 2u) != 0u)
				screen->kanji_text_units++;
			if ((screen->buttons_pressed & 1u) != 0u)
				screen->kanji_text_units--;
			if (screen->kanji_text_units < 0)
				screen->kanji_text_units = 9;
			if (screen->kanji_text_units > 9)
				screen->kanji_text_units = 0;
		}
		if (screen->index == 0u)
		{
			text_number = (uint16_t)(screen->kanji_text_units + screen->kanji_text_tens * 0xAu + screen->kanji_text_hundreds * 0x64u);
			screen->kanji_text_number = text_number;
			cell = TEXT_CELL(5, 10);
			kanji_text_viewer_draw_number(cell, text_number);
			current_object->kanji_text_viewer.blink_count++;
			if ((current_object->kanji_text_viewer.blink_count & 3u) == 0u)
			{
				cursor_cell = TEXT_CELL(0, 10) + current_object->x;
				TEXT_LAYER_CELLS(cursor_cell)[0] = 0x602Au;
			}
		}
		if (screen->index != 0u)
			return;
		text_ended = 0u;
		line = 0u;
		text = kanji_texts[screen->kanji_text_number];
		do
		{
			line_place = VIDEO_RAM + 0x1040u + line * 0x1000u;
			kanji_text_viewer_draw_line(line_place, &text_ended, &text);
			line++;
		} while (text_ended == 0u);
		return;
	case 2:
		if (screen->index != 0u)
			return;
		text_ended = 0u;
		line = 0u;
		text = kanji_texts[screen->kanji_text_number];
		do
		{
			line_place = VIDEO_RAM + 0x1040u + line * 0x1000u;
			kanji_text_viewer_draw_line(line_place, &text_ended, &text);
			line++;
		} while (text_ended == 0u);
		return;
	case 3:
		return;
	}
}

/* Only unused code reaches it (kanji_text_viewer_update): draws a line of the text at R10 on the bitmap
   layer from R11: each halfword a character (kanji_text_viewer_character_address,
   kanji_text_viewer_draw_character, 16 pixels on); 0 ends the line, a negative one the text (R4 = -1). */
void kanji_text_viewer_draw_line(uint32_t place, uint32_t *p_text_ended, uint32_t *p_text)
{
	uint32_t character = 0;

	for (;;)
	{
		character = KANJI_TEXT(*p_text)[0];
		*p_text += 2u;
		if ((int32_t)character < 0)
			break;
		if (character == 0u)
		{
			return;
		}
		character = kanji_text_viewer_character_address(character);
		r12 = kanji_text_viewer_draw_character(place, character);
		place += 8u;
	}
	*p_text_ended = 0xFFFFFFFFu;
}

/* Only unused code reaches it (kanji_text_viewer_draw_line): draws a 16x16 character on the bitmap layer
   at R11 from its 32 bytes at R13: four 8x8 quarters (kanji_text_viewer_draw_quarter), upper left, upper
   right, lower left, lower right. */
uint32_t kanji_text_viewer_draw_character(uint32_t place, uint32_t rows)
{
	uint32_t bitmap_place = 0;

	bitmap_place = place;
	kanji_text_viewer_draw_quarter(&bitmap_place, &rows);
	bitmap_place = place + 4u;
	kanji_text_viewer_draw_quarter(&bitmap_place, &rows);
	bitmap_place = place + 0x800u;
	kanji_text_viewer_draw_quarter(&bitmap_place, &rows);
	bitmap_place = place + 0x804u;
	kanji_text_viewer_draw_quarter(&bitmap_place, &rows);
	return bitmap_place;
}

/* Only unused code reaches it (kanji_text_viewer_draw_character): draws eight rows of eight one-bit
   pixels (a byte each, from R13) at R12 on the bitmap layer, 100 bytes a row, each byte spread by the
   table at 1A6340 into eight 4-bit pixels of pen 1. */
void kanji_text_viewer_draw_quarter(uint32_t *p_bitmap_place, uint32_t *p_rows)
{
	uint32_t rows_left = 0;

	rows_left = 8u;
	do
	{
		BITMAP_LAYER_WORDS(*p_bitmap_place)[0] = byte_swap32(kanji_row_pixels[KANJI_CHARACTER_ROWS(*p_rows)[0]]);
		*p_bitmap_place += 0x100u;
		rows_left--;
		(*p_rows)++;
	} while (rows_left != 0u);
}

/* Only unused code reaches it (kanji_text_viewer_draw_line): the address of character R13 of the kanji
   font at 1CF000 (8 bytes a step: a character is 32). */
uint32_t kanji_text_viewer_character_address(uint32_t character)
{
	character = kanji_font_ADDRESS + character * 2u * 4u;
	return character;
}

/* Only unused code reaches it (kanji_text_viewer_update): writes R4 (0-999) as three decimal digits on
   the text layer from cell R3 (attribute 60). */
void kanji_text_viewer_draw_number(uint32_t cell, uint16_t number)
{
	uint16_t hundreds = 0, tens = 0;

	tens = 0u;
	hundreds = 0u;
	while ((int16_t)number >= 0x64)
	{
		number -= 0x64u;
		hundreds++;
	}
	while ((int16_t)number >= 0xA)
	{
		number -= 0xAu;
		tens++;
	}
	TEXT_LAYER_CELLS(cell)[0] = hundreds + 0x6030u;
	cell += 2u;
	TEXT_LAYER_CELLS(cell)[0] = tens + 0x6030u;
	TEXT_LAYER_CELLS(cell)[1] = number + 0x6030u;
}
