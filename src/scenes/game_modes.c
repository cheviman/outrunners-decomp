/**
 *    Name:        scenes/game_modes.c
 *    Description: the game modes: the title, the power on, the car presentation, the demo race, the ranking and the game itself
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Game mode 2, the title (title_screen_update), by mode_step: 0 the screen cleared (as clear_screen),
   the title's palette (title_palette), title_screen_update started, a fade in (fade 1), the input ring's
   places to 0 (input_write_place, input_read_place), the screen on (C0001C = 6, C8001C = 86 for screen
   2); 1 when the fade is over, mode_timer 450 frames (15 seconds); 2 counts them down, then a fade out
   (fade 0); 3 when it is over, game mode 3 (the car presentation). attract_lamps_update every frame
   first. */
void game_mode_title(void)
{
	uint32_t colour_block = 0;
	struct object *object = NULL;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (screen->mode_step)
	{
	case 0:
		clear_objects();
		reset_tile_layers();
		clear_sprite_group_count();
		clear_text_layer();
		colour_block = title_palette;
		queue_palette(colour_block);
		object = start_object();
		object->handler = title_screen_update_ADDRESS;
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		screen->input_write_place = 0u;
		screen->input_read_place = 0u;
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x86u);
		else
			io_write8(IO_1_CNT, 6u);
		screen->mode_step++;
		return;
	case 1:
		if ((screen->flags & 1u) == 0u)
		{
			screen->mode_timer = 0x1C2u;
			screen->mode_step++;
		}
		return;
	case 2:
		mode_timer = screen->mode_timer;
		screen->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
		{
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			screen->mode_step++;
		}
		return;
	case 3:
		if ((screen->flags & 1u) == 0u)
			AS32(screen->game_mode) = 3u;
		return;
	}
}

/* Game mode 0 (the first frame after start-up): game mode 2 and its first step (game_mode_title). */
void game_mode_power_on(void)
{
	AS32(screen->game_mode) = 2u;
	game_mode_title();
}

/* Game mode 1 (after the ranking, game_mode_ranking, and after the test mode): game mode 2 and its first
   step (game_mode_title). */
void game_mode_restart(void)
{
	AS32(screen->game_mode) = 2u;
	game_mode_title();
}

/* Game mode 3, the presentation of a car (car_presentation_update), by mode_step: 0 the screen cleared
   (as clear_screen), the palettes of the car's scene (scene_palettes by attract_car_scene_palettes by
   attract_car) and 031740, car_presentation_object_handlers (credit_display, coin_prompt,
   car_presentation) started, a fade in; 1 when it is over, mode_timer 180 frames (6 seconds; the staff
   credits code adds 1200); 2 counts them down, then a fade out; 3 when it is over, link_event 3 and game
   mode 4 (the demo race). attract_lamps_update every frame first. */
void game_mode_car_presentation(void)
{
	uint32_t colour_block = 0, handler_list = 0;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (screen->mode_step)
	{
	case 0:
		break;
	case 1:
		if ((screen->flags & 1u) == 0u)
		{
			screen->mode_timer = 0xB4u;
			screen->mode_step++;
		}
		return;
	case 2:
		mode_timer = screen->mode_timer;
		screen->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
		{
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			screen->mode_step++;
		}
		return;
	case 3:
		if ((screen->flags & 1u) == 0u)
		{
			screen->link_event = 3u;
			AS32(screen->game_mode) = 4u;
		}
		return;
	}
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	colour_block = scene_palettes[attract_car_scene_palettes[screen->attract_car]];
	queue_palette(colour_block);
	colour_block = black_background_palette_ADDRESS;
	queue_palette(colour_block);
	handler_list = car_presentation_objects_ADDRESS;
	start_object_list(handler_list);
	screen->fade_kind = 1u;
	screen->flags |= 1u;
	screen->mode_step++;
}

/* Game mode 4, the demo race of the attract mode (the car just presented, attract_car, on its stage):
   attract_lamps_update, then by mode_step demo_race_set_up, demo_race_fade_in, demo_race_start_timer,
   demo_race_count_down, demo_race_end, and but on the set-up's and the end's frames the race's own steps
   (game_mode_play by game_step, from step 10). */
