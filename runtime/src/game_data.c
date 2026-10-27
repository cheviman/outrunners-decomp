/**
 *    Name:        game_data.c
 *    Description: the ROM set's regions and the board's mirrors of its chips (game/data.h)
 *    Version:     0.2.0
 *    Created:     25-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    For the tools that run the original code (rom_set.c fills the regions): the game has its assets.
 */
#include "game/data.h"

#include <string.h>

uint8_t game_program[GAME_PROGRAM_SIZE];
uint8_t game_tiles[GAME_TILES_SIZE];
uint8_t game_sprites[GAME_SPRITES_SIZE];
uint8_t game_sound[GAME_SOUND_SIZE];
uint8_t game_pcm[GAME_PCM_SIZE];

/* The chips the board decodes more than once (MAME's ROM_START( orunners )): the region, where the chip's
   bytes are, how many, and where they show again */
typedef struct
{
	uint8_t *region;
	uint32_t from, size;
	uint32_t to[3];
} chip_mirror;

static const chip_mirror mirrors[] = {
	{ game_program, 0x000000u, 0x040000u, { 0x040000u, 0x080000u, 0x0C0000u } },    /* the code chips, x4 */
};

void game_data_mirror(void)
{
	size_t mirror, copy;

	for (mirror = 0; mirror < sizeof(mirrors) / sizeof(mirrors[0]); mirror++)
		for (copy = 0; copy < 3 && mirrors[mirror].to[copy] != 0u; copy++)
			memcpy(mirrors[mirror].region + mirrors[mirror].to[copy], mirrors[mirror].region + mirrors[mirror].from,
				mirrors[mirror].size);
}
