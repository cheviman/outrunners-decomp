/**
 *    Name:        machine.h
 *    Description: Sega System Multi 32 board as the OutRunners program sees it: memory map, interrupts,
 *                 timers, I/O
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME: sega/segas32.cpp, segas32_v.cpp, segas32_m.cpp (Aaron Giles),
 *    sega/315_5296.cpp (hap), sega/s32comm.cpp (Ariane Fugmann), machine/eepromser.cpp and
 *    machine/eeprom.cpp (Aaron Giles), machine/msm6253.cpp (AJR). MAME's terms are in
 *    tools/core/system32/reference/mame/LICENSE.
 *
 *    The Multi 32 is the System 32 for two screens: the same memory map, with a V70 on a 32-bit bus,
 *    twice the work RAM, a palette and a mixer for each screen, two I/O chips and, on OutRunners' board,
 *    an A/D converter for the wheels and pedals. The names keep the family's prefix, s32.
 *
 *    The V70 side is reached through s32_read* / s32_write*: a little-endian bus with 24 address lines.
 *    Its devices sit on the byte lanes MAME's map gives them, so the board takes every access as 16-bit
 *    halves, as on the System 32: a 32-bit access is the two halves in order.
 *
 *    Time runs one of two ways. With cycle timing the caller advances s32_board.cycles as the CPU runs
 *    and calls the frame events itself (tools/core/system32/reference/s32ref, to follow the real CPU's pace). With
 *    frame timing the program takes no time at all: when it waits for the next frame it calls
 *    s32_wait(), which moves the board to its next event (a timer, the start or end of VBLANK, the
 *    sprite list) and runs it. That is the CPU of a decompiled program, and it never lags.
 */
#ifndef S32_MACHINE_H
#define S32_MACHINE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S32_CPU_CLOCK        20000000u    /* 40 MHz crystal / 2: the V70 */
#define S32_FRAME_RATE       60
#define S32_LINES_PER_FRAME  262
#define S32_VISIBLE_LINES    224
#define S32_SCREENS          2

#define S32_PROGRAM_SIZE     0x200000
#define S32_WORK_RAM_SIZE    0x20000
#define S32_VIDEO_RAM_SIZE   0x20000
#define S32_SPRITE_RAM_SIZE  0x20000
#define S32_PALETTE_ENTRIES  0x4000       /* a screen's */
#define S32_MIXER_REGISTERS  0x40         /* a screen's */
#define S32_SHARED_RAM_SIZE  0x2000
#define S32_COMM_RAM_SIZE    0x800        /* the link board's memory, on the low byte lanes at 0x800000 */
#define S32_EEPROM_WORDS     64
#define S32_ANALOG_INPUTS    8            /* MAME's ANALOG1-8 */

/* The sound board: a Z80 and a YM3438 on 32 MHz / 4, a MultiPCM on 40 MHz / 4 */
#define S32_SOUND_CLOCK      8000000u
#define S32_MULTIPCM_CLOCK   10000000u
#define S32_SOUND_ROM_SIZE   0x80000      /* the Z80 program: 0x0000-0x9FFF, then 8 KB pages at 0xA000 */
#define S32_PCM_ROM_SIZE     0x400000     /* the MultiPCM's samples */

/* Work RAM left out of state comparisons: the V70's stacks, which a decompiled program does not have.
   Boot starts the first task's at 0x21F400; the second task's goes down from 0x21F800 and its interrupt
   frames from 0x220000; where each task's stack pointers wait while the other runs (0x20E000-0x20E007,
   0[R24] and 4[R24]) */
#define S32_STACK_START      0x1f000
#define S32_STACK_END        0x20000
#define S32_TASK_STACKS      0xe000
#define S32_TASK_STACKS_SIZE 8
/* The test mode's work RAM test keeps a copy of the work RAM in the sprite RAM while it checks it, and leaves
   it there: the copy of what is left out above is left out of the sprite RAM too (the game uses those
   places of the sprite RAM for nothing else) */
/* Its sound RAM test does the same with the sound board's RAM (0x700000-0x701FFF to the sprite RAM's
   0x0000-0x1FFF): the copy of the Z80's stack (0xFD00-0xFDFF), which the decompiled sound program does not
   have either (sound_board_state_hash leaves it out), is left out of the sprite RAM too */
#define S32_SOUND_STACK_COPY      0x1d00
#define S32_SOUND_STACK_COPY_SIZE 0x100

/* Interrupt controller sources, highest priority first */
enum
{
	S32_IRQ_VBLANK_START,
	S32_IRQ_VBLANK_END,
	S32_IRQ_SOUND,
	S32_IRQ_TIMER0,
	S32_IRQ_TIMER1
};

