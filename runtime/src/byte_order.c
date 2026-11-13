/**
 *    Name:        byte_order.c
 *    Description: where each byte of the work RAM and of the RAM the sound program shares is in the V70's
 *                 order (s32/byte_order.h), from every read and write the C makes
 *    Version:     0.1.0
 *    Created:     13-11-2026
 *    Modified:    13-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Built with S32_BYTE_ORDER_CHECK only. The compiler's calls are ThreadSanitizer's (-fsanitize=thread on the
 *    decompiled C: CMake's ORUNNERS_BYTE_ORDER_CHECK), one before every read and every write with its address
 *    and width, the write of an x++ too (AddressSanitizer's checks a place once in a block, its read): here they
 *    keep the bytes' places, and check nothing; its library is not linked.
 *
 *    A byte's place is where the byte of the V70's memory it holds is, as how far from it: a value's bytes are
 *    the other way round on a big-endian processor, so its places say so (a halfword's +1 and -1), and a place
 *    always points at a byte of the same value, whose place points back. A write's call comes before the write,
 *    so it is settled at the next call (or when the board asks), with the bytes written:
 *    - when they are the bytes of a read of the same width a little before (the reads kept, READS) that were one
 *      value, or bytes of their own, the write is those bytes copied (a halfword read and written again, the
 *      program's bytes moved a halfword or a word at a time), and keeps their places;
 *    - when they are the bytes read there before the writes since (SAVED: the memory test writes its patterns
 *      over a word and puts back what it read), they are put back, with their places;
 *    - otherwise it is a value of its width in the host's order.
 *    A write over part of a wider value leaves the value's other bytes as bytes of their own: what the V70 holds
 *    there if the C did what the V70 does (a byte written into a halfword the C keeps whole would be the other
 *    byte on a big-endian processor, which the state's hash then shows). The bytes the memory test moves out to
 *    the sprite RAM, a halfword at a time through the bus, and back keep their places too (s32_order_out,
 *    s32_order_in), and while they are there they are hashed as the V70's bytes it moved (s32_order_words).
 *
 *    S32_BYTE_ORDER_WATCH=ADDRESS (hexadecimal, of the work RAM or of the sound board's RAM as the V70 sees it)
 *    says on stderr how each write over that byte placed it, and from where in the program it came
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s32/byte_order.h"
#include "s32/machine.h"

#define WIDEST 8       /* the widest value: wider writes are blocks of bytes */
#define READS 4        /* the reads kept: a copy's write comes right after its read, or a few after (reads put
                          together before their writes); further back a value's bytes are like a read's by chance */
#define SAVED 1024     /* the reads kept by where they read, the last one before a write there */

struct read
{
	const uint8_t *place;
	size_t width;
	uint8_t bytes[WIDEST];
	int8_t places[WIDEST];
	int written;    /* (a saved read: written over since) */
};

static int8_t work_places[S32_WORK_RAM_SIZE];
static int8_t shared_places[S32_SHARED_RAM_SIZE];
static uint8_t work_image[S32_WORK_RAM_SIZE];
static uint8_t shared_image[S32_SHARED_RAM_SIZE];

static struct read reads[READS];
static unsigned next_read;
static struct read saved[SAVED];
/* what s32_order_out took to the video RAM [0] and the sprite RAM [1]: the bytes, their places, and whether
   each is there */
static uint8_t out_bytes[2][S32_VIDEO_RAM_SIZE > S32_SPRITE_RAM_SIZE ? S32_VIDEO_RAM_SIZE : S32_SPRITE_RAM_SIZE];
static int8_t out_places[2][sizeof(out_bytes[0])];
static uint8_t out_kept[2][sizeof(out_bytes[0])];
static uint8_t out_v70[2][sizeof(out_bytes[0])];      /* ... the V70's byte at each address, as it moved them */
static uint8_t out_written[2][sizeof(out_bytes[0])];  /* ... the host's byte the move left at the address's lane */
static uint8_t word_image[sizeof(out_bytes[0])];
static const uint8_t *written;    /* the write the compiler's last call came before, not settled yet */
static size_t written_width;
static const void *written_by;    /* where in the program that call came from */
static int blocks_said;
static int watching = -1;         /* S32_BYTE_ORDER_WATCH: 0 none, 1 an address (watched) */
static uint32_t watched;
static const struct read *copied; /* the read a write was taken to copy, for watch() */
static unsigned copied_back;

/* the places of the bytes at 'place', when all 'width' of them are in one of the two memories; with 'memory'
   the places of the whole memory and 'size' its size */
