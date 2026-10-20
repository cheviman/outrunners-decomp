/**
 *    Name:        bus.h
 *    Description: the game program's view of memory: reads and writes through the System 32 bus
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    15-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The V60 drives 24 address lines, so an address wraps at 16 MB. Values are little endian, as the V60
 *    keeps them.
 */
#ifndef S32_BUS_H
#define S32_BUS_H

#include <stdint.h>

#include "s32/machine.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline uint8_t mem_read8(uint32_t address)
{
	return s32_read8(address & 0xffffff);
}

static inline uint16_t mem_read16(uint32_t address)
{
	return s32_read16(address & 0xffffff);
}

static inline uint32_t mem_read32(uint32_t address)
{
	return s32_read32(address & 0xffffff);
}

static inline void mem_write8(uint32_t address, uint8_t data)
{
	s32_write8(address & 0xffffff, data);
}

static inline void mem_write16(uint32_t address, uint16_t data)
{
	s32_write16(address & 0xffffff, data);
}

static inline void mem_write32(uint32_t address, uint32_t data)
{
	s32_write32(address & 0xffffff, data);
}

static inline uint64_t mem_read64(uint32_t address)
{
	const uint64_t low = mem_read32(address);
	return low | ((uint64_t)mem_read32(address + 4) << 32);
}

static inline void mem_write64(uint32_t address, uint64_t data)
{
	mem_write32(address, (uint32_t)data);
	mem_write32(address + 4, (uint32_t)(data >> 32));
}

#ifdef S32_MEMORY_CENSUS
/* a development aid (runtime/src/memory_census.c): every access the code built with S32_MEMORY_CENSUS
   makes through mem_read and mem_write, counted by its file and line */
#include <stdio.h>

void s32_memory_census(uint32_t address, int bits, int write, const char *file, int line);
void s32_memory_census_write(FILE *output);

static inline uint8_t mem_read8_at(uint32_t address, const char *file, int line)
{
	s32_memory_census(address, 8, 0, file, line);
	return mem_read8(address);
}

static inline uint16_t mem_read16_at(uint32_t address, const char *file, int line)
{
	s32_memory_census(address, 16, 0, file, line);
	return mem_read16(address);
}

static inline uint32_t mem_read32_at(uint32_t address, const char *file, int line)
{
	s32_memory_census(address, 32, 0, file, line);
	return mem_read32(address);
}

static inline void mem_write8_at(uint32_t address, uint8_t data, const char *file, int line)
{
	s32_memory_census(address, 8, 1, file, line);
	mem_write8(address, data);
}

static inline void mem_write16_at(uint32_t address, uint16_t data, const char *file, int line)
{
	s32_memory_census(address, 16, 1, file, line);
	mem_write16(address, data);
}

static inline void mem_write32_at(uint32_t address, uint32_t data, const char *file, int line)
{
	s32_memory_census(address, 32, 1, file, line);
	mem_write32(address, data);
}

#define mem_read8(address) mem_read8_at((address), __FILE__, __LINE__)
#define mem_read16(address) mem_read16_at((address), __FILE__, __LINE__)
#define mem_read32(address) mem_read32_at((address), __FILE__, __LINE__)
#define mem_write8(address, data) mem_write8_at((address), (data), __FILE__, __LINE__)
#define mem_write16(address, data) mem_write16_at((address), (data), __FILE__, __LINE__)
#define mem_write32(address, data) mem_write32_at((address), (data), __FILE__, __LINE__)
#endif

#ifdef __cplusplus
}
#endif

#endif
