/**
 *    Name:        machine.c
 *    Description: Multi 32 memory map, interrupt controller, timers, video registers and link board
 *    Version:     0.1.1
 *    Created:     11-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME sega/segas32.cpp, segas32_v.cpp and segas32_m.cpp (Aaron Giles), and
 *    sega/s32comm.cpp (Ariane Fugmann) for the link board.
 */
#include "internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

s32_machine s32_board;

void s32_log(const char *format, ...)
{
	char message[256];
	va_list arguments;

	if (!s32_board.log)
		return;
	va_start(arguments, format);
	vsnprintf(message, sizeof(message), format, arguments);
	va_end(arguments);
	s32_board.log(message);
}

static inline uint16_t load16(const uint8_t *source)
{
	return (uint16_t)(source[0] | (source[1] << 8));
}

static inline void store16(uint8_t *dest, uint16_t data, uint16_t mask)
{
	if (mask & 0x00ff)
		dest[0] = (uint8_t)data;
	if (mask & 0xff00)
		dest[1] = (uint8_t)(data >> 8);
}

/* ---- interrupt controller (0xD00000) ---- */

static uint8_t pending_irqs(void)
{
	return s32_board.irq_control[7] & ~s32_board.irq_control[6] & 0x1f;
}

static void update_irq(void)
{
	if (s32_board.irq_changed)
		s32_board.irq_changed(pending_irqs() != 0);
}

void s32_signal_irq(int source)
{
	if (source >= 0 && source < 5)
		s32_board.irq_control[7] |= (uint8_t)(1u << source);
	update_irq();
}

bool s32_irq_asserted(void)
{
	return pending_irqs() != 0;
}

uint8_t s32_irq_acknowledge(void)
{
	const uint8_t pending = pending_irqs();

	for (int source = 0; source < 5; source++)
		if (pending & (1u << source))
			return s32_board.irq_control[source];
	return 0;
}

static void update_next_timer(void)
{
	s32_board.next_timer = UINT64_MAX;
	for (int which = 0; which < 2; which++)
		if (s32_board.timer_deadline[which] && s32_board.timer_deadline[which] < s32_board.next_timer)
			s32_board.next_timer = s32_board.timer_deadline[which];
}

static void start_timer(int which)
{
	uint64_t period;

	if (which == 0)
	{
		const uint16_t duration = s32_board.irq_control[8] | ((s32_board.irq_control[9] << 8) & 0xf00);
		if (!duration)
			return;
		/* 0x800 ticks of 32 MHz / 2 per count (the System 32 counts the V60's clock; the Multi 32 keeps
		   the 32 MHz crystal's), converted to V70 cycles */
		period = 0x800ull * duration * S32_CPU_CLOCK / 16000000u;
	}
	else
	{
		const uint16_t duration = s32_board.irq_control[10] | ((s32_board.irq_control[11] << 8) & 0xf00);
		if (!duration)
			return;
		/* 0x100 ticks of 50 MHz / 16 per count, converted to V70 cycles */
		period = (0x100ull * duration * S32_CPU_CLOCK + 3124999) / 3125000;
	}
	s32_board.timer_deadline[which] = s32_board.cycles + period;
	update_next_timer();
}

uint64_t s32_next_timer(void)
{
	return s32_board.next_timer;
}

void s32_run_timers(void)
{
	for (int which = 0; which < 2; which++)
	{
		if (s32_board.timer_deadline[which] && s32_board.timer_deadline[which] <= s32_board.cycles)
		{
			s32_board.timer_deadline[which] = 0;
			update_next_timer();
			s32_signal_irq(S32_IRQ_TIMER0 + which);
		}
	}
}

static uint8_t int_control_read(uint32_t offset)
{
	(void)offset;
	return 0xff;    /* MAME does not model the timer count-down read-back either */
}

