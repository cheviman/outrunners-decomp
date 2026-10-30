/**
 *    Name:        objects/car_radio.c
 *    Description: the car radio: the songs, the DJ and his messages, the songs' titles
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Sets up the radio as the race starts: the car each player chose (byte 49 of its race_copy record) by
   car number in car_models (FF where none) and how many in cars_to_introduce, this screen's car number,
   clears screen nearing_fork, gives the car the song of its model (car song = car_model_songs by car
   model), notes the stage (screen route_stage), then car_radio_wait_start. */
void car_radio_start(void)
{
	uint32_t player = 0, race = 0, racers_left = 0;
	struct object *object = NULL;

	object = current_object;
	object->car_radio.car_models[0] = 0xFFu;
	object->car_radio.car_models[1] = 0xFFu;
	object->car_radio.car_models[2] = 0xFFu;
	object->car_radio.car_models[3] = 0xFFu;
	object->car_radio.car_models[4] = 0xFFu;
	object->car_radio.car_models[5] = 0xFFu;
	object->car_radio.car_models[6] = 0xFFu;
	object->car_radio.car_models[7] = 0xFFu;
	race = screen->host_record + 0x1Fu;
	racers_left = RACE(race - 0x1Fu)->racer_count;
	current_object->car_radio.cars_to_introduce = racers_left;
	race++;
	do
	{
		player = RACER_NUMBERS(race)[0];
		current_object->car_radio.car_models[game->car_numbers[player]] = game->race_copies[player].model;
		racers_left--;
		race++;
	} while (racers_left != 0u);
	current_object->car_radio.car_number = game->car_numbers[screen->player];
	screen->nearing_fork = 0u;
	player_car->song = car_model_songs[player_car->model];
	current_object->car_radio.stage = screen->route_stage;
	current_object->handler = car_radio_wait_start_ADDRESS;
	car_radio_wait_start();
}

/* Waits for the start (the car object's timer, the frames to it, down to 0), then goes on to
   car_radio_read_buttons. */
void car_radio_wait_start(void)
{
	if (OBJECT(player_car->object)->player_car.timer != 0u)
		return;
	current_object->handler = car_radio_read_buttons_ADDRESS;
	car_radio_read_buttons();
}

/* Each frame of the race: the buttons newly pressed (screen buttons_pressed) into buttons, then
   car_radio_update. Once the race is over (car at_goal at the goal, or race_over) clears them, silences
   the DJ (talk_step 5), turns the screen's two lamps off (button_lamps) and leaves car_radio_update
   alone, without the buttons. */
void car_radio_read_buttons(void)
{
	current_object->car_radio.buttons = (uint16_t)screen->buttons_pressed;
	if ((player_car->at_goal | player_car->race_over) == 0u)
	{
		car_radio_update();
		return;
	}
	current_object->car_radio.buttons = 0u;
	current_object->car_radio.talk_step = 5u;
	screen->button_lamps = 0u;
	current_object->handler = car_radio_update_ADDRESS;
	car_radio_update();
}

/* The radio, by state: 0 plays the car's song (car song) with its title; 1 << and >> step through the
   ten songs (both together: song 10, Jingle Bell), START does >>, the DJ/music button goes to the
   station; 2 the station: title "Mega Radio Station", DJ message 29 queued, the stage's sound
   (route_stage_radio_sounds by screen route_stage); 3 the station plays; 4-8 while screen nearing_fork
   is set (fork_camera_update sets it, a new route_stage clears it): 3F to the screen's sound byte
   (fade_out_music_at), sound E0 after 60 frames, sound EE, 7F when it clears and after 120 frames the
   stage's message (stage_level_radio_messages by screen current_stage_level) and sound. On the station
   (3, without nearing_fork) and while 4-8 wait, the DJ talks (car_radio_talk). */
