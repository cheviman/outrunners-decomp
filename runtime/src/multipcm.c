/**
 *    Name:        multipcm.c
 *    Description: Yamaha YMW-258-F (Sega 315-5560) "MultiPCM": the Multi 32's sample player
 *    Version:     0.1.0
 *    Created:     21-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME's sound/multipcm.cpp and sound/gew.cpp (Miguel Angel Horna) sample for sample:
 *    the tables are worked out with the same float arithmetic, and every sample the same way.
 *    tools/reference/sound/chipcheck.cpp compares the two. MAME's terms are in
 *    tools/reference/mame/LICENSE.
 *
 *    Voice registers: 0 pan (high nibble) and effect send (low); 1 the sample (bit 0 of register 2 is its
 *    ninth bit); 2-3 the pitch (ooooppppppppppxx: a 4-bit signed octave and 10 bits); 4 key on (bit 7); 5
 *    the level to reach (bits 1-7, 0 the loudest) and whether to glide to it (bit 0 clear); 6 the LFO
 *    frequency and pitch depth; 7-9 the envelope (attack, decay 1, decay level, decay 2, key rate scale,
 *    release); 10 the amplitude depth. A sample's header in the ROM, 12 bytes at 12 x its number, loads
 *    6-10 when register 1 is written.
 */
#include "s32/multipcm.h"

#include <math.h>
#include <string.h>

#define TL_SHIFT   12
#define EG_SHIFT   16
#define LFO_SHIFT  8
#define CLOCK_DIVIDER 224

enum
{
	ATTACK,
	DECAY1,
	DECAY2,
	RELEASE
};

/* Times are based on a 44100 Hz timebase */
static const double base_times[64] = {
	0,          0,          0,          0,
	6222.95,    4978.37,    4148.66,    3556.01,
	3111.47,    2489.21,    2074.33,    1778.00,
	1555.74,    1244.63,    1037.19,    889.02,
	777.87,     622.31,     518.59,     444.54,
	388.93,     311.16,     259.32,     222.27,
	194.47,     155.60,     129.66,     111.16,
	97.23,      77.82,      64.85,      55.60,
	48.62,      38.91,      32.43,      27.80,
	24.31,      19.46,      16.24,      13.92,
	12.15,      9.75,       8.12,       6.98,
	6.08,       4.90,       4.08,       3.49,
	3.04,       2.49,       2.13,       1.90,
	1.72,       1.41,       1.18,       1.04,
	0.91,       0.73,       0.59,       0.50,
	0.45,       0.45,       0.45,       0.45
};

static const float lfo_frequencies[8] = { 0.168f, 2.019f, 3.196f, 4.206f, 5.215f, 5.888f, 6.224f, 7.066f };    /* Hz */
static const float phase_scale_limits[8] = { 0.0f, 3.378f, 5.065f, 6.750f, 10.114f, 20.170f, 40.180f, 79.307f };    /* cents */
static const float amplitude_scale_limits[8] = { 0.0f, 0.4f, 0.8f, 1.5f, 3.0f, 6.0f, 12.0f, 24.0f };    /* dB */

/* register 1's value -> voice: every eighth number picks none */
static const int value_to_voice[32] = {
	0, 1, 2, 3, 4, 5, 6, -1,
	7, 8, 9, 10, 11, 12, 13, -1,
	14, 15, 16, 17, 18, 19, 20, -1,
	21, 22, 23, 24, 25, 26, 27, -1
};

/* The tables, the same for every chip on the same clock (MAME works them out in device_start) */
static struct
{
	float rate;
	uint32_t attack_step[0x40];
	uint32_t decay_release_step[0x40];
	uint32_t freq_step[0x400];
	int32_t left_pan[0x800];
	int32_t right_pan[0x800];
	int32_t linear_to_exp_volume[0x400];
	int32_t total_level_steps[2];
	int32_t pitch_table[256];
	int32_t pitch_scale_tables[8][256];
	int32_t amplitude_table[256];
	int32_t amplitude_scale_tables[8][256];
} tables;

static uint32_t value_to_fixed(uint32_t bits, float value)
{
	const float float_shift = (float)(1 << bits);
	return (uint32_t)(float_shift * value);
}

