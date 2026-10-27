/**
 *    Name:        scenes/route_map.c
 *    Description: the route map after the race: the car, the signs, the landmarks, the legs of the route, the stage results and the GAME OVER banner
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The map of the route after the race (game step 12, route_map_object_handlers); keeps "Thank you for
   your playing." scrolling on the LCD (F40 = 005014). By state: 0 copies its screen's tilemap register
   addresses (route_map_register_sets) into 80-BC, first_branch from screen route_branches, the
   background (set_up_background with 004868 by screen and first_branch: the routes on tilemap 0 over the
   world on the second), the palette route_map_background_palette (scene_palettes[2C]) (queue_palette); 1
   zoom 1:1 about the middle of the screen, sea_colour black, 30 frames (timer); 2 the world's tilemap
   off, sea_colour 7EE0; 3 zooms in (zoom + 20 a frame for 9 frames, 200 being 1:1), then the close-up
   map (pages route_map_close_up_pages) at its start (x scroll route_map_start_scroll_xs by first_branch,
   y 8C), starts route_map_car and the 32 route_map_landmark objects; 4 checkpoint_legs_left from the
   car's unk_D0, unk_12 from screen current_stage_level, stage 0; 5-14 the car drives the route leg by
   leg (leg_frames frames each): 22 frames up from the start to the first checkpoint (the car moves, the
   map stands), then 6 frames of 8 pixels sideways (first_branch picks the side), 11 of 5, then for each
   stage 10 frames of 4 up or down by the branch taken (screen route_branches + stage - 1) and 15 of 7
   sideways; on these legs the map scrolls under the car (the object's x and y move the other way, for
   the objects on the map), screen route_map_signals telling the car its way (bits 0 down, 1 up, 2 right,
   3 left, 5 the map at its edge, 7 standing); a stage_result_line starts where each stage begins (5, 7,
   11), a route_map_sign (C.P) and stage + 1 where a checkpoint leg ends; when its count of legs (unk_12
   for the first leg of a stage, checkpoint_legs_left for the others) is 0 at a leg's start, the race
   ended on it: last_leg_progress and leg_frames are worked out from the race_copy record's segment, the
   car's reached_stage_start and the stage lengths at 700 of the screen, and screen route_map_signals bit
   6 ends the drive after it; at the end (stage 6 or a cut leg) the car stands (bit 7), FINISH
   (route_map_sign 1) if the goal was reached (race_copy finished), 60 frames; 15 the routes (pages
   route_map_routes_pages) at zoom 2x where the close-up map was, the steps of the zoom out into x and y,
   screen route_map_signals bit 4 (the car, the landmarks and the signs end); 16 zooms out over 9 frames
   to 1:1 and the whole route (x scroll route_map_whole_scroll_xs by first_branch, y 70), then the
   world's tilemap again (pages route_map_world_pages); 17 sea_colour black, starts 00463C and an 004240
   for each leg (stage + 1, at most 6), DJ message 38, 90 frames; 18 DJ message 39 once the DJ is quiet
   (screen dj_mouth_shapes); 19 when the 90 frames are over sets the game's next step (screen game_mode
   byte 2 = 0x10 (step 16)) and starts 0046BB; 20 nothing. */
