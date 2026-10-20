/**
 *    Name:        s32ref.cpp
 *    Description: development reference: runs the original V70 program on MAME's V70 core against the
 *                 runtime's Multi 32 model, to find code, log hardware use and record state per frame
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    Not part of the game. Usage:
 *      s32ref [--timing frames|cycles] [--frames N] [--script controls.txt]
 *                           [--hashes hashes.txt] [--trace executed.txt] [--calls called.txt]
 *                           [--transfers transfers.txt] [--watch ADDRESS ...] [--profile profile.txt]
 *                           [--dump-frame N ... --dump-dir DIR] [--video-hashes hashes.txt]
 *                           [--video-dump N ... --video-dir DIR] [--sound-hashes hashes.txt]
 *                           [--sound-log writes.txt] [--sound-wav sound.wav] [--sound-trace executed.txt]
 *                           [--verbose]
 *
 *    The program and the other ROMs are the game's data the executable carries (game/data.h).
 *
 *    --timing frames, the default, runs the program the way the decompiled game runs: instructions take
 *    no time, and the board moves on to its next event only while the game waits for the next frame in
 *    its frame loop; a frame ends when that wait is over. --timing cycles keeps MAME's pace, 8 cycles an
 *    instruction, and a frame ends 50 us after VBLANK, when the sprite list goes out. While it waits, the
 *    program may unpack data a piece at a time (000908 and 0008F0-000906); with frame timing that work
 *    takes no time either, so what it unpacks is ready within the frame that asked for it.
 *
 *    --video-hashes and --video-dump draw every frame of both screens with MAME's Multi 32 video
 *    (video/s32video.cpp): the hash of each frame, and frames as BMP, the left screen beside the right.
 *
 *    --sound-hashes, --sound-log and --sound-wav run the sound board too (sound/s32sound.cpp: MAME's Z80
 *    and MultiPCM, ymfm's YM3438): for each frame the hash of the samples the chips made and of the sound
 *    board's memory; every chip write; the sound mixed. --sound-trace lists the Z80 addresses executed.
 *    Without them the Z80 stays in reset, and the program sees the shared RAM as it leaves it.
 */
#include "emu.h"
#include "v60.h"

#include "game/data.h"
#include "s32/machine.h"
#include "s32/script.h"
#include "s32sound.h"
#include "s32video.h"

#include <algorithm>
#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace {

// The game's frame loop (000908): it polls 23[R24] until an interrupt sets it, then clears it
constexpr uint32_t kFrameWaitLoop = 0x000908;
constexpr uint32_t kFrameWaitDone = 0x00090D;
constexpr uint32_t kFrameReady = 0x20E023;
// With frame timing, a program that stops reaching its frame loop has stopped making frames
constexpr uint64_t kInstructionsPerFrameLimit = 500000000;    // the memory test runs some 80 million at once

v70_device *g_cpu = nullptr;
bool g_line_wanted = false;
bool g_line_applied = false;
uint64_t g_slice_start = 0;
int g_slice_budget = 0;
uint64_t g_instructions = 0;
uint64_t g_interrupts = 0;
std::vector<uint8_t> g_executed(1 << 21);    // one bit per address of the 24-bit bus
std::vector<uint8_t> g_called(1 << 21);      // addresses reached by jsr, bsr or call
bool g_call_pending = false;                 // the previous instruction was a call
bool g_verbose = false;
s32_script *g_script = nullptr;              // --script: controls applied when each VBLANK starts

// Frames and what is recorded at the end of each
uint64_t g_frames_wanted = 600;
uint64_t g_frames_done = 0;
uint64_t g_instructions_at_frame = 0;
bool g_stop = false;
std::ofstream g_hashes;
std::set<uint64_t> g_dump_frames;            // with several, each one goes to DIR/frame_N
std::string g_dump_dir;
std::ofstream g_video_hashes;                 // --video-hashes: the hash of every frame MAME's video draws
std::set<uint64_t> g_video_dump_frames;      // --video-dump: frames written as BMP to g_video_dir
std::string g_video_dir;
bool g_video = false;                        // the reference video is attached
bool g_sound = false;                        // the sound board runs
std::ofstream g_sound_hashes;                 // --sound-hashes: frame, samples hash, sound memory hash
std::vector<int16_t> g_sound_mix;            // --sound-wav: everything the chips made, mixed
std::vector<uint32_t> g_watched;             // bytes whose changes are printed at the end of every frame
std::vector<uint8_t> g_watched_values;

