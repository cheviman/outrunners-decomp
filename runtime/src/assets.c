/**
 *    Name:        assets.c
 *    Description: loads the game's assets from their files and finds the data an address of the game
 *                 reaches (game/assets.h)
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The program's data is assets/data: layout.json lists its pieces in the order of their addresses (name,
 *    address, size, format, and the file and section that hold it, or a fill), names.txt the address of
 *    every name a piece may give for an address. The formats are the extractor's (tools/core/data_formats.py
 *    and tools/core/formats/typed.py): 'words' (16-bit hex), 'bytes' (hex), 'fill' ('bytes' and 'fill XX N'
 *    lines) and 'typed TYPE' (a layout line, then values or records in columns; an address may be a name,
 *    or name+0xNN). The code ('kept' pieces) is no asset: the C replaces it.
 */
#include "game/assets.h"

#include "game/support.h"
#include "assets_internal.h"
#include "json.h"
#include "png.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
	uint32_t address;
	uint32_t size;
	uint8_t *data;
} piece;

typedef struct
{
	char *name;
	uint32_t address;
} named_address;

/* a text file of sections, read once: its sections' names and where their lines are */
typedef struct
{
	char *path;
	char *text;
	size_t count;
	char **names;
	char **bodies;    /* each section's lines, ended with a 0 where the next header starts */
} section_file;

/* the pieces of a program's data and the names its files may give for addresses: the V70's (assets/data)
   and the sound program's (assets/sound/data, by their offsets in its chip) */
typedef struct
{
	const char *folder;
	piece *pieces;
	size_t count;
	size_t last;
	size_t bytes;
	named_address *names;
	size_t name_count;
} data_region;

static data_region program = { .folder = "data" }, sound = { .folder = "sound/data" };
static data_region *reading = &program;    /* the region whose files are being read (address_of_name) */
static section_file *files;
static size_t file_count;
char assets_problem[ASSETS_PROBLEM_SIZE];

/* ---- files ---- */

char *assets_read_file(const char *path)
{
	FILE *input = fopen(path, "rb");
	long size;
	char *text;

	if (!input)
		return NULL;
	if (fseek(input, 0, SEEK_END) != 0 || (size = ftell(input)) < 0 || fseek(input, 0, SEEK_SET) != 0
		|| !(text = malloc((size_t)size + 1)))
	{
		fclose(input);
		return NULL;
	}
	if (fread(text, 1, (size_t)size, input) != (size_t)size)
	{
		free(text);
		fclose(input);
		return NULL;
	}
	fclose(input);
	text[size] = 0;
	return text;
}

const char *assets_path(const char *folder, const char *path)
{
	static char buffer[1024];

	snprintf(buffer, sizeof(buffer), "%s/%s", folder, path);
	return buffer;
}

/* A file of sections, from the cache or read now: "======== NAME (ADDRESS, format) ========" starts each */
static section_file *sections_of(const char *folder, const char *path)
{
	section_file *file;
	char *line;
	size_t room = 0;

	for (size_t index = 0; index < file_count; index++)
		if (strcmp(files[index].path, path) == 0)
			return &files[index];
	file = realloc(files, (file_count + 1) * sizeof(*files));
	if (!file)
		return NULL;
	files = file;
	file = &files[file_count];
	memset(file, 0, sizeof(*file));
	if (!(file->text = assets_read_file(assets_path(folder, path))) || !(file->path = malloc(strlen(path) + 1)))
		return NULL;
	strcpy(file->path, path);
	file_count++;
	for (line = file->text; *line;)
	{
		char *end = strchr(line, '\n');
		char *next = end ? end + 1 : line + strlen(line);

		if (strncmp(line, "======== ", 9) == 0)
		{
			char *name = line + 9;
			char *name_end = name;

			while (*name_end && !isspace((unsigned char)*name_end))
				name_end++;
			if (file->count == room)
			{
				char **grown_names, **grown_bodies;

				room = room ? room * 2 : 32;
				grown_names = realloc(file->names, room * sizeof(char *));
				if (grown_names)
					file->names = grown_names;
				grown_bodies = realloc(file->bodies, room * sizeof(char *));
				if (grown_bodies)
					file->bodies = grown_bodies;
				if (!grown_names || !grown_bodies)
					return NULL;
			}
			*name_end = 0;
			file->names[file->count] = name;
			file->bodies[file->count] = next;
			file->count++;
			*line = 0;    /* (the section before ends here) */
		}
		line = next;
	}
	return file;
}