void route_map_update(void)
{
	bool L_003BFC = false, L_003C2D = false;
	uint8_t dj_message = 0, landmark_index = 0, leg_number = 0;
	uint32_t background_setup = 0, branch_road_length = 0, colour_block = 0, landmarks_left = 0, leg_distance = 0, legs_left = 0, lengths_place = 0, object = 0, route_branches = 0, route_map_registers = 0, setup_base = 0, stage_length = 0, stage_lengths = 0;
	uint8_t timer = 0, timer_2 = 0, start_leg_frames = 0, timer_3 = 0, turn_leg_frames = 0, approach_leg_frames = 0, branch_leg_frames = 0, stage_leg_frames = 0, timer_4 = 0, timer_5 = 0;
	uint32_t jump_target = 0, zoom_x_address = 0, zoom_y_address = 0, scroll_x_address = 0, scroll_x_address_2 = 0, scroll_y_address = 0, scroll_x_address_3 = 0, scroll_y_address_2 = 0, scroll_x_address_4 = 0, scroll_y_address_3 = 0, scroll_x_address_5 = 0, scroll_x_address_6 = 0, scroll_x_address_7 = 0, scroll_x_address_8 = 0, scroll_y_address_4 = 0, scroll_x_address_9 = 0, scroll_y_address_5 = 0, scroll_x_address_10 = 0, scroll_x_address_11 = 0, scroll_x_address_12 = 0, zoom_x_address_2 = 0, zoom_y_address_2 = 0, scroll_x_address_13 = 0, scroll_y_address_6 = 0, second_scroll_x_address = 0, second_scroll_y_address = 0;

	if ((SCREEN(screen)->lcd_scroll | SCREEN(screen)->lcd_scroll_request) == 0u)
		SCREEN(screen)->lcd_scroll_request = thank_you_lcd_text_ADDRESS;
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
							do
							{
								do
								{
									jump_target = L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003BFC ? 0x383Au : L_003C2D ? 0x383Au : route_map_state_jumps[OBJECT(current_object)->state];
									switch (jump_target)
									{
									case 0x003594u:
										SCREEN(screen)->route_map_signals = 0u;
										route_map_registers = route_map_register_sets_ADDRESS + ((uint32_t)SCREEN(screen)->index << 4) * 4u;
										OBJECT(current_object)->route_map.pages_address = ROUTE_MAP_REGISTERS(route_map_registers)->pages_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x4u)->scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x8u)->scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0xCu)->zoom_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x10u)->zoom_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_center_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x14u)->zoom_center_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.zoom_center_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x18u)->zoom_center_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.register_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x1Cu)->register_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.register_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x20u)->register_scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.tile_bank_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x24u)->tile_bank_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_pages_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x28u)->second_pages_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x2Cu)->second_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x30u)->second_scroll_y_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_x_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x34u)->second_register_scroll_x_address;
										route_map_registers += 4u;
										OBJECT(current_object)->route_map.second_register_scroll_y_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x38u)->second_register_scroll_y_address;
										OBJECT(current_object)->route_map.second_tile_bank_address = ROUTE_MAP_REGISTERS(route_map_registers - 0x38u)->second_tile_bank_address;
										setup_base = SCREEN(screen)->index * 5u * 2u;
										route_branches = SCREEN(screen)->route_branches[0];
										OBJECT(current_object)->route_map.first_branch = route_branches;
										background_setup = 0x4868u + (setup_base + route_branches * 5u) * 4u;
										set_up_background(background_setup);
										colour_block = route_map_background_palette;
										queue_palette(colour_block);
										OBJECT(current_object)->state++;
										return;
									case 0x003644u:
										OBJECT(current_object)->route_map.sea_colour_address = route_map_sea_colour_addresses[SCREEN(screen)->index];
										io_write16(OBJECT(current_object)->route_map.sea_colour_address, 0u);
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_x_address)[0] = 0x200u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_y_address)[0] = 0x200u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_x_address)[0] = 0xA0u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_y_address)[0] = 0x70u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0] = 0x180u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0] = 0x70u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_x_address)[0] = 0x8C00u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_y_address)[0] = 0x70u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_x_address)[0] = 0xF8u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_y_address)[0] = 0u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_register_scroll_x_address)[0] = 0xF8u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_register_scroll_y_address)[0] = 0u;
										send_video_registers = 0xFFu;
										OBJECT(current_object)->route_map.timer = 0x1Eu;
										OBJECT(current_object)->state++;
										return;
									case 0x0036C0u:
										timer = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer - 1u;
										if (timer == 1u)
										{
											PAGE_SELECT(OBJECT(current_object)->route_map.second_pages_address)[0] = 0u;
											io_write16(OBJECT(current_object)->route_map.sea_colour_address, 0x7EE0u);
											OBJECT(current_object)->route_map.timer = 9u;
											OBJECT(current_object)->state++;
										}
										return;
									case 0x0036DFu:
										zoom_x_address = OBJECT(current_object)->route_map.zoom_x_address;
										VIDEO_REGISTER_WORDS(zoom_x_address)[0] += 0x20u;
										zoom_y_address = OBJECT(current_object)->route_map.zoom_y_address;
										VIDEO_REGISTER_WORDS(zoom_y_address)[0] += 0x20u;
										send_video_registers = 0xFFu;
										timer_2 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_2 - 1u;
										if (timer_2 != 1u)
											return;
										PAGE_SELECT(OBJECT(current_object)->route_map.pages_address)[0] = route_map_close_up_pages[SCREEN(screen)->index];
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_x_address)[0] = 0x200u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_y_address)[0] = 0x200u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_x_address)[0] = 0u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_y_address)[0] = 0u;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0] = route_map_start_scroll_xs[OBJECT(current_object)->route_map.first_branch];
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0] = 0x8Cu;
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_x_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0];
										VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_y_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0];
										send_video_registers = 0xFFu;
										SCREEN(screen)->skip_scroll_update = 0xFFu;
										object = start_object();
										OBJECT(object)->handler = route_map_car_update_ADDRESS;
										OBJECT(object)->route_map_car.model = CAR(player_car)->model;
										OBJECT(object)->route_map_car.route_map = current_object;
										SCREEN(screen)->route_map_signals |= 0x80u;
										OBJECT(current_object)->x = 0xA0u;
										OBJECT(current_object)->y = 0x70u;
										landmarks_left = 0x20u;
										landmark_index = 0u;
										do
										{
											object = start_object();
											OBJECT(object)->handler = route_map_landmark_update_ADDRESS;
											OBJECT(object)->route_map_landmark.index_then_half_rate = landmark_index;
											OBJECT(object)->route_map_landmark.route_map = current_object;
											landmark_index++;
											landmarks_left--;
										} while (landmarks_left != 0u);
										OBJECT(current_object)->state++;
										return;
									case 0x0037A7u:
										OBJECT(current_object)->route_map.first_legs_left = SCREEN(screen)->current_stage_level;
										OBJECT(current_object)->route_map.checkpoint_legs_left = CAR(player_car)->forks_taken;
										OBJECT(current_object)->route_map.stage = 0u;
										OBJECT(current_object)->state++;
										return;
									case 0x0037BEu:
										SCREEN(screen)->route_map_signals &= ~0x80u;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0x16;
											OBJECT(current_object)->state++;
										}
										else
										{
											OBJECT(object)->stage_result_line.last = 1u;
											leg_distance = mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7));
											stage_lengths = SCREEN(screen)->stage_lengths[0];
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 3, stage_lengths);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(((leg_distance * 5u) << 2) + leg_distance * 2u, stage_lengths);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x00383Au:
										if (!L_003BFC)
										{
											if (!L_003BFC)
											{
												if (!L_003BFC)
												{
													if (!L_003BFC)
													{
														if (!L_003BFC)
														{
															if (!L_003BFC)
															{
																if (!L_003BFC)
																{
																	if (!L_003C2D)
																	{
																		start_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
																		OBJECT(current_object)->route_map.leg_frames = start_leg_frames;
																		if ((int8_t)start_leg_frames >= 0)
																		{
																			SCREEN(screen)->route_map_signals |= 2u;
																			return;
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
										if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003C2D || (SCREEN(screen)->route_map_signals & 0x40u) != 0u)
										{
											if (L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || L_003BFC || !L_003C2D)
											{
												SCREEN(screen)->route_map_signals |= 0x80u;
												if (RACE(SCREEN(screen)->race_copy)->finished != 0u)
												{
													object = start_object();
													OBJECT(object)->handler = route_map_sign_update_ADDRESS;
													OBJECT(object)->route_map_sign.route_map = current_object;
													OBJECT(object)->route_map_sign.kind = 1u;
												}
												OBJECT(current_object)->route_map.timer = 0x3Cu;
												OBJECT(current_object)->state = 0xFu;
											}
											timer_3 = OBJECT(current_object)->route_map.timer;
											OBJECT(current_object)->route_map.timer = timer_3 - 1u;
											if (timer_3 == 1u)
											{
												PAGE_SELECT(OBJECT(current_object)->route_map.pages_address)[0] = route_map_routes_pages[SCREEN(screen)->index];
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_x_address)[0] = 0x400u;
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_y_address)[0] = 0x400u;
												scroll_x_address = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address)[0] -= 0x180u;
												scroll_x_address_2 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_2)[0] -= (uint32_t)OBJECT(current_object)->route_map.first_branch << 8;
												scroll_y_address = OBJECT(current_object)->route_map.scroll_y_address;
												VIDEO_REGISTER_WORDS(scroll_y_address)[0] -= 0xF7u;
												scroll_x_address_3 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_3)[0] = (int16_t)VIDEO_REGISTER_WORDS(scroll_x_address_3)[0] >> 1;
												scroll_y_address_2 = OBJECT(current_object)->route_map.scroll_y_address;
												VIDEO_REGISTER_WORDS(scroll_y_address_2)[0] = (int16_t)VIDEO_REGISTER_WORDS(scroll_y_address_2)[0] >> 1;
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_x_address)[0] = 0xA0u;
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_center_y_address)[0] = 0x70u;
												scroll_x_address_4 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_4)[0] += 0x1D0u;
												scroll_y_address_3 = OBJECT(current_object)->route_map.scroll_y_address;
												VIDEO_REGISTER_WORDS(scroll_y_address_3)[0] += 0xA3u;
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_x_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0];
												VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_y_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0];
												OBJECT(current_object)->x = route_map_whole_scroll_xs[OBJECT(current_object)->route_map.first_branch];
												OBJECT(current_object)->y = 0x70u;
												OBJECT(current_object)->x -= VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0];
												OBJECT(current_object)->y -= VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0];
												OBJECT(current_object)->x = (int16_t)OBJECT(current_object)->x >> 3;
												OBJECT(current_object)->y = (int16_t)OBJECT(current_object)->y >> 3;
												send_video_registers = 0xFFu;
												SCREEN(screen)->skip_scroll_update = 0xFFu;
												SCREEN(screen)->route_map_signals |= 0x10u;
												OBJECT(current_object)->route_map.timer = 9u;
												OBJECT(current_object)->state++;
											}
											return;
										}
										object = start_object();
										OBJECT(object)->handler = route_map_sign_update_ADDRESS;
										OBJECT(object)->route_map_sign.route_map = current_object;
										OBJECT(object)->route_map_sign.kind = 0u;
										SCREEN(screen)->route_map_signals &= ~2u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003877u:
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											OBJECT(object)->stage_result_line.last = 1u;
										if (OBJECT(current_object)->route_map.first_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 6;
											OBJECT(current_object)->state++;
										}
										else
										{
											lengths_place = screen + 0x700u;
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - SCREEN(lengths_place - 0x700u)->stage_lengths[SCREEN(screen)->current_stage];
											branch_road_length = SCREEN(lengths_place - 0x700u)->branch_road_length;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, branch_road_length);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 3u * 2u, branch_road_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003908u:
										turn_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = turn_leg_frames;
										if ((int8_t)turn_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												scroll_x_address_5 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_5)[0] -= 8u;
												OBJECT(current_object)->x += 8u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											scroll_x_address_6 = OBJECT(current_object)->route_map.scroll_x_address;
											VIDEO_REGISTER_WORDS(scroll_x_address_6)[0] += 8u;
											OBJECT(current_object)->x -= 8u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.first_legs_left--;
										OBJECT(current_object)->state++;
									case 0x00394Eu:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xB;
											OBJECT(current_object)->state++;
										}
										else
										{
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											stage_length = SCREEN(screen)->stage_lengths[SCREEN(screen)->current_stage];
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, stage_length);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 5u * 2u + leg_distance, stage_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x0039B4u:
										approach_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = approach_leg_frames;
										if ((int8_t)approach_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												scroll_x_address_7 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_7)[0] -= 5u;
												OBJECT(current_object)->x += 5u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											scroll_x_address_8 = OBJECT(current_object)->route_map.scroll_x_address;
											VIDEO_REGISTER_WORDS(scroll_x_address_8)[0] += 5u;
											OBJECT(current_object)->x -= 5u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = route_map_sign_update_ADDRESS;
										OBJECT(object)->route_map_sign.route_map = current_object;
										OBJECT(object)->route_map_sign.kind = 0u;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										OBJECT(current_object)->state++;
										return;
									case 0x003A14u:
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										object = start_object();
										OBJECT(object)->handler = stage_result_line_draw_ADDRESS;
										OBJECT(object)->stage_result_line.stage = OBJECT(current_object)->route_map.stage;
										if (OBJECT(current_object)->route_map.checkpoint_legs_left == 0u)
											OBJECT(object)->stage_result_line.last = 1u;
										if (OBJECT(current_object)->route_map.first_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xA;
											OBJECT(current_object)->state++;
										}
										else
										{
											lengths_place = screen + 0x700u;
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start - SCREEN(lengths_place - 0x700u)->stage_lengths[SCREEN(screen)->current_stage];
											branch_road_length = SCREEN(lengths_place - 0x700u)->branch_road_length;
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 2, branch_road_length);
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 5u * 2u, branch_road_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003AA5u:
										branch_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = branch_leg_frames;
										if ((int8_t)branch_leg_frames >= 0)
										{
											if (mem_read8(screen + 0x34Au + (OBJECT(current_object)->route_map.stage - 1u)) != 0u)
											{
												if (OBJECT(current_object)->route_map.first_branch != 0u)
												{
													SCREEN(screen)->route_map_signals |= 1u;
													scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
													VIDEO_REGISTER_WORDS(scroll_y_address_4)[0] += 4u;
													scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
													VIDEO_REGISTER_WORDS(scroll_x_address_9)[0]--;
													OBJECT(current_object)->y -= 4u;
													OBJECT(current_object)->x++;
													return;
												}
												SCREEN(screen)->route_map_signals |= 2u;
												scroll_y_address_5 = OBJECT(current_object)->route_map.scroll_y_address;
												VIDEO_REGISTER_WORDS(scroll_y_address_5)[0] -= 4u;
												scroll_x_address_10 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_10)[0]++;
												OBJECT(current_object)->y += 4u;
												OBJECT(current_object)->x--;
												return;
											}
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 2u;
												scroll_y_address_5 = OBJECT(current_object)->route_map.scroll_y_address;
												VIDEO_REGISTER_WORDS(scroll_y_address_5)[0] -= 4u;
												scroll_x_address_10 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_10)[0]++;
												OBJECT(current_object)->y += 4u;
												OBJECT(current_object)->x--;
												return;
											}
											SCREEN(screen)->route_map_signals |= 1u;
											scroll_y_address_4 = OBJECT(current_object)->route_map.scroll_y_address;
											VIDEO_REGISTER_WORDS(scroll_y_address_4)[0] += 4u;
											scroll_x_address_9 = OBJECT(current_object)->route_map.scroll_x_address;
											VIDEO_REGISTER_WORDS(scroll_x_address_9)[0]--;
											OBJECT(current_object)->y -= 4u;
											OBJECT(current_object)->x++;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFFCu;
										OBJECT(current_object)->route_map.first_legs_left--;
										OBJECT(current_object)->state++;
									case 0x003B16u:
										if (OBJECT(current_object)->route_map.checkpoint_legs_left != 0u)
										{
											OBJECT(current_object)->route_map.leg_frames = 0xF;
											OBJECT(current_object)->state++;
										}
										else
										{
											leg_distance = (uint32_t)mem_read16(0x20FC42u + ((uint32_t)SCREEN(screen)->player << 7)) - CAR(player_car)->reached_stage_start;
											stage_length = SCREEN(screen)->stage_lengths[SCREEN(screen)->current_stage];
											OBJECT(current_object)->route_map.last_leg_progress = divide_s32(leg_distance << 3, stage_length);
											OBJECT(current_object)->route_map.last_leg_progress += 4u;
											OBJECT(current_object)->route_map.leg_frames = divide_s32(leg_distance * 3u * 5u, stage_length);
											SCREEN(screen)->route_map_signals |= 0x40u;
											OBJECT(current_object)->state++;
										}
									case 0x003B77u:
										stage_leg_frames = OBJECT(current_object)->route_map.leg_frames - 1u;
										OBJECT(current_object)->route_map.leg_frames = stage_leg_frames;
										if ((int8_t)stage_leg_frames >= 0)
										{
											if (OBJECT(current_object)->route_map.first_branch != 0u)
											{
												SCREEN(screen)->route_map_signals |= 8u;
												if ((int16_t)VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0] <= 0x10)
												{
													SCREEN(screen)->route_map_signals |= 0x20u;
													return;
												}
												scroll_x_address_11 = OBJECT(current_object)->route_map.scroll_x_address;
												VIDEO_REGISTER_WORDS(scroll_x_address_11)[0] -= 7u;
												OBJECT(current_object)->x += 7u;
												return;
											}
											SCREEN(screen)->route_map_signals |= 4u;
											if ((int16_t)VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0] >= 0x2A0)
											{
												SCREEN(screen)->route_map_signals |= 0x20u;
												return;
											}
											scroll_x_address_12 = OBJECT(current_object)->route_map.scroll_x_address;
											VIDEO_REGISTER_WORDS(scroll_x_address_12)[0] += 7u;
											OBJECT(current_object)->x -= 7u;
											return;
										}
										L_003BFC = (SCREEN(screen)->route_map_signals & 0x40u) != 0u;
										if (L_003BFC)
											break;
										SCREEN(screen)->kanji_text_number &= 0xFFF3u;
										OBJECT(current_object)->route_map.checkpoint_legs_left--;
										OBJECT(current_object)->route_map.stage++;
										if (OBJECT(current_object)->route_map.stage != 6u)
										{
											object = start_object();
											OBJECT(object)->handler = route_map_sign_update_ADDRESS;
											OBJECT(object)->route_map_sign.route_map = current_object;
											OBJECT(object)->route_map_sign.kind = 0u;
											OBJECT(current_object)->state = 0xBu;
											return;
										}
										L_003BFC = true;
										break;
									case 0x003C2Du:
										L_003C2D = true;
										break;
									case 0x003CF1u:
										zoom_x_address_2 = OBJECT(current_object)->route_map.zoom_x_address;
										VIDEO_REGISTER_WORDS(zoom_x_address_2)[0] -= 0x40u;
										zoom_y_address_2 = OBJECT(current_object)->route_map.zoom_y_address;
										VIDEO_REGISTER_WORDS(zoom_y_address_2)[0] -= 0x40u;
										scroll_x_address_13 = OBJECT(current_object)->route_map.scroll_x_address;
										VIDEO_REGISTER_WORDS(scroll_x_address_13)[0] += OBJECT(current_object)->x;
										scroll_y_address_6 = OBJECT(current_object)->route_map.scroll_y_address;
										VIDEO_REGISTER_WORDS(scroll_y_address_6)[0] += OBJECT(current_object)->y;
										send_video_registers = 0xFFu;
										timer_4 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_4 - 1u;
										if (timer_4 == 1u)
										{
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_x_address)[0] = 0x200u;
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.zoom_y_address)[0] = 0x200u;
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0] = route_map_whole_scroll_xs[OBJECT(current_object)->route_map.first_branch];
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0] = 0x70u;
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_x_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0];
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.register_scroll_y_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0];
											PAGE_SELECT(OBJECT(current_object)->route_map.second_pages_address)[0] = route_map_world_pages[SCREEN(screen)->index];
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_x_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_x_address)[0];
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_y_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.scroll_y_address)[0];
											second_scroll_x_address = OBJECT(current_object)->route_map.second_scroll_x_address;
											VIDEO_REGISTER_WORDS(second_scroll_x_address)[0] -= 0x88u;
											second_scroll_y_address = OBJECT(current_object)->route_map.second_scroll_y_address;
											VIDEO_REGISTER_WORDS(second_scroll_y_address)[0] -= 0x70u;
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_register_scroll_x_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_x_address)[0];
											VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_register_scroll_y_address)[0] = VIDEO_REGISTER_WORDS(OBJECT(current_object)->route_map.second_scroll_y_address)[0];
											send_video_registers = 0xFFu;
											OBJECT(current_object)->state++;
										}
										return;
									case 0x003D94u:
										io_write16(OBJECT(current_object)->route_map.sea_colour_address, 0u);
										object = start_object();
										OBJECT(object)->handler = route_map_direction_sign_show_ADDRESS;
										legs_left = OBJECT(current_object)->route_map.stage;
										if (legs_left != 6u)
											legs_left++;
										leg_number = 0u;
										do
										{
											object = start_object();
											OBJECT(object)->handler = route_map_leg_show_ADDRESS;
											OBJECT(object)->route_map_leg.leg = leg_number;
											OBJECT(object)->route_map_leg.progress = OBJECT(current_object)->route_map.last_leg_progress;
											OBJECT(object)->route_map_leg.map = current_object;
											leg_number++;
											legs_left--;
										} while (legs_left != 0u);
										dj_message = 0x38u;
										start_dj_message(dj_message);
										OBJECT(current_object)->route_map.timer = 0x5Au;
										OBJECT(current_object)->state++;
										return;
									case 0x003DE9u:
										if (SCREEN(screen)->dj_mouth_shapes == 0u)
										{
											dj_message = 0x39u;
											start_dj_message(dj_message);
											OBJECT(current_object)->state++;
										}
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										object = start_object();
										OBJECT(object)->handler = route_map_wait_banner_show_ADDRESS;
										OBJECT(current_object)->state++;
										return;
									case 0x003DFCu:
										timer_5 = OBJECT(current_object)->route_map.timer;
										OBJECT(current_object)->route_map.timer = timer_5 - 1u;
										if (timer_5 != 1u)
											return;
										SCREEN(screen)->game_step = 0x10u;
										object = start_object();
										OBJECT(object)->handler = route_map_wait_banner_show_ADDRESS;
										OBJECT(current_object)->state++;
										return;
									case 0x003E1Au:
										return;
									default:
										invalid_jump(jump_target);
									}
								} while (L_003BFC);
							} while (L_003BFC);
						} while (L_003BFC);
					} while (L_003BFC);
				} while (L_003BFC);
			} while (L_003BFC);
		} while (L_003BFC);
	} while (L_003C2D);
}

