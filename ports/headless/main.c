/**
 *    Name:        main.c
 *    Description: runs the game with no screen or sound, with the controls a script drives, and writes a
 *                 hash of the board state (and of the frames the video draws) after every frame, in the
 *                 format tools/core/system32/reference/s32ref uses
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: orunners_headless [--frames N] [--hashes FILE] [--script FILE] [--dump-frame N --dump-dir DIR]
 *                             [--video-hashes FILE] [--video-dump N ... --video-dir DIR] [--sound-hashes FILE]
 *                             [--sprite-census FILE] [--tile-census FILE] [--assets FILE] [--extract ROM_SET]
 *    The game's data is its assets pack (game/data.h): --assets, or orunners.assets beside the executable.
 *    --extract first writes that pack from the player's ROM set (a .zip or a folder), as the first run does.
 *    The scaffold (ORUNNERS_SCAFFOLD) runs the original code, so it reads the ROM set in rom/ instead.
 *
 *    The board runs with frame timing, as s32ref does by default, and the hashes are the ones s32ref
 *    writes, so the two compare byte by byte. --video-hashes and --video-dump draw every frame of both
 *    screens with the board's video (runtime/src/video.c): the hash of each frame, and frames as BMP, the
 *    left screen beside the right. --sound-hashes runs the sound board too (the decompiled sound program
 *    and the chips, runtime/src/sound_cpu.c and sound_chips.c), and writes for each frame the hash of its
 *    samples and of the sound board's memory. --sprite-census and --tile-census write what the sprites and
 *    the tilemaps draw, with the colors each pen shows, for the asset extractor (tools/palette_census.py).
 */
#include "s32/machine.h"
#include "s32/program.h"
#include "s32/script.h"
#include "s32/video.h"
#include "sound/chips.h"
#include "game/data.h"
#include "game/extract.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: orunners_headless [--frames N] [--hashes FILE] [--script FILE] [--dump-frame N --dump-dir DIR]" \
	" [--video-hashes FILE] [--video-dump N ... --video-dir DIR] [--sound-hashes FILE]" \
	" [--sprite-census FILE] [--tile-census FILE] [--assets FILE] [--extract ROM_SET]\n"
#define ASSETS_FILE "orunners.assets"
#define MAX_VIDEO_DUMPS 64
#define CENSUS_SLOTS (1u << 22)     /* open addressing for the censuses' hashes */
#define TILE_CENSUS_EVERY 30        /* the tilemaps are looked at every this many frames */

static uint64_t frames_wanted = 600;
static FILE *hashes;
static s32_script *script;
static uint64_t dump_frame = UINT64_MAX;
static const char *dump_dir;
static FILE *video_hashes;
static uint64_t video_dumps[MAX_VIDEO_DUMPS];
static int video_dump_count;
static const char *video_dir;
static FILE *sound_hashes;
static FILE *sprite_census;
static FILE *tile_census;
static uint64_t *census_seen;       /* hashes of what the censuses wrote (0 = free) */

/* Whether a census line with this hash was written already; notes it if not */
static bool census_written(uint64_t hash)
{
	uint32_t slot;

	if (hash == 0)
		hash = 1;
	for (slot = (uint32_t)hash & (CENSUS_SLOTS - 1); census_seen[slot] != 0; slot = (slot + 1) & (CENSUS_SLOTS - 1))
		if (census_seen[slot] == hash)
			return true;
	census_seen[slot] = hash;
	return false;
}

/* --sprite-census: every sprite of the ROM the list draws, with the colors its pens show on its screen, once
   per set of colors and way of drawing it: "ADDRESS WORDS ROWS BITS OPAQUE SCREEN COLOR..." (hex; OPAQUE 1
   when it draws the end code and goes on past it; colors RGB555, one a pen) */
