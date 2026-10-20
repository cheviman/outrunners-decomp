/**
 *    Name:        internal.h
 *    Description: functions shared between the files of the Multi 32 machine model
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef S32_INTERNAL_H
#define S32_INTERNAL_H

#include "s32/machine.h"

/* I/O areas at 0xC00000 (chip 0) and 0xC80000 (chip 1), each mirrored every 0x80 bytes: a 315-5296 at
   0x00-0x1F; next to chip 0, the A/D converter at 0x50-0x57 and its input bank at 0x60. 'offset' is
   the byte address inside the 0x80-byte window; devices sit on the low byte lane. */
uint8_t s32_io_read(int chip, uint32_t offset);
void s32_io_write(int chip, uint32_t offset, uint8_t data);
void s32_io_reset(void);

void s32_log(const char *format, ...);

#endif