/* The player's car on the route map (animation route_map_car_animations by the car's model, palette
   route_map_car_palettes, size 50): 0 at x 8C, y C3 with frame C; 1 until the route map's stage is not
   0, drives up 4 pixels and right 1 a frame unless the map says it stands (screen route_map_signals bit
   7); 2 stays where it is while the map scrolls under it, its frame by the way the map says it goes (bit
   2 frame 9, bit 3 frame 4, bit 0 frame 6, bit 1 frame C), moving itself 7 pixels a frame sideways when
   the map is at its edge (bit 5); ends when the map zooms out (bit 4). Its depth layer follows its y
   (map_depth_from_y). */
void route_map_car_update(void)
{
	uint32_t animation = 0, car_model = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_car_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		car_model = OBJECT(current_object)->route_map_car.model;
		animation = route_map_car_animations[car_model];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_car_palettes[car_model];
		OBJECT(current_object)->width_scale = 0x50u;
		OBJECT(current_object)->height_scale = 0x50u;
		OBJECT(current_object)->x = 0x8Cu;
		OBJECT(current_object)->y = 0xC3u;
		OBJECT(current_object)->frame = 0xCu;
		map_depth_from_y();
		OBJECT(current_object)->state++;
		car_model = draw_object(car_model);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if (OBJECT(OBJECT(current_object)->route_map_car.route_map)->route_map.stage != 0u)
		{
			OBJECT(current_object)->state++;
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 0x80u) != 0u)
		{
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->y -= 4u;
		OBJECT(current_object)->x++;
		map_depth_from_y();
		routine = draw_object(routine);
		return;
	case 2:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 0x80u) != 0u)
		{
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 4u) != 0u)
		{
			OBJECT(current_object)->frame = 9u;
			if ((SCREEN(screen)->route_map_signals & 0x20u) != 0u)
				OBJECT(current_object)->x += 7u;
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 8u) != 0u)
		{
			OBJECT(current_object)->frame = 4u;
			if ((SCREEN(screen)->route_map_signals & 0x20u) != 0u)
			{
				OBJECT(current_object)->x -= 7u;
				map_depth_from_y();
				routine = draw_object(routine);
				return;
			}
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		if ((SCREEN(screen)->route_map_signals & 1u) == 0u)
		{
			if ((SCREEN(screen)->route_map_signals & 2u) == 0u)
			{
				map_depth_from_y();
				routine = draw_object(routine);
				return;
			}
			OBJECT(current_object)->frame = 0xCu;
			map_depth_from_y();
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->frame = 6u;
		map_depth_from_y();
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_car_state_jumps[state]);
	}
}

