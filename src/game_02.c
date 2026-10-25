/**
 *    Name:        game_02.c
 *    Description: the game's functions, part 02 (in the order of their V60 addresses)
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The entries to a race, each frame. Player 0's screen keeps them in its record: once every entry is
   taken (it means to wait for each record's entry_serial_seen, but compares player 0's own each time), a
   player's entry_request not yet accepted (accepted_players) is accepted: entrant = that player,
   entry_serial + 1, and the first one starts entry_frames at 600 (rom_00E614), cut to 40 once every
   player is in; entry_frames counting down to 0 closes the entries (entrant FF, entry_serial + 1,
   accepted_players cleared, 1 to the link board's 80000A). Every screen: when some player's race is over
   (race unk_28) an idle screen (game steps 1-5) goes to step 5 (unk_20E02E cleared), and while player
   0's entry_frames runs to step 7 (entry_invitation_step); each new entry_serial of player 0's record
   (entry_serial_seen) adds its entrant to the screen's entrants (entrant_count), or the close clears
   them (and entry_accepted); the screen's own player accepted: its entry_request cleared,
   entry_accepted, the first entrant into host_player and is_host when that is itself; the host keeps
   racer_count and race_order in its record from the entrants. */
void race_entries_update(void)
{
	uint32_t zero = 0;
	uint8_t r0_2 = 0, entrants = 0, entry_serial = 0;
	uint16_t r2 = 0, r9 = 0;
	uint32_t r0 = 0, r1 = 0, r3 = 0, r13 = 0, race = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;
	uint32_t t3 = 0;

	race = SCREEN(screen)->race;
	r12 = 0x20FC00u;
	if (SCREEN(screen)->player == 0u)
	{
		r1 = player_count;
		r2 = 0x80u;
		entry_serial = RACE(race)->entry_serial;
		r13 = r12 + 0x1Cu;
		r14 = screen + 0x300u;
		r0 = RACE(race)->entry_frames;
		do
		{
			r1--;
		} while ((r1 != 0u) & (unk_20FC1C == entry_serial));
		if (unk_20FC1C == entry_serial)
		{
			if (r0 == 0u)
			{
				r1 = 0u;
				if (stand_alone == 0u)
					r1 = machine_count;
				r9 = rom_00E614[r1];
				r13 = r12 + 0x19u;
				r1 = player_count;
				r3 = 0u;
				for (;;)
				{
					if (mem_read8(r14 + r3) != 0u || mem_read8(r13) == 0u)
					{
						r3 = (uint8_t)(r3 + 1u);
						r13 = (r13 & ~0xFFFFu) | (uint16_t)(r13 + r2);
						r1--;
						if (r1 == 0u)
							break;
						continue;
					}
					mem_write8(r14 + r3, 0xFFu);
					RACE(race)->entrant = r3;
					RACE(race)->entry_serial++;
					r0 = r9;
					if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != player_count || r0 <= 0x28u)
						break;
					r0 = 0x28u;
					break;
				}
			}
			else
			{
				t1 = r0;
				r0 = (uint16_t)(t1 - 1u);
				r9 = (uint16_t)r0;
				if (t1 != 1u)
				{
					r13 = r12 + 0x19u;
					r1 = player_count;
					r3 = 0u;
					for (;;)
					{
						if (mem_read8(r14 + r3) != 0u || mem_read8(r13) == 0u)
						{
							r3 = (uint8_t)(r3 + 1u);
							r13 = (r13 & ~0xFFFFu) | (uint16_t)(r13 + r2);
							r1--;
							if (r1 == 0u)
								break;
							continue;
						}
						mem_write8(r14 + r3, 0xFFu);
						RACE(race)->entrant = r3;
						RACE(race)->entry_serial++;
						r0 = r9;
						if ((uint8_t)(SCREEN(screen)->entrant_count + 1u) != player_count || r0 <= 0x28u)
							break;
						r0 = 0x28u;
						break;
					}
				}
				else
				{
					RACE(race)->entry_serial++;
					RACE(race)->entrant = 0xFFu;
					AS32(SCREEN(r14 - 0x300u)->accepted_players) = 0u;
					r14 = r14 + 4u;
					SCREEN(r14 - 0x304u)->unk_304 = 0u;
					mem_write8(0x80000Au, 1u);
					r14 = r14 + 4u;
				}
			}
		}
		RACE(race)->entry_frames = r0;
	}
	r1 = player_count;
	r13 = r12 + 0x28u;
	do
	{
		t2 = mem_read8(r13);
		zero = t2 == 0u;
		r13 += 0x80u;
		r1--;
	} while ((r1 != 0u) & (t2 == 0u));
	if (zero)
	{
		if (unk_20FC1A != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	else if (SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
	{
		AS32(SCREEN(screen)->game_mode) = 5u;
		unk_20E02E = 0u;
		if (unk_20FC1A != 0u && SCREEN(screen)->game_mode != 0u && SCREEN(screen)->game_mode <= 5u)
			AS32(SCREEN(screen)->game_mode) = 7u;
	}
	if (unk_20FC1D == RACE(race)->entry_serial_seen)
		return;
	RACE(race)->entry_serial_seen = unk_20FC1D;
	r0_2 = unk_20FC1E;
	if ((int8_t)r0_2 < 0)
	{
		SCREEN(screen)->entrant_count = 0u;
		SCREEN(screen)->entry_accepted = 0u;
		return;
	}
	r13 = screen + 0x30Fu;
	r1 = SCREEN(r13 - 0x30Fu)->entrant_count;
	r1 = (uint8_t)(r1 + 1u);
	SCREEN(r13 - 0x30Fu)->entrant_count = r1;
	mem_write8(r13 + r1, r0_2);
	if (SCREEN(screen)->player == r0_2)
	{
		RACE(race)->entry_request = 0u;
		SCREEN(screen)->entry_accepted = 0xFFu;
		entrants = SCREEN(r13 - 0x30Fu)->entrants;
		SCREEN(screen)->host_player = entrants;
		if (SCREEN(screen)->player == entrants)
			SCREEN(screen)->is_host = 0xFFu;
	}
	if (SCREEN(screen)->entry_accepted != 0u && SCREEN(screen)->is_host != 0u)
	{
		r14 = race + 0x1Fu;
		RACE(r14 - 0x1Fu)->racer_count = SCREEN(r13 - 0x30Fu)->entrant_count;
		r14 = r14 + 1u;
		r13++;
		t3 = SCREEN(r13 - 0x310u)->unk_314;
		AS32(RACE(r14 - 0x20u)->race_order) = AS32(SCREEN(r13 - 0x310u)->entrants);
		RACE(r14 - 0x20u)->unk_24 = t3;
		r14 = r14 + 8u;
	}
}

/* The race's progress, each frame of a race (screen unk_31A). The host (is_host) runs it in its record:
   once every racer has taken the last pass (pass_seen = pass_count), a racer whose stage_count changed
   (against counted_stages) has passed a checkpoint (stage_count - 1): pass_player, pass_checkpoint,
   pass_order (counted_passes), pass_branch (the side of the road it was on: race unk_44 bit 15),
   pass_time (race_time in hundredths plus the order), pass_count + 1; the first to pass a checkpoint
   before the goal adds the time of the stage its branch leads to (rom_00E5B3 by rom_00E5D3[stage * 2 +
   branch]) plus the setting for that checkpoint (unk_20FC0B) to time_left. Once the race has started
   (race_started) race_time + 34 and time_left - 1 a frame, and one pass of sorting race_order: the
   finished (race unk_5E) first by finish_time, then by distance (race unk_40-43). Every screen from the
   host's record (host_record): the leader's segment and stage (leader_window_segment, leader_stage),
   each player's place (places, its own in place); each new pass: pass_seen (screen and race),
   checkpoints_passed, checkpoint_passes, the order and the time by player and by order
   (checkpoint_orders, checkpoint_order, checkpoint_times_by_player, checkpoint_times); the first car
   past a fork decides the route: forks_decided + 1, its branch in route_branches and the stage it leads
   to in route_stages. */
void race_progress_update(void)
{
	uint8_t r1 = 0, r5 = 0, pass_branch = 0, pass_count = 0, pass_count_2 = 0, player = 0;
	uint16_t r4 = 0, pass_time = 0;
	uint32_t r0 = 0, r2 = 0, r3 = 0, r4_2 = 0, r5_2 = 0, r6 = 0, r13 = 0, r15 = 0, pass_checkpoint = 0, pass_order = 0, pass_player = 0, race = 0, race_2 = 0, race_3 = 0, race_4 = 0, racer_count = 0, racer_count_2 = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0, t3 = 0, t4 = 0, t5 = 0;

	race = SCREEN(screen)->race;
	r12 = SCREEN(screen)->host_record;
	if (SCREEN(screen)->in_game == 0u)
		return;
	if (SCREEN(screen)->is_host != 0u)
	{
		race_3 = r12 + 0x1Fu;
		racer_count_2 = RACE(race_3 - 0x1Fu)->racer_count;
		pass_count_2 = RACE(race)->pass_count;
		race_3++;
		r13 = race_3;
		r0 = racer_count_2;
		do
		{
			r2 = rom_00E0A0[mem_read8(r13)] + unk_20E000_ADDRESS;
			r0--;
			r13++;
		} while ((r0 != 0u) & (mem_read8(r2 + 0x61u) == pass_count_2));
		if (mem_read8(r2 + 0x61u) == pass_count_2)
		{
			r13 = race_3;
			r14 = screen + 0x330u;
			r0 = racer_count_2;
			do
			{
				r2 = mem_read8(r13);
				r15 = rom_00E0A0[r2] + unk_20E000_ADDRESS;
				r0--;
				r13++;
			} while ((r0 != 0u) & (mem_read8(r14 + r2) == mem_read8(r15 + 0x60u)));
			if (mem_read8(r14 + r2) != mem_read8(r15 + 0x60u))
			{
				r3 = mem_read8(r15 + 0x60u);
				mem_write8(r14 + r2, r3);
				RACE(race)->pass_player = r2;
				r3 = (uint8_t)(r3 - 1u);
				RACE(race)->pass_checkpoint = r3;
				r4 = mem_read8(screen + 0x338u + r3);
				t1 = screen + 0x338u + r3;
				mem_write8(t1, mem_read8(t1) + 1u);
				RACE(race)->pass_order = (uint8_t)r4;
				r5 = (uint8_t)(mem_read16(r15 + 0x44u) >> 15);
				RACE(race)->pass_branch = r5;
				RACE(race)->pass_time = RACE(race)->race_time / 0xAu + r4;
				RACE(race)->pass_count++;
				if (r4 == 0u && r3 != 5u)
				{
					r0 = mem_read8(r15 + 0x62u);
					r0 = (uint8_t)(r0 * 2u);
					r0 = (uint8_t)(r0 + r5);
					RACE(race)->time_left += (int8_t)(rom_00E5B3[rom_00E5D3[r0]] + unk_20FC0B[r3]) * 0x1Eu;
				}
			}
		}
		if (SCREEN(screen)->race_started != 0u)
		{
			RACE(race)->race_time += 0x22u;
			if (RACE(race)->time_left != 0u)
				RACE(race)->time_left--;
		}
		race_4 = race + 0x1Fu;
		racer_count = RACE(race_4 - 0x1Fu)->racer_count;
		t2 = racer_count;
		r0 = (uint8_t)(t2 - 1u);
		race_4++;
		if ((int8_t)t2 > 1)
		{
			r4_2 = RACE(race_4 - 0x20u)->race_order;
			r13 = rom_00E0A0[r4_2] + unk_20E000_ADDRESS;
			race_4++;
			if (SCREEN(screen)->race_started != 0u)
			{
				do
				{
					r6 = mem_read8(race_4);
					r14 = rom_00E0A0[r6] + unk_20E000_ADDRESS;
					race_4++;
					if (mem_read8(r13 + 0x5Eu) != 0u)
					{
						if (mem_read8(r14 + 0x5Eu) == 0u || mem_read16(r13 + 0x16u) <= mem_read16(r14 + 0x16u))
						{
							t3 = r6;
							r6 = r4_2;
							r4_2 = t3;
							r13 = r14;
						}
					}
					else if (mem_read32(r14 + 0x40u) <= mem_read32(r13 + 0x40u))
					{
						t3 = r6;
						r6 = r4_2;
						r4_2 = t3;
						r13 = r14;
					}
					mem_write8(race_4 - 2u, r6);
					r0--;
				} while (r0 != 0u);
				mem_write8(race_4 - 1u, r4_2);
			}
		}
	}
	race_2 = r12 + 0x1Fu;
	r0 = RACE(race_2 - 0x1Fu)->racer_count;
	r1 = 0u;
	race_2++;
	r2 = RACE(race_2 - 0x20u)->race_order;
	player = SCREEN(screen)->player;
	r14 = rom_00E0A0[r2] + unk_20E000_ADDRESS;
	SCREEN(screen)->leader_window_segment = mem_read16(r14 + 0x46u);
	SCREEN(screen)->leader_stage = mem_read8(r14 + 0x62u);
	race_2++;
	do
	{
		mem_write8(screen + 0x320u + r2, r1);
		if (r2 == player)
			SCREEN(screen)->place = r1;
		r2 = mem_read8(race_2);
		r1++;
		r0--;
		race_2++;
	} while (r0 != 0u);
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
		r5_2 = pass_checkpoint * 2u;
		t4 = screen + 0x480u + pass_player;
		mem_write8(t4, mem_read8(t4) + 1u);
		t5 = screen + 0x35Au + pass_checkpoint;
		mem_write8(t5, mem_read8(t5) + 1u);
		mem_write8(screen + 0x360u + r5_2 * 4u + pass_player, pass_order);
		mem_write8(screen + 0x390u + r5_2 * 4u + pass_order, pass_player);
		r5_2 = r5_2 * 2u;
		mem_write16(screen + 0x3C0u + r5_2 * 4u + pass_player * 2u, pass_time);
		mem_write16(screen + 0x420u + r5_2 * 4u + pass_order * 2u, pass_time);
		if (pass_order == 0u && pass_checkpoint != 5u)
		{
			SCREEN(screen)->forks_decided++;
			mem_write8(screen + 0x34Au + pass_checkpoint, pass_branch);
			r0 = mem_read8(rom_00E0A0[pass_player] + 0x20E062u);
			r0 = (uint8_t)(r0 * 2u);
			mem_write8(screen + 0x344u + pass_checkpoint, rom_00E5D3[(uint8_t)(r0 + pass_branch)]);
		}
	}
}

/* Game step 7, an idle seat while another one's game takes entries (race_entries_update sends it here),
   by screen unk_80's byte 2: 0 screen unk_153 = 0 and unk_94 bit 0; 1 once that bit has cleared: the
   seat's sound channel (012004), the screen's objects (011C60), layers (01070C), sprite count (010889)
   and text layer (01076A) set afresh, its race record's race part cleared (race_record_clear), not the
   demo (unk_90 bit 8), the wheel motor off (unk_20E0D0), both of the seat's lamps blinking (bytes C0-C3
   and C8-CB of its entry at checkpoint_lamp); 2 starts object_handlers_DF74 (entry_countdown_start,
   credit_display_update, coin_prompt_update): the invitation to join with the entry time left; 3 the
   seat's screen on (its I/O chip's CNT, 6 or 86), unk_153 = 1, unk_94 bit 0; 4 once player 0's
   entry_frames is over: the sound channel again (012004), step 2, the lamps off, the LCD cleared
   (clear_lcd). */
void entry_invitation_step(void)
{
	uint32_t r10 = 0, ignored = 0, index = 0;

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
		r10 = 0xDF74u;
		ignored = start_object_list(r10);
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
		if (unk_20FC1A != 0u)
			return;
		reset_screen_sound();
		AS32(SCREEN(screen)->game_mode) = 2u;
		r10 = unk_20E000_ADDRESS + SCREEN(screen)->index * 4u;
		mem_write32(r10 + 0xC0u, 0u);
		mem_write32(r10 + 0xC8u, 0u);
		clear_lcd();
		return;
	default:
		invalid_jump(rom_00DDB4[SCREEN(screen)->game_step]);
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
		unk_20E0D0[index] = 0u;
		r10 = unk_20E000_ADDRESS + index * 4u;
		mem_write8(r10 + 0xC0u, 2u);
		mem_write8(r10 + 0xC8u, 8u);
		mem_write8(r10 + 0xC1u, 0xFFu);
		mem_write8(r10 + 0xC2u, 0x10u);
		mem_write8(r10 + 0xC3u, 1u);
		mem_write8(r10 + 0xC9u, 0xFFu);
		mem_write8(r10 + 0xCAu, 0x10u);
		mem_write8(r10 + 0xCBu, 1u);
	}
}

/* Clears bytes 30-7F of the screen's race record (the race's part: the car's published state, the
   checkpoint passes, the times), from the zeros at 100800. */
