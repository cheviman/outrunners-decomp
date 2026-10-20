/**
 *    Name:        chipcheck.cpp
 *    Description: the game's YM3438 and RF5C68 against ymfm's and MAME's, sample for sample
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A development tool, never part of the game. It replays the chip writes s32ref logs (--sound-log)
 *    through two sound boards' worth of chips at once: the reference's (ymfm's ym3438 twice, MAME's
 *    rf5c68.cpp built here as s32sound.cpp builds it) and the game's own (runtime/src/ym3438.c,
 *    rf5c68.c), each with a wave memory of its own, and compares every sample of every chip, and after
 *    every write and every timer the YM3438s' status, the IRQ changes and what they ask of the host
 *    (timer periods, busy time). Time is s32sound's: sample n of a chip starts at n periods (288 board
 *    cycles for a YM3438, 384 x S32_CPU_CLOCK / S32_PCM_CLOCK for the RF5C68), and before a write at
 *    time t a chip makes every sample that starts before t. A YM3438's timer runs out at the board
 *    time it asked for, before the writes that come later.
 *
 *    The synthetic runs drive both sides with random sequences instead (registers, keys, timers and
 *    their expiries, status reads, the DAC, CSM, wave memory, loops), for what the game never does.
 *
 *    Usage: chipcheck [--synthetic SEEDS] [--steps N] [LOG ...]
 */
#include "s32/machine.h"
#include "s32/rf5c68.h"
#include "s32/ym3438.h"

#include "ymfm_opn.h"

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// MAME's RF5C68, in a namespace of its own over the stand-ins of this folder, as s32sound.cpp has it
namespace mame
{
#include "emu.h"
#include "../mame/sound/rf5c68.cpp"
}

namespace
{

constexpr uint64_t kFmPeriod = 288;                                 // board cycles a YM3438 sample takes
constexpr uint64_t kPcmNumerator = uint64_t(384) * S32_CPU_CLOCK;   // board cycles x S32_PCM_CLOCK an RF5C68 sample takes
constexpr uint64_t kNever = UINT64_MAX;
constexpr uint64_t kOneFrame = S32_CPU_CLOCK / 60;

const uint8_t kOperatorOffsets[4] = { 0, 8, 4, 12 };

// the time the chips' callbacks see
uint64_t g_now = 0;

// the first difference ends the run
bool g_failed = false;

void fail(const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	std::fputs("DIFFERENCE: ", stdout);
	std::vprintf(format, arguments);
	std::fputc('\n', stdout);
	va_end(arguments);
	g_failed = true;
}

uint64_t fm_samples_before(uint64_t time)
{
	return (time + kFmPeriod - 1) / kFmPeriod;
}

uint64_t pcm_samples_before(uint64_t time)
{
	return (time * S32_PCM_CLOCK + kPcmNumerator - 1) / kPcmNumerator;
}

// a list of counts, only the ones that are not 0: "index:count ..."
template <size_t Size>
std::string histogram(const uint64_t (&counts)[Size])
{
	std::string text;
	for (size_t index = 0; index < Size; index++)
		if (counts[index])
		{
			char item[48];
			std::snprintf(item, sizeof(item), "%s%zu:%" PRIu64, text.empty() ? "" : " ", index, counts[index]);
			text += item;
		}
	return text.empty() ? "-" : text;
}

// ---- what a YM3438 asks of its host ----

struct host_event
{
	uint64_t time;
	char kind;          // t: set_timer, b: set_busy_end, i: update_irq
	int32_t number;
	int64_t value;

	bool operator==(const host_event &other) const
	{
		return time == other.time && kind == other.kind && number == other.number && value == other.value;
	}
	bool operator!=(const host_event &other) const { return !(*this == other); }
};

std::string describe(const host_event &event)
{
	char text[96];
	if (event.kind == 't')
		std::snprintf(text, sizeof(text), "at %" PRIu64 " timer %c %" PRId64 " clocks", event.time, event.number ? 'B' : 'A', event.value);
	else if (event.kind == 'b')
		std::snprintf(text, sizeof(text), "at %" PRIu64 " busy %" PRId64 " clocks", event.time, event.value);
	else
		std::snprintf(text, sizeof(text), "at %" PRIu64 " IRQ %s", event.time, event.value ? "asserted" : "cleared");
	return text;
}

// The board's side of a YM3438, the same for both: timers and busy as s32sound.cpp keeps them (a chip
// clock is two board cycles)
struct fm_host
{
	uint64_t deadline[2] = { kNever, kNever };
	uint64_t busy_until = 0;
	std::vector<host_event> events;

	void set_timer(int timer, int32_t clocks)
	{
		events.push_back({ g_now, 't', timer, clocks });
		deadline[timer & 1] = clocks < 0 ? kNever : g_now + 2 * uint64_t(clocks);
	}

	void set_busy_end(uint32_t clocks)
	{
		events.push_back({ g_now, 'b', 0, clocks });
		busy_until = g_now + 2 * uint64_t(clocks);
	}

	bool is_busy() const
	{
		return g_now < busy_until;
	}

	void update_irq(bool asserted)
	{
		events.push_back({ g_now, 'i', 0, asserted ? 1 : 0 });
	}
};

// ymfm's YM3438
class reference_fm : public ymfm::ymfm_interface
{
public:
	fm_host host;