static void int_control_write(uint32_t offset, uint8_t data)
{
	switch (offset)
	{
		case 0: case 1: case 2: case 3: case 4:    /* vectors */
		case 5:                                      /* unknown */
			s32_board.irq_control[offset] = data;
			break;
		case 6:                                      /* mask */
			s32_board.irq_control[6] = data;
			update_irq();
			break;
		case 7:                                      /* acknowledge */
			s32_board.irq_control[7] &= data;
			update_irq();
			break;
		case 8: case 9:
			s32_board.irq_control[offset] = data;
			start_timer(0);
			break;
		case 10: case 11:
			s32_board.irq_control[offset] = data;
			start_timer(1);
			break;
		default:                                     /* 12-15: interrupt to the sound CPU */
			s32_board.sound_requests++;
			s32_sound_signal_irq(S32_SOUND_IRQ_V60);
			break;
	}
}

/* ---- video ---- */

static uint16_t to_alternate_format(uint16_t value)
{
	/* xBBBBBGGGGGRRRRR -> xBGRBBBBGGGGRRRR */
	const int r = value & 0x1f;
	const int g = (value >> 5) & 0x1f;
	const int b = (value >> 10) & 0x1f;
	uint16_t result = (value & 0x8000) | ((b & 0x01) << 14) | ((g & 0x01) << 13) | ((r & 0x01) << 12);
	return result | ((b & 0x1e) << 7) | ((g & 0x1e) << 3) | ((r & 0x1e) >> 1);
}

static uint16_t from_alternate_format(uint16_t value)
{
	const int r = ((value >> 12) & 0x01) | ((value << 1) & 0x1e);
	const int g = ((value >> 13) & 0x01) | ((value >> 3) & 0x1e);
	const int b = ((value >> 14) & 0x01) | ((value >> 7) & 0x1e);
	return (uint16_t)((value & 0x8000) | (b << 10) | (g << 5) | r);
}

static uint16_t palette_read(int screen, uint32_t index)
{
	/* the upper half of the window shows the same entries in the alternate format */
	const uint16_t value = s32_board.palette[screen][index & 0x3fff];
	return (index & 0x4000) ? to_alternate_format(value) : value;
}

static void palette_store(int screen, uint32_t index, uint16_t data, uint16_t mask, bool alternate)
{
	uint16_t value = s32_board.palette[screen][index];
	if (alternate)
		value = to_alternate_format(value);
	value = (uint16_t)((value & ~mask) | (data & mask));
	if (alternate)
		value = from_alternate_format(value);
	s32_board.palette[screen][index] = value;
}

static void palette_write(int screen, uint32_t index, uint16_t data, uint16_t mask)
{
	const bool alternate = (index & 0x4000) != 0;

	index &= 0x3fff;
	palette_store(screen, index, data, mask, alternate);
	/* with blending enabled, writes go to both halves of palette RAM */
	if (s32_board.mixer[screen][0x4e / 2] & 0x0880)
		palette_store(screen, index ^ 0x2000, data, mask, alternate);
}

static uint8_t sprite_control_read(uint32_t offset)
{
	switch (offset)
	{
		case 0: return (uint8_t)(0xfc | s32_board.sprite_buffer);
		case 1: return 0xfd;                                  /* "normal" render status */
		case 6: return (uint8_t)(0xfc | (s32_board.sprite_control_latched[6] & 1));
		case 7: return 0xfc;
		default: return (uint8_t)(0xfc | s32_board.sprite_control_latched[offset]);
	}
}

bool s32_display_enabled(int screen)
{
	return (s32_board.io_cnt[screen & 1] & 2) != 0;
}

uint64_t s32_frame_start(uint64_t frame)
{
	return frame * S32_CPU_CLOCK / S32_FRAME_RATE;
}

uint64_t s32_line_start(uint64_t frame, int line)
{
	return (frame * S32_LINES_PER_FRAME + (uint64_t)line) * S32_CPU_CLOCK / (S32_FRAME_RATE * S32_LINES_PER_FRAME);
}

/* MAME's timing: VBLANK starts at line 224 and ends with the frame; sprites go 50 us later */
static uint64_t next_frame_event(void)
{
	switch (s32_board.frame_phase)
	{
		case 0: return s32_line_start(s32_board.frame, S32_VISIBLE_LINES);
		case 1: return s32_frame_start(s32_board.frame + 1);
		default: return s32_frame_start(s32_board.frame + 1) + S32_CPU_CLOCK / 20000;
	}
}

