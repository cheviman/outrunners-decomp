/**
 *    Name:        game.h
 *    Description: prototypes of the game's functions and the addresses of the code it keeps in memory slots
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    24-11-2026
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
extern uint32_t r12, r14, stack_pointer;
extern struct object *current_object;
extern struct car *player_car;
extern struct screen *screen;

/* the number of each function the game's state holds in a slot: an index into game_functions[] */
enum game_function
{
	NO_FUNCTION,
	FUNCTION_second_task,
	FUNCTION_isdn_status_update,
	FUNCTION_bar_picture_bar_start,
	FUNCTION_bar_picture_bar_fly,
	FUNCTION_bar_picture_bar_draw,
	FUNCTION_car_presentation_update,
	FUNCTION_staff_credits_code_start,
	FUNCTION_staff_credits_code_watch,
	FUNCTION_staff_credits_code_show,
	FUNCTION_staff_credits_code_done,
	FUNCTION_car_name_start,
	FUNCTION_car_name_letter_update,
	FUNCTION_name_entry_update,
	FUNCTION_name_entry_letter_update,
	FUNCTION_name_entry_lcd_update,
	FUNCTION_name_entry_car_update,
	FUNCTION_name_entry_drivers_update,
	FUNCTION_name_entry_course_name_update,
	FUNCTION_ranking_update,
	FUNCTION_ranking_background_update,
	FUNCTION_ranking_title_left_update,
	FUNCTION_ranking_title_right_update,
	FUNCTION_ranking_course_banner_update,
	FUNCTION_ranking_record_start,
	FUNCTION_ranking_record_character_update,
	FUNCTION_ranking_record_car_update,
	FUNCTION_passing_traffic_start,
	FUNCTION_passing_traffic_update,
	FUNCTION_passing_car_update,
	FUNCTION_route_map_update,
	FUNCTION_route_map_car_update,
	FUNCTION_route_map_sign_update,
	FUNCTION_route_map_landmark_update,
	FUNCTION_stage_result_line_draw,
	FUNCTION_route_map_leg_show,
	FUNCTION_route_map_leg_car_show,
	FUNCTION_route_map_checkpoint_show,
	FUNCTION_route_map_direction_sign_show,
	FUNCTION_route_map_wait_banner_show,
	FUNCTION_game_over_banner_show,
	FUNCTION_car_select_start,
	FUNCTION_car_select_run,
	FUNCTION_car_attendant_run,
	FUNCTION_turntable_car_show,
	FUNCTION_turntable_scenery_show,
	FUNCTION_welcome_letter_start,
	FUNCTION_welcome_letter_show,
	FUNCTION_select_car_heading_show,
	FUNCTION_select_instruction_show,
	FUNCTION_transmission_choice_run,
	FUNCTION_automatic_panel_show,
	FUNCTION_manual_panel_show,
	FUNCTION_transmission_label_start_automatic,
	FUNCTION_transmission_label_start_manual,
	FUNCTION_transmission_label_follow,
	FUNCTION_driver_couple_start,
	FUNCTION_car_caption_show,
	FUNCTION_car_gears_caption_show,
	FUNCTION_waiting_banner_show,
	FUNCTION_car_select_entries_run,
	FUNCTION_choice_flag_start,
	FUNCTION_choice_flag_follow,
	FUNCTION_stacked_choice_flag_start,
	FUNCTION_stacked_choice_flag_follow,
	FUNCTION_choice_flag_number_start,
	FUNCTION_choice_flag_number_follow,
	FUNCTION_entry_countdown_start,
	FUNCTION_entry_countdown_count,
	FUNCTION_entry_countdown_done,
	FUNCTION_entry_banner_start,
	FUNCTION_entry_banner_blink,
	FUNCTION_entry_car_ring_start,
	FUNCTION_entry_car_ring_draw,
	FUNCTION_entry_traffic_run,
	FUNCTION_entry_traffic_car_drive,
	FUNCTION_player_car_update,
	FUNCTION_rival_warning_update,
	FUNCTION_rival_alarm_update,
	FUNCTION_race_cars_start,
	FUNCTION_race_car_draw,
	FUNCTION_distance_offset_from_x,
	FUNCTION_distance_offset_from_z,
	FUNCTION_rev_meter_update,
	FUNCTION_speedometer_update,
	FUNCTION_speed_unit_update,
	FUNCTION_gear_indicator_update,
	FUNCTION_shift_up_hint_update,
	FUNCTION_place_count_update,
	FUNCTION_place_number_update,
	FUNCTION_lap_timer_update,
	FUNCTION_stage_indicator_start,
	FUNCTION_stage_indicator_draw,
	FUNCTION_start_message_update,
	FUNCTION_time_left_display_update,
	FUNCTION_extend_time_message_start,
	FUNCTION_extend_time_message_blink,
	FUNCTION_stage_entry_update,
	FUNCTION_goal_update,
	FUNCTION_race_result_start,
	FUNCTION_race_result_hold,
	FUNCTION_race_result_verdict_start,
	FUNCTION_race_result_verdict_blink,
	FUNCTION_branch_arrow_update,
	FUNCTION_checkpoint_board_update,
	FUNCTION_draw_object,
	FUNCTION_road_object_stream_read,
	FUNCTION_crest_clip_update,
	FUNCTION_scenery_update,
	FUNCTION_flashing_scenery_update,
	FUNCTION_scenery_pair_update,
	FUNCTION_overhead_scenery_update,
	FUNCTION_aurora_update,
	FUNCTION_fork_sign_update,
	FUNCTION_animated_scenery_update,
	FUNCTION_animated_scenery_pair_update,
	FUNCTION_road_crosser_update,
	FUNCTION_kangaroo_update,
	FUNCTION_moving_scenery_update,
	FUNCTION_swooping_helicopter_update,
	FUNCTION_start_gate_update,
	FUNCTION_start_flagman_update,
	FUNCTION_flagman_update,
	FUNCTION_start_balloon_update,
	FUNCTION_ambient_sound_update,
	FUNCTION_one_shot_sound_update,
	FUNCTION_west_east_panel_update,
	FUNCTION_santa_sleigh_update,
	FUNCTION_goal_parade_rightward_update,
	FUNCTION_sounding_scenery_update,
	FUNCTION_goal_parade_leftward_update,
	FUNCTION_goal_parade_dragon_update,
	FUNCTION_traffic_spawner_update,
	FUNCTION_traffic_car_start_convoy,
	FUNCTION_traffic_car_update,
	FUNCTION_traffic_follower_update,
	FUNCTION_pulled_vehicle_update,
	FUNCTION_traffic_stopping_car_update,
	FUNCTION_traffic_figure_start_team,
	FUNCTION_traffic_figure_update,
	FUNCTION_traffic_hit_sound_wait,
	FUNCTION_fork_branch_start_right,
	FUNCTION_fork_branch_start_left,
	FUNCTION_fork_branch_draw,
	FUNCTION_race_car_occupant_update,
	FUNCTION_rider_gesture_update,
	FUNCTION_race_opening_update,
	FUNCTION_stage_end_sounds_update,
	FUNCTION_fork_camera_update,
	FUNCTION_race_car_start_ending,
	FUNCTION_goal_actor_car_start,
	FUNCTION_goal_actor_driver_start,
	FUNCTION_goal_actor_passenger_start,
	FUNCTION_goal_actor_start,
	FUNCTION_goal_actor_run,
	FUNCTION_road_surface_update,
	FUNCTION_road_tiles_change_update,
	FUNCTION_race_background_update,
	FUNCTION_race_background_lower_animation_wait,
	FUNCTION_race_background_lower_animation_cycle,
	FUNCTION_race_background_upper_animation_wait,
	FUNCTION_race_background_upper_animation_cycle,
	FUNCTION_palette_cycle_update,
	FUNCTION_sunset_fade_start,
	FUNCTION_sunset_fade_wait,
	FUNCTION_backdrop_far_update,
	FUNCTION_backdrop_near_start,
	FUNCTION_backdrop_near_update,
	FUNCTION_backdrop_landmark_start,
	FUNCTION_backdrop_landmark_update,
	FUNCTION_backdrop_flashing_update,
	FUNCTION_backdrop_translucent_start,
	FUNCTION_backdrop_translucent_update,
	FUNCTION_race_background_fade_watch_update,
	FUNCTION_palette_fade_prepare,
	FUNCTION_palette_fade_step,
	FUNCTION_lcd_race_info_start,
	FUNCTION_lcd_race_info_update,
	FUNCTION_lcd_race_info_finish,
	FUNCTION_lcd_race_info_paused,
	FUNCTION_car_radio_start,
	FUNCTION_car_radio_wait_start,
	FUNCTION_car_radio_read_buttons,
	FUNCTION_car_radio_update,
	FUNCTION_song_title_show,
	FUNCTION_title_screen_update,
	FUNCTION_title_car_drive,
	FUNCTION_title_car_driver_update,
	FUNCTION_title_car_passenger_update,
	FUNCTION_title_palm_tree_update,
	FUNCTION_title_logo_update,
	FUNCTION_title_logo_copy_update,
	FUNCTION_title_logo_car_update,
	FUNCTION_title_logo_stripe_update,
	FUNCTION_title_board_palms_update,
	FUNCTION_title_seagull_fly,
	FUNCTION_title_picture_draw,
	FUNCTION_coin_prompt_update,
	FUNCTION_coin_prompt_car_drive,
	FUNCTION_coin_prompt_message_update,
	FUNCTION_ignition_key_prompt_update,
	FUNCTION_credit_display_update,
	FUNCTION_demo_captions_run,
	FUNCTION_demo_caption_update,
	FUNCTION_test_mode_menu,
	FUNCTION_test_mode_menu_run,
	FUNCTION_test_mode_menu_toggle_continue,
	FUNCTION_test_mode_memory_test,
	FUNCTION_test_mode_memory_test_run,
	FUNCTION_test_mode_volume_adjustment,
	FUNCTION_test_mode_volume_adjustment_select,
	FUNCTION_test_mode_volume_adjustment_adjust,
	FUNCTION_test_mode_input_test,
	FUNCTION_test_mode_input_test_run,
	FUNCTION_test_mode_input_test_release,
	FUNCTION_test_mode_output_test,
	FUNCTION_test_mode_output_test_run,
	FUNCTION_test_mode_sound_test,
	FUNCTION_test_mode_sound_test_run,
	FUNCTION_test_mode_crt_test,
	FUNCTION_test_mode_crt_test_bars_run,
	FUNCTION_test_mode_crt_test_grid,
	FUNCTION_test_mode_crt_test_grid_run,
	FUNCTION_test_mode_game_assignments,
	FUNCTION_test_mode_game_assignments_run,
	FUNCTION_test_mode_coin_assignments,
	FUNCTION_test_mode_coin_assignments_show,
	FUNCTION_test_mode_coin_assignments_run,
	FUNCTION_test_mode_coin_assignments_manual_run,
	FUNCTION_test_mode_network_assignments_start,
	FUNCTION_test_mode_network_assignments_run,
	FUNCTION_test_mode_bookkeeping_start,
	FUNCTION_test_mode_bookkeeping_wait,
	FUNCTION_test_mode_bookkeeping_second_page_start,
	FUNCTION_test_mode_bookkeeping_second_page_wait,
	FUNCTION_test_mode_backup_data_clear_start,
	FUNCTION_test_mode_backup_data_clear_run,
	FUNCTION_test_mode_backup_data_clear_wait,
	FUNCTION_test_mode_exit,
	FUNCTION_test_mode_exit_caution_wait,
	FUNCTION_int_vector_40,
	FUNCTION_int_vector_41,
	FUNCTION_int_vector_42,
	FUNCTION_int_vector_43,
	FUNCTION_int_vector_44,
	FUNCTION_COUNT    /* (how many numbers there are) */
};
uint32_t function_number(uint32_t address);     /* the number of the function at a V70 address read
                                                   out of the program ROM's tables (the projection's) */