	reference_fm() : m_chip(*this) { m_chip.reset(); }

	void reset() { m_chip.reset(); }
	uint8_t read(int offset) { return m_chip.read(uint32_t(offset)); }
	void write(int offset, uint8_t data) { m_chip.write(uint32_t(offset), data); }
	void expire(int timer) { m_engine->engine_timer_expired(uint32_t(timer)); }

	void generate(int32_t out[2])
	{
		ymfm::ym3438::output_data output;
		m_chip.generate(&output);
		out[0] = output.data[0];
		out[1] = output.data[1];
	}

	// ymfm_interface
	void ymfm_set_timer(uint32_t number, int32_t clocks) override { host.set_timer(int(number), clocks); }
	void ymfm_set_busy_end(uint32_t clocks) override { host.set_busy_end(clocks); }
	bool ymfm_is_busy() override { return host.is_busy(); }
	void ymfm_update_irq(bool asserted) override { host.update_irq(asserted); }

private:
	ymfm::ym3438 m_chip;
};

// the game's YM3438
struct own_fm
{
	fm_host host;
	s32_fm chip;

	own_fm()
	{
		std::memset(&chip, 0, sizeof(chip));
		chip.host = &host;
		chip.set_timer = [](void *context, int timer, int32_t clocks) { static_cast<fm_host *>(context)->set_timer(timer, clocks); };
		chip.update_irq = [](void *context, bool asserted) { static_cast<fm_host *>(context)->update_irq(asserted); };
		chip.set_busy_end = [](void *context, uint32_t clocks) { static_cast<fm_host *>(context)->set_busy_end(clocks); };
		chip.is_busy = [](void *context) { return static_cast<const fm_host *>(context)->is_busy(); };
		s32_fm_init(&chip);
	}
};

// what the game made the YM3438 do
struct fm_coverage
{
	uint64_t writes[S32_FM_REGISTERS] = {};      // registers written through the data ports
	uint64_t ignored_writes = 0;                 // data port and selected bank do not match
	uint64_t key_ons = 0;
	uint64_t key_offs = 0;
	uint64_t channels[6] = {};
	uint64_t algorithms[8] = {};
	uint64_t feedback[8] = {};
	uint64_t pan[4] = {};                        // bit 1 left, bit 0 right
	uint64_t pm_depths[8] = {};                  // at key-ons with the LFO on
	uint64_t am_depths[4] = {};                  // at key-ons with the LFO on and AM on for an operator
	uint64_t lfo_rates[8] = {};
	uint64_t blocks[8] = {};
	uint64_t detunes[8] = {};                    // operators keyed on
	uint64_t multiples[16] = {};
	uint64_t key_scaling[4] = {};
	uint64_t ssg_modes[8] = {};                  // operators keyed on with SSG-EG
	uint64_t fast_attacks = 0;                   // attack rate 62-63 at a key on
	uint64_t dac_samples = 0;
	uint64_t special_samples = 0;                // channel 3 special mode
	uint64_t csm_samples = 0;
	uint64_t lfo_samples = 0;
	uint64_t expirations[2] = {};
	uint64_t timer_starts[2] = {};
	uint64_t timer_stops[2] = {};
	uint64_t irq_changes[2] = {};                // cleared, asserted
	uint64_t busy_requests = 0;
	uint64_t status_reads = 0;
	std::set<int> status_values;
	uint64_t clipped_samples = 0;                // outputs at or past the 16-bit limits
	uint64_t beyond_samples = 0;                 // past them (ymfm does not clamp)
	uint64_t nonzero_samples = 0;

	void key_written(const s32_fm &chip, uint8_t data)
	{
		if ((data & 3) == 3)
			return;
		const int channel = (data & 3) + ((data >> 2) & 1) * 3;
		const uint32_t operators = data >> 4;
		if (!operators)
		{
			key_offs++;
			return;
		}
		const uint8_t *registers = chip.registers;
		const uint32_t at = uint32_t(channel % 3 + 0x100 * (channel / 3));
		const bool lfo = (registers[0x22] & 8) != 0;
		key_ons++;
		channels[channel]++;
		algorithms[registers[0xb0 + at] & 7]++;
		feedback[(registers[0xb0 + at] >> 3) & 7]++;
		pan[registers[0xb4 + at] >> 6]++;
		blocks[(registers[0xa4 + at] >> 3) & 7]++;
		if (lfo)
		{
			lfo_rates[registers[0x22] & 7]++;
			pm_depths[registers[0xb4 + at] & 7]++;
		}
		bool am = false;
		for (int index = 0; index < 4; index++)
		{
			if (!((operators >> index) & 1))
				continue;
			const uint32_t op = at + kOperatorOffsets[index];
			detunes[(registers[0x30 + op] >> 4) & 7]++;
			multiples[registers[0x30 + op] & 15]++;
			key_scaling[registers[0x50 + op] >> 6]++;
			if ((registers[0x50 + op] & 0x1f) == 0x1f)
				fast_attacks++;
			if (registers[0x60 + op] & 0x80)
				am = true;
			if (registers[0x90 + op] & 8)
				ssg_modes[registers[0x90 + op] & 7]++;
		}
		if (lfo && am)
			am_depths[(registers[0xb4 + at] >> 4) & 3]++;
	}