void game_mode_demo_race(void)
{
	uint32_t left_caller = 0;

	attract_lamps_update();
	left_caller = 0u;
	switch (screen->mode_step)
	{
	case 0:
		left_caller = demo_race_set_up();
		break;
	case 1:
		demo_race_fade_in();
		break;
	case 2:
		demo_race_start_timer();
		break;
	case 3:
		demo_race_count_down();
		break;
	case 4:
		left_caller = demo_race_end(left_caller);
		break;
	}
	if (left_caller != 0u)
		return;
	game_mode_play();
}

/* Step 0 of the demo race: demo_flags bit 8 (the race's code tells the demo by it: the car drives
   itself), the screen cleared (as clear_screen), step 1 with mode_timer 0, and game_step 10 (the demo
   race's set-up in game_mode_play, which does not run this frame). */
uint32_t demo_race_set_up(void)
{
	uint32_t left_caller = 0;

	screen->demo_flags |= 0x100u;
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	screen->mode_step++;
	screen->mode_timer &= 0xFF00u;
	screen->game_step = 0xAu;
	left_caller = 1u;
	return left_caller;
}

/* Step 1 of the demo race: after two frames (mode_timer), a fade in and step 2. */
void demo_race_fade_in(void)
{
	uint8_t frame_parity = 0;

	screen->mode_timer = (screen->mode_timer & 0xFF00u) | ((uint8_t)screen->mode_timer + 1u & 0xFFu);
	frame_parity = (uint8_t)screen->mode_timer & 1u;
	screen->mode_timer = (screen->mode_timer & 0xFF00u) | (frame_parity & 0xFFu);
	if (frame_parity == 0u)
	{
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		screen->mode_step++;
	}
}

/* Step 2 of the demo race: when the fade is over, mode_timer 600 frames (20 seconds) and step 3. */
void demo_race_start_timer(void)
{
	if ((screen->flags & 1u) == 0u)
	{
		screen->mode_timer = 0x258u;
		screen->mode_step++;
	}
}

/* Step 3 of the demo race: counts mode_timer down, then a fade out and step 4. */
void demo_race_count_down(void)
{
	uint16_t mode_timer = 0;

	mode_timer = screen->mode_timer;
	screen->mode_timer = mode_timer - 1u;
	if (mode_timer == 1u)
	{
		screen->fade_kind = 0u;
		screen->flags |= 1u;
		screen->mode_step++;
	}
}

/* Step 4 of the demo race: when the fade is over, game mode 5 (the ranking) and the race record's car
   part cleared (race_record_clear), without the race's step that frame. */
uint32_t demo_race_end(uint32_t left_caller)
{
	uint32_t left_caller_2 = 0;

	if ((screen->flags & 1u) != 0u)
		return left_caller;
	AS32(screen->game_mode) = 5u;
	race_record_clear();
	left_caller_2 = 1u;
	return left_caller_2;
}

/* Game mode 5, the ranking (ranking_update), after the demo race and after a game, by mode_step: 0
   demo_flags bit 8 clear, the wheel motor off (wheel_motor by index), the sound off
   (reset_screen_sound), the screen cleared (as clear_screen), clear_published_car, the ranking's palette
   (ranking_palette), ranking_objects (credit_display, coin_prompt) and ranking_update started,
   mode_timer 270 frames (9 seconds), race game_over cleared; 1 waits until no fade runs
   (ranking_background fades the screen in); 2 counts the 270 frames down; 3 when no fade runs, game mode
   1 (back to the title). attract_lamps_update every frame first. */
