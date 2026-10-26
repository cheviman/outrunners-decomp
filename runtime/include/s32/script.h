/**
 *    Name:        script.h
 *    Description: scripted controls for test runs: coins, starts, buttons, wheels and pedals, DIP
 *                 switches and memory pokes, applied when VBLANK starts in the frames a text script names
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A run that applies the same script at the same frame boundaries sees the same inputs, so the
 *    translated game and tools/core/system32/reference/s32ref stay comparable frame by frame.
 *
 *    Script format, one event per line, '#' starts a comment:
 *      FRAME ACTION [ARGUMENTS]        FRAME is a frame number, or +N for N frames after the previous event
 *      press INPUT [FRAMES]            pressed for FRAMES frames (4 by default), then released
 *      hold INPUT / release INPUT      pressed / released until another event changes it
 *      wheel PLAYER POSITION           player 1-2's wheel at POSITION (0-255, 128 the centre) from now on
 *      gas PLAYER POSITION             the gas pedal (0 up, 255 down)
 *      brake PLAYER POSITION           the brake pedal
 *      dip SWITCH on|off               DIP switch 1-4
 *      poke ADDRESS VALUE              writes a byte through the bus (hexadecimal address and value)
 *      end                             the run ends with this frame
 *    INPUT: coin1 coin2 start1 start2 service test service2 test2, and for players 1 and 2 shift_upN
 *           shift_downN musicN previousN nextN (the DJ/music, << and >> buttons)
 */
#ifndef S32_SCRIPT_H
#define S32_SCRIPT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s32_script s32_script;

/* NULL if the script cannot be read or parsed; 'error' then says what and where */
s32_script *s32_script_load(const char *path, char *error, size_t error_size);
void s32_script_free(s32_script *script);

/* Applies what is due in 'frame' (counted from 0): call it from s32_board.controls_update, which runs
   when the frame's VBLANK starts, the moment MAME polls the controls */
void s32_script_apply(s32_script *script, uint64_t frame);

/* Frames the script asks to run ('end'), or 0 if it does not say */
uint64_t s32_script_frames(const s32_script *script);

#ifdef __cplusplus
}
#endif

#endif
