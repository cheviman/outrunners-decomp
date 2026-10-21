/**
 *    Name:        multipcm.h
 *    Description: Yamaha YMW-258-F (Sega 315-5560) "MultiPCM": the Multi 32's sample player
 *    Version:     0.1.0
 *    Created:     21-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    28 voices that play 8- or 12-bit samples from a ROM of up to 4 MB (the board banks it), each with an
 *    envelope, a pitch and an amplitude LFO, a level that may glide, and a pan, into two outputs.
 *    Behavior follows MAME's sound/multipcm.cpp and sound/gew.cpp (Miguel Angel Horna) sample for sample,
 *    the float tables included; MAME's terms are in tools/reference/mame/LICENSE.
 *
 *    The host makes the samples: multipcm_generate() with the next ones, as the clock divided by 224
 *    gives them. A register write changes what the samples after it are, so the host makes the ones due
 *    before it first (as MAME's stream update does), and likewise before the board changes its banks.
 */
#ifndef S32_MULTIPCM_H
#define S32_MULTIPCM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MULTIPCM_VOICES 28

typedef struct multipcm_lfo
{
	uint16_t phase;
	uint32_t phase_step;
	const int32_t *table;
	const int32_t *scale;
} multipcm_lfo;

typedef struct multipcm_voice
{
	uint8_t regs[11];
	bool playing;
	/* the sample, from the header the ROM has for it */
	uint32_t start;
	uint32_t loop;
	uint32_t end;
	uint8_t attack_reg;
	uint8_t decay1_reg;
	uint8_t decay2_reg;
	uint8_t decay_level_reg;
	uint8_t release_reg;
	uint8_t key_rate_scale;
	uint8_t lfo_vibrato_reg;
	uint8_t lfo_amplitude_reg;
	uint8_t format;
	/* playing it */
	uint32_t offset;            /* in the sample, 20.12 */
	uint8_t octave;
	uint16_t pitch;
	uint32_t step;
	bool reverse;
	uint32_t pan;
	uint8_t dsp_send;
	uint32_t total_level;       /* 7.12, from the top: 0 is the loudest */
	uint32_t dest_total_level;
	int32_t total_level_step;
	int32_t prev_sample;
	/* the envelope */
	int32_t volume;             /* 10.16 */
	uint8_t state;              /* attack, decay 1, decay 2, release */
	bool reverb;
	int32_t attack_rate;
	int32_t decay1_rate;
	int32_t decay2_rate;
	int32_t release_rate;
	int32_t decay_level;
	/* the LFOs */
	uint8_t lfo_frequency;
	multipcm_lfo pitch_lfo;
	uint8_t vibrato;
	multipcm_lfo amplitude_lfo;
	uint8_t tremolo;
} multipcm_voice;

typedef struct multipcm
{
	uint8_t (*read)(uint32_t address);    /* a byte of the sample ROM, 22 bits of address */
	float rate;                           /* samples a second */
	multipcm_voice voices[MULTIPCM_VOICES];
	int current_voice;
	uint8_t address;
} multipcm;

/* A chip on 'clock' (Hz) reading its samples through 'read'; keys off */
void multipcm_init(multipcm *chip, uint32_t clock, uint8_t (*read)(uint32_t address));
void multipcm_reset(multipcm *chip);

/* The bus: offset 0 writes the register 'address' picks of the voice offset 1 picked, offset 2 picks the
   register; the chip reads back 0 */
void multipcm_write(multipcm *chip, int offset, uint8_t data);
uint8_t multipcm_read(multipcm *chip);

/* The next 'count' samples of each output, between -32768 and 32767 */
void multipcm_generate(multipcm *chip, int32_t *left, int32_t *right, int count);

#ifdef __cplusplus
}
#endif

#endif