void game_mode_ranking(void)
{
	uint32_t colour_block = 0, handler_list = 0;
	struct object *object = NULL;
	uint16_t mode_timer = 0;

	attract_lamps_update();
	switch (screen->mode_step)
	{
	case 0:
		break;
	case 1:
		if ((screen->flags & 1u) == 0u)
			screen->mode_step++;
		return;
	case 2:
		mode_timer = screen->mode_timer;
		screen->mode_timer = mode_timer - 1u;
		if (mode_timer == 1u)
			screen->mode_step++;
		return;
	case 3:
		if ((screen->flags & 1u) == 0u)
			AS32(screen->game_mode) = 1u;
		return;
	}
	screen->demo_flags &= ~0x100u;
	game->wheel_motor[screen->index] = 0u;
	reset_screen_sound();
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
	r12 = address_of(clear_published_car());
	colour_block = ranking_palette;
	queue_palette(colour_block);
	screen->mode_step++;
	handler_list = ranking_objects_ADDRESS;
	start_object_list(handler_list);
	object = start_object();
	object->handler = ranking_update_ADDRESS;
	screen->mode_timer = 0x10Eu;
	RACE(screen->race)->game_over = 0u;
}

/* Game mode 6, a game (check_game_start starts it at step 0) and the demo's race (game_mode_demo_race,
   from step 10), by game_step: 0 sound 21 (queue_sound), the screen's two lamps on (button_lamps),
   lcd_restart_frames cleared (the LCD's timer), a fade out, race game_over cleared; 1 when the fade is
   over, waits until the screen's player is in the list of the race's players (from byte 1F of the
   race_copy record of host_player), then the host itself (not on ISDN) writes 1 to the link board's
   80000A; 2 the car select: demo_flags bit 8 clear, the wheel motor off (wheel_motor), game_start_time
   from the backup RAM's seconds (701F10), a game counted on the seat (701F14 + 2 * index,
   bookkeeping_increment16), C00074 = game_start_c00074_bytes by index, the sound off
   (reset_screen_sound) unless the entry countdown's music plays (entry_countdown_shown), clear_screen,
   clear_race_state, palette car_select_palette, in_game, host_record (the host's race_copy record), the
   stages (screen first_stage-A6) 0, the seat's lamps on (checkpoint_lamp 2, leader_lamp 8),
   car_select_start started, a fade in, the screen on; 3 the car select (car_select_run goes on to 4); 4
   a fade out, entry_countdown_shown cleared, the lamps on; 5 when the fade is over, race ready cleared,
   silence_and_clear_screen, the screen off (C0001C = 4), the stage's palette (queue_stage_load), the
   screen's car (player_car_object_handlers: player_car_update, its place from screen 320 + player), step
   23; 23 once the car has published its state (race_copy published): race_running, the race's objects
   (race_object_handlers; the demo's demo_race_object_handlers), 24; 24 and 25 a frame each; 26 step 11,
   the input ring's places to 0, the screen on; 11 a fade in, step 6; 6 when it is over, 7; 7 the race,
   until its ending is over (flags bit 1, race_car_start_ending): race_running cleared, a fade out, 8; 8
   when it is over, the lamps on, 9; 9 the text and the tilemaps off (clear_text_layer,
   reset_tile_layers), no objects, step 12; 12 the route map: short of the goal (race_copy finished
   clear) the sound off, the song title Meaning of the Light (start_song_title with 003504) and sound 0B;
   palette route_map_palette, route_map_update (route_map_object_handlers), a fade in; 13 when it is
   over, 14; 14 the map until START (buttons_pressed bit 6) or route_map_update sets 16; 15 (nothing sets
   it) 16; 16 with a new course record (course_record_beaten) a fade out and 18, else 21; 18 when the
   fade is over, no objects, name_entry_update started, 19; 19 a fade in, 20; 20 the name entry
   (name_entry_update sets 21); 21 set_player_ready and 22; 21 and 22 once every player of the race is
   ready (players_not_ready): a fade out, fade_out_music, 17; 17 when the fade is over, the game's end:
   the time played (the backup's seconds less game_start_time) added to the seat's total (701F18 + 4 *
   index) and kept if the longest (701F20) or the shortest (701F22), a game counted at the stage level
   reached (screen current_stage_level, 6 at the goal: 701F24 + 2 * it), C00074 = game_end_c00074_bytes
   then game_end_c00074_next_bytes by index, sound 1E, the sound and the tilemaps off, in_game, screen
   race_started and is_host cleared, the race record's car part cleared (race_record_clear), no objects,
   race game_over (the screens in the attract modes go to the ranking, race_entries_update), the lamps
   off, clear_lcd, 28; 28 game mode 5 (the ranking). The demo: 10 silence_and_clear_screen,
   clear_race_state, the stage and the car model from attract_demo_races by attract_car (the stage into
   screen first_stage, A1, A3, A5), host_record its own record at screen + 490 (demo_player_count 1,
   demo_players this screen's player, demo_stage), the model into car model and race model, palette
   title_palette, then as 27; 27 waits until race_copy published is clear, then as 5. */
