/**
 *    Name:        race/physics.c
 *    Description: the car's physics: the gears and torque curves, the controls, driving along the road, the engine, the road's drag and its edges, the wheels' effects
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Loads the car's model and transmission: its handling record (car handling), its torque curves
   (torque_curves), its gear ratios (unk_54) and top gear (top_gear), the first gear's ratio (unk_64),
   and the roadside surfaces of the first stage (player_car_next_surfaces). */
void player_car_load_gears(void)
{
	uint32_t gear_list = 0, model = 0, model_transmission = 0;

	model = CAR(player_car)->model;
	r12 = car_handlings[model];
	model_transmission = CAR(player_car)->transmission + model * 2u;
	CAR(player_car)->torque_curves = car_torque_curves[model_transmission];
	gear_list = car_gear_lists[model_transmission];
	CAR(player_car)->top_gear = GEAR_LIST(gear_list)->top_gear;
	gear_list++;
	CAR(player_car)->gear_ratios = gear_list;
	CAR(player_car)->gear_ratio = GEAR_LIST(gear_list - 0x1u)->first_ratio;
	CAR(player_car)->handling = r12;
	player_car_next_surfaces();
}

/* Builds the drive force of each gear by speed at screen + 1300 (256 values a gear), interpolating the
   model's points (car torque_curves). */
void player_car_build_torque_curves(void)
{
	uint32_t curve_entry = 0, curve_force = 0, force_step = 0, gears_left = 0, speed = 0, steps_left = 0, torque_point = 0;
	uint16_t next_speed = 0, next_force = 0;

	curve_entry = screen + 0x1300u;
	torque_point = CAR(player_car)->torque_curves;
	gears_left = CAR(player_car)->top_gear;
	gears_left = (uint8_t)(gears_left + 1u);
	do
	{
		r12 = torque_point + 4u;
		do
		{
			speed = (uint16_t)TORQUE_POINT(torque_point)->speed;
			torque_point += 2u;
			next_speed = TORQUE_POINT(r12)->speed;
			r12 += 2u;
			next_force = TORQUE_POINT(r12 - 0x2u)->force;
			r12 += 2u;
			curve_force = (uint32_t)TORQUE_POINT(torque_point - 0x2u)->force << 16;
			steps_left = next_speed - speed;
			force_step = divide_s32(((uint32_t)next_force << 16) - curve_force, steps_left);
			torque_point += 2u;
			do
			{
				TORQUE_CURVE_FORCES(curve_entry)[0] = curve_force >> 16;
				curve_force += force_step;
				steps_left--;
				curve_entry += 2u;
			} while (steps_left != 0u);
		} while (TORQUE_POINT(r12)->speed >= 0);
		torque_point = r12 + 4u;
		gears_left--;
	} while (gears_left != 0u);
}

/* Takes the controls: gas, brake and wheel from the screen's pedals and wheel (screen gas, brake,
   wheel). In the demo (screen demo_flags bit 8) the car drives itself: no brake, the gas up to F0 (eased
   off while skidding), the wheel back toward the middle when off the road or slowed, else against its
   sideways drift. */
void player_car_read_controls(void)
{
	uint8_t gas = 0, wheel = 0;

	if ((SCREEN(screen)->demo_flags & 0x100u) == 0u)
	{
		OBJECT(current_object)->player_car.gas = SCREEN(screen)->gas;
		OBJECT(current_object)->player_car.brake = SCREEN(screen)->brake;
		OBJECT(current_object)->player_car.wheel = SCREEN(screen)->wheel;
		return;
	}
	OBJECT(current_object)->player_car.brake = 0u;
	wheel = OBJECT(current_object)->player_car.wheel;
	if (CAR(player_car)->slowed_frames != 0u)
	{
		if (CAR(player_car)->road_x < 0)
		{
			if (wheel < 0xF8u)
				OBJECT(current_object)->player_car.wheel = wheel + 8u;
			gas = OBJECT(current_object)->player_car.gas;
			if (CAR(player_car)->skidding == 0u)
			{
				if (gas < 0xF0u)
					OBJECT(current_object)->player_car.gas = gas + 0x10u;
				return;
			}
			if (gas < 4u)
				return;
			OBJECT(current_object)->player_car.gas = gas - 4u;
			return;
		}
		if (wheel >= 8u)
			OBJECT(current_object)->player_car.wheel = wheel - 8u;
		gas = OBJECT(current_object)->player_car.gas;
		if (CAR(player_car)->skidding == 0u)
		{
			if (gas < 0xF0u)
				OBJECT(current_object)->player_car.gas = gas + 0x10u;
			return;
		}
		if (gas < 4u)
			return;
		OBJECT(current_object)->player_car.gas = gas - 4u;
		return;
	}
	if (CAR(player_car)->off_road != 0u && CAR(player_car)->model != 7u)
	{
		if (CAR(player_car)->road_x < 0)
		{
			if (wheel >= 8u)
				OBJECT(current_object)->player_car.wheel = wheel - 8u;
			gas = OBJECT(current_object)->player_car.gas;
			if (CAR(player_car)->skidding == 0u)
			{
				if (gas < 0xF0u)
					OBJECT(current_object)->player_car.gas = gas + 0x10u;
				return;
			}
			if (gas < 4u)
				return;
			OBJECT(current_object)->player_car.gas = gas - 4u;
			return;
		}
		if (wheel < 0xF8u)
			OBJECT(current_object)->player_car.wheel = wheel + 8u;
		gas = OBJECT(current_object)->player_car.gas;
		if (CAR(player_car)->skidding == 0u)
		{
			if (gas < 0xF0u)
				OBJECT(current_object)->player_car.gas = gas + 0x10u;
			return;
		}
		if (gas < 4u)
			return;
		OBJECT(current_object)->player_car.gas = gas - 4u;
		return;
	}
	if ((uint16_t)OBJECT(current_object)->player_car.sideways_speed != 0u)
	{
		if (OBJECT(current_object)->player_car.sideways_speed < 0)
		{
			if (wheel >= 8u)
				OBJECT(current_object)->player_car.wheel = wheel - 8u;
		}
		else if (wheel < 0xF8u)
			OBJECT(current_object)->player_car.wheel = wheel + 8u;
	}
	gas = OBJECT(current_object)->player_car.gas;
	if (CAR(player_car)->skidding == 0u)
	{
		if (gas < 0xF0u)
			OBJECT(current_object)->player_car.gas = gas + 0x10u;
		return;
	}
	if (gas < 4u)
		return;
	OBJECT(current_object)->player_car.gas = gas - 4u;
}