static const char *section_body(section_file *file, const char *name)
{
	for (size_t index = 0; index < file->count; index++)
		if (strcmp(file->names[index], name) == 0)
			return file->bodies[index];
	return NULL;
}

/* ---- names ---- */

static int by_name(const void *first, const void *second)
{
	return strcmp(((const named_address *)first)->name, ((const named_address *)second)->name);
}

/* The region's names.txt: NAME ADDRESS lines (hex) */
static int read_names(const char *folder, data_region *region)
{
	char path[256];
	char *text;
	char *line;
	size_t room = 0;

	snprintf(path, sizeof(path), "%s/names.txt", region->folder);
	if (!(text = assets_read_file(assets_path(folder, path))))
		return 0;
	for (line = text; *line;)
	{
		char *end = strchr(line, '\n');
		char name[256];
		unsigned long address;

		if (end)
			*end = 0;
		if (line[0] != '#' && sscanf(line, "%255s %lx", name, &address) == 2)
		{
			if (region->name_count == room)
			{
				named_address *grown;

				room = room ? room * 2 : 1024;
				if (!(grown = realloc(region->names, room * sizeof(*grown))))
					return 0;
				region->names = grown;
			}
			if (!(region->names[region->name_count].name = malloc(strlen(name) + 1)))
				return 0;
			strcpy(region->names[region->name_count].name, name);
			region->names[region->name_count].address = (uint32_t)address;
			region->name_count++;
		}
		line = end ? end + 1 : line + strlen(line);
	}
	free(text);
	qsort(region->names, region->name_count, sizeof(*region->names), by_name);
	return 1;
}

/* The address 'token' stands for (name, name+0xNN) in the names of the region being read, or -1 */
static int64_t address_of_name(const char *token)
{
	char name[256];
	const char *plus = strchr(token, '+');
	size_t length = plus ? (size_t)(plus - token) : strlen(token);
	named_address key;
	const named_address *found;

	if (length >= sizeof(name))
		return -1;
	memcpy(name, token, length);
	name[length] = 0;
	key.name = name;
	found = bsearch(&key, reading->names, reading->name_count, sizeof(*reading->names), by_name);
	if (!found)
		return -1;
	return (int64_t)found->address + (plus ? strtoll(plus + 1, NULL, 16) : 0);
}

/* ---- the formats ---- */

/* The next token of the line at *at (up to a comment or the line's end) into 'token': false at the end. A
   quoted string is one token, with its quotes, whatever it holds */
static int next_token(const char **at, char *token, size_t size)
{
	const char *start = *at;
	size_t length;

	while (*start == ' ' || *start == '\t' || *start == '\r')
		start++;
	/* ('#' starts the comment, unless a hex digit follows it: a colour, #RRGGBB) */
	if (*start == 0 || *start == '\n' || (*start == '#' && !isxdigit((unsigned char)start[1])))
	{
		*at = start;
		return 0;
	}
	if (*start == '"')
	{
		for (length = 1; start[length] && start[length] != '\n' && start[length] != '"'; length++)
			if (start[length] == '\\' && start[length + 1] && start[length + 1] != '\n')
				length++;
		if (start[length] == '"')
			length++;
	}
	else
		for (length = 0; start[length] && !isspace((unsigned char)start[length]); length++)
			;
	if (length >= size)
		length = size - 1;
	memcpy(token, start, length);
	token[length] = 0;
	*at = start + length;
	while (**at && !isspace((unsigned char)**at))
		(*at)++;
	return 1;
}

static const char *next_line(const char *at)
{
	const char *end = strchr(at, '\n');

	return end ? end + 1 : at + strlen(at);
}

