/**
 *    Name:        video/text.c
 *    Description: the text: the fonts, the text layer's strings, numbers in hexadecimal and BCD, text in sprites
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Copies the text layer's characters (3000 bytes, 384 characters of 8x8 pixels) from 104020 into video
   RAM at 318000. */
void load_text_font(void)
{
	move_string(text_font_ADDRESS, 0x1800u, VIDEO_RAM + 0x18000u, 0x1800u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: builds the text layer's first 128 characters (318000, where load_text_font puts its 384) from
   the one-bit font at 103C20 (8 bytes a character, a bit a pixel): the character's pixels in pen 1, and
   a shadow in pen 2 one pixel right and one row down under them. */
void load_shadowed_text_font(void)
{
	uint32_t carry = 0;
	uint8_t row_bits = 0;
	uint32_t characters_left = 0, destination = 0, font_rows = 0, pen = 0, pixel_mask = 0, pixel_pairs_left = 0, row_pixels = 0, rows_left = 0, shadow = 0, shadow_pixels = 0;

	font_rows = one_bit_font_ADDRESS;
	destination = VIDEO_RAM + 0x18000u;
	pen = 1u;
	characters_left = 0x80u;
	do
	{
		rows_left = 8u;
		shadow = 0u;
		do
		{
			row_bits = TEXT_FONT_ROWS(font_rows)[0];
			pixel_pairs_left = 4u;
			row_pixels = 0u;
			font_rows++;
			do
			{
				carry = (uint8_t)(row_bits * 2u) < row_bits;
				row_bits = (uint8_t)(row_bits * 2u);
				row_pixels = rotate_with_carry_by(row_pixels, 4u, carry, 32);
				carry = (uint8_t)(row_bits * 2u) < row_bits;
				row_bits = (uint8_t)(row_bits * 2u);
				row_pixels = rotate_with_carry_by(row_pixels, 4u, carry, 32);
				pixel_pairs_left--;
			} while (pixel_pairs_left != 0u);
			row_pixels = (row_pixels << 29) | (row_pixels >> 3);
			shadow_pixels = row_pixels;
			pixel_mask = row_pixels * 0xFu;
			row_pixels = row_pixels * pen;
			mem_write32(destination, byte_swap32((shadow & ~pixel_mask) | row_pixels));
			shadow = (row_pixels + shadow_pixels) >> 4;
			destination += 4u;
			rows_left--;
		} while (rows_left != 0u);
		characters_left--;
	} while (characters_left != 0u);
}

/* Writes the strings of the list R19 on the text layer (print_text_layer each) until an FFFF halfword.
   Not run in the scenarios. */
void print_text_layer_list(uint32_t text_record)
{
	do
	{
		text_record = print_text_layer(text_record);
	} while (mem_read16(text_record) != 0xFFFFu);
}

/* Writes a string on the text layer: from the record R19, the cell's address (310000 + its first
   halfword), an attribute byte (the cells' high byte: palette) and the characters, closed by 0; returns
   the address after it. */
uint32_t print_text_layer(uint32_t text_record)
{
	uint16_t cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	text_record += 2u;
	cell = (uint16_t)((uint32_t)mem_read8(text_record) << 8);
	text_record++;
	cell |= mem_read8(text_record);
	text_record++;
	if ((uint8_t)cell != 0u)
	{
		do
		{
			mem_write16(cell_place, cell);
			cell = (uint16_t)((cell & ~0xFFu) | mem_read8(text_record));
			cell_place += 2u;
			text_record++;
		} while ((uint8_t)cell != 0u);
	}
	return text_record;
}

/* Unused: erases a string from the text layer: from the cell of the print_text_layer record R19, writes
   0 cells until the next cell's character is 0. */
void erase_text_layer_to_blank_character(uint32_t text_record)
{
	uint16_t blank_cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	blank_cell = 0u;
	do
	{
		mem_write16(cell_place, blank_cell);
		cell_place += 2u;
	} while (mem_read8(cell_place) != 0u);
}

/* Unused: print_text_layer with the characters as halfwords: from the record R19, the cell's address
   (310000 + its first halfword), the attribute byte, then the characters, 16 bits each (ORed with the
   attribute << 8, so they can reach past 255: the font has 384), closed by a 0 halfword. */
void print_text_layer_halfwords(uint32_t text_record)
{
	uint16_t attribute = 0, character = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	text_record += 2u;
	attribute = (uint16_t)((uint32_t)mem_read8(text_record) << 8);
	text_record++;
	character = mem_read16(text_record);
	text_record += 2u;
	if (character != 0u)
	{
		do
		{
			mem_write16(cell_place, character | attribute);
			character = mem_read16(text_record);
			cell_place += 2u;
			text_record += 2u;
		} while (character != 0u);
	}
}

/* Unused: erases a string from the text layer as erase_text_layer_to_blank_character does, but until the
   next cell is all 0 (character and attribute). */
void erase_text_layer_to_blank_cell(uint32_t text_record)
{
	uint16_t blank_cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	blank_cell = 0u;
	do
	{
		mem_write16(cell_place, blank_cell);
		cell_place += 2u;
	} while (mem_read16(cell_place) != 0u);
}

/* Never runs in the scenarios (ISDN only): writes a number's low byte as two hex digits on the text
   layer, ending before the cell it is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_byte(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number = (uint8_t)number;
	digit_count = 2u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a number's low halfword as four hex digits on the text layer, ending before the cell it
   is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_halfword(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number = (uint16_t)number;
	digit_count = 4u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a number's low 24 bits as six hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits). */
void print_hex_24_bits(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number &= 0xFFFFFFu;
	digit_count = 6u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a 32-bit number as eight hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits, which it runs on into). */
void print_hex_word(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	digit_count = 8u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Writes a number in hex on the text layer, in as many cells as it is told, ending before the cell it is
   given, the lowest digit last: each digit a character ('0'-'9', 'A'-'F') with the attribute byte it is
   given; when the number runs out, the cells left before it are cleared (0). Only print_hex_byte runs
   it; the entries for four, six and eight digits before it (print_hex_halfword, print_hex_24_bits,
   print_hex_word) are unused. */
void print_hex_digits(uint32_t number, uint16_t cell, uint32_t digit_count, uint32_t cell_place)
{
	uint32_t higher_digits = 0;

	cell_place += digit_count * 2u;
	do
	{
		cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)number);
		cell = (uint16_t)((cell & ~0xFFu) | (cell & 0xFu));
		if ((uint8_t)cell >= 0xAu)
			cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)(cell + 7u));
		cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)(cell + 0x30u));
		cell_place -= 2u;
		mem_write16(cell_place, cell);
		higher_digits = number >> 4;
		number = higher_digits;
		digit_count--;
	} while ((digit_count != 0u) & (higher_digits != 0u));
	if (digit_count != 0u)
	{
		do
		{
			cell_place -= 2u;
			mem_write16(cell_place, number);
			digit_count--;
		} while (digit_count != 0u);
	}
}