void car_radio_update(void)
{
	uint8_t fade_level = 0, route_stage = 0, sound = 0;
	uint16_t buttons = 0;
	uint32_t song = 0, song_number = 0, title = 0;
	struct object *object = NULL;
	uint8_t previous_song = 0, timer = 0, timer_2 = 0, timer_3 = 0;

	object = pointer_to(current_object->car_radio.title);
	if (object != NULL && (object->flags & 1u) == 0u)
		current_object->car_radio.title = 0u;
	if (screen->nearing_fork != 0u)
	{
		route_stage = screen->route_stage;
		if (current_object->car_radio.stage != route_stage)
		{
			current_object->car_radio.stage = route_stage;
			screen->nearing_fork = 0u;
		}
	}
	for (;;)
	{
		switch (current_object->state)
		{
		case 0:
			break;
		case 1:
			buttons = current_object->car_radio.buttons;
			if ((buttons & 0x100u) != 0u)
			{
				current_object->state = 2u;
				sound = 0x1Fu;
				request_sound(sound);
				return;
			}
			if ((buttons & 0x200u) != 0u)
			{
				if ((buttons & 0x400u) != 0u)
				{
					song_number = 0xAu;
					song_number = car_radio_song(song_number);
					request_sound((uint8_t)song_number);
					return;
				}
				sound = 0x1Fu;
				request_sound(sound);
				song = player_car->song;
				previous_song = song - 1u;
				song_number = previous_song;
				if ((int8_t)previous_song >= 0)
				{
					player_car->song = song_number;
					song_number = car_radio_song(song_number);
					request_sound((uint8_t)song_number);
					return;
				}
				song_number = 9u;
				player_car->song = song_number;
				song_number = car_radio_song(song_number);
				request_sound((uint8_t)song_number);
				return;
			}
			if ((buttons & 0x400u) == 0u && (buttons & 0x40u) == 0u)
				return;
			sound = 0x1Fu;
			request_sound(sound);
			song_number = player_car->song;
			song_number = (uint8_t)(song_number + 1u);
			if ((int8_t)song_number < 0xA)
			{
				player_car->song = song_number;
				song_number = car_radio_song(song_number);
				request_sound((uint8_t)song_number);
				return;
			}
			song_number = 0u;
			player_car->song = song_number;
			song_number = car_radio_song(song_number);
			request_sound((uint8_t)song_number);
			return;
		case 2:
			title = radio_station_title_ADDRESS;
			car_radio_show_title(title);
			current_object->car_radio.queued_message = 0x29u;
			if (screen->nearing_fork != 0u)
			{
				sound = 0xE0u;
				request_sound(sound);
				current_object->car_radio.timer = 0xB4u;
				current_object->state = 5u;
				continue;
			}
			sound = route_stage_radio_sounds[screen->route_stage];
			request_sound(sound);
			current_object->state++;
		case 3:
			if (screen->nearing_fork == 0u)
			{
				car_radio_talk();
				return;
			}
			fade_level = 0x3Fu;
			fade_out_music_at(fade_level);
			current_object->car_radio.timer = 0x3Cu;
			current_object->state++;
		case 4:
			timer = current_object->car_radio.timer;
			current_object->car_radio.timer = timer - 1u;
			if (timer != 1u)
			{
				car_radio_talk();
				return;
			}
			sound = 0xE0u;
			request_sound(sound);
			current_object->car_radio.timer = 0xB4u;
			current_object->state = 5u;
			continue;
		case 5:
			timer_2 = current_object->car_radio.timer;
			current_object->car_radio.timer = timer_2 - 1u;
			if (timer_2 != 1u)
			{
				car_radio_talk();
				return;
			}
			current_object->state++;
		case 6:
			if (screen->nearing_fork == 0u)
			{
				current_object->car_radio.queued_message = stage_level_radio_messages[screen->current_stage_level];
				sound = route_stage_radio_sounds[screen->route_stage];
				request_sound(sound);
				current_object->state = 3u;
				car_radio_talk();
				return;
			}
			sound = 0xEEu;
			request_sound(sound);
			current_object->state++;
		case 7:
			if (screen->nearing_fork != 0u)
			{
				car_radio_talk();
				return;
			}
			fade_level = 0x7Fu;
			fade_out_music_at(fade_level);
			current_object->car_radio.timer = 0x78u;
			current_object->state++;
		case 8:
			timer_3 = current_object->car_radio.timer;
			current_object->car_radio.timer = timer_3 - 1u;
			if (timer_3 != 1u)
			{
				car_radio_talk();
				return;
			}
			current_object->car_radio.queued_message = stage_level_radio_messages[screen->current_stage_level];
			sound = route_stage_radio_sounds[screen->route_stage];
			request_sound(sound);
			current_object->state = 3u;
			car_radio_talk();
			return;
		}
		break;
	}
	song_number = player_car->song;
	song_number = car_radio_song(song_number);
	request_sound((uint8_t)song_number);
	current_object->state++;
	buttons = current_object->car_radio.buttons;
	if ((buttons & 0x100u) != 0u)
	{
		current_object->state = 2u;
		sound = 0x1Fu;
		request_sound(sound);
		return;
	}
	if ((buttons & 0x200u) != 0u)
	{
		if ((buttons & 0x400u) != 0u)
		{
			song_number = 0xAu;
			song_number = car_radio_song(song_number);
			request_sound((uint8_t)song_number);
			return;
		}
		sound = 0x1Fu;
		request_sound(sound);
		song = player_car->song;
		previous_song = song - 1u;
		song_number = previous_song;
		if ((int8_t)previous_song < 0)
			song_number = 9u;
		player_car->song = song_number;
		song_number = car_radio_song(song_number);
		request_sound((uint8_t)song_number);
		return;
	}
	if ((buttons & 0x400u) == 0u && (buttons & 0x40u) == 0u)
		return;
	sound = 0x1Fu;
	request_sound(sound);
	song_number = player_car->song;
	song_number = (uint8_t)(song_number + 1u);
	if ((int8_t)song_number < 0xA)
	{
		player_car->song = song_number;
		song_number = car_radio_song(song_number);
		request_sound((uint8_t)song_number);
		return;
	}
	song_number = 0u;
	player_car->song = song_number;
	song_number = car_radio_song(song_number);
	request_sound((uint8_t)song_number);
}

