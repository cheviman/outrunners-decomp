/**
 *    Name:        race/hud.c
 *    Description: the race's display: the rev meter, the speedometer, the gear, the place, the lap timer, the time left, the stage and goal messages, the race result, the checkpoint board
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The rev meter, the bar left of the speed at the bottom right: state 0 sets it up (animation 6, at F2,
   D6), 1 draws the frame of the revs (car revs >> 11, 32 steps). */
void rev_meter_update(void)
{
	uint32_t animation = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		OBJECT(current_object)->frame = CAR(player_car)->revs >> 11;
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->state++;
	animation = REV_METER;
	OBJECT(current_object)->palette = 0x44u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF2u;
	OBJECT(current_object)->y = 0xD6u;
	animation_start(animation);
}

/* The speed in km/h at the bottom right: state 0 sets it up (digits of animation 12, units_x, digits_y)
   and clears the screen's maximum (maximum_speed), sum (speed_sum) and count (speed_samples); 1 each
   frame the car's speed / 88 into screen shown_speed, and while moving adds it to the sum, counts it and
   keeps the maximum (for the LCD's pages), then draws its digits from the units leftwards. */
void speedometer_update(void)
{
	uint16_t state_code = 0;
	uint32_t animation = 0, digit = 0, speed_kmh = 0;
	uint32_t saved_speed_kmh = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = SPEED_DIGITS;
		OBJECT(current_object)->palette = 0x48u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->speedometer.units_x = 0x118u;
		OBJECT(current_object)->speedometer.digits_y = 0xCEu;
		SCREEN(screen)->speed_sum = 0u;
		SCREEN(screen)->speed_samples = 0u;
		SCREEN(screen)->maximum_speed = 0;
		animation_start(animation);
		return;
	case 1:
		speed_kmh = OBJECT(CAR(player_car)->object)->player_car.speed;
		SCREEN(screen)->shown_speed = speed_kmh;
		if (speed_kmh != 0u)
		{
			speed_kmh = speed_kmh / 0x88u;
			SCREEN(screen)->shown_speed = speed_kmh;
			SCREEN(screen)->speed_sum += speed_kmh;
			SCREEN(screen)->speed_samples++;
			if (SCREEN(screen)->maximum_speed < (int16_t)speed_kmh)
				SCREEN(screen)->maximum_speed = speed_kmh;
		}
		OBJECT(current_object)->x = OBJECT(current_object)->speedometer.units_x;
		OBJECT(current_object)->y = OBJECT(current_object)->speedometer.digits_y;
		do
		{
			digit = 0u;
			divx_pair(0xAu, &speed_kmh, &digit);
			OBJECT(current_object)->frame = digit;
			saved_speed_kmh = speed_kmh;
			draw_object(state_code);
			speed_kmh = saved_speed_kmh;
			OBJECT(current_object)->x -= 8u;
		} while ((uint16_t)speed_kmh != 0u);
		return;
	}
}

/* The km/h after the speed: state 0 sets it up (animation 10 at 127, CE), 1 draws it. */
void speed_unit_update(void)
{
	uint32_t animation = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		draw_object(0u);
		return;
	}
	OBJECT(current_object)->state++;
	animation = SPEED_UNIT_KMH;
	OBJECT(current_object)->palette = 0x47u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x127u;
	OBJECT(current_object)->y = 0xCEu;
	animation_start(animation);
}

/* The gear at the bottom right, and the shifting: state 0 sets it up (gear_indicator_animations by the
   top gear: AUTO for the automatic). 1 each frame: the shift buttons (screen buttons_pressed bits 4 up,
   5 down) change the gear within 0 and the top gear; shifting up at revs F000 or more, or down into revs
   past FFFF, spins the wheels 30 frames (car shift_spin, tyre noise 1); the gear's ratio to car
   gear_ratio, its frame drawn. Below the top gear with the revs at the limit (FF00) and the car moving,
   after 150 frames starts the shift_up_hint once. Once the race is over (car at_goal or race_over), 2
   only draws. */
void gear_indicator_update(void)
{
	uint8_t buttons_pressed = 0, tyre_noise = 0;
	uint32_t animation = 0, gear = 0, object = 0, old_ratio = 0, top_gear_then_frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = gear_indicator_animations[CAR(player_car)->top_gear];
		OBJECT(current_object)->palette = 0x45u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0x114u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(animation);
		return;
	case 1:
		if (CAR(player_car)->shift_spin != 0u)
		{
			CAR(player_car)->shift_spin--;
			tyre_noise = 1u;
			set_tyre_noise_on_road(tyre_noise);
		}
		top_gear_then_frames = CAR(player_car)->top_gear;
		gear = CAR(player_car)->gear;
		buttons_pressed = (uint8_t)SCREEN(screen)->buttons_pressed;
		if ((buttons_pressed & 0x10u) != 0u && gear != top_gear_then_frames)
		{
			gear = (uint8_t)(gear + 1u);
			if (CAR(player_car)->revs >= 0xF000u)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		if ((buttons_pressed & 0x20u) != 0u && gear != 0u)
		{
			old_ratio = CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[(int32_t)gear];
			gear = (uint8_t)(gear - 1u);
			if (divide_u32(CAR(player_car)->revs * old_ratio, CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[(int32_t)gear]) >= 0xFFFFu)
				CAR(player_car)->shift_spin = 0x1Eu;
		}
		CAR(player_car)->gear = gear;
		CAR(player_car)->gear_ratio = CAR_GEAR_RATIOS(CAR(player_car)->gear_ratios)[(int32_t)gear];
		OBJECT(current_object)->frame = gear;
		if (gear == top_gear_then_frames)
		{
			if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
				OBJECT(current_object)->state++;
			draw_object(top_gear_then_frames);
			return;
		}
		top_gear_then_frames = 0u;
		if (CAR(player_car)->revs >= 0xFF00u && CAR(player_car)->speed != 0u)
		{
			top_gear_then_frames = (uint16_t)(OBJECT(current_object)->gear_indicator.redline_frames + 1u);
			if (OBJECT(current_object)->gear_indicator.hint_started == 0u && top_gear_then_frames >= 0x96u)
			{
				OBJECT(current_object)->gear_indicator.hint_started = 0xFFu;
				object = start_object();
				OBJECT(object)->handler = shift_up_hint_update_ADDRESS;
				OBJECT(object)->shift_up_hint.indicator = current_object;
				OBJECT(object)->shift_up_hint.gear = gear;
			}
		}
		OBJECT(current_object)->gear_indicator.redline_frames = top_gear_then_frames;
		if (CAR(player_car)->at_goal != 0u || CAR(player_car)->race_over != 0u)
			OBJECT(current_object)->state++;
		draw_object(top_gear_then_frames);
		return;
	case 2:
		draw_object(0u);
		return;
	}
}

