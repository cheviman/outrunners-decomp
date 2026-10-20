/**
 *    Name:        ym3438.c
 *    Description: Yamaha YM3438 (OPN2C) FM sound chip: six four-operator channels, the DAC, two timers
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows ymfm (Aaron Giles) sample for sample: its ym3438, which is the ym2612's interface
 *    and generate() of its own over fm_engine_base<opna_registers>. The tables of the chip's ROMs are
 *    ymfm's, which took them from the die. tools/reference/sound/chipcheck compares the two. ymfm's
 *    terms are in tools/reference/ymfm/LICENSE.
 *
 *    How a sample is made: first the channels whose registers changed are refreshed (each operator works
 *    out its phase step, rates and levels from the registers, and takes its key), then every operator
 *    moves its envelope (on one sample in three) and its phase. Then each channel that can be heard runs
 *    its operators in the order its algorithm gives, each one's output shifting the phase of the next,
 *    and sums its carriers at 9 bits with clipping. The six channels (or five and the DAC) are added and
 *    scaled to 16 bits as the chip multiplexes them.
 *
 *    Levels are logarithmic until the last step: an envelope or a sine is an attenuation, attenuations
 *    add, and one ROM turns the sum into a linear volume.
 */
#include "s32/ym3438.h"

#include <string.h>

#define PRESCALE            6          /* input clocks per operator slot */
#define OPERATOR_SLOTS      24         /* a sample is a pass over the 24 operators */
#define ALL_CHANNELS        0x3f
#define STATUS_TIMER_A      0x01
#define STATUS_TIMER_B      0x02
#define STATUS_BUSY         0x80
#define IRQ_SOURCES         (STATUS_TIMER_A | STATUS_TIMER_B)
#define QUIET               0x380      /* an envelope past this is heard as nothing */
#define STEP_FOLLOWS_LFO    1          /* an operator's step: work it out each sample, with the PM */
#define KEY_REGISTER        0          /* bits of an operator's keys */
#define KEY_CSM             2
#define OUTPUT_SHIFT        5          /* a channel's sum is 9 bits */
#define OUTPUT_CLIP         256
#define BUSY_CLOCKS         (32 * PRESCALE)

/* The registers the code names */
#define REG_LFO             0x22
#define REG_TIMER_A_HIGH    0x24
#define REG_TIMER_A_LOW     0x25
#define REG_TIMER_B         0x26
#define REG_MODE            0x27
#define REG_KEY             0x28
#define REG_DAC             0x2a
#define REG_DAC_ENABLE      0x2b
#define REG_DAC_LOW         0x2c

/* ---- arithmetic ---- */

static uint32_t field(uint32_t value, int start, int length)
{
	return (value >> start) & ((1u << length) - 1);
}

/* x >> count, rounding down for negative numbers too */
static int32_t shift_down(int32_t value, int count)
{
	return value >= 0 ? value >> count : ~(~value >> count);
}

static int32_t clamp(int32_t value, int32_t low, int32_t high)
{
	if (value < low)
		return low;
	if (value > high)
		return high;
	return value;
}

static int32_t sign_extend(uint32_t value, int bits)
{
	const uint32_t sign = 1u << (bits - 1);
	return (int32_t)((value & ((sign << 1) - 1)) ^ sign) - (int32_t)sign;
}

/* ---- the chip's ROMs ---- */

/* The attenuation of |sin| over the first quarter of the wave (256 steps), 4.8 fixed point */
static const uint16_t quarter_sine[256] =
{
	0x859, 0x6c3, 0x607, 0x58b, 0x52e, 0x4e4, 0x4a6, 0x471, 0x443, 0x41a, 0x3f5, 0x3d3, 0x3b5, 0x398, 0x37e, 0x365,
	0x34e, 0x339, 0x324, 0x311, 0x2ff, 0x2ed, 0x2dc, 0x2cd, 0x2bd, 0x2af, 0x2a0, 0x293, 0x286, 0x279, 0x26d, 0x261,
	0x256, 0x24b, 0x240, 0x236, 0x22c, 0x222, 0x218, 0x20f, 0x206, 0x1fd, 0x1f5, 0x1ec, 0x1e4, 0x1dc, 0x1d4, 0x1cd,
	0x1c5, 0x1be, 0x1b7, 0x1b0, 0x1a9, 0x1a2, 0x19b, 0x195, 0x18f, 0x188, 0x182, 0x17c, 0x177, 0x171, 0x16b, 0x166,
	0x160, 0x15b, 0x155, 0x150, 0x14b, 0x146, 0x141, 0x13c, 0x137, 0x133, 0x12e, 0x129, 0x125, 0x121, 0x11c, 0x118,
	0x114, 0x10f, 0x10b, 0x107, 0x103, 0x0ff, 0x0fb, 0x0f8, 0x0f4, 0x0f0, 0x0ec, 0x0e9, 0x0e5, 0x0e2, 0x0de, 0x0db,
	0x0d7, 0x0d4, 0x0d1, 0x0cd, 0x0ca, 0x0c7, 0x0c4, 0x0c1, 0x0be, 0x0bb, 0x0b8, 0x0b5, 0x0b2, 0x0af, 0x0ac, 0x0a9,
	0x0a7, 0x0a4, 0x0a1, 0x09f, 0x09c, 0x099, 0x097, 0x094, 0x092, 0x08f, 0x08d, 0x08a, 0x088, 0x086, 0x083, 0x081,
	0x07f, 0x07d, 0x07a, 0x078, 0x076, 0x074, 0x072, 0x070, 0x06e, 0x06c, 0x06a, 0x068, 0x066, 0x064, 0x062, 0x060,
	0x05e, 0x05c, 0x05b, 0x059, 0x057, 0x055, 0x053, 0x052, 0x050, 0x04e, 0x04d, 0x04b, 0x04a, 0x048, 0x046, 0x045,
	0x043, 0x042, 0x040, 0x03f, 0x03e, 0x03c, 0x03b, 0x039, 0x038, 0x037, 0x035, 0x034, 0x033, 0x031, 0x030, 0x02f,
	0x02e, 0x02d, 0x02b, 0x02a, 0x029, 0x028, 0x027, 0x026, 0x025, 0x024, 0x023, 0x022, 0x021, 0x020, 0x01f, 0x01e,
	0x01d, 0x01c, 0x01b, 0x01a, 0x019, 0x018, 0x017, 0x017, 0x016, 0x015, 0x014, 0x014, 0x013, 0x012, 0x011, 0x011,
	0x010, 0x00f, 0x00f, 0x00e, 0x00d, 0x00d, 0x00c, 0x00c, 0x00b, 0x00a, 0x00a, 0x009, 0x009, 0x008, 0x008, 0x007,
	0x007, 0x007, 0x006, 0x006, 0x005, 0x005, 0x005, 0x004, 0x004, 0x004, 0x003, 0x003, 0x003, 0x002, 0x002, 0x002,
	0x002, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000
};

