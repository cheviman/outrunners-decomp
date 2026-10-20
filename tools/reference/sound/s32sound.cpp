/**
 *    Name:        s32sound.cpp
 *    Description: reference Multi 32 sound board: MAME's Z80 and MultiPCM and ymfm's YM3438 on the board model
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    MAME's files are compiled here inside a namespace of their own, over the stand-ins of this folder
 *    (emu.h, dirom.h, machine/z80daisy.h, logmacro.h, wavwrite.h), so that their names keep away from the
 *    V70 core's stand-ins in the same program. ymfm is a library of its own and builds as it is.
 *
 *    The Z80 runs cycle by cycle in slices that end at the YM3438's timers (a timer set while it runs
 *    cuts its slice short), and takes interrupts as MAME's core does. Its time is its own cycle count,
 *    which gives the board's: 2.5 board cycles a Z80 cycle.
 */
#include "s32sound.h"

#include "s32/machine.h"

#include "ymfm_opn.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace s32s
{
#include "emu.h"
#include "logmacro.h"
#include "../mame/z80/z80.cpp"
#include "../mame/z80/z80dasm.cpp"
#include "../mame/sound/gew.cpp"
#include "../mame/sound/multipcm.cpp"
}

namespace
{

constexpr uint64_t kFmPeriod = 144ull * S32_CPU_CLOCK / S32_SOUND_CLOCK;       // board cycles a YM3438 sample takes
constexpr uint64_t kPcmPeriod = 224ull * S32_CPU_CLOCK / S32_MULTIPCM_CLOCK;   // board cycles a MultiPCM sample takes
static_assert(kFmPeriod * S32_SOUND_CLOCK == 144ull * S32_CPU_CLOCK, "a YM3438 sample is a whole number of board cycles");
static_assert(kPcmPeriod * S32_MULTIPCM_CLOCK == 224ull * S32_CPU_CLOCK, "a MultiPCM sample is a whole number of board cycles");

// the board's time when the Z80 has run 'cycles', and the first Z80 cycle at or after board time 'time'
uint64_t board_time(uint64_t cycles)
{
	return cycles * S32_CPU_CLOCK / S32_SOUND_CLOCK;
}

uint64_t z80_time(uint64_t time)
{
	return (time * S32_SOUND_CLOCK + S32_CPU_CLOCK - 1) / S32_CPU_CLOCK;
}

// board cycles 'clocks' of the YM3438 (which runs on the Z80's clock) take, rounded up
uint64_t fm_span(uint64_t clocks)
{
	return (clocks * S32_CPU_CLOCK + S32_SOUND_CLOCK - 1) / S32_SOUND_CLOCK;
}

// time: the board's, the Z80's cycles, and while the Z80 runs, where its slice started and how long it is
uint64_t g_time = 0;
uint64_t g_z80_time = 0;
bool g_running = false;
uint64_t g_slice_z80 = 0;
int g_slice_budget = 0;
uint64_t g_now_override = UINT64_MAX;       // while a timer fires, its own time

// ---- the Z80 ----

class sound_cpu : public s32s::z80_device
{
public:
	sound_cpu(const s32s::machine_config &config) : s32s::z80_device(config, "soundcpu", nullptr, S32_SOUND_CLOCK) {}

	// how many of the slice's cycles have gone by
	int cycles_done() const { return g_slice_budget - cycles_left(); }

	// end the running slice early, with no more than 'left' cycles to go: how many it took off
	int cut_slice(int left)
	{
		const int taken = cycles_left() - left;
		if (taken <= 0)
			return 0;
		m_icount -= taken;
		m_cut += taken;
		return taken;
	}

