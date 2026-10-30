/**
 *    Name:        table.c
 *    Description: the registers the whole program shares, the code calls through memory slots reach (by address), and the reset entry
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

uint32_t r12, r14, stack_pointer = 0x21F400u;
struct object *current_object;
struct car *player_car;
struct screen *screen;

/* the same, for the runtime to keep a copy for each of the game's tasks (1: a pointer into the game's
   state, where the original holds its address) */
const game_register game_context[] = { { &r12, 0 }, { &r14, 0 }, { &current_object, 1 }, { &player_car, 1 }, { &screen, 1 }, { &stack_pointer, 0 } };
const uint8_t game_context_registers[] = { 12, 14, 20, 23, 25, 32 };
const size_t game_context_count = 6;

/* How call_code() runs second_task from a slot that holds its address: with 0 for what it takes, and
   what it hands back dropped. */
static void call_0006B0(void)
{
	second_task(0u);
}

/* How call_code() runs draw_object from a slot that holds its address: with 0 for what it takes, and
   what it hands back dropped. */
static void call_00EAE4(void)
{
	draw_object(0);
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
	{ 0x0018A4u, staff_credits_code_watch },
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
	{ 0x00C857u, extend_time_message_blink },
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
	{ 0x0138EBu, traffic_car_update },
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
	{ 0x01864Au, palette_fade_step },
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

const void *boot_e80000_bytes_data;
const void *seat_credits_addresses_data;
const void *game_mode_jumps_data;
const void *analog_calibration_addresses_data;
const void *analog_input_counts_data;
const void *first_attract_car_lists_data;
const void *isdn_connection_step_jumps_data;
const void *isdn_status_state_jumps_data;
const void *isdn_connecting_texts_data;
const void *isdn_failed_text_data;
const void *isdn_success_text_data;
const void *isdn_this_side_text_data;
const void *isdn_other_side_text_data;
const void *isdn_total_text_data;
const void *title_step_jumps_data;
const void *bar_picture_bars_data;
const void *car_presentation_step_jumps_data;
const void *demo_race_step_jumps_data;
const void *attract_lamp_steps_data;
const void *car_presentation_state_jumps_data;
const void *unused_attract_car_name_sounds_data;
const void *unused_attract_car_animations_2_data;
const void *unused_attract_car_palettes_2_data;
const void *unused_attract_car_animations_1_data;
const void *unused_attract_car_palettes_1_data;
const void *unused_attract_car_caption_palettes_data;
const void *car_name_letter_state_jumps_data;
const void *attract_car_picture_animations_data;
const void *attract_car_scene_palettes_data;
const void *car_presentation_palette_cycle_data;
const void *staff_credits_code_sequence_data;
const void *attract_car_names_data;
const void *ranking_step_jumps_data;
const void *name_entry_state_jumps_data;
const void *name_entry_gas_step_jumps_data;
const void *name_entry_place_jumps_data;
const void *name_entry_letter_state_jumps_data;
const void *name_entry_lcd_state_jumps_data;
const void *name_entry_car_state_jumps_data;
const void *name_entry_drivers_state_jumps_data;
const void *name_entry_course_name_state_jumps_data;
const void *ranking_state_jumps_data;
const void *ranking_background_state_jumps_data;
const void *ranking_title_left_state_jumps_data;
const void *ranking_title_right_state_jumps_data;
const void *ranking_course_banner_state_jumps_data;
const void *ranking_record_character_state_jumps_data;
const void *ranking_record_car_state_jumps_data;
const void *passing_traffic_state_jumps_data;
const void *passing_car_state_jumps_data;
const void *ranking_background_tile_banks_data;
const void *ranking_background_register_sets_data;
const void *ranking_mixer_addresses_data;
const void *ranking_background_page_numbers_data;
const void *ranking_background_scroll_xs_data;
const void *default_course_records_data;
const void *name_entry_letters_data;
const void *course_backgrounds_data;
const void *ranking_record_character_xs_data;
const void *ranking_record_character_ys_data;
const void *name_entry_turn_rates_data;
const void *name_entry_rejected_names_data;
const void *name_entry_place_sounds_data;
const void *certificate_car_animations_data;
const void *car_model_palettes_data;
const void *certificate_driver_animations_data;
const void *certificate_driver_palettes_data;
const void *car_model_turn_animations_data;
const void *ranking_record_character_palettes_data;
const void *ranking_course_banner_animations_data;
const void *ranking_course_banner_palettes_data;
const void *certificate_course_animations_data;
const void *certificate_course_palettes_data;
const void *game_step_jumps_data;
const void *game_start_c00074_bytes_data;
const void *game_end_c00074_bytes_data;
const void *game_end_c00074_next_bytes_data;
const void *attract_demo_races_data;
const void *route_map_state_jumps_data;
const void *route_map_car_state_jumps_data;
const void *route_map_sign_state_jumps_data;
const void *route_map_sign_animations_data;
const void *route_map_sign_palettes_data;
const void *route_map_goal_xs_data;
const void *route_map_landmark_state_jumps_data;
const void *route_map_leg_state_jumps_data;
const void *route_map_leg_jumps_data;
const void *route_map_leg_car_state_jumps_data;
const void *route_map_leg_car_leg_jumps_data;
const void *route_map_checkpoint_state_jumps_data;
const void *route_map_finish_x_offsets_data;
const void *route_map_finish_y_offsets_data;
const void *route_map_direction_sign_state_jumps_data;
const void *route_map_wait_banner_state_jumps_data;
const void *game_over_banner_state_jumps_data;
const void *route_map_register_sets_data;
const void *route_map_close_up_pages_data;
const void *route_map_routes_pages_data;
const void *route_map_world_pages_data;
const void *route_map_start_scroll_xs_data;
const void *route_map_whole_scroll_xs_data;
const void *route_map_sea_colour_addresses_data;
const void *route_map_car_animations_data;
const void *route_map_car_palettes_data;
const void *route_map_stage_xs_data;
const void *route_map_stage_ys_data;
const void *route_map_direction_x_shifts_data;
const void *route_map_first_fork_leg_animations_data;
const void *route_map_fork_leg_animations_data;
const void *route_map_start_leg_car_places_data;
const void *route_map_first_fork_leg_car_places_data;
const void *route_map_fork_leg_car_places_data;
const void *stage_result_labels_data;
const void *stage_result_places_data;
const void *stage_result_retire_data;
const void *route_map_landmarks_data;
const void *car_select_state_jumps_data;
const void *car_select_choice_turn_jumps_data;
const void *car_select_rowscroll_copies_jumps_data;
const void *car_attendant_state_jumps_data;
const void *turntable_car_state_jumps_data;
const void *turntable_scenery_state_jumps_data;
const void *welcome_letter_state_jumps_data;
const void *select_car_heading_state_jumps_data;
const void *select_instruction_state_jumps_data;
const void *transmission_choice_state_jumps_data;
const void *automatic_panel_state_jumps_data;
const void *manual_panel_state_jumps_data;
const void *car_caption_state_jumps_data;
const void *car_gears_caption_state_jumps_data;
const void *waiting_banner_state_jumps_data;
const void *car_select_entries_state_jumps_data;
const void *car_select_gas_down_jumps_data;
const void *entry_countdown_lcd_texts_data;
const void *entry_traffic_state_jumps_data;
const void *entry_traffic_car_state_jumps_data;
const void *car_pass_sounds_data;
const void *traffic_path_data;
const void *car_select_tilemap_setups_data;
const void *car_select_default_cars_data;
const void *turntable_car_looks_data;
const void *turntable_car_frames_data;
const void *turntable_scenery_groups_data;
const void *car_select_turn_delays_data;
const void *car_select_wheel_transmissions_data;
const void *welcome_letter_places_data;
const void *car_caption_palettes_data;
const void *car_select_car_sounds_data;
const void *select_instruction_pieces_data;
const void *automatic_panel_palette_lists_data;
const void *manual_panel_palette_lists_data;
const void *driver_couple_looks_data;
const void *car_gears_caption_frames_data;
const void *choice_flag_palettes_data;
const void *choice_flag_number_animations_data;
const void *car_select_entries_flag_blink_data;
const void *unused_numbers_006EC8_data;
const void *player_car_state_jumps_data;
const void *player_car_time_up_step_jumps_data;
const void *player_car_goal_step_jumps_data;
const void *player_car_rival_step_jumps_data;
const void *rival_warning_state_jumps_data;
const void *rival_alarm_state_jumps_data;
const void *player_car_hit_kind_jumps_data;
const void *crash_place_offsets_data;
const void *car_weights_data;
const void *car_crash_physics_data;
const void *grid_road_place_lists_data;
const void *grid_segment_lists_data;
const void *player_car_steer_animations_data;
const void *steer_frames_by_steering_data;
const void *player_car_wheelspin_animations_data;
const void *player_car_skid_animations_wheel_high_data;
const void *player_car_skid_animations_wheel_low_data;
const void *player_car_palettes_data;
const void *player_car_arrival_animations_data;
const void *player_car_time_up_brakes_data;
const void *goal_stops_data;
const void *race_car_first_body_pieces_data;
const void *race_car_first_body_animations_data;
const void *race_car_driver_animations_data;
const void *race_car_passenger_animations_data;
const void *race_car_driver_gesture_masks_data;
const void *race_car_passenger_gesture_masks_data;
const void *race_car_driver_palettes_data;
const void *race_car_passenger_palettes_data;
const void *race_car_driver_frame_counts_data;
const void *race_car_passenger_frame_counts_data;
const void *race_car_body_wheel_kinds_data;
const void *race_car_driver_handlers_data;
const void *race_car_passenger_handlers_data;
const void *unused_car_animations_data;
const void *race_car_spray_palettes_data;
const void *race_car_spray_2_animations_data;
const void *race_car_spray_1_animations_data;
const void *race_car_spray_frame_counts_data;
const void *race_car_tag_palettes_data;
const void *race_car_tag_animations_data;
const void *race_car_blink_starts_data;
const void *race_car_tag_blink_data;
const void *race_car_ending_handlers_data;
const void *unused_numbers_009FE4_data;
const void *player_car_heading_quadrant_jumps_data;
const void *player_car_view_yaw_quadrant_jumps_data;
const void *car_skid_noises_data;
const void *gas_pedal_curve_data;
const void *brake_pedal_curve_data;
const void *car_handlings_data;
const void *car_gear_lists_data;
const void *car_torque_curves_data;
const void *car_model_off_road_noises_data;
const void *car_model_off_road_shakes_data;
const void *wheel_effect_1_jumps_data;
const void *wheel_effect_2_jumps_data;
const void *stage_roadside_surface_lists_data;
const void *rev_meter_state_jumps_data;
const void *speedometer_state_jumps_data;
const void *speed_unit_state_jumps_data;
const void *gear_indicator_state_jumps_data;
const void *shift_up_hint_state_jumps_data;
const void *place_count_state_jumps_data;
const void *place_number_state_jumps_data;
const void *lap_timer_state_jumps_data;
const void *start_message_state_jumps_data;
const void *time_left_display_state_jumps_data;
const void *stage_entry_state_jumps_data;
const void *goal_state_jumps_data;
const void *branch_arrow_state_jumps_data;
const void *checkpoint_board_state_jumps_data;
const void *checkpoint_board_row_state_jumps_data;
const void *checkpoint_board_car_places_data;
const void *checkpoint_board_number_places_data;
const void *checkpoint_board_behind_places_data;
const void *checkpoint_board_car_sprites_data;
const void *checkpoint_board_number_animations_data;
const void *checkpoint_board_time_places_data;
const void *gear_indicator_animations_data;
const void *stage_level_texts_data;
const void *lap_timer_checkpoint_states_data;
const void *entry_invitation_step_jumps_data;
const void *race_copy_offsets_data;
const void *race_copy_player_offsets_data;
const void *link_board_ic_texts_data;
const void *stage_start_times_data;
const void *fork_next_stages_data;
const void *entry_frames_by_machines_data;
const void *draw_object_mode_jumps_data;
const void *sprite_flip_x_signs_data;
const void *sprite_flip_y_signs_data;
const void *animation_order_jumps_data;
const void *smoothed_flip_x_signs_data;
const void *smoothed_flip_signs_data;
const void *sprite_list_switches_data;
const void *course_command_jumps_data;
const void *unused_segment_steps_data;
const void *unused_segment_shifts_data;
const void *unused_numbers_01060C_data;
const void *hidden_layer_pairs_data;
const void *sprite_control_mode_data;
const void *sprite_control_flip_data;
const void *video_control_default_data;
const void *screen_hardware_records_data;
const void *screen_view_defaults_data;
const void *model_view_pitch_bases_data;
const void *screen_fade_kind_jumps_data;
const void *screen_fade_starts_data;
const void *music_fade_addresses_data;
const void *car_model_engine_samples_data;
const void *demo_sounds_data;
const void *demo_second_sounds_data;
const void *octant_base_angles_data;
const void *octant_angle_factors_data;
const void *road_object_stream_state_jumps_data;
const void *road_object_stream_command_jumps_data;
const void *crest_clip_state_jumps_data;
const void *scenery_state_jumps_data;
const void *flashing_scenery_state_jumps_data;
const void *scenery_pair_state_jumps_data;
const void *unused_0129D0_state_jumps_data;
const void *overhead_scenery_state_jumps_data;
const void *aurora_state_jumps_data;
const void *fork_sign_state_jumps_data;
const void *animated_scenery_state_jumps_data;
const void *animated_scenery_pair_state_jumps_data;
const void *road_crosser_state_jumps_data;
const void *kangaroo_state_jumps_data;
const void *moving_scenery_state_jumps_data;
const void *swooping_helicopter_state_jumps_data;
const void *start_gate_state_jumps_data;
const void *start_gate_light_palettes_data;
const void *start_flagman_state_jumps_data;
const void *flagman_state_jumps_data;
const void *start_balloon_state_jumps_data;
const void *ambient_sound_state_jumps_data;
const void *west_east_panel_state_jumps_data;
const void *santa_sleigh_state_jumps_data;
const void *goal_parade_rightward_state_jumps_data;
const void *sounding_scenery_state_jumps_data;
const void *goal_parade_leftward_state_jumps_data;
const void *goal_parade_dragon_state_jumps_data;
const void *traffic_spawner_state_jumps_data;
const void *traffic_car_state_jumps_data;
const void *traffic_stopping_car_state_jumps_data;
const void *traffic_figure_state_jumps_data;
const void *traffic_shake_frame_offsets_data;
const void *course_length_command_jumps_data;
const void *fork_branch_state_jumps_data;
const void *fork_branch_stage_palettes_data;
const void *race_car_occupant_state_jumps_data;
const void *rider_gesture_state_jumps_data;
const void *rider_gesture_x_offsets_data;
const void *rider_gesture_y_offsets_data;
const void *rider_gesture_car_states_data;
const void *rider_gesture_views_data;
const void *race_car_occupant_animations_data;
const void *race_car_rider_palettes_data;
const void *rider_gesture_animations_data;
const void *race_opening_state_jumps_data;
const void *race_opening_sound_step_jumps_data;
const void *race_opening_camera_step_jumps_data;
const void *stage_end_sounds_state_jumps_data;
const void *fork_camera_state_jumps_data;
const void *race_opening_sound_cues_data;
const void *stage_end_sound_cues_data;
const void *fork_camera_lift_steps_data;
const void *fork_camera_tilt_steps_data;
const void *race_car_ending_state_jumps_data;
const void *goal_actor_command_jumps_data;
const void *goal_actor_place_waits_data;
const void *ending_actor_lists_data;
const void *unfinished_ending_actor_count_data;
const void *unfinished_ending_actors_data;
const void *road_surface_state_jumps_data;
const void *road_surface_event_jumps_data;
const void *road_surface_layer_setups_data;
const void *road_surface_page_words_data;
const void *road_tiles_change_state_jumps_data;
const void *road_tiles_change_first_data_data;
const void *road_tiles_change_second_data_data;
const void *race_background_state_jumps_data;
const void *race_background_change_step_jumps_data;
const void *background_lower_animation_tile_pages_data;
const void *background_lower_animation_page_pairs_data;
const void *background_upper_animation_tile_pages_data;
const void *background_upper_animation_page_pairs_data;
const void *race_background_layer_setups_data;
const void *background_register_lists_data;
const void *background_page_words_data;
const void *stage_backgrounds_data;
const void *background_tile_banks_data;
const void *stage_tile_banks_data;
const void *palette_cycle_state_jumps_data;
const void *palette_cycle_running_jumps_data;
const void *sunset_fade_state_jumps_data;
const void *backdrop_far_state_jumps_data;
const void *backdrop_near_state_jumps_data;
const void *backdrop_landmark_state_jumps_data;
const void *backdrop_flashing_state_jumps_data;
const void *backdrop_translucent_state_jumps_data;
const void *stage_background_object_lists_data;
const void *lcd_scroll_step_jumps_data;
const void *dj_face_characters_data;
const void *lcd_restart_nibbles_data;
const void *lcd_custom_characters_data;
const void *lcd_boot_text_data;
const void *lcd_race_info_state_jumps_data;
const void *lcd_race_info_page_jumps_data;
const void *lcd_race_info_time_step_jumps_data;
const void *lcd_race_info_order_step_jumps_data;
const void *lcd_race_info_speed_step_jumps_data;
const void *lcd_race_info_maximum_step_jumps_data;
const void *lcd_race_info_average_step_jumps_data;
const void *lcd_race_info_finish_state_jumps_data;
const void *lcd_countdown_bars_data;
const void *lcd_time_label_data;
const void *lcd_order_label_data;
const void *lcd_speed_label_data;
const void *lcd_maximum_label_data;
const void *lcd_average_label_data;
const void *lcd_finish_label_data;
const void *car_radio_state_jumps_data;
const void *car_radio_talk_step_jumps_data;
const void *song_title_state_jumps_data;
const void *car_model_songs_data;
const void *radio_songs_data;
const void *route_stage_radio_sounds_data;
const void *radio_station_title_data;
const void *song_title_colours_data;
const void *stage_level_radio_messages_data;
const void *dj_messages_data;
const void *dj_remark_lists_data;
const void *title_screen_state_jumps_data;
const void *title_car_state_jumps_data;
const void *title_car_driver_state_jumps_data;
const void *title_car_passenger_state_jumps_data;
const void *title_driver_animations_data;
const void *title_passenger_animations_data;
const void *title_driver_palettes_data;
const void *title_passenger_palettes_data;
const void *title_rider_scales_data;
const void *title_driver_x_offsets_data;
const void *title_driver_y_offsets_data;
const void *title_passenger_x_offsets_data;
const void *title_passenger_y_offsets_data;
const void *title_palm_tree_state_jumps_data;
const void *title_logo_state_jumps_data;
const void *title_logo_copy_state_jumps_data;
const void *title_logo_car_state_jumps_data;
const void *title_logo_stripe_state_jumps_data;
const void *title_board_palms_state_jumps_data;
const void *title_seagull_state_jumps_data;
const void *title_picture_state_jumps_data;
const void *coin_prompt_state_jumps_data;
const void *coin_prompt_screen_jumps_data;
const void *coin_prompt_car_state_jumps_data;
const void *coin_prompt_message_state_jumps_data;
const void *ignition_key_prompt_state_jumps_data;
const void *ignition_key_prompt_pieces_data;
const void *coin_prompt_car_animations_data;
const void *coin_prompt_car_palettes_data;
const void *coin_prompt_message_animations_data;
const void *coin_prompt_message_palettes_data;
const void *coin_prompt_lcd_texts_data;
const void *credit_display_state_jumps_data;
const void *credit_display_screen_jumps_data;
const void *credit_digit_characters_data;
const void *credit_display_animations_data;
const void *credit_display_palettes_data;
const void *title_car_screen_offsets_data;
const void *title_car_records_data;
const void *title_car_animations_data;
const void *title_logo_car_palettes_data;
const void *title_pictures_data;
const void *title_seagull_places_data;
const void *title_logo_stripe_places_data;
const void *title_logo_car_places_data;
const void *title_logo_car_wave_frames_data;
const void *title_screen_layer_setups_data;
const void *title_background_setups_data;
const void *demo_captions_state_jumps_data;
const void *demo_caption_state_jumps_data;
const void *demo_caption_pictures_data;
const void *kanji_text_viewer_state_jumps_data;
const void *unused_kanji_text_viewer_words_data;
const void *kanji_texts_data;
const void *test_mode_lcd_text_data;
const void *test_mode_lcd_text_4_to_7_data;
const void *test_mode_lcd_text_8_to_11_data;
const void *test_mode_lcd_text_12_to_15_data;
const void *test_mode_page_jumps_data;
const void *test_mode_menu_headings_data;
const void *test_mode_menu_cursor_cells_data;
const void *memory_test_result_texts_data;
const void *volume_adjustment_cursor_cells_data;
const void *volume_adjustment_cancel_cell_data;
const void *input_test_on_off_cells_data;
const void *output_test_cursor_cells_data;
const void *output_test_exit_cell_data;
const void *output_test_outputs_data;
const void *test_mode_output_test_item_jumps_data;
const void *output_test_on_off_cells_data;
const void *test_mode_sound_test_item_jumps_data;
const void *sound_test_cursor_cells_data;
const void *sound_test_exit_cell_data;
const void *sound_test_codes_data;
const void *test_mode_game_assignments_item_jumps_data;
const void *game_assignments_cursor_cells_data;
const void *game_assignments_exit_cell_data;
const void *steering_wheel_reaction_texts_data;
const void *advertise_sound_texts_data;
const void *test_mode_coin_assignments_item_jumps_data;
const void *test_mode_coin_assignments_manual_item_jumps_data;
const void *coin_assignments_cursor_cells_data;
const void *coin_manual_cursor_cells_data;
const void *coin_manual_exit_cell_data;
const void *coin_setting_descriptions_data;
const void *coin_setting_records_data;
const void *test_mode_network_assignments_item_jumps_data;
const void *network_assignments_cursor_cells_data;
const void *network_assignments_exit_cell_data;
const void *communication_texts_data;
const void *privilege_mode_texts_data;
const void *privilege_mode_settings_data;
const void *backup_data_clear_cursor_cells_data;
const void *backup_data_clear_no_cell_data;
const void *test_mode_instructions_text_data;
const void *test_mode_palettes_1_bytes_data;
const void *test_mode_palettes_2_bytes_data;
const void *road_object_streams_data;
const void *stage_road_object_lists_data;
const void *background_palettes_data;
const void *background_leaving_fades_data;
const void *background_palette_fades_data;
const void *stage_road_palettes_data;
const void *background_road_palettes_data;
const void *scene_palettes_data;
const void *car_select_palette_data;
const void *ranking_palette_data;
const void *route_map_palette_data;
const void *title_palette_data;
const void *route_map_background_palette_data;
const void *sprite_palettes_data;
const void *sprite_animations_data;
const void *fork_branch_piece_animation_data;
const void *stage_courses_data;
const void *stage_road_page_sets_data;
const void *vblank_timer_period_data;
const void *lcd_timer_period_data;
const void *link_message_kind_jumps_data;
const void *vblank_seat_inputs_jumps_data;
const void *screen_vblank_counters_data;
const void *ic37_checksum_data;
const void *ic40_checksum_data;
const void *ic36_checksum_data;
const void *ic39_checksum_data;
const void *blank_sprite_places_data;
const void *sine_table_data;
const void *cosine_table_data;
const void *arctangents_data;
const void *boot_tile_pages_5d_data;
const void *boot_tile_pages_5e_data;
const void *boot_tile_pages_29_to_4e_data;
const void *no_background_tile_pages_data;
const void *background_tile_page_lists_data;
const void *text_sprite_words_0_1_data;
const void *text_sprite_size_data;
const void *text_sprite_font_data;
const void *text_sprite_palette_data;
const void *sprite_size_steps_data;
const void *entry_countdown_palette_fades_data;
const void *name_entry_palette_fades_data;
const void *manual_picked_palettes_data;
const void *automatic_picked_palettes_data;
const void *no_background_palettes_data;
const void *road_row_size_step_lists_data;
const void *road_row_size_shift_lists_data;
const void *race_car_rider_places_data;
const void *perspective_scales_data;
const void *depth_layers_by_depth_data;
const void *kanji_row_pixels_data;
const void *kanji_font_data;

/* the tables the game loads from its assets: their names, addresses and pointers */
const game_table game_tables[] =
{
	{ "boot_e80000_bytes", 0x0000B4u, &boot_e80000_bytes_data },
	{ "seat_credits_addresses", 0x0008C4u, &seat_credits_addresses_data },
	{ "game_mode_jumps", 0x0008CCu, &game_mode_jumps_data },
	{ "analog_calibration_addresses", 0x000B0Cu, &analog_calibration_addresses_data },
	{ "analog_input_counts", 0x000B14u, &analog_input_counts_data },
	{ "first_attract_car_lists", 0x000D14u, &first_attract_car_lists_data },
	{ "isdn_connection_step_jumps", 0x000DACu, &isdn_connection_step_jumps_data },
	{ "isdn_status_state_jumps", 0x000EB0u, &isdn_status_state_jumps_data },
	{ "isdn_connecting_texts", 0x000F78u, &isdn_connecting_texts_data },
	{ "isdn_failed_text", 0x000F80u, &isdn_failed_text_data },
	{ "isdn_success_text", 0x000F84u, &isdn_success_text_data },
	{ "isdn_this_side_text", 0x000F88u, &isdn_this_side_text_data },
	{ "isdn_other_side_text", 0x000F8Cu, &isdn_other_side_text_data },
	{ "isdn_total_text", 0x000F90u, &isdn_total_text_data },
	{ "title_step_jumps", 0x001014u, &title_step_jumps_data },
	{ "bar_picture_bars", 0x0011B1u, &bar_picture_bars_data },
	{ "car_presentation_step_jumps", 0x00146Cu, &car_presentation_step_jumps_data },
	{ "demo_race_step_jumps", 0x001530u, &demo_race_step_jumps_data },
	{ "attract_lamp_steps", 0x001607u, &attract_lamp_steps_data },
	{ "car_presentation_state_jumps", 0x001694u, &car_presentation_state_jumps_data },
	{ "unused_attract_car_name_sounds", 0x00183Bu, &unused_attract_car_name_sounds_data },
	{ "unused_attract_car_animations_2", 0x001844u, &unused_attract_car_animations_2_data },
	{ "unused_attract_car_palettes_2", 0x001854u, &unused_attract_car_palettes_2_data },
	{ "unused_attract_car_animations_1", 0x001864u, &unused_attract_car_animations_1_data },
	{ "unused_attract_car_palettes_1", 0x001874u, &unused_attract_car_palettes_1_data },
	{ "unused_attract_car_caption_palettes", 0x001884u, &unused_attract_car_caption_palettes_data },
	{ "car_name_letter_state_jumps", 0x0019CCu, &car_name_letter_state_jumps_data },
	{ "attract_car_picture_animations", 0x001A88u, &attract_car_picture_animations_data },
	{ "attract_car_scene_palettes", 0x001A98u, &attract_car_scene_palettes_data },
	{ "car_presentation_palette_cycle", 0x001AC8u, &car_presentation_palette_cycle_data },
	{ "staff_credits_code_sequence", 0x001ADCu, &staff_credits_code_sequence_data },
	{ "attract_car_names", 0x001CECu, &attract_car_names_data },
	{ "ranking_step_jumps", 0x001DE0u, &ranking_step_jumps_data },
	{ "name_entry_state_jumps", 0x001ED0u, &name_entry_state_jumps_data },
	{ "name_entry_gas_step_jumps", 0x002088u, &name_entry_gas_step_jumps_data },
	{ "name_entry_place_jumps", 0x0020C8u, &name_entry_place_jumps_data },
	{ "name_entry_letter_state_jumps", 0x00221Cu, &name_entry_letter_state_jumps_data },
	{ "name_entry_lcd_state_jumps", 0x002304u, &name_entry_lcd_state_jumps_data },
	{ "name_entry_car_state_jumps", 0x002384u, &name_entry_car_state_jumps_data },
	{ "name_entry_drivers_state_jumps", 0x0023D4u, &name_entry_drivers_state_jumps_data },
	{ "name_entry_course_name_state_jumps", 0x002424u, &name_entry_course_name_state_jumps_data },
	{ "ranking_state_jumps", 0x002488u, &ranking_state_jumps_data },
	{ "ranking_background_state_jumps", 0x002564u, &ranking_background_state_jumps_data },
	{ "ranking_title_left_state_jumps", 0x002620u, &ranking_title_left_state_jumps_data },
	{ "ranking_title_right_state_jumps", 0x00266Cu, &ranking_title_right_state_jumps_data },
	{ "ranking_course_banner_state_jumps", 0x0026B8u, &ranking_course_banner_state_jumps_data },
	{ "ranking_record_character_state_jumps", 0x00279Cu, &ranking_record_character_state_jumps_data },
	{ "ranking_record_car_state_jumps", 0x00287Cu, &ranking_record_car_state_jumps_data },
	{ "passing_traffic_state_jumps", 0x002960u, &passing_traffic_state_jumps_data },
	{ "passing_car_state_jumps", 0x0029A4u, &passing_car_state_jumps_data },
	{ "ranking_background_tile_banks", 0x002C18u, &ranking_background_tile_banks_data },
	{ "ranking_background_register_sets", 0x002C44u, &ranking_background_register_sets_data },
	{ "ranking_mixer_addresses", 0x002CA4u, &ranking_mixer_addresses_data },
	{ "ranking_background_page_numbers", 0x002CACu, &ranking_background_page_numbers_data },
	{ "ranking_background_scroll_xs", 0x002CB4u, &ranking_background_scroll_xs_data },
	{ "default_course_records", 0x002CC4u, &default_course_records_data },
	{ "name_entry_letters", 0x002D00u, &name_entry_letters_data },
	{ "course_backgrounds", 0x002D20u, &course_backgrounds_data },
	{ "ranking_record_character_xs", 0x002D2Au, &ranking_record_character_xs_data },
	{ "ranking_record_character_ys", 0x002D40u, &ranking_record_character_ys_data },
	{ "name_entry_turn_rates", 0x002D6Cu, &name_entry_turn_rates_data },
	{ "name_entry_rejected_names", 0x002D7Cu, &name_entry_rejected_names_data },
	{ "name_entry_place_sounds", 0x002D98u, &name_entry_place_sounds_data },
	{ "certificate_car_animations", 0x002D9Cu, &certificate_car_animations_data },
	{ "car_model_palettes", 0x002DACu, &car_model_palettes_data },
	{ "certificate_driver_animations", 0x002DBCu, &certificate_driver_animations_data },
	{ "certificate_driver_palettes", 0x002DCCu, &certificate_driver_palettes_data },
	{ "car_model_turn_animations", 0x002DDCu, &car_model_turn_animations_data },
	{ "ranking_record_character_palettes", 0x002DECu, &ranking_record_character_palettes_data },
	{ "ranking_course_banner_animations", 0x002DFCu, &ranking_course_banner_animations_data },
	{ "ranking_course_banner_palettes", 0x002E10u, &ranking_course_banner_palettes_data },
	{ "certificate_course_animations", 0x002E24u, &certificate_course_animations_data },
	{ "certificate_course_palettes", 0x002E38u, &certificate_course_palettes_data },
	{ "game_step_jumps", 0x002E80u, &game_step_jumps_data },
	{ "game_start_c00074_bytes", 0x003463u, &game_start_c00074_bytes_data },
	{ "game_end_c00074_bytes", 0x003465u, &game_end_c00074_bytes_data },
	{ "game_end_c00074_next_bytes", 0x003467u, &game_end_c00074_next_bytes_data },
	{ "attract_demo_races", 0x0034F4u, &attract_demo_races_data },
	{ "route_map_state_jumps", 0x003540u, &route_map_state_jumps_data },
	{ "route_map_car_state_jumps", 0x003E28u, &route_map_car_state_jumps_data },
	{ "route_map_sign_state_jumps", 0x003F34u, &route_map_sign_state_jumps_data },
	{ "route_map_sign_animations", 0x00403Au, &route_map_sign_animations_data },
	{ "route_map_sign_palettes", 0x00403Eu, &route_map_sign_palettes_data },
	{ "route_map_goal_xs", 0x004042u, &route_map_goal_xs_data },
	{ "route_map_landmark_state_jumps", 0x004064u, &route_map_landmark_state_jumps_data },
	{ "route_map_leg_state_jumps", 0x00424Cu, &route_map_leg_state_jumps_data },
	{ "route_map_leg_jumps", 0x004280u, &route_map_leg_jumps_data },
	{ "route_map_leg_car_state_jumps", 0x00442Cu, &route_map_leg_car_state_jumps_data },
	{ "route_map_leg_car_leg_jumps", 0x004474u, &route_map_leg_car_leg_jumps_data },
	{ "route_map_checkpoint_state_jumps", 0x004588u, &route_map_checkpoint_state_jumps_data },
	{ "route_map_finish_x_offsets", 0x00462Cu, &route_map_finish_x_offsets_data },
	{ "route_map_finish_y_offsets", 0x004634u, &route_map_finish_y_offsets_data },
	{ "route_map_direction_sign_state_jumps", 0x004648u, &route_map_direction_sign_state_jumps_data },
	{ "route_map_wait_banner_state_jumps", 0x0046C8u, &route_map_wait_banner_state_jumps_data },
	{ "game_over_banner_state_jumps", 0x00477Cu, &game_over_banner_state_jumps_data },
	{ "route_map_register_sets", 0x0047E8u, &route_map_register_sets_data },
	{ "route_map_close_up_pages", 0x0048B8u, &route_map_close_up_pages_data },
	{ "route_map_routes_pages", 0x0048C0u, &route_map_routes_pages_data },
	{ "route_map_world_pages", 0x0048C8u, &route_map_world_pages_data },
	{ "route_map_start_scroll_xs", 0x0048D0u, &route_map_start_scroll_xs_data },
	{ "route_map_whole_scroll_xs", 0x0048D4u, &route_map_whole_scroll_xs_data },
	{ "route_map_sea_colour_addresses", 0x0048D8u, &route_map_sea_colour_addresses_data },
	{ "route_map_car_animations", 0x0048E0u, &route_map_car_animations_data },
	{ "route_map_car_palettes", 0x0048F0u, &route_map_car_palettes_data },
	{ "route_map_stage_xs", 0x004900u, &route_map_stage_xs_data },
	{ "route_map_stage_ys", 0x00492Au, &route_map_stage_ys_data },
	{ "route_map_direction_x_shifts", 0x004954u, &route_map_direction_x_shifts_data },
	{ "route_map_first_fork_leg_animations", 0x004958u, &route_map_first_fork_leg_animations_data },
	{ "route_map_fork_leg_animations", 0x00495Cu, &route_map_fork_leg_animations_data },
	{ "route_map_start_leg_car_places", 0x004964u, &route_map_start_leg_car_places_data },
	{ "route_map_first_fork_leg_car_places", 0x0049A4u, &route_map_first_fork_leg_car_places_data },
	{ "route_map_fork_leg_car_places", 0x004A24u, &route_map_fork_leg_car_places_data },
	{ "stage_result_labels", 0x004BA4u, &stage_result_labels_data },
	{ "stage_result_places", 0x004BDAu, &stage_result_places_data },
	{ "stage_result_retire", 0x004C0Au, &stage_result_retire_data },
	{ "route_map_landmarks", 0x004C14u, &route_map_landmarks_data },
	{ "car_select_state_jumps", 0x005058u, &car_select_state_jumps_data },
	{ "car_select_choice_turn_jumps", 0x0053ECu, &car_select_choice_turn_jumps_data },
	{ "car_select_rowscroll_copies_jumps", 0x00548Cu, &car_select_rowscroll_copies_jumps_data },
	{ "car_attendant_state_jumps", 0x005534u, &car_attendant_state_jumps_data },
	{ "turntable_car_state_jumps", 0x0056C0u, &turntable_car_state_jumps_data },
	{ "turntable_scenery_state_jumps", 0x005770u, &turntable_scenery_state_jumps_data },
	{ "welcome_letter_state_jumps", 0x005890u, &welcome_letter_state_jumps_data },
	{ "select_car_heading_state_jumps", 0x005914u, &select_car_heading_state_jumps_data },
	{ "select_instruction_state_jumps", 0x0059C4u, &select_instruction_state_jumps_data },
	{ "transmission_choice_state_jumps", 0x005A28u, &transmission_choice_state_jumps_data },
	{ "automatic_panel_state_jumps", 0x005B50u, &automatic_panel_state_jumps_data },
	{ "manual_panel_state_jumps", 0x005BE4u, &manual_panel_state_jumps_data },
	{ "car_caption_state_jumps", 0x005D68u, &car_caption_state_jumps_data },
	{ "car_gears_caption_state_jumps", 0x005E1Cu, &car_gears_caption_state_jumps_data },
	{ "waiting_banner_state_jumps", 0x005EC0u, &waiting_banner_state_jumps_data },
	{ "car_select_entries_state_jumps", 0x005F5Cu, &car_select_entries_state_jumps_data },
	{ "car_select_gas_down_jumps", 0x0062B0u, &car_select_gas_down_jumps_data },
	{ "entry_countdown_lcd_texts", 0x006464u, &entry_countdown_lcd_texts_data },
	{ "entry_traffic_state_jumps", 0x00661Cu, &entry_traffic_state_jumps_data },
	{ "entry_traffic_car_state_jumps", 0x00665Cu, &entry_traffic_car_state_jumps_data },
	{ "car_pass_sounds", 0x0067A9u, &car_pass_sounds_data },
	{ "traffic_path", 0x0067B2u, &traffic_path_data },
	{ "car_select_tilemap_setups", 0x006920u, &car_select_tilemap_setups_data },
	{ "car_select_default_cars", 0x0069B0u, &car_select_default_cars_data },
	{ "turntable_car_looks", 0x0069B8u, &turntable_car_looks_data },
	{ "turntable_car_frames", 0x0069D8u, &turntable_car_frames_data },
	{ "turntable_scenery_groups", 0x006A18u, &turntable_scenery_groups_data },
	{ "car_select_turn_delays", 0x006D9Cu, &car_select_turn_delays_data },
	{ "car_select_wheel_transmissions", 0x006DA4u, &car_select_wheel_transmissions_data },
	{ "welcome_letter_places", 0x006DACu, &welcome_letter_places_data },
	{ "car_caption_palettes", 0x006DDEu, &car_caption_palettes_data },
	{ "car_select_car_sounds", 0x006DEEu, &car_select_car_sounds_data },
	{ "select_instruction_pieces", 0x006DF6u, &select_instruction_pieces_data },
	{ "automatic_panel_palette_lists", 0x006E30u, &automatic_panel_palette_lists_data },
	{ "manual_panel_palette_lists", 0x006E44u, &manual_panel_palette_lists_data },
	{ "driver_couple_looks", 0x006E58u, &driver_couple_looks_data },
	{ "car_gears_caption_frames", 0x006E78u, &car_gears_caption_frames_data },
	{ "choice_flag_palettes", 0x006E80u, &choice_flag_palettes_data },
	{ "choice_flag_number_animations", 0x006E90u, &choice_flag_number_animations_data },
	{ "car_select_entries_flag_blink", 0x006EA0u, &car_select_entries_flag_blink_data },
	{ "unused_numbers_006EC8", 0x006EC8u, &unused_numbers_006EC8_data },
	{ "player_car_state_jumps", 0x006F18u, &player_car_state_jumps_data },
	{ "player_car_time_up_step_jumps", 0x007164u, &player_car_time_up_step_jumps_data },
	{ "player_car_goal_step_jumps", 0x00720Cu, &player_car_goal_step_jumps_data },
	{ "player_car_rival_step_jumps", 0x007917u, &player_car_rival_step_jumps_data },
	{ "rival_warning_state_jumps", 0x007C44u, &rival_warning_state_jumps_data },
	{ "rival_alarm_state_jumps", 0x007CACu, &rival_alarm_state_jumps_data },
	{ "player_car_hit_kind_jumps", 0x007D28u, &player_car_hit_kind_jumps_data },
	{ "crash_place_offsets", 0x008026u, &crash_place_offsets_data },
	{ "car_weights", 0x008038u, &car_weights_data },
	{ "car_crash_physics", 0x00804Au, &car_crash_physics_data },
	{ "grid_road_place_lists", 0x0090ACu, &grid_road_place_lists_data },
	{ "grid_segment_lists", 0x0091CCu, &grid_segment_lists_data },
	{ "player_car_steer_animations", 0x00926Cu, &player_car_steer_animations_data },
	{ "steer_frames_by_steering", 0x00927Cu, &steer_frames_by_steering_data },
	{ "player_car_wheelspin_animations", 0x009300u, &player_car_wheelspin_animations_data },
	{ "player_car_skid_animations_wheel_high", 0x009310u, &player_car_skid_animations_wheel_high_data },
	{ "player_car_skid_animations_wheel_low", 0x009410u, &player_car_skid_animations_wheel_low_data },
	{ "player_car_palettes", 0x009510u, &player_car_palettes_data },
	{ "player_car_arrival_animations", 0x009520u, &player_car_arrival_animations_data },
	{ "player_car_time_up_brakes", 0x009530u, &player_car_time_up_brakes_data },
	{ "goal_stops", 0x009540u, &goal_stops_data },
	{ "race_car_first_body_pieces", 0x009DCCu, &race_car_first_body_pieces_data },
	{ "race_car_first_body_animations", 0x009DDCu, &race_car_first_body_animations_data },
	{ "race_car_driver_animations", 0x009DECu, &race_car_driver_animations_data },
	{ "race_car_passenger_animations", 0x009DFCu, &race_car_passenger_animations_data },
	{ "race_car_driver_gesture_masks", 0x009E0Cu, &race_car_driver_gesture_masks_data },
	{ "race_car_passenger_gesture_masks", 0x009E14u, &race_car_passenger_gesture_masks_data },
	{ "race_car_driver_palettes", 0x009E1Cu, &race_car_driver_palettes_data },
	{ "race_car_passenger_palettes", 0x009E2Cu, &race_car_passenger_palettes_data },
	{ "race_car_driver_frame_counts", 0x009E3Cu, &race_car_driver_frame_counts_data },
	{ "race_car_passenger_frame_counts", 0x009E44u, &race_car_passenger_frame_counts_data },
	{ "race_car_body_wheel_kinds", 0x009E4Cu, &race_car_body_wheel_kinds_data },
	{ "race_car_driver_handlers", 0x009EBCu, &race_car_driver_handlers_data },
	{ "race_car_passenger_handlers", 0x009EDCu, &race_car_passenger_handlers_data },
	{ "unused_car_animations", 0x009EFCu, &unused_car_animations_data },
	{ "race_car_spray_palettes", 0x009F0Cu, &race_car_spray_palettes_data },
	{ "race_car_spray_2_animations", 0x009F2Cu, &race_car_spray_2_animations_data },
	{ "race_car_spray_1_animations", 0x009F4Cu, &race_car_spray_1_animations_data },
	{ "race_car_spray_frame_counts", 0x009F6Cu, &race_car_spray_frame_counts_data },
	{ "race_car_tag_palettes", 0x009F7Cu, &race_car_tag_palettes_data },
	{ "race_car_tag_animations", 0x009F8Cu, &race_car_tag_animations_data },
	{ "race_car_blink_starts", 0x009F9Cu, &race_car_blink_starts_data },
	{ "race_car_tag_blink", 0x009FA4u, &race_car_tag_blink_data },
	{ "race_car_ending_handlers", 0x009FC4u, &race_car_ending_handlers_data },
	{ "unused_numbers_009FE4", 0x009FE4u, &unused_numbers_009FE4_data },
	{ "player_car_heading_quadrant_jumps", 0x00A5F4u, &player_car_heading_quadrant_jumps_data },
	{ "player_car_view_yaw_quadrant_jumps", 0x00A798u, &player_car_view_yaw_quadrant_jumps_data },
	{ "car_skid_noises", 0x00ABC3u, &car_skid_noises_data },
	{ "gas_pedal_curve", 0x00ABCBu, &gas_pedal_curve_data },
	{ "brake_pedal_curve", 0x00ACCBu, &brake_pedal_curve_data },
	{ "car_handlings", 0x00ADCCu, &car_handlings_data },
	{ "car_gear_lists", 0x00B04Cu, &car_gear_lists_data },
	{ "car_torque_curves", 0x00B0C0u, &car_torque_curves_data },
	{ "car_model_off_road_noises", 0x00B6FCu, &car_model_off_road_noises_data },
	{ "car_model_off_road_shakes", 0x00B704u, &car_model_off_road_shakes_data },
	{ "wheel_effect_1_jumps", 0x00B70Cu, &wheel_effect_1_jumps_data },
	{ "wheel_effect_2_jumps", 0x00B74Cu, &wheel_effect_2_jumps_data },
	{ "stage_roadside_surface_lists", 0x00B78Cu, &stage_roadside_surface_lists_data },
	{ "rev_meter_state_jumps", 0x00BFCCu, &rev_meter_state_jumps_data },
	{ "speedometer_state_jumps", 0x00C024u, &speedometer_state_jumps_data },
	{ "speed_unit_state_jumps", 0x00C0D4u, &speed_unit_state_jumps_data },
	{ "gear_indicator_state_jumps", 0x00C118u, &gear_indicator_state_jumps_data },
	{ "shift_up_hint_state_jumps", 0x00C23Cu, &shift_up_hint_state_jumps_data },
	{ "place_count_state_jumps", 0x00C2B8u, &place_count_state_jumps_data },
	{ "place_number_state_jumps", 0x00C350u, &place_number_state_jumps_data },
	{ "lap_timer_state_jumps", 0x00C44Cu, &lap_timer_state_jumps_data },
	{ "start_message_state_jumps", 0x00C5A8u, &start_message_state_jumps_data },
	{ "time_left_display_state_jumps", 0x00C66Cu, &time_left_display_state_jumps_data },
	{ "stage_entry_state_jumps", 0x00C880u, &stage_entry_state_jumps_data },
	{ "goal_state_jumps", 0x00C8C4u, &goal_state_jumps_data },
	{ "branch_arrow_state_jumps", 0x00CAC8u, &branch_arrow_state_jumps_data },
	{ "checkpoint_board_state_jumps", 0x00CBCCu, &checkpoint_board_state_jumps_data },
	{ "checkpoint_board_row_state_jumps", 0x00CC70u, &checkpoint_board_row_state_jumps_data },
	{ "checkpoint_board_car_places", 0x00CF68u, &checkpoint_board_car_places_data },
	{ "checkpoint_board_number_places", 0x00CF88u, &checkpoint_board_number_places_data },
	{ "checkpoint_board_behind_places", 0x00CFA8u, &checkpoint_board_behind_places_data },
	{ "checkpoint_board_car_sprites", 0x00CFC8u, &checkpoint_board_car_sprites_data },
	{ "checkpoint_board_number_animations", 0x00CFE8u, &checkpoint_board_number_animations_data },
	{ "checkpoint_board_time_places", 0x00CFF8u, &checkpoint_board_time_places_data },
	{ "gear_indicator_animations", 0x00D018u, &gear_indicator_animations_data },
	{ "stage_level_texts", 0x00D034u, &stage_level_texts_data },
	{ "lap_timer_checkpoint_states", 0x00D0C0u, &lap_timer_checkpoint_states_data },
	{ "entry_invitation_step_jumps", 0x00DDB4u, &entry_invitation_step_jumps_data },
	{ "race_copy_offsets", 0x00E0A0u, &race_copy_offsets_data },
	{ "race_copy_player_offsets", 0x00E0C4u, &race_copy_player_offsets_data },
	{ "link_board_ic_texts", 0x00E4D0u, &link_board_ic_texts_data },
	{ "stage_start_times", 0x00E5B3u, &stage_start_times_data },
	{ "fork_next_stages", 0x00E5D3u, &fork_next_stages_data },
	{ "entry_frames_by_machines", 0x00E614u, &entry_frames_by_machines_data },
	{ "draw_object_mode_jumps", 0x00EB44u, &draw_object_mode_jumps_data },
	{ "sprite_flip_x_signs", 0x00F13Cu, &sprite_flip_x_signs_data },
	{ "sprite_flip_y_signs", 0x00F140u, &sprite_flip_y_signs_data },
	{ "animation_order_jumps", 0x00F34Cu, &animation_order_jumps_data },
	{ "smoothed_flip_x_signs", 0x00F4E8u, &smoothed_flip_x_signs_data },
	{ "smoothed_flip_signs", 0x00F4ECu, &smoothed_flip_signs_data },
	{ "sprite_list_switches", 0x00F508u, &sprite_list_switches_data },
	{ "course_command_jumps", 0x00FF40u, &course_command_jumps_data },
	{ "unused_segment_steps", 0x010309u, &unused_segment_steps_data },
	{ "unused_segment_shifts", 0x010425u, &unused_segment_shifts_data },
	{ "unused_numbers_01060C", 0x01060Cu, &unused_numbers_01060C_data },
	{ "hidden_layer_pairs", 0x010B06u, &hidden_layer_pairs_data },
	{ "sprite_control_mode", 0x010B0Au, &sprite_control_mode_data },
	{ "sprite_control_flip", 0x010B0Bu, &sprite_control_flip_data },
	{ "video_control_default", 0x010B10u, &video_control_default_data },
	{ "screen_hardware_records", 0x010BA0u, &screen_hardware_records_data },
	{ "screen_view_defaults", 0x010BC8u, &screen_view_defaults_data },
	{ "model_view_pitch_bases", 0x010BD2u, &model_view_pitch_bases_data },
	{ "screen_fade_kind_jumps", 0x01168Cu, &screen_fade_kind_jumps_data },
	{ "screen_fade_starts", 0x01173Au, &screen_fade_starts_data },
	{ "music_fade_addresses", 0x011FFCu, &music_fade_addresses_data },
	{ "car_model_engine_samples", 0x012026u, &car_model_engine_samples_data },
	{ "demo_sounds", 0x01202Eu, &demo_sounds_data },
	{ "demo_second_sounds", 0x01204Eu, &demo_second_sounds_data },
	{ "octant_base_angles", 0x0121D8u, &octant_base_angles_data },
	{ "octant_angle_factors", 0x0121E8u, &octant_angle_factors_data },
	{ "road_object_stream_state_jumps", 0x012708u, &road_object_stream_state_jumps_data },
	{ "road_object_stream_command_jumps", 0x012748u, &road_object_stream_command_jumps_data },
	{ "crest_clip_state_jumps", 0x0127FCu, &crest_clip_state_jumps_data },
	{ "scenery_state_jumps", 0x012888u, &scenery_state_jumps_data },
	{ "flashing_scenery_state_jumps", 0x012914u, &flashing_scenery_state_jumps_data },
	{ "scenery_pair_state_jumps", 0x0129B8u, &scenery_pair_state_jumps_data },
	{ "unused_0129D0_state_jumps", 0x0129DCu, &unused_0129D0_state_jumps_data },
	{ "overhead_scenery_state_jumps", 0x012A00u, &overhead_scenery_state_jumps_data },
	{ "aurora_state_jumps", 0x012A2Cu, &aurora_state_jumps_data },
	{ "fork_sign_state_jumps", 0x012ADCu, &fork_sign_state_jumps_data },
	{ "animated_scenery_state_jumps", 0x012B00u, &animated_scenery_state_jumps_data },
	{ "animated_scenery_pair_state_jumps", 0x012B90u, &animated_scenery_pair_state_jumps_data },
	{ "road_crosser_state_jumps", 0x012BBCu, &road_crosser_state_jumps_data },
	{ "kangaroo_state_jumps", 0x012CB8u, &kangaroo_state_jumps_data },
	{ "moving_scenery_state_jumps", 0x012DC4u, &moving_scenery_state_jumps_data },
	{ "swooping_helicopter_state_jumps", 0x012EB0u, &swooping_helicopter_state_jumps_data },
	{ "start_gate_state_jumps", 0x012FC8u, &start_gate_state_jumps_data },
	{ "start_gate_light_palettes", 0x01306Eu, &start_gate_light_palettes_data },
	{ "start_flagman_state_jumps", 0x01308Cu, &start_flagman_state_jumps_data },
	{ "flagman_state_jumps", 0x013110u, &flagman_state_jumps_data },
	{ "start_balloon_state_jumps", 0x0131B0u, &start_balloon_state_jumps_data },
	{ "ambient_sound_state_jumps", 0x013238u, &ambient_sound_state_jumps_data },
	{ "west_east_panel_state_jumps", 0x0132A4u, &west_east_panel_state_jumps_data },
	{ "santa_sleigh_state_jumps", 0x013348u, &santa_sleigh_state_jumps_data },
	{ "goal_parade_rightward_state_jumps", 0x01345Cu, &goal_parade_rightward_state_jumps_data },
	{ "sounding_scenery_state_jumps", 0x01352Cu, &sounding_scenery_state_jumps_data },
	{ "goal_parade_leftward_state_jumps", 0x0135D8u, &goal_parade_leftward_state_jumps_data },
	{ "goal_parade_dragon_state_jumps", 0x0136A8u, &goal_parade_dragon_state_jumps_data },
	{ "traffic_spawner_state_jumps", 0x013778u, &traffic_spawner_state_jumps_data },
	{ "traffic_car_state_jumps", 0x0138F8u, &traffic_car_state_jumps_data },
	{ "traffic_stopping_car_state_jumps", 0x013B04u, &traffic_stopping_car_state_jumps_data },
	{ "traffic_figure_state_jumps", 0x013BB0u, &traffic_figure_state_jumps_data },
	{ "traffic_shake_frame_offsets", 0x013D45u, &traffic_shake_frame_offsets_data },
	{ "course_length_command_jumps", 0x013D94u, &course_length_command_jumps_data },
	{ "fork_branch_state_jumps", 0x013DF0u, &fork_branch_state_jumps_data },
	{ "fork_branch_stage_palettes", 0x0140C8u, &fork_branch_stage_palettes_data },
	{ "race_car_occupant_state_jumps", 0x014114u, &race_car_occupant_state_jumps_data },
	{ "rider_gesture_state_jumps", 0x014200u, &rider_gesture_state_jumps_data },
	{ "rider_gesture_x_offsets", 0x014360u, &rider_gesture_x_offsets_data },
	{ "rider_gesture_y_offsets", 0x014380u, &rider_gesture_y_offsets_data },
	{ "rider_gesture_car_states", 0x0143A0u, &rider_gesture_car_states_data },
	{ "rider_gesture_views", 0x0143A6u, &rider_gesture_views_data },
	{ "race_car_occupant_animations", 0x01440Eu, &race_car_occupant_animations_data },
	{ "race_car_rider_palettes", 0x01442Eu, &race_car_rider_palettes_data },
	{ "rider_gesture_animations", 0x01444Eu, &rider_gesture_animations_data },
	{ "race_opening_state_jumps", 0x01447Cu, &race_opening_state_jumps_data },
	{ "race_opening_sound_step_jumps", 0x0144A9u, &race_opening_sound_step_jumps_data },
	{ "race_opening_camera_step_jumps", 0x0144ECu, &race_opening_camera_step_jumps_data },
	{ "stage_end_sounds_state_jumps", 0x014520u, &stage_end_sounds_state_jumps_data },
	{ "fork_camera_state_jumps", 0x0145A0u, &fork_camera_state_jumps_data },
	{ "race_opening_sound_cues", 0x014660u, &race_opening_sound_cues_data },
	{ "stage_end_sound_cues", 0x01466Fu, &stage_end_sound_cues_data },
	{ "fork_camera_lift_steps", 0x014674u, &fork_camera_lift_steps_data },
	{ "fork_camera_tilt_steps", 0x014684u, &fork_camera_tilt_steps_data },
	{ "race_car_ending_state_jumps", 0x0146A0u, &race_car_ending_state_jumps_data },
	{ "goal_actor_command_jumps", 0x014994u, &goal_actor_command_jumps_data },
	{ "goal_actor_place_waits", 0x014A16u, &goal_actor_place_waits_data },
	{ "ending_actor_lists", 0x014A20u, &ending_actor_lists_data },
	{ "unfinished_ending_actor_count", 0x014A60u, &unfinished_ending_actor_count_data },
	{ "unfinished_ending_actors", 0x014A64u, &unfinished_ending_actors_data },
	{ "road_surface_state_jumps", 0x016B0Cu, &road_surface_state_jumps_data },
	{ "road_surface_event_jumps", 0x016BBCu, &road_surface_event_jumps_data },
	{ "road_surface_layer_setups", 0x01710Cu, &road_surface_layer_setups_data },
	{ "road_surface_page_words", 0x01714Cu, &road_surface_page_words_data },
	{ "road_tiles_change_state_jumps", 0x0171E0u, &road_tiles_change_state_jumps_data },
	{ "road_tiles_change_first_data", 0x017444u, &road_tiles_change_first_data_data },
	{ "road_tiles_change_second_data", 0x0174B4u, &road_tiles_change_second_data_data },
	{ "race_background_state_jumps", 0x017530u, &race_background_state_jumps_data },
	{ "race_background_change_step_jumps", 0x017738u, &race_background_change_step_jumps_data },
	{ "background_lower_animation_tile_pages", 0x017904u, &background_lower_animation_tile_pages_data },
	{ "background_lower_animation_page_pairs", 0x01790Cu, &background_lower_animation_page_pairs_data },
	{ "background_upper_animation_tile_pages", 0x017988u, &background_upper_animation_tile_pages_data },
	{ "background_upper_animation_page_pairs", 0x0179A0u, &background_upper_animation_page_pairs_data },
	{ "race_background_layer_setups", 0x017A50u, &race_background_layer_setups_data },
	{ "background_register_lists", 0x017A78u, &background_register_lists_data },
	{ "background_page_words", 0x017AE0u, &background_page_words_data },
	{ "stage_backgrounds", 0x017B60u, &stage_backgrounds_data },
	{ "background_tile_banks", 0x017B7Fu, &background_tile_banks_data },
	{ "stage_tile_banks", 0x017BA9u, &stage_tile_banks_data },
	{ "palette_cycle_state_jumps", 0x017C08u, &palette_cycle_state_jumps_data },
	{ "palette_cycle_running_jumps", 0x017C68u, &palette_cycle_running_jumps_data },
	{ "sunset_fade_state_jumps", 0x017CB8u, &sunset_fade_state_jumps_data },
	{ "backdrop_far_state_jumps", 0x017DE4u, &backdrop_far_state_jumps_data },
	{ "backdrop_near_state_jumps", 0x017E98u, &backdrop_near_state_jumps_data },
	{ "backdrop_landmark_state_jumps", 0x017F7Cu, &backdrop_landmark_state_jumps_data },
	{ "backdrop_flashing_state_jumps", 0x018094u, &backdrop_flashing_state_jumps_data },
	{ "backdrop_translucent_state_jumps", 0x01814Cu, &backdrop_translucent_state_jumps_data },
	{ "stage_background_object_lists", 0x0181B8u, &stage_background_object_lists_data },
	{ "lcd_scroll_step_jumps", 0x018A70u, &lcd_scroll_step_jumps_data },
	{ "dj_face_characters", 0x018AE8u, &dj_face_characters_data },
	{ "lcd_restart_nibbles", 0x018B21u, &lcd_restart_nibbles_data },
	{ "lcd_custom_characters", 0x018B4Eu, &lcd_custom_characters_data },
	{ "lcd_boot_text", 0x018B90u, &lcd_boot_text_data },
	{ "lcd_race_info_state_jumps", 0x018BD4u, &lcd_race_info_state_jumps_data },
	{ "lcd_race_info_page_jumps", 0x018C64u, &lcd_race_info_page_jumps_data },
	{ "lcd_race_info_time_step_jumps", 0x018C84u, &lcd_race_info_time_step_jumps_data },
	{ "lcd_race_info_order_step_jumps", 0x018CF0u, &lcd_race_info_order_step_jumps_data },
	{ "lcd_race_info_speed_step_jumps", 0x018D5Cu, &lcd_race_info_speed_step_jumps_data },
	{ "lcd_race_info_maximum_step_jumps", 0x018D88u, &lcd_race_info_maximum_step_jumps_data },
	{ "lcd_race_info_average_step_jumps", 0x018DB4u, &lcd_race_info_average_step_jumps_data },
	{ "lcd_race_info_finish_state_jumps", 0x018E40u, &lcd_race_info_finish_state_jumps_data },
	{ "lcd_countdown_bars", 0x018EA0u, &lcd_countdown_bars_data },
	{ "lcd_time_label", 0x018F1Eu, &lcd_time_label_data },
	{ "lcd_order_label", 0x018F2Cu, &lcd_order_label_data },
	{ "lcd_speed_label", 0x018F3Au, &lcd_speed_label_data },
	{ "lcd_maximum_label", 0x018F48u, &lcd_maximum_label_data },
	{ "lcd_average_label", 0x018F56u, &lcd_average_label_data },
	{ "lcd_finish_label", 0x018F64u, &lcd_finish_label_data },
	{ "car_radio_state_jumps", 0x019044u, &car_radio_state_jumps_data },
	{ "car_radio_talk_step_jumps", 0x0191E0u, &car_radio_talk_step_jumps_data },
	{ "song_title_state_jumps", 0x019330u, &song_title_state_jumps_data },
	{ "car_model_songs", 0x019421u, &car_model_songs_data },
	{ "radio_songs", 0x01942Cu, &radio_songs_data },
	{ "route_stage_radio_sounds", 0x019458u, &route_stage_radio_sounds_data },
	{ "radio_station_title", 0x01954Cu, &radio_station_title_data },
	{ "song_title_colours", 0x019561u, &song_title_colours_data },
	{ "stage_level_radio_messages", 0x019571u, &stage_level_radio_messages_data },
	{ "dj_messages", 0x019576u, &dj_messages_data },
	{ "dj_remark_lists", 0x01966Cu, &dj_remark_lists_data },
	{ "title_screen_state_jumps", 0x019CB8u, &title_screen_state_jumps_data },
	{ "title_car_state_jumps", 0x019E78u, &title_car_state_jumps_data },
	{ "title_car_driver_state_jumps", 0x019F58u, &title_car_driver_state_jumps_data },
	{ "title_car_passenger_state_jumps", 0x019F6Cu, &title_car_passenger_state_jumps_data },
	{ "title_driver_animations", 0x01A012u, &title_driver_animations_data },
	{ "title_passenger_animations", 0x01A022u, &title_passenger_animations_data },
	{ "title_driver_palettes", 0x01A032u, &title_driver_palettes_data },
	{ "title_passenger_palettes", 0x01A042u, &title_passenger_palettes_data },
	{ "title_rider_scales", 0x01A052u, &title_rider_scales_data },
	{ "title_driver_x_offsets", 0x01A062u, &title_driver_x_offsets_data },
	{ "title_driver_y_offsets", 0x01A072u, &title_driver_y_offsets_data },
	{ "title_passenger_x_offsets", 0x01A082u, &title_passenger_x_offsets_data },
	{ "title_passenger_y_offsets", 0x01A092u, &title_passenger_y_offsets_data },
	{ "title_palm_tree_state_jumps", 0x01A0F0u, &title_palm_tree_state_jumps_data },
	{ "title_logo_state_jumps", 0x01A198u, &title_logo_state_jumps_data },
	{ "title_logo_copy_state_jumps", 0x01A2A8u, &title_logo_copy_state_jumps_data },
	{ "title_logo_car_state_jumps", 0x01A334u, &title_logo_car_state_jumps_data },
	{ "title_logo_stripe_state_jumps", 0x01A3F4u, &title_logo_stripe_state_jumps_data },
	{ "title_board_palms_state_jumps", 0x01A4A4u, &title_board_palms_state_jumps_data },
	{ "title_seagull_state_jumps", 0x01A544u, &title_seagull_state_jumps_data },
	{ "title_picture_state_jumps", 0x01A680u, &title_picture_state_jumps_data },
	{ "coin_prompt_state_jumps", 0x01A6DCu, &coin_prompt_state_jumps_data },
	{ "coin_prompt_screen_jumps", 0x01A744u, &coin_prompt_screen_jumps_data },
	{ "coin_prompt_car_state_jumps", 0x01A7D0u, &coin_prompt_car_state_jumps_data },
	{ "coin_prompt_message_state_jumps", 0x01A84Cu, &coin_prompt_message_state_jumps_data },
	{ "ignition_key_prompt_state_jumps", 0x01A8F0u, &ignition_key_prompt_state_jumps_data },
	{ "ignition_key_prompt_pieces", 0x01A95Bu, &ignition_key_prompt_pieces_data },
	{ "coin_prompt_car_animations", 0x01A986u, &coin_prompt_car_animations_data },
	{ "coin_prompt_car_palettes", 0x01A996u, &coin_prompt_car_palettes_data },
	{ "coin_prompt_message_animations", 0x01A9A6u, &coin_prompt_message_animations_data },
	{ "coin_prompt_message_palettes", 0x01A9ACu, &coin_prompt_message_palettes_data },
	{ "coin_prompt_lcd_texts", 0x01A9BCu, &coin_prompt_lcd_texts_data },
	{ "credit_display_state_jumps", 0x01AA0Cu, &credit_display_state_jumps_data },
	{ "credit_display_screen_jumps", 0x01AA4Cu, &credit_display_screen_jumps_data },
	{ "credit_digit_characters", 0x01AB8Eu, &credit_digit_characters_data },
	{ "credit_display_animations", 0x01AB98u, &credit_display_animations_data },
	{ "credit_display_palettes", 0x01AB9Eu, &credit_display_palettes_data },
	{ "title_car_screen_offsets", 0x01ABA4u, &title_car_screen_offsets_data },
	{ "title_car_records", 0x01ABB4u, &title_car_records_data },
	{ "title_car_animations", 0x01AC34u, &title_car_animations_data },
	{ "title_logo_car_palettes", 0x01AC44u, &title_logo_car_palettes_data },
	{ "title_pictures", 0x01AC70u, &title_pictures_data },
	{ "title_seagull_places", 0x01ACA0u, &title_seagull_places_data },
	{ "title_logo_stripe_places", 0x01ACA8u, &title_logo_stripe_places_data },
	{ "title_logo_car_places", 0x01ACB4u, &title_logo_car_places_data },
	{ "title_logo_car_wave_frames", 0x01ACC0u, &title_logo_car_wave_frames_data },
	{ "title_screen_layer_setups", 0x01ACCCu, &title_screen_layer_setups_data },
	{ "title_background_setups", 0x01AD0Cu, &title_background_setups_data },
	{ "demo_captions_state_jumps", 0x01ADF4u, &demo_captions_state_jumps_data },
	{ "demo_caption_state_jumps", 0x01AE98u, &demo_caption_state_jumps_data },
	{ "demo_caption_pictures", 0x01AF00u, &demo_caption_pictures_data },
	{ "kanji_text_viewer_state_jumps", 0x01AFE8u, &kanji_text_viewer_state_jumps_data },
	{ "unused_kanji_text_viewer_words", 0x01B248u, &unused_kanji_text_viewer_words_data },
	{ "kanji_texts", 0x01B254u, &kanji_texts_data },
	{ "test_mode_lcd_text", 0x01C378u, &test_mode_lcd_text_data },
	{ "test_mode_lcd_text_4_to_7", 0x01C37Cu, &test_mode_lcd_text_4_to_7_data },
	{ "test_mode_lcd_text_8_to_11", 0x01C380u, &test_mode_lcd_text_8_to_11_data },
	{ "test_mode_lcd_text_12_to_15", 0x01C384u, &test_mode_lcd_text_12_to_15_data },
	{ "test_mode_page_jumps", 0x01C45Cu, &test_mode_page_jumps_data },
	{ "test_mode_menu_headings", 0x01C560u, &test_mode_menu_headings_data },
	{ "test_mode_menu_cursor_cells", 0x01C588u, &test_mode_menu_cursor_cells_data },
	{ "memory_test_result_texts", 0x01CDC8u, &memory_test_result_texts_data },
	{ "volume_adjustment_cursor_cells", 0x01CF9Cu, &volume_adjustment_cursor_cells_data },
	{ "volume_adjustment_cancel_cell", 0x01CF9Eu, &volume_adjustment_cancel_cell_data },
	{ "input_test_on_off_cells", 0x01D3C0u, &input_test_on_off_cells_data },
	{ "output_test_cursor_cells", 0x01D5AAu, &output_test_cursor_cells_data },
	{ "output_test_exit_cell", 0x01D5BEu, &output_test_exit_cell_data },
	{ "output_test_outputs", 0x01D5C0u, &output_test_outputs_data },
	{ "test_mode_output_test_item_jumps", 0x01D5ECu, &test_mode_output_test_item_jumps_data },
	{ "output_test_on_off_cells", 0x01D618u, &output_test_on_off_cells_data },
	{ "test_mode_sound_test_item_jumps", 0x01D77Cu, &test_mode_sound_test_item_jumps_data },
	{ "sound_test_cursor_cells", 0x01D7C0u, &sound_test_cursor_cells_data },
	{ "sound_test_exit_cell", 0x01D7C4u, &sound_test_exit_cell_data },
	{ "sound_test_codes", 0x01D7C6u, &sound_test_codes_data },
	{ "test_mode_game_assignments_item_jumps", 0x01DBD0u, &test_mode_game_assignments_item_jumps_data },
	{ "game_assignments_cursor_cells", 0x01DCCEu, &game_assignments_cursor_cells_data },
	{ "game_assignments_exit_cell", 0x01DCE0u, &game_assignments_exit_cell_data },
	{ "steering_wheel_reaction_texts", 0x01DCE4u, &steering_wheel_reaction_texts_data },
	{ "advertise_sound_texts", 0x01DCECu, &advertise_sound_texts_data },
	{ "test_mode_coin_assignments_item_jumps", 0x01E0D8u, &test_mode_coin_assignments_item_jumps_data },
	{ "test_mode_coin_assignments_manual_item_jumps", 0x01E0E4u, &test_mode_coin_assignments_manual_item_jumps_data },
	{ "coin_assignments_cursor_cells", 0x01E200u, &coin_assignments_cursor_cells_data },
	{ "coin_manual_cursor_cells", 0x01E206u, &coin_manual_cursor_cells_data },
	{ "coin_manual_exit_cell", 0x01E20Cu, &coin_manual_exit_cell_data },
	{ "coin_setting_descriptions", 0x01E220u, &coin_setting_descriptions_data },
	{ "coin_setting_records", 0x01E3E8u, &coin_setting_records_data },
	{ "test_mode_network_assignments_item_jumps", 0x01E61Cu, &test_mode_network_assignments_item_jumps_data },
	{ "network_assignments_cursor_cells", 0x01E67Cu, &network_assignments_cursor_cells_data },
	{ "network_assignments_exit_cell", 0x01E682u, &network_assignments_exit_cell_data },
	{ "communication_texts", 0x01E684u, &communication_texts_data },
	{ "privilege_mode_texts", 0x01E68Cu, &privilege_mode_texts_data },
	{ "privilege_mode_settings", 0x01E6E5u, &privilege_mode_settings_data },
	{ "backup_data_clear_cursor_cells", 0x01EEAEu, &backup_data_clear_cursor_cells_data },
	{ "backup_data_clear_no_cell", 0x01EEB0u, &backup_data_clear_no_cell_data },
	{ "test_mode_instructions_text", 0x01F282u, &test_mode_instructions_text_data },
	{ "test_mode_palettes_1_bytes", 0x01F302u, &test_mode_palettes_1_bytes_data },
	{ "test_mode_palettes_2_bytes", 0x01F504u, &test_mode_palettes_2_bytes_data },
	{ "road_object_streams", 0x020800u, &road_object_streams_data },
	{ "stage_road_object_lists", 0x02E1F0u, &stage_road_object_lists_data },
	{ "background_palettes", 0x0318CCu, &background_palettes_data },
	{ "background_leaving_fades", 0x0319F0u, &background_leaving_fades_data },
	{ "background_palette_fades", 0x031B14u, &background_palette_fades_data },
	{ "stage_road_palettes", 0x031C38u, &stage_road_palettes_data },
	{ "background_road_palettes", 0x031CB8u, &background_road_palettes_data },
	{ "scene_palettes", 0x031DDCu, &scene_palettes_data },
	{ "car_select_palette", 0x031E7Cu, &car_select_palette_data },
	{ "ranking_palette", 0x031E80u, &ranking_palette_data },
	{ "route_map_palette", 0x031E84u, &route_map_palette_data },
	{ "title_palette", 0x031E88u, &title_palette_data },
	{ "route_map_background_palette", 0x031E8Cu, &route_map_background_palette_data },
	{ "sprite_palettes", 0x032BA4u, &sprite_palettes_data },
	{ "sprite_animations", 0x034138u, &sprite_animations_data },
	{ "fork_branch_piece_animation", 0x034308u, &fork_branch_piece_animation_data },
	{ "stage_courses", 0x03CC00u, &stage_courses_data },
	{ "stage_road_page_sets", 0x03CC80u, &stage_road_page_sets_data },
	{ "vblank_timer_period", 0x03F82Au, &vblank_timer_period_data },
	{ "lcd_timer_period", 0x03F82Cu, &lcd_timer_period_data },
	{ "link_message_kind_jumps", 0x03F85Cu, &link_message_kind_jumps_data },
	{ "vblank_seat_inputs_jumps", 0x03FAC4u, &vblank_seat_inputs_jumps_data },
	{ "screen_vblank_counters", 0x03FACCu, &screen_vblank_counters_data },
	{ "ic37_checksum", 0x03FFF8u, &ic37_checksum_data },
	{ "ic40_checksum", 0x03FFFAu, &ic40_checksum_data },
	{ "ic36_checksum", 0x03FFFCu, &ic36_checksum_data },
	{ "ic39_checksum", 0x03FFFEu, &ic39_checksum_data },
	{ "blank_sprite_places", 0x100000u, &blank_sprite_places_data },
	{ "sine_table", 0x100C00u, &sine_table_data },
	{ "cosine_table", 0x101400u, &cosine_table_data },
	{ "arctangents", 0x103400u, &arctangents_data },
	{ "boot_tile_pages_5d", 0x109420u, &boot_tile_pages_5d_data },
	{ "boot_tile_pages_5e", 0x109428u, &boot_tile_pages_5e_data },
	{ "boot_tile_pages_29_to_4e", 0x109578u, &boot_tile_pages_29_to_4e_data },
	{ "no_background_tile_pages", 0x109684u, &no_background_tile_pages_data },
	{ "background_tile_page_lists", 0x109694u, &background_tile_page_lists_data },
	{ "text_sprite_words_0_1", 0x122B5Cu, &text_sprite_words_0_1_data },
	{ "text_sprite_size", 0x122B60u, &text_sprite_size_data },
	{ "text_sprite_font", 0x122B64u, &text_sprite_font_data },
	{ "text_sprite_palette", 0x122B66u, &text_sprite_palette_data },
	{ "sprite_size_steps", 0x122B74u, &sprite_size_steps_data },
	{ "entry_countdown_palette_fades", 0x13F66Cu, &entry_countdown_palette_fades_data },
	{ "name_entry_palette_fades", 0x13F6C0u, &name_entry_palette_fades_data },
	{ "manual_picked_palettes", 0x13F95Cu, &manual_picked_palettes_data },
	{ "automatic_picked_palettes", 0x13F998u, &automatic_picked_palettes_data },
	{ "no_background_palettes", 0x13F9D4u, &no_background_palettes_data },
	{ "road_row_size_step_lists", 0x1598CCu, &road_row_size_step_lists_data },
	{ "road_row_size_shift_lists", 0x159AFCu, &road_row_size_shift_lists_data },
	{ "race_car_rider_places", 0x18D25Cu, &race_car_rider_places_data },
	{ "perspective_scales", 0x19A340u, &perspective_scales_data },
	{ "depth_layers_by_depth", 0x1A0340u, &depth_layers_by_depth_data },
	{ "kanji_row_pixels", 0x1A6340u, &kanji_row_pixels_data },
	{ "kanji_font", 0x1CF000u, &kanji_font_data },
};

const size_t game_table_count = 541;

/* the code at the reset vector */
void game_entry(void)
{
	reset();
}