/* A sign on the route map, by kind: 0 C.P where the car passed a checkpoint (animation 2F9), 1 FINISH at
   the goal (2FA); palette 2CF, depth layer 195. State 0: x_offset and y_offset are the middle of the
   screen (A0, 70 less 8) less the map's place, where the car is; kind 1 goes to state 2; 1 follows the
   map (its x and y plus the offsets) and turns (animation_advance); 2 FINISH at the goal's place on the
   map (the map's x plus route_map_goal_xs by the goal, screen current_stage - 15; y 5C), drawn 4 frames
   of 8 (blink); both end when the map zooms out (screen route_map_signals bit 4). */
void route_map_sign_update(void)
{
	uint32_t animation = 0, goal = 0, sign_kind = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	sign_kind = route_map_sign_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		sign_kind = OBJECT(current_object)->route_map_sign.kind;
		animation = route_map_sign_animations[sign_kind];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_sign_palettes[sign_kind];
		OBJECT(current_object)->width_scale = 0x100u;
		OBJECT(current_object)->height_scale = 0x100u;
		OBJECT(current_object)->route_map_sign.x_offset = 0xA0;
		OBJECT(current_object)->route_map_sign.y_offset = 0x70;
		OBJECT(current_object)->route_map_sign.x_offset -= OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->route_map_sign.y_offset -= OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->route_map_sign.y_offset -= 8u;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		OBJECT(current_object)->depth_layer = 0x195u;
		if (OBJECT(current_object)->route_map_sign.kind == 1u)
		{
			OBJECT(current_object)->state = 2u;
			return;
		}
		OBJECT(current_object)->state++;
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		sign_kind = draw_object(sign_kind);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_sign.y_offset;
		animation_advance(0u);
		sign_kind = draw_object(sign_kind);
		return;
	case 2:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		goal = SCREEN(screen)->current_stage - 0x15u;
		OBJECT(current_object)->route_map_sign.x_offset = route_map_goal_xs[goal];
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_sign.route_map)->x;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_sign.x_offset;
		OBJECT(current_object)->y = 0x5Cu;
		OBJECT(current_object)->route_map_sign.blink--;
		if ((OBJECT(current_object)->route_map_sign.blink & 4u) == 0u)
			return;
		goal = draw_object(goal);
		return;
	default:
		invalid_jump(route_map_sign_state_jumps[state]);
	}
}