// --transfers: where every jmp, jsr, call and return went, as (from << 24 | to)
std::set<uint64_t> g_transfers;
std::set<uint64_t> g_code_writes;            // 32-bit writes of program ROM addresses, as (value << 24 | writer)
bool g_record_transfers = false;
bool g_transfer_pending = false;             // the previous instruction was one of those
uint32_t g_transfer_from = 0;
bool g_interrupt_entry = false;              // the next instruction is the first one of an interrupt handler

bool is_transfer(uint8_t opcode)
{
	switch (opcode)
	{
		case 0x49:                  // call
		case 0xca:                  // rsr
		case 0xd6: case 0xd7:       // jmp
		case 0xe2: case 0xe3:       // ret
		case 0xe8: case 0xe9:       // jsr
		case 0xea: case 0xeb:       // retiu
		case 0xfa: case 0xfb:       // retis
			return true;
		default:
			return false;
	}
}

// --profile: how often each instruction runs and each non-ROM address is read
std::vector<uint32_t> g_pc_counts;
std::vector<uint32_t> g_read_counts;
uint64_t g_region_reads[16];
uint64_t g_region_writes[16];

inline void count_read(offs_t address)
{
	if (g_read_counts.empty())
		return;
	g_region_reads[(address >> 20) & 0xf]++;
	if ((address >> 20) >= 2 && (address >> 20) < 0xf)
		g_read_counts[address & 0xffffff]++;
}

inline void count_write(offs_t address)
{
	if (!g_read_counts.empty())
		g_region_writes[(address >> 20) & 0xf]++;
}

u8 read8(offs_t address) { count_read(address); return s32_read8(address & 0xffffff); }
u16 read16(offs_t address) { count_read(address); return s32_read16(address & 0xffffff); }
u32 read32(offs_t address) { count_read(address); return s32_read32(address & 0xffffff); }
void write8(offs_t address, u8 data) { count_write(address); s32_write8(address & 0xffffff, data); }
void write16(offs_t address, u16 data) { count_write(address); s32_write16(address & 0xffffff, data); }
void write32(offs_t address, u32 data)
{
	count_write(address);
	// with --transfers: who stores addresses of the program ROM (code pointers among them)
	if (g_record_transfers && data < 0x200000)
		g_code_writes.insert(uint64_t(data) << 24 | s32_board.pc);
	s32_write32(address & 0xffffff, data);
}

void write_profile(const std::string &path)
{
	std::ofstream out(path);
	auto top = [](const std::vector<uint32_t> &counts, size_t limit) {
		std::vector<std::pair<uint32_t, uint32_t>> entries;
		for (uint32_t address = 0; address < counts.size(); address++)
			if (counts[address])
				entries.emplace_back(counts[address], address);
		std::sort(entries.rbegin(), entries.rend());
		if (entries.size() > limit)
			entries.resize(limit);
		return entries;
	};
	char line[96];
	out << "Most executed instructions\n";
	for (const auto &[count, address] : top(g_pc_counts, 60))
	{
		std::snprintf(line, sizeof(line), "  %06X  %10u\n", address, count);
		out << line;
	}
	out << "\nMost read addresses outside ROM\n";
	for (const auto &[count, address] : top(g_read_counts, 40))
	{
		std::snprintf(line, sizeof(line), "  %06X  %10u\n", address, count);
		out << line;
	}
	out << "\nAccesses per 1 MB region (reads / writes)\n";
	for (int region = 0; region < 16; region++)
		if (g_region_reads[region] || g_region_writes[region])
		{
			std::snprintf(line, sizeof(line), "  %X00000  %12" PRIu64 " / %12" PRIu64 "\n",
				region, g_region_reads[region], g_region_writes[region]);
			out << line;
		}
}