typedef struct
{
	uint8_t *out;
	uint32_t size;
	uint32_t at;
	const char *piece;
} writer;

static int put(writer *into, uint64_t value, int bytes)
{
	if (into->at + (uint32_t)bytes > into->size)
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s holds more than its %u bytes", into->piece, into->size);
		return 0;
	}
	for (int index = 0; index < bytes; index++)
		into->out[into->at++] = (uint8_t)(value >> (8 * index));
	return 1;
}

static int hex_values(writer *into, const char *body, int bytes, int keyword)
{
	char token[64];

	for (const char *line = body; *line; line = next_line(line))
	{
		const char *at = line;

		if (keyword)
		{
			if (!next_token(&at, token, sizeof(token)))
				continue;
			if (strcmp(token, "fill") == 0)
			{
				char value[8], count[16];

				if (!next_token(&at, value, sizeof(value)) || !next_token(&at, count, sizeof(count)))
					return 0;
				for (long index = strtol(count, NULL, 10); index > 0; index--)
					if (!put(into, strtoul(value, NULL, 16), 1))
						return 0;
				continue;
			}
			if (strcmp(token, "bytes") != 0)
			{
				snprintf(assets_problem, sizeof(assets_problem), "the asset %s has a line this format does not take", into->piece);
				return 0;
			}
		}
		while (next_token(&at, token, sizeof(token)))
			if (!put(into, strtoul(token, NULL, 16), bytes))
				return 0;
	}
	return 1;
}

/* A quoted string (typed.py string_text: \" \\ and \xNN escapes) and the 0 that closes it */
static int text_value(writer *into, const char *token)
{
	const size_t length = strlen(token);

	if (length < 2 || token[0] != '"' || token[length - 1] != '"')
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s has a text without its quotes: %s", into->piece, token);
		return 0;
	}
	for (size_t at = 1; at < length - 1;)
	{
		unsigned long byte = (unsigned char)token[at];

		if (token[at] == '\\' && token[at + 1] == 'x')
		{
			char digits[3] = { token[at + 2], token[at + 3], 0 };

			byte = strtoul(digits, NULL, 16);
			at += 4;
		}
		else if (token[at] == '\\')
		{
			byte = (unsigned char)token[at + 1];
			at += 2;
		}
		else
			at++;
		if (!put(into, byte, 1))
			return 0;
	}
	return put(into, 0, 1);
}

/* A 16-bit colour from #RRGGBB (a '+' after it: bit 15), five bits a component (typed.py color_value): as
   the palette RAM holds it, or in the palette window's alternate format (the low bit of blue, green and red
   in bits 14-12, the rest 4:4:4) */
static int color_value(writer *into, const char *token, int alternate)
{
	unsigned red, green, blue, value;
	char parts[3][3] = { { token[1], token[2], 0 }, { token[3], token[4], 0 }, { token[5], token[6], 0 } };

	if (token[0] != '#' || strlen(token) < 7 || (strlen(token) > 7 && strcmp(token + 7, "+") != 0))
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s has %s where a colour goes", into->piece, token);
		return 0;
	}
	red = (unsigned)strtoul(parts[0], NULL, 16) >> 3;
	green = (unsigned)strtoul(parts[1], NULL, 16) >> 3;
	blue = (unsigned)strtoul(parts[2], NULL, 16) >> 3;
	if (alternate)
		value = (red & 1u) << 12 | (green & 1u) << 13 | (blue & 1u) << 14 | (red >> 1) | (green >> 1) << 4 | (blue >> 1) << 8;
	else
		value = red | green << 5 | blue << 10;
	if (token[7] == '+')
		value |= 0x8000u;
	return put(into, value, 2);
}

/* A value of a typed piece as the bytes of its type: a number (0x.. or decimal, with a sign), a name, a
   quoted string (text) or a colour */