/* One of the 32 animated pictures on the close-up route map (people, animals and vehicles by the
   landmarks): 0 from its record (route_map_landmarks: 16 * the index in 9, the second set of 32 (+ 200)
   when screen route_branches is 1): animation, palette, size, x_offset and y_offset from the map's
   place, then field 9 from the record (set for one record: it then shows only every other frame); 1
   follows the map (its x and y plus the offsets), animation_advance and draw_object, with field 9 set
   only when toggle comes round; ends when the map zooms out (screen route_map_signals bit 4). Its depth
   layer follows its y (map_depth_from_y). */
void route_map_landmark_update(void)
{
	uint32_t animation = 0, landmark = 0, record_offset = 0, routine = 0;
	uint8_t frame_toggle = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_landmark_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		record_offset = ((uint32_t)OBJECT(current_object)->route_map_landmark.index_then_half_rate << 4) + ((uint32_t)SCREEN(screen)->route_branches[0] << 9);
		landmark = route_map_landmarks_ADDRESS + record_offset;
		animation = ROUTE_MAP_LANDMARK_RECORD(landmark)->animation;
		landmark += 2u;
		animation_start(animation);
		OBJECT(current_object)->palette = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x2u)->palette;
		landmark += 2u;
		OBJECT(current_object)->width_scale = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x4u)->width_scale;
		landmark += 2u;
		OBJECT(current_object)->height_scale = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x6u)->height_scale;
		landmark += 2u;
		OBJECT(current_object)->route_map_landmark.x_offset = ROUTE_MAP_LANDMARK_RECORD(landmark - 0x8u)->x_offset;
		landmark += 2u;
		OBJECT(current_object)->route_map_landmark.y_offset = ROUTE_MAP_LANDMARK_RECORD(landmark - 0xAu)->y_offset;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		OBJECT(current_object)->route_map_landmark.index_then_half_rate = (uint8_t)ROUTE_MAP_LANDMARK_RECORD(landmark - 0xAu)->half_rate;
		OBJECT(current_object)->state++;
		map_depth_from_y();
		record_offset = draw_object(record_offset);
		return;
	case 1:
		if ((SCREEN(screen)->route_map_signals & 0x10u) != 0u)
		{
			OBJECT(current_object)->flags = 0u;
			return;
		}
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_landmark.route_map)->y;
		OBJECT(current_object)->x += OBJECT(current_object)->route_map_landmark.x_offset;
		OBJECT(current_object)->y += OBJECT(current_object)->route_map_landmark.y_offset;
		if (OBJECT(current_object)->route_map_landmark.index_then_half_rate != 0u)
		{
			OBJECT(current_object)->route_map_landmark.toggle++;
			frame_toggle = OBJECT(current_object)->route_map_landmark.toggle & 1u;
			OBJECT(current_object)->route_map_landmark.toggle = frame_toggle;
			if (frame_toggle != 0u)
				return;
		}
		map_depth_from_y();
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_landmark_state_jumps[state]);
	}
}