bool write_file(const std::string &path, const void *data, size_t size)
{
	std::ofstream out(path, std::ios::binary);
	out.write(static_cast<const char *>(data), std::streamsize(size));
	return bool(out);
}

void dump_state(const std::string &dir)
{
	write_file(dir + "/work_ram.bin", s32_board.work_ram, sizeof(s32_board.work_ram));
	write_file(dir + "/video_ram.bin", s32_board.video_ram, sizeof(s32_board.video_ram));
	write_file(dir + "/sprite_ram.bin", s32_board.sprite_ram, sizeof(s32_board.sprite_ram));
	write_file(dir + "/palette.bin", s32_board.palette, sizeof(s32_board.palette));
	write_file(dir + "/mixer.bin", s32_board.mixer, sizeof(s32_board.mixer));
	write_file(dir + "/shared_ram.bin", s32_board.shared_ram, sizeof(s32_board.shared_ram));
	write_file(dir + "/comm_ram.bin", s32_board.comm_ram, sizeof(s32_board.comm_ram));
	write_file(dir + "/eeprom.bin", s32_board.eeprom.data, sizeof(s32_board.eeprom.data));

	std::ofstream registers(dir + "/registers.txt");
	for (int index = V60_R0; index <= V60_PSW; index++)
	{
		char line[64];
		std::snprintf(line, sizeof(line), "%d %08X\n", index - V60_R0, g_cpu->state_value(index));
		registers << line;
	}
}

// Stereo 16-bit samples as a WAV file
void write_wav(const std::string &path, const std::vector<int16_t> &samples, uint32_t rate)
{
	const uint32_t data_size = uint32_t(samples.size() * 2);
	std::vector<uint8_t> file(44 + data_size, 0);
	auto put16 = [&](size_t at, uint32_t value) { file[at] = uint8_t(value); file[at + 1] = uint8_t(value >> 8); };
	auto put32 = [&](size_t at, uint32_t value) { put16(at, value & 0xffff); put16(at + 2, value >> 16); };
	std::memcpy(&file[0], "RIFF", 4);
	put32(4, 36 + data_size);
	std::memcpy(&file[8], "WAVEfmt ", 8);
	put32(16, 16);
	put16(20, 1);
	put16(22, 2);
	put32(24, rate);
	put32(28, rate * 4);
	put16(32, 4);
	put16(34, 16);
	std::memcpy(&file[36], "data", 4);
	put32(40, data_size);
	for (size_t index = 0; index < samples.size(); index++)
		put16(44 + index * 2, uint16_t(samples[index]));
	write_file(path, file.data(), file.size());
}

// A frame drawn by the reference video, as a 24-bit BMP: the left screen, then the right one
void write_bmp(const std::string &path)
{
	int widths[S32V_SCREENS] = {};
	const uint32_t *screens[S32V_SCREENS];
	for (int screen = 0; screen < S32V_SCREENS; screen++)
		screens[screen] = s32v_frame(screen, &widths[screen]);
	const int width = widths[0] + widths[1];
	const int row = (width * 3 + 3) & ~3;
	const uint32_t size = 54 + uint32_t(row) * S32V_HEIGHT;
	std::vector<uint8_t> file(size, 0);
	auto put32 = [&](size_t at, uint32_t value) { for (int byte = 0; byte < 4; byte++) file[at + byte] = uint8_t(value >> (8 * byte)); };
	file[0] = 'B';
	file[1] = 'M';
	put32(2, size);
	put32(10, 54);
	put32(14, 40);
	put32(18, uint32_t(width));
	put32(22, uint32_t(S32V_HEIGHT));
	file[26] = 1;
	file[28] = 24;
	for (int y = 0; y < S32V_HEIGHT; y++)
		for (int x = 0; x < width; x++)
		{
			const int screen = x < widths[0] ? 0 : 1;
			const uint32_t pixel = screens[screen][y * S32V_MAX_WIDTH + (screen ? x - widths[0] : x)];
			uint8_t *out = &file[54 + size_t(S32V_HEIGHT - 1 - y) * row + x * 3];
			out[0] = uint8_t(pixel);
			out[1] = uint8_t(pixel >> 8);
			out[2] = uint8_t(pixel >> 16);
		}
	write_file(path, file.data(), file.size());
}