/* A hint to shift up, blinking at the bottom centre (animation E): state 0 sets it up; 1 blinks it (4
   frames of 8) while the car stays in the gear it started in with the revs at FC00 or more, then ends
   and lets the gear indicator start it again. */
void shift_up_hint_update(void)
{
	uint32_t animation = 0, blink = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		animation = SHIFT_UP_HINT;
		OBJECT(current_object)->palette = 0x46u;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD8u;
		animation_start(animation);
		return;
	case 1:
		if (CAR(player_car)->gear != OBJECT(current_object)->shift_up_hint.gear || CAR(player_car)->revs < 0xFC00u)
		{
			OBJECT(current_object)->flags = 0u;
			OBJECT(current_object)->flags_high = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.hint_started = 0u;
			OBJECT(OBJECT(current_object)->shift_up_hint.indicator)->gear_indicator.redline_frames = 0u;
			return;
		}
		blink = (uint8_t)(OBJECT(current_object)->shift_up_hint.blink + 1u);
		OBJECT(current_object)->shift_up_hint.blink = blink;
		if ((blink & 4u) != 0u)
			return;
		draw_object(blink);
		return;
	}
}

/* The number of cars after the place at the bottom left (/2): ends at once with one player; state 0 sets
   it up (animation 5, frame the players less one) and starts the place_number; 1 draws it, and when the
   screen's place (screen place) changes passes the new one to the place_number (its place, state 2). */
void place_count_update(void)
{
	uint8_t count_frame = 0, place = 0;
	uint32_t animation = 0, initial_place = 0, object = 0;
	uint32_t saved_count_frame = 0;
	uint8_t racer_count = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	case 1:
		initial_place = SCREEN(screen)->place;
		if (OBJECT(current_object)->place_count.place != initial_place)
		{
			OBJECT(current_object)->place_count.place = initial_place;
			object = OBJECT(current_object)->place_count.number;
			OBJECT(object)->place_number.place = initial_place;
			OBJECT(object)->state = 2u;
		}
		draw_object(initial_place);
		return;
	}
	racer_count = RACE(SCREEN(screen)->host_record)->racer_count;
	count_frame = (uint8_t)(racer_count - 1u);
	if (racer_count == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	animation = PLACE_COUNT;
	OBJECT(current_object)->palette = 0x43u;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->x = 0x2Au;
	OBJECT(current_object)->y = 0xD8u;
	saved_count_frame = count_frame;
	animation_start(animation);
	OBJECT(current_object)->frame = saved_count_frame;
	place = SCREEN(screen)->place;
	OBJECT(current_object)->place_count.place = place;
	object = start_object();
	OBJECT(object)->handler = place_number_update_ADDRESS;
	OBJECT(object)->place_number.place = place;
	OBJECT(current_object)->place_count.number = object;
	OBJECT(current_object)->state++;
	initial_place = SCREEN(screen)->place;
	if (OBJECT(current_object)->place_count.place != initial_place)
	{
		OBJECT(current_object)->place_count.place = initial_place;
		object = OBJECT(current_object)->place_count.number;
		OBJECT(object)->place_number.place = initial_place;
		OBJECT(object)->state = 2u;
	}
	draw_object(initial_place);
}

/* The screen's place, 1st or 2nd, at the bottom left: state 0 sets it up (animation 4, the place's frame
   and colour); 1 draws it; 2 a new place: twice the size and blinking for 40 frames, 4 shrinking back
   into place in 8, then 5 draws it. */
void place_number_update(void)
{
	uint32_t animation = 0, new_palette = 0, palette = 0;
	uint8_t place = 0, new_place = 0, frames = 0, blink_timer = 0, frames_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		animation = 4u;
		animation_start(animation);
		place = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = place;
		palette = 0x41u;
		if (place != 0u)
			palette = 0x42u;
		OBJECT(current_object)->palette = palette;
		OBJECT(current_object)->x = 0x1Cu;
		OBJECT(current_object)->y = 0xD8u;
		OBJECT(current_object)->state++;
		draw_object(palette);
		return;
	case 2:
		new_place = ~~OBJECT(current_object)->place_number.place;
		OBJECT(current_object)->frame = new_place;
		new_palette = 0x41u;
		if (new_place != 0u)
			new_palette = 0x42u;
		OBJECT(current_object)->palette = new_palette;
		OBJECT(current_object)->x = 0x2Cu;
		OBJECT(current_object)->y = 0xD8u;
		OBJECT(current_object)->width_scale = 0x200u;
		OBJECT(current_object)->height_scale = 0x200u;
		OBJECT(current_object)->place_number.frames = 0x28u;
		OBJECT(current_object)->place_number.blink_timer = 4u;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	case 4:
		OBJECT(current_object)->x -= 2u;
		OBJECT(current_object)->width_scale -= 0x20u;
		OBJECT(current_object)->height_scale -= 0x20u;
		frames = OBJECT(current_object)->place_number.frames;
		OBJECT(current_object)->place_number.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->draw_mode = 0u;
			OBJECT(current_object)->state++;
		}
		draw_object(new_palette);
		return;
	case 1:
	case 5:
		draw_object(0u);
		return;
	}
	blink_timer = OBJECT(current_object)->place_number.blink_timer;
	OBJECT(current_object)->place_number.blink_timer = blink_timer - 1u;
	if (blink_timer == 1u)
	{
		OBJECT(current_object)->place_number.blink_timer = 4u;
		OBJECT(current_object)->flags ^= 2u;
	}
	frames_left = OBJECT(current_object)->place_number.frames;
	OBJECT(current_object)->place_number.frames = frames_left - 1u;
	if (frames_left != 1u)
	{
		draw_object(new_palette);
		return;
	}
	OBJECT(current_object)->flags &= 0xFDu;
	OBJECT(current_object)->flags &= 0xDFu;
	OBJECT(current_object)->draw_mode = 0xAu;
	OBJECT(current_object)->place_number.frames = 8u;
	OBJECT(current_object)->state++;
	OBJECT(current_object)->x -= 2u;
	OBJECT(current_object)->width_scale -= 0x20u;
	OBJECT(current_object)->height_scale -= 0x20u;
	frames = OBJECT(current_object)->place_number.frames;
	OBJECT(current_object)->place_number.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->state++;
	}
	draw_object(new_palette);
}

/* The lap time after LAP at the top right: state 0 sets it up (the label, the text record and the
   checkpoints passed); 1 shows the race time since the last checkpoint (car race_time / 10 less car
   checkpoint_time, time_to_text); when the screen's car passes a checkpoint (screen 480 + player) the
   time it took (screen 3C0 records) starts the next lap (car checkpoint_time) and stays shown in another
   colour for 180 frames (split_frames; state 2), and C00074, which the board does not map, gets 8 + 2 *
   screen + (1 - the stage's parity); after the last (lap_timer_checkpoint_states) 3 keeps the last lap's
   time. */
