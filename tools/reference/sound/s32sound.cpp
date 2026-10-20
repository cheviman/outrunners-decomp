/**
 *    Name:        s32sound.cpp
 *    Description: reference System 32 sound board: MAME's Z80 and RF5C68 and ymfm's YM3438 on the board model
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    MAME's files are compiled here inside a namespace of their own, over the stand-ins of this folder
 *    (emu.h, machine/z80daisy.h, logmacro.h), so that their names keep away from the V60 core's
 *    stand-ins in the same program. ymfm is a library of its own and builds as it is.
 *
 *    Two ways to keep the Z80's time:
 *      - free: the Z80 runs cycle by cycle in slices that end at the YM3438 timers (a timer set while it
 *        runs cuts its slice short), and takes interrupts as MAME's core does;
 *      - the timing model (s32s_use_timing, the table tools/decompile_sound.py --timing writes): the rules
 *        of runtime/src/sound_cpu.c, kept around MAME's core by its instruction hook. The time is the
 *        model's: every block of the table adds its cycles when it starts, and a condition's longer way
 *        its extra ones; the timers run out and interrupts are taken only where a block starts (this
 *        code pushes the address and jumps to 0038 itself: the core never sees the line); every access
 *        happens at its block's start; the board stops the Z80 where the main loop's top finds its time
 *        past the board's. The game's own sound board keeps the same rules, so the two compare sample for
 *        sample.
 */
#include "s32sound.h"

#include "s32/machine.h"

#include "ymfm_opn.h"

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
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
#include "../mame/sound/rf5c68.cpp"
}

namespace
{

constexpr uint64_t kFmPeriod = 288;                                 // board cycles a YM3438 sample takes
constexpr uint64_t kPcmNumerator = uint64_t(384) * S32_CPU_CLOCK;   // board cycles x S32_PCM_CLOCK a sample takes
constexpr int kInterruptCycles = 13;
constexpr uint32_t kStackStart = 0xFE00;                            // the Z80's stack, FE00-FEFF: not in the hash
constexpr uint32_t kStackEnd = 0xFF00;

// time: the board's, and while the Z80 runs, how far into its slice it is
uint64_t g_time = 0;
bool g_running = false;
uint64_t g_slice_start = 0;
int g_slice_budget = 0;
uint64_t g_now_override = UINT64_MAX;       // while a timer fires, its own time

// ---- the timing model ----

struct block_timing
{
	int cycles;
	uint16_t last;          // the block's last instruction
	uint8_t length;         // its length
	int extra;              // its condition's longer way
};

struct
{
	bool on = false;
	std::map<uint16_t, block_timing> blocks;
	std::set<uint16_t> repeats;
	uint16_t loop = 0;
	uint16_t irq = 0;
	uint64_t clock = 0;             // where the next block starts
	uint64_t access = 0;            // when the current block's accesses happen
	uint64_t until = 0;             // where the board stops the Z80
	bool stop = false;              // the hook stopped the core at the loop's top
	bool rewind = false;            // the core fetched the loop top's opcode before it stopped
	bool line = false;              // the board's interrupt line
	int previous = -1;              // the instruction before this one
	uint8_t previous_opcode = 0;
	const block_timing *current = nullptr;
} g_model;

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

	uint16_t pc() const { return m_pc.w; }

	// the timing model takes the interrupt itself: the return address on the stack, IFF1 and IFF2 off,
	// on to the handler (interrupt mode 1)
	void interrupt_to(uint16_t handler)
	{
		m_sp.w = uint16_t(m_sp.w - 1);
		s32_z80_write(m_sp.w, m_pc.b.h);
		m_sp.w = uint16_t(m_sp.w - 1);
		s32_z80_write(m_sp.w, m_pc.b.l);
		m_pc.w = handler;
		m_iff1 = m_iff2 = false;
	}

	bool interrupts_on() const { return m_iff1; }

