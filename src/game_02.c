/**
 *    Name:        game_02.c
 *    Description: the game's functions, part 02 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    22-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void sub_00D9F3(void)
{
	uint32_t zero = 0;
	uint8_t r0_2 = 0, r1_2 = 0, r3 = 0;
	uint16_t r2 = 0, r9 = 0;
	uint32_t r0 = 0, r1 = 0, r3_2 = 0, r11 = 0, r13 = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;
	uint32_t t3 = 0;

	r11 = mem_read32(r25 + 0x24u);
	r12 = 0x20FC00u;
	if (mem_read8(r25 + 1u) == 0u)
	{
		r1 = unk_20E041;
		r2 = 0x80u;
		r3 = mem_read8(r11 + 0x1Du);
		r13 = r12 + 0x1Cu;
		r14 = r25 + 0x300u;
		r0 = mem_read16(r11 + 0x1Au);
		do
		{
			r1--;
		} while ((r1 != 0u) & (unk_20FC1C == r3));
		if (unk_20FC1C == r3)
		{
			if (r0 == 0u)
			{
				r1 = 0u;
				if (unk_20E620 == 0u)
					r1 = unk_20E040;
				r9 = rom_00E614[r1];
				r13 = r12 + 0x19u;
				r1 = unk_20E041;
				r3_2 = 0u;
				for (;;)
				{
					if (mem_read8(r14 + r3_2) != 0u || mem_read8(r13) == 0u)
					{
						r3_2 = (uint8_t)(r3_2 + 1u);
						r13 = (r13 & ~0xFFFFu) | (uint16_t)(r13 + r2);
						r1--;
						if (r1 == 0u)
							break;
						continue;
					}
					mem_write8(r14 + r3_2, 0xFFu);
					mem_write8(r11 + 0x1Eu, r3_2);
					mem_write8(r11 + 0x1Du, mem_read8(r11 + 0x1Du) + 1u);
					r0 = r9;
					if ((uint8_t)(mem_read8(r25 + 0x30Fu) + 1u) != unk_20E041 || r0 <= 0x28u)
						break;
					r0 = 0x28u;
					break;
				}
			}
			else
			{
				t1 = r0;
				r0 = (uint16_t)(t1 - 1u);
				r9 = (uint16_t)r0;
				if (t1 != 1u)
				{
					r13 = r12 + 0x19u;
					r1 = unk_20E041;
					r3_2 = 0u;
					for (;;)
					{
						if (mem_read8(r14 + r3_2) != 0u || mem_read8(r13) == 0u)
						{
							r3_2 = (uint8_t)(r3_2 + 1u);
							r13 = (r13 & ~0xFFFFu) | (uint16_t)(r13 + r2);
							r1--;
							if (r1 == 0u)
								break;
							continue;
						}
						mem_write8(r14 + r3_2, 0xFFu);
						mem_write8(r11 + 0x1Eu, r3_2);
						mem_write8(r11 + 0x1Du, mem_read8(r11 + 0x1Du) + 1u);
						r0 = r9;
						if ((uint8_t)(mem_read8(r25 + 0x30Fu) + 1u) != unk_20E041 || r0 <= 0x28u)
							break;
						r0 = 0x28u;
						break;
					}
				}
				else
				{
					mem_write8(r11 + 0x1Du, mem_read8(r11 + 0x1Du) + 1u);
					mem_write8(r11 + 0x1Eu, 0xFFu);
					mem_write32(r14, 0u);
					r14 = r14 + 4u;
					mem_write32(r14, 0u);
					mem_write8(0x80000Au, 1u);
					r14 = r14 + 4u;
				}
			}
		}
		mem_write16(r11 + 0x1Au, r0);
	}
	r1 = unk_20E041;
	r13 = r12 + 0x28u;
	do
	{
		t2 = mem_read8(r13);
		zero = t2 == 0u;
		r13 += 0x80u;
		r1--;
	} while ((r1 != 0u) & (t2 == 0u));
	if (zero)
	{
		if (unk_20FC1A != 0u && mem_read8(r25 + 0x80u) != 0u && mem_read8(r25 + 0x80u) <= 5u)
			mem_write32(r25 + 0x80u, 7u);
	}
	else if (mem_read8(r25 + 0x80u) != 0u && mem_read8(r25 + 0x80u) <= 5u)
	{
		mem_write32(r25 + 0x80u, 5u);
		unk_20E02E = 0u;
		if (unk_20FC1A != 0u && mem_read8(r25 + 0x80u) != 0u && mem_read8(r25 + 0x80u) <= 5u)
			mem_write32(r25 + 0x80u, 7u);
	}
	if (unk_20FC1D == mem_read8(r11 + 0x1Cu))
		return;
	mem_write8(r11 + 0x1Cu, unk_20FC1D);
	r0_2 = unk_20FC1E;
	if ((int8_t)r0_2 < 0)
	{
		mem_write8(r25 + 0x30Fu, 0u);
		mem_write8(r25 + 0x319u, 0u);
		return;
	}
	r13 = r25 + 0x30Fu;
	r1 = mem_read8(r13);
	r1 = (uint8_t)(r1 + 1u);
	mem_write8(r13, r1);
	mem_write8(r13 + r1, r0_2);
	if (mem_read8(r25 + 1u) == r0_2)
	{
		mem_write8(r11 + 0x19u, 0u);
		mem_write8(r25 + 0x319u, 0xFFu);
		r1_2 = mem_read8(r13 + 1u);
		mem_write8(r25 + 0x30Cu, r1_2);
		if (mem_read8(r25 + 1u) == r1_2)
			mem_write8(r25 + 0x318u, 0xFFu);
	}
	if (mem_read8(r25 + 0x319u) != 0u && mem_read8(r25 + 0x318u) != 0u)
	{
		r14 = r11 + 0x1Fu;
		mem_write8(r14, mem_read8(r13));
		r14 = r14 + 1u;
		r13++;
		t3 = mem_read32(r13 + 4u);
		mem_write32(r14, mem_read32(r13));
		mem_write32(r14 + 4u, t3);
		r14 = r14 + 8u;
	}
}

void sub_00DB6D(void)
{
	uint8_t r0_2 = 0, r1 = 0, r3_2 = 0, r5 = 0;
	uint16_t r4 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r3 = 0, r4_2 = 0, r5_2 = 0, r6 = 0, r9 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r19 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0, t3 = 0, t4 = 0, t5 = 0;

	r11 = mem_read32(r25 + 0x24u);
	r12 = mem_read32(r25 + 0x308u);
	if (mem_read8(r25 + 0x31Au) == 0u)
		return;
	if (mem_read8(r25 + 0x318u) != 0u)
	{
		r19 = r12 + 0x1Fu;
		r9 = mem_read8(r19);
		r1 = mem_read8(r11 + 0x63u);
		r19++;
		r13 = r19;
		r0 = r9;
		do
		{
			r2 = rom_00E0A0[mem_read8(r13)] + unk_20E000_ADDRESS;
			r0--;
			r13++;
		} while ((r0 != 0u) & (mem_read8(r2 + 0x61u) == r1));
		if (mem_read8(r2 + 0x61u) == r1)
		{
			r13 = r19;
			r14 = r25 + 0x330u;
			r0 = r9;
			do
			{
				r2 = mem_read8(r13);
				r15 = rom_00E0A0[r2] + unk_20E000_ADDRESS;
				r0--;
				r13++;
			} while ((r0 != 0u) & (mem_read8(r14 + r2) == mem_read8(r15 + 0x60u)));
			if (mem_read8(r14 + r2) != mem_read8(r15 + 0x60u))
			{
				r3 = mem_read8(r15 + 0x60u);
				mem_write8(r14 + r2, r3);
				mem_write8(r11 + 0x64u, r2);
				r3 = (uint8_t)(r3 - 1u);
				mem_write8(r11 + 0x65u, r3);
				r4 = mem_read8(r25 + 0x338u + r3);
				t1 = r25 + 0x338u + r3;
				mem_write8(t1, mem_read8(t1) + 1u);
				mem_write8(r11 + 0x66u, (uint8_t)r4);
				r5 = (uint8_t)(mem_read16(r15 + 0x44u) >> 15);
				mem_write8(r11 + 0x67u, r5);
				mem_write16(r11 + 0x68u, mem_read32(r11 + 0x6Cu) / 0xAu + r4);
				mem_write8(r11 + 0x63u, mem_read8(r11 + 0x63u) + 1u);
				if (r4 == 0u && r3 != 5u)
				{
					r0 = mem_read8(r15 + 0x62u);
					r0 = (uint8_t)(r0 * 2u);
					r0 = (uint8_t)(r0 + r5);
					mem_write16(r11 + 0x6Au, mem_read16(r11 + 0x6Au) + (int8_t)(rom_00E5B3[rom_00E5D3[r0]] + unk_20FC0B[r3]) * 0x1Eu);
				}
			}
		}
		if (mem_read8(r25 + 0x31Cu) != 0u)
		{
			mem_write32(r11 + 0x6Cu, mem_read32(r11 + 0x6Cu) + 0x22u);
			if (mem_read16(r11 + 0x6Au) != 0u)
				mem_write16(r11 + 0x6Au, mem_read16(r11 + 0x6Au) - 1u);
		}
		r16 = r11 + 0x1Fu;
		r0 = mem_read8(r16);
		t2 = r0;
		r0 = (uint8_t)(t2 - 1u);
		r16++;
		if ((int8_t)t2 > 1)
		{
			r4_2 = mem_read8(r16);
			r13 = rom_00E0A0[r4_2] + unk_20E000_ADDRESS;
			r16++;
			if (mem_read8(r25 + 0x31Cu) != 0u)
			{
				do
				{
					r6 = mem_read8(r16);
					r14 = rom_00E0A0[r6] + unk_20E000_ADDRESS;
					r16++;
					if (mem_read8(r13 + 0x5Eu) != 0u)
					{
						if (mem_read8(r14 + 0x5Eu) == 0u || mem_read16(r13 + 0x16u) <= mem_read16(r14 + 0x16u))
						{
							t3 = r6;
							r6 = r4_2;
							r4_2 = t3;
							r13 = r14;
						}
					}
					else if (mem_read32(r14 + 0x40u) <= mem_read32(r13 + 0x40u))
					{
						t3 = r6;
						r6 = r4_2;
						r4_2 = t3;
						r13 = r14;
					}
					mem_write8(r16 - 2u, r6);
					r0--;
				} while (r0 != 0u);
				mem_write8(r16 - 1u, r4_2);
			}
		}
	}
	r13 = r12 + 0x1Fu;
	r0 = mem_read8(r13);
	r1 = 0u;
	r13++;
	r2 = mem_read8(r13);
	r3_2 = mem_read8(r25 + 1u);
	r14 = rom_00E0A0[r2] + unk_20E000_ADDRESS;
	mem_write16(r25 + 0x350u, mem_read16(r14 + 0x46u));
	mem_write8(r25 + 0x352u, mem_read8(r14 + 0x62u));
	r13++;
	do
	{
		mem_write8(r25 + 0x320u + r2, r1);
		if (r2 == r3_2)
			mem_write8(r25 + 0x31Fu, r1);
		r2 = mem_read8(r13);
		r1++;
		r0--;
		r13++;
	} while (r0 != 0u);
	if (mem_read8(r25 + 0x342u) != mem_read8(r12 + 0x63u))
	{
		r0_2 = mem_read8(r12 + 0x63u);
		mem_write8(r25 + 0x342u, r0_2);
		mem_write8(r11 + 0x61u, r0_2);
		r0 = mem_read8(r12 + 0x64u);
		r1_2 = mem_read8(r12 + 0x65u);
		r2 = mem_read8(r12 + 0x66u);
		r3_2 = mem_read8(r12 + 0x67u);
		r4 = mem_read16(r12 + 0x68u);
		r5_2 = r1_2 * 2u;
		t4 = r25 + 0x480u + r0;
		mem_write8(t4, mem_read8(t4) + 1u);
		t5 = r25 + 0x35Au + r1_2;
		mem_write8(t5, mem_read8(t5) + 1u);
		mem_write8(r25 + 0x360u + r5_2 * 4u + r0, r2);
		mem_write8(r25 + 0x390u + r5_2 * 4u + r2, r0);
		r5_2 = r5_2 * 2u;
		mem_write16(r25 + 0x3C0u + r5_2 * 4u + r0 * 2u, r4);
		mem_write16(r25 + 0x420u + r5_2 * 4u + r2 * 2u, r4);
		if (r2 == 0u && r1_2 != 5u)
		{
			mem_write8(r25 + 0x349u, mem_read8(r25 + 0x349u) + 1u);
			mem_write8(r25 + 0x34Au + r1_2, r3_2);
			r0 = mem_read8(rom_00E0A0[r0] + 0x20E062u);
			r0 = (uint8_t)(r0 * 2u);
			mem_write8(r25 + 0x344u + r1_2, rom_00E5D3[(uint8_t)(r0 + r3_2)]);
		}
	}
}

void sub_00DDA8(void)
{
	uint32_t r0 = 0, r10 = 0, ignored = 0;

	switch (mem_read8(r25 + 0x82u))
	{
	case 0:
		mem_write8(r25 + 0x153u, 0u);
		mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) | 1u);
		mem_write8(r25 + 0x82u, mem_read8(r25 + 0x82u) + 1u);
		return;
	case 1:
		break;
	case 2:
		mem_write8(r25 + 0x82u, mem_read8(r25 + 0x82u) + 1u);
		r10 = 0xDF74u;
		ignored = sub_011E2C(r10);
		return;
	case 3:
		if (mem_read8(r25) != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		mem_write8(r25 + 0x153u, 1u);
		mem_write32(r25 + 0x94u, mem_read32(r25 + 0x94u) | 1u);
		mem_write8(r25 + 0x82u, mem_read8(r25 + 0x82u) + 1u);
		return;
	case 4:
		if (unk_20FC1A != 0u)
			return;
		sub_012004();
		mem_write32(r25 + 0x80u, 2u);
		r10 = unk_20E000_ADDRESS + mem_read8(r25) * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		sub_018AF0();
		return;
	default:
		invalid_jump(rom_00DDB4[mem_read8(r25 + 0x82u)]);
	}
	if ((mem_read32(r25 + 0x94u) & 1u) == 0u)
	{
		mem_write8(r25 + 0x82u, mem_read8(r25 + 0x82u) + 1u);
		sub_012004();
		sub_011C60();
		sub_01070C();
		sub_010889();
		sub_01076A();
		sub_00DEC2();
		mem_write32(r25 + 0x90u, mem_read32(r25 + 0x90u) & ~0x100u);
		r0 = mem_read8(r25);
		unk_20E0D0[r0] = 0u;
		r10 = unk_20E000_ADDRESS + r0 * 4u;
		mem_write8(r10 + 0xC0u, 2u);
		mem_write8(r10 + 0xC8u, 8u);
		mem_write8(r10 + 0xC1u, 0xFFu);
		mem_write8(r10 + 0xC2u, 0x10u);
		mem_write8(r10 + 0xC3u, 1u);
		mem_write8(r10 + 0xC9u, 0xFFu);
		mem_write8(r10 + 0xCAu, 0x10u);
		mem_write8(r10 + 0xCBu, 1u);
	}
}

void sub_00DEC2(void)
{
	move_string(0x100800u, 0x28u, mem_read32(r25 + 0x24u) + 0x30u, 0x28u, 2u, 0u, 0u, r26, &r27, &r28);
}

void sub_00DED2(void)
{
	mem_write8(mem_read32(r25 + 0x24u) + 0x5Cu, 0xFFu);
}

uint32_t sub_00DED9(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, r1 = 0;
	uint8_t t1 = 0;

	r0 = mem_read32(r25 + 0x308u) + 0x1Fu;
	r1 = mem_read8(r0);
	r0++;
	do
	{
		t1 = mem_read8(0x20FC5Cu + rom_00E0C4[mem_read8(r0)]);
		zero = t1 == 0u;
		r1--;
		r0++;
	} while ((r1 != 0u) & (t1 != 0u));
	return zero;
}

void sub_00DEF8(void)
{
	uint16_t r1 = 0;
	uint32_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, ignored = 0;
	uint8_t t1 = 0;

	r10 = r25 + 0x328u;
	r9 = unk_20E041;
	r11 = unk_20FB80_ADDRESS;
	do
	{
		r11 += 0x80u;
		t1 = mem_read8(r10);
		r10++;
		if (mem_read8(r11 + 0x11u) != t1)
		{
			mem_write8(r10 - 1u, mem_read8(r11 + 0x11u));
			r1 = mem_read16(r11 + 0x16u);
			r8 = mem_read8(r11 + 0x18u);
			r8 = (uint8_t)(r8 * 6u);
			r12 = unk_20E300_ADDRESS + r8;
			if (r1 <= mem_read16(r12 + 4u))
			{
				mem_write16(r12 + 4u, r1);
				mem_write8(r12, mem_read8(r11 + 0x12u));
				mem_write8(r12 + 1u, mem_read8(r11 + 0x13u));
				mem_write16(r12 + 2u, mem_read16(r11 + 0x14u));
				r13 = SOUND_RAM + 0x1F32u + r8;
				mem_write32(r13, mem_read32(r12));
				mem_write16(r13 + 4u, mem_read16(r12 + 4u));
				sub_011418(ignored);
				r13 = SOUND_RAM + 0x1FAEu + r8;
				mem_write32(r13, mem_read32(r12));
				mem_write16(r13 + 4u, mem_read16(r12 + 4u));
			}
		}
		r9--;
	} while (r9 != 0u);
}

void sub_00E620(void)
{
	bool L_00E854 = false;
	uint32_t r1 = 0, r10 = 0, r11 = 0;
	uint8_t t65 = 0, t66 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0, t33 = 0, t34 = 0, t35 = 0, t36 = 0, t37 = 0, t38 = 0, t39 = 0, t40 = 0, t41 = 0, t42 = 0, t43 = 0, t44 = 0, t45 = 0, t46 = 0, t47 = 0, t48 = 0, t49 = 0, t50 = 0, t51 = 0, t52 = 0, t53 = 0, t54 = 0, t55 = 0, t56 = 0, t57 = 0, t58 = 0, t59 = 0, t60 = 0, t61 = 0, t62 = 0, t63 = 0, t64 = 0;

	for (;;)
	{
		if (L_00E854 || unk_20E623 == 0u)
		{
			if (L_00E854 || unk_20E043 != 0u || unk_20E042 != 0u)
			{
				if (!L_00E854)
				{
					r10 = unk_20FB00_ADDRESS;
					r11 = 0x20FC00u;
					r1 = 1u;
				}
				do
				{
					t1 = r11;
					r11 += 8u;
					t2 = mem_read32(r10 + 4u);
					mem_write32(t1, mem_read32(r10));
					mem_write32(t1 + 4u, t2);
					r10 += 8u;
					t3 = r11;
					r11 += 8u;
					t4 = mem_read32(r10 + 4u);
					mem_write32(t3, mem_read32(r10));
					mem_write32(t3 + 4u, t4);
					r10 += 8u;
					t5 = r11;
					r11 += 8u;
					t6 = mem_read32(r10 + 4u);
					mem_write32(t5, mem_read32(r10));
					mem_write32(t5 + 4u, t6);
					r10 += 8u;
					t7 = r11;
					r11 += 8u;
					t8 = mem_read32(r10 + 4u);
					mem_write32(t7, mem_read32(r10));
					mem_write32(t7 + 4u, t8);
					r10 += 8u;
					t9 = r11;
					r11 += 8u;
					t10 = mem_read32(r10 + 4u);
					mem_write32(t9, mem_read32(r10));
					mem_write32(t9 + 4u, t10);
					r10 += 8u;
					t11 = r11;
					r11 += 8u;
					t12 = mem_read32(r10 + 4u);
					mem_write32(t11, mem_read32(r10));
					mem_write32(t11 + 4u, t12);
					r10 += 8u;
					t13 = r11;
					r11 += 8u;
					t14 = mem_read32(r10 + 4u);
					mem_write32(t13, mem_read32(r10));
					mem_write32(t13 + 4u, t14);
					r10 += 8u;
					t15 = r11;
					r11 += 8u;
					t16 = mem_read32(r10 + 4u);
					mem_write32(t15, mem_read32(r10));
					mem_write32(t15 + 4u, t16);
					r10 += 8u;
					t17 = r11;
					r11 += 8u;
					t18 = mem_read32(r10 + 4u);
					mem_write32(t17, mem_read32(r10));
					mem_write32(t17 + 4u, t18);
					r10 += 8u;
					t19 = r11;
					r11 += 8u;
					t20 = mem_read32(r10 + 4u);
					mem_write32(t19, mem_read32(r10));
					mem_write32(t19 + 4u, t20);
					r10 += 8u;
					t21 = r11;
					r11 += 8u;
					t22 = mem_read32(r10 + 4u);
					mem_write32(t21, mem_read32(r10));
					mem_write32(t21 + 4u, t22);
					r10 += 8u;
					t23 = r11;
					r11 += 8u;
					t24 = mem_read32(r10 + 4u);
					mem_write32(t23, mem_read32(r10));
					mem_write32(t23 + 4u, t24);
					r10 += 8u;
					t25 = r11;
					r11 += 8u;
					t26 = mem_read32(r10 + 4u);
					mem_write32(t25, mem_read32(r10));
					mem_write32(t25 + 4u, t26);
					r10 += 8u;
					t27 = r11;
					r11 += 8u;
					t28 = mem_read32(r10 + 4u);
					mem_write32(t27, mem_read32(r10));
					mem_write32(t27 + 4u, t28);
					r10 += 8u;
					t29 = r11;
					r11 += 8u;
					t30 = mem_read32(r10 + 4u);
					mem_write32(t29, mem_read32(r10));
					mem_write32(t29 + 4u, t30);
					r10 += 8u;
					t31 = r11;
					r11 += 8u;
					t32 = mem_read32(r10 + 4u);
					mem_write32(t31, mem_read32(r10));
					mem_write32(t31 + 4u, t32);
					r10 += 8u;
					t33 = r11;
					r11 += 8u;
					t34 = mem_read32(r10 + 4u);
					mem_write32(t33, mem_read32(r10));
					mem_write32(t33 + 4u, t34);
					r10 += 8u;
					t35 = r11;
					r11 += 8u;
					t36 = mem_read32(r10 + 4u);
					mem_write32(t35, mem_read32(r10));
					mem_write32(t35 + 4u, t36);
					r10 += 8u;
					t37 = r11;
					r11 += 8u;
					t38 = mem_read32(r10 + 4u);
					mem_write32(t37, mem_read32(r10));
					mem_write32(t37 + 4u, t38);
					r10 += 8u;
					t39 = r11;
					r11 += 8u;
					t40 = mem_read32(r10 + 4u);
					mem_write32(t39, mem_read32(r10));
					mem_write32(t39 + 4u, t40);
					r10 += 8u;
					t41 = r11;
					r11 += 8u;
					t42 = mem_read32(r10 + 4u);
					mem_write32(t41, mem_read32(r10));
					mem_write32(t41 + 4u, t42);
					r10 += 8u;
					t43 = r11;
					r11 += 8u;
					t44 = mem_read32(r10 + 4u);
					mem_write32(t43, mem_read32(r10));
					mem_write32(t43 + 4u, t44);
					r10 += 8u;
					t45 = r11;
					r11 += 8u;
					t46 = mem_read32(r10 + 4u);
					mem_write32(t45, mem_read32(r10));
					mem_write32(t45 + 4u, t46);
					r10 += 8u;
					t47 = r11;
					r11 += 8u;
					t48 = mem_read32(r10 + 4u);
					mem_write32(t47, mem_read32(r10));
					mem_write32(t47 + 4u, t48);
					r10 += 8u;
					t49 = r11;
					r11 += 8u;
					t50 = mem_read32(r10 + 4u);
					mem_write32(t49, mem_read32(r10));
					mem_write32(t49 + 4u, t50);
					r10 += 8u;
					t51 = r11;
					r11 += 8u;
					t52 = mem_read32(r10 + 4u);
					mem_write32(t51, mem_read32(r10));
					mem_write32(t51 + 4u, t52);
					r10 += 8u;
					t53 = r11;
					r11 += 8u;
					t54 = mem_read32(r10 + 4u);
					mem_write32(t53, mem_read32(r10));
					mem_write32(t53 + 4u, t54);
					r10 += 8u;
					t55 = r11;
					r11 += 8u;
					t56 = mem_read32(r10 + 4u);
					mem_write32(t55, mem_read32(r10));
					mem_write32(t55 + 4u, t56);
					r10 += 8u;
					t57 = r11;
					r11 += 8u;
					t58 = mem_read32(r10 + 4u);
					mem_write32(t57, mem_read32(r10));
					mem_write32(t57 + 4u, t58);
					r10 += 8u;
					t59 = r11;
					r11 += 8u;
					t60 = mem_read32(r10 + 4u);
					mem_write32(t59, mem_read32(r10));
					mem_write32(t59 + 4u, t60);
					r10 += 8u;
					t61 = r11;
					r11 += 8u;
					t62 = mem_read32(r10 + 4u);
					mem_write32(t61, mem_read32(r10));
					mem_write32(t61 + 4u, t62);
					r10 += 8u;
					t63 = r11;
					r11 += 8u;
					t64 = mem_read32(r10 + 4u);
					mem_write32(t63, mem_read32(r10));
					mem_write32(t63 + 4u, t64);
					r1--;
					r10 += 8u;
				} while (r1 != 0u);
				return;
			}
			t65 = mem_read8(0x80000Cu);
			if (t65 != 0u)
			{
				sub_00D2EB();
				return;
			}
			r10 = 0x800020u;
			r11 = 0x20FC00u;
			r1 = 8u;
			t66 = mem_read8(0x80000Eu);
			if (t66 == 0u)
			{
				sub_00E67C(r1, r10, r11);
				return;
			}
			r10 += 0x700u;
			sub_00E67C(r1, r10, r11);
			return;
		}
		r10 = 0x21E000u;
		r11 = 0x20FC00u;
		r1 = 4u;
		L_00E854 = true;
	}
}

void sub_00E638(void)
{
	uint32_t r1 = 0, r10 = 0, r11 = 0;

	r10 = 0x800600u;
	r11 = 0x21E000u;
	r1 = 8u;
	sub_00E67C(r1, r10, r11);
}

void sub_00E67C(uint32_t r1, uint32_t r10, uint32_t r11)
{
	do
	{
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11 += 0x11u;
		r1--;
		r10 += 2u;
	} while (r1 != 0u);
}

void sub_00E8DF(void)
{
	uint32_t r1 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0;

	if (unk_20E623 == 0u)
	{
		unk_20E046 = unk_20E038;
		if (unk_20E043 != 0u || unk_20E042 != 0u)
			return;
		t1 = mem_read8(0x80000Cu);
		if (t1 != 0u)
		{
			sub_00D2EB();
			return;
		}
		r10 = 0x20FC00u;
		r11 = 0x800FE0u;
		r1 = 2u;
		do
		{
			r10 = r10 - 0x10u - 1u;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r1--;
		} while (r1 != 0u);
		mem_write8(0x800006u, 0xFFu);
	}
}

uint32_t sub_00EAE4(uint32_t r0)
{
	uint32_t r2 = 0, r10 = 0, r11 = 0, left_caller = 0;
	uint32_t target1 = 0;

	if ((OBJECT(r20)->unk_00 & 2u) != 0u)
		return r0;
	r0 = rom_00EB44[OBJECT(r20)->unk_3E];
	r10 = mem_read32(r25 + 0xC0u) + 0x10u;
	if (mem_read32(r25 + 0xC4u) <= r10)
		return r0;
	left_caller = 0u;
	target1 = r0;
	switch (target1)
	{
	case 0x00EF32u:
		sub_00EF32(&r0, &r10);
		break;
	case 0x00F104u:
		r10 = sub_00F104(r10);
		break;
	case 0x00EB74u:
		sub_00EB74(&r0, &r10, &left_caller);
		break;
	case 0x00EEBFu:
		sub_00EEBF(&r0, &r10, &left_caller);
		break;
	case 0x00EE37u:
		sub_00EE37(&r0, &r10, &left_caller);
		break;
	case 0x00EC1Bu:
		sub_00EC1B(&r0, &r10, &left_caller);
		break;
	case 0x00F126u:
		r10 = sub_00F126(r10);
		break;
	case 0x00ECCDu:
		sub_00ECCD(&r0, &r10, &left_caller);
		break;
	case 0x00F04Eu:
		sub_00F04E(&r0, &r10);
		break;
	case 0x00EE75u:
		sub_00EE75(&r0, &r10, &left_caller);
		break;
	case 0x00F15Cu:
		sub_00F15C(&r0, &r10);
		break;
	case 0x00ED82u:
		sub_00ED82(&r0, &r10, &left_caller);
		break;
	default:
		invalid_jump(target1);
	}
	if (left_caller != 0u)
		return r0;
	r0 = OBJECT(r20)->unk_3C;
	r11 = r25 + 0x5000u + r0 * 4u;
	if (mem_read16(r11) == 0u)
		mem_write16(r11 + 2u, r10);
	r12 = mem_read32(r25 + 0xC0u) + 0x10u;
	r2 = mem_read16(r11);
	mem_write32(r10, r2);
	mem_write32(r10 + 0x3000u, (uint16_t)(r2 + 0x300u));
	mem_write32(r25 + 0xC0u, r10);
	r12 = r12 >> 4;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x8000u);
	mem_write16(r11, r12);
	mem_write16(r25 + 0xB0u, mem_read16(r25 + 0xB0u) + 1u);
	return r0;
}

void sub_00EB74(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0;
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
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = r25 + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(r20)->unk_20 * r2) >> 12;
	r1 = (int32_t)((int16_t)OBJECT(r20)->unk_24 * r2) >> 12;
	r4 = (int32_t)((int16_t)OBJECT(r20)->unk_20 * r3) >> 12;
	r5 = (int32_t)((int16_t)OBJECT(r20)->unk_24 * r3) >> 12;
	r7 = (uint16_t)OBJECT(r20)->unk_28;
	r6 = (uint16_t)(0x80u - r7);
	r4 = ((r4 & ~0xFFFFu) | (uint16_t)(r4 + mem_read16(r12 + 4u))) * r7;
	r0 = (int32_t)(((r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u))) * r6 + r4) >> 7;
	r5 = ((r5 & ~0xFFFFu) | (uint16_t)(r5 + mem_read16(r12 + 6u))) * r7;
	r1 = (int32_t)(((r1 & ~0xFFFFu) | (uint16_t)(r1 + mem_read16(r11 + 6u))) * r6 + r5) >> 7;
	r2 = (r2 * r4 + r3 * r5) >> 7;
	sub_00EEFF((uint16_t)r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00EC1B(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	r1 = r0;
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
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = r25 + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(r20)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(r20)->unk_20 + (uint32_t)mem_read16(r25 + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(r20)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (r2 * r4 + r3 * r5) >> 7;
	sub_00EEF4(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00ECCD(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	r1 = r0;
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
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = r25 + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(r20)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(r20)->unk_20 + (uint32_t)mem_read16(r25 + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(r20)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (int32_t)(r2 * r4 + r3 * r5) >> 11;
	sub_00EEFF(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

void sub_00ED82(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(r20)->unk_1C;
	r1 = r0;
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
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = r25 + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(r20)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(r20)->unk_20 + (uint32_t)mem_read16(r25 + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(r20)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (int32_t)(r2 * r4 + r3 * r5) >> 10;
	sub_00EEFF(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

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
	move_string(0x10B12u, 0x47u, unk_20E202_ADDRESS, 0x47u, 2u, 0u, 0u, r26, &r27, &r28);
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

void sub_01078F(uint16_t r9)
{
	uint16_t r7 = 0, r9_2 = 0;
	uint16_t t1 = 0;

	r7 = (uint16_t)r9;
	do
	{
		r9_2 = (uint16_t)r7;
		sub_0107A0(r9_2);
		t1 = r7;
		r7 = (uint16_t)(t1 - 0x400u);
	} while ((int16_t)t1 >= 0x400);
}

void sub_0107A0(uint16_t r9)
{
	move_string(0x100800u, 0x200u, 0x300000u | (uint16_t)r9, 0x200u, 2u, 0u, 0u, r26, &r27, &r28);
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

void sub_010996(uint16_t r0, uint32_t r10, uint32_t r11)
{
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0, t33 = 0, t34 = 0, t35 = 0, t36 = 0, t37 = 0, t38 = 0, t39 = 0, t40 = 0, t41 = 0, t42 = 0, t43 = 0, t44 = 0, t45 = 0, t46 = 0, t47 = 0, t48 = 0, t49 = 0, t50 = 0, t51 = 0, t52 = 0, t53 = 0, t54 = 0, t55 = 0, t56 = 0, t57 = 0, t58 = 0, t59 = 0, t60 = 0, t61 = 0, t62 = 0, t63 = 0, t64 = 0, t65 = 0, t66 = 0, t67 = 0, t68 = 0, t69 = 0, t70 = 0, t71 = 0, t72 = 0, t73 = 0, t74 = 0, t75 = 0, t76 = 0, t77 = 0, t78 = 0, t79 = 0, t80 = 0, t81 = 0, t82 = 0, t83 = 0, t84 = 0, t85 = 0, t86 = 0, t87 = 0, t88 = 0, t89 = 0, t90 = 0, t91 = 0, t92 = 0, t93 = 0, t94 = 0, t95 = 0, t96 = 0, t97 = 0, t98 = 0, t99 = 0, t100 = 0, t101 = 0, t102 = 0, t103 = 0, t104 = 0, t105 = 0, t106 = 0, t107 = 0, t108 = 0, t109 = 0, t110 = 0, t111 = 0, t112 = 0, t113 = 0, t114 = 0, t115 = 0, t116 = 0, t117 = 0, t118 = 0, t119 = 0, t120 = 0, t121 = 0, t122 = 0, t123 = 0, t124 = 0, t125 = 0, t126 = 0, t127 = 0;

	switch (0u - (((r0 & 0x3F8u) >> 1) - 0x200u))
	{
	case 0x0u:
		break;
	default:
		invalid_jump(0x109B4u + (0u - (((r0 & 0x3F8u) >> 1) - 0x200u)));
	}
	t1 = r11;
	r11 += 8u;
	t2 = mem_read32(r10 + 4u);
	mem_write32(t1, mem_read32(r10));
	mem_write32(t1 + 4u, t2);
	r10 += 8u;
	t3 = r11;
	r11 += 8u;
	t4 = mem_read32(r10 + 4u);
	mem_write32(t3, mem_read32(r10));
	mem_write32(t3 + 4u, t4);
	r10 += 8u;
	t5 = r11;
	r11 += 8u;
	t6 = mem_read32(r10 + 4u);
	mem_write32(t5, mem_read32(r10));
	mem_write32(t5 + 4u, t6);
	r10 += 8u;
	t7 = r11;
	r11 += 8u;
	t8 = mem_read32(r10 + 4u);
	mem_write32(t7, mem_read32(r10));
	mem_write32(t7 + 4u, t8);
	r10 += 8u;
	t9 = r11;
	r11 += 8u;
	t10 = mem_read32(r10 + 4u);
	mem_write32(t9, mem_read32(r10));
	mem_write32(t9 + 4u, t10);
	r10 += 8u;
	t11 = r11;
	r11 += 8u;
	t12 = mem_read32(r10 + 4u);
	mem_write32(t11, mem_read32(r10));
	mem_write32(t11 + 4u, t12);
	r10 += 8u;
	t13 = r11;
	r11 += 8u;
	t14 = mem_read32(r10 + 4u);
	mem_write32(t13, mem_read32(r10));
	mem_write32(t13 + 4u, t14);
	r10 += 8u;
	t15 = r11;
	r11 += 8u;
	t16 = mem_read32(r10 + 4u);
	mem_write32(t15, mem_read32(r10));
	mem_write32(t15 + 4u, t16);
	r10 += 8u;
	t17 = r11;
	r11 += 8u;
	t18 = mem_read32(r10 + 4u);
	mem_write32(t17, mem_read32(r10));
	mem_write32(t17 + 4u, t18);
	r10 += 8u;
	t19 = r11;
	r11 += 8u;
	t20 = mem_read32(r10 + 4u);
	mem_write32(t19, mem_read32(r10));
	mem_write32(t19 + 4u, t20);
	r10 += 8u;
	t21 = r11;
	r11 += 8u;
	t22 = mem_read32(r10 + 4u);
	mem_write32(t21, mem_read32(r10));
	mem_write32(t21 + 4u, t22);
	r10 += 8u;
	t23 = r11;
	r11 += 8u;
	t24 = mem_read32(r10 + 4u);
	mem_write32(t23, mem_read32(r10));
	mem_write32(t23 + 4u, t24);
	r10 += 8u;
	t25 = r11;
	r11 += 8u;
	t26 = mem_read32(r10 + 4u);
	mem_write32(t25, mem_read32(r10));
	mem_write32(t25 + 4u, t26);
	r10 += 8u;
	t27 = r11;
	r11 += 8u;
	t28 = mem_read32(r10 + 4u);
	mem_write32(t27, mem_read32(r10));
	mem_write32(t27 + 4u, t28);
	r10 += 8u;
	t29 = r11;
	r11 += 8u;
	t30 = mem_read32(r10 + 4u);
	mem_write32(t29, mem_read32(r10));
	mem_write32(t29 + 4u, t30);
	r10 += 8u;
	t31 = r11;
	r11 += 8u;
	t32 = mem_read32(r10 + 4u);
	mem_write32(t31, mem_read32(r10));
	mem_write32(t31 + 4u, t32);
	r10 += 8u;
	t33 = r11;
	r11 += 8u;
	t34 = mem_read32(r10 + 4u);
	mem_write32(t33, mem_read32(r10));
	mem_write32(t33 + 4u, t34);
	r10 += 8u;
	t35 = r11;
	r11 += 8u;
	t36 = mem_read32(r10 + 4u);
	mem_write32(t35, mem_read32(r10));
	mem_write32(t35 + 4u, t36);
	r10 += 8u;
	t37 = r11;
	r11 += 8u;
	t38 = mem_read32(r10 + 4u);
	mem_write32(t37, mem_read32(r10));
	mem_write32(t37 + 4u, t38);
	r10 += 8u;
	t39 = r11;
	r11 += 8u;
	t40 = mem_read32(r10 + 4u);
	mem_write32(t39, mem_read32(r10));
	mem_write32(t39 + 4u, t40);
	r10 += 8u;
	t41 = r11;
	r11 += 8u;
	t42 = mem_read32(r10 + 4u);
	mem_write32(t41, mem_read32(r10));
	mem_write32(t41 + 4u, t42);
	r10 += 8u;
	t43 = r11;
	r11 += 8u;
	t44 = mem_read32(r10 + 4u);
	mem_write32(t43, mem_read32(r10));
	mem_write32(t43 + 4u, t44);
	r10 += 8u;
	t45 = r11;
	r11 += 8u;
	t46 = mem_read32(r10 + 4u);
	mem_write32(t45, mem_read32(r10));
	mem_write32(t45 + 4u, t46);
	r10 += 8u;
	t47 = r11;
	r11 += 8u;
	t48 = mem_read32(r10 + 4u);
	mem_write32(t47, mem_read32(r10));
	mem_write32(t47 + 4u, t48);
	r10 += 8u;
	t49 = r11;
	r11 += 8u;
	t50 = mem_read32(r10 + 4u);
	mem_write32(t49, mem_read32(r10));
	mem_write32(t49 + 4u, t50);
	r10 += 8u;
	t51 = r11;
	r11 += 8u;
	t52 = mem_read32(r10 + 4u);
	mem_write32(t51, mem_read32(r10));
	mem_write32(t51 + 4u, t52);
	r10 += 8u;
	t53 = r11;
	r11 += 8u;
	t54 = mem_read32(r10 + 4u);
	mem_write32(t53, mem_read32(r10));
	mem_write32(t53 + 4u, t54);
	r10 += 8u;
	t55 = r11;
	r11 += 8u;
	t56 = mem_read32(r10 + 4u);
	mem_write32(t55, mem_read32(r10));
	mem_write32(t55 + 4u, t56);
	r10 += 8u;
	t57 = r11;
	r11 += 8u;
	t58 = mem_read32(r10 + 4u);
	mem_write32(t57, mem_read32(r10));
	mem_write32(t57 + 4u, t58);
	r10 += 8u;
	t59 = r11;
	r11 += 8u;
	t60 = mem_read32(r10 + 4u);
	mem_write32(t59, mem_read32(r10));
	mem_write32(t59 + 4u, t60);
	r10 += 8u;
	t61 = r11;
	r11 += 8u;
	t62 = mem_read32(r10 + 4u);
	mem_write32(t61, mem_read32(r10));
	mem_write32(t61 + 4u, t62);
	r10 += 8u;
	t63 = r11;
	r11 += 8u;
	t64 = mem_read32(r10 + 4u);
	mem_write32(t63, mem_read32(r10));
	mem_write32(t63 + 4u, t64);
	r10 += 8u;
	t65 = r11;
	r11 += 8u;
	t66 = mem_read32(r10 + 4u);
	mem_write32(t65, mem_read32(r10));
	mem_write32(t65 + 4u, t66);
	r10 += 8u;
	t67 = r11;
	r11 += 8u;
	t68 = mem_read32(r10 + 4u);
	mem_write32(t67, mem_read32(r10));
	mem_write32(t67 + 4u, t68);
	r10 += 8u;
	t69 = r11;
	r11 += 8u;
	t70 = mem_read32(r10 + 4u);
	mem_write32(t69, mem_read32(r10));
	mem_write32(t69 + 4u, t70);
	r10 += 8u;
	t71 = r11;
	r11 += 8u;
	t72 = mem_read32(r10 + 4u);
	mem_write32(t71, mem_read32(r10));
	mem_write32(t71 + 4u, t72);
	r10 += 8u;
	t73 = r11;
	r11 += 8u;
	t74 = mem_read32(r10 + 4u);
	mem_write32(t73, mem_read32(r10));
	mem_write32(t73 + 4u, t74);
	r10 += 8u;
	t75 = r11;
	r11 += 8u;
	t76 = mem_read32(r10 + 4u);
	mem_write32(t75, mem_read32(r10));
	mem_write32(t75 + 4u, t76);
	r10 += 8u;
	t77 = r11;
	r11 += 8u;
	t78 = mem_read32(r10 + 4u);
	mem_write32(t77, mem_read32(r10));
	mem_write32(t77 + 4u, t78);
	r10 += 8u;
	t79 = r11;
	r11 += 8u;
	t80 = mem_read32(r10 + 4u);
	mem_write32(t79, mem_read32(r10));
	mem_write32(t79 + 4u, t80);
	r10 += 8u;
	t81 = r11;
	r11 += 8u;
	t82 = mem_read32(r10 + 4u);
	mem_write32(t81, mem_read32(r10));
	mem_write32(t81 + 4u, t82);
	r10 += 8u;
	t83 = r11;
	r11 += 8u;
	t84 = mem_read32(r10 + 4u);
	mem_write32(t83, mem_read32(r10));
	mem_write32(t83 + 4u, t84);
	r10 += 8u;
	t85 = r11;
	r11 += 8u;
	t86 = mem_read32(r10 + 4u);
	mem_write32(t85, mem_read32(r10));
	mem_write32(t85 + 4u, t86);
	r10 += 8u;
	t87 = r11;
	r11 += 8u;
	t88 = mem_read32(r10 + 4u);
	mem_write32(t87, mem_read32(r10));
	mem_write32(t87 + 4u, t88);
	r10 += 8u;
	t89 = r11;
	r11 += 8u;
	t90 = mem_read32(r10 + 4u);
	mem_write32(t89, mem_read32(r10));
	mem_write32(t89 + 4u, t90);
	r10 += 8u;
	t91 = r11;
	r11 += 8u;
	t92 = mem_read32(r10 + 4u);
	mem_write32(t91, mem_read32(r10));
	mem_write32(t91 + 4u, t92);
	r10 += 8u;
	t93 = r11;
	r11 += 8u;
	t94 = mem_read32(r10 + 4u);
	mem_write32(t93, mem_read32(r10));
	mem_write32(t93 + 4u, t94);
	r10 += 8u;
	t95 = r11;
	r11 += 8u;
	t96 = mem_read32(r10 + 4u);
	mem_write32(t95, mem_read32(r10));
	mem_write32(t95 + 4u, t96);
	r10 += 8u;
	t97 = r11;
	r11 += 8u;
	t98 = mem_read32(r10 + 4u);
	mem_write32(t97, mem_read32(r10));
	mem_write32(t97 + 4u, t98);
	r10 += 8u;
	t99 = r11;
	r11 += 8u;
	t100 = mem_read32(r10 + 4u);
	mem_write32(t99, mem_read32(r10));
	mem_write32(t99 + 4u, t100);
	r10 += 8u;
	t101 = r11;
	r11 += 8u;
	t102 = mem_read32(r10 + 4u);
	mem_write32(t101, mem_read32(r10));
	mem_write32(t101 + 4u, t102);
	r10 += 8u;
	t103 = r11;
	r11 += 8u;
	t104 = mem_read32(r10 + 4u);
	mem_write32(t103, mem_read32(r10));
	mem_write32(t103 + 4u, t104);
	r10 += 8u;
	t105 = r11;
	r11 += 8u;
	t106 = mem_read32(r10 + 4u);
	mem_write32(t105, mem_read32(r10));
	mem_write32(t105 + 4u, t106);
	r10 += 8u;
	t107 = r11;
	r11 += 8u;
	t108 = mem_read32(r10 + 4u);
	mem_write32(t107, mem_read32(r10));
	mem_write32(t107 + 4u, t108);
	r10 += 8u;
	t109 = r11;
	r11 += 8u;
	t110 = mem_read32(r10 + 4u);
	mem_write32(t109, mem_read32(r10));
	mem_write32(t109 + 4u, t110);
	r10 += 8u;
	t111 = r11;
	r11 += 8u;
	t112 = mem_read32(r10 + 4u);
	mem_write32(t111, mem_read32(r10));
	mem_write32(t111 + 4u, t112);
	r10 += 8u;
	t113 = r11;
	r11 += 8u;
	t114 = mem_read32(r10 + 4u);
	mem_write32(t113, mem_read32(r10));
	mem_write32(t113 + 4u, t114);
	r10 += 8u;
	t115 = r11;
	r11 += 8u;
	t116 = mem_read32(r10 + 4u);
	mem_write32(t115, mem_read32(r10));
	mem_write32(t115 + 4u, t116);
	r10 += 8u;
	t117 = r11;
	r11 += 8u;
	t118 = mem_read32(r10 + 4u);
	mem_write32(t117, mem_read32(r10));
	mem_write32(t117 + 4u, t118);
	r10 += 8u;
	t119 = r11;
	r11 += 8u;
	t120 = mem_read32(r10 + 4u);
	mem_write32(t119, mem_read32(r10));
	mem_write32(t119 + 4u, t120);
	r10 += 8u;
	t121 = r11;
	r11 += 8u;
	t122 = mem_read32(r10 + 4u);
	mem_write32(t121, mem_read32(r10));
	mem_write32(t121 + 4u, t122);
	r10 += 8u;
	t123 = r11;
	r11 += 8u;
	t124 = mem_read32(r10 + 4u);
	mem_write32(t123, mem_read32(r10));
	mem_write32(t123 + 4u, t124);
	r10 += 8u;
	t125 = r11;
	r11 += 8u;
	t126 = mem_read32(r10 + 4u);
	mem_write32(t125, mem_read32(r10));
	mem_write32(t125 + 4u, t126);
	r10 += 8u;
	t127 = mem_read32(r10 + 4u);
	mem_write32(r11, mem_read32(r10));
	mem_write32(r11 + 4u, t127);
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

void sub_010F66(uint32_t r10)
{
	uint32_t r10_2 = 0, r11 = 0;
	uint32_t t1 = 0;

	do
	{
		r12 = mem_read32(r10);
		r10 += 4u;
		r11 = mem_read32(r10);
		t1 = r10 + 4u;
		r10_2 = r12;
		r12 = t1;
		sub_010DC4(r10_2, r11);
		r10 = r12;
	} while ((int32_t)mem_read32(r10) >= 0);
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

void sub_010FA6(uint32_t r10)
{
	uint32_t r0 = 0, r11 = 0;
	uint32_t t1 = 0;

	do
	{
		r0 = mem_read8(r25 + 0xB03u);
		r11 = r25 + 0xC00u + r0 * 2u * 4u;
		mem_write8(r25 + 0xB03u, (r0 + 1u) & 0x3Fu);
		t1 = mem_read32(r10 + 4u);
		mem_write32(r11, mem_read32(r10));
		mem_write32(r11 + 4u, t1);
		mem_write8(r25 + 0xB00u, 0xFFu);
		r10 += 8u;
	} while ((int32_t)mem_read32(r10) >= 0);
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

void sub_011314(uint32_t r7)
{
	if (mem_read8(r7) != 0xFFu)
	{
		mem_write8(r7, mem_read8(r7) + 1u);
		sub_011418(0u);
		mem_write8(r7 + 0x7Cu, mem_read8(r7 + 0x7Cu) + 1u);
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

void sub_011362(uint32_t r7, uint8_t r9)
{
	uint32_t carry = 0;
	uint8_t t1 = 0;

	t1 = mem_read8(r7);
	carry = (uint8_t)(t1 + r9) < t1;
	mem_write8(r7, t1 + r9);
	if (carry)
	{
		mem_write8(r7, 0xFFu);
		sub_011418(0u);
		mem_write8(r7, 0xFFu);
		return;
	}
	sub_011418(0u);
	mem_write8(r7 + 0x7Cu, mem_read8(r7 + 0x7Cu) + r9);
}

void sub_01137D(uint32_t r7, uint16_t r9)
{
	mem_write16(r7, r9);
	sub_011418(0u);
	mem_write16(r7 + 0x7Cu, r9);
}

void sub_011388(uint32_t r7, uint32_t r9)
{
	mem_write32(r7, r9);
	sub_011418(0u);
	mem_write32(r7 + 0x7Cu, r9);
}

void sub_011393(uint32_t r7, uint8_t r9)
{
	mem_write8(r7, r9);
	sub_011418(0u);
	mem_write8(r7 + 0x7Cu, r9);
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

void sub_01142A(void)
{
	uint32_t r0 = 0, r2 = 0;

	r2 = SOUND_RAM + 0x1F02u;
	r0 = sub_01143E(0u, r2);
	if (mem_read16(SOUND_701F00) != (uint16_t)r0)
	{
		r0 = sub_0113E5(r0);
		return;
	}
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

void sub_0117FE(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r18 = 0, r19 = 0;

	r19 = 0x103C20u;
	r18 = VIDEO_RAM + 0x18000u;
	r5 = 1u;
	r1 = 0x80u;
	do
	{
		r3 = 8u;
		r6 = 0u;
		do
		{
			r0 = ROM8(r19);
			r2 = 4u;
			r4 = 0u;
			r19++;
			do
			{
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r4 = rotate_with_carry_by(r4, 4u, carry, 32);
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r4 = rotate_with_carry_by(r4, 4u, carry, 32);
				r2--;
			} while (r2 != 0u);
			r4 = (r4 << 29) | (r4 >> 3);
			r7 = r4;
			r0_2 = r4 * 0xFu;
			r4 = r4 * r5;
			mem_write32(r18, byte_swap32((r6 & ~r0_2) | r4));
			r6 = (r4 + r7) >> 4;
			r18 += 4u;
			r3--;
		} while (r3 != 0u);
		r1--;
	} while (r1 != 0u);
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

void sub_0118BD(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r9 = 0u;
	do
	{
		mem_write16(r18, r9);
		r18 += 2u;
	} while (mem_read8(r18) != 0u);
}

void sub_0118D2(uint32_t r19)
{
	uint16_t r8 = 0, r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r19 += 2u;
	r9 = (uint16_t)((uint32_t)mem_read8(r19) << 8);
	r19++;
	r8 = mem_read16(r19);
	r19 += 2u;
	if (r8 != 0u)
	{
		do
		{
			mem_write16(r18, r8 | r9);
			r8 = mem_read16(r19);
			r18 += 2u;
			r19 += 2u;
		} while (r8 != 0u);
	}
}

void sub_0118F7(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r9 = 0u;
	do
	{
		mem_write16(r18, r9);
		r18 += 2u;
	} while (mem_read16(r18) != 0u);
}

void sub_01190C(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 = (uint8_t)r0;
	r2 = 2u;
	sub_01192B(r0, r1, r2, r11);
}

void sub_011914(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 = (uint16_t)r0;
	r2 = 4u;
	sub_01192B(r0, r1, r2, r11);
}

void sub_01191C(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 &= 0xFFFFFFu;
	r2 = 6u;
	sub_01192B(r0, r1, r2, r11);
}

void sub_011928(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r2 = 8u;
	sub_01192B(r0, r1, r2, r11);
}

void sub_01192B(uint32_t r0, uint16_t r1, uint32_t r2, uint32_t r11)
{
	uint32_t t1 = 0;

	r11 += r2 * 2u;
	do
	{
		r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)r0);
		r1 = (uint16_t)((r1 & ~0xFFu) | (r1 & 0xFu));
		if ((uint8_t)r1 >= 0xAu)
			r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)(r1 + 7u));
		r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)(r1 + 0x30u));
		r11 -= 2u;
		mem_write16(r11, r1);
		t1 = r0 >> 4;
		r0 = t1;
		r2--;
	} while ((r2 != 0u) & (t1 != 0u));
	if (r2 != 0u)
	{
		do
		{
			r11 -= 2u;
			mem_write16(r11, r0);
			r2--;
		} while (r2 != 0u);
	}
}

void sub_011958(void)
{
	uint32_t r2 = 0;

	r2 = 8u;
	do
	{
		r2--;
	} while (r2 != 0u);
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

void sub_011DEB(void)
{
	uint32_t zero = 0;

	search_bytes(r25 - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
		return;
	mem_write8(r28, 0xFFu);
	r26 = 0x80u;
	move_string(0x100800u, 0x80u, (r28 & ~0xFFFFu) | (uint16_t)(r28 << 8), 0x80u, 2u, 0u, 0u, r26, &r27, &r28);
}

void sub_011E1D(uint16_t r21)
{
	mem_write8(r25 - 0x7FF0u + ((uint16_t)r21 >> 8), 0u);
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

void sub_011EF4(uint8_t r0)
{
	uint32_t r1 = 0, r2 = 0;

	if ((mem_read32(r25 + 0x90u) & 0x100u) == 0u)
	{
		r2 = r25 + 0x841u;
		r1 = mem_read8(r2);
		r2++;
		mem_write8(r2 + r1, r0);
		mem_write8(r2 - 1u, (r1 + 1u) & 0x1Fu);
	}
	else if (unk_20E60D != 0u)
	{
		r2 = (uint8_t)r0;
		if ((rom_01204E[r2 >> 5] & shl32(1u, (int)(r2 & 0x1Fu))) != 0u)
		{
			r2 = r25 + 0x841u;
			r1 = mem_read8(r2);
			r2++;
			mem_write8(r2 + r1, r0);
			mem_write8(r2 - 1u, (r1 + 1u) & 0x1Fu);
			return;
		}
	}
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