	void print(const char *name) const
	{
		// the operator registers by block of 16, the others one by one
		std::string written;
		for (uint32_t number = 0; number < S32_FM_REGISTERS; number++)
		{
			const uint32_t low = number & 0xff;
			const bool block = low >= 0x30 && low < 0xa0;
			if (block && (low & 0x0f) != 0)
				continue;
			uint64_t count = 0;
			for (uint32_t other = number; other < number + (block ? 16 : 1); other++)
				count += writes[other];
			if (!count)
				continue;
			char item[16];
			if (block)
				std::snprintf(item, sizeof(item), " %03X-%03X", number, number + 0x0f);
			else
				std::snprintf(item, sizeof(item), " %03X", number);
			written += item;
		}
		std::printf("  %s registers written:%s%s\n", name, written.c_str(),
			ignored_writes ? (" (+" + std::to_string(ignored_writes) + " writes to the wrong data port)").c_str() : "");
		std::printf("  %s key-ons %" PRIu64 ", key-offs %" PRIu64 "; by channel %s\n", name, key_ons, key_offs, histogram(channels).c_str());
		std::printf("  %s at key-ons: algorithm %s; feedback %s; pan (L,R bits) %s; block %s\n", name,
			histogram(algorithms).c_str(), histogram(feedback).c_str(), histogram(pan).c_str(), histogram(blocks).c_str());
		std::printf("  %s operators keyed on: detune %s; multiple %s; key scaling %s; attack 31: %" PRIu64 "; SSG-EG modes %s\n", name,
			histogram(detunes).c_str(), histogram(multiples).c_str(), histogram(key_scaling).c_str(), fast_attacks, histogram(ssg_modes).c_str());
		std::printf("  %s LFO: rate %s; PM depth %s; AM depth (operators with AM) %s; samples with the LFO on %" PRIu64 "\n", name,
			histogram(lfo_rates).c_str(), histogram(pm_depths).c_str(), histogram(am_depths).c_str(), lfo_samples);
		std::printf("  %s samples not silent %" PRIu64 "; with the DAC on %" PRIu64 ", in channel 3 special mode %" PRIu64 ", in CSM %" PRIu64
			"; outputs at or past the 16-bit limits %" PRIu64 ", past them %" PRIu64 "\n",
			name, nonzero_samples, dac_samples, special_samples, csm_samples, clipped_samples, beyond_samples);
		std::string statuses;
		for (int value : status_values)
		{
			char item[8];
			std::snprintf(item, sizeof(item), " %02X", value);
			statuses += item;
		}
		std::printf("  %s timers: A started %" PRIu64 " stopped %" PRIu64 " expired %" PRIu64 "; B started %" PRIu64 " stopped %" PRIu64 " expired %" PRIu64
			"; IRQ asserted %" PRIu64 " cleared %" PRIu64 "; busy %" PRIu64 "; status reads %" PRIu64 " (values%s)\n", name,
			timer_starts[0], timer_stops[0], expirations[0], timer_starts[1], timer_stops[1], expirations[1],
			irq_changes[1], irq_changes[0], busy_requests, status_reads, statuses.c_str());
	}
};

// both YM3438s of one place on the board
struct fm_pair
{
	std::string name;
	reference_fm reference;
	own_fm own;
	uint64_t samples = 0;
	fm_coverage coverage;

