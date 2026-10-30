/**
 *    Name:        engine.c
 *    Description: the engine and tyre noise of each screen's car
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* Screen 1's engine on MultiPCM voice 26 (selected as 1D), each main loop pass while
   screen_1_engine_requests is set: revs 0 key it off (when screen_1_engine_state bit 7 says it is on)
   and clear the requests; else it is keyed on first (pan 70, key_on_with_pan), then the requests are
   done and cleared: bit 6 nothing on screen 1, bit 7 the sample (set_engine_sample with
   screen_1_engine_sample), bit 5 the pitch and level for screen_1_engine_revs (set_engine_pitch with
   screen_1_engine_loudness). */
void update_engine_screen_1(void)
{
	uint8_t engine_revs = 0, engine_state = 0, pan = 0, pcm_register = 0, revs = 0, sample = 0, value = 0;
	uint16_t pcm_data_at = 0, revs_and_loudness = 0, screen_page = 0;

	z80_block(22, 3);
	if (screen_1_engine_requests == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(10, 1);
	pcm_data_at = pcm_data;
	do
	{
		z80_block(21, 3);
	} while (z80_read8(pcm_data_at) & 1u);
	z80_block(51, 6);
	z80_write8(pcm_voice_select, 0x1Du);
	engine_revs = screen_1_engine_revs;
	revs = engine_revs;
	if (engine_revs == 0u)
	{
		z80_block(26, 4);
		engine_state = screen_1_engine_state;
		if (engine_state >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		screen_1_engine_state = engine_state & 0x7Fu;
		value = 0u;
		pcm_register = 4u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 4);
		screen_1_engine_requests = 0u;
		return;
	}
	z80_block(31, 4);
	engine_state = screen_1_engine_state;
	if (engine_state >> 7 == 0u)
	{
		z80_block(45, 5);
		screen_1_engine_state = engine_state | 0x80u;
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
		revs_and_loudness = ((uint32_t)revs << 8) | screen_1_engine_loudness;
		set_engine_pitch(revs_and_loudness);
		return;
	}
	z80_block(10, 1);
}

/* As update_engine_screen_1 for screen 2 on voice 27 (1E), pan 90; its bit 6 sets the level to
   screen_2_engine_loudness x 2. */
void update_engine_screen_2(void)
{
	uint8_t engine_revs = 0, engine_state = 0, pan = 0, pcm_register = 0, revs = 0, sample = 0, value = 0;
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
	engine_revs = screen_2_engine_revs;
	revs = engine_revs;
	if (engine_revs == 0u)
	{
		z80_block(26, 4);
		engine_state = screen_2_engine_state;
		if (engine_state >> 7 == 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(52, 6);
		screen_2_engine_state = engine_state & 0x7Fu;
		value = 0u;
		pcm_register = 4u;
		write_pcm_register(pcm_register, value);
		z80_block(31, 4);
		screen_2_engine_requests = 0u;
		return;
	}
	z80_block(31, 4);
	engine_state = screen_2_engine_state;
	if (engine_state >> 7 == 0u)
	{
		z80_block(45, 5);
		screen_2_engine_state = engine_state | 0x80u;
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
		revs_and_loudness = ((uint32_t)revs << 8) | screen_2_engine_loudness;
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
	uint8_t entry = 0, pcm_register = 0, random_amount = 0, random_lfo = 0, random_sign = 0, refresh_value = 0, value = 0;
	uint16_t pitch = 0;

	z80_block(31, 3);
	entry = (uint8_t)(revs_and_loudness >> 8);
	pitch = engine_pitches_ADDRESS;
	pitch = read_table_word(pitch, entry);
	z80_block(18, 3);
	if ((int8_t)(pitch >> 8) < 0)
	{
		z80_block(27, 5);
		refresh_value = refresh_read(2);
		random_amount = refresh_value;
		if ((int8_t)refresh_value < 0)
		{
			z80_block(7, 1);
			random_sign = 0xFFu;
		}
		else
		{
			z80_block(17, 2);
			random_sign = 0u;
		}
		z80_block(11, 1);
		pitch += ((uint32_t)random_sign << 8) | random_amount;
	}
	z80_block(35, 4);
	value = pitch & 0xFCu;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(28, 3);
	value = (uint8_t)(pitch >> 8);
	pcm_register = 3u;
	write_pcm_register(pcm_register, value);
	z80_block(67, 8);
	value = engine_levels[revs_and_loudness >> 8] - revs_and_loudness;
	pcm_register = 5u;
	write_pcm_register(pcm_register, value);
	z80_block(40, 5);
	random_lfo = refresh_read(2);
	random_lfo &= 0x3Fu;
	pcm_register = 6u;
	write_pcm_register(pcm_register, random_lfo);
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
	uint8_t pcm_register = 0, saved_sample = 0, value = 0;

	z80_block(32, 4);
	saved_sample = sample;
	value = 0u;
	pcm_register = 2u;
	write_pcm_register(pcm_register, value);
	z80_block(28, 3);
	value = saved_sample;
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
	uint8_t revs = 0, sweep_high = 0, sweep_low = 0, wait_ticks = 0;
	uint16_t lower_position = 0, higher_position = 0;

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
	wait_ticks = engine_sweep_wait;
	if (wait_ticks != 0u)
	{
		z80_block(27, 3);
		engine_sweep_wait = wait_ticks - 1u;
		return;
	}
	z80_block(81, 9);
	engine_sweep_wait = 0x40u;
	sweep_low = engine_sweep_position_low;
	sweep_high = engine_sweep_position_high;
	if (engine_sweep_down != 0u)
	{
		z80_block(6, 1);
		lower_position = (((uint32_t)sweep_high << 8) | sweep_low) - 1u;
		sweep_high = (uint8_t)(lower_position >> 8);
		sweep_low = (uint8_t)lower_position;
	}
	else
	{
		z80_block(16, 2);
		higher_position = (((uint32_t)sweep_high << 8) | sweep_low) + 1u;
		sweep_high = (uint8_t)(higher_position >> 8);
		sweep_low = (uint8_t)higher_position;
	}
	z80_block(55, 7);
	engine_sweep_position_low = sweep_low;
	engine_sweep_position_high = sweep_high;
	revs = z80_read8((uint16_t)(((uint32_t)sweep_high << 8) | sweep_low));
	if (revs != 0u)
	{
		z80_block(43, 4);
		screen_1_engine_revs = revs;
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
	uint8_t noise_request = 0, pan = 0, pcm_register = 0, tyre_noise_time = 0, value = 0;
	uint16_t noise = 0, page = 0;

	z80_block(27, 3);
	noise_request = screen_1_tyre_noise;
	if (noise_request != 0u)
	{
		z80_block(172, 24);
		page = SCREEN_PAGE_ADDRESS(0);
		noise = tyre_noises_ADDRESS + (uint8_t)((noise_request - 1u) * 2u * 2u);
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
	tyre_noise_time = screen_1_tyre_noise_time;
	if (tyre_noise_time != 0u)
	{
		z80_block(27, 3);
		screen_1_tyre_noise_time = tyre_noise_time - 1u;
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
	uint8_t noise_request = 0, pan = 0, pcm_register = 0, tyre_noise_time = 0, value = 0;
	uint16_t noise = 0, page = 0;

	z80_block(27, 3);
	noise_request = screen_2_tyre_noise;
	if (noise_request != 0u)
	{
		z80_block(172, 24);
		page = SCREEN_PAGE_ADDRESS(1);
		noise = tyre_noises_ADDRESS + (uint8_t)((noise_request - 1u) * 2u * 2u);
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
	tyre_noise_time = screen_2_tyre_noise_time;
	if (tyre_noise_time != 0u)
	{
		z80_block(27, 3);
		screen_2_tyre_noise_time = tyre_noise_time - 1u;
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
