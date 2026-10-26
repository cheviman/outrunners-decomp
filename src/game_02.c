/**
 *    Name:        game_02.c
 *    Description: the game's functions, part 02 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The entries to a race, each frame. Player 0's screen keeps them in its record: once every entry is
   taken (it means to wait for each record's entry_serial_seen, but compares player 0's own each time), a
   player's entry_request not yet accepted (accepted_players) is accepted: entrant = that player,
   entry_serial + 1, and the first one starts entry_frames at 600 (entry_frames_by_machines), cut to 40
   once every player is in; entry_frames counting down to 0 closes the entries (entrant FF, entry_serial
   + 1, accepted_players cleared, 1 to the link board's 80000A). Every screen: when some player's race is
   over (race game_over) an idle screen (game steps 1-5) goes to step 5 (cleared_on_race_over cleared),
   and while player 0's entry_frames runs to step 7 (entry_invitation_step); each new entry_serial of
   player 0's record (entry_serial_seen) adds its entrant to the screen's entrants (entrant_count), or
   the close clears them (and entry_accepted); the screen's own player accepted: its entry_request
   cleared, entry_accepted, the first entrant into host_player and is_host when that is itself; the host
   keeps racer_count and race_order in its record from the entrants. */
void race_entries_update(void)
{
	uint32_t zero = 0;
	uint8_t entrant = 0, entrants = 0, entry_serial = 0;
	uint16_t next_entry_frames = 0, record_size = 0;
	uint32_t entrant_count = 0, entrant_list = 0, entry_frames = 0, machines = 0, other_race = 0, player = 0, players_left = 0, race = 0, request = 0;
	uint8_t game_over = 0;
	uint16_t old_entry_frames = 0;
	uint32_t last_four_entrants = 0, race_order_long = 0;

	race = SCREEN(screen)->race;
	r12 = RACE_COPY_1;
	if (SCREEN(screen)->player == 0u)
	{
		players_left = player_count;
		record_size = 0x80u;
		entry_serial = RACE(race)->entry_serial;
		other_race = r12 + 0x1Cu;
		r14 = screen + 0x300u;
		entry_frames = RACE(race)->entry_frames;
		do
		{
			players_left--;
		} while ((players_left != 0u) & (RACE(other_race - 0x1Cu)->entry_serial_seen == entry_serial));
		if (RACE(other_race - 0x1Cu)->entry_serial_seen == entry_serial)
		{
			if (entry_frames == 0u)
			{
				machines = 0u;
				if (stand_alone == 0u)
					machines = machine_count;
				next_entry_frames = entry_frames_by_machines[machines];
				request = r12 + 0x19u;
				players_left = player_count;
				player = 0u;
				for (;;)
				{
					if (mem_read8(r14 + player) != 0u || mem_read8(request) == 0u)
					{
						player = (uint8_t)(player + 1u);
						request = (request & ~0xFFFFu) | (uint16_t)(request + record_size);
						players_left--;
						if (players_left == 0u)
							break;
						continue;
					}
					mem_write8(r14 + player, 0xFFu);
					RACE(race)->entrant = player;
					RACE(race)->entry_serial++;
					entry_frames = next_entry_frames;
					if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != player_count || entry_frames <= 0x28u)
						break;
					entry_frames = 0x28u;
					break;
				}
			}
			else
			{
				old_entry_frames = entry_frames;
				entry_frames = (uint16_t)(old_entry_frames - 1u);
				next_entry_frames = (uint16_t)entry_frames;
				if (old_entry_frames != 1u)
				{
					request = r12 + 0x19u;
					players_left = player_count;
					player = 0u;
					for (;;)
					{
						if (mem_read8(r14 + player) != 0u || mem_read8(request) == 0u)
						{
							player = (uint8_t)(player + 1u);
							request = (request & ~0xFFFFu) | (uint16_t)(request + record_size);
							players_left--;
							if (players_left == 0u)
								break;
							continue;
						}
						mem_write8(r14 + player, 0xFFu);
						RACE(race)->entrant = player;
						RACE(race)->entry_serial++;
						entry_frames = next_entry_frames;
						if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != player_count || entry_frames <= 0x28u)
							break;
						entry_frames = 0x28u;
						break;
					}
				}
				else
				{
					RACE(race)->entry_serial++;
					RACE(race)->entrant = 0xFFu;
					AS32(SCREEN(r14 - 0x300u)->accepted_players) = 0u;
					r14 = r14 + 4u;
					SCREEN(r14 - 0x304u)->last_four_accepted_players = 0u;
					mem_write8(0x80000Au, 1u);
					r14 = r14 + 4u;
				}
			}
		}
		RACE(race)->entry_frames = entry_frames;
	}
	players_left = player_count;
	other_race = r12 + 0x28u;
	do
	{
		game_over = RACE(other_race - 0x28u)->game_over;
		zero = game_over == 0u;
		other_race += 0x80u;
		players_left--;
	} while ((players_left != 0u) & (game_over == 0u));
	if (zero)
	{
		if (RACE(r12)->entry_frames != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	else if (SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
	{
		AS32(SCREEN(screen)->game_mode) = 5u;
		cleared_on_race_over = 0u;
		if (RACE(r12)->entry_frames != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	if (RACE(r12)->entry_serial == RACE(race)->entry_serial_seen)
		return;
	RACE(race)->entry_serial_seen = RACE(r12)->entry_serial;
	entrant = RACE(r12)->entrant;
	if ((int8_t)entrant < 0)
	{
		SCREEN(screen)->entrant_count = 0u;
		SCREEN(screen)->entry_accepted = 0u;
		return;
	}
	entrant_list = screen + 0x30Fu;
	entrant_count = SCREEN(entrant_list - 0x30Fu)->entrant_count;
	entrant_count = (uint8_t)(entrant_count + 1u);
	SCREEN(entrant_list - 0x30Fu)->entrant_count = entrant_count;
	mem_write8(entrant_list + entrant_count, entrant);
	if (SCREEN(screen)->player == entrant)
	{
		RACE(race)->entry_request = 0u;
		SCREEN(screen)->entry_accepted = 0xFFu;
		entrants = SCREEN(entrant_list - 0x30Fu)->entrants;
		SCREEN(screen)->host_player = entrants;
		if (SCREEN(screen)->player == entrants)
			SCREEN(screen)->is_host = 0xFFu;
	}
	if (SCREEN(screen)->entry_accepted != 0u && SCREEN(screen)->is_host != 0u)
	{
		r14 = race + 0x1Fu;
		RACE(r14 - 0x1Fu)->racer_count = SCREEN(entrant_list - 0x30Fu)->entrant_count;
		r14 = r14 + 1u;
		entrant_list++;
		last_four_entrants = SCREEN(entrant_list - 0x310u)->last_four_entrants;
		race_order_long = AS32(SCREEN(entrant_list - 0x310u)->entrants);
		RACE(r14 - 0x20u)->race_order = (uint8_t)race_order_long;
		RACE(r14 - 0x20u)->race_order_1 = (uint8_t)(race_order_long >> 8u);
		RACE(r14 - 0x20u)->race_order_2 = (uint8_t)(race_order_long >> 16u);
		RACE(r14 - 0x20u)->race_order_3 = (uint8_t)(race_order_long >> 24u);
		RACE(r14 - 0x20u)->race_order_4_7 = last_four_entrants;
		r14 = r14 + 8u;
	}
}

/* The race's progress, each frame of a race (screen in_game). The host (is_host) runs it in its record:
   once every racer has taken the last pass (pass_seen = pass_count), a racer whose stage_count changed
   (against counted_stages) has passed a checkpoint (stage_count - 1): pass_player, pass_checkpoint,
   pass_order (counted_passes), pass_branch (the side of the road it was on: race road_x bit 15),
   pass_time (race_time in hundredths plus the order), pass_count + 1; the first to pass a checkpoint
   before the goal adds the time of the stage its branch leads to (stage_start_times by
   fork_next_stages[stage * 2 + branch]) plus the setting for that checkpoint (master_time_difficulty) to
   time_left. Once the race has started (race_started) race_time + 34 and time_left - 1 a frame, and one
   pass of sorting race_order: the finished (race finished) first by finish_time, then by distance (race
   segment_fraction-43). Every screen from the host's record (host_record): the leader's segment and
   stage (leader_window_segment, leader_stage), each player's place (places, its own in place); each new
   pass: pass_seen (screen and race), checkpoints_passed, checkpoint_passes, the order and the time by
   player and by order (checkpoint_orders, checkpoint_order, checkpoint_times_by_player,
   checkpoint_times); the first car past a fork decides the route: forks_decided + 1, its branch in
   route_branches and the stage it leads to in route_stages. */
void race_progress_update(void)
{
	uint8_t branch = 0, host_pass_count = 0, pass_branch = 0, pass_count = 0, place = 0, player = 0;
	uint16_t order = 0, pass_time = 0;
	uint32_t ahead_player = 0, ahead_race = 0, checked_race = 0, checkpoint = 0, checkpoint_offset = 0, comparisons_left = 0, fork_index = 0, host_order = 0, host_racer_count = 0, next_player = 0, order_entry = 0, order_place = 0, pass_checkpoint = 0, pass_order = 0, pass_player = 0, passing_race = 0, race = 0, racer = 0, racer_count = 0, racers_left = 0, sort_place = 0;
	uint8_t racer_total = 0;
	uint32_t pass_counter = 0, swapped_player = 0, player_checkpoint_count = 0, checkpoint_pass_count = 0;

	race = SCREEN(screen)->race;
	r12 = SCREEN(screen)->host_record;
	if (SCREEN(screen)->in_game == 0u)
		return;
	if (SCREEN(screen)->is_host != 0u)
	{
		host_order = r12 + 0x1Fu;
		host_racer_count = RACE(host_order - 0x1Fu)->racer_count;
		host_pass_count = RACE(race)->pass_count;
		host_order++;
		order_entry = host_order;
		racers_left = host_racer_count;
		do
		{
			checked_race = race_copy_offsets[mem_read8(order_entry)] + GAME_VARIABLES;
			racers_left--;
			order_entry++;
		} while ((racers_left != 0u) & (RACE(checked_race)->pass_seen == host_pass_count));
		if (RACE(checked_race)->pass_seen == host_pass_count)
		{
			order_entry = host_order;
			r14 = screen + 0x330u;
			racers_left = host_racer_count;
			do
			{
				racer = mem_read8(order_entry);
				passing_race = race_copy_offsets[racer] + GAME_VARIABLES;
				racers_left--;
				order_entry++;
			} while ((racers_left != 0u) & (mem_read8(r14 + racer) == RACE(passing_race)->stage_count));
			if (mem_read8(r14 + racer) != RACE(passing_race)->stage_count)
			{
				checkpoint = RACE(passing_race)->stage_count;
				mem_write8(r14 + racer, checkpoint);
				RACE(race)->pass_player = racer;
				checkpoint = (uint8_t)(checkpoint - 1u);
				RACE(race)->pass_checkpoint = checkpoint;
				order = mem_read8(screen + 0x338u + checkpoint);
				pass_counter = screen + 0x338u + checkpoint;
				mem_write8(pass_counter, mem_read8(pass_counter) + 1u);
				RACE(race)->pass_order = (uint8_t)order;
				branch = (uint8_t)((uint16_t)RACE(passing_race)->road_x >> 15);
				RACE(race)->pass_branch = branch;
				RACE(race)->pass_time = RACE(race)->race_time / 0xAu + order;
				RACE(race)->pass_count++;
				if (order == 0u && checkpoint != 5u)
				{
					fork_index = RACE(passing_race)->stage;
					fork_index = (uint8_t)(fork_index * 2u);
					fork_index = (uint8_t)(fork_index + branch);
					RACE(race)->time_left += (int8_t)(stage_start_times[fork_next_stages[fork_index]] + master_time_difficulty[checkpoint]) * 0x1Eu;
				}
			}
		}
		if (SCREEN(screen)->race_started != 0u)
		{
			RACE(race)->race_time += 0x22u;
			if (RACE(race)->time_left != 0u)
				RACE(race)->time_left--;
		}
		sort_place = race + 0x1Fu;
		racer_count = RACE(sort_place - 0x1Fu)->racer_count;
		racer_total = racer_count;
		comparisons_left = (uint8_t)(racer_total - 1u);
		sort_place++;
		if ((int8_t)racer_total > 1)
		{
			ahead_player = RACE(sort_place - 0x20u)->race_order;
			ahead_race = race_copy_offsets[ahead_player] + GAME_VARIABLES;
			sort_place++;
			if (SCREEN(screen)->race_started != 0u)
			{
				do
				{
					next_player = mem_read8(sort_place);
					r14 = race_copy_offsets[next_player] + GAME_VARIABLES;
					sort_place++;
					if (RACE(ahead_race)->finished != 0u)
					{
						if (RACE(r14)->finished == 0u || RACE(ahead_race)->finish_time <= RACE(r14)->finish_time)
						{
							swapped_player = next_player;
							next_player = ahead_player;
							ahead_player = swapped_player;
							ahead_race = r14;
						}
					}
					else if ((RACE(r14)->segment_fraction | ((uint32_t)RACE(r14)->segment << 16u)) <= (RACE(ahead_race)->segment_fraction | ((uint32_t)RACE(ahead_race)->segment << 16u)))
					{
						swapped_player = next_player;
						next_player = ahead_player;
						ahead_player = swapped_player;
						ahead_race = r14;
					}
					mem_write8(sort_place - 2u, next_player);
					comparisons_left--;
				} while (comparisons_left != 0u);
				mem_write8(sort_place - 1u, ahead_player);
			}
		}
	}
	order_place = r12 + 0x1Fu;
	racers_left = RACE(order_place - 0x1Fu)->racer_count;
	place = 0u;
	order_place++;
	racer = RACE(order_place - 0x20u)->race_order;
	player = SCREEN(screen)->player;
	r14 = race_copy_offsets[racer] + GAME_VARIABLES;
	SCREEN(screen)->leader_window_segment = RACE(r14)->window_segment;
	SCREEN(screen)->leader_stage = RACE(r14)->stage;
	order_place++;
	do
	{
		mem_write8(screen + 0x320u + racer, place);
		if (racer == player)
			SCREEN(screen)->place = place;
		racer = mem_read8(order_place);
		place++;
		racers_left--;
		order_place++;
	} while (racers_left != 0u);
	if (SCREEN(screen)->pass_seen != RACE(r12)->pass_count)
	{
		pass_count = RACE(r12)->pass_count;
		SCREEN(screen)->pass_seen = pass_count;
		RACE(race)->pass_seen = pass_count;
		pass_player = RACE(r12)->pass_player;
		pass_checkpoint = RACE(r12)->pass_checkpoint;
		pass_order = RACE(r12)->pass_order;
		pass_branch = RACE(r12)->pass_branch;
		pass_time = RACE(r12)->pass_time;
		checkpoint_offset = pass_checkpoint * 2u;
		player_checkpoint_count = screen + 0x480u + pass_player;
		mem_write8(player_checkpoint_count, mem_read8(player_checkpoint_count) + 1u);
		checkpoint_pass_count = screen + 0x35Au + pass_checkpoint;
		mem_write8(checkpoint_pass_count, mem_read8(checkpoint_pass_count) + 1u);
		mem_write8(screen + 0x360u + checkpoint_offset * 4u + pass_player, pass_order);
		mem_write8(screen + 0x390u + checkpoint_offset * 4u + pass_order, pass_player);
		checkpoint_offset = checkpoint_offset * 2u;
		mem_write16(screen + 0x3C0u + checkpoint_offset * 4u + pass_player * 2u, pass_time);
		mem_write16(screen + 0x420u + checkpoint_offset * 4u + pass_order * 2u, pass_time);
		if (pass_order == 0u && pass_checkpoint != 5u)
		{
			SCREEN(screen)->forks_decided++;
			mem_write8(screen + 0x34Au + pass_checkpoint, pass_branch);
			fork_index = mem_read8(race_copy_offsets[pass_player] + 0x20E062u);
			fork_index = (uint8_t)(fork_index * 2u);
			mem_write8(screen + 0x344u + pass_checkpoint, fork_next_stages[(uint8_t)(fork_index + pass_branch)]);
		}
	}
}

/* Game step 7, an idle seat while another one's game takes entries (race_entries_update sends it here),
   by screen game_mode's byte 2: 0 screen fade_kind = 0 and unk_94 bit 0; 1 once that bit has cleared:
   the seat's sound channel (012004), the screen's objects (011C60), layers (01070C), sprite count
   (010889) and text layer (01076A) set afresh, its race record's race part cleared (race_record_clear),
   not the demo (unk_90 bit 8), the wheel motor off (wheel_motor), both of the seat's lamps blinking
   (bytes C0-C3 and C8-CB of its entry at checkpoint_lamp); 2 starts object_handlers_DF74
   (entry_countdown_start, credit_display_update, coin_prompt_update): the invitation to join with the
   entry time left; 3 the seat's screen on (its I/O chip's CNT, 6 or 86), unk_153 = 1, unk_94 bit 0; 4
   once player 0's entry_frames is over: the sound channel again (012004), step 2, the lamps off, the LCD
   cleared (clear_lcd). */
void entry_invitation_step(void)
{
	uint32_t ignored_2 = 0, index = 0, lamp = 0, object_list = 0;

	switch (SCREEN(screen)->game_step)
	{
	case 0:
		SCREEN(screen)->fade_kind = 0u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 1:
		break;
	case 2:
		SCREEN(screen)->game_step++;
		object_list = entry_invitation_objects_ADDRESS;
		ignored_2 = start_object_list(object_list);
		return;
	case 3:
		if (SCREEN(screen)->index != 0u)
			mem_write8(0xC8001Cu, 0x86u);
		else
			mem_write8(IO_C0001C, 6u);
		SCREEN(screen)->fade_kind = 1u;
		SCREEN(screen)->flags |= 1u;
		SCREEN(screen)->game_step++;
		return;
	case 4:
		if (RACE(RACE_COPY_1)->entry_frames != 0u)
			return;
		reset_screen_sound();
		AS32(SCREEN(screen)->game_mode) = 2u;
		lamp = GAME_VARIABLES + SCREEN(screen)->index * 4u;
		mem_write32(lamp + 0xC0u, 0u);
		mem_write32(lamp + 0xC8u, 0u);
		clear_lcd();
		return;
	default:
		invalid_jump(entry_invitation_step_jumps[SCREEN(screen)->game_step]);
	}
	if ((SCREEN(screen)->flags & 1u) == 0u)
	{
		SCREEN(screen)->game_step++;
		reset_screen_sound();
		clear_objects();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		race_record_clear();
		SCREEN(screen)->demo_flags &= ~0x100u;
		index = SCREEN(screen)->index;
		wheel_motor[index] = 0u;
		lamp = GAME_VARIABLES + index * 4u;
		mem_write8(lamp + 0xC0u, 2u);
		mem_write8(lamp + 0xC8u, 8u);
		mem_write8(lamp + 0xC1u, 0xFFu);
		mem_write8(lamp + 0xC2u, 0x10u);
		mem_write8(lamp + 0xC3u, 1u);
		mem_write8(lamp + 0xC9u, 0xFFu);
		mem_write8(lamp + 0xCAu, 0x10u);
		mem_write8(lamp + 0xCBu, 1u);
	}
}

/* Clears bytes 30-7F of the screen's race record (the race's part: the car's published state, the
   checkpoint passes, the times), from the zeros at 100800. */
void race_record_clear(void)
{
	move_string(zero_block_ADDRESS, 0x28u, SCREEN(screen)->race + 0x30u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Marks the screen's player ready in its race record (unk_5C), after its selections. */
void set_player_ready(void)
{
	RACE(SCREEN(screen)->race)->ready = 0xFFu;
}

/* Whether a player of the race has not marked itself ready yet (zero set): byte 5C of each one's
   race_copy record. */
uint32_t players_not_ready(void)
{
	uint32_t zero = 0;
	uint32_t race = 0, racers_left = 0;
	uint8_t ready = 0;

	race = SCREEN(screen)->host_record + 0x1Fu;
	racers_left = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		ready = RACE(RACE_COPY_1 + race_copy_player_offsets[mem_read8(race)])->ready;
		zero = ready == 0u;
		racers_left--;
		race++;
	} while ((racers_left != 0u) & (ready != 0u));
	return zero;
}

/* Takes the players' new course records, each frame: when a player's record_serial changes (against
   name_entries_seen), its time (finish_time) on its course (record_course) at or under the course's
   record (course_records, 6 bytes a course: model, the three letters of the name, the time) replaces it
   with its record_model and record_name (the first letter) and record_name_rest (the other two), also in
   the two copies in the backup (701F32 with its checksum, 011418, and 701FAE). */
void course_records_update(void)
{
	uint16_t finish_time = 0;
	uint32_t backup_record = 0, ignored = 0, players_left = 0, race = 0, record_offset = 0, seen_serials = 0;
	uint8_t seen_serial = 0;

	seen_serials = screen + 0x328u;
	players_left = player_count;
	race = RACE_2;
	do
	{
		race += 0x80u;
		seen_serial = mem_read8(seen_serials);
		seen_serials++;
		if (RACE(race)->record_serial != seen_serial)
		{
			mem_write8(seen_serials - 1u, RACE(race)->record_serial);
			finish_time = RACE(race)->finish_time;
			record_offset = RACE(race)->record_course;
			record_offset = (uint8_t)(record_offset * 6u);
			r12 = course_records_ADDRESS + record_offset;
			if (finish_time <= mem_read16(r12 + 4u))
			{
				mem_write16(r12 + 4u, finish_time);
				mem_write8(r12, RACE(race)->record_model);
				mem_write8(r12 + 1u, RACE(race)->record_name);
				mem_write16(r12 + 2u, RACE(race)->record_name_rest);
				backup_record = SOUND_RAM + 0x1F32u + record_offset;
				mem_write32(backup_record, mem_read32(r12));
				mem_write16(backup_record + 4u, mem_read16(r12 + 4u));
				bookkeeping_update_checksum(ignored);
				backup_record = SOUND_RAM + 0x1FAEu + record_offset;
				mem_write32(backup_record, mem_read32(r12));
				mem_write16(backup_record + 4u, mem_read16(r12 + 4u));
			}
		}
		players_left--;
	} while (players_left != 0u);
}

/* Takes the race records of all the players into 20FC00 (80 bytes each, by player), each frame: alone
   (link_off, link_failed) this machine's own two (20FB00, 20FB80); in ISDN mode the eight from 21E000;
   on the link the eight from the link board (link_copy_records_in from 800020, or 800720 when its byte
   80000E is set), or link_lost when its error byte 80000C is set. */
void link_receive_records(void)
{
	uint32_t destination = 0, pairs_left = 0, record_count = 0, source = 0;
	uint8_t board_error = 0, second_buffer = 0;
	uint32_t block_00 = 0, bytes_04 = 0, block_08 = 0, bytes_0C = 0, block_10 = 0, bytes_14 = 0, block_18 = 0, bytes_1C = 0, block_20 = 0, bytes_24 = 0, block_28 = 0, bytes_2C = 0, block_30 = 0, bytes_34 = 0, block_38 = 0, bytes_3C = 0, block_40 = 0, bytes_44 = 0, block_48 = 0, bytes_4C = 0, block_50 = 0, bytes_54 = 0, block_58 = 0, bytes_5C = 0, block_60 = 0, bytes_64 = 0, block_68 = 0, bytes_6C = 0, block_70 = 0, bytes_74 = 0, block_78 = 0, bytes_7C = 0, block_80 = 0, bytes_84 = 0, block_88 = 0, bytes_8C = 0, block_90 = 0, bytes_94 = 0, block_98 = 0, bytes_9C = 0, block_A0 = 0, bytes_A4 = 0, block_A8 = 0, bytes_AC = 0, block_B0 = 0, bytes_B4 = 0, block_B8 = 0, bytes_BC = 0, block_C0 = 0, bytes_C4 = 0, block_C8 = 0, bytes_CC = 0, block_D0 = 0, bytes_D4 = 0, block_D8 = 0, bytes_DC = 0, block_E0 = 0, bytes_E4 = 0, block_E8 = 0, bytes_EC = 0, block_F0 = 0, bytes_F4 = 0, block_F8 = 0, bytes_FC = 0;

	if (isdn_mode == 0u)
	{
		if (link_failed == 0u && link_off == 0u)
		{
			board_error = mem_read8(0x80000Cu);
			if (board_error != 0u)
			{
				link_lost();
				return;
			}
			source = 0x800020u;
			destination = RACE_COPY_1;
			record_count = 8u;
			second_buffer = mem_read8(0x80000Eu);
			if (second_buffer == 0u)
			{
				link_copy_records_in(record_count, source, destination);
				return;
			}
			source += 0x700u;
			link_copy_records_in(record_count, source, destination);
			return;
		}
		source = RACE_1;
		destination = RACE_COPY_1;
		pairs_left = 1u;
	}
	else
	{
		source = 0x21E000u;
		destination = RACE_COPY_1;
		pairs_left = 4u;
	}
	do
	{
		block_00 = destination;
		destination += 8u;
		bytes_04 = mem_read32(source + 4u);
		mem_write32(block_00, mem_read32(source));
		mem_write32(block_00 + 4u, bytes_04);
		source += 8u;
		block_08 = destination;
		destination += 8u;
		bytes_0C = mem_read32(source + 4u);
		mem_write32(block_08, mem_read32(source));
		mem_write32(block_08 + 4u, bytes_0C);
		source += 8u;
		block_10 = destination;
		destination += 8u;
		bytes_14 = mem_read32(source + 4u);
		mem_write32(block_10, mem_read32(source));
		mem_write32(block_10 + 4u, bytes_14);
		source += 8u;
		block_18 = destination;
		destination += 8u;
		bytes_1C = mem_read32(source + 4u);
		mem_write32(block_18, mem_read32(source));
		mem_write32(block_18 + 4u, bytes_1C);
		source += 8u;
		block_20 = destination;
		destination += 8u;
		bytes_24 = mem_read32(source + 4u);
		mem_write32(block_20, mem_read32(source));
		mem_write32(block_20 + 4u, bytes_24);
		source += 8u;
		block_28 = destination;
		destination += 8u;
		bytes_2C = mem_read32(source + 4u);
		mem_write32(block_28, mem_read32(source));
		mem_write32(block_28 + 4u, bytes_2C);
		source += 8u;
		block_30 = destination;
		destination += 8u;
		bytes_34 = mem_read32(source + 4u);
		mem_write32(block_30, mem_read32(source));
		mem_write32(block_30 + 4u, bytes_34);
		source += 8u;
		block_38 = destination;
		destination += 8u;
		bytes_3C = mem_read32(source + 4u);
		mem_write32(block_38, mem_read32(source));
		mem_write32(block_38 + 4u, bytes_3C);
		source += 8u;
		block_40 = destination;
		destination += 8u;
		bytes_44 = mem_read32(source + 4u);
		mem_write32(block_40, mem_read32(source));
		mem_write32(block_40 + 4u, bytes_44);
		source += 8u;
		block_48 = destination;
		destination += 8u;
		bytes_4C = mem_read32(source + 4u);
		mem_write32(block_48, mem_read32(source));
		mem_write32(block_48 + 4u, bytes_4C);
		source += 8u;
		block_50 = destination;
		destination += 8u;
		bytes_54 = mem_read32(source + 4u);
		mem_write32(block_50, mem_read32(source));
		mem_write32(block_50 + 4u, bytes_54);
		source += 8u;
		block_58 = destination;
		destination += 8u;
		bytes_5C = mem_read32(source + 4u);
		mem_write32(block_58, mem_read32(source));
		mem_write32(block_58 + 4u, bytes_5C);
		source += 8u;
		block_60 = destination;
		destination += 8u;
		bytes_64 = mem_read32(source + 4u);
		mem_write32(block_60, mem_read32(source));
		mem_write32(block_60 + 4u, bytes_64);
		source += 8u;
		block_68 = destination;
		destination += 8u;
		bytes_6C = mem_read32(source + 4u);
		mem_write32(block_68, mem_read32(source));
		mem_write32(block_68 + 4u, bytes_6C);
		source += 8u;
		block_70 = destination;
		destination += 8u;
		bytes_74 = mem_read32(source + 4u);
		mem_write32(block_70, mem_read32(source));
		mem_write32(block_70 + 4u, bytes_74);
		source += 8u;
		block_78 = destination;
		destination += 8u;
		bytes_7C = mem_read32(source + 4u);
		mem_write32(block_78, mem_read32(source));
		mem_write32(block_78 + 4u, bytes_7C);
		source += 8u;
		block_80 = destination;
		destination += 8u;
		bytes_84 = mem_read32(source + 4u);
		mem_write32(block_80, mem_read32(source));
		mem_write32(block_80 + 4u, bytes_84);
		source += 8u;
		block_88 = destination;
		destination += 8u;
		bytes_8C = mem_read32(source + 4u);
		mem_write32(block_88, mem_read32(source));
		mem_write32(block_88 + 4u, bytes_8C);
		source += 8u;
		block_90 = destination;
		destination += 8u;
		bytes_94 = mem_read32(source + 4u);
		mem_write32(block_90, mem_read32(source));
		mem_write32(block_90 + 4u, bytes_94);
		source += 8u;
		block_98 = destination;
		destination += 8u;
		bytes_9C = mem_read32(source + 4u);
		mem_write32(block_98, mem_read32(source));
		mem_write32(block_98 + 4u, bytes_9C);
		source += 8u;
		block_A0 = destination;
		destination += 8u;
		bytes_A4 = mem_read32(source + 4u);
		mem_write32(block_A0, mem_read32(source));
		mem_write32(block_A0 + 4u, bytes_A4);
		source += 8u;
		block_A8 = destination;
		destination += 8u;
		bytes_AC = mem_read32(source + 4u);
		mem_write32(block_A8, mem_read32(source));
		mem_write32(block_A8 + 4u, bytes_AC);
		source += 8u;
		block_B0 = destination;
		destination += 8u;
		bytes_B4 = mem_read32(source + 4u);
		mem_write32(block_B0, mem_read32(source));
		mem_write32(block_B0 + 4u, bytes_B4);
		source += 8u;
		block_B8 = destination;
		destination += 8u;
		bytes_BC = mem_read32(source + 4u);
		mem_write32(block_B8, mem_read32(source));
		mem_write32(block_B8 + 4u, bytes_BC);
		source += 8u;
		block_C0 = destination;
		destination += 8u;
		bytes_C4 = mem_read32(source + 4u);
		mem_write32(block_C0, mem_read32(source));
		mem_write32(block_C0 + 4u, bytes_C4);
		source += 8u;
		block_C8 = destination;
		destination += 8u;
		bytes_CC = mem_read32(source + 4u);
		mem_write32(block_C8, mem_read32(source));
		mem_write32(block_C8 + 4u, bytes_CC);
		source += 8u;
		block_D0 = destination;
		destination += 8u;
		bytes_D4 = mem_read32(source + 4u);
		mem_write32(block_D0, mem_read32(source));
		mem_write32(block_D0 + 4u, bytes_D4);
		source += 8u;
		block_D8 = destination;
		destination += 8u;
		bytes_DC = mem_read32(source + 4u);
		mem_write32(block_D8, mem_read32(source));
		mem_write32(block_D8 + 4u, bytes_DC);
		source += 8u;
		block_E0 = destination;
		destination += 8u;
		bytes_E4 = mem_read32(source + 4u);
		mem_write32(block_E0, mem_read32(source));
		mem_write32(block_E0 + 4u, bytes_E4);
		source += 8u;
		block_E8 = destination;
		destination += 8u;
		bytes_EC = mem_read32(source + 4u);
		mem_write32(block_E8, mem_read32(source));
		mem_write32(block_E8 + 4u, bytes_EC);
		source += 8u;
		block_F0 = destination;
		destination += 8u;
		bytes_F4 = mem_read32(source + 4u);
		mem_write32(block_F0, mem_read32(source));
		mem_write32(block_F0 + 4u, bytes_F4);
		source += 8u;
		block_F8 = destination;
		destination += 8u;
		bytes_FC = mem_read32(source + 4u);
		mem_write32(block_F8, mem_read32(source));
		mem_write32(block_F8 + 4u, bytes_FC);
		pairs_left--;
		source += 8u;
	} while (pairs_left != 0u);
}

/* ISDN mode: copies the eight race records from the link board (800600) to 21E000, where
   link_receive_records takes them; the link interrupt (int_vector_44) does it on the board's command 3. */
void link_isdn_receive_records(void)
{
	uint32_t board_records = 0, race_records = 0, record_count = 0;

	board_records = 0x800600u;
	race_records = 0x21E000u;
	record_count = 8u;
	link_copy_records_in(record_count, board_records, race_records);
}

/* Copies R1 race records from the link board's memory at R10 (a byte in each halfword) to R11: their
   first 70 bytes, 80 apart. */
void link_copy_records_in(uint32_t record_count, uint32_t board_records, uint32_t race_records)
{
	do
	{
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records++;
		board_records += 2u;
		mem_write8(race_records, (uint8_t)mem_read16(board_records));
		race_records += 0x11u;
		record_count--;
		board_records += 2u;
	} while (record_count != 0u);
}

/* Sends this machine's two race records to the link board, each frame (not in ISDN mode, nor alone):
   their first 70 bytes to 800E20 and 800F00 (a byte in each halfword), then FF to its byte 800006;
   link_lost when its error byte 80000C is set. link_wait_frames from link_wait_frames_start (which
   nothing sets). */
void link_send_records(void)
{
	uint32_t board_records = 0, race = 0, record_count = 0;
	uint8_t board_error = 0;

	if (isdn_mode == 0u)
	{
		link_wait_frames = link_wait_frames_start;
		if (link_failed != 0u || link_off != 0u)
			return;
		board_error = mem_read8(0x80000Cu);
		if (board_error != 0u)
		{
			link_lost();
			return;
		}
		race = RACE_COPY_1;
		board_records = 0x800FE0u;
		record_count = 2u;
		do
		{
			race = race - 0x10u - 1u;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x6Fu)->race_time >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x6Eu)->race_time >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x6Du)->race_time >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x6Cu)->race_time);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x6Bu)->time_left >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x6Au)->time_left);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x69u)->pass_time >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x68u)->pass_time);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x67u)->pass_branch);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x66u)->pass_order);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x65u)->pass_checkpoint);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x64u)->pass_player);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x63u)->pass_count);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x62u)->stage);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x61u)->pass_seen);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x60u)->stage_count);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Fu)->published);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Eu)->finished);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Du)->started);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Cu)->ready);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Bu)->unk_5B);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5Au)->unk_5A);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x59u)->car_flags >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x58u)->car_flags);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x57u)->bounce_height >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x56u)->bounce_height);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x55u)->segment_pitch >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x54u)->segment_pitch);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x53u)->hard_acceleration);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x52u)->wheel_effect_2);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x51u)->wheel_effect_1);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x50u)->wheel_effect_sides);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x4Fu)->sideways_speed >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x4Eu)->sideways_speed);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x4Du)->speed >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x4Cu)->speed);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x4Bu)->view_slot);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x4Au)->frame);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x49u)->model);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x48u)->car_choice);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x47u)->window_segment >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x46u)->window_segment);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x45u)->road_x >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x44u)->road_x);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x43u)->segment >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x42u)->segment);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x41u)->segment_fraction >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x40u)->segment_fraction);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x3Fu)->palette >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x3Eu)->palette);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x3Du)->animation >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x3Cu)->animation);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x3Bu)->world_z >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x3Au)->world_z >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x39u)->world_z >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x38u)->world_z);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x37u)->world_y >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x36u)->world_y >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x35u)->world_y >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x34u)->world_y);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x33u)->world_x >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x32u)->world_x >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x31u)->world_x >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x30u)->world_x);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Fu)->link_brake);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Eu)->link_gas);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Du)->link_wheel);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Cu)->link_buttons);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Bu)->link_status);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2Au)->players_other_side);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x29u)->players_this_side);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x28u)->game_over);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x27u)->race_order_4_7 >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x26u)->race_order_4_7 >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x25u)->race_order_4_7 >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x24u)->race_order_4_7);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x23u)->race_order_3);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x22u)->race_order_2);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x21u)->race_order_1);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x20u)->race_order);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x1Fu)->racer_count);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x1Eu)->entrant);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x1Du)->entry_serial);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x1Cu)->entry_serial_seen);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x1Bu)->entry_frames >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x1Au)->entry_frames);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x19u)->entry_request);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x18u)->record_course);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x17u)->finish_time >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x16u)->finish_time);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x15u)->record_name_rest >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0x14u)->record_name_rest);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x13u)->record_name);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x12u)->record_model);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x11u)->record_serial);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0x10u)->master_time_difficulty_2_5 >> 24u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0xFu)->master_time_difficulty_2_5 >> 16u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)(RACE(race - 0xEu)->master_time_difficulty_2_5 >> 8u));
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0xDu)->master_time_difficulty_2_5);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0xCu)->unk_0C);
			race--;
			board_records -= 2u;
			mem_write16(board_records, (uint8_t)RACE(race - 0xBu)->master_time_difficulty_0);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0xAu)->master_game_difficulty);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x9u)->master_free_play);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x8u)->master_coin_setting);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x7u)->master_bonus_adder);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x6u)->master_first_chute_multiplier);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x5u)->master_coins_per_credit);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x4u)->master_steering_wheel_reaction);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x3u)->master_advertise_sound);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x2u)->car_number);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race - 0x1u)->player);
			race--;
			board_records -= 2u;
			mem_write16(board_records, RACE(race)->machine);
			record_count--;
		} while (record_count != 0u);
		mem_write8(0x800006u, 0xFFu);
	}
}