	explicit fm_pair(std::string chip_name) : name(std::move(chip_name)) {}
};

bool fm_compare_host(fm_pair &pair, const char *after)
{
	std::vector<host_event> &theirs = pair.reference.host.events;
	std::vector<host_event> &ours = pair.own.host.events;
	if (theirs != ours)
	{
		std::string listing;
		for (const host_event &event : theirs)
			listing += "\n    ymfm: " + describe(event);
		for (const host_event &event : ours)
			listing += "\n    ours: " + describe(event);
		fail("%s: the host requests after %s differ at %" PRIu64 ":%s", pair.name.c_str(), after, g_now, listing.c_str());
		return false;
	}
	for (const host_event &event : ours)
	{
		if (event.kind == 't')
		{
			if (event.value < 0)
				pair.coverage.timer_stops[event.number & 1]++;
			else
				pair.coverage.timer_starts[event.number & 1]++;
		}
		else if (event.kind == 'b')
			pair.coverage.busy_requests++;
		else
			pair.coverage.irq_changes[event.value ? 1 : 0]++;
	}
	theirs.clear();
	ours.clear();
	return true;
}

bool fm_compare_status(fm_pair &pair, int offset, const char *after)
{
	const uint8_t theirs = pair.reference.read(offset);
	const uint8_t ours = s32_fm_read(&pair.own.chip, offset);
	pair.coverage.status_reads++;
	pair.coverage.status_values.insert(theirs);
	if (theirs != ours)
	{
		fail("%s: read of offset %d after %s at %" PRIu64 ": ymfm %02X, ours %02X", pair.name.c_str(), offset, after, g_now, theirs, ours);
		return false;
	}
	return true;
}

// every sample that starts before 'time'
bool fm_catch_up(fm_pair &pair, uint64_t time)
{
	const uint64_t due = fm_samples_before(time);
	while (pair.samples < due)
	{
		int32_t theirs[2];
		int32_t ours[2];
		pair.reference.generate(theirs);
		s32_fm_generate(&pair.own.chip, ours);
		if (theirs[0] != ours[0] || theirs[1] != ours[1])
		{
			fail("%s: sample %" PRIu64 " (time %" PRIu64 "): ymfm %d %d, ours %d %d", pair.name.c_str(), pair.samples,
				pair.samples * kFmPeriod, theirs[0], theirs[1], ours[0], ours[1]);
			return false;
		}
		const uint8_t mode = pair.own.chip.registers[0x27] >> 6;
		pair.coverage.dac_samples += pair.own.chip.dac_enable;
		pair.coverage.special_samples += mode == 1;
		pair.coverage.csm_samples += mode == 2;
		pair.coverage.lfo_samples += (pair.own.chip.registers[0x22] >> 3) & 1;
		for (int side = 0; side < 2; side++)
		{
			if (ours[side] >= 32767 || ours[side] <= -32768)
				pair.coverage.clipped_samples++;
			if (ours[side] > 32767 || ours[side] < -32768)
				pair.coverage.beyond_samples++;
		}
		if (ours[0] || ours[1])
			pair.coverage.nonzero_samples++;
		pair.samples++;
	}
	return true;
}

// the timers that run out before 'time', at the time they asked for
bool fm_fire_timers(fm_pair &pair, uint64_t time)
{
	for (;;)
	{
		const uint64_t *deadline = pair.reference.host.deadline;
		if (deadline[0] != pair.own.host.deadline[0] || deadline[1] != pair.own.host.deadline[1])
		{
			fail("%s: timer deadlines differ at %" PRIu64 ": ymfm %" PRIu64 " %" PRIu64 ", ours %" PRIu64 " %" PRIu64, pair.name.c_str(), g_now,
				deadline[0], deadline[1], pair.own.host.deadline[0], pair.own.host.deadline[1]);
			return false;
		}
		const uint64_t next = std::min(deadline[0], deadline[1]);
		if (next >= time)
			return true;
		const int timer = deadline[0] <= deadline[1] ? 0 : 1;
		pair.reference.host.deadline[timer] = kNever;
		pair.own.host.deadline[timer] = kNever;
		if (!fm_catch_up(pair, next))
			return false;
		g_now = next;
		pair.reference.expire(timer);
		s32_fm_timer_expired(&pair.own.chip, timer);
		pair.coverage.expirations[timer]++;
		const char *after = timer ? "timer B ran out" : "timer A ran out";
		if (!fm_compare_host(pair, after) || !fm_compare_status(pair, 0, after))
			return false;
	}
}

bool fm_write(fm_pair &pair, uint64_t time, int offset, uint8_t data)
{
	if (!fm_fire_timers(pair, time) || !fm_catch_up(pair, time))
		return false;
	g_now = time;
	const uint16_t address = pair.own.chip.address;
	pair.reference.write(offset, data);
	s32_fm_write(&pair.own.chip, offset, data);
	if (offset & 1)
	{
		if (((offset & 2) != 0) != ((address & 0x100) != 0))
			pair.coverage.ignored_writes++;
		else
		{
			pair.coverage.writes[address & 0x1ff]++;
			if (address == 0x28)
				pair.coverage.key_written(pair.own.chip, data);
		}
	}
	char after[48];
	std::snprintf(after, sizeof(after), "writing %02X to offset %d", data, offset);
	return fm_compare_host(pair, after) && fm_compare_status(pair, 0, after);
}

// a register through the ports
bool fm_write_register(fm_pair &pair, uint64_t time, uint32_t number, uint8_t data)
{
	const int bank = (number & 0x100) ? 2 : 0;
	return fm_write(pair, time, bank, uint8_t(number)) && fm_write(pair, time, bank + 1, data);
}

// ---- the RF5C68s ----

mame::machine_config g_config;

// MAME's RF5C68 on a wave memory of its own
class reference_pcm : public mame::rf5c68_device
{
public:
	explicit reference_pcm(uint8_t *memory) : mame::rf5c68_device(g_config, "rfsnd", nullptr, S32_PCM_CLOCK)
	{
		space(0).read = [memory](uint32_t address) { return memory[address & 0xffff]; };
		space(0).write = [memory](uint32_t address, uint8_t data) { memory[address & 0xffff] = data; };
		start();
	}
};

// what the game made the RF5C68 do
struct pcm_coverage
{
	uint64_t writes[16] = {};
	uint64_t memory_writes = 0;
	uint64_t memory_reads = 0;
	std::set<int> channel_banks;
	std::set<int> wave_banks;
	uint64_t enabled_samples = 0;
	uint64_t channel_samples[S32_PCM_CHANNELS] = {};
	uint64_t loops = 0;                         // a channel read 0xFF and went to its loop
	uint64_t stuck = 0;                         // and found 0xFF there too
	uint64_t top = 0;                           // outputs at 32767: clamped, a 10-bit value never is
	uint64_t bottom = 0;                        // at -32768: clamped or exactly there
	uint64_t nonzero_samples = 0;
	int32_t peak = 0;

