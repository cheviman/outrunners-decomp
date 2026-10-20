/**
 *    Name:        oracle.cpp
 *    Description: runs MAME's own V60 disassembler over an image, one line per address
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    11-10-2026
 *    Author:      Sebastian Toyos
 *
 *    Usage: oracle <image.bin> <first address> <end address> <output.txt>
 *    Output: "AAAAAA LEN TEXT" per address, the same format compare_with_mame.py produces.
 */
#include "emu.h"
#include "v60d.h"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		std::fprintf(stderr, "Usage: oracle <image.bin> <first address> <end address> <output.txt>\n");
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
	v60_disassembler disassembler;

	std::ofstream output(argv[4], std::ios::binary);
	char prefix[32];
	for (u32 address = first; address < end; address++)
	{
		std::ostringstream text;
		const u32 result = disassembler.disassemble(text, address, buffer, buffer);
		std::snprintf(prefix, sizeof(prefix), "%06X %u ", address, result & util::disasm_interface::LENGTHMASK);
		output << prefix << text.str() << '\n';
	}
	return 0;
}