void race_record_clear(void)
{
	move_string(0x100800u, 0x28u, SCREEN(screen)->race + 0x30u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
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
	uint32_t r1 = 0, race = 0;
	uint8_t t1 = 0;

	race = SCREEN(screen)->host_record + 0x1Fu;
	r1 = RACE(race - 0x1Fu)->racer_count;
	race++;
	do
	{
		t1 = mem_read8(0x20FC5Cu + rom_00E0C4[mem_read8(race)]);
		zero = t1 == 0u;
		r1--;
		race++;
	} while ((r1 != 0u) & (t1 != 0u));
	return zero;
}

/* Takes the players' new course records, each frame: when a player's record_serial changes (against
   name_entries_seen), its time (finish_time) on its course (record_course) at or under the course's
   record (course_records, 6 bytes a course: model, the three letters of the name, the time) replaces it
   with its record_model and record_name, also in the two copies in the backup (701F32 with its checksum,
   011418, and 701FAE). */
void course_records_update(void)
{
	uint16_t r1 = 0;
	uint32_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, ignored = 0;
	uint8_t t1 = 0;

	r10 = screen + 0x328u;
	r9 = player_count;
	r11 = unk_20FB80_ADDRESS;
	do
	{
		r11 += 0x80u;
		t1 = mem_read8(r10);
		r10++;
		if (mem_read8(r11 + 0x11u) != t1)
		{
			mem_write8(r10 - 1u, mem_read8(r11 + 0x11u));
			r1 = mem_read16(r11 + 0x16u);
			r8 = mem_read8(r11 + 0x18u);
			r8 = (uint8_t)(r8 * 6u);
			r12 = course_records_ADDRESS + r8;
			if (r1 <= mem_read16(r12 + 4u))
			{
				mem_write16(r12 + 4u, r1);
				mem_write8(r12, mem_read8(r11 + 0x12u));
				mem_write8(r12 + 1u, mem_read8(r11 + 0x13u));
				mem_write16(r12 + 2u, mem_read16(r11 + 0x14u));
				r13 = SOUND_RAM + 0x1F32u + r8;
				mem_write32(r13, mem_read32(r12));
				mem_write16(r13 + 4u, mem_read16(r12 + 4u));
				bookkeeping_update_checksum(ignored);
				r13 = SOUND_RAM + 0x1FAEu + r8;
				mem_write32(r13, mem_read32(r12));
				mem_write16(r13 + 4u, mem_read16(r12 + 4u));
			}
		}
		r9--;
	} while (r9 != 0u);
}

/* Takes the race records of all the players into 20FC00 (80 bytes each, by player), each frame: alone
   (link_off, link_failed) this machine's own two (20FB00, 20FB80); in ISDN mode the eight from 21E000;
   on the link the eight from the link board (link_copy_records_in from 800020, or 800720 when its byte
   80000E is set), or link_lost when its error byte 80000C is set. */
void link_receive_records(void)
{
	bool L_00E854 = false;
	uint32_t r1 = 0, r10 = 0, r11 = 0;
	uint8_t t65 = 0, t66 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0, t33 = 0, t34 = 0, t35 = 0, t36 = 0, t37 = 0, t38 = 0, t39 = 0, t40 = 0, t41 = 0, t42 = 0, t43 = 0, t44 = 0, t45 = 0, t46 = 0, t47 = 0, t48 = 0, t49 = 0, t50 = 0, t51 = 0, t52 = 0, t53 = 0, t54 = 0, t55 = 0, t56 = 0, t57 = 0, t58 = 0, t59 = 0, t60 = 0, t61 = 0, t62 = 0, t63 = 0, t64 = 0;

	for (;;)
	{
		if (L_00E854 || isdn_mode == 0u)
		{
			if (L_00E854 || link_failed != 0u || link_off != 0u)
			{
				if (!L_00E854)
				{
					r10 = unk_20FB00_ADDRESS;
					r11 = 0x20FC00u;
					r1 = 1u;
				}
				do
				{
					t1 = r11;
					r11 += 8u;
					t2 = mem_read32(r10 + 4u);
					mem_write32(t1, mem_read32(r10));
					mem_write32(t1 + 4u, t2);
					r10 += 8u;
					t3 = r11;
					r11 += 8u;
					t4 = mem_read32(r10 + 4u);
					mem_write32(t3, mem_read32(r10));
					mem_write32(t3 + 4u, t4);
					r10 += 8u;
					t5 = r11;
					r11 += 8u;
					t6 = mem_read32(r10 + 4u);
					mem_write32(t5, mem_read32(r10));
					mem_write32(t5 + 4u, t6);
					r10 += 8u;
					t7 = r11;
					r11 += 8u;
					t8 = mem_read32(r10 + 4u);
					mem_write32(t7, mem_read32(r10));
					mem_write32(t7 + 4u, t8);
					r10 += 8u;
					t9 = r11;
					r11 += 8u;
					t10 = mem_read32(r10 + 4u);
					mem_write32(t9, mem_read32(r10));
					mem_write32(t9 + 4u, t10);
					r10 += 8u;
					t11 = r11;
					r11 += 8u;
					t12 = mem_read32(r10 + 4u);
					mem_write32(t11, mem_read32(r10));
					mem_write32(t11 + 4u, t12);
					r10 += 8u;
					t13 = r11;
					r11 += 8u;
					t14 = mem_read32(r10 + 4u);
					mem_write32(t13, mem_read32(r10));
					mem_write32(t13 + 4u, t14);
					r10 += 8u;
					t15 = r11;
					r11 += 8u;
					t16 = mem_read32(r10 + 4u);
					mem_write32(t15, mem_read32(r10));
					mem_write32(t15 + 4u, t16);
					r10 += 8u;
					t17 = r11;
					r11 += 8u;
					t18 = mem_read32(r10 + 4u);
					mem_write32(t17, mem_read32(r10));
					mem_write32(t17 + 4u, t18);
					r10 += 8u;
					t19 = r11;
					r11 += 8u;
					t20 = mem_read32(r10 + 4u);
					mem_write32(t19, mem_read32(r10));
					mem_write32(t19 + 4u, t20);
					r10 += 8u;
					t21 = r11;
					r11 += 8u;
					t22 = mem_read32(r10 + 4u);
					mem_write32(t21, mem_read32(r10));
					mem_write32(t21 + 4u, t22);
					r10 += 8u;
					t23 = r11;
					r11 += 8u;
					t24 = mem_read32(r10 + 4u);
					mem_write32(t23, mem_read32(r10));
					mem_write32(t23 + 4u, t24);
					r10 += 8u;
					t25 = r11;
					r11 += 8u;
					t26 = mem_read32(r10 + 4u);
					mem_write32(t25, mem_read32(r10));
					mem_write32(t25 + 4u, t26);
					r10 += 8u;
					t27 = r11;
					r11 += 8u;
					t28 = mem_read32(r10 + 4u);
					mem_write32(t27, mem_read32(r10));
					mem_write32(t27 + 4u, t28);
					r10 += 8u;
					t29 = r11;
					r11 += 8u;
					t30 = mem_read32(r10 + 4u);
					mem_write32(t29, mem_read32(r10));
					mem_write32(t29 + 4u, t30);
					r10 += 8u;
					t31 = r11;
					r11 += 8u;
					t32 = mem_read32(r10 + 4u);
					mem_write32(t31, mem_read32(r10));
					mem_write32(t31 + 4u, t32);
					r10 += 8u;
					t33 = r11;
					r11 += 8u;
					t34 = mem_read32(r10 + 4u);
					mem_write32(t33, mem_read32(r10));
					mem_write32(t33 + 4u, t34);
					r10 += 8u;
					t35 = r11;
					r11 += 8u;
					t36 = mem_read32(r10 + 4u);
					mem_write32(t35, mem_read32(r10));
					mem_write32(t35 + 4u, t36);
					r10 += 8u;
					t37 = r11;
					r11 += 8u;
					t38 = mem_read32(r10 + 4u);
					mem_write32(t37, mem_read32(r10));
					mem_write32(t37 + 4u, t38);
					r10 += 8u;
					t39 = r11;
					r11 += 8u;
					t40 = mem_read32(r10 + 4u);
					mem_write32(t39, mem_read32(r10));
					mem_write32(t39 + 4u, t40);
					r10 += 8u;
					t41 = r11;
					r11 += 8u;
					t42 = mem_read32(r10 + 4u);
					mem_write32(t41, mem_read32(r10));
					mem_write32(t41 + 4u, t42);
					r10 += 8u;
					t43 = r11;
					r11 += 8u;
					t44 = mem_read32(r10 + 4u);
					mem_write32(t43, mem_read32(r10));
					mem_write32(t43 + 4u, t44);
					r10 += 8u;
					t45 = r11;
					r11 += 8u;
					t46 = mem_read32(r10 + 4u);
					mem_write32(t45, mem_read32(r10));
					mem_write32(t45 + 4u, t46);
					r10 += 8u;
					t47 = r11;
					r11 += 8u;
					t48 = mem_read32(r10 + 4u);
					mem_write32(t47, mem_read32(r10));
					mem_write32(t47 + 4u, t48);
					r10 += 8u;
					t49 = r11;
					r11 += 8u;
					t50 = mem_read32(r10 + 4u);
					mem_write32(t49, mem_read32(r10));
					mem_write32(t49 + 4u, t50);
					r10 += 8u;
					t51 = r11;
					r11 += 8u;
					t52 = mem_read32(r10 + 4u);
					mem_write32(t51, mem_read32(r10));
					mem_write32(t51 + 4u, t52);
					r10 += 8u;
					t53 = r11;
					r11 += 8u;
					t54 = mem_read32(r10 + 4u);
					mem_write32(t53, mem_read32(r10));
					mem_write32(t53 + 4u, t54);
					r10 += 8u;
					t55 = r11;
					r11 += 8u;
					t56 = mem_read32(r10 + 4u);
					mem_write32(t55, mem_read32(r10));
					mem_write32(t55 + 4u, t56);
					r10 += 8u;
					t57 = r11;
					r11 += 8u;
					t58 = mem_read32(r10 + 4u);
					mem_write32(t57, mem_read32(r10));
					mem_write32(t57 + 4u, t58);
					r10 += 8u;
					t59 = r11;
					r11 += 8u;
					t60 = mem_read32(r10 + 4u);
					mem_write32(t59, mem_read32(r10));
					mem_write32(t59 + 4u, t60);
					r10 += 8u;
					t61 = r11;
					r11 += 8u;
					t62 = mem_read32(r10 + 4u);
					mem_write32(t61, mem_read32(r10));
					mem_write32(t61 + 4u, t62);
					r10 += 8u;
					t63 = r11;
					r11 += 8u;
					t64 = mem_read32(r10 + 4u);
					mem_write32(t63, mem_read32(r10));
					mem_write32(t63 + 4u, t64);
					r1--;
					r10 += 8u;
				} while (r1 != 0u);
				return;
			}
			t65 = mem_read8(0x80000Cu);
			if (t65 != 0u)
			{
				link_lost();
				return;
			}
			r10 = 0x800020u;
			r11 = 0x20FC00u;
			r1 = 8u;
			t66 = mem_read8(0x80000Eu);
			if (t66 == 0u)
			{
				link_copy_records_in(r1, r10, r11);
				return;
			}
			r10 += 0x700u;
			link_copy_records_in(r1, r10, r11);
			return;
		}
		r10 = 0x21E000u;
		r11 = 0x20FC00u;
		r1 = 4u;
		L_00E854 = true;
	}
}

/* ISDN mode: copies the eight race records from the link board (800600) to 21E000, where
   link_receive_records takes them; the link interrupt (int_vector_44) does it on the board's command 3. */
void link_isdn_receive_records(void)
{
	uint32_t r1 = 0, r10 = 0, r11 = 0;

	r10 = 0x800600u;
	r11 = 0x21E000u;
	r1 = 8u;
	link_copy_records_in(r1, r10, r11);
}

/* Copies R1 race records from the link board's memory at R10 (a byte in each halfword) to R11: their
   first 70 bytes, 80 apart. */
void link_copy_records_in(uint32_t r1, uint32_t r10, uint32_t r11)
{
	do
	{
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11++;
		r10 += 2u;
		mem_write8(r11, (uint8_t)mem_read16(r10));
		r11 += 0x11u;
		r1--;
		r10 += 2u;
	} while (r1 != 0u);
}

/* Sends this machine's two race records to the link board, each frame (not in ISDN mode, nor alone):
   their first 70 bytes to 800E20 and 800F00 (a byte in each halfword), then FF to its byte 800006;
   link_lost when its error byte 80000C is set. link_wait_frames from unk_20E038 (which nothing sets). */
void link_send_records(void)
{
	uint32_t r1 = 0, r10 = 0, r11 = 0;
	uint8_t t1 = 0;

	if (isdn_mode == 0u)
	{
		link_wait_frames = unk_20E038;
		if (link_failed != 0u || link_off != 0u)
			return;
		t1 = mem_read8(0x80000Cu);
		if (t1 != 0u)
		{
			link_lost();
			return;
		}
		r10 = 0x20FC00u;
		r11 = 0x800FE0u;
		r1 = 2u;
		do
		{
			r10 = r10 - 0x10u - 1u;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r10--;
			r11 -= 2u;
			mem_write16(r11, mem_read8(r10));
			r1--;
		} while (r1 != 0u);
		mem_write8(0x800006u, 0xFFu);
	}
}

/* Draws the object's sprite, unless flags bit 1 is set or the screen's sprite list is full: field 3E
   picks the routine (rom_00EB44): some project the object's road position (1C) with its offsets (20, 24)
   and set fields 30 and 32 to where it lands and 3C to the road's depth layer there, some draw it at the
   screen position in 30 and 32, some copy a ready sprite entry from the address in field 38; then links
   it into the depth list of layer 3C (screen + 5000). */
uint32_t draw_object(uint32_t r0)
{
	uint32_t r2 = 0, r10 = 0, r11 = 0, left_caller = 0;
	uint32_t target1 = 0;

	if ((OBJECT(current_object)->flags & 2u) != 0u)
		return r0;
	r0 = rom_00EB44[OBJECT(current_object)->draw_mode];
	r10 = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= r10)
		return r0;
	left_caller = 0u;
	target1 = r0;
	switch (target1)
	{
	case 0x00EF32u:
		draw_object_on_screen(&r0, &r10);
		break;
	case 0x00F104u:
		r10 = draw_object_entry(r10);
		break;
	case 0x00EB74u:
		draw_object_between_segments(&r0, &r10, &left_caller);
		break;
	case 0x00EEBFu:
		draw_object_at_car_window_segment_on_road(&r0, &r10, &left_caller);
		break;
	case 0x00EE37u:
		draw_object_at_car_window_segment(&r0, &r10, &left_caller);
		break;
	case 0x00EC1Bu:
		draw_object_from_road_edge(&r0, &r10, &left_caller);
		break;
	case 0x00F126u:
		r10 = draw_object_command(r10);
		break;
	case 0x00ECCDu:
		draw_object_from_road_edge_unshifted(&r0, &r10, &left_caller);
		break;
	case 0x00F04Eu:
		draw_object_one_piece(&r0, &r10);
		break;
	case 0x00EE75u:
		draw_object_at_segment(&r0, &r10, &left_caller);
		break;
	case 0x00F15Cu:
		draw_object_smoothed(&r0, &r10);
		break;
	case 0x00ED82u:
		draw_object_from_road_edge_doubled(&r0, &r10, &left_caller);
		break;
	default:
		invalid_jump(target1);
	}
	if (left_caller != 0u)
		return r0;
	r0 = OBJECT(current_object)->depth_layer;
	r11 = screen + 0x5000u + r0 * 4u;
	if (mem_read16(r11) == 0u)
		mem_write16(r11 + 2u, r10);
	r12 = SCREEN(screen)->sprite_list_last + 0x10u;
	r2 = mem_read16(r11);
	mem_write32(r10, r2);
	mem_write32(r10 + 0x3000u, (uint16_t)(r2 + 0x300u));
	SCREEN(screen)->sprite_list_last = r10;
	r12 = r12 >> 4;
	r12 = (r12 & ~0xFFFFu) | (uint16_t)(r12 | 0x8000u);
	mem_write16(r11, r12);
	SCREEN(screen)->sprite_groups++;
	return r0;
}

/* Draw mode 2: the object between the road's segment at its position (field 1C, counted from the view
   drawn: race_copy unk_46) and the next, by the fraction in field 28 (80ths), offset across and up by
   fields 20 and 24 scaled by each segment's scale (draw_object_at_scale). Its size, meant as the two
   segments' scales by the fraction, multiplies them by the second point's offsets instead (R4 and R5
   used twice): a slip in the game's code; nothing in the scenarios draws with it. */
void draw_object_between_segments(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	t1 = RACE(SCREEN(screen)->race_copy)->unk_46;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = screen + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(current_object)->unk_20 * r2) >> 12;
	r1 = (int32_t)((int16_t)OBJECT(current_object)->unk_24 * r2) >> 12;
	r4 = (int32_t)((int16_t)OBJECT(current_object)->unk_20 * r3) >> 12;
	r5 = (int32_t)((int16_t)OBJECT(current_object)->unk_24 * r3) >> 12;
	r7 = (uint16_t)OBJECT(current_object)->unk_28;
	r6 = (uint16_t)(0x80u - r7);
	r4 = ((r4 & ~0xFFFFu) | (uint16_t)(r4 + mem_read16(r12 + 4u))) * r7;
	r0 = (int32_t)(((r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u))) * r6 + r4) >> 7;
	r5 = ((r5 & ~0xFFFFu) | (uint16_t)(r5 + mem_read16(r12 + 6u))) * r7;
	r1 = (int32_t)(((r1 & ~0xFFFFu) | (uint16_t)(r1 + mem_read16(r11 + 6u))) * r6 + r5) >> 7;
	r2 = (r2 * r4 + r3 * r5) >> 7;
	draw_object_at_scale((uint16_t)r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode 5: the object on the road's surface between its segment (field 1C) and the next by the
   fraction in field 28, across at field 20 from the road's edge on side field 3F (the segment's half
   width times the side), its size the two segments' scales by the fraction shifted by scale_shift
   (draw_object_at_shifted_scale). */
