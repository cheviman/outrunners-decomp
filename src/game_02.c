/**
 *    Name:        game_02.c
 *    Description: the game's functions, part 02 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_00EE37(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	t1 = OBJECT(r23)->unk_42;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = r25 + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(r20)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = (uint16_t)(((int32_t)((int16_t)OBJECT(r20)->unk_24 * r2) >> 12) + mem_read16(r11 + 6u));
	sub_00EEF0(r1, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00EE75(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	t1 = mem_read16(mem_read32(r25 + 0x20u) + 0x46u);
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = r25 + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(r20)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = (uint16_t)(((int32_t)((int16_t)OBJECT(r20)->unk_24 * r2) >> 12) + mem_read16(r11 + 6u));
	r2 = shift_by(r2, OBJECT(r20)->unk_44, 32) >> 4;
	sub_00EEFF(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00EEBF(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	t1 = OBJECT(r23)->unk_42;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = r25 + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(r20)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = mem_read16(r11 + 6u);
	sub_00EEF0(r1, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00EEF0(uint16_t r1, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;
	uint32_t r2 = 0;

	r2 = mem_read16(r11);
	sub_00EEF4(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

void sub_00EEF4(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;

	r2 = shift_by(r2, OBJECT(r20)->unk_44, 32) >> 4;
	sub_00EEFF(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

void sub_00EEFF(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;
	uint32_t r3 = 0, left_caller_2 = 0;

	OBJECT(r20)->unk_30 = r0;
	OBJECT(r20)->unk_32 = r1;
	if ((int16_t)r0 > 0x1A0 || (int16_t)r0 < -0x1A0 || (int16_t)r1 > 0x80 || (int16_t)r1 < -0x160)
	{
		left_caller_2 = 1u;
		*p_left_caller = left_caller_2;
		return;
	}
	r3 = r2;
	OBJECT(r20)->unk_3C = mem_read16(r11 + 8u);
	sub_00EF4A(r1, r2, r3, &r0, p_r10);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

void sub_00EF32(uint32_t *p_r0, uint32_t *p_r10)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r3 = 0;

	r0 = (uint16_t)(OBJECT(r20)->unk_30 - mem_read16(r25 + 0x14u));
	r1 = (uint16_t)(OBJECT(r20)->unk_32 - mem_read16(r25 + 0x16u));
	r2 = OBJECT(r20)->unk_36;
	r3 = OBJECT(r20)->unk_34;
	sub_00EF4A(r1, r2, r3, &r0, p_r10);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

void sub_00EF4A(uint16_t r1, uint32_t r2, uint32_t r3, uint32_t *p_r0, uint32_t *p_r10)
{
	uint16_t r0 = (uint16_t)*p_r0;
	uint32_t r10 = *p_r10;
	uint16_t r0_2 = 0, r1_2 = 0, r2_2 = 0, r3_2 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r19 = 0;
	uint32_t r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	r8 = (uint16_t)r0;
	r9 = (uint16_t)r1;
	r16 = r2;
	r15 = r3;
	r26 = OBJECT(r20)->unk_38;
	r12 = rom_034138[r26];
	r26 = OBJECT(r20)->unk_40;
	r12 = mem_read32(r12 + 4u + r26 * 4u);
	r26 = OBJECT(r20)->unk_3A;
	r13 = rom_032BA4[r26];
	r19 = AS16(OBJECT(r20)->unk_00) & 0xC0u;
	r26 = r19;
	r26 = r26 >> 5;
	r17 = r15 * rom_00F13C[r26];
	r18 = r16 * rom_00F140[r26];
	r11 = mem_read16(r12);
	r12 = r12 + 2u;
	do
	{
		r0_2 = mem_read16(r12) ^ r19;
		r26 = (uint8_t)(OBJECT(r20)->unk_42 >> 8u);
		r26 = r26 * 3u;
		r12 = r12 + 2u;
		r26 = (uint16_t)(r26 + mem_read16(r12));
		r14 = 0x122B74u + r26 * 4u;
		t1 = ROM16(r14);
		r14 = r14 + 2u;
		r1_2 = t1;
		t2 = ROM16(r14);
		r14 = r14 + 2u;
		r6 = t2;
		t3 = mem_read16(r13);
		r13 += 2u;
		r7 = t3;
		t4 = ROM16(r14);
		r14 = r14 + 2u;
		t5 = ROM16(r14);
		r14 = r14 + 2u;
		r2_2 = (((t4 * r16) >> 8) & 0x3FFu) | t5;
		t6 = ROM16(r14);
		r14 = r14 + 2u;
		t7 = ROM16(r14);
		r14 = r14 + 2u;
		r3_2 = (((t6 * r15) >> 8) & 0x3FFu) | t7 | mem_read16(r25 + 0x1Au);
		r12 = r12 + 2u;
		r4 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r18) >> 8) + r9);
		r12 = r12 + 2u;
		r5 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r17) >> 8) + r8);
		mem_write16(r10 + 0x3000u, r0_2);
		mem_write16(r10, r0_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r1_2);
		mem_write16(r10, r1_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r2_2);
		mem_write16(r10, r2_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r3_2);
		mem_write16(r10, r3_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r4);
		mem_write16(r10, r4);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r5);
		mem_write16(r10, r5);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r6);
		mem_write16(r10, r6);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r7);
		mem_write16(r10, r7);
		r10 += 2u;
		r11--;
		r12 = r12 + 2u;
	} while (r11 != 0u);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0_2;
	*p_r10 = r10;
}

void sub_00F04E(uint32_t *p_r0, uint32_t *p_r10)
{
	uint32_t r10 = *p_r10;
	uint16_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;
	uint32_t r13 = 0;

	r26 = OBJECT(r20)->unk_38;
	r12 = rom_034138[r26];
	r26 = OBJECT(r20)->unk_40;
	r12 = mem_read32(r12 + 4u + r26 * 4u);
	r26 = OBJECT(r20)->unk_3A;
	r13 = rom_032BA4[r26];
	r12 = r12 + 2u;
	r0 = mem_read16(r12);
	r26 = (uint8_t)(OBJECT(r20)->unk_42 >> 8u);
	r26 = r26 * 3u;
	r12 = r12 + 2u;
	r26 = (uint16_t)(r26 + mem_read16(r12));
	r14 = 0x122B74u + r26 * 4u;
	r1 = ROM16(r14);
	r14 = r14 + 2u;
	r6 = ROM16(r14);
	r7 = mem_read16(r13);
	r14 = r14 + 2u;
	r2 = ROM16(r14);
	r14 = r14 + 2u;
	r2 |= ROM16(r14);
	r14 = r14 + 2u;
	r3 = ROM16(r14);
	r14 = r14 + 2u;
	r3 = r3 | ROM16(r14) | mem_read16(r25 + 0x1Au);
	r12 = r12 + 2u;
	r4 = (uint16_t)(mem_read16(r12) + (OBJECT(r20)->unk_32 - mem_read16(r25 + 0x16u)));
	r12 = r12 + 2u;
	r5 = (uint16_t)(mem_read16(r12) + (OBJECT(r20)->unk_30 - mem_read16(r25 + 0x14u)));
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r3);
	mem_write16(r10, r3);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r4);
	mem_write16(r10, r4);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r5);
	mem_write16(r10, r5);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r6);
	mem_write16(r10, r6);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r7);
	mem_write16(r10, r7);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = r14 + 2u;
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0;
	*p_r10 = r10;
}

uint32_t sub_00F104(uint32_t r10)
{
	move_halfwords(r10, AS32(OBJECT(r20)->unk_38), 8u);
	move_string(AS32(OBJECT(r20)->unk_38), 8u, r10 + 0x3000u, 8u, 2u, 0u, 0u, r26, &r27, &r28);
	mem_write16(r10 + 6u, mem_read16(r10 + 6u) | mem_read16(r25 + 0x1Au));
	mem_write16(r10 + 0x3006u, mem_read16(r10 + 0x3006u) | mem_read16(r25 + 0x1Au));
	r10 += 0x10u;
	return r10;
}

uint32_t sub_00F126(uint32_t r10)
{
	move_halfwords(r10, AS32(OBJECT(r20)->unk_38), 8u);
	move_string(AS32(OBJECT(r20)->unk_38), 8u, r10 + 0x3000u, 8u, 2u, 0u, 0u, r26, &r27, &r28);
	r10 += 0x10u;
	return r10;
}

void sub_00F15C(uint32_t *p_r0, uint32_t *p_r10)
{
	uint32_t r10 = *p_r10;
	uint16_t r4 = 0, r9 = 0, r15 = 0, r16 = 0, r19 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r13 = 0, r15_2 = 0, r18 = 0;
	uint16_t t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;
	uint32_t t1 = 0;

	r28 = OBJECT(r20)->unk_50;
	r27 = mem_read32(r25 + 0xB4u);
	r0 = (uint32_t)(int16_t)OBJECT(r20)->unk_30 - mem_read16(r25 + 0x14u);
	r1 = (uint32_t)(int16_t)OBJECT(r20)->unk_32 - mem_read16(r25 + 0x16u);
	r2 = OBJECT(r20)->unk_34;
	r3 = OBJECT(r20)->unk_36;
	t1 = AS32(OBJECT(r20)->unk_00);
	AS32(OBJECT(r20)->unk_00) = t1 | 0x20u;
	if ((t1 & 0x20u) == 0u)
	{
		r4 = (uint16_t)r0;
		r5 = r1;
		r6 = r2;
		r7 = r3;
	}
	else
	{
		r7 = mem_read16(r28);
		r28 = r28 + 2u;
		r6 = mem_read16(r28);
		r28 = r28 + 2u;
		r5 = (int16_t)mem_read16(r28);
		r28 = r28 + 2u;
		r4 = (uint16_t)((int32_t)((int16_t)mem_read16(r28) + r0) >> 1);
		r5 = (int32_t)(r5 + r1) >> 1;
		r6 = (r6 + r2) >> 1;
		r7 = (r7 + r3) >> 1;
	}
	OBJECT(r20)->unk_50 = r27;
	mem_write16(r27, r3);
	r27 = r27 + 2u;
	mem_write16(r27, r2);
	r27 = r27 + 2u;
	mem_write16(r27, r1);
	r27 = r27 + 2u;
	mem_write16(r27, r0);
	r27 = r27 + 2u;
	r27 = r27 + 8u;
	r27 = (r27 & ~0xFFFFu) | (r27 & 0xEFFFu);
	mem_write32(r25 + 0xB4u, r27);
	r26 = OBJECT(r20)->unk_38;
	r12 = rom_034138[r26];
	r26 = OBJECT(r20)->unk_40;
	r12 = mem_read32(r12 + 4u + r26 * 4u);
	r26 = OBJECT(r20)->unk_3A;
	r13 = rom_032BA4[r26];
	r19 = AS16(OBJECT(r20)->unk_00) & 0xC0u;
	r26 = r19;
	r26 = r26 >> 5;
	r18 = rom_00F4EC[r26];
	r28 = mem_read16(r12);
	r11 = r10 + 0x3000u;
	r12 = r12 + 2u;
	do
	{
		r27 = (r27 & ~0xFFFFu) | mem_read16(r12);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 ^ r19);
		mem_write16(r10, r27);
		r10 += 2u;
		mem_write16(r11, r27);
		r11 += 2u;
		r26 = (uint8_t)(OBJECT(r20)->unk_42 >> 8u);
		r26 = r26 * 3u;
		r12 = r12 + 2u;
		r26 = (uint16_t)(r26 + mem_read16(r12));
		r14 = 0x122B74u + r26 * 4u;
		t2 = ROM16(r14);
		r14 = r14 + 2u;
		r15 = t2;
		mem_write16(r10, r15);
		r10 += 2u;
		mem_write16(r11, r15);
		r11 += 2u;
		t3 = ROM16(r14);
		r14 = r14 + 2u;
		r26 = t3;
		t4 = ROM16(r14);
		r14 = r14 + 2u;
		r15_2 = t4;
		t5 = ROM16(r14);
		r14 = r14 + 2u;
		r9 = t5;
		mem_write16(r10, (((r15_2 * r3) >> 8) & 0x3FFu) | r9);
		r10 += 2u;
		mem_write16(r11, (((r15_2 * r7) >> 8) & 0x3FFu) | r9);
		r11 += 2u;
		t6 = ROM16(r14);
		r14 = r14 + 2u;
		r15_2 = t6;
		r16 = ((r15_2 * r6) >> 8) & 0x3FFu;
		t7 = ROM16(r14);
		r14 = r14 + 2u;
		r9 = t7;
		r15_2 = (((r15_2 * r2) >> 8) & 0x3FFu) | r9;
		r16 |= r9;
		r9 = mem_read16(r25 + 0x1Au);
		mem_write16(r10, r15_2 | r9);
		r10 += 2u;
		mem_write16(r11, r16 | r9);
		r11 += 2u;
		r12 = r12 + 2u;
		r15_2 = (int16_t)mem_read16(r12) * r18;
		mem_write16(r10, ((int32_t)(r15_2 * r3) >> 8) + r1);
		r10 += 2u;
		mem_write16(r11, ((int32_t)(r15_2 * r7) >> 8) + r5);
		r11 += 2u;
		r12 = r12 + 2u;
		r15_2 = (int16_t)mem_read16(r12) * r18;
		mem_write16(r10, ((int32_t)(r15_2 * r2) >> 8) + r0);
		r10 += 2u;
		mem_write16(r11, (uint32_t)((int32_t)(r15_2 * r6) >> 8) + r4);
		r11 += 2u;
		mem_write16(r10, r26);
		r10 += 2u;
		mem_write16(r11, r26);
		r11 += 2u;
		t8 = mem_read16(r13);
		r13 += 2u;
		r27 = (r27 & ~0xFFFFu) | t8;
		mem_write16(r10, r27);
		r10 += 2u;
		mem_write16(r11, r27);
		r11 += 2u;
		r28 = r28 - 1u;
		r12 = r12 + 2u;
	} while (r28 != 0u);
	*p_r0 = r0;
	*p_r10 = r10;
}

uint32_t sub_00F30D(uint32_t r7)
{
	uint32_t r7_2 = 0, r8 = 0, r9 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0;

	if (OBJECT(r20)->unk_41 == 0u)
		return r7;
	t1 = (uint8_t)OBJECT(r20)->unk_42;
	AS8(OBJECT(r20)->unk_42) = t1 - 1u;
	if ((int8_t)t1 > 1)
		return r7;
	AS8(OBJECT(r20)->unk_42) = OBJECT(r20)->unk_41;
	OBJECT(r20)->unk_40++;
	r9 = OBJECT(r20)->unk_40;
	r8 = ROM32(OBJECT(r20)->unk_38 * 2u * 2u + rom_034138_ADDRESS);
	if ((int32_t)mem_read32(r8 + 4u + r9 * 4u) > 0)
		return r7;
	t2 = 0u - mem_read32(r8 + 4u + r9 * 4u);
	r7_2 = rom_00F34C[t2];
	switch (t2)
	{
	case 0:
		OBJECT(r20)->unk_41 = 0u;
		OBJECT(r20)->unk_40--;
		return r7_2;
	case 2:
		r9 = mem_read32(r8 + 8u + r9 * 4u);
		sub_00F366(r9);
		return r7_2;
	case 1:
		OBJECT(r20)->unk_40 = 0u;
		return r7_2;
	default:
		invalid_jump(rom_00F34C[t2]);
	}
}

void sub_00F366(uint32_t r9)
{
	uint8_t r9_2 = 0;

	OBJECT(r20)->unk_38 = r9;
	r9_2 = mem_read8(rom_034138[r9]);
	OBJECT(r20)->unk_41 = r9_2;
	AS8(OBJECT(r20)->unk_42) = r9_2;
	AT8(OBJECT(r20)->unk_42, 1) = 0u;
	OBJECT(r20)->unk_34 = 0x100u;
	OBJECT(r20)->unk_36 = 0x100u;
	OBJECT(r20)->unk_40 = 0u;
}

void sub_00F396(void)
{
	uint16_t r3 = 0, r4 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	r9 = mem_read8(r25);
	r9 = (uint8_t)(r9 * 2u);
	r9 = r9 | (uint8_t)unk_20E020;
	r9 = (uint8_t)(r9 * 2u);
	r19 = rom_00F508[r9 | (uint8_t)(unk_20E020 >> 8u)];
	r10 = mem_read32(r19);
	r19 += 4u;
	if (mem_read16(r25 + 0xB0u) != 0u)
	{
		r11 = r25 + 0x5000u;
		r1 = 0x34u;
		r0 = 0u;
		r2 = 0u;
		r3 = 0x300u;
		r4 = 4u;
		do
		{
			if (mem_read16(r11) != 0u)
			{
				t1 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t1);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t2 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t2);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t3 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t3);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t4 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t4);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t5 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t5);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t6 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t6);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t7 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t7);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t8 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t8);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			r1--;
		} while (r1 != 0u);
	}
	mem_write16(r10, mem_read16(r19));
	mem_write16(r10 + 0x3000u, mem_read16(r19));
	r19 += 2u;
	mem_write16(SPRITE_RAM_400020, mem_read16(r19));
	r19 += 2u;
	mem_write16(SPRITE_RAM_400030, mem_read16(r19));
	r19 += 2u;
	unk_20E020 = mem_read16(r19);
	r19 += 2u;
	mem_write32(r25 + 0xC0u, mem_read32(r19));
	mem_write32(r25 + 0xC4u, mem_read32(r19 + 4u));
	mem_write16(r25 + 0xB0u, 0u);
}

void sub_00F5C8(uint32_t r7, uint32_t r15)
{
	uint16_t r3 = 0, r4_2 = 0, r5 = 0, r6 = 0, r21 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3_2 = 0, r4 = 0, r6_2 = 0, r8 = 0, r9 = 0, r10 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0, t3 = 0;

	r0 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	r1 = OBJECT(r20)->unk_58;
	r2 = mem_read16(mem_read32(r25 + 0x20u) + 0x46u);
	r3 = (uint16_t)(r2 + (mem_read16(r25 + 0x12u) >> 7));
	r4 = AS16(OBJECT(r20)->unk_10);
	if ((int16_t)r1 <= (int16_t)r2)
	{
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	}
	if ((int16_t)r3 <= (int16_t)r0)
		return;
	while ((int16_t)r0 < (int16_t)r2)
	{
		r0 = (uint16_t)(r0 + r4);
	}
	if ((int16_t)r3 < (int16_t)r1)
		r1 = r3;
	r3_2 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	AT16(OBJECT(r20)->unk_54, 2) = r0;
	r1 = (uint16_t)(r1 - r0);
	r1 = (uint16_t)(r1 + r4);
	r1 = (uint16_t)(r1 - 1u);
	t1 = divide_u16(r1, r4);
	r1 = t1;
	if ((int16_t)t1 <= 0)
		return;
	r28 = 0x100000u;
	r27 = mem_read32(r25 + 0xB4u);
	r21 = 0xEFFFu;
	t2 = AS32(OBJECT(r20)->unk_00);
	AS32(OBJECT(r20)->unk_00) = t2 | 0x20u;
	if ((t2 & 0x20u) != 0u)
	{
		r28 = OBJECT(r20)->unk_50;
		t3 = r3_2;
		r3_2 = t3 - r0;
		if (t3 != r0)
		{
			r28 = r28 - (divide_s32(r3_2, r4) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		}
	}
	OBJECT(r20)->unk_50 = r27;
	r18 = r25 + 0x2000u + ((r0 << 5) & 0xFFFu);
	r19 = r25 + 0x3000u + ((r0 - r2) << 4);
	r9 = r4 << 4;
	r8 = r4 << 5;
	r17 = r1;
	r12 = mem_read32(rom_034138[OBJECT(r20)->unk_38] + 4u + OBJECT(r20)->unk_40 * 4u) + 2u;
	r16 = (uint32_t)mem_read16(rom_032BA4[OBJECT(r20)->unk_3A]) << 16;
	r0 = AS16(OBJECT(r20)->unk_00);
	r0 = r0 & 0xC0u;
	r0 = r0 ^ mem_read16(r12);
	r12 = r12 + 2u;
	r1 = mem_read16(r12);
	r13 = 0x122B74u + r1 * 4u;
	r10 = mem_read32(r25 + 0xC0u) + 0x10u;
	AT8(OBJECT(r20)->unk_42, 1) = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		r3_2 = mem_read16(r19);
		if (r3_2 != 0u && r3_2 <= 0x1200u)
		{
			r6 = (uint16_t)((int32_t)(((uint32_t)mem_read16(r18 + 0x12u) * (int8_t)OBJECT(r20)->unk_3F + (int16_t)OBJECT(r20)->unk_20) * r3_2) >> 12);
			r5 = (uint16_t)(((int32_t)((int16_t)OBJECT(r20)->unk_24 * r3_2) >> 12) + mem_read16(r19 + 6u));
			if ((uint8_t)OBJECT(r20)->unk_5C == 0u)
				r6 += mem_read16(r19 + 4u);
			if ((int16_t)r6 > 0x1E0 || (int16_t)r6 < -0x1E0 || (int16_t)r5 > 0x80 || (int16_t)r5 < -0x160)
			{
				mem_write32(r27 + 4u, 0xF800F800u);
				r18 += r8;
				r19 += r9;
				r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
				r27 = (r27 & ~0xFFFFu) | (r27 & r21);
				r28 = (r28 & ~0xFFFFu) | (r28 & r21);
				r17--;
				if (r17 != 0u)
					continue;
				mem_write32(r25 + 0xC0u, r10);
				break;
			}
			r26 = OBJECT(r20)->unk_44;
			r3_2 = shift_by(r3_2, r26, 32);
			r4 = r3_2;
			if (r4 >= 0x1000u)
				r4 = 0xFFFu;
			r4 = r4 >> 5;
			r26 = OBJECT(r20)->unk_45;
			r3_2 = shift_by(r3_2, (uint8_t)(int8_t)mem_read8(rom_159AFC[r26] + r4), 32);
			r4 = mem_read8(rom_1598CC[r26] + r4);
			if (r4 != (uint8_t)(OBJECT(r20)->unk_42 >> 8u))
			{
				AT8(OBJECT(r20)->unk_42, 1) = r4;
				r4 = r13 + r4 * 3u * 4u;
				r0 = (r0 & 0xFFFFu) | ((uint32_t)ROM16(r4) << 16);
				r4 += 2u;
				r7 = ROM16(r4);
				r4 += 2u;
				r14 = ROM16(r4);
				r4 += 2u;
				r1 = ROM16(r4);
				r4 += 2u;
				r15 = ROM16(r4);
				r2 = ROM16(r4 + 2u) | mem_read16(r25 + 0x1Au);
				r7 |= r16;
			}
			r4_2 = (uint16_t)((r3_2 * r15) >> 8);
			r3_2 = ((r3_2 * r14) >> 8) & 0x3FFu;
			r4_2 &= 0x3FFu;
			if (mem_read32(r25 + 0xC4u) <= r10)
				break;
			mem_write32(r10 + 0x3000u, r0);
			mem_write32(r10, r0);
			mem_write16(r27 + 8u, r7);
			r10 += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)r7)
			{
				r26 = mem_read16(r28);
				mem_write16(r27, r3_2);
				r26 = (uint16_t)(r26 + r3_2);
				r26 = r26 >> 1;
				r26 = r26 | r1;
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r3_2 | r1);
				r26 = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, r4_2);
				r26 = (uint16_t)(r26 + r4_2);
				r26 = r26 >> 1;
				r26 = r26 | r2;
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r4_2 | r2);
				r26 = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, r5);
				r26 = (uint16_t)(r26 + r5);
				r26 = (uint16_t)((int16_t)r26 >> 1);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r5);
				r26 = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, r6);
				r26 = (uint16_t)(r26 + r6);
				r26 = (uint16_t)((int16_t)r26 >> 1);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			else
			{
				mem_write16(r27, r3_2);
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3000u, r3_2);
				mem_write16(r10, r3_2);
				mem_write16(r27 + 2u, r4_2);
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r4_2);
				mem_write16(r10, r4_2);
				mem_write16(r27 + 4u, r5);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r5);
				mem_write16(r10, r5);
				mem_write16(r27 + 6u, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r6);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			mem_write32(r10 + 0x3000u, r7);
			mem_write32(r10, r7);
			r4 = r25 + 0x5000u + mem_read16(r19 + 8u) * 4u;
			r10 += 4u;
			if (mem_read16(r4) == 0u)
				mem_write16(r4 + 2u, r10);
			r6_2 = mem_read16(r4);
			mem_write32(r10, r6_2);
			mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
			mem_write16(r4, ((r10 - 0x10u) >> 4) | 0x8000u);
			mem_write16(r25 + 0xB0u, mem_read16(r25 + 0xB0u) + 1u);
			r10 += 0x10u;
		}
		r18 += r8;
		r19 += r9;
		r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & r21);
		r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		r17--;
		if (r17 == 0u)
		{
			mem_write32(r25 + 0xC0u, r10);
			break;
		}
	}
	r26 = rom_100000;
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r25 + 0xB4u, r27);
}

void sub_00F911(uint32_t r7, uint32_t r15)
{
	uint16_t r3 = 0, r4_2 = 0, r5 = 0, r21 = 0, r30 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3_2 = 0, r4 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0, t3 = 0;

	r30 = 0x43u;
	r0 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	r1 = OBJECT(r20)->unk_58;
	r2 = mem_read16(mem_read32(r25 + 0x20u) + 0x46u);
	r3 = (uint16_t)(r2 + (mem_read16(r25 + 0x12u) >> 7));
	r4 = AS16(OBJECT(r20)->unk_10);
	if ((int16_t)r1 <= (int16_t)r2)
	{
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	}
	if ((int16_t)r3 <= (int16_t)r0)
		return;
	while ((int16_t)r0 < (int16_t)r2)
	{
		r0 = (uint16_t)(r0 + r4);
	}
	if ((int16_t)r3 < (int16_t)r1)
		r1 = r3;
	r3_2 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	AT16(OBJECT(r20)->unk_54, 2) = r0;
	r1 = (uint16_t)(r1 - r0);
	r1 = (uint16_t)(r1 + r4);
	r1 = (uint16_t)(r1 - 1u);
	t1 = divide_u16(r1, r4);
	r1 = t1;
	if ((int16_t)t1 <= 0)
		return;
	r28 = 0x100000u;
	r27 = mem_read32(r25 + 0xB4u);
	r21 = 0xEFFFu;
	t2 = AS32(OBJECT(r20)->unk_00);
	AS32(OBJECT(r20)->unk_00) = t2 | 0x20u;
	if ((t2 & 0x20u) != 0u)
	{
		r28 = OBJECT(r20)->unk_50;
		t3 = r3_2;
		r3_2 = t3 - r0;
		if (t3 != r0)
		{
			r28 = r28 - (divide_s32(r3_2, r4) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		}
	}
	OBJECT(r20)->unk_50 = r27;
	r18 = r25 + 0x2000u + ((r0 << 5) & 0xFFFu);
	r19 = r25 + 0x3000u + ((r0 - r2) << 4);
	r9 = r4 << 4;
	r8 = r4 << 5;
	r17 = r1;
	r12 = mem_read32(rom_034138[OBJECT(r20)->unk_38] + 4u + OBJECT(r20)->unk_40 * 4u) + 2u;
	r16 = (uint32_t)mem_read16(rom_032BA4[OBJECT(r20)->unk_3A]) << 16;
	r0 = AS16(OBJECT(r20)->unk_00);
	r0 = r0 & 0xC0u;
	r0 = r0 ^ mem_read16(r12);
	r12 = r12 + 2u;
	r1 = mem_read16(r12);
	r13 = 0x122B74u + r1 * 4u;
	r10 = mem_read32(r25 + 0xC0u) + 0x10u;
	AT8(OBJECT(r20)->unk_42, 1) = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		r3_2 = mem_read16(r19);
		if (r3_2 != 0u && r3_2 <= 0x1200u)
		{
			r6 = (int32_t)(((uint32_t)mem_read16(r18 + 0x12u) * (int8_t)OBJECT(r20)->unk_3F + (int16_t)OBJECT(r20)->unk_20) * r3_2) >> 12;
			r5 = (uint16_t)(((int32_t)((int16_t)OBJECT(r20)->unk_24 * r3_2) >> 12) + mem_read16(r19 + 6u));
			r26 = (r26 & ~0xFFFFu) | (uint16_t)r6;
			r6 = (r6 & ~0xFFFFu) | (uint16_t)(r6 + mem_read16(r19 + 4u));
			r6 = (((r6 << 16) | (r6 >> 16)) & ~0xFFFFu) | (uint16_t)(0u - r26);
			r6 = (r6 & ~0xFFFFu) | (uint16_t)(r6 + mem_read16(r19 + 4u));
			r26 = OBJECT(r20)->unk_44;
			r3_2 = shift_by(r3_2, r26, 32);
			r4 = r3_2;
			if (r4 >= 0x1000u)
				r4 = 0xFFFu;
			r4 = r4 >> 5;
			r26 = OBJECT(r20)->unk_45;
			r3_2 = shift_by(r3_2, (uint8_t)(int8_t)mem_read8(rom_159AFC[r26] + r4), 32);
			r4 = mem_read8(rom_1598CC[r26] + r4);
			if (r4 != (uint8_t)(OBJECT(r20)->unk_42 >> 8u))
			{
				AT8(OBJECT(r20)->unk_42, 1) = r4;
				r4 = r13 + r4 * 3u * 4u;
				r0 = (r0 & 0xFFFFu) | ((uint32_t)ROM16(r4) << 16);
				r4 += 2u;
				r7 = ROM16(r4);
				r4 += 2u;
				r14 = ROM16(r4);
				r4 += 2u;
				r1 = ROM16(r4);
				r4 += 2u;
				r15 = ROM16(r4);
				r2 = ROM16(r4 + 2u) | mem_read16(r25 + 0x1Au);
				r7 |= r16;
			}
			r4_2 = (uint16_t)((r3_2 * r15) >> 8);
			r3_2 = ((r3_2 * r14) >> 8) & 0x3FFu;
			r4_2 &= 0x3FFu;
			if (mem_read32(r25 + 0xC4u) <= r10)
				break;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r30);
			mem_write32(r10 + 0x3010u, r0);
			mem_write32(r10 + 0x10u, r0);
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r30);
			mem_write32(r10 + 0x3000u, r0);
			mem_write32(r10, r0);
			mem_write16(r27 + 8u, r7);
			r10 += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)r7)
			{
				r26 = mem_read16(r28);
				mem_write16(r27, r3_2);
				r26 = (uint16_t)(r26 + r3_2);
				r26 = r26 >> 1;
				r26 = r26 | r1;
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r3_2);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r3_2);
				r26 = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, r4_2);
				r26 = (uint16_t)(r26 + r4_2);
				r26 = r26 >> 1;
				r26 = r26 | r2;
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r4_2);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r4_2);
				r26 = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, r5);
				r26 = (uint16_t)(r26 + r5);
				r26 = (uint16_t)((int16_t)r26 >> 1);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r5);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r5);
				r26 = mem_read16(r28 + 0xAu);
				mem_write16(r27 + 0xAu, r6);
				r26 = (uint16_t)(r26 + r6);
				r26 = (uint16_t)((int16_t)r26 >> 1);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r6);
				r6 = (r6 << 16) | (r6 >> 16);
				r26 = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, r6);
				r26 = (uint16_t)(r26 + r6);
				r26 = (uint16_t)((int16_t)r26 >> 1);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			else
			{
				mem_write16(r27, r3_2);
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3010u, r3_2);
				mem_write16(r10 + 0x10u, r3_2);
				mem_write16(r10 + 0x3000u, r3_2);
				mem_write16(r10, r3_2);
				mem_write16(r27 + 2u, r4_2);
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r4_2);
				mem_write16(r10 + 0x10u, r4_2);
				mem_write16(r10 + 0x3000u, r4_2);
				mem_write16(r10, r4_2);
				mem_write16(r27 + 4u, r5);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r5);
				mem_write16(r10 + 0x10u, r5);
				mem_write16(r10 + 0x3000u, r5);
				mem_write16(r10, r5);
				mem_write16(r27 + 0xAu, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r6);
				mem_write16(r10 + 0x10u, r6);
				r6 = (r6 << 16) | (r6 >> 16);
				mem_write16(r27 + 6u, r6);
				mem_write16(r10 + 0x3000u, r6);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			mem_write32(r10 + 0x3010u, r7);
			mem_write32(r10 + 0x10u, r7);
			mem_write32(r10 + 0x3000u, r7);
			mem_write32(r10, r7);
			r10 += 0x14u;
			r4 = r25 + 0x5000u + mem_read16(r19 + 8u) * 4u;
			if (mem_read16(r4) == 0u)
				mem_write16(r4 + 2u, r10);
			r6 = mem_read16(r4);
			mem_write32(r10, r6);
			mem_write32(r10 + 0x3000u, (uint16_t)(r6 + 0x300u));
			mem_write16(r4, ((r10 - 0x20u) >> 4) | 0x8000u);
			mem_write16(r25 + 0xB0u, mem_read16(r25 + 0xB0u) + 2u);
			r10 += 0x10u;
		}
		r18 += r8;
		r19 += r9;
		r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & r21);
		r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		r17--;
		if (r17 == 0u)
		{
			mem_write32(r25 + 0xC0u, r10);
			break;
		}
	}
	r26 = rom_100000;
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r25 + 0xB4u, r27);
}

void sub_00FCBC(void)
{
	uint16_t r0 = 0, r5 = 0;
	uint32_t r11 = 0;

	move_string(0x100800u, 0x200u, r25 + 0x2000u, 0x200u, 2u, 0u, 0u, r26, &r27, &r28);
	r11 = rom_03CC00[mem_read8(r25 + 0xA0u)];
	mem_write32(r23 + 0xA0u, r11);
	r0 = mem_read8(r11 + 8u);
	AS8(OBJECT(r23)->unk_9C) = (uint8_t)r0;
	mem_write16(r23 + 0xBCu, r0 * 0xB0u);
	mem_write16(r23 + 0xACu, mem_read16(r25 + 0x12u) >> 7);
	r5 = 0xFFFu;
	do
	{
		sub_00FD78(r5);
	} while ((int16_t)mem_read16(r23 + 0xAAu) < (int16_t)mem_read16(r23 + 0xACu));
	mem_write8(r25 + 0xA4u, mem_read8(r25 + 0xA2u));
	mem_write8(r23 + 0xD1u, mem_read8(r25 + 0xA1u));
}

void sub_00FD22(void)
{
	uint8_t r1_2 = 0;
	uint16_t r0 = 0, r1 = 0, r5 = 0;
	uint32_t r0_2 = 0, r2 = 0, r10 = 0, r11 = 0, r21 = 0;

	r5 = 0xFFFu;
	if (mem_read8(r23 + 0xCBu) == 0u)
	{
		r0 = mem_read16(r23 + 0xA4u);
		AS32(OBJECT(r23)->unk_3C) = ((r25 + 0x2000u) & ~0xFFFFu) | (uint16_t)(r25 + 0x2000u + r0);
		mem_write16(r23 + 0xA8u, (r0 + 0xD00u) & 0xFFFu);
		if (mem_read16(r23 + 0xA6u) != mem_read16(r23 + 0xA8u))
		{
			do
			{
				if (mem_read8(r23 + 0xCBu) != 0u)
					break;
				sub_00FD78(r5);
			} while (mem_read16(r23 + 0xA6u) != mem_read16(r23 + 0xA8u));
		}
		return;
	}
	r0_2 = mem_read8(r23 + 0xCCu);
	if (mem_read8(r25 + 0x349u) < r0_2)
		return;
	r11 = mem_read32(r23 + 0xA0u);
	r0_2 = mem_read8(r25 + 0x349u + r0_2);
	mem_write8(r23 + 0xC9u, r0_2);
	r1_2 = mem_read8(r11 + r0_2);
	mem_write32(r23 + 0xA0u, mem_read32(r11 + 2u + r0_2 * 4u));
	mem_write8(r25 + 0xA1u, r1_2);
	mem_write8(r23 + 0xCAu, 0xFFu);
	mem_write8(r25 + 0xA5u, mem_read8(r25 + 0xA5u) + 1u);
	r2 = mem_read8(r23 + 0xD0u);
	mem_write8(r23 + 0xD7u + r2, r0_2);
	r2 = (uint8_t)(r2 + 1u);
	mem_write8(r23 + 0xD1u + r2, r1_2);
	mem_write8(r23 + 0xD0u, r2);
	mem_write8(r23 + 0xCBu, 0u);
	sub_0115B1();
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00CABC_ADDRESS);
	for (;;)
	{
		mem_write16(r23 + 0xC2u, mem_read16(r23 + 0xC2u) + 1u);
		r11 = mem_read32(r23 + 0xA0u);
		r0_2 = (int16_t)mem_read16(r11);
		r11 += 2u;
		if ((int16_t)r0_2 > 0)
			break;
		switch (0u - r0_2)
		{
		case 0:
			OBJECT(r20)->unk_0C = 3u;
			OBJECT(r20)->unk_0D = 0u;
			OBJECT(r20)->unk_0E = 0u;
			OBJECT(r20)->unk_0F = 0u;
			return;
		case 6:
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_00C8B6_ADDRESS);
			mem_write16(r21 + 0x1Cu, mem_read16(r23 + 0xAAu));
			mem_write32(r23 + 0xA0u, r11);
			continue;
		case 1:
			mem_write32(r23 + 0xA0u, mem_read32(r11));
			continue;
		case 2:
			r10 = mem_read32(r25 + 0x24u);
			mem_write8(r23 + 0xCCu, mem_read8(r23 + 0xCCu) + 1u);
			mem_write8(r10 + 0x60u, mem_read8(r10 + 0x60u) + 1u);
			mem_write32(r23 + 0xA0u, r11);
			mem_write8(r23 + 0xCBu, 0xFFu);
			return;
		case 3:
			mem_write32(r23 + 0xB0u, 0u);
			mem_write32(r23 + 0xA0u, r11 + 4u);
			continue;
		case 4:
			mem_write8(r25 + 0xA2u, mem_read8(r11));
			r11++;
			mem_write8(r25 + 0xA3u, mem_read8(r11));
			OBJECT(r23)->unk_46 = 0xFFFFu;
			mem_write32(r23 + 0xA0u, r11 + 1u);
			OBJECT(r23)->unk_48 = mem_read16(r23 + 0xAAu);
			continue;
		case 5:
			mem_write32(r23 + 0xB4u, 0u);
			mem_write32(r23 + 0xA0u, r11 + 4u);
			continue;
		default:
			invalid_jump(rom_00FF40[0u - r0_2]);
		}
	}
	mem_write16(r23 + 0xAEu, r0_2);
	mem_write16(r23 + 0xB8u, (uint32_t)(int8_t)mem_read8(r11) << 8);
	r11++;
	mem_write16(r23 + 0xBAu, (uint32_t)(int8_t)mem_read8(r11) << 8);
	r11++;
	r1 = mem_read8(r11);
	r11++;
	mem_write8(r23 + 0xCEu, mem_read8(r11));
	mem_write32(r23 + 0xA0u, r11 + 1u);
	AS8(OBJECT(r23)->unk_9C) = (uint8_t)r1;
	r1 = (uint16_t)(r1 * 0xB0u);
	mem_write16(r23 + 0xC0u, r1);
	mem_write16(r23 + 0xBEu, (int16_t)(r1 - mem_read16(r23 + 0xBCu)) >> 6);
	sub_00FD78(r5);
}

void sub_00FD78(uint16_t r5)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r10 = 0, r11 = 0, r21 = 0;
	uint16_t t1 = 0;

	for (;;)
	{
		t1 = mem_read16(r23 + 0xAEu);
		mem_write16(r23 + 0xAEu, t1 - 1u);
		if ((int16_t)t1 >= 1)
			break;
		for (;;)
		{
			mem_write16(r23 + 0xC2u, mem_read16(r23 + 0xC2u) + 1u);
			r11 = mem_read32(r23 + 0xA0u);
			r0 = (int16_t)mem_read16(r11);
			r11 += 2u;
			if ((int16_t)r0 > 0)
				break;
			switch (0u - r0)
			{
			case 0:
				OBJECT(r20)->unk_0C = 3u;
				OBJECT(r20)->unk_0D = 0u;
				OBJECT(r20)->unk_0E = 0u;
				OBJECT(r20)->unk_0F = 0u;
				return;
			case 6:
				r21 = sub_011D43();
				mem_write32(r21 + 4u, sub_00C8B6_ADDRESS);
				mem_write16(r21 + 0x1Cu, mem_read16(r23 + 0xAAu));
				mem_write32(r23 + 0xA0u, r11);
				continue;
			case 1:
				mem_write32(r23 + 0xA0u, mem_read32(r11));
				continue;
			case 2:
				r10 = mem_read32(r25 + 0x24u);
				mem_write8(r23 + 0xCCu, mem_read8(r23 + 0xCCu) + 1u);
				mem_write8(r10 + 0x60u, mem_read8(r10 + 0x60u) + 1u);
				mem_write32(r23 + 0xA0u, r11);
				mem_write8(r23 + 0xCBu, 0xFFu);
				return;
			case 3:
				mem_write32(r23 + 0xB0u, 0u);
				mem_write32(r23 + 0xA0u, r11 + 4u);
				continue;
			case 4:
				mem_write8(r25 + 0xA2u, mem_read8(r11));
				r11++;
				mem_write8(r25 + 0xA3u, mem_read8(r11));
				OBJECT(r23)->unk_46 = 0xFFFFu;
				mem_write32(r23 + 0xA0u, r11 + 1u);
				OBJECT(r23)->unk_48 = mem_read16(r23 + 0xAAu);
				continue;
			case 5:
				mem_write32(r23 + 0xB4u, 0u);
				mem_write32(r23 + 0xA0u, r11 + 4u);
				continue;
			default:
				invalid_jump(rom_00FF40[0u - r0]);
			}
		}
		mem_write16(r23 + 0xAEu, r0);
		mem_write16(r23 + 0xB8u, (uint32_t)(int8_t)mem_read8(r11) << 8);
		r11++;
		mem_write16(r23 + 0xBAu, (uint32_t)(int8_t)mem_read8(r11) << 8);
		r11++;
		r1_2 = mem_read8(r11);
		r11++;
		mem_write8(r23 + 0xCEu, mem_read8(r11));
		mem_write32(r23 + 0xA0u, r11 + 1u);
		AS8(OBJECT(r23)->unk_9C) = (uint8_t)r1_2;
		r1_2 = (uint16_t)(r1_2 * 0xB0u);
		mem_write16(r23 + 0xC0u, r1_2);
		mem_write16(r23 + 0xBEu, (int16_t)(r1_2 - mem_read16(r23 + 0xBCu)) >> 6);
	}
	AT8(OBJECT(r23)->unk_9C, 1) = 0xFFu;
	if (mem_read16(r23 + 0xBEu) == 0u)
		AT8(OBJECT(r23)->unk_9C, 1) = 0u;
	else if ((int16_t)mem_read16(r23 + 0xBEu) < 0)
	{
		mem_write16(r23 + 0xBCu, mem_read16(r23 + 0xBCu) + mem_read16(r23 + 0xBEu));
		if (mem_read16(r23 + 0xBCu) <= mem_read16(r23 + 0xC0u))
		{
			mem_write16(r23 + 0xBCu, mem_read16(r23 + 0xC0u));
			mem_write16(r23 + 0xBEu, 0u);
			AT8(OBJECT(r23)->unk_9C, 1) = 0u;
		}
	}
	else
	{
		mem_write16(r23 + 0xBCu, mem_read16(r23 + 0xBCu) + mem_read16(r23 + 0xBEu));
		if (mem_read16(r23 + 0xC0u) <= mem_read16(r23 + 0xBCu))
		{
			mem_write16(r23 + 0xBCu, mem_read16(r23 + 0xC0u));
			mem_write16(r23 + 0xBEu, 0u);
			AT8(OBJECT(r23)->unk_9C, 1) = 0u;
		}
	}
	mem_write32(r23 + 0xB4u, (((uint32_t)(int16_t)mem_read16(r23 + 0xBAu) << 8) + mem_read32(r23 + 0xB4u)) & 0xFFFFFFFu);
	mem_write32(r23 + 0xB0u, (((uint32_t)(int16_t)mem_read16(r23 + 0xB8u) << 8) + mem_read32(r23 + 0xB0u)) & 0xFFFFFFFu);
	r2 = mem_read16(r23 + 0xB2u);
	r4 = mem_read16(r23 + 0xB6u);
	r1 = (int16_t)rom_100C00[r2];
	r2 = (int16_t)rom_101400[r2];
	r3 = (int16_t)rom_100C00[r4];
	r4 = (int16_t)rom_101400[r4];
	OBJECT(r23)->unk_90 += (int32_t)(r3 * r2) >> 13;
	OBJECT(r23)->unk_94 -= r1 << 1;
	OBJECT(r23)->unk_98 += (int32_t)(r2 * r4) >> 13;
	OBJECT(r23)->unk_80 = OBJECT(r23)->unk_90;
	OBJECT(r23)->unk_80 = (int32_t)OBJECT(r23)->unk_80 >> 8;
	OBJECT(r23)->unk_84 = OBJECT(r23)->unk_94;
	OBJECT(r23)->unk_84 = (int32_t)OBJECT(r23)->unk_84 >> 8;
	OBJECT(r23)->unk_88 = OBJECT(r23)->unk_98;
	OBJECT(r23)->unk_88 = (int32_t)OBJECT(r23)->unk_88 >> 8;
	r11 = ((r25 + 0x2000u) & ~0xFFFFu) | (uint16_t)(r25 + 0x2000u + mem_read16(r23 + 0xA6u));
	mem_write16(r11, mem_read16(r23 + 0xB6u));
	mem_write16(r11 + 2u, mem_read16(r23 + 0xB2u));
	mem_write32(r11 + 4u, OBJECT(r23)->unk_80);
	mem_write32(r11 + 8u, OBJECT(r23)->unk_88);
	mem_write32(r11 + 0xCu, OBJECT(r23)->unk_84);
	mem_write8(r11 + 0x15u, mem_read8(r23 + 0xCEu));
	r1_2 = mem_read16(r23 + 0xBCu);
	mem_write16(r11 + 0x10u, r1_2);
	mem_write16(r11 + 0x12u, r1_2 >> 1);
	if ((uint8_t)(OBJECT(r23)->unk_9C >> 8u) == 0u)
		mem_write8(r11 + 0x14u, (uint8_t)OBJECT(r23)->unk_9C);
	else
		mem_write8(r11 + 0x14u, 0u);
	mem_write16(r23 + 0xA6u, (r11 + 0x20u) & r5);
	mem_write16(r23 + 0xAAu, mem_read16(r23 + 0xAAu) + 1u);
	OBJECT(r23)->unk_46++;
}

void sub_010062(void)
{
	uint32_t carry = 0;
	uint16_t r5 = 0, r21 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5_2 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r29 = 0;
	uint8_t t1 = 0;
	uint16_t t4 = 0;
	uint32_t t2 = 0, t3 = 0;

	r1 = mem_read16(mem_read32(r25 + 0xD0u) + 0xCu);
	r2 = mem_read16(mem_read32(r25 + 0xD0u) + 0xEu);
	r16 = (int16_t)rom_100C00[r1];
	r15 = (int16_t)rom_100C00[r2];
	r13 = (int16_t)rom_101400[r1];
	r10 = (int16_t)rom_101400[r2];
	r8 = 0xFFFFFFF2u;
	r11 = 0u - r15;
	r12 = r16 * r15;
	carry = (r12 >> 13) & 1u;
	r12 = ((int32_t)r12 >> 14) + carry;
	r14 = r16 * r10;
	carry = (r14 >> 13) & 1u;
	r14 = ((int32_t)r14 >> 14) + carry;
	r15 = r15 * r13;
	carry = (r15 >> 13) & 1u;
	r15 = ((int32_t)r15 >> 14) + carry;
	r16 = 0u - r16;
	r17 = r13 * r10;
	carry = (r17 >> 13) & 1u;
	r17 = ((int32_t)r17 >> 14) + carry;
	r0 = r23 + 0xDCu;
	mem_write32(r0, r10);
	r0 += 4u;
	mem_write32(r0, 0u);
	r0 += 4u;
	mem_write32(r0, r11);
	r0 += 4u;
	mem_write32(r0, r12);
	r0 += 4u;
	mem_write32(r0, r13);
	r0 += 4u;
	mem_write32(r0, r14);
	r0 += 4u;
	mem_write32(r0, r15);
	r0 += 4u;
	mem_write32(r0, r16);
	mem_write32(r0 + 4u, r17);
	r0 = 0u - mem_read32(mem_read32(r25 + 0xD0u));
	r1 = 0u - mem_read32(mem_read32(r25 + 0xD0u) + 4u);
	r2 = 0u - mem_read32(mem_read32(r25 + 0xD0u) + 8u);
	r3 = mem_read16(r25 + 0x10u);
	r3 = r3 >> 1;
	r4 = mem_read16(r25 + 0x12u);
	r5 = (uint16_t)(0u - mem_read16(r25 + 0x16u));
	r18 = r25 + 0x1FE0u + mem_read16(mem_read32(r25 + 0xD0u) + 0x10u);
	r19 = r25 + 0x2FF0u;
	r9 = r4 >> 7;
	mem_write8(r25 + 0x1F00u, 0u);
	r21 = 0x7FFFu;
	mem_write16(r25 + 0x1F30u, r5 + 0xE0u);
	r29 = 0x10509u;
	for (;;)
	{
		r18 = ((r18 + 0x20u) & ~0xFFFFu) | ((r18 + 0x20u) & 0xAFFFu);
		t1 = ROM8(r29);
		r29++;
		r19 += 0x10u;
		if (t1 == 0u)
		{
			r26 = r19 - 0x10u;
			t2 = mem_read32(r26 + 4u);
			mem_write32(r19, mem_read32(r26));
			mem_write32(r19 + 4u, t2);
			r26 = r26 + 8u;
			t3 = mem_read32(r26 + 4u);
			mem_write32(r19 + 8u, mem_read32(r26));
			mem_write32(r19 + 0xCu, t3);
			mem_write16(r19 + 0xCu, mem_read16(r18 + 0x14u));
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				sub_010243();
				return;
			}
			continue;
		}
		r26 = r0;
		r26 = r26 + mem_read32(r18 + 4u);
		r27 = r1;
		r27 = r27 + mem_read32(r18 + 0xCu);
		r28 = r2;
		r28 = r28 + mem_read32(r18 + 8u);
		r7 = r28;
		r28 = r28 * r17;
		r28 = r28 + r26 * r15;
		r28 = r28 + r27 * r16;
		r28 = r28 + r8;
		r28 = (int32_t)r28 >> 14;
		r27 = r27 * r13;
		r27 = r27 + r26 * r12;
		r27 = r27 + r7 * r14;
		r27 = r27 + r8;
		r27 = (int32_t)r27 >> 14;
		r26 = r26 * r10;
		r26 = r26 + r7 * r11;
		r26 = r26 + r8;
		r26 = (int32_t)r26 >> 14;
		if ((int32_t)r4 <= (int32_t)r28 || (int32_t)r28 <= (int32_t)r3)
		{
			mem_write16(r19, 0u);
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				r27 = (r27 & ~0xFFFFu) | r21;
				r28 = (r28 & ~0xFFFFu) | 3u;
				sub_010243();
				return;
			}
			continue;
		}
		r5_2 = rom_19A340[r28];
		mem_write16(r19, r5_2);
		r26 = r26 * r5_2;
		r27 = r27 * r5_2;
		carry = (r26 >> 11) & 1u;
		r26 = (int32_t)r26 >> 12;
		r26 = r26 + carry;
		carry = (r27 >> 11) & 1u;
		r27 = (int32_t)r27 >> 12;
		r27 = r27 + carry;
		r5_2 = r5_2 * mem_read16(r18 + 0x10u);
		carry = (r5_2 >> 11) & 1u;
		mem_write16(r19 + 2u, ((int32_t)r5_2 >> 12) + carry);
		mem_write16(r19 + 0xCu, mem_read16(r18 + 0x14u));
		mem_write16(r19 + 4u, r26);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(0u - r27);
		mem_write16(r19 + 6u, r27);
		r28 = rom_1A0340[r28];
		mem_write16(r19 + 8u, r28);
		t4 = r21;
		r21 = (uint16_t)r27;
		if ((int16_t)t4 < (int16_t)r27)
		{
			sub_010243();
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				sub_010243();
				return;
			}
			continue;
		}
		r9--;
		if (r9 == 0u)
			break;
	}
	mem_write16(r19 + 0x10u, 0u);
	sub_010243();
}

void sub_010243(void)
{
	uint32_t r7 = 0;

	r7 = mem_read8(r25 + 0x1F00u);
	if (r7 == 0u || (int16_t)r27 < (int16_t)mem_read16(r25 + 0x1F30u + r7 * 2u))
	{
		mem_write8(r25 + 0x1F00u, mem_read8(r25 + 0x1F00u) + 1u);
		mem_write16(r25 + 0x1F10u + r7 * 2u, r28);
		mem_write16(r25 + 0x1F12u + r7 * 2u, 3u);
		mem_write16(r25 + 0x1F32u + r7 * 2u, r27);
	}
}

void sub_01028A(uint32_t r0, uint32_t r3, uint32_t r4, uint32_t *p_r1, uint32_t *p_r2)
{
	uint32_t r1 = *p_r1;
	uint32_t r2 = *p_r2;
	uint32_t r3_2 = 0, r4_2 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0;

	r5 = (int16_t)rom_100C00[r4];
	r6 = (int16_t)rom_101400[r4];
	r4_2 = (int16_t)rom_101400[r3];
	r3_2 = (int16_t)rom_100C00[r3];
	r7 = r0;
	r9 = 0u - r1;
	r1 = (int32_t)(r1 * r4_2 + ((int32_t)(r7 * r3_2) >> 14) * r5 + ((int32_t)(r2 * r3_2) >> 14) * r6) >> 14;
	r2 = (int32_t)(((int32_t)(r2 * r4_2) >> 14) * r6 + (r9 * r3_2 + ((int32_t)(r7 * r4_2) >> 14) * r5)) >> 14;
	*p_r1 = r1;
	*p_r2 = r2;
}

uint32_t sub_010634(void)
{
	bool L_013DB6 = false;
	uint16_t r8 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	r1 = r25 >> 16;
	r1 = r1 & 1u;
	mem_write8(r25, r1);
	r2 = rom_010BA0[r1];
	mem_write16(r25 + 0x1Au, mem_read16(r2));
	r2 += 2u;
	mem_write32(r25 + 4u, mem_read32(r2));
	r2 += 4u;
	mem_write32(r25 + 8u, mem_read32(r2));
	mem_write32(r25 + 0xCu, mem_read32(r2 + 4u));
	mem_write16(r25 + 0x12u, mem_read16(rom_010BC8 + 2u));
	mem_write16(r25 + 0x10u, rom_010BCC);
	mem_write16(r25 + 0x14u, rom_010BCE);
	mem_write16(r25 + 0x16u, rom_010BD0);
	mem_write16(r25 + 0x18u, rom_010BD2[0]);
	r10 = rom_03CC00_ADDRESS;
	r11 = r25 + 0x700u;
	r9 = 0x1Fu;
	do
	{
		r12 = ROM32(r10);
		r0 = 0u;
		r8 = 0u;
		r10 += 4u;
		for (;;)
		{
			r8 += r0;
			for (;;)
			{
				r0 = mem_read16(r12);
				r12 = r12 + 6u;
				if ((int16_t)r0 >= 0)
					break;
				t1 = (uint16_t)~r0;
				r0 = rom_013D94[t1];
				switch (t1)
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
					invalid_jump(rom_013D94[t1]);
				}
				if (L_013DB6)
					break;
			}
			if (L_013DB6)
				break;
		}
		L_013DB6 = false;
		mem_write16(r11, r8);
		r9--;
		r11 += 2u;
	} while (r9 != 0u);
	mem_write16(r11, 0x14Au);
	return r0;
}

void sub_010688(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = r25 - 0x8000u;
	r0 = 0x38u;
	r1 = 0x200u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, r26, &r27, &r28);
		r10 += 0x400u;
		r0--;
	} while (r0 != 0u);
}

uint32_t sub_0106AF(void)
{
	sub_010741();
	sub_0117E4();
	mem_write16(VIDEO_31FF00, rom_010B10);
	unk_20E200 = rom_010B10;
	if (unk_20E60C != 0u)
	{
		mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) | 0x200u);
		unk_20E200 |= 0x200u;
	}
	move_halfwords(unk_20E290_ADDRESS, 0x100800u, 0x10u);
	move_halfwords(VIDEO_RAM + 0x1FF02u, 0x10B12u, 0x47u);
	move_string(0x10B12u, 0x47u, 0x20E202u, 0x47u, 2u, 0u, 0u, r26, &r27, &r28);
	return sub_01070C();
}

uint32_t sub_01070C(void)
{
	uint32_t r0 = 0;
	uint32_t t1 = 0, t2 = 0;

	mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) & 0xFFFu);
	unk_20E200 &= 0xFFFu;
	r0 = mem_read8(r25);
	unk_20E240[r0] = 0u;
	unk_20E248[r0] = 0u;
	t1 = unk_20E290_ADDRESS + r0 * 8u;
	mem_write32(t1, 0u);
	mem_write32(t1 + 4u, 0u);
	t2 = unk_20E2A0_ADDRESS + r0 * 8u;
	mem_write32(t2, 0u);
	mem_write32(t2 + 4u, 0u);
	return r0;
}

void sub_010741(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = 0x300000u;
	r0 = 0x1FFu;
	r1 = 0x80u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, r26, &r27, &r28);
		r10 += 0x100u;
		r0--;
	} while (r0 != 0u);
}

void sub_01076A(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = TEXT_CELL(0, 0);
	r0 = 0xFu;
	r1 = 0x80u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, r26, &r27, &r28);
		r10 += 0x100u;
		r0--;
	} while (r0 != 0u);
}

void sub_0107BC(void)
{
	uint16_t r0 = 0;
	uint32_t r18 = 0, r19 = 0;

	mem_write16(SPRITE_CONTROL_500000, rom_010B0A);
	r18 = SPRITE_CONTROL + 0x4u;
	r0 = rom_010B0B;
	r19 = 0x10B0Cu;
	if (unk_20E60C != 0u)
		r0 = r0 ^ 3u;
	mem_write16(r18, r0);
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	r19++;
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	r19++;
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	mem_write16(r18 + 2u, ROM8(r19 + 1u));
	sub_0107F0();
}

void sub_0107F0(void)
{
	move_string(0x10BF4u, 0x28u, 0x400000u, 0x28u, 2u, 0u, 0u, r26, &r27, &r28);
	unk_2080C0 = SPRITE_RAM_404000;
	unk_2180C0 = SPRITE_RAM_414000;
	unk_2080B4 = 0x20C000u;
	unk_2180B4 = 0x21C000u;
	mem_write16(SPRITE_RAM_404000, 0xFFFFu);
	mem_write16(SPRITE_RAM_407000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40D000, 0xFFFFu);
	mem_write16(SPRITE_RAM_414000, 0xFFFFu);
	mem_write16(SPRITE_RAM_417000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41D000, 0xFFFFu);
	unk_20E022 = 1u;
	unk_20E020 = 0u;
	unk_2080B0 = 0u;
	unk_2180B0 = 0u;
}

void sub_010889(void)
{
	mem_write16(r25 + 0xB0u, 0u);
}

void sub_010890(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0, r11 = 0;

	r11 = mem_read32(r25 + 8u);
	move_halfwords_backward(r11, 0x10AB6u, 0x28u);
	move_halfwords(r25 + 0x160u, 0x10AB6u, 0x28u);
	move_string(0x10AB6u, 0x28u, r25 + 0x1B0u, 0x28u, 2u, 0u, 0u, r26, &r27, &r28);
	r10 = 0x10B06u + mem_read8(r25) * 2u;
	r0 = ROM8(r10);
	r1 = ROM8(r10 + 1u);
	mem_write16(r11 + r0 * 2u, 0u);
	mem_write16(r11 + r1 * 2u, 0u);
	mem_write16(r25 + 0x160u + r0 * 2u, 0u);
	mem_write16(r25 + 0x160u + r1 * 2u, 0u);
	mem_write16(r25 + 0x1B0u + r0 * 2u, 0u);
	mem_write16(r25 + 0x1B0u + r1 * 2u, 0u);
}

void sub_0108EA(void)
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

void sub_010936(void)
{
	move_halfwords(SPRITE_RAM + 0x10000u, 0x12297Cu, 0xF0u);
	move_halfwords(SPRITE_RAM + 0x1000u, 0x107020u, 0x1200u);
	move_string(0x10C44u, 0xC0u, SPRITE_RAM + 0x3400u, 0xC0u, 2u, 0u, 0u, r26, &r27, &r28);
}

void sub_010DC4(uint32_t r10, uint32_t r11)
{
	uint32_t carry = 0;
	uint8_t r0 = 0, r3_2 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0;
	uint32_t t1 = 0;

	r0 = mem_read8(r10);
	r1 = 9u;
	r10++;
	for (;;)
	{
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			mem_write8(r11, mem_read8(r10));
			r11++;
			r10++;
			continue;
		}
		r2 = 0xFFFFFFFFu;
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			r2 = (r2 & ~0xFFFFu) | mem_read16(r10);
			r3_2 = (uint8_t)r2;
			r2 = (int32_t)r2 >> 3;
			t1 = r3_2 & 7u;
			r3 = t1;
			r10 += 2u;
			if (t1 != 0u)
				r3 += 2u;
			else
			{
				r3 = mem_read8(r10);
				r10++;
				if (r3 == 0u)
					break;
				r3++;
			}
		}
		else
		{
			r3 = 0u;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			r3 = r3 * 2u + carry;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			r3 = r3 * 2u + carry;
			r2 = (r2 & ~0xFFu) | mem_read8(r10);
			r10++;
			r3 += 2u;
		}
		r2 += r11;
		do
		{
			mem_write8(r11, mem_read8(r2));
			r11++;
			r3--;
			r2++;
		} while (r3 != 0u);
	}
}

uint32_t sub_010E42(uint32_t r2)
{
	bool L_010F01 = false;
	uint32_t carry = 0;
	uint8_t r0 = 0, r3 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2_2 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0;

	do
	{
		if (L_010F01 || (int8_t)mem_read8(r25 + 0xB01u) <= 0)
		{
			if (!L_010F01)
			{
				if (mem_read8(r25 + 0xB01u) != 0u)
				{
					r10 = mem_read32(r25 + 0xB0Cu);
					r11 = mem_read32(r25 + 0xB08u);
					mem_write8(r11, mem_read8(r10));
					mem_write32(r25 + 0xB0Cu, r10 + 1u);
					mem_write32(r25 + 0xB08u, r11 + 1u);
					t1 = mem_read8(r25 + 0xB10u);
					mem_write8(r25 + 0xB10u, t1 - 1u);
					if (t1 == 1u)
					{
						mem_write8(r25 + 0xB01u, 1u);
						return r2;
					}
					return r2;
				}
			}
			L_010F01 = false;
			r0_2 = mem_read8(r25 + 0xB02u);
			if (r0_2 == mem_read8(r25 + 0xB03u))
			{
				mem_write16(r25 + 0xB00u, 0u);
				return r2;
			}
			r11 = r25 + 0xC00u + r0_2 * 2u * 4u;
			mem_write8(r25 + 0xB02u, (r0_2 + 1u) & 0x3Fu);
			r10 = mem_read32(r11);
			r11 = mem_read32(r11 + 4u);
			r0 = mem_read8(r10);
			r1 = 9u;
			mem_write8(r25 + 0xB01u, 1u);
			r10++;
		}
		else
		{
			r0 = mem_read8(r25 + 0xB11u);
			r1 = mem_read8(r25 + 0xB12u);
			r10 = mem_read32(r25 + 0xB04u);
			r11 = mem_read32(r25 + 0xB08u);
		}
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry == 0u)
		{
			r2_2 = 0xFFFFFFFFu;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			if (carry)
			{
				r2 = (r2_2 & ~0xFFFFu) | mem_read16(r10);
				r3 = (uint8_t)r2;
				r2 = (int32_t)r2 >> 3;
				t2 = r3 & 7u;
				r3 = (uint8_t)t2;
				r10 += 2u;
				if (t2 != 0u)
				{
					r3++;
					r2 += r11;
					mem_write8(r11, mem_read8(r2));
					mem_write8(r25 + 0xB11u, r0);
					mem_write8(r25 + 0xB12u, r1);
					r2++;
					mem_write32(r25 + 0xB0Cu, r2);
					mem_write8(r25 + 0xB10u, r3);
					mem_write32(r25 + 0xB04u, r10);
					mem_write32(r25 + 0xB08u, r11 + 1u);
					mem_write8(r25 + 0xB01u, 0xFFu);
					return r2;
				}
				r3 = mem_read8(r10);
				r10++;
				if (r3 != 0u)
				{
					r2 += r11;
					mem_write8(r11, mem_read8(r2));
					mem_write8(r25 + 0xB11u, r0);
					mem_write8(r25 + 0xB12u, r1);
					r2++;
					mem_write32(r25 + 0xB0Cu, r2);
					mem_write8(r25 + 0xB10u, r3);
					mem_write32(r25 + 0xB04u, r10);
					mem_write32(r25 + 0xB08u, r11 + 1u);
					mem_write8(r25 + 0xB01u, 0xFFu);
					return r2;
				}
				L_010F01 = true;
			}
			if (!L_010F01)
			{
				r3 = 0u;
				r1--;
				if (r1 == 0u)
				{
					r0 = mem_read8(r10);
					r1 = 8u;
					r10++;
				}
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r3 = (uint8_t)(r3 * 2u + carry);
				r1--;
				if (r1 == 0u)
				{
					r0 = mem_read8(r10);
					r1 = 8u;
					r10++;
				}
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r3 = (uint8_t)(r3 * 2u + carry);
				r2 = (r2_2 & ~0xFFu) | mem_read8(r10);
				r10++;
				r3++;
				r2 += r11;
				mem_write8(r11, mem_read8(r2));
				mem_write8(r25 + 0xB11u, r0);
				mem_write8(r25 + 0xB12u, r1);
				r2++;
				mem_write32(r25 + 0xB0Cu, r2);
				mem_write8(r25 + 0xB10u, r3);
				mem_write32(r25 + 0xB04u, r10);
				mem_write32(r25 + 0xB08u, r11 + 1u);
				mem_write8(r25 + 0xB01u, 0xFFu);
				return r2;
			}
		}
	} while (L_010F01);
	mem_write8(r11, mem_read8(r10));
	mem_write8(r25 + 0xB11u, r0);
	mem_write8(r25 + 0xB12u, r1);
	mem_write32(r25 + 0xB04u, r10 + 1u);
	mem_write32(r25 + 0xB08u, r11 + 1u);
	return r2;
}

void sub_010FA1(uint32_t r10)
{
	uint32_t r0 = 0, r10_2 = 0, r11 = 0;
	uint32_t t1 = 0;

	while ((int32_t)mem_read32(r10) >= 0)
	{
		r0 = mem_read32(r10);
		r12 = r0;
		r0 &= ~0xFFFFFFu;
		r11 = 0x300000u + (byte_swap32(r0) << 8) * 4u;
		t1 = r10 + 4u;
		r10_2 = r12 ^ r0;
		r12 = t1;
		sub_010DC4(r10_2, r11);
		r10 = r12;
	}
}

void sub_011011(uint32_t r10)
{
	uint32_t r0 = 0, r1 = 0, r11 = 0;

	while ((int32_t)mem_read32(r10) >= 0)
	{
		r0 = mem_read8(r25 + 0xB03u);
		r11 = r25 + 0xC00u + r0 * 2u * 4u;
		mem_write8(r25 + 0xB03u, (r0 + 1u) & 0x3Fu);
		r0 = mem_read32(r10);
		r1 = r0;
		r0 &= ~0xFFFFFFu;
		mem_write32(r11, r1 ^ r0);
		mem_write32(r11 + 4u, 0x300000u + (byte_swap32(r0) << 8) * 4u);
		mem_write8(r25 + 0xB00u, 0xFFu);
		r10 += 4u;
	}
}

void sub_011018(void)
{
	uint8_t t1 = 0;

	t1 = mem_read8(0xC8000Au);
	unk_20E104 = ~t1 & 0xFu;
	unk_20E091 = 0u;
	if (unk_20E615 == 0u && unk_20E612 == unk_20E611)
		unk_20E091 = 0x14u;
	unk_20E0A0 = mem_read8(SOUND_701F74);
	unk_20E0A4 = mem_read8(SOUND_701F76);
	unk_20E0A8 = mem_read8(SOUND_701F78);
	unk_20E0AC = mem_read8(SOUND_701F7A);
}

void sub_01106C(uint32_t r6)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r6_2 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint32_t saved_r25 = 0;
	uint8_t t1 = 0;

	if (unk_20E615 != 0u)
		return;
	saved_r25 = r25;
	if (unk_20E08C != 0u)
	{
		r0 = 0x4Eu;
		r25 = GAME_VARIABLES;
		sub_011EDE(r0);
		unk_20E08C--;
	}
	if (unk_20E08D != 0u)
	{
		r0 = 0x4Eu;
		r25 = 0x218000u;
		sub_011EDE(r0);
		unk_20E08D--;
	}
	if (unk_20E08E != 0u)
	{
		r0 = 0x50u;
		r25 = GAME_VARIABLES;
		sub_011EDE(r0);
		unk_20E08E--;
	}
	if (unk_20E08F != 0u)
	{
		r0 = 0x50u;
		r25 = 0x218000u;
		sub_011EDE(r0);
		unk_20E08F--;
	}
	r25 = saved_r25;
	r6_2 = (r6 & ~0xFFu) | unk_20E102;
	r9 = 0u;
	if ((r6_2 & 1u) != 0u)
	{
		r10 = unk_20E0A0_ADDRESS;
		r11 = unk_20E08E_ADDRESS;
		r12 = GAME_VARIABLES;
		r9 = sub_01122C(r9, r10, r11);
		r7 = SOUND_RAM + 0x1F04u;
		sub_011324(r7, (uint16_t)r9);
	}
	r9 = 0u;
	if ((r6_2 & 2u) != 0u)
	{
		r10 = unk_20E0A8_ADDRESS;
		r11 = unk_20E08F_ADDRESS;
		r12 = 0x218000u;
		r9 = sub_01122C(r9, r10, r11);
		r7 = SOUND_RAM + 0x1F04u;
		sub_011324(r7, (uint16_t)r9);
	}
	r6_2 = (r6_2 & ~0xFFu) | unk_20E103;
	t1 = r6_2 & 0x3Cu;
	r6_2 = (r6_2 & ~0xFFu) | t1;
	if (t1 != 0u)
	{
		r9 = 0u;
		r12 = GAME_VARIABLES;
		r19 = 0x114D4u;
		r9 = sub_011252(r6_2, r9, r19);
		r19 = 0x11504u;
		r9 = sub_011252(r6_2, r9, r19);
		r12 = 0x218000u;
		r19 = 0x11534u;
		r9 = sub_011252(r6_2, r9, r19);
		r19 = 0x11564u;
		r9 = sub_011252(r6_2, r9, r19);
		r7 = SOUND_RAM + 0x1F02u;
		sub_011324(r7, (uint16_t)r9);
	}
	if (unk_20E091 == 0u)
	{
		r7 = 0x20E083u;
		r1 = 1u;
		r12 = unk_20E092_ADDRESS;
		sub_0111CB(r1, r7);
	}
	r7 = 0x20E080u;
	r1 = 0u;
	r12 = unk_20E092_ADDRESS;
	sub_0111CB(r1, r7);
	if (unk_20E091 == 0u)
	{
		r7 = 0x20E089u;
		r1 = 1u;
		r12 = 0x20E093u;
		sub_0111CB(r1, r7);
	}
	r7 = 0x20E086u;
	r1 = 0u;
	r12 = 0x20E093u;
	sub_0111CB(r1, r7);
}

void sub_0111CB(uint8_t r1, uint32_t r7)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	r0 = mem_read16(r7);
	r7 += 2u;
	if (r0 == 0u)
	{
		if (mem_read8(r7) != 0u)
		{
			mem_write8(r7, mem_read8(r7) - 1u);
			mem_write8(r12, mem_read8(r12) | shift_by(1u, (uint8_t)r1, 8));
			mem_write16(r7 - 2u, 0x606u);
		}
		r12 = SOUND_701F74;
		sub_011213();
		sub_011418(r0);
		r12 = SOUND_701FF0;
		sub_011213();
		return;
	}
	if ((uint8_t)r0 == 0u)
	{
		r7--;
		mem_write8(r7, mem_read8(r7) - 1u);
		r12 = SOUND_701F74;
		sub_011213();
		sub_011418(r0);
		r12 = SOUND_701FF0;
		sub_011213();
		return;
	}
	t1 = mem_read8(r7 - 2u);
	mem_write8(r7 - 2u, t1 - 1u);
	if (t1 != 1u)
	{
		r12 = SOUND_701F74;
		sub_011213();
		sub_011418(r0);
		r12 = SOUND_701FF0;
		sub_011213();
		return;
	}
	mem_write8(r12, mem_read8(r12) & rotate_by(0xFEu, (uint8_t)r1, 8));
	r12 = SOUND_701F74;
	sub_011213();
	sub_011418(r0);
	r12 = SOUND_701FF0;
	sub_011213();
}

void sub_011213(void)
{
	mem_write16(r12, AS16(unk_20E0A0));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(unk_20E0A4));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(unk_20E0A8));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(unk_20E0AC));
	r12 = r12 + 2u;
}

uint32_t sub_01122C(uint32_t r9, uint32_t r10, uint32_t r11)
{
	uint32_t zero = 0;
	uint8_t t1 = 0;

	mem_write8(r10, mem_read8(r10) + 1u);
	t1 = mem_read8(r10);
	zero = (uint8_t)(t1 - 9u) == 0u;
	if (t1 >= 9u)
	{
		mem_write8(r10 + 2u, 0u);
		mem_write8(r10 + 1u, 0u);
		if (zero == 0u)
		{
			mem_write8(r10, 9u);
			return r9;
		}
	}
	sub_0112D9();
	mem_write8(r11, 1u);
	r9 = (r9 & ~0xFFu) | (uint8_t)(r9 + 1u);
	return r9;
}

uint32_t sub_011252(uint32_t r6, uint32_t r9, uint32_t r19)
{
	uint8_t r1 = 0, r2_2 = 0, r3 = 0;
	uint32_t r2 = 0, r7 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0, t2 = 0;

	t1 = mem_read32(r19);
	r19 += 4u;
	if ((r6 & shl32(1u, (int)(t1))) == 0u)
		return r9;
	r11 = mem_read32(r19);
	r19 = r19 + 4u + unk_20E091;
	r10 = mem_read32(r19);
	mem_write8(IO_C00074, mem_read8(r19 + 0x10u));
	t2 = mem_read32(r19 + 4u);
	mem_write8(t2, mem_read8(t2) + 1u);
	r7 = mem_read32(r19 + 8u);
	sub_0112F0(r7);
	if (mem_read8(r10) >= 9u)
		return r9;
	mem_write8(r11, mem_read8(r11) + 1u);
	r1 = mem_read8(mem_read32(r19 + 0xCu));
	r2 = unk_20E613;
	if (r2 != 0u)
	{
		r3 = (uint8_t)(mem_read8(r10 + 2u) + r1);
		while (r2 <= r3)
		{
			mem_write8(r10 + 1u, mem_read8(r10 + 1u) + 1u);
			r3 -= r2;
		}
		mem_write8(r10 + 2u, r3);
	}
	r2_2 = unk_20E610;
	r3 = (uint8_t)(mem_read8(r10 + 1u) + r1);
	do
	{
		if (r3 < r2_2)
		{
			mem_write8(r10 + 1u, r3);
			return r9;
		}
		mem_write8(r10, mem_read8(r10) + 1u);
		sub_0112D9();
		r9 = (r9 & ~0xFFu) | (uint8_t)(r9 + 1u);
		r3 -= r2_2;
	} while (mem_read8(r10) < 9u);
	r3 = 0u;
	mem_write8(r10 + 2u, 0u);
	mem_write8(r10 + 1u, r3);
	return r9;
}

void sub_0112D9(void)
{
	if ((int8_t)mem_read8(r12 + 0x80u) > 0 && (int8_t)mem_read8(r12 + 0x80u) < 6)
		mem_write32(r12 + 0x80u, 2u);
}

void sub_0112F0(uint32_t r7)
{
	if (mem_read16(r7) != 0xFFFFu)
	{
		mem_write16(r7, mem_read16(r7) + 1u);
		sub_011418(0u);
		mem_write16(r7 + 0x7Cu, mem_read16(r7 + 0x7Cu) + 1u);
	}
}

void sub_011324(uint32_t r7, uint16_t r9)
{
	uint32_t carry = 0;
	uint16_t t1 = 0;

	t1 = mem_read16(r7);
	carry = (uint16_t)(t1 + r9) < t1;
	mem_write16(r7, t1 + r9);
	if (carry)
	{
		mem_write16(r7, 0xFFFFu);
		sub_011418(0u);
		mem_write16(r7, 0xFFFFu);
		return;
	}
	sub_011418(0u);
	mem_write16(r7 + 0x7Cu, mem_read16(r7 + 0x7Cu) + r9);
}

void sub_011341(uint32_t r7, uint32_t r9)
{
	uint32_t carry = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r7);
	carry = t1 + r9 < t1;
	mem_write32(r7, t1 + r9);
	if (carry)
	{
		mem_write32(r7, 0xFFFFFFFFu);
		sub_011418(0u);
		mem_write32(r7, 0xFFFFFFFFu);
		return;
	}
	sub_011418(0u);
	mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + r9);
}

void sub_01137D(uint32_t r7, uint16_t r9)
{
	mem_write16(r7, r9);
	sub_011418(0u);
	mem_write16(r7 + 0x7Cu, r9);
}

void sub_01139E(void)
{
	mem_write32(SOUND_701F74, 0u);
	mem_write32(SOUND_701F78, 0u);
	sub_011418(0u);
	mem_write32(SOUND_701FF0, 0u);
	mem_write32(SOUND_701FF4, 0u);
	r12 = SOUND_RAM + 0x1FF8u;
}

void sub_0113C0(void)
{
	uint16_t r3 = 0;
	uint32_t r0 = 0, r2 = 0, r3_2 = 0;

	r3 = mem_read16(SOUND_701F00);
	r2 = SOUND_RAM + 0x1F02u;
	r0 = sub_01143E(0u, r2);
	if ((uint16_t)r0 == r3)
	{
		r2 = SOUND_RAM + 0x1F02u;
		r3_2 = SOUND_RAM + 0x1F7Eu;
		r0 = sub_01140E(r0, r2, r3_2);
		return;
	}
	r2 = SOUND_RAM + 0x1F7Eu;
	r0 = sub_01143E(r0, r2);
	if ((uint16_t)r0 == r3)
	{
		r3_2 = SOUND_RAM + 0x1F02u;
		r2 = SOUND_RAM + 0x1F7Eu;
		r0 = sub_01140E(r0, r2, r3_2);
		return;
	}
	r0 = sub_0113E5(r0);
}

uint32_t sub_0113E5(uint32_t r0)
{
	uint32_t r2 = 0, r3 = 0;

	r2 = 0x11458u;
	r3 = SOUND_RAM + 0x1F02u;
	r0 = sub_01140E(r0, r2, r3);
	r2 = SOUND_RAM + 0x1F02u;
	r3 = SOUND_RAM + 0x1F7Eu;
	return sub_01140E(r0, r2, r3);
}

uint32_t sub_01140E(uint32_t r0, uint32_t r2, uint32_t r3)
{
	move_string(r2, 0x3Eu, r3, 0x3Eu, 2u, 0u, 0u, r26, &r27, &r28);
	return sub_011418(r0);
}

uint32_t sub_011418(uint32_t r0)
{
	uint32_t r2 = 0;

	r2 = SOUND_RAM + 0x1F02u;
	r0 = sub_01143E(r0, r2);
	mem_write16(SOUND_701F00, r0);
	return r0;
}

uint32_t sub_01143E(uint32_t r0, uint32_t r2)
{
	uint32_t carry = 0;
	uint32_t r0_2 = 0, r1 = 0;

	r0_2 = (r0 & ~0xFFFFu) | 1u;
	r1 = 0x3Eu;
	do
	{
		r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 ^ mem_read16(r2));
		carry = r0_2 & 1u;
		r0_2 = (r0_2 & ~0xFFFFu) | ((uint16_t)r0_2 >> 1);
		r2 += 2u;
		if (carry)
			r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 ^ 0x8810u);
		r1--;
	} while (r1 != 0u);
	return r0_2;
}

void sub_011594(void)
{
	uint32_t r19 = 0;

	r19 = 0x3153Cu;
	sub_011615(r19);
	r19 = 0x31740u;
	sub_011615(r19);
	r19 = 0x153904u;
	sub_011615(r19);
}

void sub_0115B1(void)
{
	uint32_t r19 = 0;

	r19 = rom_031DDC[mem_read8(r25 + 0xA1u)];
	sub_0115C0(r19);
}

void sub_0115C0(uint32_t r19)
{
	uint32_t r8 = 0;

	r8 = 0u;
	sub_0115CC(r8, r19);
}

void sub_0115C5(uint32_t r19)
{
	uint32_t r8 = 0;

	r8 = 0x80000000u;
	sub_0115CC(r8, r19);
}

void sub_0115CC(uint32_t r8, uint32_t r19)
{
	uint32_t r9 = 0, r18 = 0;

	r18 = r25 + 0x900u;
	if ((int32_t)mem_read32(r18) < 0x80)
	{
		r9 = mem_read32(r18) + 1u;
		mem_write32(r18 + r9 * 4u, r19 | r8);
		mem_write32(r18, r9);
	}
}

void sub_0115EB(void)
{
	uint32_t r0 = 0, r10 = 0, r19 = 0;
	uint32_t t1 = 0;

	r10 = r25 + 0x900u;
	r0 = mem_read32(r10);
	r10 += 4u;
	for (;;)
	{
		t1 = mem_read32(r10);
		r19 = t1 & 0x7FFFFFFFu;
		r10 += 4u;
		if ((t1 & 0x80000000u) == 0u)
		{
			sub_011615(r19);
			r0--;
			if (r0 == 0u)
				break;
			continue;
		}
		sub_011643(r19);
		r0--;
		if (r0 == 0u)
			break;
	}
	mem_write32(r25 + 0x900u, r0);
}

void sub_011615(uint32_t r19)
{
	uint32_t r9 = 0, r18 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	r18 = mem_read8(r25);
	r18 = (r18 << 19) | (r18 >> 13) | 0x600000u | mem_read16(r19);
	r19 += 2u;
	r9 = mem_read16(r19);
	r9 = r9 >> 5;
	r19 += 2u;
	do
	{
		t1 = r18;
		r18 += 8u;
		t2 = mem_read32(r19 + 4u);
		mem_write32(t1, mem_read32(r19));
		mem_write32(t1 + 4u, t2);
		r19 += 8u;
		t3 = r18;
		r18 += 8u;
		t4 = mem_read32(r19 + 4u);
		mem_write32(t3, mem_read32(r19));
		mem_write32(t3 + 4u, t4);
		r19 += 8u;
		t5 = r18;
		r18 += 8u;
		t6 = mem_read32(r19 + 4u);
		mem_write32(t5, mem_read32(r19));
		mem_write32(t5 + 4u, t6);
		r19 += 8u;
		t7 = r18;
		r18 += 8u;
		t8 = mem_read32(r19 + 4u);
		mem_write32(t7, mem_read32(r19));
		mem_write32(t7 + 4u, t8);
		r9--;
		r19 += 8u;
	} while (r9 != 0u);
}

void sub_011643(uint32_t r19)
{
	uint32_t r8 = 0, r9 = 0, r17 = 0, r18 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	r18 = mem_read8(r25);
	r18 = (r18 << 19) | (r18 >> 13) | 0x600000u;
	r9 = mem_read32(r19) >> 3;
	r19 += 4u;
	do
	{
		r17 = mem_read32(r19);
		r19 += 4u;
		r18 = (r18 & ~0xFFFFu) | mem_read16(r19);
		r19 += 2u;
		r8 = mem_read16(r19);
		r19 += 2u;
		do
		{
			t1 = mem_read32(r17 + 4u);
			mem_write32(r18, mem_read32(r17));
			mem_write32(r18 + 4u, t1);
			r18 += 8u;
			t2 = mem_read32(r17 + 0xCu);
			mem_write32(r18, mem_read32(r17 + 8u));
			mem_write32(r18 + 4u, t2);
			r18 += 8u;
			t3 = mem_read32(r17 + 0x14u);
			mem_write32(r18, mem_read32(r17 + 0x10u));
			mem_write32(r18 + 4u, t3);
			r18 += 8u;
			t4 = mem_read32(r17 + 0x1Cu);
			mem_write32(r18, mem_read32(r17 + 0x18u));
			mem_write32(r18 + 4u, t4);
			r8--;
			r18 += 8u;
		} while (r8 != 0u);
		r9--;
	} while (r9 != 0u);
}

void sub_01167E(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0;
	uint16_t r9 = 0;
	uint32_t r1 = 0, r19 = 0;

	switch (mem_read8(r25 + 0x153u))
	{
	case 0:
		r19 = 0x1173Au;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 1:
		r19 = 0x1173Eu;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 2:
		r19 = 0x11742u;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 3:
		r19 = 0x11746u;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 4:
		r19 = 0x1174Au;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 5:
		r19 = 0x1174Eu;
		mem_write8(r25 + 0x150u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x151u, ROM8(r19));
		r19++;
		mem_write8(r25 + 0x152u, ROM8(r19));
		mem_write8(r25 + 0x154u, ROM8(r19 + 1u));
		mem_write8(r25 + 0x153u, 6u);
		break;
	case 6:
		break;
	default:
		invalid_jump(rom_01168C[mem_read8(r25 + 0x153u)]);
	}
	r9 = (uint8_t)(mem_read8(r25 + 0x150u) + mem_read8(r25 + 0x151u)) & 0x3Fu;
	mem_write8(r25 + 0x150u, (uint8_t)r9);
	r19 = r25 + 0x19Fu;
	mem_write8(r19, 0xC0u);
	r0 = mem_read8(r25 + 0x154u);
	r1 = 3u;
	r19++;
	do
	{
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			mem_write16(r19, r9);
			r19 += 2u;
		}
		r1--;
	} while (r1 != 0u);
	if (r9 == mem_read8(r25 + 0x152u))
		mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) & ~1u);
}

void sub_0117E4(void)
{
	move_string(0x104020u, 0x1800u, VIDEO_RAM + 0x18000u, 0x1800u, 2u, 0u, 0u, r26, &r27, &r28);
}

void sub_011868(uint32_t r19)
{
	do
	{
		r19 = sub_011874(r19);
	} while (mem_read16(r19) != 0xFFFFu);
}

uint32_t sub_011874(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r19 += 2u;
	r9 = (uint16_t)((uint32_t)mem_read8(r19) << 8);
	r19++;
	r9 |= mem_read8(r19);
	r19++;
	if ((uint8_t)r9 != 0u)
	{
		do
		{
			mem_write16(r18, r9);
			r9 = (uint16_t)((r9 & ~0xFFu) | mem_read8(r19));
			r18 += 2u;
			r19++;
		} while ((uint8_t)r9 != 0u);
	}
	return r19;
}

uint32_t sub_011975(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0;

	r3 = 8u;
	r2 = 0u;
	do
	{
		r1 = 0u;
		divux_pair(0xAu, &r0, &r1);
		r2 += r1;
		r2 = (r2 << 28) | (r2 >> 4);
		r3--;
	} while ((r3 != 0u) & (r0 != 0u));
	if (r0 == 0u && r3 != 0u)
	{
		do
		{
			r2 = (r2 << 28) | (r2 >> 4);
			r3--;
		} while (r3 != 0u);
	}
	r0_2 = r2;
	return r0_2;
}

void sub_0119FA(uint32_t r3, uint16_t r5, uint32_t r18)
{
	uint16_t r4 = 0;
	uint32_t r1 = 0, r2 = 0, r10 = 0, r11 = 0;

	r10 = mem_read32(r25 + 0xC0u) + 0x10u;
	if (mem_read32(r25 + 0xC4u) <= r10)
		return;
	r11 = r10 + 0x3000u;
	r1 = rom_122B5C;
	r3 = (r3 << 16) | (r3 >> 16);
	r2 = rom_122B60 | ((uint32_t)mem_read16(r25 + 0x1Au) << 16);
	r4 = rom_122B64;
	r5 |= rom_122B66;
	r18 = sub_011A72(r1, r2, r3, r4, r5, r10, r11, r18);
}

uint32_t sub_011A32(uint32_t r19)
{
	uint16_t r4 = 0, r5 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0, r10 = 0, r11 = 0, r18 = 0;

	r18 = r19 + 6u;
	r10 = mem_read32(r25 + 0xC0u) + 0x10u;
	if (mem_read32(r25 + 0xC4u) <= r10)
		return r18;
	r11 = r10 + 0x3000u;
	r1 = rom_122B5C;
	r2 = rom_122B60 | ((uint32_t)mem_read16(r25 + 0x1Au) << 16);
	r3 = mem_read32(r19);
	r3 = (r3 << 16) | (r3 >> 16);
	r4 = rom_122B64;
	r5 = rom_122B66 | mem_read16(r19 + 4u);
	return sub_011A72(r1, r2, r3, r4, r5, r10, r11, r18);
}

uint32_t sub_011A72(uint32_t r1, uint32_t r2, uint32_t r3, uint16_t r4, uint16_t r5, uint32_t r10, uint32_t r11, uint32_t r18)
{
	uint16_t r0 = 0;
	uint32_t r1_2 = 0, r2_2 = 0, r3_2 = 0, r6 = 0;
	uint8_t t1 = 0;

	r0 = PLAYER(r18)->unk_00;
	r6 = 0x80000u;
	r18++;
	do
	{
		r0 = (uint16_t)(((uint32_t)r0 << 3) + r4);
		mem_write32(r10, r1);
		mem_write32(r11, r1);
		r11 += 4u;
		r10 += 4u;
		mem_write32(r10, r2);
		mem_write32(r11, r2);
		r11 += 4u;
		r10 += 4u;
		mem_write32(r10, r3);
		mem_write32(r11, r3);
		r11 += 4u;
		r10 += 4u;
		mem_write16(r10, r0);
		mem_write16(r11, r0);
		r11 += 2u;
		r10 += 2u;
		mem_write16(r10, r5);
		mem_write16(r11, r5);
		r11 += 2u;
		r3 += r6;
		t1 = mem_read8(r18);
		r18++;
		r0 = t1;
		r10 += 2u;
	} while (r0 != 0u);
	r1_2 = r25 + 0x5678u;
	r3_2 = mem_read16(r1_2);
	if (r3_2 == 0u)
		mem_write16(r1_2 + 2u, r10);
	r2_2 = mem_read32(r25 + 0xC0u) + 0x10u;
	mem_write32(r10, r3_2);
	mem_write32(r11, (uint16_t)(r3_2 + 0x300u));
	mem_write32(r25 + 0xC0u, r10);
	mem_write16(r1_2, (r2_2 >> 4) | 0x8000u);
	mem_write16(r25 + 0xB0u, mem_read16(r25 + 0xB0u) + 1u);
	return r18;
}

void sub_011C60(void)
{
	uint32_t r0 = 0;

	move_halfwords(r25 - 0x7FF0u, 0x100800u, 0x7Cu);
	mem_write32(r25 - 0x8000u, r25 - 0x7E00u);
	mem_write32(r25 - 0x7FFCu, r25 - 0x7DFEu);
	mem_write32(r25 - 0x7FF4u, r25 - 0x7DFEu);
	mem_write32(r25 - 0x7FF8u, r25 - 0x7D00u);
	r0 = mem_read8(r25 - 0x7E00u);
	r0 = (uint16_t)(r0 + 1u);
	move_halfwords(r25 - 0x7E00u, 0x100800u, r0);
	r0 = mem_read8(r25 - 0x7D00u);
	r0 = (uint16_t)(r0 + 1u);
	move_string(0x100800u, r0, r25 - 0x7D00u, r0, 2u, 0u, 0u, r26, &r27, &r28);
	mem_write16(r25 + 0x900u, 0u);
}

void sub_011CC0(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint32_t t1 = 0;

	r20 = r25;
	r22 = mem_read32(r25 - 0x8000u) + 2u;
	mem_write16(mem_read32(r25 - 0x7FF8u), 0u);
	for (;;)
	{
		r20 = (r20 & ~0xFFFFu) | mem_read16(r22);
		r22 = r22 + 2u;
		if ((uint16_t)r20 == 0u)
			break;
		if ((OBJECT(r20)->unk_00 & 1u) != 0u)
		{
			call_code(OBJECT(r20)->unk_04);
			t1 = mem_read32(r25 - 0x7FF8u);
			mem_write16(t1, mem_read16(t1) + 1u);
			mem_write16(mem_read32(r25 - 0x7FF8u) + mem_read16(mem_read32(r25 - 0x7FF8u)) * 2u, r20);
			continue;
		}
		mem_write8(r25 - 0x7FF0u + (uint8_t)OBJECT(r20)->unk_08, 0u);
	}
	r1 = mem_read32(r25 - 0x8000u);
	r0 = mem_read16(r1);
	r0 = (uint16_t)(r0 + 4u);
	r0 = r0 >> 2;
	r2 = 0u;
	r3 = 0u;
	do
	{
		mem_write32(r1, r2);
		mem_write32(r1 + 4u, r3);
		r0--;
		r1 += 8u;
	} while (r0 != 0u);
	mem_write16(r25 - 0x8000u, mem_read16(r25 - 0x8000u) ^ 0x100u);
	mem_write16(r25 - 0x7FF8u, mem_read16(r25 - 0x7FF8u) ^ 0x100u);
	r0 = mem_read16(mem_read32(r25 - 0x8000u));
	mem_write32(r25 - 0x7FFCu, mem_read32(r25 - 0x8000u) + 2u);
	mem_write32(r25 - 0x7FF4u, mem_read32(r25 - 0x7FFCu) + r0 * 2u);
}

uint32_t sub_011D43(void)
{
	uint32_t zero = 0;
	uint8_t r9 = 0;
	uint32_t r21 = 0;
	uint32_t t1 = 0;

	search_bytes(r25 - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
	{
		r21 = r25 - 0x7F00u;
		return r21;
	}
	mem_write8(r28, 0xFFu);
	r9 = (uint8_t)r27;
	r27 = r27 << 8;
	r21 = r25 - 0x7C00u + r27;
	r27 = 0u;
	r28 = 0u;
	r26 = r21;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 = r26 + 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	mem_write8(r21, mem_read8(r21) | 1u);
	mem_write8(r21 + 8u, r9);
	t1 = mem_read32(r25 - 0x8000u);
	mem_write16(t1, mem_read16(t1) + 1u);
	mem_write16(mem_read32(r25 - 0x8000u) + mem_read16(mem_read32(r25 - 0x8000u)) * 2u, r21);
	r26 = r26 + 8u;
	return r21;
}

uint32_t sub_011E2C(uint32_t r10)
{
	uint8_t r7 = 0;
	uint32_t r8 = 0, r21 = 0;

	r8 = mem_read32(r10);
	r7 = 0u;
	r10 += 4u;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, mem_read32(r10));
		mem_write8(r21 + 9u, r7);
		r7++;
		r8--;
		r10 += 4u;
	} while (r8 != 0u);
	return r21;
}

uint32_t sub_011E45(void)
{
	uint32_t r0 = 0, r1 = 0;

	r1 = mem_read32(r25 + 0xF00u);
	if ((uint16_t)r1 == 0u)
		r1 ^= 0x2A6D365Bu;
	r0 = r1;
	r1 = (r1 * 2u * 2u + r0) * 2u * 2u * 2u + r0;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)r1;
	r1 = (r1 << 16) | (r1 >> 16);
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + r1);
	r1 = (r1 & ~0xFFFFu) | (uint16_t)r0;
	mem_write32(r25 + 0xF00u, (r1 << 16) | (r1 >> 16));
	return r0;
}

void sub_011E88(void)
{
	unk_208800 = 0x100u;
	mem_write8(IO_C0001C, 6u);
	unk_208822 = 0u;
	mem_write8(SOUND_701FFF, 0u);
}

void sub_011EB8(uint8_t r0)
{
	uint32_t r2 = 0;

	if ((mem_read32(r25 + 0x90u) & 0x100u) == 0u)
	{
		sub_011EDE(r0);
		return;
	}
	if (unk_20E60D != 0u)
	{
		r2 = (uint8_t)r0;
		if ((rom_01202E[r2 >> 5] & shl32(1u, (int)(r2 & 0x1Fu))) != 0u)
		{
			sub_011EDE(r0);
			return;
		}
	}
}

void sub_011EDE(uint8_t r0)
{
	uint32_t r1 = 0, r2 = 0;

	r2 = r25 + 0x801u;
	r1 = mem_read8(r2);
	r2++;
	mem_write8(r2 + r1, r0);
	mem_write8(r2 - 1u, (r1 + 1u) & 0x1Fu);
}

void sub_011F30(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	if ((mem_read32(r25 + 0x90u) & 0x100u) != 0u && unk_20E60D == 0u)
	{
		r10 = (uint32_t)mem_read8(r25) << 8;
		mem_write8(r10 + SOUND_700710, 0u);
		mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
		return;
	}
	r10 = (uint32_t)mem_read8(r25) << 8;
	mem_write8(r10 + (SOUND_RAM + 0x713u), rom_012026[OBJECT(r23)->unk_60]);
	r0 = 0x10u;
	if (OBJECT(r23)->unk_61 != 0u)
		r0 = 0x30u;
	mem_write8(r10 + (SOUND_RAM + 0x714u), r0);
	mem_write8(r10 + SOUND_700710, 1u);
	mem_write8(r10 + SOUND_700712, 0xA0u);
}

void sub_011F7C(void)
{
	uint32_t r10 = 0;

	if ((mem_read32(r25 + 0x90u) & 0x100u) == 0u || unk_20E60D != 0u)
	{
		r10 = (uint32_t)mem_read8(r25) << 8;
		mem_write8(r10 + SOUND_700710, OBJECT(r23)->unk_70 >> 8);
		mem_write8(r10 + SOUND_700712, 0x20u);
		return;
	}
	r10 = (uint32_t)mem_read8(r25) << 8;
	mem_write8(r10 + SOUND_700710, 0u);
	mem_write8(r10 + SOUND_700712, mem_read8(r10 + SOUND_700712) | 0x20u);
}

void sub_011FC3(uint8_t r0)
{
	if (OBJECT(r23)->unk_7C == 0u)
	{
		sub_011FC9(r0);
		return;
	}
}

void sub_011FC9(uint8_t r0)
{
	uint32_t r10 = 0;

	if ((mem_read32(r25 + 0x90u) & 0x100u) == 0u || unk_20E60D != 0u)
	{
		mem_write8(((uint32_t)mem_read8(r25) << 8) + (SOUND_RAM + 0x718u), r0);
		return;
	}
	r10 = (uint32_t)mem_read8(r25) << 8;
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
	mem_write8(rom_011FFC[mem_read8(r25)], r0);
}

void sub_012004(void)
{
	uint32_t r10 = 0;

	r10 = (uint32_t)mem_read8(r25) << 8;
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

	r10 = mem_read32(r25 + 0x308u) + 0x20u;
	r0 = mem_read8(mem_read32(r25 + 0x308u) + 0x1Fu);
	r9 = mem_read8(r25 + 1u);
	if ((mem_read32(r25 + 0x90u) & 0x100u) != 0u)
		r0 = 1u;
	r11 = r25 + 0x510u;
	mem_write32(r11, r0);
	r11 += 4u;
	do
	{
		r1 = mem_read8(r10);
		r10++;
		if ((mem_read32(r25 + 0x90u) & 0x100u) != 0u || r9 == r1)
		{
			r1 = r9;
			mem_write32(r25 + 0x500u, r11);
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
	r11 = r25 + 0x5B0u;
	r15 = OBJECT(r23)->unk_32;
	r17 = OBJECT(r23)->unk_38;
	r1_2 = AS16(OBJECT(r23)->unk_44);
	r2 = OBJECT(r23)->unk_60;
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
	uint32_t t1 = 0;

	if ((mem_read32(AS32(OBJECT(r23)->unk_00)) & 8u) != 0u)
		return;
	r0 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	r15_2 = mem_read32(r25 + 0x500u) + 6u;
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
	if ((int16_t)((r12 - OBJECT(r20)->unk_58) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = OBJECT(r20)->unk_10;
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
		r10 = AS32(OBJECT(r20)->unk_60);
		r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(r20)->unk_3F * mem_read16(r25 + 0x2012u + r7_2 * 2u) + OBJECT(r20)->unk_20);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(r20)->unk_28);
		r7 = r6;
		r6 += mem_read16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + mem_read16(r10));
		r10 += 2u;
		r7 = (uint16_t)(r7 - r16);
		r9 = (uint16_t)(r8 + mem_read16(r10 + 2u) - r18);
	} while ((int16_t)(((r8 + mem_read16(r10) - r19) ^ r9) & ((r6 - r17) ^ r7)) >= 0);
	t1 = AS32(OBJECT(r23)->unk_00);
	mem_write16(t1, mem_read16(t1) | 4u);
	mem_write8(AS32(OBJECT(r23)->unk_00) + 0x6Eu, r27);
	r7 = (uint16_t)((int16_t)r7 >> 1);
	mem_write16(r23 + 0x122u, r7);
	mem_write16(r23 + 0x120u, (int16_t)r9 >> 1);
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
	uint32_t t2 = 0;

	r27 = 0u;
	if ((mem_read32(AS32(OBJECT(r23)->unk_00)) & 8u) != 0u)
		return;
	r0 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	r15_2 = mem_read32(r25 + 0x500u) + 6u;
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
	if ((int16_t)((r12 - OBJECT(r20)->unk_58) ^ (r13 - r0)) >= 0)
	{
		*p_r15 = r15_2;
		return;
	}
	r4 = OBJECT(r20)->unk_10;
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
		r10 = AS32(OBJECT(r20)->unk_60);
		r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(r20)->unk_3F * mem_read16(r25 + 0x2012u + r7_2 * 2u) + OBJECT(r20)->unk_20);
		r6 = (uint16_t)(((uint32_t)r6 << 7) + (uint16_t)OBJECT(r20)->unk_28);
		r7 = r6;
		r9 = (uint16_t)r8;
		r6 += mem_read16(r10);
		r10 += 2u;
		r7 = (uint16_t)(r7 + mem_read16(r10));
		r10 += 2u;
		r8 += mem_read16(r10);
		r9 += mem_read16(r10 + 2u);
		r26 = (r26 & ~0xFFFFu) | (uint16_t)(0u - r8);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(0u - r9);
		r7 = (uint16_t)(r7 - r16);
		r9 -= r18;
		r6 = (uint16_t)((r6 - r17) ^ r7);
		r26 = (r26 & ~0xFFFFu) | (uint16_t)(r26 - r18);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 - r19);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 ^ r26);
		r28 = (r28 & ~0xFFFFu) | (r28 & r6);
		t1 = r28 | (((r8 - r19) ^ r9) & r6);
		r28 = (r28 & ~0xFFFFu) | t1;
	} while ((int16_t)t1 >= 0);
	t2 = AS32(OBJECT(r23)->unk_00);
	mem_write16(t2, mem_read16(t2) | 4u);
	mem_write8(AS32(OBJECT(r23)->unk_00) + 0x6Eu, r27);
	r7 = (uint16_t)((int16_t)r7 >> 1);
	mem_write16(r23 + 0x122u, r7);
	mem_write16(r23 + 0x120u, (int16_t)r9 >> 1);
	*p_r7 = r7;
	*p_r15 = r15_2;
}

void sub_01256F(void)
{
	uint16_t r4 = 0, r6_2 = 0, r7 = 0, r8 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0;
	uint32_t r5 = 0, r6 = 0, r15 = 0;
	uint32_t t1 = 0;

	if ((mem_read32(AS32(OBJECT(r23)->unk_00)) & 8u) != 0u)
		return;
	r14 = r25 + 0x510u;
	r15 = mem_read32(r25 + 0x500u);
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
	r5 = mem_read32(r14);
	r14 = r14 + 4u;
	if ((uint8_t)r5 < 2u)
		return;
	for (;;)
	{
		r26 = (r26 & ~0xFFFFu) | mem_read16(r14);
		r14 = r14 + 2u;
		if (r4 == (uint16_t)r26)
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
			t1 = AS32(OBJECT(r23)->unk_00);
			mem_write16(t1, mem_read16(t1) | 4u);
			mem_write8(AS32(OBJECT(r23)->unk_00) + 0x6Eu, 2u);
			mem_write16(AS32(OBJECT(r23)->unk_00) + 0x68u, r26);
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
	uint32_t r0 = 0, r6 = 0, r7 = 0, r10 = 0, r15 = 0;
	uint16_t t1 = 0;

	if ((mem_read32(AS32(OBJECT(r23)->unk_00)) & 8u) != 0u)
	{
		negative = 0u;
		return negative;
	}
	r6 = OBJECT(r20)->unk_1C;
	r15 = mem_read32(r25 + 0x500u) + 6u;
	r12 = (r12 & ~0xFFFFu) | mem_read16(r15);
	r15 += 4u;
	r7 = (uint16_t)(r6 << 4);
	r10 = AS32(OBJECT(r20)->unk_60);
	r8 = (uint16_t)((uint32_t)(int8_t)OBJECT(r20)->unk_3F * mem_read16(r25 + 0x2012u + (r7 & 0x7FFu) * 2u) + OBJECT(r20)->unk_20);
	r6 = (uint16_t)(((r6 - r12) << 7) + (uint16_t)OBJECT(r20)->unk_28);
	t1 = ((r8 + mem_read16(r10 + 4u) - mem_read16(r15 + 6u)) ^ (r8 + mem_read16(r10 + 6u) - mem_read16(r15 + 4u))) & ((r6 + mem_read16(r10) - mem_read16(r15 + 2u)) ^ (r6 + mem_read16(r10 + 2u) - mem_read16(r15)));
	negative = t1 >> 15;
	if ((int16_t)t1 < 0)
	{
		r0 = AS32(OBJECT(r23)->unk_00);
		mem_write16(r0, mem_read16(r0) | 4u);
		mem_write8(r0 + 0x6Eu, 1u);
		mem_write16(r0 + 0x68u, r20);
		negative = 1u;
	}
	return negative;
}

void sub_0126FC(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r10 = 0, left_caller = 0;
	uint32_t target1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_80 = road_object_streams[mem_read8(r25 + 0xA3u)];
		return;
	case 1:
		break;
	case 2:
		if (mem_read8(r25 + 0xA3u) != (uint8_t)(OBJECT(r20)->unk_84 >> 16u))
			return;
		OBJECT(r20)->unk_0C--;
		r0_2 = mem_read8(r25 + 0xA3u);
		AS16(OBJECT(r20)->unk_84) = mem_read16(r23 + 0xAAu);
		OBJECT(r20)->unk_80 = road_object_streams[r0_2];
		break;
	case 3:
		if (mem_read8(r25 + 0xA1u) == (uint8_t)(OBJECT(r20)->unk_84 >> 16u))
			return;
		OBJECT(r20)->unk_0C -= 2u;
		OBJECT(r20)->unk_80 = mem_read32(OBJECT(r20)->unk_80 + mem_read8(r23 + 0xC9u) * 4u);
		break;
	default:
		invalid_jump(rom_012708[OBJECT(r20)->unk_0C]);
	}
	r10 = OBJECT(r20)->unk_80;
	for (;;)
	{
		r0 = mem_read16(r10);
		r10 += 2u;
		if ((int16_t)OBJECT(r23)->unk_46 < (int16_t)r0)
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
	mem_write16(r21 + 0x1Cu, r0 + (uint16_t)OBJECT(r20)->unk_84);
	mem_write32(r21 + 4u, mem_read32(r10));
	r10 += 4u;
	mem_write32(r21 + 0x18u, mem_read32(r10));
	r10 += 4u;
	OBJECT(r20)->unk_80 = r10;
	return r10;
}

uint32_t sub_012777(void)
{
	uint32_t left_caller = 0;

	OBJECT(r20)->unk_0C++;
	AT8(OBJECT(r20)->unk_84, 2) = mem_read8(r25 + 0xA1u);
	left_caller = 1u;
	return left_caller;
}

uint32_t sub_012786(uint32_t r10)
{
	uint32_t left_caller = 0;

	OBJECT(r20)->unk_0C += 2u;
	OBJECT(r20)->unk_80 = r10;
	AT8(OBJECT(r20)->unk_84, 2) = mem_read8(r25 + 0xA3u);
	left_caller = 1u;
	return left_caller;
}

uint32_t sub_01279C(void)
{
	uint32_t left_caller = 0;

	OBJECT(r20)->unk_00 = 0u;
	OBJECT(r20)->unk_01 = 0u;
	left_caller = 1u;
	return left_caller;
}

void sub_0127EE(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_3E = 6u;
		AS32(OBJECT(r20)->unk_38) = r20 + 0x80u;
		AS16(OBJECT(r20)->unk_80) = 0x7000u;
		AT16(OBJECT(r20)->unk_80, 2) = 0xDFu;
		AS16(OBJECT(r20)->unk_84) = 0u;
		AT16(OBJECT(r20)->unk_84, 2) = 0x13Fu;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_0127FC[OBJECT(r20)->unk_0C]);
	}
	if (mem_read8(r25 + 0x1F00u) == 0u)
		return;
	OBJECT(r20)->unk_10 = 0u;
	OBJECT(r20)->unk_3C = 0xFFFFu;
	do
	{
		r1 = OBJECT(r20)->unk_10;
		r0 = mem_read16(r25 + 0x1F10u + r1 * 2u);
		if (r0 < OBJECT(r20)->unk_3C)
		{
			OBJECT(r20)->unk_3C = r0;
			r0_2 = (uint16_t)(mem_read16(r25 + 0x1F30u + r1 * 2u) + mem_read16(r25 + 0x16u) - 1u);
			AT16(OBJECT(r20)->unk_80, 2) = r0_2;
			sub_00EAE4(r0_2);
		}
		OBJECT(r20)->unk_10++;
	} while (OBJECT(r20)->unk_10 <= mem_read8(r25 + 0x1F00u));
}

void sub_01287C(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
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
		invalid_jump(rom_012888[OBJECT(r20)->unk_0C]);
	}
}

void sub_012894(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(r20)->unk_0C++;
	r10 = OBJECT(r20)->unk_18;
	r9 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_00 |= mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_45 = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_44 = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_3F = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_24 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(r20)->unk_10) = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(r20)->unk_12) = mem_read16(r10);
	r10 += 2u;
	if (mem_read32(r10) == 0u)
		OBJECT(r20)->unk_0C++;
	AS32(OBJECT(r20)->unk_60) = r10;
	sub_00F366(r9);
	r0 = OBJECT(r20)->unk_1C;
	r1 = (uint16_t)(AS16(OBJECT(r20)->unk_10) * (AS16(OBJECT(r20)->unk_12) - 1u) + 1u);
	AT16(OBJECT(r20)->unk_54, 2) = r0;
	OBJECT(r20)->unk_58 = r0 + r1;
}

void sub_012906(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r7 = 0, r9 = 0, r10 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_18 = mem_read32(r10);
		r10 += 4u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_45 = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_10) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_12) = mem_read16(r10);
		r10 += 2u;
		if (mem_read32(r10) == 0u)
			OBJECT(r20)->unk_0C++;
		AS32(OBJECT(r20)->unk_60) = r10;
		sub_00F366(r9);
		r0 = OBJECT(r20)->unk_1C;
		r1 = (uint16_t)(AS16(OBJECT(r20)->unk_10) * (AS16(OBJECT(r20)->unk_12) - 1u) + 1u);
		AT16(OBJECT(r20)->unk_54, 2) = r0;
		OBJECT(r20)->unk_58 = r0 + r1;
		return;
	case 1:
		sub_0122E8(&r7, &r15);
		r10 = OBJECT(r20)->unk_18;
		r0_2 = (int16_t)mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
		{
			r10 += r0_2;
			r0_2 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(r20)->unk_3A = r0_2;
		OBJECT(r20)->unk_18 = r10;
		sub_00F5C8(r7, r15);
		return;
	case 2:
		r10 = OBJECT(r20)->unk_18;
		r0_2 = (int16_t)mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
		{
			r10 += r0_2;
			r0_2 = mem_read16(r10);
			r10 += 2u;
		}
		OBJECT(r20)->unk_3A = r0_2;
		OBJECT(r20)->unk_18 = r10;
		sub_00F5C8(r7, r15);
		return;
	default:
		invalid_jump(rom_012914[OBJECT(r20)->unk_0C]);
	}
}

void sub_0129AB(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
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
		invalid_jump(rom_0129B8[OBJECT(r20)->unk_0C]);
	}
	sub_00F911(r7, r15);
}

void sub_0129F4(void)
{
	uint32_t r7 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		AS8(OBJECT(r20)->unk_5C) = 0xFFu;
		sub_012894();
		return;
	case 1:
	case 2:
		r7 = sub_00F30D(0u);
		sub_00F5C8(r7, 0u);
		return;
	default:
		invalid_jump(rom_012A00[OBJECT(r20)->unk_0C]);
	}
}

void sub_012A20(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = (uint16_t)rom_012A2C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		OBJECT(r20)->unk_18 = mem_read32(r10);
		r10 += 4u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		AT8(OBJECT(r20)->unk_08, 1) = mem_read8(r10 + 1u);
		OBJECT(r20)->unk_3E = 0u;
		OBJECT(r20)->unk_30 = 0xFF60u;
		if (OBJECT(r23)->unk_60 != 6u)
		{
			OBJECT(r20)->unk_32 = 0x40u;
			sub_012AAB();
		}
		else
		{
			OBJECT(r20)->unk_32 = 0x30u;
			sub_012AAB();
		}
		OBJECT(r20)->unk_30 += 8u;
		if ((int16_t)OBJECT(r20)->unk_30 >= 0xA0)
			OBJECT(r20)->unk_0C++;
		break;
	case 1:
		OBJECT(r20)->unk_30 += 8u;
		if ((int16_t)OBJECT(r20)->unk_30 >= 0xA0)
			OBJECT(r20)->unk_0C++;
		sub_012AA5();
		r0 = (uint16_t)((r0 & ~0xFFu) | OBJECT(r20)->unk_10);
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
	r0 = (uint16_t)((r0 & ~0xFFu) | OBJECT(r20)->unk_10);
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

	t1 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t1 - 1u;
	if (t1 == 1u)
	{
		sub_012AAB();
		return;
	}
}

void sub_012AAB(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	r10 = OBJECT(r20)->unk_18;
	r0 = (int16_t)mem_read16(r10);
	r10 += 2u;
	if ((int16_t)r0 < 0)
	{
		r10 += r0;
		r0 = (int16_t)mem_read16(r10);
		r10 += 2u;
	}
	r9 = r0;
	OBJECT(r20)->unk_18 = r10;
	OBJECT(r20)->unk_10 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	sub_00F366(r9);
}

void sub_012ACF(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
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
		invalid_jump(rom_012ADC[OBJECT(r20)->unk_0C]);
	}
}

void sub_012AF4(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
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
		invalid_jump(rom_012B00[OBJECT(r20)->unk_0C]);
	}
	r7 = sub_00F30D(r7);
	sub_00F5C8(r7, r15);
}

void sub_012B0C(void)
{
	uint16_t r0 = 0, r1 = 0;
	uint32_t r9 = 0, r10 = 0;

	OBJECT(r20)->unk_0C++;
	r10 = OBJECT(r20)->unk_18;
	r9 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_00 |= mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_45 = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_44 = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_3F = mem_read8(r10);
	r10++;
	OBJECT(r20)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_24 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(r20)->unk_10) = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(r20)->unk_12) = mem_read16(r10);
	r10 += 2u;
	if (mem_read32(r10) == 0u)
		OBJECT(r20)->unk_0C++;
	AS32(OBJECT(r20)->unk_60) = r10;
	sub_00F366(r9);
	r0 = OBJECT(r20)->unk_1C;
	r1 = (uint16_t)(AS16(OBJECT(r20)->unk_10) * (AS16(OBJECT(r20)->unk_12) - 1u) + 1u);
	AT16(OBJECT(r20)->unk_54, 2) = r0;
	OBJECT(r20)->unk_58 = r0 + r1;
}

void sub_012B84(void)
{
	uint32_t r7 = 0, r15 = 0;

	switch (OBJECT(r20)->unk_0C)
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
		invalid_jump(rom_012B90[OBJECT(r20)->unk_0C]);
	}
	r7 = sub_00F30D(r7);
	sub_00F911(r7, r15);
}

void sub_012BAE(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_012BBC[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x165u)
			AS32(OBJECT(r20)->unk_24) = (int16_t)0xFF00u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(r20)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_84) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		OBJECT(r20)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		if ((int16_t)AS16(OBJECT(r23)->unk_44) < (int16_t)OBJECT(r20)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		r9 = (uint16_t)OBJECT(r20)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(r23)->unk_42 < (int16_t)OBJECT(r20)->unk_1C)
		{
			OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)AS16(OBJECT(r23)->unk_44) < (int16_t)OBJECT(r20)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		r9 = (uint16_t)OBJECT(r20)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(r23)->unk_42 < (int16_t)OBJECT(r20)->unk_1C)
		{
			OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	case 3:
		if ((int16_t)OBJECT(r23)->unk_42 < (int16_t)OBJECT(r20)->unk_1C)
		{
			OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_012CB8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(r20)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_84) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		OBJECT(r20)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		if ((int16_t)AS16(OBJECT(r23)->unk_44) < (int16_t)OBJECT(r20)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		r9 = (uint16_t)OBJECT(r20)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(r20)->unk_28 + (OBJECT(r23)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				AS16(OBJECT(r20)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(r20)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0 || (int16_t)OBJECT(r20)->unk_30 > 0x130)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		return;
	case 2:
		if ((int16_t)AS16(OBJECT(r23)->unk_44) < (int16_t)OBJECT(r20)->unk_58)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		r9 = (uint16_t)OBJECT(r20)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(r20)->unk_28 + (OBJECT(r23)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				AS16(OBJECT(r20)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(r20)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0 || (int16_t)OBJECT(r20)->unk_30 > 0x130)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		return;
	case 3:
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_84;
		r0 = (uint16_t)((uint16_t)OBJECT(r20)->unk_28 + (OBJECT(r23)->unk_74 >> 9));
		if ((int16_t)r0 >= 0x80)
		{
			r0 = (uint16_t)(r0 - 0x80u);
			OBJECT(r20)->unk_1C++;
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				AS16(OBJECT(r20)->unk_28) = r0;
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		AS16(OBJECT(r20)->unk_28) = r0;
		sub_00F30D(0u);
		sub_00EAE4(r0);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0 || (int16_t)OBJECT(r20)->unk_30 > 0x130)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_012DC4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		AS16(OBJECT(r20)->unk_80) += 0x10u;
		r10 += 2u;
		AT16(OBJECT(r20)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_84) = mem_read16(r10);
		AT16(OBJECT(r20)->unk_84, 2) = mem_read16(r10 + 2u);
		sub_00F366(r9);
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - (uint16_t)OBJECT(r20)->unk_80;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)OBJECT(r20)->unk_58)
			return;
		OBJECT(r20)->unk_0C++;
		if ((int16_t)OBJECT(r23)->unk_42 < (int16_t)OBJECT(r20)->unk_1C)
		{
			OBJECT(r20)->unk_20 += (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
			OBJECT(r20)->unk_24 += (uint16_t)OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_1C += (uint16_t)(OBJECT(r20)->unk_84 >> 16u);
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	case 2:
		if ((int16_t)OBJECT(r23)->unk_42 < (int16_t)OBJECT(r20)->unk_1C)
		{
			OBJECT(r20)->unk_20 += (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
			OBJECT(r20)->unk_24 += (uint16_t)OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_1C += (uint16_t)(OBJECT(r20)->unk_84 >> 16u);
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_012EB0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		AS16(OBJECT(r20)->unk_80) += 0x10u;
		r10 += 2u;
		AT16(OBJECT(r20)->unk_80, 2) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_84) = mem_read16(r10);
		r10 += 2u;
		AT16(OBJECT(r20)->unk_84, 2) = mem_read16(r10);
		r10 += 2u;
		AS8(OBJECT(r20)->unk_88) = mem_read8(r10);
		AT16(OBJECT(r20)->unk_88, 2) = mem_read16(r10 + 1u);
		sub_00F366(r9);
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - (uint16_t)OBJECT(r20)->unk_80;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)OBJECT(r20)->unk_58 < (int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u))
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_12 = (uint8_t)OBJECT(r20)->unk_88;
			r0 = sub_00EAE4(r0);
			return;
		}
		return;
	case 2:
		OBJECT(r20)->unk_24 += (uint16_t)(OBJECT(r20)->unk_88 >> 16u);
		t2 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t2 - 1u;
		if (t2 != 1u)
		{
			OBJECT(r20)->unk_1C += (uint16_t)(OBJECT(r20)->unk_84 >> 16u);
			if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
			{
				sub_00F30D(0u);
				sub_00EAE4(r0);
				if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
					return;
			}
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_0C++;
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_20 += (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		OBJECT(r20)->unk_24 += (uint16_t)OBJECT(r20)->unk_84;
		OBJECT(r20)->unk_1C += (uint16_t)(OBJECT(r20)->unk_84 >> 16u);
		if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
		{
			sub_00F30D(0u);
			sub_00EAE4(r0);
			if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
				return;
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	case 3:
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_20 += (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		OBJECT(r20)->unk_24 += (uint16_t)OBJECT(r20)->unk_84;
		OBJECT(r20)->unk_1C += (uint16_t)(OBJECT(r20)->unk_84 >> 16u);
		if ((int16_t)(0u - AS16(OBJECT(r20)->unk_10) + OBJECT(r20)->unk_1C) < (int16_t)OBJECT(r23)->unk_42)
		{
			sub_00F30D(0u);
			sub_00EAE4(r0);
			if ((int16_t)OBJECT(r20)->unk_30 >= -0xF0 && (int16_t)OBJECT(r20)->unk_30 <= 0x130)
				return;
		}
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_012FC8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_44 = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_24 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_10) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_12) = mem_read16(r10);
		r10 += 2u;
		if (mem_read32(r10) == 0u)
			OBJECT(r20)->unk_0C++;
		AS32(OBJECT(r20)->unk_60) = r10;
		sub_00F366(r9);
		r0_2 = OBJECT(r20)->unk_1C;
		r1 = (uint16_t)(AS16(OBJECT(r20)->unk_10) * (AS16(OBJECT(r20)->unk_12) - 1u) + 1u);
		AT16(OBJECT(r20)->unk_54, 2) = r0_2;
		OBJECT(r20)->unk_58 = r0_2 + r1;
		return;
	case 1:
		sub_0122E8(&ignored, &ignored);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = mem_read16(AS32(OBJECT(r23)->unk_00) + 0x10u);
		r0 = (uint16_t)(r0 + 0x1Du);
		r0 = r0 / 0x1Eu;
		OBJECT(r20)->unk_3A = rom_01306E[r0];
		if (r0 == 0u)
			OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_012FC8[t1]);
	}
}
