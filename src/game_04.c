/**
 *    Name:        game_04.c
 *    Description: the game's functions, part 04 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Waits for the background's palette fades (fades, count of them) to end, forgetting each as it ends
   (its flags bit 0 clear); then clears screen background_fading (the background's palettes are changing)
   and ends. */
void race_background_fade_watch_update(void)
{
	uint32_t all_flags = 0, fade = 0, fade_flags = 0, fade_slot = 0, fades_left = 0;

	all_flags = 0u;
	fade_slot = current_object + 0x80u;
	fades_left = OBJECT(current_object)->race_background_fade_watch.count;
	do
	{
		fade = mem_read32(fade_slot);
		fade_slot += 4u;
		if (fade != 0u)
		{
			fade_flags = mem_read32(fade);
			all_flags |= fade_flags;
			if ((fade_flags & 1u) == 0u)
				mem_write32(fade_slot - 4u, 0u);
		}
		fades_left--;
	} while (fades_left != 0u);
	if ((all_flags & 1u) == 0u)
	{
		SCREEN(screen)->background_fading = 0u;
		OBJECT(current_object)->flags = 0u;
	}
}

/* Starts a palette_fade of 16 colours from the palette at R10 to the one at R11, for palette offset R12
   (R0 blocks of 16 in its upload record), a step every R1 frames, the first after (screen
   palette_fades_started mod R1) + 1 (unk_F0E counts the fades started); returns the object. */
uint32_t start_palette_fade(uint8_t blocks, uint8_t period, uint32_t from, uint32_t to)
{
	uint8_t phase = 0;
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->handler = palette_fade_prepare_ADDRESS;
	OBJECT(object)->palette_fade.upload_length = 8u;
	OBJECT(object)->palette_fade.upload_colours = object + 0x34u;
	OBJECT(object)->palette_fade.palette_offset = r12;
	OBJECT(object)->palette_fade.blocks = (uint8_t)blocks;
	phase = remainder_u8(SCREEN(screen)->palette_fades_started, (uint8_t)period);
	SCREEN(screen)->palette_fades_started++;
	OBJECT(object)->palette_fade.delay = phase + 1u;
	OBJECT(object)->palette_fade.period = period;
	OBJECT(object)->palette_fade.from = from;
	OBJECT(object)->palette_fade.to = to;
	OBJECT(object)->palette_fade.colour_place = object + 0x34u;
	OBJECT(object)->palette_fade.difference_place = object + 0x54u;
	OBJECT(object)->palette_fade.colours_left = 0x10u;
	return object;
}

/* Prepares a palette fade, one colour a frame for its 16 (colours_left): the colour of the first palette
   (from) into its colours (34 on) and its red, green and blue (five bits each) less those of the second
   (to) into its differences (54 on); then palette_fade_step. */
void palette_fade_prepare(void)
{
	uint16_t to_colour = 0;
	uint32_t difference_place = 0, from = 0, from_colour = 0, to = 0;
	uint8_t colours_left = 0;

	from = OBJECT(current_object)->palette_fade.from;
	to = OBJECT(current_object)->palette_fade.to;
	r12 = OBJECT(current_object)->palette_fade.colour_place;
	difference_place = OBJECT(current_object)->palette_fade.difference_place;
	from_colour = PALETTE_COLOURS(from)[0];
	to_colour = PALETTE_COLOURS(to)[0];
	mem_write16(r12, from_colour);
	r12 = r12 + 2u;
	mem_write8(difference_place, (from_colour & 0x1Fu) - (to_colour & 0x1Fu));
	difference_place++;
	from_colour = from_colour >> 5;
	to_colour = to_colour >> 5;
	mem_write8(difference_place, (from_colour & 0x1Fu) - (to_colour & 0x1Fu));
	difference_place++;
	from_colour = from_colour >> 5;
	from_colour = from_colour & 0x1Fu;
	from_colour = (uint8_t)(from_colour - ((to_colour >> 5) & 0x1Fu));
	mem_write8(difference_place, from_colour);
	difference_place++;
	colours_left = OBJECT(current_object)->palette_fade.colours_left;
	OBJECT(current_object)->palette_fade.colours_left = colours_left - 1u;
	from += 2u;
	to += 2u;
	if (colours_left == 1u)
	{
		OBJECT(current_object)->handler = palette_fade_step_ADDRESS;
		palette_fade_step(from_colour);
		return;
	}
	OBJECT(current_object)->palette_fade.from = from;
	OBJECT(current_object)->palette_fade.to = to;
	OBJECT(current_object)->palette_fade.colour_place = r12;
	OBJECT(current_object)->palette_fade.difference_place = difference_place;
}

/* Every period frames (delay; start_palette_fade staggers the first) moves each of the 16 colours one
   step of each of red, green and blue toward the second palette (by its differences, counted down to 0)
   and queues them (queue_palette_list with its record at 28: 8 bytes, the colours, palette_offset,
   blocks); ends when no colour moved, or when the screen fades out (screen flags bit 0, with unk_153 at
   most 5 and even). */
void palette_fade_step(uint32_t from_colour)
{
	bool L_0186AA = false, L_0186B2 = false, L_0186BA = false, L_0186CB = false, L_0186D1 = false, L_0186D7 = false, L_0186E0 = false, L_0186E9 = false, L_0186F2 = false;
	uint16_t colour_step = 0;
	uint32_t colour_place = 0, colours_left = 0, difference_place = 0, fade_kind = 0, upload = 0;
	uint8_t delay = 0;

	if ((SCREEN(screen)->flags & 1u) != 0u)
	{
		fade_kind = (from_colour & ~0xFFu) | SCREEN(screen)->fade_kind;
		if ((int8_t)fade_kind <= 5 && (fade_kind & 1u) == 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
	}
	delay = OBJECT(current_object)->palette_fade.delay;
	OBJECT(current_object)->palette_fade.delay = delay - 1u;
	if (delay != 1u)
		return;
	OBJECT(current_object)->palette_fade.delay = OBJECT(current_object)->palette_fade.period;
	difference_place = current_object + 0x54u;
	colour_place = current_object + 0x34u;
	colour_step = 0u;
	colours_left = 0x10u;
	do
	{
		do
		{
			do
			{
				do
				{
					do
					{
						do
						{
							if (L_0186D1 || L_0186D7 || L_0186E0 || L_0186E9 || L_0186F2 || (int8_t)mem_read8(difference_place) < 0)
							{
								for (;;)
								{
									do
									{
										if (L_0186B2 || L_0186B2 || L_0186D1 || L_0186D7 || !L_0186E0)
										{
											do
											{
												do
												{
													do
													{
														if (L_0186CB || L_0186AA || L_0186AA || !L_0186B2)
														{
															if (L_0186CB || L_0186AA || L_0186AA || !L_0186B2)
															{
																if (L_0186CB || L_0186AA || L_0186AA || !L_0186D1)
																{
																	if (L_0186CB || L_0186AA || L_0186AA || !L_0186D7)
																	{
																		if (L_0186CB || L_0186AA || L_0186AA || !L_0186E9)
																		{
																			if (L_0186CB || L_0186AA || L_0186AA || !L_0186F2)
																			{
																				if (L_0186CB || !L_0186AA)
																				{
																					if (L_0186CB || !L_0186AA)
																					{
																						L_0186CB = false;
																						mem_write8(difference_place, mem_read8(difference_place) + 1u);
																						colour_step++;
																						difference_place++;
																					}
																				}
																				L_0186AA = false;
																			}
																		}
																	}
																}
															}
														}
														if (L_0186B2 || L_0186B2 || L_0186D1 || L_0186D7 || L_0186E9 || L_0186F2 || (int8_t)mem_read8(difference_place) < 0)
														{
															for (;;)
															{
																do
																{
																	if (L_0186BA || L_0186BA || L_0186B2 || L_0186B2 || L_0186D1 || L_0186D7 || L_0186E9 || !L_0186F2)
																	{
																		if (!L_0186BA)
																		{
																			if (!L_0186BA)
																			{
																				if (L_0186B2 || L_0186B2 || !L_0186D1)
																				{
																					if (L_0186B2 || L_0186B2 || L_0186D7 || !L_0186E9)
																					{
																						if (!L_0186B2)
																						{
																							if (!L_0186B2)
																							{
																								L_0186D7 = false;
																								mem_write8(difference_place, mem_read8(difference_place) + 1u);
																								colour_step += 0x20u;
																								difference_place++;
																							}
																						}
																						L_0186B2 = false;
																					}
																				}
																			}
																		}
																		if (L_0186BA || L_0186BA || L_0186D1 || L_0186E9 || (int8_t)mem_read8(difference_place) < 0)
																		{
																			if (L_0186BA || L_0186BA || !L_0186D1)
																			{
																				if (!L_0186BA)
																				{
																					if (!L_0186BA)
																					{
																						L_0186E9 = false;
																						mem_write8(difference_place, mem_read8(difference_place) + 1u);
																						colour_step += 0x400u;
																						difference_place++;
																					}
																				}
																				L_0186BA = false;
																				mem_write16(colour_place, mem_read16(colour_place) + colour_step);
																				colours_left--;
																				colour_place += 2u;
																				if (colours_left == 0u)
																				{
																					upload = current_object + 0x28u;
																					queue_palette_list(upload);
																					return;
																				}
																				colour_step = 0u;
																				L_0186CB = (int8_t)mem_read8(difference_place) < 0;
																			}
																			if (!L_0186CB)
																			{
																				if (L_0186D1 || (int8_t)mem_read8(difference_place) > 0)
																				{
																					L_0186D1 = false;
																					mem_write8(difference_place, mem_read8(difference_place) - 1u);
																					colour_step--;
																					difference_place++;
																					L_0186AA = true;
																				}
																				if (!L_0186AA)
																				{
																					difference_place++;
																					L_0186AA = true;
																				}
																			}
																		}
																	}
																	if (L_0186CB)
																		break;
																	if (L_0186AA)
																		break;
																	if (L_0186F2 || (int8_t)mem_read8(difference_place) > 0)
																	{
																		L_0186F2 = false;
																		mem_write8(difference_place, mem_read8(difference_place) - 1u);
																		colour_step -= 0x400u;
																		difference_place++;
																		L_0186BA = true;
																	}
																} while (L_0186BA);
																if (L_0186CB)
																	break;
																if (L_0186AA)
																	break;
																difference_place++;
																L_0186BA = true;
															}
														}
													} while (L_0186CB);
												} while (L_0186AA);
											} while (L_0186AA);
										}
										if (L_0186E0 || (int8_t)mem_read8(difference_place) > 0)
										{
											L_0186E0 = false;
											mem_write8(difference_place, mem_read8(difference_place) - 1u);
											colour_step -= 0x20u;
											difference_place++;
											L_0186B2 = true;
										}
									} while (L_0186B2);
									difference_place++;
									L_0186B2 = true;
								}
							}
							L_0186D1 = (int8_t)mem_read8(difference_place) > 0;
						} while (L_0186D1);
						difference_place++;
						L_0186D7 = (int8_t)mem_read8(difference_place) < 0;
					} while (L_0186D7);
					L_0186E0 = (int8_t)mem_read8(difference_place) > 0;
				} while (L_0186E0);
				difference_place++;
				L_0186E9 = (int8_t)mem_read8(difference_place) < 0;
			} while (L_0186E9);
			L_0186F2 = (int8_t)mem_read8(difference_place) > 0;
		} while (L_0186F2);
		difference_place++;
		colour_place += 2u;
		colours_left--;
	} while (colours_left != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* Starts the cabinets' LCDs at boot (both, through port G of the two I/O chips): the 4-bit start
   (command nibbles 3, 3, 3, 2), display on (0C), SEGA Enterprises as both screens' text (lcd_text),
   command 40 then the eight custom characters (lcd_custom_characters: the DJ's face among them); the
   screens' lamps off (screen button_lamps), lcd_restart_frames E10. */
void lcd_start(void)
{
	uint8_t nibble = 0;
	uint32_t character_row = 0, text_0_to_3 = 0, text_12_to_15 = 0, text_4_to_7 = 0, text_8_to_11 = 0, to_send = 0, wait_count = 0;

	lcd_send_step = 0u;
	lcd_busy = 0u;
	lcd_restart_frames = 0xE10u;
	SCREEN(SCREEN_1)->button_lamps = 0u;
	SCREEN(SCREEN_2)->button_lamps = 0u;
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
	text_0_to_3 = lcd_boot_text[0].text_0_to_3;
	text_4_to_7 = lcd_boot_text[0].text_4_to_7;
	AS32(SCREEN(SCREEN_1)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = text_4_to_7;
	AS32(SCREEN(SCREEN_2)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = text_4_to_7;
	text_8_to_11 = lcd_boot_text[0].text_8_to_11;
	text_12_to_15 = lcd_boot_text[0].text_12_to_15;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = text_12_to_15;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = text_12_to_15;
	to_send = (text_8_to_11 & ~0xFFu) | 0x40u;
	r12 = unk_218F20_ADDRESS;
	lcd_command_nibble_after_pause((uint8_t)to_send);
	to_send &= ~0xFFu;
	lcd_command_nibble_after_pause((uint8_t)to_send);
	character_row = lcd_custom_characters_ADDRESS;
	for (;;)
	{
		to_send = (to_send & ~0xFFu) | LCD_CHARACTER_ROWS(character_row)[0];
		character_row++;
		if ((int8_t)to_send < 0)
			break;
		to_send = lcd_write_character(to_send);
	}
}

/* Writes the data byte R0 to both LCDs, its high nibble then its low one (lcd_write_data_nibble), with
   pauses. */
uint32_t lcd_write_character(uint32_t character)
{
	uint32_t low_nibble = 0, wait_count = 0;
	uint32_t character_kept = 0;

	wait_count = 0x50u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	character_kept = character;
	character = character & 0xF0u;
	lcd_write_data_nibble((uint8_t)character);
	wait_count = 0x50u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	low_nibble = (character_kept & ~0xFFu) | (uint8_t)(character_kept << 4);
	lcd_write_data_nibble((uint8_t)low_nibble);
	return low_nibble;
}

/* Writes the high nibble of R0 to both LCDs as data (port G of the two I/O chips, inverted: the data
   lines, register select set, then the enable strobe bit 3 dropped). */
void lcd_write_data_nibble(uint8_t nibble)
{
	uint32_t wait_count = 0;

	nibble |= 0xFu;
	mem_write8(IO_C0000C, ~nibble);
	mem_write8(0xC8000Cu, ~nibble);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble &= ~8u;
	mem_write8(IO_C0000C, ~nibble);
	mem_write8(0xC8000Cu, ~nibble);
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
	mem_write8(IO_C0000C, ~nibble);
	mem_write8(0xC8000Cu, ~nibble);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	nibble &= ~8u;
	mem_write8(IO_C0000C, ~nibble);
	mem_write8(0xC8000Cu, ~nibble);
}

/* After the test mode: both screens' LCD text back to SEGA Enterprises (lcd_text), their lamps off
   (screen button_lamps), and lcd_restart_frames 0 so that the next VBLANK starts the LCDs again. */
void lcd_reset(void)
{
	uint32_t text_0_to_3 = 0, text_12_to_15 = 0, text_4_to_7 = 0, text_8_to_11 = 0;

	lcd_restart_frames = 0u;
	SCREEN(SCREEN_1)->button_lamps = 0u;
	SCREEN(SCREEN_2)->button_lamps = 0u;
	text_0_to_3 = lcd_boot_text[0].text_0_to_3;
	text_4_to_7 = lcd_boot_text[0].text_4_to_7;
	AS32(SCREEN(SCREEN_1)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = text_4_to_7;
	AS32(SCREEN(SCREEN_2)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = text_4_to_7;
	text_8_to_11 = lcd_boot_text[0].text_8_to_11;
	text_12_to_15 = lcd_boot_text[0].text_12_to_15;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = text_12_to_15;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = text_12_to_15;
	r12 = unk_218F20_ADDRESS;
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
	uint32_t lcd_text_4_to_7 = 0, lcd_send_buffer_1_0_long = 0, lcd_text_12_to_15 = 0, lcd_send_buffer_1_8_long = 0, lcd_text_4_to_7_2 = 0, lcd_send_buffer_2_0_long = 0, lcd_text_12_to_15_2 = 0, lcd_send_buffer_2_8_long = 0, lcd_send_buffer_1_0_long_2 = 0, lcd_send_buffer_1_8_long_2 = 0, lcd_send_buffer_2_0_long_2 = 0, lcd_send_buffer_2_8_long_2 = 0;

	restart_frames = lcd_restart_frames;
	lcd_restart_frames = restart_frames - 1u;
	if ((int16_t)restart_frames <= 1)
	{
		lcd_init_step = 0x2Cu;
		lcd_restart_frames = 0xE10u;
		return;
	}
	if (lcd_busy != 0u)
		return;
	send_step = lcd_send_step;
	carry = send_step & 1u;
	send_step = send_step >> 1;
	if (carry == 0u)
	{
		lcd_send_step = 0u;
		lcd_text_4_to_7 = SCREEN(SCREEN_1)->lcd_text_4_to_7;
		lcd_send_buffer_1_0_long = AS32(SCREEN(SCREEN_1)->lcd_text);
		lcd_send_buffer_1[0] = (uint8_t)lcd_send_buffer_1_0_long;
		lcd_send_buffer_1[1] = (uint8_t)(lcd_send_buffer_1_0_long >> 8u);
		lcd_send_buffer_1[2] = (uint8_t)(lcd_send_buffer_1_0_long >> 16u);
		lcd_send_buffer_1[3] = (uint8_t)(lcd_send_buffer_1_0_long >> 24u);
		lcd_send_buffer_1[4] = (uint8_t)lcd_text_4_to_7;
		lcd_send_buffer_1[5] = (uint8_t)(lcd_text_4_to_7 >> 8u);
		lcd_send_buffer_1[6] = (uint8_t)(lcd_text_4_to_7 >> 16u);
		lcd_send_buffer_1[7] = (uint8_t)(lcd_text_4_to_7 >> 24u);
		lcd_text_12_to_15 = SCREEN(SCREEN_1)->lcd_text_12_to_15;
		lcd_send_buffer_1_8_long = SCREEN(SCREEN_1)->lcd_text_8_to_11;
		lcd_send_buffer_1[8] = (uint8_t)lcd_send_buffer_1_8_long;
		lcd_send_buffer_1[9] = (uint8_t)(lcd_send_buffer_1_8_long >> 8u);
		lcd_send_buffer_1[10] = (uint8_t)(lcd_send_buffer_1_8_long >> 16u);
		lcd_send_buffer_1[11] = (uint8_t)(lcd_send_buffer_1_8_long >> 24u);
		lcd_send_buffer_1[12] = (uint8_t)lcd_text_12_to_15;
		lcd_send_buffer_1[13] = (uint8_t)(lcd_text_12_to_15 >> 8u);
		lcd_send_buffer_1[14] = (uint8_t)(lcd_text_12_to_15 >> 16u);
		lcd_send_buffer_1[15] = (uint8_t)(lcd_text_12_to_15 >> 24u);
		lcd_text_4_to_7_2 = SCREEN(SCREEN_2)->lcd_text_4_to_7;
		lcd_send_buffer_2_0_long = AS32(SCREEN(SCREEN_2)->lcd_text);
		lcd_send_buffer_2[0] = (uint8_t)lcd_send_buffer_2_0_long;
		lcd_send_buffer_2[1] = (uint8_t)(lcd_send_buffer_2_0_long >> 8u);
		lcd_send_buffer_2[2] = (uint8_t)(lcd_send_buffer_2_0_long >> 16u);
		lcd_send_buffer_2[3] = (uint8_t)(lcd_send_buffer_2_0_long >> 24u);
		lcd_send_buffer_2[4] = (uint8_t)lcd_text_4_to_7_2;
		lcd_send_buffer_2[5] = (uint8_t)(lcd_text_4_to_7_2 >> 8u);
		lcd_send_buffer_2[6] = (uint8_t)(lcd_text_4_to_7_2 >> 16u);
		lcd_send_buffer_2[7] = (uint8_t)(lcd_text_4_to_7_2 >> 24u);
		lcd_text_12_to_15_2 = SCREEN(SCREEN_2)->lcd_text_12_to_15;
		lcd_send_buffer_2_8_long = SCREEN(SCREEN_2)->lcd_text_8_to_11;
		lcd_send_buffer_2[8] = (uint8_t)lcd_send_buffer_2_8_long;
		lcd_send_buffer_2[9] = (uint8_t)(lcd_send_buffer_2_8_long >> 8u);
		lcd_send_buffer_2[10] = (uint8_t)(lcd_send_buffer_2_8_long >> 16u);
		lcd_send_buffer_2[11] = (uint8_t)(lcd_send_buffer_2_8_long >> 24u);
		lcd_send_buffer_2[12] = (uint8_t)lcd_text_12_to_15_2;
		lcd_send_buffer_2[13] = (uint8_t)(lcd_text_12_to_15_2 >> 8u);
		lcd_send_buffer_2[14] = (uint8_t)(lcd_text_12_to_15_2 >> 16u);
		lcd_send_buffer_2[15] = (uint8_t)(lcd_text_12_to_15_2 >> 24u);
		return;
	}
	half_step = send_step;
	nibble = (uint8_t)(half_step - 1u);
	if (half_step != 1u)
		nibble = 4u;
	lcd_write_command_nibble(nibble);
	lcd_send_step = 0u;
	lcd_text_4_to_7 = SCREEN(SCREEN_1)->lcd_text_4_to_7;
	lcd_send_buffer_1_0_long_2 = AS32(SCREEN(SCREEN_1)->lcd_text);
	lcd_send_buffer_1[0] = (uint8_t)lcd_send_buffer_1_0_long_2;
	lcd_send_buffer_1[1] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 8u);
	lcd_send_buffer_1[2] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 16u);
	lcd_send_buffer_1[3] = (uint8_t)(lcd_send_buffer_1_0_long_2 >> 24u);
	lcd_send_buffer_1[4] = (uint8_t)lcd_text_4_to_7;
	lcd_send_buffer_1[5] = (uint8_t)(lcd_text_4_to_7 >> 8u);
	lcd_send_buffer_1[6] = (uint8_t)(lcd_text_4_to_7 >> 16u);
	lcd_send_buffer_1[7] = (uint8_t)(lcd_text_4_to_7 >> 24u);
	lcd_text_12_to_15 = SCREEN(SCREEN_1)->lcd_text_12_to_15;
	lcd_send_buffer_1_8_long_2 = SCREEN(SCREEN_1)->lcd_text_8_to_11;
	lcd_send_buffer_1[8] = (uint8_t)lcd_send_buffer_1_8_long_2;
	lcd_send_buffer_1[9] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 8u);
	lcd_send_buffer_1[10] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 16u);
	lcd_send_buffer_1[11] = (uint8_t)(lcd_send_buffer_1_8_long_2 >> 24u);
	lcd_send_buffer_1[12] = (uint8_t)lcd_text_12_to_15;
	lcd_send_buffer_1[13] = (uint8_t)(lcd_text_12_to_15 >> 8u);
	lcd_send_buffer_1[14] = (uint8_t)(lcd_text_12_to_15 >> 16u);
	lcd_send_buffer_1[15] = (uint8_t)(lcd_text_12_to_15 >> 24u);
	lcd_text_4_to_7_2 = SCREEN(SCREEN_2)->lcd_text_4_to_7;
	lcd_send_buffer_2_0_long_2 = AS32(SCREEN(SCREEN_2)->lcd_text);
	lcd_send_buffer_2[0] = (uint8_t)lcd_send_buffer_2_0_long_2;
	lcd_send_buffer_2[1] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 8u);
	lcd_send_buffer_2[2] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 16u);
	lcd_send_buffer_2[3] = (uint8_t)(lcd_send_buffer_2_0_long_2 >> 24u);
	lcd_send_buffer_2[4] = (uint8_t)lcd_text_4_to_7_2;
	lcd_send_buffer_2[5] = (uint8_t)(lcd_text_4_to_7_2 >> 8u);
	lcd_send_buffer_2[6] = (uint8_t)(lcd_text_4_to_7_2 >> 16u);
	lcd_send_buffer_2[7] = (uint8_t)(lcd_text_4_to_7_2 >> 24u);
	lcd_text_12_to_15_2 = SCREEN(SCREEN_2)->lcd_text_12_to_15;
	lcd_send_buffer_2_8_long_2 = SCREEN(SCREEN_2)->lcd_text_8_to_11;
	lcd_send_buffer_2[8] = (uint8_t)lcd_send_buffer_2_8_long_2;
	lcd_send_buffer_2[9] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 8u);
	lcd_send_buffer_2[10] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 16u);
	lcd_send_buffer_2[11] = (uint8_t)(lcd_send_buffer_2_8_long_2 >> 24u);
	lcd_send_buffer_2[12] = (uint8_t)lcd_text_12_to_15_2;
	lcd_send_buffer_2[13] = (uint8_t)(lcd_text_12_to_15_2 >> 8u);
	lcd_send_buffer_2[14] = (uint8_t)(lcd_text_12_to_15_2 >> 16u);
	lcd_send_buffer_2[15] = (uint8_t)(lcd_text_12_to_15_2 >> 24u);
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

	if (lcd_init_step != 0u)
	{
		lcd_busy = 0xFFu;
		screen_1_port = (uint8_t)~lcd_restart_nibbles[lcd_init_step];
		screen_2_port = screen_1_port | port_g_outputs[1];
		mem_write8(IO_C0000C, screen_1_port | port_g_outputs[0]);
		mem_write8(0xC8000Cu, screen_2_port);
		init_step = lcd_init_step;
		lcd_init_step = init_step - 1u;
		if (init_step == 1u)
		{
			lcd_send_step = 0x22u;
			lcd_busy = 0u;
		}
		return;
	}
	send_step = lcd_send_step;
	if (send_step >= 0x22u)
		return;
	lcd_busy = 0xFFu;
	lcd_send_step++;
	current_step = send_step;
	send_step = (uint8_t)(current_step - 2u);
	if ((int8_t)current_step < 2)
	{
		send_step = (uint8_t)~send_step;
		send_step = (uint8_t)~((send_step << 7) | (send_step >> 1) | 0xBu);
		screen_2_port = (uint8_t)send_step;
		send_step = send_step | port_g_outputs[0];
		screen_2_port |= port_g_outputs[1];
		mem_write8(IO_C0000C, send_step);
		mem_write8(0xC8000Cu, screen_2_port);
		wait_count = 0x14u;
		do
		{
			wait_count--;
		} while (wait_count != 0u);
		mem_write8(IO_C0000C, send_step | 8u);
		mem_write8(0xC8000Cu, screen_2_port | 8u);
		lcd_busy = 0u;
		return;
	}
	carry = send_step & 1u;
	send_step = send_step >> 1;
	screen_2_port = lcd_send_buffer_2[send_step];
	screen_1_port = lcd_send_buffer_1[send_step];
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
	screen_1_port = (uint8_t)(~(screen_1_port | 0xFu) | port_g_outputs[0]);
	screen_2_port = (uint8_t)(~(screen_2_port | 0xFu) | port_g_outputs[1]);
	mem_write8(IO_C0000C, screen_1_port);
	mem_write8(0xC8000Cu, screen_2_port);
	wait_count = 0x14u;
	do
	{
		wait_count--;
	} while (wait_count != 0u);
	mem_write8(IO_C0000C, screen_1_port | 8u);
	mem_write8(0xC8000Cu, screen_2_port | 8u);
	lcd_busy = 0u;
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
	uint32_t lcd_scroll = 0, lcd_scroll_request = 0, mouth_shape = 0, mouth_shapes = 0, scroll_width = 0, text_place = 0;
	uint8_t dj_mouth_frames = 0, lcd_scroll_frames = 0, lcd_scroll_frames_2 = 0;

	text_place = screen + 0xF10u;
	if (SCREEN(screen)->dj_face == 0u)
		scroll_width = 0xFu;
	else
	{
		mouth_shapes = SCREEN(screen)->dj_mouth_shapes;
		if (mouth_shapes == 0u)
		{
			AS16(SCREEN(text_place - 0xF10u)->lcd_text) = dj_face_characters[0];
			SCREEN(screen)->dj_mouth_frames = 3u;
			scroll_width = 0xDu;
			text_place += 2u;
		}
		else
		{
			dj_mouth_frames = SCREEN(screen)->dj_mouth_frames;
			SCREEN(screen)->dj_mouth_frames = dj_mouth_frames - 1u;
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
					SCREEN(screen)->dj_mouth_shapes = 0u;
					AS16(SCREEN(text_place - 0xF10u)->lcd_text) = dj_face_characters[0];
					SCREEN(screen)->dj_mouth_frames = 3u;
					scroll_width = 0xDu;
					text_place += 2u;
				}
				else
				{
					AS16(SCREEN(text_place - 0xF10u)->lcd_text) = dj_face_characters[mouth_shape];
					SCREEN(screen)->dj_mouth_shapes = mouth_shapes;
					SCREEN(screen)->dj_mouth_frames = 3u;
					scroll_width = 0xDu;
					text_place += 2u;
				}
			}
		}
	}
	lcd_scroll_request = SCREEN(screen)->lcd_scroll_request;
	if (lcd_scroll_request != 0u)
	{
		SCREEN(screen)->lcd_scroll = lcd_scroll_request;
		SCREEN(screen)->lcd_scroll_request = 0u;
		SCREEN(screen)->lcd_scroll_step = 0u;
		space_character = 0x20u;
		move_string(text_place, 0u, text_place, scroll_width + 1u, 1u, 1u, 0u, space_character, &r27, &r28);
	}
	lcd_scroll = SCREEN(screen)->lcd_scroll;
	if (lcd_scroll == 0u)
		return;
	switch (SCREEN(screen)->lcd_scroll_step)
	{
	case 0:
		character = mem_read8(lcd_scroll);
		SCREEN(screen)->lcd_scroll = lcd_scroll + 1u;
		if (character == 0x60u)
			character = 7u;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, character);
		SCREEN(screen)->lcd_scroll_frames = 5u;
		SCREEN(screen)->lcd_scroll_step++;
		lcd_scroll_frames = SCREEN(screen)->lcd_scroll_frames;
		SCREEN(screen)->lcd_scroll_frames = lcd_scroll_frames - 1u;
		if (lcd_scroll_frames != 1u)
			return;
		if (mem_read8(SCREEN(screen)->lcd_scroll) != 0u)
		{
			SCREEN(screen)->lcd_scroll_step--;
			return;
		}
		SCREEN(screen)->lcd_scroll = scroll_width + 1u;
		SCREEN(screen)->lcd_scroll_step++;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->lcd_scroll_frames = 5u;
		SCREEN(screen)->lcd_scroll_step++;
		break;
	case 1:
		lcd_scroll_frames = SCREEN(screen)->lcd_scroll_frames;
		SCREEN(screen)->lcd_scroll_frames = lcd_scroll_frames - 1u;
		if (lcd_scroll_frames != 1u)
			return;
		if (mem_read8(SCREEN(screen)->lcd_scroll) != 0u)
		{
			SCREEN(screen)->lcd_scroll_step--;
			return;
		}
		SCREEN(screen)->lcd_scroll = scroll_width + 1u;
		SCREEN(screen)->lcd_scroll_step++;
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->lcd_scroll_frames = 5u;
		SCREEN(screen)->lcd_scroll_step++;
		break;
	case 2:
		move_string(text_place + 1u, scroll_width, text_place, scroll_width, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->lcd_scroll_frames = 5u;
		SCREEN(screen)->lcd_scroll_step++;
		lcd_scroll_frames_2 = SCREEN(screen)->lcd_scroll_frames;
		SCREEN(screen)->lcd_scroll_frames = lcd_scroll_frames_2 - 1u;
		if (lcd_scroll_frames_2 == 1u)
		{
			SCREEN(screen)->lcd_scroll--;
			SCREEN(screen)->lcd_scroll_step--;
		}
		return;
	case 3:
		break;
	default:
		invalid_jump(lcd_scroll_step_jumps[SCREEN(screen)->lcd_scroll_step]);
	}
	lcd_scroll_frames_2 = SCREEN(screen)->lcd_scroll_frames;
	SCREEN(screen)->lcd_scroll_frames = lcd_scroll_frames_2 - 1u;
	if (lcd_scroll_frames_2 != 1u)
		return;
	SCREEN(screen)->lcd_scroll--;
	SCREEN(screen)->lcd_scroll_step--;
}

