/**
 *    Name:        game_04.c
 *    Description: the game's functions, part 04 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
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
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = current_object + 0x80u;
	r9 = OBJECT(current_object)->race_background_fade_watch.count;
	do
	{
		r1 = mem_read32(r10);
		r10 += 4u;
		if (r1 != 0u)
		{
			r1 = mem_read32(r1);
			r0 |= r1;
			if ((r1 & 1u) == 0u)
				mem_write32(r10 - 4u, 0u);
		}
		r9--;
	} while (r9 != 0u);
	if ((r0 & 1u) == 0u)
	{
		SCREEN(screen)->background_fading = 0u;
		OBJECT(current_object)->flags = 0u;
	}
}

/* Starts a palette_fade of 16 colours from the palette at R10 to the one at R11, for palette offset R12
   (R0 blocks of 16 in its upload record), a step every R1 frames, the first after (screen
   palette_fades_started mod R1) + 1 (unk_F0E counts the fades started); returns the object. */
uint32_t start_palette_fade(uint8_t r0, uint8_t r1, uint32_t r10, uint32_t r11)
{
	uint8_t r0_2 = 0;
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->handler = palette_fade_prepare_ADDRESS;
	OBJECT(object)->palette_fade.upload_length = 8u;
	OBJECT(object)->palette_fade.upload_colours = object + 0x34u;
	OBJECT(object)->palette_fade.palette_offset = r12;
	OBJECT(object)->palette_fade.blocks = (uint8_t)r0;
	r0_2 = remainder_u8(SCREEN(screen)->palette_fades_started, (uint8_t)r1);
	SCREEN(screen)->palette_fades_started++;
	OBJECT(object)->palette_fade.delay = r0_2 + 1u;
	OBJECT(object)->palette_fade.period = r1;
	OBJECT(object)->palette_fade.from = r10;
	OBJECT(object)->palette_fade.to = r11;
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
	uint16_t r1 = 0;
	uint32_t r0 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint8_t colours_left = 0;

	r10 = OBJECT(current_object)->palette_fade.from;
	r11 = OBJECT(current_object)->palette_fade.to;
	r12 = OBJECT(current_object)->palette_fade.colour_place;
	r13 = OBJECT(current_object)->palette_fade.difference_place;
	r0 = ROM16(r10);
	r1 = ROM16(r11);
	mem_write16(r12, r0);
	r12 = r12 + 2u;
	mem_write8(r13, (r0 & 0x1Fu) - (r1 & 0x1Fu));
	r13++;
	r0 = r0 >> 5;
	r1 = r1 >> 5;
	mem_write8(r13, (r0 & 0x1Fu) - (r1 & 0x1Fu));
	r13++;
	r0 = r0 >> 5;
	r0 = r0 & 0x1Fu;
	r0 = (uint8_t)(r0 - ((r1 >> 5) & 0x1Fu));
	mem_write8(r13, r0);
	r13++;
	colours_left = OBJECT(current_object)->palette_fade.colours_left;
	OBJECT(current_object)->palette_fade.colours_left = colours_left - 1u;
	r10 += 2u;
	r11 += 2u;
	if (colours_left == 1u)
	{
		OBJECT(current_object)->handler = palette_fade_step_ADDRESS;
		palette_fade_step(r0);
		return;
	}
	OBJECT(current_object)->palette_fade.from = r10;
	OBJECT(current_object)->palette_fade.to = r11;
	OBJECT(current_object)->palette_fade.colour_place = r12;
	OBJECT(current_object)->palette_fade.difference_place = r13;
}

/* Every period frames (delay; start_palette_fade staggers the first) moves each of the 16 colours one
   step of each of red, green and blue toward the second palette (by its differences, counted down to 0)
   and queues them (queue_palette_list with its record at 28: 8 bytes, the colours, palette_offset,
   blocks); ends when no colour moved, or when the screen fades out (screen flags bit 0, with unk_153 at
   most 5 and even). */
void palette_fade_step(uint32_t r0)
{
	bool L_0186AA = false, L_0186B2 = false, L_0186BA = false, L_0186CB = false, L_0186D1 = false, L_0186D7 = false, L_0186E0 = false, L_0186E9 = false, L_0186F2 = false;
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint8_t delay = 0;

	if ((SCREEN(screen)->flags & 1u) != 0u)
	{
		r0_2 = (r0 & ~0xFFu) | SCREEN(screen)->fade_kind;
		if ((int8_t)r0_2 <= 5 && (r0_2 & 1u) == 0u)
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
	r10 = current_object + 0x54u;
	r11 = current_object + 0x34u;
	r0_3 = 0u;
	r9 = 0x10u;
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
							if (L_0186D1 || L_0186D7 || L_0186E0 || L_0186E9 || L_0186F2 || (int8_t)mem_read8(r10) < 0)
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
																						mem_write8(r10, mem_read8(r10) + 1u);
																						r0_3++;
																						r10++;
																					}
																				}
																				L_0186AA = false;
																			}
																		}
																	}
																}
															}
														}
														if (L_0186B2 || L_0186B2 || L_0186D1 || L_0186D7 || L_0186E9 || L_0186F2 || (int8_t)mem_read8(r10) < 0)
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
																								mem_write8(r10, mem_read8(r10) + 1u);
																								r0_3 += 0x20u;
																								r10++;
																							}
																						}
																						L_0186B2 = false;
																					}
																				}
																			}
																		}
																		if (L_0186BA || L_0186BA || L_0186D1 || L_0186E9 || (int8_t)mem_read8(r10) < 0)
																		{
																			if (L_0186BA || L_0186BA || !L_0186D1)
																			{
																				if (!L_0186BA)
																				{
																					if (!L_0186BA)
																					{
																						L_0186E9 = false;
																						mem_write8(r10, mem_read8(r10) + 1u);
																						r0_3 += 0x400u;
																						r10++;
																					}
																				}
																				L_0186BA = false;
																				mem_write16(r11, mem_read16(r11) + r0_3);
																				r9--;
																				r11 += 2u;
																				if (r9 == 0u)
																				{
																					r19 = current_object + 0x28u;
																					queue_palette_list(r19);
																					return;
																				}
																				r0_3 = 0u;
																				L_0186CB = (int8_t)mem_read8(r10) < 0;
																			}
																			if (!L_0186CB)
																			{
																				if (L_0186D1 || (int8_t)mem_read8(r10) > 0)
																				{
																					L_0186D1 = false;
																					mem_write8(r10, mem_read8(r10) - 1u);
																					r0_3--;
																					r10++;
																					L_0186AA = true;
																				}
																				if (!L_0186AA)
																				{
																					r10++;
																					L_0186AA = true;
																				}
																			}
																		}
																	}
																	if (L_0186CB)
																		break;
																	if (L_0186AA)
																		break;
																	if (L_0186F2 || (int8_t)mem_read8(r10) > 0)
																	{
																		L_0186F2 = false;
																		mem_write8(r10, mem_read8(r10) - 1u);
																		r0_3 -= 0x400u;
																		r10++;
																		L_0186BA = true;
																	}
																} while (L_0186BA);
																if (L_0186CB)
																	break;
																if (L_0186AA)
																	break;
																r10++;
																L_0186BA = true;
															}
														}
													} while (L_0186CB);
												} while (L_0186AA);
											} while (L_0186AA);
										}
										if (L_0186E0 || (int8_t)mem_read8(r10) > 0)
										{
											L_0186E0 = false;
											mem_write8(r10, mem_read8(r10) - 1u);
											r0_3 -= 0x20u;
											r10++;
											L_0186B2 = true;
										}
									} while (L_0186B2);
									r10++;
									L_0186B2 = true;
								}
							}
							L_0186D1 = (int8_t)mem_read8(r10) > 0;
						} while (L_0186D1);
						r10++;
						L_0186D7 = (int8_t)mem_read8(r10) < 0;
					} while (L_0186D7);
					L_0186E0 = (int8_t)mem_read8(r10) > 0;
				} while (L_0186E0);
				r10++;
				L_0186E9 = (int8_t)mem_read8(r10) < 0;
			} while (L_0186E9);
			L_0186F2 = (int8_t)mem_read8(r10) > 0;
		} while (L_0186F2);
		r10++;
		r11 += 2u;
		r9--;
	} while (r9 != 0u);
	OBJECT(current_object)->flags = 0u;
}

/* Starts the cabinets' LCDs at boot (both, through port G of the two I/O chips): the 4-bit start
   (command nibbles 3, 3, 3, 2), display on (0C), SEGA Enterprises as both screens' text (lcd_text),
   command 40 then the eight custom characters (rom_018B4E: the DJ's face among them); the screens' lamps
   off (screen button_lamps), lcd_restart_frames E10. */
void lcd_start(void)
{
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r9 = 0, r10 = 0;

	lcd_send_step = 0u;
	lcd_busy = 0u;
	lcd_restart_frames = 0xE10u;
	SCREEN(SCREEN_1)->button_lamps = 0u;
	SCREEN(SCREEN_2)->button_lamps = 0u;
	r0 = 0x30u;
	lcd_write_command_nibble(r0);
	r9 = 0x2008u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 = 0x30u;
	lcd_write_command_nibble(r0);
	r9 = 0xC8u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 = 0x30u;
	lcd_write_command_nibble(r0);
	r0 = 0x20u;
	lcd_command_nibble_after_pause(r0);
	r0 = 0u;
	lcd_command_nibble_after_pause(r0);
	r0 = 0xC0u;
	lcd_command_nibble_after_pause(r0);
	r0_2 = rom_018B90;
	r1 = rom_018B94;
	AS32(SCREEN(SCREEN_1)->lcd_text) = r0_2;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = r1;
	AS32(SCREEN(SCREEN_2)->lcd_text) = r0_2;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = r1;
	r0_2 = rom_018B98;
	r1 = rom_018B9C;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = r0_2;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = r1;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = r0_2;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = r1;
	r0_2 = (r0_2 & ~0xFFu) | 0x40u;
	r12 = unk_218F20_ADDRESS;
	lcd_command_nibble_after_pause((uint8_t)r0_2);
	r0_2 &= ~0xFFu;
	lcd_command_nibble_after_pause((uint8_t)r0_2);
	r10 = 0x18B4Eu;
	for (;;)
	{
		r0_2 = (r0_2 & ~0xFFu) | ROM8(r10);
		r10++;
		if ((int8_t)r0_2 < 0)
			break;
		r0_2 = lcd_write_character(r0_2);
	}
}

/* Writes the data byte R0 to both LCDs, its high nibble then its low one (lcd_write_data_nibble), with
   pauses. */
uint32_t lcd_write_character(uint32_t r0)
{
	uint32_t r0_2 = 0, r9 = 0;
	uint32_t stack_4 = 0;

	r9 = 0x50u;
	do
	{
		r9--;
	} while (r9 != 0u);
	stack_4 = r0;
	r0 = r0 & 0xF0u;
	lcd_write_data_nibble((uint8_t)r0);
	r9 = 0x50u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0_2 = (stack_4 & ~0xFFu) | (uint8_t)(stack_4 << 4);
	lcd_write_data_nibble((uint8_t)r0_2);
	return r0_2;
}

/* Writes the high nibble of R0 to both LCDs as data (port G of the two I/O chips, inverted: the data
   lines, register select set, then the enable strobe bit 3 dropped). */
void lcd_write_data_nibble(uint8_t r0)
{
	uint32_t r9 = 0;

	r0 |= 0xFu;
	mem_write8(IO_C0000C, ~r0);
	mem_write8(0xC8000Cu, ~r0);
	r9 = 0x14u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 &= ~8u;
	mem_write8(IO_C0000C, ~r0);
	mem_write8(0xC8000Cu, ~r0);
}

/* A pause, then lcd_write_command_nibble. */
void lcd_command_nibble_after_pause(uint8_t r0)
{
	uint32_t r9 = 0;

	r9 = 0x50u;
	do
	{
		r9--;
	} while (r9 != 0u);
	lcd_write_command_nibble(r0);
}

/* Writes the high nibble of R0 to both LCDs as a command (register select clear), then drops the enable
   strobe (bit 3). */
void lcd_write_command_nibble(uint8_t r0)
{
	uint32_t r9 = 0;

	r0 |= 0xBu;
	mem_write8(IO_C0000C, ~r0);
	mem_write8(0xC8000Cu, ~r0);
	r9 = 0x14u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 &= ~8u;
	mem_write8(IO_C0000C, ~r0);
	mem_write8(0xC8000Cu, ~r0);
}

/* After the test mode: both screens' LCD text back to SEGA Enterprises (lcd_text), their lamps off
   (screen button_lamps), and lcd_restart_frames 0 so that the next VBLANK starts the LCDs again. */
