/**
 *    Name:        logmacro.h
 *    Description: stand-in for MAME's logmacro.h: the cores' log lines go nowhere
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#pragma once

#define LOG(...) do { } while (0)
#define LOGMASKED(mask, ...) do { } while (0)
