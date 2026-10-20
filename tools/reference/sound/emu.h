/**
 *    Name:        emu.h
 *    Description: minimal stand-in for MAME's emu.h, enough to build MAME's Z80 core, its disassembler and
 *                 the RF5C68 alone
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Only what z80.cpp, z80dasm.cpp and rf5c68.cpp use, with MAME's meaning: devices with a clock,
 *    a CPU that runs for a number of cycles, memory as four functions the host sets (Z80 memory and
 *    I/O, the RF5C68's wave memory), a sound stream the host pulls samples from, and no-ops for saving
 *    state, logging and the debugger.
 */
#pragma once

#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using offs_t = uint32_t;

#define ATTR_COLD
#define NAME(x) x, #x
#define STRUCT_MEMBER(array, member) array, #member
#define FUNC(x) &x, #x
#define DECLARE_DEVICE_TYPE(Type, Class) extern const device_type Type;
#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName) const device_type Type = ShortName;

enum { CLEAR_LINE = 0, ASSERT_LINE = 1 };
enum { INPUT_LINE_IRQ0 = 0, INPUT_LINE_NMI = 32 };
enum { AS_PROGRAM = 0, AS_DATA = 1, AS_IO = 2, AS_OPCODES = 3 };
enum { STATE_GENPC = -1, STATE_GENPCBASE = -2, STATE_GENFLAGS = -3 };
enum endianness_t { ENDIANNESS_LITTLE, ENDIANNESS_BIG };

// The cores call this on conditions they cannot continue from; the host catches it
[[noreturn]] inline void fatalerror(const char *format, ...)
{
	char message[512];
	va_list arguments;
	va_start(arguments, format);
	std::vsnprintf(message, sizeof(message), format, arguments);
	va_end(arguments);
	throw std::runtime_error(message);
}

template <typename... Args>
std::string string_format(const char *format, Args... args)
{
	char buffer[512];
	std::snprintf(buffer, sizeof(buffer), format, args...);
	return buffer;
}

template <typename... Args> inline void osd_printf_error(const char *, Args...) {}

// a register pair, as MAME's PAIR16 (little-endian host)
union PAIR16
{
	struct { u8 l, h; } b;
	struct { s8 l, h; } sb;
	u16 w;
	s16 sw;
};

namespace util {

// an argument as printf takes it: MAME's formatter also accepts std::string, which printf does not
template <typename T>
T format_argument(T value)
{
	return value;
}

inline const char *format_argument(const std::string &value)
{
	return value.c_str();
}

// printf-style formatting into a stream, like MAME's util::stream_format
template <typename... Args>
void stream_format(std::ostream &stream, const char *format, Args... args)
{
	char buffer[256];
	std::snprintf(buffer, sizeof(buffer), format, format_argument(args)...);
	stream << buffer;
}

class disasm_interface
{
public:
	enum : u32
	{
		LENGTHMASK = 0x0000ffff,
		STEP_COND = 0x10000000,
		STEP_OVER = 0x20000000,
		STEP_OUT = 0x40000000,
		SUPPORTED = 0x80000000
	};

	// reads from a flat image; addresses wrap with a mask, like a mirrored bus
	class data_buffer
	{
	public:
		data_buffer(const u8 *data, u32 mask) : m_data(data), m_mask(mask) {}
		u8 r8(offs_t address) const { return m_data[address & m_mask]; }
		u16 r16(offs_t address) const { return u16(r8(address) | (r8(address + 1) << 8)); }

	private:
		const u8 *m_data;
		u32 m_mask;
	};

	virtual ~disasm_interface() = default;
	virtual u32 opcode_alignment() const = 0;
	virtual offs_t disassemble(std::ostream &stream, offs_t pc, const data_buffer &opcodes, const data_buffer &params) = 0;
};

} // namespace util

class machine_config {};
class validity_checker {};
class device_t;
using device_type = const char *;

// ---- memory: four functions the host sets per space ----

class address_space_config
{
public:
	address_space_config(const char *name, endianness_t endian, int data_width, int address_width, int shift = 0)
	{
		(void)name; (void)endian; (void)data_width; (void)address_width; (void)shift;
	}
};