/* Moves the car for the frame: the drag (player_car_road_drag), the slope of its segment (race
   segment_pitch), the brake (no gas while braking; brake lights car brake_lights) and the gas through
   the gear's torque curve go to player_car_engine (speed, revs); the wheel steers through the model's
   table (less at low speed) into steering; the road's turn under it (previous_heading less heading)
   pushes it out (less with catch-up, car catch_up); beyond the model's grip it skids (car skidding,
   slide, tyre noise 3, smoke on both sides); sideways_speed and sideways_push move it across (car
   road_x, within the road's half width + 60); the speed along (less its sideways angle, vector_angle)
   plus a catch-up bonus by the distance to the car ahead (car catch_up_target, unk_6A; not once
   finished) moves it along, passing segments (car window_segment, segment, segment_offset, unk_A4,
   segment_progress, segment_fraction); then player_car_world_position. In ISDN mode (isdn_mode) the race
   record the link brings in gives all this. */
void player_car_drive(uint32_t *p_segment_record, uint32_t *p_road_x, uint32_t *p_segment_fraction, uint32_t *p_segment_length)
{
	uint32_t segment_record = *p_segment_record;
	uint32_t zero = 0, negative = 0, overflow = 0, carry = 0;
	uint8_t tyre_noise = 0;
	uint16_t catch_up_target = 0, half_width = 0, max_road_x = 0, min_road_x = 0, new_sideways_speed = 0, road_half_width = 0, segment_then_offset = 0, start_road_x = 0, window_segment = 0;
	uint32_t brake_force = 0, catch_up = 0, cornering = 0, drag = 0, drift = 0, drive_force = 0, forward_speed = 0, gas = 0, gear = 0, gear_curve = 0, gear_ratio = 0, grip = 0, new_speed = 0, origin_x = 0, origin_z = 0, road_force = 0, road_x = 0, segment_fraction = 0, segment_length = 0, segment_pitch = 0, speed = 0, steering = 0, wheel = 0;
	uint16_t target_sideways_speed = 0, gap_to_ahead = 0, catch_up_change = 0;

	r12 = CAR(player_car)->handling;
	CAR(player_car)->wheel_effect_sides = 0u;
	CAR(player_car)->wheel_effect_1 = 0u;
	CAR(player_car)->wheel_effect_2 = 0u;
	if (CAR(player_car)->speed != 0u && CAR(player_car)->shift_spin != 0u)
	{
		CAR(player_car)->wheel_effect_1 = 0xBu;
		CAR(player_car)->wheel_effect_2 = 0xBu;
		CAR(player_car)->wheel_effect_sides = 3u;
	}
	drag = CAR(player_car)->speed;
	if (drag != 0u)
	{
		drag = (drag * drag) >> 16;
		start_road_x = (uint16_t)CAR(player_car)->road_x;
		road_half_width = ROAD_SEGMENT(segment_record)->half_width;
		half_width = CAR_HANDLING(r12)->half_width;
		drag = player_car_road_drag(drag, start_road_x, road_half_width, half_width);
	}
	segment_pitch = ROAD_SEGMENT(segment_record)->pitch;
	RACE(SCREEN(screen)->race)->segment_pitch = segment_pitch;
	road_force = ((int32_t)sine_table[segment_pitch] >> 1) - (drag + 0x1800u);
	gas = OBJECT(current_object)->player_car.gas;
	AS16(CAR(player_car)->brake_lights) = 0u;
	brake_force = brake_pedal_curve[OBJECT(current_object)->player_car.brake];
	if (brake_force != 0u)
	{
		gas = 0u;
		AS16(CAR(player_car)->brake_lights) = 2u;
	}
	OBJECT(current_object)->player_car.brake_force = brake_force;
	brake_force = brake_force * CAR_HANDLING(r12)->brake_factor;
	gear = CAR(player_car)->gear;
	gear_curve = screen + 0x1300u + (uint16_t)(gear << 9);
	speed = CAR(player_car)->speed;
	drive_force = (int32_t)((uint32_t)gas_pedal_curve[gas] * TORQUE_CURVE_FORCES(gear_curve)[speed >> 8]) >> 6;
	gear_ratio = CAR(player_car)->gear_ratio;
	player_car_engine(road_force, drive_force, brake_force, gear_ratio);
	if (isdn_mode != 0u)
		CAR(player_car)->speed = RACE(SCREEN(screen)->race_copy)->speed;
	new_speed = CAR(player_car)->speed;
	OBJECT(current_object)->player_car.speed = new_speed;
	wheel = OBJECT(current_object)->player_car.wheel;
	steering = (int32_t)(WHEEL_STEERING(CAR_HANDLING(r12)->steering)[wheel >> 3] * 0x14u) / 0x10;
	if (new_speed <= 0xA000u)
	{
		if (new_speed > 0x2000u)
			steering = (int32_t)(steering * (new_speed + 0x6000u)) / 0x10000;
		else
			steering = (int32_t)(steering * new_speed) / 0x4000;
	}
	OBJECT(current_object)->player_car.steering = steering;
	new_sideways_speed = (uint16_t)((int16_t)((((uint32_t)OBJECT(current_object)->player_car.previous_heading << 4) - ((uint32_t)OBJECT(current_object)->player_car.heading << 4)) * 0x20u) / 0x100);
	if (OBJECT(current_object)->state == 5u)
		new_sideways_speed = (uint16_t)((int16_t)new_sideways_speed >> 4);
	new_sideways_speed = (uint16_t)divide_s32((uint32_t)(int16_t)new_sideways_speed << 8, CAR(player_car)->catch_up + 0x100u);
	CAR(player_car)->skidding = 0u;
	cornering = OBJECT(current_object)->player_car.speed * steering;
	grip = CAR_HANDLING(r12)->grip;
	if ((int32_t)grip <= (int32_t)cornering)
	{
		CAR(player_car)->skidding = 0xFFu;
		cornering = (int32_t)((cornering - grip) * CAR_HANDLING(r12)->slide_factor) / -0x400000;
		OBJECT(current_object)->player_car.slide = cornering;
		new_sideways_speed = (uint16_t)(new_sideways_speed + cornering + steering);
		if (CAR(player_car)->wheel_effect_sides == 0u)
		{
			CAR(player_car)->wheel_effect_1 = 3u;
			CAR(player_car)->wheel_effect_2 = 3u;
			CAR(player_car)->wheel_effect_sides = 3u;
		}
		tyre_noise = 3u;
		set_tyre_noise_on_road(tyre_noise);
		OBJECT(current_object)->player_car.sideways_speed = new_sideways_speed;
		drift = new_sideways_speed;
	}
	else
	{
		grip = 0u - grip;
		if ((int32_t)cornering <= (int32_t)grip)
		{
			CAR(player_car)->skidding = 0xFFu;
			cornering = (int32_t)((cornering - grip) * CAR_HANDLING(r12)->slide_factor) / -0x400000;
			OBJECT(current_object)->player_car.slide = cornering;
			new_sideways_speed = (uint16_t)(new_sideways_speed + cornering + steering);
			if (CAR(player_car)->wheel_effect_sides == 0u)
			{
				CAR(player_car)->wheel_effect_1 = 3u;
				CAR(player_car)->wheel_effect_2 = 3u;
				CAR(player_car)->wheel_effect_sides = 3u;
			}
			tyre_noise = 3u;
			set_tyre_noise_on_road(tyre_noise);
			OBJECT(current_object)->player_car.sideways_speed = new_sideways_speed;
			drift = new_sideways_speed;
		}
		else
		{
			new_sideways_speed += steering;
			if ((SCREEN(screen)->demo_flags & 0x100u) != 0u || RACE(SCREEN(screen)->race_copy)->finished != 0u)
			{
				drift = (uint16_t)OBJECT(current_object)->player_car.sideways_speed;
				target_sideways_speed = new_sideways_speed;
				new_sideways_speed = (uint16_t)(target_sideways_speed - drift);
				if (target_sideways_speed != drift)
					drift = (uint16_t)(drift + ((int16_t)(new_sideways_speed * 1u) / 4 + (shift_signed_by(new_sideways_speed, 0xF0u, 16) * 2u + 1u)));
				OBJECT(current_object)->player_car.sideways_speed = drift;
			}
			else
			{
				drift = new_sideways_speed;
				OBJECT(current_object)->player_car.sideways_speed = drift;
			}
		}
	}
	if (isdn_mode != 0u)
	{
		drift = (uint16_t)RACE(SCREEN(screen)->race_copy)->sideways_speed;
		OBJECT(current_object)->player_car.sideways_speed = drift;
	}
	drift = (uint32_t)(int16_t)drift << 8;
	speed = OBJECT(current_object)->player_car.speed;
	forward_speed = speed;
	origin_x = 0u;
	origin_z = 0u;
	drift = vector_angle(drift, speed, origin_x, origin_z);
	forward_speed = (forward_speed * cosine_table[drift]) >> 14;
	segment_fraction = CAR(player_car)->segment_progress + forward_speed;
	if (OBJECT(current_object)->player_car.finished == 0u)
	{
		catch_up_target = (uint16_t)(RACE(SCREEN(screen)->ahead_record)->segment - RACE(SCREEN(screen)->race_copy)->segment);
		if ((int16_t)catch_up_target > 0x20)
			catch_up_target = 0x20u;
		gap_to_ahead = catch_up_target;
		catch_up_target = (uint16_t)(gap_to_ahead - 1u);
		if ((int16_t)gap_to_ahead < 1)
			catch_up_target = 0u;
		CAR(player_car)->catch_up_target = catch_up_target;
		catch_up = CAR(player_car)->catch_up;
		catch_up_change = catch_up_target - catch_up;
		overflow = (uint32_t)(int16_t)catch_up_target - (int16_t)catch_up != (uint32_t)(int16_t)(catch_up_target - catch_up);
		zero = catch_up_change == 0u;
		negative = catch_up_change >> 15;
		if (catch_up_target != catch_up)
		{
			if (((negative ^ overflow) | zero) == 0u)
				catch_up = (uint16_t)(catch_up + 1u);
			else
				catch_up = (uint16_t)(catch_up - 1u);
		}
		CAR(player_car)->catch_up = catch_up;
		segment_fraction += (catch_up * forward_speed) >> 7;
	}
	if (isdn_mode != 0u)
	{
		segment_fraction = RACE(SCREEN(screen)->race_copy)->segment_fraction;
		segment_then_offset = RACE(SCREEN(screen)->race_copy)->segment;
		road_x = RACE(SCREEN(screen)->race_copy)->road_x;
		CAR(player_car)->segment_fraction = segment_fraction;
		CAR(player_car)->segment_progress = segment_fraction;
		CAR(player_car)->segment_progress = CAR(player_car)->segment_progress << 8;
		CAR(player_car)->segment = segment_then_offset;
		segment_then_offset = (segment_then_offset * 0x20u) & 0xFFFu;
		CAR(player_car)->segment_offset = segment_then_offset;
		segment_record = PAGE_ADD(screen + 0x2000u, segment_then_offset);
		r12 = (segment_record + 0x20u) & ~0x5000u;
		segment_length = ROAD_SEGMENT(segment_record)->heading;
		segment_length = (uint16_t)(segment_length - ROAD_SEGMENT(r12)->heading);
		segment_length = sine_table[segment_length & 0xFFFu] * road_x;
		carry = (segment_length >> 5) & 1u;
		segment_length = (int32_t)(((int32_t)segment_length >> 6) + 0x8000u + carry) >> 8;
		window_segment = RACE(SCREEN(screen)->race_copy)->window_segment;
		CAR(player_car)->window_segment = window_segment;
		CAR(player_car)->window_offset = (window_segment * 0x20u) & 0xFFFu;
	}
	else
	{
		road_x = (int16_t)(CAR(player_car)->road_x + OBJECT(current_object)->player_car.sideways_speed + OBJECT(current_object)->player_car.sideways_push);
		CAR(player_car)->road_x = road_x;
		for (;;)
		{
			r12 = (segment_record + 0x20u) & ~0x5000u;
			segment_length = ROAD_SEGMENT(segment_record)->heading;
			segment_length = (uint16_t)(segment_length - ROAD_SEGMENT(r12)->heading);
			segment_length = sine_table[segment_length & 0xFFFu] * road_x;
			carry = (segment_length >> 5) & 1u;
			segment_length = ((int32_t)segment_length >> 6) + 0x8000u + carry;
			if ((int32_t)segment_fraction < (int32_t)segment_length)
				break;
			segment_fraction -= segment_length;
			segment_record = r12;
			CAR(player_car)->window_segment++;
			CAR(player_car)->segment++;
			CAR(player_car)->window_offset += 0x20u;
			CAR(player_car)->window_offset &= 0xAFFFu;
			CAR(player_car)->segment_offset += 0x20u;
			CAR(player_car)->segment_offset &= 0xAFFFu;
		}
		CAR(player_car)->segment_progress = segment_fraction;
		segment_fraction = (int32_t)segment_fraction >> 8;
		segment_length = (int32_t)segment_length >> 8;
		CAR(player_car)->segment_fraction = segment_fraction;
	}
	if (isdn_mode == 0u)
	{
		max_road_x = ROAD_SEGMENT(segment_record)->half_width;
		min_road_x = (uint16_t)(0u - max_road_x);
		max_road_x += 0x60u;
		min_road_x -= 0x60u;
		if ((int16_t)road_x < (int16_t)max_road_x)
		{
			if ((int16_t)min_road_x < (int16_t)road_x)
				player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
			else
			{
				road_x = (road_x & ~0xFFFFu) | min_road_x;
				CAR(player_car)->road_x = road_x;
				player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
			}
		}
		else
		{
			road_x = (road_x & ~0xFFFFu) | max_road_x;
			CAR(player_car)->road_x = road_x;
			player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
		}
	}
	else
	{
		CAR(player_car)->road_x = road_x;
		player_car_world_position(segment_record, road_x, segment_fraction, segment_length);
	}
	*p_segment_record = segment_record;
	*p_road_x = road_x;
	*p_segment_fraction = segment_fraction;
	*p_segment_length = segment_length;
}