static void make_tables(float rate)
{
	if (tables.rate == rate)
		return;
	tables.rate = rate;

	/* volume and pan */
	for (int32_t level = 0; level < 0x80; ++level)
	{
		const float vol_db = (float)level * (-24.0f) / 64.0f;
		const float total_level = powf(10.0f, vol_db / 20.0f) / 4.0f;

		for (int32_t pan = 0; pan < 0x10; ++pan)
		{
			float pan_left;
			float pan_right;

			if (pan == 0x8)
			{
				pan_left = 0.0f;
				pan_right = 0.0f;
			}
			else if (pan == 0x0)
			{
				pan_left = 1.0f;
				pan_right = 1.0f;
			}
			else if (pan & 0x8)
			{
				const int32_t inverted_pan = 0x10 - pan;
				const float pan_vol_db = (float)inverted_pan * (-12.0f) / 4.0f;

				pan_left = 1.0f;
				pan_right = powf(10.0f, pan_vol_db / 20.0f);
				if ((inverted_pan & 0x7) == 7)
					pan_right = 0.0f;
			}
			else
			{
				const float pan_vol_db = (float)pan * (-12.0f) / 4.0f;

				pan_right = 1.0f;
				pan_left = powf(10.0f, pan_vol_db / 20.0f);
				if ((pan & 0x7) == 7)
					pan_left = 0.0f;
			}
			tables.left_pan[(pan << 7) | level] = (int32_t)value_to_fixed(TL_SHIFT, pan_left * total_level);
			tables.right_pan[(pan << 7) | level] = (int32_t)value_to_fixed(TL_SHIFT, pan_right * total_level);
		}
	}

	/* pitch steps */
	for (int32_t i = 0; i < 0x400; ++i)
	{
		const float fcent = rate * (1024.0f + (float)i) / 1024.0f;
		tables.freq_step[i] = value_to_fixed(TL_SHIFT, fcent);
	}

	/* envelope steps: the times are for 44100 Hz, decays 14.32833 times the attacks */
	for (int32_t i = 4; i < 0x40; ++i)
	{
		tables.attack_step[i] = (uint32_t)((float)(0x400 << EG_SHIFT) / (float)(base_times[i] * 44100.0 / 1000.0));
		tables.decay_release_step[i] = (uint32_t)((float)(0x400 << EG_SHIFT) / (float)(base_times[i] * 14.32833 * 44100.0 / 1000.0));
	}
	tables.attack_step[0] = tables.attack_step[1] = tables.attack_step[2] = tables.attack_step[3] = 0;
	tables.attack_step[0x3f] = 0x400 << EG_SHIFT;
	tables.decay_release_step[0] = tables.decay_release_step[1] = tables.decay_release_step[2] = tables.decay_release_step[3] = 0;

	/* level glides */
	tables.total_level_steps[0] = (int32_t)(-(float)(0x80 << TL_SHIFT) / (78.2f * 44100.0f / 1000.0f));    /* down */
	tables.total_level_steps[1] = (int32_t)((float)(0x80 << TL_SHIFT) / (78.2f * 2 * 44100.0f / 1000.0f));  /* up */

	/* linear to exponential volume */
	for (int32_t i = 0; i < 0x400; ++i)
	{
		const float db = -(96.0f - (96.0f * (float)i / (float)0x400));
		const float exp_volume = powf(10.0f, db / 20.0f);
		tables.linear_to_exp_volume[i] = (int32_t)value_to_fixed(TL_SHIFT, exp_volume);
	}

	/* the LFOs' waves and depths */
	for (int32_t i = 0; i < 256; ++i)
	{
		if (i < 64)
			tables.pitch_table[i] = i * 2 + 128;
		else if (i < 128)
			tables.pitch_table[i] = 383 - i * 2;
		else if (i < 192)
			tables.pitch_table[i] = 384 - i * 2;
		else
			tables.pitch_table[i] = i * 2 - 383;
		tables.amplitude_table[i] = i < 128 ? 255 - (i * 2) : (i * 2) - 256;
	}
	for (int32_t table = 0; table < 8; ++table)
	{
		float limit = phase_scale_limits[table];

		for (int32_t i = -128; i < 128; ++i)
		{
			const float value = (limit * (float)i) / 128.0f;
			const float converted = powf(2.0f, value / 1200.0f);
			tables.pitch_scale_tables[table][i + 128] = (int32_t)value_to_fixed(LFO_SHIFT, converted);
		}
		limit = -amplitude_scale_limits[table];
		for (int32_t i = 0; i < 256; ++i)
		{
			const float value = (limit * (float)i) / 256.0f;
			const float converted = powf(10.0f, value / 20.0f);
			tables.amplitude_scale_tables[table][i] = (int32_t)value_to_fixed(LFO_SHIFT, converted);
		}
	}
}

