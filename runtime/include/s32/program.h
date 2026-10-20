/**
 *    Name:        program.h
 *    Description: what a port needs from the game program: start it, wire its interrupt line, hear about
 *                 every frame it finishes
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Two builds provide it: the decompiled game (runtime/src/game.c with the C of tools/decompile.py)
 *    and the translation scaffold (runtime/src/v60.c with the C of tools/recompile.py). The program
 *    never returns on its own: the port stops it from program_frame_done (program_stop) when it has run
 *    the frames it wanted.
 */
#ifndef S32_PROGRAM_H
#define S32_PROGRAM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Puts the program in its power-on state; the board is set up apart (s32_init) */
void program_reset(void);

/* Runs the program from its reset vector, until program_stop */
void program_run(void);

/* From program_frame_done: program_run returns once the callback is done. The program's tasks are left
   where they are (their stacks are not freed): a port stops it to quit */
void program_stop(void);

/* The board's interrupt line (s32_board.irq_changed) */
void program_irq_line(bool asserted);

uint64_t program_frames(void);          /* frames the program finished */
uint32_t program_interrupts(void);      /* interrupts it took */

/* Called each time the program finishes a frame */
extern void (*program_frame_done)(void);

#ifdef __cplusplus
}
#endif

#endif