/* Clears the cabinet's LCD: sixteen spaces, no DJ message (screen dj_mouth_shapes), no scrolling text
   (unk_F40, unk_F44), the DJ's face off (unk_F35). */
void clear_lcd(void)
{
	uint32_t text_place = 0;

	text_place = screen + 0xF10u;
	AS32(SCREEN(text_place - 0xF10u)->lcd_text) = 0x20202020u;
	text_place += 4u;
	SCREEN(text_place - 0xF14u)->lcd_text_4_to_7 = 0x20202020u;
	text_place += 4u;
	SCREEN(text_place - 0xF18u)->lcd_text_8_to_11 = 0x20202020u;
	SCREEN(text_place - 0xF18u)->lcd_text_12_to_15 = 0x20202020u;
	SCREEN(screen)->dj_mouth_shapes = 0u;
	SCREEN(screen)->dj_mouth_frames = 3u;
	SCREEN(screen)->dj_face = 0u;
	SCREEN(screen)->lcd_scroll_request = 0u;
	SCREEN(screen)->lcd_scroll = 0u;
}

/* Starts the race information on the cabinet LCD: clears it (clear_lcd), shows the DJ's face in its
   first two characters (screen dj_face) and goes on to lcd_race_info_update. */
void lcd_race_info_start(void)
{
	clear_lcd();
	SCREEN(screen)->dj_face = 0xFFu;
	OBJECT(current_object)->handler = lcd_race_info_update_ADDRESS;
	lcd_race_info_update();
}

/* Shows the race on the cabinet LCD. Before the start, while the car object's unk_10 counts the frames
   to it: a bar shrinking through each second (lcd_countdown_bars) with the seconds left. Then a page
   every 150 frames (page_frames), by page: 0 Time (lcd_race_info_time), 1 Order (the car numbers,
   car_numbers + 1, in race order from order_list, this screen's own blinking; skipped with one player),
   2 Speed (screen shown_speed), 3 Max. (unk_F52), 4 Ave. (lcd_race_info_average); page_step 0 writes the
   label, 1 updates the number. When the car reaches the goal (car at_goal) goes to lcd_race_info_finish;
   when the race is over (car race_over) shows the average and ends; while the LCD scrolls a text (screen
   lcd_scroll) waits in lcd_race_info_paused. */
