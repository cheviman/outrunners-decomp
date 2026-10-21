/**
 *    Name:        sound.h
 *    Description: the sound program's ports, variables, tables, structures and functions
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#ifndef ORUNNERS_SOUND_H
#define ORUNNERS_SOUND_H

#include "sound/support.h"

/* the board's I/O ports the program uses */


/* the sound program's variables, in the RAM it shares with the V60 (E000-FFFF) */
#define unk_E000_ADDRESS 0xE000u
#define unk_E000 SOUND_RAM8(unk_E000_ADDRESS)
#define unk_E002_ADDRESS 0xE002u
#define unk_E002 SOUND_RAM8(unk_E002_ADDRESS)
#define unk_E008_ADDRESS 0xE008u
#define unk_E008 SOUND_RAM8(unk_E008_ADDRESS)
#define unk_E00D_ADDRESS 0xE00Du
#define unk_E00D SOUND_RAM8(unk_E00D_ADDRESS)
#define unk_E010_ADDRESS 0xE010u
#define unk_E010 SOUND_RAM8(unk_E010_ADDRESS)
#define unk_E012_ADDRESS 0xE012u
#define unk_E012 SOUND_RAM8(unk_E012_ADDRESS)
#define unk_E013_ADDRESS 0xE013u
#define unk_E013 SOUND_RAM8(unk_E013_ADDRESS)
#define unk_E014_ADDRESS 0xE014u
#define unk_E014 SOUND_RAM16(unk_E014_ADDRESS)
#define unk_E017_ADDRESS 0xE017u
#define unk_E017 SOUND_RAM8(unk_E017_ADDRESS)
#define unk_E018_ADDRESS 0xE018u
#define unk_E018 SOUND_RAM16(unk_E018_ADDRESS)
#define unk_E01A_ADDRESS 0xE01Au
#define unk_E01A SOUND_RAM8(unk_E01A_ADDRESS)
#define unk_E01B_ADDRESS 0xE01Bu
#define unk_E01B SOUND_RAM8(unk_E01B_ADDRESS)
#define unk_E01C_ADDRESS 0xE01Cu
#define unk_E01C SOUND_RAM8(unk_E01C_ADDRESS)
#define unk_E01D_ADDRESS 0xE01Du
#define unk_E01D SOUND_RAM8(unk_E01D_ADDRESS)
#define unk_E01E_ADDRESS 0xE01Eu
#define unk_E01E SOUND_RAM8(unk_E01E_ADDRESS)
#define unk_E01F_ADDRESS 0xE01Fu
#define unk_E01F SOUND_RAM8(unk_E01F_ADDRESS)
#define unk_E028_ADDRESS 0xE028u
#define unk_E028 SOUND_RAM16(unk_E028_ADDRESS)
#define unk_E02A_ADDRESS 0xE02Au
#define unk_E02A SOUND_RAM8(unk_E02A_ADDRESS)
#define unk_E02B_ADDRESS 0xE02Bu
#define unk_E02B SOUND_RAM8(unk_E02B_ADDRESS)
#define unk_E02C_ADDRESS 0xE02Cu
#define unk_E02C SOUND_RAM8(unk_E02C_ADDRESS)
#define unk_E02D_ADDRESS 0xE02Du
#define unk_E02D SOUND_RAM8(unk_E02D_ADDRESS)
#define unk_E060_ADDRESS 0xE060u
#define unk_E060 ((uint8_t *)sound_ram_at(unk_E060_ADDRESS))
#define unk_E082_ADDRESS 0xE082u
#define unk_E082 SOUND_RAM8(unk_E082_ADDRESS)
#define unk_E088_ADDRESS 0xE088u
#define unk_E088 SOUND_RAM8(unk_E088_ADDRESS)
#define unk_E09C_ADDRESS 0xE09Cu
#define unk_E09C SOUND_RAM8(unk_E09C_ADDRESS)
#define unk_E0E0_ADDRESS 0xE0E0u
#define unk_E0E0 ((uint8_t *)sound_ram_at(unk_E0E0_ADDRESS))
#define unk_E700_ADDRESS 0xE700u
#define unk_E700 SOUND_RAM8(unk_E700_ADDRESS)
#define unk_E710_ADDRESS 0xE710u
#define unk_E710 SOUND_RAM8(unk_E710_ADDRESS)
#define unk_E711_ADDRESS 0xE711u
#define unk_E711 SOUND_RAM8(unk_E711_ADDRESS)
#define unk_E712_ADDRESS 0xE712u
#define unk_E712 SOUND_RAM8(unk_E712_ADDRESS)
#define unk_E713_ADDRESS 0xE713u
#define unk_E713 SOUND_RAM8(unk_E713_ADDRESS)
#define unk_E714_ADDRESS 0xE714u
#define unk_E714 SOUND_RAM8(unk_E714_ADDRESS)
#define unk_E718_ADDRESS 0xE718u
#define unk_E718 SOUND_RAM8(unk_E718_ADDRESS)
#define unk_E719_ADDRESS 0xE719u
#define unk_E719 SOUND_RAM8(unk_E719_ADDRESS)
#define unk_E71C_ADDRESS 0xE71Cu
#define unk_E71C SOUND_RAM8(unk_E71C_ADDRESS)
#define unk_E71D_ADDRESS 0xE71Du
#define unk_E71D SOUND_RAM8(unk_E71D_ADDRESS)
#define unk_E740_ADDRESS 0xE740u
#define unk_E740 SOUND_RAM8(unk_E740_ADDRESS)
#define unk_E758_ADDRESS 0xE758u
#define unk_E758 SOUND_RAM8(unk_E758_ADDRESS)
#define unk_E759_ADDRESS 0xE759u
#define unk_E759 SOUND_RAM8(unk_E759_ADDRESS)
#define unk_E780_ADDRESS 0xE780u
#define unk_E780 SOUND_RAM8(unk_E780_ADDRESS)
#define unk_E781_ADDRESS 0xE781u
#define unk_E781 SOUND_RAM8(unk_E781_ADDRESS)
#define unk_E784_ADDRESS 0xE784u
#define unk_E784 SOUND_RAM8(unk_E784_ADDRESS)
#define unk_E785_ADDRESS 0xE785u
#define unk_E785 SOUND_RAM8(unk_E785_ADDRESS)
#define unk_E786_ADDRESS 0xE786u
#define unk_E786 SOUND_RAM8(unk_E786_ADDRESS)
#define unk_E800_ADDRESS 0xE800u
#define unk_E800 SOUND_RAM8(unk_E800_ADDRESS)
#define unk_E810_ADDRESS 0xE810u
#define unk_E810 SOUND_RAM8(unk_E810_ADDRESS)
#define unk_E811_ADDRESS 0xE811u
#define unk_E811 SOUND_RAM8(unk_E811_ADDRESS)
#define unk_E812_ADDRESS 0xE812u
#define unk_E812 SOUND_RAM8(unk_E812_ADDRESS)
#define unk_E813_ADDRESS 0xE813u
#define unk_E813 SOUND_RAM8(unk_E813_ADDRESS)
#define unk_E814_ADDRESS 0xE814u
#define unk_E814 SOUND_RAM8(unk_E814_ADDRESS)
#define unk_E818_ADDRESS 0xE818u
#define unk_E818 SOUND_RAM8(unk_E818_ADDRESS)
#define unk_E819_ADDRESS 0xE819u
#define unk_E819 SOUND_RAM8(unk_E819_ADDRESS)
#define unk_E81C_ADDRESS 0xE81Cu
#define unk_E81C SOUND_RAM8(unk_E81C_ADDRESS)
#define unk_E81D_ADDRESS 0xE81Du
#define unk_E81D SOUND_RAM8(unk_E81D_ADDRESS)
#define unk_E858_ADDRESS 0xE858u
#define unk_E858 SOUND_RAM8(unk_E858_ADDRESS)
#define unk_E859_ADDRESS 0xE859u
#define unk_E859 SOUND_RAM8(unk_E859_ADDRESS)
#define unk_E900_ADDRESS 0xE900u
#define unk_E900 SOUND_RAM8(unk_E900_ADDRESS)
#define unk_F100_ADDRESS 0xF100u
#define unk_F100 SOUND_RAM8(unk_F100_ADDRESS)
#define unk_FFFF_ADDRESS 0xFFFFu
#define unk_FFFF SOUND_RAM8(unk_FFFF_ADDRESS)