void s32_wait(void)
{
	const uint64_t event = next_frame_event();

	if (s32_board.time_passes)
		s32_board.time_passes(s32_board.next_timer <= event ? s32_board.next_timer : event);
	if (s32_board.next_timer <= event)
	{
		s32_board.cycles = s32_board.next_timer;
		s32_run_timers();
		return;
	}
	s32_board.cycles = event;
	switch (s32_board.frame_phase)
	{
		case 0:
			s32_vblank_start();
			s32_board.frame_phase = 1;
			break;
		case 1:
			s32_vblank_end();
			s32_board.frame_phase = 2;
			break;
		default:
			s32_sprite_update();
			s32_board.frame++;
			s32_board.frame_phase = 0;
			break;
	}
}

static uint64_t hash_bytes(const void *data, size_t size, uint64_t hash)
{
	const uint8_t *bytes = (const uint8_t *)data;
	for (size_t index = 0; index < size; index++)
		hash = (hash ^ bytes[index]) * 0x100000001b3ull;
	return hash;
}

uint64_t s32_state_hash(void)
{
	uint64_t hash = 0xcbf29ce484222325ull;
	hash = hash_bytes(s32_board.work_ram, S32_TASK_STACKS, hash);
	hash = hash_bytes(s32_board.work_ram + S32_TASK_STACKS + S32_TASK_STACKS_SIZE,
		S32_STACK_START - S32_TASK_STACKS - S32_TASK_STACKS_SIZE, hash);
	hash = hash_bytes(s32_board.work_ram + S32_STACK_END, S32_WORK_RAM_SIZE - S32_STACK_END, hash);
	hash = hash_bytes(s32_board.video_ram, sizeof(s32_board.video_ram), hash);
	hash = hash_bytes(s32_board.sprite_ram, S32_SOUND_STACK_COPY, hash);
	hash = hash_bytes(s32_board.sprite_ram + S32_SOUND_STACK_COPY + S32_SOUND_STACK_COPY_SIZE,
		S32_TASK_STACKS - S32_SOUND_STACK_COPY - S32_SOUND_STACK_COPY_SIZE, hash);
	hash = hash_bytes(s32_board.sprite_ram + S32_TASK_STACKS + S32_TASK_STACKS_SIZE,
		S32_STACK_START - S32_TASK_STACKS - S32_TASK_STACKS_SIZE, hash);
	hash = hash_bytes(s32_board.sprite_ram + S32_STACK_END, sizeof(s32_board.sprite_ram) - S32_STACK_END, hash);
	hash = hash_bytes(s32_board.palette, sizeof(s32_board.palette), hash);
	return hash_bytes(s32_board.mixer, sizeof(s32_board.mixer), hash);
}

/* ---- link board (0x800000) ---- */

/* What the link board does each frame with no other cabinet on the line (MAME's simulation of
   EPR-15033, the board of Stadium Cross and OutRunners): when the program has written its greeting,
   "V70", the board answers "Z80" and clears the rest of its memory; the link stays offline */
static void comm_tick(void)
{
	uint8_t *ram = s32_board.comm_ram;

	if (!s32_board.comm_cn)
		return;
	if (ram[0] == 0x56 && ram[1] == 0x37 && ram[2] == 0x30)
	{
		memset(&ram[3], 0, S32_COMM_RAM_SIZE - 3);
		ram[0x08] = 0x5a;
		ram[0x09] = 0x38;
		ram[0x0a] = 0x30;
	}
	ram[4] = 0x00;
}

static uint8_t comm_read(uint32_t address)
{
	if (address < 0x801000)
		return s32_board.comm_ram[(address & 0xfff) >> 1];
	if (address == 0x801000)
		return (uint8_t)(s32_board.comm_cn | 0xfe);
	/* 0x801002: the flag the program sets, and the board's own (always clear: bit 7 reads 1) */
	return (uint8_t)(s32_board.comm_fg | 0xfe);
}

static void comm_write(uint32_t address, uint8_t data)
{
	if (address < 0x801000)
		s32_board.comm_ram[(address & 0xfff) >> 1] = data;
	else if (address == 0x801000)
	{
		s32_board.comm_cn = data & 1;
		comm_tick();
	}
	else if (s32_board.comm_cn)
		s32_board.comm_fg = data & 1;
}