void draw_object_from_road_edge(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	r1 = r0;
	t1 = RACE(SCREEN(screen)->race_copy)->unk_46;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = screen + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(current_object)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(current_object)->unk_20 + (uint32_t)mem_read16(screen + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(current_object)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (r2 * r4 + r3 * r5) >> 7;
	draw_object_at_shifted_scale(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode 7: as draw_object_from_road_edge, at the size of scale_shift 0 whatever the object's. */
void draw_object_from_road_edge_unshifted(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	r1 = r0;
	t1 = RACE(SCREEN(screen)->race_copy)->unk_46;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = screen + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(current_object)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(current_object)->unk_20 + (uint32_t)mem_read16(screen + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(current_object)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (int32_t)(r2 * r4 + r3 * r5) >> 11;
	draw_object_at_scale(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode B (the traffic cars'): as draw_object_from_road_edge, at twice the size of
   draw_object_from_road_edge_unshifted (scale_shift 1) whatever the object's. */
void draw_object_from_road_edge_doubled(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1_2 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	r1 = r0;
	t1 = RACE(SCREEN(screen)->race_copy)->unk_46;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = (int16_t)mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 + 0x10u);
	r12 = screen + 0x3000u + r0;
	r3 = (int16_t)mem_read16(r12);
	if (r3 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r1 = (uint16_t)(r1 << 5);
	r5 = (uint16_t)OBJECT(current_object)->unk_28;
	r4 = (uint16_t)(0x80u - r5);
	r0 = (int16_t)OBJECT(current_object)->unk_20 + (uint32_t)mem_read16(screen + 0x2000u + (r1 & 0xFFFu) + 0x12u) * (int8_t)OBJECT(current_object)->unk_3F;
	r0 = (int32_t)((int16_t)(((int32_t)(r0 * r2) >> 12) + mem_read16(r11 + 4u)) * r4 + (int16_t)(((int32_t)(r0 * r3) >> 12) + mem_read16(r12 + 4u)) * r5) >> 7;
	r1_2 = (uint16_t)((int32_t)((int16_t)mem_read16(r11 + 6u) * r4 + (int16_t)mem_read16(r12 + 6u) * r5) >> 7);
	r2 = (int32_t)(r2 * r4 + r3 * r5) >> 10;
	draw_object_at_scale(r1_2, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode 4: as draw_object_at_segment, but the segment counted from the screen car's own
   window_segment instead of the view drawn (race_copy unk_46). */
void draw_object_at_car_window_segment(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t window_segment = 0, t1 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	window_segment = CAR(player_car)->window_segment;
	t1 = r0;
	r0 = (uint16_t)(t1 - window_segment);
	if ((int16_t)t1 < (int16_t)window_segment)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(current_object)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = (uint16_t)(((int32_t)((int16_t)OBJECT(current_object)->unk_24 * r2) >> 12) + mem_read16(r11 + 6u));
	draw_object_at_segment_scale(r1, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode 9: the object at the road's segment of its position (field 1C, counted from the view drawn:
   race_copy unk_46), offset across and up by fields 20 and 24 scaled by the segment's scale, its size
   that scale shifted by scale_shift; not drawn when the segment is behind or not projected. */
void draw_object_at_segment(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t t1 = 0, t2 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	t1 = RACE(SCREEN(screen)->race_copy)->unk_46;
	t2 = r0;
	r0 = (uint16_t)(t2 - t1);
	if ((int16_t)t2 < (int16_t)t1)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(current_object)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = (uint16_t)(((int32_t)((int16_t)OBJECT(current_object)->unk_24 * r2) >> 12) + mem_read16(r11 + 6u));
	r2 = shift_by(r2, OBJECT(current_object)->scale_shift, 32) >> 4;
	draw_object_at_scale(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draw mode 3: as draw_object_at_car_window_segment, on the road's surface (no up offset). */
void draw_object_at_car_window_segment_on_road(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0;
	uint16_t window_segment = 0, t1 = 0;

	r0 = OBJECT(current_object)->unk_1C;
	window_segment = CAR(player_car)->window_segment;
	t1 = r0;
	r0 = (uint16_t)(t1 - window_segment);
	if ((int16_t)t1 < (int16_t)window_segment)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (uint16_t)(r0 * 0x10u);
	r11 = screen + 0x3000u + r0;
	r2 = mem_read16(r11);
	if (r2 == 0u)
	{
		*p_left_caller = 1u;
		*p_r0 = r0;
		return;
	}
	r0 = (int32_t)((int16_t)OBJECT(current_object)->unk_20 * r2) >> 12;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + mem_read16(r11 + 4u));
	r1 = mem_read16(r11 + 6u);
	draw_object_at_segment_scale(r1, r11, &r0, p_r10, p_left_caller);
	*p_r0 = r0;
}

/* Draws the object at R0, R1 at the projected segment R11's own scale (draw_object_at_shifted_scale). */
void draw_object_at_segment_scale(uint16_t r1, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;
	uint32_t r2 = 0;

	r2 = mem_read16(r11);
	draw_object_at_shifted_scale(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

/* Draws the object at R0, R1 at the scale R2 shifted by its scale_shift (and >> 4)
   (draw_object_at_scale). */
void draw_object_at_shifted_scale(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;

	r2 = shift_by(r2, OBJECT(current_object)->scale_shift, 32) >> 4;
	draw_object_at_scale(r1, r2, r11, &r0, p_r10, p_left_caller);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

/* Places a road object projected at R0, R1 with size R2 (both scales): x and y; off the screen (x beyond
   1A0 or -1A0, y beyond 80 or -160) back to draw_object's caller undrawn; the depth_layer of the
   projected segment R11 (+8), then put_object_sprites. */
void draw_object_at_scale(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller)
{
	uint32_t r0 = *p_r0;
	uint32_t r3 = 0, left_caller_2 = 0;

	OBJECT(current_object)->x = r0;
	OBJECT(current_object)->y = r1;
	if ((int16_t)r0 > 0x1A0 || (int16_t)r0 < -0x1A0 || (int16_t)r1 > 0x80 || (int16_t)r1 < -0x160)
	{
		left_caller_2 = 1u;
		*p_left_caller = left_caller_2;
		return;
	}
	r3 = r2;
	OBJECT(current_object)->depth_layer = mem_read16(r11 + 8u);
	put_object_sprites(r1, r2, r3, &r0, p_r10);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

/* Draw mode 0: the object at x, y on the screen, less the screen's scroll (screen unk_14, unk_16), at
   its width_scale and height_scale (put_object_sprites). */
void draw_object_on_screen(uint32_t *p_r0, uint32_t *p_r10)
{
	uint16_t r1 = 0;
	uint32_t r0 = 0, height_scale = 0, width_scale = 0;

	r0 = (uint16_t)(OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x);
	r1 = (uint16_t)(OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y);
	height_scale = OBJECT(current_object)->height_scale;
	width_scale = OBJECT(current_object)->width_scale;
	put_object_sprites(r1, height_scale, width_scale, &r0, p_r10);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | (r0 & 0xFFFFu);
}

/* Writes the object's frame (animation, frame) into the screen's sprite list at R10 and its copy at +
   3000, a sprite of 8 halfwords for each piece: its word flipped by flags bits 6-7, its size from
   0x122B74 by size_step scaled by R2 (height) and R3 (width), placed at R0, R1 plus its offsets scaled
   the same way (signs flipped by the flags: rom_00F13C, rom_00F140), its palette word from rom_032BA4 by
   palette; R10 past them. */
void put_object_sprites(uint16_t r1, uint32_t r2, uint32_t r3, uint32_t *p_r0, uint32_t *p_r10)
{
	uint16_t r0 = (uint16_t)*p_r0;
	uint32_t r10 = *p_r10;
	uint16_t r0_2 = 0, r1_2 = 0, r2_2 = 0, r3_2 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r19 = 0;
	uint32_t r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r26 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	r8 = (uint16_t)r0;
	r9 = (uint16_t)r1;
	r16 = r2;
	r15 = r3;
	r12 = mem_read32(rom_034138[OBJECT(current_object)->animation] + 4u + OBJECT(current_object)->frame * 4u);
	r13 = rom_032BA4[OBJECT(current_object)->palette];
	r19 = AS16(OBJECT(current_object)->flags) & 0xC0u;
	r26 = r19;
	r26 = r26 >> 5;
	r17 = r15 * rom_00F13C[r26];
	r18 = r16 * rom_00F140[r26];
	r11 = mem_read16(r12);
	r12 = r12 + 2u;
	do
	{
		r0_2 = mem_read16(r12) ^ r19;
		r26 = OBJECT(current_object)->size_step * 3u;
		r12 = r12 + 2u;
		r14 = 0x122B74u + (uint16_t)(r26 + mem_read16(r12)) * 4u;
		t1 = ROM16(r14);
		r14 = r14 + 2u;
		r1_2 = t1;
		t2 = ROM16(r14);
		r14 = r14 + 2u;
		r6 = t2;
		t3 = mem_read16(r13);
		r13 += 2u;
		r7 = t3;
		t4 = ROM16(r14);
		r14 = r14 + 2u;
		t5 = ROM16(r14);
		r14 = r14 + 2u;
		r2_2 = (((t4 * r16) >> 8) & 0x3FFu) | t5;
		t6 = ROM16(r14);
		r14 = r14 + 2u;
		t7 = ROM16(r14);
		r14 = r14 + 2u;
		r3_2 = (((t6 * r15) >> 8) & 0x3FFu) | t7 | SCREEN(screen)->sprite_screen_bit;
		r12 = r12 + 2u;
		r4 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r18) >> 8) + r9);
		r12 = r12 + 2u;
		r5 = (uint16_t)(((int32_t)((int16_t)mem_read16(r12) * r17) >> 8) + r8);
		mem_write16(r10 + 0x3000u, r0_2);
		mem_write16(r10, r0_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r1_2);
		mem_write16(r10, r1_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r2_2);
		mem_write16(r10, r2_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r3_2);
		mem_write16(r10, r3_2);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r4);
		mem_write16(r10, r4);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r5);
		mem_write16(r10, r5);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r6);
		mem_write16(r10, r6);
		r10 += 2u;
		mem_write16(r10 + 0x3000u, r7);
		mem_write16(r10, r7);
		r10 += 2u;
		r11--;
		r12 = r12 + 2u;
	} while (r11 != 0u);
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0_2;
	*p_r10 = r10;
}

/* Draw mode 8: the first piece of the animation's frame at its own size (no zoom, no flip), at x, y on
   the screen (less sprite_origin_x, sprite_origin_y), with the first word of its palette (rom_032BA4),
   into the sprite list and its halfway copy alike. */
void draw_object_one_piece(uint32_t *p_r0, uint32_t *p_r10)
{
	uint32_t r10 = *p_r10;
	uint16_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;
	uint32_t r26 = 0;

	r12 = mem_read32(rom_034138[OBJECT(current_object)->animation] + 4u + OBJECT(current_object)->frame * 4u) + 2u;
	r0 = mem_read16(r12);
	r26 = OBJECT(current_object)->size_step * 3u;
	r12 = r12 + 2u;
	r14 = 0x122B74u + (uint16_t)(r26 + mem_read16(r12)) * 4u;
	r1 = ROM16(r14);
	r14 = r14 + 2u;
	r6 = ROM16(r14);
	r7 = mem_read16(rom_032BA4[OBJECT(current_object)->palette]);
	r14 = r14 + 2u;
	r2 = ROM16(r14);
	r14 = r14 + 2u;
	r2 |= ROM16(r14);
	r14 = r14 + 2u;
	r3 = ROM16(r14);
	r14 = r14 + 2u;
	r3 = r3 | ROM16(r14) | SCREEN(screen)->sprite_screen_bit;
	r12 = r12 + 2u;
	r4 = (uint16_t)(mem_read16(r12) + (OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y));
	r12 = r12 + 2u;
	r5 = (uint16_t)(mem_read16(r12) + (OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x));
	mem_write16(r10 + 0x3000u, r0);
	mem_write16(r10, r0);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r1);
	mem_write16(r10, r1);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r2);
	mem_write16(r10, r2);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r3);
	mem_write16(r10, r3);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r4);
	mem_write16(r10, r4);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r5);
	mem_write16(r10, r5);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r6);
	mem_write16(r10, r6);
	r10 += 2u;
	mem_write16(r10 + 0x3000u, r7);
	mem_write16(r10, r7);
	r10 += 2u;
	r12 = r12 + 2u;
	r14 = r14 + 2u;
	*p_r0 = (*p_r0 & 0xFFFF0000u) | r0;
	*p_r10 = r10;
}

/* Draw mode 1: copies a ready 16-byte sprite entry, from the address in fields 38-3B (animation and
   palette read as one 32-bit address), into the sprite list and its halfway copy, with the screen's bit
   in its fourth word (sprite_screen_bit). Not run in the scenarios. */
uint32_t draw_object_entry(uint32_t r10)
{
	move_halfwords(r10, AS32(OBJECT(current_object)->animation), 8u);
	move_string(AS32(OBJECT(current_object)->animation), 8u, r10 + 0x3000u, 8u, 2u, 0u, 0u, 0u, &r27, &r28);
	mem_write16(r10 + 6u, mem_read16(r10 + 6u) | SCREEN(screen)->sprite_screen_bit);
	mem_write16(r10 + 0x3006u, mem_read16(r10 + 0x3006u) | SCREEN(screen)->sprite_screen_bit);
	r10 += 0x10u;
	return r10;
}

/* Draw mode 6: copies a ready 16-byte entry from the address in fields 38-3B into the sprite list and
   its halfway copy as it is, without the screen's bit: for list commands such as crest_clip_update's
   clip. */
uint32_t draw_object_command(uint32_t r10)
{
	move_halfwords(r10, AS32(OBJECT(current_object)->animation), 8u);
	move_string(AS32(OBJECT(current_object)->animation), 8u, r10 + 0x3000u, 8u, 2u, 0u, 0u, 0u, &r27, &r28);
	r10 += 0x10u;
	return r10;
}

/* Draw mode A: every piece of the animation's frame at x, y on the screen (less sprite_origin_x,
   sprite_origin_y), zoomed by width_scale and height_scale, flipped by flags bits 6-7 (with bit 7 the
   pieces' offsets are negated, rom_00F4EC), each with its palette word; the halfway copy (+3000) is
   drawn from the averages of this frame's x, y, width_scale and height_scale and the last frame's, kept
   in the ring of sprite places (next_sprite_place, 16 bytes a record; the object's field 50 points at
   its record) when flags bit 5 says it was drawn the frame before. Keeps this frame's values in a new
   record and sets flags bit 5. */
void draw_object_smoothed(uint32_t *p_r0, uint32_t *p_r10)
{
	uint32_t r10 = *p_r10;
	uint16_t r4 = 0, r9 = 0, r15 = 0, r16 = 0, r19 = 0, r26_2 = 0, sprite_screen_bit = 0;
	uint32_t r0 = 0, r1 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r13 = 0, r15_2 = 0, r18 = 0, r26 = 0, height_scale = 0, width_scale = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
	uint32_t flags = 0;

	r28 = OBJECT(current_object)->unk_50;
	r27 = SCREEN(screen)->next_sprite_place;
	r0 = (uint32_t)(int16_t)OBJECT(current_object)->x - SCREEN(screen)->sprite_origin_x;
	r1 = (uint32_t)(int16_t)OBJECT(current_object)->y - SCREEN(screen)->sprite_origin_y;
	width_scale = OBJECT(current_object)->width_scale;
	height_scale = OBJECT(current_object)->height_scale;
	flags = AS32(OBJECT(current_object)->flags);
	AS32(OBJECT(current_object)->flags) = flags | 0x20u;
	if ((flags & 0x20u) == 0u)
	{
		r4 = (uint16_t)r0;
		r5 = r1;
		r6 = width_scale;
		r7 = height_scale;
	}
	else
	{
		r7 = mem_read16(r28);
		r28 = r28 + 2u;
		r6 = mem_read16(r28);
		r28 = r28 + 2u;
		r5 = (int16_t)mem_read16(r28);
		r28 = r28 + 2u;
		r4 = (uint16_t)((int32_t)((int16_t)mem_read16(r28) + r0) >> 1);
		r5 = (int32_t)(r5 + r1) >> 1;
		r6 = (r6 + width_scale) >> 1;
		r7 = (r7 + height_scale) >> 1;
	}
	OBJECT(current_object)->unk_50 = r27;
	mem_write16(r27, height_scale);
	r27 = r27 + 2u;
	mem_write16(r27, width_scale);
	r27 = r27 + 2u;
	mem_write16(r27, r1);
	r27 = r27 + 2u;
	mem_write16(r27, r0);
	r27 = r27 + 2u;
	r27 = r27 + 8u;
	r27 = (r27 & ~0xFFFFu) | (r27 & 0xEFFFu);
	SCREEN(screen)->next_sprite_place = r27;
	r12 = mem_read32(rom_034138[OBJECT(current_object)->animation] + 4u + OBJECT(current_object)->frame * 4u);
	r13 = rom_032BA4[OBJECT(current_object)->palette];
	r19 = AS16(OBJECT(current_object)->flags) & 0xC0u;
	r26 = r19;
	r18 = rom_00F4EC[r26 >> 5];
	r28 = mem_read16(r12);
	r11 = r10 + 0x3000u;
	r12 = r12 + 2u;
	do
	{
		r27 = (r27 & ~0xFFFFu) | mem_read16(r12);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 ^ r19);
		mem_write16(r10, r27);
		r10 += 2u;
		mem_write16(r11, r27);
		r11 += 2u;
		r26 = OBJECT(current_object)->size_step * 3u;
		r12 = r12 + 2u;
		r14 = 0x122B74u + (uint16_t)(r26 + mem_read16(r12)) * 4u;
		t1 = ROM16(r14);
		r14 = r14 + 2u;
		r15 = t1;
		mem_write16(r10, r15);
		r10 += 2u;
		mem_write16(r11, r15);
		r11 += 2u;
		t2 = ROM16(r14);
		r14 = r14 + 2u;
		r26_2 = t2;
		t3 = ROM16(r14);
		r14 = r14 + 2u;
		r15_2 = t3;
		t4 = ROM16(r14);
		r14 = r14 + 2u;
		r9 = t4;
		mem_write16(r10, (((r15_2 * height_scale) >> 8) & 0x3FFu) | r9);
		r10 += 2u;
		mem_write16(r11, (((r15_2 * r7) >> 8) & 0x3FFu) | r9);
		r11 += 2u;
		t5 = ROM16(r14);
		r14 = r14 + 2u;
		r15_2 = t5;
		r16 = ((r15_2 * r6) >> 8) & 0x3FFu;
		t6 = ROM16(r14);
		r14 = r14 + 2u;
		r9 = t6;
		r15_2 = (((r15_2 * width_scale) >> 8) & 0x3FFu) | r9;
		r16 |= r9;
		sprite_screen_bit = SCREEN(screen)->sprite_screen_bit;
		mem_write16(r10, r15_2 | sprite_screen_bit);
		r10 += 2u;
		mem_write16(r11, r16 | sprite_screen_bit);
		r11 += 2u;
		r12 = r12 + 2u;
		r15_2 = (int16_t)mem_read16(r12) * r18;
		mem_write16(r10, ((int32_t)(r15_2 * height_scale) >> 8) + r1);
		r10 += 2u;
		mem_write16(r11, ((int32_t)(r15_2 * r7) >> 8) + r5);
		r11 += 2u;
		r12 = r12 + 2u;
		r15_2 = (int16_t)mem_read16(r12) * r18;
		mem_write16(r10, ((int32_t)(r15_2 * width_scale) >> 8) + r0);
		r10 += 2u;
		mem_write16(r11, (uint32_t)((int32_t)(r15_2 * r6) >> 8) + r4);
		r11 += 2u;
		mem_write16(r10, r26_2);
		r10 += 2u;
		mem_write16(r11, r26_2);
		r11 += 2u;
		t7 = mem_read16(r13);
		r13 += 2u;
		r27 = (r27 & ~0xFFFFu) | t7;
		mem_write16(r10, r27);
		r10 += 2u;
		mem_write16(r11, r27);
		r11 += 2u;
		r28 = r28 - 1u;
		r12 = r12 + 2u;
	} while (r28 != 0u);
	*p_r0 = r0;
	*p_r10 = r10;
}

/* Steps the object's animation: nothing while frame_time is 0; when frame_countdown runs out (back to
   frame_time), the next frame. A frame's entry in the record (32 bits at +4 + frame * 4) is the address
   of its data when positive, else an order: 0 holds the frame before (frame_time 0), -1 goes back to
   frame 0, -2 starts the animation whose number is the next entry. */
uint32_t animation_advance(uint32_t r7)
{
	uint32_t r7_2 = 0, r8 = 0, r9 = 0, frame = 0;
	uint8_t frame_countdown = 0;
	uint32_t t1 = 0;

	if (OBJECT(current_object)->animation_view.frame_time == 0u)
		return r7;
	frame_countdown = OBJECT(current_object)->animation_view.frame_countdown;
	OBJECT(current_object)->animation_view.frame_countdown = frame_countdown - 1u;
	if ((int8_t)frame_countdown > 1)
		return r7;
	OBJECT(current_object)->animation_view.frame_countdown = OBJECT(current_object)->animation_view.frame_time;
	OBJECT(current_object)->frame++;
	frame = OBJECT(current_object)->frame;
	r8 = ROM32(OBJECT(current_object)->animation * 2u * 2u + rom_034138_ADDRESS);
	if ((int32_t)mem_read32(r8 + 4u + frame * 4u) > 0)
		return r7;
	t1 = 0u - mem_read32(r8 + 4u + frame * 4u);
	r7_2 = rom_00F34C[t1];
	switch (t1)
	{
	case 0:
		OBJECT(current_object)->animation_view.frame_time = 0u;
		OBJECT(current_object)->frame--;
		return r7_2;
	case 2:
		r9 = mem_read32(r8 + 8u + frame * 4u);
		animation_start(r9);
		return r7_2;
	case 1:
		OBJECT(current_object)->frame = 0u;
		return r7_2;
	default:
		invalid_jump(rom_00F34C[t1]);
	}
}

/* Starts animation R9 (rom_034138[R9] is its record) at its first frame: frame_time and frame_countdown
   from the record's first byte, size_step 0, and fields 34 and 36 to 100 (the width and height zoom of
   the screen-position drawing, draw_object_on_screen: 100 the sprite's own size). */
void animation_start(uint32_t r9)
{
	uint8_t r9_2 = 0;

	OBJECT(current_object)->animation = r9;
	r9_2 = mem_read8(rom_034138[r9]);
	OBJECT(current_object)->animation_view.frame_time = r9_2;
	OBJECT(current_object)->animation_view.frame_countdown = r9_2;
	OBJECT(current_object)->size_step = 0u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->frame = 0u;
}

/* Finishes the screen's sprite list in the vertical blank (int_vector_40): picks a record of rom_00F508
   by the screen (index) and sprite_list_buffers; when groups were drawn (sprite_groups), chains the
   depth lists from layer 0 to 415 (depth_lists: the list's start entry jumps to the first layer's newest
   group, each layer's tail to the next layer's head; the heads cleared), in the list and its halfway
   copy, and ends the chain with the record's word (a jump on to sprite entry 3, screen 2's list, after
   screen 1's; entry 4, the end, after screen 2's); points sprite RAM entries 2 and 3 (the jumps to each
   screen's list) at the halfway copy of the list just finished and at the other screen's finished list;
   then the next buffer (sprite_list_buffers, sprite_list_last, sprite_list_limit) and sprite_groups 0. */
void finish_sprite_list(void)
{
	uint16_t r3 = 0, r4 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	r9 = SCREEN(screen)->index;
	r9 = (uint8_t)(r9 * 2u);
	r9 = r9 | (uint8_t)sprite_list_buffers;
	r9 = (uint8_t)(r9 * 2u);
	r19 = rom_00F508[r9 | (uint8_t)(sprite_list_buffers >> 8u)];
	r10 = mem_read32(r19);
	r19 += 4u;
	if (SCREEN(screen)->sprite_groups != 0u)
	{
		r11 = screen + 0x5000u;
		r1 = 0x34u;
		r0 = 0u;
		r2 = 0u;
		r3 = 0x300u;
		r4 = 4u;
		do
		{
			if (mem_read16(r11) != 0u)
			{
				t1 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t1);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t2 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t2);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t3 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t3);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t4 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t4);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t5 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t5);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t6 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t6);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t7 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t7);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			if (mem_read16(r11) != 0u)
			{
				t8 = r2;
				r2 = mem_read16(r11);
				mem_write16(r11, t8);
				mem_write32(r10, r2);
				mem_write32(r10 + 0x3000u, (uint16_t)(r2 + r3));
				r10 = (r10 & ~0xFFFFu) | mem_read16(r11 + 2u);
				r2 = r0;
			}
			r11 = (r11 & ~0xFFFFu) | (uint16_t)(r11 + r4);
			r1--;
		} while (r1 != 0u);
	}
	mem_write16(r10, mem_read16(r19));
	mem_write16(r10 + 0x3000u, mem_read16(r19));
	r19 += 2u;
	mem_write16(SPRITE_RAM_400020, mem_read16(r19));
	r19 += 2u;
	mem_write16(SPRITE_RAM_400030, mem_read16(r19));
	r19 += 2u;
	sprite_list_buffers = mem_read16(r19);
	r19 += 2u;
	SCREEN(screen)->sprite_list_last = mem_read32(r19);
	SCREEN(screen)->sprite_list_limit = mem_read32(r19 + 4u);
	SCREEN(screen)->sprite_groups = 0u;
}

/* Draws the row's items in view: ends the object once end_position is not past the view's start
   (race_copy unk_46); nothing while first_position is beyond the draw distance (SCREEN(screen)->unk_12
   >> 7 segments); moves first_position up to the first item in view; for each item (spacing apart, to
   the draw distance) where the road has a scale: x = (the road's width * side + x_offset) * scale >> 12
   plus the road's screen x unless screen_centered, y = y_offset * scale >> 12 plus the road's screen y;
   off the screen (x beyond -1E0..1E0, y beyond -160..80) it is skipped, else the scale shifted by
   scale_shift picks the size (size_table: a size step and a shift) and a sprite of the animation's frame
   with its palette (flags bits 6-7 flip it) goes into the screen's sprite list (unk_C0; the copy at
   +3000 is the halfway point from the item's sprite of the frame before, previous_sprites) and the depth
   list of its road segment (screen + 5000). Sets flags bit 5 and keeps where its sprites went in
   previous_sprites. Used by scenery, fork_sign, animated_scenery, flashing_scenery and overhead_scenery
   (and 01351E). */
void road_row_draw(uint32_t r7, uint32_t r15)
{
	uint16_t r3 = 0, r4_2 = 0, r5 = 0, r6 = 0, r21 = 0, r26 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3_2 = 0, r4 = 0, r6_2 = 0, r8 = 0, r9 = 0, r10 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26_2 = 0, first_position = 0, size_table = 0, spacing = 0;
	uint16_t t1 = 0;
	uint32_t flags = 0, t2 = 0;

	r0 = (uint16_t)OBJECT(current_object)->road_row.first_position;
	r1 = (uint16_t)OBJECT(current_object)->road_row.end_position;
	r2 = RACE(SCREEN(screen)->race_copy)->unk_46;
	r3 = (uint16_t)(r2 + (SCREEN(screen)->unk_12 >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)r1 <= (int16_t)r2)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	if ((int16_t)r3 <= (int16_t)r0)
		return;
	while ((int16_t)r0 < (int16_t)r2)
	{
		r0 = (uint16_t)(r0 + spacing);
	}
	if ((int16_t)r3 < (int16_t)r1)
		r1 = r3;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = r0;
	r1 = (uint16_t)(r1 - r0);
	r1 = (uint16_t)(r1 + spacing);
	r1 = (uint16_t)(r1 - 1u);
	t1 = divide_u16(r1, spacing);
	r1 = t1;
	if ((int16_t)t1 <= 0)
		return;
	r28 = 0x100000u;
	r27 = SCREEN(screen)->next_sprite_place;
	r21 = 0xEFFFu;
	flags = AS32(OBJECT(current_object)->flags);
	AS32(OBJECT(current_object)->flags) = flags | 0x20u;
	if ((flags & 0x20u) != 0u)
	{
		r28 = OBJECT(current_object)->road_row.previous_sprites;
		t2 = first_position;
		r3_2 = t2 - r0;
		if (t2 != r0)
		{
			r28 = r28 - (divide_s32(r3_2, spacing) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = r27;
	r18 = screen + 0x2000u + ((r0 << 5) & 0xFFFu);
	r19 = screen + 0x3000u + ((r0 - r2) << 4);
	r9 = spacing << 4;
	r8 = spacing << 5;
	r17 = r1;
	r12 = mem_read32(rom_034138[OBJECT(current_object)->animation] + 4u + OBJECT(current_object)->frame * 4u) + 2u;
	r16 = (uint32_t)mem_read16(rom_032BA4[OBJECT(current_object)->palette]) << 16;
	r0 = AS16(OBJECT(current_object)->flags);
	r0 = r0 & 0xC0u;
	r0 = r0 ^ mem_read16(r12);
	r12 = r12 + 2u;
	r1 = mem_read16(r12);
	r13 = 0x122B74u + r1 * 4u;
	r10 = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		r3_2 = mem_read16(r19);
		if (r3_2 != 0u && r3_2 <= 0x1200u)
		{
			r6 = (uint16_t)((int32_t)(((uint32_t)mem_read16(r18 + 0x12u) * OBJECT(current_object)->road_row.side + OBJECT(current_object)->road_row.x_offset) * r3_2) >> 12);
			r5 = (uint16_t)(((int32_t)(OBJECT(current_object)->road_row.y_offset * r3_2) >> 12) + mem_read16(r19 + 6u));
			if (OBJECT(current_object)->road_row.screen_centered == 0u)
				r6 += mem_read16(r19 + 4u);
			if ((int16_t)r6 > 0x1E0 || (int16_t)r6 < -0x1E0 || (int16_t)r5 > 0x80 || (int16_t)r5 < -0x160)
			{
				mem_write32(r27 + 4u, 0xF800F800u);
				r18 += r8;
				r19 += r9;
				r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
				r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
				r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
				r27 = (r27 & ~0xFFFFu) | (r27 & r21);
				r28 = (r28 & ~0xFFFFu) | (r28 & r21);
				r17--;
				if (r17 != 0u)
					continue;
				SCREEN(screen)->sprite_list_last = r10;
				break;
			}
			r3_2 = shift_by(r3_2, OBJECT(current_object)->scale_shift, 32);
			r4 = r3_2;
			if (r4 >= 0x1000u)
				r4 = 0xFFFu;
			r4 = r4 >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			r3_2 = shift_by(r3_2, (uint8_t)(int8_t)mem_read8(rom_159AFC[size_table] + r4), 32);
			r4 = mem_read8(rom_1598CC[size_table] + r4);
			if (r4 != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = r4;
				r4 = r13 + r4 * 3u * 4u;
				r0 = (r0 & 0xFFFFu) | ((uint32_t)ROM16(r4) << 16);
				r4 += 2u;
				r7 = ROM16(r4);
				r4 += 2u;
				r14 = ROM16(r4);
				r4 += 2u;
				r1 = ROM16(r4);
				r4 += 2u;
				r15 = ROM16(r4);
				r2 = ROM16(r4 + 2u) | SCREEN(screen)->sprite_screen_bit;
				r7 |= r16;
			}
			r4_2 = (uint16_t)((r3_2 * r15) >> 8);
			r3_2 = ((r3_2 * r14) >> 8) & 0x3FFu;
			r4_2 &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= r10)
				break;
			mem_write32(r10 + 0x3000u, r0);
			mem_write32(r10, r0);
			mem_write16(r27 + 8u, r7);
			r10 += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)r7)
			{
				r26 = mem_read16(r28);
				mem_write16(r27, r3_2);
				mem_write16(r10 + 0x3000u, ((uint16_t)(r26 + r3_2) >> 1) | r1);
				mem_write16(r10, r3_2 | r1);
				r26 = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, r4_2);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, ((uint16_t)(r26 + r4_2) >> 1) | r2);
				mem_write16(r10, r4_2 | r2);
				r26 = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, r5);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, (int16_t)(r26 + r5) >> 1);
				mem_write16(r10, r5);
				r26 = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, (int16_t)(r26 + r6) >> 1);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			else
			{
				mem_write16(r27, r3_2);
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3000u, r3_2);
				mem_write16(r10, r3_2);
				mem_write16(r27 + 2u, r4_2);
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r4_2);
				mem_write16(r10, r4_2);
				mem_write16(r27 + 4u, r5);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r5);
				mem_write16(r10, r5);
				mem_write16(r27 + 6u, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3000u, r6);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			mem_write32(r10 + 0x3000u, r7);
			mem_write32(r10, r7);
			r4 = screen + 0x5000u + mem_read16(r19 + 8u) * 4u;
			r10 += 4u;
			if (mem_read16(r4) == 0u)
				mem_write16(r4 + 2u, r10);
			r6_2 = mem_read16(r4);
			mem_write32(r10, r6_2);
			mem_write32(r10 + 0x3000u, (uint16_t)(r6_2 + 0x300u));
			mem_write16(r4, ((r10 - 0x10u) >> 4) | 0x8000u);
			SCREEN(screen)->sprite_groups++;
			r10 += 0x10u;
		}
		r18 += r8;
		r19 += r9;
		r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & r21);
		r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		r17--;
		if (r17 == 0u)
		{
			SCREEN(screen)->sprite_list_last = r10;
			break;
		}
	}
	r26_2 = rom_100000;
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	SCREEN(screen)->next_sprite_place = r27;
}

/* road_row_draw for the rows drawn in mirrored pairs: each item twice, at the same distance either side
   of the road's centre, the second with sprite attribute bits 43 toggled (bit 6 flips it, bits 0-1 swap
   its X alignment); there is no off-screen test and no screen_centered. Used by scenery_pair and
   animated_scenery_pair. */
