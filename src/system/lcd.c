/**
 *    Name:        system/lcd.c
 *    Description: the cabinets' LCD: its driver and the race's information on it
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the cabinets' LCDs at boot (both, through port G of the two I/O chips): the 4-bit start
   (command nibbles 3, 3, 3, 2), display on (0C), SEGA Enterprises as both screens' text (lcd_text),
   command 40 then the eight custom characters (lcd_custom_characters: the DJ's face among them); the
   screens' lamps off (screen button_lamps), lcd_restart_frames E10. */
void lcd_start(void)
{
	uint8_t nibble = 0, to_send = 0;
	uint32_t character_row = 0, text_0_to = 0, text_12_to = 0, text_4_to = 0, text_8_to = 0, wait_count = 0;

	game->lcd_send_step = 0u;
	game->lcd_busy = 0u;
	game->lcd_restart_frames = 0xE10u;
	game->screen_1.button_lamps = 0u;
	game->screen_2.button_lamps = 0u;
	nibble = 0x30u;
	lcd_write_command_nibble(nibble);
	wait_count = 0x2008u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble = 0x30u;
	lcd_write_command_nibble(nibble);
	wait_count = 0xC8u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble = 0x30u;
	lcd_write_command_nibble(nibble);
	nibble = 0x20u;
	lcd_command_nibble_after_pause(nibble);
	nibble = 0u;
	lcd_command_nibble_after_pause(nibble);
	nibble = 0xC0u;
	lcd_command_nibble_after_pause(nibble);
	text_0_to = lcd_boot_text[0].text_0_to_3;
	text_4_to = lcd_boot_text[0].text_4_to_7;
	AS32(game->screen_1.lcd_text[0]) = text_0_to;
	AS32(game->screen_1.lcd_text[4]) = text_4_to;
	AS32(game->screen_2.lcd_text[0]) = text_0_to;
	AS32(game->screen_2.lcd_text[4]) = text_4_to;
	text_8_to = lcd_boot_text[0].text_8_to_11;
	text_12_to = lcd_boot_text[0].text_12_to_15;
	AS32(game->screen_1.lcd_text[8]) = text_8_to;
	AS32(game->screen_1.lcd_text[12]) = text_12_to;
	AS32(game->screen_2.lcd_text[8]) = text_8_to;
	AS32(game->screen_2.lcd_text[12]) = text_12_to;
	to_send = 0x40u;
	lcd_command_nibble_after_pause(to_send);
	to_send = 0u;
	lcd_command_nibble_after_pause(to_send);
	character_row = lcd_custom_characters_ADDRESS;
	for (;;)
	{
		to_send = LCD_CHARACTER_ROWS(character_row)[0];
		character_row++;
		if ((int8_t)to_send < 0)
			break;
		lcd_write_character(to_send);
	}
}

/* Writes the data byte R0 to both LCDs, its high nibble then its low one (lcd_write_data_nibble), with
   pauses. */
void lcd_write_character(uint8_t character)
{
	uint8_t low_nibble = 0;
	uint32_t wait_count = 0;
	uint32_t character_kept = 0;

	wait_count = 0x50u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	character_kept = character;
	character &= ~0xFu;
	lcd_write_data_nibble(character);
	wait_count = 0x50u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	low_nibble = (uint8_t)(character_kept << 4);
	lcd_write_data_nibble(low_nibble);
}

/* Writes the high nibble of R0 to both LCDs as data (port G of the two I/O chips, inverted: the data
   lines, register select set, then the enable strobe bit 3 dropped). */
void lcd_write_data_nibble(uint8_t nibble)
{
	uint32_t wait_count = 0;

	nibble |= 0xFu;
	io_write8(IO_1_PORT_G, ~nibble);
	io_write8(IO_2_PORT_G, ~nibble);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble &= ~8u;
	io_write8(IO_1_PORT_G, ~nibble);
	io_write8(IO_2_PORT_G, ~nibble);
}

/* A pause, then lcd_write_command_nibble. */
void lcd_command_nibble_after_pause(uint8_t nibble)
{
	uint32_t wait_count = 0;

	wait_count = 0x50u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	lcd_write_command_nibble(nibble);
}

/* Writes the high nibble of R0 to both LCDs as a command (register select clear), then drops the enable
   strobe (bit 3). */
void lcd_write_command_nibble(uint8_t nibble)
{
	uint32_t wait_count = 0;

	nibble |= 0xBu;
	io_write8(IO_1_PORT_G, ~nibble);
	io_write8(IO_2_PORT_G, ~nibble);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble &= ~8u;
	io_write8(IO_1_PORT_G, ~nibble);
	io_write8(IO_2_PORT_G, ~nibble);
}

