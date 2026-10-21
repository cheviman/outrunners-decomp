/**
 *    Name:        game_04.c
 *    Description: the game's functions, part 04 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_019FD3(void)
{
	uint32_t r0 = 0;

	r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	OBJECT(r20)->unk_34 = rom_01A052[r0];
	OBJECT(r20)->unk_36 = rom_01A052[r0];
	OBJECT(r20)->unk_30 = mem_read16(OBJECT(r20)->unk_14 + 0x30u) + AS16(OBJECT(r20)->unk_10);
	r0 = (uint16_t)(mem_read16(OBJECT(r20)->unk_14 + 0x32u) + AS16(OBJECT(r20)->unk_12) - (mem_read16(OBJECT(r20)->unk_14 + 0x10u) & 1u));
	OBJECT(r20)->unk_32 = r0;
	r0 = sub_00EAE4(r0);
}

void sub_01A0A2(void)
{
	uint32_t r21 = 0;

	OBJECT(r20)->unk_12++;
	if ((OBJECT(r20)->unk_12 & 7u) == 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A0E2_ADDRESS);
		mem_write8(r21 + 9u, 0u);
		return;
	}
	if ((OBJECT(r20)->unk_12 & 1u) == 0u)
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A0F0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r9 = 0x59u;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = 0x6Du;
		if ((uint8_t)(OBJECT(r20)->unk_08 >> 8u) != 0u)
		{
			OBJECT(r20)->unk_34 = 0x60u;
			OBJECT(r20)->unk_36 = 0x60u;
			OBJECT(r20)->unk_30 = 0x170u;
			OBJECT(r20)->unk_32 = 0x96u;
			OBJECT(r20)->unk_3C = 0x185u;
		}
		else
		{
			OBJECT(r20)->unk_34 = 0x100u;
			OBJECT(r20)->unk_36 = 0x100u;
			OBJECT(r20)->unk_30 = 0x170u;
			OBJECT(r20)->unk_32 = 0xF0u;
			OBJECT(r20)->unk_3C = 0x194u;
		}
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01A0F0[t1]);
	}
	if ((int16_t)OBJECT(r20)->unk_30 <= -0x30)
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	if ((uint8_t)(OBJECT(r20)->unk_08 >> 8u) != 0u)
	{
		OBJECT(r20)->unk_30 -= 0x12u;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_30 -= 0x30u;
	r0 = sub_00EAE4(r0);
}

void sub_01A18C(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A198[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r9 = 0x25Du;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = 0x26Au;
		OBJECT(r20)->unk_30 = 0x142u;
		OBJECT(r20)->unk_32 = 0x47u;
		OBJECT(r20)->unk_3C = 0x199u;
		OBJECT(r20)->unk_10 = 0xBu;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_0C++;
			OBJECT(r20)->unk_3A = 0x269u;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		r10 = OBJECT(r20)->unk_80;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		OBJECT(r20)->unk_80 = r10 + 2u;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		if (mem_read8(r25 + 0x5801u) != 0u)
		{
			OBJECT(r20)->unk_40 = 1u;
			OBJECT(r20)->unk_41 = 2u;
			AS8(OBJECT(r20)->unk_42) = 2u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A198[t1]);
	}
	t3 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t3 - 1u;
	if (t3 != 1u)
	{
		OBJECT(r20)->unk_30 -= 0x10u;
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
	OBJECT(r20)->unk_80 = 0x1AC54u;
	OBJECT(r20)->unk_10 = 6u;
	OBJECT(r20)->unk_0C++;
	r0 = sub_00EAE4(r0);
}

void sub_01A29C(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A2A8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r9 = 0x25Eu;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = 0x26Du;
		OBJECT(r20)->unk_30 = 2u;
		OBJECT(r20)->unk_32 = 0x47u;
		OBJECT(r20)->unk_3C = 0x199u;
		OBJECT(r20)->unk_10 = 0xBu;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		r10 = OBJECT(r20)->unk_80;
		OBJECT(r20)->unk_3A = mem_read16(r10);
		OBJECT(r20)->unk_80 = r10 + 2u;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A2A8[t1]);
	}
	t3 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t3 - 1u;
	if (t3 == 1u)
	{
		OBJECT(r20)->unk_80 = 0x1AC62u;
		OBJECT(r20)->unk_10 = 6u;
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_30 += 0x10u;
	r0 = sub_00EAE4(r0);
}

void sub_01A329(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A334[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_10 = ((uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2) + 1u;
		OBJECT(r20)->unk_0C++;
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(r20)->unk_3E = 0xAu;
		r0 = sub_011E45();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = rom_01AC44[r0];
		r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(r20)->unk_30 = ROM16(r10);
		OBJECT(r20)->unk_32 = ROM16(r10 + 2u);
		OBJECT(r20)->unk_34 = 0x40u;
		OBJECT(r20)->unk_36 = 0x40u;
		OBJECT(r20)->unk_40 = 9u;
		OBJECT(r20)->unk_3C = 0x19Au;
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(r20)->unk_3E = 0xAu;
		r0 = sub_011E45();
		r0 &= 7u;
		r9 = rom_01AC34[r0];
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = rom_01AC44[r0];
		r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2;
		r10 = 0x1ACB4u + r0;
		OBJECT(r20)->unk_30 = ROM16(r10);
		OBJECT(r20)->unk_32 = ROM16(r10 + 2u);
		OBJECT(r20)->unk_34 = 0x40u;
		OBJECT(r20)->unk_36 = 0x40u;
		OBJECT(r20)->unk_40 = 9u;
		OBJECT(r20)->unk_3C = 0x19Au;
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(r20)->unk_30 += 0x10u;
		if ((int16_t)OBJECT(r20)->unk_30 <= 0x148)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((uint8_t)(OBJECT(r20)->unk_08 >> 8u) == 0u)
			mem_write8(r25 + 0x5801u, 1u);
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		mem_write8(r25 + 0x5801u, 0u);
		OBJECT(r20)->unk_00 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A3F4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_10 = ((uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2) + 1u;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_3E = 0xAu;
			OBJECT(r20)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_30 += 0x10u;
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		if (mem_read8(r25 + 0x5801u) != 0u)
		{
			OBJECT(r20)->unk_40 = 0u;
			OBJECT(r20)->unk_41 = 2u;
			AS8(OBJECT(r20)->unk_42) = 2u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A3F4[t1]);
	}
	t3 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t3 - 1u;
	if (t3 != 1u)
		return;
	OBJECT(r20)->unk_3E = 0xAu;
	r9 = 0x25Fu;
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = 0x270u;
	r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2;
	r10 = 0x1ACA8u + r0;
	OBJECT(r20)->unk_30 = ROM16(r10);
	OBJECT(r20)->unk_32 = ROM16(r10 + 2u);
	OBJECT(r20)->unk_3C = 0x198u;
	OBJECT(r20)->unk_10 = 0x14u;
	OBJECT(r20)->unk_0C++;
	t2 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(r20)->unk_3E = 0xAu;
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_30 += 0x10u;
	r0 = sub_00EAE4(r0);
}

void sub_01A499(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A4A4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r9 = 0x25Au;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = 0x272u;
		OBJECT(r20)->unk_34 = 0xC0u;
		OBJECT(r20)->unk_36 = 0xC0u;
		OBJECT(r20)->unk_30 = 0xE2u;
		OBJECT(r20)->unk_32 = 0x4Eu;
		OBJECT(r20)->unk_3C = 0x198u;
		OBJECT(r20)->unk_10 = 9u;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		if (mem_read8(r25 + 0x5801u) != 0u)
		{
			OBJECT(r20)->unk_40 = 0u;
			OBJECT(r20)->unk_41 = 3u;
			AS8(OBJECT(r20)->unk_42) = 6u;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A4A4[t1]);
	}
	t2 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(r20)->unk_3E = 0xAu;
		OBJECT(r20)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_34 += 8u;
	OBJECT(r20)->unk_36 += 8u;
	r0 = sub_00EAE4(r0);
}

void sub_01A536(void)
{
	uint16_t r3 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A544[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_3E = 0xAu;
		r9 = 0x25Bu;
		sub_00F366(r9);
		OBJECT(r20)->unk_3A = 0x272u;
		OBJECT(r20)->unk_34 = 0xC0u;
		OBJECT(r20)->unk_36 = 0xC0u;
		r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 2;
		r10 = 0x1ACA0u + r0;
		OBJECT(r20)->unk_30 = ROM16(r10);
		OBJECT(r20)->unk_32 = ROM16(r10 + 2u);
		OBJECT(r20)->unk_3C = 0x198u;
		OBJECT(r20)->unk_10 = 9u;
		OBJECT(r20)->unk_0C++;
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_3E = 0xAu;
			r0 = (uint16_t)((uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 11);
			AS16(OBJECT(r20)->unk_88) = r0;
			AS16(OBJECT(r20)->unk_98) = r0;
			AS16(OBJECT(r20)->unk_8C) = OBJECT(r20)->unk_30;
			AS16(OBJECT(r20)->unk_90) = OBJECT(r20)->unk_32;
			OBJECT(r20)->unk_0C++;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_34 += 8u;
		OBJECT(r20)->unk_36 += 8u;
		OBJECT(r20)->unk_30--;
		OBJECT(r20)->unk_32 -= 5u;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_3E = 0xAu;
			r0 = (uint16_t)((uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 11);
			AS16(OBJECT(r20)->unk_88) = r0;
			AS16(OBJECT(r20)->unk_98) = r0;
			AS16(OBJECT(r20)->unk_8C) = OBJECT(r20)->unk_30;
			AS16(OBJECT(r20)->unk_90) = OBJECT(r20)->unk_32;
			OBJECT(r20)->unk_0C++;
			sub_00F30D(0u);
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_34 += 8u;
		OBJECT(r20)->unk_36 += 8u;
		OBJECT(r20)->unk_30--;
		OBJECT(r20)->unk_32 -= 5u;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		AS16(OBJECT(r20)->unk_88) += 0x20u;
		if ((int16_t)(uint16_t)OBJECT(r20)->unk_88 >= 0x1000)
			AS16(OBJECT(r20)->unk_88) -= 0x1000u;
		AS16(OBJECT(r20)->unk_98) += 0x40u;
		if ((int16_t)(uint16_t)OBJECT(r20)->unk_98 >= 0x1000)
			AS16(OBJECT(r20)->unk_98) -= 0x1000u;
		OBJECT(r20)->unk_30 = (uint16_t)OBJECT(r20)->unk_8C;
		OBJECT(r20)->unk_32 = (uint16_t)OBJECT(r20)->unk_90;
		r0 = (int16_t)(uint16_t)OBJECT(r20)->unk_88 & 0xFFFu;
		r3 = (uint16_t)((int16_t)rom_100C00[(uint32_t)(int16_t)(uint16_t)OBJECT(r20)->unk_98] >> 12);
		OBJECT(r20)->unk_30 += (int16_t)rom_100C00[r0] >> 9;
		OBJECT(r20)->unk_32 += r3;
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

	t1 = OBJECT(r20)->unk_0C;
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
	OBJECT(r20)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 4;
	r10 = 0x1AC70u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_32 = mem_read16(r10);
	OBJECT(r20)->unk_3C = mem_read16(r10 + 2u);
	OBJECT(r20)->unk_0C++;
	r0 = sub_00EAE4(r0);
}

void sub_01A6CF(void)
{
	uint8_t r0 = 0, r2 = 0, r10 = 0, r11 = 0;
	uint32_t r1 = 0, r21 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		if (mem_read8(r25 + 0x80u) == 6u)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
			return;
		}
		AT8(OBJECT(r20)->unk_08, 2) = mem_read8(r25 + 2u);
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		t1 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t1 - 1u;
		if (t1 != 1u)
		{
			if (mem_read8(r25 + 0x80u) != 6u && mem_read8(r25 + 0x80u) != 7u && mem_read32(r25 + 0xF44u) == 0u)
				mem_write32(r25 + 0xF40u, rom_01A9BC[(uint8_t)(OBJECT(r20)->unk_08 >> 8u)]);
			return;
		}
		break;
	default:
		invalid_jump(rom_01A6DC[OBJECT(r20)->unk_0C]);
	}
	OBJECT(r20)->unk_10 = 0x1Eu;
	AT8(OBJECT(r20)->unk_08, 3) = (uint8_t)(OBJECT(r20)->unk_08 >> 16u);
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_01A7C5_ADDRESS);
	mem_write8(r21 + 0xAu, (uint8_t)(OBJECT(r20)->unk_08 >> 16u));
	mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(r20)->unk_08 >> 16u));
	AT8(OBJECT(r20)->unk_08, 2)++;
	OBJECT(r20)->unk_08 &= 0xFF07FFFFu;
	if (unk_20E615 != 0u)
	{
		r0 = 2u;
		r1 = 2u;
		r2 = 0u;
		r21 = sub_01A8CA(r1, r2);
	}
	else
	{
		switch (mem_read8(r25))
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
			invalid_jump(rom_01A744[mem_read8(r25)]);
		}
		if (r10 != 0u)
		{
			r0 = 2u;
			r1 = 2u;
			r2 = 0u;
			r21 = sub_01A8CA(r1, r2);
			AT8(OBJECT(r20)->unk_08, 1) = r0;
			mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(r20)->unk_08 >> 24u));
			if (mem_read8(r25 + 0x80u) != 6u && mem_read8(r25 + 0x80u) != 7u && mem_read32(r25 + 0xF44u) == 0u)
				mem_write32(r25 + 0xF40u, rom_01A9BC[(uint8_t)(OBJECT(r20)->unk_08 >> 8u)]);
			return;
		}
		if (r11 != 0u)
			r0 = 1u;
		else
			r0 = 0u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_01A834_ADDRESS);
		mem_write32(r21 + 0x14u, r20);
		mem_write8(r21 + 9u, r0);
	}
	AT8(OBJECT(r20)->unk_08, 1) = r0;
	mem_write8(r21 + 0xBu, (uint8_t)(OBJECT(r20)->unk_08 >> 24u));
	if (mem_read8(r25 + 0x80u) != 6u && mem_read8(r25 + 0x80u) != 7u && mem_read32(r25 + 0xF44u) == 0u)
		mem_write32(r25 + 0xF40u, rom_01A9BC[(uint8_t)(OBJECT(r20)->unk_08 >> 8u)]);
}

void sub_01A7C5(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A7D0[t1];
	switch (t1)
	{
	case 0:
		r9 = rom_01A986[(uint8_t)(OBJECT(r20)->unk_08 >> 16u)];
		sub_00F366(r9);
		r0 = (uint16_t)(rom_01A996[(uint8_t)(OBJECT(r20)->unk_08 >> 24u)] + 4u);
		OBJECT(r20)->unk_3A = r0;
		OBJECT(r20)->unk_3E = 0xAu;
		OBJECT(r20)->unk_34 = 0x40u;
		OBJECT(r20)->unk_36 = 0x40u;
		OBJECT(r20)->unk_30 = 0x140u;
		OBJECT(r20)->unk_32 = 0xC0u;
		OBJECT(r20)->unk_40 = 4u;
		OBJECT(r20)->unk_3C = 0x19Eu;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_01A7D0[t1]);
	}
	t2 = OBJECT(r20)->unk_30 - 0xEu;
	OBJECT(r20)->unk_30 = t2;
	if ((int16_t)t2 >= 0)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_00 = 0u;
}

void sub_01A834(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0, t3 = 0, t4 = 0;
	uint32_t t1 = 0;

	if (mem_read8(OBJECT(r20)->unk_14 + 9u) != (uint8_t)(OBJECT(r20)->unk_08 >> 8u))
	{
		OBJECT(r20)->unk_00 = 0u;
		return;
	}
	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A84C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(r20)->unk_10 = 6u;
		OBJECT(r20)->unk_0C++;
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(r20)->unk_3E = 0u;
		r9 = rom_01A9A6[(uint8_t)(OBJECT(r20)->unk_08 >> 8u)];
		sub_00F366(r9);
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 24u);
		OBJECT(r20)->unk_3A = rom_01A9AC[r0];
		OBJECT(r20)->unk_30 = 0xA0u;
		OBJECT(r20)->unk_32 = 0xC0u;
		OBJECT(r20)->unk_3C = 0x19Du;
		OBJECT(r20)->unk_10 = 0x16u;
		OBJECT(r20)->unk_0C++;
		break;
	case 1:
		t2 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(r20)->unk_3E = 0u;
		r9 = rom_01A9A6[(uint8_t)(OBJECT(r20)->unk_08 >> 8u)];
		sub_00F366(r9);
		r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 24u);
		OBJECT(r20)->unk_3A = rom_01A9AC[r0];
		OBJECT(r20)->unk_30 = 0xA0u;
		OBJECT(r20)->unk_32 = 0xC0u;
		OBJECT(r20)->unk_3C = 0x19Du;
		OBJECT(r20)->unk_10 = 0x16u;
		OBJECT(r20)->unk_0C++;
		break;
	case 2:
		break;
	case 3:
		t3 = OBJECT(r20)->unk_10;
		OBJECT(r20)->unk_10 = t3 - 1u;
		if (t3 != 1u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(r20)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_01A84C[t1]);
	}
	sub_00F30D(0u);
	t4 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t4 - 1u;
	if (t4 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_3C--;
	OBJECT(r20)->unk_10 = 0x1Eu;
	OBJECT(r20)->unk_0C++;
	t3 = OBJECT(r20)->unk_10;
	OBJECT(r20)->unk_10 = t3 - 1u;
	if (t3 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(r20)->unk_00 = 0u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01A8F0[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		t2 = AS16(OBJECT(r20)->unk_10);
		AS16(OBJECT(r20)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01A8F0[t1]);
	}
	OBJECT(r20)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 4;
	r10 = 0x1A95Bu + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_32 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_3C = mem_read16(r10);
	AS16(OBJECT(r20)->unk_10) = mem_read16(r10 + 2u);
	OBJECT(r20)->unk_0C++;
	sub_00F30D(0u);
	r0 = sub_00EAE4(r0);
}

void sub_01AA01(void)
{
	uint8_t r10_2 = 0, r11_2 = 0;
	uint32_t r10 = 0, r11 = 0, r19 = 0, ignored = 0;
	uint32_t stack_10 = 0, stack_14 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		if (mem_read8(r25 + 0x80u) == 6u)
		{
			OBJECT(r20)->unk_00 = 0u;
			OBJECT(r20)->unk_01 = 0u;
		}
		if (unk_20E615 != 0u)
		{
			OBJECT(r20)->unk_0C++;
			r19 = 0x1AB62u;
			ignored = sub_011A32(r19);
			return;
		}
		OBJECT(r20)->unk_0C += 2u;
		break;
	case 1:
		r19 = 0x1AB62u;
		ignored = sub_011A32(r19);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_01AA0C[OBJECT(r20)->unk_0C]);
	}
	switch (mem_read8(r25))
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
		invalid_jump(rom_01AA4C[mem_read8(r25)]);
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
	OBJECT(r20)->unk_32 = 0xCFu;
	OBJECT(r20)->unk_3C = 0x19Eu;
	while ((int8_t)r10_2 >= 0xA)
	{
		r10_2 -= 0xAu;
		r12 = (r12 & ~0xFFu) | (uint8_t)(r12 + 1u);
	}
	if ((uint8_t)r12 != 0u)
	{
		AT8(OBJECT(r20)->unk_08, 1) = 0u;
		AT8(OBJECT(r20)->unk_08, 2) = rom_01AB8E[(uint8_t)r12];
		OBJECT(r20)->unk_30 = 0xACu;
		sub_01AB20();
	}
	AT8(OBJECT(r20)->unk_08, 1) = 0u;
	AT8(OBJECT(r20)->unk_08, 2) = rom_01AB8E[(uint8_t)r10_2];
	OBJECT(r20)->unk_30 = 0xB4u;
	sub_01AB20();
	if ((uint8_t)r11_2 != 0u)
	{
		AT8(OBJECT(r20)->unk_08, 1) = 1u;
		AT8(OBJECT(r20)->unk_08, 2) = r11_2;
		OBJECT(r20)->unk_30 = 0xC0u;
		sub_01AB20();
		AT8(OBJECT(r20)->unk_08, 1) = 2u;
		AT8(OBJECT(r20)->unk_08, 2) = unk_20E610;
		OBJECT(r20)->unk_30 = 0xC8u;
		sub_01AB20();
	}
}

void sub_01AB20(void)
{
	uint32_t r0 = 0, r9 = 0;

	OBJECT(r20)->unk_3E = 0u;
	r0 = (uint8_t)(OBJECT(r20)->unk_08 >> 8u);
	r9 = rom_01AB98[r0];
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = rom_01AB9E[r0];
	OBJECT(r20)->unk_40 = (uint8_t)(OBJECT(r20)->unk_08 >> 16u);
	sub_00EAE4(r0);
}

void sub_01ADE8(void)
{
	uint32_t zero = 0;
	uint32_t r10 = 0;
	uint32_t t1 = 0, target1 = 0;

	switch (OBJECT(r20)->unk_0C)
	{
	case 0:
		OBJECT(r20)->unk_80 = timeline_1AFC0_ADDRESS;
		OBJECT(r20)->unk_10 = 0u;
		OBJECT(r20)->unk_11 = 0u;
		OBJECT(r20)->unk_0C++;
		return;
	case 1:
		break;
	default:
		invalid_jump(rom_01ADF4[OBJECT(r20)->unk_0C]);
	}
	AS16(OBJECT(r20)->unk_10)++;
	r10 = OBJECT(r20)->unk_80;
	t1 = mem_read32(r10) - AS16(OBJECT(r20)->unk_10);
	zero = t1 == 0u;
	r10 += 4u;
	if ((int32_t)t1 < 0)
	{
		OBJECT(r20)->unk_00 = 0u;
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
		OBJECT(r20)->unk_80 += 8u;
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

	t1 = OBJECT(r20)->unk_0C;
	r0 = rom_01AE98[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		t2 = AS16(OBJECT(r20)->unk_10);
		AS16(OBJECT(r20)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(r20)->unk_00 = 0u;
			return;
		}
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_01AE98[t1]);
	}
	OBJECT(r20)->unk_3E = 0u;
	r0 = (uint32_t)(uint8_t)(OBJECT(r20)->unk_08 >> 8u) << 4;
	r10 = 0x1AF00u + r0;
	r9 = mem_read16(r10);
	r10 += 2u;
	sub_00F366(r9);
	OBJECT(r20)->unk_3A = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_34 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_36 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_30 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_32 = mem_read16(r10);
	r10 += 2u;
	OBJECT(r20)->unk_3C = mem_read16(r10);
	AS16(OBJECT(r20)->unk_10) = mem_read16(r10 + 2u);
	OBJECT(r20)->unk_0C++;
	sub_00F30D(0u);
	r0 = sub_00EAE4(r0);
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
	uint8_t r1_2 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t3 = 0, t4 = 0, t6 = 0, t7 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0, t8 = 0, t9 = 0, t10 = 0;
	uint32_t t11 = 0, t12 = 0;

	interrupts_off();
	r0 = sub_01F17D();
	mem_write8(SOUND_701FFF, 1u);
	r20 = 0x20E716u;
	r26 = r26 & ~0xFFu;
	set_bytes(WORK_RAM + 0xE716u, 0x19u, r26);
	OBJECT(r20)->unk_00 = 1u;
	OBJECT(r20)->unk_01 = 1u;
	sub_01C93B(r0);
	r10 = 0u;
	r0 = rom_03FFF8_ADDRESS;
	sub_01C98E(r10, &r0, &r1);
	t1 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFF8 != t1)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	OBJECT(r20)->unk_00 = (uint8_t)r0_2;
	r0 = (r0_2 & ~0xFFu) | 2u;
	r20 = r20 + 1u;
	if (rom_03FFFA != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	OBJECT(r20 - 0x1u)->unk_01 = r0;
	r20 = r20 + 1u;
	OBJECT(r20 - 0x2u)->unk_02 = 1u;
	OBJECT(r20 - 0x2u)->unk_03 = 1u;
	sub_01C93B(r0);
	r10 = 0x100000u;
	r0 = 0xFF000u;
	sub_01C98E(r10, &r0, &r1);
	t2 = r0;
	r0_2 = (uint16_t)((r0 & ~0xFFu) | 2u);
	if (rom_03FFFC != t2)
		r0_2 = (uint16_t)((r0_2 & ~0xFFu) | 3u);
	OBJECT(r20 - 0x2u)->unk_02 = (uint8_t)r0_2;
	r0 = (r0_2 & ~0xFFu) | 2u;
	r20 = r20 + 1u;
	if (rom_03FFFE != (uint16_t)r1)
		r0 = (r0 & ~0xFFu) | 3u;
	OBJECT(r20 - 0x3u)->unk_03 = r0;
	r20 = r20 + 1u;
	OBJECT(r20 - 0x4u)->unk_04 = 0x1010101u;
	sub_01C93B(r0);
	r10 = 0x300000u;
	r0 = 0x20000u;
	r0 = sub_01CB8F(r0, r10);
	t3 = r0 & 0xFu;
	r1_2 = 2u;
	if (t3 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(r20 - 0x4u)->unk_04) = r1_2;
	t4 = r0 & 0xF0u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t4 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x5u)->unk_04, 1) = r1_2;
	t5 = r0 & 0xF00u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t5 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x6u)->unk_04, 2) = r1_2;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if ((r0 & 0xF000u) != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x7u)->unk_04, 3) = r1_2;
	r20 = r20 + 1u;
	r0 = sub_0106AF();
	OBJECT(r20 - 0x8u)->unk_08 = 0x1010101u;
	sub_01C93B(r0);
	r10 = 0x400000u;
	r0 = 0x20000u;
	r0 = sub_01CB8F(r0, r10);
	t6 = r0 & 0xFu;
	r1_2 = 2u;
	if (t6 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(r20 - 0x8u)->unk_08) = r1_2;
	t7 = r0 & 0xF0u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t7 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x9u)->unk_08, 1) = r1_2;
	t8 = r0 & 0xF00u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t8 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0xAu)->unk_08, 2) = r1_2;
	t9 = r0 & 0xF000u;
	r0 = t9;
	r1_2 = 2u;
	r20 = r20 + 1u;
	AT8(OBJECT(r20 - 0xBu)->unk_08, 3) = r1_2;
	r20 = r20 + 1u;
	sub_0107F0();
	OBJECT(r20 - 0xCu)->unk_0C = 1u;
	OBJECT(r20 - 0xCu)->unk_0D = 1u;
	sub_01C93B(r0);
	r10 = 0x600000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0xCu)->unk_0C = r1_2;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0xDu)->unk_0D = r1_2;
	r20 = r20 + 1u;
	r0 = sub_01EF6E();
	OBJECT(r20 - 0xEu)->unk_0E = 1u;
	OBJECT(r20 - 0xEu)->unk_0F = 1u;
	sub_01C93B(r0);
	r10 = PALETTE_RAM + 0x4000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0xEu)->unk_0E = r1_2;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0xFu)->unk_0F = r1_2;
	r20 = r20 + 1u;
	r0 = sub_01EF6E();
	OBJECT(r20 - 0x10u)->unk_10 = 1u;
	OBJECT(r20 - 0x10u)->unk_11 = 1u;
	sub_01C93B(r0);
	r10 = 0x680000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0x10u)->unk_10 = r1_2;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0x11u)->unk_11 = r1_2;
	r20 = r20 + 1u;
	r0 = sub_01EF6E();
	OBJECT(r20 - 0x12u)->unk_12 = 1u;
	OBJECT(r20 - 0x12u)->unk_13 = 1u;
	sub_01C93B(r0);
	r10 = 0x684000u;
	r0 = 0x4000u;
	r0 = sub_01CA5D(r0, r10);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0x12u)->unk_12 = r1_2;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if ((r0 & 0xFF00u) != 0u)
		r1_2 = 3u;
	OBJECT(r20 - 0x13u)->unk_13 = r1_2;
	r20 = r20 + 1u;
	r0 = sub_01EF6E();
	OBJECT(r20 - 0x14u)->unk_14 = 0x1010101u;
	sub_01C93B(r0);
	move_halfwords(SPRITE_RAM, WORK_RAM, 0x10000u);
	r10 = 0x200000u;
	r0 = 0x20000u;
	r0 = sub_01C9BE(r0, r10);
	move_halfwords(WORK_RAM, SPRITE_RAM, 0x10000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(r20 - 0x14u)->unk_14) = r1_2;
	t10 = r0 & 0xFF00u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t10 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x15u)->unk_14, 1) = r1_2;
	t11 = r0 & 0xFF0000u;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t11 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x16u)->unk_14, 2) = r1_2;
	t12 = r0 & ~0xFFFFFFu;
	r0 = t12;
	r1_2 = 2u;
	r20 = r20 + 1u;
	if (t12 != 0u)
		r1_2 = 3u;
	AT8(OBJECT(r20 - 0x17u)->unk_14, 3) = r1_2;
	r20 = r20 + 1u;
	sub_0107F0();
	AS8(OBJECT(r20 - 0x18u)->unk_18) = 1u;
	sub_01C93B(r0);
	move_halfwords(SPRITE_RAM, SOUND_RAM, 0x1000u);
	r10 = 0x700000u;
	r0 = 0x1FFFu;
	r0 = sub_01CAF8(r0, r10);
	move_halfwords(SOUND_RAM, SPRITE_RAM, 0x1000u);
	r1_2 = 2u;
	if ((uint8_t)r0 != 0u)
		r1_2 = 3u;
	AS8(OBJECT(r20 - 0x18u)->unk_18) = r1_2;
	r20 = r20 + 1u;
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
	uint32_t t1 = 0;

	r26 = (r26 & ~0xFFFFu) | 0x6600u;
	r27 = TEXT_CELL(14, 25);
	r28 = 0x1CD87u;
	do
	{
		r26 = (r26 & ~0xFFu) | ROM8(r28);
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
		r20 = unk_20E600_ADDRESS;
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

	r20 = unk_20E600_ADDRESS;
	r10 = TEXT_CELL(32, 5);
	r1 = 0x64u;
	r2 = 2u;
	r0 = unk_20E630 + 1u;
	sub_01F103(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF3A8u;
	t1 = (uint8_t)OBJECT(r20)->unk_30 * 2u;
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
	r12 = r20 + 0x32u;
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
	r0 = (uint8_t)OBJECT(r20)->unk_24;
	r0 = r0 & 1u;
	r10 = rom_01DCE4[r0];
	sub_01F0CA(r0, r10);
	r0 = OBJECT(r20)->unk_0D;
	r0 = r0 & 1u;
	r10 = rom_01DCEC[r0];
	return sub_01F0CA(r0, r10);
}

void sub_01DB58(void)
{
	uint32_t ignored = 0;

	AS8(OBJECT(r20)->unk_30) = ((uint8_t)OBJECT(r20)->unk_30 + 1u) & 7u;
	ignored = sub_01DA7C();
}

void sub_01DB68(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0, ignored = 0;

	r1 = (uint8_t)(unk_20E701 >> 16u);
	r0 = (uint8_t)(mem_read8(r20 + 0x31u + r1) + 1u);
	if ((int8_t)r0 > 5)
		r0 = 0xFBu;
	mem_write8(r20 + 0x31u + r1, r0);
	ignored = sub_01DA7C();
}

void sub_01DB85(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = (uint8_t)OBJECT(r20)->unk_24 > 0u;
	AS8(OBJECT(r20)->unk_24) = 1u - carry;
	ignored = sub_01DA7C();
}

void sub_01DB97(void)
{
	uint32_t carry = 0;
	uint32_t ignored = 0;

	carry = OBJECT(r20)->unk_0D > 0u;
	OBJECT(r20)->unk_0D = 1u - carry;
	ignored = sub_01DA7C();
}

void sub_01DBA9(void)
{
	uint32_t r10 = 0;

	r10 = 0x1DD14u;
	r20 = unk_20E600_ADDRESS;
	do
	{
		r20 = (r20 & ~0xFFu) | ROM8(r10);
		r10++;
		if ((uint8_t)r20 == 0u)
		{
			AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
			return;
		}
	} while ((uint8_t)OBJECT(r20)->unk_3C == OBJECT(r20)->unk_00);
	unk_20E701 |= 2u;
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01DD1E(void)
{
	AT8(unk_20E701, 2) = 2u;
	sub_01DD2B();
}

void sub_01DD2B(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1E0F4u;
	sub_01F0CA(r0, r10);
	sub_01DDAC(&r0, &r1);
	sub_01F02C(r0);
	r10 = 0x310000u | rom_01E200[(uint8_t)(unk_20E701 >> 16u)];
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01DD58_ADDRESS;
	sub_01DD58(r1);
}

void sub_01DD58(uint32_t r1)
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
		r1 = (r1 & ~0xFFu) | 0xCu;
		r1 = (r1 & ~0xFFu) | ((uint8_t)r1 >> 2);
		if ((int8_t)r1 <= (int8_t)r0)
			r0 = 0u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01E200[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
	{
		r20 = unk_20E600_ADDRESS;
		switch ((uint8_t)(unk_20E701 >> 16u))
		{
		case 0:
			sub_01DE56();
			break;
		case 1:
			sub_01DEC7(r1);
			break;
		case 2:
			sub_01E0B0();
			break;
		case 3:
			sub_01E04F(r1);
			break;
		case 4:
			sub_01E073(r1);
			break;
		case 5:
			sub_01E09B(r1);
			break;
		case 6:
			sub_01E0A1();
			break;
		default:
			invalid_jump(rom_01E0D8[(uint8_t)(unk_20E701 >> 16u)]);
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

void sub_01DDAC(uint32_t *p_r0, uint32_t *p_r1)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;

	r20 = unk_20E600_ADDRESS;
	r10 = TEXT_CELL(32, 9);
	r1 = 0x64u;
	r2 = 2u;
	r0 = unk_20E614 + 1u;
	sub_01F103(r2, &r0, &r1, &r10);
	r11 = rom_01E220[(uint8_t)OBJECT(r20)->unk_14];
	r26 = r26 & ~0xFFFFu;
	r12 = TEXT_CELL(17, 10);
	r9 = 5u;
	for (;;)
	{
		r0 = mem_read8(r11);
		zero = r0 == 0u;
		r11++;
		if ((int8_t)r0 <= 0)
			break;
		r1 = (r1 & ~0xFFFFu) | 0x6400u;
		r10 = r12;
		r13 = 0x1E20Eu;
		do
		{
			mem_write16(r10, r1);
			r1 = (r1 & ~0xFFu) | ROM8(r13);
			r10 += 2u;
			r13++;
		} while ((uint8_t)r1 != 0u);
		r10 = r12;
		r1 = (r1 & ~0xFFFFu) | 0x6464u;
		r2 = 1u;
		r3 = 6u;
		sub_01F0ED(r0, r2, r3, &r1, &r10);
		r10 = r12 + 0x10u;
		r0 = mem_read8(r11);
		r1 = (r1 & ~0xFFFFu) | 0x6464u;
		r2 = 1u;
		r3 = 8u;
		r11++;
		sub_01F0ED(r0, r2, r3, &r1, &r10);
		r9--;
		r12 = r12 + 0x80u;
	}
	if (r9 == 0u)
	{
		*p_r0 = r0;
		*p_r1 = r1;
		return;
	}
	r10 = r12;
	do
	{
		move_string(r25, 0u, r10, 0x11u, 2u, 1u, 0u, r26, &r27, &r28);
		r9--;
		r10 += 0x80u;
	} while (r9 != 0u);
	if (zero)
	{
		*p_r0 = r0;
		*p_r1 = r1;
		return;
	}
	r0 = 0x6200u;
	for (;;)
	{
		r0 = (r0 & ~0xFFu) | mem_read8(r11);
		r11++;
		if ((uint8_t)r0 == 0u)
			break;
		mem_write16(r12, r0);
		r12 = r12 + 2u;
	}
	*p_r0 = r0;
	*p_r1 = r1;
}

void sub_01DE56(void)
{
	uint32_t r0 = 0, r10 = 0, ignored = 0;

	do
	{
		r0 = (uint8_t)OBJECT(r20)->unk_14;
		r0 = (uint8_t)(r0 + 1u);
		if ((int8_t)r0 >= 0x1A)
			r0 = 0u;
		AS8(OBJECT(r20)->unk_14) = r0;
		r10 = 0x1E3E8u + r0 * 4u;
		if ((int8_t)ROM8(r10) < 0)
		{
			OBJECT(r20)->unk_10 = 1u;
			OBJECT(r20)->unk_13 = 0xFFu;
			OBJECT(r20)->unk_11 = 1u;
			OBJECT(r20)->unk_12 = 1u;
			AT8(OBJECT(r20)->unk_14, 1) = 0xFFu;
			sub_01DDAC(&ignored, &ignored);
			return;
		}
	} while (r0 != 0u && OBJECT(r20)->unk_10 == ROM8(r10) && OBJECT(r20)->unk_13 == ROM8(r10 + 1u) && OBJECT(r20)->unk_11 == ROM8(r10 + 2u));
	OBJECT(r20)->unk_10 = ROM8(r10);
	r10++;
	OBJECT(r20)->unk_13 = ROM8(r10);
	r10++;
	OBJECT(r20)->unk_11 = ROM8(r10);
	OBJECT(r20)->unk_12 = ROM8(r10 + 1u);
	AT8(OBJECT(r20)->unk_14, 1) = 0u;
	sub_01DDAC(&ignored, &ignored);
}

void sub_01DEC7(uint32_t r1)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1E13Fu;
	sub_01F0CA(r0, r10);
	r0 = sub_01DF51(r1);
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 3u;
	r10 = 0x310000u | rom_01E20C;
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01DEFD_ADDRESS;
}

void sub_01DEFD(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 16u);
		r0 = (uint8_t)(r0 + 1u);
		r1 = 0x10u;
		r1 = r1 >> 2;
		if ((int8_t)r1 <= (int8_t)r0)
			r0 = 0u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01E206[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
	{
		r20 = unk_20E600_ADDRESS;
		switch ((uint8_t)(unk_20E701 >> 16u))
		{
		case 0:
			sub_01E04F(r1);
			break;
		case 1:
			sub_01E073(r1);
			break;
		case 2:
			sub_01E09B(r1);
			break;
		case 3:
			sub_01E0A1();
			break;
		default:
			invalid_jump(rom_01E0E4[(uint8_t)(unk_20E701 >> 16u)]);
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

uint32_t sub_01DF51(uint32_t r1)
{
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	r20 = unk_20E600_ADDRESS;
	r10 = TEXT_CELL(23, 8);
	r0 = unk_20E610;
	r1_2 = (r1 & ~0xFFFFu) | 0x6464u;
	r2 = 1u;
	r3 = 6u;
	sub_01F0ED(r0, r2, r3, &r1_2, &r10);
	r0 = OBJECT(r20)->unk_13;
	if ((int8_t)r0 > 0)
	{
		r10 = (r10 & ~0xFFFFu) | 0xF51Eu;
		r1_2 = 0x64u;
		r2 = 1u;
		sub_01F103(r2, &r0, &r1_2, &r10);
		r10 = 0x1E3CBu;
		sub_01F0CA(r0, r10);
	}
	else if ((int8_t)r0 < 0)
	{
		r10 = 0x1E3BCu;
		sub_01F0CA(r0, r10);
	}
	else
	{
		r10 = 0x1E39Eu;
		sub_01F0CA(r0, r10);
	}
	r10 = TEXT_CELL(32, 12);
	r0 = OBJECT(r20)->unk_11;
	r1_2 = (r1_2 & ~0xFFFFu) | 0x6464u;
	r2 = 1u;
	r3 = 6u;
	sub_01F0ED(r0, r2, r3, &r1_2, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF784u;
	r0 = OBJECT(r20)->unk_10;
	r1_2 = (int8_t)OBJECT(r20)->unk_13;
	r2 = OBJECT(r20)->unk_11;
	r3 = 0u;
	r4 = 0u;
	r9 = 9u;
	do
	{
		r3 += r2;
		if ((int32_t)r1_2 > 0)
		{
			r4 += r2;
			r5 = 0u;
			divux_pair(r1_2, &r4, &r5);
			r3 += r4;
			r4 = r5;
		}
		r5 = r3;
		r6 = 0u;
		divux_pair(r0, &r5, &r6);
		if ((uint8_t)r5 >= 9u)
		{
			r5 = (r5 & ~0xFFu) | 9u;
			r6 = 0u;
		}
		stack_4 = r6;
		r10 = sub_01E02E(r5, r10);
		r5 = stack_4;
		if (r5 != 0u)
		{
			mem_write16(r10, r5 + 0x6A7Fu);
			r10 += 2u;
			mem_write16(r10, r0 + 0x6A86u);
			r10 += 2u;
		}
		else
		{
			mem_write32(r10, 0x200020u);
			r10 += 4u;
		}
		r9--;
	} while (r9 != 0u);
	return r0;
}

uint32_t sub_01E02E(uint32_t r5, uint32_t r10)
{
	uint32_t zero = 0;
	uint32_t r6 = 0;

	r6 = 0u;
	divux_pair(0xAu, &r5, &r6);
	zero = (uint32_t)(r5 == 0u);
	if (zero == 0u)
	{
		mem_write16(r10, r5 + 0x6A30u);
		r10 += 2u;
	}
	else
	{
		mem_write16(r10, 0x20u);
		r10 += 2u;
	}
	mem_write16(r10, r6 + 0x6A30u);
	r10 += 2u;
	return r10;
}

void sub_01E04F(uint32_t r1)
{
	uint32_t r0 = 0;

	r0 = 0x10u;
	sub_01E053(r0, r1);
}

void sub_01E053(uint32_t r0, uint32_t r1)
{
	uint32_t r1_2 = 0, ignored = 0;

	r1_2 = (r1 & ~0xFFu) | mem_read8(r20 + r0);
	r1_2 = (r1_2 & ~0xFFu) | (uint8_t)(r1_2 + 1u);
	if ((uint8_t)r1_2 >= 0xAu)
		r1_2 = (r1_2 & ~0xFFu) | 1u;
	mem_write8(r20 + r0, r1_2);
	if ((uint8_t)(OBJECT(r20)->unk_14 >> 8u) == 0u)
		AS8(OBJECT(r20)->unk_14) = 0x1Au;
	ignored = sub_01DF51(r1_2);
}

void sub_01E073(uint32_t r1)
{
	uint8_t r0 = 0;
	uint32_t ignored = 0;
	uint8_t t1 = 0;

	r0 = OBJECT(r20)->unk_13;
	if (r0 == 0u)
		r0++;
	t1 = r0 + 1u;
	r0 = t1;
	if (t1 == 0u)
		AT8(OBJECT(r20)->unk_14, 1) = 0u;
	if (r0 >= 0xAu)
		r0 = 0u;
	OBJECT(r20)->unk_13 = r0;
	AS8(OBJECT(r20)->unk_14) = 0x1Au;
	ignored = sub_01DF51(r1);
}

void sub_01E09B(uint32_t r1)
{
	uint32_t r0 = 0;

	r0 = 0x11u;
	sub_01E053(r0, r1);
}

void sub_01E0A1(void)
{
	AT8(unk_20E701, 2) = 1u;
	AT32(unk_20E701, 3) = sub_01DD2B_ADDRESS;
}

void sub_01E0B0(void)
{
	uint32_t r10 = 0;

	r10 = 0x1E44Du;
	do
	{
		r20 = (r20 & ~0xFFu) | ROM8(r10);
		r10++;
		if ((uint8_t)r20 == 0u)
		{
			AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
			return;
		}
	} while ((uint8_t)OBJECT(r20)->unk_3C == OBJECT(r20)->unk_00);
	unk_20E701 |= 2u;
	sub_01139E();
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01E453(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1E62Cu;
	sub_01F0CA(r0, r10);
	r20 = unk_20E600_ADDRESS;
	if (unk_20E620 != 0u)
	{
		r10 = TEXT_CELL(8, 12);
		r0 = 0x6Eu;
		sub_01F16C(r0, r10);
		r10 = TEXT_CELL(8, 14);
		r0 = 0x6Eu;
		sub_01F16C(r0, r10);
	}
	unk_20E754 = (uint8_t)(OBJECT(r20)->unk_20 >> 8u) & 1u;
	r0 = sub_01E523();
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 3u;
	r10 = 0x310000u | rom_01E682;
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01E4BA_ADDRESS;
	sub_01E4BA();
}

void sub_01E4BA(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 16u);
		if (unk_20E620 == 0u)
		{
			r0 = (uint8_t)(r0 + 1u);
			if ((int8_t)r0 >= 4)
				r0 = 0u;
		}
		else
			r0 = r0 ^ 3u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01E67C[r0];
		sub_01F085(r0, r10);
	}
	if ((unk_20E70E & 2u) != 0u)
	{
		r20 = unk_20E600_ADDRESS;
		r0 = (uint8_t)(unk_20E701 >> 16u);
		switch (r0)
		{
		case 0:
			sub_01E57F(r0);
			break;
		case 1:
			sub_01E5C3();
			break;
		case 2:
			sub_01E5E7();
			break;
		case 3:
			sub_01E5F7();
			break;
		default:
			invalid_jump(rom_01E61C[r0]);
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

uint32_t sub_01E523(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;

	r20 = unk_20E600_ADDRESS;
	r0 = unk_20E620;
	r0 = r0 & 1u;
	r10 = rom_01E684[r0];
	sub_01F0CA(r0, r10);
	r0 = unk_20E754;
	r10 = rom_01E68C[r0];
	sub_01F0CA(r0, r10);
	r10 = TEXT_CELL(33, 14);
	r1 = 0x64u;
	r2 = 1u;
	r0 = OBJECT(r20)->unk_22 + 1u;
	sub_01F103(r2, &r0, &r1, &r10);
	if ((uint8_t)OBJECT(r20)->unk_20 != 0u)
	{
		r10 = TEXT_CELL(28, 12);
		r0 = (r0 & ~0xFFu) | 0x6Eu;
		r0 = sub_01F16C(r0, r10);
		r10 = TEXT_CELL(33, 14);
		r0 = (r0 & ~0xFFu) | 0x6Eu;
		r0 = sub_01F16C(r0, r10);
	}
	return r0;
}

void sub_01E57F(uint32_t r0)
{
	uint32_t carry = 0;
	uint32_t r0_2 = 0, r10 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;

	carry = (uint8_t)OBJECT(r20)->unk_20 > 0u;
	r0_2 = (r0 & ~0xFFu) | 1u;
	t1 = r0_2;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(t1 - carry);
	AS8(OBJECT(r20)->unk_20) = r0_2;
	AT8(OBJECT(r20)->unk_20, 1) = 1u;
	OBJECT(r20)->unk_23 = 0u;
	unk_20E754 = 1u;
	r0_2 = (r0_2 & ~0xFFu) | 0x60u;
	if (t1 != carry)
		r0_2 = (r0_2 & ~0xFFu) | 0x6Eu;
	r10 = TEXT_CELL(8, 12);
	stack_4 = r0_2;
	sub_01F16C(r0_2, r10);
	r10 = TEXT_CELL(8, 14);
	r0_2 = stack_4;
	sub_01F16C(r0_2, r10);
	ignored = sub_01E523();
}

void sub_01E5C3(void)
{
	uint32_t r0 = 0, r10 = 0, ignored = 0;
	uint8_t t1 = 0;

	r0 = unk_20E754;
	t1 = r0 - 1u;
	r0 = t1;
	if ((int8_t)t1 < 0)
		r0 = 1u;
	unk_20E754 = r0;
	r10 = 0x1E6E5u + r0 * 2u;
	AT8(OBJECT(r20)->unk_20, 1) = ROM8(r10);
	OBJECT(r20)->unk_23 = ROM8(r10 + 1u);
	ignored = sub_01E523();
}

void sub_01E5E7(void)
{
	uint32_t ignored = 0;

	OBJECT(r20)->unk_22 = (OBJECT(r20)->unk_22 + 1u) & 3u;
	ignored = sub_01E523();
}

void sub_01E5F7(void)
{
	uint32_t r10 = 0;

	r10 = 0x1E6E0u;
	do
	{
		r20 = (r20 & ~0xFFu) | ROM8(r10);
		r10++;
		if ((uint8_t)r20 == 0u)
		{
			AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
			return;
		}
	} while ((uint8_t)OBJECT(r20)->unk_3C == OBJECT(r20)->unk_00);
	unk_20E701 |= 2u;
	AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
}

void sub_01E6EB(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1EA5Cu;
	sub_01F0CA(r0, r10);
	r0 = sub_01E740();
	sub_01F043();
	AT32(unk_20E701, 3) = sub_01E703_ADDRESS;
	sub_01E703((uint16_t)r0);
}

void sub_01E703(uint16_t r0)
{
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = sub_01E715_ADDRESS;
	sub_01F052(r0);
}

void sub_01E715(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1EBB9u;
	sub_01F0CA(r0, r10);
	r0 = sub_01E862();
	sub_01F034();
	AT32(unk_20E701, 3) = sub_01E72D_ADDRESS;
	sub_01E72D((uint16_t)r0);
}

void sub_01E72D(uint16_t r0)
{
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	sub_01F052(r0);
}

uint32_t sub_01E740(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	r20 = SOUND_701F00;
	r10 = TEXT_CELL(26, 5);
	r0 = mem_read16(SOUND_701F06);
	r4 = r0;
	r1 = 0x64u;
	r2 = 5u;
	sub_01F103(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF334u;
	r0 = mem_read16(r20 + 0xAu);
	r4 += r0;
	r1 = 0x64u;
	r2 = 5u;
	sub_01F103(r2, &r0, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF3B2u;
	r0 = r4;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 6u;
	r3 = 0xFFFFFFFEu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF4B4u;
	r0 = mem_read16(r20 + 2u);
	r4 = r0;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF534u;
	r0 = mem_read16(r20 + 4u);
	r4 += r0;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF5B2u;
	r0 = r4;
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 6u;
	r3 = 0xFFFFFFFEu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF6B4u;
	r0 = mem_read16(r20 + 0x14u);
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r10 = (r10 & ~0xFFFFu) | 0xF734u;
	r0 = mem_read16(r20 + 0x16u);
	r1 = (r1 & ~0xFFFFu) | 0x646Au;
	r2 = 5u;
	r3 = 0xFFFFFFFDu;
	sub_01F0ED(r0, r2, r3, &r1, &r10);
	r11 = TEXT_CELL(20, 16);
	r0 = mem_read32(r20 + 0x10u);
	r11 = sub_01E9A4(r0, r11);
	r11 = (r11 & ~0xFFFFu) | 0xF8A8u;
	r0 = mem_read32(r20 + 0x18u);
	r11 = sub_01E9A4(r0, r11);
	r11 = (r11 & ~0xFFFFu) | 0xF928u;
	r0 = mem_read32(r20 + 0x1Cu);
	r11 = sub_01E9A4(r0, r11);
	r0 = mem_read16(r20 + 0x14u);
	t1 = mem_read16(r20 + 0x16u) + r0;
	r1 = t1;
	if (t1 == 0u)
		return r0;
	r11 = (r11 & ~0xFFFFu) | 0xFA34u;
	r0 = divide_u32(mem_read32(r20 + 0x18u) + mem_read32(r20 + 0x1Cu), r1);
	sub_01E9E6(&r0, &r11);
	r11 = (r11 & ~0xFFFFu) | 0xFAB4u;
	r0 = mem_read16(r20 + 0x20u);
	sub_01E9E6(&r0, &r11);
	r11 = (r11 & ~0xFFFFu) | 0xFB34u;
	r0 = mem_read16(r20 + 0x22u);
	sub_01E9E6(&r0, &r11);
	return r0;
}

uint32_t sub_01E862(void)
{
	uint32_t carry = 0;
	uint16_t r0_2 = 0, r1 = 0, r3 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0;
	uint32_t t1 = 0;

	r20 = SOUND_701F00;
	r10 = SOUND_RAM + 0x1F24u;
	r0 = 0u;
	r9 = 7u;
	do
	{
		r1 = mem_read16(r10);
		r10 += 2u;
		if (r0 < r1)
			r0 = r1;
		r9--;
	} while (r9 != 0u);
	r4 = 0u;
	if (r0 != 0u)
		r4 = divide_u32(0x50000000u, r0);
	r11 = r20 + 0x24u;
	r12 = TEXT_CELL(13, 5);
	r9 = 7u;
	do
	{
		r0 = mem_read16(r11);
		r5 = r0;
		r10 = r12;
		r1_2 = 0x64u;
		r2 = 5u;
		r11 += 2u;
		sub_01F103(r2, &r0, &r1_2, &r10);
		r5 = r5 * r4;
		carry = (r5 >> 25) & 1u;
		t1 = (r5 >> 26) + carry;
		r5 = t1;
		if (t1 != 0u)
		{
			r10 = r12 + 0xAu;
			do
			{
				mem_write16(r10, 0x6403u);
				r10 += 2u;
				r5--;
			} while (r5 != 0u);
		}
		r9--;
		r12 = r12 + 0x80u;
	} while (r9 != 0u);
	r10 = TEXT_CELL(19, 14);
	r11 = r20 + 0x32u;
	r9 = 0xAu;
	do
	{
		r0 = mem_read8(r11);
		r11++;
		if ((int8_t)r0 < 0)
			r11 += 5u;
		else
		{
			r12 = r10;
			r13 = 0x1ED94u + r0 * 3u * 2u;
			r0_2 = (uint16_t)(0x6400u | ROM8(r13));
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r13++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | ROM8(r13 + 1u));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | mem_read8(r11));
			r12 = r12 + 4u;
			mem_write16(r12, r0_2);
			r11++;
			r0_2 = (uint16_t)((r0_2 & ~0xFFu) | mem_read8(r11));
			r12 = r12 + 2u;
			mem_write16(r12, r0_2);
			r11++;
			r12 = r12 + 2u;
			mem_write16(r12, (r0_2 & ~0xFFu) | mem_read8(r11));
			r11++;
			r0 = mem_read16(r11);
			r3 = 0x6430u;
			r13 = 0x1E970u;
			r11 += 2u;
			r12 = r12 + 4u;
			do
			{
				r1_2 = 0u;
				r2 = ROM32(r13);
				r13 += 4u;
				divux_pair(r2, &r0, &r1_2);
				mem_write16(r12, r0 + r3);
				r12 = r12 + ROM32(r13);
				r0 = r1_2;
				r13 += 4u;
			} while ((int32_t)ROM32(r13) >= 0);
		}
		r9--;
		r10 += 0x80u;
	} while (r9 != 0u);
	return r0;
}

uint32_t sub_01E9A4(uint32_t r0, uint32_t r11)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	r1 = 0u;
	divux_pair(0x15180u, &r0, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 5u;
	r10 = r11;
	r11 += 0xCu;
	sub_01F103(r2, &r0, &r1, &r10);
	r0_2 = stack_4;
	r1 = 0u;
	divux_pair(0xE10u, &r0_2, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	r11 += 6u;
	sub_01F103(r2, &r0_2, &r1, &r10);
	r0_2 = stack_4;
	sub_01E9E6(&r0_2, &r11);
	return r11;
}

void sub_01E9E6(uint32_t *p_r0, uint32_t *p_r11)
{
	uint32_t r11 = *p_r11;
	uint32_t r1 = 0, r2 = 0, r10 = 0;
	uint32_t stack_4 = 0;

	r1 = 0u;
	divux_pair(0x3Cu, p_r0, &r1);
	stack_4 = r1;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	r11 += 6u;
	sub_01F103(r2, p_r0, &r1, &r10);
	*p_r0 = stack_4;
	r1 = 0x64u;
	r2 = 2u;
	r10 = r11;
	sub_01F103(r2, p_r0, &r1, &r10);
	*p_r11 = r11;
}

void sub_01EDC4(void)
{
	uint32_t r0 = 0, r10 = 0;

	r0 = sub_01F17D();
	r10 = 0x1EE79u;
	r0 = sub_01F0CA(r0, r10);
	sub_01F02C(r0);
	AT8(unk_20E701, 2) = 1u;
	r10 = 0x310000u | rom_01EEB0;
	sub_01F085(r0, r10);
	AT32(unk_20E701, 3) = sub_01EDF1_ADDRESS;
	sub_01EDF1();
}

void sub_01EDF1(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint16_t t3 = 0;
	uint32_t t2 = 0;

	if ((unk_20E70E & 1u) != 0u)
	{
		sub_01F09A();
		r0 = (uint8_t)(unk_20E701 >> 16u) ^ 1u;
		AT8(unk_20E701, 2) = r0;
		r10 = 0x310000u | rom_01EEAE[r0];
		sub_01F085(r0, r10);
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
		mem_write16(unk_20E708, 0x6A7Fu);
		r0 = sub_0113E5(r0);
		r10 = 0x1EEB2u;
		sub_01F0CA(r0, r10);
		r10 = 0x310000u | rom_01F282;
		r0 = 0x6Eu;
		sub_01F16C(r0, r10);
		sub_01F034();
		AT32(unk_20E701, 3) = sub_01EE66_ADDRESS;
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

void sub_01EE66(void)
{
	if ((unk_20E70E & 2u) != 0u)
		AT32(unk_20E701, 3) = sub_01C392_ADDRESS;
	sub_01F052(0u);
}

void sub_01EEC1(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = unk_20E701;
	unk_20E701 = t1 & ~2u;
	if ((t1 & 2u) != 0u)
	{
		sub_00036B();
		if (unk_20E60A < 0x2710u)
		{
			unk_20E700 = 0u;
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			return;
		}
		r0 = sub_01F17D();
		r10 = 0x1EF12u;
		sub_01F0CA(r0, r10);
		sub_01F034();
		AT32(unk_20E701, 3) = sub_01EEEF_ADDRESS;
		return;
	}
	unk_20E700 = 0u;
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
}

void sub_01EEEF(void)
{
	if ((unk_20E70E & 2u) != 0u)
	{
		unk_20E700 = 0u;
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		return;
	}
	sub_01F052(0u);
}

uint32_t sub_01EF6E(void)
{
	uint16_t r0_2 = 0, r1 = 0, r2 = 0;
	uint32_t r0 = 0, r1_2 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;

	r26 = (r26 & ~0xFFFFu) | 0x733u;
	set_halfwords(PALETTE_RAM + 0x8000u, 0x4000u, r26);
	set_halfwords(0x688000u, 0x4000u, r26);
	r0 = rom_01F302;
	r0 = r0 >> 1;
	move_halfwords(PALETTE_RAM + 0x9E00u, 0x1F304u, r0);
	r0 = rom_01F504;
	r0 = r0 >> 1;
	move_string(0x1F506u, r0, 0x689E00u, r0, 2u, 0u, 0u, r26, &r27, &r28);
	r27 = r27 & ~0x8000u;
	r0_2 = 0u;
	r9 = 0x20u;
	r27 = r27 + 2u;
	do
	{
		r10 = r27;
		r11 = r10 & ~0x80000u;
		r1 = (uint16_t)r0_2;
		mem_write16(r11, r1);
		mem_write16(r10, r1);
		r2 = (uint16_t)r1;
		r1 = (uint16_t)((uint32_t)r1 << 5);
		r11 += 2u;
		mem_write16(r11, r1);
		r10 += 2u;
		mem_write16(r10, r1);
		r2 += r1;
		r1 = (uint16_t)((uint32_t)r1 << 5);
		r11 += 2u;
		mem_write16(r11, r1);
		r10 += 2u;
		mem_write16(r10, r1);
		r2 += r1;
		mem_write16(r11 + 2u, r2);
		mem_write16(r10 + 2u, r2);
		r0_2++;
		r9--;
		r27 = r27 + 0x20u;
	} while (r9 != 0u);
	r10 = VIDEO_RAM + 0x18020u;
	r0 = 0u;
	r1_2 = 0x11111111u;
	r9 = 4u;
	do
	{
		r0 += r1_2;
		r8 = 8u;
		do
		{
			mem_write32(r10, r0);
			r10 += 4u;
			r8--;
		} while (r8 != 0u);
		r9--;
	} while (r9 != 0u);
	return r0;
}

void sub_01F02C(uint32_t r0)
{
	uint32_t r10 = 0;

	r10 = rom_01F282_ADDRESS;
	r0 = sub_01F0CA(r0, r10);
}

void sub_01F034(void)
{
	unk_20E708 = 0x1F2BCu;
	unk_20E72F = 0u;
}

void sub_01F043(void)
{
	unk_20E708 = 0x1F2DEu;
	unk_20E72F = 0u;
}

void sub_01F052(uint32_t r0)
{
	uint32_t r10 = 0, r11 = 0;
	uint8_t t1 = 0;

	t1 = unk_20E72F - 1u;
	unk_20E72F = t1;
	if ((int8_t)t1 >= 0)
		return;
	r10 = unk_20E708;
	r11 = 0x310000u | mem_read16(r10);
	if (mem_read16(r11) == 0u)
	{
		sub_01F0CA(r0, r10);
		unk_20E72F = 0x14u;
		return;
	}
	do
	{
		mem_write16(r11, 0u);
		r11 += 2u;
	} while (mem_read16(r11) != 0u);
	unk_20E72F = 0xAu;
}

uint32_t sub_01F085(uint32_t r0, uint32_t r10)
{
	uint32_t r0_2 = 0;

	unk_20E708 = r10;
	unk_20E72F = 0u;
	r10 += 4u;
	r0_2 = (r0 & ~0xFFu) | 0x6Au;
	return sub_01F16C(r0_2, r10);
}

void sub_01F09A(void)
{
	uint32_t r0 = 0, r10 = 0;

	r10 = unk_20E708;
	mem_write16(r10, 0u);
	r10 += 4u;
	r0 = 0x60u;
	r0 = sub_01F16C(r0, r10);
}

uint32_t sub_01F0CA(uint32_t r0, uint32_t r10)
{
	uint32_t r11 = 0;

	r11 = TEXT_CELL(0, 0);
	for (;;)
	{
		r11 = (r11 & ~0xFFFFu) | mem_read16(r10);
		r10 += 2u;
		if ((uint16_t)r11 == 0u)
			break;
		for (;;)
		{
			r0 = (r0 & ~0xFFu) | mem_read8(r10);
			r10++;
			if ((uint8_t)r0 == 0u)
				break;
			if ((int8_t)r0 >= 0)
			{
				mem_write16(r11, r0);
				r11 += 2u;
				continue;
			}
			r0 = (r0 & ~0xFFu) | (uint8_t)~r0;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 8);
		}
	}
	return r0;
}

void sub_01F0ED(uint32_t r0, uint32_t r2, uint32_t r3, uint32_t *p_r1, uint32_t *p_r10)
{
	uint32_t r1 = *p_r1;

	r1 = r1 << 8;
	if (r0 <= 1u)
	{
		mem_write16(*p_r10 + r3 * 2u, r1);
		r1 = byte_swap32(r1 << 8);
		sub_01F103(r2, &r0, &r1, p_r10);
		*p_r1 = r1;
		return;
	}
	r1 |= 0x53u;
	mem_write16(*p_r10 + r3 * 2u, r1);
	r1 = byte_swap32(r1 << 8);
	sub_01F103(r2, &r0, &r1, p_r10);
	*p_r1 = r1;
}

void sub_01F103(uint32_t r2, uint32_t *p_r0, uint32_t *p_r1, uint32_t *p_r10)
{
	uint8_t r1 = (uint8_t)*p_r1;
	uint32_t r10 = *p_r10;
	uint16_t r3 = 0;
	uint32_t r1_2 = 0;

	r10 = r10 + r2 + r2;
	r3 = (uint16_t)((uint16_t)((uint32_t)(uint8_t)r1 << 8) | 0x30u);
	for (;;)
	{
		r1_2 = 0u;
		divux_pair(0xAu, p_r0, &r1_2);
		r1_2 = (r1_2 & ~0xFFFFu) | (uint16_t)(r1_2 + r3);
		r10 -= 2u;
		mem_write16(r10, r1_2);
		if (*p_r0 == 0u)
			break;
		r2--;
		if (r2 == 0u)
		{
			*p_r1 = r1_2;
			*p_r10 = r10;
			return;
		}
	}
	for (;;)
	{
		r2--;
		if (r2 == 0u)
			break;
		r10 -= 2u;
		mem_write16(r10, 0u);
	}
	*p_r1 = r1_2;
	*p_r10 = r10;
}

uint32_t sub_01F131(uint32_t r0, uint8_t r1, uint32_t r2, uint32_t r10)
{
	uint16_t r3 = 0;
	uint32_t r1_2 = 0;

	r10 = r10 + r2 + r2;
	r3 = (uint16_t)((uint16_t)((uint32_t)(uint8_t)r1 << 8) | 0x30u);
	for (;;)
	{
		r1_2 = 0u;
		divux_pair(0x10u, &r0, &r1_2);
		r1_2 = (uint16_t)(r1_2 + r3);
		if ((int8_t)r1_2 > 0x39)
			r1_2 = (r1_2 & ~0xFFu) | (uint8_t)(r1_2 + 7u);
		r10 -= 2u;
		mem_write16(r10, r1_2);
		if (r0 == 0u)
			break;
		r2--;
		if (r2 == 0u)
			return r10;
	}
	for (;;)
	{
		r2--;
		if (r2 == 0u)
			break;
		r10 -= 2u;
		mem_write16(r10, 0u);
	}
	return r10;
}

uint32_t sub_01F16C(uint32_t r0, uint32_t r10)
{
	uint16_t t1 = 0;

	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 8);
	for (;;)
	{
		t1 = mem_read16(r10) & ~0xE00u;
		mem_write16(r10, t1);
		if (t1 == 0u)
			break;
		mem_write16(r10, mem_read16(r10) | r0);
		r10 += 2u;
	}
	return r0;
}

uint32_t sub_01F17D(void)
{
	uint32_t r0 = 0;

	r0 = 0x780u;
	r26 = r26 & ~0xFFFFu;
	move_string(r25, 0u, TEXT_CELL(0, 0), 0x780u, 2u, 1u, 0u, r26, &r27, &r28);
	return r0;
}