void lap_timer_update(void)
{
	uint32_t animation = 0, checkpoint_time = 0, checkpoints = 0, lap_time = 0, object = 0, player = 0, text = 0, time_text = 0;
	uint16_t split_frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		animation = LAP_TIMER;
		OBJECT(current_object)->palette = 0x4Au;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xE0u;
		OBJECT(current_object)->y = 0x16u;
		animation_start(animation);
		object = current_object + 0x80u;
		OBJECT(object - 0x80u)->lap_timer.text_x = 0xF8u;
		object += 2u;
		OBJECT(object - 0x82u)->lap_timer.text_y = 0xEu;
		OBJECT(object - 0x82u)->lap_timer.text_attribute = 0x90u;
		OBJECT(current_object)->lap_timer.checkpoints = RACE(SCREEN(screen)->race_copy)->stage_count;
		OBJECT(current_object)->state++;
		return;
	case 2:
		split_frames = OBJECT(current_object)->lap_timer.split_frames;
		OBJECT(current_object)->lap_timer.split_frames = split_frames - 1u;
		if (split_frames != 1u)
		{
			text = current_object + 0x80u;
			draw_text(text);
			draw_object(lap_time);
			return;
		}
		OBJECT(current_object)->lap_timer.text_attribute = 0x90u;
		OBJECT(current_object)->state--;
	case 1:
		for (;;)
		{
			text = current_object + 0x80u;
			time_text = text + 6u;
			player = SCREEN(screen)->player;
			checkpoints = SCREEN(screen)->checkpoints_passed[(int32_t)player];
			if (OBJECT(current_object)->lap_timer.checkpoints == checkpoints)
				break;
			OBJECT(current_object)->lap_timer.checkpoints = checkpoints;
			checkpoints = (uint8_t)(checkpoints - 1u);
			OBJECT(current_object)->state = lap_timer_checkpoint_states[checkpoints];
			checkpoints = (uint8_t)(checkpoints << 3);
			lap_time = SCREEN(screen)->checkpoint_times_by_player[(uint8_t)(checkpoints + player)];
			checkpoint_time = CAR(player_car)->checkpoint_time;
			CAR(player_car)->checkpoint_time = lap_time;
			lap_time -= checkpoint_time;
			time_to_text(lap_time, time_text);
			OBJECT(current_object)->lap_timer.text_attribute = 0xB0u;
			OBJECT(current_object)->lap_timer.split_frames = 0xB4u;
			io_write8(EVENT_CODE, SCREEN(screen)->index * 2u + (~SCREEN(screen)->current_stage_level & 1u) + 8u);
			split_frames = OBJECT(current_object)->lap_timer.split_frames;
			OBJECT(current_object)->lap_timer.split_frames = split_frames - 1u;
			if (split_frames != 1u)
			{
				text = current_object + 0x80u;
				draw_text(text);
				draw_object(lap_time);
				return;
			}
			OBJECT(current_object)->lap_timer.text_attribute = 0x90u;
			OBJECT(current_object)->state--;
		}
		lap_time = CAR(player_car)->race_time / 0xAu - CAR(player_car)->checkpoint_time;
		time_to_text(lap_time, time_text);
		draw_text(text);
		draw_object(lap_time);
		return;
	case 3:
		text = current_object + 0x80u;
		draw_text(text);
		draw_object(lap_time);
		return;
	}
}

/* Unused: does nothing (a lone return after lap_timer_update's last jump to draw_object). */
void lap_timer_spare_return(void)
{
}

/* Sets up the STAGE label at the top left of the race screen (sprite set 2E), then stage_indicator_draw. */
void stage_indicator_start(void)
{
	uint32_t animation = 0;

	animation = STAGE_INDICATOR;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0x20u;
	OBJECT(current_object)->y = 0x16u;
	animation_start(animation);
	OBJECT(current_object)->handler = stage_indicator_draw_ADDRESS;
	stage_indicator_draw();
}

/* Draws the stage's name after the label (stage_level_texts by the forks taken, screen
   route_stage_level: PRE-STAGE, 1 to 4, FINAL STAGE) and the label itself. */
void stage_indicator_draw(void)
{
	uint32_t text = 0;

	text = stage_level_texts[SCREEN(screen)->route_stage_level];
	draw_text(text);
	draw_object(0u);
}

/* The start's messages in the middle of the screen: state 0 shows YOU'RE IN CAR NUMBER n! (animation 28F
   + the screen's car number, screen car_number, with DJ message 21 + it); when the start's countdown
   (the car object's timer) is at 30 frames, Ready (animation 31, DJ message 31); at 0, Go! (animation
   32, DJ message 32), 30 frames, then it ends. */
void start_message_update(void)
{
	uint32_t animation = 0, dj_message = 0;
	uint8_t frames = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x80u;
		animation = SCREEN(screen)->car_number;
		animation = (uint16_t)(animation + 0x28Fu);
		animation_start(animation);
		OBJECT(current_object)->palette = CAR(player_car)->model + 0x289u;
		dj_message = (uint8_t)(0x21u + SCREEN(screen)->car_number);
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			draw_object(dj_message);
			return;
		}
		animation = 0x31u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		dj_message = 0x31u;
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		break;
	case 1:
		if (OBJECT(CAR(player_car)->object)->player_car.timer != 0x1Eu)
		{
			draw_object(dj_message);
			return;
		}
		animation = 0x31u;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		dj_message = 0x31u;
		start_dj_message((uint8_t)dj_message);
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	case 3:
		frames = OBJECT(current_object)->start_message.frames;
		OBJECT(current_object)->start_message.frames = frames - 1u;
		if (frames == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(dj_message);
		return;
	}
	if (OBJECT(CAR(player_car)->object)->player_car.timer != 0u)
	{
		draw_object(dj_message);
		return;
	}
	animation = 0x32u;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x4Bu;
	OBJECT(current_object)->start_message.frames = 0x1Eu;
	dj_message = 0x32u;
	start_dj_message((uint8_t)dj_message);
	OBJECT(current_object)->state++;
	frames = OBJECT(current_object)->start_message.frames;
	OBJECT(current_object)->start_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(dj_message);
}

/* The time left in seconds at the top centre, and the time up: state 0 waits for the race's time
   (host_record's time_left), takes it into car time_left and sets the digits up (time_digits_start); 1
   shows it during the start's countdown; 2 follows the race's time (until the screen's car has finished,
   race_copy finished) and starts EXTEND TIME at each checkpoint (time_left_display_check_extension); at
   0 the time is up (car time_up) and 3 shows 0, until time comes back (the car drives on: its state 2).
   The digits flip as they change (time_digits_draw). */