/* After the test mode: both screens' LCD text back to SEGA Enterprises (lcd_text), their lamps off
   (screen button_lamps), and lcd_restart_frames 0 so that the next VBLANK starts the LCDs again. */
void lcd_reset(void)
{
	uint32_t text_0_to = 0, text_12_to = 0, text_4_to = 0, text_8_to = 0;

	game->lcd_restart_frames = 0u;
	game->screen_1.button_lamps = 0u;
	game->screen_2.button_lamps = 0u;
	text_0_to = lcd_boot_text[0].text_0_to_3;
	text_4_to = lcd_boot_text[0].text_4_to_7;
	AS32(game->screen_1.lcd_text[0]) = text_0_to;
	AS32(game->screen_1.lcd_text[4]) = text_4_to;
	AS32(game->screen_2.lcd_text[0]) = text_0_to;
	AS32(game->screen_2.lcd_text[4]) = text_4_to;
	text_8_to = lcd_boot_text[0].text_8_to_11;
	text_12_to = lcd_boot_text[0].text_12_to_15;
	AS32(game->screen_1.lcd_text[8]) = text_8_to;
	AS32(game->screen_1.lcd_text[12]) = text_12_to;
	AS32(game->screen_2.lcd_text[8]) = text_8_to;
	AS32(game->screen_2.lcd_text[12]) = text_12_to;
}

/* Each VBLANK: counts lcd_restart_frames down and, when it runs out, has the LCDs started again
   (lcd_init_step 2C, E10 VBLANKs to the next, a minute); else, unless a nibble is being written
   (lcd_busy), after a byte cut in half (an odd lcd_send_step) writes one more command nibble (0 or 4) to
   keep the LCDs in step, takes both screens' lcd_text into lcd_send_buffer_1 and lcd_send_buffer_2 and
   starts sending them (lcd_send_step 0). */