/* The station's DJ, each frame of car_radio_update's state 3 (without screen nearing_fork) and while its
   states 4-8 wait: any of the three buttons goes back to the car's song (state 0, talk_step 4, screen
   dj_mouth_shapes 0, sound 1F); while a message plays (dj_mouth_shapes) the queued_message is dropped;
   else by talk_step: 0 message 0, 1-2 each car number (message 9 + number) and its car (message 1 +
   model), cars_to_introduce of them, 3 message 2A, 4 the queued_message, or in state 3 the next remark
   of the list (remark_list), a new list of dj_remark_lists 14 times in 1024, 5 nothing. */
void car_radio_talk(void)
{
	uint8_t message = 0, queued_message = 0, sound = 0;
	uint32_t car_number = 0, remark_choice = 0, remark_list = 0, remark_routine = 0;
	uint8_t cars_to_introduce = 0;

	if ((current_object->car_radio.buttons & 0x700u) != 0u)
	{
		current_object->state = 0u;
		current_object->car_radio.talk_step = 4u;
		screen->dj_mouth_shapes = 0u;
		sound = 0x1Fu;
		request_sound(sound);
		return;
	}
	if (screen->dj_mouth_shapes != 0u)
	{
		current_object->car_radio.queued_message = 0u;
		return;
	}
	switch (current_object->car_radio.talk_step)
	{
	case 0:
		message = 0u;
		start_dj_message(message);
		current_object->car_radio.talk_step++;
		return;
	case 1:
		car_number = current_object->car_radio.next_car_number;
		while ((int8_t)current_object->car_radio.car_models[(int32_t)car_number] < 0)
		{
			car_number = (uint8_t)(car_number + 1u);
		}
		current_object->car_radio.next_car_number = car_number;
		car_number = (uint8_t)(car_number + 9u);
		start_dj_message((uint8_t)car_number);
		current_object->car_radio.talk_step++;
		return;
	case 2:
		message = (uint8_t)(current_object->car_radio.car_models[current_object->car_radio.next_car_number] + 1u);
		start_dj_message(message);
		current_object->car_radio.next_car_number++;
		cars_to_introduce = current_object->car_radio.cars_to_introduce;
		current_object->car_radio.cars_to_introduce = cars_to_introduce - 1u;
		if (cars_to_introduce == 1u)
		{
			current_object->car_radio.talk_step++;
			return;
		}
		current_object->car_radio.talk_step--;
		return;
	case 3:
		message = 0x2Au;
		start_dj_message(message);
		current_object->car_radio.talk_step++;
		return;
	case 4:
		queued_message = current_object->car_radio.queued_message;
		if (queued_message != 0u)
		{
			current_object->car_radio.remark_list = 0u;
			start_dj_message(queued_message);
			return;
		}
		if (current_object->state != 3u)
			return;
		remark_list = current_object->car_radio.remark_list;
		if (remark_list == 0u)
		{
			remark_choice = random_number();
			remark_choice &= 0x3FFu;
			if ((int32_t)remark_choice >= 0xE)
				return;
			remark_list = dj_remark_lists[remark_choice];
		}
		remark_routine = REMARK_ROUTINES(remark_list)[0];
		remark_list += 4u;
		if (REMARK_ROUTINES(remark_list)[0] == 0u)
			remark_list = 0u;
		current_object->car_radio.remark_list = remark_list;
		switch (remark_routine)
		{
		case 0x01929Eu:
			car_radio_dj_leader();
			return;
		case 0x0192B1u:
			car_radio_dj_own_car();
			return;
		case 0x0192BCu:
			car_radio_dj_remark_2a();
			return;
		case 0x0192C3u:
			car_radio_dj_remark_2b();
			return;
		case 0x0192CAu:
			car_radio_dj_remark_2d();
			return;
		case 0x0192D1u:
			car_radio_dj_remark_2f();
			return;
		case 0x0192D8u:
			car_radio_dj_remark_2c();
			return;
		}
		break;
	case 5:
		return;
	}
}