static void on_sprite_drawn(int screen, uint32_t address, int words, int rows, bool bits8, bool opaque,
	const uint16_t *pixels)
{
	const int pens = bits8 ? 256 : 16;
	uint16_t colors[256];
	uint64_t hash = 0xcbf29ce484222325ull;

	for (int pen = 0; pen < pens; pen++)
		colors[pen] = s32_video_sprite_pixel_color(screen, pixels[pen]);
	hash = (hash ^ (address | (uint64_t)screen << 32)) * 0x100000001b3ull;
	hash = (hash ^ (uint64_t)((uint32_t)words << 16 | (uint32_t)rows << 2 | (opaque ? 2u : 0u) | (bits8 ? 1u : 0u)))
		* 0x100000001b3ull;
	for (int pen = 0; pen < pens; pen++)
		hash = (hash ^ colors[pen]) * 0x100000001b3ull;
	if (census_written(hash))
		return;
	fprintf(sprite_census, "%06X %d %d %d %d %d", address, words, rows, bits8 ? 8 : 4, opaque ? 1 : 0, screen);
	for (int pen = 0; pen < pens; pen++)
		fprintf(sprite_census, " %04X", colors[pen]);
	fputc('\n', sprite_census);
}

/* --tile-census: every tile the tilemaps that are on show (every TILE_CENSUS_EVERY frames), with the colors its
   pens show on each screen that is on (the screens share the tilemaps, not the palettes), once per set of
   colors: "TILE COLOR TILEMAP SCREEN COLOR..." (hex; colors RGB555, one a pen) */
static void on_tile_shown(int tilemap, uint32_t tile, uint16_t color)
{
	if (program_frames() % TILE_CENSUS_EVERY != 0)
		return;
	for (int screen = 0; screen < S32_SCREENS; screen++)
	{
		uint16_t colors[16];
		uint64_t hash = 0x84222325cbf29ce4ull;

		if (!s32_display_enabled(screen))
			continue;
		for (int pen = 0; pen < 16; pen++)
			colors[pen] = s32_video_tile_pen_color(screen, tilemap, color, pen);
		hash = (hash ^ (tile | (uint64_t)screen << 32)) * 0x100000001b3ull;
		for (int pen = 0; pen < 16; pen++)
			hash = (hash ^ colors[pen]) * 0x100000001b3ull;
		if (census_written(hash))
			continue;
		fprintf(tile_census, "%04X %03X %d %d", tile, color, tilemap, screen);
		for (int pen = 0; pen < 16; pen++)
			fprintf(tile_census, " %04X", colors[pen]);
		fputc('\n', tile_census);
	}
}

static void write_file(const char *name, const void *data, size_t size)
{
	char path[1024];
	FILE *output;

	snprintf(path, sizeof(path), "%s/%s", dump_dir, name);
	output = fopen(path, "wb");
	if (!output)
	{
		fprintf(stderr, "%s: cannot write\n", path);
		return;
	}
	fwrite(data, 1, size, output);
	fclose(output);
}

static void dump_state(void)
{
	write_file("work_ram.bin", s32_board.work_ram, sizeof(s32_board.work_ram));
	write_file("video_ram.bin", s32_board.video_ram, sizeof(s32_board.video_ram));
	write_file("sprite_ram.bin", s32_board.sprite_ram, sizeof(s32_board.sprite_ram));
	write_file("palette.bin", s32_board.palette, sizeof(s32_board.palette));
	write_file("mixer.bin", s32_board.mixer, sizeof(s32_board.mixer));
	write_file("shared_ram.bin", s32_board.shared_ram, sizeof(s32_board.shared_ram));
	write_file("comm_ram.bin", s32_board.comm_ram, sizeof(s32_board.comm_ram));
	write_file("eeprom.bin", s32_board.eeprom.data, sizeof(s32_board.eeprom.data));
}

/* The last frames the video drew, as a 24-bit BMP: the left screen, then the right one */
static void write_bmp(const char *path)
{
	int widths[S32_SCREENS] = { 0 };
	const uint32_t *screens[S32_SCREENS];
	int width;
	int row;
	uint32_t size;
	uint8_t *file;
	FILE *output;

	for (int screen = 0; screen < S32_SCREENS; screen++)
		screens[screen] = s32_video_frame(screen, &widths[screen]);
	width = widths[0] + widths[1];
	row = (width * 3 + 3) & ~3;
	size = 54 + (uint32_t)row * S32_VIDEO_HEIGHT;
	file = calloc(size, 1);
	if (!file)
		return;
	file[0] = 'B';
	file[1] = 'M';
	for (int byte = 0; byte < 4; byte++)
	{
		file[2 + byte] = (uint8_t)(size >> (8 * byte));
		file[10 + byte] = (uint8_t)(54 >> (8 * byte));
		file[14 + byte] = (uint8_t)(40 >> (8 * byte));
		file[18 + byte] = (uint8_t)((uint32_t)width >> (8 * byte));
		file[22 + byte] = (uint8_t)((uint32_t)S32_VIDEO_HEIGHT >> (8 * byte));
	}
	file[26] = 1;
	file[28] = 24;
	for (int y = 0; y < S32_VIDEO_HEIGHT; y++)
		for (int x = 0; x < width; x++)
		{
			const int screen = x < widths[0] ? 0 : 1;
			const uint32_t pixel = screens[screen][y * S32_VIDEO_MAX_WIDTH + (screen ? x - widths[0] : x)];
			uint8_t *out = &file[54 + (size_t)(S32_VIDEO_HEIGHT - 1 - y) * (size_t)row + (size_t)x * 3];

			out[0] = (uint8_t)pixel;
			out[1] = (uint8_t)(pixel >> 8);
			out[2] = (uint8_t)(pixel >> 16);
		}
	output = fopen(path, "wb");
	if (output)
	{
		fwrite(file, 1, size, output);
		fclose(output);
	}
	else
		fprintf(stderr, "%s: cannot write\n", path);
	free(file);
}

