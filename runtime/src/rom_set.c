/**
 *    Name:        rom_set.c
 *    Description: the player's ROM set into the board's memory (game/data.h), from a .zip or a folder
 *    Version:     0.1.0
 *    Created:     25-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Each of the set's 17 files is known by its CRC32: in a .zip by its own, whatever its name (stored or
 *    deflated: inflate.c); in a folder by MAME's name, or in the folder's orunners.zip. The regions are laid
 *    out as tools/orunners_rom.py lays them, after ROM_START( orunners ) in MAME's src/mame/sega/segas32.cpp
 *    (BSD-3-Clause). The reference runs the original code, so it reads the set when it starts, as MAME does;
 *    the game reads it once, to write its assets pack (extract.c).
 */
#include "game/extract.h"

#include "game/data.h"
#include "inflate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZIP_END             0x06054B50u    /* the end of the central directory */
#define ZIP_ENTRY           0x02014B50u    /* a central directory entry */
#define ZIP_LOCAL           0x04034B50u    /* a local header */
#define ZIP_STORED          0
#define ZIP_DEFLATED        8
#define ZIP_COMMENT_MOST    0xFFFFu
#define SET_ZIP             "orunners.zip"    /* MAME's name for the set's .zip */

typedef struct
{
	const char *name;
	uint32_t crc, size;
} rom_file;

enum
{
	IC37, IC40, IC36, IC39, IC31, IC3, IC11, IC14, IC15, IC10, IC38, IC22, IC23, IC18, IC41, IC1, IC2, FILES
};

static const rom_file files[FILES] = {
	{ "epr-15620.ic37", 0x84f5ad92u, 0x020000u },
	{ "epr-15621.ic40", 0xd98b765au, 0x020000u },
	{ "mpr-15538.ic36", 0x93958820u, 0x080000u },
	{ "mpr-15539.ic39", 0x219760fau, 0x080000u },
	{ "epr-15550.ic31", 0x0205d2edu, 0x080000u },
	{ "mpr-15548.ic3", 0xb6470a66u, 0x200000u },
	{ "mpr-15549.ic11", 0x81d12520u, 0x200000u },
	{ "mpr-15540.ic14", 0xa10d72b4u, 0x200000u },
	{ "mpr-15542.ic15", 0x40952374u, 0x200000u },
	{ "mpr-15544.ic10", 0x39e3df45u, 0x200000u },
	{ "mpr-15546.ic38", 0xe3fcc12cu, 0x200000u },
	{ "mpr-15541.ic22", 0xa2003c2du, 0x200000u },
	{ "mpr-15543.ic23", 0x933e8e7bu, 0x200000u },
	{ "mpr-15545.ic18", 0x53dd0235u, 0x200000u },
	{ "mpr-15547.ic41", 0xedcb2a43u, 0x200000u },
	{ "mpr-15551.ic1", 0x4894bc73u, 0x200000u },
	{ "mpr-15552.ic2", 0x1c4b5e73u, 0x200000u },
};

static uint32_t crc32_of(const uint8_t *data, uint32_t size)
{
	static uint32_t table[256];
	uint32_t crc = 0xFFFFFFFFu, index;
	int bit;

	if (table[1] == 0u)
		for (index = 0; index < 256u; index++)
		{
			table[index] = index;
			for (bit = 0; bit < 8; bit++)
				table[index] = (table[index] >> 1) ^ (0xEDB88320u & (0u - (table[index] & 1u)));
		}
	for (index = 0; index < size; index++)
		crc = (crc >> 8) ^ table[(crc ^ data[index]) & 0xFFu];
	return ~crc;
}

static uint32_t little16(const uint8_t *bytes)
{
	return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8;
}

static uint32_t little32(const uint8_t *bytes)
{
	return little16(bytes) | little16(bytes + 2) << 16;
}

/* A whole file into memory (the caller frees it), or NULL */
static uint8_t *read_whole(const char *path, size_t *size)
{
	FILE *input = fopen(path, "rb");
	uint8_t *data = NULL;
	long length;

	if (!input)
		return NULL;
	if (fseek(input, 0, SEEK_END) == 0 && (length = ftell(input)) > 0 && fseek(input, 0, SEEK_SET) == 0
		&& (data = malloc((size_t)length)) != NULL && fread(data, 1, (size_t)length, input) != (size_t)length)
	{
		free(data);
		data = NULL;
	}
	fclose(input);
	if (data)
		*size = (size_t)length;
	return data;
}

