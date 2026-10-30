/**
 *    Name:        scenes/attract.c
 *    Description: the attract mode's scenes: the bar picture, the cabinet lamps, the car presentation and its car name, the staff credits code, the demo race's captions
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Unused: starts the 81 bars of a picture, an object each (start_object, handler bar_picture_bar_start)
   from the 8-byte records of bar_picture_bars (ended by a negative first word): its place on the screen
   into target_x, target_y and its two scales into scales. The places make two rows across the screen (y
   19-107 and 139-195); what the bars show cannot be told without drawing them. */
void bar_picture_start(void)
{
	const struct bar_picture_bar *bar = NULL;
	struct object *object = NULL;
	uint32_t scales = 0;

	bar = rom_pointer(bar_picture_bars_ADDRESS);
	do
	{
		object = start_object();
		object->handler = bar_picture_bar_start_ADDRESS;
		scales = bar->scales;
		object->list_data = bar->target;
		object->bar_picture.scales = scales;
		bar = bar + 1;
	} while ((int32_t)bar->target >= 0);
}

/* Only unused code reaches it (bar_picture_start): sets up a bar of the picture: drawn on the screen
   (draw_mode 0) with animation 0 and palette 25, its width_scale and height_scale from scales, depth
   layer 3, at the middle of the screen (x A0, y 70), with a random speed_x and speed_y (-8 to 7: a byte
   of RANDOM_GENERATOR shifted right 4); then bar_picture_bar_fly. */
void bar_picture_bar_start(void)
{
	uint32_t animation = 0;
	uint8_t random_x = 0, random_y = 0;
	uint32_t width_scale_long = 0;

	current_object->draw_mode = 0u;
	animation = 0u;
	animation_start(animation);
	current_object->palette = 0x25u;
	width_scale_long = current_object->bar_picture.scales;
	current_object->width_scale = (uint16_t)width_scale_long;
	current_object->height_scale = (uint16_t)(width_scale_long >> 16u);
	current_object->depth_layer = 3u;
	current_object->x = 0xA0u;
	current_object->y = 0x70u;
	random_x = io_read8(0xD80000u);
	current_object->bar_picture.speed_x = (int16_t)(int8_t)random_x >> 4;
	random_y = io_read8(0xD80000u);
	current_object->bar_picture.speed_y = (int16_t)(int8_t)random_y >> 4;
	current_object->handler = bar_picture_bar_fly_ADDRESS;
}

/* Only unused code reaches it (bar_picture_start): each frame the bar's speed_x and speed_y grow by 1
   toward its place (target_x, target_y) and it moves by them, swinging about it; an axis whose next step
   lands exactly on the place is put there for that frame. When both land on the same frame it shows
   frame D and stays (bar_picture_bar_draw). Drawn every frame (draw_object). */
