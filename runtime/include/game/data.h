/**
 *    Name:        data.h
 *    Description: the game's data the executable carries: the board's ROM regions
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The arrays are written at build time by tools/embed_data.py from the player's ROM set (rom/), so
 *    the executables need no file to run, wherever they are copied. Their bytes are the regions as the
 *    board's chips hold them, laid out as MAME's ROM_START( orunners ) loads them.
 */
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GAME_PROGRAM_ROM_SIZE   0x200000     /* the V70's program and data, as it sees them at 000000 */
#define GAME_TILE_ROM_SIZE      0x400000     /* 16x16 tiles at 4 bits a pixel */
#define GAME_SPRITE_ROM_SIZE    0x1000000    /* sprite pixels, every 32-bit word big endian */
#define GAME_SOUND_ROM_SIZE     0x80000      /* the Z80 program */
#define GAME_PCM_ROM_SIZE       0x400000     /* the MultiPCM's samples */

extern const uint8_t game_program_rom[GAME_PROGRAM_ROM_SIZE];
extern const uint8_t game_tile_rom[GAME_TILE_ROM_SIZE];
extern const uint8_t game_sprite_rom[GAME_SPRITE_ROM_SIZE];
extern const uint8_t game_sound_rom[GAME_SOUND_ROM_SIZE];
extern const uint8_t game_pcm_rom[GAME_PCM_ROM_SIZE];

#ifdef __cplusplus
}
#endif

#endif