void game_mode_play(void)
{
	uint32_t zero = 0;
	uint8_t model = 0, player = 0, sound_code = 0, stage = 0;
	uint32_t bookkeeping_entry = 0, colour_block = 0, demo_record = 0, handler_list = 0, index = 0, race = 0, racers_left = 0, song_title = 0, stage_level = 0, time_played = 0;
	const struct demo_race_choice *demo_race_choice = NULL;
	struct object *car = NULL, *object = NULL;
	struct screen_lamps *lamp_place = NULL;
	uint8_t racer = 0;
	uint32_t flags = 0;

	switch (screen->game_step)
	{
	case 0:
		sound_code = 0x21u;
		queue_sound(sound_code);
		screen->button_lamps = 0xFFFFu;
		game->lcd_restart_frames = 0u;
		screen->fade_kind = 0u;
		screen->flags |= 1u;
		screen->game_step++;
		RACE(screen->race)->game_over = 0u;
		return;
	case 1:
		if ((screen->flags & 1u) == 0u)
		{
			race = race_copy_offsets[screen->host_player] + 0x20E01Fu;
			racers_left = RACE(race - 0x1Fu)->racer_count;
			race++;
			if (racers_left == 0u)
				return;
			player = screen->player;
			do
			{
				racer = RACE_ORDER_PLAYERS(race)[0];
				zero = (uint8_t)(racer - player) == 0u;
				race++;
				racers_left--;
			} while ((racers_left != 0u) & (racer != player));
			if (zero)
			{
				if (screen->player == screen->host_player && game->isdn_mode == 0u)
					io_write8(LINK_ENTRIES_CLOSED, 1u);
				screen->game_step++;
			}
		}
		return;
	case 2:
		screen->demo_flags &= ~0x100u;
		game->wheel_motor[screen->index] = 0u;
		screen->game_start_time = BOOKKEEPING_TOTAL_TIME;
		bookkeeping_entry = BOOKKEEPING_GAMES_A_ADDRESS + screen->index * 2u;
		bookkeeping_increment16(bookkeeping_entry);
		io_write8(EVENT_CODE, game_start_c00074_bytes[screen->index]);
		if (screen->entry_countdown_shown == 0u)
			reset_screen_sound();
		clear_screen();
		clear_race_state();
		colour_block = car_select_palette;
		queue_palette(colour_block);
		screen->in_game = 0xFFu;
		screen->host_record = race_copy_offsets[screen->host_player] + GAME_VARIABLES;
		screen->current_stage_level = 0u;
		screen->first_stage = 0u;
		screen->route_stage_level = 0u;
		screen->built_stage = 0u;
		screen->current_stage = 0u;
		screen->route_stage = 0u;
		screen->built_stage_level = 0u;
		lamp_place = pointer_to(GAME_VARIABLES + screen->index * 4u);
		lamp_place->checkpoint_all = 2u;
		lamp_place->leader_all = 8u;
		object = start_object();
		object->handler = car_select_start_ADDRESS;
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x86u);
		else
			io_write8(IO_1_CNT, 6u);
		screen->game_step++;
		return;
	case 3:
		return;
	case 4:
		screen->fade_kind = 0u;
		screen->flags |= 1u;
		screen->game_step++;
		screen->entry_countdown_shown = 0u;
		lamp_place = pointer_to(GAME_VARIABLES + screen->index * 4u);
		lamp_place->checkpoint_all = 2u;
		lamp_place->leader_all = 8u;
		return;
	case 10:
		silence_and_clear_screen();
		clear_race_state();
		demo_race_choice = rom_pointer(attract_demo_races_ADDRESS + screen->attract_car * 2u);
		stage = demo_race_choice->stage;
		screen->first_stage = stage;
		screen->built_stage = stage;
		screen->route_stage = stage;
		screen->current_stage_level = 0u;
		screen->route_stage_level = stage;
		screen->built_stage_level = 0u;
		demo_record = address_of(screen) + 0x490u;
		screen->host_record = demo_record;
		SCREEN(demo_record - 0x490u)->demo_player_count = 1u;
		SCREEN(demo_record - 0x490u)->demo_players = screen->player;
		SCREEN(demo_record - 0x490u)->demo_stage = stage;
		model = demo_race_choice->model;
		player_car->model = model;
		RACE(screen->race)->model = model;
		screen->game_step = 0x1Bu;
		colour_block = title_palette;
		queue_palette(colour_block);
		if (RACE(screen->race_copy)->published != 0u)
			return;
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x84u);
		else
			io_write8(IO_1_CNT, 4u);
		queue_stage_load();
		handler_list = player_car_objects_ADDRESS;
		car = start_object_list(handler_list);
		car->player_car.place = screen->places[screen->player];
		screen->game_step = 0x17u;
		return;
	case 27:
		if (RACE(screen->race_copy)->published != 0u)
			return;
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x84u);
		else
			io_write8(IO_1_CNT, 4u);
		queue_stage_load();
		handler_list = player_car_objects_ADDRESS;
		car = start_object_list(handler_list);
		car->player_car.place = screen->places[screen->player];
		screen->game_step = 0x17u;
		return;
	case 5:
		if ((screen->flags & 1u) != 0u)
			return;
		RACE(screen->race)->ready = 0u;
		silence_and_clear_screen();
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x84u);
		else
			io_write8(IO_1_CNT, 4u);
		queue_stage_load();
		handler_list = player_car_objects_ADDRESS;
		car = start_object_list(handler_list);
		car->player_car.place = screen->places[screen->player];
		screen->game_step = 0x17u;
		return;
	case 23:
		if (RACE(screen->race_copy)->published == 0u)
			return;
		screen->race_running = 0xFFu;
		handler_list = race_objects_ADDRESS;
		if ((screen->demo_flags & 0x100u) != 0u)
			handler_list = demo_race_objects_ADDRESS;
		start_object_list(handler_list);
		screen->game_step++;
		return;
	case 24:
	case 25:
		screen->game_step++;
		return;
	case 26:
		screen->game_step = 0xBu;
		screen->input_write_place = 0u;
		screen->input_read_place = 0u;
		if (screen->index != 0u)
			io_write8(IO_2_CNT, 0x86u);
		else
			io_write8(IO_1_CNT, 6u);
		return;
	case 11:
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		screen->game_step = 6u;
		return;
	case 6:
		if ((screen->flags & 1u) == 0u)
			screen->game_step++;
		return;
	case 7:
		flags = screen->flags;
		screen->flags = flags & ~2u;
		if ((flags & 2u) != 0u)
		{
			screen->race_running = 0u;
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			screen->game_step++;
		}
		return;
	case 8:
		if ((screen->flags & 1u) == 0u)
		{
			lamp_place = pointer_to(GAME_VARIABLES + screen->index * 4u);
			lamp_place->checkpoint_all = 2u;
			lamp_place->leader_all = 8u;
			screen->game_step++;
		}
		return;
	case 9:
		clear_text_layer();
		reset_tile_layers();
		screen->game_step = 0xCu;
		clear_objects();
		return;
	case 12:
		if (RACE(screen->race_copy)->finished == 0u)
		{
			reset_screen_sound();
			song_title = meaning_of_the_light_title_ADDRESS;
			start_song_title(song_title);
			sound_code = 0xBu;
			request_sound(sound_code);
		}
		colour_block = route_map_palette;
		queue_palette(colour_block);
		handler_list = route_map_objects_ADDRESS;
		start_object_list(handler_list);
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		screen->game_step++;
		return;
	case 13:
		if ((screen->flags & 1u) == 0u)
			screen->game_step++;
		return;
	case 14:
		if ((screen->buttons_pressed & 0x40u) != 0u)
		{
			screen->game_step = 0x10u;
			return;
		}
		return;
	case 15:
		screen->game_step = 0x10u;
		return;
	case 16:
		zero = course_record_beaten();
		if (zero == 0u)
		{
			screen->game_step = 0x12u;
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			return;
		}
		screen->game_step = 0x15u;
		return;
	case 18:
		if ((screen->flags & 1u) == 0u)
		{
			clear_objects();
			screen->game_step++;
			object = start_object();
			object->handler = name_entry_update_ADDRESS;
		}
		return;
	case 19:
		screen->fade_kind = 1u;
		screen->flags |= 1u;
		screen->game_step++;
		return;
	case 20:
		return;
	case 21:
		set_player_ready();
		screen->game_step++;
		zero = players_not_ready();
		if (zero == 0u)
		{
			screen->game_step = 0x11u;
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			fade_out_music();
		}
		return;
	case 22:
		zero = players_not_ready();
		if (zero == 0u)
		{
			screen->game_step = 0x11u;
			screen->fade_kind = 0u;
			screen->flags |= 1u;
			fade_out_music();
		}
		return;
	case 17:
		if ((screen->flags & 1u) != 0u)
			return;
		time_played = BOOKKEEPING_TOTAL_TIME - screen->game_start_time;
		bookkeeping_entry = SOUND_RAM + 0x1F18u + screen->index * 4u;
		bookkeeping_add32(bookkeeping_entry, time_played);
		if (BOOKKEEPING_LONGEST_PLAY < (uint16_t)time_played)
		{
			bookkeeping_entry = BOOKKEEPING_LONGEST_PLAY_ADDRESS;
			bookkeeping_set16(bookkeeping_entry, (uint16_t)time_played);
		}
		if ((uint16_t)time_played < BOOKKEEPING_SHORTEST_PLAY)
		{
			bookkeeping_entry = BOOKKEEPING_SHORTEST_PLAY_ADDRESS;
			bookkeeping_set16(bookkeeping_entry, (uint16_t)time_played);
		}
		stage_level = screen->current_stage_level;
		if (RACE(screen->race_copy)->finished != 0u)
			stage_level = 6u;
		bookkeeping_entry = BOOKKEEPING_HISTOGRAM_ADDRESS + stage_level * 2u;
		bookkeeping_increment16(bookkeeping_entry);
		index = screen->index;
		io_write8(EVENT_CODE, game_end_c00074_bytes[index]);
		io_write8(EVENT_CODE, game_end_c00074_next_bytes[index]);
		sound_code = 0x1Eu;
		request_sound(sound_code);
		reset_screen_sound();
		reset_tile_layers();
		screen->race_started = 0u;
		screen->in_game = 0u;
		screen->is_host = 0u;
		race_record_clear();
		clear_objects();
		screen->game_step = 0x1Cu;
		RACE(screen->race)->game_over = 0xFFu;
		lamp_place = pointer_to(GAME_VARIABLES + screen->index * 4u);
		lamp_place->checkpoint_all = 0u;
		lamp_place->leader_all = 0u;
		clear_lcd();
		return;
	case 28:
		AS32(screen->game_mode) = 5u;
		return;
	}
}

/* The screen's sound off (reset_screen_sound), then clear_screen. */
void silence_and_clear_screen(void)
{
	reset_screen_sound();
	clear_screen();
}

/* Clears what the screen shows: no objects (clear_objects), its tilemaps off in the mixer
   (reset_tile_layers), no sprites (clear_sprite_group_count, screen sprite_groups) and the text layer
   cleared (clear_text_layer). */
void clear_screen(void)
{
	clear_objects();
	reset_tile_layers();
	clear_sprite_group_count();
	clear_text_layer();
}

/* Clears the race's state: the race record's car part (race_record_clear), the screen's car block and
   the 100 bytes after it (300 bytes from player_car), the race's order, places and times (screen + 330,
   160 bytes) and the ring of views (screen + 600, 100 bytes). */
void clear_race_state(void)
{
	race_record_clear();
	move_halfwords(address_of(player_car), zero_block_ADDRESS, 0x180u);
	move_halfwords(address_of(screen) + 0x330u, zero_block_ADDRESS, 0xB0u);
	move_halfwords(address_of(screen) + 0x600u, zero_block_ADDRESS, 0x80u);
}
