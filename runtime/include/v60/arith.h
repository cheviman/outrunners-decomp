/**
 *    Name:        arith.h
 *    Description: V60 arithmetic as pure functions: results and flags of MAME's core, each one on its own
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Ported from MAME src/devices/cpu/v60 (v60.cpp macros and op12.hxx, Farfetch'd, R. Belmont), the same
 *    behavior as ops.h but with no state: code generated from tools/v60/ir.py computes each flag where it
 *    needs it. Shift counts are masked as on x86-64, where the reference runs (see ops.h).
 */
#ifndef V60_ARITH_H
#define V60_ARITH_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint32_t shl32(uint32_t value, int count) { return value << (count & 31); }
static inline uint32_t shr32(uint32_t value, int count) { return value >> (count & 31); }
static inline int32_t sar32(int32_t value, int count) { return value >> (count & 31); }
static inline uint64_t shl64(uint64_t value, int count) { return value << (count & 63); }
static inline uint64_t shr64(uint64_t value, int count) { return value >> (count & 63); }

/* ---- add / subtract: carry and overflow of a + b + carry_in and a - b - borrow_in ---- */

static inline uint32_t v60_carry_add8(uint32_t a, uint32_t b, uint32_t carry) { return ((a + b + carry) >> 8) & 1; }
static inline uint32_t v60_carry_add16(uint32_t a, uint32_t b, uint32_t carry) { return ((a + b + carry) >> 16) & 1; }
static inline uint32_t v60_carry_add32(uint32_t a, uint32_t b, uint32_t carry)
{
	return (uint32_t)((((uint64_t)a + b + carry) >> 32) & 1);
}

static inline uint32_t v60_overflow_add8(uint32_t a, uint32_t b, uint32_t carry)
{
	const uint32_t result = a + b + carry;
	return ((result ^ b) & (result ^ a) & 0x80) ? 1 : 0;
}

static inline uint32_t v60_overflow_add16(uint32_t a, uint32_t b, uint32_t carry)
{
	const uint32_t result = a + b + carry;
	return ((result ^ b) & (result ^ a) & 0x8000) ? 1 : 0;
}

static inline uint32_t v60_overflow_add32(uint32_t a, uint32_t b, uint32_t carry)
{
	const uint64_t result = (uint64_t)a + b + carry;
	return ((result ^ b) & (result ^ a) & 0x80000000u) ? 1 : 0;
}

static inline uint32_t v60_carry_sub8(uint32_t a, uint32_t b, uint32_t borrow) { return ((a - b - borrow) >> 8) & 1; }
static inline uint32_t v60_carry_sub16(uint32_t a, uint32_t b, uint32_t borrow) { return ((a - b - borrow) >> 16) & 1; }
static inline uint32_t v60_carry_sub32(uint32_t a, uint32_t b, uint32_t borrow)
{
	return (uint32_t)((((uint64_t)a - b - borrow) >> 32) & 1);
}

static inline uint32_t v60_overflow_sub8(uint32_t a, uint32_t b, uint32_t borrow)
{
	const uint32_t result = a - b - borrow;
	return ((a ^ b) & (a ^ result) & 0x80) ? 1 : 0;
}

static inline uint32_t v60_overflow_sub16(uint32_t a, uint32_t b, uint32_t borrow)
{
	const uint32_t result = a - b - borrow;
	return ((a ^ b) & (a ^ result) & 0x8000) ? 1 : 0;
}

static inline uint32_t v60_overflow_sub32(uint32_t a, uint32_t b, uint32_t borrow)
{
	const uint64_t result = (uint64_t)a - b - borrow;
	return (((uint64_t)a ^ b) & ((uint64_t)a ^ result) & 0x80000000u) ? 1 : 0;
}

/* ---- divide: no change when dividing by zero; signed division overflows on MIN / -1 ---- */

static inline uint32_t v60_div_overflow8(uint32_t a, uint32_t b) { return a == 0x80 && b == 0xff; }
static inline uint32_t v60_div_overflow16(uint32_t a, uint32_t b) { return a == 0x8000 && b == 0xffff; }
static inline uint32_t v60_div_overflow32(uint32_t a, uint32_t b) { return a == 0x80000000u && b == 0xffffffffu; }