/* The linear volume of the fraction (low 8 bits) of an attenuation: a 10-bit mantissa under an implied
   0x400, from attenuation 0 (loudest) up */
static const uint16_t volume_mantissa[256] =
{
	0x3fa, 0x3f5, 0x3ef, 0x3ea, 0x3e4, 0x3df, 0x3da, 0x3d4, 0x3cf, 0x3c9, 0x3c4, 0x3bf, 0x3b9, 0x3b4, 0x3ae, 0x3a9,
	0x3a4, 0x39f, 0x399, 0x394, 0x38f, 0x38a, 0x384, 0x37f, 0x37a, 0x375, 0x370, 0x36a, 0x365, 0x360, 0x35b, 0x356,
	0x351, 0x34c, 0x347, 0x342, 0x33d, 0x338, 0x333, 0x32e, 0x329, 0x324, 0x31f, 0x31a, 0x315, 0x310, 0x30b, 0x306,
	0x302, 0x2fd, 0x2f8, 0x2f3, 0x2ee, 0x2e9, 0x2e5, 0x2e0, 0x2db, 0x2d6, 0x2d2, 0x2cd, 0x2c8, 0x2c4, 0x2bf, 0x2ba,
	0x2b5, 0x2b1, 0x2ac, 0x2a8, 0x2a3, 0x29e, 0x29a, 0x295, 0x291, 0x28c, 0x288, 0x283, 0x27f, 0x27a, 0x276, 0x271,
	0x26d, 0x268, 0x264, 0x25f, 0x25b, 0x257, 0x252, 0x24e, 0x249, 0x245, 0x241, 0x23c, 0x238, 0x234, 0x230, 0x22b,
	0x227, 0x223, 0x21e, 0x21a, 0x216, 0x212, 0x20e, 0x209, 0x205, 0x201, 0x1fd, 0x1f9, 0x1f5, 0x1f0, 0x1ec, 0x1e8,
	0x1e4, 0x1e0, 0x1dc, 0x1d8, 0x1d4, 0x1d0, 0x1cc, 0x1c8, 0x1c4, 0x1c0, 0x1bc, 0x1b8, 0x1b4, 0x1b0, 0x1ac, 0x1a8,
	0x1a4, 0x1a0, 0x19c, 0x199, 0x195, 0x191, 0x18d, 0x189, 0x185, 0x181, 0x17e, 0x17a, 0x176, 0x172, 0x16f, 0x16b,
	0x167, 0x163, 0x160, 0x15c, 0x158, 0x154, 0x151, 0x14d, 0x149, 0x146, 0x142, 0x13e, 0x13b, 0x137, 0x134, 0x130,
	0x12c, 0x129, 0x125, 0x122, 0x11e, 0x11b, 0x117, 0x114, 0x110, 0x10c, 0x109, 0x106, 0x102, 0x0ff, 0x0fb, 0x0f8,
	0x0f4, 0x0f1, 0x0ed, 0x0ea, 0x0e7, 0x0e3, 0x0e0, 0x0dc, 0x0d9, 0x0d6, 0x0d2, 0x0cf, 0x0cc, 0x0c8, 0x0c5, 0x0c2,
	0x0be, 0x0bb, 0x0b8, 0x0b5, 0x0b1, 0x0ae, 0x0ab, 0x0a8, 0x0a4, 0x0a1, 0x09e, 0x09b, 0x098, 0x094, 0x091, 0x08e,
	0x08b, 0x088, 0x085, 0x082, 0x07e, 0x07b, 0x078, 0x075, 0x072, 0x06f, 0x06c, 0x069, 0x066, 0x063, 0x060, 0x05d,
	0x05a, 0x057, 0x054, 0x051, 0x04e, 0x04b, 0x048, 0x045, 0x042, 0x03f, 0x03c, 0x039, 0x036, 0x033, 0x030, 0x02d,
	0x02a, 0x028, 0x025, 0x022, 0x01f, 0x01c, 0x019, 0x016, 0x014, 0x011, 0x00e, 0x00b, 0x008, 0x006, 0x003, 0x000
};

