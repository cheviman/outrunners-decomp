/**
 *    Name:        code.h
 *    Description: interface between the V60 runtime and the C code generated from the game's program
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    tools/recompile.py writes the table: every translated V60 function, sorted by the address it
 *    runs at, so calls through computed addresses (jsr [R0], interrupt vectors) can find their C code.
 */
#ifndef V60_CODE_H
#define V60_CODE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct v60_code_entry
{
	uint32_t address;
	void (*function)(void);
} v60_code_entry;

extern const v60_code_entry v60_code_entries[];
extern const size_t v60_code_entry_count;

#ifdef __cplusplus
}
#endif

#endif
