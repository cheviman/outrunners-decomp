/**
 *    Name:        png.c
 *    Description: reads the indexed PNGs of the game's assets (png.h)
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The format is the W3C's (Portable Network Graphics, second edition): the chunks, the zlib stream of
 *    the IDAT chunks (inflate.c, past its two-byte header), and the five filters of each row.
 */
#include "png.h"

#include "inflate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t big32(const uint8_t *bytes)
{
	return (uint32_t)bytes[0] << 24 | (uint32_t)bytes[1] << 16 | (uint32_t)bytes[2] << 8 | bytes[3];
}

static uint8_t *read_whole(const char *path, size_t *size)
{
	FILE *input = fopen(path, "rb");
	long length;
	uint8_t *data = NULL;

	if (!input)
		return NULL;
	if (fseek(input, 0, SEEK_END) == 0 && (length = ftell(input)) > 0 && fseek(input, 0, SEEK_SET) == 0
		&& (data = malloc((size_t)length)) != NULL && fread(data, 1, (size_t)length, input) != (size_t)length)
	{
		free(data);
		data = NULL;
	}
	if (data)
		*size = (size_t)length;
	fclose(input);
	return data;
}

static int paeth(int left, int up, int up_left)
{
	const int estimate = left + up - up_left;
	const int to_left = abs(estimate - left), to_up = abs(estimate - up), to_up_left = abs(estimate - up_left);

	if (to_left <= to_up && to_left <= to_up_left)
		return left;
	return to_up <= to_up_left ? up : up_left;
}

/* Undoes the filters of 'rows' rows of 'stride' bytes (each after its filter byte), in place */
static int unfilter(uint8_t *data, uint32_t rows, size_t stride, int bytes_per_pixel)
{
	uint8_t *previous = NULL;

	for (uint32_t row = 0; row < rows; row++)
	{
		uint8_t *line = data + row * (stride + 1);
		const uint8_t filter = line[0];
		uint8_t *pixels = line + 1;

		for (size_t at = 0; at < stride; at++)
		{
			const int left = at >= (size_t)bytes_per_pixel ? pixels[at - bytes_per_pixel] : 0;
			const int up = previous ? previous[at] : 0;
			const int up_left = (previous && at >= (size_t)bytes_per_pixel) ? previous[at - bytes_per_pixel] : 0;

			switch (filter)
			{
			case 0: break;
			case 1: pixels[at] = (uint8_t)(pixels[at] + left); break;
			case 2: pixels[at] = (uint8_t)(pixels[at] + up); break;
			case 3: pixels[at] = (uint8_t)(pixels[at] + (left + up) / 2); break;
			case 4: pixels[at] = (uint8_t)(pixels[at] + paeth(left, up, up_left)); break;
			default: return 0;
			}
		}
		previous = pixels;
	}
	return 1;
}

const char *png_read(const char *path, png_image *image)
{
	static const uint8_t signature[8] = { 0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a };
	size_t size = 0, at = 8, packed_size = 0;
	uint8_t *file = read_whole(path, &size);
	uint8_t *packed = NULL, *rows = NULL;
	uint32_t width = 0, height = 0;
	int bits = 0, kind = -1;
	size_t stride, unpacked_size;
	const char *problem = NULL;

	memset(image, 0, sizeof(*image));
	if (!file)
		return "cannot be read";
	if (size < 8 || memcmp(file, signature, 8) != 0)
		problem = "is not a PNG";
	while (!problem && at + 12 <= size)
	{
		const uint32_t length = big32(file + at);
		const uint8_t *type = file + at + 4, *body = file + at + 8;

		if (length > size - at - 12)
		{
			problem = "is cut short";
			break;
		}
		if (memcmp(type, "IHDR", 4) == 0 && length >= 13)
		{
			width = big32(body);
			height = big32(body + 4);
			bits = body[8];
			kind = body[9];
			if (body[10] != 0 || body[11] != 0 || body[12] != 0)
				problem = "is interlaced or of a method the game does not read";
		}
		else if (memcmp(type, "IDAT", 4) == 0)
		{
			uint8_t *grown = realloc(packed, packed_size + length);

			if (!grown)
			{
				problem = "is too large";
				break;
			}
			packed = grown;
			memcpy(packed + packed_size, body, length);
			packed_size += length;
		}
		else if (memcmp(type, "IEND", 4) == 0)
			break;
		at += 12 + length;
	}
	if (!problem && ((kind != 3 && kind != 0) || (bits != 1 && bits != 2 && bits != 4 && bits != 8)))
		problem = "is not a PNG of palette indexes";
	if (!problem && (width == 0 || height == 0 || packed_size < 6))
		problem = "has no picture";
	if (!problem)
	{
		stride = ((size_t)width * (size_t)bits + 7) / 8;
		unpacked_size = (stride + 1) * height;
		rows = malloc(unpacked_size);
		image->pixels = malloc((size_t)width * height);
		/* (the zlib stream: its two-byte header, the DEFLATE stream, and its check) */
		if (!rows || !image->pixels)
			problem = "is too large";
		else if (!inflate_raw(packed + 2, packed_size - 6, rows, unpacked_size) || !unfilter(rows, height, stride, 1))
			problem = "is damaged";
	}
	if (!problem)
	{
		const int per_byte = 8 / bits;
		const int mask = (1 << bits) - 1;

		for (uint32_t y = 0; y < height; y++)
		{
			const uint8_t *line = rows + y * (stride + 1) + 1;

			for (uint32_t x = 0; x < width; x++)
				image->pixels[y * width + x] = (uint8_t)((line[x / per_byte] >> ((per_byte - 1 - x % per_byte) * bits)) & mask);
		}
		image->width = width;
		image->height = height;
	}
	free(file);
	free(packed);
	free(rows);
	if (problem)
		png_free(image);
	return problem;
}

void png_free(png_image *image)
{
	free(image->pixels);
	memset(image, 0, sizeof(*image));
}
