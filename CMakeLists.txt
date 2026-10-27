# Name:        CMakeLists.txt
# Description: build of the OutRunners runtime and its development tools
# Version:     0.1.0
# Created:     11-10-2026
# Modified:    27-10-2026
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

# The packages a player downloads (.github/workflows/build.yml) carry SDL2 inside the window port, built
# from its own source
option(ORUNNERS_STATIC_SDL2 "Build SDL2 from its source into the executables" OFF)

# A development aid: everything built with AddressSanitizer, which stops the game at a read past a piece of
# its assets (each piece is a block of its own, runtime/src/assets.c: the code reads a table only inside it)
option(ORUNNERS_ASAN "Build with AddressSanitizer" OFF)
if(ORUNNERS_ASAN)
	if(MSVC)
		add_compile_options(/fsanitize=address /Zi)
		add_link_options(/DEBUG /INCREMENTAL:NO)
	else()
		add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
		add_link_options(-fsanitize=address)
	endif()
endif()

if(MSVC)
	add_compile_options(/W3 /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	# with ORUNNERS_STATIC_SDL2 the C runtime goes inside the executables too: they need no Visual C++
	# redistributable on the player's machine
	if(ORUNNERS_STATIC_SDL2)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)    # (SDL2's targets too)
	endif()
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
if(UNIX AND NOT APPLE)
	target_link_libraries(s32runtime PUBLIC m)    # (the C library's maths are apart there)
endif()

# The game's data: no executable carries the ROM or anything from it. The game loads its assets when it
# starts (runtime/src/assets.c): the files extract_assets.py takes out of the player's ROM set (rom/, its
# files or its .zip; ORUNNERS_ROM picks another) into assets/, in formats the usual tools open, where it
# looks for them (beside the executable, or the current folder's). The reference and the scaffold run the
# original code, so they read the ROM set themselves when they start (runtime/src/rom_set.c). The ROM never
# goes into the repository. Without a ROM set (a build for another machine) nothing of the game's is made,
# only the executables
find_package(Python3 COMPONENTS Interpreter)
set(ORUNNERS_ROM ${CMAKE_SOURCE_DIR}/rom CACHE PATH "The ROM set the assets are taken out of")
set(ORUNNERS_ROM_FILES "")
if(IS_DIRECTORY ${ORUNNERS_ROM})
	file(GLOB ORUNNERS_ROM_FILES ${ORUNNERS_ROM}/*)
elseif(EXISTS ${ORUNNERS_ROM})
	set(ORUNNERS_ROM_FILES ${ORUNNERS_ROM})
endif()
add_library(orunners_data STATIC runtime/src/game_data.c runtime/src/rom_set.c runtime/src/inflate.c)
target_include_directories(orunners_data PUBLIC runtime/include)
if(ORUNNERS_ROM_FILES AND Python3_FOUND)
	set(ORUNNERS_ASSETS_STAMP ${CMAKE_SOURCE_DIR}/assets/.extracted)
	add_custom_command(OUTPUT ${ORUNNERS_ASSETS_STAMP}
		COMMAND Python3::Interpreter ${CMAKE_SOURCE_DIR}/extract_assets.py ${ORUNNERS_ROM}
		DEPENDS ${ORUNNERS_ROM_FILES} ${CMAKE_SOURCE_DIR}/assets.json ${CMAKE_SOURCE_DIR}/extract_assets.py
		COMMENT "Taking the assets out of the ROM set")
	add_custom_target(orunners_assets ALL DEPENDS ${ORUNNERS_ASSETS_STAMP})
else()
	message(STATUS "No ROM set at ${ORUNNERS_ROM} (or no Python): no assets; the game needs them beside it")
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
	add_library(orunners_game STATIC runtime/src/game.c runtime/src/assets.c runtime/src/assets_media.c
		runtime/src/assets_midi.c runtime/src/json.c runtime/src/png.c runtime/src/inflate.c ${ORUNNERS_DECOMPILED})
	target_include_directories(orunners_game PUBLIC runtime/include ${ORUNNERS_DECOMPILED_DIR})
	target_link_libraries(orunners_game PUBLIC s32runtime)
	if(MSVC)
		target_compile_options(orunners_game PRIVATE /bigobj)
	else()
		# variables are typed lvalues over the work RAM bytes (game/support.h)
		target_compile_options(orunners_game PRIVATE -fno-strict-aliasing)
	endif()

	# a development aid: every mem_read and mem_write of the game's C counted by its line
	# (orunners_game_headless --memory-census FILE; runtime/src/memory_census.c)
	option(ORUNNERS_MEMORY_CENSUS "Count the game's memory accesses by line of C" OFF)
	if(ORUNNERS_MEMORY_CENSUS)
		target_compile_definitions(orunners_game PUBLIC S32_MEMORY_CENSUS)
	endif()

	add_executable(orunners_game_headless ports/headless/main.c)
	target_link_libraries(orunners_game_headless PRIVATE orunners_game orunners_sound)

	# The game in a window (ports/sdl2): SDL2 from the system, or on Windows the development package
	# unpacked in build/downloads/sdl2 (SDL2_DIR picks another), a library beside the executable; or, with
	# ORUNNERS_STATIC_SDL2, SDL2 built from its own source into the executable
	if(ORUNNERS_STATIC_SDL2)
		include(FetchContent)
		FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.32.10
			GIT_SHALLOW TRUE)
		set(SDL_SHARED OFF CACHE BOOL "" FORCE)
		set(SDL_STATIC ON CACHE BOOL "" FORCE)
		set(SDL_TEST OFF CACHE BOOL "" FORCE)
		set(SDL_FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)    # (MSVC: the C runtime inside, as the game's)
		FetchContent_MakeAvailable(SDL2)
		set(ORUNNERS_SDL2 SDL2::SDL2-static)
		set(SDL2_FOUND TRUE)
	else()
		set(ORUNNERS_SDL2_UNPACKED "")
		if(WIN32)    # (the VC package: its libraries are Windows' only, a source tree shared with WSL has it)
			file(GLOB ORUNNERS_SDL2_UNPACKED ${CMAKE_SOURCE_DIR}/build/downloads/sdl2/SDL2-*/cmake)
		endif()
		find_package(SDL2 CONFIG QUIET HINTS ${ORUNNERS_SDL2_UNPACKED})
		set(ORUNNERS_SDL2 SDL2::SDL2)
	endif()
	if(TARGET SDL2::SDL2main)
		list(APPEND ORUNNERS_SDL2 SDL2::SDL2main)
	endif()
	if(SDL2_FOUND)
		# (an .app on macOS: SDL_GetBasePath is its Resources folder)
		add_executable(orunners WIN32 MACOSX_BUNDLE ports/sdl2/main.c)
		target_link_libraries(orunners PRIVATE orunners_game orunners_sound ${ORUNNERS_SDL2})
		set_target_properties(orunners PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "OutRunners"
			MACOSX_BUNDLE_GUI_IDENTIFIER io.github.cheviman.outrunners-decomp)
		if(WIN32 AND NOT ORUNNERS_STATIC_SDL2)
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
	# the reference on MAME's cores (arcade-decomp's system32/reference: s32ref, the disassemblers' oracles,
	# pcmcheck), with OutRunners' data library
	set(S32_GAME_DATA orunners_data)
	include(${CMAKE_SOURCE_DIR}/tools/core/system32/reference/reference.cmake)
endif()