/* data in the program ROM the code reads */
#define rom_1F62_ADDRESS 0x1F62u
#define rom_1F62 ((const uint8_t *)sound_rom_at(rom_1F62_ADDRESS))
#define rom_37F3_ADDRESS 0x37F3u
#define rom_37F3 ((const uint8_t *)sound_rom_at(rom_37F3_ADDRESS))

/* the sample bank's window and the RF5C68 (read and written through the bus) */
#define PCM_C000 0xC000u
#define PCM_C001 0xC001u
#define PCM_C002 0xC002u
#define PCM_C009 0xC009u

/* data in the program ROM sound_symbols.txt labels */
#define table_401E_ADDRESS 0x401Eu

/* the sound program's structures in the shared RAM */
#pragma pack(push, 1)
#pragma pack(pop)



/* sound_00.c: the sound program's functions, part 00 (in the order of their Z80 addresses) */
void sound_reset(void);
void sound_irq(void);
void sound_main_loop(void);
void sound_0158(void);
void sound_01B4(void);
uint16_t sound_01ED(void);
uint16_t sound_0212(uint8_t bc, uint8_t a);
void sound_021C(uint8_t bc, uint8_t a);
void sound_0228(uint8_t bc, uint8_t a);
void sound_0292(void);
void sound_02CA(uint8_t bc, uint8_t a);
void sound_0302(uint32_t bc);
void sound_0316(void);
uint16_t sound_0366(uint16_t hl, uint8_t a);
void sound_0372(void);
void sound_03B7(uint8_t bc);
void sound_03D3(uint8_t bc, uint16_t ix);
void sound_06E3(uint16_t iy);
void sound_07BC(uint16_t ix, uint16_t *p_bc, uint16_t *p_hl);
void sound_0A39(uint16_t *p_bc, uint8_t *p_a);
void sound_0A43(void);
uint16_t sound_0B77(uint16_t de, uint16_t hl);
uint16_t sound_0BA6(void);
void sound_0BAD(void);
void sound_0BF2(uint8_t bc);
void sound_0C0E(uint8_t bc, uint16_t ix);
void sound_0F20(uint16_t iy);
void sound_0FF8(uint16_t ix, uint16_t *p_bc, uint16_t *p_hl);
void sound_127B(uint16_t *p_bc, uint8_t *p_a);
void sound_12E0(uint8_t bc, uint16_t de, uint16_t hl);
void sound_13D8(uint16_t iy, uint8_t a);
void sound_1449(uint16_t iy);
void sound_145A(uint8_t bc, uint16_t hl, uint16_t iy);
void sound_14A8(uint8_t bc, uint16_t hl, uint16_t ix);
void sound_14F6(uint8_t a, uint16_t *p_bc, uint16_t *p_de, uint16_t *p_hl);
void sound_1515(uint8_t a, uint16_t *p_bc, uint16_t *p_de, uint16_t *p_hl);
void sound_36AD(void);
void sound_36F6(void);
void sound_3748(uint8_t de);
void sound_376D(uint16_t de);
void sound_37B5(uint8_t a);
void sound_3AF3(void);
void sound_3DE6(void);
void sound_3E36(void);
void sound_3E86(uint8_t de, uint16_t ix, uint16_t iy);
void sound_3EED(void);
void sound_3FC2(uint16_t de, uint16_t ix, uint16_t iy, uint16_t *p_hl, uint8_t *p_a);
uint16_t sound_403E(uint16_t de, uint16_t hl, uint16_t iy);
uint8_t sound_4097(uint16_t ix, uint8_t a);
void sound_41D2(uint16_t iy);
void sound_41F2(void);
void sound_420F(uint16_t de, uint16_t hl, uint8_t a);
void sound_4211(uint16_t de, uint8_t a);
void sound_4238(uint16_t de, uint8_t a);
void sound_4256(uint16_t de, uint16_t hl);
void sound_42C8(uint16_t *p_de, uint16_t *p_hl);
uint16_t sound_42D3(uint16_t hl);
uint16_t sound_42E2(uint16_t de, uint16_t hl, uint16_t iy);
uint16_t sound_433A(uint16_t de, uint16_t hl, uint16_t iy);
uint16_t sound_4387(uint16_t hl, uint16_t ix);

#endif