static int typed_value(writer *into, const char *token, const char *type)
{
	const int bytes = strstr(type, "32") ? 4 : strstr(type, "24") ? 3 : strstr(type, "16") ? 2 : 1;
	const int is_signed = type[0] == 's';
	int64_t value;

	if (strcmp(type, "text") == 0)
		return text_value(into, token);
	if (strncmp(type, "rgb555", 6) == 0)
		return color_value(into, token, strcmp(type, "rgb555alt") == 0);
	if (isdigit((unsigned char)token[0]) || token[0] == '-' || token[0] == '+')
		value = strtoll(token, NULL, 0);
	else if ((value = address_of_name(token)) < 0)
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s names %s, which the game does not know", into->piece, token);
		return 0;
	}
	if (is_signed ? (value < -((int64_t)1 << (bytes * 8 - 1)) || value >= ((int64_t)1 << (bytes * 8 - 1)))
		: (value < -((int64_t)1 << (bytes * 8 - 1)) || value >= ((int64_t)1 << (bytes * 8))))
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s has a value too large for its type: %s", into->piece, token);
		return 0;
	}
	return put(into, (uint64_t)value, bytes);
}

#define MAX_COLUMNS 256
#define MAX_KINDS 64
#define FIXED_BYTES 4

/* A column of a layout or record line (typed.py Column): its type, the bit it is split at (HIGH:LOW), and
   the fixed value the record always holds there, which the rows do not write */
typedef struct
{
	char type[16];
	int split;           /* -1: not split */
	int fixed;           /* whether it is fixed */
	uint8_t value[FIXED_BYTES];
} typed_column;

typedef struct
{
	char keyword[64];    /* a record kind's ("" for the layout's records) */
	uint32_t size;       /* 0: as long as its columns */
	int count;           /* -1: values of one type, columns[0] */
	typed_column columns[MAX_COLUMNS];
} typed_shape;

static int type_bytes(const char *type)
{
	if (strncmp(type, "rgb555", 6) == 0)
		return 2;
	return strstr(type, "32") ? 4 : strstr(type, "24") ? 3 : strstr(type, "16") ? 2 : 1;
}

/* A column's text (name:type, name:type:N, either with =VALUE) into 'column' */
static int read_column(writer *into, char *text, typed_column *column)
{
	char *equals = strchr(text, '=');
	char *colon = strchr(text, ':');
	char *second;

	column->split = -1;
	column->fixed = 0;
	if (equals)
		*equals = 0;
	if (!colon)
		return 0;
	second = strchr(colon + 1, ':');
	if (second)
	{
		*second = 0;
		column->split = atoi(second + 1);
	}
	if (strlen(colon + 1) >= sizeof(column->type))
		return 0;
	strcpy(column->type, colon + 1);
	if (equals)
	{
		/* (the fixed value: the bytes typed_value writes, kept aside) */
		writer aside = { column->value, FIXED_BYTES, 0u, into->piece };

		if (strcmp(column->type, "text") == 0 || !typed_value(&aside, equals + 1, column->type))
			return 0;
		column->fixed = 1;
	}
	return 1;
}

static int read_shape(writer *into, const char **at, typed_shape *shape)
{
	char token[256];

	if (!next_token(at, token, sizeof(token)))
		return 0;
	shape->size = (uint32_t)strtoul(token, NULL, 0);
	for (shape->count = 0; shape->count < MAX_COLUMNS && next_token(at, token, sizeof(token)); shape->count++)
		if (!read_column(into, token, &shape->columns[shape->count]))
			return 0;
	return shape->count > 0;
}

/* A column's value from its token: HIGH:LOW when it is split */
static int column_value(writer *into, const char *token, const typed_column *column)
{
	if (column->split >= 0)
	{
		char number[32];
		const char *colon = strchr(token, ':');

		if (!colon)
		{
			snprintf(assets_problem, sizeof(assets_problem), "the asset %s has %s where HIGH:LOW goes", into->piece, token);
			return 0;
		}
		snprintf(number, sizeof(number), "%llu", (unsigned long long)((strtoull(token, NULL, 16) << column->split)
			| strtoull(colon + 1, NULL, 16)));
		return typed_value(into, number, column->type);
	}
	return typed_value(into, token, column->type);
}