/* ---- the bus ---- */

static uint16_t random_read(void)
{
	s32_board.random_state = 1664525u * s32_board.random_state + 1013904223u;
	return (uint16_t)(s32_board.random_state ^ (s32_board.random_state >> 15));
}

static uint16_t unmapped_read(uint32_t address)
{
	if (s32_board.unmapped_reads++ < 64)
		s32_log("unmapped read %06X at PC %06X", address, s32_board.pc);
	return 0xffff;
}

/* What boot writes where the board maps nothing, on every run: 16 bytes to E00000 and 4 to E80000
   (boot_register_values, boot_e80000_bytes). Counted, not logged */
static bool boot_write(uint32_t address)
{
	return (address >= 0xe00000u && address < 0xe00010u) || (address >= 0xe80000u && address < 0xe80004u);
}

static void unmapped_write(uint32_t address, uint16_t data, uint16_t mask)
{
	if (s32_board.unmapped_writes++ < 64 && !boot_write(address))
		s32_log("unmapped write %06X = %04X & %04X at PC %06X", address, data, mask, s32_board.pc);
}

static bool comm_mapped(uint32_t address)
{
	return address < 0x801000 || address == 0x801000 || address == 0x801002;
}

/* The halfword of the program's chips at 'address' (even, below 0x200000): from the image, or through what
   the port reads them with, byte by byte and only the bytes 'mask' wants (a halfword read at an odd address
   takes one byte of the aligned halfword; the other one may be code, which the game's data has not) */
static uint16_t program_read(uint32_t address, uint16_t mask)
{
	uint16_t value = 0;

	if (s32_board.program)
		return load16(&s32_board.program[address]);
	if (mask & 0x00ff)
		value |= *s32_board.program_at(address);
	if (mask & 0xff00)
		value |= (uint16_t)(*s32_board.program_at(address + 1) << 8);
	return value;
}

static uint16_t bus_read(uint32_t address, uint16_t mask)
{
	address &= 0xfffffe;
	switch (address >> 20)
	{
		case 0x0: case 0x1:
			return program_read(address, mask);
		case 0x2:
			return load16(&s32_board.work_ram[address & 0x1ffff]);
		case 0x3:
			return load16(&s32_board.video_ram[address & 0x1ffff]);
		case 0x4:
			return load16(&s32_board.sprite_ram[address & 0x1ffff]);
		case 0x5:
			return 0xff00 | sprite_control_read((address >> 1) & 7);
		case 0x6:
		{
			/* bit 19 picks the screen, bit 16 the mixer; bits 17-18 are mirrors */
			const int screen = (address >> 19) & 1;
			if (address & 0x10000)
				return s32_board.mixer[screen][(address >> 1) & 0x3f];
			return palette_read(screen, (address >> 1) & 0x7fff);
		}
		case 0x7:
			return load16(&s32_board.shared_ram[address & 0x1fff]);
		case 0x8:
			if (comm_mapped(address))
				return 0xff00 | comm_read(address);
			return unmapped_read(address);
		case 0xc:
			return 0xff00 | s32_io_read((address >> 19) & 1, address & 0x7f);
		case 0xd:
			if (address < 0xd80000)
				return (uint16_t)(int_control_read(address & 0xf) | (int_control_read((address & 0xf) + 1) << 8));
			return random_read();
		case 0xf:
			return program_read(address & 0xfffff, mask);
		default:
			(void)mask;
			return unmapped_read(address);
	}
}

