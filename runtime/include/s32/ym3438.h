/**
 *    Name:        ym3438.h
 *    Description: Yamaha YM3438 (OPN2C) FM sound chip: six four-operator channels, the DAC, two timers
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The System 32 sound board has two, on 32.2159 MHz / 4. Each makes a stereo sample every 144 of its
 *    clocks: six channels of four operators (sine waves with an envelope that modulate each other in one
 *    of eight ways), the sixth of which can play 9-bit samples written to a register instead (the DAC).
 *    Two timers set bits of the status when they run out, and those bits drive the IRQ line.
 *
 *    The chip keeps no time of its own. Like ymfm's, it asks its host to time its timers and how long it
 *    stays busy after a write, through the callbacks at the top of s32_fm, and the host calls
 *    s32_fm_timer_expired() when a timer it was asked for runs out.
 *
 *    The interface (offsets 0-3): 0 selects a register 0x00-0xFF, 1 writes it; 2 selects a register
 *    0x100-0x1FF, 3 writes it. Reading offset 0 gives the status: bit 0 timer A, bit 1 timer B, bit 7 busy.
 */
#ifndef S32_YM3438_H
#define S32_YM3438_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S32_FM_CHANNELS            6
#define S32_FM_REGISTERS           0x200
#define S32_FM_CLOCKS_PER_SAMPLE   144

/* The stages of an operator's envelope */
enum
{
	S32_FM_ATTACK,
	S32_FM_DECAY,
	S32_FM_SUSTAIN,
	S32_FM_RELEASE,
	S32_FM_STAGES
};

/* One operator: a sine wave at a multiple of the channel's frequency, scaled by its envelope */
typedef struct s32_fm_operator
{
	uint16_t offset;                 /* of its registers, from 0x30, 0x40, ... 0x90 */
	uint32_t phase;                  /* 10.10: the top 10 bits are the point of the sine */
	uint16_t attenuation;            /* the envelope, 0 (loudest) to 0x3FF (silent) */
	uint8_t stage;                   /* S32_FM_ATTACK ... S32_FM_RELEASE */
	uint8_t ssg_inverted;            /* SSG-EG: the envelope is turned upside down */
	uint8_t key;                     /* key on, as the envelope last took it */
	uint8_t keys;                    /* who holds the key on: bit 0 register 0x28, bit 2 timer A in CSM */

	/* What the registers make of it, worked out again whenever a register changes */
	uint32_t block_frequency;        /* block (3 bits) and frequency number (11 bits) */
	int32_t detune;                  /* added to the phase step */
	uint32_t multiple;               /* x.1: 1 is a half */
	uint32_t step;                   /* phase step a sample, or 1: work it out each sample (LFO PM) */
	uint32_t total_level;            /* added to the envelope, 0-0x3F8 */
	uint32_t sustain_level;          /* where decay turns into sustain, 0-0x3E0 */
	uint8_t rates[S32_FM_STAGES];    /* each stage's rate, 0-63, key scaling included */
} s32_fm_operator;

typedef struct s32_fm_channel
{
	uint16_t offset;                 /* of its registers: 0-2, 0x100-0x102 */
	int16_t feedback[2];             /* operator 1's last two outputs */
	int16_t feedback_next;           /* its output of this sample, which goes in at the next */
	s32_fm_operator operators[4];    /* 1, 2, 3, 4: registers +0, +8, +4, +12 */
} s32_fm_channel;

typedef struct s32_fm
{
	/* The host: set before s32_fm_init(), which keeps them. Clocks are the chip's own; any callback may
	   be NULL (timers that never run out, a chip that is never busy). */
	void *host;
	void (*set_timer)(void *host, int timer, int32_t clocks);   /* timer 0 (A) or 1 (B) runs out in
	                                                               'clocks' from now; -1 stops it */
	void (*update_irq)(void *host, bool asserted);
	void (*set_busy_end)(void *host, uint32_t clocks);          /* busy from now on for 'clocks' */
	bool (*is_busy)(void *host);

	/* the interface */
	uint16_t address;                /* the register selected, 0x100 and up from offset 2 */
	uint16_t dac_data;               /* 9 bits, signed */
	uint8_t dac_enable;              /* channel 6 plays dac_data */

	/* the core */
	uint8_t registers[S32_FM_REGISTERS];
	uint32_t envelope_counter;       /* x.2: the envelopes move when the low two bits wrap, every third sample */
	uint32_t lfo_counter;
	uint8_t lfo_am;                  /* the LFO's amplitude modulation, 0-0x3F */
	uint8_t status;
	uint8_t irq_state;
	uint8_t timer_running[2];
	uint8_t total_clocks;            /* samples made, low 8 bits (timer B's first period is short by the low 4) */
	uint32_t active_channels;        /* channels that may be heard, as the last refresh found them */
	uint32_t modified_channels;      /* channels whose registers changed since the last refresh */
	uint32_t refresh_count;          /* samples since the last refresh: one every 4097 catches ending notes */
	s32_fm_channel channels[S32_FM_CHANNELS];
} s32_fm;

/* Power on: the host's fields are kept, everything else as the chip comes up, then s32_fm_reset() */
void s32_fm_init(s32_fm *chip);

/* The reset ymfm does: status, registers, timers, channels and operators (the LFO, the envelope counter
   and the DAC latch carry on) */
void s32_fm_reset(s32_fm *chip);

uint8_t s32_fm_read(s32_fm *chip, int offset);
void s32_fm_write(s32_fm *chip, int offset, uint8_t data);

/* The host's timer 0 (A) or 1 (B) ran out */
void s32_fm_timer_expired(s32_fm *chip, int timer);

/* The next sample: left and right */
void s32_fm_generate(s32_fm *chip, int32_t out[2]);

#ifdef __cplusplus
}
#endif

#endif