// The screens are drawn only for the frames the options keep: the hashes need every one, the dumps theirs
void choose_drawing(uint64_t frame)
{
	if (g_video)
		s32v_draw_screens(g_video_hashes.is_open() || g_video_dump_frames.count(frame) != 0);
}

// A frame is over: record what the options ask for
void end_of_frame()
{
	const uint64_t frame = g_frames_done;
	if (g_sound)
	{
		s32s_end_frame(s32_board.cycles);
		if (g_sound_hashes.is_open())
		{
			char line[80];
			std::snprintf(line, sizeof(line), "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", frame,
				s32s_frame_samples_hash(), s32s_state_hash());
			g_sound_hashes << line;
		}
		size_t frames = 0;
		const int16_t *mix = s32s_frame_mix(&frames);
		g_sound_mix.insert(g_sound_mix.end(), mix, mix + frames * 2);
	}
	if (g_video_hashes.is_open())
	{
		char line[64];
		std::snprintf(line, sizeof(line), "%" PRIu64 " %016" PRIx64 "\n", frame, s32v_frame_hash());
		g_video_hashes << line;
	}
	if (g_video_dump_frames.count(frame) && !g_video_dir.empty())
	{
		std::filesystem::create_directories(g_video_dir);
		write_bmp(g_video_dir + "/frame_" + std::to_string(frame) + ".bmp");
	}
	if (g_hashes.is_open())
	{
		char line[64];
		std::snprintf(line, sizeof(line), "%" PRIu64 " %016" PRIx64 "\n", frame, s32_state_hash());
		g_hashes << line;
	}
	if (g_dump_frames.count(frame) && !g_dump_dir.empty())
	{
		std::string dir = g_dump_dir;
		if (g_dump_frames.size() > 1)
			dir += "/frame_" + std::to_string(frame);
		std::filesystem::create_directories(dir);
		dump_state(dir);
	}
	for (size_t index = 0; index < g_watched.size(); index++)
	{
		const uint8_t value = s32_read8(g_watched[index]);
		if (frame == 0 || value != g_watched_values[index])
		{
			char line[64];
			std::snprintf(line, sizeof(line), "frame %" PRIu64 ": %06X = %02X\n", frame, g_watched[index], value);
			std::cout << line;
		}
		g_watched_values[index] = value;
	}
	g_frames_done++;
	choose_drawing(g_frames_done);
	g_instructions_at_frame = g_instructions;
	if (g_frames_done >= g_frames_wanted)
		g_stop = true;
}

void on_irq_changed(bool asserted)
{
	g_line_wanted = asserted;
}

void on_log(const char *message)
{
	std::cerr << "[s32] " << message << '\n';
}

void on_instruction(offs_t pc)
{
	pc &= 0xffffff;
	s32_board.pc = pc;
	g_executed[pc >> 3] |= uint8_t(1u << (pc & 7));
	g_instructions++;
	if (!g_pc_counts.empty())
		g_pc_counts[pc]++;
	if (g_call_pending)
		g_called[pc >> 3] |= uint8_t(1u << (pc & 7));
	const uint8_t opcode = s32_read8(pc);
	g_call_pending = opcode == 0x48 || opcode == 0x49 || opcode == 0xe8 || opcode == 0xe9;
	if (g_record_transfers)
	{
		if (g_transfer_pending && !g_interrupt_entry)
			g_transfers.insert(uint64_t(g_transfer_from) << 24 | pc);
		g_transfer_pending = is_transfer(opcode);
		g_transfer_from = pc;
	}
	g_interrupt_entry = false;

	if (s32_board.frame_timing)
	{
		// the game waits for the next frame: the board moves on to its next event
		if (pc == kFrameWaitLoop && s32_read8(kFrameReady) == 0)
			s32_wait();
		else if (pc == kFrameWaitDone && !g_stop)
			end_of_frame();
		else if (g_instructions - g_instructions_at_frame > kInstructionsPerFrameLimit)
			throw std::runtime_error("the program no longer reaches its frame loop");
	}
	else
	{
		s32_board.cycles = g_slice_start + uint64_t(g_slice_budget - g_cpu->cycles_left());
		if (s32_board.cycles >= s32_next_timer())
			s32_run_timers();
	}
	// interrupt line changes reach the CPU between instructions, never in the middle of one
	if (g_line_wanted != g_line_applied)
	{
		g_line_applied = g_line_wanted;
		g_cpu->set_input_line(0, g_line_applied ? ASSERT_LINE : CLEAR_LINE);
	}
}

