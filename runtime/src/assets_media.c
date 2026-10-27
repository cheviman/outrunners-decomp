/**
 *    Name:        assets_media.c
 *    Description: loads the media of the game's assets: the tilesets, the sprite sheets and the MultiPCM's
 *                 samples with their headers (game/assets.h)
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The files are the extractor's (extract_assets.py):
 *    - assets/tilesets: tilesets.txt (a tileset a line: its name, then the tile numbers it holds as hex
 *      ranges FIRST-LAST, in the order its PNG lays them out, 32 a row), and NAME.png, 8-bit indexes whose
 *      low 4 bits are the pens (tools/tilesets.py);
 *    - assets/sprites: sheets.txt (the sheets, a line each), NAME.png and NAME.json (its 'bits', 4 or 8, and
 *      its 'frames': where each piece is in the PNG, x, y, width, height, and the place of its graphics in
 *      the sprite chips, 'word': bank << 20 | offset, in 32-bit words). A piece is rows of words, each word
 *      8 pens of 4 bits or 4 of 8, the first pen in its top bits; a 4-bit sheet's index holds the pen in its
 *      low 4 bits (the rest are the sheet's palette slots);
 *    - assets/sound/samples: samples.txt (where each sample is in the MultiPCM's 4 MB: 'OFFSET LENGTH NAME',
 *      and 'page P sets S...': the sound sets that put page P, the 512 KB from P x 80000, in a window of
 *      the chip), NAME.wav (mono, 8 bits unsigned or 16 bits; the chip's bytes are signed), and headers.txt
 *      (the 512 sample headers, tools/pcm_samples.py: their fields by name and the sample each one plays).
 *    The graphics and the samples are kept by the places the game's numbers give them (a tile number, a
 *    place of the sprite chips, a place of the MultiPCM's address space): what no asset holds reads as the
 *    board's empty chips do, all ones.
 */
#include "game/assets.h"

#include "assets_internal.h"
#include "json.h"
#include "png.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_COUNT     0x8000u
#define TILESET_ROW    32u           /* tiles a row of a tileset's PNG */
#define PCM_SIZE       0x400000u
#define PCM_HEADERS    0x200u
#define PCM_HEADER     12u
#define PCM_FIXED      0x100000u     /* the chip's addresses below it are its own; above, two windows */
#define PCM_WINDOW     0x80000u
#define PCM_SETS       32

/* ---- lines of a text ---- */

/* The words of the line at 'line' (up to a '#' or its end) into 'words': how many */
static int line_words(char *line, char **words, int room)
{
	int count = 0;
	char *hash = strchr(line, '#');

	if (hash)
		*hash = 0;
	for (char *at = strtok(line, " \t\r"); at && count < room; at = strtok(NULL, " \t\r"))
		words[count++] = at;
	return count;
}

/* The lines of 'text' one after the other: each call hands the next one (cut in place), NULL at the end */
static char *next_text_line(char **at)
{
	char *line = *at;
	char *end;

	if (!line || !*line)
		return NULL;
	end = strchr(line, '\n');
	if (end)
	{
		*end = 0;
		*at = end + 1;
	}
	else
		*at = line + strlen(line);
	return line;
}

/* ---- the tilesets ---- */

static const uint8_t *tile_pens[TILE_COUNT];