static inline uint32_t v60_div_value8(uint32_t a, uint32_t b)
{
	return (b && !v60_div_overflow8(a, b)) ? (uint8_t)((int8_t)a / (int8_t)b) : a;
}

static inline uint32_t v60_div_value16(uint32_t a, uint32_t b)
{
	return (b && !v60_div_overflow16(a, b)) ? (uint16_t)((int16_t)a / (int16_t)b) : a;
}

static inline uint32_t v60_div_value32(uint32_t a, uint32_t b)
{
	return (b && !v60_div_overflow32(a, b)) ? (uint32_t)((int32_t)a / (int32_t)b) : a;
}

static inline uint32_t v60_divu_value8(uint32_t a, uint32_t b) { return b ? (uint8_t)(a / b) : a; }
static inline uint32_t v60_divu_value16(uint32_t a, uint32_t b) { return b ? (uint16_t)(a / b) : a; }
static inline uint32_t v60_divu_value32(uint32_t a, uint32_t b) { return b ? a / b : a; }

static inline uint32_t v60_rem_value8(uint32_t a, uint32_t b) { return b ? (uint8_t)((int8_t)a % (int8_t)b) : a; }
static inline uint32_t v60_rem_value16(uint32_t a, uint32_t b) { return b ? (uint16_t)((int16_t)a % (int16_t)b) : a; }

static inline uint32_t v60_rem_value32(uint32_t a, uint32_t b)
{
	if (!b)
		return a;
	/* INT_MIN % -1 traps on x86; its value is 0 */
	return (a == 0x80000000u && b == 0xffffffffu) ? 0 : (uint32_t)((int32_t)a % (int32_t)b);
}

static inline uint32_t v60_remu_value8(uint32_t a, uint32_t b) { return b ? (uint8_t)(a % b) : a; }
static inline uint32_t v60_remu_value16(uint32_t a, uint32_t b) { return b ? (uint16_t)(a % b) : a; }
static inline uint32_t v60_remu_value32(uint32_t a, uint32_t b) { return b ? a % b : a; }

/* ---- shifts and rotates: the count is the low byte of the operand, signed; negative goes right ---- */

static inline uint32_t v60_width_mask(int bits) { return bits == 32 ? 0xffffffffu : (1u << bits) - 1; }

static inline uint32_t v60_shl_value(uint32_t value, uint32_t operand, int bits)
{
	const int count = (int8_t)(operand & 0xff);
	if (count > 0)
		return shl32(value, count) & v60_width_mask(bits);
	if (count < 0)
		return shr32(value, -count) & v60_width_mask(bits);
	return value;
}

static inline uint32_t v60_shl_cy(uint32_t value, uint32_t operand, int bits)
{
	const int count = (int8_t)(operand & 0xff);
	if (count > 0)
		return bits == 32 ? (uint32_t)((shl64(value, count) >> 32) & 1) : (shl32(value, count) >> bits) & 1;
	if (count < 0)
		return (uint32_t)((bits == 32 ? shr64(value, -count - 1) : shr32(value, -count - 1)) & 1);
	return 0;
}

static inline uint32_t v60_shl_ov(uint32_t value, uint32_t operand, int bits)
{
	(void)value;
	(void)operand;
	(void)bits;
	return 0;
}

static inline uint32_t v60_sha_value(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = v60_width_mask(bits);
	const uint32_t sign = (value >> (bits - 1)) & 1;

	if (count == 0)
		return value;
	if (count > 0)
		return count >= bits ? 0 : shl32(value, count) & mask;
	count = (int8_t)-count;    /* -128 stays -128, as in MAME's int8_t arithmetic */
	if (count >= bits)
		return sign ? mask : 0;
	{
		const int32_t extended = bits == 8 ? (int8_t)value : bits == 16 ? (int16_t)value : (int32_t)value;
		return (uint32_t)sar32(extended, count) & mask;
	}
}

static inline uint32_t v60_sha_cy(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	if (count == 0)
		return 0;
	if (count > 0)
		return (uint32_t)(shr32(value, bits - count) & 1);
	count = (int8_t)-count;
	return (uint32_t)(shr32(value, count - 1) & 1);
}

static inline uint32_t v60_sha_ov(uint32_t value, uint32_t operand, int bits)
{
	const int8_t count = (int8_t)(operand & 0xff);
	if (count > 0)
	{
		const uint32_t sign = (value >> (bits - 1)) & 1;
		uint32_t out = count == 32 ? 0xffffffffu : shl32(1, count) - 1;
		out = shl32(out, bits - count);
		return sign ? (value & out) != out : (value & out) != 0;
	}
	return 0;
}

