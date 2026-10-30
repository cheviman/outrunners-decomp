/**
 *    Name:        objects/race_people.c
 *    Description: the race's people and moments: the cars' occupants and their gestures, the opening, the stage end, the camera at the forks, the goal's actors
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* One of the two people in a race car (race_car_occupant_animations, race_car_rider_palettes by
   occupant), seen only while the car shows an animation of its own (its special_body: a crash): then
   drawn at the car's place and size until its animation ends, when the second one lets the car go back
   to its body. */
void race_car_occupant_update(void)
{
	uint32_t animation = 0, occupant = 0, scale = 0;
	struct object *object = NULL;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->draw_mode = 0u;
		occupant = current_object->race_car_occupant.occupant;
		animation = race_car_occupant_animations[occupant];
		current_object->palette = race_car_rider_palettes[occupant];
		animation_start(animation);
		object = pointer_to(LINKED(address_of(screen), current_object->race_car_occupant.car));
		if (object->race_car.special_body == 0u)
			return;
		current_object->state++;
		current_object->y = object->y + screen->sprite_origin_y;
		current_object->frame = 0u;
		current_object->frame_time = 1u;
		current_object->frame_countdown = 1;
		break;
	case 1:
		object = pointer_to(LINKED(address_of(screen), current_object->race_car_occupant.car));
		if (object->race_car.special_body == 0u)
			return;
		current_object->state++;
		current_object->y = object->y + screen->sprite_origin_y;
		current_object->frame = 0u;
		current_object->frame_time = 1u;
		current_object->frame_countdown = 1;
		break;
	case 2:
		break;
	}
	object = pointer_to(LINKED(address_of(screen), current_object->race_car_occupant.car));
	object->race_car.spray_2_frame = 0u;
	object->race_car.spray_1_frame = 0u;
	if (object->race_car.special_body == 0u)
	{
		animation_advance(0u);
		if (current_object->frame_time == 0u)
		{
			object = pointer_to(LINKED(address_of(screen), current_object->race_car_occupant.car));
			current_object->state--;
			object->race_car.special_body = current_object->race_car_occupant.body_back;
			return;
		}
		return;
	}
	current_object->depth_layer = object->depth_layer;
	scale = object->width_scale;
	if (scale > 0x100u)
		scale = 0x100u;
	current_object->width_scale = scale;
	current_object->height_scale = scale;
	current_object->x = object->x + screen->sprite_origin_x;
	if (object->race_car.rival != 0u)
		current_object->y = ((int32_t)(RACE(object->race_car.record)->bounce_height * scale) >> 8) + object->y + screen->sprite_origin_y;
	draw_object(scale);
	animation_advance(0u);
	if (current_object->frame_time == 0u)
	{
		object = pointer_to(LINKED(address_of(screen), current_object->race_car_occupant.car));
		current_object->state--;
		object->race_car.special_body = current_object->race_car_occupant.body_back;
		return;
	}
}

/* A rider's gesture on another racer's car (00974C; race_cars_start starts one for its driver, rider =
   model, and one for its passenger, model + 8; only some have a gesture, rider_gesture_animations, the
   others end at once): when the car moves up a place in the race (rider_gesture_check_place) it plays
   its gesture once (the driver raises an arm, a finger up) in place of the rider, and while the car can
   be seen from the angle and the screen's car is racing (rider_gesture_check_view) it is drawn on the
   car (rider_gesture_place) at the car's size and a layer in front; then waits for the next time. */