/* How much an envelope moves on each of eight successive steps at a rate (0-63), a nibble each with the
   first step lowest; on attack it is how big a part of the way to 0 it goes, in sixteenths */
static const uint32_t envelope_steps[64] =
{
	0x00000000, 0x00000000, 0x10101010, 0x10101010,
	0x10101010, 0x10101010, 0x11101110, 0x11101110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x10101010, 0x10111010, 0x11101110, 0x11111110,
	0x11111111, 0x21112111, 0x21212121, 0x22212221,
	0x22222222, 0x42224222, 0x42424242, 0x44424442,
	0x44444444, 0x84448444, 0x84848484, 0x88848884,
	0x88888888, 0x88888888, 0x88888888, 0x88888888
};

/* The phase step a detune of 1-3 adds, by key code (0-31) */
static const uint8_t detune_steps[32][4] =
{
	{ 0, 0, 1, 2 }, { 0, 0, 1, 2 }, { 0, 0, 1, 2 }, { 0, 0, 1, 2 },
	{ 0, 1, 2, 2 }, { 0, 1, 2, 3 }, { 0, 1, 2, 3 }, { 0, 1, 2, 3 },
	{ 0, 1, 2, 4 }, { 0, 1, 3, 4 }, { 0, 1, 3, 4 }, { 0, 1, 3, 5 },
	{ 0, 2, 4, 5 }, { 0, 2, 4, 6 }, { 0, 2, 4, 6 }, { 0, 2, 5, 7 },
	{ 0, 2, 5, 8 }, { 0, 3, 6, 8 }, { 0, 3, 6, 9 }, { 0, 3, 7, 10 },
	{ 0, 4, 8, 11 }, { 0, 4, 8, 12 }, { 0, 4, 9, 13 }, { 0, 5, 10, 14 },
	{ 0, 5, 11, 16 }, { 0, 6, 12, 17 }, { 0, 6, 13, 19 }, { 0, 7, 14, 20 },
	{ 0, 8, 16, 22 }, { 0, 8, 16, 22 }, { 0, 8, 16, 22 }, { 0, 8, 16, 22 }
};

/* The LFO's PM moves the frequency number by the sum of two right shifts of its top 7 bits (7 shifts
   them out), by PM depth (0-7) and the LFO's PM step (0-7): the shifts in the low and high nibble */
static const uint8_t pm_shifts[8][8] =
{
	{ 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77 },
	{ 0x77, 0x77, 0x77, 0x77, 0x72, 0x72, 0x72, 0x72 },
	{ 0x77, 0x77, 0x77, 0x72, 0x72, 0x72, 0x17, 0x17 },
	{ 0x77, 0x77, 0x72, 0x72, 0x17, 0x17, 0x12, 0x12 },
	{ 0x77, 0x77, 0x72, 0x17, 0x17, 0x17, 0x12, 0x07 },
	{ 0x77, 0x77, 0x17, 0x12, 0x07, 0x07, 0x02, 0x01 },
	{ 0x77, 0x77, 0x17, 0x12, 0x07, 0x07, 0x02, 0x01 },
	{ 0x77, 0x77, 0x17, 0x12, 0x07, 0x07, 0x02, 0x01 }
};

/* Samples between the LFO's steps, by its rate (register 0x22), less one */
static const uint8_t lfo_periods[8] = { 109, 78, 72, 68, 63, 45, 9, 6 };

/* The algorithms: where operators 2-4 take their modulation from (0 none, 1-3 operator 1-3, 5 operators
   1 + 2, 6 operators 1 + 3, 7 operators 2 + 3), and which of operators 1-3 are carriers besides 4
   (bit 0 operator 1, bit 1 operator 2, bit 2 operator 3) */
typedef struct algorithm
{
	uint8_t input2;
	uint8_t input3;
	uint8_t input4;
	uint8_t carriers;
} algorithm;

static const algorithm algorithms[8] =
{
	{ 1, 2, 3, 0 },     /* 1 > 2 > 3 > 4 */
	{ 0, 5, 3, 0 },     /* (1 + 2) > 3 > 4 */
	{ 0, 2, 6, 0 },     /* (1 + (2 > 3)) > 4 */
	{ 1, 0, 7, 0 },     /* ((1 > 2) + 3) > 4 */
	{ 1, 0, 3, 2 },     /* (1 > 2) + (3 > 4) */
	{ 1, 1, 1, 6 },     /* (1 > 2) + (1 > 3) + (1 > 4) */
	{ 1, 0, 0, 6 },     /* (1 > 2) + 3 + 4 */
	{ 0, 0, 0, 7 }      /* 1 + 2 + 3 + 4 */
};

/* The attenuation of the sine at a point of the wave (10 bits); bit 15 is the sign of the second half */
static uint32_t sine_attenuation(uint32_t point)
{
	const uint32_t quarter = (point & 0x100) ? ~point : point;

	return quarter_sine[quarter & 0xff] | ((point >> 9) & 1) << 15;
}

/* A 5.8 attenuation as a 13-bit linear volume */
static uint32_t attenuation_to_volume(uint32_t attenuation)
{
	return ((volume_mantissa[attenuation & 0xff] | 0x400u) << 2) >> (attenuation >> 8);
}

/* ---- registers ---- */

static uint32_t chip_register(const s32_fm *chip, uint32_t number, int start, int length)
{
	return field(chip->registers[number], start, length);
}

static bool lfo_enabled(const s32_fm *chip)
{
	return chip_register(chip, REG_LFO, 3, 1) != 0;
}

