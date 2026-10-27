/**
 *    Name:        png.h
 *    Description: reads the indexed PNGs of the game's assets (the tilesets, the sprite sheets): their pens
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A PNG of palette indexes (color type 3) or of gray levels (0), 1, 2, 4 or 8 bits a pixel, not
 *    interlaced: every pixel's index, a byte each, row after row. The palette is not read: the pens are
 *    what the game takes (its colors come from its own palettes).
 */
#ifndef GAME_PNG_H
#define GAME_PNG_H

#include <stdint.h>

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint8_t *pixels;    /* width * height indexes; png_free frees them */
} png_image;

/* The PNG at 'path' into 'image': NULL, or what is wrong with it */
const char *png_read(const char *path, png_image *image);
void png_free(png_image *image);

#endif