	// before a sample: what each channel is about to do
	void sample_due(const s32_pcm &chip)
	{
		if (!chip.enable)
			return;
		enabled_samples++;
		for (int index = 0; index < S32_PCM_CHANNELS; index++)
		{
			const s32_pcm_channel &channel = chip.channels[index];
			if (!channel.enable)
				continue;
			channel_samples[index]++;
			if (chip.memory[(channel.address >> 11) & 0xffff] == 0xff)
			{
				loops++;
				if (chip.memory[channel.loop] == 0xff)
					stuck++;
			}
		}
	}

	void print() const
	{
		auto list = [](const std::set<int> &values) {
			std::string text;
			for (int value : values)
				text += (text.empty() ? "" : " ") + std::to_string(value);
			return text.empty() ? std::string("-") : text;
		};
		std::printf("  pcm writes by register %s; wave memory writes %" PRIu64 "\n", histogram(writes).c_str(), memory_writes);
		std::printf("  pcm channels selected %s; wave banks %s\n", list(channel_banks).c_str(), list(wave_banks).c_str());
		std::printf("  pcm samples with the chip on %" PRIu64 "; by channel on %s\n", enabled_samples, histogram(channel_samples).c_str());
		std::printf("  pcm loop marks reached %" PRIu64 " (onto another loop mark %" PRIu64 "); samples not silent %" PRIu64
			"; outputs at 32767 (clamped) %" PRIu64 ", at -32768 %" PRIu64 "; peak %d\n", loops, stuck, nonzero_samples, top, bottom, peak);
	}
};

struct pcm_pair
{
	std::vector<uint8_t> reference_memory;
	std::vector<uint8_t> own_memory;
	std::unique_ptr<reference_pcm> reference;
	s32_pcm own;
	uint64_t samples = 0;
	pcm_coverage coverage;

	pcm_pair() : reference_memory(S32_PCM_MEMORY_SIZE, 0xff), own_memory(S32_PCM_MEMORY_SIZE, 0xff)
	{
		reference = std::make_unique<reference_pcm>(reference_memory.data());
		s32_pcm_reset(&own, own_memory.data());
	}
};

// every sample that starts before 'time'; MAME's in batches, as s32sound.cpp asks for them
bool pcm_catch_up(pcm_pair &pair, uint64_t time)
{
	const uint64_t due = pcm_samples_before(time);
	while (pair.samples < due)
	{
		const int count = int(std::min<uint64_t>(due - pair.samples, 4096));
		pair.reference->generate(count);
		const mame::sound_stream &stream = pair.reference->stream();
		for (int index = 0; index < count; index++)
		{
			pair.coverage.sample_due(pair.own);
			int32_t ours[2];
			s32_pcm_generate(&pair.own, ours);
			const int32_t theirs[2] = { stream.m_outputs[0][size_t(index)], stream.m_outputs[1][size_t(index)] };
			if (theirs[0] != ours[0] || theirs[1] != ours[1])
			{
				fail("pcm: sample %" PRIu64 " (time %" PRIu64 "): MAME %d %d, ours %d %d", pair.samples,
					(pair.samples * kPcmNumerator + S32_PCM_CLOCK - 1) / S32_PCM_CLOCK, theirs[0], theirs[1], ours[0], ours[1]);
				return false;
			}
			for (int side = 0; side < 2; side++)
			{
				pair.coverage.top += ours[side] == 32767;
				pair.coverage.bottom += ours[side] == -32768;
				pair.coverage.peak = std::max(pair.coverage.peak, ours[side] < 0 ? -ours[side] : ours[side]);
			}
			if (ours[0] || ours[1])
				pair.coverage.nonzero_samples++;
			pair.samples++;
		}
	}
	return true;
}

bool pcm_write(pcm_pair &pair, uint64_t time, int offset, uint8_t data)
{
	if (!pcm_catch_up(pair, time))
		return false;
	g_now = time;
	pair.reference->rf5c68_w(uint32_t(offset), data);
	s32_pcm_write(&pair.own, offset, data);
	pair.coverage.writes[offset & 15]++;
	if (offset == 7)
	{
		if (data & 0x40)
			pair.coverage.channel_banks.insert(data & 7);
		else
			pair.coverage.wave_banks.insert(data & 15);
	}
	return true;
}

bool pcm_memory_check(pcm_pair &pair, int offset)
{
	const uint8_t theirs = pair.reference->rf5c68_mem_r(uint32_t(offset));
	const uint8_t ours = s32_pcm_memory_read(&pair.own, offset);
	pair.coverage.memory_reads++;
	if (theirs != ours)
	{
		fail("pcm: wave memory read at offset %03X (time %" PRIu64 "): MAME %02X, ours %02X", offset, g_now, theirs, ours);
		return false;
	}
	return true;
}

bool pcm_memory_write(pcm_pair &pair, uint64_t time, int offset, uint8_t data)
{
	if (!pcm_catch_up(pair, time))
		return false;
	g_now = time;
	pair.reference->rf5c68_mem_w(uint32_t(offset), data);
	s32_pcm_memory_write(&pair.own, offset, data);
	pair.coverage.memory_writes++;
	return pcm_memory_check(pair, offset);
}

bool pcm_memories_match(pcm_pair &pair)
{
	if (pair.reference_memory != pair.own_memory)
	{
		for (size_t index = 0; index < pair.own_memory.size(); index++)
			if (pair.reference_memory[index] != pair.own_memory[index])
			{
				fail("pcm: wave memory differs at %04zX: MAME %02X, ours %02X", index, pair.reference_memory[index], pair.own_memory[index]);
				break;
			}
		return false;
	}
	return true;
}

// ---- a board's worth ----

struct board
{
	std::unique_ptr<fm_pair> fm[2];
	std::unique_ptr<pcm_pair> pcm;

