/**
 *    Name:        registers.h
 *    Description: V60 register names for the generated code (R0-R28, AP, FP, SP)
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Only the generated files include this: the short names would clash with platform headers.
 */
#ifndef V60_REGISTERS_H
#define V60_REGISTERS_H

#include "v60/cpu.h"

#define R0 (cpu.reg[0])
#define R1 (cpu.reg[1])
#define R2 (cpu.reg[2])
#define R3 (cpu.reg[3])
#define R4 (cpu.reg[4])
#define R5 (cpu.reg[5])
#define R6 (cpu.reg[6])
#define R7 (cpu.reg[7])
#define R8 (cpu.reg[8])
#define R9 (cpu.reg[9])
#define R10 (cpu.reg[10])
#define R11 (cpu.reg[11])
#define R12 (cpu.reg[12])
#define R13 (cpu.reg[13])
#define R14 (cpu.reg[14])
#define R15 (cpu.reg[15])
#define R16 (cpu.reg[16])
#define R17 (cpu.reg[17])
#define R18 (cpu.reg[18])
#define R19 (cpu.reg[19])
#define R20 (cpu.reg[20])
#define R21 (cpu.reg[21])
#define R22 (cpu.reg[22])
#define R23 (cpu.reg[23])
#define R24 (cpu.reg[24])
#define R25 (cpu.reg[25])
#define R26 (cpu.reg[26])
#define R27 (cpu.reg[27])
#define R28 (cpu.reg[28])
#define AP (cpu.reg[V60_AP])
#define FP (cpu.reg[V60_FP])
#define SP (cpu.reg[V60_SP])

#endif
