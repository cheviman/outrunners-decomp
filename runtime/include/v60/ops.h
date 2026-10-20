/**
 *    Name:        ops.h
 *    Description: V60 arithmetic, logic, shift and bit operations with the flag results of MAME's core
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Ported from MAME src/devices/cpu/v60 (v60.cpp macros and op12.hxx, Farfetch'd, R. Belmont).
 *    MAME leaves some shift counts to the compiler (counts at or above the operand width); the
 *    reference build runs on x86-64, which masks them to 5 bits (32-bit shifts) or 6 bits (64-bit).
 *    The shl32 / shr32 / sar32 / shl64 / shr64 helpers make that masking explicit, so every
 *    platform gets the reference results.
 */
#ifndef V60_OPS_H
#define V60_OPS_H

#include "v60/arith.h"
#include "v60/cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void set_sz8(uint32_t value) { cpu.z = (uint8_t)value == 0; cpu.s = (value >> 7) & 1; }
static inline void set_sz16(uint32_t value) { cpu.z = (uint16_t)value == 0; cpu.s = (value >> 15) & 1; }
static inline void set_sz32(uint32_t value) { cpu.z = value == 0; cpu.s = (value >> 31) & 1; }

/* ---- add / subtract (ADD, ADDC, INC, SUB, SUBC, DEC, CMP, NEG, TASI) ---- */

static inline uint8_t v60_add8(uint8_t dst, uint8_t src, unsigned carry)
{
	const unsigned result = (unsigned)dst + src + carry;
	cpu.cy = (result >> 8) & 1;
	cpu.ov = ((result ^ src) & (result ^ dst) & 0x80) ? 1 : 0;
	set_sz8(result);
	return (uint8_t)result;
}

static inline uint16_t v60_add16(uint16_t dst, uint16_t src, unsigned carry)
{
	const unsigned result = (unsigned)dst + src + carry;
	cpu.cy = (result >> 16) & 1;
	cpu.ov = ((result ^ src) & (result ^ dst) & 0x8000) ? 1 : 0;
	set_sz16(result);
	return (uint16_t)result;
}

static inline uint32_t v60_add32(uint32_t dst, uint32_t src, unsigned carry)
{
	const uint64_t result = (uint64_t)dst + src + carry;
	cpu.cy = (result >> 32) & 1;
	cpu.ov = ((result ^ src) & (result ^ dst) & 0x80000000u) ? 1 : 0;
	set_sz32((uint32_t)result);
	return (uint32_t)result;
}

/* 'src' is signed so NEG can pass MAME's sign-extended operand; other callers pass it unsigned */
static inline uint8_t v60_sub8(uint8_t dst, int32_t src, unsigned borrow)
{
	const unsigned result = (unsigned)((int32_t)dst - src - (int32_t)borrow);
	cpu.cy = (result >> 8) & 1;
	cpu.ov = (((unsigned)dst ^ (unsigned)src) & ((unsigned)dst ^ result) & 0x80) ? 1 : 0;
	set_sz8(result);
	return (uint8_t)result;
}

static inline uint16_t v60_sub16(uint16_t dst, int32_t src, unsigned borrow)
{
	const unsigned result = (unsigned)((int32_t)dst - src - (int32_t)borrow);
	cpu.cy = (result >> 16) & 1;
	cpu.ov = (((unsigned)dst ^ (unsigned)src) & ((unsigned)dst ^ result) & 0x8000) ? 1 : 0;
	set_sz16(result);
	return (uint16_t)result;
}

static inline uint32_t v60_sub32(uint32_t dst, int64_t src, unsigned borrow)
{
	const uint64_t result = (uint64_t)dst - (uint64_t)src - borrow;
	cpu.cy = (result >> 32) & 1;
	cpu.ov = (((uint64_t)dst ^ (uint64_t)src) & ((uint64_t)dst ^ result) & 0x80000000u) ? 1 : 0;
	set_sz32((uint32_t)result);
	return (uint32_t)result;
}

/* ---- logic (OR, AND, XOR, NOT, TEST) ---- */