void road_row_pair_draw(uint32_t r7, uint32_t r15)
{
	uint16_t r3 = 0, r4_2 = 0, r5 = 0, r21 = 0, r26 = 0, r30 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3_2 = 0, r4 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r13 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26_2 = 0, first_position = 0, size_table = 0, spacing = 0;
	uint16_t t1 = 0;
	uint32_t flags = 0, t2 = 0;

	r30 = 0x43u;
	r0 = (uint16_t)OBJECT(current_object)->road_row.first_position;
	r1 = (uint16_t)OBJECT(current_object)->road_row.end_position;
	r2 = RACE(SCREEN(screen)->race_copy)->unk_46;
	r3 = (uint16_t)(r2 + (SCREEN(screen)->unk_12 >> 7));
	spacing = OBJECT(current_object)->road_row.spacing;
	if ((int16_t)r1 <= (int16_t)r2)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->unk_01 = 0u;
		return;
	}
	if ((int16_t)r3 <= (int16_t)r0)
		return;
	while ((int16_t)r0 < (int16_t)r2)
	{
		r0 = (uint16_t)(r0 + spacing);
	}
	if ((int16_t)r3 < (int16_t)r1)
		r1 = r3;
	first_position = (uint16_t)OBJECT(current_object)->road_row.first_position;
	OBJECT(current_object)->road_row.first_position = r0;
	r1 = (uint16_t)(r1 - r0);
	r1 = (uint16_t)(r1 + spacing);
	r1 = (uint16_t)(r1 - 1u);
	t1 = divide_u16(r1, spacing);
	r1 = t1;
	if ((int16_t)t1 <= 0)
		return;
	r28 = 0x100000u;
	r27 = SCREEN(screen)->next_sprite_place;
	r21 = 0xEFFFu;
	flags = AS32(OBJECT(current_object)->flags);
	AS32(OBJECT(current_object)->flags) = flags | 0x20u;
	if ((flags & 0x20u) != 0u)
	{
		r28 = OBJECT(current_object)->road_row.previous_sprites;
		t2 = first_position;
		r3_2 = t2 - r0;
		if (t2 != r0)
		{
			r28 = r28 - (divide_s32(r3_2, spacing) << 3);
			r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		}
	}
	OBJECT(current_object)->road_row.previous_sprites = r27;
	r18 = screen + 0x2000u + ((r0 << 5) & 0xFFFu);
	r19 = screen + 0x3000u + ((r0 - r2) << 4);
	r9 = spacing << 4;
	r8 = spacing << 5;
	r17 = r1;
	r12 = mem_read32(rom_034138[OBJECT(current_object)->animation] + 4u + OBJECT(current_object)->frame * 4u) + 2u;
	r16 = (uint32_t)mem_read16(rom_032BA4[OBJECT(current_object)->palette]) << 16;
	r0 = AS16(OBJECT(current_object)->flags);
	r0 = r0 & 0xC0u;
	r0 = r0 ^ mem_read16(r12);
	r12 = r12 + 2u;
	r1 = mem_read16(r12);
	r13 = 0x122B74u + r1 * 4u;
	r10 = SCREEN(screen)->sprite_list_last + 0x10u;
	OBJECT(current_object)->size_step = 0xFFu;
	r12 = r12 + 2u;
	for (;;)
	{
		r3_2 = mem_read16(r19);
		if (r3_2 != 0u && r3_2 <= 0x1200u)
		{
			r6 = (int32_t)(((uint32_t)mem_read16(r18 + 0x12u) * OBJECT(current_object)->road_row.side + OBJECT(current_object)->road_row.x_offset) * r3_2) >> 12;
			r5 = (uint16_t)(((int32_t)(OBJECT(current_object)->road_row.y_offset * r3_2) >> 12) + mem_read16(r19 + 6u));
			r26 = (uint16_t)r6;
			r6 = (r6 & ~0xFFFFu) | (uint16_t)(r6 + mem_read16(r19 + 4u));
			r6 = (((r6 << 16) | (r6 >> 16)) & ~0xFFFFu) | (uint16_t)(0u - r26);
			r6 = (r6 & ~0xFFFFu) | (uint16_t)(r6 + mem_read16(r19 + 4u));
			r3_2 = shift_by(r3_2, OBJECT(current_object)->scale_shift, 32);
			r4 = r3_2;
			if (r4 >= 0x1000u)
				r4 = 0xFFFu;
			r4 = r4 >> 5;
			size_table = OBJECT(current_object)->road_row.size_table;
			r3_2 = shift_by(r3_2, (uint8_t)(int8_t)mem_read8(rom_159AFC[size_table] + r4), 32);
			r4 = mem_read8(rom_1598CC[size_table] + r4);
			if (r4 != OBJECT(current_object)->size_step)
			{
				OBJECT(current_object)->size_step = r4;
				r4 = r13 + r4 * 3u * 4u;
				r0 = (r0 & 0xFFFFu) | ((uint32_t)ROM16(r4) << 16);
				r4 += 2u;
				r7 = ROM16(r4);
				r4 += 2u;
				r14 = ROM16(r4);
				r4 += 2u;
				r1 = ROM16(r4);
				r4 += 2u;
				r15 = ROM16(r4);
				r2 = ROM16(r4 + 2u) | SCREEN(screen)->sprite_screen_bit;
				r7 |= r16;
			}
			r4_2 = (uint16_t)((r3_2 * r15) >> 8);
			r3_2 = ((r3_2 * r14) >> 8) & 0x3FFu;
			r4_2 &= 0x3FFu;
			if (SCREEN(screen)->sprite_list_limit <= r10)
				break;
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r30);
			mem_write32(r10 + 0x3010u, r0);
			mem_write32(r10 + 0x10u, r0);
			r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 ^ r30);
			mem_write32(r10 + 0x3000u, r0);
			mem_write32(r10, r0);
			mem_write16(r27 + 8u, r7);
			r10 += 4u;
			if (mem_read16(r28 + 8u) == (uint16_t)r7)
			{
				r26 = mem_read16(r28);
				mem_write16(r27, r3_2);
				r26 = ((uint16_t)(r26 + r3_2) >> 1) | r1;
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r3_2);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r3_2);
				r26 = mem_read16(r28 + 2u);
				mem_write16(r27 + 2u, r4_2);
				r26 = ((uint16_t)(r26 + r4_2) >> 1) | r2;
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r4_2);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r4_2);
				r26 = mem_read16(r28 + 4u);
				mem_write16(r27 + 4u, r5);
				r26 = (uint16_t)((int16_t)(r26 + r5) >> 1);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r26);
				mem_write16(r10 + 0x10u, r5);
				mem_write16(r10 + 0x3000u, r26);
				mem_write16(r10, r5);
				r26 = mem_read16(r28 + 0xAu);
				mem_write16(r27 + 0xAu, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, (int16_t)(r26 + r6) >> 1);
				mem_write16(r10 + 0x10u, r6);
				r6 = (r6 << 16) | (r6 >> 16);
				r26 = mem_read16(r28 + 6u);
				mem_write16(r27 + 6u, r6);
				mem_write16(r10 + 0x3000u, (int16_t)(r26 + r6) >> 1);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			else
			{
				mem_write16(r27, r3_2);
				r3_2 = r3_2 | r1;
				mem_write16(r10 + 0x3010u, r3_2);
				mem_write16(r10 + 0x10u, r3_2);
				mem_write16(r10 + 0x3000u, r3_2);
				mem_write16(r10, r3_2);
				mem_write16(r27 + 2u, r4_2);
				r4_2 |= r2;
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r4_2);
				mem_write16(r10 + 0x10u, r4_2);
				mem_write16(r10 + 0x3000u, r4_2);
				mem_write16(r10, r4_2);
				mem_write16(r27 + 4u, r5);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r5);
				mem_write16(r10 + 0x10u, r5);
				mem_write16(r10 + 0x3000u, r5);
				mem_write16(r10, r5);
				mem_write16(r27 + 0xAu, r6);
				r10 += 2u;
				mem_write16(r10 + 0x3010u, r6);
				mem_write16(r10 + 0x10u, r6);
				r6 = (r6 << 16) | (r6 >> 16);
				mem_write16(r27 + 6u, r6);
				mem_write16(r10 + 0x3000u, r6);
				mem_write16(r10, r6);
				r10 += 2u;
			}
			mem_write32(r10 + 0x3010u, r7);
			mem_write32(r10 + 0x10u, r7);
			mem_write32(r10 + 0x3000u, r7);
			mem_write32(r10, r7);
			r10 += 0x14u;
			r4 = screen + 0x5000u + mem_read16(r19 + 8u) * 4u;
			if (mem_read16(r4) == 0u)
				mem_write16(r4 + 2u, r10);
			r6 = mem_read16(r4);
			mem_write32(r10, r6);
			mem_write32(r10 + 0x3000u, (uint16_t)(r6 + 0x300u));
			mem_write16(r4, ((r10 - 0x20u) >> 4) | 0x8000u);
			SCREEN(screen)->sprite_groups += 2u;
			r10 += 0x10u;
		}
		r18 += r8;
		r19 += r9;
		r18 = (r18 & ~0xFFFFu) | (r18 & 0xAFFFu);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(r27 + 0x10u);
		r28 = (r28 & ~0xFFFFu) | (uint16_t)(r28 + 0x10u);
		r27 = (r27 & ~0xFFFFu) | (r27 & r21);
		r28 = (r28 & ~0xFFFFu) | (r28 & r21);
		r17--;
		if (r17 == 0u)
		{
			SCREEN(screen)->sprite_list_last = r10;
			break;
		}
	}
	r26_2 = rom_100000;
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	mem_write32(r27, r26_2);
	r27 = r27 + 4u;
	r27 = (r27 & ~0xFFFFu) | (r27 & r21);
	SCREEN(screen)->next_sprite_place = r27;
}

/* Starts the race's course (rom_03CC00 by screen unk_A0, car unk_A0): clears the ring of segments, the
   first road width, and builds the segments as far as the view reaches (screen unk_12 >> 7,
   prebuilt_segments); the first stage into the stage history. */
void player_car_start_course(void)
{
	uint16_t r0 = 0, r5 = 0;
	uint32_t r11 = 0;

	move_string(0x100800u, 0x200u, screen + 0x2000u, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
	r11 = rom_03CC00[SCREEN(screen)->unk_A0];
	CAR(player_car)->course = r11;
	r0 = mem_read8(r11 + 8u);
	CAR(player_car)->width_code = (uint8_t)r0;
	CAR(player_car)->road_width = r0 * 0xB0u;
	CAR(player_car)->prebuilt_segments = SCREEN(screen)->unk_12 >> 7;
	r5 = 0xFFFu;
	do
	{
		player_car_build_segment(r5);
	} while ((int16_t)CAR(player_car)->built_segments < (int16_t)CAR(player_car)->prebuilt_segments);
	SCREEN(screen)->unk_A4 = SCREEN(screen)->unk_A2;
	CAR(player_car)->stage_history = SCREEN(screen)->unk_A1;
}

/* Keeps the course built 104 segments past the window's start (window_record, build_end_offset). At a
   fork the building waits (at_fork) until the checkpoint is passed (screen unk_349): then the branch
   taken (screen 349 + the fork) goes on: branch, the course of that branch (car unk_A0), its stage
   (screen unk_A1, stage indicator A5), the stage and branch histories and forks_taken, the stage's data
   queued (queue_stage_load) and the branch_arrow. */
void player_car_extend_course(void)
{
	uint8_t r1_2 = 0;
	uint16_t r1 = 0, r5 = 0, window_offset = 0;
	uint32_t r0 = 0, r2 = 0, r11 = 0, r21 = 0, course = 0, forks_reached = 0, race = 0;

	r5 = 0xFFFu;
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
				player_car_build_segment(r5);
			} while (CAR(player_car)->build_offset != CAR(player_car)->build_end_offset);
		}
		return;
	}
	forks_reached = CAR(player_car)->forks_reached;
	if (SCREEN(screen)->forks_decided < forks_reached)
		return;
	course = CAR(player_car)->course;
	r0 = mem_read8(screen + 0x349u + forks_reached);
	CAR(player_car)->branch = r0;
	r1_2 = ROM8(course + r0);
	CAR(player_car)->course = ROM32(course + 2u + r0 * 4u);
	SCREEN(screen)->unk_A1 = r1_2;
	CAR(player_car)->unk_CA = 0xFFu;
	SCREEN(screen)->unk_A5++;
	r2 = CAR(player_car)->forks_taken;
	mem_write8(player_car + 0xD7u + r2, r0);
	r2 = (uint8_t)(r2 + 1u);
	mem_write8(player_car + 0xD1u + r2, r1_2);
	CAR(player_car)->forks_taken = r2;
	CAR(player_car)->at_fork = 0u;
	queue_stage_load();
	r21 = start_object();
	mem_write32(r21 + 4u, branch_arrow_update_ADDRESS);
	for (;;)
	{
		CAR(player_car)->unk_C2++;
		r11 = CAR(player_car)->course;
		r0 = (int16_t)ROM16(r11);
		r11 += 2u;
		if ((int16_t)r0 > 0)
			break;
		switch (0u - r0)
		{
		case 0:
			OBJECT(current_object)->state = 3u;
			OBJECT(current_object)->player_car.step = 0u;
			OBJECT(current_object)->unk_0E = 0u;
			OBJECT(current_object)->unk_0F = 0u;
			return;
		case 6:
			r21 = start_object();
			mem_write32(r21 + 4u, goal_update_ADDRESS);
			mem_write16(r21 + 0x1Cu, CAR(player_car)->built_segments);
			CAR(player_car)->course = r11;
			continue;
		case 1:
			CAR(player_car)->course = ROM32(r11);
			continue;
		case 2:
			race = SCREEN(screen)->race;
			CAR(player_car)->forks_reached++;
			RACE(race)->stage_count++;
			CAR(player_car)->course = r11;
			CAR(player_car)->at_fork = 0xFFu;
			return;
		case 3:
			CAR(player_car)->pitch_angle = 0u;
			CAR(player_car)->course = r11 + 4u;
			continue;
		case 4:
			SCREEN(screen)->unk_A2 = ROM8(r11);
			r11++;
			SCREEN(screen)->unk_A3 = ROM8(r11);
			CAR(player_car)->stage_segments = 0xFFFFu;
			CAR(player_car)->course = r11 + 1u;
			CAR(player_car)->stage_start = CAR(player_car)->built_segments;
			continue;
		case 5:
			CAR(player_car)->heading_angle = 0u;
			CAR(player_car)->course = r11 + 4u;
			continue;
		default:
			invalid_jump(rom_00FF40[0u - r0]);
		}
	}
	CAR(player_car)->run_left = r0;
	CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(r11) << 8;
	r11++;
	CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(r11) << 8;
	r11++;
	r1 = ROM8(r11);
	r11++;
	CAR(player_car)->unk_CE = ROM8(r11);
	CAR(player_car)->course = r11 + 1u;
	CAR(player_car)->width_code = (uint8_t)r1;
	r1 = (uint16_t)(r1 * 0xB0u);
	CAR(player_car)->road_width_target = r1;
	CAR(player_car)->road_width_step = (int16_t)(r1 - CAR(player_car)->road_width) >> 6;
	player_car_build_segment(r5);
}

/* Builds the next segment of the road from the course (car unk_A0): at the end of a run reads the next
   records: 0 the course ends (the car's state 3), 1 a jump to another part, 2 a fork (forks_reached, the
   race's stage count, at_fork), 3 and 5 the pitch and the heading back to 0, 4 a new stage (screen
   unk_A2, unk_A3; stage_segments, stage_start), 6 the goal (a goal object at this segment), else a run
   (its length, pitch and heading steps, road width, a byte for the segments). The segment: heading and
   pitch turned by the run's steps, the position moved along them (next_x/y/z), the width eased toward
   the run's; written at build_offset, then built_segments and stage_segments count up. */
void player_car_build_segment(uint16_t r5)
{
	uint16_t r1_2 = 0, road_width = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r11 = 0, r21 = 0, race = 0;
	uint16_t run_left = 0;

	for (;;)
	{
		run_left = CAR(player_car)->run_left;
		CAR(player_car)->run_left = run_left - 1u;
		if ((int16_t)run_left >= 1)
			break;
		for (;;)
		{
			CAR(player_car)->unk_C2++;
			r11 = CAR(player_car)->course;
			r0 = (int16_t)ROM16(r11);
			r11 += 2u;
			if ((int16_t)r0 > 0)
				break;
			switch (0u - r0)
			{
			case 0:
				OBJECT(current_object)->state = 3u;
				OBJECT(current_object)->player_car.step = 0u;
				OBJECT(current_object)->unk_0E = 0u;
				OBJECT(current_object)->unk_0F = 0u;
				return;
			case 6:
				r21 = start_object();
				mem_write32(r21 + 4u, goal_update_ADDRESS);
				mem_write16(r21 + 0x1Cu, CAR(player_car)->built_segments);
				CAR(player_car)->course = r11;
				continue;
			case 1:
				CAR(player_car)->course = ROM32(r11);
				continue;
			case 2:
				race = SCREEN(screen)->race;
				CAR(player_car)->forks_reached++;
				RACE(race)->stage_count++;
				CAR(player_car)->course = r11;
				CAR(player_car)->at_fork = 0xFFu;
				return;
			case 3:
				CAR(player_car)->pitch_angle = 0u;
				CAR(player_car)->course = r11 + 4u;
				continue;
			case 4:
				SCREEN(screen)->unk_A2 = ROM8(r11);
				r11++;
				SCREEN(screen)->unk_A3 = ROM8(r11);
				CAR(player_car)->stage_segments = 0xFFFFu;
				CAR(player_car)->course = r11 + 1u;
				CAR(player_car)->stage_start = CAR(player_car)->built_segments;
				continue;
			case 5:
				CAR(player_car)->heading_angle = 0u;
				CAR(player_car)->course = r11 + 4u;
				continue;
			default:
				invalid_jump(rom_00FF40[0u - r0]);
			}
		}
		CAR(player_car)->run_left = r0;
		CAR(player_car)->pitch_step = (uint32_t)(int8_t)ROM8(r11) << 8;
		r11++;
		CAR(player_car)->heading_step = (uint32_t)(int8_t)ROM8(r11) << 8;
		r11++;
		r1_2 = ROM8(r11);
		r11++;
		CAR(player_car)->unk_CE = ROM8(r11);
		CAR(player_car)->course = r11 + 1u;
		CAR(player_car)->width_code = (uint8_t)r1_2;
		r1_2 = (uint16_t)(r1_2 * 0xB0u);
		CAR(player_car)->road_width_target = r1_2;
		CAR(player_car)->road_width_step = (int16_t)(r1_2 - CAR(player_car)->road_width) >> 6;
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
	r2 = (uint16_t)(CAR(player_car)->pitch_angle >> 16u);
	r4 = (uint16_t)(CAR(player_car)->heading_angle >> 16u);
	r1 = (int16_t)rom_100C00[r2];
	r2 = (int16_t)rom_101400[r2];
	r3 = (int16_t)rom_100C00[r4];
	r4 = (int16_t)rom_101400[r4];
	CAR(player_car)->next_x_fine += (int32_t)(r3 * r2) >> 13;
	CAR(player_car)->next_y_fine -= r1 << 1;
	CAR(player_car)->next_z_fine += (int32_t)(r2 * r4) >> 13;
	CAR(player_car)->next_x = CAR(player_car)->next_x_fine;
	CAR(player_car)->next_x = (int32_t)CAR(player_car)->next_x >> 8;
	CAR(player_car)->next_y = CAR(player_car)->next_y_fine;
	CAR(player_car)->next_y = (int32_t)CAR(player_car)->next_y >> 8;
	CAR(player_car)->next_z = CAR(player_car)->next_z_fine;
	CAR(player_car)->next_z = (int32_t)CAR(player_car)->next_z >> 8;
	r11 = ((screen + 0x2000u) & ~0xFFFFu) | (uint16_t)(screen + 0x2000u + CAR(player_car)->build_offset);
	mem_write16(r11, (uint16_t)(CAR(player_car)->heading_angle >> 16u));
	mem_write16(r11 + 2u, (uint16_t)(CAR(player_car)->pitch_angle >> 16u));
	mem_write32(r11 + 4u, CAR(player_car)->next_x);
	mem_write32(r11 + 8u, CAR(player_car)->next_z);
	mem_write32(r11 + 0xCu, CAR(player_car)->next_y);
	mem_write8(r11 + 0x15u, CAR(player_car)->unk_CE);
	road_width = CAR(player_car)->road_width;
	mem_write16(r11 + 0x10u, road_width);
	mem_write16(r11 + 0x12u, road_width >> 1);
	if (CAR(player_car)->width_changing == 0u)
		mem_write8(r11 + 0x14u, CAR(player_car)->width_code);
	else
		mem_write8(r11 + 0x14u, 0u);
	CAR(player_car)->build_offset = (r11 + 0x20u) & r5;
	CAR(player_car)->built_segments++;
	CAR(player_car)->stage_segments++;
}

/* Projects the road for the view of screen unk_D0 (player_car_publish): its rotation (view_matrix_*,
   from the view's pitch and yaw), then each segment from the window's start out to the view's distance
   turned into the view and scaled by its depth into the screen's list at screen + 3000 (scale, width, x,
   y, the segment's byte), segments behind the view or too far left out. */
void player_car_project_road(void)
{
	uint32_t carry = 0;
	uint16_t r5 = 0, r21 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5_2 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r13 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r26 = 0, r29 = 0, car = 0;
	uint8_t t1 = 0;
	uint16_t t4 = 0;
	uint32_t t2 = 0, t3 = 0;

	r1 = mem_read16(SCREEN(screen)->unk_D0 + 0xCu);
	r2 = mem_read16(SCREEN(screen)->unk_D0 + 0xEu);
	r16 = (int16_t)rom_100C00[r1];
	r15 = (int16_t)rom_100C00[r2];
	r13 = (int16_t)rom_101400[r1];
	r10 = (int16_t)rom_101400[r2];
	r8 = 0xFFFFFFF2u;
	r11 = 0u - r15;
	r12 = r16 * r15;
	carry = (r12 >> 13) & 1u;
	r12 = ((int32_t)r12 >> 14) + carry;
	r14 = r16 * r10;
	carry = (r14 >> 13) & 1u;
	r14 = ((int32_t)r14 >> 14) + carry;
	r15 = r15 * r13;
	carry = (r15 >> 13) & 1u;
	r15 = ((int32_t)r15 >> 14) + carry;
	r16 = 0u - r16;
	r17 = r13 * r10;
	carry = (r17 >> 13) & 1u;
	r17 = ((int32_t)r17 >> 14) + carry;
	car = player_car + 0xDCu;
	CAR(car - 0xDCu)->view_matrix_xx = r10;
	car += 4u;
	CAR(car - 0xE0u)->view_matrix_xy = 0;
	car += 4u;
	CAR(car - 0xE4u)->view_matrix_xz = r11;
	car += 4u;
	CAR(car - 0xE8u)->view_matrix_yx = r12;
	car += 4u;
	CAR(car - 0xECu)->view_matrix_yy = r13;
	car += 4u;
	CAR(car - 0xF0u)->view_matrix_yz = r14;
	car += 4u;
	CAR(car - 0xF4u)->view_matrix_zx = r15;
	car += 4u;
	CAR(car - 0xF8u)->view_matrix_zy = r16;
	CAR(car - 0xF8u)->view_matrix_zz = r17;
	r0 = 0u - mem_read32(SCREEN(screen)->unk_D0);
	r1 = 0u - mem_read32(SCREEN(screen)->unk_D0 + 4u);
	r2 = 0u - mem_read32(SCREEN(screen)->unk_D0 + 8u);
	r3 = SCREEN(screen)->unk_10;
	r3 = r3 >> 1;
	r4 = SCREEN(screen)->unk_12;
	r5 = (uint16_t)(0u - SCREEN(screen)->sprite_origin_y);
	r18 = screen + 0x1FE0u + mem_read16(SCREEN(screen)->unk_D0 + 0x10u);
	r19 = screen + 0x2FF0u;
	r9 = r4 >> 7;
	SCREEN(screen)->crest_count = 0u;
	r21 = 0x7FFFu;
	SCREEN(screen)->crest_heights = r5 + 0xE0u;
	r29 = 0x10509u;
	for (;;)
	{
		r18 = ((r18 + 0x20u) & ~0xFFFFu) | ((r18 + 0x20u) & 0xAFFFu);
		t1 = ROM8(r29);
		r29++;
		r19 += 0x10u;
		if (t1 == 0u)
		{
			r26 = r19 - 0x10u;
			t2 = mem_read32(r26 + 4u);
			mem_write32(r19, mem_read32(r26));
			mem_write32(r19 + 4u, t2);
			r26 += 8u;
			t3 = mem_read32(r26 + 4u);
			mem_write32(r19 + 8u, mem_read32(r26));
			mem_write32(r19 + 0xCu, t3);
			mem_write16(r19 + 0xCu, mem_read16(r18 + 0x14u));
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		r26 = r0 + mem_read32(r18 + 4u);
		r27 = r1;
		r27 = r27 + mem_read32(r18 + 0xCu);
		r28 = r2;
		r28 = r28 + mem_read32(r18 + 8u);
		r7 = r28;
		r28 = r28 * r17;
		r28 = r28 + r26 * r15;
		r28 = r28 + r27 * r16;
		r28 = r28 + r8;
		r28 = (int32_t)r28 >> 14;
		r27 = r27 * r13;
		r27 = r27 + r26 * r12;
		r27 = r27 + r7 * r14;
		r27 = r27 + r8;
		r27 = (int32_t)r27 >> 14;
		r26 = (int32_t)(r26 * r10 + r7 * r11 + r8) >> 14;
		if ((int32_t)r4 <= (int32_t)r28 || (int32_t)r28 <= (int32_t)r3)
		{
			mem_write16(r19, 0u);
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				r27 = (r27 & ~0xFFFFu) | r21;
				r28 = (r28 & ~0xFFFFu) | 3u;
				note_road_crest();
				return;
			}
			continue;
		}
		r5_2 = rom_19A340[r28];
		mem_write16(r19, r5_2);
		r26 = r26 * r5_2;
		r27 = r27 * r5_2;
		carry = (r26 >> 11) & 1u;
		r26 = ((int32_t)r26 >> 12) + carry;
		carry = (r27 >> 11) & 1u;
		r27 = (int32_t)r27 >> 12;
		r27 = r27 + carry;
		r5_2 = r5_2 * mem_read16(r18 + 0x10u);
		carry = (r5_2 >> 11) & 1u;
		mem_write16(r19 + 2u, ((int32_t)r5_2 >> 12) + carry);
		mem_write16(r19 + 0xCu, mem_read16(r18 + 0x14u));
		mem_write16(r19 + 4u, r26);
		r27 = (r27 & ~0xFFFFu) | (uint16_t)(0u - r27);
		mem_write16(r19 + 6u, r27);
		r28 = rom_1A0340[r28];
		mem_write16(r19 + 8u, r28);
		t4 = r21;
		r21 = (uint16_t)r27;
		if ((int16_t)t4 < (int16_t)r27)
		{
			note_road_crest();
			r9--;
			if (r9 == 0u)
			{
				mem_write16(r19 + 0x10u, 0u);
				note_road_crest();
				return;
			}
			continue;
		}
		r9--;
		if (r9 == 0u)
			break;
	}
	mem_write16(r19 + 0x10u, 0u);
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
void rotate_offset(uint32_t r0, uint32_t r3, uint32_t r4, uint32_t *p_r1, uint32_t *p_r2)
{
	uint32_t r1 = *p_r1;
	uint32_t r2 = *p_r2;
	uint32_t r3_2 = 0, r4_2 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0;

	r5 = (int16_t)rom_100C00[r4];
	r6 = (int16_t)rom_101400[r4];
	r4_2 = (int16_t)rom_101400[r3];
	r3_2 = (int16_t)rom_100C00[r3];
	r7 = r0;
	r9 = 0u - r1;
	r1 = (int32_t)(r1 * r4_2 + ((int32_t)(r7 * r3_2) >> 14) * r5 + ((int32_t)(r2 * r3_2) >> 14) * r6) >> 14;
	r2 = (int32_t)(((int32_t)(r2 * r4_2) >> 14) * r6 + (r9 * r3_2 + ((int32_t)(r7 * r4_2) >> 14) * r5)) >> 14;
	*p_r1 = r1;
	*p_r2 = r2;
}

/* Sets up the constants of the screen's block at boot: index (0 for the block at 208000, 1 at 218000)
   and by it (rom_010BA0) sprite_screen_bit, palette_ram, mixer and io_chip; unk_10, unk_12 (the second
   of the draw distances at 010BE2), sprite_origin_x, sprite_origin_y and unk_18 from 010BC8; and
   stage_lengths: for each of the 31 roads of rom_03CC00 (the courses player_car_start_course starts by
   screen unk_A0) the sum of its sections' lengths (the first halfword of each 6-byte record; the
   negative ones are commands: -1 goes on at the address it holds, -4 is 4 bytes long, -2 and -6 end it),
   and 14A after them. */
uint32_t init_screen(void)
{
	bool L_013DB6 = false;
	uint16_t r8 = 0;
	uint32_t r0 = 0, r1 = 0, r2 = 0, r9 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0;

	r1 = screen >> 16;
	r1 = r1 & 1u;
	SCREEN(screen)->index = r1;
	r2 = rom_010BA0[r1];
	SCREEN(screen)->sprite_screen_bit = mem_read16(r2);
	r2 += 2u;
	SCREEN(screen)->palette_ram = mem_read32(r2);
	r2 += 4u;
	SCREEN(screen)->mixer = mem_read32(r2);
	SCREEN(screen)->io_chip = mem_read32(r2 + 4u);
	SCREEN(screen)->unk_12 = mem_read16(rom_010BC8 + 2u);
	SCREEN(screen)->unk_10 = rom_010BCC;
	SCREEN(screen)->sprite_origin_x = rom_010BCE;
	SCREEN(screen)->sprite_origin_y = rom_010BD0;
	SCREEN(screen)->unk_18 = rom_010BD2[0];
	r10 = rom_03CC00_ADDRESS;
	r11 = screen + 0x700u;
	r9 = 0x1Fu;
	do
	{
		r12 = ROM32(r10);
		r0 = 0u;
		r8 = 0u;
		r10 += 4u;
		for (;;)
		{
			r8 += r0;
			for (;;)
			{
				r0 = mem_read16(r12);
				r12 = r12 + 6u;
				if ((int16_t)r0 >= 0)
					break;
				t1 = (uint16_t)~r0;
				r0 = rom_013D94[t1];
				switch (t1)
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
					invalid_jump(rom_013D94[t1]);
				}
				if (L_013DB6)
					break;
			}
			if (L_013DB6)
				break;
		}
		L_013DB6 = false;
		mem_write16(r11, r8);
		r9--;
		r11 += 2u;
	} while (r9 != 0u);
	mem_write16(r11, 0x14Au);
	return r0;
}

/* Clears the screen's work RAM at boot: E000 bytes from 8000 below its block (the objects and their
   lists) to 6000 into it. */
void clear_screen_ram(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = screen - 0x8000u;
	r0 = 0x38u;
	r1 = 0x200u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, 0u, &r27, &r28);
		r10 += 0x400u;
		r0--;
	} while (r0 != 0u);
}