void lcd_refresh(void)
{
	uint32_t carry = 0;
	uint8_t nibble = 0, send_step = 0;
	uint8_t half_step = 0;
	uint16_t restart_frames = 0;
	uint32_t lcd_text_4 = 0, lcd_send_buffer_1_0_long = 0, lcd_text_12 = 0, lcd_send_buffer_1_8_long = 0, lcd_text_4_2 = 0, lcd_send_buffer_2_0_long = 0, lcd_text_12_2 = 0, lcd_send_buffer_2_8_long = 0, lcd_send_buffer_1_0_long_2 = 0, lcd_send_buffer_1_8_long_2 = 0, lcd_send_buffer_2_0_long_2 = 0, lcd_send_buffer_2_8_long_2 = 0;

	restart_frames = game->lcd_restart_frames;
	game->lcd_restart_frames = restart_frames - 1u;
	if ((int16_t)restart_frames <= 1)
	{
		game->lcd_init_step = 0x2Cu;
		game->lcd_restart_frames = 0xE10u;
		return;
	}
	if (game->lcd_busy != 0u)
		return;
	send_step = game->lcd_send_step;
	carry = send_step & 1u;
	send_step = send_step >> 1;
	if (carry == 0u)
	{
		game->lcd_send_step = 0u;
		lcd_text_4 = AS32(game->screen_1.lcd_text[4]);
		lcd_send_buffer_1_0_long = AS32(game->screen_1.lcd_text[0]);
		game->lcd_send_buffer_1[0] = (uint8_t)lcd_send_buffer_1_0_long;
		game->lcd_send_buffer_1[1] = (uint8_t)(lcd_send_buffer_1_0_long >> 8u);
		game->lcd_send_buffer_1[2] = (uint8_t)(lcd_send_buffer_1_0_long >> 16u);
		game->lcd_send_buffer_1[3] = (uint8_t)(lcd_send_buffer_1_0_long >> 24u);
		game->lcd_send_buffer_1[4] = (uint8_t)lcd_text_4;
		game->lcd_send_buffer_1[5] = (uint8_t)(lcd_text_4 >> 8u);
		game->lcd_send_buffer_1[6] = (uint8_t)(lcd_text_4 >> 16u);
		game->lcd_send_buffer_1[7] = (uint8_t)(lcd_text_4 >> 24u);
		lcd_text_12 = AS32(game->screen_1.lcd_text[12]);
		lcd_send_buffer_1_8_long = AS32(game->screen_1.lcd_text[8]);
		game->lcd_send_buffer_1[8] = (uint8_t)lcd_send_buffer_1_8_long;
		game->lcd_send_buffer_1[9] = (uint8_t)(lcd_send_buffer_1_8_long >> 8u);
		game->lcd_send_buffer_1[10] = (uint8_t)(lcd_send_buffer_1_8_long >> 16u);
		game->lcd_send_buffer_1[11] = (uint8_t)(lcd_send_buffer_1_8_long >> 24u);
		game->lcd_send_buffer_1[12] = (uint8_t)lcd_text_12;
		game->lcd_send_buffer_1[13] = (uint8_t)(lcd_text_12 >> 8u);
		game->lcd_send_buffer_1[14] = (uint8_t)(lcd_text_12 >> 16u);
		game->lcd_send_buffer_1[15] = (uint8_t)(lcd_text_12 >> 24u);
		lcd_text_4_2 = AS32(game->screen_2.lcd_text[4]);
		lcd_send_buffer_2_0_long = AS32(game->screen_2.lcd_text[0]);
		game->lcd_send_buffer_2[0] = (uint8_t)lcd_send_buffer_2_0_long;
		game->lcd_send_buffer_2[1] = (uint8_t)(lcd_send_buffer_2_0_long >> 8u);
		game->lcd_send_buffer_2[2] = (uint8_t)(lcd_send_buffer_2_0_long >> 16u);
		game->lcd_send_buffer_2[3] = (uint8_t)(lcd_send_buffer_2_0_long >> 24u);
		game->lcd_send_buffer_2[4] = (uint8_t)lcd_text_4_2;
		game->lcd_send_buffer_2[5] = (uint8_t)(lcd_text_4_2 >> 8u);
		game->lcd_send_buffer_2[6] = (uint8_t)(lcd_text_4_2 >> 16u);
		game->lcd_send_buffer_2[7] = (uint8_t)(lcd_text_4_2 >> 24u);
		lcd_text_12_2 = AS32(game->screen_2.lcd_text[12]);
		lcd_send_buffer_2_8_long = AS32(game->screen_2.lcd_text[8]);
		game->lcd_send_buffer_2[8] = (uint8_t)lcd_send_buffer_2_8_long;
		game->lcd_send_buffer_2[9] = (uint8_t)(lcd_send_buffer_2_8_long >> 8u);
		game->lcd_send_buffer_2[10] = (uint8_t)(lcd_send_buffer_2_8_long >> 16u);
		game->lcd_send_buffer_2[11] = (uint8_t)(lcd_send_buffer_2_8_long >> 24u);
		game->lcd_send_buffer_2[12] = (uint8_t)lcd_text_12_2;
		game->lcd_send_buffer_2[13] = (uint8_t)(lcd_text_12_2 >> 8u);
		game->lcd_send_buffer_2[14] = (uint8_t)(lcd_text_12_2 >> 16u);
		game->lcd_send_buffer_2[15] = (uint8_t)(lcd_text_12_2 >> 24u);
		return;
	}
	half_step = send_step;
	nibble = (uint8_t)(half_step - 1u);
	if (half_step != 1u)
		nibble = 4u;
	lcd_write_command_nibble(nibble);
	game->lcd_send_step = 0u;
	lcd_text_4 = AS32(game->screen_1.lcd_text[4]);
	lcd_send_buffer_1_0_long_2 = AS32(game->screen_1.lcd_text[0]);
	game->lcd_send_buffer_1[0] = (uint8_t)lcd_send_buffer_1_0_long_2;
	game->lcd_send_buffer_1[1] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 8u);
	game->lcd_send_buffer_1[2] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 16u);
	game->lcd_send_buffer_1[3] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 24u);
	game->lcd_send_buffer_1[4] = (uint8_t)lcd_text_4;
	game->lcd_send_buffer_1[5] = (uint8_t)(lcd_text_4 >> 8u);
	game->lcd_send_buffer_1[6] = (uint8_t)(lcd_text_4 >> 16u);
	game->lcd_send_buffer_1[7] = (uint8_t)(lcd_text_4 >> 24u);
	lcd_text_12 = AS32(game->screen_1.lcd_text[12]);
	lcd_send_buffer_1_8_long_2 = AS32(game->screen_1.lcd_text[8]);
	game->lcd_send_buffer_1[8] = (uint8_t)lcd_send_buffer_1_8_long_2;
	game->lcd_send_buffer_1[9] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 8u);
	game->lcd_send_buffer_1[10] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 16u);
	game->lcd_send_buffer_1[11] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 24u);
	game->lcd_send_buffer_1[12] = (uint8_t)lcd_text_12;
	game->lcd_send_buffer_1[13] = (uint8_t)(lcd_text_12 >> 8u);
	game->lcd_send_buffer_1[14] = (uint8_t)(lcd_text_12 >> 16u);
	game->lcd_send_buffer_1[15] = (uint8_t)(lcd_text_12 >> 24u);
	lcd_text_4_2 = AS32(game->screen_2.lcd_text[4]);
	lcd_send_buffer_2_0_long_2 = AS32(game->screen_2.lcd_text[0]);
	game->lcd_send_buffer_2[0] = (uint8_t)lcd_send_buffer_2_0_long_2;
	game->lcd_send_buffer_2[1] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 8u);
	game->lcd_send_buffer_2[2] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 16u);
	game->lcd_send_buffer_2[3] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 24u);
	game->lcd_send_buffer_2[4] = (uint8_t)lcd_text_4_2;
	game->lcd_send_buffer_2[5] = (uint8_t)(lcd_text_4_2 >> 8u);
	game->lcd_send_buffer_2[6] = (uint8_t)(lcd_text_4_2 >> 16u);
	game->lcd_send_buffer_2[7] = (uint8_t)(lcd_text_4_2 >> 24u);
	lcd_text_12_2 = AS32(game->screen_2.lcd_text[12]);
	lcd_send_buffer_2_8_long_2 = AS32(game->screen_2.lcd_text[8]);
	game->lcd_send_buffer_2[8] = (uint8_t)lcd_send_buffer_2_8_long_2;
	game->lcd_send_buffer_2[9] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 8u);
	game->lcd_send_buffer_2[10] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 16u);
	game->lcd_send_buffer_2[11] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 24u);
	game->lcd_send_buffer_2[12] = (uint8_t)lcd_text_12_2;
	game->lcd_send_buffer_2[13] = (uint8_t)(lcd_text_12_2 >> 8u);
	game->lcd_send_buffer_2[14] = (uint8_t)(lcd_text_12_2 >> 16u);
	game->lcd_send_buffer_2[15] = (uint8_t)(lcd_text_12_2 >> 24u);
}