/* Places the car in the world from its segment: world_x and world_z from the segment's position and the
   car's place across and along it turned by the road's heading and pitch (rotate_offset), world_y the
   road's height there plus bounce_height (the old one to previous_world_y); the rest of the segment (car
   segment_remaining) and the road's heading under the car (heading, the old one to previous_heading). */
void player_car_world_position(uint32_t segment_record, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length)
{
	uint16_t previous_heading = 0;
	uint32_t heading_end = 0, heading_start = 0, height = 0, offset_x = 0, offset_y = 0, offset_z = 0, previous_world_y = 0, quadrant_heading = 0, segment_heading = 0, segment_pitch = 0, segment_remaining = 0;
	uint16_t heading = 0;
	uint32_t world_y = 0;

	if (isdn_mode == 0u)
	{
		offset_y = 0u;
		offset_x = road_x << 7;
		offset_z = segment_fraction << 7;
		segment_pitch = ROAD_SEGMENT(segment_record)->pitch;
		segment_heading = ROAD_SEGMENT(segment_record)->heading;
		rotate_offset(offset_y, segment_heading, segment_pitch, &offset_x, &offset_z);
		OBJECT(current_object)->player_car.world_x = (int32_t)(offset_x * 2u + (ROAD_SEGMENT(segment_record)->x << 8)) >> 8;
		OBJECT(current_object)->player_car.world_z = (int32_t)(offset_z * 2u + (ROAD_SEGMENT(segment_record)->z << 8)) >> 8;
		segment_remaining = segment_length - segment_fraction;
		height = divide_s32(ROAD_SEGMENT(r12)->y * segment_fraction + ROAD_SEGMENT(segment_record)->y * segment_remaining, segment_length) + OBJECT(current_object)->player_car.bounce_height;
	}
	else
	{
		OBJECT(current_object)->player_car.world_x = RACE(SCREEN(screen)->race_copy)->world_x;
		height = (uint32_t)RACE(SCREEN(screen)->race_copy)->world_y + RACE(SCREEN(screen)->race_copy)->bounce_height;
		OBJECT(current_object)->player_car.world_z = RACE(SCREEN(screen)->race_copy)->world_z;
		segment_remaining = segment_length - segment_fraction;
	}
	world_y = height;
	previous_world_y = OBJECT(current_object)->player_car.world_y;
	OBJECT(current_object)->player_car.world_y = world_y;
	OBJECT(current_object)->player_car.previous_world_y = previous_world_y;
	CAR(player_car)->segment_remaining = segment_remaining;
	heading_start = (uint16_t)((uint32_t)ROAD_SEGMENT(segment_record)->heading << 4);
	heading_end = (uint16_t)((uint32_t)ROAD_SEGMENT(r12)->heading << 4);
	quadrant_heading = heading_start;
	switch (quadrant_heading >> 14)
	{
	case 0:
	case 3:
		heading_start = (int16_t)heading_start;
		heading_end = (int16_t)heading_end;
		break;
	case 1:
	case 2:
		break;
	}
	heading = ((int32_t)divide_s32(heading_start * segment_remaining + heading_end * segment_fraction, segment_length) >> 4) & 0xFFFu;
	previous_heading = OBJECT(current_object)->player_car.heading;
	OBJECT(current_object)->player_car.heading = heading;
	OBJECT(current_object)->player_car.previous_heading = previous_heading;
}

