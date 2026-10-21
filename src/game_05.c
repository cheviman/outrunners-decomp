/**
 *    Name:        game_05.c
 *    Description: the game's functions, part 05 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

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
