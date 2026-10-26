# Name:        CMakeLists.txt
# Description: build of the OutRunners runtime and its development tools
# Version:     0.1.0
# Created:     11-10-2026
# Modified:    21-10-2026
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

# Multi 32 board model (script.c drives the controls in test runs; video.c draws the two screens; sound.c
# is the Z80's side of the board and sound_chips.c its YM3438 and MultiPCM)
add_library(s32runtime STATIC
	runtime/src/machine.c
	runtime/src/io.c
	runtime/src/script.c
	runtime/src/sound.c
	runtime/src/ym3438.c
	runtime/src/memory_census.c
	runtime/src/tasks.c
	runtime/src/video.c
	runtime/src/multipcm.c
	runtime/src/sound_chips.c
)
target_include_directories(s32runtime PUBLIC runtime/include)

# The game's data: no executable carries the ROM or anything from it. The game loads its assets pack when
# it starts (runtime/src/game_data.c): tools/make_pack.py writes it beside the executables from assets/,
# which extract_assets.py takes out of the player's ROM set (rom/, its files or its .zip; ORUNNERS_ROM picks
# another); a change to an asset goes into the pack. The reference and the scaffold run the original code,
# so they read the ROM set themselves when they start (runtime/src/rom_set.c). The ROM never goes into the
# repository. Without a ROM set (a build for another machine) nothing of the game's is made, only the
# executables
find_package(Python3 COMPONENTS Interpreter)
set(ORUNNERS_ROM ${CMAKE_SOURCE_DIR}/rom CACHE PATH "The ROM set the assets are taken out of")
set(ORUNNERS_ROM_FILES "")
if(IS_DIRECTORY ${ORUNNERS_ROM})
	file(GLOB ORUNNERS_ROM_FILES ${ORUNNERS_ROM}/*)
elseif(EXISTS ${ORUNNERS_ROM})
	set(ORUNNERS_ROM_FILES ${ORUNNERS_ROM})
endif()
add_library(orunners_data STATIC runtime/src/game_data.c runtime/src/rom_set.c runtime/src/inflate.c
	runtime/src/extract.c runtime/src/pack_layout.c)
target_include_directories(orunners_data PUBLIC runtime/include)
if(ORUNNERS_ROM_FILES AND Python3_FOUND)
	file(GLOB_RECURSE ORUNNERS_ASSET_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/assets/*)
	file(GLOB ORUNNERS_FORMAT_TOOLS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tools/formats/*.py)
	set(ORUNNERS_ASSETS_PACK ${ORUNNERS_BIN_DIR}/orunners.assets)
	add_custom_command(OUTPUT ${ORUNNERS_ASSETS_PACK}
		COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/extract_assets.py ${ORUNNERS_ROM}
		COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/tools/make_pack.py ${ORUNNERS_ASSETS_PACK}
		DEPENDS ${ORUNNERS_ROM_FILES} ${ORUNNERS_ASSET_FILES} ${ORUNNERS_FORMAT_TOOLS} ${CMAKE_SOURCE_DIR}/assets.json
			${CMAKE_SOURCE_DIR}/extract_assets.py ${CMAKE_SOURCE_DIR}/tools/make_pack.py
			${CMAKE_SOURCE_DIR}/tools/orunners_rom.py ${CMAKE_SOURCE_DIR}/tools/asset_formats.py
			${CMAKE_SOURCE_DIR}/tools/data_formats.py
		COMMENT "Taking the assets out of the ROM set and packing them for the game")
	add_custom_target(orunners_assets ALL DEPENDS ${ORUNNERS_ASSETS_PACK})
	add_dependencies(orunners_data orunners_assets)
else()
	message(STATUS "No ROM set at ${ORUNNERS_ROM} (or no Python): no assets pack; the game needs one beside it")
endif()

# The game: its runtime plus its C, src/ (tools/decompile.py writes it; ORUNNERS_DECOMPILED_DIR picks
# another folder). Its own video and sound come back into it when their libraries draw the two screens and
# play the MultiPCM
set(ORUNNERS_DECOMPILED_DIR ${CMAKE_SOURCE_DIR}/src CACHE PATH "The game's C (tools/decompile.py)")
file(GLOB_RECURSE ORUNNERS_DECOMPILED CONFIGURE_DEPENDS ${ORUNNERS_DECOMPILED_DIR}/*.c)
list(FILTER ORUNNERS_DECOMPILED EXCLUDE REGEX "/sound/[^/]*\\.c$")    # (the sound program's: a library of its own)

# The sound program: the Z80's C (src/sound/, tools/decompile_sound.py) on the board's time
# (runtime/src/sound_cpu.c). The decompiled game and the scaffold both play it
file(GLOB ORUNNERS_SOUND_DECOMPILED CONFIGURE_DEPENDS ${ORUNNERS_DECOMPILED_DIR}/sound/*.c)
if(ORUNNERS_SOUND_DECOMPILED)
	add_library(orunners_sound STATIC runtime/src/sound_cpu.c ${ORUNNERS_SOUND_DECOMPILED})
	target_include_directories(orunners_sound PUBLIC runtime/include PRIVATE ${ORUNNERS_DECOMPILED_DIR}/sound)
	target_link_libraries(orunners_sound PUBLIC s32runtime)
	if(MSVC)
		target_compile_options(orunners_sound PRIVATE /bigobj)
	else()
		target_compile_options(orunners_sound PRIVATE -fno-strict-aliasing)
	endif()
else()
	message(STATUS "No sound program C in ${ORUNNERS_DECOMPILED_DIR}/sound")
endif()

if(ORUNNERS_DECOMPILED AND ORUNNERS_SOUND_DECOMPILED)
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
	target_link_libraries(orunners_game_headless PRIVATE orunners_game orunners_sound orunners_data)

	# The game in a window (ports/sdl2): SDL2 from the system, or on Windows the development package
	# unpacked in build/downloads/sdl2 (SDL2_DIR picks another), a library beside the executable
	set(ORUNNERS_SDL2_UNPACKED "")
	if(WIN32)    # (the VC package: its libraries are Windows' only)
		file(GLOB ORUNNERS_SDL2_UNPACKED ${CMAKE_SOURCE_DIR}/build/downloads/sdl2/SDL2-*/cmake)
	endif()
	find_package(SDL2 CONFIG QUIET HINTS ${ORUNNERS_SDL2_UNPACKED})
	if(SDL2_FOUND)
		set(ORUNNERS_SDL2 SDL2::SDL2)
		if(TARGET SDL2::SDL2main)
			list(APPEND ORUNNERS_SDL2 SDL2::SDL2main)
		endif()
		add_executable(orunners WIN32 ports/sdl2/main.c)
		target_link_libraries(orunners PRIVATE orunners_game orunners_sound orunners_data ${ORUNNERS_SDL2})
		if(WIN32)
			add_custom_command(TARGET orunners POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:orunners>)
		endif()
	else()
		message(STATUS "No SDL2: the window port (orunners) is not built")
	endif()