/* ---- the envelope ---- */

static uint32_t get_rate(const uint32_t *steps, int32_t rate, uint32_t value)
{
	int r;

	if (value == 0)
		return steps[0];
	if (value == 0xf)
		return steps[0x3f];
	r = 4 * (int)value + rate;
	r = r < 0 ? 0 : (r > 0x3f ? 0x3f : r);
	return steps[r];
}

static void envelope_calc(multipcm_voice *voice)
{
	int32_t octave = voice->octave;
	int32_t rate;

	if (octave & 8)
		octave = octave - 16;
	if (voice->key_rate_scale != 0xf)
		rate = (octave + voice->key_rate_scale) * 2 + ((voice->pitch >> 9) & 1);
	else
		rate = 0;
	voice->attack_rate = (int32_t)get_rate(tables.attack_step, rate, voice->attack_reg);
	voice->decay1_rate = (int32_t)get_rate(tables.decay_release_step, rate, voice->decay1_reg);
	voice->decay2_rate = (int32_t)get_rate(tables.decay_release_step, rate, voice->decay2_reg);
	voice->release_rate = (int32_t)get_rate(tables.decay_release_step, rate, voice->release_reg);
	voice->decay_level = 0xf - voice->decay_level_reg;
	voice->reverb = false;
}

static int32_t envelope_update(multipcm_voice *voice)
{
	switch (voice->state)
	{
		case ATTACK:
			voice->volume += (int32_t)(((int64_t)((0x817 << (EG_SHIFT - 1)) - voice->volume) * voice->attack_rate) >> 24);
			if (voice->volume >= (0x3ff << EG_SHIFT))
			{
				voice->state = DECAY1;
				if (voice->decay1_rate >= (0x400 << EG_SHIFT))    /* skip decay 1 */
					voice->state = DECAY2;
				voice->volume = 0x3ff << EG_SHIFT;
			}
			break;
		case DECAY1:
			voice->volume -= voice->decay1_rate;
			if (voice->volume <= 0)
				voice->volume = 0;
			if (voice->volume >> (EG_SHIFT + 6) <= voice->decay_level)
				voice->state = DECAY2;
			break;
		case DECAY2:
			voice->volume -= voice->decay2_rate;
			if (voice->volume <= 0)
				voice->volume = 0;
			break;
		case RELEASE:
			voice->volume -= voice->release_rate;
			if (voice->volume <= 0)
			{
				voice->volume = 0;
				voice->playing = false;
			}
			break;
		default:
			return 1 << TL_SHIFT;
	}
	/* (MAME's GEW7 reverb: never set on the MultiPCM) */
	if (voice->reverb && voice->state != ATTACK && (voice->volume >> EG_SHIFT) <= 0x300)
	{
		voice->decay1_rate = (int32_t)tables.decay_release_step[17];
		voice->decay2_rate = (int32_t)tables.decay_release_step[17];
		voice->release_rate = (int32_t)tables.decay_release_step[17];
	}
	return tables.linear_to_exp_volume[voice->volume >> EG_SHIFT];
}

/* ---- the LFOs ---- */

static int32_t lfo_step(multipcm_lfo *lfo)
{
	int32_t p;

	lfo->phase = (uint16_t)(lfo->phase + lfo->phase_step);
	p = lfo->table[(lfo->phase >> LFO_SHIFT) & 0xff];
	p = lfo->scale[p];
	return p << (TL_SHIFT - LFO_SHIFT);
}

static void lfo_compute_step(const multipcm *chip, multipcm_lfo *lfo, uint32_t frequency, uint32_t scale, bool amplitude)
{
	const float step = (float)lfo_frequencies[frequency] * 256.0f / (float)chip->rate;

	lfo->phase_step = (uint32_t)((float)(1 << LFO_SHIFT) * step);
	if (amplitude)
	{
		lfo->table = tables.amplitude_table;
		lfo->scale = tables.amplitude_scale_tables[scale];
	}
	else
	{
		lfo->table = tables.pitch_table;
		lfo->scale = tables.pitch_scale_tables[scale];
	}
}

/* ---- the voices ---- */

