/**
 *    Name:        sequences.c
 *    Description: the sequence players and the playlist: their tick, their events into the MIDI queues, the sound commands, the music's fade
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* The sequence players' step, on every timer tick: screen_1_tick_countdown runs 4 to 1 and picks a
   group, each stepped once in four ticks (step_player): at its wrap screen 2's seven effect players
   (fade_player_volume on the first of them before), at 3 screen 2's music player (fade_music_volume
   before), at 2 screen 1's seven effect players, at 1 screen 1's music player; a group waits while its
   screen's MIDI queue holds 64 messages or more. */
void sequence_tick(void)
{
	uint16_t chained_queue_pointer = 0, first_player = 0, page = 0, player_address = 0, screen_page = 0;
	uint8_t countdown = 0;

	z80_block(31, 3);
	screen_page = screen_1_tick_countdown_ADDRESS;
	countdown = screen_1_tick_countdown;
	screen_1_tick_countdown = countdown - 1u;
	if (countdown == 1u)
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
		first_player = SEQUENCE_PLAYER_ADDRESS(9);
		chained_queue_pointer = 0xE200u | screen_2_midi_write;
		chained_queue_pointer = fade_player_volume(chained_queue_pointer, first_player, page);
		z80_block(24, 2);
		chained_queue_pointer = step_player(chained_queue_pointer, first_player, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(10);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(11);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(12);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(13);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(14);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(15);
		step_player(chained_queue_pointer, player_address, page);
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
		first_player = SEQUENCE_PLAYER_ADDRESS(8);
		chained_queue_pointer = 0xE200u | screen_2_midi_write;
		chained_queue_pointer = fade_music_volume(chained_queue_pointer, first_player, page);
		z80_block(24, 2);
		step_player(chained_queue_pointer, first_player, page);
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
		first_player = SEQUENCE_PLAYER_ADDRESS(1);
		chained_queue_pointer = 0xE100u | screen_1_midi_write;
		chained_queue_pointer = fade_player_volume(chained_queue_pointer, first_player, page);
		z80_block(24, 2);
		chained_queue_pointer = step_player(chained_queue_pointer, first_player, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(2);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(3);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(4);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(5);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(6);
		chained_queue_pointer = step_player(chained_queue_pointer, player_address, page);
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(7);
		step_player(chained_queue_pointer, player_address, page);
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
	first_player = SEQUENCE_PLAYER_ADDRESS(0);
	chained_queue_pointer = 0xE100u | screen_1_midi_write;
	chained_queue_pointer = fade_music_volume(chained_queue_pointer, first_player, page);
	z80_block(24, 2);
	step_player(chained_queue_pointer, first_player, page);
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
	uint16_t cleared_player = 0, handler = 0;
	uint16_t saved_playlist = 0, saved_player = 0;

	z80_block(45, 4);
	saved_playlist = playlist;
	playlist_byte &= 0xFu;
	handler = playlist_command_table_ADDRESS;
	handler = read_table_word(handler, playlist_byte);
	z80_block(4, 1);
	switch (handler)
	{
	case 0x003FCCu:
		z80_block(57, 8);
		playlist = z80_read16(saved_playlist);
		playlist_byte = z80_read8(playlist);
		playlist++;
		break;
	case 0x003FD4u:
		z80_block(53, 6);
		playlist = saved_playlist;
		playlist_byte = (SCREEN_PAGE(page)->playlist_choice_1 & 7u) | 0x90u;
		break;
	case 0x003FDDu:
		z80_block(53, 6);
		playlist = saved_playlist;
		playlist_byte = (SCREEN_PAGE(page)->playlist_choice_2 & 7u) | 0x90u;
		break;
	case 0x003FE6u:
		z80_block(117, 15);
		saved_player = player;
		cleared_player = saved_player;
		playlist_byte = 0u;
		SEQUENCE_PLAYER(cleared_player)->flags = 0u;
		move_block(cleared_player, (uint16_t)(cleared_player + 1u), 0x30u, 1u);
		playlist = saved_playlist;
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
		break;
	}
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
	uint8_t control_value = 0, controller_number = 0, data_byte = 0, delta_byte = 0, delta_time_left_high = 0, delta_time_left_low = 0, delta_time_low = 0, meta_51_value = 0, meta_bytes_left = 0, meta_type = 0, new_delta_high = 0, playlist_byte = 0, playlist_player_low = 0, queued_byte = 0, status = 0, sysex_byte = 0, sysex_bytes_left = 0, system_status = 0, volume_value = 0, write_index = 0, zero_delta = 0;
	uint16_t delta_time_at = 0, player = 0, playlist = 0, playlist_player = 0, position = 0, sequence_player = 0;
	uint8_t queue_page = 0, saved_write_index = 0;
	uint16_t player_page_start = 0, saved_player = 0;
	uint8_t controller = 0;
	uint16_t delta_time_left = 0;

	write_index = (uint8_t)queue_pointer;
	z80_block(17, 3);
	if (SEQUENCE_PLAYER(player_address)->flags >> 7 == 0u)
	{
		z80_cycles(6);
		return queue_pointer;
	}
	z80_block(49, 8);
	sequence_player = IN_PAGE(player_address, 7u);
	delta_time_low = SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low;
	sequence_player = IN_PAGE(sequence_player, (uint8_t)(sequence_player - 1u));
	delta_time_left = (((uint32_t)z80_read8(sequence_player) << 8) | delta_time_low) - 1u;
	delta_time_left_high = (uint8_t)(delta_time_left >> 8);
	delta_time_left_low = (uint8_t)delta_time_left;
	zero_delta = 0u;
	if (0u != delta_time_left_low)
	{
		z80_block(35, 5);
		delta_time_at = IN_PAGE(sequence_player, 7u);
		z80_write8(delta_time_at, delta_time_left_low);
		z80_write8(IN_PAGE(delta_time_at, (uint8_t)(delta_time_at - 1u)), delta_time_left_high);
		return queue_pointer;
	}
	z80_block(14, 2);
	if (zero_delta != delta_time_left_high)
	{
		z80_block(35, 5);
		delta_time_at = IN_PAGE(sequence_player, 7u);
		z80_write8(delta_time_at, delta_time_left_low);
		z80_write8(IN_PAGE(delta_time_at, (uint8_t)(delta_time_at - 1u)), delta_time_left_high);
		return queue_pointer;
	}
	z80_block(32, 4);
	player_page_start = sequence_player & ~0xFFu;
	player = player_page_start;
	for (;;)
	{
		z80_block(68, 7);
		port_out(ROM_BANK, SEQUENCE_PLAYER(player)->bank);
		position = SEQUENCE_PLAYER(player)->position;
		for (;;)
		{
			z80_block(48, 6);
			SEQUENCE_PLAYER(player)->flags &= 0xBFu;
			status = z80_read8(position);
			if (status >> 7 == 0u)
			{
				z80_block(35, 4);
				status = SEQUENCE_PLAYER(player)->running_status;
				position--;
			}
			else
			{
				z80_block(19, 2);
				SEQUENCE_PLAYER(player)->running_status = status;
			}
			z80_block(17, 2);
			if (status == 0xFFu)
			{
				z80_block(17, 1);
				position = next_sequence_byte(position, player);
				z80_block(24, 3);
				meta_type = z80_read8(position);
				if (meta_type == 0x2Fu)
					break;
				z80_block(17, 2);
				if (meta_type == 0x51u)
				{
					z80_block(17, 1);
					position = next_sequence_byte(position, player);
					z80_block(17, 1);
					position = next_sequence_byte(position, player);
					z80_block(25, 4);
					meta_51_value = z80_read8(position);
					if (meta_51_value >> 7)
					{
						z80_block(23, 5);
						meta_51_value = (0u - meta_51_value) | 0x80u;
					}
					z80_block(36, 3);
					SEQUENCE_PLAYER(player)->meta_51_value = meta_51_value;
					position = next_sequence_byte(position, player);
					z80_block(10, 1);
				}
				else
				{
					z80_block(17, 1);
					position = next_sequence_byte(position, player);
					z80_block(7, 1);
					meta_bytes_left = z80_read8(position);
					for (;;)
					{
						z80_block(17, 1);
						position = next_sequence_byte(position, player);
						z80_block(8, 1);
						meta_bytes_left--;
						if (meta_bytes_left == 0u)
							break;
						z80_cycles(5);
					}
					z80_block(17, 1);
					position = next_sequence_byte(position, player);
					z80_block(10, 1);
				}
			}
			else
			{
				z80_block(28, 4);
				queued_byte = status;
				status &= ~0xFu;
				if (status == 0xF0u)
				{
					z80_block(21, 2);
					system_status = queued_byte;
					position = next_sequence_byte(position, player);
					z80_block(17, 2);
					if (system_status == 0xF7u)
					{
						z80_block(7, 1);
						sysex_bytes_left = z80_read8(position);
						for (;;)
						{
							z80_block(24, 2);
							position = next_sequence_byte(position, player);
							z80_block(8, 1);
							sysex_bytes_left--;
							if (sysex_bytes_left == 0u)
								break;
							z80_cycles(5);
						}
						z80_block(10, 1);
					}
					else
					{
						z80_block(17, 2);
						if (system_status != 0xF0u)
						{
							z80_block(10, 1);
							break;
						}
						for (;;)
						{
							z80_block(24, 2);
							sysex_byte = z80_read8(position);
							position = next_sequence_byte(position, player);
							z80_block(17, 2);
							if (sysex_byte == 0xF7u)
								break;
							z80_block(10, 1);
						}
					}
					z80_block(33, 3);
					SCREEN_PAGE(page)->midi_count++;
				}
				else
				{
					z80_block(17, 2);
					if (status == 0x80u)
					{
						z80_block(32, 4);
						z80_write8(IN_PAGE(queue_pointer, write_index), queued_byte);
						write_index++;
						position = next_sequence_byte(position, player);
						z80_block(25, 4);
						data_byte = z80_read8(position);
						if (data_byte >> 7 != 0u)
						{
							z80_cycles(7);
							data_byte = chain_next_event(player, data_byte);
						}
						z80_block(28, 3);
						z80_write8(IN_PAGE(queue_pointer, write_index), data_byte);
						write_index++;
						position = next_sequence_byte(position, player);
						z80_block(51, 6);
						z80_write8(IN_PAGE(queue_pointer, write_index), 0x7Fu);
						write_index++;
						SCREEN_PAGE(page)->midi_count++;
					}
					else
					{
						z80_block(17, 2);
						if (status == 0xB0u)
						{
							z80_block(32, 4);
							z80_write8(IN_PAGE(queue_pointer, write_index), queued_byte);
							write_index++;
							position = next_sequence_byte(position, player);
							z80_block(35, 4);
							controller_number = z80_read8(position);
							z80_write8(IN_PAGE(queue_pointer, write_index), controller_number);
							write_index++;
							position = next_sequence_byte(position, player);
							z80_block(24, 3);
							controller = controller_number;
							control_value = z80_read8(position);
							if (controller == 0x10u)
							{
								z80_block(37, 5);
								volume_value = control_value;
								if (SEQUENCE_PLAYER(player)->fade_speed == 0u)
								{
									z80_block(31, 5);
									control_value = volume_value;
									SEQUENCE_PLAYER(player)->fade_volume = volume_value & 0x7Fu;
								}
								else
								{
									z80_block(29, 3);
									control_value = SEQUENCE_PLAYER(player)->fade_volume;
								}
							}
							z80_block(18, 3);
							if (control_value >> 7 != 0u)
							{
								z80_cycles(7);
								control_value = chain_next_event(player, control_value);
							}
							z80_block(28, 3);
							z80_write8(IN_PAGE(queue_pointer, write_index), control_value);
							write_index++;
							position = next_sequence_byte(position, player);
							z80_block(33, 3);
							SCREEN_PAGE(page)->midi_count++;
						}
						else
						{
							z80_block(17, 2);
							if (status != 0xC0u)
							{
								z80_block(17, 2);
								if (status != 0xD0u)
								{
									z80_block(10, 1);
									z80_block(32, 4);
									z80_write8(IN_PAGE(queue_pointer, write_index), queued_byte);
									write_index++;
									position = next_sequence_byte(position, player);
									z80_block(7, 1);
									queued_byte = z80_read8(position);
								}
							}
							z80_block(32, 4);
							z80_write8(IN_PAGE(queue_pointer, write_index), queued_byte);
							write_index++;
							position = next_sequence_byte(position, player);
							z80_block(25, 4);
							data_byte = z80_read8(position);
							if (data_byte >> 7 != 0u)
							{
								z80_cycles(7);
								data_byte = chain_next_event(player, data_byte);
							}
							z80_block(28, 3);
							z80_write8(IN_PAGE(queue_pointer, write_index), data_byte);
							write_index++;
							position = next_sequence_byte(position, player);
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
			delta_byte = z80_read8(position);
			position = next_sequence_byte(position, player);
			z80_block(14, 2);
			if (delta_byte == 0u)
				continue;
			z80_block(18, 3);
			if (delta_byte >> 7)
			{
				z80_block(37, 7);
				delta_byte &= 0x7Fu;
				carry = delta_byte & 1u;
				new_delta_high = delta_byte >> 1;
				delta_byte = z80_read8(position);
				if (carry)
				{
					z80_block(8, 2);
					delta_byte |= 0x80u;
				}
				z80_block(17, 1);
				position = next_sequence_byte(position, player);
				z80_block(19, 2);
				SEQUENCE_PLAYER(player)->delta_time_high = new_delta_high;
			}
			z80_block(86, 9);
			SEQUENCE_PLAYER(player)->delta_time_low = delta_byte;
			SEQUENCE_PLAYER(player)->position = position;
			SCREEN_PAGE(page)->midi_write = write_index;
			queue_pointer = (queue_pointer & ~0xFFu) | write_index;
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
			SCREEN_PAGE(page)->midi_write = write_index;
			queue_pointer = (queue_pointer & ~0xFFu) | write_index;
			return queue_pointer;
		}
		z80_block(62, 6);
		playlist = SEQUENCE_PLAYER(player)->playlist;
		playlist_byte = z80_read8(playlist);
		playlist = next_sequence_byte(playlist, player);
		z80_block(17, 2);
		if (playlist_byte >= 0xF0u)
		{
			z80_cycles(7);
			playlist_command(queue_pointer, player, page, &playlist, &playlist_byte);
		}
		z80_block(14, 2);
		if (playlist_byte == 0u)
		{
			z80_block(30, 3);
			if (((SEQUENCE_PLAYER(player)->flags >> 4) & 1u) != 0u)
			{
				z80_cycles(7);
				clear_player_end_flag(page);
			}
			z80_block(52, 6);
			SEQUENCE_PLAYER(player)->flags = 0u;
			SCREEN_PAGE(page)->midi_write = write_index;
			queue_pointer = (queue_pointer & ~0xFFu) | write_index;
			return queue_pointer;
		}
		z80_block(104, 11);
		SEQUENCE_PLAYER(player)->playlist = playlist;
		queue_page = queue_pointer >> 8;
		saved_write_index = write_index;
		saved_player = player;
		playlist_player = saved_player;
		playlist_player_low = (uint8_t)saved_player;
		if (((SEQUENCE_PLAYER(player)->flags >> 4) & 1u) != 0u)
		{
			z80_block(17, 1);
			playlist_player |= playlist_player_low;
			start_sound(playlist_player, playlist_byte);
			z80_block(20, 2);
			queue_pointer = (uint32_t)queue_page << 8;
			write_index = saved_write_index;
			continue;
		}
		z80_block(17, 1);
		start_song(playlist_player, playlist_byte);
		z80_block(20, 2);
		queue_pointer = (uint32_t)queue_page << 8;
		write_index = saved_write_index;
	}
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
	uint8_t command = 0;
	uint16_t page = 0, player_address = 0;

	z80_block(31, 4);
	page = SCREEN_PAGE_ADDRESS(0);
	command = screen_1_command;
	if (command != 0u)
	{
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(0);
		take_sound_command(player_address, page, command);
	}
	z80_block(31, 4);
	page = SCREEN_PAGE_ADDRESS(1);
	command = screen_2_command;
	if (command != 0u)
	{
		z80_block(27, 2);
		player_address = SEQUENCE_PLAYER_ADDRESS(8);
		take_sound_command(player_address, page, command);
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
	uint8_t header_flags = 0, priority = 0, song = 0;
	uint16_t command_header = 0, header = 0, player = 0, sequence_player = 0;
	uint16_t swapped_player = 0;

	z80_block(27, 2);
	command_header = sound_commands_ADDRESS;
	command_header = read_table_word(command_header, command);
	z80_block(29, 5);
	swapped_player = player_address;
	header = command_header;
	sequence_player = swapped_player;
	header_flags = z80_read8(header);
	if (header_flags >> 7 != 0u)
	{
		z80_block(18, 3);
		if (((header_flags >> 6) & 1u) != 0u)
		{
			z80_block(29, 4);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
			select_channel_player(&header, &sequence_player);
			z80_block(46, 7);
			priority = z80_read8(header) & 0xFu;
			if (priority < (z80_read8(sequence_player) & 0xFu))
			{
				z80_block(10, 1);
				return;
			}
			z80_block(17, 1);
			sequence_player = clear_player(sequence_player);
			z80_block(87, 12);
			header++;
			SEQUENCE_PLAYER(sequence_player)->flags = priority;
			sequence_player = IN_PAGE(sequence_player, 2u);
			SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)header;
			sequence_player = IN_PAGE(sequence_player, (uint8_t)(sequence_player + 1u));
			SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = header >> 8;
			sequence_player = IN_PAGE(sequence_player, 7u);
			SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
			sequence_player &= ~0xFFu;
			SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
			return;
		}
		z80_block(10, 1);
		z80_block(54, 9);
		sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
		sequence_player = (sequence_player & 0xFFu) | ((uint32_t)(uint8_t)((sequence_player >> 8) + 1u) << 8);
		priority = z80_read8(header) & 0xFu;
		if (priority < (z80_read8(sequence_player) & 0xFu))
		{
			z80_block(10, 1);
			return;
		}
		z80_block(17, 1);
		sequence_player = clear_player(sequence_player);
		z80_block(7, 1);
		SEQUENCE_PLAYER(sequence_player)->flags = priority;
		z80_block(80, 11);
		header++;
		sequence_player = IN_PAGE(sequence_player, 2u);
		SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)header;
		sequence_player = IN_PAGE(sequence_player, (uint8_t)(sequence_player + 1u));
		SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = header >> 8;
		sequence_player = IN_PAGE(sequence_player, 7u);
		SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
		return;
	}
	z80_block(18, 3);
	if (((header_flags >> 6) & 1u) != 0u)
	{
		z80_block(80, 11);
		header++;
		sequence_player = IN_PAGE(sequence_player, 2u);
		SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = (uint8_t)header;
		sequence_player = IN_PAGE(sequence_player, (uint8_t)(sequence_player + 1u));
		SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = header >> 8;
		sequence_player = IN_PAGE(sequence_player, 7u);
		SEQUENCE_PLAYER(sequence_player - 0x7u)->delta_time_low = 1u;
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags |= 0x80u;
		return;
	}
	z80_block(18, 3);
	if (((header_flags >> 5) & 1u) != 0u)
	{
		start_sequence(header, sequence_player);
		return;
	}
	z80_block(18, 3);
	if (((header_flags >> 4) & 1u) == 0u)
		z80_block(10, 1);
	z80_block(17, 1);
	sequence_player = clear_player(sequence_player);
	z80_block(82, 12);
	header++;
	song = z80_read8(header);
	SEQUENCE_PLAYER(sequence_player)->flags |= 0x20u;
	header++;
	sequence_player = IN_PAGE(sequence_player, 0x20u);
	SEQUENCE_PLAYER(sequence_player - 0x20u)->playlist_low = (uint8_t)header;
	sequence_player++;
	SEQUENCE_PLAYER(sequence_player - 0x21u)->playlist_high = header >> 8;
	player = sequence_player;
	start_song(player, song);
}

/* Starts song 'song' (songs) on the player: start_sequence with its record. */
void start_song(uint16_t player, uint8_t song)
{
	uint16_t header = 0, player_2 = 0, song_record = 0;
	uint16_t swapped_player = 0;

	z80_block(27, 2);
	song_record = songs_ADDRESS;
	song_record = read_table_word(song_record, song);
	z80_block(21, 3);
	swapped_player = player;
	header = song_record;
	player_2 = swapped_player;
	start_sequence(header, player_2);
}

/* The player plays the sequence of a song record or of a sound header (its flags byte skipped): its bank
   (byte 1) and address (bytes 2-3), delta_time_low 1, flags bit 7; nothing else of the player changes. */
void start_sequence(uint16_t header, uint16_t player)
{
	uint8_t bank = 0;
	uint16_t sequence_player = 0;

	z80_block(127, 18);
	header++;
	sequence_player = IN_PAGE(player, 0x1Bu);
	bank = z80_read8(header);
	header++;
	SEQUENCE_PLAYER(sequence_player - 0x1Bu)->bank = bank;
	sequence_player = IN_PAGE(sequence_player, 2u);
	SEQUENCE_PLAYER(sequence_player - 0x2u)->position_low = z80_read8(header);
	sequence_player = IN_PAGE(sequence_player, (uint8_t)(sequence_player + 1u));
	SEQUENCE_PLAYER(sequence_player - 0x3u)->position_high = z80_read8((uint16_t)(header + 1u));
	sequence_player = IN_PAGE(sequence_player, 7u);
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
	uint8_t player_page = 0;

	z80_block(55, 9);
	header++;
	player_page = (z80_read8(header) & 0xFu) - 0xAu + (player_address >> 8);
	header--;
	player_address = (uint32_t)player_page << 8;
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
	uint8_t write_index = 0;
	uint16_t sequence_player = 0;

	write_index = (uint8_t)queue_pointer;
	z80_block(23, 4);
	sequence_player = IN_PAGE(player, 0x18u);
	if (SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed == 0u)
	{
		z80_cycles(6);
		return queue_pointer;
	}
	z80_block(28, 4);
	sequence_player = IN_PAGE(sequence_player, 0x19u);
	if (SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait != 0u)
	{
		z80_block(21, 2);
		SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait--;
		return queue_pointer;
	}
	z80_block(39, 5);
	sequence_player = IN_PAGE(sequence_player, 0x15u);
	SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume--;
	if (SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume == 0u)
	{
		z80_block(223, 31);
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags &= 0x7Fu;
		SEQUENCE_PLAYER(sequence_player)->fade_speed = 0u;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0xBEu);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0x50u);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0x7Eu);
		write_index++;
		SCREEN_PAGE(page)->midi_count++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0xBEu);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0x79u);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0u);
		write_index++;
		SCREEN_PAGE(page)->midi_count++;
		SCREEN_PAGE(page)->midi_write = write_index;
		queue_pointer = (queue_pointer & ~0xFFu) | write_index;
		return queue_pointer;
	}
	z80_block(134, 18);
	z80_write8(IN_PAGE(queue_pointer, write_index), 0xBEu);
	write_index++;
	z80_write8(IN_PAGE(queue_pointer, write_index), 0x10u);
	write_index++;
	z80_write8(IN_PAGE(queue_pointer, write_index), SEQUENCE_PLAYER(sequence_player - 0x15u)->fade_volume);
	write_index++;
	SCREEN_PAGE(page)->midi_write = write_index;
	SCREEN_PAGE(page)->midi_count++;
	sequence_player = IN_PAGE(sequence_player, 0x18u);
	SEQUENCE_PLAYER(sequence_player & ~0xFFu)->fade_wait = SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed;
	queue_pointer = (queue_pointer & ~0xFFu) | write_index;
	return queue_pointer;
}