void time_left_display_update(void)
{
	uint32_t shown_time = 0, time_left = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		time_left = RACE(SCREEN(screen)->host_record)->time_left;
		if (time_left == 0u)
			return;
		CAR(player_car)->time_left = time_left;
		OBJECT(current_object)->state++;
		time_digits_start();
		return;
	case 1:
		if (OBJECT(CAR(player_car)->object)->state != 1u)
		{
			OBJECT(current_object)->state++;
			time_left_display_check_extension();
			if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
				CAR(player_car)->time_left = RACE(SCREEN(screen)->host_record)->time_left;
		}
		break;
	case 2:
		time_left_display_check_extension();
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
			CAR(player_car)->time_left = RACE(SCREEN(screen)->host_record)->time_left;
		break;
	case 3:
		time_left_display_check_extension();
		shown_time = RACE(SCREEN(screen)->host_record)->time_left;
		if (shown_time != 0u)
		{
			CAR(player_car)->time_left = shown_time;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->player_car.state_high = 0u;
		}
		shown_time = binary_to_bcd(shown_time);
		time_digits_draw(shown_time);
		return;
	}
	shown_time = CAR(player_car)->time_left;
	if (shown_time == 0u)
	{
		OBJECT(current_object)->state++;
		CAR(player_car)->time_up = 0xFFu;
		time_left_display_check_extension();
		shown_time = RACE(SCREEN(screen)->host_record)->time_left;
		if (shown_time != 0u)
		{
			CAR(player_car)->time_left = shown_time;
			OBJECT(current_object)->state--;
			CAR(player_car)->time_up = 0u;
			OBJECT(CAR(player_car)->object)->state = 2u;
			OBJECT(CAR(player_car)->object)->player_car.step = 0u;
			OBJECT(CAR(player_car)->object)->player_car.state_high = 0u;
		}
		shown_time = binary_to_bcd(shown_time);
		time_digits_draw(shown_time);
		return;
	}
	shown_time = shown_time / 0x1Eu;
	shown_time = binary_to_bcd(shown_time);
	time_digits_draw(shown_time);
}

/* Sets up the three digits of the time at the top centre (animation 3 at A9, 11), each to open from
   nothing at its first value. Used by time_left_display, car_select_run and 001EC2. */
void time_digits_start(void)
{
	uint32_t animation = 0, object = 0;

	object = current_object + 0x80u;
	OBJECT(object - 0x80u)->time_digits.units_flip = 1;
	OBJECT(object - 0x80u)->time_digits.units_digit = 0u;
	OBJECT(object - 0x80u)->time_digits.units_width = 0x40u;
	object += 4u;
	OBJECT(object - 0x84u)->time_digits.units_remaining = 0u;
	object += 4u;
	OBJECT(object - 0x88u)->time_digits.tens_flip = 1;
	OBJECT(object - 0x88u)->time_digits.tens_digit = 0u;
	OBJECT(object - 0x88u)->time_digits.tens_width = 0x40u;
	object += 4u;
	OBJECT(object - 0x8Cu)->time_digits.tens_remaining = 0u;
	object += 4u;
	OBJECT(object - 0x90u)->time_digits.hundreds_flip = 1;
	OBJECT(object - 0x90u)->time_digits.hundreds_digit = 0u;
	OBJECT(object - 0x90u)->time_digits.hundreds_width = 0x40u;
	OBJECT(object - 0x90u)->time_digits.hundreds_remaining = 0u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 0u;
	animation = TIME_DIGITS;
	OBJECT(current_object)->palette = 0x40u;
	OBJECT(current_object)->time_digits.units_x = 0xA9u;
	OBJECT(current_object)->y = 0x11u;
	animation_start(animation);
}

/* Draws the three digits of R0 (binary-coded decimal), the units at the right: a digit that changes
   squeezes shut and opens again as the new one; leading zeros are left out. */
void time_digits_draw(uint32_t digits)
{
	uint32_t negative = 0;
	uint8_t digit = 0, shown_digit = 0;
	uint16_t flip_width = 0;
	uint32_t digit_record = 0, digits_left = 0, remaining_digits = 0;
	uint32_t saved_digits = 0, saved_digit_record = 0, saved_digits_left = 0;
	uint8_t flip = 0;
	uint16_t closing_width = 0;

	OBJECT(current_object)->x = OBJECT(current_object)->time_digits.units_x;
	digit_record = current_object + 0x80u;
	digits_left = 3u;
	for (;;)
	{
		digit = digits & 0xFu;
		flip = TIME_DIGIT(digit_record)->flip;
		negative = flip >> 7;
		digit_record++;
		shown_digit = TIME_DIGIT(digit_record - 0x1u)->digit;
		digit_record++;
		flip_width = TIME_DIGIT(digit_record - 0x2u)->width;
		digit_record += 6u;
		remaining_digits = digits;
		if (flip != 0u)
		{
			if (negative == 0u)
			{
				remaining_digits = TIME_DIGIT(digit_record - 0x8u)->remaining;
				closing_width = flip_width;
				flip_width = (uint16_t)(closing_width - 0x40u);
				TIME_DIGIT(digit_record - 0x8u)->width = flip_width;
				if (closing_width == 0x40u)
				{
					TIME_DIGIT(digit_record - 0x8u)->flip = -1;
					TIME_DIGIT(digit_record - 0x8u)->digit = digit;
					TIME_DIGIT(digit_record - 0x8u)->remaining = digits;
					digits = digits >> 4;
					OBJECT(current_object)->x -= 0x12u;
					digits_left--;
					if (digits_left == 0u)
						break;
					continue;
				}
			}
			else
			{
				flip_width += 0x40u;
				TIME_DIGIT(digit_record - 0x8u)->width = flip_width;
				if ((uint8_t)flip_width == 0u)
					TIME_DIGIT(digit_record - 0x8u)->flip = 0;
			}
		}
		else if (shown_digit != digit)
		{
			TIME_DIGIT(digit_record - 0x8u)->flip = 1;
			remaining_digits = TIME_DIGIT(digit_record - 0x8u)->remaining;
			closing_width = flip_width;
			flip_width = (uint16_t)(closing_width - 0x40u);
			TIME_DIGIT(digit_record - 0x8u)->width = flip_width;
			if (closing_width == 0x40u)
			{
				TIME_DIGIT(digit_record - 0x8u)->flip = -1;
				TIME_DIGIT(digit_record - 0x8u)->digit = digit;
				TIME_DIGIT(digit_record - 0x8u)->remaining = digits;
				digits = digits >> 4;
				OBJECT(current_object)->x -= 0x12u;
				digits_left--;
				if (digits_left == 0u)
					break;
				continue;
			}
		}
		if (digits_left == 3u || remaining_digits != 0u)
		{
			OBJECT(current_object)->frame = shown_digit;
			OBJECT(current_object)->width_scale = flip_width;
			saved_digits = digits;
			saved_digit_record = digit_record;
			saved_digits_left = digits_left;
			draw_object(digits);
			digits_left = saved_digits_left;
			digit_record = saved_digit_record;
			digits = saved_digits;
		}
		digits = digits >> 4;
		OBJECT(current_object)->x -= 0x12u;
		digits_left--;
		if (digits_left == 0u)
			break;
	}
}

