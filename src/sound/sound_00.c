/**
 *    Name:        sound_00.c
 *    Description: the sound program's functions, part 00 (in the order of their Z80 addresses)
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* The sound CPU's start: interrupts off, the shared RAM E000-FDFF cleared (from shared_ram_start), the
   interrupt inputs mapped (IRQ_VECTOR0_INPUT-IRQ_VECTOR2_INPUT: 0 the YM3438's timers, 1 the V70's
   request, 2), the pending ones cleared, none masked, sound_init, then interrupts on. */
void sound_reset(void)
{
	z80_block(14, 2);
	interrupts_off();
	z80_block(201, 23);
	shared_ram_start = 0u;
	move_block(shared_ram_start_ADDRESS, 0xE001u, 0x1DFFu, 1u);
	port_out(IRQ_VECTOR0_INPUT, 0u);
	port_out(IRQ_VECTOR1_INPUT, 1u);
	port_out(IRQ_VECTOR2_INPUT, 2u);
	port_out(IRQ_CONTROL, 0u);
	port_out(IRQ_ACKNOWLEDGE, 0u);
	port_out(IRQ_CONTROL_D8, 0xBu);
	port_out(IRQ_MASK, 0u);
	sound_init();
	z80_block(4, 1);
	interrupts_on_after_block();
}

/* The timer interrupt (the YM3438's timer A): acknowledges the pending inputs, rearms the timer
   (register 27 = 15), then sequence_tick, drum_timeout_tick and engine_sweep_tick. */
void sound_irq(void)
{
	z80_block(121, 16);
	interrupts_off();
	port_out(IRQ_ACKNOWLEDGE, 0u);
	port_out(FM_ADDRESS, 0x27u);
	port_out(FM_DATA, 0x15u);
	sequence_tick();
	z80_block(17, 1);
	drum_timeout_tick();
	z80_block(17, 1);
	engine_sweep_tick();
	z80_block(54, 9);
	interrupts_on();
}

/* One pass of the main loop: waits, interrupts off, while sound_cpu_hold is set; then
   stop_flagged_voices, one message of each screen's MIDI queue (handle_midi_message_screen_1 and
   _screen_2), both engines, both tyre noises and poll_sound_commands. */
void sound_main_loop(void)
{
	z80_block(27, 3);
	if (sound_cpu_hold != 0u)
	{
		do
		{
			z80_block(31, 4);
			interrupts_off();
		} while (sound_cpu_hold != 0u);
		z80_block(4, 1);
		interrupts_on_after_block();
	}
	z80_block(17, 1);
	stop_flagged_voices();
	z80_block(17, 1);
	handle_midi_message_screen_1();
	z80_block(17, 1);
	handle_midi_message_screen_2();
	z80_block(17, 1);
	update_engine_screen_1();
	z80_block(17, 1);
	update_engine_screen_2();
	z80_block(17, 1);
	update_tyre_noise_screen_1();
	z80_block(17, 1);
	update_tyre_noise_screen_2();
	z80_block(17, 1);
	poll_sound_commands();
	z80_block(10, 1);
}

/* The drum notes' time-out, on every timer tick: drum_timeout_wait counts 140 ticks, then a pass
   (drum_timeout_pass bit 0) visits the 24 MultiPCM voices, one a tick (drum_timeout_cursor,
   drum_timeout_voices_left): each one's drum_timeout goes down by one, and a sounding drum note (flags
   bits 6 and 4) whose count runs out, and that is not being stopped already, gets the stop flag (flags
   bit 0) for stop_flagged_voices. Instrument notes are counted down too, but never stopped by it. */
void drum_timeout_tick(void)
{
	uint8_t a = 0, c = 0;
	uint16_t hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31, 4);
	if ((drum_timeout_pass & 1u) == 0u)
	{
		z80_block(26, 3);
		t1 = drum_timeout_wait;
		drum_timeout_wait = t1 - 1u;
		if (t1 != 1u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(101, 10);
		drum_timeout_wait = 0x8Cu;
		drum_timeout_cursor = voice_0_drum_timeout_ADDRESS;
		drum_timeout_voices_left = 0x19u;
		drum_timeout_pass |= 1u;
		return;
	}
	z80_block(31, 3);
	t2 = drum_timeout_voices_left;
	drum_timeout_voices_left = t2 - 1u;
	if (t2 == 1u)
	{
		z80_block(35, 4);
		drum_timeout_pass &= ~1u;
		return;
	}
	z80_block(37, 3);
	hl = drum_timeout_cursor;
	t3 = z80_read8(hl);
	z80_write8(hl, t3 - 1u);
	if (t3 != 1u)
	{
		z80_block(47, 4);
		drum_timeout_cursor = hl + 9u;
		return;
	}
	z80_block(49, 6);
	hl -= 4u;
	a = z80_read8(hl);
	c = a;
	if ((a & 0x81u) == 0u)
	{
		z80_block(28, 4);
		if ((c & 0x70u) >= 0x50u)
		{
			z80_block(15, 2);
			z80_write8(hl, z80_read8(hl) | 1u);
		}
	}
	z80_block(47, 4);
	drum_timeout_cursor = hl + 0xDu;
}

/* Sets the sound up at reset: the MultiPCM (init_pcm_chip) and the YM3438 (init_fm_chip); both screens'
   MIDI queues empty (screen_1_midi_write 0, screen_1_midi_read and screen_2_midi_read FF), next_voice
   the first voice, screen_1_tick_countdown 1, drum_timeout_wait 140 ticks, both music volumes FF; the
   voices' chip channels (init_voice_channels) and both screens' parts (init_parts); last, YM3438
   register 27 = 15: timer A loaded and its interrupt on. */
void sound_init(void)
{
	uint8_t c = 0, fm_register = 0, unused_c = 0, value = 0;
	uint16_t last_register = 0;

	z80_block(17, 1);
	init_pcm_chip();
	z80_block(17, 1);
	last_register = init_fm_chip();
	c = (uint8_t)last_register;
	z80_block(172, 15);
	screen_1_midi_write = 0u;
	screen_1_midi_read = 0xFFu;
	screen_2_midi_read = 0xFFu;
	next_voice = VOICE_ADDRESS(0);
	screen_1_tick_countdown = 1u;
	AS16(drum_timeout_wait) = 0x8Cu;
	screen_1_music_volume = 0xFFu;
	screen_2_music_volume = 0xFFu;
	unused_c = c;
	init_voice_channels(unused_c);
	z80_block(17, 1);
	init_parts();
	z80_block(31, 3);
	value = 0x15u;
	fm_register = 0x27u;
	write_fm(fm_register, value);
	z80_block(10, 1);
}

/* Sets the YM3438 up: fm_global_init's 11 pairs on part I (the LFO on, timer A 3F0 and timer B FF, all
   six channels keyed off, the DAC off), then fm_operator_init's 36 pairs on both parts (every operator
   silent: total level 7F, release FF, no SSG-EG). Hands back the last register written in last_register. */