/* Sound interrupt controller inputs */
enum
{
	S32_SOUND_IRQ_FM,           /* the YM3438's timers */
	S32_SOUND_IRQ_V60           /* the V70 writes the interrupt controller's 0x0C-0x0F */
};

/* The sound chips as the Z80 reaches them: the reference plugs in MAME's, the game its own */
typedef struct s32_sound_chips
{
	uint8_t (*fm_read)(int offset);                         /* YM3438, offsets 0-3 */
	void (*fm_write)(int offset, uint8_t data);
	uint8_t (*pcm_read)(int offset);                        /* MultiPCM, offsets 0-0x1FFF of 0xC000 */
	void (*pcm_write)(int offset, uint8_t data);
} s32_sound_chips;

/* Controls, active low like the board: a cleared bit is a pressed button. [0] is the first I/O chip,
   [1] the second (MAME's _A and _B ports). OutRunners: port A bits 0-1 shift up and down, port B bits
   0-2 the DJ/music, << and >> buttons, of player 1 on [0] and player 2 on [1]; port E bits 0 service,
   1 test, 2 coin, 4 start (coin 1 and start 1 on [0], coin 2 and start 2 on [1]); port F bits 0-3 the
   DIP switches and 4-5 the PCB buttons on [0], 4-5 the second board's buttons on [1], bit 7 the EEPROM
   data line on both */
typedef struct s32_inputs
{
	uint8_t port_a[2];
	uint8_t port_b[2];
	uint8_t port_c[2];
	uint8_t port_e[2];
	uint8_t port_f[2];
	uint8_t analog[S32_ANALOG_INPUTS];   /* ANALOG1-8: player 1's wheel, gas, brake, player 2's wheel,
	                                        two unused, player 2's gas and brake (wheels centre on 0x80) */
} s32_inputs;

/* 93C46 serial EEPROM in 16-bit mode */
typedef struct s32_eeprom
{
	uint16_t data[S32_EEPROM_WORDS];
	int state;
	int command;
	int address;
	int bits;
	uint32_t shift;
	uint32_t command_bits;
	bool cs;
	bool clk;
	bool di;
	bool locked;
	uint64_t busy_until;         /* V70 cycle when the internal write or erase ends */
	bool dirty;                  /* contents changed since the last save */
} s32_eeprom;

