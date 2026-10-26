/**
 *    Name:        cpu.h
 *    Description: NEC V60 state for the C code generated from the game's program, and the frame loop
 *                 hooks that run board events and interrupts while the game waits for a frame
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's V60 core (src/devices/cpu/v60, Farfetch'd, R. Belmont): the generated code
 *    touches memory in the same order and leaves the same registers and flags as that core. Time follows
 *    the board's frame timing (s32/machine.h): instructions take none, and the board runs its events,
 *    and the CPU its interrupts, only in the game's frame loop. tools/core/system32/reference/s32ref runs the original
 *    program the same way, so the two compare frame by frame.
 */
#ifndef V60_CPU_H
#define V60_CPU_H

#include <stdbool.h>
#include <stdint.h>

#include "s32/bus.h"
#include "s32/machine.h"
#include "s32/program.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Register file layout, as MAME's core keeps it: R0-R28, AP, FP, SP, then the privileged registers
   that LDPR / STPR address as 36 + n */
enum
{
	V60_AP = 29,
	V60_FP = 30,
	V60_SP = 31,
	V60_ISP = 36,
	V60_L0SP = 37,
	V60_SBR = 41,
	V60_TR = 42,
	V60_SYCW = 43,
	V60_TKCW = 44,
	V60_PIR = 45,
	V60_PSW2 = 51,
	V60_REGISTERS = 68
};

#define V60_PSW_IE   (1u << 18)    /* maskable interrupts enabled */
#define V60_PSW_IS   (1u << 28)    /* running on the interrupt stack */

typedef struct v60_cpu
{
	uint32_t reg[V60_REGISTERS];
	uint32_t psw;           /* control bits; the condition flags below are folded in when read */
	uint8_t z;              /* condition flags, always 0 or 1 */
	uint8_t s;
	uint8_t ov;
	uint8_t cy;
	uint32_t pc;            /* set by returns and by interrupt entry; checked by callers */
	bool irq_line;          /* interrupt line as the board drives it */
	uint64_t frame;         /* frames completed */
	uint32_t interrupts;    /* interrupts taken */
} v60_cpu;

extern v60_cpu cpu;

/* Partial register writes, like MAME's SETREG8 / SETREG16 */
static inline void reg_write8(int index, uint32_t value) { cpu.reg[index] = (cpu.reg[index] & ~0xffu) | (value & 0xff); }
static inline void reg_write16(int index, uint32_t value) { cpu.reg[index] = (cpu.reg[index] & ~0xffffu) | (value & 0xffff); }

/* Stack, as JSR / BSR / RSR / PUSH / POP use it */
static inline void push32(uint32_t value)
{
	cpu.reg[V60_SP] -= 4;
	mem_write32(cpu.reg[V60_SP], value);
}

static inline uint32_t pop32(void)
{
	const uint32_t value = mem_read32(cpu.reg[V60_SP]);
	cpu.reg[V60_SP] += 4;
	return value;
}

void v60_reset(void);
uint32_t v60_read_psw(void);
void v60_write_psw(uint32_t value);
void v60_load_privileged(uint32_t number, uint32_t value);
uint32_t v60_read_privileged(uint32_t number);

/* SCHCU.B / SKPCU.B: the first byte of 'length' from 'address' that is (or is not) 'value'; R28 gets its
   address and R27 its index ('length' when there is none, which the generated code turns into Z) */
void v60_search_bytes(uint32_t address, uint32_t length, uint8_t value, bool until_equal);
void v60_call(uint32_t address);

/* A jump through a register: to the function that starts there, or, where none does, back to a caller
   whose return address the code took off the stack into the register (01CB8F: "pop R11" ... "jmp [R11]",
   a memory test that keeps out of the stack); the generated code then leaves as after a return */
void v60_jump(uint32_t address);
/* After a call from 'from': whether it came back to 'expected'. A callee that drops its caller's return
   address too (OutRunners 001544, "add.w #4, SP; rsr") comes back past it, and the caller leaves as
   well, up to the code the return goes to */
bool v60_returned_to(uint32_t expected, uint32_t from);
void v60_unknown_jump(uint32_t address, uint32_t from);

/* The game's frame loop polls 'frame_ready' until an interrupt sets it (OutRunners: 23[R24], 20E023).
   The generated code calls these where s32ref steps the board: before the poll, the board runs its next
   event if the game still waits, and the CPU takes the interrupts that event raised before the poll runs
   ('resume', where they return to, is the poll); where the wait is over, a frame is done. */
void v60_frame_wait(uint32_t frame_ready);
void v60_take_interrupts(uint32_t resume);
void v60_frame_end(void);    /* calls program_frame_done (s32/program.h) */

#ifdef __cplusplus
}
#endif

#endif