/* Places the view behind the car: a point on the road some way back of the car (the nod, the intro, the
   rival and road-object pulls: car view_shift_nod, view_shift_intro, view_shift_rival, view_shift_lift)
   and across (view_side) and above (view_rise) it (view_target_x/y/z), looking toward the road twelve
   segments on (vector_angles: car view_yaw the yaw, road_pitch the road's pitch; car view_pitch the
   pitch with the view's tilt), the eye screen eye_distance back from the target along those angles
   (angles_to_offset) in car view_x-28. */
void player_car_place_view(uint32_t segment_record, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length)
{
	uint16_t pitch_here = 0, tilt = 0;
	uint32_t eye_distance = 0, eye_x = 0, eye_y = 0, eye_z = 0, far_segment = 0, far_z = 0, near_x = 0, near_y = 0, near_z = 0, offset_x = 0, offset_y = 0, offset_z = 0, pitch = 0, quadrant_yaw = 0, segment_heading = 0, segment_pitch = 0, segment_remaining = 0, shift_offset = 0, view_shift = 0, yaw = 0, yaw_here = 0;

	view_shift = (uint32_t)(int16_t)CAR(player_car)->view_shift_rival + (int16_t)CAR(player_car)->view_shift_lift;
	if ((int32_t)view_shift >= 0x30)
		view_shift = 0x30u;
	if ((int32_t)view_shift <= -0xC0)
		view_shift = 0xFFFFFF40u;
	view_shift = view_shift + (int16_t)CAR(player_car)->view_shift_intro + (int16_t)CAR(player_car)->view_shift_nod;
	shift_offset = ((int32_t)view_shift >> 2) & ~0x1Fu;
	segment_record += shift_offset;
	segment_record = LINKED(segment_record, (uint16_t)(segment_record | 0x2000u));
	segment_record = LINKED(segment_record, segment_record & 0xAFFFu);
	r12 += shift_offset;
	r12 = LINKED(r12, (uint16_t)(r12 | 0x2000u));
	segment_fraction += view_shift & 0x7Fu;
	offset_y = 0u;
	offset_x = (road_x + (int16_t)CAR(player_car)->view_side) << 7;
	offset_z = segment_fraction << 7;
	segment_pitch = ROAD_SEGMENT(segment_record)->pitch;
	segment_heading = ROAD_SEGMENT(segment_record)->heading;
	rotate_offset(offset_y, segment_heading, segment_pitch, &offset_x, &offset_z);
	OBJECT(current_object)->player_car.view_target_x = ((int32_t)(offset_x * 2u) >> 8) + ROAD_SEGMENT(segment_record)->x;
	OBJECT(current_object)->player_car.view_target_z = ((int32_t)(offset_z * 2u) >> 8) + ROAD_SEGMENT(segment_record)->z;
	segment_remaining = segment_length - segment_fraction;
	OBJECT(current_object)->player_car.view_target_y = divide_s32(ROAD_SEGMENT(LINKED(r12, r12 & 0xAFFFu))->y * segment_fraction + ROAD_SEGMENT(segment_record)->y * segment_remaining, segment_length) + (int16_t)CAR(player_car)->view_rise;
	far_segment = (segment_record + 0x180u) & ~0x5000u;
	pitch = ROAD_SEGMENT(far_segment)->x;
	yaw = ROAD_SEGMENT(far_segment)->y;
	far_z = ROAD_SEGMENT(far_segment)->z;
	near_x = ROAD_SEGMENT(segment_record)->x;
	near_y = ROAD_SEGMENT(segment_record)->y;
	near_z = ROAD_SEGMENT(segment_record)->z;
	vector_angles(far_z, near_x, near_y, near_z, &pitch, &yaw);
	segment_record = (segment_record + 0x20u) & ~0x5000u;
	r12 = (segment_record + 0x180u) & ~0x5000u;
	pitch_here = (uint16_t)pitch;
	yaw_here = yaw;
	pitch = ROAD_SEGMENT(r12)->x;
	yaw = ROAD_SEGMENT(r12)->y;
	far_z = ROAD_SEGMENT(r12)->z;
	near_x = ROAD_SEGMENT(segment_record)->x;
	near_y = ROAD_SEGMENT(segment_record)->y;
	near_z = ROAD_SEGMENT(segment_record)->z;
	vector_angles(far_z, near_x, near_y, near_z, &pitch, &yaw);
	pitch = (uint16_t)(pitch << 4);
	yaw = (uint16_t)(yaw << 4);
	pitch_here = (uint16_t)((uint32_t)pitch_here << 4);
	yaw_here = (uint16_t)(yaw_here << 4);
	quadrant_yaw = yaw;
	switch (quadrant_yaw >> 14)
	{
	case 0:
	case 3:
		yaw = (int16_t)yaw;
		yaw_here = (int16_t)yaw_here;
		break;
	case 1:
	case 2:
		break;
	}
	yaw = ((int32_t)divide_s32(yaw * segment_fraction + yaw_here * segment_remaining, segment_length) >> 4) & 0xFFFu;
	pitch = ((int32_t)divide_s32((int16_t)pitch * segment_fraction + (int16_t)pitch_here * segment_remaining, segment_length) >> 4) & 0xFFFu;
	CAR(player_car)->view_yaw = yaw;
	CAR(player_car)->road_pitch = pitch;
	tilt = (uint16_t)(CAR(player_car)->view_pitch_nod + CAR(player_car)->view_pitch_rival + CAR(player_car)->view_pitch_lift + SCREEN(screen)->view_pitch_base);
	if ((int16_t)tilt > 0x200)
		tilt = 0x200u;
	if ((int16_t)tilt < 0xC0)
		tilt = 0xC0u;
	pitch = (uint16_t)(pitch + tilt + CAR(player_car)->view_pitch_intro) & 0xFFFu;
	CAR(player_car)->view_pitch = pitch;
	eye_distance = SCREEN(screen)->eye_distance;
	pitch = (uint16_t)(pitch + 0x800u);
	pitch = pitch & 0xFFFu;
	angles_to_offset(pitch, yaw, eye_distance, &eye_x, &eye_y, &eye_z);
	eye_y += OBJECT(current_object)->player_car.view_target_y;
	eye_z += OBJECT(current_object)->player_car.view_target_z;
	CAR(player_car)->view_x = eye_x + OBJECT(current_object)->player_car.view_target_x;
	CAR(player_car)->view_y = eye_y;
	CAR(player_car)->view_z = eye_z;
}