	board()
	{
		g_now = 0;
		fm[0] = std::make_unique<fm_pair>("fm0");
		fm[1] = std::make_unique<fm_pair>("fm1");
		pcm = std::make_unique<pcm_pair>();
	}

	// everything up to 'time'
	bool finish(uint64_t time)
	{
		for (auto &chip : fm)
			if (!fm_compare_host(*chip, "start") || !fm_fire_timers(*chip, time) || !fm_catch_up(*chip, time))
				return false;
		return pcm_catch_up(*pcm, time) && pcm_memories_match(*pcm);
	}
};

// ---- write logs ----

bool replay(const char *path)
{
	FILE *file = std::fopen(path, "r");
	if (!file)
	{
		std::printf("%s: cannot be opened\n", path);
		return false;
	}
	std::printf("%s\n", path);

	board chips;
	uint64_t writes[4] = {};
	uint64_t last_time = 0;
	uint64_t line_number = 0;
	char line[128];
	bool ok = fm_compare_host(*chips.fm[0], "power on") && fm_compare_host(*chips.fm[1], "power on");

	while (ok && std::fgets(line, sizeof(line), file))
	{
		line_number++;
		char chip[16];
		unsigned long long time;
		unsigned offset;
		unsigned data;
		if (std::sscanf(line, "%llu %15s %x %x", &time, chip, &offset, &data) != 4 || data > 0xff)
		{
			std::printf("%s: line %" PRIu64 " is not TIME CHIP OFFSET DATA\n", path, line_number);
			ok = false;
			break;
		}
		if (time < last_time)
		{
			std::printf("%s: line %" PRIu64 " goes back in time\n", path, line_number);
			ok = false;
			break;
		}
		last_time = time;
		int kind = -1;
		if (std::strcmp(chip, "fm0") == 0 || std::strcmp(chip, "fm1") == 0)
			kind = chip[2] - '0';
		else if (std::strcmp(chip, "pcm") == 0)
			kind = 2;
		else if (std::strcmp(chip, "pcm_memory") == 0)
			kind = 3;
		static const unsigned offsets[4] = { 4, 4, 16, 0x1000 };
		if (kind < 0 || offset >= offsets[kind])
		{
			std::printf("%s: line %" PRIu64 " names no chip or offset of the board\n", path, line_number);
			ok = false;
			break;
		}
		writes[kind]++;
		if (kind < 2)
			ok = fm_write(*chips.fm[kind], time, int(offset), uint8_t(data));
		else if (kind == 2)
			ok = pcm_write(*chips.pcm, time, int(offset), uint8_t(data));
		else
			ok = pcm_memory_write(*chips.pcm, time, int(offset), uint8_t(data));
	}
	std::fclose(file);

	// and one frame past the last write
	if (ok)
		ok = chips.finish(last_time + kOneFrame);
	if (g_failed)
		std::printf("  at line %" PRIu64 " of %s\n", line_number, path);
	if (!ok)
		return false;

	std::printf("  %" PRIu64 " writes (fm0 %" PRIu64 ", fm1 %" PRIu64 ", pcm %" PRIu64 ", wave memory %" PRIu64 ") over %" PRIu64 " board cycles (%.1f s)\n",
		writes[0] + writes[1] + writes[2] + writes[3], writes[0], writes[1], writes[2], writes[3], last_time, double(last_time) / S32_CPU_CLOCK);
	chips.fm[0]->coverage.print("fm0");
	chips.fm[1]->coverage.print("fm1");
	chips.pcm->coverage.print();
	std::printf("  identical: fm0 %" PRIu64 " samples, fm1 %" PRIu64 " samples, pcm %" PRIu64 " samples; status reads %" PRIu64 ", timer expirations %" PRIu64 "\n",
		chips.fm[0]->samples, chips.fm[1]->samples, chips.pcm->samples,
		chips.fm[0]->coverage.status_reads + chips.fm[1]->coverage.status_reads,
		chips.fm[0]->coverage.expirations[0] + chips.fm[0]->coverage.expirations[1] + chips.fm[1]->coverage.expirations[0] + chips.fm[1]->coverage.expirations[1]);
	return true;
}

// ---- synthetic sequences ----

struct random_source
{
	uint64_t state;

	uint64_t next()
	{
		// splitmix64
		uint64_t value = (state += 0x9e3779b97f4a7c15ull);
		value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ull;
		value = (value ^ (value >> 27)) * 0x94d049bb133111ebull;
		return value ^ (value >> 31);
	}