static int8_t *places_of(const void *place, size_t width, int8_t **memory, size_t *size)
{
	const uintptr_t at = (uintptr_t)place;
	const uintptr_t work = (uintptr_t)s32_board.work_ram;
	const uintptr_t shared = (uintptr_t)s32_board.shared_ram;

	if (work && at >= work && at - work + width <= S32_WORK_RAM_SIZE)
	{
		*memory = work_places;
		*size = S32_WORK_RAM_SIZE;
		return &work_places[at - work];
	}
	if (at >= shared && at - shared + width <= S32_SHARED_RAM_SIZE)
	{
		*memory = shared_places;
		*size = S32_SHARED_RAM_SIZE;
		return &shared_places[at - shared];
	}
	return NULL;
}

/* a value of 'width' bytes in the host's order: on a big-endian processor its first byte is the V70's last */
static void value_places(int8_t *places, size_t width)
{
	for (size_t index = 0; index < width; index++)
		places[index] = S32_HOST_BIG_ENDIAN ? (int8_t)((int)width - 1 - 2 * (int)index) : 0;
}

/* true when 'places' keep each of 'width' bytes inside them: one value, or bytes of their own */
static int whole(const int8_t *places, size_t width)
{
	for (size_t index = 0; index < width; index++)
		if ((int)index + places[index] < 0 || (int)index + places[index] >= (int)width)
			return 0;
	return 1;
}

static void watch(const int8_t *memory, const int8_t *places, size_t width, const int8_t *new_places,
                  const char *how, const void *by)
{
	const uint32_t base = memory == work_places ? 0x200000u : 0x700000u;
	const uint32_t address = base + (uint32_t)(places - memory);

	if (watching < 0)
	{
		const char *text = getenv("S32_BYTE_ORDER_WATCH");
		watching = text && *text;
		watched = watching ? (uint32_t)strtoul(text, NULL, 16) : 0;
	}
	if (!watching || watched < address || watched >= address + width)
		return;
	fprintf(stderr, "byte order: frame %llu, %u bytes at %06X %s (from %p)", (unsigned long long)s32_board.frame,
		(unsigned)width, (unsigned)address, how, by);
	if (copied)
	{
		const uintptr_t from = (uintptr_t)copied->place;
		const uintptr_t work = (uintptr_t)s32_board.work_ram;
		fprintf(stderr, " read %u reads back at %06X", copied_back, (unsigned)(from - work < S32_WORK_RAM_SIZE
			? 0x200000u + (from - work) : 0x700000u + (from - (uintptr_t)s32_board.shared_ram)));
	}
	fprintf(stderr, ":");
	for (size_t index = 0; index < width; index++)
		fprintf(stderr, " %+d", new_places[index]);
	fprintf(stderr, "\n");
}

/* 'width' bytes at 'places' (of 'memory', 'size' bytes) take 'new_places': the bytes of a value they leave
   become bytes of their own */
static void place(int8_t *memory, size_t size, int8_t *places, size_t width, const int8_t *new_places,
                  const char *how, const void *by)
{
	const size_t first = (size_t)(places - memory);

	watch(memory, places, width, new_places, how, by);
	for (size_t index = 0; index < width; index++)
	{
		const ptrdiff_t old = (ptrdiff_t)(first + index) + places[index];
		const ptrdiff_t now = (ptrdiff_t)(first + index) + new_places[index];
		if (old != now && (old < (ptrdiff_t)first || old >= (ptrdiff_t)(first + width)) && old >= 0
			&& (size_t)old < size && old + memory[old] == (ptrdiff_t)(first + index))
			memory[old] = 0;
	}
	memmove(places, new_places, width);
}

/* a write at 'at' other than the bytes read there put back */
static void saved_written(const uint8_t *at)
{
	struct read *before = &saved[((uintptr_t)at >> 1) % SAVED];
	if (before->place == at)
		before->written = 1;
}

static void settle(void)
{
	const uint8_t *at = written;
	const size_t width = written_width;
	int8_t *memory, *places;
	int8_t new_places[WIDEST];
	size_t size;

	written = NULL;
	if (!at || !(places = places_of(at, width, &memory, &size)))
		return;
	if (width > WIDEST)
	{
		if (!blocks_said++)
			fprintf(stderr, "byte order: a write of %u bytes at once, taken as bytes\n", (unsigned)width);
		for (size_t index = 0; index < width; index += WIDEST)
		{
			const size_t part = width - index < WIDEST ? width - index : WIDEST;
			memset(new_places, 0, part);
			place(memory, size, places + index, part, new_places, "as bytes", written_by);
		}
		return;
	}
	for (unsigned back = 1; back <= READS; back++)
	{
		const struct read *read = &reads[(next_read - back) % READS];
		if (read->width == width && memcmp(read->bytes, at, width) == 0)
		{
			copied = read;
			copied_back = back;
			place(memory, size, places, width, read->places, "as a read's bytes", written_by);
			copied = NULL;
			saved_written(at);
			return;
		}
	}
	{
		struct read *before = &saved[((uintptr_t)at >> 1) % SAVED];
		if (before->place == at && before->width == width && before->written && memcmp(before->bytes, at, width) == 0)
		{
			copied = before;
			copied_back = 0;
			place(memory, size, places, width, before->places, "as the bytes read there before", written_by);
			copied = NULL;
			before->written = 0;
			return;
		}
	}
	value_places(new_places, width);
	place(memory, size, places, width, new_places, "as a value", written_by);
	saved_written(at);
}