/* The engine and the speed: the force (brake, slope and drag, drive; acceleration) changes the speed
   (car speed, also impact_speed), with any speed_change from a hit, up to the top speed of the gear (its
   ratio, car gear_ratio); a force past the model's grip spins the wheels, and hard braking locks them
   (car wheel_slip: tyre noise 1 or the model's skid noise, smoke; player_car_engine_wheelspin), the rest
   has grip (player_car_engine_grip); the revs (car revs) follow the speed through the gear's ratio, or
   the gas when the car stands; car moving says it moves. */
void player_car_engine(uint32_t road_force, uint32_t drive_force, uint32_t force, uint32_t gear_ratio)
{
	uint8_t tyre_noise = 0;
	uint32_t top_speed = 0;

	force = (int32_t)(force + road_force + drive_force) >> 6;
	OBJECT(current_object)->player_car.acceleration = force;
	top_speed = (gear_ratio * 0xFFFFu) >> 8;
	if (CAR(player_car)->wheel_slip == 0u)
	{
		if (CAR_HANDLING(r12)->spin_start_force <= (int32_t)force)
		{
			CAR(player_car)->wheel_slip = 0xFFu;
			if (CAR(player_car)->skidding == 0u)
			{
				tyre_noise = 1u;
				set_tyre_noise_on_road(tyre_noise);
			}
			force = CAR_HANDLING(r12)->spin_drive_force;
			if (CAR(player_car)->wheel_effect_sides != 0u)
			{
				player_car_engine_wheelspin(force, top_speed, gear_ratio);
				return;
			}
			CAR(player_car)->wheel_effect_1 = 1u;
			CAR(player_car)->wheel_effect_2 = 1u;
			CAR(player_car)->wheel_effect_sides = 3u;
			player_car_engine_wheelspin(force, top_speed, gear_ratio);
			return;
		}
		if (CAR(player_car)->speed == 0u || (int32_t)force > -0x380)
		{
			player_car_engine_grip(force, top_speed, gear_ratio);
			return;
		}
		CAR(player_car)->wheel_slip = 0xFFu;
		if (SCREEN(screen)->brake == 0u)
		{
			player_car_engine_wheelspin(force, top_speed, gear_ratio);
			return;
		}
		if (CAR(player_car)->skidding == 0u)
		{
			tyre_noise = car_skid_noises[CAR(player_car)->model];
			set_tyre_noise(tyre_noise);
		}
		if (CAR(player_car)->wheel_effect_sides != 0u)
		{
			player_car_engine_wheelspin(force, top_speed, gear_ratio);
			return;
		}
		CAR(player_car)->wheel_effect_1 = 2u;
		CAR(player_car)->wheel_effect_2 = 2u;
		CAR(player_car)->wheel_effect_sides = 3u;
		player_car_engine_wheelspin(force, top_speed, gear_ratio);
		return;
	}
	if (CAR_HANDLING(r12)->spin_end_force <= (int32_t)force)
	{
		if (CAR(player_car)->skidding == 0u)
		{
			tyre_noise = 1u;
			set_tyre_noise_on_road(tyre_noise);
		}
		force = CAR_HANDLING(r12)->spin_drive_force;
		if (CAR(player_car)->wheel_effect_sides != 0u)
		{
			player_car_engine_wheelspin(force, top_speed, gear_ratio);
			return;
		}
		CAR(player_car)->wheel_effect_1 = 1u;
		CAR(player_car)->wheel_effect_2 = 1u;
		CAR(player_car)->wheel_effect_sides = 3u;
		player_car_engine_wheelspin(force, top_speed, gear_ratio);
		return;
	}
	if (CAR(player_car)->speed == 0u)
	{
		player_car_engine_grip(force, top_speed, gear_ratio);
		return;
	}
	if ((int32_t)force > -0x480)
	{
		player_car_engine_grip(force, top_speed, gear_ratio);
		return;
	}
	if (SCREEN(screen)->brake == 0u)
	{
		player_car_engine_wheelspin(force, top_speed, gear_ratio);
		return;
	}
	if (CAR(player_car)->skidding == 0u)
	{
		tyre_noise = car_skid_noises[CAR(player_car)->model];
		set_tyre_noise(tyre_noise);
	}
	if (CAR(player_car)->wheel_effect_sides != 0u)
	{
		player_car_engine_wheelspin(force, top_speed, gear_ratio);
		return;
	}
	CAR(player_car)->wheel_effect_1 = 2u;
	CAR(player_car)->wheel_effect_2 = 2u;
	CAR(player_car)->wheel_effect_sides = 3u;
	player_car_engine_wheelspin(force, top_speed, gear_ratio);
}

