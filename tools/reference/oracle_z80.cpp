/**
 *    Name:        oracle_z80.cpp
 *    Description: runs MAME's own Z80 disassembler over an image, one line per offset
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: oracle_z80 <image.bin> <first offset> <end offset> <output.txt>
 *    Output: "AAAAA LEN TEXT" per offset, the same format compare_z80_with_mame.py produces.
 *    The image is decoded as one linear block: MAME gets the offset as pc and reads the bytes that
 *    follow it, wrapping at the end of the image. MAME only uses pc for the target of a relative jump,
 *    which it masks to 16 bits, so every line is what the Z80 sees at offset & 0xFFFF.
 */
#include "emu.h"
#include "z80dasm.h"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::fprintf(stderr, "Usage: oracle_z80 <image.bin> <first offset> <end offset> <output.txt>\n");
		return 1;
	}

	std::ifstream input(argv[1], std::ios::binary);
	std::vector<u8> image((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	if (image.empty() || (image.size() & (image.size() - 1)) != 0)
	{
		std::fprintf(stderr, "The image must be a non-empty power-of-two size\n");
		return 1;
	}

	const u32 first = u32(std::strtoul(argv[2], nullptr, 0));
	const u32 end = u32(std::strtoul(argv[3], nullptr, 0));
	const util::disasm_interface::data_buffer buffer(image.data(), u32(image.size() - 1));
	z80_disassembler disassembler;

	std::ofstream output(argv[4], std::ios::binary);
	if (!output)
	{
		std::fprintf(stderr, "%s could not be written\n", argv[4]);
		return 1;
	}
	char prefix[32];
	for (u32 offset = first; offset < end; offset++)
	{
		std::ostringstream text;
		const u32 result = disassembler.disassemble(text, offset, buffer, buffer);
		std::snprintf(prefix, sizeof(prefix), "%05X %u ", offset, result & util::disasm_interface::LENGTHMASK);
		output << prefix << text.str() << '\n';
	}
	return 0;
}
