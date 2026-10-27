/**
 *    Name:        data.h
 *    Description: the ROM set's regions as the board has them, for the tools that run the original code
 *    Version:     0.3.0
 *    Created:     13-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The reference (tools/core/system32/reference) and the translation scaffold run the original code, so
 *    they fill these regions from the player's ROM set when they start (game/extract.h). The game does not:
 *    it loads its assets, by name (game/assets.h), and has no image of the ROM.
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

/* The board's mirrors of its chips: what the regions repeat where the board decodes the same chip again */
void game_data_mirror(void);

#ifdef __cplusplus
}
#endif

#endif