void lcd_race_info_update(void)
{
	uint8_t character = 0, page = 0;
	uint32_t countdown = 0, countdown_frames = 0, maximum_speed = 0, order_list = 0, racer = 0, racers_left = 0, shown_speed = 0, text_place = 0;
	uint16_t page_frames = 0;

	if (CAR(player_car)->at_goal != 0u)
	{
		OBJECT(current_object)->state = 0u;
		OBJECT(current_object)->handler = lcd_race_info_finish_ADDRESS;
		lcd_race_info_finish();
		return;
	}
	if (CAR(player_car)->race_over != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		lcd_race_info_average_page();
		return;
	}
	if (SCREEN(screen)->lcd_scroll != 0u)
	{
		OBJECT(current_object)->state = 1u;
		OBJECT(current_object)->handler = lcd_race_info_paused_ADDRESS;
		lcd_race_info_paused();
		return;
	}
	switch (OBJECT(current_object)->state)
	{
	case 0:
		countdown = OBJECT(CAR(player_car)->object)->player_car.timer;
		if (countdown != 0u)
		{
			countdown_frames = 0u;
			divux_pair(0x1Eu, &countdown, &countdown_frames);
			move_string(lcd_countdown_bars[(countdown_frames + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			SCREEN(screen)->lcd_text_8_to_11 = (SCREEN(screen)->lcd_text_8_to_11 & 0xFFFF00FFu) | ((countdown + 0x31u & 0xFFu) << 8u);
			return;
		}
		OBJECT(current_object)->lcd_race_info.racer_count = RACE(SCREEN(screen)->host_record)->racer_count;
		OBJECT(current_object)->lcd_race_info.order_list = SCREEN(screen)->host_record + 0x20u;
		OBJECT(current_object)->lcd_race_info.page_frames = 0x96u;
		OBJECT(current_object)->state++;
		OBJECT(current_object)->lcd_race_info.page_step = 0u;
		OBJECT(current_object)->state++;
		break;
	case 1:
		OBJECT(current_object)->lcd_race_info.page_step = 0u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	default:
		invalid_jump(lcd_race_info_state_jumps[OBJECT(current_object)->state]);
	}
	page_frames = OBJECT(current_object)->lcd_race_info.page_frames;
	OBJECT(current_object)->lcd_race_info.page_frames = page_frames - 1u;
	if (page_frames == 1u)
	{
		page = (uint8_t)(OBJECT(current_object)->lcd_race_info.page + 1u);
		if ((int8_t)page >= 5)
			page = 0u;
		OBJECT(current_object)->lcd_race_info.page = page;
		OBJECT(current_object)->lcd_race_info.page_step = 0u;
		OBJECT(current_object)->lcd_race_info.page_frames = 0x96u;
	}
	switch (OBJECT(current_object)->lcd_race_info.page)
	{
	case 0:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			lcd_race_info_time_page();
			return;
		case 1:
			lcd_race_info_time();
			return;
		default:
			invalid_jump(lcd_race_info_time_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		break;
	case 1:
		break;
	case 2:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			move_string(lcd_speed_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		default:
			invalid_jump(lcd_race_info_speed_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		text_place = screen + 0xF1Cu;
		shown_speed = SCREEN(screen)->shown_speed;
		lcd_race_info_write_number(shown_speed, text_place);
		return;
	case 3:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			move_string(lcd_maximum_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		default:
			invalid_jump(lcd_race_info_maximum_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		text_place = screen + 0xF1Cu;
		maximum_speed = (uint16_t)SCREEN(screen)->maximum_speed;
		lcd_race_info_write_number(maximum_speed, text_place);
		return;
	case 4:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			lcd_race_info_average_page();
			return;
		case 1:
			lcd_race_info_average();
			return;
		default:
			invalid_jump(lcd_race_info_average_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		break;
	default:
		invalid_jump(lcd_race_info_page_jumps[OBJECT(current_object)->lcd_race_info.page]);
	}
	switch (OBJECT(current_object)->lcd_race_info.page_step)
	{
	case 0:
		if (RACE(SCREEN(screen)->host_record)->racer_count == 1u)
		{
			OBJECT(current_object)->lcd_race_info.page++;
			switch (OBJECT(current_object)->lcd_race_info.page_step)
			{
			case 0:
				move_string(lcd_speed_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
				OBJECT(current_object)->lcd_race_info.page_step++;
				break;
			case 1:
				break;
			default:
				invalid_jump(lcd_race_info_speed_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
			}
			text_place = screen + 0xF1Cu;
			shown_speed = SCREEN(screen)->shown_speed;
			lcd_race_info_write_number(shown_speed, text_place);
			return;
		}
		move_string(lcd_order_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->lcd_race_info.frames = 0u;
		OBJECT(current_object)->lcd_race_info.page_step++;
		break;
	case 1:
		break;
	default:
		invalid_jump(lcd_race_info_order_step_jumps[OBJECT(current_object)->lcd_race_info.page_step]);
	}
	text_place = screen + 0xF18u;
	order_list = OBJECT(current_object)->lcd_race_info.order_list;
	racers_left = OBJECT(current_object)->lcd_race_info.racer_count;
	do
	{
		racer = mem_read8(order_list);
		order_list++;
		if (SCREEN(screen)->player != racer || (OBJECT(current_object)->lcd_race_info.frames & 8u) == 0u)
			character = (uint8_t)(car_numbers[racer] + 0x31u);
		else
			character = 0x20u;
		mem_write8(text_place, character);
		racers_left--;
		text_place++;
	} while (racers_left != 0u);
	OBJECT(current_object)->lcd_race_info.frames++;
}

/* Writes the Time page's label on the LCD, then the time (lcd_race_info_time). */
void lcd_race_info_time_page(void)
{
	move_string(lcd_time_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->lcd_race_info.page_step++;
	lcd_race_info_time();
}

/* Writes the race time (the car's unk_04, in thousandths of a second) on the LCD as
   minutes'seconds"hundredths. */
void lcd_race_info_time(void)
{
	uint32_t digit = 0, race_time = 0, text_place = 0;

	race_time = CAR(player_car)->race_time / 0xAu;
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place = screen + 0xF20u - 1u;
	SCREEN(text_place - 0xF1Fu)->lcd_text_12_to_15 = (SCREEN(text_place - 0xF1Fu)->lcd_text_12_to_15 & 0xFFFFFFu) | ((digit + 0x30u & 0xFFu) << 24u);
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place--;
	SCREEN(text_place - 0xF1Eu)->lcd_text_12_to_15 = (SCREEN(text_place - 0xF1Eu)->lcd_text_12_to_15 & 0xFF00FFFFu) | ((digit + 0x30u & 0xFFu) << 16u);
	digit = 0u;
	divx_pair(0xAu, &race_time, &digit);
	text_place = text_place - 1u - 1u;
	SCREEN(text_place - 0xF1Cu)->lcd_text_12_to_15 = (SCREEN(text_place - 0xF1Cu)->lcd_text_12_to_15 & 0xFFFFFF00u) | (digit + 0x30u & 0xFFu);
	digit = 0u;
	divx_pair(6u, &race_time, &digit);
	text_place--;
	SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((digit + 0x30u & 0xFFu) << 24u);
	mem_write8(text_place - 1u - 1u, race_time + 0x30u);
}

/* Writes the Ave. page's label on the LCD, then the average speed (lcd_race_info_average). */
void lcd_race_info_average_page(void)
{
	move_string(lcd_average_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->lcd_race_info.page_step++;
	lcd_race_info_average();
}

/* Writes the average speed on the LCD: the sum of the speeds (screen speed_sum) over how many there are
   (unk_F58), with lcd_race_info_write_number. */
void lcd_race_info_average(void)
{
	uint32_t speed_samples = 0, speed_sum = 0, text_place = 0;

	text_place = screen + 0xF1Cu;
	speed_sum = SCREEN(screen)->speed_sum;
	speed_samples = SCREEN(screen)->speed_samples;
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
		SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((digit + digit_zero & 0xFFu) << 24u);
		text_place--;
		SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | 0x200000u;
		SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	text_place--;
	SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((digit + digit_zero & 0xFFu) << 24u);
	digit = 0u;
	divx_pair(0xAu, &speed, &digit);
	zero = (uint32_t)(speed == 0u);
	if (zero)
	{
		text_place--;
		SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | ((digit + digit_zero & 0xFFu) << 16u);
		SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	text_place--;
	SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | ((digit + digit_zero & 0xFFu) << 16u);
	if (speed == 0u)
	{
		SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(text_place - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | ((speed + digit_zero & 0xFFu) << 8u);
}

/* The car has reached the goal: shows */
void lcd_race_info_finish(void)
{
	uint16_t frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		move_string(lcd_finish_label_ADDRESS, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->state++;
		if (SCREEN(screen)->lcd_scroll == 0u)
			return;
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (SCREEN(screen)->lcd_scroll == 0u)
			return;
		OBJECT(current_object)->state++;
		if (SCREEN(screen)->lcd_scroll != 0u)
			return;
		lcd_race_info_average_page();
		OBJECT(current_object)->lcd_race_info.frames = 0x96u;
		OBJECT(current_object)->state++;
		frames = OBJECT(current_object)->lcd_race_info.frames;
		OBJECT(current_object)->lcd_race_info.frames = frames - 1u;
		if (frames != 1u)
			return;
		OBJECT(current_object)->flags = 0u;
		lcd_race_info_time_page();
		return;
	case 2:
		break;
	case 3:
		frames = OBJECT(current_object)->lcd_race_info.frames;
		OBJECT(current_object)->lcd_race_info.frames = frames - 1u;
		if (frames != 1u)
			return;
		OBJECT(current_object)->flags = 0u;
		lcd_race_info_time_page();
		return;
	default:
		invalid_jump(lcd_race_info_finish_state_jumps[OBJECT(current_object)->state]);
	}
	if (SCREEN(screen)->lcd_scroll != 0u)
		return;
	lcd_race_info_average_page();
	OBJECT(current_object)->lcd_race_info.frames = 0x96u;
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->lcd_race_info.frames;
	OBJECT(current_object)->lcd_race_info.frames = frames - 1u;
	if (frames != 1u)
		return;
	OBJECT(current_object)->flags = 0u;
	lcd_race_info_time_page();
}

/* Waits while the LCD scrolls a text (screen lcd_scroll), then goes back to lcd_race_info_update, which
   writes the page's label again. */
void lcd_race_info_paused(void)
{
	if (SCREEN(screen)->lcd_scroll == 0u)
	{
		OBJECT(current_object)->handler = lcd_race_info_update_ADDRESS;
		lcd_race_info_update();
		return;
	}
}

/* Sets up the radio as the race starts: the car each player chose (byte 49 of its race_copy record) by
   car number in car_models (FF where none) and how many in cars_to_introduce, this screen's car number,
   clears screen nearing_fork, gives the car the song of its model (car song = car_model_songs by car
   model), notes the stage (screen route_stage), then car_radio_wait_start. */
void car_radio_start(void)
{
	uint32_t object = 0, player = 0, race = 0, racers_left = 0;

	object = current_object + 0x80u;
	AS32(OBJECT(object - 0x80u)->car_radio.car_models) = 0xFFFFFFFFu;
	OBJECT(object - 0x80u)->unk_84 = 0xFFFFu;
	OBJECT(object - 0x80u)->unk_86 = 0xFFu;
	OBJECT(object - 0x80u)->unk_87 = 0xFFu;
	race = SCREEN(screen)->host_record + 0x1Fu;
	racers_left = RACE(race - 0x1Fu)->racer_count;
	OBJECT(current_object)->car_radio.cars_to_introduce = racers_left;
	race++;
	do
	{
		player = mem_read8(race);
		mem_write8(current_object + 0x80u + car_numbers[player], mem_read8(0x20FC49u + (player << 7)));
		racers_left--;
		race++;
	} while (racers_left != 0u);
	OBJECT(current_object)->car_radio.car_number = car_numbers[SCREEN(screen)->player];
	SCREEN(screen)->nearing_fork = 0u;
	CAR(player_car)->song = car_model_songs[CAR(player_car)->model];
	OBJECT(current_object)->car_radio.stage = SCREEN(screen)->route_stage;
	OBJECT(current_object)->handler = car_radio_wait_start_ADDRESS;
	car_radio_wait_start();
}

/* Waits for the start (the car object's unk_10, the frames to it, down to 0), then goes on to
   car_radio_read_buttons. */
void car_radio_wait_start(void)
{
	if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
		return;
	OBJECT(current_object)->handler = car_radio_read_buttons_ADDRESS;
	car_radio_read_buttons();
}

/* Each frame of the race: the buttons newly pressed (screen buttons_pressed) into buttons, then
   car_radio_update. Once the race is over (car at_goal at the goal, or unk_182) clears them, silences
   the DJ (talk_step 5), turns the screen's two lamps off (unk_F20) and leaves car_radio_update alone,
   without the buttons. */
void car_radio_read_buttons(void)
{
	OBJECT(current_object)->car_radio.buttons = (uint16_t)SCREEN(screen)->buttons_pressed;
	if ((CAR(player_car)->at_goal | CAR(player_car)->race_over) == 0u)
	{
		car_radio_update();
		return;
	}
	OBJECT(current_object)->car_radio.buttons = 0u;
	OBJECT(current_object)->car_radio.talk_step = 5u;
	SCREEN(screen)->button_lamps = 0u;
	OBJECT(current_object)->handler = car_radio_update_ADDRESS;
	car_radio_update();
}

/* The radio, by state: 0 plays the car's song (car song) with its title; 1 << and >> step through the
   ten songs (both together: song 10, Jingle Bell), START does >>, the DJ/music button goes to the
   station; 2 the station: title "Mega Radio Station", DJ message 29 queued, the stage's sound
   (route_stage_radio_sounds by screen route_stage); 3 the station plays; 4-8 while screen nearing_fork
   is set (fork_camera_update sets it, a new route_stage clears it): 3F to the screen's sound byte
   (fade_out_music_at), sound E0 after 60 frames, sound EE, 7F when it clears and after 120 frames the
   stage's message (stage_level_radio_messages by screen current_stage_level) and sound. On the station:
   any of the three buttons goes back to the car's song (state 0, talk_step 4); the DJ talks when no
   message plays (screen dj_mouth_shapes), by talk_step: 0 message 0, 1-2 each car number (message 9 +
   number) and its car (message 1 + model), 3 message 2A, 4 the queued_message or, at random, a list of
   remarks (routine_lists_1966C), 5 nothing. */
void car_radio_update(void)
{
	bool L_019118 = false, L_019132 = false, L_019137 = false, L_01914C = false, L_019154 = false, L_019167 = false, L_019180 = false;
	uint8_t fade_level = 0, message = 0, queued_message = 0, route_stage = 0, sound = 0;
	uint16_t buttons = 0;
	uint32_t car_number = 0, object = 0, remark_choice = 0, remark_list = 0, remark_routine = 0, song = 0, song_number = 0, title = 0;
	uint8_t previous_song = 0, timer = 0, timer_2 = 0, timer_3 = 0, cars_to_introduce = 0;
	uint32_t jump_target = 0;

	object = OBJECT(current_object)->car_radio.title;
	if (object != 0u && (OBJECT(object)->flags & 1u) == 0u)
		OBJECT(current_object)->car_radio.title = 0u;
	if (SCREEN(screen)->nearing_fork != 0u)
	{
		route_stage = SCREEN(screen)->route_stage;
		if (OBJECT(current_object)->car_radio.stage != route_stage)
		{
			OBJECT(current_object)->car_radio.stage = route_stage;
			SCREEN(screen)->nearing_fork = 0u;
		}
	}
	do
	{
		do
		{
			do
			{
				do
				{
					do
					{
						do
						{
							jump_target = L_019118 ? 0x190F0u : L_019132 ? 0x190F0u : L_01914C ? 0x190F0u : L_019154 ? 0x190F0u : L_019167 ? 0x190F0u : L_019180 ? 0x190F0u : car_radio_state_jumps[OBJECT(current_object)->state];
							switch (jump_target)
							{
							case 0x019068u:
								song_number = CAR(player_car)->song;
								song_number = car_radio_song(song_number);
								request_sound((uint8_t)song_number);
								OBJECT(current_object)->state++;
							case 0x019078u:
								buttons = OBJECT(current_object)->car_radio.buttons;
								if ((buttons & 0x100u) != 0u)
								{
									OBJECT(current_object)->state = 2u;
									sound = 0x1Fu;
									request_sound(sound);
									return;
								}
								if ((buttons & 0x200u) != 0u)
								{
									if ((buttons & 0x400u) != 0u)
									{
										song_number = 0xAu;
										song_number = car_radio_song(song_number);
										request_sound((uint8_t)song_number);
										return;
									}
									sound = 0x1Fu;
									request_sound(sound);
									song = CAR(player_car)->song;
									previous_song = song - 1u;
									song_number = previous_song;
									if ((int8_t)previous_song < 0)
										song_number = 9u;
									CAR(player_car)->song = song_number;
									song_number = car_radio_song(song_number);
									request_sound((uint8_t)song_number);
									return;
								}
								if ((buttons & 0x400u) == 0u && (buttons & 0x40u) == 0u)
									return;
								sound = 0x1Fu;
								request_sound(sound);
								song_number = CAR(player_car)->song;
								song_number = (uint8_t)(song_number + 1u);
								if ((int8_t)song_number < 0xA)
								{
									CAR(player_car)->song = song_number;
									song_number = car_radio_song(song_number);
									request_sound((uint8_t)song_number);
									return;
								}
								song_number = 0u;
								CAR(player_car)->song = song_number;
								song_number = car_radio_song(song_number);
								request_sound((uint8_t)song_number);
								return;
							case 0x0190F0u:
								if (!L_019118)
								{
									if (!L_019132)
									{
										if (!L_01914C)
										{
											if (!L_019154)
											{
												if (!L_019167)
												{
													if (!L_019180)
													{
														title = radio_station_title_ADDRESS;
														car_radio_show_title(title);
														OBJECT(current_object)->car_radio.queued_message = 0x29u;
													}
												}
											}
										}
									}
								}
								do
								{
									if (L_019137 || (!L_019118 && !L_019132 && (L_01914C || L_019154 || L_019167 || L_019180 || SCREEN(screen)->nearing_fork != 0u)))
									{
										if (L_019137 || L_01914C || !L_019154)
										{
											if (L_019137 || L_01914C || !L_019167)
											{
												if (L_019137 || L_01914C || !L_019180)
												{
													if (L_019137 || !L_01914C)
													{
														L_019137 = false;
														sound = 0xE0u;
														request_sound(sound);
														OBJECT(current_object)->car_radio.timer = 0xB4u;
														OBJECT(current_object)->state = 5u;
													}
													L_01914C = false;
													timer = OBJECT(current_object)->car_radio.timer;
													OBJECT(current_object)->car_radio.timer = timer - 1u;
												}
											}
										}
										if (L_019154 || L_019167 || L_019180 || timer == 1u)
										{
											if (L_019154 || !L_019167)
											{
												if (L_019154 || !L_019180)
												{
													if (!L_019154)
														OBJECT(current_object)->state++;
													L_019154 = false;
												}
											}
											if (!L_019167 && !L_019180 && SCREEN(screen)->nearing_fork == 0u)
											{
												OBJECT(current_object)->car_radio.queued_message = stage_level_radio_messages[SCREEN(screen)->current_stage_level];
												sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
												request_sound(sound);
												OBJECT(current_object)->state = 3u;
											}
											else
											{
												if (L_019167 || !L_019180)
												{
													if (!L_019167)
													{
														sound = 0xEEu;
														request_sound(sound);
														OBJECT(current_object)->state++;
													}
													L_019167 = false;
												}
												if (L_019180 || SCREEN(screen)->nearing_fork == 0u)
												{
													if (!L_019180)
													{
														fade_level = 0x7Fu;
														fade_out_music_at(fade_level);
														OBJECT(current_object)->car_radio.timer = 0x78u;
														OBJECT(current_object)->state++;
													}
													L_019180 = false;
													timer_2 = OBJECT(current_object)->car_radio.timer;
													OBJECT(current_object)->car_radio.timer = timer_2 - 1u;
													if (timer_2 == 1u)
													{
														OBJECT(current_object)->car_radio.queued_message = stage_level_radio_messages[SCREEN(screen)->current_stage_level];
														sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
														request_sound(sound);
														OBJECT(current_object)->state = 3u;
													}
												}
											}
										}
									}
									else
									{
										if (L_019118 || !L_019132)
										{
											if (!L_019118)
											{
												sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
												request_sound(sound);
												OBJECT(current_object)->state++;
											}
											L_019118 = false;
										}
										if (L_019132 || SCREEN(screen)->nearing_fork != 0u)
										{
											if (!L_019132)
											{
												fade_level = 0x3Fu;
												fade_out_music_at(fade_level);
												OBJECT(current_object)->car_radio.timer = 0x3Cu;
												OBJECT(current_object)->state++;
											}
											L_019132 = false;
											timer_3 = OBJECT(current_object)->car_radio.timer;
											OBJECT(current_object)->car_radio.timer = timer_3 - 1u;
											L_019137 = timer_3 == 1u;
										}
									}
								} while (L_019137);
								if ((OBJECT(current_object)->car_radio.buttons & 0x700u) != 0u)
								{
									OBJECT(current_object)->state = 0u;
									OBJECT(current_object)->car_radio.talk_step = 4u;
									SCREEN(screen)->dj_mouth_shapes = 0u;
									sound = 0x1Fu;
									request_sound(sound);
									return;
								}
								if (SCREEN(screen)->dj_mouth_shapes != 0u)
								{
									OBJECT(current_object)->car_radio.queued_message = 0u;
									return;
								}
								switch (OBJECT(current_object)->car_radio.talk_step)
								{
								case 0:
									message = 0u;
									start_dj_message(message);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 1:
									car_number = OBJECT(current_object)->car_radio.next_car_number;
									while ((int8_t)mem_read8(current_object + 0x80u + car_number) < 0)
									{
										car_number = (uint8_t)(car_number + 1u);
									}
									OBJECT(current_object)->car_radio.next_car_number = car_number;
									car_number = (uint8_t)(car_number + 9u);
									start_dj_message((uint8_t)car_number);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 2:
									message = (uint8_t)(mem_read8(current_object + 0x80u + OBJECT(current_object)->car_radio.next_car_number) + 1u);
									start_dj_message(message);
									OBJECT(current_object)->car_radio.next_car_number++;
									cars_to_introduce = OBJECT(current_object)->car_radio.cars_to_introduce;
									OBJECT(current_object)->car_radio.cars_to_introduce = cars_to_introduce - 1u;
									if (cars_to_introduce == 1u)
									{
										OBJECT(current_object)->car_radio.talk_step++;
										return;
									}
									OBJECT(current_object)->car_radio.talk_step--;
									return;
								case 3:
									message = 0x2Au;
									start_dj_message(message);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 4:
									queued_message = OBJECT(current_object)->car_radio.queued_message;
									if (queued_message != 0u)
									{
										OBJECT(current_object)->car_radio.remark_list = 0u;
										start_dj_message(queued_message);
										return;
									}
									if (OBJECT(current_object)->state != 3u)
										return;
									remark_list = OBJECT(current_object)->car_radio.remark_list;
									if (remark_list == 0u)
									{
										remark_choice = random_number();
										remark_choice &= 0x3FFu;
										if ((int32_t)remark_choice >= 0xE)
											return;
										remark_list = routine_lists_1966C[remark_choice];
									}
									remark_routine = mem_read32(remark_list);
									remark_list += 4u;
									if (mem_read32(remark_list) == 0u)
										remark_list = 0u;
									OBJECT(current_object)->car_radio.remark_list = remark_list;
									switch (remark_routine)
									{
									case 0x01929Eu:
										car_radio_dj_leader();
										return;
									case 0x0192B1u:
										car_radio_dj_own_car();
										return;
									case 0x0192BCu:
										car_radio_dj_remark_2a();
										return;
									case 0x0192C3u:
										car_radio_dj_remark_2b();
										return;
									case 0x0192CAu:
										car_radio_dj_remark_2d();
										return;
									case 0x0192D1u:
										car_radio_dj_remark_2f();
										return;
									case 0x0192D8u:
										car_radio_dj_remark_2c();
										return;
									default:
										invalid_jump(remark_routine);
									}
									break;
								case 5:
									return;
								default:
									invalid_jump(car_radio_talk_step_jumps[OBJECT(current_object)->car_radio.talk_step]);
								}
								break;
							case 0x019118u:
								L_019118 = true;
								break;
							case 0x019132u:
								L_019132 = true;
								break;
							case 0x01914Cu:
								L_01914C = true;
								break;
							case 0x019154u:
								L_019154 = true;
								break;
							case 0x019167u:
								L_019167 = true;
								break;
							case 0x019180u:
								L_019180 = true;
								break;
							default:
								invalid_jump(jump_target);
							}
						} while (L_019118);
					} while (L_019132);
				} while (L_01914C);
			} while (L_019154);
		} while (L_019167);
	} while (L_019180);
}

/* A DJ remark: message 11 plus the car number of the car in the lead. */
void car_radio_dj_leader(void)
{
	uint8_t message = 0;

	message = (uint8_t)(car_numbers[RACE(SCREEN(screen)->host_record)->race_order] + 0x11u);
	start_dj_message(message);
}

/* A DJ remark: message 19 plus this screen's car number. */
void car_radio_dj_own_car(void)
{
	uint8_t message = 0;

	message = (uint8_t)(OBJECT(current_object)->car_radio.car_number + 0x19u);
	start_dj_message(message);
}

/* A DJ remark: message 2A (also the end of the cars' introduction). */
void car_radio_dj_remark_2a(void)
{
	uint8_t message = 0;

	message = 0x2Au;
	start_dj_message(message);
}

/* A DJ remark: message 2B. */
void car_radio_dj_remark_2b(void)
{
	uint8_t message = 0;

	message = 0x2Bu;
	start_dj_message(message);
}

/* A DJ remark: message 2D. */
void car_radio_dj_remark_2d(void)
{
	uint8_t message = 0;

	message = 0x2Du;
	start_dj_message(message);
}

/* A DJ remark: message 2F. */
void car_radio_dj_remark_2f(void)
{
	uint8_t message = 0;

	message = 0x2Fu;
	start_dj_message(message);
}

/* A DJ remark: message 2C. */
void car_radio_dj_remark_2c(void)
{
	uint8_t message = 0;

	message = 0x2Cu;
	start_dj_message(message);
}

/* Shows the title of song R0 (radio_songs: 0 Magical Sound Shower, 1 Passing Breeze, 2 Splash Wave, 3
   Picture the Rivers, 4 Blow Your Cool, 5 Looking for the Rainbow, 6 Speed King, 7 Adventure, 8 Sonic
   Control, 9 Last Wave, 10 Jingle Bell) and returns the song's sound. */
uint32_t car_radio_song(uint32_t song_number)
{
	uint32_t song_entry = 0, song_sound = 0;

	song_entry = radio_songs[song_number];
	song_sound = byte_swap32(song_entry);
	song_entry &= 0xFFFFFFu;
	car_radio_show_title(song_entry);
	return song_sound;
}

/* Starts a song title (start_song_title) for the record it is given, sends the one shown (title) away
   and keeps the new one in title. */
void car_radio_show_title(uint32_t title)
{
	uint32_t object = 0, title_object = 0;

	object = OBJECT(current_object)->car_radio.title;
	if (object != 0u)
		OBJECT(object)->song_title.time_left = 0;
	title_object = start_song_title(title);
	OBJECT(current_object)->car_radio.title = title_object;
}

/* Starts an object that shows the song title of record R10 (x, then the text) on the screen and scrolls
   it on the LCD; returns the object. */
uint32_t start_song_title(uint32_t song_title)
{
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->handler = song_title_show_ADDRESS;
	OBJECT(object)->song_title.text = song_title;
	OBJECT(object)->song_title.time_left = 0x3C;
	return object;
}

/* A song title on the screen (start_song_title starts it, for car_radio, the car select and 00C8B6): its
   x and text from the record (x, and the text, which also scrolls on the LCD through screen
   lcd_scroll_request), then slides in from the right (slide from D2, slowing down to a stop by speed),
   stays for time_left (60 frames) and slides out to the left speeding up; sent away early (time_left 0,
   car_radio_show_title) it leaves at 40 a frame. Drawn each frame with draw_text_at at y C8 in colours
   that cycle every 2 frames (song_title_colours by colour_step). */
void song_title_show(void)
{
	bool L_019387 = false, L_01938A = false;
	uint32_t negative = 0;
	uint16_t colour = 0, next_speed = 0, speed = 0;
	uint32_t colour_step = 0, position = 0, record = 0, text = 0;
	uint16_t time_left = 0, new_time_left = 0;
	uint32_t jump_target = 0;

	do
	{
		jump_target = L_01938A ? 0x19369u : song_title_state_jumps[OBJECT(current_object)->state];
		switch (jump_target)
		{
		case 0x019344u:
			record = OBJECT(current_object)->song_title.text;
			OBJECT(current_object)->x = SONG_TITLE_RECORD(record)->x;
			record += 2u;
			OBJECT(current_object)->song_title.text = record;
			SCREEN(screen)->lcd_scroll_request = record;
			OBJECT(current_object)->song_title.slide = 0xD2;
			OBJECT(current_object)->song_title.y = 0xC8u;
			OBJECT(current_object)->song_title.speed = 0x14u;
			OBJECT(current_object)->state++;
		case 0x019369u:
			do
			{
				if (L_019387 || L_01938A || (uint16_t)OBJECT(current_object)->song_title.time_left == 0u)
				{
					if (L_019387 || !L_01938A)
					{
						L_019387 = false;
						OBJECT(current_object)->state++;
					}
					L_01938A = false;
					time_left = OBJECT(current_object)->song_title.time_left;
					new_time_left = time_left - 1u;
					negative = new_time_left >> 15;
					OBJECT(current_object)->song_title.time_left = new_time_left;
					if ((int16_t)time_left > 1)
					{
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
						colour_step = OBJECT(current_object)->song_title.colour_step;
						colour_step = (uint8_t)(colour_step + 1u);
						OBJECT(current_object)->song_title.colour_step = colour_step;
						colour_step = colour_step >> 1;
						colour = song_title_colours[colour_step & 7u];
						text = OBJECT(current_object)->song_title.text;
						draw_text_at(position, colour, text);
						return;
					}
					if (negative)
					{
						OBJECT(current_object)->state += 2u;
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
						OBJECT(current_object)->song_title.slide = (uint16_t)position;
						OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
						if ((int16_t)position <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
					else
					{
						OBJECT(current_object)->state++;
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						next_speed = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
						position = (position & ~0xFFFFu) | (uint16_t)(position - next_speed);
						OBJECT(current_object)->song_title.speed = next_speed;
						OBJECT(current_object)->song_title.slide = (uint16_t)position;
						OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
						if ((int16_t)position <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
				}
				else
				{
					position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
					speed = OBJECT(current_object)->song_title.speed;
					position = (position & ~0xFFFFu) | (uint16_t)(position - speed);
					OBJECT(current_object)->song_title.speed = speed - 1u;
					OBJECT(current_object)->song_title.slide = (uint16_t)position;
					OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
					L_019387 = (uint16_t)position == 0u;
				}
			} while (L_019387);
			break;
		case 0x01938Au:
			L_01938A = true;
			break;
		case 0x019394u:
			position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			next_speed = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
			position = (position & ~0xFFFFu) | (uint16_t)(position - next_speed);
			OBJECT(current_object)->song_title.speed = next_speed;
			OBJECT(current_object)->song_title.slide = (uint16_t)position;
			OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
			if ((int16_t)position <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
			colour_step = OBJECT(current_object)->song_title.colour_step;
			colour_step = (uint8_t)(colour_step + 1u);
			OBJECT(current_object)->song_title.colour_step = colour_step;
			colour_step = colour_step >> 1;
			colour = song_title_colours[colour_step & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(position, colour, text);
			return;
		case 0x0193BAu:
			position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
			OBJECT(current_object)->song_title.slide = (uint16_t)position;
			OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
			if ((int16_t)position <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
			colour_step = OBJECT(current_object)->song_title.colour_step;
			colour_step = (uint8_t)(colour_step + 1u);
			OBJECT(current_object)->song_title.colour_step = colour_step;
			colour_step = colour_step >> 1;
			colour = song_title_colours[colour_step & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(position, colour, text);
			return;
		default:
			invalid_jump(jump_target);
		}
	} while (L_01938A);
	position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
	colour_step = OBJECT(current_object)->song_title.colour_step;
	colour_step = (uint8_t)(colour_step + 1u);
	OBJECT(current_object)->song_title.colour_step = colour_step;
	colour_step = colour_step >> 1;
	colour = song_title_colours[colour_step & 7u];
	text = OBJECT(current_object)->song_title.text;
	draw_text_at(position, colour, text);
}

/* Starts DJ message R0 (start_dj_message) unless one is being said (screen dj_mouth_shapes). */
void start_dj_message_if_quiet(uint8_t dj_message)
{
	if (SCREEN(screen)->dj_mouth_shapes != 0u)
		return;
	start_dj_message(dj_message);
}

/* Starts DJ message R0 (typed_messages): requests its voice (the sound in the record's top byte) and has
   the face on the LCD move its mouth by the record's string (screen dj_mouth_shapes). */
void start_dj_message(uint8_t message)
{
	uint8_t sound = 0;
	uint32_t message_entry = 0;

	message_entry = dj_messages[(uint8_t)message];
	sound = (uint8_t)byte_swap32(message_entry);
	SCREEN(screen)->dj_mouth_shapes = message_entry & 0xFFFFFFu;
	request_sound(sound);
}

/* The title of the attract mode. By state: 0 sets it up: clears screen 5800 and 5801 (title_logo_shine,
   the logo's shine signal), keeps the addresses of the tilemaps' register copies
   (title_screen_layer_setups by screen: scroll_x_address, scroll_y_address for the first tilemap,
   second_scroll_x_address, second_scroll_y_address for the second), the background (set_up_background
   with title_background_setups by screen) and its scrolls, picks the car of the next car presentation
   (screen attract_car from unk_9E, which steps through the eight), starts the credits line
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
		mem_write16(OBJECT(current_object)->title_screen.second_scroll_y_address, 0xFFF0u);
		mem_write16(OBJECT(current_object)->title_screen.scroll_x_address, 0x360u);
		scroll_x_address = OBJECT(current_object)->title_screen.scroll_x_address;
		mem_write16(scroll_x_address, mem_read16(scroll_x_address) + ((SCREEN(screen)->car_number * 5u) << 6));
		mem_write16(OBJECT(current_object)->title_screen.scroll_y_address, 0x72u);
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
		mem_write16(scroll_x_address_2, mem_read16(scroll_x_address_2) + 8u);
		second_scroll_x_address = OBJECT(current_object)->title_screen.second_scroll_x_address;
		mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) + 0x14u);
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
		mem_write16(scroll_x_address_3, mem_read16(scroll_x_address_3) + 8u);
		second_scroll_x_address_2 = OBJECT(current_object)->title_screen.second_scroll_x_address;
		mem_write16(second_scroll_x_address_2, mem_read16(second_scroll_x_address_2) + 0x14u);
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
		animation = 0x59u;
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
		animation = 0x25Du;
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
		animation = 0x25Eu;
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
	animation = 0x25Fu;
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
		animation = 0x25Au;
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
		animation = 0x25Bu;
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

/* The prompt at the bottom of the title, the car presentations and the demo races, and its text on the
   cabinet's LCD; ends at once in game mode 6 (screen game_mode). Every 30 frames (round_frames): a
   little car crosses the message (coin_prompt_car with this round's car; next_car then steps through the
   eight, from screen car_number) and the message by the seat (seat1_credits and seat1_coin_units for
   screen 1, seat2_credits and seat2_coin_units for screen 2): with free play (free_play) or a credit
   message 2, TURN THE IGNITION KEY (coin_prompt_start_key_prompt); else message 1 with coins in toward a
   credit or 0, a coin_prompt_message in the car's colour (INSERT COIN(S)). Each frame the LCD's text to
   scroll (screen lcd_scroll_request) is the message's (coin_prompt_lcd_texts: Insert Coin(s), Insert
   More Coin(s), Turn the Ignition Key), except in game modes 6 and 7 or while a text scrolls (unk_F44). */
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
	default:
		invalid_jump(coin_prompt_state_jumps[OBJECT(current_object)->state]);
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
		default:
			invalid_jump(coin_prompt_screen_jumps[SCREEN(screen)->index]);
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
	uint32_t animation = 0, palette = 0;
	uint16_t x = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	palette = coin_prompt_car_state_jumps[state];
	switch (state)
	{
	case 0:
		animation = coin_prompt_car_animations[OBJECT(current_object)->coin_prompt_car.car];
		animation_start(animation);
		palette = (uint16_t)(coin_prompt_car_palettes[OBJECT(current_object)->coin_prompt_car.car_colour] + 4u);
		OBJECT(current_object)->palette = palette;
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
	default:
		invalid_jump(coin_prompt_car_state_jumps[state]);
	}
	x = OBJECT(current_object)->x - 0xEu;
	OBJECT(current_object)->x = x;
	if ((int16_t)x >= 0)
	{
		palette = draw_object(palette);
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
	uint32_t animation = 0, car_colour = 0;
	uint8_t frames = 0, frames_2 = 0, frames_3 = 0;
	uint32_t state = 0;

	if (OBJECT(OBJECT(current_object)->coin_prompt_message.prompt)->coin_prompt.message != OBJECT(current_object)->coin_prompt_message.message)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	state = OBJECT(current_object)->state;
	car_colour = coin_prompt_message_state_jumps[state];
	switch (state)
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
		car_colour = OBJECT(current_object)->coin_prompt_message.car_colour;
		OBJECT(current_object)->palette = coin_prompt_message_palettes[car_colour];
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
		car_colour = OBJECT(current_object)->coin_prompt_message.car_colour;
		OBJECT(current_object)->palette = coin_prompt_message_palettes[car_colour];
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
			car_colour = draw_object(car_colour);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(coin_prompt_message_state_jumps[state]);
	}
	animation_advance(0u);
	frames_3 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_3 - 1u;
	if (frames_3 != 1u)
	{
		car_colour = draw_object(car_colour);
		return;
	}
	OBJECT(current_object)->depth_layer--;
	OBJECT(current_object)->coin_prompt_message.frames = 0x1Eu;
	OBJECT(current_object)->state++;
	frames_2 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		car_colour = draw_object(car_colour);
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
	uint32_t animation = 0, piece = 0, piece_offset = 0, state_routine = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = ignition_key_prompt_state_jumps[state];
	switch (state)
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
		state_routine = draw_object(state_routine);
		return;
	default:
		invalid_jump(ignition_key_prompt_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	piece_offset = (uint32_t)OBJECT(current_object)->ignition_key_prompt.piece << 4;
	piece = ignition_key_prompt_pieces_ADDRESS + piece_offset;
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
	piece_offset = draw_object(piece_offset);
}

/* The credits line under the prompt (title, car presentations, demo races); ends in game mode 6. With
   free play (free_play) FREE PLAY; otherwise, for the screen's seat (seat1_credits and seat1_coin_units
   for screen 1, seat2_credits and seat2_coin_units for screen 2): Credit (0 or 1) or Credits and the
   number of credits in the font's digits (tens at x AC when there are, units at B4), and while coins are
   in toward the next credit the coins and the coins a credit costs (coins_per_credit) as a fraction (C0
   and C8), each drawn with credit_display_digit. */
void credit_display_update(void)
{
	uint8_t coin_units_shown = 0, units = 0;
	uint32_t coin_units = 0, credits = 0, ignored = 0, text = 0;
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
			ignored = draw_text(text);
			return;
		}
		OBJECT(current_object)->state += 2u;
		break;
	case 1:
		text = credit_free_play_text_ADDRESS;
		ignored = draw_text(text);
		return;
	case 2:
		break;
	default:
		invalid_jump(credit_display_state_jumps[OBJECT(current_object)->state]);
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
	default:
		invalid_jump(credit_display_screen_jumps[SCREEN(screen)->index]);
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
	r12 = r12 & ~0xFFu;
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

/* The captions over the demo race of the attract mode: counts its frames and runs the routines of
   timeline_1AFC0 as it reaches their frames (30 demo_captions_head_to_head, 225
   demo_captions_checkpoint, 420 demo_captions_music_select); ends once the count passes a record's frame
   (the closing FFFF). */
void demo_captions_run(void)
{
	uint32_t zero = 0;
	uint32_t cue = 0;
	uint32_t frames_to_cue = 0, jump_target = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->demo_captions.timeline = timeline_1AFC0_ADDRESS;
		OBJECT(current_object)->demo_captions.frames &= 0xFFFF0000u;
		OBJECT(current_object)->state++;
		return;
	case 1:
		break;
	default:
		invalid_jump(demo_captions_state_jumps[OBJECT(current_object)->state]);
	}
	OBJECT(current_object)->demo_captions.frames = (OBJECT(current_object)->demo_captions.frames & 0xFFFF0000u) | ((uint16_t)OBJECT(current_object)->demo_captions.frames + 1u & 0xFFFFu);
	cue = OBJECT(current_object)->demo_captions.timeline;
	frames_to_cue = DEMO_CAPTION_CUE(cue)->frame - (uint16_t)OBJECT(current_object)->demo_captions.frames;
	zero = frames_to_cue == 0u;
	cue += 4u;
	if ((int32_t)frames_to_cue < 0)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (zero)
	{
		jump_target = DEMO_CAPTION_CUE(cue - 0x4u)->routine;
		switch (jump_target)
		{
		case 0x01AE2Du:
			demo_captions_head_to_head();
			break;
		case 0x01AE61u:
			demo_captions_checkpoint();
			break;
		case 0x01AE6Bu:
			demo_captions_music_select();
			break;
		default:
			invalid_jump(jump_target);
		}
		OBJECT(current_object)->demo_captions.timeline += 8u;
	}
}

/* Frame 30 of the demo captions: HEAD TO HEAD! 2 PLAYER RACING (demo_caption records 0, 1 and 3 +
   machine_count, or 4 when stand_alone is set; 4 in attract.txt). */
void demo_captions_head_to_head(void)
{
	uint8_t first_record = 0, record = 0;
	uint32_t object = 0, record_count = 0;

	record_count = 2u;
	first_record = 0u;
	demo_captions_start(first_record, record_count);
	record = 3u;
	if (stand_alone == 0u)
		record += machine_count;
	else
		record++;
	object = start_object();
	OBJECT(object)->handler = demo_caption_update_ADDRESS;
	OBJECT(object)->demo_caption.record = record;
}

/* Frame 225 of the demo captions: AT THE CHECKPOINT ONLY THE RACE LEADER CAN SELECT THE NEXT STAGE !
   (demo_caption records 2 and 3). */
void demo_captions_checkpoint(void)
{
	uint8_t first_record = 0;
	uint32_t record_count = 0;

	record_count = 2u;
	first_record = 2u;
	demo_captions_start(first_record, record_count);
}

/* Frame 420 of the demo captions: the radio's panel, the hand pressing its button and MUSIC SELECT
   (demo_caption records 8-11). */
void demo_captions_music_select(void)
{
	uint8_t first_record = 0;
	uint32_t record_count = 0;

	record_count = 4u;
	first_record = 8u;
	demo_captions_start(first_record, record_count);
}

/* Starts R1 demo_caption objects with the records from R0 on. */
void demo_captions_start(uint8_t first_record, uint32_t record_count)
{
	uint32_t object = 0;

	do
	{
		object = start_object();
		OBJECT(object)->handler = demo_caption_update_ADDRESS;
		OBJECT(object)->demo_caption.record = first_record;
		first_record++;
		record_count--;
	} while (record_count != 0u);
}

/* One picture of the demo captions (demo_captions_start starts them): from its record at 01AF00
   (animation, palette, width_scale, height_scale, x, y, depth_layer, frames_left: 180 frames for records
   0-7, 240 for 8-11), animated and drawn until frames_left runs out. */
void demo_caption_update(void)
{
	uint32_t animation = 0, picture = 0, record_offset = 0, state_routine = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	state_routine = demo_caption_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		frames_left = OBJECT(current_object)->demo_caption.frames_left;
		OBJECT(current_object)->demo_caption.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		animation_advance(0u);
		state_routine = draw_object(state_routine);
		return;
	default:
		invalid_jump(demo_caption_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	record_offset = (uint32_t)OBJECT(current_object)->demo_caption.record << 4;
	picture = demo_caption_pictures_ADDRESS + record_offset;
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
	picture += 2u;
	OBJECT(current_object)->depth_layer = PICTURE_RECORD(picture - 0xCu)->depth_layer;
	OBJECT(current_object)->demo_caption.frames_left = PICTURE_RECORD(picture - 0xCu)->frames_left;
	OBJECT(current_object)->state++;
	animation_advance(0u);
	record_offset = draw_object(record_offset);
}

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
	uint32_t cell = 0, line = 0, line_place = 0, text = 0, text_ended = 0;
	uint32_t video_registers_0_long = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		SCREEN(screen)->unpack_state &= 0xFF00u;
		init_video();
		video_registers_0_long = (video_registers[0] | ((uint32_t)video_registers[1] << 16u)) & ~0x800u;
		video_registers[0] = (uint16_t)video_registers_0_long;
		video_registers[1] = (uint16_t)(video_registers_0_long >> 16u);
		video_registers[1] = 0xFu;
		video_registers[70] = 0xFFu;
		video_registers[68] = 0u;
		video_registers[69] = 0u;
		send_video_registers = 0xFFu;
		SCREEN(screen)->kanji_text_number = 0u;
		SCREEN(screen)->kanji_text_units = 0;
		SCREEN(screen)->kanji_text_tens = 0;
		SCREEN(screen)->kanji_text_hundreds = 0;
		OBJECT(current_object)->x = 0xEu;
		OBJECT(current_object)->state++;
		return;
	case 1:
		if ((SCREEN(screen)->buttons_pressed & 8u) != 0u)
			OBJECT(current_object)->x -= 2u;
		if ((SCREEN(screen)->buttons_pressed & 4u) != 0u)
			OBJECT(current_object)->x += 2u;
		if ((int16_t)OBJECT(current_object)->x < 0xA)
			OBJECT(current_object)->x = 0xEu;
		if ((int16_t)OBJECT(current_object)->x > 0xE)
			OBJECT(current_object)->x = 0xAu;
		if (OBJECT(current_object)->x == 0xAu)
		{
			if ((SCREEN(screen)->buttons_pressed & 2u) != 0u)
				SCREEN(screen)->kanji_text_hundreds++;
			if ((SCREEN(screen)->buttons_pressed & 1u) != 0u)
				SCREEN(screen)->kanji_text_hundreds--;
			if (SCREEN(screen)->kanji_text_hundreds < 0)
				SCREEN(screen)->kanji_text_hundreds = 9;
			if (SCREEN(screen)->kanji_text_hundreds > 9)
				SCREEN(screen)->kanji_text_hundreds = 0;
		}
		else if (OBJECT(current_object)->x == 0xCu)
		{
			if ((SCREEN(screen)->buttons_pressed & 2u) != 0u)
				SCREEN(screen)->kanji_text_tens++;
			if ((SCREEN(screen)->buttons_pressed & 1u) != 0u)
				SCREEN(screen)->kanji_text_tens--;
			if (SCREEN(screen)->kanji_text_tens < 0)
				SCREEN(screen)->kanji_text_tens = 9;
			if (SCREEN(screen)->kanji_text_tens > 9)
				SCREEN(screen)->kanji_text_tens = 0;
		}
		else
		{
			if ((SCREEN(screen)->buttons_pressed & 2u) != 0u)
				SCREEN(screen)->kanji_text_units++;
			if ((SCREEN(screen)->buttons_pressed & 1u) != 0u)
				SCREEN(screen)->kanji_text_units--;
			if (SCREEN(screen)->kanji_text_units < 0)
				SCREEN(screen)->kanji_text_units = 9;
			if (SCREEN(screen)->kanji_text_units > 9)
				SCREEN(screen)->kanji_text_units = 0;
		}
		if (SCREEN(screen)->index == 0u)
		{
			text_number = (uint16_t)(SCREEN(screen)->kanji_text_units + SCREEN(screen)->kanji_text_tens * 0xAu + SCREEN(screen)->kanji_text_hundreds * 0x64u);
			SCREEN(screen)->kanji_text_number = text_number;
			cell = TEXT_CELL(5, 10);
			kanji_text_viewer_draw_number(cell, text_number);
			OBJECT(current_object)->kanji_text_viewer.blink_count++;
			if ((OBJECT(current_object)->kanji_text_viewer.blink_count & 3u) == 0u)
				mem_write16(TEXT_CELL(0, 10) + OBJECT(current_object)->x, 0x602Au);
		}
		if (SCREEN(screen)->index != 0u)
			return;
		text_ended = 0u;
		line = 0u;
		text = kanji_texts[SCREEN(screen)->kanji_text_number];
		do
		{
			line_place = VIDEO_RAM + 0x1040u + line * 0x1000u;
			kanji_text_viewer_draw_line(line_place, &text_ended, &text);
			line++;
		} while (text_ended == 0u);
		return;
	case 2:
		if (SCREEN(screen)->index != 0u)
			return;
		text_ended = 0u;
		line = 0u;
		text = kanji_texts[SCREEN(screen)->kanji_text_number];
		do
		{
			line_place = VIDEO_RAM + 0x1040u + line * 0x1000u;
			kanji_text_viewer_draw_line(line_place, &text_ended, &text);
			line++;
		} while (text_ended == 0u);
		return;
	case 3:
		return;
	default:
		invalid_jump(kanji_text_viewer_state_jumps[OBJECT(current_object)->state]);
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
		kanji_text_viewer_draw_character(place, character);
		place += 8u;
	}
	*p_text_ended = 0xFFFFFFFFu;
}

/* Only unused code reaches it (kanji_text_viewer_draw_line): draws a 16x16 character on the bitmap layer
   at R11 from its 32 bytes at R13: four 8x8 quarters (kanji_text_viewer_draw_quarter), upper left, upper
   right, lower left, lower right. */
void kanji_text_viewer_draw_character(uint32_t place, uint32_t rows)
{
	r12 = place;
	rows = kanji_text_viewer_draw_quarter(rows);
	r12 = place + 4u;
	rows = kanji_text_viewer_draw_quarter(rows);
	r12 = place + 0x800u;
	rows = kanji_text_viewer_draw_quarter(rows);
	r12 = place + 0x804u;
	rows = kanji_text_viewer_draw_quarter(rows);
}

/* Only unused code reaches it (kanji_text_viewer_draw_character): draws eight rows of eight one-bit
   pixels (a byte each, from R13) at R12 on the bitmap layer, 100 bytes a row, each byte spread by the
   table at 1A6340 into eight 4-bit pixels of pen 1. */
uint32_t kanji_text_viewer_draw_quarter(uint32_t rows)
{
	uint32_t rows_left = 0;

	rows_left = 8u;
	do
	{
		mem_write32(r12, byte_swap32(kanji_row_pixels[KANJI_CHARACTER_ROWS(rows)[0]]));
		r12 = r12 + 0x100u;
		rows_left--;
		rows++;
	} while (rows_left != 0u);
	return rows;
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
	mem_write16(cell, hundreds + 0x6030u);
	cell += 2u;
	mem_write16(cell, tens + 0x6030u);
	mem_write16(cell + 2u, number + 0x6030u);
}

/* Enters the test mode (reset calls it once when test_mode_on says the test mode is on): turns the link
   board off, clears the tilemaps and the sprite lists (init_video, init_sprite_lists), clears both
   screens' pending tile unpacking (screen unpack_state), demo flag (unk_90 bit 8) and unk_94 bit 0, the
   mixers' registers 40-4A, turns both screens on (the I/O chips' CNT), sets up the test mode's palettes
   and font (test_mode_set_up_colours), test_mode_flags 0 (INDIVIDUAL) and the cursor on EXIT, stops both
   monitors' sounds (command 1F through the sound command queue, test_mode_queue_sound and
   test_mode_queue_sound_screen_2), turns the lamps and the wheel motor off
   (checkpoint_lamp-wheel_motor[1], screen button_lamps, port_d_outputs, port_g_outputs), writes " LCD
   status OK! " on both cabinets' LCDs (the 16 bytes at F10 of each screen's block), sets
   test_mode_routine to test_mode_menu and reads the controls (test_mode_read_controls). */
void test_mode_start(void)
{
	uint16_t sound = 0;
	uint32_t text_0_to_3 = 0, text_12_to_15 = 0, text_4_to_7 = 0, text_8_to_11 = 0;
	uint32_t port_d_outputs_0_halfword = 0, port_g_outputs_0_halfword = 0;

	mem_write16(0x801000u, 0u);
	mem_write8(IO_C00074, 0x1Fu);
	init_video();
	init_sprite_lists();
	SCREEN(SCREEN_1)->unpack_state = 0u;
	SCREEN(SCREEN_1)->unpack_read_place = 0u;
	SCREEN(SCREEN_1)->unpack_write_place = 0u;
	SCREEN(SCREEN_2)->unpack_state = 0u;
	SCREEN(SCREEN_2)->unpack_read_place = 0u;
	SCREEN(SCREEN_2)->unpack_write_place = 0u;
	SCREEN(SCREEN_1)->demo_flags &= ~0x100u;
	SCREEN(SCREEN_2)->demo_flags &= ~0x100u;
	SCREEN(SCREEN_1)->flags &= ~1u;
	SCREEN(SCREEN_2)->flags &= ~1u;
	mem_write32(MIXER_610040, 0u);
	mem_write32(MIXER_610044, 0u);
	mem_write32(MIXER_610048, 0u);
	mem_write32(0x690040u, 0u);
	mem_write32(0x690044u, 0u);
	mem_write32(0x690048u, 0u);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	test_mode_set_up_colours();
	test_mode_flags = 0u;
	test_mode_cursor = 0xCu;
	test_mode_sound_queue_write = 0u;
	test_mode_sound_queue_read = 0u;
	sound = 0x1Fu;
	test_mode_queue_sound(sound);
	test_mode_queue_sound_screen_2(sound);
	mem_write8(SOUND_ENGINE_REVS_1, 0u);
	mem_write8(SOUND_ENGINE_REQUESTS_1, mem_read8(SOUND_ENGINE_REQUESTS_1) | 0x20u);
	mem_write8(SOUND_ENGINE_REVS_2, 0u);
	mem_write8(SOUND_ENGINE_REQUESTS_2, mem_read8(SOUND_ENGINE_REQUESTS_2) | 0x20u);
	wheel_motor[0] = 0u;
	wheel_motor[1] = 0u;
	checkpoint_lamp[0] = 0u;
	checkpoint_lamp_blinks = 0u;
	checkpoint_lamp_period = 0u;
	checkpoint_lamp_timer = 0u;
	checkpoint_lamp_2 = 0u;
	checkpoint_lamp_2_blinks = 0u;
	checkpoint_lamp_2_period = 0u;
	checkpoint_lamp_2_timer = 0u;
	leader_lamp[0] = 0u;
	leader_lamp_blinks = 0u;
	leader_lamp_period = 0u;
	leader_lamp_timer = 0u;
	leader_lamp_2 = 0u;
	leader_lamp_2_blinks = 0u;
	leader_lamp_2_period = 0u;
	leader_lamp_2_timer = 0u;
	SCREEN(SCREEN_1)->button_lamps = 0u;
	SCREEN(SCREEN_2)->button_lamps = 0u;
	port_d_outputs_0_halfword = (port_d_outputs[0] | ((uint32_t)port_d_outputs[1] << 8u)) & 0xE0E1u;
	port_d_outputs[0] = (uint8_t)port_d_outputs_0_halfword;
	port_d_outputs[1] = (uint8_t)(port_d_outputs_0_halfword >> 8u);
	port_g_outputs_0_halfword = (port_g_outputs[0] | ((uint32_t)port_g_outputs[1] << 8u)) & ~0x403u;
	port_g_outputs[0] = (uint8_t)port_g_outputs_0_halfword;
	port_g_outputs[1] = (uint8_t)(port_g_outputs_0_halfword >> 8u);
	text_0_to_3 = test_mode_lcd_text;
	text_4_to_7 = test_mode_lcd_text_4_to_7;
	AS32(SCREEN(SCREEN_1)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = text_4_to_7;
	AS32(SCREEN(SCREEN_2)->lcd_text) = text_0_to_3;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = text_4_to_7;
	text_8_to_11 = test_mode_lcd_text_8_to_11;
	text_12_to_15 = test_mode_lcd_text_12_to_15;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = text_12_to_15;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = text_8_to_11;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = text_12_to_15;
	test_mode_routine = test_mode_menu_ADDRESS;
	r12 = unk_218F20_ADDRESS;
	test_mode_read_controls();
}

/* The test mode's frame, which reset runs every video frame while the test mode is on: reads the buttons
   and the analog inputs (test_mode_read_controls), sends the sound board the next queued sound command
   (test_mode_send_sound) and runs test_mode_routine, the step of the menu or page being shown. */
void test_mode_frame(void)
{
	test_mode_read_controls();
	test_mode_send_sound();
	call_code(test_mode_routine);
}

/* Shows the test mode's menu, where every page returns: in CONTINUE mode (test_mode_flags bit 0) the
   cursor first moves on to the next item; clears the text layer, shows the E2PROM write count while
   SERVICE is held (test_mode_menu_show_write_count), draws TEST MODE, the heading (INDIVIDUAL or
   CONTINUE), the twelve items and SELECT BY SERVICE BUTTON AND PUSH TEST BUTTON, puts the cursor on
   test_mode_cursor (test_mode_set_cursor) and goes on to test_mode_menu_run. With the cursor on the
   heading (after test_mode_menu_toggle_continue has redrawn it) it goes straight on. */
void test_mode_menu(void)
{
	uint32_t carry = 0;
	uint32_t cell_value = 0, cursor_cell = 0, heading = 0, text = 0;

	if (test_mode_cursor == 0u)
	{
		test_mode_routine = test_mode_menu_run_ADDRESS;
		test_mode_menu_run();
		return;
	}
	if ((test_mode_flags & 1u) != 0u)
		test_mode_cursor++;
	cell_value = test_mode_clear_text();
	cell_value = test_mode_menu_show_write_count(cell_value);
	text = test_mode_menu_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	carry = (test_mode_flags & 1u) != 0u;
	heading = carry;
	text = test_mode_menu_headings[heading];
	heading = test_mode_draw_texts(heading, text);
	test_mode_draw_instructions(heading);
	cursor_cell = 0x310000u | test_mode_menu_cursor_cells[test_mode_cursor];
	test_mode_set_cursor(heading, cursor_cell);
	test_mode_routine = test_mode_menu_run_ADDRESS;
	test_mode_menu_run();
}

/* The menu, each frame: SERVICE moves the cursor (test_mode_cursor) down, from EXIT back to the heading;
   TEST starts the item's page (test_mode_page_jumps: test_mode_menu_toggle_continue,
   test_mode_memory_test, test_mode_volume_adjustment, test_mode_input_test, test_mode_output_test,
   test_mode_sound_test, test_mode_crt_test, test_mode_game_assignments, test_mode_coin_assignments, then
   the network assignments, bookkeeping, backup data clear and exit pages,
   test_mode_network_assignments_start, test_mode_bookkeeping_start, test_mode_backup_data_clear_start,
   test_mode_exit); the cursor's arrow blinks (test_mode_blink_address, test_mode_blink_timer: 11 frames
   shown, 6 hidden). */
void test_mode_menu_run(void)
{
	uint32_t cursor = 0, cursor_cell = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		cursor = test_mode_cursor;
		cursor = (uint8_t)(cursor + 1u);
		if ((int8_t)cursor >= 0xD)
			cursor = 0u;
		test_mode_cursor = cursor;
		cursor_cell = 0x310000u | test_mode_menu_cursor_cells[cursor];
		test_mode_set_cursor(cursor, cursor_cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_page_jumps[test_mode_cursor];
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* While SERVICE is held as the menu is drawn: E2PROM WRITE COUNT and the count (eeprom_write_count, 5
   digits) at the bottom of the menu. */
uint32_t test_mode_menu_show_write_count(uint32_t cell_value)
{
	uint32_t attribute = 0, cell = 0, digits = 0, text = 0, write_count = 0;

	if ((test_mode_buttons & 1u) == 0u)
		return cell_value;
	text = test_mode_write_count_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	cell = TEXT_CELL(27, 26);
	write_count = eeprom_write_count;
	attribute = 0x64u;
	digits = 5u;
	test_mode_draw_number(digits, &write_count, &attribute, &cell);
	return write_count;
}

/* The menu's heading: TEST switches between INDIVIDUAL and CONTINUE (test_mode_flags bit 0) and redraws
   it, then back to test_mode_menu. In CONTINUE mode each return to the menu moves the cursor to the next
   item, so the pages run one after another with TEST alone. */
void test_mode_menu_toggle_continue(void)
{
	uint32_t carry = 0;
	uint32_t heading = 0, text = 0;
	uint32_t mode_flags = 0, test_mode_flags_long = 0;

	mode_flags = (test_mode_flags | ((uint32_t)test_mode_cursor << 8u) | ((uint32_t)test_mode_page_cursor << 16u) | ((uint32_t)(test_mode_routine & 0xFFu) << 24u));
	carry = (mode_flags & 1u) != 0u;
	test_mode_flags_long = mode_flags ^ 1u;
	test_mode_flags = (uint8_t)test_mode_flags_long;
	test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	test_mode_routine = (test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	heading = 0u - (0xFFFFFFFFu + carry);
	text = test_mode_menu_headings[heading];
	test_mode_draw_texts(heading, text);
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* The MEMORY TEST, all at once with the interrupts off and the sound program held in its wait loop
   (sound RAM 1FFF = 1): each chip's result shows TEST while it is tested, then GOOD or BAD
   (memory_test_results, test_mode_memory_test_draw). The program ROM's two halves by
   test_mode_memory_test_checksum against the sums at 03FFF8-03FFFF (IC37, IC40: 000000-03FFF7; IC36,
   IC39: 100000-1FEFFF); the video RAM (IC60, IC58, IC57, IC54) and the sprite RAM (IC61, IC67, IC68,
   IC70) by 4-bit lanes (test_mode_memory_test_nibbles); the palette RAMs' four 16 KB blocks by byte
   (IC1, IC3, IC2, IC4, IC120, IC121, IC122, IC123; test_mode_memory_test_halfwords); the work RAM
   (IC115-IC118, test_mode_memory_test_words) and the sound RAM (IC21, test_mode_memory_test_bytes) each
   saved in the sprite RAM and put back. The tilemaps, sprite lists and palettes it overwrites are set up
   again (init_video, init_sprite_lists, test_mode_set_up_colours). IC70 always shows GOOD: the test of
   its lane has no branch to BAD. Ends with the blinking PUSH TEST BUTTON TO EXIT
   (test_mode_show_exit_prompt) and test_mode_memory_test_run. */
void test_mode_memory_test(void)
{
	uint8_t blank_result = 0, chip_result = 0;
	uint16_t even_chip_result = 0;
	uint32_t cell_value = 0, failures = 0, memory_start = 0, odd_check = 0, odd_chip_result = 0, rom_check = 0;
	uint8_t video_lane_0_failures = 0, video_lane_1_failures = 0, sprite_lane_0_failures = 0, sprite_lane_1_failures = 0;
	uint16_t low_rom_check = 0, high_rom_check = 0, video_lane_2_failures = 0, sprite_lane_2_failures = 0, sprite_lane_3_failures = 0, work_byte_1_failures = 0;
	uint32_t work_byte_2_failures = 0, work_byte_3_failures = 0;

	interrupts_off();
	cell_value = test_mode_clear_text();
	mem_write8(SOUND_701FFF, 1u);
	current_object = memory_test_results_ADDRESS;
	blank_result = 0u;
	set_bytes(memory_test_results_ADDRESS, 0x19u, blank_result);
	OBJECT(current_object)->flags = 1u;
	OBJECT(current_object)->flags_high = 1u;
	test_mode_memory_test_draw(cell_value);
	memory_start = 0u;
	rom_check = LOW_ROM_TESTED_BYTES;
	test_mode_memory_test_checksum(memory_start, &rom_check, &odd_check);
	low_rom_check = rom_check;
	even_chip_result = (uint16_t)((rom_check & ~0xFFu) | 2u);
	if (ic37_checksum != low_rom_check)
		even_chip_result = (uint16_t)((even_chip_result & ~0xFFu) | 3u);
	OBJECT(current_object)->flags = (uint8_t)even_chip_result;
	odd_chip_result = (even_chip_result & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (ic40_checksum != (uint16_t)odd_check)
		odd_chip_result = (odd_chip_result & ~0xFFu) | 3u;
	OBJECT(current_object - 0x1u)->flags_high = odd_chip_result;
	current_object = current_object + 1u;
	OBJECT(current_object - 0x2u)->unk_02 = 0x101u;
	test_mode_memory_test_draw(odd_chip_result);
	memory_start = 0x100000u;
	rom_check = HIGH_ROM_TESTED_BYTES;
	test_mode_memory_test_checksum(memory_start, &rom_check, &odd_check);
	high_rom_check = rom_check;
	even_chip_result = (uint16_t)((rom_check & ~0xFFu) | 2u);
	if (ic36_checksum != high_rom_check)
		even_chip_result = (uint16_t)((even_chip_result & ~0xFFu) | 3u);
	OBJECT(current_object - 0x2u)->unk_02 = (OBJECT(current_object - 0x2u)->unk_02 & 0xFF00u) | (even_chip_result & 0xFFu);
	odd_chip_result = (even_chip_result & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (ic39_checksum != (uint16_t)odd_check)
		odd_chip_result = (odd_chip_result & ~0xFFu) | 3u;
	OBJECT(current_object - 0x3u)->unk_02 = (OBJECT(current_object - 0x3u)->unk_02 & 0xFFu) | ((odd_chip_result & 0xFFu) << 8u);
	current_object = current_object + 1u;
	OBJECT(current_object - 0x4u)->handler = 0x1010101u;
	test_mode_memory_test_draw(odd_chip_result);
	memory_start = 0x300000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_nibbles(failures, memory_start);
	video_lane_0_failures = failures & 0xFu;
	chip_result = 2u;
	if (video_lane_0_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x4u)->handler = (OBJECT(current_object - 0x4u)->handler & 0xFFFFFF00u) | (chip_result & 0xFFu);
	video_lane_1_failures = failures & 0xF0u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (video_lane_1_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x5u)->handler = (OBJECT(current_object - 0x5u)->handler & 0xFFFF00FFu) | ((chip_result & 0xFFu) << 8u);
	video_lane_2_failures = failures & 0xF00u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (video_lane_2_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x6u)->handler = (OBJECT(current_object - 0x6u)->handler & 0xFF00FFFFu) | ((chip_result & 0xFFu) << 16u);
	chip_result = 2u;
	current_object = current_object + 1u;
	if ((failures & 0xF000u) != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x7u)->handler = (OBJECT(current_object - 0x7u)->handler & 0xFFFFFFu) | ((chip_result & 0xFFu) << 24u);
	current_object = current_object + 1u;
	cell_value = init_video();
	OBJECT(current_object - 0x8u)->slot = 1u;
	OBJECT(current_object - 0x8u)->unk_09 = 1u;
	OBJECT(current_object - 0x8u)->unk_0A = 1u;
	OBJECT(current_object - 0x8u)->unk_0B = 1u;
	test_mode_memory_test_draw(cell_value);
	memory_start = 0x400000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_nibbles(failures, memory_start);
	sprite_lane_0_failures = failures & 0xFu;
	chip_result = 2u;
	if (sprite_lane_0_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x8u)->slot = chip_result;
	sprite_lane_1_failures = failures & 0xF0u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (sprite_lane_1_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x9u)->unk_09 = chip_result;
	sprite_lane_2_failures = failures & 0xF00u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (sprite_lane_2_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0xAu)->unk_0A = chip_result;
	sprite_lane_3_failures = failures & 0xF000u;
	failures = sprite_lane_3_failures;
	chip_result = 2u;
	current_object = current_object + 1u;
	OBJECT(current_object - 0xBu)->unk_0B = chip_result;
	current_object = current_object + 1u;
	init_sprite_lists();
	OBJECT(current_object - 0xCu)->state = 1u;
	OBJECT(current_object - 0xCu)->unk_0D = 1u;
	test_mode_memory_test_draw(failures);
	memory_start = 0x600000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0xCu)->state = chip_result;
	chip_result = 2u;
	current_object = current_object + 1u;
	if ((failures & 0xFF00u) != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0xDu)->unk_0D = chip_result;
	current_object = current_object + 1u;
	cell_value = test_mode_set_up_colours();
	OBJECT(current_object - 0xEu)->unk_0E = 1u;
	OBJECT(current_object - 0xEu)->unk_0F = 1u;
	test_mode_memory_test_draw(cell_value);
	memory_start = PALETTE_RAM + 0x4000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0xEu)->unk_0E = chip_result;
	chip_result = 2u;
	current_object = current_object + 1u;
	if ((failures & 0xFF00u) != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0xFu)->unk_0F = chip_result;
	current_object = current_object + 1u;
	cell_value = test_mode_set_up_colours();
	OBJECT(current_object - 0x10u)->unk_10 = 0x101u;
	test_mode_memory_test_draw(cell_value);
	memory_start = 0x680000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x10u)->unk_10 = (OBJECT(current_object - 0x10u)->unk_10 & 0xFF00u) | (chip_result & 0xFFu);
	chip_result = 2u;
	current_object = current_object + 1u;
	if ((failures & 0xFF00u) != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x11u)->unk_10 = (OBJECT(current_object - 0x11u)->unk_10 & 0xFFu) | ((chip_result & 0xFFu) << 8u);
	current_object = current_object + 1u;
	cell_value = test_mode_set_up_colours();
	OBJECT(current_object - 0x12u)->unk_12 = 1u;
	OBJECT(current_object - 0x12u)->unk_13 = 1u;
	test_mode_memory_test_draw(cell_value);
	memory_start = 0x684000u;
	failures = 0x4000u;
	failures = test_mode_memory_test_halfwords(failures, memory_start);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x12u)->unk_12 = chip_result;
	chip_result = 2u;
	current_object = current_object + 1u;
	if ((failures & 0xFF00u) != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x13u)->unk_13 = chip_result;
	current_object = current_object + 1u;
	cell_value = test_mode_set_up_colours();
	OBJECT(current_object - 0x14u)->unk_14 = 0x1010101u;
	test_mode_memory_test_draw(cell_value);
	move_halfwords(SPRITE_RAM, WORK_RAM, 0x10000u);
	memory_start = 0x200000u;
	failures = 0x20000u;
	failures = test_mode_memory_test_words(failures, memory_start);
	move_halfwords(WORK_RAM, SPRITE_RAM, 0x10000u);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x14u)->unk_14 = (OBJECT(current_object - 0x14u)->unk_14 & 0xFFFFFF00u) | (chip_result & 0xFFu);
	work_byte_1_failures = failures & 0xFF00u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (work_byte_1_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x15u)->unk_14 = (OBJECT(current_object - 0x15u)->unk_14 & 0xFFFF00FFu) | ((chip_result & 0xFFu) << 8u);
	work_byte_2_failures = failures & 0xFF0000u;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (work_byte_2_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x16u)->unk_14 = (OBJECT(current_object - 0x16u)->unk_14 & 0xFF00FFFFu) | ((chip_result & 0xFFu) << 16u);
	work_byte_3_failures = failures & ~0xFFFFFFu;
	failures = work_byte_3_failures;
	chip_result = 2u;
	current_object = current_object + 1u;
	if (work_byte_3_failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x17u)->unk_14 = (OBJECT(current_object - 0x17u)->unk_14 & 0xFFFFFFu) | ((chip_result & 0xFFu) << 24u);
	current_object = current_object + 1u;
	init_sprite_lists();
	OBJECT(current_object - 0x18u)->unk_18 = (OBJECT(current_object - 0x18u)->unk_18 & 0xFFFFFF00u) | 0x1u;
	test_mode_memory_test_draw(failures);
	move_halfwords(SPRITE_RAM, SOUND_RAM, 0x1000u);
	memory_start = 0x700000u;
	failures = 0x1FFFu;
	failures = test_mode_memory_test_bytes(failures, memory_start);
	move_halfwords(SOUND_RAM, SPRITE_RAM, 0x1000u);
	chip_result = 2u;
	if ((uint8_t)failures != 0u)
		chip_result = 3u;
	OBJECT(current_object - 0x18u)->unk_18 = (OBJECT(current_object - 0x18u)->unk_18 & 0xFFFFFF00u) | (chip_result & 0xFFu);
	current_object = current_object + 1u;
	init_sprite_lists();
	failures = test_mode_memory_test_draw(failures);
	mem_write8(SOUND_701FFF, 0u);
	interrupts_on();
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_memory_test_run_ADDRESS;
	test_mode_memory_test_run((uint16_t)failures);
}

/* The memory test's results, each frame: TEST goes back to the menu; PUSH TEST BUTTON TO EXIT blinks
   (test_mode_blink_prompt). */
void test_mode_memory_test_run(uint16_t character)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(character);
}

/* Draws the memory test's page (MEMORY TEST, 837-8890 <ROM> and its four chips, 837-8676 <RAM> and its
   21, NOW TESTING) and each chip's result from memory_test_results beside its name: ---- (not yet),
   TEST, GOOD or BAD. */
uint32_t test_mode_memory_test_draw(uint32_t cell_value)
{
	uint32_t cell_contents = 0, result_cells = 0, result_offset = 0, result_place = 0, result_text = 0, results_left = 0, text = 0;
	uint8_t colour = 0, character_0 = 0, character_1 = 0, character_2 = 0;
	uint16_t cell = 0;

	text = memory_test_page_text_ADDRESS;
	test_mode_draw_texts(cell_value, text);
	result_place = memory_test_results_ADDRESS;
	result_cells = memory_test_result_cells_ADDRESS;
	r12 = TEXT_CELL(0, 0);
	results_left = 0x19u;
	do
	{
		result_offset = mem_read8(result_place) * 3u;
		result_text = memory_test_result_texts_ADDRESS + result_offset * 2u;
		colour = MEMORY_TEST_RESULT_TEXT(result_text)->colour;
		result_text++;
		cell_contents = colour;
		cell = TEST_MODE_CELLS(result_cells)[0];
		result_cells += 2u;
		r12 = (r12 & ~0xFFFFu) | cell;
		character_0 = MEMORY_TEST_RESULT_TEXT(result_text - 0x1u)->character_0;
		result_text++;
		cell_contents = (uint16_t)(cell_contents << 8) | character_0;
		mem_write16(r12, cell_contents);
		r12 = r12 + 2u;
		character_1 = MEMORY_TEST_RESULT_TEXT(result_text - 0x2u)->character_1;
		result_text++;
		cell_contents = (cell_contents & ~0xFFu) | character_1;
		mem_write16(r12, cell_contents);
		r12 = r12 + 2u;
		character_2 = MEMORY_TEST_RESULT_TEXT(result_text - 0x3u)->character_2;
		cell_contents = (cell_contents & ~0xFFu) | character_2;
		mem_write16(r12, cell_contents);
		r12 = r12 + 2u;
		cell_contents = (cell_contents & ~0xFFu) | MEMORY_TEST_RESULT_TEXT(result_text - 0x3u)->character_3;
		mem_write16(r12, cell_contents);
		r12 = r12 + 2u;
		results_left--;
		result_place++;
	} while (results_left != 0u);
	return cell_contents;
}

/* Sums a stretch of the program ROM as two 16-bit checks, one of the even halfwords (the chip of the low
   half of each 32-bit word: IC37 or IC36) and one of the odd (IC40 or IC39): each halfword is xored in,
   the sum shifted right by one and xored with 8810 when a 1 falls out (a CRC); both start at 1. */
void BOARD_ORIGINAL(test_mode_memory_test_checksum)(uint32_t memory_start, uint32_t *p_byte_count, uint32_t *p_odd_check)
{
	uint32_t byte_count = *p_byte_count;
	uint32_t carry = 0;
	uint8_t shift = 0;
	uint16_t even_sum = 0, odd_sum = 0, polynomial = 0;
	uint32_t pairs_left = 0;

	pairs_left = byte_count >> 2;
	even_sum = 1u;
	odd_sum = 1u;
	shift = 0xFFu;
	polynomial = 0x8810u;
	do
	{
		even_sum ^= mem_read16(memory_start);
		carry = v60_shl_cy(even_sum, shift, 16);
		even_sum = shift_by(even_sum, shift, 16);
		memory_start += 2u;
		if (carry)
			even_sum ^= polynomial;
		odd_sum ^= mem_read16(memory_start);
		carry = v60_shl_cy(odd_sum, shift, 16);
		odd_sum = shift_by(odd_sum, shift, 16);
		memory_start += 2u;
		if (carry)
			odd_sum ^= polynomial;
		pairs_left--;
	} while (pairs_left != 0u);
	*p_byte_count = (*p_byte_count & 0xFFFF0000u) | even_sum;
	*p_odd_check = (*p_odd_check & 0xFFFF0000u) | odd_sum;
}

/* Tests RAM 32 bits at a time (the work RAM, in place: it takes its return address off the stack into a
   register and jumps back through it, so that it needs no stack in the RAM it overwrites): 8 passes,
   each writing a walking pattern (01010101 the first pass, one bit further each pass, turned by a few
   bits from word to word by the word's address) and reading it back; returns the bits found wrong (each
   mismatch's expected ^ read, xored together), a byte per chip; NOW TESTING blinks once a pass
   (test_mode_memory_test_blink). */
uint32_t test_mode_memory_test_words(uint32_t byte_count, uint32_t memory_start)
{
	uint32_t carry = 0;
	uint8_t rotation = 0;
	uint32_t address_bits = 0, expected = 0, failures = 0, pattern = 0, word_count = 0, words_left = 0;
	uint32_t low_17_bits = 0, low_14_bits = 0, low_11_bits = 0, low_8_bits = 0, found = 0, read_low_17_bits = 0, read_low_14_bits = 0, read_low_11_bits = 0, read_low_8_bits = 0;

	word_count = byte_count >> 2;
	pattern = 0x1010101u;
	failures = 0u;
	do
	{
		r12 = memory_start;
		expected = pattern;
		words_left = word_count;
		do
		{
			mem_write32(r12, expected);
			r12 = r12 + 4u;
			rotation = 6u;
			low_17_bits = r12 << 15;
			address_bits = low_17_bits;
			if (low_17_bits != 0u)
			{
				rotation--;
				low_14_bits = address_bits << 3;
				address_bits = low_14_bits;
				if (low_14_bits != 0u)
				{
					rotation--;
					low_11_bits = address_bits << 3;
					address_bits = low_11_bits;
					if (low_11_bits != 0u)
					{
						rotation--;
						low_8_bits = address_bits << 3;
						address_bits = low_8_bits;
						if (low_8_bits != 0u)
						{
							rotation--;
							if (address_bits << 3 != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 32);
			words_left--;
		} while (words_left != 0u);
		r12 = memory_start;
		expected = pattern;
		words_left = word_count;
		do
		{
			found = mem_read32(r12);
			r12 = r12 + 4u;
			if (found != expected)
				failures = failures ^ expected ^ mem_read32(r12 - 4u);
			rotation = 6u;
			read_low_17_bits = r12 << 15;
			address_bits = read_low_17_bits;
			if (read_low_17_bits != 0u)
			{
				rotation--;
				read_low_14_bits = address_bits << 3;
				address_bits = read_low_14_bits;
				if (read_low_14_bits != 0u)
				{
					rotation--;
					read_low_11_bits = address_bits << 3;
					address_bits = read_low_11_bits;
					if (read_low_11_bits != 0u)
					{
						rotation--;
						read_low_8_bits = address_bits << 3;
						address_bits = read_low_8_bits;
						if (read_low_8_bits != 0u)
						{
							rotation--;
							if (address_bits << 3 != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 32);
			words_left--;
		} while (words_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pattern, 1u, 32);
		pattern = (pattern << 1) | (pattern >> 31);
	} while (carry == 0u);
	return failures;
}

/* test_mode_memory_test_words 16 bits at a time (the palette RAMs: a chip for each byte), with the
   pattern 0101; returns the bits found wrong. */
uint32_t test_mode_memory_test_halfwords(uint32_t failures, uint32_t memory_start)
{
	uint32_t carry = 0;
	uint8_t rotation = 0;
	uint16_t address_bits = 0, expected = 0, pattern = 0;
	uint32_t halfword_count = 0, halfwords_left = 0;
	uint16_t low_13_bits = 0, low_10_bits = 0, low_7_bits = 0, found = 0, read_low_13_bits = 0, read_low_10_bits = 0, read_low_7_bits = 0;

	halfword_count = failures >> 1;
	pattern = 0x101u;
	failures &= ~0xFFFFu;
	do
	{
		r12 = memory_start;
		expected = (uint16_t)pattern;
		halfwords_left = halfword_count;
		do
		{
			mem_write16(r12, expected);
			r12 = r12 + 2u;
			address_bits = (uint16_t)r12;
			rotation = 6u;
			if ((uint16_t)address_bits != 0u)
			{
				rotation--;
				low_13_bits = (uint32_t)address_bits << 3;
				address_bits = low_13_bits;
				if (low_13_bits != 0u)
				{
					rotation--;
					low_10_bits = (uint32_t)address_bits << 3;
					address_bits = low_10_bits;
					if (low_10_bits != 0u)
					{
						rotation--;
						low_7_bits = (uint32_t)address_bits << 3;
						address_bits = low_7_bits;
						if (low_7_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		r12 = memory_start;
		expected = (uint16_t)pattern;
		halfwords_left = halfword_count;
		do
		{
			found = mem_read16(r12);
			r12 = r12 + 2u;
			if (found != expected)
			{
				failures = (failures & ~0xFFFFu) | (uint16_t)(failures ^ expected);
				failures = (failures & ~0xFFFFu) | (uint16_t)(failures ^ mem_read16(r12 - 2u));
			}
			address_bits = (uint16_t)r12;
			rotation = 6u;
			if ((uint16_t)address_bits != 0u)
			{
				rotation--;
				read_low_13_bits = (uint32_t)address_bits << 3;
				address_bits = read_low_13_bits;
				if (read_low_13_bits != 0u)
				{
					rotation--;
					read_low_10_bits = (uint32_t)address_bits << 3;
					address_bits = read_low_10_bits;
					if (read_low_10_bits != 0u)
					{
						rotation--;
						read_low_7_bits = (uint32_t)address_bits << 3;
						address_bits = read_low_7_bits;
						if (read_low_7_bits != 0u)
						{
							rotation--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								rotation--;
						}
					}
				}
			}
			expected = rotate_by(expected, rotation, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pattern, 1u, 16);
		pattern = (uint16_t)(((uint32_t)pattern << 1) | (pattern >> 15));
	} while (carry == 0u);
	return failures;
}

/* test_mode_memory_test_words a byte at a time (the sound RAM, all but its last byte, which holds the
   sound program), with the pattern 01; returns the bits found wrong. */
uint32_t test_mode_memory_test_bytes(uint32_t size_then_wrong_bits, uint32_t ram_start)
{
	uint32_t carry = 0;
	uint8_t pass_pattern = 0, pattern = 0, turn = 0;
	uint16_t address_bits = 0;
	uint32_t byte_count = 0, bytes_left = 0;
	uint8_t read_byte = 0;
	uint16_t address_low_15 = 0, address_low_12 = 0, address_low_9 = 0, address_low_6 = 0, checked_address_low_15 = 0, checked_address_low_12 = 0, checked_address_low_9 = 0, checked_address_low_6 = 0;

	byte_count = size_then_wrong_bits;
	pass_pattern = 1u;
	size_then_wrong_bits &= ~0xFFu;
	do
	{
		r12 = ram_start;
		pattern = (uint8_t)pass_pattern;
		bytes_left = byte_count;
		do
		{
			mem_write8(r12, pattern);
			r12 = r12 + 1u;
			turn = 6u;
			address_low_15 = r12 * 2u;
			address_bits = address_low_15;
			if (address_low_15 != 0u)
			{
				turn--;
				address_low_12 = (uint32_t)address_bits << 3;
				address_bits = address_low_12;
				if (address_low_12 != 0u)
				{
					turn--;
					address_low_9 = (uint32_t)address_bits << 3;
					address_bits = address_low_9;
					if (address_low_9 != 0u)
					{
						turn--;
						address_low_6 = (uint32_t)address_bits << 3;
						address_bits = address_low_6;
						if (address_low_6 != 0u)
						{
							turn--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								turn--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		r12 = ram_start;
		pattern = (uint8_t)pass_pattern;
		bytes_left = byte_count;
		do
		{
			read_byte = mem_read8(r12);
			r12 = r12 + 1u;
			if (read_byte != pattern)
			{
				size_then_wrong_bits = (size_then_wrong_bits & ~0xFFu) | (uint8_t)(size_then_wrong_bits ^ pattern);
				size_then_wrong_bits = (size_then_wrong_bits & ~0xFFu) | (uint8_t)(size_then_wrong_bits ^ mem_read8(r12 - 1u));
			}
			turn = 6u;
			checked_address_low_15 = r12 * 2u;
			address_bits = checked_address_low_15;
			if (checked_address_low_15 != 0u)
			{
				turn--;
				checked_address_low_12 = (uint32_t)address_bits << 3;
				address_bits = checked_address_low_12;
				if (checked_address_low_12 != 0u)
				{
					turn--;
					checked_address_low_9 = (uint32_t)address_bits << 3;
					address_bits = checked_address_low_9;
					if (checked_address_low_9 != 0u)
					{
						turn--;
						checked_address_low_6 = (uint32_t)address_bits << 3;
						address_bits = checked_address_low_6;
						if (checked_address_low_6 != 0u)
						{
							turn--;
							if ((uint16_t)((uint32_t)address_bits << 3) != 0u)
								turn--;
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 8);
			bytes_left--;
		} while (bytes_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pass_pattern, 1u, 8);
		pass_pattern = (uint8_t)(((uint32_t)pass_pattern << 1) | (pass_pattern >> 7));
	} while (carry == 0u);
	return size_then_wrong_bits;
}

/* test_mode_memory_test_words for RAM of 4-bit chips, 16 bits at a time (the video RAM and the sprite
   RAM): 4 passes with the pattern 1111 (a bit per chip); returns the bits found wrong, a nibble per
   chip. */
uint32_t test_mode_memory_test_nibbles(uint32_t size_then_wrong_bits, uint32_t ram_start)
{
	uint32_t carry = 0;
	uint8_t turn = 0;
	uint16_t pass_pattern = 0, pattern = 0;
	uint32_t address_bits = 0, halfword_count = 0, halfwords_left = 0;
	uint16_t read_halfword = 0;
	uint32_t address_low_17 = 0, address_low_15 = 0, address_low_13 = 0, address_low_11 = 0, address_low_9 = 0, address_low_7 = 0, address_low_5 = 0, checked_address_low_17 = 0, checked_address_low_15 = 0, checked_address_low_13 = 0, checked_address_low_11 = 0, checked_address_low_9 = 0, checked_address_low_7 = 0, checked_address_low_5 = 0;

	halfword_count = size_then_wrong_bits >> 1;
	pass_pattern = 0x1111u;
	size_then_wrong_bits &= ~0xFFFFu;
	do
	{
		r12 = ram_start;
		pattern = (uint16_t)pass_pattern;
		halfwords_left = halfword_count;
		do
		{
			mem_write16(r12, pattern);
			r12 = r12 + 2u;
			turn = 9u;
			address_low_17 = r12 << 15;
			address_bits = address_low_17;
			if (address_low_17 != 0u)
			{
				turn--;
				address_low_15 = address_bits * 2u * 2u;
				address_bits = address_low_15;
				if (address_low_15 != 0u)
				{
					turn--;
					address_low_13 = address_bits * 2u * 2u;
					address_bits = address_low_13;
					if (address_low_13 != 0u)
					{
						turn--;
						address_low_11 = address_bits * 2u * 2u;
						address_bits = address_low_11;
						if (address_low_11 != 0u)
						{
							turn--;
							address_low_9 = address_bits * 2u * 2u;
							address_bits = address_low_9;
							if (address_low_9 != 0u)
							{
								turn--;
								address_low_7 = address_bits * 2u * 2u;
								address_bits = address_low_7;
								if (address_low_7 != 0u)
								{
									turn--;
									address_low_5 = address_bits * 2u * 2u;
									address_bits = address_low_5;
									if (address_low_5 != 0u)
									{
										turn--;
										if (address_bits * 2u * 2u != 0u)
											turn--;
									}
								}
							}
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		r12 = ram_start;
		pattern = (uint16_t)pass_pattern;
		halfwords_left = halfword_count;
		do
		{
			read_halfword = mem_read16(r12);
			r12 = r12 + 2u;
			if (read_halfword != pattern)
			{
				size_then_wrong_bits = (size_then_wrong_bits & ~0xFFFFu) | (uint16_t)(size_then_wrong_bits ^ pattern);
				size_then_wrong_bits = (size_then_wrong_bits & ~0xFFFFu) | (uint16_t)(size_then_wrong_bits ^ mem_read16(r12 - 2u));
			}
			turn = 9u;
			checked_address_low_17 = r12 << 15;
			address_bits = checked_address_low_17;
			if (checked_address_low_17 != 0u)
			{
				turn--;
				checked_address_low_15 = address_bits * 2u * 2u;
				address_bits = checked_address_low_15;
				if (checked_address_low_15 != 0u)
				{
					turn--;
					checked_address_low_13 = address_bits * 2u * 2u;
					address_bits = checked_address_low_13;
					if (checked_address_low_13 != 0u)
					{
						turn--;
						checked_address_low_11 = address_bits * 2u * 2u;
						address_bits = checked_address_low_11;
						if (checked_address_low_11 != 0u)
						{
							turn--;
							checked_address_low_9 = address_bits * 2u * 2u;
							address_bits = checked_address_low_9;
							if (checked_address_low_9 != 0u)
							{
								turn--;
								checked_address_low_7 = address_bits * 2u * 2u;
								address_bits = checked_address_low_7;
								if (checked_address_low_7 != 0u)
								{
									turn--;
									checked_address_low_5 = address_bits * 2u * 2u;
									address_bits = checked_address_low_5;
									if (checked_address_low_5 != 0u)
									{
										turn--;
										if (address_bits * 2u * 2u != 0u)
											turn--;
									}
								}
							}
						}
					}
				}
			}
			pattern = rotate_by(pattern, turn, 16);
			halfwords_left--;
		} while (halfwords_left != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(pass_pattern, 1u, 16);
		pass_pattern = (uint16_t)(((uint32_t)pass_pattern << 1) | (pass_pattern >> 15));
	} while (carry == 0u);
	return size_then_wrong_bits;
}

/* Shows or hides NOW TESTING (each of its cells xored with its character in palette 66): the RAM tests
   call it after each pass. */
void test_mode_memory_test_blink(void)
{
	uint16_t character = 0;
	uint32_t cell = 0;

	character = 0x6600u;
	r27 = TEXT_CELL(14, 25);
	r28 = memory_test_now_testing_text_ADDRESS;
	do
	{
		character = (uint16_t)((character & ~0xFFu) | TEST_MODE_CHARACTERS(r28)[0]);
		cell = r27;
		r27 = r27 + 2u;
		mem_write16(cell, mem_read16(cell) ^ character);
		r28 = r28 + 1u;
	} while (TEST_MODE_CHARACTERS(r28)[0] != 0u);
}

/* Starts the VOLUME ADJUSTMENT, the calibration of the wheels' and pedals' potentiometers: draws ADJUST
   and CANCEL with the cursor on CANCEL (test_mode_page_cursor 1), the six controls' names (MONITOR A and
   B: STEERING WHEEL, ACCELERATOR, BRAKE) and the prompt; then test_mode_volume_adjustment_select. */
void test_mode_volume_adjustment(void)
{
	uint32_t cell = 0, character = 0, text = 0;

	character = test_mode_clear_text();
	text = volume_adjustment_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	text = volume_adjustment_controls_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 1u;
	cell = 0x310000u | volume_adjustment_cancel_cell;
	character = test_mode_set_cursor(character, cell);
	test_mode_routine = test_mode_volume_adjustment_select_ADDRESS;
	test_mode_volume_adjustment_select(character);
}

/* Each frame: the bars (test_mode_volume_adjustment_draw); SERVICE moves the cursor between ADJUST and
   CANCEL; TEST on CANCEL goes back to the menu; TEST on ADJUST starts the adjustment: ADJUSTING, PUSH
   TEST BUTTON TO EXIT, each control's range (volume_ranges) narrowed to where it is now, then
   test_mode_volume_adjustment_adjust. */
void test_mode_volume_adjustment_select(uint32_t character)
{
	uint8_t position = 0;
	uint32_t analog_input = 0, cell = 0, controls_left = 0, drawn_character = 0, page_cursor = 0, range = 0, text = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	test_mode_volume_adjustment_draw(character);
	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor ^ 1u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | volume_adjustment_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		blink_timer = test_mode_blink_timer - 1u;
		test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			test_mode_blink_timer = 5;
			blink_cell = test_mode_blink_address;
			cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
			mem_write16(blink_cell, cursor_character);
			if (cursor_character != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	if (test_mode_page_cursor == 0u)
	{
		drawn_character = test_mode_clear_text();
		text = volume_adjusting_text_ADDRESS;
		drawn_character = test_mode_draw_texts(drawn_character, text);
		text = volume_adjustment_controls_text_ADDRESS;
		test_mode_draw_texts(drawn_character, text);
		test_mode_show_exit_prompt();
		range = volume_ranges_ADDRESS;
		analog_input = test_mode_analog_inputs_ADDRESS;
		controls_left = 6u;
		do
		{
			position = (uint8_t)(mem_read8(analog_input) & ~1u);
			mem_write8(range, position);
			range++;
			mem_write8(range, position | 1u);
			controls_left--;
			analog_input++;
			range++;
		} while (controls_left != 0u);
		test_mode_routine = test_mode_volume_adjustment_adjust_ADDRESS;
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* The adjustment, each frame: each control's range (volume_ranges) widens to take its position
   (test_mode_analog_inputs), as the operator turns the wheels and presses the pedals all the way; the
   bars; TEST marks the settings changed (test_mode_flags bit 1) and goes back to the menu; PUSH TEST
   BUTTON TO EXIT blinks. */
void test_mode_volume_adjustment_adjust(void)
{
	uint32_t analog_input = 0, controls_left = 0, position = 0, range = 0;

	analog_input = test_mode_analog_inputs_ADDRESS;
	range = volume_ranges_ADDRESS;
	controls_left = 6u;
	do
	{
		position = mem_read8(analog_input);
		analog_input++;
		if (position < mem_read8(range))
			mem_write8(range, position);
		range++;
		if (mem_read8(range) < position)
			mem_write8(range, position);
		range++;
		controls_left--;
	} while (controls_left != 0u);
	position = test_mode_volume_adjustment_draw(position);
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		test_mode_flags |= 2u;
		test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt(position);
}

/* Draws the six controls' bars (32 cells, a cell for 8 steps): the range of volume_ranges in green and
   the control's position (test_mode_analog_inputs) in red. */
uint32_t test_mode_volume_adjustment_draw(uint32_t character)
{
	uint16_t range_character = 0;
	uint32_t bar_cells = 0, cell = 0, controls_left = 0, position_step = 0, range_length = 0, range_start = 0, text = 0;
	uint16_t range_cell_a = 0, range_cell_b = 0, position_cell = 0;

	text = volume_adjustment_bars_text_ADDRESS;
	test_mode_draw_texts(character, text);
	cell = TEXT_CELL(0, 0);
	bar_cells = volume_adjustment_bar_cells_ADDRESS;
	r12 = volume_ranges_ADDRESS;
	controls_left = 3u;
	do
	{
		range_start = mem_read8(r12);
		range_start = range_start >> 3;
		r12 = r12 + 1u;
		range_length = mem_read8(r12);
		range_length = range_length >> 3;
		range_length = (uint8_t)(range_length - range_start);
		range_length = (uint8_t)(range_length + 1u);
		range_cell_a = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = ((cell & ~0xFFFFu) | range_cell_a) + range_start * 2u;
		range_character = 0x6403u;
		r12 = r12 + 1u;
		do
		{
			mem_write16(cell, range_character);
			range_length--;
			cell += 2u;
		} while (range_length != 0u);
		controls_left--;
	} while (controls_left != 0u);
	controls_left = 3u;
	do
	{
		range_start = mem_read8(r12);
		range_start = range_start >> 3;
		r12 = r12 + 1u;
		range_length = mem_read8(r12);
		range_length = range_length >> 3;
		range_length = (uint8_t)(range_length - range_start);
		range_length = (uint8_t)(range_length + 1u);
		range_cell_b = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = ((cell & ~0xFFFFu) | range_cell_b) + range_start * 2u;
		range_character = 0x6403u;
		r12 = r12 + 1u;
		do
		{
			mem_write16(cell, range_character);
			range_length--;
			cell += 2u;
		} while (range_length != 0u);
		controls_left--;
	} while (controls_left != 0u);
	bar_cells = volume_adjustment_bar_cells_ADDRESS;
	r12 = test_mode_analog_inputs_ADDRESS;
	controls_left = 6u;
	do
	{
		position_step = mem_read8(r12) >> 3;
		position_cell = TEST_MODE_CELLS(bar_cells)[0];
		bar_cells += 2u;
		cell = (cell & ~0xFFFFu) | position_cell;
		mem_write16(cell + position_step * 2u, 0x6203u);
		controls_left--;
		r12 = r12 + 1u;
	} while (controls_left != 0u);
	return position_step;
}

/* Starts the INPUT TEST: clears the text layer, draws the inputs' names and the blinking PUSH TEST
   BUTTON TO EXIT, then test_mode_input_test_run. */
void test_mode_input_test(void)
{
	uint32_t character = 0, text = 0;

	character = test_mode_clear_text();
	text = input_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_input_test_run_ADDRESS;
	test_mode_input_test_run(character);
}

/* The input test, each frame: the inputs (test_mode_input_test_draw); TEST goes on to
   test_mode_input_test_release, so that the press shows as TEST On before the page ends; PUSH TEST
   BUTTON TO EXIT blinks (test_mode_blink_prompt). */
void test_mode_input_test_run(uint32_t character)
{
	character = test_mode_input_test_draw(character);
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		test_mode_blink_prompt(character);
		return;
	}
	test_mode_routine = test_mode_input_test_release_ADDRESS;
	test_mode_blink_prompt(character);
}

/* The input test once TEST is pressed: the inputs each frame until TEST is let go, then back to the
   menu. */
void test_mode_input_test_release(void)
{
	uint32_t test_switches = 0;

	test_switches = test_mode_input_test_draw(0u);
	if ((test_mode_buttons_released & 2u) == 0u)
	{
		test_mode_blink_prompt(test_switches);
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(test_switches);
}

/* Draws the inputs of both monitors (cabinets): the wheel, accelerator and brake as the game sees them
   through the calibration (test_mode_analog_inputs clamped to volume_ranges and scaled to 0-FF), as a
   mark on a bar of 16 cells and a hexadecimal number; SHIFT UP, SHIFT DOWN, START, DJ/MUSIC, <<, >>,
   COIN and SERVICE On or Off, read from each I/O chip's ports (active low); TEST On when either
   monitor's is pressed. */
uint32_t test_mode_input_test_draw(uint32_t character)
{
	uint32_t carry = 0;
	uint8_t colour = 0, test_switches_b = 0;
	uint16_t range_high = 0, range_low = 0;
	uint32_t bit = 0, cell = 0, controls_left = 0, digit_count = 0, item = 0, port_b = 0, position = 0, range = 0, test_switches = 0, text = 0;

	text = input_test_bars_text_ADDRESS;
	test_mode_draw_texts(character, text);
	cell = TEXT_CELL(0, 0);
	item = input_test_items_ADDRESS;
	r12 = test_mode_analog_inputs_ADDRESS;
	range = volume_ranges_ADDRESS;
	r14 = input_test_bar_cells_ADDRESS;
	controls_left = 6u;
	do
	{
		position = mem_read8(r12);
		range_low = mem_read8(range);
		r12 = r12 + 1u;
		range++;
		if (position < range_low)
			position = range_low;
		range_high = mem_read8(range);
		range++;
		if (range_high < position)
			position = range_high;
		position = (uint8_t)(position - range_low);
		range_high = (uint8_t)(range_high - range_low);
		range_low = (uint16_t)position;
		position = (uint16_t)(position << 8);
		position = (uint16_t)(position - range_low);
		position = divide_u16(position, range_high);
		cell = (cell & ~0xFFFFu) | TEST_MODE_CELLS(r14)[0];
		mem_write16(cell + ((uint8_t)position >> 4) * 2u, 0x6403u);
		colour = 0x64u;
		digit_count = 2u;
		cell = (cell & ~0xFFFFu) | ROM16(item);
		r14 = r14 + 2u;
		item += 2u;
		cell = test_mode_draw_hex_number(position, colour, digit_count, cell);
		controls_left--;
	} while (controls_left != 0u);
	r12 = 0xC00000u;
	port_b = 0xC80000u;
	do
	{
		r12 = (r12 & ~0xFFu) | ROM8(item);
		item++;
		bit = ROM8(item);
		carry = (mem_read32(r12) & shl32(1u, (int)(bit))) != 0u;
		item++;
		cell = (cell & ~0xFFFFu) | ROM16(item);
		mem_write32(cell, input_test_on_off_cells[carry]);
		port_b = (port_b & ~0xFFu) | (uint8_t)r12;
		carry = (mem_read32(port_b) & shl32(1u, (int)(bit))) != 0u;
		item += 2u;
		cell = (cell & ~0xFFFFu) | ROM16(item);
		mem_write32(cell, input_test_on_off_cells[carry]);
		item += 2u;
	} while ((int16_t)ROM16(item) >= 0);
	r12 = (r12 & ~0xFFu) | 8u;
	port_b = (port_b & ~0xFFu) | (uint8_t)r12;
	test_switches = mem_read8(r12);
	r12 = (r12 & ~0xFFu) | 0xAu;
	test_switches = test_switches & (mem_read8(r12) >> 4);
	test_switches_b = mem_read8(port_b) & (mem_read8((port_b & ~0xFFu) | (uint8_t)r12) >> 4);
	carry = (test_switches & 1u) != 0u;
	item += 2u;
	cell = (cell & ~0xFFFFu) | ROM16(item);
	mem_write32(cell, input_test_on_off_cells[carry]);
	carry = (test_switches_b & 1u) != 0u;
	item += 2u;
	cell = (cell & ~0xFFFFu) | ROM16(item);
	mem_write32(cell, input_test_on_off_cells[carry]);
	test_switches = test_switches & test_switches_b;
	carry = (test_switches & 2u) != 0u;
	mem_write32((cell & ~0xFFFFu) | ROM16(item + 2u), input_test_on_off_cells[carry]);
	return test_switches;
}

/* Starts the OUTPUT TEST: the ten outputs of the two monitors (cabinets) with their state
   (test_mode_output_test_draw), EXIT and the prompt, the cursor on EXIT (test_mode_page_cursor 10); then
   test_mode_output_test_run. */
void test_mode_output_test(void)
{
	uint32_t cell = 0, character = 0, text = 0;

	character = test_mode_clear_text();
	text = output_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 0xAu;
	cell = 0x310000u | output_test_exit_cell;
	test_mode_set_cursor(character, cell);
	test_mode_output_test_draw();
	test_mode_routine = test_mode_output_test_run_ADDRESS;
	test_mode_output_test_run();
}

/* The output test, each frame: TEST turns the output under the cursor on and letting go of it turns it
   off again (test_mode_output_test_toggle both times), so it is on while TEST is held; TEST on EXIT runs
   test_mode_output_test_exit; SERVICE, while TEST is not held, moves the cursor through the eleven
   items; the arrow blinks. */
void test_mode_output_test_run(void)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		switch (test_mode_page_cursor)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			test_mode_output_test_toggle();
			break;
		case 10:
			test_mode_output_test_exit();
			break;
		default:
			invalid_jump(test_mode_output_test_item_jumps[test_mode_page_cursor]);
		}
		test_mode_output_test_draw();
	}
	if ((test_mode_buttons_released & 2u) != 0u)
	{
		page_cursor = test_mode_page_cursor;
		if (page_cursor != 0xAu)
		{
			switch (page_cursor)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				test_mode_output_test_toggle();
				break;
			case 10:
				test_mode_output_test_exit();
				break;
			default:
				invalid_jump(test_mode_output_test_item_jumps[page_cursor]);
			}
			test_mode_output_test_draw();
		}
	}
	if ((test_mode_buttons_pressed & 1u) == 0u || (test_mode_buttons & 2u) != 0u)
	{
		blink_timer = test_mode_blink_timer - 1u;
		test_mode_blink_timer = blink_timer;
		if ((int8_t)blink_timer < 0)
		{
			test_mode_blink_timer = 5;
			blink_cell = test_mode_blink_address;
			cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
			mem_write16(blink_cell, cursor_character);
			if (cursor_character != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	test_mode_clear_cursor();
	page_cursor = test_mode_page_cursor;
	page_cursor = (uint8_t)(page_cursor + 1u);
	if ((int8_t)page_cursor >= 0xB)
		page_cursor = 0u;
	test_mode_page_cursor = page_cursor;
	cell = 0x310000u | output_test_cursor_cells[page_cursor];
	test_mode_set_cursor(page_cursor, cell);
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws On or Off for the ten outputs, from their bits in the game's output bytes: monitor A's CHECK
   POINT and RACE LEADER lamps (port_d_outputs bits 1 and 3), the steering wheel's motor (port_d_outputs
   bit 4), the DJ/MUSIC and << >> lamps (port_g_outputs bits 0 and 1); monitor B's the same bits of
   20E093 and 20E095. */
void test_mode_output_test_draw(void)
{
	uint32_t carry = 0;
	uint32_t cell = 0, output = 0;

	cell = TEXT_CELL(0, 0);
	output = output_test_outputs_ADDRESS;
	do
	{
		cell = (cell & ~0xFFFFu) | OUTPUT_TEST_OUTPUT(output)->cell;
		output += 2u;
		r12 = OUTPUT_TEST_OUTPUT(output - 0x2u)->variable;
		output++;
		r12 = r12 + GAME_VARIABLES;
		carry = (mem_read32(r12) & shl32(1u, (int)(OUTPUT_TEST_OUTPUT(output - 0x3u)->bit))) != 0u;
		mem_write32(cell, output_test_on_off_cells[carry]);
		output++;
	} while (OUTPUT_TEST_OUTPUT(output)->cell != 0u);
}

/* Turns the output under the cursor (test_mode_page_cursor) on or off: flips its bit in the game's
   output bytes (the list at 01D5C0). */
void test_mode_output_test_toggle(void)
{
	uint32_t output = 0, output_variable = 0;

	output = output_test_outputs_ADDRESS + 0x2u + test_mode_page_cursor * 4u;
	output_variable = OUTPUT_TEST_OUTPUT(output - 0x2u)->variable + GAME_VARIABLES;
	mem_write32(output_variable, mem_read32(output_variable) ^ shl32(1u, (int)(OUTPUT_TEST_OUTPUT(output - 0x2u)->bit)));
}

/* The output test's EXIT: turns the lamps and the wheel motor off (port_d_outputs &= E0E1,
   port_g_outputs &= FBFC: monitor B's DJ/MUSIC and << >> bits, 20E095 bits 0-1, are not among them) and
   goes back to the menu. */
void test_mode_output_test_exit(void)
{
	uint32_t port_d_outputs_0_halfword = 0, port_g_outputs_0_halfword = 0;

	port_d_outputs_0_halfword = (port_d_outputs[0] | ((uint32_t)port_d_outputs[1] << 8u)) & 0xE0E1u;
	port_d_outputs[0] = (uint8_t)port_d_outputs_0_halfword;
	port_d_outputs[1] = (uint8_t)(port_d_outputs_0_halfword >> 8u);
	port_g_outputs_0_halfword = (port_g_outputs[0] | ((uint32_t)port_g_outputs[1] << 8u)) & ~0x403u;
	port_g_outputs[0] = (uint8_t)port_g_outputs_0_halfword;
	port_g_outputs[1] = (uint8_t)(port_g_outputs_0_halfword >> 8u);
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the SOUND TEST: MONITOR A No., MONITOR B No. (the numbers of the next sounds, both 0:
   sound_test_number_a, sound_test_number_b), EXIT with the cursor on it (test_mode_page_cursor 2) and
   the prompt; then test_mode_sound_test_run. */
void test_mode_sound_test(void)
{
	uint32_t cell = 0, character = 0, text = 0;

	character = test_mode_clear_text();
	text = sound_test_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 2u;
	cell = 0x310000u | sound_test_exit_cell;
	test_mode_set_cursor(character, cell);
	sound_test_number_a = 0u;
	sound_test_number_b = 0u;
	test_mode_sound_test_draw();
	test_mode_routine = test_mode_sound_test_run_ADDRESS;
	test_mode_sound_test_run();
}

/* The sound test, each frame: SERVICE moves the cursor through the three items; TEST stops both
   monitors' sounds (command 1F to each), then plays monitor A's next sound
   (test_mode_sound_test_play_a), monitor B's (test_mode_sound_test_play_b) or goes back to the menu
   (test_mode_sound_test_exit), and redraws the numbers; the arrow blinks. */
void test_mode_sound_test_run(void)
{
	uint16_t stop_request = 0;
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		if ((int8_t)page_cursor >= 3)
			page_cursor = 0u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | sound_test_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		stop_request = 0x1Fu;
		test_mode_queue_sound(stop_request);
		test_mode_queue_sound_screen_2(stop_request);
		switch (test_mode_page_cursor)
		{
		case 0:
			test_mode_sound_test_play_a();
			break;
		case 1:
			test_mode_sound_test_play_b();
			break;
		case 2:
			test_mode_sound_test_exit();
			break;
		default:
			invalid_jump(test_mode_sound_test_item_jumps[test_mode_page_cursor]);
		}
		test_mode_sound_test_draw();
	}
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the two monitors' sound numbers (sound_test_number_a, sound_test_number_b), 3 digits each. */
void test_mode_sound_test_draw(void)
{
	uint32_t cell = 0, colour = 0, digit_count = 0, number = 0;

	cell = TEXT_CELL(28, 11);
	number = sound_test_number_a;
	colour = 0x64u;
	digit_count = 3u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
	cell = (cell & ~0xFFFFu) | 0xF6B8u;
	number = sound_test_number_b;
	colour = 0x64u;
	digit_count = 3u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
}

/* Plays monitor A's sound number sound_test_number_a from the list of 121 sound codes at 01D7C6
   (test_mode_queue_sound) and steps to the next; from number 40 on it first sends command D6 (DA from
   66, D8 from 6B), which selects sound set 6 (A, 8): the music parts' templates and the screen's
   MultiPCM sample bank; past the list's end (its closing 0) the number goes back to 0 without a sound. */
void test_mode_sound_test_play_a(void)
{
	uint16_t sound_set_request = 0;
	uint32_t sound_code = 0, sound_number = 0;
	uint32_t saved_sound_number = 0;

	sound_number = sound_test_number_a;
	if (sound_number < 0x40u)
	{
		sound_code = mem_read8(sound_test_codes_ADDRESS + sound_number);
		if (sound_code == 0u)
		{
			sound_test_number_a = 0u;
			return;
		}
		sound_test_number_a = sound_number + 1u;
		test_mode_queue_sound((uint16_t)sound_code);
		return;
	}
	sound_set_request = 0xD6u;
	if (sound_number >= 0x66u)
	{
		sound_set_request = 0xDAu;
		if (sound_number >= 0x6Bu)
			sound_set_request = 0xD8u;
	}
	saved_sound_number = sound_number;
	test_mode_queue_sound(sound_set_request);
	sound_number = saved_sound_number;
	sound_code = mem_read8(sound_test_codes_ADDRESS + sound_number);
	if (sound_code == 0u)
	{
		sound_test_number_a = 0u;
		return;
	}
	sound_test_number_a = sound_number + 1u;
	test_mode_queue_sound((uint16_t)sound_code);
}

/* test_mode_sound_test_play_a for monitor B: sound_test_number_b, sent through
   test_mode_queue_sound_screen_2. */
void test_mode_sound_test_play_b(void)
{
	uint16_t sound_set_request = 0;
	uint32_t sound_code = 0, sound_number = 0;
	uint32_t saved_sound_number = 0;

	sound_number = sound_test_number_b;
	if (sound_number < 0x40u)
	{
		sound_code = mem_read8(sound_test_codes_ADDRESS + sound_number);
		if (sound_code == 0u)
		{
			sound_test_number_b = 0u;
			return;
		}
		sound_test_number_b = sound_number + 1u;
		test_mode_queue_sound_screen_2((uint16_t)sound_code);
		return;
	}
	sound_set_request = 0xD6u;
	if (sound_number >= 0x66u)
	{
		sound_set_request = 0xDAu;
		if (sound_number >= 0x6Bu)
			sound_set_request = 0xD8u;
	}
	saved_sound_number = sound_number;
	test_mode_queue_sound_screen_2(sound_set_request);
	sound_number = saved_sound_number;
	sound_code = mem_read8(sound_test_codes_ADDRESS + sound_number);
	if (sound_code == 0u)
	{
		sound_test_number_b = 0u;
		return;
	}
	sound_test_number_b = sound_number + 1u;
	test_mode_queue_sound_screen_2((uint16_t)sound_code);
}

/* The sound test's EXIT: back to the menu. */
void test_mode_sound_test_exit(void)
{
	test_mode_routine = test_mode_menu_ADDRESS;
}

/* Starts the C.R.T. TEST's first page: a black background (the background colour of both palettes,
   609C00 and 689C00), C.R.T. TEST 1/2, the colour bars (test_mode_crt_test_draw_bars) and the blinking
   PUSH TEST BUTTON TO CONTINUE; then test_mode_crt_test_bars_run. */
void test_mode_crt_test(void)
{
	uint32_t character = 0, text = 0;

	character = test_mode_clear_text();
	mem_write16(PALETTE_609C00, 0u);
	mem_write16(0x689C00u, 0u);
	text = crt_test_first_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_crt_test_draw_bars(character);
	test_mode_show_continue_prompt();
	test_mode_routine = test_mode_crt_test_bars_run_ADDRESS;
	test_mode_crt_test_bars_run((uint16_t)character);
}

/* The colour bars, each frame: TEST goes on to the second page (test_mode_crt_test_grid); the message
   blinks. */
void test_mode_crt_test_bars_run(uint16_t character)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_crt_test_grid_ADDRESS;
	test_mode_blink_prompt(character);
}

/* The C.R.T. test's second page: the crosshatch (test_mode_crt_test_draw_grid), C.R.T. TEST 2/2 and the
   blinking PUSH TEST BUTTON TO EXIT; then test_mode_crt_test_grid_run. */
void test_mode_crt_test_grid(void)
{
	uint32_t character = 0, text = 0;

	character = test_mode_crt_test_draw_grid(0u);
	text = crt_test_second_page_text_ADDRESS;
	character = test_mode_draw_texts(character, text);
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_crt_test_grid_run_ADDRESS;
	test_mode_crt_test_grid_run(character);
}

/* The crosshatch, each frame: TEST clears the text layer, gives the background back its blue (733) and
   goes back to the menu; the message blinks. */
void test_mode_crt_test_grid_run(uint32_t character)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		character = test_mode_clear_text();
		mem_write16(PALETTE_609C00, 0x733u);
		mem_write16(0x689C00u, 0x733u);
		test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt(character);
}

/* Draws the first page on the text layer: a frame around the screen and four bars of 32 steps
   (test_mode_crt_test_draw_bar, characters 8001-8004: red, green, blue and white). */
uint32_t test_mode_crt_test_draw_bars(uint32_t character)
{
	uint32_t cell = 0, cells_left = 0, drawn_character = 0, rows_left = 0;

	cell = TEXT_CELL(1, 0);
	drawn_character = (character & ~0xFFFFu) | 0x6C15u;
	cells_left = 0x26u;
	do
	{
		mem_write16(cell + 0xD80u, drawn_character);
		mem_write16(cell, drawn_character);
		cells_left--;
		cell += 2u;
	} while (cells_left != 0u);
	cell = (cell & ~0xFFFFu) | 0xF080u;
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	rows_left = 0x1Au;
	do
	{
		mem_write16(cell + 0x4Eu, drawn_character);
		mem_write16(cell, drawn_character);
		cell += 0x80u;
		rows_left--;
	} while (rows_left != 0u);
	cell = (cell & ~0xFFFFu) | 0xF000u;
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	mem_write16(cell, drawn_character);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	mem_write16(cell + 0x4Eu, drawn_character);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	mem_write16(cell + 0xD80u, drawn_character);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	mem_write16(cell + 0xDCEu, drawn_character);
	cell = (cell & ~0xFFFFu) | 0xF208u;
	drawn_character = (drawn_character & ~0xFFFFu) | 0x8001u;
	cell = test_mode_crt_test_draw_bar((uint16_t)drawn_character, cell);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	cell = test_mode_crt_test_draw_bar((uint16_t)drawn_character, cell);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	cell = test_mode_crt_test_draw_bar((uint16_t)drawn_character, cell);
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	cell = test_mode_crt_test_draw_bar((uint16_t)drawn_character, cell);
	return drawn_character;
}

/* Draws one colour bar: 5 rows of 32 cells of character R0, each column with the next palette (the cell
   + 200), so that the colour steps up across it; returns the place 5 rows down for the next bar. */
uint32_t test_mode_crt_test_draw_bar(uint16_t character, uint32_t cell)
{
	uint16_t column_character = 0;
	uint32_t column_cell = 0, columns_left = 0;

	column_cell = cell;
	column_character = (uint16_t)character;
	columns_left = 0x20u;
	do
	{
		mem_write16(column_cell + 0x200u, column_character);
		mem_write16(column_cell + 0x180u, column_character);
		mem_write16(column_cell + 0x100u, column_character);
		mem_write16(column_cell + 0x80u, column_character);
		mem_write16(column_cell, column_character);
		column_character += 0x200u;
		columns_left--;
		column_cell += 2u;
	} while (columns_left != 0u);
	cell += 0x280u;
	return cell;
}

/* Draws the crosshatch of the second page over the whole text layer (characters 6C1B-6C1F: the edges and
   the inside), and blanks the cell after the exit message. */
uint32_t test_mode_crt_test_draw_grid(uint32_t character)
{
	uint16_t far_edge_character = 0;
	uint32_t cell = 0, cells_left = 0, cells_left_in_row = 0, drawn_character = 0, row_cell = 0, rows_left = 0;

	cell = TEXT_CELL(0, 1);
	drawn_character = (character & ~0xFFFFu) | 0x6C1Bu;
	far_edge_character = (uint16_t)(drawn_character + 1u);
	rows_left = 0x1Au;
	do
	{
		mem_write16(cell + 0x4Eu, far_edge_character);
		mem_write16(cell, drawn_character);
		cell += 0x80u;
		rows_left--;
	} while (rows_left != 0u);
	cell = (cell & ~0xFFFFu) | 0xF002u;
	far_edge_character++;
	drawn_character = (drawn_character & ~0xFFFFu) | far_edge_character;
	far_edge_character++;
	cells_left = 0x26u;
	do
	{
		mem_write16(cell + 0xD80u, far_edge_character);
		mem_write16(cell, drawn_character);
		cells_left--;
		cell += 2u;
	} while (cells_left != 0u);
	cell = (cell & ~0xFFFFu) | 0xF082u;
	drawn_character = (drawn_character & ~0xFFFFu) | far_edge_character;
	drawn_character = (drawn_character & ~0xFFFFu) | (uint16_t)(drawn_character + 1u);
	rows_left = 0x1Au;
	do
	{
		row_cell = cell;
		cells_left_in_row = 0x26u;
		do
		{
			mem_write16(row_cell, drawn_character);
			cells_left_in_row--;
			row_cell += 2u;
		} while (cells_left_in_row != 0u);
		cell += 0x80u;
		rows_left--;
	} while (rows_left != 0u);
	mem_write16((cell & ~0xFFFFu) | 0xFCC0u, 0u);
	return drawn_character;
}

/* Starts the GAME ASSIGNMENTS: the settings' names and values (test_mode_game_assignments_draw), EXIT,
   the prompt, the cursor on EXIT (test_mode_page_cursor 9); then test_mode_game_assignments_run. */
void test_mode_game_assignments(void)
{
	uint32_t cell = 0, character = 0, text = 0;

	character = test_mode_clear_text();
	text = game_assignments_page_text_ADDRESS;
	test_mode_draw_texts(character, text);
	character = test_mode_game_assignments_draw();
	test_mode_draw_instructions(character);
	test_mode_page_cursor = 9u;
	cell = 0x310000u | game_assignments_exit_cell;
	test_mode_set_cursor(character, cell);
	test_mode_routine = test_mode_game_assignments_run_ADDRESS;
	test_mode_game_assignments_run();
}

/* The game assignments, each frame: SERVICE moves the cursor through the ten items; TEST changes the
   setting under it: the game difficulty (test_mode_game_assignments_difficulty), a stage's time
   (test_mode_game_assignments_time), the steering wheel reaction (test_mode_game_assignments_steering),
   the advertise sound (test_mode_game_assignments_advertise), or EXIT (test_mode_game_assignments_exit);
   the arrow blinks. */
void test_mode_game_assignments_run(void)
{
	uint32_t cell = 0, page_cursor = 0;
	uint8_t blink_timer = 0;
	uint16_t cursor_character = 0;
	uint32_t blink_cell = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		page_cursor = test_mode_page_cursor;
		page_cursor = (uint8_t)(page_cursor + 1u);
		if ((int8_t)page_cursor >= 0xA)
			page_cursor = 0u;
		test_mode_page_cursor = page_cursor;
		cell = 0x310000u | game_assignments_cursor_cells[page_cursor];
		test_mode_set_cursor(page_cursor, cell);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		current_object = settings_magic_ADDRESS;
		switch (test_mode_page_cursor)
		{
		case 0:
			test_mode_game_assignments_difficulty();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			test_mode_game_assignments_time();
			break;
		case 7:
			test_mode_game_assignments_steering();
			break;
		case 8:
			test_mode_game_assignments_advertise();
			break;
		case 9:
			test_mode_game_assignments_exit();
			break;
		default:
			invalid_jump(test_mode_game_assignments_item_jumps[test_mode_page_cursor]);
		}
	}
	blink_timer = test_mode_blink_timer - 1u;
	test_mode_blink_timer = blink_timer;
	if ((int8_t)blink_timer < 0)
	{
		test_mode_blink_timer = 5;
		blink_cell = test_mode_blink_address;
		cursor_character = mem_read16(blink_cell) ^ 0x6A7Fu;
		mem_write16(blink_cell, cursor_character);
		if (cursor_character != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the game settings: game_difficulty + 1 (1-8) and its bar from EASY to HARD (two cells a level),
   the six stages' time_difficulty (NORMAL, or +n SEC / -n SEC), and steering_wheel_reaction and
   advertise_sound (bit 0) as On or Off. */
uint32_t test_mode_game_assignments_draw(void)
{
	uint16_t sign_character = 0;
	uint32_t advertise_on = 0, bar_cells_left = 0, cell = 0, colour = 0, digit_count = 0, grey = 0, number = 0, row_cell = 0, seconds = 0, stages_left = 0, steering_on = 0, text = 0;
	uint32_t bar_length = 0;

	current_object = settings_magic_ADDRESS;
	cell = TEXT_CELL(32, 5);
	colour = 0x64u;
	digit_count = 2u;
	number = game_difficulty + 1u;
	test_mode_draw_number(digit_count, &number, &colour, &cell);
	cell = (cell & ~0xFFFFu) | 0xF3A8u;
	bar_length = game_difficulty * 2u;
	bar_cells_left = bar_length;
	if (bar_length != 0u)
	{
		do
		{
			mem_write16(cell, (mem_read16(cell) & 0x1FFu) | 0x6400u);
			cell += 2u;
			bar_cells_left--;
		} while (bar_cells_left != 0u);
	}
	grey = 0x60u;
	test_mode_colour_line(grey, cell);
	row_cell = TEXT_CELL(28, 10);
	r12 = current_object + 0x32u;
	stages_left = 6u;
	do
	{
		cell = row_cell;
		seconds = mem_read8(r12);
		r12 = r12 + 1u;
		if (seconds == 0u)
		{
			mem_write16(cell, 0x644Eu);
			cell += 2u;
			mem_write16(cell, 0x644Fu);
			cell += 2u;
			mem_write16(cell, 0x6452u);
			cell += 2u;
			mem_write16(cell, 0x644Du);
			cell += 2u;
			mem_write16(cell, 0x6441u);
			mem_write16(cell + 2u, 0x644Cu);
		}
		else
		{
			sign_character = 0x642Bu;
			if ((int8_t)seconds < 0)
			{
				sign_character = (uint16_t)((sign_character & ~0xFFu) | 0x2Du);
				seconds = (uint8_t)(0u - seconds);
			}
			mem_write16(cell, sign_character);
			colour = 0x64u;
			digit_count = 2u;
			cell += 2u;
			test_mode_draw_number(digit_count, &seconds, &colour, &cell);
			cell += 4u;
			mem_write16(cell, 0x6653u);
			cell += 2u;
			mem_write16(cell, 0x6645u);
			mem_write16(cell + 2u, 0x6643u);
		}
		stages_left--;
		row_cell += 0x80u;
	} while (stages_left != 0u);
	steering_on = steering_wheel_reaction;
	steering_on = steering_on & 1u;
	text = steering_wheel_reaction_texts[steering_on];
	test_mode_draw_texts(steering_on, text);
	advertise_on = advertise_sound;
	advertise_on = advertise_on & 1u;
	text = advertise_sound_texts[advertise_on];
	return test_mode_draw_texts(advertise_on, text);
}

/* GAME DIFFICULTY: game_difficulty one level harder, after the eighth back to the first; redraws. */
void test_mode_game_assignments_difficulty(void)
{
	uint32_t ignored = 0;

	game_difficulty = (game_difficulty + 1u) & 7u;
	ignored = test_mode_game_assignments_draw();
}

/* TIME DIFFICULTY: the time_difficulty of the stage under the cursor (items 1-6: PRE-STAGE, STAGE 1-4,
   FINAL STAGE) one second more, from +5 back to -5; redraws. */
void test_mode_game_assignments_time(void)
{
	uint8_t seconds = 0;
	uint32_t ignored = 0, stage_item = 0;

	stage_item = test_mode_page_cursor;
	seconds = (uint8_t)(mem_read8(current_object + 0x31u + stage_item) + 1u);
	if ((int8_t)seconds > 5)
		seconds = 0xFBu;
	mem_write8(current_object + 0x31u + stage_item, seconds);
	ignored = test_mode_game_assignments_draw();
}

/* STEERING WHEEL REACTION: steering_wheel_reaction on or off (1 or 0); redraws. */
void test_mode_game_assignments_steering(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = steering_wheel_reaction > 0u;
	steering_wheel_reaction = 1u - carry;
	ignored = test_mode_game_assignments_draw();
}