void s32_order_loaded(const void *at, size_t width)
{
	int8_t *memory;
	const int8_t *places;
	struct read *read, *before;
	size_t size;

	settle();
	if (width > WIDEST || !(places = places_of(at, width, &memory, &size)))
		return;
	before = &saved[((uintptr_t)at >> 1) % SAVED];
	if (before->place != at || before->width != width || !before->written)
	{
		before->place = at;
		before->width = width;
		before->written = 0;
		memcpy(before->bytes, at, width);
		memcpy(before->places, places, width);
	}
	if (!whole(places, width))    /* (part of a wider value: copied elsewhere, it is not one) */
		return;
	read = &reads[next_read++ % READS];
	read->place = at;
	read->width = width;
	memcpy(read->bytes, at, width);
	memcpy(read->places, places, width);
}

void s32_order_stored(const void *at, size_t width)
{
	settle();
	written = at;
	written_width = width;
	written_by = __builtin_return_address(0);
	settle();
}

void s32_order_bytes(const void *at, size_t size)
{
	int8_t *memory, *places;
	int8_t none[WIDEST] = { 0 };
	size_t memory_size;

	settle();
	if (!(places = places_of(at, size, &memory, &memory_size)))
		return;
	for (size_t index = 0; index < size; index += WIDEST)
		place(memory, memory_size, places + index, size - index < WIDEST ? size - index : WIDEST, none, "as bytes",
			__builtin_return_address(0));
}

void s32_order_moved(const void *to, const void *from, size_t size)
{
	int8_t *memory, *places, *from_memory;
	const int8_t *from_places;
	int8_t moved[WIDEST];
	size_t memory_size, from_size;

	settle();
	if (!(places = places_of(to, size, &memory, &memory_size)))
		return;
	from_places = places_of(from, size, &from_memory, &from_size);
	for (size_t index = 0; index < size; index += WIDEST)
	{
		const size_t part = size - index < WIDEST ? size - index : WIDEST;
		if (from_places)
			memcpy(moved, from_places + index, part);
		else
			memset(moved, 0, part);
		place(memory, memory_size, places + index, part, moved, "moved", __builtin_return_address(0));
	}
}

void s32_order_placed(const void *at, const int8_t *new_places, size_t size)
{
	int8_t *memory, *places;
	size_t memory_size;

	settle();
	if (size <= WIDEST && (places = places_of(at, size, &memory, &memory_size)) != NULL)
		place(memory, memory_size, places, size, new_places, "from the program's data", __builtin_return_address(0));
}

/* which of out_bytes' memories 'address' is in, or -1 */
static int out_memory(uint32_t address)
{
	switch ((address >> 20) & 0xf)
	{
		case 0x3: return 0;
		case 0x4: return 1;
		default: return -1;
	}
}

void s32_order_out(const void *from, uint32_t address, size_t size)
{
	const int memory = out_memory(address);
	const size_t at = address & (sizeof(out_bytes[0]) - 1);
	int8_t *places_memory;
	const int8_t *places;
	size_t memory_size;

	settle();
	const uint8_t *words = memory ? s32_board.sprite_ram : s32_board.video_ram;
	const uint8_t *bytes = from;

	settle();
	if (memory < 0 || at + size > sizeof(out_bytes[0]) || !(places = places_of(from, size, &places_memory, &memory_size)))
		return;
	memcpy(&out_bytes[memory][at], from, size);
	memcpy(&out_places[memory][at], places, size);
	memset(&out_kept[memory][at], 1, size);
	for (size_t index = 0; index < size; index++)
	{
		/* (the V70's byte at the source's address: the host's byte whose place is there, a value's bytes being
		   at most WIDEST apart) */
		const ptrdiff_t source = (const int8_t *)places - places_memory + (ptrdiff_t)index;
		uint8_t v70 = bytes[index];
		for (ptrdiff_t near = source - WIDEST; near <= source + WIDEST; near++)
			if (near >= 0 && (size_t)near < memory_size && near + places_memory[near] == source)
			{
				v70 = bytes[near - source + (ptrdiff_t)index];
				break;
			}
		out_v70[memory][at + index] = v70;
		out_written[memory][at + index] = words[(at + index) ^ S32_BYTE_LANE];
	}
}