/* Unused: converts R0's eight decimal digits (four bits each, the highest first) into a number, returned
   in R0: eight times, the result so far times 10 plus the next digit. The inverse of binary_to_bcd. */
uint32_t bcd_to_binary(uint32_t value)
{
	uint32_t digits_left = 0, number = 0, total = 0;

	digits_left = 8u;
	total = 0u;
	do
	{
		value = (value << 4) | (value >> 28);
		total = total * 0xAu + (0xFu & value);
		digits_left--;
	} while (digits_left != 0u);
	number = total;
	return number;
}

/* Converts R0 into eight decimal digits, one per four bits, the units in the lowest. */
uint32_t binary_to_bcd(uint32_t number)
{
	uint32_t bcd = 0, digit = 0, digits = 0, digits_left = 0;

	digits_left = 8u;
	digits = 0u;
	do
	{
		digit = 0u;
		divux_pair(0xAu, &number, &digit);
		digits += digit;
		digits = (digits << 28) | (digits >> 4);
		digits_left--;
	} while ((digits_left != 0u) & (number != 0u));
	if (number == 0u && digits_left != 0u)
	{
		do
		{
			digits = (digits << 28) | (digits >> 4);
			digits_left--;
		} while (digits_left != 0u);
	}
	bcd = digits;
	return bcd;
}

