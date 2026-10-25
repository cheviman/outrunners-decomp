/**
 *    Name:        extract.h
 *    Description: the first run: the player's ROM set into the game's assets pack, and the ROM set for the
 *                 reference (rom_set.c, extract.c)
 *    Version:     0.1.0
 *    Created:     25-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The ROM set is MAME's 'orunners' (OutRunners, World), each of its 17 files known by its CRC32: in a
 *    .zip (stored or deflated) or a folder of its files (by MAME's names). The game reads it once, to write
 *    its assets pack: the pieces pack_layout.c lists, taken from where the board's memory has them, as
 *    tools/make_pack.py writes them from the assets. The original code is in none of them.
 */
#ifndef GAME_EXTRACT_H
#define GAME_EXTRACT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A piece of the assets pack, without its bytes (pack_layout.c, tools/make_pack.py --layout) */
typedef struct
{
	uint8_t kind;      /* 0 the bytes of the board's memory there, 1 a fill */
	uint8_t region;    /* 0 program, 1 tiles, 2 sprites, 3 sound, 4 pcm */
	uint8_t fill;
	uint32_t offset, size;
	const char *name;
} pack_piece;

extern const pack_piece pack_pieces[];
extern const uint32_t pack_piece_count;

/* The ROM set at 'source' into game/data.h's regions, laid out as the board has them, the code too (for
   the reference, which runs it, and for extract): NULL, or what is wrong with it */
const char *game_rom_set_load(const char *source);

/* The ROM set at 'source' into the assets pack at 'pack': NULL, or what failed. The regions hold the ROM
   set after it: game_data_load(pack) puts the assets alone in them */
const char *game_data_extract(const char *source, const char *pack);

#ifdef __cplusplus
}
#endif

#endif
