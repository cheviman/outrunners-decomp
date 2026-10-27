/**
 *    Name:        objects/car_radio.c
 *    Description: the car radio: the songs, the DJ and his messages, the songs' titles
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
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
	uint32_t object = 0, player = 0, race = 0, racers_left = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->list_data = 0xFFFFFFFFu;
	OBJECT(object - 0x80u)->unk_84 = 0xFFFFu;
	OBJECT(object - 0x80u)->unk_86 = 0xFFu;
	OBJECT(object - 0x80u)->unk_87 = 0xFFu;
	race = SCREEN(screen)->host_record + 0x1Fu;
	racers_left = RACE(race - 0x1Fu)->racer_count;
	OBJECT(current_object)->car_radio.cars_to_introduce = racers_left;
	race++;
	do
	{
		player = RACER_NUMBERS(race)[0];
		mem_write8(current_object + 0x80u + car_numbers[player], mem_read8(0x20FC49u + (player << 7)));
		racers_left--;
		race++;
	} while (racers_left != 0u);
	OBJECT(current_object)->car_radio.car_number = car_numbers[SCREEN(screen)->player];
	SCREEN(screen)->nearing_fork = 0u;
	CAR(player_car)->song = car_model_songs[CAR(player_car)->model];
	OBJECT(current_object)->car_radio.stage = SCREEN(screen)->route_stage;
	OBJECT(current_object)->handler = car_radio_wait_start_ADDRESS;
	car_radio_wait_start();
}

/* Waits for the start (the car object's unk_10, the frames to it, down to 0), then goes on to
   car_radio_read_buttons. */
void car_radio_wait_start(void)
{
	if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
		return;
	OBJECT(current_object)->handler = car_radio_read_buttons_ADDRESS;
	car_radio_read_buttons();
}

/* Each frame of the race: the buttons newly pressed (screen buttons_pressed) into buttons, then
   car_radio_update. Once the race is over (car at_goal at the goal, or race_over) clears them, silences
   the DJ (talk_step 5), turns the screen's two lamps off (button_lamps) and leaves car_radio_update
   alone, without the buttons. */
void car_radio_read_buttons(void)
{
	OBJECT(current_object)->car_radio.buttons = (uint16_t)SCREEN(screen)->buttons_pressed;
	if ((CAR(player_car)->at_goal | CAR(player_car)->race_over) == 0u)
	{
		car_radio_update();
		return;
	}
	OBJECT(current_object)->car_radio.buttons = 0u;
	OBJECT(current_object)->car_radio.talk_step = 5u;
	SCREEN(screen)->button_lamps = 0u;
	OBJECT(current_object)->handler = car_radio_update_ADDRESS;
	car_radio_update();
}

/* The radio, by state: 0 plays the car's song (car song) with its title; 1 << and >> step through the
   ten songs (both together: song 10, Jingle Bell), START does >>, the DJ/music button goes to the
   station; 2 the station: title "Mega Radio Station", DJ message 29 queued, the stage's sound
   (route_stage_radio_sounds by screen route_stage); 3 the station plays; 4-8 while screen nearing_fork
   is set (fork_camera_update sets it, a new route_stage clears it): 3F to the screen's sound byte
   (fade_out_music_at), sound E0 after 60 frames, sound EE, 7F when it clears and after 120 frames the
   stage's message (stage_level_radio_messages by screen current_stage_level) and sound. On the station:
   any of the three buttons goes back to the car's song (state 0, talk_step 4); the DJ talks when no
   message plays (screen dj_mouth_shapes), by talk_step: 0 message 0, 1-2 each car number (message 9 +
   number) and its car (message 1 + model), 3 message 2A, 4 the queued_message or, at random, a list of
   remarks (dj_remark_lists), 5 nothing. */