/* Draws the string at R18 as draw_text does (a sprite per character into the screen's sprite list), at
   the place R3 holds (x in its low half, y in its high half, as a text record's first 32 bits) with the
   attribute R5. Used by stage_result_line, stage_indicator_draw and song_title_show (the song title). */
void draw_text_at(uint32_t place, uint16_t attribute, uint32_t text)
{
	uint16_t font = 0;
	uint32_t sprite = 0, sprite_copy = 0, sprite_size = 0, sprite_words_0_1 = 0;

	sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite)
		return;
	sprite_copy = sprite + 0x3000u;
	sprite_words_0_1 = text_sprite_words_0_1;
	place = (place << 16) | (place >> 16);
	sprite_size = text_sprite_size | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	font = text_sprite_font;
	attribute |= text_sprite_palette;
	text = draw_text_sprites(sprite_words_0_1, sprite_size, place, font, attribute, sprite, sprite_copy, text);
}

/* Draws the text record R19 (x and y, an attribute, a string closed by 0) as one sprite per character
   and returns the address of the record after it. */
uint32_t draw_text(uint32_t text_record)
{
	uint16_t attribute = 0, font = 0;
	uint32_t place = 0, sprite = 0, sprite_copy = 0, sprite_size = 0, sprite_words_0_1 = 0, text = 0;

	text = text_record + 6u;
	sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite)
		return text;
	sprite_copy = sprite + 0x3000u;
	sprite_words_0_1 = text_sprite_words_0_1;
	sprite_size = text_sprite_size | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	place = mem_read32(text_record);
	place = (place << 16) | (place >> 16);
	font = text_sprite_font;
	attribute = text_sprite_palette | mem_read16(text_record + 4u);
	return draw_text_sprites(sprite_words_0_1, sprite_size, place, font, attribute, sprite, sprite_copy, text);
}

/* Writes a sprite per character of the string at R18 (closed by 0) into the sprite list and its halfway
   copy alike: words 0-3 from R1 and R2 (an 8x8 piece, with the screen's bit), the place R3 (y, x; x 8
   more each character), the graphics R4 + 8 * the character, the attribute R5; then links them into
   depth layer 19E (screen 5678, drawn after nearly all the others) and counts them (sprite_groups).
   Returns the address after the string. The end of draw_text and draw_text_at. */
uint32_t draw_text_sprites(uint32_t sprite_words_0_1, uint32_t sprite_size, uint32_t place, uint16_t font, uint16_t attribute, uint32_t sprite, uint32_t sprite_copy, uint32_t text)
{
	uint16_t character = 0;
	uint32_t depth_layer = 0, first_sprite = 0, text_depth_list_head = 0, x_step = 0;
	uint8_t next_character = 0;

	character = mem_read8(text);
	x_step = 0x80000u;
	text++;
	do
	{
		character = (uint16_t)(((uint32_t)character << 3) + font);
		mem_write32(sprite, sprite_words_0_1);
		mem_write32(sprite_copy, sprite_words_0_1);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write32(sprite, sprite_size);
		mem_write32(sprite_copy, sprite_size);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write32(sprite, place);
		mem_write32(sprite_copy, place);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write16(sprite, character);
		mem_write16(sprite_copy, character);
		sprite_copy += 2u;
		sprite += 2u;
		mem_write16(sprite, attribute);
		mem_write16(sprite_copy, attribute);
		sprite_copy += 2u;
		place += x_step;
		next_character = mem_read8(text);
		text++;
		character = next_character;
		sprite += 2u;
	} while (character != 0u);
	depth_layer = screen + 0x5678u;
	text_depth_list_head = SCREEN(depth_layer - 0x5678u)->text_depth_list_head;
	if (text_depth_list_head == 0u)
		SCREEN(depth_layer - 0x5678u)->text_depth_list_tail = sprite;
	first_sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	mem_write32(sprite, text_depth_list_head);
	mem_write32(sprite_copy, (uint16_t)(text_depth_list_head + 0x300u));
	SCREEN(screen)->sprite_list_last = sprite;
	SCREEN(depth_layer - 0x5678u)->text_depth_list_head = (first_sprite >> 4) | 0x8000u;
	SCREEN(screen)->sprite_groups++;
	return text;
}