void lcd_reset(void)
{
	uint32_t r0 = 0, r1 = 0;

	lcd_restart_frames = 0u;
	SCREEN(SCREEN_1)->button_lamps = 0u;
	SCREEN(SCREEN_2)->button_lamps = 0u;
	r0 = rom_018B90;
	r1 = rom_018B94;
	AS32(SCREEN(SCREEN_1)->lcd_text) = r0;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = r1;
	AS32(SCREEN(SCREEN_2)->lcd_text) = r0;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = r1;
	r0 = rom_018B98;
	r1 = rom_018B9C;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = r0;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = r1;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = r0;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = r1;
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
	uint8_t r0 = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;
	uint32_t lcd_text_4_to_7 = 0, lcd_send_buffer_1_0_long = 0, lcd_text_12_to_15 = 0, lcd_send_buffer_1_8_long = 0, lcd_text_4_to_7_2 = 0, lcd_send_buffer_2_0_long = 0, lcd_text_12_to_15_2 = 0, lcd_send_buffer_2_8_long = 0, lcd_send_buffer_1_0_long_2 = 0, lcd_send_buffer_1_8_long_2 = 0, lcd_send_buffer_2_0_long_2 = 0, lcd_send_buffer_2_8_long_2 = 0;

	t1 = lcd_restart_frames;
	lcd_restart_frames = t1 - 1u;
	if ((int16_t)t1 <= 1)
	{
		lcd_init_step = 0x2Cu;
		lcd_restart_frames = 0xE10u;
		return;
	}
	if (lcd_busy != 0u)
		return;
	r0 = lcd_send_step;
	carry = r0 & 1u;
	r0 = r0 >> 1;
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
	t2 = r0;
	r0 = (uint8_t)(t2 - 1u);
	if (t2 != 1u)
		r0 = 4u;
	lcd_write_command_nibble(r0);
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
   (port_g_outputs, the lamps): while lcd_init_step counts, the start sequence (rom_018B21, from the
   end), then idle (lcd_send_step 22); else by lcd_send_step: the cursor home command (80), then the 16
   characters of lcd_send_buffer_1 to screen 1's LCD and of lcd_send_buffer_2 to screen 2's, a nibble at
   a time; lcd_busy while it writes the port. */
void lcd_send_next_nibble(void)
{
	uint32_t carry = 0;
	uint8_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0;
	uint8_t t1 = 0, t2 = 0;

	if (lcd_init_step != 0u)
	{
		lcd_busy = 0xFFu;
		r0_2 = (uint8_t)~rom_018B21[lcd_init_step];
		r1 = r0_2 | port_g_outputs[1];
		mem_write8(IO_C0000C, r0_2 | port_g_outputs[0]);
		mem_write8(0xC8000Cu, r1);
		t1 = lcd_init_step;
		lcd_init_step = t1 - 1u;
		if (t1 == 1u)
		{
			lcd_send_step = 0x22u;
			lcd_busy = 0u;
		}
		return;
	}
	r0 = lcd_send_step;
	if (r0 >= 0x22u)
		return;
	lcd_busy = 0xFFu;
	lcd_send_step++;
	t2 = r0;
	r0 = (uint8_t)(t2 - 2u);
	if ((int8_t)t2 < 2)
	{
		r0 = (uint8_t)~r0;
		r0 = (uint8_t)~((r0 << 7) | (r0 >> 1) | 0xBu);
		r1 = (uint8_t)r0;
		r0 = r0 | port_g_outputs[0];
		r1 |= port_g_outputs[1];
		mem_write8(IO_C0000C, r0);
		mem_write8(0xC8000Cu, r1);
		r9 = 0x14u;
		do
		{
			r9--;
		} while (r9 != 0u);
		mem_write8(IO_C0000C, r0 | 8u);
		mem_write8(0xC8000Cu, r1 | 8u);
		lcd_busy = 0u;
		return;
	}
	carry = r0 & 1u;
	r0 = r0 >> 1;
	r1 = lcd_send_buffer_2[r0];
	r0_2 = lcd_send_buffer_1[r0];
	if (carry)
	{
		r0_2 = (uint8_t)((uint32_t)r0_2 << 4);
		r1 = (uint8_t)((uint32_t)r1 << 4);
	}
	else
	{
		r0_2 &= ~0xFu;
		r1 &= ~0xFu;
	}
	r0_2 = (uint8_t)(~(r0_2 | 0xFu) | port_g_outputs[0]);
	r1 = (uint8_t)(~(r1 | 0xFu) | port_g_outputs[1]);
	mem_write8(IO_C0000C, r0_2);
	mem_write8(0xC8000Cu, r1);
	r9 = 0x14u;
	do
	{
		r9--;
	} while (r9 != 0u);
	mem_write8(IO_C0000C, r0_2 | 8u);
	mem_write8(0xC8000Cu, r1 | 8u);
	lcd_busy = 0u;
}

/* The screen's LCD, each frame. With the DJ's face on (dj_face) its first two characters are the face,
   its mouth the next shape of dj_mouth_shapes every third frame (dj_mouth_frames; rom_018AE8, pairs of
   custom characters), closed when the string ends (a negative byte); the rest of the line (all 16
   without the face) scrolls a text: a new lcd_scroll_request clears the line and starts lcd_scroll,
   which shifts the text in from the right a character every 5 frames (lcd_scroll_frames; ` shows custom
   character 7), then shifts spaces in until the text has gone (lcd_scroll_step 0-3). */
void lcd_update(void)
{
	uint8_t r1 = 0, r26 = 0;
	uint32_t r0 = 0, r10 = 0, r11 = 0, lcd_scroll = 0, lcd_scroll_request = 0;
	uint8_t dj_mouth_frames = 0, lcd_scroll_frames = 0, lcd_scroll_frames_2 = 0;

	r10 = screen + 0xF10u;
	if (SCREEN(screen)->dj_face == 0u)
		r0 = 0xFu;
	else
	{
		r11 = SCREEN(screen)->dj_mouth_shapes;
		if (r11 == 0u)
		{
			AS16(SCREEN(r10 - 0xF10u)->lcd_text) = rom_018AE8[0];
			SCREEN(screen)->dj_mouth_frames = 3u;
			r0 = 0xDu;
			r10 += 2u;
		}
		else
		{
			dj_mouth_frames = SCREEN(screen)->dj_mouth_frames;
			SCREEN(screen)->dj_mouth_frames = dj_mouth_frames - 1u;
			if (dj_mouth_frames != 1u)
			{
				r10 += 2u;
				r0 = 0xDu;
			}
			else
			{
				r0 = ROM8(r11);
				r11++;
				if ((int8_t)r0 < 0)
				{
					SCREEN(screen)->dj_mouth_shapes = 0u;
					AS16(SCREEN(r10 - 0xF10u)->lcd_text) = rom_018AE8[0];
					SCREEN(screen)->dj_mouth_frames = 3u;
					r0 = 0xDu;
					r10 += 2u;
				}
				else
				{
					AS16(SCREEN(r10 - 0xF10u)->lcd_text) = rom_018AE8[r0];
					SCREEN(screen)->dj_mouth_shapes = r11;
					SCREEN(screen)->dj_mouth_frames = 3u;
					r0 = 0xDu;
					r10 += 2u;
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
		r26 = 0x20u;
		move_string(r10, 0u, r10, r0 + 1u, 1u, 1u, 0u, r26, &r27, &r28);
	}
	lcd_scroll = SCREEN(screen)->lcd_scroll;
	if (lcd_scroll == 0u)
		return;
	switch (SCREEN(screen)->lcd_scroll_step)
	{
	case 0:
		r1 = mem_read8(lcd_scroll);
		SCREEN(screen)->lcd_scroll = lcd_scroll + 1u;
		if (r1 == 0x60u)
			r1 = 7u;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, r1);
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
		SCREEN(screen)->lcd_scroll = r0 + 1u;
		SCREEN(screen)->lcd_scroll_step++;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
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
		SCREEN(screen)->lcd_scroll = r0 + 1u;
		SCREEN(screen)->lcd_scroll_step++;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->lcd_scroll_frames = 5u;
		SCREEN(screen)->lcd_scroll_step++;
		break;
	case 2:
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
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
		invalid_jump(rom_018A70[SCREEN(screen)->lcd_scroll_step]);
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
	uint32_t r10 = 0;

	r10 = screen + 0xF10u;
	AS32(SCREEN(r10 - 0xF10u)->lcd_text) = 0x20202020u;
	r10 += 4u;
	SCREEN(r10 - 0xF14u)->lcd_text_4_to_7 = 0x20202020u;
	r10 += 4u;
	SCREEN(r10 - 0xF18u)->lcd_text_8_to_11 = 0x20202020u;
	SCREEN(r10 - 0xF18u)->lcd_text_12_to_15 = 0x20202020u;
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
   to it: a bar shrinking through each second (rom_018EA0) with the seconds left. Then a page every 150
   frames (page_frames), by page: 0 Time (lcd_race_info_time), 1 Order (the car numbers, car_numbers + 1,
   in race order from order_list, this screen's own blinking; skipped with one player), 2 Speed (screen
   shown_speed), 3 Max. (unk_F52), 4 Ave. (lcd_race_info_average); page_step 0 writes the label, 1
   updates the number. When the car reaches the goal (car at_goal) goes to lcd_race_info_finish; when the
   race is over (car race_over) shows the average and ends; while the LCD scrolls a text (screen
   lcd_scroll) waits in lcd_race_info_paused. */
void lcd_race_info_update(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0, maximum_speed = 0, shown_speed = 0;
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
		r0 = OBJECT(CAR(player_car)->object)->player_car.timer;
		if (r0 != 0u)
		{
			r1 = 0u;
			divux_pair(0x1Eu, &r0, &r1);
			move_string(rom_018EA0[(r1 + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			SCREEN(screen)->lcd_text_8_to_11 = (SCREEN(screen)->lcd_text_8_to_11 & 0xFFFF00FFu) | ((r0 + 0x31u & 0xFFu) << 8u);
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
		invalid_jump(rom_018BD4[OBJECT(current_object)->state]);
	}
	page_frames = OBJECT(current_object)->lcd_race_info.page_frames;
	OBJECT(current_object)->lcd_race_info.page_frames = page_frames - 1u;
	if (page_frames == 1u)
	{
		r0_2 = (uint8_t)(OBJECT(current_object)->lcd_race_info.page + 1u);
		if ((int8_t)r0_2 >= 5)
			r0_2 = 0u;
		OBJECT(current_object)->lcd_race_info.page = r0_2;
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
			invalid_jump(rom_018C84[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		break;
	case 1:
		break;
	case 2:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			move_string(0x18F3Au, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D5C[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		r10 = screen + 0xF1Cu;
		shown_speed = SCREEN(screen)->shown_speed;
		lcd_race_info_write_number(shown_speed, r10);
		return;
	case 3:
		switch (OBJECT(current_object)->lcd_race_info.page_step)
		{
		case 0:
			move_string(0x18F48u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->lcd_race_info.page_step++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D88[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		r10 = screen + 0xF1Cu;
		maximum_speed = (uint16_t)SCREEN(screen)->maximum_speed;
		lcd_race_info_write_number(maximum_speed, r10);
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
			invalid_jump(rom_018DB4[OBJECT(current_object)->lcd_race_info.page_step]);
		}
		break;
	default:
		invalid_jump(rom_018C64[OBJECT(current_object)->lcd_race_info.page]);
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
				move_string(0x18F3Au, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
				OBJECT(current_object)->lcd_race_info.page_step++;
				break;
			case 1:
				break;
			default:
				invalid_jump(rom_018D5C[OBJECT(current_object)->lcd_race_info.page_step]);
			}
			r10 = screen + 0xF1Cu;
			shown_speed = SCREEN(screen)->shown_speed;
			lcd_race_info_write_number(shown_speed, r10);
			return;
		}
		move_string(0x18F2Cu, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->lcd_race_info.frames = 0u;
		OBJECT(current_object)->lcd_race_info.page_step++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_018CF0[OBJECT(current_object)->lcd_race_info.page_step]);
	}
	r10 = screen + 0xF18u;
	r11 = OBJECT(current_object)->lcd_race_info.order_list;
	r9 = OBJECT(current_object)->lcd_race_info.racer_count;
	do
	{
		r0 = mem_read8(r11);
		r11++;
		if (SCREEN(screen)->player != r0 || (OBJECT(current_object)->lcd_race_info.frames & 8u) == 0u)
			r0_2 = (uint8_t)(car_numbers[r0] + 0x31u);
		else
			r0_2 = 0x20u;
		mem_write8(r10, r0_2);
		r9--;
		r10++;
	} while (r9 != 0u);
	OBJECT(current_object)->lcd_race_info.frames++;
}

/* Writes the Time page's label on the LCD, then the time (lcd_race_info_time). */
void lcd_race_info_time_page(void)
{
	move_string(0x18F1Eu, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->lcd_race_info.page_step++;
	lcd_race_info_time();
}

/* Writes the race time (the car's unk_04, in thousandths of a second) on the LCD as
   minutes'seconds"hundredths. */
void lcd_race_info_time(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r0 = CAR(player_car)->race_time / 0xAu;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = screen + 0xF20u - 1u;
	SCREEN(r10 - 0xF1Fu)->lcd_text_12_to_15 = (SCREEN(r10 - 0xF1Fu)->lcd_text_12_to_15 & 0xFFFFFFu) | ((r1 + 0x30u & 0xFFu) << 24u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	SCREEN(r10 - 0xF1Eu)->lcd_text_12_to_15 = (SCREEN(r10 - 0xF1Eu)->lcd_text_12_to_15 & 0xFF00FFFFu) | ((r1 + 0x30u & 0xFFu) << 16u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = r10 - 1u - 1u;
	SCREEN(r10 - 0xF1Cu)->lcd_text_12_to_15 = (SCREEN(r10 - 0xF1Cu)->lcd_text_12_to_15 & 0xFFFFFF00u) | (r1 + 0x30u & 0xFFu);
	r1 = 0u;
	divx_pair(6u, &r0, &r1);
	r10--;
	SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((r1 + 0x30u & 0xFFu) << 24u);
	mem_write8(r10 - 1u - 1u, r0 + 0x30u);
}

/* Writes the Ave. page's label on the LCD, then the average speed (lcd_race_info_average). */
void lcd_race_info_average_page(void)
{
	move_string(0x18F56u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->lcd_race_info.page_step++;
	lcd_race_info_average();
}

/* Writes the average speed on the LCD: the sum of the speeds (screen speed_sum) over how many there are
   (unk_F58), with lcd_race_info_write_number. */
void lcd_race_info_average(void)
{
	uint32_t r0 = 0, r10 = 0, speed_samples = 0;

	r10 = screen + 0xF1Cu;
	r0 = SCREEN(screen)->speed_sum;
	speed_samples = SCREEN(screen)->speed_samples;
	if (speed_samples == 0u)
	{
		lcd_race_info_write_number(r0, r10);
		return;
	}
	r0 = divide_u32(r0, speed_samples);
	lcd_race_info_write_number(r0, r10);
}

/* Writes a number of up to three digits on the LCD, right-aligned to end before the address it is given
   (F1C of the screen for the km/h pages), spaces for the leading zeros. */
void lcd_race_info_write_number(uint32_t r0, uint32_t r10)
{
	uint32_t zero = 0;
	uint8_t r2 = 0;
	uint32_t r1 = 0;

	r2 = 0x30u;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero)
	{
		r10--;
		SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((r1 + r2 & 0xFFu) << 24u);
		r10--;
		SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | 0x200000u;
		SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	r10--;
	SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Bu)->lcd_text_8_to_11 & 0xFFFFFFu) | ((r1 + r2 & 0xFFu) << 24u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero)
	{
		r10--;
		SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | ((r1 + r2 & 0xFFu) << 16u);
		SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	r10--;
	SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFF00FFFFu) | ((r1 + r2 & 0xFFu) << 16u);
	if (r0 == 0u)
	{
		SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | 0x2000u;
		return;
	}
	SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 = (SCREEN(r10 - 0xF1Au)->lcd_text_8_to_11 & 0xFFFF00FFu) | ((r0 + r2 & 0xFFu) << 8u);
}

/* The car has reached the goal: shows */
void lcd_race_info_finish(void)
{
	uint16_t frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		move_string(0x18F64u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
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
		invalid_jump(rom_018E40[OBJECT(current_object)->state]);
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
   clears screen nearing_fork, gives the car the song of its model (car song = rom_019421 by car model),
   notes the stage (screen route_stage), then car_radio_wait_start. */
void car_radio_start(void)
{
	uint32_t r0 = 0, r9 = 0, object = 0, race = 0;

	object = current_object + 0x80u;
	AS32(OBJECT(object - 0x80u)->car_radio.car_models) = 0xFFFFFFFFu;
	OBJECT(object - 0x80u)->unk_84 = 0xFFFFu;
	OBJECT(object - 0x80u)->unk_86 = 0xFFu;
	OBJECT(object - 0x80u)->unk_87 = 0xFFu;
	race = SCREEN(screen)->host_record + 0x1Fu;
	r9 = RACE(race - 0x1Fu)->racer_count;
	OBJECT(current_object)->car_radio.cars_to_introduce = r9;
	race++;
	do
	{
		r0 = mem_read8(race);
		mem_write8(current_object + 0x80u + car_numbers[r0], mem_read8(0x20FC49u + (r0 << 7)));
		r9--;
		race++;
	} while (r9 != 0u);
	OBJECT(current_object)->car_radio.car_number = car_numbers[SCREEN(screen)->player];
	SCREEN(screen)->nearing_fork = 0u;
	CAR(player_car)->song = rom_019421[CAR(player_car)->model];
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
   (rom_019458 by screen route_stage); 3 the station plays; 4-8 while screen nearing_fork is set
   (fork_camera_update sets it, a new route_stage clears it): 3F to the screen's sound byte
   (fade_out_music_at), sound E0 after 60 frames, sound EE, 7F when it clears and after 120 frames the
   stage's message (rom_019571 by screen current_stage_level) and sound. On the station: any of the three
   buttons goes back to the car's song (state 0, talk_step 4); the DJ talks when no message plays (screen
   dj_mouth_shapes), by talk_step: 0 message 0, 1-2 each car number (message 9 + number) and its car
   (message 1 + model), 3 message 2A, 4 the queued_message or, at random, a list of remarks
   (routine_lists_1966C), 5 nothing. */
void car_radio_update(void)
{
	bool L_019118 = false, L_019132 = false, L_019137 = false, L_01914C = false, L_019154 = false, L_019167 = false, L_019180 = false;
	uint8_t r0_2 = 0, queued_message = 0, route_stage = 0;
	uint16_t buttons = 0;
	uint32_t r0 = 0, r10 = 0, object = 0, song = 0;
	uint8_t t1 = 0, timer = 0, timer_2 = 0, timer_3 = 0, cars_to_introduce = 0;
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
							jump_target = L_019118 ? 0x190F0u : L_019132 ? 0x190F0u : L_01914C ? 0x190F0u : L_019154 ? 0x190F0u : L_019167 ? 0x190F0u : L_019180 ? 0x190F0u : rom_019044[OBJECT(current_object)->state];
							switch (jump_target)
							{
							case 0x019068u:
								r0 = CAR(player_car)->song;
								r0 = car_radio_song(r0);
								request_sound((uint8_t)r0);
								OBJECT(current_object)->state++;
							case 0x019078u:
								buttons = OBJECT(current_object)->car_radio.buttons;
								if ((buttons & 0x100u) != 0u)
								{
									OBJECT(current_object)->state = 2u;
									r0_2 = 0x1Fu;
									request_sound(r0_2);
									return;
								}
								if ((buttons & 0x200u) != 0u)
								{
									if ((buttons & 0x400u) != 0u)
									{
										r0 = 0xAu;
										r0 = car_radio_song(r0);
										request_sound((uint8_t)r0);
										return;
									}
									r0_2 = 0x1Fu;
									request_sound(r0_2);
									song = CAR(player_car)->song;
									t1 = song - 1u;
									r0 = t1;
									if ((int8_t)t1 < 0)
										r0 = 9u;
									CAR(player_car)->song = r0;
									r0 = car_radio_song(r0);
									request_sound((uint8_t)r0);
									return;
								}
								if ((buttons & 0x400u) == 0u && (buttons & 0x40u) == 0u)
									return;
								r0_2 = 0x1Fu;
								request_sound(r0_2);
								r0 = CAR(player_car)->song;
								r0 = (uint8_t)(r0 + 1u);
								if ((int8_t)r0 < 0xA)
								{
									CAR(player_car)->song = r0;
									r0 = car_radio_song(r0);
									request_sound((uint8_t)r0);
									return;
								}
								r0 = 0u;
								CAR(player_car)->song = r0;
								r0 = car_radio_song(r0);
								request_sound((uint8_t)r0);
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
														r10 = 0x1954Cu;
														car_radio_show_title(r10);
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
														r0_2 = 0xE0u;
														request_sound(r0_2);
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
												OBJECT(current_object)->car_radio.queued_message = rom_019571[SCREEN(screen)->current_stage_level];
												r0_2 = rom_019458[SCREEN(screen)->route_stage];
												request_sound(r0_2);
												OBJECT(current_object)->state = 3u;
											}
											else
											{
												if (L_019167 || !L_019180)
												{
													if (!L_019167)
													{
														r0_2 = 0xEEu;
														request_sound(r0_2);
														OBJECT(current_object)->state++;
													}
													L_019167 = false;
												}
												if (L_019180 || SCREEN(screen)->nearing_fork == 0u)
												{
													if (!L_019180)
													{
														r0_2 = 0x7Fu;
														fade_out_music_at(r0_2);
														OBJECT(current_object)->car_radio.timer = 0x78u;
														OBJECT(current_object)->state++;
													}
													L_019180 = false;
													timer_2 = OBJECT(current_object)->car_radio.timer;
													OBJECT(current_object)->car_radio.timer = timer_2 - 1u;
													if (timer_2 == 1u)
													{
														OBJECT(current_object)->car_radio.queued_message = rom_019571[SCREEN(screen)->current_stage_level];
														r0_2 = rom_019458[SCREEN(screen)->route_stage];
														request_sound(r0_2);
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
												r0_2 = rom_019458[SCREEN(screen)->route_stage];
												request_sound(r0_2);
												OBJECT(current_object)->state++;
											}
											L_019118 = false;
										}
										if (L_019132 || SCREEN(screen)->nearing_fork != 0u)
										{
											if (!L_019132)
											{
												r0_2 = 0x3Fu;
												fade_out_music_at(r0_2);
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
									r0_2 = 0x1Fu;
									request_sound(r0_2);
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
									r0_2 = 0u;
									start_dj_message(r0_2);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 1:
									r0 = OBJECT(current_object)->car_radio.next_car_number;
									while ((int8_t)mem_read8(current_object + 0x80u + r0) < 0)
									{
										r0 = (uint8_t)(r0 + 1u);
									}
									OBJECT(current_object)->car_radio.next_car_number = r0;
									r0 = (uint8_t)(r0 + 9u);
									start_dj_message((uint8_t)r0);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 2:
									r0_2 = (uint8_t)(mem_read8(current_object + 0x80u + OBJECT(current_object)->car_radio.next_car_number) + 1u);
									start_dj_message(r0_2);
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
									r0_2 = 0x2Au;
									start_dj_message(r0_2);
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
									r10 = OBJECT(current_object)->car_radio.remark_list;
									if (r10 == 0u)
									{
										r0 = random_number();
										r0 &= 0x3FFu;
										if ((int32_t)r0 >= 0xE)
											return;
										r10 = routine_lists_1966C[r0];
									}
									r0 = mem_read32(r10);
									r10 += 4u;
									if (mem_read32(r10) == 0u)
										r10 = 0u;
									OBJECT(current_object)->car_radio.remark_list = r10;
									switch (r0)
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
										invalid_jump(r0);
									}
									break;
								case 5:
									return;
								default:
									invalid_jump(rom_0191E0[OBJECT(current_object)->car_radio.talk_step]);
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
	uint8_t r0 = 0;

	r0 = (uint8_t)(car_numbers[RACE(SCREEN(screen)->host_record)->race_order] + 0x11u);
	start_dj_message(r0);
}

/* A DJ remark: message 19 plus this screen's car number. */
void car_radio_dj_own_car(void)
{
	uint8_t r0 = 0;

	r0 = (uint8_t)(OBJECT(current_object)->car_radio.car_number + 0x19u);
	start_dj_message(r0);
}

/* A DJ remark: message 2A (also the end of the cars' introduction). */
void car_radio_dj_remark_2a(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Au;
	start_dj_message(r0);
}

/* A DJ remark: message 2B. */
void car_radio_dj_remark_2b(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Bu;
	start_dj_message(r0);
}

/* A DJ remark: message 2D. */
void car_radio_dj_remark_2d(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Du;
	start_dj_message(r0);
}

/* A DJ remark: message 2F. */
void car_radio_dj_remark_2f(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Fu;
	start_dj_message(r0);
}

/* A DJ remark: message 2C. */
void car_radio_dj_remark_2c(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Cu;
	start_dj_message(r0);
}

/* Shows the title of song R0 (rom_01942C: 0 Magical Sound Shower, 1 Passing Breeze, 2 Splash Wave, 3
   Picture the Rivers, 4 Blow Your Cool, 5 Looking for the Rainbow, 6 Speed King, 7 Adventure, 8 Sonic
   Control, 9 Last Wave, 10 Jingle Bell) and returns the song's sound. */
uint32_t car_radio_song(uint32_t r0)
{
	uint32_t r0_2 = 0, r10 = 0;

	r10 = rom_01942C[r0];
	r0_2 = byte_swap32(r10);
	r10 &= 0xFFFFFFu;
	car_radio_show_title(r10);
	return r0_2;
}

/* Starts a song title (start_song_title) for the record it is given, sends the one shown (title) away
   and keeps the new one in title. */
void car_radio_show_title(uint32_t r10)
{
	uint32_t r21 = 0, object = 0;

	object = OBJECT(current_object)->car_radio.title;
	if (object != 0u)
		OBJECT(object)->song_title.time_left = 0;
	r21 = start_song_title(r10);
	OBJECT(current_object)->car_radio.title = r21;
}

/* Starts an object that shows the song title of record R10 (x, then the text) on the screen and scrolls
   it on the LCD; returns the object. */
uint32_t start_song_title(uint32_t r10)
{
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->handler = song_title_show_ADDRESS;
	OBJECT(object)->song_title.text = r10;
	OBJECT(object)->song_title.time_left = 0x3C;
	return object;
}

/* A song title on the screen (start_song_title starts it, for car_radio, the car select and 00C8B6): its
   x and text from the record (x, and the text, which also scrolls on the LCD through screen
   lcd_scroll_request), then slides in from the right (slide from D2, slowing down to a stop by speed),
   stays for time_left (60 frames) and slides out to the left speeding up; sent away early (time_left 0,
   car_radio_show_title) it leaves at 40 a frame. Drawn each frame with draw_text_at at y C8 in colours
   that cycle every 2 frames (rom_019561 by colour_step). */
void song_title_show(void)
{
	bool L_019387 = false, L_01938A = false;
	uint32_t negative = 0;
	uint16_t r0 = 0, r5_2 = 0, speed = 0;
	uint32_t r3 = 0, r5 = 0, r10 = 0, text = 0;
	uint16_t time_left = 0, t1 = 0;
	uint32_t jump_target = 0;

	do
	{
		jump_target = L_01938A ? 0x19369u : rom_019330[OBJECT(current_object)->state];
		switch (jump_target)
		{
		case 0x019344u:
			r10 = OBJECT(current_object)->song_title.text;
			OBJECT(current_object)->x = ROM16(r10);
			r10 += 2u;
			OBJECT(current_object)->song_title.text = r10;
			SCREEN(screen)->lcd_scroll_request = r10;
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
					t1 = time_left - 1u;
					negative = t1 >> 15;
					OBJECT(current_object)->song_title.time_left = t1;
					if ((int16_t)time_left > 1)
					{
						r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->x);
						r5 = OBJECT(current_object)->song_title.colour_step;
						r5 = (uint8_t)(r5 + 1u);
						OBJECT(current_object)->song_title.colour_step = r5;
						r5 = r5 >> 1;
						r5_2 = rom_019561[r5 & 7u];
						text = OBJECT(current_object)->song_title.text;
						draw_text_at(r3, r5_2, text);
						return;
					}
					if (negative)
					{
						OBJECT(current_object)->state += 2u;
						r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
						OBJECT(current_object)->song_title.slide = (uint16_t)r3;
						OBJECT(current_object)->song_title.y = (uint16_t)(r3 >> 16u);
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
					else
					{
						OBJECT(current_object)->state++;
						r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						r0 = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
						OBJECT(current_object)->song_title.speed = r0;
						OBJECT(current_object)->song_title.slide = (uint16_t)r3;
						OBJECT(current_object)->song_title.y = (uint16_t)(r3 >> 16u);
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
				}
				else
				{
					r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
					speed = OBJECT(current_object)->song_title.speed;
					r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - speed);
					OBJECT(current_object)->song_title.speed = speed - 1u;
					OBJECT(current_object)->song_title.slide = (uint16_t)r3;
					OBJECT(current_object)->song_title.y = (uint16_t)(r3 >> 16u);
					L_019387 = (uint16_t)r3 == 0u;
				}
			} while (L_019387);
			break;
		case 0x01938Au:
			L_01938A = true;
			break;
		case 0x019394u:
			r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			r0 = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
			OBJECT(current_object)->song_title.speed = r0;
			OBJECT(current_object)->song_title.slide = (uint16_t)r3;
			OBJECT(current_object)->song_title.y = (uint16_t)(r3 >> 16u);
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->x);
			r5 = OBJECT(current_object)->song_title.colour_step;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(current_object)->song_title.colour_step = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(r3, r5_2, text);
			return;
		case 0x0193BAu:
			r3 = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
			OBJECT(current_object)->song_title.slide = (uint16_t)r3;
			OBJECT(current_object)->song_title.y = (uint16_t)(r3 >> 16u);
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->x);
			r5 = OBJECT(current_object)->song_title.colour_step;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(current_object)->song_title.colour_step = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(r3, r5_2, text);
			return;
		default:
			invalid_jump(jump_target);
		}
	} while (L_01938A);
	r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->x);
	r5 = OBJECT(current_object)->song_title.colour_step;
	r5 = (uint8_t)(r5 + 1u);
	OBJECT(current_object)->song_title.colour_step = r5;
	r5 = r5 >> 1;
	r5_2 = rom_019561[r5 & 7u];
	text = OBJECT(current_object)->song_title.text;
	draw_text_at(r3, r5_2, text);
}

/* Starts DJ message R0 (start_dj_message) unless one is being said (screen dj_mouth_shapes). */
void start_dj_message_if_quiet(uint8_t r0)
{
	if (SCREEN(screen)->dj_mouth_shapes != 0u)
		return;
	start_dj_message(r0);
}

/* Starts DJ message R0 (typed_messages): requests its voice (the sound in the record's top byte) and has
   the face on the LCD move its mouth by the record's string (screen dj_mouth_shapes). */
void start_dj_message(uint8_t r0)
{
	uint8_t r0_2 = 0;
	uint32_t r10 = 0;

	r10 = dj_messages[(uint8_t)r0];
	r0_2 = (uint8_t)byte_swap32(r10);
	SCREEN(screen)->dj_mouth_shapes = r10 & 0xFFFFFFu;
	request_sound(r0_2);
}

/* The title of the attract mode. By state: 0 sets it up: clears screen 5800 and 5801 (title_logo_shine,
   the logo's shine signal), keeps the addresses of the tilemaps' register copies (01ACCC by screen:
   scroll_x_address, scroll_y_address for the first tilemap, second_scroll_x_address,
   second_scroll_y_address for the second), the background (set_up_background with 01AD0C by screen) and
   its scrolls, picks the car of the next car presentation (screen attract_car from unk_9E, which steps
   through the eight), starts the credits line (credit_display), the prompt (coin_prompt) and the eight
   cars driving past (title_car); 1 for 45 frames (frames): palm trees (title_screen_start_palms) and the
   two tilemaps scrolled by 8 and 20 a frame, then the logo (title_logo, title_logo_copy), three logo
   cars (title_logo_car) and the three lines across the logo (title_logo_stripe); 2 the same palms and
   scrolls, with three more logo cars at each count of wave_times (120, 240, 300, 390, 450). */
void title_screen_update(void)
{
	uint8_t r0 = 0, next_attract_car = 0;
	uint32_t r1 = 0, r19 = 0, object = 0;
	uint8_t t1 = 0;
	uint32_t scroll_x_address = 0, scroll_x_address_2 = 0, second_scroll_x_address = 0, scroll_x_address_3 = 0, second_scroll_x_address_2 = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		SCREEN(screen)->route_map_signals &= 0xFFFFFF00u;
		SCREEN(screen)->title_logo_shine = 0u;
		r19 = 0x1ACCCu + ((uint32_t)SCREEN(screen)->index << 3) * 4u;
		OBJECT(current_object)->title_screen.tile_bank_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.pages_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.scroll_x_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.scroll_y_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.second_tile_bank_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.second_pages_address = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->title_screen.second_scroll_x_address = ROM32(r19);
		OBJECT(current_object)->title_screen.second_scroll_y_address = ROM32(r19 + 4u);
		r19 = 0x1AD0Cu + SCREEN(screen)->index * 5u * 4u;
		set_up_background(r19);
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
		r1 = 8u;
		r0 = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_car_drive_ADDRESS;
			OBJECT(object)->title_car.index = r0;
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->title_screen.frames = 0x2Du;
		OBJECT(current_object)->state++;
		return;
	case 1:
		title_screen_start_palms();
		scroll_x_address_2 = OBJECT(current_object)->title_screen.scroll_x_address;
		mem_write16(scroll_x_address_2, mem_read16(scroll_x_address_2) + 8u);
		second_scroll_x_address = OBJECT(current_object)->title_screen.second_scroll_x_address;
		mem_write16(second_scroll_x_address, mem_read16(second_scroll_x_address) + 0x14u);
		t1 = (uint8_t)OBJECT(current_object)->title_screen.frames;
		OBJECT(current_object)->title_screen.frames = (OBJECT(current_object)->title_screen.frames & 0xFF00u) | (t1 - 1u & 0xFFu);
		if (t1 != 1u)
			return;
		object = start_object();
		OBJECT(object)->handler = title_logo_update_ADDRESS;
		object = start_object();
		OBJECT(object)->handler = title_logo_copy_update_ADDRESS;
		r1 = 3u;
		r0 = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_car_update_ADDRESS;
			OBJECT(object)->title_logo_car.index = r0;
			r0++;
			r1--;
		} while (r1 != 0u);
		r1 = 3u;
		r0 = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_stripe_update_ADDRESS;
			OBJECT(object)->title_logo_stripe.index = r0;
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->title_screen.wave_times = 0x1ACC0u;
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
		if (OBJECT(current_object)->title_screen.frames != ROM16(OBJECT(current_object)->title_screen.wave_times))
			return;
		r1 = 3u;
		r0 = 0u;
		do
		{
			object = start_object();
			OBJECT(object)->handler = title_logo_car_update_ADDRESS;
			OBJECT(object)->title_logo_car.index = r0;
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->title_screen.wave_times += 2u;
		return;
	default:
		invalid_jump(rom_019CB8[OBJECT(current_object)->state]);
	}
}

/* One of the eight cars driving past at the bottom of the title (title_screen starts them, index 0-7).
   Sets itself up from its record (01ABB4 by index: the model, whose animation rom_01AC34 gives, palette,
   width_scale, height_scale, position, y, speed, and its depth_layer less 18F; draw mode A, frame 9) and
   starts its driver (title_car_driver) and passenger (title_car_passenger); each frame drives on by
   speed along a strip 4096 wide that wraps (clearing flags bit 5 as it wraps), is drawn at the position
   less the screen's offset (rom_01ABA4 by screen car_number) and bobs a line up on odd frames (frames;
   rest_y keeps its y meanwhile). */
void title_car_drive(void)
{
	uint16_t r2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, object = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_019E78[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r10 = 0x1ABB4u + ((uint32_t)OBJECT(current_object)->title_car.index << 4);
		r0 = mem_read16(r10);
		r9 = rom_01AC34[r0];
		r10 += 2u;
		animation_start(r9);
		OBJECT(current_object)->palette = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->width_scale = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->height_scale = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->title_car.position = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->y = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->title_car.speed = mem_read16(r10);
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x18Fu;
		OBJECT(current_object)->depth_layer += mem_read16(r10 + 2u);
		object = start_object();
		OBJECT(object)->handler = title_car_passenger_update_ADDRESS;
		OBJECT(object)->title_car_passenger.car = current_object;
		OBJECT(object)->title_car_passenger.model = r0;
		object = start_object();
		OBJECT(object)->handler = title_car_driver_update_ADDRESS;
		OBJECT(object)->title_car_driver.car = current_object;
		OBJECT(object)->title_car_driver.model = r0;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_019E78[state]);
	}
	OBJECT(current_object)->title_car.position += OBJECT(current_object)->title_car.speed;
	if (OBJECT(current_object)->title_car.position >= 0x1000)
	{
		OBJECT(current_object)->flags &= 0xDFu;
		OBJECT(current_object)->title_car.position &= 0xFFFu;
	}
	OBJECT(current_object)->x = OBJECT(current_object)->title_car.position;
	OBJECT(current_object)->x -= rom_01ABA4[SCREEN(screen)->car_number];
	OBJECT(current_object)->title_car.frames++;
	r2 = OBJECT(current_object)->title_car.frames & 1u;
	OBJECT(current_object)->title_car.rest_y = OBJECT(current_object)->y;
	OBJECT(current_object)->y -= r2;
	draw_object(r0);
	OBJECT(current_object)->y = OBJECT(current_object)->title_car.rest_y;
}

/* The driver of a title car (the blond man at the wheel): sets up its picture by the car's model
   (animation rom_01A012, palette rom_01A032, x_offset rom_01A062, y_offset rom_01A072, a layer in front
   of the car), then is drawn with the car each frame (title_rider_draw). */
void title_car_driver_update(void)
{
	uint32_t r9 = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		model = OBJECT(current_object)->title_car_driver.model;
		OBJECT(current_object)->palette = rom_01A032[model];
		OBJECT(current_object)->title_car_driver.x_offset = rom_01A062[model];
		OBJECT(current_object)->title_car_driver.y_offset = rom_01A072[model];
		r9 = rom_01A012[model];
		animation_start(r9);
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = OBJECT(OBJECT(current_object)->title_car_driver.car)->depth_layer;
		OBJECT(current_object)->depth_layer++;
		return;
	case 1:
		title_rider_draw();
		return;
	default:
		invalid_jump(rom_019F58[OBJECT(current_object)->state]);
	}
}

/* The passenger of a title car (the red-haired woman beside the driver): sets up its picture by the
   car's model (animation rom_01A022, palette rom_01A042, x_offset rom_01A082, y_offset rom_01A092, a
   layer in front of the car), then is drawn with the car each frame (title_rider_draw). */
void title_car_passenger_update(void)
{
	uint32_t r9 = 0, model = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		title_rider_draw();
		return;
	default:
		invalid_jump(rom_019F6C[OBJECT(current_object)->state]);
	}
	model = OBJECT(current_object)->title_car_passenger.model;
	OBJECT(current_object)->palette = rom_01A042[model];
	OBJECT(current_object)->title_car_passenger.x_offset = rom_01A082[model];
	OBJECT(current_object)->title_car_passenger.y_offset = rom_01A092[model];
	r9 = rom_01A022[model];
	animation_start(r9);
	OBJECT(current_object)->state++;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->depth_layer = OBJECT(OBJECT(current_object)->title_car_passenger.car)->depth_layer;
	OBJECT(current_object)->depth_layer++;
}

/* Draws a rider of a title car (title_car_driver, title_car_passenger) on its car: at the car's x and y
   plus its x_offset and y_offset, bobbing with the car (bit 0 of the car's frames), at the size
   rom_01A052 gives for the model. */
void title_rider_draw(void)
{
	uint32_t r0 = 0, model = 0;

	model = OBJECT(current_object)->title_rider.model;
	OBJECT(current_object)->width_scale = rom_01A052[model];
	OBJECT(current_object)->height_scale = rom_01A052[model];
	OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->title_rider.car)->x + OBJECT(current_object)->title_rider.x_offset;
	r0 = (uint16_t)(OBJECT(OBJECT(current_object)->title_rider.car)->y + OBJECT(current_object)->title_rider.y_offset - (OBJECT(OBJECT(current_object)->title_rider.car)->title_car.frames & 1u));
	OBJECT(current_object)->y = r0;
	r0 = draw_object(r0);
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
	uint32_t r0 = 0, r9 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A0F0[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x59u;
		animation_start(r9);
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
		invalid_jump(rom_01A0F0[state]);
	}
	if ((int16_t)OBJECT(current_object)->x <= -0x30)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (OBJECT(current_object)->title_palm_tree.near_or_far != 0u)
	{
		OBJECT(current_object)->x -= 0x12u;
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->x -= 0x30u;
	r0 = draw_object(r0);
}

/* The OutRunners logo of the title: slides in from the right (16 a frame for 10 frames); then starts the
   palm trees of its board (title_board_palms), the two seagulls (title_seagull) and the title's still
   pictures (title_picture: the board, SEGA, (C) SEGA 1992), flashes through the palettes at 01AC54
   (palette_list) for 6 frames and settles in palette 269 at its next frame; then shines (its animation
   again from frame 1) each time the first logo car reaches the right edge (screen title_logo_shine). */
void title_logo_update(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, object = 0, palette_list = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A198[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x25Du;
		animation_start(r9);
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
			r0 = draw_object(r0);
			return;
		}
		palette_list = OBJECT(current_object)->title_logo.palette_list;
		OBJECT(current_object)->palette = ROM16(palette_list);
		OBJECT(current_object)->title_logo.palette_list = palette_list + 2u;
		r0 = draw_object(r0);
		return;
	case 3:
		if (SCREEN(screen)->title_logo_shine != 0u)
		{
			OBJECT(current_object)->frame = 1u;
			OBJECT(current_object)->frame_time = 2u;
			OBJECT(current_object)->frame_countdown = 2;
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A198[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo.frames;
	OBJECT(current_object)->title_logo.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		OBJECT(current_object)->x -= 0x10u;
		r0 = draw_object(r0);
		return;
	}
	object = start_object();
	OBJECT(object)->handler = title_board_palms_update_ADDRESS;
	r1 = 2u;
	r0 &= ~0xFFu;
	do
	{
		object = start_object();
		OBJECT(object)->handler = title_seagull_fly_ADDRESS;
		OBJECT(object)->title_seagull.index = r0;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		r1--;
	} while (r1 != 0u);
	r1 = 3u;
	r0 &= ~0xFFu;
	do
	{
		object = start_object();
		OBJECT(object)->handler = title_picture_draw_ADDRESS;
		OBJECT(object)->title_picture.index = r0;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		r1--;
	} while (r1 != 0u);
	OBJECT(current_object)->title_logo.palette_list = 0x1AC54u;
	OBJECT(current_object)->title_logo.frames = 6u;
	OBJECT(current_object)->state++;
	r0 = draw_object(r0);
}

/* A copy of the logo in other colours (cyan) that slides in from the left as the logo (title_logo)
   slides in from the right, meets it, flashes through the palettes at 01AC62 (palette_list) and ends. */
void title_logo_copy_update(void)
{
	uint32_t r0 = 0, r9 = 0, palette_list = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A2A8[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x25Eu;
		animation_start(r9);
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
		OBJECT(current_object)->palette = ROM16(palette_list);
		OBJECT(current_object)->title_logo_copy.palette_list = palette_list + 2u;
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A2A8[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo_copy.frames;
	OBJECT(current_object)->title_logo_copy.frames = frames_2 - 1u;
	if (frames_2 == 1u)
	{
		OBJECT(current_object)->title_logo_copy.palette_list = 0x1AC62u;
		OBJECT(current_object)->title_logo_copy.frames = 6u;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->x += 0x10u;
	r0 = draw_object(r0);
}

/* A small car racing across the logo along one of its lines (title_screen starts three at a time, index
   0-2): after 1, 5 or 9 frames by index, one of the eight cars at random (rom_01AC34, rom_01AC44) at a
   quarter of its size at the left edge; 16 a frame to the right edge; the first of the three then makes
   the logo, its board's palm trees and the lines shine (screen title_logo_shine for a frame), and it
   ends. */
void title_logo_car_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t delay = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A334[state];
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
		r0 = random_number();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		animation_start(r9);
		OBJECT(current_object)->palette = rom_01AC44[r0];
		r0 = (uint32_t)OBJECT(current_object)->title_logo_car.index << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(current_object)->x = ROM16(r10);
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Au;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 1:
		delay = OBJECT(current_object)->title_logo_car.delay;
		OBJECT(current_object)->title_logo_car.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(current_object)->draw_mode = 0xAu;
		r0 = random_number();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		animation_start(r9);
		OBJECT(current_object)->palette = rom_01AC44[r0];
		r0 = (uint32_t)OBJECT(current_object)->title_logo_car.index << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(current_object)->x = ROM16(r10);
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		OBJECT(current_object)->width_scale = 0x40u;
		OBJECT(current_object)->height_scale = 0x40u;
		OBJECT(current_object)->frame = 9u;
		OBJECT(current_object)->depth_layer = 0x19Au;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 2:
		OBJECT(current_object)->x += 0x10u;
		if ((int16_t)OBJECT(current_object)->x <= 0x148)
		{
			r0 = draw_object(r0);
			return;
		}
		if (OBJECT(current_object)->title_logo_car.index == 0u)
			SCREEN(screen)->title_logo_shine = 1u;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	case 3:
		SCREEN(screen)->title_logo_shine = 0u;
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_01A334[state]);
	}
}

/* One of the three thin coloured lines across the logo that the logo cars race along (index 0-2): after
   1, 5 or 9 frames by index it slides in from the left (16 a frame for 19 frames), then shines with the
   logo (its animation from frame 0 when screen title_logo_shine is set). */
void title_logo_stripe_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t frames = 0, frames_2 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A3F4[state];
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
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->x += 0x10u;
		r0 = draw_object(r0);
		return;
	case 3:
		if (SCREEN(screen)->title_logo_shine != 0u)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->frame_time = 2u;
			OBJECT(current_object)->frame_countdown = 2;
		}
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A3F4[state]);
	}
	frames_2 = OBJECT(current_object)->title_logo_stripe.frames;
	OBJECT(current_object)->title_logo_stripe.frames = frames_2 - 1u;
	if (frames_2 != 1u)
		return;
	OBJECT(current_object)->draw_mode = 0xAu;
	r9 = 0x25Fu;
	animation_start(r9);
	OBJECT(current_object)->palette = 0x270u;
	r0 = (uint32_t)OBJECT(current_object)->title_logo_stripe.index << 2;
	r10 = 0x1ACA8u + r0;
	OBJECT(current_object)->x = ROM16(r10);
	OBJECT(current_object)->y = ROM16(r10 + 2u);
	OBJECT(current_object)->depth_layer = 0x198u;
	OBJECT(current_object)->title_logo_stripe.frames = 0x14u;
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->title_logo_stripe.frames;
	OBJECT(current_object)->title_logo_stripe.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->x += 0x10u;
	r0 = draw_object(r0);
}

/* The palm trees at the right end of the logo's board: grow in from 3/4 of their size (8 frames), then
   sway each time the logo shines (screen title_logo_shine). */
void title_board_palms_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A4A4[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x25Au;
		animation_start(r9);
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
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A4A4[state]);
	}
	frames = OBJECT(current_object)->title_board_palms.frames;
	OBJECT(current_object)->title_board_palms.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->width_scale += 8u;
	OBJECT(current_object)->height_scale += 8u;
	r0 = draw_object(r0);
}

/* One of the two seagulls over the title (index 0, 1): flies up out of the logo for 8 frames growing to
   full size, then circles about that place in a figure of eight (the sine of x_angle, a turn in 128
   frames, and of y_angle, twice as fast), flapping (its animation). */
void title_seagull_fly(void)
{
	uint16_t r3 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t frames = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A544[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		r9 = 0x25Bu;
		animation_start(r9);
		OBJECT(current_object)->palette = 0x272u;
		OBJECT(current_object)->width_scale = 0xC0u;
		OBJECT(current_object)->height_scale = 0xC0u;
		r0 = (uint32_t)OBJECT(current_object)->title_seagull.index << 2;
		r10 = 0x1ACA0u + r0;
		OBJECT(current_object)->x = ROM16(r10);
		OBJECT(current_object)->y = ROM16(r10 + 2u);
		OBJECT(current_object)->depth_layer = 0x198u;
		OBJECT(current_object)->title_seagull.frames = 9u;
		OBJECT(current_object)->state++;
		frames = OBJECT(current_object)->title_seagull.frames;
		OBJECT(current_object)->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			r0 = (uint16_t)((uint32_t)OBJECT(current_object)->title_seagull.index << 11);
			OBJECT(current_object)->title_seagull.x_angle = r0;
			OBJECT(current_object)->title_seagull.y_angle = r0;
			OBJECT(current_object)->title_seagull.centre_x = OBJECT(current_object)->x;
			OBJECT(current_object)->title_seagull.centre_y = OBJECT(current_object)->y;
			OBJECT(current_object)->state++;
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->width_scale += 8u;
		OBJECT(current_object)->height_scale += 8u;
		OBJECT(current_object)->x--;
		OBJECT(current_object)->y -= 5u;
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	case 1:
		frames = OBJECT(current_object)->title_seagull.frames;
		OBJECT(current_object)->title_seagull.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			r0 = (uint16_t)((uint32_t)OBJECT(current_object)->title_seagull.index << 11);
			OBJECT(current_object)->title_seagull.x_angle = r0;
			OBJECT(current_object)->title_seagull.y_angle = r0;
			OBJECT(current_object)->title_seagull.centre_x = OBJECT(current_object)->x;
			OBJECT(current_object)->title_seagull.centre_y = OBJECT(current_object)->y;
			OBJECT(current_object)->state++;
			animation_advance(0u);
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->width_scale += 8u;
		OBJECT(current_object)->height_scale += 8u;
		OBJECT(current_object)->x--;
		OBJECT(current_object)->y -= 5u;
		animation_advance(0u);
		r0 = draw_object(r0);
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
		r0 = OBJECT(current_object)->title_seagull.x_angle & 0xFFFu;
		r3 = (uint16_t)((int16_t)rom_100C00[(uint32_t)OBJECT(current_object)->title_seagull.y_angle] >> 12);
		OBJECT(current_object)->x += (int16_t)rom_100C00[r0] >> 9;
		OBJECT(current_object)->y += r3;
		animation_advance(0u);
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A544[state]);
	}
}

/* One of the title's still pictures (title_logo starts them): by index, from its record at 01AC70
   (animation, palette, width_scale, height_scale, x, y, depth_layer): 0 (C) SEGA 1992, 1 the oval board
   behind the logo, 2 the SEGA logo. */
void title_picture_draw(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A680[state];
	switch (state)
	{
	case 1:
		r0 = draw_object(r0);
		return;
	case 0:
		break;
	default:
		invalid_jump(rom_01A680[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	r0 = (uint32_t)OBJECT(current_object)->title_picture.index << 4;
	r10 = 0x1AC70u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	animation_start(r9);
	OBJECT(current_object)->palette = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->width_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->height_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->x = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->y = mem_read16(r10);
	OBJECT(current_object)->depth_layer = mem_read16(r10 + 2u);
	OBJECT(current_object)->state++;
	r0 = draw_object(r0);
}

/* The prompt at the bottom of the title, the car presentations and the demo races, and its text on the
   cabinet's LCD; ends at once in game mode 6 (screen game_mode). Every 30 frames (round_frames): a
   little car crosses the message (coin_prompt_car with this round's car; next_car then steps through the
   eight, from screen car_number) and the message by the seat (seat1_credits and seat1_coin_units for
   screen 1, seat2_credits and seat2_coin_units for screen 2): with free play (free_play) or a credit
   message 2, TURN THE IGNITION KEY (coin_prompt_start_key_prompt); else message 1 with coins in toward a
   credit or 0, a coin_prompt_message in the car's colour (INSERT COIN(S)). Each frame the LCD's text to
   scroll (screen lcd_scroll_request) is the message's (rom_01A9BC: Insert Coin(s), Insert More Coin(s),
   Turn the Ignition Key), except in game modes 6 and 7 or while a text scrolls (unk_F44). */
void coin_prompt_update(void)
{
	uint8_t r0 = 0, r2 = 0, r10 = 0, r11 = 0;
	uint32_t r1 = 0, object = 0;
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
				SCREEN(screen)->lcd_scroll_request = rom_01A9BC[OBJECT(current_object)->coin_prompt.message];
			return;
		}
		break;
	default:
		invalid_jump(rom_01A6DC[OBJECT(current_object)->state]);
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
		r0 = 2u;
		r1 = 2u;
		r2 = 0u;
		object = coin_prompt_start_key_prompt(r1, r2);
	}
	else
	{
		switch (SCREEN(screen)->index)
		{
		case 0:
			r10 = seat1_credits;
			r11 = seat1_coin_units;
			break;
		case 1:
			r10 = seat2_credits;
			r11 = seat2_coin_units;
			break;
		default:
			invalid_jump(rom_01A744[SCREEN(screen)->index]);
		}
		if (r10 != 0u)
		{
			r0 = 2u;
			r1 = 2u;
			r2 = 0u;
			object = coin_prompt_start_key_prompt(r1, r2);
			OBJECT(current_object)->coin_prompt.message = r0;
			OBJECT(object)->unk_0B = OBJECT(current_object)->coin_prompt.car;
			if (SCREEN(screen)->game_mode != 6u && SCREEN(screen)->game_mode != 7u && SCREEN(screen)->lcd_scroll == 0u)
				SCREEN(screen)->lcd_scroll_request = rom_01A9BC[OBJECT(current_object)->coin_prompt.message];
			return;
		}
		if (r11 != 0u)
			r0 = 1u;
		else
			r0 = 0u;
		object = start_object();
		OBJECT(object)->handler = coin_prompt_message_update_ADDRESS;
		OBJECT(object)->coin_prompt_message.prompt = current_object;
		OBJECT(object)->coin_prompt_message.message = r0;
	}
	OBJECT(current_object)->coin_prompt.message = r0;
	OBJECT(object)->unk_0B = OBJECT(current_object)->coin_prompt.car;
	if (SCREEN(screen)->game_mode != 6u && SCREEN(screen)->game_mode != 7u && SCREEN(screen)->lcd_scroll == 0u)
		SCREEN(screen)->lcd_scroll_request = rom_01A9BC[OBJECT(current_object)->coin_prompt.message];
}

/* The little car that drives across the prompt from right to left each round (coin_prompt starts it):
   one of the eight cars (car) at a quarter of its size, 14 a frame, ending at the left edge. */
void coin_prompt_car_drive(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t t1 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A7D0[state];
	switch (state)
	{
	case 0:
		r9 = rom_01A986[OBJECT(current_object)->coin_prompt_car.car];
		animation_start(r9);
		r0 = (uint16_t)(rom_01A996[OBJECT(current_object)->coin_prompt_car.car_colour] + 4u);
		OBJECT(current_object)->palette = r0;
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
		invalid_jump(rom_01A7D0[state]);
	}
	t1 = OBJECT(current_object)->x - 0xEu;
	OBJECT(current_object)->x = t1;
	if ((int16_t)t1 >= 0)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* The prompt's message for a round (coin_prompt starts one every 30 frames): after 6 frames, INSERT
   COIN(S) (message 0; 1 when coins are in, animation rom_01A9A6) in the colour of the round's car
   (rom_01A9AC by car_colour), animating in for 22 frames over the one before, then a layer back for 30
   frames, and ends; ends at once when the prompt's message changes. */
void coin_prompt_message_update(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t frames = 0, frames_2 = 0, frames_3 = 0;
	uint32_t state = 0;

	if (OBJECT(OBJECT(current_object)->coin_prompt_message.prompt)->coin_prompt.message != OBJECT(current_object)->coin_prompt_message.message)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	state = OBJECT(current_object)->state;
	r0 = rom_01A84C[state];
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
		r9 = rom_01A9A6[OBJECT(current_object)->coin_prompt_message.message];
		animation_start(r9);
		r0 = OBJECT(current_object)->coin_prompt_message.car_colour;
		OBJECT(current_object)->palette = rom_01A9AC[r0];
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
		r9 = rom_01A9A6[OBJECT(current_object)->coin_prompt_message.message];
		animation_start(r9);
		r0 = OBJECT(current_object)->coin_prompt_message.car_colour;
		OBJECT(current_object)->palette = rom_01A9AC[r0];
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
			r0 = draw_object(r0);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	default:
		invalid_jump(rom_01A84C[state]);
	}
	animation_advance(0u);
	frames_3 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_3 - 1u;
	if (frames_3 != 1u)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->depth_layer--;
	OBJECT(current_object)->coin_prompt_message.frames = 0x1Eu;
	OBJECT(current_object)->state++;
	frames_2 = OBJECT(current_object)->coin_prompt_message.frames;
	OBJECT(current_object)->coin_prompt_message.frames = frames_2 - 1u;
	if (frames_2 != 1u)
	{
		r0 = draw_object(r0);
		return;
	}
	OBJECT(current_object)->flags = 0u;
}

/* Starts R1 pieces of the ignition key prompt (ignition_key_prompt), pieces R2 on, and returns the last
   one. */
uint32_t coin_prompt_start_key_prompt(uint32_t r1, uint8_t r2)
{
	uint32_t object = 0;

	do
	{
		object = start_object();
		OBJECT(object)->handler = ignition_key_prompt_update_ADDRESS;
		OBJECT(object)->ignition_key_prompt.piece = r2;
		r2++;
		r1--;
	} while (r1 != 0u);
	return object;
}

/* A piece of the prompt when a game can start (coin_prompt starts both every round): by piece, from its
   record at 01A95B (animation, palette, width_scale, height_scale, x, y, depth_layer, frames_left): 0
   the words TURN THE IGNITION KEY, 1 the START / IGNITION key with its arrow; animates until frames_left
   runs out (24 or 30 frames) and ends. */
void ignition_key_prompt_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01A8F0[state];
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
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01A8F0[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	r0 = (uint32_t)OBJECT(current_object)->ignition_key_prompt.piece << 4;
	r10 = 0x1A95Bu + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	animation_start(r9);
	OBJECT(current_object)->palette = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->width_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->height_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->x = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->y = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->depth_layer = mem_read16(r10);
	OBJECT(current_object)->ignition_key_prompt.frames_left = mem_read16(r10 + 2u);
	OBJECT(current_object)->state++;
	animation_advance(0u);
	r0 = draw_object(r0);
}

/* The credits line under the prompt (title, car presentations, demo races); ends in game mode 6. With
   free play (free_play) FREE PLAY; otherwise, for the screen's seat (seat1_credits and seat1_coin_units
   for screen 1, seat2_credits and seat2_coin_units for screen 2): Credit (0 or 1) or Credits and the
   number of credits in the font's digits (tens at x AC when there are, units at B4), and while coins are
   in toward the next credit the coins and the coins a credit costs (coins_per_credit) as a fraction (C0
   and C8), each drawn with credit_display_digit. */
void credit_display_update(void)
{
	uint8_t r10_2 = 0, r11_2 = 0;
	uint32_t r10 = 0, r11 = 0, r19 = 0, ignored = 0;
	uint32_t stack_10 = 0, stack_14 = 0;

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
			r19 = 0x1AB62u;
			ignored = draw_text(r19);
			return;
		}
		OBJECT(current_object)->state += 2u;
		break;
	case 1:
		r19 = 0x1AB62u;
		ignored = draw_text(r19);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_01AA0C[OBJECT(current_object)->state]);
	}
	switch (SCREEN(screen)->index)
	{
	case 0:
		r10 = seat1_credits;
		r11 = seat1_coin_units;
		break;
	case 1:
		r10 = seat2_credits;
		r11 = seat2_coin_units;
		break;
	default:
		invalid_jump(rom_01AA4C[SCREEN(screen)->index]);
	}
	if (r10 < 2u)
		r19 = 0x1AB72u;
	else
		r19 = 0x1AB80u;
	stack_10 = r10;
	stack_14 = r11;
	draw_text(r19);
	r11_2 = (uint8_t)stack_14;
	r10_2 = (uint8_t)stack_10;
	r12 = r12 & ~0xFFu;
	OBJECT(current_object)->y = 0xCFu;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	while ((int8_t)r10_2 >= 0xA)
	{
		r10_2 -= 0xAu;
		r12 = (r12 & ~0xFFu) | (uint8_t)(r12 + 1u);
	}
	if ((uint8_t)r12 != 0u)
	{
		OBJECT(current_object)->credit_display.digit_kind = 0u;
		OBJECT(current_object)->credit_display.digit = rom_01AB8E[(uint8_t)r12];
		OBJECT(current_object)->x = 0xACu;
		credit_display_digit();
	}
	OBJECT(current_object)->credit_display.digit_kind = 0u;
	OBJECT(current_object)->credit_display.digit = rom_01AB8E[(uint8_t)r10_2];
	OBJECT(current_object)->x = 0xB4u;
	credit_display_digit();
	if ((uint8_t)r11_2 != 0u)
	{
		OBJECT(current_object)->credit_display.digit_kind = 1u;
		OBJECT(current_object)->credit_display.digit = r11_2;
		OBJECT(current_object)->x = 0xC0u;
		credit_display_digit();
		OBJECT(current_object)->credit_display.digit_kind = 2u;
		OBJECT(current_object)->credit_display.digit = coins_per_credit;
		OBJECT(current_object)->x = 0xC8u;
		credit_display_digit();
	}
}

/* Draws one digit of the credits line at x and y: digit_kind picks the animation and palette
   (rom_01AB98, rom_01AB9E: 0 the font, whose frames are its characters, 1 the coins, 2 the coins a
   credit costs) and digit the frame. */
void credit_display_digit(void)
{
	uint32_t r0 = 0, r9 = 0;

	OBJECT(current_object)->draw_mode = 0u;
	r0 = OBJECT(current_object)->credit_display.digit_kind;
	r9 = rom_01AB98[r0];
	animation_start(r9);
	OBJECT(current_object)->palette = rom_01AB9E[r0];
	OBJECT(current_object)->frame = OBJECT(current_object)->credit_display.digit;
	draw_object(r0);
}

/* The captions over the demo race of the attract mode: counts its frames and runs the routines of
   timeline_1AFC0 as it reaches their frames (30 demo_captions_head_to_head, 225
   demo_captions_checkpoint, 420 demo_captions_music_select); ends once the count passes a record's frame
   (the closing FFFF). */
void demo_captions_run(void)
{
	uint32_t zero = 0;
	uint32_t r10 = 0;
	uint32_t t1 = 0, jump_target = 0;

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
		invalid_jump(rom_01ADF4[OBJECT(current_object)->state]);
	}
	OBJECT(current_object)->demo_captions.frames = (OBJECT(current_object)->demo_captions.frames & 0xFFFF0000u) | ((uint16_t)OBJECT(current_object)->demo_captions.frames + 1u & 0xFFFFu);
	r10 = OBJECT(current_object)->demo_captions.timeline;
	t1 = ROM32(r10) - (uint16_t)OBJECT(current_object)->demo_captions.frames;
	zero = t1 == 0u;
	r10 += 4u;
	if ((int32_t)t1 < 0)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (zero)
	{
		jump_target = ROM32(r10);
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
	uint8_t r0 = 0;
	uint32_t r1 = 0, object = 0;

	r1 = 2u;
	r0 = 0u;
	demo_captions_start(r0, r1);
	r0 = 3u;
	if (stand_alone == 0u)
		r0 += machine_count;
	else
		r0++;
	object = start_object();
	OBJECT(object)->handler = demo_caption_update_ADDRESS;
	OBJECT(object)->demo_caption.record = r0;
}

/* Frame 225 of the demo captions: AT THE CHECKPOINT ONLY THE RACE LEADER CAN SELECT THE NEXT STAGE !
   (demo_caption records 2 and 3). */
void demo_captions_checkpoint(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0;

	r1 = 2u;
	r0 = 2u;
	demo_captions_start(r0, r1);
}

/* Frame 420 of the demo captions: the radio's panel, the hand pressing its button and MUSIC SELECT
   (demo_caption records 8-11). */
void demo_captions_music_select(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0;

	r1 = 4u;
	r0 = 8u;
	demo_captions_start(r0, r1);
}

/* Starts R1 demo_caption objects with the records from R0 on. */
void demo_captions_start(uint8_t r0, uint32_t r1)
{
	uint32_t object = 0;

	do
	{
		object = start_object();
		OBJECT(object)->handler = demo_caption_update_ADDRESS;
		OBJECT(object)->demo_caption.record = r0;
		r0++;
		r1--;
	} while (r1 != 0u);
}

/* One picture of the demo captions (demo_captions_start starts them): from its record at 01AF00
   (animation, palette, width_scale, height_scale, x, y, depth_layer, frames_left: 180 frames for records
   0-7, 240 for 8-11), animated and drawn until frames_left runs out. */
void demo_caption_update(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t frames_left = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	r0 = rom_01AE98[state];
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
		r0 = draw_object(r0);
		return;
	default:
		invalid_jump(rom_01AE98[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	r0 = (uint32_t)OBJECT(current_object)->demo_caption.record << 4;
	r10 = 0x1AF00u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	animation_start(r9);
	OBJECT(current_object)->palette = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->width_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->height_scale = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->x = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->y = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->depth_layer = mem_read16(r10);
	OBJECT(current_object)->demo_caption.frames_left = mem_read16(r10 + 2u);
	OBJECT(current_object)->state++;
	animation_advance(0u);
	r0 = draw_object(r0);
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
	uint16_t r4 = 0;
	uint32_t r3 = 0, r4_2 = 0, r5 = 0, r10 = 0, r11 = 0;
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
			r4 = (uint16_t)(SCREEN(screen)->kanji_text_units + SCREEN(screen)->kanji_text_tens * 0xAu + SCREEN(screen)->kanji_text_hundreds * 0x64u);
			SCREEN(screen)->kanji_text_number = r4;
			r3 = TEXT_CELL(5, 10);
			kanji_text_viewer_draw_number(r3, r4);
			OBJECT(current_object)->kanji_text_viewer.blink_count++;
			if ((OBJECT(current_object)->kanji_text_viewer.blink_count & 3u) == 0u)
				mem_write16(TEXT_CELL(0, 10) + OBJECT(current_object)->x, 0x602Au);
		}
		if (SCREEN(screen)->index != 0u)
			return;
		r4_2 = 0u;
		r5 = 0u;
		r10 = rom_01B254[SCREEN(screen)->kanji_text_number];
		do
		{
			r11 = VIDEO_RAM + 0x1040u + r5 * 0x1000u;
			kanji_text_viewer_draw_line(r11, &r4_2, &r10);
			r5++;
		} while (r4_2 == 0u);
		return;
	case 2:
		if (SCREEN(screen)->index != 0u)
			return;
		r4_2 = 0u;
		r5 = 0u;
		r10 = rom_01B254[SCREEN(screen)->kanji_text_number];
		do
		{
			r11 = VIDEO_RAM + 0x1040u + r5 * 0x1000u;
			kanji_text_viewer_draw_line(r11, &r4_2, &r10);
			r5++;
		} while (r4_2 == 0u);
		return;
	case 3:
		return;
	default:
		invalid_jump(rom_01AFE8[OBJECT(current_object)->state]);
	}
}

/* Only unused code reaches it (kanji_text_viewer_update): draws a line of the text at R10 on the bitmap
   layer from R11: each halfword a character (kanji_text_viewer_character_address,
   kanji_text_viewer_draw_character, 16 pixels on); 0 ends the line, a negative one the text (R4 = -1). */
void kanji_text_viewer_draw_line(uint32_t r11, uint32_t *p_r4, uint32_t *p_r10)
{
	uint32_t r13 = 0;

	for (;;)
	{
		r13 = (int16_t)mem_read16(*p_r10);
		*p_r10 += 2u;
		if ((int32_t)r13 < 0)
			break;
		if (r13 == 0u)
		{
			return;
		}
		r13 = kanji_text_viewer_character_address(r13);
		kanji_text_viewer_draw_character(r11, r13);
		r11 += 8u;
	}
	*p_r4 = 0xFFFFFFFFu;
}

/* Only unused code reaches it (kanji_text_viewer_draw_line): draws a 16x16 character on the bitmap layer
   at R11 from its 32 bytes at R13: four 8x8 quarters (kanji_text_viewer_draw_quarter), upper left, upper
   right, lower left, lower right. */
void kanji_text_viewer_draw_character(uint32_t r11, uint32_t r13)
{
	r12 = r11;
	r13 = kanji_text_viewer_draw_quarter(r13);
	r12 = r11 + 4u;
	r13 = kanji_text_viewer_draw_quarter(r13);
	r12 = r11 + 0x800u;
	r13 = kanji_text_viewer_draw_quarter(r13);
	r12 = r11 + 0x804u;
	r13 = kanji_text_viewer_draw_quarter(r13);
}

/* Only unused code reaches it (kanji_text_viewer_draw_character): draws eight rows of eight one-bit
   pixels (a byte each, from R13) at R12 on the bitmap layer, 100 bytes a row, each byte spread by the
   table at 1A6340 into eight 4-bit pixels of pen 1. */
uint32_t kanji_text_viewer_draw_quarter(uint32_t r13)
{
	uint32_t r1 = 0;

	r1 = 8u;
	do
	{
		mem_write32(r12, byte_swap32(rom_1A6340[mem_read8(r13)]));
		r12 = r12 + 0x100u;
		r1--;
		r13++;
	} while (r1 != 0u);
	return r13;
}

/* Only unused code reaches it (kanji_text_viewer_draw_line): the address of character R13 of the kanji
   font at 1CF000 (8 bytes a step: a character is 32). */
uint32_t kanji_text_viewer_character_address(uint32_t r13)
{
	r13 = 0x1CF000u + r13 * 2u * 4u;
	return r13;
}

/* Only unused code reaches it (kanji_text_viewer_update): writes R4 (0-999) as three decimal digits on
   the text layer from cell R3 (attribute 60). */
void kanji_text_viewer_draw_number(uint32_t r3, uint16_t r4)
{
	uint16_t r5 = 0, r6 = 0;

	r5 = 0u;
	r6 = 0u;
	while ((int16_t)r4 >= 0x64)
	{
		r4 -= 0x64u;
		r6++;
	}
	while ((int16_t)r4 >= 0xA)
	{
		r4 -= 0xAu;
		r5++;
	}
	mem_write16(r3, r6 + 0x6030u);
	r3 += 2u;
	mem_write16(r3, r5 + 0x6030u);
	mem_write16(r3 + 2u, r4 + 0x6030u);
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
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;
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
	r0 = 0x1Fu;
	test_mode_queue_sound(r0);
	test_mode_queue_sound_screen_2(r0);
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
	r0_2 = rom_01C378;
	r1 = rom_01C37C;
	AS32(SCREEN(SCREEN_1)->lcd_text) = r0_2;
	SCREEN(SCREEN_1)->lcd_text_4_to_7 = r1;
	AS32(SCREEN(SCREEN_2)->lcd_text) = r0_2;
	SCREEN(SCREEN_2)->lcd_text_4_to_7 = r1;
	r0_2 = rom_01C380;
	r1 = rom_01C384;
	SCREEN(SCREEN_1)->lcd_text_8_to_11 = r0_2;
	SCREEN(SCREEN_1)->lcd_text_12_to_15 = r1;
	SCREEN(SCREEN_2)->lcd_text_8_to_11 = r0_2;
	SCREEN(SCREEN_2)->lcd_text_12_to_15 = r1;
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
	uint32_t r0 = 0, r10 = 0;

	if (test_mode_cursor == 0u)
	{
		test_mode_routine = test_mode_menu_run_ADDRESS;
		test_mode_menu_run();
		return;
	}
	if ((test_mode_flags & 1u) != 0u)
		test_mode_cursor++;
	r0 = test_mode_clear_text();
	r0 = test_mode_menu_show_write_count(r0);
	r10 = 0x1C490u;
	test_mode_draw_texts(r0, r10);
	carry = (test_mode_flags & 1u) != 0u;
	r0 = carry;
	r10 = rom_01C560[r0];
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_draw_instructions(r0);
	r10 = 0x310000u | rom_01C588[test_mode_cursor];
	test_mode_set_cursor(r0, r10);
	test_mode_routine = test_mode_menu_run_ADDRESS;
	test_mode_menu_run();
}

/* The menu, each frame: SERVICE moves the cursor (test_mode_cursor) down, from EXIT back to the heading;
   TEST starts the item's page (rom_01C45C: test_mode_menu_toggle_continue, test_mode_memory_test,
   test_mode_volume_adjustment, test_mode_input_test, test_mode_output_test, test_mode_sound_test,
   test_mode_crt_test, test_mode_game_assignments, test_mode_coin_assignments, then the network
   assignments, bookkeeping, backup data clear and exit pages, test_mode_network_assignments_start,
   test_mode_bookkeeping_start, test_mode_backup_data_clear_start, test_mode_exit); the cursor's arrow
   blinks (test_mode_blink_address, test_mode_blink_timer: 11 frames shown, 6 hidden). */
void test_mode_menu_run(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_cursor;
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0xD)
			r0 = 0u;
		test_mode_cursor = r0;
		r10 = 0x310000u | rom_01C588[r0];
		test_mode_set_cursor(r0, r10);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = rom_01C45C[test_mode_cursor];
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* While SERVICE is held as the menu is drawn: E2PROM WRITE COUNT and the count (eeprom_write_count, 5
   digits) at the bottom of the menu. */
uint32_t test_mode_menu_show_write_count(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r10 = 0;

	if ((test_mode_buttons & 1u) == 0u)
		return r0;
	r10 = 0x1C5A2u;
	test_mode_draw_texts(r0, r10);
	r10 = TEXT_CELL(27, 26);
	r0_2 = eeprom_write_count;
	r1 = 0x64u;
	r2 = 5u;
	test_mode_draw_number(r2, &r0_2, &r1, &r10);
	return r0_2;
}

/* The menu's heading: TEST switches between INDIVIDUAL and CONTINUE (test_mode_flags bit 0) and redraws
   it, then back to test_mode_menu. In CONTINUE mode each return to the menu moves the cursor to the next
   item, so the pages run one after another with TEST alone. */
void test_mode_menu_toggle_continue(void)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r10 = 0;
	uint32_t t1 = 0, test_mode_flags_long = 0;

	t1 = (test_mode_flags | ((uint32_t)test_mode_cursor << 8u) | ((uint32_t)test_mode_page_cursor << 16u) | ((uint32_t)(test_mode_routine & 0xFFu) << 24u));
	carry = (t1 & 1u) != 0u;
	test_mode_flags_long = t1 ^ 1u;
	test_mode_flags = (uint8_t)test_mode_flags_long;
	test_mode_cursor = (uint8_t)(test_mode_flags_long >> 8u);
	test_mode_page_cursor = (uint8_t)(test_mode_flags_long >> 16u);
	test_mode_routine = (test_mode_routine & 0xFFFFFF00u) | ((test_mode_flags_long >> 24u) & 0xFFu);
	r0 = 0u - (0xFFFFFFFFu + carry);
	r10 = rom_01C560[r0];
	test_mode_draw_texts(r0, r10);
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
	uint8_t r1_2 = 0, r26 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t3 = 0, t4 = 0, t6 = 0, t7 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0, t8 = 0, t9 = 0, t10 = 0;
	uint32_t t11 = 0, t12 = 0;

	interrupts_off();
	r0 = test_mode_clear_text();
	mem_write8(SOUND_701FFF, 1u);
	current_object = memory_test_results_ADDRESS;
	r26 = 0u;
	set_bytes(memory_test_results_ADDRESS, 0x19u, r26);
	OBJECT(current_object)->flags = 1u;
	OBJECT(current_object)->flags_high = 1u;
	test_mode_memory_test_draw(r0);
	r10 = 0u;
	r0 = rom_03FFF8_ADDRESS;
	test_mode_memory_test_checksum(r10, &r0, &r1);
	t1 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFF8 != t1)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	OBJECT(current_object)->flags = (uint8_t)r0_2;
	r0 = (r0_2 & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (rom_03FFFA != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	OBJECT(current_object - 0x1u)->flags_high = r0;
	current_object = current_object + 1u;
	OBJECT(current_object - 0x2u)->unk_02 = 0x101u;
	test_mode_memory_test_draw(r0);
	r10 = 0x100000u;
	r0 = 0xFF000u;
	test_mode_memory_test_checksum(r10, &r0, &r1);
	t2 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFFC != t2)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	OBJECT(current_object - 0x2u)->unk_02 = (OBJECT(current_object - 0x2u)->unk_02 & 0xFF00u) | (r0_2 & 0xFFu);
	r0 = (r0_2 & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (rom_03FFFE != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	OBJECT(current_object - 0x3u)->unk_02 = (OBJECT(current_object - 0x3u)->unk_02 & 0xFFu) | ((r0 & 0xFFu) << 8u);
	current_object = current_object + 1u;
	OBJECT(current_object - 0x4u)->handler = 0x1010101u;
	test_mode_memory_test_draw(r0);
	r10 = 0x300000u;
	r0 = 0x20000u;
	r0 = test_mode_memory_test_nibbles(r0, r10);
	t3 = r0 & 0xFu;
	r1_2 = 2u;
	if (t3 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x4u)->handler = (OBJECT(current_object - 0x4u)->handler & 0xFFFFFF00u) | (r1_2 & 0xFFu);
	t4 = r0 & 0xF0u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t4 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x5u)->handler = (OBJECT(current_object - 0x5u)->handler & 0xFFFF00FFu) | ((r1_2 & 0xFFu) << 8u);
	t5 = r0 & 0xF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t5 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x6u)->handler = (OBJECT(current_object - 0x6u)->handler & 0xFF00FFFFu) | ((r1_2 & 0xFFu) << 16u);
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xF000u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x7u)->handler = (OBJECT(current_object - 0x7u)->handler & 0xFFFFFFu) | ((r1_2 & 0xFFu) << 24u);
	current_object = current_object + 1u;
	r0 = init_video();
	OBJECT(current_object - 0x8u)->slot = 1u;
	OBJECT(current_object - 0x8u)->unk_09 = 1u;
	OBJECT(current_object - 0x8u)->unk_0A = 1u;
	OBJECT(current_object - 0x8u)->unk_0B = 1u;
	test_mode_memory_test_draw(r0);
	r10 = 0x400000u;
	r0 = 0x20000u;
	r0 = test_mode_memory_test_nibbles(r0, r10);
	t6 = r0 & 0xFu;
	r1_2 = 2u;
	if (t6 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x8u)->slot = r1_2;
	t7 = r0 & 0xF0u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t7 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x9u)->unk_09 = r1_2;
	t8 = r0 & 0xF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t8 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xAu)->unk_0A = r1_2;
	t9 = r0 & 0xF000u;
	r0 = t9;
	r1_2 = 2u;
	current_object = current_object + 1u;
	OBJECT(current_object - 0xBu)->unk_0B = r1_2;
	current_object = current_object + 1u;
	init_sprite_lists();
	OBJECT(current_object - 0xCu)->state = 1u;
	OBJECT(current_object - 0xCu)->unk_0D = 1u;
	test_mode_memory_test_draw(r0);
	r10 = 0x600000u;
	r0 = 0x4000u;
	r0 = test_mode_memory_test_halfwords(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xCu)->state = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xDu)->unk_0D = r1_2;
	current_object = current_object + 1u;
	r0 = test_mode_set_up_colours();
	OBJECT(current_object - 0xEu)->unk_0E = 1u;
	OBJECT(current_object - 0xEu)->unk_0F = 1u;
	test_mode_memory_test_draw(r0);
	r10 = PALETTE_RAM + 0x4000u;
	r0 = 0x4000u;
	r0 = test_mode_memory_test_halfwords(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xEu)->unk_0E = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xFu)->unk_0F = r1_2;
	current_object = current_object + 1u;
	r0 = test_mode_set_up_colours();
	OBJECT(current_object - 0x10u)->unk_10 = 0x101u;
	test_mode_memory_test_draw(r0);
	r10 = 0x680000u;
	r0 = 0x4000u;
	r0 = test_mode_memory_test_halfwords(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x10u)->unk_10 = (OBJECT(current_object - 0x10u)->unk_10 & 0xFF00u) | (r1_2 & 0xFFu);
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x11u)->unk_10 = (OBJECT(current_object - 0x11u)->unk_10 & 0xFFu) | ((r1_2 & 0xFFu) << 8u);
	current_object = current_object + 1u;
	r0 = test_mode_set_up_colours();
	OBJECT(current_object - 0x12u)->unk_12 = 1u;
	OBJECT(current_object - 0x12u)->unk_13 = 1u;
	test_mode_memory_test_draw(r0);
	r10 = 0x684000u;
	r0 = 0x4000u;
	r0 = test_mode_memory_test_halfwords(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x12u)->unk_12 = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x13u)->unk_13 = r1_2;
	current_object = current_object + 1u;
	r0 = test_mode_set_up_colours();
	OBJECT(current_object - 0x14u)->unk_14 = 0x1010101u;
	test_mode_memory_test_draw(r0);
	move_halfwords(SPRITE_RAM, WORK_RAM, 0x10000u);
	r10 = 0x200000u;
	r0 = 0x20000u;
	r0 = test_mode_memory_test_words(r0, r10);
	move_halfwords(WORK_RAM, SPRITE_RAM, 0x10000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x14u)->unk_14 = (OBJECT(current_object - 0x14u)->unk_14 & 0xFFFFFF00u) | (r1_2 & 0xFFu);
	t10 = r0 & 0xFF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t10 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x15u)->unk_14 = (OBJECT(current_object - 0x15u)->unk_14 & 0xFFFF00FFu) | ((r1_2 & 0xFFu) << 8u);
	t11 = r0 & 0xFF0000u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t11 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x16u)->unk_14 = (OBJECT(current_object - 0x16u)->unk_14 & 0xFF00FFFFu) | ((r1_2 & 0xFFu) << 16u);
	t12 = r0 & ~0xFFFFFFu;
	r0 = t12;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t12 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x17u)->unk_14 = (OBJECT(current_object - 0x17u)->unk_14 & 0xFFFFFFu) | ((r1_2 & 0xFFu) << 24u);
	current_object = current_object + 1u;
	init_sprite_lists();
	OBJECT(current_object - 0x18u)->unk_18 = (OBJECT(current_object - 0x18u)->unk_18 & 0xFFFFFF00u) | 0x1u;
	test_mode_memory_test_draw(r0);
	move_halfwords(SPRITE_RAM, SOUND_RAM, 0x1000u);
	r10 = 0x700000u;
	r0 = 0x1FFFu;
	r0 = test_mode_memory_test_bytes(r0, r10);
	move_halfwords(SOUND_RAM, SPRITE_RAM, 0x1000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x18u)->unk_18 = (OBJECT(current_object - 0x18u)->unk_18 & 0xFFFFFF00u) | (r1_2 & 0xFFu);
	current_object = current_object + 1u;
	init_sprite_lists();
	r0 = test_mode_memory_test_draw(r0);
	mem_write8(SOUND_701FFF, 0u);
	interrupts_on();
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_memory_test_run_ADDRESS;
	test_mode_memory_test_run((uint16_t)r0);
}

