/**
 *    Name:        dirom.h
 *    Description: stand-in for MAME's dirom.h: a device's ROM, read a byte at a time through a function the
 *                 host sets
 *    Version:     0.1.0
 *    Created:     19-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The MultiPCM reads its samples through it: the host gives it the board's sample memory, banks and all.
 */
#pragma once

template <int AddrWidth, int AddrShift = 0, int DataWidth = 0, endianness_t Endian = ENDIANNESS_LITTLE>
class device_rom_interface
{
public:
	device_rom_interface(const machine_config &, device_t &) {}
	virtual ~device_rom_interface() = default;

	u8 read_byte(offs_t address) { return m_read ? m_read(address & ((1u << AddrWidth) - 1)) : 0; }

	// host side
	std::function<u8(offs_t)> m_read;

protected:
	virtual void rom_bank_pre_change() {}
};
