/**
 *    Name:        main.c
 *    Description: the sound program's frame: the reset, the timer interrupt, sound_init and the chips set up and written
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
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
	uint8_t saved_flags = 0, voice_flags = 0;
	uint16_t cursor = 0;
	uint8_t wait_ticks = 0, voices_left = 0, drum_timeout = 0;

	z80_block(31, 4);
	if ((drum_timeout_pass & 1u) == 0u)
	{
		z80_block(26, 3);
		wait_ticks = drum_timeout_wait;
		drum_timeout_wait = wait_ticks - 1u;
		if (wait_ticks != 1u)
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
	voices_left = drum_timeout_voices_left;
	drum_timeout_voices_left = voices_left - 1u;
	if (voices_left == 1u)
	{
		z80_block(35, 4);
		drum_timeout_pass &= ~1u;
		return;
	}
	z80_block(37, 3);
	cursor = drum_timeout_cursor;
	drum_timeout = z80_read8(cursor);
	z80_write8(cursor, drum_timeout - 1u);
	if (drum_timeout != 1u)
	{
		z80_block(47, 4);
		drum_timeout_cursor = cursor + 9u;
		return;
	}
	z80_block(49, 6);
	cursor -= 4u;
	voice_flags = z80_read8(cursor);
	saved_flags = voice_flags;
	if ((voice_flags & 0x81u) == 0u)
	{
		z80_block(28, 4);
		if ((saved_flags & 0x70u) >= 0x50u)
		{
			z80_block(15, 2);
			z80_write8(cursor, z80_read8(cursor) | 1u);
		}
	}
	z80_block(47, 4);
	drum_timeout_cursor = cursor + 0xDu;
}

/* Sets the sound up at reset: the MultiPCM (init_pcm_chip) and the YM3438 (init_fm_chip); both screens'
   MIDI queues empty (screen_1_midi_write 0, screen_1_midi_read and screen_2_midi_read FF), next_voice
   the first voice, screen_1_tick_countdown 1, drum_timeout_wait 140 ticks, both music volumes FF; the
   voices' chip channels (init_voice_channels) and both screens' parts (init_parts); last, YM3438
   register 27 = 15: timer A loaded and its interrupt on. */
void sound_init(void)
{
	uint8_t fm_register = 0, value = 0;

	z80_block(17, 1);
	init_pcm_chip();
	z80_block(17, 1);
	init_fm_chip();
	z80_block(172, 15);
	screen_1_midi_write = 0u;
	screen_1_midi_read = 0xFFu;
	screen_2_midi_read = 0xFFu;
	next_voice = VOICE_ADDRESS(0);
	screen_1_tick_countdown = 1u;
	AS16(drum_timeout_wait) = 0x8Cu;
	screen_1_music_volume = 0xFFu;
	screen_2_music_volume = 0xFFu;
	init_voice_channels();
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
   silent: total level 7F, release FF, no SSG-EG). */
void init_fm_chip(void)
{
	uint8_t fm_register = 0, pairs_left = 0, register_number = 0, value = 0;
	uint16_t init_entry = 0;

	z80_block(17, 2);
	init_entry = fm_global_init_ADDRESS;
	pairs_left = 0xBu;
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
		pairs_left--;
		if (pairs_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(17, 2);
	init_entry = fm_operator_init_ADDRESS;
	pairs_left = 0x24u;
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
		pairs_left--;
		if (pairs_left == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

/* Writes value to the YM3438 register fm_register on the part fm_channel's bit 2 picks (write_fm_part2
   when it is set, else write_fm); hands fm_register back with value in its high byte. */
uint16_t write_fm_channel(uint16_t fm_register, uint8_t value)
{
	uint8_t saved_value = 0;
	uint8_t channel_code = 0;

	z80_block(39, 6);
	saved_value = value;
	channel_code = fm_channel;
	value = saved_value;
	if (((channel_code >> 2) & 1u) != 0u)
	{
		fm_register = (fm_register & 0xFFu) | ((uint32_t)saved_value << 8);
		write_fm_part2((uint8_t)fm_register, value);
		return fm_register;
	}
	fm_register = (fm_register & 0xFFu) | ((uint32_t)saved_value << 8);
	write_fm((uint8_t)fm_register, value);
	return fm_register;
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
	uint8_t chip_code = 0, pcm_register = 0, value = 0, voices_left = 0;
	uint16_t code_entry = 0;

	z80_block(55, 6);
	z80_write8(MULTIPCM_C009, 3u);
	port_out(PCM_BANKS, 0x1Au);
	code_entry = voice_chip_channels_ADDRESS;
	voices_left = 0x1Cu;
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
		voices_left--;
		if (voices_left == 0u)
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
void init_voice_channels(void)
{
	uint8_t voices_left = 0;
	uint16_t code_entry = 0, voice = 0;
	uint8_t saved_voices_left = 0;

	z80_block(27, 3);
	voice = voice_0_chip_channel_ADDRESS;
	code_entry = voice_chip_channels_ADDRESS;
	voices_left = 0x1Eu;
	for (;;)
	{
		z80_block(70, 8);
		saved_voices_left = voices_left;
		VOICE(voice - 0x1u)->chip_channel = VOICE_CHIP_CODES(code_entry)[0];
		code_entry++;
		voice += 9u;
		voices_left = saved_voices_left - 1u;
		if (voices_left == 0u)
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
	uint8_t entry = 0, factor = 0, rounded_volume = 0, volume_scale = 0;
	uint16_t part = 0, part_template = 0, parts_left = 0, template_then_product = 0;
	uint16_t saved_part_template = 0;

	z80_block(34, 3);
	entry = 0u;
	part_template = part_templates_ADDRESS;
	part_template = read_table_word(part_template, entry);
	z80_block(178, 21);
	saved_part_template = part_template;
	move_block(part_template, MIDI_PART_ADDRESS(0), 0x50u, 1u);
	move_block((uint16_t)(part_template + 0x51u), MIDI_PART_ADDRESS(8), 0x32u, 1u);
	template_then_product = saved_part_template;
	move_block(template_then_product, MIDI_PART_ADDRESS(13), 0x50u, 1u);
	move_block((uint16_t)(template_then_product + 0x51u), MIDI_PART_ADDRESS(21), 0x32u, 1u);
	part = MIDI_PART_ADDRESS(0);
	parts_left = 0x1A00u;
	for (;;)
	{
		z80_block(56, 6);
		template_then_product = (uint32_t)MIDI_PART(part)->volume << 8;
		volume_scale = 0xFFu;
		if ((MIDI_PART(part)->flags & 1u) == 0u)
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
		MIDI_PART(part)->effective_volume = rounded_volume;
		part += 0xAu;
		parts_left = (uint32_t)(uint8_t)((parts_left >> 8) - 1u) << 8;
		if (parts_left >> 8 == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(10, 1);
}

/* Hands back the 16-bit entry 'entry' of the table of words at 'table'. */
uint16_t read_table_word(uint16_t table, uint8_t entry)
{
	uint16_t offset = 0;

	z80_block(88, 11);
	offset = entry;
	table = table + offset + offset;
	table = z80_read16(table);
	return table;
}