	uint32_t below(uint32_t limit) { return uint32_t(next() % limit); }
	uint8_t byte() { return uint8_t(next()); }
	bool chance(uint32_t in) { return below(in) == 0; }
};

// an audible voice on every channel, all keyed on
bool fm_voices(fm_pair &pair, random_source &random, uint64_t time)
{
	bool ok = true;
	for (int channel = 0; ok && channel < 6; channel++)
	{
		const uint32_t at = uint32_t(channel % 3 + 0x100 * (channel / 3));
		ok = fm_write_register(pair, time, 0xb0 + at, uint8_t(random.below(64)))
			&& fm_write_register(pair, time, 0xb4 + at, uint8_t(0xc0 | random.below(64)))
			&& fm_write_register(pair, time, 0xa4 + at, uint8_t(random.below(64)))
			&& fm_write_register(pair, time, 0xa0 + at, random.byte());
		for (int index = 0; ok && index < 4; index++)
		{
			const uint32_t op = at + kOperatorOffsets[index];
			ok = fm_write_register(pair, time, 0x30 + op, random.byte())
				&& fm_write_register(pair, time, 0x40 + op, uint8_t(random.below(40)))
				&& fm_write_register(pair, time, 0x50 + op, uint8_t(random.byte() | 0x18))
				&& fm_write_register(pair, time, 0x60 + op, uint8_t(random.below(12) | (random.chance(4) ? 0x80 : 0)))
				&& fm_write_register(pair, time, 0x70 + op, uint8_t(random.below(8)))
				&& fm_write_register(pair, time, 0x80 + op, random.byte())
				&& fm_write_register(pair, time, 0x90 + op, random.chance(4) ? uint8_t(8 | random.below(8)) : 0);
		}
		ok = ok && fm_write_register(pair, time, 0x28, uint8_t(0xf0 | (channel % 3) | (channel / 3) << 2));
	}
	return ok;
}

bool synthetic_fm(uint64_t seed, uint64_t steps)
{
	random_source random{ seed * 0x632be59bd9b4e019ull + 1 };
	g_now = 0;
	fm_pair pair("fm (synthetic)");
	uint64_t time = 0;
	uint64_t actions = 0;

	if (!fm_compare_host(pair, "power on") || !fm_voices(pair, random, time))
		return false;

	for (uint64_t step = 0; step < steps; step++)
	{
		time += random.chance(64) ? random.below(400000) : random.below(3000);
		if (!fm_fire_timers(pair, time) || !fm_catch_up(pair, time))
			return false;
		g_now = time;
		actions++;

		const uint32_t action = random.below(100);
		const uint32_t bank = random.chance(2) ? 0x100 : 0;
		bool ok = true;
		if (action < 20)
		{
			// operator registers 0x30-0x9F, carriers' total level mostly low
			const uint32_t number = bank | (0x30 + random.below(0x70));
			uint8_t data = random.byte();
			if ((number & 0xf0) == 0x40 && random.chance(2))
				data = uint8_t(random.below(32));
			ok = fm_write_register(pair, time, number, data);
		}
		else if (action < 34)
			ok = fm_write_register(pair, time, 0x28, random.byte());
		else if (action < 42)
		{
			// a frequency, upper byte first as the manual has it; channel 3's special ones too
			const uint32_t number = random.chance(4) ? 0xa8 + random.below(3) : bank | (0xa0 + random.below(3));
			ok = fm_write_register(pair, time, number + 4, uint8_t(random.below(64))) && fm_write_register(pair, time, number, random.byte());
		}
		else if (action < 46)
			ok = fm_write_register(pair, time, bank | (0xa0 + random.below(0x20)), random.byte());
		else if (action < 52)
			ok = fm_write_register(pair, time, bank | (0xb0 + random.below(8)), random.byte());
		else if (action < 56)
			ok = fm_write_register(pair, time, 0x22, uint8_t(random.chance(3) ? random.below(8) : 8 | random.below(8)));
		else if (action < 63)
		{
			// the mode: timers loaded, enabled and reset in every way, CSM and the special mode
			static const uint8_t modes[] = { 0x00, 0x15, 0x2a, 0x3f, 0x05, 0x0a, 0x0f, 0x30, 0x95, 0x85, 0x55, 0x45, 0x81, 0xbf, 0x7f, 0xc5 };
			ok = fm_write_register(pair, time, 0x27, random.chance(2) ? modes[random.below(sizeof(modes))] : random.byte());
		}
		else if (action < 68)
		{
			// timer periods, sometimes the shortest
			const uint32_t number = 0x24 + random.below(3);
			ok = fm_write_register(pair, time, number, random.chance(3) ? 0xff : random.byte());
		}
		else if (action < 74)
		{
			// the DAC
			const uint32_t which = random.below(4);
			if (which == 0)
				ok = fm_write_register(pair, time, 0x2b, uint8_t(random.chance(2) ? 0x80 : 0x00));
			else if (which == 1)
				ok = fm_write_register(pair, time, 0x2c, random.byte());
			else
				ok = fm_write_register(pair, time, 0x2a, random.byte());
		}
		else if (action < 78)
		{
			// a port alone: an address, or data to the port of the other bank (ignored)
			ok = fm_write(pair, time, int(random.below(4)), random.byte());
		}
		else if (action < 84)
		{
			const int offset = int(random.below(4));
			ok = fm_compare_status(pair, offset, "a status read");
		}
		else if (action < 86)
		{
			// a timer the host lets run out early, running or not
			const int timer = int(random.below(2));
			pair.reference.host.deadline[timer] = kNever;
			pair.own.host.deadline[timer] = kNever;
			pair.reference.expire(timer);
			s32_fm_timer_expired(&pair.own.chip, timer);
			pair.coverage.expirations[timer]++;
			ok = fm_compare_host(pair, "an early expiry") && fm_compare_status(pair, 0, "an early expiry");
		}
		else if (action < 87 && random.chance(8))
		{
			pair.reference.reset();
			s32_fm_reset(&pair.own.chip);
			ok = fm_compare_host(pair, "a reset") && fm_voices(pair, random, time);
		}
		else if (action < 90)
			ok = fm_voices(pair, random, time);
		else
			ok = fm_write_register(pair, time, random.below(S32_FM_REGISTERS), random.byte());
		if (!ok)
			return false;
	}
	if (!fm_fire_timers(pair, time + kOneFrame) || !fm_catch_up(pair, time + kOneFrame))
		return false;

	std::printf("synthetic fm, seed %" PRIu64 ": %" PRIu64 " actions over %" PRIu64 " board cycles; %" PRIu64 " samples identical\n",
		seed, actions, time, pair.samples);
	pair.coverage.print("fm");
	return true;
}

bool synthetic_pcm(uint64_t seed, uint64_t steps)
{
	random_source random{ seed * 0xd1342543de82ef95ull + 7 };
	g_now = 0;
	pcm_pair pair;
	uint64_t time = 0;

	// waves in every bank, with loop marks here and there
	for (int bank = 0; bank < 16; bank++)
	{
		if (!pcm_write(pair, time, 7, uint8_t(bank)))
			return false;
		for (int offset = 0; offset < 0x1000; offset++)
		{
			const uint8_t data = random.chance(512) ? 0xff : uint8_t(random.byte() & 0xfe);
			if (!pcm_memory_write(pair, time, offset, data))
				return false;
		}
	}

	for (uint64_t step = 0; step < steps; step++)
	{
		time += random.chance(32) ? random.below(200000) : random.below(4000);
		g_now = time;
		const uint32_t action = random.below(100);
		bool ok = true;
		if (action < 40)
		{
			// a channel register: steps up to 8 samples, so that the channels move through their waves
			const int offset = int(random.below(7));
			uint8_t data = random.byte();
			if (offset == 3)
				data = uint8_t(random.below(16));
			ok = pcm_write(pair, time, offset, data);
		}
		else if (action < 52)
			ok = pcm_write(pair, time, 7, uint8_t((random.chance(4) ? 0 : 0x80) | 0x40 | random.below(8)));
		else if (action < 58)
			ok = pcm_write(pair, time, 7, uint8_t((random.chance(4) ? 0 : 0x80) | random.below(16)));
		else if (action < 66)
			ok = pcm_write(pair, time, 8, random.byte());
		else if (action < 68)
			ok = pcm_write(pair, time, int(9 + random.below(7)), random.byte());
		else if (action < 88)
		{
			// a burst into the wave memory, loop marks included
			const int start = int(random.below(0x1000));
			const int length = int(1 + random.below(64));
			for (int index = 0; ok && index < length; index++)
				ok = pcm_memory_write(pair, time, (start + index) & 0xfff, random.chance(16) ? 0xff : random.byte());
		}
		else
			ok = pcm_memory_check(pair, int(random.below(0x1000)));
		if (!ok)
			return false;
	}
	if (!pcm_catch_up(pair, time + kOneFrame) || !pcm_memories_match(pair))
		return false;

	std::printf("synthetic pcm, seed %" PRIu64 ": %" PRIu64 " steps over %" PRIu64 " board cycles; %" PRIu64 " samples identical, wave memory reads %" PRIu64 "\n",
		seed, steps, time, pair.samples, pair.coverage.memory_reads);
	pair.coverage.print();
	return true;
}

}    // namespace