static void bus_write(uint32_t address, uint16_t data, uint16_t mask)
{
	address &= 0xfffffe;
	switch (address >> 20)
	{
		case 0x2:
			store16(&s32_board.work_ram[address & 0x1ffff], data, mask);
			break;
		case 0x3:
			store16(&s32_board.video_ram[address & 0x1ffff], data, mask);
			break;
		case 0x4:
			store16(&s32_board.sprite_ram[address & 0x1ffff], data, mask);
			break;
		case 0x5:
			if (mask & 0x00ff)
				s32_board.sprite_control[(address >> 1) & 7] = (uint8_t)data;
			break;
		case 0x6:
		{
			const int screen = (address >> 19) & 1;
			if (address & 0x10000)
			{
				uint16_t *reg = &s32_board.mixer[screen][(address >> 1) & 0x3f];
				*reg = (uint16_t)((*reg & ~mask) | (data & mask));
			}
			else
				palette_write(screen, (address >> 1) & 0x7fff, data, mask);
			break;
		}
		case 0x7:
			store16(&s32_board.shared_ram[address & 0x1fff], data, mask);
			break;
		case 0x8:
			if (!comm_mapped(address))
				unmapped_write(address, data, mask);
			else if (mask & 0x00ff)
				comm_write(address, (uint8_t)data);
			break;
		case 0xc:
			if (mask & 0x00ff)
				s32_io_write((address >> 19) & 1, address & 0x7f, (uint8_t)data);
			break;
		case 0xd:
			if (address < 0xd80000)
			{
				if (mask & 0x00ff)
					int_control_write(address & 0xf, (uint8_t)data);
				if (mask & 0xff00)
					int_control_write((address & 0xf) + 1, (uint8_t)(data >> 8));
			}
			break;                              /* the random number seed write is ignored */
		case 0x0: case 0x1: case 0xf:
			break;                              /* ROM */
		default:
			unmapped_write(address, data, mask);
			break;
	}
}

/* Wider and unaligned accesses are split into 16-bit halves: on the V70's 32-bit bus the devices sit on
   the lanes MAME gives them, which reach the same bytes */

uint8_t s32_read8(uint32_t address)
{
	if (address & 1)
		return (uint8_t)(bus_read(address, 0xff00) >> 8);
	return (uint8_t)bus_read(address, 0x00ff);
}

uint16_t s32_read16(uint32_t address)
{
	if (address & 1)
		return (uint16_t)(s32_read8(address) | (s32_read8(address + 1) << 8));
	return bus_read(address, 0xffff);
}

uint32_t s32_read32(uint32_t address)
{
	if (address & 1)
		return s32_read8(address) | ((uint32_t)s32_read16(address + 1) << 8) | ((uint32_t)s32_read8(address + 3) << 24);
	return s32_read16(address) | ((uint32_t)s32_read16(address + 2) << 16);
}

void s32_write8(uint32_t address, uint8_t data)
{
	if (address & 1)
		bus_write(address, (uint16_t)(data << 8), 0xff00);
	else
		bus_write(address, data, 0x00ff);
}

void s32_write16(uint32_t address, uint16_t data)
{
	if (address & 1)
	{
		s32_write8(address, (uint8_t)data);
		s32_write8(address + 1, (uint8_t)(data >> 8));
	}
	else
		bus_write(address, data, 0xffff);
}

void s32_write32(uint32_t address, uint32_t data)
{
	if (address & 1)
	{
		s32_write8(address, (uint8_t)data);
		s32_write16(address + 1, (uint16_t)(data >> 8));
		s32_write8(address + 3, (uint8_t)(data >> 24));
	}
	else
	{
		s32_write16(address, (uint16_t)data);
		s32_write16(address + 2, (uint16_t)(data >> 16));
	}
}

/* ---- frame events ---- */

void s32_vblank_start(void)
{
	if (s32_board.frame_shown)
		s32_board.frame_shown();
	if (s32_board.controls_update)
		s32_board.controls_update();
	s32_signal_irq(S32_IRQ_VBLANK_START);
	comm_tick();
}

void s32_vblank_end(void)
{
	s32_signal_irq(S32_IRQ_VBLANK_END);
}

void s32_sprite_update(void)
{
	bool erase;
	bool draw;

	/* automatic mode renders every frame (control 3 bit 0 = 0) or every other frame */
	if (!(s32_board.sprite_control[3] & 2))
	{
		if (s32_board.sprite_render_count-- == 0)
		{
			s32_board.sprite_control[0] = 3;
			s32_board.sprite_render_count = s32_board.sprite_control[3] & 1;
		}
	}
	erase = (s32_board.sprite_control[0] & 2) != 0;
	draw = (s32_board.sprite_control[0] & 1) != 0;
	if (draw)
	{
		s32_board.sprite_buffer ^= 1;
		memcpy(s32_board.sprite_control_latched, s32_board.sprite_control, sizeof(s32_board.sprite_control_latched));
		s32_board.sprite_lists++;
	}
	s32_board.sprite_control[0] = 0;
	if (s32_board.sprites_due)
		s32_board.sprites_due(erase, draw);
}

