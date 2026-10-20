/**
 *    Name:        rf5c68.c
 *    Description: Ricoh RF5C68 PCM sound chip: eight channels playing 8-bit samples from a 64 KB wave memory
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's sound/rf5c68.cpp (Olivier Galibert, Aaron Giles) sample for sample: the
 *    values its sound_stream_update() puts in the stream, as integers. tools/reference/sound/chipcheck
 *    compares the two. MAME's terms are in tools/reference/mame/LICENSE.
 *
 *    A channel that loops to a 0xFF stops where it is, silent, until its registers or the wave memory
 *    change: MAME leaves the channel for the rest of the samples it was asked for, and on the next ones
 *    it finds the same 0xFF again. Since the host makes the samples due before every change, one sample
 *    at a time comes to the same.
 */
#include "s32/rf5c68.h"

#include <string.h>

#define LOOP_MARK      0xff
#define ADDRESS_SHIFT  11            /* addresses and steps are 5.11 sample positions */
#define OUTPUT_BITS    10

static int32_t clamp16(int32_t value)
{
	if (value < -32768)
		return -32768;
	if (value > 32767)
		return 32767;
	return value;
}

static uint8_t wave(const s32_pcm *chip, uint32_t address)
{
	return chip->memory[(address >> ADDRESS_SHIFT) & 0xffff];
}

void s32_pcm_reset(s32_pcm *chip, uint8_t *memory)
{
	memset(chip, 0, sizeof(*chip));
	chip->memory = memory;
}

void s32_pcm_write(s32_pcm *chip, int offset, uint8_t data)
{
	s32_pcm_channel *channel = &chip->channels[chip->channel_bank];

	switch (offset)
	{
		case 0:
			channel->envelope = data;
			break;
		case 1:
			channel->pan = data;
			break;
		case 2:
			channel->step = (uint16_t)((channel->step & 0xff00) | data);
			break;
		case 3:
			channel->step = (uint16_t)((channel->step & 0x00ff) | data << 8);
			break;
		case 4:
			channel->loop = (uint16_t)((channel->loop & 0xff00) | data);
			break;
		case 5:
			channel->loop = (uint16_t)((channel->loop & 0x00ff) | data << 8);
			break;
		case 6:
			/* a channel that is off waits at its start */
			channel->start = data;
			if (!channel->enable)
				channel->address = (uint32_t)channel->start << (8 + ADDRESS_SHIFT);
			break;
		case 7:
			chip->enable = (data >> 7) & 1;
			if (data & 0x40)
				chip->channel_bank = data & 7;
			else
				chip->wave_bank = (uint16_t)((data & 0x0f) << 12);
			break;
		case 8:
			for (int index = 0; index < S32_PCM_CHANNELS; index++)
			{
				s32_pcm_channel *each = &chip->channels[index];

				each->enable = (~data >> index) & 1;
				if (!each->enable)
					each->address = (uint32_t)each->start << (8 + ADDRESS_SHIFT);
			}
			break;
		default:
			break;
	}
}

uint8_t s32_pcm_memory_read(const s32_pcm *chip, int offset)
{
	return chip->memory[(chip->wave_bank | (uint32_t)offset) & 0xffff];
}

void s32_pcm_memory_write(s32_pcm *chip, int offset, uint8_t data)
{
	chip->memory[(chip->wave_bank | (uint32_t)offset) & 0xffff] = data;
}

void s32_pcm_generate(s32_pcm *chip, int32_t out[2])
{
	int32_t left = 0;
	int32_t right = 0;

	out[0] = out[1] = 0;
	if (!chip->enable)
		return;

	for (int index = 0; index < S32_PCM_CHANNELS; index++)
	{
		s32_pcm_channel *channel = &chip->channels[index];

		if (!channel->enable)
			continue;

		const int left_volume = (channel->pan & 0x0f) * channel->envelope;
		const int right_volume = ((channel->pan >> 4) & 0x0f) * channel->envelope;
		int sample = wave(chip, channel->address);

		if (sample == LOOP_MARK)
		{
			channel->address = (uint32_t)channel->loop << ADDRESS_SHIFT;
			sample = wave(chip, channel->address);
			/* looped onto a loop mark: stuck */
			if (sample == LOOP_MARK)
				continue;
		}
		channel->address += channel->step;

		/* sign and magnitude */
		if (sample & 0x80)
		{
			sample &= 0x7f;
			left += (sample * left_volume) >> 5;
			right += (sample * right_volume) >> 5;
		}
		else
		{
			left -= (sample * left_volume) >> 5;
			right -= (sample * right_volume) >> 5;
		}
	}

	/* the chip's 10 bits, at the top of 16 */
	const int32_t drop = (1 << (16 - OUTPUT_BITS)) - 1;

	out[0] = clamp16(left & ~drop);
	out[1] = clamp16(right & ~drop);
}