/* Starts the EXTEND TIME message once for each checkpoint passed (screen forks_decided, counted in car
   extensions_shown). */
void time_left_display_check_extension(void)
{
	uint32_t object = 0;

	if (CAR(player_car)->extensions_shown < SCREEN(screen)->forks_decided)
	{
		CAR(player_car)->extensions_shown = SCREEN(screen)->forks_decided;
		object = start_object();
		OBJECT(object)->handler = extend_time_message_start_ADDRESS;
	}
}

/* Sets up the EXTEND TIME message under the time (animation 33) for 90 frames, blinks one of the
   screen's cabinet lamps 16 times (bytes C1-C3 of its entry at checkpoint_lamp), then
   extend_time_message_blink. */
void extend_time_message_start(void)
{
	uint32_t animation = 0, lamp = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Fu;
	animation = EXTEND_TIME;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x3Fu;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x30u;
	OBJECT(current_object)->extend_time_message.frames = 0x5Au;
	OBJECT(current_object)->extend_time_message.blink = 0u;
	OBJECT(current_object)->handler = extend_time_message_blink_ADDRESS;
	lamp = GAME_VARIABLES + SCREEN(screen)->index * 4u;
	SCREEN_LAMPS(lamp)->checkpoint_timer = 1u;
	SCREEN_LAMPS(lamp)->checkpoint_period = 0x10u;
	SCREEN_LAMPS(lamp)->checkpoint_blinks = 0x10u;
	extend_time_message_blink();
}

/* Blinks EXTEND TIME (4 frames on, 4 off) until its frames run out. */
void extend_time_message_blink(void)
{
	uint32_t blink = 0;
	uint8_t frames = 0;

	frames = OBJECT(current_object)->extend_time_message.frames;
	OBJECT(current_object)->extend_time_message.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	blink = (uint8_t)(OBJECT(current_object)->extend_time_message.blink + 1u);
	OBJECT(current_object)->extend_time_message.blink = blink;
	if ((blink & 4u) != 0u)
		return;
	draw_object(blink);
}

/* The start of a stage's road (position 0 of streams 1-30): when the car (car segment) reaches it, the
   stage the car is in becomes the one whose road is being built (screen current_stage_level =
   built_stage_level, its level; current_stage = built_stage, its number; car reached_stage_start =
   stage_start, the segment it starts at), and it ends. In the attract demo (screen demo_flags bit 8) it
   ends at once. */
