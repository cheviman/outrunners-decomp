/**
 *    Name:        midi.c
 *    Description: the MIDI messages of each screen's queue for its parts: note on and off, pitch bend, controllers, programs; the voices they take
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* Takes one message off screen 1's MIDI queue each main loop pass, while screen_1_midi_count is not 0:
   its first byte (read_midi_byte_screen_1) is a new screen_1_midi_status, or a data byte under the last
   one; every part of screen 1 that plays (flags bit 7) on the message's channel gets it
   (play_midi_message_screen_1), each reading from the same place (screen_1_midi_read is put back after
   each); then skip_midi_data_screen_1 moves past the message. */
void handle_midi_message_screen_1(void)
{
	uint8_t parts_left = 0, status = 0;
	uint16_t channel_and_index = 0, message_byte = 0, part = 0, screen_page = 0;
	uint8_t saved_parts_left = 0, saved_message_byte = 0;
	uint16_t saved_channel_and_index = 0;

	z80_block(26, 4);
	screen_page = screen_1_midi_count_ADDRESS;
	if (screen_1_midi_count == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	SCREEN_PAGE(screen_page - 0x59u)->midi_count--;
	read_midi_byte_screen_1(&message_byte, &status);
	z80_block(18, 3);
	if (status >> 7 == 0u)
	{
		z80_block(13, 1);
		status = screen_1_midi_status;
	}
	else
	{
		z80_block(23, 2);
		screen_1_midi_status = status;
	}
	z80_block(49, 7);
	channel_and_index = ((status & 0xFu) << 8) | screen_1_midi_read;
	parts_left = 0xDu;
	part = MIDI_PART_ADDRESS(0);
	for (;;)
	{
		z80_block(30, 3);
		if (MIDI_PART(part)->flags >> 7)
		{
			z80_block(33, 4);
			if (channel_and_index >> 8 == MIDI_PART(part)->midi_channel)
			{
				z80_block(39, 3);
				saved_parts_left = parts_left;
				saved_message_byte = (uint8_t)message_byte;
				saved_channel_and_index = channel_and_index;
				play_midi_message_screen_1((uint8_t)message_byte, part);
				z80_block(37, 4);
				channel_and_index = saved_channel_and_index;
				message_byte = saved_message_byte;
				parts_left = saved_parts_left;
				screen_1_midi_read = (uint8_t)channel_and_index;
			}
		}
		z80_block(33, 4);
		part += 0xAu;
		parts_left--;
		if (parts_left == 0u)
			break;
		z80_cycles(5);
	}
	skip_midi_data_screen_1((uint8_t)message_byte);
}

/* Moves screen_1_midi_read to the last byte of the message that starts with first_byte: past a status
   byte, then past one data byte for C0 and D0 (program change, channel pressure) and two for the others.
   play_midi_message_screen_1 does the same for A0 and D0, which play nothing
   (handle_midi_message_screen_1 puts the place back and skips the message itself). */
void skip_midi_data_screen_1(uint8_t first_byte)
{
	uint8_t event = 0;
	uint16_t read_index_address = 0;

	z80_block(28, 4);
	read_index_address = screen_1_midi_read_ADDRESS;
	if ((uint8_t)first_byte >> 7 != 0u)
	{
		z80_block(22, 3);
		event = first_byte & 0xF0u;
		screen_1_midi_read++;
	}
	else
	{
		z80_block(30, 3);
		event = screen_1_midi_status & ~0xFu;
	}
	z80_block(12, 2);
	if (event == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (event == 0xD0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(21, 2);
	screen_1_midi_read++;
}

/* Plays one MIDI message of screen 1's queue for part, by screen_1_midi_status: 90 note on (velocity 0:
   note off): note_number (+ transpose) and its note_code, note_level from the velocity; a drum kit
   (program F0 on, drum_kits) gives drum_pitch, the drum_timeout and the sample (FF: no sound), and on
   channel 15 notes 2A and 2C (closed and pedal hi-hat) stop screen 1's open hi-hat (note code CA); an FM
   instrument (instruments' flags bit 1) takes a free FM voice for start_fm_note; else the instrument's
   key split for note_number gives sample_or_patch, split_tune and split_octave; the MultiPCM voice is
   the next free one from next_voice, or the oldest (allocation_order 0) taken over; its record (flags
   40, + 10 for a drum note), then the pitch, the sample, the level (x the part's effective_volume), pan
   70 (right) and key on. 80 note off: drum notes ignore it; the sounding voice of the note on the
   channel gets the stop flag (an FM one is freed and keyed off), or is held (flags bit 1) while the
   sustain pedal is down. E0 pitch bend: through bend_range_tables[bend_range] into pitch_bend and
   screen_1_pitch_bends, the channel's sounding voices retuned. B0 control change: 1 modulation
   (set_voice_modulation_screen_1), 7 the part's volume, 10 screen_1_music_volume for the 8 music parts,
   40 sustain pedal, 7B all notes off, 50 stops all of the screen's MultiPCM voices (7E: all but drum
   notes on channels other than 15) and clears its channels' controls and half of their pitch bends, 20 a
   sound set (part_templates: the 8 music parts and screen 1's sample bank), 51 stops one note; a new
   volume reaches the sounding voices at once. C0 program change. A0 and D0 skipped. */
void play_midi_message_screen_1(uint8_t message_byte, uint16_t part)
{
	uint32_t carry = 0;
	uint8_t bend = 0, bend_low = 0, bend_range = 0, bend_sign = 0, bend_steps = 0, channel = 0, channel_index = 0, channels_left = 0, chip_channel = 0, control_value = 0, controller = 0, controller_number = 0, controls = 0, depth = 0, drum_note = 0, effective_volume = 0, entry = 0, event = 0, factor = 0, first_data_byte = 0, flags = 0, ignored8 = 0, level = 0, midi_byte = 0, midi_channel = 0, midi_note = 0, modulation_depth = 0, new_banks = 0, note = 0, note_to_mark = 0, old_controls = 0, part_program = 0, parts_left = 0, pcm_register = 0, pitch_bend_high = 0, pitch_bend_low = 0, rounded_volume = 0, sample = 0, stored_note = 0, transposed_note = 0, value = 0, velocity_then_timeout = 0, velocity_then_timeout_2 = 0, voice_channel = 0, voice_chip_channel = 0, voice_note = 0, voices_left = 0, volume = 0, volume_scale = 0;
	uint16_t bend_entry = 0, bend_index = 0, channel_offset = 0, controls_entry = 0, ignored16 = 0, instrument = 0, kept_channel = 0, kit_note = 0, level_product = 0, midi_byte_copy = 0, music_part = 0, note_index = 0, pitch_bends = 0, scaled_part_volume = 0, step_table = 0, template_then_product = 0, velocity_copy = 0, velocity_index = 0, voice = 0, voice_2 = 0, voice_field = 0, voice_size = 0, voices_left_pair = 0;
	uint8_t saved_channel = 0, saved_parts_left = 0;
	uint16_t pushed_voice_field = 0, saved_voice_field = 0;
	uint8_t negated_bend = 0, negated_step = 0, modulation_wheel = 0, wheel_halved = 0, wheel_quartered = 0, part_volume = 0, music_volume = 0;

	first_data_byte = (uint8_t)message_byte;
	z80_block(37, 4);
	event = screen_1_midi_status & ~0xFu;
	if (event == 0x90u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(109, 12);
		midi_note = first_data_byte;
		transposed_note = MIDI_PART(part)->transpose + first_data_byte;
		note_number = transposed_note;
		note_code = note_codes[transposed_note];
		read_midi_byte_screen_1(&velocity_copy, &midi_byte);
		z80_block(14, 2);
		if (midi_byte == 0u)
		{
			release_note_screen_1(part);
			return;
		}
		z80_block(88, 10);
		velocity_index = midi_byte;
		note_level = velocity_levels[velocity_index];
		part_program = MIDI_PART(part)->program;
		if (part_program < 0xF0u)
		{
			z80_block(27, 2);
			instrument = instruments_ADDRESS;
			instrument = read_table_word(instrument, part_program);
			z80_block(22, 3);
			if (((z80_read8(instrument) >> 1) & 1u) != 0u)
			{
				z80_block(59, 7);
				sample_or_patch = z80_read8((uint16_t)(instrument + 2u));
				voice = VOICE_ADDRESS(24);
				voice_size = 9u;
				voices_left = 6u;
				for (;;)
				{
					z80_block(21, 3);
					if (VOICE(voice)->flags == 0u)
						break;
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
					{
						z80_block(14, 2);
						return;
					}
					z80_cycles(5);
				}
				z80_block(182, 22);
				VOICE(voice)->flags = 0x40u;
				voice++;
				fm_channel = VOICE(voice - 0x1u)->chip_channel & 7u;
				voice++;
				stored_note = note_code;
				VOICE(voice - 0x2u)->note = stored_note;
				voice_note = stored_note;
				voice++;
				channel = screen_1_midi_status & 0xFu;
				VOICE(voice - 0x3u)->midi_channel = channel;
				voice_channel = channel;
				level = note_level;
				VOICE(voice - 0x3u)->level = level;
				level_product = (uint32_t)level << 8;
				effective_volume = MIDI_PART(part)->effective_volume;
				level_product = multiply_bytes(effective_volume, level_product);
				z80_block(112, 15);
				note_level = 0x7Fu - ((uint16_t)((uint32_t)((level_product >> 8) >> 1) << 8) >> 8);
				channel_offset = voice_channel;
				voice_modulation = (screen_1_channel_controls[channel_offset] & 0xFu) | 0x40u;
				pitch_bends = screen_1_pitch_bends_ADDRESS;
				note = voice_note;
				start_fm_note(note, channel_offset, pitch_bends);
				return;
			}
			for (;;)
			{
				z80_block(44, 6);
				instrument++;
				if (note_number <= z80_read8(instrument))
					break;
				z80_block(28, 4);
				instrument += 3u;
			}
			z80_block(78, 9);
			instrument++;
			sample_or_patch = z80_read8(instrument);
			instrument++;
			split_tune = z80_read8(instrument);
			split_octave = z80_read8((uint16_t)(instrument + 1u));
			take_voice_screen_1((uint8_t)velocity_copy, part);
			return;
		}
		z80_block(34, 3);
		part_program += 0x10u;
		kit_note = drum_kits_ADDRESS;
		kit_note = read_table_word(kit_note, part_program);
		z80_block(112, 14);
		note_index = midi_note;
		kit_note = kit_note + note_index + note_index + note_index;
		drum_pitch = z80_read8(kit_note);
		kit_note++;
		velocity_then_timeout = z80_read8(kit_note);
		sample = z80_read8((uint16_t)(kit_note + 1u));
		sample_or_patch = sample;
		if (sample == 0xFFu)
		{
			z80_cycles(6);
			return;
		}
		z80_block(37, 4);
		if ((screen_1_midi_status & 0xFu) != 0xFu)
		{
			velocity_then_timeout_2 = velocity_then_timeout;
			take_voice_screen_1(velocity_then_timeout_2, part);
			return;
		}
		z80_block(21, 3);
		drum_note = midi_note;
		if (drum_note != 0x2Cu)
		{
			z80_block(17, 2);
			if (drum_note != 0x2Au)
			{
				velocity_then_timeout_2 = velocity_then_timeout;
				take_voice_screen_1(velocity_then_timeout_2, part);
				return;
			}
		}
		velocity_then_timeout_2 = velocity_then_timeout;
		cut_open_hi_hat_screen_1(velocity_then_timeout_2, part);
		return;
	}
	z80_block(17, 2);
	if (event == 0x80u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(96, 11);
		note_code = note_codes[(uint8_t)(MIDI_PART(part)->transpose + first_data_byte)];
		screen_1_midi_read++;
		release_note_screen_1(part);
		return;
	}
	z80_block(17, 2);
	if (event == 0xE0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		bend_low = first_data_byte;
		read_midi_byte_screen_1(&ignored16, &midi_byte);
		z80_block(26, 5);
		carry = midi_byte & 1u;
		bend_sign = midi_byte >> 1;
		bend = bend_low;
		if (carry)
		{
			z80_block(7, 1);
			bend |= 0x80u;
		}
		z80_block(78, 18);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		if ((bend_sign & 1u) == 0u)
		{
			z80_block(18, 3);
			negated_bend = ~bend + 1u;
			bend = negated_bend;
			if (negated_bend == 0u)
			{
				z80_block(4, 1);
				bend--;
			}
		}
		z80_block(57, 6);
		bend_index = bend;
		bend_range = MIDI_PART(part)->bend_range;
		step_table = bend_range_tables_ADDRESS;
		step_table = read_table_word(step_table, bend_range);
		z80_block(36, 5);
		bend_steps = z80_read8((uint16_t)(step_table + bend_index));
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			bend_sign--;
			negated_step = ~bend_steps + 1u;
			bend_steps = negated_step;
			if (negated_step == 0u)
			{
				z80_block(4, 1);
				bend_steps--;
			}
		}
		z80_block(145, 19);
		pitch_bend_high = bend_sign;
		pitch_bend_low = bend_steps;
		pitch_bend = ((uint32_t)pitch_bend_high << 8) | pitch_bend_low;
		bend_entry = screen_1_pitch_bends_ADDRESS + (uint8_t)((screen_1_midi_status & 0xFu) * 2u);
		z80_write8(bend_entry, pitch_bend_low);
		z80_write8((uint16_t)(bend_entry + 1u), pitch_bend_high);
		voice_2 = VOICE_ADDRESS(0);
		voices_left = 0x1Eu;
		channel = screen_1_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (channel == VOICE(voice_2)->midi_channel)
			{
				z80_block(48, 6);
				saved_channel = channel;
				flags = VOICE(voice_2)->flags;
				if (((flags >> 3) & 1u) == 0u)
				{
					z80_block(17, 2);
					if ((flags & 0x7Fu) != 0u)
					{
						z80_cycles(7);
						retune_voice_screen_1(voice_2);
					}
				}
				z80_block(10, 1);
				channel = saved_channel;
			}
			z80_block(33, 4);
			voice_2 += 9u;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (event == 0xB0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		controller_number = first_data_byte;
		read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
		control_value = (uint8_t)midi_byte_copy;
		z80_block(21, 3);
		controller = controller_number;
		carry = controller == 0u;
		if (controller == 1u)
		{
			z80_block(123, 19);
			modulation_wheel = control_value;
			depth = (modulation_wheel >> 1) | (carry << 7);
			carry = modulation_wheel & 1u;
			wheel_halved = depth;
			depth = (wheel_halved >> 1) | (carry << 7);
			carry = wheel_halved & 1u;
			wheel_quartered = depth;
			depth = (wheel_quartered >> 1) | (carry << 7);
			carry = wheel_quartered & 1u;
			depth = ((depth >> 1) | (carry << 7)) & 7u;
			modulation_depth = depth;
			voice_modulation = depth;
			controls_entry = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
			controls = z80_read8(controls_entry);
			old_controls = controls;
			if ((controls & 0x7Fu) == modulation_depth)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(controls_entry, (old_controls & 0x80u) | modulation_depth);
			voice = voice_0_midi_channel_ADDRESS;
			voices_left = 0x1Eu;
			channel = screen_1_midi_status & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (channel == VOICE(voice - 0x3u)->midi_channel)
				{
					z80_block(47, 8);
					kept_channel = (uint32_t)channel << 8;
					voice = voice - 1u - 1u - 1u;
					flags = VOICE(voice)->flags;
					if (((flags >> 3) & 1u) == 0u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							voices_left_pair = (uint32_t)voices_left << 8;
							set_voice_modulation_screen_1(part, &voices_left_pair, &voice);
							voices_left = (uint8_t)(voices_left_pair >> 8);
						}
					}
					z80_block(22, 4);
					voice += 3u;
					channel = (uint8_t)(kept_channel >> 8);
				}
				z80_block(29, 3);
				voice += 9u;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 7u)
		{
			z80_block(18, 3);
			part_volume = control_value * 2u;
			volume = part_volume;
			if (part_volume != 0u)
			{
				z80_block(4, 1);
				volume++;
			}
			z80_block(60, 7);
			MIDI_PART(part)->volume = volume;
			scaled_part_volume = (uint32_t)volume << 8;
			volume_scale = 0xFFu;
			if ((MIDI_PART(part)->flags & 1u) == 0u)
			{
				z80_block(13, 1);
				volume_scale = screen_1_music_volume;
			}
			z80_block(21, 2);
			factor = volume_scale;
			scaled_part_volume = multiply_bytes(factor, scaled_part_volume);
			z80_block(18, 3);
			rounded_volume = (uint8_t)(scaled_part_volume >> 8);
			if (rounded_volume != 0u)
			{
				z80_block(4, 1);
				rounded_volume++;
			}
			z80_block(36, 4);
			MIDI_PART(part)->effective_volume = rounded_volume;
			voice_field = VOICE_ADDRESS(0);
			voices_left = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				pushed_voice_field = voice_field;
				if ((VOICE(voice_field)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((VOICE(voice_field)->flags >> 3) & 1u) == 0u)
					{
						z80_block(55, 7);
						voice_field += 3u;
						if ((screen_1_midi_status & 0xFu) == VOICE(voice_field - 0x3u)->midi_channel)
						{
							z80_block(37, 6);
							voice_field = voice_field - 1u - 1u;
							voice_chip_channel = VOICE(voice_field - 0x1u)->chip_channel;
							if (voice_chip_channel >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(pcm_data) & 1u);
								z80_block(81, 8);
								z80_write8(pcm_voice_select, voice_chip_channel);
								voice_field = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
								effective_volume = MIDI_PART(part)->effective_volume;
								voice_field = multiply_bytes(effective_volume, voice_field);
								z80_block(39, 5);
								value = ~(voice_field >> 8) & 0xFEu;
								pcm_register = 5u;
								write_pcm_register(pcm_register, value);
							}
							else
							{
								z80_block(17, 1);
								chip_channel = voice_chip_channel;
								apply_fm_channel_volume(chip_channel, voice_field, part);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				voice_field = pushed_voice_field + 9u;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x10u)
		{
			z80_block(18, 3);
			music_volume = control_value * 2u;
			volume = music_volume;
			if (music_volume != 0u)
			{
				z80_block(4, 1);
				volume++;
			}
			z80_block(34, 4);
			screen_1_music_volume = volume;
			music_part = MIDI_PART_ADDRESS(0);
			parts_left = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_parts_left = parts_left;
				scaled_part_volume = (uint32_t)MIDI_PART(music_part)->volume << 8;
				volume_scale = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					volume_scale = screen_1_music_volume;
				}
				z80_block(21, 2);
				factor = volume_scale;
				scaled_part_volume = multiply_bytes(factor, scaled_part_volume);
				z80_block(18, 3);
				rounded_volume = (uint8_t)(scaled_part_volume >> 8);
				if (rounded_volume != 0u)
				{
					z80_block(4, 1);
					rounded_volume++;
				}
				z80_block(36, 4);
				MIDI_PART(music_part)->effective_volume = rounded_volume;
				voice_field = voice_0_midi_channel_ADDRESS;
				voices_left = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_voice_field = voice_field;
					if (MIDI_PART(music_part)->midi_channel == VOICE(voice_field - 0x3u)->midi_channel)
					{
						z80_block(40, 6);
						voice_field = voice_field - 1u - 1u - 1u;
						if ((VOICE(voice_field)->flags >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((VOICE(voice_field)->flags >> 3) & 1u) == 0u)
							{
								z80_block(31, 5);
								voice_field++;
								voice_chip_channel = VOICE(voice_field - 0x1u)->chip_channel;
								if (voice_chip_channel >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(pcm_data) & 1u);
									z80_block(81, 8);
									z80_write8(pcm_voice_select, voice_chip_channel);
									voice_field = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
									effective_volume = MIDI_PART(music_part)->effective_volume;
									voice_field = multiply_bytes(effective_volume, voice_field);
									z80_block(39, 5);
									value = ~(voice_field >> 8) & 0xFEu;
									pcm_register = 5u;
									write_pcm_register(pcm_register, value);
								}
								else
								{
									z80_block(17, 1);
									chip_channel = voice_chip_channel;
									apply_fm_music_volume(chip_channel, voice_field, music_part);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					voice_field = saved_voice_field + 9u;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				music_part += 0xAu;
				parts_left = saved_parts_left - 1u;
				if (parts_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x40u)
		{
			z80_block(21, 3);
			if (control_value >= 0x40u)
			{
				z80_block(77, 9);
				controls_entry = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
				z80_write8(controls_entry, z80_read8(controls_entry) | 0x80u);
				return;
			}
			z80_block(51, 6);
			voice_2 = VOICE_ADDRESS(0);
			voice_size = 9u;
			voices_left = 0x1Eu;
			channel = screen_1_midi_status & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (channel == VOICE(voice_2)->midi_channel)
				{
					z80_block(41, 6);
					channel_index = channel;
					flags = VOICE(voice_2)->flags;
					if ((flags >> 1) & 1u)
					{
						z80_block(18, 3);
						if (((flags >> 3) & 1u) == 0u)
						{
							z80_block(37, 5);
							if (VOICE(voice_2)->chip_channel >> 7 == 0u)
							{
								z80_block(23, 2);
								VOICE(voice_2)->flags |= 1u;
							}
							else
							{
								z80_block(17, 1);
								stop_fm_voice(voice_2);
								z80_block(10, 1);
							}
						}
					}
					z80_block(4, 1);
					channel = (uint8_t)channel_index;
				}
				z80_block(23, 3);
				voice_2 += voice_size;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			controls_entry = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
			z80_write8(controls_entry, z80_read8(controls_entry) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x7Bu)
		{
			z80_block(47, 5);
			channel = screen_1_midi_status & 0xFu;
			voice = VOICE_ADDRESS(0);
			voice_size = 9u;
			voices_left = 0x18u;
			for (;;)
			{
				z80_block(22, 3);
				if ((VOICE(voice)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((VOICE(voice)->flags >> 3) & 1u) == 0u)
					{
						z80_block(35, 5);
						voice += 3u;
						if (channel != VOICE(voice - 0x3u)->midi_channel)
						{
							z80_block(18, 3);
							voice = voice - 1u - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							voice = voice - 1u - 1u - 1u;
							VOICE(voice)->flags |= 1u;
						}
					}
				}
				z80_block(19, 2);
				voice += voice_size;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x50u)
		{
			z80_block(21, 3);
			if (control_value == 0x7Eu)
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				voice_size = 9u;
				voices_left = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if (((VOICE(voice)->flags >> 3) & 1u) == 0u)
					{
						z80_block(22, 3);
						if ((VOICE(voice)->flags >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((VOICE(voice)->flags >> 4) & 1u) == 0u)
							{
								z80_block(15, 2);
								VOICE(voice)->flags |= 1u;
							}
							else
							{
								z80_block(60, 9);
								voice += 3u;
								midi_channel = VOICE(voice - 0x3u)->midi_channel;
								voice = voice - 1u - 1u - 1u;
								if (midi_channel == 0xFu)
								{
									z80_block(15, 2);
									VOICE(voice)->flags |= 1u;
								}
							}
						}
					}
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				voice_size = 9u;
				voices_left = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if (((VOICE(voice)->flags >> 3) & 1u) == 0u)
					{
						z80_block(22, 3);
						if ((VOICE(voice)->flags >> 6) & 1u)
						{
							z80_block(15, 2);
							VOICE(voice)->flags |= 1u;
						}
					}
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			controls_entry = screen_1_channel_controls_ADDRESS;
			channels_left = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(controls_entry, 0u);
				controls_entry++;
				channels_left--;
				if (channels_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			bend_entry = screen_1_pitch_bends_ADDRESS;
			channels_left = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(bend_entry, 0u);
				bend_entry++;
				channels_left--;
				if (channels_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x20u)
		{
			z80_block(31, 3);
			entry = control_value;
			template_then_product = part_templates_ADDRESS;
			template_then_product = read_table_word(template_then_product, entry);
			z80_block(112, 13);
			move_block(template_then_product, MIDI_PART_ADDRESS(0), 0x50u, 1u);
			new_banks = (pcm_banks & 0x38u) | z80_read8((uint16_t)(template_then_product + 0x50u));
			port_out(PCM_BANKS, new_banks);
			pcm_banks = new_banks;
			music_part = MIDI_PART_ADDRESS(0);
			parts_left = 8u;
			for (;;)
			{
				z80_block(56, 6);
				template_then_product = (uint32_t)MIDI_PART(music_part)->volume << 8;
				volume_scale = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					volume_scale = screen_1_music_volume;
				}
				z80_block(21, 2);
				factor = volume_scale;
				template_then_product = multiply_bytes(factor, template_then_product);
				z80_block(18, 3);
				rounded_volume = (uint8_t)(template_then_product >> 8);
				if (rounded_volume != 0u)
				{
					z80_block(4, 1);
					rounded_volume++;
				}
				z80_block(52, 6);
				MIDI_PART(music_part)->effective_volume = rounded_volume;
				music_part += 0xAu;
				parts_left--;
				if (parts_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		note_to_mark = note_codes[control_value];
		voice = VOICE_ADDRESS(0);
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(22, 3);
			if ((VOICE(voice)->flags >> 6) & 1u)
			{
				z80_block(22, 3);
				if (((VOICE(voice)->flags >> 3) & 1u) == 0u)
				{
					z80_block(33, 5);
					voice += 2u;
					if (note_to_mark != VOICE(voice - 0x2u)->note)
					{
						z80_block(12, 2);
						voice = voice - 1u - 1u;
					}
					else
					{
						z80_block(43, 5);
						voice++;
						if ((screen_1_midi_status & 0xFu) != VOICE(voice - 0x3u)->midi_channel)
						{
							z80_block(6, 1);
							voice--;
							z80_block(12, 2);
							voice = voice - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							voice = voice - 1u - 1u - 1u;
							VOICE(voice)->flags |= 1u;
						}
					}
				}
			}
			z80_block(29, 3);
			voice += 9u;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (event == 0xC0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(29, 3);
		MIDI_PART(part)->program = first_data_byte;
		return;
	}
	z80_block(17, 2);
	if (event != 0xD0u)
	{
		z80_block(17, 2);
		if (event != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	skip_midi_data_screen_1(message_byte);
}

/* A closed or pedal hi-hat (notes 2A and 2C on channel 15) stops screen 1's open hi-hat: each MultiPCM
   voice of screen 1 (flags bit 6, not bit 3) on channel 15 playing note code CA gets the stop flag (bit
   0); then take_voice_screen_1 for the new note. */
void cut_open_hi_hat_screen_1(uint8_t velocity_then_timeout, uint16_t part)
{
	uint8_t midi_channel = 0, voices_left = 0;
	uint16_t voice = 0, voice_size = 0;

	z80_block(27, 3);
	voice = voice_0_note_ADDRESS;
	voice_size = 9u;
	voices_left = 0x18u;
	for (;;)
	{
		z80_block(24, 3);
		if (VOICE(voice - 0x2u)->note == 0xCAu)
		{
			z80_block(36, 5);
			voice++;
			midi_channel = VOICE(voice - 0x3u)->midi_channel;
			voice--;
			if (midi_channel == 0xFu)
			{
				z80_block(34, 5);
				voice = voice - 1u - 1u;
				if ((VOICE(voice)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((VOICE(voice)->flags >> 3) & 1u) == 0u)
					{
						z80_block(15, 2);
						VOICE(voice)->flags |= 1u;
					}
				}
				z80_block(12, 2);
				voice += 2u;
			}
		}
		z80_block(19, 2);
		voice += voice_size;
		voices_left--;
		if (voices_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	take_voice_screen_1(velocity_then_timeout, part);
}

/* A MultiPCM voice for a note of screen 1 (play_midi_message_screen_1 and cut_open_hi_hat_screen_1 go on
   to it): the next free one from next_voice (reset_next_voice past the last), or else the oldest
   (allocation_order 0) taken over, the others' orders one down (none: no note); its record (flags 40, +
   10 and the drum_timeout for a drum note), then the pitch (an instrument's through pcm_pitch_tables,
   with its channel's pitch bend, split_tune and split_octave; a drum's drum_pitch), the sample, the
   level (x the part's effective_volume), pan 70 (right) and key on. */
void take_voice_screen_1(uint8_t velocity_then_timeout, uint16_t part)
{
	uint8_t channel = 0, drum_timeout = 0, effective_volume = 0, oldest_order = 0, order = 0, pcm_register = 0, stored_note = 0, value = 0, voice_channel = 0, voice_note = 0, voices_left = 0;
	uint16_t bend_entry = 0, channel_offset = 0, level_product = 0, new_voice_field = 0, pitch = 0, pitch_entry = 0, pitch_table_entry = 0, tune = 0, voice = 0, voice_size = 0;
	uint16_t pushed_voice = 0;

	drum_timeout = (uint8_t)velocity_then_timeout;
	z80_block(33, 3);
	voice = next_voice;
	voice_size = 9u;
	voices_left = 0x18u;
	for (;;)
	{
		z80_block(21, 3);
		if ((uint8_t)voice == 0xE8u)
		{
			z80_cycles(7);
			voice = reset_next_voice();
		}
		z80_block(21, 3);
		oldest_order = 0u;
		if (0u == VOICE(voice)->flags)
		{
			z80_block(58, 5);
			pushed_voice = voice;
			next_voice = voice + voice_size;
			new_voice_field = pushed_voice;
			break;
		}
		z80_block(19, 2);
		voice += voice_size;
		voices_left--;
		if (voices_left != 0u)
		{
			z80_cycles(5);
			continue;
		}
		z80_block(10, 1);
		z80_block(17, 2);
		voice = voice_0_allocation_order_ADDRESS;
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(17, 2);
			if (oldest_order == VOICE(voice - 0x6u)->allocation_order)
				break;
			z80_block(19, 2);
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
			{
				z80_block(14, 2);
				return;
			}
			z80_cycles(5);
		}
		z80_block(42, 5);
		VOICE(voice - 0x6u)->allocation_order = 0x18u;
		pushed_voice = voice;
		voice = voice_0_allocation_order_ADDRESS;
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(30, 3);
			VOICE(voice - 0x6u)->allocation_order--;
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(52, 5);
		pcm_voices_used--;
		new_voice_field = pushed_voice - 6u;
		break;
	}
	z80_block(46, 5);
	VOICE(new_voice_field)->flags = 0x40u;
	if (MIDI_PART(part)->program < 0xF0u)
	{
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(479, 64);
		new_voice_field++;
		z80_write8(pcm_voice_select, VOICE(new_voice_field - 0x1u)->chip_channel);
		new_voice_field++;
		stored_note = note_code;
		VOICE(new_voice_field - 0x2u)->note = stored_note;
		voice_note = stored_note;
		new_voice_field++;
		channel = screen_1_midi_status & 0xFu;
		VOICE(new_voice_field - 0x3u)->midi_channel = channel;
		voice_channel = channel;
		new_voice_field += 2u;
		VOICE(new_voice_field - 0x5u)->level = note_level;
		new_voice_field++;
		order = pcm_voices_used;
		VOICE(new_voice_field - 0x6u)->allocation_order = order;
		pcm_voices_used = order + 1u;
		new_voice_field++;
		VOICE(new_voice_field - 0x7u)->tune = split_tune;
		VOICE(new_voice_field - 0x7u)->octave_shift = split_octave;
		channel_offset = (uint8_t)(voice_channel * 2u);
		bend_entry = screen_1_pitch_bends_ADDRESS + channel_offset;
		tune = split_tune;
		pushed_voice = z80_read16(bend_entry) + tune;
		pitch_table_entry = pcm_pitch_tables_ADDRESS + (uint8_t)((voice_note & 0xFu) * 2u);
		pitch_entry = PCM_PITCH_RUNS(pitch_table_entry)[0] + pushed_voice + pushed_voice;
		pitch = PCM_PITCH_STEPS(pitch_entry)[0];
		pitch = (pitch & 0xFFu) | ((uint32_t)(uint8_t)((voice_note & ~0xFu) + (pitch >> 8)) << 8);
		value = split_octave + (pitch >> 8);
	}
	else
	{
		z80_block(15, 2);
		VOICE(new_voice_field)->flags |= 0x10u;
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(171, 21);
		new_voice_field++;
		z80_write8(pcm_voice_select, VOICE(new_voice_field - 0x1u)->chip_channel);
		new_voice_field++;
		VOICE(new_voice_field - 0x2u)->note = note_code;
		new_voice_field++;
		VOICE(new_voice_field - 0x3u)->midi_channel = screen_1_midi_status & 0xFu;
		new_voice_field++;
		VOICE(new_voice_field - 0x4u)->drum_timeout = drum_timeout;
		new_voice_field++;
		VOICE(new_voice_field - 0x5u)->level = note_level;
		new_voice_field++;
		if ((screen_1_midi_status & 0xFu) == 0xFu)
		{
			z80_block(37, 4);
			order = pcm_voices_used;
			VOICE(new_voice_field - 0x6u)->allocation_order = order;
			pcm_voices_used = order + 1u;
		}
		else
		{
			z80_block(24, 3);
			VOICE(new_voice_field - 0x6u)->allocation_order = 0xFFu;
		}
		z80_block(30, 3);
		value = drum_pitch;
		pitch = 0u;
	}
	z80_block(24, 2);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(28, 3);
	value = (uint8_t)pitch;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(37, 3);
	value = sample_or_patch;
	pcm_register = 1u;
	write_pcm_register(pcm_register, value);
	z80_block(53, 5);
	level_product = (uint32_t)note_level << 8;
	effective_volume = MIDI_PART(part)->effective_volume;
	level_product = multiply_bytes(effective_volume, level_product);
	z80_block(39, 5);
	value = ~(level_product >> 8) | 1u;
	pcm_register = 5u;
	write_pcm_register(pcm_register, value);
	z80_block(31, 3);
	value = 0x70u;
	pcm_register = 0u;
	write_pcm_register(pcm_register, value);
	z80_block(24, 3);
	value = 0x80u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
}

/* Releases a note of screen 1 (note off, or a note on at velocity 0: play_midi_message_screen_1 goes on
   to it): drum notes (program F0 on) ignore it; the voice playing the note on its channel (an FM voice
   for an FM instrument, else a MultiPCM one; screen 1's, sounding, not held) is held (flags bit 1) while
   the channel's sustain pedal is down, else the MultiPCM voice gets the stop flag (bit 0) and the FM
   voice is freed and keyed off. */
void release_note_screen_1(uint16_t part)
{
	uint8_t channel = 0, channel_index = 0, flags = 0, fm_register = 0, program = 0, value = 0, voices_left = 0;
	uint16_t instrument = 0, voice = 0, voice_size = 0;

	z80_block(31, 4);
	program = MIDI_PART(part)->program;
	if (program >= 0xF0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 2);
	instrument = instruments_ADDRESS;
	instrument = read_table_word(instrument, program);
	z80_block(22, 3);
	if (((z80_read8(instrument) >> 1) & 1u) != 0u)
	{
		z80_block(51, 6);
		voice = VOICE_ADDRESS(24);
		voice_size = 9u;
		voices_left = 6u;
		channel = screen_1_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (channel == VOICE(voice)->midi_channel)
			{
				z80_block(46, 5);
				channel_index = channel;
				if (note_code == VOICE(voice)->note)
				{
					z80_block(37, 5);
					flags = VOICE(voice)->flags;
					if (((flags >> 1) & 1u) == 0u)
					{
						z80_block(18, 3);
						if (((flags >> 3) & 1u) == 0u)
						{
							z80_block(17, 2);
							if ((flags & 0x7Fu) != 0u)
								break;
						}
					}
				}
				z80_block(4, 1);
				channel = (uint8_t)channel_index;
			}
			z80_block(23, 3);
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
			{
				z80_block(10, 1);
				return;
			}
			z80_cycles(5);
		}
		z80_block(65, 9);
		if (screen_1_channel_controls[channel_index] >> 7)
		{
			z80_block(33, 3);
			VOICE(voice)->flags |= 2u;
			return;
		}
		z80_block(85, 10);
		VOICE(voice)->flags = 0u;
		VOICE(voice)->midi_channel = 0u;
		value = VOICE(voice)->chip_channel & 7u;
		fm_register = 0x28u;
		write_fm(fm_register, value);
		return;
	}
	z80_block(51, 6);
	voice = VOICE_ADDRESS(0);
	voice_size = 9u;
	voices_left = 0x18u;
	channel = screen_1_midi_status & 0xFu;
	for (;;)
	{
		z80_block(29, 3);
		if (channel == VOICE(voice)->midi_channel)
		{
			z80_block(46, 5);
			channel_index = channel;
			if (note_code == VOICE(voice)->note)
			{
				z80_block(37, 5);
				flags = VOICE(voice)->flags;
				if (((flags >> 1) & 1u) == 0u)
				{
					z80_block(18, 3);
					if (((flags >> 3) & 1u) == 0u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
							break;
					}
				}
			}
			z80_block(4, 1);
			channel = (uint8_t)channel_index;
		}
		z80_block(23, 3);
		voice += voice_size;
		voices_left--;
		if (voices_left == 0u)
		{
			z80_block(10, 1);
			return;
		}
		z80_cycles(5);
	}
	z80_block(65, 9);
	if (screen_1_channel_controls[channel_index] >> 7 == 0u)
	{
		z80_block(33, 3);
		VOICE(voice)->flags |= 1u;
		return;
	}
	z80_block(33, 3);
	VOICE(voice)->flags |= 2u;
}

/* Retunes a sounding voice after a pitch bend of screen 1: an FM voice through retune_fm_voice; a
   MultiPCM voice gets its note's pitch again from pcm_pitch_tables[semitone] at pitch_bend + its tune,
   its octave + octave_shift (registers 3 and 2). The same code as retune_voice_screen_2. */
void retune_voice_screen_1(uint16_t voice)
{
	uint8_t chip_channel = 0, chip_code = 0, note = 0, pcm_register = 0, value = 0;
	uint16_t bend = 0, pitch = 0, pitch_entry = 0, pitch_table_entry = 0, tune = 0;

	z80_block(37, 5);
	chip_channel = VOICE(voice)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		retune_fm_voice(voice, chip_channel);
		return;
	}
	z80_block(4, 1);
	chip_code = chip_channel;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(291, 38);
	z80_write8(pcm_voice_select, chip_code);
	note = VOICE(voice)->note;
	pitch_table_entry = pcm_pitch_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	bend = pitch_bend;
	pitch_entry = PCM_PITCH_RUNS(pitch_table_entry)[0] + bend + bend;
	tune = VOICE(voice)->tune;
	pitch_entry = pitch_entry + tune + tune;
	pitch = PCM_PITCH_STEPS(pitch_entry)[0];
	pitch = (pitch & 0xFFu) | ((uint32_t)(uint8_t)((note & ~0xFu) + (pitch >> 8)) << 8);
	value = VOICE(voice)->octave_shift + (pitch >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(21, 3);
	value = (uint8_t)pitch;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
}

/* CC 1 of screen 1 for a sounding voice of the channel: a MultiPCM voice gets register 6 =
   voice_modulation | the part's lfo_frequency. For an FM voice the program means B4 = 40 | the depth,
   but takes the channel from voices_left, the caller's count of voices left, instead of the voice's
   chip_channel, and write_fm_channel hands the value back where that count was, so the caller's walk
   goes on past the 30 voices. */
void set_voice_modulation_screen_1(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice)
{
	uint16_t voices_left = (uint16_t)*p_voices_left;
	uint16_t voice = (uint16_t)*p_voice;
	uint8_t chip_channel = 0, count_as_channel = 0, lfo_register = 0, pan_register = 0, value = 0;

	z80_block(31, 5);
	voice++;
	chip_channel = VOICE(voice - 0x1u)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		z80_block(79, 9);
		count_as_channel = (uint8_t)(voices_left >> 8);
		fm_channel = count_as_channel;
		pan_register = (count_as_channel & 3u) - 0x4Cu;
		value = (voice_modulation & 0x3Fu) | 0x40u;
		voices_left = pan_register;
		voices_left = write_fm_channel(voices_left, value);
		z80_block(16, 2);
		voice--;
		voices_left = (voices_left & ~0xFFu) | pan_register;
	}
	else
	{
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(73, 7);
		z80_write8(pcm_voice_select, chip_channel);
		lfo_register = 6u;
		value = voice_modulation | MIDI_PART(part)->lfo_frequency;
		voices_left = (voices_left & ~0xFFu) | 6u;
		write_pcm_register((uint8_t)voices_left, value);
		z80_block(16, 2);
		voice--;
		voices_left = (voices_left & ~0xFFu) | lfo_register;
	}
	*p_voices_left = voices_left;
	*p_voice = voice;
}

/* Moves screen_1_midi_read on by one and hands back the byte of screen_1_midi_queue there. */
void read_midi_byte_screen_1(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte)
{
	uint8_t midi_byte = 0;
	uint16_t midi_byte_copy = 0;

	z80_block(56, 7);
	screen_1_midi_read++;
	midi_byte = z80_read8(screen_1_midi_read | 0xE100u);
	midi_byte_copy = midi_byte;
	*p_midi_byte_copy = (*p_midi_byte_copy & 0xFF00u) | midi_byte_copy;
	*p_midi_byte = midi_byte;
}

/* Stops the MultiPCM voices flagged to stop (flags bit 0), first thing each main loop pass: the voice is
   freed (flags and midi_channel 0) and keyed off; unless it was a drum note on a channel other than 15
   (which has no allocation order), pcm_voices_used goes down by one and the voices allocated after it
   move one down. */
void stop_flagged_voices(void)
{
	uint8_t key_off_value = 0, midi_channel = 0, own_order = 0, pcm_register = 0, voice_flags = 0, voices_left = 0;
	uint16_t voice = 0, voice_field = 0, voice_size = 0;
	uint8_t saved_flags = 0, saved_voices_left = 0;
	uint16_t saved_voice_field = 0;

	z80_block(27, 3);
	voice_field = VOICE_ADDRESS(0);
	voice_size = 9u;
	voices_left = 0x18u;
	for (;;)
	{
		z80_block(22, 3);
		if (VOICE(voice_field)->flags & 1u)
		{
			z80_block(51, 6);
			saved_voices_left = voices_left;
			saved_voice_field = voice_field;
			saved_flags = VOICE(voice_field)->flags;
			VOICE(voice_field)->flags = 0u;
			do
			{
				z80_block(27, 3);
			} while (z80_read8(pcm_data) & 1u);
			z80_block(54, 6);
			voice_field++;
			z80_write8(pcm_voice_select, VOICE(voice_field - 0x1u)->chip_channel);
			key_off_value = 0u;
			pcm_register = 4u;
			write_pcm_register(pcm_register, key_off_value);
			z80_block(79, 12);
			voice_field += 2u;
			midi_channel = VOICE(voice_field - 0x3u)->midi_channel;
			VOICE(voice_field - 0x3u)->midi_channel = key_off_value;
			voice_field += 3u;
			voice_flags = saved_flags;
			if (midi_channel == 0xFu)
			{
				z80_block(55, 6);
				own_order = VOICE(voice_field - 0x6u)->allocation_order;
				pcm_voices_used--;
				voice = voice_0_allocation_order_ADDRESS;
				voice_size = 9u;
				voices_left = 0x18u;
				for (;;)
				{
					z80_block(17, 2);
					if (own_order < VOICE(voice - 0x6u)->allocation_order)
					{
						z80_block(11, 1);
						VOICE(voice - 0x6u)->allocation_order--;
					}
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
			}
			else
			{
				z80_block(22, 4);
				if (((voice_flags >> 4) & 1u) == 0u)
				{
					z80_block(55, 6);
					own_order = VOICE(voice_field - 0x6u)->allocation_order;
					pcm_voices_used--;
					voice = voice_0_allocation_order_ADDRESS;
					voice_size = 9u;
					voices_left = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (own_order < VOICE(voice - 0x6u)->allocation_order)
						{
							z80_block(11, 1);
							VOICE(voice - 0x6u)->allocation_order--;
						}
						z80_block(19, 2);
						voice += voice_size;
						voices_left--;
						if (voices_left == 0u)
							break;
						z80_cycles(5);
					}
				}
			}
			z80_block(30, 3);
			voice_field = saved_voice_field;
			voices_left = saved_voices_left;
			voice_size = 9u;
		}
		z80_block(19, 2);
		voice_field += voice_size;
		voices_left--;
		if (voices_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

/* Hands back the high byte of value times the low byte of factor (8 x 8 bits, shift and add); the
   callers keep the product's high byte: a level or a volume scaled by another. */
uint16_t multiply_bytes(uint16_t factor, uint16_t value)
{
	uint32_t carry = 0;

	z80_block(35, 4);
	factor &= 0xFFu;
	value &= ~0xFFu;
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(21, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry)
	{
		z80_block(11, 1);
		value += factor;
	}
	z80_block(16, 2);
	carry = (int16_t)value < 0;
	value = value * 2u;
	if (carry == 0u)
	{
		z80_cycles(6);
		return value;
	}
	z80_block(21, 2);
	value += factor;
	return value;
}

/* Puts next_voice back on the first voice and hands it back: the free voice search wraps after the 24
   MultiPCM voices. */
uint16_t reset_next_voice(void)
{
	uint16_t first_voice = 0;

	z80_block(36, 3);
	first_voice = VOICE_ADDRESS(0);
	next_voice = VOICE_ADDRESS(0);
	return first_voice;
}

/* As handle_midi_message_screen_1 for screen 2: screen_2_midi_count, screen_2_midi_status,
   screen_2_midi_read, screen 2's parts (midi_part 13-25), play_midi_message_screen_2 and
   skip_midi_data_screen_2. */
void handle_midi_message_screen_2(void)
{
	uint8_t parts_left = 0, status = 0;
	uint16_t channel_and_index = 0, message_byte = 0, part = 0, screen_page = 0;
	uint8_t saved_parts_left = 0, saved_message_byte = 0;
	uint16_t saved_channel_and_index = 0;

	z80_block(26, 4);
	screen_page = screen_2_midi_count_ADDRESS;
	if (screen_2_midi_count == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	SCREEN_PAGE(screen_page - 0x59u)->midi_count--;
	read_midi_byte_screen_2(&message_byte, &status);
	z80_block(18, 3);
	if (status >> 7 == 0u)
	{
		z80_block(13, 1);
		status = screen_2_midi_status;
	}
	else
	{
		z80_block(23, 2);
		screen_2_midi_status = status;
	}
	z80_block(49, 7);
	channel_and_index = ((status & 0xFu) << 8) | screen_2_midi_read;
	parts_left = 0xDu;
	part = MIDI_PART_ADDRESS(13);
	for (;;)
	{
		z80_block(30, 3);
		if (MIDI_PART(part)->flags >> 7)
		{
			z80_block(33, 4);
			if (channel_and_index >> 8 == MIDI_PART(part)->midi_channel)
			{
				z80_block(39, 3);
				saved_parts_left = parts_left;
				saved_message_byte = (uint8_t)message_byte;
				saved_channel_and_index = channel_and_index;
				play_midi_message_screen_2((uint8_t)message_byte, part);
				z80_block(37, 4);
				channel_and_index = saved_channel_and_index;
				message_byte = saved_message_byte;
				parts_left = saved_parts_left;
				screen_2_midi_read = (uint8_t)channel_and_index;
			}
		}
		z80_block(33, 4);
		part += 0xAu;
		parts_left--;
		if (parts_left == 0u)
			break;
		z80_cycles(5);
	}
	skip_midi_data_screen_2((uint8_t)message_byte);
}

/* As skip_midi_data_screen_1 on screen_2_midi_read and screen_2_midi_status. */
void skip_midi_data_screen_2(uint8_t first_byte)
{
	uint8_t event = 0;
	uint16_t read_index_address = 0;

	z80_block(28, 4);
	read_index_address = screen_2_midi_read_ADDRESS;
	if ((uint8_t)first_byte >> 7 != 0u)
	{
		z80_block(22, 3);
		event = first_byte & 0xF0u;
		screen_2_midi_read++;
	}
	else
	{
		z80_block(30, 3);
		event = screen_2_midi_status & ~0xFu;
	}
	z80_block(12, 2);
	if (event == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (event == 0xD0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(21, 2);
	screen_2_midi_read++;
}

/* As play_midi_message_screen_1 for screen 2's queue and parts: its voices get flags bit 3 (48), and it
   only touches voices with that bit (screen 1's only those without); its notes play the samples from 100
   on (MultiPCM register 2 bit 0) on the left (pan 90; FM B4 80); its channels' state is
   screen_2_channel_controls and screen_2_pitch_bends; CC 10 sets screen_2_music_volume and CC 20 screen
   2's sample bank (pcm_banks bits 3-5). */
void play_midi_message_screen_2(uint8_t message_byte, uint16_t part)
{
	uint32_t carry = 0;
	uint8_t bend = 0, bend_low = 0, bend_range = 0, bend_sign = 0, bend_steps = 0, channel = 0, channel_index = 0, channels_left = 0, chip_channel = 0, control_value = 0, controller = 0, controller_number = 0, controls = 0, depth = 0, drum_note = 0, effective_volume = 0, entry = 0, event = 0, factor = 0, first_data_byte = 0, flags = 0, ignored8 = 0, level = 0, midi_byte = 0, midi_channel = 0, midi_note = 0, modulation_depth = 0, new_banks = 0, note = 0, note_to_mark = 0, old_controls = 0, part_program = 0, parts_left = 0, pcm_register = 0, pitch_bend_high = 0, pitch_bend_low = 0, rounded_volume = 0, sample = 0, stored_note = 0, transposed_note = 0, value = 0, velocity_then_timeout = 0, velocity_then_timeout_2 = 0, voice_channel = 0, voice_chip_channel = 0, voice_note = 0, voices_left = 0, volume = 0, volume_scale = 0;
	uint16_t bend_entry = 0, bend_index = 0, channel_offset = 0, controls_entry = 0, ignored16 = 0, instrument = 0, kept_channel = 0, kit_note = 0, level_product = 0, midi_byte_copy = 0, music_part = 0, note_index = 0, pitch_bends = 0, scaled_part_volume = 0, step_table = 0, template_then_product = 0, velocity_copy = 0, velocity_index = 0, voice = 0, voice_2 = 0, voice_field = 0, voice_size = 0, voices_left_pair = 0;
	uint8_t saved_channel = 0, saved_parts_left = 0;
	uint16_t pushed_voice_field = 0, saved_voice_field = 0;
	uint8_t negated_bend = 0, negated_step = 0, modulation_wheel = 0, wheel_halved = 0, wheel_quartered = 0, part_volume = 0, music_volume = 0;

	first_data_byte = (uint8_t)message_byte;
	z80_block(37, 4);
	event = screen_2_midi_status & ~0xFu;
	if (event == 0x90u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(109, 12);
		midi_note = first_data_byte;
		transposed_note = MIDI_PART(part)->transpose + first_data_byte;
		note_number = transposed_note;
		note_code = note_codes[transposed_note];
		read_midi_byte_screen_2(&velocity_copy, &midi_byte);
		z80_block(14, 2);
		if (midi_byte == 0u)
		{
			release_note_screen_2(part);
			return;
		}
		z80_block(88, 10);
		velocity_index = midi_byte;
		note_level = velocity_levels[velocity_index];
		part_program = MIDI_PART(part)->program;
		if (part_program < 0xF0u)
		{
			z80_block(27, 2);
			instrument = instruments_ADDRESS;
			instrument = read_table_word(instrument, part_program);
			z80_block(22, 3);
			if (((z80_read8(instrument) >> 1) & 1u) != 0u)
			{
				z80_block(59, 7);
				sample_or_patch = z80_read8((uint16_t)(instrument + 2u));
				voice = VOICE_ADDRESS(24);
				voice_size = 9u;
				voices_left = 6u;
				for (;;)
				{
					z80_block(21, 3);
					if (VOICE(voice)->flags == 0u)
						break;
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
					{
						z80_block(14, 2);
						return;
					}
					z80_cycles(5);
				}
				z80_block(182, 22);
				VOICE(voice)->flags = 0x48u;
				voice++;
				fm_channel = VOICE(voice - 0x1u)->chip_channel & 7u;
				voice++;
				stored_note = note_code;
				VOICE(voice - 0x2u)->note = stored_note;
				voice_note = stored_note;
				voice++;
				channel = screen_2_midi_status & 0xFu;
				VOICE(voice - 0x3u)->midi_channel = channel;
				voice_channel = channel;
				level = note_level;
				VOICE(voice - 0x3u)->level = level;
				level_product = (uint32_t)level << 8;
				effective_volume = MIDI_PART(part)->effective_volume;
				level_product = multiply_bytes(effective_volume, level_product);
				z80_block(122, 16);
				note_level = 0x7Fu - ((uint16_t)((uint32_t)((level_product >> 8) >> 1) << 8) >> 8);
				channel_offset = voice_channel;
				voice_modulation = (screen_2_channel_controls[channel_offset] & 0xFu) | 0x80u;
				pitch_bends = screen_2_pitch_bends_ADDRESS;
				note = voice_note;
				start_fm_note(note, channel_offset, pitch_bends);
				return;
			}
			for (;;)
			{
				z80_block(44, 6);
				instrument++;
				if (note_number <= z80_read8(instrument))
					break;
				z80_block(28, 4);
				instrument += 3u;
			}
			z80_block(78, 9);
			instrument++;
			sample_or_patch = z80_read8(instrument);
			instrument++;
			split_tune = z80_read8(instrument);
			split_octave = z80_read8((uint16_t)(instrument + 1u));
			take_voice_screen_2((uint8_t)velocity_copy, part);
			return;
		}
		z80_block(34, 3);
		part_program += 0x10u;
		kit_note = drum_kits_ADDRESS;
		kit_note = read_table_word(kit_note, part_program);
		z80_block(112, 14);
		note_index = midi_note;
		kit_note = kit_note + note_index + note_index + note_index;
		drum_pitch = z80_read8(kit_note);
		kit_note++;
		velocity_then_timeout = z80_read8(kit_note);
		sample = z80_read8((uint16_t)(kit_note + 1u));
		sample_or_patch = sample;
		if (sample == 0xFFu)
		{
			z80_cycles(6);
			return;
		}
		z80_block(37, 4);
		if ((screen_2_midi_status & 0xFu) != 0xFu)
		{
			velocity_then_timeout_2 = velocity_then_timeout;
			take_voice_screen_2(velocity_then_timeout_2, part);
			return;
		}
		z80_block(21, 3);
		drum_note = midi_note;
		if (drum_note != 0x2Cu)
		{
			z80_block(17, 2);
			if (drum_note != 0x2Au)
			{
				velocity_then_timeout_2 = velocity_then_timeout;
				take_voice_screen_2(velocity_then_timeout_2, part);
				return;
			}
		}
		velocity_then_timeout_2 = velocity_then_timeout;
		cut_open_hi_hat_screen_2(velocity_then_timeout_2, part);
		return;
	}
	z80_block(17, 2);
	if (event == 0x80u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(96, 11);
		note_code = note_codes[(uint8_t)(MIDI_PART(part)->transpose + first_data_byte)];
		screen_2_midi_read++;
		release_note_screen_2(part);
		return;
	}
	z80_block(17, 2);
	if (event == 0xE0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		bend_low = first_data_byte;
		read_midi_byte_screen_2(&ignored16, &midi_byte);
		z80_block(26, 5);
		carry = midi_byte & 1u;
		bend_sign = midi_byte >> 1;
		bend = bend_low;
		if (carry)
		{
			z80_block(7, 1);
			bend |= 0x80u;
		}
		z80_block(78, 18);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		bend = (bend >> 1) | (carry << 7);
		if ((bend_sign & 1u) == 0u)
		{
			z80_block(18, 3);
			negated_bend = ~bend + 1u;
			bend = negated_bend;
			if (negated_bend == 0u)
			{
				z80_block(4, 1);
				bend--;
			}
		}
		z80_block(57, 6);
		bend_index = bend;
		bend_range = MIDI_PART(part)->bend_range;
		step_table = bend_range_tables_ADDRESS;
		step_table = read_table_word(step_table, bend_range);
		z80_block(36, 5);
		bend_steps = z80_read8((uint16_t)(step_table + bend_index));
		carry = bend_sign & 1u;
		bend_sign = bend_sign >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			bend_sign--;
			negated_step = ~bend_steps + 1u;
			bend_steps = negated_step;
			if (negated_step == 0u)
			{
				z80_block(4, 1);
				bend_steps--;
			}
		}
		z80_block(145, 19);
		pitch_bend_high = bend_sign;
		pitch_bend_low = bend_steps;
		pitch_bend = ((uint32_t)pitch_bend_high << 8) | pitch_bend_low;
		bend_entry = screen_2_pitch_bends_ADDRESS + (uint8_t)((screen_2_midi_status & 0xFu) * 2u);
		z80_write8(bend_entry, pitch_bend_low);
		z80_write8((uint16_t)(bend_entry + 1u), pitch_bend_high);
		voice_2 = VOICE_ADDRESS(0);
		voices_left = 0x1Eu;
		channel = screen_2_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (channel == VOICE(voice_2)->midi_channel)
			{
				z80_block(48, 6);
				saved_channel = channel;
				flags = VOICE(voice_2)->flags;
				if ((flags >> 3) & 1u)
				{
					z80_block(17, 2);
					if ((flags & 0x7Fu) != 0u)
					{
						z80_cycles(7);
						retune_voice_screen_2(voice_2);
					}
				}
				z80_block(10, 1);
				channel = saved_channel;
			}
			z80_block(33, 4);
			voice_2 += 9u;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (event == 0xB0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		controller_number = first_data_byte;
		read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
		control_value = (uint8_t)midi_byte_copy;
		z80_block(21, 3);
		controller = controller_number;
		carry = controller == 0u;
		if (controller == 1u)
		{
			z80_block(123, 19);
			modulation_wheel = control_value;
			depth = (modulation_wheel >> 1) | (carry << 7);
			carry = modulation_wheel & 1u;
			wheel_halved = depth;
			depth = (wheel_halved >> 1) | (carry << 7);
			carry = wheel_halved & 1u;
			wheel_quartered = depth;
			depth = (wheel_quartered >> 1) | (carry << 7);
			carry = wheel_quartered & 1u;
			depth = ((depth >> 1) | (carry << 7)) & 7u;
			modulation_depth = depth;
			voice_modulation = depth;
			controls_entry = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
			controls = z80_read8(controls_entry);
			old_controls = controls;
			if ((controls & 0x7Fu) == modulation_depth)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(controls_entry, (old_controls & 0x80u) | modulation_depth);
			voice = voice_0_midi_channel_ADDRESS;
			voices_left = 0x1Eu;
			channel = screen_2_midi_status & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (channel == VOICE(voice - 0x3u)->midi_channel)
				{
					z80_block(47, 8);
					kept_channel = (uint32_t)channel << 8;
					voice = voice - 1u - 1u - 1u;
					flags = VOICE(voice)->flags;
					if ((flags >> 3) & 1u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							voices_left_pair = (uint32_t)voices_left << 8;
							set_voice_modulation_screen_2(part, &voices_left_pair, &voice);
							voices_left = (uint8_t)(voices_left_pair >> 8);
						}
					}
					z80_block(22, 4);
					voice += 3u;
					channel = (uint8_t)(kept_channel >> 8);
				}
				z80_block(29, 3);
				voice += 9u;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 7u)
		{
			z80_block(18, 3);
			part_volume = control_value * 2u;
			volume = part_volume;
			if (part_volume != 0u)
			{
				z80_block(4, 1);
				volume++;
			}
			z80_block(60, 7);
			MIDI_PART(part)->volume = volume;
			scaled_part_volume = (uint32_t)volume << 8;
			volume_scale = 0xFFu;
			if ((MIDI_PART(part)->flags & 1u) == 0u)
			{
				z80_block(13, 1);
				volume_scale = screen_2_music_volume;
			}
			z80_block(21, 2);
			factor = volume_scale;
			scaled_part_volume = multiply_bytes(factor, scaled_part_volume);
			z80_block(18, 3);
			rounded_volume = (uint8_t)(scaled_part_volume >> 8);
			if (rounded_volume != 0u)
			{
				z80_block(4, 1);
				rounded_volume++;
			}
			z80_block(36, 4);
			MIDI_PART(part)->effective_volume = rounded_volume;
			voice_field = VOICE_ADDRESS(0);
			voices_left = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				pushed_voice_field = voice_field;
				if ((VOICE(voice_field)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((VOICE(voice_field)->flags >> 3) & 1u)
					{
						z80_block(55, 7);
						voice_field += 3u;
						if ((screen_2_midi_status & 0xFu) == VOICE(voice_field - 0x3u)->midi_channel)
						{
							z80_block(37, 6);
							voice_field = voice_field - 1u - 1u;
							voice_chip_channel = VOICE(voice_field - 0x1u)->chip_channel;
							if (voice_chip_channel >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(pcm_data) & 1u);
								z80_block(81, 8);
								z80_write8(pcm_voice_select, voice_chip_channel);
								voice_field = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
								effective_volume = MIDI_PART(part)->effective_volume;
								voice_field = multiply_bytes(effective_volume, voice_field);
								z80_block(39, 5);
								value = ~(voice_field >> 8) & 0xFEu;
								pcm_register = 5u;
								write_pcm_register(pcm_register, value);
							}
							else
							{
								z80_block(17, 1);
								chip_channel = voice_chip_channel;
								apply_fm_channel_volume(chip_channel, voice_field, part);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				voice_field = pushed_voice_field + 9u;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x10u)
		{
			z80_block(18, 3);
			music_volume = control_value * 2u;
			volume = music_volume;
			if (music_volume != 0u)
			{
				z80_block(4, 1);
				volume++;
			}
			z80_block(34, 4);
			screen_2_music_volume = volume;
			music_part = MIDI_PART_ADDRESS(13);
			parts_left = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_parts_left = parts_left;
				scaled_part_volume = (uint32_t)MIDI_PART(music_part)->volume << 8;
				volume_scale = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					volume_scale = screen_2_music_volume;
				}
				z80_block(21, 2);
				factor = volume_scale;
				scaled_part_volume = multiply_bytes(factor, scaled_part_volume);
				z80_block(18, 3);
				rounded_volume = (uint8_t)(scaled_part_volume >> 8);
				if (rounded_volume != 0u)
				{
					z80_block(4, 1);
					rounded_volume++;
				}
				z80_block(36, 4);
				MIDI_PART(music_part)->effective_volume = rounded_volume;
				voice_field = voice_0_midi_channel_ADDRESS;
				voices_left = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_voice_field = voice_field;
					if (MIDI_PART(music_part)->midi_channel == VOICE(voice_field - 0x3u)->midi_channel)
					{
						z80_block(40, 6);
						voice_field = voice_field - 1u - 1u - 1u;
						if ((VOICE(voice_field)->flags >> 6) & 1u)
						{
							z80_block(22, 3);
							if ((VOICE(voice_field)->flags >> 3) & 1u)
							{
								z80_block(31, 5);
								voice_field++;
								voice_chip_channel = VOICE(voice_field - 0x1u)->chip_channel;
								if (voice_chip_channel >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(pcm_data) & 1u);
									z80_block(81, 8);
									z80_write8(pcm_voice_select, voice_chip_channel);
									voice_field = (uint32_t)VOICE(voice_field - 0x1u)->level << 8;
									effective_volume = MIDI_PART(music_part)->effective_volume;
									voice_field = multiply_bytes(effective_volume, voice_field);
									z80_block(39, 5);
									value = ~(voice_field >> 8) & 0xFEu;
									pcm_register = 5u;
									write_pcm_register(pcm_register, value);
								}
								else
								{
									z80_block(17, 1);
									chip_channel = voice_chip_channel;
									apply_fm_music_volume(chip_channel, voice_field, music_part);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					voice_field = saved_voice_field + 9u;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				music_part += 0xAu;
				parts_left = saved_parts_left - 1u;
				if (parts_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x40u)
		{
			z80_block(21, 3);
			if (control_value >= 0x40u)
			{
				z80_block(77, 9);
				controls_entry = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
				z80_write8(controls_entry, z80_read8(controls_entry) | 0x80u);
				return;
			}
			z80_block(51, 6);
			voice_2 = VOICE_ADDRESS(0);
			voice_size = 9u;
			voices_left = 0x1Eu;
			channel = screen_2_midi_status & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (channel == VOICE(voice_2)->midi_channel)
				{
					z80_block(41, 6);
					channel_index = channel;
					flags = VOICE(voice_2)->flags;
					if ((flags >> 1) & 1u)
					{
						z80_block(18, 3);
						if ((flags >> 3) & 1u)
						{
							z80_block(37, 5);
							if (VOICE(voice_2)->chip_channel >> 7 == 0u)
							{
								z80_block(23, 2);
								VOICE(voice_2)->flags |= 1u;
							}
							else
							{
								z80_block(17, 1);
								stop_fm_voice(voice_2);
								z80_block(10, 1);
							}
						}
					}
					z80_block(4, 1);
					channel = (uint8_t)channel_index;
				}
				z80_block(23, 3);
				voice_2 += voice_size;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			controls_entry = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
			z80_write8(controls_entry, z80_read8(controls_entry) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x7Bu)
		{
			z80_block(47, 5);
			channel = screen_2_midi_status & 0xFu;
			voice = VOICE_ADDRESS(0);
			voice_size = 9u;
			voices_left = 0x18u;
			for (;;)
			{
				z80_block(22, 3);
				if ((VOICE(voice)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((VOICE(voice)->flags >> 3) & 1u)
					{
						z80_block(35, 5);
						voice += 3u;
						if (channel != VOICE(voice - 0x3u)->midi_channel)
						{
							z80_block(18, 3);
							voice = voice - 1u - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							voice = voice - 1u - 1u - 1u;
							VOICE(voice)->flags |= 1u;
						}
					}
				}
				z80_block(19, 2);
				voice += voice_size;
				voices_left--;
				if (voices_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x50u)
		{
			z80_block(21, 3);
			if (control_value == 0x7Eu)
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				voice_size = 9u;
				voices_left = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if ((VOICE(voice)->flags >> 3) & 1u)
					{
						z80_block(22, 3);
						if ((VOICE(voice)->flags >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((VOICE(voice)->flags >> 4) & 1u) == 0u)
							{
								z80_block(15, 2);
								VOICE(voice)->flags |= 1u;
							}
							else
							{
								z80_block(60, 9);
								voice += 3u;
								midi_channel = VOICE(voice - 0x3u)->midi_channel;
								voice = voice - 1u - 1u - 1u;
								if (midi_channel == 0xFu)
								{
									z80_block(15, 2);
									VOICE(voice)->flags |= 1u;
								}
							}
						}
					}
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				voice_size = 9u;
				voices_left = 0x18u;
				for (;;)
				{
					z80_block(22, 3);
					if ((VOICE(voice)->flags >> 3) & 1u)
					{
						z80_block(22, 3);
						if ((VOICE(voice)->flags >> 6) & 1u)
						{
							z80_block(15, 2);
							VOICE(voice)->flags |= 1u;
						}
					}
					z80_block(19, 2);
					voice += voice_size;
					voices_left--;
					if (voices_left == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			controls_entry = screen_2_channel_controls_ADDRESS;
			channels_left = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(controls_entry, 0u);
				controls_entry++;
				channels_left--;
				if (channels_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			bend_entry = screen_2_pitch_bends_ADDRESS;
			channels_left = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(bend_entry, 0u);
				bend_entry++;
				channels_left--;
				if (channels_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller == 0x20u)
		{
			z80_block(31, 3);
			entry = control_value;
			template_then_product = part_templates_ADDRESS;
			template_then_product = read_table_word(template_then_product, entry);
			z80_block(136, 19);
			move_block(template_then_product, MIDI_PART_ADDRESS(13), 0x50u, 1u);
			new_banks = (pcm_banks & 7u) | ((((uint32_t)z80_read8((uint16_t)(template_then_product + 0x50u)) << 1) << 1) << 1);
			port_out(PCM_BANKS, new_banks);
			pcm_banks = new_banks;
			music_part = MIDI_PART_ADDRESS(13);
			parts_left = 8u;
			for (;;)
			{
				z80_block(56, 6);
				template_then_product = (uint32_t)MIDI_PART(music_part)->volume << 8;
				volume_scale = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					volume_scale = screen_2_music_volume;
				}
				z80_block(21, 2);
				factor = volume_scale;
				template_then_product = multiply_bytes(factor, template_then_product);
				z80_block(18, 3);
				rounded_volume = (uint8_t)(template_then_product >> 8);
				if (rounded_volume != 0u)
				{
					z80_block(4, 1);
					rounded_volume++;
				}
				z80_block(52, 6);
				MIDI_PART(music_part)->effective_volume = rounded_volume;
				music_part += 0xAu;
				parts_left--;
				if (parts_left == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (controller != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		note_to_mark = note_codes[control_value];
		voice = VOICE_ADDRESS(0);
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(22, 3);
			if ((VOICE(voice)->flags >> 6) & 1u)
			{
				z80_block(22, 3);
				if ((VOICE(voice)->flags >> 3) & 1u)
				{
					z80_block(33, 5);
					voice += 2u;
					if (note_to_mark != VOICE(voice - 0x2u)->note)
					{
						z80_block(12, 2);
						voice = voice - 1u - 1u;
					}
					else
					{
						z80_block(43, 5);
						voice++;
						if ((screen_2_midi_status & 0xFu) != VOICE(voice - 0x3u)->midi_channel)
						{
							z80_block(6, 1);
							voice--;
							z80_block(12, 2);
							voice = voice - 1u - 1u;
						}
						else
						{
							z80_block(43, 6);
							voice = voice - 1u - 1u - 1u;
							VOICE(voice)->flags |= 1u;
						}
					}
				}
			}
			z80_block(29, 3);
			voice += 9u;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (event == 0xC0u)
	{
		z80_block(18, 3);
		if (first_data_byte >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			first_data_byte = (uint8_t)midi_byte_copy;
		}
		z80_block(29, 3);
		MIDI_PART(part)->program = first_data_byte;
		return;
	}
	z80_block(17, 2);
	if (event != 0xD0u)
	{
		z80_block(17, 2);
		if (event != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	skip_midi_data_screen_2(message_byte);
}

/* As cut_open_hi_hat_screen_1 for screen 2's voices (flags bit 3); then take_voice_screen_2. */
void cut_open_hi_hat_screen_2(uint8_t velocity_then_timeout, uint16_t part)
{
	uint8_t midi_channel = 0, voices_left = 0;
	uint16_t voice = 0, voice_size = 0;

	z80_block(27, 3);
	voice = voice_0_note_ADDRESS;
	voice_size = 9u;
	voices_left = 0x18u;
	for (;;)
	{
		z80_block(24, 3);
		if (VOICE(voice - 0x2u)->note == 0xCAu)
		{
			z80_block(36, 5);
			voice++;
			midi_channel = VOICE(voice - 0x3u)->midi_channel;
			voice--;
			if (midi_channel == 0xFu)
			{
				z80_block(34, 5);
				voice = voice - 1u - 1u;
				if ((VOICE(voice)->flags >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((VOICE(voice)->flags >> 3) & 1u)
					{
						z80_block(15, 2);
						VOICE(voice)->flags |= 1u;
					}
				}
				z80_block(12, 2);
				voice += 2u;
			}
		}
		z80_block(19, 2);
		voice += voice_size;
		voices_left--;
		if (voices_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	take_voice_screen_2(velocity_then_timeout, part);
}

/* As take_voice_screen_1 for a note of screen 2 (play_midi_message_screen_2 and cut_open_hi_hat_screen_2
   go on to it): its voice gets flags bit 3 (48), plays the samples from 100 on and pans left (90). */
void take_voice_screen_2(uint8_t velocity_then_timeout, uint16_t part)
{
	uint8_t channel = 0, drum_timeout = 0, effective_volume = 0, oldest_order = 0, order = 0, pcm_register = 0, stored_note = 0, value = 0, voice_channel = 0, voice_note = 0, voices_left = 0;
	uint16_t bend_entry = 0, channel_offset = 0, level_product = 0, new_voice_field = 0, pitch = 0, pitch_entry = 0, pitch_table_entry = 0, tune = 0, voice = 0, voice_size = 0;
	uint16_t pushed_voice = 0;

	drum_timeout = (uint8_t)velocity_then_timeout;
	z80_block(33, 3);
	voice = next_voice;
	voice_size = 9u;
	voices_left = 0x18u;
	for (;;)
	{
		z80_block(21, 3);
		if ((uint8_t)voice == 0xE8u)
		{
			z80_cycles(7);
			voice = reset_next_voice();
		}
		z80_block(21, 3);
		oldest_order = 0u;
		if (0u == VOICE(voice)->flags)
		{
			z80_block(58, 5);
			pushed_voice = voice;
			next_voice = voice + voice_size;
			new_voice_field = pushed_voice;
			break;
		}
		z80_block(19, 2);
		voice += voice_size;
		voices_left--;
		if (voices_left != 0u)
		{
			z80_cycles(5);
			continue;
		}
		z80_block(10, 1);
		z80_block(17, 2);
		voice = voice_0_allocation_order_ADDRESS;
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(17, 2);
			if (oldest_order == VOICE(voice - 0x6u)->allocation_order)
				break;
			z80_block(19, 2);
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
			{
				z80_block(14, 2);
				return;
			}
			z80_cycles(5);
		}
		z80_block(42, 5);
		VOICE(voice - 0x6u)->allocation_order = 0x18u;
		pushed_voice = voice;
		voice = voice_0_allocation_order_ADDRESS;
		voices_left = 0x18u;
		for (;;)
		{
			z80_block(30, 3);
			VOICE(voice - 0x6u)->allocation_order--;
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(52, 5);
		pcm_voices_used--;
		new_voice_field = pushed_voice - 6u;
		break;
	}
	z80_block(46, 5);
	VOICE(new_voice_field)->flags = 0x48u;
	if (MIDI_PART(part)->program < 0xF0u)
	{
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(479, 64);
		new_voice_field++;
		z80_write8(pcm_voice_select, VOICE(new_voice_field - 0x1u)->chip_channel);
		new_voice_field++;
		stored_note = note_code;
		VOICE(new_voice_field - 0x2u)->note = stored_note;
		voice_note = stored_note;
		new_voice_field++;
		channel = screen_2_midi_status & 0xFu;
		VOICE(new_voice_field - 0x3u)->midi_channel = channel;
		voice_channel = channel;
		new_voice_field += 2u;
		VOICE(new_voice_field - 0x5u)->level = note_level;
		new_voice_field++;
		order = pcm_voices_used;
		VOICE(new_voice_field - 0x6u)->allocation_order = order;
		pcm_voices_used = order + 1u;
		new_voice_field++;
		VOICE(new_voice_field - 0x7u)->tune = split_tune;
		VOICE(new_voice_field - 0x7u)->octave_shift = split_octave;
		channel_offset = (uint8_t)(voice_channel * 2u);
		bend_entry = screen_2_pitch_bends_ADDRESS + channel_offset;
		tune = split_tune;
		pushed_voice = z80_read16(bend_entry) + tune;
		pitch_table_entry = pcm_pitch_tables_ADDRESS + (uint8_t)((voice_note & 0xFu) * 2u);
		pitch_entry = PCM_PITCH_RUNS(pitch_table_entry)[0] + pushed_voice + pushed_voice;
		pitch = PCM_PITCH_STEPS(pitch_entry)[0];
		pitch = (pitch & 0xFFu) | ((uint32_t)(uint8_t)((voice_note & ~0xFu) + (pitch >> 8)) << 8);
		value = split_octave + (pitch >> 8);
	}
	else
	{
		z80_block(15, 2);
		VOICE(new_voice_field)->flags |= 0x10u;
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(171, 21);
		new_voice_field++;
		z80_write8(pcm_voice_select, VOICE(new_voice_field - 0x1u)->chip_channel);
		new_voice_field++;
		VOICE(new_voice_field - 0x2u)->note = note_code;
		new_voice_field++;
		VOICE(new_voice_field - 0x3u)->midi_channel = screen_2_midi_status & 0xFu;
		new_voice_field++;
		VOICE(new_voice_field - 0x4u)->drum_timeout = drum_timeout;
		new_voice_field++;
		VOICE(new_voice_field - 0x5u)->level = note_level;
		new_voice_field++;
		if ((screen_2_midi_status & 0xFu) == 0xFu)
		{
			z80_block(37, 4);
			order = pcm_voices_used;
			VOICE(new_voice_field - 0x6u)->allocation_order = order;
			pcm_voices_used = order + 1u;
		}
		else
		{
			z80_block(24, 3);
			VOICE(new_voice_field - 0x6u)->allocation_order = 0xFFu;
		}
		z80_block(30, 3);
		value = drum_pitch;
		pitch = 0u;
	}
	z80_block(24, 2);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(36, 5);
	value = pitch | 1u;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(37, 3);
	value = sample_or_patch;
	pcm_register = 1u;
	write_pcm_register(pcm_register, value);
	z80_block(53, 5);
	level_product = (uint32_t)note_level << 8;
	effective_volume = MIDI_PART(part)->effective_volume;
	level_product = multiply_bytes(effective_volume, level_product);
	z80_block(39, 5);
	value = ~(level_product >> 8) | 1u;
	pcm_register = 5u;
	write_pcm_register(pcm_register, value);
	z80_block(31, 3);
	value = 0x90u;
	pcm_register = 0u;
	write_pcm_register(pcm_register, value);
	z80_block(24, 3);
	value = 0x80u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
}

/* As release_note_screen_1 for screen 2's voices (flags bit 3) (play_midi_message_screen_2 goes on to
   it). */
void release_note_screen_2(uint16_t part)
{
	uint8_t channel = 0, channel_index = 0, flags = 0, fm_register = 0, program = 0, value = 0, voices_left = 0;
	uint16_t instrument = 0, voice = 0, voice_size = 0;

	z80_block(31, 4);
	program = MIDI_PART(part)->program;
	if (program >= 0xF0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 2);
	instrument = instruments_ADDRESS;
	instrument = read_table_word(instrument, program);
	z80_block(22, 3);
	if (((z80_read8(instrument) >> 1) & 1u) != 0u)
	{
		z80_block(51, 6);
		voice = VOICE_ADDRESS(24);
		voice_size = 9u;
		voices_left = 6u;
		channel = screen_2_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (channel == VOICE(voice)->midi_channel)
			{
				z80_block(46, 5);
				channel_index = channel;
				if (note_code == VOICE(voice)->note)
				{
					z80_block(37, 5);
					flags = VOICE(voice)->flags;
					if (((flags >> 1) & 1u) == 0u)
					{
						z80_block(18, 3);
						if ((flags >> 3) & 1u)
						{
							z80_block(17, 2);
							if ((flags & 0x7Fu) != 0u)
								break;
						}
					}
				}
				z80_block(4, 1);
				channel = (uint8_t)channel_index;
			}
			z80_block(23, 3);
			voice += voice_size;
			voices_left--;
			if (voices_left == 0u)
			{
				z80_block(10, 1);
				return;
			}
			z80_cycles(5);
		}
		z80_block(65, 9);
		if (screen_2_channel_controls[channel_index] >> 7)
		{
			z80_block(33, 3);
			VOICE(voice)->flags |= 2u;
			return;
		}
		z80_block(85, 10);
		VOICE(voice)->flags = 0u;
		VOICE(voice)->midi_channel = 0u;
		value = VOICE(voice)->chip_channel & 7u;
		fm_register = 0x28u;
		write_fm(fm_register, value);
		return;
	}
	z80_block(51, 6);
	voice = VOICE_ADDRESS(0);
	voice_size = 9u;
	voices_left = 0x18u;
	channel = screen_2_midi_status & 0xFu;
	for (;;)
	{
		z80_block(29, 3);
		if (channel == VOICE(voice)->midi_channel)
		{
			z80_block(46, 5);
			channel_index = channel;
			if (note_code == VOICE(voice)->note)
			{
				z80_block(37, 5);
				flags = VOICE(voice)->flags;
				if (((flags >> 1) & 1u) == 0u)
				{
					z80_block(18, 3);
					if ((flags >> 3) & 1u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
							break;
					}
				}
			}
			z80_block(4, 1);
			channel = (uint8_t)channel_index;
		}
		z80_block(23, 3);
		voice += voice_size;
		voices_left--;
		if (voices_left == 0u)
		{
			z80_block(10, 1);
			return;
		}
		z80_cycles(5);
	}
	z80_block(65, 9);
	if (screen_2_channel_controls[channel_index] >> 7 == 0u)
	{
		z80_block(33, 3);
		VOICE(voice)->flags |= 1u;
		return;
	}
	z80_block(33, 3);
	VOICE(voice)->flags |= 2u;
}

/* The same code as retune_voice_screen_1, for screen 2's pitch bend. */
void retune_voice_screen_2(uint16_t voice)
{
	uint8_t chip_channel = 0, chip_code = 0, note = 0, pcm_register = 0, value = 0;
	uint16_t bend = 0, pitch = 0, pitch_entry = 0, pitch_table_entry = 0, tune = 0;

	z80_block(37, 5);
	chip_channel = VOICE(voice)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		retune_fm_voice(voice, chip_channel);
		return;
	}
	z80_block(4, 1);
	chip_code = chip_channel;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(287, 37);
	z80_write8(pcm_voice_select, chip_code);
	note = VOICE(voice)->note;
	pitch_table_entry = pcm_pitch_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	bend = pitch_bend;
	pitch_entry = PCM_PITCH_RUNS(pitch_table_entry)[0] + bend + bend;
	tune = VOICE(voice)->tune;
	pitch_entry = pitch_entry + tune + tune;
	pitch = PCM_PITCH_STEPS(pitch_entry)[0];
	pitch = (pitch & 0xFFu) | ((uint32_t)(uint8_t)((note & ~0xFu) + (pitch >> 8)) << 8);
	value = VOICE(voice)->octave_shift + (pitch >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(21, 3);
	value = (uint8_t)pitch;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
}

/* As set_voice_modulation_screen_1 for screen 2 (FM: 80 | the depth, with the same mistake over the
   channel and the caller's count). */
void set_voice_modulation_screen_2(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice)
{
	uint16_t voices_left = (uint16_t)*p_voices_left;
	uint16_t voice = (uint16_t)*p_voice;
	uint8_t chip_channel = 0, count_as_channel = 0, lfo_register = 0, pan_register = 0, value = 0;

	z80_block(31, 5);
	voice++;
	chip_channel = VOICE(voice - 0x1u)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		z80_block(79, 9);
		count_as_channel = (uint8_t)(voices_left >> 8);
		fm_channel = count_as_channel;
		pan_register = (count_as_channel & 3u) - 0x4Cu;
		value = (voice_modulation & 0x3Fu) | 0x80u;
		voices_left = pan_register;
		voices_left = write_fm_channel(voices_left, value);
		z80_block(16, 2);
		voice--;
		voices_left = (voices_left & ~0xFFu) | pan_register;
	}
	else
	{
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(73, 7);
		z80_write8(pcm_voice_select, chip_channel);
		lfo_register = 6u;
		value = voice_modulation | MIDI_PART(part)->lfo_frequency;
		voices_left = (voices_left & ~0xFFu) | 6u;
		write_pcm_register((uint8_t)voices_left, value);
		z80_block(16, 2);
		voice--;
		voices_left = (voices_left & ~0xFFu) | lfo_register;
	}
	*p_voices_left = voices_left;
	*p_voice = voice;
}

/* Moves screen_2_midi_read on by one and hands back the byte of screen_2_midi_queue there. */
void read_midi_byte_screen_2(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte)
{
	uint8_t midi_byte = 0;
	uint16_t midi_byte_copy = 0;

	z80_block(56, 7);
	screen_2_midi_read++;
	midi_byte = z80_read8(screen_2_midi_read | 0xE200u);
	midi_byte_copy = midi_byte;
	*p_midi_byte_copy = (*p_midi_byte_copy & 0xFF00u) | midi_byte_copy;
	*p_midi_byte = midi_byte;
}
