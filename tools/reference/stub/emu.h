/**
 *    Name:        emu.h
 *    Description: minimal stand-in for MAME's emu.h, enough to build MAME's V60 core and disassembler, and
 *                 its Z80 disassembler, alone
 *    Version:     0.2.1
 *    Created:     11-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    Only what v60.cpp, v60d.cpp, their .hxx files and z80dasm.cpp use. Memory goes through function
 *    pointers set by the host program; cpu_device adds a small public API (start, reset, run, registers).
 */
#pragma once

#include <cassert>
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
#define DECLARE_DEVICE_TYPE(Type, Class) extern const device_type Type;
#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName) const device_type Type = ShortName;

enum { CLEAR_LINE = 0, ASSERT_LINE = 1 };
enum { INPUT_LINE_NMI = 32 };
enum { AS_PROGRAM = 0, AS_IO = 1 };
enum { STATE_GENPC = -1, STATE_GENPCBASE = -2, STATE_GENFLAGS = -3 };
enum endianness_t { ENDIANNESS_LITTLE, ENDIANNESS_BIG };

// The CPU core calls this on conditions it cannot continue from; the host catches it
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

// Arithmetic helpers from MAME's eminline.h and coretmpl.h
inline int64_t mul_32x32(int32_t a, int32_t b)
{
	return int64_t(a) * int64_t(b);
}

inline uint64_t mulu_32x32(uint32_t a, uint32_t b)
{
	return uint64_t(a) * uint64_t(b);
}

inline int32_t div_64x32_rem(int64_t a, int32_t b, int32_t &remainder)
{
	if (b == 0)
		fatalerror("V60 division by zero\n");
	remainder = int32_t(a % b);
	return int32_t(a / b);
}

inline uint32_t divu_64x32_rem(uint64_t a, uint32_t b, uint32_t &remainder)
{
	if (b == 0)
		fatalerror("V60 division by zero\n");
	remainder = uint32_t(a % b);
	return uint32_t(a / b);
}

inline uint32_t swapendian_int32(uint32_t value)
{
	return (value >> 24) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | (value << 24);
}

// bitswap<N>(value, b...): the first listed bit becomes the most significant bit of the result
template <int Bits, typename T, typename... Positions>
T bitswap(T value, Positions... positions)
{
	T result = 0;
	for (int position : { int(positions)... })
		result = T((result << 1) | ((value >> position) & 1));
	return result;
}

namespace util {

// An argument as printf takes it: MAME's formatter also accepts std::string, which printf does not
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

	// Little-endian reads from a flat image; addresses wrap with a mask, like a mirrored bus
	class data_buffer
	{
	public:
		data_buffer(const u8 *data, u32 mask) : m_data(data), m_mask(mask) {}
		u8 r8(offs_t address) const { return m_data[address & m_mask]; }
		u16 r16(offs_t address) const { return u16(r8(address) | (r8(address + 1) << 8)); }
		u32 r32(offs_t address) const { return u32(r16(address)) | (u32(r16(address + 2)) << 16); }

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
class device_t;
using device_type = const char *;

class address_space_config
{
public:
	address_space_config(const char *name, endianness_t endian, int data_width, int address_width, int shift)
		: m_data_width(data_width)
	{
		(void)name; (void)endian; (void)address_width; (void)shift;
	}

	int m_data_width;
};

// Memory as the CPU sees it; the host fills in the six access functions
class address_space
{
public:
	u8 (*r8)(offs_t) = nullptr;
	u16 (*r16)(offs_t) = nullptr;
	u32 (*r32)(offs_t) = nullptr;
	void (*w8)(offs_t, u8) = nullptr;
	void (*w16)(offs_t, u16) = nullptr;
	void (*w32)(offs_t, u32) = nullptr;
	int width = 16;

	int data_width() const { return width; }
	u8 read_byte(offs_t address) { return r8(address); }
	u16 read_word(offs_t address) { return r16(address); }
	u16 read_word_unaligned(offs_t address) { return r16(address); }
	u32 read_dword(offs_t address) { return r32(address); }
	u32 read_dword_unaligned(offs_t address) { return r32(address); }
	u64 read_qword_unaligned(offs_t address) { return r32(address) | (u64(r32(address + 4)) << 32); }
	void write_byte(offs_t address, u8 data) { w8(address, data); }
	void write_word(offs_t address, u16 data) { w16(address, data); }
	void write_word_unaligned(offs_t address, u16 data) { w16(address, data); }
	void write_dword(offs_t address, u32 data) { w32(address, data); }
	void write_dword_unaligned(offs_t address, u32 data) { w32(address, data); }
	void write_qword_unaligned(offs_t address, u64 data) { w32(address, u32(data)); w32(address + 4, u32(data >> 32)); }

