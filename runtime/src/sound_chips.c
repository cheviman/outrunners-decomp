/**
 *    Name:        sound_chips.c
 *    Description: the sound board's chips on the board's time: the YM3438 and the MultiPCM, their samples
 *                 frame by frame
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Plugs runtime/src/ym3438.c and multipcm.c into the Z80's side of the board (s32_board.chips). The
 *    chips keep no time: before every write they make the samples that start before it, at the time the
 *    Z80's block makes its accesses (s32_board.sound_cycles, sound_cpu.c), and the timers they ask for run
 *    out where sound_cpu.c says (sound_chips_timers_due). Sample n of the YM3438 starts at n x 360 board
 *    cycles (144 of its clocks, at 8 MHz), the MultiPCM's at n x 448 (224 of its clocks, at 10 MHz).
 *    tools/core/system32/reference/sound/s32sound.cpp does the same with ymfm's YM3438 and MAME's MultiPCM: the frame's
 *    hashes compare. OutRunners uses the YM3438 for its timers only: it plays no note.
 */
#include "sound/chips.h"

#include "s32/multipcm.h"
#include "s32/ym3438.h"

#include <stdlib.h>
#include <string.h>

#define FM_PERIOD           (144ull * S32_CPU_CLOCK / S32_SOUND_CLOCK)     /* board cycles a YM3438 sample takes */
#define PCM_PERIOD          (224ull * S32_CPU_CLOCK / S32_MULTIPCM_CLOCK)  /* ... and a MultiPCM one */
#define NO_TIMER            UINT64_MAX
#define STACK_START         0xFD00u                                     /* the Z80's stack, FD00-FDFF: not in the hash */
#define STACK_END           0xFE00u

/* The samples a chip has made and the frame has not taken yet: its two outputs, from sample 'first' on */
typedef struct sample_buffer
{
	int32_t *data;
	size_t count;           /* values (two a sample) */
	size_t capacity;
	uint64_t first;
} sample_buffer;

static struct
{
	s32_fm fm;
	uint64_t fm_samples;                /* made so far */
	uint64_t deadline[2];               /* when its timers run out, board cycles */
	uint64_t busy_until;
	sample_buffer fm_buffer;
	multipcm pcm;
	uint64_t pcm_samples;
	sample_buffer pcm_buffer;
	uint64_t override;                  /* while a timer runs out: its own time */
	uint64_t frame_hash;
	int32_t *frame[2];                  /* the frame's samples, for the mix */
	size_t frame_counts[2];
	size_t frame_capacity[2];
	int16_t *mix;
	size_t mix_capacity;
	size_t mix_frames;
} sound = { .override = NO_TIMER };

static uint64_t now(void)
{
	return sound.override != NO_TIMER ? sound.override : s32_board.sound_cycles;
}

/* board cycles 'clocks' of the YM3438 (which runs on the Z80's clock) take, rounded up */
static uint64_t fm_span(uint64_t clocks)
{
	return (clocks * S32_CPU_CLOCK + S32_SOUND_CLOCK - 1) / S32_SOUND_CLOCK;
}

static void append(sample_buffer *buffer, int32_t first, int32_t second)
{
	if (buffer->count + 2 > buffer->capacity)
	{
		buffer->capacity = buffer->capacity ? buffer->capacity * 2 : 4096;
		buffer->data = (int32_t *)realloc(buffer->data, buffer->capacity * sizeof(int32_t));
	}
	buffer->data[buffer->count++] = first;
	buffer->data[buffer->count++] = second;
}

/* ---- the YM3438 ---- */

static void fm_catch_up(uint64_t time)
{
	const uint64_t due = (time + FM_PERIOD - 1) / FM_PERIOD;
	int32_t out[2];

	while (sound.fm_samples < due)
	{
		s32_fm_generate(&sound.fm, out);
		append(&sound.fm_buffer, out[0], out[1]);
		sound.fm_samples++;
	}
}

static void fm_set_timer(void *opaque, int timer, int32_t clocks)
{
	(void)opaque;
	sound.deadline[timer] = clocks < 0 ? NO_TIMER : now() + fm_span((uint64_t)clocks);
}

static void fm_update_irq(void *opaque, bool asserted)
{
	(void)opaque;
	if (asserted)
		s32_sound_signal_irq(S32_SOUND_IRQ_FM);
	else
		s32_sound_clear_irq(S32_SOUND_IRQ_FM);
}

static void fm_set_busy_end(void *opaque, uint32_t clocks)
{
	(void)opaque;
	sound.busy_until = now() + fm_span(clocks);
}

static bool fm_is_busy(void *opaque)
{
	(void)opaque;
	return now() < sound.busy_until;
}

static uint64_t fm_next_timer(void)
{
	return sound.deadline[0] < sound.deadline[1] ? sound.deadline[0] : sound.deadline[1];
}

void sound_chips_timers_due(uint64_t time)
{
	/* the timers due by 'time', in the order they run out */
	while (fm_next_timer() <= time)
	{
		const int timer = sound.deadline[0] <= sound.deadline[1] ? 0 : 1;
		const uint64_t when = sound.deadline[timer];

		sound.deadline[timer] = NO_TIMER;
		fm_catch_up(when);
		sound.override = when;
		s32_fm_timer_expired(&sound.fm, timer);
		sound.override = NO_TIMER;
	}
}

static uint8_t fm_read(int offset)
{
	return s32_fm_read(&sound.fm, offset);
}

static void fm_write(int offset, uint8_t data)
{
	fm_catch_up(now());
	s32_fm_write(&sound.fm, offset, data);
}

/* ---- the MultiPCM ---- */