void run_until(uint64_t target)
{
	while (s32_board.cycles < target)
	{
		// with the sound board, the two CPUs take turns often enough to talk
		const uint64_t slice = std::min<uint64_t>(target - s32_board.cycles, g_sound ? 2048 : 1 << 20);
		g_slice_start = s32_board.cycles;
		g_slice_budget = int(slice);
		const int executed = g_cpu->run(g_slice_budget);
		if (s32_board.time_passes)
			s32_board.time_passes(g_slice_start + uint64_t(executed));
		s32_board.cycles = g_slice_start + uint64_t(executed);
		s32_run_timers();
	}
}

// MAME's timing: VBLANK starts at line 224; it ends at line 0, and the sprite processing for the next
// frame happens 50 us later
void run_cycle_timing()
{
	for (uint64_t frame = 0; !g_stop; frame++)
	{
		s32_board.frame = frame;
		run_until(s32_line_start(frame, S32_VISIBLE_LINES));
		s32_vblank_start();
		run_until(s32_frame_start(frame + 1));
		s32_vblank_end();
		run_until(s32_frame_start(frame + 1) + S32_CPU_CLOCK / 20000);
		s32_sprite_update();
		end_of_frame();
	}
}

void run_frame_timing()
{
	// slices only bound how long the run goes on after the last frame; instructions take no time
	while (!g_stop)
		g_cpu->run(4096);
}

void write_addresses(const std::string &path, const std::vector<uint8_t> &bits, const char *title)
{
	std::ofstream out(path);
	out << "# " << title << ", one per line\n";
	for (uint32_t address = 0; address < (1u << 24); address++)
		if (bits[address >> 3] & (1u << (address & 7)))
		{
			char line[16];
			std::snprintf(line, sizeof(line), "%06X\n", address);
			out << line;
		}
}

void write_transfers(const std::string &path)
{
	std::ofstream out(path);
	out << "# where jmp, jsr, call and returns went: FROM TO, one pair per line\n";
	for (uint64_t pair : g_transfers)
	{
		char line[24];
		std::snprintf(line, sizeof(line), "%06X %06X\n", uint32_t(pair >> 24), uint32_t(pair & 0xffffff));
		out << line;
	}

	// targets of jumps and calls that read their address from a register or memory
	std::set<uint32_t> targets;
	for (uint64_t pair : g_transfers)
	{
		const uint8_t opcode = s32_read8(uint32_t(pair >> 24));
		if (opcode == 0x49 || opcode == 0xd6 || opcode == 0xd7 || opcode == 0xe8 || opcode == 0xe9)
			targets.insert(uint32_t(pair & 0xffffff));
	}
	out << "# instructions that stored those targets with a 32-bit write: STORED TARGET WRITER\n";
	for (uint64_t pair : g_code_writes)
	{
		const uint32_t value = uint32_t(pair >> 24);
		if (!targets.count(value))
			continue;
		char line[32];
		std::snprintf(line, sizeof(line), "STORED %06X %06X\n", value, uint32_t(pair & 0xffffff));
		out << line;
	}
}

} // namespace