/* A record of 'shape' from the rest of the line at 'at' (its values, the fixed columns left out) */
static int typed_record(writer *into, const char **at, const typed_shape *shape, const char *first)
{
	char token[4096];
	const uint32_t start = into->at;
	int column = 0;
	int have = first != NULL;

	if (have)
		strcpy(token, first);
	for (; column < shape->count; column++)
	{
		const typed_column *current = &shape->columns[column];

		if (current->fixed)
		{
			for (int index = 0; index < type_bytes(current->type); index++)
				if (!put(into, current->value[index], 1))
					return 0;
			continue;
		}
		if (!have && !next_token(at, token, sizeof(token)))
			break;
		have = 0;
		if (!column_value(into, token, current))
			return 0;
	}
	if (column != shape->count || next_token(at, token, sizeof(token)) || (shape->size && into->at - start != shape->size))
	{
		snprintf(assets_problem, sizeof(assets_problem), "the asset %s has a record that is not as its layout says", into->piece);
		return 0;
	}
	return 1;
}

static int typed_values(writer *into, const char *body)
{
	static typed_shape current, kinds[MAX_KINDS];
	char token[4096];
	int kind_count = 0;
	int have_layout = 0;

	for (const char *line = body; *line; line = next_line(line))
	{
		const char *at = line;
		int kind;

		if (!next_token(&at, token, sizeof(token)))
			continue;
		if (strcmp(token, "layout") == 0 || strcmp(token, "record") == 0)
		{
			const int record = token[0] == 'r';
			char name[64];
			const char *after;

			if (!next_token(&at, name, sizeof(name)))
				return 0;
			after = at;
			if (!record && !next_token(&after, token, sizeof(token)))
			{
				/* values of one type */
				current.count = -1;
				current.columns[0].split = -1;
				current.columns[0].fixed = 0;
				if (strlen(name) >= sizeof(current.columns[0].type))
					return 0;
				strcpy(current.columns[0].type, name);
				have_layout = 1;
				continue;
			}
			if (record && kind_count == MAX_KINDS)
				return 0;
			if (!read_shape(into, &at, record ? &kinds[kind_count] : &current))
			{
				if (!assets_problem[0])
					snprintf(assets_problem, sizeof(assets_problem), "the asset %s has a layout this game does not read", into->piece);
				return 0;
			}
			if (record)
				strcpy(kinds[kind_count++].keyword, name);
			else
				have_layout = 1;
			continue;
		}
		if (strcmp(token, "bytes") == 0)
		{
			while (next_token(&at, token, sizeof(token)))
				if (!put(into, strtoul(token, NULL, 16), 1))
					return 0;
			continue;
		}
		for (kind = 0; kind < kind_count && strcmp(kinds[kind].keyword, token) != 0; kind++)
			;
		if (kind < kind_count)
		{
			if (!typed_record(into, &at, &kinds[kind], NULL))
				return 0;
			continue;
		}
		if (!have_layout)
		{
			snprintf(assets_problem, sizeof(assets_problem), "the asset %s has values before its layout", into->piece);
			return 0;
		}
		if (token[0] == '[' && !next_token(&at, token, sizeof(token)))
			continue;    /* (the number of the line's first element) */
		if (current.count < 0)
		{
			do
				if (!typed_value(into, token, current.columns[0].type))
					return 0;
			while (next_token(&at, token, sizeof(token)));
			continue;
		}
		if (!typed_record(into, &at, &current, token))
			return 0;
	}
	return 1;
}

/* ---- the pieces ---- */

/* Whether a piece of this format is code (the C replaces it: the extractor keeps it for the ROM set's
   sake, and the game has no use for it) */
static int is_code(const char *format)
{
	const size_t length = strlen(format);

	return strcmp(format, "kept") == 0 || (length > 5 && strcmp(format + length - 5, "_code") == 0);
}

/* A font in a PNG (tools/formats/program_fonts.py): 8x8 characters, 16 a row, a pixel's index its pen;
   'bits' 4 (a character 32 bytes, 4 a row, the left pixel in the high nibble) or 1 (8 bytes, a bit a pixel,
   the left one in bit 7) */