static inline uint32_t v60_logic8(uint32_t result) { cpu.ov = 0; set_sz8(result); return result & 0xff; }
static inline uint32_t v60_logic16(uint32_t result) { cpu.ov = 0; set_sz16(result); return result & 0xffff; }
static inline uint32_t v60_logic32(uint32_t result) { cpu.ov = 0; set_sz32(result); return result; }

static inline void v60_test8(uint32_t value) { cpu.z = value == 0; cpu.s = (value >> 7) & 1; cpu.cy = 0; cpu.ov = 0; }
static inline void v60_test16(uint32_t value) { cpu.z = value == 0; cpu.s = (value >> 15) & 1; cpu.cy = 0; cpu.ov = 0; }
static inline void v60_test32(uint32_t value) { cpu.z = value == 0; cpu.s = (value >> 31) & 1; cpu.cy = 0; cpu.ov = 0; }

/* ---- multiply / divide ---- */

static inline uint8_t v60_mul8(uint8_t dst, uint32_t src)
{
	const uint32_t product = (uint32_t)((int8_t)dst * (int32_t)(int8_t)src);
	const uint8_t result = (uint8_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 7) & 1;
	cpu.ov = (product >> 8) != 0;
	return result;
}

static inline uint16_t v60_mul16(uint16_t dst, uint32_t src)
{
	const uint32_t product = (uint32_t)((int16_t)dst * (int32_t)(int16_t)src);
	const uint16_t result = (uint16_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 15) & 1;
	cpu.ov = (product >> 16) != 0;
	return result;
}

static inline uint32_t v60_mul32(uint32_t dst, uint32_t src)
{
	const uint64_t product = (uint64_t)((int64_t)(int32_t)dst * (int64_t)(int32_t)src);
	const uint32_t result = (uint32_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 31) & 1;
	cpu.ov = (product >> 32) != 0;
	return result;
}

static inline uint8_t v60_mulu8(uint8_t dst, uint32_t src)
{
	const uint32_t product = (uint32_t)(dst * (uint8_t)src);
	const uint8_t result = (uint8_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 7) & 1;
	cpu.ov = (product >> 8) != 0;
	return result;
}

static inline uint16_t v60_mulu16(uint16_t dst, uint32_t src)
{
	const uint32_t product = (uint32_t)dst * (uint16_t)src;
	const uint16_t result = (uint16_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 15) & 1;
	cpu.ov = (product >> 16) != 0;
	return result;
}

static inline uint32_t v60_mulu32(uint32_t dst, uint32_t src)
{
	const uint64_t product = (uint64_t)dst * src;
	const uint32_t result = (uint32_t)product;
	cpu.z = result == 0;
	cpu.s = (result >> 31) & 1;
	cpu.ov = (product >> 32) != 0;
	return result;
}