void car_radio_update(void)
{
	bool L_019118 = false, L_019132 = false, L_019137 = false, L_01914C = false, L_019154 = false, L_019167 = false, L_019180 = false;
	uint8_t fade_level = 0, message = 0, queued_message = 0, route_stage = 0, sound = 0;
	uint16_t buttons = 0;
	uint32_t car_number = 0, object = 0, remark_choice = 0, remark_list = 0, remark_routine = 0, song = 0, song_number = 0, title = 0;
	uint8_t previous_song = 0, timer = 0, timer_2 = 0, timer_3 = 0, cars_to_introduce = 0;
	uint32_t jump_target = 0;

	object = OBJECT(current_object)->car_radio.title;
	if (object != 0u && (OBJECT(object)->flags & 1u) == 0u)
		OBJECT(current_object)->car_radio.title = 0u;
	if (SCREEN(screen)->nearing_fork != 0u)
	{
		route_stage = SCREEN(screen)->route_stage;
		if (OBJECT(current_object)->car_radio.stage != route_stage)
		{
			OBJECT(current_object)->car_radio.stage = route_stage;
			SCREEN(screen)->nearing_fork = 0u;
		}
	}
	do
	{
		do
		{
			do
			{
				do
				{
					do
					{
						do
						{
							jump_target = L_019118 ? 0x190F0u : L_019132 ? 0x190F0u : L_01914C ? 0x190F0u : L_019154 ? 0x190F0u : L_019167 ? 0x190F0u : L_019180 ? 0x190F0u : car_radio_state_jumps[OBJECT(current_object)->state];
							switch (jump_target)
							{
							case 0x019068u:
								song_number = CAR(player_car)->song;
								song_number = car_radio_song(song_number);
								request_sound((uint8_t)song_number);
								OBJECT(current_object)->state++;
							case 0x019078u:
								buttons = OBJECT(current_object)->car_radio.buttons;
								if ((buttons & 0x100u) != 0u)
								{
									OBJECT(current_object)->state = 2u;
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
									song = CAR(player_car)->song;
									previous_song = song - 1u;
									song_number = previous_song;
									if ((int8_t)previous_song < 0)
										song_number = 9u;
									CAR(player_car)->song = song_number;
									song_number = car_radio_song(song_number);
									request_sound((uint8_t)song_number);
									return;
								}
								if ((buttons & 0x400u) == 0u && (buttons & 0x40u) == 0u)
									return;
								sound = 0x1Fu;
								request_sound(sound);
								song_number = CAR(player_car)->song;
								song_number = (uint8_t)(song_number + 1u);
								if ((int8_t)song_number < 0xA)
								{
									CAR(player_car)->song = song_number;
									song_number = car_radio_song(song_number);
									request_sound((uint8_t)song_number);
									return;
								}
								song_number = 0u;
								CAR(player_car)->song = song_number;
								song_number = car_radio_song(song_number);
								request_sound((uint8_t)song_number);
								return;
							case 0x0190F0u:
								if (!L_019118)
								{
									if (!L_019132)
									{
										if (!L_01914C)
										{
											if (!L_019154)
											{
												if (!L_019167)
												{
													if (!L_019180)
													{
														title = radio_station_title_ADDRESS;
														car_radio_show_title(title);
														OBJECT(current_object)->car_radio.queued_message = 0x29u;
													}
												}
											}
										}
									}
								}
								do
								{
									if (L_019137 || (!L_019118 && !L_019132 && (L_01914C || L_019154 || L_019167 || L_019180 || SCREEN(screen)->nearing_fork != 0u)))
									{
										if (L_019137 || L_01914C || !L_019154)
										{
											if (L_019137 || L_01914C || !L_019167)
											{
												if (L_019137 || L_01914C || !L_019180)
												{
													if (L_019137 || !L_01914C)
													{
														L_019137 = false;
														sound = 0xE0u;
														request_sound(sound);
														OBJECT(current_object)->car_radio.timer = 0xB4u;
														OBJECT(current_object)->state = 5u;
													}
													L_01914C = false;
													timer = OBJECT(current_object)->car_radio.timer;
													OBJECT(current_object)->car_radio.timer = timer - 1u;
												}
											}
										}
										if (L_019154 || L_019167 || L_019180 || timer == 1u)
										{
											if (L_019154 || !L_019167)
											{
												if (L_019154 || !L_019180)
												{
													if (!L_019154)
														OBJECT(current_object)->state++;
													L_019154 = false;
												}
											}
											if (!L_019167 && !L_019180 && SCREEN(screen)->nearing_fork == 0u)
											{
												OBJECT(current_object)->car_radio.queued_message = stage_level_radio_messages[SCREEN(screen)->current_stage_level];
												sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
												request_sound(sound);
												OBJECT(current_object)->state = 3u;
											}
											else
											{
												if (L_019167 || !L_019180)
												{
													if (!L_019167)
													{
														sound = 0xEEu;
														request_sound(sound);
														OBJECT(current_object)->state++;
													}
													L_019167 = false;
												}
												if (L_019180 || SCREEN(screen)->nearing_fork == 0u)
												{
													if (!L_019180)
													{
														fade_level = 0x7Fu;
														fade_out_music_at(fade_level);
														OBJECT(current_object)->car_radio.timer = 0x78u;
														OBJECT(current_object)->state++;
													}
													L_019180 = false;
													timer_2 = OBJECT(current_object)->car_radio.timer;
													OBJECT(current_object)->car_radio.timer = timer_2 - 1u;
													if (timer_2 == 1u)
													{
														OBJECT(current_object)->car_radio.queued_message = stage_level_radio_messages[SCREEN(screen)->current_stage_level];
														sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
														request_sound(sound);
														OBJECT(current_object)->state = 3u;
													}
												}
											}
										}
									}
									else
									{
										if (L_019118 || !L_019132)
										{
											if (!L_019118)
											{
												sound = route_stage_radio_sounds[SCREEN(screen)->route_stage];
												request_sound(sound);
												OBJECT(current_object)->state++;
											}
											L_019118 = false;
										}
										if (L_019132 || SCREEN(screen)->nearing_fork != 0u)
										{
											if (!L_019132)
											{
												fade_level = 0x3Fu;
												fade_out_music_at(fade_level);
												OBJECT(current_object)->car_radio.timer = 0x3Cu;
												OBJECT(current_object)->state++;
											}
											L_019132 = false;
											timer_3 = OBJECT(current_object)->car_radio.timer;
											OBJECT(current_object)->car_radio.timer = timer_3 - 1u;
											L_019137 = timer_3 == 1u;
										}
									}
								} while (L_019137);
								if ((OBJECT(current_object)->car_radio.buttons & 0x700u) != 0u)
								{
									OBJECT(current_object)->state = 0u;
									OBJECT(current_object)->car_radio.talk_step = 4u;
									SCREEN(screen)->dj_mouth_shapes = 0u;
									sound = 0x1Fu;
									request_sound(sound);
									return;
								}
								if (SCREEN(screen)->dj_mouth_shapes != 0u)
								{
									OBJECT(current_object)->car_radio.queued_message = 0u;
									return;
								}
								switch (OBJECT(current_object)->car_radio.talk_step)
								{
								case 0:
									message = 0u;
									start_dj_message(message);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 1:
									car_number = OBJECT(current_object)->car_radio.next_car_number;
									while ((int8_t)mem_read8(current_object + 0x80u + car_number) < 0)
									{
										car_number = (uint8_t)(car_number + 1u);
									}
									OBJECT(current_object)->car_radio.next_car_number = car_number;
									car_number = (uint8_t)(car_number + 9u);
									start_dj_message((uint8_t)car_number);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 2:
									message = (uint8_t)(mem_read8(current_object + 0x80u + OBJECT(current_object)->car_radio.next_car_number) + 1u);
									start_dj_message(message);
									OBJECT(current_object)->car_radio.next_car_number++;
									cars_to_introduce = OBJECT(current_object)->car_radio.cars_to_introduce;
									OBJECT(current_object)->car_radio.cars_to_introduce = cars_to_introduce - 1u;
									if (cars_to_introduce == 1u)
									{
										OBJECT(current_object)->car_radio.talk_step++;
										return;
									}
									OBJECT(current_object)->car_radio.talk_step--;
									return;
								case 3:
									message = 0x2Au;
									start_dj_message(message);
									OBJECT(current_object)->car_radio.talk_step++;
									return;
								case 4:
									queued_message = OBJECT(current_object)->car_radio.queued_message;
									if (queued_message != 0u)
									{
										OBJECT(current_object)->car_radio.remark_list = 0u;
										start_dj_message(queued_message);
										return;
									}
									if (OBJECT(current_object)->state != 3u)
										return;
									remark_list = OBJECT(current_object)->car_radio.remark_list;
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
									OBJECT(current_object)->car_radio.remark_list = remark_list;
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
									default:
										invalid_jump(remark_routine);
									}
									break;
								case 5:
									return;
								default:
									invalid_jump(car_radio_talk_step_jumps[OBJECT(current_object)->car_radio.talk_step]);
								}
								break;
							case 0x019118u:
								L_019118 = true;
								break;
							case 0x019132u:
								L_019132 = true;
								break;
							case 0x01914Cu:
								L_01914C = true;
								break;
							case 0x019154u:
								L_019154 = true;
								break;
							case 0x019167u:
								L_019167 = true;
								break;
							case 0x019180u:
								L_019180 = true;
								break;
							default:
								invalid_jump(jump_target);
							}
						} while (L_019118);
					} while (L_019132);
				} while (L_01914C);
			} while (L_019154);
		} while (L_019167);
	} while (L_019180);
}