/* Channel 3 with a frequency of its own for operators 1-3 (mode bits 01, and 10 for CSM) */
static bool special_mode(const s32_fm *chip)
{
	return chip_register(chip, REG_MODE, 6, 2) != 0;
}

/* Key on channel 3 when timer A runs out (mode bits 10) */
static bool csm_mode(const s32_fm *chip)
{
	return chip_register(chip, REG_MODE, 6, 2) == 2;
}

/* Block and frequency number: the latched upper byte (0xA4) over 0xA0, by channel */
static uint32_t channel_frequency(const s32_fm *chip, uint32_t channel)
{
	return chip_register(chip, 0xa4 + channel, 0, 6) << 8 | chip->registers[0xa0 + channel];
}

/* Channel 3's special frequencies 0-2 (0xAC-0xAE over 0xA8-0xAA): operators 3, 1 and 2 */
static uint32_t special_frequency(const s32_fm *chip, uint32_t number)
{
	return chip_register(chip, 0xac + number, 0, 6) << 8 | chip->registers[0xa8 + number];
}

static uint32_t pm_depth(const s32_fm *chip, uint32_t channel)
{
	return chip_register(chip, 0xb4 + channel, 0, 3);
}

static bool ssg_enabled(const s32_fm *chip, const s32_fm_operator *op)
{
	return chip_register(chip, 0x90 + op->offset, 3, 1) != 0;
}

static uint32_t ssg_mode(const s32_fm *chip, const s32_fm_operator *op)
{
	return chip_register(chip, 0x90 + op->offset, 0, 3);
}

/* ---- status and timers ---- */

static void check_irq(s32_fm *chip)
{
	const uint8_t before = chip->irq_state;

	chip->irq_state = (chip->status & IRQ_SOURCES) != 0;
	if (chip->irq_state != before && chip->update_irq)
		chip->update_irq(chip->host, chip->irq_state != 0);
}

static void change_status(s32_fm *chip, uint8_t set, uint8_t clear)
{
	chip->status = (uint8_t)((chip->status | set) & ~(clear | STATUS_BUSY));
	check_irq(chip);
}

/* Starts a timer that is enabled and not running, with 'delta' added to its period in samples; stops
   one that is not enabled */
static void update_timer(s32_fm *chip, int timer, bool enable, int32_t delta)
{
	if (enable && !chip->timer_running[timer])
	{
		uint32_t period;

		if (timer == 0)
			period = 1024 - (chip->registers[REG_TIMER_A_HIGH] << 2 | chip_register(chip, REG_TIMER_A_LOW, 0, 2));
		else
			period = 16 * (256 - (uint32_t)chip->registers[REG_TIMER_B]);
		period += (uint32_t)delta;
		if (chip->set_timer)
			chip->set_timer(chip->host, timer, (int32_t)(period * OPERATOR_SLOTS * PRESCALE));
		chip->timer_running[timer] = 1;
	}
	else if (!enable)
	{
		if (chip->set_timer)
			chip->set_timer(chip->host, timer, -1);
		chip->timer_running[timer] = 0;
	}
}

/* ---- operators ---- */

/* Hand a key on or off from one source to the operators of a channel: bit n of 'operators' is
   operator n + 1. The envelopes take it at the next refresh. */
static void key_channel(s32_fm_channel *channel, uint32_t operators, int source)
{
	for (int index = 0; index < 4; index++)
	{
		s32_fm_operator *op = &channel->operators[index];

		op->keys = (uint8_t)((op->keys & ~(1u << source)) | field(operators, index, 1) << source);
	}
}

/* How far the LFO's PM moves a frequency number with these top 7 bits */
static int32_t pm_offset(uint32_t top_bits, uint32_t depth, int32_t lfo_pm)
{
	const uint32_t magnitude = (uint32_t)(lfo_pm < 0 ? -lfo_pm : lfo_pm);
	const uint32_t shifts = pm_shifts[depth][magnitude & 7];
	int32_t offset = (int32_t)((top_bits >> (shifts & 15)) + (top_bits >> (shifts >> 4)));

	if (depth > 5)
		offset <<= depth - 5;
	offset >>= 2;
	return lfo_pm < 0 ? -offset : offset;
}

/* The phase step of an operator: its frequency number moved by the LFO's PM, shifted by its block,
   detuned, kept to 17 bits and multiplied */
static uint32_t phase_step(const s32_fm *chip, const s32_fm_channel *channel, const s32_fm_operator *op, int32_t lfo_pm)
{
	uint32_t number = field(op->block_frequency, 0, 11) << 1;
	const uint32_t depth = pm_depth(chip, channel->offset);

	if (depth != 0)
	{
		number += (uint32_t)pm_offset(field(op->block_frequency, 4, 7), depth, lfo_pm);
		number &= 0xfff;
	}

	uint32_t step = (number << field(op->block_frequency, 11, 3)) >> 2;

	step += (uint32_t)op->detune;
	step &= 0x1ffff;
	return (step * op->multiple) >> 1;
}

/* A rate (0-63) with key scaling added; 0 stays 0 */
static uint8_t scaled_rate(uint32_t rate, uint32_t scaling)
{
	if (rate == 0)
		return 0;
	return (uint8_t)(rate + scaling < 63 ? rate + scaling : 63);
}