void rider_gesture_update(void)
{
	uint32_t zero = 0;
	uint8_t visible_mask = 0;
	uint16_t scale = 0;
	uint32_t animation = 0, rider = 0;
	struct object *object = NULL;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->draw_mode = 0u;
		rider = current_object->rider_gesture.rider;
		animation = rider_gesture_animations[rider];
		if (animation == 0u)
		{
			current_object->flags = 0u;
			current_object->flags_high = 0u;
			return;
		}
		current_object->palette = race_car_rider_palettes[rider];
		animation_start(animation);
		object = pointer_to(LINKED(address_of(screen), current_object->rider_gesture.car));
		visible_mask = 0xFFu;
		zero = rider_gesture_check_place(visible_mask, object);
		if (zero == 0u)
		{
			current_object->state++;
			current_object->frame = 0u;
			current_object->frame_time = 1u;
			current_object->frame_countdown = 1;
			object->race_car.gesture_shown = 0xFFu;
		}
		return;
	case 1:
		object = pointer_to(LINKED(address_of(screen), current_object->rider_gesture.car));
		visible_mask = 0xFFu;
		zero = rider_gesture_check_place(visible_mask, object);
		if (zero == 0u)
		{
			current_object->state++;
			current_object->frame = 0u;
			current_object->frame_time = 1u;
			current_object->frame_countdown = 1;
			object->race_car.gesture_shown = 0xFFu;
		}
		return;
	case 2:
		object = pointer_to(LINKED(address_of(screen), current_object->rider_gesture.car));
		zero = rider_gesture_check_view(object);
		if (zero)
		{
			animation_advance(0u);
			if (current_object->frame_time == 0u)
			{
				object = pointer_to(LINKED(address_of(screen), current_object->rider_gesture.car));
				current_object->state--;
				object->race_car.gesture_shown = 0u;
				return;
			}
			return;
		}
		current_object->depth_layer = object->depth_layer;
		current_object->depth_layer++;
		scale = object->width_scale;
		if (scale > 0x100u)
			scale = 0x100u;
		current_object->width_scale = scale;
		current_object->height_scale = scale;
		rider_gesture_place(scale, object);
		draw_object(scale);
		animation_advance(0u);
		if (current_object->frame_time == 0u)
		{
			object = pointer_to(LINKED(address_of(screen), current_object->rider_gesture.car));
			current_object->state--;
			object->race_car.gesture_shown = 0u;
			return;
		}
		return;
	}
}

/* Whether the gesture shows (zero clear): only while the screen's car object is in state 2 or 4
   (rider_gesture_car_states) and the car is seen from one of the angles that show it
   (rider_gesture_views by the car's view); otherwise clears the car's gesture_shown; then
   rider_gesture_check_place. */
uint32_t rider_gesture_check_view(struct object *car)
{
	uint32_t zero = 0;
	uint32_t visible_mask = 0;
	uint8_t gesture_shown = 0;

	visible_mask = rider_gesture_car_states[OBJECT(player_car->object)->state];
	visible_mask = visible_mask & rider_gesture_views[car->race_car.view];
	if (visible_mask == 0u)
	{
		gesture_shown = car->race_car.gesture_shown & visible_mask;
		zero = gesture_shown == 0u;
		car->race_car.gesture_shown = gesture_shown;
		return zero;
	}
	return rider_gesture_check_place((uint8_t)visible_mask, car);
}

/* Notes the racer's place in the race (screen + 320 + player) in its car's last_place and sets its
   gesture_shown when it is better than the last one; returns gesture_shown masked with R9 (zero set when
   there is no gesture to show). */
uint32_t rider_gesture_check_place(uint8_t visible_mask, struct object *car)
{
	uint32_t zero = 0;
	uint8_t last_place = 0, place = 0;
	uint8_t gesture_shown = 0;

	place = screen->places[current_object->rider_gesture.player];
	last_place = car->race_car.last_place;
	car->race_car.last_place = place;
	if (place < last_place)
		car->race_car.gesture_shown = 0xFFu;
	gesture_shown = car->race_car.gesture_shown & visible_mask;
	zero = gesture_shown == 0u;
	car->race_car.gesture_shown = gesture_shown;
	return zero;
}

/* Places the gesture on its car: the car's x and y (plus the screen's scroll, screen sprite_origin_x and
   sprite_origin_y) and the rider's offset, rider_gesture_x_offsets and rider_gesture_y_offsets by rider
   plus the seat's offset for the car's view (race_car_rider_places by model, by the car's view, the
   driver's or the passenger's), scaled by R0 (the car's width_scale). */