/* A DJ remark: message 11 plus the car number of the car in the lead. */
void car_radio_dj_leader(void)
{
	uint8_t message = 0;

	message = (uint8_t)(car_numbers[RACE(SCREEN(screen)->host_record)->race_order] + 0x11u);
	start_dj_message(message);
}

/* A DJ remark: message 19 plus this screen's car number. */
void car_radio_dj_own_car(void)
{
	uint8_t message = 0;

	message = (uint8_t)(OBJECT(current_object)->car_radio.car_number + 0x19u);
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
uint32_t car_radio_song(uint32_t song_number)
{
	uint32_t song_entry = 0, song_sound = 0;

	song_entry = radio_songs[song_number];
	song_sound = byte_swap32(song_entry);
	song_entry &= 0xFFFFFFu;
	car_radio_show_title(song_entry);
	return song_sound;
}

/* Starts a song title (start_song_title) for the record it is given, sends the one shown (title) away
   and keeps the new one in title. */
void car_radio_show_title(uint32_t title)
{
	uint32_t object = 0, title_object = 0;

	object = OBJECT(current_object)->car_radio.title;
	if (object != 0u)
		OBJECT(object)->song_title.time_left = 0;
	title_object = start_song_title(title);
	OBJECT(current_object)->car_radio.title = title_object;
}

/* Starts an object that shows the song title of record R10 (x, then the text) on the screen and scrolls
   it on the LCD; returns the object. */
uint32_t start_song_title(uint32_t song_title)
{
	uint32_t object = 0;

	object = start_object();
	OBJECT(object)->handler = song_title_show_ADDRESS;
	OBJECT(object)->song_title.text = song_title;
	OBJECT(object)->song_title.time_left = 0x3C;
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
	bool L_019387 = false, L_01938A = false;
	uint32_t negative = 0;
	uint16_t colour = 0, next_speed = 0, speed = 0;
	uint32_t colour_step = 0, position = 0, record = 0, text = 0;
	uint16_t time_left = 0, new_time_left = 0;
	uint32_t jump_target = 0;

	do
	{
		jump_target = L_01938A ? 0x19369u : song_title_state_jumps[OBJECT(current_object)->state];
		switch (jump_target)
		{
		case 0x019344u:
			record = OBJECT(current_object)->song_title.text;
			OBJECT(current_object)->x = SONG_TITLE_RECORD(record)->x;
			record += 2u;
			OBJECT(current_object)->song_title.text = record;
			SCREEN(screen)->lcd_scroll_request = record;
			OBJECT(current_object)->song_title.slide = 0xD2;
			OBJECT(current_object)->song_title.y = 0xC8u;
			OBJECT(current_object)->song_title.speed = 0x14u;
			OBJECT(current_object)->state++;
		case 0x019369u:
			do
			{
				if (L_019387 || L_01938A || (uint16_t)OBJECT(current_object)->song_title.time_left == 0u)
				{
					if (L_019387 || !L_01938A)
					{
						L_019387 = false;
						OBJECT(current_object)->state++;
					}
					L_01938A = false;
					time_left = OBJECT(current_object)->song_title.time_left;
					new_time_left = time_left - 1u;
					negative = new_time_left >> 15;
					OBJECT(current_object)->song_title.time_left = new_time_left;
					if ((int16_t)time_left > 1)
					{
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
						colour_step = OBJECT(current_object)->song_title.colour_step;
						colour_step = (uint8_t)(colour_step + 1u);
						OBJECT(current_object)->song_title.colour_step = colour_step;
						colour_step = colour_step >> 1;
						colour = song_title_colours[colour_step & 7u];
						text = OBJECT(current_object)->song_title.text;
						draw_text_at(position, colour, text);
						return;
					}
					if (negative)
					{
						OBJECT(current_object)->state += 2u;
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
						OBJECT(current_object)->song_title.slide = (uint16_t)position;
						OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
						if ((int16_t)position <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
					else
					{
						OBJECT(current_object)->state++;
						position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
						next_speed = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
						position = (position & ~0xFFFFu) | (uint16_t)(position - next_speed);
						OBJECT(current_object)->song_title.speed = next_speed;
						OBJECT(current_object)->song_title.slide = (uint16_t)position;
						OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
						if ((int16_t)position <= -0xD2)
						{
							OBJECT(current_object)->flags = 0u;
							return;
						}
					}
				}
				else
				{
					position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
					speed = OBJECT(current_object)->song_title.speed;
					position = (position & ~0xFFFFu) | (uint16_t)(position - speed);
					OBJECT(current_object)->song_title.speed = speed - 1u;
					OBJECT(current_object)->song_title.slide = (uint16_t)position;
					OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
					L_019387 = (uint16_t)position == 0u;
				}
			} while (L_019387);
			break;
		case 0x01938Au:
			L_01938A = true;
			break;
		case 0x019394u:
			position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			next_speed = (uint16_t)(OBJECT(current_object)->song_title.speed + 1u);
			position = (position & ~0xFFFFu) | (uint16_t)(position - next_speed);
			OBJECT(current_object)->song_title.speed = next_speed;
			OBJECT(current_object)->song_title.slide = (uint16_t)position;
			OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
			if ((int16_t)position <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
			colour_step = OBJECT(current_object)->song_title.colour_step;
			colour_step = (uint8_t)(colour_step + 1u);
			OBJECT(current_object)->song_title.colour_step = colour_step;
			colour_step = colour_step >> 1;
			colour = song_title_colours[colour_step & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(position, colour, text);
			return;
		case 0x0193BAu:
			position = ((uint16_t)OBJECT(current_object)->song_title.slide | ((uint32_t)OBJECT(current_object)->song_title.y << 16u));
			position = (position & ~0xFFFFu) | (uint16_t)(position - 0x28u);
			OBJECT(current_object)->song_title.slide = (uint16_t)position;
			OBJECT(current_object)->song_title.y = (uint16_t)(position >> 16u);
			if ((int16_t)position <= -0xD2)
			{
				OBJECT(current_object)->flags = 0u;
				return;
			}
			position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
			colour_step = OBJECT(current_object)->song_title.colour_step;
			colour_step = (uint8_t)(colour_step + 1u);
			OBJECT(current_object)->song_title.colour_step = colour_step;
			colour_step = colour_step >> 1;
			colour = song_title_colours[colour_step & 7u];
			text = OBJECT(current_object)->song_title.text;
			draw_text_at(position, colour, text);
			return;
		default:
			invalid_jump(jump_target);
		}
	} while (L_01938A);
	position = (position & ~0xFFFFu) | (uint16_t)(position + OBJECT(current_object)->x);
	colour_step = OBJECT(current_object)->song_title.colour_step;
	colour_step = (uint8_t)(colour_step + 1u);
	OBJECT(current_object)->song_title.colour_step = colour_step;
	colour_step = colour_step >> 1;
	colour = song_title_colours[colour_step & 7u];
	text = OBJECT(current_object)->song_title.text;
	draw_text_at(position, colour, text);
}

/* Starts DJ message R0 (start_dj_message) unless one is being said (screen dj_mouth_shapes). */
void start_dj_message_if_quiet(uint8_t dj_message)
{
	if (SCREEN(screen)->dj_mouth_shapes != 0u)
		return;
	start_dj_message(dj_message);
}

/* Starts DJ message R0 (typed_messages): requests its voice (the sound in the record's top byte) and has
   the face on the LCD move its mouth by the record's string (screen dj_mouth_shapes). */
void start_dj_message(uint8_t message)
{
	uint8_t sound = 0;
	uint32_t message_entry = 0;

	message_entry = dj_messages[(uint8_t)message];
	sound = (uint8_t)byte_swap32(message_entry);
	SCREEN(screen)->dj_mouth_shapes = message_entry & 0xFFFFFFu;
	request_sound(sound);
}