class address_space
{
public:
	std::function<u8(offs_t)> read;
	std::function<void(offs_t, u8)> write;

	template <typename Access> void cache(Access &target) { target.space = this; }
	template <typename Access> void specific(Access &target) { target.space = this; }
};

template <int Width, int AddressShift, int Unused, endianness_t Endian>
struct memory_access
{
	struct cache
	{
		address_space *space = nullptr;
		u8 read_byte(offs_t address) { return space->read(address); }
		void write_byte(offs_t address, u8 data) { space->write(address, data); }
	};
	struct specific
	{
		address_space *space = nullptr;
		u8 read_byte(offs_t address) { return space->read(address); }
		void write_byte(offs_t address, u8 data) { space->write(address, data); }
		u8 read_interruptible(offs_t address) { return space->read(address); }
		void write_interruptible(offs_t address, u8 data) { space->write(address, data); }
	};
};

// an address map: only the RF5C68 has one, and the host decodes the Z80's itself
class address_map
{
public:
	struct entry
	{
		entry &mirror(offs_t) { return *this; }
		template <typename... T> entry &w(T &&...) { return *this; }
		template <typename... T> entry &r(T &&...) { return *this; }
		template <typename... T> entry &rw(T &&...) { return *this; }
	};
	entry &operator()(offs_t, offs_t) { return m_entry; }

private:
	entry m_entry;
};

// ---- callbacks the cores raise; nothing listens ----

class devcb_write_line
{
public:
	template <typename T> explicit devcb_write_line(T &) {}
	devcb_write_line &bind() { return *this; }
	void operator()(int) {}
	bool isunset() const { return true; }
	void resolve_safe() {}
};

class devcb_write8
{
public:
	template <typename T> explicit devcb_write8(T &) {}
	devcb_write8 &bind() { return *this; }
	void operator()(offs_t, u8, u8 = 0xff) {}
	bool isunset() const { return true; }
};

template <typename Signature> class device_delegate;
template <typename R, typename... A> class device_delegate<R (A...)>
{
public:
	template <typename T> explicit device_delegate(T &) {}
	bool isnull() const { return true; }
	void resolve() {}
	R operator()(A...) { return R(); }
	template <typename... T> void set(T &&...) {}
};

// ---- devices ----

class device_state_entry
{
public:
	explicit device_state_entry(int index) : m_index(index) {}
	int index() const { return m_index; }

private:
	int m_index;
};

class device_t
{
public:
	device_t(const machine_config &config, device_type type, const char *tag, device_t *owner, u32 clock)
		: m_clock(clock)
	{
		(void)config; (void)type; (void)tag; (void)owner;
	}
	virtual ~device_t() = default;

	u32 clock() const { return m_clock; }
	const char *tag() const { return "sound"; }
	template <typename... Args> void logerror(const char *, Args...) {}
	template <typename T> void save_item(T &&, const char *) {}
	template <typename T> void save_item(T &&, const char *, int) {}

	// host API
	void start() { device_start(); }
	void reset() { device_reset(); }

protected:
	virtual void device_start() {}
	virtual void device_reset() {}
	virtual void device_clock_changed() {}
	virtual void device_validity_check(validity_checker &) const {}
	virtual void device_pre_save() {}
	virtual void device_post_load() {}

private:
	u32 m_clock;
};

class device_memory_interface
{
public:
	using space_config_vector = std::vector<std::pair<int, const address_space_config *>>;

	device_memory_interface(const machine_config &, device_t &) {}
	virtual ~device_memory_interface() = default;

	address_space &space(int index = AS_PROGRAM) { return m_spaces[index]; }
	bool has_space(int index) const { return index != AS_OPCODES; }
	bool has_configured_map(int index) const { return index != AS_OPCODES; }

protected:
	virtual space_config_vector memory_space_config() const = 0;

private:
	address_space m_spaces[4];
};