void rider_gesture_place(uint16_t scale, struct object *car)
{
	uint16_t x_offset = 0, y = 0, y_offset = 0;
	uint32_t model = 0, rider = 0, view = 0;
	const struct car_part_place *seat_place = NULL;

	rider = current_object->rider_gesture.rider;
	x_offset = rider_gesture_x_offsets[rider];
	y_offset = rider_gesture_y_offsets[rider];
	model = current_object->rider_gesture.model;
	view = car->race_car.view;
	rider = (uint8_t)(rider - model);
	seat_place = rom_pointer(race_car_rider_places[model] + (uint16_t)(view << 4) + (rider >> 1));
	y = (uint16_t)(car->y + screen->sprite_origin_y + ((int16_t)((uint32_t)(y_offset + seat_place->y) * scale) >> 8));
	current_object->x = car->x + screen->sprite_origin_x + ((int16_t)((uint32_t)(x_offset + seat_place->x) * scale) >> 8);
	current_object->y = y;
}

/* The opening of a race (not in the demo): the view comes down from high above the start line to behind
   the car, taking the car's view_shift_intro up by 18 and view_pitch_intro down by 3 a step until it is
   0 (80 steps from FA60 and F0; camera_step), while the opening's sounds play (race_opening_sounds_play;
   sound_step); ends when both are done. */
void race_opening_update(void)
{
	uint16_t view_pitch_intro = 0;

	switch (current_object->state)
	{
	case 0:
	case 1:
		break;
	case 2:
		current_object->flags = 0u;
		current_object->flags_high = 0u;
		return;
	}
	switch (current_object->race_opening.sound_step)
	{
	case 0:
		race_opening_sounds_start();
		break;
	case 1:
		race_opening_sounds_play();
		break;
	case 2:
		race_opening_sounds_done();
		break;
	}
	switch (current_object->race_opening.camera_step)
	{
	case 0:
		current_object->race_opening.camera_step++;
		return;
	case 1:
		player_car->view_shift_intro += 0x12u;
		view_pitch_intro = player_car->view_pitch_intro;
		player_car->view_pitch_intro = view_pitch_intro - 3u;
		if (view_pitch_intro != 3u)
			return;
		current_object->state++;
		current_object->race_opening.camera_step++;
		return;
	case 2:
		return;
	}
}

/* Starts the opening's sounds: the first at count 1, the rest of race_opening_sound_cues. */
void race_opening_sounds_start(void)
{
	current_object->race_opening.sound_step++;
	current_object->race_opening.next_count = race_opening_sound_cues[0].time;
	current_object->race_opening.sound_list = race_opening_sound_cues_ADDRESS + 0x1u;
}

/* Counts the opening's steps (count) and requests each sound of the list at its count (sound_list:
   sounds 2D, 24, 0A, 21, 2E, 2D, 24 at the counts 1, 20, 65, 120, 150, 160, 190); after the last goes on
   to race_opening_sounds_done. */
void race_opening_sounds_play(void)
{
	uint8_t sound = 0;
	const struct sound_cue *sound_cue = NULL;

	current_object->race_opening.count++;
	if (current_object->race_opening.count < current_object->race_opening.next_count)
	{
		race_opening_sounds_done();
		return;
	}
	sound_cue = rom_pointer(current_object->race_opening.sound_list - 1u);
	sound = sound_cue->sound;
	sound_cue++;
	request_sound(sound);
	if (sound_cue->time != 0u)
	{
		current_object->race_opening.next_count = sound_cue->time;
		current_object->race_opening.sound_list = address_of(sound_cue) + 1u;
		return;
	}
	current_object->state++;
	current_object->race_opening.sound_step++;
	race_opening_sounds_done();
}

/* Does nothing: the opening's sounds are over. */
void race_opening_sounds_done(void)
{
}

