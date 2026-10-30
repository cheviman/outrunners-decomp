/**
 *    Name:        game.c
 *    Description: runtime of the decompiled game program: processor status, the frame loop and its
 *                 interrupts, the game's two tasks, calls by address, and the V60's string and bit-string
 *                 moves and searches
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's V60 core (src/devices/cpu/v60/v60.cpp, op12.hxx, op7a.hxx;
 *    Farfetch'd, R. Belmont) and the frame timing of tools/core/system32/reference/s32ref.cpp.
 */
#include "game/support.h"
#include "host/tasks.h"

#include <stdio.h>
#include <stdlib.h>

#define PSW_IE        (1u << 18)    /* maskable interrupts enabled */
#define PSW_IS        (1u << 28)    /* running on the interrupt stack */
#define PRIVILEGED    29            /* LDPR numbers 0-28 */
#define SBR           5             /* system base register: the vector table, in 4 KB steps */
#define TASK_FRAME    0x20E000u     /* 0[R24]: the interrupt frame of the task that waits */
#define FRAME_PC      116           /* in such a frame (pushm #1FFFFFFF, then the V70's own): R0-R28, */
#define FRAME_PSW     120           /* then the PC and the PSW RETIS takes */
#define CONTEXT_LIMIT 32

uint32_t psw;
void (*program_frame_done)(void);

/* the game's state (ram.h's struct game): the work RAM of the one world the program runs, which the board
   reads too (program_reset hands it over) */
static uint64_t first_world[S32_WORK_RAM_SIZE / sizeof(uint64_t)];
struct game *game = (struct game *)first_world;

static uint32_t privileged[PRIVILEGED];
static bool irq_line;
static uint64_t frames;
static uint32_t interrupts;

/* A task of the game: a native stack of its own and its copy of the registers the program shares */
typedef struct
{
	host_task *native;
	uint32_t frame;                       /* the interrupt frame a new task starts from */
	uint32_t token;                       /* what 0[R24] holds while it waits */
	uintptr_t context[CONTEXT_LIMIT];     /* a number, or a pointer into the game's state (game_context) */
} game_task;

static game_task task_records[2];
static game_task *running_task;
static game_task *waiting_task;
static uint32_t tokens;
static bool swap_pending;
static host_task *main_task;

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

void frame_wait(uint32_t frame_ready)
{
	if (mem_read8(frame_ready) == 0)
		s32_wait();
}

static void switch_task(void);

void take_interrupts(void)
{
	/* the interrupt cycle asks the controller for its vector, as MAME's v60_try_irq does; the handler
	   runs at level 0 on the interrupt stack with interrupts off, and RETIS puts the status back. A
	   handler that leaves another source pending is followed by the next one. A handler that swapped the
	   tasks returns into the other one: this one goes on when a handler swaps them back */
	while (irq_line && (psw & PSW_IE))
	{
		const uint32_t vector = s32_irq_acknowledge() + 0x40u;
		const uint32_t saved = psw;
		const uint32_t handler = mem_read32((privileged[SBR] & ~0xfffu) + vector * 4);

		interrupts++;
		psw &= ~((3u << 24) | (1u << 18) | (1u << 16) | (1u << 27) | (1u << 17) | (1u << 29));
		psw |= PSW_IS | (1u << 31);
		call_code(handler);
		if (swap_pending)
		{
			swap_pending = false;
			switch_task();
		}
		psw = saved;
	}
}

void frame_end(void)
{
	frames++;
	if (program_frame_done)
		program_frame_done();
}

/* ---- the game's two tasks ---- */

/* OutRunners runs two tasks, each with registers and a stack of its own: boot's, and one that starts from
   the interrupt frame boot writes at 21FF84 (its PC, 0006B0, and its registers). The VBLANK handler swaps
   them (03F5FC, swap_tasks): it keeps the running task's stack pointers at 0[R24] and 4[R24], takes the
   other one's, and returns into it. Here each task runs on a native stack of its own and keeps its own
   copy of the registers the program shares; the swap happens when the handler is done (what the handler
   does after it, it does for the task it interrupted). While a task waits, 0[R24] holds a token for it;
   an address there is a new frame (boot again, after the test mode): a new task starts from it */