/* What the registers make of an operator */
static void refresh_values(const s32_fm *chip, const s32_fm_channel *channel, s32_fm_operator *op)
{
	const uint32_t at = op->offset;
	uint32_t block_frequency = channel_frequency(chip, channel->offset);

	if (special_mode(chip) && channel->offset == 2)
	{
		if (at == 2)
			block_frequency = special_frequency(chip, 1);
		else if (at == 10)
			block_frequency = special_frequency(chip, 2);
		else if (at == 6)
			block_frequency = special_frequency(chip, 0);
	}
	op->block_frequency = block_frequency;

	/* the key code: block and the top bit of the number, and below them a bit the next three decide
	   (the YM2608 manual's formula, as a table in a 16-bit constant) */
	const uint32_t key_code = field(block_frequency, 10, 4) << 1 | field(0xfe80, (int)field(block_frequency, 7, 4), 1);
	const uint32_t detune = chip_register(chip, 0x30 + at, 4, 3);
	const int32_t detune_step = detune_steps[key_code][detune & 3];

	op->detune = (detune & 4) ? -detune_step : detune_step;
	op->multiple = chip_register(chip, 0x30 + at, 0, 4) * 2;
	if (op->multiple == 0)
		op->multiple = 1;
	if (!lfo_enabled(chip) || pm_depth(chip, channel->offset) == 0)
		op->step = phase_step(chip, channel, op, 0);
	else
		op->step = STEP_FOLLOWS_LFO;
	op->total_level = chip_register(chip, 0x40 + at, 0, 7) << 3;

	/* 4 bits of sustain level, 15 meaning 31 */
	uint32_t sustain = chip_register(chip, 0x80 + at, 4, 4);

	sustain |= (sustain + 1) & 0x10;
	op->sustain_level = sustain << 5;

	const uint32_t scaling = key_code >> (chip_register(chip, 0x50 + at, 6, 2) ^ 3);

	op->rates[S32_FM_ATTACK] = scaled_rate(chip_register(chip, 0x50 + at, 0, 5) * 2, scaling);
	op->rates[S32_FM_DECAY] = scaled_rate(chip_register(chip, 0x60 + at, 0, 5) * 2, scaling);
	op->rates[S32_FM_SUSTAIN] = scaled_rate(chip_register(chip, 0x70 + at, 0, 5) * 2, scaling);
	op->rates[S32_FM_RELEASE] = scaled_rate(chip_register(chip, 0x80 + at, 0, 4) * 4 + 2, scaling);
}

/* Key on: the attack starts from where the envelope is, the phase from 0 (a restart by SSG-EG keeps
   both the phase and the inversion) */
static void start_attack(const s32_fm *chip, s32_fm_operator *op, bool restart)
{
	if (op->stage == S32_FM_ATTACK)
		return;
	op->stage = S32_FM_ATTACK;
	if (!restart)
	{
		op->ssg_inverted = (uint8_t)(ssg_enabled(chip, op) & field(ssg_mode(chip, op), 2, 1));
		op->phase = 0;
	}
	/* the fastest attacks jump straight to the top */
	if (op->rates[S32_FM_ATTACK] >= 62)
		op->attenuation = 0;
}

/* Key off: an inverted SSG-EG envelope is turned the right way up where it stands */
static void start_release(s32_fm_operator *op)
{
	if (op->stage >= S32_FM_RELEASE)
		return;
	op->stage = S32_FM_RELEASE;
	if (op->ssg_inverted)
	{
		op->attenuation = (uint16_t)((0x200 - op->attenuation) & 0x3ff);
		op->ssg_inverted = 0;
	}
}

/* The refresh of an operator: its values, and its key. Whether it may still be heard. */
static bool refresh_operator(const s32_fm *chip, const s32_fm_channel *channel, s32_fm_operator *op)
{
	refresh_values(chip, channel, op);

	const uint8_t key = op->keys != 0;

	if (key != op->key)
	{
		op->key = key;
		if (key)
			start_attack(chip, op, false);
		else
			start_release(op);
	}
	/* CSM keys on for one refresh only */
	op->keys &= (uint8_t)~(1u << KEY_CSM);
	return op->stage != S32_FM_RELEASE || op->attenuation < QUIET;
}

/* SSG-EG, once the envelope is past its middle: modes 1/3/5/7 hold it at the bottom or the top, modes
   0/2/4/6 restart the attack, 2 and 6 turning it over each time, 0 and 4 resetting the phase */
static void clock_ssg(const s32_fm *chip, s32_fm_operator *op)
{
	if (!(op->attenuation & 0x200))
		return;

	const uint32_t mode = ssg_mode(chip, op);

	if (mode & 1)
	{
		op->ssg_inverted = (uint8_t)(field(mode, 2, 1) ^ field(mode, 1, 1));
		if (op->stage != S32_FM_ATTACK)
			op->attenuation = op->ssg_inverted ? 0x200 : 0x3ff;
	}
	else
	{
		op->ssg_inverted ^= (uint8_t)field(mode, 1, 1);
		if (op->stage == S32_FM_DECAY || op->stage == S32_FM_SUSTAIN)
			start_attack(chip, op, true);
		if (field(mode, 1, 1) == 0)
			op->phase = 0;
	}
	if (op->stage == S32_FM_RELEASE)
		op->attenuation = 0x3ff;
}