static inline uint8_t v60_div8(uint8_t dst, uint32_t src)
{
	cpu.ov = dst == 0x80 && src == 0xff;
	if (src && !cpu.ov)
		dst = (uint8_t)((int8_t)dst / (int8_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 7) & 1;
	return dst;
}

static inline uint16_t v60_div16(uint16_t dst, uint32_t src)
{
	cpu.ov = dst == 0x8000 && src == 0xffff;
	if (src && !cpu.ov)
		dst = (uint16_t)((int16_t)dst / (int16_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 15) & 1;
	return dst;
}

static inline uint32_t v60_div32(uint32_t dst, uint32_t src)
{
	cpu.ov = dst == 0x80000000u && src == 0xffffffffu;
	if (src && !cpu.ov)
		dst = (uint32_t)((int32_t)dst / (int32_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 31) & 1;
	return dst;
}

static inline uint8_t v60_divu8(uint8_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint8_t)(dst / (uint8_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 7) & 1;
	return dst;
}

static inline uint16_t v60_divu16(uint16_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint16_t)(dst / (uint16_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 15) & 1;
	return dst;
}

static inline uint32_t v60_divu32(uint32_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst /= src;
	cpu.z = dst == 0;
	cpu.s = (dst >> 31) & 1;
	return dst;
}

static inline uint8_t v60_rem8(uint8_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint8_t)((int8_t)dst % (int8_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 7) & 1;
	return dst;
}

static inline uint16_t v60_rem16(uint16_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint16_t)((int16_t)dst % (int16_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 15) & 1;
	return dst;
}

static inline uint32_t v60_rem32(uint32_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)    /* INT_MIN % -1 traps on x86; its value is 0 */
		dst = (dst == 0x80000000u && src == 0xffffffffu) ? 0 : (uint32_t)((int32_t)dst % (int32_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 31) & 1;
	return dst;
}

static inline uint8_t v60_remu8(uint8_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint8_t)(dst % (uint8_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 7) & 1;
	return dst;
}

static inline uint16_t v60_remu16(uint16_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst = (uint16_t)(dst % (uint16_t)src);
	cpu.z = dst == 0;
	cpu.s = (dst >> 15) & 1;
	return dst;
}

static inline uint32_t v60_remu32(uint32_t dst, uint32_t src)
{
	cpu.ov = 0;
	if (src)
		dst %= src;
	cpu.z = dst == 0;
	cpu.s = (dst >> 31) & 1;
	return dst;
}

/* ---- shifts and rotates: the count is the low byte of the first operand, signed ---- */

static inline uint32_t v60_shl(uint32_t value, uint32_t operand, int bits)
{
	const int count = (int8_t)(operand & 0xff);
	const uint32_t mask = bits == 32 ? 0xffffffffu : (1u << bits) - 1;

	if (count > 0)
	{
		cpu.ov = 0;
		if (bits == 32)
			cpu.cy = (shl64(value, count) >> 32) & 1;
		else
			cpu.cy = (shl32(value, count) >> bits) & 1;
		value = shl32(value, count) & mask;
	}
	else if (count == 0)
	{
		cpu.cy = cpu.ov = 0;
	}
	else
	{
		cpu.cy = (uint8_t)((bits == 32 ? shr64(value, -count - 1) : shr32(value, -count - 1)) & 1);
		cpu.ov = 0;
		value = shr32(value, -count) & mask;
	}
	cpu.z = value == 0;
	cpu.s = (value >> (bits - 1)) & 1;
	return value;
}

static inline uint32_t v60_sha(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = bits == 32 ? 0xffffffffu : (1u << bits) - 1;
	const uint32_t sign = (value >> (bits - 1)) & 1;

	if (count == 0)
	{
		cpu.cy = cpu.ov = 0;
	}
	else if (count > 0)
	{
		uint32_t out = count == 32 ? 0xffffffffu : shl32(1, count) - 1;
		out = shl32(out, bits - count);
		cpu.ov = sign ? (value & out) != out : (value & out) != 0;
		cpu.cy = (uint8_t)(shr32(value, bits - count) & 1);
		value = count >= bits ? 0 : shl32(value, count) & mask;
	}
	else
	{
		count = (int8_t)-count;    /* -128 stays -128, as in MAME's int8_t arithmetic */
		cpu.ov = 0;
		cpu.cy = (uint8_t)(shr32(value, count - 1) & 1);
		if (count >= bits)
			value = sign ? mask : 0;
		else
		{
			const int32_t extended = bits == 8 ? (int8_t)value : bits == 16 ? (int16_t)value : (int32_t)value;
			value = (uint32_t)sar32(extended, count) & mask;
		}
	}
	cpu.z = value == 0;
	cpu.s = (value >> (bits - 1)) & 1;
	return value;
}

static inline uint32_t v60_rot(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = bits == 32 ? 0xffffffffu : (1u << bits) - 1;
	const uint32_t top = 1u << (bits - 1);

	if (count > 0)
	{
		for (int8_t i = 0; i < count; i++)
			value = ((value << 1) | ((value & top) ? 1 : 0)) & mask;
		cpu.cy = value & 1;
	}
	else if (count < 0)
	{
		count = (int8_t)-count;
		for (int8_t i = 0; i < count; i++)
			value = (value >> 1) | ((value & 1) ? top : 0);
		cpu.cy = (value & top) ? 1 : 0;
	}
	else
		cpu.cy = 0;
	cpu.ov = 0;
	cpu.z = value == 0;
	cpu.s = (value & top) ? 1 : 0;
	return value;
}

static inline uint32_t v60_rotc(uint32_t value, uint32_t operand, int bits)
{
	int8_t count = (int8_t)(operand & 0xff);
	const uint32_t mask = bits == 32 ? 0xffffffffu : (1u << bits) - 1;
	const uint32_t top = 1u << (bits - 1);

	if (count > 0)
	{
		for (int8_t i = 0; i < count; i++)
		{
			const uint32_t carry = cpu.cy;
			cpu.cy = (value & top) ? 1 : 0;
			value = ((value << 1) | carry) & mask;
		}
	}
	else if (count < 0)
	{
		count = (int8_t)-count;
		for (int8_t i = 0; i < count; i++)
		{
			const uint32_t carry = cpu.cy;
			cpu.cy = value & 1;
			value = (value >> 1) | (carry ? top : 0);
		}
	}
	else
		cpu.cy = 0;
	cpu.ov = 0;
	cpu.z = value == 0;
	cpu.s = (value & top) ? 1 : 0;
	return value;
}

/* ---- single bits (SET1, CLR1, NOT1, TEST1): the bit number is not limited to 0-31 ---- */

static inline uint32_t v60_bit_test(uint32_t value, uint32_t bit)
{
	cpu.cy = (value & shl32(1, (int)bit)) != 0;
	cpu.z = !cpu.cy;
	return value;
}

static inline uint32_t v60_set1(uint32_t value, uint32_t bit) { v60_bit_test(value, bit); return value | shl32(1, (int)bit); }
static inline uint32_t v60_clr1(uint32_t value, uint32_t bit) { v60_bit_test(value, bit); return value & ~shl32(1, (int)bit); }
static inline uint32_t v60_not1(uint32_t value, uint32_t bit) { v60_bit_test(value, bit); return value ^ shl32(1, (int)bit); }

/* ---- moves with a check or a condition ---- */

/* MOVT: truncate; overflow unless the dropped bits are a sign extension of the result */
static inline uint32_t v60_movt(uint32_t value, int from_bits, int to_bits)
{
	const uint32_t result = value & ((1u << to_bits) - 1);
	const uint32_t dropped = (from_bits == 32 ? 0xffffffffu : (1u << from_bits) - 1) & ~((1u << to_bits) - 1);
	const bool negative = (result >> (to_bits - 1)) & 1;
	cpu.ov = !((negative && (value & dropped) == dropped) || (!negative && (value & dropped) == 0));
	return result;
}

/* SETF: 1 when the condition named by the low 4 bits holds */
static inline uint8_t v60_setf(uint32_t condition)
{
	switch (condition & 0xf)
	{
		case 0: return cpu.ov;
		case 1: return !cpu.ov;
		case 2: return cpu.cy;
		case 3: return !cpu.cy;
		case 4: return cpu.z;
		case 5: return !cpu.z;
		case 6: return cpu.cy | cpu.z;
		case 7: return !(cpu.cy | cpu.z);
		case 8: return cpu.s;
		case 9: return !cpu.s;
		case 10: return 1;
		case 11: return 0;
		case 12: return cpu.s ^ cpu.ov;
		case 13: return !(cpu.s ^ cpu.ov);
		case 14: return (cpu.s ^ cpu.ov) | cpu.z;
		default: return !((cpu.s ^ cpu.ov) | cpu.z);
	}
}

/* ---- rarely used operations, in v60.c ---- */

void v60_divx(uint32_t divisor, int destination_register, uint32_t destination_address);
void v60_divux(uint32_t divisor, int destination_register, uint32_t destination_address);
void v60_move_string(uint32_t source, uint32_t source_length, uint32_t dest, uint32_t dest_length,
                     int element_size, bool fill, bool downward);
void v60_move_bits_down(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length);
void v60_move_bits_up(uint32_t source, uint32_t source_offset, uint32_t dest, uint32_t dest_offset, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