void bar_picture_bar_fly(void)
{
	uint32_t zero = 0, negative = 0, overflow = 0;
	uint16_t old_x_speed = 0, old_y_speed = 0, x_motion = 0;
	uint32_t landed = 0, y_motion = 0;
	uint16_t target_x = 0, x_distance = 0, target_y = 0, y_distance = 0;

	landed = 0xFFFFFFFFu;
	x_motion = (uint16_t)current_object->bar_picture.speed_x;
	old_x_speed = (uint16_t)x_motion;
	x_motion += current_object->x;
	target_x = current_object->bar_picture.target_x;
	x_distance = target_x - x_motion;
	overflow = (uint32_t)(int16_t)target_x - (int16_t)x_motion != (uint32_t)(int16_t)(target_x - x_motion);
	zero = x_distance == 0u;
	negative = x_distance >> 15;
	if (target_x != x_motion)
	{
		x_motion = 1u;
		if ((negative ^ overflow) | zero)
			x_motion = 0xFFFFu;
		x_motion += old_x_speed;
		current_object->bar_picture.speed_x = x_motion;
		x_motion += current_object->x;
		landed = 0u;
	}
	current_object->x = x_motion;
	y_motion = (uint16_t)current_object->bar_picture.speed_y;
	old_y_speed = (uint16_t)y_motion;
	y_motion = (uint16_t)(y_motion + current_object->y);
	target_y = current_object->bar_picture.target_y;
	y_distance = target_y - y_motion;
	overflow = (uint32_t)(int16_t)target_y - (int16_t)y_motion != (uint32_t)(int16_t)(target_y - y_motion);
	zero = y_distance == 0u;
	negative = y_distance >> 15;
	if (target_y == y_motion)
	{
		current_object->y = y_motion;
		if (landed == 0u)
		{
			draw_object(y_motion);
			return;
		}
		current_object->handler = bar_picture_bar_draw_ADDRESS;
		current_object->frame = 0xDu;
		draw_object(y_motion);
		return;
	}
	y_motion = 1u;
	if ((negative ^ overflow) | zero)
		y_motion = 0xFFFFu;
	y_motion = (uint16_t)(y_motion + old_y_speed);
	current_object->bar_picture.speed_y = y_motion;
	y_motion = (uint16_t)(y_motion + current_object->y);
	landed = 0u;
	current_object->y = y_motion;
	if (landed == 0u)
	{
		draw_object(y_motion);
		return;
	}
	current_object->handler = bar_picture_bar_draw_ADDRESS;
	current_object->frame = 0xDu;
	draw_object(y_motion);
}

/* Only unused code reaches it (bar_picture_start): the bar in its place: draws it (draw_object) each
   frame. */
void bar_picture_bar_draw(void)
{
	draw_object(0u);
}

/* The lamp show of the attract modes (game_mode_title, game_mode_car_presentation, game_mode_demo_race,
   game_mode_ranking, game_mode_isdn_connection): every 8 video frames (vblank_counter >> 3) the next of
   32 steps of the pattern attract_lamp_steps, two bytes a step and screen, into the screen's two lamp
   entries (byte 0 of checkpoint_lamp and leader_lamp by 4 * index: bits 1 and 3 of the seat's output
   port D, which the VBLANK writes). */
void attract_lamps_update(void)
{
	uint32_t screen_index = 0;
	const struct attract_lamp_step *attract_lamp_step = NULL;

	attract_lamp_step = rom_pointer(game->vblank_counter);
	attract_lamp_step = rom_pointer(address_of(attract_lamp_step) >> 3);
	attract_lamp_step = rom_pointer((uint8_t)(address_of(attract_lamp_step) * 2u));
	screen_index = screen->index;
	attract_lamp_step = rom_pointer(attract_lamp_steps_ADDRESS + (uint8_t)(address_of(attract_lamp_step) + screen_index) * 2u);
	screen_index = (uint8_t)(screen_index * 2u);
	screen_index = (uint8_t)(screen_index * 2u);
	game->checkpoint_lamp[screen_index] = attract_lamp_step->checkpoint;
	game->leader_lamp[screen_index] = attract_lamp_step->leader;
}

/* The presentation of a car in the attract mode (game mode 3, car_presentation_object_handlers): the
   picture of the car screen attract_car names. By state: 0 starts car_name and staff_credits_code (with
   this object as its presentation, kept in staff_credits_code), the screen's background
   (set_up_background with 001AA0 by screen), the picture (animation attract_car_picture_animations by
   the car, palette 275, draw mode 0) at x A0, y 60 on depth layer 0; 1 steps its animation
   (animation_advance) and draws it. Every frame, in all states, it queues the next of the four palette
   records of car_presentation_palette_cycle (palette_step, round and round) with queue_palette_list, so
   the picture's colours cycle. State 2 (the palette only, no picture) is not set by its own code. */