/* A step of the envelope: 'counter' is the envelope counter in whole steps */
static void clock_envelope(const s32_fm *chip, s32_fm_operator *op, uint32_t counter)
{
	if (op->stage == S32_FM_ATTACK && op->attenuation == 0)
		op->stage = S32_FM_DECAY;
	/* right after the attack too, so that a sustain level of 0 skips the decay */
	if (op->stage == S32_FM_DECAY && op->attenuation >= op->sustain_level)
		op->stage = S32_FM_SUSTAIN;

	/* the rate decides on which steps the envelope moves (the counter as a 5.11 number has no
	   fraction) and by how much (the bits above the fraction) */
	const uint32_t rate = op->rates[op->stage];
	const uint32_t rate_shift = rate >> 2;

	counter <<= rate_shift;
	if (field(counter, 0, 11) != 0)
		return;

	const uint32_t position = field(counter, rate_shift <= 11 ? 11 : (int)rate_shift, 3);
	const uint32_t increment = field(envelope_steps[rate], 4 * (int)position, 4);

	if (op->stage == S32_FM_ATTACK)
	{
		/* the attack goes a part of the way to 0; rates 62 and 63 do not move at all, unless they
		   were set at the key on */
		if (rate < 62)
			op->attenuation = (uint16_t)(op->attenuation + (((~(uint32_t)op->attenuation) * increment) >> 4));
	}
	else
	{
		/* SSG-EG moves four times as fast, and only in the upper half */
		if (!ssg_enabled(chip, op))
			op->attenuation = (uint16_t)(op->attenuation + increment);
		else if (op->attenuation < 0x200)
			op->attenuation = (uint16_t)(op->attenuation + 4 * increment);
		if (op->attenuation >= 0x400)
			op->attenuation = 0x3ff;
	}
}

static void clock_operator(const s32_fm *chip, const s32_fm_channel *channel, s32_fm_operator *op, uint32_t envelope_counter, int32_t lfo_pm)
{
	if (ssg_enabled(chip, op))
		clock_ssg(chip, op);
	else
		op->ssg_inverted = 0;
	if (field(envelope_counter, 0, 2) == 0)
		clock_envelope(chip, op, envelope_counter >> 2);

	uint32_t step = op->step;

	if (step == STEP_FOLLOWS_LFO)
		step = phase_step(chip, channel, op, lfo_pm);
	op->phase += step;
}

/* The envelope as heard: turned over by SSG-EG, with the LFO's AM and the total level, at most 0x3FF */
static uint32_t envelope_attenuation(const s32_fm *chip, const s32_fm_operator *op, uint32_t am_offset)
{
	uint32_t attenuation = op->attenuation;

	if (op->ssg_inverted)
		attenuation = (0x200 - attenuation) & 0x3ff;
	if (chip_register(chip, 0x60 + op->offset, 7, 1))
		attenuation += am_offset;
	attenuation += op->total_level;
	return attenuation < 0x3ff ? attenuation : 0x3ff;
}

/* An operator's output at a point of its wave: 14 bits, signed */
static int32_t operator_output(const s32_fm *chip, const s32_fm_operator *op, uint32_t point, uint32_t am_offset)
{
	if (op->attenuation > QUIET)
		return 0;

	const uint32_t sine = sine_attenuation(point & 0x3ff);
	const uint32_t envelope = envelope_attenuation(chip, op, am_offset) << 2;
	const int32_t volume = (int32_t)attenuation_to_volume((sine & 0x7fff) + envelope);

	return (sine & 0x8000) ? -volume : volume;
}

/* ---- channels ---- */

/* The LFO's AM for a channel, by its AM depth: none, or the LFO's 0-0x3F as 0-0x0F, 0-0x3F or 0-0x7E */
static uint32_t am_offset(const s32_fm *chip, uint32_t channel)
{
	const uint32_t shift = (1u << (chip_register(chip, 0xb4 + channel, 4, 2) ^ 3)) - 1;

	return ((uint32_t)chip->lfo_am << 1) >> shift;
}

/* A channel's sample, added to the outputs it is panned to */
static void channel_output(const s32_fm *chip, s32_fm_channel *channel, int32_t out[2])
{
	const uint32_t at = channel->offset;
	const uint32_t am = am_offset(chip, at);
	const s32_fm_operator *ops = channel->operators;

	/* operator 1 modulates itself with the sum of its last two outputs, scaled by the feedback level */
	const uint32_t feedback = chip_register(chip, 0xb0 + at, 3, 3);
	int32_t modulation = 0;

	if (feedback != 0)
		modulation = shift_down(channel->feedback[0] + channel->feedback[1], (int)(10 - feedback));

	const int32_t first = operator_output(chip, &ops[0], (ops[0].phase >> 10) + (uint32_t)modulation, am);

	channel->feedback_next = (int16_t)first;
	if (chip_register(chip, 0xb4 + at, 6, 2) == 0)
		return;

	/* the operators' outputs, and their sums, as the algorithms take them: 0 none, 1-3 operators 1-3,
	   5-7 operators 1 + 2, 1 + 3, 2 + 3 */
	const algorithm *route = &algorithms[chip_register(chip, 0xb0 + at, 0, 3)];
	int16_t outputs[8];

	outputs[0] = 0;
	outputs[1] = (int16_t)first;
	outputs[2] = (int16_t)operator_output(chip, &ops[1], (ops[1].phase >> 10) + (uint32_t)shift_down(outputs[route->input2], 1), am);
	outputs[5] = (int16_t)(outputs[1] + outputs[2]);
	outputs[3] = (int16_t)operator_output(chip, &ops[2], (ops[2].phase >> 10) + (uint32_t)shift_down(outputs[route->input3], 1), am);
	outputs[6] = (int16_t)(outputs[1] + outputs[3]);
	outputs[7] = (int16_t)(outputs[2] + outputs[3]);

	/* the carriers, 9 bits each, clipped to -257..256 as they add up (a single carrier needs no clip) */
	int32_t sum = shift_down(operator_output(chip, &ops[3], (ops[3].phase >> 10) + (uint32_t)shift_down(outputs[route->input4], 1), am), OUTPUT_SHIFT);

	for (int index = 0; index < 3; index++)
		if (route->carriers & (1u << index))
			sum = clamp(sum + shift_down(outputs[index + 1], OUTPUT_SHIFT), -OUTPUT_CLIP - 1, OUTPUT_CLIP);

	if (chip_register(chip, 0xb4 + at, 7, 1))
		out[0] += sum;
	if (chip_register(chip, 0xb4 + at, 6, 1))
		out[1] += sum;
}

