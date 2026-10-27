/**
 *    Name:        extract.h
 *    Description: the player's ROM set for the tools that run the original code: the reference and the
 *                 translation scaffold (rom_set.c)
 *    Version:     0.2.0
 *    Created:     25-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The ROM set is MAME's 'orunners' (OutRunners, World), each of its 17 files known by its CRC32: in a
 *    .zip (stored or deflated) or a folder of its files (by MAME's names). The game itself never reads it:
 *    it loads the assets extract_assets.py takes out of it (game/assets.h).
 */
#ifndef GAME_EXTRACT_H
#define GAME_EXTRACT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The ROM set at 'source' into game/data.h's regions, laid out as the board has them, the code too (for
   the reference and the scaffold, which run it): NULL, or what is wrong with it */
const char *game_rom_set_load(const char *source);

#ifdef __cplusplus
}
#endif

#endif
