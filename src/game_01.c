/**
 *    Name:        game_01.c
 *    Description: the game's functions, part 01 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_006070(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0;

	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 0xEEu;
	r9 = 0x1Bu;
	sub_00F366(r9);
	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
	OBJECT(current_object)->unk_3A = rom_006E80[r0];
	OBJECT(current_object)->unk_24 = r0 << 9;
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_006173_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	OBJECT(current_object)->unk_04 = sub_0060B1_ADDRESS;
	sub_0060B1();
}

void sub_0060B1(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r10 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0, t3 = 0;

	r10 = OBJECT(current_object)->unk_14;
	t1 = ~~mem_read8(r10 + 0x12u);
	OBJECT(current_object)->unk_12 = t1;
	if (t1 == 0u)
	{
		OBJECT(current_object)->unk_00 &= 0xDFu;
		return;
	}
	r0 = OBJECT(current_object)->unk_24;
	r0 = (uint16_t)(r0 - mem_read16(mem_read32(r10 + 0x14u) + 0x24u));
	r0 = r0 & 0xFFFu;
	r2 = mem_read32(screen + 0xF08u) + 0x140u;
	r1 = (int32_t)((int16_t)rom_100C00[r0] * r2) >> 14;
	r3 = (int32_t)((0u - (int16_t)rom_101400[r0]) * r2) >> 14;
	r2 = 0x44u;
	r3 += 0x140u;
	r0 = 0x100000u;
	t2 = mem_read32(screen + 0xF04u);
	r3 += 0x100u;
	t3 = r3 + t2;
	overflow = v60_overflow_add32(r3, t2, 0u);
	zero = t3 == 0u;
	negative = t3 >> 31;
	r3 = t3;
	if ((negative ^ overflow) | zero)
	{
		OBJECT(current_object)->unk_00 &= 0xDFu;
		return;
	}
	r0 = divide_s32(r0, r3);
	r1 = r1 * r0;
	carry = (r1 >> 11) & 1u;
	r1 = (uint16_t)(((int32_t)r1 >> 12) + 0xA0u + carry);
	r2 = r2 * r0;
	carry = (r2 >> 11) & 1u;
	OBJECT(current_object)->unk_30 = r1;
	OBJECT(current_object)->unk_32 = ((int32_t)r2 >> 12) + 0x60u + carry;
	r0 = sub_00EAE4(r0);
}

void sub_006105(void)
{
	uint32_t r9 = 0, r21 = 0;

	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 0xEEu;
	r9 = 0x1Bu;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = rom_006E80[(uint8_t)(OBJECT(current_object)->unk_08 >> 16u)];
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_006173_ADDRESS);
	mem_write32(r21 + 0x14u, current_object);
	OBJECT(current_object)->unk_04 = sub_00613F_ADDRESS;
	sub_00613F();
}

void sub_00613F(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;

	r10 = OBJECT(current_object)->unk_14;
	if ((mem_read32(r10) & 0x20u) != 0u)
	{
		t1 = ~~mem_read8(r10 + 0x12u);
		OBJECT(current_object)->unk_12 = t1;
		if (t1 != 0u)
		{
			OBJECT(current_object)->unk_30 = mem_read16(r10 + 0x30u) + 3u;
			r0 = (uint16_t)(mem_read16(r10 + 0x32u) - 0x14u);
			OBJECT(current_object)->unk_32 = r0;
			r0 = sub_00EAE4(r0);
			return;
		}
	}
	OBJECT(current_object)->unk_00 &= 0xDFu;
}

void sub_006173(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 0xEFu;
	r9 = rom_006E90[unk_20E030[mem_read8(OBJECT(current_object)->unk_14 + 9u)]];
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x27u;
	OBJECT(current_object)->unk_04 = sub_0061A2_ADDRESS;
	sub_0061A2();
}

void sub_0061A2(void)
{
	uint32_t r0 = 0, r10 = 0;
	uint8_t t1 = 0;

	r10 = OBJECT(current_object)->unk_14;
	if ((mem_read32(r10) & 0x20u) != 0u)
	{
		t1 = ~~mem_read8(r10 + 0x12u);
		OBJECT(current_object)->unk_12 = t1;
		if (t1 != 0u)
		{
			OBJECT(current_object)->unk_30 = mem_read16(r10 + 0x30u) + 0x13u;
			r0 = (uint16_t)(mem_read16(r10 + 0x32u) - 0x10u);
			OBJECT(current_object)->unk_32 = r0;
			r0 = sub_00EAE4(r0);
			return;
		}
	}
	OBJECT(current_object)->unk_00 &= 0xDFu;
}

uint32_t sub_0061DB(uint32_t r1, uint32_t r2, uint32_t r3)
{
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t r3_4 = 0;
	uint16_t r3_3 = 0;
	uint32_t r0 = 0, r3_2 = 0;
	uint16_t t3 = 0;
	uint32_t t1 = 0, t2 = 0;

	r0 = 0x100000u;
	t1 = mem_read32(screen + 0xF04u);
	r3 += 0x100u;
	t2 = r3 + t1;
	overflow = v60_overflow_add32(r3, t1, 0u);
	zero = t2 == 0u;
	negative = t2 >> 31;
	r3_2 = t2;
	if (((negative ^ overflow) | zero) == 0u)
	{
		r0 = divide_s32(r0, r3_2);
		t3 = (uint16_t)r3_2 >> 4;
		r3_3 = t3;
		if (t3 != 0u)
		{
			OBJECT(current_object)->unk_3C = 0u - (r3_3 - 0x100u);
			r3_4 = 0xF4u;
			r1 = r1 * r0;
			carry = (r1 >> 11) & 1u;
			r1 = ((int32_t)r1 >> 12) + 0xA0u + carry;
			if ((int32_t)r1 <= -0x28 || (int32_t)r1 >= 0x168)
			{
				OBJECT(current_object)->unk_00 &= 0xDFu;
				return r0;
			}
			r2 = r2 * r0;
			carry = v60_sha_cy(r2, r3_4, 32);
			OBJECT(current_object)->unk_30 = r1;
			OBJECT(current_object)->unk_32 = shift_signed_by(r2, r3_4, 32) + 0x60u + carry;
			r0 = r0 * OBJECT(current_object)->unk_1C;
			carry = v60_sha_cy(r0, r3_4, 32);
			r0 = shift_signed_by(r0, r3_4, 32);
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + carry);
			OBJECT(current_object)->unk_34 = r0;
			OBJECT(current_object)->unk_36 = r0;
			return sub_00EAE4(r0);
		}
	}
	OBJECT(current_object)->unk_00 &= 0xDFu;
	return r0;
}

uint32_t sub_006294(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0;

	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
	if (r0 != 0u)
		AT8(OBJECT(current_object)->unk_08, 3) = r0 - 1u;
	switch (OBJECT(current_object)->unk_0D)
	{
	case 0:
		if (mem_read8(screen + 0x101u) < 0xC0u)
		{
			if ((mem_read32(screen + 0x118u) & 0x40u) != 0u)
			{
				if ((uint8_t)(OBJECT(current_object)->unk_08 >> 24u) == 0u)
				{
					zero = 1u == 0u;
					return zero;
				}
			}
			zero = 0u == 0u;
			return zero;
		}
		OBJECT(current_object)->unk_0D++;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 24u) != 0u)
		{
			zero = 0u == 0u;
			return zero;
		}
		zero = 1u == 0u;
		return zero;
	case 1:
		if (mem_read8(screen + 0x101u) >= 0x40u)
		{
			if ((mem_read32(screen + 0x118u) & 0x40u) != 0u)
			{
				if ((uint8_t)(OBJECT(current_object)->unk_08 >> 24u) == 0u)
				{
					zero = 1u == 0u;
					return zero;
				}
			}
			zero = 0u == 0u;
			return zero;
		}
		OBJECT(current_object)->unk_0D--;
		if ((mem_read32(screen + 0x118u) & 0x40u) != 0u)
		{
			if ((uint8_t)(OBJECT(current_object)->unk_08 >> 24u) == 0u)
			{
				zero = 1u == 0u;
				return zero;
			}
		}
		zero = 0u == 0u;
		return zero;
	default:
		invalid_jump(rom_0062B0[OBJECT(current_object)->unk_0D]);
	}
}

void sub_006356(void)
{
	uint8_t r0 = 0;
	uint32_t r9 = 0, r19 = 0, r21 = 0;

	r19 = 0x6988u + mem_read8(screen) * 5u * 4u;
	sub_0179AC(r19);
	r19 = 0x13F66Cu;
	sub_0184D0(r19);
	sub_018AF0();
	mem_write8(screen + 0xF35u, 0xFFu);
	r0 = 0xFu;
	sub_011EB8(r0);
	r0 = 0x3Bu;
	sub_019402(r0);
	mem_write8(screen + 0xE2u, 0xFFu);
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_0064E2_ADDRESS);
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00652A_ADDRESS);
	OBJECT(current_object)->unk_14 = r21;
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00660F_ADDRESS);
	OBJECT(current_object)->unk_3E = 0u;
	OBJECT(current_object)->unk_3C = 0x19Du;
	r9 = 0x26Fu;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x4Eu;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0x7Au;
	OBJECT(current_object)->unk_04 = sub_0063F9_ADDRESS;
	sub_0063F9();
}

void sub_0063F9(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, r1 = 0;
	uint32_t t1 = 0;

	r0 = unk_20FC1A;
	if (r0 == 0u)
	{
		mem_write8(screen + 0xE2u, 0u);
		sub_011FE6();
		OBJECT(current_object)->unk_04 = sub_006463_ADDRESS;
		sub_006463();
		return;
	}
	t1 = r0 - 6u;
	r0 = t1;
	if ((int32_t)t1 < 0)
		r0 = 0u;
	r1 = 0u;
	divux_pair(0x1Eu, &r0, &r1);
	OBJECT(current_object)->unk_40 = r0;
	mem_write8(OBJECT(current_object)->unk_14 + 0x10u, r1);
	move_string(rom_006464[(r1 + 2u) / 5u], 7u, screen + 0xF12u, 7u, 2u, 0u, 0u, r26, &r27, &r28);
	r1 = 0u;
	divux_pair(0xAu, &r0, &r1);
	zero = (uint32_t)(r0 == 0u);
	if (zero == 0u)
	{
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 0x30u);
		mem_write8(screen + 0xF18u, r0);
	}
	mem_write8(screen + 0xF19u, r1 + 0x30u);
	r0 = sub_00EAE4(r0);
}

void sub_006463(void)
{
}

void sub_0064E2(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->unk_3E = 0u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	r9 = 0x287u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x287u;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0x70u;
	OBJECT(current_object)->unk_04 = sub_006514_ADDRESS;
	sub_006514();
}

void sub_006514(void)
{
	uint32_t r0 = 0;

	r0 = OBJECT(current_object)->unk_10;
	r0 = (uint8_t)(r0 + 1u);
	OBJECT(current_object)->unk_10 = r0;
	if ((r0 & 0x10u) != 0u)
		return;
	r0 = sub_00EAE4(r0);
}

void sub_00652A(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->unk_3E = 0u;
	OBJECT(current_object)->unk_3C = 0x19Bu;
	r9 = rom_0069B8;
	OBJECT(current_object)->unk_3A = rom_0069BA + 4u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_34 = 0x40u;
	OBJECT(current_object)->unk_36 = 0x40u;
	OBJECT(current_object)->unk_04 = sub_006560_ADDRESS;
	sub_006560();
}

void sub_006560(void)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t t1 = 0;

	r0 = OBJECT(current_object)->unk_10;
	if (r0 == 0u)
	{
		r0 = OBJECT(current_object)->unk_12;
		r0 = (uint8_t)(r0 + 1u);
		r0 = r0 & 7u;
		OBJECT(current_object)->unk_12 = r0;
		r10 = rom_0069B8_ADDRESS + r0 * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->unk_3A = ROM16(rom_000002_ADDRESS + r10) + 4u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_34 = 0x40u;
		OBJECT(current_object)->unk_36 = 0x40u;
		return;
	}
	if ((int8_t)r0 > 0x10)
		r0 = 0x10u;
	OBJECT(current_object)->unk_10 = r0;
	do
	{
		r0 = OBJECT(current_object)->unk_10;
		r0 = (r0 & 0xFu) << 8;
		r1 = (int16_t)rom_100C00[r0];
		r1 += r1 * 2u * 2u;
		carry = (r1 >> 9) & 1u;
		r1 = (uint16_t)(((int32_t)r1 >> 10) + 0xA0u + carry);
		r2 = 0u - (int16_t)rom_101400[r0];
		r0 = r2 * 2u * 2u;
		r2 += r0;
		carry = (r2 >> 9) & 1u;
		OBJECT(current_object)->unk_30 = r1;
		OBJECT(current_object)->unk_32 = ((int32_t)r2 >> 10) + 0x70u + carry;
		sub_00EAE4(r0);
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
	} while (t1 != 1u);
	ignored = sub_00F30D(0u);
	ignored = sub_00F30D(ignored);
}

void sub_00660F(void)
{
	uint32_t r0 = 0, r21 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = sub_011E45();
		if ((r0 & 0xFu) == 0u)
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_00664E_ADDRESS);
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		return;
	case 1:
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 == 1u)
			OBJECT(current_object)->unk_0C--;
		return;
	default:
		invalid_jump(rom_00661C[OBJECT(current_object)->unk_0C]);
	}
}

void sub_00664E(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = sub_011E45();
		r1 = byte_swap32(r0);
		r0 &= 7u;
		AT8(OBJECT(current_object)->unk_08, 1) = r0;
		r10 = rom_0069B8_ADDRESS + r0 * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->unk_3A = ROM16(rom_000002_ADDRESS + r10);
		sub_00F366(r9);
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 3u;
		OBJECT(current_object)->unk_40 = 6u;
		OBJECT(current_object)->unk_0C++;
		t1 = r1 & 0x7Fu;
		r1 = (r1 & ~0xFFu) | t1;
		if (t1 == 0u)
		{
			OBJECT(current_object)->unk_12 = 0x2Du;
			OBJECT(current_object)->unk_13 = 0u;
			OBJECT(current_object)->unk_0C += 2u;
		}
		if ((int32_t)r1 >= 0)
		{
			r0 = AS16(OBJECT(current_object)->unk_10);
			AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
			r10 = 0x67B2u + r0 * 3u * 2u;
			r0 = ROM16(r10);
			r10 += 2u;
			if (r0 != 0u)
			{
				OBJECT(current_object)->unk_34 = r0;
				OBJECT(current_object)->unk_36 = r0;
				OBJECT(current_object)->unk_30 = ROM16(r10);
				OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
				r0 = sub_00EAE4(r0);
				return;
			}
			r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			sub_011EB8(r0_2);
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_40 = 7u;
		OBJECT(current_object)->unk_0C++;
		r0 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			sub_011EB8(r0_2);
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->unk_30 = r0;
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		if ((OBJECT(current_object)->unk_12 | OBJECT(current_object)->unk_13) != 0u)
		{
			t2 = AS16(OBJECT(current_object)->unk_12);
			AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
			if (t2 != 1u)
			{
				r0 = AS16(OBJECT(current_object)->unk_10);
				AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
				r10 = 0x67B2u + r0 * 3u * 2u;
				r0 = ROM16(r10);
				r10 += 2u;
				if (r0 != 0u)
				{
					OBJECT(current_object)->unk_34 = r0;
					OBJECT(current_object)->unk_36 = r0;
					OBJECT(current_object)->unk_30 = ROM16(r10);
					OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
					r0 = sub_00EAE4(r0);
					return;
				}
				r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
				sub_011EB8(r0_2);
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
		}
		ignored = sub_00F30D(0u);
		ignored = sub_00F30D(ignored);
		ignored = sub_00F30D(ignored);
		ignored = sub_00F30D(ignored);
		sub_00F30D(ignored);
		r0_2 = 4u;
		sub_011FC9(r0_2);
		r0 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 != 0u)
		{
			OBJECT(current_object)->unk_34 = r0;
			OBJECT(current_object)->unk_36 = r0;
			OBJECT(current_object)->unk_30 = ROM16(r10);
			OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
			r0 = sub_00EAE4(r0);
			return;
		}
		r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
		sub_011EB8(r0_2);
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 1:
		r0 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 != 0u)
		{
			OBJECT(current_object)->unk_34 = r0;
			OBJECT(current_object)->unk_36 = r0;
			OBJECT(current_object)->unk_30 = ROM16(r10);
			OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
			r0 = sub_00EAE4(r0);
			return;
		}
		r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
		sub_011EB8(r0_2);
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 4:
		if ((OBJECT(current_object)->unk_12 | OBJECT(current_object)->unk_13) != 0u)
		{
			t3 = AS16(OBJECT(current_object)->unk_12);
			AS16(OBJECT(current_object)->unk_12) = t3 - 1u;
			if (t3 != 1u)
			{
				r0 = AS16(OBJECT(current_object)->unk_10);
				AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
				r10 = 0x67B2u + r0 * 3u * 2u;
				r0 = ROM16(r10);
				r10 += 2u;
				if (r0 == 0u)
				{
					r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
					sub_011EB8(r0_2);
					OBJECT(current_object)->unk_00 = 0u;
					return;
				}
				OBJECT(current_object)->unk_34 = r0;
				OBJECT(current_object)->unk_36 = r0;
				r0 = (uint16_t)(0x96u - ROM16(r10));
				OBJECT(current_object)->unk_30 = r0;
				OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
				r0 = sub_00EAE4(r0);
				return;
			}
		}
		ignored = sub_00F30D(0u);
		ignored = sub_00F30D(ignored);
		ignored = sub_00F30D(ignored);
		ignored = sub_00F30D(ignored);
		sub_00F30D(ignored);
		r0_2 = 4u;
		sub_011FC9(r0_2);
		r0 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			sub_011EB8(r0_2);
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->unk_30 = r0;
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = r0 + 1u;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		r10 += 2u;
		if (r0 == 0u)
		{
			r0_2 = rom_0067A9[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
			sub_011EB8(r0_2);
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->unk_30 = r0;
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00665C[OBJECT(current_object)->unk_0C]);
	}
}

void sub_0068FE(void)
{
	uint32_t r0 = 0, r10 = 0;

	r10 = OBJECT(current_object)->unk_18;
	r0 = (int16_t)mem_read16(r10);
	r10 += 2u;
	if ((int16_t)r0 < 0)
	{
		r10 += r0;
		r0 = mem_read16(r10);
		r10 += 2u;
	}
	OBJECT(current_object)->unk_3A = r0;
	OBJECT(current_object)->unk_18 = r10;
}

void sub_006EF0(void)
{
	bool L_0071FB = false, L_007E41 = false, L_007F31 = false;
	uint32_t overflow = 0;
	uint8_t r0_3 = 0, r9_2 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r17 = 0, r19 = 0, r21 = 0, ignored = 0;
	uint8_t t2 = 0, t3 = 0;
	uint16_t t1 = 0, t4 = 0, t5 = 0, t7 = 0;
	uint32_t target1 = 0, t6 = 0;

	unk_20E0D0[mem_read8(screen)] = 0u;
	sub_007443();
	sub_00A098();
	sub_012238();
	do
	{
		do
		{
			target1 = L_0071FB ? 0x7134u : L_007F31 ? 0x70B9u : rom_006F18[OBJECT(current_object)->unk_0C];
			switch (target1)
			{
			case 0x006F30u:
				mem_write8(screen + 0xA6u, mem_read8(screen + 0xA3u));
				r0 = rom_010BC8;
				r1 = mem_read8(mem_read32(screen + 0x308u) + 0x1Fu);
				r2 = rom_010BCC_ADDRESS;
				if (unk_20E623 != 0u)
					r1 = 0u;
				mem_write16(screen + 0x12u, mem_read16(r0 + r1 * 2u));
				mem_write16(screen + 0x10u, ROM16(r2));
				r2 += 2u;
				mem_write16(screen + 0x14u, ROM16(r2));
				r2 += 2u;
				mem_write16(screen + 0x16u, ROM16(r2));
				mem_write16(screen + 0x18u, ROM16(r2 + 2u + OBJECT(player_car)->unk_60 * 2u));
				sub_007734();
				OBJECT(current_object)->unk_0C++;
				AS32(OBJECT(player_car)->unk_00) = current_object;
				sub_00FCBC();
				r11 = screen + 0x2000u;
				AS32(OBJECT(player_car)->unk_3C) = r11;
				r0_2 = 0x10u;
				OBJECT(player_car)->unk_42 = 0u;
				r1 = mem_read8(screen + 0x320u + mem_read8(screen + 1u));
				r2 = 0u;
				if ((mem_read32(screen + 0x90u) & 0x100u) == 0u)
				{
					r2 = mem_read8(mem_read32(screen + 0x308u) + 0x1Fu);
					r2 = (uint8_t)(r2 - 1u);
				}
				r0_2 += mem_read16(rom_0091CC[r2] + r1 * 2u);
				AS16(OBJECT(player_car)->unk_44) = r0_2;
				r0_2 = (uint16_t)(r0_2 * 0x20u);
				AS16(OBJECT(player_car)->unk_40) = r0_2;
				r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r0_2);
				r1 = mem_read32(rom_0090AC[r2] + r1 * 4u);
				AS32(OBJECT(player_car)->unk_38) = r1;
				AT16(OBJECT(current_object)->unk_8C, 2) = mem_read16(r11);
				r11 += 2u;
				AS16(OBJECT(current_object)->unk_8C) = mem_read16(r11);
				r11 += 2u;
				AS32(OBJECT(current_object)->unk_20) = r1 + mem_read32(r11);
				r11 += 4u;
				OBJECT(current_object)->unk_28 = mem_read32(r11);
				AS32(OBJECT(current_object)->unk_24) = mem_read32(r11 + 4u);
				OBJECT(current_object)->unk_3A = rom_009510[OBJECT(player_car)->unk_60];
				OBJECT(current_object)->unk_10 = 8u;
				OBJECT(current_object)->unk_11 = 0u;
				if ((mem_read32(screen + 0x90u) & 0x100u) == 0u)
				{
					OBJECT(current_object)->unk_10 = 0x95u;
					OBJECT(current_object)->unk_11 = 0u;
					mem_write16(player_car + 0x10Cu, 0xFA60u);
					mem_write16(player_car + 0x10Eu, 0xF0u);
				}
				sub_00A00C();
				sub_00A04A();
				sub_0077A4();
				sub_007734();
				sub_00FD22();
				mem_write32(screen + 0xD0u, screen + 0x600u);
				sub_010062();
				sub_007CF1();
				if (mem_read8(screen + 0x318u) != 0u)
					mem_write16(mem_read32(screen + 0x24u) + 0x6Au, ((int8_t)unk_20FC0B[0] + 0x28u) * 0x1Eu);
				sub_011F30();
				sub_00780E();
				sub_007551();
				return;
			case 0x007088u:
				t1 = AS16(OBJECT(current_object)->unk_10);
				AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
				if (t1 != 1u)
				{
					mem_write8(current_object + 0xD1u, 0xFFu);
					mem_write8(current_object + 0xD2u, 0x80u);
					sub_0070EC();
					return;
				}
				OBJECT(current_object)->unk_0C++;
				AT8(OBJECT(player_car)->unk_64, 2) = 0xFFu;
				if ((mem_read32(screen + 0x90u) & 0x100u) == 0u)
				{
					mem_write8(mem_read32(screen + 0x24u) + 0x5Du, 0xFFu);
					mem_write8(screen + 0x31Cu, 0xFFu);
				}
			case 0x0070B9u:
				if (L_007F31 || mem_read8(player_car + 0x180u) == 0u)
				{
					if (!L_007F31)
					{
						L_0071FB = mem_read8(player_car + 0x181u) != 0u;
						if (!L_0071FB)
						{
							if ((uint8_t)(OBJECT(current_object)->unk_6C >> 24u) == 0u)
								OBJECT(current_object)->unk_00 &= 0xF7u;
							else
							{
								t2 = (uint8_t)(OBJECT(current_object)->unk_6C >> 24u);
								AT8(OBJECT(current_object)->unk_6C, 3) = t2 - 1u;
								if (t2 == 1u)
									OBJECT(current_object)->unk_00 &= 0xF7u;
							}
							sub_01256F();
							if ((OBJECT(current_object)->unk_00 & 4u) == 0u)
							{
								OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
								sub_0070EC();
								return;
							}
							switch ((uint8_t)(OBJECT(current_object)->unk_6C >> 16u))
							{
							case 3:
								OBJECT(current_object)->unk_00 &= 0xFBu;
								mem_write8(player_car + 0x147u, 0xFu);
								OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
								sub_0070EC();
								return;
							case 0:
								r19 = mem_read32(player_car + 0x130u);
								r5 = mem_read32(current_object + 0xC0u);
								if (r5 >= 0xFFFFu)
									r5 = 0xFFFFu;
								r0 = 0u - r5;
								r5 = (r5 >> 7) + rom_008026[mem_read8(screen + 0x320u + mem_read8(screen + 1u))];
								if (r5 >= 0x1F0u)
									r5 = 0x1F0u;
								r19 += (r5 & ~0xFFFFu) | (r5 & 0x1F0u);
								mem_write32(current_object + 0xC4u, (int32_t)(r0 * mem_read16(r19 + 4u)) >> 8);
								r8 = OBJECT(player_car)->unk_38;
								r8 = r8 >> 15;
								mem_write16(current_object + 0xCEu, mem_read16(r19 + 6u + r8 * 2u));
								mem_write16(current_object + 0xCCu, mem_read16(r19 + 0xAu));
								break;
							case 1:
								r10 = (screen & ~0xFFFFu) | (uint16_t)OBJECT(current_object)->unk_68;
								r4 = OBJECT(current_object)->unk_2C;
								r5 = (uint32_t)mem_read16(r10 + 0x2Cu) << 8;
								r6 = mem_read16(r10 + 0x6Au);
								r0 = (int16_t)OBJECT(player_car)->unk_38 * 2u;
								r1 = ((int16_t)mem_read16(r10 + 0x20u) + (uint32_t)mem_read16((((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40))) + 0x12u) * (int8_t)mem_read8(r10 + 0x3Fu)) * 2u;
								r2 = (int16_t)OBJECT(current_object)->unk_2E * 2u;
								r3 = (int16_t)mem_read16(r10 + 0x2Eu) * 2u;
								L_007E41 = true;
								break;
							case 2:
								r10 = (uint16_t)OBJECT(current_object)->unk_68;
								r10 = 0x20FC00u + (uint16_t)(r10 << 7);
								r0 = (int16_t)OBJECT(player_car)->unk_38;
								r1 = (int16_t)mem_read16(r10 + 0x44u);
								r2 = (int16_t)OBJECT(current_object)->unk_2E;
								r3 = (int16_t)mem_read16(r10 + 0x4Eu);
								r4 = OBJECT(current_object)->unk_2C;
								r5 = mem_read16(r10 + 0x4Cu);
								r4 = r4 >> 1;
								r5 = r5 >> 1;
								r6 = rom_008038[mem_read8(r10 + 0x49u)];
								L_007E41 = true;
								break;
							default:
								invalid_jump(rom_007D28[(uint8_t)(OBJECT(current_object)->unk_6C >> 16u)]);
							}
							if (L_007E41)
							{
								L_007E41 = false;
								r7 = rom_008038[OBJECT(player_car)->unk_60];
								r19 = mem_read32(player_car + 0x130u);
								r5 -= r4;
								r4 = r5;
								if ((int32_t)r5 < 0)
									r5 = (0u - r5) * 2u;
								r5 = divide_s32((r5 >> 8) * r6, r7) + rom_008026[mem_read8(screen + 0x320u + mem_read8(screen + 1u))];
								if (r5 >= 0x1F0u)
									r5 = 0x1F0u;
								r19 += (r5 & ~0xFFFFu) | (r5 & 0x1F0u);
								mem_write32(current_object + 0xC4u, (int32_t)(((int32_t)(r4 * mem_read16(r19 + 0xEu)) >> 8) * r6) >> 8);
								r0 = r0 - r1 + ((r3 - r2) << 1);
								r8 = (0u - r0) >> 31;
								r0 = divide_s32(((int32_t)(r0 * (int16_t)mem_read16(r19 + 8u)) >> 7) * r6, r7);
								overflow = v60_movt_overflow(r0, 32, 16);
								mem_write16(current_object + 0xCEu, r0);
								mem_write16(current_object + 0xCCu, mem_read16(r19 + 0xAu));
							}
							OBJECT(current_object)->unk_0C = 5u;
							OBJECT(current_object)->unk_0D = 0u;
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->unk_00 |= 8u;
							mem_write8(player_car + 0x134u, mem_read8(r19 + 0xDu));
							mem_write8(player_car + 0x135u, 0xAu);
							r0_3 = (uint8_t)mem_read16(r19 + 0xCu);
							sub_011EB8(r0_3);
							r9 = mem_read16(r19 + r8 * 2u);
							r1 = sub_0078D3();
							r9 = (uint16_t)(r9 + r1);
							OBJECT(current_object)->unk_38 = r9;
							r9_2 = mem_read8(rom_034138[r9]);
							OBJECT(current_object)->unk_41 = r9_2;
							AS8(OBJECT(current_object)->unk_42) = r9_2;
							OBJECT(current_object)->unk_40 = 0u;
							mem_write8(player_car + 0x136u, 0xFFu);
						}
					}
					if (!L_0071FB)
					{
						if (mem_read8(player_car + 0x181u) != 0u)
						{
							OBJECT(current_object)->unk_0C = 2u;
							OBJECT(current_object)->unk_0D = 0u;
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->unk_00 &= 0xFBu;
							AT8(OBJECT(current_object)->unk_6C, 3) = 0xFu;
							mem_write16(current_object + 0xCEu, 0u);
							mem_write16(current_object + 0xCCu, 0u);
							mem_write32(current_object + 0xC8u, 0u);
							mem_write32(current_object + 0xC4u, 0u);
							mem_write8(current_object + 0xD0u, 0u);
							OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
							sub_0070EC();
							return;
						}
						unk_20E0D0[mem_read8(screen)] = 0x10u;
						if (mem_read8(player_car + 0x135u) != 0u)
						{
							t3 = mem_read8(player_car + 0x135u);
							mem_write8(player_car + 0x135u, t3 - 1u);
							if (t3 == 1u)
							{
								r0 = mem_read8(player_car + 0x134u);
								if (r0 != 0u)
									sub_011EB8((uint8_t)r0);
							}
						}
						if (OBJECT(current_object)->unk_41 == 0u)
						{
							OBJECT(current_object)->unk_0C = 2u;
							OBJECT(current_object)->unk_0D = 0u;
							OBJECT(current_object)->unk_0E = 0u;
							OBJECT(current_object)->unk_0F = 0u;
							OBJECT(current_object)->unk_00 &= 0xFBu;
							AT8(OBJECT(current_object)->unk_6C, 3) = 0xFu;
							mem_write16(current_object + 0xCEu, 0u);
							mem_write16(current_object + 0xCCu, 0u);
							mem_write32(current_object + 0xC8u, 0u);
							mem_write32(current_object + 0xC4u, 0u);
							mem_write8(current_object + 0xD0u, 0u);
							OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
							sub_0070EC();
							return;
						}
						mem_write8(current_object + 0xD0u, 0u);
						mem_write8(current_object + 0xD1u, 0u);
						mem_write8(current_object + 0xD2u, 0x80u);
						mem_write16(current_object + 0xCEu, (int32_t)((uint32_t)(int16_t)mem_read16(current_object + 0xCEu) * mem_read16(player_car + 0x128u)) / 0x100);
						mem_write32(current_object + 0xC4u, (int32_t)(mem_read32(current_object + 0xC4u) * mem_read16(player_car + 0x12Cu)) / 0x100);
						r8 = mem_read32(current_object + 0xC8u);
						r6 = mem_read16(player_car + 0x12Au);
						r9 = (int16_t)mem_read16(current_object + 0xCCu) - r6;
						r7 = r8;
						r8 += r9;
						if ((int32_t)(r7 ^ r8) < 0)
						{
							r8 = 0u;
							r9 = (int32_t)((r9 + r6) * mem_read16(player_car + 0x126u)) / -0x100;
							if (r9 != 0u)
							{
								r0_3 = 0x31u;
								sub_011EB8(r0_3);
							}
						}
						mem_write16(current_object + 0xCCu, r9);
						mem_write32(current_object + 0xC8u, r8);
						OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
						sub_0070EC();
						return;
					}
				}
				if (L_0071FB)
					break;
				OBJECT(current_object)->unk_0C = 3u;
				OBJECT(current_object)->unk_0D = 0u;
				OBJECT(current_object)->unk_0E = 0u;
				OBJECT(current_object)->unk_0F = 0u;
			case 0x007134u:
				if (!L_0071FB)
				{
					if (mem_read8(player_car + 0x181u) == 0u)
					{
						mem_write8(current_object + 0xD0u, 0u);
						r0 = OBJECT(player_car)->unk_60;
						r0 = (uint8_t)(r0 * 2u);
						mem_write8(current_object + 0xD1u, rom_009530[(uint8_t)(r0 + OBJECT(player_car)->unk_61)]);
						switch (OBJECT(current_object)->unk_0D)
						{
						case 0:
							OBJECT(current_object)->unk_0D++;
							OBJECT(current_object)->unk_10 = 0xB4u;
							OBJECT(current_object)->unk_11 = 0u;
							sub_0077DF();
						case 1:
							t4 = AS16(OBJECT(current_object)->unk_10);
							AS16(OBJECT(current_object)->unk_10) = t4 - 1u;
							if (t4 != 1u)
							{
								if (OBJECT(player_car)->unk_74 != 0u)
								{
									OBJECT(player_car)->unk_04 = mem_read32(mem_read32(screen + 0x308u) + 0x6Cu);
									sub_0070EC();
									return;
								}
								sub_0070EC();
								r9 = OBJECT(player_car)->unk_60;
								sub_007829(r9);
								return;
							}
							OBJECT(current_object)->unk_0D++;
							OBJECT(current_object)->unk_10 = 0x3Cu;
							OBJECT(current_object)->unk_11 = 0u;
							mem_write8(player_car + 0x182u, 0xFFu);
							if (mem_read8(mem_read32(screen + 0x308u) + 0x1Fu) <= 1u)
							{
								r21 = sub_011D43();
								mem_write32(r21 + 4u, sub_004771_ADDRESS);
							}
							else
							{
								r21 = sub_011D43();
								mem_write32(r21 + 4u, sub_00C9ED_ADDRESS);
							}
							t5 = AS16(OBJECT(current_object)->unk_10);
							AS16(OBJECT(current_object)->unk_10) = t5 - 1u;
							if (t5 == 1u)
							{
								OBJECT(current_object)->unk_0D++;
								mem_write16(screen + 0x94u, mem_read16(screen + 0x94u) | 0x10u);
								r0_3 = 0x37u;
								if ((int8_t)mem_read8(screen + 0xA4u) <= 3)
									r0_3 = 0x36u;
								sub_019402(r0_3);
							}
							break;
						case 2:
							t5 = AS16(OBJECT(current_object)->unk_10);
							AS16(OBJECT(current_object)->unk_10) = t5 - 1u;
							if (t5 != 1u)
							{
								sub_0070EC();
								r9 = OBJECT(player_car)->unk_60;
								sub_007829(r9);
								return;
							}
							OBJECT(current_object)->unk_0D++;
							mem_write16(screen + 0x94u, mem_read16(screen + 0x94u) | 0x10u);
							r0_3 = 0x37u;
							if ((int8_t)mem_read8(screen + 0xA4u) <= 3)
								r0_3 = 0x36u;
							sub_019402(r0_3);
							sub_0070EC();
							r9 = OBJECT(player_car)->unk_60;
							sub_007829(r9);
							return;
						case 3:
							break;
						default:
							invalid_jump(rom_007164[OBJECT(current_object)->unk_0D]);
						}
						sub_0070EC();
						r9 = OBJECT(player_car)->unk_60;
						sub_007829(r9);
						return;
					}
				}
				OBJECT(current_object)->unk_0C = 4u;
				OBJECT(current_object)->unk_0D = 0u;
				OBJECT(current_object)->unk_0E = 0u;
				OBJECT(current_object)->unk_0F = 0u;
			case 0x007200u:
				switch (OBJECT(current_object)->unk_0D)
				{
				case 0:
					OBJECT(current_object)->unk_0D++;
					mem_write8(current_object + 0xF8u, 0xFFu);
					t6 = mem_read32(screen + 0x24u) + 0x60u;
					mem_write8(t6, mem_read8(t6) + 1u);
					mem_write16(current_object + 0xF0u, AS16(OBJECT(player_car)->unk_44));
					sub_0077DF();
					OBJECT(current_object)->unk_00 |= 8u;
					sub_0070EC();
					return;
				case 1:
					r0 = mem_read16(screen + 0x410u + mem_read8(screen + 1u) * 2u);
					if (r0 == 0u)
					{
						sub_0070EC();
						return;
					}
					mem_write16(mem_read32(screen + 0x24u) + 0x16u, r0);
					OBJECT(player_car)->unk_04 = r0 * 0xAu;
					mem_write8(mem_read32(screen + 0x24u) + 0x5Eu, 0xFFu);
					OBJECT(current_object)->unk_0D++;
					r0 = rom_009540[mem_read8(screen + 0x320u + mem_read8(screen + 1u))];
					mem_write16(current_object + 0xF0u, mem_read16(current_object + 0xF0u) + mem_read16(r0));
					r0 += 2u;
					mem_write16(current_object + 0xF2u, mem_read16(r0));
					mem_write16(current_object + 0xF4u, mem_read16(r0 + 2u));
					mem_write8(current_object + 0xF6u, mem_read8(current_object + 0xD2u));
					mem_write8(current_object + 0xF7u, mem_read8(current_object + 0xD0u));
				case 2:
					if (AS16(OBJECT(player_car)->unk_44) < mem_read16(current_object + 0xF0u))
					{
						sub_007396();
						sub_0070EC();
						return;
					}
					OBJECT(current_object)->unk_0D++;
					mem_write16(current_object + 0xF4u, 0u);
					r9 = rom_009520[OBJECT(player_car)->unk_60];
					sub_00F366(r9);
				case 3:
					mem_write8(current_object + 0xD0u, 0x40u);
					mem_write8(current_object + 0xD1u, 0u);
					mem_write8(current_object + 0xD2u, 0x80u);
					sub_007396();
					r0_3 = rom_00ABC3[OBJECT(player_car)->unk_60];
					sub_011FC9(r0_3);
					if (OBJECT(player_car)->unk_74 != 0u)
					{
						r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
						sub_00A130(&r11, &r13, &r15, &r17);
						mem_write8(player_car + 0x141u, 2u);
						mem_write8(player_car + 0x142u, 2u);
						mem_write8(player_car + 0x140u, 3u);
						OBJECT(player_car)->unk_7E = 2u;
						sub_00A632(r11, r13, r15, r17);
						sub_007908();
						sub_007734();
						sub_007551();
						sub_00FD22();
						sub_010062();
						sub_011F7C();
						ignored = sub_00F30D(0u);
						return;
					}
					OBJECT(current_object)->unk_0D++;
					OBJECT(current_object)->unk_10 = 0x5Au;
					OBJECT(current_object)->unk_11 = 0u;
					mem_write8(current_object + 0xD0u, 0u);
					mem_write8(current_object + 0xD1u, 0x80u);
					mem_write8(current_object + 0xD2u, 0x80u);
					t7 = AS16(OBJECT(current_object)->unk_10);
					AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
					if (t7 == 1u)
					{
						OBJECT(current_object)->unk_0D++;
						mem_write16(screen + 0x94u, mem_read16(screen + 0x94u) | 0x10u);
					}
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
					sub_00A130(&r11, &r13, &r15, &r17);
					sub_00A632(r11, r13, r15, r17);
					sub_007908();
					sub_007734();
					sub_007551();
					sub_00FD22();
					sub_010062();
					sub_011F7C();
					ignored = sub_00F30D(0u);
					return;
				case 4:
					mem_write8(current_object + 0xD0u, 0u);
					mem_write8(current_object + 0xD1u, 0x80u);
					mem_write8(current_object + 0xD2u, 0x80u);
					t7 = AS16(OBJECT(current_object)->unk_10);
					AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
					if (t7 == 1u)
					{
						OBJECT(current_object)->unk_0D++;
						mem_write16(screen + 0x94u, mem_read16(screen + 0x94u) | 0x10u);
					}
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
					sub_00A130(&r11, &r13, &r15, &r17);
					sub_00A632(r11, r13, r15, r17);
					sub_007908();
					sub_007734();
					sub_007551();
					sub_00FD22();
					sub_010062();
					sub_011F7C();
					ignored = sub_00F30D(0u);
					return;
				case 5:
					mem_write8(current_object + 0xD0u, 0u);
					mem_write8(current_object + 0xD1u, 0x80u);
					mem_write8(current_object + 0xD2u, 0x80u);
					r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
					sub_00A130(&r11, &r13, &r15, &r17);
					sub_00A632(r11, r13, r15, r17);
					sub_007908();
					sub_007734();
					sub_007551();
					sub_00FD22();
					sub_010062();
					sub_011F7C();
					ignored = sub_00F30D(0u);
					return;
				default:
					invalid_jump(rom_00720C[OBJECT(current_object)->unk_0D]);
				}
				break;
			case 0x007F31u:
				L_007F31 = true;
				break;
			default:
				invalid_jump(target1);
			}
		} while (L_0071FB);
	} while (L_007F31);
}

void sub_0070EC(void)
{
	uint32_t r11 = 0, r13 = 0, r15 = 0, r17 = 0;

	r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
	sub_00A130(&r11, &r13, &r15, &r17);
	sub_00A632(r11, r13, r15, r17);
	if (OBJECT(current_object)->unk_0C == 5u)
		mem_write8(player_car + 0x140u, 0u);
	sub_007908();
	sub_007734();
	sub_007551();
	sub_00FD22();
	sub_010062();
	sub_011F7C();
	sub_00780E();
}

void sub_007396(void)
{
	uint32_t negative = 0, overflow = 0;
	uint8_t r0_2 = 0;
	uint16_t r1_2 = 0, r2 = 0;
	uint32_t r0 = 0, r1 = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	mem_write8(current_object + 0xD1u, 0u);
	mem_write8(current_object + 0xD0u, 0x40u);
	r0 = OBJECT(player_car)->unk_74;
	t1 = mem_read16(current_object + 0xF4u);
	r1 = t1 - r0;
	if (t1 != r0)
		r0 += (int32_t)(r1 * 1u) / 0x10 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
	OBJECT(player_car)->unk_74 = r0;
	r0_2 = mem_read8(current_object + 0xF6u);
	r1_2 = mem_read16(current_object + 0xF2u);
	t2 = OBJECT(player_car)->unk_38;
	t3 = t2 - r1_2;
	overflow = (uint32_t)(int16_t)t2 - (int16_t)r1_2 != (uint32_t)(int16_t)(t2 - r1_2);
	negative = t3 >> 15;
	r2 = t3;
	if (t2 == r1_2)
	{
		mem_write8(current_object + 0xF6u, 0x80u);
		mem_write8(current_object + 0xD2u, 0x80u);
		return;
	}
	if (negative ^ overflow)
	{
		if ((int16_t)r2 >= -8)
		{
			mem_write8(current_object + 0xF6u, 0x80u);
			mem_write8(current_object + 0xD2u, 0x80u);
			return;
		}
		r0_2--;
		if ((int8_t)r0_2 < 0x10)
			r0_2 = 0x10u;
		if ((int8_t)r0_2 > 0x6F)
			r0_2 = 0x6Fu;
		mem_write8(current_object + 0xF6u, r0_2);
		mem_write8(current_object + 0xD2u, r0_2);
		return;
	}
	if ((int16_t)r2 <= 8)
	{
		mem_write8(current_object + 0xF6u, 0x80u);
		mem_write8(current_object + 0xD2u, 0x80u);
		return;
	}
	r0_2++;
	if ((int8_t)r0_2 > -0x31)
		r0_2 = 0xCFu;
	if ((int8_t)r0_2 < -0x70)
		r0_2 = 0x90u;
	mem_write8(current_object + 0xF6u, r0_2);
	mem_write8(current_object + 0xD2u, r0_2);
}

void sub_007443(void)
{
	uint32_t zero = 0, carry = 0;
	uint8_t r0_2 = 0;
	uint16_t r3_2 = 0;
	uint32_t r0 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0;
	uint8_t t1 = 0;

	if ((mem_read32(screen + 0x90u) & 0x100u) != 0u)
	{
		r7 = mem_read8(screen + 1u);
		r6 = r7;
		r5 = r7;
		r3 = r7;
		mem_write8(screen + 0x70u, r7);
		mem_write8(screen + 0x71u, r6);
		mem_write8(screen + 0x72u, r5);
		mem_write8(screen + 0x73u, r3);
		r7 = rom_00E0A0[r7] + unk_20E000_ADDRESS;
		mem_write32(screen + 0x60u, r7);
		mem_write32(screen + 0x64u, rom_00E0A0[r6] + unk_20E000_ADDRESS);
		mem_write32(screen + 0x68u, rom_00E0A0[r5] + unk_20E000_ADDRESS);
		mem_write32(screen + 0x6Cu, rom_00E0A0[r3] + unk_20E000_ADDRESS);
		mem_write16(screen + 0x350u, mem_read16(r7 + 0x46u));
		mem_write8(screen + 0x352u, mem_read8(r7 + 0x62u));
		return;
	}
	r10 = mem_read32(screen + 0x308u) + 0x1Fu;
	r9 = mem_read8(r10);
	r9 = (uint8_t)(r9 - 1u);
	r10++;
	r6 = mem_read8(r10 + r9);
	r7 = mem_read8(r10);
	r4 = mem_read8(screen + 0x320u + mem_read8(screen + 1u));
	if (r4 == 0u)
	{
		r0 = unk_20E000_ADDRESS + mem_read8(screen) * 4u;
		if (mem_read8(r0 + 0xC9u) == 0u)
		{
			mem_write8(r0 + 0xC9u, 2u);
			mem_write8(r0 + 0xCAu, 0x10u);
			mem_write8(r0 + 0xCBu, 0x10u);
		}
	}
	t1 = mem_read8(current_object + 0xDAu);
	carry = r4 < t1;
	zero = (uint8_t)(r4 - t1) == 0u;
	if (r4 != t1)
	{
		mem_write8(current_object + 0xDAu, r4);
		r3 = mem_read32(screen + 0x6Cu);
		if (carry | zero)
			r3 = mem_read32(screen + 0x68u);
		r3_2 = (uint16_t)(mem_read16(r3 + 0x44u) - mem_read16(mem_read32(screen + 0x20u) + 0x44u));
		r0_2 = 0x3Au;
		if ((int16_t)r3_2 < 0x160 && (int16_t)r3_2 > -0x160)
			r0_2 = 0x37u;
		if (OBJECT(current_object)->unk_0C != 0u)
			sub_011EB8(r0_2);
	}
	r3 = r4;
	if (r4 != 0u)
		r4 = (uint8_t)(r4 - 1u);
	if (r3 != r9)
		r3 = (uint8_t)(r3 + 1u);
	r5 = mem_read8(r10 + r4);
	r3 = mem_read8(r10 + r3);
	mem_write8(screen + 0x70u, r7);
	mem_write8(screen + 0x71u, r6);
	mem_write8(screen + 0x72u, r5);
	mem_write8(screen + 0x73u, r3);
	r7 = rom_00E0A0[r7] + unk_20E000_ADDRESS;
	mem_write32(screen + 0x60u, r7);
	mem_write32(screen + 0x64u, rom_00E0A0[r6] + unk_20E000_ADDRESS);
	mem_write32(screen + 0x68u, rom_00E0A0[r5] + unk_20E000_ADDRESS);
	mem_write32(screen + 0x6Cu, rom_00E0A0[r3] + unk_20E000_ADDRESS);
	mem_write16(screen + 0x350u, mem_read16(r7 + 0x46u));
	mem_write8(screen + 0x352u, mem_read8(r7 + 0x62u));
}

void sub_007551(void)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0, r13 = 0;

	r10 = mem_read32(screen + 0x24u);
	r11 = mem_read32(screen + 0x20u);
	if (unk_20E623 == 0u)
	{
		r0 = mem_read8(screen + 0xD4u);
		r0 = (uint8_t)(r0 + 1u);
		r0 = r0 & 7u;
		mem_write8(screen + 0xD4u, r0);
		mem_write8(r10 + 0x4Bu, r0);
		r12 = screen + 0x600u + (uint16_t)(r0 << 5);
		r0 = mem_read8(r11 + 0x4Bu);
		r13 = screen + 0x600u + (uint16_t)(r0 << 5);
		mem_write32(screen + 0xD0u, r13);
	}
	else
	{
		r12 = screen + 0x600u;
		r13 = r12;
		mem_write32(screen + 0xD0u, r13);
	}
	mem_write32(r12, AS32(OBJECT(player_car)->unk_20));
	AS32(OBJECT(player_car)->unk_20) = mem_read32(r13);
	mem_write32(r12 + 4u, AS32(OBJECT(player_car)->unk_24));
	AS32(OBJECT(player_car)->unk_24) = mem_read32(r13 + 4u);
	mem_write32(r12 + 8u, OBJECT(player_car)->unk_28);
	OBJECT(player_car)->unk_28 = mem_read32(r13 + 8u);
	mem_write16(r12 + 0xCu, OBJECT(player_car)->unk_2C);
	OBJECT(player_car)->unk_2C = mem_read16(r13 + 0xCu);
	mem_write16(r12 + 0xEu, OBJECT(player_car)->unk_2E);
	OBJECT(player_car)->unk_2E = mem_read16(r13 + 0xEu);
	mem_write16(r12 + 0x10u, mem_read16(player_car + 0xA4u));
	mem_write16(r12 + 0x14u, OBJECT(player_car)->unk_3A);
	mem_write16(r12 + 0x12u, mem_read16(screen + 0xB8u));
	mem_write16(screen + 0xB8u, mem_read16(r13 + 0x12u));
	if (unk_20E623 == 0u)
	{
		mem_write32(r10 + 0x30u, AS32(OBJECT(current_object)->unk_20));
		mem_write32(r10 + 0x34u, AS32(OBJECT(current_object)->unk_24));
		mem_write32(r10 + 0x38u, OBJECT(current_object)->unk_28);
		mem_write16(r10 + 0x3Cu, OBJECT(current_object)->unk_38);
		mem_write16(r10 + 0x3Eu, OBJECT(current_object)->unk_3A);
		mem_write8(r10 + 0x4Au, OBJECT(current_object)->unk_40);
		mem_write8(r10 + 0x50u, mem_read8(player_car + 0x140u));
		mem_write8(r10 + 0x53u, mem_read8(player_car + 0x145u));
		mem_write8(r10 + 0x51u, mem_read8(player_car + 0x141u));
		mem_write8(r10 + 0x52u, mem_read8(player_car + 0x142u));
		mem_write16(r10 + 0x4Cu, OBJECT(current_object)->unk_2C);
		mem_write16(r10 + 0x4Eu, OBJECT(current_object)->unk_2E);
		mem_write16(r10 + 0x58u, AS16(OBJECT(current_object)->unk_00));
		mem_write16(r10 + 0x40u, OBJECT(player_car)->unk_32);
		mem_write16(r10 + 0x42u, AS16(OBJECT(player_car)->unk_44));
		mem_write16(r10 + 0x44u, OBJECT(player_car)->unk_38);
		mem_write16(r10 + 0x46u, OBJECT(player_car)->unk_42);
		mem_write8(r10 + 0x62u, mem_read8(screen + 0xA3u));
		mem_write16(r10 + 0x56u, mem_read16(current_object + 0xC8u));
		mem_write8(r10 + 0x5Fu, 0xFFu);
	}
}

void sub_007677(void)
{
	uint32_t r0 = 0, r10 = 0;

	r10 = mem_read32(screen + 0x24u);
	mem_write8(r10 + 0x5Fu, 0u);
	r0 = mem_read8(screen + 0xD4u);
	r0 = (uint8_t)(r0 + 1u);
	r0 = r0 & 7u;
	mem_write8(screen + 0xD4u, r0);
	mem_write8(r10 + 0x4Bu, r0);
	r12 = screen + 0x600u + (uint16_t)(r0 << 5);
	mem_write32(screen + 0xD0u, r12);
	OBJECT(player_car)->unk_20 = 0u;
	OBJECT(player_car)->unk_22 = 0u;
	OBJECT(player_car)->unk_23 = 0u;
	mem_write32(r12, 0u);
	AS32(OBJECT(player_car)->unk_24) = 0u;
	mem_write32(r12 + 4u, 0u);
	OBJECT(player_car)->unk_28 = 0u;
	mem_write32(r12 + 8u, 0u);
	OBJECT(player_car)->unk_2C = 0u;
	mem_write16(r12 + 0xCu, 0u);
	OBJECT(player_car)->unk_2E = 0u;
	mem_write16(r12 + 0xEu, 0u);
	mem_write8(player_car + 0x140u, 0u);
	mem_write8(r10 + 0x50u, 0u);
	mem_write8(player_car + 0x145u, 0u);
	mem_write8(r10 + 0x53u, 0u);
	mem_write8(player_car + 0x141u, 0u);
	mem_write8(r10 + 0x51u, 0u);
	mem_write8(player_car + 0x142u, 0u);
	mem_write8(r10 + 0x52u, 0u);
	OBJECT(player_car)->unk_32 = 0u;
	mem_write16(r10 + 0x40u, 0u);
	OBJECT(player_car)->unk_44 = 0u;
	OBJECT(player_car)->unk_45 = 0u;
	mem_write16(r10 + 0x42u, 0u);
	OBJECT(player_car)->unk_38 = 0u;
	mem_write16(r10 + 0x44u, 0u);
	OBJECT(player_car)->unk_42 = 0u;
	mem_write16(r10 + 0x46u, 0u);
	mem_write16(player_car + 0xA4u, 0u);
	mem_write16(r12 + 0x10u, 0u);
	OBJECT(player_car)->unk_3A = 0u;
	mem_write16(r12 + 0x14u, 0u);
	mem_write16(screen + 0xB8u, 0u);
	mem_write16(r12 + 0x12u, 0u);
	mem_write16(current_object + 0xC8u, 0u);
	mem_write16(r10 + 0x56u, 0u);
}

void sub_007734(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0;

	r0 = (uint16_t)(mem_read16(player_car + 0x102u) + mem_read16(player_car + 0x112u) + mem_read16(player_car + 0x10Au) + mem_read16(screen + 0x18u));
	if ((int16_t)r0 > 0x200)
		r0 = 0x200u;
	if ((int16_t)r0 < 0xC0)
		r0 = 0xC0u;
	r0 = (uint16_t)(r0 + mem_read16(player_car + 0x10Eu)) & 0xFFFu;
	r1 = mem_read16(screen + 0x10u);
	r2 = mem_read16(screen + 0x12u);
	mem_write16(screen + 0xB8u, divide_s32(divide_s32((int16_t)rom_100C00[r0] * r2, (uint32_t)(int16_t)rom_101400[r0]) * r1, r2 + r1) - mem_read16(screen + 0x16u) + 0x100u);
}

void sub_0077A4(void)
{
	uint32_t r11 = 0, r13 = 0, r15 = 0, r17 = 0;

	OBJECT(player_car)->unk_32 = 0u;
	OBJECT(player_car)->unk_38 = OBJECT(current_object)->unk_20;
	r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + AS16(OBJECT(player_car)->unk_40));
	r13 = OBJECT(player_car)->unk_38;
	r17 = 0x80u;
	r15 = OBJECT(player_car)->unk_32;
	r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
	sub_00A537(r11, r13, r15, r17);
	sub_00A632(r11, r13, r15, r17);
}

void sub_0077DF(void)
{
	mem_write16(current_object + 0xCEu, 0u);
	mem_write32(current_object + 0xC4u, 0u);
	mem_write32(current_object + 0xC8u, 0u);
	mem_write16(current_object + 0xCCu, 0u);
	mem_write16(current_object + 0xCEu, 0u);
	OBJECT(current_object)->unk_00 &= 0xFBu;
	AT8(OBJECT(player_car)->unk_64, 3) = 0u;
	AT8(OBJECT(player_car)->unk_64, 1) = 0u;
}

void sub_00780E(void)
{
	uint8_t r8 = 0, r9_2 = 0;
	uint32_t r1 = 0, r9 = 0, ignored = 0;

	r9 = OBJECT(player_car)->unk_60;
	if ((OBJECT(current_object)->unk_00 & 4u) != 0u)
	{
		ignored = sub_00F30D(0u);
		return;
	}
	if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) != 0u)
	{
		r8 = (uint8_t)mem_read16(current_object + 0xB8u);
		r9 = (uint16_t)(r9 << 4);
		if ((int8_t)mem_read8(current_object + 0xD2u) >= 0)
		{
			if ((int8_t)r8 > 0)
				r8 = (uint8_t)(0u - r8);
			r9 = rom_009410[(r9 & ~0xFFu) | (uint8_t)(r9 + (0u - r8))];
		}
		else
		{
			if ((int8_t)r8 < 0)
				r8 = (uint8_t)(0u - r8);
			r9 = rom_009310[(r9 & ~0xFFu) | (uint8_t)(r9 + r8)];
		}
		r1 = sub_0078D3();
		r9 = (uint16_t)(r9 + r1);
		if (OBJECT(current_object)->unk_38 != r9)
		{
			OBJECT(current_object)->unk_38 = r9;
			r9_2 = mem_read8(rom_034138[r9]);
			OBJECT(current_object)->unk_41 = r9_2;
			AS8(OBJECT(current_object)->unk_42) = r9_2;
			OBJECT(current_object)->unk_40 = 0u;
		}
		ignored = sub_00F30D(0u);
		return;
	}
	if ((int16_t)(uint16_t)OBJECT(current_object)->unk_94 <= 0 || (uint8_t)(OBJECT(player_car)->unk_64 >> 8u) == 0u)
	{
		sub_007829(r9);
		return;
	}
	r9 = rom_009300[r9];
	r1 = sub_0078D3();
	r9 = (uint16_t)(r9 + r1);
	if (OBJECT(current_object)->unk_38 != r9)
	{
		OBJECT(current_object)->unk_38 = r9;
		r9_2 = mem_read8(rom_034138[r9]);
		OBJECT(current_object)->unk_41 = r9_2;
		AS8(OBJECT(current_object)->unk_42) = r9_2;
		OBJECT(current_object)->unk_40 = 0u;
	}
	ignored = sub_00F30D(0u);
}

void sub_007829(uint32_t r9)
{
	uint8_t r0_2 = 0, r1_2 = 0;
	uint16_t r0 = 0;
	uint32_t r1 = 0;

	r0 = rom_00926C[r9];
	r1 = sub_0078D3();
	OBJECT(current_object)->unk_38 = r0 + r1;
	r0_2 = mem_read8(0x92BCu + (int16_t)(uint16_t)(OBJECT(current_object)->unk_94 >> 16u));
	r1_2 = mem_read8(current_object + 0xDBu);
	if (r0_2 == r1_2)
	{
		OBJECT(current_object)->unk_40 = r1_2;
		mem_write8(current_object + 0xDBu, r1_2);
		return;
	}
	if (r0_2 < r1_2)
		r1_2--;
	else
		r1_2++;
	OBJECT(current_object)->unk_40 = r1_2;
	mem_write8(current_object + 0xDBu, r1_2);
}

uint32_t sub_0078D3(void)
{
	uint16_t r3 = 0, r4 = 0;
	uint32_t r1 = 0;
	uint16_t t1 = 0;

	r1 = 0u;
	r3 = 5u;
	t1 = ((OBJECT(player_car)->unk_2C - 0xB0u) << 4) - ((uint32_t)mem_read16(mem_read32(screen + 0x20u) + 0x54u) << 4);
	r4 = t1;
	if ((int16_t)t1 >= 0)
	{
		r1 = r4 / 0x300u;
		if (r3 <= r1)
			r1 = r3;
	}
	mem_write8(current_object + 0xD9u, r1);
	return r1;
}

void sub_007908(void)
{
	bool L_007ACB = false, L_007AD1 = false, L_007AFA = false;
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint32_t r0_2 = 0, r21 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t7 = 0, t9 = 0;
	uint32_t target1 = 0, t6 = 0, t8 = 0;

	do
	{
		do
		{
			target1 = L_007AD1 ? 0x792Bu : L_007AFA ? 0x792Bu : rom_007917[mem_read8(current_object + 0xE0u)];
			switch (target1)
			{
			case 0x007927u:
				mem_write8(current_object + 0xE0u, mem_read8(current_object + 0xE0u) + 1u);
			case 0x00792Bu:
				if (L_007AD1 || L_007AFA || mem_read8(current_object + 0xE1u) != 0u)
				{
					do
					{
						if (L_007ACB || L_007AD1 || !L_007AFA)
						{
							if (L_007ACB || !L_007AD1)
							{
								if (!L_007ACB)
								{
									r21 = sub_011D43();
									mem_write32(r21 + 4u, sub_007C36_ADDRESS);
								}
								L_007ACB = false;
								mem_write8(current_object + 0xE0u, 2u);
							}
							L_007AD1 = false;
							sub_007B28();
						}
						if (L_007AFA || mem_read8(current_object + 0xE1u) == 0u)
						{
							if (!L_007AFA)
								mem_write8(current_object + 0xE0u, 3u);
							L_007AFA = false;
							sub_007B28();
							L_007ACB = mem_read8(current_object + 0xE1u) != 0u;
							if (!L_007ACB)
							{
								r0_2 = mem_read16(player_car + 0x110u);
								if (r0_2 == 0u)
									mem_write8(current_object + 0xE0u, 1u);
								else
								{
									r0_2 = (uint16_t)(r0_2 + 0x10u);
									mem_write16(player_car + 0x112u, mem_read16(player_car + 0x112u) - 8u);
									mem_write16(player_car + 0x110u, r0_2);
								}
							}
						}
						else
						{
							r0_2 = mem_read16(player_car + 0x110u);
							if (r0_2 != 0xFF00u)
							{
								r0_2 = (uint16_t)(r0_2 - 0x10u);
								mem_write16(player_car + 0x112u, mem_read16(player_car + 0x112u) + 8u);
								mem_write16(player_car + 0x110u, r0_2);
							}
						}
					} while (L_007ACB);
				}
				break;
			case 0x007AD1u:
				L_007AD1 = true;
				break;
			case 0x007AFAu:
				L_007AFA = true;
				break;
			default:
				invalid_jump(target1);
			}
		} while (L_007AD1);
	} while (L_007AFA);
	mem_write8(player_car + 0x145u, 0u);
	r0 = (uint16_t)(0u - OBJECT(current_object)->unk_2C);
	r1 = mem_read16(current_object + 0xB2u);
	mem_write16(current_object + 0xB2u, r0);
	r0 -= r1;
	r1 = mem_read16(current_object + 0xE2u);
	if ((OBJECT(current_object)->unk_00 & 4u) != 0u)
		r0 = 0u;
	else if ((int16_t)r0 < 0)
	{
		if ((uint8_t)(OBJECT(player_car)->unk_64 >> 8u) != 0u || (int16_t)r0 >= -0xA0)
			r0 = 0u;
		else
		{
			r0 = (uint16_t)(r0 * 1u);
			mem_write8(player_car + 0x145u, 0xFFu);
		}
	}
	else if (mem_read16(current_object + 0xB0u) == 0u)
		r0 = 0u;
	else
	{
		t1 = r0;
		r0 = (uint16_t)(t1 - 0x120u);
		if ((int16_t)t1 <= 0x120)
			r0 = 0u;
		else
			r0 = (uint16_t)(r0 * 1u);
	}
	t2 = (int16_t)(r0 * 1u) / 5;
	r0 = (uint16_t)(t2 - r1);
	if (t2 != r1)
	{
		t3 = (int16_t)r0 >> 3;
		r0 = t3;
		if (t3 == 0u)
			r0++;
	}
	if ((int16_t)r0 < -0x20)
		r0 = 0xFFE0u;
	if ((int16_t)r0 > 6)
		r0 = 6u;
	r1 += r0;
	if ((int16_t)r1 > 0x18)
		r1 = 0x18u;
	if ((int16_t)r1 < -0x100)
		r1 = 0xFF00u;
	mem_write16(current_object + 0xE2u, r1);
	mem_write16(player_car + 0x100u, r1);
	if ((int16_t)r1 >= 0)
		r1 = (uint16_t)((int16_t)(r1 * 1u) / 1);
	else
		r1 = (uint16_t)((int16_t)(r1 * 1u) / 1);
	mem_write16(player_car + 0x102u, r1);
	r0 = (uint16_t)((0u - OBJECT(current_object)->unk_2E) << 1);
	r1 = mem_read16(player_car + 0x106u);
	if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
	{
		if (r1 != 0u)
		{
			if ((int16_t)r1 >= 0)
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			else
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if ((int16_t)r0 < 0)
	{
		t4 = r0;
		r2 = (uint16_t)(t4 - r1);
		if (t4 != r1)
		{
			r1 += ((int16_t)r2 >> 2) + 1u;
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if ((int16_t)r0 > 0)
	{
		t5 = r0;
		r2 = (uint16_t)(t5 - r1);
		if (t5 != r1)
		{
			r1 += (int16_t)r2 >> 2;
			if ((int16_t)r1 > 0x40)
				r1 = 0x40u;
		}
	}
	else if (r1 != 0u)
	{
		if ((int16_t)r1 >= 0)
			r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		else
			r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		if ((int16_t)r1 > 0x40)
			r1 = 0x40u;
	}
	r1 = (uint16_t)(int16_t)mem_read16(player_car + 0x104u);
	t6 = AS32(OBJECT(current_object)->unk_24) - mem_read32(current_object + 0xB4u);
	r0_2 = t6 + 0xCu;
	if ((int32_t)t6 < -0xC)
	{
		t7 = r0_2;
		r2 = (uint16_t)(t7 - r1);
		if (t7 == (uint16_t)r1)
			return;
		r1 += ((int16_t)r2 >> 2) + 1u;
	}
	else
	{
		t8 = r0_2;
		r0 = (uint16_t)(t8 - 0xCu);
		if ((int32_t)t8 > 0xC)
		{
			t9 = r0;
			r2 = (uint16_t)(t9 - r1);
			if (t9 == (uint16_t)r1)
				return;
			r1 += (int16_t)r2 >> 2;
		}
		else
		{
			if ((uint16_t)r1 == 0u)
				return;
			if ((int16_t)r1 >= 0)
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
			else
				r1 = (uint16_t)((int16_t)(r1 * 3u) / 4);
		}
	}
	if ((int16_t)r1 > 8)
		r1 = 8u;
	if ((int16_t)r1 < -8)
		r1 = 0xFFF8u;
	mem_write16(player_car + 0x104u, r1);
}

void sub_007B28(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r10 = 0, r11 = 0;

	r10 = mem_read32(screen + 0x20u);
	r11 = mem_read32(screen + 0x68u);
	r12 = mem_read32(screen + 0x6Cu);
	r0 = mem_read32(r10 + 0x40u);
	r1 = mem_read16(r10 + 0x4Cu);
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 << 9);
	if (r11 != r10)
	{
		r2 = mem_read32(r11 + 0x40u);
		r2 = ((r2 & ~0xFFFFu) | (uint16_t)(r2 << 9)) - r0;
		r3 = mem_read16(r11 + 0x4Cu) - r1;
		if (mem_read8(current_object + 0xE1u) == 0u)
		{
			if (r1 < 0x6000u)
			{
				if (mem_read8(current_object + 0xE4u) == 0u)
				{
					mem_write8(current_object + 0xE1u, 0u);
					return;
				}
				mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
				return;
			}
			if ((int32_t)r2 <= 0x18000 && (int32_t)r3 <= -1 && (int32_t)r3 >= -0x100)
			{
				mem_write8(current_object + 0xE4u, 0x1Eu);
				if (mem_read8(current_object + 0xE4u) != 0u)
					mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
				mem_write8(current_object + 0xE1u, 0xFFu);
				return;
			}
		}
		else
		{
			if (r1 < 0x5000u)
			{
				if (mem_read8(current_object + 0xE4u) == 0u)
				{
					mem_write8(current_object + 0xE1u, 0u);
					return;
				}
				mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
				return;
			}
			if ((int32_t)r2 <= 0x20000 && (int32_t)r3 <= -1 && (int32_t)r3 >= -0x200)
			{
				if (mem_read8(current_object + 0xE4u) != 0u)
					mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
				mem_write8(current_object + 0xE1u, 0xFFu);
				return;
			}
		}
	}
	if (r12 == r10)
	{
		if (mem_read8(current_object + 0xE4u) == 0u)
		{
			mem_write8(current_object + 0xE1u, 0u);
			return;
		}
		mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
		return;
	}
	r2 = mem_read32(r12 + 0x40u);
	r2 = ((r2 & ~0xFFFFu) | (uint16_t)(r2 << 9)) - r0;
	r3 = mem_read16(r12 + 0x4Cu) - r1;
	if (mem_read8(current_object + 0xE1u) == 0u)
	{
		if (r1 < 0x6000u || (int32_t)r2 < -0x30000 || (int32_t)r3 > 0x2000 || (int32_t)r3 < 0)
		{
			if (mem_read8(current_object + 0xE4u) == 0u)
			{
				mem_write8(current_object + 0xE1u, 0u);
				return;
			}
			mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
			return;
		}
		mem_write8(current_object + 0xE4u, 0x1Eu);
		if (mem_read8(current_object + 0xE4u) != 0u)
			mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
		mem_write8(current_object + 0xE1u, 0xFFu);
		return;
	}
	if (r1 < 0x5000u || (int32_t)r2 < -0x40000 || (int32_t)r3 > 0x3000 || (int32_t)r3 < -0x1000)
	{
		if (mem_read8(current_object + 0xE4u) == 0u)
		{
			mem_write8(current_object + 0xE1u, 0u);
			return;
		}
		mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
		return;
	}
	if (mem_read8(current_object + 0xE4u) != 0u)
		mem_write8(current_object + 0xE4u, mem_read8(current_object + 0xE4u) - 1u);
	mem_write8(current_object + 0xE1u, 0xFFu);
}

void sub_007C36(void)
{
	uint32_t r19 = 0, r21 = 0, ignored = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_12 = 2u;
		OBJECT(current_object)->unk_10 = 0xCu;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_007C9E_ADDRESS);
		break;
	case 1:
		break;
	case 2:
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(current_object)->unk_0C--;
			OBJECT(current_object)->unk_10 = 0xCu;
		}
		return;
	default:
		invalid_jump(rom_007C44[OBJECT(current_object)->unk_0C]);
	}
	t2 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t2 - 1u;
	if (t2 != 1u)
	{
		r19 = 0x7CDBu;
		ignored = sub_011A32(r19);
		return;
	}
	t3 = OBJECT(current_object)->unk_12;
	OBJECT(current_object)->unk_12 = t3 - 1u;
	if (t3 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_10 = 6u;
	t1 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_0C--;
		OBJECT(current_object)->unk_10 = 0xCu;
	}
}

void sub_007C9E(void)
{
	uint8_t r0 = 0;
	uint8_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = 0x2Bu;
		sub_011EB8(r0);
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_10 = 0xFu;
		return;
	case 1:
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 == 1u)
		{
			r0 = 0x2Bu;
			sub_011EB8(r0);
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
		}
		return;
	default:
		invalid_jump(rom_007CAC[OBJECT(current_object)->unk_0C]);
	}
}

void sub_007CF1(void)
{
	uint32_t r10 = 0;

	r10 = rom_00804A[OBJECT(player_car)->unk_60];
	mem_write16(player_car + 0x126u, mem_read16(r10));
	r10 += 2u;
	mem_write16(player_car + 0x12Cu, mem_read16(r10));
	r10 += 2u;
	mem_write16(player_car + 0x128u, mem_read16(r10));
	r10 += 2u;
	mem_write16(player_car + 0x12Au, mem_read16(r10));
	mem_write32(player_car + 0x130u, r10 + 2u);
}

void sub_009590(void)
{
	uint8_t r3_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r10 = 0, r11 = 0, r21 = 0;

	r10 = mem_read32(screen + 0x308u) + 0x1Fu;
	r0 = mem_read8(r10);
	r10++;
	do
	{
		r21 = sub_011D43();
		r1 = mem_read8(r10);
		mem_write32(r21 + 4u, sub_00974C_ADDRESS);
		mem_write8(r21 + 9u, r1);
		r11 = rom_00E0A0[r1] + unk_20E000_ADDRESS;
		mem_write32(r21 + 0x80u, r11);
		r2 = mem_read8(r11 + 0x49u);
		mem_write8(r21 + 0xAu, r2);
		mem_write16(r21 + 0x8Cu, rom_009DCC[r2]);
		mem_write16(r21 + 0x8Eu, rom_009DDC[r2]);
		mem_write16(r21 + 0x94u, rom_009DEC[r2]);
		mem_write16(r21 + 0x96u, rom_009E1C[r2]);
		mem_write8(r21 + 0x9Bu, rom_009E3C[r2]);
		mem_write8(r21 + 0x98u, rom_009E0C[r2]);
		mem_write16(r21 + 0xA4u, rom_009DFC[r2]);
		mem_write16(r21 + 0xA6u, rom_009E2C[r2]);
		mem_write8(r21 + 0xABu, rom_009E44[r2]);
		mem_write8(r21 + 0xA8u, rom_009E14[r2]);
		r3 = rom_18D25C[r2];
		mem_write32(r21 + 0x90u, r3);
		mem_write32(r21 + 0xA0u, r3 + 4u);
		mem_write32(r21 + 0xC0u, 0x9F2Cu);
		mem_write32(r21 + 0xC4u, r3 + 8u);
		mem_write32(r21 + 0xC8u, r11 + 0x52u);
		mem_write8(r21 + 0xCDu, 2u);
		mem_write32(r21 + 0xD0u, 0x9F4Cu);
		mem_write32(r21 + 0xD4u, r3 + 0xCu);
		mem_write32(r21 + 0xD8u, r11 + 0x51u);
		mem_write8(r21 + 0xDDu, 1u);
		r1 = mem_read8(r11 + 2u);
		mem_write16(r21 + 0xBAu, rom_009F8C[r1]);
		mem_write16(r21 + 0xB8u, rom_009F7C[r2]);
		mem_write8(r21 + 0xBCu, rom_009F9C[r1]);
		mem_write8(r21 + 0xBDu, r1 - mem_read8(mem_read32(screen + 0x20u) + 2u));
		r12 = (r12 & ~0xFFFFu) | (uint16_t)r21;
		r3 = r2;
		r10++;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, rom_009EBC[r3]);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3);
		mem_write8(r21 + 0xAu, 0xFFu);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, rom_009EDC[r3]);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3 + 8u);
		mem_write8(r21 + 0xAu, 0u);
		r3_2 = (uint8_t)r2;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0141F4_ADDRESS);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3_2);
		mem_write8(r21 + 0xAu, r2);
		mem_write8(r21 + 0xBu, mem_read8(r12 + 9u));
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0141F4_ADDRESS);
		mem_write16(r21 + 0x14u, r12);
		mem_write8(r21 + 9u, r3_2 + 8u);
		mem_write8(r21 + 0xAu, r2);
		mem_write8(r21 + 0xBu, mem_read8(r12 + 9u));
		r0--;
	} while (r0 != 0u);
	OBJECT(current_object)->unk_00 = 0u;
	OBJECT(current_object)->unk_01 = 0u;
}

void sub_00974C(void)
{
	bool L_00978F = false, L_009874 = false, L_009AAD = false, L_009B31 = false;
	uint8_t r0_3 = 0, r7_2 = 0;
	uint16_t r0 = 0, r1 = 0, r2 = 0, r5_2 = 0, r6 = 0, r13_2 = 0, r17_2 = 0, r18 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r2_2 = 0, r3 = 0, r4 = 0, r5 = 0, r6_2 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r19 = 0, ignored = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	mem_write8(current_object + 0xBCu, mem_read8(current_object + 0xBCu) + 1u);
	r10 = mem_read32(screen + 0xC0u) + 0x10u;
	if (mem_read32(screen + 0xC4u) <= r10)
		return;
	r0 = OBJECT(player_car)->unk_42;
	r1 = (uint16_t)(r0 + (mem_read16(screen + 0x12u) >> 7));
	r19 = OBJECT(current_object)->unk_80;
	r2 = mem_read16(r19 + 0x42u);
	if (mem_read8(current_object + 0xBDu) != 0u)
	{
		L_009B31 = r2 <= r0;
		if (!L_009B31)
			L_00978F = true;
	}
	if (!L_009B31)
	{
		if (!L_00978F && (mem_read32(screen + 0x94u) & 0x10u) != 0u)
			OBJECT(current_object)->unk_04 = rom_009FC4[(uint8_t)(OBJECT(current_object)->unk_08 >> 16u)];
		else if (r1 <= r2)
			return;
		r5 = player_car + 0xDCu;
		r12 = mem_read32(r19 + 0x30u) - AS32(OBJECT(player_car)->unk_20);
		r13 = mem_read32(r19 + 0x34u) - AS32(OBJECT(player_car)->unk_24);
		r14 = mem_read32(r19 + 0x38u) - OBJECT(player_car)->unk_28;
		r2_2 = r12 * mem_read32(r5);
		r5 += 4u;
		r3 = r13 * mem_read32(r5);
		r5 += 4u;
		r15 = (int32_t)(r2_2 + r14 * mem_read32(r5) + r3) >> 14;
		r5 += 4u;
		r2_2 = r12 * mem_read32(r5);
		r5 += 4u;
		r3 = r13 * mem_read32(r5);
		r5 += 4u;
		r16 = (int32_t)(r2_2 + r14 * mem_read32(r5) + r3) >> 14;
		r17 = (int32_t)(r12 * mem_read32(r5 + 4u) + r14 * mem_read32(r5 + 0xCu) + r13 * mem_read32(r5 + 8u)) >> 14;
		if (r17 > 0x40u && r17 < 0x3000u)
		{
			r9 = rom_1A0340[r17];
			r8 = rom_19A340[r17];
			r15 = (int32_t)(r15 * r8) >> 12;
			r16 = 0u - ((int32_t)(r16 * r8) >> 12);
			L_009B31 = (int32_t)r15 > 0x120 || (int32_t)r15 < -0x120 || (int32_t)r16 > 0xF0;
			if (!L_009B31)
			{
				r8 = (uint16_t)((int16_t)r8 >> 4);
				L_009874 = true;
			}
		}
	}
	if (!L_009874 && mem_read8(current_object + 0xBDu) != 0u)
	{
		AS8(OBJECT(current_object)->unk_2E) = 0u;
		mem_write8(current_object + 0xE1u, 0u);
		r11 = mem_read32(screen + 0x20u);
		r7 = mem_read16(r11 + 0x42u);
		r7 = (uint16_t)(r7 - mem_read16(r19 + 0x42u));
		t1 = r7 - 0x100u;
		r7 = t1;
		if ((int16_t)t1 >= 0)
			return;
		r7 = (uint16_t)(0u - r7);
		r9 = 0x191u;
		r17_2 = (uint16_t)((int16_t)((mem_read16(r19 + 0x44u) - mem_read16(r11 + 0x44u)) * 3u) >> 2);
		if ((int16_t)r17_2 <= 0x58)
		{
			if ((int16_t)r17_2 < -0x58)
				r17_2 = 0xFFA8u;
		}
		else
			r17_2 = 0x58u;
		r18 = (uint16_t)((int16_t)(0u - mem_read16(r19 + 0x56u)) >> 2);
		r8 = r7 * 3u;
		r8 = (r8 & ~0xFFFFu) | ((uint16_t)r8 >> 3);
		r12 = mem_read16(r19 + 0x3Cu);
		r13 = mem_read16(r19 + 0x3Eu);
		r14 = mem_read8(r19 + 0x4Au);
		sub_009C9E(r8, r17_2, r18, &ignored, &r10, &r13);
		r18 = (uint16_t)((int16_t)(r7 * 0xFFECu) >> 8);
		r8 = r7;
		r12 = mem_read16(current_object + 0xBAu);
		r13 = mem_read16(current_object + 0xB8u);
		r14 = 0u;
		sub_009C9E(r8, r17_2, r18, &ignored, &r10, &r13);
	}
	else
	{
		if (!L_009874)
		{
			r9 = 0x1C0u;
			r8 = 0x100u;
			r15 = 0u;
			r16 = 0u;
		}
		OBJECT(current_object)->unk_88 += mem_read16(r19 + 0x4Cu);
		r13 = (uint8_t)(OBJECT(current_object)->unk_88 >> 16u);
		r13 = r13 & 1u;
		if ((uint8_t)(OBJECT(current_object)->unk_88 >> 24u) != r13)
		{
			AT8(OBJECT(current_object)->unk_88, 3) = r13;
			AS16(OBJECT(current_object)->unk_88) = 0u;
		}
		r13 = (uint16_t)(r13 + OBJECT(player_car)->unk_7E);
		r13 = (uint16_t)(r13 + mem_read16(r19 + 0x3Eu));
		r12 = mem_read16(r19 + 0x3Cu);
		r14 = mem_read8(r19 + 0x4Au);
		r17_2 = (uint16_t)r15;
		r18 = (uint16_t)r16;
		OBJECT(current_object)->unk_30 = r15;
		OBJECT(current_object)->unk_32 = r16;
		OBJECT(current_object)->unk_34 = r8;
		OBJECT(current_object)->unk_3C = r9;
		sub_009C9E(r8, r17_2, r18, &r0_2, &r10, &r13);
		AS16(OBJECT(current_object)->unk_84) = r0_2;
		if ((uint8_t)OBJECT(current_object)->unk_2E == 0u)
		{
			r2_2 = mem_read16(r19 + 0x3Cu);
			r1_2 = (uint16_t)(r2_2 - (uint16_t)(OBJECT(current_object)->unk_8C >> 16u));
			if (rom_009E4C[r1_2] != 0u)
			{
				mem_write8(player_car + 0x137u, 0u);
				r27 = current_object + 0x90u;
				r28 = current_object + 0xA0u;
				if (((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) != 6u || rom_009E4C[r1_2] == 2u) && (r0_2 & 0x40u) == 0u)
				{
					t2 = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | t2;
				}
				if (mem_read16(r19 + 0x4Cu) == 0u)
				{
					r14 = 0u;
					r7 = 0u;
				}
				else
				{
					r14 = mem_read8(r27 + 0xAu);
					r7 = mem_read8(r28 + 0xAu);
					if (mem_read8(r27 + 0xBu) <= r14)
						r14 = 1u;
					else
						r14 = (uint8_t)(r14 + 1u);
					if (mem_read8(r28 + 0xBu) <= r7)
						r7 = 1u;
					else
						r7 = (uint8_t)(r7 + 1u);
				}
				mem_write8(r27 + 0xAu, r14);
				mem_write8(r28 + 0xAu, r7);
				r11 = mem_read32(rom_034138[r2_2] + 4u + mem_read8(r19 + 0x4Au) * 4u) + 2u;
				r3 = mem_read16(r11);
				r11 += 2u;
				r3 = r3 & 0x40u;
				r3 = r3 >> 6;
				r3 = (uint16_t)(r3 * 0x34u);
				r3 = (uint16_t)(r3 + (uint16_t)(mem_read16(r11) - (uint16_t)OBJECT(current_object)->unk_8C) / 3u);
				mem_write8(current_object + 0xE3u, r3);
				r4 = (uint16_t)(r3 << 4);
				r11 += 2u;
				r6 = mem_read16(r11);
				r5_2 = mem_read16(r11 + 2u);
				r12 = (r12 & ~0xFFu) | mem_read8(current_object + 0xE1u);
				if ((r12 & mem_read8(r27 + 8u)) == 0u)
				{
					r11 = mem_read32(r27);
					r13 = mem_read16(r27 + 6u);
					r12 = mem_read16(r27 + 4u) + r3;
					r10 = sub_009C71(r4, r5_2, r6, r8, r10, r11, r13, (uint16_t)r15, (uint16_t)r16);
				}
				r12 = (r12 & ~0xFFu) | mem_read8(current_object + 0xE1u);
				if ((r12 & mem_read8(r28 + 8u)) == 0u)
				{
					r14 = r7;
					r11 = mem_read32(r28);
					r13 = mem_read16(r28 + 6u);
					r12 = mem_read16(r28 + 4u) + r3;
					r10 = sub_009C71(r4, r5_2, r6, r8, r10, r11, r13, (uint16_t)r15, (uint16_t)r16);
				}
				r27 = current_object + 0xC0u;
				r28 = current_object + 0xD0u;
				if ((OBJECT(current_object)->unk_84 & 0x40u) == 0u)
				{
					t3 = r28;
					r28 = (r28 & ~0xFFFFu) | (uint16_t)r27;
					r27 = (r27 & ~0xFFFFu) | t3;
				}
				r7_2 = mem_read8(r19 + 0x50u);
				r10 = sub_009C1E(r4, r5_2, r6, r7_2, r8, r10, (uint16_t)r15, (uint16_t)r16);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)r28;
				r10 = sub_009C1E(r4, r5_2, r6, r7_2, r8, r10, (uint16_t)r15, (uint16_t)r16);
				r0_3 = mem_read8(current_object + 0xBFu);
				if (r0_3 != 0u || mem_read8(r19 + 0x53u) != 0u)
				{
					r14 = r0_3;
					r0_3++;
					if (r0_3 >= 5u)
						r0_3 = 0u;
					mem_write8(current_object + 0xBFu, r0_3);
					r12 = 0x44u;
					r13 = 0x58u;
					r17_2 = (uint16_t)r15;
					r18 = (uint16_t)r16;
					sub_009C9E(r8, r17_2, r18, &ignored, &r10, &r13);
					sub_009CB6(r8, r17_2, r18, &ignored, &r10, &r13);
				}
			}
			else
			{
				AS8(OBJECT(current_object)->unk_2E) = 0xFFu;
				if (mem_read8(current_object + 0xBDu) == 0u)
					mem_write8(player_car + 0x137u, 0xFFu);
			}
		}
		do
		{
			if (L_009AAD || mem_read8(screen + 0x31Cu) != 0u || mem_read16(AS32(OBJECT(player_car)->unk_00) + 0x10u) < 0x3Cu)
			{
				if (L_009AAD || mem_read8(current_object + 0xBDu) == 0u)
				{
					L_009AAD = false;
					if (mem_read8(current_object + 0xB2u) == 0u)
					{
						if (mem_read8(player_car + 0x136u) == 0u)
						{
							r4 = screen + 0x5000u + r9 * 4u;
							if (mem_read16(r4) == 0u)
								mem_write16(r4 + 2u, r10);
							r5 = mem_read32(screen + 0xC0u) + 0x10u;
							r6_2 = mem_read16(r4);
							mem_write32(r10, r6_2);
							mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
							mem_write16(r4, (r5 >> 4) | 0x8000u);
							mem_write32(screen + 0xC0u, r10);
							return;
						}
						mem_write8(player_car + 0x136u, 0u);
						mem_write8(current_object + 0xB3u, 0u);
						mem_write8(current_object + 0xB2u, 0x1Eu);
						mem_write16(current_object + 0xB0u, r16);
						mem_write16(current_object + 0xB4u, 0x2Fu);
						mem_write16(current_object + 0xB6u, 0x4Bu);
						if (mem_read8(player_car + 0x134u) == 0u)
						{
							mem_write16(current_object + 0xB4u, 0x30u);
							mem_write16(current_object + 0xB6u, 0x4Cu);
						}
					}
					mem_write8(current_object + 0xB2u, mem_read8(current_object + 0xB2u) - 1u);
					r12 = mem_read16(current_object + 0xB4u);
					r13 = mem_read16(current_object + 0xB6u);
					r14 = mem_read8(current_object + 0xB3u);
					r14 = r14 >> 1;
					r17_2 = (uint16_t)r15;
					r18 = (uint16_t)(int16_t)mem_read16(current_object + 0xB0u);
					r8 = 0x100u;
					sub_009C9E(r8, r17_2, r18, &ignored, &r10, &r13);
					mem_write8(current_object + 0xB3u, mem_read8(current_object + 0xB3u) + 1u);
					mem_write8(current_object + 0xB3u, mem_read8(current_object + 0xB3u) & 7u);
				}
				else
				{
					r0_2 = mem_read8(current_object + 0xBCu);
					if (rom_009FA4[r0_2 & 0x1Fu] != 0u)
					{
						r13_2 = mem_read16(rom_032BA4[mem_read16(current_object + 0xB8u)]);
						r12 = 0x1Bu;
						r17_2 = (uint16_t)(r15 - 3u);
						r18 = (uint16_t)((uint16_t)r16 - ((0x30u * r8) >> 8));
						r10 = sub_009D54(r10, r13_2, r17_2, r18);
						r12 = mem_read16(current_object + 0xBAu);
						r18 -= 0x16u;
						r17_2 += 0x15u;
						r10 = sub_009D54(r10, r13_2, r17_2, r18);
					}
				}
			}
			else
			{
				L_009AAD = mem_read8(current_object + 0xBDu) != 0u;
				if (!L_009AAD)
				{
					r13_2 = mem_read16(rom_032BA4[mem_read16(current_object + 0xB8u)]);
					r12 = 0x1Bu;
					r17_2 = (uint16_t)(r15 - 3u);
					r18 = (uint16_t)((uint16_t)r16 - ((0x30u * r8) >> 8));
					r10 = sub_009D54(r10, r13_2, r17_2, r18);
					r12 = mem_read16(current_object + 0xBAu);
					r18 -= 0x16u;
					r17_2 += 0x15u;
					r10 = sub_009D54(r10, r13_2, r17_2, r18);
				}
			}
		} while (L_009AAD);
	}
	r4 = screen + 0x5000u + r9 * 4u;
	if (mem_read16(r4) == 0u)
		mem_write16(r4 + 2u, r10);
	r5 = mem_read32(screen + 0xC0u) + 0x10u;
	r6_2 = mem_read16(r4);
	mem_write32(r10, r6_2);
	mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
	mem_write16(r4, (r5 >> 4) | 0x8000u);
	mem_write32(screen + 0xC0u, r10);
}

uint32_t sub_009C1E(uint32_t r4, uint16_t r5, uint16_t r6, uint8_t r7, uint32_t r8, uint32_t r10, uint16_t r15, uint16_t r16)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r11 = 0, r13 = 0;

	r1 = mem_read8(r27 + 0xCu);
	if (r1 == 0u && (r7 & mem_read8(r27 + 0xDu)) == 0u)
	{
		mem_write8(r27 + 0xCu, 0u);
		mem_write8(r27 + 0xEu, 0u);
		return r10;
	}
	r0 = mem_read8(r27 + 0xEu);
	r2 = mem_read8(mem_read32(r27 + 8u));
	if (r2 != 0u && r2 != r0)
	{
		r0 = r2;
		mem_write8(r27 + 0xEu, r2);
		r1 = 0u;
	}
	r14 = r1;
	r1 = (uint8_t)(r1 + 1u);
	if (rom_009F6C[r0] <= r1)
		r1 = 0u;
	mem_write8(r27 + 0xCu, r1);
	r11 = mem_read32(r27 + 4u);
	r12 = mem_read16(mem_read32(r27) + r0 * 2u);
	r13 = rom_009F0C[r0];
	return sub_009C71(r4, r5, r6, r8, r10, r11, r13, r15, r16);
}

uint32_t sub_009C71(uint32_t r4, uint16_t r5, uint16_t r6, uint32_t r8, uint32_t r10, uint32_t r11, uint32_t r13, uint16_t r15, uint16_t r16)
{
	uint32_t carry = 0;
	uint16_t r17 = 0, r18 = 0;
	uint32_t ignored = 0;

	r17 = (uint16_t)r5;
	r18 = (uint16_t)r6;
	r11 += r4;
	if (mem_read32(r11) == 0u)
		return r10;
	r17 = (uint16_t)((r17 + mem_read16(r11)) * r8);
	r18 = (uint16_t)((r18 + mem_read16(r11 + 2u)) * r8);
	carry = (r17 >> 7) & 1u;
	r17 = (uint16_t)(((int16_t)r17 >> 8) + carry);
	carry = (r18 >> 7) & 1u;
	r17 += r15;
	r18 = (uint16_t)(((int16_t)r18 >> 8) + carry + r16);
	sub_009C9E(r8, r17, r18, &ignored, &r10, &r13);
	return r10;
}

void sub_009C9E(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13)
{
	uint32_t r0 = 0;

	r12 = mem_read32(rom_034138[r12] + 4u + r14 * 4u) + 2u;
	*p_r13 = rom_032BA4[*p_r13];
	sub_009CB6(r8, r17, r18, &r0, p_r10, p_r13);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

void sub_009CB6(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13)
{
	uint32_t r10 = *p_r10;
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	r0 = mem_read16(r12);
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = 0x122B74u + mem_read16(r12) * 4u;
	t1 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t1;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t2 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t2;
	t3 = ROM16(r14);
	r14 = r14 + 2u;
	t4 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = (((t3 * r8) >> 8) & 0x3FFu) | t4;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	t5 = ROM16(r14);
	r14 = r14 + 2u;
	t6 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = (((t5 * r8) >> 8) & 0x3FFu) | t6 | mem_read16(screen + 0x1Au);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	r12 = r12 + 2u;
	r2 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r8) >> 8) + r18);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	r12 = r12 + 2u;
	r2 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r8) >> 8) + r17);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t7 = mem_read16(*p_r13);
	*p_r13 += 2u;
	r2 = t7;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(screen + 0xB0u, mem_read16(screen + 0xB0u) + 1u);
	r12 = r12 + 2u;
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0;
	*p_r10 = r10;
}

uint32_t sub_009D54(uint32_t r10, uint16_t r13, uint16_t r17, uint16_t r18)
{
	uint16_t r0 = 0, r1 = 0, r2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;

	r12 = mem_read32(rom_034138[r12] + 4u) + 2u;
	r0 = mem_read16(r12);
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = 0x122B74u + mem_read16(r12) * 4u;
	t1 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t1;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	t2 = ROM16(r14);
	r14 = r14 + 2u;
	r1 = t2;
	t3 = ROM16(r14);
	r14 = r14 + 2u;
	t4 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = t3 | t4;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	t5 = ROM16(r14);
	r14 = r14 + 2u;
	t6 = ROM16(r14);
	r14 = r14 + 2u;
	r2 = t5 | t6 | mem_read16(screen + 0x1Au);
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r18);
	mem_write16(r10, r18);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r17);
	mem_write16(r10, r17);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r13);
	mem_write16(r10, r13);
	r10 += 2u;
	mem_write16(screen + 0xB0u, mem_read16(screen + 0xB0u) + 1u);
	r12 = r12 + 2u;
	return r10;
}

void sub_00A00C(void)
{
	uint32_t r0 = 0, r1 = 0;

	r0 = OBJECT(player_car)->unk_60;
	r12 = rom_00ADCC[r0];
	r1 = OBJECT(player_car)->unk_61 + r0 * 2u;
	OBJECT(player_car)->unk_50 = rom_00B0C0[r1];
	r1 = rom_00B04C[r1];
	OBJECT(player_car)->unk_63 = mem_read8(r1);
	r1++;
	OBJECT(player_car)->unk_54 = r1;
	AS8(OBJECT(player_car)->unk_64) = mem_read8(r1);
	AS32(OBJECT(player_car)->unk_58) = r12;
	sub_00B4F8();
}

void sub_00A04A(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r10 = screen + 0x1300u;
	r11 = OBJECT(player_car)->unk_50;
	r9 = OBJECT(player_car)->unk_63;
	r9 = (uint8_t)(r9 + 1u);
	do
	{
		r12 = r11 + 4u;
		do
		{
			r0 = mem_read16(r11);
			r11 += 2u;
			t1 = mem_read16(r12);
			r12 = r12 + 2u;
			t2 = mem_read16(r12);
			r12 = r12 + 2u;
			r1 = (uint32_t)mem_read16(r11) << 16;
			r2 = t1 - r0;
			r3 = divide_s32(((uint32_t)t2 << 16) - r1, r2);
			r11 += 2u;
			do
			{
				mem_write16(r10, r1 >> 16);
				r1 += r3;
				r2--;
				r10 += 2u;
			} while (r2 != 0u);
		} while ((int16_t)mem_read16(r12) >= 0);
		r11 = r12 + 4u;
		r9--;
	} while (r9 != 0u);
}

void sub_00A098(void)
{
	uint8_t r0 = 0;

	if ((mem_read32(screen + 0x90u) & 0x100u) == 0u)
	{
		mem_write8(current_object + 0xD0u, mem_read8(screen + 0x101u));
		mem_write8(current_object + 0xD1u, mem_read8(screen + 0x102u));
		mem_write8(current_object + 0xD2u, mem_read8(screen + 0x100u));
		return;
	}
	mem_write8(current_object + 0xD1u, 0u);
	r0 = mem_read8(current_object + 0xD2u);
	if (mem_read8(player_car + 0x147u) != 0u)
	{
		if ((int16_t)OBJECT(player_car)->unk_38 < 0)
		{
			if (r0 < 0xF8u)
				mem_write8(current_object + 0xD2u, r0 + 8u);
			r0 = mem_read8(current_object + 0xD0u);
			if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
			{
				if (r0 < 0xF0u)
					mem_write8(current_object + 0xD0u, r0 + 0x10u);
				return;
			}
			if (r0 < 4u)
				return;
			mem_write8(current_object + 0xD0u, r0 - 4u);
			return;
		}
		if (r0 >= 8u)
			mem_write8(current_object + 0xD2u, r0 - 8u);
		r0 = mem_read8(current_object + 0xD0u);
		if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
		{
			if (r0 < 0xF0u)
				mem_write8(current_object + 0xD0u, r0 + 0x10u);
			return;
		}
		if (r0 < 4u)
			return;
		mem_write8(current_object + 0xD0u, r0 - 4u);
		return;
	}
	if (OBJECT(player_car)->unk_7C != 0u && OBJECT(player_car)->unk_60 != 7u)
	{
		if ((int16_t)OBJECT(player_car)->unk_38 < 0)
		{
			if (r0 >= 8u)
				mem_write8(current_object + 0xD2u, r0 - 8u);
			r0 = mem_read8(current_object + 0xD0u);
			if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
			{
				if (r0 < 0xF0u)
					mem_write8(current_object + 0xD0u, r0 + 0x10u);
				return;
			}
			if (r0 < 4u)
				return;
			mem_write8(current_object + 0xD0u, r0 - 4u);
			return;
		}
		if (r0 < 0xF8u)
			mem_write8(current_object + 0xD2u, r0 + 8u);
		r0 = mem_read8(current_object + 0xD0u);
		if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
		{
			if (r0 < 0xF0u)
				mem_write8(current_object + 0xD0u, r0 + 0x10u);
			return;
		}
		if (r0 < 4u)
			return;
		mem_write8(current_object + 0xD0u, r0 - 4u);
		return;
	}
	if (OBJECT(current_object)->unk_2E != 0u)
	{
		if ((int16_t)OBJECT(current_object)->unk_2E < 0)
		{
			if (r0 >= 8u)
				mem_write8(current_object + 0xD2u, r0 - 8u);
		}
		else if (r0 < 0xF8u)
			mem_write8(current_object + 0xD2u, r0 + 8u);
	}
	r0 = mem_read8(current_object + 0xD0u);
	if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
	{
		if (r0 < 0xF0u)
			mem_write8(current_object + 0xD0u, r0 + 0x10u);
		return;
	}
	if (r0 < 4u)
		return;
	mem_write8(current_object + 0xD0u, r0 - 4u);
}

void sub_00A130(uint32_t *p_r11, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r17)
{
	uint32_t r11 = *p_r11;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t r0_2 = 0;
	uint16_t r1 = 0, r2 = 0, r3 = 0, r6_2 = 0, r7_2 = 0, r8_2 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2_2 = 0, r3_2 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r13 = 0, r15 = 0, r17 = 0, r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	r12 = AS32(OBJECT(player_car)->unk_58);
	mem_write8(player_car + 0x140u, 0u);
	mem_write8(player_car + 0x141u, 0u);
	mem_write8(player_car + 0x142u, 0u);
	if (OBJECT(player_car)->unk_74 != 0u && mem_read8(player_car + 0x144u) != 0u)
	{
		mem_write8(player_car + 0x141u, 0xBu);
		mem_write8(player_car + 0x142u, 0xBu);
		mem_write8(player_car + 0x140u, 3u);
	}
	r0 = OBJECT(player_car)->unk_74;
	if (r0 != 0u)
	{
		r0 = (r0 * r0) >> 16;
		r1 = (uint16_t)(int16_t)OBJECT(player_car)->unk_38;
		r2 = mem_read16(r11 + 0x12u);
		r3 = mem_read16(r12 + 0x1Au);
		r0 = sub_00B52F(r0, r1, r2, r3);
	}
	r7 = mem_read16(r11 + 2u);
	mem_write16(mem_read32(screen + 0x24u) + 0x54u, r7);
	r7 = ((int32_t)(int16_t)rom_100C00[r7] >> 1) - (r0 + 0x1800u);
	r8 = mem_read8(current_object + 0xD0u);
	OBJECT(player_car)->unk_7E = 0u;
	r9 = rom_00ACCB[mem_read8(current_object + 0xD1u)];
	if (r9 != 0u)
	{
		r8 = 0u;
		OBJECT(player_car)->unk_7E = 2u;
	}
	mem_write16(current_object + 0xB0u, r9);
	r9 = r9 * mem_read32(r12 + 0x1Cu);
	r1_2 = OBJECT(player_car)->unk_62;
	r2_2 = screen + 0x1300u + (uint16_t)(r1_2 << 9);
	r1_2 = OBJECT(player_car)->unk_74;
	r8 = (int32_t)((uint32_t)(int8_t)rom_00ABCB[r8] * mem_read16(r2_2 + (r1_2 >> 8) * 2u)) >> 6;
	r19 = (uint8_t)OBJECT(player_car)->unk_64;
	sub_00A85D(r7, r8, r9, r19);
	if (unk_20E623 != 0u)
		OBJECT(player_car)->unk_74 = mem_read16(mem_read32(screen + 0x20u) + 0x4Cu);
	r6 = OBJECT(player_car)->unk_74;
	OBJECT(current_object)->unk_2C = r6;
	r0 = mem_read8(current_object + 0xD2u);
	r0 = (int32_t)((int8_t)mem_read8(mem_read32(r12) + (r0 >> 3)) * 0x14u) / 0x10;
	if (r6 <= 0xA000u)
	{
		if (r6 > 0x2000u)
			r0 = (int32_t)(r0 * (r6 + 0x6000u)) / 0x10000;
		else
			r0 = (int32_t)(r0 * r6) / 0x4000;
	}
	AT16(OBJECT(current_object)->unk_94, 2) = r0;
	r8_2 = (uint16_t)((int16_t)((((uint32_t)(uint16_t)OBJECT(current_object)->unk_90 << 4) - ((uint32_t)(uint16_t)(OBJECT(current_object)->unk_8C >> 16u) << 4)) * 0x20u) / 0x100);
	if (OBJECT(current_object)->unk_0C == 5u)
		r8_2 = (uint16_t)((int16_t)r8_2 >> 4);
	r8_2 = (uint16_t)divide_s32((uint32_t)(int16_t)r8_2 << 8, (uint16_t)(OBJECT(player_car)->unk_68 >> 16u) + 0x100u);
	AT8(OBJECT(player_car)->unk_64, 3) = 0u;
	r1_2 = OBJECT(current_object)->unk_2C * r0;
	r2_2 = mem_read32(r12 + 0x20u);
	if ((int32_t)r2_2 <= (int32_t)r1_2)
	{
		AT8(OBJECT(player_car)->unk_64, 3) = 0xFFu;
		r1_2 = (int32_t)((r1_2 - r2_2) * mem_read32(r12 + 0x24u)) / -0x400000;
		mem_write16(current_object + 0xB8u, r1_2);
		r8_2 = (uint16_t)(r8_2 + r1_2 + r0);
		if (mem_read8(player_car + 0x140u) == 0u)
		{
			mem_write8(player_car + 0x141u, 3u);
			mem_write8(player_car + 0x142u, 3u);
			mem_write8(player_car + 0x140u, 3u);
		}
		r0_2 = 3u;
		sub_011FC3(r0_2);
		OBJECT(current_object)->unk_2E = r8_2;
		r0 = r8_2;
	}
	else
	{
		r2_2 = 0u - r2_2;
		if ((int32_t)r1_2 <= (int32_t)r2_2)
		{
			AT8(OBJECT(player_car)->unk_64, 3) = 0xFFu;
			r1_2 = (int32_t)((r1_2 - r2_2) * mem_read32(r12 + 0x24u)) / -0x400000;
			mem_write16(current_object + 0xB8u, r1_2);
			r8_2 = (uint16_t)(r8_2 + r1_2 + r0);
			if (mem_read8(player_car + 0x140u) == 0u)
			{
				mem_write8(player_car + 0x141u, 3u);
				mem_write8(player_car + 0x142u, 3u);
				mem_write8(player_car + 0x140u, 3u);
			}
			r0_2 = 3u;
			sub_011FC3(r0_2);
			OBJECT(current_object)->unk_2E = r8_2;
			r0 = r8_2;
		}
		else
		{
			r8_2 += r0;
			if ((mem_read32(screen + 0x90u) & 0x100u) != 0u || mem_read8(mem_read32(screen + 0x20u) + 0x5Eu) != 0u)
			{
				r0 = OBJECT(current_object)->unk_2E;
				t1 = r8_2;
				r8_2 = (uint16_t)(t1 - r0);
				if (t1 != r0)
					r0 = (uint16_t)(r0 + ((int16_t)(r8_2 * 1u) / 4 + (shift_signed_by(r8_2, 0xF0u, 16) * 2u + 1u)));
				OBJECT(current_object)->unk_2E = r0;
			}
			else
			{
				r0 = r8_2;
				OBJECT(current_object)->unk_2E = r0;
			}
		}
	}
	if (unk_20E623 != 0u)
	{
		r0 = mem_read16(mem_read32(screen + 0x20u) + 0x4Eu);
		OBJECT(current_object)->unk_2E = r0;
	}
	r0 = (uint32_t)(int16_t)r0 << 8;
	r1_2 = OBJECT(current_object)->unk_2C;
	r9 = r1_2;
	r2_2 = 0u;
	r3_2 = 0u;
	r0 = sub_0120AC(r0, r1_2, r2_2, r3_2);
	r9 = (r9 * (int16_t)rom_101400[r0]) >> 14;
	r15 = AS32(OBJECT(player_car)->unk_34) + r9;
	if (mem_read8(current_object + 0xF8u) == 0u)
	{
		r7_2 = (uint16_t)(mem_read16(mem_read32(screen + 0x68u) + 0x42u) - mem_read16(mem_read32(screen + 0x20u) + 0x42u));
		if ((int16_t)r7_2 > 0x20)
			r7_2 = 0x20u;
		t2 = r7_2;
		r7_2 = (uint16_t)(t2 - 1u);
		if ((int16_t)t2 < 1)
			r7_2 = 0u;
		AS16(OBJECT(player_car)->unk_68) = r7_2;
		r6 = (uint16_t)(OBJECT(player_car)->unk_68 >> 16u);
		t3 = r7_2 - r6;
		overflow = (uint32_t)(int16_t)r7_2 - (int16_t)r6 != (uint32_t)(int16_t)(r7_2 - r6);
		zero = t3 == 0u;
		negative = t3 >> 15;
		if (r7_2 != r6)
		{
			if (((negative ^ overflow) | zero) == 0u)
				r6 = (uint16_t)(r6 + 1u);
			else
				r6 = (uint16_t)(r6 - 1u);
		}
		AT16(OBJECT(player_car)->unk_68, 2) = r6;
		r15 += (r6 * r9) >> 7;
	}
	if (unk_20E623 != 0u)
	{
		r15 = mem_read16(mem_read32(screen + 0x20u) + 0x40u);
		r6_2 = mem_read16(mem_read32(screen + 0x20u) + 0x42u);
		r13 = (int16_t)mem_read16(mem_read32(screen + 0x20u) + 0x44u);
		OBJECT(player_car)->unk_32 = r15;
		AS32(OBJECT(player_car)->unk_34) = r15;
		AS32(OBJECT(player_car)->unk_34) = AS32(OBJECT(player_car)->unk_34) << 8;
		AS16(OBJECT(player_car)->unk_44) = r6_2;
		r6_2 = (r6_2 * 0x20u) & 0xFFFu;
		AS16(OBJECT(player_car)->unk_40) = r6_2;
		r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + r6_2);
		r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
		r17 = mem_read16(r11);
		r17 = (uint16_t)(r17 - mem_read16(r12));
		r17 = (int16_t)rom_100C00[r17 & 0xFFFu] * r13;
		carry = (r17 >> 5) & 1u;
		r17 = (int32_t)(((int32_t)r17 >> 6) + 0x8000u + carry) >> 8;
		r6_2 = mem_read16(mem_read32(screen + 0x20u) + 0x46u);
		OBJECT(player_car)->unk_42 = r6_2;
		mem_write16(player_car + 0xA4u, (r6_2 * 0x20u) & 0xFFFu);
	}
	else
	{
		r13 = (int16_t)(OBJECT(player_car)->unk_38 + OBJECT(current_object)->unk_2E + mem_read16(current_object + 0xCEu));
		OBJECT(player_car)->unk_38 = r13;
		for (;;)
		{
			r12 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
			r17 = mem_read16(r11);
			r17 = (uint16_t)(r17 - mem_read16(r12));
			r17 = (int16_t)rom_100C00[r17 & 0xFFFu] * r13;
			carry = (r17 >> 5) & 1u;
			r17 = ((int32_t)r17 >> 6) + 0x8000u + carry;
			if ((int32_t)r15 < (int32_t)r17)
				break;
			r15 -= r17;
			r11 = r12;
			OBJECT(player_car)->unk_42++;
			AS16(OBJECT(player_car)->unk_44)++;
			mem_write16(player_car + 0xA4u, mem_read16(player_car + 0xA4u) + 0x20u);
			mem_write16(player_car + 0xA4u, mem_read16(player_car + 0xA4u) & 0xAFFFu);
			AS16(OBJECT(player_car)->unk_40) += 0x20u;
			OBJECT(player_car)->unk_41 &= 0xAFu;
		}
		AS32(OBJECT(player_car)->unk_34) = r15;
		r15 = (int32_t)r15 >> 8;
		r17 = (int32_t)r17 >> 8;
		OBJECT(player_car)->unk_32 = r15;
	}
	if (unk_20E623 != 0u)
	{
		OBJECT(player_car)->unk_38 = r13;
		sub_00A537(r11, r13, r15, r17);
		*p_r11 = r11;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r17 = r17;
		return;
	}
	r6_2 = mem_read16(r11 + 0x12u);
	r7_2 = (uint16_t)(0u - r6_2);
	r6_2 += 0x60u;
	r7_2 -= 0x60u;
	if ((int16_t)r13 < (int16_t)r6_2)
	{
		if ((int16_t)r7_2 < (int16_t)r13)
		{
			sub_00A537(r11, r13, r15, r17);
			*p_r11 = r11;
			*p_r13 = r13;
			*p_r15 = r15;
			*p_r17 = r17;
			return;
		}
		r13 = (r13 & ~0xFFFFu) | r7_2;
		OBJECT(player_car)->unk_38 = r13;
		sub_00A537(r11, r13, r15, r17);
		*p_r11 = r11;
		*p_r13 = r13;
		*p_r15 = r15;
		*p_r17 = r17;
		return;
	}
	r13 = (r13 & ~0xFFFFu) | r6_2;
	OBJECT(player_car)->unk_38 = r13;
	sub_00A537(r11, r13, r15, r17);
	*p_r11 = r11;
	*p_r13 = r13;
	*p_r15 = r15;
	*p_r17 = r17;
}

void sub_00A537(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17)
{
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r18 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	if (unk_20E623 == 0u)
	{
		r0 = 0u;
		r1 = r13 << 7;
		r2 = r15 << 7;
		r4 = mem_read16(r11 + 2u);
		r3 = mem_read16(r11);
		sub_01028A(r0, r3, r4, &r1, &r2);
		AS32(OBJECT(current_object)->unk_20) = (int32_t)(r1 * 2u + (mem_read32(r11 + 4u) << 8)) >> 8;
		OBJECT(current_object)->unk_28 = (int32_t)(r2 * 2u + (mem_read32(r11 + 8u) << 8)) >> 8;
		r18 = r17 - r15;
		r1 = divide_s32(mem_read32(r12 + 0xCu) * r15 + mem_read32(r11 + 0xCu) * r18, r17) + mem_read32(current_object + 0xC8u);
	}
	else
	{
		AS32(OBJECT(current_object)->unk_20) = mem_read32(mem_read32(screen + 0x20u) + 0x30u);
		r1 = mem_read32(mem_read32(screen + 0x20u) + 0x34u) + (int16_t)mem_read16(mem_read32(screen + 0x20u) + 0x56u);
		OBJECT(current_object)->unk_28 = mem_read32(mem_read32(screen + 0x20u) + 0x38u);
		r18 = r17 - r15;
	}
	t1 = r1;
	r1 = AS32(OBJECT(current_object)->unk_24);
	AS32(OBJECT(current_object)->unk_24) = t1;
	mem_write32(current_object + 0xB4u, r1);
	OBJECT(player_car)->unk_3A = r18;
	r0 = (uint16_t)((uint32_t)mem_read16(r11) << 4);
	r1 = (uint16_t)((uint32_t)mem_read16(r12) << 4);
	r2 = r0;
	switch (r2 >> 14)
	{
	case 0:
	case 3:
		r0 = (int16_t)r0;
		r1 = (int16_t)r1;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(rom_00A5F4[r2 >> 14]);
	}
	t2 = ((int32_t)divide_s32(r0 * r18 + r1 * r15, r17) >> 4) & 0xFFFu;
	r0_2 = (uint16_t)(OBJECT(current_object)->unk_8C >> 16u);
	AT16(OBJECT(current_object)->unk_8C, 2) = t2;
	AS16(OBJECT(current_object)->unk_90) = r0_2;
}

void sub_00A632(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17)
{
	uint16_t r2_2 = 0, r8 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r9 = 0, r18 = 0;

	r0 = (uint32_t)(int16_t)mem_read16(player_car + 0x110u) + (int16_t)mem_read16(player_car + 0x108u);
	if ((int32_t)r0 >= 0x30)
		r0 = 0x30u;
	if ((int32_t)r0 <= -0xC0)
		r0 = 0xFFFFFF40u;
	r0 = r0 + (int16_t)mem_read16(player_car + 0x10Cu) + (int16_t)mem_read16(player_car + 0x100u);
	r9 = ((int32_t)r0 >> 2) & ~0x1Fu;
	r11 += r9;
	r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 | 0x2000u);
	r11 = (r11 & ~0xFFFFu) | (r11 & 0xAFFFu);
	r12 = r12 + r9;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x2000u);
	r15 += r0 & 0x7Fu;
	r0 = 0u;
	r1 = (r13 + (int16_t)mem_read16(player_car + 0x106u)) << 7;
	r2 = r15 << 7;
	r4 = mem_read16(r11 + 2u);
	r3 = mem_read16(r11);
	sub_01028A(r0, r3, r4, &r1, &r2);
	mem_write32(current_object + 0xA0u, ((int32_t)(r1 * 2u) >> 8) + mem_read32(r11 + 4u));
	mem_write32(current_object + 0xA8u, ((int32_t)(r2 * 2u) >> 8) + mem_read32(r11 + 8u));
	r18 = r17 - r15;
	mem_write32(current_object + 0xA4u, divide_s32(mem_read32(((r12 & ~0xFFFFu) | (r12 & 0xAFFFu)) + 0xCu) * r15 + mem_read32(r11 + 0xCu) * r18, r17) + (int16_t)mem_read16(player_car + 0x104u));
	r12 = ((r11 + 0x180u) & ~0xFFFFu) | ((r11 + 0x180u) & 0xAFFFu);
	r0 = mem_read32(r12 + 4u);
	r1 = mem_read32(r12 + 0xCu);
	r2 = mem_read32(r12 + 8u);
	r3 = mem_read32(r11 + 4u);
	r4 = mem_read32(r11 + 0xCu);
	r5 = mem_read32(r11 + 8u);
	sub_012109(r2, r3, r4, r5, &r0, &r1);
	r11 = ((r11 + 0x20u) & ~0xFFFFu) | ((r11 + 0x20u) & 0xAFFFu);
	r12 = ((r11 + 0x180u) & ~0xFFFFu) | ((r11 + 0x180u) & 0xAFFFu);
	r8 = (uint16_t)r0;
	r9 = r1;
	r0 = mem_read32(r12 + 4u);
	r1 = mem_read32(r12 + 0xCu);
	r2 = mem_read32(r12 + 8u);
	r3 = mem_read32(r11 + 4u);
	r4 = mem_read32(r11 + 0xCu);
	r5 = mem_read32(r11 + 8u);
	sub_012109(r2, r3, r4, r5, &r0, &r1);
	r0 = (uint16_t)(r0 << 4);
	r1 = (uint16_t)(r1 << 4);
	r8 = (uint16_t)((uint32_t)r8 << 4);
	r9 = (uint16_t)(r9 << 4);
	r2 = r1;
	switch (r2 >> 14)
	{
	case 0:
	case 3:
		r1 = (int16_t)r1;
		r9 = (int16_t)r9;
		break;
	case 1:
	case 2:
		break;
	default:
		invalid_jump(rom_00A798[r2 >> 14]);
	}
	r1 = ((int32_t)divide_s32(r1 * r15 + r9 * r18, r17) >> 4) & 0xFFFu;
	r0 = ((int32_t)divide_s32((int16_t)r0 * r15 + (int16_t)r8 * r18, r17) >> 4) & 0xFFFu;
	OBJECT(player_car)->unk_2E = r1;
	OBJECT(player_car)->unk_30 = r0;
	r2_2 = (uint16_t)(mem_read16(player_car + 0x102u) + mem_read16(player_car + 0x112u) + mem_read16(player_car + 0x10Au) + mem_read16(screen + 0x18u));
	if ((int16_t)r2_2 > 0x200)
		r2_2 = 0x200u;
	if ((int16_t)r2_2 < 0xC0)
		r2_2 = 0xC0u;
	r0 = (uint16_t)(r0 + r2_2 + mem_read16(player_car + 0x10Eu)) & 0xFFFu;
	OBJECT(player_car)->unk_2C = r0;
	r2 = mem_read16(screen + 0x10u);
	r0 = (uint16_t)(r0 + 0x800u);
	r0 = r0 & 0xFFFu;
	sub_00AABC(r0, r1, r2, &r3, &r4, &r5);
	r4 += mem_read32(current_object + 0xA4u);
	r5 += mem_read32(current_object + 0xA8u);
	AS32(OBJECT(player_car)->unk_20) = r3 + mem_read32(current_object + 0xA0u);
	AS32(OBJECT(player_car)->unk_24) = r4;
	OBJECT(player_car)->unk_28 = r5;
}

void sub_00A85D(uint32_t r7, uint32_t r8, uint32_t r9, uint32_t r19)
{
	bool L_00A8A3 = false, L_00A8E6 = false, L_00A914 = false, L_00A9E9 = false;
	uint32_t negative = 0, overflow = 0;
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r18 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0;

	r9 = (int32_t)(r9 + r7 + r8) >> 6;
	AS16(OBJECT(current_object)->unk_94) = r9;
	r18 = (r19 * 0xFFFFu) >> 8;
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
						if (L_00A8E6 || L_00A9E9 || L_00A914 || L_00A914 || L_00A914 || (uint8_t)(OBJECT(player_car)->unk_64 >> 8u) == 0u)
						{
							if (L_00A8E6 || !L_00A9E9)
							{
								if (L_00A8E6 || L_00A914 || L_00A914 || L_00A914 || (int32_t)mem_read32(r12 + 0xCu) <= (int32_t)r9)
								{
									if (L_00A8E6 || !L_00A914)
									{
										if (L_00A8E6 || !L_00A914)
										{
											if (L_00A8E6 || !L_00A914)
											{
												if (!L_00A8E6)
													AT8(OBJECT(player_car)->unk_64, 1) = 0xFFu;
												if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
												{
													r0 = 1u;
													sub_011FC3(r0);
												}
												r9 = mem_read32(r12 + 0x14u);
												if (mem_read8(player_car + 0x140u) == 0u)
												{
													mem_write8(player_car + 0x141u, 1u);
													mem_write8(player_car + 0x142u, 1u);
													mem_write8(player_car + 0x140u, 3u);
												}
											}
										}
									}
									r0_2 = OBJECT(player_car)->unk_74;
									t1 = mem_read32(current_object + 0xC4u);
									t2 = r0_2 + t1;
									overflow = v60_overflow_add32(r0_2, t1, 0u);
									negative = t2 >> 31;
									r0_2 = t2;
									if (negative ^ overflow)
										r0_2 = 0u;
									r1 = r0_2 + r9;
									if ((int32_t)r1 <= (int32_t)r18)
									{
										if ((int32_t)r1 < 0)
											r1 = 0u;
										t3 = r1;
										r1 = t3 - r0_2;
										if (t3 != r0_2)
											r0_2 += (int32_t)(r1 * 1u) / 2 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
									}
									else
									{
										t4 = r18;
										r1 = t4 - r0_2;
										if (t4 != r0_2)
											r0_2 += (int32_t)(r1 * 1u) / 0x1E + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
										if (r0_2 >= 0xFFFFu)
											r0_2 = 0xFFFFu;
									}
									OBJECT(player_car)->unk_74 = r0_2;
									mem_write32(current_object + 0xC0u, r0_2);
									if (r0_2 == 0u)
									{
										AT8(OBJECT(player_car)->unk_64, 2) = 0u;
										r0_2 = (uint32_t)mem_read8(current_object + 0xD0u) << 8;
										r1 = OBJECT(player_car)->unk_70;
										t5 = r0_2;
										r0_2 = t5 - r1;
										if (t5 != r1)
											r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
										if (r1 >= 0xFFFFu)
											r1 = 0xFFFFu;
										OBJECT(player_car)->unk_70 = r1;
										return;
									}
									AT8(OBJECT(player_car)->unk_64, 2) = 0xFFu;
									r1 = OBJECT(player_car)->unk_70;
									t6 = divide_u32(r0_2 << 8, r19);
									r0_2 = t6 - r1;
									if (t6 != r1)
										r1 += (int32_t)(r0_2 * 1u) / 0xA + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
									if (r1 >= 0xFFFFu)
										r1 = 0xFFFFu;
									OBJECT(player_car)->unk_70 = r1;
									return;
								}
							}
							if (L_00A9E9 || OBJECT(player_car)->unk_74 == 0u || (int32_t)r9 > -0x380)
							{
								AT8(OBJECT(player_car)->unk_64, 1) = 0u;
								r0_2 = OBJECT(player_car)->unk_74;
								t7 = mem_read32(current_object + 0xC4u);
								t8 = r0_2 + t7;
								overflow = v60_overflow_add32(r0_2, t7, 0u);
								negative = t8 >> 31;
								r0_2 = t8;
								if (negative ^ overflow)
									r0_2 = 0u;
								r1 = r0_2 + r9;
								if ((int32_t)r1 <= (int32_t)r18)
								{
									if ((int32_t)r1 < 0)
										r1 = 0u;
									t9 = r1;
									r1 = t9 - r0_2;
									if (t9 != r0_2)
										r0_2 += (int32_t)(r1 * 1u) / 2 + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
								}
								else
								{
									t10 = r18;
									r1 = t10 - r0_2;
									if (t10 != r0_2)
										r0_2 += (int32_t)(r1 * 1u) / 0x1E + (shift_signed_by(r1, 0xE0u, 32) * 2u + 1u);
									if (r0_2 >= 0xFFFFu)
										r0_2 = 0xFFFFu;
								}
								OBJECT(player_car)->unk_74 = r0_2;
								mem_write32(current_object + 0xC0u, r0_2);
								if (r0_2 == 0u)
								{
									AT8(OBJECT(player_car)->unk_64, 2) = 0u;
									r0_2 = (uint32_t)mem_read8(current_object + 0xD0u) << 8;
									r1 = OBJECT(player_car)->unk_70;
									t5 = r0_2;
									r0_2 = t5 - r1;
									if (t5 != r1)
										r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
									if (r1 >= 0xFFFFu)
										r1 = 0xFFFFu;
									OBJECT(player_car)->unk_70 = r1;
									return;
								}
								AT8(OBJECT(player_car)->unk_64, 2) = 0xFFu;
								r0_2 = divide_u32(r0_2 << 8, r19);
								r1 = OBJECT(player_car)->unk_70;
								t5 = r0_2;
								r0_2 = t5 - r1;
								if (t5 != r1)
									r1 += (int32_t)(r0_2 * 1u) / 3 + (shift_signed_by(r0_2, 0xE0u, 32) * 2u + 1u);
								if (r1 >= 0xFFFFu)
									r1 = 0xFFFFu;
								OBJECT(player_car)->unk_70 = r1;
								return;
							}
							AT8(OBJECT(player_car)->unk_64, 1) = 0xFFu;
							L_00A8A3 = true;
						}
						else
						{
							L_00A8E6 = (int32_t)mem_read32(r12 + 0x10u) <= (int32_t)r9;
							if (!L_00A8E6)
							{
								L_00A9E9 = OBJECT(player_car)->unk_74 == 0u || (int32_t)r9 > -0x480;
								if (!L_00A9E9)
									L_00A8A3 = true;
							}
						}
					} while (L_00A8E6);
				} while (L_00A9E9);
				if (L_00A8A3)
				{
					L_00A8A3 = false;
					L_00A914 = mem_read8(screen + 0x102u) == 0u;
					if (!L_00A914)
					{
						if ((uint8_t)(OBJECT(player_car)->unk_64 >> 24u) == 0u)
						{
							r0 = rom_00ABC3[OBJECT(player_car)->unk_60];
							sub_011FC9(r0);
						}
						L_00A914 = mem_read8(player_car + 0x140u) != 0u;
						if (!L_00A914)
						{
							mem_write8(player_car + 0x141u, 2u);
							mem_write8(player_car + 0x142u, 2u);
							mem_write8(player_car + 0x140u, 3u);
							L_00A914 = true;
						}
					}
				}
			} while (L_00A914);
		} while (L_00A914);
	} while (L_00A914);
}

void sub_00AABC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t *p_r3, uint32_t *p_r4, uint32_t *p_r5)
{
	uint32_t r6 = 0;

	r6 = (int16_t)rom_101400[r0];
	*p_r3 = (int32_t)(((int32_t)((int16_t)rom_100C00[r1] * r6) >> 14) * r2) >> 14;
	*p_r4 = 0u - ((int32_t)((int16_t)rom_100C00[r0] * r2) >> 14);
	*p_r5 = (int32_t)(((int32_t)((int16_t)rom_101400[r1] * r6) >> 14) * r2) >> 14;
}

void sub_00AB24(void)
{
	sub_00AB46();
}

void sub_00AB36(void)
{
	sub_00AB46();
}

void sub_00AB46(void)
{
}

void sub_00B4F8(void)
{
	uint16_t r5 = 0;
	uint32_t r7 = 0;

	r7 = mem_read8(screen + 0xA3u);
	mem_write8(player_car + 0x146u, r7);
	r5 = (uint16_t)(mem_read16(screen + 0x700u + r7 * 2u) + mem_read16(screen + 0x73Eu));
	OBJECT(player_car)->unk_4E = OBJECT(player_car)->unk_4C;
	OBJECT(player_car)->unk_4C += r5;
	r7 = 0xB78Cu + (uint8_t)(r7 * 2u) * 4u;
	mem_write32(player_car + 0x148u, ROM32(r7));
	mem_write32(player_car + 0x14Cu, ROM32(r7 + 4u));
}

uint32_t sub_00B52F(uint32_t r0, uint16_t r1, uint16_t r2, uint16_t r3)
{
	uint32_t carry = 0;
	uint16_t r4_2 = 0, r5_2 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0;
	uint32_t r1_2 = 0, r4 = 0, r5 = 0, r9 = 0, r13 = 0;

	r13 = mem_read32(player_car + 0x148u);
	r14 = mem_read32(player_car + 0x14Cu);
	if ((int16_t)mem_read16(r13) < 0 && (int16_t)mem_read16(r14) < 0 && OBJECT(player_car)->unk_4C <= AS16(OBJECT(player_car)->unk_44))
	{
		sub_00B4F8();
		r13 = mem_read32(player_car + 0x148u);
		r14 = mem_read32(player_car + 0x14Cu);
	}
	r4 = AS16(OBJECT(player_car)->unk_44);
	r5 = OBJECT(player_car)->unk_4E;
	if (mem_read16(r13) + r5 <= r4)
	{
		r13 += 8u;
		mem_write32(player_car + 0x148u, r13);
	}
	if (mem_read16(r14) + r5 <= r4)
	{
		r14 = r14 + 8u;
		mem_write32(player_car + 0x14Cu, r14);
	}
	if (mem_read8(player_car + 0x147u) != 0u)
	{
		mem_write8(player_car + 0x147u, mem_read8(player_car + 0x147u) - 1u);
		r0 = (r0 * mem_read32(r12 + 8u)) >> 4;
		sub_00B6D1();
		return r0;
	}
	r4_2 = (uint16_t)(r1 - r3);
	r9 = 0u;
	r3 = (uint16_t)(r3 + r1 - 0x8000u);
	r4_2 -= 0x8000u;
	carry = (uint16_t)(0x8000u + r2) < r3;
	r1_2 = carry;
	carry = r4_2 < (uint16_t)(0u - r2 - 0x8000u);
	OBJECT(player_car)->unk_7C = 0u;
	if ((uint8_t)(r1_2 + carry) != 0u)
	{
		OBJECT(player_car)->unk_7C = 0xFFu;
		r0 = (r0 * mem_read32(r12 + 8u)) >> 4;
		sub_00B6D1();
	}
	r15 = mem_read16(r13 + 2u);
	r16 = mem_read16(r13 + 4u);
	r17 = mem_read16(r14 + 2u);
	r18 = mem_read16(r14 + 4u);
	r5_2 = (uint16_t)(r3 - r2);
	r3 += r2;
	carry = r15 < r5_2;
	r1_2 = (uint8_t)(r9 * 2u + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r16 < r5_2;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r3 < r17;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r3 < r18;
	switch ((uint8_t)(r1_2 + r9 + carry))
	{
	case 0:
		sub_00B65F();
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
		sub_00B68A();
		break;
	case 2:
	case 6:
	case 10:
	case 14:
		sub_00B67C();
		break;
	case 4:
	case 12:
		sub_00B66E(r13);
		break;
	case 8:
		sub_00B660(r13);
		break;
	case 16:
		sub_00B698();
		break;
	case 17:
	case 19:
		sub_00B6C3();
		break;
	case 18:
		sub_00B6B5();
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 28:
	case 29:
	case 30:
	case 31:
		sub_00B6A7(r13);
		break;
	case 24:
	case 25:
	case 26:
	case 27:
		sub_00B699(r13);
		break;
	default:
		invalid_jump(rom_00B70C[(uint8_t)(r1_2 + r9 + carry)]);
	}
	r5_2 = (uint16_t)(r4_2 - r2);
	r4_2 += r2;
	carry = r15 < r5_2;
	r1_2 = (uint8_t)(r9 * 2u + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r16 < r5_2;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r4_2 < r17;
	r1_2 = (uint8_t)(r1_2 + r9 + carry);
	r1_2 = (uint8_t)(r1_2 * 2u);
	carry = r4_2 < r18;
	switch ((uint8_t)(r1_2 + r9 + carry))
	{
	case 0:
		sub_00B698();
		return r0;
	case 8:
	case 9:
	case 10:
	case 11:
		sub_00B699(r13);
		return r0;
	case 4:
	case 5:
	case 6:
	case 7:
	case 12:
	case 13:
	case 14:
	case 15:
		sub_00B6A7(r13);
		return r0;
	case 2:
		sub_00B6B5();
		return r0;
	case 1:
	case 3:
		sub_00B6C3();
		return r0;
	default:
		invalid_jump(rom_00B74C[(uint8_t)(r1_2 + r9 + carry)]);
	}
}

void sub_00B65F(void)
{
}

void sub_00B660(uint32_t r13)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 1u);
	mem_write8(player_car + 0x141u, mem_read8(r13 + 6u));
}

void sub_00B66E(uint32_t r13)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 1u);
	mem_write8(player_car + 0x141u, mem_read8(r13 + 7u));
}

void sub_00B67C(void)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 1u);
	mem_write8(player_car + 0x141u, mem_read8(r14 + 6u));
}

void sub_00B68A(void)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 1u);
	mem_write8(player_car + 0x141u, mem_read8(r14 + 7u));
}

void sub_00B698(void)
{
}

void sub_00B699(uint32_t r13)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 2u);
	mem_write8(player_car + 0x142u, mem_read8(r13 + 6u));
}

void sub_00B6A7(uint32_t r13)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 2u);
	mem_write8(player_car + 0x142u, mem_read8(r13 + 7u));
}

void sub_00B6B5(void)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 2u);
	mem_write8(player_car + 0x142u, mem_read8(r14 + 6u));
}

void sub_00B6C3(void)
{
	mem_write8(player_car + 0x140u, mem_read8(player_car + 0x140u) | 2u);
	mem_write8(player_car + 0x142u, mem_read8(r14 + 7u));
}

void sub_00B6D1(void)
{
	uint8_t r0 = 0;
	uint32_t r1 = 0;

	r1 = OBJECT(player_car)->unk_60;
	unk_20E0D0[mem_read8(screen)] = rom_00B704[r1];
	r0 = rom_00B6FC[r1];
	sub_011FC9(r0);
}

void sub_00BFC1(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00BFCC[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		OBJECT(current_object)->unk_40 = OBJECT(player_car)->unk_70 >> 11;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00BFCC[t1]);
	}
	OBJECT(current_object)->unk_0C++;
	r9 = 6u;
	OBJECT(current_object)->unk_3A = 0x44u;
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_30 = 0xF2u;
	OBJECT(current_object)->unk_32 = 0xD6u;
	sub_00F366(r9);
}

void sub_00C017(void)
{
	uint32_t r0 = 0, r9 = 0, r18 = 0, r19 = 0;
	uint32_t stack_4 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C024[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r9 = 0x12u;
		OBJECT(current_object)->unk_3A = 0x48u;
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_20 = 0x118u;
		OBJECT(current_object)->unk_24 = 0xCEu;
		mem_write32(screen + 0xF54u, 0u);
		mem_write16(screen + 0xF58u, 0u);
		mem_write16(screen + 0xF52u, 0u);
		sub_00F366(r9);
		return;
	case 1:
		r18 = mem_read16(AS32(OBJECT(player_car)->unk_00) + 0x2Cu);
		mem_write16(screen + 0xF50u, r18);
		if (r18 != 0u)
		{
			r18 = r18 / 0x88u;
			mem_write16(screen + 0xF50u, r18);
			mem_write32(screen + 0xF54u, mem_read32(screen + 0xF54u) + r18);
			mem_write16(screen + 0xF58u, mem_read16(screen + 0xF58u) + 1u);
			if ((int16_t)mem_read16(screen + 0xF52u) < (int16_t)r18)
				mem_write16(screen + 0xF52u, r18);
		}
		OBJECT(current_object)->unk_30 = OBJECT(current_object)->unk_20;
		OBJECT(current_object)->unk_32 = OBJECT(current_object)->unk_24;
		do
		{
			r19 = 0u;
			divx_pair(0xAu, &r18, &r19);
			OBJECT(current_object)->unk_40 = r19;
			stack_4 = r18;
			r0 = sub_00EAE4(r0);
			r18 = stack_4;
			OBJECT(current_object)->unk_30 -= 8u;
		} while ((uint16_t)r18 != 0u);
		return;
	default:
		invalid_jump(rom_00C024[t1]);
	}
}

void sub_00C0C6(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C0D4[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00C0D4[t1]);
	}
	OBJECT(current_object)->unk_0C++;
	r9 = 0x10u;
	OBJECT(current_object)->unk_3A = 0x47u;
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_30 = 0x127u;
	OBJECT(current_object)->unk_32 = 0xCEu;
	sub_00F366(r9);
}

void sub_00C10D(void)
{
	uint8_t r0_2 = 0, r2 = 0;
	uint32_t r0 = 0, r1 = 0, r2_2 = 0, r9 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C118[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r9 = rom_00D018[OBJECT(player_car)->unk_63];
		OBJECT(current_object)->unk_3A = 0x45u;
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_30 = 0x114u;
		OBJECT(current_object)->unk_32 = 0xD8u;
		sub_00F366(r9);
		return;
	case 1:
		if (mem_read8(player_car + 0x144u) != 0u)
		{
			mem_write8(player_car + 0x144u, mem_read8(player_car + 0x144u) - 1u);
			r0_2 = 1u;
			sub_011FC3(r0_2);
		}
		r0 = OBJECT(player_car)->unk_63;
		r1 = OBJECT(player_car)->unk_62;
		r2 = mem_read8(screen + 0x118u);
		if ((r2 & 0x10u) != 0u && r1 != r0)
		{
			r1 = (uint8_t)(r1 + 1u);
			if (OBJECT(player_car)->unk_70 >= 0xF000u)
				mem_write8(player_car + 0x144u, 0x1Eu);
		}
		if ((r2 & 0x20u) != 0u && r1 != 0u)
		{
			r2_2 = mem_read8(OBJECT(player_car)->unk_54 + r1);
			r1 = (uint8_t)(r1 - 1u);
			if (divide_u32(OBJECT(player_car)->unk_70 * r2_2, mem_read8(OBJECT(player_car)->unk_54 + r1)) >= 0xFFFFu)
				mem_write8(player_car + 0x144u, 0x1Eu);
		}
		OBJECT(player_car)->unk_62 = r1;
		AS8(OBJECT(player_car)->unk_64) = mem_read8(OBJECT(player_car)->unk_54 + r1);
		OBJECT(current_object)->unk_40 = r1;
		if (r1 == r0)
		{
			if (mem_read8(player_car + 0x181u) != 0u || mem_read8(player_car + 0x182u) != 0u)
				OBJECT(current_object)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		r0 = 0u;
		if (OBJECT(player_car)->unk_70 >= 0xFF00u && OBJECT(player_car)->unk_74 != 0u)
		{
			r0 = (uint16_t)(AS16(OBJECT(current_object)->unk_10) + 1u);
			if ((uint8_t)OBJECT(current_object)->unk_80 == 0u && r0 >= 0x96u)
			{
				AS8(OBJECT(current_object)->unk_80) = 0xFFu;
				r21 = sub_011D43();
				mem_write32(r21 + 4u, sub_00C231_ADDRESS);
				mem_write32(r21 + 0x14u, current_object);
				mem_write8(r21 + 9u, r1);
			}
		}
		AS16(OBJECT(current_object)->unk_10) = r0;
		if (mem_read8(player_car + 0x181u) != 0u || mem_read8(player_car + 0x182u) != 0u)
			OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00C118[t1]);
	}
}

void sub_00C231(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = (uint16_t)rom_00C23C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r9 = 0xEu;
		OBJECT(current_object)->unk_3A = 0x46u;
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xD8u;
		sub_00F366(r9);
		return;
	case 1:
		if (OBJECT(player_car)->unk_62 != (uint8_t)(OBJECT(current_object)->unk_08 >> 8u) || OBJECT(player_car)->unk_70 < 0xFC00u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			OBJECT(current_object)->unk_01 = 0u;
			mem_write8(OBJECT(current_object)->unk_14 + 0x80u, 0u);
			mem_write16(OBJECT(current_object)->unk_14 + 0x10u, 0u);
			return;
		}
		r0_2 = (r0 & ~0xFFu) | OBJECT(current_object)->unk_10;
		r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 1u);
		OBJECT(current_object)->unk_10 = r0_2;
		if ((r0_2 & 4u) != 0u)
			return;
		r0_2 = sub_00EAE4(r0_2);
		return;
	default:
		invalid_jump(rom_00C23C[t1]);
	}
}

void sub_00C2AC(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0;
	uint32_t stack_4 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C2B8[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = (r0 & ~0xFFu) | mem_read8(screen + 0x31Fu);
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != (uint8_t)r0)
		{
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			r21 = OBJECT(current_object)->unk_14;
			mem_write8(r21 + 9u, r0);
			mem_write8(r21 + 0xCu, 2u);
		}
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00C2B8[t1]);
	}
	r0 = (r0 & ~0xFFu) | mem_read8(mem_read32(screen + 0x308u) + 0x1Fu);
	t2 = r0;
	r0 = (r0 & ~0xFFu) | (uint8_t)(t2 - 1u);
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r9 = 5u;
	OBJECT(current_object)->unk_3A = 0x43u;
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Du;
	OBJECT(current_object)->unk_30 = 0x2Au;
	OBJECT(current_object)->unk_32 = 0xD8u;
	stack_4 = r0;
	sub_00F366(r9);
	OBJECT(current_object)->unk_40 = stack_4;
	r0 = (r0 & ~0xFFu) | mem_read8(screen + 0x31Fu);
	AT8(OBJECT(current_object)->unk_08, 1) = r0;
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00C342_ADDRESS);
	mem_write8(r21 + 9u, r0);
	OBJECT(current_object)->unk_14 = r21;
	OBJECT(current_object)->unk_0C++;
	r0 = (r0 & ~0xFFu) | mem_read8(screen + 0x31Fu);
	if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != (uint8_t)r0)
	{
		AT8(OBJECT(current_object)->unk_08, 1) = r0;
		r21 = OBJECT(current_object)->unk_14;
		mem_write8(r21 + 9u, r0);
		mem_write8(r21 + 0xCu, 2u);
	}
	r0 = sub_00EAE4(r0);
}

void sub_00C342(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C350[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 4u;
		sub_00F366(r9);
		t2 = ~~(uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_40 = t2;
		r0 = 0x41u;
		if (t2 != 0u)
			r0 = 0x42u;
		OBJECT(current_object)->unk_3A = r0;
		OBJECT(current_object)->unk_30 = 0x1Cu;
		OBJECT(current_object)->unk_32 = 0xD8u;
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		t3 = ~~(uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_40 = t3;
		r0 = 0x41u;
		if (t3 != 0u)
			r0 = 0x42u;
		OBJECT(current_object)->unk_3A = r0;
		OBJECT(current_object)->unk_30 = 0x2Cu;
		OBJECT(current_object)->unk_32 = 0xD8u;
		OBJECT(current_object)->unk_34 = 0x200u;
		OBJECT(current_object)->unk_36 = 0x200u;
		OBJECT(current_object)->unk_10 = 0x28u;
		OBJECT(current_object)->unk_12 = 4u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->unk_30 -= 2u;
		OBJECT(current_object)->unk_34 -= 0x20u;
		OBJECT(current_object)->unk_36 -= 0x20u;
		t4 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t4 - 1u;
		if (t4 == 1u)
		{
			OBJECT(current_object)->unk_3E = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 1:
	case 5:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00C350[t1]);
	}
	t5 = OBJECT(current_object)->unk_12;
	OBJECT(current_object)->unk_12 = t5 - 1u;
	if (t5 == 1u)
	{
		OBJECT(current_object)->unk_12 = 4u;
		OBJECT(current_object)->unk_00 ^= 2u;
	}
	t6 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t6 - 1u;
	if (t6 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_00 &= 0xFDu;
	OBJECT(current_object)->unk_00 &= 0xDFu;
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_10 = 8u;
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_30 -= 2u;
	OBJECT(current_object)->unk_34 -= 0x20u;
	OBJECT(current_object)->unk_36 -= 0x20u;
	t4 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t4 - 1u;
	if (t4 == 1u)
	{
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_0C++;
	}
	r0 = sub_00EAE4(r0);
}

void sub_00C43E(void)
{
	bool L_00C4B3 = false;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r19 = 0, object = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0, target1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C44C[t1];
	do
	{
		target1 = L_00C4B3 ? 0xC4B3u : rom_00C44C[t1];
		switch (target1)
		{
		case 0x00C45Cu:
			r9 = 0x2Du;
			OBJECT(current_object)->unk_3A = 0x4Au;
			OBJECT(current_object)->unk_3E = 8u;
			OBJECT(current_object)->unk_3C = 0x19Eu;
			OBJECT(current_object)->unk_30 = 0xE0u;
			OBJECT(current_object)->unk_32 = 0x16u;
			sub_00F366(r9);
			object = current_object + 0x80u;
			AS16(OBJECT(object - 0x80u)->unk_80) = 0xF8u;
			object += 2u;
			AT16(OBJECT(object - 0x82u)->unk_80, 2) = 0xEu;
			AS16(OBJECT(object - 0x82u)->unk_84) = 0x90u;
			AT8(OBJECT(current_object)->unk_08, 2) = mem_read8(mem_read32(screen + 0x20u) + 0x60u);
			OBJECT(current_object)->unk_0C++;
			return;
		case 0x00C4B3u:
			r19 = current_object + 0x80u;
			r10 = r19 + 6u;
			r0 = mem_read8(screen + 1u);
			r1 = mem_read8(screen + 0x480u + r0);
			if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) == r1)
			{
				r0 = OBJECT(player_car)->unk_04 / 0xAu - OBJECT(player_car)->unk_08;
				sub_00CEA7(r0, r10);
				sub_011A32(r19);
				r0 = sub_00EAE4(r0);
				return;
			}
			AT8(OBJECT(current_object)->unk_08, 2) = r1;
			r1 = (uint8_t)(r1 - 1u);
			OBJECT(current_object)->unk_0C = rom_00D0C0[r1];
			r1 = (uint8_t)(r1 << 3);
			r0 = mem_read16(screen + 0x3C0u + (uint8_t)(r1 + r0) * 2u);
			r1 = OBJECT(player_car)->unk_08;
			OBJECT(player_car)->unk_08 = r0;
			r0 -= r1;
			sub_00CEA7(r0, r10);
			AS16(OBJECT(current_object)->unk_84) = 0xB0u;
			OBJECT(current_object)->unk_10 = 0xB4u;
			OBJECT(current_object)->unk_11 = 0u;
			r0 = (r0 & ~0xFFu) | mem_read8(screen);
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 * 2u);
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + (~mem_read8(screen + 0xA4u) & 1u));
			r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 8u);
			mem_write8(IO_C00074, r0);
		case 0x00C53Bu:
			t2 = AS16(OBJECT(current_object)->unk_10);
			AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
			if (t2 != 1u)
			{
				r19 = current_object + 0x80u;
				sub_011A32(r19);
				r0 = sub_00EAE4(r0);
				return;
			}
			AS16(OBJECT(current_object)->unk_84) = 0x90u;
			OBJECT(current_object)->unk_0C--;
			L_00C4B3 = true;
			break;
		case 0x00C541u:
			r19 = current_object + 0x80u;
			sub_011A32(r19);
			r0 = sub_00EAE4(r0);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_00C4B3);
}

void sub_00C552(void)
{
}

void sub_00C553(void)
{
	uint32_t r9 = 0;

	r9 = 0x2Eu;
	OBJECT(current_object)->unk_3A = 0x4Au;
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_30 = 0x20u;
	OBJECT(current_object)->unk_32 = 0x16u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_04 = sub_00C585_ADDRESS;
	sub_00C585();
}

void sub_00C585(void)
{
	uint32_t r0 = 0, r19 = 0;

	r0 = mem_read8(screen + 0xA5u);
	r19 = rom_00D034[r0];
	sub_011A32(r19);
	r0 = sub_00EAE4(r0);
}

void sub_00C59C(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C5A8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x80u;
		r9 = mem_read8(screen + 2u);
		r9 = (uint16_t)(r9 + 0x28Fu);
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = OBJECT(player_car)->unk_60 + 0x289u;
		r0 = (uint8_t)(0x21u + mem_read8(screen + 2u));
		sub_019402((uint8_t)r0);
		OBJECT(current_object)->unk_0C++;
		if (mem_read16(AS32(OBJECT(player_car)->unk_00) + 0x10u) != 0x1Eu)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		r9 = 0x31u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x4Cu;
		r0 = 0x31u;
		sub_019402((uint8_t)r0);
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		if (mem_read16(AS32(OBJECT(player_car)->unk_00) + 0x10u) != 0x1Eu)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		r9 = 0x31u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x4Cu;
		r0 = 0x31u;
		sub_019402((uint8_t)r0);
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	case 3:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00C5A8[t1]);
	}
	if (mem_read16(AS32(OBJECT(player_car)->unk_00) + 0x10u) != 0u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	r9 = 0x32u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x4Bu;
	OBJECT(current_object)->unk_10 = 0x1Eu;
	r0 = 0x32u;
	sub_019402((uint8_t)r0);
	OBJECT(current_object)->unk_0C++;
	t2 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r0 = sub_00EAE4(r0);
}

void sub_00C65F(void)
{
	uint32_t r0 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = mem_read16(mem_read32(screen + 0x308u) + 0x6Au);
		if (r0 == 0u)
			return;
		AS16(OBJECT(player_car)->unk_12) = r0;
		OBJECT(current_object)->unk_0C++;
		sub_00C68E();
		return;
	case 1:
		if (mem_read8(AS32(OBJECT(player_car)->unk_00) + 0xCu) != 1u)
		{
			OBJECT(current_object)->unk_0C++;
			sub_00C7CD();
			if (mem_read8(mem_read32(screen + 0x20u) + 0x5Eu) == 0u)
				AS16(OBJECT(player_car)->unk_12) = mem_read16(mem_read32(screen + 0x308u) + 0x6Au);
		}
		break;
	case 2:
		sub_00C7CD();
		if (mem_read8(mem_read32(screen + 0x20u) + 0x5Eu) == 0u)
			AS16(OBJECT(player_car)->unk_12) = mem_read16(mem_read32(screen + 0x308u) + 0x6Au);
		break;
	case 3:
		sub_00C7CD();
		r0 = mem_read16(mem_read32(screen + 0x308u) + 0x6Au);
		if (r0 != 0u)
		{
			AS16(OBJECT(player_car)->unk_12) = r0;
			OBJECT(current_object)->unk_0C--;
			mem_write8(player_car + 0x180u, 0u);
			mem_write32(AS32(OBJECT(player_car)->unk_00) + 0xCu, 2u);
		}
		r0 = sub_011975(r0);
		sub_00C70C(r0);
		return;
	default:
		invalid_jump(rom_00C66C[OBJECT(current_object)->unk_0C]);
	}
	r0 = AS16(OBJECT(player_car)->unk_12);
	if (r0 == 0u)
	{
		OBJECT(current_object)->unk_0C++;
		mem_write8(player_car + 0x180u, 0xFFu);
		sub_00C7CD();
		r0 = mem_read16(mem_read32(screen + 0x308u) + 0x6Au);
		if (r0 != 0u)
		{
			AS16(OBJECT(player_car)->unk_12) = r0;
			OBJECT(current_object)->unk_0C--;
			mem_write8(player_car + 0x180u, 0u);
			mem_write32(AS32(OBJECT(player_car)->unk_00) + 0xCu, 2u);
		}
		r0 = sub_011975(r0);
		sub_00C70C(r0);
		return;
	}
	r0 = r0 / 0x1Eu;
	r0 = sub_011975(r0);
	sub_00C70C(r0);
}

void sub_00C68E(void)
{
	uint32_t r9 = 0, object = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->unk_80 = SPRITE_RAM + 0x1u;
	object += 4u;
	OBJECT(object - 0x84u)->unk_84 = 0u;
	object += 4u;
	OBJECT(object - 0x88u)->unk_88 = SPRITE_RAM + 0x1u;
	object += 4u;
	OBJECT(object - 0x8Cu)->unk_8C = 0u;
	object += 4u;
	OBJECT(object - 0x90u)->unk_90 = SPRITE_RAM + 0x1u;
	OBJECT(object - 0x90u)->unk_94 = 0u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_3E = 0u;
	r9 = 3u;
	OBJECT(current_object)->unk_3A = 0x40u;
	OBJECT(current_object)->unk_20 = 0xA9u;
	OBJECT(current_object)->unk_32 = 0x11u;
	sub_00F366(r9);
}

void sub_00C70C(uint32_t r0)
{
	bool L_00C73B = false;
	uint32_t negative = 0;
	uint8_t r1 = 0, r2 = 0;
	uint16_t r3 = 0;
	uint32_t r4 = 0, r18 = 0, r19 = 0;
	uint32_t stack_4 = 0, stack_8 = 0, stack_C = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	OBJECT(current_object)->unk_30 = OBJECT(current_object)->unk_20;
	r18 = current_object + 0x80u;
	r19 = 3u;
	do
	{
		r1 = r0 & 0xFu;
		t1 = mem_read8(r18);
		negative = t1 >> 7;
		r18++;
		r2 = mem_read8(r18);
		r18++;
		r3 = mem_read16(r18);
		r18 += 6u;
		r4 = r0;
		do
		{
			if (L_00C73B || t1 != 0u)
			{
				if (L_00C73B || negative == 0u)
				{
					L_00C73B = false;
					r4 = mem_read32(r18 - 4u);
					t2 = r3;
					r3 = (uint16_t)(t2 - 0x40u);
					mem_write16(r18 - 6u, r3);
					if (t2 != 0x40u)
					{
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->unk_40 = r2;
							OBJECT(current_object)->unk_34 = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							sub_00EAE4(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
					else
					{
						mem_write8(r18 - 8u, 0xFFu);
						mem_write8(r18 - 7u, r1);
						mem_write32(r18 - 4u, r0);
					}
				}
				else
				{
					r3 += 0x40u;
					mem_write16(r18 - 6u, r3);
					if ((uint8_t)r3 != 0u)
					{
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->unk_40 = r2;
							OBJECT(current_object)->unk_34 = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							sub_00EAE4(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
					else
					{
						mem_write8(r18 - 8u, 0u);
						if (r19 == 3u || r4 != 0u)
						{
							OBJECT(current_object)->unk_40 = r2;
							OBJECT(current_object)->unk_34 = r3;
							stack_4 = r0;
							stack_8 = r18;
							stack_C = r19;
							sub_00EAE4(r0);
							r19 = stack_C;
							r18 = stack_8;
							r0 = stack_4;
						}
					}
				}
			}
			else
			{
				if (r2 != r1)
				{
					mem_write8(r18 - 8u, 1u);
					L_00C73B = true;
				}
				if (!L_00C73B)
				{
					if (r19 == 3u || r4 != 0u)
					{
						OBJECT(current_object)->unk_40 = r2;
						OBJECT(current_object)->unk_34 = r3;
						stack_4 = r0;
						stack_8 = r18;
						stack_C = r19;
						sub_00EAE4(r0);
						r19 = stack_C;
						r18 = stack_8;
						r0 = stack_4;
					}
				}
			}
		} while (L_00C73B);
		r0 = r0 >> 4;
		OBJECT(current_object)->unk_30 -= 0x12u;
		r19--;
	} while (r19 != 0u);
}

void sub_00C7CD(void)
{
	uint32_t r21 = 0;

	if (mem_read8(player_car + 0xCDu) < mem_read8(screen + 0x349u))
	{
		mem_write8(player_car + 0xCDu, mem_read8(screen + 0x349u));
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00C7EC_ADDRESS);
	}
}

void sub_00C7EC(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;

	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Fu;
	r9 = 0x33u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x3Fu;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0x30u;
	OBJECT(current_object)->unk_10 = 0x5Au;
	OBJECT(current_object)->unk_12 = 0u;
	OBJECT(current_object)->unk_04 = sub_00C857_ADDRESS;
	r0 = mem_read8(screen);
	r10 = unk_20E000_ADDRESS + r0 * 4u;
	mem_write8(r10 + 0xC3u, 1u);
	mem_write8(r10 + 0xC2u, 0x10u);
	mem_write8(r10 + 0xC1u, 0x10u);
	sub_00C857(r0);
}

void sub_00C857(uint32_t r0)
{
	uint32_t r0_2 = 0;
	uint8_t t1 = 0;

	t1 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	r0_2 = (r0 & ~0xFFu) | OBJECT(current_object)->unk_12;
	r0_2 = (r0_2 & ~0xFFu) | (uint8_t)(r0_2 + 1u);
	OBJECT(current_object)->unk_12 = r0_2;
	if ((r0_2 & 4u) != 0u)
		return;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_00C873(void)
{
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		break;
	default:
		invalid_jump(rom_00C880[OBJECT(current_object)->unk_0C]);
	}
	if ((mem_read32(screen + 0x90u) & 0x100u) != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	if ((int16_t)OBJECT(current_object)->unk_1C <= (int16_t)AS16(OBJECT(player_car)->unk_44))
	{
		mem_write8(screen + 0xA4u, mem_read8(screen + 0xA2u));
		mem_write8(screen + 0xA6u, mem_read8(screen + 0xA3u));
		OBJECT(player_car)->unk_4A = OBJECT(player_car)->unk_48;
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
}

void sub_00C8B5(void)
{
}

void sub_00C8B6(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r19 = 0, r21 = 0, ignored = 0, object = 0;
	uint16_t t2 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00C8C4[t1];
	switch (t1)
	{
	case 0:
		if ((int16_t)AS16(OBJECT(player_car)->unk_44) < (int16_t)OBJECT(current_object)->unk_1C)
			return;
		mem_write8(player_car + 0x181u, 0xFFu);
		OBJECT(current_object)->unk_0C++;
	case 1:
		if (mem_read8(mem_read32(screen + 0x24u) + 0x5Eu) == 0u)
			return;
		mem_write8(player_car + 0x181u, 0xFFu);
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Cu;
		r9 = 0x2F5u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x4Cu;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x90u;
		object = current_object + 0x80u;
		AS16(OBJECT(object - 0x80u)->unk_80) = 0xB0u;
		object += 2u;
		AT16(OBJECT(object - 0x82u)->unk_80, 2) = 0x30u;
		object += 2u;
		AS16(OBJECT(object - 0x84u)->unk_84) = 0x90u;
		r0 = mem_read16(screen + 0x410u + mem_read8(screen + 1u) * 2u);
		object += 2u;
		sub_00CEA7(r0, object);
		r0_2 = 0x34u;
		if (mem_read8(screen + 0x31Fu) != 0u)
			r0_2 = 0x35u;
		sub_019402(r0_2);
		r0 = 0x7Fu;
		sub_011FEA((uint8_t)r0);
		OBJECT(current_object)->unk_10 = 0x3Cu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_10 = 0x3Cu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 != 1u)
			return;
		OBJECT(current_object)->unk_10 = 0x3Cu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 3:
		break;
	case 4:
		t3 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00EAE4(r0);
		r19 = 0xD08Eu;
		sub_011A32(r19);
		r19 = current_object + 0x80u;
		ignored = sub_011A32(r19);
		return;
	case 5:
		t4 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t4 - 1u;
		if (t4 != 1u)
		{
			r19 = 0xD08Eu;
			sub_011A32(r19);
			r19 = current_object + 0x80u;
			ignored = sub_011A32(r19);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_00C8C4[t1]);
	}
	t5 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t5 - 1u;
	if (t5 != 1u)
	{
		sub_00EAE4(r0);
		r19 = 0xD08Eu;
		sub_011A32(r19);
		r19 = current_object + 0x80u;
		ignored = sub_011A32(r19);
		return;
	}
	r10 = 0xD0AEu;
	sub_01930A(r10);
	r0 = (r0 & ~0xFFu) | 0x10u;
	sub_011EB8((uint8_t)r0);
	if (mem_read8(mem_read32(screen + 0x308u) + 0x1Fu) != 1u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00C9ED_ADDRESS);
		OBJECT(current_object)->unk_0C++;
	}
	OBJECT(current_object)->unk_10 = 0xF0u;
	OBJECT(current_object)->unk_11 = 0u;
	OBJECT(current_object)->unk_0C++;
	t3 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t3 - 1u;
	if (t3 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	sub_00EAE4(r0);
	r19 = 0xD08Eu;
	sub_011A32(r19);
	r19 = current_object + 0x80u;
	ignored = sub_011A32(r19);
}

void sub_00C9ED(void)
{
	uint16_t r0 = 0;
	uint32_t r9 = 0, r21 = 0;
	uint8_t t1 = 0;

	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00CA50_ADDRESS);
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Cu;
	r9 = 0x2F8u;
	sub_00F366(r9);
	t1 = ~~mem_read8(screen + 0x31Fu);
	OBJECT(current_object)->unk_40 = t1;
	r0 = 0x41u;
	if (t1 != 0u)
		r0 = 0x42u;
	OBJECT(current_object)->unk_3A = r0;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0xC8u;
	OBJECT(current_object)->unk_10 = 0xF0u;
	OBJECT(current_object)->unk_11 = 0u;
	OBJECT(current_object)->unk_04 = sub_00CA44_ADDRESS;
	sub_00CA44();
}

void sub_00CA44(void)
{
	uint32_t ignored = 0;
	uint16_t t1 = 0;

	t1 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	ignored = sub_00EAE4(0u);
}

void sub_00CA50(void)
{
	uint32_t r9 = 0;
	uint8_t t1 = 0;

	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Cu;
	t1 = ~~mem_read8(screen + 0x31Fu);
	OBJECT(current_object)->unk_40 = t1;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0x70u;
	if (t1 != 0u)
	{
		r9 = 0x2F7u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x42u;
		OBJECT(current_object)->unk_10 = 0xF0u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_04 = sub_00CA44_ADDRESS;
		sub_00CA44();
		return;
	}
	r9 = 0x2F6u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x41u;
	OBJECT(current_object)->unk_10 = 0xF0u;
	OBJECT(current_object)->unk_11 = 0u;
	OBJECT(current_object)->unk_04 = sub_00CAA8_ADDRESS;
	sub_00CAA8();
}

void sub_00CAA8(void)
{
	uint32_t ignored = 0;
	uint16_t t1 = 0;

	t1 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	if ((OBJECT(current_object)->unk_10 & 4u) == 0u)
		return;
	ignored = sub_00EAE4(0u);
}

void sub_00CABC(void)
{
	uint8_t r0 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0_3 = 0, r9 = 0, r10 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = 0x3Au;
		if (mem_read8(screen + 0x31Fu) == 0u)
			r0 = 0x33u;
		sub_0193FC(r0);
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Du;
		r10 = 0xCB98u;
		r9 = 0x34u;
		if (mem_read8(player_car + 0xC9u) != 0u)
		{
			r10 = 0xCBACu;
			r9 = 0x35u;
		}
		mem_write32(screen + 0xF40u, r10);
		sub_00F366(r9);
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xD0u;
		OBJECT(current_object)->unk_18 = 0xD0A0u;
		AS16(OBJECT(current_object)->unk_10) = mem_read16(mem_read32(screen + 0x20u) + 0x42u) + 0x39u;
		OBJECT(current_object)->unk_0C++;
	case 1:
		if (mem_read8(player_car + 0x182u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		if ((int16_t)mem_read16(mem_read32(screen + 0x20u) + 0x42u) < (int16_t)AS16(OBJECT(current_object)->unk_10))
		{
			r10 = OBJECT(current_object)->unk_18;
			r0_3 = (int16_t)mem_read16(r10);
			r10 += 2u;
			if ((int16_t)r0_3 < 0)
			{
				r10 += r0_3;
				r0_3 = mem_read16(r10);
				r10 += 2u;
			}
			OBJECT(current_object)->unk_3A = r0_3;
			OBJECT(current_object)->unk_18 = r10;
			r0_3 = sub_00EAE4(r0_3);
			return;
		}
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00CAC8[OBJECT(current_object)->unk_0C]);
	}
	r0_2 = (uint16_t)(OBJECT(current_object)->unk_32 + 3u);
	if ((int16_t)r0_2 > 0x140)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	OBJECT(current_object)->unk_32 = r0_2;
	r0_2 = (uint16_t)(OBJECT(current_object)->unk_34 + 6u);
	OBJECT(current_object)->unk_34 = r0_2;
	OBJECT(current_object)->unk_36 = r0_2;
	r10 = OBJECT(current_object)->unk_18;
	r0_3 = (int16_t)mem_read16(r10);
	r10 += 2u;
	if ((int16_t)r0_3 < 0)
	{
		r10 += r0_3;
		r0_3 = mem_read16(r10);
		r10 += 2u;
	}
	OBJECT(current_object)->unk_3A = r0_3;
	OBJECT(current_object)->unk_18 = r10;
	r0_3 = sub_00EAE4(r0_3);
}

void sub_00CBC0(void)
{
	bool L_00CCDE = false, L_00CCFC = false;
	uint8_t r0_2 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint8_t t8 = 0, t9 = 0;
	uint16_t t1 = 0, t4 = 0;
	uint32_t target1 = 0, t2 = 0, t3 = 0, t5 = 0, t6 = 0, t7 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r10 = mem_read32(screen + 0x308u) + 0x1Fu;
		r9 = mem_read8(r10);
		r10++;
		do
		{
			r0 = mem_read8(r10);
			mem_write32(current_object + 0x80u + r0 * 4u, rom_00CFC8[mem_read8(0x20FC49u + (r0 << 5) * 4u)]);
			r10++;
			r9--;
		} while (r9 != 0u);
		r10 = screen + 0x480u + mem_read8(screen + 1u);
		AT8(OBJECT(current_object)->unk_08, 2) = mem_read8(r10);
		OBJECT(current_object)->unk_18 = r10;
		OBJECT(current_object)->unk_0F = 0u;
		OBJECT(current_object)->unk_0C++;
		r0_2 = mem_read8(OBJECT(current_object)->unk_18);
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) == r0_2)
			return;
		AT8(OBJECT(current_object)->unk_08, 2) = r0_2;
		AT8(OBJECT(current_object)->unk_08, 3) = r0_2 - 1u;
		AS16(OBJECT(current_object)->unk_10) = AS16(OBJECT(player_car)->unk_44) + 0xC0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		r0_2 = mem_read8(OBJECT(current_object)->unk_18);
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) == r0_2)
			return;
		AT8(OBJECT(current_object)->unk_08, 2) = r0_2;
		AT8(OBJECT(current_object)->unk_08, 3) = r0_2 - 1u;
		AS16(OBJECT(current_object)->unk_10) = AS16(OBJECT(player_car)->unk_44) + 0xC0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00CBCC[OBJECT(current_object)->unk_0C]);
	}
	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
	OBJECT(current_object)->unk_12 = 0u;
	OBJECT(current_object)->unk_13 = mem_read8(screen + 0x35Au + r0);
	OBJECT(current_object)->unk_14 = screen + 0x390u + r0 * 2u * 4u;
	do
	{
		r21 = mem_read8(OBJECT(current_object)->unk_14);
		do
		{
			do
			{
				target1 = L_00CCFC ? 0xCCECu : L_00CCFC ? 0xCCECu : rom_00CC70[mem_read8(current_object + 0xF0u + r21)];
				switch (target1)
				{
				case 0x00CC84u:
					OBJECT(current_object)->unk_0F++;
					r0 = mem_read16(screen + 0x3C0u + (((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 24u) << 3) + r21) * 2u);
					r1 = r21;
					if (OBJECT(current_object)->unk_12 != 0u)
					{
						t1 = r0 - OBJECT(current_object)->unk_1C;
						r0 = t1;
						if ((int16_t)t1 < 0)
							r0 = 0u;
						sub_00CEF3(r0, r1);
					}
					else
					{
						OBJECT(current_object)->unk_1C = r0;
						sub_00CE99(r0, r1);
					}
					r0 = mem_read8(screen + 1u);
					if (r0 != r21)
					{
						L_00CCDE = mem_read8(current_object + 0xF0u + r0) != 0u;
						if (!L_00CCDE)
						{
							mem_write16(current_object + 0xA0u + r21 * 2u, 0u);
							mem_write8(current_object + 0xF0u + r21, 2u);
							L_00CCFC = true;
						}
					}
					if (!L_00CCDE)
					{
						if (L_00CCFC)
							break;
						OBJECT(current_object)->unk_0E = 4u;
						OBJECT(current_object)->unk_0D = 8u;
					}
					L_00CCDE = false;
					mem_write16(current_object + 0xA0u + r21 * 2u, 0x100u);
					t2 = current_object + 0xF0u + r21;
					mem_write8(t2, mem_read8(t2) + 1u);
				case 0x00CCECu:
					if (!L_00CCFC)
					{
						if (!L_00CCFC)
						{
							t3 = current_object + 0xA0u + r21 * 2u;
							t4 = mem_read16(t3);
							mem_write16(t3, t4 - 0x10u);
						}
					}
					if (!L_00CCFC && !L_00CCFC && t4 != 0x10u)
						sub_00CD8B(r21);
					else
					{
						if (!L_00CCFC)
						{
							if (!L_00CCFC)
							{
								t5 = current_object + 0xF0u + r21;
								mem_write8(t5, mem_read8(t5) + 1u);
							}
						}
						L_00CCFC = false;
						if ((OBJECT(current_object)->unk_10 | OBJECT(current_object)->unk_11) == 0u || mem_read8(player_car + 0x182u) != 0u)
						{
							t6 = current_object + 0xF0u + r21;
							mem_write8(t6, mem_read8(t6) + 1u);
							r0_3 = (uint16_t)(mem_read16(current_object + 0xA0u + r21 * 2u) - 0x10u);
							mem_write16(current_object + 0xA0u + r21 * 2u, r0_3);
							if (r0_3 != 0xFF00u)
								sub_00CD8B(r21);
							else
							{
								OBJECT(current_object)->unk_0F--;
								t7 = current_object + 0xF0u + r21;
								mem_write8(t7, mem_read8(t7) + 1u);
								sub_00CD8B(r21);
							}
						}
						else if (mem_read8(screen + 1u) != r21)
							sub_00CD8B(r21);
						else
						{
							r0 = OBJECT(current_object)->unk_0E;
							t8 = OBJECT(current_object)->unk_0D;
							OBJECT(current_object)->unk_0D = t8 - 1u;
							if (t8 == 1u)
							{
								r0 = r0 ^ 4u;
								OBJECT(current_object)->unk_0E = r0;
								OBJECT(current_object)->unk_0D = 4u + r0;
							}
							if (r0 != 0u)
								sub_00CD8B(r21);
						}
					}
					break;
				case 0x00CCFCu:
					L_00CCFC = true;
					break;
				case 0x00CD32u:
					r0_3 = (uint16_t)(mem_read16(current_object + 0xA0u + r21 * 2u) - 0x10u);
					mem_write16(current_object + 0xA0u + r21 * 2u, r0_3);
					if (r0_3 == 0xFF00u)
					{
						OBJECT(current_object)->unk_0F--;
						t7 = current_object + 0xF0u + r21;
						mem_write8(t7, mem_read8(t7) + 1u);
					}
					sub_00CD8B(r21);
					break;
				case 0x00CD55u:
					break;
				default:
					invalid_jump(target1);
				}
			} while (L_00CCFC);
		} while (L_00CCFC);
		OBJECT(current_object)->unk_14++;
		OBJECT(current_object)->unk_12++;
		t9 = OBJECT(current_object)->unk_13;
		OBJECT(current_object)->unk_13 = t9 - 1u;
	} while (t9 != 1u);
	r0 = AS16(OBJECT(current_object)->unk_10);
	if (r0 != 0u && (int16_t)r0 <= (int16_t)mem_read16(mem_read32(screen + 0x20u) + 0x42u))
	{
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_11 = 0u;
	}
	if (OBJECT(current_object)->unk_0F == 0u)
	{
		r10 = current_object + 0xF0u;
		mem_write32(r10, 0u);
		mem_write32(r10 + 4u, 0u);
		OBJECT(current_object)->unk_0C--;
	}
}

void sub_00CD8B(uint32_t r21)
{
	uint16_t r5 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r10 = 0, r18 = 0;

	r10 = 0xCF88u + OBJECT(current_object)->unk_12 * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->unk_30 = r0;
	OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
	r9 = rom_00CFE8[mem_read8(unk_20E030_ADDRESS + r21)];
	OBJECT(current_object)->unk_3A = 0x27u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_3E = 8u;
	sub_00F366(r9);
	sub_00EAE4(r0);
	r10 = 0xCF68u + OBJECT(current_object)->unk_12 * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->unk_30 = r0;
	OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
	r10 = current_object + 0x80u + r21 * 4u;
	r9 = mem_read16(r10);
	OBJECT(current_object)->unk_3A = mem_read16(r10 + 2u);
	OBJECT(current_object)->unk_3C = 0x19Du;
	OBJECT(current_object)->unk_3E = 0u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_40 = 4u;
	OBJECT(current_object)->unk_34 = 0x40u;
	OBJECT(current_object)->unk_36 = 0x40u;
	sub_00EAE4(r0);
	r0 = OBJECT(current_object)->unk_12;
	r3 = rom_00CFF8[r0];
	r3 = (r3 & ~0xFFFFu) | (uint16_t)(r3 + mem_read16(current_object + 0xA0u + r21 * 2u));
	r5 = 0xC0u;
	if (r0 != 0u)
		r5 = 0x90u;
	r18 = current_object + 0xB0u + r21 * 2u * 4u;
	sub_0119FA(r3, r5, r18);
	r0 = OBJECT(current_object)->unk_12;
	if (r0 == 0u)
		return;
	r10 = 0xCFA8u + r0 * 4u;
	r0 = (uint16_t)(ROM16(r10) + mem_read16(current_object + 0xA0u + r21 * 2u));
	OBJECT(current_object)->unk_30 = r0;
	OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
	r9 = 0x36u;
	OBJECT(current_object)->unk_3A = 0x4Au;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_3E = 8u;
	sub_00F366(r9);
	r0 = sub_00EAE4(r0);
}

void sub_00CE99(uint32_t r0, uint32_t r1)
{
	uint32_t r10 = 0;

	r10 = current_object + 0xB0u + r1 * 2u * 4u;
	sub_00CEA7(r0, r10);
}

void sub_00CEA4(uint32_t r0, uint32_t r10)
{
	r0 = r0 / 0xAu;
	sub_00CEA7(r0, r10);
}

void sub_00CEA7(uint32_t r0, uint32_t r10)
{
	uint32_t r1 = 0;

	r10 += 7u;
	mem_write8(r10, 0u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r10--;
	mem_write8(r10, 0x22u);
	r1 = 0u;
	divx_pair(0xAu, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r1 = 0u;
	divx_pair(6u, &r0, &r1);
	r10--;
	mem_write8(r10, r1 + 0x30u);
	r10--;
	mem_write8(r10, 0x27u);
	mem_write8(r10 - 1u, r0 + 0x30u);
}

void sub_00CEF3(uint32_t r0, uint32_t r1)
{
	uint32_t zero = 0;
	uint8_t r2 = 0;
	uint32_t r1_2 = 0, r10 = 0;

	r10 = current_object + 0xB7u + r1 * 2u * 4u;
	r2 = 0x30u;
	mem_write8(r10, 0u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	r10--;
	mem_write8(r10, r1_2 + 0x30u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	r10--;
	mem_write8(r10, r1_2 + 0x30u);
	r10--;
	mem_write8(r10, 0x22u);
	r1_2 = 0u;
	divx_pair(0xAu, &r0, &r1_2);
	zero = (uint32_t)(r0 == 0u);
	if (zero == 0u)
	{
		r10--;
		mem_write8(r10, r1_2 + r2);
		r1_2 = 0u;
		divx_pair(6u, &r0, &r1_2);
		zero = (uint32_t)(r0 == 0u);
		if (zero)
		{
			r10--;
			mem_write8(r10, r1_2 + r2);
			mem_write16(r10 - 2u, 0x2020u);
			return;
		}
		r10--;
		mem_write8(r10, r1_2 + r2);
		r10--;
		mem_write8(r10, 0x27u);
		mem_write8(r10 - 1u, r0 + r2);
		return;
	}
	r10--;
	mem_write8(r10, r1_2 + r2);
	r10--;
	mem_write8(r10, 0x20u);
	mem_write16(r10 - 2u, 0x2020u);
}

void sub_00D0C8(void)
{
	uint32_t r1 = 0;

	if (unk_20E623 == 0u)
	{
		sub_00D13A();
		return;
	}
	mem_write16(0x801000u, 0u);
	r12 = 0x800000u;
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	move_halfwords(unk_20E040_ADDRESS, 0x100800u, 0x20u);
	move_halfwords(r12 + 4u, 0x100800u, 0x10u);
	move_halfwords(r12 + 0x600u, 0x100800u, 0x400u);
	move_halfwords(unk_20FB00_ADDRESS, 0x100800u, 0x80u);
	move_string(0x100800u, 0x200u, 0x20FC00u, 0x200u, 2u, 0u, 0u, r26, &r27, &r28);
	sub_00D2BE();
}

void sub_00D13A(void)
{
	uint32_t r2 = 0, r19 = 0, ignored = 0, left_caller = 0;
	uint8_t t4 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	mem_write16(0x801000u, 0u);
	r12 = 0x800000u;
	move_halfwords(unk_20E040_ADDRESS, 0x100800u, 0x20u);
	move_halfwords(r12, 0x100800u, 8u);
	move_halfwords(unk_20FB00_ADDRESS, 0x100800u, 0x80u);
	move_halfwords(WORK_RAM + 0xFC00u, 0x100800u, 0x200u);
	unk_20E042 = unk_20E620;
	if (unk_20E042 != 0u)
	{
		sub_00D2F0();
		return;
	}
	mem_write8(0x801000u, 0u);
	r19 = 0xE10Cu;
	if (unk_20E621 == 0u)
		r19 = 0xE128u;
	if (unk_20E623 != 0u)
		r19 = 0xE143u;
	sub_011874(r19);
	r19 = 0xE15Eu;
	sub_011874(r19);
	left_caller = 0u;
	left_caller = sub_00D524(left_caller);
	if (left_caller != 0u)
		return;
	mem_write16(r12, 0x56u);
	mem_write16(r12 + 2u, 0x37u);
	mem_write16(r12 + 4u, 0x30u);
	mem_write8(r12 + 0x14u, 0u);
	mem_write16(0x801002u, 0u);
	mem_write16(0x801000u, 1u);
	left_caller = 0u;
	sub_00D5E0(&ignored, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	sub_00D664(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	t1 = mem_read32(0x801002u);
	mem_write32(0x801002u, t1 | 1u);
	r2 = sub_0008F0(r2);
	left_caller = 0u;
	sub_00D5E0(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	mem_write8(r12 + 0xCu, 0u);
	left_caller = 0u;
	sub_00D6BB(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	if (unk_20E623 != 0u)
	{
		sub_00D2BE();
		return;
	}
	mem_write8(r12 + 4u, unk_20E621);
	t2 = mem_read32(0x801002u);
	mem_write32(0x801002u, t2 & ~1u);
	r2 = sub_0008F0(r2);
	left_caller = 0u;
	sub_00D5E0(&r2, &left_caller);
	if (left_caller != 0u)
		return;
	left_caller = 0u;
	left_caller = sub_00D6AD(r2, left_caller);
	if (left_caller != 0u)
		return;
	t3 = mem_read32(0x801002u);
	mem_write32(0x801002u, t3 | 1u);
	do
	{
		r2 = sub_0008F0(r2);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			return;
		}
		current_object = 0xE1B6u;
		sub_00D6EC();
		t4 = mem_read8(0x801002u);
	} while ((int8_t)t4 >= 0);
	sub_00D716();
	mem_write8(r12 + 0xE20u, mem_read8(r12 + 2u));
	mem_write8(r12 + 6u, 0xFFu);
	r19 = 0xE1D6u;
	sub_011874(r19);
	do
	{
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			return;
		}
		if (mem_read8(r12 + 0xCu) != 0u)
		{
			sub_00D2EB();
			return;
		}
	} while (mem_read8(r12 + 8u) == 0u);
	sub_00D716();
	unk_20E046 = 0u;
	sub_00D298();
}

void sub_00D298(void)
{
	uint8_t r1 = 0, r2 = 0;
	uint32_t r0 = 0;

	r1 = mem_read8(r12);
	unk_20E040 = r1;
	r1 = (uint8_t)(r1 * 2u);
	unk_20E041 = r1;
	r0 = mem_read8(r12 + 2u);
	mem_write8(r12 + 0x1002u, 1u);
	if (unk_20E623 == 0u && r1 < 2u)
	{
		sub_00D2F0();
		return;
	}
	r2 = (uint8_t)(unk_20E622 * 2u);
	unk_208002 = r2;
	unk_20FB02 = r2;
	r2++;
	unk_218002 = r2;
	unk_20FB82 = r2;
	sub_00D341(r0);
}

void sub_00D2BE(void)
{
	uint32_t r1 = 0, left_caller = 0;

	mem_write16(0x801000u, 0u);
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	mem_write8(r12 + 0x14u, 1u);
	mem_write8(0x801002u, 0u);
	mem_write8(0x801000u, 1u);
	left_caller = 0u;
	left_caller = sub_00D612(left_caller);
	if (left_caller != 0u)
		return;
	sub_00D298();
}

void sub_00D2EB(void)
{
	unk_20E043 = 1u;
	sub_00D2F0();
}

void sub_00D2F0(void)
{
	uint32_t r0 = 0;

	unk_20E042 = 1u;
	r0 = 1u;
	unk_20E041 = 2u;
	unk_20E040 = 1u;
	unk_208002 = 0u;
	unk_20FB02 = 0u;
	unk_218002 = 1u;
	unk_20FB82 = 1u;
	sub_00D341(r0);
}

void sub_00D341(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r19 = 0, ignored = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	unk_20FB00 = r0;
	unk_20FB80 = r0;
	r0 = (r0 & ~0xFFu) | (uint8_t)(r0 - 1u);
	r0 = (r0 & ~0xFFu) | (uint8_t)(r0 * 2u);
	unk_20FB01 = r0;
	r1 = r0 + 1u;
	unk_20FB81 = r1;
	unk_208001 = r0;
	unk_218001 = r1;
	unk_208020 = rom_00E0A0[r0] + unk_20E000_ADDRESS;
	unk_218020 = rom_00E0A0[r1] + unk_20E000_ADDRESS;
	r19 = unk_20FB00_ADDRESS;
	unk_208024 = unk_20FB00_ADDRESS;
	unk_218024 = unk_20FB80_ADDRESS;
	if ((uint8_t)r0 == 0u)
	{
		unk_20FB03 = unk_20E60D;
		unk_20FB04 = unk_20E624;
		unk_20FB05 = unk_20E610;
		unk_20FB06 = unk_20E611;
		unk_20FB07 = unk_20E613;
		unk_20FB08 = unk_20E614;
		unk_20FB09 = unk_20E615;
		unk_20FB0A = unk_20E630;
		unk_20FB0B = unk_20E632;
		unk_20FB0D = unk_20E634;
		move_halfwords(r19 + 0x30u, SOUND_RAM + 0x1F32u, 0x1Eu);
	}
	sub_01076A();
	r19 = 0xDF84u;
	sub_011874(r19);
	if (unk_20E043 != 0u)
	{
		r19 = 0xDFCEu;
		sub_011874(r19);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	if (unk_20E623 != 0u)
	{
		r19 = 0xE055u;
		sub_011874(r19);
		r19 = 0xDF98u;
		sub_011874(r19);
		r19 = 0xDFB4u;
		sub_011874(r19);
		t1 = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (t1 + 0x30u) | 0x6000u);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	if (unk_20E042 != 0u)
	{
		r19 = 0xDFEAu;
		sub_011874(r19);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	t2 = mem_read8(0x800004u);
	if (t2 == 0u)
	{
		r19 = 0xE034u;
		sub_011874(r19);
		r19 = 0xDF98u;
		sub_011874(r19);
		r19 = 0xDFB4u;
		sub_011874(r19);
		t3 = mem_read8(0x800002u);
		mem_write16(VIDEO_31F736, (t3 + 0x30u) | 0x6000u);
		r0_2 = 0x80u;
		do
		{
			stack_4 = r0_2;
			ignored = sub_0008F0(ignored);
			r0_2 = stack_4 - 1u;
		} while (r0_2 != 0u);
		sub_01076A();
		return;
	}
	r19 = 0xE00Cu;
	sub_011874(r19);
	r19 = 0xDF98u;
	sub_011874(r19);
	r19 = 0xDFB4u;
	sub_011874(r19);
	t4 = mem_read8(0x800002u);
	mem_write16(VIDEO_31F736, (t4 + 0x30u) | 0x6000u);
	r0_2 = 0x80u;
	do
	{
		stack_4 = r0_2;
		ignored = sub_0008F0(ignored);
		r0_2 = stack_4 - 1u;
	} while (r0_2 != 0u);
	sub_01076A();
}

uint32_t sub_00D524(uint32_t left_caller)
{
	uint8_t r1 = 0;
	uint32_t r0 = 0, r10 = 0, r19 = 0, ignored = 0, left_caller_2 = 0;

	r10 = 0x800000u;
	r0 = 0x7FFu;
	r0 = sub_00D544(r0, r10);
	r1 = 2u;
	if ((uint8_t)r0 != 0u)
		r1 = 3u;
	if (r1 == 3u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r19 = 0xE422u;
		sub_00D7CB(r19, &ignored, &left_caller_2);
		return left_caller_2;
	}
	return left_caller;
}

uint32_t sub_00D544(uint32_t r0, uint32_t r10)
{
	uint32_t carry = 0;
	uint8_t r2 = 0, r3 = 0, r6 = 0;
	uint16_t r5 = 0;
	uint32_t r1 = 0, r4 = 0, r7 = 0;
	uint8_t t5 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;

	r1 = r0;
	r2 = 1u;
	r0 &= ~0xFFu;
	do
	{
		r7 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			mem_write16(r7, r3);
			r7 += 2u;
			r6 = 6u;
			t1 = r7 * 2u;
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
		r7 = r10;
		r3 = (uint8_t)r2;
		r4 = r1;
		do
		{
			t5 = mem_read8(r7);
			r7++;
			if (t5 != r3)
			{
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ r3);
				r0 = (r0 & ~0xFFu) | (uint8_t)(r0 ^ mem_read8(r7 - 1u));
			}
			r7++;
			r6 = 6u;
			t6 = r7 * 2u;
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
		sub_00D72D();
		carry = v60_rot_cy(r2, 1u, 8);
		r2 = (uint8_t)(((uint32_t)r2 << 1) | (r2 >> 7));
	} while (carry == 0u);
	return r0;
}

void sub_00D5E0(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r19 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0;

	unk_20E046 = 0xE10u;
	do
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			*p_left_caller = 1u;
			return;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t1 = unk_20E046;
		unk_20E046 = t1 - 1u;
		if (t1 == 1u)
		{
			mem_write8(IO_C0001C, 4u);
			mem_write8(0xC8001Cu, 0x80u);
			r19 = 0xE484u;
			sub_00D7CB(r19, p_r2, p_left_caller);
			return;
		}
		t2 = mem_read32(0x801002u);
	} while ((t2 & 0x80u) == 0u);
	sub_00D716();
}

uint32_t sub_00D612(uint32_t left_caller)
{
	uint32_t ignored = 0, left_caller_2 = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	do
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t1 = mem_read8(0x800000u);
	} while (t1 == 0u);
	unk_20E046 = 0x3Cu;
	do
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E700 != 0u)
		{
			unk_20E043 = 0xFFu;
			left_caller_2 = 1u;
			return left_caller_2;
		}
		current_object = 0xE170u;
		sub_00D6EC();
		t2 = unk_20E046;
		unk_20E046 = t2 - 1u;
	} while (t2 != 1u);
	sub_00D716();
	return left_caller;
}

void sub_00D658(void)
{
	uint32_t r0 = 0;

	r0 = 0x3000u;
	do
	{
		r0--;
	} while (r0 != 0u);
}

void sub_00D664(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r0 = 0, r19 = 0;

	r0 = 0x10u + r12;
	*p_r2 = ((((uint32_t)(uint8_t)mem_read16(r0) << 8) | (uint8_t)mem_read16(r0 + 2u)) << 8) | (uint8_t)mem_read16(r0 + 4u);
	if ((((((uint32_t)(uint8_t)mem_read16(r12) << 8) | (uint8_t)mem_read16(r12 + 2u)) << 8) | (uint8_t)mem_read16(r12 + 4u)) != 0x563730u || *p_r2 != 0x5A3830u)
	{
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r19 = 0xE422u;
		sub_00D7CB(r19, p_r2, p_left_caller);
		return;
	}
}

uint32_t sub_00D6AD(uint32_t r2, uint32_t left_caller)
{
	uint32_t r0 = 0, r2_2 = 0, r19 = 0, ignored = 0, left_caller_2 = 0;

	r2_2 = (r2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)r2_2 == 0u)
	{
		r0 = 5u;
		sub_00D6C7(r0, r2_2);
		return left_caller;
	}
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
	sub_01076A();
	r19 = 0xE464u;
	sub_011868(r19);
	left_caller_2 = 0u;
	sub_00D6BB(&r2_2, &left_caller_2);
	if (left_caller_2 != 0u)
		return left_caller_2;
	r0 = 5u;
	r2_2 = (r2_2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	sub_00D6C7(r0, r2_2);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	sub_00D884(&ignored, &left_caller_2);
	return left_caller_2;
}

void sub_00D6BB(uint32_t *p_r2, uint32_t *p_left_caller)
{
	uint32_t r2 = *p_r2;
	uint32_t r0 = 0, r2_2 = 0, r19 = 0, left_caller_2 = 0;

	r2_2 = (r2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	if ((uint8_t)r2_2 == 0u)
	{
		r0 = 4u;
		sub_00D6C7(r0, r2_2);
		*p_r2 = r2_2;
		return;
	}
	mem_write8(IO_C0001C, 4u);
	mem_write8(0xC8001Cu, 0x80u);
	sub_01076A();
	r19 = 0xE464u;
	sub_011868(r19);
	r2_2 = (r2_2 & ~0xFFu) | mem_read8(r12 + 0xCu);
	r0 = 4u;
	sub_00D6C7(r0, r2_2);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	sub_00D884(&r2_2, &left_caller_2);
	*p_r2 = r2_2;
	*p_left_caller = left_caller_2;
}

void sub_00D6C7(uint32_t r0, uint32_t r2)
{
	uint32_t r1 = 0, r3 = 0, r19 = 0;

	r1 = 0u;
	do
	{
		r3 = (uint8_t)(r1 * 2u);
		if ((r2 & shl32(1u, (int)(r1))) != 0u)
			r3 = (uint8_t)(r3 + 1u);
		r19 = rom_00E4D0[r3];
		sub_011874(r19);
		r1 = (uint8_t)(r1 + 1u);
		r0--;
	} while (r0 != 0u);
}

void sub_00D6EC(void)
{
	uint16_t r18 = 0;
	uint32_t r19 = 0;

	unk_20E048++;
	if ((unk_20E048 & 0xFu) != 0u)
		return;
	r19 = 0x310000u | 0xF000u | AS16(OBJECT(current_object)->unk_00);
	current_object = current_object + 2u;
	r18 = (uint16_t)((uint32_t)OBJECT(current_object - 0x2u)->unk_02 << 8);
	current_object = current_object + 1u;
	for (;;)
	{
		r18 = (uint16_t)((r18 & ~0xFFu) | mem_read8(current_object));
		current_object = current_object + 1u;
		if ((uint8_t)r18 == 0u)
			break;
		mem_write16(r19, mem_read16(r19) ^ r18);
		r19 += 2u;
	}
}

void sub_00D716(void)
{
	uint32_t r0 = 0, r1 = 0;

	r0 = TEXT_CELL(0, 24);
	r1 = 0x28u;
	do
	{
		mem_write16(r0, 0u);
		r1--;
		r0 += 2u;
	} while (r1 != 0u);
}

void sub_00D72D(void)
{
	uint32_t t1 = 0;

	r26 = (r26 & ~0xFFFFu) | 0x6000u;
	r27 = TEXT_CELL(8, 24);
	r28 = 0xE17Au;
	do
	{
		r26 = (r26 & ~0xFFu) | ROM8(r28);
		t1 = r27;
		r27 = r27 + 2u;
		mem_write16(t1, mem_read16(t1) ^ r26);
		r28 = r28 + 1u;
	} while (ROM8(r28) != 0u);
}

void sub_00D7CB(uint32_t r19, uint32_t *p_r2, uint32_t *p_left_caller)
{
	sub_01076A();
	sub_011868(r19);
	mem_write8(IO_C0001C, 6u);
	mem_write8(0xC8001Cu, 0x86u);
	for (;;)
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
			break;
		current_object = 0xE40Eu;
		sub_00D6EC();
	}
	unk_20E043 = 0xFFu;
	*p_left_caller = 1u;
}

void sub_00D884(uint32_t *p_r2, uint32_t *p_left_caller)
{
	for (;;)
	{
		*p_r2 = sub_0008F0(*p_r2);
		if (unk_20E700 != 0u)
			break;
		current_object = 0xE40Eu;
		sub_00D6EC();
	}
	unk_20E043 = 0xFFu;
	*p_left_caller = 1u;
}

void sub_00D8A5(void)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0;
	uint8_t t1 = 0;

	if (unk_20E700 != 0u)
		return;
	sub_00E8DF();
	ignored = sub_0008F0(0u);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	ignored = sub_0008F0(ignored);
	for (;;)
	{
		ignored = sub_0008F0(ignored);
		if (unk_20E623 == 0u)
			break;
		t1 = mem_read8(0x8006E1u);
		if (t1 != 0u)
		{
			sub_00E638();
			break;
		}
	}
	sub_00E620();
	r10 = 0x20FC02u;
	r11 = unk_20E030_ADDRESS;
	r0 = unk_20E041;
	r1 = 0x80u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10 = (r10 & ~0xFFFFu) | (uint16_t)(r10 + r1);
		r11++;
		r0--;
	} while (r0 != 0u);
	move_halfwords(SOUND_RAM + 0x1F32u, WORK_RAM + 0xFC30u, 0x1Eu);
	sub_011418(r0);
	move_string(0x20FC30u, 0x1Eu, SOUND_RAM + 0x1FAEu, 0x1Eu, 2u, 0u, 0u, r26, &r27, &r28);
	sub_001D84();
	r10 = 0x20FC00u;
	r9 = 0u;
	if (unk_20E614 != unk_20FC08 || unk_20E610 != unk_20FC05 || unk_20E611 != unk_20FC06 || unk_20E613 != unk_20FC07 || unk_20E615 != unk_20FC09)
	{
		r9 = 0xFFFFFFFFu;
		unk_20E614 = unk_20FC08;
		unk_20E610 = unk_20FC05;
		unk_20E611 = unk_20FC06;
		unk_20E613 = unk_20FC07;
		unk_20E615 = unk_20FC09;
		sub_011018();
		sub_01139E();
	}
	if (unk_20E60D == unk_20FC03 && unk_20E624 == unk_20FC04 && unk_20E630 == unk_20FC0A && unk_20E632 == AS16(unk_20FC0B[0]) && unk_20E634 == unk_20FC0D && r9 == 0u)
		return;
	unk_20E60D = unk_20FC03;
	unk_20E624 = unk_20FC04;
	unk_20E630 = unk_20FC0A;
	unk_20E632 = AS16(unk_20FC0B[0]);
	unk_20E634 = unk_20FC0D;
	sub_00036B();
}