/* The refresh of a channel: whether any of its operators may still be heard */
static bool refresh_channel(const s32_fm *chip, s32_fm_channel *channel)
{
	bool heard = false;

	for (int index = 0; index < 4; index++)
		if (refresh_operator(chip, channel, &channel->operators[index]))
			heard = true;
	return heard;
}

/* ---- the core ---- */

/* The LFO's step of a sample: it sets the AM, and gives the PM (-7 to 7) */
static int32_t clock_lfo(s32_fm *chip)
{
	if (!lfo_enabled(chip))
	{
		/* stopped, the counter stays at 0, where the AM is at its deepest */
		chip->lfo_counter = 0;
		chip->lfo_am = 0x3f;
		return 0;
	}

	/* bits 8-14 are the LFO's 7-bit value: when the low byte reaches the period, it carries (one past
	   it, as the chip does) */
	const uint32_t count = (uint8_t)chip->lfo_counter++;

	if (count >= lfo_periods[chip_register(chip, REG_LFO, 0, 3)])
		chip->lfo_counter += 0x101 - count;

	/* AM: a triangle, down over the first half */
	chip->lfo_am = (uint8_t)field(chip->lfo_counter, 8, 6);
	if (field(chip->lfo_counter, 14, 1) == 0)
		chip->lfo_am ^= 0x3f;

	/* PM: a triangle over each half, negative in the second */
	int32_t pm = (int32_t)field(chip->lfo_counter, 10, 3);

	if (field(chip->lfo_counter, 13, 1))
		pm ^= 7;
	return field(chip->lfo_counter, 14, 1) ? -pm : pm;
}

static void clock_chip(s32_fm *chip)
{
	chip->total_clocks++;

	/* a change to the registers, or 4097 samples without one, refreshes the channels */
	if (chip->modified_channels != 0 || chip->refresh_count++ >= 4096)
	{
		chip->active_channels = 0;
		for (int index = 0; index < S32_FM_CHANNELS; index++)
			if (refresh_channel(chip, &chip->channels[index]))
				chip->active_channels |= 1u << index;
		chip->modified_channels = 0;
		chip->refresh_count = 0;
	}

	/* the envelopes step on one sample in three: the low two bits count 0, 1, 2 */
	if (field(++chip->envelope_counter, 0, 2) == 3)
		chip->envelope_counter++;

	const int32_t lfo_pm = clock_lfo(chip);

	for (int index = 0; index < S32_FM_CHANNELS; index++)
	{
		s32_fm_channel *channel = &chip->channels[index];

		channel->feedback[0] = channel->feedback[1];
		channel->feedback[1] = channel->feedback_next;
		for (int op = 0; op < 4; op++)
			clock_operator(chip, channel, &channel->operators[op], chip->envelope_counter, lfo_pm);
	}
}

/* The mode register: CSM and channel 3's special mode, and the timers' resets, enables and loads. Timer
   B starts short by the low 4 bits of the sample count, since its divide by 16 runs free. */
static void write_mode(s32_fm *chip, uint8_t data)
{
	uint8_t clear = 0;

	chip->modified_channels = ALL_CHANNELS;
	chip->registers[REG_MODE] = data;
	if (field(data, 5, 1))
		clear |= STATUS_TIMER_B;
	if (field(data, 4, 1))
		clear |= STATUS_TIMER_A;
	change_status(chip, 0, clear);
	update_timer(chip, 1, field(data, 1, 1) != 0, -(int32_t)(chip->total_clocks & 15));
	update_timer(chip, 0, field(data, 0, 1) != 0, 0);
}

static void write_register(s32_fm *chip, uint32_t number, uint8_t data)
{
	if (number == REG_MODE)
	{
		write_mode(chip, data);
		return;
	}
	chip->modified_channels = ALL_CHANNELS;

	/* the frequencies: a write to the upper byte (0xA4-0xA6, 0xAC-0xAE) only latches it, the write to
	   the lower byte takes it along; one latch for each half of the block, shared by both banks */
	if ((number & 0xf0) == 0xa0)
	{
		if (field(number, 0, 2) == 3)
			return;

		const uint32_t latch = 0xb8 | field(number, 3, 1);

		if (field(number, 2, 1))
			chip->registers[latch] = data & 0x3f;
		else
		{
			chip->registers[number] = data;
			chip->registers[number | 4] = chip->registers[latch];
		}
		return;
	}
	/* where the latches live */
	if ((number & 0xf8) == 0xb8)
		return;

	chip->registers[number] = data;
	if (number == REG_KEY)
	{
		uint32_t channel = field(data, 0, 2);

		if (channel == 3)
			return;
		channel += field(data, 2, 1) * 3;
		key_channel(&chip->channels[channel], field(data, 4, 4), KEY_REGISTER);
	}
}