int main(int argc, char **argv)
{
	std::string trace_path;
	std::string calls_path;
	std::string hashes_path;
	std::string video_hashes_path;
	std::string sound_hashes_path;
	std::string sound_log_path;
	std::string sound_wav_path;
	std::string sound_trace_path;
	std::string profile_path;
	std::string script_path;
	std::string transfers_path;
	bool frames_given = false;
	bool frame_timing = true;
	const char *usage = "Usage: s32ref [--timing frames|cycles] [--frames N] [--script FILE]"
		" [--hashes FILE] [--trace FILE] [--calls FILE] [--transfers FILE] [--watch ADDRESS ...] [--profile FILE]"
		" [--dump-frame N ... --dump-dir DIR] [--video-hashes FILE] [--video-dump N ... --video-dir DIR]"
		" [--sound-hashes FILE] [--sound-log FILE] [--sound-wav FILE] [--sound-trace FILE] [--verbose]\n";

	for (int index = 1; index < argc; index++)
	{
		const std::string argument = argv[index];
		const bool has_value = index + 1 < argc;
		if (argument == "--frames" && has_value)
		{
			g_frames_wanted = std::stoull(argv[++index]);
			frames_given = true;
		}
		else if (argument == "--timing" && has_value)
		{
			const std::string timing = argv[++index];
			if (timing != "frames" && timing != "cycles")
			{
				std::cerr << usage;
				return 1;
			}
			frame_timing = timing == "frames";
		}
		else if (argument == "--watch" && has_value)
			g_watched.push_back(uint32_t(std::stoul(argv[++index], nullptr, 16)) & 0xffffff);
		else if (argument == "--trace" && has_value)
			trace_path = argv[++index];
		else if (argument == "--script" && has_value)
			script_path = argv[++index];
		else if (argument == "--transfers" && has_value)
			transfers_path = argv[++index];
		else if (argument == "--calls" && has_value)
			calls_path = argv[++index];
		else if (argument == "--profile" && has_value)
			profile_path = argv[++index];
		else if (argument == "--hashes" && has_value)
			hashes_path = argv[++index];
		else if (argument == "--dump-frame" && has_value)
			g_dump_frames.insert(std::stoull(argv[++index]));
		else if (argument == "--dump-dir" && has_value)
			g_dump_dir = argv[++index];
		else if (argument == "--video-hashes" && has_value)
			video_hashes_path = argv[++index];
		else if (argument == "--video-dump" && has_value)
			g_video_dump_frames.insert(std::stoull(argv[++index]));
		else if (argument == "--video-dir" && has_value)
			g_video_dir = argv[++index];
		else if (argument == "--sound-hashes" && has_value)
			sound_hashes_path = argv[++index];
		else if (argument == "--sound-log" && has_value)
			sound_log_path = argv[++index];
		else if (argument == "--sound-wav" && has_value)
			sound_wav_path = argv[++index];
		else if (argument == "--sound-trace" && has_value)
			sound_trace_path = argv[++index];
		else if (argument == "--verbose")
			g_verbose = true;
		else
		{
			std::cerr << usage;
			return 1;
		}
	}
	if (!script_path.empty())
	{
		char error[256];
		g_script = s32_script_load(script_path.c_str(), error, sizeof(error));
		if (!g_script)
		{
			std::cerr << error << '\n';
			return 1;
		}
		if (!frames_given && s32_script_frames(g_script))
			g_frames_wanted = s32_script_frames(g_script);
	}

	g_record_transfers = !transfers_path.empty();
	if (!profile_path.empty())
	{
		g_pc_counts.assign(1u << 24, 0);
		g_read_counts.assign(1u << 24, 0);
	}
	g_watched_values.assign(g_watched.size(), 0);
	if (!hashes_path.empty())
		g_hashes.open(hashes_path);

	// the reference video, from the tile and sprite ROMs
	if (!video_hashes_path.empty() || !g_video_dump_frames.empty())
	{
		s32v_attach(game_tile_rom, GAME_TILE_ROM_SIZE, game_sprite_rom, GAME_SPRITE_ROM_SIZE);
		g_video = true;
		if (!video_hashes_path.empty())
			g_video_hashes.open(video_hashes_path);
	}

	// the reference sound board, from the sound ROMs
	FILE *sound_log = nullptr;
	g_sound = !sound_hashes_path.empty() || !sound_log_path.empty() || !sound_wav_path.empty()
		|| !sound_trace_path.empty();
	if (g_sound)
	{
		s32_board.sound_rom = game_sound_rom;
		s32_board.pcm_rom = game_pcm_rom;
		s32s_attach();
		if (!sound_hashes_path.empty())
			g_sound_hashes.open(sound_hashes_path);
		if (!sound_log_path.empty())
		{
			sound_log = std::fopen(sound_log_path.c_str(), "w");
			s32s_log_writes(sound_log);
		}
	}

	s32_board.irq_changed = on_irq_changed;
	s32_board.controls_update = [] { s32_script_apply(g_script, s32_board.frame); };
	s32_board.log = on_log;
	s32_board.frame_timing = frame_timing;
	s32_init(game_program_rom);
	choose_drawing(0);

	machine_config config;
	v70_device cpu(config, "maincpu", nullptr, S32_CPU_CLOCK);
	g_cpu = &cpu;
	for (int index : { AS_PROGRAM, AS_IO })
	{
		address_space &space = cpu.space(index);
		space.width = index == AS_PROGRAM ? 32 : 16;    // the V70's program bus is 32 bits wide
		space.r8 = read8;
		space.r16 = read16;
		space.r32 = read32;
		space.w8 = write8;
		space.w16 = write16;
		space.w32 = write32;
	}
	cpu.irq_cycle_callback().set([] { g_interrupts++; g_interrupt_entry = true; return s32_irq_acknowledge(); });
	cpu.instruction_hook = on_instruction;
	if (g_verbose)
		cpu.log_handler = [](const std::string &message) { std::cerr << "[v60] " << message << '\n'; };
	cpu.start();
	cpu.reset();

	int status = 0;
	try
	{
		if (frame_timing)
			run_frame_timing();
		else
			run_cycle_timing();
	}
	catch (const std::exception &error)
	{
		std::cerr << "V70 stopped at PC " << std::hex << s32_board.pc << std::dec << " in frame " << g_frames_done
			<< ": " << error.what() << '\n';
		status = 2;
	}

	size_t unique = 0;
	for (uint8_t bits : g_executed)
		for (; bits; bits &= uint8_t(bits - 1))
			unique++;
	std::cout << "frames:            " << g_frames_done << '\n'
		<< "instructions:      " << g_instructions << '\n'
		<< "unique addresses:  " << unique << '\n'
		<< "interrupts taken:  " << g_interrupts << '\n'
		<< "sprite lists:      " << s32_board.sprite_lists << '\n'
		<< "sound requests:    " << s32_board.sound_requests << '\n'
		<< "unmapped reads:    " << s32_board.unmapped_reads << '\n'
		<< "unmapped writes:   " << s32_board.unmapped_writes << '\n'
		<< "eeprom written:    " << (s32_board.eeprom.dirty ? "yes" : "no") << '\n'
		<< "final PC:          " << std::hex << s32_board.pc << std::dec << '\n';
	if (g_sound)
		std::cout << "Z80 cycles:        " << s32s_z80_cycles() << '\n';
	if (!sound_wav_path.empty())
		write_wav(sound_wav_path, g_sound_mix, s32s_mix_rate());
	if (!sound_trace_path.empty())
	{
		std::ofstream out(sound_trace_path);
		out << "# Z80 addresses executed by s32ref, one per line\n";
		for (uint32_t address = 0; address < 0x10000; address++)
			if (s32s_executed()[address >> 3] & (1u << (address & 7)))
			{
				char line[8];
				std::snprintf(line, sizeof(line), "%04X\n", address);
				out << line;
			}
	}
	if (sound_log)
		std::fclose(sound_log);
	if (!trace_path.empty())
		write_addresses(trace_path, g_executed, "V70 addresses executed by s32ref");
	if (!calls_path.empty())
		write_addresses(calls_path, g_called, "V70 addresses reached by jsr, bsr or call");
	if (!profile_path.empty())
		write_profile(profile_path);
	if (!transfers_path.empty())
		write_transfers(transfers_path);
	s32_script_free(g_script);
	return status;
}