/* The speed and the revs while the wheels spin or lock (player_car_engine goes on to it from both of its
   sides): the speed (car speed, with any speed_change from a hit, not below 0) goes half the way toward
   itself plus the force (not below 0), or a thirtieth of the way down to the gear's top speed (at most
   FFFF), into car speed and impact_speed; standing, player_car_engine_standing; moving, car moving FF
   and the revs a tenth of the way toward the speed * 256 by the gear's ratio (at most FFFF). */
void player_car_engine_wheelspin(uint32_t force, uint32_t top_speed, uint32_t gear_ratio)
{
	uint32_t negative = 0, overflow = 0;
	uint32_t revs = 0, revs_change = 0, speed = 0, speed_step = 0, speed_then_revs = 0;
	uint32_t spin_speed_change = 0, spin_speed = 0, spin_target_speed = 0, spin_top_speed = 0, spin_target_revs = 0;

	speed = CAR(player_car)->speed;
	spin_speed_change = OBJECT(current_object)->player_car.speed_change;
	spin_speed = speed + spin_speed_change;
	overflow = v60_overflow_add32(speed, spin_speed_change, 0u);
	negative = spin_speed >> 31;
	speed_then_revs = spin_speed;
	if (negative ^ overflow)
		speed_then_revs = 0u;
	speed_step = speed_then_revs + force;
	if ((int32_t)speed_step <= (int32_t)top_speed)
	{
		if ((int32_t)speed_step < 0)
			speed_step = 0u;
		spin_target_speed = speed_step;
		speed_step = spin_target_speed - speed_then_revs;
		if (spin_target_speed != speed_then_revs)
			speed_then_revs += (int32_t)(speed_step * 1u) / 2 + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
	}
	else
	{
		spin_top_speed = top_speed;
		speed_step = spin_top_speed - speed_then_revs;
		if (spin_top_speed != speed_then_revs)
			speed_then_revs += (int32_t)(speed_step * 1u) / 0x1E + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
		if (speed_then_revs >= 0xFFFFu)
			speed_then_revs = 0xFFFFu;
	}
	CAR(player_car)->speed = speed_then_revs;
	OBJECT(current_object)->player_car.impact_speed = speed_then_revs;
	if (speed_then_revs == 0u)
	{
		player_car_engine_standing();
		return;
	}
	CAR(player_car)->moving = 0xFFu;
	revs = CAR(player_car)->revs;
	spin_target_revs = divide_u32(speed_then_revs << 8, gear_ratio);
	revs_change = spin_target_revs - revs;
	if (spin_target_revs != revs)
		revs += (int32_t)(revs_change * 1u) / 0xA + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
	if (revs >= 0xFFFFu)
		revs = 0xFFFFu;
	CAR(player_car)->revs = revs;
}

/* The speed and the revs with grip (player_car_engine goes on to it from both of its sides): car
   wheel_slip 0, the speed as player_car_engine_wheelspin works it out; standing,
   player_car_engine_standing; moving, car moving FF and the revs toward the speed * 256 by the gear's
   ratio (player_car_engine_revs). */
void player_car_engine_grip(uint32_t force, uint32_t top_speed, uint32_t gear_ratio)
{
	uint32_t negative = 0, overflow = 0;
	uint32_t speed = 0, speed_step = 0, speed_then_revs = 0;
	uint32_t grip_speed_change = 0, grip_speed = 0, grip_target_speed = 0, grip_top_speed = 0;

	CAR(player_car)->wheel_slip = 0u;
	speed = CAR(player_car)->speed;
	grip_speed_change = OBJECT(current_object)->player_car.speed_change;
	grip_speed = speed + grip_speed_change;
	overflow = v60_overflow_add32(speed, grip_speed_change, 0u);
	negative = grip_speed >> 31;
	speed_then_revs = grip_speed;
	if (negative ^ overflow)
		speed_then_revs = 0u;
	speed_step = speed_then_revs + force;
	if ((int32_t)speed_step <= (int32_t)top_speed)
	{
		if ((int32_t)speed_step < 0)
			speed_step = 0u;
		grip_target_speed = speed_step;
		speed_step = grip_target_speed - speed_then_revs;
		if (grip_target_speed != speed_then_revs)
			speed_then_revs += (int32_t)(speed_step * 1u) / 2 + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
	}
	else
	{
		grip_top_speed = top_speed;
		speed_step = grip_top_speed - speed_then_revs;
		if (grip_top_speed != speed_then_revs)
			speed_then_revs += (int32_t)(speed_step * 1u) / 0x1E + (shift_signed_by(speed_step, 0xE0u, 32) * 2u + 1u);
		if (speed_then_revs >= 0xFFFFu)
			speed_then_revs = 0xFFFFu;
	}
	CAR(player_car)->speed = speed_then_revs;
	OBJECT(current_object)->player_car.impact_speed = speed_then_revs;
	if (speed_then_revs == 0u)
	{
		player_car_engine_standing();
		return;
	}
	CAR(player_car)->moving = 0xFFu;
	speed_then_revs = divide_u32(speed_then_revs << 8, gear_ratio);
	player_car_engine_revs(speed_then_revs);
}

