/**
 *    Name:        pcmcheck.cpp
 *    Description: the game's MultiPCM against MAME's, sample for sample
 *    Version:     0.1.0
 *    Created:     21-10-2026
 *    Modified:    21-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A development tool, never part of the game. It replays the MultiPCM writes and bank changes s32ref
 *    logs (--sound-log) through two chips at once, MAME's (sound/multipcm.cpp and gew.cpp, built here as
 *    s32sound.cpp builds them) and the game's own (runtime/src/multipcm.c), both reading the sample ROM
 *    through the board's banks, and compares every sample of both outputs. Time is s32sound's: sample n
 *    starts at n x 448 board cycles, and before a write or a bank change at time t both chips make every
 *    sample that starts before t.
 *
 *    The synthetic runs drive both chips with random sequences instead (voices, registers, keys, levels
 *    that glide, LFOs, banks), for what the game never does.
 *
 *    Usage: pcmcheck [--synthetic SEEDS] [--steps N] [LOG ...]
 */
#include "s32/machine.h"
#include "s32/multipcm.h"

#include "game/data.h"

// (every standard header MAME's files use comes before their namespace, as in s32sound.cpp)
#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// MAME's MultiPCM, in a namespace of its own over the stand-ins of this folder, as s32sound.cpp has it
namespace mame
{
#include "emu.h"
#include "logmacro.h"
#include "../mame/sound/gew.cpp"
#include "../mame/sound/multipcm.cpp"
}

namespace
{

constexpr uint64_t kPcmPeriod = 224ull * S32_CPU_CLOCK / S32_MULTIPCM_CLOCK;    // board cycles a sample takes

mame::machine_config g_config;
bool g_failed = false;

void fail(const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	std::vfprintf(stdout, format, arguments);
	va_end(arguments);
	std::printf("\n");
	g_failed = true;
}

// MAME's chip, reading the ROM through the board's banks
class reference_pcm : public mame::multipcm_device
{
public:
	reference_pcm() : mame::multipcm_device(g_config, "multipcm", nullptr, S32_MULTIPCM_CLOCK)
	{
		m_read = [](mame::offs_t address) { return s32_pcm_sample_read(address); };
	}
};

// both chips, and how many samples they have made
struct pcm_pair
{
	std::unique_ptr<reference_pcm> reference;
	multipcm own;
	uint64_t samples = 0;
	uint64_t compared = 0;
	uint64_t not_silent = 0;
	uint64_t writes = 0;
	uint64_t key_ons = 0;