/* Draws the object's sprite, unless flags bit 1 is set or the screen's sprite list is full: field 3E
   picks the routine (draw_object_mode_jumps): some project the object's road position (1C) with its
   offsets (20, 24) and set fields 30 and 32 to where it lands and 3C to the road's depth layer there,
   some draw it at the screen position in 30 and 32, some copy a ready sprite entry from the address in
   field 38; then links it into the depth list of layer 3C (screen + 5000). */
uint32_t draw_object(uint32_t routine_then_layer)
{
	uint32_t depth_list = 0, layer_head = 0, left_caller = 0, sprite_entry = 0;

	if ((OBJECT(current_object)->flags & 2u) != 0u)
		return routine_then_layer;
	routine_then_layer = draw_object_mode_jumps[OBJECT(current_object)->draw_mode];
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
		return routine_then_layer;
	left_caller = 0u;
	switch (routine_then_layer)
	{
	case 0x00EF32u:
		draw_object_on_screen(&routine_then_layer, &sprite_entry);
		break;
	case 0x00F104u:
		sprite_entry = draw_object_entry(sprite_entry);
		break;
	case 0x00EB74u:
		draw_object_between_segments(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EEBFu:
		draw_object_at_car_window_segment_on_road(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EE37u:
		draw_object_at_car_window_segment(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00EC1Bu:
		draw_object_from_road_edge(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F126u:
		sprite_entry = draw_object_command(sprite_entry);
		break;
	case 0x00ECCDu:
		draw_object_from_road_edge_unshifted(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F04Eu:
		draw_object_one_piece(&routine_then_layer, &sprite_entry);
		break;
	case 0x00EE75u:
		draw_object_at_segment(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	case 0x00F15Cu:
		draw_object_smoothed(&routine_then_layer, &sprite_entry);
		break;
	case 0x00ED82u:
		draw_object_from_road_edge_doubled(&routine_then_layer, &sprite_entry, &left_caller);
		break;
	default:
		invalid_jump(routine_then_layer);
	}
	if (left_caller != 0u)
		return routine_then_layer;
	routine_then_layer = OBJECT(current_object)->depth_layer;
	depth_list = screen + 0x5000u + routine_then_layer * 4u;
	if (mem_read16(depth_list) == 0u)
		mem_write16(depth_list + 2u, sprite_entry);
	r12 = SCREEN(screen)->sprite_list_last + 0x10u;
	layer_head = mem_read16(depth_list);
	mem_write32(sprite_entry, layer_head);
	mem_write32(sprite_entry + 0x3000u, (uint16_t)(layer_head + 0x300u));
	SCREEN(screen)->sprite_list_last = sprite_entry;
	r12 = r12 >> 4;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x8000u);
	mem_write16(depth_list, r12);
	SCREEN(screen)->sprite_groups++;
	return routine_then_layer;
}

/* Draw mode 2: the object between the road's segment at its position (field 1C, counted from the view
   drawn: race_copy window_segment) and the next, by the fraction in field 28 (80ths), offset across and
   up by fields 20 and 24 scaled by each segment's scale (draw_object_at_scale). Its size, meant as the
   two segments' scales by the fraction, multiplies them by the second point's offsets instead (R4 and R5
   used twice): a slip in the game's code; nothing in the scenarios draws with it. */
void draw_object_between_segments(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t near_weight = 0, next_scale = 0, next_x = 0, next_y = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0, y = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = mem_read16(projected_segment);
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)mem_read16(r12);
			if (next_scale != 0u)
			{
				x = (int32_t)(OBJECT(current_object)->x_offset * scale) >> 12;
				y = (int32_t)(OBJECT(current_object)->y_offset * scale) >> 12;
				next_x = (int32_t)(OBJECT(current_object)->x_offset * next_scale) >> 12;
				next_y = (int32_t)(OBJECT(current_object)->y_offset * next_scale) >> 12;
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				next_x = ((next_x & ~0xFFFFu) | (uint16_t)(next_x + mem_read16(r12 + 4u))) * position_fraction;
				x = (int32_t)(((x & ~0xFFFFu) | (uint16_t)(x + mem_read16(projected_segment + 4u))) * near_weight + next_x) >> 7;
				next_y = ((next_y & ~0xFFFFu) | (uint16_t)(next_y + mem_read16(r12 + 6u))) * position_fraction;
				y = (int32_t)(((y & ~0xFFFFu) | (uint16_t)(y + mem_read16(projected_segment + 6u))) * near_weight + next_y) >> 7;
				scale = (scale * next_x + next_scale * next_y) >> 7;
				draw_object_at_scale((uint16_t)y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
				*p_leftover = x;
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 5: the object on the road's surface between its segment (field 1C) and the next by the
   fraction in field 28, across at field 20 from the road's edge on side field 3F (the segment's half
   width times the side), its size the two segments' scales by the fraction shifted by scale_shift
   (draw_object_at_shifted_scale). */
void draw_object_from_road_edge(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)mem_read16(projected_segment);
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)mem_read16(r12);
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)mem_read16(screen + 0x2000u + (ring_offset & 0xFFFu) + 0x12u) * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + mem_read16(projected_segment + 4u)) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + mem_read16(r12 + 4u)) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)mem_read16(projected_segment + 6u) * near_weight + (int16_t)mem_read16(r12 + 6u) * position_fraction) >> 7);
				scale = (scale * near_weight + next_scale * position_fraction) >> 7;
				draw_object_at_shifted_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
				*p_leftover = x;
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 7: as draw_object_from_road_edge, at the size of scale_shift 0 whatever the object's. */
void draw_object_from_road_edge_unshifted(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)mem_read16(projected_segment);
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)mem_read16(r12);
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)mem_read16(screen + 0x2000u + (ring_offset & 0xFFFu) + 0x12u) * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + mem_read16(projected_segment + 4u)) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + mem_read16(r12 + 4u)) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)mem_read16(projected_segment + 6u) * near_weight + (int16_t)mem_read16(r12 + 6u) * position_fraction) >> 7);
				scale = (int32_t)(scale * near_weight + next_scale * position_fraction) >> 11;
				draw_object_at_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
				*p_leftover = x;
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode B (the traffic cars'): as draw_object_from_road_edge, at twice the size of
   draw_object_from_road_edge_unshifted (scale_shift 1) whatever the object's. */
void draw_object_from_road_edge_doubled(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t near_weight = 0, next_scale = 0, position = 0, position_fraction = 0, projected_offset = 0, projected_segment = 0, ring_offset = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	ring_offset = position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = (int16_t)mem_read16(projected_segment);
		if (scale != 0u)
		{
			projected_offset = (uint16_t)(projected_offset + 0x10u);
			r12 = screen + 0x3000u + projected_offset;
			next_scale = (int16_t)mem_read16(r12);
			if (next_scale != 0u)
			{
				ring_offset = (uint16_t)(ring_offset << 5);
				position_fraction = OBJECT(current_object)->position_fraction;
				near_weight = (uint16_t)(0x80u - position_fraction);
				x = OBJECT(current_object)->x_offset + (uint32_t)mem_read16(screen + 0x2000u + (ring_offset & 0xFFFu) + 0x12u) * OBJECT(current_object)->side;
				x = (int32_t)((int16_t)(((int32_t)(x * scale) >> 12) + mem_read16(projected_segment + 4u)) * near_weight + (int16_t)(((int32_t)(x * next_scale) >> 12) + mem_read16(r12 + 4u)) * position_fraction) >> 7;
				y = (uint16_t)((int32_t)((int16_t)mem_read16(projected_segment + 6u) * near_weight + (int16_t)mem_read16(r12 + 6u) * position_fraction) >> 7);
				scale = (int32_t)(scale * near_weight + next_scale * position_fraction) >> 10;
				draw_object_at_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
				*p_leftover = x;
				return;
			}
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 4: as draw_object_at_segment, but the segment counted from the screen car's own
   window_segment instead of the view drawn (race_copy window_segment). */
void draw_object_at_car_window_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = CAR(player_car)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = mem_read16(projected_segment);
		if (scale != 0u)
		{
			x = (int32_t)(OBJECT(current_object)->x_offset * scale) >> 12;
			x = (x & ~0xFFFFu) | (uint16_t)(x + mem_read16(projected_segment + 4u));
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * scale) >> 12) + mem_read16(projected_segment + 6u));
			draw_object_at_segment_scale(y, projected_segment, &x, p_sprite_entry, p_left_caller);
			*p_leftover = x;
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 9: the object at the road's segment of its position (field 1C, counted from the view drawn:
   race_copy window_segment), offset across and up by fields 20 and 24 scaled by the segment's scale, its
   size that scale shifted by scale_shift; not drawn when the segment is behind or not projected. */
void draw_object_at_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = mem_read16(projected_segment);
		if (scale != 0u)
		{
			x = (int32_t)(OBJECT(current_object)->x_offset * scale) >> 12;
			x = (x & ~0xFFFFu) | (uint16_t)(x + mem_read16(projected_segment + 4u));
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * scale) >> 12) + mem_read16(projected_segment + 6u));
			scale = shift_by(scale, OBJECT(current_object)->scale_shift, 32) >> 4;
			draw_object_at_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
			*p_leftover = x;
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draw mode 3: as draw_object_at_car_window_segment, on the road's surface (no up offset). */
void draw_object_at_car_window_segment_on_road(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint16_t y = 0;
	uint32_t position = 0, projected_offset = 0, projected_segment = 0, scale = 0, x = 0;
	uint16_t window_segment = 0, segment = 0;

	position = (uint16_t)OBJECT(current_object)->position;
	window_segment = CAR(player_car)->window_segment;
	segment = position;
	projected_offset = (uint16_t)(segment - window_segment);
	if ((int16_t)window_segment <= (int16_t)segment)
	{
		projected_offset = (uint16_t)(projected_offset * 0x10u);
		projected_segment = screen + 0x3000u + projected_offset;
		scale = mem_read16(projected_segment);
		if (scale != 0u)
		{
			x = (int32_t)(OBJECT(current_object)->x_offset * scale) >> 12;
			x = (x & ~0xFFFFu) | (uint16_t)(x + mem_read16(projected_segment + 4u));
			y = mem_read16(projected_segment + 6u);
			draw_object_at_segment_scale(y, projected_segment, &x, p_sprite_entry, p_left_caller);
			*p_leftover = x;
			return;
		}
	}
	*p_left_caller = 1u;
	*p_leftover = projected_offset;
}

/* Draws the object at R0, R1 at the projected segment R11's own scale (draw_object_at_shifted_scale). */
void draw_object_at_segment_scale(uint16_t y, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;
	uint32_t scale = 0;

	scale = mem_read16(projected_segment);
	draw_object_at_shifted_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Draws the object at R0, R1 at the scale R2 shifted by its scale_shift (and >> 4)
   (draw_object_at_scale). */
void draw_object_at_shifted_scale(uint16_t y, uint32_t scale, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;

	scale = shift_by(scale, OBJECT(current_object)->scale_shift, 32) >> 4;
	draw_object_at_scale(y, scale, projected_segment, &x, p_sprite_entry, p_left_caller);
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Places a road object projected at R0, R1 with size R2 (both scales): x and y; off the screen (x beyond
   1A0 or -1A0, y beyond 80 or -160) back to draw_object's caller undrawn; the depth_layer of the
   projected segment R11 (+8), then put_object_sprites. */
void draw_object_at_scale(uint16_t y, uint32_t scale, uint32_t projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller)
{
	uint32_t x = *p_x;
	uint32_t width_scale = 0;

	OBJECT(current_object)->x = x;
	OBJECT(current_object)->y = y;
	if ((int16_t)x > 0x1A0 || (int16_t)x < -0x1A0 || (int16_t)y > 0x80 || (int16_t)y < -0x160)
		*p_left_caller = 1u;
	else
	{
		width_scale = scale;
		OBJECT(current_object)->depth_layer = mem_read16(projected_segment + 8u);
		put_object_sprites(y, scale, width_scale, &x, p_sprite_entry);
	}
	*p_x = (*p_x & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Draw mode 0: the object at x, y on the screen, less the screen's scroll (screen sprite_origin_x,
   unk_16), at its width_scale and height_scale (put_object_sprites). */
void draw_object_on_screen(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint16_t y = 0;
	uint32_t height_scale = 0, width_scale = 0, x = 0;

	x = (uint16_t)(OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x);
	y = (uint16_t)(OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y);
	height_scale = OBJECT(current_object)->height_scale;
	width_scale = OBJECT(current_object)->width_scale;
	put_object_sprites(y, height_scale, width_scale, &x, p_sprite_entry);
	*p_leftover = (*p_leftover & 0xFFFF0000u) | (x & 0xFFFFu);
}

/* Writes the object's frame (animation, frame) into the screen's sprite list at R10 and its copy at +
   3000, a sprite of 8 halfwords for each piece: its word flipped by flags bits 6-7, its size from
   0x122B74 by size_step scaled by R2 (height) and R3 (width), placed at R0, R1 plus its offsets scaled
   the same way (signs flipped by the flags: sprite_flip_x_signs, sprite_flip_y_signs), its palette word
   from sprite_palettes by palette; R10 past them. */
void put_object_sprites(uint16_t y, uint32_t height_scale, uint32_t width_scale, uint32_t *p_x, uint32_t *p_sprite_entry)
{
	uint16_t x = (uint16_t)*p_x;
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t attributes = 0, flip_bits = 0, frame_x = 0, frame_y = 0, sprite_height = 0, sprite_palette_word = 0, sprite_rom_offset = 0, sprite_source_size = 0, sprite_width = 0, sprite_x = 0, sprite_y = 0;
	uint32_t flip_index = 0, frame_height_scale = 0, frame_width_scale = 0, palette_words = 0, piece_count = 0, signed_height_scale = 0, signed_width_scale = 0, size_step_offset = 0;
	uint16_t source_size = 0, rom_offset = 0, palette_word = 0, height = 0, height_bits = 0, width = 0, width_bits = 0;

	frame_x = (uint16_t)x;
	frame_y = (uint16_t)y;
	frame_height_scale = height_scale;
	frame_width_scale = width_scale;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame];
	palette_words = sprite_palettes[OBJECT(current_object)->palette];
	flip_bits = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u)) & 0xC0u;
	flip_index = flip_bits;
	flip_index = flip_index >> 5;
	signed_width_scale = frame_width_scale * sprite_flip_x_signs[flip_index];
	signed_height_scale = frame_height_scale * sprite_flip_y_signs[flip_index];
	piece_count = SPRITE_FRAME(r12)->piece_count;
	r12 = r12 + 2u;
	do
	{
		attributes = SPRITE_PIECE(r12)->attributes ^ flip_bits;
		size_step_offset = OBJECT(current_object)->size_step * 3u;
		r12 = r12 + 2u;
		r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
		source_size = SPRITE_SIZE_STEP(r14)->source_size;
		r14 = r14 + 2u;
		sprite_source_size = source_size;
		rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
		r14 = r14 + 2u;
		sprite_rom_offset = rom_offset;
		palette_word = SPRITE_PALETTE_WORDS(palette_words)[0];
		palette_words += 2u;
		sprite_palette_word = palette_word;
		height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
		r14 = r14 + 2u;
		height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
		r14 = r14 + 2u;
		sprite_height = (((height * frame_height_scale) >> 8) & 0x3FFu) | height_bits;
		width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
		r14 = r14 + 2u;
		width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
		r14 = r14 + 2u;
		sprite_width = (((width * frame_width_scale) >> 8) & 0x3FFu) | width_bits | SCREEN(screen)->sprite_screen_bit;
		r12 = r12 + 2u;
		sprite_y = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset * signed_height_scale) >> 8) + frame_y);
		r12 = r12 + 2u;
		sprite_x = (uint16_t)(((int32_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset * signed_width_scale) >> 8) + frame_x);
		mem_write16(sprite_entry + 0x3000u, attributes);
		mem_write16(sprite_entry, attributes);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_source_size);
		mem_write16(sprite_entry, sprite_source_size);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_height);
		mem_write16(sprite_entry, sprite_height);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_width);
		mem_write16(sprite_entry, sprite_width);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_y);
		mem_write16(sprite_entry, sprite_y);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_x);
		mem_write16(sprite_entry, sprite_x);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_rom_offset);
		mem_write16(sprite_entry, sprite_rom_offset);
		sprite_entry += 2u;
		mem_write16(sprite_entry + 0x3000u, sprite_palette_word);
		mem_write16(sprite_entry, sprite_palette_word);
		sprite_entry += 2u;
		piece_count--;
		r12 = r12 + 2u;
	} while (piece_count != 0u);
	*p_x = (*p_x & 0xFFFF0000u) | attributes;
	*p_sprite_entry = sprite_entry;
}

