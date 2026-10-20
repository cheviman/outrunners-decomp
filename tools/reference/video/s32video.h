/**
 *    Name:        s32video.h
 *    Description: reference Multi 32 video: MAME's segas32_v.cpp drawing the board model's two screens
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A development tool, never part of the game: the frames the game's own video library has to match.
 *    s32v_attach() hooks it to the board (s32_board.frame_shown and sprites_due): the sprite list is
 *    drawn when the board sends it, and each frame of both screens when VBLANK starts, from the memories
 *    as they are.
 */
#ifndef S32_REFERENCE_VIDEO_H
#define S32_REFERENCE_VIDEO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S32V_MAX_WIDTH  416
#define S32V_HEIGHT     224
#define S32V_SCREENS    2

/* The tile ROM (4 MB) and the sprite ROMs (16 MB, every 32-bit word big endian) as tools/embed_data.py
   writes them; the reference keeps pointers to them */
void s32v_attach(const uint8_t *tiles, size_t tiles_size, const uint8_t *sprites, size_t sprites_size);

/* The last frame drawn on a screen (0 the left one, 1 the right one): 0x00RRGGBB pixels, S32V_MAX_WIDTH
   a row; its width is 320 or 416 */
const uint32_t *s32v_frame(int screen, int *width);

/* FNV-1a hash of the last frame's visible pixels, the left screen's and then the right one's */
uint64_t s32v_frame_hash(void);

#ifdef __cplusplus
}
#endif

#endif
