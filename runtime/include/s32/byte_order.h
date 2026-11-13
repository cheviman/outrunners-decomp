/**
 *    Name:        byte_order.h
 *    Description: the work RAM and the RAM the sound program shares as the V70 holds them, on any processor:
 *                 a check of the C on one unlike the PC's
 *    Version:     0.1.0
 *    Created:     13-11-2026
 *    Modified:    13-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The two memories keep each value in the host's order at the width the C writes it (s32/machine.h), so a
 *    big-endian processor holds a halfword's bytes the other way round from the V70, and which bytes are one
 *    value's depends on what the C last did there (the families of objects lay their fields out over the same
 *    bytes at other widths). The state's hashes are of the memory as the V70 holds it. Built with
 *    S32_BYTE_ORDER_CHECK (CMake: ORUNNERS_BYTE_ORDER_CHECK, GCC), the compiler calls byte_order.c before every
 *    read and write of the game's C and of the sound program's (src/), and the board calls it at its own (the
 *    bus, the moves of strings and blocks), and it keeps, for every byte of the two memories, the place in the
 *    V70's memory of the byte it holds: a value the C works out and writes, its bytes in the host's order at
 *    the width it is written; one it writes as it read it, at the width it read it, the places the bytes had
 *    there (the program's bytes that it copies a halfword or a word at a time keep their own); the bytes a move
 *    takes along, theirs. s32_order_v70 lays a memory out by those places, as the V70 holds it, which the
 *    state's hashes and the headless game's dumps take. On a little-endian processor each byte's place is its
 *    own. Without S32_BYTE_ORDER_CHECK the calls are nothing and the memories are hashed as they are
 */
#ifndef S32_BYTE_ORDER_H
#define S32_BYTE_ORDER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef S32_BYTE_ORDER_CHECK
/* the board wrote a value of 'width' bytes there, in the host's order */
void s32_order_stored(const void *place, size_t width);
/* the board read a value of 'width' bytes there (a write of the same bytes after it is a copy of them) */
void s32_order_loaded(const void *place, size_t width);
/* 'size' bytes written there each at its own place (a clear, the bytes of a file) */
void s32_order_bytes(const void *place, size_t size);
/* 'size' bytes moved as they are from 'from' (anywhere) to 'to' */
void s32_order_moved(const void *to, const void *from, size_t size);
/* 'size' bytes of the two memories at 'from' moved through the bus to 'address' of the video RAM or the sprite
   RAM (the memory test keeps the work RAM and the sound board's RAM there while it tests them, a halfword at a
   time: the processor's halfword of what the C keeps there, which is the V70's only where it is one value) */
void s32_order_out(const void *from, uint32_t address, size_t size);
/* 'size' bytes moved through the bus from 'address' of the video RAM or the sprite RAM to 'to': the bytes
   s32_order_out took there come back with their places */
void s32_order_in(const void *to, uint32_t address, size_t size);
/* 'size' bytes written there whose places the caller knows (from the program's data, as the loader laid it
   out: how far from each the V70's byte it holds is) */
void s32_order_placed(const void *place, const int8_t *places, size_t size);
/* the work RAM or the RAM the sound program shares (s32_board's) as the V70 holds it */
const uint8_t *s32_order_v70(const uint8_t *memory);
/* the video RAM or the sprite RAM (s32_board's, 16-bit words in the host's order), with the words that still
   hold what s32_order_out moved there as the V70's bytes moved: the memory test's copies, raw memory that the
   C holds in its own way */
const uint8_t *s32_order_words(const uint8_t *memory);
#else
#define s32_order_stored(place, width) ((void)0)
#define s32_order_loaded(place, width) ((void)0)
#define s32_order_bytes(place, size) ((void)0)
#define s32_order_moved(to, from, size) ((void)0)
#define s32_order_placed(place, places, size) ((void)0)
#define s32_order_out(from, address, size) ((void)0)
#define s32_order_in(to, address, size) ((void)0)
#define s32_order_v70(memory) ((const uint8_t *)(memory))
#define s32_order_words(memory) ((const uint8_t *)(memory))
#endif

#ifdef __cplusplus
}
#endif

#endif