/* Sets up video RAM: cleared (clear_video_ram), the text font (load_text_font), the video control
   register 31FF00 (0800; 0A00 flipped, flip_screen) and the registers 31FF02-31FF8F from 010B12, with
   their copy at video_registers, the four tile layers' scroll targets (unk_20E290-20E2AF) cleared, then
   reset_tile_layers. */
uint32_t init_video(void)
{
	clear_video_ram();
	load_text_font();
	mem_write16(VIDEO_31FF00, rom_010B10);
	video_registers = rom_010B10;
	if (flip_screen != 0u)
	{
		mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) | 0x200u);
		video_registers |= 0x200u;
	}
	move_halfwords(unk_20E290_ADDRESS, 0x100800u, 0x10u);
	move_halfwords(VIDEO_RAM + 0x1FF02u, 0x10B12u, 0x47u);
	move_string(0x10B12u, 0x47u, unk_20E202_ADDRESS, 0x47u, 2u, 0u, 0u, 0u, &r27, &r28);
	return reset_tile_layers();
}

/* Puts the screen's two tile layers (NBG0 and NBG2 for screen 1, NBG1 and NBG3 for screen 2) back at
   page 0 and scroll 0: their page selects (unk_20E240, unk_20E248) and scroll targets (unk_20E290,
   unk_20E2A0) cleared, and bits 12-15 of the video control register (31FF00, video_registers) cleared.
   Used as the game modes start. */
uint32_t reset_tile_layers(void)
{
	uint32_t index = 0;
	uint32_t t1 = 0, t2 = 0;

	mem_write16(VIDEO_31FF00, mem_read16(VIDEO_31FF00) & 0xFFFu);
	video_registers &= 0xFFFu;
	index = SCREEN(screen)->index;
	unk_20E240[index] = 0u;
	unk_20E248[index] = 0u;
	t1 = unk_20E290_ADDRESS + index * 8u;
	mem_write32(t1, 0u);
	mem_write32(t1 + 4u, 0u);
	t2 = unk_20E2A0_ADDRESS + index * 8u;
	mem_write32(t2, 0u);
	mem_write32(t2 + 4u, 0u);
	return index;
}

/* Clears video RAM below its registers (300000-31FEFF): the tilemap pages, the text layer and its font,
   the bitmap and the line tables. */
void clear_video_ram(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = 0x300000u;
	r0 = 0x1FFu;
	r1 = 0x80u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, 0u, &r27, &r28);
		r10 += 0x100u;
		r0--;
	} while (r0 != 0u);
}

/* Clears the text layer's first 30 rows (64 cells each, 31F000-31FEFF). */
void clear_text_layer(void)
{
	uint32_t r0 = 0, r1 = 0, r10 = 0;

	r10 = TEXT_CELL(0, 0);
	r0 = 0xFu;
	r1 = 0x80u;
	do
	{
		move_string(0x100800u, r1, r10, r1, 2u, 0u, 0u, 0u, &r27, &r28);
		r10 += 0x100u;
		r0--;
	} while (r0 != 0u);
}

/* Unused: clears the tilemap pages from the one at R9 (its offset in video RAM) down to the first, 400
   bytes apart (clear_tile_page each). */
void clear_tile_pages(uint16_t r9)
{
	uint16_t r7 = 0, r9_2 = 0;
	uint16_t t1 = 0;

	r7 = (uint16_t)r9;
	do
	{
		r9_2 = (uint16_t)r7;
		clear_tile_page(r9_2);
		t1 = r7;
		r7 = (uint16_t)(t1 - 0x400u);
	} while ((int16_t)t1 >= 0x400);
}

/* Only unused code reaches it (clear_tile_pages): clears the tilemap page at R9 (300000 + R9: 200
   halfwords copied from the zeros at 100800). */