/* The sounds as the car nears the end of a stage (before each fork's checkpoint and each goal): sound
   24, then sound 53 20 frames later (the list at 01466F: a delay, then sounds and delays closed by 0);
   when the race record's stage count (race_copy stage_count) changes (the checkpoint or the goal
   passed), sound 24 if this screen's player leads the race (screen player = leader_player), else 28, and
   it ends. */
void stage_end_sounds_update(void)
{
	uint8_t end_sound = 0, sound = 0;
	const struct sound_cue *sound_cue = NULL;
	uint8_t countdown = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		current_object->stage_end_sounds.stage = RACE(screen->race_copy)->stage_count;
		sound_cue = rom_pointer(stage_end_sound_cues_ADDRESS);
		current_object->stage_end_sounds.countdown = sound_cue->time;
		current_object->stage_end_sounds.sound_list = address_of(sound_cue) + 1u;
		return;
	case 1:
		if (current_object->stage_end_sounds.stage != RACE(screen->race_copy)->stage_count)
		{
			current_object->state++;
			return;
		}
		countdown = current_object->stage_end_sounds.countdown;
		current_object->stage_end_sounds.countdown = countdown - 1u;
		if (countdown != 1u)
			return;
		sound_cue = rom_pointer(current_object->stage_end_sounds.sound_list - 1u);
		sound = sound_cue->sound;
		sound_cue++;
		request_sound(sound);
		if (sound_cue->time != 0u)
		{
			current_object->stage_end_sounds.countdown = sound_cue->time;
			current_object->stage_end_sounds.sound_list = address_of(sound_cue) + 1u;
			return;
		}
		current_object->state++;
		return;
	case 2:
		if (current_object->stage_end_sounds.stage == RACE(screen->race_copy)->stage_count)
			return;
		end_sound = 0x24u;
		if (screen->player != screen->leader_player)
			end_sound = 0x28u;
		current_object->flags = 0u;
		current_object->flags_high = 0u;
		request_sound(end_sound);
		return;
	}
}

/* Raises and tilts the view before each fork (256 segments before it), so the fork is seen from above.
   By state: 0 waits until the car (race_copy segment) reaches its position; 1 each frame the view's
   pitch lift (car view_pitch_lift) up by lift_step to lift_height (1E0 less the car model's
   view_pitch_base, screen_view_defaults view_pitch_bases by race_copy model; screen nearing_fork = FF
   when it gets there: the radio's checkpoint message) and its shift lift (car view_shift_lift) down by
   tilt_step; 2 holds until the car reaches lower_position (224 segments on); 3 lowers it back the same
   way and ends. Does nothing while car crash_body_shown is set. */
void fork_camera_update(void)
{
	uint16_t next_pitch_lift = 0;
	uint32_t model = 0;
	uint16_t lift_step = 0, view_pitch_lift = 0;

	if (player_car->crash_body_shown != 0u)
		return;
	switch (current_object->state)
	{
	case 0:
		if (RACE(screen->race_copy)->segment < (uint16_t)current_object->position)
			return;
		current_object->state++;
		model = RACE(screen->race_copy)->model;
		current_object->fork_camera.lift_height = 0u - screen_view_defaults[0].view_pitch_bases[model] + 0x1E0u;
		current_object->fork_camera.lift_step = fork_camera_lift_steps[model];
		current_object->fork_camera.tilt_step = fork_camera_tilt_steps[model];
		current_object->fork_camera.lower_position = current_object->position + 0xE0u;
		player_car->view_pitch_lift = 0u;
		player_car->view_shift_lift = 0u;
		return;
	case 1:
		next_pitch_lift = (uint16_t)(player_car->view_pitch_lift + current_object->fork_camera.lift_step);
		if ((int16_t)current_object->fork_camera.lift_height <= (int16_t)next_pitch_lift)
		{
			current_object->state++;
			next_pitch_lift = current_object->fork_camera.lift_height;
			screen->nearing_fork = 0xFFu;
		}
		player_car->view_pitch_lift = next_pitch_lift;
		player_car->view_shift_lift -= current_object->fork_camera.tilt_step;
		return;
	case 2:
		if (RACE(screen->race_copy)->segment < current_object->fork_camera.lower_position)
			return;
		current_object->state++;
		break;
	case 3:
		break;
	}
	lift_step = current_object->fork_camera.lift_step;
	view_pitch_lift = player_car->view_pitch_lift;
	next_pitch_lift = (uint16_t)(view_pitch_lift - lift_step);
	if ((int16_t)lift_step < (int16_t)view_pitch_lift)
	{
		player_car->view_pitch_lift = next_pitch_lift;
		player_car->view_shift_lift += current_object->fork_camera.tilt_step;
		return;
	}
	current_object->flags = 0u;
	current_object->flags_high = 0u;
	player_car->view_pitch_lift = 0u;
	player_car->view_shift_lift = 0u;
}