/* The car standing (speed 0): car moving 0 and the revs toward the gas * 256 (player_car_engine_revs). */
void player_car_engine_standing(void)
{
	uint32_t gas_revs = 0;

	CAR(player_car)->moving = 0u;
	gas_revs = (uint32_t)OBJECT(current_object)->player_car.gas << 8;
	player_car_engine_revs(gas_revs);
}

/* Moves the revs (car revs) a third of the way toward target_revs, at most FFFF. */
void player_car_engine_revs(uint32_t target_revs)
{
	uint32_t revs = 0, revs_change = 0;
	uint32_t wanted_revs = 0;

	revs = CAR(player_car)->revs;
	wanted_revs = target_revs;
	revs_change = wanted_revs - revs;
	if (wanted_revs != revs)
		revs += (int32_t)(revs_change * 1u) / 3 + (shift_signed_by(revs_change, 0xE0u, 32) * 2u + 1u);
	if (revs >= 0xFFFFu)
		revs = 0xFFFFu;
	CAR(player_car)->revs = revs;
}

/* Returns the offset of length R2 along pitch R0 and yaw R1 (R3, R4, R5: x, y, z). Used by
   player_car_place_view for the eye behind the view's target. */
void angles_to_offset(uint32_t pitch, uint32_t yaw, uint32_t distance, uint32_t *p_offset_x, uint32_t *p_offset_y, uint32_t *p_offset_z)
{
	uint32_t pitch_cosine = 0;

	pitch_cosine = cosine_table[pitch];
	*p_offset_x = (int32_t)(((int32_t)(sine_table[yaw] * pitch_cosine) >> 14) * distance) >> 14;
	*p_offset_y = 0u - ((int32_t)(sine_table[pitch] * distance) >> 14);
	*p_offset_z = (int32_t)(((int32_t)(cosine_table[yaw] * pitch_cosine) >> 14) * distance) >> 14;
}

/* Unused (only distance_offset_functions lists it, for octants 1, 2, 5 and 6 of an angle): the distance
   along angle R0 that has the x distance from R11's x (+4) to the object's field 20 (that distance / the
   sine of R0), turned to angle R1 by distance_offset_at_angle. Its C has no body: nothing reads the
   results (R2, R3). */
void distance_offset_from_x(void)
{
	distance_offset_at_angle();
}

/* Unused (only distance_offset_functions lists it, for octants 0, 3, 4 and 7 of an angle): the distance
   along angle R0 that has the z distance from R11's z (+8) to the object's field 28 (that distance / the
   cosine of R0), turned to angle R1 by distance_offset_at_angle. Its C has no body: nothing reads the
   results (R2, R3). */
void distance_offset_from_z(void)
{
	distance_offset_at_angle();
}

/* Unused (the end of distance_offset_from_x and distance_offset_from_z): R2 / R6 as a length along angle
   R1, its sine part into R2 and its cosine part into R3. Its C has no body: nothing reads the results. */
void distance_offset_at_angle(void)
{
}

/* Takes the roadside surfaces of the stage (screen built_stage): the two lists of
   stage_roadside_surface_lists for it (car surface_entry_1, surface_entry_2), from the segment where the
   stage starts (car surface_start) to the next one's (surface_next_start). */
void player_car_next_surfaces(void)
{
	uint16_t stage_length = 0;
	uint32_t stage_then_surfaces = 0;

	stage_then_surfaces = SCREEN(screen)->built_stage;
	CAR(player_car)->surface_stage = stage_then_surfaces;
	stage_length = (uint16_t)(SCREEN(screen)->stage_lengths[(int32_t)stage_then_surfaces] + SCREEN(screen)->branch_road_length);
	CAR(player_car)->surface_start = CAR(player_car)->surface_next_start;
	CAR(player_car)->surface_next_start += stage_length;
	stage_then_surfaces = stage_roadside_surface_lists_ADDRESS + (uint8_t)(stage_then_surfaces * 2u) * 4u;
	CAR(player_car)->surface_entry_1 = STAGE_ROADSIDE_SURFACES(stage_then_surfaces)->first;
	CAR(player_car)->surface_entry_2 = STAGE_ROADSIDE_SURFACES(stage_then_surfaces)->second;
}

/* The drag on the car: speed squared, times the model's off-road factor while slowed (car slowed_frames)
   or off the road (car off_road, with the off-road shake); and the roadside surfaces under its wheels
   (car surface_entry_1, surface_entry_2) give the effects at each side (car wheel_effect_sides: bit 0
   with wheel_effect_1, bit 1 with wheel_effect_2). */