void s32_order_in(const void *to, uint32_t address, size_t size)
{
	const int memory = out_memory(address);
	const size_t at = address & (sizeof(out_bytes[0]) - 1);
	int8_t *places_memory, *places;
	size_t memory_size;

	settle();
	if (memory < 0 || at + size > sizeof(out_bytes[0]) || size > WIDEST
		|| !(places = places_of(to, size, &places_memory, &memory_size)) || memcmp(&out_bytes[memory][at], to, size) != 0)
		return;
	for (size_t index = 0; index < size; index++)
		if (!out_kept[memory][at + index])
			return;
	place(places_memory, memory_size, places, size, &out_places[memory][at], "as the bytes moved out there",
		__builtin_return_address(0));
}

const uint8_t *s32_order_words(const uint8_t *memory)
{
	const int which = memory == s32_board.sprite_ram ? 1 : 0;
	const size_t size = which ? S32_SPRITE_RAM_SIZE : S32_VIDEO_RAM_SIZE;

	settle();
	memcpy(word_image, memory, size);
	for (size_t at = 0; at < size; at++)
		if (out_kept[which][at] && memory[at ^ S32_BYTE_LANE] == out_written[which][at])
			word_image[at ^ S32_BYTE_LANE] = out_v70[which][at];
	return word_image;
}

const uint8_t *s32_order_v70(const uint8_t *memory)
{
	const int8_t *places = memory == s32_board.shared_ram ? shared_places : work_places;
	uint8_t *image = memory == s32_board.shared_ram ? shared_image : work_image;
	const size_t size = memory == s32_board.shared_ram ? S32_SHARED_RAM_SIZE : S32_WORK_RAM_SIZE;

	settle();
	memcpy(image, memory, size);
	for (size_t at = 0; at < size; at++)
	{
		const size_t v70 = at + (size_t)(ptrdiff_t)places[at];
		if (v70 < size)
			image[v70] = memory[at];
	}
	return image;
}

/* ---- the compiler's calls ---- */

static void will_write(const void *at, size_t width, const void *by)
{
	settle();
	written = at;
	written_width = width;
	written_by = by;
}

#define BY __builtin_return_address(0)

void __tsan_init(void);
void __tsan_func_entry(void *caller);
void __tsan_func_exit(void);
void __tsan_read1(void *at);
void __tsan_read2(void *at);
void __tsan_read4(void *at);
void __tsan_read8(void *at);
void __tsan_read16(void *at);
void __tsan_unaligned_read2(void *at);
void __tsan_unaligned_read4(void *at);
void __tsan_unaligned_read8(void *at);
void __tsan_unaligned_read16(void *at);
void __tsan_read_range(void *at, size_t size);
void __tsan_write1(void *at);
void __tsan_write2(void *at);
void __tsan_write4(void *at);
void __tsan_write8(void *at);
void __tsan_write16(void *at);
void __tsan_unaligned_write2(void *at);
void __tsan_unaligned_write4(void *at);
void __tsan_unaligned_write8(void *at);
void __tsan_unaligned_write16(void *at);
void __tsan_write_range(void *at, size_t size);

void __tsan_init(void) {}
void __tsan_func_entry(void *caller) { (void)caller; }
void __tsan_func_exit(void) {}
void __tsan_read1(void *at) { s32_order_loaded(at, 1); }
void __tsan_read2(void *at) { s32_order_loaded(at, 2); }
void __tsan_read4(void *at) { s32_order_loaded(at, 4); }
void __tsan_read8(void *at) { s32_order_loaded(at, 8); }
void __tsan_read16(void *at) { s32_order_loaded(at, 16); }
void __tsan_unaligned_read2(void *at) { s32_order_loaded(at, 2); }
void __tsan_unaligned_read4(void *at) { s32_order_loaded(at, 4); }
void __tsan_unaligned_read8(void *at) { s32_order_loaded(at, 8); }
void __tsan_unaligned_read16(void *at) { s32_order_loaded(at, 16); }
void __tsan_read_range(void *at, size_t size) { s32_order_loaded(at, size); }
void __tsan_write1(void *at) { will_write(at, 1, BY); }
void __tsan_write2(void *at) { will_write(at, 2, BY); }
void __tsan_write4(void *at) { will_write(at, 4, BY); }
void __tsan_write8(void *at) { will_write(at, 8, BY); }
void __tsan_write16(void *at) { will_write(at, 16, BY); }
void __tsan_unaligned_write2(void *at) { will_write(at, 2, BY); }
void __tsan_unaligned_write4(void *at) { will_write(at, 4, BY); }
void __tsan_unaligned_write8(void *at) { will_write(at, 8, BY); }
void __tsan_unaligned_write16(void *at) { will_write(at, 16, BY); }
void __tsan_write_range(void *at, size_t size) { will_write(at, size, BY); }
