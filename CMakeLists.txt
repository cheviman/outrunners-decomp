# Name:        CMakeLists.txt
# Description: build of the OutRunners runtime and its development tools
# Version:     0.1.0
# Created:     11-10-2026
# Modified:    20-10-2026
# Author:      Sebastian Toyos
cmake_minimum_required(VERSION 3.20)
project(outrunners C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Every executable lands in build/bin (ORUNNERS_BIN_DIR), whatever the generator or configuration
set(ORUNNERS_BIN_DIR ${CMAKE_SOURCE_DIR}/build/bin CACHE PATH "Where the executables go")
foreach(config IN ITEMS "" _DEBUG _RELEASE _RELWITHDEBINFO _MINSIZEREL)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${config} ${ORUNNERS_BIN_DIR})
endforeach()

if(MSVC)
	add_compile_options(/W3 /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Multi 32 board model (script.c drives the controls in test runs). The game's own video and sound chip
# libraries come back into it when they draw two screens and play the MultiPCM
add_library(s32runtime STATIC
	runtime/src/machine.c
	runtime/src/io.c
	runtime/src/script.c
	runtime/src/sound.c
	runtime/src/ym3438.c
	runtime/src/memory_census.c
	runtime/src/tasks.c
)
target_include_directories(s32runtime PUBLIC runtime/include)

# The game's data, carried inside the executables that run it: the board's ROM regions, written from the
# player's ROM set (rom/, its files or its .zip; ORUNNERS_ROM picks another) by tools/embed_data.py when
# the set changes. The C it writes stays in the build folder: the ROM never goes into the repository
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(ORUNNERS_ROM ${CMAKE_SOURCE_DIR}/rom CACHE PATH "The ROM set the game's data is written from")
if(IS_DIRECTORY ${ORUNNERS_ROM})
	file(GLOB ORUNNERS_ROM_FILES ${ORUNNERS_ROM}/*)
else()
	set(ORUNNERS_ROM_FILES ${ORUNNERS_ROM})
endif()
set(ORUNNERS_DATA_DIR ${CMAKE_BINARY_DIR}/data)
set(ORUNNERS_DATA_SOURCES
	${ORUNNERS_DATA_DIR}/program.c ${ORUNNERS_DATA_DIR}/tiles.c ${ORUNNERS_DATA_DIR}/sprites.c
	${ORUNNERS_DATA_DIR}/sound.c ${ORUNNERS_DATA_DIR}/pcm.c)
add_custom_command(OUTPUT ${ORUNNERS_DATA_SOURCES}
	COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/tools/embed_data.py ${ORUNNERS_ROM} ${ORUNNERS_DATA_DIR}
	DEPENDS ${ORUNNERS_ROM_FILES} ${CMAKE_SOURCE_DIR}/tools/embed_data.py ${CMAKE_SOURCE_DIR}/tools/orunners_rom.py
	COMMENT "Writing the game's data from the ROM set")
add_library(orunners_data STATIC ${ORUNNERS_DATA_SOURCES})
target_include_directories(orunners_data PUBLIC runtime/include)

# The game: its runtime plus its C, src/ (tools/decompile.py writes it; ORUNNERS_DECOMPILED_DIR picks
# another folder). Its own video and sound come back into it when their libraries draw the two screens and
# play the MultiPCM
set(ORUNNERS_DECOMPILED_DIR ${CMAKE_SOURCE_DIR}/src CACHE PATH "The game's C (tools/decompile.py)")
file(GLOB_RECURSE ORUNNERS_DECOMPILED CONFIGURE_DEPENDS ${ORUNNERS_DECOMPILED_DIR}/*.c)
if(ORUNNERS_DECOMPILED)
	add_library(orunners_game STATIC runtime/src/game.c ${ORUNNERS_DECOMPILED})
	target_include_directories(orunners_game PUBLIC runtime/include ${ORUNNERS_DECOMPILED_DIR})
	target_link_libraries(orunners_game PUBLIC s32runtime)
	if(MSVC)
		target_compile_options(orunners_game PRIVATE /bigobj)
	else()
		# variables are typed lvalues over the work RAM bytes (game/support.h)
		target_compile_options(orunners_game PRIVATE -fno-strict-aliasing)
	endif()

	add_executable(orunners_game_headless ports/headless/main.c)
	target_link_libraries(orunners_game_headless PRIVATE orunners_game orunners_data)
else()
	message(STATUS "No game C in ${ORUNNERS_DECOMPILED_DIR}: run tools/decompile.py")
endif()

# The translation scaffold: the V60 runtime plus the C tools/recompile.py writes from the user's ROM into
# build/generated, instruction by instruction (to run the program natively before the decompiler exists)
option(ORUNNERS_SCAFFOLD "Build the V70 translation scaffold (orunners_headless)" OFF)
file(GLOB ORUNNERS_GENERATED CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/build/generated/*.c)
if(ORUNNERS_SCAFFOLD AND ORUNNERS_GENERATED)
	add_library(orunners_program STATIC runtime/src/v60.c ${ORUNNERS_GENERATED})
	target_include_directories(orunners_program PUBLIC runtime/include build/generated)
	target_link_libraries(orunners_program PUBLIC s32runtime)
	if(MSVC)
		target_compile_options(orunners_program PRIVATE /bigobj)
	endif()

	add_executable(orunners_headless ports/headless/main.c)
	target_link_libraries(orunners_headless PRIVATE orunners_program orunners_data)
elseif(ORUNNERS_SCAFFOLD)
	message(STATUS "No generated program yet: run tools/recompile.py to build the scaffold")
endif()

# Development tools built on MAME's code; never linked into the game
option(ORUNNERS_REFERENCE_TOOLS "Build the MAME-based reference tools" ON)
if(ORUNNERS_REFERENCE_TOOLS)
	# MAME's V60 disassembler (the V70's instructions are the V60's), the reference
	# tools/v60/disassembler.py is compared with
	add_executable(oracle
		tools/reference/oracle.cpp
		tools/reference/mame/v60d.cpp
	)
	target_include_directories(oracle PRIVATE tools/reference/stub tools/reference/mame)

	# MAME's Z80 disassembler, the reference tools/z80/disassembler.py is compared with
	add_executable(oracle_z80
		tools/reference/oracle_z80.cpp
		tools/reference/mame/z80/z80dasm.cpp
	)
	target_include_directories(oracle_z80 PRIVATE tools/reference/stub tools/reference/mame/z80)

	# MAME's Multi 32 video drawing the board model's two screens: the frames the game's video has to match
	add_library(s32video_reference STATIC tools/reference/video/s32video.cpp)
	target_include_directories(s32video_reference PUBLIC tools/reference/video PRIVATE runtime/include)
	if(MSVC)
		target_compile_options(s32video_reference PRIVATE /wd4244 /wd4267 /wd4100 /wd4146)
	endif()

	# MAME's Z80 and MultiPCM and ymfm's YM3438 on the board model: the sound the game's has to match. The
	# Z80 core's steps are written from z80.lst at build time, as MAME does
	set(ORUNNERS_Z80_STEPS ${CMAKE_BINARY_DIR}/generated/cpu/z80/z80.hxx)
	add_custom_command(OUTPUT ${ORUNNERS_Z80_STEPS}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated/cpu/z80
		COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/tools/reference/mame/z80/z80make.py z80
			${CMAKE_SOURCE_DIR}/tools/reference/mame/z80/z80.lst ${ORUNNERS_Z80_STEPS}
		DEPENDS tools/reference/mame/z80/z80make.py tools/reference/mame/z80/z80.lst
		VERBATIM)
	add_library(s32sound_reference STATIC
		tools/reference/sound/s32sound.cpp
		tools/reference/ymfm/ymfm_opn.cpp
		tools/reference/ymfm/ymfm_adpcm.cpp
		tools/reference/ymfm/ymfm_ssg.cpp
		${ORUNNERS_Z80_STEPS}
	)
	target_include_directories(s32sound_reference PUBLIC tools/reference/sound
		PRIVATE tools/reference/ymfm runtime/include ${CMAKE_BINARY_DIR}/generated)
	if(MSVC)
		target_compile_options(s32sound_reference PRIVATE /bigobj /wd4244 /wd4267 /wd4100 /wd4146 /wd4127)
	endif()

	add_executable(s32ref
		tools/reference/s32ref.cpp
		tools/reference/mame/v60.cpp
		tools/reference/mame/v60d.cpp
	)
	target_include_directories(s32ref PRIVATE tools/reference/stub tools/reference/mame)
	target_link_libraries(s32ref PRIVATE s32runtime orunners_data s32video_reference s32sound_reference)
	if(MSVC)
		# MAME's core marks error handlers [[noreturn]] even though they return uint32_t
		target_compile_options(s32ref PRIVATE /wd4646 /wd4244)
	endif()
endif()