void stage_entry_update(void)
{
	switch (OBJECT(current_object)->state)
	{
	case 0:
		break;
	}
	if ((SCREEN(screen)->demo_flags & 0x100u) != 0u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if (OBJECT(current_object)->position <= (int16_t)CAR(player_car)->segment)
	{
		SCREEN(screen)->current_stage_level = SCREEN(screen)->built_stage_level;
		SCREEN(screen)->current_stage = SCREEN(screen)->built_stage;
		CAR(player_car)->reached_stage_start = CAR(player_car)->stage_start;
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
}

/* Unused: does nothing (a second return after stage_entry_update's). */
void stage_entry_spare_return(void)
{
}

/* The goal line and what follows it: state 0 waits until the screen's car reaches its segment
   (position), then marks the car at the goal (car at_goal); 1 when the car's finish is recorded (race
   finished) sets up Congratulations! (animation 2F5) and the TOTAL TIME (the text at D08E and the
   screen's finish time, screen 410, through time_to_text), DJ message 34 when first, else 35, the
   screen's sound byte to 7F; 2 waits; 3 after 120 frames draws them for 60, then the title Dream Flying
   (start_song_title) and sound 10; 4 240 more frames of both, or with two players 5 only the TOTAL TIME
   beside the race_result; then it ends. */
void goal_update(void)
{
	uint8_t dj_message = 0;
	uint16_t sound_code = 0;
	uint32_t animation = 0, finish_time = 0, object = 0, result_object = 0, song_title = 0, text = 0;
	uint16_t timer = 0, title_timer = 0, result_timer = 0, show_timer = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		if ((int16_t)CAR(player_car)->segment < OBJECT(current_object)->position)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->state++;
	case 1:
		if (RACE(SCREEN(screen)->race)->finished == 0u)
			return;
		CAR(player_car)->at_goal = 0xFFu;
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Cu;
		animation = CONGRATULATIONS;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x4Cu;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x90u;
		object = current_object + 0x80u;
		OBJECT(object - 0x80u)->goal.text_x = 0xB0u;
		object += 2u;
		OBJECT(object - 0x82u)->goal.text_y = 0x30u;
		object += 2u;
		OBJECT(object - 0x84u)->goal.text_attribute = 0x90u;
		finish_time = SCREEN(screen)->checkpoint_times_by_player[SCREEN(screen)->player + 0x28u];
		object += 2u;
		time_to_text(finish_time, object);
		dj_message = 0x34u;
		if (SCREEN(screen)->place != 0u)
			dj_message = 0x35u;
		start_dj_message(dj_message);
		sound_code = 0x7Fu;
		fade_out_music_at((uint8_t)sound_code);
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer - 1u;
		if (timer != 1u)
			return;
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = timer - 1u;
		if (timer != 1u)
			return;
		OBJECT(current_object)->goal.timer = 0x3Cu;
		OBJECT(current_object)->state++;
		break;
	case 3:
		break;
	case 4:
		title_timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = title_timer - 1u;
		if (title_timer == 1u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		draw_object(sound_code);
		text = total_time_text_ADDRESS;
		draw_text(text);
		text = current_object + 0x80u;
		draw_text(text);
		return;
	case 5:
		result_timer = OBJECT(current_object)->goal.timer;
		OBJECT(current_object)->goal.timer = result_timer - 1u;
		if (result_timer != 1u)
		{
			text = total_time_text_ADDRESS;
			draw_text(text);
			text = current_object + 0x80u;
			draw_text(text);
			return;
		}
		OBJECT(current_object)->flags = 0u;
		return;
	}
	show_timer = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = show_timer - 1u;
	if (show_timer != 1u)
	{
		draw_object(sound_code);
		text = total_time_text_ADDRESS;
		draw_text(text);
		text = current_object + 0x80u;
		draw_text(text);
		return;
	}
	song_title = dream_flying_song_title_ADDRESS;
	start_song_title(song_title);
	sound_code = 0x10u;
	request_sound((uint8_t)sound_code);
	if (RACE(SCREEN(screen)->host_record)->racer_count != 1u)
	{
		result_object = start_object();
		OBJECT(result_object)->handler = race_result_start_ADDRESS;
		OBJECT(current_object)->state++;
	}
	OBJECT(current_object)->goal.timer = 0xF0u;
	OBJECT(current_object)->state++;
	title_timer = OBJECT(current_object)->goal.timer;
	OBJECT(current_object)->goal.timer = title_timer - 1u;
	if (title_timer == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(sound_code);
	text = total_time_text_ADDRESS;
	draw_text(text);
	text = current_object + 0x80u;
	draw_text(text);
}

/* With two players, the result at the end of the race: the screen's place in big letters (animation 2F8:
   1st, 2nd) at the bottom for 240 frames (race_result_hold), and the verdict above it
   (race_result_verdict_start). */
void race_result_start(void)
{
	uint16_t palette = 0;
	uint32_t animation = 0, object = 0;
	uint8_t place = 0;

	object = start_object();
	OBJECT(object)->handler = race_result_verdict_start_ADDRESS;
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	animation = RACE_PLACES;
	animation_start(animation);
	place = ~~SCREEN(screen)->place;
	OBJECT(current_object)->frame = place;
	palette = 0x41u;
	if (place != 0u)
		palette = 0x42u;
	OBJECT(current_object)->palette = palette;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0xC8u;
	OBJECT(current_object)->race_result.frames = 0xF0u;
	OBJECT(current_object)->handler = race_result_hold_ADDRESS;
	race_result_hold();
}

/* Draws the place or Loser until its frames run out. */
void race_result_hold(void)
{
	uint16_t frames = 0;

	frames = OBJECT(current_object)->race_result.frames;
	OBJECT(current_object)->race_result.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	draw_object(0u);
}

/* The verdict over the place: Winner (animation 2F6), blinking (race_result_verdict_blink), for the
   first, Loser (2F7) for the other, 240 frames. */
void race_result_verdict_start(void)
{
	uint32_t animation = 0;
	uint8_t place = 0;

	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Cu;
	place = ~~SCREEN(screen)->place;
	OBJECT(current_object)->frame = place;
	OBJECT(current_object)->x = 0xA0u;
	OBJECT(current_object)->y = 0x70u;
	if (place != 0u)
	{
		animation = LOSER;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x42u;
		OBJECT(current_object)->race_result.frames = 0xF0u;
		OBJECT(current_object)->handler = race_result_hold_ADDRESS;
		race_result_hold();
		return;
	}
	animation = WINNER;
	animation_start(animation);
	OBJECT(current_object)->palette = 0x41u;
	OBJECT(current_object)->race_result.frames = 0xF0u;
	OBJECT(current_object)->handler = race_result_verdict_blink_ADDRESS;
	race_result_verdict_blink();
}

/* Blinks Winner (4 frames on, 4 off) until its frames run out. */
void race_result_verdict_blink(void)
{
	uint16_t frames = 0;

	frames = OBJECT(current_object)->race_result.frames;
	OBJECT(current_object)->race_result.frames = frames - 1u;
	if (frames == 1u)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	if ((OBJECT(current_object)->race_result.frames & 4u) == 0u)
		return;
	draw_object(0u);
}

/* The arrow of the way taken at a fork, at the bottom centre, with CHECK POINT scrolling on the LCD
   between arrows of that way: state 0 sets it up (animation 34 or 35 by the branch, car branch; DJ
   message 33 when first, else 3A, unless one is playing) for 57 segments of the race (leave_segment); 1
   cycles its colours (palette_list); 2 then grows and drops out of the screen. Ends when the race is
   over. */
void branch_arrow_update(void)
{
	uint8_t dj_message = 0;
	uint16_t new_scale = 0, new_y = 0;
	uint32_t animation = 0, lcd_text = 0, palette = 0, palette_list = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		dj_message = 0x3Au;
		if (SCREEN(screen)->place == 0u)
			dj_message = 0x33u;
		start_dj_message_if_quiet(dj_message);
		OBJECT(current_object)->draw_mode = 8u;
		OBJECT(current_object)->depth_layer = 0x19Du;
		lcd_text = check_point_lcd_text_branch_0_ADDRESS;
		animation = BRANCH_ARROW_LEFT;
		if (CAR(player_car)->branch != 0u)
		{
			lcd_text = check_point_lcd_text_branch_1_ADDRESS;
			animation = BRANCH_ARROW_RIGHT;
		}
		SCREEN(screen)->lcd_scroll_request = lcd_text;
		animation_start(animation);
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0xD0u;
		OBJECT(current_object)->branch_arrow.palette_list = branch_arrow_palette_steps_ADDRESS;
		OBJECT(current_object)->branch_arrow.leave_segment = RACE(SCREEN(screen)->race_copy)->segment + 0x39u;
		OBJECT(current_object)->state++;
	case 1:
		if (CAR(player_car)->race_over != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((int16_t)RACE(SCREEN(screen)->race_copy)->segment < (int16_t)OBJECT(current_object)->branch_arrow.leave_segment)
		{
			palette_list = OBJECT(current_object)->branch_arrow.palette_list;
			palette = OBJECT_PALETTE_STEPS(palette_list)[0];
			palette_list += 2u;
			if ((int16_t)palette < 0)
			{
				palette_list += palette;
				palette = (uint16_t)OBJECT_PALETTE_STEPS(palette_list)[0];
				palette_list += 2u;
			}
			OBJECT(current_object)->palette = palette;
			OBJECT(current_object)->branch_arrow.palette_list = palette_list;
			draw_object(palette);
			return;
		}
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	}
	new_y = (uint16_t)(OBJECT(current_object)->y + 3u);
	if ((int16_t)new_y > 0x140)
	{
		OBJECT(current_object)->flags = 0u;
		return;
	}
	OBJECT(current_object)->y = new_y;
	new_scale = (uint16_t)(OBJECT(current_object)->width_scale + 6u);
	OBJECT(current_object)->width_scale = new_scale;
	OBJECT(current_object)->height_scale = new_scale;
	palette_list = OBJECT(current_object)->branch_arrow.palette_list;
	palette = OBJECT_PALETTE_STEPS(palette_list)[0];
	palette_list += 2u;
	if ((int16_t)palette < 0)
	{
		palette_list += palette;
		palette = (uint16_t)OBJECT_PALETTE_STEPS(palette_list)[0];
		palette_list += 2u;
	}
	OBJECT(current_object)->palette = palette;
	OBJECT(current_object)->branch_arrow.palette_list = palette_list;
	draw_object(palette);
}

/* The times at a checkpoint, in rows under the top line: state 0 takes each player's car image
   (checkpoint_board_car_sprites by the car chosen); 1 waits until the screen's car passes a checkpoint
   (screen 480 + player); 2 then shows a row for each car that has passed it, in the order they did
   (screen 390): the first with its time, the others with how far behind the first they were (BEHIND);
   each row slides in from the right, the screen's own blinks while it waits for the others, and all
   slide out once the race has gone C0 segments on (leave_segment) or is over; when the last has gone,
   back to 1. */
void checkpoint_board_update(void)
{
	uint8_t checkpoints_passed = 0;
	uint16_t row_offset = 0;
	uint32_t blink_on = 0, checkpoint = 0, checkpoint_count = 0, leave_segment = 0, object = 0, player = 0, race = 0, racer = 0, racers_left = 0, row_player = 0, row_time = 0, screen_player = 0;
	uint8_t blink_timer = 0, rows_left = 0;
	uint16_t gap = 0, offset_before = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		race = SCREEN(screen)->host_record + 0x1Fu;
		racers_left = RACE(race - 0x1Fu)->racer_count;
		race++;
		do
		{
			racer = RACE_ORDER_PLAYERS(race)[0];
			OBJECT(current_object)->checkpoint_board.car_sprites[(int32_t)racer] = checkpoint_board_car_sprites[RACE(RACE_COPY_1)[racer].model];
			race++;
			racers_left--;
		} while (racers_left != 0u);
		checkpoint_count = screen + 0x480u + SCREEN(screen)->player;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = CHECKPOINT_COUNTS(checkpoint_count)[0];
		OBJECT(current_object)->checkpoint_board.checkpoint_count = checkpoint_count;
		OBJECT(current_object)->checkpoint_board.rows_shown = 0u;
		OBJECT(current_object)->state++;
	case 1:
		checkpoints_passed = CHECKPOINT_COUNTS(OBJECT(current_object)->checkpoint_board.checkpoint_count)[0];
		if (OBJECT(current_object)->checkpoint_board.checkpoints_seen == checkpoints_passed)
			return;
		OBJECT(current_object)->checkpoint_board.checkpoints_seen = checkpoints_passed;
		OBJECT(current_object)->checkpoint_board.checkpoint = checkpoints_passed - 1u;
		OBJECT(current_object)->checkpoint_board.leave_segment = CAR(player_car)->segment + 0xC0u;
		OBJECT(current_object)->state++;
		break;
	case 2:
		break;
	}
	checkpoint = OBJECT(current_object)->checkpoint_board.checkpoint;
	OBJECT(current_object)->checkpoint_board.row = 0u;
	OBJECT(current_object)->checkpoint_board.rows_left = SCREEN(screen)->checkpoint_passes[(int32_t)checkpoint];
	OBJECT(current_object)->checkpoint_board.passing_order = screen + 0x390u + checkpoint * 2u * 4u;
	do
	{
		row_player = CHECKPOINT_PASS(OBJECT(current_object)->checkpoint_board.passing_order)->player;
		switch (OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player])
		{
		case 0:
			OBJECT(current_object)->checkpoint_board.rows_shown++;
			row_time = SCREEN(screen)->checkpoint_times_by_player[(int32_t)(((uint32_t)OBJECT(current_object)->checkpoint_board.checkpoint << 3) + row_player)];
			player = row_player;
			if (OBJECT(current_object)->checkpoint_board.row != 0u)
			{
				gap = row_time - OBJECT(current_object)->checkpoint_board.leader_time;
				row_time = gap;
				if ((int16_t)gap < 0)
					row_time = 0u;
				checkpoint_board_gap_time(row_time, player);
			}
			else
			{
				OBJECT(current_object)->checkpoint_board.leader_time = row_time;
				checkpoint_board_leader_time(row_time, player);
			}
			screen_player = SCREEN(screen)->player;
			if (screen_player == row_player)
			{
				OBJECT(current_object)->checkpoint_board.blink_on = 4u;
				OBJECT(current_object)->checkpoint_board.blink_timer = 8u;
			}
			else if (OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)screen_player] == 0u)
			{
				OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] = 0u;
				OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player] = 2u;
				if (OBJECT(current_object)->checkpoint_board.leave_segment != 0u && CAR(player_car)->race_over == 0u)
				{
					if (SCREEN(screen)->player != row_player)
					{
						checkpoint_board_draw_row(row_player);
						break;
					}
					blink_on = OBJECT(current_object)->checkpoint_board.blink_on;
					blink_timer = OBJECT(current_object)->checkpoint_board.blink_timer;
					OBJECT(current_object)->checkpoint_board.blink_timer = blink_timer - 1u;
					if (blink_timer == 1u)
					{
						blink_on = blink_on ^ 4u;
						OBJECT(current_object)->checkpoint_board.blink_on = blink_on;
						OBJECT(current_object)->checkpoint_board.blink_timer = 4u + blink_on;
					}
					if (blink_on == 0u)
						break;
					checkpoint_board_draw_row(row_player);
					break;
				}
				OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
				row_offset = (uint16_t)(OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] - 0x10u);
				OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] = row_offset;
				if (row_offset != 0xFF00u)
				{
					checkpoint_board_draw_row(row_player);
					break;
				}
				OBJECT(current_object)->checkpoint_board.rows_shown--;
				OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
				checkpoint_board_draw_row(row_player);
				break;
			}
			OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] = 0x100u;
			OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
		case 1:
			offset_before = OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player];
			OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] = offset_before - 0x10u;
			if (offset_before != 0x10u)
			{
				checkpoint_board_draw_row(row_player);
				break;
			}
			OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
		case 2:
			if (OBJECT(current_object)->checkpoint_board.leave_segment != 0u && CAR(player_car)->race_over == 0u)
			{
				if (SCREEN(screen)->player != row_player)
				{
					checkpoint_board_draw_row(row_player);
					break;
				}
				blink_on = OBJECT(current_object)->checkpoint_board.blink_on;
				blink_timer = OBJECT(current_object)->checkpoint_board.blink_timer;
				OBJECT(current_object)->checkpoint_board.blink_timer = blink_timer - 1u;
				if (blink_timer == 1u)
				{
					blink_on = blink_on ^ 4u;
					OBJECT(current_object)->checkpoint_board.blink_on = blink_on;
					OBJECT(current_object)->checkpoint_board.blink_timer = 4u + blink_on;
				}
				if (blink_on == 0u)
					break;
				checkpoint_board_draw_row(row_player);
				break;
			}
			OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
		case 3:
			row_offset = (uint16_t)(OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] - 0x10u);
			OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)row_player] = row_offset;
			if (row_offset != 0xFF00u)
			{
				checkpoint_board_draw_row(row_player);
				break;
			}
			OBJECT(current_object)->checkpoint_board.rows_shown--;
			OBJECT(current_object)->checkpoint_board.row_steps[(int32_t)row_player]++;
			checkpoint_board_draw_row(row_player);
			break;
		case 4:
			break;
		}
		OBJECT(current_object)->checkpoint_board.passing_order++;
		OBJECT(current_object)->checkpoint_board.row++;
		rows_left = OBJECT(current_object)->checkpoint_board.rows_left;
		OBJECT(current_object)->checkpoint_board.rows_left = rows_left - 1u;
	} while (rows_left != 1u);
	leave_segment = OBJECT(current_object)->checkpoint_board.leave_segment;
	if (leave_segment != 0u && (int16_t)leave_segment <= (int16_t)RACE(SCREEN(screen)->race_copy)->segment)
		OBJECT(current_object)->checkpoint_board.leave_segment = 0u;
	if (OBJECT(current_object)->checkpoint_board.rows_shown == 0u)
	{
		object = current_object + 0xF0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[0] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[1] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[2] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[3] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[4] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[5] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[6] = 0u;
		OBJECT(object - 0xF0u)->checkpoint_board.row_steps[7] = 0u;
		OBJECT(current_object)->state--;
	}
}

