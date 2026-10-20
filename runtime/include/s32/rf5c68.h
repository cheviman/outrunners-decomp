/**
 *    Name:        rf5c68.h
 *    Description: Ricoh RF5C68 PCM sound chip: eight channels playing 8-bit samples from a 64 KB wave memory
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The System 32 sound board has one, on 50 MHz / 4: a stereo sample every 384 of its clocks. Each
 *    channel steps through the wave memory at its own pitch from a start address, jumps to its loop
 *    address when it reads 0xFF, and is scaled by its envelope and its left and right pan. The chip's
 *    output is 10 bits.
 *
 *    Registers (0-8): 0 envelope, 1 pan (right in the high nibble, left in the low), 2-3 step (low,
 *    high: 5.11 addresses a sample), 4-5 loop address (low, high), 6 start (the high byte of the
 *    address), each of the channel the control register selects; 7 control: bit 7 sounds the chip, bit
 *    6 set picks the channel in bits 0-2, bit 6 clear the 4 KB bank of the wave memory (bits 0-3) the
 *    host reaches; 8 channels on and off, a cleared bit is a channel on.
 *
 *    Samples are sign and magnitude: bit 7 set is positive. 0xFF marks a loop.
 *
 *    The chip changes only through these functions; a host that keeps time makes the samples due before
 *    every write, to the registers or to the wave memory.
 */
#ifndef S32_RF5C68_H
#define S32_RF5C68_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define S32_PCM_CHANNELS           8
#define S32_PCM_MEMORY_SIZE        0x10000
#define S32_PCM_CLOCKS_PER_SAMPLE  384

typedef struct s32_pcm_channel
{
	uint8_t enable;
	uint8_t envelope;
	uint8_t pan;
	uint8_t start;                  /* the address's high byte */
	uint32_t address;               /* 16.11: the sample, and how far into it */
	uint16_t step;                  /* 5.11 */
	uint16_t loop;                  /* where 0xFF sends the channel */
} s32_pcm_channel;

typedef struct s32_pcm
{
	uint8_t *memory;                /* the wave memory, 64 KB: not the chip's, the board's */
	s32_pcm_channel channels[S32_PCM_CHANNELS];
	uint8_t channel_bank;           /* the channel registers 0-6 write to */
	uint16_t wave_bank;             /* the 4 KB of the wave memory the host reaches */
	uint8_t enable;
} s32_pcm;

/* Power on, with the wave memory it plays from */
void s32_pcm_reset(s32_pcm *chip, uint8_t *memory);

void s32_pcm_write(s32_pcm *chip, int offset, uint8_t data);

/* The host's window on the wave memory: offsets 0-0xFFF in the bank the control register picked */
uint8_t s32_pcm_memory_read(const s32_pcm *chip, int offset);
void s32_pcm_memory_write(s32_pcm *chip, int offset, uint8_t data);

/* The next sample: left and right, the channels' sum cut to 10 bits (the low 6 cleared) and held to
   the 16-bit range */
void s32_pcm_generate(s32_pcm *chip, int32_t out[2]);

#ifdef __cplusplus
}
#endif

#endif
