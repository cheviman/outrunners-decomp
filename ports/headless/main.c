/**
 *    Name:        main.c
 *    Description: runs the game with no screen or sound, with the controls a script drives, and writes a
 *                 hash of the board state after every frame, in the format tools/reference/s32ref uses
 *    Version:     0.1.0
 *    Created:     11-10-2026
 *    Modified:    20-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: orunners_headless [--frames N] [--hashes FILE] [--script FILE] [--dump-frame N --dump-dir DIR]
 *
 *    The board runs with frame timing, as s32ref does by default, and the hashes are the ones s32ref
 *    writes, so the two compare byte by byte. The game's own video and sound come in when their libraries
 *    draw the Multi 32's two screens and play the MultiPCM.
 */
#include "s32/machine.h"
#include "s32/program.h"
#include "s32/script.h"
#include "game/data.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: orunners_headless [--frames N] [--hashes FILE] [--script FILE] [--dump-frame N --dump-dir DIR]\n"

static uint64_t frames_wanted = 600;
static FILE *hashes;
static s32_script *script;
static uint64_t dump_frame = UINT64_MAX;
static const char *dump_dir;

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
		else
		{
			fprintf(stderr, USAGE);
			return 1;
		}
	}
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

	s32_board.irq_changed = on_irq_changed;
	s32_board.controls_update = on_controls_update;
	s32_board.log = on_log;
	s32_board.frame_timing = true;
	s32_init(game_program_rom);
	program_reset();
	program_frame_done = on_frame_done;

	program_run();

	if (hashes)
		fclose(hashes);
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