static void retrigger(multipcm_voice *voice)
{
	voice->offset = 0;
	voice->prev_sample = 0;
	voice->total_level = voice->dest_total_level << TL_SHIFT;
	envelope_calc(voice);
	voice->state = ATTACK;
	voice->volume = (0x3ff - 0x2a0) << EG_SHIFT;
}

static void update_step(const multipcm *chip, multipcm_voice *voice)
{
	const uint8_t octave = (uint8_t)((voice->octave - 1) & 0xf);
	uint32_t pitch = tables.freq_step[voice->pitch];

	if (octave & 0x8)
		pitch >>= (16 - octave);
	else
		pitch <<= octave;
	voice->step = (uint32_t)((float)pitch / chip->rate);
}

static void init_sample(multipcm *chip, multipcm_voice *voice, uint32_t index)
{
	const uint32_t address = index * 12;
	uint8_t header[12];

	for (int byte = 0; byte < 12; byte++)
		header[byte] = chip->read((address + (uint32_t)byte) & 0x3fffff);
	voice->start = (uint32_t)header[0] << 16 | (uint32_t)header[1] << 8 | header[2];
	voice->format = (uint8_t)((voice->start >> 20) & 0xfe);
	voice->start &= 0x3fffff;
	voice->loop = (uint32_t)header[3] << 8 | header[4];
	voice->end = 0x10000 - ((uint32_t)header[5] << 8 | header[6]);
	voice->attack_reg = (header[8] >> 4) & 0xf;
	voice->decay1_reg = header[8] & 0xf;
	voice->decay2_reg = header[9] & 0xf;
	voice->decay_level_reg = (header[9] >> 4) & 0xf;
	voice->release_reg = header[10] & 0xf;
	voice->key_rate_scale = (header[10] >> 4) & 0xf;
	voice->lfo_vibrato_reg = header[7];
	voice->lfo_amplitude_reg = header[11] & 0xf;
}

static void write_voice(multipcm *chip, multipcm_voice *voice, int reg, uint8_t data)
{
	voice->regs[reg] = data;
	switch (reg)
	{
		case 0:    /* pan */
			voice->pan = (data >> 4) & 0xf;
			voice->dsp_send = data & 0xf;
			break;
		case 1:    /* the sample: its header loads the envelope and the LFOs */
			init_sample(chip, voice, voice->regs[1] | ((voice->regs[2] & 1) << 8));
			voice->regs[7] = (uint8_t)(voice->attack_reg << 4 | voice->decay1_reg);
			voice->regs[8] = (uint8_t)(voice->decay_level_reg << 4 | voice->decay2_reg);
			voice->regs[9] = (uint8_t)(voice->key_rate_scale << 4 | voice->release_reg);
			write_voice(chip, voice, 6, voice->lfo_vibrato_reg);
			write_voice(chip, voice, 10, voice->lfo_amplitude_reg);
			if (voice->playing)
				retrigger(voice);
			break;
		case 2:    /* pitch */
		case 3:
			voice->octave = voice->regs[3] >> 4;
			voice->pitch = (uint16_t)((voice->regs[3] & 0xf) << 6 | voice->regs[2] >> 2);
			update_step(chip, voice);
			break;
		case 4:    /* key on and off */
			if (data & 0x80)
			{
				voice->playing = true;
				retrigger(voice);
			}
			else if (voice->playing)
			{
				if (voice->release_reg != 0xf)
					voice->state = RELEASE;
				else
					voice->playing = false;
			}
			break;
		case 5:    /* level, glided to or set */
			voice->dest_total_level = (data >> 1) & 0x7f;
			if (!(data & 1))
				voice->total_level_step = tables.total_level_steps[(voice->total_level >> TL_SHIFT) > voice->dest_total_level ? 0 : 1];
			else
				voice->total_level = voice->dest_total_level << TL_SHIFT;
			break;
		case 6:    /* LFO frequency and pitch depth */
		case 10:   /* amplitude depth */
			voice->lfo_frequency = (voice->regs[6] >> 3) & 7;
			voice->vibrato = voice->regs[6] & 7;
			voice->tremolo = voice->regs[10] & 7;
			if (data)
			{
				lfo_compute_step(chip, &voice->pitch_lfo, voice->lfo_frequency, voice->vibrato, false);
				lfo_compute_step(chip, &voice->amplitude_lfo, voice->lfo_frequency, voice->tremolo, true);
			}
			break;
		case 7:    /* the envelope */
		case 8:
		case 9:
			voice->attack_reg = voice->regs[7] >> 4;
			voice->decay1_reg = voice->regs[7] & 0xf;
			voice->decay_level_reg = voice->regs[8] >> 4;
			voice->decay2_reg = voice->regs[8] & 0xf;
			voice->key_rate_scale = voice->regs[9] >> 4;
			voice->release_reg = voice->regs[9] & 0xf;
			envelope_calc(voice);
			break;
	}
}