void car_presentation_update(void)
{
	uint32_t animation = 0, background_setup = 0, colour_list = 0, palette_index = 0;
	struct object *object = NULL;

	switch (current_object->state)
	{
	case 0:
		object = start_object();
		object->handler = car_name_start_ADDRESS;
		object = start_object();
		object->handler = staff_credits_code_start_ADDRESS;
		object->staff_credits_code.presentation = address_of(current_object);
		current_object->car_presentation.staff_credits_code = address_of(object);
		current_object->state++;
		background_setup = 0x1AA0u + screen->index * 5u * 4u;
		set_up_background(background_setup);
		current_object->draw_mode = 0u;
		current_object->x = 0xA0u;
		current_object->y = 0x60u;
		current_object->depth_layer = 0u;
		animation = attract_car_picture_animations[screen->attract_car];
		current_object->palette = 0x275u;
		animation_start(animation);
		animation_advance(0u);
		draw_object(0u);
		break;
	case 1:
		animation_advance(0u);
		draw_object(0u);
		palette_index = current_object->car_presentation.palette_step;
		colour_list = car_presentation_palette_cycle[palette_index];
		if (colour_list == 0u)
		{
			colour_list = car_presentation_palette_cycle[0];
			palette_index = 0u;
		}
		current_object->car_presentation.palette_step = palette_index + 1u;
		queue_palette_list(colour_list);
		return;
	case 2:
		break;
	}
	palette_index = current_object->car_presentation.palette_step;
	colour_list = car_presentation_palette_cycle[palette_index];
	if (colour_list == 0u)
	{
		colour_list = car_presentation_palette_cycle[0];
		palette_index = 0u;
	}
	current_object->car_presentation.palette_step = palette_index + 1u;
	queue_palette_list(colour_list);
}

/* Starts watching for the secret code during the presentation of the cars: takes the controls held
   (staff_credits_code_read_controls), puts code_place at the start of the code (001ADC) and goes on to
   staff_credits_code_watch. */
void staff_credits_code_start(void)
{
	uint32_t zero = 0;
	uint32_t new_controls = 0;

	staff_credits_code_read_controls(&new_controls, &zero);
	current_object->staff_credits_code.code_place = staff_credits_code_sequence_ADDRESS;
	current_object->handler = staff_credits_code_watch_ADDRESS;
	staff_credits_code_watch();
}

/* Watches the controls for the code (staff_credits_code_sequence: shift down twice, shift up twice, the
   wheel left, right, left, right, the brake, the gas; a wrong one starts over); when it is complete:
   sound 9, the presentation 1200 frames longer (screen mode_timer), and staff_credits_code_show. */
void staff_credits_code_watch(void)
{
	uint32_t zero = 0;
	uint8_t sound_code = 0;
	uint32_t new_controls = 0, next_control = 0;
	uint8_t element = 0;
	uint32_t presentation_timer = 0;

	staff_credits_code_read_controls(&new_controls, &zero);
	if (zero)
	{
		staff_credits_code_done();
		return;
	}
	next_control = current_object->staff_credits_code.code_place;
	element = STAFF_CREDITS_CODE_CONTROLS(next_control)[0];
	next_control++;
	if (element != new_controls)
		next_control = staff_credits_code_sequence_ADDRESS;
	current_object->staff_credits_code.code_place = next_control;
	if (STAFF_CREDITS_CODE_CONTROLS(next_control)[0] != 0u)
	{
		staff_credits_code_done();
		return;
	}
	sound_code = 9u;
	queue_sound(sound_code);
	screen->mode_timer += 0x4B0u;
	presentation_timer = current_object->staff_credits_code.presentation + 0x10u;
	OBJECT(presentation_timer - 0x10u)->car_presentation.timer += 0x4B0u;
	current_object->staff_credits_code.frames_left = 0x4B0u;
	current_object->handler = staff_credits_code_show_ADDRESS;
	staff_credits_code_show();
}

/* Shows the staff credits (the texts at 001AE8, draw_text) for 1200 frames, then fade_out_music and
   staff_credits_code_done. */
