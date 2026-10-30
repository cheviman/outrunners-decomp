/**
 *    Name:        voices.c
 *    Description: the YM3438's FM voices: starting a note, retuning, stopping, their volume and patches
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* The FM note on's chip side: the F-number of the note's semitone from fm_fnum_tables at the channel's
   pitch bend (pitch_bends[midi_channel]) and the block from its octave (A4, A0); the patch
   (load_fm_patch, or load_fm_patch_part2 for a channel of part II); the 4 operators' total levels
   (fm_level_registers): a patch level with bit 7 as it is, the others plus note_level; B4 =
   voice_modulation (the output side and the modulation depth); all four operators keyed on (28 = F0 |
   fm_channel). */
void start_fm_note(uint8_t note, uint16_t midi_channel, uint16_t pitch_bends)
{
	uint8_t channel_code = 0, fm_register = 0, level_register = 0, operators_left = 0, part_channel = 0, register_number = 0, total_level = 0, value = 0, voice_note = 0;
	uint16_t channel_in_part = 0, fm_register_2 = 0, fnum_entry = 0, fnumber = 0, patch_levels = 0, register_channel = 0, register_list = 0, run_entry = 0;
	uint8_t saved_operators_left = 0, saved_part_channel = 0;
	uint16_t channel_pitch_bend = 0;

	voice_note = (uint8_t)note;
	z80_block(242, 35);
	pitch_bends = pitch_bends + midi_channel + midi_channel;
	channel_pitch_bend = z80_read16(pitch_bends);
	run_entry = fm_fnum_tables_ADDRESS + (uint8_t)((voice_note & 0xFu) * 2u);
	fnum_entry = FM_FNUMBER_RUNS(run_entry)[0] + channel_pitch_bend + channel_pitch_bend;
	fnumber = FM_FNUMBERS(fnum_entry)[0];
	fnumber = (fnumber & 0xFFu) | ((uint32_t)(uint8_t)(((voice_note >> 1) & 0x38u) + (fnumber >> 8)) << 8);
	channel_code = fm_channel;
	if (((channel_code >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		register_channel = (channel_code & 3u) << 8;
		value = (uint8_t)(fnumber >> 8);
		fm_register = 0xA4u | (register_channel >> 8);
		write_fm_part2(fm_register, value);
		z80_block(65, 9);
		value = (uint8_t)fnumber;
		fm_register = 0xA0u | (register_channel >> 8);
		write_fm_part2(fm_register, value);
	}
	else
	{
		z80_block(40, 6);
		register_channel = (uint32_t)channel_code << 8;
		value = (uint8_t)(fnumber >> 8);
		fm_register = 0xA4u | (register_channel >> 8);
		write_fm(fm_register, value);
		z80_block(65, 9);
		value = (uint8_t)fnumber;
		fm_register = 0xA0u | (register_channel >> 8);
		write_fm(fm_register, value);
		z80_block(10, 1);
	}
	z80_block(31, 4);
	channel_code = fm_channel;
	if (((channel_code >> 2) & 1u) != 0u)
	{
		z80_block(17, 1);
		load_fm_patch_part2(channel_code, &channel_in_part, &register_list, &patch_levels);
		part_channel = (uint8_t)channel_in_part;
	}
	else
	{
		z80_block(17, 1);
		load_fm_patch(channel_code, &channel_in_part, &register_list, &patch_levels);
		part_channel = (uint8_t)channel_in_part;
		z80_block(10, 1);
	}
	z80_block(7, 1);
	operators_left = 4u;
	for (;;)
	{
		z80_block(63, 10);
		saved_operators_left = operators_left;
		saved_part_channel = part_channel;
		register_number = FM_REGISTER_NUMBERS(register_list)[0] + part_channel;
		register_list++;
		level_register = register_number;
		total_level = z80_read8(patch_levels);
		patch_levels++;
		if (total_level >> 7 == 0u)
		{
			z80_block(21, 3);
			total_level += note_level;
		}
		z80_block(17, 1);
		fm_register_2 = level_register;
		write_fm_channel(fm_register_2, total_level);
		z80_block(18, 2);
		part_channel = saved_part_channel;
		operators_left = saved_operators_left - 1u;
		if (operators_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(61, 6);
	value = voice_modulation;
	fm_register_2 = (uint8_t)((fm_channel & 3u) - 0x4Cu);
	write_fm_channel(fm_register_2, value);
	z80_block(37, 4);
	value = fm_channel | 0xF0u;
	fm_register = 0x28u;
	write_fm(fm_register, value);
}

/* Retunes an FM voice after a pitch bend: fm_channel from its chip_channel, the F-number of its note
   from fm_fnum_tables at pitch_bend and the block from its octave (A4, then A0, on its part). */
void retune_fm_voice(uint16_t voice, uint8_t chip_channel)
{
	uint8_t bend_high = 0, channel_code = 0, fm_register = 0, note = 0, part_channel = 0, value = 0;
	uint16_t fnum_entry = 0, fnumber = 0, run_entry = 0;
	uint16_t bend = 0;

	z80_block(234, 33);
	fm_channel = chip_channel & 7u;
	note = VOICE(voice)->note;
	run_entry = fm_fnum_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	bend = pitch_bend;
	bend_high = (uint8_t)(bend >> 8);
	fnum_entry = FM_FNUMBER_RUNS(run_entry)[0] + (((uint32_t)bend_high << 8) | (uint8_t)bend) + (((uint32_t)bend_high << 8) | (uint8_t)bend);
	fnumber = FM_FNUMBERS(fnum_entry)[0];
	fnumber = (fnumber & 0xFFu) | ((uint32_t)(uint8_t)(((note & ~0xFu) >> 1) + (fnumber >> 8)) << 8);
	channel_code = fm_channel;
	if (((channel_code >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		part_channel = channel_code & 3u;
		value = (uint8_t)(fnumber >> 8);
		fm_register = 0xA4u | part_channel;
		write_fm_part2(fm_register, value);
		z80_block(58, 9);
		value = (uint8_t)fnumber;
		fm_register = 0xA0u | part_channel;
		write_fm_part2(fm_register, value);
		return;
	}
	z80_block(40, 6);
	part_channel = channel_code;
	value = (uint8_t)(fnumber >> 8);
	fm_register = 0xA4u | part_channel;
	write_fm(fm_register, value);
	z80_block(58, 9);
	value = (uint8_t)fnumber;
	fm_register = 0xA0u | part_channel;
	write_fm(fm_register, value);
}

/* Frees an FM voice (flags and midi_channel 0) and keys its channel off (28): the sustain pedal's
   release of an FM note it held. */
void stop_fm_voice(uint16_t voice)
{
	uint8_t fm_register = 0, value = 0;

	z80_block(85, 10);
	VOICE(voice)->flags = 0u;
	VOICE(voice)->midi_channel = 0u;
	value = VOICE(voice)->chip_channel & 7u;
	fm_register = 0x28u;
	write_fm(fm_register, value);
}

/* CC 10 for a sounding FM voice (part in iy): note_level = 7F - its level x the part's effective_volume
   / 2, then the patch's operator levels without bit 7 plus note_level (write_fm_channel); those with bit
   7 are left alone. It reads the patch number from the instrument's byte 1, where every FM instrument
   has 7F (the note on reads byte 2), so the levels come from past the 32 patches of fm_patches. */
void apply_fm_music_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part)
{
	uint8_t channel_code = 0, effective_volume = 0, entry = 0, level_register = 0, operators_left = 0, part_channel = 0, program = 0, register_number = 0, total_level = 0;
	uint16_t fm_register = 0, instrument = 0, level_product = 0, patch_levels = 0, register_list = 0;
	uint8_t saved_operators_left = 0, saved_part_channel = 0;

	z80_block(103, 11);
	channel_code = (uint8_t)chip_channel;
	fm_channel = channel_code;
	part_channel = channel_code & 3u;
	level_product = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
	effective_volume = MIDI_PART(part)->effective_volume;
	level_product = multiply_bytes(effective_volume, level_product);
	z80_block(78, 9);
	note_level = 0x7Fu - ((uint16_t)((uint32_t)((level_product >> 8) >> 1) << 8) >> 8);
	program = MIDI_PART(part)->program;
	instrument = instruments_ADDRESS;
	instrument = read_table_word(instrument, program);
	z80_block(40, 4);
	entry = z80_read8((uint16_t)(instrument + 1u));
	patch_levels = fm_patches_ADDRESS;
	patch_levels = read_table_word(patch_levels, entry);
	z80_block(38, 4);
	patch_levels += 0x15u;
	operators_left = 4u;
	register_list = fm_level_registers_ADDRESS;
	for (;;)
	{
		z80_block(63, 10);
		saved_operators_left = operators_left;
		saved_part_channel = part_channel;
		register_number = FM_REGISTER_NUMBERS(register_list)[0] + part_channel;
		register_list++;
		level_register = register_number;
		total_level = z80_read8(patch_levels);
		patch_levels++;
		if (total_level >> 7 == 0u)
		{
			z80_block(38, 4);
			total_level += note_level;
			fm_register = level_register;
			write_fm_channel(fm_register, total_level);
		}
		z80_block(18, 2);
		part_channel = saved_part_channel;
		operators_left = saved_operators_left - 1u;
		if (operators_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(20, 2);
}

/* As apply_fm_music_volume for CC 7 (part in ix), with the same patch number mistake. */
void apply_fm_channel_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part)
{
	uint8_t channel_code = 0, effective_volume = 0, entry = 0, level_register = 0, operators_left = 0, part_channel = 0, program = 0, register_number = 0, total_level = 0;
	uint16_t fm_register = 0, instrument = 0, level_product = 0, patch_levels = 0, register_list = 0;
	uint8_t saved_operators_left = 0, saved_part_channel = 0;

	z80_block(103, 11);
	channel_code = (uint8_t)chip_channel;
	fm_channel = channel_code;
	part_channel = channel_code & 3u;
	level_product = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
	effective_volume = MIDI_PART(part)->effective_volume;
	level_product = multiply_bytes(effective_volume, level_product);
	z80_block(78, 9);
	note_level = 0x7Fu - ((uint16_t)((uint32_t)((level_product >> 8) >> 1) << 8) >> 8);
	program = MIDI_PART(part)->program;
	instrument = instruments_ADDRESS;
	instrument = read_table_word(instrument, program);
	z80_block(40, 4);
	entry = z80_read8((uint16_t)(instrument + 1u));
	patch_levels = fm_patches_ADDRESS;
	patch_levels = read_table_word(patch_levels, entry);
	z80_block(38, 4);
	patch_levels += 0x15u;
	operators_left = 4u;
	register_list = fm_level_registers_ADDRESS;
	for (;;)
	{
		z80_block(63, 10);
		saved_operators_left = operators_left;
		saved_part_channel = part_channel;
		register_number = FM_REGISTER_NUMBERS(register_list)[0] + part_channel;
		register_list++;
		level_register = register_number;
		total_level = z80_read8(patch_levels);
		patch_levels++;
		if (total_level >> 7 == 0u)
		{
			z80_block(38, 4);
			total_level += note_level;
			fm_register = level_register;
			write_fm_channel(fm_register, total_level);
		}
		z80_block(18, 2);
		part_channel = saved_part_channel;
		operators_left = saved_operators_left - 1u;
		if (operators_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(20, 2);
}

/* Writes the 21 registers of fm_patches[sample_or_patch] (fm_patch_registers plus the channel within the
   part) on part I; hands back the channel in channel_in_part, fm_level_registers in register_list and
   the patch's 4 operator levels in patch_data. */
void load_fm_patch(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data)
{
	uint8_t entry = 0, fm_register = 0, part_channel = 0, patch_register = 0, register_number = 0, registers_left = 0, value = 0;
	uint16_t channel_in_part = 0, patch_data = 0, register_list = 0;
	uint8_t saved_registers_left = 0, saved_part_channel = 0;

	z80_block(51, 5);
	part_channel = channel_code & 3u;
	entry = sample_or_patch;
	patch_data = fm_patches_ADDRESS;
	patch_data = read_table_word(patch_data, entry);
	z80_block(17, 2);
	register_list = fm_patch_registers_ADDRESS;
	registers_left = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_registers_left = registers_left;
		saved_part_channel = part_channel;
		register_number = FM_REGISTER_NUMBERS(register_list)[0] + part_channel;
		register_list++;
		patch_register = register_number;
		value = z80_read8(patch_data);
		patch_data++;
		fm_register = patch_register;
		write_fm(fm_register, value);
		z80_block(18, 2);
		part_channel = saved_part_channel;
		registers_left = saved_registers_left - 1u;
		if (registers_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	channel_in_part = ((uint32_t)registers_left << 8) | part_channel;
	*p_channel_in_part = channel_in_part;
	*p_register_list = register_list;
	*p_patch_data = patch_data;
}

/* As load_fm_patch on part II. */
void load_fm_patch_part2(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data)
{
	uint8_t entry = 0, fm_register = 0, part_channel = 0, patch_register = 0, register_number = 0, registers_left = 0, value = 0;
	uint16_t channel_in_part = 0, patch_data = 0, register_list = 0;
	uint8_t saved_registers_left = 0, saved_part_channel = 0;

	z80_block(51, 5);
	part_channel = channel_code & 3u;
	entry = sample_or_patch;
	patch_data = fm_patches_ADDRESS;
	patch_data = read_table_word(patch_data, entry);
	z80_block(17, 2);
	register_list = fm_patch_registers_ADDRESS;
	registers_left = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_registers_left = registers_left;
		saved_part_channel = part_channel;
		register_number = FM_REGISTER_NUMBERS(register_list)[0] + part_channel;
		register_list++;
		patch_register = register_number;
		value = z80_read8(patch_data);
		patch_data++;
		fm_register = patch_register;
		write_fm_part2(fm_register, value);
		z80_block(18, 2);
		part_channel = saved_part_channel;
		registers_left = saved_registers_left - 1u;
		if (registers_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	channel_in_part = ((uint32_t)registers_left << 8) | part_channel;
	*p_channel_in_part = channel_in_part;
	*p_register_list = register_list;
	*p_patch_data = patch_data;
}