/* One nibble to the LCDs each timer interrupt (int_vector_43), with the other bits of port G
   (port_g_outputs, the lamps): while lcd_init_step counts, the start sequence (lcd_restart_nibbles, from
   the end), then idle (lcd_send_step 22); else by lcd_send_step: the cursor home command (80), then the
   16 characters of lcd_send_buffer_1 to screen 1's LCD and of lcd_send_buffer_2 to screen 2's, a nibble
   at a time; lcd_busy while it writes the port. */
void lcd_send_next_nibble(void)
{
	uint32_t carry = 0;
	uint8_t screen_1_port = 0, screen_2_port = 0;
	uint32_t send_step = 0, wait_count = 0;
	uint8_t init_step = 0, current_step = 0;

	if (game->lcd_init_step != 0u)
	{
		game->lcd_busy = 0xFFu;
		screen_1_port = (uint8_t)~lcd_restart_nibbles[game->lcd_init_step];
		screen_2_port = screen_1_port | game->port_g_outputs[1];
		io_write8(IO_1_PORT_G, screen_1_port | game->port_g_outputs[0]);
		io_write8(IO_2_PORT_G, screen_2_port);
		init_step = game->lcd_init_step;
		game->lcd_init_step = init_step - 1u;
		if (init_step == 1u)
		{
			game->lcd_send_step = 0x22u;
			game->lcd_busy = 0u;
		}
		return;
	}
	send_step = game->lcd_send_step;
	if (send_step >= 0x22u)
		return;
	game->lcd_busy = 0xFFu;
	game->lcd_send_step++;
	current_step = send_step;
	send_step = (uint8_t)(current_step - 2u);
	if ((int8_t)current_step < 2)
	{
		send_step = (uint8_t)~send_step;
		send_step = (uint8_t)~((send_step << 7) | (send_step >> 1) | 0xBu);
		screen_2_port = (uint8_t)send_step;
		send_step = send_step | game->port_g_outputs[0];
		screen_2_port |= game->port_g_outputs[1];
		io_write8(IO_1_PORT_G, send_step);
		io_write8(IO_2_PORT_G, screen_2_port);
		wait_count = 0x14u;
		do
		{
			wait_count--;
		} while (wait_count != 0u);
		io_write8(IO_1_PORT_G, send_step | 8u);
		io_write8(IO_2_PORT_G, screen_2_port | 8u);
		game->lcd_busy = 0u;
		return;
	}
	carry = send_step & 1u;
	send_step = send_step >> 1;
	screen_2_port = game->lcd_send_buffer_2[send_step];
	screen_1_port = game->lcd_send_buffer_1[send_step];
	if (carry)
	{
		screen_1_port = (uint8_t)((uint32_t)screen_1_port << 4);
		screen_2_port = (uint8_t)((uint32_t)screen_2_port << 4);
	}
	else
	{
		screen_1_port &= ~0xFu;
		screen_2_port &= ~0xFu;
	}
	screen_1_port = (uint8_t)(~(screen_1_port | 0xFu) | game->port_g_outputs[0]);
	screen_2_port = (uint8_t)(~(screen_2_port | 0xFu) | game->port_g_outputs[1]);
	io_write8(IO_1_PORT_G, screen_1_port);
	io_write8(IO_2_PORT_G, screen_2_port);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	io_write8(IO_1_PORT_G, screen_1_port | 8u);
	io_write8(IO_2_PORT_G, screen_2_port | 8u);
	game->lcd_busy = 0u;
}