/* ---- setup ---- */

void s32_reset(void)
{
	memset(s32_board.irq_control, 0xff, sizeof(s32_board.irq_control));
	s32_board.timer_deadline[0] = s32_board.timer_deadline[1] = 0;
	update_next_timer();
	s32_io_reset();
	s32_board.comm_cn = s32_board.comm_fg = 0;
	update_irq();
}

void s32_init(const uint8_t *program)
{
	void (*irq_changed)(bool) = s32_board.irq_changed;
	void (*controls_update)(void) = s32_board.controls_update;
	void (*log)(const char *) = s32_board.log;
	void (*frame_shown)(void) = s32_board.frame_shown;
	void (*sprites_due)(bool, bool) = s32_board.sprites_due;
	void (*time_passes)(uint64_t) = s32_board.time_passes;
	void (*sound_irq_changed)(bool, uint8_t) = s32_board.sound_irq_changed;
	void (*sound_reset_changed)(bool) = s32_board.sound_reset_changed;
	const s32_sound_chips chips = s32_board.chips;
	const uint8_t *sound_rom = s32_board.sound_rom;
	const uint8_t *(*sound_at)(uint32_t) = s32_board.sound_at;
	const uint8_t *pcm_rom = s32_board.pcm_rom;
	uint8_t (*pcm_at)(uint32_t) = s32_board.pcm_at;
	const bool frame_timing = s32_board.frame_timing;
	const uint8_t *(*program_at)(uint32_t) = s32_board.program_at;
	/* the work RAM: the world a program keeps its state in (the decompiled game's), or the board's own */
	static uint8_t own_work_ram[S32_WORK_RAM_SIZE];
	uint8_t *work_ram = s32_board.work_ram ? s32_board.work_ram : own_work_ram;

	/* callbacks, the sound chips, the sound ROMs, the timing and where the work RAM is survive; everything
	   else starts from power-on, the work RAM's contents too. A saved EEPROM is loaded after this. */
	memset(&s32_board, 0, sizeof(s32_board));
	s32_board.irq_changed = irq_changed;
	s32_board.controls_update = controls_update;
	s32_board.log = log;
	s32_board.frame_shown = frame_shown;
	s32_board.sprites_due = sprites_due;
	s32_board.time_passes = time_passes;
	s32_board.sound_irq_changed = sound_irq_changed;
	s32_board.sound_reset_changed = sound_reset_changed;
	s32_board.chips = chips;
	s32_board.sound_rom = sound_rom;
	s32_board.sound_at = sound_at;
	s32_board.pcm_rom = pcm_rom;
	s32_board.pcm_at = pcm_at;
	s32_board.frame_timing = frame_timing;
	s32_board.program = program;
	s32_board.program_at = program_at;
	s32_board.work_ram = work_ram;
	memset(work_ram, 0, S32_WORK_RAM_SIZE);
	memset(s32_board.eeprom.data, 0xff, sizeof(s32_board.eeprom.data));

	/* power-on contents, as in MAME's video start */
	store16(&s32_board.video_ram[0x1ff00], 0x8000, 0xffff);
	memset(s32_board.mixer, 0xff, sizeof(s32_board.mixer));
	s32_board.sprite_buffer = 1;
	s32_board.random_state = 0x9d14abd7;
	for (int chip = 0; chip < 2; chip++)
	{
		s32_board.input.port_a[chip] = s32_board.input.port_b[chip] = s32_board.input.port_c[chip] = 0xff;
		s32_board.input.port_e[chip] = s32_board.input.port_f[chip] = 0xff;
	}
	/* the wheels at the centre, the pedals up */
	s32_board.input.analog[0] = s32_board.input.analog[3] = 0x80;
	s32_reset();
}