	int run_slice(int cycles)
	{
		m_cut = 0;
		const int done = run(cycles);
		return done - m_cut;
	}

private:
	int m_cut = 0;
};

s32s::machine_config g_config;
std::unique_ptr<sound_cpu> g_z80;
bool g_z80_held = true;
uint8_t g_vector = 0xff;
uint64_t g_z80_cycles = 0;
uint8_t g_executed[0x10000 / 8];

uint64_t running_time()
{
	return board_time(g_slice_z80 + uint64_t(g_z80->cycles_done()));
}

uint64_t current_time()
{
	if (g_now_override != UINT64_MAX)
		return g_now_override;
	return g_running ? running_time() : g_time;
}

// ---- recording ----

FILE *g_write_log = nullptr;

// a chip's samples, left and right interleaved, from sample number 'first' on
struct samples
{
	std::vector<int32_t> data;
	uint64_t first = 0;
};
samples g_fm_samples;
samples g_pcm_samples;
std::vector<int16_t> g_mix;
uint64_t g_frame_samples_hash = 0;

void log_write(const char *chip, int offset, uint8_t data)
{
	if (g_write_log)
		std::fprintf(g_write_log, "%llu %s %X %02X\n", (unsigned long long)current_time(), chip, offset, data);
}

// ---- the YM3438 ----

class fm_chip : public ymfm::ymfm_interface
{
public:
	fm_chip() : m_chip(*this) { m_chip.reset(); }

	// every sample that starts before 'time'
	void catch_up(uint64_t time)
	{
		const uint64_t due = (time + kFmPeriod - 1) / kFmPeriod;
		ymfm::ym3438::output_data output;
		while (m_samples < due)
		{
			m_chip.generate(&output);
			g_fm_samples.data.push_back(output.data[0]);
			g_fm_samples.data.push_back(output.data[1]);
			m_samples++;
		}
	}

	uint8_t read(int offset)
	{
		return m_chip.read(uint32_t(offset));
	}

	void write(int offset, uint8_t data)
	{
		catch_up(current_time());
		log_write("fm", offset, data);
		m_chip.write(uint32_t(offset), data);
	}

	uint64_t next_timer() const { return std::min(m_deadline[0], m_deadline[1]); }

	// the timers due by 'time', in the order they run out
	void fire(uint64_t time)
	{
		while (next_timer() <= time)
		{
			const int number = m_deadline[0] <= m_deadline[1] ? 0 : 1;
			const uint64_t when = m_deadline[number];

			m_deadline[number] = UINT64_MAX;
			catch_up(when);
			g_now_override = when;
			m_engine->engine_timer_expired(uint32_t(number));
			g_now_override = UINT64_MAX;
		}
	}

	// ymfm_interface
	void ymfm_set_timer(uint32_t number, int32_t clocks) override
	{
		if (clocks < 0)
		{
			m_deadline[number] = UINT64_MAX;
			return;
		}
		m_deadline[number] = current_time() + fm_span(uint64_t(clocks));
		// a running Z80 stops by then, for the interrupt to come on time
		if (g_running && g_now_override == UINT64_MAX
			&& m_deadline[number] < board_time(g_slice_z80 + uint64_t(g_slice_budget)))
		{
			const uint64_t at = g_slice_z80 + uint64_t(g_z80->cycles_done());
			const uint64_t due = z80_time(m_deadline[number]);
			g_slice_budget -= g_z80->cut_slice(due > at ? int(due - at) : 0);
		}
	}

	void ymfm_set_busy_end(uint32_t clocks) override
	{
		m_busy_until = current_time() + fm_span(clocks);
	}

	bool ymfm_is_busy() override
	{
		return current_time() < m_busy_until;
	}

	void ymfm_update_irq(bool asserted) override
	{
		if (asserted)
			s32_sound_signal_irq(S32_SOUND_IRQ_FM);
		else
			s32_sound_clear_irq(S32_SOUND_IRQ_FM);
	}

private:
	ymfm::ym3438 m_chip;
	uint64_t m_samples = 0;
	uint64_t m_deadline[2] = { UINT64_MAX, UINT64_MAX };
	uint64_t m_busy_until = 0;
};

std::unique_ptr<fm_chip> g_fm;

// ---- the MultiPCM ----

class pcm_chip : public s32s::multipcm_device
{
public:
	explicit pcm_chip(const s32s::machine_config &config) : s32s::multipcm_device(config, "multipcm", nullptr, S32_MULTIPCM_CLOCK)
	{
		m_read = [](s32s::offs_t address) { return s32_pcm_sample_read(address); };
		stream().m_before_change = [this]() { catch_up(current_time()); };
	}

