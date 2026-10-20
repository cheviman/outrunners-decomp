/**
 *    Name:        game.c
 *    Description: runtime of the decompiled game program: processor status, the frame loop and its
 *                 interrupts, calls by address, and the V60's string and bit-string moves and searches
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    15-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's V60 core (src/devices/cpu/v60/v60.cpp, op12.hxx, op7a.hxx;
 *    Farfetch'd, R. Belmont) and the frame timing of tools/reference/s32ref.cpp.
 */
#include "game/support.h"

#include <stdio.h>
#include <stdlib.h>

#define PSW_IE        (1u << 18)    /* maskable interrupts enabled */
#define PSW_IS        (1u << 28)    /* running on the interrupt stack */
#define PRIVILEGED    29            /* LDPR numbers 0-28 */
#define SBR           5             /* system base register: the vector table, in 4 KB steps */

uint32_t psw;
void (*program_frame_done)(void);

static uint32_t privileged[PRIVILEGED];
static bool irq_line;
static uint64_t frames;
static uint32_t interrupts;

/* ---- processor status ---- */

uint32_t psw_read(uint32_t z, uint32_t s, uint32_t ov, uint32_t cy)
{
	return (psw & 0xfffffff0u) | z | (s << 1) | (ov << 2) | (cy << 3);
}

void psw_write(uint32_t value)
{
	psw = value;
}

uint32_t psw_update(uint32_t current, uint32_t value, uint32_t mask)
{
	const uint32_t changed = mask & 0xffffffu;

	psw = (current & ~changed) | (value & changed);
	return psw;
}

void load_privileged(uint32_t number, uint32_t value)
{
	if (number >= PRIVILEGED)
	{
		fprintf(stderr, "LDPR to privileged register %u (frame %llu)\n", number, (unsigned long long)frames);
		exit(3);
	}
	privileged[number] = value;
}

/* ---- the frame loop and its interrupts ---- */

void frame_wait(uint32_t frames_to_wait)
{
	if ((int8_t)mem_read8(frames_to_wait) >= 0)
		s32_wait();
}

void take_interrupts(void)
{
	/* the interrupt cycle asks the controller for its vector, as MAME's v60_try_irq does; the handler
	   runs at level 0 on the interrupt stack with interrupts off, and RETIS puts the status back. A
	   handler that leaves another source pending is followed by the next one */
	while (irq_line && (psw & PSW_IE))
	{
		const uint32_t vector = s32_irq_acknowledge() + 0x40u;
		const uint32_t saved = psw;
		const uint32_t handler = mem_read32((privileged[SBR] & ~0xfffu) + vector * 4);

		interrupts++;
		psw &= ~((3u << 24) | (1u << 18) | (1u << 16) | (1u << 27) | (1u << 17) | (1u << 29));
		psw |= PSW_IS | (1u << 31);
		call_code(handler);
		psw = saved;
	}
}

void frame_end(void)
{
	frames++;
	if (program_frame_done)
		program_frame_done();
}

/* ---- calls by address ---- */

void call_code(uint32_t address)
{
	const uint32_t key = address & 0xffffff;
	size_t low = 0;
	size_t high = game_code_count;

	while (low < high)
	{
		const size_t middle = (low + high) / 2;
		if (game_code[middle].address < key)
			low = middle + 1;
		else
			high = middle;
	}
	if (low < game_code_count && game_code[low].address == key)
	{
		game_code[low].function();
		return;
	}
	unknown_code(address);
}

static void print_recent_calls(void);