void clear_tile_page(uint16_t r9)
{
	move_string(0x100800u, 0x200u, 0x300000u | (uint16_t)r9, 0x200u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Sets up the sprite chip at boot: its control registers 500000-50000C from 010B0A-010B0F (rendering
   every frame), the buffer's flip bits inverted when the screen is flipped (flip_screen), then
   init_sprite_lists. */
void init_sprite_chip(void)
{
	uint16_t r0 = 0;
	uint32_t r18 = 0, r19 = 0;

	mem_write16(SPRITE_CONTROL_500000, rom_010B0A);
	r18 = SPRITE_CONTROL + 0x4u;
	r0 = rom_010B0B;
	r19 = 0x10B0Cu;
	if (flip_screen != 0u)
		r0 = r0 ^ 3u;
	mem_write16(r18, r0);
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	r19++;
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	r19++;
	r18 += 2u;
	mem_write16(r18, ROM8(r19));
	mem_write16(r18 + 2u, ROM8(r19 + 1u));
	init_sprite_lists();
}

/* Sets up the sprite lists: sprite RAM entries 0-4 (0 clips to the whole screen, 1 sets the offset
   sprites can add, x A0 y DE (sprite_origin_x, sprite_origin_y), and jumps to 2; 2 and 3, the jumps to
   screen 1's and screen 2's lists, and 4 an end), an end at the start of the eight list buffers (404000,
   40A000, 414000, 41A000 and their halfway copies 3000 on), each screen's list at its first buffer
   (sprite_list_last 404000 and 414000, sprite_list_buffers 0), its ring of sprite places at 20C000 and
   21C000 (next_sprite_place), sprite_groups 0, and unk_20E022 = 1 (nothing reads it). */
void init_sprite_lists(void)
{
	move_string(0x10BF4u, 0x28u, 0x400000u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	unk_2080C0 = SPRITE_RAM_404000;
	unk_2180C0 = SPRITE_RAM_414000;
	unk_2080B4 = 0x20C000u;
	unk_2180B4 = 0x21C000u;
	mem_write16(SPRITE_RAM_404000, 0xFFFFu);
	mem_write16(SPRITE_RAM_407000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_40D000, 0xFFFFu);
	mem_write16(SPRITE_RAM_414000, 0xFFFFu);
	mem_write16(SPRITE_RAM_417000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41A000, 0xFFFFu);
	mem_write16(SPRITE_RAM_41D000, 0xFFFFu);
	unk_20E022 = 1u;
	sprite_list_buffers = 0u;
	unk_2080B0 = 0u;
	unk_2180B0 = 0u;
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
	uint32_t r0 = 0, r1 = 0, r10 = 0, mixer = 0;

	mixer = SCREEN(screen)->mixer;
	move_halfwords_backward(mixer, 0x10AB6u, 0x28u);
	move_halfwords(screen + 0x160u, 0x10AB6u, 0x28u);
	move_string(0x10AB6u, 0x28u, screen + 0x1B0u, 0x28u, 2u, 0u, 0u, 0u, &r27, &r28);
	r10 = 0x10B06u + SCREEN(screen)->index * 2u;
	r0 = ROM8(r10);
	r1 = ROM8(r10 + 1u);
	mem_write16(mixer + r0 * 2u, 0u);
	mem_write16(mixer + r1 * 2u, 0u);
	mem_write16(screen + 0x160u + r0 * 2u, 0u);
	mem_write16(screen + 0x160u + r1 * 2u, 0u);
	mem_write16(screen + 0x1B0u + r0 * 2u, 0u);
	mem_write16(screen + 0x1B0u + r1 * 2u, 0u);
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
	move_halfwords(SPRITE_RAM + 0x10000u, 0x12297Cu, 0xF0u);
	move_halfwords(SPRITE_RAM + 0x1000u, 0x107020u, 0x1200u);
	move_string(0x10C44u, 0xC0u, SPRITE_RAM + 0x3400u, 0xC0u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: meant to copy R0 & 3F8 bytes from R10 to R11 by jumping into a run of 8-byte moves at 0109B4,
   but the jump (200 less half the count from the run's start) is worked out for 128 moves and the run
   has 64: it copies the count less 200 bytes, nothing for 200, and for less it jumps past the run into
   the data at 010AB5. */
void unrolled_copy(uint16_t r0, uint32_t r10, uint32_t r11)
{
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0, t33 = 0, t34 = 0, t35 = 0, t36 = 0, t37 = 0, t38 = 0, t39 = 0, t40 = 0, t41 = 0, t42 = 0, t43 = 0, t44 = 0, t45 = 0, t46 = 0, t47 = 0, t48 = 0, t49 = 0, t50 = 0, t51 = 0, t52 = 0, t53 = 0, t54 = 0, t55 = 0, t56 = 0, t57 = 0, t58 = 0, t59 = 0, t60 = 0, t61 = 0, t62 = 0, t63 = 0, t64 = 0, t65 = 0, t66 = 0, t67 = 0, t68 = 0, t69 = 0, t70 = 0, t71 = 0, t72 = 0, t73 = 0, t74 = 0, t75 = 0, t76 = 0, t77 = 0, t78 = 0, t79 = 0, t80 = 0, t81 = 0, t82 = 0, t83 = 0, t84 = 0, t85 = 0, t86 = 0, t87 = 0, t88 = 0, t89 = 0, t90 = 0, t91 = 0, t92 = 0, t93 = 0, t94 = 0, t95 = 0, t96 = 0, t97 = 0, t98 = 0, t99 = 0, t100 = 0, t101 = 0, t102 = 0, t103 = 0, t104 = 0, t105 = 0, t106 = 0, t107 = 0, t108 = 0, t109 = 0, t110 = 0, t111 = 0, t112 = 0, t113 = 0, t114 = 0, t115 = 0, t116 = 0, t117 = 0, t118 = 0, t119 = 0, t120 = 0, t121 = 0, t122 = 0, t123 = 0, t124 = 0, t125 = 0, t126 = 0, t127 = 0;

	switch (0u - (((r0 & 0x3F8u) >> 1) - 0x200u))
	{
	case 0x0u:
		break;
	default:
		invalid_jump(0x109B4u + (0u - (((r0 & 0x3F8u) >> 1) - 0x200u)));
	}
	t1 = r11;
	r11 += 8u;
	t2 = mem_read32(r10 + 4u);
	mem_write32(t1, mem_read32(r10));
	mem_write32(t1 + 4u, t2);
	r10 += 8u;
	t3 = r11;
	r11 += 8u;
	t4 = mem_read32(r10 + 4u);
	mem_write32(t3, mem_read32(r10));
	mem_write32(t3 + 4u, t4);
	r10 += 8u;
	t5 = r11;
	r11 += 8u;
	t6 = mem_read32(r10 + 4u);
	mem_write32(t5, mem_read32(r10));
	mem_write32(t5 + 4u, t6);
	r10 += 8u;
	t7 = r11;
	r11 += 8u;
	t8 = mem_read32(r10 + 4u);
	mem_write32(t7, mem_read32(r10));
	mem_write32(t7 + 4u, t8);
	r10 += 8u;
	t9 = r11;
	r11 += 8u;
	t10 = mem_read32(r10 + 4u);
	mem_write32(t9, mem_read32(r10));
	mem_write32(t9 + 4u, t10);
	r10 += 8u;
	t11 = r11;
	r11 += 8u;
	t12 = mem_read32(r10 + 4u);
	mem_write32(t11, mem_read32(r10));
	mem_write32(t11 + 4u, t12);
	r10 += 8u;
	t13 = r11;
	r11 += 8u;
	t14 = mem_read32(r10 + 4u);
	mem_write32(t13, mem_read32(r10));
	mem_write32(t13 + 4u, t14);
	r10 += 8u;
	t15 = r11;
	r11 += 8u;
	t16 = mem_read32(r10 + 4u);
	mem_write32(t15, mem_read32(r10));
	mem_write32(t15 + 4u, t16);
	r10 += 8u;
	t17 = r11;
	r11 += 8u;
	t18 = mem_read32(r10 + 4u);
	mem_write32(t17, mem_read32(r10));
	mem_write32(t17 + 4u, t18);
	r10 += 8u;
	t19 = r11;
	r11 += 8u;
	t20 = mem_read32(r10 + 4u);
	mem_write32(t19, mem_read32(r10));
	mem_write32(t19 + 4u, t20);
	r10 += 8u;
	t21 = r11;
	r11 += 8u;
	t22 = mem_read32(r10 + 4u);
	mem_write32(t21, mem_read32(r10));
	mem_write32(t21 + 4u, t22);
	r10 += 8u;
	t23 = r11;
	r11 += 8u;
	t24 = mem_read32(r10 + 4u);
	mem_write32(t23, mem_read32(r10));
	mem_write32(t23 + 4u, t24);
	r10 += 8u;
	t25 = r11;
	r11 += 8u;
	t26 = mem_read32(r10 + 4u);
	mem_write32(t25, mem_read32(r10));
	mem_write32(t25 + 4u, t26);
	r10 += 8u;
	t27 = r11;
	r11 += 8u;
	t28 = mem_read32(r10 + 4u);
	mem_write32(t27, mem_read32(r10));
	mem_write32(t27 + 4u, t28);
	r10 += 8u;
	t29 = r11;
	r11 += 8u;
	t30 = mem_read32(r10 + 4u);
	mem_write32(t29, mem_read32(r10));
	mem_write32(t29 + 4u, t30);
	r10 += 8u;
	t31 = r11;
	r11 += 8u;
	t32 = mem_read32(r10 + 4u);
	mem_write32(t31, mem_read32(r10));
	mem_write32(t31 + 4u, t32);
	r10 += 8u;
	t33 = r11;
	r11 += 8u;
	t34 = mem_read32(r10 + 4u);
	mem_write32(t33, mem_read32(r10));
	mem_write32(t33 + 4u, t34);
	r10 += 8u;
	t35 = r11;
	r11 += 8u;
	t36 = mem_read32(r10 + 4u);
	mem_write32(t35, mem_read32(r10));
	mem_write32(t35 + 4u, t36);
	r10 += 8u;
	t37 = r11;
	r11 += 8u;
	t38 = mem_read32(r10 + 4u);
	mem_write32(t37, mem_read32(r10));
	mem_write32(t37 + 4u, t38);
	r10 += 8u;
	t39 = r11;
	r11 += 8u;
	t40 = mem_read32(r10 + 4u);
	mem_write32(t39, mem_read32(r10));
	mem_write32(t39 + 4u, t40);
	r10 += 8u;
	t41 = r11;
	r11 += 8u;
	t42 = mem_read32(r10 + 4u);
	mem_write32(t41, mem_read32(r10));
	mem_write32(t41 + 4u, t42);
	r10 += 8u;
	t43 = r11;
	r11 += 8u;
	t44 = mem_read32(r10 + 4u);
	mem_write32(t43, mem_read32(r10));
	mem_write32(t43 + 4u, t44);
	r10 += 8u;
	t45 = r11;
	r11 += 8u;
	t46 = mem_read32(r10 + 4u);
	mem_write32(t45, mem_read32(r10));
	mem_write32(t45 + 4u, t46);
	r10 += 8u;
	t47 = r11;
	r11 += 8u;
	t48 = mem_read32(r10 + 4u);
	mem_write32(t47, mem_read32(r10));
	mem_write32(t47 + 4u, t48);
	r10 += 8u;
	t49 = r11;
	r11 += 8u;
	t50 = mem_read32(r10 + 4u);
	mem_write32(t49, mem_read32(r10));
	mem_write32(t49 + 4u, t50);
	r10 += 8u;
	t51 = r11;
	r11 += 8u;
	t52 = mem_read32(r10 + 4u);
	mem_write32(t51, mem_read32(r10));
	mem_write32(t51 + 4u, t52);
	r10 += 8u;
	t53 = r11;
	r11 += 8u;
	t54 = mem_read32(r10 + 4u);
	mem_write32(t53, mem_read32(r10));
	mem_write32(t53 + 4u, t54);
	r10 += 8u;
	t55 = r11;
	r11 += 8u;
	t56 = mem_read32(r10 + 4u);
	mem_write32(t55, mem_read32(r10));
	mem_write32(t55 + 4u, t56);
	r10 += 8u;
	t57 = r11;
	r11 += 8u;
	t58 = mem_read32(r10 + 4u);
	mem_write32(t57, mem_read32(r10));
	mem_write32(t57 + 4u, t58);
	r10 += 8u;
	t59 = r11;
	r11 += 8u;
	t60 = mem_read32(r10 + 4u);
	mem_write32(t59, mem_read32(r10));
	mem_write32(t59 + 4u, t60);
	r10 += 8u;
	t61 = r11;
	r11 += 8u;
	t62 = mem_read32(r10 + 4u);
	mem_write32(t61, mem_read32(r10));
	mem_write32(t61 + 4u, t62);
	r10 += 8u;
	t63 = r11;
	r11 += 8u;
	t64 = mem_read32(r10 + 4u);
	mem_write32(t63, mem_read32(r10));
	mem_write32(t63 + 4u, t64);
	r10 += 8u;
	t65 = r11;
	r11 += 8u;
	t66 = mem_read32(r10 + 4u);
	mem_write32(t65, mem_read32(r10));
	mem_write32(t65 + 4u, t66);
	r10 += 8u;
	t67 = r11;
	r11 += 8u;
	t68 = mem_read32(r10 + 4u);
	mem_write32(t67, mem_read32(r10));
	mem_write32(t67 + 4u, t68);
	r10 += 8u;
	t69 = r11;
	r11 += 8u;
	t70 = mem_read32(r10 + 4u);
	mem_write32(t69, mem_read32(r10));
	mem_write32(t69 + 4u, t70);
	r10 += 8u;
	t71 = r11;
	r11 += 8u;
	t72 = mem_read32(r10 + 4u);
	mem_write32(t71, mem_read32(r10));
	mem_write32(t71 + 4u, t72);
	r10 += 8u;
	t73 = r11;
	r11 += 8u;
	t74 = mem_read32(r10 + 4u);
	mem_write32(t73, mem_read32(r10));
	mem_write32(t73 + 4u, t74);
	r10 += 8u;
	t75 = r11;
	r11 += 8u;
	t76 = mem_read32(r10 + 4u);
	mem_write32(t75, mem_read32(r10));
	mem_write32(t75 + 4u, t76);
	r10 += 8u;
	t77 = r11;
	r11 += 8u;
	t78 = mem_read32(r10 + 4u);
	mem_write32(t77, mem_read32(r10));
	mem_write32(t77 + 4u, t78);
	r10 += 8u;
	t79 = r11;
	r11 += 8u;
	t80 = mem_read32(r10 + 4u);
	mem_write32(t79, mem_read32(r10));
	mem_write32(t79 + 4u, t80);
	r10 += 8u;
	t81 = r11;
	r11 += 8u;
	t82 = mem_read32(r10 + 4u);
	mem_write32(t81, mem_read32(r10));
	mem_write32(t81 + 4u, t82);
	r10 += 8u;
	t83 = r11;
	r11 += 8u;
	t84 = mem_read32(r10 + 4u);
	mem_write32(t83, mem_read32(r10));
	mem_write32(t83 + 4u, t84);
	r10 += 8u;
	t85 = r11;
	r11 += 8u;
	t86 = mem_read32(r10 + 4u);
	mem_write32(t85, mem_read32(r10));
	mem_write32(t85 + 4u, t86);
	r10 += 8u;
	t87 = r11;
	r11 += 8u;
	t88 = mem_read32(r10 + 4u);
	mem_write32(t87, mem_read32(r10));
	mem_write32(t87 + 4u, t88);
	r10 += 8u;
	t89 = r11;
	r11 += 8u;
	t90 = mem_read32(r10 + 4u);
	mem_write32(t89, mem_read32(r10));
	mem_write32(t89 + 4u, t90);
	r10 += 8u;
	t91 = r11;
	r11 += 8u;
	t92 = mem_read32(r10 + 4u);
	mem_write32(t91, mem_read32(r10));
	mem_write32(t91 + 4u, t92);
	r10 += 8u;
	t93 = r11;
	r11 += 8u;
	t94 = mem_read32(r10 + 4u);
	mem_write32(t93, mem_read32(r10));
	mem_write32(t93 + 4u, t94);
	r10 += 8u;
	t95 = r11;
	r11 += 8u;
	t96 = mem_read32(r10 + 4u);
	mem_write32(t95, mem_read32(r10));
	mem_write32(t95 + 4u, t96);
	r10 += 8u;
	t97 = r11;
	r11 += 8u;
	t98 = mem_read32(r10 + 4u);
	mem_write32(t97, mem_read32(r10));
	mem_write32(t97 + 4u, t98);
	r10 += 8u;
	t99 = r11;
	r11 += 8u;
	t100 = mem_read32(r10 + 4u);
	mem_write32(t99, mem_read32(r10));
	mem_write32(t99 + 4u, t100);
	r10 += 8u;
	t101 = r11;
	r11 += 8u;
	t102 = mem_read32(r10 + 4u);
	mem_write32(t101, mem_read32(r10));
	mem_write32(t101 + 4u, t102);
	r10 += 8u;
	t103 = r11;
	r11 += 8u;
	t104 = mem_read32(r10 + 4u);
	mem_write32(t103, mem_read32(r10));
	mem_write32(t103 + 4u, t104);
	r10 += 8u;
	t105 = r11;
	r11 += 8u;
	t106 = mem_read32(r10 + 4u);
	mem_write32(t105, mem_read32(r10));
	mem_write32(t105 + 4u, t106);
	r10 += 8u;
	t107 = r11;
	r11 += 8u;
	t108 = mem_read32(r10 + 4u);
	mem_write32(t107, mem_read32(r10));
	mem_write32(t107 + 4u, t108);
	r10 += 8u;
	t109 = r11;
	r11 += 8u;
	t110 = mem_read32(r10 + 4u);
	mem_write32(t109, mem_read32(r10));
	mem_write32(t109 + 4u, t110);
	r10 += 8u;
	t111 = r11;
	r11 += 8u;
	t112 = mem_read32(r10 + 4u);
	mem_write32(t111, mem_read32(r10));
	mem_write32(t111 + 4u, t112);
	r10 += 8u;
	t113 = r11;
	r11 += 8u;
	t114 = mem_read32(r10 + 4u);
	mem_write32(t113, mem_read32(r10));
	mem_write32(t113 + 4u, t114);
	r10 += 8u;
	t115 = r11;
	r11 += 8u;
	t116 = mem_read32(r10 + 4u);
	mem_write32(t115, mem_read32(r10));
	mem_write32(t115 + 4u, t116);
	r10 += 8u;
	t117 = r11;
	r11 += 8u;
	t118 = mem_read32(r10 + 4u);
	mem_write32(t117, mem_read32(r10));
	mem_write32(t117 + 4u, t118);
	r10 += 8u;
	t119 = r11;
	r11 += 8u;
	t120 = mem_read32(r10 + 4u);
	mem_write32(t119, mem_read32(r10));
	mem_write32(t119 + 4u, t120);
	r10 += 8u;
	t121 = r11;
	r11 += 8u;
	t122 = mem_read32(r10 + 4u);
	mem_write32(t121, mem_read32(r10));
	mem_write32(t121 + 4u, t122);
	r10 += 8u;
	t123 = r11;
	r11 += 8u;
	t124 = mem_read32(r10 + 4u);
	mem_write32(t123, mem_read32(r10));
	mem_write32(t123 + 4u, t124);
	r10 += 8u;
	t125 = r11;
	r11 += 8u;
	t126 = mem_read32(r10 + 4u);
	mem_write32(t125, mem_read32(r10));
	mem_write32(t125 + 4u, t126);
	r10 += 8u;
	t127 = mem_read32(r10 + 4u);
	mem_write32(r11, mem_read32(r10));
	mem_write32(r11 + 4u, t127);
}

/* Unpacks the packed data at R10 into R11: flag bits read the highest first, 8 to a byte as needed; 1
   copies a byte; 01 takes a halfword (an offset back of its value >> 3, -1 to -8192, and a length of its
   low three bits + 2, or when they are 0 the next byte + 1; a 0 byte ends the data); 00 takes two bits
   more (a length of 2-5) and a byte (an offset back of -1 to -256); a length copies that many bytes from
   that far back in what has been unpacked. */
void unpack(uint32_t r10, uint32_t r11)
{
	uint32_t carry = 0;
	uint8_t r0 = 0, r3_2 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0;
	uint32_t t1 = 0;

	r0 = mem_read8(r10);
	r1 = 9u;
	r10++;
	for (;;)
	{
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			mem_write8(r11, mem_read8(r10));
			r11++;
			r10++;
			continue;
		}
		r2 = 0xFFFFFFFFu;
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			r2 = (r2 & ~0xFFFFu) | mem_read16(r10);
			r3_2 = (uint8_t)r2;
			r2 = (int32_t)r2 >> 3;
			t1 = r3_2 & 7u;
			r3 = t1;
			r10 += 2u;
			if (t1 != 0u)
				r3 += 2u;
			else
			{
				r3 = mem_read8(r10);
				r10++;
				if (r3 == 0u)
					break;
				r3++;
			}
		}
		else
		{
			r3 = 0u;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			r3 = r3 * 2u + carry;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			r3 = r3 * 2u + carry;
			r2 = (r2 & ~0xFFu) | mem_read8(r10);
			r10++;
			r3 += 2u;
		}
		r2 += r11;
		do
		{
			mem_write8(r11, mem_read8(r2));
			r11++;
			r3--;
			r2++;
		} while (r3 != 0u);
	}
}

/* Unpacks a little of the screen's queued tile pages (queue_tile_pages), from the frame loop's wait
   while unpack_state says there is work: with none under way, takes the next request of the ring
   unpack_queue (unpack_read_place; unpack_state 0 when it is empty); else goes on where it stopped, by
   unpack_state's second byte: FF one byte of a match (unpack_copy_source, unpack_copy_count), 1 one step
   of unpack's format (unpack_flags, unpack_flags_left, unpack_source, unpack_destination): a byte, or a
   match's first byte. */
uint32_t unpack_queued_step(uint32_t r2)
{
	bool L_010F01 = false;
	uint32_t carry = 0;
	uint8_t r0 = 0, r3 = 0;
	uint32_t r1 = 0, r2_2 = 0, r10 = 0, r11 = 0, unpack_copy_source = 0, unpack_destination = 0, unpack_read_place = 0;
	uint8_t unpack_copy_count = 0;
	uint32_t t1 = 0;

	do
	{
		if (L_010F01 || (int8_t)(uint8_t)(SCREEN(screen)->unpack_state >> 8u) <= 0)
		{
			if (!L_010F01)
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
						AT8(SCREEN(screen)->unpack_state, 1) = 1u;
						return r2;
					}
					return r2;
				}
			}
			L_010F01 = false;
			unpack_read_place = SCREEN(screen)->unpack_read_place;
			if (unpack_read_place == SCREEN(screen)->unpack_write_place)
			{
				SCREEN(screen)->unpack_state = 0u;
				return r2;
			}
			r11 = screen + 0xC00u + unpack_read_place * 2u * 4u;
			SCREEN(screen)->unpack_read_place = (unpack_read_place + 1u) & 0x3Fu;
			r10 = mem_read32(r11);
			r11 = mem_read32(r11 + 4u);
			r0 = mem_read8(r10);
			r1 = 9u;
			AT8(SCREEN(screen)->unpack_state, 1) = 1u;
			r10++;
		}
		else
		{
			r0 = SCREEN(screen)->unpack_flags;
			r1 = SCREEN(screen)->unpack_flags_left;
			r10 = SCREEN(screen)->unpack_source;
			r11 = SCREEN(screen)->unpack_destination;
		}
		r1--;
		if (r1 == 0u)
		{
			r0 = mem_read8(r10);
			r1 = 8u;
			r10++;
		}
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry == 0u)
		{
			r2_2 = 0xFFFFFFFFu;
			r1--;
			if (r1 == 0u)
			{
				r0 = mem_read8(r10);
				r1 = 8u;
				r10++;
			}
			carry = (uint8_t)(r0 * 2u) < r0;
			r0 = (uint8_t)(r0 * 2u);
			if (carry)
			{
				r2 = (r2_2 & ~0xFFFFu) | mem_read16(r10);
				r3 = (uint8_t)r2;
				r2 = (int32_t)r2 >> 3;
				t1 = r3 & 7u;
				r3 = (uint8_t)t1;
				r10 += 2u;
				if (t1 != 0u)
				{
					r3++;
					r2 += r11;
					mem_write8(r11, mem_read8(r2));
					SCREEN(screen)->unpack_flags = r0;
					SCREEN(screen)->unpack_flags_left = r1;
					r2++;
					SCREEN(screen)->unpack_copy_source = r2;
					SCREEN(screen)->unpack_copy_count = r3;
					SCREEN(screen)->unpack_source = r10;
					SCREEN(screen)->unpack_destination = r11 + 1u;
					AT8(SCREEN(screen)->unpack_state, 1) = 0xFFu;
					return r2;
				}
				r3 = mem_read8(r10);
				r10++;
				if (r3 != 0u)
				{
					r2 += r11;
					mem_write8(r11, mem_read8(r2));
					SCREEN(screen)->unpack_flags = r0;
					SCREEN(screen)->unpack_flags_left = r1;
					r2++;
					SCREEN(screen)->unpack_copy_source = r2;
					SCREEN(screen)->unpack_copy_count = r3;
					SCREEN(screen)->unpack_source = r10;
					SCREEN(screen)->unpack_destination = r11 + 1u;
					AT8(SCREEN(screen)->unpack_state, 1) = 0xFFu;
					return r2;
				}
				L_010F01 = true;
			}
			if (!L_010F01)
			{
				r3 = 0u;
				r1--;
				if (r1 == 0u)
				{
					r0 = mem_read8(r10);
					r1 = 8u;
					r10++;
				}
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r3 = (uint8_t)(r3 * 2u + carry);
				r1--;
				if (r1 == 0u)
				{
					r0 = mem_read8(r10);
					r1 = 8u;
					r10++;
				}
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r3 = (uint8_t)(r3 * 2u + carry);
				r2 = (r2_2 & ~0xFFu) | mem_read8(r10);
				r10++;
				r3++;
				r2 += r11;
				mem_write8(r11, mem_read8(r2));
				SCREEN(screen)->unpack_flags = r0;
				SCREEN(screen)->unpack_flags_left = r1;
				r2++;
				SCREEN(screen)->unpack_copy_source = r2;
				SCREEN(screen)->unpack_copy_count = r3;
				SCREEN(screen)->unpack_source = r10;
				SCREEN(screen)->unpack_destination = r11 + 1u;
				AT8(SCREEN(screen)->unpack_state, 1) = 0xFFu;
				return r2;
			}
		}
	} while (L_010F01);
	mem_write8(r11, mem_read8(r10));
	SCREEN(screen)->unpack_flags = r0;
	SCREEN(screen)->unpack_flags_left = r1;
	SCREEN(screen)->unpack_source = r10 + 1u;
	SCREEN(screen)->unpack_destination = r11 + 1u;
	return r2;
}

/* Unused: unpacks a list of packed blocks at once (unpack): 8-byte entries, the packed data's address
   and where to unpack it, closed by a negative entry (the first is taken without the check).
   unpack_tile_pages does the same for a list of tile pages. */
void unpack_list(uint32_t r10)
{
	uint32_t r10_2 = 0, r11 = 0;
	uint32_t t1 = 0;

	do
	{
		r12 = mem_read32(r10);
		r10 += 4u;
		r11 = mem_read32(r10);
		t1 = r10 + 4u;
		r10_2 = r12;
		r12 = t1;
		unpack(r10_2, r11);
		r10 = r12;
	} while ((int32_t)mem_read32(r10) >= 0);
}

/* Unpacks a list of tilemap pages into video RAM at once (unpack): each 32-bit entry a page number in
   its top byte (the page at 300000 + 400 * page) and the address of its packed data below, closed by a
   negative entry; the list format of queue_tile_pages. */
void unpack_tile_pages(uint32_t r10)
{
	uint32_t r0 = 0, r10_2 = 0, r11 = 0;
	uint32_t t1 = 0;

	while ((int32_t)mem_read32(r10) >= 0)
	{
		r0 = mem_read32(r10);
		r12 = r0;
		r0 &= ~0xFFFFFFu;
		r11 = 0x300000u + (byte_swap32(r0) << 8) * 4u;
		t1 = r10 + 4u;
		r10_2 = r12 ^ r0;
		r12 = t1;
		unpack(r10_2, r11);
		r10 = r12;
	}
}

/* Unused: queues the entries of an unpack_list list (the packed data's address, where to unpack it;
   closed by a negative entry, the first taken unchecked) as they are in the screen's ring unpack_queue
   (unpack_write_place), with unpack_state set to say there is work, for unpack_queued_step.
   queue_tile_pages does the same for a list of tile pages. */
void queue_unpack_list(uint32_t r10)
{
	uint32_t r11 = 0, unpack_write_place = 0;
	uint32_t t1 = 0;

	do
	{
		unpack_write_place = SCREEN(screen)->unpack_write_place;
		r11 = screen + 0xC00u + unpack_write_place * 2u * 4u;
		SCREEN(screen)->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		t1 = mem_read32(r10 + 4u);
		mem_write32(r11, mem_read32(r10));
		mem_write32(r11 + 4u, t1);
		AS8(SCREEN(screen)->unpack_state) = 0xFFu;
		r10 += 8u;
	} while ((int32_t)mem_read32(r10) >= 0);
}

/* Queues the tilemap pages of the list R10 to be unpacked into video RAM: each 32-bit entry a page
   number in its top byte (the page at 300000 + 400 * page) and the address of its packed data below,
   closed by a negative entry; the requests go into the screen's ring of 64 at C00 (unk_B03 the place)
   and unk_B00 says there is work. Used by set_up_background, ranking_background_load and
   race_background_update. */
void queue_tile_pages(uint32_t r10)
{
	uint32_t r0 = 0, r1 = 0, r11 = 0, unpack_write_place = 0;

	while ((int32_t)mem_read32(r10) >= 0)
	{
		unpack_write_place = SCREEN(screen)->unpack_write_place;
		r11 = screen + 0xC00u + unpack_write_place * 2u * 4u;
		SCREEN(screen)->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		r0 = mem_read32(r10);
		r1 = r0;
		r0 &= ~0xFFFFFFu;
		mem_write32(r11, r1 ^ r0);
		mem_write32(r11 + 4u, 0x300000u + (byte_swap32(r0) << 8) * 4u);
		AS8(SCREEN(screen)->unpack_state) = 0xFFu;
		r10 += 4u;
	}
}

/* Sets up the coins: chute_entry_offset 14 (the two chutes shared: the second's coins counted as the
   first's, on its meter) when not on free play and both chutes give the same units
   (first_chute_multiplier = second_chute_multiplier), else 0; the seats' credits back from the
   bookkeeping (701F74-701F7A, bytes); and the low four bits of I/O chip 1's port F, inverted, into
   unk_20E104 (nothing reads it). */
void init_coins(void)
{
	uint8_t t1 = 0;

	t1 = mem_read8(0xC8000Au);
	unk_20E104 = ~t1 & 0xFu;
	chute_entry_offset = 0u;
	if (free_play == 0u && second_chute_multiplier == first_chute_multiplier)
		chute_entry_offset = 0x14u;
	seat1_credits = mem_read8(SOUND_701F74);
	unk_20E0A4 = mem_read8(SOUND_701F76);
	seat2_credits = mem_read8(SOUND_701F78);
	unk_20E0AC = mem_read8(SOUND_701F7A);
}

/* The coins and service credits, in the vertical blank unless on free play (free_play): queues each
   seat's pending coin sound (4E, seat1_coin_sounds, seat2_coin_sounds) and service sound (50,
   seat1_service_sounds, seat2_service_sounds), one each vertical blank; SERVICE newly pressed
   (switches_pressed bits 0-1) gives that seat a credit (add_service_credit), counted in the
   bookkeeping's service credits (701F04); a coin chute let go (switches_released bits 2-3 seat 1's two
   chutes, 4-5 seat 2's) counts its coin (insert_coin, entries at 0114D4) and the credits it gives in the
   bookkeeping (701F02); then drives the coin meters (pulse_coin_meter: each seat's first, and its second
   unless the chutes are shared, chute_entry_offset). */
void update_coins(uint32_t r6)
{
	uint8_t r0 = 0, r1 = 0;
	uint32_t r6_2 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r19 = 0;
	uint32_t saved_screen = 0;
	uint8_t t1 = 0;

	if (free_play != 0u)
		return;
	saved_screen = screen;
	if (seat1_coin_sounds != 0u)
	{
		r0 = 0x4Eu;
		screen = SCREEN_1;
		queue_sound(r0);
		seat1_coin_sounds--;
	}
	if (seat2_coin_sounds != 0u)
	{
		r0 = 0x4Eu;
		screen = SCREEN_2;
		queue_sound(r0);
		seat2_coin_sounds--;
	}
	if (seat1_service_sounds != 0u)
	{
		r0 = 0x50u;
		screen = SCREEN_1;
		queue_sound(r0);
		seat1_service_sounds--;
	}
	if (seat2_service_sounds != 0u)
	{
		r0 = 0x50u;
		screen = SCREEN_2;
		queue_sound(r0);
		seat2_service_sounds--;
	}
	screen = saved_screen;
	r6_2 = (r6 & ~0xFFu) | switches_pressed;
	r9 = 0u;
	if ((r6_2 & 1u) != 0u)
	{
		r10 = seat1_credits_ADDRESS;
		r11 = seat1_service_sounds_ADDRESS;
		r12 = SCREEN_1;
		r9 = add_service_credit(r9, r10, r11);
		r7 = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(r7, (uint16_t)r9);
	}
	r9 = 0u;
	if ((r6_2 & 2u) != 0u)
	{
		r10 = seat2_credits_ADDRESS;
		r11 = seat2_service_sounds_ADDRESS;
		r12 = SCREEN_2;
		r9 = add_service_credit(r9, r10, r11);
		r7 = SOUND_RAM + 0x1F04u;
		bookkeeping_add16(r7, (uint16_t)r9);
	}
	r6_2 = (r6_2 & ~0xFFu) | switches_released;
	t1 = r6_2 & 0x3Cu;
	r6_2 = (r6_2 & ~0xFFu) | t1;
	if (t1 != 0u)
	{
		r9 = 0u;
		r12 = SCREEN_1;
		r19 = 0x114D4u;
		r9 = insert_coin(r6_2, r9, r19);
		r19 = 0x11504u;
		r9 = insert_coin(r6_2, r9, r19);
		r12 = SCREEN_2;
		r19 = 0x11534u;
		r9 = insert_coin(r6_2, r9, r19);
		r19 = 0x11564u;
		r9 = insert_coin(r6_2, r9, r19);
		r7 = SOUND_RAM + 0x1F02u;
		bookkeeping_add16(r7, (uint16_t)r9);
	}
	if (chute_entry_offset == 0u)
	{
		r7 = 0x20E083u;
		r1 = 1u;
		r12 = port_d_outputs_ADDRESS;
		pulse_coin_meter(r1, r7);
	}
	r7 = coin_meters_ADDRESS;
	r1 = 0u;
	r12 = port_d_outputs_ADDRESS;
	pulse_coin_meter(r1, r7);
	if (chute_entry_offset == 0u)
	{
		r7 = 0x20E089u;
		r1 = 1u;
		r12 = unk_20E093_ADDRESS;
		pulse_coin_meter(r1, r7);
	}
	r7 = 0x20E086u;
	r1 = 0u;
	r12 = unk_20E093_ADDRESS;
	pulse_coin_meter(r1, r7);
}

