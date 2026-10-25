/**
 *    Name:        game.h
 *    Description: prototypes of the game's functions and the addresses of the code it keeps in memory slots
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    24-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#ifndef ORUNNERS_GAME_H
#define ORUNNERS_GAME_H

#include "game/support.h"
#include "ram.h"

/* registers the whole program shares */
extern uint32_t r12, r14, current_object, player_car, screen, r27, r28, stack_pointer;

/* where the code the program keeps in memory slots (objects' handlers, hooks) is, as the game's own
   addresses: what it stores and compares, and what call_code() runs */
#define sub_0010EB_ADDRESS 0x0010EBu
#define sub_001141_ADDRESS 0x001141u
#define sub_0011AB_ADDRESS 0x0011ABu
#define car_presentation_update_ADDRESS 0x001687u
#define staff_credits_code_start_ADDRESS 0x001894u
#define staff_credits_code_watch_ADDRESS 0x0018A4u
#define staff_credits_code_show_ADDRESS 0x0018E5u
#define staff_credits_code_done_ADDRESS 0x0018FEu
#define car_name_start_ADDRESS 0x001980u
#define car_name_letter_update_ADDRESS 0x0019C0u
#define name_entry_update_ADDRESS 0x001EC2u
#define name_entry_letter_update_ADDRESS 0x002204u
#define name_entry_lcd_update_ADDRESS 0x0022F7u
#define name_entry_car_update_ADDRESS 0x002379u
#define name_entry_drivers_update_ADDRESS 0x0023C6u
#define name_entry_course_name_update_ADDRESS 0x002416u
#define ranking_update_ADDRESS 0x00247Au
#define ranking_background_update_ADDRESS 0x00254Du
#define ranking_title_left_update_ADDRESS 0x002612u
#define ranking_title_right_update_ADDRESS 0x00265Fu
#define ranking_course_banner_update_ADDRESS 0x0026ADu
#define ranking_record_start_ADDRESS 0x00270Cu
#define ranking_record_character_update_ADDRESS 0x00278Eu
#define ranking_record_car_update_ADDRESS 0x002870u
#define passing_traffic_start_ADDRESS 0x002933u
#define passing_traffic_update_ADDRESS 0x002952u
#define passing_car_update_ADDRESS 0x002998u
#define route_map_update_ADDRESS 0x003520u
#define route_map_car_update_ADDRESS 0x003E1Bu
#define route_map_sign_update_ADDRESS 0x003F27u
#define route_map_landmark_update_ADDRESS 0x004056u
#define stage_result_line_draw_ADDRESS 0x004152u
#define route_map_leg_show_ADDRESS 0x004240u
#define route_map_leg_car_show_ADDRESS 0x004421u
#define route_map_checkpoint_show_ADDRESS 0x00457Du
#define route_map_direction_sign_show_ADDRESS 0x00463Cu
#define route_map_wait_banner_show_ADDRESS 0x0046BBu
#define game_over_banner_show_ADDRESS 0x004771u
#define car_select_start_ADDRESS 0x005030u
#define car_select_run_ADDRESS 0x00503Du
#define car_attendant_run_ADDRESS 0x005526u
#define turntable_car_show_ADDRESS 0x0056B4u
#define turntable_scenery_show_ADDRESS 0x005762u
#define welcome_letter_start_ADDRESS 0x005862u
#define welcome_letter_show_ADDRESS 0x005883u
#define select_car_heading_show_ADDRESS 0x005908u
#define select_instruction_show_ADDRESS 0x0059B8u
#define transmission_choice_run_ADDRESS 0x005A1Du
#define automatic_panel_show_ADDRESS 0x005B45u
#define manual_panel_show_ADDRESS 0x005BD6u
#define transmission_label_start_automatic_ADDRESS 0x005C71u
#define transmission_label_start_manual_ADDRESS 0x005CA4u
#define transmission_label_follow_ADDRESS 0x005CE5u
#define driver_couple_start_ADDRESS 0x005D14u
#define car_caption_show_ADDRESS 0x005D5Cu
#define car_gears_caption_show_ADDRESS 0x005E10u
#define waiting_banner_show_ADDRESS 0x005EB4u
#define car_select_entries_run_ADDRESS 0x005F51u
#define choice_flag_start_ADDRESS 0x006070u
#define choice_flag_follow_ADDRESS 0x0060B1u
#define stacked_choice_flag_start_ADDRESS 0x006105u
#define stacked_choice_flag_follow_ADDRESS 0x00613Fu
#define choice_flag_number_start_ADDRESS 0x006173u
#define choice_flag_number_follow_ADDRESS 0x0061A2u
#define entry_countdown_start_ADDRESS 0x006356u
#define entry_countdown_count_ADDRESS 0x0063F9u
#define entry_countdown_done_ADDRESS 0x006463u
#define entry_banner_start_ADDRESS 0x0064E2u
#define entry_banner_blink_ADDRESS 0x006514u
#define entry_car_ring_start_ADDRESS 0x00652Au
#define entry_car_ring_draw_ADDRESS 0x006560u
#define entry_traffic_run_ADDRESS 0x00660Fu
#define entry_traffic_car_drive_ADDRESS 0x00664Eu
#define player_car_update_ADDRESS 0x006EF0u
#define rival_warning_update_ADDRESS 0x007C36u
#define rival_alarm_update_ADDRESS 0x007C9Eu
#define race_cars_start_ADDRESS 0x009590u
#define race_car_draw_ADDRESS 0x00974Cu
#define sub_00AB24_ADDRESS 0x00AB24u
#define sub_00AB36_ADDRESS 0x00AB36u
#define rev_meter_update_ADDRESS 0x00BFC1u
#define speedometer_update_ADDRESS 0x00C017u
#define speed_unit_update_ADDRESS 0x00C0C6u
#define gear_indicator_update_ADDRESS 0x00C10Du
#define shift_up_hint_update_ADDRESS 0x00C231u
#define place_count_update_ADDRESS 0x00C2ACu
#define place_number_update_ADDRESS 0x00C342u
#define lap_timer_update_ADDRESS 0x00C43Eu
#define stage_indicator_start_ADDRESS 0x00C553u
#define stage_indicator_draw_ADDRESS 0x00C585u
#define start_message_update_ADDRESS 0x00C59Cu
#define time_left_display_update_ADDRESS 0x00C65Fu
#define extend_time_message_start_ADDRESS 0x00C7ECu
#define extend_time_message_blink_ADDRESS 0x00C857u
#define stage_entry_update_ADDRESS 0x00C873u
#define goal_update_ADDRESS 0x00C8B6u
#define race_result_start_ADDRESS 0x00C9EDu
#define race_result_hold_ADDRESS 0x00CA44u
#define race_result_verdict_start_ADDRESS 0x00CA50u
#define race_result_verdict_blink_ADDRESS 0x00CAA8u
#define branch_arrow_update_ADDRESS 0x00CABCu
#define checkpoint_board_update_ADDRESS 0x00CBC0u
#define draw_object_ADDRESS 0x00EAE4u
#define road_object_stream_read_ADDRESS 0x0126FCu
#define crest_clip_update_ADDRESS 0x0127EEu
#define scenery_update_ADDRESS 0x01287Cu
#define flashing_scenery_update_ADDRESS 0x012906u
#define scenery_pair_update_ADDRESS 0x0129ABu
#define overhead_scenery_update_ADDRESS 0x0129F4u
#define aurora_update_ADDRESS 0x012A20u
#define fork_sign_update_ADDRESS 0x012ACFu
#define animated_scenery_update_ADDRESS 0x012AF4u
#define animated_scenery_pair_update_ADDRESS 0x012B84u
#define road_crosser_update_ADDRESS 0x012BAEu
#define kangaroo_update_ADDRESS 0x012CAAu
#define moving_scenery_update_ADDRESS 0x012DB7u
#define swooping_helicopter_update_ADDRESS 0x012EA2u
#define start_gate_update_ADDRESS 0x012FBCu
#define start_flagman_update_ADDRESS 0x01307Eu
#define flagman_update_ADDRESS 0x013104u
#define start_balloon_update_ADDRESS 0x0131A5u
#define ambient_sound_update_ADDRESS 0x01322Du
#define one_shot_sound_update_ADDRESS 0x013288u
#define west_east_panel_update_ADDRESS 0x013296u
#define santa_sleigh_update_ADDRESS 0x01333Cu
#define goal_parade_rightward_update_ADDRESS 0x01344Fu
#define sounding_scenery_update_ADDRESS 0x01351Eu
#define goal_parade_leftward_update_ADDRESS 0x0135CCu
#define goal_parade_dragon_update_ADDRESS 0x01369Au
#define traffic_spawner_update_ADDRESS 0x01376Cu
#define traffic_car_start_convoy_ADDRESS 0x01389Eu
#define traffic_car_update_ADDRESS 0x0138EBu
#define traffic_follower_update_ADDRESS 0x013AB7u
#define pulled_vehicle_update_ADDRESS 0x013AD8u
#define traffic_stopping_car_update_ADDRESS 0x013AF9u
#define traffic_figure_start_team_ADDRESS 0x013B56u
#define traffic_figure_update_ADDRESS 0x013BA3u
#define traffic_hit_sound_wait_ADDRESS 0x013D15u
#define fork_branch_start_right_ADDRESS 0x013DC4u
#define fork_branch_start_left_ADDRESS 0x013DD5u
#define fork_branch_draw_ADDRESS 0x013DE4u
#define race_car_occupant_update_ADDRESS 0x014108u
#define rider_gesture_update_ADDRESS 0x0141F4u
#define race_opening_update_ADDRESS 0x014470u
#define stage_end_sounds_update_ADDRESS 0x014514u
#define fork_camera_update_ADDRESS 0x01458Du
#define race_car_start_ending_ADDRESS 0x014694u
#define goal_actor_car_start_ADDRESS 0x014735u
#define goal_actor_driver_start_ADDRESS 0x014777u
#define goal_actor_passenger_start_ADDRESS 0x014782u
#define goal_actor_start_ADDRESS 0x014838u
#define goal_actor_run_ADDRESS 0x014846u
#define road_surface_update_ADDRESS 0x016B00u
#define road_tiles_change_update_ADDRESS 0x0171CEu
#define race_background_update_ADDRESS 0x017524u
#define race_background_lower_animation_wait_ADDRESS 0x0178C9u
#define race_background_lower_animation_cycle_ADDRESS 0x0178D5u
#define race_background_upper_animation_wait_ADDRESS 0x01794Cu
#define race_background_upper_animation_cycle_ADDRESS 0x017958u
#define palette_cycle_update_ADDRESS 0x017BFBu
#define sunset_fade_start_ADDRESS 0x017CAAu
#define sunset_fade_wait_ADDRESS 0x017D1Fu
#define sub_017DB0_ADDRESS 0x017DB0u
#define backdrop_near_start_ADDRESS 0x017E1Bu
#define backdrop_near_update_ADDRESS 0x017E5Bu
#define backdrop_landmark_start_ADDRESS 0x017EF2u
#define backdrop_landmark_update_ADDRESS 0x017F3Du
#define sub_018050_ADDRESS 0x018050u
#define backdrop_translucent_start_ADDRESS 0x0180CBu
#define backdrop_translucent_update_ADDRESS 0x018110u
#define race_background_fade_watch_update_ADDRESS 0x01854Au
#define palette_fade_prepare_ADDRESS 0x0185D2u
#define palette_fade_step_ADDRESS 0x01864Au
#define lcd_race_info_start_ADDRESS 0x018BA0u
#define lcd_race_info_update_ADDRESS 0x018BB2u
#define lcd_race_info_finish_ADDRESS 0x018E32u
#define lcd_race_info_paused_ADDRESS 0x018E98u
#define car_radio_start_ADDRESS 0x018F74u
#define car_radio_wait_start_ADDRESS 0x018FD3u
#define car_radio_read_buttons_ADDRESS 0x018FE0u
#define car_radio_update_ADDRESS 0x01900Au
#define song_title_show_ADDRESS 0x019322u
#define title_screen_update_ADDRESS 0x019CACu
#define title_car_drive_ADDRESS 0x019E6Bu
#define title_car_driver_update_ADDRESS 0x019F4Cu
#define title_car_passenger_update_ADDRESS 0x019F60u
#define title_palm_tree_update_ADDRESS 0x01A0E2u
#define title_logo_update_ADDRESS 0x01A18Cu
#define title_logo_copy_update_ADDRESS 0x01A29Cu
#define title_logo_car_update_ADDRESS 0x01A329u
#define title_logo_stripe_update_ADDRESS 0x01A3E9u
#define title_board_palms_update_ADDRESS 0x01A499u
#define title_seagull_fly_ADDRESS 0x01A536u
#define title_picture_draw_ADDRESS 0x01A672u
#define coin_prompt_update_ADDRESS 0x01A6CFu
#define coin_prompt_car_drive_ADDRESS 0x01A7C5u
#define coin_prompt_message_update_ADDRESS 0x01A834u
#define ignition_key_prompt_update_ADDRESS 0x01A8E2u
#define credit_display_update_ADDRESS 0x01AA01u
#define demo_captions_run_ADDRESS 0x01ADE8u
#define demo_caption_update_ADDRESS 0x01AE8Du
#define sub_01C392_ADDRESS 0x01C392u
#define sub_01C3E6_ADDRESS 0x01C3E6u
#define sub_01C5BA_ADDRESS 0x01C5BAu
#define sub_01C5DB_ADDRESS 0x01C5DBu
#define sub_01C928_ADDRESS 0x01C928u
#define sub_01CDE0_ADDRESS 0x01CDE0u
#define sub_01CE15_ADDRESS 0x01CE15u
#define sub_01CE97_ADDRESS 0x01CE97u
#define sub_01D111_ADDRESS 0x01D111u
#define sub_01D126_ADDRESS 0x01D126u
#define sub_01D13B_ADDRESS 0x01D13Bu
#define sub_01D3E4_ADDRESS 0x01D3E4u
#define sub_01D419_ADDRESS 0x01D419u
#define sub_01D620_ADDRESS 0x01D620u
#define sub_01D661_ADDRESS 0x01D661u
#define sub_01D840_ADDRESS 0x01D840u
#define sub_01D868_ADDRESS 0x01D868u
#define sub_01D87A_ADDRESS 0x01D87Au
#define sub_01D88F_ADDRESS 0x01D88Fu
#define sub_01D9F3_ADDRESS 0x01D9F3u
#define sub_01DA28_ADDRESS 0x01DA28u
#define sub_01DD1E_ADDRESS 0x01DD1Eu
#define sub_01DD2B_ADDRESS 0x01DD2Bu
#define sub_01DD58_ADDRESS 0x01DD58u
#define sub_01DEFD_ADDRESS 0x01DEFDu
#define sub_01E453_ADDRESS 0x01E453u
#define sub_01E4BA_ADDRESS 0x01E4BAu
#define sub_01E6EB_ADDRESS 0x01E6EBu
#define sub_01E703_ADDRESS 0x01E703u
#define sub_01E715_ADDRESS 0x01E715u
#define sub_01E72D_ADDRESS 0x01E72Du
#define sub_01EDC4_ADDRESS 0x01EDC4u
#define sub_01EDF1_ADDRESS 0x01EDF1u
#define sub_01EE66_ADDRESS 0x01EE66u
#define sub_01EEC1_ADDRESS 0x01EEC1u
#define sub_01EEEF_ADDRESS 0x01EEEFu
#define int_vector_40_ADDRESS 0x03F500u
#define int_vector_41_ADDRESS 0x03F794u
#define int_vector_42_ADDRESS 0x03F7A0u
#define int_vector_43_ADDRESS 0x03F7ACu
#define int_vector_44_ADDRESS 0x03F830u

