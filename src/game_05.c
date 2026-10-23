/**
 *    Name:        game_05.c
 *    Description: the game's functions, part 05 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    22-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

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

void sub_01EA14(void)
{
}

void sub_01EA4B(uint32_t r0, uint32_t r10)
{
	uint32_t r1 = 0;

	r1 = 0u;
	divux_pair(0x3Cu, &r0, &r1);
	mem_write8(r10, r1);
	mem_write8(r10 + 1u, r0);
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

void sub_01F190(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint32_t r0_2 = 0, r2_2 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint32_t t5 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(0xC80008u);
	t3 = mem_read8(IO_C0000A);
	t4 = mem_read8(0xC8000Au);
	r2 = (uint8_t)(~t3 | ~t4);
	r0 = ((~t1 | ~t2) & 0x33u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 3u);
	r1 = unk_20E70C;
	unk_20E70C = r0;
	unk_20E70D = r1;
	AS8(unk_20E70E) = ~r1 & r0;
	AT8(unk_20E70E, 1) = r1 & ~r0;
	t5 = unk_20E701;
	carry = (t5 & 4u) != 0u;
	unk_20E701 = t5 ^ 4u;
	r0_2 = carry;
	r10 = IO_PORTS + 0x50u + r0_2 * 4u;
	r11 = 0x20E710u + r0_2 * 4u;
	r0_2 = (uint8_t)(0u - r0_2);
	r0_2 = (uint8_t)(r0_2 + 2u);
	r0_2 = (uint8_t)(r0_2 * 2u);
	do
	{
		r1 = 0u;
		mem_write8(r10, 0xFFu);
		r2_2 = 8u;
		do
		{
			r3 = mem_read8(r10);
			carry = (uint8_t)(r3 * 2u) < r3;
			r1 = (uint8_t)(r1 * 2u + carry);
			r2_2--;
		} while (r2_2 != 0u);
		mem_write8(r11, r1);
		r0_2--;
		r11++;
		r10 += 2u;
	} while (r0_2 != 0u);
	carry = (unk_20E701 & 4u) != 0u;
	mem_write8(IO_C00060, carry);
}

void sub_01F237(void)
{
	uint8_t r1 = 0;
	uint32_t r0 = 0;
	uint16_t t1 = 0;

	r0 = unk_20E733;
	if (unk_20E732 == r0)
		return;
	t1 = ~unk_20E734[r0];
	r1 = (uint8_t)t1;
	if ((int16_t)t1 >= 0)
		mem_write8(SOUND_700800, ~r1);
	else
		mem_write8(SOUND_700700, ~r1);
	unk_20E733 = (r0 + 1u) & 0xFu;
}

void sub_01F266(uint16_t r0)
{
	r0 |= 0xFF00u;
	sub_01F26B(r0);
}

void sub_01F26B(uint16_t r0)
{
	uint32_t r1 = 0;

	r1 = unk_20E732;
	unk_20E734[r1] = r0;
	unk_20E732 = (r1 + 1u) & 0xFu;
}

void int_vector_40(void)
{
	uint32_t r0 = 0, r6 = 0;
	uint32_t saved_r28 = 0, saved_r27 = 0, saved_r25 = 0, saved_r12 = 0, saved_r25_2 = 0;

	saved_r28 = r28;
	saved_r27 = r27;
	saved_r25 = r25;
	saved_r12 = r12;
	unk_20E02D++;
	unk_20E00A++;
	mem_write16(IRQ_D00008, rom_03F82A);
	unk_20E02C = 0x22u;
	unk_20E00E--;
	if (mem_read8(r25) != 0u)
	{
		saved_r25_2 = r25;
		r25 = GAME_VARIABLES;
		sub_03F62A();
		r25 = 0x218000u;
		sub_03F62A();
		r25 = saved_r25_2;
		if (unk_20E027 == 0u)
		{
			move_halfwords(VIDEO_RAM + 0x1FF20u, unk_20E220_ADDRESS, 8u);
			move_halfwords(VIDEO_RAM + 0x1FF40u, unk_20E240_ADDRESS, 8u);
		}
		else
		{
			unk_20E027 = 0u;
			move_halfwords(VIDEO_31FF00, unk_20E200_ADDRESS, 0x48u);
		}
		r0 = unk_20E2B0;
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | unk_20E2B1);
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | unk_20E2B2);
		r0 = (uint8_t)((r0 << 6) | (r0 >> 2) | unk_20E2B3);
		mem_write8(IO_C0000E, (r0 << 6) | (r0 >> 2));
	}
	sub_00F396();
	sub_03F8DA();
	if (mem_read8(r25 + 0xA7u) == 0u)
		sub_03F6A0();
	mem_write8(r25 + 0xA7u, 0u);
	if (unk_20E700 == 0u)
	{
		r6 = sub_0009F9();
		switch (mem_read8(r25))
		{
		case 0:
			sub_000B16();
			break;
		case 1:
			r0 = sub_000BF3();
			break;
		default:
			invalid_jump(rom_03FAC4[mem_read8(r25)]);
		}
		sub_03F8C9(r0);
		if (unk_20E615 == 0u)
			sub_01106C(r6);
	}
	sub_03F96B();
	sub_018863();
	sub_03F922();
	unk_20E023 = 0xFFu;
	if (unk_20E024 == 0u && ((unk_20E700 != 0u && mem_read8(r25) == 0u) || unk_20E028 != 0u))
		swap_tasks();
	r0 = rom_03FACC[mem_read8(r25)];
	mem_write8(r0, mem_read8(r0) + 1u);
	r12 = saved_r12;
	r25 = saved_r25;
	r27 = saved_r27;
	r28 = saved_r28;
	mem_write8(IRQ_D00007, 0xFEu);
}

void sub_03F62A(void)
{
	uint32_t r0 = 0;
	uint32_t t1 = 0, t2 = 0;

	if ((mem_read32(r25 + 0x94u) & 1u) != 0u)
	{
		sub_01167E();
		move_string(r25 + 0x19Eu, 7u, mem_read32(r25 + 8u) + 0x3Eu, 7u, 2u, 0u, 0u, r26, &r27, &r28);
	}
	t1 = mem_read32(r25 + 0x94u);
	mem_write32(r25 + 0x94u, t1 & ~4u);
	if ((t1 & 4u) != 0u)
	{
		r0 = mem_read32(r25 + 8u);
		move_halfwords(r0, r25 + 0x160u, 0x1Fu);
		move_string(r25 + 0x1ACu, 2u, r0 + 0x4Cu, 2u, 2u, 0u, 0u, r26, &r27, &r28);
	}
	t2 = mem_read32(r25 + 0x94u);
	mem_write32(r25 + 0x94u, t2 & ~8u);
	if ((t2 & 8u) != 0u)
	{
		r0 = mem_read32(r25 + 8u);
		move_halfwords(r25 + 0x160u, r25 + 0x1B0u, 0x1Fu);
		move_halfwords(r25 + 0x1ACu, r25 + 0x1FCu, 2u);
		move_halfwords(r0, r25 + 0x1B0u, 0x1Fu);
		move_string(r25 + 0x1FCu, 2u, r0 + 0x4Cu, 2u, 2u, 0u, 0u, r26, &r27, &r28);
	}
	if (mem_read32(r25 + 0x900u) != 0u)
		sub_0115EB();
}

void sub_03F6A0(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;

	r0 = mem_read8(r25) * 2u;
	mem_write32(VIDEO_RAM + 0x1FF10u + r0 * 4u, unk_20E210[r0]);
	mem_write32(VIDEO_RAM + 0x1FF14u + r0 * 4u, unk_20E214[r0]);
	r1 = 0u - r0 + 2u;
	r2 = unk_20E210[r1];
	r3 = unk_20E290[r1];
	unk_20E210[r1] = r3;
	r3 = r3 << 6;
	mem_write32(VIDEO_RAM + 0x1FF10u + r1 * 4u, (((int32_t)((r2 << 6) - r3) >> 1) + r3) >> 6);
	r2 = unk_20E214[r1];
	r3 = unk_20E294[r1];
	unk_20E214[r1] = r3;
	r3 = r3 << 7;
	mem_write32(VIDEO_RAM + 0x1FF14u + r1 * 4u, (((int32_t)((r2 << 7) - r3) >> 1) + r3) >> 7);
	mem_write32(VIDEO_RAM + 0x1FF20u + r0 * 4u, unk_20E220[r0]);
	mem_write32(VIDEO_RAM + 0x1FF24u + r0 * 4u, unk_20E224[r0]);
	r2 = unk_20E220[r1];
	r3 = unk_20E2A0[r1];
	unk_20E220[r1] = r3;
	r3 = r3 << 6;
	mem_write32(VIDEO_RAM + 0x1FF20u + r1 * 4u, (((int32_t)((r2 << 6) - r3) >> 1) + r3) >> 6);
	r2 = unk_20E224[r1];
	r3 = unk_20E2A4[r1];
	unk_20E224[r1] = r3;
	r3 = r3 << 7;
	mem_write32(VIDEO_RAM + 0x1FF24u + r1 * 4u, (((int32_t)((r2 << 7) - r3) >> 1) + r3) >> 7);
	r0_2 = (uint8_t)(unk_20E204 >> 8u);
	mem_write8(VIDEO_31FF05, r0_2);
	AT8(unk_20E204, 1) = (r0_2 + 4u) & 0xECu;
}

void int_vector_41(void)
{
	mem_write8(IRQ_D00007, 0xFDu);
}

void int_vector_42(void)
{
	mem_write8(IRQ_D00007, 0xFBu);
}

void int_vector_43(void)
{
	uint8_t t1 = 0;

	t1 = unk_20E02C;
	unk_20E02C = t1 - 1u;
	if (t1 != 1u)
		mem_write16(IRQ_D00008, rom_03F82C);
	sub_0188BE();
	mem_write8(IRQ_D00007, 0xF7u);
}

void int_vector_44(void)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	if (unk_20E623 == 0u)
	{
		mem_write8(IRQ_D00007, 0xEFu);
		return;
	}
	unk_20E060++;
	r1 = 1u;
	t1 = mem_read8(0x800010u);
	r0 = t1;
	mem_write8(0x800010u, 0u);
	if (r0 < 4u)
	{
		switch (r0)
		{
		case 3:
			sub_00E638();
			r1 = 0x168u;
			unk_20E060 = 0u;
			break;
		case 0:
		case 1:
		case 2:
			break;
		default:
			invalid_jump(rom_03F85C[r0]);
		}
	}
	mem_write16(IRQ_D0000A, r1);
	mem_write8(IRQ_D00007, 0xEFu);
}

void int_default(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	int_vector_02();
}

void int_vector_02(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	sub_03F8C9(0u);
}

void sub_03F8C9(uint32_t r0)
{
	if ((((r0 & ~0xFFu) | unk_20E102) & 0x80u) != 0u)
		unk_20E700 = 0xFFu;
}

void sub_03F8DA(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;
	uint8_t t1 = 0, t2 = 0;

	r10 = (uint32_t)mem_read8(r25) << 8;
	r0 = r25 + 0x800u;
	r1 = mem_read8(r0);
	r0++;
	t1 = mem_read8(r0);
	r0++;
	if (r1 != t1)
	{
		mem_write8(r10 + SOUND_700700, mem_read8(r0 + r1));
		mem_write8(r0 - 2u, (r1 + 1u) & 0x1Fu);
	}
	r0 = r25 + 0x840u;
	r1 = mem_read8(r0);
	r0++;
	t2 = mem_read8(r0);
	r0++;
	if (r1 != t2)
	{
		mem_write8(r10 + (SOUND_RAM + 0x708u), mem_read8(r0 + r1));
		mem_write8(r0 - 2u, (r1 + 1u) & 0x1Fu);
	}
}

void sub_03F922(void)
{
	uint32_t r0 = 0, r7 = 0;

	if (unk_20E700 != 0u)
		return;
	r0 = (uint16_t)(unk_20E03A + 0x11u);
	if (r0 < 0x3E8u)
	{
		unk_20E03A = r0;
		return;
	}
	r7 = SOUND_701F10;
	if (mem_read32(SOUND_701F10) % 0x49u != 0u)
	{
		unk_20E03A = r0 - 0x3E8u;
		if (mem_read32(r7) != 0xFFFFFFFFu)
		{
			mem_write32(r7, mem_read32(r7) + 1u);
			sub_011418(r0);
			mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + 1u);
		}
		return;
	}
	if (r0 < 0x7D0u)
	{
		unk_20E03A = r0;
		return;
	}
	r0 = (uint16_t)(r0 - 0x3E8u);
	unk_20E03A = r0 - 0x3E8u;
	if (mem_read32(r7) != 0xFFFFFFFFu)
	{
		mem_write32(r7, mem_read32(r7) + 1u);
		sub_011418(r0);
		mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + 1u);
	}
}

void sub_03F96B(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (unk_20E0C1 != 0u)
	{
		t1 = unk_20E0C3;
		unk_20E0C3 = t1 - 1u;
		if (t1 == 1u)
		{
			unk_20E0C3 = unk_20E0C2;
			unk_20E0C0[0] ^= 2u;
			unk_20E0C1--;
		}
	}
	if (unk_20E0C9 != 0u)
	{
		t2 = unk_20E0CB;
		unk_20E0CB = t2 - 1u;
		if (t2 == 1u)
		{
			unk_20E0CB = unk_20E0CA;
			unk_20E0C8[0] ^= 8u;
			unk_20E0C9--;
		}
	}
	r0 = (uint8_t)unk_20E092[0] | unk_20E0C0[0] | unk_20E0C8[0];
	if ((unk_208090 & 0x100u) == 0u && unk_20E624 != 0u)
		r0 |= unk_20E0D0[0];
	mem_write8(IO_C00006, r0);
	if (unk_20E0C5 != 0u)
	{
		t3 = unk_20E0C7;
		unk_20E0C7 = t3 - 1u;
		if (t3 == 1u)
		{
			unk_20E0C7 = unk_20E0C6;
			unk_20E0C4 ^= 2u;
			unk_20E0C5--;
		}
	}
	if (unk_20E0CD != 0u)
	{
		t4 = unk_20E0CF;
		unk_20E0CF = t4 - 1u;
		if (t4 == 1u)
		{
			unk_20E0CF = unk_20E0CE;
			unk_20E0CC ^= 8u;
			unk_20E0CD--;
		}
	}
	r0 = mem_read8(0x20E093u) | unk_20E0C4 | unk_20E0CC;
	if ((unk_218090 & 0x100u) == 0u && unk_20E624 != 0u)
		r0 |= unk_20E0D1;
	mem_write8(0xC80006u, r0);
	r10 = GAME_VARIABLES;
	r11 = unk_20E094_ADDRESS;
	sub_03FA54(r10, r11);
	r10 = 0x218000u;
	r11 = 0x20E095u;
	sub_03FA54(r10, r11);
}

void sub_03FA54(uint32_t r10, uint32_t r11)
{
	uint8_t r0 = 0, r1 = 0;
	uint8_t t1 = 0, t2 = 0;

	r0 = mem_read8(r11);
	r1 = mem_read8(r10 + 0xF20u);
	if (r1 != mem_read8(r10 + 0xF22u))
	{
		mem_write8(r10 + 0xF22u, r1);
		if ((int8_t)r1 > 0)
		{
			r0 ^= 1u;
			mem_write8(r10 + 0xF24u, r1);
		}
		else if (r1 == 0u)
			r0 &= ~1u;
		else
			r0 |= 1u;
	}
	else if ((int8_t)r1 > 0)
	{
		t1 = mem_read8(r10 + 0xF24u);
		mem_write8(r10 + 0xF24u, t1 - 1u);
		if (t1 == 1u)
		{
			r0 ^= 1u;
			mem_write8(r10 + 0xF24u, r1);
		}
	}
	r1 = mem_read8(r10 + 0xF21u);
	if (r1 == mem_read8(r10 + 0xF23u))
	{
		if ((int8_t)r1 > 0)
		{
			t2 = mem_read8(r10 + 0xF25u);
			mem_write8(r10 + 0xF25u, t2 - 1u);
			if (t2 == 1u)
			{
				r0 ^= 2u;
				mem_write8(r10 + 0xF25u, r1);
			}
		}
		mem_write8(r11, r0);
		return;
	}
	mem_write8(r10 + 0xF23u, r1);
	if ((int8_t)r1 > 0)
	{
		r0 ^= 2u;
		mem_write8(r10 + 0xF25u, r1);
		mem_write8(r11, r0);
		return;
	}
	if (r1 == 0u)
	{
		r0 &= ~2u;
		mem_write8(r11, r0);
		return;
	}
	r0 |= 2u;
	mem_write8(r11, r0);
}

void reset(void)
{
	boot();
}
