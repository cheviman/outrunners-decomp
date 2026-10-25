/**
 *    Name:        game_data.c
 *    Description: loads the game's assets pack into the board's memory (game/data.h)
 *    Version:     0.1.0
 *    Created:     25-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The pack's format is tools/make_pack.py's: "ORAP", u16 version, u16 0, u32 entries; each entry a u8
 *    kind (0 bytes, 1 fill), u8 region, u16 name length, u32 offset, u32 size, the name, then the bytes or
 *    the fill byte; all little endian.
 */
#include "game/data.h"

#include <stdio.h>
#include <string.h>

#define PACK_VERSION   1u
#define KIND_BYTES     0u
#define KIND_FILL      1u
#define REGIONS        5u

uint8_t game_program[GAME_PROGRAM_SIZE];
uint8_t game_tiles[GAME_TILES_SIZE];
uint8_t game_sprites[GAME_SPRITES_SIZE];
uint8_t game_sound[GAME_SOUND_SIZE];
uint8_t game_pcm[GAME_PCM_SIZE];

static uint8_t *const regions[REGIONS] = { game_program, game_tiles, game_sprites, game_sound, game_pcm };
static const uint32_t region_sizes[REGIONS] = { GAME_PROGRAM_SIZE, GAME_TILES_SIZE, GAME_SPRITES_SIZE,
	GAME_SOUND_SIZE, GAME_PCM_SIZE };

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

static uint32_t little16(const uint8_t *bytes)
{
	return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8;
}

static uint32_t little32(const uint8_t *bytes)
{
	return little16(bytes) | little16(bytes + 2) << 16;
}

void game_data_mirror(void)
{
	size_t mirror, copy;

	for (mirror = 0; mirror < sizeof(mirrors) / sizeof(mirrors[0]); mirror++)
		for (copy = 0; copy < 3 && mirrors[mirror].to[copy] != 0u; copy++)
			memcpy(mirrors[mirror].region + mirrors[mirror].to[copy], mirrors[mirror].region + mirrors[mirror].from,
				mirrors[mirror].size);
}

/* The entries of the pack at 'input', each where it goes; false when the pack is cut short or one is out
   of its region */
static int read_entries(FILE *input, uint32_t entries)
{
	uint8_t head[12];
	uint32_t entry;

	for (entry = 0; entry < entries; entry++)
	{
		uint32_t kind, region, offset, size;
		int fill;

		if (fread(head, 1, sizeof(head), input) != sizeof(head))
			return 0;
		kind = head[0];
		region = head[1];
		offset = little32(head + 4);
		size = little32(head + 8);
		if (region >= REGIONS || offset > region_sizes[region] || size > region_sizes[region] - offset
			|| fseek(input, (long)little16(head + 2), SEEK_CUR) != 0)
			return 0;
		if (kind == KIND_FILL)
		{
			if ((fill = fgetc(input)) == EOF)
				return 0;
			memset(regions[region] + offset, fill, size);
		}
		else if (kind != KIND_BYTES || fread(regions[region] + offset, 1, size, input) != size)
			return 0;
	}
	return 1;
}

game_data_result game_data_load(const char *path)
{
	FILE *input = fopen(path, "rb");
	uint8_t head[12];
	int read;
	size_t region;

	if (!input)
		return GAME_DATA_MISSING;
	for (region = 0; region < REGIONS; region++)
		memset(regions[region], 0, region_sizes[region]);
	read = fread(head, 1, sizeof(head), input) == sizeof(head) && memcmp(head, "ORAP", 4) == 0
		&& little16(head + 4) == PACK_VERSION && read_entries(input, little32(head + 8));
	fclose(input);
	if (!read)
		return GAME_DATA_DAMAGED;
	game_data_mirror();
	return GAME_DATA_LOADED;
}