/* Draw mode 8: the first piece of the animation's frame at its own size (no zoom, no flip), at x, y on
   the screen (less sprite_origin_x, sprite_origin_y), with the first word of its palette
   (sprite_palettes), into the sprite list and its halfway copy alike. */
void draw_object_one_piece(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t attributes = 0, palette_word = 0, rom_offset = 0, source_size = 0, sprite_height = 0, sprite_width = 0, sprite_x = 0, sprite_y = 0;
	uint32_t size_step_offset = 0;

	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	attributes = SPRITE_PIECE(r12)->attributes;
	size_step_offset = OBJECT(current_object)->size_step * 3u;
	r12 = r12 + 2u;
	r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
	source_size = SPRITE_SIZE_STEP(r14)->source_size;
	r14 = r14 + 2u;
	rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
	palette_word = SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0];
	r14 = r14 + 2u;
	sprite_height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
	r14 = r14 + 2u;
	sprite_height |= SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
	r14 = r14 + 2u;
	sprite_width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
	r14 = r14 + 2u;
	sprite_width = sprite_width | SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits | SCREEN(screen)->sprite_screen_bit;
	r12 = r12 + 2u;
	sprite_y = (uint16_t)(SPRITE_PIECE(r12 - 0x4u)->y_offset + (OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y));
	r12 = r12 + 2u;
	sprite_x = (uint16_t)(SPRITE_PIECE(r12 - 0x6u)->x_offset + (OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x));
	mem_write16(sprite_entry + 0x3000u, attributes);
	mem_write16(sprite_entry, attributes);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, source_size);
	mem_write16(sprite_entry, source_size);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_height);
	mem_write16(sprite_entry, sprite_height);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_width);
	mem_write16(sprite_entry, sprite_width);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_y);
	mem_write16(sprite_entry, sprite_y);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, sprite_x);
	mem_write16(sprite_entry, sprite_x);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, rom_offset);
	mem_write16(sprite_entry, rom_offset);
	sprite_entry += 2u;
	mem_write16(sprite_entry + 0x3000u, palette_word);
	mem_write16(sprite_entry, palette_word);
	sprite_entry += 2u;
	r12 = r12 + 2u;
	r14 = r14 + 2u;
	*p_leftover = (*p_leftover & 0xFFFF0000u) | attributes;
	*p_sprite_entry = sprite_entry;
}

/* Draw mode 1: copies a ready 16-byte sprite entry, from the address in fields 38-3B (animation and
   palette read as one 32-bit address), into the sprite list and its halfway copy, with the screen's bit
   in its fourth word (sprite_screen_bit). Not run in the scenarios. */
uint32_t draw_object_entry(uint32_t sprite_entry)
{
	move_halfwords(sprite_entry, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	move_string((OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u, sprite_entry + 0x3000u, 8u, 2u, 0u, 0u, 0u, &r27, &r28);
	mem_write16(sprite_entry + 6u, mem_read16(sprite_entry + 6u) | SCREEN(screen)->sprite_screen_bit);
	mem_write16(sprite_entry + 0x3006u, mem_read16(sprite_entry + 0x3006u) | SCREEN(screen)->sprite_screen_bit);
	sprite_entry += 0x10u;
	return sprite_entry;
}

/* Draw mode 6: copies a ready 16-byte entry from the address in fields 38-3B into the sprite list and
   its halfway copy as it is, without the screen's bit: for list commands such as crest_clip_update's
   clip. */
uint32_t draw_object_command(uint32_t sprite_entry)
{
	move_halfwords(sprite_entry, (OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u);
	move_string((OBJECT(current_object)->animation | ((uint32_t)OBJECT(current_object)->palette << 16u)), 8u, sprite_entry + 0x3000u, 8u, 2u, 0u, 0u, 0u, &r27, &r28);
	sprite_entry += 0x10u;
	return sprite_entry;
}

/* Draw mode A: every piece of the animation's frame at x, y on the screen (less sprite_origin_x,
   sprite_origin_y), zoomed by width_scale and height_scale, flipped by flags bits 6-7 (with bit 7 the
   pieces' offsets are negated, smoothed_flip_signs), each with its palette word; the halfway copy
   (+3000) is drawn from the averages of this frame's x, y, width_scale and height_scale and the last
   frame's, kept in the ring of sprite places (next_sprite_place, 16 bytes a record; the object's field
   50 points at its record) when flags bit 5 says it was drawn the frame before. Keeps this frame's
   values in a new record and sets flags bit 5. */
void draw_object_smoothed(uint32_t *p_leftover, uint32_t *p_sprite_entry)
{
	uint32_t sprite_entry = *p_sprite_entry;
	uint16_t flip_bits = 0, halfway_width = 0, halfway_x = 0, piece_height_bits = 0, piece_width_bits = 0, sprite_rom_offset = 0, sprite_screen_bit = 0, sprite_source_size = 0;
	uint32_t copy_entry = 0, flip_index = 0, halfway_height_scale = 0, halfway_width_scale = 0, halfway_y = 0, height_scale = 0, offset_sign = 0, offset_x = 0, offset_y = 0, palette_words = 0, piece_height = 0, piece_width = 0, size_step_offset = 0, width_scale = 0, x = 0, y = 0;
	uint16_t source_size = 0, rom_offset = 0, height = 0, height_bits = 0, width = 0, width_bits = 0, palette_word = 0;
	uint32_t flags = 0, flags_long = 0;

	r28 = OBJECT(current_object)->previous_place;
	r27 = SCREEN(screen)->next_sprite_place;
	x = (uint32_t)(int16_t)OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x;
	y = (uint32_t)(int16_t)OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y;
	width_scale = OBJECT(current_object)->width_scale;
	height_scale = OBJECT(current_object)->height_scale;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->unk_02 << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->unk_02 = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) == 0u)
	{
		halfway_x = (uint16_t)x;
		halfway_y = y;
		halfway_width_scale = width_scale;
		halfway_height_scale = height_scale;
	}
	else
	{
		halfway_height_scale = mem_read16(r28);
		r28 = r28 + 2u;
		halfway_width_scale = mem_read16(r28);
		r28 = r28 + 2u;
		halfway_y = (int16_t)mem_read16(r28);
		r28 = r28 + 2u;
		halfway_x = (uint16_t)((int32_t)((int16_t)mem_read16(r28) + x) >> 1);
		halfway_y = (int32_t)(halfway_y + y) >> 1;
		halfway_width_scale = (halfway_width_scale + width_scale) >> 1;
		halfway_height_scale = (halfway_height_scale + height_scale) >> 1;
	}
	OBJECT(current_object)->previous_place = r27;
	mem_write16(r27, height_scale);
	r27 = r27 + 2u;
	mem_write16(r27, width_scale);
	r27 = r27 + 2u;
	mem_write16(r27, y);
	r27 = r27 + 2u;
	mem_write16(r27, x);
	r27 = r27 + 2u;
	r27 = r27 + 8u;
	r27 = (r27 & ~0xFFFFu) | (r27 & 0xEFFFu);
	SCREEN(screen)->next_sprite_place = r27;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame];
	palette_words = sprite_palettes[OBJECT(current_object)->palette];
	flip_bits = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u)) & 0xC0u;
	flip_index = flip_bits;
	offset_sign = smoothed_flip_signs[flip_index >> 5];
	r28 = SPRITE_FRAME(r12)->piece_count;
	copy_entry = sprite_entry + 0x3000u;
	r12 = r12 + 2u;
	do
	{
		r27 = (r27 & ~0xFFFFu) | SPRITE_PIECE(r12)->attributes;
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 ^ flip_bits);
		mem_write16(sprite_entry, r27);
		sprite_entry += 2u;
		mem_write16(copy_entry, r27);
		copy_entry += 2u;
		size_step_offset = OBJECT(current_object)->size_step * 3u;
		r12 = r12 + 2u;
		r14 = sprite_size_steps_ADDRESS + (uint16_t)(size_step_offset + SPRITE_PIECE(r12 - 0x2u)->sprite) * 4u;
		source_size = SPRITE_SIZE_STEP(r14)->source_size;
		r14 = r14 + 2u;
		sprite_source_size = source_size;
		mem_write16(sprite_entry, sprite_source_size);
		sprite_entry += 2u;
		mem_write16(copy_entry, sprite_source_size);
		copy_entry += 2u;
		rom_offset = SPRITE_SIZE_STEP(r14 - 0x2u)->rom_offset;
		r14 = r14 + 2u;
		sprite_rom_offset = rom_offset;
		height = SPRITE_SIZE_STEP(r14 - 0x4u)->height;
		r14 = r14 + 2u;
		piece_height = height;
		height_bits = SPRITE_SIZE_STEP(r14 - 0x6u)->height_bits;
		r14 = r14 + 2u;
		piece_height_bits = height_bits;
		mem_write16(sprite_entry, (((piece_height * height_scale) >> 8) & 0x3FFu) | piece_height_bits);
		sprite_entry += 2u;
		mem_write16(copy_entry, (((piece_height * halfway_height_scale) >> 8) & 0x3FFu) | piece_height_bits);
		copy_entry += 2u;
		width = SPRITE_SIZE_STEP(r14 - 0x8u)->width;
		r14 = r14 + 2u;
		piece_width = width;
		halfway_width = ((piece_width * halfway_width_scale) >> 8) & 0x3FFu;
		width_bits = SPRITE_SIZE_STEP(r14 - 0xAu)->width_bits;
		r14 = r14 + 2u;
		piece_width_bits = width_bits;
		piece_width = (((piece_width * width_scale) >> 8) & 0x3FFu) | piece_width_bits;
		halfway_width |= piece_width_bits;
		sprite_screen_bit = SCREEN(screen)->sprite_screen_bit;
		mem_write16(sprite_entry, piece_width | sprite_screen_bit);
		sprite_entry += 2u;
		mem_write16(copy_entry, halfway_width | sprite_screen_bit);
		copy_entry += 2u;
		r12 = r12 + 2u;
		offset_y = SPRITE_PIECE(r12 - 0x4u)->y_offset * offset_sign;
		mem_write16(sprite_entry, ((int32_t)(offset_y * height_scale) >> 8) + y);
		sprite_entry += 2u;
		mem_write16(copy_entry, ((int32_t)(offset_y * halfway_height_scale) >> 8) + halfway_y);
		copy_entry += 2u;
		r12 = r12 + 2u;
		offset_x = SPRITE_PIECE(r12 - 0x6u)->x_offset * offset_sign;
		mem_write16(sprite_entry, ((int32_t)(offset_x * width_scale) >> 8) + x);
		sprite_entry += 2u;
		mem_write16(copy_entry, (uint32_t)((int32_t)(offset_x * halfway_width_scale) >> 8) + halfway_x);
		copy_entry += 2u;
		mem_write16(sprite_entry, sprite_rom_offset);
		sprite_entry += 2u;
		mem_write16(copy_entry, sprite_rom_offset);
		copy_entry += 2u;
		palette_word = SPRITE_PALETTE_WORDS(palette_words)[0];
		palette_words += 2u;
		r27 = (r27 & ~0xFFFFu) | palette_word;
		mem_write16(sprite_entry, r27);
		sprite_entry += 2u;
		mem_write16(copy_entry, r27);
		copy_entry += 2u;
		r28 = r28 - 1u;
		r12 = r12 + 2u;
	} while (r28 != 0u);
	*p_leftover = x;
	*p_sprite_entry = sprite_entry;
}

/* Steps the object's animation: nothing while frame_time is 0; when frame_countdown runs out (back to
   frame_time), the next frame. A frame's entry in the record (32 bits at +4 + frame * 4) is the address
   of its data when positive, else an order: 0 holds the frame before (frame_time 0), -1 goes back to
   frame 0, -2 starts the animation whose number is the next entry. */
uint32_t animation_advance(uint32_t leftover)
{
	uint32_t frame = 0, next_animation = 0, order_routine = 0, sprite_animation = 0;
	uint8_t frame_countdown = 0;
	uint32_t order = 0;

	if (OBJECT(current_object)->frame_time == 0u)
		return leftover;
	frame_countdown = OBJECT(current_object)->frame_countdown;
	OBJECT(current_object)->frame_countdown = frame_countdown - 1u;
	if ((int8_t)frame_countdown > 1)
		return leftover;
	OBJECT(current_object)->frame_countdown = OBJECT(current_object)->frame_time;
	OBJECT(current_object)->frame++;
	frame = OBJECT(current_object)->frame;
	sprite_animation = sprite_animations[OBJECT(current_object)->animation];
	if (SPRITE_ANIMATION(sprite_animation)->frames[frame] > 0)
		return leftover;
	order = 0u - SPRITE_ANIMATION(sprite_animation)->frames[frame];
	order_routine = animation_order_jumps[order];
	switch (order)
	{
	case 0:
		OBJECT(current_object)->frame_time = 0u;
		OBJECT(current_object)->frame--;
		return order_routine;
	case 2:
		next_animation = SPRITE_ANIMATION(sprite_animation)->frames[frame + 1u];
		animation_start(next_animation);
		return order_routine;
	case 1:
		OBJECT(current_object)->frame = 0u;
		return order_routine;
	default:
		invalid_jump(animation_order_jumps[order]);
	}
}

/* Starts animation R9 (sprite_animations[R9] is its record) at its first frame: frame_time and
   frame_countdown from the record's first byte, size_step 0, and fields 34 and 36 to 100 (the width and
   height zoom of the screen-position drawing, draw_object_on_screen: 100 the sprite's own size). */
void animation_start(uint32_t animation)
{
	uint8_t frame_time = 0;

	OBJECT(current_object)->animation = animation;
	frame_time = SPRITE_ANIMATION(sprite_animations[animation])->frame_time;
	OBJECT(current_object)->frame_time = frame_time;
	OBJECT(current_object)->frame_countdown = frame_time;
	OBJECT(current_object)->size_step = 0u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->frame = 0u;
}

/* Finishes the screen's sprite list in the vertical blank (int_vector_40): picks a record of
   sprite_list_switches by the screen (index) and sprite_list_buffers; when groups were drawn
   (sprite_groups), chains the depth lists from layer 0 to 415 (depth_lists: the list's start entry jumps
   to the first layer's newest group, each layer's tail to the next layer's head; the heads cleared), in
   the list and its halfway copy, and ends the chain with the record's word (a jump on to sprite entry 3,
   screen 2's list, after screen 1's; entry 4, the end, after screen 2's); points sprite RAM entries 2
   and 3 (the jumps to each screen's list) at the halfway copy of the list just finished and at the other
   screen's finished list; then the next buffer (sprite_list_buffers, sprite_list_last,
   sprite_list_limit) and sprite_groups 0. */
void finish_sprite_list(void)
{
	uint16_t copy_distance = 0, depth_list_size = 0;
	uint32_t chain_tail = 0, cleared_head = 0, depth_list = 0, empty_head = 0, layer_head = 0, layer_octets = 0, sprite_list_switch = 0, switch_index = 0;
	uint16_t new_head = 0, new_head_2 = 0, new_head_3 = 0, new_head_4 = 0, new_head_5 = 0, new_head_6 = 0, new_head_7 = 0, new_head_8 = 0;

	switch_index = SCREEN(screen)->index;
	switch_index = (uint8_t)(switch_index * 2u);
	switch_index = switch_index | (uint8_t)sprite_list_buffers;
	switch_index = (uint8_t)(switch_index * 2u);
	sprite_list_switch = sprite_list_switches[switch_index | (uint8_t)(sprite_list_buffers >> 8u)];
	chain_tail = SPRITE_LIST_SWITCH(sprite_list_switch)->list;
	sprite_list_switch += 4u;
	if (SCREEN(screen)->sprite_groups != 0u)
	{
		depth_list = screen + 0x5000u;
		layer_octets = 0x34u;
		empty_head = 0u;
		cleared_head = 0u;
		copy_distance = 0x300u;
		depth_list_size = 4u;
		do
		{
			if (mem_read16(depth_list) != 0u)
			{
				new_head = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_2 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_2);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_3 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_3);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_4 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_4);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_5 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_5);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_6 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_6);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_7 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_7);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			if (mem_read16(depth_list) != 0u)
			{
				new_head_8 = cleared_head;
				layer_head = mem_read16(depth_list);
				mem_write16(depth_list, new_head_8);
				mem_write32(chain_tail, layer_head);
				mem_write32(chain_tail + 0x3000u, (uint16_t)(layer_head + copy_distance));
				chain_tail = (chain_tail & ~0xFFFFu) | mem_read16(depth_list + 2u);
				cleared_head = empty_head;
			}
			depth_list = (depth_list & ~0xFFFFu) | (uint16_t)(depth_list + depth_list_size);
			layer_octets--;
		} while (layer_octets != 0u);
	}
	mem_write16(chain_tail, SPRITE_LIST_SWITCH(sprite_list_switch - 0x4u)->end_word);
	mem_write16(chain_tail + 0x3000u, SPRITE_LIST_SWITCH(sprite_list_switch - 0x4u)->end_word);
	sprite_list_switch += 2u;
	mem_write16(SPRITE_RAM_400020, SPRITE_LIST_SWITCH(sprite_list_switch - 0x6u)->entry_2_jump);
	sprite_list_switch += 2u;
	mem_write16(SPRITE_RAM_400030, SPRITE_LIST_SWITCH(sprite_list_switch - 0x8u)->entry_3_jump);
	sprite_list_switch += 2u;
	sprite_list_buffers = SPRITE_LIST_SWITCH(sprite_list_switch - 0xAu)->next_buffers;
	sprite_list_switch += 2u;
	SCREEN(screen)->sprite_list_last = SPRITE_LIST_SWITCH(sprite_list_switch - 0xCu)->next_list;
	SCREEN(screen)->sprite_list_limit = SPRITE_LIST_SWITCH(sprite_list_switch - 0xCu)->next_limit;
	SCREEN(screen)->sprite_groups = 0u;
}

/* Draws the row's items in view: ends the object once end_position is not past the view's start
   (race_copy window_segment); nothing while first_position is beyond the draw distance
   (SCREEN(screen)->unk_12 >> 7 segments); moves first_position up to the first item in view; for each
   item (spacing apart, to the draw distance) where the road has a scale: x = (the road's width * side +
   x_offset) * scale >> 12 plus the road's screen x unless screen_centered, y = y_offset * scale >> 12
   plus the road's screen y; off the screen (x beyond -1E0..1E0, y beyond -160..80) it is skipped, else
   the scale shifted by scale_shift picks the size (size_table: a size step and a shift) and a sprite of
   the animation's frame with its palette (flags bits 6-7 flip it) goes into the screen's sprite list
   (unk_C0; the copy at +3000 is the halfway point from the item's sprite of the frame before,
   previous_sprites) and the depth list of its road segment (screen + 5000). Sets flags bit 5 and keeps
   where its sprites went in previous_sprites. Used by scenery, fork_sign, animated_scenery,
   flashing_scenery and overhead_scenery (and 01351E). */
void road_row_draw(uint32_t rom_offset_and_palette, uint32_t piece_width)
{
	uint16_t places_mask = 0, previous_word = 0, sprite_width = 0, view_end = 0, x = 0, y = 0;
	uint32_t attributes_and_size = 0, blank_place = 0, depth_list = 0, first_position = 0, item_count = 0, item_position = 0, items_left = 0, layer_head = 0, palette_word = 0, positions_moved = 0, projected_segment = 0, projected_step = 0, ring_segment = 0, ring_step = 0, size = 0, size_index = 0, size_steps = 0, size_table = 0, spacing = 0, sprite_entry = 0, sprite_size_step = 0, sprite_then_height_bits = 0, window_then_width_bits = 0;
	uint16_t items_in_view = 0;
	uint32_t flags = 0, flags_long = 0, previous_first_position = 0;

	item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	item_count = (uint16_t)OBJECT(current_object)->road_row.end_position;
	window_then_width_bits = RACE(SCREEN(screen)->race_copy)->window_segment;
	view_end = (uint16_t)(window_then_width_bits + (SCREEN(screen)->draw_distance >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)item_count <= (int16_t)window_then_width_bits)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	if ((int16_t)view_end <= (int16_t)item_position)
		return;
	while ((int16_t)item_position < (int16_t)window_then_width_bits)
	{
		item_position = (uint16_t)(item_position + spacing);
	}
	if ((int16_t)view_end < (int16_t)item_count)
		item_count = view_end;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = item_position;
	item_count = (uint16_t)(item_count - item_position);
	item_count = (uint16_t)(item_count + spacing);
	item_count = (uint16_t)(item_count - 1u);
	items_in_view = divide_u16(item_count, spacing);
	item_count = items_in_view;
	if ((int16_t)items_in_view <= 0)
		return;
	r28 = 0x100000u;
	r27 = SCREEN(screen)->next_sprite_place;
	places_mask = 0xEFFFu;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->unk_02 << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->unk_02 = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) != 0u)
	{
		r28 = OBJECT(current_object)->road_row.previous_sprites;
		previous_first_position = first_position;
		positions_moved = previous_first_position - item_position;
		if (previous_first_position != item_position)
		{
			r28 = r28 - (divide_s32(positions_moved, spacing) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & places_mask);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = r27;
	ring_segment = screen + 0x2000u + ((item_position << 5) & 0xFFFu);
	projected_segment = screen + 0x3000u + ((item_position - window_then_width_bits) << 4);
	projected_step = spacing << 4;
	ring_step = spacing << 5;
	items_left = item_count;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	palette_word = (uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0] << 16;
	attributes_and_size = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
	attributes_and_size = attributes_and_size & 0xC0u;
	attributes_and_size = attributes_and_size ^ SPRITE_PIECE(r12)->attributes;
	r12 = r12 + 2u;
	sprite_then_height_bits = SPRITE_PIECE(r12 - 0x2u)->sprite;
	size_steps = sprite_size_steps_ADDRESS + sprite_then_height_bits * 4u;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		size = mem_read16(projected_segment);
		if (size != 0u && size <= 0x1200u)
		{
			x = (uint16_t)((int32_t)(((uint32_t)mem_read16(ring_segment + 0x12u) * OBJECT(current_object)->side + OBJECT(current_object)->x_offset) * size) >> 12);
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * size) >> 12) + mem_read16(projected_segment + 6u));
			if (OBJECT(current_object)->road_row.screen_centered == 0u)
				x += mem_read16(projected_segment + 4u);
			if ((int16_t)x > 0x1E0 || (int16_t)x < -0x1E0 || (int16_t)y > 0x80 || (int16_t)y < -0x160)
			{
				mem_write32(r27 + 4u, 0xF800F800u);
				ring_segment += ring_step;
				projected_segment += projected_step;
				ring_segment = (ring_segment & ~0xFFFFu) | (ring_segment & 0xAFFFu);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
				r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
				r28 = (r28 & ~0xFFFFu) | (r28 & places_mask);
				items_left--;
				if (items_left != 0u)
					continue;
				SCREEN(screen)->sprite_list_last = sprite_entry;
				break;
			}
			size = shift_by(size, OBJECT(current_object)->scale_shift, 32);
			size_index = size;
			if (size_index >= 0x1000u)
				size_index = 0xFFFu;
			size_index = size_index >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			size = shift_by(size, (uint8_t)ROAD_ROW_SIZE_SHIFTS(road_row_size_shift_lists[size_table])[size_index], 32);
			sprite_size_step = ROAD_ROW_SIZE_STEPS(road_row_size_step_lists[size_table])[size_index];
			if (sprite_size_step != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = sprite_size_step;
				sprite_size_step = size_steps + sprite_size_step * 3u * 4u;
				attributes_and_size = (attributes_and_size & 0xFFFFu) | ((uint32_t)SPRITE_SIZE_STEP(sprite_size_step)->source_size << 16);
				sprite_size_step += 2u;
				rom_offset_and_palette = SPRITE_SIZE_STEP(sprite_size_step - 0x2u)->rom_offset;
				sprite_size_step += 2u;
				r14 = SPRITE_SIZE_STEP(sprite_size_step - 0x4u)->height;
				sprite_size_step += 2u;
				sprite_then_height_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x6u)->height_bits;
				sprite_size_step += 2u;
				piece_width = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width;
				window_then_width_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width_bits | SCREEN(screen)->sprite_screen_bit;
				rom_offset_and_palette |= palette_word;
			}
			sprite_width = (uint16_t)((size * piece_width) >> 8);
			size = ((size * r14) >> 8) & 0x3FFu;
			sprite_width &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
				break;
			mem_write32(sprite_entry + 0x3000u, attributes_and_size);
			mem_write32(sprite_entry, attributes_and_size);
			mem_write16(r27 + 8u, rom_offset_and_palette);
			sprite_entry += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)rom_offset_and_palette)
			{
				previous_word = mem_read16(r28);
				mem_write16(r27, size);
				mem_write16(sprite_entry + 0x3000u, ((uint16_t)(previous_word + size) >> 1) | sprite_then_height_bits);
				mem_write16(sprite_entry, size | sprite_then_height_bits);
				previous_word = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, sprite_width);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, ((uint16_t)(previous_word + sprite_width) >> 1) | window_then_width_bits);
				mem_write16(sprite_entry, sprite_width | window_then_width_bits);
				previous_word = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, y);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, (int16_t)(previous_word + y) >> 1);
				mem_write16(sprite_entry, y);
				previous_word = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, x);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, (int16_t)(previous_word + x) >> 1);
				mem_write16(sprite_entry, x);
				sprite_entry += 2u;
			}
			else
			{
				mem_write16(r27, size);
				size = size | sprite_then_height_bits;
				mem_write16(sprite_entry + 0x3000u, size);
				mem_write16(sprite_entry, size);
				mem_write16(r27 + 2u, sprite_width);
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, sprite_width);
				mem_write16(sprite_entry, sprite_width);
				mem_write16(r27 + 4u, y);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, y);
				mem_write16(sprite_entry, y);
				mem_write16(r27 + 6u, x);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3000u, x);
				mem_write16(sprite_entry, x);
				sprite_entry += 2u;
			}
			mem_write32(sprite_entry + 0x3000u, rom_offset_and_palette);
			mem_write32(sprite_entry, rom_offset_and_palette);
			depth_list = screen + 0x5000u + mem_read16(projected_segment + 8u) * 4u;
			sprite_entry += 4u;
			if (mem_read16(depth_list) == 0u)
				mem_write16(depth_list + 2u, sprite_entry);
			layer_head = mem_read16(depth_list);
			mem_write32(sprite_entry, layer_head);
			mem_write32(sprite_entry + 0x3000u, (uint16_t)(layer_head + 0x300u));
			mem_write16(depth_list, ((sprite_entry - 0x10u) >> 4) | 0x8000u);
			SCREEN(screen)->sprite_groups++;
			sprite_entry += 0x10u;
		}
		ring_segment += ring_step;
		projected_segment += projected_step;
		ring_segment = (ring_segment & ~0xFFFFu) | (ring_segment & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
		r28 = (r28 & ~0xFFFFu) | (r28 & places_mask);
		items_left--;
		if (items_left == 0u)
		{
			SCREEN(screen)->sprite_list_last = sprite_entry;
			break;
		}
	}
	blank_place = blank_sprite_places;
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	SCREEN(screen)->next_sprite_place = r27;
}