/* The end of the race for the screen's car: starts the objects of its ending (ending_actor_lists by its
   model and whether it won, or unfinished_ending_actor_count when it did not finish); when they are done
   (state 2) sound 4F and raises screen flags bit 1, which game step 7 waits for ( the music stopped
   without a finish). */
void race_car_start_ending(void)
{
	uint8_t sound = 0;
	uint32_t actor_count = 0, actor_list = 0, ending = 0, player = 0;
	struct object *actor = NULL;

	switch (current_object->state)
	{
	case 0:
		current_object->state++;
		ending = current_object->race_car.model;
		ending = (uint8_t)(ending * 2u);
		current_object->x += 0xA0u;
		current_object->y += 0xDEu;
		player = screen->player;
		if (RACE(screen->race_copy)->finished == 0u)
		{
			actor_count = unfinished_ending_actor_count;
			actor_list = unfinished_ending_actors_ADDRESS;
		}
		else
		{
			if (screen->places[(int32_t)player] != 0u)
				ending = (uint8_t)(ending + 1u);
			actor_list = ending_actor_lists[ending];
			actor_count = OBJECT_START_LIST(actor_list)->count;
			actor_list += 4u;
		}
		do
		{
			actor = start_object();
			actor->handler = OBJECT_LIST_PAIR(actor_list)->handler;
			actor_list += 4u;
			actor->goal_actor.script = OBJECT_LIST_PAIR(actor_list - 0x4u)->data;
			actor->goal_actor.scene = address_of(current_object);
			actor_list += 4u;
			actor_count--;
		} while (actor_count != 0u);
		break;
	case 1:
	case 3:
		break;
	case 2:
		current_object->state++;
		sound = 0x4Fu;
		request_sound(sound);
		screen->flags |= 2u;
		screen->flags &= 0xFFFFFFEFu;
		if (RACE(screen->race_copy)->finished == 0u)
		{
			fade_out_music();
			return;
		}
		break;
	}
}

/* The player's car in the goal scene: its animation, palette and frame from the race record (race_copy
   animation, palette, frame), its layer, place and scale from the scene object (scene: the car's drawing
   object, 00974C's, whose handler is 014694 at the goal); then goal_actor_start. */
void goal_actor_car_start(void)
{
	uint32_t animation = 0;
	struct object *object = NULL;

	object = pointer_to(current_object->goal_actor.scene);
	animation = RACE(screen->race_copy)->animation;
	current_object->animation = animation;
	animation_start(animation);
	current_object->palette = RACE(screen->race_copy)->palette;
	current_object->frame = RACE(screen->race_copy)->frame;
	current_object->depth_layer = object->depth_layer;
	current_object->x = object->x;
	current_object->y = object->y;
	current_object->width_scale = object->width_scale;
	current_object->height_scale = object->width_scale;
	goal_actor_start();
}

/* The driver in the goal scene: goal_actor_rider_place with the scene object's first rider record (+90). */
void goal_actor_driver_start(void)
{
	uint32_t scene = 0;

	scene = current_object->goal_actor.scene;
	r12 = scene + 0x90u;
	goal_actor_rider_place(pointer_to(scene));
}