uint32_t player_car_road_drag(uint32_t drag, uint16_t road_x, uint16_t road_half_width, uint16_t half_width)
{
	uint32_t carry = 0;
	uint16_t beyond_edge = 0, inner_edge = 0, inner_edge_2 = 0, low_side = 0, outer_edge = 0, outer_edge_2 = 0;
	uint32_t empty_bits = 0, roadside_surface = 0, segment = 0, surface_bits = 0, surface_start = 0;

	roadside_surface = CAR(player_car)->surface_entry_1;
	r14 = CAR(player_car)->surface_entry_2;
	if (ROADSIDE_SURFACE(roadside_surface)->end < 0 && ROADSIDE_SURFACE(r14)->end < 0 && CAR(player_car)->surface_next_start <= CAR(player_car)->segment)
	{
		player_car_next_surfaces();
		roadside_surface = CAR(player_car)->surface_entry_1;
		r14 = CAR(player_car)->surface_entry_2;
	}
	segment = CAR(player_car)->segment;
	surface_start = CAR(player_car)->surface_start;
	if ((uint16_t)ROADSIDE_SURFACE(roadside_surface)->end + surface_start <= segment)
	{
		roadside_surface += 8u;
		CAR(player_car)->surface_entry_1 = roadside_surface;
	}
	if ((uint16_t)ROADSIDE_SURFACE(r14)->end + surface_start <= segment)
	{
		r14 += 8u;
		CAR(player_car)->surface_entry_2 = r14;
	}
	if (CAR(player_car)->slowed_frames != 0u)
	{
		CAR(player_car)->slowed_frames--;
		drag = (drag * CAR_HANDLING(r12)->off_road_drag) >> 4;
		player_car_off_road_shake();
		return drag;
	}
	low_side = (uint16_t)(road_x - half_width);
	empty_bits = 0u;
	half_width = (uint16_t)(half_width + road_x - 0x8000u);
	low_side -= 0x8000u;
	carry = (uint16_t)(0x8000u + road_half_width) < half_width;
	surface_bits = carry;
	carry = low_side < (uint16_t)(0u - road_half_width - 0x8000u);
	CAR(player_car)->off_road = 0u;
	if ((uint8_t)(surface_bits + carry) != 0u)
	{
		CAR(player_car)->off_road = 0xFFu;
		drag = (drag * CAR_HANDLING(r12)->off_road_drag) >> 4;
		player_car_off_road_shake();
	}
	inner_edge = ROADSIDE_SURFACE(roadside_surface)->inner_edge;
	outer_edge = ROADSIDE_SURFACE(roadside_surface)->outer_edge;
	inner_edge_2 = ROADSIDE_SURFACE(r14)->inner_edge;
	outer_edge_2 = ROADSIDE_SURFACE(r14)->outer_edge;
	beyond_edge = (uint16_t)(half_width - road_half_width);
	half_width += road_half_width;
	carry = inner_edge < beyond_edge;
	surface_bits = (uint8_t)(empty_bits * 2u + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = outer_edge < beyond_edge;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = half_width < inner_edge_2;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = half_width < outer_edge_2;
	switch ((uint8_t)(surface_bits + empty_bits + carry))
	{
	case 0:
		wheel_effect_1_none();
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
		wheel_effect_1_surface_2_outer();
		break;
	case 2:
	case 6:
	case 10:
	case 14:
		wheel_effect_1_surface_2_inner();
		break;
	case 4:
	case 12:
		wheel_effect_1_surface_1_outer(roadside_surface);
		break;
	case 8:
		wheel_effect_1_surface_1_inner(roadside_surface);
		break;
	case 16:
		wheel_effect_2_none();
		break;
	case 17:
	case 19:
		wheel_effect_2_surface_2_outer();
		break;
	case 18:
		wheel_effect_2_surface_2_inner();
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 28:
	case 29:
	case 30:
	case 31:
		wheel_effect_2_surface_1_outer(roadside_surface);
		break;
	case 24:
	case 25:
	case 26:
	case 27:
		wheel_effect_2_surface_1_inner(roadside_surface);
		break;
	}
	beyond_edge = (uint16_t)(low_side - road_half_width);
	low_side += road_half_width;
	carry = inner_edge < beyond_edge;
	surface_bits = (uint8_t)(empty_bits * 2u + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = outer_edge < beyond_edge;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = low_side < inner_edge_2;
	surface_bits = (uint8_t)(surface_bits + empty_bits + carry);
	surface_bits = (uint8_t)(surface_bits * 2u);
	carry = low_side < outer_edge_2;
	switch ((uint8_t)(surface_bits + empty_bits + carry))
	{
	case 0:
		wheel_effect_2_none();
		return drag;
	case 8:
	case 9:
	case 10:
	case 11:
		wheel_effect_2_surface_1_inner(roadside_surface);
		return drag;
	case 4:
	case 5:
	case 6:
	case 7:
	case 12:
	case 13:
	case 14:
	case 15:
		wheel_effect_2_surface_1_outer(roadside_surface);
		return drag;
	case 2:
		wheel_effect_2_surface_2_inner();
		return drag;
	case 1:
	case 3:
		wheel_effect_2_surface_2_outer();
		return drag;
	}
	unreachable();
}

/* The wheel at road_x plus the model's half track (handling +1A) is within the bounds of both surface
   entries: no effect (player_car_road_drag's pick, wheel_effect_1_jumps). */
void wheel_effect_1_none(void)
{
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+6) of surface_entry_1 past its inner bound
   (+2): the wheel at road_x plus the half track is past it and no other bound (player_car_road_drag's
   pick, wheel_effect_1_jumps). */
void wheel_effect_1_surface_1_inner(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(surface)->inner_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+7) of surface_entry_1 past its outer bound
   (+4), when the wheel at road_x plus the half track is past it and not past the bounds of
   surface_entry_2. */
void wheel_effect_1_surface_1_outer(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(surface)->outer_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+6) of surface_entry_2 past its inner bound
   (+2), when the wheel at road_x plus the half track is past it but not past its outer bound. */
void wheel_effect_1_surface_2_inner(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(r14)->inner_effect;
}

/* wheel_effect_sides bit 0 and wheel_effect_1 = the effect (+7) of surface_entry_2 past its outer bound
   (+4): for the wheel at road_x plus the half track it comes before every other. */
void wheel_effect_1_surface_2_outer(void)
{
	CAR(player_car)->wheel_effect_sides |= 1u;
	CAR(player_car)->wheel_effect_1 = ROADSIDE_SURFACE(r14)->outer_effect;
}

/* The wheel at road_x less the model's half track is within the bounds of both surface entries: no
   effect (player_car_road_drag's pick, wheel_effect_2_jumps). */
void wheel_effect_2_none(void)
{
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+6) of surface_entry_1 past its inner bound
   (+2), when the wheel at road_x less the half track is past it but not past its outer bound. */
void wheel_effect_2_surface_1_inner(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(surface)->inner_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+7) of surface_entry_1 past its outer bound
   (+4): for the wheel at road_x less the half track it comes before every other. */
void wheel_effect_2_surface_1_outer(uint32_t surface)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(surface)->outer_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+6) of surface_entry_2 past its inner bound
   (+2), when the wheel at road_x less the half track passes no bound of surface_entry_1 nor the outer
   one of surface_entry_2. */
void wheel_effect_2_surface_2_inner(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(r14)->inner_effect;
}

/* wheel_effect_sides bit 1 and wheel_effect_2 = the effect (+7) of surface_entry_2 past its outer bound
   (+4), when the wheel at road_x less the half track passes no bound of surface_entry_1. */
void wheel_effect_2_surface_2_outer(void)
{
	CAR(player_car)->wheel_effect_sides |= 2u;
	CAR(player_car)->wheel_effect_2 = ROADSIDE_SURFACE(r14)->outer_effect;
}

/* Off the road: the model's shake to the screen's bits of output port D (wheel_motor, the wheel motor's;
   car_model_off_road_shakes) and its off-road noise (car_model_off_road_noises). */
void player_car_off_road_shake(void)
{
	uint8_t tyre_noise = 0;
	uint32_t model = 0;

	model = CAR(player_car)->model;
	wheel_motor[SCREEN(screen)->index] = car_model_off_road_shakes[model];
	tyre_noise = car_model_off_road_noises[model];
	set_tyre_noise(tyre_noise);
}