int assets_read_tilesets(const char *folder)
{
	char *text = assets_read_file(assets_path(folder, "tilesets/tilesets.txt"));
	char *cursor = text;
	char *line;
	uint32_t filled = 0;

	if (!text)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, "tilesets/tilesets.txt"));
		return 0;
	}
	while ((line = next_text_line(&cursor)) != NULL)
	{
		char *words[256];
		const int count = line_words(line, words, 256);
		uint32_t tiles = 0, index = 0;
		char png_path[512];
		png_image image;
		const char *wrong;
		uint8_t *pens;

		if (count < 2)
			continue;
		for (int word = 1; word < count; word++)
		{
			const unsigned long first = strtoul(words[word], NULL, 16);
			const char *dash = strchr(words[word], '-');
			const unsigned long last = dash ? strtoul(dash + 1, NULL, 16) : first;

			if (last < first || last >= TILE_COUNT)
			{
				snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the tileset %s has tiles the board has not", words[0]);
				free(text);
				return 0;
			}
			tiles += (uint32_t)(last - first + 1);
		}
		snprintf(png_path, sizeof(png_path), "tilesets/%s.png", words[0]);
		if ((wrong = png_read(assets_path(folder, png_path), &image)) != NULL)
		{
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s %s", assets_path(folder, png_path), wrong);
			free(text);
			return 0;
		}
		if (image.width != TILESET_ROW * 16 || image.height < (tiles + TILESET_ROW - 1) / TILESET_ROW * 16
			|| !(pens = malloc((size_t)tiles * 256)))
		{
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s is not as large as its tiles", assets_path(folder, png_path));
			png_free(&image);
			free(text);
			return 0;
		}
		for (int word = 1; word < count; word++)
		{
			const uint32_t first = (uint32_t)strtoul(words[word], NULL, 16);
			const char *dash = strchr(words[word], '-');
			const uint32_t last = dash ? (uint32_t)strtoul(dash + 1, NULL, 16) : first;

			for (uint32_t tile = first; tile <= last; tile++, index++)
			{
				uint8_t *tile_out = &pens[(size_t)index * 256];
				const uint32_t left = index % TILESET_ROW * 16, top = index / TILESET_ROW * 16;

				for (int y = 0; y < 16; y++)
					for (int x = 0; x < 16; x++)
						tile_out[y * 16 + x] = image.pixels[(size_t)(top + y) * image.width + left + x] & 0x0f;
				if (tile_pens[tile])
				{
					snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "tile %04X is in two tilesets", tile);
					png_free(&image);
					free(text);
					return 0;
				}
				tile_pens[tile] = tile_out;
				filled++;
			}
		}
		png_free(&image);
	}
	free(text);
	if (filled != TILE_COUNT)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the tilesets hold %u of the board's %u tiles", filled, TILE_COUNT);
		return 0;
	}
	return 1;
}

/* ---- the sprite sheets ---- */

typedef struct
{
	uint32_t first;
	uint32_t count;
	uint32_t *words;
} sprite_block;

typedef struct
{
	int sheet;
	int bits;
	uint32_t x, y, width, height;
	uint32_t word, words;
} sprite_piece;

static sprite_block *sprite_blocks;
static size_t sprite_block_count;
static size_t last_block;

static int by_word(const void *first, const void *second)
{
	const uint32_t one = ((const sprite_piece *)first)->word, other = ((const sprite_piece *)second)->word;

	return one < other ? -1 : one > other;
}

/* The block holding 'place', or sprite_block_count when none does */
static size_t block_of(uint32_t place)
{
	size_t low = 0, high = sprite_block_count;

	if (last_block < sprite_block_count && place - sprite_blocks[last_block].first < sprite_blocks[last_block].count)
		return last_block;
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (sprite_blocks[middle].first <= place)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0 || place - sprite_blocks[low - 1].first >= sprite_blocks[low - 1].count)
		return sprite_block_count;
	return last_block = low - 1;
}

static uint32_t sprite_word(uint32_t place)
{
	const size_t block = block_of(place);

	/* (no chip holds graphics where no block is) */
	return block < sprite_block_count ? sprite_blocks[block].words[place - sprite_blocks[block].first] : 0xffffffffu;
}

