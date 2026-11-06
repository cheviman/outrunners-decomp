# Name:        powerpc-linux-gnu.cmake
# Description: a big-endian 32-bit build of the game, to check the C on a processor unlike the PC's: PowerPC
#              Linux (Debian and Ubuntu's gcc-powerpc-linux-gnu), linked static so that qemu-ppc runs it as it is
# Version:     0.1.0
# Created:     06-11-2026
# Modified:    06-11-2026
# Author:      Sebastian Toyos
#
# cmake -S . -B build/ppc -DCMAKE_TOOLCHAIN_FILE=cmake/powerpc-linux-gnu.cmake -DCMAKE_BUILD_TYPE=Release
#       -DORUNNERS_REFERENCE_TOOLS=OFF -DORUNNERS_BIN_DIR=build/bin-ppc
# cmake --build build/ppc --target orunners_game_headless
# qemu-ppc build/bin-ppc/orunners_game_headless --script tests/controls/coin_start.txt --video-hashes ...
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR powerpc)
set(CMAKE_C_COMPILER powerpc-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER powerpc-linux-gnu-g++)
set(CMAKE_FIND_ROOT_PATH /usr/powerpc-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")
set(CMAKE_CROSSCOMPILING_EMULATOR qemu-ppc)