/* The memory test's results, each frame: TEST goes back to the menu; PUSH TEST BUTTON TO EXIT blinks
   (test_mode_blink_prompt). */
void test_mode_memory_test_run(uint16_t r0)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(r0);
}

/* Draws the memory test's page (MEMORY TEST, 837-8890 <ROM> and its four chips, 837-8676 <RAM> and its
   21, NOW TESTING) and each chip's result from memory_test_results beside its name: ---- (not yet),
   TEST, GOOD or BAD. */
uint32_t test_mode_memory_test_draw(uint32_t r0)
{
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint8_t t1 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint16_t t2 = 0;

	r10 = 0x1CC9Cu;
	test_mode_draw_texts(r0, r10);
	r10 = memory_test_results_ADDRESS;
	r11 = 0x1CD96u;
	r12 = TEXT_CELL(0, 0);
	r9 = 0x19u;
	do
	{
		r0_2 = mem_read8(r10) * 3u;
		r13 = 0x1CDC8u + r0_2 * 2u;
		t1 = ROM8(r13);
		r13++;
		r0_2 = t1;
		t2 = ROM16(r11);
		r11 += 2u;
		r12 = (r12 & ~0xFFFFu) | t2;
		t3 = ROM8(r13);
		r13++;
		r0_2 = (uint16_t)(r0_2 << 8) | t3;
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		t4 = ROM8(r13);
		r13++;
		r0_2 = (r0_2 & ~0xFFu) | t4;
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		t5 = ROM8(r13);
		r0_2 = (r0_2 & ~0xFFu) | t5;
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		r0_2 = (r0_2 & ~0xFFu) | ROM8(r13 + 1u);
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		r9--;
		r10++;
	} while (r9 != 0u);
	return r0_2;
}