	pcm_pair()
	{
		reference = std::make_unique<reference_pcm>();
		reference->start();
		reference->reset();
		multipcm_init(&own, S32_MULTIPCM_CLOCK, s32_pcm_sample_read);
		multipcm_reset(&own);
	}
};

// every sample that starts before 'time', from both chips: false at the first that differs
bool catch_up(pcm_pair &pair, uint64_t time)
{
	const uint64_t due = (time + kPcmPeriod - 1) / kPcmPeriod;
	if (due <= pair.samples)
		return true;
	const int count = int(due - pair.samples);
	std::vector<int32_t> left(static_cast<size_t>(count)), right(static_cast<size_t>(count));
	pair.reference->generate(count);
	multipcm_generate(&pair.own, left.data(), right.data(), count);
	for (int index = 0; index < count; index++)
	{
		const int32_t theirs_left = pair.reference->stream().m_outputs[0][size_t(index)];
		const int32_t theirs_right = pair.reference->stream().m_outputs[1][size_t(index)];
		if (theirs_left != left[size_t(index)] || theirs_right != right[size_t(index)])
		{
			fail("sample %" PRIu64 " (time %" PRIu64 "): MAME %d %d, ours %d %d", pair.samples + uint64_t(index),
				(pair.samples + uint64_t(index)) * kPcmPeriod, theirs_left, theirs_right, left[size_t(index)],
				right[size_t(index)]);
			return false;
		}
		pair.not_silent += theirs_left != 0 || theirs_right != 0;
	}
	pair.samples = due;
	pair.compared += uint64_t(count);
	return true;
}

bool write(pcm_pair &pair, uint64_t time, int offset, uint8_t data)
{
	if (!catch_up(pair, time))
		return false;
	pair.reference->write(mame::offs_t(offset), data);
	multipcm_write(&pair.own, offset, data);
	pair.writes++;
	return true;
}

bool set_bank(pcm_pair &pair, uint64_t time, uint8_t data)
{
	if (!catch_up(pair, time))
		return false;
	s32_board.pcm_bank = data;
	return true;
}

// one log of s32ref --sound-log: "TIME CHIP OFFSET DATA", the MultiPCM's lines and its bank changes
bool replay(const char *path)
{
	std::ifstream log(path);
	if (!log)
	{
		std::printf("%s: cannot read\n", path);
		return false;
	}
	pcm_pair pair;
	s32_board.pcm_bank = 0;
	std::string line;
	uint64_t last = 0;
	while (std::getline(log, line))
	{
		std::istringstream words(line);
		uint64_t time = 0;
		std::string chip;
		unsigned offset = 0;
		unsigned data = 0;
		if (!(words >> time >> chip >> std::hex >> offset >> data))
			continue;
		last = time;
		if (chip == "pcm")
		{
			pair.key_ons += (offset == 0 && (data & 0x80) != 0);
			if (!write(pair, time, int(offset), uint8_t(data)))
				return false;
		}
		else if (chip == "pcm_bank")
		{
			if (!set_bank(pair, time, uint8_t(data)))
				return false;
		}
	}
	// and a second more, for the voices still playing
	if (!catch_up(pair, last + S32_CPU_CLOCK))
		return false;
	std::printf("%s: %" PRIu64 " samples the same (%" PRIu64 " not silent), %" PRIu64 " writes\n", path, pair.compared,
		pair.not_silent, pair.writes);
	return true;
}

// a random run: voices picked and played with every register, the banks changed now and then
bool synthetic(uint32_t seed, int steps)
{
	pcm_pair pair;
	uint64_t state = seed * 0x9e3779b97f4a7c15ull + 1;
	auto random = [&state](uint32_t range)
	{
		state ^= state << 13;
		state ^= state >> 7;
		state ^= state << 17;
		return uint32_t(state % range);
	};
	uint64_t time = 0;
	s32_board.pcm_bank = 0;
	for (int step = 0; step < steps; step++)
	{
		time += random(4) == 0 ? random(40000) : random(2000);
		switch (random(10))
		{
			case 0:
				if (!set_bank(pair, time, uint8_t(random(256))))
					return false;
				break;
			case 1:
			{
				// a voice: every eighth number picks none, which MAME's chip cannot take
				uint32_t voice = random(32);
				if ((voice & 7) == 7)
					voice--;
				if (!write(pair, time, 1, uint8_t(voice)))
					return false;
				break;
			}
			default:
			{
				const uint32_t reg = random(12);
				uint8_t data = uint8_t(random(256));
				if (reg == 1)
					data = uint8_t(random(16) == 0 ? random(256) : random(48));    // samples the ROM has headers for
				if (!write(pair, time, 2, uint8_t(reg)) || !write(pair, time, 0, data))
					return false;
				break;
			}
		}
	}
	if (!catch_up(pair, time + S32_CPU_CLOCK))
		return false;
	std::printf("seed %u: %" PRIu64 " samples the same (%" PRIu64 " not silent), %" PRIu64 " writes\n", seed,
		pair.compared, pair.not_silent, pair.writes);
	return true;
}

}    // namespace

int main(int argc, char **argv)
{
	int seeds = 0;
	int steps = 20000;
	std::vector<const char *> logs;
	for (int index = 1; index < argc; index++)
	{
		if (std::strcmp(argv[index], "--synthetic") == 0 && index + 1 < argc)
			seeds = std::atoi(argv[++index]);
		else if (std::strcmp(argv[index], "--steps") == 0 && index + 1 < argc)
			steps = std::atoi(argv[++index]);
		else if (argv[index][0] == '-')
		{
			std::fprintf(stderr, "Usage: pcmcheck [--synthetic SEEDS] [--steps N] [LOG ...]\n");
			return 1;
		}
		else
			logs.push_back(argv[index]);
	}
	s32_board.pcm_rom = game_pcm_rom;
	for (const char *path : logs)
		if (!replay(path))
			break;
	for (int seed = 1; seed <= seeds && !g_failed; seed++)
		synthetic(uint32_t(seed), steps);
	if (g_failed)
		return 1;
	std::printf("the same\n");
	return 0;
}
