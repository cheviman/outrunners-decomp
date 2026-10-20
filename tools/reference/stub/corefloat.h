/**
 *    Name:        corefloat.h
 *    Description: stand-in for MAME's corefloat.h: bit casts between 32-bit integers and floats
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 */
#pragma once

#include <cstdint>
#include <cstring>

inline float u2f(uint32_t value)
{
	float result;
	std::memcpy(&result, &value, sizeof(result));
	return result;
}

inline uint32_t f2u(float value)
{
	uint32_t result;
	std::memcpy(&result, &value, sizeof(result));
	return result;
}