uint16_t init_fm_chip(void)
{
	uint8_t b = 0, fm_register = 0, register_number = 0, value = 0;
	uint16_t init_entry = 0, last_register = 0;

	z80_block(17, 2);
	init_entry = fm_global_init_ADDRESS;
	b = 0xBu;
	for (;;)
	{
		z80_block(43, 5);
		register_number = REGISTER_VALUE_PAIR(init_entry)->register_number;
		init_entry++;
		value = REGISTER_VALUE_PAIR(init_entry - 0x1u)->value;
		init_entry++;
		fm_register = register_number;
		write_fm(fm_register, value);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(17, 2);
	init_entry = fm_operator_init_ADDRESS;
	b = 0x24u;
	for (;;)
	{
		z80_block(43, 5);
		register_number = REGISTER_VALUE_PAIR(init_entry)->register_number;
		init_entry++;
		value = REGISTER_VALUE_PAIR(init_entry - 0x1u)->value;
		init_entry++;
		fm_register = register_number;
		write_fm(fm_register, value);
		z80_block(67, 6);
		fm_register = register_number;
		write_fm_part2(fm_register, value);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	last_register = ((uint32_t)b << 8) | register_number;
	return last_register;
}

/* Writes value to the YM3438 register fm_register on the part fm_channel's bit 2 picks (write_fm_part2
   when it is set, else write_fm); hands fm_register back with value in its high byte. */
uint16_t write_fm_channel(uint8_t fm_register, uint8_t value)
{
	uint8_t b = 0;
	uint16_t bc = 0, bc_2 = 0;
	uint8_t t1 = 0;

	z80_block(39, 6);
	b = value;
	t1 = fm_channel;
	value = b;
	if (((t1 >> 2) & 1u) != 0u)
	{
		bc = (fm_register & 0xFFu) | ((uint32_t)b << 8);
		write_fm_part2((uint8_t)bc, value);
		return bc;
	}
	bc_2 = (fm_register & 0xFFu) | ((uint32_t)b << 8);
	write_fm((uint8_t)bc_2, value);
	return bc_2;
}

/* Writes value to the YM3438's register fm_register on part I (FM_ADDRESS, then FM_DATA), with the
   interrupts off between the two. */
void write_fm(uint8_t fm_register, uint8_t value)
{
	uint8_t saved_value = 0;

	z80_block(73, 10);
	saved_value = value;
	interrupts_off();
	port_out(FM_ADDRESS, (uint8_t)fm_register);
	port_out(FM_DATA, saved_value);
	interrupts_on();
}

/* Writes value to the YM3438's register fm_register on part II (FM_PART2_ADDRESS, then FM_PART2_DATA),
   with the interrupts off between the two. */
void write_fm_part2(uint8_t fm_register, uint8_t value)
{
	uint8_t saved_value = 0;

	z80_block(73, 10);
	saved_value = value;
	interrupts_off();
	port_out(FM_PART2_ADDRESS, (uint8_t)fm_register);
	port_out(FM_PART2_DATA, saved_value);
	interrupts_on();
}

/* Sets the MultiPCM up: 3 into its place C009 before and after (what for is not known: the chip as MAME
   has it ignores it), PCM_BANKS 1A (the first sample window on bank 2, the second on bank 3), then for
   the first 28 entries of voice_chip_channels: the voice selected, pan 80, keyed off, level FE (silent).
   The last 4 of those 28 are YM3438 codes (80-82, 84), so MultiPCM voices 0-2 and 4 are set up twice and
   voices 24-27 (the tyre noises' and the engines') not at all. */
void init_pcm_chip(void)
{
	uint8_t b = 0, chip_code = 0, pcm_register = 0, value = 0;
	uint16_t code_entry = 0;

	z80_block(55, 6);
	z80_write8(MULTIPCM_C009, 3u);
	port_out(PCM_BANKS, 0x1Au);
	code_entry = voice_chip_channels_ADDRESS;
	b = 0x1Cu;
	for (;;)
	{
		z80_block(13, 2);
		chip_code = VOICE_CHIP_CODES(code_entry)[0];
		code_entry++;
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(48, 5);
		z80_write8(pcm_voice_select, chip_code);
		value = 0x80u;
		pcm_register = 0u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 3);
		value = 0u;
		pcm_register = 4u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 3);
		value = 0xFEu;
		pcm_register = 5u;
		write_pcm_register(pcm_register, value);
		z80_block(8, 1);
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(30, 3);
	z80_write8(MULTIPCM_C009, 3u);
}

/* Writes value to the register pcm_register of the MultiPCM voice selected in pcm_voice_select: waits
   while the chip is busy (pcm_data bit 0), puts the register number in pcm_register_select, waits again,
   then the value in pcm_data. */
void write_pcm_register(uint8_t pcm_register, uint8_t value)
{
	uint8_t saved_value = 0;

	z80_block(11, 1);
	saved_value = value;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(55, 4);
	z80_write8(pcm_register_select, pcm_register);
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(33, 3);
	z80_write8(pcm_data, saved_value);
}

/* Gives the 30 voices their chip_channel from voice_chip_channels: voices 0-23 the MultiPCM voices
   (00-06, 08-0E, 10-16, 18-1A), voices 24-29 the YM3438's channels (80-82 on part I, 84-86 on part II). */
void init_voice_channels(uint32_t unused_c)
{
	uint8_t b = 0;
	uint16_t code_entry = 0, voice = 0;
	uint8_t saved_b = 0;

	z80_block(27, 3);
	voice = voice_0_chip_channel_ADDRESS;
	code_entry = voice_chip_channels_ADDRESS;
	b = 0x1Eu;
	for (;;)
	{
		z80_block(70, 8);
		saved_b = b;
		VOICE(voice - 0x1u)->chip_channel = VOICE_CHIP_CODES(code_entry)[0];
		code_entry++;
		voice += 9u;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

/* Sets both screens' 13 parts from sound set 0 (part_templates[0]): its 8 music parts and, after its
   bank byte, the 5 parts of MIDI channels A-E (the effects' and the fades'), for screen 1 (midi_part
   0-12) and screen 2 (midi_part 13-25); then every part's effective_volume: volume x (FF with flags bit
   0, else screen_1_music_volume, for screen 2's parts too) / 256, + 1 if not 0. The set's bank byte does
   not go to PCM_BANKS here. */
void init_parts(void)
{
	uint8_t a = 0, entry = 0, factor = 0;
	uint16_t bc = 0, hl = 0, part = 0;
	uint16_t saved_hl = 0;

	z80_block(34, 3);
	entry = 0u;
	hl = part_templates_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(178, 21);
	saved_hl = hl;
	move_block(hl, MIDI_PART_ADDRESS(0), 0x50u, 1u);
	move_block((uint16_t)(hl + 0x51u), MIDI_PART_ADDRESS(8), 0x32u, 1u);
	hl = saved_hl;
	move_block(hl, MIDI_PART_ADDRESS(13), 0x50u, 1u);
	hl += 0x51u;
	move_block(hl, MIDI_PART_ADDRESS(21), 0x32u, 1u);
	hl += 0x32u;
	part = MIDI_PART_ADDRESS(0);
	bc = 0x1A00u;
	for (;;)
	{
		z80_block(56, 6);
		hl = (hl & 0xFFu) | ((uint32_t)MIDI_PART(part)->volume << 8);
		a = 0xFFu;
		if ((MIDI_PART(part)->flags & 1u) == 0u)
		{
			z80_block(13, 1);
			a = screen_1_music_volume;
		}
		z80_block(21, 2);
		factor = a;
		hl = multiply_bytes(factor, hl);
		z80_block(18, 3);
		a = (uint8_t)(hl >> 8);
		if (a != 0u)
		{
			z80_block(4, 1);
			a++;
		}
		z80_block(52, 6);
		MIDI_PART(part)->effective_volume = a;
		part += 0xAu;
		bc = (uint32_t)(uint8_t)((bc >> 8) - 1u) << 8;
		if (bc >> 8 == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

/* Hands back the 16-bit entry 'entry' of the table of words at 'table'. */
uint16_t read_table_word(uint16_t table, uint8_t entry)
{
	uint16_t bc = 0;

	z80_block(88, 11);
	bc = entry;
	table = table + bc + bc;
	table = z80_read16(table);
	return table;
}

/* Takes one message off screen 1's MIDI queue each main loop pass, while screen_1_midi_count is not 0:
   its first byte (read_midi_byte_screen_1) is a new screen_1_midi_status, or a data byte under the last
   one; every part of screen 1 that plays (flags bit 7) on the message's channel gets it
   (play_midi_message_screen_1), each reading from the same place (screen_1_midi_read is put back after
   each); then skip_midi_data_screen_1 moves past the message. */
void handle_midi_message_screen_1(void)
{
	uint8_t a = 0, b = 0;
	uint16_t bc = 0, hl = 0, part = 0, screen_page = 0;
	uint8_t saved_b = 0, saved_bc = 0;
	uint16_t saved_hl = 0;

	z80_block(26, 4);
	screen_page = screen_1_midi_count_ADDRESS;
	if (screen_1_midi_count == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	SCREEN_PAGE(screen_page - 0x59u)->midi_count--;
	read_midi_byte_screen_1(&bc, &a);
	z80_block(18, 3);
	if (a >> 7 == 0u)
	{
		z80_block(13, 1);
		a = screen_1_midi_status;
	}
	else
	{
		z80_block(23, 2);
		screen_1_midi_status = a;
	}
	z80_block(49, 7);
	hl = ((a & 0xFu) << 8) | screen_1_midi_read;
	b = 0xDu;
	part = MIDI_PART_ADDRESS(0);
	for (;;)
	{
		z80_block(30, 3);
		if (MIDI_PART(part)->flags >> 7)
		{
			z80_block(33, 4);
			if (hl >> 8 == MIDI_PART(part)->midi_channel)
			{
				z80_block(39, 3);
				saved_b = b;
				saved_bc = (uint8_t)bc;
				saved_hl = hl;
				play_midi_message_screen_1((uint8_t)bc, part);
				z80_block(37, 4);
				hl = saved_hl;
				bc = saved_bc;
				b = saved_b;
				screen_1_midi_read = (uint8_t)hl;
			}
		}
		z80_block(33, 4);
		part += 0xAu;
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	skip_midi_data_screen_1((uint8_t)bc);
}

/* Moves screen_1_midi_read to the last byte of the message that starts with first_byte: past a status
   byte, then past one data byte for C0 and D0 (program change, channel pressure) and two for the others.
   play_midi_message_screen_1 does the same for A0 and D0, which play nothing
   (handle_midi_message_screen_1 puts the place back and skips the message itself). */
void skip_midi_data_screen_1(uint8_t first_byte)
{
	uint8_t a = 0;
	uint16_t hl = 0;

	z80_block(28, 4);
	hl = screen_1_midi_read_ADDRESS;
	if ((uint8_t)first_byte >> 7 != 0u)
	{
		z80_block(22, 3);
		a = first_byte & 0xF0u;
		screen_1_midi_read++;
	}
	else
	{
		z80_block(30, 3);
		a = screen_1_midi_status & ~0xFu;
	}
	z80_block(12, 2);
	if (a == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (a == 0xD0u)
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
	bool cut_open_hi_hat_screen_1 = false, release_note_screen_1 = false, start_voice_screen_1 = false, take_voice_screen_1 = false;
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, bend_range = 0, c = 0, chip_channel = 0, effective_volume = 0, entry = 0, factor = 0, first_byte = 0, flags = 0, fm_register = 0, hl_2 = 0, ignored8 = 0, midi_byte = 0, midi_channel = 0, note = 0, pcm_register = 0, program = 0, value = 0;
	uint16_t bc = 0, de = 0, hl = 0, ignored16 = 0, midi_byte_copy = 0, music_part = 0, pitch_bends = 0, pitch_entry = 0, voice = 0, voice_2 = 0;
	uint8_t saved_a = 0, saved_b = 0;
	uint16_t stack_2 = 0, saved_hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	c = (uint8_t)message_byte;
	z80_block(37, 4);
	a = screen_1_midi_status & ~0xFu;
	do
	{
		if (release_note_screen_1 || a == 0x90u)
		{
			if (!release_note_screen_1)
			{
				z80_block(18, 3);
				if (c >> 7)
				{
					z80_block(17, 1);
					read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
					c = (uint8_t)midi_byte_copy;
				}
				z80_block(109, 12);
				b = c;
				a = MIDI_PART(part)->transpose + c;
				note_number = a;
				note_code = note_codes[a];
				read_midi_byte_screen_1(&midi_byte_copy, &midi_byte);
				c = (uint8_t)midi_byte_copy;
				z80_block(14, 2);
			}
			if (release_note_screen_1 || midi_byte == 0u)
			{
				z80_block(31, 4);
				program = MIDI_PART(part)->program;
				if (program >= 0xF0u)
				{
					z80_cycles(6);
					return;
				}
				z80_block(27, 2);
				hl = instruments_ADDRESS;
				hl = read_table_word(hl, program);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(51, 6);
					voice_2 = VOICE_ADDRESS(24);
					de = 9u;
					b = 6u;
					a = screen_1_midi_status & 0xFu;
					for (;;)
					{
						z80_block(29, 3);
						if (a == VOICE(voice_2)->midi_channel)
						{
							z80_block(46, 5);
							hl_2 = a;
							if (note_code == VOICE(voice_2)->note)
							{
								z80_block(37, 5);
								flags = VOICE(voice_2)->flags;
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
							a = (uint8_t)hl_2;
						}
						z80_block(23, 3);
						voice_2 += de;
						b--;
						if (b == 0u)
						{
							z80_block(10, 1);
							return;
						}
						z80_cycles(5);
					}
					z80_block(65, 9);
					if (screen_1_channel_controls[hl_2] >> 7)
					{
						z80_block(33, 3);
						VOICE(voice_2)->flags |= 2u;
						return;
					}
					z80_block(85, 10);
					VOICE(voice_2)->flags = 0u;
					VOICE(voice_2)->midi_channel = 0u;
					value = VOICE(voice_2)->chip_channel & 7u;
					fm_register = 0x28u;
					write_fm(fm_register, value);
					return;
				}
				z80_block(51, 6);
				voice_2 = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
				a = screen_1_midi_status & 0xFu;
				for (;;)
				{
					z80_block(29, 3);
					if (a == VOICE(voice_2)->midi_channel)
					{
						z80_block(46, 5);
						hl_2 = a;
						if (note_code == VOICE(voice_2)->note)
						{
							z80_block(37, 5);
							flags = VOICE(voice_2)->flags;
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
						a = (uint8_t)hl_2;
					}
					z80_block(23, 3);
					voice_2 += de;
					b--;
					if (b == 0u)
					{
						z80_block(10, 1);
						return;
					}
					z80_cycles(5);
				}
				z80_block(65, 9);
				if (screen_1_channel_controls[hl_2] >> 7 == 0u)
				{
					z80_block(33, 3);
					VOICE(voice_2)->flags |= 1u;
					return;
				}
				z80_block(33, 3);
				VOICE(voice_2)->flags |= 2u;
				return;
			}
			z80_block(88, 10);
			de = midi_byte;
			note_level = velocity_levels[de];
			a = MIDI_PART(part)->program;
			if (a < 0xF0u)
			{
				z80_block(27, 2);
				hl = instruments_ADDRESS;
				hl = read_table_word(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(59, 7);
					sample_or_patch = z80_read8((uint16_t)(hl + 2u));
					voice = VOICE_ADDRESS(24);
					de = 9u;
					b = 6u;
					for (;;)
					{
						z80_block(21, 3);
						if (VOICE(voice)->flags == 0u)
							break;
						z80_block(19, 2);
						voice += de;
						b--;
						if (b == 0u)
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
					a = note_code;
					VOICE(voice - 0x2u)->note = a;
					c = a;
					voice++;
					a = screen_1_midi_status & 0xFu;
					VOICE(voice - 0x3u)->midi_channel = a;
					b = a;
					voice += 2u;
					a = note_level;
					VOICE(voice - 0x5u)->level = a;
					hl = (voice & 0xFFu) | ((uint32_t)a << 8);
					effective_volume = MIDI_PART(part)->effective_volume;
					hl = multiply_bytes(effective_volume, hl);
					z80_block(112, 15);
					note_level = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
					de = b;
					voice_modulation = (z80_read8((uint16_t)(screen_1_channel_controls_ADDRESS + de)) & 0xFu) | 0x40u;
					pitch_bends = screen_1_pitch_bends_ADDRESS;
					note = c;
					start_fm_note(note, de, pitch_bends);
					return;
				}
				for (;;)
				{
					z80_block(44, 6);
					hl++;
					if (note_number <= z80_read8(hl))
						break;
					z80_block(28, 4);
					hl += 3u;
				}
				z80_block(78, 9);
				hl++;
				sample_or_patch = z80_read8(hl);
				hl++;
				split_tune = z80_read8(hl);
				split_octave = z80_read8((uint16_t)(hl + 1u));
				take_voice_screen_1 = true;
			}
			else
			{
				z80_block(34, 3);
				a += 0x10u;
				hl = drum_kits_ADDRESS;
				hl = read_table_word(hl, a);
				z80_block(112, 14);
				de = b;
				hl = hl + de + de + de;
				drum_pitch = z80_read8(hl);
				hl++;
				c = z80_read8(hl);
				a = z80_read8((uint16_t)(hl + 1u));
				sample_or_patch = a;
				if (a == 0xFFu)
				{
					z80_cycles(6);
					return;
				}
				z80_block(37, 4);
				take_voice_screen_1 = (screen_1_midi_status & 0xFu) != 0xFu;
				if (!take_voice_screen_1)
				{
					z80_block(21, 3);
					a = b;
					if (a == 0x2Cu)
						cut_open_hi_hat_screen_1 = true;
					else
					{
						z80_block(17, 2);
						take_voice_screen_1 = a != 0x2Au;
						if (!take_voice_screen_1)
							cut_open_hi_hat_screen_1 = true;
					}
					if (!take_voice_screen_1)
					{
						if (cut_open_hi_hat_screen_1)
						{
							cut_open_hi_hat_screen_1 = false;
							z80_block(27, 3);
							voice = voice_0_note_ADDRESS;
							de = 9u;
							b = 0x18u;
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
								voice += de;
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							take_voice_screen_1 = true;
						}
					}
				}
			}
			if (take_voice_screen_1)
			{
				z80_block(33, 3);
				voice = next_voice;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(21, 3);
					if ((uint8_t)voice == 0xE8u)
					{
						z80_cycles(7);
						voice = reset_next_voice();
					}
					z80_block(21, 3);
					a = 0u;
					if (0u == VOICE(voice)->flags)
					{
						z80_block(58, 5);
						stack_2 = voice;
						next_voice = voice + de;
						hl = stack_2;
						start_voice_screen_1 = true;
					}
					if (start_voice_screen_1)
						break;
					z80_block(19, 2);
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				if (!start_voice_screen_1)
				{
					z80_block(10, 1);
					z80_block(17, 2);
					voice = voice_0_allocation_order_ADDRESS;
					b = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (a == VOICE(voice - 0x6u)->allocation_order)
							break;
						z80_block(19, 2);
						voice += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(42, 5);
					VOICE(voice - 0x6u)->allocation_order = 0x18u;
					stack_2 = voice;
					voice = voice_0_allocation_order_ADDRESS;
					b = 0x18u;
					for (;;)
					{
						z80_block(30, 3);
						VOICE(voice - 0x6u)->allocation_order--;
						voice += de;
						b--;
						if (b == 0u)
							break;
						z80_cycles(5);
					}
					z80_block(52, 5);
					pcm_voices_used--;
					hl = stack_2 - 6u;
				}
				z80_block(46, 5);
				z80_write8(hl, 0x40u);
				if (MIDI_PART(part)->program < 0xF0u)
				{
					do
					{
						z80_block(27, 3);
					} while (z80_read8(pcm_data) & 1u);
					z80_block(479, 64);
					hl++;
					z80_write8(pcm_voice_select, z80_read8(hl));
					hl++;
					a = note_code;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = screen_1_midi_status & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					z80_write8(hl, note_level);
					hl++;
					a = pcm_voices_used;
					z80_write8(hl, a);
					pcm_voices_used = a + 1u;
					hl++;
					z80_write8(hl, split_tune);
					z80_write8((uint16_t)(hl + 1u), split_octave);
					de = (uint8_t)(b * 2u);
					hl = screen_1_pitch_bends_ADDRESS + de;
					de = split_tune;
					stack_2 = z80_read16(hl) + de;
					hl = pcm_pitch_tables_ADDRESS + (uint8_t)((c & 0xFu) * 2u);
					pitch_entry = PCM_PITCH_RUNS(hl)[0] + stack_2 + stack_2;
					hl = PCM_PITCH_STEPS(pitch_entry)[0];
					hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((c & ~0xFu) + (hl >> 8)) << 8);
					value = split_octave + (hl >> 8);
				}
				else
				{
					z80_block(15, 2);
					z80_write8(hl, z80_read8(hl) | 0x10u);
					do
					{
						z80_block(27, 3);
					} while (z80_read8(pcm_data) & 1u);
					z80_block(171, 21);
					hl++;
					z80_write8(pcm_voice_select, z80_read8(hl));
					hl++;
					z80_write8(hl, note_code);
					hl++;
					z80_write8(hl, screen_1_midi_status & 0xFu);
					hl++;
					z80_write8(hl, c);
					hl++;
					z80_write8(hl, note_level);
					hl++;
					if ((screen_1_midi_status & 0xFu) == 0xFu)
					{
						z80_block(37, 4);
						a = pcm_voices_used;
						z80_write8(hl, a);
						pcm_voices_used = a + 1u;
					}
					else
					{
						z80_block(24, 3);
						z80_write8(hl, 0xFFu);
					}
					z80_block(30, 3);
					value = drum_pitch;
					hl = 0u;
				}
				z80_block(24, 2);
				pcm_register = 3u;
				write_pcm_register(pcm_register, value);
				z80_block(28, 3);
				value = (uint8_t)hl;
				pcm_register = 2u;
				write_pcm_register(pcm_register, value);
				z80_block(37, 3);
				value = sample_or_patch;
				pcm_register = 1u;
				write_pcm_register(pcm_register, value);
				z80_block(53, 5);
				hl = (hl & 0xFFu) | ((uint32_t)note_level << 8);
				effective_volume = MIDI_PART(part)->effective_volume;
				hl = multiply_bytes(effective_volume, hl);
				z80_block(39, 5);
				value = ~(hl >> 8) | 1u;
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
				return;
			}
		}
		z80_block(17, 2);
		if (a == 0x80u)
		{
			z80_block(18, 3);
			if (c >> 7)
			{
				z80_block(17, 1);
				read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
				c = (uint8_t)midi_byte_copy;
			}
			z80_block(96, 11);
			note_code = note_codes[(uint8_t)(MIDI_PART(part)->transpose + c)];
			screen_1_midi_read++;
			release_note_screen_1 = true;
		}
	} while (release_note_screen_1);
	z80_block(17, 2);
	if (a == 0xE0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		b = c;
		read_midi_byte_screen_1(&ignored16, &midi_byte);
		z80_block(26, 5);
		carry = midi_byte & 1u;
		c = midi_byte >> 1;
		a = b;
		if (carry)
		{
			z80_block(7, 1);
			a |= 0x80u;
		}
		z80_block(78, 18);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		if ((c & 1u) == 0u)
		{
			z80_block(18, 3);
			t1 = ~a + 1u;
			a = t1;
			if (t1 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(57, 6);
		de = a;
		bend_range = MIDI_PART(part)->bend_range;
		hl = bend_range_tables_ADDRESS;
		hl = read_table_word(hl, bend_range);
		z80_block(36, 5);
		a = z80_read8((uint16_t)(hl + de));
		carry = c & 1u;
		c = c >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			c--;
			t2 = ~a + 1u;
			a = t2;
			if (t2 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(145, 19);
		b = c;
		c = a;
		pitch_bend = ((uint32_t)b << 8) | c;
		hl = screen_1_pitch_bends_ADDRESS + (uint8_t)((screen_1_midi_status & 0xFu) * 2u);
		z80_write8(hl, c);
		z80_write8((uint16_t)(hl + 1u), b);
		voice_2 = VOICE_ADDRESS(0);
		b = 0x1Eu;
		a = screen_1_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (a == VOICE(voice_2)->midi_channel)
			{
				z80_block(48, 6);
				saved_a = a;
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
				a = saved_a;
			}
			z80_block(33, 4);
			voice_2 += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xB0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		b = c;
		read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
		c = (uint8_t)midi_byte_copy;
		z80_block(21, 3);
		a = b;
		carry = a == 0u;
		if (a == 1u)
		{
			z80_block(123, 19);
			t3 = c;
			a = (t3 >> 1) | (carry << 7);
			carry = t3 & 1u;
			t4 = a;
			a = (t4 >> 1) | (carry << 7);
			carry = t4 & 1u;
			t5 = a;
			a = (t5 >> 1) | (carry << 7);
			carry = t5 & 1u;
			a = ((a >> 1) | (carry << 7)) & 7u;
			c = a;
			voice_modulation = a;
			hl = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
			a = z80_read8(hl);
			b = a;
			if ((a & 0x7Fu) == c)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(hl, (b & 0x80u) | c);
			voice = voice_0_midi_channel_ADDRESS;
			b = 0x1Eu;
			a = screen_1_midi_status & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (a == VOICE(voice - 0x3u)->midi_channel)
				{
					z80_block(47, 8);
					de = (uint32_t)a << 8;
					voice = voice - 1u - 1u - 1u;
					flags = VOICE(voice)->flags;
					if (((flags >> 3) & 1u) == 0u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							bc = (uint32_t)b << 8;
							set_voice_modulation_screen_1(part, &bc, &voice);
							b = (uint8_t)(bc >> 8);
						}
					}
					z80_block(22, 4);
					voice += 3u;
					a = (uint8_t)(de >> 8);
				}
				z80_block(29, 3);
				voice += 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 7u)
		{
			z80_block(18, 3);
			t6 = c * 2u;
			a = t6;
			if (t6 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(60, 7);
			MIDI_PART(part)->volume = a;
			hl = (uint32_t)a << 8;
			a = 0xFFu;
			if ((MIDI_PART(part)->flags & 1u) == 0u)
			{
				z80_block(13, 1);
				a = screen_1_music_volume;
			}
			z80_block(21, 2);
			factor = a;
			hl = multiply_bytes(factor, hl);
			z80_block(18, 3);
			a = (uint8_t)(hl >> 8);
			if (a != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(36, 4);
			MIDI_PART(part)->effective_volume = a;
			hl = VOICE_ADDRESS(0);
			b = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				stack_2 = hl;
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if (((z80_read8(hl) >> 3) & 1u) == 0u)
					{
						z80_block(55, 7);
						hl += 3u;
						if ((screen_1_midi_status & 0xFu) == z80_read8(hl))
						{
							z80_block(37, 6);
							hl = hl - 1u - 1u;
							c = z80_read8(hl);
							if (c >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(pcm_data) & 1u);
								z80_block(81, 8);
								z80_write8(pcm_voice_select, c);
								hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
								effective_volume = MIDI_PART(part)->effective_volume;
								hl = multiply_bytes(effective_volume, hl);
								z80_block(39, 5);
								value = ~(hl >> 8) & 0xFEu;
								pcm_register = 5u;
								write_pcm_register(pcm_register, value);
							}
							else
							{
								z80_block(17, 1);
								chip_channel = c;
								apply_fm_channel_volume(chip_channel, hl, part);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				hl = stack_2 + 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x10u)
		{
			z80_block(18, 3);
			t7 = c * 2u;
			a = t7;
			if (t7 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(34, 4);
			screen_1_music_volume = a;
			music_part = MIDI_PART_ADDRESS(0);
			b = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_b = b;
				hl = (hl & 0xFFu) | ((uint32_t)MIDI_PART(music_part)->volume << 8);
				a = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					a = screen_1_music_volume;
				}
				z80_block(21, 2);
				factor = a;
				hl = multiply_bytes(factor, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(36, 4);
				MIDI_PART(music_part)->effective_volume = a;
				hl = voice_0_midi_channel_ADDRESS;
				b = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_hl = hl;
					if (MIDI_PART(music_part)->midi_channel == z80_read8(hl))
					{
						z80_block(40, 6);
						hl = hl - 1u - 1u - 1u;
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if (((z80_read8(hl) >> 3) & 1u) == 0u)
							{
								z80_block(31, 5);
								hl++;
								c = z80_read8(hl);
								if (c >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(pcm_data) & 1u);
									z80_block(81, 8);
									z80_write8(pcm_voice_select, c);
									hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
									effective_volume = MIDI_PART(music_part)->effective_volume;
									hl = multiply_bytes(effective_volume, hl);
									z80_block(39, 5);
									value = ~(hl >> 8) & 0xFEu;
									pcm_register = 5u;
									write_pcm_register(pcm_register, value);
								}
								else
								{
									z80_block(17, 1);
									chip_channel = c;
									apply_fm_music_volume(chip_channel, hl, music_part);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					hl = saved_hl + 9u;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				music_part += 0xAu;
				b = saved_b - 1u;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x40u)
		{
			z80_block(21, 3);
			if (c >= 0x40u)
			{
				z80_block(77, 9);
				hl = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
				z80_write8(hl, z80_read8(hl) | 0x80u);
				return;
			}
			z80_block(51, 6);
			voice_2 = VOICE_ADDRESS(0);
			de = 9u;
			b = 0x1Eu;
			a = screen_1_midi_status & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (a == VOICE(voice_2)->midi_channel)
				{
					z80_block(41, 6);
					hl_2 = a;
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
					a = (uint8_t)hl_2;
				}
				z80_block(23, 3);
				voice_2 += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			hl = screen_1_channel_controls_ADDRESS + (screen_1_midi_status & 0xFu);
			z80_write8(hl, z80_read8(hl) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (a == 0x7Bu)
		{
			z80_block(47, 5);
			a = screen_1_midi_status & 0xFu;
			voice = VOICE_ADDRESS(0);
			de = 9u;
			b = 0x18u;
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
						if (a != VOICE(voice - 0x3u)->midi_channel)
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
				voice += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x50u)
		{
			z80_block(21, 3);
			if (c == 0x7Eu)
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
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
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
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
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			hl = screen_1_channel_controls_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			hl = screen_1_pitch_bends_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x20u)
		{
			z80_block(31, 3);
			entry = c;
			hl = part_templates_ADDRESS;
			hl = read_table_word(hl, entry);
			z80_block(112, 13);
			move_block(hl, MIDI_PART_ADDRESS(0), 0x50u, 1u);
			hl += 0x50u;
			a = (pcm_banks & 0x38u) | z80_read8(hl);
			port_out(PCM_BANKS, a);
			pcm_banks = a;
			music_part = MIDI_PART_ADDRESS(0);
			b = 8u;
			for (;;)
			{
				z80_block(56, 6);
				hl = (hl & 0xFFu) | ((uint32_t)MIDI_PART(music_part)->volume << 8);
				a = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					a = screen_1_music_volume;
				}
				z80_block(21, 2);
				factor = a;
				hl = multiply_bytes(factor, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(52, 6);
				MIDI_PART(music_part)->effective_volume = a;
				music_part += 0xAu;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		c = note_codes[c];
		voice = VOICE_ADDRESS(0);
		b = 0x18u;
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
					if (c != VOICE(voice - 0x2u)->note)
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
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xC0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_1(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(29, 3);
		MIDI_PART(part)->program = c;
		return;
	}
	z80_block(17, 2);
	if (a != 0xD0u)
	{
		z80_block(17, 2);
		if (a != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	first_byte = c;
	skip_midi_data_screen_1(first_byte);
}

/* Retunes a sounding voice after a pitch bend of screen 1: an FM voice through retune_fm_voice; a
   MultiPCM voice gets its note's pitch again from pcm_pitch_tables[semitone] at pitch_bend + its tune,
   its octave + octave_shift (registers 3 and 2). The same code as retune_voice_screen_2. */
void retune_voice_screen_1(uint16_t voice)
{
	uint8_t c = 0, chip_channel = 0, note = 0, pcm_register = 0, value = 0;
	uint16_t de = 0, hl = 0, pitch_entry = 0, tune = 0;

	z80_block(37, 5);
	chip_channel = VOICE(voice)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		retune_fm_voice(voice, chip_channel);
		return;
	}
	z80_block(4, 1);
	c = chip_channel;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(291, 38);
	z80_write8(pcm_voice_select, c);
	note = VOICE(voice)->note;
	hl = pcm_pitch_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	de = pitch_bend;
	pitch_entry = PCM_PITCH_RUNS(hl)[0] + de + de;
	tune = VOICE(voice)->tune;
	pitch_entry = pitch_entry + tune + tune;
	hl = PCM_PITCH_STEPS(pitch_entry)[0];
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((note & ~0xFu) + (hl >> 8)) << 8);
	value = VOICE(voice)->octave_shift + (hl >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(21, 3);
	value = (uint8_t)hl;
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
	uint8_t a = 0, c = 0, chip_channel = 0, value = 0;
	uint16_t bc = 0;

	z80_block(31, 5);
	voice++;
	chip_channel = VOICE(voice - 0x1u)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		z80_block(79, 9);
		a = (uint8_t)(((voices_left & ~0xFFu) | chip_channel) >> 8);
		fm_channel = a;
		c = (a & 3u) - 0x4Cu;
		value = (voice_modulation & 0x3Fu) | 0x40u;
		bc = c;
		bc = write_fm_channel((uint8_t)bc, value);
		z80_block(16, 2);
		voice--;
		voices_left = (bc & ~0xFFu) | c;
		*p_voices_left = voices_left;
		*p_voice = voice;
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(73, 7);
	z80_write8(pcm_voice_select, chip_channel);
	c = 6u;
	value = voice_modulation | MIDI_PART(part)->lfo_frequency;
	bc = (voices_left & ~0xFFu) | 6u;
	write_pcm_register((uint8_t)bc, value);
	z80_block(16, 2);
	voice--;
	voices_left = (bc & ~0xFFu) | c;
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
	uint8_t a = 0, b = 0, c = 0, pcm_register = 0;
	uint16_t de = 0, hl = 0, voice = 0;
	uint8_t stack_5 = 0, saved_b = 0;
	uint16_t saved_hl = 0;

	z80_block(27, 3);
	hl = VOICE_ADDRESS(0);
	de = 9u;
	b = 0x18u;
	for (;;)
	{
		z80_block(22, 3);
		if (z80_read8(hl) & 1u)
		{
			z80_block(51, 6);
			saved_b = b;
			saved_hl = hl;
			stack_5 = z80_read8(hl);
			z80_write8(hl, 0u);
			do
			{
				z80_block(27, 3);
			} while (z80_read8(pcm_data) & 1u);
			z80_block(54, 6);
			hl++;
			z80_write8(pcm_voice_select, z80_read8(hl));
			a = 0u;
			pcm_register = 4u;
			write_pcm_register(pcm_register, a);
			z80_block(79, 12);
			hl += 2u;
			b = z80_read8(hl);
			z80_write8(hl, a);
			hl += 3u;
			c = stack_5;
			if (b == 0xFu)
			{
				z80_block(55, 6);
				a = z80_read8(hl);
				pcm_voices_used--;
				voice = voice_0_allocation_order_ADDRESS;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(17, 2);
					if (a < VOICE(voice - 0x6u)->allocation_order)
					{
						z80_block(11, 1);
						VOICE(voice - 0x6u)->allocation_order--;
					}
					z80_block(19, 2);
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			else
			{
				z80_block(22, 4);
				if (((c >> 4) & 1u) == 0u)
				{
					z80_block(55, 6);
					a = z80_read8(hl);
					pcm_voices_used--;
					voice = voice_0_allocation_order_ADDRESS;
					de = 9u;
					b = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (a < VOICE(voice - 0x6u)->allocation_order)
						{
							z80_block(11, 1);
							VOICE(voice - 0x6u)->allocation_order--;
						}
						z80_block(19, 2);
						voice += de;
						b--;
						if (b == 0u)
							break;
						z80_cycles(5);
					}
				}
			}
			z80_block(30, 3);
			hl = saved_hl;
			b = saved_b;
			de = 9u;
		}
		z80_block(19, 2);
		hl += de;
		b--;
		if (b == 0u)
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
	uint8_t a = 0, b = 0;
	uint16_t bc = 0, hl = 0, part = 0, screen_page = 0;
	uint8_t saved_b = 0, saved_bc = 0;
	uint16_t saved_hl = 0;

	z80_block(26, 4);
	screen_page = screen_2_midi_count_ADDRESS;
	if (screen_2_midi_count == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(28, 2);
	SCREEN_PAGE(screen_page - 0x59u)->midi_count--;
	read_midi_byte_screen_2(&bc, &a);
	z80_block(18, 3);
	if (a >> 7 == 0u)
	{
		z80_block(13, 1);
		a = screen_2_midi_status;
	}
	else
	{
		z80_block(23, 2);
		screen_2_midi_status = a;
	}
	z80_block(49, 7);
	hl = ((a & 0xFu) << 8) | screen_2_midi_read;
	b = 0xDu;
	part = MIDI_PART_ADDRESS(13);
	for (;;)
	{
		z80_block(30, 3);
		if (MIDI_PART(part)->flags >> 7)
		{
			z80_block(33, 4);
			if (hl >> 8 == MIDI_PART(part)->midi_channel)
			{
				z80_block(39, 3);
				saved_b = b;
				saved_bc = (uint8_t)bc;
				saved_hl = hl;
				play_midi_message_screen_2((uint8_t)bc, part);
				z80_block(37, 4);
				hl = saved_hl;
				bc = saved_bc;
				b = saved_b;
				screen_2_midi_read = (uint8_t)hl;
			}
		}
		z80_block(33, 4);
		part += 0xAu;
		b--;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	skip_midi_data_screen_2((uint8_t)bc);
}

/* As skip_midi_data_screen_1 on screen_2_midi_read and screen_2_midi_status. */
void skip_midi_data_screen_2(uint8_t first_byte)
{
	uint8_t a = 0;
	uint16_t hl = 0;

	z80_block(28, 4);
	hl = screen_2_midi_read_ADDRESS;
	if ((uint8_t)first_byte >> 7 != 0u)
	{
		z80_block(22, 3);
		a = first_byte & 0xF0u;
		screen_2_midi_read++;
	}
	else
	{
		z80_block(30, 3);
		a = screen_2_midi_status & ~0xFu;
	}
	z80_block(12, 2);
	if (a == 0xC0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12, 2);
	if (a == 0xD0u)
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
	bool cut_open_hi_hat_screen_2 = false, release_note_screen_2 = false, start_voice_screen_2 = false, take_voice_screen_2 = false;
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, bend_range = 0, c = 0, chip_channel = 0, effective_volume = 0, entry = 0, factor = 0, first_byte = 0, flags = 0, fm_register = 0, hl_2 = 0, ignored8 = 0, midi_byte = 0, midi_channel = 0, note = 0, pcm_register = 0, program = 0, value = 0;
	uint16_t bc = 0, de = 0, hl = 0, ignored16 = 0, midi_byte_copy = 0, music_part = 0, pitch_bends = 0, pitch_entry = 0, voice = 0, voice_2 = 0;
	uint8_t saved_a = 0, saved_b = 0;
	uint16_t stack_2 = 0, saved_hl = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	c = (uint8_t)message_byte;
	z80_block(37, 4);
	a = screen_2_midi_status & ~0xFu;
	do
	{
		if (release_note_screen_2 || a == 0x90u)
		{
			if (!release_note_screen_2)
			{
				z80_block(18, 3);
				if (c >> 7)
				{
					z80_block(17, 1);
					read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
					c = (uint8_t)midi_byte_copy;
				}
				z80_block(109, 12);
				b = c;
				a = MIDI_PART(part)->transpose + c;
				note_number = a;
				note_code = note_codes[a];
				read_midi_byte_screen_2(&midi_byte_copy, &midi_byte);
				c = (uint8_t)midi_byte_copy;
				z80_block(14, 2);
			}
			if (release_note_screen_2 || midi_byte == 0u)
			{
				z80_block(31, 4);
				program = MIDI_PART(part)->program;
				if (program >= 0xF0u)
				{
					z80_cycles(6);
					return;
				}
				z80_block(27, 2);
				hl = instruments_ADDRESS;
				hl = read_table_word(hl, program);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(51, 6);
					voice_2 = VOICE_ADDRESS(24);
					de = 9u;
					b = 6u;
					a = screen_2_midi_status & 0xFu;
					for (;;)
					{
						z80_block(29, 3);
						if (a == VOICE(voice_2)->midi_channel)
						{
							z80_block(46, 5);
							hl_2 = a;
							if (note_code == VOICE(voice_2)->note)
							{
								z80_block(37, 5);
								flags = VOICE(voice_2)->flags;
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
							a = (uint8_t)hl_2;
						}
						z80_block(23, 3);
						voice_2 += de;
						b--;
						if (b == 0u)
						{
							z80_block(10, 1);
							return;
						}
						z80_cycles(5);
					}
					z80_block(65, 9);
					if (screen_2_channel_controls[hl_2] >> 7)
					{
						z80_block(33, 3);
						VOICE(voice_2)->flags |= 2u;
						return;
					}
					z80_block(85, 10);
					VOICE(voice_2)->flags = 0u;
					VOICE(voice_2)->midi_channel = 0u;
					value = VOICE(voice_2)->chip_channel & 7u;
					fm_register = 0x28u;
					write_fm(fm_register, value);
					return;
				}
				z80_block(51, 6);
				voice_2 = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
				a = screen_2_midi_status & 0xFu;
				for (;;)
				{
					z80_block(29, 3);
					if (a == VOICE(voice_2)->midi_channel)
					{
						z80_block(46, 5);
						hl_2 = a;
						if (note_code == VOICE(voice_2)->note)
						{
							z80_block(37, 5);
							flags = VOICE(voice_2)->flags;
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
						a = (uint8_t)hl_2;
					}
					z80_block(23, 3);
					voice_2 += de;
					b--;
					if (b == 0u)
					{
						z80_block(10, 1);
						return;
					}
					z80_cycles(5);
				}
				z80_block(65, 9);
				if (screen_2_channel_controls[hl_2] >> 7 == 0u)
				{
					z80_block(33, 3);
					VOICE(voice_2)->flags |= 1u;
					return;
				}
				z80_block(33, 3);
				VOICE(voice_2)->flags |= 2u;
				return;
			}
			z80_block(88, 10);
			de = midi_byte;
			note_level = velocity_levels[de];
			a = MIDI_PART(part)->program;
			if (a < 0xF0u)
			{
				z80_block(27, 2);
				hl = instruments_ADDRESS;
				hl = read_table_word(hl, a);
				z80_block(22, 3);
				if (((z80_read8(hl) >> 1) & 1u) != 0u)
				{
					z80_block(59, 7);
					sample_or_patch = z80_read8((uint16_t)(hl + 2u));
					voice = VOICE_ADDRESS(24);
					de = 9u;
					b = 6u;
					for (;;)
					{
						z80_block(21, 3);
						if (VOICE(voice)->flags == 0u)
							break;
						z80_block(19, 2);
						voice += de;
						b--;
						if (b == 0u)
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
					a = note_code;
					VOICE(voice - 0x2u)->note = a;
					c = a;
					voice++;
					a = screen_2_midi_status & 0xFu;
					VOICE(voice - 0x3u)->midi_channel = a;
					b = a;
					voice += 2u;
					a = note_level;
					VOICE(voice - 0x5u)->level = a;
					hl = (voice & 0xFFu) | ((uint32_t)a << 8);
					effective_volume = MIDI_PART(part)->effective_volume;
					hl = multiply_bytes(effective_volume, hl);
					z80_block(122, 16);
					note_level = 0x7Fu - ((uint16_t)((uint32_t)((hl >> 8) >> 1) << 8) >> 8);
					de = b;
					voice_modulation = (z80_read8((uint16_t)(screen_2_channel_controls_ADDRESS + de)) & 0xFu) | 0x80u;
					pitch_bends = screen_2_pitch_bends_ADDRESS;
					note = c;
					start_fm_note(note, de, pitch_bends);
					return;
				}
				for (;;)
				{
					z80_block(44, 6);
					hl++;
					if (note_number <= z80_read8(hl))
						break;
					z80_block(28, 4);
					hl += 3u;
				}
				z80_block(78, 9);
				hl++;
				sample_or_patch = z80_read8(hl);
				hl++;
				split_tune = z80_read8(hl);
				split_octave = z80_read8((uint16_t)(hl + 1u));
				take_voice_screen_2 = true;
			}
			else
			{
				z80_block(34, 3);
				a += 0x10u;
				hl = drum_kits_ADDRESS;
				hl = read_table_word(hl, a);
				z80_block(112, 14);
				de = b;
				hl = hl + de + de + de;
				drum_pitch = z80_read8(hl);
				hl++;
				c = z80_read8(hl);
				a = z80_read8((uint16_t)(hl + 1u));
				sample_or_patch = a;
				if (a == 0xFFu)
				{
					z80_cycles(6);
					return;
				}
				z80_block(37, 4);
				take_voice_screen_2 = (screen_2_midi_status & 0xFu) != 0xFu;
				if (!take_voice_screen_2)
				{
					z80_block(21, 3);
					a = b;
					if (a == 0x2Cu)
						cut_open_hi_hat_screen_2 = true;
					else
					{
						z80_block(17, 2);
						take_voice_screen_2 = a != 0x2Au;
						if (!take_voice_screen_2)
							cut_open_hi_hat_screen_2 = true;
					}
					if (!take_voice_screen_2)
					{
						if (cut_open_hi_hat_screen_2)
						{
							cut_open_hi_hat_screen_2 = false;
							z80_block(27, 3);
							voice = voice_0_note_ADDRESS;
							de = 9u;
							b = 0x18u;
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
								voice += de;
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							take_voice_screen_2 = true;
						}
					}
				}
			}
			if (take_voice_screen_2)
			{
				z80_block(33, 3);
				voice = next_voice;
				de = 9u;
				b = 0x18u;
				for (;;)
				{
					z80_block(21, 3);
					if ((uint8_t)voice == 0xE8u)
					{
						z80_cycles(7);
						voice = reset_next_voice();
					}
					z80_block(21, 3);
					a = 0u;
					if (0u == VOICE(voice)->flags)
					{
						z80_block(58, 5);
						stack_2 = voice;
						next_voice = voice + de;
						hl = stack_2;
						start_voice_screen_2 = true;
					}
					if (start_voice_screen_2)
						break;
					z80_block(19, 2);
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				if (!start_voice_screen_2)
				{
					z80_block(10, 1);
					z80_block(17, 2);
					voice = voice_0_allocation_order_ADDRESS;
					b = 0x18u;
					for (;;)
					{
						z80_block(17, 2);
						if (a == VOICE(voice - 0x6u)->allocation_order)
							break;
						z80_block(19, 2);
						voice += de;
						b--;
						if (b == 0u)
						{
							z80_block(14, 2);
							return;
						}
						z80_cycles(5);
					}
					z80_block(42, 5);
					VOICE(voice - 0x6u)->allocation_order = 0x18u;
					stack_2 = voice;
					voice = voice_0_allocation_order_ADDRESS;
					b = 0x18u;
					for (;;)
					{
						z80_block(30, 3);
						VOICE(voice - 0x6u)->allocation_order--;
						voice += de;
						b--;
						if (b == 0u)
							break;
						z80_cycles(5);
					}
					z80_block(52, 5);
					pcm_voices_used--;
					hl = stack_2 - 6u;
				}
				z80_block(46, 5);
				z80_write8(hl, 0x48u);
				if (MIDI_PART(part)->program < 0xF0u)
				{
					do
					{
						z80_block(27, 3);
					} while (z80_read8(pcm_data) & 1u);
					z80_block(479, 64);
					hl++;
					z80_write8(pcm_voice_select, z80_read8(hl));
					hl++;
					a = note_code;
					z80_write8(hl, a);
					c = a;
					hl++;
					a = screen_2_midi_status & 0xFu;
					z80_write8(hl, a);
					b = a;
					hl += 2u;
					z80_write8(hl, note_level);
					hl++;
					a = pcm_voices_used;
					z80_write8(hl, a);
					pcm_voices_used = a + 1u;
					hl++;
					z80_write8(hl, split_tune);
					z80_write8((uint16_t)(hl + 1u), split_octave);
					de = (uint8_t)(b * 2u);
					hl = screen_2_pitch_bends_ADDRESS + de;
					de = split_tune;
					stack_2 = z80_read16(hl) + de;
					hl = pcm_pitch_tables_ADDRESS + (uint8_t)((c & 0xFu) * 2u);
					pitch_entry = PCM_PITCH_RUNS(hl)[0] + stack_2 + stack_2;
					hl = PCM_PITCH_STEPS(pitch_entry)[0];
					hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((c & ~0xFu) + (hl >> 8)) << 8);
					value = split_octave + (hl >> 8);
				}
				else
				{
					z80_block(15, 2);
					z80_write8(hl, z80_read8(hl) | 0x10u);
					do
					{
						z80_block(27, 3);
					} while (z80_read8(pcm_data) & 1u);
					z80_block(171, 21);
					hl++;
					z80_write8(pcm_voice_select, z80_read8(hl));
					hl++;
					z80_write8(hl, note_code);
					hl++;
					z80_write8(hl, screen_2_midi_status & 0xFu);
					hl++;
					z80_write8(hl, c);
					hl++;
					z80_write8(hl, note_level);
					hl++;
					if ((screen_2_midi_status & 0xFu) == 0xFu)
					{
						z80_block(37, 4);
						a = pcm_voices_used;
						z80_write8(hl, a);
						pcm_voices_used = a + 1u;
					}
					else
					{
						z80_block(24, 3);
						z80_write8(hl, 0xFFu);
					}
					z80_block(30, 3);
					value = drum_pitch;
					hl = 0u;
				}
				z80_block(24, 2);
				pcm_register = 3u;
				write_pcm_register(pcm_register, value);
				z80_block(36, 5);
				value = hl | 1u;
				pcm_register = 2u;
				write_pcm_register(pcm_register, value);
				z80_block(37, 3);
				value = sample_or_patch;
				pcm_register = 1u;
				write_pcm_register(pcm_register, value);
				z80_block(53, 5);
				hl = (hl & 0xFFu) | ((uint32_t)note_level << 8);
				effective_volume = MIDI_PART(part)->effective_volume;
				hl = multiply_bytes(effective_volume, hl);
				z80_block(39, 5);
				value = ~(hl >> 8) | 1u;
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
				return;
			}
		}
		z80_block(17, 2);
		if (a == 0x80u)
		{
			z80_block(18, 3);
			if (c >> 7)
			{
				z80_block(17, 1);
				read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
				c = (uint8_t)midi_byte_copy;
			}
			z80_block(96, 11);
			note_code = note_codes[(uint8_t)(MIDI_PART(part)->transpose + c)];
			screen_2_midi_read++;
			release_note_screen_2 = true;
		}
	} while (release_note_screen_2);
	z80_block(17, 2);
	if (a == 0xE0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		b = c;
		read_midi_byte_screen_2(&ignored16, &midi_byte);
		z80_block(26, 5);
		carry = midi_byte & 1u;
		c = midi_byte >> 1;
		a = b;
		if (carry)
		{
			z80_block(7, 1);
			a |= 0x80u;
		}
		z80_block(78, 18);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		carry = c & 1u;
		c = c >> 1;
		a = (a >> 1) | (carry << 7);
		if ((c & 1u) == 0u)
		{
			z80_block(18, 3);
			t1 = ~a + 1u;
			a = t1;
			if (t1 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(57, 6);
		de = a;
		bend_range = MIDI_PART(part)->bend_range;
		hl = bend_range_tables_ADDRESS;
		hl = read_table_word(hl, bend_range);
		z80_block(36, 5);
		a = z80_read8((uint16_t)(hl + de));
		carry = c & 1u;
		c = c >> 1;
		if (carry == 0u)
		{
			z80_block(22, 4);
			c--;
			t2 = ~a + 1u;
			a = t2;
			if (t2 == 0u)
			{
				z80_block(4, 1);
				a--;
			}
		}
		z80_block(145, 19);
		b = c;
		c = a;
		pitch_bend = ((uint32_t)b << 8) | c;
		hl = screen_2_pitch_bends_ADDRESS + (uint8_t)((screen_2_midi_status & 0xFu) * 2u);
		z80_write8(hl, c);
		z80_write8((uint16_t)(hl + 1u), b);
		voice_2 = VOICE_ADDRESS(0);
		b = 0x1Eu;
		a = screen_2_midi_status & 0xFu;
		for (;;)
		{
			z80_block(29, 3);
			if (a == VOICE(voice_2)->midi_channel)
			{
				z80_block(48, 6);
				saved_a = a;
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
				a = saved_a;
			}
			z80_block(33, 4);
			voice_2 += 9u;
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xB0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(21, 2);
		b = c;
		read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
		c = (uint8_t)midi_byte_copy;
		z80_block(21, 3);
		a = b;
		carry = a == 0u;
		if (a == 1u)
		{
			z80_block(123, 19);
			t3 = c;
			a = (t3 >> 1) | (carry << 7);
			carry = t3 & 1u;
			t4 = a;
			a = (t4 >> 1) | (carry << 7);
			carry = t4 & 1u;
			t5 = a;
			a = (t5 >> 1) | (carry << 7);
			carry = t5 & 1u;
			a = ((a >> 1) | (carry << 7)) & 7u;
			c = a;
			voice_modulation = a;
			hl = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
			a = z80_read8(hl);
			b = a;
			if ((a & 0x7Fu) == c)
			{
				z80_cycles(6);
				return;
			}
			z80_block(59, 8);
			z80_write8(hl, (b & 0x80u) | c);
			voice = voice_0_midi_channel_ADDRESS;
			b = 0x1Eu;
			a = screen_2_midi_status & 0xFu;
			for (;;)
			{
				z80_block(17, 2);
				if (a == VOICE(voice - 0x3u)->midi_channel)
				{
					z80_block(47, 8);
					de = (uint32_t)a << 8;
					voice = voice - 1u - 1u - 1u;
					flags = VOICE(voice)->flags;
					if ((flags >> 3) & 1u)
					{
						z80_block(17, 2);
						if ((flags & 0x7Fu) != 0u)
						{
							z80_cycles(7);
							bc = (uint32_t)b << 8;
							set_voice_modulation_screen_2(part, &bc, &voice);
							b = (uint8_t)(bc >> 8);
						}
					}
					z80_block(22, 4);
					voice += 3u;
					a = (uint8_t)(de >> 8);
				}
				z80_block(29, 3);
				voice += 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 7u)
		{
			z80_block(18, 3);
			t6 = c * 2u;
			a = t6;
			if (t6 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(60, 7);
			MIDI_PART(part)->volume = a;
			hl = (uint32_t)a << 8;
			a = 0xFFu;
			if ((MIDI_PART(part)->flags & 1u) == 0u)
			{
				z80_block(13, 1);
				a = screen_2_music_volume;
			}
			z80_block(21, 2);
			factor = a;
			hl = multiply_bytes(factor, hl);
			z80_block(18, 3);
			a = (uint8_t)(hl >> 8);
			if (a != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(36, 4);
			MIDI_PART(part)->effective_volume = a;
			hl = VOICE_ADDRESS(0);
			b = 0x1Eu;
			for (;;)
			{
				z80_block(33, 4);
				stack_2 = hl;
				if ((z80_read8(hl) >> 6) & 1u)
				{
					z80_block(22, 3);
					if ((z80_read8(hl) >> 3) & 1u)
					{
						z80_block(55, 7);
						hl += 3u;
						if ((screen_2_midi_status & 0xFu) == z80_read8(hl))
						{
							z80_block(37, 6);
							hl = hl - 1u - 1u;
							c = z80_read8(hl);
							if (c >> 7 == 0u)
							{
								do
								{
									z80_block(27, 3);
								} while (z80_read8(pcm_data) & 1u);
								z80_block(81, 8);
								z80_write8(pcm_voice_select, c);
								hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
								effective_volume = MIDI_PART(part)->effective_volume;
								hl = multiply_bytes(effective_volume, hl);
								z80_block(39, 5);
								value = ~(hl >> 8) & 0xFEu;
								pcm_register = 5u;
								write_pcm_register(pcm_register, value);
							}
							else
							{
								z80_block(17, 1);
								chip_channel = c;
								apply_fm_channel_volume(chip_channel, hl, part);
								z80_block(10, 1);
							}
						}
					}
				}
				z80_block(39, 4);
				hl = stack_2 + 9u;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x10u)
		{
			z80_block(18, 3);
			t7 = c * 2u;
			a = t7;
			if (t7 != 0u)
			{
				z80_block(4, 1);
				a++;
			}
			z80_block(34, 4);
			screen_2_music_volume = a;
			music_part = MIDI_PART_ADDRESS(13);
			b = 8u;
			for (;;)
			{
				z80_block(67, 7);
				saved_b = b;
				hl = (hl & 0xFFu) | ((uint32_t)MIDI_PART(music_part)->volume << 8);
				a = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					a = screen_2_music_volume;
				}
				z80_block(21, 2);
				factor = a;
				hl = multiply_bytes(factor, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(36, 4);
				MIDI_PART(music_part)->effective_volume = a;
				hl = voice_0_midi_channel_ADDRESS;
				b = 0x1Eu;
				for (;;)
				{
					z80_block(51, 6);
					saved_hl = hl;
					if (MIDI_PART(music_part)->midi_channel == z80_read8(hl))
					{
						z80_block(40, 6);
						hl = hl - 1u - 1u - 1u;
						if ((z80_read8(hl) >> 6) & 1u)
						{
							z80_block(22, 3);
							if ((z80_read8(hl) >> 3) & 1u)
							{
								z80_block(31, 5);
								hl++;
								c = z80_read8(hl);
								if (c >> 7 == 0u)
								{
									do
									{
										z80_block(27, 3);
									} while (z80_read8(pcm_data) & 1u);
									z80_block(81, 8);
									z80_write8(pcm_voice_select, c);
									hl = ((hl + 4u) & 0xFFu) | ((uint32_t)z80_read8((uint16_t)(hl + 4u)) << 8);
									effective_volume = MIDI_PART(music_part)->effective_volume;
									hl = multiply_bytes(effective_volume, hl);
									z80_block(39, 5);
									value = ~(hl >> 8) & 0xFEu;
									pcm_register = 5u;
									write_pcm_register(pcm_register, value);
								}
								else
								{
									z80_block(17, 1);
									chip_channel = c;
									apply_fm_music_volume(chip_channel, hl, music_part);
									z80_block(10, 1);
								}
							}
						}
					}
					z80_block(39, 4);
					hl = saved_hl + 9u;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(43, 5);
				music_part += 0xAu;
				b = saved_b - 1u;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x40u)
		{
			z80_block(21, 3);
			if (c >= 0x40u)
			{
				z80_block(77, 9);
				hl = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
				z80_write8(hl, z80_read8(hl) | 0x80u);
				return;
			}
			z80_block(51, 6);
			voice_2 = VOICE_ADDRESS(0);
			de = 9u;
			b = 0x1Eu;
			a = screen_2_midi_status & 0xFu;
			for (;;)
			{
				z80_block(29, 3);
				if (a == VOICE(voice_2)->midi_channel)
				{
					z80_block(41, 6);
					hl_2 = a;
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
					a = (uint8_t)hl_2;
				}
				z80_block(23, 3);
				voice_2 += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(77, 9);
			hl = screen_2_channel_controls_ADDRESS + (screen_2_midi_status & 0xFu);
			z80_write8(hl, z80_read8(hl) & 0x7Fu);
			return;
		}
		z80_block(17, 2);
		if (a == 0x7Bu)
		{
			z80_block(47, 5);
			a = screen_2_midi_status & 0xFu;
			voice = VOICE_ADDRESS(0);
			de = 9u;
			b = 0x18u;
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
						if (a != VOICE(voice - 0x3u)->midi_channel)
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
				voice += de;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x50u)
		{
			z80_block(21, 3);
			if (c == 0x7Eu)
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
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
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
				z80_block(10, 1);
			}
			else
			{
				z80_block(27, 3);
				voice = VOICE_ADDRESS(0);
				de = 9u;
				b = 0x18u;
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
					voice += de;
					b--;
					if (b == 0u)
						break;
					z80_cycles(5);
				}
			}
			z80_block(17, 2);
			hl = screen_2_channel_controls_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(17, 2);
			hl = screen_2_pitch_bends_ADDRESS;
			b = 0x10u;
			for (;;)
			{
				z80_block(24, 3);
				z80_write8(hl, 0u);
				hl++;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a == 0x20u)
		{
			z80_block(31, 3);
			entry = c;
			hl = part_templates_ADDRESS;
			hl = read_table_word(hl, entry);
			z80_block(136, 19);
			move_block(hl, MIDI_PART_ADDRESS(13), 0x50u, 1u);
			hl += 0x50u;
			a = (pcm_banks & 7u) | ((((uint32_t)z80_read8(hl) << 1) << 1) << 1);
			port_out(PCM_BANKS, a);
			pcm_banks = a;
			music_part = MIDI_PART_ADDRESS(13);
			b = 8u;
			for (;;)
			{
				z80_block(56, 6);
				hl = (hl & 0xFFu) | ((uint32_t)MIDI_PART(music_part)->volume << 8);
				a = 0xFFu;
				if ((MIDI_PART(music_part)->flags & 1u) == 0u)
				{
					z80_block(13, 1);
					a = screen_2_music_volume;
				}
				z80_block(21, 2);
				factor = a;
				hl = multiply_bytes(factor, hl);
				z80_block(18, 3);
				a = (uint8_t)(hl >> 8);
				if (a != 0u)
				{
					z80_block(4, 1);
					a++;
				}
				z80_block(52, 6);
				MIDI_PART(music_part)->effective_volume = a;
				music_part += 0xAu;
				b--;
				if (b == 0u)
					break;
				z80_cycles(5);
			}
			z80_block(10, 1);
			return;
		}
		z80_block(17, 2);
		if (a != 0x51u)
		{
			z80_block(10, 1);
			return;
		}
		z80_block(60, 8);
		c = note_codes[c];
		voice = VOICE_ADDRESS(0);
		b = 0x18u;
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
					if (c != VOICE(voice - 0x2u)->note)
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
			b--;
			if (b == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(10, 1);
		return;
	}
	z80_block(17, 2);
	if (a == 0xC0u)
	{
		z80_block(18, 3);
		if (c >> 7)
		{
			z80_block(17, 1);
			read_midi_byte_screen_2(&midi_byte_copy, &ignored8);
			c = (uint8_t)midi_byte_copy;
		}
		z80_block(29, 3);
		MIDI_PART(part)->program = c;
		return;
	}
	z80_block(17, 2);
	if (a != 0xD0u)
	{
		z80_block(17, 2);
		if (a != 0xA0u)
		{
			z80_block(10, 1);
			return;
		}
	}
	first_byte = c;
	skip_midi_data_screen_2(first_byte);
}

/* The same code as retune_voice_screen_1, for screen 2's pitch bend. */
void retune_voice_screen_2(uint16_t voice)
{
	uint8_t c = 0, chip_channel = 0, note = 0, pcm_register = 0, value = 0;
	uint16_t de = 0, hl = 0, pitch_entry = 0, tune = 0;

	z80_block(37, 5);
	chip_channel = VOICE(voice)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		retune_fm_voice(voice, chip_channel);
		return;
	}
	z80_block(4, 1);
	c = chip_channel;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(287, 37);
	z80_write8(pcm_voice_select, c);
	note = VOICE(voice)->note;
	hl = pcm_pitch_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	de = pitch_bend;
	pitch_entry = PCM_PITCH_RUNS(hl)[0] + de + de;
	tune = VOICE(voice)->tune;
	pitch_entry = pitch_entry + tune + tune;
	hl = PCM_PITCH_STEPS(pitch_entry)[0];
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)((note & ~0xFu) + (hl >> 8)) << 8);
	value = VOICE(voice)->octave_shift + (hl >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(21, 3);
	value = (uint8_t)hl;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
}

/* As set_voice_modulation_screen_1 for screen 2 (FM: 80 | the depth, with the same mistake over the
   channel and the caller's count). */
void set_voice_modulation_screen_2(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice)
{
	uint16_t voices_left = (uint16_t)*p_voices_left;
	uint16_t voice = (uint16_t)*p_voice;
	uint8_t a = 0, c = 0, chip_channel = 0, value = 0;
	uint16_t bc = 0;

	z80_block(31, 5);
	voice++;
	chip_channel = VOICE(voice - 0x1u)->chip_channel;
	if (chip_channel >> 7 != 0u)
	{
		z80_block(79, 9);
		a = (uint8_t)(((voices_left & ~0xFFu) | chip_channel) >> 8);
		fm_channel = a;
		c = (a & 3u) - 0x4Cu;
		value = (voice_modulation & 0x3Fu) | 0x80u;
		bc = c;
		bc = write_fm_channel((uint8_t)bc, value);
		z80_block(16, 2);
		voice--;
		voices_left = (bc & ~0xFFu) | c;
		*p_voices_left = voices_left;
		*p_voice = voice;
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(73, 7);
	z80_write8(pcm_voice_select, chip_channel);
	c = 6u;
	value = voice_modulation | MIDI_PART(part)->lfo_frequency;
	bc = (voices_left & ~0xFFu) | 6u;
	write_pcm_register((uint8_t)bc, value);
	z80_block(16, 2);
	voice--;
	voices_left = (bc & ~0xFFu) | c;
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

/* The FM note on's chip side: the F-number of the note's semitone from fm_fnum_tables at the channel's
   pitch bend (pitch_bends[midi_channel]) and the block from its octave (A4, A0); the patch
   (load_fm_patch, or load_fm_patch_part2 for a channel of part II); the 4 operators' total levels
   (fm_level_registers): a patch level with bit 7 as it is, the others plus note_level; B4 =
   voice_modulation (the output side and the modulation depth); all four operators keyed on (28 = F0 |
   fm_channel). */
void start_fm_note(uint8_t note, uint16_t midi_channel, uint16_t pitch_bends)
{
	uint8_t a = 0, b = 0, c = 0, fm_register = 0, value = 0;
	uint16_t channel_in_part = 0, de = 0, fm_register_2 = 0, fnum_entry = 0, hl = 0;
	uint8_t saved_b = 0, saved_c = 0;
	uint16_t stack_2 = 0;

	c = (uint8_t)note;
	z80_block(242, 35);
	pitch_bends = pitch_bends + midi_channel + midi_channel;
	stack_2 = z80_read16(pitch_bends);
	hl = fm_fnum_tables_ADDRESS + (uint8_t)((c & 0xFu) * 2u);
	fnum_entry = FM_FNUMBER_RUNS(hl)[0] + stack_2 + stack_2;
	hl = FM_FNUMBERS(fnum_entry)[0];
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)(((c >> 1) & 0x38u) + (hl >> 8)) << 8);
	a = fm_channel;
	if (((a >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		de = (a & 3u) << 8;
		value = (uint8_t)(hl >> 8);
		fm_register = 0xA4u | (de >> 8);
		write_fm_part2(fm_register, value);
		z80_block(65, 9);
		value = (uint8_t)hl;
		fm_register = 0xA0u | (de >> 8);
		write_fm_part2(fm_register, value);
	}
	else
	{
		z80_block(40, 6);
		de = (uint32_t)a << 8;
		value = (uint8_t)(hl >> 8);
		fm_register = 0xA4u | (de >> 8);
		write_fm(fm_register, value);
		z80_block(65, 9);
		value = (uint8_t)hl;
		fm_register = 0xA0u | (de >> 8);
		write_fm(fm_register, value);
		z80_block(10, 1);
	}
	z80_block(31, 4);
	a = fm_channel;
	if (((a >> 2) & 1u) != 0u)
	{
		z80_block(17, 1);
		load_fm_patch_part2(a, &channel_in_part, &de, &hl);
		c = (uint8_t)channel_in_part;
	}
	else
	{
		z80_block(17, 1);
		load_fm_patch(a, &channel_in_part, &de, &hl);
		c = (uint8_t)channel_in_part;
		z80_block(10, 1);
	}
	z80_block(7, 1);
	b = 4u;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = FM_REGISTER_NUMBERS(de)[0] + c;
		de++;
		c = a;
		a = z80_read8(hl);
		hl++;
		if (a >> 7 == 0u)
		{
			z80_block(21, 3);
			a += note_level;
		}
		z80_block(17, 1);
		fm_register_2 = c;
		write_fm_channel((uint8_t)fm_register_2, a);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(61, 6);
	value = voice_modulation;
	fm_register_2 = (uint8_t)((fm_channel & 3u) - 0x4Cu);
	write_fm_channel((uint8_t)fm_register_2, value);
	z80_block(37, 4);
	value = fm_channel | 0xF0u;
	fm_register = 0x28u;
	write_fm(fm_register, value);
}

/* Retunes an FM voice after a pitch bend: fm_channel from its chip_channel, the F-number of its note
   from fm_fnum_tables at pitch_bend and the block from its octave (A4, then A0, on its part). */
void retune_fm_voice(uint16_t voice, uint8_t chip_channel)
{
	uint8_t a = 0, d = 0, fm_register = 0, note = 0, value = 0;
	uint16_t fnum_entry = 0, hl = 0;
	uint16_t t1 = 0;

	z80_block(234, 33);
	fm_channel = chip_channel & 7u;
	note = VOICE(voice)->note;
	hl = fm_fnum_tables_ADDRESS + (uint8_t)((note & 0xFu) * 2u);
	t1 = pitch_bend;
	d = (uint8_t)(t1 >> 8);
	fnum_entry = FM_FNUMBER_RUNS(hl)[0] + (((uint32_t)d << 8) | (uint8_t)t1) + (((uint32_t)d << 8) | (uint8_t)t1);
	hl = FM_FNUMBERS(fnum_entry)[0];
	hl = (hl & 0xFFu) | ((uint32_t)(uint8_t)(((note & ~0xFu) >> 1) + (hl >> 8)) << 8);
	a = fm_channel;
	if (((a >> 2) & 1u) != 0u)
	{
		z80_block(47, 7);
		d = a & 3u;
		value = (uint8_t)(hl >> 8);
		fm_register = 0xA4u | d;
		write_fm_part2(fm_register, value);
		z80_block(58, 9);
		value = (uint8_t)hl;
		fm_register = 0xA0u | d;
		write_fm_part2(fm_register, value);
		return;
	}
	z80_block(40, 6);
	d = a;
	value = (uint8_t)(hl >> 8);
	fm_register = 0xA4u | d;
	write_fm(fm_register, value);
	z80_block(58, 9);
	value = (uint8_t)hl;
	fm_register = 0xA0u | d;
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
	uint8_t a = 0, b = 0, c = 0, effective_volume = 0, entry = 0, program = 0;
	uint16_t de = 0, fm_register = 0, hl = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(103, 11);
	a = (uint8_t)chip_channel;
	fm_channel = a;
	c = a & 3u;
	voice_field = ((voice_field + 4u) & 0xFFu) | ((uint32_t)VOICE(voice_field - 0x1u)->level << 8);
	effective_volume = MIDI_PART(part)->effective_volume;
	voice_field = multiply_bytes(effective_volume, voice_field);
	z80_block(78, 9);
	note_level = 0x7Fu - ((uint16_t)((uint32_t)((voice_field >> 8) >> 1) << 8) >> 8);
	program = MIDI_PART(part)->program;
	hl = instruments_ADDRESS;
	hl = read_table_word(hl, program);
	z80_block(40, 4);
	entry = z80_read8((uint16_t)(hl + 1u));
	hl = fm_patches_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(38, 4);
	hl += 0x15u;
	b = 4u;
	de = fm_level_registers_ADDRESS;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = FM_REGISTER_NUMBERS(de)[0] + c;
		de++;
		c = a;
		a = z80_read8(hl);
		hl++;
		if (a >> 7 == 0u)
		{
			z80_block(38, 4);
			a += note_level;
			fm_register = c;
			write_fm_channel((uint8_t)fm_register, a);
		}
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(20, 2);
}

/* As apply_fm_music_volume for CC 7 (part in ix), with the same patch number mistake. */
void apply_fm_channel_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part)
{
	uint8_t a = 0, b = 0, c = 0, effective_volume = 0, entry = 0, program = 0;
	uint16_t de = 0, fm_register = 0, hl = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(103, 11);
	a = (uint8_t)chip_channel;
	fm_channel = a;
	c = a & 3u;
	voice_field = ((voice_field + 4u) & 0xFFu) | ((uint32_t)VOICE(voice_field - 0x1u)->level << 8);
	effective_volume = MIDI_PART(part)->effective_volume;
	voice_field = multiply_bytes(effective_volume, voice_field);
	z80_block(78, 9);
	note_level = 0x7Fu - ((uint16_t)((uint32_t)((voice_field >> 8) >> 1) << 8) >> 8);
	program = MIDI_PART(part)->program;
	hl = instruments_ADDRESS;
	hl = read_table_word(hl, program);
	z80_block(40, 4);
	entry = z80_read8((uint16_t)(hl + 1u));
	hl = fm_patches_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(38, 4);
	hl += 0x15u;
	b = 4u;
	de = fm_level_registers_ADDRESS;
	for (;;)
	{
		z80_block(63, 10);
		saved_b = b;
		saved_c = c;
		a = FM_REGISTER_NUMBERS(de)[0] + c;
		de++;
		c = a;
		a = z80_read8(hl);
		hl++;
		if (a >> 7 == 0u)
		{
			z80_block(38, 4);
			a += note_level;
			fm_register = c;
			write_fm_channel((uint8_t)fm_register, a);
		}
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
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
	uint8_t a = 0, b = 0, c = 0, entry = 0, fm_register = 0, value = 0;
	uint16_t channel_in_part = 0, hl = 0, register_list = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(51, 5);
	c = channel_code & 3u;
	entry = sample_or_patch;
	hl = fm_patches_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(17, 2);
	register_list = fm_patch_registers_ADDRESS;
	b = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_b = b;
		saved_c = c;
		a = FM_REGISTER_NUMBERS(register_list)[0] + c;
		register_list++;
		c = a;
		value = z80_read8(hl);
		hl++;
		fm_register = c;
		write_fm(fm_register, value);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	channel_in_part = ((uint32_t)b << 8) | c;
	*p_channel_in_part = channel_in_part;
	*p_register_list = register_list;
	*p_patch_data = hl;
}

/* As load_fm_patch on part II. */
void load_fm_patch_part2(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data)
{
	uint8_t a = 0, b = 0, c = 0, entry = 0, fm_register = 0, value = 0;
	uint16_t channel_in_part = 0, hl = 0, register_list = 0;
	uint8_t saved_b = 0, saved_c = 0;

	z80_block(51, 5);
	c = channel_code & 3u;
	entry = sample_or_patch;
	hl = fm_patches_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(17, 2);
	register_list = fm_patch_registers_ADDRESS;
	b = 0x15u;
	for (;;)
	{
		z80_block(62, 8);
		saved_b = b;
		saved_c = c;
		a = FM_REGISTER_NUMBERS(register_list)[0] + c;
		register_list++;
		c = a;
		value = z80_read8(hl);
		hl++;
		fm_register = c;
		write_fm_part2(fm_register, value);
		z80_block(18, 2);
		c = saved_c;
		b = saved_b - 1u;
		if (b == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
	channel_in_part = ((uint32_t)b << 8) | c;
	*p_channel_in_part = channel_in_part;
	*p_register_list = register_list;
	*p_patch_data = hl;
}

/* Screen 1's engine on MultiPCM voice 26 (selected as 1D), each main loop pass while
   screen_1_engine_requests is set: revs 0 key it off (when screen_1_engine_state bit 7 says it is on)
   and clear the requests; else it is keyed on first (pan 70, key_on_with_pan), then the requests are
   done and cleared: bit 6 nothing on screen 1, bit 7 the sample (set_engine_sample with
   screen_1_engine_sample), bit 5 the pitch and level for screen_1_engine_revs (set_engine_pitch with
   screen_1_engine_loudness). */
void update_engine_screen_1(void)
{
	uint8_t a = 0, d = 0, pan = 0, pcm_register = 0, sample = 0, value = 0;
	uint16_t hl = 0, revs_and_loudness = 0, screen_page = 0;

	z80_block(22, 3);
	if (screen_1_engine_requests == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(10, 1);
	hl = pcm_data;
	do
	{
		z80_block(21, 3);
	} while (z80_read8(hl) & 1u);
	z80_block(51, 6);
	z80_write8(pcm_voice_select, 0x1Du);
	a = screen_1_engine_revs;
	d = a;
	if (a == 0u)
	{
		z80_block(26, 4);
		a = screen_1_engine_state;
		if (a >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		screen_1_engine_state = a & 0x7Fu;
		value = 0u;
		pcm_register = 4u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 4);
		screen_1_engine_requests = 0u;
		return;
	}
	z80_block(31, 4);
	a = screen_1_engine_state;
	if (a >> 7 == 0u)
	{
		z80_block(45, 5);
		screen_1_engine_state = a | 0x80u;
		pan = 0x70u;
		key_on_with_pan(pan);
	}
	z80_block(32, 4);
	screen_page = screen_1_engine_requests_ADDRESS;
	if ((screen_1_engine_requests >> 6) & 1u)
	{
		z80_block(15, 2);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0xBFu;
	}
	z80_block(22, 3);
	if (SCREEN_PAGE(screen_page - 0x12u)->engine_requests >> 7)
	{
		z80_block(45, 4);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0x7Fu;
		sample = screen_1_engine_sample;
		set_engine_sample(sample);
	}
	z80_block(22, 3);
	if (((SCREEN_PAGE(screen_page - 0x12u)->engine_requests >> 5) & 1u) != 0u)
	{
		z80_block(62, 7);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0xDFu;
		screen_1_engine_requests = SCREEN_PAGE(screen_page - 0x12u)->engine_requests;
		revs_and_loudness = ((uint32_t)d << 8) | screen_1_engine_loudness;
		set_engine_pitch(revs_and_loudness);
		return;
	}
	z80_block(10, 1);
}

/* As update_engine_screen_1 for screen 2 on voice 27 (1E), pan 90; its bit 6 sets the level to
   screen_2_engine_loudness x 2. */
void update_engine_screen_2(void)
{
	uint8_t a = 0, d = 0, pan = 0, pcm_register = 0, sample = 0, value = 0;
	uint16_t revs_and_loudness = 0, screen_page = 0;

	z80_block(22, 3);
	if (screen_2_engine_requests == 0u)
	{
		z80_cycles(6);
		return;
	}
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(51, 6);
	z80_write8(pcm_voice_select, 0x1Eu);
	a = screen_2_engine_revs;
	d = a;
	if (a == 0u)
	{
		z80_block(26, 4);
		a = screen_2_engine_state;
		if (a >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		screen_2_engine_state = a & 0x7Fu;
		value = 0u;
		pcm_register = 4u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 4);
		screen_2_engine_requests = 0u;
		return;
	}
	z80_block(31, 4);
	a = screen_2_engine_state;
	if (a >> 7 == 0u)
	{
		z80_block(45, 5);
		screen_2_engine_state = a | 0x80u;
		pan = 0x90u;
		key_on_with_pan(pan);
	}
	z80_block(32, 4);
	screen_page = screen_2_engine_requests_ADDRESS;
	if ((screen_2_engine_requests >> 6) & 1u)
	{
		z80_block(60, 7);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0xBFu;
		value = (uint32_t)screen_2_engine_loudness << 1;
		pcm_register = 5u;
		write_pcm_register(pcm_register, value);
	}
	z80_block(22, 3);
	if (SCREEN_PAGE(screen_page - 0x12u)->engine_requests >> 7)
	{
		z80_block(45, 4);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0x7Fu;
		sample = screen_2_engine_sample;
		set_engine_sample(sample);
	}
	z80_block(22, 3);
	if (((SCREEN_PAGE(screen_page - 0x12u)->engine_requests >> 5) & 1u) != 0u)
	{
		z80_block(52, 6);
		SCREEN_PAGE(screen_page - 0x12u)->engine_requests &= 0xDFu;
		screen_2_engine_requests = SCREEN_PAGE(screen_page - 0x12u)->engine_requests;
		revs_and_loudness = ((uint32_t)d << 8) | screen_2_engine_loudness;
		set_engine_pitch(revs_and_loudness);
		return;
	}
	z80_block(10, 1);
}

/* Sets the selected MultiPCM voice's pan (register 0) and keys it on. */
void key_on_with_pan(uint8_t pan)
{
	uint8_t pcm_register = 0, value = 0;

	z80_block(28, 3);
	value = (uint8_t)pan;
	pcm_register = 0u;
	write_pcm_register(pcm_register, value);
	z80_block(31, 3);
	value = 0x80u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* The engine's sound for the revs (high byte of revs_and_loudness): the pitch engine_pitches[revs]
   (registers 3 and 2), with a random amount (the refresh register) added where its octave is negative
   (revs below A2); the level engine_levels[revs] - the loudness (low byte); a random LFO (register 6)
   and tremolo depth 1 (register 0A). */
void set_engine_pitch(uint16_t revs_and_loudness)
{
	uint8_t a = 0, b = 0, c = 0, entry = 0, pcm_register = 0, value = 0;
	uint16_t hl = 0;

	z80_block(31, 3);
	entry = (uint8_t)(revs_and_loudness >> 8);
	hl = engine_pitches_ADDRESS;
	hl = read_table_word(hl, entry);
	z80_block(18, 3);
	if ((int8_t)(hl >> 8) < 0)
	{
		z80_block(27, 5);
		a = refresh_read(2);
		c = a;
		if ((int8_t)a < 0)
		{
			z80_block(7, 1);
			b = 0xFFu;
		}
		else
		{
			z80_block(17, 2);
			b = 0u;
		}
		z80_block(11, 1);
		hl += ((uint32_t)b << 8) | c;
	}
	z80_block(35, 4);
	value = hl & 0xFCu;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(28, 3);
	value = (uint8_t)(hl >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(67, 8);
	value = engine_levels[revs_and_loudness >> 8] - revs_and_loudness;
	pcm_register = 5u;
	write_pcm_register(pcm_register, value);
	z80_block(40, 5);
	a = refresh_read(2);
	a &= 0x3Fu;
	pcm_register = 6u;
	write_pcm_register(pcm_register, a);
	z80_block(31, 3);
	value = 1u;
	pcm_register = 0xAu;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* Gives the selected MultiPCM voice its sample (register 1, which loads the sample's envelope and LFO),
   register 2 cleared first so that it is one of the first 256. */
void set_engine_sample(uint8_t sample)
{
	uint8_t b = 0, pcm_register = 0, value = 0;

	z80_block(32, 4);
	b = sample;
	value = 0u;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(28, 3);
	value = b;
	pcm_register = 1u;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* On every timer tick: while engine_sweep_on is set, every 65 ticks screen 1's engine plays the next
   revs of engine_sweep_revs (screen_1_engine_revs with a pitch request), up the table, then back down
   (engine_sweep_down turns at its ends). Nothing sets engine_sweep_on, so in the game it only turns
   engine_sweep_down and the position over every tick. */
void engine_sweep_tick(void)
{
	uint8_t a = 0, h = 0, l = 0;
	uint16_t t1 = 0, t2 = 0;

	z80_block(27, 3);
	if (engine_sweep_on == 0u)
	{
		z80_block(27, 3);
		if (engine_sweep_down == 0u)
		{
			z80_block(74, 8);
			engine_sweep_position_low = 0xC5u;
			engine_sweep_position_high = 0x3Du;
			engine_sweep_down = 0xFFu;
			return;
		}
		z80_block(71, 8);
		engine_sweep_position_low = 0x5Du;
		engine_sweep_position_high = 0x3Bu;
		engine_sweep_down = 0u;
		return;
	}
	z80_block(27, 3);
	a = engine_sweep_wait;
	if (a != 0u)
	{
		z80_block(27, 3);
		engine_sweep_wait = a - 1u;
		return;
	}
	z80_block(81, 9);
	engine_sweep_wait = 0x40u;
	l = engine_sweep_position_low;
	h = engine_sweep_position_high;
	if (engine_sweep_down != 0u)
	{
		z80_block(6, 1);
		t1 = (((uint32_t)h << 8) | l) - 1u;
		h = (uint8_t)(t1 >> 8);
		l = (uint8_t)t1;
	}
	else
	{
		z80_block(16, 2);
		t2 = (((uint32_t)h << 8) | l) + 1u;
		h = (uint8_t)(t2 >> 8);
		l = (uint8_t)t2;
	}
	z80_block(55, 7);
	engine_sweep_position_low = l;
	engine_sweep_position_high = h;
	a = z80_read8((uint16_t)(((uint32_t)h << 8) | l));
	if (a != 0u)
	{
		z80_block(43, 4);
		screen_1_engine_revs = a;
		screen_1_engine_requests = 0x20u;
		return;
	}
	z80_block(27, 3);
	if (engine_sweep_down == 0u)
	{
		z80_block(74, 8);
		engine_sweep_position_low = 0xC5u;
		engine_sweep_position_high = 0x3Du;
		engine_sweep_down = 0xFFu;
		return;
	}
	z80_block(71, 8);
	engine_sweep_position_low = 0x5Du;
	engine_sweep_position_high = 0x3Bu;
	engine_sweep_down = 0u;
}

/* Screen 1's tyre noise on MultiPCM voice 24 (selected as 1B), each main loop pass: a request
   (screen_1_tyre_noise, 1-8) is taken and cleared: screen_1_tyre_noise_time = the noise's duration and,
   unless its sample plays already (screen_1_tyre_noise_sample), start_tyre_noise with pan 70; without
   one, unless the noise is off (screen_1_tyre_noise_off), the time counts down by one a pass and at 0
   the noise is keyed off. */
void update_tyre_noise_screen_1(void)
{
	uint8_t a = 0, pan = 0, pcm_register = 0, value = 0;
	uint16_t noise = 0, page = 0;

	z80_block(27, 3);
	a = screen_1_tyre_noise;
	if (a != 0u)
	{
		z80_block(172, 24);
		page = SCREEN_PAGE_ADDRESS(0);
		noise = tyre_noises_ADDRESS + (uint8_t)((a - 1u) * 2u * 2u);
		screen_1_tyre_noise = 0u;
		screen_1_tyre_noise_time = TYRE_NOISE(noise)->duration;
		if (TYRE_NOISE(noise)->sample == screen_1_tyre_noise_sample)
		{
			z80_cycles(6);
			return;
		}
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(37, 4);
		z80_write8(pcm_voice_select, 0x1Bu);
		pan = 0x70u;
		start_tyre_noise(pan, page, noise);
		return;
	}
	z80_block(42, 6);
	page = SCREEN_PAGE_ADDRESS(0);
	if (screen_1_tyre_noise_off != 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 3);
	a = screen_1_tyre_noise_time;
	if (a != 0u)
	{
		z80_block(27, 3);
		screen_1_tyre_noise_time = a - 1u;
		return;
	}
	z80_block(49, 6);
	SCREEN_PAGE(page)->tyre_noise_sample = 0u;
	SCREEN_PAGE(page)->tyre_noise_off = 0xFFu;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(51, 5);
	z80_write8(pcm_voice_select, 0x1Bu);
	value = 0u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* As update_tyre_noise_screen_1 for screen 2 on voice 25 (1C), pan 90. */
void update_tyre_noise_screen_2(void)
{
	uint8_t a = 0, pan = 0, pcm_register = 0, value = 0;
	uint16_t noise = 0, page = 0;

	z80_block(27, 3);
	a = screen_2_tyre_noise;
	if (a != 0u)
	{
		z80_block(172, 24);
		page = SCREEN_PAGE_ADDRESS(1);
		noise = tyre_noises_ADDRESS + (uint8_t)((a - 1u) * 2u * 2u);
		screen_2_tyre_noise = 0u;
		screen_2_tyre_noise_time = TYRE_NOISE(noise)->duration;
		if (TYRE_NOISE(noise)->sample == screen_2_tyre_noise_sample)
		{
			z80_cycles(6);
			return;
		}
		do
		{
			z80_block(27, 3);
		} while (z80_read8(pcm_data) & 1u);
		z80_block(37, 4);
		z80_write8(pcm_voice_select, 0x1Cu);
		pan = 0x90u;
		start_tyre_noise(pan, page, noise);
		return;
	}
	z80_block(42, 6);
	page = SCREEN_PAGE_ADDRESS(1);
	if (screen_2_tyre_noise_off != 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(27, 3);
	a = screen_2_tyre_noise_time;
	if (a != 0u)
	{
		z80_block(27, 3);
		screen_2_tyre_noise_time = a - 1u;
		return;
	}
	z80_block(49, 6);
	SCREEN_PAGE(page)->tyre_noise_sample = 0u;
	SCREEN_PAGE(page)->tyre_noise_off = 0xFFu;
	do
	{
		z80_block(27, 3);
	} while (z80_read8(pcm_data) & 1u);
	z80_block(51, 5);
	z80_write8(pcm_voice_select, 0x1Cu);
	value = 0u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* Starts a tyre noise on the selected MultiPCM voice: the page's tyre_noise_off cleared, the noise's
   sample (register 1, also into tyre_noise_sample), level, pan, pitch (register 3) and key on. */
void start_tyre_noise(uint8_t pan, uint16_t page, uint16_t noise)
{
	uint8_t level = 0, pcm_register = 0, pitch = 0, sample = 0, value = 0;

	z80_block(85, 9);
	SCREEN_PAGE(page)->tyre_noise_off = 0u;
	sample = TYRE_NOISE(noise)->sample;
	SCREEN_PAGE(page)->tyre_noise_sample = sample;
	pcm_register = 1u;
	write_pcm_register(pcm_register, sample);
	z80_block(43, 4);
	level = TYRE_NOISE(noise)->level;
	pcm_register = 5u;
	write_pcm_register(pcm_register, level);
	z80_block(28, 3);
	value = (uint8_t)pan;
	pcm_register = 0u;
	write_pcm_register(pcm_register, value);
	z80_block(43, 4);
	pitch = TYRE_NOISE(noise)->pitch;
	pcm_register = 3u;
	write_pcm_register(pcm_register, pitch);
	z80_block(31, 3);
	value = 0x80u;
	pcm_register = 4u;
	write_pcm_register(pcm_register, value);
	z80_block(10, 1);
}

/* The sequence players' step, on every timer tick: screen_1_tick_countdown runs 4 to 1 and picks a
   group, each stepped once in four ticks (step_player): at its wrap screen 2's seven effect players
   (fade_player_volume on the first of them before), at 3 screen 2's music player (fade_music_volume
   before), at 2 screen 1's seven effect players, at 1 screen 1's music player; a group waits while its
   screen's MIDI queue holds 64 messages or more. */
void sequence_tick(void)
{
	uint8_t e = 0;
	uint16_t de = 0, hl = 0, page = 0, player_address = 0, queue_pointer = 0, screen_page = 0;
	uint8_t t1 = 0;

	z80_block(31, 3);
	screen_page = screen_1_tick_countdown_ADDRESS;
	t1 = screen_1_tick_countdown;
	screen_1_tick_countdown = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(55, 7);
		SCREEN_PAGE(screen_page - 0x40u)->tick_countdown = 4u;
		page = SCREEN_PAGE_ADDRESS(1);
		if (screen_2_midi_count >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = SEQUENCE_PLAYER_ADDRESS(9);
		de = 0xE200u | screen_2_midi_write;
		de = fade_player_volume(de, hl, page);
		e = (uint8_t)de;
		z80_block(24, 2);
		de = (de & ~0xFFu) | e;
		de = step_player(de, hl, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(10);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(11);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(12);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(13);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(14);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(15);
		queue_pointer = (de & ~0xFFu) | e;
		step_player(queue_pointer, player_address, page);
		z80_block(10, 1);
		return;
	}
	z80_block(24, 3);
	if (3u == SCREEN_PAGE(screen_page - 0x40u)->tick_countdown)
	{
		z80_block(45, 6);
		page = SCREEN_PAGE_ADDRESS(1);
		if (screen_2_midi_count >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = SEQUENCE_PLAYER_ADDRESS(8);
		de = 0xE200u | screen_2_midi_write;
		de = fade_music_volume(de, hl, page);
		e = (uint8_t)de;
		z80_block(24, 2);
		queue_pointer = (de & ~0xFFu) | e;
		step_player(queue_pointer, hl, page);
		z80_block(10, 1);
		return;
	}
	z80_block(24, 3);
	if (2u == SCREEN_PAGE(screen_page - 0x40u)->tick_countdown)
	{
		z80_block(45, 6);
		page = SCREEN_PAGE_ADDRESS(0);
		if (screen_1_midi_count >= 0x40u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(54, 5);
		hl = SEQUENCE_PLAYER_ADDRESS(1);
		de = 0xE100u | screen_1_midi_write;
		de = fade_player_volume(de, hl, page);
		e = (uint8_t)de;
		z80_block(24, 2);
		de = (de & ~0xFFu) | e;
		de = step_player(de, hl, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(2);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(3);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(4);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(5);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(6);
		de = (de & ~0xFFu) | e;
		de = step_player(de, player_address, page);
		e = (uint8_t)de;
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(7);
		queue_pointer = (de & ~0xFFu) | e;
		step_player(queue_pointer, player_address, page);
		z80_block(10, 1);
		return;
	}
	z80_block(19, 3);
	if (1u != SCREEN_PAGE(screen_page - 0x40u)->tick_countdown)
	{
		z80_cycles(6);
		return;
	}
	z80_block(45, 6);
	page = SCREEN_PAGE_ADDRESS(0);
	if (screen_1_midi_count >= 0x40u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(54, 5);
	hl = SEQUENCE_PLAYER_ADDRESS(0);
	de = 0xE100u | screen_1_midi_write;
	de = fade_music_volume(de, hl, page);
	e = (uint8_t)de;
	z80_block(24, 2);
	queue_pointer = (de & ~0xFFu) | e;
	step_player(queue_pointer, hl, page);
	z80_block(10, 1);
}

/* A playlist byte F0-FF, through playlist_command_table by its low nibble: F0 (and F3-FD) jumps to the
   playlist address that follows and hands back the byte there (the next song); F1 and F2 hand back song
   90 + (playlist_choice_1 or playlist_choice_2 & 7); FE clears the player (it stops); FF clears all 8 of
   the screen's players (the screen by the queue's page) and hands back 0, which stops it. */
void playlist_command(uint16_t queue_pointer, uint16_t player, uint16_t page, uint16_t *p_playlist, uint8_t *p_playlist_byte)
{
	uint16_t playlist = (uint16_t)*p_playlist;
	uint8_t playlist_byte = (uint8_t)*p_playlist_byte;
	uint16_t hl = 0;
	uint16_t saved_playlist = 0, saved_player = 0;

	z80_block(45, 4);
	saved_playlist = playlist;
	playlist_byte &= 0xFu;
	hl = playlist_command_table_ADDRESS;
	hl = read_table_word(hl, playlist_byte);
	z80_block(4, 1);
	switch (hl)
	{
	case 0x003FCCu:
		z80_block(57, 8);
		playlist = z80_read16(saved_playlist);
		playlist_byte = z80_read8(playlist);
		playlist++;
		*p_playlist = playlist;
		*p_playlist_byte = playlist_byte;
		return;
	case 0x003FD4u:
		z80_block(53, 6);
		playlist = saved_playlist;
		playlist_byte = (SCREEN_PAGE(page)->playlist_choice_1 & 7u) | 0x90u;
		*p_playlist = playlist;
		*p_playlist_byte = playlist_byte;
		return;
	case 0x003FDDu:
		z80_block(53, 6);
		playlist = saved_playlist;
		playlist_byte = (SCREEN_PAGE(page)->playlist_choice_2 & 7u) | 0x90u;
		*p_playlist = playlist;
		*p_playlist_byte = playlist_byte;
		return;
	case 0x003FE6u:
		break;
	case 0x003FF7u:
		z80_block(32, 4);
		if (queue_pointer >> 8 == 0xE2u)
		{
			z80_block(57, 7);
			playlist_byte = 0u;
			screen_2_music_flags = 0u;
			move_block(SEQUENCE_PLAYER_ADDRESS(8), 0xF101u, 0x7FFu, 1u);
		}
		else
		{
			z80_block(67, 8);
			playlist_byte = 0u;
			screen_1_music_flags = 0u;
			move_block(SEQUENCE_PLAYER_ADDRESS(0), 0xE901u, 0x7FFu, 1u);
		}
		z80_block(30, 3);
		playlist = saved_playlist;
		*p_playlist = playlist;
		*p_playlist_byte = playlist_byte;
		return;
	default:
		sound_invalid_jump(hl);
	}
	z80_block(117, 15);
	saved_player = player;
	hl = saved_player;
	playlist_byte = 0u;
	z80_write8(hl, 0u);
	move_block(hl, (uint16_t)(hl + 1u), 0x30u, 1u);
	playlist = saved_playlist;
	*p_playlist = playlist;
	*p_playlist_byte = playlist_byte;
}

/* Steps a playing sequence player (flags bit 7): its delta time (delta_time_high, delta_time_low) counts
   down, and at 0 the events from position on, in its bank (ROM_BANK), go into the screen's MIDI queue at
   queue_pointer, one more in the page's midi_count for each: a status byte is kept in running_status (a
   data byte reuses it); 80 note off with velocity 7F; B0 control change, controller 10's value kept in
   fade_volume or, while a fade runs, replaced by it; C0 and D0 two bytes; 90, A0 and E0 three. FF meta
   events: 2F ends the sequence, 51 its value into meta_51_value, the others are skipped; F0 and F7
   system exclusive are skipped but counted as a message; F1-FE end the sequence. After an event a data
   byte with bit 7 (chain_next_event) or a delta of 0 reads the next one at once; else the delta (one
   byte, or two with bit 7: 7 bits each) waits. At the end a player with a playlist (flags bit 5) starts
   its next playlist entry at once (playlist_command for F0-FF; start_song, or start_sound with flags bit
   4); 0, or no playlist, stops it. */
uint16_t step_player(uint16_t queue_pointer, uint16_t player_address, uint16_t page)
{
	uint32_t carry = 0;
	uint8_t a = 0, b = 0, c = 0, delta_time_low = 0, e = 0;
	uint16_t de = 0, hl = 0, player = 0, player_address_2 = 0, sequence_player = 0;
	uint8_t stack_10001 = 0, saved_e = 0;
	uint16_t stack_2 = 0, saved_player = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	e = (uint8_t)queue_pointer;
	z80_block(17, 3);
	if (SEQUENCE_PLAYER(player_address)->flags >> 7 == 0u)
	{
		z80_cycles(6);
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(49, 8);
	sequence_player = (player_address & ~0xFFu) | 7u;
	delta_time_low = SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low;
	sequence_player = (sequence_player & ~0xFFu) | (uint8_t)(sequence_player - 1u);
	t1 = (((uint32_t)z80_read8(sequence_player) << 8) | delta_time_low) - 1u;
	b = (uint8_t)(t1 >> 8);
	c = (uint8_t)t1;
	a = 0u;
	if (0u != c)
	{
		z80_block(35, 5);
		hl = (sequence_player & ~0xFFu) | 7u;
		z80_write8(hl, c);
		z80_write8((hl & ~0xFFu) | (uint8_t)(hl - 1u), b);
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(14, 2);
	if (a == b)
	{
		z80_block(32, 4);
		stack_2 = sequence_player & ~0xFFu;
		player = stack_2;
		for (;;)
		{
			z80_block(68, 7);
			port_out(ROM_BANK, SEQUENCE_PLAYER(player)->bank);
			hl = SEQUENCE_PLAYER(player)->position;
			for (;;)
			{
				z80_block(48, 6);
				SEQUENCE_PLAYER(player)->flags &= 0xBFu;
				a = z80_read8(hl);
				if (a >> 7 == 0u)
				{
					z80_block(35, 4);
					a = SEQUENCE_PLAYER(player)->running_status;
					hl--;
				}
				else
				{
					z80_block(19, 2);
					SEQUENCE_PLAYER(player)->running_status = a;
				}
				z80_block(17, 2);
				if (a == 0xFFu)
				{
					z80_block(17, 1);
					hl = next_sequence_byte(hl, player);
					z80_block(24, 3);
					a = z80_read8(hl);
					if (a == 0x2Fu)
						break;
					z80_block(17, 2);
					if (a == 0x51u)
					{
						z80_block(17, 1);
						hl = next_sequence_byte(hl, player);
						z80_block(17, 1);
						hl = next_sequence_byte(hl, player);
						z80_block(25, 4);
						a = z80_read8(hl);
						if (a >> 7)
						{
							z80_block(23, 5);
							a = (0u - a) | 0x80u;
						}
						z80_block(36, 3);
						SEQUENCE_PLAYER(player)->meta_51_value = a;
						hl = next_sequence_byte(hl, player);
						z80_block(10, 1);
					}
					else
					{
						z80_block(17, 1);
						hl = next_sequence_byte(hl, player);
						z80_block(7, 1);
						b = z80_read8(hl);
						for (;;)
						{
							z80_block(17, 1);
							hl = next_sequence_byte(hl, player);
							z80_block(8, 1);
							b--;
							if (b == 0u)
								break;
							z80_cycles(5);
						}
						z80_block(17, 1);
						hl = next_sequence_byte(hl, player);
						z80_block(10, 1);
					}
				}
				else
				{
					z80_block(28, 4);
					c = a;
					a &= ~0xFu;
					if (a == 0xF0u)
					{
						z80_block(21, 2);
						a = c;
						hl = next_sequence_byte(hl, player);
						z80_block(17, 2);
						if (a == 0xF7u)
						{
							z80_block(7, 1);
							b = z80_read8(hl);
							for (;;)
							{
								z80_block(24, 2);
								hl = next_sequence_byte(hl, player);
								z80_block(8, 1);
								b--;
								if (b == 0u)
									break;
								z80_cycles(5);
							}
							z80_block(10, 1);
							z80_block(33, 3);
							SCREEN_PAGE(page)->midi_count++;
						}
						else
						{
							z80_block(17, 2);
							if (a != 0xF0u)
							{
								z80_block(10, 1);
								break;
							}
							for (;;)
							{
								z80_block(24, 2);
								a = z80_read8(hl);
								hl = next_sequence_byte(hl, player);
								z80_block(17, 2);
								if (a == 0xF7u)
									break;
								z80_block(10, 1);
							}
							z80_block(33, 3);
							SCREEN_PAGE(page)->midi_count++;
						}
					}
					else
					{
						z80_block(17, 2);
						if (a == 0x80u)
						{
							z80_block(32, 4);
							z80_write8((queue_pointer & ~0xFFu) | e, c);
							e++;
							hl = next_sequence_byte(hl, player);
							z80_block(25, 4);
							a = z80_read8(hl);
							if (a >> 7 != 0u)
							{
								z80_cycles(7);
								a = chain_next_event(player, a);
							}
							z80_block(28, 3);
							z80_write8((queue_pointer & ~0xFFu) | e, a);
							e++;
							hl = next_sequence_byte(hl, player);
							z80_block(51, 6);
							z80_write8((queue_pointer & ~0xFFu) | e, 0x7Fu);
							e++;
							SCREEN_PAGE(page)->midi_count++;
						}
						else
						{
							z80_block(17, 2);
							if (a == 0xB0u)
							{
								z80_block(32, 4);
								z80_write8((queue_pointer & ~0xFFu) | e, c);
								e++;
								hl = next_sequence_byte(hl, player);
								z80_block(35, 4);
								a = z80_read8(hl);
								z80_write8((queue_pointer & ~0xFFu) | e, a);
								e++;
								hl = next_sequence_byte(hl, player);
								z80_block(24, 3);
								t2 = a;
								a = z80_read8(hl);
								if (t2 == 0x10u)
								{
									z80_block(37, 5);
									c = a;
									if (SEQUENCE_PLAYER(player)->fade_speed == 0u)
									{
										z80_block(31, 5);
										a = c;
										SEQUENCE_PLAYER(player)->fade_volume = c & 0x7Fu;
									}
									else
									{
										z80_block(29, 3);
										a = SEQUENCE_PLAYER(player)->fade_volume;
									}
								}
								z80_block(18, 3);
								if (a >> 7 != 0u)
								{
									z80_cycles(7);
									a = chain_next_event(player, a);
								}
								z80_block(28, 3);
								z80_write8((queue_pointer & ~0xFFu) | e, a);
								e++;
								hl = next_sequence_byte(hl, player);
								z80_block(33, 3);
								SCREEN_PAGE(page)->midi_count++;
							}
							else
							{
								z80_block(17, 2);
								if (a != 0xC0u)
								{
									z80_block(17, 2);
									if (a != 0xD0u)
									{
										z80_block(10, 1);
										z80_block(32, 4);
										z80_write8((queue_pointer & ~0xFFu) | e, c);
										e++;
										hl = next_sequence_byte(hl, player);
										z80_block(7, 1);
										c = z80_read8(hl);
									}
								}
								z80_block(32, 4);
								z80_write8((queue_pointer & ~0xFFu) | e, c);
								e++;
								hl = next_sequence_byte(hl, player);
								z80_block(25, 4);
								a = z80_read8(hl);
								if (a >> 7 != 0u)
								{
									z80_cycles(7);
									a = chain_next_event(player, a);
								}
								z80_block(28, 3);
								z80_write8((queue_pointer & ~0xFFu) | e, a);
								e++;
								hl = next_sequence_byte(hl, player);
								z80_block(33, 3);
								SCREEN_PAGE(page)->midi_count++;
							}
						}
					}
				}
				z80_block(30, 3);
				if (((SEQUENCE_PLAYER(player)->flags >> 6) & 1u) != 0u)
					continue;
				z80_block(24, 2);
				a = z80_read8(hl);
				hl = next_sequence_byte(hl, player);
				z80_block(14, 2);
				if (a == 0u)
					continue;
				z80_block(18, 3);
				if (a >> 7)
				{
					z80_block(37, 7);
					a &= 0x7Fu;
					carry = a & 1u;
					c = a >> 1;
					a = z80_read8(hl);
					if (carry)
					{
						z80_block(8, 2);
						a |= 0x80u;
					}
					z80_block(17, 1);
					hl = next_sequence_byte(hl, player);
					z80_block(19, 2);
					SEQUENCE_PLAYER(player)->delta_time_high = c;
				}
				z80_block(86, 9);
				SEQUENCE_PLAYER(player)->delta_time_low = a;
				SEQUENCE_PLAYER(player)->position = hl;
				SCREEN_PAGE(page)->midi_write = e;
				queue_pointer = (queue_pointer & ~0xFFu) | e;
				return queue_pointer;
			}
			z80_block(30, 3);
			if (((SEQUENCE_PLAYER(player)->flags >> 5) & 1u) == 0u)
			{
				z80_block(30, 3);
				if (((SEQUENCE_PLAYER(player)->flags >> 4) & 1u) != 0u)
				{
					z80_cycles(7);
					clear_player_end_flag(page);
				}
				z80_block(52, 6);
				SEQUENCE_PLAYER(player)->flags = 0u;
				SCREEN_PAGE(page)->midi_write = e;
				queue_pointer = (queue_pointer & ~0xFFu) | e;
				return queue_pointer;
			}
			z80_block(62, 6);
			hl = SEQUENCE_PLAYER(player)->playlist;
			a = z80_read8(hl);
			hl = next_sequence_byte(hl, player);
			z80_block(17, 2);
			if (a >= 0xF0u)
			{
				z80_cycles(7);
				playlist_command(queue_pointer, player, page, &hl, &a);
			}
			z80_block(14, 2);
			if (a == 0u)
			{
				z80_block(30, 3);
				if (((SEQUENCE_PLAYER(player)->flags >> 4) & 1u) != 0u)
				{
					z80_cycles(7);
					clear_player_end_flag(page);
				}
				z80_block(52, 6);
				SEQUENCE_PLAYER(player)->flags = 0u;
				SCREEN_PAGE(page)->midi_write = e;
				queue_pointer = (queue_pointer & ~0xFFu) | e;
				return queue_pointer;
			}
			z80_block(104, 11);
			SEQUENCE_PLAYER(player)->playlist = hl;
			stack_10001 = queue_pointer >> 8;
			saved_e = e;
			saved_player = player;
			de = saved_player;
			e = (uint8_t)saved_player;
			if (((SEQUENCE_PLAYER(player)->flags >> 4) & 1u) != 0u)
			{
				z80_block(17, 1);
				player_address_2 = (de & ~0xFFu) | e;
				start_sound(player_address_2, a);
				z80_block(20, 2);
				queue_pointer = ((uint32_t)stack_10001 << 8) | saved_e;
				e = saved_e;
				continue;
			}
			z80_block(17, 1);
			start_song(de, a);
			z80_block(20, 2);
			queue_pointer = ((uint32_t)stack_10001 << 8) | saved_e;
			e = saved_e;
		}
	}
	z80_block(35, 5);
	hl = (sequence_player & ~0xFFu) | 7u;
	z80_write8(hl, c);
	z80_write8((hl & ~0xFFu) | (uint8_t)(hl - 1u), b);
	queue_pointer = (queue_pointer & ~0xFFu) | e;
	return queue_pointer;
}

/* A data byte with bit 7 set: hands it back without the bit and sets the player's flags bit 6, so that
   the next event is read at once, with no delta time. */
uint8_t chain_next_event(uint16_t player, uint8_t data_byte)
{
	z80_block(41, 5);
	data_byte &= 0x7Fu;
	SEQUENCE_PLAYER(player)->flags |= 0x40u;
	return data_byte;
}

/* Clears the screen page's player_end_flag as a player with flags bit 4 stops; nothing reads it, nothing
   sets it. */
void clear_player_end_flag(uint16_t page)
{
	z80_block(33, 4);
	SCREEN_PAGE(page)->player_end_flag = 0u;
}

/* Each main loop pass: a sound command in screen_1_command starts from screen 1's music player, one in
   screen_2_command from screen 2's (take_sound_command). */
void poll_sound_commands(void)
{
	uint8_t a = 0;
	uint16_t page = 0, player_address = 0;

	z80_block(31, 4);
	page = SCREEN_PAGE_ADDRESS(0);
	a = screen_1_command;
	if (a != 0u)
	{
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(0);
		take_sound_command(player_address, page, a);
	}
	z80_block(31, 4);
	page = SCREEN_PAGE_ADDRESS(1);
	a = screen_2_command;
	if (a != 0u)
	{
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(8);
		take_sound_command(player_address, page, a);
	}
	z80_block(10, 1);
}

/* Clears the screen's command byte and starts the command (start_sound). */
void take_sound_command(uint16_t player_address, uint16_t page, uint8_t command)
{
	z80_block(10, 1);
	SCREEN_PAGE(page)->command = 0u;
	start_sound(player_address, command);
}

/* Starts sound command 'command' (sound_commands) from the screen's music player at player_address, by
   its header's flags: an effect (bit 7) goes to the screen's effect player two pages on or, with bit 6,
   to the channel player its first event's channel picks (select_channel_player), unless that player
   plays one of higher priority (flags bits 0-3): it is cleared, takes the header's flags and plays the
   sequence right after them; bit 6 alone plays that on the music player as it is (no command has it);
   bit 5 a sequence (start_sequence: the header's bank and address); else a playlist: the player cleared,
   flags bit 5, the playlist after its first song, which start_song starts. The game's commands: 00-1D
   and E0-FB playlists, 1E stops the music, 1F silences the screen, 20-8F an effect note on channel E,
   90-BF the channel players' effects, C0-CF sequences, D0-DB a sound set (CC 20), DC-DF and FC-FE
   channel A's and stop controls. */
void start_sound(uint16_t player_address, uint8_t command)
{
	uint8_t a = 0, song = 0;
	uint16_t de = 0, hl = 0, player = 0, sequence_player = 0;
	uint16_t t1 = 0;

	z80_block(27, 2);
	hl = sound_commands_ADDRESS;
	hl = read_table_word(hl, command);
	z80_block(29, 5);
	t1 = player_address;
	de = hl;
	sequence_player = t1;
	a = z80_read8(de);
	if (a >> 7 != 0u)
	{
		z80_block(18, 3);
		if (((a >> 6) & 1u) != 0u)
		{
			z80_block(29, 4);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			select_channel_player(&de, &sequence_player);
			z80_block(46, 7);
			a = z80_read8(de) & 0xFu;
			if (a < (z80_read8(sequence_player) & 0xFu))
			{
				z80_block(10, 1);
				return;
			}
			z80_block(17, 1);
			sequence_player = clear_player(sequence_player);
			z80_block(87, 12);
			de++;
			SEQUENCE_PLAYER(sequence_player)->flags = a;
			sequence_player = (sequence_player & ~0xFFu) | 2u;
			SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)de;
			sequence_player = (sequence_player & ~0xFFu) | (uint8_t)(sequence_player + 1u);
			SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = de >> 8;
			sequence_player = (sequence_player & ~0xFFu) | 7u;
			SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
			sequence_player &= ~0xFFu;
			SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
			return;
		}
		z80_block(10, 1);
		z80_block(54, 9);
		sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
		sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
		a = z80_read8(de) & 0xFu;
		if (a < (z80_read8(sequence_player) & 0xFu))
		{
			z80_block(10, 1);
			return;
		}
		z80_block(17, 1);
		sequence_player = clear_player(sequence_player);
		z80_block(7, 1);
		SEQUENCE_PLAYER(sequence_player)->flags = a;
		z80_block(80, 11);
		de++;
		sequence_player = (sequence_player & ~0xFFu) | 2u;
		SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)de;
		sequence_player = (sequence_player & ~0xFFu) | (uint8_t)(sequence_player + 1u);
		SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = de >> 8;
		sequence_player = (sequence_player & ~0xFFu) | 7u;
		SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
		return;
	}
	z80_block(18, 3);
	if (((a >> 6) & 1u) != 0u)
	{
		z80_block(80, 11);
		de++;
		sequence_player = (sequence_player & ~0xFFu) | 2u;
		SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)de;
		sequence_player = (sequence_player & ~0xFFu) | (uint8_t)(sequence_player + 1u);
		SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = de >> 8;
		sequence_player = (sequence_player & ~0xFFu) | 7u;
		SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
		return;
	}
	z80_block(18, 3);
	if (((a >> 5) & 1u) != 0u)
	{
		start_sequence(de, sequence_player);
		return;
	}
	z80_block(18, 3);
	if (((a >> 4) & 1u) == 0u)
		z80_block(10, 1);
	z80_block(17, 1);
	sequence_player = clear_player(sequence_player);
	z80_block(82, 12);
	de++;
	song = z80_read8(de);
	SEQUENCE_PLAYER(sequence_player)->flags |= 0x20u;
	de++;
	sequence_player = (sequence_player & ~0xFFu) | 0x20u;
	SEQUENCE_PLAYER(sequence_player - 0x20u)->playlist_low = (uint8_t)de;
	sequence_player++;
	SEQUENCE_PLAYER(sequence_player - 0x21u)->playlist_high = de >> 8;
	player = sequence_player & ~0xFFu;
	start_song(player, song);
}

/* Starts song 'song' (songs) on the player: start_sequence with its record. */
void start_song(uint16_t player, uint8_t song)
{
	uint16_t header = 0, hl = 0, player_2 = 0;
	uint16_t t1 = 0;

	z80_block(27, 2);
	hl = songs_ADDRESS;
	hl = read_table_word(hl, song);
	z80_block(21, 3);
	t1 = player;
	header = hl;
	player_2 = t1;
	start_sequence(header, player_2);
}

/* The player plays the sequence of a song record or of a sound header (its flags byte skipped): its bank
   (byte 1) and address (bytes 2-3), delta_time_low 1, flags bit 7; nothing else of the player changes. */
void start_sequence(uint16_t header, uint16_t player)
{
	uint8_t a = 0;
	uint16_t sequence_player = 0;

	z80_block(127, 18);
	header++;
	sequence_player = (player & ~0xFFu) | 0x1Bu;
	a = z80_read8(header);
	header++;
	SEQUENCE_PLAYER(sequence_player - 0x1Bu)->bank = a;
	sequence_player = (sequence_player & ~0xFFu) | 2u;
	SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = z80_read8(header);
	sequence_player = (sequence_player & ~0xFFu) | (uint8_t)(sequence_player + 1u);
	SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = z80_read8((uint16_t)(header + 1u));
	sequence_player = (sequence_player & ~0xFFu) | 7u;
	SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
	sequence_player &= ~0xFFu;
	SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
}

/* Moves player_address from the first channel player to the one of the effect's channel: as many pages
   on as the low nibble of the header's first event (its status byte, channel A-E) less 0A. */
void select_channel_player(uint16_t *p_header, uint16_t *p_player_address)
{
	uint16_t header = (uint16_t)*p_header;
	uint16_t player_address = (uint16_t)*p_player_address;
	uint8_t h = 0;

	z80_block(55, 9);
	header++;
	h = (z80_read8(header) & 0xFu) - 0xAu + (player_address >> 8);
	header--;
	player_address = (uint32_t)h << 8;
	*p_header = header;
	*p_player_address = (*p_player_address & 0xFFu) | player_address;
}

/* Clears the player's bytes 00-30 (flags to playlist_high) and hands back its address. */
uint16_t clear_player(uint16_t player)
{
	uint16_t saved_player = 0;

	z80_block(101, 11);
	saved_player = player;
	SEQUENCE_PLAYER(player)->flags = 0u;
	move_block(player, (uint16_t)(saved_player + 1u), 0x30u, 1u);
	player = (player + 0x30u) & 0xFF00u;
	return player;
}

/* The music player's fade, while its fade_speed is set (the V70's fade_out_music_at writes it): every
   fade_speed + 1 calls (fade_wait) fade_volume goes down by one and out as CC 10 on channel E (the
   screen's music volume); at 0 the player stops (flags bit 7), fade_speed is cleared, and CC 50 7E (the
   music's voices stop) and CC 79 0 (which nothing handles) go out on channel E. */
uint16_t fade_music_volume(uint16_t queue_pointer, uint16_t player, uint16_t page)
{
	uint8_t e = 0;
	uint16_t sequence_player = 0;

	e = (uint8_t)queue_pointer;
	z80_block(23, 4);
	sequence_player = (player & ~0xFFu) | 0x18u;
	if (SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed == 0u)
	{
		z80_cycles(6);
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(28, 4);
	sequence_player = (sequence_player & ~0xFFu) | 0x19u;
	if (SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait != 0u)
	{
		z80_block(21, 2);
		SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait--;
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(39, 5);
	sequence_player = (sequence_player & ~0xFFu) | 0x15u;
	SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume--;
	if (SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume == 0u)
	{
		z80_block(223, 31);
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags &= 0x7Fu;
		SEQUENCE_PLAYER(sequence_player)->fade_speed = 0u;
		z80_write8((queue_pointer & ~0xFFu) | e, 0xBEu);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0x50u);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0x7Eu);
		e++;
		SCREEN_PAGE(page)->midi_count++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0xBEu);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0x79u);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0u);
		e++;
		SCREEN_PAGE(page)->midi_count++;
		SCREEN_PAGE(page)->midi_write = e;
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(134, 18);
	z80_write8((queue_pointer & ~0xFFu) | e, 0xBEu);
	e++;
	z80_write8((queue_pointer & ~0xFFu) | e, 0x10u);
	e++;
	z80_write8((queue_pointer & ~0xFFu) | e, SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume);
	e++;
	SCREEN_PAGE(page)->midi_write = e;
	SCREEN_PAGE(page)->midi_count++;
	sequence_player = (sequence_player & ~0xFFu) | 0x18u;
	SEQUENCE_PLAYER(sequence_player & ~0xFFu)->fade_wait = SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed;
	queue_pointer = (queue_pointer & ~0xFFu) | e;
	return queue_pointer;
}

/* The same for the player after the music player, with fade_channel_volume going out as CC 7 on channel
   A; at 0 the player is cleared (flags 0) and CC 7B 0 (all notes off) goes out on channel A. Nothing
   sets that player's fade_speed, and no command starts that player. */
uint16_t fade_player_volume(uint16_t queue_pointer, uint16_t player, uint16_t page)
{
	uint8_t e = 0;
	uint16_t sequence_player = 0;

	e = (uint8_t)queue_pointer;
	z80_block(23, 4);
	sequence_player = (player & ~0xFFu) | 0x18u;
	if (SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed == 0u)
	{
		z80_cycles(6);
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(28, 4);
	sequence_player = (sequence_player & ~0xFFu) | 0x19u;
	if (SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait != 0u)
	{
		z80_block(21, 2);
		SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait--;
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(39, 5);
	sequence_player = (sequence_player & ~0xFFu) | 0x1Au;
	SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume--;
	if (SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume == 0u)
	{
		z80_block(138, 19);
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags = 0u;
		SEQUENCE_PLAYER(sequence_player)->fade_speed = 0u;
		z80_write8((queue_pointer & ~0xFFu) | e, 0xBAu);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0x7Bu);
		e++;
		z80_write8((queue_pointer & ~0xFFu) | e, 0u);
		e++;
		SCREEN_PAGE(page)->midi_count++;
		SCREEN_PAGE(page)->midi_write = e;
		queue_pointer = (queue_pointer & ~0xFFu) | e;
		return queue_pointer;
	}
	z80_block(134, 18);
	z80_write8((queue_pointer & ~0xFFu) | e, 0xBAu);
	e++;
	z80_write8((queue_pointer & ~0xFFu) | e, 7u);
	e++;
	z80_write8((queue_pointer & ~0xFFu) | e, SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume);
	e++;
	SCREEN_PAGE(page)->midi_write = e;
	SCREEN_PAGE(page)->midi_count++;
	sequence_player = (sequence_player & ~0xFFu) | 0x18u;
	SEQUENCE_PLAYER(sequence_player & ~0xFFu)->fade_wait = SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed;
	queue_pointer = (queue_pointer & ~0xFFu) | e;
	return queue_pointer;
}

/* Moves the sequence position on by one; past the bank window (C000) the player's bank goes one on
   (ROM_BANK) and the position back to A000. */
uint16_t next_sequence_byte(uint16_t position, uint16_t player)
{
	z80_block(38, 5);
	position++;
	if (position >> 8 == 0xC0u)
	{
		z80_block(60, 6);
		SEQUENCE_PLAYER(player)->bank++;
		port_out(ROM_BANK, SEQUENCE_PLAYER(player)->bank);
		position = (position & 0xFFu) | 0xA000u;
	}
	z80_block(20, 2);
	return position;
}