static void on_irq_changed(bool asserted)
{
	program_irq_line(asserted);
}

static void on_log(const char *message)
{
	fprintf(stderr, "[s32] %s\n", message);
}

static void on_frame_done(void)
{
	if (hashes)
		fprintf(hashes, "%" PRIu64 " %016" PRIx64 "\n", program_frames() - 1, s32_state_hash());
	if (dump_dir && program_frames() - 1 == dump_frame)
		dump_state();
	if (video_hashes)
		fprintf(video_hashes, "%" PRIu64 " %016" PRIx64 "\n", program_frames() - 1, s32_video_frame_hash());
	if (sound_hashes)
	{
		/* the sound board up to the frame's end, then the samples that start before it */
		if (s32_board.time_passes)
			s32_board.time_passes(s32_board.cycles);
		sound_chips_end_frame(s32_board.cycles);
		fprintf(sound_hashes, "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", program_frames() - 1,
			sound_chips_frame_hash(), sound_board_state_hash());
	}
	for (int index = 0; index < video_dump_count && video_dir; index++)
		if (video_dumps[index] == program_frames() - 1)
		{
			char path[1024];

			snprintf(path, sizeof(path), "%s/frame_%" PRIu64 ".bmp", video_dir, video_dumps[index]);
			write_bmp(path);
		}
	if (program_frames() >= frames_wanted)
		program_stop();
}

static void on_controls_update(void)
{
	/* VBLANK of the board's frame is starting */
	s32_script_apply(script, s32_board.frame);
}