	// back to the start of the instruction whose opcode the core fetched before the slice ended
	void restart_at(uint16_t address)
	{
		m_pc.w = address;
		m_ref = 0xffff00;
		m_r = uint8_t(m_r - 1);
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
	return g_slice_start + 2 * uint64_t(g_z80->cycles_done());
}

uint64_t current_time()
{
	if (g_now_override != UINT64_MAX)
		return g_now_override;
	if (g_model.on)
		return g_running ? g_model.access : g_model.clock;
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
samples g_fm_samples[2];
samples g_pcm_samples;
std::vector<int16_t> g_mix;
uint64_t g_frame_samples_hash = 0;

void log_write(const char *chip, int offset, uint8_t data)
{
	if (g_write_log)
		std::fprintf(g_write_log, "%llu %s %X %02X\n", (unsigned long long)current_time(), chip, offset, data);
}

// ---- the YM3438s ----

void fire_timers(uint64_t time);

class fm_chip : public ymfm::ymfm_interface
{
public:
	explicit fm_chip(int number) : m_number(number), m_chip(*this) { m_chip.reset(); }

	// every sample that starts before 'time'
	void catch_up(uint64_t time)
	{
		const uint64_t due = (time + kFmPeriod - 1) / kFmPeriod;
		ymfm::ym3438::output_data output;
		while (m_samples < due)
		{
			m_chip.generate(&output);
			g_fm_samples[m_number].data.push_back(output.data[0]);
			g_fm_samples[m_number].data.push_back(output.data[1]);
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
		log_write(m_number ? "fm1" : "fm0", offset, data);
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
		m_deadline[number] = current_time() + 2 * uint64_t(clocks);
		// free timing: a running Z80 stops by then, for the interrupt to come on time
		if (!g_model.on && g_running && g_now_override == UINT64_MAX
			&& m_deadline[number] < g_slice_start + 2 * uint64_t(g_slice_budget))
		{
			const uint64_t at = running_time();
			g_slice_budget -= g_z80->cut_slice(m_deadline[number] > at ? int((m_deadline[number] - at + 1) / 2) : 0);
		}
	}

	void ymfm_set_busy_end(uint32_t clocks) override
	{
		m_busy_until = current_time() + 2 * uint64_t(clocks);
	}

	bool ymfm_is_busy() override
	{
		return current_time() < m_busy_until;
	}

	void ymfm_update_irq(bool asserted) override
	{
		// only the first chip's interrupt is wired
		if (m_number != 0)
			return;
		if (asserted)
			s32_sound_signal_irq(S32_SOUND_IRQ_FM);
		else
			s32_sound_clear_irq(S32_SOUND_IRQ_FM);
	}

private:
	int m_number;
	ymfm::ym3438 m_chip;
	uint64_t m_samples = 0;
	uint64_t m_deadline[2] = { UINT64_MAX, UINT64_MAX };
	uint64_t m_busy_until = 0;
};

std::unique_ptr<fm_chip> g_fm[2];

void fire_timers(uint64_t time)
{
	// both chips' timers in the order they run out
	while (std::min(g_fm[0]->next_timer(), g_fm[1]->next_timer()) <= time)
	{
		const int chip = g_fm[0]->next_timer() <= g_fm[1]->next_timer() ? 0 : 1;
		g_fm[chip]->fire(g_fm[chip]->next_timer());
	}
}

// ---- the RF5C68 ----

class pcm_chip : public s32s::rf5c68_device
{
public:
	explicit pcm_chip(const s32s::machine_config &config) : s32s::rf5c68_device(config, "rfsnd", nullptr, S32_PCM_CLOCK)
	{
		space(0).read = [](uint32_t address) { return s32_board.pcm_ram[address & 0xffff]; };
		space(0).write = [](uint32_t address, uint8_t data) { s32_board.pcm_ram[address & 0xffff] = data; };
		stream().m_before_change = [this]() { catch_up(current_time()); };
	}

	void catch_up(uint64_t time)
	{
		const uint64_t due = (time * S32_PCM_CLOCK + kPcmNumerator - 1) / kPcmNumerator;
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

uint8_t fm_read(int chip, int offset) { return g_fm[chip]->read(offset); }
void fm_write(int chip, int offset, uint8_t data) { g_fm[chip]->write(offset, data); }
void pcm_write(int offset, uint8_t data) { log_write("pcm", offset, data); g_pcm->rf5c68_w(uint32_t(offset), data); }
uint8_t pcm_memory_read(int offset) { return g_pcm->rf5c68_mem_r(uint32_t(offset)); }
void pcm_memory_write(int offset, uint8_t data) { log_write("pcm_memory", offset, data); g_pcm->rf5c68_mem_w(uint32_t(offset), data); }

// ---- the timing model's hook ----

// a block starts at 'pc': the timers due, an interrupt if the Z80 takes one, then the block's time
void start_block(uint16_t pc)
{
	for (;;)
	{
		fire_timers(g_model.clock);
		if (!g_z80->interrupts_on() || g_model.previous_opcode == 0xFB || !g_model.line)
			break;
		// the interrupt: its cycles, then the handler's first block starts
		g_z80->interrupt_to(g_model.irq);
		pc = g_model.irq;
		g_model.clock += 2 * kInterruptCycles;
		g_model.previous_opcode = 0;
	}
	const auto found = g_model.blocks.find(pc);
	g_model.current = &found->second;
	g_model.access = g_model.clock;
	g_model.clock += 2 * uint64_t(found->second.cycles);
}

void on_instruction(uint32_t address)
{
	const uint16_t pc = uint16_t(address);
	g_executed[pc >> 3] |= uint8_t(1u << (pc & 7));
	if (!g_model.on)
		return;
	const int previous = g_model.previous;
	g_model.previous = pc;
	if (previous == pc && g_model.repeats.count(pc))
	{
		g_model.clock += 2 * 21;    // another round of a block move
		return;
	}
	if (g_model.blocks.count(pc))
	{
		// the way the last block left: its condition's longer way adds its extra cycles
		const block_timing *last = g_model.current;
		if (last != nullptr && previous == last->last && pc != uint16_t(last->last + last->length))
			g_model.clock += 2 * uint64_t(last->extra);
		// (coming back there from an interrupt is not coming to the top: the C took the interrupt where
		// the loop's first block starts)
		const bool from_interrupt = previous >= 0 && s32_z80_read(uint16_t(previous)) == 0xED
			&& s32_z80_read(uint16_t(previous + 1)) == 0x4D;
		if (pc == g_model.loop && g_model.clock >= g_model.until && !from_interrupt)
		{
			// the main loop's top with the board's time reached: the board takes over, and when the Z80
			// goes on, the loop's top starts its block (its way in is counted already)
			g_model.stop = true;
			g_model.rewind = true;
			g_model.previous = previous;
			g_model.current = nullptr;
			g_slice_budget -= g_z80->cut_slice(0);
			return;
		}
		start_block(pc);
	}
	g_model.previous_opcode = s32_z80_read(pc);
}

// ---- running ----

void on_sound_irq(bool asserted, uint8_t vector)
{
	g_vector = vector;
	if (g_model.on)
		g_model.line = asserted;
	else
		g_z80->set_input_line(s32s::INPUT_LINE_IRQ0, asserted ? s32s::ASSERT_LINE : s32s::CLEAR_LINE);
}

void on_sound_reset(bool held)
{
	if (held && !g_z80_held)
	{
		g_z80->reset();
		g_model.rewind = false;
		g_model.previous = -1;
		g_model.previous_opcode = 0;
		g_model.current = nullptr;
	}
	g_z80_held = held;
}

// the timing model: the Z80 up to the main loop's top past 'until'
void run_model(uint64_t until)
{
	if (g_z80_held)
	{
		fire_timers(until);
		g_model.clock = std::max(g_model.clock, until);
		return;
	}
	if (g_model.rewind)
	{
		if (g_model.clock >= until)
			return;
		g_z80->restart_at(g_model.loop);
		g_model.rewind = false;
	}
	g_model.until = until;
	g_model.stop = false;
	while (!g_model.stop)
	{
		g_slice_start = g_model.clock;
		g_slice_budget = 1 << 20;
		g_running = true;
		const int done = g_z80->run_slice(g_slice_budget);
		g_running = false;
		g_z80_cycles += uint64_t(done);
	}
}

// the Z80 and the YM3438 timers, from now up to 'until'
void on_time_passes(uint64_t until)
{
	if (g_model.on)
	{
		run_model(until);
		g_time = std::max(g_time, until);
		return;
	}
	while (g_time < until)
	{
		const uint64_t stop = std::min({ until, g_fm[0]->next_timer(), g_fm[1]->next_timer() });

		if (stop > g_time)
		{
			if (g_z80_held)
				g_time = stop;
			else
			{
				g_slice_start = g_time;
				g_slice_budget = int((stop - g_time + 1) / 2);
				g_running = true;
				const int done = g_z80->run_slice(g_slice_budget);
				g_running = false;
				g_z80_cycles += uint64_t(done);
				g_time += 2 * uint64_t(done);
			}
		}
		fire_timers(g_time);
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
	g_z80->space(s32s::AS_IO).write = [](uint32_t port, uint8_t data) { s32_z80_out(uint8_t(port), data); };
	g_z80->irq_vector = []() { return int(g_vector); };
	g_z80->instruction_hook = on_instruction;
	g_z80->start();
	g_z80->reset();
	g_fm[0] = std::make_unique<fm_chip>(0);
	g_fm[1] = std::make_unique<fm_chip>(1);
	g_pcm = std::make_unique<pcm_chip>(g_config);
	g_pcm->start();

	s32_board.chips.fm_read = fm_read;
	s32_board.chips.fm_write = fm_write;
	s32_board.chips.pcm_write = pcm_write;
	s32_board.chips.pcm_memory_read = pcm_memory_read;
	s32_board.chips.pcm_memory_write = pcm_memory_write;
	s32_board.sound_irq_changed = on_sound_irq;
	s32_board.sound_reset_changed = on_sound_reset;
	s32_board.time_passes = on_time_passes;
	g_time = g_model.clock = s32_board.cycles;
}

extern "C" bool s32s_use_timing(const char *path)
{
	std::ifstream file(path);
	if (!file)
		return false;
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream words(line);
		std::string kind;
		words >> kind;
		unsigned address = 0;
		if (kind == "loop")
			words >> std::hex >> address, g_model.loop = uint16_t(address);
		else if (kind == "irq")
			words >> std::hex >> address, g_model.irq = uint16_t(address);
		else if (kind == "repeat")
			words >> std::hex >> address, g_model.repeats.insert(uint16_t(address));
		else if (kind == "block")
		{
			block_timing block{};
			unsigned last = 0, length = 0;
			words >> std::hex >> address >> std::dec >> block.cycles >> std::hex >> last >> std::dec >> length >> block.extra;
			block.last = uint16_t(last);
			block.length = uint8_t(length);
			g_model.blocks[uint16_t(address)] = block;
		}
	}
	g_model.on = !g_model.blocks.empty();
	return g_model.on;
}

extern "C" void s32s_end_frame(uint64_t time)
{
	on_time_passes(time);
	g_fm[0]->catch_up(time);
	g_fm[1]->catch_up(time);
	g_pcm->catch_up(time);

	// the samples that start before the frame's end, whatever the chips made beyond it already
	std::vector<int32_t> fm[2], pcm;
	uint64_t hash = 0xcbf29ce484222325ull;
	hash = take(g_fm_samples[0], (time + kFmPeriod - 1) / kFmPeriod, hash, fm[0]);
	hash = take(g_fm_samples[1], (time + kFmPeriod - 1) / kFmPeriod, hash, fm[1]);
	g_frame_samples_hash = take(g_pcm_samples, (time * S32_PCM_CLOCK + kPcmNumerator - 1) / kPcmNumerator, hash, pcm);

	// the mix at the YM3438's rate: the RF5C68's latest sample at each of its samples' start
	const size_t frames = std::min(fm[0].size(), fm[1].size()) / 2;
	const size_t pcm_frames = pcm.size() / 2;
	g_mix.resize(frames * 2);
	for (size_t index = 0; index < frames; index++)
	{
		const size_t at = pcm_frames ? std::min(pcm_frames - 1, index * pcm_frames / std::max<size_t>(frames, 1)) : 0;
		for (int side = 0; side < 2; side++)
		{
			const double fm_sum = 0.30 * (fm[0][index * 2 + side] + fm[1][index * 2 + side]);
			const double rf = pcm_frames ? 0.40 * pcm[at * 2 + side] : 0.0;
			g_mix[index * 2 + side] = clamp16(fm_sum + rf);
		}
	}
}

extern "C" uint64_t s32s_frame_samples_hash(void)
{
	return g_frame_samples_hash;
}

extern "C" uint64_t s32s_state_hash(void)
{
	uint64_t hash = 0xcbf29ce484222325ull;
	hash = hash_bytes(s32_board.shared_ram, kStackStart - 0xE000, hash);
	hash = hash_bytes(s32_board.shared_ram + (kStackEnd - 0xE000), 0x10000 - kStackEnd, hash);
	return hash_bytes(s32_board.pcm_ram, sizeof(s32_board.pcm_ram), hash);
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