/* The pieces of the sheet 'name' (its JSON) added to 'pieces' */
static int sheet_pieces(const char *folder, const char *name, int sheet, sprite_piece **pieces, size_t *count,
	size_t *room)
{
	char json_path[512];
	char *text;
	json_value *document;
	const json_value *frames, *bits;
	char error[128];

	snprintf(json_path, sizeof(json_path), "sprites/%s.json", name);
	if (!(text = assets_read_file(assets_path(folder, json_path))))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, json_path));
		return 0;
	}
	document = json_parse(text, error, sizeof(error));
	free(text);
	frames = json_member(document, "frames");
	bits = json_member(document, "bits");
	if (!frames || frames->kind != JSON_ARRAY || !bits || (bits->number != 4 && bits->number != 8))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read%s%s", json_path, document ? "" : ": ", document ? "" : error);
		json_free(document);
		return 0;
	}
	for (size_t index = 0; index < frames->count; index++)
	{
		const json_value *frame = &frames->items[index];
		const json_value *x = json_member(frame, "x"), *y = json_member(frame, "y");
		const json_value *width = json_member(frame, "width"), *height = json_member(frame, "height");
		const char *word = json_text(frame, "word");
		sprite_piece *piece;

		if (!x || !y || !width || !height || !word)
		{
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s has a piece without its place", json_path);
			json_free(document);
			return 0;
		}
		if (*count == *room)
		{
			sprite_piece *grown;

			*room = *room ? *room * 2 : 4096;
			if (!(grown = realloc(*pieces, *room * sizeof(**pieces))))
			{
				json_free(document);
				return 0;
			}
			*pieces = grown;
		}
		piece = &(*pieces)[(*count)++];
		piece->sheet = sheet;
		piece->bits = (int)bits->number;
		piece->x = (uint32_t)x->number;
		piece->y = (uint32_t)y->number;
		piece->width = (uint32_t)width->number;
		piece->height = (uint32_t)height->number;
		piece->word = (uint32_t)strtoul(word, NULL, 16);
		piece->words = piece->width / (32u / (uint32_t)piece->bits) * piece->height;
	}
	json_free(document);
	return 1;
}

int assets_read_sprites(const char *folder)
{
	char *list = assets_read_file(assets_path(folder, "sprites/sheets.txt"));
	char *cursor = list;
	char *line;
	char **names = NULL;
	int sheet_count = 0, sheet_room = 0;
	sprite_piece *pieces = NULL;
	size_t piece_count = 0, piece_room = 0;
	int read = 1;

	if (!list)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, "sprites/sheets.txt"));
		return 0;
	}
	while (read && (line = next_text_line(&cursor)) != NULL)
	{
		char *words[2];

		if (line_words(line, words, 2) < 1)
			continue;
		if (sheet_count == sheet_room)
		{
			char **grown;

			sheet_room = sheet_room ? sheet_room * 2 : 256;
			if (!(grown = realloc(names, (size_t)sheet_room * sizeof(char *))))
				break;
			names = grown;
		}
		names[sheet_count] = words[0];
		read = sheet_pieces(folder, words[0], sheet_count, &pieces, &piece_count, &piece_room);
		sheet_count++;
	}
	/* the blocks: the places the pieces cover, one block a run of them that touch or overlap */
	if (read)
	{
		qsort(pieces, piece_count, sizeof(*pieces), by_word);
		sprite_blocks = calloc(piece_count ? piece_count : 1, sizeof(*sprite_blocks));
		read = sprite_blocks != NULL;
		for (size_t index = 0; read && index < piece_count; index++)
		{
			sprite_block *last = sprite_block_count ? &sprite_blocks[sprite_block_count - 1] : NULL;
			const uint32_t end = pieces[index].word + pieces[index].words;

			if (pieces[index].words == 0)
				continue;
			if (last && pieces[index].word <= last->first + last->count)
			{
				if (end > last->first + last->count)
					last->count = end - last->first;
				continue;
			}
			sprite_blocks[sprite_block_count].first = pieces[index].word;
			sprite_blocks[sprite_block_count].count = pieces[index].words;
			sprite_block_count++;
		}
		for (size_t block = 0; read && block < sprite_block_count; block++)
			read = (sprite_blocks[block].words = malloc((size_t)sprite_blocks[block].count * 4)) != NULL;
		if (!read)
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "no memory for the sprites");
	}
	/* each sheet's pieces into their blocks: their pens as the chip's words */
	for (int sheet = 0; read && sheet < sheet_count; sheet++)
	{
		char png_path[512];
		png_image image;
		const char *wrong;

		snprintf(png_path, sizeof(png_path), "sprites/%s.png", names[sheet]);
		if ((wrong = png_read(assets_path(folder, png_path), &image)) != NULL)
		{
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s %s", assets_path(folder, png_path), wrong);
			read = 0;
			break;
		}
		for (size_t index = 0; read && index < piece_count; index++)
		{
			const sprite_piece *piece = &pieces[index];
			const uint32_t per_word = 32u / (uint32_t)piece->bits;
			const uint32_t row_words = piece->width / per_word;
			const uint8_t mask = piece->bits == 4 ? 0x0f : 0xff;

			if (piece->sheet != sheet || piece->words == 0)
				continue;
			if (piece->x + piece->width > image.width || piece->y + piece->height > image.height)
			{
				snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s is smaller than its pieces", assets_path(folder, png_path));
				read = 0;
				break;
			}
			for (uint32_t row = 0; row < piece->height; row++)
				for (uint32_t column = 0; column < row_words; column++)
				{
					const uint8_t *pens = &image.pixels[(size_t)(piece->y + row) * image.width + piece->x + column * per_word];
					const uint32_t place = piece->word + row * row_words + column;
					uint32_t value = 0;

					for (uint32_t pen = 0; pen < per_word; pen++)
						value |= (uint32_t)(pens[pen] & mask) << (32 - (uint32_t)piece->bits * (pen + 1));
					sprite_blocks[block_of(place)].words[place - sprite_blocks[block_of(place)].first] = value;
				}
		}
		png_free(&image);
	}
	free(pieces);
	free(names);
	free(list);
	return read;
}