/* Sets the object's depth layer (3C) from its y: (y + D0) >> 3, at most 19F, so that what stands lower
   on the route map is drawn in front. Used by route_map_car and route_map_landmark. */
void map_depth_from_y(void)
{
	OBJECT(current_object)->depth_layer = OBJECT(current_object)->y;
	OBJECT(current_object)->depth_layer += 0xD0u;
	OBJECT(current_object)->depth_layer = OBJECT(current_object)->depth_layer >> 3;
	if (OBJECT(current_object)->depth_layer == 0u)
	{
		OBJECT(current_object)->depth_layer = 0u;
		return;
	}
	if ((int16_t)OBJECT(current_object)->depth_layer >= 0x1A0)
	{
		OBJECT(current_object)->depth_layer = 0x19Fu;
		return;
	}
}

/* A line of the player's places by stage on the route map, with two players or more (ends at once with
   one): every frame, the stage's label (stage_result_labels by stage: PRESTG, STAGE1-4, RESULT) at x A8,
   y 40 + 16 * stage, and at x E8 the player's place at that stage (stage_result_places by screen 360 +
   player + 8 * stage: 1st-8th); the last line says Retire (stage_result_retire) instead, unless it is
   RESULT (the goal reached), where the player's final place (place_address) is drawn big (animation 4,
   draw mode 8, at x F0, y B0, size 160; palette 41 for 1st, 42 for the others). */
void stage_result_line_draw(void)
{
	uint16_t attribute = 0;
	uint32_t animation = 0, place_index = 0, place_palette = 0, place_text = 0, player = 0, retire_text = 0, stage_result_text = 0, text_place = 0;
	uint32_t saved_position = 0;
	uint8_t final_place = 0;

	player = SCREEN(screen)->player;
	if (RACE(SCREEN(screen)->host_record)->racer_count <= 1u)
	{
		OBJECT(current_object)->flags = 0u;
		OBJECT(current_object)->flags_high = 0u;
		return;
	}
	OBJECT(current_object)->stage_result_line.place_address = screen + 0x320u + player;
	stage_result_text = stage_result_labels_ADDRESS + OBJECT(current_object)->stage_result_line.stage * 3u * 3u;
	attribute = STAGE_RESULT_TEXT(stage_result_text)->attribute;
	text_place = (0x40u + (OBJECT(current_object)->stage_result_line.stage << 4)) << 16;
	text_place |= (uint16_t)(text_place + 0xA8u);
	stage_result_text += 2u;
	saved_position = text_place;
	draw_text_at(text_place, attribute, stage_result_text);
	text_place = (saved_position & ~0xFFFFu) | (uint16_t)(saved_position + 0x40u);
	player = SCREEN(screen)->player;
	place_index = mem_read8(screen + 0x360u + player + (OBJECT(current_object)->stage_result_line.stage << 3)) * 3u;
	if (OBJECT(current_object)->stage_result_line.last == 0u)
	{
		place_text = stage_result_places_ADDRESS + place_index * 2u;
		attribute = STAGE_RESULT_PLACE_TEXT(place_text)->attribute;
		place_text += 2u;
		draw_text_at(text_place, attribute, place_text);
		return;
	}
	if (OBJECT(current_object)->stage_result_line.stage != 5u)
	{
		attribute = stage_result_retire[0].attribute;
		retire_text = stage_result_retire_ADDRESS + 0x2u;
		draw_text_at(text_place, attribute, retire_text);
		return;
	}
	OBJECT(current_object)->draw_mode = 8u;
	OBJECT(current_object)->depth_layer = 0x19Eu;
	OBJECT(current_object)->x = 0xF0u;
	OBJECT(current_object)->y = 0xB0u;
	OBJECT(current_object)->width_scale = 0x160u;
	OBJECT(current_object)->height_scale = 0x160u;
	animation = PLACE_NUMBER;
	animation_start(animation);
	final_place = ~~PLAYER_PLACE(OBJECT(current_object)->stage_result_line.place_address)[0];
	OBJECT(current_object)->frame = final_place;
	place_palette = 0x41u;
	if (final_place != 0u)
		place_palette = 0x42u;
	OBJECT(current_object)->palette = place_palette;
	place_palette = draw_object(place_palette);
}

/* A leg of the route the player drove, drawn in red over the map shown after the race (003520), by
   state: 0 sets it up by its number (leg): 0 from the start to the first fork, 1 on from there by the
   direction taken (screen route_branches: 0 east, 1 west, the map's 09), 2-5 by the branch taken at each
   fork (screen route_branches + leg - 1) from the place of its stage (route_map_stage_xs,
   route_map_stage_ys by screen route_stages + leg - 2), the map shifted by the direction
   (route_map_direction_x_shifts) and 6B down; a leg driven already shows whole (frame 7, or B for 2-5),
   the leg the car is on (the map's stage) up to the frame before its progress, with the car on it
   (route_map_leg_car); legs 1-5 start the CP sign at their start (route_map_checkpoint), and leg 5, when
   the race reached the goal (race_copy finished), also the FINISH sign and the car at its end; 1 draws
   it. */