	void catch_up(uint64_t time)
	{
		const uint64_t due = (time + kPcmPeriod - 1) / kPcmPeriod;
		if (due <= m_samples)
			return;
		const int count = int(due - m_samples);
		generate(count);
		for (int index = 0; index < count; index++)
		{
			g_pcm_samples.data.push_back(stream().m_outputs[0][size_t(index)]);
			g_pcm_samples.data.push_back(stream().m_outputs[1][size_t(index)]);
		}
		m_samples = due;
	}

private:
	uint64_t m_samples = 0;
};

std::unique_ptr<pcm_chip> g_pcm;

uint8_t fm_read(int offset) { return g_fm->read(offset); }
void fm_write(int offset, uint8_t data) { g_fm->write(offset, data); }
uint8_t pcm_read(int offset) { (void)offset; return g_pcm->read(); }
void pcm_write(int offset, uint8_t data) { log_write("pcm", offset, data); g_pcm->write(s32s::offs_t(offset), data); }

// ---- running ----

void on_sound_irq(bool asserted, uint8_t vector)
{
	g_vector = vector;
	g_z80->set_input_line(s32s::INPUT_LINE_IRQ0, asserted ? s32s::ASSERT_LINE : s32s::CLEAR_LINE);
}

void on_sound_reset(bool held)
{
	if (held && !g_z80_held)
		g_z80->reset();
	g_z80_held = held;
}

// the Z80 and the YM3438 timers, from now up to 'until'
void on_time_passes(uint64_t until)
{
	while (g_time < until)
	{
		const uint64_t stop = std::min(until, g_fm->next_timer());

		if (stop > g_time)
		{
			if (g_z80_held)
			{
				g_time = stop;
				g_z80_time = std::max(g_z80_time, z80_time(stop));
			}
			else
			{
				const uint64_t target = z80_time(stop);
				if (target > g_z80_time)
				{
					g_slice_z80 = g_z80_time;
					g_slice_budget = int(std::min<uint64_t>(target - g_z80_time, 1u << 24));
					g_running = true;
					const int done = g_z80->run_slice(g_slice_budget);
					g_running = false;
					g_z80_cycles += uint64_t(done);
					g_z80_time += uint64_t(done);
				}
				g_time = std::max(g_time, board_time(g_z80_time));
			}
		}
		g_fm->fire(g_time);
	}
}

uint64_t hash_bytes(const void *data, size_t size, uint64_t hash)
{
	const uint8_t *bytes = static_cast<const uint8_t *>(data);
	for (size_t index = 0; index < size; index++)
		hash = (hash ^ bytes[index]) * 0x100000001b3ull;
	return hash;
}

int16_t clamp16(double value)
{
	return int16_t(std::max(-32768.0, std::min(32767.0, value)));
}

// the samples of a chip before sample number 'due': hashed and taken out
uint64_t take(samples &chip, uint64_t due, uint64_t hash, std::vector<int32_t> &out)
{
	const size_t count = size_t(std::min<uint64_t>(due - chip.first, chip.data.size() / 2)) * 2;
	out.assign(chip.data.begin(), chip.data.begin() + ptrdiff_t(count));
	chip.data.erase(chip.data.begin(), chip.data.begin() + ptrdiff_t(count));
	chip.first += count / 2;
	return hash_bytes(out.data(), out.size() * sizeof(int32_t), hash);
}

}    // namespace