/* The same for the player after the music player, with fade_channel_volume going out as CC 7 on channel
   A; at 0 the player is cleared (flags 0) and CC 7B 0 (all notes off) goes out on channel A. Nothing
   sets that player's fade_speed, and no command starts that player. */
uint16_t fade_player_volume(uint16_t queue_pointer, uint16_t player, uint16_t page)
{
	uint8_t write_index = 0;
	uint16_t sequence_player = 0;

	write_index = (uint8_t)queue_pointer;
	z80_block(23, 4);
	sequence_player = IN_PAGE(player, 0x18u);
	if (SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed == 0u)
	{
		z80_cycles(6);
		return queue_pointer;
	}
	z80_block(28, 4);
	sequence_player = IN_PAGE(sequence_player, 0x19u);
	if (SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait != 0u)
	{
		z80_block(21, 2);
		SEQUENCE_PLAYER(sequence_player - 0x19u)->fade_wait--;
		return queue_pointer;
	}
	z80_block(39, 5);
	sequence_player = IN_PAGE(sequence_player, 0x1Au);
	SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume--;
	if (SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume == 0u)
	{
		z80_block(138, 19);
		sequence_player &= ~0xFFu;
		SEQUENCE_PLAYER(sequence_player)->flags = 0u;
		SEQUENCE_PLAYER(sequence_player)->fade_speed = 0u;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0xBAu);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0x7Bu);
		write_index++;
		z80_write8(IN_PAGE(queue_pointer, write_index), 0u);
		write_index++;
		SCREEN_PAGE(page)->midi_count++;
		SCREEN_PAGE(page)->midi_write = write_index;
		queue_pointer = (queue_pointer & ~0xFFu) | write_index;
		return queue_pointer;
	}
	z80_block(134, 18);
	z80_write8(IN_PAGE(queue_pointer, write_index), 0xBAu);
	write_index++;
	z80_write8(IN_PAGE(queue_pointer, write_index), 7u);
	write_index++;
	z80_write8(IN_PAGE(queue_pointer, write_index), SEQUENCE_PLAYER(sequence_player - 0x1Au)->fade_channel_volume);
	write_index++;
	SCREEN_PAGE(page)->midi_write = write_index;
	SCREEN_PAGE(page)->midi_count++;
	sequence_player = IN_PAGE(sequence_player, 0x18u);
	SEQUENCE_PLAYER(sequence_player & ~0xFFu)->fade_wait = SEQUENCE_PLAYER(sequence_player - 0x18u)->fade_speed;
	queue_pointer = (queue_pointer & ~0xFFu) | write_index;
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