/* Drives coin meter R1 (its bit in the port D outputs at R12, port_d_outputs: bit 0 a seat's first
   chute's meter, bit 1 its second's) from its record R7 in coin_meters (frames on, frames off, pulses
   owed): when idle with a pulse owed, the bit goes on for 6 vertical blanks and off for 6. Then writes
   the credits into the bookkeeping and its backup (write_bookkeeping_credits) with the checksum. */
void pulse_coin_meter(uint8_t r1, uint32_t r7)
{
	uint32_t r0 = 0;
	uint8_t t1 = 0;

	r0 = mem_read16(r7);
	r7 += 2u;
	if (r0 == 0u)
	{
		if (mem_read8(r7) != 0u)
		{
			mem_write8(r7, mem_read8(r7) - 1u);
			mem_write8(r12, mem_read8(r12) | shift_by(1u, (uint8_t)r1, 8));
			mem_write16(r7 - 2u, 0x606u);
		}
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(r0);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	if ((uint8_t)r0 == 0u)
	{
		r7--;
		mem_write8(r7, mem_read8(r7) - 1u);
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(r0);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	t1 = mem_read8(r7 - 2u);
	mem_write8(r7 - 2u, t1 - 1u);
	if (t1 != 1u)
	{
		r12 = SOUND_701F74;
		write_bookkeeping_credits();
		bookkeeping_update_checksum(r0);
		r12 = SOUND_701FF0;
		write_bookkeeping_credits();
		return;
	}
	mem_write8(r12, mem_read8(r12) & rotate_by(0xFEu, (uint8_t)r1, 8));
	r12 = SOUND_701F74;
	write_bookkeeping_credits();
	bookkeeping_update_checksum(r0);
	r12 = SOUND_701FF0;
	write_bookkeeping_credits();
}

/* Writes the seats' credits (the halfwords at seat1_credits, unk_20E0A4, seat2_credits, unk_20E0AC) at
   R12: 701F74 in the bookkeeping or 701FF0 in its backup. */
void write_bookkeeping_credits(void)
{
	mem_write16(r12, AS16(seat1_credits));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(unk_20E0A4));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(seat2_credits));
	r12 = r12 + 2u;
	mem_write16(r12, AS16(unk_20E0AC));
	r12 = r12 + 2u;
}

/* A service credit for the seat whose credits are at R10 (seat1_credits or seat2_credits): one credit
   more, up to 9 (at 9 its coin units are cleared; beyond, nothing); a credit given brings the screen R12
   to the title from the attract mode (show_title_on_credit), asks for the seat's service sound (R11) and
   counts one in R9, which it returns. Not run in the scenarios. */
uint32_t add_service_credit(uint32_t r9, uint32_t r10, uint32_t r11)
{
	uint32_t zero = 0;
	uint8_t t1 = 0;

	mem_write8(r10, mem_read8(r10) + 1u);
	t1 = mem_read8(r10);
	zero = (uint8_t)(t1 - 9u) == 0u;
	if (t1 >= 9u)
	{
		mem_write8(r10 + 2u, 0u);
		mem_write8(r10 + 1u, 0u);
		if (zero == 0u)
		{
			mem_write8(r10, 9u);
			return r9;
		}
	}
	show_title_on_credit();
	mem_write8(r11, 1u);
	r9 = (r9 & ~0xFFu) | (uint8_t)(r9 + 1u);
	return r9;
}

/* A coin from the chute of entry R19 (when its bit is in R6): takes the entry's variant
   (chute_entry_offset: with the chutes shared, the second counts as the first), writes its byte to I/O
   C00074, owes the chute's coin meter a pulse (coin_meters) and counts the coin in the bookkeeping
   (701F06-701F0C, bookkeeping_increment16); with fewer than 9 credits: the screen's coin sound, and the
   chute's coin units (first_chute_multiplier or second_chute_multiplier) go toward a bonus unit (the
   credit record's third byte; one more unit each bonus_adder, when set) and toward a credit (its second
   byte, seat1_coin_units: a credit each coins_per_credit, up to 9, show_title_on_credit). Returns R9
   plus the credits given. */
uint32_t insert_coin(uint32_t r6, uint32_t r9, uint32_t r19)
{
	uint8_t r1 = 0, r2_2 = 0, r3 = 0;
	uint32_t r2 = 0, r7 = 0, r10 = 0, r11 = 0;
	uint32_t t1 = 0, t2 = 0;

	t1 = mem_read32(r19);
	r19 += 4u;
	if ((r6 & shl32(1u, (int)(t1))) == 0u)
		return r9;
	r11 = mem_read32(r19);
	r19 = r19 + 4u + chute_entry_offset;
	r10 = mem_read32(r19);
	mem_write8(IO_C00074, mem_read8(r19 + 0x10u));
	t2 = mem_read32(r19 + 4u);
	mem_write8(t2, mem_read8(t2) + 1u);
	r7 = mem_read32(r19 + 8u);
	bookkeeping_increment16(r7);
	if (mem_read8(r10) >= 9u)
		return r9;
	mem_write8(r11, mem_read8(r11) + 1u);
	r1 = mem_read8(mem_read32(r19 + 0xCu));
	r2 = bonus_adder;
	if (r2 != 0u)
	{
		r3 = (uint8_t)(mem_read8(r10 + 2u) + r1);
		while (r2 <= r3)
		{
			mem_write8(r10 + 1u, mem_read8(r10 + 1u) + 1u);
			r3 -= r2;
		}
		mem_write8(r10 + 2u, r3);
	}
	r2_2 = coins_per_credit;
	r3 = (uint8_t)(mem_read8(r10 + 1u) + r1);
	do
	{
		if (r3 < r2_2)
		{
			mem_write8(r10 + 1u, r3);
			return r9;
		}
		mem_write8(r10, mem_read8(r10) + 1u);
		show_title_on_credit();
		r9 = (r9 & ~0xFFu) | (uint8_t)(r9 + 1u);
		r3 -= r2_2;
	} while (mem_read8(r10) < 9u);
	r3 = 0u;
	mem_write8(r10 + 2u, 0u);
	mem_write8(r10 + 1u, r3);
	return r9;
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
void bookkeeping_increment16(uint32_t r7)
{
	if (mem_read16(r7) != 0xFFFFu)
	{
		mem_write16(r7, mem_read16(r7) + 1u);
		bookkeeping_update_checksum(0u);
		mem_write16(r7 + 0x7Cu, mem_read16(r7 + 0x7Cu) + 1u);
	}
}

/* Unused: adds one to the 8-bit bookkeeping counter at R7 (and its backup's, 7C on) unless it is at FF,
   with the checksum (bookkeeping_update_checksum). */
void bookkeeping_increment8(uint32_t r7)
{
	if (mem_read8(r7) != 0xFFu)
	{
		mem_write8(r7, mem_read8(r7) + 1u);
		bookkeeping_update_checksum(0u);
		mem_write8(r7 + 0x7Cu, mem_read8(r7 + 0x7Cu) + 1u);
	}
}

/* Adds R9 to the 16-bit bookkeeping counter at R7 (the credits from coins, 701F02, and the service
   credits, 701F04: update_coins) and to its backup's (7C on), with the checksum; past FFFF it stays at
   FFFF (and the backup's is left as it was: the code writes R7 twice). */
void bookkeeping_add16(uint32_t r7, uint16_t r9)
{
	uint32_t carry = 0;
	uint16_t t1 = 0;

	t1 = mem_read16(r7);
	carry = (uint16_t)(t1 + r9) < t1;
	mem_write16(r7, t1 + r9);
	if (carry)
	{
		mem_write16(r7, 0xFFFFu);
		bookkeeping_update_checksum(0u);
		mem_write16(r7, 0xFFFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write16(r7 + 0x7Cu, mem_read16(r7 + 0x7Cu) + r9);
}

/* Adds R9 to the 32-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum; past
   FFFFFFFF it stays there (the backup's left as it was): the play time by seat (game_mode_play). */
void bookkeeping_add32(uint32_t r7, uint32_t r9)
{
	uint32_t carry = 0;
	uint32_t t1 = 0;

	t1 = mem_read32(r7);
	carry = t1 + r9 < t1;
	mem_write32(r7, t1 + r9);
	if (carry)
	{
		mem_write32(r7, 0xFFFFFFFFu);
		bookkeeping_update_checksum(0u);
		mem_write32(r7, 0xFFFFFFFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write32(r7 + 0x7Cu, mem_read32(r7 + 0x7Cu) + r9);
}

/* Unused: adds R9 to the 8-bit bookkeeping counter at R7 and to its backup's (7C on), with the checksum;
   past FF it stays at FF (and the backup's is left as it was: the code writes R7 twice), as
   bookkeeping_add16 does. */
void bookkeeping_add8(uint32_t r7, uint8_t r9)
{
	uint32_t carry = 0;
	uint8_t t1 = 0;

	t1 = mem_read8(r7);
	carry = (uint8_t)(t1 + r9) < t1;
	mem_write8(r7, t1 + r9);
	if (carry)
	{
		mem_write8(r7, 0xFFu);
		bookkeeping_update_checksum(0u);
		mem_write8(r7, 0xFFu);
		return;
	}
	bookkeeping_update_checksum(0u);
	mem_write8(r7 + 0x7Cu, mem_read8(r7 + 0x7Cu) + r9);
}

/* Sets the 16-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum: the longest
   and shortest play (701F20, 701F22; game_mode_play). */
void bookkeeping_set16(uint32_t r7, uint16_t r9)
{
	mem_write16(r7, r9);
	bookkeeping_update_checksum(0u);
	mem_write16(r7 + 0x7Cu, r9);
}

/* Unused: sets the 32-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set32(uint32_t r7, uint32_t r9)
{
	mem_write32(r7, r9);
	bookkeeping_update_checksum(0u);
	mem_write32(r7 + 0x7Cu, r9);
}

/* Unused: sets the 8-bit bookkeeping value at R7 and its backup's (7C on) to R9, with the checksum. */
void bookkeeping_set8(uint32_t r7, uint8_t r9)
{
	mem_write8(r7, r9);
	bookkeeping_update_checksum(0u);
	mem_write8(r7 + 0x7Cu, r9);
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
	uint16_t r3 = 0;
	uint32_t r0 = 0, r2 = 0, r3_2 = 0;

	r3 = mem_read16(SOUND_701F00);
	r2 = SOUND_RAM + 0x1F02u;
	r0 = bookkeeping_checksum(0u, r2);
	if ((uint16_t)r0 == r3)
	{
		r2 = SOUND_RAM + 0x1F02u;
		r3_2 = SOUND_RAM + 0x1F7Eu;
		r0 = bookkeeping_copy(r0, r2, r3_2);
		return;
	}
	r2 = SOUND_RAM + 0x1F7Eu;
	r0 = bookkeeping_checksum(r0, r2);
	if ((uint16_t)r0 == r3)
	{
		r3_2 = SOUND_RAM + 0x1F02u;
		r2 = SOUND_RAM + 0x1F7Eu;
		r0 = bookkeeping_copy(r0, r2, r3_2);
		return;
	}
	r0 = bookkeeping_reset(r0);
}

/* Resets the bookkeeping: the defaults at 011458 into the block (701F02), then the block into its backup
   (701F7E), with the checksum. */
uint32_t bookkeeping_reset(uint32_t r0)
{
	uint32_t r2 = 0, r3 = 0;

	r2 = 0x11458u;
	r3 = SOUND_RAM + 0x1F02u;
	r0 = bookkeeping_copy(r0, r2, r3);
	r2 = SOUND_RAM + 0x1F02u;
	r3 = SOUND_RAM + 0x1F7Eu;
	return bookkeeping_copy(r0, r2, r3);
}

/* Copies 3E halfwords from R2 over R3 (the bookkeeping and its backup at 701F7E, either way, or the
   defaults), then bookkeeping_update_checksum. */
uint32_t bookkeeping_copy(uint32_t r0, uint32_t r2, uint32_t r3)
{
	move_string(r2, 0x3Eu, r3, 0x3Eu, 2u, 0u, 0u, 0u, &r27, &r28);
	return bookkeeping_update_checksum(r0);
}

/* Writes the checksum of the bookkeeping (701F02-701F7D in the shared sound RAM) into 701F00, after a
   change to it; returns it. */
uint32_t bookkeeping_update_checksum(uint32_t r0)
{
	uint32_t r2 = 0;

	r2 = SOUND_RAM + 0x1F02u;
	r0 = bookkeeping_checksum(r0, r2);
	mem_write16(SOUND_701F00, r0);
	return r0;
}

/* Unused: checks the bookkeeping block (701F02) against its checksum (701F00) and resets both it and its
   backup (bookkeeping_reset) when they differ; unlike bookkeeping_check it does not try the backup. */
void bookkeeping_check_block(void)
{
	uint32_t r0 = 0, r2 = 0;

	r2 = SOUND_RAM + 0x1F02u;
	r0 = bookkeeping_checksum(0u, r2);
	if (mem_read16(SOUND_701F00) != (uint16_t)r0)
	{
		r0 = bookkeeping_reset(r0);
		return;
	}
}

/* Returns the checksum of the 3E halfwords at R2 (the bookkeeping or its backup): from 1, each halfword
   XORed in, then a shift right by one, with 8810 XORed in when the bit shifted out was 1. */
uint32_t bookkeeping_checksum(uint32_t r0, uint32_t r2)
{
	uint32_t carry = 0;
	uint32_t r0_2 = 0, r1 = 0;

	r0_2 = (r0 & ~0xFFFFu) | 1u;
	r1 = 0x3Eu;
	do
	{
		r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 ^ mem_read16(r2));
		carry = r0_2 & 1u;
		r0_2 = (r0_2 & ~0xFFFFu) | ((uint16_t)r0_2 >> 1);
		r2 += 2u;
		if (carry)
			r0_2 = (r0_2 & ~0xFFFFu) | (uint16_t)(r0_2 ^ 0x8810u);
		r1--;
	} while (r1 != 0u);
	return r0_2;
}

/* Uploads three blocks of colours straight into the screen's palette RAM at boot (upload_palette): 256
   colours at 9E00 (03153C), 16 at 9C00 (031740) and 2560 at 8000 (153904). */
void upload_boot_palettes(void)
{
	uint32_t r19 = 0;

	r19 = 0x3153Cu;
	upload_palette(r19);
	r19 = 0x31740u;
	upload_palette(r19);
	r19 = 0x153904u;
	upload_palette(r19);
}

/* Queues the palette of the stage the screen goes to (rom_031DDC by screen unk_A1) through queue_palette
   (the palette queue at screen + 900). Used by player_car_extend_course and the game's code (002E74). */
void queue_stage_load(void)
{
	uint32_t r19 = 0;

	r19 = rom_031DDC[SCREEN(screen)->unk_A1];
	queue_palette(r19);
}

/* Queues a block of colours (record R19: its place in the palette, its length in bytes, the colours) for
   the screen's palette RAM, copied in the vertical blank (upload_queued_palettes). */
void queue_palette(uint32_t r19)
{
	uint32_t r8 = 0;

	r8 = 0u;
	queue_palette_upload(r8, r19);
}

/* Queues a list of colour blocks (record R19: its length in bytes, then 8-byte entries: the colours'
   address, their place in the palette, how many blocks of 16) for the screen's palette RAM, copied in
   the vertical blank (upload_queued_palettes). */
void queue_palette_list(uint32_t r19)
{
	uint32_t r8 = 0;

	r8 = 0x80000000u;
	queue_palette_upload(r8, r19);
}

/* Adds R19 (bit 31 from R8: a list, not a block) to the screen's queue of palette uploads (the count at
   unk_900, the entries after it, up to 128). */
void queue_palette_upload(uint32_t r8, uint32_t r19)
{
	uint32_t r9 = 0, r18 = 0;

	r18 = screen + 0x900u;
	if ((int32_t)SCREEN(r18 - 0x900u)->unk_900 < 0x80)
	{
		r9 = SCREEN(r18 - 0x900u)->unk_900 + 1u;
		mem_write32(r18 + r9 * 4u, r19 | r8);
		SCREEN(r18 - 0x900u)->unk_900 = r9;
	}
}

/* Copies the queued palette uploads (queue_palette, queue_palette_list) into the screen's palette RAM in
   the vertical blank and empties the queue. */
void upload_queued_palettes(void)
{
	uint32_t r0 = 0, r10 = 0, r19 = 0;
	uint32_t t1 = 0;

	r10 = screen + 0x900u;
	r0 = SCREEN(r10 - 0x900u)->unk_900;
	r10 += 4u;
	for (;;)
	{
		t1 = mem_read32(r10);
		r19 = t1 & 0x7FFFFFFFu;
		r10 += 4u;
		if ((t1 & 0x80000000u) == 0u)
		{
			upload_palette(r19);
			r0--;
			if (r0 == 0u)
				break;
			continue;
		}
		upload_palette_list(r19);
		r0--;
		if (r0 == 0u)
			break;
	}
	SCREEN(screen)->unk_900 = r0;
}

/* Copies a block of colours (place, length in bytes, colours) into the screen's palette RAM (600000 for
   screen 1, 680000 for screen 2). */
void upload_palette(uint32_t r19)
{
	uint32_t r9 = 0, r18 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	r18 = SCREEN(screen)->index;
	r18 = (r18 << 19) | (r18 >> 13) | 0x600000u | mem_read16(r19);
	r19 += 2u;
	r9 = mem_read16(r19);
	r9 = r9 >> 5;
	r19 += 2u;
	do
	{
		t1 = r18;
		r18 += 8u;
		t2 = mem_read32(r19 + 4u);
		mem_write32(t1, mem_read32(r19));
		mem_write32(t1 + 4u, t2);
		r19 += 8u;
		t3 = r18;
		r18 += 8u;
		t4 = mem_read32(r19 + 4u);
		mem_write32(t3, mem_read32(r19));
		mem_write32(t3 + 4u, t4);
		r19 += 8u;
		t5 = r18;
		r18 += 8u;
		t6 = mem_read32(r19 + 4u);
		mem_write32(t5, mem_read32(r19));
		mem_write32(t5 + 4u, t6);
		r19 += 8u;
		t7 = r18;
		r18 += 8u;
		t8 = mem_read32(r19 + 4u);
		mem_write32(t7, mem_read32(r19));
		mem_write32(t7 + 4u, t8);
		r9--;
		r19 += 8u;
	} while (r9 != 0u);
}

/* Copies a list of colour blocks (the list's length in bytes, then the address, place and number of
   32-byte blocks of each) into the screen's palette RAM. */
void upload_palette_list(uint32_t r19)
{
	uint32_t r8 = 0, r9 = 0, r17 = 0, r18 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	r18 = SCREEN(screen)->index;
	r18 = (r18 << 19) | (r18 >> 13) | 0x600000u;
	r9 = mem_read32(r19) >> 3;
	r19 += 4u;
	do
	{
		r17 = mem_read32(r19);
		r19 += 4u;
		r18 = (r18 & ~0xFFFFu) | mem_read16(r19);
		r19 += 2u;
		r8 = mem_read16(r19);
		r19 += 2u;
		do
		{
			t1 = mem_read32(r17 + 4u);
			mem_write32(r18, mem_read32(r17));
			mem_write32(r18 + 4u, t1);
			r18 += 8u;
			t2 = mem_read32(r17 + 0xCu);
			mem_write32(r18, mem_read32(r17 + 8u));
			mem_write32(r18 + 4u, t2);
			r18 += 8u;
			t3 = mem_read32(r17 + 0x14u);
			mem_write32(r18, mem_read32(r17 + 0x10u));
			mem_write32(r18 + 4u, t3);
			r18 += 8u;
			t4 = mem_read32(r17 + 0x1Cu);
			mem_write32(r18, mem_read32(r17 + 0x18u));
			mem_write32(r18 + 4u, t4);
			r8--;
			r18 += 8u;
		} while (r8 != 0u);
		r9--;
	} while (r9 != 0u);
}

/* Steps the screen's fade while unk_94 bit 0 is set (vblank_update_mixer, every other vertical blank): a
   fade asked for by fade_kind (0 to black, 1 from black, 2 to white, 3 from white, 4 to a red tint, 5
   back from it; records at 01173A) sets fade_level, fade_step, fade_end and fade_channels and fade_kind
   6 (under way); then each time the level moves a step (6 bits: 20 is -32, black; 1F is +31, white) and
   goes into the mixer's colour offset 0 in mixer_values (registers 40-44: red, green, blue, those
   fade_channels has; register 3E's high byte C0), which vblank_update_mixer copies to the mixer; unk_94
   bit 0 is cleared when the level reaches fade_end. */
void step_screen_fade(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0;
	uint16_t r9 = 0;
	uint32_t r1 = 0, r19 = 0;

	switch (SCREEN(screen)->fade_kind)
	{
	case 0:
		r19 = 0x1173Au;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 1:
		r19 = 0x1173Eu;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 2:
		r19 = 0x11742u;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 3:
		r19 = 0x11746u;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 4:
		r19 = 0x1174Au;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 5:
		r19 = 0x1174Eu;
		SCREEN(screen)->fade_level = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_step = ROM8(r19);
		r19++;
		SCREEN(screen)->fade_end = ROM8(r19);
		SCREEN(screen)->fade_channels = ROM8(r19 + 1u);
		SCREEN(screen)->fade_kind = 6u;
		break;
	case 6:
		break;
	default:
		invalid_jump(rom_01168C[SCREEN(screen)->fade_kind]);
	}
	r9 = (uint8_t)(SCREEN(screen)->fade_level + SCREEN(screen)->fade_step) & 0x3Fu;
	SCREEN(screen)->fade_level = (uint8_t)r9;
	r19 = screen + 0x19Fu;
	SCREEN(r19 - 0x19Fu)->unk_19F = 0xC0u;
	r0 = SCREEN(screen)->fade_channels;
	r1 = 3u;
	r19++;
	do
	{
		carry = (uint8_t)(r0 * 2u) < r0;
		r0 = (uint8_t)(r0 * 2u);
		if (carry)
		{
			mem_write16(r19, r9);
			r19 += 2u;
		}
		r1--;
	} while (r1 != 0u);
	if (r9 == SCREEN(screen)->fade_end)
		SCREEN(screen)->flags &= ~1u;
}

/* Copies the text layer's characters (3000 bytes, 384 characters of 8x8 pixels) from 104020 into video
   RAM at 318000. */
void load_text_font(void)
{
	move_string(0x104020u, 0x1800u, VIDEO_RAM + 0x18000u, 0x1800u, 2u, 0u, 0u, 0u, &r27, &r28);
}

/* Unused: builds the text layer's first 128 characters (318000, where load_text_font puts its 384) from
   the one-bit font at 103C20 (8 bytes a character, a bit a pixel): the character's pixels in pen 1, and
   a shadow in pen 2 one pixel right and one row down under them. */
void load_shadowed_text_font(void)
{
	uint32_t carry = 0;
	uint8_t r0 = 0;
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r18 = 0, r19 = 0;

	r19 = 0x103C20u;
	r18 = VIDEO_RAM + 0x18000u;
	r5 = 1u;
	r1 = 0x80u;
	do
	{
		r3 = 8u;
		r6 = 0u;
		do
		{
			r0 = ROM8(r19);
			r2 = 4u;
			r4 = 0u;
			r19++;
			do
			{
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r4 = rotate_with_carry_by(r4, 4u, carry, 32);
				carry = (uint8_t)(r0 * 2u) < r0;
				r0 = (uint8_t)(r0 * 2u);
				r4 = rotate_with_carry_by(r4, 4u, carry, 32);
				r2--;
			} while (r2 != 0u);
			r4 = (r4 << 29) | (r4 >> 3);
			r7 = r4;
			r0_2 = r4 * 0xFu;
			r4 = r4 * r5;
			mem_write32(r18, byte_swap32((r6 & ~r0_2) | r4));
			r6 = (r4 + r7) >> 4;
			r18 += 4u;
			r3--;
		} while (r3 != 0u);
		r1--;
	} while (r1 != 0u);
}

/* Writes the strings of the list R19 on the text layer (print_text_layer each) until an FFFF halfword.
   Not run in the scenarios. */
void print_text_layer_list(uint32_t r19)
{
	do
	{
		r19 = print_text_layer(r19);
	} while (mem_read16(r19) != 0xFFFFu);
}

/* Writes a string on the text layer: from the record R19, the cell's address (310000 + its first
   halfword), an attribute byte (the cells' high byte: palette) and the characters, closed by 0; returns
   the address after it. */
uint32_t print_text_layer(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r19 += 2u;
	r9 = (uint16_t)((uint32_t)mem_read8(r19) << 8);
	r19++;
	r9 |= mem_read8(r19);
	r19++;
	if ((uint8_t)r9 != 0u)
	{
		do
		{
			mem_write16(r18, r9);
			r9 = (uint16_t)((r9 & ~0xFFu) | mem_read8(r19));
			r18 += 2u;
			r19++;
		} while ((uint8_t)r9 != 0u);
	}
	return r19;
}

/* Unused: erases a string from the text layer: from the cell of the print_text_layer record R19, writes
   0 cells until the next cell's character is 0. */
void erase_text_layer_to_blank_character(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r9 = 0u;
	do
	{
		mem_write16(r18, r9);
		r18 += 2u;
	} while (mem_read8(r18) != 0u);
}

/* Unused: print_text_layer with the characters as halfwords: from the record R19, the cell's address
   (310000 + its first halfword), the attribute byte, then the characters, 16 bits each (ORed with the
   attribute << 8, so they can reach past 255: the font has 384), closed by a 0 halfword. */
void print_text_layer_halfwords(uint32_t r19)
{
	uint16_t r8 = 0, r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r19 += 2u;
	r9 = (uint16_t)((uint32_t)mem_read8(r19) << 8);
	r19++;
	r8 = mem_read16(r19);
	r19 += 2u;
	if (r8 != 0u)
	{
		do
		{
			mem_write16(r18, r8 | r9);
			r8 = mem_read16(r19);
			r18 += 2u;
			r19 += 2u;
		} while (r8 != 0u);
	}
}

/* Unused: erases a string from the text layer as erase_text_layer_to_blank_character does, but until the
   next cell is all 0 (character and attribute). */
void erase_text_layer_to_blank_cell(uint32_t r19)
{
	uint16_t r9 = 0;
	uint32_t r18 = 0;

	r18 = 0x310000u | mem_read16(r19);
	r9 = 0u;
	do
	{
		mem_write16(r18, r9);
		r18 += 2u;
	} while (mem_read16(r18) != 0u);
}

/* Never runs in the scenarios (ISDN only): writes a number's low byte as two hex digits on the text
   layer, ending before the cell it is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_byte(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 = (uint8_t)r0;
	r2 = 2u;
	print_hex_digits(r0, r1, r2, r11);
}

/* Unused: writes a number's low halfword as four hex digits on the text layer, ending before the cell it
   is given, with the attribute byte it is given (print_hex_digits). */
void print_hex_halfword(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 = (uint16_t)r0;
	r2 = 4u;
	print_hex_digits(r0, r1, r2, r11);
}

/* Unused: writes a number's low 24 bits as six hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits). */
void print_hex_24_bits(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r0 &= 0xFFFFFFu;
	r2 = 6u;
	print_hex_digits(r0, r1, r2, r11);
}

/* Unused: writes a 32-bit number as eight hex digits on the text layer, as print_hex_halfword does
   (print_hex_digits, which it runs on into). */
void print_hex_word(uint32_t r0, uint16_t r1, uint32_t r11)
{
	uint32_t r2 = 0;

	r2 = 8u;
	print_hex_digits(r0, r1, r2, r11);
}

/* Writes a number in hex on the text layer, in as many cells as it is told, ending before the cell it is
   given, the lowest digit last: each digit a character ('0'-'9', 'A'-'F') with the attribute byte it is
   given; when the number runs out, the cells left before it are cleared (0). Only print_hex_byte runs
   it; the entries for four, six and eight digits before it (print_hex_halfword, print_hex_24_bits,
   print_hex_word) are unused. */
void print_hex_digits(uint32_t r0, uint16_t r1, uint32_t r2, uint32_t r11)
{
	uint32_t t1 = 0;

	r11 += r2 * 2u;
	do
	{
		r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)r0);
		r1 = (uint16_t)((r1 & ~0xFFu) | (r1 & 0xFu));
		if ((uint8_t)r1 >= 0xAu)
			r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)(r1 + 7u));
		r1 = (uint16_t)((r1 & ~0xFFu) | (uint8_t)(r1 + 0x30u));
		r11 -= 2u;
		mem_write16(r11, r1);
		t1 = r0 >> 4;
		r0 = t1;
		r2--;
	} while ((r2 != 0u) & (t1 != 0u));
	if (r2 != 0u)
	{
		do
		{
			r11 -= 2u;
			mem_write16(r11, r0);
			r2--;
		} while (r2 != 0u);
	}
}

