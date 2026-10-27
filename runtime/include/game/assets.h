/**
 *    Name:        assets.h
 *    Description: the game's assets, loaded from the files the extractor writes, and the data an address the
 *                 game holds reaches
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The game is the decompiled C and these files (extract_assets.py takes them out of the player's ROM
 *    set once): every piece of the program's data from its text file (assets/data), in a block of memory
 *    of its own. The C reads a table by its name (NAME[i]: the pointer game_tables sets); an address the
 *    game keeps in its variables (a pointer into its data) is the identity of a piece, and data_at() finds
 *    the piece it falls in. No image of the ROM is made: the pieces are where the loader put them.
 */
#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <stddef.h>
#include <stdint.h>

#include "s32/video.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A table of the program's data the C reads by name (the decompiler's registry, table.c): its name, its
   address, and the pointer the loader sets to the piece that holds it */
typedef struct
{
	const char *name;
	uint32_t address;
	const void **data;
} game_table;

extern const game_table game_tables[];
extern const size_t game_table_count;

/* Loads the assets in 'folder' (the extractor's assets/) and sets every table's pointer: NULL, or what
   failed, in words for the player */
const char *assets_load(const char *folder);

/* The data at an address of the program's data the game holds (a pointer it keeps): the piece's bytes
   there. The board decodes the program's chips more than once (the code's four times below 0x100000, the
   first MB again from 0xF00000), so those addresses reach the same pieces; an address of the work RAM
   reaches the work RAM, as the bus would send it. Anywhere else, or in code, the program stops. */
const uint8_t *data_at(uint32_t address);

/* The sound program's data at an offset of its chip (512 KB: the Z80 sees the start at 0000-9FFF and the
   rest through its bank's window at A000-BFFF): the piece's bytes there, NULL where no piece is (its code:
   the C replaces it). For the board's sound: s32_board.sound_at */
const uint8_t *assets_sound_data(uint32_t offset);

/* The number of the loaded pieces of the programs' data, and the bytes they hold */
size_t assets_piece_count(void);
size_t assets_data_bytes(void);

/* The board's graphics from the tilesets and the sprite sheets, for the video (s32_video_init_graphics) */
const s32_graphics *assets_graphics(void);

/* A byte of the MultiPCM's 4 MB from the samples and their headers, by its place: 0xFF where no sample is
   (for the board's sound: s32_board.pcm_at) */
uint8_t assets_pcm_byte(uint32_t place);

/* Every loaded piece into 'path' (a development aid: what the loader read, to compare with the ROM set):
   per piece its address and size (32 bits, little endian), then its bytes; false when it cannot be written */
int assets_dump(const char *path);

#ifdef __cplusplus
}
#endif

#endif