static void pcm_catch_up(uint64_t time)
{
	const uint64_t due = (time + PCM_PERIOD - 1) / PCM_PERIOD;
	int32_t first[256];
	int32_t second[256];

	while (sound.pcm_samples < due)
	{
		const int count = due - sound.pcm_samples > 256 ? 256 : (int)(due - sound.pcm_samples);

		multipcm_generate(&sound.pcm, first, second, count);
		for (int index = 0; index < count; index++)
			append(&sound.pcm_buffer, first[index], second[index]);
		sound.pcm_samples += (uint64_t)count;
	}
}

static uint8_t pcm_read(int offset)
{
	(void)offset;
	return multipcm_read(&sound.pcm);
}

static void pcm_write(int offset, uint8_t data)
{
	pcm_catch_up(now());
	multipcm_write(&sound.pcm, offset, data);
}

void sound_chips_pcm_banks_changing(void)
{
	pcm_catch_up(now());
}

/* ---- frames ---- */

static uint64_t hash_bytes(const void *data, size_t size, uint64_t hash)
{
	const uint8_t *bytes = (const uint8_t *)data;

	for (size_t index = 0; index < size; index++)
		hash = (hash ^ bytes[index]) * 0x100000001b3ull;
	return hash;
}

/* the samples before sample number 'due': hashed and moved to the frame's list 'which' */
static uint64_t take(sample_buffer *buffer, uint64_t due, uint64_t hash, int which)
{
	size_t values = (size_t)(due - buffer->first) * 2;

	if (values > buffer->count)
		values = buffer->count;
	if (values > sound.frame_capacity[which])
	{
		sound.frame_capacity[which] = values;
		sound.frame[which] = (int32_t *)realloc(sound.frame[which], values * sizeof(int32_t));
	}
	if (values)
		memcpy(sound.frame[which], buffer->data, values * sizeof(int32_t));
	sound.frame_counts[which] = values;
	memmove(buffer->data, buffer->data + values, (buffer->count - values) * sizeof(int32_t));
	buffer->count -= values;
	buffer->first += values / 2;
	return hash_bytes(sound.frame[which], values * sizeof(int32_t), hash);
}

static int16_t clamp16(double value)
{
	return (int16_t)(value < -32768.0 ? -32768.0 : (value > 32767.0 ? 32767.0 : value));
}

/* The mix at the YM3438's rate: the MultiPCM's latest sample at each of its samples' start; each chip's
   second output goes to the left speaker, its first to the right one (MAME's routes) */
static void mix(void)
{
	const size_t frames = sound.frame_counts[0] / 2;
	const size_t pcm_frames = sound.frame_counts[1] / 2;

	if (frames * 2 > sound.mix_capacity)
	{
		sound.mix_capacity = frames * 2;
		sound.mix = (int16_t *)realloc(sound.mix, sound.mix_capacity * sizeof(int16_t));
	}
	for (size_t index = 0; index < frames; index++)
	{
		size_t at = 0;

		if (pcm_frames)
		{
			at = index * pcm_frames / (frames ? frames : 1);
			if (at > pcm_frames - 1)
				at = pcm_frames - 1;
		}
		for (int side = 0; side < 2; side++)
		{
			const int output = 1 - side;
			const double fm_part = 0.15 * sound.frame[0][index * 2 + (size_t)output];
			const double pcm_part = pcm_frames ? 0.35 * sound.frame[1][at * 2 + (size_t)output] : 0.0;

			sound.mix[index * 2 + (size_t)side] = clamp16(fm_part + pcm_part);
		}
	}
	sound.mix_frames = frames;
}

void sound_chips_end_frame(uint64_t time)
{
	uint64_t hash = 0xcbf29ce484222325ull;

	fm_catch_up(time);
	pcm_catch_up(time);
	hash = take(&sound.fm_buffer, (time + FM_PERIOD - 1) / FM_PERIOD, hash, 0);
	sound.frame_hash = take(&sound.pcm_buffer, (time + PCM_PERIOD - 1) / PCM_PERIOD, hash, 1);
	mix();
}

uint64_t sound_chips_frame_hash(void)
{
	return sound.frame_hash;
}

uint64_t sound_board_state_hash(void)
{
	uint64_t hash = 0xcbf29ce484222325ull;

	hash = hash_bytes(s32_board.shared_ram, STACK_START - 0xE000u, hash);
	return hash_bytes(s32_board.shared_ram + (STACK_END - 0xE000u), 0x10000u - STACK_END, hash);
}

const int32_t *sound_chips_frame_samples(int chip, size_t *count)
{
	if (count)
		*count = sound.frame_counts[chip & 1] / 2;
	return sound.frame[chip & 1];
}

const int16_t *sound_chips_frame_mix(size_t *frames)
{
	if (frames)
		*frames = sound.mix_frames;
	return sound.mix;
}

uint32_t sound_chips_mix_rate(void)
{
	return S32_SOUND_CLOCK / 144;
}

void sound_chips_attach(void)
{
	memset(&sound.fm, 0, sizeof(sound.fm));
	sound.deadline[0] = sound.deadline[1] = NO_TIMER;
	sound.fm.host = NULL;
	sound.fm.set_timer = fm_set_timer;
	sound.fm.update_irq = fm_update_irq;
	sound.fm.set_busy_end = fm_set_busy_end;
	sound.fm.is_busy = fm_is_busy;
	s32_fm_init(&sound.fm);
	multipcm_init(&sound.pcm, S32_MULTIPCM_CLOCK, s32_pcm_sample_read);
	multipcm_reset(&sound.pcm);
	sound.fm_samples = sound.pcm_samples = 0;
	sound.fm_buffer.first = sound.pcm_buffer.first = 0;
	s32_board.chips.fm_read = fm_read;
	s32_board.chips.fm_write = fm_write;
	s32_board.chips.pcm_read = pcm_read;
	s32_board.chips.pcm_write = pcm_write;
}