static inline uint32_t v60_rot_value(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = v60_width_mask(bits);
	const uint32_t top = 1u << (bits - 1);

	if (count > 0)
	{
		for (int8_t i = 0; i < count; i++)
			value = ((value << 1) | ((value & top) ? 1 : 0)) & mask;
	}
	else if (count < 0)
	{
		count = (int8_t)-count;
		for (int8_t i = 0; i < count; i++)
			value = (value >> 1) | ((value & 1) ? top : 0);
	}
	return value;
}

static inline uint32_t v60_rot_cy(uint32_t value, uint32_t operand, int bits)
{
	const int8_t count = (int8_t)(operand & 0xff);
	const uint32_t result = v60_rot_value(value, operand, bits);
	if (count > 0)
		return result & 1;
	if (count < 0)
		return (result & (1u << (bits - 1))) ? 1 : 0;
	return 0;
}

static inline uint32_t v60_rot_ov(uint32_t value, uint32_t operand, int bits)
{
	(void)value;
	(void)operand;
	(void)bits;
	return 0;
}

/* ROTC rotates through the carry: 'carry' is the flag before the instruction */
static inline uint32_t v60_rotc_both(uint32_t value, uint32_t operand, uint32_t carry, int bits, uint32_t *carry_out)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = v60_width_mask(bits);
	const uint32_t top = 1u << (bits - 1);

	if (count > 0)
	{
		for (int8_t i = 0; i < count; i++)
		{
			const uint32_t in = carry;
			carry = (value & top) ? 1 : 0;
			value = ((value << 1) | in) & mask;
		}
	}
	else if (count < 0)
	{
		count = (int8_t)-count;
		for (int8_t i = 0; i < count; i++)
		{
			const uint32_t in = carry;
			carry = value & 1;
			value = (value >> 1) | (in ? top : 0);
		}
	}
	else
		carry = 0;
	*carry_out = carry;
	return value;
}

static inline uint32_t v60_rotc_value(uint32_t value, uint32_t operand, uint32_t carry, int bits)
{
	uint32_t carry_out;
	return v60_rotc_both(value, operand, carry, bits, &carry_out);
}

static inline uint32_t v60_rotc_cy(uint32_t value, uint32_t operand, uint32_t carry, int bits)
{
	uint32_t carry_out;
	v60_rotc_both(value, operand, carry, bits, &carry_out);
	return carry_out;
}

static inline uint32_t v60_rotc_ov(uint32_t value, uint32_t operand, uint32_t carry, int bits)
{
	(void)value;
	(void)operand;
	(void)carry;
	(void)bits;
	return 0;
}

/* ---- MOVT: overflow unless the dropped bits are a sign extension of the result ---- */

static inline uint32_t v60_movt_overflow(uint32_t value, int from_bits, int to_bits)
{
	const uint32_t result = value & ((1u << to_bits) - 1);
	const uint32_t dropped = v60_width_mask(from_bits) & ~((1u << to_bits) - 1);
	const bool negative = (result >> (to_bits - 1)) & 1;
	return !((negative && (value & dropped) == dropped) || (!negative && (value & dropped) == 0));
}

/* ---- SETF: 1 when the condition named by the low 4 bits holds ---- */

static inline uint32_t v60_condition(uint32_t condition, uint32_t z, uint32_t s, uint32_t ov, uint32_t cy)
{
	switch (condition & 0xf)
	{
		case 0: return ov;
		case 1: return !ov;
		case 2: return cy;
		case 3: return !cy;
		case 4: return z;
		case 5: return !z;
		case 6: return cy | z;
		case 7: return !(cy | z);
		case 8: return s;
		case 9: return !s;
		case 10: return 1;
		case 11: return 0;
		case 12: return s ^ ov;
		case 13: return !(s ^ ov);
		case 14: return (s ^ ov) | z;
		default: return !((s ^ ov) | z);
	}
}

/* ---- RVBYT: the four bytes in reverse order ---- */

static inline uint32_t v60_byte_swap(uint32_t value)
{
	return (value >> 24) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | (value << 24);
}

#ifdef __cplusplus
}
#endif

#endif