static int read_font(const char *path, int bits, piece *into)
{
	png_image image;
	const uint32_t bytes_per_character = bits == 4 ? 32u : 8u;
	const uint32_t characters = into->size / bytes_per_character;
	const char *wrong = png_read(path, &image);

	if (wrong)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s %s", path, wrong);
		return 0;
	}
	if (image.width != 128 || image.height != (characters + 15) / 16 * 8 || characters * bytes_per_character != into->size)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s is not as large as its font", path);
		png_free(&image);
		return 0;
	}
	memset(into->data, 0, into->size);
	for (uint32_t character = 0; character < characters; character++)
		for (uint32_t y = 0; y < 8; y++)
			for (uint32_t x = 0; x < 8; x++)
			{
				const uint8_t pen = image.pixels[(character / 16 * 8 + y) * image.width + character % 16 * 8 + x];

				if (bits == 4)
					into->data[character * 32 + y * 4 + x / 2] |= (uint8_t)((pen & 0x0f) << ((x & 1) ? 0 : 4));
				else
					into->data[character * 8 + y] |= (uint8_t)((pen & 1) << (7 - x));
			}
	png_free(&image);
	return 1;
}

/* A piece in a file of its own: a sequence of the sound program as a MIDI file (assets_midi.c), an
   effect's with its header's flags byte first; a font as a PNG */
static int read_binary(const char *folder, const data_region *region, const char *file, const char *format,
	const char *name, piece *into)
{
	char path[512];
	FILE *input;
	long size;
	uint8_t *blob;
	size_t length = 0;
	int flags, read;
	const int effect = strcmp(format, "sound_effect") == 0;

	if (strcmp(format, "program_text_font") == 0 || strcmp(format, "program_one_bit_font") == 0)
	{
		snprintf(path, sizeof(path), "%s/%s", region->folder, file);
		return read_font(assets_path(folder, path), format[8] == 't' ? 4 : 1, into);
	}
	if (strcmp(format, "sound_sequence") != 0 && !effect)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the asset %s is in a format the game does not read (%s)", name, format);
		return 0;
	}
	snprintf(path, sizeof(path), "%s/%s", region->folder, file);
	if (!(input = fopen(assets_path(folder, path), "rb")) || fseek(input, 0, SEEK_END) != 0 || (size = ftell(input)) < 0
		|| fseek(input, 0, SEEK_SET) != 0 || !(blob = malloc((size_t)size + 1)) || fread(blob, 1, (size_t)size, input) != (size_t)size)
	{
		if (input)
			fclose(input);
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, path));
		return 0;
	}
	fclose(input);
	read = assets_midi_sequence(blob, (size_t)size, into->data + effect, into->size - (uint32_t)effect, &length, &flags);
	free(blob);
	if (!read || (effect && flags < 0))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s is no sequence the sound program plays", path);
		return 0;
	}
	if (effect)
		into->data[0] = (uint8_t)flags;
	if (length + (size_t)effect != into->size)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the sequence %s is %u bytes, not %u", name,
			(unsigned)(length + (size_t)effect), into->size);
		return 0;
	}
	return 1;
}

static int read_piece(const char *folder, const data_region *region, const json_value *entry, piece *into)
{
	const char *name = json_text(entry, "name");
	const char *format = json_text(entry, "format");
	const char *file = json_text(entry, "file");
	const char *fill = json_text(entry, "fill");
	const char *section = json_text(entry, "section");
	const json_value *size = json_member(entry, "size");
	section_file *sections;
	const char *body;
	char path[512];
	writer output;
	int read;

	into->address = (uint32_t)strtoul(json_text(entry, "address"), NULL, 16);
	into->size = size ? (uint32_t)size->number : 0u;
	/* a byte more than the piece holds: a pointer just past its end (a list walked to its end) is still this
	   piece's, which program_data_address takes back to the address after it */
	if (!(into->data = malloc((size_t)into->size + 1u)))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "no memory for the asset %s", name);
		return 0;
	}
	if (fill)
	{
		memset(into->data, (int)strtoul(fill, NULL, 16), into->size);
		return 1;
	}
	if (!file || !format)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the layout of the assets has no file for %s", name);
		return 0;
	}
	if (!section)
		return read_binary(folder, region, file, format, name, into);
	snprintf(path, sizeof(path), "%s/%s", region->folder, file);
	if (!(sections = sections_of(folder, path)))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, path));
		return 0;
	}
	if (!(body = section_body(sections, section)))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s has no section %s", file, section);
		return 0;
	}
	output = (writer){ into->data, into->size, 0u, name };
	if (strcmp(format, "words") == 0)
		read = hex_values(&output, body, 2, 0);
	else if (strcmp(format, "bytes") == 0)
		read = hex_values(&output, body, 1, 0);
	else if (strcmp(format, "fill") == 0)
		read = hex_values(&output, body, 1, 1);
	else
		read = typed_values(&output, body);    /* (every other text format is the typed one) */
	if (!read)
	{
		if (!assets_problem[0])
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the asset %s cannot be read", name);
		return 0;
	}
	if (output.at != into->size)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the asset %s holds %u bytes, not %u", name, output.at, into->size);
		return 0;
	}
	return 1;
}