/* The passenger in the goal scene: goal_actor_rider_place with the scene object's second rider record
   (+A0). */
void goal_actor_passenger_start(void)
{
	uint32_t scene = 0;

	scene = current_object->goal_actor.scene;
	r12 = scene + 0xA0u;
	goal_actor_rider_place(pointer_to(scene));
}

/* Places a rider in the scene's car: from the car's current frame (the car's animation and frame from
   race_copy animation, frame) the view it is drawn from (bit 6 of the frame's attributes picks the
   mirrored set) and the seat's offset; from the rider record (R12: a table of offsets by view, the
   animation of the first view, the palette, the frame) the rider's animation, palette and frame; scaled
   by the scene object's scale onto its place, in its layer; then goal_actor_start. */
void goal_actor_rider_place(struct object *scene)
{
	uint32_t carry = 0;
	uint32_t animation = 0, view = 0, width_scale = 0, x_offset = 0, y = 0;
	const struct car_part_place *view_offsets = NULL;
	struct race *race = NULL;
	const struct sprite_piece *piece = NULL;

	current_object->palette = RACE_CAR_RIDER(r12)->palette;
	current_object->depth_layer = scene->depth_layer;
	race = pointer_to(screen->race_copy);
	piece = rom_pointer(SPRITE_ANIMATION(sprite_animations[race->animation])->frames[race->frame] + 2u);
	view = piece->attributes;
	view = view & 0x40u;
	view = view >> 6;
	view = (uint16_t)(view * 0x34u);
	view = (uint16_t)(view + (uint16_t)(piece->sprite - scene->race_car.first_body_piece) / 3u);
	y = (uint16_t)piece->y_offset;
	x_offset = (uint16_t)piece->x_offset;
	view_offsets = rom_pointer(RACE_CAR_RIDER(r12)->places + (view << 4));
	x_offset = (uint16_t)(x_offset + view_offsets->x);
	y = (uint16_t)(y + view_offsets->y);
	animation = RACE_CAR_RIDER(r12)->animation + view;
	current_object->animation = animation;
	animation_start(animation);
	current_object->frame = RACE_CAR_RIDER(r12)->frame;
	width_scale = scene->width_scale;
	current_object->width_scale = width_scale;
	current_object->height_scale = width_scale;
	x_offset = (int16_t)x_offset * width_scale;
	y = (int16_t)y * width_scale;
	carry = (x_offset >> 7) & 1u;
	x_offset = (uint16_t)(((int32_t)x_offset >> 8) + carry);
	carry = (y >> 7) & 1u;
	y = (uint16_t)(((int32_t)y >> 8) + carry + scene->y);
	current_object->x = x_offset + scene->x;
	current_object->y = y;
	goal_actor_start();
}

/* Sets an actor of the goal scene to a screen position (draw_mode 0) and goes on to goal_actor_run. */
void goal_actor_start(void)
{
	current_object->handler = goal_actor_run_ADDRESS;
	current_object->draw_mode = 0u;
	goal_actor_run();
}

/* Plays the actor's script (script; step is where this frame's step began), a step a frame: a halfword
   of flags, from bit 15: wait (counter to the next halfword, drawn), repeat from step (until the counter
   passes the next halfword), animation_advance, frame =, x +=, x =, y +=, y =, scale += and = (34, 36),
   layer += and =, a new animation, palette =, a sound (request_sound); with bit 0 clear the step ends
   and the actor is drawn (draw_object); with bit 0 set a command (goal_actor_command_jumps): 0 tells the
   scene object it is over (its state = 2), 1 ends the actor, 2 ends the step undrawn, 3 flips it (flags
   bit 6), 4 car segment_progress + 4000, 5 a repeat whose count grows by 8 for each place behind
   (goal_actor_place_waits by screen + 320 + player). */