static void set_busy(s32_fm *chip)
{
	if (chip->set_busy_end)
		chip->set_busy_end(chip->host, BUSY_CLOCKS);
}

/* ---- the interface ---- */

void s32_fm_init(s32_fm *chip)
{
	void *host = chip->host;
	void (*set_timer)(void *, int, int32_t) = chip->set_timer;
	void (*update_irq)(void *, bool) = chip->update_irq;
	void (*set_busy_end)(void *, uint32_t) = chip->set_busy_end;
	bool (*is_busy)(void *) = chip->is_busy;

	memset(chip, 0, sizeof(*chip));
	chip->host = host;
	chip->set_timer = set_timer;
	chip->update_irq = update_irq;
	chip->set_busy_end = set_busy_end;
	chip->is_busy = is_busy;

	static const uint16_t operator_offsets[4] = { 0, 8, 4, 12 };

	for (int index = 0; index < S32_FM_CHANNELS; index++)
	{
		s32_fm_channel *channel = &chip->channels[index];

		channel->offset = (uint16_t)(index % 3 + 0x100 * (index / 3));
		for (int op = 0; op < 4; op++)
			channel->operators[op].offset = (uint16_t)(channel->offset + operator_offsets[op]);
	}
	chip->active_channels = ALL_CHANNELS;
	chip->modified_channels = ALL_CHANNELS;
	s32_fm_reset(chip);
}

void s32_fm_reset(s32_fm *chip)
{
	change_status(chip, 0, 0xff);
	memset(chip->registers, 0, sizeof(chip->registers));

	/* every channel to both outputs */
	chip->registers[0xb4] = chip->registers[0xb5] = chip->registers[0xb6] = 0xc0;
	chip->registers[0x1b4] = chip->registers[0x1b5] = chip->registers[0x1b6] = 0xc0;
	write_mode(chip, 0);

	for (int index = 0; index < S32_FM_CHANNELS; index++)
	{
		s32_fm_channel *channel = &chip->channels[index];

		channel->feedback[0] = channel->feedback[1] = 0;
		channel->feedback_next = 0;
		for (int op = 0; op < 4; op++)
		{
			s32_fm_operator *each = &channel->operators[op];

			each->phase = 0;
			each->attenuation = 0x3ff;
			each->stage = S32_FM_RELEASE;
			each->ssg_inverted = 0;
			each->key = 0;
			each->keys = 0;
		}
	}
}

uint8_t s32_fm_read(s32_fm *chip, int offset)
{
	/* the status is at offset 0; ymfm reads the other three as 0 */
	if ((offset & 3) != 0)
		return 0;

	uint8_t status = chip->status & (uint8_t)~STATUS_BUSY;

	if (chip->is_busy && chip->is_busy(chip->host))
		status |= STATUS_BUSY;
	return status;
}

void s32_fm_write(s32_fm *chip, int offset, uint8_t data)
{
	switch (offset & 3)
	{
		case 0:
			chip->address = data;
			break;
		case 1:
			/* a register of the upper bank takes offset 3 */
			if (chip->address & 0x100)
				return;
			if (chip->address == REG_DAC)
				chip->dac_data = (uint16_t)((chip->dac_data & ~0x1feu) | (uint32_t)(data ^ 0x80) << 1);
			else if (chip->address == REG_DAC_ENABLE)
				chip->dac_enable = (uint8_t)field(data, 7, 1);
			else if (chip->address == REG_DAC_LOW)
				chip->dac_data = (uint16_t)((chip->dac_data & ~1u) | field(data, 3, 1));
			else
				write_register(chip, chip->address, data);
			set_busy(chip);
			break;
		case 2:
			chip->address = 0x100 | data;
			break;
		case 3:
			if (!(chip->address & 0x100))
				return;
			write_register(chip, chip->address, data);
			set_busy(chip);
			break;
	}
}

void s32_fm_timer_expired(s32_fm *chip, int timer)
{
	if (timer != 0 && timer != 1)
		return;
	if (timer == 0 && chip_register(chip, REG_MODE, 2, 1))
		change_status(chip, STATUS_TIMER_A, 0);
	else if (timer == 1 && chip_register(chip, REG_MODE, 3, 1))
		change_status(chip, STATUS_TIMER_B, 0);

	/* CSM: timer A keys channel 3 on at the next refresh, and off at the one after, unless it runs out
	   again before */
	if (timer == 0 && csm_mode(chip))
	{
		key_channel(&chip->channels[2], 0xf, KEY_CSM);
		chip->modified_channels |= 1u << 2;
	}

	/* and it starts over */
	chip->timer_running[timer] = 0;
	update_timer(chip, timer, true, 0);
}

void s32_fm_generate(s32_fm *chip, int32_t out[2])
{
	uint32_t channels = ALL_CHANNELS;

	clock_chip(chip);
	out[0] = out[1] = 0;

	/* the DAC takes channel 6's place */
	if (chip->dac_enable)
	{
		const int32_t dac = sign_extend(chip->dac_data, 9);

		out[0] = chip_register(chip, 0x1b6, 7, 1) ? dac : 0;
		out[1] = chip_register(chip, 0x1b6, 6, 1) ? dac : 0;
		channels ^= 1u << 5;
	}

	channels &= chip->active_channels;
	for (int index = 0; index < S32_FM_CHANNELS; index++)
		if (channels & (1u << index))
			channel_output(chip, &chip->channels[index], out);

	/* the chip sends the channels one after the other: their average, to 16 bits */
	out[0] = out[0] * 128 / 6;
	out[1] = out[1] * 128 / 6;
}