/* A region's pieces from its layout.json, every one but the code */
static int read_region(const char *folder, data_region *region)
{
	char path[256];
	char *text;
	json_value *layout;
	const json_value *list;
	char error[128];

	reading = region;
	if (!read_names(folder, region))
	{
		snprintf(path, sizeof(path), "%s/names.txt", region->folder);
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, path));
		return 0;
	}
	snprintf(path, sizeof(path), "%s/layout.json", region->folder);
	if (!(text = assets_read_file(assets_path(folder, path))))
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read", assets_path(folder, path));
		return 0;
	}
	layout = json_parse(text, error, sizeof(error));
	free(text);
	list = json_member(layout, "pieces");
	if (!list || list->kind != JSON_ARRAY)
	{
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "%s cannot be read: %s", path, layout ? "no pieces" : error);
		json_free(layout);
		return 0;
	}
	if (!(region->pieces = calloc(list->count ? list->count : 1, sizeof(*region->pieces))))
	{
		json_free(layout);
		snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "no memory for the assets");
		return 0;
	}
	for (size_t index = 0; index < list->count; index++)
	{
		const char *format = json_text(&list->items[index], "format");

		if (format && is_code(format))
			continue;
		if (!read_piece(folder, region, &list->items[index], &region->pieces[region->count]))
		{
			json_free(layout);
			return 0;
		}
		region->bytes += region->pieces[region->count].size;
		region->count++;
	}
	json_free(layout);
	return 1;
}

static void forget_files(void)
{
	for (size_t index = 0; index < file_count; index++)
	{
		free(files[index].path);
		free(files[index].text);
		free(files[index].names);
		free(files[index].bodies);
	}
	free(files);
	files = NULL;
	file_count = 0;
}

/* The piece of 'region' holding 'address', or NULL */
static const uint8_t *find(data_region *region, uint32_t address)
{
	size_t low = 0, high = region->count;
	const piece *pieces = region->pieces;

	if (region->last < region->count && address - pieces[region->last].address < pieces[region->last].size)
		return pieces[region->last].data + (address - pieces[region->last].address);
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (pieces[middle].address <= address)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0 || address - pieces[low - 1].address >= pieces[low - 1].size)
		return NULL;
	region->last = low - 1;
	return pieces[low - 1].data + (address - pieces[low - 1].address);
}

/* An address of the program's chips as the pieces are named: the code's chips repeat four times below
   0x100000 (the first 256 KB), and the first MB shows again from 0xF00000 */
static uint32_t folded(uint32_t address)
{
	address &= 0xffffff;
	if ((address & 0xf00000) == 0xf00000)
		address &= 0xfffff;
	if (address < 0x100000)
		address &= 0x3ffff;
	return address;
}

const uint8_t *data_at(uint32_t address)
{
	const uint32_t place = address & 0xffffff;
	const uint8_t *found;

	if (place >= 0x200000 && (place & 0xf00000) != 0xf00000)
	{
		if ((place & 0xf00000) == 0x200000)
			return ram(address);
		data_outside(address);
	}
	if (!(found = find(&program, folded(place))))
		data_outside(address);
	return found;
}