/* The screen's LCD, each frame. With the DJ's face on (dj_face) its first two characters are the face,
   its mouth the next shape of dj_mouth_shapes every third frame (dj_mouth_frames; dj_face_characters,
   pairs of custom characters), closed when the string ends (a negative byte); the rest of the line (all
   16 without the face) scrolls a text: a new lcd_scroll_request clears the line and starts lcd_scroll,
   which shifts the text in from the right a character every 5 frames (lcd_scroll_frames; ` shows custom
   character 7), then shifts spaces in until the text has gone (lcd_scroll_step 0-3). */
void lcd_update(void)
{
	uint8_t character = 0, space_character = 0;
	uint32_t ignored = 0, lcd_scroll = 0, lcd_scroll_request = 0, mouth_shape = 0, mouth_shapes = 0, scroll_width = 0, text_end = 0, text_place = 0;
	uint8_t dj_mouth_frames = 0, lcd_scroll_frames = 0, lcd_scroll_frames_2 = 0;

	text_place = address_of(screen) + 0xF10u;
	if (screen->dj_face == 0u)
		scroll_width = 0xFu;
	else
	{
		mouth_shapes = screen->dj_mouth_shapes;
		if (mouth_shapes == 0u)
		{
			AS16(SCREEN(text_place - 0xF10u)->lcd_text[0]) = dj_face_characters[0];
			screen->dj_mouth_frames = 3u;
			scroll_width = 0xDu;
			text_place += 2u;
		}
		else
		{
			dj_mouth_frames = screen->dj_mouth_frames;
			screen->dj_mouth_frames = dj_mouth_frames - 1u;
			if (dj_mouth_frames != 1u)
			{
				text_place += 2u;
				scroll_width = 0xDu;
			}
			else
			{
				mouth_shape = DJ_MOUTH_SHAPE_NUMBERS(mouth_shapes)[0];
				mouth_shapes++;
				if ((int8_t)mouth_shape < 0)
				{
					screen->dj_mouth_shapes = 0u;
					AS16(SCREEN(text_place - 0xF10u)->lcd_text[0]) = dj_face_characters[0];
					screen->dj_mouth_frames = 3u;
					scroll_width = 0xDu;
					text_place += 2u;
				}
				else
				{
					AS16(SCREEN(text_place - 0xF10u)->lcd_text[0]) = dj_face_characters[mouth_shape];
					screen->dj_mouth_shapes = mouth_shapes;
					screen->dj_mouth_frames = 3u;
					scroll_width = 0xDu;
					text_place += 2u;
				}
			}
		}
	}
	lcd_scroll_request = screen->lcd_scroll_request;
	if (lcd_scroll_request != 0u)
	{
		screen->lcd_scroll = lcd_scroll_request;
		screen->lcd_scroll_request = 0u;
		screen->lcd_scroll_step = 0u;
		space_character = 0x20u;
		set_bytes(text_place, scroll_width + 1u, space_character);
	}
	lcd_scroll = screen->lcd_scroll;
	if (lcd_scroll == 0u)
		return;
	switch (screen->lcd_scroll_step)
	{
	case 0:
		character = TEXT_CHARACTERS(lcd_scroll)[0];
		screen->lcd_scroll = lcd_scroll + 1u;
		if (character == 0x60u)
			character = 7u;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &text_end, &ignored);
		LCD_CHARACTERS(text_end)[0] = character;
		screen->lcd_scroll_frames = 5u;
		screen->lcd_scroll_step++;
		lcd_scroll_frames = screen->lcd_scroll_frames;
		screen->lcd_scroll_frames = lcd_scroll_frames - 1u;
		if (lcd_scroll_frames != 1u)
			return;
		if (TEXT_CHARACTERS(screen->lcd_scroll)[0] != 0u)
		{
			screen->lcd_scroll_step--;
			return;
		}
		screen->lcd_scroll = scroll_width + 1u;
		screen->lcd_scroll_step++;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &text_end, &ignored);
		LCD_CHARACTERS(text_end)[0] = 0x20u;
		screen->lcd_scroll_frames = 5u;
		screen->lcd_scroll_step++;
		break;
	case 1:
		lcd_scroll_frames = screen->lcd_scroll_frames;
		screen->lcd_scroll_frames = lcd_scroll_frames - 1u;
		if (lcd_scroll_frames != 1u)
			return;
		if (TEXT_CHARACTERS(screen->lcd_scroll)[0] != 0u)
		{
			screen->lcd_scroll_step--;
			return;
		}
		screen->lcd_scroll = scroll_width + 1u;
		screen->lcd_scroll_step++;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &text_end, &ignored);
		LCD_CHARACTERS(text_end)[0] = 0x20u;
		screen->lcd_scroll_frames = 5u;
		screen->lcd_scroll_step++;
		break;
	case 2:
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &text_end, &ignored);
		LCD_CHARACTERS(text_end)[0] = 0x20u;
		screen->lcd_scroll_frames = 5u;
		screen->lcd_scroll_step++;
		lcd_scroll_frames_2 = screen->lcd_scroll_frames;
		screen->lcd_scroll_frames = lcd_scroll_frames_2 - 1u;
		if (lcd_scroll_frames_2 == 1u)
		{
			screen->lcd_scroll--;
			screen->lcd_scroll_step--;
		}
		return;
	case 3:
		break;
	}
	lcd_scroll_frames_2 = screen->lcd_scroll_frames;
	screen->lcd_scroll_frames = lcd_scroll_frames_2 - 1u;
	if (lcd_scroll_frames_2 != 1u)
		return;
	screen->lcd_scroll--;
	screen->lcd_scroll_step--;
}

