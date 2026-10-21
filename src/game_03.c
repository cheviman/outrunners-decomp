/**
 *    Name:        game_03.c
 *    Description: the game's functions, part 03 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_01307E(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01308C[t1];
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
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		OBJECT(r20)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(r20)->unk_3E = 9u;
		OBJECT(r20)->unk_44 = 1u;
		return;
	case 1:
		r0 = mem_read16(AS32(OBJECT(r23)->unk_00) + 0x10u);
		if (r0 != 0u)
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
		sub_00F30D(0u);
		sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_013110[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		OBJECT(r20)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(r20)->unk_3E = 9u;
		OBJECT(r20)->unk_44 = 1u;
		OBJECT(r20)->unk_0C++;
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - 0x1Eu;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		r9 = (uint16_t)OBJECT(r20)->unk_80;
		sub_00F366(r9);
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
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

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		OBJECT(r20)->unk_24 = mem_read16(r10 + 2u);
		sub_00F366(r9);
		OBJECT(r20)->unk_3E = 9u;
		OBJECT(r20)->unk_44 = 1u;
		return;
	case 1:
		if (mem_read16(AS32(OBJECT(r23)->unk_00) + 0x10u) != 0u)
			return;
		OBJECT(r20)->unk_0C++;
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42 || (int16_t)OBJECT(r20)->unk_24 < -0x3000)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = (uint16_t)(AS16(OBJECT(r20)->unk_12) + 1u);
		AS16(OBJECT(r20)->unk_12) = r0;
		r0 = r0 >> 1;
		OBJECT(r20)->unk_24 -= r0;
		OBJECT(r20)->unk_20 += 8u;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((int16_t)OBJECT(r20)->unk_1C <= (int16_t)OBJECT(r23)->unk_42 || (int16_t)OBJECT(r20)->unk_24 < -0x3000)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = (uint16_t)(AS16(OBJECT(r20)->unk_12) + 1u);
		AS16(OBJECT(r20)->unk_12) = r0;
		r0 = r0 >> 1;
		OBJECT(r20)->unk_24 -= r0;
		OBJECT(r20)->unk_20 += 8u;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0131B0[OBJECT(r20)->unk_0C]);
	}
}

void sub_01322D(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		AS16(OBJECT(r20)->unk_10) = AS16(OBJECT(r23)->unk_44) + OBJECT(r20)->unk_18;
		OBJECT(r20)->unk_0C++;
		r0_2 = 0x51u;
		sub_011EB8(r0_2);
		return;
	case 1:
		r0 = sub_011E45();
		OBJECT(r20)->unk_12 = (r0 & 0x3Fu) + 0xFu;
		OBJECT(r20)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_013238[OBJECT(r20)->unk_0C]);
	}
	if ((int16_t)AS16(OBJECT(r20)->unk_10) <= (int16_t)AS16(OBJECT(r23)->unk_44))
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(r20)->unk_12;
	OBJECT(r20)->unk_12 = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(r20)->unk_0C--;
	r0_2 = 0x51u;
	sub_011EB8(r0_2);
}

void sub_013288(void)
{
	uint8_t r0 = 0;

	OBJECT(r20)->unk_00 = 0u;
	r0 = 0x23u;
	sub_011EB8(r0);
}

void sub_013296(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_3E = 0xAu;
		OBJECT(r20)->unk_3C = 0x19Du;
		OBJECT(r20)->unk_30 = 0xA0u;
		OBJECT(r20)->unk_32 = 0x118u;
		r9 = 0x39u;
		AS16(OBJECT(r20)->unk_80) = 0x67u;
		sub_00F366(r9);
		return;
	case 1:
		OBJECT(r20)->unk_32 -= 2u;
		if (OBJECT(r20)->unk_32 == 0xDCu)
			OBJECT(r20)->unk_0C++;
		if (mem_read16(mem_read32(r25 + 0x308u) + 0x6Au) == 0u || AS16(OBJECT(r23)->unk_44) > 0x250u)
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_32 += 2u;
			if (OBJECT(r20)->unk_32 == 0x118u)
			{
				OBJECT(r20)->unk_00 = 0u;
				OBJECT(r20)->unk_01 = 0u;
				return;
			}
		}
		r0 = AS16(OBJECT(r20)->unk_10);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		AS16(OBJECT(r20)->unk_10) = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(r20)->unk_80);
		OBJECT(r20)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read16(mem_read32(r25 + 0x308u) + 0x6Au) == 0u || AS16(OBJECT(r23)->unk_44) > 0x250u)
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_32 += 2u;
			if (OBJECT(r20)->unk_32 == 0x118u)
			{
				OBJECT(r20)->unk_00 = 0u;
				OBJECT(r20)->unk_01 = 0u;
				return;
			}
		}
		r0 = AS16(OBJECT(r20)->unk_10);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		AS16(OBJECT(r20)->unk_10) = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(r20)->unk_80);
		OBJECT(r20)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(r20)->unk_32 += 2u;
		if (OBJECT(r20)->unk_32 == 0x118u)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		r0 = AS16(OBJECT(r20)->unk_10);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		AS16(OBJECT(r20)->unk_10) = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(r20)->unk_80);
		OBJECT(r20)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0132A4[OBJECT(r20)->unk_0C]);
	}
}

void sub_01333C(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_013348[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(r20)->unk_24 = 0xFE80u;
		if (r9 == 0x3A0u)
			OBJECT(r20)->unk_24 = 0xFF00u;
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
		OBJECT(r20)->unk_12 = 0xAu;
		sub_00F366(r9);
		OBJECT(r20)->unk_0C++;
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - 0x64u;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
	case 2:
		t2 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_12 = 0xFu;
			r0 = (uint16_t)OBJECT(r20)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(r20)->unk_20 += (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(r20)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 |= 0x40u;
		t3 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(r20)->unk_12 = 0xFu;
			r0 = (uint16_t)OBJECT(r20)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(r20)->unk_20 -= (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C = 2u;
			OBJECT(r20)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		t3 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(r20)->unk_12 = 0xFu;
			r0 = (uint16_t)OBJECT(r20)->unk_84;
			sub_011EB8((uint8_t)r0);
		}
		OBJECT(r20)->unk_20 -= (uint16_t)(OBJECT(r20)->unk_80 >> 16u);
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C = 2u;
			OBJECT(r20)->unk_00 &= 0xBFu;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01345C[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(r20)->unk_24 = 0xFE80u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		OBJECT(r20)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(r20)->unk_0C++;
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - 0x64u;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(r20)->unk_30 <= 0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 &= 0xBFu;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 |= 0x40u;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(r20)->unk_30 <= 0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 &= 0xBFu;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 |= 0x40u;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 < -0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 |= 0x40u;
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

	switch (OBJECT(r20)->unk_0C)
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
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		AT8(OBJECT(r20)->unk_80, 2) = 0x3Cu;
		AS32(OBJECT(r20)->unk_60) = r10;
		sub_00F366(r9);
		r0 = OBJECT(r20)->unk_1C;
		r1 = (uint16_t)(AS16(OBJECT(r20)->unk_10) * (AS16(OBJECT(r20)->unk_12) - 1u) + 1u);
		AT16(OBJECT(r20)->unk_54, 2) = r0;
		OBJECT(r20)->unk_58 = r0 + r1;
		return;
	case 1:
	case 2:
		t1 = (uint8_t)(OBJECT(r20)->unk_80 >> 16u);
		AT8(OBJECT(r20)->unk_80, 2) = t1 - 1u;
		if (t1 == 1u)
		{
			AT8(OBJECT(r20)->unk_80, 2) = 0x3Cu;
			r0_2 = (uint8_t)(uint16_t)OBJECT(r20)->unk_80;
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
		invalid_jump(rom_01352C[OBJECT(r20)->unk_0C]);
	}
}

void sub_0135CC(void)
{
	uint16_t r0_2 = 0, r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_0135D8[t1];
	switch (t1)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		if (r9 == 0x3A2u)
			OBJECT(r20)->unk_24 = 0xFE80u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		OBJECT(r20)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(r20)->unk_0C++;
		r0_2 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0_2 - 0x64u;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0_2 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(r20)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 |= 0x40u;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		ignored = sub_00F30D(0u);
		if ((int16_t)OBJECT(r20)->unk_30 >= -0xB0)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 |= 0x40u;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 &= 0xBFu;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		sub_00F30D(ignored);
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 &= 0xBFu;
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

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r9 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_00 |= mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_3F = mem_read8(r10);
		r10++;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_80) = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_3E = mem_read8(r10);
		OBJECT(r20)->unk_44 = mem_read8(r10 + 1u);
		sub_00F366(r9);
		OBJECT(r20)->unk_0C++;
		r0 = OBJECT(r20)->unk_1C;
		OBJECT(r20)->unk_58 = r0 - 0x64u;
		r1 = mem_read16(r25 + 0x12u) >> 7;
		AT16(OBJECT(r20)->unk_54, 2) = r0 - r1;
		AS16(OBJECT(r20)->unk_10) = r1;
		return;
	case 1:
		if ((int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u) <= (int16_t)(uint16_t)(OBJECT(r20)->unk_54 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		if ((int16_t)OBJECT(r20)->unk_30 >= -0xB0)
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
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 |= 0x40u;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		if ((int16_t)OBJECT(r20)->unk_30 <= 0xF0)
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
		OBJECT(r20)->unk_0C--;
		OBJECT(r20)->unk_00 &= 0xBFu;
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
		OBJECT(r20)->unk_20 += (uint16_t)OBJECT(r20)->unk_80;
		if ((int16_t)OBJECT(r20)->unk_30 >= -0xB0)
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
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_00 |= 0x40u;
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		if ((int16_t)OBJECT(r20)->unk_30 <= 0xF0)
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
		OBJECT(r20)->unk_0C--;
		OBJECT(r20)->unk_00 &= 0xBFu;
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
		OBJECT(r20)->unk_20 -= (uint16_t)OBJECT(r20)->unk_80;
		if ((int16_t)OBJECT(r20)->unk_30 > 0xF0)
		{
			OBJECT(r20)->unk_0C--;
			OBJECT(r20)->unk_00 &= 0xBFu;
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
		invalid_jump(rom_0136A8[OBJECT(r20)->unk_0C]);
	}
}

void sub_01376C(void)
{
	uint32_t negative = 0, overflow = 0;
	uint16_t r0_2 = 0, r1 = 0, r6 = 0, r7 = 0, r8 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r21 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_1C = 0u;
		r0 = mem_read8(r25 + 0xA3u);
		OBJECT(r20)->unk_1E = r0;
		OBJECT(r20)->unk_18 = object_lists_2E1F0[r0];
		OBJECT(r20)->unk_2C = (uint8_t)(8u - mem_read8(mem_read32(r25 + 0x308u) + 0x1Fu) + unk_20E630);
		mem_write16(r25 + 0xFA6u, mem_read16(r25 + 0x12u) >> 7);
		mem_write32(r25 + 0xF80u, mem_read32(r25 + 0x308u) + 0x1Fu);
		mem_write8(r25 + 0xFA8u, 0u);
		OBJECT(r20)->unk_0C++;
		return;
	case 1:
		r10 = OBJECT(r20)->unk_18;
		if ((mem_read32(r25 + 0x90u) & 0x100u) == 0u && OBJECT(r20)->unk_1E != mem_read8(r25 + 0x352u))
		{
			OBJECT(r20)->unk_1C = mem_read16(r25 + 0x350u);
			r0 = mem_read8(r25 + 0x352u);
			OBJECT(r20)->unk_1E = r0;
			r10 = object_lists_2E1F0[r0];
			OBJECT(r20)->unk_18 = r10;
		}
		for (;;)
		{
			r0_2 = mem_read16(r10);
			t1 = OBJECT(r20)->unk_1C;
			t2 = r0_2 + t1;
			overflow = (uint32_t)(int16_t)r0_2 + (int16_t)t1 != (uint32_t)(int16_t)(r0_2 + t1);
			negative = t2 >> 15;
			r0_2 = t2;
			r10 += 2u;
			if (negative ^ overflow)
				break;
			r1 = (uint16_t)r0_2;
			if ((int16_t)mem_read16(mem_read32(r25 + 0x60u) + 0x46u) < (int16_t)(r0_2 - mem_read16(r25 + 0xFA6u)))
				break;
			t3 = mem_read16(r10);
			r10 += 2u;
			if (OBJECT(r20)->unk_2C < t3 || mem_read8(r25 + 0xFA8u) >= 0x20u)
			{
				r10 += 0xCu;
				OBJECT(r20)->unk_18 = r10;
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
			OBJECT(r20)->unk_18 = r10;
			mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) + 1u);
		}
		r10 = mem_read32(r25 + 0xF80u);
		r11 = r25 + 0xF84u;
		r6 = mem_read16(r25 + 0xFA6u);
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
		mem_write16(r25 + 0xF84u, r7);
		return;
	default:
		invalid_jump(rom_013778[OBJECT(r20)->unk_0C]);
	}
}

void sub_01389E(void)
{
	uint16_t r0 = 0;
	uint32_t r1 = 0, r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(r20)->unk_18;
	r0 = (uint16_t)mem_read32(r10);
	r11 = r20;
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
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) + 1u);
	}
	OBJECT(r20)->unk_18 = r10;
	OBJECT(r20)->unk_04 = sub_0138EB_ADDRESS;
	sub_0138EB(r1);
}

void sub_0138EB(uint32_t r1)
{
	uint32_t r0 = 0, r1_2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
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
	if (mem_read8(r23 + 0x181u) == 0u)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_28;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(r20)->unk_2C);
		r1_2 = (r1 & ~0xFFFFu) | 0x80u;
		if ((int16_t)r1_2 <= (int16_t)r0)
		{
			do
			{
				OBJECT(r20)->unk_1C++;
				r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1_2);
			} while ((int16_t)r1_2 <= (int16_t)r0);
		}
		sub_01397E(r0, r1_2);
		return;
	}
	r0 = (r0 & ~0xFFFFu) | OBJECT(r20)->unk_1C;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
	if ((int16_t)r0 < (int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u))
	{
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_28;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(r20)->unk_2C);
	r1_2 = (r1 & ~0xFFFFu) | 0x80u;
	if ((int16_t)r1_2 <= (int16_t)r0)
	{
		do
		{
			OBJECT(r20)->unk_1C++;
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

	r10 = OBJECT(r20)->unk_18;
	r11 = mem_read32(r10);
	t1 = mem_read16(r11);
	r11 += 2u;
	r9 = t1;
	OBJECT(r20)->unk_3E = 0xBu;
	t2 = mem_read16(r11);
	r11 += 2u;
	OBJECT(r20)->unk_3A = t2;
	OBJECT(r20)->unk_44 = 0xFFu;
	t3 = mem_read8(r11);
	r11++;
	AT8(OBJECT(r20)->unk_80, 1) = t3;
	t4 = mem_read8(r11);
	r11++;
	AT8(OBJECT(r20)->unk_80, 2) = t4;
	t5 = mem_read16(r11);
	AT16(OBJECT(r20)->unk_68, 2) = t5;
	AS32(OBJECT(r20)->unk_60) = r11 + 2u;
	r10 += 4u;
	OBJECT(r20)->unk_3F = (uint8_t)mem_read16(r10);
	r10 += 2u;
	AS32(OBJECT(r20)->unk_20) = (int16_t)mem_read16(r10);
	OBJECT(r20)->unk_2C = mem_read16(r10 + 2u);
	AS8(OBJECT(r20)->unk_80) = 0u;
	OBJECT(r20)->unk_0C++;
	sub_00F366(r9);
}

void sub_01397E(uint32_t r0, uint32_t r1)
{
	uint32_t zero = 0, negative = 0, carry = 0;
	uint8_t r0_3 = 0, r2_2 = 0;
	uint16_t r1_3 = 0, r2 = 0, r3 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r10 = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t t1 = 0;

	AS16(OBJECT(r20)->unk_28) = r0;
	r0_2 = (r0 & ~0xFFFFu) | OBJECT(r20)->unk_1C;
	zero = sub_013D29((uint16_t)r0_2);
	if (zero)
	{
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	r10 = mem_read32(r25 + 0x20u);
	r1_2 = (r1 & ~0xFFFFu) | (uint16_t)r0_2;
	r1_2 = (r1_2 & ~0xFFFFu) | (uint16_t)(r1_2 - mem_read16(r25 + 0xFA6u));
	r2 = mem_read16(r10 + 0x42u);
	if ((uint8_t)OBJECT(r20)->unk_80 != 0u)
	{
		if ((int16_t)r2 < (int16_t)r0_2)
		{
			AS8(OBJECT(r20)->unk_80) = ~(uint8_t)OBJECT(r20)->unk_80;
			stack_4 = r0_2;
			stack_8 = r1_2;
			r0_3 = (uint8_t)(OBJECT(r20)->unk_80 >> 8u);
			sub_011EB8(r0_3);
			r1_2 = stack_8;
			r0_2 = stack_4;
		}
	}
	else if ((int16_t)r0_2 < (int16_t)r2)
	{
		AS8(OBJECT(r20)->unk_80) = ~(uint8_t)OBJECT(r20)->unk_80;
		stack_4 = r0_2;
		stack_8 = r1_2;
		r0_3 = (uint8_t)(OBJECT(r20)->unk_80 >> 8u);
		sub_011EB8(r0_3);
		r1_2 = stack_8;
		r0_2 = stack_4;
	}
	r2 = mem_read16(r10 + 0x46u);
	if ((int16_t)r0_2 < (int16_t)r2 || (int16_t)r2 <= (int16_t)r1_2)
		return;
	r10 = r25 + 0x2000u + AS16(OBJECT(r23)->unk_40);
	r0_2 = mem_read16(r10);
	r2 = mem_read16(r10 + 2u);
	r10 = r25 + 0x2000u + (((uint32_t)OBJECT(r20)->unk_1C << 5) & 0xFFFu);
	r3 = mem_read16(r10 + 2u);
	r0_2 = (uint16_t)((int16_t)((r0_2 << 4) - ((uint32_t)mem_read16(r10) << 4)) >> 6);
	r1_3 = (uint16_t)((int16_t)OBJECT(r20)->unk_30 >> 2);
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
	r2 = (uint16_t)(((uint32_t)r2 << 4) - ((uint32_t)r3 << 4) + ((int16_t)((uint32_t)OBJECT(r23)->unk_2C << 4) >> 2));
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
	if ((uint8_t)(OBJECT(r20)->unk_84 >> 24u) != 0u)
	{
		r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + rom_013D45[(uint8_t)(OBJECT(r20)->unk_84 >> 24u)]);
		AT8(OBJECT(r20)->unk_84, 3)++;
		if ((uint8_t)(OBJECT(r20)->unk_84 >> 24u) == 0x15u)
			AT8(OBJECT(r20)->unk_84, 3) = 0u;
	}
	OBJECT(r20)->unk_40 = r0_2;
	negative = sub_01262A();
	if (negative)
	{
		sub_013CF6();
		AT8(OBJECT(r20)->unk_84, 3) = 1u;
	}
	r0_2 = sub_00EAE4(r0_2);
}

void sub_013AB7(void)
{
	if (mem_read8(OBJECT(r20)->unk_14) == 0u)
	{
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	sub_0138EB(0u);
	if (OBJECT(r20)->unk_00 == 0u)
	{
		OBJECT(r20)->unk_00 = 1u;
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) + 1u);
	}
}

void sub_013AD8(void)
{
	if (mem_read8(OBJECT(r20)->unk_14) == 0u)
	{
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	sub_013AF9();
	if (OBJECT(r20)->unk_00 == 0u)
	{
		OBJECT(r20)->unk_00 = 1u;
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) + 1u);
	}
}

void sub_013AF9(void)
{
	uint32_t r0 = 0, r1 = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
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
	if (mem_read8(r23 + 0x181u) != 0u)
	{
		r0 = (r0 & ~0xFFFFu) | OBJECT(r20)->unk_1C;
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - 8u);
		if ((int16_t)r0 < (int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u))
		{
			mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
	}
	r0 = (r0 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_28;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + OBJECT(r20)->unk_2C);
	r1 = 0x80u;
	if ((int16_t)r0 < (int16_t)r1)
	{
		sub_01397E(r0, r1);
		return;
	}
	OBJECT(r20)->unk_1C++;
	if ((uint8_t)(OBJECT(r20)->unk_84 >> 16u) == 0u)
	{
		t2 = (uint16_t)OBJECT(r20)->unk_84;
		AS16(OBJECT(r20)->unk_84) = t2 - 1u;
		if (t2 == 1u)
			AT8(OBJECT(r20)->unk_84, 2) = 1u;
	}
	else
	{
		t3 = OBJECT(r20)->unk_2C;
		OBJECT(r20)->unk_2C = t3 - 1u;
		if ((int16_t)t3 <= 1)
			OBJECT(r20)->unk_2C = 0u;
	}
	for (;;)
	{
		r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 - r1);
		if ((int16_t)r0 < (int16_t)r1)
			break;
		OBJECT(r20)->unk_1C++;
	}
	sub_01397E(r0, r1);
}

void sub_013B56(void)
{
	uint16_t r0 = 0;
	uint32_t r10 = 0, r11 = 0, r21 = 0;

	r10 = OBJECT(r20)->unk_18;
	r0 = (uint16_t)mem_read32(r10);
	r11 = r20;
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
		mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) + 1u);
	}
	OBJECT(r20)->unk_18 = r10;
	OBJECT(r20)->unk_04 = sub_013BA3_ADDRESS;
	sub_013BA3();
}

void sub_013BA3(void)
{
	uint32_t zero = 0, negative = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r10 = OBJECT(r20)->unk_18;
		r11 = mem_read32(r10);
		r9 = mem_read16(r11);
		r10 += 4u;
		r11 += 2u;
		if (r9 != 0x165u)
		{
			if (r9 == 0x258u || r9 == 0x185u)
				OBJECT(r20)->unk_44 = 0u;
			else
				OBJECT(r20)->unk_44 = 1u;
			OBJECT(r20)->unk_3A = mem_read16(r11);
			OBJECT(r20)->unk_3E = 5u;
			r11 += 2u;
			AT8(OBJECT(r20)->unk_80, 1) = mem_read8(r11);
			r11++;
			AT8(OBJECT(r20)->unk_80, 2) = mem_read8(r11);
			r11++;
			AT16(OBJECT(r20)->unk_68, 2) = mem_read16(r11);
			AS32(OBJECT(r20)->unk_60) = r11 + 2u;
			OBJECT(r20)->unk_3F = (uint8_t)mem_read16(r10);
			r10 += 2u;
			AS32(OBJECT(r20)->unk_20) = (int16_t)mem_read16(r10);
			OBJECT(r20)->unk_2C = mem_read16(r10 + 2u);
			OBJECT(r20)->unk_0C++;
			sub_00F366(r9);
			return;
		}
		OBJECT(r20)->unk_44 = 2u;
		OBJECT(r20)->unk_3A = mem_read16(r11);
		OBJECT(r20)->unk_3E = 4u;
		r11 += 2u;
		AT8(OBJECT(r20)->unk_80, 1) = mem_read8(r11);
		r11++;
		AT8(OBJECT(r20)->unk_80, 2) = mem_read8(r11);
		r11++;
		AT16(OBJECT(r20)->unk_68, 2) = mem_read16(r11);
		AS32(OBJECT(r20)->unk_60) = r11 + 2u;
		OBJECT(r20)->unk_3F = (uint8_t)mem_read16(r10);
		r10 += 2u;
		AS32(OBJECT(r20)->unk_20) = (int16_t)mem_read16(r10);
		OBJECT(r20)->unk_2C = mem_read16(r10 + 2u);
		AS32(OBJECT(r20)->unk_24) = (int16_t)0xFD40u;
		OBJECT(r20)->unk_0C++;
		sub_00F366(r9);
		return;
	case 1:
		if (mem_read8(r23 + 0x181u) != 0u && (int16_t)(OBJECT(r20)->unk_1C - 8u) < (int16_t)mem_read16(mem_read32(r25 + 0x20u) + 0x46u))
		{
			mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		r0 = (uint16_t)((uint16_t)OBJECT(r20)->unk_28 + OBJECT(r20)->unk_2C);
		r1 = 0x80u;
		if ((int16_t)r0 >= 0x80)
		{
			do
			{
				OBJECT(r20)->unk_1C++;
				if ((uint8_t)(OBJECT(r20)->unk_84 >> 16u) == 0u)
				{
					t1 = (uint16_t)OBJECT(r20)->unk_84;
					AS16(OBJECT(r20)->unk_84) = t1 - 1u;
					if (t1 == 1u)
						AT8(OBJECT(r20)->unk_84, 2) = 1u;
				}
				else
				{
					t2 = OBJECT(r20)->unk_2C;
					OBJECT(r20)->unk_2C = t2 - 1u;
					if ((int16_t)t2 <= 1)
						OBJECT(r20)->unk_2C = 0u;
				}
				r0 -= r1;
			} while ((int16_t)r1 <= (int16_t)r0);
		}
		AS16(OBJECT(r20)->unk_28) = r0;
		r0_2 = OBJECT(r20)->unk_1C;
		zero = sub_013D29((uint16_t)r0_2);
		if (zero)
		{
			mem_write8(r25 + 0xFA8u, mem_read8(r25 + 0xFA8u) - 1u);
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		r10 = mem_read32(r25 + 0x20u);
		r1 = (uint16_t)(r0_2 - mem_read16(r25 + 0xFA6u));
		if ((int16_t)r0_2 < (int16_t)mem_read16(r10 + 0x46u) || (int16_t)mem_read16(r10 + 0x46u) <= (int16_t)r1)
			return;
		if (mem_read32(AS32(OBJECT(r20)->unk_60)) != 0u)
		{
			negative = sub_01262A();
			if (negative)
				sub_013CF6();
		}
		sub_00EAE4(r0_2);
		ignored = sub_00F30D(0u);
		return;
	default:
		invalid_jump(rom_013BB0[OBJECT(r20)->unk_0C]);
	}
}

void sub_013CF6(void)
{
	uint32_t r21 = 0;

	if ((uint8_t)(OBJECT(r20)->unk_80 >> 16u) != 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_013D15_ADDRESS);
		mem_write8(r21 + 0x10u, 0xFu);
		mem_write8(r21 + 0x12u, (uint8_t)(OBJECT(r20)->unk_80 >> 16u));
	}
}

void sub_013D15(void)
{
	uint8_t r0 = 0;
	uint8_t t1 = 0;

	t1 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(r20)->unk_00 = 0u;
		r0 = OBJECT(r20)->unk_12;
		sub_011EB8(r0);
		return;
	}
}

uint32_t sub_013D29(uint16_t r0)
{
	uint32_t zero = 0;
	uint32_t r9 = 0, r10 = 0;
	uint16_t t1 = 0, t2 = 0;

	r10 = r25 + 0xF84u;
	r9 = mem_read16(r10);
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
	OBJECT(r20)->unk_04 = sub_013DE4_ADDRESS;
	OBJECT(r20)->unk_80 = 0xFFFFFFFFu;
	sub_013DE4();
}

void sub_013DD5(void)
{
	OBJECT(r20)->unk_04 = sub_013DE4_ADDRESS;
	OBJECT(r20)->unk_80 = 1u;
	sub_013DE4();
}

void sub_013DE4(void)
{
	uint16_t r0 = 0, r9 = 0, r16 = 0, r17 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r13 = 0, r15 = 0, r18 = 0, r19 = 0, r21 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r0 = OBJECT(r20)->unk_1C;
		AT16(OBJECT(r20)->unk_54, 2) = r0;
		OBJECT(r20)->unk_58 = r0 + 0x28u;
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_013DF0[OBJECT(r20)->unk_0C]);
	}
	r9 = mem_read16(mem_read32(r25 + 0x20u) + 0x46u);
	r16 = (uint16_t)(OBJECT(r20)->unk_54 >> 16u);
	r17 = OBJECT(r20)->unk_58;
	if (r17 <= r9)
	{
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	}
	r19 = r25 + 0x2FF0u;
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
		r26 = r19;
		r26 = r26 & 0xFFFu;
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
	r26 = (uint16_t)(r26 + 0x800u);
	r26 = (int16_t)rom_101400[r26];
	r26 = r26 + 0x4000u;
	r26 = r26 * 0x5186u;
	r26 = (int32_t)r26 >> 14;
	r26 = r26 * r4;
	r26 = (int32_t)r26 >> 12;
	r26 = r26 * OBJECT(r20)->unk_80;
	r2 += r26;
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
		r26 = r19;
		r26 = r26 & 0xFFFu;
		r26 = r26 >> 4;
		r26 = (uint16_t)(r26 + r9);
		if (r17 < r26)
			break;
		r26 = (uint16_t)(r26 - r16);
		r26 = (uint16_t)(r26 << 3);
		r26 = (uint16_t)(r26 + 0x800u);
		r26 = (int16_t)rom_101400[r26];
		r26 = r26 + 0x4000u;
		r26 = r26 * 0x5186u;
		r26 = (int32_t)r26 >> 14;
		r26 = r26 * r5;
		r26 = (int32_t)r26 >> 12;
		r26 = r26 * OBJECT(r20)->unk_80;
		r3 += r26;
		if ((int32_t)r3 > 0x280 || (int32_t)r3 < -0x280)
			break;
		r10 = mem_read32(r25 + 0xC0u) + 0x10u;
		r8 = r0_2 - r1;
		r18 += r8;
		if ((int32_t)(r18 - 1u) < 0)
			continue;
		if (r18 == 1u)
		{
			if (mem_read32(r25 + 0xC4u) <= r10)
				break;
			r18 = 0u;
			mem_write32(r10 + 0x3000u, 0u);
			mem_write32(r10, r12);
			r10 += 4u;
			mem_write16(r10, r13);
			r26 = r4;
			r26 = r26 * 0x370u;
			r26 = r26 >> 12;
			r26 = (uint16_t)(r26 | r14);
			r10 += 2u;
			mem_write16(r10, r26);
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
					if (mem_read32(r25 + 0xC4u) <= r10)
						return;
					mem_write32(r10 + 0x3000u, 0u);
					mem_write32(r10, r12);
					r10 += 4u;
					mem_write16(r10, r13);
					r26 = r4;
					r27 = r5;
					r26 = r26 * r7;
					r27 = r27 * r8;
					r27 = r27 + r26;
					r27 = divide_s32(r27, r6);
					r27 = r27 * 0x370u;
					r27 = r27 >> 12;
					r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 | r14);
					r10 += 2u;
					mem_write16(r10, r27);
					r10 += 2u;
					mem_write16(r10, r0);
					r0++;
					r26 = r2;
					r27 = r3;
					r26 = r26 * r7;
					r27 = r27 * r8;
					r27 = r27 + r26;
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
		r27 = r25 + 0x5000u + r21 * 4u;
		if (mem_read16(r27) == 0u)
			mem_write16(r27 + 2u, r10);
		r26 = mem_read32(r25 + 0xC0u) + 0x10u;
		r28 = mem_read16(r27);
		mem_write32(r10, r28);
		r28 = (uint16_t)(r28 + 0x300u);
		mem_write32(r10 + 0x3000u, r28);
		mem_write32(r25 + 0xC0u, r10);
		r26 = r26 >> 4;
		r26 = (r26 & ~0xFFFFu) | (uint16_t)(r26 | 0x8000u);
		mem_write16(r27, r26);
		mem_write16(r25 + 0xB0u, mem_read16(r25 + 0xB0u) + 1u);
	}
}

void sub_01405F(uint32_t *p_r13, uint32_t *p_r15)
{
	uint16_t r13 = 0;

	r26 = rom_034308;
	r26 = mem_read32(r26 + 4u);
	r27 = (r27 & ~0xFFFFu) | mem_read16(r26 + 2u);
	r26 = mem_read16(r26 + 4u);
	r26 = 0x122B74u + r26 * 4u;
	r12 = ((uint32_t)ROM16(r26) << 16) | (uint16_t)r27;
	r26 = r26 + 2u;
	r27 = (r27 & ~0xFFFFu) | ROM16(r26);
	r26 = r26 + 2u;
	r26 = r26 + 2u;
	r13 = 1u | ROM16(r26);
	r26 = r26 + 2u;
	r14 = ROM16(r26);
	r26 = r26 + 2u;
	r14 = ROM16(r26);
	r14 = r14 | mem_read16(r25 + 0x1Au);
	*p_r15 = ((uint32_t)mem_read16(rom_032BA4[rom_0140C8[mem_read8(r25 + 0xA3u)]]) << 16) | (uint16_t)r27;
	r26 = r26 + 2u;
	*p_r13 = (*p_r13 & 0xFFFF0000u) | r13;
}

void sub_014108(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_3E = 0u;
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
		r9 = rom_01440E[r0];
		OBJECT(r20)->unk_3A = rom_01442E[r0];
		sub_00F366(r9);
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		if (mem_read8(r10 + 0x2Eu) == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_32 = mem_read16(r10 + 0x32u) + mem_read16(r25 + 0x16u);
		OBJECT(r20)->unk_40 = 0u;
		OBJECT(r20)->unk_41 = 1u;
		AS8(OBJECT(r20)->unk_42) = 1u;
		break;
	case 1:
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		if (mem_read8(r10 + 0x2Eu) == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_32 = mem_read16(r10 + 0x32u) + mem_read16(r25 + 0x16u);
		OBJECT(r20)->unk_40 = 0u;
		OBJECT(r20)->unk_41 = 1u;
		AS8(OBJECT(r20)->unk_42) = 1u;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_014114[OBJECT(r20)->unk_0C]);
	}
	r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
	mem_write8(r10 + 0xCCu, 0u);
	mem_write8(r10 + 0xDCu, 0u);
	if (mem_read8(r10 + 0x2Eu) == 0u)
	{
		sub_00F30D(0u);
		if (OBJECT(r20)->unk_41 == 0u)
		{
			r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
			OBJECT(r20)->unk_0C--;
			mem_write8(r10 + 0x2Eu, (uint8_t)(OBJECT(r20)->unk_08 >> 16u));
			return;
		}
		return;
	}
	OBJECT(r20)->unk_3C = mem_read16(r10 + 0x3Cu);
	r0 = mem_read16(r10 + 0x34u);
	if (r0 > 0x100u)
		r0 = 0x100u;
	OBJECT(r20)->unk_34 = r0;
	OBJECT(r20)->unk_36 = r0;
	OBJECT(r20)->unk_30 = mem_read16(r10 + 0x30u) + mem_read16(r25 + 0x14u);
	if (mem_read8(r10 + 0xBDu) != 0u)
		OBJECT(r20)->unk_32 = ((int32_t)((int16_t)mem_read16(mem_read32(r10 + 0x80u) + 0x56u) * r0) >> 8) + mem_read16(r10 + 0x32u) + mem_read16(r25 + 0x16u);
	sub_00EAE4(r0);
	sub_00F30D(0u);
	if (OBJECT(r20)->unk_41 == 0u)
	{
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		OBJECT(r20)->unk_0C--;
		mem_write8(r10 + 0x2Eu, (uint8_t)(OBJECT(r20)->unk_08 >> 16u));
		return;
	}
}

void sub_0141F4(void)
{
	uint32_t zero = 0;
	uint8_t r9_2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_3E = 0u;
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
		r9 = rom_01444E[r0];
		if (r9 == 0u)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		OBJECT(r20)->unk_3A = rom_01442E[r0];
		sub_00F366(r9);
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		r9_2 = 0xFFu;
		zero = sub_0142D2(r9_2, r10);
		if (zero == 0u)
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_40 = 0u;
			OBJECT(r20)->unk_41 = 1u;
			AS8(OBJECT(r20)->unk_42) = 1u;
			mem_write8(r10 + 0xE1u, 0xFFu);
		}
		return;
	case 1:
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		r9_2 = 0xFFu;
		zero = sub_0142D2(r9_2, r10);
		if (zero == 0u)
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_40 = 0u;
			OBJECT(r20)->unk_41 = 1u;
			AS8(OBJECT(r20)->unk_42) = 1u;
			mem_write8(r10 + 0xE1u, 0xFFu);
		}
		return;
	case 2:
		r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
		zero = sub_0142B4(r10);
		if (zero)
		{
			sub_00F30D(0u);
			if (OBJECT(r20)->unk_41 == 0u)
			{
				r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
				OBJECT(r20)->unk_0C--;
				mem_write8(r10 + 0xE1u, 0u);
				return;
			}
			return;
		}
		OBJECT(r20)->unk_3C = mem_read16(r10 + 0x3Cu);
		OBJECT(r20)->unk_3C++;
		r0 = mem_read16(r10 + 0x34u);
		if (r0 > 0x100u)
			r0 = 0x100u;
		OBJECT(r20)->unk_34 = r0;
		OBJECT(r20)->unk_36 = r0;
		sub_0142F7((uint16_t)r0, r10);
		sub_00EAE4(r0);
		sub_00F30D(0u);
		if (OBJECT(r20)->unk_41 == 0u)
		{
			r10 = (r25 & ~0xFFFFu) | (uint16_t)OBJECT(r20)->unk_14;
			OBJECT(r20)->unk_0C--;
			mem_write8(r10 + 0xE1u, 0u);
			return;
		}
		return;
	default:
		invalid_jump(rom_014200[OBJECT(r20)->unk_0C]);
	}
}

uint32_t sub_0142B4(uint32_t r10)
{
	uint32_t zero = 0;
	uint32_t r9 = 0;
	uint8_t t1 = 0;

	r9 = rom_0143A0[mem_read8(AS32(OBJECT(r23)->unk_00) + 0xCu)];
	r9 = r9 & rom_0143A6[mem_read8(r10 + 0xE3u)];
	if (r9 == 0u)
	{
		t1 = mem_read8(r10 + 0xE1u) & r9;
		zero = t1 == 0u;
		mem_write8(r10 + 0xE1u, t1);
		return zero;
	}
	return sub_0142D2((uint8_t)r9, r10);
}

uint32_t sub_0142D2(uint8_t r9, uint32_t r10)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint8_t t1 = 0;

	r0 = mem_read8(r25 + 0x320u + (uint8_t)(OBJECT(r20)->unk_08 >> 24u));
	r1 = mem_read8(r10 + 0xE2u);
	mem_write8(r10 + 0xE2u, r0);
	if (r0 < r1)
		mem_write8(r10 + 0xE1u, 0xFFu);
	t1 = mem_read8(r10 + 0xE1u) & r9;
	zero = t1 == 0u;
	mem_write8(r10 + 0xE1u, t1);
	return zero;
}

void sub_0142F7(uint16_t r0, uint32_t r10)
{
	uint16_t r6 = 0, r7 = 0, r9 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0, r11 = 0;

	r3 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	r6 = rom_014360[r3];
	r7 = rom_014380[r3];
	r1 = (uint8_t)(OBJECT(r20)->unk_08 >> 16u);
	r2 = mem_read8(r10 + 0xE3u);
	r3 = (uint8_t)(r3 - r1);
	r11 = rom_18D25C[r1] + (uint16_t)(r2 << 4) + (r3 >> 1);
	r9 = (uint16_t)(mem_read16(r10 + 0x32u) + mem_read16(r25 + 0x16u) + ((int16_t)((uint32_t)(r7 + mem_read16(r11 + 2u)) * r0) >> 8));
	OBJECT(r20)->unk_30 = mem_read16(r10 + 0x30u) + mem_read16(r25 + 0x14u) + ((int16_t)((uint32_t)(r6 + mem_read16(r11)) * r0) >> 8);
	OBJECT(r20)->unk_32 = r9;
}

void sub_014470(void)
{
	uint16_t t1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
	case 1:
		break;
	case 2:
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		return;
	default:
		invalid_jump(rom_01447C[OBJECT(r20)->unk_0C]);
	}
	switch (OBJECT(r20)->unk_0D)
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
		invalid_jump(rom_0144A9[OBJECT(r20)->unk_0D]);
	}
	switch (OBJECT(r20)->unk_0E)
	{
	case 0:
		OBJECT(r20)->unk_0E++;
		return;
	case 1:
		mem_write16(r23 + 0x10Cu, mem_read16(r23 + 0x10Cu) + 0x12u);
		t1 = mem_read16(r23 + 0x10Eu);
		mem_write16(r23 + 0x10Eu, t1 - 3u);
		if (t1 != 3u)
			return;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_0E++;
		return;
	case 2:
		return;
	default:
		invalid_jump(rom_0144EC[OBJECT(r20)->unk_0E]);
	}
}

void sub_0144B5(void)
{
	OBJECT(r20)->unk_0D++;
	OBJECT(r20)->unk_12 = rom_014660;
	OBJECT(r20)->unk_18 = 0x14661u;
}

void sub_0144C9(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	OBJECT(r20)->unk_10++;
	if (OBJECT(r20)->unk_10 < OBJECT(r20)->unk_12)
	{
		sub_0144EB();
		return;
	}
	r10 = OBJECT(r20)->unk_18;
	r0 = mem_read8(r10);
	r10++;
	sub_011EB8(r0);
	if (mem_read8(r10) != 0u)
	{
		OBJECT(r20)->unk_12 = mem_read8(r10);
		OBJECT(r20)->unk_18 = r10 + 1u;
		return;
	}
	OBJECT(r20)->unk_0C++;
	OBJECT(r20)->unk_0D++;
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

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		AT8(OBJECT(r20)->unk_08, 1) = mem_read8(mem_read32(r25 + 0x20u) + 0x60u);
		r10 = 0x1466Fu;
		OBJECT(r20)->unk_10 = mem_read8(r10);
		OBJECT(r20)->unk_18 = r10 + 1u;
		return;
	case 1:
		if ((uint8_t)(OBJECT(r20)->unk_08 >> 8u) != mem_read8(mem_read32(r25 + 0x20u) + 0x60u))
		{
			OBJECT(r20)->unk_0C++;
			return;
		}
		t1 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t1 - 1u;
		if (t1 != 1u)
			return;
		r10 = OBJECT(r20)->unk_18;
		r0 = mem_read8(r10);
		r10++;
		sub_011EB8(r0);
		if (mem_read8(r10) != 0u)
		{
			OBJECT(r20)->unk_10 = mem_read8(r10);
			OBJECT(r20)->unk_18 = r10 + 1u;
			return;
		}
		OBJECT(r20)->unk_0C++;
		return;
	case 2:
		if ((uint8_t)(OBJECT(r20)->unk_08 >> 8u) == mem_read8(mem_read32(r25 + 0x20u) + 0x60u))
			return;
		r0 = 0x24u;
		if (mem_read8(r25 + 1u) != mem_read8(r25 + 0x70u))
			r0 = 0x28u;
		OBJECT(r20)->unk_00 = 0u;
		OBJECT(r20)->unk_01 = 0u;
		sub_011EB8(r0);
		return;
	default:
		invalid_jump(rom_014520[OBJECT(r20)->unk_0C]);
	}
}

void sub_01458D(void)
{
	uint16_t r0_2 = 0;
	uint32_t r0 = 0;
	uint16_t t1 = 0, t2 = 0;

	if (mem_read8(r23 + 0x137u) != 0u)
		return;
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		if (mem_read16(mem_read32(r25 + 0x20u) + 0x42u) < OBJECT(r20)->unk_1C)
			return;
		OBJECT(r20)->unk_0C++;
		r0 = mem_read8(mem_read32(r25 + 0x20u) + 0x49u);
		AS16(OBJECT(r20)->unk_84) = 0u - rom_010BD2[r0] + 0x1E0u;
		AS16(OBJECT(r20)->unk_80) = rom_014674[r0];
		AT16(OBJECT(r20)->unk_80, 2) = rom_014684[r0];
		AT16(OBJECT(r20)->unk_84, 2) = OBJECT(r20)->unk_1C + 0xE0u;
		mem_write16(r23 + 0x10Au, 0u);
		mem_write16(r23 + 0x108u, 0u);
		return;
	case 1:
		r0_2 = (uint16_t)(mem_read16(r23 + 0x10Au) + (uint16_t)OBJECT(r20)->unk_80);
		if ((int16_t)(uint16_t)OBJECT(r20)->unk_84 <= (int16_t)r0_2)
		{
			OBJECT(r20)->unk_0C++;
			r0_2 = (uint16_t)OBJECT(r20)->unk_84;
			mem_write8(r25 + 0xF0Fu, 0xFFu);
		}
		mem_write16(r23 + 0x10Au, r0_2);
		mem_write16(r23 + 0x108u, mem_read16(r23 + 0x108u) - (uint16_t)(OBJECT(r20)->unk_80 >> 16u));
		return;
	case 2:
		if (mem_read16(mem_read32(r25 + 0x20u) + 0x42u) < (uint16_t)(OBJECT(r20)->unk_84 >> 16u))
			return;
		OBJECT(r20)->unk_0C++;
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_0145A0[OBJECT(r20)->unk_0C]);
	}
	t1 = (uint16_t)OBJECT(r20)->unk_80;
	t2 = mem_read16(r23 + 0x10Au);
	r0_2 = (uint16_t)(t2 - t1);
	if ((int16_t)t1 < (int16_t)t2)
	{
		mem_write16(r23 + 0x10Au, r0_2);
		mem_write16(r23 + 0x108u, mem_read16(r23 + 0x108u) + (uint16_t)(OBJECT(r20)->unk_80 >> 16u));
		return;
	}
	OBJECT(r20)->unk_00 = 0u;
	OBJECT(r20)->unk_01 = 0u;
	mem_write16(r23 + 0x10Au, 0u);
	mem_write16(r23 + 0x108u, 0u);
}

void sub_014694(void)
{
	bool L_0146F4 = false;
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0, r21 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_0C++;
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 16u);
		r0 = (uint8_t)(r0 * 2u);
		OBJECT(r20)->unk_30 += 0xA0u;
		OBJECT(r20)->unk_32 += 0xDEu;
		r1 = mem_read8(r25 + 1u);
		for (;;)
		{
			if (L_0146F4 || mem_read8(mem_read32(r25 + 0x20u) + 0x5Eu) == 0u)
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
					mem_write32(r21 + 0x14u, r20);
					r10 += 4u;
					r0--;
				} while (r0 != 0u);
				return;
			}
			if (mem_read8(r25 + 0x320u + r1) != 0u)
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
		OBJECT(r20)->unk_0C++;
		r0_2 = 0x4Fu;
		sub_011EB8(r0_2);
		mem_write16(r25 + 0x94u, mem_read16(r25 + 0x94u) | 2u);
		mem_write16(r25 + 0x94u, mem_read16(r25 + 0x94u) & ~0x10u);
		if (mem_read8(mem_read32(r25 + 0x20u) + 0x5Eu) == 0u)
		{
			sub_011FE6();
			return;
		}
		break;
	default:
		invalid_jump(rom_0146A0[OBJECT(r20)->unk_0C]);
	}
}

void sub_014735(void)
{
	uint32_t r9 = 0, r10 = 0;

	r10 = OBJECT(r20)->unk_14;
	r9 = mem_read16(mem_read32(r25 + 0x20u) + 0x3Cu);
	OBJECT(r20)->unk_38 = r9;
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = mem_read16(mem_read32(r25 + 0x20u) + 0x3Eu);
	OBJECT(r20)->unk_40 = mem_read8(mem_read32(r25 + 0x20u) + 0x4Au);
	OBJECT(r20)->unk_3C = mem_read16(r10 + 0x3Cu);
	OBJECT(r20)->unk_30 = mem_read16(r10 + 0x30u);
	OBJECT(r20)->unk_32 = mem_read16(r10 + 0x32u);
	OBJECT(r20)->unk_34 = mem_read16(r10 + 0x34u);
	OBJECT(r20)->unk_36 = mem_read16(r10 + 0x34u);
	sub_014838();
}

void sub_014777(void)
{
	uint32_t r10 = 0;

	r10 = OBJECT(r20)->unk_14;
	r12 = r10 + 0x90u;
	sub_01478B(r10);
}

void sub_014782(void)
{
	uint32_t r10 = 0;

	r10 = OBJECT(r20)->unk_14;
	r12 = r10 + 0xA0u;
	sub_01478B(r10);
}

void sub_01478B(uint32_t r10)
{
	uint32_t carry = 0;
	uint32_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r11 = 0, r19 = 0;

	OBJECT(r20)->unk_3A = mem_read16(r12 + 6u);
	OBJECT(r20)->unk_3C = mem_read16(r10 + 0x3Cu);
	r19 = mem_read32(r25 + 0x20u);
	r11 = mem_read32(rom_034138[mem_read16(r19 + 0x3Cu)] + 4u + mem_read8(r19 + 0x4Au) * 4u) + 2u;
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
	OBJECT(r20)->unk_38 = r9;
	sub_00F366(r9);
	OBJECT(r20)->unk_40 = mem_read8(r12 + 0xAu);
	r7 = mem_read16(r10 + 0x34u);
	OBJECT(r20)->unk_34 = r7;
	OBJECT(r20)->unk_36 = r7;
	r5 = (int16_t)r5 * r7;
	r6 = (int16_t)r6 * r7;
	carry = (r5 >> 7) & 1u;
	r5 = (uint16_t)(((int32_t)r5 >> 8) + carry);
	carry = (r6 >> 7) & 1u;
	r6 = (uint16_t)(((int32_t)r6 >> 8) + carry + mem_read16(r10 + 0x32u));
	OBJECT(r20)->unk_30 = r5 + mem_read16(r10 + 0x30u);
	OBJECT(r20)->unk_32 = r6;
	sub_014838();
}

void sub_014838(void)
{
	OBJECT(r20)->unk_04 = sub_014846_ADDRESS;
	OBJECT(r20)->unk_3E = 0u;
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

	r10 = OBJECT(r20)->unk_80;
	for (;;)
	{
		r5 = mem_read16(r10);
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		r10 += 2u;
		if (carry)
		{
			AS16(OBJECT(r20)->unk_10)++;
			t1 = mem_read16(r10);
			r10 += 2u;
			if (AS16(OBJECT(r20)->unk_10) <= t1)
			{
				r0 = sub_00EAE4(r0);
				return;
			}
			OBJECT(r20)->unk_10 = 0u;
			OBJECT(r20)->unk_11 = 0u;
			OBJECT(r20)->unk_80 = r10;
			continue;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			AS16(OBJECT(r20)->unk_10)++;
			t2 = mem_read16(r10);
			r10 += 2u;
			if (AS16(OBJECT(r20)->unk_10) <= t2)
			{
				r10 = OBJECT(r20)->unk_84;
				OBJECT(r20)->unk_80 = r10;
				continue;
			}
			OBJECT(r20)->unk_10 = 0u;
			OBJECT(r20)->unk_11 = 0u;
			OBJECT(r20)->unk_80 = r10;
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
			OBJECT(r20)->unk_40 = (uint8_t)mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_30 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_30 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_32 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_32 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_34 += mem_read16(r10);
			OBJECT(r20)->unk_36 += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_34 = mem_read16(r10);
			OBJECT(r20)->unk_36 = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_3C += mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_3C = mem_read16(r10);
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			r9 = mem_read16(r10);
			OBJECT(r20)->unk_38 = r9;
			r9_2 = mem_read8(rom_034138[r9]);
			OBJECT(r20)->unk_41 = r9_2;
			AS8(OBJECT(r20)->unk_42) = r9_2;
			OBJECT(r20)->unk_40 = 0u;
			r10 += 2u;
		}
		carry = (uint16_t)(r5 * 2u) < r5;
		r5 = (uint16_t)(r5 * 2u);
		if (carry)
		{
			OBJECT(r20)->unk_3A = mem_read16(r10);
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
			r10 = OBJECT(r20)->unk_80;
			OBJECT(r20)->unk_80 = t3;
			OBJECT(r20)->unk_84 = r10;
			r0 = sub_00EAE4(r0);
			return;
		}
		t4 = mem_read16(r10);
		r0 = rom_014994[t4];
		r10 += 2u;
		switch (t4)
		{
		case 0:
			mem_write8(OBJECT(r20)->unk_14 + 0xCu, 2u);
			t3 = r10;
			r10 = OBJECT(r20)->unk_80;
			OBJECT(r20)->unk_80 = t3;
			OBJECT(r20)->unk_84 = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 1:
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		case 2:
			t5 = r10;
			r10 = OBJECT(r20)->unk_80;
			OBJECT(r20)->unk_80 = t5;
			OBJECT(r20)->unk_84 = r10;
			return;
		case 3:
			OBJECT(r20)->unk_00 ^= 0x40u;
			t3 = r10;
			r10 = OBJECT(r20)->unk_80;
			OBJECT(r20)->unk_80 = t3;
			OBJECT(r20)->unk_84 = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 4:
			OBJECT(r23)->unk_34 += 0x4000u;
			t3 = r10;
			r10 = OBJECT(r20)->unk_80;
			OBJECT(r20)->unk_80 = t3;
			OBJECT(r20)->unk_84 = r10;
			r0 = sub_00EAE4(r0);
			return;
		case 5:
			break;
		default:
			invalid_jump(rom_014994[t4]);
		}
		AS16(OBJECT(r20)->unk_10)++;
		r0 = (uint16_t)(mem_read16(r10) + ((uint32_t)rom_014A16[mem_read8(r25 + 0x320u + mem_read8(r25 + 1u))] << 3));
		r10 += 2u;
		if (AS16(OBJECT(r20)->unk_10) <= r0)
		{
			r10 = OBJECT(r20)->unk_84;
			OBJECT(r20)->unk_80 = r10;
			continue;
		}
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_80 = r10;
	}
}

void sub_016B00(void)
{
	uint32_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r13 = 0, r15 = 0, r16 = 0, r18 = 0, r19 = 0, r21 = 0, ignored = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		sub_016C23();
		AS8(OBJECT(r20)->unk_80) = 0x1Eu;
		OBJECT(r20)->unk_0C++;
		return;
	case 1:
		sub_016D00(&ignored, &ignored, &ignored, &ignored, &ignored, &ignored, &ignored);
		OBJECT(r20)->unk_0F ^= 0x60u;
		OBJECT(r20)->unk_0C++;
		if (OBJECT(r20)->unk_0E == 0u)
		{
			r19 = mem_read32(r25 + 0x208u);
			sub_0184D0(r19);
			OBJECT(r20)->unk_0C++;
		}
		break;
	case 2:
		sub_016D00(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		sub_016E8C(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(r20)->unk_0E == 0u)
		{
			r19 = mem_read32(r25 + 0x208u);
			sub_0184D0(r19);
			OBJECT(r20)->unk_0C++;
		}
		break;
	case 3:
		sub_016D00(&r4, &r5, &r6, &r13, &r15, &r16, &r18);
		sub_016E8C(r4, r5, r6, r13, (uint8_t)r15, r16, (uint16_t)r18);
		if (OBJECT(r20)->unk_0E != 0u)
		{
			OBJECT(r20)->unk_0D = OBJECT(r20)->unk_0E;
			r19 = mem_read32(r25 + 0x208u);
			sub_0184EC(r19);
			OBJECT(r20)->unk_0C--;
		}
		break;
	default:
		invalid_jump(rom_016B0C[OBJECT(r20)->unk_0C]);
	}
	r0 = AS16(OBJECT(r20)->unk_10);
	r0 = (uint16_t)(r0 + 0x4FFu);
	mem_write32(OBJECT(r20)->unk_54, mem_read32(r20 - 0x24u + ((r0 >> 9) + OBJECT(r20)->unk_0D * 5u) * 4u));
	if ((uint16_t)OBJECT(r20)->unk_14 <= (uint16_t)(OBJECT(r20)->unk_14 >> 16u))
		return;
	AT16(OBJECT(r20)->unk_14, 2) = (uint16_t)OBJECT(r20)->unk_14;
	if ((uint8_t)OBJECT(r20)->unk_80 != 0u)
	{
		OBJECT(r20)->unk_13 = OBJECT(r20)->unk_12;
		AS8(OBJECT(r20)->unk_80)--;
		return;
	}
	r0 = OBJECT(r20)->unk_12;
	if (OBJECT(r20)->unk_13 == r0)
		return;
	OBJECT(r20)->unk_13 = r0;
	switch (r0)
	{
	case 4:
		mem_write8(r25 + 0x203u, 0xFFu);
		mem_write8(r25 + 0x204u, 0u);
		return;
	case 5:
		mem_write8(r25 + 0x203u, 0xFFu);
		mem_write8(r25 + 0x204u, 1u);
		return;
	case 0:
		return;
	case 1:
		t1 = ~(uint8_t)(OBJECT(r20)->unk_08 >> 16u);
		AT8(OBJECT(r20)->unk_08, 2) = t1;
		if (t1 == 0u)
			return;
		sub_017188();
		return;
	case 3:
		t2 = ~(uint8_t)(OBJECT(r20)->unk_08 >> 16u);
		AT8(OBJECT(r20)->unk_08, 2) = t2;
		if (t2 != 0u)
			sub_017188();
		t3 = ~(uint8_t)(OBJECT(r20)->unk_08 >> 24u);
		AT8(OBJECT(r20)->unk_08, 3) = t3;
		if (t3 == 0u)
			return;
		r21 = sub_011D43();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, r20);
		mem_write32(r21 + 0x3Cu, r20 + 0xBu);
		mem_write32(r21 + 0x38u, 0x174B4u);
		mem_write32(r21 + 4u, sub_0171CE_ADDRESS);
		return;
	case 2:
		t3 = ~(uint8_t)(OBJECT(r20)->unk_08 >> 24u);
		AT8(OBJECT(r20)->unk_08, 3) = t3;
		if (t3 == 0u)
			return;
		r21 = sub_011D43();
		mem_write8(r21 + 9u, 1u);
		mem_write32(r21 + 0x14u, r20);
		mem_write32(r21 + 0x3Cu, r20 + 0xBu);
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

	r10 = 0x1710Cu + ((uint32_t)mem_read8(r25) << 3) * 4u;
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
	OBJECT(r20)->unk_54 = ROM32(r10);
	r10 += 4u;
	r11 = ROM32(r10);
	r0_2 = mem_read32(r11);
	r0_2 = (r0_2 & ~0xFFFFu) | (r0_2 & 0xFFF0u);
	mem_write32(r11, (r0_2 & ~0xFFu) | (uint8_t)(r0_2 | 8u));
	mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) | 4u);
	r10 += 4u;
	AS32(OBJECT(r20)->unk_58) = ROM32(r10);
	r10 = ROM32(r10 + 4u);
	OBJECT(r20)->unk_5C = r10;
	move_halfwords(r10 - 0x1C0u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x640u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0xE40u, 0x100800u, 0xE0u);
	move_halfwords(r10 + 0x1640u, 0x100800u, 0xE0u);
	OBJECT(r20)->unk_0D = rom_03CC80[mem_read8(r25 + 0xA0u)];
	OBJECT(r20)->unk_0F = 0u;
	OBJECT(r20)->unk_13 = 0u;
	AT16(OBJECT(r20)->unk_14, 2) = 0u;
	AT8(OBJECT(r20)->unk_08, 2) = 0u;
	AT8(OBJECT(r20)->unk_08, 3) = 0u;
	move_string(0x1714Cu, 0x1Eu, r20 + 0x18u, 0x1Eu, 2u, 0u, 0u, r26, &r27, &r28);
	r19 = rom_031C38[mem_read8(r25 + 0xA0u)];
	mem_write32(r25 + 0x208u, mem_read32(r19));
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
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r19 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint16_t t1 = 0, t3 = 0;
	uint32_t t2 = 0, t4 = 0, t5 = 0;

	r10 = r25 + 0x3000u;
	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 8u);
	r0 = (r0 & 0xCu) << 7;
	r11 = AS32(OBJECT(r20)->unk_58) + r0 * 4u;
	stack_4 = r11;
	r12 = OBJECT(r20)->unk_5C + r0 * 4u;
	r13 = mem_read16(mem_read32(r25 + 0x20u) + 0x46u);
	r14 = r14 & ~0xFFFFu;
	r15 = 0xFFFFFFF4u;
	r16 = 0x800u;
	r17 = (int16_t)mem_read16(r25 + 0x16u);
	r18 = 0xFFu;
	r19 = 1u;
	r26 = OBJECT(r20)->unk_0F;
	r26 = r26 * 2u;
	r26 = r25 + 0x3800u + r26 * 4u;
	r0 = 0xE0u;
	while (mem_read16(r10) == 0u)
	{
		r10 += 0x10u;
		r13++;
	}
	OBJECT(r20)->unk_0E = mem_read8(r10 + 0xCu);
	OBJECT(r20)->unk_12 = mem_read8(r10 + 0xDu);
	AS16(OBJECT(r20)->unk_14) = r13;
	mem_write16(r26, r13);
	r13 = 0u - ((r13 & ~0xFFFFu) | (r13 & 3u));
	overflow = v60_overflow_add32(r13, 2u, 0u);
	zero = r13 + 2u == 0u;
	negative = (r13 + 2u) >> 31;
	r13 += 2u;
	r26 = r26 + 2u;
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
		r26 = r26 + 2u;
		r10 += 2u;
		r2 = 0u - (int16_t)mem_read16(r10);
		mem_write16(r26, r2);
		r26 = r26 + 2u;
		r10 += 2u;
		r1 = (int16_t)mem_read16(r10) + r17;
		mem_write16(r26, r1);
		r26 = r26 + 2u;
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
												r26 = r26 + 2u;
												r10 += 2u;
												mem_write16(r26, 0u - mem_read16(r10));
												r26 = r26 + 2u;
												r10 += 2u;
												r1 = (int16_t)mem_read16(r10) + r17;
												mem_write16(r26, r1);
												r26 = r26 + 2u;
												r10 += 0xAu;
											}
											mem_write16(r25 + 0x200u, r1);
											mem_write16(r26, 0xFFFFu);
											sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
											AS16(OBJECT(r20)->unk_10) = mem_read16(stack_4 - 2u);
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
							OBJECT(r20)->unk_60 = 0u;
							OBJECT(r20)->unk_61 = 0u;
							OBJECT(r20)->unk_62 = 0u;
							OBJECT(r20)->unk_63 = 0u;
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
								OBJECT(r20)->unk_64 = r5;
								OBJECT(r20)->unk_68 = r6;
								sub_016FEB(r0, r1, r2, r3, (uint8_t)r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
								OBJECT(r20)->unk_6C = r7;
								AS32(OBJECT(r20)->unk_70) = r8;
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
			AS32(OBJECT(r20)->unk_60) = r1;
			AS32(OBJECT(r20)->unk_74) = r11;
			OBJECT(r20)->unk_78 = r12;
			AS32(OBJECT(r20)->unk_7C) = r14;
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
		mem_write16(r25 + 0x200u, r1);
		mem_write16(r26, 0xFFFFu);
		sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
		AS16(OBJECT(r20)->unk_10) = mem_read16(stack_4 - 2u);
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
		mem_write16(r25 + 0x200u, r1);
		mem_write16(r26, 0xFFFFu);
		sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
		AS16(OBJECT(r20)->unk_10) = mem_read16(stack_4 - 2u);
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
	mem_write16(r25 + 0x200u, r1);
	mem_write16(r26, 0xFFFFu);
	sub_0170A6((uint8_t)r15, r16, r18, &r5, &r6);
	AS16(OBJECT(r20)->unk_10) = mem_read16(stack_4 - 2u);
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
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r7 = 0, r8 = 0, r9 = 0, r11 = 0, r13_2 = 0, r19 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint32_t t6 = 0, t7 = 0, t8 = 0;

	r0 = (uint8_t)(unk_20E204 >> 8u);
	r0 = (uint8_t)(r0 + 4u);
	r0 = (r0 & 0xCu) << 7;
	r11 = AS32(OBJECT(r20)->unk_58) + r0 * 4u;
	r12 = OBJECT(r20)->unk_5C + r0 * 4u;
	r14 = r14 & ~0xFFFFu;
	r19 = 1u;
	r0 = 0xE0u;
	r27 = OBJECT(r20)->unk_0F;
	r28 = r27;
	r28 = r28 ^ 0x60u;
	OBJECT(r20)->unk_0F = r28;
	r27 = r27 * 2u;
	r28 = r28 * 2u;
	r27 = r25 + 0x3800u + r27 * 4u;
	r28 = r25 + 0x3800u + r28 * 4u;
	r26 = (r26 & ~0xFFFFu) | mem_read16(r27);
	t1 = mem_read16(r28);
	r28 = r28 + 2u;
	r13_2 = (r13 & ~0xFFFFu) | t1;
	r27 = r27 + 2u;
	if ((uint16_t)r13_2 != (uint16_t)r26)
	{
		if ((uint16_t)r13_2 < (uint16_t)r26)
		{
			do
			{
				r28 = r28 + 6u;
				r13_2 = (r13_2 & ~0xFFFFu) | (uint16_t)(r13_2 + 1u);
			} while ((uint16_t)r13_2 < (uint16_t)r26);
		}
		else
		{
			do
			{
				r27 = r27 + 6u;
				r26 = (r26 & ~0xFFFFu) | (uint16_t)(r26 + 1u);
			} while ((uint16_t)r26 < (uint16_t)r13_2);
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
	for (;;)
	{
		r26 = (r26 & ~0xFFFFu) | mem_read16(r27);
		t2 = r26 | mem_read16(r28);
		r26 = (r26 & ~0xFFFFu) | t2;
		if ((int16_t)t2 < 0)
			break;
		t3 = mem_read16(r28);
		r28 = r28 + 2u;
		r26 = t3;
		r3 = shift_by(mem_read16(r27) + r26, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t4 = mem_read16(r28);
		r28 = r28 + 2u;
		r26 = (int16_t)t4;
		r2 = shift_signed_by((int16_t)mem_read16(r27) + r26, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		t5 = mem_read16(r28);
		r28 = r28 + 2u;
		r26 = (int16_t)t5;
		r1 = shift_signed_by((int16_t)mem_read16(r27) + r26, (uint8_t)r18, 32);
		r27 = r27 + 2u;
		if (r19 == 0u)
		{
			t6 = r0;
			r9 = t6 - r1;
			if ((int32_t)r1 < (int32_t)t6)
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
			t7 = r0;
			r9 = t7 - r1;
			if ((int32_t)r1 < (int32_t)t7)
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
						OBJECT(r20)->unk_64 = r5;
						OBJECT(r20)->unk_68 = r6;
						sub_016FEB(r0, r1, r2, r3, r15, r16, r18, &r4, &r5, &r6, &r7, &r8, &r11, &zero);
						OBJECT(r20)->unk_6C = r7;
						AS32(OBJECT(r20)->unk_70) = r8;
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
					AS32(OBJECT(r20)->unk_60) = r1;
					AS32(OBJECT(r20)->unk_74) = r11;
					OBJECT(r20)->unk_78 = r12;
					AS32(OBJECT(r20)->unk_7C) = r14;
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
				OBJECT(r20)->unk_60 = 0u;
				OBJECT(r20)->unk_61 = 0u;
				OBJECT(r20)->unk_62 = 0u;
				OBJECT(r20)->unk_63 = 0u;
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
	t8 = shift_by(r0, (uint8_t)r18, 32);
	carry = v60_shl_cy(r0, (uint8_t)r18, 32);
	zero = t8 == 0u;
	r0 = t8;
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

	r1 = AS32(OBJECT(r20)->unk_60);
	if (r1 == 0u)
		return;
	*p_r5 = (OBJECT(r20)->unk_64 << 12) + r16;
	*p_r6 = (OBJECT(r20)->unk_68 << 12) + r16;
	r7 = OBJECT(r20)->unk_6C;
	r8 = AS32(OBJECT(r20)->unk_70);
	r11 = AS32(OBJECT(r20)->unk_74);
	r12 = OBJECT(r20)->unk_78;
	r14 = AS32(OBJECT(r20)->unk_7C);
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
	mem_write32(r21 + 0x14u, r20);
	mem_write32(r21 + 0x3Cu, r20 + 0xAu);
	mem_write32(r21 + 0x38u, 0x17444u);
	mem_write32(r21 + 4u, sub_0171CE_ADDRESS);
}

void sub_0171CE(void)
{
	uint32_t r0 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r21 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;

	r21 = AS32(OBJECT(r20)->unk_38);
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r10 = r20 + 0x18u;
		r11 = r20 + 0x28u;
		r0 = mem_read8(OBJECT(r20)->unk_14 + 0xDu);
		AT8(OBJECT(r20)->unk_08, 2) = r0;
		r12 = r21 - 0xCu + r0 * 4u;
		r13 = r21 + 0x18u + mem_read8(r25) * 4u;
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
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_12 = 4u;
		OBJECT(r20)->unk_0C++;
	case 1:
		r0 = OBJECT(r20)->unk_10;
		move_string(mem_read32(r20 + 0x18u + r0 * 4u), 0x200u, mem_read32(r20 + 0x28u + r0 * 4u), 0x200u, 2u, 0u, 0u, r26, &r27, &r28);
		OBJECT(r20)->unk_10++;
		t2 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t2 - 1u;
		if (t2 != 1u)
			return;
		r10 = r21 + 0x20u + mem_read8(r25) * 5u * 2u;
		r11 = OBJECT(r20)->unk_14 + 0x18u + (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
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
		r10 = r20 + 0x18u;
		r11 = r21 + (uint8_t)(OBJECT(r20)->unk_08 >> 16u) * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(r20)->unk_10 = 0x10u;
		OBJECT(r20)->unk_0C++;
		return;
	case 2:
		r0 = 0x10u;
		r10 = r20 + 0x18u;
		r11 = r20 + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, r26, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		t3 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t3 - 1u;
		if (t3 != 1u)
			return;
		r10 = r21 + 0x34u;
		r11 = OBJECT(r20)->unk_14 + 0x18u + (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, mem_read8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(r20)->unk_0C++;
	case 3:
		if (mem_read8(AS32(OBJECT(r20)->unk_3C)) != 0u)
			return;
		r10 = r20 + 0x18u;
		r11 = r20 + 0x28u;
		r0 = mem_read8(OBJECT(r20)->unk_14 + 0xDu);
		AT8(OBJECT(r20)->unk_08, 2) = r0;
		r12 = r21 + r0 * 4u;
		r13 = r21 + 0x18u + mem_read8(r25) * 4u;
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
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_12 = 4u;
		OBJECT(r20)->unk_0C++;
	case 4:
		r0 = OBJECT(r20)->unk_10;
		move_string(mem_read32(r20 + 0x18u + r0 * 4u), 0x200u, mem_read32(r20 + 0x28u + r0 * 4u), 0x200u, 2u, 0u, 0u, r26, &r27, &r28);
		OBJECT(r20)->unk_10++;
		t5 = OBJECT(r20)->unk_12;
		OBJECT(r20)->unk_12 = t5 - 1u;
		if (t5 != 1u)
			return;
		r10 = r21 + 0x20u + mem_read8(r25) * 5u * 2u;
		r11 = OBJECT(r20)->unk_14 + 0x18u + (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
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
		r10 = r20 + 0x18u;
		r11 = r21 - 0xCu + (uint8_t)(OBJECT(r20)->unk_08 >> 16u) * 4u;
		r9 = 4u;
		do
		{
			mem_write32(r10, 0x300000u + ((uint32_t)mem_read8(r11) << 8) * 4u);
			r9--;
			r11++;
			r10 += 4u;
		} while (r9 != 0u);
		OBJECT(r20)->unk_10 = 0x10u;
		OBJECT(r20)->unk_0C++;
		return;
	case 5:
		r0 = 0x10u;
		r10 = r20 + 0x18u;
		r11 = r20 + 0x28u;
		r9 = 4u;
		do
		{
			r12 = mem_read32(r10);
			move_string(r12, r0, mem_read32(r11), r0, 2u, 0u, 0u, r26, &r27, &r28);
			mem_write32(r10, r28);
			mem_write32(r11, r27);
			r9--;
			r10 += 4u;
			r11 += 4u;
		} while (r9 != 0u);
		t6 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t6 - 1u;
		if (t6 != 1u)
			return;
		r10 = r21 + 0x52u;
		r11 = OBJECT(r20)->unk_14 + 0x18u + (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
		r9 = 0x1Eu;
		do
		{
			mem_write8(r11, mem_read8(r10));
			r9--;
			r10++;
			r11 += 2u;
		} while (r9 != 0u);
		OBJECT(r20)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_0171E0[OBJECT(r20)->unk_0C]);
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_017530[t1];
	switch (t1)
	{
	case 0:
		r0 = mem_read8(r25);
		OBJECT(r20)->unk_0E = r0;
		r10 = 0x17A50u + r0 * 5u * 4u;
		OBJECT(r20)->unk_18 = ROM32(r10);
		r10 += 4u;
		AS32(OBJECT(r20)->unk_1C) = ROM32(r10);
		r10 += 4u;
		AS32(OBJECT(r20)->unk_24) = ROM32(r10);
		r10 += 4u;
		r11 = ROM32(r10);
		AS32(OBJECT(r20)->unk_20) = r11;
		mem_write32(r11, rom_017AE0[(uint32_t)OBJECT(r20)->unk_0E << 3]);
		r10 = ROM32(r10 + 4u);
		r0 = mem_read32(r10);
		r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
		mem_write32(r10, (r0 & ~0xFFu) | (uint8_t)(r0 | 4u));
		mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) | 4u);
		r0 = mem_read8(r25 + 0xA0u);
		if (r0 != 0u)
			r0 = (uint8_t)~rom_017B60[r0];
		t2 = ~r0;
		OBJECT(r20)->unk_0F = t2;
		r10 = r20 + 0x2Cu;
		r11 = 0x109684u;
		if ((int8_t)t2 >= 0)
			r11 = rom_109694[(uint8_t)~r0] + 0x10u;
		r12 = r10;
		r0 = OBJECT(r20)->unk_0E * 5u * 2u + 1u;
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
		mem_write8(AS32(OBJECT(r20)->unk_24), rom_017BA9[mem_read8(r25 + 0xA0u)]);
		mem_write8(r25 + 0x202u, 0u);
		mem_write8(r25 + 0x203u, 0u);
		mem_write8(r25 + 0x205u, 0u);
		mem_write8(r25 + 0x206u, 0xFFu);
		mem_write8(r25 + 0x207u, 0xFFu);
		r0 = mem_read8(r25 + 0xA0u);
		sub_017BC8(r0);
		r19 = 0x13F9D4u;
		r0 = OBJECT(r20)->unk_0F;
		if ((int8_t)r0 >= 0)
		{
			r19 = rom_031B14[r0];
			sub_01850B(r19);
			r19 = rom_0318CC[OBJECT(r20)->unk_0F];
		}
		sub_0115C5(r19);
		OBJECT(r20)->unk_0C++;
		r1 = mem_read8(r25 + 0xA0u);
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
		if (mem_read8(r25 + 0x203u) != 0u)
			L_017685 = true;
		else if (mem_read8(r23 + 0xCAu) != 0u)
		{
			mem_write8(r23 + 0xCAu, 0u);
			L_017685 = true;
		}
		if (L_017685)
		{
			mem_write8(r25 + 0x202u, 0xFFu);
			r0 = mem_read8(r25 + 0xA3u);
			if (mem_read8(r25 + 0x203u) == 0u)
			{
				r0 = r0 * 2u;
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + mem_read8(r23 + 0xC9u));
			}
			else
				r0 = (uint8_t)(r0 + 0x2Au);
			OBJECT(r20)->unk_0F = r0;
			r10 = r20 + 0x2Cu;
			r11 = rom_109694[r0];
			r12 = r10;
			r0 = OBJECT(r20)->unk_0E;
			r0 = r0 ^ 2u;
			OBJECT(r20)->unk_0E = r0;
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
			r0 = OBJECT(r20)->unk_0E * 2u;
			r1 = mem_read8(r25 + 0x204u);
			if (mem_read8(r25 + 0x203u) == 0u)
				r1 = mem_read8(r23 + 0xC9u);
			OBJECT(r20)->unk_28 = rom_017AE0_ADDRESS + ((r0 & ~0xFFu) | (uint8_t)(r0 + r1)) * 2u * 2u * 4u;
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_0D = 0u;
		}
		break;
	case 2:
		r0 = (r0 & ~0xFFFFu) | OBJECT(r23)->unk_2E;
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
		switch (OBJECT(r20)->unk_0D)
		{
		case 0:
			if (zero == 0u)
			{
				mem_write8(r25 + 0x206u, 0xFFu);
				r0 = OBJECT(r20)->unk_0F;
				r19 = rom_0318CC[r0];
				sub_0115C5(r19);
				mem_write8(AS32(OBJECT(r20)->unk_24), rom_017B7F[r0]);
				r10 = OBJECT(r20)->unk_28;
				mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
				OBJECT(r20)->unk_28 = r10 + 4u;
				OBJECT(r20)->unk_0D++;
			}
			break;
		case 1:
			if ((int16_t)r0 >= 0x2A0)
			{
				mem_write8(r25 + 0x205u, 0xFFu);
				r19 = rom_0319F0[OBJECT(r20)->unk_0F];
				sub_0184D0(r19);
				r19 = rom_031CB8[OBJECT(r20)->unk_0F];
				if (r19 == 0u)
				{
					r10 = OBJECT(r20)->unk_28;
					mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
					OBJECT(r20)->unk_28 = r10 + 4u;
					OBJECT(r20)->unk_0D++;
				}
				else
				{
					mem_write32(r25 + 0x208u, mem_read32(r19));
					r19 += 4u;
					sub_0184D0(r19);
					r10 = OBJECT(r20)->unk_28;
					mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
					OBJECT(r20)->unk_28 = r10 + 4u;
					OBJECT(r20)->unk_0D++;
				}
			}
			break;
		case 2:
			stack_4 = r0;
			mem_write8(r25 + 0x205u, 0u);
			mem_write8(r25 + 0x207u, 0u);
			r0 = mem_read8(r25 + 0xA1u);
			sub_017BC8(r0);
			r0 = stack_4;
			OBJECT(r20)->unk_0D++;
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(r20)->unk_0F];
				sub_01850B(r19);
				r10 = OBJECT(r20)->unk_28;
				mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
				OBJECT(r20)->unk_28 = r10 + 4u;
				OBJECT(r20)->unk_0D++;
			}
			break;
		case 3:
			if ((int16_t)r0 <= 0x160)
			{
				r19 = rom_031B14[OBJECT(r20)->unk_0F];
				sub_01850B(r19);
				r10 = OBJECT(r20)->unk_28;
				mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
				OBJECT(r20)->unk_28 = r10 + 4u;
				OBJECT(r20)->unk_0D++;
			}
			break;
		case 4:
			if ((int16_t)r0 <= 0x60)
			{
				r10 = OBJECT(r20)->unk_28;
				mem_write32(AS32(OBJECT(r20)->unk_20), mem_read32(r10));
				OBJECT(r20)->unk_28 = r10 + 4u;
				OBJECT(r20)->unk_0D++;
			}
			break;
		case 5:
			if (zero)
			{
				mem_write8(r25 + 0x202u, 0u);
				mem_write8(r25 + 0x203u, 0u);
				OBJECT(r20)->unk_0C--;
				r1 = OBJECT(r20)->unk_0F;
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
							r0_2 = OBJECT(r23)->unk_2E;
							if (mem_read8(r25 + 0x202u) == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(r20)->unk_18, r0_2 - 0xA0u);
							t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)OBJECT(r23)->unk_30] >> 6) + mem_read16(r25 + 0xB8u) - 1u;
							r0_2 = t5;
							if ((int16_t)t5 >= 0)
							{
								r1_2 = (uint16_t)(0u - mem_read16(r25 + 0x200u) + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							mem_write16(r25 + 0x20Cu, r0_2);
							mem_write16(AS32(OBJECT(r20)->unk_1C), r0_2);
							return;
						}
						r0 = 1u;
						if (r1 == 0x26u)
						{
							sub_01791C(r0);
							r0_2 = OBJECT(r23)->unk_2E;
							if (mem_read8(r25 + 0x202u) == 0u)
								r0_2 = r0_2 >> 2;
							mem_write16(OBJECT(r20)->unk_18, r0_2 - 0xA0u);
							t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)OBJECT(r23)->unk_30] >> 6) + mem_read16(r25 + 0xB8u) - 1u;
							r0_2 = t5;
							if ((int16_t)t5 >= 0)
							{
								r1_2 = (uint16_t)(0u - mem_read16(r25 + 0x200u) + 0x180u);
								if ((int16_t)r1_2 < (int16_t)r0_2)
									r0_2 = (uint16_t)r1_2;
							}
							else
								r0_2 = 0u;
							mem_write16(r25 + 0x20Cu, r0_2);
							mem_write16(AS32(OBJECT(r20)->unk_1C), r0_2);
							return;
						}
					}
				}
			}
			break;
		default:
			invalid_jump(rom_017738[OBJECT(r20)->unk_0D]);
		}
		break;
	default:
		invalid_jump(rom_017530[t1]);
	}
	r0_2 = OBJECT(r23)->unk_2E;
	if (mem_read8(r25 + 0x202u) == 0u)
		r0_2 = r0_2 >> 2;
	mem_write16(OBJECT(r20)->unk_18, r0_2 - 0xA0u);
	t5 = ((int16_t)rom_100C00[(uint32_t)(int16_t)OBJECT(r23)->unk_30] >> 6) + mem_read16(r25 + 0xB8u) - 1u;
	r0_2 = t5;
	if ((int16_t)t5 >= 0)
	{
		r1_2 = (uint16_t)(0u - mem_read16(r25 + 0x200u) + 0x180u);
		if ((int16_t)r1_2 < (int16_t)r0_2)
			r0_2 = (uint16_t)r1_2;
	}
	else
		r0_2 = 0u;
	mem_write16(r25 + 0x20Cu, r0_2);
	mem_write16(AS32(OBJECT(r20)->unk_1C), r0_2);
}

void sub_01789C(void)
{
	uint32_t r10 = 0, r21 = 0;

	r10 = rom_017904[mem_read8(r25)];
	sub_011011(r10);
	r21 = sub_011D43();
	mem_write8(r21 + 0x10u, 2u);
	mem_write8(r21 + 0x12u, mem_read8(r25));
	mem_write32(r21 + 0x20u, AS32(OBJECT(r20)->unk_20));
	mem_write32(r21 + 4u, sub_0178C9_ADDRESS);
}

void sub_0178C9(void)
{
	if (mem_read16(r25 + 0xB00u) != 0u)
		return;
	OBJECT(r20)->unk_04 = sub_0178D5_ADDRESS;
	sub_0178D5();
}

void sub_0178D5(void)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (mem_read8(r25 + 0x202u) != 0u)
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(r20)->unk_10 = 2u;
		r0 = OBJECT(r20)->unk_12;
		r0 = (uint8_t)(r0 + 2u);
		r0 = r0 & 7u;
		OBJECT(r20)->unk_12 = r0;
		mem_write16(AS32(OBJECT(r20)->unk_20) + 2u, rom_01790C[r0]);
	}
}

void sub_01791C(uint32_t r0)
{
	uint32_t r10 = 0, r21 = 0;

	r0 = r0 * 2u;
	r10 = rom_017988[(r0 & ~0xFFu) | (uint8_t)(r0 + mem_read8(r25))];
	sub_011011(r10);
	r21 = sub_011D43();
	mem_write8(r21 + 0x10u, 3u);
	mem_write8(r21 + 0x12u, mem_read8(r25));
	mem_write32(r21 + 0x20u, AS32(OBJECT(r20)->unk_20));
	mem_write32(r21 + 4u, sub_01794C_ADDRESS);
}

void sub_01794C(void)
{
	if (mem_read16(r25 + 0xB00u) != 0u)
		return;
	OBJECT(r20)->unk_04 = sub_017958_ADDRESS;
	sub_017958();
}

void sub_017958(void)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (mem_read8(r25 + 0x202u) != 0u)
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(r20)->unk_10 = 3u;
	r0 = OBJECT(r20)->unk_12;
	r0 = (uint8_t)(r0 + 2u);
	if ((int8_t)r0 >= 6)
		r0 = (uint8_t)(r0 - 6u);
	OBJECT(r20)->unk_12 = r0;
	mem_write16(AS32(OBJECT(r20)->unk_20), rom_0179A0[r0]);
}

void sub_0179AC(uint32_t r19)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0, r19_2 = 0;
	uint8_t t3 = 0, t4 = 0;
	uint32_t t1 = 0, t2 = 0;

	r10 = rom_017A78[mem_read8(r25)];
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
	mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) | 4u);
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

	if (mem_read8(r25 + 0x203u) != 0u)
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
			mem_write32(r21 + 0x14u, r20);
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

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r10 = OBJECT(r20)->unk_80;
		AS16(OBJECT(r20)->unk_10) = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(r20)->unk_12) = mem_read16(r10);
		r10 += 2u;
		AS32(OBJECT(r20)->unk_38) = mem_read32(r10);
		r0 = (uint16_t)mem_read32(r10 + 4u);
		AS16(OBJECT(r20)->unk_14) = r0;
		AT16(OBJECT(r20)->unk_14, 2) = r0;
		OBJECT(r20)->unk_0D = 0u;
		OBJECT(r20)->unk_0C++;
		if ((OBJECT(r20)->unk_10 | OBJECT(r20)->unk_11) != 0u)
		{
			t1 = AS16(OBJECT(r20)->unk_10);
			AS16(OBJECT(r20)->unk_10) = t1 - 1u;
			if (t1 != 1u)
				return;
		}
		OBJECT(r20)->unk_0C++;
		if ((OBJECT(r20)->unk_12 | OBJECT(r20)->unk_13) != 0u)
		{
			t2 = AS16(OBJECT(r20)->unk_12);
			AS16(OBJECT(r20)->unk_12) = t2 - 1u;
			if (t2 == 1u)
			{
				OBJECT(r20)->unk_00 = 0u;
				return;
			}
		}
		else
			OBJECT(r20)->unk_0C++;
		break;
	case 1:
		t1 = AS16(OBJECT(r20)->unk_10);
		AS16(OBJECT(r20)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(r20)->unk_0C++;
		if ((OBJECT(r20)->unk_12 | OBJECT(r20)->unk_13) != 0u)
		{
			t2 = AS16(OBJECT(r20)->unk_12);
			AS16(OBJECT(r20)->unk_12) = t2 - 1u;
			if (t2 == 1u)
			{
				OBJECT(r20)->unk_00 = 0u;
				return;
			}
		}
		else
			OBJECT(r20)->unk_0C++;
		break;
	case 2:
		t2 = AS16(OBJECT(r20)->unk_12);
		AS16(OBJECT(r20)->unk_12) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		break;
	case 3:
		break;
	default:
		invalid_jump(rom_017C08[OBJECT(r20)->unk_0C]);
	}
	switch (OBJECT(r20)->unk_0D)
	{
	case 0:
		if (mem_read8(r25 + 0x206u) != 0u)
			return;
		OBJECT(r20)->unk_0D++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017C68[OBJECT(r20)->unk_0D]);
	}
	if (mem_read8(r25 + 0x206u) != 0u)
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	t3 = (uint16_t)(OBJECT(r20)->unk_14 >> 16u);
	AT16(OBJECT(r20)->unk_14, 2) = t3 - 1u;
	if (t3 != 1u)
		return;
	AT16(OBJECT(r20)->unk_14, 2) = (uint16_t)OBJECT(r20)->unk_14;
	r10 = AS32(OBJECT(r20)->unk_38);
	r19 = mem_read32(r10);
	r10 += 4u;
	if ((int32_t)r19 < 0)
	{
		r10 += r19;
		r19 = mem_read32(r10);
		r10 += 4u;
	}
	AS32(OBJECT(r20)->unk_38) = r10;
	sub_0115C5(r19);
}

void sub_017CAA(void)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r10 = 0, r11 = 0, r17 = 0, r18 = 0, r19 = 0, r21 = 0;
	uint32_t t1 = 0, t2 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r10 = OBJECT(r20)->unk_80;
		AS32(OBJECT(r20)->unk_10) = mem_read32(r10);
		AS32(OBJECT(r20)->unk_38) = mem_read32(r10 + 4u);
		OBJECT(r20)->unk_0C++;
		if ((OBJECT(r20)->unk_10 | OBJECT(r20)->unk_11) != 0u)
		{
			t1 = AS32(OBJECT(r20)->unk_10);
			AS32(OBJECT(r20)->unk_10) = t1 - 1u;
			if (t1 != 1u)
				return;
		}
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		t1 = AS32(OBJECT(r20)->unk_10);
		AS32(OBJECT(r20)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(r20)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017CB8[OBJECT(r20)->unk_0C]);
	}
	if (mem_read8(r25 + 0x206u) != 0u)
		return;
	r19 = AS32(OBJECT(r20)->unk_38);
	t2 = mem_read32(r19) / 0xCu;
	r18 = t2;
	r19 += 4u;
	if (t2 == 0u)
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	AS32(OBJECT(r20)->unk_10) = r18;
	r17 = r20 + 0x18u;
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
	OBJECT(r20)->unk_04 = sub_017D1F_ADDRESS;
}

void sub_017D1F(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = r20 + 0x18u;
	r9 = AS32(OBJECT(r20)->unk_10);
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
		OBJECT(r20)->unk_00 = 0u;
	if (mem_read8(r25 + 0x206u) == 0u)
		return;
	r10 = r20 + 0x18u;
	r9 = AS32(OBJECT(r20)->unk_10);
	do
	{
		r1 = mem_read32(r10);
		r10 += 4u;
		if (r1 != 0u)
			mem_write8(r1, 0u);
		r9--;
	} while (r9 != 0u);
	OBJECT(r20)->unk_00 = 0u;
}

void sub_017E1B(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (mem_read8(r25 + 0x207u) != 0u)
		OBJECT(r20)->unk_0C++;
	OBJECT(r20)->unk_0D = 0u;
	r10 = OBJECT(r20)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_24 = mem_read16(r10);
	AS16(OBJECT(r20)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(r20)->unk_3E = 0xAu;
	OBJECT(r20)->unk_3C = 1u;
	sub_00F366(r9);
	OBJECT(r20)->unk_04 = sub_017E5B_ADDRESS;
	sub_017E5B();
}

void sub_017E5B(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(r20)->unk_0D |= mem_read8(r25 + 0x205u);
	r0 = OBJECT(r23)->unk_2E;
	if (mem_read8(r25 + 0x202u) == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(r20)->unk_20) & 0x3FFu;
	OBJECT(r20)->unk_30 = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_017E98[OBJECT(r20)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(r20)->unk_0D != 0u)
			OBJECT(r20)->unk_00 = 0u;
		return;
	}
	r1 = OBJECT(r20)->unk_24;
	r0_2 = (uint16_t)(mem_read16(r25 + 0x20Cu) - 0xFFu - r1);
	r1 -= mem_read16(r25 + 0x200u);
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(r20)->unk_32);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(r20)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_017EF2(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (mem_read8(r25 + 0x207u) != 0u)
		OBJECT(r20)->unk_0C++;
	OBJECT(r20)->unk_0D = 0u;
	r10 = OBJECT(r20)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_24 = mem_read16(r10);
	r10 += 2u;
	AS16(OBJECT(r20)->unk_28) = mem_read16(r10);
	OBJECT(r20)->unk_3E = 0xAu;
	OBJECT(r20)->unk_3C = 2u;
	sub_00F366(r9);
	AS16(OBJECT(r20)->unk_10) = 0u - mem_read32(r10 + 2u) + OBJECT(r23)->unk_42;
	OBJECT(r20)->unk_04 = sub_017F3D_ADDRESS;
	sub_017F3D();
}

void sub_017F3D(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0;

	OBJECT(r20)->unk_0D |= mem_read8(r25 + 0x205u);
	r0 = OBJECT(r23)->unk_2E;
	if (mem_read8(r25 + 0x202u) == 0u)
		r0 = r0 >> 2;
	r0 = (r0 - OBJECT(r20)->unk_20) & 0x3FFu;
	OBJECT(r20)->unk_30 = 0u - (r0 + (r0 >> 2) - 0x320u);
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		r0_2 = OBJECT(r23)->unk_42;
		r0_2 = (uint16_t)(r0_2 - AS16(OBJECT(r20)->unk_10));
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(r20)->unk_0C++;
		}
		OBJECT(r20)->unk_34 = r0;
		OBJECT(r20)->unk_36 = r0;
		break;
	case 1:
		r0_2 = OBJECT(r23)->unk_42;
		r0_2 = (uint16_t)(r0_2 - AS16(OBJECT(r20)->unk_10));
		r0 = rom_100C00[r0_2 >> 1] >> 6;
		if ((int16_t)r0 >= 0x100)
		{
			r0 = 0x100u;
			OBJECT(r20)->unk_0C++;
		}
		OBJECT(r20)->unk_34 = r0;
		OBJECT(r20)->unk_36 = r0;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_017F7C[OBJECT(r20)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(r20)->unk_0D != 0u)
			OBJECT(r20)->unk_00 = 0u;
		return;
	}
	r1 = OBJECT(r20)->unk_24;
	r0_2 = (uint16_t)(mem_read16(r25 + 0x20Cu) - 0xFFu - r1);
	r1 -= mem_read16(r25 + 0x200u);
	if ((int16_t)r1 < (int16_t)r0_2)
		r0_2 = r1;
	r1 = (uint16_t)(0u - r0_2 - OBJECT(r20)->unk_32);
	if ((int16_t)r1 > 1 || (int16_t)r1 < -1)
		OBJECT(r20)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_0180CB(void)
{
	uint32_t r9 = 0, r10 = 0;

	if (mem_read8(r25 + 0x207u) != 0u)
		OBJECT(r20)->unk_0C++;
	OBJECT(r20)->unk_0D = 0u;
	r10 = OBJECT(r20)->unk_80;
	r9 = mem_read32(r10);
	r10 += 4u;
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_20 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_24 = mem_read16(r10);
	AS16(OBJECT(r20)->unk_28) = mem_read16(r10 + 2u);
	OBJECT(r20)->unk_3E = 0xAu;
	OBJECT(r20)->unk_3C = 1u;
	sub_00F366(r9);
	OBJECT(r20)->unk_10 = 0u;
	OBJECT(r20)->unk_04 = sub_018110_ADDRESS;
	sub_018110();
}

void sub_018110(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0;
	uint32_t r0_2 = 0;

	OBJECT(r20)->unk_0D |= mem_read8(r25 + 0x205u);
	r0 = OBJECT(r23)->unk_2E;
	if (mem_read8(r25 + 0x202u) == 0u)
		r0 = r0 >> 2;
	OBJECT(r20)->unk_30 = 0u - (((r0 - OBJECT(r20)->unk_20) & 0x3FFu) - 0x2A0u);
	sub_00F30D(0u);
	OBJECT(r20)->unk_10++;
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		zero = sub_01818A();
		if (zero == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01814C[OBJECT(r20)->unk_0C]);
	}
	zero = sub_01818A();
	if (zero)
	{
		if (OBJECT(r20)->unk_0D != 0u)
			OBJECT(r20)->unk_00 = 0u;
		return;
	}
	if ((OBJECT(r20)->unk_10 & 1u) != 0u)
		return;
	r0_2 = (uint16_t)(mem_read16(r25 + 0x20Cu) - 0xFFu - OBJECT(r20)->unk_24);
	OBJECT(r20)->unk_32 = 0u - r0_2;
	r0_2 = sub_00EAE4(r0_2);
}

uint32_t sub_01818A(void)
{
	uint32_t zero = 0;
	uint16_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;

	r0 = 0xA0u;
	r1 = (uint16_t)(OBJECT(r20)->unk_30 - 0xA0u);
	r3 = (uint16_t)OBJECT(r20)->unk_28;
	r2 = (uint16_t)(0u - r1 - r3);
	if ((int16_t)(r1 - r3) > 0xA0 || (int16_t)r0 < (int16_t)r2)
	{
		OBJECT(r20)->unk_00 &= 0xDFu;
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
		mem_write8(r25 + 0x206u, 0u);
		return;
	}
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01854A_ADDRESS);
	mem_write32(r21 + 0x14u, r20);
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

void sub_01854A(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;

	r0 = 0u;
	r10 = r20 + 0x80u;
	r9 = AS32(OBJECT(r20)->unk_10);
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
		mem_write8(r25 + 0x206u, 0u);
		OBJECT(r20)->unk_00 = 0u;
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
	r0_2 = remainder_u8(mem_read8(r25 + 0xF0Eu), (uint8_t)r1);
	mem_write8(r25 + 0xF0Eu, mem_read8(r25 + 0xF0Eu) + 1u);
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

	r10 = OBJECT(r20)->unk_18;
	r11 = AS32(OBJECT(r20)->unk_1C);
	r12 = AS32(OBJECT(r20)->unk_20);
	r13 = AS32(OBJECT(r20)->unk_24);
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
	t1 = OBJECT(r20)->unk_13;
	OBJECT(r20)->unk_13 = t1 - 1u;
	r10 += 2u;
	r11 += 2u;
	if (t1 == 1u)
	{
		OBJECT(r20)->unk_04 = sub_01864A_ADDRESS;
		sub_01864A(r0);
		return;
	}
	OBJECT(r20)->unk_18 = r10;
	AS32(OBJECT(r20)->unk_1C) = r11;
	AS32(OBJECT(r20)->unk_20) = r12;
	AS32(OBJECT(r20)->unk_24) = r13;
}

void sub_01864A(uint32_t r0)
{
	bool L_0186AA = false, L_0186B2 = false, L_0186BA = false, L_0186CB = false, L_0186D1 = false, L_0186D7 = false, L_0186E0 = false, L_0186E9 = false, L_0186F2 = false;
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint8_t t1 = 0;

	if ((mem_read32(r25 + 0x94u) & 1u) != 0u)
	{
		r0_2 = (r0 & ~0xFFu) | mem_read8(r25 + 0x153u);
		if ((int8_t)r0_2 <= 5 && (r0_2 & 1u) == 0u)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
	}
	t1 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(r20)->unk_10 = OBJECT(r20)->unk_12;
	r10 = r20 + 0x54u;
	r11 = r20 + 0x34u;
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
																					r19 = r20 + 0x28u;
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
	OBJECT(r20)->unk_00 = 0u;
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
	uint8_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	r10 = r25 + 0xF10u;
	if (mem_read8(r25 + 0xF35u) == 0u)
		r0 = 0xFu;
	else
	{
		r11 = mem_read32(r25 + 0xF30u);
		if (r11 == 0u)
		{
			mem_write16(r10, rom_018AE8[0]);
			mem_write8(r25 + 0xF34u, 3u);
			r0 = 0xDu;
			r10 += 2u;
		}
		else
		{
			t1 = mem_read8(r25 + 0xF34u);
			mem_write8(r25 + 0xF34u, t1 - 1u);
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
					mem_write32(r25 + 0xF30u, 0u);
					mem_write16(r10, rom_018AE8[0]);
					mem_write8(r25 + 0xF34u, 3u);
					r0 = 0xDu;
					r10 += 2u;
				}
				else
				{
					mem_write16(r10, rom_018AE8[r0]);
					mem_write32(r25 + 0xF30u, r11);
					mem_write8(r25 + 0xF34u, 3u);
					r0 = 0xDu;
					r10 += 2u;
				}
			}
		}
	}
	r1 = mem_read32(r25 + 0xF40u);
	if (r1 != 0u)
	{
		mem_write32(r25 + 0xF44u, r1);
		mem_write32(r25 + 0xF40u, 0u);
		mem_write8(r25 + 0xF48u, 0u);
		r26 = (r26 & ~0xFFu) | 0x20u;
		move_string(r10, 0u, r10, r0 + 1u, 1u, 1u, 0u, r26, &r27, &r28);
	}
	r11 = mem_read32(r25 + 0xF44u);
	if (r11 == 0u)
		return;
	switch (mem_read8(r25 + 0xF48u))
	{
	case 0:
		r1_2 = mem_read8(r11);
		mem_write32(r25 + 0xF44u, r11 + 1u);
		if (r1_2 == 0x60u)
			r1_2 = 7u;
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, r26, &r27, &r28);
		mem_write8(r27, r1_2);
		mem_write8(r25 + 0xF49u, 5u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		t2 = mem_read8(r25 + 0xF49u);
		mem_write8(r25 + 0xF49u, t2 - 1u);
		if (t2 != 1u)
			return;
		if (mem_read8(mem_read32(r25 + 0xF44u)) != 0u)
		{
			mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) - 1u);
			return;
		}
		mem_write32(r25 + 0xF44u, r0 + 1u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, r26, &r27, &r28);
		mem_write8(r27, 0x20u);
		mem_write8(r25 + 0xF49u, 5u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		break;
	case 1:
		t2 = mem_read8(r25 + 0xF49u);
		mem_write8(r25 + 0xF49u, t2 - 1u);
		if (t2 != 1u)
			return;
		if (mem_read8(mem_read32(r25 + 0xF44u)) != 0u)
		{
			mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) - 1u);
			return;
		}
		mem_write32(r25 + 0xF44u, r0 + 1u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, r26, &r27, &r28);
		mem_write8(r27, 0x20u);
		mem_write8(r25 + 0xF49u, 5u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		break;
	case 2:
		move_string(r10 + 1u, r0, r10, r0, 1u, 0u, 0u, r26, &r27, &r28);
		mem_write8(r27, 0x20u);
		mem_write8(r25 + 0xF49u, 5u);
		mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) + 1u);
		t3 = mem_read8(r25 + 0xF49u);
		mem_write8(r25 + 0xF49u, t3 - 1u);
		if (t3 == 1u)
		{
			mem_write32(r25 + 0xF44u, mem_read32(r25 + 0xF44u) - 1u);
			mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) - 1u);
		}
		return;
	case 3:
		break;
	default:
		invalid_jump(rom_018A70[mem_read8(r25 + 0xF48u)]);
	}
	t3 = mem_read8(r25 + 0xF49u);
	mem_write8(r25 + 0xF49u, t3 - 1u);
	if (t3 != 1u)
		return;
	mem_write32(r25 + 0xF44u, mem_read32(r25 + 0xF44u) - 1u);
	mem_write8(r25 + 0xF48u, mem_read8(r25 + 0xF48u) - 1u);
}

void sub_018AF0(void)
{
	uint32_t r10 = 0;

	r10 = r25 + 0xF10u;
	mem_write32(r10, 0x20202020u);
	r10 += 4u;
	mem_write32(r10, 0x20202020u);
	r10 += 4u;
	mem_write32(r10, 0x20202020u);
	mem_write32(r10 + 4u, 0x20202020u);
	mem_write32(r25 + 0xF30u, 0u);
	mem_write16(r25 + 0xF34u, 3u);
	mem_write32(r25 + 0xF40u, 0u);
	mem_write32(r25 + 0xF44u, 0u);
}

void sub_018BA0(void)
{
	sub_018AF0();
	mem_write8(r25 + 0xF35u, 0xFFu);
	OBJECT(r20)->unk_04 = sub_018BB2_ADDRESS;
	sub_018BB2();
}

void sub_018BB2(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0;

	if (mem_read8(r23 + 0x181u) != 0u)
	{
		OBJECT(r20)->unk_0C = 0u;
		OBJECT(r20)->unk_04 = sub_018E32_ADDRESS;
		sub_018E32();
		return;
	}
	if (mem_read8(r23 + 0x182u) != 0u)
	{
		OBJECT(r20)->unk_00 = 0u;
		sub_018DBC();
		return;
	}
	if (mem_read32(r25 + 0xF44u) != 0u)
	{
		OBJECT(r20)->unk_0C = 1u;
		OBJECT(r20)->unk_04 = sub_018E98_ADDRESS;
		sub_018E98();
		return;
	}
	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r0 = mem_read16(AS32(OBJECT(r23)->unk_00) + 0x10u);
		if (r0 != 0u)
		{
			r1 = 0u;
			divux_pair(0x1Eu, &r0, &r1);
			move_string(rom_018EA0[(r1 + 2u) / 5u], 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
			mem_write8(r25 + 0xF19u, r0 + 0x31u);
			return;
		}
		AT8(OBJECT(r20)->unk_08, 1) = mem_read8(mem_read32(r25 + 0x308u) + 0x1Fu);
		OBJECT(r20)->unk_18 = mem_read32(r25 + 0x308u) + 0x20u;
		OBJECT(r20)->unk_12 = 0x96u;
		OBJECT(r20)->unk_13 = 0u;
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_0E = 0u;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		OBJECT(r20)->unk_0E = 0u;
		OBJECT(r20)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_018BD4[OBJECT(r20)->unk_0C]);
	}
	t1 = AS16(OBJECT(r20)->unk_12);
	AS16(OBJECT(r20)->unk_12) = t1 - 1u;
	if (t1 == 1u)
	{
		r0_2 = (uint8_t)(OBJECT(r20)->unk_0D + 1u);
		if ((int8_t)r0_2 >= 5)
			r0_2 = 0u;
		OBJECT(r20)->unk_0D = r0_2;
		OBJECT(r20)->unk_0E = 0u;
		OBJECT(r20)->unk_12 = 0x96u;
		OBJECT(r20)->unk_13 = 0u;
	}
	switch (OBJECT(r20)->unk_0D)
	{
	case 0:
		switch (OBJECT(r20)->unk_0E)
		{
		case 0:
			sub_018C8C();
			return;
		case 1:
			sub_018C99();
			return;
		default:
			invalid_jump(rom_018C84[OBJECT(r20)->unk_0E]);
		}
		break;
	case 1:
		break;
	case 2:
		switch (OBJECT(r20)->unk_0E)
		{
		case 0:
			move_string(0x18F3Au, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
			OBJECT(r20)->unk_0E++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D5C[OBJECT(r20)->unk_0E]);
		}
		r10 = r25 + 0xF1Cu;
		r0 = mem_read16(r25 + 0xF50u);
		sub_018DDF(r0, r10);
		return;
	case 3:
		switch (OBJECT(r20)->unk_0E)
		{
		case 0:
			move_string(0x18F48u, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
			OBJECT(r20)->unk_0E++;
			break;
		case 1:
			break;
		default:
			invalid_jump(rom_018D88[OBJECT(r20)->unk_0E]);
		}
		r10 = r25 + 0xF1Cu;
		r0 = mem_read16(r25 + 0xF52u);
		sub_018DDF(r0, r10);
		return;
	case 4:
		switch (OBJECT(r20)->unk_0E)
		{
		case 0:
			sub_018DBC();
			return;
		case 1:
			sub_018DC9();
			return;
		default:
			invalid_jump(rom_018DB4[OBJECT(r20)->unk_0E]);
		}
		break;
	default:
		invalid_jump(rom_018C64[OBJECT(r20)->unk_0D]);
	}
	switch (OBJECT(r20)->unk_0E)
	{
	case 0:
		if (mem_read8(mem_read32(r25 + 0x308u) + 0x1Fu) == 1u)
		{
			OBJECT(r20)->unk_0D++;
			switch (OBJECT(r20)->unk_0E)
			{
			case 0:
				move_string(0x18F3Au, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
				OBJECT(r20)->unk_0E++;
				break;
			case 1:
				break;
			default:
				invalid_jump(rom_018D5C[OBJECT(r20)->unk_0E]);
			}
			r10 = r25 + 0xF1Cu;
			r0 = mem_read16(r25 + 0xF50u);
			sub_018DDF(r0, r10);
			return;
		}
		move_string(0x18F2Cu, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_0E++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_018CF0[OBJECT(r20)->unk_0E]);
	}
	r10 = r25 + 0xF18u;
	r11 = OBJECT(r20)->unk_18;
	r9 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	do
	{
		r0 = mem_read8(r11);
		r11++;
		if (mem_read8(r25 + 1u) != r0 || (OBJECT(r20)->unk_10 & 8u) == 0u)
			r0_2 = (uint8_t)(unk_20E030[r0] + 0x31u);
		else
			r0_2 = 0x20u;
		mem_write8(r10, r0_2);
		r9--;
		r10++;
	} while (r9 != 0u);
	AS16(OBJECT(r20)->unk_10)++;
}

void sub_018C8C(void)
{
	move_string(0x18F1Eu, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
	OBJECT(r20)->unk_0E++;
	sub_018C99();
}

void sub_018C99(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r0 = OBJECT(r23)->unk_04 / 0xAu;
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = r25 + 0xF20u - 1u;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10 = r10 - 1u - 1u;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(6u, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	mem_write8(r10 - 1u - 1u, r0 + 0x30u);
}

void sub_018DBC(void)
{
	move_string(0x18F56u, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
	OBJECT(r20)->unk_0E++;
	sub_018DC9();
}

void sub_018DC9(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = r25 + 0xF1Cu;
	r0 = mem_read32(r25 + 0xF54u);
	r1 = mem_read16(r25 + 0xF58u);
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
		mem_write8(r10, r1 + r2);
		r10--;
		mem_write8(r10, 0x20u);
		mem_write8(r10 - 1u, 0x20u);
		return;
	}
	r10--;
	mem_write8(r10, r1 + r2);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero)
	{
		r10--;
		mem_write8(r10, r1 + r2);
		mem_write8(r10 - 1u, 0x20u);
		return;
	}
	r10--;
	mem_write8(r10, r1 + r2);
	if (r0 == 0u)
	{
		mem_write8(r10 - 1u, 0x20u);
		return;
	}
	mem_write8(r10 - 1u, r0 + r2);
}

void sub_018E32(void)
{
	uint16_t t1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		move_string(0x18F64u, 7u, r25 + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
		OBJECT(r20)->unk_0C++;
		if (mem_read32(r25 + 0xF44u) == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		if (mem_read32(r25 + 0xF44u) == 0u)
			return;
		OBJECT(r20)->unk_0C++;
		if (mem_read32(r25 + 0xF44u) != 0u)
			return;
		sub_018DBC();
		OBJECT(r20)->unk_10 = 0x96u;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_0C++;
		t1 = AS16(OBJECT(r20)->unk_10);
		AS16(OBJECT(r20)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(r20)->unk_00 = 0u;
		sub_018C8C();
		return;
	case 2:
		break;
	case 3:
		t1 = AS16(OBJECT(r20)->unk_10);
		AS16(OBJECT(r20)->unk_10) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(r20)->unk_00 = 0u;
		sub_018C8C();
		return;
	default:
		invalid_jump(rom_018E40[OBJECT(r20)->unk_0C]);
	}
	if (mem_read32(r25 + 0xF44u) != 0u)
		return;
	sub_018DBC();
	OBJECT(r20)->unk_10 = 0x96u;
	OBJECT(r20)->unk_11 = 0u;
	OBJECT(r20)->unk_0C++;
	t1 = AS16(OBJECT(r20)->unk_10);
	AS16(OBJECT(r20)->unk_10) = t1 - 1u;
	if (t1 != 1u)
		return;
	OBJECT(r20)->unk_00 = 0u;
	sub_018C8C();
}

void sub_018E98(void)
{
	if (mem_read32(r25 + 0xF44u) == 0u)
	{
		OBJECT(r20)->unk_04 = sub_018BB2_ADDRESS;
		sub_018BB2();
		return;
	}
}

void sub_018F74(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, object = 0;

	object = r20 + 0x80u;
	OBJECT(object - 0x80u)->unk_80 = 0xFFFFFFFFu;
	OBJECT(object - 0x80u)->unk_84 = 0xFFFFFFFFu;
	r10 = mem_read32(r25 + 0x308u) + 0x1Fu;
	r9 = mem_read8(r10);
	AS8(OBJECT(r20)->unk_88) = r9;
	r10++;
	do
	{
		r0 = mem_read8(r10);
		mem_write8(r20 + 0x80u + unk_20E030[r0], mem_read8(0x20FC49u + (r0 << 7)));
		r9--;
		r10++;
	} while (r9 != 0u);
	AT8(OBJECT(r20)->unk_08, 1) = unk_20E030[mem_read8(r25 + 1u)];
	mem_write8(r25 + 0xF0Fu, 0u);
	AS8(OBJECT(r23)->unk_14) = rom_019421[OBJECT(r23)->unk_60];
	AT8(OBJECT(r20)->unk_08, 2) = mem_read8(r25 + 0xA1u);
	OBJECT(r20)->unk_04 = sub_018FD3_ADDRESS;
	sub_018FD3();
}

void sub_018FD3(void)
{
	if (mem_read16(AS32(OBJECT(r23)->unk_00) + 0x10u) != 0u)
		return;
	OBJECT(r20)->unk_04 = sub_018FE0_ADDRESS;
	sub_018FE0();
}

void sub_018FE0(void)
{
	OBJECT(r20)->unk_2C = mem_read16(r25 + 0x118u);
	if ((mem_read8(r23 + 0x181u) | mem_read8(r23 + 0x182u)) == 0u)
	{
		sub_01900A();
		return;
	}
	OBJECT(r20)->unk_2C = 0u;
	OBJECT(r20)->unk_0D = 5u;
	mem_write16(r25 + 0xF20u, 0u);
	OBJECT(r20)->unk_04 = sub_01900A_ADDRESS;
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

	r21 = OBJECT(r20)->unk_18;
	if (r21 != 0u && (mem_read32(r21) & 1u) == 0u)
		OBJECT(r20)->unk_18 = 0u;
	if (mem_read8(r25 + 0xF0Fu) != 0u)
	{
		r0 = mem_read8(r25 + 0xA1u);
		if ((uint8_t)(OBJECT(r20)->unk_08 >> 16u) != r0)
		{
			AT8(OBJECT(r20)->unk_08, 2) = r0;
			mem_write8(r25 + 0xF0Fu, 0u);
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
							target1 = L_019118 ? 0x190F0u : L_019132 ? 0x190F0u : L_01914C ? 0x190F0u : L_019154 ? 0x190F0u : L_019167 ? 0x190F0u : L_019180 ? 0x190F0u : rom_019044[OBJECT(r20)->unk_0C];
							switch (target1)
							{
							case 0x019068u:
								r0_2 = (uint8_t)OBJECT(r23)->unk_14;
								r0_2 = sub_0192E5(r0_2);
								sub_011EB8((uint8_t)r0_2);
								OBJECT(r20)->unk_0C++;
							case 0x019078u:
								r0_3 = OBJECT(r20)->unk_2C;
								if ((r0_3 & 0x100u) != 0u)
								{
									OBJECT(r20)->unk_0C = 2u;
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
									r0_2 = (uint8_t)OBJECT(r23)->unk_14;
									t1 = r0_2 - 1u;
									r0_2 = t1;
									if ((int8_t)t1 < 0)
										r0_2 = 9u;
									AS8(OBJECT(r23)->unk_14) = r0_2;
									r0_2 = sub_0192E5(r0_2);
									sub_011EB8((uint8_t)r0_2);
									return;
								}
								if ((r0_3 & 0x400u) == 0u && (r0_3 & 0x40u) == 0u)
									return;
								r0 = 0x1Fu;
								sub_011EB8(r0);
								r0_2 = (uint8_t)OBJECT(r23)->unk_14;
								r0_2 = (uint8_t)(r0_2 + 1u);
								if ((int8_t)r0_2 < 0xA)
								{
									AS8(OBJECT(r23)->unk_14) = r0_2;
									r0_2 = sub_0192E5(r0_2);
									sub_011EB8((uint8_t)r0_2);
									return;
								}
								r0_2 = 0u;
								AS8(OBJECT(r23)->unk_14) = r0_2;
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
														AT8(OBJECT(r20)->unk_08, 3) = 0x29u;
													}
												}
											}
										}
									}
								}
								do
								{
									if (L_019137 || (!L_019118 && !L_019132 && (L_01914C || L_019154 || L_019167 || L_019180 || mem_read8(r25 + 0xF0Fu) != 0u)))
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
														OBJECT(r20)->unk_10 = 0xB4u;
														OBJECT(r20)->unk_0C = 5u;
													}
													L_01914C = false;
													t2 = OBJECT(r20)->unk_10;
													OBJECT(r20)->unk_10 = t2 - 1u;
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
														OBJECT(r20)->unk_0C++;
													L_019154 = false;
												}
											}
											if (!L_019167 && !L_019180 && mem_read8(r25 + 0xF0Fu) == 0u)
											{
												AT8(OBJECT(r20)->unk_08, 3) = rom_019571[mem_read8(r25 + 0xA4u)];
												r0 = rom_019458[mem_read8(r25 + 0xA1u)];
												sub_011EB8(r0);
												OBJECT(r20)->unk_0C = 3u;
											}
											else
											{
												if (L_019167 || !L_019180)
												{
													if (!L_019167)
													{
														r0 = 0xEEu;
														sub_011EB8(r0);
														OBJECT(r20)->unk_0C++;
													}
													L_019167 = false;
												}
												if (L_019180 || mem_read8(r25 + 0xF0Fu) == 0u)
												{
													if (!L_019180)
													{
														r0 = 0x7Fu;
														sub_011FEA(r0);
														OBJECT(r20)->unk_10 = 0x78u;
														OBJECT(r20)->unk_0C++;
													}
													L_019180 = false;
													t3 = OBJECT(r20)->unk_10;
													OBJECT(r20)->unk_10 = t3 - 1u;
													if (t3 == 1u)
													{
														AT8(OBJECT(r20)->unk_08, 3) = rom_019571[mem_read8(r25 + 0xA4u)];
														r0 = rom_019458[mem_read8(r25 + 0xA1u)];
														sub_011EB8(r0);
														OBJECT(r20)->unk_0C = 3u;
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
												r0 = rom_019458[mem_read8(r25 + 0xA1u)];
												sub_011EB8(r0);
												OBJECT(r20)->unk_0C++;
											}
											L_019118 = false;
										}
										if (L_019132 || mem_read8(r25 + 0xF0Fu) != 0u)
										{
											if (!L_019132)
											{
												r0 = 0x3Fu;
												sub_011FEA(r0);
												OBJECT(r20)->unk_10 = 0x3Cu;
												OBJECT(r20)->unk_0C++;
											}
											L_019132 = false;
											t4 = OBJECT(r20)->unk_10;
											OBJECT(r20)->unk_10 = t4 - 1u;
											L_019137 = t4 == 1u;
										}
									}
								} while (L_019137);
								if ((OBJECT(r20)->unk_2C & 0x700u) != 0u)
								{
									OBJECT(r20)->unk_0C = 0u;
									OBJECT(r20)->unk_0D = 4u;
									mem_write32(r25 + 0xF30u, 0u);
									r0 = 0x1Fu;
									sub_011EB8(r0);
									return;
								}
								if (mem_read32(r25 + 0xF30u) != 0u)
								{
									AT8(OBJECT(r20)->unk_08, 3) = 0u;
									return;
								}
								switch (OBJECT(r20)->unk_0D)
								{
								case 0:
									r0 = 0u;
									sub_019402(r0);
									OBJECT(r20)->unk_0D++;
									return;
								case 1:
									r0_2 = (uint8_t)(OBJECT(r20)->unk_88 >> 8u);
									while ((int8_t)mem_read8(r20 + 0x80u + r0_2) < 0)
									{
										r0_2 = (uint8_t)(r0_2 + 1u);
									}
									AT8(OBJECT(r20)->unk_88, 1) = r0_2;
									r0_2 = (uint8_t)(r0_2 + 9u);
									sub_019402((uint8_t)r0_2);
									OBJECT(r20)->unk_0D++;
									return;
								case 2:
									r0 = (uint8_t)(mem_read8(r20 + 0x80u + (uint8_t)(OBJECT(r20)->unk_88 >> 8u)) + 1u);
									sub_019402(r0);
									AT8(OBJECT(r20)->unk_88, 1)++;
									t5 = (uint8_t)OBJECT(r20)->unk_88;
									AS8(OBJECT(r20)->unk_88) = t5 - 1u;
									if (t5 == 1u)
									{
										OBJECT(r20)->unk_0D++;
										return;
									}
									OBJECT(r20)->unk_0D--;
									return;
								case 3:
									r0 = 0x2Au;
									sub_019402(r0);
									OBJECT(r20)->unk_0D++;
									return;
								case 4:
									r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 24u);
									if (r0 != 0u)
									{
										OBJECT(r20)->unk_38 = 0u;
										OBJECT(r20)->unk_3A = 0u;
										sub_019402(r0);
										return;
									}
									if (OBJECT(r20)->unk_0C != 3u)
										return;
									r10 = AS32(OBJECT(r20)->unk_38);
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
									AS32(OBJECT(r20)->unk_38) = r10;
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
									invalid_jump(rom_0191E0[OBJECT(r20)->unk_0D]);
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

	r0 = (uint8_t)(unk_20E030[mem_read8(mem_read32(r25 + 0x308u) + 0x20u)] + 0x11u);
	sub_019402(r0);
}

void sub_0192B1(void)
{
	uint8_t r0 = 0;

	r0 = (uint8_t)((uint8_t)(OBJECT(r20)->unk_08 >> 8u) + 0x19u);
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

	r21 = OBJECT(r20)->unk_18;
	if (r21 != 0u)
		mem_write16(r21 + 0x10u, 0u);
	r21 = sub_01930A(r10);
	OBJECT(r20)->unk_18 = r21;
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
		target1 = L_01938A ? 0x19369u : rom_019330[OBJECT(r20)->unk_0C];
		switch (target1)
		{
		case 0x019344u:
			r10 = OBJECT(r20)->unk_18;
			OBJECT(r20)->unk_30 = mem_read16(r10);
			r10 += 2u;
			OBJECT(r20)->unk_18 = r10;
			mem_write32(r25 + 0xF40u, r10);
			OBJECT(r20)->unk_20 = 0xD2u;
			OBJECT(r20)->unk_22 = 0xC8u;
			OBJECT(r20)->unk_23 = 0u;
			OBJECT(r20)->unk_12 = 0x14u;
			OBJECT(r20)->unk_13 = 0u;
			OBJECT(r20)->unk_0C++;
		case 0x019369u:
			do
			{
				if (L_019387 || L_01938A || (OBJECT(r20)->unk_10 | OBJECT(r20)->unk_11) == 0u)
				{
					if (L_019387 || !L_01938A)
					{
						L_019387 = false;
						OBJECT(r20)->unk_0C++;
					}
					L_01938A = false;
					t1 = AS16(OBJECT(r20)->unk_10);
					t2 = t1 - 1u;
					negative = t2 >> 15;
					AS16(OBJECT(r20)->unk_10) = t2;
					if ((int16_t)t1 > 1)
					{
						r3 = AS32(OBJECT(r20)->unk_20);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(r20)->unk_30);
						r5 = OBJECT(r20)->unk_0D;
						r5 = (uint8_t)(r5 + 1u);
						OBJECT(r20)->unk_0D = r5;
						r5 = r5 >> 1;
						r5_2 = rom_019561[r5 & 7u];
						r18 = OBJECT(r20)->unk_18;
						sub_0119FA(r3, r5_2, r18);
						return;
					}
					if (negative)
					{
						OBJECT(r20)->unk_0C += 2u;
						r3 = AS32(OBJECT(r20)->unk_20);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
						AS32(OBJECT(r20)->unk_20) = r3;
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(r20)->unk_00 = 0u;
							return;
						}
					}
					else
					{
						OBJECT(r20)->unk_0C++;
						r3 = AS32(OBJECT(r20)->unk_20);
						r0 = (uint16_t)(AS16(OBJECT(r20)->unk_12) + 1u);
						r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
						AS16(OBJECT(r20)->unk_12) = r0;
						AS32(OBJECT(r20)->unk_20) = r3;
						if ((int16_t)r3 <= -0xD2)
						{
							OBJECT(r20)->unk_00 = 0u;
							return;
						}
					}
				}
				else
				{
					r3 = AS32(OBJECT(r20)->unk_20);
					r0 = AS16(OBJECT(r20)->unk_12);
					r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
					AS16(OBJECT(r20)->unk_12) = r0 - 1u;
					AS32(OBJECT(r20)->unk_20) = r3;
					L_019387 = (uint16_t)r3 == 0u;
				}
			} while (L_019387);
			break;
		case 0x01938Au:
			L_01938A = true;
			break;
		case 0x019394u:
			r3 = AS32(OBJECT(r20)->unk_20);
			r0 = (uint16_t)(AS16(OBJECT(r20)->unk_12) + 1u);
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - r0);
			AS16(OBJECT(r20)->unk_12) = r0;
			AS32(OBJECT(r20)->unk_20) = r3;
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(r20)->unk_00 = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(r20)->unk_30);
			r5 = OBJECT(r20)->unk_0D;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(r20)->unk_0D = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			r18 = OBJECT(r20)->unk_18;
			sub_0119FA(r3, r5_2, r18);
			return;
		case 0x0193BAu:
			r3 = AS32(OBJECT(r20)->unk_20);
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 - 0x28u);
			AS32(OBJECT(r20)->unk_20) = r3;
			if ((int16_t)r3 <= -0xD2)
			{
				OBJECT(r20)->unk_00 = 0u;
				return;
			}
			r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(r20)->unk_30);
			r5 = OBJECT(r20)->unk_0D;
			r5 = (uint8_t)(r5 + 1u);
			OBJECT(r20)->unk_0D = r5;
			r5 = r5 >> 1;
			r5_2 = rom_019561[r5 & 7u];
			r18 = OBJECT(r20)->unk_18;
			sub_0119FA(r3, r5_2, r18);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_01938A);
	r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + OBJECT(r20)->unk_30);
	r5 = OBJECT(r20)->unk_0D;
	r5 = (uint8_t)(r5 + 1u);
	OBJECT(r20)->unk_0D = r5;
	r5 = r5 >> 1;
	r5_2 = rom_019561[r5 & 7u];
	r18 = OBJECT(r20)->unk_18;
	sub_0119FA(r3, r5_2, r18);
}

void sub_0193FC(uint8_t r0)
{
	if (mem_read32(r25 + 0xF30u) != 0u)
		return;
	sub_019402(r0);
}

void sub_019402(uint8_t r0)
{
	uint8_t r0_2 = 0;
	uint32_t r10 = 0;

	r10 = rom_019576[(uint8_t)r0];
	r0_2 = (uint8_t)byte_swap32(r10);
	mem_write32(r25 + 0xF30u, r10 & 0xFFFFFFu);
	sub_011EB8(r0_2);
}

void sub_019CAC(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0, r19 = 0, r21 = 0;
	uint8_t t4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t5 = 0, t6 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		mem_write8(r25 + 0x5800u, 0u);
		mem_write8(r25 + 0x5801u, 0u);
		r19 = 0x1ACCCu + ((uint32_t)mem_read8(r25) << 3) * 4u;
		OBJECT(r20)->unk_84 = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_88 = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_8C = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_90 = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_94 = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_98 = ROM32(r19);
		r19 += 4u;
		OBJECT(r20)->unk_9C = ROM32(r19);
		mem_write32(r20 + 0xA0u, ROM32(r19 + 4u));
		r19 = 0x1AD0Cu + mem_read8(r25) * 5u * 4u;
		sub_0179AC(r19);
		mem_write16(mem_read32(r20 + 0xA0u), 0xFFF0u);
		mem_write16(OBJECT(r20)->unk_8C, 0x360u);
		t1 = OBJECT(r20)->unk_8C;
		mem_write16(t1, mem_read16(t1) + ((mem_read8(r25 + 2u) * 5u) << 6));
		mem_write16(OBJECT(r20)->unk_90, 0x72u);
		r0 = mem_read8(r25 + 0x9Eu);
		mem_write8(r25 + 0x9Cu, r0);
		mem_write8(r25 + 0x9Eu, (r0 + 1u) & 7u);
		mem_write8(r25 + 0x41u, mem_read8(r25 + 0x9Cu));
		mem_write8(r25 + 0x40u, 5u);
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
		OBJECT(r20)->unk_10 = 0x2Du;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_0C++;
		return;
	case 1:
		sub_01A0A2();
		t2 = OBJECT(r20)->unk_8C;
		mem_write16(t2, mem_read16(t2) + 8u);
		t3 = OBJECT(r20)->unk_9C;
		mem_write16(t3, mem_read16(t3) + 0x14u);
		t4 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t4 - 1u;
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
		OBJECT(r20)->unk_80 = 0x1ACC0u;
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_0C++;
		return;
	case 2:
		sub_01A0A2();
		t5 = OBJECT(r20)->unk_8C;
		mem_write16(t5, mem_read16(t5) + 8u);
		t6 = OBJECT(r20)->unk_9C;
		mem_write16(t6, mem_read16(t6) + 0x14u);
		AS16(OBJECT(r20)->unk_10)++;
		if (AS16(OBJECT(r20)->unk_10) != mem_read16(OBJECT(r20)->unk_80))
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
		OBJECT(r20)->unk_80 += 2u;
		return;
	default:
		invalid_jump(rom_019CB8[OBJECT(r20)->unk_0C]);
	}
}

void sub_019E6B(void)
{
	uint16_t r2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_019E78[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r10 = 0x1ABB4u + ((uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 4);
		r0 = mem_read16(r10);
		r9 = rom_01AC34[r0];
		r10 += 2u;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_34 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_36 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_20 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_32 = mem_read16(r10);
		r10 += 2u;
		OBJECT(r20)->unk_2C = mem_read16(r10);
		OBJECT(r20)->unk_40 = 9u;
		OBJECT(r20)->unk_3C = 0x18Fu;
		OBJECT(r20)->unk_3C += mem_read16(r10 + 2u);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_019F60_ADDRESS);
		mem_write32(r21 + 0x14u, r20);
		mem_write8(r21 + 9u, r0);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_019F4C_ADDRESS);
		mem_write32(r21 + 0x14u, r20);
		mem_write8(r21 + 9u, r0);
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_019E78[t1]);
	}
	OBJECT(r20)->unk_20 += OBJECT(r20)->unk_2C;
	if ((int16_t)OBJECT(r20)->unk_20 >= 0x1000)
	{
		OBJECT(r20)->unk_00 &= 0xDFu;
		OBJECT(r20)->unk_20 &= 0xFFFu;
	}
	OBJECT(r20)->unk_30 = OBJECT(r20)->unk_20;
	OBJECT(r20)->unk_30 -= rom_01ABA4[mem_read8(r25 + 2u)];
	AS16(OBJECT(r20)->unk_10)++;
	r2 = AS16(OBJECT(r20)->unk_10) & 1u;
	OBJECT(r20)->unk_24 = OBJECT(r20)->unk_32;
	OBJECT(r20)->unk_32 -= r2;
	sub_00EAE4(r0);
	OBJECT(r20)->unk_32 = OBJECT(r20)->unk_24;
}

void sub_019F4C(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
		OBJECT(r20)->unk_3A = rom_01A032[r0];
		AS16(OBJECT(r20)->unk_10) = rom_01A062[r0];
		AS16(OBJECT(r20)->unk_12) = rom_01A072[r0];
		r9 = rom_01A012[r0];
		sub_00F366(r9);
		OBJECT(r20)->unk_0C++;
		OBJECT(r20)->unk_3E = 0xAu;
		OBJECT(r20)->unk_3C = mem_read16(OBJECT(r20)->unk_14 + 0x3Cu);
		OBJECT(r20)->unk_3C++;
		return;
	case 1:
		sub_019FD3();
		return;
	default:
		invalid_jump(rom_019F58[OBJECT(r20)->unk_0C]);
	}
}

void sub_019F60(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		break;
	case 1:
		sub_019FD3();
		return;
	default:
		invalid_jump(rom_019F6C[OBJECT(r20)->unk_0C]);
	}
	r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	OBJECT(r20)->unk_3A = rom_01A042[r0];
	AS16(OBJECT(r20)->unk_10) = rom_01A082[r0];
	AS16(OBJECT(r20)->unk_12) = rom_01A092[r0];
	r9 = rom_01A022[r0];
	sub_00F366(r9);
	OBJECT(r20)->unk_0C++;
	OBJECT(r20)->unk_3E = 0xAu;
	OBJECT(r20)->unk_3C = mem_read16(OBJECT(r20)->unk_14 + 0x3Cu);
	OBJECT(r20)->unk_3C++;
}