/* road_row_draw for the rows drawn in mirrored pairs: each item twice, at the same distance either side
   of the road's centre, the second with sprite attribute bits 43 toggled (bit 6 flips it, bits 0-1 swap
   its X alignment); there is no off-screen test and no screen_centered. Used by scenery_pair and
   animated_scenery_pair. */
void road_row_pair_draw(uint32_t rom_offset_and_palette, uint32_t piece_width)
{
	uint16_t halfway_word = 0, mirror_bits = 0, offset_x = 0, places_mask = 0, sprite_width = 0, view_end = 0, y = 0;
	uint32_t attributes_and_size = 0, blank_place = 0, depth_list = 0, first_position = 0, item_count = 0, item_position = 0, items_left = 0, layer_head = 0, pair_x = 0, palette_word = 0, positions_moved = 0, projected_segment = 0, projected_step = 0, ring_segment = 0, ring_step = 0, size = 0, size_index = 0, size_steps = 0, size_table = 0, spacing = 0, sprite_entry = 0, sprite_size_step = 0, sprite_then_height_bits = 0, window_then_width_bits = 0;
	uint16_t items_in_view = 0;
	uint32_t flags = 0, flags_long = 0, previous_first_position = 0;

	mirror_bits = 0x43u;
	item_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	item_count = (uint16_t)OBJECT(current_object)->road_row.end_position;
	window_then_width_bits = RACE(SCREEN(screen)->race_copy)->window_segment;
	view_end = (uint16_t)(window_then_width_bits + (SCREEN(screen)->draw_distance >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)item_count <= (int16_t)window_then_width_bits)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	if ((int16_t)view_end <= (int16_t)item_position)
		return;
	while ((int16_t)item_position < (int16_t)window_then_width_bits)
	{
		item_position = (uint16_t)(item_position + spacing);
	}
	if ((int16_t)view_end < (int16_t)item_count)
		item_count = view_end;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = item_position;
	item_count = (uint16_t)(item_count - item_position);
	item_count = (uint16_t)(item_count + spacing);
	item_count = (uint16_t)(item_count - 1u);
	items_in_view = divide_u16(item_count, spacing);
	item_count = items_in_view;
	if ((int16_t)items_in_view <= 0)
		return;
	r28 = 0x100000u;
	r27 = SCREEN(screen)->next_sprite_place;
	places_mask = 0xEFFFu;
	flags = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u) | ((uint32_t)OBJECT(current_object)->unk_02 << 16u));
	flags_long = flags | 0x20u;
	OBJECT(current_object)->flags = (uint8_t)flags_long;
	OBJECT(current_object)->flags_high = (uint8_t)(flags_long >> 8u);
	OBJECT(current_object)->unk_02 = (uint16_t)(flags_long >> 16u);
	if ((flags & 0x20u) != 0u)
	{
		r28 = OBJECT(current_object)->road_row.previous_sprites;
		previous_first_position = first_position;
		positions_moved = previous_first_position - item_position;
		if (previous_first_position != item_position)
		{
			r28 = r28 - (divide_s32(positions_moved, spacing) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & places_mask);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = r27;
	ring_segment = screen + 0x2000u + ((item_position << 5) & 0xFFFu);
	projected_segment = screen + 0x3000u + ((item_position - window_then_width_bits) << 4);
	projected_step = spacing << 4;
	ring_step = spacing << 5;
	items_left = item_count;
	r12 = SPRITE_ANIMATION(sprite_animations[OBJECT(current_object)->animation])->frames[OBJECT(current_object)->frame] + 2u;
	palette_word = (uint32_t)SPRITE_PALETTE_WORDS(sprite_palettes[OBJECT(current_object)->palette])[0] << 16;
	attributes_and_size = (OBJECT(current_object)->flags | ((uint32_t)OBJECT(current_object)->flags_high << 8u));
	attributes_and_size = attributes_and_size & 0xC0u;
	attributes_and_size = attributes_and_size ^ SPRITE_PIECE(r12)->attributes;
	r12 = r12 + 2u;
	sprite_then_height_bits = SPRITE_PIECE(r12 - 0x2u)->sprite;
	size_steps = sprite_size_steps_ADDRESS + sprite_then_height_bits * 4u;
	sprite_entry = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		size = mem_read16(projected_segment);
		if (size != 0u && size <= 0x1200u)
		{
			pair_x = (int32_t)(((uint32_t)mem_read16(ring_segment + 0x12u) * OBJECT(current_object)->side + OBJECT(current_object)->x_offset) * size) >> 12;
			y = (uint16_t)(((int32_t)(OBJECT(current_object)->y_offset * size) >> 12) + mem_read16(projected_segment + 6u));
			offset_x = (uint16_t)pair_x;
			pair_x = (pair_x & ~0xFFFFu) | (uint16_t)(pair_x + mem_read16(projected_segment + 4u));
			pair_x = (((pair_x << 16) | (pair_x >> 16)) & ~0xFFFFu) | (uint16_t)(0u - offset_x);
			pair_x = (pair_x & ~0xFFFFu) | (uint16_t)(pair_x + mem_read16(projected_segment + 4u));
			size = shift_by(size, OBJECT(current_object)->scale_shift, 32);
			size_index = size;
			if (size_index >= 0x1000u)
				size_index = 0xFFFu;
			size_index = size_index >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			size = shift_by(size, (uint8_t)ROAD_ROW_SIZE_SHIFTS(road_row_size_shift_lists[size_table])[size_index], 32);
			sprite_size_step = ROAD_ROW_SIZE_STEPS(road_row_size_step_lists[size_table])[size_index];
			if (sprite_size_step != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = sprite_size_step;
				sprite_size_step = size_steps + sprite_size_step * 3u * 4u;
				attributes_and_size = (attributes_and_size & 0xFFFFu) | ((uint32_t)SPRITE_SIZE_STEP(sprite_size_step)->source_size << 16);
				sprite_size_step += 2u;
				rom_offset_and_palette = SPRITE_SIZE_STEP(sprite_size_step - 0x2u)->rom_offset;
				sprite_size_step += 2u;
				r14 = SPRITE_SIZE_STEP(sprite_size_step - 0x4u)->height;
				sprite_size_step += 2u;
				sprite_then_height_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x6u)->height_bits;
				sprite_size_step += 2u;
				piece_width = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width;
				window_then_width_bits = SPRITE_SIZE_STEP(sprite_size_step - 0x8u)->width_bits | SCREEN(screen)->sprite_screen_bit;
				rom_offset_and_palette |= palette_word;
			}
			sprite_width = (uint16_t)((size * piece_width) >> 8);
			size = ((size * r14) >> 8) & 0x3FFu;
			sprite_width &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= sprite_entry)
				break;
			attributes_and_size = (attributes_and_size & ~0xFFFFu) | (uint16_t)(attributes_and_size ^ mirror_bits);
			mem_write32(sprite_entry + 0x3010u, attributes_and_size);
			mem_write32(sprite_entry + 0x10u, attributes_and_size);
			attributes_and_size = (attributes_and_size & ~0xFFFFu) | (uint16_t)(attributes_and_size ^ mirror_bits);
			mem_write32(sprite_entry + 0x3000u, attributes_and_size);
			mem_write32(sprite_entry, attributes_and_size);
			mem_write16(r27 + 8u, rom_offset_and_palette);
			sprite_entry += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)rom_offset_and_palette)
			{
				halfway_word = mem_read16(r28);
				mem_write16(r27, size);
				halfway_word = ((uint16_t)(halfway_word + size) >> 1) | sprite_then_height_bits;
				size = size | sprite_then_height_bits;
				mem_write16(sprite_entry + 0x3010u, halfway_word);
				mem_write16(sprite_entry + 0x10u, size);
				mem_write16(sprite_entry + 0x3000u, halfway_word);
				mem_write16(sprite_entry, size);
				halfway_word = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, sprite_width);
				halfway_word = ((uint16_t)(halfway_word + sprite_width) >> 1) | window_then_width_bits;
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, halfway_word);
				mem_write16(sprite_entry + 0x10u, sprite_width);
				mem_write16(sprite_entry + 0x3000u, halfway_word);
				mem_write16(sprite_entry, sprite_width);
				halfway_word = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, y);
				halfway_word = (uint16_t)((int16_t)(halfway_word + y) >> 1);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, halfway_word);
				mem_write16(sprite_entry + 0x10u, y);
				mem_write16(sprite_entry + 0x3000u, halfway_word);
				mem_write16(sprite_entry, y);
				halfway_word = mem_read16(r28 + 0xAu);
				mem_write16(r27 + 0xAu, pair_x);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, (int16_t)(halfway_word + pair_x) >> 1);
				mem_write16(sprite_entry + 0x10u, pair_x);
				pair_x = (pair_x << 16) | (pair_x >> 16);
				halfway_word = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, pair_x);
				mem_write16(sprite_entry + 0x3000u, (int16_t)(halfway_word + pair_x) >> 1);
				mem_write16(sprite_entry, pair_x);
				sprite_entry += 2u;
			}
			else
			{
				mem_write16(r27, size);
				size = size | sprite_then_height_bits;
				mem_write16(sprite_entry + 0x3010u, size);
				mem_write16(sprite_entry + 0x10u, size);
				mem_write16(sprite_entry + 0x3000u, size);
				mem_write16(sprite_entry, size);
				mem_write16(r27 + 2u, sprite_width);
				sprite_width |= window_then_width_bits;
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, sprite_width);
				mem_write16(sprite_entry + 0x10u, sprite_width);
				mem_write16(sprite_entry + 0x3000u, sprite_width);
				mem_write16(sprite_entry, sprite_width);
				mem_write16(r27 + 4u, y);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, y);
				mem_write16(sprite_entry + 0x10u, y);
				mem_write16(sprite_entry + 0x3000u, y);
				mem_write16(sprite_entry, y);
				mem_write16(r27 + 0xAu, pair_x);
				sprite_entry += 2u;
				mem_write16(sprite_entry + 0x3010u, pair_x);
				mem_write16(sprite_entry + 0x10u, pair_x);
				pair_x = (pair_x << 16) | (pair_x >> 16);
				mem_write16(r27 + 6u, pair_x);
				mem_write16(sprite_entry + 0x3000u, pair_x);
				mem_write16(sprite_entry, pair_x);
				sprite_entry += 2u;
			}
			mem_write32(sprite_entry + 0x3010u, rom_offset_and_palette);
			mem_write32(sprite_entry + 0x10u, rom_offset_and_palette);
			mem_write32(sprite_entry + 0x3000u, rom_offset_and_palette);
			mem_write32(sprite_entry, rom_offset_and_palette);
			sprite_entry += 0x14u;
			depth_list = screen + 0x5000u + mem_read16(projected_segment + 8u) * 4u;
			if (mem_read16(depth_list) == 0u)
				mem_write16(depth_list + 2u, sprite_entry);
			layer_head = mem_read16(depth_list);
			mem_write32(sprite_entry, layer_head);
			mem_write32(sprite_entry + 0x3000u, (uint16_t)(layer_head + 0x300u));
			mem_write16(depth_list, ((sprite_entry - 0x20u) >> 4) | 0x8000u);
			SCREEN(screen)->sprite_groups += 2u;
			sprite_entry += 0x10u;
		}
		ring_segment += ring_step;
		projected_segment += projected_step;
		ring_segment = (ring_segment & ~0xFFFFu) | (ring_segment & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
		r28 = (r28 & ~0xFFFFu) | (r28 & places_mask);
		items_left--;
		if (items_left == 0u)
		{
			SCREEN(screen)->sprite_list_last = sprite_entry;
			break;
		}
	}
	blank_place = blank_sprite_places;
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	mem_write32(r27, blank_place);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & places_mask);
	SCREEN(screen)->next_sprite_place = r27;
}

/* Starts the race's course (stage_courses by screen first_stage, car course): clears the ring of
   segments, the first road width, and builds the segments as far as the view reaches (screen
   draw_distance >> 7, prebuilt_segments); the first stage into the stage history. */
void player_car_start_course(void)
{
	uint16_t ring_mask = 0, width_code = 0;
	uint32_t course = 0;

	move_string(zero_block_ADDRESS, 0x200u, screen + 0x2000u, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
	course = stage_courses[SCREEN(screen)->first_stage];
	CAR(player_car)->course = course;
	width_code = mem_read8(course + 8u);
	CAR(player_car)->width_code = (uint8_t)width_code;
	CAR(player_car)->road_width = width_code * 0xB0u;
	CAR(player_car)->prebuilt_segments = SCREEN(screen)->draw_distance >> 7;
	ring_mask = 0xFFFu;
	do
	{
		player_car_build_segment(ring_mask);
	} while ((int16_t)CAR(player_car)->built_segments < (int16_t)CAR(player_car)->prebuilt_segments);
	SCREEN(screen)->current_stage_level = SCREEN(screen)->built_stage_level;
	CAR(player_car)->stage_history = SCREEN(screen)->route_stage;
}

/* Keeps the course built 104 segments past the window's start (window_record, build_end_offset). At a
   fork the building waits (at_fork) until the checkpoint is passed (screen forks_decided): then the
   branch taken (screen 349 + the fork) goes on: branch, the course of that branch (car course), its
   stage (screen route_stage, stage indicator A5), the stage and branch histories and forks_taken, the
   stage's data queued (queue_stage_load) and the branch_arrow. */
void player_car_extend_course(void)
{
	uint8_t branch_stages = 0;
	uint16_t ring_mask = 0, run_width = 0, window_offset = 0;
	uint32_t branch = 0, course = 0, course_place = 0, forks_reached = 0, forks_taken = 0, object = 0, race = 0, run_length = 0;

	ring_mask = 0xFFFu;
	if (CAR(player_car)->at_fork == 0u)
	{
		window_offset = CAR(player_car)->window_offset;
		CAR(player_car)->window_record = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + window_offset);
		CAR(player_car)->build_end_offset = (window_offset + 0xD00u) & 0xFFFu;
		if (CAR(player_car)->build_offset != CAR(player_car)->build_end_offset)
		{
			do
			{
				if (CAR(player_car)->at_fork != 0u)
					break;
				player_car_build_segment(ring_mask);
			} while (CAR(player_car)->build_offset != CAR(player_car)->build_end_offset);
		}
		return;
	}
	forks_reached = CAR(player_car)->forks_reached;
	if (SCREEN(screen)->forks_decided < forks_reached)
		return;
	course = CAR(player_car)->course;
	branch = mem_read8(screen + 0x349u + forks_reached);
	CAR(player_car)->branch = branch;
	branch_stages = COURSE_FORK(course)->branch_stages[branch];
	CAR(player_car)->course = COURSE_FORK(course)->branch_courses[branch];
	SCREEN(screen)->route_stage = branch_stages;
	CAR(player_car)->new_branch = 0xFFu;
	SCREEN(screen)->route_stage_level++;
	forks_taken = CAR(player_car)->forks_taken;
	mem_write8(player_car + 0xD7u + forks_taken, branch);
	forks_taken = (uint8_t)(forks_taken + 1u);
	mem_write8(player_car + 0xD1u + forks_taken, branch_stages);
	CAR(player_car)->forks_taken = forks_taken;
	CAR(player_car)->at_fork = 0u;
	queue_stage_load();
	object = start_object();
	OBJECT(object)->handler = branch_arrow_update_ADDRESS;
	for (;;)
	{
		CAR(player_car)->course_records_read++;
		course_place = CAR(player_car)->course;
		run_length = (int16_t)ROM16(course_place);
		course_place += 2u;
		if ((int16_t)run_length > 0)
			break;
		switch (0u - run_length)
		{
		case 0:
			OBJECT(current_object)->state = 3u;
			OBJECT(current_object)->player_car.step = 0u;
			OBJECT(current_object)->player_car.state_high = 0u;
			return;
		case 6:
			object = start_object();
			OBJECT(object)->handler = goal_update_ADDRESS;
			OBJECT(object)->position = CAR(player_car)->built_segments;
			CAR(player_car)->course = course_place;
			continue;
		case 1:
			CAR(player_car)->course = ROM32(course_place);
			continue;
		case 2:
			race = SCREEN(screen)->race;
			CAR(player_car)->forks_reached++;
			RACE(race)->stage_count++;
			CAR(player_car)->course = course_place;
			CAR(player_car)->at_fork = 0xFFu;
			return;
		case 3:
			CAR(player_car)->pitch_angle = 0u;
			CAR(player_car)->course = course_place + 4u;
			continue;
		case 4:
			SCREEN(screen)->built_stage_level = ROM8(course_place);
			course_place++;
			SCREEN(screen)->built_stage = ROM8(course_place);
			CAR(player_car)->stage_segments = 0xFFFFu;
			CAR(player_car)->course = course_place + 1u;
			CAR(player_car)->stage_start = CAR(player_car)->built_segments;
			continue;
		case 5:
			CAR(player_car)->heading_angle = 0u;
			CAR(player_car)->course = course_place + 4u;
			continue;
		default:
			invalid_jump(course_command_jumps[0u - run_length]);
		}
	}
	CAR(player_car)->run_left = run_length;
	CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
	course_place++;
	CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
	course_place++;
	run_width = ROM8(course_place);
	course_place++;
	CAR(player_car)->road_event = ROM8(course_place);
	CAR(player_car)->course = course_place + 1u;
	CAR(player_car)->width_code = (uint8_t)run_width;
	run_width = (uint16_t)(run_width * 0xB0u);
	CAR(player_car)->road_width_target = run_width;
	CAR(player_car)->road_width_step = (int16_t)(run_width - CAR(player_car)->road_width) >> 6;
	player_car_build_segment(ring_mask);
}

/* Builds the next segment of the road from the course (car course): at the end of a run reads the next
   records: 0 the course ends (the car's state 3), 1 a jump to another part, 2 a fork (forks_reached, the
   race's stage count, at_fork), 3 and 5 the pitch and the heading back to 0, 4 a new stage (screen
   built_stage_level, unk_A3; stage_segments, stage_start), 6 the goal (a goal object at this segment),
   else a run (its length, pitch and heading steps, road width, a byte for the segments). The segment:
   heading and pitch turned by the run's steps, the position moved along them (next_x/y/z), the width
   eased toward the run's; written at build_offset, then built_segments and stage_segments count up. */
void player_car_build_segment(uint16_t ring_mask)
{
	uint16_t road_width = 0, run_width = 0;
	uint32_t course_place = 0, heading_cosine = 0, heading_sine = 0, object = 0, pitch_cosine = 0, pitch_sine = 0, race = 0, ring_segment = 0, run_length = 0;
	uint16_t run_left = 0;

	for (;;)
	{
		run_left = CAR(player_car)->run_left;
		CAR(player_car)->run_left = run_left - 1u;
		if ((int16_t)run_left >= 1)
			break;
		for (;;)
		{
			CAR(player_car)->course_records_read++;
			course_place = CAR(player_car)->course;
			run_length = (int16_t)ROM16(course_place);
			course_place += 2u;
			if ((int16_t)run_length > 0)
				break;
			switch (0u - run_length)
			{
			case 0:
				OBJECT(current_object)->state = 3u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->player_car.state_high = 0u;
				return;
			case 6:
				object = start_object();
				OBJECT(object)->handler = goal_update_ADDRESS;
				OBJECT(object)->position = CAR(player_car)->built_segments;
				CAR(player_car)->course = course_place;
				continue;
			case 1:
				CAR(player_car)->course = ROM32(course_place);
				continue;
			case 2:
				race = SCREEN(screen)->race;
				CAR(player_car)->forks_reached++;
				RACE(race)->stage_count++;
				CAR(player_car)->course = course_place;
				CAR(player_car)->at_fork = 0xFFu;
				return;
			case 3:
				CAR(player_car)->pitch_angle = 0u;
				CAR(player_car)->course = course_place + 4u;
				continue;
			case 4:
				SCREEN(screen)->built_stage_level = ROM8(course_place);
				course_place++;
				SCREEN(screen)->built_stage = ROM8(course_place);
				CAR(player_car)->stage_segments = 0xFFFFu;
				CAR(player_car)->course = course_place + 1u;
				CAR(player_car)->stage_start = CAR(player_car)->built_segments;
				continue;
			case 5:
				CAR(player_car)->heading_angle = 0u;
				CAR(player_car)->course = course_place + 4u;
				continue;
			default:
				invalid_jump(course_command_jumps[0u - run_length]);
			}
		}
		CAR(player_car)->run_left = run_length;
		CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
		course_place++;
		CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(course_place) << 8;
		course_place++;
		run_width = ROM8(course_place);
		course_place++;
		CAR(player_car)->road_event = ROM8(course_place);
		CAR(player_car)->course = course_place + 1u;
		CAR(player_car)->width_code = (uint8_t)run_width;
		run_width = (uint16_t)(run_width * 0xB0u);
		CAR(player_car)->road_width_target = run_width;
		CAR(player_car)->road_width_step = (int16_t)(run_width - CAR(player_car)->road_width) >> 6;
	}
	CAR(player_car)->width_changing = 0xFFu;
	if ((uint16_t)CAR(player_car)->road_width_step == 0u)
		CAR(player_car)->width_changing = 0u;
	else if (CAR(player_car)->road_width_step < 0)
	{
		CAR(player_car)->road_width += CAR(player_car)->road_width_step;
		if (CAR(player_car)->road_width <= CAR(player_car)->road_width_target)
		{
			CAR(player_car)->road_width = CAR(player_car)->road_width_target;
			CAR(player_car)->road_width_step = 0;
			CAR(player_car)->width_changing = 0u;
		}
	}
	else
	{
		CAR(player_car)->road_width += CAR(player_car)->road_width_step;
		if (CAR(player_car)->road_width_target <= CAR(player_car)->road_width)
		{
			CAR(player_car)->road_width = CAR(player_car)->road_width_target;
			CAR(player_car)->road_width_step = 0;
			CAR(player_car)->width_changing = 0u;
		}
	}
	CAR(player_car)->heading_angle = (((uint32_t)(int16_t)CAR(player_car)->heading_step << 8) + CAR(player_car)->heading_angle) & 0xFFFFFFFu;
	CAR(player_car)->pitch_angle = (((uint32_t)(int16_t)CAR(player_car)->pitch_step << 8) + CAR(player_car)->pitch_angle) & 0xFFFFFFFu;
	pitch_cosine = (uint16_t)(CAR(player_car)->pitch_angle >> 16u);
	heading_cosine = (uint16_t)(CAR(player_car)->heading_angle >> 16u);
	pitch_sine = sine_table[pitch_cosine];
	pitch_cosine = cosine_table[pitch_cosine];
	heading_sine = sine_table[heading_cosine];
	heading_cosine = cosine_table[heading_cosine];
	CAR(player_car)->next_x_fine += (int32_t)(heading_sine * pitch_cosine) >> 13;
	CAR(player_car)->next_y_fine -= pitch_sine << 1;
	CAR(player_car)->next_z_fine += (int32_t)(pitch_cosine * heading_cosine) >> 13;
	CAR(player_car)->next_x = CAR(player_car)->next_x_fine;
	CAR(player_car)->next_x = (int32_t)CAR(player_car)->next_x >> 8;
	CAR(player_car)->next_y = CAR(player_car)->next_y_fine;
	CAR(player_car)->next_y = (int32_t)CAR(player_car)->next_y >> 8;
	CAR(player_car)->next_z = CAR(player_car)->next_z_fine;
	CAR(player_car)->next_z = (int32_t)CAR(player_car)->next_z >> 8;
	ring_segment = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->build_offset);
	mem_write16(ring_segment, (uint16_t)(CAR(player_car)->heading_angle >> 16u));
	mem_write16(ring_segment + 2u, (uint16_t)(CAR(player_car)->pitch_angle >> 16u));
	mem_write32(ring_segment + 4u, CAR(player_car)->next_x);
	mem_write32(ring_segment + 8u, CAR(player_car)->next_z);
	mem_write32(ring_segment + 0xCu, CAR(player_car)->next_y);
	mem_write8(ring_segment + 0x15u, CAR(player_car)->road_event);
	road_width = CAR(player_car)->road_width;
	mem_write16(ring_segment + 0x10u, road_width);
	mem_write16(ring_segment + 0x12u, road_width >> 1);
	if (CAR(player_car)->width_changing == 0u)
		mem_write8(ring_segment + 0x14u, CAR(player_car)->width_code);
	else
		mem_write8(ring_segment + 0x14u, 0u);
	CAR(player_car)->build_offset = (ring_segment + 0x20u) & ring_mask;
	CAR(player_car)->built_segments++;
	CAR(player_car)->stage_segments++;
}

/* Projects the road for the view of screen shown_view (player_car_publish): its rotation (view_matrix_*,
   from the view's pitch and yaw), then each segment from the window's start out to the view's distance
   turned into the view and scaled by its depth into the screen's list at screen + 3000 (scale, width, x,
   y, the segment's byte), segments behind the view or too far left out. */
