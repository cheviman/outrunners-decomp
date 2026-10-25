/**
 *    Name:        table.c
 *    Description: the registers the whole program shares, the code calls through memory slots reach (by address), and the reset entry
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

uint32_t r12, r14, current_object, player_car, screen, r27, r28, stack_pointer = 0x21F400u;

/* the same, for the runtime to keep a copy for each of the game's tasks */
uint32_t *const game_context[] = { &r12, &r14, &current_object, &player_car, &screen, &r27, &r28, &stack_pointer };
const uint8_t game_context_registers[] = { 12, 14, 20, 23, 25, 27, 28, 32 };
const size_t game_context_count = 8;

/* How call_code() runs second_task from a slot that holds its address: with 0 for what it takes, and
   what it hands back dropped. */
static void call_0006B0(void)
{
	second_task(0u);
}

/* How call_code() runs staff_credits_code_watch from a slot that holds its address: with 0 for what it
   takes, and what it hands back dropped. */
static void call_0018A4(void)
{
	staff_credits_code_watch(0, 0);
}

/* How call_code() runs extend_time_message_blink from a slot that holds its address: with 0 for what it
   takes, and what it hands back dropped. */
static void call_00C857(void)
{
	extend_time_message_blink(0);
}

/* How call_code() runs draw_object from a slot that holds its address: with 0 for what it takes, and
   what it hands back dropped. */
static void call_00EAE4(void)
{
	draw_object(0);
}

/* How call_code() runs traffic_car_update from a slot that holds its address: with 0 for what it takes,
   and what it hands back dropped. */
static void call_0138EB(void)
{
	traffic_car_update(0);
}

/* How call_code() runs palette_fade_step from a slot that holds its address: with 0 for what it takes,
   and what it hands back dropped. */
static void call_01864A(void)
{
	palette_fade_step(0);
}

/* How call_code() runs test_mode_memory_test_run from a slot that holds its address: with 0 for what it
   takes, and what it hands back dropped. */
static void call_01C928(void)
{
	test_mode_memory_test_run(0);
}

/* How call_code() runs test_mode_volume_adjustment_select from a slot that holds its address: with 0 for
   what it takes, and what it hands back dropped. */
static void call_01CE15(void)
{
	test_mode_volume_adjustment_select(0);
}

/* How call_code() runs test_mode_input_test_run from a slot that holds its address: with 0 for what it
   takes, and what it hands back dropped. */
static void call_01D126(void)
{
	test_mode_input_test_run(0);
}

/* How call_code() runs test_mode_crt_test_bars_run from a slot that holds its address: with 0 for what
   it takes, and what it hands back dropped. */
static void call_01D868(void)
{
	test_mode_crt_test_bars_run(0);
}

/* How call_code() runs test_mode_crt_test_grid_run from a slot that holds its address: with 0 for what
   it takes, and what it hands back dropped. */
static void call_01D88F(void)
{
	test_mode_crt_test_grid_run(0);
}

/* How call_code() runs test_mode_coin_assignments_run from a slot that holds its address: with 0 for
   what it takes, and what it hands back dropped. */
static void call_01DD58(void)
{
	test_mode_coin_assignments_run(0);
}

/* How call_code() runs test_mode_bookkeeping_wait from a slot that holds its address: with 0 for what it
   takes, and what it hands back dropped. */
static void call_01E703(void)
{
	test_mode_bookkeeping_wait(0);
}

/* How call_code() runs test_mode_bookkeeping_second_page_wait from a slot that holds its address: with 0
   for what it takes, and what it hands back dropped. */
static void call_01E72D(void)
{
	test_mode_bookkeeping_second_page_wait(0);
}