/* Sums a stretch of the program ROM as two 16-bit checks, one of the even halfwords (the chip of the low
   half of each 32-bit word: IC37 or IC36) and one of the odd (IC40 or IC39): each halfword is xored in,
   the sum shifted right by one and xored with 8810 when a 1 falls out (a CRC); both start at 1. */
void BOARD_ORIGINAL(test_mode_memory_test_checksum)(uint32_t r10, uint32_t *p_r0, uint32_t *p_r1)
{
	uint32_t r0 = *p_r0;
	uint32_t carry = 0;
	uint8_t r2 = 0;
	uint16_t r0_2 = 0, r1 = 0, r3 = 0;
	uint32_t r9 = 0;

	r9 = r0 >> 2;
	r0_2 = 1u;
	r1 = 1u;
	r2 = 0xFFu;
	r3 = 0x8810u;
	do
	{
		r0_2 ^= mem_read16(r10);
		carry = v60_shl_cy(r0_2, r2, 16);
		r0_2 = shift_by(r0_2, r2, 16);
		r10 += 2u;
		if (carry)
			r0_2 ^= r3;
		r1 ^= mem_read16(r10);
		carry = v60_shl_cy(r1, r2, 16);
		r1 = shift_by(r1, r2, 16);
		r10 += 2u;
		if (carry)
			r1 ^= r3;
		r9--;
	} while (r9 != 0u);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0_2;
	*p_r1 = (*p_r1 & 0xFFFF0000u) | r1;
}