/* the pieces of the program's data by where their bytes are, for program_data_address (made once the
   pieces are loaded) */
static const piece **by_place;
static size_t by_place_count;

static int compare_places(const void *a, const void *b)
{
	const uintptr_t left = (uintptr_t)(*(const piece *const *)a)->data;
	const uintptr_t right = (uintptr_t)(*(const piece *const *)b)->data;
	return left < right ? -1 : left > right;
}

int program_data_address(const void *pointer, uint32_t *address)
{
	size_t low = 0, high = program.count;

	if (by_place_count != program.count)
	{
		const piece **made = program.count ? malloc(program.count * sizeof *made) : NULL;
		if (!made)
			return 0;
		for (size_t index = 0; index < program.count; index++)
			made[index] = &program.pieces[index];
		qsort(made, program.count, sizeof *made, compare_places);
		free((void *)by_place);
		by_place = made;
		by_place_count = program.count;
	}
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if ((uintptr_t)by_place[middle]->data <= (uintptr_t)pointer)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0 || (uintptr_t)pointer - (uintptr_t)by_place[low - 1]->data > by_place[low - 1]->size)
		return 0;    /* (just past a piece's end is its: each piece has a byte more than its data, no other's) */
	*address = by_place[low - 1]->address + (uint32_t)((uintptr_t)pointer - (uintptr_t)by_place[low - 1]->data);
	return 1;
}

const uint8_t *data_if_any(uint32_t address)
{
	const uint32_t place = address & 0xffffff;

	if (place >= 0x200000 && (place & 0xf00000) != 0xf00000)
		return NULL;
	return find(&program, folded(place));
}

const uint8_t *assets_sound_data(uint32_t offset)
{
	return find(&sound, offset);
}

const char *assets_load(const char *folder)
{
	assets_problem[0] = 0;
	if (!read_region(folder, &program) || !read_region(folder, &sound))
	{
		forget_files();
		return assets_problem;
	}
	forget_files();
	if (!assets_read_tilesets(folder) || !assets_read_sprites(folder) || !assets_read_samples(folder))
		return assets_problem;
	for (size_t index = 0; index < game_table_count; index++)
	{
		const uint8_t *found = find(&program, folded(game_tables[index].address));
		named_address key;
		const named_address *named;

		/* (assets that name the table at another address are cut another way: the table the C reads is not
		   whole there) */
		key.name = (char *)game_tables[index].name;
		named = bsearch(&key, program.names, program.name_count, sizeof(*program.names), by_name);
		if (!found || (named && folded(named->address) != folded(game_tables[index].address)))
		{
			snprintf(assets_problem, ASSETS_PROBLEM_SIZE, "the assets have no table %s", game_tables[index].name);
			return assets_problem;
		}
		*game_tables[index].data = found;
	}
	return NULL;
}

size_t assets_piece_count(void)
{
	return program.count + sound.count;
}

size_t assets_data_bytes(void)
{
	return program.bytes + sound.bytes;
}

int assets_dump(const char *path)
{
	FILE *output = fopen(path, "wb");
	int written = output != NULL;

	/* (the program's pieces, then the sound program's, their addresses with bit 31 set) */
	for (int which = 0; which < 2 && written; which++)
	{
		const data_region *region = which ? &sound : &program;

		for (size_t index = 0; index < region->count && written; index++)
		{
			const piece *each = &region->pieces[index];
			const uint32_t address = each->address | (which ? 0x80000000u : 0u);
			const uint8_t head[8] = {
				(uint8_t)address, (uint8_t)(address >> 8), (uint8_t)(address >> 16), (uint8_t)(address >> 24),
				(uint8_t)each->size, (uint8_t)(each->size >> 8), (uint8_t)(each->size >> 16), (uint8_t)(each->size >> 24) };

			written = fwrite(head, 1, 8, output) == 8 && fwrite(each->data, 1, each->size, output) == each->size;
		}
	}
	if (output && fclose(output) != 0)
		written = 0;
	return written;
}