/* the code calls through memory slots and interrupts can reach, by address */
const game_code_entry game_code[] =
{
	{ 0x0006B0u, call_0006B0 },
	{ 0x000EA4u, isdn_status_update },
	{ 0x0010EBu, bar_picture_bar_start },
	{ 0x001141u, bar_picture_bar_fly },
	{ 0x0011ABu, bar_picture_bar_draw },
	{ 0x001687u, car_presentation_update },
	{ 0x001894u, staff_credits_code_start },
	{ 0x0018A4u, call_0018A4 },
	{ 0x0018E5u, staff_credits_code_show },
	{ 0x0018FEu, staff_credits_code_done },
	{ 0x001980u, car_name_start },
	{ 0x0019C0u, car_name_letter_update },
	{ 0x001EC2u, name_entry_update },
	{ 0x002204u, name_entry_letter_update },
	{ 0x0022F7u, name_entry_lcd_update },
	{ 0x002379u, name_entry_car_update },
	{ 0x0023C6u, name_entry_drivers_update },
	{ 0x002416u, name_entry_course_name_update },
	{ 0x00247Au, ranking_update },
	{ 0x00254Du, ranking_background_update },
	{ 0x002612u, ranking_title_left_update },
	{ 0x00265Fu, ranking_title_right_update },
	{ 0x0026ADu, ranking_course_banner_update },
	{ 0x00270Cu, ranking_record_start },
	{ 0x00278Eu, ranking_record_character_update },
	{ 0x002870u, ranking_record_car_update },
	{ 0x002933u, passing_traffic_start },
	{ 0x002952u, passing_traffic_update },
	{ 0x002998u, passing_car_update },
	{ 0x003520u, route_map_update },
	{ 0x003E1Bu, route_map_car_update },
	{ 0x003F27u, route_map_sign_update },
	{ 0x004056u, route_map_landmark_update },
	{ 0x004152u, stage_result_line_draw },
	{ 0x004240u, route_map_leg_show },
	{ 0x004421u, route_map_leg_car_show },
	{ 0x00457Du, route_map_checkpoint_show },
	{ 0x00463Cu, route_map_direction_sign_show },
	{ 0x0046BBu, route_map_wait_banner_show },
	{ 0x004771u, game_over_banner_show },
	{ 0x005030u, car_select_start },
	{ 0x00503Du, car_select_run },
	{ 0x005526u, car_attendant_run },
	{ 0x0056B4u, turntable_car_show },
	{ 0x005762u, turntable_scenery_show },
	{ 0x005862u, welcome_letter_start },
	{ 0x005883u, welcome_letter_show },
	{ 0x005908u, select_car_heading_show },
	{ 0x0059B8u, select_instruction_show },
	{ 0x005A1Du, transmission_choice_run },
	{ 0x005B45u, automatic_panel_show },
	{ 0x005BD6u, manual_panel_show },
	{ 0x005C71u, transmission_label_start_automatic },
	{ 0x005CA4u, transmission_label_start_manual },
	{ 0x005CE5u, transmission_label_follow },
	{ 0x005D14u, driver_couple_start },
	{ 0x005D5Cu, car_caption_show },
	{ 0x005E10u, car_gears_caption_show },
	{ 0x005EB4u, waiting_banner_show },
	{ 0x005F51u, car_select_entries_run },
	{ 0x006070u, choice_flag_start },
	{ 0x0060B1u, choice_flag_follow },
	{ 0x006105u, stacked_choice_flag_start },
	{ 0x00613Fu, stacked_choice_flag_follow },
	{ 0x006173u, choice_flag_number_start },
	{ 0x0061A2u, choice_flag_number_follow },
	{ 0x006356u, entry_countdown_start },
	{ 0x0063F9u, entry_countdown_count },
	{ 0x006463u, entry_countdown_done },
	{ 0x0064E2u, entry_banner_start },
	{ 0x006514u, entry_banner_blink },
	{ 0x00652Au, entry_car_ring_start },
	{ 0x006560u, entry_car_ring_draw },
	{ 0x00660Fu, entry_traffic_run },
	{ 0x00664Eu, entry_traffic_car_drive },
	{ 0x006EF0u, player_car_update },
	{ 0x007C36u, rival_warning_update },
	{ 0x007C9Eu, rival_alarm_update },
	{ 0x009590u, race_cars_start },
	{ 0x00974Cu, race_car_draw },
	{ 0x00AB24u, distance_offset_from_x },
	{ 0x00AB36u, distance_offset_from_z },
	{ 0x00BFC1u, rev_meter_update },
	{ 0x00C017u, speedometer_update },
	{ 0x00C0C6u, speed_unit_update },
	{ 0x00C10Du, gear_indicator_update },
	{ 0x00C231u, shift_up_hint_update },
	{ 0x00C2ACu, place_count_update },
	{ 0x00C342u, place_number_update },
	{ 0x00C43Eu, lap_timer_update },
	{ 0x00C553u, stage_indicator_start },
	{ 0x00C585u, stage_indicator_draw },
	{ 0x00C59Cu, start_message_update },
	{ 0x00C65Fu, time_left_display_update },
	{ 0x00C7ECu, extend_time_message_start },
	{ 0x00C857u, call_00C857 },
	{ 0x00C873u, stage_entry_update },
	{ 0x00C8B6u, goal_update },
	{ 0x00C9EDu, race_result_start },
	{ 0x00CA44u, race_result_hold },
	{ 0x00CA50u, race_result_verdict_start },
	{ 0x00CAA8u, race_result_verdict_blink },
	{ 0x00CABCu, branch_arrow_update },
	{ 0x00CBC0u, checkpoint_board_update },
	{ 0x00EAE4u, call_00EAE4 },
	{ 0x0126FCu, road_object_stream_read },
	{ 0x0127EEu, crest_clip_update },
	{ 0x01287Cu, scenery_update },
	{ 0x012906u, flashing_scenery_update },
	{ 0x0129ABu, scenery_pair_update },
	{ 0x0129F4u, overhead_scenery_update },
	{ 0x012A20u, aurora_update },
	{ 0x012ACFu, fork_sign_update },
	{ 0x012AF4u, animated_scenery_update },
	{ 0x012B84u, animated_scenery_pair_update },
	{ 0x012BAEu, road_crosser_update },
	{ 0x012CAAu, kangaroo_update },
	{ 0x012DB7u, moving_scenery_update },
	{ 0x012EA2u, swooping_helicopter_update },
	{ 0x012FBCu, start_gate_update },
	{ 0x01307Eu, start_flagman_update },
	{ 0x013104u, flagman_update },
	{ 0x0131A5u, start_balloon_update },
	{ 0x01322Du, ambient_sound_update },
	{ 0x013288u, one_shot_sound_update },
	{ 0x013296u, west_east_panel_update },
	{ 0x01333Cu, santa_sleigh_update },
	{ 0x01344Fu, goal_parade_rightward_update },
	{ 0x01351Eu, sounding_scenery_update },
	{ 0x0135CCu, goal_parade_leftward_update },
	{ 0x01369Au, goal_parade_dragon_update },
	{ 0x01376Cu, traffic_spawner_update },
	{ 0x01389Eu, traffic_car_start_convoy },
	{ 0x0138EBu, call_0138EB },
	{ 0x013AB7u, traffic_follower_update },
	{ 0x013AD8u, pulled_vehicle_update },
	{ 0x013AF9u, traffic_stopping_car_update },
	{ 0x013B56u, traffic_figure_start_team },
	{ 0x013BA3u, traffic_figure_update },
	{ 0x013D15u, traffic_hit_sound_wait },
	{ 0x013DC4u, fork_branch_start_right },
	{ 0x013DD5u, fork_branch_start_left },
	{ 0x013DE4u, fork_branch_draw },
	{ 0x014108u, race_car_occupant_update },
	{ 0x0141F4u, rider_gesture_update },
	{ 0x014470u, race_opening_update },
	{ 0x014514u, stage_end_sounds_update },
	{ 0x01458Du, fork_camera_update },
	{ 0x014694u, race_car_start_ending },
	{ 0x014735u, goal_actor_car_start },
	{ 0x014777u, goal_actor_driver_start },
	{ 0x014782u, goal_actor_passenger_start },
	{ 0x014838u, goal_actor_start },
	{ 0x014846u, goal_actor_run },
	{ 0x016B00u, road_surface_update },
	{ 0x0171CEu, road_tiles_change_update },
	{ 0x017524u, race_background_update },
	{ 0x0178C9u, race_background_lower_animation_wait },
	{ 0x0178D5u, race_background_lower_animation_cycle },
	{ 0x01794Cu, race_background_upper_animation_wait },
	{ 0x017958u, race_background_upper_animation_cycle },
	{ 0x017BFBu, palette_cycle_update },
	{ 0x017CAAu, sunset_fade_start },
	{ 0x017D1Fu, sunset_fade_wait },
	{ 0x017DB0u, backdrop_far_update },
	{ 0x017E1Bu, backdrop_near_start },
	{ 0x017E5Bu, backdrop_near_update },
	{ 0x017EF2u, backdrop_landmark_start },
	{ 0x017F3Du, backdrop_landmark_update },
	{ 0x018050u, backdrop_flashing_update },
	{ 0x0180CBu, backdrop_translucent_start },
	{ 0x018110u, backdrop_translucent_update },
	{ 0x01854Au, race_background_fade_watch_update },
	{ 0x0185D2u, palette_fade_prepare },
	{ 0x01864Au, call_01864A },
	{ 0x018BA0u, lcd_race_info_start },
	{ 0x018BB2u, lcd_race_info_update },
	{ 0x018E32u, lcd_race_info_finish },
	{ 0x018E98u, lcd_race_info_paused },
	{ 0x018F74u, car_radio_start },
	{ 0x018FD3u, car_radio_wait_start },
	{ 0x018FE0u, car_radio_read_buttons },
	{ 0x01900Au, car_radio_update },
	{ 0x019322u, song_title_show },
	{ 0x019CACu, title_screen_update },
	{ 0x019E6Bu, title_car_drive },
	{ 0x019F4Cu, title_car_driver_update },
	{ 0x019F60u, title_car_passenger_update },
	{ 0x01A0E2u, title_palm_tree_update },
	{ 0x01A18Cu, title_logo_update },
	{ 0x01A29Cu, title_logo_copy_update },
	{ 0x01A329u, title_logo_car_update },
	{ 0x01A3E9u, title_logo_stripe_update },
	{ 0x01A499u, title_board_palms_update },
	{ 0x01A536u, title_seagull_fly },
	{ 0x01A672u, title_picture_draw },
	{ 0x01A6CFu, coin_prompt_update },
	{ 0x01A7C5u, coin_prompt_car_drive },
	{ 0x01A834u, coin_prompt_message_update },
	{ 0x01A8E2u, ignition_key_prompt_update },
	{ 0x01AA01u, credit_display_update },
	{ 0x01ADE8u, demo_captions_run },
	{ 0x01AE8Du, demo_caption_update },
	{ 0x01C392u, test_mode_menu },
	{ 0x01C3E6u, test_mode_menu_run },
	{ 0x01C5BAu, test_mode_menu_toggle_continue },
	{ 0x01C5DBu, test_mode_memory_test },
	{ 0x01C928u, call_01C928 },
	{ 0x01CDE0u, test_mode_volume_adjustment },
	{ 0x01CE15u, call_01CE15 },
	{ 0x01CE97u, test_mode_volume_adjustment_adjust },
	{ 0x01D111u, test_mode_input_test },
	{ 0x01D126u, call_01D126 },
	{ 0x01D13Bu, test_mode_input_test_release },
	{ 0x01D3E4u, test_mode_output_test },
	{ 0x01D419u, test_mode_output_test_run },
	{ 0x01D620u, test_mode_sound_test },
	{ 0x01D661u, test_mode_sound_test_run },
	{ 0x01D840u, test_mode_crt_test },
	{ 0x01D868u, call_01D868 },
	{ 0x01D87Au, test_mode_crt_test_grid },
	{ 0x01D88Fu, call_01D88F },
	{ 0x01D9F3u, test_mode_game_assignments },
	{ 0x01DA28u, test_mode_game_assignments_run },
	{ 0x01DD1Eu, test_mode_coin_assignments },
	{ 0x01DD2Bu, test_mode_coin_assignments_show },
	{ 0x01DD58u, call_01DD58 },
	{ 0x01DEFDu, test_mode_coin_assignments_manual_run },
	{ 0x01E453u, test_mode_network_assignments_start },
	{ 0x01E4BAu, test_mode_network_assignments_run },
	{ 0x01E6EBu, test_mode_bookkeeping_start },
	{ 0x01E703u, call_01E703 },
	{ 0x01E715u, test_mode_bookkeeping_second_page_start },
	{ 0x01E72Du, call_01E72D },
	{ 0x01EDC4u, test_mode_backup_data_clear_start },
	{ 0x01EDF1u, test_mode_backup_data_clear_run },
	{ 0x01EE66u, test_mode_backup_data_clear_wait },
	{ 0x01EEC1u, test_mode_exit },
	{ 0x01EEEFu, test_mode_exit_caution_wait },
	{ 0x03F500u, int_vector_40 },
	{ 0x03F794u, int_vector_41 },
	{ 0x03F7A0u, int_vector_42 },
	{ 0x03F7ACu, int_vector_43 },
	{ 0x03F830u, int_vector_44 },
};

const size_t game_code_count = 244;

/* the code at the reset vector */
void game_entry(void)
{
	reset();
}