/* Clears the cabinet's LCD: sixteen spaces, no DJ message (screen dj_mouth_shapes), no scrolling text
   (lcd_scroll_request, lcd_scroll), the DJ's face off (dj_face). */
void clear_lcd(void)
{
	struct screen *text_place = NULL;

	text_place = screen;
	AS32(text_place->lcd_text[0]) = 0x20202020u;
	AS32(text_place->lcd_text[4]) = 0x20202020u;
	AS32(text_place->lcd_text[8]) = 0x20202020u;
	AS32(text_place->lcd_text[12]) = 0x20202020u;
	screen->dj_mouth_shapes = 0u;
	screen->dj_mouth_frames = 3u;
	screen->dj_face = 0u;
	screen->lcd_scroll_request = 0u;
	screen->lcd_scroll = 0u;
}

/* Starts the race information on the cabinet LCD: clears it (clear_lcd), shows the DJ's face in its
   first two characters (screen dj_face) and goes on to lcd_race_info_update. */
void lcd_race_info_start(void)
{
	clear_lcd();
	screen->dj_face = 0xFFu;
	current_object->handler = lcd_race_info_update_ADDRESS;
	lcd_race_info_update();
}

/* Shows the race on the cabinet LCD. Before the start, while the car object's timer counts the frames to
   it: a bar shrinking through each second (lcd_countdown_bars) with the seconds left. Then a page every
   150 frames (page_frames), by page: 0 Time (lcd_race_info_time), 1 Order (the car numbers, car_numbers
   + 1, in race order from order_list, this screen's own blinking; skipped with one player), 2 Speed
   (screen shown_speed), 3 Max. (screen maximum_speed), 4 Ave. (lcd_race_info_average); page_step 0
   writes the label, 1 updates the number. When the car reaches the goal (car at_goal) goes to
   lcd_race_info_finish; when the race is over (car race_over) shows the average and ends; while the LCD
   scrolls a text (screen lcd_scroll) waits in lcd_race_info_paused. */