void goal_actor_run(void)
{
	uint32_t carry = 0;
	uint8_t frame_time = 0;
	uint16_t step_flags = 0;
	uint32_t animation = 0, ignored = 0, script = 0, script_word = 0, step_value = 0;
	uint16_t wait_count = 0, repeat_count = 0;
	uint32_t next_step = 0, command = 0, step_end = 0;

	script_word = current_object->goal_actor.script;
	for (;;)
	{
		step_flags = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		script_word += 2u;
		if (carry)
		{
			current_object->goal_actor.counter++;
			wait_count = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			if (current_object->goal_actor.counter <= wait_count)
			{
				draw_object(step_value);
				return;
			}
			current_object->goal_actor.counter = 0u;
			current_object->goal_actor.script = script_word;
			continue;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->goal_actor.counter++;
			repeat_count = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			if (current_object->goal_actor.counter <= repeat_count)
			{
				script_word = current_object->goal_actor.step;
				current_object->goal_actor.script = script_word;
				continue;
			}
			current_object->goal_actor.counter = 0u;
			current_object->goal_actor.script = script_word;
			continue;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
			animation_advance(ignored);
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->frame = (uint8_t)GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->x += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->x = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->y += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->y = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->width_scale += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			current_object->height_scale += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->width_scale = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			current_object->height_scale = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->depth_layer += GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->depth_layer = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			animation = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			current_object->animation = animation;
			frame_time = SPRITE_ANIMATION(sprite_animations[animation])->frame_time;
			current_object->frame_time = frame_time;
			current_object->frame_countdown = frame_time;
			current_object->frame = 0u;
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			current_object->palette = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		step_flags = (uint16_t)(step_flags * 2u);
		if (carry)
		{
			step_value = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
			script_word += 2u;
			request_sound((uint8_t)step_value);
		}
		carry = (uint16_t)(step_flags * 2u) < step_flags;
		if (carry == 0u)
		{
			next_step = script_word;
			script = current_object->goal_actor.script;
			current_object->goal_actor.script = next_step;
			current_object->goal_actor.step = script;
			draw_object(step_value);
			return;
		}
		command = GOAL_ACTOR_SCRIPT_WORDS(script_word)[0];
		script_word += 2u;
		switch (command)
		{
		case 0:
			OBJECT(current_object->goal_actor.scene)->state = 2u;
			next_step = script_word;
			script = current_object->goal_actor.script;
			current_object->goal_actor.script = next_step;
			current_object->goal_actor.step = script;
			draw_object(step_value);
			return;
		case 1:
			current_object->flags = 0u;
			current_object->flags_high = 0u;
			return;
		case 2:
			step_end = script_word;
			script = current_object->goal_actor.script;
			current_object->goal_actor.script = step_end;
			current_object->goal_actor.step = script;
			return;
		case 3:
			current_object->flags ^= 0x40u;
			next_step = script_word;
			script = current_object->goal_actor.script;
			current_object->goal_actor.script = next_step;
			current_object->goal_actor.step = script;
			draw_object(step_value);
			return;
		case 4:
			player_car->segment_progress = (player_car->segment_progress & 0xFFFF0000u) | ((uint16_t)player_car->segment_progress + 0x4000u & 0xFFFFu);
			next_step = script_word;
			script = current_object->goal_actor.script;
			current_object->goal_actor.script = next_step;
			current_object->goal_actor.step = script;
			draw_object(step_value);
			return;
		case 5:
			break;
		}
		current_object->goal_actor.counter++;
		step_value = (uint16_t)(GOAL_ACTOR_SCRIPT_WORDS(script_word)[0] + ((uint32_t)goal_actor_place_waits[screen->places[screen->player]] << 3));
		script_word += 2u;
		if (current_object->goal_actor.counter <= step_value)
		{
			script_word = current_object->goal_actor.step;
			current_object->goal_actor.script = script_word;
			continue;
		}
		current_object->goal_actor.counter = 0u;
		current_object->goal_actor.script = script_word;
	}
}
