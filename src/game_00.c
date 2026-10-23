/**
 *    Name:        game_00.c
 *    Description: the game's functions, part 00 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

void boot(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0, r11 = 0;
	uint32_t stack_100004 = 0;
	uint32_t psw1 = 0;

	load_privileged(5u, vector_table_ADDRESS);
	r10 = 0xA4u;
	r11 = 0xE00000u;
	r1 = 8u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10++;
		r11++;
		mem_write8(r11, mem_read8(r10));
		r1--;
		r10++;
		r11++;
	} while (r1 != 0u);
	mem_write8(0xE80000u, rom_0000B4);
	mem_write8(0xE80001u, rom_0000B5);
	mem_write8(0xE80002u, rom_0000B6);
	mem_write8(0xE80003u, rom_0000B7);
	r11 = INTERRUPT_CONTROL;
	r1 = 6u;
	r10 = 0xB8u;
	do
	{
		mem_write8(r11, mem_read8(r10));
		r10++;
		r11++;
		mem_write8(r11, mem_read8(r10));
		r1--;
		r10++;
		r11++;
	} while (r1 != 0u);
	stack_100004 = 0u;
	psw1 = stack_100004;
	psw_write(psw1);
	load_privileged(2u, 0x210000u);
	load_privileged(3u, 0x210000u);
	load_privileged(4u, 0x210000u);
	r0 = 0x10000u;
	do
	{
		r0--;
	} while (r0 != 0u);
	r0 = 0u;
	r1 = 0u;
	r2 = 0x20u;
	r10 = 0x21E000u;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	mem_write32(0x801000u, 0u);
	r1 = 0x16000u;
	do
	{
		r1--;
	} while (r1 != 0u);
	r0 = 0u;
	r1 = 0u;
	r10 = 0x21F000u;
	r2 = 0x80u;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	sub_0108EA();
	r0 = 0u;
	r1 = 0u;
	r2 = 0x100u;
	r10 = unk_20E000_ADDRESS;
	do
	{
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		mem_write32(r10, r0);
		mem_write32(r10 + 4u, r1);
		r10 += 8u;
		r2--;
	} while (r2 != 0u);
	move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
	player_car = 0x219000u;
	unk_20E000 = 0x21FF84u;
	unk_20E004 = 0x21F800u;
	sub_000913();
	sub_0186FC();
	mem_write8(IRQ_D00007, 0u);
	mem_write8(IRQ_D00005, 0u);
	interrupts_on();
	sub_00D13A();
	second_task(1u);
}

uint32_t sub_0000C4(uint32_t r10, uint32_t r11)
{
	uint32_t carry = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0;

	r2 = 0x10210000u;
	r0 = 0xDEBDEB00u;
	do
	{
		r0 = (r0 & ~0xFFu) | mem_read8(r10);
		r1 = 8u;
		r10++;
		do
		{
			carry = r0 * 2u < r0;
			r0 = r0 * 2u;
			if (carry)
				r0 ^= r2;
			r1--;
		} while (r1 != 0u);
		r11--;
	} while (r11 != 0u);
	r1 = 8u;
	do
	{
		carry = r0 * 2u < r0;
		r0 = r0 * 2u;
		if (carry)
			r0 ^= r2;
		r1--;
	} while (r1 != 0u);
	r0 = r0 >> 16;
	return r0;
}

uint32_t sub_0000FC(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = 0x10Cu;
	r11 = 0x30u;
	return sub_0000C4(r10, r11);
}

void sub_00013C(uint32_t r0)
{
	uint16_t r0_3 = 0;
	uint32_t r0_2 = 0, r10 = 0, r11 = 0;

	sub_000337(r0);
	if (unk_20E600 == 0x41474553u)
	{
		sub_0113E5(r0);
		move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
		unk_20E60A = 0x64u;
		unk_20E646 = 0x64u;
		r0_2 = sub_0000FC();
		unk_20E604 = r0_2;
		r10 = unk_20E60A_ADDRESS;
		r11 = 0x3Au;
		r0_2 = sub_0000C4(r10, r11);
		unk_20E608 = r0_2;
		unk_20E644 = r0_2;
		sub_000375();
		return;
	}
	if (unk_20E600 != 0x41323353u)
	{
		sub_0113E5(r0);
		move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
		r0_2 = sub_0000FC();
		unk_20E604 = r0_2;
		r10 = unk_20E60A_ADDRESS;
		r11 = 0x3Au;
		r0_2 = sub_0000C4(r10, r11);
		unk_20E608 = r0_2;
		unk_20E644 = r0_2;
		sub_000375();
		return;
	}
	r0_2 = sub_0000FC();
	if (unk_20E604 != (uint16_t)r0_2 || unk_20E604 != (uint16_t)r0_2)
	{
		unk_20E604 = r0_2;
		sub_0113E5(r0_2);
		r10 = unk_20E60A_ADDRESS;
		r11 = 0x3Au;
		r0_2 = sub_0000C4(r10, r11);
		r10 = unk_20E608_ADDRESS;
		if (unk_20E608 == (uint16_t)r0_2)
		{
			r0_3 = mem_read16(r10 + 2u);
			move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
			unk_20E60A = r0_3;
			unk_20E646 = r0_3;
			r0_2 = sub_0000FC();
			unk_20E604 = r0_2;
			r10 = unk_20E60A_ADDRESS;
			r11 = 0x3Au;
			r0_2 = sub_0000C4(r10, r11);
			unk_20E608 = r0_2;
			unk_20E644 = r0_2;
			sub_000375();
			return;
		}
		r10 = unk_20E646_ADDRESS;
		r11 = 0x3Au;
		r0 = sub_0000C4(r10, r11);
		r10 = unk_20E644_ADDRESS;
		if (unk_20E644 != (uint16_t)r0)
		{
			sub_0113E5(r0);
			move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
			r0_2 = sub_0000FC();
			unk_20E604 = r0_2;
			r10 = unk_20E60A_ADDRESS;
			r11 = 0x3Au;
			r0_2 = sub_0000C4(r10, r11);
			unk_20E608 = r0_2;
			unk_20E644 = r0_2;
			sub_000375();
			return;
		}
		r0_3 = mem_read16(r10 + 2u);
		move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
		unk_20E60A = r0_3;
		unk_20E646 = r0_3;
		r0_2 = sub_0000FC();
		unk_20E604 = r0_2;
		r10 = unk_20E60A_ADDRESS;
		r11 = 0x3Au;
		r0_2 = sub_0000C4(r10, r11);
		unk_20E608 = r0_2;
		unk_20E644 = r0_2;
		sub_000375();
		return;
	}
	r10 = unk_20E60A_ADDRESS;
	r11 = 0x3Au;
	r0_2 = sub_0000C4(r10, r11);
	if (unk_20E608 != (uint16_t)r0_2)
	{
		r10 = unk_20E646_ADDRESS;
		r11 = 0x3Au;
		r0 = sub_0000C4(r10, r11);
		if (unk_20E644 == (uint16_t)r0)
		{
			move_halfwords(unk_20E608_ADDRESS, unk_20E644_ADDRESS, 0x1Eu);
			sub_000375();
			return;
		}
		sub_0113E5(r0);
		move_halfwords(unk_20E600_ADDRESS, 0x4C0u, 0x40u);
		r0_2 = sub_0000FC();
		unk_20E604 = r0_2;
		r10 = unk_20E60A_ADDRESS;
		r11 = 0x3Au;
		r0_2 = sub_0000C4(r10, r11);
		unk_20E608 = r0_2;
		unk_20E644 = r0_2;
		sub_000375();
		return;
	}
	r10 = unk_20E646_ADDRESS;
	r11 = 0x3Au;
	r0_2 = sub_0000C4(r10, r11);
	if (unk_20E608 != (uint16_t)r0_2 || unk_20E644 != (uint16_t)r0_2)
	{
		move_halfwords(unk_20E644_ADDRESS, unk_20E608_ADDRESS, 0x1Eu);
		sub_000375();
		return;
	}
}

void sub_0002CA(void)
{
	unk_20E60A++;
	if (unk_20E60A >= 0x2710u)
		unk_20E60A = 0x2710u;
}

void sub_0002E1(void)
{
	uint8_t r4 = 0;
	uint32_t r2 = 0, r13 = 0;

	r4 = 0u;
	sub_0004A1(&r2, &r13);
	sub_000451(r2, r4, r13);
	interrupts_on();
}

void sub_000310(uint8_t r4)
{
	uint32_t r2 = 0, r13 = 0, ignored = 0;

	r4 |= 0x80u;
	sub_0004A1(&r2, &r13);
	sub_0003BA(r4, r13, &ignored, &r2);
	interrupts_on();
}

void sub_000327(uint32_t r0)
{
	sub_000337(r0);
	interrupts_on();
}

void sub_000337(uint32_t r0)
{
	uint8_t r4 = 0;
	uint32_t r2 = 0, r5 = 0, r11 = 0, r13 = 0;

	r11 = unk_20E600_ADDRESS;
	r4 = 0x80u;
	r5 = 0x40u;
	sub_0004A1(&r2, &r13);
	do
	{
		sub_0003BA(r4, r13, &r0, &r2);
		r4++;
		mem_write16(r11, r0);
		r5--;
		r11 += 2u;
	} while (r5 != 0u);
}

void sub_000354(uint16_t r0, uint8_t r4)
{
	uint32_t r2 = 0, r13 = 0;

	r4 |= 0x40u;
	sub_0004A1(&r2, &r13);
	sub_0003ED(r0, r2, r4, r13);
	interrupts_on();
}

void sub_00036B(void)
{
	sub_000372();
	sub_0002E1();
}

void sub_000372(void)
{
	sub_0002CA();
	sub_000375();
}

void sub_000375(void)
{
	uint8_t r4 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r2 = 0, r5 = 0, r10 = 0, r11 = 0, r13 = 0;

	r10 = unk_20E60A_ADDRESS;
	r11 = 0x3Au;
	r0 = sub_0000C4(r10, r11);
	unk_20E608 = r0;
	move_string(unk_20E608_ADDRESS, 0x1Eu, unk_20E644_ADDRESS, 0x1Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	r4 = 0x30u;
	sub_0004A1(&r2, &r13);
	sub_000451(r2, r4, r13);
	r10 = unk_20E600_ADDRESS;
	r4 = 0x40u;
	r5 = 0x40u;
	sub_0004A1(&r2, &r13);
	do
	{
		r0_2 = mem_read16(r10);
		r10 += 2u;
		r2 = sub_0003ED(r0_2, r2, r4, r13);
		r4++;
		r5--;
	} while (r5 != 0u);
}

void sub_0003BA(uint8_t r4, uint32_t r13, uint32_t *p_r0, uint32_t *p_r2)
{
	uint32_t r0 = *p_r0;
	uint32_t r2 = *p_r2;
	uint32_t r3 = 0;

	r2 = sub_000451(r2, r4, r13);
	r2 = (r2 & 0x3Fu) | 0x20u;
	r3 = 0x12u;
	do
	{
		r2 = sub_0004B8(r2, r13);
		r0 = ((r0 & ~0xFFu) | mem_read8(r12)) * 2u;
		r2 = (uint8_t)(r2 ^ 0x40u);
		r2 = sub_0004B8(r2, r13);
		r2 = (uint8_t)(r2 ^ 0x40u);
		r3--;
	} while (r3 != 0u);
	r2 = r2 & 0x1Fu;
	r2 = sub_0004B8(r2, r13);
	r0 = r0 >> 9;
	*p_r0 = r0;
	*p_r2 = (*p_r2 & 0xFFFFFF00u) | (r2 & 0xFFu);
}

uint32_t sub_0003ED(uint32_t r0, uint32_t r2, uint8_t r4, uint32_t r13)
{
	uint8_t r0_2 = 0;
	uint16_t r6 = 0;
	uint32_t r3 = 0;

	r6 = (uint16_t)r0;
	r2 = sub_000451(r2, r4, r13);
	r6 = (uint16_t)(((uint32_t)r6 << 8) | (r6 >> 8));
	r3 = 0x10u;
	do
	{
		r0 = (r0 & ~0xFFu) | (uint8_t)r6;
		r0 = (r0 & ~0xFFu) | (r0 & 0x80u);
		r6 = (uint16_t)(((uint32_t)r6 << 1) | (r6 >> 15));
		r2 = ((r2 & ~0xFFu) | (r2 & 0x1Fu)) + 0x20u + r0;
		r2 = sub_0004B8(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = sub_0004B8(r2, r13);
		r3--;
	} while (r3 != 0u);
	r3 = 2u;
	do
	{
		r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
		r2 = sub_0004B8(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = sub_0004B8(r2, r13);
		r3--;
	} while (r3 != 0u);
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	do
	{
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | 0x20u);
		r2 = sub_0004B8(r2, r13);
		r0_2 = mem_read8(r12);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = sub_0004B8(r2, r13);
	} while ((int8_t)r0_2 >= 0);
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	r2 = sub_0004B8(r2, r13);
	return r2;
}

uint32_t sub_000451(uint32_t r2, uint8_t r4, uint32_t r13)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r3 = 0;

	r1 = (uint8_t)r4;
	r2 = (r2 & ~0xFFu) | (r2 & 0x1Fu);
	r2 = sub_0004B8(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = sub_0004B8(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x60u);
	r2 = sub_0004B8(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = sub_0004B8(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0xC0u);
	r2 = sub_0004B8(r2, r13);
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
	r2 = sub_0004B8(r2, r13);
	r3 = 8u;
	do
	{
		r0 = (uint8_t)r1;
		r1 = (uint8_t)(r1 * 2u);
		r2 = (r2 & ~0xFFu) | (r2 & 0x3Fu);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | (r0 & 0x80u));
		r2 = sub_0004B8(r2, r13);
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 ^ 0x40u);
		r2 = sub_0004B8(r2, r13);
		r3--;
	} while (r3 != 0u);
	return r2;
}

void sub_0004A1(uint32_t *p_r2, uint32_t *p_r13)
{
	uint8_t r2 = 0;

	*p_r13 = 0xC8000Eu;
	r12 = 0xC8000Au;
	r2 = mem_read8(0xC8000Eu);
	interrupts_off();
	*p_r2 = (*p_r2 & 0xFFFFFF00u) | r2;
}

uint32_t sub_0004B8(uint32_t r2, uint32_t r13)
{
	r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | 0x10u);
	mem_write8(r13, r2);
	return r2;
}

void second_task(uint32_t entry_point)
{
	bool L_000637 = false, L_000719 = false, L_00077F = false;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r10 = 0;
	uint32_t target1 = 0;

	for (;;)
	{
		for (;;)
		{
			target1 = L_000719 ? 1u : L_000637 ? 1u : entry_point;
			switch (target1)
			{
			case 0x000000u:
				break;
			case 0x000001u:
				if (!L_000719)
				{
					L_000637 = false;
					sub_00D8A5();
					mem_write16(IRQ_D00008, 0x39u);
					mem_write16(IRQ_D0000A, 1u);
					unk_20E0FD = 0u;
					unk_20E0FC = 0u;
					unk_20E0FF = 0u;
					unk_20E0FE = 0u;
					unk_20E00A = 0u;
					unk_20E028 = 0xFFu;
					r0 = rom_000D14[unk_20E040];
					r2 = unk_218002;
					unk_20809E = mem_read8(r0 + unk_208002);
					unk_21809E = mem_read8(r0 + r2);
					mem_write8(IO_C0001C, 6u);
					mem_write8(0xC8001Cu, 0x86u);
				}
				do
				{
					L_000719 = false;
					if (unk_20E02B == 0u)
						sub_00E8DF();
					r2 = sub_0008F0(r2);
					L_00077F = unk_20E700 != 0u;
					if (L_00077F)
						break;
					if (unk_20E02B == 0u)
						sub_00E620();
					sub_00D9F3();
					sub_00DB6D();
					sub_00DEF8();
					screen = SCREEN_2;
					r2 = sub_000C5A();
					r0 = (uint8_t)SCREEN(screen)->unk_80;
					player_car = screen + 0x1000u;
					r0 = sub_000848(r0, r2);
					switch (r0)
					{
					case 0:
						sub_001440();
						break;
					case 1:
						sub_00144C();
						break;
					case 2:
						sub_001000();
						break;
					case 3:
						sub_001458();
						break;
					case 4:
						sub_001516();
						break;
					case 5:
						sub_001DCC();
						break;
					case 6:
						sub_002E74();
						break;
					case 7:
						sub_00DDA8();
						break;
					case 8:
						sub_000D9F();
						break;
					default:
						invalid_jump(rom_0008CC[r0]);
					}
					sub_011CC0();
					sub_0189D9();
				} while (unk_20E024 == 0u);
				if (L_00077F)
					break;
				break;
			default:
				invalid_jump(target1);
			}
			if (L_00077F)
				break;
			do
			{
				unk_20E02B = 0xFFu;
				sub_00E8DF();
				r2 = sub_0008F0(r2);
				unk_20E02B = 0u;
				L_00077F = unk_20E700 != 0u;
				if (L_00077F)
					break;
				sub_00E620();
				sub_00D9F3();
				sub_00DB6D();
				sub_00DEF8();
				unk_20E026++;
				screen = SCREEN_1;
				r2 = sub_000C5A();
				r0 = (uint8_t)SCREEN(screen)->unk_80;
				player_car = screen + 0x1000u;
				r0 = sub_000848(r0, r2);
				switch (r0)
				{
				case 0:
					sub_001440();
					break;
				case 1:
					sub_00144C();
					break;
				case 2:
					sub_001000();
					break;
				case 3:
					sub_001458();
					break;
				case 4:
					sub_001516();
					break;
				case 5:
					sub_001DCC();
					break;
				case 6:
					sub_002E74();
					break;
				case 7:
					sub_00DDA8();
					break;
				case 8:
					sub_000D9F();
					break;
				default:
					invalid_jump(rom_0008CC[r0]);
				}
				sub_011CC0();
				sub_0189D9();
			} while (unk_20E024 == 0u);
			if (L_00077F)
				break;
			L_000719 = true;
		}
		L_00077F = false;
		unk_20E028 = 0u;
		sub_0008F0(r2);
		sub_01C241();
		do
		{
			sub_0008F0(r2);
			sub_01C388();
		} while (unk_20E700 != 0u);
		interrupts_off();
		mem_write8(IO_C0001C, 4u);
		mem_write8(0xC8001Cu, 0x80u);
		r0 = 0u;
		r1 = 0u;
		r2 = 0x100u;
		r10 = unk_20E000_ADDRESS;
		do
		{
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			mem_write32(r10, r0);
			mem_write32(r10 + 4u, r1);
			r10 += 8u;
			r2--;
		} while (r2 != 0u);
		move_halfwords(WORK_RAM + 0x1FF84u, 0xC98u, 0x3Eu);
		player_car = 0x219000u;
		unk_20E000 = 0x21FF84u;
		unk_20E004 = 0x21F800u;
		sub_000913();
		sub_018827();
		unk_208080 = 1u;
		unk_208153 = 1u;
		unk_208094 |= 1u;
		unk_218080 = 1u;
		unk_218153 = 1u;
		unk_218094 |= 1u;
		interrupts_on();
		sub_00D0C8();
		L_000637 = true;
	}
}

uint32_t sub_000848(uint32_t r0, uint32_t r2)
{
	uint8_t r3 = 0;
	uint32_t r0_2 = 0, r1 = 0;
	uint32_t t1 = 0;

	if (r0 == 0u || (uint8_t)r0 == 6u)
		return r0;
	r1 = SCREEN(screen)->unk_00;
	if (unk_20E615 != 0u)
	{
		r3 = (uint8_t)(SCREEN(screen)->unk_E1 + 1u);
		SCREEN(screen)->unk_E1 = r3;
		RAM8(unk_20E092_ADDRESS + r1) = (RAM8(unk_20E092_ADDRESS + r1) & ~4u) | ((r3 & 8u) >> 1);
		if ((SCREEN(screen)->unk_118 & 0x40u) == 0u)
			return r0;
		if (unk_20E615 == 0u)
		{
			mem_write8(r2, mem_read8(r2) - 1u);
			mem_write8(r2 + 2u, 0u);
		}
		SCREEN(screen)->unk_E1 = 0u;
		t1 = unk_20E092_ADDRESS + r1;
		mem_write8(t1, mem_read8(t1) & ~4u);
		r0_2 = 6u;
		SCREEN(screen)->unk_80 = 6u;
		SCREEN(screen)->unk_40 = 1u;
		sub_00DEC2();
		mem_write8(SCREEN(screen)->unk_24 + 0x19u, 0xFFu);
		return r0_2;
	}
	r2 = rom_0008C4[r1];
	if (mem_read8(r2) == 0u)
		return r0;
	r3 = (uint8_t)(SCREEN(screen)->unk_E1 + 1u);
	SCREEN(screen)->unk_E1 = r3;
	RAM8(unk_20E092_ADDRESS + r1) = (RAM8(unk_20E092_ADDRESS + r1) & ~4u) | ((r3 & 8u) >> 1);
	if ((SCREEN(screen)->unk_118 & 0x40u) == 0u)
		return r0;
	if (unk_20E615 == 0u)
	{
		mem_write8(r2, mem_read8(r2) - 1u);
		mem_write8(r2 + 2u, 0u);
	}
	SCREEN(screen)->unk_E1 = 0u;
	t1 = unk_20E092_ADDRESS + r1;
	mem_write8(t1, mem_read8(t1) & ~4u);
	r0_2 = 6u;
	SCREEN(screen)->unk_80 = 6u;
	SCREEN(screen)->unk_40 = 1u;
	sub_00DEC2();
	mem_write8(SCREEN(screen)->unk_24 + 0x19u, 0xFFu);
	return r0_2;
}

uint32_t sub_0008F0(uint32_t r2)
{
	for (;;)
	{
		if ((uint8_t)SCREEN(screen)->unk_B00 == 0u)
		{
			do
			{
				frame_wait(unk_20E023_ADDRESS);
				take_interrupts();
			} while (unk_20E023 == 0u);
			frame_end();
			unk_20E023 = 0u;
			break;
		}
		r2 = sub_010E42(r2);
		if (unk_20E023 != 0u)
		{
			unk_20E023 = 0u;
			break;
		}
	}
	return r2;
}

void sub_000913(void)
{
	uint32_t r0 = 0;

	mem_write8(IO_C00074, 0x13u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0u);
	mem_write8(IO_C00074, 0x1Fu);
	screen = SCREEN_2;
	sub_010688();
	sub_010634();
	screen = SCREEN_1;
	sub_010688();
	r0 = sub_010634();
	sub_000995(r0);
	screen = SCREEN_1;
	sub_000976();
	screen = SCREEN_2;
	sub_000976();
}

void sub_000976(void)
{
	sub_012004();
	sub_011C60();
	sub_010890();
	sub_011594();
	sub_001D84();
}

void sub_000995(uint32_t r0)
{
	uint32_t r10 = 0;

	sub_0108EA();
	sub_00013C(r0);
	sub_0107BC();
	sub_010936();
	sub_0106AF();
	sub_0113C0();
	sub_011018();
	sub_011E88();
	r10 = 0x109420u;
	sub_010FA1(r10);
	r10 = 0x109428u;
	sub_010FA1(r10);
	r10 = 0x109578u;
	sub_010FA1(r10);
	sub_0009F9();
	sub_0009F9();
	sub_000C5A();
	sub_000C5A();
}

uint32_t sub_0009F9(void)
{
	uint8_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint16_t r0_2 = 0, r7 = 0, r8 = 0;
	uint32_t r1_2 = 0, r2_2 = 0, r3_2 = 0, r4 = 0, r5 = 0, r6 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	uint16_t t5 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(IO_C0000A);
	r1 = (uint8_t)~t2;
	r0 = (uint8_t)(~t1 | ((((uint32_t)r1 << 4) | (r1 >> 4)) & 3u));
	t3 = mem_read8(0xC80008u);
	t4 = mem_read8(0xC8000Au);
	r3 = (uint8_t)~t4;
	r2 = (uint8_t)(~t3 | ((((uint32_t)r3 << 4) | (r3 >> 4)) & 3u));
	r1 = (r0 | r2) & 2u;
	r0 = (uint8_t)((r0 & 0xDu) | ((((r2 & 0xCu) * 2u) | (r2 & 1u)) * 2u) | ((uint32_t)r1 << 6) | (r1 >> 2));
	r1 = unk_20E100;
	unk_20E100 = r0;
	unk_20E101 = r1;
	unk_20E102 = ~r1 & r0;
	unk_20E103 = r1 & ~r0;
	r5 = SCREEN(screen)->unk_00;
	r5 = r5 & 1u;
	r1_2 = IO_PORTS + 0x50u + r5 * 4u;
	r2_2 = 0x20E110u + r5 * 4u;
	r3_2 = unk_20E116_ADDRESS + r5 * 4u;
	r4 = rom_000B14[r5];
	r6 = rom_000B0C[r5];
	do
	{
		mem_write8(r1_2, 0xFFu);
		r0_2 = (((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)((((uint16_t)(mem_read8(r1_2) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) * 2u) & ~0xFFu) | mem_read8(r1_2)) >> 7;
		mem_write8(r2_2, (uint8_t)r0_2);
		r7 = mem_read8(r6);
		r6++;
		r8 = mem_read8(r6);
		r2_2++;
		r6++;
		if (r8 < r0_2)
			r0_2 = (uint16_t)r8;
		t5 = r0_2;
		r0_2 = (uint16_t)(t5 - r7);
		if ((int16_t)t5 < (int16_t)r7)
			r0_2 = 0u;
		mem_write8(r3_2, divide_u16((uint16_t)(r0_2 * 0xFFu), (uint16_t)(r8 - r7)));
		r4--;
		r3_2++;
		r1_2 += 2u;
	} while (r4 != 0u);
	mem_write8(IO_C00060, r5 ^ 1u);
	return r6;
}

void sub_000B16(void)
{
	uint8_t r0 = 0, r1 = 0, r2 = 0;
	uint32_t r2_2 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	t1 = mem_read8(IO_C00008);
	t2 = mem_read8(IO_C00000);
	r2 = (uint8_t)~t2;
	r0 = ((~t1 * 2u * 2u) & 0x40u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 0x3Fu);
	t3 = mem_read8(0xC80008u);
	t4 = mem_read8(0xC80000u);
	r2 = (uint8_t)~t4;
	r1 = ((~t3 * 2u * 2u) & 0x40u) | ((((uint32_t)r2 << 4) | (r2 >> 4)) & 0x3Fu);
	r10 = unk_20E116_ADDRESS;
	if (unk_20E623 != 0u)
	{
		r11 = 0x800020u;
		r2_2 = 0x208040u;
		sub_000BBC(r0, r2_2, &r10, &r11);
		r2_2 = 0x218040u;
		r0 = (uint8_t)r1;
		sub_000BBC(r0, r2_2, &r10, &r11);
		return;
	}
	unk_20E120 = unk_208001 + 1u;
	unk_20E121 = r0;
	unk_20E122 = ~unk_20E116;
	r10++;
	unk_20E123 = unk_20E117;
	r10++;
	unk_20E124 = unk_20E118;
	unk_20E125 = unk_218001 + 1u;
	unk_20E126 = r1;
	r10++;
	unk_20E127 = ~unk_20E119;
	r10++;
	unk_20E128 = unk_20E11A;
	unk_20E129 = unk_20E11B;
}

void sub_000BBC(uint8_t r0, uint32_t r2, uint32_t *p_r10, uint32_t *p_r11)
{
	uint32_t r10 = *p_r10;
	uint32_t r11 = *p_r11;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	if (mem_read8(r2) != 0u)
	{
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2++;
		mem_write16(r11, mem_read8(r2));
		r11 += 2u;
		r2 = r2 + 1u - 4u;
		mem_write32(r2, 0u);
		mem_write8(r2 - 1u, 0u);
		*p_r11 = r11;
		return;
	}
	mem_write16(r11, 0u);
	r11 += 2u;
	mem_write16(r11, (uint8_t)r0);
	t1 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, (uint8_t)~t1);
	t2 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, t2);
	t3 = mem_read8(r10);
	r10++;
	r11 += 2u;
	mem_write16(r11, t3);
	r11 += 2u;
	*p_r10 = r10;
	*p_r11 = r11;
}

uint32_t sub_000BF3(void)
{
	uint32_t r10 = 0, r11 = 0;

	r10 = SCREEN_1;
	r11 = unk_20E121_ADDRESS;
	r12 = 0xC00000u;
	sub_000C1C(r10, r11);
	r10 = SCREEN_2;
	r11 = unk_20E126_ADDRESS;
	r12 = 0xC80000u;
	return sub_000C1C(r10, r11);
}

uint32_t sub_000C1C(uint32_t r10, uint32_t r11)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0;

	if (unk_20E623 != 0u)
		r11 = SCREEN(r10)->unk_20 + 0x2Cu;
	r0 = (uint8_t)~mem_read8(r12 + 2u);
	r2 = mem_read32(r11);
	r0 = (uint16_t)(r0 << 8) | (uint8_t)r2;
	r1 = SCREEN(r10)->unk_108;
	mem_write32(r10 + 0x130u + r1 * 4u, r0);
	SCREEN(r10)->unk_108 = (r1 + 1u) & 7u;
	AS32(SCREEN(r10)->unk_100) = r2 >> 8;
	return r0;
}

uint32_t sub_000C5A(void)
{
	uint8_t r0_2 = 0, r1_2 = 0, r3_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0;
	uint8_t t1 = 0;

	r1 = SCREEN(screen)->unk_109;
	r0 = mem_read32(screen + 0x130u + r1 * 4u);
	SCREEN(screen)->unk_109 = (r1 + 1u) & 7u;
	r3 = screen + 0x110u;
	r1 = SCREEN(r3 - 0x110u)->unk_110;
	SCREEN(r3 - 0x110u)->unk_110 = r0;
	r3 += 4u;
	SCREEN(r3 - 0x114u)->unk_114 = r1;
	r2 = ~r1 & r0;
	r3 += 4u;
	SCREEN(r3 - 0x118u)->unk_118 = r2;
	SCREEN(r3 - 0x118u)->unk_11C = r1 & ~r0;
	if (unk_20E623 != 0u)
	{
		r0_2 = mem_read8(SCREEN(screen)->unk_20 + 0x2Bu);
		t1 = r0_2;
		r1_2 = SCREEN(screen)->unk_45;
		SCREEN(screen)->unk_45 = t1;
		r2 = (r2 & ~0xFFu) | (uint8_t)~r1_2;
		r2 = (r2 & ~0xFFu) | (r2 & r0_2);
		SCREEN(screen)->unk_46 = r2;
		r3_2 = ~r0_2 & r1_2;
		SCREEN(screen)->unk_47 = r3_2;
		r2 = (r2 & ~0xFFu) | (uint8_t)(r2 | r3_2);
		SCREEN(screen)->unk_48 = r2;
		if ((uint8_t)SCREEN(screen)->unk_80 != 8u && (r0_2 & 0xCu) != 0u)
			SCREEN(screen)->unk_80 = 8u;
	}
	return r2;
}

void sub_000D7D(void)
{
	uint8_t r0 = 0, r1 = 0;

	r0 = mem_read8(SCREEN(screen)->unk_20 + 0x29u);
	r1 = mem_read8(SCREEN(screen)->unk_20 + 0x2Au);
	unk_20E04A = r0;
	unk_20E04B = r1;
	r1 += r0;
	unk_20E041 = r1;
	unk_20E040 = r1 >> 1;
}

void sub_000D9F(void)
{
	uint32_t r0 = 0, r10 = 0, r19 = 0;
	uint8_t t1 = 0;

	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 8u))
	{
	case 0:
		SCREEN(screen)->unk_153 = 0u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 1)++;
		sub_0015D8();
		return;
	case 1:
		if ((SCREEN(screen)->unk_94 & 1u) != 0u)
		{
			sub_0015D8();
			return;
		}
		AT8(SCREEN(screen)->unk_80, 1)++;
		sub_011C60();
		sub_012004();
		sub_01070C();
		sub_010889();
		sub_01076A();
		r19 = 0x31740u;
		sub_0115C0(r19);
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		sub_018AF0();
		r10 = 0xF70u;
		sub_011E2C(r10);
		return;
	case 2:
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 1)++;
		return;
	case 3:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			AT8(SCREEN(screen)->unk_80, 1)++;
			return;
		}
		return;
	case 4:
		break;
	case 5:
		t1 = (uint8_t)SCREEN(screen)->unk_88;
		AS8(SCREEN(screen)->unk_88) = t1 - 1u;
		if (t1 == 1u)
		{
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			AT8(SCREEN(screen)->unk_80, 1)++;
		}
		return;
	case 6:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			SCREEN(screen)->unk_80 = 2u;
		return;
	default:
		invalid_jump(rom_000DAC[(uint8_t)(SCREEN(screen)->unk_80 >> 8u)]);
	}
	r0 = SCREEN(screen)->unk_45;
	if ((r0 & 1u) != 0u)
	{
		sub_000D7D();
		AT8(SCREEN(screen)->unk_80, 1)++;
		AS8(SCREEN(screen)->unk_88) = 0x96u;
		return;
	}
	if ((r0 & 0xCu) == 0u)
	{
		AT8(SCREEN(screen)->unk_80, 1)++;
		AS8(SCREEN(screen)->unk_88) = 0x96u;
		return;
	}
}

void sub_000EA4(void)
{
	uint8_t r0 = 0, r8 = 0, r9 = 0;
	uint16_t r1 = 0;
	uint32_t r0_2 = 0, r11 = 0, r19 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		break;
	case 1:
		return;
	default:
		invalid_jump(rom_000EB0[OBJECT(current_object)->unk_0C]);
	}
	r0 = SCREEN(screen)->unk_45;
	if ((r0 & 1u) == 0u)
	{
		if ((r0 & 0xCu) == 0u)
		{
			OBJECT(current_object)->unk_0C++;
			r19 = rom_000F80;
			r19 = sub_011874(r19);
			return;
		}
		r0_2 = OBJECT(current_object)->unk_10;
		r0_2 = (uint8_t)(r0_2 + 1u);
		OBJECT(current_object)->unk_10 = r0_2;
		r0_2 = r0_2 >> 3;
		r19 = rom_000F78[r0_2 & 1u];
		r19 = sub_011874(r19);
		return;
	}
	OBJECT(current_object)->unk_0C++;
	r19 = rom_000F84;
	sub_011874(r19);
	r19 = rom_000F88;
	sub_011874(r19);
	r19 = rom_000F8C;
	sub_011874(r19);
	r19 = rom_000F90;
	sub_011874(r19);
	r1 = 0x6000u;
	r0 = mem_read8(SCREEN(screen)->unk_20 + 0x29u);
	r9 = (uint8_t)r0;
	r11 = TEXT_CELL(24, 12);
	sub_01190C(r0, r1, r11);
	r0 = mem_read8(SCREEN(screen)->unk_20 + 0x2Au);
	r8 = (uint8_t)r0;
	r11 = TEXT_CELL(24, 13);
	sub_01190C(r0, r1, r11);
	r0 = (uint8_t)((uint32_t)r9 + r8);
	r11 = TEXT_CELL(24, 14);
	sub_01190C(r0, r1, r11);
}

void sub_001000(void)
{
	uint32_t r19 = 0, r21 = 0;
	uint16_t t1 = 0;

	sub_0015D8();
	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 8u))
	{
	case 0:
		sub_011C60();
		sub_01070C();
		sub_010889();
		sub_01076A();
		r19 = rom_031E88;
		sub_0115C0(r19);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_019CAC_ADDRESS);
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		SCREEN(screen)->unk_108 = 0u;
		SCREEN(screen)->unk_109 = 0u;
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		AT8(SCREEN(screen)->unk_80, 1)++;
		return;
	case 1:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			SCREEN(screen)->unk_88 = 0x1C2u;
			AT8(SCREEN(screen)->unk_80, 1)++;
		}
		return;
	case 2:
		t1 = SCREEN(screen)->unk_88;
		SCREEN(screen)->unk_88 = t1 - 1u;
		if (t1 == 1u)
		{
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			AT8(SCREEN(screen)->unk_80, 1)++;
		}
		return;
	case 3:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			SCREEN(screen)->unk_80 = 3u;
		return;
	default:
		invalid_jump(rom_001014[(uint8_t)(SCREEN(screen)->unk_80 >> 8u)]);
	}
}

void sub_0010CD(void)
{
	uint32_t r19 = 0, r21 = 0;
	uint32_t t1 = 0;

	r19 = 0x11B1u;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0010EB_ADDRESS);
		t1 = ROM32(r19 + 4u);
		mem_write32(r21 + 0x80u, ROM32(r19));
		mem_write32(r21 + 0x84u, t1);
		r19 += 8u;
	} while ((int32_t)ROM32(r19) >= 0);
}

void sub_0010EB(void)
{
	uint32_t r9 = 0;
	uint8_t t1 = 0, t2 = 0;

	OBJECT(current_object)->unk_3E = 0u;
	r9 = 0u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x25u;
	AS32(OBJECT(current_object)->unk_34) = AS32(OBJECT(current_object)->unk_84);
	OBJECT(current_object)->unk_3C = 3u;
	OBJECT(current_object)->unk_30 = 0xA0u;
	OBJECT(current_object)->unk_32 = 0x70u;
	t1 = mem_read8(RANDOM_D80000);
	AS16(OBJECT(current_object)->unk_10) = (int16_t)(int8_t)t1 >> 4;
	t2 = mem_read8(RANDOM_D80000);
	AS16(OBJECT(current_object)->unk_12) = (int16_t)(int8_t)t2 >> 4;
	OBJECT(current_object)->unk_04 = sub_001141_ADDRESS;
}

void sub_001141(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0;
	uint16_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	r2 = 0xFFFFFFFFu;
	r0 = AS16(OBJECT(current_object)->unk_10);
	r1 = (uint16_t)r0;
	r0 += OBJECT(current_object)->unk_30;
	t1 = (uint16_t)OBJECT(current_object)->unk_80;
	t2 = t1 - r0;
	overflow = (uint32_t)(int16_t)t1 - (int16_t)r0 != (uint32_t)(int16_t)(t1 - r0);
	zero = t2 == 0u;
	negative = t2 >> 15;
	if (t1 != r0)
	{
		r0 = 1u;
		if ((negative ^ overflow) | zero)
			r0 = 0xFFFFu;
		r0 += r1;
		AS16(OBJECT(current_object)->unk_10) = r0;
		r0 += OBJECT(current_object)->unk_30;
		r2 = 0u;
	}
	OBJECT(current_object)->unk_30 = r0;
	r0_2 = AS16(OBJECT(current_object)->unk_12);
	r1 = (uint16_t)r0_2;
	r0_2 = (uint16_t)(r0_2 + OBJECT(current_object)->unk_32);
	t3 = (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
	t4 = t3 - r0_2;
	overflow = (uint32_t)(int16_t)t3 - (int16_t)r0_2 != (uint32_t)(int16_t)(t3 - r0_2);
	zero = t4 == 0u;
	negative = t4 >> 15;
	if (t3 == r0_2)
	{
		OBJECT(current_object)->unk_32 = r0_2;
		if (r2 == 0u)
		{
			r0_2 = sub_00EAE4(r0_2);
			return;
		}
		OBJECT(current_object)->unk_04 = sub_0011AB_ADDRESS;
		OBJECT(current_object)->unk_40 = 0xDu;
		r0_2 = sub_00EAE4(r0_2);
		return;
	}
	r0_2 = 1u;
	if ((negative ^ overflow) | zero)
		r0_2 = 0xFFFFu;
	r0_2 = (uint16_t)(r0_2 + r1);
	AS16(OBJECT(current_object)->unk_12) = r0_2;
	r0_2 = (uint16_t)(r0_2 + OBJECT(current_object)->unk_32);
	r2 = 0u;
	OBJECT(current_object)->unk_32 = r0_2;
	if (r2 == 0u)
	{
		r0_2 = sub_00EAE4(r0_2);
		return;
	}
	OBJECT(current_object)->unk_04 = sub_0011AB_ADDRESS;
	OBJECT(current_object)->unk_40 = 0xDu;
	r0_2 = sub_00EAE4(r0_2);
}

void sub_0011AB(void)
{
	uint32_t ignored = 0;

	ignored = sub_00EAE4(0u);
}

void sub_001440(void)
{
	SCREEN(screen)->unk_80 = 2u;
	sub_001000();
}

void sub_00144C(void)
{
	SCREEN(screen)->unk_80 = 2u;
	sub_001000();
}

void sub_001458(void)
{
	uint32_t r10 = 0, r19 = 0;
	uint16_t t1 = 0;

	sub_0015D8();
	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 8u))
	{
	case 0:
		break;
	case 1:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			SCREEN(screen)->unk_88 = 0xB4u;
			AT8(SCREEN(screen)->unk_80, 1)++;
		}
		return;
	case 2:
		t1 = SCREEN(screen)->unk_88;
		SCREEN(screen)->unk_88 = t1 - 1u;
		if (t1 == 1u)
		{
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			AT8(SCREEN(screen)->unk_80, 1)++;
		}
		return;
	case 3:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			SCREEN(screen)->unk_40 = 3u;
			SCREEN(screen)->unk_80 = 4u;
		}
		return;
	default:
		invalid_jump(rom_00146C[(uint8_t)(SCREEN(screen)->unk_80 >> 8u)]);
	}
	sub_011C60();
	sub_01070C();
	sub_010889();
	sub_01076A();
	r19 = rom_031DDC[rom_001A98[SCREEN(screen)->unk_9C]];
	sub_0115C0(r19);
	r19 = 0x31740u;
	sub_0115C0(r19);
	r10 = 0x1A78u;
	sub_011E2C(r10);
	SCREEN(screen)->unk_153 = 1u;
	SCREEN(screen)->unk_94 |= 1u;
	AT8(SCREEN(screen)->unk_80, 1)++;
}

void sub_001516(void)
{
	uint32_t left_caller = 0;

	sub_0015D8();
	left_caller = 0u;
	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 8u))
	{
	case 0:
		left_caller = sub_001544();
		break;
	case 1:
		sub_001577();
		break;
	case 2:
		sub_001594();
		break;
	case 3:
		sub_0015A9();
		break;
	case 4:
		left_caller = sub_0015C0(left_caller);
		break;
	default:
		invalid_jump(rom_001530[(uint8_t)(SCREEN(screen)->unk_80 >> 8u)]);
	}
	if (left_caller != 0u)
		return;
	sub_002E74();
}

uint32_t sub_001544(void)
{
	uint32_t left_caller = 0;

	SCREEN(screen)->unk_90 |= 0x100u;
	sub_011C60();
	sub_01070C();
	sub_010889();
	sub_01076A();
	AT8(SCREEN(screen)->unk_80, 1)++;
	AS8(SCREEN(screen)->unk_88) = 0u;
	AT8(SCREEN(screen)->unk_80, 2) = 0xAu;
	left_caller = 1u;
	return left_caller;
}

void sub_001577(void)
{
	uint8_t t1 = 0;

	AS8(SCREEN(screen)->unk_88)++;
	t1 = (uint8_t)SCREEN(screen)->unk_88 & 1u;
	AS8(SCREEN(screen)->unk_88) = t1;
	if (t1 == 0u)
	{
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 1)++;
	}
}

void sub_001594(void)
{
	if ((SCREEN(screen)->unk_94 & 1u) == 0u)
	{
		SCREEN(screen)->unk_88 = 0x258u;
		AT8(SCREEN(screen)->unk_80, 1)++;
	}
}

void sub_0015A9(void)
{
	uint16_t t1 = 0;

	t1 = SCREEN(screen)->unk_88;
	SCREEN(screen)->unk_88 = t1 - 1u;
	if (t1 == 1u)
	{
		SCREEN(screen)->unk_153 = 0u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 1)++;
	}
}

uint32_t sub_0015C0(uint32_t left_caller)
{
	uint32_t left_caller_2 = 0;

	if ((SCREEN(screen)->unk_94 & 1u) != 0u)
		return left_caller;
	SCREEN(screen)->unk_80 = 5u;
	sub_00DEC2();
	left_caller_2 = 1u;
	return left_caller_2;
}

void sub_0015D8(void)
{
	uint32_t r0 = 0, r1 = 0;

	r0 = unk_20E02D;
	r0 = r0 >> 3;
	r0 = (uint8_t)(r0 * 2u);
	r1 = SCREEN(screen)->unk_00;
	r0 = 0x1607u + (uint8_t)(r0 + r1) * 2u;
	r1 = (uint8_t)(r1 * 2u);
	r1 = (uint8_t)(r1 * 2u);
	unk_20E0C0[r1] = ROM8(r0);
	unk_20E0C8[r1] = rom_000001[r0];
}

void sub_001687(void)
{
	uint32_t r0 = 0, r9 = 0, r19 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_001694[t1];
	switch (t1)
	{
	case 0:
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_001980_ADDRESS);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_001894_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_14 = r21;
		OBJECT(current_object)->unk_0C++;
		r19 = 0x1AA0u + SCREEN(screen)->unk_00 * 5u * 4u;
		sub_0179AC(r19);
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x60u;
		OBJECT(current_object)->unk_3C = 0u;
		r0 = SCREEN(screen)->unk_9C;
		r9 = rom_001A88[r0];
		OBJECT(current_object)->unk_3A = 0x275u;
		sub_00F366(r9);
		sub_00F30D(0u);
		sub_00EAE4(r0);
		break;
	case 1:
		sub_00F30D(0u);
		sub_00EAE4(r0);
		r0 = OBJECT(current_object)->unk_12;
		r19 = rom_001AC8[r0];
		if (r19 == 0u)
		{
			r19 = rom_001AC8[0];
			r0 = 0u;
		}
		OBJECT(current_object)->unk_12 = r0 + 1u;
		sub_0115C5(r19);
		return;
	case 2:
		break;
	default:
		invalid_jump(rom_001694[t1]);
	}
	r0 = OBJECT(current_object)->unk_12;
	r19 = rom_001AC8[r0];
	if (r19 == 0u)
	{
		r19 = rom_001AC8[0];
		r0 = 0u;
	}
	OBJECT(current_object)->unk_12 = r0 + 1u;
	sub_0115C5(r19);
}

void sub_001894(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0;

	sub_00190B(0u, &r0, &zero);
	OBJECT(current_object)->unk_18 = 0x1ADCu;
	OBJECT(current_object)->unk_04 = sub_0018A4_ADDRESS;
	sub_0018A4(r0, 0u);
}

void sub_0018A4(uint32_t r0, uint32_t r2)
{
	uint32_t zero = 0;
	uint8_t r0_2 = 0;
	uint32_t r10 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0;

	sub_00190B(r2, &r0, &zero);
	if (zero)
	{
		sub_0018FE();
		return;
	}
	r10 = OBJECT(current_object)->unk_18;
	t1 = mem_read8(r10);
	r10++;
	if (t1 != (uint8_t)r0)
		r10 = 0x1ADCu;
	OBJECT(current_object)->unk_18 = r10;
	if (mem_read8(r10) != 0u)
	{
		sub_0018FE();
		return;
	}
	r0_2 = 9u;
	sub_011EDE(r0_2);
	SCREEN(screen)->unk_88 += 0x4B0u;
	t2 = OBJECT(current_object)->unk_14 + 0x10u;
	mem_write16(t2, mem_read16(t2) + 0x4B0u);
	OBJECT(current_object)->unk_10 = 0xB0u;
	OBJECT(current_object)->unk_11 = 4u;
	OBJECT(current_object)->unk_04 = sub_0018E5_ADDRESS;
	sub_0018E5();
}

void sub_0018E5(void)
{
	uint32_t r18 = 0, r19 = 0;
	uint16_t t1 = 0;

	t1 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
	if (t1 == 1u)
	{
		OBJECT(current_object)->unk_04 = sub_0018FE_ADDRESS;
		sub_011FE6();
		return;
	}
	r18 = 0x1AE8u;
	while ((int16_t)mem_read16(r18) >= 0)
	{
		r19 = r18;
		r18 = sub_011A32(r19);
	}
	sub_0018FE();
}

void sub_0018FE(void)
{
}

void sub_00190B(uint32_t r2, uint32_t *p_r0, uint32_t *p_zero)
{
	uint32_t r0 = *p_r0;
	uint32_t zero = *p_zero;
	uint8_t r0_3 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2_2 = 0, r3 = 0;
	uint8_t t1 = 0;

	r0_2 = (r0 & ~0xFFu) | (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	r1 = 0u;
	r2_2 = (r2 & ~0xFFu) | (uint8_t)~SCREEN(screen)->unk_100;
	if ((r0_2 & 8u) != 0u)
	{
		if ((uint8_t)r2_2 < 0x40u)
			r1 |= 8u;
	}
	else if ((uint8_t)r2_2 < 0x20u)
		r1 |= 8u;
	r3 = 2u;
	r1 = sub_001969(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | SCREEN(screen)->unk_101;
	r3 = 5u;
	r1 = sub_001969(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | SCREEN(screen)->unk_102;
	r3 = 4u;
	r1 = sub_001969(r0_2, r1, (uint8_t)r2_2, r3);
	r2_2 = (r2_2 & ~0xFFu) | (uint8_t)SCREEN(screen)->unk_110;
	if ((r2_2 & 0x10u) != 0u)
		r1 |= 1u;
	if ((r2_2 & 0x20u) == 0u)
	{
		AT8(OBJECT(current_object)->unk_08, 1) = r1;
		t1 = ~r0_2 & r1;
		zero = t1 == 0u;
		r0_3 = t1;
		*p_r0 = (*p_r0 & 0xFFFFFF00u) | r0_3;
		*p_zero = zero;
		return;
	}
	r1 |= 2u;
	AT8(OBJECT(current_object)->unk_08, 1) = r1;
	t1 = ~r0_2 & r1;
	zero = t1 == 0u;
	r0_3 = t1;
	*p_r0 = (*p_r0 & 0xFFFFFF00u) | r0_3;
	*p_zero = zero;
}

uint32_t sub_001969(uint32_t r0, uint32_t r1, uint8_t r2, uint32_t r3)
{
	if ((r0 & shl32(1u, (int)(r3))) != 0u)
	{
		if ((uint8_t)r2 >= 0xC0u)
			r1 |= shl32(1u, (int)(r3));
	}
	else if ((uint8_t)r2 >= 0xE0u)
	{
		r1 |= shl32(1u, (int)(r3));
		return r1;
	}
	return r1;
}

void sub_001980(void)
{
	uint8_t r1 = 0;
	uint16_t r0 = 0;
	uint32_t r10 = 0, r21 = 0;

	r10 = rom_001CEC[SCREEN(screen)->unk_9C];
	r0 = 0x168u;
	r1 = 0xFu;
	while (mem_read8(r10) != 0u)
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0019C0_ADDRESS);
		mem_write8(r21 + 9u, mem_read8(r10));
		mem_write16(r21 + 0x30u, r0);
		r0 += 0x12u;
		mem_write8(r21 + 0x10u, r1);
		r1 += 2u;
		r10++;
	}
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_0019C0(void)
{
	uint32_t r9 = 0, ignored = 0;
	uint8_t t1 = 0, t2 = 0;
	uint16_t t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x346u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_18 = 0x1D70u;
		OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_30 += 8u;
		OBJECT(current_object)->unk_32 = 0x14u;
		OBJECT(current_object)->unk_34 = 0xA0u;
		OBJECT(current_object)->unk_36 = 0xA0u;
		OBJECT(current_object)->unk_12 = 0x78u;
		OBJECT(current_object)->unk_13 = 0u;
		OBJECT(current_object)->unk_11 = 0xEu;
		OBJECT(current_object)->unk_0C++;
		AS16(OBJECT(current_object)->unk_12)--;
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_30 -= 0x18u;
		t2 = OBJECT(current_object)->unk_11;
		OBJECT(current_object)->unk_11 = t2 - 1u;
		if (t2 == 1u)
			OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		AS16(OBJECT(current_object)->unk_12)--;
		t1 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_30 -= 0x18u;
		t2 = OBJECT(current_object)->unk_11;
		OBJECT(current_object)->unk_11 = t2 - 1u;
		if (t2 == 1u)
			OBJECT(current_object)->unk_0C++;
		t3 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t3 - 1u;
		if (t3 == 1u)
			OBJECT(current_object)->unk_0C++;
		sub_0068FE();
		ignored = sub_00EAE4(0u);
		return;
	case 2:
		OBJECT(current_object)->unk_30 -= 0x18u;
		t2 = OBJECT(current_object)->unk_11;
		OBJECT(current_object)->unk_11 = t2 - 1u;
		if (t2 == 1u)
			OBJECT(current_object)->unk_0C++;
		t3 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t3 - 1u;
		if (t3 == 1u)
			OBJECT(current_object)->unk_0C++;
		sub_0068FE();
		ignored = sub_00EAE4(0u);
		return;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->unk_30 -= 0x18u;
		if ((int16_t)OBJECT(current_object)->unk_30 >= 0)
		{
			sub_0068FE();
			ignored = sub_00EAE4(0u);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_0019CC[OBJECT(current_object)->unk_0C]);
	}
	t3 = AS16(OBJECT(current_object)->unk_12);
	AS16(OBJECT(current_object)->unk_12) = t3 - 1u;
	if (t3 == 1u)
		OBJECT(current_object)->unk_0C++;
	sub_0068FE();
	ignored = sub_00EAE4(0u);
}

void sub_001D84(void)
{
	uint32_t r0 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	r0 = 0xAu;
	r10 = SOUND_RAM + 0x1F32u;
	r11 = 0x2CC4u;
	r12 = unk_20E300_ADDRESS;
	for (;;)
	{
		if ((int8_t)mem_read8(r10) >= 0)
		{
			mem_write8(r12, mem_read8(r10));
			t1 = r12;
			r12 = r12 + 1u;
			mem_write8(t1, mem_read8(t1) & 7u);
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write8(r12, mem_read8(r10));
			r12 = r12 + 1u;
			r10++;
			mem_write16(r12, mem_read16(r10));
			r12 = r12 + 2u;
			r11 += 6u;
			r0--;
			r10 += 2u;
			if (r0 == 0u)
				return;
			continue;
		}
		mem_write32(r12, ROM32(r11));
		r12 = r12 + 4u;
		r11 += 4u;
		mem_write16(r12, ROM16(r11));
		r12 = r12 + 2u;
		r10 += 6u;
		r0--;
		r11 += 2u;
		if (r0 != 0u)
			continue;
		break;
	}
}

void sub_001DCC(void)
{
	uint32_t r10 = 0, r19 = 0, r21 = 0;
	uint16_t t1 = 0;

	sub_0015D8();
	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 8u))
	{
	case 0:
		break;
	case 1:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			AT8(SCREEN(screen)->unk_80, 1)++;
		return;
	case 2:
		t1 = SCREEN(screen)->unk_88;
		SCREEN(screen)->unk_88 = t1 - 1u;
		if (t1 == 1u)
			AT8(SCREEN(screen)->unk_80, 1)++;
		return;
	case 3:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			SCREEN(screen)->unk_80 = 1u;
		return;
	default:
		invalid_jump(rom_001DE0[(uint8_t)(SCREEN(screen)->unk_80 >> 8u)]);
	}
	SCREEN(screen)->unk_90 &= ~0x100u;
	unk_20E0D0[SCREEN(screen)->unk_00] = 0u;
	sub_012004();
	sub_011C60();
	sub_01070C();
	sub_010889();
	sub_01076A();
	sub_007677();
	r19 = rom_031E80;
	sub_0115C0(r19);
	AT8(SCREEN(screen)->unk_80, 1)++;
	r10 = 0x2CB8u;
	sub_011E2C(r10);
	r21 = sub_011D43();
	mem_write32(r21 + 4u, sub_00247A_ADDRESS);
	SCREEN(screen)->unk_88 = 0x10Eu;
	mem_write8(SCREEN(screen)->unk_24 + 0x28u, 0u);
}

uint32_t sub_001E89(void)
{
	uint32_t zero = 0;
	uint32_t r0 = 0, r1 = 0;

	if (mem_read8(SCREEN(screen)->unk_20 + 0x5Eu) != 0u)
	{
		r0 = SCREEN(screen)->unk_A3;
		r0 = (uint8_t)(r0 - 0x15u);
		SCREEN(screen)->unk_9D = r0;
		r1 = SCREEN(screen)->unk_01;
		if (mem_read8(screen + 0x320u + r1) == 0u && mem_read16(screen + 0x410u + r1 * 2u) < unk_20E304[r0 * 3u])
		{
			zero = 1u == 0u;
			return zero;
		}
	}
	zero = 0u == 0u;
	return zero;
}

void sub_001EC2(void)
{
	bool L_00200F = false, L_00209D = false, L_0020B6 = false, L_0020FB = false, L_00212C = false, L_002130 = false, L_002133 = false;
	uint8_t r0 = 0, r1 = 0;
	uint16_t r0_3 = 0, r1_2 = 0;
	uint32_t r0_2 = 0, r1_3 = 0, r10 = 0, r19 = 0, r21 = 0, ignored = 0, object = 0;
	uint32_t stack_4 = 0, stack_8 = 0;
	uint8_t t1 = 0, t3 = 0, t4 = 0;
	uint16_t t2 = 0, t5 = 0, t7 = 0;
	uint32_t target1 = 0, target2 = 0, t6 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		SCREEN(screen)->unk_F20 = 0xFFFFu;
		unk_20E3F0 = 0u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0022F7_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r19 = 0x2E4Cu + SCREEN(screen)->unk_00 * 5u * 4u;
		sub_0179AC(r19);
		r19 = 0x13F6C0u;
		sub_0184D0(r19);
		sub_00C68E();
		r10 = 0x2D58u;
		sub_011E2C(r10);
		r0 = 0x20u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 0x14u, current_object);
			mem_write32(r21 + 4u, sub_002204_ADDRESS);
			t1 = r0;
			r0 = (uint8_t)(t1 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t1 != 1u);
		AT8(OBJECT(current_object)->unk_08, 1) = 0xFFu;
		OBJECT(current_object)->unk_10 = 0x58u;
		OBJECT(current_object)->unk_11 = 2u;
		OBJECT(current_object)->unk_12 = 0x28u;
		OBJECT(current_object)->unk_13 = 0u;
		OBJECT(current_object)->unk_2C = 0xA80u;
		AT8(OBJECT(current_object)->unk_08, 2) = 0xFFu;
		r0_2 = mem_read16(screen + 0x410u + SCREEN(screen)->unk_01 * 2u);
		object = current_object + 0x9Au;
		AT16(OBJECT(object - 0x9Au)->unk_98, 2) = 0xF8u;
		object += 2u;
		AS16(OBJECT(object - 0x9Cu)->unk_9C) = 0x54u;
		AT16(OBJECT(object - 0x9Cu)->unk_9C, 2) = 0xF0u;
		object = current_object + 0xA4u;
		AS16(OBJECT(object - 0xA4u)->unk_A4) = 0xA8u;
		object += 2u;
		AT16(OBJECT(object - 0xA6u)->unk_A4, 2) = 0x54u;
		object += 2u;
		AS16(OBJECT(object - 0xA8u)->unk_A8) = 0xF0u;
		object += 2u;
		sub_00CEA7(r0_2, object);
		OBJECT(current_object)->unk_0C++;
	case 1:
		OBJECT(current_object)->unk_2C += 0x20u;
		t2 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
		if (t2 != 1u)
		{
			r0_2 = AS16(OBJECT(current_object)->unk_10);
			r0_2 = r0_2 / 0x1Eu;
			r0_2 = sub_011975(r0_2);
			sub_00C70C(r0_2);
			r19 = current_object + 0xA4u;
			sub_011A32(r19);
			r19 = current_object + 0x9Au;
			ignored = sub_011A32(r19);
			return;
		}
		OBJECT(current_object)->unk_0C++;
	case 2:
		r0 = (uint8_t)OBJECT(current_object)->unk_98;
		L_00200F = r0 == 0u;
		if (!L_00200F)
		{
			if ((int8_t)r0 < 0)
			{
				if (SCREEN(screen)->unk_100 >= 0x60u)
				{
					AS8(OBJECT(current_object)->unk_98) = r0 + 1u;
					r0_3 = (uint16_t)(OBJECT(current_object)->unk_2C - 0x20u);
				}
				else
				{
					r0 += 4u;
					AS8(OBJECT(current_object)->unk_98) = r0 - 1u;
					r0_3 = (uint16_t)(OBJECT(current_object)->unk_2C + 0x20u);
				}
			}
			else if (SCREEN(screen)->unk_100 < 0xA0u)
			{
				AS8(OBJECT(current_object)->unk_98) = r0 - 1u;
				r0_3 = (uint16_t)(OBJECT(current_object)->unk_2C + 0x20u);
			}
			else
			{
				r0 -= 4u;
				AS8(OBJECT(current_object)->unk_98) = r0 + 1u;
				r0_3 = (uint16_t)(OBJECT(current_object)->unk_2C - 0x20u);
			}
			OBJECT(current_object)->unk_2C = r0_3;
			L_00200F = (uint8_t)OBJECT(current_object)->unk_98 == 0u;
		}
		if (L_00200F)
		{
			r0_2 = SCREEN(screen)->unk_100;
			r0_2 = r0_2 >> 4;
			r1 = rom_002D6C[r0_2];
			if (r1 != 0u)
			{
				t3 = (uint8_t)(OBJECT(current_object)->unk_98 >> 8u);
				AT8(OBJECT(current_object)->unk_98, 1) = t3 - r1;
				if ((int8_t)t3 <= (int8_t)r1)
				{
					AT8(OBJECT(current_object)->unk_98, 1) = 0xFu;
					r1 = 4u;
					if (r0_2 >> 2 != 0u)
						r1 = (uint8_t)(0u - r1);
					AS8(OBJECT(current_object)->unk_98) = r1;
				}
			}
			else
			{
				r1_2 = (uint16_t)SCREEN(screen)->unk_118;
				if ((r1_2 & 0x200u) != 0u)
				{
					if ((r1_2 & 0x400u) == 0u)
					{
						r0_2 = 0xFFu;
						r1 = 0xFu;
						t3 = (uint8_t)(OBJECT(current_object)->unk_98 >> 8u);
						AT8(OBJECT(current_object)->unk_98, 1) = t3 - r1;
						if ((int8_t)t3 <= (int8_t)r1)
						{
							AT8(OBJECT(current_object)->unk_98, 1) = 0xFu;
							r1 = 4u;
							if (r0_2 >> 2 != 0u)
								r1 = (uint8_t)(0u - r1);
							AS8(OBJECT(current_object)->unk_98) = r1;
						}
					}
				}
				else if ((r1_2 & 0x400u) != 0u)
				{
					r0_2 = 0u;
					r1 = 0xFu;
					t3 = (uint8_t)(OBJECT(current_object)->unk_98 >> 8u);
					AT8(OBJECT(current_object)->unk_98, 1) = t3 - r1;
					if ((int8_t)t3 <= (int8_t)r1)
					{
						AT8(OBJECT(current_object)->unk_98, 1) = 0xFu;
						r1 = 4u;
						if (r0_2 >> 2 != 0u)
							r1 = (uint8_t)(0u - r1);
						AS8(OBJECT(current_object)->unk_98) = r1;
					}
				}
				else
					AT8(OBJECT(current_object)->unk_98, 1) = 1u;
			}
		}
		AT8(OBJECT(current_object)->unk_08, 2) = ((OBJECT(current_object)->unk_2C + 0x40u) & 0xFFFu) >> 7;
		do
		{
			target1 = L_00209D ? 0x2090u : rom_002088[OBJECT(current_object)->unk_0D];
			switch (target1)
			{
			case 0x002090u:
				if (!L_00209D && SCREEN(screen)->unk_101 >= 0x40u)
					L_002133 = true;
				else
				{
					if (!L_00209D)
						OBJECT(current_object)->unk_0D++;
					L_00209D = false;
					if (SCREEN(screen)->unk_101 < 0xC0u)
					{
						r0_3 = (uint16_t)SCREEN(screen)->unk_118;
						L_0020B6 = (r0_3 & 0x100u) != 0u || (r0_3 & 0x40u) != 0u;
						if (!L_0020B6)
							L_002133 = true;
					}
					if (L_0020B6 || !L_002133)
					{
						L_0020B6 = false;
						r0_2 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
						r1_3 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
						do
						{
							do
							{
								target2 = L_00212C ? 0x20D4u : L_0020FB ? 0x20D4u : rom_0020C8[r1_3];
								switch (target2)
								{
								case 0x0020D4u:
									do
									{
										if (L_002130 || !L_00212C)
										{
											if (L_002130 || !L_0020FB)
											{
												if (L_002130 || r0_2 == 0x1Eu)
												{
													L_002130 = false;
													OBJECT(current_object)->unk_0D--;
													L_002133 = true;
												}
											}
										}
										if (L_002133)
											break;
										if (L_00212C || L_0020FB || r0_2 != 0x1Fu)
										{
											if (!L_00212C)
											{
												L_0020FB = false;
												stack_4 = r0_2;
												stack_8 = r1_3;
												r0 = rom_002D98[r1_3];
												sub_011EB8(r0);
												r1_3 = stack_8;
												r0_2 = stack_4;
												t4 = r0_2;
												r0 = mem_read8(rom_002D00_ADDRESS + r0_2);
												if (t4 == 0x1Du)
													r0 = 0x20u;
												mem_write8(current_object + 0xA0u + r1_3, r0);
												r1_3 = (r1_3 & ~0xFFu) | (uint8_t)(r1_3 + 1u);
											}
											if (L_00212C || (uint8_t)r1_3 != 3u)
											{
												L_00212C = false;
												AT8(OBJECT(current_object)->unk_08, 3) = r1_3;
												L_002130 = true;
											}
										}
									} while (L_002130);
									if (L_002133)
										break;
									break;
								case 0x0020E2u:
									if (r0_2 == 0x1Eu)
									{
										mem_write8(current_object + 0xA0u + r1_3, 0u);
										r1_3 = (uint8_t)(r1_3 - 1u);
										L_00212C = true;
									}
									if (L_00212C)
										break;
									L_0020FB = r0_2 != 0x1Fu;
									if (L_0020FB)
										break;
									break;
								default:
									invalid_jump(target2);
								}
								if (L_002133)
									break;
							} while (L_00212C);
							if (L_002133)
								break;
						} while (L_0020FB);
					}
				}
				if (L_002133)
				{
					L_002133 = false;
					r1_3 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
					r0 = rom_002D00[(uint8_t)(OBJECT(current_object)->unk_08 >> 16u)];
					if ((unk_20E026 & 4u) == 0u)
						r0 = 0u;
					mem_write8(current_object + 0xA0u + r1_3, r0);
					t5 = AS16(OBJECT(current_object)->unk_10);
					AS16(OBJECT(current_object)->unk_10) = t5 - 1u;
					if (t5 != 1u)
					{
						r0_2 = AS16(OBJECT(current_object)->unk_10);
						r0_2 = r0_2 / 0x1Eu;
						r0_2 = sub_011975(r0_2);
						sub_00C70C(r0_2);
						r19 = current_object + 0xA4u;
						sub_011A32(r19);
						r19 = current_object + 0x9Au;
						ignored = sub_011A32(r19);
						return;
					}
				}
				break;
			case 0x00209Du:
				L_00209D = true;
				break;
			default:
				invalid_jump(target1);
			}
		} while (L_00209D);
		AT8(OBJECT(current_object)->unk_08, 1) = 0u;
		mem_write8(current_object + 0xA0u + r1_3, 0u);
		r0_2 = OBJECT(current_object)->unk_A0;
		if (r0_2 == 0u)
			OBJECT(current_object)->unk_A0 = 0x2D2D2Du;
		else
		{
			r10 = 0x2D7Cu;
			while ((int32_t)ROM32(r10) >= 0)
			{
				t6 = ROM32(r10);
				r10 += 4u;
				if (r0_2 == t6)
				{
					OBJECT(current_object)->unk_A0 = 0x2D2D2Du;
					break;
				}
			}
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_10 = 0x3Cu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_12 = 0u;
		OBJECT(current_object)->unk_13 = 0u;
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			SCREEN(screen)->unk_F20 = 0u;
			AT8(SCREEN(screen)->unk_80, 2) = 0x15u;
			r10 = SCREEN(screen)->unk_24;
			mem_write8(r10 + 0x11u, mem_read8(r10 + 0x11u) + 1u);
			mem_write16(r10 + 0x16u, mem_read16(screen + 0x410u + SCREEN(screen)->unk_01 * 2u));
			mem_write8(r10 + 0x12u, CAR(player_car)->unk_60);
			mem_write8(r10 + 0x13u, (uint8_t)OBJECT(current_object)->unk_A0);
			mem_write16(r10 + 0x14u, (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u));
			mem_write8(r10 + 0x18u, SCREEN(screen)->unk_A3 - 0x15u);
			OBJECT(current_object)->unk_0C++;
		}
		break;
	case 3:
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			SCREEN(screen)->unk_F20 = 0u;
			AT8(SCREEN(screen)->unk_80, 2) = 0x15u;
			r10 = SCREEN(screen)->unk_24;
			mem_write8(r10 + 0x11u, mem_read8(r10 + 0x11u) + 1u);
			mem_write16(r10 + 0x16u, mem_read16(screen + 0x410u + SCREEN(screen)->unk_01 * 2u));
			mem_write8(r10 + 0x12u, CAR(player_car)->unk_60);
			mem_write8(r10 + 0x13u, (uint8_t)OBJECT(current_object)->unk_A0);
			mem_write16(r10 + 0x14u, (uint16_t)(OBJECT(current_object)->unk_A0 >> 8u));
			mem_write8(r10 + 0x18u, SCREEN(screen)->unk_A3 - 0x15u);
			OBJECT(current_object)->unk_0C++;
		}
		r19 = current_object + 0xA4u;
		sub_011A32(r19);
		r19 = current_object + 0x9Au;
		ignored = sub_011A32(r19);
		return;
	case 4:
		break;
	default:
		invalid_jump(rom_001ED0[OBJECT(current_object)->unk_0C]);
	}
	r19 = current_object + 0xA4u;
	sub_011A32(r19);
	r19 = current_object + 0x9Au;
	ignored = sub_011A32(r19);
}

void sub_002204(void)
{
	uint32_t carry = 0;
	uint16_t r2 = 0;
	uint32_t r0 = 0, r1 = 0, r2_2 = 0, r3 = 0, r9 = 0, r21 = 0;
	uint16_t t1 = 0;
	uint32_t t2 = 0;

	r21 = OBJECT(current_object)->unk_14;
	if (mem_read8(r21 + 9u) == 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x2DEu;
		sub_00F366(r9);
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_40 = rom_002D00[r0];
		OBJECT(current_object)->unk_2C = r0 << 7;
		AS32(OBJECT(current_object)->unk_24) = 0xFFFFFFFBu;
		OBJECT(current_object)->unk_12 = 0x28u;
		OBJECT(current_object)->unk_13 = 0u;
		OBJECT(current_object)->unk_0C++;
		AS32(OBJECT(current_object)->unk_24)++;
		t1 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t1 - 1u;
		if (t1 == 1u)
			OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		AS32(OBJECT(current_object)->unk_24)++;
		t1 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t1 - 1u;
		if (t1 == 1u)
			OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_00221C[OBJECT(current_object)->unk_0C]);
	}
	r0 = OBJECT(current_object)->unk_2C;
	r0 = (uint16_t)(r0 - mem_read16(r21 + 0x2Cu));
	r0 = r0 & 0xFFFu;
	r1 = (int16_t)rom_100C00[r0];
	r3 = (int16_t)rom_101400[r0];
	r0 = r3;
	r1 = (int32_t)r1 >> 7;
	r3 = 0u - ((int32_t)r3 >> 7);
	AS32(OBJECT(current_object)->unk_20) = r1;
	OBJECT(current_object)->unk_28 = r3;
	r2 = 0x34u;
	if (mem_read8(r21 + 0xAu) == (uint8_t)(OBJECT(current_object)->unk_08 >> 8u))
	{
		if ((unk_20E026 & 4u) == 0u)
		{
			OBJECT(current_object)->unk_00 &= 0xDFu;
			return;
		}
		r2 = 0x30u;
	}
	t2 = r0 - 0x2000u;
	r0 = t2;
	if ((int32_t)t2 < 0)
	{
		OBJECT(current_object)->unk_00 |= 0x40u;
		r0 = 0u - r0;
		r2 = 0x27u;
	}
	else
	{
		OBJECT(current_object)->unk_00 &= 0xBFu;
		r0 = r0 * 3u;
	}
	AS32(OBJECT(current_object)->unk_1C) = (int32_t)(r0 * 3u) >> 8;
	OBJECT(current_object)->unk_3A = r2;
	r2_2 = AS32(OBJECT(current_object)->unk_24);
	r0 = divide_s32(0x100000u, r3 + 0x100u);
	r1 = r1 * r0;
	r2_2 = r2_2 * r0;
	carry = (r1 >> 11) & 1u;
	r1 = (uint16_t)(((int32_t)r1 >> 12) + carry);
	carry = (r2_2 >> 11) & 1u;
	r2_2 = (uint16_t)(((int32_t)r2_2 >> 12) + carry + 0x40u);
	OBJECT(current_object)->unk_30 = r1 + 0xE0u;
	OBJECT(current_object)->unk_32 = r2_2;
	OBJECT(current_object)->unk_3C = r2_2;
	carry = (r0 >> 3) & 1u;
	r0 = ((int32_t)r0 >> 4) + carry;
	OBJECT(current_object)->unk_36 = r0;
	r0 = (int32_t)(r0 * AS32(OBJECT(current_object)->unk_1C)) >> 8;
	OBJECT(current_object)->unk_34 = r0;
	r0 = sub_00EAE4(r0);
}

void sub_0022F7(void)
{
	uint8_t r0 = 0;
	uint32_t r9 = 0, r10 = 0, r11 = 0, r21 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		sub_018AF0();
		SCREEN(screen)->unk_F35 = 0xFFu;
		move_string(OBJECT(current_object)->unk_14 + 0xAAu, 7u, screen + 0xF13u, 7u, 1u, 0u, 0u, 0u, &r27, &r28);
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_002304[OBJECT(current_object)->unk_0C]);
	}
	r21 = OBJECT(current_object)->unk_14;
	r10 = r21 + 0xA0u;
	r11 = screen + 0xF1Cu;
	r9 = 3u;
	do
	{
		r0 = mem_read8(r10);
		r10++;
		if (r10 == 0u)
		{
			do
			{
				mem_write8(r11, 0x20u);
				r9--;
				r11++;
			} while (r9 != 0u);
			break;
		}
		if (r0 == 0x7Fu)
			r0 = 0x7Eu;
		if (r0 == 0x7Cu)
			r0 = 0x7Fu;
		if (r0 == 0x5Cu)
			r0 = 0x65u;
		mem_write8(r11, r0);
		r9--;
		r11++;
	} while (r9 != 0u);
	if (mem_read8(r21 + 9u) == 0u)
		OBJECT(current_object)->unk_00 = 0u;
}

void sub_002379(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_002384[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_002384[t1]);
	}
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_3E = 0xAu;
	r0 = CAR(player_car)->unk_60;
	r9 = rom_002D9C[r0];
	OBJECT(current_object)->unk_3A = rom_002DAC[r0];
	OBJECT(current_object)->unk_3C = 0x19Cu;
	sub_00F366(r9);
	OBJECT(current_object)->unk_30 = 0x40u;
	OBJECT(current_object)->unk_32 = 0xC0u;
	OBJECT(current_object)->unk_40 = 0u;
	sub_0023C6();
}

void sub_0023C6(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0023D4[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0023D4[t1]);
	}
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_3E = 0xAu;
	r0 = CAR(player_car)->unk_60;
	r9 = rom_002DBC[r0];
	OBJECT(current_object)->unk_3A = rom_002DCC[r0];
	OBJECT(current_object)->unk_3C = 0x19Du;
	sub_00F366(r9);
	OBJECT(current_object)->unk_30 = 0x60u;
	OBJECT(current_object)->unk_32 = 0xC8u;
	OBJECT(current_object)->unk_40 = 0u;
	sub_002416();
}

void sub_002416(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_002424[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_002424[t1]);
	}
	OBJECT(current_object)->unk_0C++;
	r0 = SCREEN(screen)->unk_A3;
	r0 = (uint8_t)(r0 - 0x15u);
	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3A = rom_002E38[r0];
	r9 = rom_002E24[r0];
	OBJECT(current_object)->unk_3C = 0x19Cu;
	sub_00F366(r9);
	OBJECT(current_object)->unk_30 = 0xE0u;
	OBJECT(current_object)->unk_36 = 0x100u;
	OBJECT(current_object)->unk_34 = 0x100u;
	OBJECT(current_object)->unk_32 = 0x40u;
	r0 = sub_00EAE4(r0);
}

void sub_00247A(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0, r21 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_10 = 0u;
		r10 = 0x2CB8u;
		sub_011E2C(r10);
		r21 = sub_011D43();
		mem_write32(r21 + 0x14u, current_object);
		mem_write32(r21 + 4u, sub_00254D_ADDRESS);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_002612_ADDRESS);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00265F_ADDRESS);
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		OBJECT(current_object)->unk_02 &= 0xDFFFu;
		if ((SCREEN(screen)->unk_94 & 1u) != 0u)
			return;
		if (OBJECT(current_object)->unk_10 >= 3u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_10++;
		OBJECT(current_object)->unk_12 = 0x3Cu;
		OBJECT(current_object)->unk_13 = 0u;
		r0 = SCREEN(screen)->unk_9D;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_0026AD_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r0);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00270C_ADDRESS);
		mem_write8(r21 + 9u, r0);
		OBJECT(current_object)->unk_0C++;
		return;
	case 2:
		t1 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t1 - 1u;
		if (t1 != 1u)
			return;
		OBJECT(current_object)->unk_02 |= 0x2000u;
		r0 = (uint8_t)(SCREEN(screen)->unk_9D + 1u);
		if (r0 >= 0xAu)
			r0 = 0u;
		SCREEN(screen)->unk_9D = r0;
		OBJECT(current_object)->unk_0C--;
		return;
	default:
		invalid_jump(rom_002488[OBJECT(current_object)->unk_0C]);
	}
}

void sub_00254D(void)
{
	uint32_t t1 = 0;

	if ((uint8_t)SCREEN(screen)->unk_80 == 5u)
	{
		switch (OBJECT(current_object)->unk_0C)
		{
		case 0:
			OBJECT(current_object)->unk_10 = mem_read8(OBJECT(current_object)->unk_14 + 0x10u);
			if (OBJECT(current_object)->unk_10 == 0u)
			{
				if (SCREEN(screen)->unk_00 != 0u)
					mem_write8(0xC8001Cu, 0x86u);
				else
					mem_write8(IO_C0001C, 6u);
				SCREEN(screen)->unk_153 = 1u;
				SCREEN(screen)->unk_94 |= 1u;
			}
			sub_002AC8();
			OBJECT(current_object)->unk_0C++;
		case 1:
			if (SCREEN(screen)->unk_B00 != 0u)
			{
				t1 = OBJECT(current_object)->unk_88;
				mem_write16(t1, mem_read16(t1) - 3u);
				return;
			}
			if (OBJECT(current_object)->unk_10 != 0u)
			{
				SCREEN(screen)->unk_153 = 3u;
				SCREEN(screen)->unk_94 |= 1u;
			}
			OBJECT(current_object)->unk_0C++;
			if ((mem_read32(OBJECT(current_object)->unk_14) & 0x20000000u) == 0u)
			{
				t1 = OBJECT(current_object)->unk_88;
				mem_write16(t1, mem_read16(t1) - 3u);
				return;
			}
			if (OBJECT(current_object)->unk_10 != 3u)
			{
				SCREEN(screen)->unk_153 = 2u;
				SCREEN(screen)->unk_94 |= 1u;
				OBJECT(current_object)->unk_0C++;
				if ((SCREEN(screen)->unk_94 & 1u) == 0u)
					OBJECT(current_object)->unk_0C = 0u;
			}
			else
			{
				SCREEN(screen)->unk_153 = 0u;
				SCREEN(screen)->unk_94 |= 1u;
				OBJECT(current_object)->unk_0C = 4u;
			}
			break;
		case 2:
			if ((mem_read32(OBJECT(current_object)->unk_14) & 0x20000000u) == 0u)
			{
				t1 = OBJECT(current_object)->unk_88;
				mem_write16(t1, mem_read16(t1) - 3u);
				return;
			}
			if (OBJECT(current_object)->unk_10 != 3u)
			{
				SCREEN(screen)->unk_153 = 2u;
				SCREEN(screen)->unk_94 |= 1u;
				OBJECT(current_object)->unk_0C++;
				if ((SCREEN(screen)->unk_94 & 1u) == 0u)
					OBJECT(current_object)->unk_0C = 0u;
			}
			else
			{
				SCREEN(screen)->unk_153 = 0u;
				SCREEN(screen)->unk_94 |= 1u;
				OBJECT(current_object)->unk_0C = 4u;
			}
			t1 = OBJECT(current_object)->unk_88;
			mem_write16(t1, mem_read16(t1) - 3u);
			return;
		case 3:
			if ((SCREEN(screen)->unk_94 & 1u) == 0u)
				OBJECT(current_object)->unk_0C = 0u;
			t1 = OBJECT(current_object)->unk_88;
			mem_write16(t1, mem_read16(t1) - 3u);
			return;
		case 4:
			break;
		default:
			invalid_jump(rom_002564[OBJECT(current_object)->unk_0C]);
		}
	}
	t1 = OBJECT(current_object)->unk_88;
	mem_write16(t1, mem_read16(t1) - 3u);
}

void sub_002612(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_002620[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3A = 0x2B2u;
		r9 = 0x2D2u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_30 = 0u;
		OBJECT(current_object)->unk_32 = 0x30u;
		break;
	case 1:
		break;
	case 2:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_002620[t1]);
	}
	r0 = sub_00EAE4(r0);
}

void sub_00265F(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00266C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3A = 0x2B3u;
		r9 = 0x2D3u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3C = 0x19Du;
		OBJECT(current_object)->unk_30 = 0x13Fu;
		OBJECT(current_object)->unk_32 = 0x30u;
		break;
	case 1:
		break;
	case 2:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00266C[t1]);
	}
	r0 = sub_00EAE4(r0);
}

void sub_0026AD(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0026B8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3A = rom_002E10[r0];
		r9 = rom_002DFC[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3C = 0x19Cu;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x50u;
		OBJECT(current_object)->unk_10 = 0x5Au;
		OBJECT(current_object)->unk_11 = 0u;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_0026B8[t1]);
	}
	t2 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
	if (t2 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_00270C(void)
{
	uint8_t r1 = 0, r4 = 0;
	uint32_t r0 = 0, r3 = 0, r11 = 0, r21 = 0, object = 0;
	uint8_t t1 = 0;

	object = current_object + 0x80u;
	r3 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	r3 = (uint8_t)(r3 * 6u);
	r11 = 0x20E301u + r3;
	r4 = mem_read8(r11);
	r11++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	AS8(OBJECT(object - 0x80u)->unk_80) = r4;
	t1 = mem_read8(r11);
	r11++;
	r4 = t1;
	object++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	AT8(OBJECT(object - 0x81u)->unk_80, 1) = r4;
	r4 = mem_read8(r11);
	object++;
	if (r4 == 0x2Du)
		r4 = 0x2Eu;
	AT8(OBJECT(object - 0x82u)->unk_80, 2) = r4;
	object++;
	AT8(OBJECT(object - 0x83u)->unk_80, 3) = 0u;
	r0 = unk_20E304[r3 >> 1];
	object++;
	sub_00CEA7(r0, object);
	r0 = 0xBu;
	r1 = 0u;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 0x14u, current_object);
		mem_write8(r21 + 9u, r1);
		mem_write8(r21 + 0xAu, (uint8_t)(OBJECT(current_object)->unk_08 >> 8u));
		mem_write32(r21 + 4u, sub_00278E_ADDRESS);
		r1++;
		r0--;
	} while (r0 != 0u);
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_00278E(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0;
	uint8_t t2 = 0;
	uint16_t t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00279C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		AT16(OBJECT(current_object)->unk_80, 2) = 0x3Au;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3A = 0x27u;
		r9 = 0x2DEu;
		sub_00F366(r9);
		r21 = OBJECT(current_object)->unk_14;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_3C = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		OBJECT(current_object)->unk_40 = mem_read8(r21 + 0x80u + r0);
		OBJECT(current_object)->unk_30 = rom_002D2A[r0];
		OBJECT(current_object)->unk_32 = rom_002D40[r0];
		OBJECT(current_object)->unk_10 = 0u;
	case 1:
		OBJECT(current_object)->unk_30 += 0x20u;
		OBJECT(current_object)->unk_10++;
		if (OBJECT(current_object)->unk_10 >= 7u)
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_002870_ADDRESS);
			mem_write8(r21 + 9u, (uint8_t)(OBJECT(current_object)->unk_08 >> 16u));
			OBJECT(current_object)->unk_0C++;
		}
		r0 = (r0 & ~0xFFu) | (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 2u);
		OBJECT(current_object)->unk_12 = r0;
		t2 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t2 - 1u;
		if (t2 == 1u)
		{
			r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			r0 = unk_20E300[(uint8_t)(r0 * 6u)];
			OBJECT(current_object)->unk_3A = rom_002DEC[r0];
			OBJECT(current_object)->unk_0C++;
		}
		break;
	case 2:
		t2 = OBJECT(current_object)->unk_12;
		OBJECT(current_object)->unk_12 = t2 - 1u;
		if (t2 == 1u)
		{
			r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			r0 = unk_20E300[(uint8_t)(r0 * 6u)];
			OBJECT(current_object)->unk_3A = rom_002DEC[r0];
			OBJECT(current_object)->unk_0C++;
		}
		t3 = (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
		AT16(OBJECT(current_object)->unk_80, 2) = t3 - 1u;
		if (t3 != 1u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_30 -= 0x10u;
		if (OBJECT(current_object)->unk_30 != 0xFFE0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->unk_30 -= 0x10u;
		if (OBJECT(current_object)->unk_30 != 0xFFE0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	default:
		invalid_jump(rom_00279C[t1]);
	}
	t3 = (uint16_t)(OBJECT(current_object)->unk_80 >> 16u);
	AT16(OBJECT(current_object)->unk_80, 2) = t3 - 1u;
	if (t3 != 1u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_0C++;
	OBJECT(current_object)->unk_30 -= 0x10u;
	if (OBJECT(current_object)->unk_30 != 0xFFE0u)
	{
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_002870(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00287C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_10 = 0xFu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r0 = unk_20E300[(uint8_t)(r0 * 6u)];
		r9 = rom_002DDC[r0];
		OBJECT(current_object)->unk_3A = rom_002DAC[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_30 = 0u;
		OBJECT(current_object)->unk_32 = 0x88u;
		OBJECT(current_object)->unk_36 = 0x50u;
		OBJECT(current_object)->unk_34 = 0x50u;
		OBJECT(current_object)->unk_40 = 9u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_30 += 0x10u;
		if (OBJECT(current_object)->unk_30 < 0x108u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		OBJECT(current_object)->unk_30 += 0x10u;
		if (OBJECT(current_object)->unk_30 < 0x108u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		OBJECT(current_object)->unk_40--;
		if (OBJECT(current_object)->unk_40 != 4u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 != 1u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_30 -= 0x10u;
		if (OBJECT(current_object)->unk_30 != 0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 3:
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 != 1u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_30 -= 0x10u;
		if (OBJECT(current_object)->unk_30 != 0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 4:
		OBJECT(current_object)->unk_30 -= 0x10u;
		if (OBJECT(current_object)->unk_30 != 0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_00 = 0u;
		return;
	case 5:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00287C[t1]);
	}
}

void sub_002933(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	r10 = 0x67B2u;
	r0 = 0u;
	while (ROM16(r10) != 0u)
	{
		r0++;
		r10 += 6u;
	}
	AT8(OBJECT(current_object)->unk_08, 1) = r0;
	OBJECT(current_object)->unk_04 = sub_002952_ADDRESS;
	sub_002952();
}

void sub_002952(void)
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
			mem_write32(r21 + 4u, sub_002998_ADDRESS);
			mem_write16(r21 + 0x10u, (uint8_t)(OBJECT(current_object)->unk_08 >> 8u));
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
		invalid_jump(rom_002960[OBJECT(current_object)->unk_0C]);
	}
}

void sub_002998(void)
{
	uint8_t r0_2 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r10 = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r0 = sub_011E45();
		r3 = r0;
		r0 &= 7u;
		r9 = rom_002DDC[r0];
		OBJECT(current_object)->unk_3A = rom_002DAC[r0];
		sub_00F366(r9);
		r0_2 = rom_0067A9[r0];
		sub_011EB8(r0_2);
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 3u;
		OBJECT(current_object)->unk_40 = 0xCu;
		OBJECT(current_object)->unk_0C++;
		if ((int32_t)r3 >= 0)
		{
			r0 = AS16(OBJECT(current_object)->unk_10);
			t1 = r0 - 1u;
			r0 = t1;
			if ((int16_t)t1 < 0)
			{
				OBJECT(current_object)->unk_00 = 0u;
				return;
			}
			AS16(OBJECT(current_object)->unk_10) = r0;
			r10 = 0x67B2u + r0 * 3u * 2u;
			r0 = ROM16(r10);
			OBJECT(current_object)->unk_34 = r0;
			OBJECT(current_object)->unk_36 = r0;
			r10 += 2u;
			OBJECT(current_object)->unk_30 = ROM16(r10);
			OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_40 = 1u;
		OBJECT(current_object)->unk_0C++;
		r0 = AS16(OBJECT(current_object)->unk_10);
		t2 = r0 - 1u;
		r0 = t2;
		if ((int16_t)t2 < 0)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_10) = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0_3 = ROM16(r10);
		OBJECT(current_object)->unk_34 = r0_3;
		OBJECT(current_object)->unk_36 = r0_3;
		r10 += 2u;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->unk_30 = r0;
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		r0 = AS16(OBJECT(current_object)->unk_10);
		t1 = r0 - 1u;
		r0 = t1;
		if ((int16_t)t1 < 0)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_10) = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0 = ROM16(r10);
		OBJECT(current_object)->unk_34 = r0;
		OBJECT(current_object)->unk_36 = r0;
		r10 += 2u;
		OBJECT(current_object)->unk_30 = ROM16(r10);
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = AS16(OBJECT(current_object)->unk_10);
		t2 = r0 - 1u;
		r0 = t2;
		if ((int16_t)t2 < 0)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		AS16(OBJECT(current_object)->unk_10) = r0;
		r10 = 0x67B2u + r0 * 3u * 2u;
		r0_3 = ROM16(r10);
		OBJECT(current_object)->unk_34 = r0_3;
		OBJECT(current_object)->unk_36 = r0_3;
		r10 += 2u;
		r0 = (uint16_t)(0x96u - ROM16(r10));
		OBJECT(current_object)->unk_30 = r0;
		OBJECT(current_object)->unk_32 = ROM16(r10 + 2u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0029A4[OBJECT(current_object)->unk_0C]);
	}
}

void sub_002AC8(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint32_t stack_4 = 0;

	r19 = 0x2C44u + SCREEN(screen)->unk_00 * 0xCu * 4u;
	OBJECT(current_object)->unk_80 = mem_read32(r19);
	r19 += 4u;
	AS32(OBJECT(current_object)->unk_84) = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_88 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_8C = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_90 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_94 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_98 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_9C = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_A0 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_A4 = mem_read32(r19);
	r19 += 4u;
	OBJECT(current_object)->unk_A8 = mem_read32(r19);
	OBJECT(current_object)->unk_AC = mem_read32(r19 + 4u);
	unk_20E027 = 0xFFu;
	r0 = rom_002D20[SCREEN(screen)->unk_9D];
	mem_write8(OBJECT(current_object)->unk_80, rom_002C18[r0]);
	stack_4 = rom_031B14[r0];
	r19 = rom_0318CC[r0];
	r10 = rom_109694[r0] + 0x10u;
	r11 = current_object + 0xB0u;
	r0 = rom_002CAC[SCREEN(screen)->unk_00];
	r9 = 4u;
	do
	{
		mem_write32(r11, mem_read32(r10) | (r0 & ~0xFFFFFFu));
		r11 += 4u;
		r0 = r0 << 8;
		r9--;
		r10 += 4u;
	} while (r9 != 0u);
	mem_write32(r11, 0xFFFFFFFFu);
	r10 = current_object + 0xB0u;
	sub_011011(r10);
	sub_0115C5(r19);
	r19 = stack_4;
	sub_0184D0(r19);
	mem_write16(OBJECT(current_object)->unk_90, 0x200u);
	mem_write16(OBJECT(current_object)->unk_94, 0x200u);
	mem_write16(OBJECT(current_object)->unk_98, 0u);
	mem_write16(OBJECT(current_object)->unk_9C, 0u);
	mem_write16(OBJECT(current_object)->unk_A0, 0u);
	mem_write16(OBJECT(current_object)->unk_A4, 0u);
	mem_write16(OBJECT(current_object)->unk_A8, 0u);
	mem_write16(OBJECT(current_object)->unk_AC, 0u);
	r0 = SCREEN(screen)->unk_00;
	mem_write32(AS32(OBJECT(current_object)->unk_84), byte_swap32(rom_002CAC[r0]));
	mem_write16(OBJECT(current_object)->unk_88, rom_002CB4[r0]);
	mem_write16(OBJECT(current_object)->unk_8C, 0x60u);
	r10 = rom_002CA4[r0];
	r0 = mem_read32(r10);
	r0 = (r0 & ~0xFFFFu) | (r0 & 0xFFF0u);
	mem_write32(r10, (r0 & ~0xFFu) | (uint8_t)(r0 | 1u));
	r10 += 4u;
	r0 = mem_read32(r10);
	mem_write32(r10, (r0 & ~0xFFFFu) | (r0 & 0xFFF0u));
	SCREEN(screen)->unk_94 |= 4u;
}

void sub_002E74(void)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint32_t r0_2 = 0, r1_2 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0, r21 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0;

	switch ((uint8_t)(SCREEN(screen)->unk_80 >> 16u))
	{
	case 0:
		r0 = 0x21u;
		sub_011EDE(r0);
		SCREEN(screen)->unk_F20 = 0xFFFFu;
		unk_20E3F0 = 0u;
		SCREEN(screen)->unk_153 = 0u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 2)++;
		mem_write8(SCREEN(screen)->unk_24 + 0x28u, 0u);
		return;
	case 1:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			r10 = rom_00E0A0[SCREEN(screen)->unk_30C] + 0x20E01Fu;
			r0_2 = mem_read8(r10);
			r10++;
			if (r0_2 == 0u)
				return;
			r1 = SCREEN(screen)->unk_01;
			do
			{
				t1 = mem_read8(r10);
				zero = (uint8_t)(t1 - r1) == 0u;
				r10++;
				r0_2--;
			} while ((r0_2 != 0u) & (t1 != r1));
			if (zero)
			{
				if (SCREEN(screen)->unk_01 == SCREEN(screen)->unk_30C && unk_20E623 == 0u)
					mem_write8(0x80000Au, 1u);
				AT8(SCREEN(screen)->unk_80, 2)++;
			}
		}
		return;
	case 2:
		SCREEN(screen)->unk_90 &= ~0x100u;
		unk_20E0D0[SCREEN(screen)->unk_00] = 0u;
		SCREEN(screen)->unk_98 = mem_read32(SOUND_701F10);
		r7 = SOUND_RAM + 0x1F14u + SCREEN(screen)->unk_00 * 2u;
		sub_0112F0(r7);
		mem_write8(IO_C00074, rom_003463[SCREEN(screen)->unk_00]);
		if (SCREEN(screen)->unk_E2 == 0u)
			sub_012004();
		sub_00340D();
		sub_003425();
		r19 = rom_031E7C;
		sub_0115C0(r19);
		SCREEN(screen)->unk_31A = 0xFFu;
		SCREEN(screen)->unk_308 = rom_00E0A0[SCREEN(screen)->unk_30C] + unk_20E000_ADDRESS;
		SCREEN(screen)->unk_A4 = 0u;
		SCREEN(screen)->unk_A0 = 0u;
		SCREEN(screen)->unk_A5 = 0u;
		SCREEN(screen)->unk_A3 = 0u;
		SCREEN(screen)->unk_A6 = 0u;
		SCREEN(screen)->unk_A1 = 0u;
		SCREEN(screen)->unk_A2 = 0u;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
		mem_write32(r10 + 0xC0u, 2u);
		mem_write32(r10 + 0xC8u, 8u);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005030_ADDRESS);
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 3:
		return;
	case 4:
		SCREEN(screen)->unk_153 = 0u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 2)++;
		SCREEN(screen)->unk_E2 = 0u;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
		mem_write32(r10 + 0xC0u, 2u);
		mem_write32(r10 + 0xC8u, 8u);
		return;
	case 10:
		sub_003407();
		sub_003425();
		r10 = 0x34F4u + SCREEN(screen)->unk_9C * 2u;
		r0 = ROM8(r10);
		SCREEN(screen)->unk_A0 = r0;
		SCREEN(screen)->unk_A3 = r0;
		SCREEN(screen)->unk_A1 = r0;
		SCREEN(screen)->unk_A4 = 0u;
		SCREEN(screen)->unk_A5 = r0;
		SCREEN(screen)->unk_A2 = 0u;
		r11 = screen + 0x490u;
		SCREEN(screen)->unk_308 = r11;
		SCREEN(r11 - 0x490u)->unk_4AF = 1u;
		SCREEN(r11 - 0x490u)->unk_4B0 = SCREEN(screen)->unk_01;
		SCREEN(r11 - 0x490u)->unk_4F2 = r0;
		r0 = rom_000001[r10];
		CAR(player_car)->unk_60 = r0;
		mem_write8(SCREEN(screen)->unk_24 + 0x49u, r0);
		AT8(SCREEN(screen)->unk_80, 2) = 0x1Bu;
		r19 = rom_031E88;
		sub_0115C0(r19);
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Fu) != 0u)
			return;
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		sub_0115B1();
		r10 = 0x346Cu;
		r21 = sub_011E2C(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->unk_01));
		AT8(SCREEN(screen)->unk_80, 2) = 0x17u;
		return;
	case 27:
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Fu) != 0u)
			return;
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		sub_0115B1();
		r10 = 0x346Cu;
		r21 = sub_011E2C(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->unk_01));
		AT8(SCREEN(screen)->unk_80, 2) = 0x17u;
		return;
	case 5:
		if ((SCREEN(screen)->unk_94 & 1u) != 0u)
			return;
		mem_write8(SCREEN(screen)->unk_24 + 0x5Cu, 0u);
		sub_003407();
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x84u);
		else
			mem_write8(IO_C0001C, 4u);
		sub_0115B1();
		r10 = 0x346Cu;
		r21 = sub_011E2C(r10);
		mem_write8(r21 + 0xDAu, mem_read8(screen + 0x320u + SCREEN(screen)->unk_01));
		AT8(SCREEN(screen)->unk_80, 2) = 0x17u;
		return;
	case 23:
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Fu) == 0u)
			return;
		SCREEN(screen)->unk_31B = 0xFFu;
		r10 = 0x3474u;
		if ((SCREEN(screen)->unk_90 & 0x100u) != 0u)
			r10 = 0x34C4u;
		sub_011E2C(r10);
		AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 24:
	case 25:
		AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 26:
		AT8(SCREEN(screen)->unk_80, 2) = 0xBu;
		SCREEN(screen)->unk_108 = 0u;
		SCREEN(screen)->unk_109 = 0u;
		if (SCREEN(screen)->unk_00 != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		return;
	case 11:
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 2) = 6u;
		return;
	case 6:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 7:
		t2 = SCREEN(screen)->unk_94;
		SCREEN(screen)->unk_94 = t2 & ~2u;
		if ((t2 & 2u) != 0u)
		{
			SCREEN(screen)->unk_31B = 0u;
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			AT8(SCREEN(screen)->unk_80, 2)++;
		}
		return;
	case 8:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			r10 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
			mem_write32(r10 + 0xC0u, 2u);
			mem_write32(r10 + 0xC8u, 8u);
			AT8(SCREEN(screen)->unk_80, 2)++;
		}
		return;
	case 9:
		sub_01076A();
		sub_01070C();
		AT8(SCREEN(screen)->unk_80, 2) = 0xCu;
		sub_011C60();
		return;
	case 12:
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Eu) == 0u)
		{
			sub_012004();
			r10 = 0x3504u;
			sub_01930A(r10);
			r0 = 0xBu;
			sub_011EB8(r0);
		}
		r19 = rom_031E84;
		sub_0115C0(r19);
		r10 = 0x34ECu;
		sub_011E2C(r10);
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 13:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
			AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 14:
		if ((SCREEN(screen)->unk_118 & 0x40u) != 0u)
		{
			AT8(SCREEN(screen)->unk_80, 2) = 0x10u;
			return;
		}
		return;
	case 15:
		AT8(SCREEN(screen)->unk_80, 2) = 0x10u;
		return;
	case 16:
		zero = sub_001E89();
		if (zero == 0u)
		{
			AT8(SCREEN(screen)->unk_80, 2) = 0x12u;
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			return;
		}
		AT8(SCREEN(screen)->unk_80, 2) = 0x15u;
		return;
	case 18:
		if ((SCREEN(screen)->unk_94 & 1u) == 0u)
		{
			sub_011C60();
			AT8(SCREEN(screen)->unk_80, 2)++;
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_001EC2_ADDRESS);
		}
		return;
	case 19:
		SCREEN(screen)->unk_153 = 1u;
		SCREEN(screen)->unk_94 |= 1u;
		AT8(SCREEN(screen)->unk_80, 2)++;
		return;
	case 20:
		return;
	case 21:
		sub_00DED2();
		AT8(SCREEN(screen)->unk_80, 2)++;
		zero = sub_00DED9();
		if (zero == 0u)
		{
			AT8(SCREEN(screen)->unk_80, 2) = 0x11u;
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			sub_011FE6();
		}
		return;
	case 22:
		zero = sub_00DED9();
		if (zero == 0u)
		{
			AT8(SCREEN(screen)->unk_80, 2) = 0x11u;
			SCREEN(screen)->unk_153 = 0u;
			SCREEN(screen)->unk_94 |= 1u;
			sub_011FE6();
		}
		return;
	case 17:
		if ((SCREEN(screen)->unk_94 & 1u) != 0u)
			return;
		r9 = mem_read32(SOUND_701F10) - SCREEN(screen)->unk_98;
		r7 = SOUND_RAM + 0x1F18u + SCREEN(screen)->unk_00 * 4u;
		sub_011341(r7, r9);
		if (mem_read16(SOUND_701F20) < (uint16_t)r9)
		{
			r7 = SOUND_701F20;
			sub_01137D(r7, (uint16_t)r9);
		}
		if ((uint16_t)r9 < mem_read16(SOUND_701F22))
		{
			r7 = SOUND_701F22;
			sub_01137D(r7, (uint16_t)r9);
		}
		r6 = SCREEN(screen)->unk_A4;
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Eu) != 0u)
			r6 = 6u;
		r7 = SOUND_RAM + 0x1F24u + r6 * 2u;
		sub_0112F0(r7);
		r1_2 = SCREEN(screen)->unk_00;
		mem_write8(IO_C00074, rom_003465[r1_2]);
		mem_write8(IO_C00074, rom_003467[r1_2]);
		r0 = 0x1Eu;
		sub_011EB8(r0);
		sub_012004();
		sub_01070C();
		SCREEN(screen)->unk_31C = 0u;
		SCREEN(screen)->unk_31A = 0u;
		SCREEN(screen)->unk_318 = 0u;
		sub_00DEC2();
		sub_011C60();
		AT8(SCREEN(screen)->unk_80, 2) = 0x1Cu;
		mem_write8(SCREEN(screen)->unk_24 + 0x28u, 0xFFu);
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->unk_00 * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		sub_018AF0();
		return;
	case 28:
		SCREEN(screen)->unk_80 = 5u;
		return;
	default:
		invalid_jump(rom_002E80[(uint8_t)(SCREEN(screen)->unk_80 >> 16u)]);
	}
}

void sub_003407(void)
{
	sub_012004();
	sub_00340D();
}

void sub_00340D(void)
{
	sub_011C60();
	sub_01070C();
	sub_010889();
	sub_01076A();
}

void sub_003425(void)
{
	sub_00DEC2();
	move_halfwords(player_car, 0x100800u, 0x180u);
	move_halfwords(screen + 0x330u, 0x100800u, 0xB0u);
	move_string(0x100800u, 0x80u, screen + 0x600u, 0x80u, 2u, 0u, 0u, 0u, &r27, &r28);
}

void sub_003520(void)
{
	bool L_003BFC = false, L_003C2D = false;
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r19 = 0, r21 = 0;
	uint8_t t1 = 0, t4 = 0, t5 = 0, t6 = 0, t14 = 0, t17 = 0, t20 = 0, t25 = 0, t32 = 0, t35 = 0;
	uint32_t target1 = 0, t2 = 0, t3 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t15 = 0, t16 = 0, t18 = 0, t19 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t33 = 0, t34 = 0;

	if ((SCREEN(screen)->unk_F44 | SCREEN(screen)->unk_F40) == 0u)
		SCREEN(screen)->unk_F40 = 0x5014u;
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
							do
							{
								do
								{
									target1 = L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003C2D ? 0x383Au : rom_003540[OBJECT(current_object)->unk_0C];
									switch (target1)
									{
									case 0x003594u:
										SCREEN(screen)->unk_5800 = 0u;
										r19 = 0x47E8u + ((uint32_t)SCREEN(screen)->unk_00 << 4) * 4u;
										OBJECT(current_object)->unk_80 = mem_read32(r19);
										r19 += 4u;
										AS32(OBJECT(current_object)->unk_84) = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_88 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_8C = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_90 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_94 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_98 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_9C = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_A0 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_A4 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_A8 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_AC = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_B0 = mem_read32(r19);
										r19 += 4u;
										OBJECT(current_object)->unk_B4 = mem_read32(r19);
										r19 += 4u;
										AS32(OBJECT(current_object)->unk_B8) = mem_read32(r19);
										AS32(OBJECT(current_object)->unk_BC) = mem_read32(r19 + 4u);
										r0 = SCREEN(screen)->unk_00 * 5u * 2u;
										r1 = SCREEN(screen)->unk_34A;
										AT8(OBJECT(current_object)->unk_08, 1) = r1;
										r19 = 0x4868u + (r0 + r1 * 5u) * 4u;
										sub_0179AC(r19);
										r19 = rom_031E8C;
										sub_0115C0(r19);
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003644u:
										OBJECT(current_object)->unk_18 = rom_0048D8[SCREEN(screen)->unk_00];
										mem_write16(OBJECT(current_object)->unk_18, 0u);
										mem_write16(OBJECT(current_object)->unk_8C, 0x200u);
										mem_write16(OBJECT(current_object)->unk_90, 0x200u);
										mem_write16(OBJECT(current_object)->unk_94, 0xA0u);
										mem_write16(OBJECT(current_object)->unk_98, 0x70u);
										mem_write16(AS32(OBJECT(current_object)->unk_84), 0x180u);
										mem_write16(OBJECT(current_object)->unk_88, 0x70u);
										mem_write16(OBJECT(current_object)->unk_9C, 0x8C00u);
										mem_write16(OBJECT(current_object)->unk_A0, 0x70u);
										mem_write16(OBJECT(current_object)->unk_AC, 0xF8u);
										mem_write16(OBJECT(current_object)->unk_B0, 0u);
										mem_write16(OBJECT(current_object)->unk_B4, 0xF8u);
										mem_write16(AS32(OBJECT(current_object)->unk_B8), 0u);
										unk_20E027 = 0xFFu;
										OBJECT(current_object)->unk_10 = 0x1Eu;
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x0036C0u:
										t1 = OBJECT(current_object)->unk_10;
										OBJECT(current_object)->unk_10 = t1 - 1u;
										if (t1 == 1u)
										{
											mem_write32(OBJECT(current_object)->unk_A8, 0u);
											mem_write16(OBJECT(current_object)->unk_18, 0x7EE0u);
											OBJECT(current_object)->unk_10 = 9u;
											OBJECT(current_object)->unk_0C++;
										}
										return;
									case 0x0036DFu:
										t2 = OBJECT(current_object)->unk_8C;
										mem_write16(t2, mem_read16(t2) + 0x20u);
										t3 = OBJECT(current_object)->unk_90;
										mem_write16(t3, mem_read16(t3) + 0x20u);
										unk_20E027 = 0xFFu;
										t4 = OBJECT(current_object)->unk_10;
										OBJECT(current_object)->unk_10 = t4 - 1u;
										if (t4 != 1u)
											return;
										mem_write32(OBJECT(current_object)->unk_80, rom_0048B8[SCREEN(screen)->unk_00]);
										mem_write16(OBJECT(current_object)->unk_8C, 0x200u);
										mem_write16(OBJECT(current_object)->unk_90, 0x200u);
										mem_write16(OBJECT(current_object)->unk_94, 0u);
										mem_write16(OBJECT(current_object)->unk_98, 0u);
										mem_write16(AS32(OBJECT(current_object)->unk_84), rom_0048D0[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)]);
										mem_write16(OBJECT(current_object)->unk_88, 0x8Cu);
										mem_write16(OBJECT(current_object)->unk_9C, mem_read16(AS32(OBJECT(current_object)->unk_84)));
										mem_write16(OBJECT(current_object)->unk_A0, mem_read16(OBJECT(current_object)->unk_88));
										unk_20E027 = 0xFFu;
										SCREEN(screen)->unk_A7 = 0xFFu;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_003E1B_ADDRESS);
										mem_write8(r21 + 9u, CAR(player_car)->unk_60);
										mem_write32(r21 + 0x14u, current_object);
										SCREEN(screen)->unk_5800 |= 0x80u;
										OBJECT(current_object)->unk_30 = 0xA0u;
										OBJECT(current_object)->unk_32 = 0x70u;
										r1 = 0x20u;
										r0_2 = 0u;
										do
										{
											r21 = sub_011D43();
											mem_write32(r21 + 4u, sub_004056_ADDRESS);
											mem_write8(r21 + 9u, r0_2);
											mem_write32(r21 + 0x14u, current_object);
											r0_2++;
											r1--;
										} while (r1 != 0u);
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x0037A7u:
										OBJECT(current_object)->unk_12 = SCREEN(screen)->unk_A4;
										OBJECT(current_object)->unk_13 = CAR(player_car)->unk_D0;
										OBJECT(current_object)->unk_28 = 0u;
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x0037BEu:
										SCREEN(screen)->unk_5800 &= ~0x80u;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_004152_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->unk_28);
										if (OBJECT(current_object)->unk_13 != 0u)
										{
											OBJECT(current_object)->unk_11 = 0x16u;
											OBJECT(current_object)->unk_0C++;
										}
										else
										{
											mem_write8(r21 + 0xAu, 1u);
											r0 = mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->unk_01 << 7));
											r1 = SCREEN(screen)->unk_700;
											AT8(OBJECT(current_object)->unk_08, 3) = divide_s32(r0 << 3, r1);
											OBJECT(current_object)->unk_11 = divide_s32(((r0 * 5u) << 2) + r0 * 2u, r1);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->unk_0C++;
										}
									case 0x00383Au:
										if (!L_003BFC)
										{
											if (!L_003BFC)
											{
												if (!L_003BFC)
												{
													if (!L_003BFC)
													{
														if (!L_003BFC)
														{
															if (!L_003BFC)
															{
																if (!L_003BFC)
																{
																	if (!L_003C2D)
																	{
																		t5 = OBJECT(current_object)->unk_11 - 1u;
																		OBJECT(current_object)->unk_11 = t5;
																		if ((int8_t)t5 >= 0)
																		{
																			SCREEN(screen)->unk_5800 |= 2u;
																			return;
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
										if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003C2D || (SCREEN(screen)->unk_5800 & 0x40u) != 0u)
										{
											if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || !L_003C2D)
											{
												SCREEN(screen)->unk_5800 |= 0x80u;
												if (mem_read8(SCREEN(screen)->unk_20 + 0x5Eu) != 0u)
												{
													r21 = sub_011D43();
													mem_write32(r21 + 4u, sub_003F27_ADDRESS);
													mem_write32(r21 + 0x14u, current_object);
													mem_write8(r21 + 9u, 1u);
												}
												OBJECT(current_object)->unk_10 = 0x3Cu;
												OBJECT(current_object)->unk_0C = 0xFu;
											}
											t6 = OBJECT(current_object)->unk_10;
											OBJECT(current_object)->unk_10 = t6 - 1u;
											if (t6 == 1u)
											{
												mem_write32(OBJECT(current_object)->unk_80, rom_0048C0[SCREEN(screen)->unk_00]);
												mem_write16(OBJECT(current_object)->unk_8C, 0x400u);
												mem_write16(OBJECT(current_object)->unk_90, 0x400u);
												t7 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t7, mem_read16(t7) - 0x180u);
												t8 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t8, mem_read16(t8) - ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 8));
												t9 = OBJECT(current_object)->unk_88;
												mem_write16(t9, mem_read16(t9) - 0xF7u);
												t10 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t10, (int16_t)mem_read16(t10) >> 1);
												t11 = OBJECT(current_object)->unk_88;
												mem_write16(t11, (int16_t)mem_read16(t11) >> 1);
												mem_write16(OBJECT(current_object)->unk_94, 0xA0u);
												mem_write16(OBJECT(current_object)->unk_98, 0x70u);
												t12 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t12, mem_read16(t12) + 0x1D0u);
												t13 = OBJECT(current_object)->unk_88;
												mem_write16(t13, mem_read16(t13) + 0xA3u);
												mem_write16(OBJECT(current_object)->unk_9C, mem_read16(AS32(OBJECT(current_object)->unk_84)));
												mem_write16(OBJECT(current_object)->unk_A0, mem_read16(OBJECT(current_object)->unk_88));
												OBJECT(current_object)->unk_30 = rom_0048D4[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)];
												OBJECT(current_object)->unk_32 = 0x70u;
												OBJECT(current_object)->unk_30 -= mem_read16(AS32(OBJECT(current_object)->unk_84));
												OBJECT(current_object)->unk_32 -= mem_read16(OBJECT(current_object)->unk_88);
												OBJECT(current_object)->unk_30 = (int16_t)OBJECT(current_object)->unk_30 >> 3;
												OBJECT(current_object)->unk_32 = (int16_t)OBJECT(current_object)->unk_32 >> 3;
												unk_20E027 = 0xFFu;
												SCREEN(screen)->unk_A7 = 0xFFu;
												SCREEN(screen)->unk_5800 |= 0x10u;
												OBJECT(current_object)->unk_10 = 9u;
												OBJECT(current_object)->unk_0C++;
											}
											return;
										}
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_003F27_ADDRESS);
										mem_write32(r21 + 0x14u, current_object);
										mem_write8(r21 + 9u, 0u);
										SCREEN(screen)->unk_5800 &= ~2u;
										OBJECT(current_object)->unk_13--;
										OBJECT(current_object)->unk_28++;
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003877u:
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_004152_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->unk_28);
										if (OBJECT(current_object)->unk_13 == 0u)
											mem_write8(r21 + 0xAu, 1u);
										if (OBJECT(current_object)->unk_12 != 0u)
										{
											OBJECT(current_object)->unk_11 = 6u;
											OBJECT(current_object)->unk_0C++;
										}
										else
										{
											r2 = screen + 0x700u;
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->unk_01 << 7)) - CAR(player_car)->unk_4A - mem_read16(r2 + SCREEN(screen)->unk_A6 * 2u);
											r3 = SCREEN(r2 - 0x700u)->unk_73E;
											AT8(OBJECT(current_object)->unk_08, 3) = divide_s32(r0 << 2, r3);
											OBJECT(current_object)->unk_11 = divide_s32(r0 * 3u * 2u, r3);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->unk_0C++;
										}
									case 0x003908u:
										t14 = OBJECT(current_object)->unk_11 - 1u;
										OBJECT(current_object)->unk_11 = t14;
										if ((int8_t)t14 >= 0)
										{
											if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												t15 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t15, mem_read16(t15) - 8u);
												OBJECT(current_object)->unk_30 += 8u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											t16 = AS32(OBJECT(current_object)->unk_84);
											mem_write16(t16, mem_read16(t16) + 8u);
											OBJECT(current_object)->unk_30 -= 8u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->unk_12--;
										OBJECT(current_object)->unk_0C++;
									case 0x00394Eu:
										if (OBJECT(current_object)->unk_13 != 0u)
										{
											OBJECT(current_object)->unk_11 = 0xBu;
											OBJECT(current_object)->unk_0C++;
										}
										else
										{
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->unk_01 << 7)) - CAR(player_car)->unk_4A;
											r1 = mem_read16(screen + 0x700u + SCREEN(screen)->unk_A6 * 2u);
											AT8(OBJECT(current_object)->unk_08, 3) = divide_s32(r0 << 2, r1);
											AT8(OBJECT(current_object)->unk_08, 3) += 4u;
											OBJECT(current_object)->unk_11 = divide_s32(r0 * 5u * 2u + r0, r1);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->unk_0C++;
										}
									case 0x0039B4u:
										t17 = OBJECT(current_object)->unk_11 - 1u;
										OBJECT(current_object)->unk_11 = t17;
										if ((int8_t)t17 >= 0)
										{
											if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												t18 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t18, mem_read16(t18) - 5u);
												OBJECT(current_object)->unk_30 += 5u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											t19 = AS32(OBJECT(current_object)->unk_84);
											mem_write16(t19, mem_read16(t19) + 5u);
											OBJECT(current_object)->unk_30 -= 5u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_003F27_ADDRESS);
										mem_write32(r21 + 0x14u, current_object);
										mem_write8(r21 + 9u, 0u);
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->unk_13--;
										OBJECT(current_object)->unk_28++;
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003A14u:
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_004152_ADDRESS);
										mem_write32(r21 + 0x28u, OBJECT(current_object)->unk_28);
										if (OBJECT(current_object)->unk_13 == 0u)
											mem_write8(r21 + 0xAu, 1u);
										if (OBJECT(current_object)->unk_12 != 0u)
										{
											OBJECT(current_object)->unk_11 = 0xAu;
											OBJECT(current_object)->unk_0C++;
										}
										else
										{
											r2 = screen + 0x700u;
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->unk_01 << 7)) - CAR(player_car)->unk_4A - mem_read16(r2 + SCREEN(screen)->unk_A6 * 2u);
											r3 = SCREEN(r2 - 0x700u)->unk_73E;
											AT8(OBJECT(current_object)->unk_08, 3) = divide_s32(r0 << 2, r3);
											OBJECT(current_object)->unk_11 = divide_s32(r0 * 5u * 2u, r3);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->unk_0C++;
										}
									case 0x003AA5u:
										t20 = OBJECT(current_object)->unk_11 - 1u;
										OBJECT(current_object)->unk_11 = t20;
										if ((int8_t)t20 >= 0)
										{
											if (mem_read8(screen + 0x34Au + (OBJECT(current_object)->unk_28 - 1u)) != 0u)
											{
												if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
												{
													SCREEN(screen)->unk_5800 |= 1u;
													t21 = OBJECT(current_object)->unk_88;
													mem_write16(t21, mem_read16(t21) + 4u);
													t22 = AS32(OBJECT(current_object)->unk_84);
													mem_write16(t22, mem_read16(t22) - 1u);
													OBJECT(current_object)->unk_32 -= 4u;
													OBJECT(current_object)->unk_30++;
													return;
												}
												SCREEN(screen)->unk_5800 |= 2u;
												t23 = OBJECT(current_object)->unk_88;
												mem_write16(t23, mem_read16(t23) - 4u);
												t24 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t24, mem_read16(t24) + 1u);
												OBJECT(current_object)->unk_32 += 4u;
												OBJECT(current_object)->unk_30--;
												return;
											}
											if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
											{
												SCREEN(screen)->unk_5800 |= 2u;
												t23 = OBJECT(current_object)->unk_88;
												mem_write16(t23, mem_read16(t23) - 4u);
												t24 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t24, mem_read16(t24) + 1u);
												OBJECT(current_object)->unk_32 += 4u;
												OBJECT(current_object)->unk_30--;
												return;
											}
											SCREEN(screen)->unk_5800 |= 1u;
											t21 = OBJECT(current_object)->unk_88;
											mem_write16(t21, mem_read16(t21) + 4u);
											t22 = AS32(OBJECT(current_object)->unk_84);
											mem_write16(t22, mem_read16(t22) - 1u);
											OBJECT(current_object)->unk_32 -= 4u;
											OBJECT(current_object)->unk_30++;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFFCu;
										OBJECT(current_object)->unk_12--;
										OBJECT(current_object)->unk_0C++;
									case 0x003B16u:
										if (OBJECT(current_object)->unk_13 != 0u)
										{
											OBJECT(current_object)->unk_11 = 0xFu;
											OBJECT(current_object)->unk_0C++;
										}
										else
										{
											r0 = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->unk_01 << 7)) - CAR(player_car)->unk_4A;
											r1 = mem_read16(screen + 0x700u + SCREEN(screen)->unk_A6 * 2u);
											AT8(OBJECT(current_object)->unk_08, 3) = divide_s32(r0 << 3, r1);
											AT8(OBJECT(current_object)->unk_08, 3) += 4u;
											OBJECT(current_object)->unk_11 = divide_s32(r0 * 3u * 5u, r1);
											SCREEN(screen)->unk_5800 |= 0x40u;
											OBJECT(current_object)->unk_0C++;
										}
									case 0x003B77u:
										t25 = OBJECT(current_object)->unk_11 - 1u;
										OBJECT(current_object)->unk_11 = t25;
										if ((int8_t)t25 >= 0)
										{
											if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
											{
												SCREEN(screen)->unk_5800 |= 8u;
												if ((int16_t)mem_read16(AS32(OBJECT(current_object)->unk_84)) <= 0x10)
												{
													SCREEN(screen)->unk_5800 |= 0x20u;
													return;
												}
												t26 = AS32(OBJECT(current_object)->unk_84);
												mem_write16(t26, mem_read16(t26) - 7u);
												OBJECT(current_object)->unk_30 += 7u;
												return;
											}
											SCREEN(screen)->unk_5800 |= 4u;
											if ((int16_t)mem_read16(AS32(OBJECT(current_object)->unk_84)) >= 0x2A0)
											{
												SCREEN(screen)->unk_5800 |= 0x20u;
												return;
											}
											t27 = AS32(OBJECT(current_object)->unk_84);
											mem_write16(t27, mem_read16(t27) + 7u);
											OBJECT(current_object)->unk_30 -= 7u;
											return;
										}
										L_003BFC = (SCREEN(screen)->unk_5800 & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->unk_5800 &= 0xFFFFFFF3u;
										OBJECT(current_object)->unk_13--;
										OBJECT(current_object)->unk_28++;
										if (OBJECT(current_object)->unk_28 != 6u)
										{
											r21 = sub_011D43();
											mem_write32(r21 + 4u, sub_003F27_ADDRESS);
											mem_write32(r21 + 0x14u, current_object);
											mem_write8(r21 + 9u, 0u);
											OBJECT(current_object)->unk_0C = 0xBu;
											return;
										}
										L_003BFC = true;
										break;
									case 0x003C2Du:
										L_003C2D = true;
										break;
									case 0x003CF1u:
										t28 = OBJECT(current_object)->unk_8C;
										mem_write16(t28, mem_read16(t28) - 0x40u);
										t29 = OBJECT(current_object)->unk_90;
										mem_write16(t29, mem_read16(t29) - 0x40u);
										t30 = AS32(OBJECT(current_object)->unk_84);
										mem_write16(t30, mem_read16(t30) + OBJECT(current_object)->unk_30);
										t31 = OBJECT(current_object)->unk_88;
										mem_write16(t31, mem_read16(t31) + OBJECT(current_object)->unk_32);
										unk_20E027 = 0xFFu;
										t32 = OBJECT(current_object)->unk_10;
										OBJECT(current_object)->unk_10 = t32 - 1u;
										if (t32 == 1u)
										{
											mem_write16(OBJECT(current_object)->unk_8C, 0x200u);
											mem_write16(OBJECT(current_object)->unk_90, 0x200u);
											mem_write16(AS32(OBJECT(current_object)->unk_84), rom_0048D4[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)]);
											mem_write16(OBJECT(current_object)->unk_88, 0x70u);
											mem_write16(OBJECT(current_object)->unk_9C, mem_read16(AS32(OBJECT(current_object)->unk_84)));
											mem_write16(OBJECT(current_object)->unk_A0, mem_read16(OBJECT(current_object)->unk_88));
											mem_write32(OBJECT(current_object)->unk_A8, rom_0048C8[SCREEN(screen)->unk_00]);
											mem_write16(OBJECT(current_object)->unk_AC, mem_read16(AS32(OBJECT(current_object)->unk_84)));
											mem_write16(OBJECT(current_object)->unk_B0, mem_read16(OBJECT(current_object)->unk_88));
											t33 = OBJECT(current_object)->unk_AC;
											mem_write16(t33, mem_read16(t33) - 0x88u);
											t34 = OBJECT(current_object)->unk_B0;
											mem_write16(t34, mem_read16(t34) - 0x70u);
											mem_write16(OBJECT(current_object)->unk_B4, mem_read16(OBJECT(current_object)->unk_AC));
											mem_write16(AS32(OBJECT(current_object)->unk_B8), mem_read16(OBJECT(current_object)->unk_B0));
											unk_20E027 = 0xFFu;
											OBJECT(current_object)->unk_0C++;
										}
										return;
									case 0x003D94u:
										mem_write16(OBJECT(current_object)->unk_18, 0u);
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_00463C_ADDRESS);
										r1 = OBJECT(current_object)->unk_28;
										if (r1 != 6u)
											r1++;
										r0_2 = 0u;
										do
										{
											r21 = sub_011D43();
											mem_write32(r21 + 4u, sub_004240_ADDRESS);
											mem_write8(r21 + 9u, r0_2);
											mem_write8(r21 + 0xAu, (uint8_t)(OBJECT(current_object)->unk_08 >> 24u));
											mem_write32(r21 + 0x14u, current_object);
											r0_2++;
											r1--;
										} while (r1 != 0u);
										r0_2 = 0x38u;
										sub_019402(r0_2);
										OBJECT(current_object)->unk_10 = 0x5Au;
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003DE9u:
										if (SCREEN(screen)->unk_F30 == 0u)
										{
											r0_2 = 0x39u;
											sub_019402(r0_2);
											OBJECT(current_object)->unk_0C++;
										}
										t35 = OBJECT(current_object)->unk_10;
										OBJECT(current_object)->unk_10 = t35 - 1u;
										if (t35 != 1u)
											return;
										AT8(SCREEN(screen)->unk_80, 2) = 0x10u;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_0046BB_ADDRESS);
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003DFCu:
										t35 = OBJECT(current_object)->unk_10;
										OBJECT(current_object)->unk_10 = t35 - 1u;
										if (t35 != 1u)
											return;
										AT8(SCREEN(screen)->unk_80, 2) = 0x10u;
										r21 = sub_011D43();
										mem_write32(r21 + 4u, sub_0046BB_ADDRESS);
										OBJECT(current_object)->unk_0C++;
										return;
									case 0x003E1Au:
										return;
									default:
										invalid_jump(target1);
									}
								} while (L_003BFC);
							} while (L_003BFC);
						} while (L_003BFC);
					} while (L_003BFC);
				} while (L_003BFC);
			} while (L_003BFC);
		} while (L_003BFC);
	} while (L_003C2D);
}

void sub_003E1B(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_003E28[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = rom_0048E0[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = rom_0048F0[r0];
		OBJECT(current_object)->unk_34 = 0x50u;
		OBJECT(current_object)->unk_36 = 0x50u;
		OBJECT(current_object)->unk_30 = 0x8Cu;
		OBJECT(current_object)->unk_32 = 0xC3u;
		OBJECT(current_object)->unk_40 = 0xCu;
		sub_004122();
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		if (mem_read32(OBJECT(current_object)->unk_14 + 0x28u) != 0u)
		{
			OBJECT(current_object)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 0x80u) != 0u)
		{
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_32 -= 4u;
		OBJECT(current_object)->unk_30++;
		sub_004122();
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 0x80u) != 0u)
		{
			sub_004122();
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 4u) != 0u)
		{
			OBJECT(current_object)->unk_40 = 9u;
			if ((SCREEN(screen)->unk_5800 & 0x20u) != 0u)
				OBJECT(current_object)->unk_30 += 7u;
			sub_004122();
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 8u) != 0u)
		{
			OBJECT(current_object)->unk_40 = 4u;
			if ((SCREEN(screen)->unk_5800 & 0x20u) != 0u)
			{
				OBJECT(current_object)->unk_30 -= 7u;
				sub_004122();
				r0 = sub_00EAE4(r0);
				return;
			}
			sub_004122();
			r0 = sub_00EAE4(r0);
			return;
		}
		if ((SCREEN(screen)->unk_5800 & 1u) == 0u)
		{
			if ((SCREEN(screen)->unk_5800 & 2u) == 0u)
			{
				sub_004122();
				r0 = sub_00EAE4(r0);
				return;
			}
			OBJECT(current_object)->unk_40 = 0xCu;
			sub_004122();
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_40 = 6u;
		sub_004122();
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_003E28[t1]);
	}
}

void sub_003F27(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_003F34[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r9 = rom_00403A[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = rom_00403E[r0];
		OBJECT(current_object)->unk_34 = 0x100u;
		OBJECT(current_object)->unk_36 = 0x100u;
		OBJECT(current_object)->unk_84 = 0xA0u;
		AS16(OBJECT(current_object)->unk_88) = 0x70u;
		OBJECT(current_object)->unk_84 -= mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		AS16(OBJECT(current_object)->unk_88) -= mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		AS16(OBJECT(current_object)->unk_88) -= 8u;
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 += (uint16_t)OBJECT(current_object)->unk_88;
		OBJECT(current_object)->unk_3C = 0x195u;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == 1u)
		{
			OBJECT(current_object)->unk_0C = 2u;
			return;
		}
		OBJECT(current_object)->unk_0C++;
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 += (uint16_t)OBJECT(current_object)->unk_88;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 += (uint16_t)OBJECT(current_object)->unk_88;
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = SCREEN(screen)->unk_A6 - 0x15u;
		OBJECT(current_object)->unk_84 = rom_004042[r0];
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 = 0x5Cu;
		AS16(OBJECT(current_object)->unk_10)--;
		if ((OBJECT(current_object)->unk_10 & 4u) == 0u)
			return;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_003F34[t1]);
	}
}

void sub_004056(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_004064[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = ((uint32_t)(uint8_t)(OBJECT(current_object)->unk_08 >> 8u) << 4) + ((uint32_t)SCREEN(screen)->unk_34A << 9);
		r10 = 0x4C14u + r0;
		r9 = mem_read16(r10);
		r10 += 2u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_34 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_36 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_84 = mem_read16(r10);
		r10 += 2u;
		AS16(OBJECT(current_object)->unk_88) = mem_read16(r10);
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 += (uint16_t)OBJECT(current_object)->unk_88;
		AT8(OBJECT(current_object)->unk_08, 1) = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->unk_0C++;
		sub_004122();
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if ((SCREEN(screen)->unk_5800 & 0x10u) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_30 += OBJECT(current_object)->unk_84;
		OBJECT(current_object)->unk_32 += (uint16_t)OBJECT(current_object)->unk_88;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
		{
			OBJECT(current_object)->unk_10++;
			t2 = OBJECT(current_object)->unk_10 & 1u;
			OBJECT(current_object)->unk_10 = t2;
			if (t2 != 0u)
				return;
		}
		sub_004122();
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_004064[t1]);
	}
}

void sub_004122(void)
{
	OBJECT(current_object)->unk_3C = OBJECT(current_object)->unk_32;
	OBJECT(current_object)->unk_3C += 0xD0u;
	OBJECT(current_object)->unk_3C = OBJECT(current_object)->unk_3C >> 3;
	if (OBJECT(current_object)->unk_3C == 0u)
	{
		OBJECT(current_object)->unk_3C = 0u;
		return;
	}
	if ((int16_t)OBJECT(current_object)->unk_3C >= 0x1A0)
	{
		OBJECT(current_object)->unk_3C = 0x19Fu;
		return;
	}
}

void sub_004152(void)
{
	uint16_t r5 = 0;
	uint32_t r0 = 0, r3 = 0, r9 = 0, r18 = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0;

	r0 = SCREEN(screen)->unk_01;
	if (mem_read8(SCREEN(screen)->unk_308 + 0x1Fu) <= 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	AS32(OBJECT(current_object)->unk_20) = screen + 0x320u + r0;
	r18 = 0x4BA4u + OBJECT(current_object)->unk_28 * 3u * 3u;
	r5 = mem_read16(r18);
	r3 = (0x40u + (OBJECT(current_object)->unk_28 << 4)) << 16;
	r3 |= (uint16_t)(r3 + 0xA8u);
	r18 += 2u;
	stack_4 = r3;
	sub_0119FA(r3, r5, r18);
	r3 = (stack_4 & ~0xFFFFu) | (uint16_t)(stack_4 + 0x40u);
	r0 = SCREEN(screen)->unk_01;
	r0 = mem_read8(screen + 0x360u + r0 + (OBJECT(current_object)->unk_28 << 3)) * 3u;
	if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) == 0u)
	{
		r18 = 0x4BDAu + r0 * 2u;
		r5 = ROM16(r18);
		r18 += 2u;
		sub_0119FA(r3, r5, r18);
		return;
	}
	if (OBJECT(current_object)->unk_28 != 5u)
	{
		r5 = rom_004C0A;
		r18 = 0x4C0Cu;
		sub_0119FA(r3, r5, r18);
		return;
	}
	OBJECT(current_object)->unk_3E = 8u;
	OBJECT(current_object)->unk_3C = 0x19Eu;
	OBJECT(current_object)->unk_30 = 0xF0u;
	OBJECT(current_object)->unk_32 = 0xB0u;
	OBJECT(current_object)->unk_34 = 0x160u;
	OBJECT(current_object)->unk_36 = 0x160u;
	r9 = 4u;
	sub_00F366(r9);
	t1 = ~~mem_read8(AS32(OBJECT(current_object)->unk_20));
	OBJECT(current_object)->unk_40 = t1;
	r0 = 0x41u;
	if (t1 != 0u)
		r0 = 0x42u;
	OBJECT(current_object)->unk_3A = r0;
	r0 = sub_00EAE4(r0);
}

void sub_004240(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r21 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00424C[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00424C[t1]);
	}
	OBJECT(current_object)->unk_3E = 0u;
	OBJECT(current_object)->unk_3A = 0x2D1u;
	OBJECT(current_object)->unk_34 = 0x100u;
	OBJECT(current_object)->unk_36 = 0x100u;
	OBJECT(current_object)->unk_3C = 0x18Eu;
	switch ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u))
	{
	case 0:
		r9 = 0x306u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_30 = 0xFFF7u;
		OBJECT(current_object)->unk_32 = 0x2Bu;
		OBJECT(current_object)->unk_3C++;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != mem_read8(OBJECT(current_object)->unk_14 + 0x28u))
			OBJECT(current_object)->unk_40 = 7u;
		else
		{
			OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_004421_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->unk_40--;
			if ((int8_t)OBJECT(current_object)->unk_40 < 0)
				OBJECT(current_object)->unk_40 = 0u;
		}
		break;
	case 1:
		r9 = rom_004958[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
		sub_00F366(r9);
		OBJECT(current_object)->unk_30 = 0u;
		OBJECT(current_object)->unk_32 = 0u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00457D_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != mem_read8(OBJECT(current_object)->unk_14 + 0x28u))
		{
			OBJECT(current_object)->unk_40 = 7u;
			OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
			OBJECT(current_object)->unk_32 += 0x6Bu;
			OBJECT(current_object)->unk_0C++;
			return;
		}
		OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_004421_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_40--;
		if ((int8_t)OBJECT(current_object)->unk_40 < 0)
		{
			OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
			OBJECT(current_object)->unk_32 += 0x6Bu;
			OBJECT(current_object)->unk_0C++;
			return;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		r1 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u) - 1u;
		r9 = rom_00495C[mem_read8(OBJECT(current_object)->unk_14 + 9u) * 2u + mem_read8(screen + 0x34Au + r1)];
		sub_00F366(r9);
		r0 = mem_read8(screen + 0x343u + r1);
		OBJECT(current_object)->unk_30 = rom_004900[r0];
		OBJECT(current_object)->unk_32 = rom_00492A[r0];
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_00457D_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_3C += (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 5u)
		{
			if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != mem_read8(OBJECT(current_object)->unk_14 + 0x28u))
			{
				OBJECT(current_object)->unk_40 = 0xBu;
				OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
				OBJECT(current_object)->unk_32 += 0x6Bu;
				OBJECT(current_object)->unk_0C++;
				return;
			}
			OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_004421_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->unk_40--;
			if ((int8_t)OBJECT(current_object)->unk_40 < 0)
				OBJECT(current_object)->unk_40 = 0u;
			OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
			OBJECT(current_object)->unk_32 += 0x6Bu;
			OBJECT(current_object)->unk_0C++;
			return;
		}
		if (mem_read8(SCREEN(screen)->unk_20 + 0x5Eu) == 0u)
		{
			OBJECT(current_object)->unk_40 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_004421_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->unk_40--;
			if ((int8_t)OBJECT(current_object)->unk_40 < 0)
			{
				OBJECT(current_object)->unk_40 = 0u;
				OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
				OBJECT(current_object)->unk_32 += 0x6Bu;
				OBJECT(current_object)->unk_0C++;
				return;
			}
		}
		else
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_00457D_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			mem_write8(r21 + 0xAu, 1u);
			AT8(OBJECT(current_object)->unk_08, 2) = 0xBu;
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_004421_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			OBJECT(current_object)->unk_40 = 0xBu;
		}
		break;
	default:
		invalid_jump(rom_004280[(uint8_t)(OBJECT(current_object)->unk_08 >> 8u)]);
	}
	OBJECT(current_object)->unk_30 += rom_004954[mem_read8(OBJECT(current_object)->unk_14 + 9u)];
	OBJECT(current_object)->unk_32 += 0x6Bu;
	OBJECT(current_object)->unk_0C++;
}

void sub_004421(void)
{
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00442C[t1];
	switch (t1)
	{
	case 0:
		break;
	case 1:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00442C[t1]);
	}
	OBJECT(current_object)->unk_3E = 0u;
	r0 = CAR(player_car)->unk_60;
	r9 = rom_0048E0[r0];
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = rom_0048F0[r0];
	OBJECT(current_object)->unk_34 = 0x40u;
	OBJECT(current_object)->unk_36 = 0x40u;
	OBJECT(current_object)->unk_3C = 0x194u;
	switch (mem_read8(OBJECT(current_object)->unk_14 + 9u))
	{
	case 0:
		r10 = 0x4964u + ((uint32_t)mem_read8(OBJECT(current_object)->unk_14 + 0xAu) << 3);
		OBJECT(current_object)->unk_30 = ROM16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_32 = ROM16(r10);
		OBJECT(current_object)->unk_30 += rom_004954[SCREEN(screen)->unk_34A];
		OBJECT(current_object)->unk_32 += 0x6Bu;
		OBJECT(current_object)->unk_40 = (uint8_t)ROM16(r10 + 2u);
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		r10 = 0x49A4u + (((uint32_t)SCREEN(screen)->unk_34A << 6) + ((uint32_t)mem_read8(OBJECT(current_object)->unk_14 + 0xAu) << 3));
		OBJECT(current_object)->unk_30 = mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_32 = mem_read16(r10);
		OBJECT(current_object)->unk_30 += rom_004954[SCREEN(screen)->unk_34A];
		OBJECT(current_object)->unk_32 += 0x6Bu;
		OBJECT(current_object)->unk_40 = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->unk_0C++;
		return;
	case 2:
	case 3:
	case 4:
	case 5:
		r1 = mem_read8(OBJECT(current_object)->unk_14 + 9u) - 1u;
		r10 = 0x4A24u + (((SCREEN(screen)->unk_34A * 3u) << 6) + ((mem_read8(screen + 0x34Au + r1) * 3u) << 5) + ((uint32_t)mem_read8(OBJECT(current_object)->unk_14 + 0xAu) << 3));
		r0 = mem_read8(screen + 0x343u + r1);
		OBJECT(current_object)->unk_30 = rom_004900[r0];
		OBJECT(current_object)->unk_32 = rom_00492A[r0];
		OBJECT(current_object)->unk_30 += mem_read16(r10);
		r10 += 2u;
		OBJECT(current_object)->unk_32 += mem_read16(r10);
		OBJECT(current_object)->unk_30 += rom_004954[SCREEN(screen)->unk_34A];
		OBJECT(current_object)->unk_32 += 0x6Bu;
		OBJECT(current_object)->unk_40 = (uint8_t)mem_read16(r10 + 2u);
		OBJECT(current_object)->unk_0C++;
		return;
	default:
		invalid_jump(rom_004474[mem_read8(OBJECT(current_object)->unk_14 + 9u)]);
	}
}

void sub_004578(void)
{
	OBJECT(current_object)->unk_00 = 0u;
}

void sub_00457D(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_004588[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
		r9 = rom_00403A[r0];
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = rom_00403E[r0];
		OBJECT(current_object)->unk_34 = 0x100u;
		OBJECT(current_object)->unk_36 = 0x100u;
		OBJECT(current_object)->unk_30 = mem_read16(OBJECT(current_object)->unk_14 + 0x30u);
		OBJECT(current_object)->unk_32 = mem_read16(OBJECT(current_object)->unk_14 + 0x32u);
		OBJECT(current_object)->unk_32 -= 6u;
		OBJECT(current_object)->unk_3C = 0x194u;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 16u) == 1u)
		{
			r0 = SCREEN(screen)->unk_34A * 2u + mem_read8(screen + 0x34Au + (mem_read8(OBJECT(current_object)->unk_14 + 9u) - 1u));
			OBJECT(current_object)->unk_30 += rom_00462C[r0];
			OBJECT(current_object)->unk_32 += rom_004634[r0];
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_0C++;
		return;
	case 1:
		sub_00F30D(0u);
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		AS16(OBJECT(current_object)->unk_10)--;
		if ((OBJECT(current_object)->unk_10 & 4u) == 0u)
			return;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_004588[t1]);
	}
}

void sub_00463C(void)
{
	uint32_t r0 = 0, r9 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0x19Du;
		OBJECT(current_object)->unk_30 = rom_004954[SCREEN(screen)->unk_34A];
		OBJECT(current_object)->unk_32 = 0x118u;
		r9 = 0x39u;
		AS16(OBJECT(current_object)->unk_C0) = 0x67u;
		sub_00F366(r9);
		return;
	case 1:
		OBJECT(current_object)->unk_32 -= 2u;
		if (OBJECT(current_object)->unk_32 == 0xDCu)
			OBJECT(current_object)->unk_0C++;
		r0 = AS16(OBJECT(current_object)->unk_10);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		AS16(OBJECT(current_object)->unk_10) = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(current_object)->unk_C0);
		OBJECT(current_object)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		r0 = AS16(OBJECT(current_object)->unk_10);
		r0 = (r0 & ~0xFFu) | (uint8_t)(r0 + 1u);
		AS16(OBJECT(current_object)->unk_10) = r0;
		r0 = (uint16_t)(((r0 >> 2) & 3u) + (uint16_t)OBJECT(current_object)->unk_C0);
		OBJECT(current_object)->unk_3A = r0;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_004648[OBJECT(current_object)->unk_0C]);
	}
}

void sub_0046BB(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0, t3 = 0, t4 = 0, t5 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0046C8[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_10++;
		t2 = OBJECT(current_object)->unk_10 & 0x1Fu;
		OBJECT(current_object)->unk_10 = t2;
		if (t2 == 0u)
		{
			OBJECT(current_object)->unk_3E = 0xAu;
			r9 = 0x28Du;
			sub_00F366(r9);
			OBJECT(current_object)->unk_3A = 0u;
			OBJECT(current_object)->unk_34 = 4u;
			OBJECT(current_object)->unk_36 = 4u;
			OBJECT(current_object)->unk_30 = 0xA0u;
			OBJECT(current_object)->unk_32 = 0x70u;
			OBJECT(current_object)->unk_3C = 0x19Fu;
			OBJECT(current_object)->unk_10 = 0xFu;
			OBJECT(current_object)->unk_0C++;
		}
		return;
	case 1:
		t3 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xFu;
			OBJECT(current_object)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		OBJECT(current_object)->unk_34 += 0x12u;
		OBJECT(current_object)->unk_36 += 0x12u;
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		t4 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t4 - 1u;
		if (t4 == 1u)
		{
			OBJECT(current_object)->unk_10 = 5u;
			OBJECT(current_object)->unk_0C++;
			r0 = sub_00EAE4(r0);
			return;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		t5 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t5 - 1u;
		if (t5 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xFu;
			OBJECT(current_object)->unk_0C--;
			return;
		}
		return;
	default:
		invalid_jump(rom_0046C8[t1]);
	}
}

void sub_004771(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_00477C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r9 = 0x2F4u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x2Au;
		OBJECT(current_object)->unk_34 = 4u;
		OBJECT(current_object)->unk_36 = 4u;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x70u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		OBJECT(current_object)->unk_10 = 0xFu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_00477C[t1]);
	}
	t2 = OBJECT(current_object)->unk_10;
	OBJECT(current_object)->unk_10 = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_0C++;
		r0 = sub_00EAE4(r0);
		return;
	}
	OBJECT(current_object)->unk_34 += 0x12u;
	OBJECT(current_object)->unk_36 += 0x12u;
	r0 = sub_00EAE4(r0);
}

void sub_005030(void)
{
	OBJECT(current_object)->unk_1C = 0x276u;
	OBJECT(current_object)->unk_04 = sub_00503D_ADDRESS;
	sub_00503D();
}

void sub_00503D(void)
{
	uint32_t zero = 0;
	uint8_t r0 = 0, r1 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0_3 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r19 = 0, r21 = 0;
	uint32_t stack_4 = 0;
	uint8_t t1 = 0, t2 = 0, t5 = 0, t6 = 0;
	uint16_t t3 = 0, t7 = 0;
	uint32_t t4 = 0;

	if (unk_20FC1A == 0u)
		AS8(OBJECT(current_object)->unk_B0) = 0xFFu;
	OBJECT(current_object)->unk_1C--;
	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		if (SCREEN(screen)->unk_E2 == 0u)
		{
			sub_018AF0();
			SCREEN(screen)->unk_F35 = 0xFFu;
			r10 = 0x6EB6u;
			sub_01930A(r10);
			r0 = 0xFu;
			sub_011EB8(r0);
			r0 = 0x3Bu;
			sub_019402(r0);
		}
		r10 = 0x6920u + ((uint32_t)SCREEN(screen)->unk_00 << 3) * 4u;
		OBJECT(current_object)->unk_98 = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_9C = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_A8 = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_AC = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_A0 = ROM32(r10);
		r10 += 4u;
		OBJECT(current_object)->unk_A4 = ROM32(r10);
		r10 += 4u;
		r19 = ROM32(r10);
		stack_4 = r10 + 4u;
		sub_0179AC(r19);
		unk_20E204 |= mem_read16(stack_4);
		mem_write16(OBJECT(current_object)->unk_AC, 0xE0u);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005862_ADDRESS);
		OBJECT(current_object)->unk_0C++;
		r10 = OBJECT(current_object)->unk_A4;
		r11 = r10 + 0x800u;
		r12 = r10 + 0x1000u;
		r13 = r10 + 0x1800u;
		r0_2 = 0x98u;
		r9 = 0xE0u;
		do
		{
			mem_write16(r10, r0_2);
			mem_write16(r11, r0_2);
			mem_write16(r12, r0_2);
			r12 = r12 + 2u;
			mem_write16(r13, r0_2);
			r13 += 2u;
			r0_2++;
			r9--;
			r10 += 2u;
			r11 += 2u;
		} while (r9 != 0u);
		r10 = OBJECT(current_object)->unk_A0;
		move_halfwords(r10, 0x100800u, 0xE0u);
		move_halfwords(r10 + 0x800u, 0x100800u, 0xE0u);
		move_halfwords(r10 + 0x1000u, 0x100800u, 0xE0u);
		move_halfwords(r10 + 0x1800u, 0x100800u, 0xE0u);
		move_string(0x100800u, 0xE0u, screen + 0x3800u, 0xE0u, 2u, 0u, 0u, 0u, &r27, &r28);
		return;
	case 1:
		if (SCREEN(screen)->unk_B00 != 0u)
			return;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005526_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005F51_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r0 = 8u;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_0056B4_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			t1 = r0;
			r0 = (uint8_t)(t1 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t1 != 1u);
		r0 = 0xCu;
		do
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_005762_ADDRESS);
			mem_write32(r21 + 0x14u, current_object);
			t2 = r0;
			r0 = (uint8_t)(t2 - 1u);
			mem_write8(r21 + 9u, r0);
		} while (t2 != 1u);
		r0_2 = rom_0069B0[mem_read8(screen + 0x320u + SCREEN(screen)->unk_01)];
		CAR(player_car)->unk_60 = (uint8_t)r0_2;
		CAR(player_car)->unk_61 = 0u;
		OBJECT(current_object)->unk_2C = 0u;
		r0_2 = (uint16_t)((uint32_t)r0_2 << 7);
		mem_write16(OBJECT(current_object)->unk_98, 0u - r0_2);
		mem_write16(OBJECT(current_object)->unk_9C, 0u);
		OBJECT(current_object)->unk_24 = (uint32_t)r0_2 << 2;
		AS16(OBJECT(current_object)->unk_28) = 0u;
		AT8(OBJECT(current_object)->unk_08, 1) = 0u;
		AT8(OBJECT(current_object)->unk_08, 2) = 1u;
		OBJECT(current_object)->unk_0E = 0u;
		OBJECT(current_object)->unk_0D = 0u;
		AT8(OBJECT(current_object)->unk_08, 3) = 0x1Eu;
		sub_00C68E();
		SCREEN(screen)->unk_F04 = 0xFFFFFD00u;
		SCREEN(screen)->unk_F08 = 0u;
		OBJECT(current_object)->unk_10 = 0x1Eu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		r19 = rom_031E7C;
		sub_0115C0(r19);
		zero = sub_006294();
		t3 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t3 - 1u;
		if (t3 == 1u)
			OBJECT(current_object)->unk_0C++;
		r0_3 = OBJECT(current_object)->unk_1C;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = sub_011975(r0_3);
		sub_00C70C(r0_3);
		return;
	case 2:
		zero = sub_006294();
		t3 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t3 - 1u;
		if (t3 == 1u)
			OBJECT(current_object)->unk_0C++;
		r0_3 = OBJECT(current_object)->unk_1C;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = sub_011975(r0_3);
		sub_00C70C(r0_3);
		return;
	case 3:
		zero = sub_006294();
		t4 = SCREEN(screen)->unk_F04 + 0x40u;
		SCREEN(screen)->unk_F04 = t4;
		if (t4 != 0u)
		{
			r0_3 = OBJECT(current_object)->unk_1C;
			r0_3 = r0_3 / 0x1Eu;
			r0_3 = sub_011975(r0_3);
			sub_00C70C(r0_3);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005908_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005D5C_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r0_3 = OBJECT(current_object)->unk_1C;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = sub_011975(r0_3);
		sub_00C70C(r0_3);
		return;
	case 4:
		if (OBJECT(current_object)->unk_1C != 0u)
		{
			r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
			if (r0 == 0u)
			{
				r0_3 = SCREEN(screen)->unk_100;
				r0_3 = r0_3 >> 5;
				t5 = rom_006D9C[r0_3];
				t6 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
				AT8(OBJECT(current_object)->unk_08, 2) = t6 - t5;
				if ((int8_t)t6 <= (int8_t)t5)
				{
					AT8(OBJECT(current_object)->unk_08, 2) = 0xFu;
					r1 = 0x10u;
					if (r0_3 >> 2 != 0u)
						r1 = (uint8_t)(0u - r1);
					AT8(OBJECT(current_object)->unk_08, 1) = r1;
				}
			}
			else
			{
				if ((int8_t)r0 < 0)
				{
					if (SCREEN(screen)->unk_100 >= 0x60u)
					{
						AT8(OBJECT(current_object)->unk_08, 1) = r0 + 1u;
						sub_0054C1();
						r0_2 = (uint16_t)(OBJECT(current_object)->unk_24 - 0x20u);
					}
					else
					{
						r0 += 0x10u;
						AT8(OBJECT(current_object)->unk_08, 1) = r0 - 1u;
						sub_0054BC();
						r0_2 = (uint16_t)(OBJECT(current_object)->unk_24 + 0x20u);
					}
				}
				else if (SCREEN(screen)->unk_100 < 0xA0u)
				{
					AT8(OBJECT(current_object)->unk_08, 1) = r0 - 1u;
					sub_0054BC();
					r0_2 = (uint16_t)(OBJECT(current_object)->unk_24 + 0x20u);
				}
				else
				{
					r0 -= 0x10u;
					AT8(OBJECT(current_object)->unk_08, 1) = r0 + 1u;
					sub_0054C1();
					r0_2 = (uint16_t)(OBJECT(current_object)->unk_24 - 0x20u);
				}
				OBJECT(current_object)->unk_24 = r0_2;
				mem_write16(OBJECT(current_object)->unk_98, 0u - (r0_2 >> 2));
				if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == 0u)
				{
					r0_3 = SCREEN(screen)->unk_100;
					r0_3 = r0_3 >> 5;
					t5 = rom_006D9C[r0_3];
					t6 = (uint8_t)(OBJECT(current_object)->unk_08 >> 16u);
					AT8(OBJECT(current_object)->unk_08, 2) = t6 - t5;
					if ((int8_t)t6 <= (int8_t)t5)
					{
						AT8(OBJECT(current_object)->unk_08, 2) = 0xFu;
						r1 = 0x10u;
						if (r0_3 >> 2 != 0u)
							r1 = (uint8_t)(0u - r1);
						AT8(OBJECT(current_object)->unk_08, 1) = r1;
					}
				}
			}
			CAR(player_car)->unk_60 = ((OBJECT(current_object)->unk_24 + 0x100u) & 0xFFFu) >> 9;
			zero = sub_006294();
			if (zero)
			{
				sub_0053DF();
				sub_005443();
				r0_3 = OBJECT(current_object)->unk_1C;
				r0_3 = r0_3 / 0x1Eu;
				r0_3 = sub_011975(r0_3);
				sub_00C70C(r0_3);
				return;
			}
		}
		AS8(OBJECT(current_object)->unk_2C) = 0xFFu;
		r10 = SCREEN(screen)->unk_24;
		r0_3 = CAR(player_car)->unk_60;
		mem_write8(r10 + 0x49u, r0_3);
		mem_write8(r10 + 0x48u, r0_3 + 1u);
		AT8(OBJECT(current_object)->unk_08, 3) = 0x1Eu;
		r0 = rom_006DEE[r0_3];
		sub_011EB8(r0);
		if ((int16_t)OBJECT(current_object)->unk_1C < 0xB4)
			OBJECT(current_object)->unk_1C = 0xB4u;
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005A1D_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_0C++;
		sub_0053DF();
		sub_005443();
		r0_3 = OBJECT(current_object)->unk_1C;
		r0_3 = r0_3 / 0x1Eu;
		r0_3 = sub_011975(r0_3);
		sub_00C70C(r0_3);
		return;
	case 5:
		if (OBJECT(current_object)->unk_1C != 0u)
		{
			r0_3 = SCREEN(screen)->unk_100;
			CAR(player_car)->unk_61 = rom_006DA4[r0_3 >> 5];
			zero = sub_006294();
			if (zero)
			{
				sub_0053DF();
				sub_005443();
				r0_3 = OBJECT(current_object)->unk_1C;
				r0_3 = r0_3 / 0x1Eu;
				r0_3 = sub_011975(r0_3);
				sub_00C70C(r0_3);
				return;
			}
		}
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005EB4_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		AT8(OBJECT(current_object)->unk_2C, 1) = 0xFFu;
		OBJECT(current_object)->unk_0C++;
	case 6:
		OBJECT(current_object)->unk_10 = 0x3Cu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		if ((uint8_t)OBJECT(current_object)->unk_B0 == 0u)
		{
			sub_0053DF();
			sub_005443();
			return;
		}
		sub_00DED2();
		OBJECT(current_object)->unk_0C++;
		zero = sub_00DED9();
		if (zero)
		{
			sub_0053DF();
			sub_005443();
			return;
		}
		r0 = 0x3Cu;
		sub_0193FC(r0);
		OBJECT(current_object)->unk_0C++;
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			sub_011FE6();
			AT8(SCREEN(screen)->unk_80, 2)++;
			OBJECT(current_object)->unk_0C++;
		}
		sub_0053DF();
		sub_005443();
		return;
	case 7:
		if ((uint8_t)OBJECT(current_object)->unk_B0 == 0u)
		{
			sub_0053DF();
			sub_005443();
			return;
		}
		sub_00DED2();
		OBJECT(current_object)->unk_0C++;
		zero = sub_00DED9();
		if (zero)
		{
			sub_0053DF();
			sub_005443();
			return;
		}
		r0 = 0x3Cu;
		sub_0193FC(r0);
		OBJECT(current_object)->unk_0C++;
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			sub_011FE6();
			AT8(SCREEN(screen)->unk_80, 2)++;
			OBJECT(current_object)->unk_0C++;
		}
		sub_0053DF();
		sub_005443();
		return;
	case 8:
		zero = sub_00DED9();
		if (zero)
		{
			sub_0053DF();
			sub_005443();
			return;
		}
		r0 = 0x3Cu;
		sub_0193FC(r0);
		OBJECT(current_object)->unk_0C++;
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			sub_011FE6();
			AT8(SCREEN(screen)->unk_80, 2)++;
			OBJECT(current_object)->unk_0C++;
		}
		sub_0053DF();
		sub_005443();
		return;
	case 9:
		t7 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t7 - 1u;
		if (t7 == 1u)
		{
			sub_011FE6();
			AT8(SCREEN(screen)->unk_80, 2)++;
			OBJECT(current_object)->unk_0C++;
		}
		sub_0053DF();
		sub_005443();
		return;
	case 10:
		sub_0053DF();
		sub_005443();
		return;
	default:
		invalid_jump(rom_005058[OBJECT(current_object)->unk_0C]);
	}
}

void sub_0053DF(void)
{
	uint32_t negative = 0;
	uint16_t r1 = 0;
	uint16_t t1 = 0;

	switch (OBJECT(current_object)->unk_0F)
	{
	case 0:
		if ((uint8_t)OBJECT(current_object)->unk_2C == 0u)
			return;
		OBJECT(current_object)->unk_0F++;
		r1 = OBJECT(current_object)->unk_24;
		t1 = (((uint32_t)CAR(player_car)->unk_60 << 9) - r1) << 4;
		negative = t1 >> 15;
		if (t1 != 0u)
		{
			if (negative)
			{
				r1 -= 0x20u;
				OBJECT(current_object)->unk_24 = r1;
				mem_write16(OBJECT(current_object)->unk_98, 0u - (r1 >> 2));
				sub_0054C1();
				return;
			}
			r1 += 0x20u;
			OBJECT(current_object)->unk_24 = r1;
			mem_write16(OBJECT(current_object)->unk_98, 0u - (r1 >> 2));
			sub_0054BC();
			return;
		}
		OBJECT(current_object)->unk_0F++;
		return;
	case 1:
		r1 = OBJECT(current_object)->unk_24;
		t1 = (((uint32_t)CAR(player_car)->unk_60 << 9) - r1) << 4;
		negative = t1 >> 15;
		if (t1 != 0u)
		{
			if (negative)
			{
				r1 -= 0x20u;
				OBJECT(current_object)->unk_24 = r1;
				mem_write16(OBJECT(current_object)->unk_98, 0u - (r1 >> 2));
				sub_0054C1();
				return;
			}
			r1 += 0x20u;
			OBJECT(current_object)->unk_24 = r1;
			mem_write16(OBJECT(current_object)->unk_98, 0u - (r1 >> 2));
			sub_0054BC();
			return;
		}
		OBJECT(current_object)->unk_0F++;
		return;
	case 2:
		return;
	default:
		invalid_jump(rom_0053EC[OBJECT(current_object)->unk_0F]);
	}
}

void sub_005443(void)
{
	uint8_t r1_2 = 0;
	uint16_t r0_2 = 0;
	uint32_t r0 = 0, r1 = 0, r9 = 0, r10 = 0, r11 = 0;

	r0 = (uint8_t)(unk_20E204 >> 8u);
	r1 = r0;
	r0 = (uint8_t)(r0 + 4u);
	r1 = (uint8_t)(r1 + 8u);
	r11 = OBJECT(current_object)->unk_A0 + ((r0 & 0xCu) << 7) * 4u;
	r12 = OBJECT(current_object)->unk_A0 + ((r1 & 0xCu) << 7) * 4u;
	r0_2 = (uint16_t)((uint16_t)OBJECT(current_object)->unk_28 + 1u);
	AS16(OBJECT(current_object)->unk_28) = r0_2;
	r9 = 0x68u;
	do
	{
		mem_write16(r11, r0_2);
		mem_write16(r12, r0_2);
		r12 = r12 + 2u;
		r9--;
		r11 += 2u;
	} while (r9 != 0u);
	switch (OBJECT(current_object)->unk_0E)
	{
	case 1:
	case 2:
		r1_2 = 0xFAu;
		r10 = screen + 0x3800u;
		r9 = 0x78u;
		do
		{
			r0_2 = shift_by(mem_read16(r10), (uint8_t)r1_2, 16);
			mem_write16(r11, r0_2);
			r11 += 2u;
			mem_write16(r12, r0_2);
			r12 = r12 + 2u;
			r9--;
			r10 += 2u;
		} while (r9 != 0u);
		OBJECT(current_object)->unk_0E--;
		break;
	case 3:
		OBJECT(current_object)->unk_0E--;
		return;
	case 0:
		break;
	default:
		invalid_jump(rom_00548C[OBJECT(current_object)->unk_0E]);
	}
}

void sub_0054BC(void)
{
	uint16_t r0 = 0;

	r0 = 6u;
	sub_0054C4(r0);
}

void sub_0054C1(void)
{
	uint16_t r0 = 0;

	r0 = 0xFFFAu;
	sub_0054C4(r0);
}

void sub_0054C4(uint16_t r0)
{
	uint8_t r4 = 0;
	uint16_t r1_2 = 0, r2_2 = 0, r3 = 0;
	uint32_t r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;

	r1 = (uint8_t)(unk_20E204 >> 8u);
	r2 = r1;
	r1 = (uint8_t)(r1 + 4u);
	r2 = (uint8_t)(r2 + 8u);
	r10 = OBJECT(current_object)->unk_A0;
	r11 = r10 + 0xD0u + ((r1 & 0xCu) << 7) * 4u;
	r12 = r10 + 0xD0u + ((r2 & 0xCu) << 7) * 4u;
	r10 = screen + 0x3800u;
	r3 = 0u;
	r4 = 0xFAu;
	r9 = 0x78u;
	do
	{
		r3 += r0;
		r1_2 = (uint16_t)(mem_read16(r10) + r3);
		r2_2 = (uint16_t)(r1_2 + r3);
		mem_write16(r10, r2_2);
		mem_write16(r11, shift_by(r1_2, (uint8_t)r4, 16));
		r11 += 2u;
		mem_write16(r12, shift_by(r2_2, (uint8_t)r4, 16));
		r12 = r12 + 2u;
		r9--;
		r10 += 2u;
	} while (r9 != 0u);
	OBJECT(current_object)->unk_0E = 3u;
}

void sub_005526(void)
{
	bool L_0055F8 = false;
	uint32_t negative = 0;
	uint16_t r1 = 0;
	uint32_t r0 = 0, r1_2 = 0, r2 = 0, r3 = 0, r9 = 0, ignored = 0;
	uint16_t t1 = 0;
	uint32_t target1 = 0;

	do
	{
		target1 = L_0055F8 ? 0x557Du : rom_005534[OBJECT(current_object)->unk_0C];
		switch (target1)
		{
		case 0x005548u:
			OBJECT(current_object)->unk_3E = 0xAu;
			r9 = 0x269u;
			OBJECT(current_object)->unk_3A = 0x7Cu;
			sub_00F366(r9);
			OBJECT(current_object)->unk_2C = mem_read16(OBJECT(current_object)->unk_14 + 0x24u) + 0x100u;
			OBJECT(current_object)->unk_2E = 0x160u;
			OBJECT(current_object)->unk_1C = 0xC0u;
			OBJECT(current_object)->unk_0C++;
		case 0x00557Du:
			if (L_0055F8 || ((((uint32_t)CAR(player_car)->unk_60 << 9) - OBJECT(current_object)->unk_2C) & 0xFFFu) == 0u)
			{
				if (!L_0055F8)
				{
					if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) == 0u)
					{
						r0 = OBJECT(current_object)->unk_2C;
						r0 = (uint16_t)(r0 - 0x40u);
						r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
						r0 = r0 & 0xFFFu;
						r1_2 = (int16_t)rom_100C00[r0];
						r3 = 0u - (int16_t)rom_101400[r0];
						r0 = OBJECT(current_object)->unk_2E;
						r1_2 = (int32_t)(r1_2 * r0) >> 14;
						r2 = 0x6Cu;
						r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
						sub_0061DB(r1_2, r2, r3);
						ignored = sub_00F30D(0u);
						return;
					}
					OBJECT(current_object)->unk_20 = 0u;
					OBJECT(current_object)->unk_0C = 3u;
				}
				r0 = OBJECT(current_object)->unk_20;
				r1 = (uint16_t)((uint32_t)CAR(player_car)->unk_61 << 7);
				if (r0 == r1)
				{
					if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Du) == 0u)
					{
						r9 = 0x269u;
						if (OBJECT(current_object)->unk_38 != 0x269u)
							sub_00F366(r9);
						r0 = (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
						r0 = (uint16_t)(r0 - 0x40u);
						r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
						r0 = r0 & 0xFFFu;
						r1_2 = (int16_t)rom_100C00[r0];
						r3 = 0u - (int16_t)rom_101400[r0];
						r0 = OBJECT(current_object)->unk_2E;
						r1_2 = (int32_t)(r1_2 * r0) >> 14;
						r2 = 0x6Cu;
						r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
						sub_0061DB(r1_2, r2, r3);
						ignored = sub_00F30D(0u);
						return;
					}
					OBJECT(current_object)->unk_2C += OBJECT(current_object)->unk_20;
					r9 = 0x267u;
					if (CAR(player_car)->unk_61 != 0u)
						r9 = 0x268u;
					sub_00F366(r9);
					OBJECT(current_object)->unk_0C++;
					r0 = OBJECT(current_object)->unk_2C;
					r0 = (uint16_t)(r0 - 0x40u);
					r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
					r0 = r0 & 0xFFFu;
					r1_2 = (int16_t)rom_100C00[r0];
					r3 = 0u - (int16_t)rom_101400[r0];
					r0 = OBJECT(current_object)->unk_2E;
					r1_2 = (int32_t)(r1_2 * r0) >> 14;
					r2 = 0x6Cu;
					r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
					sub_0061DB(r1_2, r2, r3);
					ignored = sub_00F30D(0u);
					return;
				}
				if ((int16_t)r1 < (int16_t)r0)
				{
					OBJECT(current_object)->unk_00 &= 0xBFu;
					r0 = (uint16_t)(r0 - 0x20u);
				}
				else
				{
					OBJECT(current_object)->unk_00 |= 0x40u;
					r0 = (uint16_t)(r0 + 0x20u);
				}
				OBJECT(current_object)->unk_20 = r0;
				r9 = 0x266u;
				if (OBJECT(current_object)->unk_38 == 0x266u)
				{
					r0 = (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
					r0 = (uint16_t)(r0 - 0x40u);
					r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
					r0 = r0 & 0xFFFu;
					r1_2 = (int16_t)rom_100C00[r0];
					r3 = 0u - (int16_t)rom_101400[r0];
					r0 = OBJECT(current_object)->unk_2E;
					r1_2 = (int32_t)(r1_2 * r0) >> 14;
					r2 = 0x6Cu;
					r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
					sub_0061DB(r1_2, r2, r3);
					ignored = sub_00F30D(0u);
					return;
				}
				sub_00F366(r9);
				r0 = (uint16_t)(r0 + OBJECT(current_object)->unk_2C);
				r0 = (uint16_t)(r0 - 0x40u);
				r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
				r0 = r0 & 0xFFFu;
				r1_2 = (int16_t)rom_100C00[r0];
				r3 = 0u - (int16_t)rom_101400[r0];
				r0 = OBJECT(current_object)->unk_2E;
				r1_2 = (int32_t)(r1_2 * r0) >> 14;
				r2 = 0x6Cu;
				r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
				sub_0061DB(r1_2, r2, r3);
				ignored = sub_00F30D(0u);
				return;
			}
			r9 = 0x266u;
			sub_00F366(r9);
			OBJECT(current_object)->unk_0C++;
		case 0x0055A6u:
			r0 = OBJECT(current_object)->unk_2C;
			t1 = (r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u)) << 4;
			negative = t1 >> 15;
			if (t1 == 0u)
			{
				r9 = 0x269u;
				sub_00F366(r9);
				OBJECT(current_object)->unk_00 &= 0xBFu;
				OBJECT(current_object)->unk_0C--;
			}
			else if (negative)
			{
				OBJECT(current_object)->unk_00 |= 0x40u;
				r0 = (uint16_t)(r0 + 0x20u);
				OBJECT(current_object)->unk_2C = r0;
			}
			else
			{
				OBJECT(current_object)->unk_00 &= 0xBFu;
				r0 = (uint16_t)(r0 - 0x20u);
				OBJECT(current_object)->unk_2C = r0;
			}
			break;
		case 0x0055F8u:
			L_0055F8 = true;
			break;
		case 0x00566Au:
			r0 = OBJECT(current_object)->unk_2C;
			r0 = (uint16_t)(r0 - 0x40u);
			r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
			r0 = r0 & 0xFFFu;
			r1_2 = (int16_t)rom_100C00[r0];
			r3 = 0u - (int16_t)rom_101400[r0];
			r0 = OBJECT(current_object)->unk_2E;
			r1_2 = (int32_t)(r1_2 * r0) >> 14;
			r2 = 0x6Cu;
			r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
			sub_0061DB(r1_2, r2, r3);
			ignored = sub_00F30D(0u);
			return;
		default:
			invalid_jump(target1);
		}
	} while (L_0055F8);
	r0 = (uint16_t)(r0 - 0x40u);
	r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
	r0 = r0 & 0xFFFu;
	r1_2 = (int16_t)rom_100C00[r0];
	r3 = 0u - (int16_t)rom_101400[r0];
	r0 = OBJECT(current_object)->unk_2E;
	r1_2 = (int32_t)(r1_2 * r0) >> 14;
	r2 = 0x6Cu;
	r3 = ((int32_t)(r3 * r0) >> 14) + 0x140u;
	sub_0061DB(r1_2, r2, r3);
	ignored = sub_00F30D(0u);
}

void sub_0056B4(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, ignored = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0056C0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r10 = rom_0069B8_ADDRESS + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u) * 4u;
		r9 = ROM16(r10);
		OBJECT(current_object)->unk_3A = ROM16(rom_000002_ADDRESS + r10);
		sub_00F366(r9);
		OBJECT(current_object)->unk_1C = 0x100u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_0056C0[t1]);
	}
	r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
	r0 = (uint16_t)(r0 << 9);
	r0 = (uint16_t)(r0 - mem_read16(OBJECT(current_object)->unk_14 + 0x24u));
	t2 = r0 & 0xFFFu;
	r0 = t2;
	if (t2 == 0u && mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
	{
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == CAR(player_car)->unk_60)
			OBJECT(current_object)->unk_3A += 4u;
		OBJECT(current_object)->unk_0C++;
	}
	OBJECT(current_object)->unk_40 = rom_0069D8[r0 >> 6];
	r2 = SCREEN(screen)->unk_F08 + 0x140u;
	r1 = (int32_t)((int16_t)rom_100C00[r0] * r2) >> 14;
	r3 = (int32_t)((0u - (int16_t)rom_101400[r0]) * r2) >> 14;
	r2 = 0x70u;
	r3 += 0x140u;
	ignored = sub_0061DB(r1, r2, r3);
}

void sub_005762(void)
{
	uint16_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0, ignored = 0;
	uint32_t stack_4 = 0, stack_8 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		r10 = 0x6A18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u) * 2u * 4u;
		r9 = ROM16(r10);
		r10 += 2u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = ROM16(r10);
		r10 += 2u;
		r0 = ROM16(r10);
		OBJECT(current_object)->unk_43 = r0 >> 12;
		OBJECT(current_object)->unk_1C = r0 & 0xFFFu;
		OBJECT(current_object)->unk_18 = r10 + 2u + ROM16(r10 + 2u);
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	default:
		invalid_jump(rom_005770[OBJECT(current_object)->unk_0C]);
	}
	r10 = OBJECT(current_object)->unk_18;
	r11 = current_object + 0x80u;
	OBJECT(current_object)->unk_2C = mem_read16(OBJECT(current_object)->unk_14 + 0x24u);
	for (;;)
	{
		r0_2 = mem_read16(r10);
		r10 += 2u;
		if ((int16_t)r0_2 < 0)
			break;
		r0_2 = (uint16_t)(r0_2 - OBJECT(current_object)->unk_2C);
		r0_2 = r0_2 & 0xFFFu;
		r2 = mem_read16(r10);
		r10 += 2u;
		if (r0_2 < 0x480u || r0_2 > 0xB7Fu)
		{
			r10 += 2u;
			r11 += 2u;
			mem_write16(r11, 0u);
			r11 += 2u;
			continue;
		}
		r2 += SCREEN(screen)->unk_F08;
		r1 = (int32_t)((int16_t)rom_100C00[r0_2] * r2) >> 14;
		r3 = (int32_t)((0u - (int16_t)rom_101400[r0_2]) * r2) >> 14;
		r2 = (int16_t)mem_read16(r10);
		r3 += 0x140u;
		stack_4 = r11;
		OBJECT(current_object)->unk_50 = (screen & ~0xFFFFu) | mem_read16(r11);
		OBJECT(current_object)->unk_00 = (OBJECT(current_object)->unk_00 & 0xDFu) | mem_read16(r11 + 2u);
		stack_8 = r10 + 2u;
		r0_2 = sub_0061DB(r1, r2, r3);
		r10 = stack_8;
		r11 = stack_4;
		mem_write16(r11, (uint16_t)OBJECT(current_object)->unk_50);
		r0 = (uint16_t)((r0_2 & ~0xFFu) | OBJECT(current_object)->unk_00);
		r11 += 2u;
		mem_write16(r11, (r0 & ~0xFFu) | (r0 & 0x20u));
		r11 += 2u;
	}
	ignored = sub_00F30D(0u);
}

void sub_005862(void)
{
	uint32_t r0 = 0, r21 = 0;

	r0 = 7u;
	do
	{
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005883_ADDRESS);
		mem_write8(r21 + 9u, r0);
		r0--;
	} while (r0 != 0u);
	AT8(OBJECT(current_object)->unk_08, 1) = r0;
	OBJECT(current_object)->unk_04 = sub_005883_ADDRESS;
	sub_005883();
}

void sub_005883(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_005890[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 8u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x346u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x18u;
		r0 = (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		r10 = 0x6DACu + r0 * 4u;
		OBJECT(current_object)->unk_30 = ROM16(r10);
		OBJECT(current_object)->unk_32 = 0x50u;
		OBJECT(current_object)->unk_40 = ROM8(rom_000002_ADDRESS + r10);
		r0 = (uint16_t)(r0 * 2u);
		AS16(OBJECT(current_object)->unk_10) = r0;
		OBJECT(current_object)->unk_12 = 0x2Du;
		OBJECT(current_object)->unk_13 = 0u;
		OBJECT(current_object)->unk_18 = 0x6DCCu;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		t2 = AS16(OBJECT(current_object)->unk_12);
		AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00EAE4(r0);
		sub_0068FE();
		return;
	default:
		invalid_jump(rom_005890[t1]);
	}
	AS16(OBJECT(current_object)->unk_12)--;
	t3 = AS16(OBJECT(current_object)->unk_10) - 1u;
	AS16(OBJECT(current_object)->unk_10) = t3;
	if ((int16_t)t3 >= 0)
		return;
	OBJECT(current_object)->unk_0C++;
	t2 = AS16(OBJECT(current_object)->unk_12);
	AS16(OBJECT(current_object)->unk_12) = t2 - 1u;
	if (t2 == 1u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	sub_00EAE4(r0);
	sub_0068FE();
}

void sub_005908(void)
{
	uint32_t r0 = 0, r9 = 0, r10 = 0, r21 = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_005914[t1];
	switch (t1)
	{
	case 0:
		r10 = 0x6DF6u;
		while (ROM16(r10) != 0u)
		{
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_0059B8_ADDRESS);
			mem_write32(r21 + 0x14u, OBJECT(current_object)->unk_14);
			mem_write16(r21 + 0x18u, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x3Au, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x30u, ROM16(r10));
			r10 += 2u;
			mem_write16(r21 + 0x32u, ROM16(r10));
			r10 += 2u;
		}
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x2B5u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x2A6u;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x30u;
		OBJECT(current_object)->unk_10 = 0x14u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		t3 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C--;
		}
		return;
	default:
		invalid_jump(rom_005914[t1]);
	}
}

void sub_0059B8(void)
{
	uint32_t r0 = 0, r9 = 0, ignored = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_0059C4[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0x19Du;
		r9 = (uint16_t)OBJECT(current_object)->unk_18;
		sub_00F366(r9);
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) == 0u)
		{
			sub_00EAE4(r0);
			ignored = sub_00F30D(0u);
			return;
		}
		OBJECT(current_object)->unk_10 = 0x10u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) == 0u)
		{
			sub_00EAE4(r0);
			ignored = sub_00F30D(0u);
			return;
		}
		OBJECT(current_object)->unk_10 = 0x10u;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		OBJECT(current_object)->unk_32 -= 2u;
		t2 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
		if (t2 != 1u)
		{
			sub_00EAE4(r0);
			ignored = sub_00F30D(0u);
			return;
		}
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Du) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00EAE4(r0);
		ignored = sub_00F30D(0u);
		return;
	case 2:
		break;
	case 3:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Du) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		sub_00EAE4(r0);
		ignored = sub_00F30D(0u);
		return;
	default:
		invalid_jump(rom_0059C4[t1]);
	}
	OBJECT(current_object)->unk_32 -= 2u;
	t2 = AS16(OBJECT(current_object)->unk_10);
	AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
	if (t2 != 1u)
	{
		sub_00EAE4(r0);
		ignored = sub_00F30D(0u);
		return;
	}
	OBJECT(current_object)->unk_0C++;
	if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Du) != 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	sub_00EAE4(r0);
	ignored = sub_00F30D(0u);
}

void sub_005A1D(void)
{
	uint8_t r0_2 = 0;
	uint16_t r0_3 = 0;
	uint32_t r0 = 0, r19 = 0, r21 = 0;
	uint8_t t4 = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005B45_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005BD6_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		AT8(OBJECT(current_object)->unk_08, 1) = 0xFFu;
		OBJECT(current_object)->unk_0C++;
	case 1:
		r21 = OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_32 -= 0x10u;
		t1 = mem_read32(r21 + 0xACu);
		t2 = mem_read16(t1);
		mem_write16(t1, t2 - 0x10u);
		if (t2 != 0x10u)
			return;
		OBJECT(current_object)->unk_0C++;
	case 2:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Du) != 0u)
		{
			OBJECT(current_object)->unk_2C = 0x10u;
			r0_2 = 0u;
			if (CAR(player_car)->unk_61 == 0u)
			{
				OBJECT(current_object)->unk_2C = (int8_t)0xF0u;
				r0_2 = 1u;
			}
			OBJECT(current_object)->unk_13 = 0xAu;
			r21 = sub_011D43();
			mem_write32(r21 + 4u, sub_005D14_ADDRESS);
			mem_write8(r21 + 9u, r0_2);
			OBJECT(current_object)->unk_0C++;
			r21 = OBJECT(current_object)->unk_14;
			r0_3 = OBJECT(current_object)->unk_2C;
			OBJECT(current_object)->unk_30 += r0_3;
			t3 = mem_read32(r21 + 0xA8u);
			mem_write16(t3, mem_read16(t3) + r0_3);
			t4 = OBJECT(current_object)->unk_13;
			OBJECT(current_object)->unk_13 = t4 - 1u;
			if (t4 == 1u)
				OBJECT(current_object)->unk_0C++;
		}
		else
		{
			r0 = CAR(player_car)->unk_61;
			if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != r0)
			{
				AT8(OBJECT(current_object)->unk_08, 1) = r0;
				r19 = 0x13F998u;
				if (r0 != 0u)
					r19 = 0x13F95Cu;
				sub_0115C5(r19);
			}
		}
		break;
	case 3:
		r21 = OBJECT(current_object)->unk_14;
		r0_3 = OBJECT(current_object)->unk_2C;
		OBJECT(current_object)->unk_30 += r0_3;
		t3 = mem_read32(r21 + 0xA8u);
		mem_write16(t3, mem_read16(t3) + r0_3);
		t4 = OBJECT(current_object)->unk_13;
		OBJECT(current_object)->unk_13 = t4 - 1u;
		if (t4 == 1u)
			OBJECT(current_object)->unk_0C++;
		break;
	case 4:
		break;
	default:
		invalid_jump(rom_005A28[OBJECT(current_object)->unk_0C]);
	}
	if (CAR(player_car)->unk_61 == 0u)
	{
		r19 = rom_006E30[OBJECT(current_object)->unk_12 >> 1];
		if (r19 == 0u)
		{
			OBJECT(current_object)->unk_12 = 0u;
			r19 = rom_006E30[0];
		}
		OBJECT(current_object)->unk_12++;
		sub_0115C5(r19);
		return;
	}
	r19 = rom_006E44[OBJECT(current_object)->unk_10 >> 1];
	if (r19 == 0u)
	{
		OBJECT(current_object)->unk_10 = 0u;
		r19 = rom_006E44[0];
	}
	OBJECT(current_object)->unk_10++;
	sub_0115C5(r19);
}

void sub_005B45(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005C71_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0xF0u;
		r9 = 0x2BDu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x2A8u;
		OBJECT(current_object)->unk_20 = 0x54u;
		OBJECT(current_object)->unk_24 = 0xFF94u;
		OBJECT(current_object)->unk_18 = 0x6E18u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		r21 = OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
		r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = r0;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_005B50[OBJECT(current_object)->unk_0C]);
	}
	if (CAR(player_car)->unk_61 != 0u)
	{
		OBJECT(current_object)->unk_3A = 0x2AEu;
		r21 = OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
		r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = r0;
		r0 = sub_00EAE4(r0);
		return;
	}
	sub_0068FE();
	r21 = OBJECT(current_object)->unk_14;
	OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
	r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
	OBJECT(current_object)->unk_32 = r0;
	r0 = sub_00EAE4(r0);
}

void sub_005BD6(void)
{
	uint32_t r0 = 0, r9 = 0, r21 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005CA4_ADDRESS);
		mem_write32(r21 + 0x14u, current_object);
		OBJECT(current_object)->unk_3E = 0xAu;
		OBJECT(current_object)->unk_3C = 0xF0u;
		r9 = 0x2BEu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x2A8u;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_20 = 0xECu;
		OBJECT(current_object)->unk_24 = 0xFF94u;
		OBJECT(current_object)->unk_18 = 0x6E18u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		break;
	case 2:
		r21 = OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
		r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = r0;
		r0 = sub_00EAE4(r0);
		return;
	default:
		invalid_jump(rom_005BE4[OBJECT(current_object)->unk_0C]);
	}
	if (CAR(player_car)->unk_61 == 0u)
	{
		OBJECT(current_object)->unk_3A = 0x2AEu;
		r21 = OBJECT(current_object)->unk_14;
		OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
		r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
		OBJECT(current_object)->unk_32 = r0;
		r0 = sub_00EAE4(r0);
		return;
	}
	sub_0068FE();
	r21 = OBJECT(current_object)->unk_14;
	OBJECT(current_object)->unk_30 = 0u - mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
	r0 = (uint16_t)(0u - mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
	OBJECT(current_object)->unk_32 = r0;
	r0 = sub_00EAE4(r0);
}

void sub_005C71(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 0xF0u;
	r9 = 0x2C1u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_3A = 0x2A8u;
	OBJECT(current_object)->unk_20 = 0u;
	OBJECT(current_object)->unk_24 = 0x24u;
	OBJECT(current_object)->unk_04 = sub_005CE5_ADDRESS;
	sub_005CE5();
}

void sub_005CA4(void)
{
	uint32_t r9 = 0;

	OBJECT(current_object)->unk_3E = 0xAu;
	OBJECT(current_object)->unk_3C = 0xF0u;
	r9 = 0x2C9u;
	sub_00F366(r9);
	OBJECT(current_object)->unk_40 = rom_006E78[CAR(player_car)->unk_60];
	OBJECT(current_object)->unk_3A = 0x2A8u;
	OBJECT(current_object)->unk_20 = 0xFFE8u;
	OBJECT(current_object)->unk_24 = 0x5Cu;
	OBJECT(current_object)->unk_04 = sub_005CE5_ADDRESS;
	sub_005CE5();
}

void sub_005CE5(void)
{
	uint32_t r0 = 0, r21 = 0;

	r21 = OBJECT(current_object)->unk_14;
	if ((mem_read32(r21) & 1u) == 0u)
	{
		OBJECT(current_object)->unk_00 = 0u;
		return;
	}
	OBJECT(current_object)->unk_30 = mem_read16(r21 + 0x30u) + OBJECT(current_object)->unk_20;
	r0 = (uint16_t)(mem_read16(r21 + 0x32u) + OBJECT(current_object)->unk_24);
	OBJECT(current_object)->unk_32 = r0;
	OBJECT(current_object)->unk_3A = mem_read16(r21 + 0x3Au);
	r0 = sub_00EAE4(r0);
}

void sub_005D14(void)
{
	uint32_t r9 = 0, r10 = 0;

	OBJECT(current_object)->unk_3E = 0u;
	OBJECT(current_object)->unk_3C = 0xEFu;
	r10 = 0x6E58u + CAR(player_car)->unk_60 * 4u;
	r9 = ROM16(r10);
	OBJECT(current_object)->unk_3A = ROM16(rom_000002_ADDRESS + r10);
	sub_00F366(r9);
	OBJECT(current_object)->unk_30 = 0xF0u;
	if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != 0u)
		OBJECT(current_object)->unk_30 = 0x50u;
	OBJECT(current_object)->unk_32 = 0xD0u;
	OBJECT(current_object)->unk_04 = sub_00EAE4_ADDRESS;
}

void sub_005D5C(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r9 = 0, r21 = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		r21 = sub_011D43();
		mem_write32(r21 + 4u, sub_005E10_ADDRESS);
		mem_write32(r21 + 0x14u, OBJECT(current_object)->unk_14);
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x26Eu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xC4u;
		OBJECT(current_object)->unk_10 = 0xAu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != r0)
		{
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
		}
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_40 = r0;
		OBJECT(current_object)->unk_3A = rom_006DDE[r0];
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != r0)
		{
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
		}
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_40 = r0;
		OBJECT(current_object)->unk_3A = rom_006DDE[r0];
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0_2 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == r0_2)
		{
			t2 = AS16(OBJECT(current_object)->unk_10);
			AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
			if (t2 == 1u)
			{
				AT8(OBJECT(current_object)->unk_08, 1) = r0_2;
				OBJECT(current_object)->unk_10 = 0xAu;
				OBJECT(current_object)->unk_11 = 0u;
				OBJECT(current_object)->unk_0C--;
			}
			return;
		}
		AT8(OBJECT(current_object)->unk_08, 1) = r0_2;
		OBJECT(current_object)->unk_10 = 0xAu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C--;
		return;
	default:
		invalid_jump(rom_005D68[OBJECT(current_object)->unk_0C]);
	}
}

void sub_005E10(void)
{
	uint8_t r0_2 = 0;
	uint32_t r0 = 0, r9 = 0;
	uint16_t t1 = 0, t2 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x2D1u;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x2AFu;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0xD4u;
		AT8(OBJECT(current_object)->unk_08, 1) = 0xFFu;
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != r0)
		{
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
		}
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_40 = rom_006E78[r0];
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) != r0)
		{
			AT8(OBJECT(current_object)->unk_08, 1) = r0;
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
		}
		t1 = AS16(OBJECT(current_object)->unk_10);
		AS16(OBJECT(current_object)->unk_10) = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_11 = 0u;
			OBJECT(current_object)->unk_0C++;
		}
		OBJECT(current_object)->unk_40 = rom_006E78[r0];
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0x2Cu) != 0u)
		{
			OBJECT(current_object)->unk_00 = 0u;
			return;
		}
		r0_2 = CAR(player_car)->unk_60;
		if ((uint8_t)(OBJECT(current_object)->unk_08 >> 8u) == r0_2)
		{
			t2 = AS16(OBJECT(current_object)->unk_10);
			AS16(OBJECT(current_object)->unk_10) = t2 - 1u;
			if (t2 == 1u)
			{
				AT8(OBJECT(current_object)->unk_08, 1) = r0_2;
				OBJECT(current_object)->unk_10 = 0xAu;
				OBJECT(current_object)->unk_11 = 0u;
				OBJECT(current_object)->unk_0C--;
			}
			return;
		}
		AT8(OBJECT(current_object)->unk_08, 1) = r0_2;
		OBJECT(current_object)->unk_10 = 0xAu;
		OBJECT(current_object)->unk_11 = 0u;
		OBJECT(current_object)->unk_0C--;
		return;
	default:
		invalid_jump(rom_005E1C[OBJECT(current_object)->unk_0C]);
	}
}

void sub_005EB4(void)
{
	uint32_t r0 = 0, r9 = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	t1 = OBJECT(current_object)->unk_0C;
	r0 = rom_005EC0[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->unk_3E = 0u;
		OBJECT(current_object)->unk_3C = 0x19Eu;
		r9 = 0x28Bu;
		sub_00F366(r9);
		OBJECT(current_object)->unk_3A = 0x287u;
		OBJECT(current_object)->unk_30 = 0xA0u;
		OBJECT(current_object)->unk_32 = 0x88u;
		OBJECT(current_object)->unk_10 = 0x14u;
		OBJECT(current_object)->unk_0C++;
		if (mem_read8(OBJECT(current_object)->unk_14 + 0xB0u) != 0u)
		{
			r9 = 0x28Du;
			sub_00F366(r9);
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C = 3u;
		}
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 1:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0xB0u) != 0u)
		{
			r9 = 0x28Du;
			sub_00F366(r9);
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C = 3u;
		}
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 3:
		t2 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t2 - 1u;
		if (t2 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0xAu;
			OBJECT(current_object)->unk_0C++;
		}
		r0 = sub_00EAE4(r0);
		return;
	case 2:
		if (mem_read8(OBJECT(current_object)->unk_14 + 0xB0u) != 0u)
		{
			r9 = 0x28Du;
			sub_00F366(r9);
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C = 3u;
			t2 = OBJECT(current_object)->unk_10;
			OBJECT(current_object)->unk_10 = t2 - 1u;
			if (t2 == 1u)
			{
				OBJECT(current_object)->unk_10 = 0xAu;
				OBJECT(current_object)->unk_0C++;
			}
			r0 = sub_00EAE4(r0);
			return;
		}
		t3 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C--;
		}
		return;
	case 4:
		t3 = OBJECT(current_object)->unk_10;
		OBJECT(current_object)->unk_10 = t3 - 1u;
		if (t3 == 1u)
		{
			OBJECT(current_object)->unk_10 = 0x14u;
			OBJECT(current_object)->unk_0C--;
		}
		return;
	default:
		invalid_jump(rom_005EC0[t1]);
	}
}

void sub_005F51(void)
{
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r21 = 0;
	uint8_t t1 = 0;
	uint32_t t2 = 0, t3 = 0;

	switch (OBJECT(current_object)->unk_0C)
	{
	case 0:
		OBJECT(current_object)->unk_18 = SCREEN(screen)->unk_308 + 0x1Fu;
		OBJECT(current_object)->unk_40 = 0u;
		OBJECT(current_object)->unk_10 = 0u;
		OBJECT(current_object)->unk_0C++;
		if (SCREEN(screen)->unk_F44 != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		r9 = unk_20E041;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + r9), 0x5Fu);
			r9--;
		} while (r9 != 0u);
		r0 = unk_20E030[SCREEN(screen)->unk_01];
		AT8(OBJECT(current_object)->unk_08, 1) = r0;
		AT8(OBJECT(current_object)->unk_08, 2) = (r0 + 0x31u) ^ 0x5Fu;
		AT8(OBJECT(current_object)->unk_08, 3) = 8u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 1:
		if (SCREEN(screen)->unk_F44 != 0u)
			return;
		move_string(0x6EB0u, 3u, screen + 0xF12u, 7u, 2u, 0u, 0u, 0u, &r27, &r28);
		r9 = unk_20E041;
		do
		{
			mem_write8(r27 + mem_read8(0x20E02Fu + r9), 0x5Fu);
			r9--;
		} while (r9 != 0u);
		r0 = unk_20E030[SCREEN(screen)->unk_01];
		AT8(OBJECT(current_object)->unk_08, 1) = r0;
		AT8(OBJECT(current_object)->unk_08, 2) = (r0 + 0x31u) ^ 0x5Fu;
		AT8(OBJECT(current_object)->unk_08, 3) = 8u;
		OBJECT(current_object)->unk_0C++;
		break;
	case 2:
		break;
	default:
		invalid_jump(rom_005F5C[OBJECT(current_object)->unk_0C]);
	}
	t1 = (uint8_t)(OBJECT(current_object)->unk_08 >> 24u);
	AT8(OBJECT(current_object)->unk_08, 3) = t1 - 1u;
	if (t1 == 1u)
	{
		t2 = screen + 0xF18u + (uint8_t)(OBJECT(current_object)->unk_08 >> 8u);
		mem_write8(t2, mem_read8(t2) ^ (uint8_t)(OBJECT(current_object)->unk_08 >> 16u));
		AT8(OBJECT(current_object)->unk_08, 3) = 8u;
	}
	r10 = OBJECT(current_object)->unk_18;
	r8 = mem_read8(r10);
	r10++;
	do
	{
		r0_2 = mem_read8(r10);
		r10++;
		if (SCREEN(screen)->unk_01 != r0_2 && (AS32(OBJECT(current_object)->unk_40) & shl32(1u, (int)(r0_2))) == 0u)
		{
			r1 = unk_20E030[r0_2];
			mem_write8(screen + 0xF18u + r1, r1 + 0x31u);
			t3 = mem_read8(0x20FC48u + (r0_2 << 7)) - 1u;
			r1 = t3;
			if ((int32_t)t3 >= 0)
			{
				r11 = mem_read32(current_object + 0x80u + r1 * 4u);
				r21 = sub_011D43();
				if (r11 == 0u)
				{
					mem_write32(r21 + 4u, sub_006070_ADDRESS);
					mem_write32(r21 + 0x18u, r21);
					mem_write32(r21 + 0x14u, current_object);
					mem_write32(current_object + 0x80u + r1 * 4u, r21);
				}
				else
				{
					mem_write32(r21 + 4u, sub_006105_ADDRESS);
					mem_write32(r21 + 0x14u, mem_read32(r11 + 0x18u));
					mem_write32(r11 + 0x18u, r21);
				}
				mem_write8(r21 + 9u, r0_2);
				mem_write8(r21 + 0xAu, r1);
				AS32(OBJECT(current_object)->unk_40) |= shl32(1u, (int)(r0_2));
			}
		}
		r8--;
	} while (r8 != 0u);
	r0_2 = OBJECT(current_object)->unk_10;
	r0_2 = (uint8_t)(r0_2 + 1u);
	OBJECT(current_object)->unk_10 = r0_2;
	r0_2 = r0_2 >> 1;
	OBJECT(current_object)->unk_12 = rom_006EA0[r0_2 & 0xFu];
}