/* Unused: converts R0's eight decimal digits (four bits each, the highest first) into a number, returned
   in R0: eight times, the result so far times 10 plus the next digit. The inverse of binary_to_bcd. */
uint32_t bcd_to_binary(uint32_t value)
{
	uint32_t r0 = 0, r2 = 0, r3 = 0;

	r2 = 8u;
	r3 = 0u;
	do
	{
		value = (value << 4) | (value >> 28);
		r3 = r3 * 0xAu + (0xFu & value);
		r2--;
	} while (r2 != 0u);
	r0 = r3;
	return r0;
}

/* Converts R0 into eight decimal digits, one per four bits, the units in the lowest. */
uint32_t binary_to_bcd(uint32_t r0)
{
	uint32_t r0_2 = 0, r1 = 0, r2 = 0, r3 = 0;

	r3 = 8u;
	r2 = 0u;
	do
	{
		r1 = 0u;
		divux_pair(0xAu, &r0, &r1);
		r2 += r1;
		r2 = (r2 << 28) | (r2 >> 4);
		r3--;
	} while ((r3 != 0u) & (r0 != 0u));
	if (r0 == 0u && r3 != 0u)
	{
		do
		{
			r2 = (r2 << 28) | (r2 >> 4);
			r3--;
		} while (r3 != 0u);
	}
	r0_2 = r2;
	return r0_2;
}

/* Draws the string at R18 as draw_text does (a sprite per character into the screen's sprite list), at
   the place R3 holds (x in its low half, y in its high half, as a text record's first 32 bits) with the
   attribute R5. Used by stage_result_line, stage_indicator_draw and song_title_show (the song title). */
void draw_text_at(uint32_t r3, uint16_t r5, uint32_t r18)
{
	uint16_t r4 = 0;
	uint32_t r1 = 0, r2 = 0, r10 = 0, r11 = 0;

	r10 = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= r10)
		return;
	r11 = r10 + 0x3000u;
	r1 = rom_122B5C;
	r3 = (r3 << 16) | (r3 >> 16);
	r2 = rom_122B60 | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	r4 = rom_122B64;
	r5 |= rom_122B66;
	r18 = draw_text_sprites(r1, r2, r3, r4, r5, r10, r11, r18);
}

/* Draws the text record R19 (x and y, an attribute, a string closed by 0) as one sprite per character
   and returns the address of the record after it. */
uint32_t draw_text(uint32_t r19)
{
	uint16_t r4 = 0, r5 = 0;
	uint32_t r1 = 0, r2 = 0, r3 = 0, r10 = 0, r11 = 0, r18 = 0;

	r18 = r19 + 6u;
	r10 = SCREEN(screen)->sprite_list_last + 0x10u;
	if (SCREEN(screen)->sprite_list_limit <= r10)
		return r18;
	r11 = r10 + 0x3000u;
	r1 = rom_122B5C;
	r2 = rom_122B60 | ((uint32_t)SCREEN(screen)->sprite_screen_bit << 16);
	r3 = mem_read32(r19);
	r3 = (r3 << 16) | (r3 >> 16);
	r4 = rom_122B64;
	r5 = rom_122B66 | mem_read16(r19 + 4u);
	return draw_text_sprites(r1, r2, r3, r4, r5, r10, r11, r18);
}

/* Writes a sprite per character of the string at R18 (closed by 0) into the sprite list and its halfway
   copy alike: words 0-3 from R1 and R2 (an 8x8 piece, with the screen's bit), the place R3 (y, x; x 8
   more each character), the graphics R4 + 8 * the character, the attribute R5; then links them into
   depth layer 19E (screen 5678, drawn after nearly all the others) and counts them (sprite_groups).
   Returns the address after the string. The end of draw_text and draw_text_at. */
uint32_t draw_text_sprites(uint32_t r1, uint32_t r2, uint32_t r3, uint16_t r4, uint16_t r5, uint32_t r10, uint32_t r11, uint32_t r18)
{
	uint16_t r0 = 0;
	uint32_t r1_2 = 0, r2_2 = 0, r3_2 = 0, r6 = 0;
	uint8_t t1 = 0;

	r0 = mem_read8(r18);
	r6 = 0x80000u;
	r18++;
	do
	{
		r0 = (uint16_t)(((uint32_t)r0 << 3) + r4);
		mem_write32(r10, r1);
		mem_write32(r11, r1);
		r11 += 4u;
		r10 += 4u;
		mem_write32(r10, r2);
		mem_write32(r11, r2);
		r11 += 4u;
		r10 += 4u;
		mem_write32(r10, r3);
		mem_write32(r11, r3);
		r11 += 4u;
		r10 += 4u;
		mem_write16(r10, r0);
		mem_write16(r11, r0);
		r11 += 2u;
		r10 += 2u;
		mem_write16(r10, r5);
		mem_write16(r11, r5);
		r11 += 2u;
		r3 += r6;
		t1 = mem_read8(r18);
		r18++;
		r0 = t1;
		r10 += 2u;
	} while (r0 != 0u);
	r1_2 = screen + 0x5678u;
	r3_2 = SCREEN(r1_2 - 0x5678u)->unk_5678;
	if (r3_2 == 0u)
		SCREEN(r1_2 - 0x5678u)->unk_567A = r10;
	r2_2 = SCREEN(screen)->sprite_list_last + 0x10u;
	mem_write32(r10, r3_2);
	mem_write32(r11, (uint16_t)(r3_2 + 0x300u));
	SCREEN(screen)->sprite_list_last = r10;
	SCREEN(r1_2 - 0x5678u)->unk_5678 = (r2_2 >> 4) | 0x8000u;
	SCREEN(screen)->sprite_groups++;
	return r18;
}

/* Ends all of the screen's objects at once: every slot free (screen - 7FF0), both object lists empty
   (this frame's at - 7E00, the next one's at - 7D00, with run_objects' pointers to them at - 8000, -
   7FFC, - 7FF4 and - 7FF8), and the queue of palette uploads (unk_900) emptied. At boot and as each game
   mode starts. */
void clear_objects(void)
{
	uint32_t r0 = 0;

	move_halfwords(screen - 0x7FF0u, 0x100800u, 0x7Cu);
	mem_write32(screen - 0x8000u, screen - 0x7E00u);
	mem_write32(screen - 0x7FFCu, screen - 0x7DFEu);
	mem_write32(screen - 0x7FF4u, screen - 0x7DFEu);
	mem_write32(screen - 0x7FF8u, screen - 0x7D00u);
	r0 = mem_read8(screen - 0x7E00u);
	r0 = (uint16_t)(r0 + 1u);
	move_halfwords(screen - 0x7E00u, 0x100800u, r0);
	r0 = mem_read8(screen - 0x7D00u);
	r0 = (uint16_t)(r0 + 1u);
	move_string(0x100800u, r0, screen - 0x7D00u, r0, 2u, 0u, 0u, 0u, &r27, &r28);
	AS16(SCREEN(screen)->unk_900) = 0u;
}

/* The object loop, once a frame for the screen: each live object (flags bit 0) of this frame's list
   (screen - 8000) has its handler run and goes on the next frame's list (screen - 7FF8); a dead one
   frees its slot (its byte at screen - 7FF0, by its slot number at 8). Then this frame's list is cleared
   and the two lists swap. */
void run_objects(void)
{
	uint32_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r22 = 0;
	uint32_t t1 = 0;

	current_object = screen;
	r22 = mem_read32(screen - 0x8000u) + 2u;
	mem_write16(mem_read32(screen - 0x7FF8u), 0u);
	for (;;)
	{
		current_object = (current_object & ~0xFFFFu) | mem_read16(r22);
		r22 += 2u;
		if ((uint16_t)current_object == 0u)
			break;
		if ((OBJECT(current_object)->flags & 1u) != 0u)
		{
			call_code(OBJECT(current_object)->handler);
			t1 = mem_read32(screen - 0x7FF8u);
			mem_write16(t1, mem_read16(t1) + 1u);
			mem_write16(mem_read32(screen - 0x7FF8u) + mem_read16(mem_read32(screen - 0x7FF8u)) * 2u, current_object);
			continue;
		}
		mem_write8(screen - 0x7FF0u + (uint8_t)OBJECT(current_object)->unk_08, 0u);
	}
	r1 = mem_read32(screen - 0x8000u);
	r0 = mem_read16(r1);
	r0 = (uint16_t)(r0 + 4u);
	r0 = r0 >> 2;
	r2 = 0u;
	r3 = 0u;
	do
	{
		mem_write32(r1, r2);
		mem_write32(r1 + 4u, r3);
		r0--;
		r1 += 8u;
	} while (r0 != 0u);
	mem_write16(screen - 0x8000u, mem_read16(screen - 0x8000u) ^ 0x100u);
	mem_write16(screen - 0x7FF8u, mem_read16(screen - 0x7FF8u) ^ 0x100u);
	r0 = mem_read16(mem_read32(screen - 0x8000u));
	mem_write32(screen - 0x7FFCu, mem_read32(screen - 0x8000u) + 2u);
	mem_write32(screen - 0x7FF4u, mem_read32(screen - 0x7FFCu) + r0 * 2u);
}

/* Takes the first free object slot of the screen (FF into its byte at screen - 7FF0), clears its 100
   bytes, makes it live (flags bit 0) with its slot number at 8, puts it on this frame's list and returns
   it; with no slot free, it returns the spare object at screen - 7F00 without any of that. */
uint32_t start_object(void)
{
	uint32_t zero = 0;
	uint8_t r9 = 0;
	uint32_t r21 = 0, r26 = 0;
	uint32_t t1 = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
	{
		r21 = screen - 0x7F00u;
		return r21;
	}
	mem_write8(r28, 0xFFu);
	r9 = (uint8_t)r27;
	r27 = r27 << 8;
	r21 = screen - 0x7C00u + r27;
	r27 = 0u;
	r28 = 0u;
	r26 = r21;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	r26 += 8u;
	mem_write32(r26, 0u);
	mem_write32(r26 + 4u, 0u);
	mem_write8(r21, mem_read8(r21) | 1u);
	mem_write8(r21 + 8u, r9);
	t1 = mem_read32(screen - 0x8000u);
	mem_write16(t1, mem_read16(t1) + 1u);
	mem_write16(mem_read32(screen - 0x8000u) + mem_read16(mem_read32(screen - 0x8000u)) * 2u, r21);
	return r21;
}

/* Unused: takes the first free object slot as start_object does (FF into its byte of the 7C at screen -
   7FF0) and clears 100 bytes for it, but at the slot byte's address with its low half shifted 8 left
   (201000 + 100 * slot for screen 1), not where start_object puts slot n (screen - 7C00 + 100 * n), and
   without making it live, its slot number or the frame's list; with no slot free, the spare object at
   screen - 7F00. Returns the object. */
uint32_t reserve_object_slot(void)
{
	uint32_t zero = 0;
	uint32_t r21 = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &r27, &r28);
	zero = r27 == 0x7Cu;
	if (zero)
	{
		r21 = screen - 0x7F00u;
		return r21;
	}
	mem_write8(r28, 0xFFu);
	r21 = (r28 & ~0xFFFFu) | (uint16_t)(r28 << 8);
	move_string(0x100800u, 0x80u, r21, 0x80u, 2u, 0u, 0u, 0u, &r27, &r28);
	return r21;
}

/* Unused: frees the slot of the object R21: clears the slot byte (screen - 7FF0) numbered by bits 8-15
   of the object's address, which is not the object's own slot: for start_object's slot n it clears slot
   n + 4, for reserve_object_slot's slot n slot n + 10. */
void free_object_slot(uint16_t r21)
{
	mem_write8(screen - 0x7FF0u + ((uint16_t)r21 >> 8), 0u);
}

/* Starts an object for each handler of the list R10 (a 32-bit count, then the handlers: the
   object_handlers_* tables), each with its place in the list at 9, and returns the last one. Used by the
   game modes (game_mode_car_presentation, game_mode_ranking, game_mode_play, initialise_board...),
   name_entry and ranking. */
uint32_t start_object_list(uint32_t r10)
{
	uint8_t r7 = 0;
	uint32_t r8 = 0, r21 = 0;

	r8 = mem_read32(r10);
	r7 = 0u;
	r10 += 4u;
	do
	{
		r21 = start_object();
		mem_write32(r21 + 4u, mem_read32(r10));
		mem_write8(r21 + 9u, r7);
		r7++;
		r8--;
		r10 += 4u;
	} while (r8 != 0u);
	return r21;
}

/* Returns the next pseudo-random number of the screen's sequence (kept in unk_F00). */
uint32_t random_number(void)
{
	uint32_t r0 = 0, r1 = 0;

	r1 = SCREEN(screen)->unk_F00;
	if ((uint16_t)r1 == 0u)
		r1 ^= 0x2A6D365Bu;
	r0 = r1;
	r1 = (r1 * 2u * 2u + r0) * 2u * 2u * 2u + r0;
	r0 = (r0 & ~0xFFFFu) | (uint16_t)r1;
	r1 = (r1 << 16) | (r1 >> 16);
	r0 = (r0 & ~0xFFFFu) | (uint16_t)(r0 + r1);
	r1 = (r1 & ~0xFFFFu) | (uint16_t)r0;
	SCREEN(screen)->unk_F00 = (r1 << 16) | (r1 >> 16);
	return r0;
}

/* Starts the sound at boot: the screen's ring of sound requests with one request of sound 0 waiting (800
   the place read, 801 the place written, the requests from 802), I/O chip 0's CNT 6 (its screen on, the
   sound CPU out of reset), screen byte 822 cleared, and 701FFF cleared, which lets the sound program's
   main loop run. */
void start_sound_program(void)
{
	AS32(SCREEN(screen)->sound_queue_read) = 0x100u;
	mem_write8(IO_C0001C, 6u);
	SCREEN(screen)->unk_822 = 0u;
	mem_write8(SOUND_701FFF, 0u);
}

/* Requests sound R0 for the screen (queue_sound), except while the screen's unk_90 bit 8 is set, when
   only the sounds the table at 01202E allows play, and only with advertise_sound set. */
void request_sound(uint8_t r0)
{
	uint32_t r2 = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		queue_sound(r0);
		return;
	}
	if (advertise_sound != 0u)
	{
		r2 = (uint8_t)r0;
		if ((rom_01202E[r2 >> 5] & shl32(1u, (int)(r2 & 0x1Fu))) != 0u)
		{
			queue_sound(r0);
			return;
		}
	}
}

/* Puts sound R0 in the screen's ring of 32 sound requests (801 of the screen: the place, then the
   requests). */
void queue_sound(uint8_t r0)
{
	uint32_t r2 = 0, sound_queue_write = 0;

	r2 = screen + 0x801u;
	sound_queue_write = SCREEN(r2 - 0x801u)->sound_queue_write;
	r2++;
	mem_write8(r2 + sound_queue_write, r0);
	SCREEN(r2 - 0x802u)->sound_queue_write = (sound_queue_write + 1u) & 0x1Fu;
}

/* Unused: puts sound R0 in the screen's second ring of 32 sound requests (second_sound_queue_write, the
   requests from 842; send_sound_requests sends them to the command byte 8 after the first ring's), as
   request_sound does for the first: while demo_flags bit 8 is set only the sounds the table at 01204E
   allows, and only with advertise_sound set. */
void request_second_sound(uint8_t r0)
{
	uint32_t r2 = 0, second_sound_queue_write = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		r2 = screen + 0x841u;
		second_sound_queue_write = SCREEN(r2 - 0x841u)->second_sound_queue_write;
		r2++;
		mem_write8(r2 + second_sound_queue_write, r0);
		SCREEN(r2 - 0x842u)->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
	}
	else if (advertise_sound != 0u)
	{
		r2 = (uint8_t)r0;
		if ((rom_01204E[r2 >> 5] & shl32(1u, (int)(r2 & 0x1Fu))) != 0u)
		{
			r2 = screen + 0x841u;
			second_sound_queue_write = SCREEN(r2 - 0x841u)->second_sound_queue_write;
			r2++;
			mem_write8(r2 + second_sound_queue_write, r0);
			SCREEN(r2 - 0x842u)->second_sound_queue_write = (second_sound_queue_write + 1u) & 0x1Fu;
			return;
		}
	}
}

/* Starts the car's engine sound on the screen's channel of the sound board: the model's engine sample
   (rom_012026) and the engine's loudness by the transmission (10, or 30, 6 dB louder, with the
   transmission's value set), then the revs on (1) and the requests for the sample and the pitch (A0);
   silent in the demo without sound. */
void player_car_start_engine_sound(void)
{
	uint8_t r0 = 0;
	uint32_t r10 = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u && advertise_sound == 0u)
	{
		r10 = (uint32_t)SCREEN(screen)->index << 8;
		mem_write8(r10 + SOUND_ENGINE_REVS_1, 0u);
		mem_write8(r10 + SOUND_ENGINE_REQUESTS_1, mem_read8(r10 + SOUND_ENGINE_REQUESTS_1) | 0x20u);
		return;
	}
	r10 = (uint32_t)SCREEN(screen)->index << 8;
	mem_write8(r10 + SOUND_ENGINE_SAMPLE_1, rom_012026[CAR(player_car)->model]);
	r0 = 0x10u;
	if (CAR(player_car)->transmission != 0u)
		r0 = 0x30u;
	mem_write8(r10 + SOUND_ENGINE_LOUDNESS_1, r0);
	mem_write8(r10 + SOUND_ENGINE_REVS_1, 1u);
	mem_write8(r10 + SOUND_ENGINE_REQUESTS_1, 0xA0u);
}
