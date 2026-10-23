/**
 *    Name:        game_04.c
 *    Description: the game's functions, part 04 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_01854A(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = current_object + 0x80u;
	r9 = AS32(OBJECT(current_object)->unk_10);
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
		SCREEN(screen)->unk_206 = 0u;
		OBJECT(current_object)->unk_00 = 0u;
	}
}

uint32_t sub_018581(uint8_t r0, uint8_t r1, uint32_t r10, uint32_t r11)
{
	uint8_t r0_2 = 0;
	uint32_t r21 = 0;

	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_0185D2_ADDRESS);
	mem_write16(r21 + 0x28u, 8u);
	mem_write32(r21 + 0x2Cu, r21 + 0x34u);
	mem_write16(r21 + 0x30u, r12);
	mem_write16(r21 + 0x32u, (uint8_t)r0);
	r0_2 = remainder_u8(SCREEN(screen)->unk_F0E, (uint8_t)r1);
	SCREEN(screen)->unk_F0E++;
	mem_write8(r21 + 0x10u, r0_2 + 1u);
	mem_write8(r21 + 0x12u, r1);
	mem_write32(r21 + 0x18u, r10);
	mem_write32(r21 + 0x1Cu, r11);
	mem_write32(r21 + 0x20u, r21 + 0x34u);
	mem_write32(r21 + 0x24u, r21 + 0x54u);
	mem_write8(r21 + 0x13u, 0x10u);
	return r21;
}

void sub_0185D2(void)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint8_t t1 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r11 = AS32(OBJECT(current_object)->unk_1C);
	r12 = AS32(OBJECT(current_object)->unk_20);
	r13 = AS32(OBJECT(current_object)->unk_24);
	r0 = mem_read16(r10);
	r1 = mem_read16(r11);
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
	t1 = OBJECT(current_object)->unk_13;
	OBJECT(current_object)->unk_13 = t1 - 1u;
	r10 += 2u;
	r11 += 2u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_04 = sub_01864A_ADDRESS;
		sub_01864A(r0);
		return;
	}
	OBJECT(current_object)->unk_18 = r10;
	AS32(OBJECT(current_object)->unk_1C) = r11;
	AS32(OBJECT(current_object)->unk_20) = r12;
	AS32(OBJECT(current_object)->unk_24) = r13;
}

void sub_01864A(uint32_t r0)
{
	bool L_0186AA = false, L_0186B2 = false, L_0186BA = false, L_0186CB = false, L_0186D1 = false, L_0186D7 = false, L_0186E0 = false, L_0186E9 = false, L_0186F2 = false;
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint8_t t1 = 0;

	if ((SCREEN(screen)->unk_94 & 1u) != 0u)
	{
		r0_2 = (r0 & ~0xFFu) | SCREEN(screen)->unk_153;
		if ((int8_t)r0_2 <= 5 && (r0_2 & 1u) == 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
	}
	t1 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(current_object)->unk_10 = OBJECT(current_object)->unk_12;
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
																					sub_0115C5(r19);
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
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_0186FC(void)
{
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r9 = 0, r10 = 0;

	unk_20E3F2 = 0u;
	unk_20E3F3 = 0u;
	unk_20E3F0 = 0xE10u;
	unk_208F20 = 0u;
	unk_218F20 = 0u;
	r0 = 0x30u;
	sub_0187F8(r0);
	r9 = 0x2008u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 = 0x30u;
	sub_0187F8(r0);
	r9 = 0xC8u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0 = 0x30u;
	sub_0187F8(r0);
	r0 = 0x20u;
	sub_0187F0(r0);
	r0 = 0u;
	sub_0187F0(r0);
	r0 = 0xC0u;
	sub_0187F0(r0);
	r0_2 = rom_018B90;
	r1 = rom_018B94;
	unk_208F10 = r0_2;
	unk_208F14 = r1;
	unk_218F10 = r0_2;
	unk_218F14 = r1;
	r0_2 = rom_018B98;
	r1 = rom_018B9C;
	unk_208F18 = r0_2;
	unk_208F1C = r1;
	unk_218F18 = r0_2;
	unk_218F1C = r1;
	r0_2 = (r0_2 & ~0xFFu) | 0x40u;
	r12 = unk_218F20_ADDRESS;
	sub_0187F0((uint8_t)r0_2);
	r0_2 &= ~0xFFu;
	sub_0187F0((uint8_t)r0_2);
	r10 = 0x18B4Eu;
	for (;;)
	{
		r0_2 = (r0_2 & ~0xFFu) | ROM8(r10);
		r10++;
		if ((int8_t)r0_2 < 0)
			break;
		r0_2 = sub_0187A3(r0_2);
	}
}

uint32_t sub_0187A3(uint32_t r0)
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
	sub_0187C1((uint8_t)r0);
	r9 = 0x50u;
	do
	{
		r9--;
	} while (r9 != 0u);
	r0_2 = (stack_4 & ~0xFFu) | (uint8_t)(stack_4 << 4);
	sub_0187C1((uint8_t)r0_2);
	return r0_2;
}

void sub_0187C1(uint8_t r0)
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

void sub_0187F0(uint8_t r0)
{
	uint32_t r9 = 0;

	r9 = 0x50u;
	do
	{
		r9--;
	} while (r9 != 0u);
	sub_0187F8(r0);
}

void sub_0187F8(uint8_t r0)
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

void sub_018827(void)
{
	uint32_t r0 = 0, r1 = 0;

	unk_20E3F0 = 0u;
	unk_208F20 = 0u;
	unk_218F20 = 0u;
	r0 = rom_018B90;
	r1 = rom_018B94;
	unk_208F10 = r0;
	unk_208F14 = r1;
	unk_218F10 = r0;
	unk_218F14 = r1;
	r0 = rom_018B98;
	r1 = rom_018B9C;
	unk_208F18 = r0;
	unk_208F1C = r1;
	unk_218F18 = r0;
	unk_218F1C = r1;
	r12 = unk_218F20_ADDRESS;
}

void sub_018863(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0;
	uint8_t t6 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0, t3 = 0, t4 = 0, t5 = 0;

	t1 = unk_20E3F0;
	unk_20E3F0 = t1 - 1u;
	if ((int16_t)t1 <= 1)
	{
		unk_20E3F4 = 0x2Cu;
		unk_20E3F0 = 0xE10u;
		return;
	}
	if (unk_20E3F3 != 0u)
		return;
	r0 = unk_20E3F2;
	carry = r0 & 1u;
	r0 = r0 >> 1;
	if (carry == 0u)
	{
		unk_20E3F2 = 0u;
		t2 = unk_208F14;
		AS32(unk_20E3C0[0]) = unk_208F10;
		unk_20E3C4 = t2;
		t3 = unk_208F1C;
		unk_20E3C8 = unk_208F18;
		unk_20E3CC = t3;
		t4 = unk_218F14;
		AS32(unk_20E3D0[0]) = unk_218F10;
		unk_20E3D4 = t4;
		t5 = unk_218F1C;
		unk_20E3D8 = unk_218F18;
		unk_20E3DC = t5;
		return;
	}
	t6 = r0;
	r0 = (uint8_t)(t6 - 1u);
	if (t6 != 1u)
		r0 = 4u;
	sub_0187F8(r0);
	unk_20E3F2 = 0u;
	t2 = unk_208F14;
	AS32(unk_20E3C0[0]) = unk_208F10;
	unk_20E3C4 = t2;
	t3 = unk_208F1C;
	unk_20E3C8 = unk_208F18;
	unk_20E3CC = t3;
	t4 = unk_218F14;
	AS32(unk_20E3D0[0]) = unk_218F10;
	unk_20E3D4 = t4;
	t5 = unk_218F1C;
	unk_20E3D8 = unk_218F18;
	unk_20E3DC = t5;
}

void sub_0188BE(void)
{
	uint32_t carry = 0;
	uint8_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0;
	uint8_t t1 = 0, t2 = 0;

	if (unk_20E3F4 != 0u)
	{
		unk_20E3F3 = 0xFFu;
		r0_2 = (uint8_t)~rom_018B21[unk_20E3F4];
		r1 = r0_2 | (uint8_t)(unk_20E094 >> 8u);
		mem_write8(IO_C0000C, r0_2 | (uint8_t)unk_20E094);
		mem_write8(0xC8000Cu, r1);
		t1 = unk_20E3F4;
		unk_20E3F4 = t1 - 1u;
		if (t1 == 1u)
		{
			unk_20E3F2 = 0x22u;
			unk_20E3F3 = 0u;
		}
		return;
	}
	r0 = unk_20E3F2;
	if (r0 >= 0x22u)
		return;
	unk_20E3F3 = 0xFFu;
	unk_20E3F2++;
	t2 = r0;
	r0 = (uint8_t)(t2 - 2u);
	if ((int8_t)t2 < 2)
	{
		r0 = (uint8_t)~r0;
		r0 = (uint8_t)~((r0 << 7) | (r0 >> 1) | 0xBu);
		r1 = (uint8_t)r0;
		r0 = r0 | (uint8_t)unk_20E094;
		r1 |= (uint8_t)(unk_20E094 >> 8u);
		mem_write8(IO_C0000C, r0);
		mem_write8(0xC8000Cu, r1);
		r9 = 0x14u;
		do
		{
			r9--;
		} while (r9 != 0u);
		mem_write8(IO_C0000C, r0 | 8u);
		mem_write8(0xC8000Cu, r1 | 8u);
		unk_20E3F3 = 0u;
		return;
	}
	carry = r0 & 1u;
	r0 = r0 >> 1;
	r1 = unk_20E3D0[r0];
	r0_2 = unk_20E3C0[r0];
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
	r0_2 = (uint8_t)(~(r0_2 | 0xFu) | (uint8_t)unk_20E094);
	r1 = (uint8_t)(~(r1 | 0xFu) | (uint8_t)(unk_20E094 >> 8u));
	mem_write8(IO_C0000C, r0_2);
	mem_write8(0xC8000Cu, r1);
	r9 = 0x14u;
	do
	{
		r9--;
	} while (r9 != 0u);
	mem_write8(IO_C0000C, r0_2 | 8u);
	mem_write8(0xC8000Cu, r1 | 8u);
	unk_20E3F3 = 0u;
}

void sub_0189D9(void)
{
	uint8_t r1_2 = 0, r26 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	r10 = screen + 0xF10u;
	if (SCREEN(screen)->unk_F35 == 0u)
		r0 = 0xFu;
	else
	{
		r11 = SCREEN(screen)->unk_F30;
		if (r11 == 0u)
		{
			SCREEN(r10 - 0xF10u)->unk_F10 = rom_018AE8[0];
			SCREEN(screen)->unk_F34 = 3u;
			r0 = 0xDu;
			r10 += 2u;
		}
		else
		{
			t1 = SCREEN(screen)->unk_F34;
			SCREEN(screen)->unk_F34 = t1 - 1u;
			if (t1 != 1u)
			{
				r10 += 2u;
				r0 = 0xDu;
			}
			else
			{
				r0 = mem_read8(r11);
				r11++;
				if ((int8_t)r0 < 0)
				{
					SCREEN(screen)->unk_F30 = 0u;
					SCREEN(r10 - 0xF10u)->unk_F10 = rom_018AE8[0];
					SCREEN(screen)->unk_F34 = 3u;
					r0 = 0xDu;
					r10 += 2u;
				}
				else
				{
					SCREEN(r10 - 0xF10u)->unk_F10 = rom_018AE8[r0];
					SCREEN(screen)->unk_F30 = r11;
					SCREEN(screen)->unk_F34 = 3u;
					r0 = 0xDu;
					r10 += 2u;
				}
			}
		}
	}
	r1 = SCREEN(screen)->unk_F40;
	if (r1 != 0u)
	{
		SCREEN(screen)->unk_F44 = r1;
		SCREEN(screen)->unk_F40 = 0u;
		SCREEN(screen)->unk_F48 = 0u;
		r26 = 0x20u;
		move_string(r10, 0u, r10, r0 + 1u, 1u, 1u, 0u, r26, &r27, &r28);
	}
	r11 = SCREEN(screen)->unk_F44;
	if (r11 == 0u)
		return;
	switch (SCREEN(screen)->unk_F48)
	{
	case 0:
		r1_2 = mem_read8(r11);
		SCREEN(screen)->unk_F44 = r11 + 1u;
		if (r1_2 == 0x60u)
			r1_2 = 7u;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, r1_2);
		SCREEN(screen)->unk_F49 = 5u;
		SCREEN(screen)->unk_F48++;
		t2 = SCREEN(screen)->unk_F49;
		SCREEN(screen)->unk_F49 = t2 - 1u;
		if (t2 != 1u)
			return;
		if (mem_read8(SCREEN(screen)->unk_F44) != 0u)
		{
			SCREEN(screen)->unk_F48--;
			return;
		}
		SCREEN(screen)->unk_F44 = r0 + 1u;
		SCREEN(screen)->unk_F48++;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->unk_F49 = 5u;
		SCREEN(screen)->unk_F48++;
		break;
	case 1:
		t2 = SCREEN(screen)->unk_F49;
		SCREEN(screen)->unk_F49 = t2 - 1u;
		if (t2 != 1u)
			return;
		if (mem_read8(SCREEN(screen)->unk_F44) != 0u)
		{
			SCREEN(screen)->unk_F48--;
			return;
		}
		SCREEN(screen)->unk_F44 = r0 + 1u;
		SCREEN(screen)->unk_F48++;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->unk_F49 = 5u;
		SCREEN(screen)->unk_F48++;
		break;
	case 2:
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, 0u, &r27, &r28);
		mem_write8(r27, 0x20u);
		SCREEN(screen)->unk_F49 = 5u;
		SCREEN(screen)->unk_F48++;
		t3 = SCREEN(screen)->unk_F49;
		SCREEN(screen)->unk_F49 = t3 - 1u;
		if (t3 == 1u)
		{
			SCREEN(screen)->unk_F44--;
			SCREEN(screen)->unk_F48--;
		}
		return;
	case 3:
		break;
	default:
		invalid_jump(rom_018A70[SCREEN(screen)->unk_F48]);
	}
	t3 = SCREEN(screen)->unk_F49;
	SCREEN(screen)->unk_F49 = t3 - 1u;
	if (t3 != 1u)
		return;
	SCREEN(screen)->unk_F44--;
	SCREEN(screen)->unk_F48--;
}

void sub_018AF0(void)
{
	uint32_t r10 = 0;

	r10 = screen + 0xF10u;
	AS32(SCREEN(r10 - 0xF10u)->unk_F10) = 0x20202020u;
	r10 += 4u;
	SCREEN(r10 - 0xF14u)->unk_F14 = 0x20202020u;
	r10 += 4u;
	SCREEN(r10 - 0xF18u)->unk_F18 = 0x20202020u;
	SCREEN(r10 - 0xF18u)->unk_F1C = 0x20202020u;
	SCREEN(screen)->unk_F30 = 0u;
	SCREEN(screen)->unk_F34 = 3u;
	SCREEN(screen)->unk_F35 = 0u;
	SCREEN(screen)->unk_F40 = 0u;
	SCREEN(screen)->unk_F44 = 0u;
}

void sub_018BA0(void)
{
	sub_018AF0();
	SCREEN(screen)->unk_F35 = 0xFFu;
	OBJECT(current_object)->unk_04 = sub_018BB2_ADDRESS;
	sub_018BB2();
}

void sub_018BB2(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0;

	if (CAR(player_car)->unk_181 != 0u)
	{
		OBJECT(current_object)->unk_0C = 0u;
		OBJECT(current_object)->unk_04 = sub_018E32_ADDRESS;
		sub_018E32();
		return;
	}
	if (CAR(player_car)->unk_182 != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		sub_018DBC();
		return;
	}
	if (SCREEN(screen)->unk_F44 != 0u)
	{
		OBJECT(current_object)->unk_0C = 1u;
		OBJECT(current_object)->unk_04 = sub_018E98_ADDRESS;
		sub_018E98();
		return;
	}
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = mem_read16(CAR(player_car)->unk_00 + 0x10u);
		if (r0 != 0u)
		{
			r1 = 0u;
			divux_pair(0x1Eu, &r0, &r1);
			move_string(rom_018EA0[(r1 + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			AT8(SCREEN(screen)->unk_F18, 1) = r0 + 0x31u;
			return;
		}
		AT8(OBJECT(current_object)->unk_08, 1) = mem_read8(SCREEN(screen)->unk_308 + 0x1Fu);
		OBJECT(current_object)->unk_18 = SCREEN(screen)->unk_308 + 0x20u;
		OBJECT(current_object)->unk_12 = 0x96u;
		OBJECT(current_object)->unk_13 = 0u;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_0E = 0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		OBJECT(current_object)->unk_0E = 0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_018BD4[OBJECT(current_object)->unk_0C]);
	}
	t1 = AS16(OBJECT(current_object)->unk_12);
	AS16(OBJECT(current_object)->unk_12) = t1 - 1u;
	if (t1 == 1u)
	{
		r0_2 = (uint8_t)(OBJECT(current_object)->unk_0D + 1u);
		if ((int8_t)r0_2 >= 5)
			r0_2 = 0u;
		OBJECT(current_object)->unk_0D = r0_2;
		OBJECT(current_object)->unk_0E = 0u;
		OBJECT(current_object)->unk_12 = 0x96u;
		OBJECT(current_object)->unk_13 = 0u;
	}
	switch (OBJECT(current_object)->unk_0D)
	{
	case 0:
		switch (OBJECT(current_object)->unk_0E)
		{
		case 0:
			sub_018C8C();
			return;
		case 1:
			sub_018C99();
			return;
		default:
			invalid_jump(rom_018C84[OBJECT(current_object)->unk_0E]);
		}
		break;
	case 1:
		break;
	case 2:
		switch (OBJECT(current_object)->unk_0E)
		{
		case 0:
			move_string(0x18F3Au, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->unk_0E++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D5C[OBJECT(current_object)->unk_0E]);
		}
		r10 = screen + 0xF1Cu;
		r0 = SCREEN(screen)->unk_F50;
		sub_018DDF(r0, r10);
		return;
	case 3:
		switch (OBJECT(current_object)->unk_0E)
		{
		case 0:
			move_string(0x18F48u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
			OBJECT(current_object)->unk_0E++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D88[OBJECT(current_object)->unk_0E]);
		}
		r10 = screen + 0xF1Cu;
		r0 = SCREEN(screen)->unk_F52;
		sub_018DDF(r0, r10);
		return;
	case 4:
		switch (OBJECT(current_object)->unk_0E)
		{
		case 0:
			sub_018DBC();
			return;
		case 1:
			sub_018DC9();
			return;
		default:
			invalid_jump(rom_018DB4[OBJECT(current_object)->unk_0E]);
		}
		break;
	default:
		invalid_jump(rom_018C64[OBJECT(current_object)->unk_0D]);
	}
	switch (OBJECT(current_object)->unk_0E)
	{
	case 0:
		if (mem_read8(SCREEN(screen)->unk_308 + 0x1Fu) == 1u)
		{
			OBJECT(current_object)->unk_0D++;
			switch (OBJECT(current_object)->unk_0E)
			{
			case 0:
				move_string(0x18F3Au, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
				OBJECT(current_object)->unk_0E++;
				break;
			case 1:
				break;
			default:
				invalid_jump(rom_018D5C[OBJECT(current_object)->unk_0E]);
			}
			r10 = screen + 0xF1Cu;
			r0 = SCREEN(screen)->unk_F50;
			sub_018DDF(r0, r10);
			return;
		}
		move_string(0x18F2Cu, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0E++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_018CF0[OBJECT(current_object)->unk_0E]);
	}
	r10 = screen + 0xF18u;
	r11 = OBJECT(current_object)->unk_18;
	r9 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	do
	{
		r0 = mem_read8(r11);
		r11++;
		if (SCREEN(screen)->unk_01 != r0 || (OBJECT(current_object)->unk_10 & 8u) == 0u)
			r0_2 = (uint8_t)(unk_20E030[r0] + 0x31u);
		else
			r0_2 = 0x20u;
		mem_write8(r10, r0_2);
		r9--;
		r10++;
	} while (r9 != 0u);
	AS16(OBJECT(current_object)->unk_10)++;
}

void sub_018C8C(void)
{
	move_string(0x18F1Eu, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->unk_0E++;
	sub_018C99();
}

void sub_018C99(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r0 = CAR(player_car)->unk_04 / 0xAu;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = screen + 0xF20u - 1u;
	AT8(SCREEN(r10 - 0xF1Fu)->unk_F1C, 3) = r1 + 0x30u;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	AT8(SCREEN(r10 - 0xF1Eu)->unk_F1C, 2) = r1 + 0x30u;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = r10 - 1u - 1u;
	AS8(SCREEN(r10 - 0xF1Cu)->unk_F1C) = r1 + 0x30u;
	r1 = 0u;
	divx_pair(6u, &r0, &r1);
	r10--;
	AT8(SCREEN(r10 - 0xF1Bu)->unk_F18, 3) = r1 + 0x30u;
	mem_write8(r10 - 1u - 1u, r0 + 0x30u);
}

void sub_018DBC(void)
{
	move_string(0x18F56u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
	OBJECT(current_object)->unk_0E++;
	sub_018DC9();
}

void sub_018DC9(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = screen + 0xF1Cu;
	r0 = SCREEN(screen)->unk_F54;
	r1 = SCREEN(screen)->unk_F58;
	if (r1 == 0u)
	{
		sub_018DDF(r0, r10);
		return;
	}
	r0 = divide_u32(r0, r1);
	sub_018DDF(r0, r10);
}

void sub_018DDF(uint32_t r0, uint32_t r10)
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
		AT8(SCREEN(r10 - 0xF1Bu)->unk_F18, 3) = r1 + r2;
		r10--;
		AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 2) = 0x20u;
		AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 1) = 0x20u;
		return;
	}
	r10--;
	AT8(SCREEN(r10 - 0xF1Bu)->unk_F18, 3) = r1 + r2;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero)
	{
		r10--;
		AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 2) = r1 + r2;
		AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 1) = 0x20u;
		return;
	}
	r10--;
	AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 2) = r1 + r2;
	if (r0 == 0u)
	{
		AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 1) = 0x20u;
		return;
	}
	AT8(SCREEN(r10 - 0xF1Au)->unk_F18, 1) = r0 + r2;
}

void sub_018E32(void)
{
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		move_string(0x18F64u, 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->unk_0C++;
		if (SCREEN(screen)->unk_F44 == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		if (SCREEN(screen)->unk_F44 == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		if (SCREEN(screen)->unk_F44 != 0u)
			return;
		sub_018DBC();
		OBJECT(current_object)->unk_10 = 0x96u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_00 = 0u;
		sub_018C8C();
		return;
	case 2:
		break;
	case 3:
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_00 = 0u;
		sub_018C8C();
		return;
	default:
		invalid_jump(rom_018E40[OBJECT(current_object)->unk_0C]);
	}
	if (SCREEN(screen)->unk_F44 != 0u)
		return;
	sub_018DBC();
	OBJECT(current_object)->unk_10 = 0x96u;
	OBJECT(current_object)->unk_11 = 0u;
	OBJECT(current_object)->unk_0C++;
	t1 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(current_object)->unk_00 = 0u;
	sub_018C8C();
}

void sub_018E98(void)
{
	if (SCREEN(screen)->unk_F44 == 0u)
	{
		OBJECT(current_object)->unk_04 = sub_018BB2_ADDRESS;
		sub_018BB2();
		return;
	}
}

void sub_018F74(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, object = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->unk_80 = 0xFFFFFFFFu;
	OBJECT(object - 0x80u)->unk_84 = 0xFFFFu;
	OBJECT(object - 0x80u)->unk_86 = 0xFFu;
	OBJECT(object - 0x80u)->unk_87 = 0xFFu;
	r10 = SCREEN(screen)->unk_308 + 0x1Fu;
	r9 = mem_read8(r10);
	AS8(OBJECT(current_object)->unk_88) = r9;
	r10++;
	do
	{
		r0 = mem_read8(r10);
		mem_write8(current_object + 0x80u + unk_20E030[r0], mem_read8(0x20FC49u + (r0 << 7)));
		r9--;
		r10++;
	} while (r9 != 0u);
	AT8(OBJECT(current_object)->unk_08, 1) = unk_20E030[SCREEN(screen)->unk_01];
	SCREEN(screen)->unk_F0F = 0u;
	CAR(player_car)->unk_14 = rom_019421[CAR(player_car)->unk_60];
	AT8(OBJECT(current_object)->unk_08, 2) = SCREEN(screen)->unk_A1;
	OBJECT(current_object)->unk_04 = sub_018FD3_ADDRESS;
	sub_018FD3();
}

void sub_018FD3(void)
{
	if (mem_read16(CAR(player_car)->unk_00 + 0x10u) != 0u)
		return;
	OBJECT(current_object)->unk_04 = sub_018FE0_ADDRESS;
	sub_018FE0();
}

void sub_018FE0(void)
{
	OBJECT(current_object)->unk_2C = (uint16_t)SCREEN(screen)->unk_118;
	if ((CAR(player_car)->unk_181 | CAR(player_car)->unk_182) == 0u)
	{
		sub_01900A();
		return;
	}
	OBJECT(current_object)->unk_2C = 0u;
	OBJECT(current_object)->unk_0D = 5u;
	SCREEN(screen)->unk_F20 = 0u;
	OBJECT(current_object)->unk_04 = sub_01900A_ADDRESS;
	sub_01900A();
}

void sub_01900A(void)
{
	bool L_019118 = false, L_019132 = false, L_019137 = false, L_01914C = false, L_019154 = false, L_019167 = false, L_019180 = false;
	uint8_t r0 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r10 = 0, r21 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint32_t target1 = 0, target2 = 0;

	r21 = OBJECT(current_object)->unk_18;
	if (r21 != 0u && (mem_read32(r21) & 1u) == 0u)
		OBJECT(current_object)->unk_18 = 0u;
	if (SCREEN(screen)->unk_F0F != 0u)
	{
		r0 = SCREEN(screen)->unk_A1;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) != r0)
		{
			AT8(OBJECT(current_object)->unk_08, 2) = r0;
			SCREEN(screen)->unk_F0F = 0u;
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
							target1 = L_019118 ? 0x190F0u : L_019132 ? 0x190F0u : L_01914C ? 0x190F0u : L_019154 ? 0x190F0u : L_019167 ? 0x190F0u : L_019180 ? 0x190F0u : rom_019044[OBJECT(current_object)->unk_0C];
							switch (target1)
							{
							case 0x019068u:
								r0_2 = CAR(player_car)->unk_14;
								r0_2 = sub_0192E5(r0_2);
								sub_011EB8((uint8_t)r0_2);
								OBJECT(current_object)->unk_0C++;
							case 0x019078u:
								r0_3 = OBJECT(current_object)->unk_2C;
								if ((r0_3 & 0x100u) != 0u)
								{
									OBJECT(current_object)->unk_0C = 2u;
									r0 = 0x1Fu;
									sub_011EB8(r0);
									return;
								}
								if ((r0_3 & 0x200u) != 0u)
								{
									if ((r0_3 & 0x400u) != 0u)
									{
										r0_2 = 0xAu;
										r0_2 = sub_0192E5(r0_2);
										sub_011EB8((uint8_t)r0_2);
										return;
									}
									r0 = 0x1Fu;
									sub_011EB8(r0);
									r0_2 = CAR(player_car)->unk_14;
									t1 = r0_2 - 1u;
									r0_2 = t1;
									if ((int8_t)t1 < 0)
										r0_2 = 9u;
									CAR(player_car)->unk_14 = r0_2;
									r0_2 = sub_0192E5(r0_2);
									sub_011EB8((uint8_t)r0_2);
									return;
								}
								if ((r0_3 & 0x400u) == 0u && (r0_3 & 0x40u) == 0u)
									return;
								r0 = 0x1Fu;
								sub_011EB8(r0);
								r0_2 = CAR(player_car)->unk_14;
								r0_2 = (uint8_t)(r0_2 + 1u);
								if ((int8_t)r0_2 < 0xA)
								{
									CAR(player_car)->unk_14 = r0_2;
									r0_2 = sub_0192E5(r0_2);
									sub_011EB8((uint8_t)r0_2);
									return;
								}
								r0_2 = 0u;
								CAR(player_car)->unk_14 = r0_2;
								r0_2 = sub_0192E5(r0_2);
								sub_011EB8((uint8_t)r0_2);
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
														sub_0192F5(r10);
														AT8(OBJECT(current_object)->unk_08, 3) = 0x29u;
													}
												}
											}
										}
									}
								}
								do
								{
									if (L_019137 || (!L_019118 && !L_019132 && (L_01914C || L_019154 || L_019167 || L_019180 || SCREEN(screen)->unk_F0F != 0u)))
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
														r0 = 0xE0u;
														sub_011EB8(r0);
														OBJECT(current_object)->unk_10 = 0xB4u;
														OBJECT(current_object)->unk_0C = 5u;
													}
													L_01914C = false;
													t2 = OBJECT(current_object)->unk_10;
													OBJECT(current_object)->unk_10 = t2 - 1u;
												}
											}
										}
										if (L_019154 || L_019167 || L_019180 || t2 == 1u)
										{
											if (L_019154 || !L_019167)
											{
												if (L_019154 || !L_019180)
												{
													if (!L_019154)
														OBJECT(current_object)->unk_0C++;
													L_019154 = false;
												}
											}
											if (!L_019167 && !L_019180 && SCREEN(screen)->unk_F0F == 0u)
											{
												AT8(OBJECT(current_object)->unk_08, 3) = rom_019571[SCREEN(screen)->unk_A4];
												r0 = rom_019458[SCREEN(screen)->unk_A1];
												sub_011EB8(r0);
												OBJECT(current_object)->unk_0C = 3u;
											}
											else
											{
												if (L_019167 || !L_019180)
												{
													if (!L_019167)
													{
														r0 = 0xEEu;
														sub_011EB8(r0);
														OBJECT(current_object)->unk_0C++;
													}
													L_019167 = false;
												}
												if (L_019180 || SCREEN(screen)->unk_F0F == 0u)
												{
													if (!L_019180)
													{
														r0 = 0x7Fu;
														sub_011FEA(r0);
														OBJECT(current_object)->unk_10 = 0x78u;
														OBJECT(current_object)->unk_0C++;
													}
													L_019180 = false;
													t3 = OBJECT(current_object)->unk_10;
													OBJECT(current_object)->unk_10 = t3 - 1u;
													if (t3 == 1u)
													{
														AT8(OBJECT(current_object)->unk_08, 3) = rom_019571[SCREEN(screen)->unk_A4];
														r0 = rom_019458[SCREEN(screen)->unk_A1];
														sub_011EB8(r0);
														OBJECT(current_object)->unk_0C = 3u;
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
												r0 = rom_019458[SCREEN(screen)->unk_A1];
												sub_011EB8(r0);
												OBJECT(current_object)->unk_0C++;
											}
											L_019118 = false;
										}
										if (L_019132 || SCREEN(screen)->unk_F0F != 0u)
										{
											if (!L_019132)
											{
												r0 = 0x3Fu;
												sub_011FEA(r0);
												OBJECT(current_object)->unk_10 = 0x3Cu;
												OBJECT(current_object)->unk_0C++;
											}
											L_019132 = false;
											t4 = OBJECT(current_object)->unk_10;
											OBJECT(current_object)->unk_10 = t4 - 1u;
											L_019137 = t4 == 1u;
										}
									}
								} while (L_019137);
								if ((OBJECT(current_object)->unk_2C & 0x700u) != 0u)
								{
									OBJECT(current_object)->unk_0C = 0u;
									OBJECT(current_object)->unk_0D = 4u;
									SCREEN(screen)->unk_F30 = 0u;
									r0 = 0x1Fu;
									sub_011EB8(r0);
									return;
								}
								if (SCREEN(screen)->unk_F30 != 0u)
								{
									AT8(OBJECT(current_object)->unk_08, 3) = 0u;
									return;
								}
								switch (OBJECT(current_object)->unk_0D)
								{
								case 0:
									r0 = 0u;
									sub_019402(r0);
									OBJECT(current_object)->unk_0D++;
									return;
								case 1:
									r0_2 = (uint8_t)(OBJECT(current_object)->unk_88 >> 8u);
									while ((int8_t)mem_read8(current_object + 0x80u + r0_2) < 0)
									{
										r0_2 = (uint8_t)(r0_2 + 1u);
									}
									AT8(OBJECT(current_object)->unk_88, 1) = r0_2;
									r0_2 = (uint8_t)(r0_2 + 9u);
									sub_019402((uint8_t)r0_2);
									OBJECT(current_object)->unk_0D++;
									return;
								case 2:
									r0 = (uint8_t)(mem_read8(current_object + 0x80u + (uint8_t)(OBJECT(current_object)->unk_88 >> 8u)) + 1u);
									sub_019402(r0);
									AT8(OBJECT(current_object)->unk_88, 1)++;
									t5 = (uint8_t)OBJECT(current_object)->unk_88;
									AS8(OBJECT(current_object)->unk_88) = t5 - 1u;
									if (t5 == 1u)
									{
										OBJECT(current_object)->unk_0D++;
										return;
									}
									OBJECT(current_object)->unk_0D--;
									return;
								case 3:
									r0 = 0x2Au;
									sub_019402(r0);
									OBJECT(current_object)->unk_0D++;
									return;
								case 4:
									r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
									if (r0 != 0u)
									{
										OBJECT(current_object)->unk_38 = 0u;
										OBJECT(current_object)->unk_3A = 0u;
										sub_019402(r0);
										return;
									}
									if (OBJECT(current_object)->unk_0C != 3u)
										return;
									r10 = AS32(OBJECT(current_object)->unk_38);
									if (r10 == 0u)
									{
										r0_2 = sub_011E45();
										r0_2 &= 0x3FFu;
										if ((int32_t)r0_2 >= 0xE)
											return;
										r10 = routine_lists_1966C[r0_2];
									}
									r0_2 = mem_read32(r10);
									r10 += 4u;
									if (mem_read32(r10) == 0u)
										r10 = 0u;
									AS32(OBJECT(current_object)->unk_38) = r10;
									target2 = r0_2;
									switch (target2)
									{
									case 0x01929Eu:
										sub_01929E();
										return;
									case 0x0192B1u:
										sub_0192B1();
										return;
									case 0x0192BCu:
										sub_0192BC();
										return;
									case 0x0192C3u:
										sub_0192C3();
										return;
									case 0x0192CAu:
										sub_0192CA();
										return;
									case 0x0192D1u:
										sub_0192D1();
										return;
									case 0x0192D8u:
										sub_0192D8();
										return;
									default:
										invalid_jump(target2);
									}
									break;
								case 5:
									return;
								default:
									invalid_jump(rom_0191E0[OBJECT(current_object)->unk_0D]);
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
								invalid_jump(target1);
							}
						} while (L_019118);
					} while (L_019132);
				} while (L_01914C);
			} while (L_019154);
		} while (L_019167);
	} while (L_019180);
}

void sub_01929E(void)
{
	uint8_t r0 = 0;

	r0 = (uint8_t)(unk_20E030[mem_read8(SCREEN(screen)->unk_308 + 0x20u)] + 0x11u);
	sub_019402(r0);
}

void sub_0192B1(void)
{
	uint8_t r0 = 0;

	r0 = (uint8_t)((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) + 0x19u);
	sub_019402(r0);
}

void sub_0192BC(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Au;
	sub_019402(r0);
}

void sub_0192C3(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Bu;
	sub_019402(r0);
}

void sub_0192CA(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Du;
	sub_019402(r0);
}

void sub_0192D1(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Fu;
	sub_019402(r0);
}

void sub_0192D8(void)
{
	uint8_t r0 = 0;

	r0 = 0x2Cu;
	sub_019402(r0);
}

uint32_t sub_0192E5(uint32_t r0)
{
	uint32_t r0_2 = 0, r10 = 0;

	r10 = rom_01942C[r0];
	r0_2 = byte_swap32(r10);
	r10 &= 0xFFFFFFu;
	sub_0192F5(r10);
	return r0_2;
}

void sub_0192F5(uint32_t r10)
{
	uint32_t r21 = 0;

	r21 = OBJECT(current_object)->unk_18;
	if (r21 != 0u)
		mem_write16(r21 + 0x10u, 0u);
	r21 = sub_01930A(r10);
	OBJECT(current_object)->unk_18 = r21;
}

uint32_t sub_01930A(uint32_t r10)
{
	uint32_t r21 = 0;

	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_019322_ADDRESS);
	mem_write32(r21 + 0x18u, r10);
	mem_write16(r21 + 0x10u, 0x3Cu);
	return r21;
}

void sub_019322(void)
{
	bool L_019387 = false, L_01938A = false;
	uint32_t negative = 0;
	uint16_t r0 = 0, r5_2 = 0;
	uint32_t r3 = 0, r5 = 0, r10 = 0, r18 = 0;
	uint16_t t1 = 0, t2 = 0;
	uint32_t target1 = 0;

	do
	{
		target1 = L_01938A ? 0x19369u : rom_019330[OBJECT(current_object)->unk_0C];
		switch (target1)
		{
		case 0x019344u:
			r10 = OBJECT(current_object)->unk_18;
			OBJECT(current_object)->unk_30 = mem_read16(r10);
			r10 += 2u;
			OBJECT(current_object)->unk_18 = r10;
			SCREEN(screen)->unk_F40 = r10;
			OBJECT(current_object)->unk_20 = 0xD2u;
			OBJECT(current_object)->unk_22 = 0xC8u;
			OBJECT(current_object)->unk_23 = 0u;
			OBJECT(current_object)->unk_12 = 0x14u;
			OBJECT(current_object)->unk_13 = 0u;
			OBJECT(current_object)->unk_0C++;
		case 0x019369u:
			do
			{
				if (L_019387 || L_01938A || (OBJECT(current_object)->unk_10 | OBJECT(current_object)->unk_11) == 0u)
				{
					if (L_019387 || !L_01938A)
					{
						L_019387 = false;
						OBJECT(current_object)->unk_0C++;
					}
					L_01938A = false;
					t1 = AS16(OBJECT(current_object)->unk_10);
					t2 = t1 - 1u;
					negative = t2 >> 15;
					AS16(OBJECT(current_object)->unk_10) = t2;
					if ((int16_t)t1 > 1)
					{
						r3 = AS32(OBJECT(current_object)->unk_20);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->unk_30);
						r5 = OBJECT(current_object)->unk_0D;
						r5 = (uint8_t)(r5 + 1u);
						OBJECT(current_object)->unk_0D = r5;
						r5 = r5 >> 1;
						r5_2 = rom_019561[r5 & 7u];
						r18 = OBJECT(current_object)->unk_18;
						sub_0119FA(r3, r5_2, r18);
						return;
					}
					if (negative)
					{
						OBJECT(current_object)->unk_0C += 2u;
						r3 = AS32(OBJECT(current_object)->unk_20);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
						AS32(OBJECT(current_object)->unk_20) = r3;
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(current_object)->unk_00 = 0u;
							return;
						}
					}
					else
					{
						OBJECT(current_object)->unk_0C++;
						r3 = AS32(OBJECT(current_object)->unk_20);
						r0 = (uint16_t)(AS16(OBJECT(current_object)->unk_12) + 1u);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
						AS16(OBJECT(current_object)->unk_12) = r0;
						AS32(OBJECT(current_object)->unk_20) = r3;
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(current_object)->unk_00 = 0u;
							return;
						}
					}
				}
				else
				{
					r3 = AS32(OBJECT(current_object)->unk_20);
					r0 = AS16(OBJECT(current_object)->unk_12);
					r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
					AS16(OBJECT(current_object)->unk_12) = r0 - 1u;
					AS32(OBJECT(current_object)->unk_20) = r3;
					L_019387 = (uint16_t)r3 == 0u;
				}
			} while (L_019387);
			break;
		case 0x01938Au:
			L_01938A = true;
			break;
		case 0x019394u:
			r3 = AS32(OBJECT(current_object)->unk_20);
			r0 = (uint16_t)(AS16(OBJECT(current_object)->unk_12) + 1u);
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
			AS16(OBJECT(current_object)->unk_12) = r0;
			AS32(OBJECT(current_object)->unk_20) = r3;
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->unk_30);
			r5 = OBJECT(current_object)->unk_0D;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(current_object)->unk_0D = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			r18 = OBJECT(current_object)->unk_18;
			sub_0119FA(r3, r5_2, r18);
			return;
		case 0x0193BAu:
			r3 = AS32(OBJECT(current_object)->unk_20);
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
			AS32(OBJECT(current_object)->unk_20) = r3;
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->unk_30);
			r5 = OBJECT(current_object)->unk_0D;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(current_object)->unk_0D = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			r18 = OBJECT(current_object)->unk_18;
			sub_0119FA(r3, r5_2, r18);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_01938A);
	r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(current_object)->unk_30);
	r5 = OBJECT(current_object)->unk_0D;
	r5 = (uint8_t)(r5 + 1u);
	OBJECT(current_object)->unk_0D = r5;
	r5 = r5 >> 1;
	r5_2 = rom_019561[r5 & 7u];
	r18 = OBJECT(current_object)->unk_18;
	sub_0119FA(r3, r5_2, r18);
}

void sub_0193FC(uint8_t r0)
{
	if (SCREEN(screen)->unk_F30 != 0u)
		return;
	sub_019402(r0);
}

void sub_019402(uint8_t r0)
{
	uint8_t r0_2 = 0;
	uint32_t r10 = 0;

	r10 = typed_messages[(uint8_t)r0];
	r0_2 = (uint8_t)byte_swap32(r10);
	SCREEN(screen)->unk_F30 = r10 & 0xFFFFFFu;
	sub_011EB8(r0_2);
}

void sub_019CAC(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0, r19 = 0, r21 = 0;
	uint8_t t4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t5 = 0, t6 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		AS8(SCREEN(screen)->unk_5800) = 0u;
		AT8(SCREEN(screen)->unk_5800, 1) = 0u;
		r19 = 0x1ACCCu + ((uint32_t)SCREEN(screen)->unk_00 << 3) * 4u;
		AS32(OBJECT(current_object)->unk_84) = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_88 = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_8C = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_90 = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_94 = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_98 = ROM32(r19);
		r19 += 4u;
		OBJECT(current_object)->unk_9C = ROM32(r19);
		OBJECT(current_object)->unk_A0 = ROM32(r19 + 4u);
		r19 = 0x1AD0Cu + SCREEN(screen)->unk_00 * 5u * 4u;
		sub_0179AC(r19);
		mem_write16(OBJECT(current_object)->unk_A0, 0xFFF0u);
		mem_write16(OBJECT(current_object)->unk_8C, 0x360u);
		t1 = OBJECT(current_object)->unk_8C;
		mem_write16(t1, mem_read16(t1) + ((SCREEN(screen)->unk_02 * 5u) << 6));
		mem_write16(OBJECT(current_object)->unk_90, 0x72u);
		r0 = SCREEN(screen)->unk_9E;
		SCREEN(screen)->unk_9C = r0;
		SCREEN(screen)->unk_9E = (r0 + 1u) & 7u;
		SCREEN(screen)->unk_41 = SCREEN(screen)->unk_9C;
		SCREEN(screen)->unk_40 = 5u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01AA01_ADDRESS);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A6CF_ADDRESS);
		r1 = 8u;
		r0 = 0u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_019E6B_ADDRESS);
			mem_write8(r21 + 9u, r0);
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->unk_10 = 0x2Du;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		sub_01A0A2();
		t2 = OBJECT(current_object)->unk_8C;
		mem_write16(t2, mem_read16(t2) + 8u);
		t3 = OBJECT(current_object)->unk_9C;
		mem_write16(t3, mem_read16(t3) + 0x14u);
		t4 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t4 - 1u;
		if (t4 != 1u)
			return;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A18C_ADDRESS);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A29C_ADDRESS);
		r1 = 3u;
		r0 = 0u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_01A329_ADDRESS);
			mem_write8(r21 + 9u, r0);
			r0++;
			r1--;
		} while (r1 != 0u);
		r1 = 3u;
		r0 = 0u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_01A3E9_ADDRESS);
			mem_write8(r21 + 9u, r0);
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->unk_80 = 0x1ACC0u;
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 2:
		sub_01A0A2();
		t5 = OBJECT(current_object)->unk_8C;
		mem_write16(t5, mem_read16(t5) + 8u);
		t6 = OBJECT(current_object)->unk_9C;
		mem_write16(t6, mem_read16(t6) + 0x14u);
		AS16(OBJECT(current_object)->unk_10)++;
		if (AS16(OBJECT(current_object)->unk_10) != mem_read16(OBJECT(current_object)->unk_80))
			return;
		r1 = 3u;
		r0 = 0u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_01A329_ADDRESS);
			mem_write8(r21 + 9u, r0);
			r0++;
			r1--;
		} while (r1 != 0u);
		OBJECT(current_object)->unk_80 += 2u;
		return;
	default:
		invalid_jump(rom_019CB8[OBJECT(current_object)->unk_0C]);
	}
}

void sub_019E6B(void)
{
	uint16_t r2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_019E78[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r10 = 0x1ABB4u + ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4);
		r0 = mem_read16(r10);
		r9 = rom_01AC34[r0];
		r10 += 2u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_34 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_36 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_32 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_2C = mem_read16(r10);
		OBJECT(current_object)->unk_40 = 9u;
		OBJECT(current_object)->unk_3C = 0x18Fu;
		OBJECT(current_object)->unk_3C += mem_read16(r10 + 2u);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_019F60_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r0);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_019F4C_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r0);
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_019E78[t1]);
	}
	OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_2C;
	if ((int16_t)OBJECT(current_object)->unk_20 >= 0x1000)
	{
		OBJECT(current_object)->unk_00 &= 0xDFu;
		OBJECT(current_object)->unk_20 &= 0xFFFu;
	}
	OBJECT(current_object)->unk_30 = OBJECT(current_object)->unk_20;
	OBJECT(current_object)->unk_30 -= rom_01ABA4[SCREEN(screen)->unk_02];
	AS16(OBJECT(current_object)->unk_10)++;
	r2 = AS16(OBJECT(current_object)->unk_10) & 1u;
	OBJECT(current_object)->unk_24 = OBJECT(current_object)->unk_32;
	OBJECT(current_object)->unk_32 -= r2;
	sub_00EAE4(r0);
	OBJECT(current_object)->unk_32 = OBJECT(current_object)->unk_24;
}

void sub_019F4C(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_3A = rom_01A032[r0];
		AS16(OBJECT(current_object)->unk_10) = rom_01A062[r0];
		AS16(OBJECT(current_object)->unk_12) = rom_01A072[r0];
		r9 = rom_01A012[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = mem_read16(OBJECT(current_object)->unk_14 + 0x3Cu);
		OBJECT(current_object)->unk_3C++;
		return;
	case 1:
		sub_019FD3();
		return;
	default:
		invalid_jump(rom_019F58[OBJECT(current_object)->unk_0C]);
	}
}

void sub_019F60(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		break;
	case 1:
		sub_019FD3();
		return;
	default:
		invalid_jump(rom_019F6C[OBJECT(current_object)->unk_0C]);
	}
	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	OBJECT(current_object)->unk_3A = rom_01A042[r0];
	AS16(OBJECT(current_object)->unk_10) = rom_01A082[r0];
	AS16(OBJECT(current_object)->unk_12) = rom_01A092[r0];
	r9 = rom_01A022[r0];
	sub_00F366(r9);
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = mem_read16(OBJECT(current_object)->unk_14 + 0x3Cu);
	OBJECT(current_object)->unk_3C++;
}

void sub_019FD3(void)
{
	uint32_t r0 = 0;

	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	OBJECT(current_object)->unk_34 = rom_01A052[r0];
	OBJECT(current_object)->unk_36 = rom_01A052[r0];
	OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u) + AS16(OBJECT(current_object)->unk_10);
	r0 = (uint16_t)(mem_read16(OBJECT(current_object)->unk_14 + 0x32u) + AS16(OBJECT(current_object)->unk_12) - (mem_read16(OBJECT(current_object)->unk_14 + 0x10u) & 1u));
	OBJECT(current_object)->unk_32 = r0;
	r0 = sub_00EAE4(r0);
}

void sub_01A0A2(void)
{
	uint32_t r21 = 0;

	OBJECT(current_object)->unk_12++;
	if ((OBJECT(current_object)->unk_12 & 7u) == 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A0E2_ADDRESS);
		mem_write8(r21 + 9u, 0u);
		return;
	}
	if ((OBJECT(current_object)->unk_12 & 1u) == 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A0E2_ADDRESS);
		mem_write8(r21 + 9u, 1u);
		return;
	}
}

void sub_01A0E2(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A0F0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x59u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x6Du;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
		{
			OBJECT(current_object)->unk_34 = 0x60u;
			OBJECT(current_object)->unk_36 = 0x60u;
			OBJECT(current_object)->unk_30 = 0x170u;
			OBJECT(current_object)->unk_32 = 0x96u;
			OBJECT(current_object)->unk_3C = 0x185u;
		}
		else
		{
			OBJECT(current_object)->unk_34 = 0x100u;
			OBJECT(current_object)->unk_36 = 0x100u;
			OBJECT(current_object)->unk_30 = 0x170u;
			OBJECT(current_object)->unk_32 = 0xF0u;
			OBJECT(current_object)->unk_3C = 0x194u;
		}
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01A0F0[t1]);
	}
	if ((int16_t)OBJECT(current_object)->unk_30 <= -0x30)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
	{
		OBJECT(current_object)->unk_30 -= 0x12u;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_30 -= 0x30u;
	r0 = sub_00EAE4(r0);
}

void sub_01A18C(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A198[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x25Du;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x26Au;
		OBJECT(current_object)->unk_30 = 0x142u;
		OBJECT(current_object)->unk_32 = 0x47u;
		OBJECT(current_object)->unk_3C = 0x199u;
		OBJECT(current_object)->unk_10 = 0xBu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_3A = 0x269u;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		r10 = OBJECT(current_object)->unk_80;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		OBJECT(current_object)->unk_80 = r10 + 2u;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		if ((uint8_t)(SCREEN(screen)->unk_5800 >> 8u) != 0u)
		{
			OBJECT(current_object)->unk_40 = 1u;
			OBJECT(current_object)->unk_41 = 2u;
			OBJECT(current_object)->unk_42 = 2u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A198[t1]);
	}
	t3 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t3 - 1u;
	if (t3 != 1u)
	{
		OBJECT(current_object)->unk_30 -= 0x10u;
		r0 = sub_00EAE4(r0);
		return;
	}
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01A499_ADDRESS);
	r1 = 2u;
	r0 &= ~0xFFu;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A536_ADDRESS);
		mem_write8(r21 + 9u, r0);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		r1--;
	} while (r1 != 0u);
	r1 = 3u;
	r0 &= ~0xFFu;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A672_ADDRESS);
		mem_write8(r21 + 9u, r0);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		r1--;
	} while (r1 != 0u);
	OBJECT(current_object)->unk_80 = 0x1AC54u;
	OBJECT(current_object)->unk_10 = 6u;
	OBJECT(current_object)->unk_0C++;
	r0 = sub_00EAE4(r0);
}

void sub_01A29C(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A2A8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x25Eu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x26Du;
		OBJECT(current_object)->unk_30 = 2u;
		OBJECT(current_object)->unk_32 = 0x47u;
		OBJECT(current_object)->unk_3C = 0x199u;
		OBJECT(current_object)->unk_10 = 0xBu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r10 = OBJECT(current_object)->unk_80;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		OBJECT(current_object)->unk_80 = r10 + 2u;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A2A8[t1]);
	}
	t3 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t3 - 1u;
	if (t3 == 1u)
	{
		OBJECT(current_object)->unk_80 = 0x1AC62u;
		OBJECT(current_object)->unk_10 = 6u;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_30 += 0x10u;
	r0 = sub_00EAE4(r0);
}

void sub_01A329(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A334[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_10 = ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2) + 1u;
		OBJECT(current_object)->unk_0C++;
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = sub_011E45();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = rom_01AC44[r0];
		r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(current_object)->unk_30 = ROM16(r10);
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		OBJECT(current_object)->unk_34 = 0x40u;
		OBJECT(current_object)->unk_36 = 0x40u;
		OBJECT(current_object)->unk_40 = 9u;
		OBJECT(current_object)->unk_3C = 0x19Au;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = sub_011E45();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = rom_01AC44[r0];
		r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(current_object)->unk_30 = ROM16(r10);
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		OBJECT(current_object)->unk_34 = 0x40u;
		OBJECT(current_object)->unk_36 = 0x40u;
		OBJECT(current_object)->unk_40 = 9u;
		OBJECT(current_object)->unk_3C = 0x19Au;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(current_object)->unk_30 += 0x10u;
		if ((int16_t)OBJECT(current_object)->unk_30 <= 0x148)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == 0u)
			AT8(SCREEN(screen)->unk_5800, 1) = 1u;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		AT8(SCREEN(screen)->unk_5800, 1) = 0u;
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_01A334[t1]);
	}
}

void sub_01A3E9(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A3F4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_10 = ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2) + 1u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_3E = 0xAu;
			OBJECT(current_object)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_30 += 0x10u;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		if ((uint8_t)(SCREEN(screen)->unk_5800 >> 8u) != 0u)
		{
			OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_41 = 2u;
			OBJECT(current_object)->unk_42 = 2u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A3F4[t1]);
	}
	t3 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t3 - 1u;
	if (t3 != 1u)
		return;
	OBJECT(current_object)->unk_3E = 0xAu;
	r9 = 0x25Fu;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x270u;
	r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2;
	r10 = 0x1ACA8u + r0;
	OBJECT(current_object)->unk_30 = ROM16(r10);
	OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
	OBJECT(current_object)->unk_3C = 0x198u;
	OBJECT(current_object)->unk_10 = 0x14u;
	OBJECT(current_object)->unk_0C++;
	t2 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_30 += 0x10u;
	r0 = sub_00EAE4(r0);
}

void sub_01A499(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A4A4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x25Au;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x272u;
		OBJECT(current_object)->unk_34 = 0xC0u;
		OBJECT(current_object)->unk_36 = 0xC0u;
		OBJECT(current_object)->unk_30 = 0xE2u;
		OBJECT(current_object)->unk_32 = 0x4Eu;
		OBJECT(current_object)->unk_3C = 0x198u;
		OBJECT(current_object)->unk_10 = 9u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		if ((uint8_t)(SCREEN(screen)->unk_5800 >> 8u) != 0u)
		{
			OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_41 = 3u;
			OBJECT(current_object)->unk_42 = 6u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A4A4[t1]);
	}
	t2 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_34 += 8u;
	OBJECT(current_object)->unk_36 += 8u;
	r0 = sub_00EAE4(r0);
}

void sub_01A536(void)
{
	uint16_t r3 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A544[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x25Bu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x272u;
		OBJECT(current_object)->unk_34 = 0xC0u;
		OBJECT(current_object)->unk_36 = 0xC0u;
		r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 2;
		r10 = 0x1ACA0u + r0;
		OBJECT(current_object)->unk_30 = ROM16(r10);
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		OBJECT(current_object)->unk_3C = 0x198u;
		OBJECT(current_object)->unk_10 = 9u;
		OBJECT(current_object)->unk_0C++;
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_3E = 0xAu;
			r0 = (uint16_t)((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 11);
			AS16(OBJECT(current_object)->unk_88) = r0;
			AS16(OBJECT(current_object)->unk_98) = r0;
			AS16(OBJECT(current_object)->unk_8C) = OBJECT(current_object)->unk_30;
			AS16(OBJECT(current_object)->unk_90) = OBJECT(current_object)->unk_32;
			OBJECT(current_object)->unk_0C++;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_34 += 8u;
		OBJECT(current_object)->unk_36 += 8u;
		OBJECT(current_object)->unk_30--;
		OBJECT(current_object)->unk_32 -= 5u;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_3E = 0xAu;
			r0 = (uint16_t)((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 11);
			AS16(OBJECT(current_object)->unk_88) = r0;
			AS16(OBJECT(current_object)->unk_98) = r0;
			AS16(OBJECT(current_object)->unk_8C) = OBJECT(current_object)->unk_30;
			AS16(OBJECT(current_object)->unk_90) = OBJECT(current_object)->unk_32;
			OBJECT(current_object)->unk_0C++;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_34 += 8u;
		OBJECT(current_object)->unk_36 += 8u;
		OBJECT(current_object)->unk_30--;
		OBJECT(current_object)->unk_32 -= 5u;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		AS16(OBJECT(current_object)->unk_88) += 0x20u;
		if ((int16_t)(uint16_t)OBJECT(current_object)->unk_88 >= 0x1000)
			AS16(OBJECT(current_object)->unk_88) -= 0x1000u;
		AS16(OBJECT(current_object)->unk_98) += 0x40u;
		if ((int16_t)(uint16_t)OBJECT(current_object)->unk_98 >= 0x1000)
			AS16(OBJECT(current_object)->unk_98) -= 0x1000u;
		OBJECT(current_object)->unk_30 = (uint16_t)OBJECT(current_object)->unk_8C;
		OBJECT(current_object)->unk_32 = (uint16_t)OBJECT(current_object)->unk_90;
		r0 = (int16_t)(uint16_t)OBJECT(current_object)->unk_88 & 0xFFFu;
		r3 = (uint16_t)((int16_t)rom_100C00[(uint32_t)(int16_t)(uint16_t)OBJECT(current_object)->unk_98] >> 12);
		OBJECT(current_object)->unk_30 += (int16_t)rom_100C00[r0] >> 9;
		OBJECT(current_object)->unk_32 += r3;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A544[t1]);
	}
}

void sub_01A672(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A680[t1];
	switch (t1)
	{
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	case 0:
		break;
	default:
		invalid_jump(rom_01A680[t1]);
	}
	OBJECT(current_object)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4;
	r10 = 0x1AC70u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_32 = mem_read16(r10);
	OBJECT(current_object)->unk_3C = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_0C++;
	r0 = sub_00EAE4(r0);
}

void sub_01A6CF(void)
{
	uint8_t r0 = 0, r2 = 0, r10 = 0, r11 = 0;
	uint32_t r1 = 0, r21 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		if ((uint8_t)SCREEN(screen)->unk_80 == 6u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		AT8(OBJECT(current_object)->unk_08, 2) = SCREEN(screen)->unk_02;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 != 1u)
		{
			if ((uint8_t)SCREEN(screen)->unk_80 != 6u && (uint8_t)SCREEN(screen)->unk_80 != 7u && SCREEN(screen)->unk_F44 == 0u)
				SCREEN(screen)->unk_F40 = rom_01A9BC[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			return;
		}
		break;
	default:
		invalid_jump(rom_01A6DC[OBJECT(current_object)->unk_0C]);
	}
	OBJECT(current_object)->unk_10 = 0x1Eu;
	AT8(OBJECT(current_object)->unk_08, 3) = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01A7C5_ADDRESS);
	mem_write8(r21 + 0xAu, (uint8_t)(OBJECT(current_object)->unk_08 >> 16u));
	mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(current_object)->unk_08 >> 16u));
	AT8(OBJECT(current_object)->unk_08, 2)++;
	OBJECT(current_object)->unk_08 &= 0xFF07FFFFu;
	if (unk_20E615 != 0u)
	{
		r0 = 2u;
		r1 = 2u;
		r2 = 0u;
		r21 = sub_01A8CA(r1, r2);
	}
	else
	{
		switch (SCREEN(screen)->unk_00)
		{
		case 0:
			r10 = unk_20E0A0;
			r11 = unk_20E0A1;
			break;
		case 1:
			r10 = unk_20E0A8;
			r11 = unk_20E0A9;
			break;
		default:
			invalid_jump(rom_01A744[SCREEN(screen)->unk_00]);
		}
		if (r10 != 0u)
		{
			r0 = 2u;
			r1 = 2u;
			r2 = 0u;
			r21 = sub_01A8CA(r1, r2);
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(current_object)->unk_08 >> 24u));
			if ((uint8_t)SCREEN(screen)->unk_80 != 6u && (uint8_t)SCREEN(screen)->unk_80 != 7u && SCREEN(screen)->unk_F44 == 0u)
				SCREEN(screen)->unk_F40 = rom_01A9BC[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			return;
		}
		if (r11 != 0u)
			r0 = 1u;
		else
			r0 = 0u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A834_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r0);
	}
	AT8(OBJECT(current_object)->unk_08, 1) = r0;
	mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(current_object)->unk_08 >> 24u));
	if ((uint8_t)SCREEN(screen)->unk_80 != 6u && (uint8_t)SCREEN(screen)->unk_80 != 7u && SCREEN(screen)->unk_F44 == 0u)
		SCREEN(screen)->unk_F40 = rom_01A9BC[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
}

void sub_01A7C5(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A7D0[t1];
	switch (t1)
	{
	case 0:
		r9 = rom_01A986[(uint8_t)(OBJECT(current_object)->unk_08 >> 16u)];
		sub_00F366(r9);
		r0 = (uint16_t)(rom_01A996[(uint8_t)(OBJECT(current_object)->unk_08 >> 24u)] + 4u);
		OBJECT(current_object)->unk_3A = r0;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_34 = 0x40u;
		OBJECT(current_object)->unk_36 = 0x40u;
		OBJECT(current_object)->unk_30 = 0x140u;
		OBJECT(current_object)->unk_32 = 0xC0u;
		OBJECT(current_object)->unk_40 = 4u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01A7D0[t1]);
	}
	t2 = OBJECT(current_object)->unk_30 - 0xEu;
	OBJECT(current_object)->unk_30 = t2;
	if ((int16_t)t2 >= 0)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_01A834(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0, t3 = 0, t4 = 0;
	uint32_t t1 = 0;

	if (mem_read8(OBJECT(current_object)->unk_14 + 9u) != (uint8_t)(OBJECT(current_object)->unk_08 >> 8u))
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A84C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_10 = 6u;
		OBJECT(current_object)->unk_0C++;
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_3E = 0u;
		r9 = rom_01A9A6[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
		sub_00F366(r9);
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
		OBJECT(current_object)->unk_3A = rom_01A9AC[r0];
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xC0u;
		OBJECT(current_object)->unk_3C = 0x19Du;
		OBJECT(current_object)->unk_10 = 0x16u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_3E = 0u;
		r9 = rom_01A9A6[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
		sub_00F366(r9);
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
		OBJECT(current_object)->unk_3A = rom_01A9AC[r0];
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xC0u;
		OBJECT(current_object)->unk_3C = 0x19Du;
		OBJECT(current_object)->unk_10 = 0x16u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	case 3:
		t3 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t3 - 1u;
		if (t3 != 1u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_01A84C[t1]);
	}
	sub_00F30D(0u);
	t4 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t4 - 1u;
	if (t4 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_3C--;
	OBJECT(current_object)->unk_10 = 0x1Eu;
	OBJECT(current_object)->unk_0C++;
	t3 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t3 - 1u;
	if (t3 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_00 = 0u;
}

uint32_t sub_01A8CA(uint32_t r1, uint8_t r2)
{
	uint32_t r21 = 0;

	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A8E2_ADDRESS);
		mem_write8(r21 + 9u, r2);
		r2++;
		r1--;
	} while (r1 != 0u);
	return r21;
}

void sub_01A8E2(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01A8F0[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A8F0[t1]);
	}
	OBJECT(current_object)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4;
	r10 = 0x1A95Bu + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_32 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_3C = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_10) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_0C++;
	sub_00F30D(0u);
	r0 = sub_00EAE4(r0);
}

void sub_01AA01(void)
{
	uint8_t r10_2 = 0, r11_2 = 0;
	uint32_t r10 = 0, r11 = 0, r19 = 0, ignored = 0;
	uint32_t stack_10 = 0, stack_14 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		if ((uint8_t)SCREEN(screen)->unk_80 == 6u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
		}
		if (unk_20E615 != 0u)
		{
			OBJECT(current_object)->unk_0C++;
			r19 = 0x1AB62u;
			ignored = sub_011A32(r19);
			return;
		}
		OBJECT(current_object)->unk_0C += 2u;
		break;
	case 1:
		r19 = 0x1AB62u;
		ignored = sub_011A32(r19);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_01AA0C[OBJECT(current_object)->unk_0C]);
	}
	switch (SCREEN(screen)->unk_00)
	{
	case 0:
		r10 = unk_20E0A0;
		r11 = unk_20E0A1;
		break;
	case 1:
		r10 = unk_20E0A8;
		r11 = unk_20E0A9;
		break;
	default:
		invalid_jump(rom_01AA4C[SCREEN(screen)->unk_00]);
	}
	if (r10 < 2u)
		r19 = 0x1AB72u;
	else
		r19 = 0x1AB80u;
	stack_10 = r10;
	stack_14 = r11;
	sub_011A32(r19);
	r11_2 = (uint8_t)stack_14;
	r10_2 = (uint8_t)stack_10;
	r12 = r12 & ~0xFFu;
	OBJECT(current_object)->unk_32 = 0xCFu;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	while ((int8_t)r10_2 >= 0xA)
	{
		r10_2 -= 0xAu;
		r12 = (r12 & ~0xFFu) | (uint8_t)(r12 + 1u);
	}
	if ((uint8_t)r12 != 0u)
	{
		AT8(OBJECT(current_object)->unk_08, 1) = 0u;
		AT8(OBJECT(current_object)->unk_08, 2) = rom_01AB8E[(uint8_t)r12];
		OBJECT(current_object)->unk_30 = 0xACu;
		sub_01AB20();
	}
	AT8(OBJECT(current_object)->unk_08, 1) = 0u;
	AT8(OBJECT(current_object)->unk_08, 2) = rom_01AB8E[(uint8_t)r10_2];
	OBJECT(current_object)->unk_30 = 0xB4u;
	sub_01AB20();
	if ((uint8_t)r11_2 != 0u)
	{
		AT8(OBJECT(current_object)->unk_08, 1) = 1u;
		AT8(OBJECT(current_object)->unk_08, 2) = r11_2;
		OBJECT(current_object)->unk_30 = 0xC0u;
		sub_01AB20();
		AT8(OBJECT(current_object)->unk_08, 1) = 2u;
		AT8(OBJECT(current_object)->unk_08, 2) = unk_20E610;
		OBJECT(current_object)->unk_30 = 0xC8u;
		sub_01AB20();
	}
}

void sub_01AB20(void)
{
	uint32_t r0 = 0, r9 = 0;

	OBJECT(current_object)->unk_3E = 0u;
	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	r9 = rom_01AB98[r0];
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = rom_01AB9E[r0];
	OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
	sub_00EAE4(r0);
}

void sub_01ADE8(void)
{
	uint32_t zero = 0;
	uint32_t r10 = 0;
	uint32_t t1 = 0, target1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_80 = timeline_1AFC0_ADDRESS;
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_01ADF4[OBJECT(current_object)->unk_0C]);
	}
	AS16(OBJECT(current_object)->unk_10)++;
	r10 = OBJECT(current_object)->unk_80;
	t1 = mem_read32(r10) - AS16(OBJECT(current_object)->unk_10);
	zero = t1 == 0u;
	r10 += 4u;
	if ((int32_t)t1 < 0)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	if (zero)
	{
		target1 = mem_read32(r10);
		switch (target1)
		{
		case 0x01AE2Du:
			sub_01AE2D();
			break;
		case 0x01AE61u:
			sub_01AE61();
			break;
		case 0x01AE6Bu:
			sub_01AE6B();
			break;
		default:
			invalid_jump(target1);
		}
		OBJECT(current_object)->unk_80 += 8u;
	}
}

void sub_01AE2D(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0, r21 = 0;

	r1 = 2u;
	r0 = 0u;
	sub_01AE75(r0, r1);
	r0 = 3u;
	if (unk_20E620 == 0u)
		r0 += unk_20E040;
	else
		r0++;
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01AE8D_ADDRESS);
	mem_write8(r21 + 9u, r0);
}

void sub_01AE61(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0;

	r1 = 2u;
	r0 = 2u;
	sub_01AE75(r0, r1);
}

void sub_01AE6B(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0;

	r1 = 4u;
	r0 = 8u;
	sub_01AE75(r0, r1);
}

void sub_01AE75(uint8_t r0, uint32_t r1)
{
	uint32_t r21 = 0;

	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01AE8D_ADDRESS);
		mem_write8(r21 + 9u, r0);
		r0++;
		r1--;
	} while (r1 != 0u);
}

void sub_01AE8D(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01AE98[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01AE98[t1]);
	}
	OBJECT(current_object)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4;
	r10 = 0x1AF00u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_32 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_3C = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_10) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_0C++;
	sub_00F30D(0u);
	r0 = sub_00EAE4(r0);
}

void sub_01AFDC(void)
{
	uint16_t r4 = 0;
	uint32_t r3 = 0, r4_2 = 0, r5 = 0, r10 = 0, r11 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		AS8(SCREEN(screen)->unk_B00) = 0u;
		sub_0106AF();
		unk_20E200 &= 0xF7FFu;
		unk_20E202 = 0xFu;
		unk_20E28C = 0xFFu;
		unk_20E288 = 0u;
		unk_20E28A = 0u;
		unk_20E027 = 0xFFu;
		AS16(SCREEN(screen)->unk_5800) = 0u;
		AT16(SCREEN(screen)->unk_5800, 2) = 0u;
		SCREEN(screen)->unk_5804 = 0u;
		SCREEN(screen)->unk_5806 = 0u;
		OBJECT(current_object)->unk_30 = 0xEu;
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		if ((SCREEN(screen)->unk_118 & 8u) != 0u)
			OBJECT(current_object)->unk_30 -= 2u;
		if ((SCREEN(screen)->unk_118 & 4u) != 0u)
			OBJECT(current_object)->unk_30 += 2u;
		if ((int16_t)OBJECT(current_object)->unk_30 < 0xA)
			OBJECT(current_object)->unk_30 = 0xEu;
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xE)
			OBJECT(current_object)->unk_30 = 0xAu;
		if (OBJECT(current_object)->unk_30 == 0xAu)
		{
			if ((SCREEN(screen)->unk_118 & 2u) != 0u)
				SCREEN(screen)->unk_5806++;
			if ((SCREEN(screen)->unk_118 & 1u) != 0u)
				SCREEN(screen)->unk_5806--;
			if ((int16_t)SCREEN(screen)->unk_5806 < 0)
				SCREEN(screen)->unk_5806 = 9u;
			if ((int16_t)SCREEN(screen)->unk_5806 > 9)
				SCREEN(screen)->unk_5806 = 0u;
		}
		else if (OBJECT(current_object)->unk_30 == 0xCu)
		{
			if ((SCREEN(screen)->unk_118 & 2u) != 0u)
				SCREEN(screen)->unk_5804++;
			if ((SCREEN(screen)->unk_118 & 1u) != 0u)
				SCREEN(screen)->unk_5804--;
			if ((int16_t)SCREEN(screen)->unk_5804 < 0)
				SCREEN(screen)->unk_5804 = 9u;
			if ((int16_t)SCREEN(screen)->unk_5804 > 9)
				SCREEN(screen)->unk_5804 = 0u;
		}
		else
		{
			if ((SCREEN(screen)->unk_118 & 2u) != 0u)
				AT16(SCREEN(screen)->unk_5800, 2)++;
			if ((SCREEN(screen)->unk_118 & 1u) != 0u)
				AT16(SCREEN(screen)->unk_5800, 2)--;
			if ((int16_t)(uint16_t)(SCREEN(screen)->unk_5800 >> 16u) < 0)
				AT16(SCREEN(screen)->unk_5800, 2) = 9u;
			if ((int16_t)(uint16_t)(SCREEN(screen)->unk_5800 >> 16u) > 9)
				AT16(SCREEN(screen)->unk_5800, 2) = 0u;
		}
		if (SCREEN(screen)->unk_00 == 0u)
		{
			r4 = (uint16_t)((uint16_t)(SCREEN(screen)->unk_5800 >> 16u) + SCREEN(screen)->unk_5804 * 0xAu + SCREEN(screen)->unk_5806 * 0x64u);
			AS16(SCREEN(screen)->unk_5800) = r4;
			r3 = TEXT_CELL(5, 10);
			sub_01B20C(r3, r4);
			OBJECT(current_object)->unk_10++;
			if ((OBJECT(current_object)->unk_10 & 3u) == 0u)
				mem_write16(TEXT_CELL(0, 10) + OBJECT(current_object)->unk_30, 0x602Au);
		}
		if (SCREEN(screen)->unk_00 != 0u)
			return;
		r4_2 = 0u;
		r5 = 0u;
		r10 = rom_01B254[(uint16_t)SCREEN(screen)->unk_5800];
		do
		{
			r11 = VIDEO_RAM + 0x1040u + r5 * 0x1000u;
			sub_01B1B1(r11, &r4_2, &r10);
			r5++;
		} while (r4_2 == 0u);
		return;
	case 2:
		if (SCREEN(screen)->unk_00 != 0u)
			return;
		r4_2 = 0u;
		r5 = 0u;
		r10 = rom_01B254[(uint16_t)SCREEN(screen)->unk_5800];
		do
		{
			r11 = VIDEO_RAM + 0x1040u + r5 * 0x1000u;
			sub_01B1B1(r11, &r4_2, &r10);
			r5++;
		} while (r4_2 == 0u);
		return;
	case 3:
		return;
	default:
		invalid_jump(rom_01AFE8[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01B1B1(uint32_t r11, uint32_t *p_r4, uint32_t *p_r10)
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
		r13 = sub_01B200(r13);
		sub_01B1CD(r11, r13);
		r11 += 8u;
	}
	*p_r4 = 0xFFFFFFFFu;
}

void sub_01B1CD(uint32_t r11, uint32_t r13)
{
	r12 = r11;
	r13 = sub_01B1E7(r13);
	r12 = r11 + 4u;
	r13 = sub_01B1E7(r13);
	r12 = r11 + 0x800u;
	r13 = sub_01B1E7(r13);
	r12 = r11 + 0x804u;
	r13 = sub_01B1E7(r13);
}

uint32_t sub_01B1E7(uint32_t r13)
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

uint32_t sub_01B200(uint32_t r13)
{
	r13 = 0x1CF000u + r13 * 2u * 4u;
	return r13;
}

void sub_01B20C(uint32_t r3, uint16_t r4)
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

void sub_01C241(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;

	mem_write16(0x801000u, 0u);
	mem_write8(IO_C00074, 0x1Fu);
	sub_0106AF();
	sub_0107F0();
	unk_208B00 = 0u;
	unk_218B00 = 0u;
	unk_208090 &= ~0x100u;
	unk_218090 &= ~0x100u;
	unk_208094 &= ~1u;
	unk_218094 &= ~1u;
	mem_write32(MIXER_610040, 0u);
	mem_write32(MIXER_610044, 0u);
	mem_write32(MIXER_610048, 0u);
	mem_write32(0x690040u, 0u);
	mem_write32(0x690044u, 0u);
	mem_write32(0x690048u, 0u);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	sub_01EF6E();
	AS8(unk_20E701) = 0u;
	AT8(unk_20E701, 1) = 0xCu;
	unk_20E732 = 0u;
	unk_20E733 = 0u;
	r0 = 0x1Fu;
	sub_01F26B(r0);
	sub_01F266(r0);
	mem_write8(SOUND_700710, 0u);
	mem_write8(SOUND_700712, mem_read8(SOUND_700712) | 0x20u);
	mem_write8(SOUND_700810, 0u);
	mem_write8(SOUND_700812, mem_read8(SOUND_700812) | 0x20u);
	unk_20E0D0[0] = 0u;
	unk_20E0D1 = 0u;
	unk_20E0C0[0] = 0u;
	unk_20E0C1 = 0u;
	unk_20E0C2 = 0u;
	unk_20E0C3 = 0u;
	unk_20E0C4 = 0u;
	unk_20E0C5 = 0u;
	unk_20E0C6 = 0u;
	unk_20E0C7 = 0u;
	unk_20E0C8[0] = 0u;
	unk_20E0C9 = 0u;
	unk_20E0CA = 0u;
	unk_20E0CB = 0u;
	unk_20E0CC = 0u;
	unk_20E0CD = 0u;
	unk_20E0CE = 0u;
	unk_20E0CF = 0u;
	unk_208F20 = 0u;
	unk_218F20 = 0u;
	unk_20E092[0] &= 0xE0E1u;
	unk_20E094 &= ~0x403u;
	r0_2 = rom_01C378;
	r1 = rom_01C37C;
	unk_208F10 = r0_2;
	unk_208F14 = r1;
	unk_218F10 = r0_2;
	unk_218F14 = r1;
	r0_2 = rom_01C380;
	r1 = rom_01C384;
	unk_208F18 = r0_2;
	unk_208F1C = r1;
	unk_218F18 = r0_2;
	unk_218F1C = r1;
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	r12 = unk_218F20_ADDRESS;
	sub_01F190();
}

void sub_01C388(void)
{
	sub_01F190();
	sub_01F237();
	call_code(AT32(unk_20E701, 3));
}

void sub_01C392(void)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r10 = 0;

	if ((uint8_t)(unk_20E701 >> 8u) == 0u)
	{
		AT32(unk_20E701, 3) = sub_01C3E6_ADDRESS;
		sub_01C3E6();
		return;
	}
	if ((unk_20E701 & 1u) != 0u)
		AT8(unk_20E701, 1)++;
	r0 = sub_01F17D();
	r0 = sub_01C435(r0);
	r10 = 0x1C490u;
	sub_01F0CA(r0, r10);
	carry = (unk_20E701 & 1u) != 0u;
	r0 = carry;
	r10 = rom_01C560[r0];
	r0 = sub_01F0CA(r0, r10);
	sub_01F02C(r0);
	r10 = 0x310000u | rom_01C588[(uint8_t)(unk_20E701 >> 8u)];
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01C3E6_ADDRESS;
	sub_01C3E6();
}

void sub_01C3E6(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 8u);
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0xD)
			r0 = 0u;
		AT8(unk_20E701, 1) = r0;
		r10 = 0x310000u | rom_01C588[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = rom_01C45C[(uint8_t)(unk_20E701 >> 8u)];
	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 < 0)
	{
		unk_20E72F = 5u;
		t2 = unk_20E708;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			unk_20E72F = (uint32_t)unk_20E72F << 1;
	}
}

uint32_t sub_01C435(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r10 = 0;

	if ((unk_20E70C & 1u) == 0u)
		return r0;
	r10 = 0x1C5A2u;
	sub_01F0CA(r0, r10);
	r10 = TEXT_CELL(27, 26);
	r0_2 = unk_20E60A;
	r1 = 0x64u;
	r2 = 5u;
	sub_01F103(r2, &r0_2, &r1, &r10);
	return r0_2;
}

void sub_01C5BA(void)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = unk_20E701;
	carry = (t1 & 1u) != 0u;
	unk_20E701 = t1 ^ 1u;
	r0 = 0u - (0xFFFFFFFFu + carry);
	r10 = rom_01C560[r0];
	sub_01F0CA(r0, r10);
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01C5DB(void)
{
	uint8_t r1_2 = 0, r26 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t3 = 0, t4 = 0, t6 = 0, t7 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0, t8 = 0, t9 = 0, t10 = 0;
	uint32_t t11 = 0, t12 = 0;

	interrupts_off();
	r0 = sub_01F17D();
	mem_write8(SOUND_701FFF, 1u);
	current_object = 0x20E716u;
	r26 = 0u;
	set_bytes(WORK_RAM + 0xE716u, 0x19u, r26);
	OBJECT(current_object)->unk_00 = 1u;
	OBJECT(current_object)->unk_01 = 1u;
	sub_01C93B(r0);
	r10 = 0u;
	r0 = rom_03FFF8_ADDRESS;
	sub_01C98E(r10, &r0, &r1);
	t1 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFF8 != t1)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	OBJECT(current_object)->unk_00 = (uint8_t)r0_2;
	r0 = (r0_2 & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (rom_03FFFA != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	OBJECT(current_object - 0x1u)->unk_01 = r0;
	current_object = current_object + 1u;
	OBJECT(current_object - 0x2u)->unk_02 = 0x101u;
	sub_01C93B(r0);
	r10 = 0x100000u;
	r0 = 0xFF000u;
	sub_01C98E(r10, &r0, &r1);
	t2 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFFC != t2)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	AS8(OBJECT(current_object - 0x2u)->unk_02) = (uint8_t)r0_2;
	r0 = (r0_2 & ~0xFFu) | 2u;
	current_object = current_object + 1u;
	if (rom_03FFFE != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	AT8(OBJECT(current_object - 0x3u)->unk_02, 1) = r0;
	current_object = current_object + 1u;
	OBJECT(current_object - 0x4u)->unk_04 = 0x1010101u;
	sub_01C93B(r0);
	r10 = 0x300000u;
	r0 = 0x20000u;
	r0 = sub_01CB8F(r0, r10);
	t3 = r0 & 0xFu;
	r1_2 = 2u;
	if (t3 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(current_object - 0x4u)->unk_04) = r1_2;
	t4 = r0 & 0xF0u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t4 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x5u)->unk_04, 1) = r1_2;
	t5 = r0 & 0xF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t5 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x6u)->unk_04, 2) = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xF000u) != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x7u)->unk_04, 3) = r1_2;
	current_object = current_object + 1u;
	r0 = sub_0106AF();
	OBJECT(current_object - 0x8u)->unk_08 = 0x1010101u;
	sub_01C93B(r0);
	r10 = 0x400000u;
	r0 = 0x20000u;
	r0 = sub_01CB8F(r0, r10);
	t6 = r0 & 0xFu;
	r1_2 = 2u;
	if (t6 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(current_object - 0x8u)->unk_08) = r1_2;
	t7 = r0 & 0xF0u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t7 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x9u)->unk_08, 1) = r1_2;
	t8 = r0 & 0xF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t8 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0xAu)->unk_08, 2) = r1_2;
	t9 = r0 & 0xF000u;
	r0 = t9;
	r1_2 = 2u;
	current_object = current_object + 1u;
	AT8(OBJECT(current_object - 0xBu)->unk_08, 3) = r1_2;
	current_object = current_object + 1u;
	sub_0107F0();
	OBJECT(current_object - 0xCu)->unk_0C = 1u;
	OBJECT(current_object - 0xCu)->unk_0D = 1u;
	sub_01C93B(r0);
	r10 = 0x600000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xCu)->unk_0C = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0xDu)->unk_0D = r1_2;
	current_object = current_object + 1u;
	r0 = sub_01EF6E();
	OBJECT(current_object - 0xEu)->unk_0E = 1u;
	OBJECT(current_object - 0xEu)->unk_0F = 1u;
	sub_01C93B(r0);
	r10 = PALETTE_RAM + 0x4000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
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
	r0 = sub_01EF6E();
	OBJECT(current_object - 0x10u)->unk_10 = 1u;
	OBJECT(current_object - 0x10u)->unk_11 = 1u;
	sub_01C93B(r0);
	r10 = 0x680000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x10u)->unk_10 = r1_2;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(current_object - 0x11u)->unk_11 = r1_2;
	current_object = current_object + 1u;
	r0 = sub_01EF6E();
	OBJECT(current_object - 0x12u)->unk_12 = 1u;
	OBJECT(current_object - 0x12u)->unk_13 = 1u;
	sub_01C93B(r0);
	r10 = 0x684000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
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
	r0 = sub_01EF6E();
	OBJECT(current_object - 0x14u)->unk_14 = 0x1010101u;
	sub_01C93B(r0);
	move_halfwords(SPRITE_RAM, WORK_RAM, 0x10000u);
	r10 = 0x200000u;
	r0 = 0x20000u;
	r0 = sub_01C9BE(r0, r10);
	move_halfwords(WORK_RAM, SPRITE_RAM, 0x10000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(current_object - 0x14u)->unk_14) = r1_2;
	t10 = r0 & 0xFF00u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t10 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x15u)->unk_14, 1) = r1_2;
	t11 = r0 & 0xFF0000u;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t11 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x16u)->unk_14, 2) = r1_2;
	t12 = r0 & ~0xFFFFFFu;
	r0 = t12;
	r1_2 = 2u;
	current_object = current_object + 1u;
	if (t12 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(current_object - 0x17u)->unk_14, 3) = r1_2;
	current_object = current_object + 1u;
	sub_0107F0();
	AS8(OBJECT(current_object - 0x18u)->unk_18) = 1u;
	sub_01C93B(r0);
	move_halfwords(SPRITE_RAM, SOUND_RAM, 0x1000u);
	r10 = 0x700000u;
	r0 = 0x1FFFu;
	r0 = sub_01CAF8(r0, r10);
	move_halfwords(SOUND_RAM, SPRITE_RAM, 0x1000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(current_object - 0x18u)->unk_18) = r1_2;
	current_object = current_object + 1u;
	sub_0107F0();
	r0 = sub_01C93B(r0);
	mem_write8(SOUND_701FFF, 0u);
	interrupts_on();
	sub_01F034();
	AT32(unk_20E701, 3) = sub_01C928_ADDRESS;
	sub_01C928((uint16_t)r0);
}

void sub_01C928(uint16_t r0)
{
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	sub_01F052(r0);
}

uint32_t sub_01C93B(uint32_t r0)
{
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint8_t t1 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint16_t t2 = 0;

	r10 = 0x1CC9Cu;
	sub_01F0CA(r0, r10);
	r10 = 0x20E716u;
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

void sub_01C98E(uint32_t r10, uint32_t *p_r0, uint32_t *p_r1)
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

uint32_t sub_01C9BE(uint32_t r0, uint32_t r10)
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
		sub_01CC80();
		carry = v60_rot_cy(r2, 1u, 32);
		r2 = (r2 << 1) | (r2 >> 31);
	} while (carry == 0u);
	return r0_2;
}

uint32_t sub_01CA5D(uint32_t r0, uint32_t r10)
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
		sub_01CC80();
		carry = v60_rot_cy(r2, 1u, 16);
		r2 = (uint16_t)(((uint32_t)r2 << 1) | (r2 >> 15));
	} while (carry == 0u);
	return r0;
}

uint32_t sub_01CAF8(uint32_t r0, uint32_t r10)
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
		sub_01CC80();
		carry = v60_rot_cy(r2, 1u, 8);
		r2 = (uint8_t)(((uint32_t)r2 << 1) | (r2 >> 7));
	} while (carry == 0u);
	return r0;
}

uint32_t sub_01CB8F(uint32_t r0, uint32_t r10)
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
		sub_01CC80();
		carry = v60_rot_cy(r2, 1u, 16);
		r2 = (uint16_t)(((uint32_t)r2 << 1) | (r2 >> 15));
	} while (carry == 0u);
	return r0;
}

void sub_01CC80(void)
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

void sub_01CDE0(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1CF71u;
	r0 = sub_01F0CA(r0, r10);
	r10 = 0x1CFA0u;
	r0 = sub_01F0CA(r0, r10);
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 1u;
	r10 = 0x310000u | rom_01CF9E;
	r0 = sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01CE15_ADDRESS;
	sub_01CE15(r0);
}

void sub_01CE15(uint32_t r0)
{
	uint8_t r0_3 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	sub_01CEDB(r0);
	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0_2 = (uint8_t)(unk_20E701 >> 16u) ^ 1u;
		AT8(unk_20E701, 2) = r0_2;
		r10 = 0x310000u | rom_01CF9C[r0_2];
		sub_01F085(r0_2, r10);
	}
	if ((unk_20E70E & 2u) == 0u)
	{
		t1 = unk_20E72F - 1u;
		unk_20E72F = t1;
		if ((int8_t)t1 < 0)
		{
			unk_20E72F = 5u;
			t2 = unk_20E708;
			t3 = mem_read16(t2) ^ 0x6A7Fu;
			mem_write16(t2, t3);
			if (t3 != 0u)
				unk_20E72F = (uint32_t)unk_20E72F << 1;
		}
		return;
	}
	if ((uint8_t)(unk_20E701 >> 16u) == 0u)
	{
		r0_2 = sub_01F17D();
		r10 = 0x1D021u;
		r0_2 = sub_01F0CA(r0_2, r10);
		r10 = 0x1CFA0u;
		sub_01F0CA(r0_2, r10);
		sub_01F034();
		r10 = 0x20E638u;
		r11 = 0x20E710u;
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
		AT32(unk_20E701, 3) = sub_01CE97_ADDRESS;
		return;
	}
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 < 0)
	{
		unk_20E72F = 5u;
		t2 = unk_20E708;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			unk_20E72F = (uint32_t)unk_20E72F << 1;
	}
}

void sub_01CE97(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0;

	r10 = 0x20E710u;
	r11 = 0x20E638u;
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
	r0 = sub_01CEDB(r0);
	if ((unk_20E70E & 2u) != 0u)
	{
		unk_20E701 |= 2u;
		AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	}
	sub_01F052(r0);
}

uint32_t sub_01CEDB(uint32_t r0)
{
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	r10 = 0x1D03Cu;
	sub_01F0CA(r0, r10);
	r10 = TEXT_CELL(0, 0);
	r11 = 0x1D030u;
	r12 = 0x20E638u;
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
	r12 = 0x20E710u;
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

void sub_01D111(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1D26Cu;
	r0 = sub_01F0CA(r0, r10);
	sub_01F034();
	AT32(unk_20E701, 3) = sub_01D126_ADDRESS;
	sub_01D126(r0);
}

void sub_01D126(uint32_t r0)
{
	r0 = sub_01D151(r0);
	if ((unk_20E70E & 2u) == 0u)
	{
		sub_01F052(r0);
		return;
	}
	AT32(unk_20E701, 3) = sub_01D13B_ADDRESS;
	sub_01F052(r0);
}

void sub_01D13B(void)
{
	uint32_t r0 = 0;

	r0 = sub_01D151(0u);
	if ((unk_20E70E & 0x200u) == 0u)
	{
		sub_01F052(r0);
		return;
	}
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	sub_01F052(r0);
}

uint32_t sub_01D151(uint32_t r0)
{
	uint32_t carry = 0;
	uint8_t r1_2 = 0;
	uint16_t r1 = 0, r2 = 0;
	uint32_t r0_2 = 0, r2_2 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;

	r10 = 0x1D30Au;
	sub_01F0CA(r0, r10);
	r10 = TEXT_CELL(0, 0);
	r11 = 0x1D380u;
	r12 = 0x20E710u;
	r13 = 0x20E638u;
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
		r10 = sub_01F131(r0_2, r1_2, r2_2, r10);
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

void sub_01D3E4(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1D4F8u;
	r0 = sub_01F0CA(r0, r10);
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 0xAu;
	r10 = 0x310000u | rom_01D5BE;
	sub_01F085(r0, r10);
	sub_01D49A();
	AT32(unk_20E701, 3) = sub_01D419_ADDRESS;
	sub_01D419();
}

void sub_01D419(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 2u) != 0u)
	{
		switch ((uint8_t)(unk_20E701 >> 16u))
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
			sub_01D4C7();
			break;
		case 10:
			sub_01D4DF();
			break;
		default:
			invalid_jump(rom_01D5EC[(uint8_t)(unk_20E701 >> 16u)]);
		}
		sub_01D49A();
	}
	if ((unk_20E70E & 0x200u) != 0u)
	{
		r0 = (uint8_t)(unk_20E701 >> 16u);
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
				sub_01D4C7();
				break;
			case 10:
				sub_01D4DF();
				break;
			default:
				invalid_jump(rom_01D5EC[r0]);
			}
			sub_01D49A();
		}
	}
	if ((unk_20E70E & 1u) == 0u || (unk_20E70C & 2u) != 0u)
	{
		t1 = unk_20E72F - 1u;
		unk_20E72F = t1;
		if ((int8_t)t1 < 0)
		{
			unk_20E72F = 5u;
			t2 = unk_20E708;
			t3 = mem_read16(t2) ^ 0x6A7Fu;
			mem_write16(t2, t3);
			if (t3 != 0u)
				unk_20E72F = (uint32_t)unk_20E72F << 1;
		}
		return;
	}
	sub_01F09A();
	r0 = (uint8_t)(unk_20E701 >> 16u);
	r0 = (uint8_t)(r0 + 1u);
	if ((int8_t)r0 >= 0xB)
		r0 = 0u;
	AT8(unk_20E701, 2) = r0;
	r10 = 0x310000u | rom_01D5AA[r0];
	sub_01F085(r0, r10);
	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 < 0)
	{
		unk_20E72F = 5u;
		t2 = unk_20E708;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			unk_20E72F = (uint32_t)unk_20E72F << 1;
	}
}

void sub_01D49A(void)
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
		r12 = r12 + unk_20E000_ADDRESS;
		carry = (mem_read32(r12) & shl32(1u, (int)(ROM8(r11)))) != 0u;
		mem_write32(r10, rom_01D618[carry]);
		r11++;
	} while (ROM16(r11) != 0u);
}

void sub_01D4C7(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = 0x1D5C2u + (uint8_t)(unk_20E701 >> 16u) * 4u;
	r11 = ROM8(r10) + unk_20E000_ADDRESS;
	mem_write32(r11, mem_read32(r11) ^ shl32(1u, (int)(ROM8(r10 + 1u))));
}

void sub_01D4DF(void)
{
	unk_20E092[0] &= 0xE0E1u;
	unk_20E094 &= ~0x403u;
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01D620(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1D788u;
	r0 = sub_01F0CA(r0, r10);
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 2u;
	r10 = 0x310000u | rom_01D7C4;
	sub_01F085(r0, r10);
	unk_20E730 = 0u;
	unk_20E731 = 0u;
	sub_01D6BD();
	AT32(unk_20E701, 3) = sub_01D661_ADDRESS;
	sub_01D661();
}

void sub_01D661(void)
{
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 16u);
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 3)
			r0 = 0u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01D7C0[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
	{
		r0_2 = 0x1Fu;
		sub_01F26B(r0_2);
		sub_01F266(r0_2);
		switch ((uint8_t)(unk_20E701 >> 16u))
		{
		case 0:
			sub_01D6E7();
			break;
		case 1:
			sub_01D72C();
			break;
		case 2:
			sub_01D771();
			break;
		default:
			invalid_jump(rom_01D77C[(uint8_t)(unk_20E701 >> 16u)]);
		}
		sub_01D6BD();
	}
	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 < 0)
	{
		unk_20E72F = 5u;
		t2 = unk_20E708;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			unk_20E72F = (uint32_t)unk_20E72F << 1;
	}
}

void sub_01D6BD(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;

	r10 = TEXT_CELL(28, 11);
	r0 = unk_20E730;
	r1 = 0x64u;
	r2 = 3u;
	sub_01F103(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF6B8u;
	r0 = unk_20E731;
	r1 = 0x64u;
	r2 = 3u;
	sub_01F103(r2, &r0, &r1, &r10);
}

void sub_01D6E7(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;
	uint32_t stack_4 = 0;

	r1 = unk_20E730;
	if (r1 < 0x40u)
	{
		r0_2 = mem_read8(0x1D7C6u + r1);
		if (r0_2 == 0u)
		{
			unk_20E730 = 0u;
			return;
		}
		unk_20E730 = r1 + 1u;
		sub_01F26B((uint16_t)r0_2);
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
	sub_01F26B(r0);
	r1 = stack_4;
	r0_2 = mem_read8(0x1D7C6u + r1);
	if (r0_2 == 0u)
	{
		unk_20E730 = 0u;
		return;
	}
	unk_20E730 = r1 + 1u;
	sub_01F26B((uint16_t)r0_2);
}

void sub_01D72C(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;
	uint32_t stack_4 = 0;

	r1 = unk_20E731;
	if (r1 < 0x40u)
	{
		r0_2 = mem_read8(0x1D7C6u + r1);
		if (r0_2 == 0u)
		{
			unk_20E731 = 0u;
			return;
		}
		unk_20E731 = r1 + 1u;
		sub_01F266((uint16_t)r0_2);
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
	sub_01F266(r0);
	r1 = stack_4;
	r0_2 = mem_read8(0x1D7C6u + r1);
	if (r0_2 == 0u)
	{
		unk_20E731 = 0u;
		return;
	}
	unk_20E731 = r1 + 1u;
	sub_01F266((uint16_t)r0_2);
}

void sub_01D771(void)
{
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01D840(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	mem_write16(PALETTE_609C00, 0u);
	mem_write16(0x689C00u, 0u);
	r10 = 0x1D9C7u;
	sub_01F0CA(r0, r10);
	r0 = sub_01D8B9(r0);
	sub_01F043();
	AT32(unk_20E701, 3) = sub_01D868_ADDRESS;
	sub_01D868((uint16_t)r0);
}

void sub_01D868(uint16_t r0)
{
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = sub_01D87A_ADDRESS;
	sub_01F052(r0);
}

void sub_01D87A(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01D957(0u);
	r10 = 0x1D9DCu;
	r0 = sub_01F0CA(r0, r10);
	sub_01F034();
	AT32(unk_20E701, 3) = sub_01D88F_ADDRESS;
	sub_01D88F(r0);
}

void sub_01D88F(uint32_t r0)
{
	if ((unk_20E70E & 2u) != 0u)
	{
		r0 = sub_01F17D();
		mem_write16(PALETTE_609C00, 0x733u);
		mem_write16(0x689C00u, 0x733u);
		AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	}
	sub_01F052(r0);
}

uint32_t sub_01D8B9(uint32_t r0)
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
	r10 = sub_01D927((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = sub_01D927((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = sub_01D927((uint16_t)r0_2, r10);
	r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 + 1u);
	r10 = sub_01D927((uint16_t)r0_2, r10);
	return r0_2;
}

uint32_t sub_01D927(uint16_t r0, uint32_t r10)
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

uint32_t sub_01D957(uint32_t r0)
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

void sub_01D9F3(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1DBF8u;
	sub_01F0CA(r0, r10);
	r0 = sub_01DA7C();
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 9u;
	r10 = 0x310000u | rom_01DCE0;
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01DA28_ADDRESS;
	sub_01DA28();
}

void sub_01DA28(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 16u);
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0xA)
			r0 = 0u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01DCCE[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
	{
		current_object = unk_20E600_ADDRESS;
		switch ((uint8_t)(unk_20E701 >> 16u))
		{
		case 0:
			sub_01DB58();
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			sub_01DB68();
			break;
		case 7:
			sub_01DB85();
			break;
		case 8:
			sub_01DB97();
			break;
		case 9:
			sub_01DBA9();
			break;
		default:
			invalid_jump(rom_01DBD0[(uint8_t)(unk_20E701 >> 16u)]);
		}
	}
	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 < 0)
	{
		unk_20E72F = 5u;
		t2 = unk_20E708;
		t3 = mem_read16(t2) ^ 0x6A7Fu;
		mem_write16(t2, t3);
		if (t3 != 0u)
			unk_20E72F = (uint32_t)unk_20E72F << 1;
	}
}

uint32_t sub_01DA7C(void)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	current_object = unk_20E600_ADDRESS;
	r10 = TEXT_CELL(32, 5);
	r1 = 0x64u;
	r2 = 2u;
	r0 = unk_20E630 + 1u;
	sub_01F103(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF3A8u;
	t1 = (uint8_t)OBJECT(current_object)->unk_30 * 2u;
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
	sub_01F16C(r0, r10);
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
			sub_01F103(r2, &r0, &r1, &r10);
			r10 += 4u;
			mem_write16(r10, 0x6653u);
			r10 += 2u;
			mem_write16(r10, 0x6645u);
			mem_write16(r10 + 2u, 0x6643u);
		}
		r9--;
		r11 += 0x80u;
	} while (r9 != 0u);
	r0 = (uint8_t)OBJECT(current_object)->unk_24;
	r0 = r0 & 1u;
	r10 = rom_01DCE4[r0];
	sub_01F0CA(r0, r10);
	r0 = OBJECT(current_object)->unk_0D;
	r0 = r0 & 1u;
	r10 = rom_01DCEC[r0];
	return sub_01F0CA(r0, r10);
}

void sub_01DB58(void)
{
	uint32_t ignored = 0;

	AS8(OBJECT(current_object)->unk_30) = ((uint8_t)OBJECT(current_object)->unk_30 + 1u) & 7u;
	ignored = sub_01DA7C();
}

void sub_01DB68(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0, ignored = 0;

	r1 = (uint8_t)(unk_20E701 >> 16u);
	r0 = (uint8_t)(mem_read8(current_object + 0x31u + r1) + 1u);
	if ((int8_t)r0 > 5)
		r0 = 0xFBu;
	mem_write8(current_object + 0x31u + r1, r0);
	ignored = sub_01DA7C();
}

void sub_01DB85(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = (uint8_t)OBJECT(current_object)->unk_24 > 0u;
	AS8(OBJECT(current_object)->unk_24) = 1u - carry;
	ignored = sub_01DA7C();
}