void route_map_leg_show(void)
{
	uint32_t animation = 0, fork_index = 0, leg_stage = 0, object = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_leg_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_leg_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	OBJECT(current_object)->palette = 0x2D1u;
	OBJECT(current_object)->width_scale = 0x100u;
	OBJECT(current_object)->height_scale = 0x100u;
	OBJECT(current_object)->depth_layer = 0x18Eu;
	switch (OBJECT(current_object)->route_map_leg.leg)
	{
	case 0:
		animation = ROUTE_MAP_LEG;
		animation_start(animation);
		OBJECT(current_object)->x = 0xFFF7u;
		OBJECT(current_object)->y = 0x2Bu;
		OBJECT(current_object)->depth_layer++;
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
			OBJECT(current_object)->frame = 7u;
		else
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
		}
		break;
	case 1:
		animation = route_map_first_fork_leg_animations[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
		animation_start(animation);
		OBJECT(current_object)->x = 0u;
		OBJECT(current_object)->y = 0u;
		object = start_object();
		OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
		OBJECT(object)->route_map_checkpoint.leg = current_object;
		if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
		{
			OBJECT(current_object)->frame = 7u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
		object = start_object();
		OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
		OBJECT(object)->route_map_leg_car.leg = current_object;
		OBJECT(current_object)->frame--;
		if ((int8_t)OBJECT(current_object)->frame < 0)
		{
			OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		fork_index = OBJECT(current_object)->route_map_leg.leg - 1u;
		animation = route_map_fork_leg_animations[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch * 2u + SCREEN(screen)->route_branches[(int32_t)fork_index]];
		animation_start(animation);
		leg_stage = mem_read8(screen + 0x343u + fork_index);
		OBJECT(current_object)->x = route_map_stage_xs[leg_stage];
		OBJECT(current_object)->y = route_map_stage_ys[leg_stage];
		object = start_object();
		OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
		OBJECT(object)->route_map_checkpoint.leg = current_object;
		OBJECT(current_object)->depth_layer += OBJECT(current_object)->route_map_leg.leg;
		if (OBJECT(current_object)->route_map_leg.leg != 5u)
		{
			if (OBJECT(current_object)->route_map_leg.leg != (uint8_t)OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.stage)
			{
				OBJECT(current_object)->frame = 0xBu;
				OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
				OBJECT(current_object)->frame = 0u;
			OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
			OBJECT(current_object)->y += 0x6Bu;
			OBJECT(current_object)->state++;
			return;
		}
		if (RACE(SCREEN(screen)->race_copy)->finished == 0u)
		{
			OBJECT(current_object)->frame = OBJECT(current_object)->route_map_leg.progress;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame--;
			if ((int8_t)OBJECT(current_object)->frame < 0)
			{
				OBJECT(current_object)->frame = 0u;
				OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
				OBJECT(current_object)->y += 0x6Bu;
				OBJECT(current_object)->state++;
				return;
			}
		}
		else
		{
			object = start_object();
			OBJECT(object)->handler = route_map_checkpoint_show_ADDRESS;
			OBJECT(object)->route_map_checkpoint.leg = current_object;
			OBJECT(object)->route_map_checkpoint.kind = 1u;
			OBJECT(current_object)->route_map_leg.progress = 0xBu;
			object = start_object();
			OBJECT(object)->handler = route_map_leg_car_show_ADDRESS;
			OBJECT(object)->route_map_leg_car.leg = current_object;
			OBJECT(current_object)->frame = 0xBu;
		}
		break;
	default:
		invalid_jump(route_map_leg_jumps[OBJECT(current_object)->route_map_leg.leg]);
	}
	OBJECT(current_object)->x += route_map_direction_x_shifts[OBJECT(OBJECT(current_object)->route_map_leg.map)->route_map.first_branch];
	OBJECT(current_object)->y += 0x6Bu;
	OBJECT(current_object)->state++;
}

/* The player's car on the map after the race, where the red line of its leg ends, by state: 0 sets it
   up: the sprite of its model (route_map_car_animations, route_map_car_palettes by car model) at a
   quarter size, its place and frame (which way it faces) from a record by the leg's progress
   (route_map_start_leg_car_places for leg 0, route_map_first_fork_leg_car_places for leg 1 by the
   direction, route_map_fork_leg_car_places for legs 2-5 by the direction and the branch, from the
   stage's place), shifted like the legs; 1 draws it. */
void route_map_leg_car_show(void)
{
	uint32_t animation = 0, fork_index = 0, leg_stage = 0, model = 0, place = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_leg_car_state_jumps[state];
	switch (state)
	{
	case 0:
		break;
	case 1:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_leg_car_state_jumps[state]);
	}
	OBJECT(current_object)->draw_mode = 0u;
	model = CAR(player_car)->model;
	animation = route_map_car_animations[model];
	animation_start(animation);
	OBJECT(current_object)->palette = route_map_car_palettes[model];
	OBJECT(current_object)->width_scale = 0x40u;
	OBJECT(current_object)->height_scale = 0x40u;
	OBJECT(current_object)->depth_layer = 0x194u;
	switch (OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg)
	{
	case 0:
		place = route_map_start_leg_car_places_ADDRESS + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3);
		OBJECT(current_object)->x = ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y = ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches[0]];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	case 1:
		place = route_map_first_fork_leg_car_places_ADDRESS + (((uint32_t)SCREEN(screen)->route_branches[0] << 6) + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3));
		OBJECT(current_object)->x = ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y = ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches[0]];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	case 2:
	case 3:
	case 4:
	case 5:
		fork_index = OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg - 1u;
		place = route_map_fork_leg_car_places_ADDRESS + (((SCREEN(screen)->route_branches[0] * 3u) << 6) + ((SCREEN(screen)->route_branches[(int32_t)fork_index] * 3u) << 5) + ((uint32_t)OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.progress << 3));
		leg_stage = mem_read8(screen + 0x343u + fork_index);
		OBJECT(current_object)->x = route_map_stage_xs[leg_stage];
		OBJECT(current_object)->y = route_map_stage_ys[leg_stage];
		OBJECT(current_object)->x += ROUTE_MAP_LEG_CAR_PLACE(place)->x;
		place += 2u;
		OBJECT(current_object)->y += ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->y;
		OBJECT(current_object)->x += route_map_direction_x_shifts[SCREEN(screen)->route_branches[0]];
		OBJECT(current_object)->y += 0x6Bu;
		OBJECT(current_object)->frame = (uint8_t)ROUTE_MAP_LEG_CAR_PLACE(place - 0x2u)->frame;
		OBJECT(current_object)->state++;
		return;
	default:
		invalid_jump(route_map_leg_car_leg_jumps[OBJECT(OBJECT(current_object)->route_map_leg_car.leg)->route_map_leg.leg]);
	}
}

/* Unused: ends the object (flags 0). It follows the code of route_map_leg_car_show's last state
   (draw_object), whose state table lists only states 0 and 1. */
void route_map_leg_car_end(void)
{
	OBJECT(current_object)->flags = 0u;
}

/* A sign on the map after the race, by state: 0 sets it up by its kind (route_map_sign_animations,
   route_map_sign_palettes): 0 the CP sign at the start of its leg (the fork it leaves from), 1 the
   FINISH sign beside the goal end of the last leg (route_map_finish_x_offsets,
   route_map_finish_y_offsets by the direction and the branch); 1 the CP sign turns (animation_advance)
   and is drawn; 2 FINISH blinks, drawn 4 frames out of 8 (blink). */