/* game_00.c: the game's functions, part 00 (in the order of their V60 addresses) */
void boot(void);
uint32_t sub_0000C4(uint32_t r10, uint32_t r11);
uint32_t sub_0000FC(void);
void sub_00013C(uint32_t r0);
void sub_0002CA(void);
void sub_0002E1(void);
void sub_000310(uint8_t r4);
void sub_000327(uint32_t r0);
void sub_000337(uint32_t r0);
void sub_000354(uint16_t r0, uint8_t r4);
void sub_00036B(void);
void sub_000372(void);
void sub_000375(void);
void sub_0003BA(uint8_t r4, uint32_t r13, uint32_t *p_r0, uint32_t *p_r2);
uint32_t sub_0003ED(uint32_t r0, uint32_t r2, uint8_t r4, uint32_t r13);
uint32_t sub_000451(uint32_t r2, uint8_t r4, uint32_t r13);
void sub_0004A1(uint32_t *p_r2, uint32_t *p_r13);
uint32_t sub_0004B8(uint32_t r2, uint32_t r13);
void second_task(uint32_t entry_point);
uint32_t sub_000848(uint32_t r0, uint32_t r2);
uint32_t sub_0008F0(uint32_t r2);
void sub_000913(void);
void sub_000976(void);
void sub_000995(uint32_t r0);
uint32_t sub_0009F9(void);
void sub_000B16(void);
void sub_000BBC(uint8_t r0, uint32_t r2, uint32_t *p_r10, uint32_t *p_r11);
uint32_t sub_000BF3(void);
uint32_t sub_000C1C(uint32_t r10, uint32_t r11);
uint32_t sub_000C5A(void);
void sub_000D7D(void);
void sub_000D9F(void);
void sub_000EA4(void);
void sub_001000(void);
void sub_0010CD(void);
void sub_0010EB(void);
void sub_001141(void);
void sub_0011AB(void);
void sub_001440(void);
void sub_00144C(void);
void sub_001458(void);
void sub_001516(void);
uint32_t sub_001544(void);
void sub_001577(void);
void sub_001594(void);
void sub_0015A9(void);
uint32_t sub_0015C0(uint32_t left_caller);
void sub_0015D8(void);
void car_presentation_update(void);
void staff_credits_code_start(void);
void staff_credits_code_watch(uint32_t r0, uint32_t r2);
void staff_credits_code_show(void);
void staff_credits_code_done(void);
void staff_credits_code_read_controls(uint32_t r2, uint32_t *p_r0, uint32_t *p_zero);
uint32_t sub_001969(uint32_t r0, uint32_t r1, uint8_t r2, uint32_t r3);
void car_name_start(void);
void car_name_letter_update(void);
void sub_001D84(void);
void sub_001DCC(void);
uint32_t sub_001E89(void);
void name_entry_update(void);
void name_entry_letter_update(void);
void name_entry_lcd_update(void);
void name_entry_car_update(void);
void name_entry_drivers_update(void);
void name_entry_course_name_update(void);
void ranking_update(void);
void ranking_background_update(void);
void ranking_title_left_update(void);
void ranking_title_right_update(void);
void ranking_course_banner_update(void);
void ranking_record_start(void);
void ranking_record_character_update(void);
void ranking_record_car_update(void);
void passing_traffic_start(void);
void passing_traffic_update(void);
void passing_car_update(void);
void ranking_background_load(void);
void sub_002E74(void);
void sub_003407(void);
void sub_00340D(void);
void sub_003425(void);
void route_map_update(void);
void route_map_car_update(void);
void route_map_sign_update(void);
void route_map_landmark_update(void);
void map_depth_from_y(void);
void stage_result_line_draw(void);
void route_map_leg_show(void);
void route_map_leg_car_show(void);
void sub_004578(void);
void route_map_checkpoint_show(void);
void route_map_direction_sign_show(void);
void route_map_wait_banner_show(void);
void game_over_banner_show(void);
void car_select_start(void);
void car_select_run(void);
void car_select_turn_to_choice(void);
void car_select_update_rowscroll(void);
void car_select_skew_floor_next(void);
void car_select_skew_floor_previous(void);
void car_select_skew_floor(uint16_t r0);
void car_attendant_run(void);
void turntable_car_show(void);
void turntable_scenery_show(void);
void welcome_letter_start(void);
void welcome_letter_show(void);
void select_car_heading_show(void);
void select_instruction_show(void);
void transmission_choice_run(void);
void automatic_panel_show(void);
void manual_panel_show(void);
void transmission_label_start_automatic(void);
void transmission_label_start_manual(void);
void transmission_label_follow(void);
void driver_couple_start(void);
void car_caption_show(void);
void car_gears_caption_show(void);
void waiting_banner_show(void);
void car_select_entries_run(void);