void swap_tasks(void)
{
	swap_pending = true;
}

/* A register the whole program shares, as the C keeps it: the number, or the pointer into the game's state
   for the address the original holds there (where the C keeps a pointer) */
static uintptr_t shared_register(size_t index)
{
	if (game_context[index].pointer)
		return (uintptr_t)*(void **)game_context[index].variable;
	return *(uint32_t *)game_context[index].variable;
}

static void set_shared_register(size_t index, uintptr_t value)
{
	if (game_context[index].pointer)
		*(void **)game_context[index].variable = (void *)value;
	else
		*(uint32_t *)game_context[index].variable = (uint32_t)value;
}

static void start_task(void)
{
	const uint32_t frame = running_task->frame;

	for (size_t index = 0; index < game_context_count; index++)
	{
		const uint32_t value = mem_read32(frame + 4u * game_context_registers[index]);
		set_shared_register(index, game_context[index].pointer ? (uintptr_t)pointer_to(value) : value);
	}
	psw = mem_read32(frame + FRAME_PSW);
	call_code(mem_read32(frame + FRAME_PC));
	fprintf(stderr, "The task started from the frame at %06X came to its end (frame %llu)\n", frame,
		(unsigned long long)frames);
	exit(3);
}

static void switch_task(void)
{
	const uint32_t frame = mem_read32(TASK_FRAME);
	game_task *from = running_task;
	game_task *to;
	bool fresh = false;

	for (size_t index = 0; index < game_context_count; index++)
		from->context[index] = shared_register(index);
	if (waiting_task && frame == waiting_task->token)
		to = waiting_task;
	else
	{
		/* the one that waited, if any, is left where it is: the game started over without it */
		to = from == &task_records[0] ? &task_records[1] : &task_records[0];
		to->native = host_task_create(start_task);
		to->frame = frame;
		fresh = true;
	}
	from->token = 0xff000000u | ++tokens;
	mem_write32(TASK_FRAME, from->token);
	waiting_task = from;
	running_task = to;
	if (!fresh)
		for (size_t index = 0; index < game_context_count; index++)
			set_shared_register(index, to->context[index]);
	host_task_switch(to->native);
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

/* the copies of the ROM ram_outside hands out, each with the address it copied from: a few, so that a
   pointer the C keeps into one (the text R20 walks) outlives the copies other reads make */
#define OUTSIDE_COPIES 4
static uint8_t outside_copy[OUTSIDE_COPIES][0x200];    /* as large as the largest structure (struct player) */
static uint32_t outside_address[OUTSIDE_COPIES];
static unsigned outside_next;

uint8_t *ram_outside(uint32_t address)
{
	unsigned copy;

	if (address == 0)
		return NULL;    /* (no place: what a pointer the C keeps holds where the original holds 0) */
	if ((address & 0xffffff) < 0x200000 || (address & 0xf00000) == 0xf00000)
	{
		/* the copy of the same place again (what was written to it goes, as the bus drops it), or the
		   oldest one */
		for (copy = 0; copy < OUTSIDE_COPIES && outside_address[copy] != address; copy++)
			;
		if (copy == OUTSIDE_COPIES)
		{
			copy = outside_next;
			outside_next = (outside_next + 1) % OUTSIDE_COPIES;
		}
		for (uint32_t index = 0; index < sizeof outside_copy[copy]; index++)
		{
			/* (0 where no piece has data: what the code reads through such a copy is the ROM's data) */
			const uint8_t *byte = data_if_any(address + index);
			outside_copy[copy][index] = byte ? *byte : 0;
		}
		outside_address[copy] = address;
		return outside_copy[copy];
	}
	fprintf(stderr, "Work RAM access at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

void *pointer_to(uint32_t address)
{
	switch ((address >> 20) & 0xf)
	{
	case 0x3:
	case 0x4:
	case 0x7:
		return board(address);    /* the video RAM, the sprite RAM, the RAM the sound program shares */
	default:
		return ram(address);      /* the game's state; none for 0; a copy of the ROM; else the program stops */
	}
}

const void *rom_pointer(uint32_t address)
{
	const uint8_t *data;

	const uint32_t area = (address >> 20) & 0xf;

	if (address != 0 && (area < 0x2 || area == 0xf) && (data = data_if_any(address)) != NULL)
		return data;    /* the piece of the program's data there, read only */
	return pointer_to(address);    /* the game's state, a memory of the board, none for 0, else a copy */
}

uint32_t address_outside(const void *pointer)
{
	const uintptr_t place = (uintptr_t)pointer;
	uint32_t data_address;

	if (pointer == NULL)
		return 0;
	if (program_data_address(pointer, &data_address))
		return data_address;
	if (place - (uintptr_t)s32_board.video_ram < S32_VIDEO_RAM_SIZE)
		return 0x300000u + (uint32_t)(place - (uintptr_t)s32_board.video_ram);
	if (place - (uintptr_t)s32_board.sprite_ram < S32_SPRITE_RAM_SIZE)
		return 0x400000u + (uint32_t)(place - (uintptr_t)s32_board.sprite_ram);
	if (place - (uintptr_t)s32_board.shared_ram < S32_SHARED_RAM_SIZE)
		return 0x700000u + (uint32_t)(place - (uintptr_t)s32_board.shared_ram);
	for (unsigned copy = 0; copy < OUTSIDE_COPIES; copy++)
	{
		const uintptr_t offset = (uintptr_t)pointer - (uintptr_t)outside_copy[copy];
		if (offset < sizeof outside_copy[copy])
			return outside_address[copy] + (uint32_t)offset;
	}
	fprintf(stderr, "A pointer into the game's state points outside it (frame %llu)\n", (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void data_outside(uint32_t address)
{
	fprintf(stderr, "Game data read at %06X, where no asset has data (frame %llu)\n", address & 0xffffff,
		(unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void board_outside(uint32_t address)
{
	fprintf(stderr, "Board memory access at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

GAME_NORETURN void unknown_code(uint32_t address)
{
	fprintf(stderr, "No code at %06X (frame %llu)\n", address & 0xffffff, (unsigned long long)frames);
	print_recent_calls();
	exit(3);
}

/* The MEMORY TEST's ROM part (runtime/replaced.txt): the game's, original_test_mode_memory_test_checksum, sums
   the program ROM's code chips (from 000000) or its data chips (from 100000) as two 16-bit checks; the
   decompiled game has no chips to measure (the code is in none of its data), so it hands back the sums the
   program keeps for them, at 03FFF8 for the code chips and 03FFFC for the data chips, as good chips give */
void test_mode_memory_test_checksum(uint32_t r10, uint32_t *p_r0, uint32_t *p_r1)
{
	const uint32_t sums = r10 < 0x100000u ? 0x03FFF8u : 0x03FFFCu;

	*p_r0 = (*p_r0 & 0xFFFF0000u) | DATA16(sums);
	*p_r1 = (*p_r1 & 0xFFFF0000u) | DATA16(sums + 2u);
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

/* ---- SCHCU.B / SKPCU.B (op7a.hxx, opSEARCHUB) ---- */

void search_bytes(uint32_t address, uint32_t length, uint32_t value, uint32_t until_equal, uint32_t *index,
	uint32_t *found)
{
	uint32_t count;

	for (count = 0; count < length; count++)
		if ((mem_read8(address + count) == (uint8_t)value) == (until_equal != 0))
			break;
	*index = count;
	*found = address + count;
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
	/* the board's work RAM is the game's state from here on (s32_init left it cleared, as the world is
	   before its first run; from then on s32_init clears the world) */
	s32_board.work_ram = (uint8_t *)game;
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
	swap_pending = false;
	waiting_task = NULL;
}

static void run_entry(void)
{
	game_entry();
	fprintf(stderr, "The program came to its end (frame %llu)\n", (unsigned long long)frames);
	exit(3);
}

void program_run(void)
{
	/* the program's first task runs on a stack of its own, like the one it starts later; the port's
	   stays apart, for program_stop to go back to */
	if (game_context_count > CONTEXT_LIMIT)
	{
		fprintf(stderr, "The program shares more registers than a task keeps\n");
		exit(3);
	}
	main_task = host_task_main();
	running_task = &task_records[0];
	running_task->native = host_task_create(run_entry);
	host_task_switch(running_task->native);
}

void program_stop(void)
{
	host_task_switch(main_task);
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
