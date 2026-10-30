/**
 *    Name:        tables.c
 *    Description: the tables the sound program reads by name, which the board sets
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

const void *fm_global_init_data;
const void *fm_operator_init_data;
const void *voice_chip_channels_data;
const void *fm_patch_registers_data;
const void *fm_level_registers_data;
const void *fm_fnum_tables_data;
const void *part_templates_data;
const void *instruments_data;
const void *note_codes_data;
const void *velocity_levels_data;
const void *pcm_pitch_tables_data;
const void *bend_range_tables_data;
const void *fm_patches_data;
const void *drum_kits_data;
const void *engine_levels_data;
const void *engine_pitches_data;
const void *tyre_noises_data;
const void *songs_data;
const void *sound_commands_data;

/* the tables the game loads from its assets: their names, addresses and pointers */
const sound_table sound_tables[] =
{
	{ "fm_global_init", 0x0234u, &fm_global_init_data },
	{ "fm_operator_init", 0x024Au, &fm_operator_init_data },
	{ "voice_chip_channels", 0x02E4u, &voice_chip_channels_data },
	{ "fm_patch_registers", 0x1534u, &fm_patch_registers_data },
	{ "fm_level_registers", 0x1549u, &fm_level_registers_data },
	{ "fm_fnum_tables", 0x1565u, &fm_fnum_tables_data },
	{ "part_templates", 0x1847u, &part_templates_data },
	{ "instruments", 0x1BC2u, &instruments_data },
	{ "note_codes", 0x1F62u, &note_codes_data },
	{ "velocity_levels", 0x2016u, &velocity_levels_data },
	{ "pcm_pitch_tables", 0x20AEu, &pcm_pitch_tables_data },
	{ "bend_range_tables", 0x2546u, &bend_range_tables_data },
	{ "fm_patches", 0x2A5Eu, &fm_patches_data },
	{ "drum_kits", 0x2DA5u, &drum_kits_data },
	{ "engine_levels", 0x37F3u, &engine_levels_data },
	{ "engine_pitches", 0x38F3u, &engine_pitches_data },
	{ "tyre_noises", 0x3DC6u, &tyre_noises_data },
	{ "songs", 0x439Bu, &songs_data },
	{ "sound_commands", 0x44DBu, &sound_commands_data },
};

const size_t sound_table_count = 19;