/* game_01.c: the game's functions, part 01 (in the order of their V60 addresses) */
void choice_flag_start(void);
void choice_flag_follow(void);
void stacked_choice_flag_start(void);
void stacked_choice_flag_follow(void);
void choice_flag_number_start(void);
void choice_flag_number_follow(void);
uint32_t turntable_draw(uint32_t r1, uint32_t r2, uint32_t r3);
uint32_t car_select_wait_confirm(void);
void entry_countdown_start(void);
void entry_countdown_count(void);
void entry_countdown_done(void);
void entry_banner_start(void);
void entry_banner_blink(void);
void entry_car_ring_start(void);
void entry_car_ring_draw(void);
void entry_traffic_run(void);
void entry_traffic_car_drive(void);
void step_palette(void);
void player_car_update(void);
void player_car_move(void);
void player_car_steer_to_stop(void);
void player_car_check_place(void);
void player_car_publish(void);
void sub_007677(void);
void player_car_set_horizon(void);
void player_car_place_on_road(void);
void player_car_clear_hit(void);
void player_car_animate(void);
void player_car_steer_frame(uint32_t r9);
uint32_t player_car_view_height(void);
void player_car_move_view(void);
void player_car_check_rivals(void);
void rival_warning_update(void);
void rival_alarm_update(void);
void player_car_load_crash_physics(void);
void race_cars_start(void);
void race_car_draw(void);
uint32_t race_car_draw_spray(uint32_t r4, uint16_t r5, uint16_t r6, uint8_t r7, uint32_t r8, uint32_t r10, uint16_t r15, uint16_t r16);
uint32_t race_car_draw_part(uint32_t r4, uint16_t r5, uint16_t r6, uint32_t r8, uint32_t r10, uint32_t r11, uint32_t r13, uint16_t r15, uint16_t r16);
void race_car_put_frame(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13);
void race_car_put_sprite(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13);
uint32_t race_car_put_flat_sprite(uint32_t r10, uint16_t r13, uint16_t r17, uint16_t r18);
void player_car_load_gears(void);
void player_car_build_torque_curves(void);
void player_car_read_controls(void);
void player_car_drive(uint32_t *p_r11, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r17);
void player_car_world_position(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17);
void player_car_place_view(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17);
void player_car_engine(uint32_t r7, uint32_t r8, uint32_t r9, uint32_t r19);
void angles_to_offset(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t *p_r3, uint32_t *p_r4, uint32_t *p_r5);
void sub_00AB24(void);
void sub_00AB36(void);
void sub_00AB46(void);
void player_car_next_surfaces(void);
uint32_t player_car_road_drag(uint32_t r0, uint16_t r1, uint16_t r2, uint16_t r3);
void sub_00B65F(void);
void sub_00B660(uint32_t r13);
void sub_00B66E(uint32_t r13);
void sub_00B67C(void);
void sub_00B68A(void);
void sub_00B698(void);
void sub_00B699(uint32_t r13);
void sub_00B6A7(uint32_t r13);
void sub_00B6B5(void);
void sub_00B6C3(void);
void player_car_off_road_shake(void);
void rev_meter_update(void);
void speedometer_update(void);
void speed_unit_update(void);
void gear_indicator_update(void);
void shift_up_hint_update(void);
void place_count_update(void);
void place_number_update(void);
void lap_timer_update(void);
void sub_00C552(void);
void stage_indicator_start(void);
void stage_indicator_draw(void);
void start_message_update(void);
void time_left_display_update(void);
void time_digits_start(void);
void time_digits_draw(uint32_t r0);
void time_left_display_check_extension(void);
void extend_time_message_start(void);
void extend_time_message_blink(uint32_t r0);
void stage_entry_update(void);
void sub_00C8B5(void);
void goal_update(void);
void race_result_start(void);
void race_result_hold(void);
void race_result_verdict_start(void);
void race_result_verdict_blink(void);
void branch_arrow_update(void);
void checkpoint_board_update(void);
void checkpoint_board_draw_row(uint32_t r21);
void checkpoint_board_leader_time(uint32_t r0, uint32_t r1);
void sub_00CEA4(uint32_t r0, uint32_t r10);
void time_to_text(uint32_t r0, uint32_t r10);
void checkpoint_board_gap_time(uint32_t r0, uint32_t r1);
void sub_00D0C8(void);
void sub_00D13A(void);
void sub_00D298(void);
void sub_00D2BE(void);
void sub_00D2EB(void);
void sub_00D2F0(void);
void sub_00D341(uint32_t r0);
uint32_t sub_00D524(uint32_t left_caller);
uint32_t sub_00D544(uint32_t r0, uint32_t r10);
void sub_00D5E0(uint32_t *p_r2, uint32_t *p_left_caller);
uint32_t sub_00D612(uint32_t left_caller);
void sub_00D658(void);
void sub_00D664(uint32_t *p_r2, uint32_t *p_left_caller);
uint32_t sub_00D6AD(uint32_t r2, uint32_t left_caller);
void sub_00D6BB(uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D6C7(uint32_t r0, uint32_t r2);
void sub_00D6EC(void);
void sub_00D716(void);
void sub_00D72D(void);
void sub_00D7CB(uint32_t r19, uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D884(uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D8A5(void);

/* game_02.c: the game's functions, part 02 (in the order of their V60 addresses) */
void sub_00D9F3(void);
void sub_00DB6D(void);
void sub_00DDA8(void);
void sub_00DEC2(void);
void set_player_ready(void);
uint32_t players_not_ready(void);
void sub_00DEF8(void);
void sub_00E620(void);
void sub_00E638(void);
void sub_00E67C(uint32_t r1, uint32_t r10, uint32_t r11);
void sub_00E8DF(void);
uint32_t draw_object(uint32_t r0);
void sub_00EB74(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EC1B(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00ECCD(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00ED82(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EE37(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EE75(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEBF(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEF0(uint16_t r1, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEF4(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEFF(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EF32(uint32_t *p_r0, uint32_t *p_r10);
void sub_00EF4A(uint16_t r1, uint32_t r2, uint32_t r3, uint32_t *p_r0, uint32_t *p_r10);
void sub_00F04E(uint32_t *p_r0, uint32_t *p_r10);
uint32_t sub_00F104(uint32_t r10);
uint32_t sub_00F126(uint32_t r10);
void sub_00F15C(uint32_t *p_r0, uint32_t *p_r10);
uint32_t animation_advance(uint32_t r7);
void animation_start(uint32_t r9);
void sub_00F396(void);
void road_row_draw(uint32_t r7, uint32_t r15);
void road_row_pair_draw(uint32_t r7, uint32_t r15);
void player_car_start_course(void);
void player_car_extend_course(void);
void player_car_build_segment(uint16_t r5);
void player_car_project_road(void);
void sub_010243(void);
void rotate_offset(uint32_t r0, uint32_t r3, uint32_t r4, uint32_t *p_r1, uint32_t *p_r2);
uint32_t sub_010634(void);
void sub_010688(void);
uint32_t sub_0106AF(void);
uint32_t sub_01070C(void);
void sub_010741(void);
void sub_01076A(void);
void sub_01078F(uint16_t r9);
void sub_0107A0(uint16_t r9);
void sub_0107BC(void);
void sub_0107F0(void);
void sub_010889(void);
void sub_010890(void);
void sub_0108EA(void);
void sub_010936(void);
void sub_010996(uint16_t r0, uint32_t r10, uint32_t r11);
void sub_010DC4(uint32_t r10, uint32_t r11);
uint32_t sub_010E42(uint32_t r2);
void sub_010F66(uint32_t r10);
void sub_010FA1(uint32_t r10);
void sub_010FA6(uint32_t r10);
void queue_tile_pages(uint32_t r10);
void sub_011018(void);
void sub_01106C(uint32_t r6);
void sub_0111CB(uint8_t r1, uint32_t r7);
void sub_011213(void);
uint32_t sub_01122C(uint32_t r9, uint32_t r10, uint32_t r11);
uint32_t sub_011252(uint32_t r6, uint32_t r9, uint32_t r19);
void sub_0112D9(void);
void sub_0112F0(uint32_t r7);
void sub_011314(uint32_t r7);
void sub_011324(uint32_t r7, uint16_t r9);
void sub_011341(uint32_t r7, uint32_t r9);
void sub_011362(uint32_t r7, uint8_t r9);
void sub_01137D(uint32_t r7, uint16_t r9);
void sub_011388(uint32_t r7, uint32_t r9);
void sub_011393(uint32_t r7, uint8_t r9);
void sub_01139E(void);
void sub_0113C0(void);
uint32_t sub_0113E5(uint32_t r0);
uint32_t sub_01140E(uint32_t r0, uint32_t r2, uint32_t r3);
uint32_t sub_011418(uint32_t r0);
void sub_01142A(void);
uint32_t sub_01143E(uint32_t r0, uint32_t r2);
void sub_011594(void);
void queue_stage_load(void);
void queue_palette(uint32_t r19);
void queue_palette_list(uint32_t r19);
void queue_palette_upload(uint32_t r8, uint32_t r19);
void upload_queued_palettes(void);
void upload_palette(uint32_t r19);
void upload_palette_list(uint32_t r19);
void sub_01167E(void);
void sub_0117E4(void);
void sub_0117FE(void);
void sub_011868(uint32_t r19);
uint32_t sub_011874(uint32_t r19);
void sub_0118BD(uint32_t r19);
void sub_0118D2(uint32_t r19);
void sub_0118F7(uint32_t r19);
void sub_01190C(uint32_t r0, uint16_t r1, uint32_t r11);
void sub_011914(uint32_t r0, uint16_t r1, uint32_t r11);
void sub_01191C(uint32_t r0, uint16_t r1, uint32_t r11);
void sub_011928(uint32_t r0, uint16_t r1, uint32_t r11);
void sub_01192B(uint32_t r0, uint16_t r1, uint32_t r2, uint32_t r11);
void sub_011958(void);
uint32_t binary_to_bcd(uint32_t r0);
void draw_text_at(uint32_t r3, uint16_t r5, uint32_t r18);
uint32_t draw_text(uint32_t r19);
uint32_t sub_011A72(uint32_t r1, uint32_t r2, uint32_t r3, uint16_t r4, uint16_t r5, uint32_t r10, uint32_t r11, uint32_t r18);
void sub_011C60(void);
void run_objects(void);
uint32_t start_object(void);
void sub_011DEB(void);
void sub_011E1D(uint16_t r21);
uint32_t start_object_list(uint32_t r10);
uint32_t random_number(void);
void sub_011E88(void);
void request_sound(uint8_t r0);
void queue_sound(uint8_t r0);
void sub_011EF4(uint8_t r0);
void player_car_start_engine_sound(void);

/* game_03.c: the game's functions, part 03 (in the order of their V60 addresses) */
void player_car_engine_sound(void);
void set_tyre_noise_on_road(uint8_t r0);
void set_tyre_noise(uint8_t r0);
void sub_011FE6(void);
void sub_011FEA(uint8_t r0);
void sub_012004(void);
uint32_t vector_angle(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);
void vector_angles(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t *p_r0, uint32_t *p_r1);
void player_car_build_hit_boxes(void);
uint32_t player_car_write_hit_box(uint16_t r1, uint32_t r2, uint32_t r11, uint16_t r15, uint16_t r17);
void fork_sign_hit_check(uint32_t *p_r7, uint32_t *p_r15);
void road_row_crash_check(uint32_t *p_r7, uint32_t *p_r15);
void road_row_hit_check(uint32_t *p_r7, uint32_t *p_r15);
void road_row_pair_crash_check(uint32_t *p_r7, uint32_t *p_r15);
void sub_01248C(void);
void player_car_check_car_hits(void);
uint32_t traffic_hit_check(void);
void road_object_stream_read(void);
uint32_t road_object_stream_start_object(uint16_t r0, uint32_t r10);
uint32_t road_object_stream_stage_end(void);
uint32_t road_object_stream_fork(uint32_t r10);
uint32_t road_object_stream_end(void);
void crest_clip_update(void);
void scenery_update(void);
void road_row_start(void);
void flashing_scenery_update(void);
void scenery_pair_update(void);
void overhead_scenery_update(void);
void aurora_update(void);
void aurora_count_down(void);
void aurora_next_animation(void);
void fork_sign_update(void);
void animated_scenery_update(void);
void road_row_start_copy(void);
void animated_scenery_pair_update(void);
void road_crosser_update(void);
void kangaroo_update(void);
void moving_scenery_update(void);
void swooping_helicopter_update(void);
void start_gate_update(void);
void start_flagman_update(void);
void flagman_update(void);
void start_balloon_update(void);
void ambient_sound_update(void);
void one_shot_sound_update(void);
void west_east_panel_update(void);
void santa_sleigh_update(void);
void goal_parade_rightward_update(void);
void sounding_scenery_update(void);
void goal_parade_leftward_update(void);
void goal_parade_dragon_update(void);
void traffic_spawner_update(void);
void traffic_car_start_convoy(void);
void traffic_car_update(uint32_t r1);
void traffic_car_start(void);
void traffic_car_draw(uint32_t r0, uint32_t r1);
void traffic_follower_update(void);
void pulled_vehicle_update(void);
void traffic_stopping_car_update(void);
void traffic_figure_start_team(void);
void traffic_figure_update(void);
void traffic_start_hit_sound(void);
void traffic_hit_sound_wait(void);
uint32_t traffic_out_of_view(uint16_t r0);
void fork_branch_start_right(void);
void fork_branch_start_left(void);
void fork_branch_draw(void);
void fork_branch_sprite(uint32_t *p_r13, uint32_t *p_r15);
void race_car_occupant_update(void);
void rider_gesture_update(void);
uint32_t rider_gesture_check_view(uint32_t r10);
uint32_t rider_gesture_check_place(uint8_t r9, uint32_t r10);
void rider_gesture_place(uint16_t r0, uint32_t r10);
void race_opening_update(void);
void race_opening_sounds_start(void);
void race_opening_sounds_play(void);
void race_opening_sounds_done(void);
void stage_end_sounds_update(void);
void fork_camera_update(void);
void race_car_start_ending(void);
void goal_actor_car_start(void);
void goal_actor_driver_start(void);
void goal_actor_passenger_start(void);
void goal_actor_rider_place(uint32_t r10);
void goal_actor_start(void);
void goal_actor_run(void);
void road_surface_update(void);
void road_surface_start(void);
void road_surface_lines(uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r16, uint32_t *p_r18);
void road_surface_halfway_lines(uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r13, uint8_t r15, uint32_t r16, uint16_t r18);
void road_surface_fill_lines(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3, uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r7, uint32_t *p_r8, uint32_t *p_r11, uint32_t *p_zero);
void road_surface_fill_skipped_lines(uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r5, uint32_t *p_r6);
void road_surface_start_tiles_change(void);
void road_tiles_change_update(void);
void race_background_update(void);
void race_background_start_lower_animation(void);
void race_background_lower_animation_wait(void);
void race_background_lower_animation_cycle(void);
void race_background_start_upper_animation(uint32_t r0);
void race_background_upper_animation_wait(void);
void race_background_upper_animation_cycle(void);
void set_up_background(uint32_t r19);
void race_background_start_objects(uint32_t r0);
void palette_cycle_update(void);
void sunset_fade_start(void);
void sunset_fade_wait(void);
void sub_017D70(void);
void sub_017DB0(void);
void backdrop_near_start(void);
void backdrop_near_update(void);
void backdrop_landmark_start(void);
void backdrop_landmark_update(void);
void sub_018009(void);
void sub_018050(void);
void backdrop_translucent_start(void);
void backdrop_translucent_update(void);
uint32_t backdrop_off_screen(void);
void start_palette_fades(uint32_t r19);
void road_surface_fade_back(uint32_t r19);
void race_background_fade_palettes(uint32_t r19);

/* game_04.c: the game's functions, part 04 (in the order of their V60 addresses) */
void race_background_fade_watch_update(void);
uint32_t start_palette_fade(uint8_t r0, uint8_t r1, uint32_t r10, uint32_t r11);
void palette_fade_prepare(void);
void palette_fade_step(uint32_t r0);
void sub_0186FC(void);
uint32_t sub_0187A3(uint32_t r0);
void sub_0187C1(uint8_t r0);
void sub_0187F0(uint8_t r0);
void sub_0187F8(uint8_t r0);
void sub_018827(void);
void sub_018863(void);
void sub_0188BE(void);
void sub_0189D9(void);
void clear_lcd(void);
void lcd_race_info_start(void);
void lcd_race_info_update(void);
void lcd_race_info_time_page(void);
void lcd_race_info_time(void);
void lcd_race_info_average_page(void);
void lcd_race_info_average(void);
void lcd_race_info_write_number(uint32_t r0, uint32_t r10);
void lcd_race_info_finish(void);
void lcd_race_info_paused(void);
void car_radio_start(void);
void car_radio_wait_start(void);
void car_radio_read_buttons(void);
void car_radio_update(void);
void car_radio_dj_leader(void);
void car_radio_dj_own_car(void);
void car_radio_dj_remark_2a(void);
void car_radio_dj_remark_2b(void);
void car_radio_dj_remark_2d(void);
void car_radio_dj_remark_2f(void);
void car_radio_dj_remark_2c(void);
uint32_t car_radio_song(uint32_t r0);
void car_radio_show_title(uint32_t r10);
uint32_t start_song_title(uint32_t r10);
void song_title_show(void);
void start_dj_message_if_quiet(uint8_t r0);
void start_dj_message(uint8_t r0);
void title_screen_update(void);
void title_car_drive(void);
void title_car_driver_update(void);
void title_car_passenger_update(void);
void title_car_rider_draw(void);
void title_screen_start_palms(void);
void title_palm_tree_update(void);
void title_logo_update(void);
void title_logo_copy_update(void);
void title_logo_car_update(void);
void title_logo_stripe_update(void);
void title_board_palms_update(void);
void title_seagull_fly(void);
void title_picture_draw(void);
void coin_prompt_update(void);
void coin_prompt_car_drive(void);
void coin_prompt_message_update(void);
uint32_t coin_prompt_start_key_prompt(uint32_t r1, uint8_t r2);
void ignition_key_prompt_update(void);
void credit_display_update(void);
void credit_display_digit(void);
void demo_captions_run(void);
void demo_captions_head_to_head(void);
void demo_captions_checkpoint(void);
void demo_captions_music_select(void);
void demo_captions_start(uint8_t r0, uint32_t r1);
void demo_caption_update(void);
void sub_01AFDC(void);
void sub_01B1B1(uint32_t r11, uint32_t *p_r4, uint32_t *p_r10);
void sub_01B1CD(uint32_t r11, uint32_t r13);
uint32_t sub_01B1E7(uint32_t r13);
uint32_t sub_01B200(uint32_t r13);
void sub_01B20C(uint32_t r3, uint16_t r4);
void sub_01C241(void);
void sub_01C388(void);
void sub_01C392(void);
void sub_01C3E6(void);
uint32_t sub_01C435(uint32_t r0);
void sub_01C5BA(void);
void sub_01C5DB(void);
void sub_01C928(uint16_t r0);
uint32_t sub_01C93B(uint32_t r0);
void sub_01C98E(uint32_t r10, uint32_t *p_r0, uint32_t *p_r1);
uint32_t sub_01C9BE(uint32_t r0, uint32_t r10);
uint32_t sub_01CA5D(uint32_t r0, uint32_t r10);
uint32_t sub_01CAF8(uint32_t r0, uint32_t r10);
uint32_t sub_01CB8F(uint32_t r0, uint32_t r10);
void sub_01CC80(void);
void sub_01CDE0(void);
void sub_01CE15(uint32_t r0);
void sub_01CE97(void);
uint32_t sub_01CEDB(uint32_t r0);
void sub_01D111(void);
void sub_01D126(uint32_t r0);
void sub_01D13B(void);
uint32_t sub_01D151(uint32_t r0);
void sub_01D3E4(void);
void sub_01D419(void);
void sub_01D49A(void);
void sub_01D4C7(void);
void sub_01D4DF(void);
void sub_01D620(void);
void sub_01D661(void);
void sub_01D6BD(void);
void sub_01D6E7(void);
void sub_01D72C(void);
void sub_01D771(void);
void sub_01D840(void);
void sub_01D868(uint16_t r0);
void sub_01D87A(void);
void sub_01D88F(uint32_t r0);
uint32_t sub_01D8B9(uint32_t r0);
uint32_t sub_01D927(uint16_t r0, uint32_t r10);
uint32_t sub_01D957(uint32_t r0);
void sub_01D9F3(void);
void sub_01DA28(void);
uint32_t sub_01DA7C(void);
void sub_01DB58(void);
void sub_01DB68(void);
void sub_01DB85(void);

/* game_05.c: the game's functions, part 05 (in the order of their V60 addresses) */
void sub_01DB97(void);
void sub_01DBA9(void);
void sub_01DD1E(void);
void sub_01DD2B(void);
void sub_01DD58(uint32_t r1);
void sub_01DDAC(uint32_t *p_r0, uint32_t *p_r1);
void sub_01DE56(void);
void sub_01DEC7(uint32_t r1);
void sub_01DEFD(void);
uint32_t sub_01DF51(uint32_t r1);
uint32_t sub_01E02E(uint32_t r5, uint32_t r10);
void sub_01E04F(uint32_t r1);
void sub_01E053(uint32_t r0, uint32_t r1);
void sub_01E073(uint32_t r1);
void sub_01E09B(uint32_t r1);
void sub_01E0A1(void);
void sub_01E0B0(void);
void sub_01E453(void);
void sub_01E4BA(void);
uint32_t sub_01E523(void);
void sub_01E57F(uint32_t r0);
void sub_01E5C3(void);
void sub_01E5E7(void);
void sub_01E5F7(void);
void sub_01E6EB(void);
void sub_01E703(uint16_t r0);
void sub_01E715(void);
void sub_01E72D(uint16_t r0);
uint32_t sub_01E740(void);
uint32_t sub_01E862(void);
uint32_t sub_01E9A4(uint32_t r0, uint32_t r11);
void sub_01E9E6(uint32_t *p_r0, uint32_t *p_r11);
void sub_01EA14(void);
void sub_01EA4B(uint32_t r0, uint32_t r10);
void sub_01EDC4(void);
void sub_01EDF1(void);
void sub_01EE66(void);
void sub_01EEC1(void);
void sub_01EEEF(void);
uint32_t sub_01EF6E(void);
void sub_01F02C(uint32_t r0);
void sub_01F034(void);
void sub_01F043(void);
void sub_01F052(uint32_t r0);
uint32_t sub_01F085(uint32_t r0, uint32_t r10);
void sub_01F09A(void);
uint32_t sub_01F0CA(uint32_t r0, uint32_t r10);
void sub_01F0ED(uint32_t r0, uint32_t r2, uint32_t r3, uint32_t *p_r1, uint32_t *p_r10);
void sub_01F103(uint32_t r2, uint32_t *p_r0, uint32_t *p_r1, uint32_t *p_r10);
uint32_t sub_01F131(uint32_t r0, uint8_t r1, uint32_t r2, uint32_t r10);
uint32_t sub_01F16C(uint32_t r0, uint32_t r10);
uint32_t sub_01F17D(void);
void sub_01F190(void);
void sub_01F237(void);
void sub_01F266(uint16_t r0);
void sub_01F26B(uint16_t r0);
void int_vector_40(void);
void sub_03F62A(void);
void sub_03F6A0(void);
void int_vector_41(void);
void int_vector_42(void);
void int_vector_43(void);
void int_vector_44(void);
void int_default(void);
void int_vector_02(void);
void sub_03F8C9(uint32_t r0);
void sub_03F8DA(void);
void sub_03F922(void);
void sub_03F96B(void);
void sub_03FA54(uint32_t r10, uint32_t r11);
void reset(void);

#endif