/* Tests RAM 32 bits at a time (the work RAM, in place: it takes its return address off the stack into a
   register and jumps back through it, so that it needs no stack in the RAM it overwrites): 8 passes,
   each writing a walking pattern (01010101 the first pass, one bit further each pass, turned by a few
   bits from word to word by the word's address) and reading it back; returns the bits found wrong (each
   mismatch's expected ^ read, xored together), a byte per chip; NOW TESTING blinks once a pass
   (test_mode_memory_test_blink). */
uint32_t test_mode_memory_test_words(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r6 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;

	r1 = r0 >> 2;
	r2 = 0x1010101u;
	r0_2 = 0u;
	do
	{
		r12 = r10;
		r3 = r2;
		r4 = r1;
		do
		{
			mem_write32(r12, r3);
			r12 = r12 + 4u;
			r6 = 6u;
			t1 = r12 << 15;
			r5 = t1;
			if (t1 != 0u)
			{
				r6--;
				t2 = r5 << 3;
				r5 = t2;
				if (t2 != 0u)
				{
					r6--;
					t3 = r5 << 3;
					r5 = t3;
					if (t3 != 0u)
					{
						r6--;
						t4 = r5 << 3;
						r5 = t4;
						if (t4 != 0u)
						{
							r6--;
							if (r5 << 3 != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 32);
			r4--;
		} while (r4 != 0u);
		r12 = r10;
		r3 = r2;
		r4 = r1;
		do
		{
			t5 = mem_read32(r12);
			r12 = r12 + 4u;
			if (t5 != r3)
				r0_2 = r0_2 ^ r3 ^ mem_read32(r12 - 4u);
			r6 = 6u;
			t6 = r12 << 15;
			r5 = t6;
			if (t6 != 0u)
			{
				r6--;
				t7 = r5 << 3;
				r5 = t7;
				if (t7 != 0u)
				{
					r6--;
					t8 = r5 << 3;
					r5 = t8;
					if (t8 != 0u)
					{
						r6--;
						t9 = r5 << 3;
						r5 = t9;
						if (t9 != 0u)
						{
							r6--;
							if (r5 << 3 != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 32);
			r4--;
		} while (r4 != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(r2, 1u, 32);
		r2 = (r2 << 1) | (r2 >> 31);
	} while (carry == 0u);
	return r0_2;
}

/* test_mode_memory_test_words 16 bits at a time (the palette RAMs: a chip for each byte), with the
   pattern 0101; returns the bits found wrong. */
uint32_t test_mode_memory_test_halfwords(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r6 = 0;
	uint16_t r2 = 0, r3 = 0, r5 = 0;
	uint32_t r1 = 0, r4 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	r1 = r0 >> 1;
	r2 = 0x101u;
	r0 &= ~0xFFFFu;
	do
	{
		r12 = r10;
		r3 = (uint16_t)r2;
		r4 = r1;
		do
		{
			mem_write16(r12, r3);
			r12 = r12 + 2u;
			r5 = (uint16_t)r12;
			r6 = 6u;
			if ((uint16_t)r5 != 0u)
			{
				r6--;
				t1 = (uint32_t)r5 << 3;
				r5 = t1;
				if (t1 != 0u)
				{
					r6--;
					t2 = (uint32_t)r5 << 3;
					r5 = t2;
					if (t2 != 0u)
					{
						r6--;
						t3 = (uint32_t)r5 << 3;
						r5 = t3;
						if (t3 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 16);
			r4--;
		} while (r4 != 0u);
		r12 = r10;
		r3 = (uint16_t)r2;
		r4 = r1;
		do
		{
			t4 = mem_read16(r12);
			r12 = r12 + 2u;
			if (t4 != r3)
			{
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r3);
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ mem_read16(r12 - 2u));
			}
			r5 = (uint16_t)r12;
			r6 = 6u;
			if ((uint16_t)r5 != 0u)
			{
				r6--;
				t5 = (uint32_t)r5 << 3;
				r5 = t5;
				if (t5 != 0u)
				{
					r6--;
					t6 = (uint32_t)r5 << 3;
					r5 = t6;
					if (t6 != 0u)
					{
						r6--;
						t7 = (uint32_t)r5 << 3;
						r5 = t7;
						if (t7 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 16);
			r4--;
		} while (r4 != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(r2, 1u, 16);
		r2 = (uint16_t)(((uint32_t)r2 << 1) | (r2 >> 15));
	} while (carry == 0u);
	return r0;
}

/* test_mode_memory_test_words a byte at a time (the sound RAM, all but its last byte, which holds the
   sound program), with the pattern 01; returns the bits found wrong. */
uint32_t test_mode_memory_test_bytes(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r2 = 0, r3 = 0, r6 = 0;
	uint16_t r5 = 0;
	uint32_t r1 = 0, r4 = 0;
	uint8_t t5 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;

	r1 = r0;
	r2 = 1u;
	r0 &= ~0xFFu;
	do
	{
		r12 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			mem_write8(r12, r3);
			r12 = r12 + 1u;
			r6 = 6u;
			t1 = r12 * 2u;
			r5 = t1;
			if (t1 != 0u)
			{
				r6--;
				t2 = (uint32_t)r5 << 3;
				r5 = t2;
				if (t2 != 0u)
				{
					r6--;
					t3 = (uint32_t)r5 << 3;
					r5 = t3;
					if (t3 != 0u)
					{
						r6--;
						t4 = (uint32_t)r5 << 3;
						r5 = t4;
						if (t4 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 8);
			r4--;
		} while (r4 != 0u);
		r12 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			t5 = mem_read8(r12);
			r12 = r12 + 1u;
			if (t5 != r3)
			{
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ r3);
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ mem_read8(r12 - 1u));
			}
			r6 = 6u;
			t6 = r12 * 2u;
			r5 = t6;
			if (t6 != 0u)
			{
				r6--;
				t7 = (uint32_t)r5 << 3;
				r5 = t7;
				if (t7 != 0u)
				{
					r6--;
					t8 = (uint32_t)r5 << 3;
					r5 = t8;
					if (t8 != 0u)
					{
						r6--;
						t9 = (uint32_t)r5 << 3;
						r5 = t9;
						if (t9 != 0u)
						{
							r6--;
							if ((uint16_t)((uint32_t)r5 << 3) != 0u)
								r6--;
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 8);
			r4--;
		} while (r4 != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(r2, 1u, 8);
		r2 = (uint8_t)(((uint32_t)r2 << 1) | (r2 >> 7));
	} while (carry == 0u);
	return r0;
}

/* test_mode_memory_test_words for RAM of 4-bit chips, 16 bits at a time (the video RAM and the sprite
   RAM): 4 passes with the pattern 1111 (a bit per chip); returns the bits found wrong, a nibble per
   chip. */
uint32_t test_mode_memory_test_nibbles(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r6 = 0;
	uint16_t r2 = 0, r3 = 0;
	uint32_t r1 = 0, r4 = 0, r5 = 0;
	uint16_t t8 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0;

	r1 = r0 >> 1;
	r2 = 0x1111u;
	r0 &= ~0xFFFFu;
	do
	{
		r12 = r10;
		r3 = (uint16_t)r2;
		r4 = r1;
		do
		{
			mem_write16(r12, r3);
			r12 = r12 + 2u;
			r6 = 9u;
			t1 = r12 << 15;
			r5 = t1;
			if (t1 != 0u)
			{
				r6--;
				t2 = r5 * 2u * 2u;
				r5 = t2;
				if (t2 != 0u)
				{
					r6--;
					t3 = r5 * 2u * 2u;
					r5 = t3;
					if (t3 != 0u)
					{
						r6--;
						t4 = r5 * 2u * 2u;
						r5 = t4;
						if (t4 != 0u)
						{
							r6--;
							t5 = r5 * 2u * 2u;
							r5 = t5;
							if (t5 != 0u)
							{
								r6--;
								t6 = r5 * 2u * 2u;
								r5 = t6;
								if (t6 != 0u)
								{
									r6--;
									t7 = r5 * 2u * 2u;
									r5 = t7;
									if (t7 != 0u)
									{
										r6--;
										if (r5 * 2u * 2u != 0u)
											r6--;
									}
								}
							}
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 16);
			r4--;
		} while (r4 != 0u);
		r12 = r10;
		r3 = (uint16_t)r2;
		r4 = r1;
		do
		{
			t8 = mem_read16(r12);
			r12 = r12 + 2u;
			if (t8 != r3)
			{
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r3);
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ mem_read16(r12 - 2u));
			}
			r6 = 9u;
			t9 = r12 << 15;
			r5 = t9;
			if (t9 != 0u)
			{
				r6--;
				t10 = r5 * 2u * 2u;
				r5 = t10;
				if (t10 != 0u)
				{
					r6--;
					t11 = r5 * 2u * 2u;
					r5 = t11;
					if (t11 != 0u)
					{
						r6--;
						t12 = r5 * 2u * 2u;
						r5 = t12;
						if (t12 != 0u)
						{
							r6--;
							t13 = r5 * 2u * 2u;
							r5 = t13;
							if (t13 != 0u)
							{
								r6--;
								t14 = r5 * 2u * 2u;
								r5 = t14;
								if (t14 != 0u)
								{
									r6--;
									t15 = r5 * 2u * 2u;
									r5 = t15;
									if (t15 != 0u)
									{
										r6--;
										if (r5 * 2u * 2u != 0u)
											r6--;
									}
								}
							}
						}
					}
				}
			}
			r3 = rotate_by(r3, r6, 16);
			r4--;
		} while (r4 != 0u);
		test_mode_memory_test_blink();
		carry = v60_rot_cy(r2, 1u, 16);
		r2 = (uint16_t)(((uint32_t)r2 << 1) | (r2 >> 15));
	} while (carry == 0u);
	return r0;
}

/* Shows or hides NOW TESTING (each of its cells xored with its character in palette 66): the RAM tests
   call it after each pass. */
void test_mode_memory_test_blink(void)
{
	uint16_t r26 = 0;
	uint32_t t1 = 0;

	r26 = 0x6600u;
	r27 = TEXT_CELL(14, 25);
	r28 = 0x1CD87u;
	do
	{
		r26 = (uint16_t)((r26 & ~0xFFu) | ROM8(r28));
		t1 = r27;
		r27 = r27 + 2u;
		mem_write16(t1, mem_read16(t1) ^ r26);
		r28 = r28 + 1u;
	} while (ROM8(r28) != 0u);
}

/* Starts the VOLUME ADJUSTMENT, the calibration of the wheels' and pedals' potentiometers: draws ADJUST
   and CANCEL with the cursor on CANCEL (test_mode_page_cursor 1), the six controls' names (MONITOR A and
   B: STEERING WHEEL, ACCELERATOR, BRAKE) and the prompt; then test_mode_volume_adjustment_select. */
void test_mode_volume_adjustment(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1CF71u;
	r0 = test_mode_draw_texts(r0, r10);
	r10 = 0x1CFA0u;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 1u;
	r10 = 0x310000u | rom_01CF9E;
	r0 = test_mode_set_cursor(r0, r10);
	test_mode_routine = test_mode_volume_adjustment_select_ADDRESS;
	test_mode_volume_adjustment_select(r0);
}

/* Each frame: the bars (test_mode_volume_adjustment_draw); SERVICE moves the cursor between ADJUST and
   CANCEL; TEST on CANCEL goes back to the menu; TEST on ADJUST starts the adjustment: ADJUSTING, PUSH
   TEST BUTTON TO EXIT, each control's range (volume_ranges) narrowed to where it is now, then
   test_mode_volume_adjustment_adjust. */
void test_mode_volume_adjustment_select(uint32_t r0)
{
	uint8_t r0_3 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	test_mode_volume_adjustment_draw(r0);
	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0_2 = test_mode_page_cursor ^ 1u;
		test_mode_page_cursor = r0_2;
		r10 = 0x310000u | rom_01CF9C[r0_2];
		test_mode_set_cursor(r0_2, r10);
	}
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		t1 = test_mode_blink_timer - 1u;
		test_mode_blink_timer = t1;
		if ((int8_t)t1 < 0)
		{
			test_mode_blink_timer = 5;
			t2 = test_mode_blink_address;
			t3 = mem_read16(t2) ^ 0x6A7Fu;
			mem_write16(t2, t3);
			if (t3 != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	if (test_mode_page_cursor == 0u)
	{
		r0_2 = test_mode_clear_text();
		r10 = 0x1D021u;
		r0_2 = test_mode_draw_texts(r0_2, r10);
		r10 = 0x1CFA0u;
		test_mode_draw_texts(r0_2, r10);
		test_mode_show_exit_prompt();
		r10 = volume_ranges_ADDRESS;
		r11 = test_mode_analog_inputs_ADDRESS;
		r9 = 6u;
		do
		{
			r0_3 = (uint8_t)(mem_read8(r11) & ~1u);
			mem_write8(r10, r0_3);
			r10++;
			mem_write8(r10, r0_3 | 1u);
			r9--;
			r11++;
			r10++;
		} while (r9 != 0u);
		test_mode_routine = test_mode_volume_adjustment_adjust_ADDRESS;
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* The adjustment, each frame: each control's range (volume_ranges) widens to take its position
   (test_mode_analog_inputs), as the operator turns the wheels and presses the pedals all the way; the
   bars; TEST marks the settings changed (test_mode_flags bit 1) and goes back to the menu; PUSH TEST
   BUTTON TO EXIT blinks. */
void test_mode_volume_adjustment_adjust(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0;

	r10 = test_mode_analog_inputs_ADDRESS;
	r11 = volume_ranges_ADDRESS;
	r9 = 6u;
	do
	{
		r0 = mem_read8(r10);
		r10++;
		if (r0 < mem_read8(r11))
			mem_write8(r11, r0);
		r11++;
		if (mem_read8(r11) < r0)
			mem_write8(r11, r0);
		r11++;
		r9--;
	} while (r9 != 0u);
	r0 = test_mode_volume_adjustment_draw(r0);
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		test_mode_flags |= 2u;
		test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt(r0);
}

/* Draws the six controls' bars (32 cells, a cell for 8 steps): the range of volume_ranges in green and
   the control's position (test_mode_analog_inputs) in red. */
uint32_t test_mode_volume_adjustment_draw(uint32_t r0)
{
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	r10 = 0x1D03Cu;
	test_mode_draw_texts(r0, r10);
	r10 = TEXT_CELL(0, 0);
	r11 = 0x1D030u;
	r12 = volume_ranges_ADDRESS;
	r9 = 3u;
	do
	{
		r0_2 = mem_read8(r12);
		r0_2 = r0_2 >> 3;
		r12 = r12 + 1u;
		r1 = mem_read8(r12);
		r1 = r1 >> 3;
		r1 = (uint8_t)(r1 - r0_2);
		r1 = (uint8_t)(r1 + 1u);
		t1 = ROM16(r11);
		r11 += 2u;
		r10 = ((r10 & ~0xFFFFu) | t1) + r0_2 * 2u;
		r0_3 = 0x6403u;
		r12 = r12 + 1u;
		do
		{
			mem_write16(r10, r0_3);
			r1--;
			r10 += 2u;
		} while (r1 != 0u);
		r9--;
	} while (r9 != 0u);
	r9 = 3u;
	do
	{
		r0_2 = mem_read8(r12);
		r0_2 = r0_2 >> 3;
		r12 = r12 + 1u;
		r1 = mem_read8(r12);
		r1 = r1 >> 3;
		r1 = (uint8_t)(r1 - r0_2);
		r1 = (uint8_t)(r1 + 1u);
		t2 = ROM16(r11);
		r11 += 2u;
		r10 = ((r10 & ~0xFFFFu) | t2) + r0_2 * 2u;
		r0_3 = 0x6403u;
		r12 = r12 + 1u;
		do
		{
			mem_write16(r10, r0_3);
			r1--;
			r10 += 2u;
		} while (r1 != 0u);
		r9--;
	} while (r9 != 0u);
	r11 = 0x1D030u;
	r12 = test_mode_analog_inputs_ADDRESS;
	r9 = 6u;
	do
	{
		r0_2 = mem_read8(r12) >> 3;
		t3 = ROM16(r11);
		r11 += 2u;
		r10 = (r10 & ~0xFFFFu) | t3;
		mem_write16(r10 + r0_2 * 2u, 0x6203u);
		r9--;
		r12 = r12 + 1u;
	} while (r9 != 0u);
	return r0_2;
}

/* Starts the INPUT TEST: clears the text layer, draws the inputs' names and the blinking PUSH TEST
   BUTTON TO EXIT, then test_mode_input_test_run. */
void test_mode_input_test(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1D26Cu;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_input_test_run_ADDRESS;
	test_mode_input_test_run(r0);
}

/* The input test, each frame: the inputs (test_mode_input_test_draw); TEST goes on to
   test_mode_input_test_release, so that the press shows as TEST On before the page ends; PUSH TEST
   BUTTON TO EXIT blinks (test_mode_blink_prompt). */
void test_mode_input_test_run(uint32_t r0)
{
	r0 = test_mode_input_test_draw(r0);
	if ((test_mode_buttons_pressed & 2u) == 0u)
	{
		test_mode_blink_prompt(r0);
		return;
	}
	test_mode_routine = test_mode_input_test_release_ADDRESS;
	test_mode_blink_prompt(r0);
}

/* The input test once TEST is pressed: the inputs each frame until TEST is let go, then back to the
   menu. */
void test_mode_input_test_release(void)
{
	uint32_t r0 = 0;

	r0 = test_mode_input_test_draw(0u);
	if ((test_mode_buttons_released & 2u) == 0u)
	{
		test_mode_blink_prompt(r0);
		return;
	}
	test_mode_routine = test_mode_menu_ADDRESS;
	test_mode_blink_prompt(r0);
}

/* Draws the inputs of both monitors (cabinets): the wheel, accelerator and brake as the game sees them
   through the calibration (test_mode_analog_inputs clamped to volume_ranges and scaled to 0-FF), as a
   mark on a bar of 16 cells and a hexadecimal number; SHIFT UP, SHIFT DOWN, START, DJ/MUSIC, <<, >>,
   COIN and SERVICE On or Off, read from each I/O chip's ports (active low); TEST On when either
   monitor's is pressed. */
uint32_t test_mode_input_test_draw(uint32_t r0)
{
	uint32_t carry = 0;
	uint8_t r1_2 = 0;
	uint16_t r1 = 0, r2 = 0;
	uint32_t r0_2 = 0, r2_2 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;

	r10 = 0x1D30Au;
	test_mode_draw_texts(r0, r10);
	r10 = TEXT_CELL(0, 0);
	r11 = 0x1D380u;
	r12 = test_mode_analog_inputs_ADDRESS;
	r13 = volume_ranges_ADDRESS;
	r14 = 0x1D3D8u;
	r9 = 6u;
	do
	{
		r0_2 = mem_read8(r12);
		r2 = mem_read8(r13);
		r12 = r12 + 1u;
		r13++;
		if (r0_2 < r2)
			r0_2 = r2;
		r1 = mem_read8(r13);
		r13++;
		if (r1 < r0_2)
			r0_2 = r1;
		r0_2 = (uint8_t)(r0_2 - r2);
		r1 = (uint8_t)(r1 - r2);
		r2 = (uint16_t)r0_2;
		r0_2 = (uint16_t)(r0_2 << 8);
		r0_2 = (uint16_t)(r0_2 - r2);
		r0_2 = divide_u16(r0_2, r1);
		r10 = (r10 & ~0xFFFFu) | ROM16(r14);
		mem_write16(r10 + ((uint8_t)r0_2 >> 4) * 2u, 0x6403u);
		r1_2 = 0x64u;
		r2_2 = 2u;
		r10 = (r10 & ~0xFFFFu) | ROM16(r11);
		r14 = r14 + 2u;
		r11 += 2u;
		r10 = test_mode_draw_hex_number(r0_2, r1_2, r2_2, r10);
		r9--;
	} while (r9 != 0u);
	r12 = 0xC00000u;
	r13 = 0xC80000u;
	do
	{
		r12 = (r12 & ~0xFFu) | ROM8(r11);
		r11++;
		r0_2 = ROM8(r11);
		carry = (mem_read32(r12) & shl32(1u, (int)(r0_2))) != 0u;
		r11++;
		r10 = (r10 & ~0xFFFFu) | ROM16(r11);
		mem_write32(r10, rom_01D3C0[carry]);
		r13 = (r13 & ~0xFFu) | (uint8_t)r12;
		carry = (mem_read32(r13) & shl32(1u, (int)(r0_2))) != 0u;
		r11 += 2u;
		r10 = (r10 & ~0xFFFFu) | ROM16(r11);
		mem_write32(r10, rom_01D3C0[carry]);
		r11 += 2u;
	} while ((int16_t)ROM16(r11) >= 0);
	r12 = (r12 & ~0xFFu) | 8u;
	r13 = (r13 & ~0xFFu) | (uint8_t)r12;
	r0_2 = mem_read8(r12);
	r12 = (r12 & ~0xFFu) | 0xAu;
	r0_2 = r0_2 & (mem_read8(r12) >> 4);
	r1_2 = mem_read8(r13) & (mem_read8((r13 & ~0xFFu) | (uint8_t)r12) >> 4);
	carry = (r0_2 & 1u) != 0u;
	r11 += 2u;
	r10 = (r10 & ~0xFFFFu) | ROM16(r11);
	mem_write32(r10, rom_01D3C0[carry]);
	carry = (r1_2 & 1u) != 0u;
	r11 += 2u;
	r10 = (r10 & ~0xFFFFu) | ROM16(r11);
	mem_write32(r10, rom_01D3C0[carry]);
	r0_2 = r0_2 & r1_2;
	carry = (r0_2 & 2u) != 0u;
	mem_write32((r10 & ~0xFFFFu) | ROM16(r11 + 2u), rom_01D3C0[carry]);
	return r0_2;
}

/* Starts the OUTPUT TEST: the ten outputs of the two monitors (cabinets) with their state
   (test_mode_output_test_draw), EXIT and the prompt, the cursor on EXIT (test_mode_page_cursor 10); then
   test_mode_output_test_run. */
void test_mode_output_test(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1D4F8u;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 0xAu;
	r10 = 0x310000u | rom_01D5BE;
	test_mode_set_cursor(r0, r10);
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
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

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
			invalid_jump(rom_01D5EC[test_mode_page_cursor]);
		}
		test_mode_output_test_draw();
	}
	if ((test_mode_buttons_released & 2u) != 0u)
	{
		r0 = test_mode_page_cursor;
		if (r0 != 0xAu)
		{
			switch (r0)
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
				invalid_jump(rom_01D5EC[r0]);
			}
			test_mode_output_test_draw();
		}
	}
	if ((test_mode_buttons_pressed & 1u) == 0u || (test_mode_buttons & 2u) != 0u)
	{
		t1 = test_mode_blink_timer - 1u;
		test_mode_blink_timer = t1;
		if ((int8_t)t1 < 0)
		{
			test_mode_blink_timer = 5;
			t2 = test_mode_blink_address;
			t3 = mem_read16(t2) ^ 0x6A7Fu;
			mem_write16(t2, t3);
			if (t3 != 0u)
				test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
		}
		return;
	}
	test_mode_clear_cursor();
	r0 = test_mode_page_cursor;
	r0 = (uint8_t)(r0 + 1u);
	if ((int8_t)r0 >= 0xB)
		r0 = 0u;
	test_mode_page_cursor = r0;
	r10 = 0x310000u | rom_01D5AA[r0];
	test_mode_set_cursor(r0, r10);
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
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
	uint32_t r10 = 0, r11 = 0;

	r10 = TEXT_CELL(0, 0);
	r11 = 0x1D5C0u;
	do
	{
		r10 = (r10 & ~0xFFFFu) | ROM16(r11);
		r11 += 2u;
		r12 = ROM8(r11);
		r11++;
		r12 = r12 + GAME_VARIABLES;
		carry = (mem_read32(r12) & shl32(1u, (int)(ROM8(r11)))) != 0u;
		mem_write32(r10, rom_01D618[carry]);
		r11++;
	} while (ROM16(r11) != 0u);
}

/* Turns the output under the cursor (test_mode_page_cursor) on or off: flips its bit in the game's
   output bytes (the list at 01D5C0). */
void test_mode_output_test_toggle(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = 0x1D5C2u + test_mode_page_cursor * 4u;
	r11 = ROM8(r10) + GAME_VARIABLES;
	mem_write32(r11, mem_read32(r11) ^ shl32(1u, (int)(ROM8(r10 + 1u))));
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
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1D788u;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 2u;
	r10 = 0x310000u | rom_01D7C4;
	test_mode_set_cursor(r0, r10);
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
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor;
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 3)
			r0 = 0u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01D7C0[r0];
		test_mode_set_cursor(r0, r10);
	}
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		r0_2 = 0x1Fu;
		test_mode_queue_sound(r0_2);
		test_mode_queue_sound_screen_2(r0_2);
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
			invalid_jump(rom_01D77C[test_mode_page_cursor]);
		}
		test_mode_sound_test_draw();
	}
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the two monitors' sound numbers (sound_test_number_a, sound_test_number_b), 3 digits each. */
void test_mode_sound_test_draw(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;

	r10 = TEXT_CELL(28, 11);
	r0 = sound_test_number_a;
	r1 = 0x64u;
	r2 = 3u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF6B8u;
	r0 = sound_test_number_b;
	r1 = 0x64u;
	r2 = 3u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
}

/* Plays monitor A's sound number sound_test_number_a from the list of 121 sound codes at 01D7C6
   (test_mode_queue_sound) and steps to the next; from number 40 on it first sends command D6 (DA from
   66, D8 from 6B), which selects sound set 6 (A, 8): the music parts' templates and the screen's
   MultiPCM sample bank; past the list's end (its closing 0) the number goes back to 0 without a sound. */
void test_mode_sound_test_play_a(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;
	uint32_t stack_4 = 0;

	r1 = sound_test_number_a;
	if (r1 < 0x40u)
	{
		r0_2 = mem_read8(0x1D7C6u + r1);
		if (r0_2 == 0u)
		{
			sound_test_number_a = 0u;
			return;
		}
		sound_test_number_a = r1 + 1u;
		test_mode_queue_sound((uint16_t)r0_2);
		return;
	}
	r0 = 0xD6u;
	if (r1 >= 0x66u)
	{
		r0 = 0xDAu;
		if (r1 >= 0x6Bu)
			r0 = 0xD8u;
	}
	stack_4 = r1;
	test_mode_queue_sound(r0);
	r1 = stack_4;
	r0_2 = mem_read8(0x1D7C6u + r1);
	if (r0_2 == 0u)
	{
		sound_test_number_a = 0u;
		return;
	}
	sound_test_number_a = r1 + 1u;
	test_mode_queue_sound((uint16_t)r0_2);
}

/* test_mode_sound_test_play_a for monitor B: sound_test_number_b, sent through
   test_mode_queue_sound_screen_2. */
void test_mode_sound_test_play_b(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;
	uint32_t stack_4 = 0;

	r1 = sound_test_number_b;
	if (r1 < 0x40u)
	{
		r0_2 = mem_read8(0x1D7C6u + r1);
		if (r0_2 == 0u)
		{
			sound_test_number_b = 0u;
			return;
		}
		sound_test_number_b = r1 + 1u;
		test_mode_queue_sound_screen_2((uint16_t)r0_2);
		return;
	}
	r0 = 0xD6u;
	if (r1 >= 0x66u)
	{
		r0 = 0xDAu;
		if (r1 >= 0x6Bu)
			r0 = 0xD8u;
	}
	stack_4 = r1;
	test_mode_queue_sound_screen_2(r0);
	r1 = stack_4;
	r0_2 = mem_read8(0x1D7C6u + r1);
	if (r0_2 == 0u)
	{
		sound_test_number_b = 0u;
		return;
	}
	sound_test_number_b = r1 + 1u;
	test_mode_queue_sound_screen_2((uint16_t)r0_2);
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
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	mem_write16(PALETTE_609C00, 0u);
	mem_write16(0x689C00u, 0u);
	r10 = 0x1D9C7u;
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_crt_test_draw_bars(r0);
	test_mode_show_continue_prompt();
	test_mode_routine = test_mode_crt_test_bars_run_ADDRESS;
	test_mode_crt_test_bars_run((uint16_t)r0);
}

/* The colour bars, each frame: TEST goes on to the second page (test_mode_crt_test_grid); the message
   blinks. */
void test_mode_crt_test_bars_run(uint16_t r0)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
		test_mode_routine = test_mode_crt_test_grid_ADDRESS;
	test_mode_blink_prompt(r0);
}

/* The C.R.T. test's second page: the crosshatch (test_mode_crt_test_draw_grid), C.R.T. TEST 2/2 and the
   blinking PUSH TEST BUTTON TO EXIT; then test_mode_crt_test_grid_run. */
void test_mode_crt_test_grid(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_crt_test_draw_grid(0u);
	r10 = 0x1D9DCu;
	r0 = test_mode_draw_texts(r0, r10);
	test_mode_show_exit_prompt();
	test_mode_routine = test_mode_crt_test_grid_run_ADDRESS;
	test_mode_crt_test_grid_run(r0);
}

/* The crosshatch, each frame: TEST clears the text layer, gives the background back its blue (733) and
   goes back to the menu; the message blinks. */
void test_mode_crt_test_grid_run(uint32_t r0)
{
	if ((test_mode_buttons_pressed & 2u) != 0u)
	{
		r0 = test_mode_clear_text();
		mem_write16(PALETTE_609C00, 0x733u);
		mem_write16(0x689C00u, 0x733u);
		test_mode_routine = test_mode_menu_ADDRESS;
	}
	test_mode_blink_prompt(r0);
}

/* Draws the first page on the text layer: a frame around the screen and four bars of 32 steps
   (test_mode_crt_test_draw_bar, characters 8001-8004: red, green, blue and white). */
uint32_t test_mode_crt_test_draw_bars(uint32_t r0)
{
	uint32_t r0_2 = 0, r9 = 0, r10 = 0;

	r10 = TEXT_CELL(1, 0);
	r0_2 = (r0 & ~0xFFFFu) | 0x6C15u;
	r9 = 0x26u;
	do
	{
		mem_write16(r10 + 0xD80u, r0_2);
		mem_write16(r10, r0_2);
		r9--;
		r10 += 2u;
	} while (r9 != 0u);
	r10 = (r10 & ~0xFFFFu) | 0xF080u;
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r9 = 0x1Au;
	do
	{
		mem_write16(r10 + 0x4Eu, r0_2);
		mem_write16(r10, r0_2);
		r10 += 0x80u;
		r9--;
	} while (r9 != 0u);
	r10 = (r10 & ~0xFFFFu) | 0xF000u;
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	mem_write16(r10, r0_2);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	mem_write16(r10 + 0x4Eu, r0_2);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	mem_write16(r10 + 0xD80u, r0_2);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	mem_write16(r10 + 0xDCEu, r0_2);
	r10 = (r10 & ~0xFFFFu) | 0xF208u;
	r0_2 = (r0_2 & ~0xFFFFu) | 0x8001u;
	r10 = test_mode_crt_test_draw_bar((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = test_mode_crt_test_draw_bar((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = test_mode_crt_test_draw_bar((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = test_mode_crt_test_draw_bar((uint16_t)r0_2, r10);
	return r0_2;
}

/* Draws one colour bar: 5 rows of 32 cells of character R0, each column with the next palette (the cell
   + 200), so that the colour steps up across it; returns the place 5 rows down for the next bar. */
uint32_t test_mode_crt_test_draw_bar(uint16_t r0, uint32_t r10)
{
	uint16_t r1 = 0;
	uint32_t r9 = 0, r11 = 0;

	r11 = r10;
	r1 = (uint16_t)r0;
	r9 = 0x20u;
	do
	{
		mem_write16(r11 + 0x200u, r1);
		mem_write16(r11 + 0x180u, r1);
		mem_write16(r11 + 0x100u, r1);
		mem_write16(r11 + 0x80u, r1);
		mem_write16(r11, r1);
		r1 += 0x200u;
		r9--;
		r11 += 2u;
	} while (r9 != 0u);
	r10 += 0x280u;
	return r10;
}

/* Draws the crosshatch of the second page over the whole text layer (characters 6C1B-6C1F: the edges and
   the inside), and blanks the cell after the exit message. */
uint32_t test_mode_crt_test_draw_grid(uint32_t r0)
{
	uint16_t r1 = 0;
	uint32_t r0_2 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;

	r10 = TEXT_CELL(0, 1);
	r0_2 = (r0 & ~0xFFFFu) | 0x6C1Bu;
	r1 = (uint16_t)(r0_2 + 1u);
	r9 = 0x1Au;
	do
	{
		mem_write16(r10 + 0x4Eu, r1);
		mem_write16(r10, r0_2);
		r10 += 0x80u;
		r9--;
	} while (r9 != 0u);
	r10 = (r10 & ~0xFFFFu) | 0xF002u;
	r1++;
	r0_2 = (r0_2 & ~0xFFFFu) | r1;
	r1++;
	r9 = 0x26u;
	do
	{
		mem_write16(r10 + 0xD80u, r1);
		mem_write16(r10, r0_2);
		r9--;
		r10 += 2u;
	} while (r9 != 0u);
	r10 = (r10 & ~0xFFFFu) | 0xF082u;
	r0_2 = (r0_2 & ~0xFFFFu) | r1;
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r9 = 0x1Au;
	do
	{
		r11 = r10;
		r8 = 0x26u;
		do
		{
			mem_write16(r11, r0_2);
			r8--;
			r11 += 2u;
		} while (r8 != 0u);
		r10 += 0x80u;
		r9--;
	} while (r9 != 0u);
	mem_write16((r10 & ~0xFFFFu) | 0xFCC0u, 0u);
	return r0_2;
}

/* Starts the GAME ASSIGNMENTS: the settings' names and values (test_mode_game_assignments_draw), EXIT,
   the prompt, the cursor on EXIT (test_mode_page_cursor 9); then test_mode_game_assignments_run. */
void test_mode_game_assignments(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = test_mode_clear_text();
	r10 = 0x1DBF8u;
	test_mode_draw_texts(r0, r10);
	r0 = test_mode_game_assignments_draw();
	test_mode_draw_instructions(r0);
	test_mode_page_cursor = 9u;
	r10 = 0x310000u | rom_01DCE0;
	test_mode_set_cursor(r0, r10);
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
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((test_mode_buttons_pressed & 1u) != 0u)
	{
		test_mode_clear_cursor();
		r0 = test_mode_page_cursor;
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0xA)
			r0 = 0u;
		test_mode_page_cursor = r0;
		r10 = 0x310000u | rom_01DCCE[r0];
		test_mode_set_cursor(r0, r10);
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
			invalid_jump(rom_01DBD0[test_mode_page_cursor]);
		}
	}
	t1 = test_mode_blink_timer - 1u;
	test_mode_blink_timer = t1;
	if ((int8_t)t1 < 0)
	{
		test_mode_blink_timer = 5;
		t2 = test_mode_blink_address;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			test_mode_blink_timer = (uint32_t)test_mode_blink_timer << 1;
	}
}

/* Draws the game settings: game_difficulty + 1 (1-8) and its bar from EASY to HARD (two cells a level),
   the six stages' time_difficulty (NORMAL, or +n SEC / -n SEC), and steering_wheel_reaction and
   advertise_sound (bit 0) as On or Off. */
uint32_t test_mode_game_assignments_draw(void)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	current_object = settings_magic_ADDRESS;
	r10 = TEXT_CELL(32, 5);
	r1 = 0x64u;
	r2 = 2u;
	r0 = game_difficulty + 1u;
	test_mode_draw_number(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF3A8u;
	t1 = game_difficulty * 2u;
	r9 = t1;
	if (t1 != 0u)
	{
		do
		{
			mem_write16(r10, (mem_read16(r10) & 0x1FFu) | 0x6400u);
			r10 += 2u;
			r9--;
		} while (r9 != 0u);
	}
	r0 = 0x60u;
	test_mode_colour_line(r0, r10);
	r11 = TEXT_CELL(28, 10);
	r12 = current_object + 0x32u;
	r9 = 6u;
	do
	{
		r10 = r11;
		r0 = mem_read8(r12);
		r12 = r12 + 1u;
		if (r0 == 0u)
		{
			mem_write16(r10, 0x644Eu);
			r10 += 2u;
			mem_write16(r10, 0x644Fu);
			r10 += 2u;
			mem_write16(r10, 0x6452u);
			r10 += 2u;
			mem_write16(r10, 0x644Du);
			r10 += 2u;
			mem_write16(r10, 0x6441u);
			mem_write16(r10 + 2u, 0x644Cu);
		}
		else
		{
			r1_2 = 0x642Bu;
			if ((int8_t)r0 < 0)
			{
				r1_2 = (uint16_t)((r1_2 & ~0xFFu) | 0x2Du);
				r0 = (uint8_t)(0u - r0);
			}
			mem_write16(r10, r1_2);
			r1 = 0x64u;
			r2 = 2u;
			r10 += 2u;
			test_mode_draw_number(r2, &r0, &r1, &r10);
			r10 += 4u;
			mem_write16(r10, 0x6653u);
			r10 += 2u;
			mem_write16(r10, 0x6645u);
			mem_write16(r10 + 2u, 0x6643u);
		}
		r9--;
		r11 += 0x80u;
	} while (r9 != 0u);
	r0 = steering_wheel_reaction;
	r0 = r0 & 1u;
	r10 = rom_01DCE4[r0];
	test_mode_draw_texts(r0, r10);
	r0 = advertise_sound;
	r0 = r0 & 1u;
	r10 = rom_01DCEC[r0];
	return test_mode_draw_texts(r0, r10);
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
	uint8_t r0 = 0;
	uint32_t r1 = 0, ignored = 0;

	r1 = test_mode_page_cursor;
	r0 = (uint8_t)(mem_read8(current_object + 0x31u + r1) + 1u);
	if ((int8_t)r0 > 5)
		r0 = 0xFBu;
	mem_write8(current_object + 0x31u + r1, r0);
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