void call_second_task(void);
void call_draw_object(void);
void call_test_mode_memory_test_run(void);
void call_test_mode_volume_adjustment_select(void);
void call_test_mode_input_test_run(void);
void call_test_mode_crt_test_bars_run(void);
void call_test_mode_crt_test_grid_run(void);
void call_test_mode_coin_assignments_run(void);
void call_test_mode_bookkeeping_wait(void);
void call_test_mode_bookkeeping_second_page_wait(void);

/* engine/objects.c: the objects: the object loop, starting and freeing objects and their lists, the random numbers */
void clear_objects(void);
void run_objects(void);
struct object *start_object(void);
uint32_t reserve_object_slot(void);
void free_object_slot(uint16_t object);
struct object *start_object_list(uint32_t handler_list);
uint32_t random_number(void);

/* engine/sprites.c: drawing an object: its draw modes, its sprites from its animation's frame, the rows of sprites along the road */
uint16_t draw_object(uint32_t routine_then_layer);
void draw_object_between_segments(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_from_road_edge(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_from_road_edge_unshifted(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_from_road_edge_doubled(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_car_window_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_segment(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_car_window_segment_on_road(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_segment_scale(uint16_t y, struct projected_segment *projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_shifted_scale(uint16_t y, uint32_t scale, struct projected_segment *projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_at_scale(uint16_t y, uint32_t scale, struct projected_segment *projected_segment, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_left_caller);
void draw_object_on_screen(uint32_t *p_leftover, uint32_t *p_sprite_entry);
void put_object_sprites(uint16_t y, uint32_t height_scale, uint32_t width_scale, uint32_t *p_x, uint32_t *p_sprite_entry, uint32_t *p_last_frame, uint32_t *p_last_size_step);
void draw_object_one_piece(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_last_size_step);
uint32_t draw_object_entry(uint32_t sprite_entry);
uint32_t draw_object_command(uint32_t sprite_entry);
void draw_object_smoothed(uint32_t *p_leftover, uint32_t *p_sprite_entry, uint32_t *p_last_frame, uint32_t *p_last_size_step);
uint32_t animation_advance(uint32_t leftover);
void animation_start(uint32_t animation);
void finish_sprite_list(void);
void road_row_draw(uint32_t rom_offset_and_palette, uint32_t piece_width, uint32_t *p_last_piece, uint32_t *p_step_height);
void road_row_pair_draw(uint32_t rom_offset_and_palette, uint32_t piece_width, uint32_t *p_last_piece, uint32_t *p_step_height);

/* objects/backgrounds.c: the backgrounds: their layers and animations, the palette cycles, the sunset, the backdrops */
void race_background_update(void);
void race_background_start_lower_animation(void);
void race_background_lower_animation_wait(void);
void race_background_lower_animation_cycle(void);
void race_background_start_upper_animation(uint32_t animation);
void race_background_upper_animation_wait(void);
void race_background_upper_animation_cycle(void);
void set_up_background(const struct background_setup *background_setup);
void race_background_start_objects(uint32_t stage);
void palette_cycle_update(void);
void sunset_fade_start(void);
void sunset_fade_wait(void);
void backdrop_far_start(void);
void backdrop_far_update(void);
void backdrop_near_start(void);
void backdrop_near_update(void);
void backdrop_landmark_start(void);
void backdrop_landmark_update(void);
void backdrop_flashing_start(void);
void backdrop_flashing_update(void);
void backdrop_translucent_start(void);
void backdrop_translucent_update(void);
uint32_t backdrop_off_screen(void);

/* objects/car_radio.c: the car radio: the songs, the DJ and his messages, the songs' titles */
void car_radio_start(void);
void car_radio_wait_start(void);
void car_radio_read_buttons(void);
void car_radio_update(void);
void car_radio_talk(void);
void car_radio_dj_leader(void);
void car_radio_dj_own_car(void);
void car_radio_dj_remark_2a(void);
void car_radio_dj_remark_2b(void);
void car_radio_dj_remark_2d(void);
void car_radio_dj_remark_2f(void);
void car_radio_dj_remark_2c(void);
uint8_t car_radio_song(uint32_t song_number);
void car_radio_show_title(uint32_t title);
struct object *start_song_title(uint32_t song_title);
void song_title_show(void);
void start_dj_message_if_quiet(uint8_t dj_message);
void start_dj_message(uint8_t message);

/* objects/race_people.c: the race's people and moments: the cars' occupants and their gestures, the opening, the stage end, the camera at the forks, the goal's actors */
void race_car_occupant_update(void);
void rider_gesture_update(void);
uint32_t rider_gesture_check_view(struct object *car);
uint32_t rider_gesture_check_place(uint8_t visible_mask, struct object *car);
void rider_gesture_place(uint16_t scale, struct object *car);
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
void goal_actor_rider_place(struct object *scene);
void goal_actor_start(void);
void goal_actor_run(void);

/* objects/road_surface.c: the road's surface drawn line by line, and its tiles' changes */
void road_surface_update(void);
void road_surface_start(void);
void road_surface_lines(uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_select_place, uint32_t *p_stripe_count, uint32_t *p_stripe, uint32_t *p_fraction_shift, uint32_t *p_rounding, uint32_t *p_max_depth);
void road_surface_halfway_lines(uint32_t previous_line, uint32_t previous_x, uint32_t previous_depth, uint32_t stripe_count, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_select_place, uint32_t *p_stripe);
void road_surface_fill_lines(uint32_t line, uint32_t point_line, uint32_t point_x, uint32_t point_depth, uint16_t stripe, uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_line, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_x_step, uint32_t *p_depth_step, uint32_t *p_line_scroll_place, uint32_t *p_select_place, uint32_t *p_zero);
void road_surface_fill_skipped_lines(uint8_t fraction_shift, uint32_t rounding, uint16_t max_depth, uint32_t *p_previous_x, uint32_t *p_previous_depth, uint32_t *p_select_place, uint32_t *p_stripe);
void road_surface_start_tiles_change(void);
void road_tiles_change_update(void);

/* objects/scenery.c: the road's objects: the streams that start them along the course, the scenery, the signs, the start gate and flagmen, the goal parades, the fork's branch */
void road_object_stream_read(void);
uint32_t road_object_stream_start_object(uint16_t position, uint32_t entry);
uint32_t road_object_stream_stage_end(void);
uint32_t road_object_stream_fork(uint32_t branch_streams);
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
void fork_branch_start_right(void);
void fork_branch_start_left(void);
void fork_branch_draw(void);
void fork_branch_sprite(uint32_t *p_size_and_attributes, uint32_t *p_height_word, uint32_t *p_width_bits, uint32_t *p_offset_palette);

/* objects/traffic.c: the traffic: its cars, convoys, people and their hits */
void traffic_spawner_update(void);
void traffic_car_start_convoy(void);
void traffic_car_update(void);
void traffic_car_start(void);
void traffic_car_draw(uint16_t position_fraction);
void traffic_follower_update(void);
void pulled_vehicle_update(void);
void traffic_stopping_car_update(void);
void traffic_figure_start_team(void);
void traffic_figure_update(void);
void traffic_start_hit_sound(void);
void traffic_hit_sound_wait(void);
uint32_t traffic_out_of_view(uint16_t position);

/* race/course.c: the course: its segments built ahead of the car, the road projected for the view */
void player_car_start_course(void);
void player_car_extend_course(void);
void player_car_build_segment(uint16_t ring_mask);
void player_car_project_road(uint32_t *p_matrix_yx, uint32_t *p_matrix_yz);
void note_road_crest(uint16_t crest_y, uint16_t crest_layer);
void rotate_offset(uint32_t y, uint32_t road_heading, uint32_t road_pitch, uint32_t *p_x, uint32_t *p_z);

/* race/hits.c: the angles of a vector, the hit boxes and the checks of the car against the road's objects and the traffic */
uint32_t vector_angle(uint32_t x, uint32_t y, uint32_t from_x, uint32_t from_y);
void vector_angles(uint32_t z, uint32_t from_x, uint32_t from_y, uint32_t from_z, uint32_t *p_x, uint32_t *p_y);
void player_car_build_hit_boxes(void);
uint32_t player_car_write_hit_box(uint16_t segment, uint32_t model, uint32_t hit_box, uint32_t racer_copy, uint16_t segment_fraction, uint16_t road_x);
void fork_sign_hit_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box);
void road_row_crash_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box);
void road_row_hit_check(uint8_t hit_kind, uint32_t *p_along_distance, uint32_t *p_next_hit_box);
void road_row_pair_crash_check(uint32_t *p_along_distance, uint32_t *p_next_hit_box);
void road_row_pair_crash_check_unflipped(void);
void player_car_check_car_hits(void);
uint32_t traffic_hit_check(void);

/* race/hud.c: the race's display: the rev meter, the speedometer, the gear, the place, the lap timer, the time left, the stage and goal messages, the race result, the checkpoint board */
void rev_meter_update(void);
void speedometer_update(void);
void speed_unit_update(void);
void gear_indicator_update(void);
void shift_up_hint_update(void);
void place_count_update(void);
void place_number_update(void);
void lap_timer_update(void);
void lap_timer_spare_return(void);
void stage_indicator_start(void);
void stage_indicator_draw(void);
void start_message_update(void);
void time_left_display_update(void);
void time_digits_start(void);
void time_digits_draw(uint32_t digits);
void time_left_display_check_extension(void);
void extend_time_message_start(void);
void extend_time_message_blink(void);
void stage_entry_update(void);
void stage_entry_spare_return(void);
void goal_update(void);
void race_result_start(void);
void race_result_hold(void);
void race_result_verdict_start(void);
void race_result_verdict_blink(void);
void branch_arrow_update(void);
void checkpoint_board_update(void);
void checkpoint_board_draw_row(uint32_t player);
void checkpoint_board_leader_time(uint32_t time, uint32_t player);
void time_to_text_thousandths(uint32_t time, uint32_t text);
void time_to_text(uint32_t time, uint32_t text);
void checkpoint_board_gap_time(uint32_t time, uint32_t player);

/* race/link.c: the link between the cabinets: the ISDN connection, the network check at boot, the race's entries and progress, the course records, the records sent and received */
void isdn_count_players(void);
void game_mode_isdn_connection(void);
void isdn_status_update(void);
void network_check_again(void);
void network_check(void);
void network_check_join(void);
void network_check_isdn(void);
void link_lost(void);
void network_check_stand_alone(void);
void network_check_result(uint32_t machine);
uint32_t network_check_board_memory(uint32_t left_caller);
uint8_t network_check_memory_test(uint32_t count, uint32_t memory);
uint32_t network_check_wait_board(uint32_t left_caller);
uint32_t network_check_wait_isdn(uint32_t left_caller);
void network_check_delay(void);
uint32_t network_check_greeting(uint32_t left_caller);
uint32_t network_check_five_ics(uint32_t left_caller);
uint32_t network_check_four_ics(uint32_t left_caller);
void network_check_show_ics(uint32_t ic_count, uint8_t self_test);
void network_check_blink_text(void);
void network_check_clear_status_line(void);
void network_check_toggle_memory_text(void);
uint32_t network_check_error(uint32_t error_texts);
uint32_t network_check_halt(void);
void link_take_master_settings(void);
uint32_t race_entries_update(uint32_t entry_place);
void race_progress_update(uint32_t *p_host_record, uint32_t *p_racer_copy);
void entry_invitation_step(void);
void race_record_clear(void);
void set_player_ready(void);
uint32_t players_not_ready(void);
struct course_record_entry *course_records_update(struct course_record_entry *record_entry);
void link_receive_records(void);
void link_isdn_receive_records(void);
void link_copy_records_in(uint32_t record_count, uint32_t board_records, uint32_t race_records);
void link_send_records(void);

/* race/physics.c: the car's physics: the gears and torque curves, the controls, driving along the road, the engine, the road's drag and its edges, the wheels' effects */
void player_car_load_gears(void);
void player_car_build_torque_curves(void);
void player_car_read_controls(void);
void player_car_drive(uint32_t *p_segment_record, uint32_t *p_next_segment, uint32_t *p_road_x, uint32_t *p_segment_fraction, uint32_t *p_segment_length);
void player_car_world_position(struct road_segment *segment_record, struct road_segment *next_segment, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length);
void player_car_place_view(struct road_segment *segment_record, uint32_t view_segment, uint32_t road_x, uint32_t segment_fraction, uint32_t segment_length);
void player_car_engine(uint32_t road_force, uint32_t drive_force, uint32_t force, const struct car_handling *handling, uint32_t gear_ratio);
void player_car_engine_wheelspin(uint32_t force, uint32_t top_speed, uint32_t gear_ratio);
void player_car_engine_grip(uint32_t force, uint32_t top_speed, uint32_t gear_ratio);
void player_car_engine_standing(void);
void player_car_engine_revs(uint32_t target_revs);
void angles_to_offset(uint32_t pitch, uint32_t yaw, uint32_t distance, uint32_t *p_offset_x, uint32_t *p_offset_y, uint32_t *p_offset_z);
void distance_offset_from_x(void);
void distance_offset_from_z(void);
void distance_offset_at_angle(void);
void player_car_next_surfaces(void);
uint32_t player_car_road_drag(uint32_t drag, uint16_t road_x, uint16_t road_half_width, uint16_t half_width, const struct car_handling *handling);
void wheel_effect_1_none(void);
void wheel_effect_1_surface_1_inner(const struct roadside_surface *surface);
void wheel_effect_1_surface_1_outer(const struct roadside_surface *surface);
void wheel_effect_1_surface_2_inner(const struct roadside_surface *surface);
void wheel_effect_1_surface_2_outer(const struct roadside_surface *surface);
void wheel_effect_2_none(void);
void wheel_effect_2_surface_1_inner(const struct roadside_surface *surface);
void wheel_effect_2_surface_1_outer(const struct roadside_surface *surface);
void wheel_effect_2_surface_2_inner(const struct roadside_surface *surface);
void wheel_effect_2_surface_2_outer(const struct roadside_surface *surface);
void player_car_off_road_shake(void);

/* race/player_car.c: the screen's car each frame: its place, the view, the rivals' warning, the crash physics */
void player_car_update(void);
void player_car_move(void);
void player_car_steer_to_stop(void);
void player_car_check_place(void);
struct view *player_car_publish(void);
struct view *clear_published_car(void);
void player_car_set_horizon(void);
void player_car_place_on_road(void);
void player_car_clear_hit(void);
void player_car_animate(void);
void player_car_steer_frame(uint32_t model);
uint16_t player_car_view_height(void);
void player_car_move_view(void);
void player_car_check_rivals(void);
void rival_warning_update(void);
void rival_alarm_update(void);
void player_car_load_crash_physics(void);
void player_car_crash_step(void);

/* race/race_cars.c: the cars of the race drawn as the screen's view sees them: the body, the spray, the pieces */
void race_cars_start(void);
void race_car_draw(void);
void race_car_draw_behind(uint32_t sprite_entry, struct race *race, uint32_t *p_last_piece, uint32_t *p_last_size_step);
void race_car_draw_link(uint32_t depth_layer, uint32_t sprite_entry);
uint32_t race_car_draw_spray(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint8_t wheel_effect_sides, uint32_t scale, uint32_t sprite_entry, uint16_t screen_x, uint16_t screen_y, struct race_car_spray *spray);
uint32_t race_car_draw_part(uint32_t view_offset, uint16_t x_offset, uint16_t y_offset, uint32_t scale, uint32_t sprite_entry, const struct car_part_place *places, uint32_t palette, uint16_t screen_x, uint16_t screen_y);
void race_car_put_frame(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_palette, uint32_t *p_last_size_step);
void race_car_put_sprite(uint32_t scale, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_attributes, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_palette, uint32_t *p_last_size_step);
void race_car_put_flat_sprite(uint16_t palette_word, uint16_t sprite_x, uint16_t sprite_y, uint32_t *p_sprite_entry, uint32_t *p_last_piece, uint32_t *p_last_size_step);

/* scenes/attract.c: the attract mode's scenes: the bar picture, the cabinet lamps, the car presentation and its car name, the staff credits code, the demo race's captions */
void bar_picture_start(void);
void bar_picture_bar_start(void);
void bar_picture_bar_fly(void);
void bar_picture_bar_draw(void);
void attract_lamps_update(void);
void car_presentation_update(void);
void staff_credits_code_start(void);
void staff_credits_code_watch(void);
void staff_credits_code_show(void);
void staff_credits_code_done(void);
void staff_credits_code_read_controls(uint32_t *p_new_controls, uint32_t *p_zero);
uint8_t staff_credits_code_control_bit(uint8_t held_before, uint8_t controls_now, uint8_t reading, uint32_t control_bit);
void car_name_start(void);
void car_name_letter_update(void);
void demo_captions_run(void);
void demo_captions_head_to_head(void);
void demo_captions_checkpoint(void);
void demo_captions_music_select(void);
void demo_captions_start(uint8_t first_record, uint32_t record_count);
void demo_caption_update(void);

/* scenes/car_select.c: the car select: the attendant and the turntable, the cars, the transmission, the flags of the other cabinets' choices, the countdown and the entry traffic */
void car_select_start(void);
void car_select_run(void);
void car_select_turn_to_choice(void);
uint32_t car_select_update_rowscroll(void);
void car_select_skew_floor_next(void);
void car_select_skew_floor_previous(void);
void car_select_skew_floor(uint16_t skew_step);
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
void choice_flag_start(void);
void choice_flag_follow(void);
void stacked_choice_flag_start(void);
void stacked_choice_flag_follow(void);
void choice_flag_number_start(void);
void choice_flag_number_follow(void);
uint16_t turntable_draw(uint32_t across, uint32_t down, uint32_t point_depth);
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

/* scenes/coin_prompts.c: the prompts for coins and the ignition key, and the credits shown */
void coin_prompt_update(void);
void coin_prompt_car_drive(void);
void coin_prompt_message_update(void);
struct object *coin_prompt_start_key_prompt(uint32_t piece_count, uint8_t first_piece);
void ignition_key_prompt_update(void);
void credit_display_update(void);
void credit_display_digit(void);

/* scenes/game_modes.c: the game modes: the title, the power on, the car presentation, the demo race, the ranking and the game itself */
void game_mode_title(void);
void game_mode_power_on(void);
void game_mode_restart(void);
void game_mode_car_presentation(void);
void game_mode_demo_race(void);
uint32_t demo_race_set_up(void);
void demo_race_fade_in(void);
void demo_race_start_timer(void);
void demo_race_count_down(void);
uint32_t demo_race_end(uint32_t left_caller);
void game_mode_ranking(void);
void game_mode_play(void);
void silence_and_clear_screen(void);
void clear_screen(void);
void clear_race_state(void);

/* scenes/kanji_text_viewer.c: a viewer of kanji texts nothing starts (unused) */
void kanji_text_viewer_update(void);
void kanji_text_viewer_draw_line(uint32_t place, uint32_t *p_text_ended, uint32_t *p_text);
uint32_t kanji_text_viewer_draw_character(uint32_t place, uint32_t rows);
void kanji_text_viewer_draw_quarter(uint32_t *p_bitmap_place, uint32_t *p_rows);
uint32_t kanji_text_viewer_character_address(uint32_t character);
void kanji_text_viewer_draw_number(uint32_t cell, uint16_t number);

/* scenes/name_entry.c: the name entry after a record: the certificate, the letters' drum, the LCD, the car, the drivers and the course's name */
uint32_t course_record_beaten(void);
void name_entry_update(void);
void name_entry_read_wheel(void);
void name_entry_choose(void);
void name_entry_finish(uint32_t name_place);
void name_entry_store_record(void);
void name_entry_draw_time(void);
void name_entry_letter_update(void);
void name_entry_lcd_update(void);
void name_entry_car_update(void);
void name_entry_drivers_update(void);
void name_entry_course_name_update(void);

/* scenes/ranking.c: the ranking: the course records, OutRunner of OutRunners, its titles, banners and record lines, and the traffic passing behind them */
uint32_t load_course_records(void);
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

/* scenes/route_map.c: the route map after the race: the car, the signs, the landmarks, the legs of the route, the stage results and the GAME OVER banner */
void route_map_update(void);
void route_map_car_update(void);
void route_map_sign_update(void);
void route_map_landmark_update(void);
void map_depth_from_y(void);
void stage_result_line_draw(void);
void route_map_leg_show(void);
void route_map_leg_car_show(void);
void route_map_leg_car_end(void);
void route_map_checkpoint_show(void);
void route_map_direction_sign_show(void);
void route_map_wait_banner_show(void);
void game_over_banner_show(void);

/* scenes/title.c: the title screen: the cars and riders, the palms, the logo, the seagull, the pictures */
void title_screen_update(void);
void title_car_drive(void);
void title_car_driver_update(void);
void title_car_passenger_update(void);
void title_rider_draw(void);
void title_screen_start_palms(void);
void title_palm_tree_update(void);
void title_logo_update(void);
void title_logo_copy_update(void);
void title_logo_car_update(void);
void title_logo_stripe_update(void);
void title_board_palms_update(void);
void title_seagull_fly(void);
void title_picture_draw(void);

/* system/coins.c: the coins and credits, and the bookkeeping */
void init_coins(void);
void update_coins(void);
void pulse_coin_meter(uint8_t meter_bit, struct coin_meter *coin_meter, uint32_t meter_bits);
void write_bookkeeping_credits(uint32_t bookkeeping_place);
uint16_t add_service_credit(uint16_t credits_given, struct seat_credits *seat_credits, uint32_t service_sounds);
uint16_t insert_coin(uint8_t released_switches, uint16_t credits_given, uint32_t chute_entry);
void show_title_on_credit(struct screen *screen_block);
void bookkeeping_increment16(uint32_t counter);
void bookkeeping_increment8(uint32_t counter_address);
void bookkeeping_add16(uint32_t counter, uint16_t amount);
void bookkeeping_add32(uint32_t counter, uint32_t amount);
void bookkeeping_add8(uint32_t counter, uint8_t amount);
void bookkeeping_set16(uint32_t counter, uint16_t amount);
void bookkeeping_set32(uint32_t counter, uint32_t amount);
void bookkeeping_set8(uint32_t counter, uint8_t amount);
uint32_t bookkeeping_clear_credits(void);
void bookkeeping_check(void);
uint16_t bookkeeping_reset(uint32_t checksum);
uint16_t bookkeeping_copy(uint32_t checksum, uint32_t source, uint32_t destination);
uint16_t bookkeeping_update_checksum(uint16_t checksum);
void bookkeeping_check_block(void);
uint16_t bookkeeping_checksum(uint32_t checksum, uint32_t block);

/* system/inputs.c: the controls: the switches and the analog inputs (wheel, pedals), each seat's inputs and the screens' new presses */
void read_switches_and_analog(void);
void pack_seat_inputs(void);
void link_send_seat_inputs(uint8_t seat_buttons, uint32_t link_event_place, uint32_t *p_analog_place, uint32_t *p_link_place);
void queue_seat_inputs(void);
void queue_screen_inputs(struct screen *seat_screen, struct seat_inputs *seat_record, uint32_t io_chip);
uint32_t take_screen_inputs(void);

/* system/interrupts.c: the interrupts: the VBLANK (the tasks, the mixer and scrolls, the test button, the sound requests, the lamps), the timers and the link */
void int_vector_40(void);
void vblank_update_mixer(void);
void vblank_update_scrolls(void);
void int_vector_41(void);
void int_vector_42(void);
void int_vector_43(void);
void int_vector_44(void);
void int_default(void);
void int_vector_02(void);
void check_test_button(void);
void send_sound_requests(void);
void count_total_time(void);
void update_lamps(void);
void update_button_lamps(struct screen *screen_block, uint32_t port_g);

/* system/lcd.c: the cabinets' LCD: its driver and the race's information on it */
void lcd_start(void);
void lcd_write_character(uint8_t character);
void lcd_write_data_nibble(uint8_t nibble);
void lcd_command_nibble_after_pause(uint8_t nibble);
void lcd_write_command_nibble(uint8_t nibble);
void lcd_reset(void);
void lcd_refresh(void);
void lcd_send_next_nibble(void);
void lcd_update(void);
void clear_lcd(void);
void lcd_race_info_start(void);
void lcd_race_info_update(void);
void lcd_race_info_time_page(void);
void lcd_race_info_time(void);
void lcd_race_info_average_page(void);
void lcd_race_info_average(void);
void lcd_race_info_write_number(uint32_t speed, uint32_t text_place);
void lcd_race_info_finish(void);
void lcd_race_info_paused(void);

/* system/main.c: the machine's main program: the boot and its checks, the second task, the frame wait, the start of a game, a screen and the board, the reset */
void boot(void);
uint16_t crc16(uint32_t bytes, uint32_t byte_count);
uint16_t signature_checksum(void);
void second_task(uint32_t entry_point);
void second_task_frames(void);
uint32_t check_game_start(uint32_t mode, struct seat_credits *seat_credits);
void wait_for_frame(void);
void initialise_game(void);
void initialise_screen(void);
void initialise_board(uint32_t checksum);
void reset(void);

/* system/settings.c: the settings in the EEPROM: loading, checking and saving them, and the EEPROM's serial lines */
void load_settings(uint32_t checksum);
void count_eeprom_write(void);
void eeprom_lock(void);
void eeprom_read_one_word(uint8_t command);
void reload_settings(uint32_t eeprom_word);
void eeprom_read_settings(uint32_t eeprom_word);
void eeprom_write_one_word(uint16_t eeprom_word, uint8_t command);
void save_settings(void);
void eeprom_write_settings_counted(void);
void eeprom_write_settings(void);
void eeprom_read_word(uint8_t command, uint32_t eeprom_port, uint32_t lines_port, uint32_t *p_eeprom_word, uint32_t *p_lines);
uint8_t eeprom_write_word(uint16_t eeprom_word, uint8_t lines, uint8_t command, uint32_t eeprom_port, uint32_t lines_port);
uint8_t eeprom_send_command(uint8_t lines, uint8_t command, uint32_t lines_port);
void eeprom_begin(uint32_t *p_lines, uint32_t *p_eeprom_port, uint32_t *p_lines_port);
uint8_t eeprom_set_lines(uint8_t lines, uint32_t lines_port);

/* system/sound.c: the sound requests to the sound program: sounds, the engine and tyre noise, the music's fade */
void start_sound_program(void);
void request_sound(uint8_t sound);
void queue_sound(uint8_t sound);
void request_second_sound(uint8_t sound);
void player_car_start_engine_sound(void);
void player_car_engine_sound(void);
void set_tyre_noise_on_road(uint8_t tyre_noise);
void set_tyre_noise(uint8_t tyre_noise);
void fade_out_music(void);
void fade_out_music_at(uint8_t fade_delay);
void reset_screen_sound(void);

/* system/test_mode.c: the test mode: its menu and pages (memory, volume, input, output, sound, CRT, game, coins, network, bookkeeping, backup clear) and their drawing */
uint32_t test_mode_start(void);
void test_mode_frame(void);
void test_mode_menu(void);
void test_mode_menu_run(void);
uint16_t test_mode_menu_show_write_count(uint16_t cell_value);
void test_mode_menu_toggle_continue(void);
void test_mode_memory_test(void);
void test_mode_memory_test_run(uint16_t character);
void test_mode_memory_test_draw(uint32_t *p_cell_value, uint32_t *p_text_place);
void test_mode_memory_test_checksum(uint32_t memory_start, uint32_t *p_byte_count, uint32_t *p_odd_check);
void BOARD_ORIGINAL(test_mode_memory_test_checksum)(uint32_t memory_start, uint32_t *p_byte_count, uint32_t *p_odd_check);
uint32_t test_mode_memory_test_words(uint32_t byte_count, uint32_t memory_start);
uint16_t test_mode_memory_test_halfwords(uint32_t size, uint32_t memory_start);
uint16_t test_mode_memory_test_bytes(uint32_t size_then_wrong_bits, uint32_t ram_start);
uint16_t test_mode_memory_test_nibbles(uint32_t size, uint32_t ram_start);
void test_mode_memory_test_blink(void);
void test_mode_volume_adjustment(void);
void test_mode_volume_adjustment_select(uint32_t character);
void test_mode_volume_adjustment_adjust(void);
void test_mode_volume_adjustment_draw(uint32_t *p_character, uint32_t *p_last_analog_input);
void test_mode_input_test(void);
void test_mode_input_test_run(uint32_t character);
void test_mode_input_test_release(void);
void test_mode_input_test_draw(uint32_t *p_character, uint32_t *p_last_port, uint32_t *p_last_bar_cells);
void test_mode_output_test(void);
void test_mode_output_test_run(void);
uint32_t test_mode_output_test_draw(void);
void test_mode_output_test_toggle(void);
void test_mode_output_test_exit(void);
void test_mode_sound_test(void);
void test_mode_sound_test_run(void);
void test_mode_sound_test_draw(void);
void test_mode_sound_test_play_a(void);
void test_mode_sound_test_play_b(void);
void test_mode_sound_test_exit(void);
void test_mode_crt_test(void);
void test_mode_crt_test_bars_run(uint16_t character);
void test_mode_crt_test_grid(void);
void test_mode_crt_test_grid_run(uint16_t character);
uint16_t test_mode_crt_test_draw_bars(uint32_t character);
uint32_t test_mode_crt_test_draw_bar(uint16_t character, uint32_t cell);
uint16_t test_mode_crt_test_draw_grid(uint32_t character);
void test_mode_game_assignments(void);
void test_mode_game_assignments_run(void);
void test_mode_game_assignments_draw(uint32_t *p_character, uint32_t *p_difficulty_seconds);
void test_mode_game_assignments_difficulty(void);
void test_mode_game_assignments_time(void);
void test_mode_game_assignments_steering(void);
void test_mode_game_assignments_advertise(void);
void test_mode_game_assignments_exit(void);
void test_mode_coin_assignments(void);
void test_mode_coin_assignments_show(void);
void test_mode_coin_assignments_run(uint32_t item_count);
void test_mode_coin_assignments_draw(uint32_t *p_character, uint32_t *p_colours, uint32_t *p_line_cell);
void test_mode_coin_assignments_next_setting(void);
void test_mode_coin_assignments_manual_start(uint32_t item_count);
void test_mode_coin_assignments_manual_run(void);
uint16_t test_mode_coin_assignments_manual_draw(uint32_t item_count);
uint32_t test_mode_draw_two_digits(uint32_t number, uint32_t cell);
void test_mode_coin_assignments_step_coin_to_credit(uint32_t item_count);
void test_mode_coin_assignments_step_value(uint32_t setting_offset, uint32_t item_count);
void test_mode_coin_assignments_step_bonus_adder(uint32_t item_count);
void test_mode_coin_assignments_step_multiplier(uint32_t item_count);
void test_mode_coin_assignments_manual_exit(void);
void test_mode_coin_assignments_exit(void);
void test_mode_network_assignments_start(void);
void test_mode_network_assignments_run(void);
uint16_t test_mode_network_assignments_draw(void);
void test_mode_network_assignments_toggle_communication(void);
void test_mode_network_assignments_toggle_privilege_mode(void);
void test_mode_network_assignments_step_cabinet_id(void);
void test_mode_network_assignments_exit(void);
void test_mode_bookkeeping_start(void);
void test_mode_bookkeeping_wait(uint16_t character);
void test_mode_bookkeeping_second_page_start(void);
void test_mode_bookkeeping_second_page_wait(uint16_t character);
uint16_t test_mode_bookkeeping_draw_totals(void);
void test_mode_bookkeeping_draw_histogram(uint32_t *p_last_character, uint32_t *p_cell);
uint32_t test_mode_draw_duration(uint32_t seconds, uint32_t cell);
void test_mode_draw_minutes_seconds(uint32_t *p_seconds, uint32_t *p_cell);
uint32_t test_mode_duration_to_seconds(struct stored_duration *duration);
void test_mode_split_minutes_seconds(uint32_t seconds, struct stored_duration *stored_time);
void test_mode_backup_data_clear_start(void);
void test_mode_backup_data_clear_run(void);
void test_mode_backup_data_clear_wait(void);
void test_mode_exit(void);
void test_mode_exit_caution_wait(void);
uint16_t test_mode_set_up_colours(void);
void test_mode_draw_instructions(uint32_t character);
void test_mode_show_exit_prompt(void);
void test_mode_show_continue_prompt(void);
void test_mode_blink_prompt(uint16_t character);
uint16_t test_mode_set_cursor(uint32_t character, uint32_t cell);
void test_mode_clear_cursor(void);
uint16_t test_mode_draw_texts(uint16_t character, uint32_t text);
void test_mode_draw_count(uint32_t number, uint32_t digit_count, uint32_t plural_offset, uint32_t *p_colours, uint32_t *p_cell);
void test_mode_draw_number(uint32_t digit_count, uint32_t *p_number, uint32_t *p_colour, uint32_t *p_cell);
uint32_t test_mode_draw_hex_number(uint32_t number, uint8_t colour, uint32_t digit_count, uint32_t cell);
uint16_t test_mode_colour_line(uint16_t colour, uint32_t cell);
uint16_t test_mode_clear_text(void);
void test_mode_read_controls(void);
void test_mode_send_sound(void);
void test_mode_queue_sound_screen_2(uint16_t request);
void test_mode_queue_sound(uint16_t request);

/* video/palettes.c: the palettes: the boot's, the stages', the upload queue, the screen fades and the palette fades */
void upload_boot_palettes(void);
void queue_stage_load(void);
void queue_palette(uint32_t colour_block);
void queue_palette_list(uint32_t colour_list);
void queue_palette_upload(uint32_t list_flag, uint32_t colour_record);
void upload_queued_palettes(void);
void upload_palette(uint32_t colour_block);
void upload_palette_list(uint32_t colour_list);
void step_screen_fade(void);
void start_palette_fades(uint32_t fade_list);
void road_surface_fade_back(uint32_t fade_list);
void race_background_fade_palettes(uint32_t fade_list);
void race_background_fade_watch_update(void);
struct object *start_palette_fade(uint8_t blocks, uint8_t period, uint32_t from, uint32_t to, uint16_t palette_offset);
void palette_fade_prepare(void);
void palette_fade_step(void);

/* video/text.c: the text: the fonts, the text layer's strings, numbers in hexadecimal and BCD, text in sprites */
void load_text_font(void);
void load_shadowed_text_font(void);
void print_text_layer_list(const struct text_layer_record *text_record);
uint32_t print_text_layer(uint32_t text_record);
void erase_text_layer_to_blank_character(const struct text_layer_record *text_record);
void print_text_layer_halfwords(uint32_t text_record);
void erase_text_layer_to_blank_cell(const struct text_layer_record *text_record);
void print_hex_byte(uint32_t number, uint16_t cell, uint32_t cell_place);
void print_hex_halfword(uint32_t number, uint16_t cell, uint32_t cell_place);
void print_hex_24_bits(uint32_t number, uint16_t cell, uint32_t cell_place);
void print_hex_word(uint32_t number, uint16_t cell, uint32_t cell_place);
void print_hex_digits(uint32_t number, uint16_t cell, uint32_t digit_count, uint32_t cell_place);
uint32_t bcd_to_binary(uint32_t value);
uint32_t binary_to_bcd(uint32_t number);
void draw_text_at(uint32_t place, uint16_t attribute, uint32_t text);
uint32_t draw_text(const struct sprite_text_record *text_record);
uint32_t draw_text_sprites(uint32_t sprite_words_0_1, uint32_t sprite_size, uint32_t place, uint16_t font, uint16_t attribute, uint32_t sprite, uint32_t sprite_copy, uint32_t text);

/* video/unpack.c: unpacking the tile pages and the lists of packed blocks, a step each frame */
void unpack(uint32_t source, uint32_t destination);
void unpack_queued_step(void);
void unpack_list(const struct packed_block_entry *block_list);
uint32_t unpack_tile_pages(uint32_t page_list, uint32_t list_next);
void queue_unpack_list(const struct packed_block_entry *block_list);
void queue_tile_pages(uint32_t page_list);

/* video/video.c: the video chips set up and cleared: the screens, the tile layers, the sprite chip and its lists, the mixer, the I/O chips */
uint32_t init_screen(void);
void clear_screen_ram(void);
uint16_t init_video(void);
uint16_t reset_tile_layers(void);
void clear_video_ram(void);
void clear_text_layer(void);
void clear_tile_pages(uint16_t last_page);
void clear_tile_page(uint16_t page);
void init_sprite_chip(void);
void init_sprite_lists(void);
void clear_sprite_group_count(void);
void init_mixer(void);
void init_io_chips(void);
void load_sprite_ram_tables(void);
void unrolled_copy(uint16_t byte_count, uint32_t source, uint32_t destination);

#endif
