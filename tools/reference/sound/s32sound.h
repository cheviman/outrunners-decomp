/**
 *    Name:        s32sound.h
 *    Description: reference System 32 sound board: MAME's Z80 and RF5C68 and ymfm's YM3438 on the board model
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A development tool, never part of the game: the sound the game's own sound board has to make.
 *    s32s_attach() hooks it to the board: the Z80 runs the sound program from the sound ROM, cycle by
 *    cycle, whenever the board's time moves on (s32_board.time_passes), against the board's Z80 side
 *    (runtime/src/sound.c), and the chips make their samples up to each moment something changes them.
 *
 *    Time is the board's (V60 cycles, S32_CPU_CLOCK): a Z80 cycle is two of them. A YM3438 makes a
 *    sample every 144 of its clocks (288 board cycles); the RF5C68 every 384 of its own (50 MHz / 4).
 *    Sample n of a chip is the one it makes from time n x period on: a write at time t changes the
 *    samples from the first one that starts at t or later.
 */
#ifndef S32_REFERENCE_SOUND_H
#define S32_REFERENCE_SOUND_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Hooks the sound board to s32_board; s32_board.sound_rom has to be set */
void s32s_attach(void);

/* Keeps the Z80's time with the timing model of the table at 'path' (tools/decompile_sound.py --timing)
   instead of cycle by cycle; false if the table cannot be read */
bool s32s_use_timing(const char *path);

/* Brings the chips' samples up to 'time' (board cycles) and closes the frame's samples */
void s32s_end_frame(uint64_t time);

/* FNV-1a hashes of the frame just closed: the samples of the three chips, and the sound board's
   memories (the RAM it shares with the V60, which is all the RAM the Z80 has, and the wave memory) */
uint64_t s32s_frame_samples_hash(void);
uint64_t s32s_state_hash(void);

/* The frame's samples, mixed as MAME's routes do (each YM3438 at 0.30, the RF5C68 at 0.40) at the
   YM3438's rate, the RF5C68 held between its samples: left and right interleaved */
const int16_t *s32s_frame_mix(size_t *frames);
uint32_t s32s_mix_rate(void);

/* Every chip write from now on, one line each: board time, chip (fm0, fm1, pcm, pcm memory), offset,
   data */
void s32s_log_writes(FILE *file);

/* The Z80's progress: the cycles it ran */
uint64_t s32s_z80_cycles(void);

/* The Z80 addresses executed so far, a bit each (8 KB, address >> 3, bit address & 7) */
const uint8_t *s32s_executed(void);

#ifdef __cplusplus
}
#endif

#endif