void lcd_race_info_update(void)
{
	uint8_t character = 0, page = 0;
	uint32_t countdown = 0, countdown_frames = 0, maximum_speed = 0, order_list = 0, racer = 0, racers_left = 0, shown_speed = 0, text_place = 0;
	uint16_t page_frames = 0;

	if (player_car->at_goal != 0u)
	{
		current_object->state = 0u;
		current_object->handler = lcd_race_info_finish_ADDRESS;
		lcd_race_info_finish();
		return;
	}
	if (player_car->race_over != 0u)
	{
		current_object->flags = 0u;
		lcd_race_info_average_page();
		return;
	}
	if (screen->lcd_scroll != 0u)
	{
		current_object->state = 1u;
		current_object->handler = lcd_race_info_paused_ADDRESS;
		lcd_race_info_paused();
		return;
	}
	switch (current_object->state)
	{
	case 0:
		countdown = OBJECT(player_car->object)->player_car.timer;
		if (countdown != 0u)
		{
			countdown_frames = 0u;
			divux_pair(0x1Eu, &countdown, &countdown_frames);
			move_halfwords(address_of(screen) + 0xF12u, lcd_countdown_bars[(countdown_frames + 2u) / 5u], 7u);
			screen->lcd_text[9] = countdown + 0x31u;
			return;
		}
		current_object->lcd_race_info.racer_count = RACE(screen->host_record)->racer_count;
		current_object->lcd_race_info.order_list = screen->host_record + 0x20u;
		current_object->lcd_race_info.page_frames = 0x96u;
		current_object->state++;
		current_object->lcd_race_info.page_step = 0u;
		current_object->state++;
		break;
	case 1:
		current_object->lcd_race_info.page_step = 0u;
		current_object->state++;
		break;
	case 2:
		break;
	}
	page_frames = current_object->lcd_race_info.page_frames;
	current_object->lcd_race_info.page_frames = page_frames - 1u;
	if (page_frames == 1u)
	{
		page = (uint8_t)(current_object->lcd_race_info.page + 1u);
		if ((int8_t)page >= 5)
			page = 0u;
		current_object->lcd_race_info.page = page;
		current_object->lcd_race_info.page_step = 0u;
		current_object->lcd_race_info.page_frames = 0x96u;
	}
	switch (current_object->lcd_race_info.page)
	{
	case 0:
		switch (current_object->lcd_race_info.page_step)
		{
		case 0:
			lcd_race_info_time_page();
			return;
		case 1:
			lcd_race_info_time();
			return;
		}
		break;
	case 1:
		break;
	case 2:
		switch (current_object->lcd_race_info.page_step)
		{
		case 0:
			move_halfwords(address_of(screen) + 0xF12u, lcd_speed_label_ADDRESS, 7u);
			current_object->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		}
		text_place = address_of(screen) + 0xF1Cu;
		shown_speed = screen->shown_speed;
		lcd_race_info_write_number(shown_speed, text_place);
		return;
	case 3:
		switch (current_object->lcd_race_info.page_step)
		{
		case 0:
			move_halfwords(address_of(screen) + 0xF12u, lcd_maximum_label_ADDRESS, 7u);
			current_object->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		}
		text_place = address_of(screen) + 0xF1Cu;
		maximum_speed = (uint16_t)screen->maximum_speed;
		lcd_race_info_write_number(maximum_speed, text_place);
		return;
	case 4:
		switch (current_object->lcd_race_info.page_step)
		{
		case 0:
			lcd_race_info_average_page();
			return;
		case 1:
			lcd_race_info_average();
			return;
		}
		break;
	}
	switch (current_object->lcd_race_info.page_step)
	{
	case 0:
		if (RACE(screen->host_record)->racer_count == 1u)
		{
			current_object->lcd_race_info.page++;
			switch (current_object->lcd_race_info.page_step)
			{
			case 0:
				move_halfwords(address_of(screen) + 0xF12u, lcd_speed_label_ADDRESS, 7u);
				current_object->lcd_race_info.page_step++;
				break;
			case 1:
				break;
			}
			text_place = address_of(screen) + 0xF1Cu;
			shown_speed = screen->shown_speed;
			lcd_race_info_write_number(shown_speed, text_place);
			return;
		}
		move_halfwords(address_of(screen) + 0xF12u, lcd_order_label_ADDRESS, 7u);
		current_object->lcd_race_info.frames = 0u;
		current_object->lcd_race_info.page_step++;
		break;
	case 1:
		break;
	}
	text_place = address_of(screen) + 0xF18u;
	order_list = current_object->lcd_race_info.order_list;
	racers_left = current_object->lcd_race_info.racer_count;
	do
	{
		racer = RACE_ORDER_PLAYERS(order_list)[0];
		order_list++;
		if (screen->player != racer || (current_object->lcd_race_info.frames & 8u) == 0u)
			character = (uint8_t)(game->car_numbers[racer] + 0x31u);
		else
			character = 0x20u;
		LCD_CHARACTERS(text_place)[0] = character;
		racers_left--;
		text_place++;
	} while (racers_left != 0u);
	current_object->lcd_race_info.frames++;
}

/* Writes the Time page's label on the LCD, then the time (lcd_race_info_time). */
void lcd_race_info_time_page(void)
{
	move_halfwords(address_of(screen) + 0xF12u, lcd_time_label_ADDRESS, 7u);
	current_object->lcd_race_info.page_step++;
	lcd_race_info_time();
}

/* Writes the race time (the car's race_time, in thousandths of a second) on the LCD as
   minutes'seconds"hundredths. */
void lcd_race_info_time(void)
{
	uint32_t digit = 0, race_time = 0, text_place = 0;

	race_time = player_car->race_time / 0xAu;
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place = address_of(screen) + 0xF20u - 1u;
	LCD_CHARACTERS(text_place)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place--;
	LCD_CHARACTERS(text_place)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place = text_place - 1u - 1u;
	LCD_CHARACTERS(text_place)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(6u, &race_time, &digit);
	text_place--;
	LCD_CHARACTERS(text_place)[0] = digit + 0x30u;
	LCD_CHARACTERS(text_place)[-2] = race_time + 0x30u;
}