void route_map_checkpoint_show(void)
{
	uint32_t animation = 0, finish_index = 0, kind = 0, routine = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_checkpoint_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		kind = OBJECT(current_object)->route_map_checkpoint.kind;
		animation = route_map_sign_animations[kind];
		animation_start(animation);
		OBJECT(current_object)->palette = route_map_sign_palettes[kind];
		OBJECT(current_object)->width_scale = 0x100u;
		OBJECT(current_object)->height_scale = 0x100u;
		OBJECT(current_object)->x = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->x;
		OBJECT(current_object)->y = OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->y;
		OBJECT(current_object)->y -= 6u;
		OBJECT(current_object)->depth_layer = 0x194u;
		if (OBJECT(current_object)->route_map_checkpoint.kind == 1u)
		{
			finish_index = SCREEN(screen)->route_branches[0] * 2u + mem_read8(screen + 0x34Au + (OBJECT(OBJECT(current_object)->route_map_checkpoint.leg)->route_map_leg.leg - 1u));
			OBJECT(current_object)->x += route_map_finish_x_offsets[finish_index];
			OBJECT(current_object)->y += route_map_finish_y_offsets[finish_index];
			OBJECT(current_object)->state++;
		}
		OBJECT(current_object)->state++;
		return;
	case 1:
		animation_advance(0u);
		routine = draw_object(routine);
		return;
	case 2:
		OBJECT(current_object)->route_map_checkpoint.blink--;
		if ((OBJECT(current_object)->route_map_checkpoint.blink & 4u) == 0u)
			return;
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(route_map_checkpoint_state_jumps[state]);
	}
}

/* The WEST BOUND / EAST BOUND sign at the foot of the map after the race: a globe between the two,
   placed by the direction (screen route_branches) so that only the half of the way taken shows, by
   state: 0 sets it up below the screen; 1 rises 2 a frame to DC; 1 and 2 cycle its colours through four
   palettes from palette_base, one every 4 frames (colour_timer), and draw it. */
void route_map_direction_sign_show(void)
{
	uint32_t animation = 0, colour_cycle = 0;

	switch (OBJECT(current_object)->state)
	{
	case 0:
		OBJECT(current_object)->state++;
		OBJECT(current_object)->draw_mode = 0xAu;
		OBJECT(current_object)->depth_layer = 0x19Du;
		OBJECT(current_object)->x = route_map_direction_x_shifts[SCREEN(screen)->route_branches[0]];
		OBJECT(current_object)->y = 0x118u;
		animation = WEST_EAST_PANEL;
		OBJECT(current_object)->route_map_direction_sign.palette_base = 0x67u;
		animation_start(animation);
		return;
	case 1:
		OBJECT(current_object)->y -= 2u;
		if (OBJECT(current_object)->y == 0xDCu)
			OBJECT(current_object)->state++;
		colour_cycle = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		colour_cycle = (colour_cycle & ~0xFFu) | (uint8_t)(colour_cycle + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = colour_cycle;
		colour_cycle = (uint16_t)(((colour_cycle >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = colour_cycle;
		colour_cycle = draw_object(colour_cycle);
		return;
	case 2:
		colour_cycle = OBJECT(current_object)->route_map_direction_sign.colour_timer;
		colour_cycle = (colour_cycle & ~0xFFu) | (uint8_t)(colour_cycle + 1u);
		OBJECT(current_object)->route_map_direction_sign.colour_timer = colour_cycle;
		colour_cycle = (uint16_t)(((colour_cycle >> 2) & 3u) + OBJECT(current_object)->route_map_direction_sign.palette_base);
		OBJECT(current_object)->palette = colour_cycle;
		colour_cycle = draw_object(colour_cycle);
		return;
	default:
		invalid_jump(route_map_direction_sign_state_jumps[OBJECT(current_object)->state]);
	}
}

/* JUST A MOMENT PLEASE over the map after the race, while the game waits to go on, by state: 0 waits 32
   frames (timer), then sets it up at the centre of the screen, tiny; 1 grows to full size in 15 frames;
   2 drawn 15 frames; 3 hidden 5 frames, back to 2. */
void route_map_wait_banner_show(void)
{
	uint32_t animation = 0, routine = 0;
	uint8_t wait_count = 0, timer = 0, timer_2 = 0, timer_3 = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = route_map_wait_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->route_map_wait_banner.timer++;
		wait_count = OBJECT(current_object)->route_map_wait_banner.timer & 0x1Fu;
		OBJECT(current_object)->route_map_wait_banner.timer = wait_count;
		if (wait_count == 0u)
		{
			OBJECT(current_object)->draw_mode = 0xAu;
			animation = WAIT_A_MINUTE_BANNER;
			animation_start(animation);
			OBJECT(current_object)->palette = 0u;
			OBJECT(current_object)->width_scale = 4u;
			OBJECT(current_object)->height_scale = 4u;
			OBJECT(current_object)->x = 0xA0u;
			OBJECT(current_object)->y = 0x70u;
			OBJECT(current_object)->depth_layer = 0x19Fu;
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state++;
		}
		return;
	case 1:
		timer = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer - 1u;
		if (timer == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state++;
			routine = draw_object(routine);
			return;
		}
		OBJECT(current_object)->width_scale += 0x12u;
		OBJECT(current_object)->height_scale += 0x12u;
		routine = draw_object(routine);
		return;
	case 2:
		timer_2 = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer_2 - 1u;
		if (timer_2 == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 5u;
			OBJECT(current_object)->state++;
			routine = draw_object(routine);
			return;
		}
		routine = draw_object(routine);
		return;
	case 3:
		timer_3 = OBJECT(current_object)->route_map_wait_banner.timer;
		OBJECT(current_object)->route_map_wait_banner.timer = timer_3 - 1u;
		if (timer_3 == 1u)
		{
			OBJECT(current_object)->route_map_wait_banner.timer = 0xFu;
			OBJECT(current_object)->state--;
			return;
		}
		return;
	default:
		invalid_jump(route_map_wait_banner_state_jumps[state]);
	}
}

/* GAME OVER at the centre of the screen when a one-player race ends short of the goal, by state: 0 sets
   it up, tiny; 0 and 1 grow it to full size in 15 frames (timer); 2 draws it. */
void game_over_banner_show(void)
{
	uint32_t animation = 0, routine = 0;
	uint8_t timer = 0;
	uint32_t state = 0;

	state = OBJECT(current_object)->state;
	routine = game_over_banner_state_jumps[state];
	switch (state)
	{
	case 0:
		OBJECT(current_object)->draw_mode = 0xAu;
		animation = GAME_OVER_BANNER;
		animation_start(animation);
		OBJECT(current_object)->palette = 0x2Au;
		OBJECT(current_object)->width_scale = 4u;
		OBJECT(current_object)->height_scale = 4u;
		OBJECT(current_object)->x = 0xA0u;
		OBJECT(current_object)->y = 0x70u;
		OBJECT(current_object)->depth_layer = 0x19Eu;
		OBJECT(current_object)->game_over_banner.timer = 0xFu;
		OBJECT(current_object)->state++;
		break;
	case 1:
		break;
	case 2:
		routine = draw_object(routine);
		return;
	default:
		invalid_jump(game_over_banner_state_jumps[state]);
	}
	timer = OBJECT(current_object)->game_over_banner.timer;
	OBJECT(current_object)->game_over_banner.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(current_object)->state++;
		routine = draw_object(routine);
		return;
	}
	OBJECT(current_object)->width_scale += 0x12u;
	OBJECT(current_object)->height_scale += 0x12u;
	routine = draw_object(routine);
}