else()
	message(STATUS "No game C in ${ORUNNERS_DECOMPILED_DIR}")
endif()

# The translation scaffold: the V60 runtime plus the C tools/recompile.py writes from the user's ROM into
# build/generated, instruction by instruction (to run the program natively before the decompiler exists)
option(ORUNNERS_SCAFFOLD "Build the V70 translation scaffold (orunners_headless)" OFF)
file(GLOB ORUNNERS_GENERATED CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/build/generated/*.c)
if(ORUNNERS_SCAFFOLD AND ORUNNERS_GENERATED AND ORUNNERS_SOUND_DECOMPILED)
	add_library(orunners_program STATIC runtime/src/v60.c ${ORUNNERS_GENERATED})
	target_include_directories(orunners_program PUBLIC runtime/include build/generated)
	target_link_libraries(orunners_program PUBLIC s32runtime)
	if(MSVC)
		target_compile_options(orunners_program PRIVATE /bigobj)
	endif()

	add_executable(orunners_headless ports/headless/main.c)
	target_compile_definitions(orunners_headless PRIVATE ORUNNERS_SCAFFOLD)
	target_link_libraries(orunners_headless PRIVATE orunners_program orunners_sound orunners_data)
elseif(ORUNNERS_SCAFFOLD)
	message(STATUS "No generated program in build/generated: no scaffold")
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

	# the game's MultiPCM against MAME's, sample for sample, over what s32ref --sound-log records
	add_executable(pcmcheck tools/reference/sound/pcmcheck.cpp)
	target_include_directories(pcmcheck PRIVATE tools/reference/sound runtime/include)
	target_link_libraries(pcmcheck PRIVATE s32runtime orunners_data)
	if(MSVC)
		target_compile_options(pcmcheck PRIVATE /wd4244 /wd4267 /wd4100 /wd4146)
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