/* The set's files in the .zip 'zip' ('size' bytes) into 'chips', by their CRC32: 0 when it is no .zip */
static int read_zip(const uint8_t *zip, size_t size, uint8_t **chips)
{
	size_t end, entry, local;
	uint32_t entries, index;
	int file;

	if (size < 22)
		return 0;
	for (end = size - 22;; end--)
	{
		if (little32(zip + end) == ZIP_END)
			break;
		if (end == 0 || size - end > 22 + ZIP_COMMENT_MOST)
			return 0;
	}
	entries = little16(zip + end + 10);
	entry = little32(zip + end + 16);
	for (index = 0; index < entries; index++)
	{
		uint32_t method, crc, packed, unpacked;

		if (entry > size || size - entry < 46 || little32(zip + entry) != ZIP_ENTRY)
			return 0;
		method = little16(zip + entry + 10);
		crc = little32(zip + entry + 16);
		packed = little32(zip + entry + 20);
		unpacked = little32(zip + entry + 24);
		local = little32(zip + entry + 42);
		entry += 46 + little16(zip + entry + 28) + little16(zip + entry + 30) + little16(zip + entry + 32);
		for (file = 0; file < FILES; file++)
			if (!chips[file] && files[file].crc == crc && files[file].size == unpacked)
				break;
		if (file == FILES || local > size || size - local < 30 || little32(zip + local) != ZIP_LOCAL)
			continue;
		local += 30 + little16(zip + local + 26) + little16(zip + local + 28);
		if (local > size || packed > size - local || (method == ZIP_STORED && packed != unpacked)
			|| !(chips[file] = malloc(unpacked)))
			continue;    /* (what the entry says does not fit the file) */
		if ((method == ZIP_STORED ? (memcpy(chips[file], zip + local, unpacked), 1)
				: method == ZIP_DEFLATED && inflate_raw(zip + local, packed, chips[file], unpacked))
			&& crc32_of(chips[file], unpacked) == crc)
			continue;
		free(chips[file]);
		chips[file] = NULL;
	}
	return 1;
}

/* The set's files in the folder 'folder' into 'chips', by their MAME names and their CRC32 */
static void read_folder(const char *folder, uint8_t **chips)
{
	char path[1024];
	int file;

	for (file = 0; file < FILES; file++)
	{
		size_t size = 0;

		snprintf(path, sizeof(path), "%s/%s", folder, files[file].name);
		chips[file] = read_whole(path, &size);
		if (chips[file] && (size != files[file].size || crc32_of(chips[file], files[file].size) != files[file].crc))
		{
			free(chips[file]);
			chips[file] = NULL;
		}
	}
}

/* ROM_LOAD16_BYTE: even bytes from one chip, odd bytes from the other */
static void interleave16(uint8_t *out, const uint8_t *even, const uint8_t *odd, uint32_t size)
{
	uint32_t index;

	for (index = 0; index < size; index++)
	{
		out[index * 2u] = even[index];
		out[index * 2u + 1u] = odd[index];
	}
}

/* ROM_LOAD32_WORD: every 32-bit word takes a 16-bit word of each chip, the first chip's first */
static void interleave32(uint8_t *out, const uint8_t *low, const uint8_t *high, uint32_t size)
{
	uint32_t word;

	for (word = 0; word < size / 2u; word++)
	{
		out[word * 4u] = low[word * 2u];
		out[word * 4u + 1u] = low[word * 2u + 1u];
		out[word * 4u + 2u] = high[word * 2u];
		out[word * 4u + 3u] = high[word * 2u + 1u];
	}
}

/* ROM_LOAD64_WORD: four chips' 16-bit words spread over 8-byte groups, from 'base' */
static void sprite_lanes(uint32_t base, uint8_t *const *lanes, uint32_t size)
{
	uint32_t lane, word;

	for (lane = 0; lane < 4u; lane++)
		for (word = 0; word < size / 2u; word++)
		{
			game_sprites[base + word * 8u + lane * 2u] = lanes[lane][word * 2u];
			game_sprites[base + word * 8u + lane * 2u + 1u] = lanes[lane][word * 2u + 1u];
		}
}

const char *game_rom_set_load(const char *source)
{
	static char problem[96];
	uint8_t *chips[FILES] = { NULL };
	size_t size = 0;
	uint8_t *whole = read_whole(source, &size);
	int file;

	if (whole)
	{
		const int zip = read_zip(whole, size, chips);

		free(whole);
		if (!zip)
			return "not a .zip";
	}
	else
	{
		char zip_path[1024];

		read_folder(source, chips);
		/* (a folder that holds the set's .zip, as MAME keeps it) */
		snprintf(zip_path, sizeof(zip_path), "%s/" SET_ZIP, source);
		if ((whole = read_whole(zip_path, &size)) != NULL)
		{
			read_zip(whole, size, chips);
			free(whole);
		}
	}
	problem[0] = '\0';
	for (file = 0; file < FILES && !problem[0]; file++)
		if (!chips[file])
			snprintf(problem, sizeof(problem), "no %s (or not this version of it)", files[file].name);
	if (!problem[0])
	{
		interleave32(game_program, chips[IC37], chips[IC40], 0x20000u);
		game_data_mirror();
		interleave32(game_program + 0x100000u, chips[IC36], chips[IC39], 0x80000u);
		interleave16(game_tiles, chips[IC3], chips[IC11], 0x200000u);
		memset(game_sprites, 0xFF, GAME_SPRITES_SIZE);
		sprite_lanes(0x000000u, &chips[IC14], 0x200000u);
		sprite_lanes(0x800000u, &chips[IC22], 0x200000u);
		memcpy(game_sound, chips[IC31], 0x80000u);
		memcpy(game_pcm, chips[IC1], 0x200000u);
		memcpy(game_pcm + 0x200000u, chips[IC2], 0x200000u);
	}
	for (file = 0; file < FILES; file++)
		free(chips[file]);
	return problem[0] ? problem : NULL;
}