/* ---- the samples ---- */

typedef struct
{
	uint32_t offset;
	uint32_t length;
	const uint8_t *data;
	char *name;
} pcm_block;

static pcm_block *pcm_blocks;
static size_t pcm_block_count;
static size_t last_pcm;
static uint8_t pcm_headers[PCM_HEADERS * PCM_HEADER];
static int set_pages[PCM_SETS];

/* A WAV's samples as the chip's bytes (signed: the WAV's unsigned 8 bits, or the top of its 16): NULL when
   it is no mono PCM WAV */
static uint8_t *read_wav(const char *path, uint32_t *length)
{
	FILE *input = fopen(path, "rb");
	uint8_t head[12], chunk[8];
	int bits = 0, mono = 0;
	uint8_t *samples = NULL;

	if (!input)
		return NULL;
	if (fread(head, 1, 12, input) != 12 || memcmp(head, "RIFF", 4) != 0 || memcmp(head + 8, "WAVE", 4) != 0)
	{
		fclose(input);
		return NULL;
	}
	while (fread(chunk, 1, 8, input) == 8)
	{
		const uint32_t size = (uint32_t)chunk[4] | (uint32_t)chunk[5] << 8 | (uint32_t)chunk[6] << 16 | (uint32_t)chunk[7] << 24;

		if (memcmp(chunk, "fmt ", 4) == 0 && size >= 16)
		{
			uint8_t format[16];

			if (fread(format, 1, 16, input) != 16)
				break;
			mono = format[0] == 1 && format[1] == 0 && format[2] == 1 && format[3] == 0;
			bits = format[14];
			if (fseek(input, (long)(size - 16 + (size & 1)), SEEK_CUR) != 0)
				break;
		}
		else if (memcmp(chunk, "data", 4) == 0 && mono && (bits == 8 || bits == 16))
		{
			uint8_t *raw = malloc(size ? size : 1);

			if (!raw || fread(raw, 1, size, input) != size)
			{
				free(raw);
				break;
			}
			*length = bits == 8 ? size : size / 2;
			for (uint32_t at = 0; at < *length; at++)
				raw[at] = (uint8_t)((bits == 8 ? raw[at] : raw[at * 2 + 1] + 0x80) ^ 0x80);
			samples = raw;
			break;
		}
		else if (fseek(input, (long)(size + (size & 1)), SEEK_CUR) != 0)
			break;
	}
	fclose(input);
	return samples;
}

static int by_offset(const void *first, const void *second)
{
	const uint32_t one = ((const pcm_block *)first)->offset, other = ((const pcm_block *)second)->offset;

	return one < other ? -1 : one > other;
}

/* The place in the 4 MB of the sample 'name' (NAME or NAME+X) on one of 'pages' (a bit a page; 0: the chip's
   fixed part), plus X: -1 when no such sample is there */
static int64_t sample_place(const char *reference, unsigned pages)
{
	char name[256];
	const char *plus = strchr(reference, '+');
	const size_t length = plus ? (size_t)(plus - reference) : strlen(reference);

	if (length >= sizeof(name))
		return -1;
	memcpy(name, reference, length);
	name[length] = 0;
	for (size_t index = 0; index < pcm_block_count; index++)
	{
		const pcm_block *block = &pcm_blocks[index];

		if (strcmp(block->name, name) != 0)
			continue;
		if (pages == 0 ? block->offset < PCM_FIXED : ((pages >> (block->offset / PCM_WINDOW)) & 1u) != 0)
			return (int64_t)block->offset + (plus ? strtoll(plus + 1, NULL, 16) : 0);
	}
	return -1;
}