/* Draws row of the board for player R21, slid by its offset: the car number, the car's small image, the
   time text (draw_text_at; another colour after the first row) and, after the first, BEHIND. */
void checkpoint_board_draw_row(uint32_t player)
{
	uint16_t row_x = 0, text_attribute = 0;
	uint32_t animation = 0, car_sprite = 0, place = 0, row = 0, row_x_2 = 0, text_x = 0, time_text = 0;

	place = checkpoint_board_number_places_ADDRESS + OBJECT(current_object)->checkpoint_board.row * 4u;
	OBJECT(current_object)->x = CHECKPOINT_BOARD_PLACE(place)->x + OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)player];
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	animation = checkpoint_board_number_animations[game->car_numbers[player]];
	OBJECT(current_object)->palette = 0x27u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(animation);
	draw_object(0u);
	place = checkpoint_board_car_places_ADDRESS + OBJECT(current_object)->checkpoint_board.row * 4u;
	OBJECT(current_object)->x = CHECKPOINT_BOARD_PLACE(place)->x + OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)player];
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	car_sprite = current_object + 0x80u + player * 4u;
	animation = BOARD_CAR_SPRITE(car_sprite)->animation;
	OBJECT(current_object)->palette = BOARD_CAR_SPRITE(car_sprite)->palette;
	OBJECT(current_object)->depth_layer = 0x19Du;
	OBJECT(current_object)->draw_mode = 0u;
	animation_start(animation);
	OBJECT(current_object)->frame = 4u;
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	draw_object(row_x);
	row = OBJECT(current_object)->checkpoint_board.row;
	text_x = checkpoint_board_time_places[row];
	text_x = (text_x & ~0xFFFFu) | (uint16_t)(text_x + OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)player]);
	text_attribute = 0xC0u;
	if (row != 0u)
		text_attribute = 0x90u;
	time_text = current_object + 0xB0u + player * 2u * 4u;
	draw_text_at(text_x, text_attribute, time_text);
	row_x_2 = OBJECT(current_object)->checkpoint_board.row;
	if (row_x_2 == 0u)
		return;
	place = checkpoint_board_behind_places_ADDRESS + row_x_2 * 4u;
	OBJECT(current_object)->x = CHECKPOINT_BOARD_PLACE(place)->x + OBJECT(current_object)->checkpoint_board.row_offsets[(int32_t)player];
	OBJECT(current_object)->y = CHECKPOINT_BOARD_PLACE(place)->y;
	animation = CHECKPOINT_BOARD_ROW;
	OBJECT(current_object)->palette = 0x4Au;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->draw_mode = 8u;
	animation_start(animation);
	draw_object(row_x_2);
}