/* A DJ remark: message 11 plus the car number of the car in the lead. */
void car_radio_dj_leader(void)
{
	uint8_t message = 0;

	message = (uint8_t)(game->car_numbers[RACE(screen->host_record)->race_order] + 0x11u);
	start_dj_message(message);
}

/* A DJ remark: message 19 plus this screen's car number. */
void car_radio_dj_own_car(void)
{
	uint8_t message = 0;

	message = (uint8_t)(current_object->car_radio.car_number + 0x19u);
	start_dj_message(message);
}

/* A DJ remark: message 2A (also the end of the cars' introduction). */
void car_radio_dj_remark_2a(void)
{
	uint8_t message = 0;

	message = 0x2Au;
	start_dj_message(message);
}

/* A DJ remark: message 2B. */
void car_radio_dj_remark_2b(void)
{
	uint8_t message = 0;

	message = 0x2Bu;
	start_dj_message(message);
}

/* A DJ remark: message 2D. */
void car_radio_dj_remark_2d(void)
{
	uint8_t message = 0;

	message = 0x2Du;
	start_dj_message(message);
}

/* A DJ remark: message 2F. */
void car_radio_dj_remark_2f(void)
{
	uint8_t message = 0;

	message = 0x2Fu;
	start_dj_message(message);
}

/* A DJ remark: message 2C. */
void car_radio_dj_remark_2c(void)
{
	uint8_t message = 0;

	message = 0x2Cu;
	start_dj_message(message);
}

/* Shows the title of song R0 (radio_songs: 0 Magical Sound Shower, 1 Passing Breeze, 2 Splash Wave, 3
   Picture the Rivers, 4 Blow Your Cool, 5 Looking for the Rainbow, 6 Speed King, 7 Adventure, 8 Sonic
   Control, 9 Last Wave, 10 Jingle Bell) and returns the song's sound. */
uint8_t car_radio_song(uint32_t song_number)
{
	uint8_t song_sound = 0;
	uint32_t song_entry = 0;

	song_entry = radio_songs[song_number];
	song_sound = (uint8_t)byte_swap32(song_entry);
	song_entry &= 0xFFFFFFu;
	car_radio_show_title(song_entry);
	return song_sound;
}

/* Starts a song title (start_song_title) for the record it is given, sends the one shown (title) away
   and keeps the new one in title. */
void car_radio_show_title(uint32_t title)
{
	uint32_t title_object = 0;
	struct object *object = NULL;

	object = pointer_to(current_object->car_radio.title);
	if (object != NULL)
		object->song_title.time_left = 0;
	title_object = address_of(start_song_title(title));
	current_object->car_radio.title = title_object;
}

/* Starts an object that shows the song title of record R10 (x, then the text) on the screen and scrolls
   it on the LCD; returns the object. */
struct object *start_song_title(uint32_t song_title)
{
	struct object *object = NULL;

	object = start_object();
	object->handler = song_title_show_ADDRESS;
	object->song_title.text = song_title;
	object->song_title.time_left = 0x3C;
	return object;
}

/* A song title on the screen (start_song_title starts it, for car_radio, the car select and 00C8B6): its
   x and text from the record (x, and the text, which also scrolls on the LCD through screen
   lcd_scroll_request), then slides in from the right (slide from D2, slowing down to a stop by speed),
   stays for time_left (60 frames) and slides out to the left speeding up; sent away early (time_left 0,
   car_radio_show_title) it leaves at 40 a frame. Drawn each frame with draw_text_at at y C8 in colours
   that cycle every 2 frames (song_title_colours by colour_step). */
