/**
 *    Name:        data.h
 *    Description: the game's data in the board's memory, loaded from its assets pack
 *    Version:     0.2.0
 *    Created:     13-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    No executable carries the ROM or anything from it. game_data_load fills the regions from the assets
 *    pack (tools/make_pack.py writes it from the assets extract_assets.py takes out of the player's ROM
 *    set): every asset where the board's memory has it, then the board's mirrors of its chips. The original
 *    code is in no region: the C replaces it. The reference (tools/reference) and the translation scaffold
 *    run the original code, so they fill the regions from the ROM set when they start (game/extract.h).
 */
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GAME_PROGRAM_SIZE   0x200000     /* the V70's program space, as it sees it at 000000: the data */
#define GAME_TILES_SIZE     0x400000     /* 16x16 tiles at 4 bits a pixel */
#define GAME_SPRITES_SIZE   0x1000000    /* sprite pixels, every 32-bit word big endian */
#define GAME_SOUND_SIZE     0x80000      /* the Z80's program space: its data */
#define GAME_PCM_SIZE       0x400000     /* the MultiPCM's sample headers and samples */

extern uint8_t game_program[GAME_PROGRAM_SIZE];
extern uint8_t game_tiles[GAME_TILES_SIZE];
extern uint8_t game_sprites[GAME_SPRITES_SIZE];
extern uint8_t game_sound[GAME_SOUND_SIZE];
extern uint8_t game_pcm[GAME_PCM_SIZE];

typedef enum
{
	GAME_DATA_LOADED,
	GAME_DATA_MISSING,    /* no pack at the path */
	GAME_DATA_DAMAGED     /* a pack the game cannot read: another version, cut short, or out of bounds */
} game_data_result;

game_data_result game_data_load(const char *path);

/* The board's mirrors of its chips: what the regions repeat where the board decodes the same chip again */
void game_data_mirror(void);

#ifdef __cplusplus
}
#endif

#endif