/* Writes the first car's time R0 as M'SS"HH into player R1's row text. */
void checkpoint_board_leader_time(uint32_t time, uint32_t player)
{
	uint32_t time_text = 0;

	time_text = current_object + 0xB0u + player * 2u * 4u;
	time_to_text(time, time_text);
}

/* Unused: time_to_text for a time in thousandths of a second: R0 / 10 (into hundredths), then
   time_to_text at R10 (it runs on into it). */
void time_to_text_thousandths(uint32_t time, uint32_t text)
{
	time = time / 0xAu;
	time_to_text(time, text);
}

/* Writes the time R0 (hundredths of a second) as M'SS"HH, closed by 0, at R10. Used by lap_timer, goal,
   checkpoint_board, 001EC2 and 00270C. */
void time_to_text(uint32_t time, uint32_t text)
{
	uint32_t digit = 0;

	text += 7u;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0x22u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(6u, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0x27u;
	OBJECT_TEXT_CHARACTERS(text)[-1] = time + 0x30u;
}

/* Writes how far behind the first player R1 passed (R0, hundredths) into its row text, as time_to_text
   does but with spaces for zero minutes and zero tens of seconds. */
void checkpoint_board_gap_time(uint32_t time, uint32_t player)
{
	uint32_t zero = 0;
	uint8_t zero_char = 0;
	uint32_t digit = 0, text = 0;

	text = current_object + 0xB7u + player * 2u * 4u;
	zero_char = 0x30u;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + 0x30u;
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0x22u;
	digit = 0u;
	divx_pair(0xAu, &time, &digit);
	zero = (uint32_t)(time == 0u);
	if (zero == 0u)
	{
		text--;
		OBJECT_TEXT_CHARACTERS(text)[0] = digit + zero_char;
		digit = 0u;
		divx_pair(6u, &time, &digit);
		zero = (uint32_t)(time == 0u);
		if (zero)
		{
			text--;
			OBJECT_TEXT_CHARACTERS(text)[0] = digit + zero_char;
			AS16(OBJECT_TEXT_CHARACTERS(text)[-2]) = 0x2020u;
			return;
		}
		text--;
		OBJECT_TEXT_CHARACTERS(text)[0] = digit + zero_char;
		text--;
		OBJECT_TEXT_CHARACTERS(text)[0] = 0x27u;
		OBJECT_TEXT_CHARACTERS(text)[-1] = time + zero_char;
		return;
	}
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = digit + zero_char;
	text--;
	OBJECT_TEXT_CHARACTERS(text)[0] = 0x20u;
	AS16(OBJECT_TEXT_CHARACTERS(text)[-2]) = 0x2020u;
}