void staff_credits_code_show(void)
{
	uint32_t text_record = 0;
	const struct sprite_text_record *next_text = NULL;
	uint16_t frames_left = 0;

	frames_left = current_object->staff_credits_code.frames_left;
	current_object->staff_credits_code.frames_left = frames_left - 1u;
	if (frames_left == 1u)
	{
		current_object->handler = staff_credits_code_done_ADDRESS;
		fade_out_music();
		return;
	}
	next_text = rom_pointer(staff_credits_texts_ADDRESS);
	while ((int16_t)next_text->x >= 0)
	{
		text_record = address_of(next_text);
		next_text = rom_pointer(draw_text(rom_pointer(text_record)));
	}
	staff_credits_code_done();
}

/* Does nothing: the code has been used; the object stays until the presentation's objects are cleared. */
void staff_credits_code_done(void)
{
}

/* Reads the controls as bits (0 shift up, 1 shift down, 2 the wheel right, 3 the wheel left, 4 the
   brake, 5 the gas, each with less needed to stay on), keeps them in controls_held and returns the ones
   newly on (zero set when there are none). */
void staff_credits_code_read_controls(uint32_t *p_new_controls, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;
	uint8_t controls_now = 0, held_before = 0, newly_on = 0, reading = 0;
	uint32_t control_bit = 0;
	uint8_t pressed_controls = 0;

	held_before = current_object->staff_credits_code.controls_held;
	controls_now = 0u;
	reading = (uint8_t)~screen->wheel;
	if ((held_before & 8u) != 0u)
	{
		if (reading < 0x40u)
			controls_now |= 8u;
	}
	else if (reading < 0x20u)
		controls_now |= 8u;
	control_bit = 2u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, reading, control_bit);
	reading = screen->gas;
	control_bit = 5u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, reading, control_bit);
	reading = screen->brake;
	control_bit = 4u;
	controls_now = staff_credits_code_control_bit(held_before, controls_now, reading, control_bit);
	reading = (uint8_t)screen->buttons;
	if ((reading & 0x10u) != 0u)
		controls_now |= 1u;
	if ((reading & 0x20u) != 0u)
		controls_now |= 2u;
	current_object->staff_credits_code.controls_held = controls_now;
	pressed_controls = ~held_before & controls_now;
	zero = pressed_controls == 0u;
	newly_on = pressed_controls;
	*p_new_controls = (*p_new_controls & 0xFFFFFF00u) | newly_on;
	*p_zero = zero;
}

/* Sets bit R3 of the controls being read (R1) when the value R2 is E0 or more, or C0 or more when the
   bit was on in the controls held before (R0): staff_credits_code_read_controls' test for the wheel
   turned right, the gas and the brake. */
uint8_t staff_credits_code_control_bit(uint8_t held_before, uint8_t controls_now, uint8_t reading, uint32_t control_bit)
{
	if ((held_before & shl32(1u, (int)(control_bit))) != 0u)
	{
		if (reading >= 0xC0u)
			controls_now |= shl32(1u, (int)(control_bit));
	}
	else if (reading >= 0xE0u)
	{
		controls_now |= shl32(1u, (int)(control_bit));
		return controls_now;
	}
	return controls_now;
}

/* Starts the letters of the presented car's name (attract_car_names by screen attract_car: MAD POWER,
   SPEED BUSTER, EASY HANDLING, SMOOTH OPERATOR, BAD BOY, ROAD MONSTER, QUICK REACTOR, WILD CHASER): a
   car_name_letter for each character, spaces too, 18 pixels apart from x 168, each 2 frames later than
   the one before (delay 15, 17...); then ends. */
void car_name_start(void)
{
	uint8_t letter_delay = 0;
	uint16_t letter_x = 0;
	uint32_t letter_place = 0;
	struct object *object = NULL;

	letter_place = attract_car_names[screen->attract_car];
	letter_x = 0x168u;
	letter_delay = 0xFu;
	while (CAR_NAME_LETTERS(letter_place)[0] != 0u)
	{
		object = start_object();
		object->handler = car_name_letter_update_ADDRESS;
		object->car_name_letter.character = CAR_NAME_LETTERS(letter_place)[0];
		object->x = letter_x;
		letter_x += 0x12u;
		object->car_name_letter.delay = letter_delay;
		letter_delay += 2u;
		letter_place++;
	}
	current_object->flags = 0u;
}