extern "C" void s32s_attach(void)
{
	g_z80 = std::make_unique<sound_cpu>(g_config);
	g_z80->space(s32s::AS_PROGRAM).read = [](uint32_t address) { return s32_z80_read(uint16_t(address)); };
	g_z80->space(s32s::AS_PROGRAM).write = [](uint32_t address, uint8_t data) { s32_z80_write(uint16_t(address), data); };
	g_z80->space(s32s::AS_IO).read = [](uint32_t port) { return s32_z80_in(uint8_t(port)); };
	g_z80->space(s32s::AS_IO).write = [](uint32_t port, uint8_t data)
	{
		// the MultiPCM's samples up to now come from the banks as they were
		if ((port & 0xf0) == 0xb0)
		{
			g_pcm->catch_up(current_time());
			log_write("pcm_bank", int(port & 0x0f), data);
		}
		s32_z80_out(uint8_t(port), data);
	};
	g_z80->irq_vector = []() { return int(g_vector); };
	g_z80->instruction_hook = [](uint32_t address)
	{
		g_executed[(address & 0xffff) >> 3] |= uint8_t(1u << (address & 7));
	};
	g_z80->start();
	g_z80->reset();
	g_fm = std::make_unique<fm_chip>();
	g_pcm = std::make_unique<pcm_chip>(g_config);
	g_pcm->start();
	g_pcm->reset();

	s32_board.chips.fm_read = fm_read;
	s32_board.chips.fm_write = fm_write;
	s32_board.chips.pcm_read = pcm_read;
	s32_board.chips.pcm_write = pcm_write;
	s32_board.sound_irq_changed = on_sound_irq;
	s32_board.sound_reset_changed = on_sound_reset;
	s32_board.time_passes = on_time_passes;
	g_time = s32_board.cycles;
	g_z80_time = z80_time(g_time);
}

extern "C" void s32s_end_frame(uint64_t time)
{
	on_time_passes(time);
	g_fm->catch_up(time);
	g_pcm->catch_up(time);

	// the samples that start before the frame's end, whatever the chips made beyond it already
	std::vector<int32_t> fm, pcm;
	uint64_t hash = 0xcbf29ce484222325ull;
	hash = take(g_fm_samples, (time + kFmPeriod - 1) / kFmPeriod, hash, fm);
	g_frame_samples_hash = take(g_pcm_samples, (time + kPcmPeriod - 1) / kPcmPeriod, hash, pcm);

	// the mix at the YM3438's rate: the MultiPCM's latest sample at each of its samples' start; each
	// chip's second output goes to the left speaker, its first to the right one (MAME's routes)
	const size_t frames = fm.size() / 2;
	const size_t pcm_frames = pcm.size() / 2;
	g_mix.resize(frames * 2);
	for (size_t index = 0; index < frames; index++)
	{
		const size_t at = pcm_frames ? std::min(pcm_frames - 1, index * pcm_frames / std::max<size_t>(frames, 1)) : 0;
		for (int side = 0; side < 2; side++)
		{
			const int output = 1 - side;
			const double fm_part = 0.15 * fm[index * 2 + size_t(output)];
			const double pcm_part = pcm_frames ? 0.35 * pcm[at * 2 + size_t(output)] : 0.0;
			g_mix[index * 2 + size_t(side)] = clamp16(fm_part + pcm_part);
		}
	}
}

extern "C" uint64_t s32s_frame_samples_hash(void)
{
	return g_frame_samples_hash;
}

extern "C" uint64_t s32s_state_hash(void)
{
	return hash_bytes(s32_board.shared_ram, sizeof(s32_board.shared_ram), 0xcbf29ce484222325ull);
}

extern "C" const int16_t *s32s_frame_mix(size_t *frames)
{
	if (frames)
		*frames = g_mix.size() / 2;
	return g_mix.data();
}

extern "C" uint32_t s32s_mix_rate(void)
{
	return S32_SOUND_CLOCK / 144;
}

extern "C" void s32s_log_writes(FILE *file)
{
	g_write_log = file;
}

extern "C" uint64_t s32s_z80_cycles(void)
{
	return g_z80_cycles;
}

extern "C" const uint8_t *s32s_executed(void)
{
	return g_executed;
}