void song_title_show(void)
{
	uint32_t negative = 0;
	uint16_t colour = 0, next_speed = 0, speed = 0;
	uint32_t colour_step = 0, position = 0, text = 0;
	const struct song_title_record *record = NULL;
	uint16_t time_left = 0, new_time_left = 0;

	switch (current_object->state)
	{
	case 0:
		record = rom_pointer(current_object->song_title.text);
		current_object->x = record->x;
		record = record + 1;
		current_object->song_title.text = address_of(record);
		screen->lcd_scroll_request = address_of(record);
		current_object->song_title.slide = 0xD2;
		current_object->song_title.y = 0xC8u;
		current_object->song_title.speed = 0x14u;
		current_object->state++;
	case 1:
		if ((uint16_t)current_object->song_title.time_left != 0u)
		{
			position = ((uint16_t)current_object->song_title.slide | ((uint32_t)current_object->song_title.y << 16u));
			speed = current_object->song_title.speed;
			position = (position & ~0xFFFFu) | (uint16_t)(position - speed);
			current_object->song_title.speed = speed - 1u;
			current_object->song_title.slide = (uint16_t)position;
			current_object->song_title.y = (uint16_t)(position >> 16u);
			if ((uint16_t)position != 0u)
				break;
		}
		current_object->state++;
	case 2:
		time_left = current_object->song_title.time_left;
		new_time_left = time_left - 1u;
		negative = new_time_left >> 15;
		current_object->song_title.time_left = new_time_left;
		if ((int16_t)time_left > 1)
		{
			position = ((uint16_t)current_object->song_title.slide | ((uint32_t)current_object->song_title.y << 16u));
			break;
		}
		if (negative)
		{
			current_object->state += 2u;
			position = ((uint16_t)current_object->song_title.slide | ((uint32_t)current_object->song_title.y << 16u));
			position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
			current_object->song_title.slide = (uint16_t)position;
			current_object->song_title.y = (uint16_t)(position >> 16u);
			if ((int16_t)position <= -0xD2)
			{
				current_object->flags = 0u;
				return;
			}
			break;
		}
		current_object->state++;
	case 3:
		position = ((uint16_t)current_object->song_title.slide | ((uint32_t)current_object->song_title.y << 16u));
		next_speed = (uint16_t)(current_object->song_title.speed + 1u);
		position = (position & ~0xFFFFu) | (uint16_t)(position - next_speed);
		current_object->song_title.speed = next_speed;
		current_object->song_title.slide = (uint16_t)position;
		current_object->song_title.y = (uint16_t)(position >> 16u);
		if ((int16_t)position <= -0xD2)
		{
			current_object->flags = 0u;
			return;
		}
		break;
	case 4:
		position = ((uint16_t)current_object->song_title.slide | ((uint32_t)current_object->song_title.y << 16u));
		position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
		current_object->song_title.slide = (uint16_t)position;
		current_object->song_title.y = (uint16_t)(position >> 16u);
		if ((int16_t)position <= -0xD2)
		{
			current_object->flags = 0u;
			return;
		}
		break;
	}
	position = (position & ~0xFFFFu) | (uint16_t)(position + current_object->x);
	colour_step = current_object->song_title.colour_step;
	colour_step = (uint8_t)(colour_step + 1u);
	current_object->song_title.colour_step = colour_step;
	colour_step = colour_step >> 1;
	colour = song_title_colours[colour_step & 7u];
	text = current_object->song_title.text;
	draw_text_at(position, colour, text);
}

/* Starts DJ message R0 (start_dj_message) unless one is being said (screen dj_mouth_shapes). */
void start_dj_message_if_quiet(uint8_t dj_message)
{
	if (screen->dj_mouth_shapes != 0u)
		return;
	start_dj_message(dj_message);
}

/* Starts DJ message R0 (typed_messages): requests its voice (the sound in the record's top byte) and has
   the face on the LCD move its mouth by the record's string (screen dj_mouth_shapes). */
void start_dj_message(uint8_t message)
{
	uint8_t sound = 0;
	uint32_t message_entry = 0;

	message_entry = dj_messages[message];
	sound = (uint8_t)byte_swap32(message_entry);
	screen->dj_mouth_shapes = message_entry & 0xFFFFFFu;
	request_sound(sound);
}