void player_car_project_road(void)
{
	uint32_t carry = 0;
	uint16_t previous_y = 0, screen_top_y = 0;
	uint32_t car = 0, draw_distance = 0, fixed_point_shift = 0, matrix_xx = 0, matrix_xz = 0, matrix_yy = 0, matrix_zx = 0, matrix_zy = 0, matrix_zz = 0, near_limit = 0, previous_projected = 0, projected_segment = 0, projection_pattern = 0, ring_segment = 0, scale = 0, segments_left = 0, to_view_x = 0, to_view_y = 0, to_view_z = 0, unrotated_z = 0, view_pitch = 0, view_yaw = 0, x = 0;
	uint8_t projected = 0;
	uint16_t last_y = 0;
	uint32_t copied_x_y = 0, copied_road_bytes = 0;

	view_pitch = mem_read16(SCREEN(screen)->shown_view + 0xCu);
	view_yaw = mem_read16(SCREEN(screen)->shown_view + 0xEu);
	matrix_zy = sine_table[view_pitch];
	matrix_zx = sine_table[view_yaw];
	matrix_yy = cosine_table[view_pitch];
	matrix_xx = cosine_table[view_yaw];
	fixed_point_shift = 0xFFFFFFF2u;
	matrix_xz = 0u - matrix_zx;
	r12 = matrix_zy * matrix_zx;
	carry = (r12 >> 13) & 1u;
	r12 = ((int32_t)r12 >> 14) + carry;
	r14 = matrix_zy * matrix_xx;
	carry = (r14 >> 13) & 1u;
	r14 = ((int32_t)r14 >> 14) + carry;
	matrix_zx = matrix_zx * matrix_yy;
	carry = (matrix_zx >> 13) & 1u;
	matrix_zx = ((int32_t)matrix_zx >> 14) + carry;
	matrix_zy = 0u - matrix_zy;
	matrix_zz = matrix_yy * matrix_xx;
	carry = (matrix_zz >> 13) & 1u;
	matrix_zz = ((int32_t)matrix_zz >> 14) + carry;
	car = player_car + 0xDCu;
	CAR(car - 0xDCu)->view_matrix_xx = matrix_xx;
	car += 4u;
	CAR(car - 0xE0u)->view_matrix_xy = 0;
	car += 4u;
	CAR(car - 0xE4u)->view_matrix_xz = matrix_xz;
	car += 4u;
	CAR(car - 0xE8u)->view_matrix_yx = r12;
	car += 4u;
	CAR(car - 0xECu)->view_matrix_yy = matrix_yy;
	car += 4u;
	CAR(car - 0xF0u)->view_matrix_yz = r14;
	car += 4u;
	CAR(car - 0xF4u)->view_matrix_zx = matrix_zx;
	car += 4u;
	CAR(car - 0xF8u)->view_matrix_zy = matrix_zy;
	CAR(car - 0xF8u)->view_matrix_zz = matrix_zz;
	to_view_x = 0u - mem_read32(SCREEN(screen)->shown_view);
	to_view_y = 0u - mem_read32(SCREEN(screen)->shown_view + 4u);
	to_view_z = 0u - mem_read32(SCREEN(screen)->shown_view + 8u);
	near_limit = SCREEN(screen)->eye_distance;
	near_limit = near_limit >> 1;
	draw_distance = SCREEN(screen)->draw_distance;
	screen_top_y = (uint16_t)(0u - SCREEN(screen)->sprite_origin_y);
	ring_segment = screen + 0x1FE0u + mem_read16(SCREEN(screen)->shown_view + 0x10u);
	projected_segment = screen + 0x2FF0u;
	segments_left = draw_distance >> 7;
	SCREEN(screen)->crest_count = 0u;
	previous_y = 0x7FFFu;
	SCREEN(screen)->crest_heights = screen_top_y + 0xE0u;
	projection_pattern = segment_projection_pattern_ADDRESS;
	for (;;)
	{
		ring_segment = ((ring_segment + 0x20u) & ~0xFFFFu) | ((ring_segment + 0x20u) & 0xAFFFu);
		projected = ROAD_PROJECTION_PATTERN(projection_pattern)[0];
		projection_pattern++;
		projected_segment += 0x10u;
		if (projected == 0u)
		{
			previous_projected = projected_segment - 0x10u;
			copied_x_y = mem_read32(previous_projected + 4u);
			mem_write32(projected_segment, mem_read32(previous_projected));
			mem_write32(projected_segment + 4u, copied_x_y);
			previous_projected += 8u;
			copied_road_bytes = mem_read32(previous_projected + 4u);
			mem_write32(projected_segment + 8u, mem_read32(previous_projected));
			mem_write32(projected_segment + 0xCu, copied_road_bytes);
			mem_write16(projected_segment + 0xCu, mem_read16(ring_segment + 0x14u));
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		x = to_view_x + mem_read32(ring_segment + 4u);
		r27 = to_view_y;
		r27 = r27 + mem_read32(ring_segment + 0xCu);
		r28 = to_view_z;
		r28 = r28 + mem_read32(ring_segment + 8u);
		unrotated_z = r28;
		r28 = r28 * matrix_zz;
		r28 = r28 + x * matrix_zx;
		r28 = r28 + r27 * matrix_zy;
		r28 = r28 + fixed_point_shift;
		r28 = (int32_t)r28 >> 14;
		r27 = r27 * matrix_yy;
		r27 = r27 + x * r12;
		r27 = r27 + unrotated_z * r14;
		r27 = r27 + fixed_point_shift;
		r27 = (int32_t)r27 >> 14;
		x = (int32_t)(x * matrix_xx + unrotated_z * matrix_xz + fixed_point_shift) >> 14;
		if ((int32_t)draw_distance <= (int32_t)r28 || (int32_t)r28 <= (int32_t)near_limit)
		{
			mem_write16(projected_segment, 0u);
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				r27 = (r27 & ~0xFFFFu) | previous_y;
				r28 = (r28 & ~0xFFFFu) | 3u;
				note_road_crest();
				return;
			}
			continue;
		}
		scale = perspective_scales[r28];
		mem_write16(projected_segment, scale);
		x = x * scale;
		r27 = r27 * scale;
		carry = (x >> 11) & 1u;
		x = ((int32_t)x >> 12) + carry;
		carry = (r27 >> 11) & 1u;
		r27 = (int32_t)r27 >> 12;
		r27 = r27 + carry;
		scale = scale * mem_read16(ring_segment + 0x10u);
		carry = (scale >> 11) & 1u;
		mem_write16(projected_segment + 2u, ((int32_t)scale >> 12) + carry);
		mem_write16(projected_segment + 0xCu, mem_read16(ring_segment + 0x14u));
		mem_write16(projected_segment + 4u, x);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(0u - r27);
		mem_write16(projected_segment + 6u, r27);
		r28 = depth_layers_by_depth[r28];
		mem_write16(projected_segment + 8u, r28);
		last_y = previous_y;
		previous_y = (uint16_t)r27;
		if ((int16_t)last_y < (int16_t)r27)
		{
			note_road_crest();
			segments_left--;
			if (segments_left == 0u)
			{
				mem_write16(projected_segment + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		segments_left--;
		if (segments_left == 0u)
			break;
	}
	mem_write16(projected_segment + 0x10u, 0u);
	note_road_crest();
}

/* Notes a crest of the road while player_car_project_road projects it, for crest_clip_update: when it is
   the first, or higher on the screen (R27) than the last one noted, crest_count goes up, its depth layer
   R28 goes into crest_layers (at the old count, with 3 in the next place) and its height into
   crest_heights (at the new count; entry 0 is the bottom of the screen). */
void note_road_crest(void)
{
	uint32_t crest_count = 0;

	crest_count = SCREEN(screen)->crest_count;
	if (crest_count == 0u || (int16_t)r27 < (int16_t)mem_read16(screen + 0x1F30u + crest_count * 2u))
	{
		SCREEN(screen)->crest_count++;
		mem_write16(screen + 0x1F10u + crest_count * 2u, r28);
		mem_write16(screen + 0x1F12u + crest_count * 2u, 3u);
		mem_write16(screen + 0x1F32u + crest_count * 2u, r27);
	}
}

/* Turns an offset (R1 across, R2 along, R0 up) by a heading (R3) and a pitch (R4) into the world's axes.
   Used by player_car_world_position and player_car_place_view. */
void rotate_offset(uint32_t y, uint32_t road_heading, uint32_t road_pitch, uint32_t *p_x, uint32_t *p_z)
{
	uint32_t x = *p_x;
	uint32_t z = *p_z;
	uint32_t heading_cosine = 0, heading_sine = 0, minus_x = 0, pitch_cosine = 0, pitch_sine = 0, y_copy = 0;

	pitch_sine = sine_table[road_pitch];
	pitch_cosine = cosine_table[road_pitch];
	heading_cosine = cosine_table[road_heading];
	heading_sine = sine_table[road_heading];
	y_copy = y;
	minus_x = 0u - x;
	x = (int32_t)(x * heading_cosine + ((int32_t)(y_copy * heading_sine) >> 14) * pitch_sine + ((int32_t)(z * heading_sine) >> 14) * pitch_cosine) >> 14;
	z = (int32_t)(((int32_t)(z * heading_cosine) >> 14) * pitch_cosine + (minus_x * heading_sine + ((int32_t)(y_copy * heading_cosine) >> 14) * pitch_sine)) >> 14;
	*p_x = x;
	*p_z = z;
}

/* Sets up the constants of the screen's block at boot: index (0 for the block at 208000, 1 at 218000)
   and by it (screen_hardware_records) sprite_screen_bit, palette_ram, mixer and io_chip; eye_distance,
   draw_distance (the second of the draw distances at 010BE2), sprite_origin_x, sprite_origin_y and
   view_pitch_base from 010BCC (010BC8 holds the draw distances' address); and stage_lengths: for each of
   the 31 roads of stage_courses (the courses player_car_start_course starts by screen first_stage) the
   sum of its sections' lengths (the first halfword of each 6-byte record; the negative ones are
   commands: -1 goes on at the address it holds, -4 is 4 bytes long, -2 and -6 end it), and
   branch_road_length (14A) after them. */
uint32_t init_screen(void)
{
	bool L_013DB6 = false;
	uint16_t course_length = 0;
	uint32_t command_routine = 0, courses_left = 0, index = 0, length_place = 0, screen_hardware = 0, section_length = 0, stage_course = 0;
	uint32_t command = 0;

	index = screen >> 16;
	index = index & 1u;
	SCREEN(screen)->index = index;
	screen_hardware = screen_hardware_records[index];
	SCREEN(screen)->sprite_screen_bit = SCREEN_HARDWARE(screen_hardware)->sprite_screen_bit;
	screen_hardware += 2u;
	SCREEN(screen)->palette_ram = SCREEN_HARDWARE(screen_hardware - 0x2u)->palette_ram;
	screen_hardware += 4u;
	SCREEN(screen)->mixer = SCREEN_HARDWARE(screen_hardware - 0x6u)->mixer;
	SCREEN(screen)->io_chip = SCREEN_HARDWARE(screen_hardware - 0x6u)->io_chip;
	SCREEN(screen)->draw_distance = DRAW_DISTANCE_LIST(screen_view_defaults[0].draw_distances)[1];
	SCREEN(screen)->eye_distance = screen_view_defaults[0].eye_distance;
	SCREEN(screen)->sprite_origin_x = screen_view_defaults[0].sprite_origin_x;
	SCREEN(screen)->sprite_origin_y = screen_view_defaults[0].sprite_origin_y;
	SCREEN(screen)->view_pitch_base = screen_view_defaults[0].view_pitch_bases[0];
	stage_course = stage_courses_ADDRESS;
	length_place = screen + 0x700u;
	courses_left = 0x1Fu;
	do
	{
		r12 = COURSE_POINTER(stage_course)[0];
		section_length = 0u;
		course_length = 0u;
		stage_course += 4u;
		for (;;)
		{
			course_length += section_length;
			for (;;)
			{
				section_length = mem_read16(r12);
				r12 = r12 + 6u;
				if ((int16_t)section_length >= 0)
					break;
				command = (uint16_t)~section_length;
				command_routine = course_length_command_jumps[command];
				switch (command)
				{
				case 2:
				case 4:
					continue;
				case 0:
					r12 = mem_read32(r12 - 4u);
					continue;
				case 3:
					r12 = r12 - 2u;
					continue;
				case 1:
				case 5:
					L_013DB6 = true;
					break;
				default:
					invalid_jump(course_length_command_jumps[command]);
				}
				if (L_013DB6)
					break;
			}
			if (L_013DB6)
				break;
		}
		L_013DB6 = false;
		mem_write16(length_place, course_length);
		courses_left--;
		length_place += 2u;
	} while (courses_left != 0u);
	mem_write16(length_place, 0x14Au);
	return command_routine;
}

/* Clears the screen's work RAM at boot: E000 bytes from 8000 below its block (the objects and their
   lists) to 6000 into it. */
void clear_screen_ram(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = screen - 0x8000u;
	blocks_left = 0x38u;
	block_halfwords = 0x200u;
	do
	{
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
		block += 0x400u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Sets up video RAM: cleared (clear_video_ram), the text font (load_text_font), the video control
   register 31FF00 (0800; 0A00 flipped, flip_screen) and the registers 31FF02-31FF8F from 010B12, with
   their copy at video_registers, the four tile layers' scroll targets (scroll_targets-20E2AF) cleared,
   then reset_tile_layers. */
uint32_t init_video(void)
{
	clear_video_ram();
	load_text_font();
	mem_write16(VIDEO_31FF00, video_control_default);
	video_registers[0] = video_control_default;
	if (flip_screen != 0u)
	{
		mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) | 0x200u);
		video_registers[0] |= 0x200u;
	}
	move_halfwords(scroll_targets_ADDRESS, zero_block_ADDRESS, 0x10u);
	move_halfwords(VIDEO_RAM + 0x1FF02u, video_register_defaults_ADDRESS, 0x47u);
	move_string(video_register_defaults_ADDRESS, 0x47u, video_registers_ADDRESS + 0x2u, 0x47u, 2u, 0u, 0u, 0u, &r27, &r28);
	return reset_tile_layers();
}

/* Puts the screen's two tile layers (NBG0 and NBG2 for screen 1, NBG1 and NBG3 for screen 2) back at
   page 0 and scroll 0: their page selects (video_registers[32], video_registers[36]) and scroll targets
   (scroll_targets, scroll_targets[4]) cleared, and bits 12-15 of the video control register (31FF00,
   video_registers) cleared. Used as the game modes start. */
uint32_t reset_tile_layers(void)
{
	uint32_t index = 0;
	uint32_t first_scroll_target = 0, second_scroll_target = 0;

	mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) & 0xFFFu);
	video_registers[0] &= 0xFFFu;
	index = SCREEN(screen)->index;
	RAM32(video_registers_ADDRESS + 0x40u + index * 4u) = 0u;
	RAM32(video_registers_ADDRESS + 0x48u + index * 4u) = 0u;
	first_scroll_target = scroll_targets_ADDRESS + index * 8u;
	mem_write32(first_scroll_target, 0u);
	mem_write32(first_scroll_target + 4u, 0u);
	second_scroll_target = scroll_targets_ADDRESS + 0x10u + index * 8u;
	mem_write32(second_scroll_target, 0u);
	mem_write32(second_scroll_target + 4u, 0u);
	return index;
}

/* Clears video RAM below its registers (300000-31FEFF): the tilemap pages, the text layer and its font,
   the bitmap and the line tables. */
void clear_video_ram(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = 0x300000u;
	blocks_left = 0x1FFu;
	block_halfwords = 0x80u;
	do
	{
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
		block += 0x100u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Clears the text layer's first 30 rows (64 cells each, 31F000-31FEFF). */
void clear_text_layer(void)
{
	uint32_t block = 0, block_halfwords = 0, blocks_left = 0;

	block = TEXT_CELL(0, 0);
	blocks_left = 0xFu;
	block_halfwords = 0x80u;
	do
	{
		move_string(zero_block_ADDRESS, block_halfwords, block, block_halfwords, 2u, 0u, 0u, 0u, &r27, &r28);
		block += 0x100u;
		blocks_left--;
	} while (blocks_left != 0u);
}

/* Unused: clears the tilemap pages from the one at R9 (its offset in video RAM) down to the first, 400
   bytes apart (clear_tile_page each). */
void clear_tile_pages(uint16_t last_page)
{
	uint16_t next_page = 0, page = 0;
	uint16_t cleared_page = 0;

	next_page = (uint16_t)last_page;
	do
	{
		page = (uint16_t)next_page;
		clear_tile_page(page);
		cleared_page = next_page;
		next_page = (uint16_t)(cleared_page - 0x400u);
	} while ((int16_t)cleared_page >= 0x400);
}

/* Only unused code reaches it (clear_tile_pages): clears the tilemap page at R9 (300000 + R9: 200
   halfwords copied from the zeros at 100800). */
void clear_tile_page(uint16_t page)
{
	move_string(zero_block_ADDRESS, 0x200u, 0x300000u | (uint16_t)page, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Sets up the sprite chip at boot: its control registers 500000-50000C from 010B0A-010B0F (rendering
   every frame), the buffer's flip bits inverted when the screen is flipped (flip_screen), then
   init_sprite_lists. */
void init_sprite_chip(void)
{
	uint16_t control_flip = 0;
	uint32_t control_register = 0, control_values = 0;

	mem_write16(SPRITE_CONTROL_500000, sprite_control_mode);
	control_register = SPRITE_CONTROL + 0x4u;
	control_flip = sprite_control_flip;
	control_values = sprite_control_rest_ADDRESS;
	if (flip_screen != 0u)
		control_flip = control_flip ^ 3u;
	mem_write16(control_register, control_flip);
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	control_values++;
	control_register += 2u;
	mem_write16(control_register, SPRITE_CONTROL_VALUES(control_values)[0]);
	mem_write16(control_register + 2u, SPRITE_CONTROL_VALUES(control_values)[1]);
	init_sprite_lists();
}

/* Sets up the sprite lists: sprite RAM entries 0-4 (0 clips to the whole screen, 1 sets the offset
   sprites can add, x A0 y DE (sprite_origin_x, sprite_origin_y), and jumps to 2; 2 and 3, the jumps to
   screen 1's and screen 2's lists, and 4 an end), an end at the start of the eight list buffers (404000,
   40A000, 414000, 41A000 and their halfway copies 3000 on), each screen's list at its first buffer
   (sprite_list_last 404000 and 414000, sprite_list_buffers 0), its ring of sprite places at 20C000 and
   21C000 (next_sprite_place), sprite_groups 0, and sprite_lists_set_up = 1 (nothing reads it). */
void init_sprite_lists(void)
{
	move_string(sprite_ram_start_entries_ADDRESS, 0x28u, 0x400000u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	SCREEN(SCREEN_1)->sprite_list_last = SPRITE_RAM_404000;
	SCREEN(SCREEN_2)->sprite_list_last = SPRITE_RAM_414000;
	SCREEN(SCREEN_1)->next_sprite_place = 0x20C000u;
	SCREEN(SCREEN_2)->next_sprite_place = 0x21C000u;
	mem_write16(SPRITE_RAM_404000, 0xFFFFu);
	mem_write16(SPRITE_RAM_407000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40D000, 0xFFFFu);
	mem_write16(SPRITE_RAM_414000, 0xFFFFu);
	mem_write16(SPRITE_RAM_417000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41D000, 0xFFFFu);
	sprite_lists_set_up = 1u;
	sprite_list_buffers = 0u;
	SCREEN(SCREEN_1)->sprite_groups &= 0xFF00u;
	SCREEN(SCREEN_2)->sprite_groups &= 0xFF00u;
}

/* Sets the count of sprite groups drawn this frame (sprite_groups) to 0: unless something is drawn after
   it, the vertical blank (finish_sprite_list) ends the screen's list at its start, without sprites. Used
   as the game modes start. */
void clear_sprite_group_count(void)
{
	SCREEN(screen)->sprite_groups = 0u;
}

/* Sets the screen's mixer (registers 00-4E at mixer) and its two copies in the screen's block
   (mixer_values, mixer_defaults) to the defaults at 010AB6, with the other screen's two tile layers
   turned off (010B06: NBG1 and NBG3 on screen 1, NBG0 and NBG2 on screen 2; the two screens share video
   RAM, each showing two of the four layers). */
void init_mixer(void)
{
	uint32_t first_layer = 0, hidden_layer_pair = 0, mixer = 0, second_layer = 0;

	mixer = SCREEN(screen)->mixer;
	move_halfwords_backward(mixer, mixer_register_defaults_ADDRESS, 0x28u);
	move_halfwords(screen + 0x160u, mixer_register_defaults_ADDRESS, 0x28u);
	move_string(mixer_register_defaults_ADDRESS, 0x28u, screen + 0x1B0u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	hidden_layer_pair = hidden_layer_pairs_ADDRESS + SCREEN(screen)->index * 2u;
	first_layer = HIDDEN_LAYER_PAIR(hidden_layer_pair)->first_layer;
	second_layer = HIDDEN_LAYER_PAIR(hidden_layer_pair)->second_layer;
	mem_write16(mixer + first_layer * 2u, 0u);
	mem_write16(mixer + second_layer * 2u, 0u);
	mem_write16(screen + 0x160u + first_layer * 2u, 0u);
	mem_write16(screen + 0x160u + second_layer * 2u, 0u);
	mem_write16(screen + 0x1B0u + first_layer * 2u, 0u);
	mem_write16(screen + 0x1B0u + second_layer * 2u, 0u);
}

/* Sets up the two I/O chips at boot: ports D, G and H outputs and cleared (the coin meters, lamps, LCD,
   the tile bank and the EEPROM's lines), each chip's screen on and the sound CPU out of reset (CNT 6 and
   86), and the first bank of the wheels' and pedals' A/D inputs (C00060). */
void init_io_chips(void)
{
	mem_write8(IO_C0001C, 6u);
	mem_write8(IO_C0001E, 0xC8u);
	mem_write8(IO_C00006, 0u);
	mem_write8(IO_C0000C, 0u);
	mem_write8(IO_C0000E, 0u);
	mem_write8(0xC8001Cu, 0x86u);
	mem_write8(0xC8001Eu, 0xC8u);
	mem_write8(0xC80006u, 0u);
	mem_write8(0xC8000Cu, 0u);
	mem_write8(0xC8000Eu, 0u);
	mem_write8(IO_C00060, 0u);
}

/* Copies data into sprite RAM outside the lists at boot: 1E0 bytes from 12297C to 410000, 2400 bytes
   from 107020 to 401000, and twelve tables of sixteen halfwords from 010C44 to 403400 (laid out as the
   indirect colour tables sprites can take from sprite RAM). */
void load_sprite_ram_tables(void)
{
	move_halfwords(SPRITE_RAM + 0x10000u, solid_colour_blocks_ADDRESS, 0xF0u);
	move_halfwords(SPRITE_RAM + 0x1000u, indirect_colour_tables_ADDRESS, 0x1200u);
	move_string(last_colour_tables_ADDRESS, 0xC0u, SPRITE_RAM + 0x3400u, 0xC0u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: meant to copy R0 & 3F8 bytes from R10 to R11 by jumping into a run of 8-byte moves at 0109B4,
   but the jump (200 less half the count from the run's start) is worked out for 128 moves and the run
   has 64: it copies the count less 200 bytes, nothing for 200, and for less it jumps past the run into
   the data at 010AB5. */
void unrolled_copy(uint16_t byte_count, uint32_t source, uint32_t destination)
{
	uint32_t move_destination = 0, second_word = 0, move_destination_2 = 0, second_word_2 = 0, move_destination_3 = 0, second_word_3 = 0, move_destination_4 = 0, second_word_4 = 0, move_destination_5 = 0, second_word_5 = 0, move_destination_6 = 0, second_word_6 = 0, move_destination_7 = 0, second_word_7 = 0, move_destination_8 = 0, second_word_8 = 0, move_destination_9 = 0, second_word_9 = 0, move_destination_10 = 0, second_word_10 = 0, move_destination_11 = 0, second_word_11 = 0, move_destination_12 = 0, second_word_12 = 0, move_destination_13 = 0, second_word_13 = 0, move_destination_14 = 0, second_word_14 = 0, move_destination_15 = 0, second_word_15 = 0, move_destination_16 = 0, second_word_16 = 0, move_destination_17 = 0, second_word_17 = 0, move_destination_18 = 0, second_word_18 = 0, move_destination_19 = 0, second_word_19 = 0, move_destination_20 = 0, second_word_20 = 0, move_destination_21 = 0, second_word_21 = 0, move_destination_22 = 0, second_word_22 = 0, move_destination_23 = 0, second_word_23 = 0, move_destination_24 = 0, second_word_24 = 0, move_destination_25 = 0, second_word_25 = 0, move_destination_26 = 0, second_word_26 = 0, move_destination_27 = 0, second_word_27 = 0, move_destination_28 = 0, second_word_28 = 0, move_destination_29 = 0, second_word_29 = 0, move_destination_30 = 0, second_word_30 = 0, move_destination_31 = 0, second_word_31 = 0, move_destination_32 = 0, second_word_32 = 0, move_destination_33 = 0, second_word_33 = 0, move_destination_34 = 0, second_word_34 = 0, move_destination_35 = 0, second_word_35 = 0, move_destination_36 = 0, second_word_36 = 0, move_destination_37 = 0, second_word_37 = 0, move_destination_38 = 0, second_word_38 = 0, move_destination_39 = 0, second_word_39 = 0, move_destination_40 = 0, second_word_40 = 0, move_destination_41 = 0, second_word_41 = 0, move_destination_42 = 0, second_word_42 = 0, move_destination_43 = 0, second_word_43 = 0, move_destination_44 = 0, second_word_44 = 0, move_destination_45 = 0, second_word_45 = 0, move_destination_46 = 0, second_word_46 = 0, move_destination_47 = 0, second_word_47 = 0, move_destination_48 = 0, second_word_48 = 0, move_destination_49 = 0, second_word_49 = 0, move_destination_50 = 0, second_word_50 = 0, move_destination_51 = 0, second_word_51 = 0, move_destination_52 = 0, second_word_52 = 0, move_destination_53 = 0, second_word_53 = 0, move_destination_54 = 0, second_word_54 = 0, move_destination_55 = 0, second_word_55 = 0, move_destination_56 = 0, second_word_56 = 0, move_destination_57 = 0, second_word_57 = 0, move_destination_58 = 0, second_word_58 = 0, move_destination_59 = 0, second_word_59 = 0, move_destination_60 = 0, second_word_60 = 0, move_destination_61 = 0, second_word_61 = 0, move_destination_62 = 0, second_word_62 = 0, move_destination_63 = 0, second_word_63 = 0, second_word_64 = 0;

	switch (0u - (((byte_count & 0x3F8u) >> 1) - 0x200u))
	{
	case 0x0u:
		break;
	default:
		invalid_jump(0x109B4u + (0u - (((byte_count & 0x3F8u) >> 1) - 0x200u)));
	}
	move_destination = destination;
	destination += 8u;
	second_word = mem_read32(source + 4u);
	mem_write32(move_destination, mem_read32(source));
	mem_write32(move_destination + 4u, second_word);
	source += 8u;
	move_destination_2 = destination;
	destination += 8u;
	second_word_2 = mem_read32(source + 4u);
	mem_write32(move_destination_2, mem_read32(source));
	mem_write32(move_destination_2 + 4u, second_word_2);
	source += 8u;
	move_destination_3 = destination;
	destination += 8u;
	second_word_3 = mem_read32(source + 4u);
	mem_write32(move_destination_3, mem_read32(source));
	mem_write32(move_destination_3 + 4u, second_word_3);
	source += 8u;
	move_destination_4 = destination;
	destination += 8u;
	second_word_4 = mem_read32(source + 4u);
	mem_write32(move_destination_4, mem_read32(source));
	mem_write32(move_destination_4 + 4u, second_word_4);
	source += 8u;
	move_destination_5 = destination;
	destination += 8u;
	second_word_5 = mem_read32(source + 4u);
	mem_write32(move_destination_5, mem_read32(source));
	mem_write32(move_destination_5 + 4u, second_word_5);
	source += 8u;
	move_destination_6 = destination;
	destination += 8u;
	second_word_6 = mem_read32(source + 4u);
	mem_write32(move_destination_6, mem_read32(source));
	mem_write32(move_destination_6 + 4u, second_word_6);
	source += 8u;
	move_destination_7 = destination;
	destination += 8u;
	second_word_7 = mem_read32(source + 4u);
	mem_write32(move_destination_7, mem_read32(source));
	mem_write32(move_destination_7 + 4u, second_word_7);
	source += 8u;
	move_destination_8 = destination;
	destination += 8u;
	second_word_8 = mem_read32(source + 4u);
	mem_write32(move_destination_8, mem_read32(source));
	mem_write32(move_destination_8 + 4u, second_word_8);
	source += 8u;
	move_destination_9 = destination;
	destination += 8u;
	second_word_9 = mem_read32(source + 4u);
	mem_write32(move_destination_9, mem_read32(source));
	mem_write32(move_destination_9 + 4u, second_word_9);
	source += 8u;
	move_destination_10 = destination;
	destination += 8u;
	second_word_10 = mem_read32(source + 4u);
	mem_write32(move_destination_10, mem_read32(source));
	mem_write32(move_destination_10 + 4u, second_word_10);
	source += 8u;
	move_destination_11 = destination;
	destination += 8u;
	second_word_11 = mem_read32(source + 4u);
	mem_write32(move_destination_11, mem_read32(source));
	mem_write32(move_destination_11 + 4u, second_word_11);
	source += 8u;
	move_destination_12 = destination;
	destination += 8u;
	second_word_12 = mem_read32(source + 4u);
	mem_write32(move_destination_12, mem_read32(source));
	mem_write32(move_destination_12 + 4u, second_word_12);
	source += 8u;
	move_destination_13 = destination;
	destination += 8u;
	second_word_13 = mem_read32(source + 4u);
	mem_write32(move_destination_13, mem_read32(source));
	mem_write32(move_destination_13 + 4u, second_word_13);
	source += 8u;
	move_destination_14 = destination;
	destination += 8u;
	second_word_14 = mem_read32(source + 4u);
	mem_write32(move_destination_14, mem_read32(source));
	mem_write32(move_destination_14 + 4u, second_word_14);
	source += 8u;
	move_destination_15 = destination;
	destination += 8u;
	second_word_15 = mem_read32(source + 4u);
	mem_write32(move_destination_15, mem_read32(source));
	mem_write32(move_destination_15 + 4u, second_word_15);
	source += 8u;
	move_destination_16 = destination;
	destination += 8u;
	second_word_16 = mem_read32(source + 4u);
	mem_write32(move_destination_16, mem_read32(source));
	mem_write32(move_destination_16 + 4u, second_word_16);
	source += 8u;
	move_destination_17 = destination;
	destination += 8u;
	second_word_17 = mem_read32(source + 4u);
	mem_write32(move_destination_17, mem_read32(source));
	mem_write32(move_destination_17 + 4u, second_word_17);
	source += 8u;
	move_destination_18 = destination;
	destination += 8u;
	second_word_18 = mem_read32(source + 4u);
	mem_write32(move_destination_18, mem_read32(source));
	mem_write32(move_destination_18 + 4u, second_word_18);
	source += 8u;
	move_destination_19 = destination;
	destination += 8u;
	second_word_19 = mem_read32(source + 4u);
	mem_write32(move_destination_19, mem_read32(source));
	mem_write32(move_destination_19 + 4u, second_word_19);
	source += 8u;
	move_destination_20 = destination;
	destination += 8u;
	second_word_20 = mem_read32(source + 4u);
	mem_write32(move_destination_20, mem_read32(source));
	mem_write32(move_destination_20 + 4u, second_word_20);
	source += 8u;
	move_destination_21 = destination;
	destination += 8u;
	second_word_21 = mem_read32(source + 4u);
	mem_write32(move_destination_21, mem_read32(source));
	mem_write32(move_destination_21 + 4u, second_word_21);
	source += 8u;
	move_destination_22 = destination;
	destination += 8u;
	second_word_22 = mem_read32(source + 4u);
	mem_write32(move_destination_22, mem_read32(source));
	mem_write32(move_destination_22 + 4u, second_word_22);
	source += 8u;
	move_destination_23 = destination;
	destination += 8u;
	second_word_23 = mem_read32(source + 4u);
	mem_write32(move_destination_23, mem_read32(source));
	mem_write32(move_destination_23 + 4u, second_word_23);
	source += 8u;
	move_destination_24 = destination;
	destination += 8u;
	second_word_24 = mem_read32(source + 4u);
	mem_write32(move_destination_24, mem_read32(source));
	mem_write32(move_destination_24 + 4u, second_word_24);
	source += 8u;
	move_destination_25 = destination;
	destination += 8u;
	second_word_25 = mem_read32(source + 4u);
	mem_write32(move_destination_25, mem_read32(source));
	mem_write32(move_destination_25 + 4u, second_word_25);
	source += 8u;
	move_destination_26 = destination;
	destination += 8u;
	second_word_26 = mem_read32(source + 4u);
	mem_write32(move_destination_26, mem_read32(source));
	mem_write32(move_destination_26 + 4u, second_word_26);
	source += 8u;
	move_destination_27 = destination;
	destination += 8u;
	second_word_27 = mem_read32(source + 4u);
	mem_write32(move_destination_27, mem_read32(source));
	mem_write32(move_destination_27 + 4u, second_word_27);
	source += 8u;
	move_destination_28 = destination;
	destination += 8u;
	second_word_28 = mem_read32(source + 4u);
	mem_write32(move_destination_28, mem_read32(source));
	mem_write32(move_destination_28 + 4u, second_word_28);
	source += 8u;
	move_destination_29 = destination;
	destination += 8u;
	second_word_29 = mem_read32(source + 4u);
	mem_write32(move_destination_29, mem_read32(source));
	mem_write32(move_destination_29 + 4u, second_word_29);
	source += 8u;
	move_destination_30 = destination;
	destination += 8u;
	second_word_30 = mem_read32(source + 4u);
	mem_write32(move_destination_30, mem_read32(source));
	mem_write32(move_destination_30 + 4u, second_word_30);
	source += 8u;
	move_destination_31 = destination;
	destination += 8u;
	second_word_31 = mem_read32(source + 4u);
	mem_write32(move_destination_31, mem_read32(source));
	mem_write32(move_destination_31 + 4u, second_word_31);
	source += 8u;
	move_destination_32 = destination;
	destination += 8u;
	second_word_32 = mem_read32(source + 4u);
	mem_write32(move_destination_32, mem_read32(source));
	mem_write32(move_destination_32 + 4u, second_word_32);
	source += 8u;
	move_destination_33 = destination;
	destination += 8u;
	second_word_33 = mem_read32(source + 4u);
	mem_write32(move_destination_33, mem_read32(source));
	mem_write32(move_destination_33 + 4u, second_word_33);
	source += 8u;
	move_destination_34 = destination;
	destination += 8u;
	second_word_34 = mem_read32(source + 4u);
	mem_write32(move_destination_34, mem_read32(source));
	mem_write32(move_destination_34 + 4u, second_word_34);
	source += 8u;
	move_destination_35 = destination;
	destination += 8u;
	second_word_35 = mem_read32(source + 4u);
	mem_write32(move_destination_35, mem_read32(source));
	mem_write32(move_destination_35 + 4u, second_word_35);
	source += 8u;
	move_destination_36 = destination;
	destination += 8u;
	second_word_36 = mem_read32(source + 4u);
	mem_write32(move_destination_36, mem_read32(source));
	mem_write32(move_destination_36 + 4u, second_word_36);
	source += 8u;
	move_destination_37 = destination;
	destination += 8u;
	second_word_37 = mem_read32(source + 4u);
	mem_write32(move_destination_37, mem_read32(source));
	mem_write32(move_destination_37 + 4u, second_word_37);
	source += 8u;
	move_destination_38 = destination;
	destination += 8u;
	second_word_38 = mem_read32(source + 4u);
	mem_write32(move_destination_38, mem_read32(source));
	mem_write32(move_destination_38 + 4u, second_word_38);
	source += 8u;
	move_destination_39 = destination;
	destination += 8u;
	second_word_39 = mem_read32(source + 4u);
	mem_write32(move_destination_39, mem_read32(source));
	mem_write32(move_destination_39 + 4u, second_word_39);
	source += 8u;
	move_destination_40 = destination;
	destination += 8u;
	second_word_40 = mem_read32(source + 4u);
	mem_write32(move_destination_40, mem_read32(source));
	mem_write32(move_destination_40 + 4u, second_word_40);
	source += 8u;
	move_destination_41 = destination;
	destination += 8u;
	second_word_41 = mem_read32(source + 4u);
	mem_write32(move_destination_41, mem_read32(source));
	mem_write32(move_destination_41 + 4u, second_word_41);
	source += 8u;
	move_destination_42 = destination;
	destination += 8u;
	second_word_42 = mem_read32(source + 4u);
	mem_write32(move_destination_42, mem_read32(source));
	mem_write32(move_destination_42 + 4u, second_word_42);
	source += 8u;
	move_destination_43 = destination;
	destination += 8u;
	second_word_43 = mem_read32(source + 4u);
	mem_write32(move_destination_43, mem_read32(source));
	mem_write32(move_destination_43 + 4u, second_word_43);
	source += 8u;
	move_destination_44 = destination;
	destination += 8u;
	second_word_44 = mem_read32(source + 4u);
	mem_write32(move_destination_44, mem_read32(source));
	mem_write32(move_destination_44 + 4u, second_word_44);
	source += 8u;
	move_destination_45 = destination;
	destination += 8u;
	second_word_45 = mem_read32(source + 4u);
	mem_write32(move_destination_45, mem_read32(source));
	mem_write32(move_destination_45 + 4u, second_word_45);
	source += 8u;
	move_destination_46 = destination;
	destination += 8u;
	second_word_46 = mem_read32(source + 4u);
	mem_write32(move_destination_46, mem_read32(source));
	mem_write32(move_destination_46 + 4u, second_word_46);
	source += 8u;
	move_destination_47 = destination;
	destination += 8u;
	second_word_47 = mem_read32(source + 4u);
	mem_write32(move_destination_47, mem_read32(source));
	mem_write32(move_destination_47 + 4u, second_word_47);
	source += 8u;
	move_destination_48 = destination;
	destination += 8u;
	second_word_48 = mem_read32(source + 4u);
	mem_write32(move_destination_48, mem_read32(source));
	mem_write32(move_destination_48 + 4u, second_word_48);
	source += 8u;
	move_destination_49 = destination;
	destination += 8u;
	second_word_49 = mem_read32(source + 4u);
	mem_write32(move_destination_49, mem_read32(source));
	mem_write32(move_destination_49 + 4u, second_word_49);
	source += 8u;
	move_destination_50 = destination;
	destination += 8u;
	second_word_50 = mem_read32(source + 4u);
	mem_write32(move_destination_50, mem_read32(source));
	mem_write32(move_destination_50 + 4u, second_word_50);
	source += 8u;
	move_destination_51 = destination;
	destination += 8u;
	second_word_51 = mem_read32(source + 4u);
	mem_write32(move_destination_51, mem_read32(source));
	mem_write32(move_destination_51 + 4u, second_word_51);
	source += 8u;
	move_destination_52 = destination;
	destination += 8u;
	second_word_52 = mem_read32(source + 4u);
	mem_write32(move_destination_52, mem_read32(source));
	mem_write32(move_destination_52 + 4u, second_word_52);
	source += 8u;
	move_destination_53 = destination;
	destination += 8u;
	second_word_53 = mem_read32(source + 4u);
	mem_write32(move_destination_53, mem_read32(source));
	mem_write32(move_destination_53 + 4u, second_word_53);
	source += 8u;
	move_destination_54 = destination;
	destination += 8u;
	second_word_54 = mem_read32(source + 4u);
	mem_write32(move_destination_54, mem_read32(source));
	mem_write32(move_destination_54 + 4u, second_word_54);
	source += 8u;
	move_destination_55 = destination;
	destination += 8u;
	second_word_55 = mem_read32(source + 4u);
	mem_write32(move_destination_55, mem_read32(source));
	mem_write32(move_destination_55 + 4u, second_word_55);
	source += 8u;
	move_destination_56 = destination;
	destination += 8u;
	second_word_56 = mem_read32(source + 4u);
	mem_write32(move_destination_56, mem_read32(source));
	mem_write32(move_destination_56 + 4u, second_word_56);
	source += 8u;
	move_destination_57 = destination;
	destination += 8u;
	second_word_57 = mem_read32(source + 4u);
	mem_write32(move_destination_57, mem_read32(source));
	mem_write32(move_destination_57 + 4u, second_word_57);
	source += 8u;
	move_destination_58 = destination;
	destination += 8u;
	second_word_58 = mem_read32(source + 4u);
	mem_write32(move_destination_58, mem_read32(source));
	mem_write32(move_destination_58 + 4u, second_word_58);
	source += 8u;
	move_destination_59 = destination;
	destination += 8u;
	second_word_59 = mem_read32(source + 4u);
	mem_write32(move_destination_59, mem_read32(source));
	mem_write32(move_destination_59 + 4u, second_word_59);
	source += 8u;
	move_destination_60 = destination;
	destination += 8u;
	second_word_60 = mem_read32(source + 4u);
	mem_write32(move_destination_60, mem_read32(source));
	mem_write32(move_destination_60 + 4u, second_word_60);
	source += 8u;
	move_destination_61 = destination;
	destination += 8u;
	second_word_61 = mem_read32(source + 4u);
	mem_write32(move_destination_61, mem_read32(source));
	mem_write32(move_destination_61 + 4u, second_word_61);
	source += 8u;
	move_destination_62 = destination;
	destination += 8u;
	second_word_62 = mem_read32(source + 4u);
	mem_write32(move_destination_62, mem_read32(source));
	mem_write32(move_destination_62 + 4u, second_word_62);
	source += 8u;
	move_destination_63 = destination;
	destination += 8u;
	second_word_63 = mem_read32(source + 4u);
	mem_write32(move_destination_63, mem_read32(source));
	mem_write32(move_destination_63 + 4u, second_word_63);
	source += 8u;
	second_word_64 = mem_read32(source + 4u);
	mem_write32(destination, mem_read32(source));
	mem_write32(destination + 4u, second_word_64);
}

/* Unpacks the packed data at R10 into R11: flag bits read the highest first, 8 to a byte as needed; 1
   copies a byte; 01 takes a halfword (an offset back of its value >> 3, -1 to -8192, and a length of its
   low three bits + 2, or when they are 0 the next byte + 1; a 0 byte ends the data); 00 takes two bits
   more (a length of 2-5) and a byte (an offset back of -1 to -256); a length copies that many bytes from
   that far back in what has been unpacked. */
void unpack(uint32_t source, uint32_t destination)
{
	uint32_t carry = 0;
	uint8_t flags = 0, length_byte = 0;
	uint32_t back_offset = 0, copy_count = 0, copy_source = 0, flags_left = 0;
	uint32_t length_bits = 0;

	flags = mem_read8(source);
	flags_left = 9u;
	source++;
	for (;;)
	{
		flags_left--;
		if (flags_left == 0u)
		{
			flags = mem_read8(source);
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			mem_write8(destination, mem_read8(source));
			destination++;
			source++;
			continue;
		}
		back_offset = 0xFFFFFFFFu;
		flags_left--;
		if (flags_left == 0u)
		{
			flags = mem_read8(source);
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			copy_source = (back_offset & ~0xFFFFu) | mem_read16(source);
			length_byte = (uint8_t)copy_source;
			copy_source = (int32_t)copy_source >> 3;
			length_bits = length_byte & 7u;
			copy_count = length_bits;
			source += 2u;
			if (length_bits != 0u)
				copy_count += 2u;
			else
			{
				copy_count = mem_read8(source);
				source++;
				if (copy_count == 0u)
					break;
				copy_count++;
			}
		}
		else
		{
			copy_count = 0u;
			flags_left--;
			if (flags_left == 0u)
			{
				flags = mem_read8(source);
				flags_left = 8u;
				source++;
			}
			carry = (uint8_t)(flags * 2u) < flags;
			flags = (uint8_t)(flags * 2u);
			copy_count = copy_count * 2u + carry;
			flags_left--;
			if (flags_left == 0u)
			{
				flags = mem_read8(source);
				flags_left = 8u;
				source++;
			}
			carry = (uint8_t)(flags * 2u) < flags;
			flags = (uint8_t)(flags * 2u);
			copy_count = copy_count * 2u + carry;
			copy_source = (back_offset & ~0xFFu) | mem_read8(source);
			source++;
			copy_count += 2u;
		}
		copy_source += destination;
		do
		{
			mem_write8(destination, mem_read8(copy_source));
			destination++;
			copy_count--;
			copy_source++;
		} while (copy_count != 0u);
	}
}

/* Unpacks a little of the screen's queued tile pages (queue_tile_pages), from the frame loop's wait
   while unpack_state says there is work: with none under way, takes the next request of the ring
   unpack_queue (unpack_read_place; unpack_state 0 when it is empty); else goes on where it stopped, by
   unpack_state's second byte: FF one byte of a match (unpack_copy_source, unpack_copy_count), 1 one step
   of unpack's format (unpack_flags, unpack_flags_left, unpack_source, unpack_destination): a byte, or a
   match's first byte. */
uint32_t unpack_queued_step(uint32_t copy_source)
{
	uint32_t carry = 0;
	uint8_t copy_count = 0, flags = 0, length_byte = 0;
	uint32_t back_offset = 0, destination = 0, flags_left = 0, request = 0, source = 0, unpack_copy_source = 0, unpack_destination = 0, unpack_read_place = 0;
	uint8_t unpack_copy_count = 0;
	uint32_t length_bits = 0;

	if ((int8_t)(uint8_t)(SCREEN(screen)->unpack_state >> 8u) <= 0)
	{
		if ((uint8_t)(SCREEN(screen)->unpack_state >> 8u) != 0u)
		{
			unpack_copy_source = SCREEN(screen)->unpack_copy_source;
			unpack_destination = SCREEN(screen)->unpack_destination;
			mem_write8(unpack_destination, mem_read8(unpack_copy_source));
			SCREEN(screen)->unpack_copy_source = unpack_copy_source + 1u;
			SCREEN(screen)->unpack_destination = unpack_destination + 1u;
			unpack_copy_count = SCREEN(screen)->unpack_copy_count;
			SCREEN(screen)->unpack_copy_count = unpack_copy_count - 1u;
			if (unpack_copy_count == 1u)
			{
				SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0x100u;
				return copy_source;
			}
			return copy_source;
		}
		unpack_read_place = SCREEN(screen)->unpack_read_place;
		if (unpack_read_place == SCREEN(screen)->unpack_write_place)
		{
			SCREEN(screen)->unpack_state = 0u;
			return copy_source;
		}
		request = screen + 0xC00u + unpack_read_place * 2u * 4u;
		SCREEN(screen)->unpack_read_place = (unpack_read_place + 1u) & 0x3Fu;
		source = mem_read32(request);
		destination = mem_read32(request + 4u);
		flags = mem_read8(source);
		flags_left = 9u;
		SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0x100u;
		source++;
	}
	else
	{
		flags = SCREEN(screen)->unpack_flags;
		flags_left = SCREEN(screen)->unpack_flags_left;
		source = SCREEN(screen)->unpack_source;
		destination = SCREEN(screen)->unpack_destination;
	}
	for (;;)
	{
		flags_left--;
		if (flags_left == 0u)
		{
			flags = mem_read8(source);
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			mem_write8(destination, mem_read8(source));
			SCREEN(screen)->unpack_flags = flags;
			SCREEN(screen)->unpack_flags_left = flags_left;
			SCREEN(screen)->unpack_source = source + 1u;
			SCREEN(screen)->unpack_destination = destination + 1u;
			return copy_source;
		}
		back_offset = 0xFFFFFFFFu;
		flags_left--;
		if (flags_left == 0u)
		{
			flags = mem_read8(source);
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry == 0u)
			break;
		copy_source = (back_offset & ~0xFFFFu) | mem_read16(source);
		length_byte = (uint8_t)copy_source;
		copy_source = (int32_t)copy_source >> 3;
		length_bits = length_byte & 7u;
		copy_count = (uint8_t)length_bits;
		source += 2u;
		if (length_bits != 0u)
		{
			copy_count++;
			copy_source += destination;
			mem_write8(destination, mem_read8(copy_source));
			SCREEN(screen)->unpack_flags = flags;
			SCREEN(screen)->unpack_flags_left = flags_left;
			copy_source++;
			SCREEN(screen)->unpack_copy_source = copy_source;
			SCREEN(screen)->unpack_copy_count = copy_count;
			SCREEN(screen)->unpack_source = source;
			SCREEN(screen)->unpack_destination = destination + 1u;
			SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0xFF00u;
			return copy_source;
		}
		copy_count = mem_read8(source);
		source++;
		if (copy_count != 0u)
		{
			copy_source += destination;
			mem_write8(destination, mem_read8(copy_source));
			SCREEN(screen)->unpack_flags = flags;
			SCREEN(screen)->unpack_flags_left = flags_left;
			copy_source++;
			SCREEN(screen)->unpack_copy_source = copy_source;
			SCREEN(screen)->unpack_copy_count = copy_count;
			SCREEN(screen)->unpack_source = source;
			SCREEN(screen)->unpack_destination = destination + 1u;
			SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0xFF00u;
			return copy_source;
		}
		unpack_read_place = SCREEN(screen)->unpack_read_place;
		if (unpack_read_place == SCREEN(screen)->unpack_write_place)
		{
			SCREEN(screen)->unpack_state = 0u;
			return copy_source;
		}
		request = screen + 0xC00u + unpack_read_place * 2u * 4u;
		SCREEN(screen)->unpack_read_place = (unpack_read_place + 1u) & 0x3Fu;
		source = mem_read32(request);
		destination = mem_read32(request + 4u);
		flags = mem_read8(source);
		flags_left = 9u;
		SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0x100u;
		source++;
	}
	copy_count = 0u;
	flags_left--;
	if (flags_left == 0u)
	{
		flags = mem_read8(source);
		flags_left = 8u;
		source++;
	}
	carry = (uint8_t)(flags * 2u) < flags;
	flags = (uint8_t)(flags * 2u);
	copy_count = (uint8_t)(copy_count * 2u + carry);
	flags_left--;
	if (flags_left == 0u)
	{
		flags = mem_read8(source);
		flags_left = 8u;
		source++;
	}
	carry = (uint8_t)(flags * 2u) < flags;
	flags = (uint8_t)(flags * 2u);
	copy_count = (uint8_t)(copy_count * 2u + carry);
	copy_source = (back_offset & ~0xFFu) | mem_read8(source);
	source++;
	copy_count++;
	copy_source += destination;
	mem_write8(destination, mem_read8(copy_source));
	SCREEN(screen)->unpack_flags = flags;
	SCREEN(screen)->unpack_flags_left = flags_left;
	copy_source++;
	SCREEN(screen)->unpack_copy_source = copy_source;
	SCREEN(screen)->unpack_copy_count = copy_count;
	SCREEN(screen)->unpack_source = source;
	SCREEN(screen)->unpack_destination = destination + 1u;
	SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFFu) | 0xFF00u;
	return copy_source;
}

/* Unused: unpacks a list of packed blocks at once (unpack): 8-byte entries, the packed data's address
   and where to unpack it, closed by a negative entry (the first is taken without the check).
   unpack_tile_pages does the same for a list of tile pages. */
void unpack_list(uint32_t block_list)
{
	uint32_t destination = 0, source = 0;
	uint32_t next_entry = 0;

	do
	{
		r12 = mem_read32(block_list);
		block_list += 4u;
		destination = mem_read32(block_list);
		next_entry = block_list + 4u;
		source = r12;
		r12 = next_entry;
		unpack(source, destination);
		block_list = r12;
	} while ((int32_t)mem_read32(block_list) >= 0);
}

/* Unpacks a list of tilemap pages into video RAM at once (unpack): each 32-bit entry a page number in
   its top byte (the page at 300000 + 400 * page) and the address of its packed data below, closed by a
   negative entry; the list format of queue_tile_pages. */
void unpack_tile_pages(uint32_t page_list)
{
	uint32_t destination = 0, page_bits = 0, source = 0;
	uint32_t next_entry = 0;

	while ((int32_t)mem_read32(page_list) >= 0)
	{
		page_bits = mem_read32(page_list);
		r12 = page_bits;
		page_bits &= ~0xFFFFFFu;
		destination = 0x300000u + (byte_swap32(page_bits) << 8) * 4u;
		next_entry = page_list + 4u;
		source = r12 ^ page_bits;
		r12 = next_entry;
		unpack(source, destination);
		page_list = r12;
	}
}

/* Unused: queues the entries of an unpack_list list (the packed data's address, where to unpack it;
   closed by a negative entry, the first taken unchecked) as they are in the screen's ring unpack_queue
   (unpack_write_place), with unpack_state set to say there is work, for unpack_queued_step.
   queue_tile_pages does the same for a list of tile pages. */
void queue_unpack_list(uint32_t block_list)
{
	uint32_t request = 0, unpack_write_place = 0;
	uint32_t destination = 0;

	do
	{
		unpack_write_place = SCREEN(screen)->unpack_write_place;
		request = screen + 0xC00u + unpack_write_place * 2u * 4u;
		SCREEN(screen)->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		destination = mem_read32(block_list + 4u);
		mem_write32(request, mem_read32(block_list));
		mem_write32(request + 4u, destination);
		SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFF00u) | 0xFFu;
		block_list += 8u;
	} while ((int32_t)mem_read32(block_list) >= 0);
}

/* Queues the tilemap pages of the list R10 to be unpacked into video RAM: each 32-bit entry a page
   number in its top byte (the page at 300000 + 400 * page) and the address of its packed data below,
   closed by a negative entry; the requests go into the screen's ring of 64 at C00 (unpack_write_place
   the place) and unpack_state says there is work. Used by set_up_background, ranking_background_load and
   race_background_update. */
void queue_tile_pages(uint32_t page_list)
{
	uint32_t page_bits = 0, page_entry = 0, request = 0, unpack_write_place = 0;

	while ((int32_t)mem_read32(page_list) >= 0)
	{
		unpack_write_place = SCREEN(screen)->unpack_write_place;
		request = screen + 0xC00u + unpack_write_place * 2u * 4u;
		SCREEN(screen)->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		page_bits = mem_read32(page_list);
		page_entry = page_bits;
		page_bits &= ~0xFFFFFFu;
		mem_write32(request, page_entry ^ page_bits);
		mem_write32(request + 4u, 0x300000u + (byte_swap32(page_bits) << 8) * 4u);
		SCREEN(screen)->unpack_state = (SCREEN(screen)->unpack_state & 0xFF00u) | 0xFFu;
		page_list += 4u;
	}
}

/* Sets up the coins: chute_entry_offset 14 (the two chutes shared: the second's coins counted as the
   first's, on its meter) when not on free play and both chutes give the same units
   (first_chute_multiplier = second_chute_multiplier), else 0; the seats' credits back from the
   bookkeeping (701F74-701F7A, bytes); and the low four bits of I/O chip 1's port F, inverted, into
   second_io_chip_port_f (nothing reads it). */
void init_coins(void)
{
	uint8_t chip_2_port_f = 0;

	chip_2_port_f = mem_read8(0xC8000Au);
	second_io_chip_port_f = ~chip_2_port_f & 0xFu;
	chute_entry_offset = 0u;
	if (free_play == 0u && second_chute_multiplier == first_chute_multiplier)
		chute_entry_offset = 0x14u;
	seat1_credits = mem_read8(SOUND_701F74);
	seat1_bookkeeping_byte = mem_read8(SOUND_701F76);
	seat2_credits = mem_read8(SOUND_701F78);
	seat2_bookkeeping_byte = mem_read8(SOUND_701F7A);
}

/* The coins and service credits, in the vertical blank unless on free play (free_play): queues each
   seat's pending coin sound (4E, seat1_coin_sounds, seat2_coin_sounds) and service sound (50,
   seat1_service_sounds, seat2_service_sounds), one each vertical blank; SERVICE newly pressed
   (switches_pressed bits 0-1) gives that seat a credit (add_service_credit), counted in the
   bookkeeping's service credits (701F04); a coin chute let go (switches_released bits 2-3 seat 1's two
   chutes, 4-5 seat 2's) counts its coin (insert_coin, entries at 0114D4) and the credits it gives in the
   bookkeeping (701F02); then drives the coin meters (pulse_coin_meter: each seat's first, and its second
   unless the chutes are shared, chute_entry_offset). */
void update_coins(uint32_t calibration)
{
	uint8_t meter_bit = 0, sound = 0;
	uint32_t chute_entry = 0, coin_meter = 0, counter = 0, credits_given = 0, pressed_switches = 0, released_switches = 0, seat_credits = 0, service_sounds = 0;
	uint32_t saved_screen = 0;
	uint8_t released_chutes = 0;

	if (free_play != 0u)
		return;
	saved_screen = screen;
	if (seat1_coin_sounds != 0u)
	{
		sound = 0x4Eu;
		screen = SCREEN_1;
		queue_sound(sound);
		seat1_coin_sounds--;
	}
	if (seat2_coin_sounds != 0u)
	{
		sound = 0x4Eu;
		screen = SCREEN_2;
		queue_sound(sound);
		seat2_coin_sounds--;
	}
	if (seat1_service_sounds != 0u)
	{
		sound = 0x50u;
		screen = SCREEN_1;
		queue_sound(sound);
		seat1_service_sounds--;
	}
	if (seat2_service_sounds != 0u)
	{
		sound = 0x50u;
		screen = SCREEN_2;
		queue_sound(sound);
		seat2_service_sounds--;
	}
	screen = saved_screen;
	pressed_switches = (calibration & ~0xFFu) | switches_pressed;
	credits_given = 0u;
	if ((pressed_switches & 1u) != 0u)
	{
		seat_credits = seat1_credits_ADDRESS;
		service_sounds = seat1_service_sounds_ADDRESS;
		r12 = SCREEN_1;
		credits_given = add_service_credit(credits_given, seat_credits, service_sounds);
		counter = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(counter, (uint16_t)credits_given);
	}
	credits_given = 0u;
	if ((pressed_switches & 2u) != 0u)
	{
		seat_credits = seat2_credits_ADDRESS;
		service_sounds = seat2_service_sounds_ADDRESS;
		r12 = SCREEN_2;
		credits_given = add_service_credit(credits_given, seat_credits, service_sounds);
		counter = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(counter, (uint16_t)credits_given);
	}
	released_switches = (pressed_switches & ~0xFFu) | switches_released;
	released_chutes = released_switches & 0x3Cu;
	released_switches = (released_switches & ~0xFFu) | released_chutes;
	if (released_chutes != 0u)
	{
		credits_given = 0u;
		r12 = SCREEN_1;
		chute_entry = seat1_first_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		chute_entry = seat1_second_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		r12 = SCREEN_2;
		chute_entry = seat2_first_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		chute_entry = seat2_second_chute_entry_ADDRESS;
		credits_given = insert_coin(released_switches, credits_given, chute_entry);
		counter = SOUND_RAM + 0x1F02u;
		bookkeeping_add16(counter, (uint16_t)credits_given);
	}
	if (chute_entry_offset == 0u)
	{
		coin_meter = 0x20E083u;
		meter_bit = 1u;
		r12 = port_d_outputs_ADDRESS;
		pulse_coin_meter(meter_bit, coin_meter);
	}
	coin_meter = coin_meters_ADDRESS;
	meter_bit = 0u;
	r12 = port_d_outputs_ADDRESS;
	pulse_coin_meter(meter_bit, coin_meter);
	if (chute_entry_offset == 0u)
	{
		coin_meter = 0x20E089u;
		meter_bit = 1u;
		r12 = port_d_outputs_ADDRESS + 0x1u;
		pulse_coin_meter(meter_bit, coin_meter);
	}
	coin_meter = 0x20E086u;
	meter_bit = 0u;
	r12 = port_d_outputs_ADDRESS + 0x1u;
	pulse_coin_meter(meter_bit, coin_meter);
}

/* Drives coin meter R1 (its bit in the port D outputs at R12, port_d_outputs: bit 0 a seat's first
   chute's meter, bit 1 its second's) from its record R7 in coin_meters (frames on, frames off, pulses
   owed): when idle with a pulse owed, the bit goes on for 6 vertical blanks and off for 6. Then writes
   the credits into the bookkeeping and its backup (write_bookkeeping_credits) with the checksum. */
void pulse_coin_meter(uint8_t meter_bit, uint32_t coin_meter)
{
	uint32_t meter_frames = 0;
	uint8_t frames_on = 0;

	meter_frames = mem_read16(coin_meter);
	coin_meter += 2u;
	if (meter_frames == 0u)
	{
		if (mem_read8(coin_meter) != 0u)
		{
			mem_write8(coin_meter, mem_read8(coin_meter) - 1u);
			mem_write8(r12, mem_read8(r12) | shift_by(1u, (uint8_t)meter_bit, 8));
			mem_write16(coin_meter - 2u, 0x606u);
		}
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(meter_frames);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	if ((uint8_t)meter_frames == 0u)
	{
		coin_meter--;
		mem_write8(coin_meter, mem_read8(coin_meter) - 1u);
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(meter_frames);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	frames_on = mem_read8(coin_meter - 2u);
	mem_write8(coin_meter - 2u, frames_on - 1u);
	if (frames_on != 1u)
	{
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(meter_frames);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	mem_write8(r12, mem_read8(r12) & rotate_by(0xFEu, (uint8_t)meter_bit, 8));
	r12 = SOUND_701F74;
	write_bookkeeping_credits();
	bookkeeping_update_checksum(meter_frames);
	r12 = SOUND_701FF0;
	write_bookkeeping_credits();
}

/* Writes the seats' credits (the halfwords at seat1_credits, seat1_bookkeeping_byte, seat2_credits,
   seat2_bookkeeping_byte) at R12: 701F74 in the bookkeeping or 701FF0 in its backup. */
void write_bookkeeping_credits(void)
{
	mem_write16(r12, (seat1_credits | ((uint32_t)seat1_coin_units << 8u)));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(seat1_bookkeeping_byte));
	r12 = r12 + 2u;
	mem_write16(r12, (seat2_credits | ((uint32_t)seat2_coin_units << 8u)));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(seat2_bookkeeping_byte));
	r12 = r12 + 2u;
}

/* A service credit for the seat whose credits are at R10 (seat1_credits or seat2_credits): one credit
   more, up to 9 (at 9 its coin units are cleared; beyond, nothing); a credit given brings the screen R12
   to the title from the attract mode (show_title_on_credit), asks for the seat's service sound (R11) and
   counts one in R9, which it returns. Not run in the scenarios. */
uint32_t add_service_credit(uint32_t credits_given, uint32_t seat_credits, uint32_t service_sounds)
{
	uint32_t zero = 0;
	uint8_t credits = 0;

	mem_write8(seat_credits, mem_read8(seat_credits) + 1u);
	credits = mem_read8(seat_credits);
	zero = (uint8_t)(credits - 9u) == 0u;
	if (credits >= 9u)
	{
		mem_write8(seat_credits + 2u, 0u);
		mem_write8(seat_credits + 1u, 0u);
		if (zero == 0u)
		{
			mem_write8(seat_credits, 9u);
			return credits_given;
		}
	}
	show_title_on_credit();
	mem_write8(service_sounds, 1u);
	credits_given = (credits_given & ~0xFFu) | (uint8_t)(credits_given + 1u);
	return credits_given;
}

/* A coin from the chute of entry R19 (when its bit is in R6): takes the entry's variant
   (chute_entry_offset: with the chutes shared, the second counts as the first), writes its byte to I/O
   C00074, owes the chute's coin meter a pulse (coin_meters) and counts the coin in the bookkeeping
   (701F06-701F0C, bookkeeping_increment16); with fewer than 9 credits: the screen's coin sound, and the
   chute's coin units (first_chute_multiplier or second_chute_multiplier) go toward a bonus unit (the
   credit record's third byte; one more unit each bonus_adder, when set) and toward a credit (its second
   byte, seat1_coin_units: a credit each coins_per_credit, up to 9, show_title_on_credit). Returns R9
   plus the credits given. */
uint32_t insert_coin(uint32_t released_switches, uint32_t credits_given, uint32_t chute_entry)
{
	uint8_t bonus_units = 0, chute_units = 0, credit_units = 0, units_per_credit = 0;
	uint32_t coin_sounds = 0, counter = 0, seat_credits = 0, units_per_bonus = 0;
	uint32_t switch_bit = 0, owed_pulses = 0;

	switch_bit = mem_read32(chute_entry);
	chute_entry += 4u;
	if ((released_switches & shl32(1u, (int)(switch_bit))) == 0u)
		return credits_given;
	coin_sounds = mem_read32(chute_entry);
	chute_entry = chute_entry + 4u + chute_entry_offset;
	seat_credits = mem_read32(chute_entry);
	mem_write8(IO_C00074, mem_read8(chute_entry + 0x10u));
	owed_pulses = mem_read32(chute_entry + 4u);
	mem_write8(owed_pulses, mem_read8(owed_pulses) + 1u);
	counter = mem_read32(chute_entry + 8u);
	bookkeeping_increment16(counter);
	if (mem_read8(seat_credits) >= 9u)
		return credits_given;
	mem_write8(coin_sounds, mem_read8(coin_sounds) + 1u);
	chute_units = mem_read8(mem_read32(chute_entry + 0xCu));
	units_per_bonus = bonus_adder;
	if (units_per_bonus != 0u)
	{
		bonus_units = (uint8_t)(mem_read8(seat_credits + 2u) + chute_units);
		while (units_per_bonus <= bonus_units)
		{
			mem_write8(seat_credits + 1u, mem_read8(seat_credits + 1u) + 1u);
			bonus_units -= units_per_bonus;
		}
		mem_write8(seat_credits + 2u, bonus_units);
	}
	units_per_credit = coins_per_credit;
	credit_units = (uint8_t)(mem_read8(seat_credits + 1u) + chute_units);
	do
	{
		if (credit_units < units_per_credit)
		{
			mem_write8(seat_credits + 1u, credit_units);
			return credits_given;
		}
		mem_write8(seat_credits, mem_read8(seat_credits) + 1u);
		show_title_on_credit();
		credits_given = (credits_given & ~0xFFu) | (uint8_t)(credits_given + 1u);
		credit_units -= units_per_credit;
	} while (mem_read8(seat_credits) < 9u);
	credit_units = 0u;
	mem_write8(seat_credits + 2u, 0u);
	mem_write8(seat_credits + 1u, credit_units);
	return credits_given;
}

/* A credit on the screen R12 during the attract mode (game modes 1-5, the low byte of its unk_80) brings
   it to the title: unk_80 = 2 (game mode 2, game_mode_title, from its first step). */
void show_title_on_credit(void)
{
	if ((int8_t)SCREEN(r12)->game_mode > 0 && (int8_t)SCREEN(r12)->game_mode < 6)
		AS32(SCREEN(r12)->game_mode) = 2u;
}

/* Adds one to the 16-bit bookkeeping counter at R7 (and its backup's, 7C on) unless it is at FFFF, with
   the checksum: the coins by chute (insert_coin), the games by seat and the races' results
   (game_mode_play). */
void bookkeeping_increment16(uint32_t counter)
{
	if (mem_read16(counter) != 0xFFFFu)
	{
		mem_write16(counter, mem_read16(counter) + 1u);
		bookkeeping_update_checksum(0u);
		mem_write16(counter + 0x7Cu, mem_read16(counter + 0x7Cu) + 1u);
	}
}

/* Unused: adds one to the 8-bit bookkeeping counter at R7 (and its backup's, 7C on) unless it is at FF,
   with the checksum (bookkeeping_update_checksum). */
void bookkeeping_increment8(uint32_t counter_address)
{
	if (mem_read8(counter_address) != 0xFFu)
	{
		mem_write8(counter_address, mem_read8(counter_address) + 1u);
		bookkeeping_update_checksum(0u);
		mem_write8(counter_address + 0x7Cu, mem_read8(counter_address + 0x7Cu) + 1u);
	}
}

/* Adds R9 to the 16-bit bookkeeping counter at R7 (the credits from coins, 701F02, and the service
   credits, 701F04: update_coins) and to its backup's (7C on), with the checksum; past FFFF it stays at
   FFFF (and the backup's is left as it was: the code writes R7 twice). */
void bookkeeping_add16(uint32_t counter, uint16_t amount)
{
	uint32_t carry = 0;
	uint16_t count = 0;

	count = mem_read16(counter);
	carry = (uint16_t)(count + amount) < count;
	mem_write16(counter, count + amount);
	if (carry)
	{
		mem_write16(counter, 0xFFFFu);
		bookkeeping_update_checksum(0u);
		mem_write16(counter, 0xFFFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write16(counter + 0x7Cu, mem_read16(counter + 0x7Cu) + amount);
}

/* Adds R9 to the 32-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum; past
   FFFFFFFF it stays there (the backup's left as it was): the play time by seat (game_mode_play). */
void bookkeeping_add32(uint32_t counter, uint32_t amount)
{
	uint32_t carry = 0;
	uint32_t count = 0;

	count = mem_read32(counter);
	carry = count + amount < count;
	mem_write32(counter, count + amount);
	if (carry)
	{
		mem_write32(counter, 0xFFFFFFFFu);
		bookkeeping_update_checksum(0u);
		mem_write32(counter, 0xFFFFFFFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write32(counter + 0x7Cu, mem_read32(counter + 0x7Cu) + amount);
}

/* Unused: adds R9 to the 8-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum;
   past FF it stays at FF (and the backup's is left as it was: the code writes R7 twice), as
   bookkeeping_add16 does. */
void bookkeeping_add8(uint32_t counter, uint8_t amount)
{
	uint32_t carry = 0;
	uint8_t count = 0;

	count = mem_read8(counter);
	carry = (uint8_t)(count + amount) < count;
	mem_write8(counter, count + amount);
	if (carry)
	{
		mem_write8(counter, 0xFFu);
		bookkeeping_update_checksum(0u);
		mem_write8(counter, 0xFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write8(counter + 0x7Cu, mem_read8(counter + 0x7Cu) + amount);
}

/* Sets the 16-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum: the longest
   and shortest play (701F20, 701F22; game_mode_play). */
void bookkeeping_set16(uint32_t counter, uint16_t amount)
{
	mem_write16(counter, amount);
	bookkeeping_update_checksum(0u);
	mem_write16(counter + 0x7Cu, amount);
}

/* Unused: sets the 32-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set32(uint32_t counter, uint32_t amount)
{
	mem_write32(counter, amount);
	bookkeeping_update_checksum(0u);
	mem_write32(counter + 0x7Cu, amount);
}

/* Unused: sets the 8-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set8(uint32_t counter, uint8_t amount)
{
	mem_write8(counter, amount);
	bookkeeping_update_checksum(0u);
	mem_write8(counter + 0x7Cu, amount);
}

/* Clears the credits kept in the bookkeeping (701F74-701F7B) and in its backup (701FF0-701FF7), with the
   checksum. Not run in the scenarios. */
void bookkeeping_clear_credits(void)
{
	mem_write32(SOUND_701F74, 0u);
	mem_write32(SOUND_701F78, 0u);
	bookkeeping_update_checksum(0u);
	mem_write32(SOUND_701FF0, 0u);
	mem_write32(SOUND_701FF4, 0u);
	r12 = SOUND_RAM + 0x1FF8u;
}

/* Checks the bookkeeping at boot: when the block (701F02) matches the checksum (701F00), copies it over
   the backup (701F7E); else when the backup matches, the backup over the block; else resets both
   (bookkeeping_reset). */
void bookkeeping_check(void)
{
	uint16_t stored_checksum = 0;
	uint32_t block = 0, checksum = 0, destination = 0, source = 0;

	stored_checksum = mem_read16(SOUND_701F00);
	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(0u, block);
	if ((uint16_t)checksum == stored_checksum)
	{
		source = SOUND_RAM + 0x1F02u;
		destination = SOUND_RAM + 0x1F7Eu;
		checksum = bookkeeping_copy(checksum, source, destination);
		return;
	}
	block = SOUND_RAM + 0x1F7Eu;
	checksum = bookkeeping_checksum(checksum, block);
	if ((uint16_t)checksum == stored_checksum)
	{
		destination = SOUND_RAM + 0x1F02u;
		source = SOUND_RAM + 0x1F7Eu;
		checksum = bookkeeping_copy(checksum, source, destination);
		return;
	}
	checksum = bookkeeping_reset(checksum);
}

/* Resets the bookkeeping: the defaults at 011458 into the block (701F02), then the block into its backup
   (701F7E), with the checksum. */
uint32_t bookkeeping_reset(uint32_t checksum)
{
	uint32_t destination = 0, source = 0;

	source = bookkeeping_defaults_ADDRESS;
	destination = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_copy(checksum, source, destination);
	source = SOUND_RAM + 0x1F02u;
	destination = SOUND_RAM + 0x1F7Eu;
	return bookkeeping_copy(checksum, source, destination);
}

/* Copies 3E halfwords from R2 over R3 (the bookkeeping and its backup at 701F7E, either way, or the
   defaults), then bookkeeping_update_checksum. */
uint32_t bookkeeping_copy(uint32_t checksum, uint32_t source, uint32_t destination)
{
	move_string(source, 0x3Eu, destination, 0x3Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	return bookkeeping_update_checksum(checksum);
}

/* Writes the checksum of the bookkeeping (701F02-701F7D in the shared sound RAM) into 701F00, after a
   change to it; returns it. */
uint32_t bookkeeping_update_checksum(uint32_t checksum)
{
	uint32_t block = 0;

	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(checksum, block);
	mem_write16(SOUND_701F00, checksum);
	return checksum;
}

/* Unused: checks the bookkeeping block (701F02) against its checksum (701F00) and resets both it and its
   backup (bookkeeping_reset) when they differ; unlike bookkeeping_check it does not try the backup. */
void bookkeeping_check_block(void)
{
	uint32_t block = 0, checksum = 0;

	block = SOUND_RAM + 0x1F02u;
	checksum = bookkeeping_checksum(0u, block);
	if (mem_read16(SOUND_701F00) != (uint16_t)checksum)
	{
		checksum = bookkeeping_reset(checksum);
		return;
	}
}

/* Returns the checksum of the 3E halfwords at R2 (the bookkeeping or its backup): from 1, each halfword
   XORed in, then a shift right by one, with 8810 XORed in when the bit shifted out was 1. */
uint32_t bookkeeping_checksum(uint32_t checksum, uint32_t block)
{
	uint32_t carry = 0;
	uint32_t halfwords_left = 0, sum = 0;

	sum = (checksum & ~0xFFFFu) | 1u;
	halfwords_left = 0x3Eu;
	do
	{
		sum = (sum & ~0xFFFFu) | (uint16_t)(sum ^ mem_read16(block));
		carry = sum & 1u;
		sum = (sum & ~0xFFFFu) | ((uint16_t)sum >> 1);
		block += 2u;
		if (carry)
			sum = (sum & ~0xFFFFu) | (uint16_t)(sum ^ 0x8810u);
		halfwords_left--;
	} while (halfwords_left != 0u);
	return sum;
}

/* Uploads three blocks of colours straight into the screen's palette RAM at boot (upload_palette): 256
   colours at 9E00 (03153C), 16 at 9C00 (031740) and 2560 at 8000 (153904). */
void upload_boot_palettes(void)
{
	uint32_t colour_block = 0;

	colour_block = boot_palette_9e00_ADDRESS;
	upload_palette(colour_block);
	colour_block = black_background_palette_ADDRESS;
	upload_palette(colour_block);
	colour_block = boot_palette_8000_ADDRESS;
	upload_palette(colour_block);
}

/* Queues the palette of the stage the screen goes to (scene_palettes by screen route_stage) through
   queue_palette (the palette queue at screen + 900). Used by player_car_extend_course and the game's
   code (002E74). */
void queue_stage_load(void)
{
	uint32_t colour_block = 0;

	colour_block = scene_palettes[SCREEN(screen)->route_stage];
	queue_palette(colour_block);
}

/* Queues a block of colours (record R19: its place in the palette, its length in bytes, the colours) for
   the screen's palette RAM, copied in the vertical blank (upload_queued_palettes). */
void queue_palette(uint32_t colour_block)
{
	uint32_t list_flag = 0;

	list_flag = 0u;
	queue_palette_upload(list_flag, colour_block);
}

/* Queues a list of colour blocks (record R19: its length in bytes, then 8-byte entries: the colours'
   address, their place in the palette, how many blocks of 16) for the screen's palette RAM, copied in
   the vertical blank (upload_queued_palettes). */
void queue_palette_list(uint32_t colour_list)
{
	uint32_t list_flag = 0;

	list_flag = 0x80000000u;
	queue_palette_upload(list_flag, colour_list);
}

/* Adds R19 (bit 31 from R8: a list, not a block) to the screen's queue of palette uploads (the count at
   palette_upload_count, the entries after it, up to 128). */
void queue_palette_upload(uint32_t list_flag, uint32_t colour_record)
{
	uint32_t upload_count = 0, upload_queue = 0;

	upload_queue = screen + 0x900u;
	if ((int32_t)SCREEN(upload_queue - 0x900u)->palette_upload_count < 0x80)
	{
		upload_count = SCREEN(upload_queue - 0x900u)->palette_upload_count + 1u;
		mem_write32(upload_queue + upload_count * 4u, colour_record | list_flag);
		SCREEN(upload_queue - 0x900u)->palette_upload_count = upload_count;
	}
}

/* Copies the queued palette uploads (queue_palette, queue_palette_list) into the screen's palette RAM in
   the vertical blank and empties the queue. */
void upload_queued_palettes(void)
{
	uint32_t colour_record = 0, upload_queue = 0, uploads_left = 0;
	uint32_t queued_upload = 0;

	upload_queue = screen + 0x900u;
	uploads_left = SCREEN(upload_queue - 0x900u)->palette_upload_count;
	upload_queue += 4u;
	for (;;)
	{
		queued_upload = mem_read32(upload_queue);
		colour_record = queued_upload & 0x7FFFFFFFu;
		upload_queue += 4u;
		if ((queued_upload & 0x80000000u) == 0u)
		{
			upload_palette(colour_record);
			uploads_left--;
			if (uploads_left == 0u)
				break;
			continue;
		}
		upload_palette_list(colour_record);
		uploads_left--;
		if (uploads_left == 0u)
			break;
	}
	SCREEN(screen)->palette_upload_count = uploads_left;
}

/* Copies a block of colours (place, length in bytes, colours) into the screen's palette RAM (600000 for
   screen 1, 680000 for screen 2). */
void upload_palette(uint32_t colour_block)
{
	uint32_t blocks_left = 0, palette_address = 0;
	uint32_t colours_0_place = 0, colours_2_3 = 0, colours_4_place = 0, colours_6_7 = 0, colours_8_place = 0, colours_10_11 = 0, colours_12_place = 0, colours_14_15 = 0;

	palette_address = SCREEN(screen)->index;
	palette_address = (palette_address << 19) | (palette_address >> 13) | 0x600000u | mem_read16(colour_block);
	colour_block += 2u;
	blocks_left = mem_read16(colour_block);
	blocks_left = blocks_left >> 5;
	colour_block += 2u;
	do
	{
		colours_0_place = palette_address;
		palette_address += 8u;
		colours_2_3 = mem_read32(colour_block + 4u);
		mem_write32(colours_0_place, mem_read32(colour_block));
		mem_write32(colours_0_place + 4u, colours_2_3);
		colour_block += 8u;
		colours_4_place = palette_address;
		palette_address += 8u;
		colours_6_7 = mem_read32(colour_block + 4u);
		mem_write32(colours_4_place, mem_read32(colour_block));
		mem_write32(colours_4_place + 4u, colours_6_7);
		colour_block += 8u;
		colours_8_place = palette_address;
		palette_address += 8u;
		colours_10_11 = mem_read32(colour_block + 4u);
		mem_write32(colours_8_place, mem_read32(colour_block));
		mem_write32(colours_8_place + 4u, colours_10_11);
		colour_block += 8u;
		colours_12_place = palette_address;
		palette_address += 8u;
		colours_14_15 = mem_read32(colour_block + 4u);
		mem_write32(colours_12_place, mem_read32(colour_block));
		mem_write32(colours_12_place + 4u, colours_14_15);
		blocks_left--;
		colour_block += 8u;
	} while (blocks_left != 0u);
}

/* Copies a list of colour blocks (the list's length in bytes, then the address, place and number of
   32-byte blocks of each) into the screen's palette RAM. */
void upload_palette_list(uint32_t colour_list)
{
	uint32_t blocks_left = 0, colour_source = 0, entries_left = 0, palette_address = 0;
	uint32_t colours_2_3 = 0, colours_6_7 = 0, colours_10_11 = 0, colours_14_15 = 0;

	palette_address = SCREEN(screen)->index;
	palette_address = (palette_address << 19) | (palette_address >> 13) | 0x600000u;
	entries_left = mem_read32(colour_list) >> 3;
	colour_list += 4u;
	do
	{
		colour_source = mem_read32(colour_list);
		colour_list += 4u;
		palette_address = (palette_address & ~0xFFFFu) | mem_read16(colour_list);
		colour_list += 2u;
		blocks_left = mem_read16(colour_list);
		colour_list += 2u;
		do
		{
			colours_2_3 = mem_read32(colour_source + 4u);
			mem_write32(palette_address, mem_read32(colour_source));
			mem_write32(palette_address + 4u, colours_2_3);
			palette_address += 8u;
			colours_6_7 = mem_read32(colour_source + 0xCu);
			mem_write32(palette_address, mem_read32(colour_source + 8u));
			mem_write32(palette_address + 4u, colours_6_7);
			palette_address += 8u;
			colours_10_11 = mem_read32(colour_source + 0x14u);
			mem_write32(palette_address, mem_read32(colour_source + 0x10u));
			mem_write32(palette_address + 4u, colours_10_11);
			palette_address += 8u;
			colours_14_15 = mem_read32(colour_source + 0x1Cu);
			mem_write32(palette_address, mem_read32(colour_source + 0x18u));
			mem_write32(palette_address + 4u, colours_14_15);
			blocks_left--;
			palette_address += 8u;
		} while (blocks_left != 0u);
		entries_left--;
	} while (entries_left != 0u);
}

/* Steps the screen's fade while the screen's flags bit 0 is set (vblank_update_mixer, every other
   vertical blank): a fade asked for by fade_kind (0 to black, 1 from black, 2 to white, 3 from white, 4
   to a red tint, 5 back from it; records at 01173A) sets fade_level, fade_step, fade_end and
   fade_channels and fade_kind 6 (under way); then each time the level moves a step (6 bits: 20 is -32,
   black; 1F is +31, white) and goes into the mixer's colour offset 0 in mixer_values (registers 40-44:
   red, green, blue, those fade_channels has; register 3E's high byte C0), which vblank_update_mixer
   copies to the mixer; flags bit 0 is cleared when the level reaches fade_end. */
void step_screen_fade(void)
{
	uint32_t carry = 0;
	uint8_t channels = 0;
	uint16_t level = 0;
	uint32_t channels_left = 0, fade_start = 0, mixer_register = 0;

	switch (SCREEN(screen)->fade_kind)
	{
	case 0:
		fade_start = screen_fade_starts_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 1:
		fade_start = screen_fade_from_black_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 2:
		fade_start = screen_fade_to_white_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 3:
		fade_start = screen_fade_from_white_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 4:
		fade_start = screen_fade_to_red_tint_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 5:
		fade_start = screen_fade_from_red_tint_ADDRESS;
		SCREEN(screen)->fade_level = SCREEN_FADE_START(fade_start)->level;
		fade_start++;
		SCREEN(screen)->fade_step = SCREEN_FADE_START(fade_start - 0x1u)->step;
		fade_start++;
		SCREEN(screen)->fade_end = SCREEN_FADE_START(fade_start - 0x2u)->end;
		SCREEN(screen)->fade_channels = SCREEN_FADE_START(fade_start - 0x2u)->channels;
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 6:
		break;
	default:
		invalid_jump(screen_fade_kind_jumps[SCREEN(screen)->fade_kind]);
	}
	level = (uint8_t)(SCREEN(screen)->fade_level + SCREEN(screen)->fade_step) & 0x3Fu;
	SCREEN(screen)->fade_level = (uint8_t)level;
	mixer_register = screen + 0x19Fu;
	SCREEN(mixer_register - 0x19Fu)->colour_offset_mode = 0xC0u;
	channels = SCREEN(screen)->fade_channels;
	channels_left = 3u;
	mixer_register++;
	do
	{
		carry = (uint8_t)(channels * 2u) < channels;
		channels = (uint8_t)(channels * 2u);
		if (carry)
		{
			mem_write16(mixer_register, level);
			mixer_register += 2u;
		}
		channels_left--;
	} while (channels_left != 0u);
	if (level == SCREEN(screen)->fade_end)
		SCREEN(screen)->flags &= ~1u;
}

/* Copies the text layer's characters (3000 bytes, 384 characters of 8x8 pixels) from 104020 into video
   RAM at 318000. */
void load_text_font(void)
{
	move_string(text_font_ADDRESS, 0x1800u, VIDEO_RAM + 0x18000u, 0x1800u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: builds the text layer's first 128 characters (318000, where load_text_font puts its 384) from
   the one-bit font at 103C20 (8 bytes a character, a bit a pixel): the character's pixels in pen 1, and
   a shadow in pen 2 one pixel right and one row down under them. */
void load_shadowed_text_font(void)
{
	uint32_t carry = 0;
	uint8_t row_bits = 0;
	uint32_t characters_left = 0, destination = 0, font_rows = 0, pen = 0, pixel_mask = 0, pixel_pairs_left = 0, row_pixels = 0, rows_left = 0, shadow = 0, shadow_pixels = 0;

	font_rows = one_bit_font_ADDRESS;
	destination = VIDEO_RAM + 0x18000u;
	pen = 1u;
	characters_left = 0x80u;
	do
	{
		rows_left = 8u;
		shadow = 0u;
		do
		{
			row_bits = TEXT_FONT_ROWS(font_rows)[0];
			pixel_pairs_left = 4u;
			row_pixels = 0u;
			font_rows++;
			do
			{
				carry = (uint8_t)(row_bits * 2u) < row_bits;
				row_bits = (uint8_t)(row_bits * 2u);
				row_pixels = rotate_with_carry_by(row_pixels, 4u, carry, 32);
				carry = (uint8_t)(row_bits * 2u) < row_bits;
				row_bits = (uint8_t)(row_bits * 2u);
				row_pixels = rotate_with_carry_by(row_pixels, 4u, carry, 32);
				pixel_pairs_left--;
			} while (pixel_pairs_left != 0u);
			row_pixels = (row_pixels << 29) | (row_pixels >> 3);
			shadow_pixels = row_pixels;
			pixel_mask = row_pixels * 0xFu;
			row_pixels = row_pixels * pen;
			mem_write32(destination, byte_swap32((shadow & ~pixel_mask) | row_pixels));
			shadow = (row_pixels + shadow_pixels) >> 4;
			destination += 4u;
			rows_left--;
		} while (rows_left != 0u);
		characters_left--;
	} while (characters_left != 0u);
}

/* Writes the strings of the list R19 on the text layer (print_text_layer each) until an FFFF halfword.
   Not run in the scenarios. */
void print_text_layer_list(uint32_t text_record)
{
	do
	{
		text_record = print_text_layer(text_record);
	} while (mem_read16(text_record) != 0xFFFFu);
}

/* Writes a string on the text layer: from the record R19, the cell's address (310000 + its first
   halfword), an attribute byte (the cells' high byte: palette) and the characters, closed by 0; returns
   the address after it. */
uint32_t print_text_layer(uint32_t text_record)
{
	uint16_t cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	text_record += 2u;
	cell = (uint16_t)((uint32_t)mem_read8(text_record) << 8);
	text_record++;
	cell |= mem_read8(text_record);
	text_record++;
	if ((uint8_t)cell != 0u)
	{
		do
		{
			mem_write16(cell_place, cell);
			cell = (uint16_t)((cell & ~0xFFu) | mem_read8(text_record));
			cell_place += 2u;
			text_record++;
		} while ((uint8_t)cell != 0u);
	}
	return text_record;
}

/* Unused: erases a string from the text layer: from the cell of the print_text_layer record R19, writes
   0 cells until the next cell's character is 0. */
void erase_text_layer_to_blank_character(uint32_t text_record)
{
	uint16_t blank_cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	blank_cell = 0u;
	do
	{
		mem_write16(cell_place, blank_cell);
		cell_place += 2u;
	} while (mem_read8(cell_place) != 0u);
}

/* Unused: print_text_layer with the characters as halfwords: from the record R19, the cell's address
   (310000 + its first halfword), the attribute byte, then the characters, 16 bits each (ORed with the
   attribute << 8, so they can reach past 255: the font has 384), closed by a 0 halfword. */
void print_text_layer_halfwords(uint32_t text_record)
{
	uint16_t attribute = 0, character = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	text_record += 2u;
	attribute = (uint16_t)((uint32_t)mem_read8(text_record) << 8);
	text_record++;
	character = mem_read16(text_record);
	text_record += 2u;
	if (character != 0u)
	{
		do
		{
			mem_write16(cell_place, character | attribute);
			character = mem_read16(text_record);
			cell_place += 2u;
			text_record += 2u;
		} while (character != 0u);
	}
}

/* Unused: erases a string from the text layer as erase_text_layer_to_blank_character does, but until the
   next cell is all 0 (character and attribute). */
void erase_text_layer_to_blank_cell(uint32_t text_record)
{
	uint16_t blank_cell = 0;
	uint32_t cell_place = 0;

	cell_place = 0x310000u | mem_read16(text_record);
	blank_cell = 0u;
	do
	{
		mem_write16(cell_place, blank_cell);
		cell_place += 2u;
	} while (mem_read16(cell_place) != 0u);
}

/* Never runs in the scenarios (ISDN only): writes a number's low byte as two hex digits on the text
   layer, ending before the cell it is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_byte(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number = (uint8_t)number;
	digit_count = 2u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a number's low halfword as four hex digits on the text layer, ending before the cell it
   is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_halfword(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number = (uint16_t)number;
	digit_count = 4u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a number's low 24 bits as six hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits). */
void print_hex_24_bits(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	number &= 0xFFFFFFu;
	digit_count = 6u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Unused: writes a 32-bit number as eight hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits, which it runs on into). */
void print_hex_word(uint32_t number, uint16_t cell, uint32_t cell_place)
{
	uint32_t digit_count = 0;

	digit_count = 8u;
	print_hex_digits(number, cell, digit_count, cell_place);
}

/* Writes a number in hex on the text layer, in as many cells as it is told, ending before the cell it is
   given, the lowest digit last: each digit a character ('0'-'9', 'A'-'F') with the attribute byte it is
   given; when the number runs out, the cells left before it are cleared (0). Only print_hex_byte runs
   it; the entries for four, six and eight digits before it (print_hex_halfword, print_hex_24_bits,
   print_hex_word) are unused. */
void print_hex_digits(uint32_t number, uint16_t cell, uint32_t digit_count, uint32_t cell_place)
{
	uint32_t higher_digits = 0;

	cell_place += digit_count * 2u;
	do
	{
		cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)number);
		cell = (uint16_t)((cell & ~0xFFu) | (cell & 0xFu));
		if ((uint8_t)cell >= 0xAu)
			cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)(cell + 7u));
		cell = (uint16_t)((cell & ~0xFFu) | (uint8_t)(cell + 0x30u));
		cell_place -= 2u;
		mem_write16(cell_place, cell);
		higher_digits = number >> 4;
		number = higher_digits;
		digit_count--;
	} while ((digit_count != 0u) & (higher_digits != 0u));
	if (digit_count != 0u)
	{
		do
		{
			cell_place -= 2u;
			mem_write16(cell_place, number);
			digit_count--;
		} while (digit_count != 0u);
	}
}

/* Unused: converts R0's eight decimal digits (four bits each, the highest first) into a number, returned
   in R0: eight times, the result so far times 10 plus the next digit. The inverse of binary_to_bcd. */
uint32_t bcd_to_binary(uint32_t value)
{
	uint32_t digits_left = 0, number = 0, total = 0;

	digits_left = 8u;
	total = 0u;
	do
	{
		value = (value << 4) | (value >> 28);
		total = total * 0xAu + (0xFu & value);
		digits_left--;
	} while (digits_left != 0u);
	number = total;
	return number;
}

/* Converts R0 into eight decimal digits, one per four bits, the units in the lowest. */
uint32_t binary_to_bcd(uint32_t number)
{
	uint32_t bcd = 0, digit = 0, digits = 0, digits_left = 0;

	digits_left = 8u;
	digits = 0u;
	do
	{
		digit = 0u;
		divux_pair(0xAu, &number, &digit);
		digits += digit;
		digits = (digits << 28) | (digits >> 4);
		digits_left--;
	} while ((digits_left != 0u) & (number != 0u));
	if (number == 0u && digits_left != 0u)
	{
		do
		{
			digits = (digits << 28) | (digits >> 4);
			digits_left--;
		} while (digits_left != 0u);
	}
	bcd = digits;
	return bcd;
}

/* Draws the string at R18 as draw_text does (a sprite per character into the screen's sprite list), at
   the place R3 holds (x in its low half, y in its high half, as a text record's first 32 bits) with the
   attribute R5. Used by stage_result_line, stage_indicator_draw and song_title_show (the song title). */
void draw_text_at(uint32_t place, uint16_t attribute, uint32_t text)
{
	uint16_t font = 0;
	uint32_t sprite = 0, sprite_copy = 0, sprite_size = 0, sprite_words_0_1 = 0;

	sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite)
		return;
	sprite_copy = sprite + 0x3000u;
	sprite_words_0_1 = text_sprite_words_0_1;
	place = (place << 16) | (place >> 16);
	sprite_size = text_sprite_size | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	font = text_sprite_font;
	attribute |= text_sprite_palette;
	text = draw_text_sprites(sprite_words_0_1, sprite_size, place, font, attribute, sprite, sprite_copy, text);
}

/* Draws the text record R19 (x and y, an attribute, a string closed by 0) as one sprite per character
   and returns the address of the record after it. */
uint32_t draw_text(uint32_t text_record)
{
	uint16_t attribute = 0, font = 0;
	uint32_t place = 0, sprite = 0, sprite_copy = 0, sprite_size = 0, sprite_words_0_1 = 0, text = 0;

	text = text_record + 6u;
	sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= sprite)
		return text;
	sprite_copy = sprite + 0x3000u;
	sprite_words_0_1 = text_sprite_words_0_1;
	sprite_size = text_sprite_size | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	place = mem_read32(text_record);
	place = (place << 16) | (place >> 16);
	font = text_sprite_font;
	attribute = text_sprite_palette | mem_read16(text_record + 4u);
	return draw_text_sprites(sprite_words_0_1, sprite_size, place, font, attribute, sprite, sprite_copy, text);
}

/* Writes a sprite per character of the string at R18 (closed by 0) into the sprite list and its halfway
   copy alike: words 0-3 from R1 and R2 (an 8x8 piece, with the screen's bit), the place R3 (y, x; x 8
   more each character), the graphics R4 + 8 * the character, the attribute R5; then links them into
   depth layer 19E (screen 5678, drawn after nearly all the others) and counts them (sprite_groups).
   Returns the address after the string. The end of draw_text and draw_text_at. */
uint32_t draw_text_sprites(uint32_t sprite_words_0_1, uint32_t sprite_size, uint32_t place, uint16_t font, uint16_t attribute, uint32_t sprite, uint32_t sprite_copy, uint32_t text)
{
	uint16_t character = 0;
	uint32_t depth_layer = 0, first_sprite = 0, text_depth_list_head = 0, x_step = 0;
	uint8_t next_character = 0;

	character = mem_read8(text);
	x_step = 0x80000u;
	text++;
	do
	{
		character = (uint16_t)(((uint32_t)character << 3) + font);
		mem_write32(sprite, sprite_words_0_1);
		mem_write32(sprite_copy, sprite_words_0_1);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write32(sprite, sprite_size);
		mem_write32(sprite_copy, sprite_size);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write32(sprite, place);
		mem_write32(sprite_copy, place);
		sprite_copy += 4u;
		sprite += 4u;
		mem_write16(sprite, character);
		mem_write16(sprite_copy, character);
		sprite_copy += 2u;
		sprite += 2u;
		mem_write16(sprite, attribute);
		mem_write16(sprite_copy, attribute);
		sprite_copy += 2u;
		place += x_step;
		next_character = mem_read8(text);
		text++;
		character = next_character;
		sprite += 2u;
	} while (character != 0u);
	depth_layer = screen + 0x5678u;
	text_depth_list_head = SCREEN(depth_layer - 0x5678u)->text_depth_list_head;
	if (text_depth_list_head == 0u)
		SCREEN(depth_layer - 0x5678u)->text_depth_list_tail = sprite;
	first_sprite = SCREEN(screen)->sprite_list_last + 0x10u;
	mem_write32(sprite, text_depth_list_head);
	mem_write32(sprite_copy, (uint16_t)(text_depth_list_head + 0x300u));
	SCREEN(screen)->sprite_list_last = sprite;
	SCREEN(depth_layer - 0x5678u)->text_depth_list_head = (first_sprite >> 4) | 0x8000u;
	SCREEN(screen)->sprite_groups++;
	return text;
}

/* Ends all of the screen's objects at once: every slot free (screen - 7FF0), both object lists empty
   (this frame's at - 7E00, the next one's at - 7D00, with run_objects' pointers to them at - 8000, -
   7FFC, - 7FF4 and - 7FF8), and the queue of palette uploads (palette_upload_count) emptied. At boot and
   as each game mode starts. */
void clear_objects(void)
{
	uint32_t halfword_count = 0;

	move_halfwords(screen - 0x7FF0u, zero_block_ADDRESS, 0x7Cu);
	OBJECT_AREA(screen - 0x8000u)->list = screen - 0x7E00u;
	OBJECT_AREA(screen - 0x8000u)->list_first = screen - 0x7DFEu;
	OBJECT_AREA(screen - 0x8000u)->list_end = screen - 0x7DFEu;
	OBJECT_AREA(screen - 0x8000u)->next_list = screen - 0x7D00u;
	halfword_count = (uint8_t)OBJECT_LIST(screen - 0x7E00u)->count;
	halfword_count = (uint16_t)(halfword_count + 1u);
	move_halfwords(screen - 0x7E00u, zero_block_ADDRESS, halfword_count);
	halfword_count = (uint8_t)OBJECT_LIST(screen - 0x7D00u)->count;
	halfword_count = (uint16_t)(halfword_count + 1u);
	move_string(zero_block_ADDRESS, halfword_count, screen - 0x7D00u, halfword_count, 2u, 0u, 0u, 0u, &r27, &r28);
	SCREEN(screen)->palette_upload_count &= 0xFFFF0000u;
}

/* The object loop, once a frame for the screen: each live object (flags bit 0) of this frame's list
   (screen - 8000) has its handler run and goes on the next frame's list (screen - 7FF8); a dead one
   frees its slot (its byte at screen - 7FF0, by its slot number at 8). Then this frame's list is cleared
   and the two lists swap. */
void run_objects(void)
{
	uint32_t blocks_left = 0, count = 0, list_entry = 0, object_list = 0, zero_high = 0, zero_low = 0;
	uint32_t next_list = 0;

	current_object = screen;
	list_entry = OBJECT_AREA(screen - 0x8000u)->list + 2u;
	OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->next_list)->count = 0u;
	for (;;)
	{
		current_object = (current_object & ~0xFFFFu) | mem_read16(list_entry);
		list_entry += 2u;
		if ((uint16_t)current_object == 0u)
			break;
		if ((OBJECT(current_object)->flags & 1u) != 0u)
		{
			call_code(OBJECT(current_object)->handler);
			next_list = OBJECT_AREA(screen - 0x8000u)->next_list;
			mem_write16(next_list, mem_read16(next_list) + 1u);
			mem_write16(OBJECT_AREA(screen - 0x8000u)->next_list + OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->next_list)->count * 2u, current_object);
			continue;
		}
		mem_write8(screen - 0x7FF0u + OBJECT(current_object)->slot, 0u);
	}
	object_list = OBJECT_AREA(screen - 0x8000u)->list;
	blocks_left = OBJECT_LIST(object_list)->count;
	blocks_left = (uint16_t)(blocks_left + 4u);
	blocks_left = blocks_left >> 2;
	zero_low = 0u;
	zero_high = 0u;
	do
	{
		mem_write32(object_list, zero_low);
		mem_write32(object_list + 4u, zero_high);
		blocks_left--;
		object_list += 8u;
	} while (blocks_left != 0u);
	OBJECT_AREA(screen - 0x8000u)->list ^= 0x100u;
	OBJECT_AREA(screen - 0x8000u)->next_list ^= 0x100u;
	count = OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->list)->count;
	OBJECT_AREA(screen - 0x8000u)->list_first = OBJECT_AREA(screen - 0x8000u)->list + 2u;
	OBJECT_AREA(screen - 0x8000u)->list_end = OBJECT_AREA(screen - 0x8000u)->list_first + count * 2u;
}

/* Takes the first free object slot of the screen (FF into its byte at screen - 7FF0), clears its 100
   bytes, makes it live (flags bit 0) with its slot number at 8, puts it on this frame's list and returns
   it; with no slot free, it returns the spare object at screen - 7F00 without any of that. */
uint32_t start_object(void)
{
	uint32_t zero = 0;
	uint8_t slot = 0;
	uint32_t object = 0, object_bytes = 0;
	uint32_t list = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
	{
		object = screen - 0x7F00u;
		return object;
	}
	mem_write8(r28, 0xFFu);
	slot = (uint8_t)r27;
	r27 = r27 << 8;
	object = screen - 0x7C00u + r27;
	r27 = 0u;
	r28 = 0u;
	object_bytes = object;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	object_bytes += 8u;
	mem_write32(object_bytes, 0u);
	mem_write32(object_bytes + 4u, 0u);
	mem_write8(object, mem_read8(object) | 1u);
	mem_write8(object + 8u, slot);
	list = OBJECT_AREA(screen - 0x8000u)->list;
	mem_write16(list, mem_read16(list) + 1u);
	mem_write16(OBJECT_AREA(screen - 0x8000u)->list + OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->list)->count * 2u, object);
	return object;
}

/* Unused: takes the first free object slot as start_object does (FF into its byte of the 7C at screen -
   7FF0) and clears 100 bytes for it, but at the slot byte's address with its low half shifted 8 left
   (201000 + 100 * slot for screen 1), not where start_object puts slot n (screen - 7C00 + 100 * n), and
   without making it live, its slot number or the frame's list; with no slot free, the spare object at
   screen - 7F00. Returns the object. */
uint32_t reserve_object_slot(void)
{
	uint32_t zero = 0;
	uint32_t object = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
	{
		object = screen - 0x7F00u;
		return object;
	}
	mem_write8(r28, 0xFFu);
	object = (r28 & ~0xFFFFu) | (uint16_t)(r28 << 8);
	move_string(zero_block_ADDRESS, 0x80u, object, 0x80u, 2u, 0u, 0u, 0u, &r27, &r28);
	return object;
}

/* Unused: frees the slot of the object R21: clears the slot byte (screen - 7FF0) numbered by bits 8-15
   of the object's address, which is not the object's own slot: for start_object's slot n it clears slot
   n + 4, for reserve_object_slot's slot n slot n + 10. */
void free_object_slot(uint16_t object)
{
	mem_write8(screen - 0x7FF0u + ((uint16_t)object >> 8), 0u);
}

/* Starts an object for each handler of the list R10 (a 32-bit count, then the handlers: the
   object_handlers_* tables), each with its place in the list at 9, and returns the last one. Used by the
   game modes (game_mode_car_presentation, game_mode_ranking, game_mode_play, initialise_board...),
   name_entry and ranking. */
uint32_t start_object_list(uint32_t handler_list)
{
	uint8_t list_index = 0;
	uint32_t object = 0, objects_left = 0;

	objects_left = mem_read32(handler_list);
	list_index = 0u;
	handler_list += 4u;
	do
	{
		object = start_object();
		OBJECT(object)->handler = mem_read32(handler_list);
		OBJECT(object)->unk_09 = list_index;
		list_index++;
		objects_left--;
		handler_list += 4u;
	} while (objects_left != 0u);
	return object;
}

/* Returns the next pseudo-random number of the screen's sequence (kept in random_state). */
uint32_t random_number(void)
{
	uint32_t next_state = 0, number = 0, old_state = 0, state = 0;

	state = SCREEN(screen)->random_state;
	if ((uint16_t)state == 0u)
		state ^= 0x2A6D365Bu;
	old_state = state;
	state = (state * 2u * 2u + old_state) * 2u * 2u * 2u + old_state;
	number = (old_state & ~0xFFFFu) | (uint16_t)state;
	state = (state << 16) | (state >> 16);
	number = (number & ~0xFFFFu) | (uint16_t)(number + state);
	next_state = (state & ~0xFFFFu) | (uint16_t)number;
	SCREEN(screen)->random_state = (next_state << 16) | (next_state >> 16);
	return number;
}

/* Starts the sound at boot: the screen's ring of sound requests with one request of sound 0 waiting (800
   the place read, 801 the place written, the requests from 802), I/O chip 0's CNT 6 (its screen on, the
   sound CPU out of reset), screen byte 822 cleared, and 701FFF cleared, which lets the sound program's
   main loop run. */
void start_sound_program(void)
{
	AS32(SCREEN(screen)->sound_queue_read) = 0x100u;
	mem_write8(IO_C0001C, 6u);
	SCREEN(screen)->sound_start_cleared = 0u;
	mem_write8(SOUND_701FFF, 0u);
}

/* Requests sound R0 for the screen (queue_sound), except while the screen's demo_flags bit 8 is set,
   when only the sounds the table at 01202E (demo_sounds) allows play, and only with advertise_sound set. */
void request_sound(uint8_t sound)
{
	uint32_t sound_number = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		queue_sound(sound);
		return;
	}
	if (advertise_sound != 0u)
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
	uint32_t request_ring = 0, sound_queue_write = 0;

	request_ring = screen + 0x801u;
	sound_queue_write = SCREEN(request_ring - 0x801u)->sound_queue_write;
	request_ring++;
	mem_write8(request_ring + sound_queue_write, sound);
	SCREEN(request_ring - 0x802u)->sound_queue_write = (sound_queue_write + 1u) & 0x1Fu;
}

/* Unused: puts sound R0 in the screen's second ring of 32 sound requests (second_sound_queue_write, the
   requests from 842; send_sound_requests sends them to the command byte 8 after the first ring's), as
   request_sound does for the first: while demo_flags bit 8 is set only the sounds the table at 01204E
   (demo_second_sounds) allows, and only with advertise_sound set. */
void request_second_sound(uint8_t sound)
{
	uint32_t request_ring = 0, second_sound_queue_write = 0, sound_number = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		request_ring = screen + 0x841u;
		second_sound_queue_write = SCREEN(request_ring - 0x841u)->second_sound_queue_write;
		request_ring++;
		mem_write8(request_ring + second_sound_queue_write, sound);
		SCREEN(request_ring - 0x842u)->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
	}
	else if (advertise_sound != 0u)
	{
		sound_number = (uint8_t)sound;
		if ((demo_second_sounds[sound_number >> 5] & shl32(1u, (int)(sound_number & 0x1Fu))) != 0u)
		{
			request_ring = screen + 0x841u;
			second_sound_queue_write = SCREEN(request_ring - 0x841u)->second_sound_queue_write;
			request_ring++;
			mem_write8(request_ring + second_sound_queue_write, sound);
			SCREEN(request_ring - 0x842u)->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
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

	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u && advertise_sound == 0u)
	{
		sound_page = (uint32_t)SCREEN(screen)->index << 8;
		mem_write8(sound_page + SOUND_ENGINE_REVS_1, 0u);
		mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, mem_read8(sound_page + SOUND_ENGINE_REQUESTS_1) | 0x20u);
		return;
	}
	sound_page = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(sound_page + SOUND_ENGINE_SAMPLE_1, car_model_engine_samples[CAR(player_car)->model]);
	loudness = 0x10u;
	if (CAR(player_car)->transmission != 0u)
		loudness = 0x30u;
	mem_write8(sound_page + SOUND_ENGINE_LOUDNESS_1, loudness);
	mem_write8(sound_page + SOUND_ENGINE_REVS_1, 1u);
	mem_write8(sound_page + SOUND_ENGINE_REQUESTS_1, 0xA0u);
}