/* A letter of the presented car's name (font 346 at size A0, its colours cycling through 001D70 with
   step_palette, at y 14). By state: 0 sets itself up (its character from character, x + 8); 1 hidden
   until delay runs out; 2 slides in from the right, 24 pixels a frame for 14 frames (slide_frames); 3
   stays until 120 frames after its start (frames_left); 4 slides out to the left, 24 pixels a frame, and
   ends once x is negative. */
void car_name_letter_update(void)
{
	uint32_t animation = 0;
	uint8_t delay = 0, slide_frames = 0;
	uint16_t frames_left = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->draw_mode = 0xAu;
		current_object->depth_layer = 0x19Eu;
		animation = CAR_NAME_FONT;
		animation_start(animation);
		current_object->car_name_letter.palette_list = car_name_letter_palette_steps_ADDRESS;
		current_object->frame = current_object->car_name_letter.character;
		current_object->x += 8u;
		current_object->y = 0x14u;
		current_object->width_scale = 0xA0u;
		current_object->height_scale = 0xA0u;
		current_object->car_name_letter.frames_left = 0x78u;
		current_object->car_name_letter.slide_frames = 0xEu;
		current_object->state++;
		current_object->car_name_letter.frames_left--;
		delay = current_object->car_name_letter.delay;
		current_object->car_name_letter.delay = delay - 1u;
		if (delay != 1u)
			return;
		current_object->state++;
		current_object->x -= 0x18u;
		slide_frames = current_object->car_name_letter.slide_frames;
		current_object->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			current_object->state++;
		break;
	case 1:
		current_object->car_name_letter.frames_left--;
		delay = current_object->car_name_letter.delay;
		current_object->car_name_letter.delay = delay - 1u;
		if (delay != 1u)
			return;
		current_object->state++;
		current_object->x -= 0x18u;
		slide_frames = current_object->car_name_letter.slide_frames;
		current_object->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			current_object->state++;
		frames_left = current_object->car_name_letter.frames_left;
		current_object->car_name_letter.frames_left = frames_left - 1u;
		if (frames_left == 1u)
			current_object->state++;
		step_palette();
		draw_object(0u);
		return;
	case 2:
		current_object->x -= 0x18u;
		slide_frames = current_object->car_name_letter.slide_frames;
		current_object->car_name_letter.slide_frames = slide_frames - 1u;
		if (slide_frames == 1u)
			current_object->state++;
		frames_left = current_object->car_name_letter.frames_left;
		current_object->car_name_letter.frames_left = frames_left - 1u;
		if (frames_left == 1u)
			current_object->state++;
		step_palette();
		draw_object(0u);
		return;
	case 3:
		break;
	case 4:
		current_object->x -= 0x18u;
		if ((int16_t)current_object->x >= 0)
		{
			step_palette();
			draw_object(0u);
			return;
		}
		current_object->flags = 0u;
		return;
	}
	frames_left = current_object->car_name_letter.frames_left;
	current_object->car_name_letter.frames_left = frames_left - 1u;
	if (frames_left == 1u)
		current_object->state++;
	step_palette();
	draw_object(0u);
}

/* The captions over the demo race of the attract mode: counts its frames and runs the routines of
   timeline_1AFC0 as it reaches their frames (30 demo_captions_head_to_head, 225
   demo_captions_checkpoint, 420 demo_captions_music_select); ends once the count passes a record's frame
   (the closing FFFF). */
