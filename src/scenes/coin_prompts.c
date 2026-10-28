/**
 *    Name:        scenes/coin_prompts.c
 *    Description: the prompts for coins and the ignition key, and the credits shown
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    28-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The prompt at the bottom of the title, the car presentations and the demo races, and its text on the
   cabinet's LCD; ends at once in game mode 6 (screen game_mode). Every 30 frames (round_frames): a
   little car crosses the message (coin_prompt_car with this round's car; next_car then steps through the
   eight, from screen car_number) and the message by the seat (seat1_credits and seat1_coin_units for
   screen 1, seat2_credits and seat2_coin_units for screen 2): with free play (free_play) or a credit
   message 2, TURN THE IGNITION KEY (coin_prompt_start_key_prompt); else message 1 with coins in toward a
   credit or 0, a coin_prompt_message in the car's colour (INSERT COIN(S)). Each frame the LCD's text to
   scroll (screen lcd_scroll_request) is the message's (coin_prompt_lcd_texts: Insert Coin(s), Insert
   More Coin(s), Turn the Ignition Key), except in game modes 6 and 7 or while a text scrolls
   (lcd_scroll). */
void coin_prompt_update(void)
{
	uint8_t coin_units = 0, credits = 0, first_piece = 0, message = 0;
	uint32_t object = 0, piece_count = 0;
	uint8_t round_frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (SCREEN(screen)->game_mode == 6u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			return;
		}
		OBJECT(current_object)->coin_prompt.next_car = SCREEN(screen)->car_number;
		OBJECT(current_object)->state++;
		break;
	case 1:
		round_frames = OBJECT(current_object)->coin_prompt.round_frames;
		OBJECT(current_object)->coin_prompt.round_frames = round_frames - 1u;
		if (round_frames != 1u)
		{
			if (SCREEN(screen)->game_mode != 6u && SCREEN(screen)->game_mode != 7u && SCREEN(screen)->lcd_scroll == 0u)
				SCREEN(screen)->lcd_scroll_request = coin_prompt_lcd_texts[OBJECT(current_object)->coin_prompt.message];
			return;
		}
		break;
	}
	OBJECT(current_object)->coin_prompt.round_frames = 0x1Eu;
	OBJECT(current_object)->coin_prompt.car = OBJECT(current_object)->coin_prompt.next_car;
	object = start_object();
	OBJECT(object)->handler = coin_prompt_car_drive_ADDRESS;
	OBJECT(object)->coin_prompt_car.car = OBJECT(current_object)->coin_prompt.next_car;
	OBJECT(object)->coin_prompt_car.car_colour = OBJECT(current_object)->coin_prompt.next_car;
	OBJECT(current_object)->coin_prompt.next_car++;
	OBJECT(current_object)->coin_prompt.next_car &= 7u;
	if (free_play != 0u)
	{
		message = 2u;
		piece_count = 2u;
		first_piece = 0u;
		object = coin_prompt_start_key_prompt(piece_count, first_piece);
	}
	else
	{
		switch (SCREEN(screen)->index)
		{
		case 0:
			credits = seat1_credits;
			coin_units = seat1_coin_units;
			break;
		case 1:
			credits = seat2_credits;
			coin_units = seat2_coin_units;
			break;
		}
		if (credits != 0u)
		{
			message = 2u;
			piece_count = 2u;
			first_piece = 0u;
			object = coin_prompt_start_key_prompt(piece_count, first_piece);
			OBJECT(current_object)->coin_prompt.message = message;
			OBJECT(object)->unk_0B = OBJECT(current_object)->coin_prompt.car;
			if (SCREEN(screen)->game_mode != 6u && SCREEN(screen)->game_mode != 7u && SCREEN(screen)->lcd_scroll == 0u)
				SCREEN(screen)->lcd_scroll_request = coin_prompt_lcd_texts[OBJECT(current_object)->coin_prompt.message];
			return;
		}
		if (coin_units != 0u)
			message = 1u;
		else
			message = 0u;
		object = start_object();
		OBJECT(object)->handler = coin_prompt_message_update_ADDRESS;
		OBJECT(object)->coin_prompt_message.prompt = current_object;
		OBJECT(object)->coin_prompt_message.message = message;
	}
	OBJECT(current_object)->coin_prompt.message = message;
	OBJECT(object)->unk_0B = OBJECT(current_object)->coin_prompt.car;
	if (SCREEN(screen)->game_mode != 6u && SCREEN(screen)->game_mode != 7u && SCREEN(screen)->lcd_scroll == 0u)
		SCREEN(screen)->lcd_scroll_request = coin_prompt_lcd_texts[OBJECT(current_object)->coin_prompt.message];
}