/* ---- the chip ---- */

void multipcm_init(multipcm *chip, uint32_t clock, uint8_t (*read)(uint32_t address))
{
	memset(chip, 0, sizeof(*chip));
	chip->read = read;
	chip->rate = (float)clock / CLOCK_DIVIDER;
	make_tables(chip->rate);
}

void multipcm_reset(multipcm *chip)
{
	for (int voice = 0; voice < MULTIPCM_VOICES; voice++)
		chip->voices[voice].playing = false;
}

void multipcm_write(multipcm *chip, int offset, uint8_t data)
{
	switch (offset)
	{
		case 0:
			if (chip->address < 11 && chip->current_voice >= 0)
				write_voice(chip, &chip->voices[chip->current_voice], chip->address, data);
			break;
		case 1:
			chip->current_voice = value_to_voice[data & 0x1f];
			break;
		case 2:
			chip->address = data;
			break;
		default:
			/* 0xD: the effect DSP's control data, which the Multi 32 has no DSP for */
			break;
	}
}

uint8_t multipcm_read(multipcm *chip)
{
	(void)chip;
	return 0;
}

static int32_t clamp(int32_t value)
{
	return value < -32768 ? -32768 : (value > 32767 ? 32767 : value);
}

void multipcm_generate(multipcm *chip, int32_t *left, int32_t *right, int count)
{
	for (int i = 0; i < count; ++i)
	{
		int32_t smpl = 0;
		int32_t smpr = 0;

		for (int index = 0; index < MULTIPCM_VOICES; ++index)
		{
			multipcm_voice *voice = &chip->voices[index];
			uint32_t vol;
			uint32_t spos;
			uint32_t step;
			int32_t csample;
			int32_t fpart;
			int32_t sample;

			if (!voice->playing)
				continue;
			vol = (voice->total_level >> TL_SHIFT) | (voice->pan << 7);
			spos = voice->offset >> TL_SHIFT;
			step = voice->step;
			fpart = (int32_t)(voice->offset & ((1 << TL_SHIFT) - 1));
			if (voice->reverse)
				spos = voice->end - spos - 1;
			if (voice->format & 4)    /* 12 bits: three bytes hold two samples */
			{
				const uint32_t address = voice->start + (spos >> 1) * 3;

				if (!(spos & 1))
					csample = (int16_t)(chip->read(address & 0x3fffff) << 8 | (chip->read((address + 1) & 0x3fffff) & 0xf) << 4);
				else
					csample = (int16_t)(chip->read((address + 2) & 0x3fffff) << 8 | (chip->read((address + 1) & 0x3fffff) & 0xf0));
			}
			else
				csample = (int16_t)(chip->read((voice->start + spos) & 0x3fffff) << 8);

			sample = (csample * fpart + voice->prev_sample * ((1 << TL_SHIFT) - fpart)) >> TL_SHIFT;
			if (voice->vibrato)
			{
				step = step * (uint32_t)lfo_step(&voice->pitch_lfo);
				step >>= TL_SHIFT;
			}
			voice->offset += step;
			if (spos ^ (voice->offset >> TL_SHIFT))
				voice->prev_sample = csample;
			if (voice->offset >= (voice->end << TL_SHIFT))
			{
				voice->offset -= (voice->end - voice->loop) << TL_SHIFT;
				voice->reverse = false;
			}
			if ((voice->total_level >> TL_SHIFT) != voice->dest_total_level)
				voice->total_level += (uint32_t)voice->total_level_step;
			if (voice->tremolo)
			{
				sample = sample * lfo_step(&voice->amplitude_lfo);
				sample >>= TL_SHIFT;
			}
			sample = (sample * envelope_update(voice)) >> 10;
			smpl += (tables.left_pan[vol] * sample) >> TL_SHIFT;
			smpr += (tables.right_pan[vol] * sample) >> TL_SHIFT;
		}
		left[i] = clamp(smpl);
		right[i] = clamp(smpr);
	}
}