void demo_captions_run(void)
{
	uint32_t zero = 0;
	const struct demo_caption_cue *cue = NULL;
	uint32_t frames_to_cue = 0, jump_target = 0;

	switch (current_object->state)
	{
	case 0:
		current_object->demo_captions.timeline = timeline_1AFC0_ADDRESS;
		current_object->demo_captions.frames &= 0xFFFF0000u;
		current_object->state++;
		return;
	case 1:
		break;
	}
	current_object->demo_captions.frames = (current_object->demo_captions.frames & 0xFFFF0000u) | ((uint16_t)current_object->demo_captions.frames + 1u & 0xFFFFu);
	cue = rom_pointer(current_object->demo_captions.timeline);
	frames_to_cue = cue->frame - (uint16_t)current_object->demo_captions.frames;
	zero = frames_to_cue == 0u;
	if ((int32_t)frames_to_cue < 0)
	{
		current_object->flags = 0u;
		return;
	}
	if (zero)
	{
		jump_target = cue->routine;
		switch (jump_target)
		{
		case 0x01AE2Du:
			demo_captions_head_to_head();
			break;
		case 0x01AE61u:
			demo_captions_checkpoint();
			break;
		case 0x01AE6Bu:
			demo_captions_music_select();
			break;
		}
		current_object->demo_captions.timeline += 8u;
	}
}

/* Frame 30 of the demo captions: HEAD TO HEAD! 2 PLAYER RACING (demo_caption records 0, 1 and 3 +
   machine_count, or 4 when stand_alone is set; 4 in attract.txt). */
void demo_captions_head_to_head(void)
{
	uint8_t first_record = 0, record = 0;
	uint32_t record_count = 0;
	struct object *object = NULL;

	record_count = 2u;
	first_record = 0u;
	demo_captions_start(first_record, record_count);
	record = 3u;
	if (game->stand_alone == 0u)
		record += game->machine_count;
	else
		record++;
	object = start_object();
	object->handler = demo_caption_update_ADDRESS;
	object->demo_caption.record = record;
}

/* Frame 225 of the demo captions: AT THE CHECKPOINT ONLY THE RACE LEADER CAN SELECT THE NEXT STAGE !
   (demo_caption records 2 and 3). */
void demo_captions_checkpoint(void)
{
	uint8_t first_record = 0;
	uint32_t record_count = 0;

	record_count = 2u;
	first_record = 2u;
	demo_captions_start(first_record, record_count);
}

/* Frame 420 of the demo captions: the radio's panel, the hand pressing its button and MUSIC SELECT
   (demo_caption records 8-11). */
void demo_captions_music_select(void)
{
	uint8_t first_record = 0;
	uint32_t record_count = 0;

	record_count = 4u;
	first_record = 8u;
	demo_captions_start(first_record, record_count);
}

/* Starts R1 demo_caption objects with the records from R0 on. */
void demo_captions_start(uint8_t first_record, uint32_t record_count)
{
	struct object *object = NULL;

	do
	{
		object = start_object();
		object->handler = demo_caption_update_ADDRESS;
		object->demo_caption.record = first_record;
		first_record++;
		record_count--;
	} while (record_count != 0u);
}

/* One picture of the demo captions (demo_captions_start starts them): from its record at 01AF00
   (animation, palette, width_scale, height_scale, x, y, depth_layer, frames_left: 180 frames for records
   0-7, 240 for 8-11), animated and drawn until frames_left runs out. */
void demo_caption_update(void)
{
	uint32_t animation = 0;
	const struct picture_record *picture = NULL;
	uint16_t frames_left = 0;

	switch (current_object->state)
	{
	case 0:
		break;
	case 1:
		frames_left = current_object->demo_caption.frames_left;
		current_object->demo_caption.frames_left = frames_left - 1u;
		if (frames_left == 1u)
		{
			current_object->flags = 0u;
			return;
		}
		animation_advance(0u);
		draw_object(0u);
		return;
	}
	current_object->draw_mode = 0u;
	picture = rom_pointer(demo_caption_pictures_ADDRESS + ((uint32_t)current_object->demo_caption.record << 4));
	animation = picture->animation;
	animation_start(animation);
	current_object->palette = picture->palette;
	current_object->width_scale = picture->width_scale;
	current_object->height_scale = picture->height_scale;
	current_object->x = picture->x;
	current_object->y = picture->y;
	current_object->depth_layer = picture->depth_layer;
	current_object->demo_caption.frames_left = picture->frames_left;
	current_object->state++;
	animation_advance(0u);
	draw_object(0u);
}