/* Writes the Ave. page's label on the LCD, then the average speed (lcd_race_info_average). */
void lcd_race_info_average_page(void)
{
	move_halfwords(address_of(screen) + 0xF12u, lcd_average_label_ADDRESS, 7u);
	current_object->lcd_race_info.page_step++;
	lcd_race_info_average();
}

/* Writes the average speed on the LCD: the sum of the speeds (screen speed_sum) over how many there are
   (speed_samples), with lcd_race_info_write_number. */
void lcd_race_info_average(void)
{
	uint32_t speed_samples = 0, speed_sum = 0, text_place = 0;

	text_place = address_of(screen) + 0xF1Cu;
	speed_sum = screen->speed_sum;
	speed_samples = screen->speed_samples;
	if (speed_samples == 0u)
	{
		lcd_race_info_write_number(speed_sum, text_place);
		return;
	}
	speed_sum = divide_u32(speed_sum, speed_samples);
	lcd_race_info_write_number(speed_sum, text_place);
}

/* Writes a number of up to three digits on the LCD, right-aligned to end before the address it is given
   (F1C of the screen for the km/h pages), spaces for the leading zeros. */
void lcd_race_info_write_number(uint32_t speed, uint32_t text_place)
{
	uint32_t zero = 0;
	uint8_t digit_zero = 0;
	uint32_t digit = 0;

	digit_zero = 0x30u;
	digit = 0u;
	divx_pair(0xAu, &speed, &digit);
	zero = (uint32_t)(speed == 0u);
	if (zero)
	{
		text_place--;
		SCREEN(text_place - 0xF1Bu)->lcd_text[11] = digit + digit_zero;
		text_place--;
		SCREEN(text_place - 0xF1Au)->lcd_text[10] = 0x20u;
		SCREEN(text_place - 0xF1Au)->lcd_text[9] = 0x20u;
		return;
	}
	text_place--;
	SCREEN(text_place - 0xF1Bu)->lcd_text[11] = digit + digit_zero;
	digit = 0u;
	divx_pair(0xAu, &speed, &digit);
	zero = (uint32_t)(speed == 0u);
	if (zero)
	{
		text_place--;
		SCREEN(text_place - 0xF1Au)->lcd_text[10] = digit + digit_zero;
		SCREEN(text_place - 0xF1Au)->lcd_text[9] = 0x20u;
		return;
	}
	text_place--;
	SCREEN(text_place - 0xF1Au)->lcd_text[10] = digit + digit_zero;
	if (speed == 0u)
	{
		SCREEN(text_place - 0xF1Au)->lcd_text[9] = 0x20u;
		return;
	}
	SCREEN(text_place - 0xF1Au)->lcd_text[9] = speed + digit_zero;
}

/* The car has reached the goal: shows */
void lcd_race_info_finish(void)
{
	uint16_t frames = 0;

	switch (current_object->state)
	{
	case 0:
		move_halfwords(address_of(screen) + 0xF12u, lcd_finish_label_ADDRESS, 7u);
		current_object->state++;
		if (screen->lcd_scroll == 0u)
			return;
		current_object->state++;
		break;
	case 1:
		if (screen->lcd_scroll == 0u)
			return;
		current_object->state++;
		if (screen->lcd_scroll != 0u)
			return;
		lcd_race_info_average_page();
		current_object->lcd_race_info.frames = 0x96u;
		current_object->state++;
		frames = current_object->lcd_race_info.frames;
		current_object->lcd_race_info.frames = frames - 1u;
		if (frames != 1u)
			return;
		current_object->flags = 0u;
		lcd_race_info_time_page();
		return;
	case 2:
		break;
	case 3:
		frames = current_object->lcd_race_info.frames;
		current_object->lcd_race_info.frames = frames - 1u;
		if (frames != 1u)
			return;
		current_object->flags = 0u;
		lcd_race_info_time_page();
		return;
	}
	if (screen->lcd_scroll != 0u)
		return;
	lcd_race_info_average_page();
	current_object->lcd_race_info.frames = 0x96u;
	current_object->state++;
	frames = current_object->lcd_race_info.frames;
	current_object->lcd_race_info.frames = frames - 1u;
	if (frames != 1u)
		return;
	current_object->flags = 0u;
	lcd_race_info_time_page();
}

/* Waits while the LCD scrolls a text (screen lcd_scroll), then goes back to lcd_race_info_update, which
   writes the page's label again. */
void lcd_race_info_paused(void)
{
	if (screen->lcd_scroll == 0u)
	{
		current_object->handler = lcd_race_info_update_ADDRESS;
		lcd_race_info_update();
		return;
	}
}