/* A line of headers.txt as its header's 12 bytes (tools/pcm_samples.py header_bytes) */
static int header_line(char *line, const char *path)
{
	static const struct
	{
		const char *name;
		int at, shift, bits;
	} envelope[] = {
		{ "lfo_vibrato", 7, 0, 8 }, { "attack", 8, 4, 4 }, { "decay1", 8, 0, 4 }, { "decay_level", 9, 4, 4 },
		{ "decay2", 9, 0, 4 }, { "key_rate_scale", 10, 4, 4 }, { "release", 10, 0, 4 }, { "lfo_amplitude", 11, 0, 8 }
	};
	char *words[32];
	const int count = line_words(line, words, 32);
	unsigned long number;
	int window;
	int64_t address = -1;
	uint32_t start, loop = 0, length = 0, format = 0;
	uint8_t *raw;

	if (count == 0)
		return 1;
	number = strtoul(words[0], NULL, 16);
	window = strcmp(words[1], "window_a") == 0 ? 1 : strcmp(words[1], "window_b") == 0 ? 2 : 0;
	if (count < 3 || number >= PCM_HEADERS || (!window && strcmp(words[1], "fixed") != 0))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s has a line that is no header", path);
		return 0;
	}
	raw = &pcm_headers[number * PCM_HEADER];
	memset(raw, 0, PCM_HEADER);
	for (int word = 2; word < count; word++)
	{
		char *equals = strchr(words[word], '=');
		const char *key = words[word], *value;
		int64_t place;

		if (!equals)
			continue;
		*equals = 0;
		value = equals + 1;
		if (strcmp(key, "loop") == 0)
			loop = (uint32_t)strtoul(value, NULL, 16);
		else if (strcmp(key, "length") == 0)
			length = (uint32_t)strtoul(value, NULL, 16);
		else if (strcmp(key, "format") == 0)
			format = (uint32_t)strtoul(value, NULL, 16);
		else if (strcmp(key, "at") == 0 || strcmp(key, "sample") == 0 || strcmp(key, "sets") == 0
			|| strcmp(key, "unplayed_sets") == 0)
		{
			if (key[0] == 'a')
				place = (int64_t)strtoul(value, NULL, 16);
			else if (key[0] == 's' && key[1] == 'a')
				place = sample_place(value, 0);
			else
			{
				/* sets S,S:NAME: the sample on the pages those sets put in the window */
				const char *colon = strrchr(value, ':');
				unsigned pages = 0;

				if (!colon)
					return 0;
				for (const char *set = value; set < colon; set = strchr(set, ',') ? strchr(set, ',') + 1 : colon)
				{
					const long sound_set = strtol(set, NULL, 10);

					if (sound_set >= 0 && sound_set < PCM_SETS && set_pages[sound_set] >= 0)
						pages |= 1u << set_pages[sound_set];
				}
				place = sample_place(colon + 1, pages);
				if (place >= 0)
					place &= PCM_WINDOW - 1;
			}
			if (place < 0 || (address >= 0 && address != place))
			{
				snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s: header %03lX names a sample that is not there", path, number);
				return 0;
			}
			address = place;
		}
		else
			for (size_t field = 0; field < sizeof(envelope) / sizeof(envelope[0]); field++)
				if (strcmp(key, envelope[field].name) == 0)
					raw[envelope[field].at] |= (uint8_t)((strtoul(value, NULL, 16) & ((1u << envelope[field].bits) - 1)) << envelope[field].shift);
	}
	if (address < 0)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s: header %03lX plays no sample", path, number);
		return 0;
	}
	start = (window ? (window == 1 ? 0x100000u : 0x180000u) + (uint32_t)address : (uint32_t)address) | format << 21;
	raw[0] = (uint8_t)(start >> 16);
	raw[1] = (uint8_t)(start >> 8);
	raw[2] = (uint8_t)start;
	raw[3] = (uint8_t)(loop >> 8);
	raw[4] = (uint8_t)loop;
	raw[5] = (uint8_t)((0x10000u - length) >> 8);
	raw[6] = (uint8_t)(0x10000u - length);
	return 1;
}