uint8_t *ram_outside(uint32_t address)
{
	/* as large as the largest structure (struct player) */
	static uint8_t copy[0x200];
	uint32_t index;

	if ((address & 0xffffff) < 0x200000 || (address & 0xf00000) == 0xf00000)
	{
		for (index = 0; index < sizeof copy; index++)
			copy[index] = *rom(address + index);
		return copy;
	}
	fprintf(stderr, "Work RAM access at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void rom_outside(uint32_t address)
{
	fprintf(stderr, "Program ROM read at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void unknown_code(uint32_t address)
{
	fprintf(stderr, "No code at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void invalid_jump(uint32_t address)
{
	fprintf(stderr, "Jump to %06X, past its table (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void invalid_operand(uint32_t address)
{
	fprintf(stderr, "Instruction the V60 cannot run at %06X (frame %llu)\n", address & 0xffffff,
		(unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

static uint8_t *blocks_run;

static void write_blocks_run(void)
{
	FILE *output = fopen("blocks_run.txt", "w");

	if (!output)
		return;
	for (uint32_t address = 0; address < 0x1000000u; address++)
		if (blocks_run[address >> 3] & (1u << (address & 7)))
			fprintf(output, "%06X\n", address);
	fclose(output);
}

void game_trace_block(uint32_t address)
{
	if (!blocks_run)
	{
		blocks_run = (uint8_t *)calloc(0x1000000u / 8, 1);
		if (!blocks_run)
			return;
		atexit(write_blocks_run);
	}
	address &= 0xffffff;
	blocks_run[address >> 3] |= (uint8_t)(1u << (address & 7));
}

/* --trace: the calls so far, and the last 16 of them */
static uint64_t calls;
static uint32_t recent[16];

static void print_recent_calls(void)
{
	if (!calls)
		return;
	fprintf(stderr, "the last calls:");
	for (int index = 0; index < 16; index++)
		fprintf(stderr, " %06X", recent[(calls + index) % 16]);
	fprintf(stderr, "\n");
}

void game_trace(uint32_t address)
{
	/* the first calls one by one, then every millionth with the last few before it */
	recent[calls % 16] = address;
	calls++;
	if (calls <= 20000)
		fprintf(stderr, "call %06X frame %llu\n", address, (unsigned long long)frames);
	else if (calls % 1000000 == 0)
	{
		fprintf(stderr, "call %llu frame %llu, the last ones:", (unsigned long long)calls, (unsigned long long)frames);
		for (int index = 0; index < 16; index++)
			fprintf(stderr, " %06X", recent[(calls + index) % 16]);
		fprintf(stderr, "\n");
	}
}

/* ---- MOVCU / MOVCFU / MOVCD (op7a.hxx opMOVSTRUB/UH/DB/DH with bStop = 0) ---- */

static uint32_t read_element(uint32_t address, uint32_t size)
{
	return size == 1 ? mem_read8(address) : mem_read16(address);
}

static void write_element(uint32_t address, uint32_t value, uint32_t size)
{
	if (size == 1)
		mem_write8(address, (uint8_t)value);
	else
		mem_write16(address, (uint16_t)value);
}

void move_string(uint32_t source, uint32_t source_length, uint32_t dest, uint32_t dest_length, uint32_t element_size,
                 uint32_t fill_rest, uint32_t downward, uint32_t fill, uint32_t *dest_end, uint32_t *source_end)
{
	const uint32_t count = source_length < dest_length ? source_length : dest_length;
	const uint32_t step = element_size;
	uint32_t i;

	if (!downward)
	{
		for (i = 0; i < count; i++)
			write_element(dest + i * step, read_element(source + i * step, element_size), element_size);
		*source_end = source + i * step;
		*dest_end = dest + i * step;
		if (fill_rest && source_length < dest_length)
		{
			for (; i < dest_length; i++)
				write_element(dest + i * step, fill, element_size);
			*dest_end = dest + i * step;
		}
		return;
	}

	for (i = 0; i < count; i++)
		write_element(dest + (count - i - 1) * step, read_element(source + (count - i - 1) * step, element_size), element_size);
	*source_end = source + (source_length - i - 1) * step;
	*dest_end = dest + (dest_length - i - 1) * step;
	if (fill_rest && source_length < dest_length)
	{
		/* MAME's byte and halfword versions differ here; the byte one offsets by the copied count */
		for (; i < dest_length; i++)
		{
			if (element_size == 1)
				write_element(dest + count + (dest_length - i - 1), fill, 1);
			else
				write_element(dest + (dest_length - i - 1) * step, fill, 2);
		}
		*dest_end = dest + (dest_length - i - 1) * step;
	}
}

void move_bytes(uint32_t dest, uint32_t source, uint32_t count)
{
	uint32_t dest_end, source_end;

	move_string(source, count, dest, count, 1, 0, 0, 0, &dest_end, &source_end);
}

void move_halfwords(uint32_t dest, uint32_t source, uint32_t count)
{
	uint32_t dest_end, source_end;

	move_string(source, count, dest, count, 2, 0, 0, 0, &dest_end, &source_end);
}

void move_bytes_backward(uint32_t dest, uint32_t source, uint32_t count)
{
	uint32_t dest_end, source_end;

	move_string(source, count, dest, count, 1, 0, 1, 0, &dest_end, &source_end);
}

void move_halfwords_backward(uint32_t dest, uint32_t source, uint32_t count)
{
	uint32_t dest_end, source_end;

	move_string(source, count, dest, count, 2, 0, 1, 0, &dest_end, &source_end);
}

void set_bytes(uint32_t dest, uint32_t count, uint32_t value)
{
	uint32_t dest_end, source_end;

	move_string(0, 0, dest, count, 1, 1, 0, value, &dest_end, &source_end);
}

void set_halfwords(uint32_t dest, uint32_t count, uint32_t value)
{
	uint32_t dest_end, source_end;

	move_string(0, 0, dest, count, 2, 1, 0, value, &dest_end, &source_end);
}

/* ---- MOVBSD / MOVBSU (op7a.hxx) ---- */

void move_bits_down(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length,
                    uint32_t *dest_end, uint32_t *source_end)
{
	uint8_t source_data;
	uint8_t dest_data;

	source_offset += length - 1;
	dest_offset += length - 1;
	source += source_offset / 8;
	dest += dest_offset / 8;
	source_offset &= 7;
	dest_offset &= 7;
	source_data = mem_read8(source);
	dest_data = mem_read8(dest);
	for (uint32_t i = 0; i < length; i++)
	{
		*source_end = source;
		*dest_end = dest;
		dest_data &= (uint8_t)~(1u << dest_offset);
		dest_data |= (uint8_t)(((source_data >> source_offset) & 1) << dest_offset);
		if (source_offset == 0)
		{
			source_offset = 8;
			source--;
			source_data = mem_read8(source);
		}
		if (dest_offset == 0)
		{
			mem_write8(dest, dest_data);
			dest_offset = 8;
			dest--;
			dest_data = mem_read8(dest);
		}
		source_offset--;
		dest_offset--;
	}
	if (dest_offset != 7)
		mem_write8(dest, dest_data);
}

void move_bits_up(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length,
                  uint32_t *dest_end, uint32_t *source_end)
{
	uint8_t source_data;
	uint8_t dest_data;

	source += source_offset / 8;
	dest += dest_offset / 8;
	source_offset &= 7;
	dest_offset &= 7;
	source_data = mem_read8(source);
	dest_data = mem_read8(dest);
	for (uint32_t i = 0; i < length; i++)
	{
		*source_end = source;
		*dest_end = dest;
		dest_data &= (uint8_t)~(1u << dest_offset);
		dest_data |= (uint8_t)(((source_data >> source_offset) & 1) << dest_offset);
		source_offset++;
		dest_offset++;
		if (source_offset == 8)
		{
			source_offset = 0;
			source++;
			source_data = mem_read8(source);
		}
		if (dest_offset == 8)
		{
			mem_write8(dest, dest_data);
			dest_offset = 0;
			dest++;
			dest_data = mem_read8(dest);
		}
	}
	if (dest_offset != 0)
		mem_write8(dest, dest_data);
}

/* ---- SCH0BSU / SCH1BSU (op7a.hxx, opSCHBS) ---- */

uint32_t search_bits_up(uint32_t address, uint32_t offset, uint32_t length, uint32_t bit, uint32_t *work)
{
	uint8_t data;
	uint32_t count;

	address += offset / 8;
	data = mem_read8(address);
	offset &= 7;
	for (count = 0; count < length; count++)
	{
		*work = address;
		if (((data >> offset) & 1u) == bit)
			break;
		offset++;
		if (offset == 8)
		{
			offset = 0;
			address++;
			data = mem_read8(address);
		}
	}
	return count;
}

/* ---- DIVX / DIVUX ---- */

static void division_by_zero(void)
{
	fprintf(stderr, "V60 division by zero (frame %llu)\n", (unsigned long long)frames);
	exit(3);
}

void divx_pair(uint32_t divisor, uint32_t *low, uint32_t *high)
{
	const int64_t dividend = (int64_t)(((uint64_t)*high << 32) | *low);
	const int32_t by = (int32_t)divisor;

	if (by == 0)
		division_by_zero();
	*low = (uint32_t)(int32_t)(dividend / by);
	*high = (uint32_t)(int32_t)(dividend % by);
}

void divux_pair(uint32_t divisor, uint32_t *low, uint32_t *high)
{
	const uint64_t dividend = ((uint64_t)*high << 32) | *low;

	if (divisor == 0)
		division_by_zero();
	*low = (uint32_t)(dividend / divisor);
	*high = (uint32_t)(dividend % divisor);
}

/* ---- the program as a port runs it (s32/program.h) ---- */

void program_reset(void)
{
	for (int number = 0; number < PRIVILEGED; number++)
		privileged[number] = 0;
	privileged[9] = 0x00006000;     /* PIR */
	privileged[7] = 0x00000070;     /* SYCW */
	privileged[8] = 0x0000e000;     /* TKCW */
	privileged[15] = 0x0000f002;    /* PSW2 */
	psw = PSW_IS;
	irq_line = false;
	frames = 0;
	interrupts = 0;
}

void program_run(void)
{
	game_entry();
}

void program_irq_line(bool asserted)
{
	irq_line = asserted;
}

uint64_t program_frames(void)
{
	return frames;
}

uint32_t program_interrupts(void)
{
	return interrupts;
}
