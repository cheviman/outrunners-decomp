/**
 *    Name:        game_03.c
 *    Description: the game's functions, part 03 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_011F7C(void)
{
	uint32_t r10 = 0;

	if ((SCREEN(screen)->unk_90 & 0x100u) == 0u || unk_20E60D != 0u)
	{
		r10 = (uint32_t)SCREEN(screen)->unk_00 << 8;
		mem_write8(r10 + SOUND_700710, CAR(player_car)->unk_70 >> 8);
		mem_write8(r10 + SOUND_700712, 0x20u);
		return;
	}
	r10 = (uint32_t)SCREEN(screen)->unk_00 << 8;
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

void sub_011FC3(uint8_t r0)
{
	if (CAR(player_car)->unk_7C == 0u)
	{
		sub_011FC9(r0);
		return;
	}
}

void sub_011FC9(uint8_t r0)
{
	uint32_t r10 = 0;

	if ((SCREEN(screen)->unk_90 & 0x100u) == 0u || unk_20E60D != 0u)
	{
		mem_write8(((uint32_t)SCREEN(screen)->unk_00 << 8) + (SOUND_RAM + 0x718u), r0);
		return;
	}
	r10 = (uint32_t)SCREEN(screen)->unk_00 << 8;
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

void sub_011FE6(void)
{
	uint8_t r0 = 0;

	r0 = 0x20u;
	sub_011FEA(r0);
}

void sub_011FEA(uint8_t r0)
{
	mem_write8(rom_011FFC[SCREEN(screen)->unk_00], r0);
}

void sub_012004(void)
{
	uint32_t r10 = 0;

	r10 = (uint32_t)SCREEN(screen)->unk_00 << 8;
	mem_write8(r10 + SOUND_700700, 0x1Fu);
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

uint32_t sub_0120AC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
	uint32_t r0_2 = 0, r1_2 = 0, r4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	r4 = 0u;
	t1 = r0 - r2;
	r0_2 = t1;
	if ((int32_t)t1 < 0)
	{
		r0_2 = 0u - r0_2;
		r4 = (uint8_t)(r4 + 1u);
	}
	t2 = r1 - r3;
	r1_2 = t2;
	if ((int32_t)t2 < 0)
	{
		r1_2 = 0u - r1_2;
		r4 = (uint8_t)(r4 + 2u);
	}
	if ((int32_t)r0_2 < (int32_t)r1_2)
	{
		t3 = r1_2;
		r1_2 = r0_2;
		r0_2 = t3;
		r4 = (uint8_t)(r4 + 4u);
	}
	if (r0_2 == 0u)
		return r0_2;
	r0_2 = rom_103400[divide_u32((r1_2 << 10) + (r0_2 >> 1), r0_2)];
	r0_2 = (uint16_t)(r0_2 * rom_0121E8[r4]);
	r0_2 = (uint16_t)(r0_2 + rom_0121D8[r4]);
	r0_2 = r0_2 & 0xFFFu;
	return r0_2;
}

void sub_012109(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t *p_r0, uint32_t *p_r1)
{
	uint32_t r0 = *p_r0;
	uint32_t r1 = *p_r1;
	uint32_t r0_2 = 0, r1_2 = 0, r2_2 = 0, r3_2 = 0, r6 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	r6 = 0u;
	t1 = r0 - r3;
	r0_2 = t1;
	if ((int32_t)t1 < 0)
	{
		r0_2 = 0u - r0_2;
		r6 = (uint8_t)(r6 + 1u);
	}
	t2 = r2 - r5;
	r2_2 = t2;
	if ((int32_t)t2 < 0)
	{
		r2_2 = 0u - r2_2;
		r6 = (uint8_t)(r6 + 2u);
	}
	if ((int32_t)r0_2 < (int32_t)r2_2)
	{
		t3 = r2_2;
		r2_2 = r0_2;
		r0_2 = t3;
		r6 = (uint8_t)(r6 + 4u);
	}
	if (r0_2 == 0u)
	{
		r0_2 = 0u;
		r1_2 = 0u;
		*p_r0 = r0_2;
		*p_r1 = r1_2;
		return;
	}
	r2_2 = rom_103400[divide_u32((r2_2 << 10) + (r0_2 >> 1), r0_2)];
	r2_2 = (uint16_t)(r2_2 * rom_0121E8[r6]);
	r2_2 = (uint16_t)(r2_2 + rom_0121D8[r6]);
	r2_2 = r2_2 & 0xFFFu;
	if ((int8_t)r6 < 4)
		r3_2 = (int16_t)rom_100C00[r2_2];
	else
		r3_2 = (int16_t)rom_101400[r2_2];
	r0_2 = divide_s32((r0_2 << 14) + ((int32_t)r3_2 >> 1), r3_2);
	r6 = 5u;
	if ((int32_t)r0_2 < 0)
		r0_2 = 0u - r0_2;
	t4 = r1 - r4;
	r1_2 = t4;
	if ((int32_t)t4 < 0)
	{
		r1_2 = 0u - r1_2;
		r6 = (uint8_t)(r6 - 1u);
	}
	r0_2 = rom_103400[divide_u32((r1_2 << 10) + (r0_2 >> 1), r0_2)];
	r0_2 = (uint16_t)(r0_2 * rom_0121E8[r6]);
	r0_2 = (uint16_t)(r0_2 + rom_0121D8[r6]);
	r0_2 = r0_2 & 0xFFFu;
	r1_2 = r2_2;
	*p_r0 = r0_2;
	*p_r1 = r1_2;
}

void sub_012238(void)
{
	uint16_t r1_2 = 0, r15 = 0, r17 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;

	r10 = SCREEN(screen)->unk_308 + 0x20u;
	r0 = mem_read8(SCREEN(screen)->unk_308 + 0x1Fu);
	r9 = SCREEN(screen)->unk_01;
	if ((SCREEN(screen)->unk_90 & 0x100u) != 0u)
		r0 = 1u;
	r11 = screen + 0x510u;
	SCREEN(r11 - 0x510u)->unk_510 = r0;
	r11 += 4u;
	do
	{
		r1 = mem_read8(r10);
		r10++;
		if ((SCREEN(screen)->unk_90 & 0x100u) != 0u || r9 == r1)
		{
			r1 = r9;
			SCREEN(screen)->unk_500 = r11;
		}
		mem_write16(r11, r1);
		r12 = 0x20FC00u + (uint16_t)(r1 << 7);
		r15 = mem_read16(r12 + 0x40u);
		r17 = mem_read16(r12 + 0x44u);
		r1_2 = mem_read16(r12 + 0x42u);
		r2 = mem_read8(r12 + 0x49u);
		r11 += 2u;
		r11 = sub_0122AD(r1_2, r2, r11, r15, r17);
		r0--;
	} while (r0 != 0u);
	r11 = screen + 0x5B0u;
	r15 = CAR(player_car)->unk_32;
	r17 = CAR(player_car)->unk_38;
	r1_2 = CAR(player_car)->unk_44;
	r2 = CAR(player_car)->unk_60;
	r11 = sub_0122AD(r1_2, r2, r11, r15, r17);
}

uint32_t sub_0122AD(uint16_t r1, uint32_t r2, uint32_t r11, uint16_t r15, uint16_t r17)
{
	uint32_t r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	mem_write32(r11, r12);
	r11 += 4u;
	mem_write16(r11, r1);
	r11 += 2u;
	mem_write16(r11, r1 + 4u);
	r19 = 0x126BCu + ((r2 & ~0xFFu) | (uint8_t)(r2 * 2u)) * 4u;
	t1 = mem_read16(r19);
	r19 += 2u;
	r11 += 2u;
	mem_write16(r11, r15 + t1);
	t2 = mem_read16(r19);
	r19 += 2u;
	r11 += 2u;
	mem_write16(r11, r15 + t2);
	t3 = mem_read16(r19);
	r11 += 2u;
	mem_write16(r11, r17 + t3);
	r11 += 2u;
	mem_write16(r11, r17 + mem_read16(r19 + 2u));
	r11 += 2u;
	return r11;
}

void sub_0122E3(uint32_t *p_r7, uint32_t *p_r15)
{
	r27 = 3u;
	sub_0122EB(p_r7, p_r15);
}

void sub_0122E8(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;

	r27 = 0u;
	sub_0122EB(&r7, &r15);
	*p_r7 = r7;
	*p_r15 = r15;
}

void sub_0122EB(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;
	uint16_t r4 = 0, r6 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint32_t r0 = 0, r7_2 = 0, r10 = 0, r15_2 = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->unk_00 & 8u) != 0u)
		return;
	r0 = (uint16_t)(OBJECT(current_object)->unk_54 >> 16u);
	r15_2 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15_2);
	r15_2 += 2u;
	r13 = mem_read16(r15_2);
	r15_2 += 2u;
	r16 = mem_read16(r15_2);
	r15_2 += 2u;
	r17 = mem_read16(r15_2);
	r15_2 += 2u;
	r18 = mem_read16(r15_2);
	r15_2 += 2u;
	r19 = mem_read16(r15_2);
	r15_2 += 2u;
	if ((int16_t)((r12 - OBJECT(current_object)->unk_58) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = (uint8_t)OBJECT(current_object)->unk_10;
	if (r0 < (uint16_t)r12)
	{
		do
		{
			r0 = (uint16_t)(r0 + r4);
		} while (r0 < (uint16_t)r12);
	}
	do
	{
		if (r13 <= r0)
		{
			*p_r7 = r7;
			*p_r15 = r15_2;
			return;
		}
		r7_2 = r0;
		r6 = (uint16_t)(r0 - r12);
		r0 = (uint16_t)(r0 + r4);
		r7_2 = (uint16_t)(r7_2 << 4);
		r7_2 = r7_2 & 0x7FFu;
		r10 = OBJECT(current_object)->unk_60;
		r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(current_object)->unk_3F * mem_read16(screen + 0x2012u + r7_2 * 2u) + OBJECT(current_object)->unk_20);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(current_object)->unk_28);
		r7 = r6;
		r6 += mem_read16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + mem_read16(r10));
		r10 += 2u;
		r7 = (uint16_t)(r7 - r16);
		r9 = (uint16_t)(r8 + mem_read16(r10 + 2u) - r18);
	} while ((int16_t)(((r8 + mem_read16(r10) - r19) ^ r9) & ((r6 - r17) ^ r7)) >= 0);
	object = CAR(player_car)->object;
	mem_write16(object, mem_read16(object) | 4u);
	AT8(OBJECT(CAR(player_car)->object)->unk_6C, 2) = r27;
	r7 = (uint16_t)((int16_t)r7 >> 1);
	CAR(player_car)->unk_122 = r7;
	CAR(player_car)->unk_120 = (int16_t)r9 >> 1;
	*p_r7 = r7;
	*p_r15 = r15_2;
}

void sub_0123AF(uint32_t *p_r7, uint32_t *p_r15)
{
	uint32_t r7 = *p_r7;
	uint32_t r15 = *p_r15;
	uint16_t r4 = 0, r6 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint32_t r0 = 0, r7_2 = 0, r10 = 0, r15_2 = 0;
	uint16_t t1 = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->unk_00 & 8u) != 0u)
		return;
	r0 = (uint16_t)(OBJECT(current_object)->unk_54 >> 16u);
	r15_2 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15_2);
	r15_2 += 2u;
	r13 = mem_read16(r15_2);
	r15_2 += 2u;
	r16 = mem_read16(r15_2);
	r15_2 += 2u;
	r17 = mem_read16(r15_2);
	r15_2 += 2u;
	r18 = mem_read16(r15_2);
	r15_2 += 2u;
	r19 = mem_read16(r15_2);
	r15_2 += 2u;
	if ((int16_t)((r12 - OBJECT(current_object)->unk_58) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = (uint8_t)OBJECT(current_object)->unk_10;
	if (r0 < (uint16_t)r12)
	{
		do
		{
			r0 = (uint16_t)(r0 + r4);
		} while (r0 < (uint16_t)r12);
	}
	do
	{
		if (r13 <= r0)
		{
			*p_r7 = r7;
			*p_r15 = r15_2;
			return;
		}
		r7_2 = r0;
		r6 = (uint16_t)(r0 - r12);
		r0 = (uint16_t)(r0 + r4);
		r7_2 = (uint16_t)(r7_2 << 4);
		r7_2 = r7_2 & 0x7FFu;
		r10 = OBJECT(current_object)->unk_60;
		r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(current_object)->unk_3F * mem_read16(screen + 0x2012u + r7_2 * 2u) + OBJECT(current_object)->unk_20);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(current_object)->unk_28);
		r7 = r6;
		r9 = (uint16_t)r8;
		r6 += mem_read16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + mem_read16(r10));
		r10 += 2u;
		r8 += mem_read16(r10);
		r9 += mem_read16(r10 + 2u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - r9);
		r7 = (uint16_t)(r7 - r16);
		r9 -= r18;
		r6 = (uint16_t)((r6 - r17) ^ r7);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - r19);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (0u - r8 - r18));
		r28 = (r28 & ~0xFFFFu) | (r28 & r6);
		t1 = r28 | (((r8 - r19) ^ r9) & r6);
		r28 = (r28 & ~0xFFFFu) | t1;
	} while ((int16_t)t1 >= 0);
	object = CAR(player_car)->object;
	mem_write16(object, mem_read16(object) | 4u);
	AT8(OBJECT(CAR(player_car)->object)->unk_6C, 2) = r27;
	r7 = (uint16_t)((int16_t)r7 >> 1);
	CAR(player_car)->unk_122 = r7;
	CAR(player_car)->unk_120 = (int16_t)r9 >> 1;
	*p_r7 = r7;
	*p_r15 = r15_2;
}

void sub_01248C(void)
{
	uint16_t r4 = 0, r6 = 0, r7_2 = 0, r8 = 0, r9 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26 = 0;
	uint32_t r0 = 0, r7 = 0, r10 = 0, r15 = 0;
	uint16_t t1 = 0;
	uint32_t object = 0;

	r27 = 0u;
	if ((OBJECT(CAR(player_car)->object)->unk_00 & 8u) == 0u)
	{
		r0 = (uint16_t)(OBJECT(current_object)->unk_54 >> 16u);
		r15 = SCREEN(screen)->unk_500 + 6u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
		r15 += 2u;
		r13 = mem_read16(r15);
		r15 += 2u;
		r16 = mem_read16(r15);
		r15 += 2u;
		r17 = mem_read16(r15);
		r15 += 2u;
		r18 = mem_read16(r15);
		r19 = mem_read16(r15 + 2u);
		if ((int16_t)((r12 - OBJECT(current_object)->unk_58) ^ (r13 - r0)) >= 0)
			return;
		r4 = (uint8_t)OBJECT(current_object)->unk_10;
		if (r0 < (uint16_t)r12)
		{
			do
			{
				r0 = (uint16_t)(r0 + r4);
			} while (r0 < (uint16_t)r12);
		}
		do
		{
			if (r13 <= r0)
				return;
			r7 = r0;
			r6 = (uint16_t)(r0 - r12);
			r0 = (uint16_t)(r0 + r4);
			r7 = (uint16_t)(r7 << 4);
			r10 = OBJECT(current_object)->unk_60;
			r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(current_object)->unk_3F * mem_read16(screen + 0x2012u + (r7 & 0x7FFu) * 2u) + OBJECT(current_object)->unk_20);
			r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(current_object)->unk_28);
			r7_2 = (uint16_t)r6;
			r9 = (uint16_t)r8;
			r26 = (uint16_t)(0u - r8);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - r9);
			r6 += mem_read16(r10);
			r10 += 2u;
			r7_2 += mem_read16(r10);
			r10 += 2u;
			r8 += mem_read16(r10);
			r26 += mem_read16(r10);
			r10 += 2u;
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + mem_read16(r10));
			r7_2 -= r16;
			r9 = (uint16_t)(r9 + mem_read16(r10) - r18);
			r6 = (uint16_t)((r6 - r17) ^ r7_2);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - r18);
			r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ (r26 - r19));
			r28 = (r28 & ~0xFFFFu) | (r28 & r6);
			t1 = r28 | (((r8 - r19) ^ r9) & r6);
			r28 = (r28 & ~0xFFFFu) | t1;
		} while ((int16_t)t1 >= 0);
		object = CAR(player_car)->object;
		mem_write16(object, mem_read16(object) | 4u);
		AT8(OBJECT(CAR(player_car)->object)->unk_6C, 2) = r27;
		CAR(player_car)->unk_122 = (int16_t)r7_2 >> 1;
		CAR(player_car)->unk_120 = (int16_t)r9 >> 1;
	}
}

void sub_01256F(void)
{
	uint16_t r4 = 0, r6_2 = 0, r7 = 0, r8 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26 = 0;
	uint32_t r5 = 0, r6 = 0, r15 = 0;
	uint32_t object = 0;

	if ((OBJECT(CAR(player_car)->object)->unk_00 & 8u) != 0u)
		return;
	r14 = screen + 0x510u;
	r15 = SCREEN(screen)->unk_500;
	r4 = mem_read16(r15);
	r15 += 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
	r15 += 2u;
	r13 = mem_read16(r15);
	r15 += 2u;
	r16 = (uint16_t)(mem_read16(r15) + 1u);
	r15 += 2u;
	r17 = mem_read16(r15);
	r15 += 2u;
	r18 = mem_read16(r15);
	r19 = mem_read16(r15 + 2u);
	r5 = SCREEN(r14 - 0x510u)->unk_510;
	r14 = r14 + 4u;
	if ((uint8_t)r5 < 2u)
		return;
	for (;;)
	{
		r26 = mem_read16(r14);
		r14 = r14 + 2u;
		if (r4 == r26)
		{
			r14 = r14 + 0x10u;
			r5--;
			if (r5 == 0u)
				return;
			continue;
		}
		r6 = mem_read32(r14);
		r14 = r14 + 4u;
		if ((mem_read32(r6 + 0x58u) & 8u) != 0u)
		{
			r14 = r14 + 0xCu;
			r5--;
			if (r5 == 0u)
				return;
			continue;
		}
		r6_2 = mem_read16(r14);
		r14 = r14 + 2u;
		r8 = (uint16_t)(r12 - mem_read16(r14));
		r14 = r14 + 2u;
		if ((int16_t)((r13 - r6_2) ^ r8) >= 0)
		{
			r14 = r14 + 8u;
			r5--;
			if (r5 == 0u)
				break;
			continue;
		}
		r6_2 = (uint16_t)((r6_2 - r12) << 7);
		r7 = (uint16_t)r6_2;
		r6_2 += mem_read16(r14);
		r14 = r14 + 2u;
		r7 += mem_read16(r14);
		r14 = r14 + 2u;
		r8 = mem_read16(r14);
		r14 = r14 + 2u;
		r8 = (uint16_t)((r8 - r19) ^ (mem_read16(r14) - r18));
		r14 = r14 + 2u;
		if ((int16_t)(r8 & ((r6_2 - r17) ^ (r7 - r16))) < 0)
		{
			object = CAR(player_car)->object;
			mem_write16(object, mem_read16(object) | 4u);
			AT8(OBJECT(CAR(player_car)->object)->unk_6C, 2) = 2u;
			OBJECT(CAR(player_car)->object)->unk_68 = r26;
		}
		r5--;
		if (r5 == 0u)
			return;
	}
}

uint32_t sub_01262A(void)
{
	uint32_t negative = 0;
	uint16_t r8 = 0;
	uint32_t r6 = 0, r7 = 0, r10 = 0, r15 = 0, object = 0;
	uint16_t t1 = 0;

	if ((OBJECT(CAR(player_car)->object)->unk_00 & 8u) != 0u)
	{
		negative = 0u;
		return negative;
	}
	r6 = OBJECT(current_object)->unk_1C;
	r15 = SCREEN(screen)->unk_500 + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
	r15 += 4u;
	r7 = (uint16_t)(r6 << 4);
	r10 = OBJECT(current_object)->unk_60;
	r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(current_object)->unk_3F * mem_read16(screen + 0x2012u + (r7 & 0x7FFu) * 2u) + OBJECT(current_object)->unk_20);
	r6 = (uint16_t)(((r6 - r12) << 7) + (uint16_t)OBJECT(current_object)->unk_28);
	t1 = ((r8 + mem_read16(r10 + 4u) - mem_read16(r15 + 6u)) ^ (r8 + mem_read16(r10 + 6u) - mem_read16(r15 + 4u))) & ((r6 + mem_read16(r10) - mem_read16(r15 + 2u)) ^ (r6 + mem_read16(r10 + 2u) - mem_read16(r15)));
	negative = t1 >> 15;
	if ((int16_t)t1 < 0)
	{
		object = CAR(player_car)->object;
		OBJECT(object)->unk_00 |= 4u;
		AT8(OBJECT(object)->unk_6C, 2) = 1u;
		OBJECT(object)->unk_68 = current_object;
		negative = 1u;
	}
	return negative;
}

void sub_0126FC(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r10 = 0, left_caller = 0;
	uint32_t target1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_80 = road_object_streams[SCREEN(screen)->unk_A3];
		return;
	case 1:
		break;
	case 2:
		if (SCREEN(screen)->unk_A3 != OBJECT(current_object)->unk_86)
			return;
		OBJECT(current_object)->unk_0C--;
		r0_2 = SCREEN(screen)->unk_A3;
		OBJECT(current_object)->unk_84 = CAR(player_car)->unk_AA;
		OBJECT(current_object)->unk_80 = road_object_streams[r0_2];
		break;
	case 3:
		if (SCREEN(screen)->unk_A1 == OBJECT(current_object)->unk_86)
			return;
		OBJECT(current_object)->unk_0C -= 2u;
		OBJECT(current_object)->unk_80 = mem_read32(OBJECT(current_object)->unk_80 + CAR(player_car)->unk_C9 * 4u);
		break;
	default:
		invalid_jump(rom_012708[OBJECT(current_object)->unk_0C]);
	}
	r10 = OBJECT(current_object)->unk_80;
	for (;;)
	{
		r0 = mem_read16(r10);
		r10 += 2u;
		if ((int16_t)CAR(player_car)->unk_46 < (int16_t)r0)
			break;
		r1 = rom_012748[mem_read16(r10)];
		left_caller = 0u;
		r10 += 2u;
		target1 = r1;
		switch (target1)
		{
		case 0x012758u:
			r10 = sub_012758(r0, r10);
			break;
		case 0x012777u:
			left_caller = sub_012777();
			break;
		case 0x01279Cu:
			left_caller = sub_01279C();
			break;
		case 0x012786u:
			left_caller = sub_012786(r10);
			break;
		default:
			invalid_jump(target1);
		}
		if (left_caller != 0u)
			return;
	}
}

uint32_t sub_012758(uint16_t r0, uint32_t r10)
{
	uint32_t r21 = 0;

	r21 = sub_011D43();
	mem_write16(r21 + 0x1Cu, r0 + OBJECT(current_object)->unk_84);
	mem_write32(r21 + 4u, mem_read32(r10));
	r10 += 4u;
	mem_write32(r21 + 0x18u, mem_read32(r10));
	r10 += 4u;
	OBJECT(current_object)->unk_80 = r10;
	return r10;
}

uint32_t sub_012777(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_86 = SCREEN(screen)->unk_A1;
	left_caller = 1u;
	return left_caller;
}

uint32_t sub_012786(uint32_t r10)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->unk_0C += 2u;
	OBJECT(current_object)->unk_80 = r10;
	OBJECT(current_object)->unk_86 = SCREEN(screen)->unk_A3;
	left_caller = 1u;
	return left_caller;
}

uint32_t sub_01279C(void)
{
	uint32_t left_caller = 0;

	OBJECT(current_object)->unk_00 = 0u;
	OBJECT(current_object)->unk_01 = 0u;
	left_caller = 1u;
	return left_caller;
}

void sub_0127EE(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 6u;
		AS32(OBJECT(current_object)->unk_38) = current_object + 0x80u;
		AS16(OBJECT(current_object)->unk_80) = 0x7000u;
		AT16(OBJECT(current_object)->unk_80, 2) = 0xDFu;
		OBJECT(current_object)->unk_84 = 0u;
		OBJECT(current_object)->unk_86 = 0x3Fu;
		OBJECT(current_object)->unk_87 = 1u;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_0127FC[OBJECT(current_object)->unk_0C]);
	}
	if (SCREEN(screen)->unk_1F00 == 0u)
		return;
	AS8(OBJECT(current_object)->unk_10) = 0u;
	OBJECT(current_object)->unk_3C = 0xFFFFu;
	do
	{
		r1 = (uint8_t)OBJECT(current_object)->unk_10;
		r0 = mem_read16(screen + 0x1F10u + r1 * 2u);
		if (r0 < OBJECT(current_object)->unk_3C)
		{
			OBJECT(current_object)->unk_3C = r0;
			r0_2 = (uint16_t)(mem_read16(screen + 0x1F30u + r1 * 2u) + SCREEN(screen)->unk_16 - 1u);
			AT16(OBJECT(current_object)->unk_80, 2) = r0_2;
			sub_00EAE4(r0_2);
		}
		AS8(OBJECT(current_object)->unk_10)++;
	} while ((uint8_t)OBJECT(current_object)->unk_10 <= SCREEN(screen)->unk_1F00);
}

void sub_01287C(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_012894();
		return;
	case 1:
		sub_0122E8(&r7, &r15);
		sub_00F5C8(r7, r15);
		return;
	case 2:
		sub_00F5C8(0u, 0u);
		return;
	default:
		invalid_jump(rom_012888[OBJECT(current_object)->unk_0C]);
	}
}

void sub_012894(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->unk_0C++;
	r10 = OBJECT(current_object)->unk_18;
	r9 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_00 |= mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_45 = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_44 = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_3F = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_10 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
	r10 += 2u;
	if (mem_read32(r10) == 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_60 = r10;
	sub_00F366(r9);
	r0 = OBJECT(current_object)->unk_1C;
	r1 = (uint16_t)(OBJECT(current_object)->unk_10 * (AS16(OBJECT(current_object)->unk_12) - 1u) + 1u);
	AT16(OBJECT(current_object)->unk_54, 2) = r0;
	OBJECT(current_object)->unk_58 = r0 + r1;
}

void sub_012906(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r7 = 0, r9 = 0, r10 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_18 = mem_read32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_45 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_24 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_10 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
		r10 += 2u;
		if (mem_read32(r10) == 0u)
			OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_60 = r10;
		sub_00F366(r9);
		r0 = OBJECT(current_object)->unk_1C;
		r1 = (uint16_t)(OBJECT(current_object)->unk_10 * (AS16(OBJECT(current_object)->unk_12) - 1u) + 1u);
		AT16(OBJECT(current_object)->unk_54, 2) = r0;
		OBJECT(current_object)->unk_58 = r0 + r1;
		return;
	case 1:
		sub_0122E8(&r7, &r15);
		r10 = OBJECT(current_object)->unk_18;
		r0_2 = (int16_t)mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
		{
			r10 += r0_2;
			r0_2 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(current_object)->unk_3A = r0_2;
		OBJECT(current_object)->unk_18 = r10;
		sub_00F5C8(r7, r15);
		return;
	case 2:
		r10 = OBJECT(current_object)->unk_18;
		r0_2 = (int16_t)mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
		{
			r10 += r0_2;
			r0_2 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(current_object)->unk_3A = r0_2;
		OBJECT(current_object)->unk_18 = r10;
		sub_00F5C8(r7, r15);
		return;
	default:
		invalid_jump(rom_012914[OBJECT(current_object)->unk_0C]);
	}
}

void sub_0129AB(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_012894();
		return;
	case 1:
		sub_0123AF(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_0129B8[OBJECT(current_object)->unk_0C]);
	}
	sub_00F911(r7, r15);
}

void sub_0129F4(void)
{
	uint32_t r7 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		AS8(OBJECT(current_object)->unk_5C) = 0xFFu;
		sub_012894();
		return;
	case 1:
	case 2:
		r7 = sub_00F30D(0u);
		sub_00F5C8(r7, 0u);
		return;
	default:
		invalid_jump(rom_012A00[OBJECT(current_object)->unk_0C]);
	}
}

void sub_012A20(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = (uint16_t)rom_012A2C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		OBJECT(current_object)->unk_18 = mem_read32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		AT8(OBJECT(current_object)->unk_08, 1) = mem_read8(r10 + 1u);
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_30 = 0xFF60u;
		if (CAR(player_car)->unk_60 != 6u)
		{
			OBJECT(current_object)->unk_32 = 0x40u;
			sub_012AAB();
		}
		else
		{
			OBJECT(current_object)->unk_32 = 0x30u;
			sub_012AAB();
		}
		OBJECT(current_object)->unk_30 += 8u;
		if ((int16_t)OBJECT(current_object)->unk_30 >= 0xA0)
			OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		OBJECT(current_object)->unk_30 += 8u;
		if ((int16_t)OBJECT(current_object)->unk_30 >= 0xA0)
			OBJECT(current_object)->unk_0C++;
		sub_012AA5();
		r0 = (uint16_t)((r0 & ~0xFFu) | (uint8_t)OBJECT(current_object)->unk_10);
		t2 = r0 & 1u;
		r0_2 = (r0 & ~0xFFu) | t2;
		if (t2 == 0u)
			return;
		sub_00F30D(0u);
		r0_2 = sub_00EAE4(r0_2);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_012A2C[t1]);
	}
	sub_012AA5();
	r0 = (uint16_t)((r0 & ~0xFFu) | (uint8_t)OBJECT(current_object)->unk_10);
	t2 = r0 & 1u;
	r0_2 = (r0 & ~0xFFu) | t2;
	if (t2 == 0u)
		return;
	sub_00F30D(0u);
	r0_2 = sub_00EAE4(r0_2);
}

void sub_012AA5(void)
{
	uint8_t t1 = 0;

	t1 = (uint8_t)OBJECT(current_object)->unk_10;
	AS8(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		sub_012AAB();
		return;
	}
}

void sub_012AAB(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r0 = (int16_t)mem_read16(r10);
	r10 += 2u;
	if ((int16_t)r0 < 0)
	{
		r10 += r0;
		r0 = (int16_t)mem_read16(r10);
		r10 += 2u;
	}
	r9 = r0;
	OBJECT(current_object)->unk_18 = r10;
	AS8(OBJECT(current_object)->unk_10) = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	sub_00F366(r9);
}

void sub_012ACF(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_012894();
		return;
	case 2:
		sub_00F5C8(0u, 0u);
		return;
	case 1:
		sub_0122E3(&r7, &r15);
		sub_00F5C8(r7, r15);
		return;
	default:
		invalid_jump(rom_012ADC[OBJECT(current_object)->unk_0C]);
	}
}

void sub_012AF4(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_012B0C();
		return;
	case 1:
		sub_0122E8(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_012B00[OBJECT(current_object)->unk_0C]);
	}
	r7 = sub_00F30D(r7);
	sub_00F5C8(r7, r15);
}

void sub_012B0C(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->unk_0C++;
	r10 = OBJECT(current_object)->unk_18;
	r9 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_00 |= mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_45 = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_44 = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_3F = mem_read8(r10);
	r10++;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_10 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
	r10 += 2u;
	if (mem_read32(r10) == 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_60 = r10;
	sub_00F366(r9);
	r0 = OBJECT(current_object)->unk_1C;
	r1 = (uint16_t)(OBJECT(current_object)->unk_10 * (AS16(OBJECT(current_object)->unk_12) - 1u) + 1u);
	AT16(OBJECT(current_object)->unk_54, 2) = r0;
	OBJECT(current_object)->unk_58 = r0 + r1;
}

void sub_012B84(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_012B0C();
		return;
	case 1:
		sub_0123AF(&r7, &r15);
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_012B90[OBJECT(current_object)->unk_0C]);
	}
	r7 = sub_00F30D(r7);
	sub_00F911(r7, r15);
}

void sub_012BAE(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_012BBC[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x165u)
			AS32(OBJECT(current_object)->unk_24) = (int16_t)0xFF00u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(current_object)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		if ((int16_t)CAR(player_car)->unk_44 < (int16_t)OBJECT(current_object)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)CAR(player_car)->unk_42 < (int16_t)OBJECT(current_object)->unk_1C)
		{
			OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->unk_44 < (int16_t)OBJECT(current_object)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)CAR(player_car)->unk_42 < (int16_t)OBJECT(current_object)->unk_1C)
		{
			OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 3:
		if ((int16_t)CAR(player_car)->unk_42 < (int16_t)OBJECT(current_object)->unk_1C)
		{
			OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012BBC[t1]);
	}
}

void sub_012CAA(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_012CB8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(current_object)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		if ((int16_t)CAR(player_car)->unk_44 < (int16_t)OBJECT(current_object)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(current_object)->unk_28 + (CAR(player_car)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				AS16(OBJECT(current_object)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0 || (int16_t)OBJECT(current_object)->unk_30 > 0x130)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	case 2:
		if ((int16_t)CAR(player_car)->unk_44 < (int16_t)OBJECT(current_object)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(current_object)->unk_28 + (CAR(player_car)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				AS16(OBJECT(current_object)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0 || (int16_t)OBJECT(current_object)->unk_30 > 0x130)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	case 3:
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_20 += OBJECT(current_object)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(current_object)->unk_28 + (CAR(player_car)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(current_object)->unk_1C++;
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				AS16(OBJECT(current_object)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0 || (int16_t)OBJECT(current_object)->unk_30 > 0x130)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		return;
	default:
		invalid_jump(rom_012CB8[t1]);
	}
}

void sub_012DB7(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_012DC4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		AS16(OBJECT(current_object)->unk_80) += 0x10u;
		r10 += 2u;
		AT16(OBJECT(current_object)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		AS16(OBJECT(current_object)->unk_86) = mem_read16(r10 + 2u);
		sub_00F366(r9);
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - (uint16_t)OBJECT(current_object)->unk_80;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)OBJECT(current_object)->unk_58)
			return;
		OBJECT(current_object)->unk_0C++;
		if ((int16_t)CAR(player_car)->unk_42 < (int16_t)OBJECT(current_object)->unk_1C)
		{
			OBJECT(current_object)->unk_20 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
			OBJECT(current_object)->unk_24 += OBJECT(current_object)->unk_84;
			OBJECT(current_object)->unk_1C += AS16(OBJECT(current_object)->unk_86);
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)CAR(player_car)->unk_42 < (int16_t)OBJECT(current_object)->unk_1C)
		{
			OBJECT(current_object)->unk_20 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
			OBJECT(current_object)->unk_24 += OBJECT(current_object)->unk_84;
			OBJECT(current_object)->unk_1C += AS16(OBJECT(current_object)->unk_86);
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012DC4[t1]);
	}
}

void sub_012EA2(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_012EB0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		AS16(OBJECT(current_object)->unk_80) += 0x10u;
		r10 += 2u;
		AT16(OBJECT(current_object)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_86) = mem_read16(r10);
		r10 += 2u;
		AS8(OBJECT(current_object)->unk_88) = mem_read8(r10);
		AT16(OBJECT(current_object)->unk_88, 2) = mem_read16(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - (uint16_t)OBJECT(current_object)->unk_80;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)OBJECT(current_object)->unk_58 < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_12 = (uint8_t)OBJECT(current_object)->unk_88;
			r0 = sub_00EAE4(r0);
			return;
		}
		return;
	case 2:
		OBJECT(current_object)->unk_24 += (uint16_t)(OBJECT(current_object)->unk_88 >> 16u);
		t2 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t2 - 1u;
		if (t2 != 1u)
		{
			OBJECT(current_object)->unk_1C += AS16(OBJECT(current_object)->unk_86);
			if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
					return;
			}
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_0C++;
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_20 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		OBJECT(current_object)->unk_24 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_1C += AS16(OBJECT(current_object)->unk_86);
		if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
		{
			sub_00F30D(0u);
			sub_00EAE4(r0);
			if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
				return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	case 3:
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_20 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		OBJECT(current_object)->unk_24 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_1C += AS16(OBJECT(current_object)->unk_86);
		if ((int16_t)(0u - OBJECT(current_object)->unk_10 + OBJECT(current_object)->unk_1C) < (int16_t)CAR(player_car)->unk_42)
		{
			sub_00F30D(0u);
			sub_00EAE4(r0);
			if ((int16_t)OBJECT(current_object)->unk_30 >= -0xF0 && (int16_t)OBJECT(current_object)->unk_30 <= 0x130)
				return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_012EB0[t1]);
	}
}

void sub_012FBC(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_012FC8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_24 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_10 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
		r10 += 2u;
		if (mem_read32(r10) == 0u)
			OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_60 = r10;
		sub_00F366(r9);
		r0_2 = OBJECT(current_object)->unk_1C;
		r1 = (uint16_t)(OBJECT(current_object)->unk_10 * (AS16(OBJECT(current_object)->unk_12) - 1u) + 1u);
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2;
		OBJECT(current_object)->unk_58 = r0_2 + r1;
		return;
	case 1:
		sub_0122E8(&ignored, &ignored);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = OBJECT(CAR(player_car)->object)->unk_10;
		r0 = (uint16_t)(r0 + 0x1Du);
		r0 = r0 / 0x1Eu;
		OBJECT(current_object)->unk_3A = rom_01306E[r0];
		if (r0 == 0u)
			OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_012FC8[t1]);
	}
}

void sub_01307E(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01308C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		OBJECT(current_object)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_3E = 9u;
		OBJECT(current_object)->unk_44 = 1u;
		return;
	case 1:
		r0 = OBJECT(CAR(player_car)->object)->unk_10;
		if (r0 != 0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		sub_00F30D(0u);
		sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		sub_00F30D(0u);
		sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01308C[t1]);
	}
}

void sub_013104(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_013110[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		OBJECT(current_object)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_3E = 9u;
		OBJECT(current_object)->unk_44 = 1u;
		OBJECT(current_object)->unk_0C++;
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - 0x1Eu;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		r9 = (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_013110[t1]);
	}
}

void sub_0131A5(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		OBJECT(current_object)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_3E = 9u;
		OBJECT(current_object)->unk_44 = 1u;
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->unk_10 != 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42 || (int16_t)OBJECT(current_object)->unk_24 < -0x3000)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = (uint16_t)(AS16(OBJECT(current_object)->unk_12) + 1u);
		AS16(OBJECT(current_object)->unk_12) = r0;
		r0 = r0 >> 1;
		OBJECT(current_object)->unk_24 -= r0;
		OBJECT(current_object)->unk_20 += 8u;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)CAR(player_car)->unk_42 || (int16_t)OBJECT(current_object)->unk_24 < -0x3000)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = (uint16_t)(AS16(OBJECT(current_object)->unk_12) + 1u);
		AS16(OBJECT(current_object)->unk_12) = r0;
		r0 = r0 >> 1;
		OBJECT(current_object)->unk_24 -= r0;
		OBJECT(current_object)->unk_20 += 8u;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0131B0[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01322D(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_10 = CAR(player_car)->unk_44 + OBJECT(current_object)->unk_18;
		OBJECT(current_object)->unk_0C++;
		r0_2 = 0x51u;
		sub_011EB8(r0_2);
		return;
	case 1:
		r0 = sub_011E45();
		OBJECT(current_object)->unk_12 = (r0 & 0x3Fu) + 0xFu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_013238[OBJECT(current_object)->unk_0C]);
	}
	if ((int16_t)OBJECT(current_object)->unk_10 <= (int16_t)CAR(player_car)->unk_44)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(current_object)->unk_12;
	OBJECT(current_object)->unk_12 = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(current_object)->unk_0C--;
	r0_2 = 0x51u;
	sub_011EB8(r0_2);
}

void sub_013288(void)
{
	uint8_t r0 = 0;

	OBJECT(current_object)->unk_00 = 0u;
	r0 = 0x23u;
	sub_011EB8(r0);
}

void sub_013296(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0x19Du;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x118u;
		r9 = 0x39u;
		AS16(OBJECT(current_object)->unk_80) = 0x67u;
		sub_00F366(r9);
		return;
	case 1:
		OBJECT(current_object)->unk_32 -= 2u;
		if (OBJECT(current_object)->unk_32 == 0xDCu)
			OBJECT(current_object)->unk_0C++;
		if (mem_read16(SCREEN(screen)->unk_308 + 0x6Au) == 0u || CAR(player_car)->unk_44 > 0x250u)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_32 += 2u;
			if (OBJECT(current_object)->unk_32 == 0x118u)
			{
				OBJECT(current_object)->unk_00 = 0u;
				OBJECT(current_object)->unk_01 = 0u;
				return;
			}
		}
		r0 = OBJECT(current_object)->unk_10;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->unk_10 = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(current_object)->unk_80);
		OBJECT(current_object)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read16(SCREEN(screen)->unk_308 + 0x6Au) == 0u || CAR(player_car)->unk_44 > 0x250u)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_32 += 2u;
			if (OBJECT(current_object)->unk_32 == 0x118u)
			{
				OBJECT(current_object)->unk_00 = 0u;
				OBJECT(current_object)->unk_01 = 0u;
				return;
			}
		}
		r0 = OBJECT(current_object)->unk_10;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->unk_10 = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(current_object)->unk_80);
		OBJECT(current_object)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(current_object)->unk_32 += 2u;
		if (OBJECT(current_object)->unk_32 == 0x118u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		r0 = OBJECT(current_object)->unk_10;
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		OBJECT(current_object)->unk_10 = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(current_object)->unk_80);
		OBJECT(current_object)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0132A4[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01333C(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_013348[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->unk_24 = 0xFE80u;
		if (r9 == 0x3A0u)
			OBJECT(current_object)->unk_24 = 0xFF00u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(current_object)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		OBJECT(current_object)->unk_12 = 0xAu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
	case 2:
		t2 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_12 = 0xFu;
			r0 = OBJECT(current_object)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(current_object)->unk_20 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(current_object)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 |= 0x40u;
		t3 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_12 = 0xFu;
			r0 = OBJECT(current_object)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(current_object)->unk_20 -= (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C = 2u;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		t3 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_12 = 0xFu;
			r0 = OBJECT(current_object)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(current_object)->unk_20 -= (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C = 2u;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_013348[t1]);
	}
}

void sub_01344F(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_01345C[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->unk_24 = 0xFE80u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(current_object)->unk_30 <= 0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 &= 0xBFu;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 |= 0x40u;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(current_object)->unk_30 <= 0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 &= 0xBFu;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 |= 0x40u;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 < -0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 |= 0x40u;
		}
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01345C[t1]);
	}
}

void sub_01351E(void)
{
	uint8_t r0_2 = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r7 = 0, r9 = 0, r10 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_45 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_24 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_10 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		AT8(OBJECT(current_object)->unk_80, 2) = 0x3Cu;
		OBJECT(current_object)->unk_60 = r10;
		sub_00F366(r9);
		r0 = OBJECT(current_object)->unk_1C;
		r1 = (uint16_t)(OBJECT(current_object)->unk_10 * (AS16(OBJECT(current_object)->unk_12) - 1u) + 1u);
		AT16(OBJECT(current_object)->unk_54, 2) = r0;
		OBJECT(current_object)->unk_58 = r0 + r1;
		return;
	case 1:
	case 2:
		t1 = (uint8_t)(OBJECT(current_object)->unk_80 >> 16u);
		AT8(OBJECT(current_object)->unk_80, 2) = t1 - 1u;
		if (t1 == 1u)
		{
			AT8(OBJECT(current_object)->unk_80, 2) = 0x3Cu;
			r0_2 = (uint8_t)(uint16_t)OBJECT(current_object)->unk_80;
			sub_011EB8(r0_2);
		}
		r7 = sub_00F30D(0u);
		sub_00F5C8(r7, 0u);
		return;
	case 3:
		r7 = sub_00F30D(0u);
		sub_00F5C8(r7, 0u);
		return;
	default:
		invalid_jump(rom_01352C[OBJECT(current_object)->unk_0C]);
	}
}

void sub_0135CC(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0135D8[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(current_object)->unk_24 = 0xFE80u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		r0_2 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0_2 - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0_2 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(current_object)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 |= 0x40u;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(current_object)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 |= 0x40u;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0135D8[t1]);
	}
}

void sub_01369A(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(current_object)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_3E = mem_read8(r10);
		OBJECT(current_object)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		r0 = OBJECT(current_object)->unk_1C;
		OBJECT(current_object)->unk_58 = r0 - 0x64u;
		r1 = SCREEN(screen)->unk_12 >> 7;
		AT16(OBJECT(current_object)->unk_54, 2) = r0 - r1;
		OBJECT(current_object)->unk_10 = r1;
		return;
	case 1:
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->unk_46 <= (int16_t)(uint16_t)(OBJECT(current_object)->unk_54 >> 16u))
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		if ((int16_t)OBJECT(current_object)->unk_30 >= -0xB0)
		{
			r0_2 = sub_011E45();
			t1 = r0_2 & 3u;
			r0_2 = (r0_2 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0_2 = (r0_2 & ~0xFFu) | 0x44u;
				sub_011EB8((uint8_t)r0_2);
			}
			sub_00F30D(0u);
			r0_2 = sub_00EAE4(r0_2);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 |= 0x40u;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		if ((int16_t)OBJECT(current_object)->unk_30 <= 0xF0)
		{
			r0_2 = sub_011E45();
			t1 = r0_2 & 3u;
			r0_2 = (r0_2 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0_2 = (r0_2 & ~0xFFu) | 0x44u;
				sub_011EB8((uint8_t)r0_2);
			}
			sub_00F30D(0u);
			r0_2 = sub_00EAE4(r0_2);
			return;
		}
		OBJECT(current_object)->unk_0C--;
		OBJECT(current_object)->unk_00 &= 0xBFu;
		r0_2 = sub_011E45();
		t1 = r0_2 & 3u;
		r0_2 = (r0_2 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0_2 = (r0_2 & ~0xFFu) | 0x44u;
			sub_011EB8((uint8_t)r0_2);
		}
		sub_00F30D(0u);
		r0_2 = sub_00EAE4(r0_2);
		return;
	case 2:
		OBJECT(current_object)->unk_20 += (uint16_t)OBJECT(current_object)->unk_80;
		if ((int16_t)OBJECT(current_object)->unk_30 >= -0xB0)
		{
			r0_2 = sub_011E45();
			t1 = r0_2 & 3u;
			r0_2 = (r0_2 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0_2 = (r0_2 & ~0xFFu) | 0x44u;
				sub_011EB8((uint8_t)r0_2);
			}
			sub_00F30D(0u);
			r0_2 = sub_00EAE4(r0_2);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_00 |= 0x40u;
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		if ((int16_t)OBJECT(current_object)->unk_30 <= 0xF0)
		{
			r0_2 = sub_011E45();
			t1 = r0_2 & 3u;
			r0_2 = (r0_2 & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				r0_2 = (r0_2 & ~0xFFu) | 0x44u;
				sub_011EB8((uint8_t)r0_2);
			}
			sub_00F30D(0u);
			r0_2 = sub_00EAE4(r0_2);
			return;
		}
		OBJECT(current_object)->unk_0C--;
		OBJECT(current_object)->unk_00 &= 0xBFu;
		r0_2 = sub_011E45();
		t1 = r0_2 & 3u;
		r0_2 = (r0_2 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0_2 = (r0_2 & ~0xFFu) | 0x44u;
			sub_011EB8((uint8_t)r0_2);
		}
		sub_00F30D(0u);
		r0_2 = sub_00EAE4(r0_2);
		return;
	case 3:
		OBJECT(current_object)->unk_20 -= (uint16_t)OBJECT(current_object)->unk_80;
		if ((int16_t)OBJECT(current_object)->unk_30 > 0xF0)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_00 &= 0xBFu;
		}
		r0_2 = sub_011E45();
		t1 = r0_2 & 3u;
		r0_2 = (r0_2 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			r0_2 = (r0_2 & ~0xFFu) | 0x44u;
			sub_011EB8((uint8_t)r0_2);
		}
		sub_00F30D(0u);
		r0_2 = sub_00EAE4(r0_2);
		return;
	default:
		invalid_jump(rom_0136A8[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01376C(void)
{
	uint32_t negative = 0, overflow = 0;
	uint16_t r0_2 = 0, r1 = 0, r6 = 0, r7 = 0, r8 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r21 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_1C = 0u;
		r0 = SCREEN(screen)->unk_A3;
		OBJECT(current_object)->unk_1E = r0;
		OBJECT(current_object)->unk_18 = object_lists_2E1F0[r0];
		OBJECT(current_object)->unk_2C = (uint8_t)(8u - mem_read8(SCREEN(screen)->unk_308 + 0x1Fu) + unk_20E630);
		SCREEN(screen)->unk_FA6 = SCREEN(screen)->unk_12 >> 7;
		SCREEN(screen)->unk_F80 = SCREEN(screen)->unk_308 + 0x1Fu;
		SCREEN(screen)->unk_FA8 = 0u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		r10 = OBJECT(current_object)->unk_18;
		if ((SCREEN(screen)->unk_90 & 0x100u) == 0u && OBJECT(current_object)->unk_1E != SCREEN(screen)->unk_352)
		{
			OBJECT(current_object)->unk_1C = SCREEN(screen)->unk_350;
			r0 = SCREEN(screen)->unk_352;
			OBJECT(current_object)->unk_1E = r0;
			r10 = object_lists_2E1F0[r0];
			OBJECT(current_object)->unk_18 = r10;
		}
		for (;;)
		{
			r0_2 = mem_read16(r10);
			t1 = OBJECT(current_object)->unk_1C;
			t2 = r0_2 + t1;
			overflow = (uint32_t)(int16_t)r0_2 + (int16_t)t1 != (uint32_t)(int16_t)(r0_2 + t1);
			negative = t2 >> 15;
			r0_2 = t2;
			r10 += 2u;
			if (negative ^ overflow)
				break;
			r1 = (uint16_t)r0_2;
			if ((int16_t)mem_read16(SCREEN(screen)->unk_60 + 0x46u) < (int16_t)(r0_2 - SCREEN(screen)->unk_FA6))
				break;
			t3 = mem_read16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->unk_2C < t3 || SCREEN(screen)->unk_FA8 >= 0x20u)
			{
				r10 += 0xCu;
				OBJECT(current_object)->unk_18 = r10;
				continue;
			}
			r21 = sub_011D43();
			mem_write16(r21 + 0x1Cu, r1);
			mem_write32(r21 + 4u, mem_read32(r10));
			r10 += 4u;
			mem_write16(r21 + 0x84u, mem_read32(r10));
			r10 += 4u;
			mem_write32(r21 + 0x18u, mem_read32(r10));
			r10 += 4u;
			OBJECT(current_object)->unk_18 = r10;
			SCREEN(screen)->unk_FA8++;
		}
		r10 = SCREEN(screen)->unk_F80;
		r11 = screen + 0xF84u;
		r6 = SCREEN(screen)->unk_FA6;
		r7 = 0u;
		r8 = 0xFFFFu;
		r9 = mem_read8(r10);
		r10++;
		do
		{
			r0_2 = unk_20FC46[(uint32_t)mem_read8(r10) << 6];
			r1 = (uint16_t)((uint32_t)r0_2 + r6);
			r0_2 -= r6;
			r1++;
			r10++;
			if (r1 < r8)
			{
				mem_write16(r11, r8);
				r11 += 2u;
				mem_write16(r11, r1 - 1u);
				r7++;
				r11 += 2u;
			}
			r8 = (uint16_t)r0_2;
			r9--;
		} while (r9 != 0u);
		mem_write16(r11, r8);
		SCREEN(screen)->unk_F84 = r7;
		return;
	default:
		invalid_jump(rom_013778[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01389E(void)
{
	uint16_t r0 = 0;
	uint32_t r1 = 0, r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r0 = (uint16_t)mem_read32(r10);
	r11 = current_object;
	r10 += 4u;
	for (;;)
	{
		r12 = mem_read32(r10);
		r10 += 4u;
		if (r12 == 0u)
			break;
		r21 = sub_011D43();
		r1 = mem_read16(r11 + 0x1Cu);
		r1 = (uint16_t)(r1 - r0);
		mem_write16(r21 + 0x1Cu, r1);
		mem_write32(r21 + 4u, sub_013AB7_ADDRESS);
		mem_write16(r21 + 0x84u, mem_read16(r11 + 0x84u));
		mem_write32(r21 + 0x18u, r12);
		mem_write32(r21 + 0x14u, r11);
		r11 = r21;
		SCREEN(screen)->unk_FA8++;
	}
	OBJECT(current_object)->unk_18 = r10;
	OBJECT(current_object)->unk_04 = sub_0138EB_ADDRESS;
	sub_0138EB(r1);
}

void sub_0138EB(uint32_t r1)
{
	uint32_t r0 = 0, r1_2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0138F8[t1];
	switch (t1)
	{
	case 0:
		sub_013900();
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_0138F8[t1]);
	}
	if (CAR(player_car)->unk_181 == 0u)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_28;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
		r1_2 = (r1 & ~0xFFFFu) | 0x80u;
		if ((int16_t)r1_2 <= (int16_t)r0)
		{
			do
			{
				OBJECT(current_object)->unk_1C++;
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1_2);
			} while ((int16_t)r1_2 <= (int16_t)r0);
		}
		sub_01397E(r0, r1_2);
		return;
	}
	r0 = (r0 & ~0xFFFFu) | OBJECT(current_object)->unk_1C;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
	if ((int16_t)r0 < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_28;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
	r1_2 = (r1 & ~0xFFFFu) | 0x80u;
	if ((int16_t)r1_2 <= (int16_t)r0)
	{
		do
		{
			OBJECT(current_object)->unk_1C++;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1_2);
		} while ((int16_t)r1_2 <= (int16_t)r0);
	}
	sub_01397E(r0, r1_2);
}

void sub_013900(void)
{
	uint32_t r9 = 0, r10 = 0, r11 = 0;
	uint8_t t3 = 0, t4 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r11 = mem_read32(r10);
	t1 = mem_read16(r11);
	r11 += 2u;
	r9 = t1;
	OBJECT(current_object)->unk_3E = 0xBu;
	t2 = mem_read16(r11);
	r11 += 2u;
	OBJECT(current_object)->unk_3A = t2;
	OBJECT(current_object)->unk_44 = 0xFFu;
	t3 = mem_read8(r11);
	r11++;
	AT8(OBJECT(current_object)->unk_80, 1) = t3;
	t4 = mem_read8(r11);
	r11++;
	AT8(OBJECT(current_object)->unk_80, 2) = t4;
	t5 = mem_read16(r11);
	OBJECT(current_object)->unk_6A = t5;
	OBJECT(current_object)->unk_60 = r11 + 2u;
	r10 += 4u;
	OBJECT(current_object)->unk_3F = (uint8_t)mem_read16(r10);
	r10 += 2u;
	AS32(OBJECT(current_object)->unk_20) = (int16_t)mem_read16(r10);
	OBJECT(current_object)->unk_2C = mem_read16(r10 + 2u);
	AS8(OBJECT(current_object)->unk_80) = 0u;
	OBJECT(current_object)->unk_0C++;
	sub_00F366(r9);
}

void sub_01397E(uint32_t r0, uint32_t r1)
{
	uint32_t zero = 0, negative = 0, carry = 0;
	uint8_t r0_3 = 0, r2_2 = 0;
	uint16_t r1_3 = 0, r2 = 0, r3 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r10 = 0, race = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t t1 = 0;

	AS16(OBJECT(current_object)->unk_28) = r0;
	r0_2 = (r0 & ~0xFFFFu) | OBJECT(current_object)->unk_1C;
	zero = sub_013D29((uint16_t)r0_2);
	if (zero)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	race = SCREEN(screen)->race_copy;
	r1_2 = (r1 & ~0xFFFFu) | (uint16_t)r0_2;
	r1_2 = (r1_2 & ~0xFFFFu) | (uint16_t)(r1_2 - SCREEN(screen)->unk_FA6);
	r2 = RACE(race)->unk_42;
	if ((uint8_t)OBJECT(current_object)->unk_80 != 0u)
	{
		if ((int16_t)r2 < (int16_t)r0_2)
		{
			AS8(OBJECT(current_object)->unk_80) = ~(uint8_t)OBJECT(current_object)->unk_80;
			stack_4 = r0_2;
			stack_8 = r1_2;
			r0_3 = (uint8_t)(OBJECT(current_object)->unk_80 >> 8u);
			sub_011EB8(r0_3);
			r1_2 = stack_8;
			r0_2 = stack_4;
		}
	}
	else if ((int16_t)r0_2 < (int16_t)r2)
	{
		AS8(OBJECT(current_object)->unk_80) = ~(uint8_t)OBJECT(current_object)->unk_80;
		stack_4 = r0_2;
		stack_8 = r1_2;
		r0_3 = (uint8_t)(OBJECT(current_object)->unk_80 >> 8u);
		sub_011EB8(r0_3);
		r1_2 = stack_8;
		r0_2 = stack_4;
	}
	r2 = RACE(race)->unk_46;
	if ((int16_t)r0_2 < (int16_t)r2 || (int16_t)r2 <= (int16_t)r1_2)
		return;
	r10 = screen + 0x2000u + CAR(player_car)->unk_40;
	r0_2 = mem_read16(r10);
	r2 = mem_read16(r10 + 2u);
	r10 = screen + 0x2000u + (((uint32_t)OBJECT(current_object)->unk_1C << 5) & 0xFFFu);
	r3 = mem_read16(r10 + 2u);
	r0_2 = (uint16_t)((int16_t)((r0_2 << 4) - ((uint32_t)mem_read16(r10) << 4)) >> 6);
	r1_3 = (uint16_t)((int16_t)OBJECT(current_object)->unk_30 >> 2);
	carry = (uint16_t)(r0_2 + r1_3) < r0_2;
	r0_2 = (uint16_t)(r0_2 + r1_3);
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 0x10u + carry);
	if ((int8_t)r0_2 >= 4)
	{
		if ((int8_t)r0_2 > 0x1B)
			r0_2 = (r0_2 & ~0xFFu) | 0x1Bu;
	}
	else
		r0_2 = (r0_2 & ~0xFFu) | 4u;
	r2 = (uint16_t)(((uint32_t)r2 << 4) - ((uint32_t)r3 << 4) + ((int16_t)((uint32_t)CAR(player_car)->unk_2C << 4) >> 2));
	carry = (r2 >> 9) & 1u;
	t1 = ((int16_t)r2 >> 10) + carry;
	r2_2 = t1;
	if ((int8_t)t1 >= 0)
	{
		if ((int8_t)r2_2 > 3)
			r2_2 = 3u;
	}
	else
		r2_2 = 0u;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + ((uint32_t)r2_2 << 5));
	if (OBJECT(current_object)->unk_87 != 0u)
	{
		r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + rom_013D45[OBJECT(current_object)->unk_87]);
		OBJECT(current_object)->unk_87++;
		if (OBJECT(current_object)->unk_87 == 0x15u)
			OBJECT(current_object)->unk_87 = 0u;
	}
	OBJECT(current_object)->unk_40 = r0_2;
	negative = sub_01262A();
	if (negative)
	{
		sub_013CF6();
		OBJECT(current_object)->unk_87 = 1u;
	}
	r0_2 = sub_00EAE4(r0_2);
}

void sub_013AB7(void)
{
	if (mem_read8(OBJECT(current_object)->unk_14) == 0u)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	sub_0138EB(0u);
	if (OBJECT(current_object)->unk_00 == 0u)
	{
		OBJECT(current_object)->unk_00 = 1u;
		SCREEN(screen)->unk_FA8++;
	}
}

void sub_013AD8(void)
{
	if (mem_read8(OBJECT(current_object)->unk_14) == 0u)
	{
		SCREEN(screen)->unk_FA8--;
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	sub_013AF9();
	if (OBJECT(current_object)->unk_00 == 0u)
	{
		OBJECT(current_object)->unk_00 = 1u;
		SCREEN(screen)->unk_FA8++;
	}
}

void sub_013AF9(void)
{
	uint32_t r0 = 0, r1 = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_013B04[t1];
	switch (t1)
	{
	case 0:
		sub_013900();
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_013B04[t1]);
	}
	if (CAR(player_car)->unk_181 != 0u)
	{
		r0 = (r0 & ~0xFFFFu) | OBJECT(current_object)->unk_1C;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
		if ((int16_t)r0 < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
	}
	r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_28;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
	r1 = 0x80u;
	if ((int16_t)r0 < (int16_t)r1)
	{
		sub_01397E(r0, r1);
		return;
	}
	OBJECT(current_object)->unk_1C++;
	if (OBJECT(current_object)->unk_86 == 0u)
	{
		t2 = OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_84 = t2 - 1u;
		if (t2 == 1u)
			OBJECT(current_object)->unk_86 = 1u;
	}
	else
	{
		t3 = OBJECT(current_object)->unk_2C;
		OBJECT(current_object)->unk_2C = t3 - 1u;
		if ((int16_t)t3 <= 1)
			OBJECT(current_object)->unk_2C = 0u;
	}
	for (;;)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1);
		if ((int16_t)r0 < (int16_t)r1)
			break;
		OBJECT(current_object)->unk_1C++;
	}
	sub_01397E(r0, r1);
}

void sub_013B56(void)
{
	uint16_t r0 = 0;
	uint32_t r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r0 = (uint16_t)mem_read32(r10);
	r11 = current_object;
	r10 += 4u;
	for (;;)
	{
		r12 = mem_read32(r10);
		r10 += 4u;
		if (r12 == 0u)
			break;
		r21 = sub_011D43();
		mem_write16(r21 + 0x1Cu, mem_read16(r11 + 0x1Cu) - r0);
		mem_write32(r21 + 4u, sub_013AD8_ADDRESS);
		mem_write16(r21 + 0x84u, mem_read16(r11 + 0x84u));
		mem_write32(r21 + 0x18u, r12);
		mem_write32(r21 + 0x14u, r11);
		r11 = r21;
		SCREEN(screen)->unk_FA8++;
	}
	OBJECT(current_object)->unk_18 = r10;
	OBJECT(current_object)->unk_04 = sub_013BA3_ADDRESS;
	sub_013BA3();
}

void sub_013BA3(void)
{
	uint32_t zero = 0, negative = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0, race = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_18;
		r11 = mem_read32(r10);
		r9 = mem_read16(r11);
		r10 += 4u;
		r11 += 2u;
		if (r9 != 0x165u)
		{
			if (r9 == 0x258u || r9 == 0x185u)
				OBJECT(current_object)->unk_44 = 0u;
			else
				OBJECT(current_object)->unk_44 = 1u;
			OBJECT(current_object)->unk_3A = mem_read16(r11);
			OBJECT(current_object)->unk_3E = 5u;
			r11 += 2u;
			AT8(OBJECT(current_object)->unk_80, 1) = mem_read8(r11);
			r11++;
			AT8(OBJECT(current_object)->unk_80, 2) = mem_read8(r11);
			r11++;
			OBJECT(current_object)->unk_6A = mem_read16(r11);
			OBJECT(current_object)->unk_60 = r11 + 2u;
			OBJECT(current_object)->unk_3F = (uint8_t)mem_read16(r10);
			r10 += 2u;
			AS32(OBJECT(current_object)->unk_20) = (int16_t)mem_read16(r10);
			OBJECT(current_object)->unk_2C = mem_read16(r10 + 2u);
			OBJECT(current_object)->unk_0C++;
			sub_00F366(r9);
			return;
		}
		OBJECT(current_object)->unk_44 = 2u;
		OBJECT(current_object)->unk_3A = mem_read16(r11);
		OBJECT(current_object)->unk_3E = 4u;
		r11 += 2u;
		AT8(OBJECT(current_object)->unk_80, 1) = mem_read8(r11);
		r11++;
		AT8(OBJECT(current_object)->unk_80, 2) = mem_read8(r11);
		r11++;
		OBJECT(current_object)->unk_6A = mem_read16(r11);
		OBJECT(current_object)->unk_60 = r11 + 2u;
		OBJECT(current_object)->unk_3F = (uint8_t)mem_read16(r10);
		r10 += 2u;
		AS32(OBJECT(current_object)->unk_20) = (int16_t)mem_read16(r10);
		OBJECT(current_object)->unk_2C = mem_read16(r10 + 2u);
		AS32(OBJECT(current_object)->unk_24) = (int16_t)0xFD40u;
		OBJECT(current_object)->unk_0C++;
		sub_00F366(r9);
		return;
	case 1:
		if (CAR(player_car)->unk_181 != 0u && (int16_t)(OBJECT(current_object)->unk_1C - 8u) < (int16_t)RACE(SCREEN(screen)->race_copy)->unk_46)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = (uint16_t)((uint16_t)OBJECT(current_object)->unk_28 + OBJECT(current_object)->unk_2C);
		r1 = 0x80u;
		if ((int16_t)r0 >= 0x80)
		{
			do
			{
				OBJECT(current_object)->unk_1C++;
				if (OBJECT(current_object)->unk_86 == 0u)
				{
					t1 = OBJECT(current_object)->unk_84;
					OBJECT(current_object)->unk_84 = t1 - 1u;
					if (t1 == 1u)
						OBJECT(current_object)->unk_86 = 1u;
				}
				else
				{
					t2 = OBJECT(current_object)->unk_2C;
					OBJECT(current_object)->unk_2C = t2 - 1u;
					if ((int16_t)t2 <= 1)
						OBJECT(current_object)->unk_2C = 0u;
				}
				r0 -= r1;
			} while ((int16_t)r1 <= (int16_t)r0);
		}
		AS16(OBJECT(current_object)->unk_28) = r0;
		r0_2 = OBJECT(current_object)->unk_1C;
		zero = sub_013D29((uint16_t)r0_2);
		if (zero)
		{
			SCREEN(screen)->unk_FA8--;
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		race = SCREEN(screen)->race_copy;
		r1 = (uint16_t)(r0_2 - SCREEN(screen)->unk_FA6);
		if ((int16_t)r0_2 < (int16_t)RACE(race)->unk_46 || (int16_t)RACE(race)->unk_46 <= (int16_t)r1)
			return;
		if (mem_read32(OBJECT(current_object)->unk_60) != 0u)
		{
			negative = sub_01262A();
			if (negative)
				sub_013CF6();
		}
		sub_00EAE4(r0_2);
		ignored = sub_00F30D(0u);
		return;
	default:
		invalid_jump(rom_013BB0[OBJECT(current_object)->unk_0C]);
	}
}

void sub_013CF6(void)
{
	uint32_t r21 = 0;

	if ((uint8_t)(OBJECT(current_object)->unk_80 >> 16u) != 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_013D15_ADDRESS);
		mem_write8(r21 + 0x10u, 0xFu);
		mem_write8(r21 + 0x12u, (uint8_t)(OBJECT(current_object)->unk_80 >> 16u));
	}
}

void sub_013D15(void)
{
	uint8_t r0 = 0;
	uint8_t t1 = 0;

	t1 = (uint8_t)OBJECT(current_object)->unk_10;
	AS8(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		r0 = OBJECT(current_object)->unk_12;
		sub_011EB8(r0);
		return;
	}
}

uint32_t sub_013D29(uint16_t r0)
{
	uint32_t zero = 0;
	uint32_t r9 = 0, r10 = 0;
	uint16_t t1 = 0, t2 = 0;

	r10 = screen + 0xF84u;
	r9 = SCREEN(r10 - 0xF84u)->unk_F84;
	r10 += 2u;
	do
	{
		t1 = mem_read16(r10);
		r10 += 2u;
		if (t1 < (uint16_t)r0)
			break;
		t2 = mem_read16(r10);
		r10 += 2u;
		if (t2 < (uint16_t)r0)
		{
			zero = 1u == 0u;
			return zero;
		}
		r9--;
	} while (r9 != 0u);
	zero = 0u == 0u;
	return zero;
}

void sub_013DC4(void)
{
	OBJECT(current_object)->unk_04 = sub_013DE4_ADDRESS;
	OBJECT(current_object)->unk_80 = 0xFFFFFFFFu;
	sub_013DE4();
}

void sub_013DD5(void)
{
	OBJECT(current_object)->unk_04 = sub_013DE4_ADDRESS;
	OBJECT(current_object)->unk_80 = 1u;
	sub_013DE4();
}

void sub_013DE4(void)
{
	uint16_t r0 = 0, r9 = 0, r16 = 0, r17 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r13 = 0, r15 = 0, r18 = 0, r19 = 0, r21 = 0, r26 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r0 = OBJECT(current_object)->unk_1C;
		AT16(OBJECT(current_object)->unk_54, 2) = r0;
		OBJECT(current_object)->unk_58 = r0 + 0x28u;
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_013DF0[OBJECT(current_object)->unk_0C]);
	}
	r9 = RACE(SCREEN(screen)->race_copy)->unk_46;
	r16 = (uint16_t)(OBJECT(current_object)->unk_54 >> 16u);
	r17 = OBJECT(current_object)->unk_58;
	if (r17 <= r9)
	{
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	r19 = screen + 0x2FF0u;
	do
	{
		r19 += 0x10u;
	} while (mem_read16(r19) == 0u);
	if (mem_read16(r19) == 0u)
		return;
	r27 = r19;
	r27 = r27 & 0xFFFu;
	r27 = r27 >> 4;
	r27 = (uint16_t)(r27 + r9);
	r19 -= 0x10u;
	do
	{
		r19 += 0x10u;
		if (mem_read16(r19) == 0u)
			return;
		r26 = r19 & 0xFFFu;
		r26 = r26 >> 4;
		r26 = (uint16_t)(r26 + r9);
		t1 = r26 - r16;
		r26 = t1;
	} while ((int16_t)t1 < 0 || (int16_t)mem_read16(r19 + 0x16u) >= 2);
	r0_2 = (int16_t)mem_read16(r19 + 6u);
	r18 = 0u;
	if (r16 <= r27)
		r18 = 2u - r0_2;
	r2 = (int16_t)mem_read16(r19 + 4u);
	r4 = mem_read16(r19);
	if (r4 >= 0x1FF8u)
		r4 = 0x1FF8u;
	r26 = (uint16_t)(r26 << 3);
	r2 += ((int32_t)(((int32_t)(((int16_t)rom_101400[(uint16_t)(r26 + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * r4) >> 12) * OBJECT(current_object)->unk_80;
	sub_01405F(&r13, &r15);
	for (;;)
	{
		r19 += 0x10u;
		if (mem_read16(r19) == 0u)
			break;
		r1 = (int16_t)mem_read16(r19 + 6u);
		if ((int16_t)r0_2 <= (int16_t)r1)
			continue;
		r3 = (int16_t)mem_read16(r19 + 4u);
		r21 = mem_read16(r19 + 8u);
		r5 = mem_read16(r19);
		if (r5 >= 0x1FF8u)
			r5 = 0x1FF8u;
		r26 = r19 & 0xFFFu;
		r26 = r26 >> 4;
		r26 = (uint16_t)(r26 + r9);
		if (r17 < r26)
			break;
		r26 = (uint16_t)(r26 - r16);
		r26 = (uint16_t)(r26 << 3);
		r3 += ((int32_t)(((int32_t)(((int16_t)rom_101400[(uint16_t)(r26 + 0x800u)] + 0x4000u) * 0x5186u) >> 14) * r5) >> 12) * OBJECT(current_object)->unk_80;
		if ((int32_t)r3 > 0x280 || (int32_t)r3 < -0x280)
			break;
		r10 = SCREEN(screen)->unk_C0 + 0x10u;
		r8 = r0_2 - r1;
		r18 += r8;
		if ((int32_t)(r18 - 1u) < 0)
			continue;
		if (r18 == 1u)
		{
			if (SCREEN(screen)->unk_C4 <= r10)
				break;
			r18 = 0u;
			mem_write32(r10 + 0x3000u, 0u);
			mem_write32(r10, r12);
			r10 += 4u;
			mem_write16(r10, r13);
			r10 += 2u;
			mem_write16(r10, ((r4 * 0x370u) >> 12) | r14);
			r10 += 2u;
			mem_write16(r10, r1);
			r10 += 2u;
			mem_write16(r10, r3);
			r10 += 2u;
			mem_write32(r10, r15);
			r10 += 4u;
		}
		else
		{
			r6 = r8;
			r7 = 0u;
			r0 = (uint16_t)r1;
			do
			{
				if ((int16_t)r0 < 2)
				{
					if (SCREEN(screen)->unk_C4 <= r10)
						return;
					mem_write32(r10 + 0x3000u, 0u);
					mem_write32(r10, r12);
					r10 += 4u;
					mem_write16(r10, r13);
					r27 = r5;
					r27 = r27 * r8;
					r27 = r27 + r4 * r7;
					r27 = divide_s32(r27, r6);
					r27 = r27 * 0x370u;
					r27 = r27 >> 12;
					r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 | r14);
					r10 += 2u;
					mem_write16(r10, r27);
					r10 += 2u;
					mem_write16(r10, r0);
					r0++;
					r27 = r3;
					r27 = r27 * r8;
					r27 = r27 + r2 * r7;
					r27 = divide_s32(r27, r6);
					r10 += 2u;
					mem_write16(r10, r27);
					r10 += 2u;
					mem_write32(r10, r15);
					r10 += 4u;
				}
				r8--;
				r18--;
				r7++;
			} while (r18 != 0u);
		}
		r0_2 = r1;
		r2 = r3;
		r4 = r5;
		r27 = screen + 0x5000u + r21 * 4u;
		if (mem_read16(r27) == 0u)
			mem_write16(r27 + 2u, r10);
		r26 = SCREEN(screen)->unk_C0 + 0x10u;
		r28 = mem_read16(r27);
		mem_write32(r10, r28);
		r28 = (uint16_t)(r28 + 0x300u);
		mem_write32(r10 + 0x3000u, r28);
		SCREEN(screen)->unk_C0 = r10;
		mem_write16(r27, (r26 >> 4) | 0x8000u);
		SCREEN(screen)->unk_B0++;
	}
}

void sub_01405F(uint32_t *p_r13, uint32_t *p_r15)
{
	uint16_t r13 = 0;
	uint32_t r26 = 0;

	r26 = mem_read32(rom_034308 + 4u);
	r27 = (r27 & ~0xFFFFu) | mem_read16(r26 + 2u);
	r26 = 0x122B74u + mem_read16(r26 + 4u) * 4u;
	r12 = ((uint32_t)ROM16(r26) << 16) | (uint16_t)r27;
	r26 += 2u;
	r27 = (r27 & ~0xFFFFu) | ROM16(r26);
	r26 += 4u;
	r13 = 1u | ROM16(r26);
	r14 = ROM16(r26 + 4u);
	r14 = r14 | SCREEN(screen)->unk_1A;
	*p_r15 = ((uint32_t)mem_read16(rom_032BA4[rom_0140C8[SCREEN(screen)->unk_A3]]) << 16) | (uint16_t)r27;
	*p_r13 = (*p_r13 & 0xFFFF0000u) | r13;
}

void sub_014108(void)
{
	uint32_t r0 = 0, r9 = 0, object = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0u;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = rom_01440E[r0];
		OBJECT(current_object)->unk_3A = rom_01442E[r0];
		sub_00F366(r9);
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		if ((uint8_t)OBJECT(object)->unk_2E == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_32 = OBJECT(object)->unk_32 + SCREEN(screen)->unk_16;
		OBJECT(current_object)->unk_40 = 0u;
		OBJECT(current_object)->unk_41 = 1u;
		OBJECT(current_object)->unk_42 = 1u;
		break;
	case 1:
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		if ((uint8_t)OBJECT(object)->unk_2E == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_32 = OBJECT(object)->unk_32 + SCREEN(screen)->unk_16;
		OBJECT(current_object)->unk_40 = 0u;
		OBJECT(current_object)->unk_41 = 1u;
		OBJECT(current_object)->unk_42 = 1u;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_014114[OBJECT(current_object)->unk_0C]);
	}
	object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
	AS8(OBJECT(object)->unk_CC) = 0u;
	OBJECT(object)->unk_DC = 0u;
	if ((uint8_t)OBJECT(object)->unk_2E == 0u)
	{
		sub_00F30D(0u);
		if (OBJECT(current_object)->unk_41 == 0u)
		{
			object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
			OBJECT(current_object)->unk_0C--;
			AS8(OBJECT(object)->unk_2E) = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			return;
		}
		return;
	}
	OBJECT(current_object)->unk_3C = OBJECT(object)->unk_3C;
	r0 = OBJECT(object)->unk_34;
	if (r0 > 0x100u)
		r0 = 0x100u;
	OBJECT(current_object)->unk_34 = r0;
	OBJECT(current_object)->unk_36 = r0;
	OBJECT(current_object)->unk_30 = OBJECT(object)->unk_30 + SCREEN(screen)->unk_14;
	if (OBJECT(object)->unk_BD != 0u)
		OBJECT(current_object)->unk_32 = ((int32_t)((int16_t)mem_read16(OBJECT(object)->unk_80 + 0x56u) * r0) >> 8) + OBJECT(object)->unk_32 + SCREEN(screen)->unk_16;
	sub_00EAE4(r0);
	sub_00F30D(0u);
	if (OBJECT(current_object)->unk_41 == 0u)
	{
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_0C--;
		AS8(OBJECT(object)->unk_2E) = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		return;
	}
}

void sub_0141F4(void)
{
	uint32_t zero = 0;
	uint8_t r9_2 = 0;
	uint32_t r0 = 0, r9 = 0, object = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0u;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = rom_01444E[r0];
		if (r9 == 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		}
		OBJECT(current_object)->unk_3A = rom_01442E[r0];
		sub_00F366(r9);
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		r9_2 = 0xFFu;
		zero = sub_0142D2(r9_2, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_41 = 1u;
			OBJECT(current_object)->unk_42 = 1u;
			OBJECT(object)->unk_E1 = 0xFFu;
		}
		return;
	case 1:
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		r9_2 = 0xFFu;
		zero = sub_0142D2(r9_2, object);
		if (zero == 0u)
		{
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_41 = 1u;
			OBJECT(current_object)->unk_42 = 1u;
			OBJECT(object)->unk_E1 = 0xFFu;
		}
		return;
	case 2:
		object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
		zero = sub_0142B4(object);
		if (zero)
		{
			sub_00F30D(0u);
			if (OBJECT(current_object)->unk_41 == 0u)
			{
				object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
				OBJECT(current_object)->unk_0C--;
				OBJECT(object)->unk_E1 = 0u;
				return;
			}
			return;
		}
		OBJECT(current_object)->unk_3C = OBJECT(object)->unk_3C;
		OBJECT(current_object)->unk_3C++;
		r0 = OBJECT(object)->unk_34;
		if (r0 > 0x100u)
			r0 = 0x100u;
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		sub_0142F7((uint16_t)r0, object);
		sub_00EAE4(r0);
		sub_00F30D(0u);
		if (OBJECT(current_object)->unk_41 == 0u)
		{
			object = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_14;
			OBJECT(current_object)->unk_0C--;
			OBJECT(object)->unk_E1 = 0u;
			return;
		}
		return;
	default:
		invalid_jump(rom_014200[OBJECT(current_object)->unk_0C]);
	}
}

uint32_t sub_0142B4(uint32_t r10)
{
	uint32_t zero = 0;
	uint32_t r9 = 0;
	uint8_t t1 = 0;

	r9 = rom_0143A0[OBJECT(CAR(player_car)->object)->unk_0C];
	r9 = r9 & rom_0143A6[(uint8_t)(OBJECT(r10)->unk_E2 >> 8u)];
	if (r9 == 0u)
	{
		t1 = OBJECT(r10)->unk_E1 & r9;
		zero = t1 == 0u;
		OBJECT(r10)->unk_E1 = t1;
		return zero;
	}
	return sub_0142D2((uint8_t)r9, r10);
}

uint32_t sub_0142D2(uint8_t r9, uint32_t r10)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint8_t t1 = 0;

	r0 = mem_read8(screen + 0x320u + (uint8_t)(OBJECT(current_object)->unk_08 >> 24u));
	r1 = (uint8_t)OBJECT(r10)->unk_E2;
	AS8(OBJECT(r10)->unk_E2) = r0;
	if (r0 < r1)
		OBJECT(r10)->unk_E1 = 0xFFu;
	t1 = OBJECT(r10)->unk_E1 & r9;
	zero = t1 == 0u;
	OBJECT(r10)->unk_E1 = t1;
	return zero;
}

void sub_0142F7(uint16_t r0, uint32_t r10)
{
	uint16_t r6 = 0, r7 = 0, r9 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0, r11 = 0;

	r3 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	r6 = rom_014360[r3];
	r7 = rom_014380[r3];
	r1 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
	r2 = (uint8_t)(OBJECT(r10)->unk_E2 >> 8u);
	r3 = (uint8_t)(r3 - r1);
	r11 = rom_18D25C[r1] + (uint16_t)(r2 << 4) + (r3 >> 1);
	r9 = (uint16_t)(OBJECT(r10)->unk_32 + SCREEN(screen)->unk_16 + ((int16_t)((uint32_t)(r7 + mem_read16(r11 + 2u)) * r0) >> 8));
	OBJECT(current_object)->unk_30 = OBJECT(r10)->unk_30 + SCREEN(screen)->unk_14 + ((int16_t)((uint32_t)(r6 + mem_read16(r11)) * r0) >> 8);
	OBJECT(current_object)->unk_32 = r9;
}

void sub_014470(void)
{
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
	case 1:
		break;
	case 2:
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_01447C[OBJECT(current_object)->unk_0C]);
	}
	switch (OBJECT(current_object)->unk_0D)
	{
	case 0:
		sub_0144B5();
		break;
	case 1:
		sub_0144C9();
		break;
	case 2:
		sub_0144EB();
		break;
	default:
		invalid_jump(rom_0144A9[OBJECT(current_object)->unk_0D]);
	}
	switch (OBJECT(current_object)->unk_0E)
	{
	case 0:
		OBJECT(current_object)->unk_0E++;
		return;
	case 1:
		CAR(player_car)->unk_10C += 0x12u;
		t1 = CAR(player_car)->unk_10E;
		CAR(player_car)->unk_10E = t1 - 3u;
		if (t1 != 3u)
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_0E++;
		return;
	case 2:
		return;
	default:
		invalid_jump(rom_0144EC[OBJECT(current_object)->unk_0E]);
	}
}

void sub_0144B5(void)
{
	OBJECT(current_object)->unk_0D++;
	OBJECT(current_object)->unk_12 = rom_014660;
	OBJECT(current_object)->unk_18 = 0x14661u;
}

void sub_0144C9(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	AS8(OBJECT(current_object)->unk_10)++;
	if ((uint8_t)OBJECT(current_object)->unk_10 < OBJECT(current_object)->unk_12)
	{
		sub_0144EB();
		return;
	}
	r10 = OBJECT(current_object)->unk_18;
	r0 = mem_read8(r10);
	r10++;
	sub_011EB8(r0);
	if (mem_read8(r10) != 0u)
	{
		OBJECT(current_object)->unk_12 = mem_read8(r10);
		OBJECT(current_object)->unk_18 = r10 + 1u;
		return;
	}
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D++;
	sub_0144EB();
}

void sub_0144EB(void)
{
}

void sub_014514(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		AT8(OBJECT(current_object)->unk_08, 1) = RACE(SCREEN(screen)->race_copy)->unk_60;
		r10 = 0x1466Fu;
		AS8(OBJECT(current_object)->unk_10) = mem_read8(r10);
		OBJECT(current_object)->unk_18 = r10 + 1u;
		return;
	case 1:
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != RACE(SCREEN(screen)->race_copy)->unk_60)
		{
			OBJECT(current_object)->unk_0C++;
			return;
		}
		t1 = (uint8_t)OBJECT(current_object)->unk_10;
		AS8(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		r10 = OBJECT(current_object)->unk_18;
		r0 = mem_read8(r10);
		r10++;
		sub_011EB8(r0);
		if (mem_read8(r10) != 0u)
		{
			AS8(OBJECT(current_object)->unk_10) = mem_read8(r10);
			OBJECT(current_object)->unk_18 = r10 + 1u;
			return;
		}
		OBJECT(current_object)->unk_0C++;
		return;
	case 2:
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == RACE(SCREEN(screen)->race_copy)->unk_60)
			return;
		r0 = 0x24u;
		if (SCREEN(screen)->unk_01 != SCREEN(screen)->unk_70)
			r0 = 0x28u;
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		sub_011EB8(r0);
		return;
	default:
		invalid_jump(rom_014520[OBJECT(current_object)->unk_0C]);
	}
}

void sub_01458D(void)
{
	uint16_t r0_2 = 0;
	uint32_t r0 = 0;
	uint16_t t1 = 0, t2 = 0;

	if (CAR(player_car)->unk_137 != 0u)
		return;
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		if (RACE(SCREEN(screen)->race_copy)->unk_42 < OBJECT(current_object)->unk_1C)
			return;
		OBJECT(current_object)->unk_0C++;
		r0 = RACE(SCREEN(screen)->race_copy)->unk_49;
		OBJECT(current_object)->unk_84 = 0u - rom_010BD2[r0] + 0x1E0u;
		AS16(OBJECT(current_object)->unk_80) = rom_014674[r0];
		AT16(OBJECT(current_object)->unk_80, 2) = rom_014684[r0];
		AS16(OBJECT(current_object)->unk_86) = OBJECT(current_object)->unk_1C + 0xE0u;
		CAR(player_car)->unk_10A = 0u;
		CAR(player_car)->unk_108 = 0u;
		return;
	case 1:
		r0_2 = (uint16_t)(CAR(player_car)->unk_10A + (uint16_t)OBJECT(current_object)->unk_80);
		if ((int16_t)OBJECT(current_object)->unk_84 <= (int16_t)r0_2)
		{
			OBJECT(current_object)->unk_0C++;
			r0_2 = OBJECT(current_object)->unk_84;
			SCREEN(screen)->unk_F0F = 0xFFu;
		}
		CAR(player_car)->unk_10A = r0_2;
		CAR(player_car)->unk_108 -= (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		return;
	case 2:
		if (RACE(SCREEN(screen)->race_copy)->unk_42 < AS16(OBJECT(current_object)->unk_86))
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_0145A0[OBJECT(current_object)->unk_0C]);
	}
	t1 = (uint16_t)OBJECT(current_object)->unk_80;
	t2 = CAR(player_car)->unk_10A;
	r0_2 = (uint16_t)(t2 - t1);
	if ((int16_t)t1 < (int16_t)t2)
	{
		CAR(player_car)->unk_10A = r0_2;
		CAR(player_car)->unk_108 += (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		return;
	}
	OBJECT(current_object)->unk_00 = 0u;
	OBJECT(current_object)->unk_01 = 0u;
	CAR(player_car)->unk_10A = 0u;
	CAR(player_car)->unk_108 = 0u;
}

void sub_014694(void)
{
	bool L_0146F4 = false;
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0, r21 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		r0 = (uint8_t)(r0 * 2u);
		OBJECT(current_object)->unk_30 += 0xA0u;
		OBJECT(current_object)->unk_32 += 0xDEu;
		r1 = SCREEN(screen)->unk_01;
		for (;;)
		{
			if (L_0146F4 || RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
			{
				if (!L_0146F4)
				{
					r0 = object_list_14A60;
					r10 = 0x14A64u;
				}
				do
				{
					r21 = sub_011D43();
					mem_write32(r21 + 4u, mem_read32(r10));
					r10 += 4u;
					mem_write32(r21 + 0x80u, mem_read32(r10));
					mem_write32(r21 + 0x14u, current_object);
					r10 += 4u;
					r0--;
				} while (r0 != 0u);
				return;
			}
			if (mem_read8(screen + 0x320u + r1) != 0u)
				r0 = (uint8_t)(r0 + 1u);
			r10 = object_lists_14A20[r0];
			r0 = mem_read32(r10);
			r10 += 4u;
			L_0146F4 = true;
		}
	case 1:
	case 3:
		break;
	case 2:
		OBJECT(current_object)->unk_0C++;
		r0_2 = 0x4Fu;
		sub_011EB8(r0_2);
		SCREEN(screen)->unk_94 |= 2u;
		SCREEN(screen)->unk_94 &= 0xFFFFFFEFu;
		if (RACE(SCREEN(screen)->race_copy)->unk_5E == 0u)
		{
			sub_011FE6();
			return;
		}
		break;
	default:
		invalid_jump(rom_0146A0[OBJECT(current_object)->unk_0C]);
	}
}

void sub_014735(void)
{
	uint32_t r9 = 0, r10 = 0;

	r10 = OBJECT(current_object)->unk_14;
	r9 = RACE(SCREEN(screen)->race_copy)->unk_3C;
	OBJECT(current_object)->unk_38 = r9;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = RACE(SCREEN(screen)->race_copy)->unk_3E;
	OBJECT(current_object)->unk_40 = RACE(SCREEN(screen)->race_copy)->unk_4A;
	OBJECT(current_object)->unk_3C = mem_read16(r10 + 0x3Cu);
	OBJECT(current_object)->unk_30 = mem_read16(r10 + 0x30u);
	OBJECT(current_object)->unk_32 = mem_read16(r10 + 0x32u);
	OBJECT(current_object)->unk_34 = mem_read16(r10 + 0x34u);
	OBJECT(current_object)->unk_36 = mem_read16(r10 + 0x34u);
	sub_014838();
}

void sub_014777(void)
{
	uint32_t r10 = 0;

	r10 = OBJECT(current_object)->unk_14;
	r12 = r10 + 0x90u;
	sub_01478B(r10);
}

void sub_014782(void)
{
	uint32_t r10 = 0;

	r10 = OBJECT(current_object)->unk_14;
	r12 = r10 + 0xA0u;
	sub_01478B(r10);
}

void sub_01478B(uint32_t r10)
{
	uint32_t carry = 0;
	uint32_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r11 = 0, race = 0;

	OBJECT(current_object)->unk_3A = mem_read16(r12 + 6u);
	OBJECT(current_object)->unk_3C = mem_read16(r10 + 0x3Cu);
	race = SCREEN(screen)->race_copy;
	r11 = mem_read32(rom_034138[RACE(race)->unk_3C] + 4u + RACE(race)->unk_4A * 4u) + 2u;
	r3 = mem_read16(r11);
	r11 += 2u;
	r3 = r3 & 0x40u;
	r3 = r3 >> 6;
	r3 = (uint16_t)(r3 * 0x34u);
	r3 = (uint16_t)(r3 + (uint16_t)(mem_read16(r11) - mem_read16(r10 + 0x8Cu)) / 3u);
	r11 += 2u;
	r6 = mem_read16(r11);
	r5 = mem_read16(r11 + 2u);
	r11 = mem_read32(r12) + (r3 << 4);
	r5 = (uint16_t)(r5 + mem_read16(r11));
	r6 = (uint16_t)(r6 + mem_read16(r11 + 2u));
	r9 = mem_read16(r12 + 4u) + r3;
	OBJECT(current_object)->unk_38 = r9;
	sub_00F366(r9);
	OBJECT(current_object)->unk_40 = mem_read8(r12 + 0xAu);
	r7 = mem_read16(r10 + 0x34u);
	OBJECT(current_object)->unk_34 = r7;
	OBJECT(current_object)->unk_36 = r7;
	r5 = (int16_t)r5 * r7;
	r6 = (int16_t)r6 * r7;
	carry = (r5 >> 7) & 1u;
	r5 = (uint16_t)(((int32_t)r5 >> 8) + carry);
	carry = (r6 >> 7) & 1u;
	r6 = (uint16_t)(((int32_t)r6 >> 8) + carry + mem_read16(r10 + 0x32u));
	OBJECT(current_object)->unk_30 = r5 + mem_read16(r10 + 0x30u);
	OBJECT(current_object)->unk_32 = r6;
	sub_014838();
}

void sub_014838(void)
{
	OBJECT(current_object)->unk_04 = sub_014846_ADDRESS;
	OBJECT(current_object)->unk_3E = 0u;
	sub_014846();
}

void sub_014846(void)
{
	uint32_t carry = 0;
	uint8_t r9_2 = 0;
	uint16_t r5 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0;
	uint32_t t3 = 0, t4 = 0, t5 = 0;

	r10 = OBJECT(current_object)->unk_80;
	for (;;)
	{
		r5 = mem_read16(r10);
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		r10 += 2u;
		if (carry)
		{
			OBJECT(current_object)->unk_10++;
			t1 = mem_read16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->unk_10 <= t1)
			{
				r0 = sub_00EAE4(r0);
				return;
			}
			OBJECT(current_object)->unk_10 = 0u;
			OBJECT(current_object)->unk_80 = r10;
			continue;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_10++;
			t2 = mem_read16(r10);
			r10 += 2u;
			if (OBJECT(current_object)->unk_10 <= t2)
			{
				r10 = AS32(OBJECT(current_object)->unk_84);
				OBJECT(current_object)->unk_80 = r10;
				continue;
			}
			OBJECT(current_object)->unk_10 = 0u;
			OBJECT(current_object)->unk_80 = r10;
			continue;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
			ignored = sub_00F30D(ignored);
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_40 = (uint8_t)mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_30 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_30 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_32 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_32 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_34 += mem_read16(r10);
			OBJECT(current_object)->unk_36 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_34 = mem_read16(r10);
			OBJECT(current_object)->unk_36 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_3C += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_3C = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			r9 = mem_read16(r10);
			OBJECT(current_object)->unk_38 = r9;
			r9_2 = mem_read8(rom_034138[r9]);
			OBJECT(current_object)->unk_41 = r9_2;
			OBJECT(current_object)->unk_42 = r9_2;
			OBJECT(current_object)->unk_40 = 0u;
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(current_object)->unk_3A = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			r0 = mem_read16(r10);
			r10 += 2u;
			sub_011EB8((uint8_t)r0);
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		if (carry == 0u)
		{
			t3 = r10;
			r10 = OBJECT(current_object)->unk_80;
			OBJECT(current_object)->unk_80 = t3;
			AS32(OBJECT(current_object)->unk_84) = r10;
			r0 = sub_00EAE4(r0);
			return;
		}
		t4 = mem_read16(r10);
		r0 = rom_014994[t4];
		r10 += 2u;
		switch (t4)
		{
		case 0:
			mem_write8(OBJECT(current_object)->unk_14 + 0xCu, 2u);
			t3 = r10;
			r10 = OBJECT(current_object)->unk_80;
			OBJECT(current_object)->unk_80 = t3;
			AS32(OBJECT(current_object)->unk_84) = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 1:
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			return;
		case 2:
			t5 = r10;
			r10 = OBJECT(current_object)->unk_80;
			OBJECT(current_object)->unk_80 = t5;
			AS32(OBJECT(current_object)->unk_84) = r10;
			return;
		case 3:
			OBJECT(current_object)->unk_00 ^= 0x40u;
			t3 = r10;
			r10 = OBJECT(current_object)->unk_80;
			OBJECT(current_object)->unk_80 = t3;
			AS32(OBJECT(current_object)->unk_84) = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 4:
			AS16(CAR(player_car)->unk_34) += 0x4000u;
			t3 = r10;
			r10 = OBJECT(current_object)->unk_80;
			OBJECT(current_object)->unk_80 = t3;
			AS32(OBJECT(current_object)->unk_84) = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 5:
			break;
		default:
			invalid_jump(rom_014994[t4]);
		}
		OBJECT(current_object)->unk_10++;
		r0 = (uint16_t)(mem_read16(r10) + ((uint32_t)rom_014A16[mem_read8(screen + 0x320u + SCREEN(screen)->unk_01)] << 3));
		r10 += 2u;
		if (OBJECT(current_object)->unk_10 <= r0)
		{
			r10 = AS32(OBJECT(current_object)->unk_84);
			OBJECT(current_object)->unk_80 = r10;
			continue;
		}
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_80 = r10;
	}
}

void sub_016B00(void)
{
	uint32_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r13 = 0, r15 = 0, r16 = 0, r18 = 0, r19 = 0, r21 = 0, ignored = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_016C23();
		AS8(OBJECT(current_object)->unk_80) = 0x1Eu;
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		sub_016D00(&ignored, &ignored, &ignored, &ignored, &ignored, &ignored, &ignored);
		OBJECT(current_object)->unk_0F ^= 0x60u;
		OBJECT(current_object)->unk_0C++;
		if (OBJECT(current_object)->unk_0E == 0u)
		{
			r19 = SCREEN(screen)->unk_208;
			sub_0184D0(r19);
			OBJECT(current_object)->unk_0C++;
		}
		break;
	case 2:
		sub_016D00(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		sub_016E8C(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(current_object)->unk_0E == 0u)
		{
			r19 = SCREEN(screen)->unk_208;
			sub_0184D0(r19);
			OBJECT(current_object)->unk_0C++;
		}
		break;
	case 3:
		sub_016D00(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		sub_016E8C(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(current_object)->unk_0E != 0u)
		{
			OBJECT(current_object)->unk_0D = OBJECT(current_object)->unk_0E;
			r19 = SCREEN(screen)->unk_208;
			sub_0184EC(r19);
			OBJECT(current_object)->unk_0C--;
		}
		break;
	default:
		invalid_jump(rom_016B0C[OBJECT(current_object)->unk_0C]);
	}
	r0 = OBJECT(current_object)->unk_10;
	r0 = (uint16_t)(r0 + 0x4FFu);
	mem_write32(OBJECT(current_object)->unk_54, mem_read32(current_object - 0x24u + ((r0 >> 9) + OBJECT(current_object)->unk_0D * 5u) * 4u));
	if ((uint16_t)OBJECT(current_object)->unk_14 <= (uint16_t)(OBJECT(current_object)->unk_14 >> 16u))
		return;
	AT16(OBJECT(current_object)->unk_14, 2) = (uint16_t)OBJECT(current_object)->unk_14;
	if ((uint8_t)OBJECT(current_object)->unk_80 != 0u)
	{
		OBJECT(current_object)->unk_13 = OBJECT(current_object)->unk_12;
		AS8(OBJECT(current_object)->unk_80)--;
		return;
	}
	r0 = OBJECT(current_object)->unk_12;
	if (OBJECT(current_object)->unk_13 == r0)
		return;
	OBJECT(current_object)->unk_13 = r0;
	switch (r0)
	{
	case 4:
		SCREEN(screen)->unk_203 = 0xFFu;
		SCREEN(screen)->unk_204 = 0u;
		return;
	case 5:
		SCREEN(screen)->unk_203 = 0xFFu;
		SCREEN(screen)->unk_204 = 1u;
		return;
	case 0:
		return;
	case 1:
		t1 = ~(uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		AT8(OBJECT(current_object)->unk_08, 2) = t1;
		if (t1 == 0u)
			return;
		sub_017188();
		return;
	case 3:
		t2 = ~(uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		AT8(OBJECT(current_object)->unk_08, 2) = t2;
		if (t2 != 0u)
			sub_017188();
		t3 = ~(uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
		AT8(OBJECT(current_object)->unk_08, 3) = t3;
		if (t3 == 0u)
			return;
		r21 = sub_011D43();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 0x3Cu, current_object + 0xBu);
		mem_write32(r21 + 0x38u, 0x174B4u);
		mem_write32(r21 + 4u, sub_0171CE_ADDRESS);
		return;
	case 2:
		t3 = ~(uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
		AT8(OBJECT(current_object)->unk_08, 3) = t3;
		if (t3 == 0u)
			return;
		r21 = sub_011D43();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 0x3Cu, current_object + 0xBu);
		mem_write32(r21 + 0x38u, 0x174B4u);
		mem_write32(r21 + 4u, sub_0171CE_ADDRESS);
		return;
	default:
		invalid_jump(rom_016BBC[r0]);
	}
}

void sub_016C23(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r10 = 0, r11 = 0, r19 = 0;

	r10 = 0x1710Cu + ((uint32_t)SCREEN(screen)->unk_00 << 3) * 4u;
	r0 = unk_20E204 & ROM16(r10);
	r10 += 2u;
	unk_20E204 = r0 | ROM16(r10);
	r10 += 2u;
	mem_write16(ROM32(r10), 0x160u);
	r10 += 4u;
	mem_write16(ROM32(r10), 0u);
	unk_20E027 = 0xFFu;
	r10 += 4u;
	mem_write8(ROM32(r10), 0u);
	r10 += 4u;
	OBJECT(current_object)->unk_54 = ROM32(r10);
	r10 += 4u;
	r11 = ROM32(r10);
	r0_2 = mem_read32(r11);
	r0_2 = (r0_2 & ~0xFFFFu) | (r0_2 & 0xFFF0u);
	mem_write32(r11, (r0_2 & ~0xFFu) | (uint8_t)(r0_2 | 8u));
	SCREEN(screen)->unk_94 |= 4u;
	r10 += 4u;
	AS32(OBJECT(current_object)->unk_58) = ROM32(r10);
	r10 = ROM32(r10 + 4u);
	OBJECT(current_object)->unk_5C = r10;
	move_halfwords(r10 - 0x1C0u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x640u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0xE40u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x1640u, 0x100800u, 0xE0u);
	OBJECT(current_object)->unk_0D = rom_03CC80[SCREEN(screen)->unk_A0];
	OBJECT(current_object)->unk_0F = 0u;
	OBJECT(current_object)->unk_13 = 0u;
	AT16(OBJECT(current_object)->unk_14, 2) = 0u;
	AT8(OBJECT(current_object)->unk_08, 2) = 0u;
	AT8(OBJECT(current_object)->unk_08, 3) = 0u;
	move_string(0x1714Cu, 0x1Eu, current_object + 0x18u, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	r19 = rom_031C38[SCREEN(screen)->unk_A0];
	SCREEN(screen)->unk_208 = mem_read32(r19);
	r19 += 4u;
	sub_0115C5(r19);
}

void sub_016D00(uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r16, uint32_t *p_r18)
{
	uint32_t r4 = *p_r4;
	uint32_t r5 = *p_r5;
	uint32_t r6 = *p_r6;
	bool L_016E09 = false, L_016E73 = false, leave_loop = false, leave_loop_2 = false, leave_loop_3 = false, leave_loop_4 = false, leave_loop_5 = false, next_round = false;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t r0_2 = 0, r18 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r19 = 0, r26 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint16_t t1 = 0, t3 = 0;
	uint32_t t2 = 0, t4 = 0, t5 = 0;

	r10 = screen + 0x3000u;
	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 8u);
	r0 = (r0 & 0xCu) << 7;
	r11 = AS32(OBJECT(current_object)->unk_58) + r0 * 4u;
	stack_4 = r11;
	r12 = OBJECT(current_object)->unk_5C + r0 * 4u;
	r13 = RACE(SCREEN(screen)->race_copy)->unk_46;
	r14 = r14 & ~0xFFFFu;
	r15 = 0xFFFFFFF4u;
	r16 = 0x800u;
	r17 = (int16_t)SCREEN(screen)->unk_16;
	r18 = 0xFFu;
	r19 = 1u;
	r26 = screen + 0x3800u + OBJECT(current_object)->unk_0F * 2u * 4u;
	r0 = 0xE0u;
	while (mem_read16(r10) == 0u)
	{
		r10 += 0x10u;
		r13++;
	}
	OBJECT(current_object)->unk_0E = mem_read8(r10 + 0xCu);
	OBJECT(current_object)->unk_12 = mem_read8(r10 + 0xDu);
	AS16(OBJECT(current_object)->unk_14) = r13;
	mem_write16(r26, r13);
	r13 = 0u - ((r13 & ~0xFFFFu) | (r13 & 3u));
	overflow = v60_overflow_add32(r13, 2u, 0u);
	zero = r13 + 2u == 0u;
	negative = (r13 + 2u) >> 31;
	r13 += 2u;
	r26 += 2u;
	if ((negative ^ overflow) | zero)
	{
		r13 += 2u;
		r14 = r14 | 0x100u;
	}
	for (;;)
	{
		t1 = mem_read16(r10);
		r10 += 2u;
		if (t1 == 0u)
			break;
		r3 = mem_read16(r10) >> 2;
		mem_write16(r26, r3);
		r26 += 2u;
		r10 += 2u;
		r2 = 0u - (int16_t)mem_read16(r10);
		mem_write16(r26, r2);
		r26 += 2u;
		r10 += 2u;
		r1 = (int16_t)mem_read16(r10) + r17;
		mem_write16(r26, r1);
		r26 += 2u;
		r10 += 0xAu;
		next_round = false;
		for (;;)
		{
			leave_loop = false;
			do
			{
				leave_loop_2 = false;
				do
				{
					leave_loop_3 = false;
					do
					{
						if (L_016E09 || L_016E73 || L_016E09 || L_016E09 || r19 == 0u)
						{
							if (!L_016E09)
							{
								if (!L_016E73)
								{
									if (!L_016E09)
									{
										if (!L_016E09)
										{
											t2 = r0;
											r9 = t2 - r1;
											if ((int32_t)t2 <= (int32_t)r1)
											{
												r4 = r1;
												r5 = r2;
												r6 = r3;
												r13--;
												if (r13 != 0u)
												{
													next_round = true;
													leave_loop = true;
													leave_loop_2 = true;
													leave_loop_3 = true;
													break;
												}
												r13 = 2u;
												r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
												next_round = true;
												leave_loop = true;
												leave_loop_2 = true;
												leave_loop_3 = true;
												break;
											}
										}
									}
								}
							}
							leave_loop_4 = false;
							for (;;)
							{
								leave_loop_5 = false;
								do
								{
									if (L_016E73 || L_016E09 || L_016E09 || L_016E73 || L_016E09 || L_016E09 || r9 == 1u)
									{
										if (!L_016E73)
										{
											if (L_016E09 || L_016E09 || !L_016E73)
											{
												L_016E09 = false;
												r11 -= 2u;
												mem_write16(r11, r2);
												r0_2 = (uint16_t)r3;
												if ((int16_t)r18 < (int16_t)r0_2)
													r0_2 = (uint16_t)r18;
												r12 = r12 - 2u;
												mem_write16(r12, r0_2 + r14);
												r0 = r1;
											}
										}
										if (L_016E73 || L_016E73 || r0 == 0u)
										{
											for (;;)
											{
												t3 = mem_read16(r10);
												r10 += 2u;
												if (t3 == 0u)
													break;
												mem_write16(r26, mem_read16(r10) >> 2);
												r26 += 2u;
												r10 += 2u;
												mem_write16(r26, 0u - mem_read16(r10));
												r26 += 2u;
												r10 += 2u;
												r1 = (int16_t)mem_read16(r10) + r17;
												mem_write16(r26, r1);
												r26 += 2u;
												r10 += 0xAu;
											}
											SCREEN(screen)->unk_200 = r1;
											mem_write16(r26, 0xFFFFu);
											sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
											OBJECT(current_object)->unk_10 = mem_read16(stack_4 - 2u);
											*p_r4 = r4;
											*p_r5 = r5;
											*p_r6 = r6;
											*p_r13 = r13;
											*p_r15 = r15;
											*p_r16 = r16;
											*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
											return;
										}
										r4 = r1;
										r5 = r2;
										r6 = r3;
										r13--;
										if (r13 != 0u)
										{
											next_round = true;
											leave_loop = true;
											leave_loop_2 = true;
											leave_loop_3 = true;
											leave_loop_4 = true;
											leave_loop_5 = true;
											break;
										}
										r13 = 2u;
										r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
										next_round = true;
										leave_loop = true;
										leave_loop_2 = true;
										leave_loop_3 = true;
										leave_loop_4 = true;
										leave_loop_5 = true;
										break;
									}
									sub_016FEB(r0, r1, r2, r3, (uint8_t)r15, r16, r18, &r4, &r5, &r6, &ignored, &ignored, &r11, &zero);
									L_016E73 = zero == 0u;
								} while (L_016E73);
								if (leave_loop_5)
									break;
								L_016E09 = true;
							}
							if (leave_loop_4)
								break;
						}
						t4 = r0;
						r9 = t4 - r1;
						if ((int32_t)t4 <= (int32_t)r1)
						{
							r19 = 0u;
							OBJECT(current_object)->unk_60 = 0u;
							r4 = r1;
							r5 = r2;
							r6 = r3;
							r13--;
							if (r13 != 0u)
							{
								next_round = true;
								leave_loop = true;
								leave_loop_2 = true;
								leave_loop_3 = true;
								break;
							}
							r13 = 2u;
							r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
							next_round = true;
							leave_loop = true;
							leave_loop_2 = true;
							leave_loop_3 = true;
							break;
						}
						if ((int32_t)r19 < 0)
						{
							L_016E09 = r9 == 1u;
							if (!L_016E09)
							{
								OBJECT(current_object)->unk_64 = r5;
								AS32(OBJECT(current_object)->unk_68) = r6;
								sub_016FEB(r0, r1, r2, r3, (uint8_t)r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
								OBJECT(current_object)->unk_6C = r7;
								OBJECT(current_object)->unk_70 = r8;
								r19 = 0u;
								L_016E73 = zero == 0u;
								if (!L_016E73)
									L_016E09 = true;
							}
						}
					} while (L_016E09);
					if (leave_loop_3)
						break;
				} while (L_016E73);
				if (leave_loop_2)
					break;
			} while (L_016E09);
			if (leave_loop)
				break;
			r9 = (r9 - 1u) * 2u;
			r11 -= r9;
			r12 = r12 - r9;
			OBJECT(current_object)->unk_60 = r1;
			OBJECT(current_object)->unk_74 = r11;
			OBJECT(current_object)->unk_78 = r12;
			OBJECT(current_object)->unk_7C = r14;
			r19 = 0xFFFFFFFFu;
			L_016E09 = true;
		}
		if (next_round)
			continue;
	}
	t5 = shift_by(r0, r18, 32);
	carry = v60_shl_cy(r0, r18, 32);
	zero = t5 == 0u;
	r0 = t5;
	if (carry == 0u)
	{
		r0++;
		for (;;)
		{
			r12 = r12 - 4u;
			r0--;
			if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		SCREEN(screen)->unk_200 = r1;
		mem_write16(r26, 0xFFFFu);
		sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
		OBJECT(current_object)->unk_10 = mem_read16(stack_4 - 2u);
		*p_r4 = r4;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r16 = r16;
		*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		SCREEN(screen)->unk_200 = r1;
		mem_write16(r26, 0xFFFFu);
		sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
		OBJECT(current_object)->unk_10 = mem_read16(stack_4 - 2u);
		*p_r4 = r4;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r16 = r16;
		*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
		return;
	}
	r0++;
	for (;;)
	{
		r12 = r12 - 4u;
		r0--;
		if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	SCREEN(screen)->unk_200 = r1;
	mem_write16(r26, 0xFFFFu);
	sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
	OBJECT(current_object)->unk_10 = mem_read16(stack_4 - 2u);
	*p_r4 = r4;
	*p_r5 = r5;
	*p_r6 = r6;
	*p_r13 = r13;
	*p_r15 = r15;
	*p_r16 = r16;
	*p_r18 = (*p_r18 & 0xFFFF0000u) | r18;
}

void sub_016E8C(uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r13, uint8_t r15, uint32_t r16, uint16_t r18)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint16_t r0_2 = 0, r26 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r11 = 0, r13_2 = 0, r19 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint32_t t5 = 0, t6 = 0, t7 = 0;

	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 4u);
	r0 = (r0 & 0xCu) << 7;
	r11 = AS32(OBJECT(current_object)->unk_58) + r0 * 4u;
	r12 = OBJECT(current_object)->unk_5C + r0 * 4u;
	r14 = r14 & ~0xFFFFu;
	r19 = 1u;
	r0 = 0xE0u;
	r27 = OBJECT(current_object)->unk_0F;
	r28 = r27;
	r28 = r28 ^ 0x60u;
	OBJECT(current_object)->unk_0F = r28;
	r27 = r27 * 2u;
	r28 = r28 * 2u;
	r27 = screen + 0x3800u + r27 * 4u;
	r28 = screen + 0x3800u + r28 * 4u;
	r26 = mem_read16(r27);
	t1 = mem_read16(r28);
	r28 = r28 + 2u;
	r13_2 = (r13 & ~0xFFFFu) | t1;
	r27 = r27 + 2u;
	if ((uint16_t)r13_2 != r26)
	{
		if ((uint16_t)r13_2 < r26)
		{
			do
			{
				r28 = r28 + 6u;
				r13_2 = (r13_2 & ~0xFFFFu) | (uint16_t)(r13_2 + 1u);
			} while ((uint16_t)r13_2 < r26);
		}
		else
		{
			do
			{
				r27 = r27 + 6u;
				r26++;
			} while (r26 < (uint16_t)r13_2);
		}
	}
	r13_2 = 0u - ((r13_2 & ~0xFFFFu) | (r13_2 & 3u));
	overflow = v60_overflow_add32(r13_2, 2u, 0u);
	zero = r13_2 + 2u == 0u;
	negative = (r13_2 + 2u) >> 31;
	r13_2 += 2u;
	if ((negative ^ overflow) | zero)
	{
		r13_2 += 2u;
		r14 = r14 | 0x100u;
	}
	while ((int16_t)(mem_read16(r27) | mem_read16(r28)) >= 0)
	{
		t2 = mem_read16(r28);
		r28 = r28 + 2u;
		r3 = shift_by((uint32_t)mem_read16(r27) + t2, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t3 = mem_read16(r28);
		r28 = r28 + 2u;
		r2 = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)t3, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t4 = mem_read16(r28);
		r28 = r28 + 2u;
		r1 = shift_signed_by((uint32_t)(int16_t)mem_read16(r27) + (int16_t)t4, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		if (r19 == 0u)
		{
			t5 = r0;
			r9 = t5 - r1;
			if ((int32_t)r1 < (int32_t)t5)
			{
				if (r9 != 1u)
				{
					sub_016FEB(r0, r1, r2, r3, r15, r16, r18, &r4, &r5, &r6, &ignored, &ignored, &r11, &zero);
					if (zero == 0u)
					{
						sub_0170A6(r15, r16, r18, &r5, &r6);
						return;
					}
				}
				r11 -= 2u;
				mem_write16(r11, r2);
				r0_2 = (uint16_t)r3;
				if ((int16_t)r18 < (int16_t)r0_2)
					r0_2 = (uint16_t)r18;
				r12 = r12 - 2u;
				mem_write16(r12, r0_2 + r14);
				r0 = r1;
				if (r0 == 0u)
				{
					sub_0170A6(r15, r16, r18, &r5, &r6);
					return;
				}
			}
		}
		else
		{
			t6 = r0;
			r9 = t6 - r1;
			if ((int32_t)r1 < (int32_t)t6)
			{
				if ((int32_t)r19 < 0)
				{
					if (r9 == 1u)
					{
						r11 -= 2u;
						mem_write16(r11, r2);
						r0_2 = (uint16_t)r3;
						if ((int16_t)r18 < (int16_t)r0_2)
							r0_2 = (uint16_t)r18;
						r12 = r12 - 2u;
						mem_write16(r12, r0_2 + r14);
						r0 = r1;
						if (r0 == 0u)
						{
							sub_0170A6(r15, r16, r18, &r5, &r6);
							return;
						}
					}
					else
					{
						OBJECT(current_object)->unk_64 = r5;
						AS32(OBJECT(current_object)->unk_68) = r6;
						sub_016FEB(r0, r1, r2, r3, r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
						OBJECT(current_object)->unk_6C = r7;
						OBJECT(current_object)->unk_70 = r8;
						r19 = 0u;
						if (zero == 0u)
						{
							sub_0170A6(r15, r16, r18, &r5, &r6);
							return;
						}
						r11 -= 2u;
						mem_write16(r11, r2);
						r0_2 = (uint16_t)r3;
						if ((int16_t)r18 < (int16_t)r0_2)
							r0_2 = (uint16_t)r18;
						r12 = r12 - 2u;
						mem_write16(r12, r0_2 + r14);
						r0 = r1;
						if (r0 == 0u)
						{
							sub_0170A6(r15, r16, r18, &r5, &r6);
							return;
						}
					}
				}
				else
				{
					r9 = (r9 - 1u) * 2u;
					r11 -= r9;
					r12 = r12 - r9;
					OBJECT(current_object)->unk_60 = r1;
					OBJECT(current_object)->unk_74 = r11;
					OBJECT(current_object)->unk_78 = r12;
					OBJECT(current_object)->unk_7C = r14;
					r19 = 0xFFFFFFFFu;
					r11 -= 2u;
					mem_write16(r11, r2);
					r0_2 = (uint16_t)r3;
					if ((int16_t)r18 < (int16_t)r0_2)
						r0_2 = (uint16_t)r18;
					r12 = r12 - 2u;
					mem_write16(r12, r0_2 + r14);
					r0 = r1;
					if (r0 == 0u)
					{
						sub_0170A6(r15, r16, r18, &r5, &r6);
						return;
					}
				}
			}
			else
			{
				r19 = 0u;
				OBJECT(current_object)->unk_60 = 0u;
			}
		}
		r4 = r1;
		r5 = r2;
		r6 = r3;
		r13_2--;
		if (r13_2 != 0u)
			continue;
		r13_2 = 2u;
		r14 = (r14 & ~0xFFFFu) | (uint16_t)(r14 ^ 0x100u);
	}
	t7 = shift_by(r0, (uint8_t)r18, 32);
	carry = v60_shl_cy(r0, (uint8_t)r18, 32);
	zero = t7 == 0u;
	r0 = t7;
	if (carry == 0u)
	{
		r0++;
		for (;;)
		{
			r12 = r12 - 4u;
			r0--;
			if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
				break;
			mem_write32(r12, 0u);
		}
		sub_0170A6(r15, r16, r18, &r5, &r6);
		return;
	}
	r12 = r12 - 2u;
	mem_write16(r12, 0u);
	if (zero)
	{
		sub_0170A6(r15, r16, r18, &r5, &r6);
		return;
	}
	r0++;
	for (;;)
	{
		r12 = r12 - 4u;
		r0--;
		if (((r0 != 0u) & (mem_read32(r12) != 0u)) == 0u)
			break;
		mem_write32(r12, 0u);
	}
	sub_0170A6(r15, r16, r18, &r5, &r6);
}

void sub_016FEB(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3, uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r7, uint32_t *p_r8, uint32_t *p_r11, uint32_t *p_zero)
{
	uint32_t r4 = *p_r4;
	uint32_t r5 = *p_r5;
	uint32_t r6 = *p_r6;
	uint32_t zero = *p_zero;
	uint16_t r0_2 = 0;
	uint32_t r4_2 = 0, r7 = 0, r8 = 0, r9 = 0;
	uint32_t t1 = 0, t2 = 0;

	r9 = r4 - r1;
	r7 = divide_s32((r5 - r2) << 12, r9);
	r5 = (r5 << 12) + r16;
	r8 = divide_s32((r6 - r3) << 12, r9);
	r6 = (r6 << 12) + r16;
	if ((int32_t)r1 < 0)
	{
		r9 += r1;
		t1 = r4;
		r4_2 = t1 - r0;
		if ((int32_t)r0 < (int32_t)t1)
		{
			r9 -= r4_2;
			r5 -= r7 * r4_2;
			r6 -= r8 * r4_2;
		}
		do
		{
			r5 -= r7;
			r6 -= r8;
			*p_r11 -= 2u;
			mem_write16(*p_r11, shift_signed_by(r5, (uint8_t)r15, 32));
			r0_2 = (uint16_t)shift_signed_by(r6, (uint8_t)r15, 32);
			if ((int16_t)r18 < (int16_t)r0_2)
				r0_2 = (uint16_t)r18;
			r12 = r12 - 2u;
			mem_write16(r12, r0_2 + r14);
			r9--;
		} while (r9 != 0u);
		zero = 1u == 0u;
		*p_r4 = r4_2;
		*p_r5 = r5;
		*p_r6 = r6;
		*p_r7 = r7;
		*p_r8 = r8;
		*p_zero = zero;
		return;
	}
	r9--;
	t2 = r4;
	r4_2 = t2 - r0;
	if ((int32_t)r0 < (int32_t)t2)
	{
		r9 -= r4_2;
		r5 -= r7 * r4_2;
		r6 -= r8 * r4_2;
	}
	do
	{
		r5 -= r7;
		r6 -= r8;
		*p_r11 -= 2u;
		mem_write16(*p_r11, shift_signed_by(r5, (uint8_t)r15, 32));
		r0_2 = (uint16_t)shift_signed_by(r6, (uint8_t)r15, 32);
		if ((int16_t)r18 < (int16_t)r0_2)
			r0_2 = (uint16_t)r18;
		r12 = r12 - 2u;
		mem_write16(r12, r0_2 + r14);
		r9--;
	} while (r9 != 0u);
	zero = 0u == 0u;
	*p_r4 = r4_2;
	*p_r5 = r5;
	*p_r6 = r6;
	*p_r7 = r7;
	*p_r8 = r8;
	*p_zero = zero;
}

void sub_0170A6(uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r5, uint32_t *p_r6)
{
	uint16_t r0 = 0;
	uint32_t r1 = 0, r7 = 0, r8 = 0, r9 = 0, r11 = 0;

	r1 = OBJECT(current_object)->unk_60;
	if (r1 == 0u)
		return;
	*p_r5 = (OBJECT(current_object)->unk_64 << 12) + r16;
	*p_r6 = (AS32(OBJECT(current_object)->unk_68) << 12) + r16;
	r7 = OBJECT(current_object)->unk_6C;
	r8 = OBJECT(current_object)->unk_70;
	r11 = OBJECT(current_object)->unk_74;
	r12 = OBJECT(current_object)->unk_78;
	r14 = OBJECT(current_object)->unk_7C;
	r9 = 0xDFu - r1;
	if (0xDFu == r1)
	{
		return;
	}
	do
	{
		*p_r5 += r7;
		*p_r6 += r8;
		mem_write16(r11, shift_signed_by(*p_r5, (uint8_t)r15, 32));
		r0 = (uint16_t)shift_signed_by(*p_r6, (uint8_t)r15, 32);
		r11 += 2u;
		if ((int16_t)r18 < (int16_t)r0)
		{
			r0 = (uint16_t)r18;
			r7 = 0u;
		}
		mem_write16(r12, r0 + r14);
		r12 = r12 + 2u;
		r9--;
	} while (r9 != 0u);
}

void sub_017188(void)
{
	uint32_t r21 = 0;

	r21 = sub_011D43();
	mem_write8(r21 + 9u, 0u);
	mem_write32(r21 + 0x14u, current_object);
	mem_write32(r21 + 0x3Cu, current_object + 0xAu);
	mem_write32(r21 + 0x38u, 0x17444u);
	mem_write32(r21 + 4u, sub_0171CE_ADDRESS);
}

void sub_0171CE(void)
{
	uint32_t r0 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r21 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;

	r21 = AS32(OBJECT(current_object)->unk_38);
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r0 = mem_read8(OBJECT(current_object)->unk_14 + 0xDu);
		AT8(OBJECT(current_object)->unk_08, 2) = r0;
		r12 = r21 - 0xCu + r0 * 4u;
		r13 = r21 + 0x18u + SCREEN(screen)->unk_00 * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r12) << 8) * 4u);
			t1 = mem_read8(r13);
			r13++;
			mem_write32(r11, 0x300000u + ((uint32_t)t1 << 8) * 4u);
			r11 += 4u;
			r9--;
			r12 = r12 + 1u;
			r10 += 4u;
		} while (r9 != 0u);
		AS8(OBJECT(current_object)->unk_10) = 0u;
		OBJECT(current_object)->unk_12 = 4u;
		OBJECT(current_object)->unk_0C++;
	case 1:
		r0 = (uint8_t)OBJECT(current_object)->unk_10;
		move_string(mem_read32(current_object + 0x18u + r0 * 4u), 0x200u, mem_read32(current_object + 0x28u + r0 * 4u), 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
		AS8(OBJECT(current_object)->unk_10)++;
		t2 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t2 - 1u;
		if (t2 != 1u)
			return;
		r10 = r21 + 0x20u + SCREEN(screen)->unk_00 * 5u * 2u;
		r11 = OBJECT(current_object)->unk_14 + 0x18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = 3u;
		do
		{
			r12 = r10;
			r8 = 0xAu;
			do
			{
				mem_write8(r11, mem_read8(r12));
				r8--;
				r12 = r12 + 1u;
				r11 += 2u;
			} while (r8 != 0u);
			r9--;
		} while (r9 != 0u);
		r10 = current_object + 0x18u;
		r11 = r21 + (uint8_t)(OBJECT(current_object)->unk_08 >> 16u) * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		AS8(OBJECT(current_object)->unk_10) = 0x10u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 2:
		r0 = 0x10u;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		t3 = (uint8_t)OBJECT(current_object)->unk_10;
		AS8(OBJECT(current_object)->unk_10) = t3 - 1u;
		if (t3 != 1u)
			return;
		r10 = r21 + 0x34u;
		r11 = OBJECT(current_object)->unk_14 + 0x18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, mem_read8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->unk_0C++;
	case 3:
		if (mem_read8(AS32(OBJECT(current_object)->unk_3C)) != 0u)
			return;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r0 = mem_read8(OBJECT(current_object)->unk_14 + 0xDu);
		AT8(OBJECT(current_object)->unk_08, 2) = r0;
		r12 = r21 + r0 * 4u;
		r13 = r21 + 0x18u + SCREEN(screen)->unk_00 * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r12) << 8) * 4u);
			t4 = mem_read8(r13);
			r13++;
			mem_write32(r11, 0x300000u + ((uint32_t)t4 << 8) * 4u);
			r11 += 4u;
			r9--;
			r12 = r12 + 1u;
			r10 += 4u;
		} while (r9 != 0u);
		AS8(OBJECT(current_object)->unk_10) = 0u;
		OBJECT(current_object)->unk_12 = 4u;
		OBJECT(current_object)->unk_0C++;
	case 4:
		r0 = (uint8_t)OBJECT(current_object)->unk_10;
		move_string(mem_read32(current_object + 0x18u + r0 * 4u), 0x200u, mem_read32(current_object + 0x28u + r0 * 4u), 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
		AS8(OBJECT(current_object)->unk_10)++;
		t5 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t5 - 1u;
		if (t5 != 1u)
			return;
		r10 = r21 + 0x20u + SCREEN(screen)->unk_00 * 5u * 2u;
		r11 = OBJECT(current_object)->unk_14 + 0x18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = 3u;
		do
		{
			r12 = r10;
			r8 = 0xAu;
			do
			{
				mem_write8(r11, mem_read8(r12));
				r8--;
				r12 = r12 + 1u;
				r11 += 2u;
			} while (r8 != 0u);
			r9--;
		} while (r9 != 0u);
		r10 = current_object + 0x18u;
		r11 = r21 - 0xCu + (uint8_t)(OBJECT(current_object)->unk_08 >> 16u) * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		AS8(OBJECT(current_object)->unk_10) = 0x10u;
		OBJECT(current_object)->unk_0C++;
		return;
	case 5:
		r0 = 0x10u;
		r10 = current_object + 0x18u;
		r11 = current_object + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, 0u, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		t6 = (uint8_t)OBJECT(current_object)->unk_10;
		AS8(OBJECT(current_object)->unk_10) = t6 - 1u;
		if (t6 != 1u)
			return;
		r10 = r21 + 0x52u;
		r11 = OBJECT(current_object)->unk_14 + 0x18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, mem_read8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_0171E0[OBJECT(current_object)->unk_0C]);
	}
}

void sub_017524(void)
{
	bool L_017685 = false;
	uint32_t zero = 0;
	uint8_t r1 = 0;
	uint16_t r0_2 = 0, r1_2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint32_t stack_4 = 0;
	uint8_t t2 = 0;
	uint16_t t3 = 0, t4 = 0, t5 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_017530[t1];
	switch (t1)
	{
	case 0:
		r0 = SCREEN(screen)->unk_00;
		OBJECT(current_object)->unk_0E = r0;
		r10 = 0x17A50u + r0 * 5u * 4u;
		OBJECT(current_object)->unk_18 = ROM32(r10);
		r10 += 4u;
		AS32(OBJECT(current_object)->unk_1C) = ROM32(r10);
		r10 += 4u;
		AS32(OBJECT(current_object)->unk_24) = ROM32(r10);
		r10 += 4u;
		r11 = ROM32(r10);
		AS32(OBJECT(current_object)->unk_20) = r11;
		mem_write32(r11, rom_017AE0[(uint32_t)OBJECT(current_object)->unk_0E << 3]);
		r10 = ROM32(r10 + 4u);
		r0 = mem_read32(r10);
		r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
		mem_write32(r10, (r0 & ~0xFFu) | (uint8_t)(r0 | 4u));
		SCREEN(screen)->unk_94 |= 4u;
		r0 = SCREEN(screen)->unk_A0;
		if (r0 != 0u)
			r0 = (uint8_t)~rom_017B60[r0];
		t2 = ~r0;
		OBJECT(current_object)->unk_0F = t2;
		r10 = current_object + 0x2Cu;
		r11 = 0x109684u;
		if ((int8_t)t2 >= 0)
			r11 = rom_109694[(uint8_t)~r0] + 0x10u;
		r12 = r10;
		r0 = OBJECT(current_object)->unk_0E * 5u * 2u + 1u;
		r9 = 4u;
		do
		{
			mem_write32(r12, byte_swap32(r0) | mem_read32(r11));
			r0++;
			r9--;
			r11 += 4u;
			r12 = r12 + 4u;
		} while (r9 != 0u);
		mem_write32(r12, 0xFFFFFFFFu);
		sub_010FA1(r10);
		mem_write8(AS32(OBJECT(current_object)->unk_24), rom_017BA9[SCREEN(screen)->unk_A0]);
		SCREEN(screen)->unk_202 = 0u;
		SCREEN(screen)->unk_203 = 0u;
		SCREEN(screen)->unk_205 = 0u;
		SCREEN(screen)->unk_206 = 0xFFu;
		SCREEN(screen)->unk_207 = 0xFFu;
		r0 = SCREEN(screen)->unk_A0;
		sub_017BC8(r0);
		r19 = 0x13F9D4u;
		r0 = OBJECT(current_object)->unk_0F;
		if ((int8_t)r0 >= 0)
		{
			r19 = rom_031B14[r0];
			sub_01850B(r19);
			r19 = rom_0318CC[OBJECT(current_object)->unk_0F];
		}
		sub_0115C5(r19);
		OBJECT(current_object)->unk_0C++;
		r1 = SCREEN(screen)->unk_A0;
		if (r1 == 6u)
			sub_01789C();
		else
		{
			r0 = 0u;
			if (r1 == 8u)
				sub_01791C(r0);
			else
			{
				r0 = 1u;
				if (r1 == 0x1Cu)
					sub_01791C(r0);
			}
		}
	case 1:
		if (SCREEN(screen)->unk_203 != 0u)
			L_017685 = true;
		else if (CAR(player_car)->unk_CA != 0u)
		{
			CAR(player_car)->unk_CA = 0u;
			L_017685 = true;
		}
		if (L_017685)
		{
			SCREEN(screen)->unk_202 = 0xFFu;
			r0 = SCREEN(screen)->unk_A3;
			if (SCREEN(screen)->unk_203 == 0u)
			{
				r0 = r0 * 2u;
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + CAR(player_car)->unk_C9);
			}
			else
				r0 = (uint8_t)(r0 + 0x2Au);
			OBJECT(current_object)->unk_0F = r0;
			r10 = current_object + 0x2Cu;
			r11 = rom_109694[r0];
			r12 = r10;
			r0 = OBJECT(current_object)->unk_0E;
			r0 = r0 ^ 2u;
			OBJECT(current_object)->unk_0E = r0;
			r0 = r0 * 5u * 2u + 1u;
			r9 = 9u;
			do
			{
				mem_write32(r12, byte_swap32(r0) | mem_read32(r11));
				r0++;
				r9--;
				r11 += 4u;
				r12 = r12 + 4u;
			} while (r9 != 0u);
			mem_write32(r12, 0xFFFFFFFFu);
			sub_011011(r10);
			r0 = OBJECT(current_object)->unk_0E * 2u;
			r1 = SCREEN(screen)->unk_204;
			if (SCREEN(screen)->unk_203 == 0u)
				r1 = CAR(player_car)->unk_C9;
			OBJECT(current_object)->unk_28 = rom_017AE0_ADDRESS + ((r0 & ~0xFFu) | (uint8_t)(r0 + r1)) * 2u * 2u * 4u;
			OBJECT(current_object)->unk_0C++;
			OBJECT(current_object)->unk_0D = 0u;
		}
		break;
	case 2:
		r0 = (r0 & ~0xFFFFu) | CAR(player_car)->unk_2E;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 4);
		t3 = (int16_t)r0 >> 4;
		zero = t3 == 0u;
		r0 = (r0 & ~0xFFFFu) | t3;
		if ((int16_t)t3 < 0)
		{
			t4 = 0u - r0;
			zero = t4 == 0u;
			r0 = (r0 & ~0xFFFFu) | t4;
		}
		switch (OBJECT(current_object)->unk_0D)
		{
		case 0:
			if (zero == 0u)
			{
				SCREEN(screen)->unk_206 = 0xFFu;
				r0 = OBJECT(current_object)->unk_0F;
				r19 = rom_0318CC[r0];
				sub_0115C5(r19);
				mem_write8(AS32(OBJECT(current_object)->unk_24), rom_017B7F[r0]);
				r10 = OBJECT(current_object)->unk_28;
				mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
				OBJECT(current_object)->unk_28 = r10 + 4u;
				OBJECT(current_object)->unk_0D++;
			}
			break;
		case 1:
			if ((int16_t)r0 >= 0x2A0)
			{
				SCREEN(screen)->unk_205 = 0xFFu;
				r19 = rom_0319F0[OBJECT(current_object)->unk_0F];
				sub_0184D0(r19);
				r19 = rom_031CB8[OBJECT(current_object)->unk_0F];
				if (r19 == 0u)
				{
					r10 = OBJECT(current_object)->unk_28;
					mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
					OBJECT(current_object)->unk_28 = r10 + 4u;
					OBJECT(current_object)->unk_0D++;
				}
				else
				{
					SCREEN(screen)->unk_208 = mem_read32(r19);
					r19 += 4u;
					sub_0184D0(r19);
					r10 = OBJECT(current_object)->unk_28;
					mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
					OBJECT(current_object)->unk_28 = r10 + 4u;
					OBJECT(current_object)->unk_0D++;
				}
			}
			break;
		case 2:
			stack_4 = r0;
			SCREEN(screen)->unk_205 = 0u;
			SCREEN(screen)->unk_207 = 0u;
			r0 = SCREEN(screen)->unk_A1;
			sub_017BC8(r0);
			r0 = stack_4;
			OBJECT(current_object)->unk_0D++;
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(current_object)->unk_0F];
				sub_01850B(r19);
				r10 = OBJECT(current_object)->unk_28;
				mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
				OBJECT(current_object)->unk_28 = r10 + 4u;
				OBJECT(current_object)->unk_0D++;
			}
			break;
		case 3:
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(current_object)->unk_0F];
				sub_01850B(r19);
				r10 = OBJECT(current_object)->unk_28;
				mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
				OBJECT(current_object)->unk_28 = r10 + 4u;
				OBJECT(current_object)->unk_0D++;
			}
			break;
		case 4:
			if ((int16_t)r0 <= 0x60)
			{
				r10 = OBJECT(current_object)->unk_28;
				mem_write32(AS32(OBJECT(current_object)->unk_20), mem_read32(r10));
				OBJECT(current_object)->unk_28 = r10 + 4u;
				OBJECT(current_object)->unk_0D++;
			}
			break;
		case 5:
			if (zero)
			{
				SCREEN(screen)->unk_202 = 0u;
				SCREEN(screen)->unk_203 = 0u;
				OBJECT(current_object)->unk_0C--;
				r1 = OBJECT(current_object)->unk_0F;
				if (r1 == 5u)
					sub_01789C();
				else
				{
					r0 = 0u;
					if (r1 == 7u || r1 == 8u)
						sub_01791C(r0);
					else
					{
						r0 = 2u;
						if (r1 == 0x25u)
						{
							sub_01791C(r0);
							r0_2 = CAR(player_car)->unk_2E;
							if (SCREEN(screen)->unk_202 == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(current_object)->unk_18, r0_2 - 0xA0u);
							t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->unk_30] >> 6) + SCREEN(screen)->unk_B8 - 1u;
							r0_2 = t5;
							if ((int16_t)t5 >= 0)
							{
								r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							SCREEN(screen)->unk_20C = r0_2;
							mem_write16(AS32(OBJECT(current_object)->unk_1C), r0_2);
							return;
						}
						r0 = 1u;
						if (r1 == 0x26u)
						{
							sub_01791C(r0);
							r0_2 = CAR(player_car)->unk_2E;
							if (SCREEN(screen)->unk_202 == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(current_object)->unk_18, r0_2 - 0xA0u);
							t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->unk_30] >> 6) + SCREEN(screen)->unk_B8 - 1u;
							r0_2 = t5;
							if ((int16_t)t5 >= 0)
							{
								r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							SCREEN(screen)->unk_20C = r0_2;
							mem_write16(AS32(OBJECT(current_object)->unk_1C), r0_2);
							return;
						}
					}
				}
			}
			break;
		default:
			invalid_jump(rom_017738[OBJECT(current_object)->unk_0D]);
		}
		break;
	default:
		invalid_jump(rom_017530[t1]);
	}
	r0_2 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0_2 = r0_2 >> 2;
	mem_write16(OBJECT(current_object)->unk_18, r0_2 - 0xA0u);
	t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)CAR(player_car)->unk_30] >> 6) + SCREEN(screen)->unk_B8 - 1u;
	r0_2 = t5;
	if ((int16_t)t5 >= 0)
	{
		r1_2 = (uint16_t)(0u - SCREEN(screen)->unk_200 + 0x180u);
		if ((int16_t)r1_2 < (int16_t)r0_2)
			r0_2 = (uint16_t)r1_2;
	}
	else
		r0_2 = 0u;
	SCREEN(screen)->unk_20C = r0_2;
	mem_write16(AS32(OBJECT(current_object)->unk_1C), r0_2);
}

void sub_01789C(void)
{
	uint32_t r10 = 0, r21 = 0;

	r10 = rom_017904[SCREEN(screen)->unk_00];
	sub_011011(r10);
	r21 = sub_011D43();
	mem_write8(r21 + 0x10u, 2u);
	mem_write8(r21 + 0x12u, SCREEN(screen)->unk_00);
	mem_write32(r21 + 0x20u, AS32(OBJECT(current_object)->unk_20));
	mem_write32(r21 + 4u, sub_0178C9_ADDRESS);
}

void sub_0178C9(void)
{
	if (SCREEN(screen)->unk_B00 != 0u)
		return;
	OBJECT(current_object)->unk_04 = sub_0178D5_ADDRESS;
	sub_0178D5();
}

void sub_0178D5(void)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (SCREEN(screen)->unk_202 != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	t1 = (uint8_t)OBJECT(current_object)->unk_10;
	AS8(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		AS8(OBJECT(current_object)->unk_10) = 2u;
		r0 = OBJECT(current_object)->unk_12;
		r0 = (uint8_t)(r0 + 2u);
		r0 = r0 & 7u;
		OBJECT(current_object)->unk_12 = r0;
		mem_write16(AS32(OBJECT(current_object)->unk_20) + 2u, rom_01790C[r0]);
	}
}

void sub_01791C(uint32_t r0)
{
	uint32_t r10 = 0, r21 = 0;

	r0 = r0 * 2u;
	r10 = rom_017988[(r0 & ~0xFFu) | (uint8_t)(r0 + SCREEN(screen)->unk_00)];
	sub_011011(r10);
	r21 = sub_011D43();
	mem_write8(r21 + 0x10u, 3u);
	mem_write8(r21 + 0x12u, SCREEN(screen)->unk_00);
	mem_write32(r21 + 0x20u, AS32(OBJECT(current_object)->unk_20));
	mem_write32(r21 + 4u, sub_01794C_ADDRESS);
}

void sub_01794C(void)
{
	if (SCREEN(screen)->unk_B00 != 0u)
		return;
	OBJECT(current_object)->unk_04 = sub_017958_ADDRESS;
	sub_017958();
}

void sub_017958(void)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (SCREEN(screen)->unk_202 != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	t1 = (uint8_t)OBJECT(current_object)->unk_10;
	AS8(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 != 1u)
		return;
	AS8(OBJECT(current_object)->unk_10) = 3u;
	r0 = OBJECT(current_object)->unk_12;
	r0 = (uint8_t)(r0 + 2u);
	if ((int8_t)r0 >= 6)
		r0 = (uint8_t)(r0 - 6u);
	OBJECT(current_object)->unk_12 = r0;
	mem_write16(AS32(OBJECT(current_object)->unk_20), rom_0179A0[r0]);
}

void sub_0179AC(uint32_t r19)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0, r19_2 = 0;
	uint8_t t3 = 0, t4 = 0;
	uint32_t t1 = 0, t2 = 0;

	r10 = rom_017A78[SCREEN(screen)->unk_00];
	r11 = mem_read32(r10);
	t1 = mem_read32(r19);
	r19 += 4u;
	mem_write32(r11, t1);
	t2 = mem_read32(r19);
	r19 += 4u;
	mem_write32(r11 + 8u, t2);
	r10 += 4u;
	r11 = mem_read32(r10);
	t3 = mem_read8(r19);
	r19++;
	mem_write8(r11, t3);
	t4 = mem_read8(r19);
	r19++;
	mem_write8(r11 + 2u, t4);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0x200u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0x200u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	mem_write16(mem_read32(r10), 0u);
	r10 += 4u;
	unk_20E204 &= mem_read32(r10);
	unk_20E027 = 0xFFu;
	r11 = mem_read32(r10 + 4u);
	r0 = mem_read32(r11);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	mem_write32(r11, (r0 & ~0xFFu) | (uint8_t)(r0 | mem_read8(r19)));
	r11 += 4u;
	r0 = mem_read32(r11);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	r19++;
	mem_write32(r11, (r0 & ~0xFFu) | (uint8_t)(r0 | mem_read8(r19)));
	SCREEN(screen)->unk_94 |= 4u;
	r19++;
	r10 = mem_read32(r19);
	sub_011011(r10);
	r19_2 = mem_read32(r19 + 4u);
	sub_0115C5(r19_2);
}

void sub_017BC8(uint32_t r0)
{
	uint32_t r0_2 = 0, r10 = 0, r21 = 0;
	uint32_t t1 = 0;

	if (SCREEN(screen)->unk_203 != 0u)
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 0x1Fu);
	r10 = object_lists_181B8[r0];
	t1 = mem_read32(r10) >> 3;
	r0_2 = t1;
	r10 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, mem_read32(r10));
			r10 += 4u;
			mem_write32(r21 + 0x80u, mem_read32(r10));
			mem_write32(r21 + 0x14u, current_object);
			r0_2--;
			r10 += 4u;
		} while (r0_2 != 0u);
	}
}

void sub_017BFB(void)
{
	uint16_t r0 = 0;
	uint32_t r10 = 0, r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_80;
		OBJECT(current_object)->unk_10 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_12) = mem_read16(r10);
		r10 += 2u;
		AS32(OBJECT(current_object)->unk_38) = mem_read32(r10);
		r0 = (uint16_t)mem_read32(r10 + 4u);
		AS16(OBJECT(current_object)->unk_14) = r0;
		AT16(OBJECT(current_object)->unk_14, 2) = r0;
		OBJECT(current_object)->unk_0D = 0u;
		OBJECT(current_object)->unk_0C++;
		if (OBJECT(current_object)->unk_10 != 0u)
		{
			t1 = OBJECT(current_object)->unk_10;
			OBJECT(current_object)->unk_10 = t1 - 1u;
			if (t1 != 1u)
				return;
		}
		OBJECT(current_object)->unk_0C++;
		if ((OBJECT(current_object)->unk_12 | OBJECT(current_object)->unk_13) != 0u)
		{
			t2 = AS16(OBJECT(current_object)->unk_12);
			AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
			if (t2 == 1u)
			{
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
		}
		else
			OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_0C++;
		if ((OBJECT(current_object)->unk_12 | OBJECT(current_object)->unk_13) != 0u)
		{
			t2 = AS16(OBJECT(current_object)->unk_12);
			AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
			if (t2 == 1u)
			{
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
		}
		else
			OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		t2 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_017C08[OBJECT(current_object)->unk_0C]);
	}
	switch (OBJECT(current_object)->unk_0D)
	{
	case 0:
		if (SCREEN(screen)->unk_206 != 0u)
			return;
		OBJECT(current_object)->unk_0D++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017C68[OBJECT(current_object)->unk_0D]);
	}
	if (SCREEN(screen)->unk_206 != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	t3 = (uint16_t)(OBJECT(current_object)->unk_14 >> 16u);
	AT16(OBJECT(current_object)->unk_14, 2) = t3 - 1u;
	if (t3 != 1u)
		return;
	AT16(OBJECT(current_object)->unk_14, 2) = (uint16_t)OBJECT(current_object)->unk_14;
	r10 = AS32(OBJECT(current_object)->unk_38);
	r19 = mem_read32(r10);
	r10 += 4u;
	if ((int32_t)r19 < 0)
	{
		r10 += r19;
		r19 = mem_read32(r10);
		r10 += 4u;
	}
	AS32(OBJECT(current_object)->unk_38) = r10;
	sub_0115C5(r19);
}

void sub_017CAA(void)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r17 = 0, r18 = 0, r19 = 0, r21 = 0;
	uint32_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = OBJECT(current_object)->unk_80;
		AS32(OBJECT(current_object)->unk_10) = mem_read32(r10);
		AS32(OBJECT(current_object)->unk_38) = mem_read32(r10 + 4u);
		OBJECT(current_object)->unk_0C++;
		if (OBJECT(current_object)->unk_10 != 0u)
		{
			t1 = AS32(OBJECT(current_object)->unk_10);
			AS32(OBJECT(current_object)->unk_10) = t1 - 1u;
			if (t1 != 1u)
				return;
		}
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		t1 = AS32(OBJECT(current_object)->unk_10);
		AS32(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017CB8[OBJECT(current_object)->unk_0C]);
	}
	if (SCREEN(screen)->unk_206 != 0u)
		return;
	r19 = AS32(OBJECT(current_object)->unk_38);
	t2 = mem_read32(r19) / 0xCu;
	r18 = t2;
	r19 += 4u;
	if (t2 == 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	AS32(OBJECT(current_object)->unk_10) = r18;
	r17 = current_object + 0x18u;
	do
	{
		r10 = mem_read32(r19);
		r11 = mem_read32(r19 + 4u);
		r19 += 8u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
		r19 += 2u;
		r0 = mem_read8(r19);
		r19++;
		r1 = mem_read8(r19);
		r19++;
		r21 = sub_018581(r0, r1, r10, r11);
		mem_write32(r17, r21);
		r18--;
		r17 += 4u;
	} while (r18 != 0u);
	OBJECT(current_object)->unk_04 = sub_017D1F_ADDRESS;
}

void sub_017D1F(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = current_object + 0x18u;
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
		OBJECT(current_object)->unk_00 = 0u;
	if (SCREEN(screen)->unk_206 == 0u)
		return;
	r10 = current_object + 0x18u;
	r9 = AS32(OBJECT(current_object)->unk_10);
	do
	{
		r1 = mem_read32(r10);
		r10 += 4u;
		if (r1 != 0u)
			mem_write8(r1, 0u);
		r9--;
	} while (r9 != 0u);
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_017D70(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 1u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_04 = sub_017DB0_ADDRESS;
	sub_017DB0();
}

void sub_017DB0(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->unk_30 = 0u - (((r0 - OBJECT(current_object)->unk_20) & 0x3FFu) - 0x2A0u);
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017DE4[OBJECT(current_object)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero == 0u)
	{
		r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = 0u - r0_2;
		r0_2 = sub_00EAE4(r0_2);
		return;
	}
	if (OBJECT(current_object)->unk_0D != 0u)
		OBJECT(current_object)->unk_00 = 0u;
}

void sub_017E1B(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 1u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_04 = sub_017E5B_ADDRESS;
	sub_017E5B();
}

void sub_017E5B(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(current_object)->unk_20) & 0x3FFu;
	OBJECT(current_object)->unk_30 = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017E98[OBJECT(current_object)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(current_object)->unk_0D != 0u)
			OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r1 = OBJECT(current_object)->unk_24;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - r1);
	r1 -= SCREEN(screen)->unk_200;
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(current_object)->unk_32);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(current_object)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_017EF2(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 2u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_10 = 0u - mem_read32(r10 + 2u) + CAR(player_car)->unk_42;
	OBJECT(current_object)->unk_04 = sub_017F3D_ADDRESS;
	sub_017F3D();
}

void sub_017F3D(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(current_object)->unk_20) & 0x3FFu;
	OBJECT(current_object)->unk_30 = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		r0_2 = CAR(player_car)->unk_42;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->unk_10);
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		break;
	case 1:
		r0_2 = CAR(player_car)->unk_42;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->unk_10);
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017F7C[OBJECT(current_object)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(current_object)->unk_0D != 0u)
			OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r1 = OBJECT(current_object)->unk_24;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - r1);
	r1 -= SCREEN(screen)->unk_200;
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(current_object)->unk_32);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(current_object)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_018009(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 1u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_10 = 0x120u;
	OBJECT(current_object)->unk_04 = sub_018050_ADDRESS;
	sub_018050();
}

void sub_018050(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;
	uint16_t t1 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->unk_30 = 0u - (((r0 - OBJECT(current_object)->unk_20) & 0x3FFu) - 0x2A0u);
	sub_00F30D(0u);
	t1 = OBJECT(current_object)->unk_10;
	r0 = OBJECT(current_object)->unk_3A;
	OBJECT(current_object)->unk_3A = t1;
	OBJECT(current_object)->unk_10 = r0;
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_018094[OBJECT(current_object)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero == 0u)
	{
		r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = 0u - r0_2;
		r0_2 = sub_00EAE4(r0_2);
		return;
	}
	if (OBJECT(current_object)->unk_0D != 0u)
		OBJECT(current_object)->unk_00 = 0u;
}

void sub_0180CB(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (SCREEN(screen)->unk_207 != 0u)
		OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_0D = 0u;
	r10 = OBJECT(current_object)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(current_object)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(current_object)->unk_24 = mem_read16(r10);
	AS16(OBJECT(current_object)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 1u;
	sub_00F366(r9);
	AS8(OBJECT(current_object)->unk_10) = 0u;
	OBJECT(current_object)->unk_04 = sub_018110_ADDRESS;
	sub_018110();
}

void sub_018110(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;

	OBJECT(current_object)->unk_0D |= SCREEN(screen)->unk_205;
	r0 = CAR(player_car)->unk_2E;
	if (SCREEN(screen)->unk_202 == 0u)
		r0 = r0 >> 2;
	OBJECT(current_object)->unk_30 = 0u - (((r0 - OBJECT(current_object)->unk_20) & 0x3FFu) - 0x2A0u);
	sub_00F30D(0u);
	AS8(OBJECT(current_object)->unk_10)++;
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01814C[OBJECT(current_object)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(current_object)->unk_0D != 0u)
			OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	if ((OBJECT(current_object)->unk_10 & 1u) != 0u)
		return;
	r0_2 = (uint16_t)(SCREEN(screen)->unk_20C - 0xFFu - OBJECT(current_object)->unk_24);
	OBJECT(current_object)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

uint32_t sub_01818A(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;

	r0 = 0xA0u;
	r1 = (uint16_t)(OBJECT(current_object)->unk_30 - 0xA0u);
	r3 = (uint16_t)OBJECT(current_object)->unk_28;
	r2 = (uint16_t)(0u - r1 - r3);
	if ((int16_t)(r1 - r3) > 0xA0 || (int16_t)r0 < (int16_t)r2)
	{
		OBJECT(current_object)->unk_00 &= 0xDFu;
		zero = 0u == 0u;
		return zero;
	}
	zero = 1u == 0u;
	return zero;
}

void sub_0184D0(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r18 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r10 = mem_read32(r19);
			r11 = mem_read32(r19 + 4u);
			r19 += 8u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
			r19 += 2u;
			r0 = mem_read8(r19);
			r19++;
			r1 = mem_read8(r19);
			r19++;
			sub_018581(r0, r1, r10, r11);
			r18--;
		} while (r18 != 0u);
	}
}

void sub_0184EC(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r18 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 != 0u)
	{
		do
		{
			r11 = mem_read32(r19);
			r19 += 4u;
			r10 = mem_read32(r19);
			r19 += 4u;
			r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
			r19 += 2u;
			r0 = mem_read8(r19);
			r19++;
			r1 = mem_read8(r19);
			r19++;
			sub_018581(r0, r1, r10, r11);
			r18--;
		} while (r18 != 0u);
	}
}

void sub_01850B(uint32_t r19)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r17 = 0, r18 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r19) / 0xCu;
	r18 = t1;
	r19 += 4u;
	if (t1 == 0u)
	{
		SCREEN(screen)->unk_206 = 0u;
		return;
	}
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01854A_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	mem_write32(r21 + 0x10u, r18);
	r17 = r21 + 0x80u;
	do
	{
		r10 = mem_read32(r19);
		r11 = mem_read32(r19 + 4u);
		r19 += 8u;
		r12 = (r12 & ~0xFFFFu) | mem_read16(r19);
		r19 += 2u;
		r0 = mem_read8(r19);
		r19++;
		r1 = mem_read8(r19);
		r19++;
		r21 = sub_018581(r0, r1, r10, r11);
		mem_write32(r17, r21);
		r18--;
		r17 += 4u;
	} while (r18 != 0u);
}