typedef struct s32_machine
{
	const uint8_t *program;                        /* V70 program image (2 MB), owned by the caller, or NULL */
	const uint8_t *(*program_at)(uint32_t address);  /* without an image: the bytes at an address of the
	                                                  program's chips (the game's assets: data_at) */
	uint8_t *work_ram;                              /* S32_WORK_RAM_SIZE bytes: the state of the world the
	                                                  program runs (the game's, runtime/src/game.c), or the
	                                                  board's own; s32_init keeps it and clears it */
	uint8_t video_ram[S32_VIDEO_RAM_SIZE];          /* 16-bit words, little endian */
	uint8_t sprite_ram[S32_SPRITE_RAM_SIZE];        /* 16-bit words, little endian */
	uint16_t palette[S32_SCREENS][S32_PALETTE_ENTRIES];    /* stored as xBBBBBGGGGGRRRRR */
	uint16_t mixer[S32_SCREENS][S32_MIXER_REGISTERS];
	uint8_t shared_ram[S32_SHARED_RAM_SIZE];        /* shared with the Z80 sound CPU */

	uint8_t sprite_control[8];
	uint8_t sprite_control_latched[8];
	uint8_t sprite_buffer;                          /* bit read back at 0x500000 */
	int sprite_render_count;
	uint32_t sprite_lists;                          /* sprite lists sent to the renderer so far */

	uint8_t irq_control[16];
	uint64_t timer_deadline[2];                     /* cycle when timer 0/1 fires; 0 = stopped */
	uint64_t next_timer;                            /* earliest timer deadline, UINT64_MAX when none */
	uint64_t cycles;                                /* board time, in V70 cycles since reset */
	bool frame_timing;                              /* see the top of this file */
	uint64_t frame;                                 /* frame whose events come next */
	int frame_phase;                                /* frame timing: 0 = VBLANK start next, 1 = its end, 2 = sprites */

	uint8_t io_latch[2][8];                         /* the two 315-5296s' ports A-H */
	uint8_t io_direction[2];
	uint8_t io_cnt[2];                              /* bit 1: the chip's screen is on; chip 0's bit 2 lets the
	                                                   sound CPU run */
	uint8_t tilebank_external;                      /* chip 0's port H */
	uint8_t outputs[2][2];                          /* each chip's ports D and G: coin counters, lamps, the wheel
	                                                   motors and the LCD (nothing reads them back) */
	s32_inputs input;
	uint8_t analog_bank;                            /* which pair of inputs the A/D converter's channels 2-3 read */
	uint8_t adc_shift;                              /* the A/D converter's shift register */
	s32_eeprom eeprom;

	uint8_t comm_ram[S32_COMM_RAM_SIZE];            /* the link board: its memory and registers, with no other
	                                                   cabinet on the line */
	uint8_t comm_cn;
	uint8_t comm_fg;

	const uint8_t *sound_rom;                       /* 512 KB, owned by the caller; set after s32_init */
	const uint8_t *(*sound_at)(uint32_t offset);    /* without sound_rom: the sound program's data at an offset
	                                                   of its chip, NULL where it has none (the game's assets:
	                                                   assets_sound_data) */
	const uint8_t *pcm_rom;                         /* 4 MB, owned by the caller; set after s32_init */
	uint8_t (*pcm_at)(uint32_t place);              /* without pcm_rom: a byte of the 4 MB at a place (the
	                                                   game's samples: assets_pcm_byte) */
	uint8_t sound_bank;                             /* the 8 KB page of the sound ROM at Z80 0xA000 */
	uint8_t pcm_bank;                               /* the MultiPCM's banks: bits 0-2 its 0x100000-0x17FFFF,
	                                                   bits 3-5 its 0x180000-0x1FFFFF, 512 KB pages */
	uint8_t sound_irq_control[4];                   /* the input of vectors 0-2, and the mask */
	uint8_t sound_irq_input;                        /* pending, by vector */
	uint8_t sound_spare;                            /* the byte at Z80 port 0xF1 */
	uint64_t sound_cycles;                          /* when the Z80's reads and writes happen (board cycles) */
	s32_sound_chips chips;

	uint32_t random_state;
	uint32_t sound_requests;                        /* V70 -> Z80 interrupt requests */
	uint32_t unmapped_reads;
	uint32_t unmapped_writes;
	uint32_t pc;                                    /* instruction being executed, for diagnostics */

	void (*irq_changed)(bool asserted);             /* interrupt line to the V70 */
	void (*controls_update)(void);                  /* when VBLANK starts, where MAME polls the controls */
	void (*frame_shown)(void);                      /* when VBLANK starts: the video draws the frame (MAME's
	                                                   screen update) from the memories as they are */
	void (*sprites_due)(bool erase, bool draw);     /* when the sprite list goes out, once the board took the
	                                                   commands: erase the shown buffer, and swap the buffers
	                                                   and draw the list (with the controls just latched) */
	void (*time_passes)(uint64_t until);            /* the board's time is about to move on to 'until': the
	                                                   sound board runs up to then */
	void (*sound_irq_changed)(bool asserted, uint8_t vector);   /* interrupt line to the Z80 */
	void (*sound_reset_changed)(bool held);         /* the first I/O chip's CNT2 output holds the Z80 in reset */
	void (*log)(const char *message);
} s32_machine;

extern s32_machine s32_board;

void s32_init(const uint8_t *program);
void s32_reset(void);

uint8_t s32_read8(uint32_t address);
uint16_t s32_read16(uint32_t address);
uint32_t s32_read32(uint32_t address);
void s32_write8(uint32_t address, uint8_t data);
void s32_write16(uint32_t address, uint16_t data);
void s32_write32(uint32_t address, uint32_t data);

void s32_signal_irq(int source);
bool s32_irq_asserted(void);
uint8_t s32_irq_acknowledge(void);

uint64_t s32_next_timer(void);
void s32_run_timers(void);

void s32_vblank_start(void);
void s32_vblank_end(void);
void s32_sprite_update(void);   /* MAME runs it 50 us after the end of VBLANK */

/* Whether a screen is on: the CNT1 output of its I/O chip */
bool s32_display_enabled(int screen);

/* The Z80's side of the sound board (sound.c): its memory (the program, the banked page of the sound
   ROM, the MultiPCM and the shared RAM) and its I/O ports (the YM3438, the banks, the interrupt
   controller) */
uint8_t s32_z80_read(uint16_t address);
void s32_z80_write(uint16_t address, uint8_t data);
uint8_t s32_z80_in(uint8_t port);
void s32_z80_out(uint8_t port, uint8_t data);
void s32_sound_signal_irq(int input);
void s32_sound_clear_irq(int input);

/* A byte of the MultiPCM's address space (22 bits) through the banks: the chip's sample memory */
uint8_t s32_pcm_sample_read(uint32_t address);

/* Board time when a frame starts (VBLANK ends) and when one of its lines starts */
uint64_t s32_frame_start(uint64_t frame);
uint64_t s32_line_start(uint64_t frame, int line);

/* Frame timing: the program waits, the board runs its next event (it may raise an interrupt) */
void s32_wait(void);

/* Hash of the state a frame leaves behind: work RAM but the stack, video RAM, sprite RAM, the palettes
   and the mixers (FNV-1a, 64 bits) */
uint64_t s32_state_hash(void);

#ifdef __cplusplus
}
#endif

#endif