int main(int argc, char **argv)
{
	const char *hashes_path = NULL;
	const char *script_path = NULL;
	const char *video_hashes_path = NULL;
	const char *sound_hashes_path = NULL;
	const char *sprite_census_path = NULL;
	const char *tile_census_path = NULL;
	const char *assets_path = NULL;
	const char *extract_source = NULL;
	char beside[1024];
	bool frames_given = false;

	for (int index = 1; index < argc; index++)
	{
		if (strcmp(argv[index], "--frames") == 0 && index + 1 < argc)
		{
			frames_wanted = strtoull(argv[++index], NULL, 10);
			frames_given = true;
		}
		else if (strcmp(argv[index], "--hashes") == 0 && index + 1 < argc)
			hashes_path = argv[++index];
		else if (strcmp(argv[index], "--script") == 0 && index + 1 < argc)
			script_path = argv[++index];
		else if (strcmp(argv[index], "--dump-frame") == 0 && index + 1 < argc)
			dump_frame = strtoull(argv[++index], NULL, 10);
		else if (strcmp(argv[index], "--dump-dir") == 0 && index + 1 < argc)
			dump_dir = argv[++index];
		else if (strcmp(argv[index], "--video-hashes") == 0 && index + 1 < argc)
			video_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--video-dump") == 0 && index + 1 < argc && video_dump_count < MAX_VIDEO_DUMPS)
			video_dumps[video_dump_count++] = strtoull(argv[++index], NULL, 10);
		else if (strcmp(argv[index], "--video-dir") == 0 && index + 1 < argc)
			video_dir = argv[++index];
		else if (strcmp(argv[index], "--sound-hashes") == 0 && index + 1 < argc)
			sound_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--sprite-census") == 0 && index + 1 < argc)
			sprite_census_path = argv[++index];
		else if (strcmp(argv[index], "--tile-census") == 0 && index + 1 < argc)
			tile_census_path = argv[++index];
		else if (strcmp(argv[index], "--assets") == 0 && index + 1 < argc)
			assets_path = argv[++index];
		else if (strcmp(argv[index], "--extract") == 0 && index + 1 < argc)
			extract_source = argv[++index];
		else
		{
			fprintf(stderr, USAGE);
			return 1;
		}
	}
#ifdef ORUNNERS_SCAFFOLD
	(void)assets_path;
	(void)extract_source;
	(void)beside;
	{
		const char *problem = game_rom_set_load("rom");

		if (problem)
		{
			fprintf(stderr, "rom: %s\n", problem);
			return 1;
		}
	}
#else
	if (!assets_path)
	{
		/* (the executable's folder, as argv[0] gives it) */
		const char *slash = strrchr(argv[0], '/'), *backslash = strrchr(argv[0], '\\');
		const char *last = slash > backslash ? slash : backslash;

		snprintf(beside, sizeof(beside), "%.*s" ASSETS_FILE, last ? (int)(last - argv[0] + 1) : 0, argv[0]);
		assets_path = beside;
	}
	if (extract_source)
	{
		const char *problem = game_data_extract(extract_source, assets_path);

		if (problem)
		{
			fprintf(stderr, "%s: %s\n", extract_source, problem);
			return 1;
		}
	}
	switch (game_data_load(assets_path))
	{
	case GAME_DATA_LOADED:
		break;
	case GAME_DATA_MISSING:
		fprintf(stderr, "%s: no game assets\n", assets_path);
		return 1;
	default:
		fprintf(stderr, "%s: the game assets cannot be read\n", assets_path);
		return 1;
	}
#endif
	if (script_path)
	{
		char error[256];

		script = s32_script_load(script_path, error, sizeof(error));
		if (!script)
		{
			fprintf(stderr, "%s\n", error);
			return 1;
		}
		if (!frames_given && s32_script_frames(script))
			frames_wanted = s32_script_frames(script);
	}
	if (hashes_path)
		hashes = fopen(hashes_path, "w");
	if (video_hashes_path)
		video_hashes = fopen(video_hashes_path, "w");
	if (sprite_census_path || tile_census_path)
	{
		census_seen = calloc(CENSUS_SLOTS, sizeof(census_seen[0]));
		if (!census_seen)
		{
			fprintf(stderr, "no memory for the census\n");
			return 1;
		}
	}
	if (sprite_census_path)
	{
		sprite_census = fopen(sprite_census_path, "w");
		s32_video_sprite_observer = on_sprite_drawn;
	}
	if (tile_census_path)
	{
		tile_census = fopen(tile_census_path, "w");
		s32_video_tile_observer = on_tile_shown;
	}

	s32_board.irq_changed = on_irq_changed;
	s32_board.controls_update = on_controls_update;
	s32_board.log = on_log;
	s32_board.frame_timing = true;
	s32_init(game_program);
	if (video_hashes || video_dump_count || sprite_census || tile_census)
		s32_video_init(game_tiles, sizeof(game_tiles), game_sprites, sizeof(game_sprites));
	if (sound_hashes_path)
	{
		s32_board.sound_rom = game_sound;
		s32_board.pcm_rom = game_pcm;
		sound_chips_attach();
		sound_cpu_attach();
		sound_hashes = fopen(sound_hashes_path, "w");
	}
	program_reset();
	program_frame_done = on_frame_done;

	program_run();

	if (hashes)
		fclose(hashes);
	if (video_hashes)
		fclose(video_hashes);
	if (sound_hashes)
		fclose(sound_hashes);
	if (sprite_census)
		fclose(sprite_census);
	if (tile_census)
		fclose(tile_census);
	free(census_seen);
	s32_script_free(script);
	printf("frames:            %" PRIu64 "\n", program_frames());
	printf("interrupts taken:  %u\n", program_interrupts());
	printf("sprite lists:      %u\n", s32_board.sprite_lists);
	printf("sound requests:    %u\n", s32_board.sound_requests);
	printf("unmapped reads:    %u\n", s32_board.unmapped_reads);
	printf("unmapped writes:   %u\n", s32_board.unmapped_writes);
	printf("eeprom written:    %s\n", s32_board.eeprom.dirty ? "yes" : "no");
	return 0;
}