int main(int argc, char **argv)
{
	uint64_t seeds = 0;
	uint64_t steps = 200000;
	std::vector<const char *> logs;

	for (int index = 1; index < argc; index++)
	{
		const std::string argument = argv[index];
		if (argument == "--synthetic" && index + 1 < argc)
			seeds = std::strtoull(argv[++index], nullptr, 10);
		else if (argument == "--steps" && index + 1 < argc)
			steps = std::strtoull(argv[++index], nullptr, 10);
		else if (!argument.empty() && argument[0] == '-')
		{
			std::printf("unknown option %s\n", argument.c_str());
			return 2;
		}
		else
			logs.push_back(argv[index]);
	}
	if (logs.empty() && seeds == 0)
	{
		std::printf("Usage: chipcheck [--synthetic SEEDS] [--steps N] [LOG ...]\n"
			"  LOG: a chip write log of s32ref (--sound-log), replayed through ymfm's and MAME's chips and the game's\n"
			"  --synthetic: that many random sequences for each chip type, N actions each (200000)\n");
		return 2;
	}

	bool ok = true;
	for (const char *log : logs)
		ok = replay(log) && ok;
	for (uint64_t seed = 1; seed <= seeds; seed++)
	{
		g_failed = false;
		ok = synthetic_fm(seed, steps) && ok;
		g_failed = false;
		ok = synthetic_pcm(seed, steps) && ok;
	}
	std::printf(ok ? "identical\n" : "DIFFERENT\n");
	return ok ? 0 : 1;
}