	template <typename Cache>
	void cache(Cache &target) { target.space = this; }
};

template <int Width, int AddressShift, int Unused, endianness_t Endian>
struct memory_access
{
	struct cache
	{
		address_space *space = nullptr;
		u8 read_byte(offs_t address) { return space->read_byte(address); }
		u16 read_word_unaligned(offs_t address) { return space->read_word_unaligned(address); }
		u32 read_dword_unaligned(offs_t address) { return space->read_dword_unaligned(address); }
	};
};

class devcb_read8
{
public:
	struct binder
	{
		devcb_read8 *target;
		void set(std::function<u8()> function) { target->m_function = std::move(function); }
	};

	devcb_read8(device_t &owner, u8 default_value) : m_default(default_value) { (void)owner; }
	binder bind() { return binder{ this }; }
	u8 operator()() { return m_function ? m_function() : m_default; }

private:
	std::function<u8()> m_function;
	u8 m_default;
};

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
	{
		(void)config; (void)type; (void)tag; (void)owner; (void)clock;
	}
	virtual ~device_t() = default;

	// Messages from the core about odd situations; the host may print them
	std::function<void(const std::string &)> log_handler;

	template <typename... Args>
	void logerror(const char *format, Args... args)
	{
		if (log_handler)
			log_handler(string_format(format, args...));
	}

protected:
	virtual void device_start() {}
	virtual void device_reset() {}
};

class device_memory_interface
{
public:
	using space_config_vector = std::vector<std::pair<int, const address_space_config *>>;
};

class cpu_device : public device_t, public device_memory_interface
{
public:
	cpu_device(const machine_config &config, device_type type, const char *tag, device_t *owner, u32 clock)
		: device_t(config, type, tag, owner, clock) {}

	// Host API
	std::function<void(offs_t)> instruction_hook;
	void start() { device_start(); }
	void reset() { device_reset(); }
	int run(int cycles) { *m_icountptr = cycles; execute_run(); return cycles - *m_icountptr; }
	int cycles_left() const { return *m_icountptr; }
	void set_input_line(int line, int state) { execute_set_input(line, state); }
	address_space &space(int index = AS_PROGRAM) { return m_spaces[index]; }
	u32 state_value(int index)
	{
		state_export(device_state_entry(index));
		auto found = m_state.find(index);
		return found != m_state.end() ? *found->second : 0;
	}

protected:
	struct state_entry
	{
		state_entry &formatstr(const char *) { return *this; }
		state_entry &callimport() { return *this; }
		state_entry &callexport() { return *this; }
		state_entry &noshow() { return *this; }
	};

	virtual u32 execute_min_cycles() const noexcept { return 1; }
	virtual u32 execute_max_cycles() const noexcept { return 1; }
	virtual bool execute_input_edge_triggered(int inputnum) const noexcept { (void)inputnum; return false; }
	virtual void execute_run() = 0;
	virtual void execute_set_input(int inputnum, int state) { (void)inputnum; (void)state; }
	virtual space_config_vector memory_space_config() const = 0;
	virtual void state_import(const device_state_entry &entry) { (void)entry; }
	virtual void state_export(const device_state_entry &entry) { (void)entry; }
	virtual void state_string_export(const device_state_entry &entry, std::string &text) const { (void)entry; (void)text; }
	virtual std::unique_ptr<util::disasm_interface> create_disassembler() { return nullptr; }

	void set_icountptr(int &icount) { m_icountptr = &icount; }
	template <typename T> void save_item(T &item, const char *name) { (void)item; (void)name; }
	state_entry &state_add(int index, const char *name, u32 &value)
	{
		(void)name;
		m_state[index] = &value;
		return m_state_entry;
	}
	void standard_irq_callback(int line, offs_t pc) { (void)line; (void)pc; }
	void debugger_instruction_hook(offs_t pc) { if (instruction_hook) instruction_hook(pc); }

private:
	address_space m_spaces[2];
	int m_dummy_icount = 0;
	int *m_icountptr = &m_dummy_icount;
	std::map<int, u32 *> m_state;
	state_entry m_state_entry;
};
