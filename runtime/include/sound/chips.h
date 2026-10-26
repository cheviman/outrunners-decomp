/**
 *    Name:        chips.h
 *    Description: the sound board's chips and the Z80 program on the board's time (sound_chips.c, sound_cpu.c)
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A port sets s32_board.sound_rom and s32_board.pcm_rom, calls sound_chips_attach() and
 *    sound_cpu_attach(), and at the end of every frame sound_chips_end_frame() with the board's time: the
 *    frame's samples are then there to play, each chip at its own rate (the YM3438 every 360 board cycles,
 *    the MultiPCM every 448).
 */
#ifndef SOUND_CHIPS_H
#define SOUND_CHIPS_H

#include "s32/machine.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The YM3438 and the MultiPCM on s32_board.chips */
void sound_chips_attach(void);

/* The decompiled sound program on the board's time (sound_cpu.c) */
void sound_cpu_attach(void);

/* The YM3438 timers due by 'time' run out (sound_cpu.c calls it where a block starts) */
void sound_chips_timers_due(uint64_t time);

/* The MultiPCM makes the samples due by now, before the board changes its banks */
void sound_chips_pcm_banks_changing(void);

/* The frame ends at 'time': the samples that start before it are the frame's */
void sound_chips_end_frame(uint64_t time);

/* The frame's samples of chip 0 (the YM3438) or 1 (the MultiPCM), its two outputs interleaved */
const int32_t *sound_chips_frame_samples(int chip, size_t *count);

/* The frame's sound, mixed as tools/core/system32/reference/sound mixes it: stereo at the YM3438's rate */
const int16_t *sound_chips_frame_mix(size_t *frames);
uint32_t sound_chips_mix_rate(void);

/* FNV-1a hashes, as tools/core/system32/reference/s32ref writes them: the frame's samples of the two chips, and the
   sound board's memory (the shared RAM but the Z80's stack, FD00-FDFF) */
uint64_t sound_chips_frame_hash(void);
uint64_t sound_board_state_hash(void);

#ifdef __cplusplus
}
#endif

#endif