int assets_read_samples(const char *folder)
{
	char *layout = assets_read_file(assets_path(folder, "sound/samples/samples.txt"));
	char *headers;
	char *cursor = layout;
	char *line;
	size_t room = 0;

	for (int set = 0; set < PCM_SETS; set++)
		set_pages[set] = -1;
	if (!layout)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, "sound/samples/samples.txt"));
		return 0;
	}
	while ((line = next_text_line(&cursor)) != NULL)
	{
		char *words[64];
		const int count = line_words(line, words, 64);

		if (count >= 3 && strcmp(words[0], "page") == 0 && strcmp(words[2], "sets") == 0)
		{
			for (int word = 3; word < count; word++)
				if (atoi(words[word]) >= 0 && atoi(words[word]) < PCM_SETS)
					set_pages[atoi(words[word])] = atoi(words[1]);
		}
		else if (count >= 3 && strcmp(words[0], "uses") != 0)
		{
			pcm_block *block;
			const uint8_t *data = NULL;
			uint32_t length = 0;

			if (pcm_block_count == room)
			{
				pcm_block *grown;

				room = room ? room * 2 : 256;
				if (!(grown = realloc(pcm_blocks, room * sizeof(*pcm_blocks))))
					return 0;
				pcm_blocks = grown;
			}
			/* (a sample at more than one place: its bytes once) */
			for (size_t index = 0; index < pcm_block_count && !data; index++)
				if (strcmp(pcm_blocks[index].name, words[2]) == 0)
				{
					data = pcm_blocks[index].data;
					length = pcm_blocks[index].length;
				}
			if (!data)
			{
				char wav_path[512];

				snprintf(wav_path, sizeof(wav_path), "sound/samples/%s.wav", words[2]);
				if (!(data = read_wav(assets_path(folder, wav_path), &length)))
				{
					snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, wav_path));
					free(layout);
					return 0;
				}
			}
			if (length != (uint32_t)strtoul(words[1], NULL, 16) || strtoul(words[0], NULL, 16) + length > PCM_SIZE)
			{
				snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the sample %s is not as long as samples.txt says", words[2]);
				free(layout);
				return 0;
			}
			block = &pcm_blocks[pcm_block_count++];
			block->offset = (uint32_t)strtoul(words[0], NULL, 16);
			block->length = length;
			block->data = data;
			block->name = malloc(strlen(words[2]) + 1);
			if (!block->name)
				return 0;
			strcpy(block->name, words[2]);
		}
	}
	free(layout);
	qsort(pcm_blocks, pcm_block_count, sizeof(*pcm_blocks), by_offset);
	if (!(headers = assets_read_file(assets_path(folder, "sound/samples/headers.txt"))))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, "sound/samples/headers.txt"));
		return 0;
	}
	cursor = headers;
	while ((line = next_text_line(&cursor)) != NULL)
		if (!header_line(line, "headers.txt"))
		{
			if (!assets_problem[0])
				snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "headers.txt has a line that is no header");
			free(headers);
			return 0;
		}
	free(headers);
	return 1;
}

uint8_t assets_pcm_byte(uint32_t place)
{
	size_t low = 0, high = pcm_block_count;

	place %= PCM_SIZE;
	if (place < sizeof(pcm_headers))
		return pcm_headers[place];
	if (last_pcm < pcm_block_count && place - pcm_blocks[last_pcm].offset < pcm_blocks[last_pcm].length)
		return pcm_blocks[last_pcm].data[place - pcm_blocks[last_pcm].offset];
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (pcm_blocks[middle].offset <= place)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0 || place - pcm_blocks[low - 1].offset >= pcm_blocks[low - 1].length)
		return 0xff;    /* (no sample there) */
	last_pcm = low - 1;
	return pcm_blocks[low - 1].data[place - pcm_blocks[low - 1].offset];
}

/* ---- the graphics for the video ---- */

const s32_graphics *assets_graphics(void)
{
	static s32_graphics graphics;

	graphics.tile_pens = tile_pens;
	graphics.tile_count = TILE_COUNT;
	graphics.sprite_word = sprite_word;
	graphics.sprite_banks = 4;
	return &graphics;
}