/* The little car that drives across the prompt from right to left each round (coin_prompt starts it):
   one of the eight cars (car) at a quarter of its size, 14 a frame, ending at the left edge. */
void coin_prompt_car_drive(void)
{
	uint32_t animation = 0;
	uint16_t x = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		animation = coin_prompt_car_animations[OBJECT(current_object)->coin_prompt_car.car];
		animation_start(animation);
		OBJECT(current_object)->palette = coin_prompt_car_palettes[OBJECT(current_object)->coin_prompt_car.car_colour] + 4u;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		OBJECT(current_object)->x = 0x140u;
		OBJECT(current_object)->y = 0xC0u;
		OBJECT(current_object)->frame = 4u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	}
	x = OBJECT(current_object)->x - 0xEu;
	OBJECT(current_object)->x = x;
	if ((int16_t)x >= 0)
	{
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* The prompt's message for a round (coin_prompt starts one every 30 frames): after 6 frames, INSERT
   COIN(S) (message 0; 1 when coins are in, animation coin_prompt_message_animations) in the colour of
   the round's car (coin_prompt_message_palettes by car_colour), animating in for 22 frames over the one
   before, then a layer back for 30 frames, and ends; ends at once when the prompt's message changes. */
void coin_prompt_message_update(void)
{
	uint32_t animation = 0;
	uint8_t frames = 0, frames_2 = 0, frames_3 = 0;

	if (OBJECT(OBJECT(current_object)->coin_prompt_message.prompt)->coin_prompt.message != OBJECT(current_object)->coin_prompt_message.message)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->coin_prompt_message.frames = 6u;
		OBJECT(current_object)->state++;
		frames = OBJECT(current_object)->coin_prompt_message.frames;
		OBJECT(current_object)->coin_prompt_message.frames = frames - 1u;
		if (frames != 1u)
			return;
		OBJECT(current_object)->draw_mode = 0u;
		animation = coin_prompt_message_animations[OBJECT(current_object)->coin_prompt_message.message];
		animation_start(animation);
		OBJECT(current_object)->palette = coin_prompt_message_palettes[OBJECT(current_object)->coin_prompt_message.car_colour];
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xC0u;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->coin_prompt_message.frames = 0x16u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		frames = OBJECT(current_object)->coin_prompt_message.frames;
		OBJECT(current_object)->coin_prompt_message.frames = frames - 1u;
		if (frames != 1u)
			return;
		OBJECT(current_object)->draw_mode = 0u;
		animation = coin_prompt_message_animations[OBJECT(current_object)->coin_prompt_message.message];
		animation_start(animation);
		OBJECT(current_object)->palette = coin_prompt_message_palettes[OBJECT(current_object)->coin_prompt_message.car_colour];
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xC0u;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->coin_prompt_message.frames = 0x16u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	case 3:
		frames_2 = OBJECT(current_object)->coin_prompt_message.frames;
		OBJECT(current_object)->coin_prompt_message.frames = frames_2 - 1u;
		if (frames_2 != 1u)
		{
			draw_object(0u);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	}
	animation_advance(0u);
	frames_3 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_3 - 1u;
	if (frames_3 != 1u)
	{
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->depth_layer--;
	OBJECT(current_object)->coin_prompt_message.frames = 0x1Eu;
	OBJECT(current_object)->state++;
	frames_2 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* Starts R1 pieces of the ignition key prompt (ignition_key_prompt), pieces R2 on, and returns the last
   one. */
uint32_t coin_prompt_start_key_prompt(uint32_t piece_count, uint8_t first_piece)
{
	uint32_t object = 0;

	do
	{
		object = start_object();
		OBJECT(object)->handler = ignition_key_prompt_update_ADDRESS;
		OBJECT(object)->ignition_key_prompt.piece = first_piece;
		first_piece++;
		piece_count--;
	} while (piece_count != 0u);
	return object;
}

/* A piece of the prompt when a game can start (coin_prompt starts both every round): by piece, from its
   record at 01A95B (animation, palette, width_scale, height_scale, x, y, depth_layer, frames_left): 0
   the words TURN THE IGNITION KEY, 1 the START / IGNITION key with its arrow; animates until frames_left
   runs out (24 or 30 frames) and ends. */
void ignition_key_prompt_update(void)
{
	uint32_t animation = 0, piece = 0;
	uint16_t frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		frames_left = OBJECT(current_object)->ignition_key_prompt.frames_left;
		OBJECT(current_object)->ignition_key_prompt.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->draw_mode = 0u;
	piece = ignition_key_prompt_pieces_ADDRESS + ((uint32_t)OBJECT(current_object)->ignition_key_prompt.piece << 4);
	animation = PICTURE_RECORD(piece)->animation;
	piece += 2u;
	animation_start(animation);
	OBJECT(current_object)->palette = PICTURE_RECORD(piece - 0x2u)->palette;
	piece += 2u;
	OBJECT(current_object)->width_scale = PICTURE_RECORD(piece - 0x4u)->width_scale;
	piece += 2u;
	OBJECT(current_object)->height_scale = PICTURE_RECORD(piece - 0x6u)->height_scale;
	piece += 2u;
	OBJECT(current_object)->x = PICTURE_RECORD(piece - 0x8u)->x;
	piece += 2u;
	OBJECT(current_object)->y = PICTURE_RECORD(piece - 0xAu)->y;
	piece += 2u;
	OBJECT(current_object)->depth_layer = PICTURE_RECORD(piece - 0xCu)->depth_layer;
	OBJECT(current_object)->ignition_key_prompt.frames_left = PICTURE_RECORD(piece - 0xCu)->frames_left;
	OBJECT(current_object)->state++;
	animation_advance(0u);
	draw_object(0u);
}

/* The credits line under the prompt (title, car presentations, demo races); ends in game mode 6. With
   free play (free_play) FREE PLAY; otherwise, for the screen's seat (seat1_credits and seat1_coin_units
   for screen 1, seat2_credits and seat2_coin_units for screen 2): Credit (0 or 1) or Credits and the
   number of credits in the font's digits (tens at x AC when there are, units at B4), and while coins are
   in toward the next credit the coins and the coins a credit costs (coins_per_credit) as a fraction (C0
   and C8), each drawn with credit_display_digit. */
void credit_display_update(void)
{
	uint8_t coin_units = 0, coin_units_shown = 0, credits = 0, units = 0;
	uint32_t text = 0;
	uint32_t credits_kept = 0, coin_units_kept = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		if (SCREEN(screen)->game_mode == 6u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
		}
		if (free_play != 0u)
		{
			OBJECT(current_object)->state++;
			text = credit_free_play_text_ADDRESS;
			draw_text(text);
			return;
		}
		OBJECT(current_object)->state += 2u;
		break;
	case 1:
		text = credit_free_play_text_ADDRESS;
		draw_text(text);
		return;
	case 2:
		break;
	}
	switch (SCREEN(screen)->index)
	{
	case 0:
		credits = seat1_credits;
		coin_units = seat1_coin_units;
		break;
	case 1:
		credits = seat2_credits;
		coin_units = seat2_coin_units;
		break;
	}
	if (credits < 2u)
		text = credit_singular_text_ADDRESS;
	else
		text = credit_plural_text_ADDRESS;
	credits_kept = credits;
	coin_units_kept = coin_units;
	draw_text(text);
	coin_units_shown = (uint8_t)coin_units_kept;
	units = (uint8_t)credits_kept;
	r12 &= ~0xFFu;
	OBJECT(current_object)->y = 0xCFu;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	while ((int8_t)units >= 0xA)
	{
		units -= 0xAu;
		r12 = (r12 & ~0xFFu) | (uint8_t)(r12 + 1u);
	}
	if ((uint8_t)r12 != 0u)
	{
		OBJECT(current_object)->credit_display.digit_kind = 0u;
		OBJECT(current_object)->credit_display.digit = credit_digit_characters[(uint8_t)r12];
		OBJECT(current_object)->x = 0xACu;
		credit_display_digit();
	}
	OBJECT(current_object)->credit_display.digit_kind = 0u;
	OBJECT(current_object)->credit_display.digit = credit_digit_characters[(uint8_t)units];
	OBJECT(current_object)->x = 0xB4u;
	credit_display_digit();
	if ((uint8_t)coin_units_shown != 0u)
	{
		OBJECT(current_object)->credit_display.digit_kind = 1u;
		OBJECT(current_object)->credit_display.digit = coin_units_shown;
		OBJECT(current_object)->x = 0xC0u;
		credit_display_digit();
		OBJECT(current_object)->credit_display.digit_kind = 2u;
		OBJECT(current_object)->credit_display.digit = coins_per_credit;
		OBJECT(current_object)->x = 0xC8u;
		credit_display_digit();
	}
}

/* Draws one digit of the credits line at x and y: digit_kind picks the animation and palette
   (credit_display_animations, credit_display_palettes: 0 the font, whose frames are its characters, 1
   the coins, 2 the coins a credit costs) and digit the frame. */
void credit_display_digit(void)
{
	uint32_t animation = 0, digit_kind = 0;

	OBJECT(current_object)->draw_mode = 0u;
	digit_kind = OBJECT(current_object)->credit_display.digit_kind;
	animation = credit_display_animations[digit_kind];
	animation_start(animation);
	OBJECT(current_object)->palette = credit_display_palettes[digit_kind];
	OBJECT(current_object)->frame = OBJECT(current_object)->credit_display.digit;
	draw_object(digit_kind);
}