// a sound stream: the host asks a device for a number of samples, which it writes as integers
class sound_stream
{
public:
	int samples() const { return m_samples; }
	void put_int_clamp(int output, int index, s32 value, s32 range)
	{
		if (value < -range)
			value = -range;
		if (value > range - 1)
			value = range - 1;
		m_outputs[output][index] = value;
	}
	void update() { if (m_before_change) m_before_change(); }
	void set_sample_rate(u32 rate) { m_rate = rate; }

	// host side
	std::function<void()> m_before_change;    // brings the stream up to the present before a change
	int m_samples = 0;
	std::vector<s32> m_outputs[2];
	u32 m_rate = 0;
};

class device_sound_interface
{
public:
	device_sound_interface(const machine_config &, device_t &) {}
	virtual ~device_sound_interface() = default;

	// host API: the next 'count' samples
	void generate(int count)
	{
		m_stream.m_samples = count;
		for (auto &output : m_stream.m_outputs)
			output.assign(size_t(count), 0);
		sound_stream_update(m_stream);
	}
	sound_stream &stream() { return m_stream; }

protected:
	sound_stream *stream_alloc(int inputs, int outputs, u32 rate)
	{
		(void)inputs; (void)outputs;
		m_stream.m_rate = rate;
		return &m_stream;
	}
	virtual void sound_stream_update(sound_stream &stream) = 0;

private:
	sound_stream m_stream;
};

// the debugger is never on
struct debugger_stub
{
	void interrupt_hook(int, offs_t) {}
};

class device_execute_interface
{
public:
	struct owner_stub
	{
		debugger_stub *debug() { return nullptr; }
	};
	owner_stub &device() { return m_owner; }

private:
	owner_stub m_owner;
};

class cpu_device : public device_t, public device_memory_interface, public device_execute_interface
{
public:
	cpu_device(const machine_config &config, device_type type, const char *tag, device_t *owner, u32 clock)
		: device_t(config, type, tag, owner, clock), device_memory_interface(config, *this) {}

	// host API
	int run(int cycles)
	{
		*m_icountptr = cycles;
		execute_run();
		m_total += u64(cycles - *m_icountptr);
		return cycles - *m_icountptr;
	}
	int cycles_left() const { return *m_icountptr; }
	void set_input_line(int line, int state) { execute_set_input(line, state); }
	std::function<int()> irq_vector;            // what the interrupt cycle reads from the bus
	std::function<void(offs_t)> instruction_hook;    // before each instruction, with its address

protected:
	struct state_entry
	{
		state_entry &formatstr(const char *) { return *this; }
		state_entry &callimport() { return *this; }
		state_entry &callexport() { return *this; }
		state_entry &noshow() { return *this; }
		state_entry &mask(u64) { return *this; }
	};

	virtual bool cpu_is_interruptible() const { return false; }
	virtual u32 execute_min_cycles() const noexcept { return 1; }
	virtual u32 execute_max_cycles() const noexcept { return 1; }
	virtual u32 execute_default_irq_vector(int) const noexcept { return 0; }
	virtual bool execute_input_edge_triggered(int) const noexcept { return false; }
	virtual void execute_run() = 0;
	virtual void execute_set_input(int, int) {}
	virtual void state_import(const device_state_entry &) {}
	virtual void state_export(const device_state_entry &) {}
	virtual void state_string_export(const device_state_entry &, std::string &) const {}
	virtual std::unique_ptr<util::disasm_interface> create_disassembler() { return nullptr; }

	void set_icountptr(int &icount) { m_icountptr = &icount; }
	template <typename T> state_entry &state_add(int, const char *, T &) { return m_state_entry; }
	int standard_irq_callback(int, offs_t) { return irq_vector ? irq_vector() : 0xff; }
	u64 total_cycles() const { return m_total; }
	bool access_to_be_redone() { return false; }
	bool debugger_enabled() const { return false; }
	void debugger_instruction_hook(offs_t pc) { if (instruction_hook) instruction_hook(pc); }
	void debugger_wait_hook() {}

private:
	int m_dummy_icount = 0;
	int *m_icountptr = &m_dummy_icount;
	u64 m_total = 0;
	state_entry m_state_entry;
};
