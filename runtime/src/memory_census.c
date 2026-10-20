/**
 *    Name:        memory_census.c
 *    Description: what memory each mem_read and mem_write of the game's C reaches, by its line of C
 *    Version:     0.1.0
 *    Created:     15-10-2026
 *    Modified:    15-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A development aid. Built with S32_MEMORY_CENSUS (CMake: SONIC_MEMORY_CENSUS), the game's C counts
 *    every access it makes through mem_read/mem_write by the file and line that makes it: how many, which
 *    regions of the board they reach and the lowest and highest address. The headless port writes it
 *    (--memory-census FILE); tools/memory_census.py reads it.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "s32/bus.h"

#define CENSUS_SLOTS (1u << 16)

struct census_entry
{
	const char *file;
	int line;
	int bits;
	int write;
	uint64_t count;
	uint32_t regions;
	uint32_t low;
	uint32_t high;
};

static struct census_entry *entries;
static unsigned used;

/* the region of the board an address is in, as a bit: what --memory-census writes names them */
static uint32_t region_of(uint32_t address)
{
	address &= 0xffffff;
	switch (address >> 20)
	{
		case 0x0: case 0x1: case 0xf: return 1u << 0;    /* program ROM */
		case 0x2: return 1u << 1;                       /* work RAM */
		case 0x3: return 1u << 2;                       /* video RAM */
		case 0x4: return 1u << 3;                       /* sprite RAM */
		case 0x5: return 1u << 4;                       /* sprite control */
		case 0x6: return (address & 0x10000) ? 1u << 6 : 1u << 5;    /* mixer, palette RAM */
		case 0x7: return 1u << 7;                       /* RAM shared with the sound board */
		case 0xc: return 1u << 8;                       /* I/O chip */
		case 0xd: return 1u << 9;                       /* interrupt controller */
		default: return 1u << 10;                       /* nothing mapped */
	}
}

void s32_memory_census(uint32_t address, int bits, int write, const char *file, int line)
{
	uint32_t hash = (uint32_t)((uintptr_t)file * 2654435761u) ^ (uint32_t)line * 40503u ^ (uint32_t)(bits * 2 + write);
	uint32_t slot;

	if (!entries)
	{
		entries = calloc(CENSUS_SLOTS, sizeof(*entries));
		if (!entries)
			return;
	}
	for (slot = hash & (CENSUS_SLOTS - 1); entries[slot].file; slot = (slot + 1) & (CENSUS_SLOTS - 1))
		if (entries[slot].file == file && entries[slot].line == line && entries[slot].bits == bits
			&& entries[slot].write == write)
			break;
	if (!entries[slot].file)
	{
		if (used + 1 >= CENSUS_SLOTS)
			return;
		used++;
		entries[slot] = (struct census_entry){file, line, bits, write, 0, 0, 0xffffffff, 0};
	}
	address &= 0xffffff;
	entries[slot].count++;
	entries[slot].regions |= region_of(address);
	if (address < entries[slot].low)
		entries[slot].low = address;
	if (address > entries[slot].high)
		entries[slot].high = address;
}

void s32_memory_census_write(FILE *output)
{
	static const char *const names[] = {"rom", "work_ram", "video_ram", "sprite_ram", "sprite_control",
		"palette", "mixer", "sound_ram", "io", "interrupts", "unmapped"};

	fprintf(output, "# FILE:LINE BITS read|write COUNT LOW-HIGH REGIONS (memory_census.c)\n");
	for (uint32_t slot = 0; entries && slot < CENSUS_SLOTS; slot++)
	{
		const struct census_entry *entry = &entries[slot];
		if (!entry->file)
			continue;
		fprintf(output, "%s:%d %d %s %llu %06X-%06X", entry->file, entry->line, entry->bits,
			entry->write ? "write" : "read", (unsigned long long)entry->count, entry->low, entry->high);
		for (int region = 0; region < 11; region++)
			if (entry->regions & (1u << region))
				fprintf(output, " %s", names[region]);
		fputc('\n', output);
	}
}
