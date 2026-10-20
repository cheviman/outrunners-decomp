/**
 *    Name:        z80daisy.h
 *    Description: stand-in for MAME's machine/z80daisy.h: the sound board has no daisy chain
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The Z80 core asks its daisy chain first; with none, the interrupt controller of the board answers
 *    through the CPU's interrupt cycle (cpu_device::irq_vector in emu.h).
 */
#pragma once

class device_z80daisy_interface
{
public:
	int z80daisy_irq_ack() { return 0xff; }
};

class z80_daisy_chain_interface
{
public:
	z80_daisy_chain_interface(const machine_config &, device_t &) {}

	bool daisy_chain_present() const { return false; }
	int daisy_update_irq_state() { return CLEAR_LINE; }
	device_z80daisy_interface *daisy_get_irq_device() { return nullptr; }
	void daisy_call_reti_device() {}
};
