/**
 *    Name:        system/sound.c
 *    Description: the sound requests to the sound program: sounds, the engine and tyre noise, the music's fade
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Starts the sound at boot: the screen's ring of sound requests with one request of sound 0 waiting (800
   the place read, 801 the place written, the requests from 802), I/O chip 0's CNT 6 (its screen on, the
   sound CPU out of reset), screen byte 822 cleared, and 701FFF cleared, which lets the sound program's
   main loop run. */
void start_sound_program(void)
{
	AS32(screen->sound_queue_read) = 0x100u;
	io_write8(IO_1_CNT, 6u);
	screen->sound_start_cleared = 0u;
	SOUND_PROGRAM_HOLD = 0u;
}

/* Requests sound R0 for the screen (queue_sound), except while the screen's demo_flags bit 8 is set,
   when only the sounds the table at 01202E (demo_sounds) allows play, and only with advertise_sound set. */
void request_sound(uint8_t sound)
{
	uint32_t sound_number = 0;

	if ((screen->demo_flags & 0x100u) == 0u)
	{
		queue_sound(sound);
		return;
	}
	if (game->advertise_sound != 0u)
	{
		sound_number = (uint8_t)sound;
		if ((demo_sounds[sound_number >> 5] & shl32(1u, (int)(sound_number & 0x1Fu))) != 0u)
		{
			queue_sound(sound);
			return;
		}
	}
}

/* Puts sound R0 in the screen's ring of 32 sound requests (801 of the screen: the place, then the
   requests). */
void queue_sound(uint8_t sound)
{
	uint32_t sound_queue_write = 0;
	struct screen *request_ring = NULL;

	request_ring = screen;
	sound_queue_write = request_ring->sound_queue_write;
	request_ring->sound_queue[(int32_t)sound_queue_write] = sound;
	request_ring->sound_queue_write = (sound_queue_write + 1u) & 0x1Fu;
}

/* Unused: puts sound R0 in the screen's second ring of 32 sound requests (second_sound_queue_write, the
   requests from 842; send_sound_requests sends them to the command byte 8 after the first ring's), as
   request_sound does for the first: while demo_flags bit 8 is set only the sounds the table at 01204E
   (demo_second_sounds) allows, and only with advertise_sound set. */
void request_second_sound(uint8_t sound)
{
	uint32_t second_sound_queue_write = 0, sound_number = 0;
	struct screen *request_ring = NULL;

	if ((screen->demo_flags & 0x100u) == 0u)
	{
		request_ring = screen;
		second_sound_queue_write = request_ring->second_sound_queue_write;
		request_ring->second_sound_queue[(int32_t)second_sound_queue_write] = sound;
		request_ring->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
	}
	else if (game->advertise_sound != 0u)
	{
		sound_number = (uint8_t)sound;
		if ((demo_second_sounds[sound_number >> 5] & shl32(1u, (int)(sound_number & 0x1Fu))) != 0u)
		{
			request_ring = screen;
			second_sound_queue_write = request_ring->second_sound_queue_write;
			request_ring->second_sound_queue[(int32_t)second_sound_queue_write] = sound;
			request_ring->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
			return;
		}
	}
}

/* Starts the car's engine sound on the screen's channel of the sound board: the model's engine sample
   (car_model_engine_samples) and the engine's loudness by the transmission (10, or 30, 6 dB louder, with
   the transmission's value set), then the revs on (1) and the requests for the sample and the pitch
   (A0); silent in the demo without sound. */
void player_car_start_engine_sound(void)
{
	uint8_t loudness = 0;
	uint32_t sound_page = 0;

	if ((screen->demo_flags & 0x100u) != 0u && game->advertise_sound == 0u)
	{
		sound_page = (uint32_t)screen->index << 8;
		SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = 0u;
		SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests |= 0x20u;
		return;
	}
	sound_page = (uint32_t)screen->index << 8;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_sample = car_model_engine_samples[player_car->model];
	loudness = 0x10u;
	if (player_car->transmission != 0u)
		loudness = 0x30u;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_loudness = loudness;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = 1u;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests = 0xA0u;
}

/* Sets the engine's pitch from the revs (car revs) each frame, or silences it in the demo without sound. */
void player_car_engine_sound(void)
{
	uint32_t sound_page = 0;

	if ((screen->demo_flags & 0x100u) == 0u || game->advertise_sound != 0u)
	{
		sound_page = (uint32_t)screen->index << 8;
		SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = player_car->revs >> 8;
		SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests = 0x20u;
		return;
	}
	sound_page = (uint32_t)screen->index << 8;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = 0u;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests |= 0x20u;
}

/* set_tyre_noise, unless the car is off the road (car off_road), where the off-road noise plays. Used by
   player_car_drive, player_car_engine and gear_indicator_update. */
void set_tyre_noise_on_road(uint8_t tyre_noise)
{
	if (player_car->off_road == 0u)
	{
		set_tyre_noise(tyre_noise);
		return;
	}
}

/* Sets the noise of the screen's car's tyres on the sound board (byte 718 of the screen's page; the
   sound program keeps it while the game keeps writing it and keys it off 255 of its main loop's passes
   after the last): wheelspin, skids, off-road rumble. In the demo without sound it mutes the engine
   instead. Used by player_car (player_car_engine, player_car_off_road_shake, the goal) and 00664E. */
void set_tyre_noise(uint8_t tyre_noise)
{
	uint32_t sound_page = 0;

	if ((screen->demo_flags & 0x100u) == 0u || game->advertise_sound != 0u)
	{
		SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS)[screen->index].tyre_noise = tyre_noise;
		return;
	}
	sound_page = (uint32_t)screen->index << 8;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = 0u;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests |= 0x20u;
}

/* Fades out the screen's music with 20 (fade_out_music_at): a step every 4 x (20 + 1) of the sound
   program's ticks. */
void fade_out_music(void)
{
	uint8_t fade_delay = 0;

	fade_delay = 0x20u;
	fade_out_music_at(fade_delay);
}

/* Fades out the screen's music: R0 into byte 18 of the screen's first track in the shared sound RAM
   (700918 for screen 1, 701118 for screen 2; music_fade_addresses), which has the sound program lower
   the music's volume, from its last CC 10, by one every 4 x (R0 + 1) of its ticks; at 0 it stops the
   music's player and its MultiPCM voices (CC 50 7E). 20 from fade_out_music, 3F and 7F from
   car_radio_update, 7F from goal_update. */
void fade_out_music_at(uint8_t fade_delay)
{
	MUSIC_FADE_DELAYS(music_fade_addresses[screen->index])[0] = fade_delay;
}

/* Sends sound command 1F straight to the screen's request byte in the shared sound RAM (700700 + 100 *
   screen, without going through the ring of requests) and mutes the car's engine (710 = 0, 712 bit 5, as
   set_tyre_noise does in the silent demo). As the game modes start. Command 1F plays song 03, then FF:
   song 03 sends CC 50 7F on channel E, which stops every MultiPCM voice of the screen, and FF clears the
   screen's eight sequence players (music and effects); FM notes go on. */
void reset_screen_sound(void)
{
	uint32_t sound_page = 0;

	sound_page = (uint32_t)screen->index << 8;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->command = 0x1Fu;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_revs = 0u;
	SCREEN_SOUND_PAGE(SOUND_COMMAND_1_ADDRESS + sound_page)->engine_requests |= 0x20u;
}
