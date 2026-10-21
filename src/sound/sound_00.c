/**
 *    Name:        sound_00.c
 *    Description: the sound program's functions, part 00 (in the order of their Z80 addresses)
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

void sound_reset(void)
{
	z80_block(14, 2);
	interrupts_off();
	z80_block(201, 23);
	unk_E000 = 0u;
	move_block(unk_E000_ADDRESS, 0xE001u, 0x1DFFu, 1u);
	port_out(0xD0u, 0u);
	port_out(0xD1u, 1u);
	port_out(0xD2u, 2u);
	port_out(0xC0u, 0u);
	port_out(0xC1u, 0u);
	port_out(0xD8u, 0xBu);
	port_out(0xD3u, 0u);
	sound_01B4();
	z80_block(4, 1);
	interrupts_on_after_block();
}

void sound_irq(void)
{
	z80_block(121, 16);
	interrupts_off();
	port_out(0xC1u, 0u);
	port_out(0x80u, 0x27u);
	port_out(0x81u, 0x15u);
	sound_3EED();
	z80_block(17, 1);
	sound_0158();
	z80_block(17, 1);
	sound_3AF3();
	z80_block(54, 9);
	interrupts_on();
}

void sound_main_loop(void)
{
	z80_block(27, 3);
	if (unk_FFFF != 0u)
	{
		do
		{
			z80_block(31, 4);
			interrupts_off();
		} while (unk_FFFF != 0u);
		z80_block(4, 1);
		interrupts_on_after_block();
	}
	z80_block(17, 1);
	sound_0A43();
	z80_block(17, 1);
	sound_0372();
	z80_block(17, 1);
	sound_0BAD();
	z80_block(17, 1);
	sound_36AD();
	z80_block(17, 1);
	sound_36F6();
	z80_block(17, 1);
	sound_3DE6();
	z80_block(17, 1);
	sound_3E36();
	z80_block(17, 1);
	sound_41F2();
	z80_block(10, 1);
}

void sound_0158(void)
{
	uint8_t a = 0, c = 0;
	uint16_t hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31, 4);
	if ((unk_E017 & 1u) == 0u)
	{
		z80_block(26, 3);
		t1 = unk_E010;
		unk_E010 = t1 - 1u;
		if (t1 != 1u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(101, 10);
		unk_E010 = 0x8Cu;
		unk_E018 = 0xE414u;
		unk_E01A = 0x19u;
		unk_E017 |= 1u;
		return;
	}
	z80_block(31, 3);
	t2 = unk_E01A;
	unk_E01A = t2 - 1u;
	if (t2 == 1u)
	{
		z80_block(35, 4);
		unk_E017 &= ~1u;
		return;
	}
	z80_block(37, 3);
	hl = unk_E018;
	t3 = z80_read8(hl);
	z80_write8(hl, t3 - 1u);
	if (t3 != 1u)
	{
		z80_block(47, 4);
		unk_E018 = hl + 9u;
		return;
	}
	z80_block(49, 6);
	hl -= 4u;
	a = z80_read8(hl);
	c = a;
	if ((a & 0x81u) == 0u)
	{
		z80_block(28, 4);
		if ((c & 0x70u) >= 0x50u)
		{
			z80_block(15, 2);
			z80_write8(hl, z80_read8(hl) | 1u);
		}
	}
	z80_block(47, 4);
	unk_E018 = hl + 0xDu;
}

void sound_01B4(void)
{
	uint8_t a = 0, bc_2 = 0, c = 0;
	uint16_t bc = 0;

	z80_block(17, 1);
	sound_0292();
	z80_block(17, 1);
	bc = sound_01ED();
	c = (uint8_t)bc;
	z80_block(172, 15);
	unk_E758 = 0u;
	unk_E002 = 0xFFu;
	unk_E082 = 0xFFu;
	unk_E028 = 0xE410u;
	unk_E740 = 1u;
	AS16(unk_E010) = 0x8Cu;
	unk_E01C = 0xFFu;
	unk_E09C = 0xFFu;
	bc_2 = c;
	sound_0302(bc_2);
	z80_block(17, 1);
	sound_0316();
	z80_block(31, 3);
	a = 0x15u;
	bc_2 = 0x27u;
	sound_021C(bc_2, a);
	z80_block(10, 1);
}

uint16_t sound_01ED(void)
{
	uint8_t a = 0, b = 0, bc = 0, c = 0;
	uint16_t bc_2 = 0, hl = 0;

	z80_block(17, 2);
	hl = 0x234u;
	b = 0xBu;
	for (;;)
	{
		z80_block(43, 5);
		c = SOUND_ROM8(hl);
		hl++;
		a = SOUND_ROM8(hl);
		hl++;
		bc = c;
		sound_021C(bc, a);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(17, 2);
	hl = 0x24Au;
	b = 0x24u;
	for (;;)
	{
		z80_block(43, 5);
		c = SOUND_ROM8(hl);
		hl++;
		a = SOUND_ROM8(hl);
		hl++;
		bc = c;
		sound_021C(bc, a);
		z80_block(67, 6);
		bc = c;
		sound_0228(bc, a);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	bc_2 = ((uint32_t)b << 8) | c;
	return bc_2;
}

uint16_t sound_0212(uint8_t bc, uint8_t a)
{
	uint8_t a_2 = 0, b = 0;
	uint16_t bc_2 = 0;
	uint8_t t1 = 0;

	z80_block(39, 6);
	b = a;
	t1 = unk_E01E;
	a_2 = b;
	if (((t1 >> 2) & 1u) != 0u)
	{
		bc_2 = (bc & 0xFFu) | ((uint32_t)b << 8);
		sound_0228((uint8_t)bc_2, a_2);
		return bc_2;
	}
	bc_2 = (bc & 0xFFu) | ((uint32_t)b << 8);
	sound_021C((uint8_t)bc_2, a_2);
	return bc_2;
}

void sound_021C(uint8_t bc, uint8_t a)
{
	uint8_t saved_a = 0;

	z80_block(73, 10);
	saved_a = a;
	interrupts_off();
	port_out(0x80u, (uint8_t)bc);
	port_out(0x81u, saved_a);
	interrupts_on();
}

void sound_0228(uint8_t bc, uint8_t a)
{
	uint8_t saved_a = 0;

	z80_block(73, 10);
	saved_a = a;
	interrupts_off();
	port_out(0x82u, (uint8_t)bc);
	port_out(0x83u, saved_a);
	interrupts_on();
}

void sound_0292(void)
{
	uint8_t a = 0, b = 0, bc = 0, c = 0;
	uint16_t hl = 0;

	z80_block(55, 6);
	z80_write8(PCM_C009, 3u);
	port_out(0xB0u, 0x1Au);
	hl = 0x2E4u;
	b = 0x1Cu;
	for (;;)
	{
		z80_block(13, 2);
		c = SOUND_ROM8(hl);
		hl++;
		do
		{
			z80_block(27, 3);
		} while (z80_read8(PCM_C000) & 1u);
		z80_block(48, 5);
		z80_write8(PCM_C001, c);
		a = 0x80u;
		bc = 0u;
		sound_02CA(bc, a);
		z80_block(31, 3);
		a = 0u;
		bc = 4u;
		sound_02CA(bc, a);
		z80_block(31, 3);
		a = 0xFEu;
		bc = 5u;
		sound_02CA(bc, a);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(30, 3);
	z80_write8(PCM_C009, 3u);
}

void sound_02CA(uint8_t bc, uint8_t a)
{
	uint8_t saved_a = 0;

	z80_block(11, 1);
	saved_a = a;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(55, 4);
	z80_write8(PCM_C002, bc);
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(33, 3);
	z80_write8(PCM_C000, saved_a);
}

void sound_0302(uint32_t bc)
{
	uint8_t b = 0;
	uint16_t de = 0, hl = 0;
	uint8_t saved_b = 0;

	z80_block(27, 3);
	hl = 0xE411u;
	de = 0x2E4u;
	b = 0x1Eu;
	for (;;)
	{
		z80_block(70, 8);
		saved_b = b;
		z80_write8(hl, SOUND_ROM8(de));
		de++;
		hl += 9u;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

void sound_0316(void)
{
	uint8_t a = 0, de = 0;
	uint16_t bc = 0, hl = 0, iy = 0;
	uint16_t saved_hl = 0;

	z80_block(34, 3);
	a = 0u;
	hl = 0x1847u;
	hl = sound_0366(hl, a);
	z80_block(178, 21);
	saved_hl = hl;
	move_block(hl, 0xE300u, 0x50u, 1u);
	move_block((uint16_t)(hl + 0x51u), 0xE350u, 0x32u, 1u);
	hl = saved_hl;
	move_block(hl, 0xE382u, 0x50u, 1u);
	hl += 0x51u;
	move_block(hl, 0xE3D2u, 0x32u, 1u);
	hl += 0x32u;
	iy = 0xE300u;
	bc = 0x1A00u;
	for (;;)
	{
		z80_block(56, 6);
		hl = (hl & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(iy + 2u)) << 8);
		a = 0xFFu;
		if ((z80_read8(iy) & 1u) == 0u)
		{
			z80_block(13, 1);
			a = unk_E01C;
		}
		z80_block(21, 2);
		de = a;
		hl = sound_0B77(de, hl);
		z80_block(18, 3);
		a = (uint8_t)(hl >> 8);
		if (a != 0u)
		{
			z80_block(4, 1);
			a++;
		}
		z80_block(52, 6);
		z80_write8((uint16_t)(iy + 8u), a);
		iy += 0xAu;
		bc = (uint32_t)(uint8_t)((bc >> 8) - 1u) << 8;
		if (bc >> 8 == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

uint16_t sound_0366(uint16_t hl, uint8_t a)
{
	uint16_t bc = 0, hl_2 = 0;

	z80_block(88, 11);
	bc = a;
	hl = hl + bc + bc;
	hl_2 = z80_read16(hl);
	return hl_2;
}

void sound_0372(void)
{
	uint8_t a = 0, b = 0;
	uint16_t bc = 0, hl = 0, ix = 0;
	uint8_t saved_b = 0, saved_bc = 0;
	uint16_t saved_hl = 0;

	z80_block(26, 4);
	hl = unk_E759_ADDRESS;
	if (unk_E759 == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	unk_E759--;
	sound_0A39(&bc, &a);
	z80_block(18, 3);
	if (a >> 7 == 0u)
	{
		z80_block(13, 1);
		a = unk_E008;
	}
	else
	{
		z80_block(23, 2);
		unk_E008 = a;
	}
	z80_block(49, 7);
	hl = ((a & 0xFu) << 8) | unk_E002;
	b = 0xDu;
	ix = 0xE300u;
	for (;;)
	{
		z80_block(30, 3);
		if (z80_read8(ix) >> 7)
		{
			z80_block(33, 4);
			if (hl >> 8 == z80_read8((uint16_t)(ix + 1u)))
			{
				z80_block(39, 3);
				saved_b = b;
				saved_bc = (uint8_t)bc;
				saved_hl = hl;
				sound_03D3((uint8_t)bc, ix);
				z80_block(37, 4);
				hl = saved_hl;
				bc = saved_bc;
				b = saved_b;
				unk_E002 = (uint8_t)hl;
			}
		}
		z80_block(33, 4);
		ix += 0xAu;
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	sound_03B7((uint8_t)bc);
}

void sound_03B7(uint8_t bc)
{
	uint8_t a = 0;
	uint16_t hl = 0;

	z80_block(28, 4);
	hl = unk_E002_ADDRESS;
	if ((uint8_t)bc >> 7 != 0u)
	{
		z80_block(22, 3);
		a = bc & 0xF0u;
		unk_E002++;
	}
	else
	{
		z80_block(30, 3);
		a = unk_E008 & ~0xFu;
	}
	z80_block(12, 2);
	if (a == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (a == 0xD0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(21, 2);
	unk_E002++;
}

void sound_03D3(uint8_t bc, uint16_t ix)
{
	bool L_000461 = false, L_0004B7 = false, L_0004FD = false, L_0005FC = false;
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, bc_3 = 0, c = 0, de_2 = 0, hl_2 = 0, ignored8 = 0;
	uint16_t bc_2 = 0, de = 0, hl = 0, ignored16 = 0, iy = 0;
	uint8_t saved_a = 0, saved_b = 0;
	uint16_t stack_2 = 0, saved_hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	c = (uint8_t)bc;
	z80_block(37, 4);
	a = unk_E008 & ~0xFu;
	do
	{
		if (L_0005FC || a == 0x90u)
		{
			if (!L_0005FC)
			{
				z80_block(18, 3);
				if (c >> 7)
				{
					z80_block(17, 1);
					sound_0A39(&bc_2, &ignored8);
					c = (uint8_t)bc_2;
				}
				z80_block(109, 12);
				b = c;
				a = z80_read8((uint16_t)(ix + 4u)) + c;
				unk_E02D = a;
				unk_E012 = rom_1F62[a];
				sound_0A39(&bc_2, &a);
				c = (uint8_t)bc_2;
				z80_block(14, 2);
			}
			if (L_0005FC || a == 0u)
			{
				z80_block(31, 4);
				a = z80_read8((uint16_t)(ix + 3u));
				if (a >= 0xF0u)
				{
					z80_cycles(6);
					return;
				}
				z80_block(27, 2);
				hl = 0x1BC2u;
				hl = sound_0366(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(51, 6);
					iy = 0xE4E8u;
					de = 9u;
					b = 6u;
					a = unk_E008 & 0xFu;
					for (;;)
					{
						z80_block(29, 3);
						if (a == z80_read8((uint16_t)(iy + 3u)))
						{
							z80_block(46, 5);
							hl_2 = a;
							if (unk_E012 == z80_read8((uint16_t)(iy + 2u)))
							{
								z80_block(37, 5);
								a = z80_read8(iy);
								if (((a >> 1) & 1u) == 0u)
								{
									z80_block(18, 3);
									if (((a >> 3) & 1u) == 0u)
									{
										z80_block(17, 2);
										if ((a & 0x7Fu) != 0u)
											break;
									}
								}
							}
							z80_block(4, 1);
							a = (uint8_t)hl_2;
						}
						z80_block(23, 3);
						iy += de;
						b--;
						if (b == 0u)
						{
							z80_block(10, 1);
							return;
						}
						z80_cycles(5);
					}
					z80_block(65, 9);
					if (unk_E060[hl_2] >> 7)
					{
						z80_block(33, 3);
						z80_write8(iy, z80_read8(iy) | 2u);
						return;
					}
					z80_block(85, 10);
					z80_write8(iy, 0u);
					z80_write8((uint16_t)(iy + 3u), 0u);
					a = z80_read8((uint16_t)(iy + 1u)) & 7u;
					bc_3 = 0x28u;
					sound_021C(bc_3, a);
					return;
				}
				z80_block(51, 6);
				iy = 0xE410u;
				de = 9u;
				b = 0x18u;
				a = unk_E008 & 0xFu;
				for (;;)
				{
					z80_block(29, 3);
					if (a == z80_read8((uint16_t)(iy + 3u)))
					{
						z80_block(46, 5);
						hl_2 = a;
						if (unk_E012 == z80_read8((uint16_t)(iy + 2u)))
						{
							z80_block(37, 5);
							a = z80_read8(iy);
							if (((a >> 1) & 1u) == 0u)
							{
								z80_block(18, 3);
								if (((a >> 3) & 1u) == 0u)
								{
									z80_block(17, 2);
									if ((a & 0x7Fu) != 0u)
										break;
								}
							}
						}
						z80_block(4, 1);
						a = (uint8_t)hl_2;
					}
					z80_block(23, 3);
					iy += de;
					b--;
					if (b == 0u)
					{
						z80_block(10, 1);
						return;
					}
					z80_cycles(5);
				}
				z80_block(65, 9);
				if (unk_E060[hl_2] >> 7 == 0u)
				{
					z80_block(33, 3);
					z80_write8(iy, z80_read8(iy) | 1u);
					return;
				}
				z80_block(33, 3);
				z80_write8(iy, z80_read8(iy) | 2u);
				return;
			}
			z80_block(88, 10);
			de = a;
			unk_E013 = SOUND_ROM8(0x2016u + de);
			a = z80_read8((uint16_t)(ix + 3u));
			if (a < 0xF0u)
			{
				z80_block(27, 2);
				hl = 0x1BC2u;
				hl = sound_0366(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(59, 7);
					unk_E00D = z80_read8((uint16_t)(hl + 2u));
					hl = 0xE4E8u;
					de = 9u;
					b = 6u;
					for (;;)
					{
						z80_block(21, 3);
						if (z80_read8(hl) == 0u)
							break;
						z80_block(19, 2);
						hl += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(182, 22);
					z80_write8(hl, 0x40u);
					hl++;
					unk_E01E = z80_read8(hl) & 7u;
					hl++;
					a = unk_E012;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = unk_E008 & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					a = unk_E013;
					z80_write8(hl, a);
					hl = (hl & 0xFFu) | ((uint32_t)a << 8);
					de_2 = z80_read8((uint16_t)(ix + 8u));
					hl = sound_0B77(de_2, hl);
					z80_block(112, 15);
					unk_E013 = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
					de = b;
					unk_E01B = (z80_read8((uint16_t)(unk_E060_ADDRESS + de)) & 0xFu) | 0x40u;
					hl = 0xE040u;
					bc_3 = c;
					sound_12E0(bc_3, de, hl);
					return;
				}
				for (;;)
				{
					z80_block(44, 6);
					hl++;
					if (unk_E02D <= z80_read8(hl))
						break;
					z80_block(28, 4);
					hl += 3u;
				}
				z80_block(78, 9);
				hl++;
				unk_E00D = z80_read8(hl);
				hl++;
				unk_E02B = z80_read8(hl);
				unk_E02C = z80_read8((uint16_t)(hl + 1u));
				L_0004B7 = true;
			}
			else
			{
				z80_block(34, 3);
				a += 0x10u;
				hl = 0x2DA5u;
				hl = sound_0366(hl, a);
				z80_block(112, 14);
				de = b;
				hl = hl + de + de + de;
				unk_E01D = z80_read8(hl);
				hl++;
				c = z80_read8(hl);
				a = z80_read8((uint16_t)(hl + 1u));
				unk_E00D = a;
				if (a == 0xFFu)
				{
					z80_cycles(6);
					return;
				}
				z80_block(37, 4);
				L_0004B7 = (unk_E008 & 0xFu) != 0xFu;
				if (!L_0004B7)
				{
					z80_block(21, 3);
					a = b;
					if (a == 0x2Cu)
						L_000461 = true;
					else
					{
						z80_block(17, 2);
						L_0004B7 = a != 0x2Au;
						if (!L_0004B7)
							L_000461 = true;
					}
					if (!L_0004B7)
					{
						if (L_000461)
						{
							L_000461 = false;
							z80_block(27, 3);
							hl = 0xE412u;
							de = 9u;
							b = 0x18u;
							for (;;)
							{
								z80_block(24, 3);
								if (z80_read8(hl) == 0xCAu)
								{
									z80_block(36, 5);
									hl++;
									a = z80_read8(hl);
									hl--;
									if (a == 0xFu)
									{
										z80_block(34, 5);
										hl = hl - 1u - 1u;
										if ((z80_read8(hl) >> 6) & 1u)
										{
											z80_block(22, 3);
											if (((z80_read8(hl) >> 3) & 1u) == 0u)
											{
												z80_block(15, 2);
												z80_write8(hl, z80_read8(hl) | 1u);
											}
										}
										z80_block(12, 2);
										hl += 2u;
									}
								}
								z80_block(19, 2);
								hl += de;
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							L_0004B7 = true;
						}
					}
				}
			}
			if (L_0004B7)
			{
				z80_block(33, 3);
				hl = unk_E028;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(21, 3);
					if ((uint8_t)hl == 0xE8u)
					{
						z80_cycles(7);
						hl = sound_0BA6();
					}
					z80_block(21, 3);
					a = 0u;
					if (0u == z80_read8(hl))
					{
						z80_block(58, 5);
						stack_2 = hl;
						unk_E028 = hl + de;
						hl = stack_2;
						L_0004FD = true;
					}
					if (L_0004FD)
						break;
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				if (!L_0004FD)
				{
					z80_block(10, 1);
					z80_block(17, 2);
					hl = 0xE416u;
					b = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (a == z80_read8(hl))
							break;
						z80_block(19, 2);
						hl += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(42, 5);
					z80_write8(hl, 0x18u);
					stack_2 = hl;
					hl = 0xE416u;
					b = 0x18u;
					for (;;)
					{
						z80_block(30, 3);
						z80_write8(hl, z80_read8(hl) - 1u);
						hl += de;
						b--;
						if (b == 0u)
							break;
						z80_cycles(5);
					}
					z80_block(52, 5);
					unk_E01F--;
					hl = stack_2 - 6u;
				}
				z80_block(46, 5);
				z80_write8(hl, 0x40u);
				if (z80_read8((uint16_t)(ix + 3u)) < 0xF0u)
				{
					do
					{
						z80_block(27, 3);
					} while (z80_read8(PCM_C000) & 1u);
					z80_block(479, 64);
					hl++;
					z80_write8(PCM_C001, z80_read8(hl));
					hl++;
					a = unk_E012;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = unk_E008 & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					z80_write8(hl, unk_E013);
					hl++;
					a = unk_E01F;
					z80_write8(hl, a);
					unk_E01F = a + 1u;
					hl++;
					z80_write8(hl, unk_E02B);
					z80_write8((uint16_t)(hl + 1u), unk_E02C);
					de = (uint8_t)(b * 2u);
					hl = 0xE040u + de;
					de = unk_E02B;
					stack_2 = z80_read16(hl) + de;
					hl = 0x20AEu + (uint8_t)((c & 0xFu) * 2u);
					hl = SOUND_ROM16(hl) + stack_2 + stack_2;
					hl = z80_read16(hl);
					hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((c & ~0xFu) + (hl >> 8)) << 8);
					a = unk_E02C + (hl >> 8);
				}
				else
				{
					z80_block(15, 2);
					z80_write8(hl, z80_read8(hl) | 0x10u);
					do
					{
						z80_block(27, 3);
					} while (z80_read8(PCM_C000) & 1u);
					z80_block(171, 21);
					hl++;
					z80_write8(PCM_C001, z80_read8(hl));
					hl++;
					z80_write8(hl, unk_E012);
					hl++;
					z80_write8(hl, unk_E008 & 0xFu);
					hl++;
					z80_write8(hl, c);
					hl++;
					z80_write8(hl, unk_E013);
					hl++;
					if ((unk_E008 & 0xFu) == 0xFu)
					{
						z80_block(37, 4);
						a = unk_E01F;
						z80_write8(hl, a);
						unk_E01F = a + 1u;
					}
					else
					{
						z80_block(24, 3);
						z80_write8(hl, 0xFFu);
					}
					z80_block(30, 3);
					a = unk_E01D;
					hl = 0u;
				}
				z80_block(24, 2);
				bc_3 = 3u;
				sound_02CA(bc_3, a);
				z80_block(28, 3);
				a = (uint8_t)hl;
				bc_3 = 2u;
				sound_02CA(bc_3, a);
				z80_block(37, 3);
				a = unk_E00D;
				bc_3 = 1u;
				sound_02CA(bc_3, a);
				z80_block(53, 5);
				hl = (hl & 0xFFu) | ((uint32_t)unk_E013 << 8);
				de_2 = z80_read8((uint16_t)(ix + 8u));
				hl = sound_0B77(de_2, hl);
				z80_block(39, 5);
				a = ~(hl >> 8) | 1u;
				bc_3 = 5u;
				sound_02CA(bc_3, a);
				z80_block(31, 3);
				a = 0x70u;
				bc_3 = 0u;
				sound_02CA(bc_3, a);
				z80_block(24, 3);
				a = 0x80u;
				bc_3 = 4u;
				sound_02CA(bc_3, a);
				return;
			}
		}
		z80_block(17, 2);
		if (a == 0x80u)
		{
			z80_block(18, 3);
			if (c >> 7)
			{
				z80_block(17, 1);
				sound_0A39(&bc_2, &ignored8);
				c = (uint8_t)bc_2;
			}
			z80_block(96, 11);
			unk_E012 = rom_1F62[(uint8_t)(z80_read8((uint16_t)(ix + 4u)) + c)];
			unk_E002++;
			L_0005FC = true;
		}
	} while (L_0005FC);
	z80_block(17, 2);
	if (a == 0xE0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_0A39(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(21, 2);
		b = c;
		sound_0A39(&ignored16, &a);
		z80_block(26, 5);
		carry = a & 1u;
		c = a >> 1;
		a = b;
		if (carry)
		{
			z80_block(7, 1);
			a |= 0x80u;
		}
		z80_block(78, 18);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		if ((c & 1u) == 0u)
		{
			z80_block(18, 3);
			t1 = ~a + 1u;
			a = t1;
			if (t1 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(57, 6);
		de = a;
		a = z80_read8((uint16_t)(ix + 9u));
		hl = 0x2546u;
		hl = sound_0366(hl, a);
		z80_block(36, 5);
		a = z80_read8((uint16_t)(hl + de));
		carry = c & 1u;
		c = c >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			c--;
			t2 = ~a + 1u;
			a = t2;
			if (t2 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(145, 19);
		b = c;
		c = a;
		unk_E014 = ((uint32_t)b << 8) | c;
		hl = 0xE040u + (uint8_t)((unk_E008 & 0xFu) * 2u);
		z80_write8(hl, c);
		z80_write8((uint16_t)(hl + 1u), b);
		iy = 0xE410u;
		b = 0x1Eu;
		a = unk_E008 & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (a == z80_read8((uint16_t)(iy + 3u)))
			{
				z80_block(48, 6);
				saved_a = a;
				a = z80_read8(iy);
				if (((a >> 3) & 1u) == 0u)
				{
					z80_block(17, 2);
					if ((a & 0x7Fu) != 0u)
					{
						z80_cycles(7);
						sound_06E3(iy);
					}
				}
				z80_block(10, 1);
				a = saved_a;
			}
			z80_block(33, 4);
			iy += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xB0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_0A39(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(21, 2);
		b = c;
		sound_0A39(&bc_2, &ignored8);
		c = (uint8_t)bc_2;
		z80_block(21, 3);
		a = b;
		carry = a == 0u;
		if (a == 1u)
		{
			z80_block(123, 19);
			t3 = c;
			a = (t3 >> 1) | (carry << 7);
			carry = t3 & 1u;
			t4 = a;
			a = (t4 >> 1) | (carry << 7);
			carry = t4 & 1u;
			t5 = a;
			a = (t5 >> 1) | (carry << 7);
			carry = t5 & 1u;
			a = ((a >> 1) | (carry << 7)) & 7u;
			c = a;
			unk_E01B = a;
			hl = unk_E060_ADDRESS + (unk_E008 & 0xFu);
			a = z80_read8(hl);
			b = a;
			if ((a & 0x7Fu) == c)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(hl, (b & 0x80u) | c);
			hl = 0xE413u;
			b = 0x1Eu;
			a = unk_E008 & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (a == z80_read8(hl))
				{
					z80_block(47, 8);
					de = (uint32_t)a << 8;
					hl = hl - 1u - 1u - 1u;
					a = z80_read8(hl);
					if (((a >> 3) & 1u) == 0u)
					{
						z80_block(17, 2);
						if ((a & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							bc_2 = (uint32_t)b << 8;
							sound_07BC(ix, &bc_2, &hl);
							b = (uint8_t)(bc_2 >> 8);
						}
					}
					z80_block(22, 4);
					hl += 3u;
					a = (uint8_t)(de >> 8);
				}
				z80_block(29, 3);
				hl += 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 7u)
		{
			z80_block(18, 3);
			t6 = c * 2u;
			a = t6;
			if (t6 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(60, 7);
			z80_write8((uint16_t)(ix + 2u), a);
			hl = (uint32_t)a << 8;
			a = 0xFFu;
			if ((z80_read8(ix) & 1u) == 0u)
			{
				z80_block(13, 1);
				a = unk_E01C;
			}
			z80_block(21, 2);
			de_2 = a;
			hl = sound_0B77(de_2, hl);
			z80_block(18, 3);
			a = (uint8_t)(hl >> 8);
			if (a != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(36, 4);
			z80_write8((uint16_t)(ix + 8u), a);
			hl = 0xE410u;
			b = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				stack_2 = hl;
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((z80_read8(hl) >> 3) & 1u) == 0u)
					{
						z80_block(55, 7);
						hl += 3u;
						if ((unk_E008 & 0xFu) == z80_read8(hl))
						{
							z80_block(37, 6);
							hl = hl - 1u - 1u;
							c = z80_read8(hl);
							if (c >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(PCM_C000) & 1u);
								z80_block(81, 8);
								z80_write8(PCM_C001, c);
								hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
								de_2 = z80_read8((uint16_t)(ix + 8u));
								hl = sound_0B77(de_2, hl);
								z80_block(39, 5);
								a = ~(hl >> 8) & 0xFEu;
								bc_3 = 5u;
								sound_02CA(bc_3, a);
							}
							else
							{
								z80_block(17, 1);
								bc_3 = c;
								sound_14A8(bc_3, hl, ix);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				hl = stack_2 + 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x10u)
		{
			z80_block(18, 3);
			t7 = c * 2u;
			a = t7;
			if (t7 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(34, 4);
			unk_E01C = a;
			iy = 0xE300u;
			b = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_b = b;
				hl = (hl & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(iy + 2u)) << 8);
				a = 0xFFu;
				if ((z80_read8(iy) & 1u) == 0u)
				{
					z80_block(13, 1);
					a = unk_E01C;
				}
				z80_block(21, 2);
				de_2 = a;
				hl = sound_0B77(de_2, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(36, 4);
				z80_write8((uint16_t)(iy + 8u), a);
				hl = 0xE413u;
				b = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_hl = hl;
					if (z80_read8((uint16_t)(iy + 1u)) == z80_read8(hl))
					{
						z80_block(40, 6);
						hl = hl - 1u - 1u - 1u;
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((z80_read8(hl) >> 3) & 1u) == 0u)
							{
								z80_block(31, 5);
								hl++;
								c = z80_read8(hl);
								if (c >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(PCM_C000) & 1u);
									z80_block(81, 8);
									z80_write8(PCM_C001, c);
									hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
									de_2 = z80_read8((uint16_t)(iy + 8u));
									hl = sound_0B77(de_2, hl);
									z80_block(39, 5);
									a = ~(hl >> 8) & 0xFEu;
									bc_3 = 5u;
									sound_02CA(bc_3, a);
								}
								else
								{
									z80_block(17, 1);
									bc_3 = c;
									sound_145A(bc_3, hl, iy);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					hl = saved_hl + 9u;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				iy += 0xAu;
				b = saved_b - 1u;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x40u)
		{
			z80_block(21, 3);
			if (c >= 0x40u)
			{
				z80_block(77, 9);
				hl = unk_E060_ADDRESS + (unk_E008 & 0xFu);
				z80_write8(hl, z80_read8(hl) | 0x80u);
				return;
			}
			z80_block(51, 6);
			iy = 0xE410u;
			de = 9u;
			b = 0x1Eu;
			a = unk_E008 & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (a == z80_read8((uint16_t)(iy + 3u)))
				{
					z80_block(41, 6);
					hl_2 = a;
					a = z80_read8(iy);
					if ((a >> 1) & 1u)
					{
						z80_block(18, 3);
						if (((a >> 3) & 1u) == 0u)
						{
							z80_block(37, 5);
							if (z80_read8((uint16_t)(iy + 1u)) >> 7 == 0u)
							{
								z80_block(23, 2);
								z80_write8(iy, z80_read8(iy) | 1u);
							}
							else
							{
								z80_block(17, 1);
								sound_1449(iy);
								z80_block(10, 1);
							}
						}
					}
					z80_block(4, 1);
					a = (uint8_t)hl_2;
				}
				z80_block(23, 3);
				iy += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			hl = unk_E060_ADDRESS + (unk_E008 & 0xFu);
			z80_write8(hl, z80_read8(hl) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (a == 0x7Bu)
		{
			z80_block(47, 5);
			a = unk_E008 & 0xFu;
			hl = 0xE410u;
			de = 9u;
			b = 0x18u;
			for (;;)
			{
				z80_block(22, 3);
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((z80_read8(hl) >> 3) & 1u) == 0u)
					{
						z80_block(35, 5);
						hl += 3u;
						if (a != z80_read8(hl))
						{
							z80_block(18, 3);
							hl = hl - 1u - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							hl = hl - 1u - 1u - 1u;
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
				}
				z80_block(19, 2);
				hl += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x50u)
		{
			z80_block(21, 3);
			if (c == 0x7Eu)
			{
				z80_block(27, 3);
				hl = 0xE410u;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if (((z80_read8(hl) >> 3) & 1u) == 0u)
					{
						z80_block(22, 3);
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((z80_read8(hl) >> 4) & 1u) == 0u)
							{
								z80_block(15, 2);
								z80_write8(hl, z80_read8(hl) | 1u);
							}
							else
							{
								z80_block(60, 9);
								hl += 3u;
								a = z80_read8(hl);
								hl = hl - 1u - 1u - 1u;
								if (a == 0xFu)
								{
									z80_block(15, 2);
									z80_write8(hl, z80_read8(hl) | 1u);
								}
							}
						}
					}
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				hl = 0xE410u;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if (((z80_read8(hl) >> 3) & 1u) == 0u)
					{
						z80_block(22, 3);
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(15, 2);
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			hl = unk_E060_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			hl = 0xE040u;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x20u)
		{
			z80_block(31, 3);
			a = c;
			hl = 0x1847u;
			hl = sound_0366(hl, a);
			z80_block(112, 13);
			move_block(hl, 0xE300u, 0x50u, 1u);
			hl += 0x50u;
			a = (unk_E02A & 0x38u) | z80_read8(hl);
			port_out(0xB0u, a);
			unk_E02A = a;
			iy = 0xE300u;
			b = 8u;
			for (;;)
			{
				z80_block(56, 6);
				hl = (hl & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(iy + 2u)) << 8);
				a = 0xFFu;
				if ((z80_read8(iy) & 1u) == 0u)
				{
					z80_block(13, 1);
					a = unk_E01C;
				}
				z80_block(21, 2);
				de_2 = a;
				hl = sound_0B77(de_2, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(52, 6);
				z80_write8((uint16_t)(iy + 8u), a);
				iy += 0xAu;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		c = rom_1F62[c];
		hl = 0xE410u;
		b = 0x18u;
		for (;;)
		{
			z80_block(22, 3);
			if ((z80_read8(hl) >> 6) & 1u)
			{
				z80_block(22, 3);
				if (((z80_read8(hl) >> 3) & 1u) == 0u)
				{
					z80_block(33, 5);
					hl += 2u;
					if (c != z80_read8(hl))
					{
						z80_block(12, 2);
						hl = hl - 1u - 1u;
					}
					else
					{
						z80_block(43, 5);
						hl++;
						if ((unk_E008 & 0xFu) != z80_read8(hl))
						{
							z80_block(6, 1);
							hl--;
							z80_block(12, 2);
							hl = hl - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							hl = hl - 1u - 1u - 1u;
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
				}
			}
			z80_block(29, 3);
			hl += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xC0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_0A39(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(29, 3);
		z80_write8((uint16_t)(ix + 3u), c);
		return;
	}
	z80_block(17, 2);
	if (a != 0xD0u)
	{
		z80_block(17, 2);
		if (a != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	bc_3 = c;
	sound_03B7(bc_3);
}

void sound_06E3(uint16_t iy)
{
	uint8_t a = 0, bc = 0, c = 0;
	uint16_t de = 0, hl = 0;

	z80_block(37, 5);
	a = z80_read8((uint16_t)(iy + 1u));
	if (a >> 7 != 0u)
	{
		sound_13D8(iy, a);
		return;
	}
	z80_block(4, 1);
	c = a;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(291, 38);
	z80_write8(PCM_C001, c);
	a = z80_read8((uint16_t)(iy + 2u));
	hl = 0x20AEu + (uint8_t)((a & 0xFu) * 2u);
	de = unk_E014;
	hl = SOUND_ROM16(hl) + de + de;
	de = z80_read8((uint16_t)(iy + 7u));
	hl = hl + de + de;
	hl = z80_read16(hl);
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((a & ~0xFu) + (hl >> 8)) << 8);
	a = z80_read8((uint16_t)(iy + 8u)) + (hl >> 8);
	bc = 3u;
	sound_02CA(bc, a);
	z80_block(21, 3);
	a = (uint8_t)hl;
	bc = 2u;
	sound_02CA(bc, a);
}

void sound_07BC(uint16_t ix, uint16_t *p_bc, uint16_t *p_hl)
{
	uint16_t bc = (uint16_t)*p_bc;
	uint16_t hl = (uint16_t)*p_hl;
	uint8_t a = 0, c = 0;
	uint16_t bc_2 = 0;

	z80_block(31, 5);
	hl++;
	c = z80_read8(hl);
	if (c >> 7 != 0u)
	{
		z80_block(79, 9);
		a = (uint8_t)(((bc & ~0xFFu) | c) >> 8);
		unk_E01E = a;
		c = (a & 3u) - 0x4Cu;
		a = (unk_E01B & 0x3Fu) | 0x40u;
		bc_2 = c;
		bc_2 = sound_0212((uint8_t)bc_2, a);
		z80_block(16, 2);
		hl--;
		bc_2 = (bc_2 & ~0xFFu) | c;
		*p_bc = bc_2;
		*p_hl = hl;
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(73, 7);
	z80_write8(PCM_C001, c);
	c = 6u;
	a = unk_E01B | z80_read8((uint16_t)(ix + 6u));
	bc_2 = (bc & ~0xFFu) | 6u;
	sound_02CA((uint8_t)bc_2, a);
	z80_block(16, 2);
	hl--;
	bc_2 = (bc_2 & ~0xFFu) | c;
	*p_bc = bc_2;
	*p_hl = hl;
}

void sound_0A39(uint16_t *p_bc, uint8_t *p_a)
{
	uint8_t a = 0;
	uint16_t bc = 0;

	z80_block(56, 7);
	unk_E002++;
	a = z80_read8(unk_E002 | 0xE100u);
	bc = a;
	*p_bc = (*p_bc & 0xFF00u) | bc;
	*p_a = a;
}

void sound_0A43(void)
{
	bool L_000A7D = false;
	uint8_t a = 0, b = 0, bc = 0, c = 0;
	uint16_t de = 0, hl = 0;
	uint8_t stack_5 = 0, saved_b = 0;
	uint16_t saved_hl = 0;

	z80_block(27, 3);
	hl = 0xE410u;
	de = 9u;
	b = 0x18u;
	for (;;)
	{
		z80_block(22, 3);
		if (z80_read8(hl) & 1u)
		{
			z80_block(51, 6);
			saved_b = b;
			saved_hl = hl;
			stack_5 = z80_read8(hl);
			z80_write8(hl, 0u);
			do
			{
				z80_block(27, 3);
			} while (z80_read8(PCM_C000) & 1u);
			z80_block(54, 6);
			hl++;
			z80_write8(PCM_C001, z80_read8(hl));
			a = 0u;
			bc = 4u;
			sound_02CA(bc, a);
			z80_block(79, 12);
			hl += 2u;
			b = z80_read8(hl);
			z80_write8(hl, a);
			hl += 3u;
			c = stack_5;
			L_000A7D = b == 0xFu;
			if (!L_000A7D)
			{
				z80_block(22, 4);
				L_000A7D = ((c >> 4) & 1u) == 0u;
			}
			if (L_000A7D)
			{
				z80_block(55, 6);
				a = z80_read8(hl);
				unk_E01F--;
				hl = 0xE416u;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(17, 2);
					if (a < z80_read8(hl))
					{
						z80_block(11, 1);
						z80_write8(hl, z80_read8(hl) - 1u);
					}
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(30, 3);
			hl = saved_hl;
			b = saved_b;
			de = 9u;
		}
		z80_block(19, 2);
		hl += de;
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

uint16_t sound_0B77(uint16_t de, uint16_t hl)
{
	uint32_t carry = 0;

	z80_block(35, 4);
	de &= 0xFFu;
	hl &= ~0xFFu;
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(21, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry)
	{
		z80_block(11, 1);
		hl += de;
	}
	z80_block(16, 2);
	carry = (int16_t)hl < 0;
	hl = hl * 2u;
	if (carry == 0u)
	{
		z80_cycles(6);
		return hl;
	}
	z80_block(21, 2);
	hl += de;
	return hl;
}

uint16_t sound_0BA6(void)
{
	uint16_t hl = 0;

	z80_block(36, 3);
	hl = 0xE410u;
	unk_E028 = 0xE410u;
	return hl;
}

void sound_0BAD(void)
{
	uint8_t a = 0, b = 0;
	uint16_t bc = 0, hl = 0, ix = 0;
	uint8_t saved_b = 0, saved_bc = 0;
	uint16_t saved_hl = 0;

	z80_block(26, 4);
	hl = unk_E859_ADDRESS;
	if (unk_E859 == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	unk_E859--;
	sound_127B(&bc, &a);
	z80_block(18, 3);
	if (a >> 7 == 0u)
	{
		z80_block(13, 1);
		a = unk_E088;
	}
	else
	{
		z80_block(23, 2);
		unk_E088 = a;
	}
	z80_block(49, 7);
	hl = ((a & 0xFu) << 8) | unk_E082;
	b = 0xDu;
	ix = 0xE382u;
	for (;;)
	{
		z80_block(30, 3);
		if (z80_read8(ix) >> 7)
		{
			z80_block(33, 4);
			if (hl >> 8 == z80_read8((uint16_t)(ix + 1u)))
			{
				z80_block(39, 3);
				saved_b = b;
				saved_bc = (uint8_t)bc;
				saved_hl = hl;
				sound_0C0E((uint8_t)bc, ix);
				z80_block(37, 4);
				hl = saved_hl;
				bc = saved_bc;
				b = saved_b;
				unk_E082 = (uint8_t)hl;
			}
		}
		z80_block(33, 4);
		ix += 0xAu;
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	sound_0BF2((uint8_t)bc);
}

void sound_0BF2(uint8_t bc)
{
	uint8_t a = 0;
	uint16_t hl = 0;

	z80_block(28, 4);
	hl = unk_E082_ADDRESS;
	if ((uint8_t)bc >> 7 != 0u)
	{
		z80_block(22, 3);
		a = bc & 0xF0u;
		unk_E082++;
	}
	else
	{
		z80_block(30, 3);
		a = unk_E088 & ~0xFu;
	}
	z80_block(12, 2);
	if (a == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (a == 0xD0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(21, 2);
	unk_E082++;
}

void sound_0C0E(uint8_t bc, uint16_t ix)
{
	bool L_000C9C = false, L_000CF2 = false, L_000D38 = false, L_000E39 = false;
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, bc_3 = 0, c = 0, de_2 = 0, hl_2 = 0, ignored8 = 0;
	uint16_t bc_2 = 0, de = 0, hl = 0, ignored16 = 0, iy = 0;
	uint8_t saved_a = 0, saved_b = 0;
	uint16_t stack_2 = 0, saved_hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	c = (uint8_t)bc;
	z80_block(37, 4);
	a = unk_E088 & ~0xFu;
	do
	{
		if (L_000E39 || a == 0x90u)
		{
			if (!L_000E39)
			{
				z80_block(18, 3);
				if (c >> 7)
				{
					z80_block(17, 1);
					sound_127B(&bc_2, &ignored8);
					c = (uint8_t)bc_2;
				}
				z80_block(109, 12);
				b = c;
				a = z80_read8((uint16_t)(ix + 4u)) + c;
				unk_E02D = a;
				unk_E012 = rom_1F62[a];
				sound_127B(&bc_2, &a);
				c = (uint8_t)bc_2;
				z80_block(14, 2);
			}
			if (L_000E39 || a == 0u)
			{
				z80_block(31, 4);
				a = z80_read8((uint16_t)(ix + 3u));
				if (a >= 0xF0u)
				{
					z80_cycles(6);
					return;
				}
				z80_block(27, 2);
				hl = 0x1BC2u;
				hl = sound_0366(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(51, 6);
					iy = 0xE4E8u;
					de = 9u;
					b = 6u;
					a = unk_E088 & 0xFu;
					for (;;)
					{
						z80_block(29, 3);
						if (a == z80_read8((uint16_t)(iy + 3u)))
						{
							z80_block(46, 5);
							hl_2 = a;
							if (unk_E012 == z80_read8((uint16_t)(iy + 2u)))
							{
								z80_block(37, 5);
								a = z80_read8(iy);
								if (((a >> 1) & 1u) == 0u)
								{
									z80_block(18, 3);
									if ((a >> 3) & 1u)
									{
										z80_block(17, 2);
										if ((a & 0x7Fu) != 0u)
											break;
									}
								}
							}
							z80_block(4, 1);
							a = (uint8_t)hl_2;
						}
						z80_block(23, 3);
						iy += de;
						b--;
						if (b == 0u)
						{
							z80_block(10, 1);
							return;
						}
						z80_cycles(5);
					}
					z80_block(65, 9);
					if (unk_E0E0[hl_2] >> 7)
					{
						z80_block(33, 3);
						z80_write8(iy, z80_read8(iy) | 2u);
						return;
					}
					z80_block(85, 10);
					z80_write8(iy, 0u);
					z80_write8((uint16_t)(iy + 3u), 0u);
					a = z80_read8((uint16_t)(iy + 1u)) & 7u;
					bc_3 = 0x28u;
					sound_021C(bc_3, a);
					return;
				}
				z80_block(51, 6);
				iy = 0xE410u;
				de = 9u;
				b = 0x18u;
				a = unk_E088 & 0xFu;
				for (;;)
				{
					z80_block(29, 3);
					if (a == z80_read8((uint16_t)(iy + 3u)))
					{
						z80_block(46, 5);
						hl_2 = a;
						if (unk_E012 == z80_read8((uint16_t)(iy + 2u)))
						{
							z80_block(37, 5);
							a = z80_read8(iy);
							if (((a >> 1) & 1u) == 0u)
							{
								z80_block(18, 3);
								if ((a >> 3) & 1u)
								{
									z80_block(17, 2);
									if ((a & 0x7Fu) != 0u)
										break;
								}
							}
						}
						z80_block(4, 1);
						a = (uint8_t)hl_2;
					}
					z80_block(23, 3);
					iy += de;
					b--;
					if (b == 0u)
					{
						z80_block(10, 1);
						return;
					}
					z80_cycles(5);
				}
				z80_block(65, 9);
				if (unk_E0E0[hl_2] >> 7 == 0u)
				{
					z80_block(33, 3);
					z80_write8(iy, z80_read8(iy) | 1u);
					return;
				}
				z80_block(33, 3);
				z80_write8(iy, z80_read8(iy) | 2u);
				return;
			}
			z80_block(88, 10);
			de = a;
			unk_E013 = SOUND_ROM8(0x2016u + de);
			a = z80_read8((uint16_t)(ix + 3u));
			if (a < 0xF0u)
			{
				z80_block(27, 2);
				hl = 0x1BC2u;
				hl = sound_0366(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(59, 7);
					unk_E00D = z80_read8((uint16_t)(hl + 2u));
					hl = 0xE4E8u;
					de = 9u;
					b = 6u;
					for (;;)
					{
						z80_block(21, 3);
						if (z80_read8(hl) == 0u)
							break;
						z80_block(19, 2);
						hl += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(182, 22);
					z80_write8(hl, 0x48u);
					hl++;
					unk_E01E = z80_read8(hl) & 7u;
					hl++;
					a = unk_E012;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = unk_E088 & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					a = unk_E013;
					z80_write8(hl, a);
					hl = (hl & 0xFFu) | ((uint32_t)a << 8);
					de_2 = z80_read8((uint16_t)(ix + 8u));
					hl = sound_0B77(de_2, hl);
					z80_block(122, 16);
					unk_E013 = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
					de = b;
					unk_E01B = (z80_read8((uint16_t)(unk_E0E0_ADDRESS + de)) & 0xFu) | 0x80u;
					hl = 0xE0C0u;
					bc_3 = c;
					sound_12E0(bc_3, de, hl);
					return;
				}
				for (;;)
				{
					z80_block(44, 6);
					hl++;
					if (unk_E02D <= z80_read8(hl))
						break;
					z80_block(28, 4);
					hl += 3u;
				}
				z80_block(78, 9);
				hl++;
				unk_E00D = z80_read8(hl);
				hl++;
				unk_E02B = z80_read8(hl);
				unk_E02C = z80_read8((uint16_t)(hl + 1u));
				L_000CF2 = true;
			}
			else
			{
				z80_block(34, 3);
				a += 0x10u;
				hl = 0x2DA5u;
				hl = sound_0366(hl, a);
				z80_block(112, 14);
				de = b;
				hl = hl + de + de + de;
				unk_E01D = z80_read8(hl);
				hl++;
				c = z80_read8(hl);
				a = z80_read8((uint16_t)(hl + 1u));
				unk_E00D = a;
				if (a == 0xFFu)
				{
					z80_cycles(6);
					return;
				}
				z80_block(37, 4);
				L_000CF2 = (unk_E088 & 0xFu) != 0xFu;
				if (!L_000CF2)
				{
					z80_block(21, 3);
					a = b;
					if (a == 0x2Cu)
						L_000C9C = true;
					else
					{
						z80_block(17, 2);
						L_000CF2 = a != 0x2Au;
						if (!L_000CF2)
							L_000C9C = true;
					}
					if (!L_000CF2)
					{
						if (L_000C9C)
						{
							L_000C9C = false;
							z80_block(27, 3);
							hl = 0xE412u;
							de = 9u;
							b = 0x18u;
							for (;;)
							{
								z80_block(24, 3);
								if (z80_read8(hl) == 0xCAu)
								{
									z80_block(36, 5);
									hl++;
									a = z80_read8(hl);
									hl--;
									if (a == 0xFu)
									{
										z80_block(34, 5);
										hl = hl - 1u - 1u;
										if ((z80_read8(hl) >> 6) & 1u)
										{
											z80_block(22, 3);
											if ((z80_read8(hl) >> 3) & 1u)
											{
												z80_block(15, 2);
												z80_write8(hl, z80_read8(hl) | 1u);
											}
										}
										z80_block(12, 2);
										hl += 2u;
									}
								}
								z80_block(19, 2);
								hl += de;
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							L_000CF2 = true;
						}
					}
				}
			}
			if (L_000CF2)
			{
				z80_block(33, 3);
				hl = unk_E028;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(21, 3);
					if ((uint8_t)hl == 0xE8u)
					{
						z80_cycles(7);
						hl = sound_0BA6();
					}
					z80_block(21, 3);
					a = 0u;
					if (0u == z80_read8(hl))
					{
						z80_block(58, 5);
						stack_2 = hl;
						unk_E028 = hl + de;
						hl = stack_2;
						L_000D38 = true;
					}
					if (L_000D38)
						break;
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				if (!L_000D38)
				{
					z80_block(10, 1);
					z80_block(17, 2);
					hl = 0xE416u;
					b = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (a == z80_read8(hl))
							break;
						z80_block(19, 2);
						hl += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(42, 5);
					z80_write8(hl, 0x18u);
					stack_2 = hl;
					hl = 0xE416u;
					b = 0x18u;
					for (;;)
					{
						z80_block(30, 3);
						z80_write8(hl, z80_read8(hl) - 1u);
						hl += de;
						b--;
						if (b == 0u)
							break;
						z80_cycles(5);
					}
					z80_block(52, 5);
					unk_E01F--;
					hl = stack_2 - 6u;
				}
				z80_block(46, 5);
				z80_write8(hl, 0x48u);
				if (z80_read8((uint16_t)(ix + 3u)) < 0xF0u)
				{
					do
					{
						z80_block(27, 3);
					} while (z80_read8(PCM_C000) & 1u);
					z80_block(479, 64);
					hl++;
					z80_write8(PCM_C001, z80_read8(hl));
					hl++;
					a = unk_E012;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = unk_E088 & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					z80_write8(hl, unk_E013);
					hl++;
					a = unk_E01F;
					z80_write8(hl, a);
					unk_E01F = a + 1u;
					hl++;
					z80_write8(hl, unk_E02B);
					z80_write8((uint16_t)(hl + 1u), unk_E02C);
					de = (uint8_t)(b * 2u);
					hl = 0xE0C0u + de;
					de = unk_E02B;
					stack_2 = z80_read16(hl) + de;
					hl = 0x20AEu + (uint8_t)((c & 0xFu) * 2u);
					hl = SOUND_ROM16(hl) + stack_2 + stack_2;
					hl = z80_read16(hl);
					hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((c & ~0xFu) + (hl >> 8)) << 8);
					a = unk_E02C + (hl >> 8);
				}
				else
				{
					z80_block(15, 2);
					z80_write8(hl, z80_read8(hl) | 0x10u);
					do
					{
						z80_block(27, 3);
					} while (z80_read8(PCM_C000) & 1u);
					z80_block(171, 21);
					hl++;
					z80_write8(PCM_C001, z80_read8(hl));
					hl++;
					z80_write8(hl, unk_E012);
					hl++;
					z80_write8(hl, unk_E088 & 0xFu);
					hl++;
					z80_write8(hl, c);
					hl++;
					z80_write8(hl, unk_E013);
					hl++;
					if ((unk_E088 & 0xFu) == 0xFu)
					{
						z80_block(37, 4);
						a = unk_E01F;
						z80_write8(hl, a);
						unk_E01F = a + 1u;
					}
					else
					{
						z80_block(24, 3);
						z80_write8(hl, 0xFFu);
					}
					z80_block(30, 3);
					a = unk_E01D;
					hl = 0u;
				}
				z80_block(24, 2);
				bc_3 = 3u;
				sound_02CA(bc_3, a);
				z80_block(36, 5);
				a = hl | 1u;
				bc_3 = 2u;
				sound_02CA(bc_3, a);
				z80_block(37, 3);
				a = unk_E00D;
				bc_3 = 1u;
				sound_02CA(bc_3, a);
				z80_block(53, 5);
				hl = (hl & 0xFFu) | ((uint32_t)unk_E013 << 8);
				de_2 = z80_read8((uint16_t)(ix + 8u));
				hl = sound_0B77(de_2, hl);
				z80_block(39, 5);
				a = ~(hl >> 8) | 1u;
				bc_3 = 5u;
				sound_02CA(bc_3, a);
				z80_block(31, 3);
				a = 0x90u;
				bc_3 = 0u;
				sound_02CA(bc_3, a);
				z80_block(24, 3);
				a = 0x80u;
				bc_3 = 4u;
				sound_02CA(bc_3, a);
				return;
			}
		}
		z80_block(17, 2);
		if (a == 0x80u)
		{
			z80_block(18, 3);
			if (c >> 7)
			{
				z80_block(17, 1);
				sound_127B(&bc_2, &ignored8);
				c = (uint8_t)bc_2;
			}
			z80_block(96, 11);
			unk_E012 = rom_1F62[(uint8_t)(z80_read8((uint16_t)(ix + 4u)) + c)];
			unk_E082++;
			L_000E39 = true;
		}
	} while (L_000E39);
	z80_block(17, 2);
	if (a == 0xE0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_127B(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(21, 2);
		b = c;
		sound_127B(&ignored16, &a);
		z80_block(26, 5);
		carry = a & 1u;
		c = a >> 1;
		a = b;
		if (carry)
		{
			z80_block(7, 1);
			a |= 0x80u;
		}
		z80_block(78, 18);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		if ((c & 1u) == 0u)
		{
			z80_block(18, 3);
			t1 = ~a + 1u;
			a = t1;
			if (t1 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(57, 6);
		de = a;
		a = z80_read8((uint16_t)(ix + 9u));
		hl = 0x2546u;
		hl = sound_0366(hl, a);
		z80_block(36, 5);
		a = z80_read8((uint16_t)(hl + de));
		carry = c & 1u;
		c = c >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			c--;
			t2 = ~a + 1u;
			a = t2;
			if (t2 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(145, 19);
		b = c;
		c = a;
		unk_E014 = ((uint32_t)b << 8) | c;
		hl = 0xE0C0u + (uint8_t)((unk_E088 & 0xFu) * 2u);
		z80_write8(hl, c);
		z80_write8((uint16_t)(hl + 1u), b);
		iy = 0xE410u;
		b = 0x1Eu;
		a = unk_E088 & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (a == z80_read8((uint16_t)(iy + 3u)))
			{
				z80_block(48, 6);
				saved_a = a;
				a = z80_read8(iy);
				if ((a >> 3) & 1u)
				{
					z80_block(17, 2);
					if ((a & 0x7Fu) != 0u)
					{
						z80_cycles(7);
						sound_0F20(iy);
					}
				}
				z80_block(10, 1);
				a = saved_a;
			}
			z80_block(33, 4);
			iy += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xB0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_127B(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(21, 2);
		b = c;
		sound_127B(&bc_2, &ignored8);
		c = (uint8_t)bc_2;
		z80_block(21, 3);
		a = b;
		carry = a == 0u;
		if (a == 1u)
		{
			z80_block(123, 19);
			t3 = c;
			a = (t3 >> 1) | (carry << 7);
			carry = t3 & 1u;
			t4 = a;
			a = (t4 >> 1) | (carry << 7);
			carry = t4 & 1u;
			t5 = a;
			a = (t5 >> 1) | (carry << 7);
			carry = t5 & 1u;
			a = ((a >> 1) | (carry << 7)) & 7u;
			c = a;
			unk_E01B = a;
			hl = unk_E0E0_ADDRESS + (unk_E088 & 0xFu);
			a = z80_read8(hl);
			b = a;
			if ((a & 0x7Fu) == c)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(hl, (b & 0x80u) | c);
			hl = 0xE413u;
			b = 0x1Eu;
			a = unk_E088 & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (a == z80_read8(hl))
				{
					z80_block(47, 8);
					de = (uint32_t)a << 8;
					hl = hl - 1u - 1u - 1u;
					a = z80_read8(hl);
					if ((a >> 3) & 1u)
					{
						z80_block(17, 2);
						if ((a & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							bc_2 = (uint32_t)b << 8;
							sound_0FF8(ix, &bc_2, &hl);
							b = (uint8_t)(bc_2 >> 8);
						}
					}
					z80_block(22, 4);
					hl += 3u;
					a = (uint8_t)(de >> 8);
				}
				z80_block(29, 3);
				hl += 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 7u)
		{
			z80_block(18, 3);
			t6 = c * 2u;
			a = t6;
			if (t6 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(60, 7);
			z80_write8((uint16_t)(ix + 2u), a);
			hl = (uint32_t)a << 8;
			a = 0xFFu;
			if ((z80_read8(ix) & 1u) == 0u)
			{
				z80_block(13, 1);
				a = unk_E09C;
			}
			z80_block(21, 2);
			de_2 = a;
			hl = sound_0B77(de_2, hl);
			z80_block(18, 3);
			a = (uint8_t)(hl >> 8);
			if (a != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(36, 4);
			z80_write8((uint16_t)(ix + 8u), a);
			hl = 0xE410u;
			b = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				stack_2 = hl;
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((z80_read8(hl) >> 3) & 1u)
					{
						z80_block(55, 7);
						hl += 3u;
						if ((unk_E088 & 0xFu) == z80_read8(hl))
						{
							z80_block(37, 6);
							hl = hl - 1u - 1u;
							c = z80_read8(hl);
							if (c >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(PCM_C000) & 1u);
								z80_block(81, 8);
								z80_write8(PCM_C001, c);
								hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
								de_2 = z80_read8((uint16_t)(ix + 8u));
								hl = sound_0B77(de_2, hl);
								z80_block(39, 5);
								a = ~(hl >> 8) & 0xFEu;
								bc_3 = 5u;
								sound_02CA(bc_3, a);
							}
							else
							{
								z80_block(17, 1);
								bc_3 = c;
								sound_14A8(bc_3, hl, ix);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				hl = stack_2 + 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x10u)
		{
			z80_block(18, 3);
			t7 = c * 2u;
			a = t7;
			if (t7 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(34, 4);
			unk_E09C = a;
			iy = 0xE382u;
			b = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_b = b;
				hl = (hl & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(iy + 2u)) << 8);
				a = 0xFFu;
				if ((z80_read8(iy) & 1u) == 0u)
				{
					z80_block(13, 1);
					a = unk_E09C;
				}
				z80_block(21, 2);
				de_2 = a;
				hl = sound_0B77(de_2, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(36, 4);
				z80_write8((uint16_t)(iy + 8u), a);
				hl = 0xE413u;
				b = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_hl = hl;
					if (z80_read8((uint16_t)(iy + 1u)) == z80_read8(hl))
					{
						z80_block(40, 6);
						hl = hl - 1u - 1u - 1u;
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if ((z80_read8(hl) >> 3) & 1u)
							{
								z80_block(31, 5);
								hl++;
								c = z80_read8(hl);
								if (c >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(PCM_C000) & 1u);
									z80_block(81, 8);
									z80_write8(PCM_C001, c);
									hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
									de_2 = z80_read8((uint16_t)(iy + 8u));
									hl = sound_0B77(de_2, hl);
									z80_block(39, 5);
									a = ~(hl >> 8) & 0xFEu;
									bc_3 = 5u;
									sound_02CA(bc_3, a);
								}
								else
								{
									z80_block(17, 1);
									bc_3 = c;
									sound_145A(bc_3, hl, iy);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					hl = saved_hl + 9u;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				iy += 0xAu;
				b = saved_b - 1u;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x40u)
		{
			z80_block(21, 3);
			if (c >= 0x40u)
			{
				z80_block(77, 9);
				hl = unk_E0E0_ADDRESS + (unk_E088 & 0xFu);
				z80_write8(hl, z80_read8(hl) | 0x80u);
				return;
			}
			z80_block(51, 6);
			iy = 0xE410u;
			de = 9u;
			b = 0x1Eu;
			a = unk_E088 & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (a == z80_read8((uint16_t)(iy + 3u)))
				{
					z80_block(41, 6);
					hl_2 = a;
					a = z80_read8(iy);
					if ((a >> 1) & 1u)
					{
						z80_block(18, 3);
						if ((a >> 3) & 1u)
						{
							z80_block(37, 5);
							if (z80_read8((uint16_t)(iy + 1u)) >> 7 == 0u)
							{
								z80_block(23, 2);
								z80_write8(iy, z80_read8(iy) | 1u);
							}
							else
							{
								z80_block(17, 1);
								sound_1449(iy);
								z80_block(10, 1);
							}
						}
					}
					z80_block(4, 1);
					a = (uint8_t)hl_2;
				}
				z80_block(23, 3);
				iy += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			hl = unk_E0E0_ADDRESS + (unk_E088 & 0xFu);
			z80_write8(hl, z80_read8(hl) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (a == 0x7Bu)
		{
			z80_block(47, 5);
			a = unk_E088 & 0xFu;
			hl = 0xE410u;
			de = 9u;
			b = 0x18u;
			for (;;)
			{
				z80_block(22, 3);
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((z80_read8(hl) >> 3) & 1u)
					{
						z80_block(35, 5);
						hl += 3u;
						if (a != z80_read8(hl))
						{
							z80_block(18, 3);
							hl = hl - 1u - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							hl = hl - 1u - 1u - 1u;
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
				}
				z80_block(19, 2);
				hl += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x50u)
		{
			z80_block(21, 3);
			if (c == 0x7Eu)
			{
				z80_block(27, 3);
				hl = 0xE410u;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if ((z80_read8(hl) >> 3) & 1u)
					{
						z80_block(22, 3);
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((z80_read8(hl) >> 4) & 1u) == 0u)
							{
								z80_block(15, 2);
								z80_write8(hl, z80_read8(hl) | 1u);
							}
							else
							{
								z80_block(60, 9);
								hl += 3u;
								a = z80_read8(hl);
								hl = hl - 1u - 1u - 1u;
								if (a == 0xFu)
								{
									z80_block(15, 2);
									z80_write8(hl, z80_read8(hl) | 1u);
								}
							}
						}
					}
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				hl = 0xE410u;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if ((z80_read8(hl) >> 3) & 1u)
					{
						z80_block(22, 3);
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(15, 2);
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
					z80_block(19, 2);
					hl += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			hl = unk_E0E0_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			hl = 0xE0C0u;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x20u)
		{
			z80_block(31, 3);
			a = c;
			hl = 0x1847u;
			hl = sound_0366(hl, a);
			z80_block(136, 19);
			move_block(hl, 0xE382u, 0x50u, 1u);
			hl += 0x50u;
			a = (unk_E02A & 7u) | ((((uint32_t)z80_read8(hl) << 1) << 1) << 1);
			port_out(0xB0u, a);
			unk_E02A = a;
			iy = 0xE382u;
			b = 8u;
			for (;;)
			{
				z80_block(56, 6);
				hl = (hl & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(iy + 2u)) << 8);
				a = 0xFFu;
				if ((z80_read8(iy) & 1u) == 0u)
				{
					z80_block(13, 1);
					a = unk_E09C;
				}
				z80_block(21, 2);
				de_2 = a;
				hl = sound_0B77(de_2, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(52, 6);
				z80_write8((uint16_t)(iy + 8u), a);
				iy += 0xAu;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		c = rom_1F62[c];
		hl = 0xE410u;
		b = 0x18u;
		for (;;)
		{
			z80_block(22, 3);
			if ((z80_read8(hl) >> 6) & 1u)
			{
				z80_block(22, 3);
				if ((z80_read8(hl) >> 3) & 1u)
				{
					z80_block(33, 5);
					hl += 2u;
					if (c != z80_read8(hl))
					{
						z80_block(12, 2);
						hl = hl - 1u - 1u;
					}
					else
					{
						z80_block(43, 5);
						hl++;
						if ((unk_E088 & 0xFu) != z80_read8(hl))
						{
							z80_block(6, 1);
							hl--;
							z80_block(12, 2);
							hl = hl - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							hl = hl - 1u - 1u - 1u;
							z80_write8(hl, z80_read8(hl) | 1u);
						}
					}
				}
			}
			z80_block(29, 3);
			hl += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xC0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			sound_127B(&bc_2, &ignored8);
			c = (uint8_t)bc_2;
		}
		z80_block(29, 3);
		z80_write8((uint16_t)(ix + 3u), c);
		return;
	}
	z80_block(17, 2);
	if (a != 0xD0u)
	{
		z80_block(17, 2);
		if (a != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	bc_3 = c;
	sound_0BF2(bc_3);
}

void sound_0F20(uint16_t iy)
{
	uint8_t a = 0, bc = 0, c = 0;
	uint16_t de = 0, hl = 0;

	z80_block(37, 5);
	a = z80_read8((uint16_t)(iy + 1u));
	if (a >> 7 != 0u)
	{
		sound_13D8(iy, a);
		return;
	}
	z80_block(4, 1);
	c = a;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(287, 37);
	z80_write8(PCM_C001, c);
	a = z80_read8((uint16_t)(iy + 2u));
	hl = 0x20AEu + (uint8_t)((a & 0xFu) * 2u);
	de = unk_E014;
	hl = SOUND_ROM16(hl) + de + de;
	de = z80_read8((uint16_t)(iy + 7u));
	hl = hl + de + de;
	hl = z80_read16(hl);
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((a & ~0xFu) + (hl >> 8)) << 8);
	a = z80_read8((uint16_t)(iy + 8u)) + (hl >> 8);
	bc = 3u;
	sound_02CA(bc, a);
	z80_block(21, 3);
	a = (uint8_t)hl;
	bc = 2u;
	sound_02CA(bc, a);
}

void sound_0FF8(uint16_t ix, uint16_t *p_bc, uint16_t *p_hl)
{
	uint16_t bc = (uint16_t)*p_bc;
	uint16_t hl = (uint16_t)*p_hl;
	uint8_t a = 0, c = 0;
	uint16_t bc_2 = 0;

	z80_block(31, 5);
	hl++;
	c = z80_read8(hl);
	if (c >> 7 != 0u)
	{
		z80_block(79, 9);
		a = (uint8_t)(((bc & ~0xFFu) | c) >> 8);
		unk_E01E = a;
		c = (a & 3u) - 0x4Cu;
		a = (unk_E01B & 0x3Fu) | 0x80u;
		bc_2 = c;
		bc_2 = sound_0212((uint8_t)bc_2, a);
		z80_block(16, 2);
		hl--;
		bc_2 = (bc_2 & ~0xFFu) | c;
		*p_bc = bc_2;
		*p_hl = hl;
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(73, 7);
	z80_write8(PCM_C001, c);
	c = 6u;
	a = unk_E01B | z80_read8((uint16_t)(ix + 6u));
	bc_2 = (bc & ~0xFFu) | 6u;
	sound_02CA((uint8_t)bc_2, a);
	z80_block(16, 2);
	hl--;
	bc_2 = (bc_2 & ~0xFFu) | c;
	*p_bc = bc_2;
	*p_hl = hl;
}

void sound_127B(uint16_t *p_bc, uint8_t *p_a)
{
	uint8_t a = 0;
	uint16_t bc = 0;

	z80_block(56, 7);
	unk_E082++;
	a = z80_read8(unk_E082 | 0xE200u);
	bc = a;
	*p_bc = (*p_bc & 0xFF00u) | bc;
	*p_a = a;
}

void sound_12E0(uint8_t bc, uint16_t de, uint16_t hl)
{
	uint8_t a = 0, b = 0, bc_2 = 0, c = 0;
	uint16_t bc_3 = 0, de_2 = 0, hl_2 = 0;
	uint8_t saved_b = 0, saved_c = 0;
	uint16_t stack_2 = 0;

	c = (uint8_t)bc;
	z80_block(242, 35);
	hl = hl + de + de;
	stack_2 = z80_read16(hl);
	hl_2 = 0x1565u + (uint8_t)((c & 0xFu) * 2u);
	hl_2 = SOUND_ROM16(hl_2) + stack_2 + stack_2;
	hl_2 = z80_read16(hl_2);
	hl_2 = (hl_2 & 0xFFu) | ((uint32_t)(uint8_t)(((c >> 1) & 0x38u) + (hl_2 >> 8)) << 8);
	a = unk_E01E;
	if (((a >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		de_2 = (a & 3u) << 8;
		a = (uint8_t)(hl_2 >> 8);
		bc_2 = 0xA4u | (de_2 >> 8);
		sound_0228(bc_2, a);
		z80_block(65, 9);
		a = (uint8_t)hl_2;
		bc_2 = 0xA0u | (de_2 >> 8);
		sound_0228(bc_2, a);
	}
	else
	{
		z80_block(40, 6);
		de_2 = (uint32_t)a << 8;
		a = (uint8_t)(hl_2 >> 8);
		bc_2 = 0xA4u | (de_2 >> 8);
		sound_021C(bc_2, a);
		z80_block(65, 9);
		a = (uint8_t)hl_2;
		bc_2 = 0xA0u | (de_2 >> 8);
		sound_021C(bc_2, a);
		z80_block(10, 1);
	}
	z80_block(31, 4);
	a = unk_E01E;
	if (((a >> 2) & 1u) != 0u)
	{
		z80_block(17, 1);
		sound_1515(a, &bc_3, &de_2, &hl_2);
		c = (uint8_t)bc_3;
	}
	else
	{
		z80_block(17, 1);
		sound_14F6(a, &bc_3, &de_2, &hl_2);
		c = (uint8_t)bc_3;
		z80_block(10, 1);
	}
	z80_block(7, 1);
	b = 4u;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = z80_read8(de_2) + c;
		de_2++;
		c = a;
		a = z80_read8(hl_2);
		hl_2++;
		if (a >> 7 == 0u)
		{
			z80_block(21, 3);
			a += unk_E013;
		}
		z80_block(17, 1);
		bc_3 = c;
		sound_0212((uint8_t)bc_3, a);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(61, 6);
	a = unk_E01B;
	bc_3 = (uint8_t)((unk_E01E & 3u) - 0x4Cu);
	sound_0212((uint8_t)bc_3, a);
	z80_block(37, 4);
	a = unk_E01E | 0xF0u;
	bc_2 = 0x28u;
	sound_021C(bc_2, a);
}

void sound_13D8(uint16_t iy, uint8_t a)
{
	uint8_t a_2 = 0, bc = 0, d = 0;
	uint16_t hl = 0;
	uint16_t t1 = 0;

	z80_block(234, 33);
	unk_E01E = a & 7u;
	a_2 = z80_read8((uint16_t)(iy + 2u));
	hl = 0x1565u + (uint8_t)((a_2 & 0xFu) * 2u);
	t1 = unk_E014;
	d = (uint8_t)(t1 >> 8);
	hl = SOUND_ROM16(hl) + (((uint32_t)d << 8) | (uint8_t)t1) + (((uint32_t)d << 8) | (uint8_t)t1);
	hl = z80_read16(hl);
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)(((a_2 & ~0xFu) >> 1) + (hl >> 8)) << 8);
	a_2 = unk_E01E;
	if (((a_2 >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		d = a_2 & 3u;
		a_2 = (uint8_t)(hl >> 8);
		bc = 0xA4u | d;
		sound_0228(bc, a_2);
		z80_block(58, 9);
		a_2 = (uint8_t)hl;
		bc = 0xA0u | d;
		sound_0228(bc, a_2);
		return;
	}
	z80_block(40, 6);
	d = a_2;
	a_2 = (uint8_t)(hl >> 8);
	bc = 0xA4u | d;
	sound_021C(bc, a_2);
	z80_block(58, 9);
	a_2 = (uint8_t)hl;
	bc = 0xA0u | d;
	sound_021C(bc, a_2);
}

void sound_1449(uint16_t iy)
{
	uint8_t a = 0, bc = 0;

	z80_block(85, 10);
	z80_write8(iy, 0u);
	z80_write8((uint16_t)(iy + 3u), 0u);
	a = z80_read8((uint16_t)(iy + 1u)) & 7u;
	bc = 0x28u;
	sound_021C(bc, a);
}

void sound_145A(uint8_t bc, uint16_t hl, uint16_t iy)
{
	uint8_t a = 0, b = 0, c = 0, de = 0;
	uint16_t bc_2 = 0, de_2 = 0, hl_2 = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(103, 11);
	a = (uint8_t)bc;
	unk_E01E = a;
	c = a & 3u;
	hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
	de = z80_read8((uint16_t)(iy + 8u));
	hl = sound_0B77(de, hl);
	z80_block(78, 9);
	unk_E013 = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
	a = z80_read8((uint16_t)(iy + 3u));
	hl_2 = 0x1BC2u;
	hl_2 = sound_0366(hl_2, a);
	z80_block(40, 4);
	a = z80_read8((uint16_t)(hl_2 + 1u));
	hl_2 = 0x2A5Eu;
	hl_2 = sound_0366(hl_2, a);
	z80_block(38, 4);
	hl_2 += 0x15u;
	b = 4u;
	de_2 = 0x1549u;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = SOUND_ROM8(de_2) + c;
		de_2++;
		c = a;
		a = z80_read8(hl_2);
		hl_2++;
		if (a >> 7 == 0u)
		{
			z80_block(38, 4);
			a += unk_E013;
			bc_2 = c;
			sound_0212((uint8_t)bc_2, a);
		}
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(20, 2);
}

void sound_14A8(uint8_t bc, uint16_t hl, uint16_t ix)
{
	uint8_t a = 0, b = 0, c = 0, de = 0;
	uint16_t bc_2 = 0, de_2 = 0, hl_2 = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(103, 11);
	a = (uint8_t)bc;
	unk_E01E = a;
	c = a & 3u;
	hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
	de = z80_read8((uint16_t)(ix + 8u));
	hl = sound_0B77(de, hl);
	z80_block(78, 9);
	unk_E013 = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
	a = z80_read8((uint16_t)(ix + 3u));
	hl_2 = 0x1BC2u;
	hl_2 = sound_0366(hl_2, a);
	z80_block(40, 4);
	a = z80_read8((uint16_t)(hl_2 + 1u));
	hl_2 = 0x2A5Eu;
	hl_2 = sound_0366(hl_2, a);
	z80_block(38, 4);
	hl_2 += 0x15u;
	b = 4u;
	de_2 = 0x1549u;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = SOUND_ROM8(de_2) + c;
		de_2++;
		c = a;
		a = z80_read8(hl_2);
		hl_2++;
		if (a >> 7 == 0u)
		{
			z80_block(38, 4);
			a += unk_E013;
			bc_2 = c;
			sound_0212((uint8_t)bc_2, a);
		}
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(20, 2);
}

void sound_14F6(uint8_t a, uint16_t *p_bc, uint16_t *p_de, uint16_t *p_hl)
{
	uint8_t a_2 = 0, b = 0, bc = 0, c = 0;
	uint16_t bc_2 = 0, de = 0, hl = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(51, 5);
	c = a & 3u;
	a_2 = unk_E00D;
	hl = 0x2A5Eu;
	hl = sound_0366(hl, a_2);
	z80_block(17, 2);
	de = 0x1534u;
	b = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_b = b;
		saved_c = c;
		a_2 = SOUND_ROM8(de) + c;
		de++;
		c = a_2;
		a_2 = z80_read8(hl);
		hl++;
		bc = c;
		sound_021C(bc, a_2);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	bc_2 = ((uint32_t)b << 8) | c;
	*p_bc = bc_2;
	*p_de = de;
	*p_hl = hl;
}

void sound_1515(uint8_t a, uint16_t *p_bc, uint16_t *p_de, uint16_t *p_hl)
{
	uint8_t a_2 = 0, b = 0, bc = 0, c = 0;
	uint16_t bc_2 = 0, de = 0, hl = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(51, 5);
	c = a & 3u;
	a_2 = unk_E00D;
	hl = 0x2A5Eu;
	hl = sound_0366(hl, a_2);
	z80_block(17, 2);
	de = 0x1534u;
	b = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_b = b;
		saved_c = c;
		a_2 = SOUND_ROM8(de) + c;
		de++;
		c = a_2;
		a_2 = z80_read8(hl);
		hl++;
		bc = c;
		sound_0228(bc, a_2);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	bc_2 = ((uint32_t)b << 8) | c;
	*p_bc = bc_2;
	*p_de = de;
	*p_hl = hl;
}

void sound_36AD(void)
{
	uint8_t a = 0, bc = 0, d = 0, de = 0;
	uint16_t de_2 = 0, hl = 0;

	z80_block(22, 3);
	if (unk_E712 == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(10, 1);
	hl = PCM_C000;
	do
	{
		z80_block(21, 3);
	} while (z80_read8(hl) & 1u);
	z80_block(51, 6);
	z80_write8(PCM_C001, 0x1Du);
	a = unk_E710;
	d = a;
	if (a == 0u)
	{
		z80_block(26, 4);
		a = unk_E711;
		if (a >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		unk_E711 = a & 0x7Fu;
		a = 0u;
		bc = 4u;
		sound_02CA(bc, a);
		z80_block(31, 4);
		unk_E712 = 0u;
		return;
	}
	z80_block(31, 4);
	a = unk_E711;
	if (a >> 7 == 0u)
	{
		z80_block(45, 5);
		unk_E711 = a | 0x80u;
		de = 0x70u;
		sound_3748(de);
	}
	z80_block(32, 4);
	hl = unk_E712_ADDRESS;
	if ((unk_E712 >> 6) & 1u)
	{
		z80_block(15, 2);
		unk_E712 &= 0xBFu;
	}
	z80_block(22, 3);
	if (unk_E712 >> 7)
	{
		z80_block(45, 4);
		unk_E712 &= 0x7Fu;
		a = unk_E713;
		sound_37B5(a);
	}
	z80_block(22, 3);
	if (((unk_E712 >> 5) & 1u) != 0u)
	{
		z80_block(62, 7);
		unk_E712 &= 0xDFu;
		de_2 = ((uint32_t)d << 8) | unk_E714;
		sound_376D(de_2);
		return;
	}
	z80_block(10, 1);
}

void sound_36F6(void)
{
	uint8_t a = 0, bc = 0, d = 0, de = 0;
	uint16_t de_2 = 0, hl = 0;

	z80_block(22, 3);
	if (unk_E812 == 0u)
	{
		z80_cycles(6);
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(51, 6);
	z80_write8(PCM_C001, 0x1Eu);
	a = unk_E810;
	d = a;
	if (a == 0u)
	{
		z80_block(26, 4);
		a = unk_E811;
		if (a >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		unk_E811 = a & 0x7Fu;
		a = 0u;
		bc = 4u;
		sound_02CA(bc, a);
		z80_block(31, 4);
		unk_E812 = 0u;
		return;
	}
	z80_block(31, 4);
	a = unk_E811;
	if (a >> 7 == 0u)
	{
		z80_block(45, 5);
		unk_E811 = a | 0x80u;
		de = 0x90u;
		sound_3748(de);
	}
	z80_block(32, 4);
	hl = unk_E812_ADDRESS;
	if ((unk_E812 >> 6) & 1u)
	{
		z80_block(60, 7);
		unk_E812 &= 0xBFu;
		a = (uint32_t)unk_E814 << 1;
		bc = 5u;
		sound_02CA(bc, a);
	}
	z80_block(22, 3);
	if (unk_E812 >> 7)
	{
		z80_block(45, 4);
		unk_E812 &= 0x7Fu;
		a = unk_E813;
		sound_37B5(a);
	}
	z80_block(22, 3);
	if (((unk_E812 >> 5) & 1u) != 0u)
	{
		z80_block(52, 6);
		unk_E812 &= 0xDFu;
		de_2 = ((uint32_t)d << 8) | unk_E814;
		sound_376D(de_2);
		return;
	}
	z80_block(10, 1);
}

void sound_3748(uint8_t de)
{
	uint8_t a = 0, bc = 0;

	z80_block(28, 3);
	a = (uint8_t)de;
	bc = 0u;
	sound_02CA(bc, a);
	z80_block(31, 3);
	a = 0x80u;
	bc = 4u;
	sound_02CA(bc, a);
	z80_block(10, 1);
}

void sound_376D(uint16_t de)
{
	uint8_t a = 0, b = 0, bc = 0, c = 0;
	uint16_t hl = 0;

	z80_block(31, 3);
	a = (uint8_t)(de >> 8);
	hl = 0x38F3u;
	hl = sound_0366(hl, a);
	z80_block(18, 3);
	if ((int8_t)(hl >> 8) < 0)
	{
		z80_block(27, 5);
		a = refresh_read(2);
		c = a;
		if ((int8_t)a < 0)
		{
			z80_block(7, 1);
			b = 0xFFu;
		}
		else
		{
			z80_block(17, 2);
			b = 0u;
		}
		z80_block(11, 1);
		hl += ((uint32_t)b << 8) | c;
	}
	z80_block(35, 4);
	a = hl & 0xFCu;
	bc = 2u;
	sound_02CA(bc, a);
	z80_block(28, 3);
	a = (uint8_t)(hl >> 8);
	bc = 3u;
	sound_02CA(bc, a);
	z80_block(67, 8);
	a = rom_37F3[de >> 8] - de;
	bc = 5u;
	sound_02CA(bc, a);
	z80_block(40, 5);
	a = refresh_read(2);
	a &= 0x3Fu;
	bc = 6u;
	sound_02CA(bc, a);
	z80_block(31, 3);
	a = 1u;
	bc = 0xAu;
	sound_02CA(bc, a);
	z80_block(10, 1);
}

void sound_37B5(uint8_t a)
{
	uint8_t a_2 = 0, b = 0, bc = 0;

	z80_block(32, 4);
	b = a;
	a_2 = 0u;
	bc = 2u;
	sound_02CA(bc, a_2);
	z80_block(28, 3);
	a_2 = b;
	bc = 1u;
	sound_02CA(bc, a_2);
	z80_block(10, 1);
}

void sound_3AF3(void)
{
	uint8_t a = 0, h = 0, l = 0;
	uint16_t t1 = 0, t2 = 0;

	z80_block(27, 3);
	if (unk_E780 == 0u)
	{
		z80_block(27, 3);
		if (unk_E786 == 0u)
		{
			z80_block(74, 8);
			unk_E784 = 0xC5u;
			unk_E785 = 0x3Du;
			unk_E786 = 0xFFu;
			return;
		}
		z80_block(71, 8);
		unk_E784 = 0x5Du;
		unk_E785 = 0x3Bu;
		unk_E786 = 0u;
		return;
	}
	z80_block(27, 3);
	a = unk_E781;
	if (a != 0u)
	{
		z80_block(27, 3);
		unk_E781 = a - 1u;
		return;
	}
	z80_block(81, 9);
	unk_E781 = 0x40u;
	l = unk_E784;
	h = unk_E785;
	if (unk_E786 != 0u)
	{
		z80_block(6, 1);
		t1 = (((uint32_t)h << 8) | l) - 1u;
		h = (uint8_t)((uint16_t)((((uint32_t)h << 8) | l) - 1u) >> 8);
		l = (uint8_t)t1;
	}
	else
	{
		z80_block(16, 2);
		t2 = (((uint32_t)h << 8) | l) + 1u;
		h = (uint8_t)((uint16_t)((((uint32_t)h << 8) | l) + 1u) >> 8);
		l = (uint8_t)t2;
	}
	z80_block(55, 7);
	unk_E784 = l;
	unk_E785 = h;
	a = z80_read8((uint16_t)(((uint32_t)h << 8) | l));
	if (a != 0u)
	{
		z80_block(43, 4);
		unk_E710 = a;
		unk_E712 = 0x20u;
		return;
	}
	z80_block(27, 3);
	if (unk_E786 == 0u)
	{
		z80_block(74, 8);
		unk_E784 = 0xC5u;
		unk_E785 = 0x3Du;
		unk_E786 = 0xFFu;
		return;
	}
	z80_block(71, 8);
	unk_E784 = 0x5Du;
	unk_E785 = 0x3Bu;
	unk_E786 = 0u;
}

void sound_3DE6(void)
{
	uint8_t a = 0, bc = 0, de = 0;
	uint16_t ix = 0, iy = 0;

	z80_block(27, 3);
	a = unk_E718;
	if (a != 0u)
	{
		z80_block(172, 24);
		ix = unk_E700_ADDRESS;
		iy = 0x3DC6u + (uint8_t)((a - 1u) * 2u * 2u);
		unk_E718 = 0u;
		unk_E71C = SOUND_ROM8(iy + 3u);
		if (SOUND_ROM8(iy) == unk_E719)
		{
			z80_cycles(6);
			return;
		}
		do
		{
			z80_block(27, 3);
		} while (z80_read8(PCM_C000) & 1u);
		z80_block(37, 4);
		z80_write8(PCM_C001, 0x1Bu);
		de = 0x70u;
		sound_3E86(de, ix, iy);
		return;
	}
	z80_block(42, 6);
	ix = unk_E700_ADDRESS;
	if (unk_E71D != 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 3);
	a = unk_E71C;
	if (a != 0u)
	{
		z80_block(27, 3);
		unk_E71C = a - 1u;
		return;
	}
	z80_block(49, 6);
	unk_E719 = 0u;
	unk_E71D = 0xFFu;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(51, 5);
	z80_write8(PCM_C001, 0x1Bu);
	a = 0u;
	bc = 4u;
	sound_02CA(bc, a);
	z80_block(10, 1);
}

void sound_3E36(void)
{
	uint8_t a = 0, bc = 0, de = 0;
	uint16_t ix = 0, iy = 0;

	z80_block(27, 3);
	a = unk_E818;
	if (a != 0u)
	{
		z80_block(172, 24);
		ix = unk_E800_ADDRESS;
		iy = 0x3DC6u + (uint8_t)((a - 1u) * 2u * 2u);
		unk_E818 = 0u;
		unk_E81C = SOUND_ROM8(iy + 3u);
		if (SOUND_ROM8(iy) == unk_E819)
		{
			z80_cycles(6);
			return;
		}
		do
		{
			z80_block(27, 3);
		} while (z80_read8(PCM_C000) & 1u);
		z80_block(37, 4);
		z80_write8(PCM_C001, 0x1Cu);
		de = 0x90u;
		sound_3E86(de, ix, iy);
		return;
	}
	z80_block(42, 6);
	ix = unk_E800_ADDRESS;
	if (unk_E81D != 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 3);
	a = unk_E81C;
	if (a != 0u)
	{
		z80_block(27, 3);
		unk_E81C = a - 1u;
		return;
	}
	z80_block(49, 6);
	unk_E819 = 0u;
	unk_E81D = 0xFFu;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(PCM_C000) & 1u);
	z80_block(51, 5);
	z80_write8(PCM_C001, 0x1Cu);
	a = 0u;
	bc = 4u;
	sound_02CA(bc, a);
	z80_block(10, 1);
}

void sound_3E86(uint8_t de, uint16_t ix, uint16_t iy)
{
	uint8_t a = 0, bc = 0;

	z80_block(85, 9);
	z80_write8((uint16_t)(ix + 0x1Du), 0u);
	a = z80_read8(iy);
	z80_write8((uint16_t)(ix + 0x19u), a);
	bc = 1u;
	sound_02CA(bc, a);
	z80_block(43, 4);
	a = z80_read8((uint16_t)(iy + 2u));
	bc = 5u;
	sound_02CA(bc, a);
	z80_block(28, 3);
	a = (uint8_t)de;
	bc = 0u;
	sound_02CA(bc, a);
	z80_block(43, 4);
	a = z80_read8((uint16_t)(iy + 1u));
	bc = 3u;
	sound_02CA(bc, a);
	z80_block(31, 3);
	a = 0x80u;
	bc = 4u;
	sound_02CA(bc, a);
	z80_block(10, 1);
}

void sound_3EED(void)
{
	uint8_t e = 0;
	uint16_t de = 0, hl = 0, iy = 0;
	uint8_t t1 = 0;

	z80_block(31, 3);
	hl = unk_E740_ADDRESS;
	t1 = unk_E740;
	unk_E740 = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(55, 7);
		unk_E740 = 4u;
		iy = unk_E800_ADDRESS;
		if (unk_E859 >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = 0xF200u;
		de = 0xE200u | unk_E858;
		de = sound_433A(de, hl, iy);
		e = (uint8_t)de;
		z80_block(24, 2);
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF300u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF400u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF500u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF600u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF700u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF800u;
		de = (de & ~0xFFu) | e;
		sound_403E(de, hl, iy);
		z80_block(10, 1);
		return;
	}
	z80_block(24, 3);
	if (3u == unk_E740)
	{
		z80_block(45, 6);
		iy = unk_E800_ADDRESS;
		if (unk_E859 >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = unk_F100_ADDRESS;
		de = 0xE200u | unk_E858;
		de = sound_42E2(de, hl, iy);
		e = (uint8_t)de;
		z80_block(24, 2);
		de = (de & ~0xFFu) | e;
		sound_403E(de, hl, iy);
		z80_block(10, 1);
		return;
	}
	z80_block(24, 3);
	if (2u == unk_E740)
	{
		z80_block(45, 6);
		iy = unk_E700_ADDRESS;
		if (unk_E759 >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = 0xEA00u;
		de = 0xE100u | unk_E758;
		de = sound_433A(de, hl, iy);
		e = (uint8_t)de;
		z80_block(24, 2);
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xEB00u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xEC00u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xED00u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xEE00u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xEF00u;
		de = (de & ~0xFFu) | e;
		de = sound_403E(de, hl, iy);
		e = (uint8_t)de;
		z80_block(27, 2);
		hl = 0xF000u;
		de = (de & ~0xFFu) | e;
		sound_403E(de, hl, iy);
		z80_block(10, 1);
		return;
	}
	z80_block(19, 3);
	if (1u != unk_E740)
	{
		z80_cycles(6);
		return;
	}
	z80_block(45, 6);
	iy = unk_E700_ADDRESS;
	if (unk_E759 >= 0x40u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(54, 5);
	hl = unk_E900_ADDRESS;
	de = 0xE100u | unk_E758;
	de = sound_42E2(de, hl, iy);
	e = (uint8_t)de;
	z80_block(24, 2);
	de = (de & ~0xFFu) | e;
	sound_403E(de, hl, iy);
	z80_block(10, 1);
}

void sound_3FC2(uint16_t de, uint16_t ix, uint16_t iy, uint16_t *p_hl, uint8_t *p_a)
{
	uint16_t hl = (uint16_t)*p_hl;
	uint8_t a = (uint8_t)*p_a;
	uint8_t a_2 = 0;
	uint16_t hl_2 = 0;
	uint16_t saved_hl = 0, saved_ix = 0;
	uint32_t target1 = 0;

	z80_block(45, 4);
	saved_hl = hl;
	a &= 0xFu;
	hl_2 = table_401E_ADDRESS;
	hl_2 = sound_0366(hl_2, a);
	z80_block(4, 1);
	target1 = hl_2;
	switch (target1)
	{
	case 0x003FCCu:
		z80_block(57, 8);
		hl_2 = z80_read16(saved_hl);
		a_2 = z80_read8(hl_2);
		hl_2++;
		*p_hl = hl_2;
		*p_a = a_2;
		return;
	case 0x003FD4u:
		z80_block(53, 6);
		hl_2 = saved_hl;
		a_2 = (z80_read8((uint16_t)(iy + 9u)) & 7u) | 0x90u;
		*p_hl = hl_2;
		*p_a = a_2;
		return;
	case 0x003FDDu:
		z80_block(53, 6);
		hl_2 = saved_hl;
		a_2 = (z80_read8((uint16_t)(iy + 0xAu)) & 7u) | 0x90u;
		*p_hl = hl_2;
		*p_a = a_2;
		return;
	case 0x003FE6u:
		break;
	case 0x003FF7u:
		z80_block(32, 4);
		if (de >> 8 == 0xE2u)
		{
			z80_block(57, 7);
			a_2 = 0u;
			unk_F100 = 0u;
			move_block(unk_F100_ADDRESS, 0xF101u, 0x7FFu, 1u);
		}
		else
		{
			z80_block(67, 8);
			a_2 = 0u;
			unk_E900 = 0u;
			move_block(unk_E900_ADDRESS, 0xE901u, 0x7FFu, 1u);
		}
		z80_block(30, 3);
		hl_2 = saved_hl;
		*p_hl = hl_2;
		*p_a = a_2;
		return;
	default:
		sound_invalid_jump(target1);
	}
	z80_block(117, 15);
	saved_ix = ix;
	hl_2 = saved_ix;
	a_2 = 0u;
	z80_write8(hl_2, 0u);
	move_block(hl_2, (uint16_t)(hl_2 + 1u), 0x30u, 1u);
	hl_2 = saved_hl;
	*p_hl = hl_2;
	*p_a = a_2;
}

uint16_t sound_403E(uint16_t de, uint16_t hl, uint16_t iy)
{
	bool L_00411B = false, L_00418A = false;
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, c = 0, e = 0;
	uint16_t de_2 = 0, hl_2 = 0, ix = 0;
	uint8_t stack_10001 = 0, saved_e = 0;
	uint16_t stack_2 = 0, saved_ix = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	e = (uint8_t)de;
	z80_block(17, 3);
	if (z80_read8(hl) >> 7 == 0u)
	{
		z80_cycles(6);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(49, 8);
	hl_2 = (hl & ~0xFFu) | 7u;
	c = z80_read8(hl_2);
	hl_2 = (hl_2 & ~0xFFu) | (uint8_t)(hl_2 - 1u);
	b = z80_read8(hl_2);
	t1 = (((uint32_t)b << 8) | c) - 1u;
	b = (uint8_t)((uint16_t)((((uint32_t)b << 8) | c) - 1u) >> 8);
	c = (uint8_t)t1;
	a = 0u;
	if (0u != c)
	{
		z80_block(35, 5);
		hl_2 = (hl_2 & ~0xFFu) | 7u;
		z80_write8(hl_2, c);
		z80_write8((hl_2 & ~0xFFu) | (uint8_t)(hl_2 - 1u), b);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(14, 2);
	if (a == b)
	{
		z80_block(32, 4);
		stack_2 = hl_2 & ~0xFFu;
		ix = stack_2;
		for (;;)
		{
			z80_block(68, 7);
			port_out(0xA0u, z80_read8((uint16_t)(ix + 0x1Bu)));
			hl_2 = z80_read16((uint16_t)(ix + 2u));
			for (;;)
			{
				z80_block(48, 6);
				z80_write8(ix, z80_read8(ix) & 0xBFu);
				a = z80_read8(hl_2);
				if (a >> 7 == 0u)
				{
					z80_block(35, 4);
					a = z80_read8((uint16_t)(ix + 0x14u));
					hl_2--;
				}
				else
				{
					z80_block(19, 2);
					z80_write8((uint16_t)(ix + 0x14u), a);
				}
				z80_block(17, 2);
				if (a == 0xFFu)
				{
					z80_block(17, 1);
					hl_2 = sound_4387(hl_2, ix);
					z80_block(24, 3);
					a = z80_read8(hl_2);
					L_00418A = a == 0x2Fu;
					if (!L_00418A)
					{
						z80_block(17, 2);
						if (a == 0x51u)
						{
							z80_block(17, 1);
							hl_2 = sound_4387(hl_2, ix);
							z80_block(17, 1);
							hl_2 = sound_4387(hl_2, ix);
							z80_block(25, 4);
							a = z80_read8(hl_2);
							if (a >> 7)
							{
								z80_block(23, 5);
								a = (0u - a) | 0x80u;
							}
							z80_block(36, 3);
							z80_write8((uint16_t)(ix + 0xAu), a);
							hl_2 = sound_4387(hl_2, ix);
							z80_block(10, 1);
						}
						else
						{
							z80_block(17, 1);
							hl_2 = sound_4387(hl_2, ix);
							z80_block(7, 1);
							b = z80_read8(hl_2);
							for (;;)
							{
								z80_block(17, 1);
								hl_2 = sound_4387(hl_2, ix);
								z80_block(8, 1);
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(17, 1);
							hl_2 = sound_4387(hl_2, ix);
							z80_block(10, 1);
						}
					}
				}
				else
				{
					z80_block(28, 4);
					c = a;
					a &= ~0xFu;
					if (a == 0xF0u)
					{
						z80_block(21, 2);
						a = c;
						hl_2 = sound_4387(hl_2, ix);
						z80_block(17, 2);
						if (a == 0xF7u)
						{
							z80_block(7, 1);
							b = z80_read8(hl_2);
							for (;;)
							{
								z80_block(24, 2);
								hl_2 = sound_4387(hl_2, ix);
								z80_block(8, 1);
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							z80_block(33, 3);
							z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
						}
						else
						{
							z80_block(17, 2);
							L_00411B = a != 0xF0u;
							if (!L_00411B)
							{
								for (;;)
								{
									z80_block(24, 2);
									a = z80_read8(hl_2);
									hl_2 = sound_4387(hl_2, ix);
									z80_block(17, 2);
									if (a == 0xF7u)
										break;
									z80_block(10, 1);
								}
								z80_block(33, 3);
								z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
							}
						}
					}
					else
					{
						z80_block(17, 2);
						if (a == 0x80u)
						{
							z80_block(32, 4);
							z80_write8((de & ~0xFFu) | e, c);
							e++;
							hl_2 = sound_4387(hl_2, ix);
							z80_block(25, 4);
							a = z80_read8(hl_2);
							if (a >> 7 != 0u)
							{
								z80_cycles(7);
								a = sound_4097(ix, a);
							}
							z80_block(28, 3);
							z80_write8((de & ~0xFFu) | e, a);
							e++;
							hl_2 = sound_4387(hl_2, ix);
							z80_block(51, 6);
							z80_write8((de & ~0xFFu) | e, 0x7Fu);
							e++;
							z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
						}
						else
						{
							z80_block(17, 2);
							if (a == 0xB0u)
							{
								z80_block(32, 4);
								z80_write8((de & ~0xFFu) | e, c);
								e++;
								hl_2 = sound_4387(hl_2, ix);
								z80_block(35, 4);
								a = z80_read8(hl_2);
								z80_write8((de & ~0xFFu) | e, a);
								e++;
								hl_2 = sound_4387(hl_2, ix);
								z80_block(24, 3);
								t2 = a;
								a = z80_read8(hl_2);
								if (t2 == 0x10u)
								{
									z80_block(37, 5);
									c = a;
									if (z80_read8((uint16_t)(ix + 0x18u)) == 0u)
									{
										z80_block(31, 5);
										a = c;
										z80_write8((uint16_t)(ix + 0x15u), c & 0x7Fu);
									}
									else
									{
										z80_block(29, 3);
										a = z80_read8((uint16_t)(ix + 0x15u));
									}
								}
								z80_block(18, 3);
								if (a >> 7 != 0u)
								{
									z80_cycles(7);
									a = sound_4097(ix, a);
								}
								z80_block(28, 3);
								z80_write8((de & ~0xFFu) | e, a);
								e++;
								hl_2 = sound_4387(hl_2, ix);
								z80_block(33, 3);
								z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
							}
							else
							{
								z80_block(17, 2);
								if (a != 0xC0u)
								{
									z80_block(17, 2);
									if (a != 0xD0u)
									{
										z80_block(10, 1);
										z80_block(32, 4);
										z80_write8((de & ~0xFFu) | e, c);
										e++;
										hl_2 = sound_4387(hl_2, ix);
										z80_block(7, 1);
										c = z80_read8(hl_2);
									}
								}
								z80_block(32, 4);
								z80_write8((de & ~0xFFu) | e, c);
								e++;
								hl_2 = sound_4387(hl_2, ix);
								z80_block(25, 4);
								a = z80_read8(hl_2);
								if (a >> 7 != 0u)
								{
									z80_cycles(7);
									a = sound_4097(ix, a);
								}
								z80_block(28, 3);
								z80_write8((de & ~0xFFu) | e, a);
								e++;
								hl_2 = sound_4387(hl_2, ix);
								z80_block(33, 3);
								z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
							}
						}
					}
				}
				if (L_00418A)
					break;
				if (L_00411B)
					break;
				z80_block(30, 3);
				if (((z80_read8(ix) >> 6) & 1u) != 0u)
					continue;
				z80_block(24, 2);
				a = z80_read8(hl_2);
				hl_2 = sound_4387(hl_2, ix);
				z80_block(14, 2);
				if (a != 0u)
					break;
			}
			if (L_00418A || !L_00411B)
			{
				if (!L_00418A)
					break;
			}
			for (;;)
			{
				if (!L_00411B)
				{
					L_00418A = false;
					z80_block(30, 3);
					if (((z80_read8(ix) >> 5) & 1u) == 0u)
					{
						z80_block(30, 3);
						if (((z80_read8(ix) >> 4) & 1u) != 0u)
						{
							z80_cycles(7);
							sound_41D2(iy);
						}
						z80_block(52, 6);
						z80_write8(ix, 0u);
						z80_write8((uint16_t)(iy + 0x58u), e);
						de_2 = (de & ~0xFFu) | e;
						return de_2;
					}
					z80_block(62, 6);
					hl_2 = z80_read16((uint16_t)(ix + 0x20u));
					a = z80_read8(hl_2);
					hl_2 = sound_4387(hl_2, ix);
					z80_block(17, 2);
					if (a >= 0xF0u)
					{
						z80_cycles(7);
						sound_3FC2(de, ix, iy, &hl_2, &a);
					}
					z80_block(14, 2);
					if (a == 0u)
					{
						z80_block(30, 3);
						if (((z80_read8(ix) >> 4) & 1u) != 0u)
						{
							z80_cycles(7);
							sound_41D2(iy);
						}
						z80_block(52, 6);
						z80_write8(ix, 0u);
						z80_write8((uint16_t)(iy + 0x58u), e);
						de_2 = (de & ~0xFFu) | e;
						return de_2;
					}
					z80_block(104, 11);
					z80_write16((uint16_t)(ix + 0x20u), hl_2);
					stack_10001 = de >> 8;
					saved_e = e;
					saved_ix = ix;
					de_2 = saved_ix;
					e = (uint8_t)saved_ix;
					if (((z80_read8(ix) >> 4) & 1u) != 0u)
					{
						z80_block(17, 1);
						de_2 = (de_2 & ~0xFFu) | e;
						sound_4211(de_2, a);
						z80_block(20, 2);
						de = ((uint32_t)stack_10001 << 8) | saved_e;
						e = saved_e;
						break;
					}
					z80_block(17, 1);
					sound_4238(de_2, a);
					z80_block(20, 2);
					de = ((uint32_t)stack_10001 << 8) | saved_e;
					e = saved_e;
					break;
				}
				L_00411B = false;
				z80_block(10, 1);
			}
			continue;
		}
		z80_block(18, 3);
		if (a >> 7)
		{
			z80_block(37, 7);
			a &= 0x7Fu;
			carry = a & 1u;
			c = a >> 1;
			a = z80_read8(hl_2);
			if (carry)
			{
				z80_block(8, 2);
				a |= 0x80u;
			}
			z80_block(17, 1);
			hl_2 = sound_4387(hl_2, ix);
			z80_block(19, 2);
			z80_write8((uint16_t)(ix + 6u), c);
		}
		z80_block(86, 9);
		z80_write8((uint16_t)(ix + 7u), a);
		z80_write16((uint16_t)(ix + 2u), hl_2);
		z80_write8((uint16_t)(iy + 0x58u), e);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(35, 5);
	hl_2 = (hl_2 & ~0xFFu) | 7u;
	z80_write8(hl_2, c);
	z80_write8((hl_2 & ~0xFFu) | (uint8_t)(hl_2 - 1u), b);
	de_2 = (de & ~0xFFu) | e;
	return de_2;
}

uint8_t sound_4097(uint16_t ix, uint8_t a)
{
	z80_block(41, 5);
	a &= 0x7Fu;
	z80_write8(ix, z80_read8(ix) | 0x40u);
	return a;
}

void sound_41D2(uint16_t iy)
{
	z80_block(33, 4);
	z80_write8((uint16_t)(iy + 0xBu), 0u);
}

void sound_41F2(void)
{
	uint8_t a = 0;
	uint16_t de = 0, hl = 0;

	z80_block(31, 4);
	hl = unk_E700_ADDRESS;
	a = unk_E700;
	if (a != 0u)
	{
		z80_block(27, 2);
		de = unk_E900_ADDRESS;
		sound_420F(de, hl, a);
	}
	z80_block(31, 4);
	hl = unk_E800_ADDRESS;
	a = unk_E800;
	if (a != 0u)
	{
		z80_block(27, 2);
		de = unk_F100_ADDRESS;
		sound_420F(de, hl, a);
	}
	z80_block(10, 1);
}

void sound_420F(uint16_t de, uint16_t hl, uint8_t a)
{
	z80_block(10, 1);
	z80_write8(hl, 0u);
	sound_4211(de, a);
}

void sound_4211(uint16_t de, uint8_t a)
{
	uint8_t a_2 = 0;
	uint16_t de_2 = 0, hl = 0;
	uint16_t t1 = 0;

	z80_block(27, 2);
	hl = 0x44DBu;
	hl = sound_0366(hl, a);
	z80_block(29, 5);
	t1 = de;
	de_2 = hl;
	hl = t1;
	a_2 = z80_read8(de_2);
	if (a_2 >> 7 != 0u)
	{
		z80_block(18, 3);
		if (((a_2 >> 6) & 1u) != 0u)
		{
			z80_block(29, 4);
			hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((hl >> 8) + 1u) << 8);
			hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((hl >> 8) + 1u) << 8);
			hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((hl >> 8) + 1u) << 8);
			sound_42C8(&de_2, &hl);
			z80_block(46, 7);
			a_2 = z80_read8(de_2) & 0xFu;
			if (a_2 < (z80_read8(hl) & 0xFu))
			{
				z80_block(10, 1);
				return;
			}
			z80_block(17, 1);
			hl = sound_42D3(hl);
			z80_block(87, 12);
			de_2++;
			z80_write8(hl, a_2);
			hl = (hl & ~0xFFu) | 2u;
			z80_write8(hl, (uint8_t)de_2);
			hl = (hl & ~0xFFu) | (uint8_t)(hl + 1u);
			z80_write8(hl, de_2 >> 8);
			hl = (hl & ~0xFFu) | 7u;
			z80_write8(hl, 1u);
			hl &= ~0xFFu;
			z80_write8(hl, z80_read8(hl) | 0x80u);
			return;
		}
		z80_block(10, 1);
		z80_block(54, 9);
		hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((hl >> 8) + 1u) << 8);
		hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((hl >> 8) + 1u) << 8);
		a_2 = z80_read8(de_2) & 0xFu;
		if (a_2 < (z80_read8(hl) & 0xFu))
		{
			z80_block(10, 1);
			return;
		}
		z80_block(17, 1);
		hl = sound_42D3(hl);
		z80_block(7, 1);
		z80_write8(hl, a_2);
		z80_block(80, 11);
		de_2++;
		hl = (hl & ~0xFFu) | 2u;
		z80_write8(hl, (uint8_t)de_2);
		hl = (hl & ~0xFFu) | (uint8_t)(hl + 1u);
		z80_write8(hl, de_2 >> 8);
		hl = (hl & ~0xFFu) | 7u;
		z80_write8(hl, 1u);
		hl &= ~0xFFu;
		z80_write8(hl, z80_read8(hl) | 0x80u);
		return;
	}
	z80_block(18, 3);
	if (((a_2 >> 6) & 1u) != 0u)
	{
		z80_block(80, 11);
		de_2++;
		hl = (hl & ~0xFFu) | 2u;
		z80_write8(hl, (uint8_t)de_2);
		hl = (hl & ~0xFFu) | (uint8_t)(hl + 1u);
		z80_write8(hl, de_2 >> 8);
		hl = (hl & ~0xFFu) | 7u;
		z80_write8(hl, 1u);
		hl &= ~0xFFu;
		z80_write8(hl, z80_read8(hl) | 0x80u);
		return;
	}
	z80_block(18, 3);
	if (((a_2 >> 5) & 1u) != 0u)
	{
		sound_4256(de_2, hl);
		return;
	}
	z80_block(18, 3);
	if (((a_2 >> 4) & 1u) == 0u)
		z80_block(10, 1);
	z80_block(17, 1);
	hl = sound_42D3(hl);
	z80_block(82, 12);
	de_2++;
	a_2 = z80_read8(de_2);
	z80_write8(hl, z80_read8(hl) | 0x20u);
	de_2++;
	hl = (hl & ~0xFFu) | 0x20u;
	z80_write8(hl, (uint8_t)de_2);
	hl++;
	z80_write8(hl, de_2 >> 8);
	de_2 = hl & ~0xFFu;
	sound_4238(de_2, a_2);
}

void sound_4238(uint16_t de, uint8_t a)
{
	uint16_t de_2 = 0, hl = 0;
	uint16_t t1 = 0;

	z80_block(27, 2);
	hl = 0x439Bu;
	hl = sound_0366(hl, a);
	z80_block(21, 3);
	t1 = de;
	de_2 = hl;
	hl = t1;
	sound_4256(de_2, hl);
}

void sound_4256(uint16_t de, uint16_t hl)
{
	uint8_t a = 0;
	uint16_t hl_2 = 0;

	z80_block(127, 18);
	de++;
	hl_2 = (hl & ~0xFFu) | 0x1Bu;
	a = z80_read8(de);
	de++;
	z80_write8(hl_2, a);
	hl_2 = (hl_2 & ~0xFFu) | 2u;
	z80_write8(hl_2, z80_read8(de));
	hl_2 = (hl_2 & ~0xFFu) | (uint8_t)(hl_2 + 1u);
	z80_write8(hl_2, z80_read8((uint16_t)(de + 1u)));
	hl_2 = (hl_2 & ~0xFFu) | 7u;
	z80_write8(hl_2, 1u);
	hl_2 &= ~0xFFu;
	z80_write8(hl_2, z80_read8(hl_2) | 0x80u);
}

void sound_42C8(uint16_t *p_de, uint16_t *p_hl)
{
	uint16_t de = (uint16_t)*p_de;
	uint16_t hl = (uint16_t)*p_hl;
	uint8_t h = 0;
	uint16_t hl_2 = 0;

	z80_block(55, 9);
	de++;
	h = (z80_read8(de) & 0xFu) - 0xAu + (hl >> 8);
	de--;
	hl_2 = (uint32_t)h << 8;
	*p_de = de;
	*p_hl = (*p_hl & 0xFFu) | hl_2;
}

uint16_t sound_42D3(uint16_t hl)
{
	uint16_t saved_hl = 0;

	z80_block(101, 11);
	saved_hl = hl;
	z80_write8(hl, 0u);
	move_block(hl, (uint16_t)(saved_hl + 1u), 0x30u, 1u);
	hl = (hl + 0x30u) & 0xFF00u;
	return hl;
}

uint16_t sound_42E2(uint16_t de, uint16_t hl, uint16_t iy)
{
	uint8_t e = 0;
	uint16_t de_2 = 0, hl_2 = 0;

	e = (uint8_t)de;
	z80_block(23, 4);
	hl_2 = (hl & ~0xFFu) | 0x18u;
	if (z80_read8(hl_2) == 0u)
	{
		z80_cycles(6);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(28, 4);
	hl_2 = (hl_2 & ~0xFFu) | 0x19u;
	if (z80_read8(hl_2) != 0u)
	{
		z80_block(21, 2);
		z80_write8(hl_2, z80_read8(hl_2) - 1u);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(39, 5);
	hl_2 = (hl_2 & ~0xFFu) | 0x15u;
	z80_write8(hl_2, z80_read8(hl_2) - 1u);
	if (z80_read8(hl_2) == 0u)
	{
		z80_block(223, 31);
		hl_2 &= ~0xFFu;
		z80_write8(hl_2, z80_read8(hl_2) & 0x7Fu);
		z80_write8(hl_2 | 0x18u, 0u);
		z80_write8((de & ~0xFFu) | e, 0xBEu);
		e++;
		z80_write8((de & ~0xFFu) | e, 0x50u);
		e++;
		z80_write8((de & ~0xFFu) | e, 0x7Eu);
		e++;
		z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
		z80_write8((de & ~0xFFu) | e, 0xBEu);
		e++;
		z80_write8((de & ~0xFFu) | e, 0x79u);
		e++;
		z80_write8((de & ~0xFFu) | e, 0u);
		e++;
		z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
		z80_write8((uint16_t)(iy + 0x58u), e);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(134, 18);
	z80_write8((de & ~0xFFu) | e, 0xBEu);
	e++;
	z80_write8((de & ~0xFFu) | e, 0x10u);
	e++;
	z80_write8((de & ~0xFFu) | e, z80_read8(hl_2));
	e++;
	z80_write8((uint16_t)(iy + 0x58u), e);
	z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
	hl_2 = (hl_2 & ~0xFFu) | 0x18u;
	z80_write8((hl_2 & ~0xFFu) | 0x19u, z80_read8(hl_2));
	de_2 = (de & ~0xFFu) | e;
	return de_2;
}

uint16_t sound_433A(uint16_t de, uint16_t hl, uint16_t iy)
{
	uint8_t e = 0;
	uint16_t de_2 = 0, hl_2 = 0;

	e = (uint8_t)de;
	z80_block(23, 4);
	hl_2 = (hl & ~0xFFu) | 0x18u;
	if (z80_read8(hl_2) == 0u)
	{
		z80_cycles(6);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(28, 4);
	hl_2 = (hl_2 & ~0xFFu) | 0x19u;
	if (z80_read8(hl_2) != 0u)
	{
		z80_block(21, 2);
		z80_write8(hl_2, z80_read8(hl_2) - 1u);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(39, 5);
	hl_2 = (hl_2 & ~0xFFu) | 0x1Au;
	z80_write8(hl_2, z80_read8(hl_2) - 1u);
	if (z80_read8(hl_2) == 0u)
	{
		z80_block(138, 19);
		hl_2 &= ~0xFFu;
		z80_write8(hl_2, 0u);
		z80_write8(hl_2 | 0x18u, 0u);
		z80_write8((de & ~0xFFu) | e, 0xBAu);
		e++;
		z80_write8((de & ~0xFFu) | e, 0x7Bu);
		e++;
		z80_write8((de & ~0xFFu) | e, 0u);
		e++;
		z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
		z80_write8((uint16_t)(iy + 0x58u), e);
		de_2 = (de & ~0xFFu) | e;
		return de_2;
	}
	z80_block(134, 18);
	z80_write8((de & ~0xFFu) | e, 0xBAu);
	e++;
	z80_write8((de & ~0xFFu) | e, 7u);
	e++;
	z80_write8((de & ~0xFFu) | e, z80_read8(hl_2));
	e++;
	z80_write8((uint16_t)(iy + 0x58u), e);
	z80_write8((uint16_t)(iy + 0x59u), z80_read8((uint16_t)(iy + 0x59u)) + 1u);
	hl_2 = (hl_2 & ~0xFFu) | 0x18u;
	z80_write8((hl_2 & ~0xFFu) | 0x19u, z80_read8(hl_2));
	de_2 = (de & ~0xFFu) | e;
	return de_2;
}

uint16_t sound_4387(uint16_t hl, uint16_t ix)
{
	z80_block(38, 5);
	hl++;
	if (hl >> 8 == 0xC0u)
	{
		z80_block(60, 6);
		z80_write8((uint16_t)(ix + 0x1Bu), z80_read8((uint16_t)(ix + 0x1Bu)) + 1u);
		port_out(0xA0u, z80_read8((uint16_t)(ix + 0x1Bu)));
		hl = (hl & 0xFFu) | 0xA000u;
	}
	z80_block(20, 2);
	return hl;
}
