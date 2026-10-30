/**
 *    Name:        ram.h
 *    Description: the game's variables, tables, board registers and structures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#ifndef ORUNNERS_RAM_H
#define ORUNNERS_RAM_H

/* the game's variables in work RAM, by their addresses (the fields of struct game, at the end);
   SCREEN_1 and SCREEN_2 are the blocks of the two screens, which the code reaches through screen
   (R25 of the task that runs each) */
#define GAME_VARIABLES 0x20E000u
#define SCREEN_1 0x208000u
#define RACE_1 0x20FB00u
#define RACE_2 0x20FB80u
#define RACE_COPY_1 0x20FC00u
#define RACE_COPY_2 0x20FC80u
#define SCREEN_2 0x218000u
#define waiting_task_frame_ADDRESS 0x20E000u
#define waiting_task_stack_ADDRESS 0x20E004u
#define vblanks_since_start_ADDRESS 0x20E00Au
#define debug_timer_rounds_ADDRESS 0x20E00Eu
#define sprite_list_buffers_ADDRESS 0x20E020u
#define sprite_lists_set_up_ADDRESS 0x20E022u
#define frame_ready_ADDRESS 0x20E023u
#define single_task_ADDRESS 0x20E024u
#define game_frame_count_ADDRESS 0x20E026u
#define send_video_registers_ADDRESS 0x20E027u
#define task_swapping_ADDRESS 0x20E028u
#define second_task_waiting_ADDRESS 0x20E02Bu
#define timer_interrupts_left_ADDRESS 0x20E02Cu
#define vblank_counter_ADDRESS 0x20E02Du
#define cleared_on_race_over_ADDRESS 0x20E02Eu
#define car_numbers_ADDRESS 0x20E030u
#define link_wait_frames_start_ADDRESS 0x20E038u
#define total_time_count_ADDRESS 0x20E03Au
#define machine_count_ADDRESS 0x20E040u
#define player_count_ADDRESS 0x20E041u
#define link_off_ADDRESS 0x20E042u
#define link_failed_ADDRESS 0x20E043u
#define link_wait_frames_ADDRESS 0x20E046u
#define network_check_blink_counter_ADDRESS 0x20E048u
#define isdn_players_this_side_ADDRESS 0x20E04Au
#define isdn_players_other_side_ADDRESS 0x20E04Bu
#define link_wait_count_ADDRESS 0x20E060u
#define coin_meters_ADDRESS 0x20E080u
#define seat1_coin_sounds_ADDRESS 0x20E08Cu
#define seat2_coin_sounds_ADDRESS 0x20E08Du
#define seat1_service_sounds_ADDRESS 0x20E08Eu
#define seat2_service_sounds_ADDRESS 0x20E08Fu
#define chute_entry_offset_ADDRESS 0x20E091u
#define port_d_outputs_ADDRESS 0x20E092u
#define port_g_outputs_ADDRESS 0x20E094u
#define seat1_credits_ADDRESS 0x20E0A0u
#define seat1_coin_units_ADDRESS 0x20E0A1u
#define seat1_bookkeeping_byte_ADDRESS 0x20E0A4u
#define seat2_credits_ADDRESS 0x20E0A8u
#define seat2_coin_units_ADDRESS 0x20E0A9u
#define seat2_bookkeeping_byte_ADDRESS 0x20E0ACu
#define checkpoint_lamp_ADDRESS 0x20E0C0u
#define checkpoint_lamp_blinks_ADDRESS 0x20E0C1u
#define checkpoint_lamp_period_ADDRESS 0x20E0C2u
#define checkpoint_lamp_timer_ADDRESS 0x20E0C3u
#define checkpoint_lamp_2_ADDRESS 0x20E0C4u
#define checkpoint_lamp_2_blinks_ADDRESS 0x20E0C5u
#define checkpoint_lamp_2_period_ADDRESS 0x20E0C6u
#define checkpoint_lamp_2_timer_ADDRESS 0x20E0C7u
#define leader_lamp_ADDRESS 0x20E0C8u
#define leader_lamp_blinks_ADDRESS 0x20E0C9u
#define leader_lamp_period_ADDRESS 0x20E0CAu
#define leader_lamp_timer_ADDRESS 0x20E0CBu
#define leader_lamp_2_ADDRESS 0x20E0CCu
#define leader_lamp_2_blinks_ADDRESS 0x20E0CDu
#define leader_lamp_2_period_ADDRESS 0x20E0CEu
#define leader_lamp_2_timer_ADDRESS 0x20E0CFu
#define wheel_motor_ADDRESS 0x20E0D0u
#define screen_2_vblanks_ADDRESS 0x20E0FCu
#define screen_2_vblanks_high_ADDRESS 0x20E0FDu
#define screen_1_vblanks_ADDRESS 0x20E0FEu
#define screen_1_vblanks_high_ADDRESS 0x20E0FFu
#define switches_ADDRESS 0x20E100u
#define previous_switches_ADDRESS 0x20E101u
#define switches_pressed_ADDRESS 0x20E102u
#define switches_released_ADDRESS 0x20E103u
#define second_io_chip_port_f_ADDRESS 0x20E104u
#define analog_raw_ADDRESS 0x20E110u
#define analog_inputs_ADDRESS 0x20E116u
#define analog_gas_1_ADDRESS 0x20E117u
#define analog_brake_1_ADDRESS 0x20E118u
#define analog_wheel_2_ADDRESS 0x20E119u
#define analog_gas_2_ADDRESS 0x20E11Au
#define analog_brake_2_ADDRESS 0x20E11Bu
#define seat_1_player_ADDRESS 0x20E120u
#define seat_1_buttons_ADDRESS 0x20E121u
#define seat_1_wheel_ADDRESS 0x20E122u
#define seat_1_gas_ADDRESS 0x20E123u
#define seat_1_brake_ADDRESS 0x20E124u
#define seat_2_player_ADDRESS 0x20E125u
#define seat_2_buttons_ADDRESS 0x20E126u
#define seat_2_wheel_ADDRESS 0x20E127u
#define seat_2_gas_ADDRESS 0x20E128u
#define seat_2_brake_ADDRESS 0x20E129u
#define video_registers_ADDRESS 0x20E200u
#define scroll_targets_ADDRESS 0x20E290u
#define tile_banks_ADDRESS 0x20E2B0u
#define course_records_ADDRESS 0x20E300u
#define course_record_times_ADDRESS 0x20E304u
#define lcd_send_buffer_1_ADDRESS 0x20E3C0u
#define lcd_send_buffer_2_ADDRESS 0x20E3D0u
#define lcd_restart_frames_ADDRESS 0x20E3F0u
#define lcd_send_step_ADDRESS 0x20E3F2u
#define lcd_busy_ADDRESS 0x20E3F3u
#define lcd_init_step_ADDRESS 0x20E3F4u
#define settings_magic_ADDRESS 0x20E600u
#define settings_signature_ADDRESS 0x20E604u
#define settings_checksum_ADDRESS 0x20E608u
#define eeprom_write_count_ADDRESS 0x20E60Au
#define flip_screen_ADDRESS 0x20E60Cu
#define advertise_sound_ADDRESS 0x20E60Du
#define coins_per_credit_ADDRESS 0x20E610u
#define first_chute_multiplier_ADDRESS 0x20E611u
#define second_chute_multiplier_ADDRESS 0x20E612u
#define bonus_adder_ADDRESS 0x20E613u
#define coin_setting_ADDRESS 0x20E614u
#define free_play_ADDRESS 0x20E615u
#define stand_alone_ADDRESS 0x20E620u
#define link_master_ADDRESS 0x20E621u
#define cabinet_id_ADDRESS 0x20E622u
#define isdn_mode_ADDRESS 0x20E623u
#define steering_wheel_reaction_ADDRESS 0x20E624u
#define game_difficulty_ADDRESS 0x20E630u
#define time_difficulty_ADDRESS 0x20E632u
#define volume_ranges_ADDRESS 0x20E638u
#define backup_settings_checksum_ADDRESS 0x20E644u
#define backup_eeprom_write_count_ADDRESS 0x20E646u
#define test_mode_on_ADDRESS 0x20E700u
#define test_mode_flags_ADDRESS 0x20E701u
#define test_mode_cursor_ADDRESS 0x20E702u
#define test_mode_page_cursor_ADDRESS 0x20E703u
#define test_mode_routine_ADDRESS 0x20E704u
#define test_mode_blink_address_ADDRESS 0x20E708u
#define test_mode_buttons_ADDRESS 0x20E70Cu
#define test_mode_buttons_previous_ADDRESS 0x20E70Du
#define test_mode_buttons_pressed_ADDRESS 0x20E70Eu
#define test_mode_buttons_released_ADDRESS 0x20E70Fu
#define test_mode_analog_inputs_ADDRESS 0x20E710u
#define memory_test_results_ADDRESS 0x20E716u
#define test_mode_blink_timer_ADDRESS 0x20E72Fu
#define sound_test_number_a_ADDRESS 0x20E730u
#define sound_test_number_b_ADDRESS 0x20E731u
#define test_mode_sound_queue_write_ADDRESS 0x20E732u
#define test_mode_sound_queue_read_ADDRESS 0x20E733u
#define test_mode_sound_queue_ADDRESS 0x20E734u
#define test_mode_privilege_mode_ADDRESS 0x20E754u
#define master_time_difficulty_ADDRESS 0x20FC0Bu
#define race_copy_window_segments_ADDRESS 0x20FC46u

/* work RAM as one family of functions uses it (their scratch variables) */
#define test_mode_prompt_text_ADDRESS 0x20E708u

/* data in the program ROM the code reads */
#define boot_e80000_bytes_ADDRESS 0x0000B4u
extern const void *boot_e80000_bytes_data;
#define boot_e80000_bytes (*(const uint32_t *)boot_e80000_bytes_data)
#define seat_credits_addresses_ADDRESS 0x0008C4u
extern const void *seat_credits_addresses_data;
#define seat_credits_addresses ((const uint32_t *)seat_credits_addresses_data)
#define game_mode_jumps_ADDRESS 0x0008CCu
extern const void *game_mode_jumps_data;
#define game_mode_jumps ((const uint32_t *)game_mode_jumps_data)
#define analog_calibration_addresses_ADDRESS 0x000B0Cu
extern const void *analog_calibration_addresses_data;
#define analog_calibration_addresses ((const uint32_t *)analog_calibration_addresses_data)
#define analog_input_counts_ADDRESS 0x000B14u
extern const void *analog_input_counts_data;
#define analog_input_counts ((const uint8_t *)analog_input_counts_data)
#define first_attract_car_lists_ADDRESS 0x000D14u
extern const void *first_attract_car_lists_data;
#define first_attract_car_lists ((const uint32_t *)first_attract_car_lists_data)
#define isdn_connection_step_jumps_ADDRESS 0x000DACu
extern const void *isdn_connection_step_jumps_data;
#define isdn_connection_step_jumps ((const uint32_t *)isdn_connection_step_jumps_data)
#define isdn_status_state_jumps_ADDRESS 0x000EB0u
extern const void *isdn_status_state_jumps_data;
#define isdn_status_state_jumps ((const uint32_t *)isdn_status_state_jumps_data)
#define isdn_connecting_texts_ADDRESS 0x000F78u
extern const void *isdn_connecting_texts_data;
#define isdn_connecting_texts ((const uint32_t *)isdn_connecting_texts_data)
#define isdn_failed_text_ADDRESS 0x000F80u
extern const void *isdn_failed_text_data;
#define isdn_failed_text (*(const uint32_t *)isdn_failed_text_data)
#define isdn_success_text_ADDRESS 0x000F84u
extern const void *isdn_success_text_data;
#define isdn_success_text (*(const uint32_t *)isdn_success_text_data)
#define isdn_this_side_text_ADDRESS 0x000F88u
extern const void *isdn_this_side_text_data;
#define isdn_this_side_text (*(const uint32_t *)isdn_this_side_text_data)
#define isdn_other_side_text_ADDRESS 0x000F8Cu
extern const void *isdn_other_side_text_data;
#define isdn_other_side_text (*(const uint32_t *)isdn_other_side_text_data)
#define isdn_total_text_ADDRESS 0x000F90u
extern const void *isdn_total_text_data;
#define isdn_total_text (*(const uint32_t *)isdn_total_text_data)
#define title_step_jumps_ADDRESS 0x001014u
extern const void *title_step_jumps_data;
#define title_step_jumps ((const uint32_t *)title_step_jumps_data)
#define car_presentation_step_jumps_ADDRESS 0x00146Cu
extern const void *car_presentation_step_jumps_data;
#define car_presentation_step_jumps ((const uint32_t *)car_presentation_step_jumps_data)
#define demo_race_step_jumps_ADDRESS 0x001530u
extern const void *demo_race_step_jumps_data;
#define demo_race_step_jumps ((const uint32_t *)demo_race_step_jumps_data)
#define car_presentation_state_jumps_ADDRESS 0x001694u
extern const void *car_presentation_state_jumps_data;
#define car_presentation_state_jumps ((const uint32_t *)car_presentation_state_jumps_data)
#define unused_attract_car_name_sounds_ADDRESS 0x00183Bu
extern const void *unused_attract_car_name_sounds_data;
#define unused_attract_car_name_sounds (*(const uint8_t *)unused_attract_car_name_sounds_data)
#define unused_attract_car_animations_2_ADDRESS 0x001844u
extern const void *unused_attract_car_animations_2_data;
#define unused_attract_car_animations_2 (*(const uint16_t *)unused_attract_car_animations_2_data)
#define unused_attract_car_palettes_2_ADDRESS 0x001854u
extern const void *unused_attract_car_palettes_2_data;
#define unused_attract_car_palettes_2 (*(const uint16_t *)unused_attract_car_palettes_2_data)
#define unused_attract_car_animations_1_ADDRESS 0x001864u
extern const void *unused_attract_car_animations_1_data;
#define unused_attract_car_animations_1 (*(const uint16_t *)unused_attract_car_animations_1_data)
#define unused_attract_car_palettes_1_ADDRESS 0x001874u
extern const void *unused_attract_car_palettes_1_data;
#define unused_attract_car_palettes_1 (*(const uint16_t *)unused_attract_car_palettes_1_data)
#define unused_attract_car_caption_palettes_ADDRESS 0x001884u
extern const void *unused_attract_car_caption_palettes_data;
#define unused_attract_car_caption_palettes (*(const uint16_t *)unused_attract_car_caption_palettes_data)
#define car_name_letter_state_jumps_ADDRESS 0x0019CCu
extern const void *car_name_letter_state_jumps_data;
#define car_name_letter_state_jumps ((const uint32_t *)car_name_letter_state_jumps_data)
#define attract_car_picture_animations_ADDRESS 0x001A88u
extern const void *attract_car_picture_animations_data;
#define attract_car_picture_animations ((const uint16_t *)attract_car_picture_animations_data)
#define attract_car_scene_palettes_ADDRESS 0x001A98u
extern const void *attract_car_scene_palettes_data;
#define attract_car_scene_palettes ((const uint8_t *)attract_car_scene_palettes_data)
#define car_presentation_palette_cycle_ADDRESS 0x001AC8u
extern const void *car_presentation_palette_cycle_data;
#define car_presentation_palette_cycle ((const uint32_t *)car_presentation_palette_cycle_data)
#define attract_car_names_ADDRESS 0x001CECu
extern const void *attract_car_names_data;
#define attract_car_names ((const uint32_t *)attract_car_names_data)
#define ranking_step_jumps_ADDRESS 0x001DE0u
extern const void *ranking_step_jumps_data;
#define ranking_step_jumps ((const uint32_t *)ranking_step_jumps_data)
#define name_entry_state_jumps_ADDRESS 0x001ED0u
extern const void *name_entry_state_jumps_data;
#define name_entry_state_jumps ((const uint32_t *)name_entry_state_jumps_data)
#define name_entry_gas_step_jumps_ADDRESS 0x002088u
extern const void *name_entry_gas_step_jumps_data;
#define name_entry_gas_step_jumps ((const uint32_t *)name_entry_gas_step_jumps_data)
#define name_entry_place_jumps_ADDRESS 0x0020C8u
extern const void *name_entry_place_jumps_data;
#define name_entry_place_jumps ((const uint32_t *)name_entry_place_jumps_data)
#define name_entry_letter_state_jumps_ADDRESS 0x00221Cu
extern const void *name_entry_letter_state_jumps_data;
#define name_entry_letter_state_jumps ((const uint32_t *)name_entry_letter_state_jumps_data)
#define name_entry_lcd_state_jumps_ADDRESS 0x002304u
extern const void *name_entry_lcd_state_jumps_data;
#define name_entry_lcd_state_jumps ((const uint32_t *)name_entry_lcd_state_jumps_data)
#define name_entry_car_state_jumps_ADDRESS 0x002384u
extern const void *name_entry_car_state_jumps_data;
#define name_entry_car_state_jumps ((const uint32_t *)name_entry_car_state_jumps_data)
#define name_entry_drivers_state_jumps_ADDRESS 0x0023D4u
extern const void *name_entry_drivers_state_jumps_data;
#define name_entry_drivers_state_jumps ((const uint32_t *)name_entry_drivers_state_jumps_data)
#define name_entry_course_name_state_jumps_ADDRESS 0x002424u
extern const void *name_entry_course_name_state_jumps_data;
#define name_entry_course_name_state_jumps ((const uint32_t *)name_entry_course_name_state_jumps_data)
#define ranking_state_jumps_ADDRESS 0x002488u
extern const void *ranking_state_jumps_data;
#define ranking_state_jumps ((const uint32_t *)ranking_state_jumps_data)
#define ranking_background_state_jumps_ADDRESS 0x002564u
extern const void *ranking_background_state_jumps_data;
#define ranking_background_state_jumps ((const uint32_t *)ranking_background_state_jumps_data)
#define ranking_title_left_state_jumps_ADDRESS 0x002620u
extern const void *ranking_title_left_state_jumps_data;
#define ranking_title_left_state_jumps ((const uint32_t *)ranking_title_left_state_jumps_data)
#define ranking_title_right_state_jumps_ADDRESS 0x00266Cu
extern const void *ranking_title_right_state_jumps_data;
#define ranking_title_right_state_jumps ((const uint32_t *)ranking_title_right_state_jumps_data)
#define ranking_course_banner_state_jumps_ADDRESS 0x0026B8u
extern const void *ranking_course_banner_state_jumps_data;
#define ranking_course_banner_state_jumps ((const uint32_t *)ranking_course_banner_state_jumps_data)
#define ranking_record_character_state_jumps_ADDRESS 0x00279Cu
extern const void *ranking_record_character_state_jumps_data;
#define ranking_record_character_state_jumps ((const uint32_t *)ranking_record_character_state_jumps_data)
#define ranking_record_car_state_jumps_ADDRESS 0x00287Cu
extern const void *ranking_record_car_state_jumps_data;
#define ranking_record_car_state_jumps ((const uint32_t *)ranking_record_car_state_jumps_data)
#define passing_traffic_state_jumps_ADDRESS 0x002960u
extern const void *passing_traffic_state_jumps_data;
#define passing_traffic_state_jumps ((const uint32_t *)passing_traffic_state_jumps_data)
#define passing_car_state_jumps_ADDRESS 0x0029A4u
extern const void *passing_car_state_jumps_data;
#define passing_car_state_jumps ((const uint32_t *)passing_car_state_jumps_data)
#define ranking_background_tile_banks_ADDRESS 0x002C18u
extern const void *ranking_background_tile_banks_data;
#define ranking_background_tile_banks ((const uint8_t *)ranking_background_tile_banks_data)
#define ranking_mixer_addresses_ADDRESS 0x002CA4u
extern const void *ranking_mixer_addresses_data;
#define ranking_mixer_addresses ((const uint32_t *)ranking_mixer_addresses_data)
#define ranking_background_page_numbers_ADDRESS 0x002CACu
extern const void *ranking_background_page_numbers_data;
#define ranking_background_page_numbers ((const uint32_t *)ranking_background_page_numbers_data)
#define ranking_background_scroll_xs_ADDRESS 0x002CB4u
extern const void *ranking_background_scroll_xs_data;
#define ranking_background_scroll_xs ((const uint16_t *)ranking_background_scroll_xs_data)
#define name_entry_letters_ADDRESS 0x002D00u
extern const void *name_entry_letters_data;
#define name_entry_letters ((const uint8_t *)name_entry_letters_data)
#define course_backgrounds_ADDRESS 0x002D20u
extern const void *course_backgrounds_data;
#define course_backgrounds ((const uint8_t *)course_backgrounds_data)
#define ranking_record_character_xs_ADDRESS 0x002D2Au
extern const void *ranking_record_character_xs_data;
#define ranking_record_character_xs ((const uint16_t *)ranking_record_character_xs_data)
#define ranking_record_character_ys_ADDRESS 0x002D40u
extern const void *ranking_record_character_ys_data;
#define ranking_record_character_ys ((const uint16_t *)ranking_record_character_ys_data)
#define name_entry_turn_rates_ADDRESS 0x002D6Cu
extern const void *name_entry_turn_rates_data;
#define name_entry_turn_rates ((const uint8_t *)name_entry_turn_rates_data)
#define name_entry_place_sounds_ADDRESS 0x002D98u
extern const void *name_entry_place_sounds_data;
#define name_entry_place_sounds ((const uint8_t *)name_entry_place_sounds_data)
#define certificate_car_animations_ADDRESS 0x002D9Cu
extern const void *certificate_car_animations_data;
#define certificate_car_animations ((const uint16_t *)certificate_car_animations_data)
#define car_model_palettes_ADDRESS 0x002DACu
extern const void *car_model_palettes_data;
#define car_model_palettes ((const uint16_t *)car_model_palettes_data)
#define certificate_driver_animations_ADDRESS 0x002DBCu
extern const void *certificate_driver_animations_data;
#define certificate_driver_animations ((const uint16_t *)certificate_driver_animations_data)
#define certificate_driver_palettes_ADDRESS 0x002DCCu
extern const void *certificate_driver_palettes_data;
#define certificate_driver_palettes ((const uint16_t *)certificate_driver_palettes_data)
#define car_model_turn_animations_ADDRESS 0x002DDCu
extern const void *car_model_turn_animations_data;
#define car_model_turn_animations ((const uint16_t *)car_model_turn_animations_data)
#define ranking_record_character_palettes_ADDRESS 0x002DECu
extern const void *ranking_record_character_palettes_data;
#define ranking_record_character_palettes ((const uint16_t *)ranking_record_character_palettes_data)
#define ranking_course_banner_animations_ADDRESS 0x002DFCu
extern const void *ranking_course_banner_animations_data;
#define ranking_course_banner_animations ((const uint16_t *)ranking_course_banner_animations_data)
#define ranking_course_banner_palettes_ADDRESS 0x002E10u
extern const void *ranking_course_banner_palettes_data;
#define ranking_course_banner_palettes ((const uint16_t *)ranking_course_banner_palettes_data)
#define certificate_course_animations_ADDRESS 0x002E24u
extern const void *certificate_course_animations_data;
#define certificate_course_animations ((const uint16_t *)certificate_course_animations_data)
#define certificate_course_palettes_ADDRESS 0x002E38u
extern const void *certificate_course_palettes_data;
#define certificate_course_palettes ((const uint16_t *)certificate_course_palettes_data)
#define game_step_jumps_ADDRESS 0x002E80u
extern const void *game_step_jumps_data;
#define game_step_jumps ((const uint32_t *)game_step_jumps_data)
#define game_start_c00074_bytes_ADDRESS 0x003463u
extern const void *game_start_c00074_bytes_data;
#define game_start_c00074_bytes ((const uint8_t *)game_start_c00074_bytes_data)
#define game_end_c00074_bytes_ADDRESS 0x003465u
extern const void *game_end_c00074_bytes_data;
#define game_end_c00074_bytes ((const uint8_t *)game_end_c00074_bytes_data)
#define game_end_c00074_next_bytes_ADDRESS 0x003467u
extern const void *game_end_c00074_next_bytes_data;
#define game_end_c00074_next_bytes ((const uint8_t *)game_end_c00074_next_bytes_data)
#define route_map_state_jumps_ADDRESS 0x003540u
extern const void *route_map_state_jumps_data;
#define route_map_state_jumps ((const uint32_t *)route_map_state_jumps_data)
#define route_map_car_state_jumps_ADDRESS 0x003E28u
extern const void *route_map_car_state_jumps_data;
#define route_map_car_state_jumps ((const uint32_t *)route_map_car_state_jumps_data)
#define route_map_sign_state_jumps_ADDRESS 0x003F34u
extern const void *route_map_sign_state_jumps_data;
#define route_map_sign_state_jumps ((const uint32_t *)route_map_sign_state_jumps_data)
#define route_map_sign_animations_ADDRESS 0x00403Au
extern const void *route_map_sign_animations_data;
#define route_map_sign_animations ((const uint16_t *)route_map_sign_animations_data)
#define route_map_sign_palettes_ADDRESS 0x00403Eu
extern const void *route_map_sign_palettes_data;
#define route_map_sign_palettes ((const uint16_t *)route_map_sign_palettes_data)
#define route_map_goal_xs_ADDRESS 0x004042u
extern const void *route_map_goal_xs_data;
#define route_map_goal_xs ((const int16_t *)route_map_goal_xs_data)
#define route_map_landmark_state_jumps_ADDRESS 0x004064u
extern const void *route_map_landmark_state_jumps_data;
#define route_map_landmark_state_jumps ((const uint32_t *)route_map_landmark_state_jumps_data)
#define route_map_leg_state_jumps_ADDRESS 0x00424Cu
extern const void *route_map_leg_state_jumps_data;
#define route_map_leg_state_jumps ((const uint32_t *)route_map_leg_state_jumps_data)
#define route_map_leg_jumps_ADDRESS 0x004280u
extern const void *route_map_leg_jumps_data;
#define route_map_leg_jumps ((const uint32_t *)route_map_leg_jumps_data)
#define route_map_leg_car_state_jumps_ADDRESS 0x00442Cu
extern const void *route_map_leg_car_state_jumps_data;
#define route_map_leg_car_state_jumps ((const uint32_t *)route_map_leg_car_state_jumps_data)
#define route_map_leg_car_leg_jumps_ADDRESS 0x004474u
extern const void *route_map_leg_car_leg_jumps_data;
#define route_map_leg_car_leg_jumps ((const uint32_t *)route_map_leg_car_leg_jumps_data)
#define route_map_checkpoint_state_jumps_ADDRESS 0x004588u
extern const void *route_map_checkpoint_state_jumps_data;
#define route_map_checkpoint_state_jumps ((const uint32_t *)route_map_checkpoint_state_jumps_data)
#define route_map_finish_x_offsets_ADDRESS 0x00462Cu
extern const void *route_map_finish_x_offsets_data;
#define route_map_finish_x_offsets ((const uint16_t *)route_map_finish_x_offsets_data)
#define route_map_finish_y_offsets_ADDRESS 0x004634u
extern const void *route_map_finish_y_offsets_data;
#define route_map_finish_y_offsets ((const uint16_t *)route_map_finish_y_offsets_data)
#define route_map_direction_sign_state_jumps_ADDRESS 0x004648u
extern const void *route_map_direction_sign_state_jumps_data;
#define route_map_direction_sign_state_jumps ((const uint32_t *)route_map_direction_sign_state_jumps_data)
#define route_map_wait_banner_state_jumps_ADDRESS 0x0046C8u
extern const void *route_map_wait_banner_state_jumps_data;
#define route_map_wait_banner_state_jumps ((const uint32_t *)route_map_wait_banner_state_jumps_data)
#define game_over_banner_state_jumps_ADDRESS 0x00477Cu
extern const void *game_over_banner_state_jumps_data;
#define game_over_banner_state_jumps ((const uint32_t *)game_over_banner_state_jumps_data)
#define route_map_close_up_pages_ADDRESS 0x0048B8u
extern const void *route_map_close_up_pages_data;
#define route_map_close_up_pages ((const uint32_t *)route_map_close_up_pages_data)
#define route_map_routes_pages_ADDRESS 0x0048C0u
extern const void *route_map_routes_pages_data;
#define route_map_routes_pages ((const uint32_t *)route_map_routes_pages_data)
#define route_map_world_pages_ADDRESS 0x0048C8u
extern const void *route_map_world_pages_data;
#define route_map_world_pages ((const uint32_t *)route_map_world_pages_data)
#define route_map_start_scroll_xs_ADDRESS 0x0048D0u
extern const void *route_map_start_scroll_xs_data;
#define route_map_start_scroll_xs ((const uint16_t *)route_map_start_scroll_xs_data)
#define route_map_whole_scroll_xs_ADDRESS 0x0048D4u
extern const void *route_map_whole_scroll_xs_data;
#define route_map_whole_scroll_xs ((const uint16_t *)route_map_whole_scroll_xs_data)
#define route_map_sea_colour_addresses_ADDRESS 0x0048D8u
extern const void *route_map_sea_colour_addresses_data;
#define route_map_sea_colour_addresses ((const uint32_t *)route_map_sea_colour_addresses_data)
#define route_map_car_animations_ADDRESS 0x0048E0u
extern const void *route_map_car_animations_data;
#define route_map_car_animations ((const uint16_t *)route_map_car_animations_data)
#define route_map_car_palettes_ADDRESS 0x0048F0u
extern const void *route_map_car_palettes_data;
#define route_map_car_palettes ((const uint16_t *)route_map_car_palettes_data)
#define route_map_stage_xs_ADDRESS 0x004900u
extern const void *route_map_stage_xs_data;
#define route_map_stage_xs ((const uint16_t *)route_map_stage_xs_data)
#define route_map_stage_ys_ADDRESS 0x00492Au
extern const void *route_map_stage_ys_data;
#define route_map_stage_ys ((const uint16_t *)route_map_stage_ys_data)
#define route_map_direction_x_shifts_ADDRESS 0x004954u
extern const void *route_map_direction_x_shifts_data;
#define route_map_direction_x_shifts ((const uint16_t *)route_map_direction_x_shifts_data)
#define route_map_first_fork_leg_animations_ADDRESS 0x004958u
extern const void *route_map_first_fork_leg_animations_data;
#define route_map_first_fork_leg_animations ((const uint16_t *)route_map_first_fork_leg_animations_data)
#define route_map_fork_leg_animations_ADDRESS 0x00495Cu
extern const void *route_map_fork_leg_animations_data;
#define route_map_fork_leg_animations ((const uint16_t *)route_map_fork_leg_animations_data)
#define car_select_state_jumps_ADDRESS 0x005058u
extern const void *car_select_state_jumps_data;
#define car_select_state_jumps ((const uint32_t *)car_select_state_jumps_data)
#define car_select_choice_turn_jumps_ADDRESS 0x0053ECu
extern const void *car_select_choice_turn_jumps_data;
#define car_select_choice_turn_jumps ((const uint32_t *)car_select_choice_turn_jumps_data)
#define car_select_rowscroll_copies_jumps_ADDRESS 0x00548Cu
extern const void *car_select_rowscroll_copies_jumps_data;
#define car_select_rowscroll_copies_jumps ((const uint32_t *)car_select_rowscroll_copies_jumps_data)
#define car_attendant_state_jumps_ADDRESS 0x005534u
extern const void *car_attendant_state_jumps_data;
#define car_attendant_state_jumps ((const uint32_t *)car_attendant_state_jumps_data)
#define turntable_car_state_jumps_ADDRESS 0x0056C0u
extern const void *turntable_car_state_jumps_data;
#define turntable_car_state_jumps ((const uint32_t *)turntable_car_state_jumps_data)
#define turntable_scenery_state_jumps_ADDRESS 0x005770u
extern const void *turntable_scenery_state_jumps_data;
#define turntable_scenery_state_jumps ((const uint32_t *)turntable_scenery_state_jumps_data)
#define welcome_letter_state_jumps_ADDRESS 0x005890u
extern const void *welcome_letter_state_jumps_data;
#define welcome_letter_state_jumps ((const uint32_t *)welcome_letter_state_jumps_data)
#define select_car_heading_state_jumps_ADDRESS 0x005914u
extern const void *select_car_heading_state_jumps_data;
#define select_car_heading_state_jumps ((const uint32_t *)select_car_heading_state_jumps_data)
#define select_instruction_state_jumps_ADDRESS 0x0059C4u
extern const void *select_instruction_state_jumps_data;
#define select_instruction_state_jumps ((const uint32_t *)select_instruction_state_jumps_data)
#define transmission_choice_state_jumps_ADDRESS 0x005A28u
extern const void *transmission_choice_state_jumps_data;
#define transmission_choice_state_jumps ((const uint32_t *)transmission_choice_state_jumps_data)
#define automatic_panel_state_jumps_ADDRESS 0x005B50u
extern const void *automatic_panel_state_jumps_data;
#define automatic_panel_state_jumps ((const uint32_t *)automatic_panel_state_jumps_data)
#define manual_panel_state_jumps_ADDRESS 0x005BE4u
extern const void *manual_panel_state_jumps_data;
#define manual_panel_state_jumps ((const uint32_t *)manual_panel_state_jumps_data)
#define car_caption_state_jumps_ADDRESS 0x005D68u
extern const void *car_caption_state_jumps_data;
#define car_caption_state_jumps ((const uint32_t *)car_caption_state_jumps_data)
#define car_gears_caption_state_jumps_ADDRESS 0x005E1Cu
extern const void *car_gears_caption_state_jumps_data;
#define car_gears_caption_state_jumps ((const uint32_t *)car_gears_caption_state_jumps_data)
#define waiting_banner_state_jumps_ADDRESS 0x005EC0u
extern const void *waiting_banner_state_jumps_data;
#define waiting_banner_state_jumps ((const uint32_t *)waiting_banner_state_jumps_data)
#define car_select_entries_state_jumps_ADDRESS 0x005F5Cu
extern const void *car_select_entries_state_jumps_data;
#define car_select_entries_state_jumps ((const uint32_t *)car_select_entries_state_jumps_data)
#define car_select_gas_down_jumps_ADDRESS 0x0062B0u
extern const void *car_select_gas_down_jumps_data;
#define car_select_gas_down_jumps ((const uint32_t *)car_select_gas_down_jumps_data)
#define entry_countdown_lcd_texts_ADDRESS 0x006464u
extern const void *entry_countdown_lcd_texts_data;
#define entry_countdown_lcd_texts ((const uint32_t *)entry_countdown_lcd_texts_data)
#define entry_traffic_state_jumps_ADDRESS 0x00661Cu
extern const void *entry_traffic_state_jumps_data;
#define entry_traffic_state_jumps ((const uint32_t *)entry_traffic_state_jumps_data)
#define entry_traffic_car_state_jumps_ADDRESS 0x00665Cu
extern const void *entry_traffic_car_state_jumps_data;
#define entry_traffic_car_state_jumps ((const uint32_t *)entry_traffic_car_state_jumps_data)
#define car_pass_sounds_ADDRESS 0x0067A9u
extern const void *car_pass_sounds_data;
#define car_pass_sounds ((const uint8_t *)car_pass_sounds_data)
#define car_select_default_cars_ADDRESS 0x0069B0u
extern const void *car_select_default_cars_data;
#define car_select_default_cars ((const uint8_t *)car_select_default_cars_data)
#define turntable_car_frames_ADDRESS 0x0069D8u
extern const void *turntable_car_frames_data;
#define turntable_car_frames ((const uint8_t *)turntable_car_frames_data)
#define car_select_turn_delays_ADDRESS 0x006D9Cu
extern const void *car_select_turn_delays_data;
#define car_select_turn_delays ((const uint8_t *)car_select_turn_delays_data)
#define car_select_wheel_transmissions_ADDRESS 0x006DA4u
extern const void *car_select_wheel_transmissions_data;
#define car_select_wheel_transmissions ((const uint8_t *)car_select_wheel_transmissions_data)
#define car_caption_palettes_ADDRESS 0x006DDEu
extern const void *car_caption_palettes_data;
#define car_caption_palettes ((const uint16_t *)car_caption_palettes_data)
#define car_select_car_sounds_ADDRESS 0x006DEEu
extern const void *car_select_car_sounds_data;
#define car_select_car_sounds ((const uint8_t *)car_select_car_sounds_data)
#define automatic_panel_palette_lists_ADDRESS 0x006E30u
extern const void *automatic_panel_palette_lists_data;
#define automatic_panel_palette_lists ((const uint32_t *)automatic_panel_palette_lists_data)
#define manual_panel_palette_lists_ADDRESS 0x006E44u
extern const void *manual_panel_palette_lists_data;
#define manual_panel_palette_lists ((const uint32_t *)manual_panel_palette_lists_data)
#define car_gears_caption_frames_ADDRESS 0x006E78u
extern const void *car_gears_caption_frames_data;
#define car_gears_caption_frames ((const uint8_t *)car_gears_caption_frames_data)
#define choice_flag_palettes_ADDRESS 0x006E80u
extern const void *choice_flag_palettes_data;
#define choice_flag_palettes ((const uint16_t *)choice_flag_palettes_data)
#define choice_flag_number_animations_ADDRESS 0x006E90u
extern const void *choice_flag_number_animations_data;
#define choice_flag_number_animations ((const uint16_t *)choice_flag_number_animations_data)
#define car_select_entries_flag_blink_ADDRESS 0x006EA0u
extern const void *car_select_entries_flag_blink_data;
#define car_select_entries_flag_blink ((const uint8_t *)car_select_entries_flag_blink_data)
#define unused_numbers_006EC8_ADDRESS 0x006EC8u
extern const void *unused_numbers_006EC8_data;
#define unused_numbers_006EC8 (*(const uint32_t *)unused_numbers_006EC8_data)
#define player_car_state_jumps_ADDRESS 0x006F18u
extern const void *player_car_state_jumps_data;
#define player_car_state_jumps ((const uint32_t *)player_car_state_jumps_data)
#define player_car_time_up_step_jumps_ADDRESS 0x007164u
extern const void *player_car_time_up_step_jumps_data;
#define player_car_time_up_step_jumps ((const uint32_t *)player_car_time_up_step_jumps_data)
#define player_car_goal_step_jumps_ADDRESS 0x00720Cu
extern const void *player_car_goal_step_jumps_data;
#define player_car_goal_step_jumps ((const uint32_t *)player_car_goal_step_jumps_data)
#define player_car_rival_step_jumps_ADDRESS 0x007917u
extern const void *player_car_rival_step_jumps_data;
#define player_car_rival_step_jumps ((const uint32_t *)player_car_rival_step_jumps_data)
#define rival_warning_state_jumps_ADDRESS 0x007C44u
extern const void *rival_warning_state_jumps_data;
#define rival_warning_state_jumps ((const uint32_t *)rival_warning_state_jumps_data)
#define rival_alarm_state_jumps_ADDRESS 0x007CACu
extern const void *rival_alarm_state_jumps_data;
#define rival_alarm_state_jumps ((const uint32_t *)rival_alarm_state_jumps_data)
#define player_car_hit_kind_jumps_ADDRESS 0x007D28u
extern const void *player_car_hit_kind_jumps_data;
#define player_car_hit_kind_jumps ((const uint32_t *)player_car_hit_kind_jumps_data)
#define crash_place_offsets_ADDRESS 0x008026u
extern const void *crash_place_offsets_data;
#define crash_place_offsets ((const uint16_t *)crash_place_offsets_data)
#define car_weights_ADDRESS 0x008038u
extern const void *car_weights_data;
#define car_weights ((const uint16_t *)car_weights_data)
#define car_crash_physics_ADDRESS 0x00804Au
extern const void *car_crash_physics_data;
#define car_crash_physics ((const uint32_t *)car_crash_physics_data)
#define grid_road_place_lists_ADDRESS 0x0090ACu
extern const void *grid_road_place_lists_data;
#define grid_road_place_lists ((const uint32_t *)grid_road_place_lists_data)
#define grid_segment_lists_ADDRESS 0x0091CCu
extern const void *grid_segment_lists_data;
#define grid_segment_lists ((const uint32_t *)grid_segment_lists_data)
#define player_car_steer_animations_ADDRESS 0x00926Cu
extern const void *player_car_steer_animations_data;
#define player_car_steer_animations ((const uint16_t *)player_car_steer_animations_data)
#define steer_frames_by_steering_ADDRESS 0x00927Cu
extern const void *steer_frames_by_steering_data;
#define steer_frames_by_steering ((const uint8_t *)steer_frames_by_steering_data)
#define player_car_wheelspin_animations_ADDRESS 0x009300u
extern const void *player_car_wheelspin_animations_data;
#define player_car_wheelspin_animations ((const uint16_t *)player_car_wheelspin_animations_data)
#define player_car_skid_animations_wheel_high_ADDRESS 0x009310u
extern const void *player_car_skid_animations_wheel_high_data;
#define player_car_skid_animations_wheel_high ((const uint16_t *)player_car_skid_animations_wheel_high_data)
#define player_car_skid_animations_wheel_low_ADDRESS 0x009410u
extern const void *player_car_skid_animations_wheel_low_data;
#define player_car_skid_animations_wheel_low ((const uint16_t *)player_car_skid_animations_wheel_low_data)
#define player_car_palettes_ADDRESS 0x009510u
extern const void *player_car_palettes_data;
#define player_car_palettes ((const uint16_t *)player_car_palettes_data)
#define player_car_arrival_animations_ADDRESS 0x009520u
extern const void *player_car_arrival_animations_data;
#define player_car_arrival_animations ((const uint16_t *)player_car_arrival_animations_data)
#define player_car_time_up_brakes_ADDRESS 0x009530u
extern const void *player_car_time_up_brakes_data;
#define player_car_time_up_brakes ((const uint8_t *)player_car_time_up_brakes_data)
#define goal_stops_ADDRESS 0x009540u
extern const void *goal_stops_data;
#define goal_stops ((const uint32_t *)goal_stops_data)
#define race_car_first_body_pieces_ADDRESS 0x009DCCu
extern const void *race_car_first_body_pieces_data;
#define race_car_first_body_pieces ((const uint16_t *)race_car_first_body_pieces_data)
#define race_car_first_body_animations_ADDRESS 0x009DDCu
extern const void *race_car_first_body_animations_data;
#define race_car_first_body_animations ((const uint16_t *)race_car_first_body_animations_data)
#define race_car_driver_animations_ADDRESS 0x009DECu
extern const void *race_car_driver_animations_data;
#define race_car_driver_animations ((const uint16_t *)race_car_driver_animations_data)
#define race_car_passenger_animations_ADDRESS 0x009DFCu
extern const void *race_car_passenger_animations_data;
#define race_car_passenger_animations ((const uint16_t *)race_car_passenger_animations_data)
#define race_car_driver_gesture_masks_ADDRESS 0x009E0Cu
extern const void *race_car_driver_gesture_masks_data;
#define race_car_driver_gesture_masks ((const uint8_t *)race_car_driver_gesture_masks_data)
#define race_car_passenger_gesture_masks_ADDRESS 0x009E14u
extern const void *race_car_passenger_gesture_masks_data;
#define race_car_passenger_gesture_masks ((const uint8_t *)race_car_passenger_gesture_masks_data)
#define race_car_driver_palettes_ADDRESS 0x009E1Cu
extern const void *race_car_driver_palettes_data;
#define race_car_driver_palettes ((const uint16_t *)race_car_driver_palettes_data)
#define race_car_passenger_palettes_ADDRESS 0x009E2Cu
extern const void *race_car_passenger_palettes_data;
#define race_car_passenger_palettes ((const uint16_t *)race_car_passenger_palettes_data)
#define race_car_driver_frame_counts_ADDRESS 0x009E3Cu
extern const void *race_car_driver_frame_counts_data;
#define race_car_driver_frame_counts ((const uint8_t *)race_car_driver_frame_counts_data)
#define race_car_passenger_frame_counts_ADDRESS 0x009E44u
extern const void *race_car_passenger_frame_counts_data;
#define race_car_passenger_frame_counts ((const uint8_t *)race_car_passenger_frame_counts_data)
#define race_car_body_wheel_kinds_ADDRESS 0x009E4Cu
extern const void *race_car_body_wheel_kinds_data;
#define race_car_body_wheel_kinds ((const uint8_t *)race_car_body_wheel_kinds_data)
#define race_car_driver_handlers_ADDRESS 0x009EBCu
extern const void *race_car_driver_handlers_data;
#define race_car_driver_handlers ((const uint32_t *)race_car_driver_handlers_data)
#define race_car_passenger_handlers_ADDRESS 0x009EDCu
extern const void *race_car_passenger_handlers_data;
#define race_car_passenger_handlers ((const uint32_t *)race_car_passenger_handlers_data)
#define unused_car_animations_ADDRESS 0x009EFCu
extern const void *unused_car_animations_data;
#define unused_car_animations (*(const uint16_t *)unused_car_animations_data)
#define race_car_spray_palettes_ADDRESS 0x009F0Cu
extern const void *race_car_spray_palettes_data;
#define race_car_spray_palettes ((const uint16_t *)race_car_spray_palettes_data)
#define race_car_spray_2_animations_ADDRESS 0x009F2Cu
extern const void *race_car_spray_2_animations_data;
#define race_car_spray_2_animations (*(const uint16_t *)race_car_spray_2_animations_data)
#define race_car_spray_1_animations_ADDRESS 0x009F4Cu
extern const void *race_car_spray_1_animations_data;
#define race_car_spray_1_animations (*(const uint16_t *)race_car_spray_1_animations_data)
#define race_car_spray_frame_counts_ADDRESS 0x009F6Cu
extern const void *race_car_spray_frame_counts_data;
#define race_car_spray_frame_counts ((const uint8_t *)race_car_spray_frame_counts_data)
#define race_car_tag_palettes_ADDRESS 0x009F7Cu
extern const void *race_car_tag_palettes_data;
#define race_car_tag_palettes ((const uint16_t *)race_car_tag_palettes_data)
#define race_car_tag_animations_ADDRESS 0x009F8Cu
extern const void *race_car_tag_animations_data;
#define race_car_tag_animations ((const uint16_t *)race_car_tag_animations_data)
#define race_car_blink_starts_ADDRESS 0x009F9Cu
extern const void *race_car_blink_starts_data;
#define race_car_blink_starts ((const uint8_t *)race_car_blink_starts_data)
#define race_car_tag_blink_ADDRESS 0x009FA4u
extern const void *race_car_tag_blink_data;
#define race_car_tag_blink ((const uint8_t *)race_car_tag_blink_data)
#define race_car_ending_handlers_ADDRESS 0x009FC4u
extern const void *race_car_ending_handlers_data;
#define race_car_ending_handlers ((const uint32_t *)race_car_ending_handlers_data)
#define unused_numbers_009FE4_ADDRESS 0x009FE4u
extern const void *unused_numbers_009FE4_data;
#define unused_numbers_009FE4 (*(const uint32_t *)unused_numbers_009FE4_data)
#define player_car_heading_quadrant_jumps_ADDRESS 0x00A5F4u
extern const void *player_car_heading_quadrant_jumps_data;
#define player_car_heading_quadrant_jumps ((const uint32_t *)player_car_heading_quadrant_jumps_data)
#define player_car_view_yaw_quadrant_jumps_ADDRESS 0x00A798u
extern const void *player_car_view_yaw_quadrant_jumps_data;
#define player_car_view_yaw_quadrant_jumps ((const uint32_t *)player_car_view_yaw_quadrant_jumps_data)
#define car_skid_noises_ADDRESS 0x00ABC3u
extern const void *car_skid_noises_data;
#define car_skid_noises ((const uint8_t *)car_skid_noises_data)
#define gas_pedal_curve_ADDRESS 0x00ABCBu
extern const void *gas_pedal_curve_data;
#define gas_pedal_curve ((const int8_t *)gas_pedal_curve_data)
#define brake_pedal_curve_ADDRESS 0x00ACCBu
extern const void *brake_pedal_curve_data;
#define brake_pedal_curve ((const uint8_t *)brake_pedal_curve_data)
#define car_handlings_ADDRESS 0x00ADCCu
extern const void *car_handlings_data;
#define car_handlings ((const uint32_t *)car_handlings_data)
#define car_gear_lists_ADDRESS 0x00B04Cu
extern const void *car_gear_lists_data;
#define car_gear_lists ((const uint32_t *)car_gear_lists_data)
#define car_torque_curves_ADDRESS 0x00B0C0u
extern const void *car_torque_curves_data;
#define car_torque_curves ((const uint32_t *)car_torque_curves_data)
#define car_model_off_road_noises_ADDRESS 0x00B6FCu
extern const void *car_model_off_road_noises_data;
#define car_model_off_road_noises ((const uint8_t *)car_model_off_road_noises_data)
#define car_model_off_road_shakes_ADDRESS 0x00B704u
extern const void *car_model_off_road_shakes_data;
#define car_model_off_road_shakes ((const uint8_t *)car_model_off_road_shakes_data)
#define wheel_effect_1_jumps_ADDRESS 0x00B70Cu
extern const void *wheel_effect_1_jumps_data;
#define wheel_effect_1_jumps ((const uint32_t *)wheel_effect_1_jumps_data)
#define wheel_effect_2_jumps_ADDRESS 0x00B74Cu
extern const void *wheel_effect_2_jumps_data;
#define wheel_effect_2_jumps ((const uint32_t *)wheel_effect_2_jumps_data)
#define rev_meter_state_jumps_ADDRESS 0x00BFCCu
extern const void *rev_meter_state_jumps_data;
#define rev_meter_state_jumps ((const uint32_t *)rev_meter_state_jumps_data)
#define speedometer_state_jumps_ADDRESS 0x00C024u
extern const void *speedometer_state_jumps_data;
#define speedometer_state_jumps ((const uint32_t *)speedometer_state_jumps_data)
#define speed_unit_state_jumps_ADDRESS 0x00C0D4u
extern const void *speed_unit_state_jumps_data;
#define speed_unit_state_jumps ((const uint32_t *)speed_unit_state_jumps_data)
#define gear_indicator_state_jumps_ADDRESS 0x00C118u
extern const void *gear_indicator_state_jumps_data;
#define gear_indicator_state_jumps ((const uint32_t *)gear_indicator_state_jumps_data)
#define shift_up_hint_state_jumps_ADDRESS 0x00C23Cu
extern const void *shift_up_hint_state_jumps_data;
#define shift_up_hint_state_jumps ((const uint32_t *)shift_up_hint_state_jumps_data)
#define place_count_state_jumps_ADDRESS 0x00C2B8u
extern const void *place_count_state_jumps_data;
#define place_count_state_jumps ((const uint32_t *)place_count_state_jumps_data)
#define place_number_state_jumps_ADDRESS 0x00C350u
extern const void *place_number_state_jumps_data;
#define place_number_state_jumps ((const uint32_t *)place_number_state_jumps_data)
#define lap_timer_state_jumps_ADDRESS 0x00C44Cu
extern const void *lap_timer_state_jumps_data;
#define lap_timer_state_jumps ((const uint32_t *)lap_timer_state_jumps_data)
#define start_message_state_jumps_ADDRESS 0x00C5A8u
extern const void *start_message_state_jumps_data;
#define start_message_state_jumps ((const uint32_t *)start_message_state_jumps_data)
#define time_left_display_state_jumps_ADDRESS 0x00C66Cu
extern const void *time_left_display_state_jumps_data;
#define time_left_display_state_jumps ((const uint32_t *)time_left_display_state_jumps_data)
#define stage_entry_state_jumps_ADDRESS 0x00C880u
extern const void *stage_entry_state_jumps_data;
#define stage_entry_state_jumps ((const uint32_t *)stage_entry_state_jumps_data)
#define goal_state_jumps_ADDRESS 0x00C8C4u
extern const void *goal_state_jumps_data;
#define goal_state_jumps ((const uint32_t *)goal_state_jumps_data)
#define branch_arrow_state_jumps_ADDRESS 0x00CAC8u
extern const void *branch_arrow_state_jumps_data;
#define branch_arrow_state_jumps ((const uint32_t *)branch_arrow_state_jumps_data)
#define checkpoint_board_state_jumps_ADDRESS 0x00CBCCu
extern const void *checkpoint_board_state_jumps_data;
#define checkpoint_board_state_jumps ((const uint32_t *)checkpoint_board_state_jumps_data)
#define checkpoint_board_row_state_jumps_ADDRESS 0x00CC70u
extern const void *checkpoint_board_row_state_jumps_data;
#define checkpoint_board_row_state_jumps ((const uint32_t *)checkpoint_board_row_state_jumps_data)
#define checkpoint_board_car_sprites_ADDRESS 0x00CFC8u
extern const void *checkpoint_board_car_sprites_data;
#define checkpoint_board_car_sprites ((const uint32_t *)checkpoint_board_car_sprites_data)
#define checkpoint_board_number_animations_ADDRESS 0x00CFE8u
extern const void *checkpoint_board_number_animations_data;
#define checkpoint_board_number_animations ((const uint16_t *)checkpoint_board_number_animations_data)
#define checkpoint_board_time_places_ADDRESS 0x00CFF8u
extern const void *checkpoint_board_time_places_data;
#define checkpoint_board_time_places ((const uint32_t *)checkpoint_board_time_places_data)
#define gear_indicator_animations_ADDRESS 0x00D018u
extern const void *gear_indicator_animations_data;
#define gear_indicator_animations ((const uint16_t *)gear_indicator_animations_data)
#define stage_level_texts_ADDRESS 0x00D034u
extern const void *stage_level_texts_data;
#define stage_level_texts ((const uint32_t *)stage_level_texts_data)
#define lap_timer_checkpoint_states_ADDRESS 0x00D0C0u
extern const void *lap_timer_checkpoint_states_data;
#define lap_timer_checkpoint_states ((const uint8_t *)lap_timer_checkpoint_states_data)
#define entry_invitation_step_jumps_ADDRESS 0x00DDB4u
extern const void *entry_invitation_step_jumps_data;
#define entry_invitation_step_jumps ((const uint32_t *)entry_invitation_step_jumps_data)
#define race_copy_offsets_ADDRESS 0x00E0A0u
extern const void *race_copy_offsets_data;
#define race_copy_offsets ((const uint32_t *)race_copy_offsets_data)
#define race_copy_player_offsets_ADDRESS 0x00E0C4u
extern const void *race_copy_player_offsets_data;
#define race_copy_player_offsets ((const uint32_t *)race_copy_player_offsets_data)
#define link_board_ic_texts_ADDRESS 0x00E4D0u
extern const void *link_board_ic_texts_data;
#define link_board_ic_texts ((const uint32_t *)link_board_ic_texts_data)
#define stage_start_times_ADDRESS 0x00E5B3u
extern const void *stage_start_times_data;
#define stage_start_times ((const uint8_t *)stage_start_times_data)
#define fork_next_stages_ADDRESS 0x00E5D3u
extern const void *fork_next_stages_data;
#define fork_next_stages ((const uint8_t *)fork_next_stages_data)
#define entry_frames_by_machines_ADDRESS 0x00E614u
extern const void *entry_frames_by_machines_data;
#define entry_frames_by_machines ((const uint16_t *)entry_frames_by_machines_data)
#define draw_object_mode_jumps_ADDRESS 0x00EB44u
extern const void *draw_object_mode_jumps_data;
#define draw_object_mode_jumps ((const uint32_t *)draw_object_mode_jumps_data)
#define sprite_flip_x_signs_ADDRESS 0x00F13Cu
extern const void *sprite_flip_x_signs_data;
#define sprite_flip_x_signs ((const uint32_t *)sprite_flip_x_signs_data)
#define sprite_flip_y_signs_ADDRESS 0x00F140u
extern const void *sprite_flip_y_signs_data;
#define sprite_flip_y_signs ((const uint32_t *)sprite_flip_y_signs_data)
#define animation_order_jumps_ADDRESS 0x00F34Cu
extern const void *animation_order_jumps_data;
#define animation_order_jumps ((const uint32_t *)animation_order_jumps_data)
#define smoothed_flip_x_signs_ADDRESS 0x00F4E8u
extern const void *smoothed_flip_x_signs_data;
#define smoothed_flip_x_signs (*(const uint32_t *)smoothed_flip_x_signs_data)
#define smoothed_flip_signs_ADDRESS 0x00F4ECu
extern const void *smoothed_flip_signs_data;
#define smoothed_flip_signs ((const uint32_t *)smoothed_flip_signs_data)
#define course_command_jumps_ADDRESS 0x00FF40u
extern const void *course_command_jumps_data;
#define course_command_jumps ((const uint32_t *)course_command_jumps_data)
#define unused_segment_steps_ADDRESS 0x010309u
extern const void *unused_segment_steps_data;
#define unused_segment_steps (*(const int8_t *)unused_segment_steps_data)
#define unused_segment_shifts_ADDRESS 0x010425u
extern const void *unused_segment_shifts_data;
#define unused_segment_shifts (*(const int8_t *)unused_segment_shifts_data)
#define unused_numbers_01060C_ADDRESS 0x01060Cu
extern const void *unused_numbers_01060C_data;
#define unused_numbers_01060C (*(const uint32_t *)unused_numbers_01060C_data)
#define sprite_control_mode_ADDRESS 0x010B0Au
extern const void *sprite_control_mode_data;
#define sprite_control_mode (*(const uint8_t *)sprite_control_mode_data)
#define sprite_control_flip_ADDRESS 0x010B0Bu
extern const void *sprite_control_flip_data;
#define sprite_control_flip (*(const uint8_t *)sprite_control_flip_data)
#define video_control_default_ADDRESS 0x010B10u
extern const void *video_control_default_data;
#define video_control_default (*(const uint16_t *)video_control_default_data)
#define model_view_pitch_bases_ADDRESS 0x010BD2u
extern const void *model_view_pitch_bases_data;
#define model_view_pitch_bases ((const uint16_t *)model_view_pitch_bases_data)
#define screen_fade_kind_jumps_ADDRESS 0x01168Cu
extern const void *screen_fade_kind_jumps_data;
#define screen_fade_kind_jumps ((const uint32_t *)screen_fade_kind_jumps_data)
#define music_fade_addresses_ADDRESS 0x011FFCu
extern const void *music_fade_addresses_data;
#define music_fade_addresses ((const uint32_t *)music_fade_addresses_data)
#define car_model_engine_samples_ADDRESS 0x012026u
extern const void *car_model_engine_samples_data;
#define car_model_engine_samples ((const uint8_t *)car_model_engine_samples_data)
#define demo_sounds_ADDRESS 0x01202Eu
extern const void *demo_sounds_data;
#define demo_sounds ((const uint32_t *)demo_sounds_data)
#define demo_second_sounds_ADDRESS 0x01204Eu
extern const void *demo_second_sounds_data;
#define demo_second_sounds ((const uint32_t *)demo_second_sounds_data)
#define octant_base_angles_ADDRESS 0x0121D8u
extern const void *octant_base_angles_data;
#define octant_base_angles ((const uint16_t *)octant_base_angles_data)
#define octant_angle_factors_ADDRESS 0x0121E8u
extern const void *octant_angle_factors_data;
#define octant_angle_factors ((const uint16_t *)octant_angle_factors_data)
#define road_object_stream_state_jumps_ADDRESS 0x012708u
extern const void *road_object_stream_state_jumps_data;
#define road_object_stream_state_jumps ((const uint32_t *)road_object_stream_state_jumps_data)
#define road_object_stream_command_jumps_ADDRESS 0x012748u
extern const void *road_object_stream_command_jumps_data;
#define road_object_stream_command_jumps ((const uint32_t *)road_object_stream_command_jumps_data)
#define crest_clip_state_jumps_ADDRESS 0x0127FCu
extern const void *crest_clip_state_jumps_data;
#define crest_clip_state_jumps ((const uint32_t *)crest_clip_state_jumps_data)
#define scenery_state_jumps_ADDRESS 0x012888u
extern const void *scenery_state_jumps_data;
#define scenery_state_jumps ((const uint32_t *)scenery_state_jumps_data)
#define flashing_scenery_state_jumps_ADDRESS 0x012914u
extern const void *flashing_scenery_state_jumps_data;
#define flashing_scenery_state_jumps ((const uint32_t *)flashing_scenery_state_jumps_data)
#define scenery_pair_state_jumps_ADDRESS 0x0129B8u
extern const void *scenery_pair_state_jumps_data;
#define scenery_pair_state_jumps ((const uint32_t *)scenery_pair_state_jumps_data)
#define unused_0129D0_state_jumps_ADDRESS 0x0129DCu
extern const void *unused_0129D0_state_jumps_data;
#define unused_0129D0_state_jumps (*(const uint32_t *)unused_0129D0_state_jumps_data)
#define overhead_scenery_state_jumps_ADDRESS 0x012A00u
extern const void *overhead_scenery_state_jumps_data;
#define overhead_scenery_state_jumps ((const uint32_t *)overhead_scenery_state_jumps_data)
#define aurora_state_jumps_ADDRESS 0x012A2Cu
extern const void *aurora_state_jumps_data;
#define aurora_state_jumps ((const uint32_t *)aurora_state_jumps_data)
#define fork_sign_state_jumps_ADDRESS 0x012ADCu
extern const void *fork_sign_state_jumps_data;
#define fork_sign_state_jumps ((const uint32_t *)fork_sign_state_jumps_data)
#define animated_scenery_state_jumps_ADDRESS 0x012B00u
extern const void *animated_scenery_state_jumps_data;
#define animated_scenery_state_jumps ((const uint32_t *)animated_scenery_state_jumps_data)
#define animated_scenery_pair_state_jumps_ADDRESS 0x012B90u
extern const void *animated_scenery_pair_state_jumps_data;
#define animated_scenery_pair_state_jumps ((const uint32_t *)animated_scenery_pair_state_jumps_data)
#define road_crosser_state_jumps_ADDRESS 0x012BBCu
extern const void *road_crosser_state_jumps_data;
#define road_crosser_state_jumps ((const uint32_t *)road_crosser_state_jumps_data)
#define kangaroo_state_jumps_ADDRESS 0x012CB8u
extern const void *kangaroo_state_jumps_data;
#define kangaroo_state_jumps ((const uint32_t *)kangaroo_state_jumps_data)
#define moving_scenery_state_jumps_ADDRESS 0x012DC4u
extern const void *moving_scenery_state_jumps_data;
#define moving_scenery_state_jumps ((const uint32_t *)moving_scenery_state_jumps_data)
#define swooping_helicopter_state_jumps_ADDRESS 0x012EB0u
extern const void *swooping_helicopter_state_jumps_data;
#define swooping_helicopter_state_jumps ((const uint32_t *)swooping_helicopter_state_jumps_data)
#define start_gate_state_jumps_ADDRESS 0x012FC8u
extern const void *start_gate_state_jumps_data;
#define start_gate_state_jumps ((const uint32_t *)start_gate_state_jumps_data)
#define start_gate_light_palettes_ADDRESS 0x01306Eu
extern const void *start_gate_light_palettes_data;
#define start_gate_light_palettes ((const uint16_t *)start_gate_light_palettes_data)
#define start_flagman_state_jumps_ADDRESS 0x01308Cu
extern const void *start_flagman_state_jumps_data;
#define start_flagman_state_jumps ((const uint32_t *)start_flagman_state_jumps_data)
#define flagman_state_jumps_ADDRESS 0x013110u
extern const void *flagman_state_jumps_data;
#define flagman_state_jumps ((const uint32_t *)flagman_state_jumps_data)
#define start_balloon_state_jumps_ADDRESS 0x0131B0u
extern const void *start_balloon_state_jumps_data;
#define start_balloon_state_jumps ((const uint32_t *)start_balloon_state_jumps_data)
#define ambient_sound_state_jumps_ADDRESS 0x013238u
extern const void *ambient_sound_state_jumps_data;
#define ambient_sound_state_jumps ((const uint32_t *)ambient_sound_state_jumps_data)
#define west_east_panel_state_jumps_ADDRESS 0x0132A4u
extern const void *west_east_panel_state_jumps_data;
#define west_east_panel_state_jumps ((const uint32_t *)west_east_panel_state_jumps_data)
#define santa_sleigh_state_jumps_ADDRESS 0x013348u
extern const void *santa_sleigh_state_jumps_data;
#define santa_sleigh_state_jumps ((const uint32_t *)santa_sleigh_state_jumps_data)
#define goal_parade_rightward_state_jumps_ADDRESS 0x01345Cu
extern const void *goal_parade_rightward_state_jumps_data;
#define goal_parade_rightward_state_jumps ((const uint32_t *)goal_parade_rightward_state_jumps_data)
#define sounding_scenery_state_jumps_ADDRESS 0x01352Cu
extern const void *sounding_scenery_state_jumps_data;
#define sounding_scenery_state_jumps ((const uint32_t *)sounding_scenery_state_jumps_data)
#define goal_parade_leftward_state_jumps_ADDRESS 0x0135D8u
extern const void *goal_parade_leftward_state_jumps_data;
#define goal_parade_leftward_state_jumps ((const uint32_t *)goal_parade_leftward_state_jumps_data)
#define goal_parade_dragon_state_jumps_ADDRESS 0x0136A8u
extern const void *goal_parade_dragon_state_jumps_data;
#define goal_parade_dragon_state_jumps ((const uint32_t *)goal_parade_dragon_state_jumps_data)
#define traffic_spawner_state_jumps_ADDRESS 0x013778u
extern const void *traffic_spawner_state_jumps_data;
#define traffic_spawner_state_jumps ((const uint32_t *)traffic_spawner_state_jumps_data)
#define traffic_car_state_jumps_ADDRESS 0x0138F8u
extern const void *traffic_car_state_jumps_data;
#define traffic_car_state_jumps ((const uint32_t *)traffic_car_state_jumps_data)
#define traffic_stopping_car_state_jumps_ADDRESS 0x013B04u
extern const void *traffic_stopping_car_state_jumps_data;
#define traffic_stopping_car_state_jumps ((const uint32_t *)traffic_stopping_car_state_jumps_data)
#define traffic_figure_state_jumps_ADDRESS 0x013BB0u
extern const void *traffic_figure_state_jumps_data;
#define traffic_figure_state_jumps ((const uint32_t *)traffic_figure_state_jumps_data)
#define traffic_shake_frame_offsets_ADDRESS 0x013D45u
extern const void *traffic_shake_frame_offsets_data;
#define traffic_shake_frame_offsets ((const uint8_t *)traffic_shake_frame_offsets_data)
#define course_length_command_jumps_ADDRESS 0x013D94u
extern const void *course_length_command_jumps_data;
#define course_length_command_jumps ((const uint32_t *)course_length_command_jumps_data)
#define fork_branch_state_jumps_ADDRESS 0x013DF0u
extern const void *fork_branch_state_jumps_data;
#define fork_branch_state_jumps ((const uint32_t *)fork_branch_state_jumps_data)
#define fork_branch_stage_palettes_ADDRESS 0x0140C8u
extern const void *fork_branch_stage_palettes_data;
#define fork_branch_stage_palettes ((const uint16_t *)fork_branch_stage_palettes_data)
#define race_car_occupant_state_jumps_ADDRESS 0x014114u
extern const void *race_car_occupant_state_jumps_data;
#define race_car_occupant_state_jumps ((const uint32_t *)race_car_occupant_state_jumps_data)
#define rider_gesture_state_jumps_ADDRESS 0x014200u
extern const void *rider_gesture_state_jumps_data;
#define rider_gesture_state_jumps ((const uint32_t *)rider_gesture_state_jumps_data)
#define rider_gesture_x_offsets_ADDRESS 0x014360u
extern const void *rider_gesture_x_offsets_data;
#define rider_gesture_x_offsets ((const uint16_t *)rider_gesture_x_offsets_data)
#define rider_gesture_y_offsets_ADDRESS 0x014380u
extern const void *rider_gesture_y_offsets_data;
#define rider_gesture_y_offsets ((const uint16_t *)rider_gesture_y_offsets_data)
#define rider_gesture_car_states_ADDRESS 0x0143A0u
extern const void *rider_gesture_car_states_data;
#define rider_gesture_car_states ((const uint8_t *)rider_gesture_car_states_data)
#define rider_gesture_views_ADDRESS 0x0143A6u
extern const void *rider_gesture_views_data;
#define rider_gesture_views ((const uint8_t *)rider_gesture_views_data)
#define race_car_occupant_animations_ADDRESS 0x01440Eu
extern const void *race_car_occupant_animations_data;
#define race_car_occupant_animations ((const uint16_t *)race_car_occupant_animations_data)
#define race_car_rider_palettes_ADDRESS 0x01442Eu
extern const void *race_car_rider_palettes_data;
#define race_car_rider_palettes ((const uint16_t *)race_car_rider_palettes_data)
#define rider_gesture_animations_ADDRESS 0x01444Eu
extern const void *rider_gesture_animations_data;
#define rider_gesture_animations ((const uint16_t *)rider_gesture_animations_data)
#define race_opening_state_jumps_ADDRESS 0x01447Cu
extern const void *race_opening_state_jumps_data;
#define race_opening_state_jumps ((const uint32_t *)race_opening_state_jumps_data)
#define race_opening_sound_step_jumps_ADDRESS 0x0144A9u
extern const void *race_opening_sound_step_jumps_data;
#define race_opening_sound_step_jumps ((const uint32_t *)race_opening_sound_step_jumps_data)
#define race_opening_camera_step_jumps_ADDRESS 0x0144ECu
extern const void *race_opening_camera_step_jumps_data;
#define race_opening_camera_step_jumps ((const uint32_t *)race_opening_camera_step_jumps_data)
#define stage_end_sounds_state_jumps_ADDRESS 0x014520u
extern const void *stage_end_sounds_state_jumps_data;
#define stage_end_sounds_state_jumps ((const uint32_t *)stage_end_sounds_state_jumps_data)
#define fork_camera_state_jumps_ADDRESS 0x0145A0u
extern const void *fork_camera_state_jumps_data;
#define fork_camera_state_jumps ((const uint32_t *)fork_camera_state_jumps_data)
#define fork_camera_lift_steps_ADDRESS 0x014674u
extern const void *fork_camera_lift_steps_data;
#define fork_camera_lift_steps ((const uint16_t *)fork_camera_lift_steps_data)
#define fork_camera_tilt_steps_ADDRESS 0x014684u
extern const void *fork_camera_tilt_steps_data;
#define fork_camera_tilt_steps ((const uint16_t *)fork_camera_tilt_steps_data)
#define race_car_ending_state_jumps_ADDRESS 0x0146A0u
extern const void *race_car_ending_state_jumps_data;
#define race_car_ending_state_jumps ((const uint32_t *)race_car_ending_state_jumps_data)
#define goal_actor_command_jumps_ADDRESS 0x014994u
extern const void *goal_actor_command_jumps_data;
#define goal_actor_command_jumps ((const uint32_t *)goal_actor_command_jumps_data)
#define goal_actor_place_waits_ADDRESS 0x014A16u
extern const void *goal_actor_place_waits_data;
#define goal_actor_place_waits ((const uint8_t *)goal_actor_place_waits_data)
#define ending_actor_lists_ADDRESS 0x014A20u
extern const void *ending_actor_lists_data;
#define ending_actor_lists ((const uint32_t *)ending_actor_lists_data)
#define unfinished_ending_actor_count_ADDRESS 0x014A60u
extern const void *unfinished_ending_actor_count_data;
#define unfinished_ending_actor_count (*(const uint32_t *)unfinished_ending_actor_count_data)
#define unfinished_ending_actors_ADDRESS 0x014A64u
extern const void *unfinished_ending_actors_data;
#define unfinished_ending_actors (*(const uint32_t *)unfinished_ending_actors_data)
#define road_surface_state_jumps_ADDRESS 0x016B0Cu
extern const void *road_surface_state_jumps_data;
#define road_surface_state_jumps ((const uint32_t *)road_surface_state_jumps_data)
#define road_surface_event_jumps_ADDRESS 0x016BBCu
extern const void *road_surface_event_jumps_data;
#define road_surface_event_jumps ((const uint32_t *)road_surface_event_jumps_data)
#define road_surface_page_words_ADDRESS 0x01714Cu
extern const void *road_surface_page_words_data;
#define road_surface_page_words (*(const uint32_t *)road_surface_page_words_data)
#define road_tiles_change_state_jumps_ADDRESS 0x0171E0u
extern const void *road_tiles_change_state_jumps_data;
#define road_tiles_change_state_jumps ((const uint32_t *)road_tiles_change_state_jumps_data)
#define road_tiles_change_first_data_ADDRESS 0x017444u
extern const void *road_tiles_change_first_data_data;
#define road_tiles_change_first_data (*(const uint8_t *)road_tiles_change_first_data_data)
#define road_tiles_change_second_data_ADDRESS 0x0174B4u
extern const void *road_tiles_change_second_data_data;
#define road_tiles_change_second_data (*(const uint8_t *)road_tiles_change_second_data_data)
#define race_background_state_jumps_ADDRESS 0x017530u
extern const void *race_background_state_jumps_data;
#define race_background_state_jumps ((const uint32_t *)race_background_state_jumps_data)
#define race_background_change_step_jumps_ADDRESS 0x017738u
extern const void *race_background_change_step_jumps_data;
#define race_background_change_step_jumps ((const uint32_t *)race_background_change_step_jumps_data)
#define background_lower_animation_tile_pages_ADDRESS 0x017904u
extern const void *background_lower_animation_tile_pages_data;
#define background_lower_animation_tile_pages ((const uint32_t *)background_lower_animation_tile_pages_data)
#define background_lower_animation_page_pairs_ADDRESS 0x01790Cu
extern const void *background_lower_animation_page_pairs_data;
#define background_lower_animation_page_pairs ((const uint16_t *)background_lower_animation_page_pairs_data)
#define background_upper_animation_tile_pages_ADDRESS 0x017988u
extern const void *background_upper_animation_tile_pages_data;
#define background_upper_animation_tile_pages ((const uint32_t *)background_upper_animation_tile_pages_data)
#define background_upper_animation_page_pairs_ADDRESS 0x0179A0u
extern const void *background_upper_animation_page_pairs_data;
#define background_upper_animation_page_pairs ((const uint16_t *)background_upper_animation_page_pairs_data)
#define background_register_lists_ADDRESS 0x017A78u
extern const void *background_register_lists_data;
#define background_register_lists ((const uint32_t *)background_register_lists_data)
#define background_page_words_ADDRESS 0x017AE0u
extern const void *background_page_words_data;
#define background_page_words ((const uint32_t *)background_page_words_data)
#define stage_backgrounds_ADDRESS 0x017B60u
extern const void *stage_backgrounds_data;
#define stage_backgrounds ((const uint8_t *)stage_backgrounds_data)
#define background_tile_banks_ADDRESS 0x017B7Fu
extern const void *background_tile_banks_data;
#define background_tile_banks ((const uint8_t *)background_tile_banks_data)
#define stage_tile_banks_ADDRESS 0x017BA9u
extern const void *stage_tile_banks_data;
#define stage_tile_banks ((const uint8_t *)stage_tile_banks_data)
#define palette_cycle_state_jumps_ADDRESS 0x017C08u
extern const void *palette_cycle_state_jumps_data;
#define palette_cycle_state_jumps ((const uint32_t *)palette_cycle_state_jumps_data)
#define palette_cycle_running_jumps_ADDRESS 0x017C68u
extern const void *palette_cycle_running_jumps_data;
#define palette_cycle_running_jumps ((const uint32_t *)palette_cycle_running_jumps_data)
#define sunset_fade_state_jumps_ADDRESS 0x017CB8u
extern const void *sunset_fade_state_jumps_data;
#define sunset_fade_state_jumps ((const uint32_t *)sunset_fade_state_jumps_data)
#define backdrop_far_state_jumps_ADDRESS 0x017DE4u
extern const void *backdrop_far_state_jumps_data;
#define backdrop_far_state_jumps ((const uint32_t *)backdrop_far_state_jumps_data)
#define backdrop_near_state_jumps_ADDRESS 0x017E98u
extern const void *backdrop_near_state_jumps_data;
#define backdrop_near_state_jumps ((const uint32_t *)backdrop_near_state_jumps_data)
#define backdrop_landmark_state_jumps_ADDRESS 0x017F7Cu
extern const void *backdrop_landmark_state_jumps_data;
#define backdrop_landmark_state_jumps ((const uint32_t *)backdrop_landmark_state_jumps_data)
#define backdrop_flashing_state_jumps_ADDRESS 0x018094u
extern const void *backdrop_flashing_state_jumps_data;
#define backdrop_flashing_state_jumps ((const uint32_t *)backdrop_flashing_state_jumps_data)
#define backdrop_translucent_state_jumps_ADDRESS 0x01814Cu
extern const void *backdrop_translucent_state_jumps_data;
#define backdrop_translucent_state_jumps ((const uint32_t *)backdrop_translucent_state_jumps_data)
#define stage_background_object_lists_ADDRESS 0x0181B8u
extern const void *stage_background_object_lists_data;
#define stage_background_object_lists ((const uint32_t *)stage_background_object_lists_data)
#define lcd_scroll_step_jumps_ADDRESS 0x018A70u
extern const void *lcd_scroll_step_jumps_data;
#define lcd_scroll_step_jumps ((const uint32_t *)lcd_scroll_step_jumps_data)
#define dj_face_characters_ADDRESS 0x018AE8u
extern const void *dj_face_characters_data;
#define dj_face_characters ((const uint16_t *)dj_face_characters_data)
#define lcd_restart_nibbles_ADDRESS 0x018B21u
extern const void *lcd_restart_nibbles_data;
#define lcd_restart_nibbles ((const uint8_t *)lcd_restart_nibbles_data)
#define lcd_race_info_state_jumps_ADDRESS 0x018BD4u
extern const void *lcd_race_info_state_jumps_data;
#define lcd_race_info_state_jumps ((const uint32_t *)lcd_race_info_state_jumps_data)
#define lcd_race_info_page_jumps_ADDRESS 0x018C64u
extern const void *lcd_race_info_page_jumps_data;
#define lcd_race_info_page_jumps ((const uint32_t *)lcd_race_info_page_jumps_data)
#define lcd_race_info_time_step_jumps_ADDRESS 0x018C84u
extern const void *lcd_race_info_time_step_jumps_data;
#define lcd_race_info_time_step_jumps ((const uint32_t *)lcd_race_info_time_step_jumps_data)
#define lcd_race_info_order_step_jumps_ADDRESS 0x018CF0u
extern const void *lcd_race_info_order_step_jumps_data;
#define lcd_race_info_order_step_jumps ((const uint32_t *)lcd_race_info_order_step_jumps_data)
#define lcd_race_info_speed_step_jumps_ADDRESS 0x018D5Cu
extern const void *lcd_race_info_speed_step_jumps_data;
#define lcd_race_info_speed_step_jumps ((const uint32_t *)lcd_race_info_speed_step_jumps_data)
#define lcd_race_info_maximum_step_jumps_ADDRESS 0x018D88u
extern const void *lcd_race_info_maximum_step_jumps_data;
#define lcd_race_info_maximum_step_jumps ((const uint32_t *)lcd_race_info_maximum_step_jumps_data)
#define lcd_race_info_average_step_jumps_ADDRESS 0x018DB4u
extern const void *lcd_race_info_average_step_jumps_data;
#define lcd_race_info_average_step_jumps ((const uint32_t *)lcd_race_info_average_step_jumps_data)
#define lcd_race_info_finish_state_jumps_ADDRESS 0x018E40u
extern const void *lcd_race_info_finish_state_jumps_data;
#define lcd_race_info_finish_state_jumps ((const uint32_t *)lcd_race_info_finish_state_jumps_data)
#define lcd_countdown_bars_ADDRESS 0x018EA0u
extern const void *lcd_countdown_bars_data;
#define lcd_countdown_bars ((const uint32_t *)lcd_countdown_bars_data)
#define lcd_time_label_ADDRESS 0x018F1Eu
extern const void *lcd_time_label_data;
#define lcd_time_label (*(const uint8_t *)lcd_time_label_data)
#define lcd_order_label_ADDRESS 0x018F2Cu
extern const void *lcd_order_label_data;
#define lcd_order_label (*(const uint8_t *)lcd_order_label_data)
#define lcd_speed_label_ADDRESS 0x018F3Au
extern const void *lcd_speed_label_data;
#define lcd_speed_label (*(const uint8_t *)lcd_speed_label_data)
#define lcd_maximum_label_ADDRESS 0x018F48u
extern const void *lcd_maximum_label_data;
#define lcd_maximum_label (*(const uint8_t *)lcd_maximum_label_data)
#define lcd_average_label_ADDRESS 0x018F56u
extern const void *lcd_average_label_data;
#define lcd_average_label (*(const uint8_t *)lcd_average_label_data)
#define lcd_finish_label_ADDRESS 0x018F64u
extern const void *lcd_finish_label_data;
#define lcd_finish_label (*(const uint8_t *)lcd_finish_label_data)
#define car_radio_state_jumps_ADDRESS 0x019044u
extern const void *car_radio_state_jumps_data;
#define car_radio_state_jumps ((const uint32_t *)car_radio_state_jumps_data)
#define car_radio_talk_step_jumps_ADDRESS 0x0191E0u
extern const void *car_radio_talk_step_jumps_data;
#define car_radio_talk_step_jumps ((const uint32_t *)car_radio_talk_step_jumps_data)
#define song_title_state_jumps_ADDRESS 0x019330u
extern const void *song_title_state_jumps_data;
#define song_title_state_jumps ((const uint32_t *)song_title_state_jumps_data)
#define car_model_songs_ADDRESS 0x019421u
extern const void *car_model_songs_data;
#define car_model_songs ((const uint8_t *)car_model_songs_data)
#define radio_songs_ADDRESS 0x01942Cu
extern const void *radio_songs_data;
#define radio_songs ((const uint32_t *)radio_songs_data)
#define route_stage_radio_sounds_ADDRESS 0x019458u
extern const void *route_stage_radio_sounds_data;
#define route_stage_radio_sounds ((const uint8_t *)route_stage_radio_sounds_data)
#define song_title_colours_ADDRESS 0x019561u
extern const void *song_title_colours_data;
#define song_title_colours ((const uint16_t *)song_title_colours_data)
#define stage_level_radio_messages_ADDRESS 0x019571u
extern const void *stage_level_radio_messages_data;
#define stage_level_radio_messages ((const uint8_t *)stage_level_radio_messages_data)
#define dj_messages_ADDRESS 0x019576u
extern const void *dj_messages_data;
#define dj_messages ((const uint32_t *)dj_messages_data)
#define dj_remark_lists_ADDRESS 0x01966Cu
extern const void *dj_remark_lists_data;
#define dj_remark_lists ((const uint32_t *)dj_remark_lists_data)
#define title_screen_state_jumps_ADDRESS 0x019CB8u
extern const void *title_screen_state_jumps_data;
#define title_screen_state_jumps ((const uint32_t *)title_screen_state_jumps_data)
#define title_car_state_jumps_ADDRESS 0x019E78u
extern const void *title_car_state_jumps_data;
#define title_car_state_jumps ((const uint32_t *)title_car_state_jumps_data)
#define title_car_driver_state_jumps_ADDRESS 0x019F58u
extern const void *title_car_driver_state_jumps_data;
#define title_car_driver_state_jumps ((const uint32_t *)title_car_driver_state_jumps_data)
#define title_car_passenger_state_jumps_ADDRESS 0x019F6Cu
extern const void *title_car_passenger_state_jumps_data;
#define title_car_passenger_state_jumps ((const uint32_t *)title_car_passenger_state_jumps_data)
#define title_driver_animations_ADDRESS 0x01A012u
extern const void *title_driver_animations_data;
#define title_driver_animations ((const uint16_t *)title_driver_animations_data)
#define title_passenger_animations_ADDRESS 0x01A022u
extern const void *title_passenger_animations_data;
#define title_passenger_animations ((const uint16_t *)title_passenger_animations_data)
#define title_driver_palettes_ADDRESS 0x01A032u
extern const void *title_driver_palettes_data;
#define title_driver_palettes ((const uint16_t *)title_driver_palettes_data)
#define title_passenger_palettes_ADDRESS 0x01A042u
extern const void *title_passenger_palettes_data;
#define title_passenger_palettes ((const uint16_t *)title_passenger_palettes_data)
#define title_rider_scales_ADDRESS 0x01A052u
extern const void *title_rider_scales_data;
#define title_rider_scales ((const uint16_t *)title_rider_scales_data)
#define title_driver_x_offsets_ADDRESS 0x01A062u
extern const void *title_driver_x_offsets_data;
#define title_driver_x_offsets ((const uint16_t *)title_driver_x_offsets_data)
#define title_driver_y_offsets_ADDRESS 0x01A072u
extern const void *title_driver_y_offsets_data;
#define title_driver_y_offsets ((const uint16_t *)title_driver_y_offsets_data)
#define title_passenger_x_offsets_ADDRESS 0x01A082u
extern const void *title_passenger_x_offsets_data;
#define title_passenger_x_offsets ((const uint16_t *)title_passenger_x_offsets_data)
#define title_passenger_y_offsets_ADDRESS 0x01A092u
extern const void *title_passenger_y_offsets_data;
#define title_passenger_y_offsets ((const uint16_t *)title_passenger_y_offsets_data)
#define title_palm_tree_state_jumps_ADDRESS 0x01A0F0u
extern const void *title_palm_tree_state_jumps_data;
#define title_palm_tree_state_jumps ((const uint32_t *)title_palm_tree_state_jumps_data)
#define title_logo_state_jumps_ADDRESS 0x01A198u
extern const void *title_logo_state_jumps_data;
#define title_logo_state_jumps ((const uint32_t *)title_logo_state_jumps_data)
#define title_logo_copy_state_jumps_ADDRESS 0x01A2A8u
extern const void *title_logo_copy_state_jumps_data;
#define title_logo_copy_state_jumps ((const uint32_t *)title_logo_copy_state_jumps_data)
#define title_logo_car_state_jumps_ADDRESS 0x01A334u
extern const void *title_logo_car_state_jumps_data;
#define title_logo_car_state_jumps ((const uint32_t *)title_logo_car_state_jumps_data)
#define title_logo_stripe_state_jumps_ADDRESS 0x01A3F4u
extern const void *title_logo_stripe_state_jumps_data;
#define title_logo_stripe_state_jumps ((const uint32_t *)title_logo_stripe_state_jumps_data)
#define title_board_palms_state_jumps_ADDRESS 0x01A4A4u
extern const void *title_board_palms_state_jumps_data;
#define title_board_palms_state_jumps ((const uint32_t *)title_board_palms_state_jumps_data)
#define title_seagull_state_jumps_ADDRESS 0x01A544u
extern const void *title_seagull_state_jumps_data;
#define title_seagull_state_jumps ((const uint32_t *)title_seagull_state_jumps_data)
#define title_picture_state_jumps_ADDRESS 0x01A680u
extern const void *title_picture_state_jumps_data;
#define title_picture_state_jumps ((const uint32_t *)title_picture_state_jumps_data)
#define coin_prompt_state_jumps_ADDRESS 0x01A6DCu
extern const void *coin_prompt_state_jumps_data;
#define coin_prompt_state_jumps ((const uint32_t *)coin_prompt_state_jumps_data)
#define coin_prompt_screen_jumps_ADDRESS 0x01A744u
extern const void *coin_prompt_screen_jumps_data;
#define coin_prompt_screen_jumps ((const uint32_t *)coin_prompt_screen_jumps_data)
#define coin_prompt_car_state_jumps_ADDRESS 0x01A7D0u
extern const void *coin_prompt_car_state_jumps_data;
#define coin_prompt_car_state_jumps ((const uint32_t *)coin_prompt_car_state_jumps_data)
#define coin_prompt_message_state_jumps_ADDRESS 0x01A84Cu
extern const void *coin_prompt_message_state_jumps_data;
#define coin_prompt_message_state_jumps ((const uint32_t *)coin_prompt_message_state_jumps_data)
#define ignition_key_prompt_state_jumps_ADDRESS 0x01A8F0u
extern const void *ignition_key_prompt_state_jumps_data;
#define ignition_key_prompt_state_jumps ((const uint32_t *)ignition_key_prompt_state_jumps_data)
#define coin_prompt_car_animations_ADDRESS 0x01A986u
extern const void *coin_prompt_car_animations_data;
#define coin_prompt_car_animations ((const uint16_t *)coin_prompt_car_animations_data)
#define coin_prompt_car_palettes_ADDRESS 0x01A996u
extern const void *coin_prompt_car_palettes_data;
#define coin_prompt_car_palettes ((const uint16_t *)coin_prompt_car_palettes_data)
#define coin_prompt_message_animations_ADDRESS 0x01A9A6u
extern const void *coin_prompt_message_animations_data;
#define coin_prompt_message_animations ((const uint16_t *)coin_prompt_message_animations_data)
#define coin_prompt_message_palettes_ADDRESS 0x01A9ACu
extern const void *coin_prompt_message_palettes_data;
#define coin_prompt_message_palettes ((const uint16_t *)coin_prompt_message_palettes_data)
#define coin_prompt_lcd_texts_ADDRESS 0x01A9BCu
extern const void *coin_prompt_lcd_texts_data;
#define coin_prompt_lcd_texts ((const uint32_t *)coin_prompt_lcd_texts_data)
#define credit_display_state_jumps_ADDRESS 0x01AA0Cu
extern const void *credit_display_state_jumps_data;
#define credit_display_state_jumps ((const uint32_t *)credit_display_state_jumps_data)
#define credit_display_screen_jumps_ADDRESS 0x01AA4Cu
extern const void *credit_display_screen_jumps_data;
#define credit_display_screen_jumps ((const uint32_t *)credit_display_screen_jumps_data)
#define credit_digit_characters_ADDRESS 0x01AB8Eu
extern const void *credit_digit_characters_data;
#define credit_digit_characters ((const uint8_t *)credit_digit_characters_data)
#define credit_display_animations_ADDRESS 0x01AB98u
extern const void *credit_display_animations_data;
#define credit_display_animations ((const uint16_t *)credit_display_animations_data)
#define credit_display_palettes_ADDRESS 0x01AB9Eu
extern const void *credit_display_palettes_data;
#define credit_display_palettes ((const uint16_t *)credit_display_palettes_data)
#define title_car_screen_offsets_ADDRESS 0x01ABA4u
extern const void *title_car_screen_offsets_data;
#define title_car_screen_offsets ((const uint16_t *)title_car_screen_offsets_data)
#define title_car_animations_ADDRESS 0x01AC34u
extern const void *title_car_animations_data;
#define title_car_animations ((const uint16_t *)title_car_animations_data)
#define title_logo_car_palettes_ADDRESS 0x01AC44u
extern const void *title_logo_car_palettes_data;
#define title_logo_car_palettes ((const uint16_t *)title_logo_car_palettes_data)
#define title_logo_car_wave_frames_ADDRESS 0x01ACC0u
extern const void *title_logo_car_wave_frames_data;
#define title_logo_car_wave_frames (*(const uint16_t *)title_logo_car_wave_frames_data)
#define demo_captions_state_jumps_ADDRESS 0x01ADF4u
extern const void *demo_captions_state_jumps_data;
#define demo_captions_state_jumps ((const uint32_t *)demo_captions_state_jumps_data)
#define demo_caption_state_jumps_ADDRESS 0x01AE98u
extern const void *demo_caption_state_jumps_data;
#define demo_caption_state_jumps ((const uint32_t *)demo_caption_state_jumps_data)
#define kanji_text_viewer_state_jumps_ADDRESS 0x01AFE8u
extern const void *kanji_text_viewer_state_jumps_data;
#define kanji_text_viewer_state_jumps ((const uint32_t *)kanji_text_viewer_state_jumps_data)
#define unused_kanji_text_viewer_words_ADDRESS 0x01B248u
extern const void *unused_kanji_text_viewer_words_data;
#define unused_kanji_text_viewer_words (*(const uint16_t *)unused_kanji_text_viewer_words_data)
#define kanji_texts_ADDRESS 0x01B254u
extern const void *kanji_texts_data;
#define kanji_texts ((const uint32_t *)kanji_texts_data)
#define test_mode_lcd_text_ADDRESS 0x01C378u
extern const void *test_mode_lcd_text_data;
#define test_mode_lcd_text (*(const uint32_t *)test_mode_lcd_text_data)
#define test_mode_lcd_text_4_to_7_ADDRESS 0x01C37Cu
extern const void *test_mode_lcd_text_4_to_7_data;
#define test_mode_lcd_text_4_to_7 (*(const uint32_t *)test_mode_lcd_text_4_to_7_data)
#define test_mode_lcd_text_8_to_11_ADDRESS 0x01C380u
extern const void *test_mode_lcd_text_8_to_11_data;
#define test_mode_lcd_text_8_to_11 (*(const uint32_t *)test_mode_lcd_text_8_to_11_data)
#define test_mode_lcd_text_12_to_15_ADDRESS 0x01C384u
extern const void *test_mode_lcd_text_12_to_15_data;
#define test_mode_lcd_text_12_to_15 (*(const uint32_t *)test_mode_lcd_text_12_to_15_data)
#define test_mode_page_jumps_ADDRESS 0x01C45Cu
extern const void *test_mode_page_jumps_data;
#define test_mode_page_jumps ((const uint32_t *)test_mode_page_jumps_data)
#define test_mode_menu_headings_ADDRESS 0x01C560u
extern const void *test_mode_menu_headings_data;
#define test_mode_menu_headings ((const uint32_t *)test_mode_menu_headings_data)
#define test_mode_menu_cursor_cells_ADDRESS 0x01C588u
extern const void *test_mode_menu_cursor_cells_data;
#define test_mode_menu_cursor_cells ((const uint16_t *)test_mode_menu_cursor_cells_data)
#define volume_adjustment_cursor_cells_ADDRESS 0x01CF9Cu
extern const void *volume_adjustment_cursor_cells_data;
#define volume_adjustment_cursor_cells ((const uint16_t *)volume_adjustment_cursor_cells_data)
#define volume_adjustment_cancel_cell_ADDRESS 0x01CF9Eu
extern const void *volume_adjustment_cancel_cell_data;
#define volume_adjustment_cancel_cell (*(const uint16_t *)volume_adjustment_cancel_cell_data)
#define input_test_on_off_cells_ADDRESS 0x01D3C0u
extern const void *input_test_on_off_cells_data;
#define input_test_on_off_cells ((const uint32_t *)input_test_on_off_cells_data)
#define output_test_cursor_cells_ADDRESS 0x01D5AAu
extern const void *output_test_cursor_cells_data;
#define output_test_cursor_cells ((const uint16_t *)output_test_cursor_cells_data)
#define output_test_exit_cell_ADDRESS 0x01D5BEu
extern const void *output_test_exit_cell_data;
#define output_test_exit_cell (*(const uint16_t *)output_test_exit_cell_data)
#define test_mode_output_test_item_jumps_ADDRESS 0x01D5ECu
extern const void *test_mode_output_test_item_jumps_data;
#define test_mode_output_test_item_jumps ((const uint32_t *)test_mode_output_test_item_jumps_data)
#define output_test_on_off_cells_ADDRESS 0x01D618u
extern const void *output_test_on_off_cells_data;
#define output_test_on_off_cells ((const uint32_t *)output_test_on_off_cells_data)
#define test_mode_sound_test_item_jumps_ADDRESS 0x01D77Cu
extern const void *test_mode_sound_test_item_jumps_data;
#define test_mode_sound_test_item_jumps ((const uint32_t *)test_mode_sound_test_item_jumps_data)
#define sound_test_cursor_cells_ADDRESS 0x01D7C0u
extern const void *sound_test_cursor_cells_data;
#define sound_test_cursor_cells ((const uint16_t *)sound_test_cursor_cells_data)
#define sound_test_exit_cell_ADDRESS 0x01D7C4u
extern const void *sound_test_exit_cell_data;
#define sound_test_exit_cell (*(const uint16_t *)sound_test_exit_cell_data)
#define sound_test_codes_ADDRESS 0x01D7C6u
extern const void *sound_test_codes_data;
#define sound_test_codes ((const uint8_t *)sound_test_codes_data)
#define test_mode_game_assignments_item_jumps_ADDRESS 0x01DBD0u
extern const void *test_mode_game_assignments_item_jumps_data;
#define test_mode_game_assignments_item_jumps ((const uint32_t *)test_mode_game_assignments_item_jumps_data)
#define game_assignments_cursor_cells_ADDRESS 0x01DCCEu
extern const void *game_assignments_cursor_cells_data;
#define game_assignments_cursor_cells ((const uint16_t *)game_assignments_cursor_cells_data)
#define game_assignments_exit_cell_ADDRESS 0x01DCE0u
extern const void *game_assignments_exit_cell_data;
#define game_assignments_exit_cell (*(const uint16_t *)game_assignments_exit_cell_data)
#define steering_wheel_reaction_texts_ADDRESS 0x01DCE4u
extern const void *steering_wheel_reaction_texts_data;
#define steering_wheel_reaction_texts ((const uint32_t *)steering_wheel_reaction_texts_data)
#define advertise_sound_texts_ADDRESS 0x01DCECu
extern const void *advertise_sound_texts_data;
#define advertise_sound_texts ((const uint32_t *)advertise_sound_texts_data)
#define test_mode_coin_assignments_item_jumps_ADDRESS 0x01E0D8u
extern const void *test_mode_coin_assignments_item_jumps_data;
#define test_mode_coin_assignments_item_jumps ((const uint32_t *)test_mode_coin_assignments_item_jumps_data)
#define test_mode_coin_assignments_manual_item_jumps_ADDRESS 0x01E0E4u
extern const void *test_mode_coin_assignments_manual_item_jumps_data;
#define test_mode_coin_assignments_manual_item_jumps ((const uint32_t *)test_mode_coin_assignments_manual_item_jumps_data)
#define coin_assignments_cursor_cells_ADDRESS 0x01E200u
extern const void *coin_assignments_cursor_cells_data;
#define coin_assignments_cursor_cells ((const uint16_t *)coin_assignments_cursor_cells_data)
#define coin_manual_cursor_cells_ADDRESS 0x01E206u
extern const void *coin_manual_cursor_cells_data;
#define coin_manual_cursor_cells ((const uint16_t *)coin_manual_cursor_cells_data)
#define coin_manual_exit_cell_ADDRESS 0x01E20Cu
extern const void *coin_manual_exit_cell_data;
#define coin_manual_exit_cell (*(const uint16_t *)coin_manual_exit_cell_data)
#define coin_setting_descriptions_ADDRESS 0x01E220u
extern const void *coin_setting_descriptions_data;
#define coin_setting_descriptions ((const uint32_t *)coin_setting_descriptions_data)
#define test_mode_network_assignments_item_jumps_ADDRESS 0x01E61Cu
extern const void *test_mode_network_assignments_item_jumps_data;
#define test_mode_network_assignments_item_jumps ((const uint32_t *)test_mode_network_assignments_item_jumps_data)
#define network_assignments_cursor_cells_ADDRESS 0x01E67Cu
extern const void *network_assignments_cursor_cells_data;
#define network_assignments_cursor_cells ((const uint16_t *)network_assignments_cursor_cells_data)
#define network_assignments_exit_cell_ADDRESS 0x01E682u
extern const void *network_assignments_exit_cell_data;
#define network_assignments_exit_cell (*(const uint16_t *)network_assignments_exit_cell_data)
#define communication_texts_ADDRESS 0x01E684u
extern const void *communication_texts_data;
#define communication_texts ((const uint32_t *)communication_texts_data)
#define privilege_mode_texts_ADDRESS 0x01E68Cu
extern const void *privilege_mode_texts_data;
#define privilege_mode_texts ((const uint32_t *)privilege_mode_texts_data)
#define backup_data_clear_cursor_cells_ADDRESS 0x01EEAEu
extern const void *backup_data_clear_cursor_cells_data;
#define backup_data_clear_cursor_cells ((const uint16_t *)backup_data_clear_cursor_cells_data)
#define backup_data_clear_no_cell_ADDRESS 0x01EEB0u
extern const void *backup_data_clear_no_cell_data;
#define backup_data_clear_no_cell (*(const uint16_t *)backup_data_clear_no_cell_data)
#define test_mode_instructions_text_ADDRESS 0x01F282u
extern const void *test_mode_instructions_text_data;
#define test_mode_instructions_text (*(const uint16_t *)test_mode_instructions_text_data)
#define test_mode_palettes_1_bytes_ADDRESS 0x01F302u
extern const void *test_mode_palettes_1_bytes_data;
#define test_mode_palettes_1_bytes (*(const uint16_t *)test_mode_palettes_1_bytes_data)
#define test_mode_palettes_2_bytes_ADDRESS 0x01F504u
extern const void *test_mode_palettes_2_bytes_data;
#define test_mode_palettes_2_bytes (*(const uint16_t *)test_mode_palettes_2_bytes_data)
#define road_object_streams_ADDRESS 0x020800u
extern const void *road_object_streams_data;
#define road_object_streams ((const uint32_t *)road_object_streams_data)
#define stage_road_object_lists_ADDRESS 0x02E1F0u
extern const void *stage_road_object_lists_data;
#define stage_road_object_lists ((const uint32_t *)stage_road_object_lists_data)
#define background_palettes_ADDRESS 0x0318CCu
extern const void *background_palettes_data;
#define background_palettes ((const uint32_t *)background_palettes_data)
#define background_leaving_fades_ADDRESS 0x0319F0u
extern const void *background_leaving_fades_data;
#define background_leaving_fades ((const uint32_t *)background_leaving_fades_data)
#define background_palette_fades_ADDRESS 0x031B14u
extern const void *background_palette_fades_data;
#define background_palette_fades ((const uint32_t *)background_palette_fades_data)
#define stage_road_palettes_ADDRESS 0x031C38u
extern const void *stage_road_palettes_data;
#define stage_road_palettes ((const uint32_t *)stage_road_palettes_data)
#define background_road_palettes_ADDRESS 0x031CB8u
extern const void *background_road_palettes_data;
#define background_road_palettes ((const uint32_t *)background_road_palettes_data)
#define scene_palettes_ADDRESS 0x031DDCu
extern const void *scene_palettes_data;
#define scene_palettes ((const uint32_t *)scene_palettes_data)
#define car_select_palette_ADDRESS 0x031E7Cu
extern const void *car_select_palette_data;
#define car_select_palette (*(const uint32_t *)car_select_palette_data)
#define ranking_palette_ADDRESS 0x031E80u
extern const void *ranking_palette_data;
#define ranking_palette (*(const uint32_t *)ranking_palette_data)
#define route_map_palette_ADDRESS 0x031E84u
extern const void *route_map_palette_data;
#define route_map_palette (*(const uint32_t *)route_map_palette_data)
#define title_palette_ADDRESS 0x031E88u
extern const void *title_palette_data;
#define title_palette (*(const uint32_t *)title_palette_data)
#define route_map_background_palette_ADDRESS 0x031E8Cu
extern const void *route_map_background_palette_data;
#define route_map_background_palette (*(const uint32_t *)route_map_background_palette_data)
#define stage_road_page_sets_ADDRESS 0x03CC80u
extern const void *stage_road_page_sets_data;
#define stage_road_page_sets ((const uint8_t *)stage_road_page_sets_data)
#define vblank_timer_period_ADDRESS 0x03F82Au
extern const void *vblank_timer_period_data;
#define vblank_timer_period (*(const uint16_t *)vblank_timer_period_data)
#define lcd_timer_period_ADDRESS 0x03F82Cu
extern const void *lcd_timer_period_data;
#define lcd_timer_period (*(const uint16_t *)lcd_timer_period_data)
#define link_message_kind_jumps_ADDRESS 0x03F85Cu
extern const void *link_message_kind_jumps_data;
#define link_message_kind_jumps ((const uint32_t *)link_message_kind_jumps_data)
#define vblank_seat_inputs_jumps_ADDRESS 0x03FAC4u
extern const void *vblank_seat_inputs_jumps_data;
#define vblank_seat_inputs_jumps ((const uint32_t *)vblank_seat_inputs_jumps_data)
#define screen_vblank_counters_ADDRESS 0x03FACCu
extern const void *screen_vblank_counters_data;
#define screen_vblank_counters ((const uint32_t *)screen_vblank_counters_data)
#define ic37_checksum_ADDRESS 0x03FFF8u
extern const void *ic37_checksum_data;
#define ic37_checksum (*(const uint16_t *)ic37_checksum_data)
#define ic40_checksum_ADDRESS 0x03FFFAu
extern const void *ic40_checksum_data;
#define ic40_checksum (*(const uint16_t *)ic40_checksum_data)
#define ic36_checksum_ADDRESS 0x03FFFCu
extern const void *ic36_checksum_data;
#define ic36_checksum (*(const uint16_t *)ic36_checksum_data)
#define ic39_checksum_ADDRESS 0x03FFFEu
extern const void *ic39_checksum_data;
#define ic39_checksum (*(const uint16_t *)ic39_checksum_data)
#define blank_sprite_places_ADDRESS 0x100000u
extern const void *blank_sprite_places_data;
#define blank_sprite_places (*(const uint32_t *)blank_sprite_places_data)
#define sine_table_ADDRESS 0x100C00u
extern const void *sine_table_data;
#define sine_table ((const int16_t *)sine_table_data)
#define cosine_table_ADDRESS 0x101400u
extern const void *cosine_table_data;
#define cosine_table ((const int16_t *)cosine_table_data)
#define arctangents_ADDRESS 0x103400u
extern const void *arctangents_data;
#define arctangents ((const uint16_t *)arctangents_data)
#define boot_tile_pages_5d_ADDRESS 0x109420u
extern const void *boot_tile_pages_5d_data;
#define boot_tile_pages_5d (*(const uint32_t *)boot_tile_pages_5d_data)
#define boot_tile_pages_5e_ADDRESS 0x109428u
extern const void *boot_tile_pages_5e_data;
#define boot_tile_pages_5e (*(const uint32_t *)boot_tile_pages_5e_data)
#define boot_tile_pages_29_to_4e_ADDRESS 0x109578u
extern const void *boot_tile_pages_29_to_4e_data;
#define boot_tile_pages_29_to_4e (*(const uint32_t *)boot_tile_pages_29_to_4e_data)
#define no_background_tile_pages_ADDRESS 0x109684u
extern const void *no_background_tile_pages_data;
#define no_background_tile_pages (*(const uint32_t *)no_background_tile_pages_data)
#define background_tile_page_lists_ADDRESS 0x109694u
extern const void *background_tile_page_lists_data;
#define background_tile_page_lists ((const uint32_t *)background_tile_page_lists_data)
#define text_sprite_words_0_1_ADDRESS 0x122B5Cu
extern const void *text_sprite_words_0_1_data;
#define text_sprite_words_0_1 (*(const uint32_t *)text_sprite_words_0_1_data)
#define text_sprite_size_ADDRESS 0x122B60u
extern const void *text_sprite_size_data;
#define text_sprite_size (*(const uint32_t *)text_sprite_size_data)
#define text_sprite_font_ADDRESS 0x122B64u
extern const void *text_sprite_font_data;
#define text_sprite_font (*(const uint16_t *)text_sprite_font_data)
#define text_sprite_palette_ADDRESS 0x122B66u
extern const void *text_sprite_palette_data;
#define text_sprite_palette (*(const uint16_t *)text_sprite_palette_data)
#define entry_countdown_palette_fades_ADDRESS 0x13F66Cu
extern const void *entry_countdown_palette_fades_data;
#define entry_countdown_palette_fades (*(const uint32_t *)entry_countdown_palette_fades_data)
#define name_entry_palette_fades_ADDRESS 0x13F6C0u
extern const void *name_entry_palette_fades_data;
#define name_entry_palette_fades (*(const uint32_t *)name_entry_palette_fades_data)
#define manual_picked_palettes_ADDRESS 0x13F95Cu
extern const void *manual_picked_palettes_data;
#define manual_picked_palettes (*(const uint32_t *)manual_picked_palettes_data)
#define automatic_picked_palettes_ADDRESS 0x13F998u
extern const void *automatic_picked_palettes_data;
#define automatic_picked_palettes (*(const uint32_t *)automatic_picked_palettes_data)
#define no_background_palettes_ADDRESS 0x13F9D4u
extern const void *no_background_palettes_data;
#define no_background_palettes (*(const uint32_t *)no_background_palettes_data)
#define race_car_rider_places_ADDRESS 0x18D25Cu
extern const void *race_car_rider_places_data;
#define race_car_rider_places ((const uint32_t *)race_car_rider_places_data)
#define perspective_scales_ADDRESS 0x19A340u
extern const void *perspective_scales_data;
#define perspective_scales ((const uint16_t *)perspective_scales_data)
#define depth_layers_by_depth_ADDRESS 0x1A0340u
extern const void *depth_layers_by_depth_data;
#define depth_layers_by_depth ((const uint16_t *)depth_layers_by_depth_data)
#define kanji_row_pixels_ADDRESS 0x1A6340u
extern const void *kanji_row_pixels_data;
#define kanji_row_pixels ((const uint32_t *)kanji_row_pixels_data)
#define kanji_font_ADDRESS 0x1CF000u
extern const void *kanji_font_data;
#define kanji_font (*(const uint8_t *)kanji_font_data)

/* the board's registers (read and written through the bus: io_read8, io_write8...) */
#define SPRITE_RENDER_REQUEST 0x500000u
#define BACKGROUND_COLOUR_1 0x609C00u
#define MIXER_1_OFFSET_0_RED 0x610040u
#define MIXER_1_OFFSET_0_BLUE 0x610044u
#define MIXER_1_OFFSET_1_GREEN 0x610048u
#define BACKGROUND_COLOUR_2 0x689C00u
#define MIXER_2_OFFSET_0_RED 0x690040u
#define MIXER_2_OFFSET_0_BLUE 0x690044u
#define MIXER_2_OFFSET_1_GREEN 0x690048u
#define LINK_MACHINE_COUNT 0x800000u
#define LINK_MACHINE_NUMBER 0x800002u
#define LINK_MASTER 0x800004u
#define LINK_RECORDS_SENT 0x800006u
#define LINK_ENTRIES_CLOSED 0x80000Au
#define LINK_BOARD_ERROR 0x80000Cu
#define LINK_SECOND_BUFFER 0x80000Eu
#define LINK_MESSAGE_KIND 0x800010u
#define LINK_ISDN_RECORDS_READY 0x8006E1u
#define LINK_BOARD_ON 0x801000u
#define LINK_BOARD_FLAGS 0x801002u
#define IO_1_PORT_A 0xC00000u
#define IO_1_PORT_D 0xC00006u
#define IO_1_PORT_E 0xC00008u
#define IO_1_PORT_F 0xC0000Au
#define IO_1_PORT_G 0xC0000Cu
#define IO_1_PORT_H 0xC0000Eu
#define IO_1_CNT 0xC0001Cu
#define IO_1_DIRECTION 0xC0001Eu
#define ANALOG_INPUTS 0xC00050u
#define ANALOG_BANK 0xC00060u
#define EVENT_CODE 0xC00074u
#define IO_2_PORT_A 0xC80000u
#define IO_2_PORT_D 0xC80006u
#define IO_2_PORT_E 0xC80008u
#define IO_2_PORT_F 0xC8000Au
#define IO_2_PORT_G 0xC8000Cu
#define IO_2_PORT_H 0xC8000Eu
#define IO_2_CNT 0xC8001Cu
#define IO_2_DIRECTION 0xC8001Eu
#define IRQ_D00005 0xD00005u
#define IRQ_ACKNOWLEDGE 0xD00007u
#define IRQ_TIMER_0 0xD00008u
#define IRQ_TIMER_1 0xD0000Au
#define RANDOM_NUMBER 0xD80000u

/* the board's memories the code uses as plain memory (the video RAM, the sprite RAM, the RAM the
   sound program shares), by name: lvalues through board() */
#define MACHINE_ID_TEXT_CELL_ADDRESS 0x31F736u
#define MACHINE_ID_TEXT_CELL BOARD16(MACHINE_ID_TEXT_CELL_ADDRESS)
#define VIDEO_CONTROL_ADDRESS 0x31FF00u
#define VIDEO_CONTROL BOARD16(VIDEO_CONTROL_ADDRESS)
#define VIDEO_LINE_CONTROL_HIGH_ADDRESS 0x31FF05u
#define VIDEO_LINE_CONTROL_HIGH BOARD8(VIDEO_LINE_CONTROL_HIGH_ADDRESS)
#define NBG0_SCROLL_X_ADDRESS 0x31FF10u
#define NBG0_SCROLL_X ((uint32_t *)board(NBG0_SCROLL_X_ADDRESS))
#define NBG0_SCROLL_Y_ADDRESS 0x31FF14u
#define NBG0_SCROLL_Y ((uint32_t *)board(NBG0_SCROLL_Y_ADDRESS))
#define NBG2_SCROLL_X_ADDRESS 0x31FF20u
#define NBG2_SCROLL_X ((uint32_t *)board(NBG2_SCROLL_X_ADDRESS))
#define NBG2_SCROLL_Y_ADDRESS 0x31FF24u
#define NBG2_SCROLL_Y ((uint32_t *)board(NBG2_SCROLL_Y_ADDRESS))
#define SPRITE_SCREEN_1_LIST_JUMP_ADDRESS 0x400020u
#define SPRITE_SCREEN_1_LIST_JUMP BOARD16(SPRITE_SCREEN_1_LIST_JUMP_ADDRESS)
#define SPRITE_SCREEN_2_LIST_JUMP_ADDRESS 0x400030u
#define SPRITE_SCREEN_2_LIST_JUMP BOARD16(SPRITE_SCREEN_2_LIST_JUMP_ADDRESS)
#define SPRITE_LIST_1_A_ADDRESS 0x404000u
#define SPRITE_LIST_1_A BOARD16(SPRITE_LIST_1_A_ADDRESS)
#define SPRITE_LIST_1_A_HALFWAY_ADDRESS 0x407000u
#define SPRITE_LIST_1_A_HALFWAY BOARD16(SPRITE_LIST_1_A_HALFWAY_ADDRESS)
#define SPRITE_LIST_1_B_ADDRESS 0x40A000u
#define SPRITE_LIST_1_B BOARD16(SPRITE_LIST_1_B_ADDRESS)
#define SPRITE_LIST_1_B_HALFWAY_ADDRESS 0x40D000u
#define SPRITE_LIST_1_B_HALFWAY BOARD16(SPRITE_LIST_1_B_HALFWAY_ADDRESS)
#define SPRITE_LIST_2_A_ADDRESS 0x414000u
#define SPRITE_LIST_2_A BOARD16(SPRITE_LIST_2_A_ADDRESS)
#define SPRITE_LIST_2_A_HALFWAY_ADDRESS 0x417000u
#define SPRITE_LIST_2_A_HALFWAY BOARD16(SPRITE_LIST_2_A_HALFWAY_ADDRESS)
#define SPRITE_LIST_2_B_ADDRESS 0x41A000u
#define SPRITE_LIST_2_B BOARD16(SPRITE_LIST_2_B_ADDRESS)
#define SPRITE_LIST_2_B_HALFWAY_ADDRESS 0x41D000u
#define SPRITE_LIST_2_B_HALFWAY BOARD16(SPRITE_LIST_2_B_HALFWAY_ADDRESS)
#define SOUND_COMMAND_1_ADDRESS 0x700700u
#define SOUND_COMMAND_1 ((uint8_t *)board(SOUND_COMMAND_1_ADDRESS))
#define SOUND_SECOND_COMMAND_1_ADDRESS 0x700708u
#define SOUND_SECOND_COMMAND_1 ((uint8_t *)board(SOUND_SECOND_COMMAND_1_ADDRESS))
#define SOUND_ENGINE_REVS_1_ADDRESS 0x700710u
#define SOUND_ENGINE_REVS_1 ((uint8_t *)board(SOUND_ENGINE_REVS_1_ADDRESS))
#define SOUND_ENGINE_REQUESTS_1_ADDRESS 0x700712u
#define SOUND_ENGINE_REQUESTS_1 ((uint8_t *)board(SOUND_ENGINE_REQUESTS_1_ADDRESS))
#define SOUND_ENGINE_SAMPLE_1_ADDRESS 0x700713u
#define SOUND_ENGINE_SAMPLE_1 ((uint8_t *)board(SOUND_ENGINE_SAMPLE_1_ADDRESS))
#define SOUND_ENGINE_LOUDNESS_1_ADDRESS 0x700714u
#define SOUND_ENGINE_LOUDNESS_1 ((uint8_t *)board(SOUND_ENGINE_LOUDNESS_1_ADDRESS))
#define SOUND_TYRE_NOISE_1_ADDRESS 0x700718u
#define SOUND_TYRE_NOISE_1 BOARD8(SOUND_TYRE_NOISE_1_ADDRESS)
#define SOUND_COMMAND_2_ADDRESS 0x700800u
#define SOUND_COMMAND_2 BOARD8(SOUND_COMMAND_2_ADDRESS)
#define SOUND_ENGINE_REVS_2_ADDRESS 0x700810u
#define SOUND_ENGINE_REVS_2 BOARD8(SOUND_ENGINE_REVS_2_ADDRESS)
#define SOUND_ENGINE_REQUESTS_2_ADDRESS 0x700812u
#define SOUND_ENGINE_REQUESTS_2 BOARD8(SOUND_ENGINE_REQUESTS_2_ADDRESS)
#define BOOKKEEPING_CHECKSUM_ADDRESS 0x701F00u
#define BOOKKEEPING_CHECKSUM BOARD16(BOOKKEEPING_CHECKSUM_ADDRESS)
#define BOOKKEEPING_CHUTE_A_COINS_ADDRESS 0x701F06u
#define BOOKKEEPING_CHUTE_A_COINS BOARD16(BOOKKEEPING_CHUTE_A_COINS_ADDRESS)
#define BOOKKEEPING_TOTAL_TIME_ADDRESS 0x701F10u
#define BOOKKEEPING_TOTAL_TIME BOARD32(BOOKKEEPING_TOTAL_TIME_ADDRESS)
#define BOOKKEEPING_GAMES_A_ADDRESS 0x701F14u
#define BOOKKEEPING_GAMES_A BOARD16(BOOKKEEPING_GAMES_A_ADDRESS)
#define BOOKKEEPING_LONGEST_PLAY_ADDRESS 0x701F20u
#define BOOKKEEPING_LONGEST_PLAY BOARD16(BOOKKEEPING_LONGEST_PLAY_ADDRESS)
#define BOOKKEEPING_SHORTEST_PLAY_ADDRESS 0x701F22u
#define BOOKKEEPING_SHORTEST_PLAY BOARD16(BOOKKEEPING_SHORTEST_PLAY_ADDRESS)
#define BOOKKEEPING_HISTOGRAM_ADDRESS 0x701F24u
#define BOOKKEEPING_HISTOGRAM BOARD16(BOOKKEEPING_HISTOGRAM_ADDRESS)
#define BOOKKEEPING_COURSE_RECORDS_ADDRESS 0x701F32u
#define BOOKKEEPING_COURSE_RECORDS BOARD8(BOOKKEEPING_COURSE_RECORDS_ADDRESS)
#define BOOKKEEPING_SEAT_1_CREDITS_ADDRESS 0x701F74u
#define BOOKKEEPING_SEAT_1_CREDITS BOARD16(BOOKKEEPING_SEAT_1_CREDITS_ADDRESS)
#define BOOKKEEPING_SEAT_1_BYTE_ADDRESS 0x701F76u
#define BOOKKEEPING_SEAT_1_BYTE BOARD16(BOOKKEEPING_SEAT_1_BYTE_ADDRESS)
#define BOOKKEEPING_SEAT_2_CREDITS_ADDRESS 0x701F78u
#define BOOKKEEPING_SEAT_2_CREDITS BOARD16(BOOKKEEPING_SEAT_2_CREDITS_ADDRESS)
#define BOOKKEEPING_SEAT_2_BYTE_ADDRESS 0x701F7Au
#define BOOKKEEPING_SEAT_2_BYTE BOARD16(BOOKKEEPING_SEAT_2_BYTE_ADDRESS)
#define BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS 0x701FF0u
#define BOOKKEEPING_BACKUP_SEAT_1_CREDITS BOARD32(BOOKKEEPING_BACKUP_SEAT_1_CREDITS_ADDRESS)
#define BOOKKEEPING_BACKUP_SEAT_2_CREDITS_ADDRESS 0x701FF4u
#define BOOKKEEPING_BACKUP_SEAT_2_CREDITS BOARD32(BOOKKEEPING_BACKUP_SEAT_2_CREDITS_ADDRESS)
#define SOUND_PROGRAM_HOLD_ADDRESS 0x701FFFu
#define SOUND_PROGRAM_HOLD BOARD8(SOUND_PROGRAM_HOLD_ADDRESS)

/* data in the program ROM symbols.txt labels */
#define boot_e00000_bytes_ADDRESS 0x0000A4u
#define signature_text_ADDRESS 0x00010Cu
#define default_settings_ADDRESS 0x0004C0u
#define second_task_start_frame_ADDRESS 0x000C98u
#define isdn_connection_objects_ADDRESS 0x000F70u
#define isdn_connection_object_handlers_ADDRESS 0x000F74u
#define isdn_texts_ADDRESS 0x000F94u
#define car_presentation_objects_ADDRESS 0x001A78u
#define car_presentation_object_handlers_ADDRESS 0x001A7Cu
#define staff_credits_texts_ADDRESS 0x001AE8u
#define attract_car_name_texts_ADDRESS 0x001D0Cu
#define car_name_letter_palette_steps_ADDRESS 0x001D70u
#define ranking_objects_ADDRESS 0x002CB8u
#define name_entry_objects_ADDRESS 0x002D58u
#define name_entry_object_handlers_ADDRESS 0x002D5Cu
#define player_car_objects_ADDRESS 0x00346Cu
#define player_car_object_handlers_ADDRESS 0x003470u
#define race_objects_ADDRESS 0x003474u
#define race_object_handlers_ADDRESS 0x003478u
#define demo_race_objects_ADDRESS 0x0034C4u
#define demo_race_object_handlers_ADDRESS 0x0034C8u
#define route_map_objects_ADDRESS 0x0034ECu
#define route_map_object_handlers_ADDRESS 0x0034F0u
#define meaning_of_the_light_title_ADDRESS 0x003504u
#define thank_you_lcd_text_ADDRESS 0x005014u
#define welcome_letter_palette_steps_ADDRESS 0x006DCCu
#define transmission_panel_palette_steps_ADDRESS 0x006E18u
#define entry_lcd_text_ADDRESS 0x006EB0u
#define mega_driver_title_ADDRESS 0x006EB6u
#define rival_warning_text_ADDRESS 0x007CDBu
#define distance_offset_functions_ADDRESS 0x00AB04u
#define check_point_lcd_text_branch_0_ADDRESS 0x00CB98u
#define check_point_lcd_text_branch_1_ADDRESS 0x00CBACu
#define stage_level_text_records_ADDRESS 0x00D04Cu
#define total_time_text_ADDRESS 0x00D08Eu
#define branch_arrow_palette_steps_ADDRESS 0x00D0A0u
#define dream_flying_song_title_ADDRESS 0x00D0AEu
#define entry_invitation_objects_ADDRESS 0x00DF74u
#define entry_invitation_object_handlers_ADDRESS 0x00DF78u
#define network_check_text_ADDRESS 0x00DF84u
#define communication_success_text_ADDRESS 0x00DF98u
#define machine_id_text_ADDRESS 0x00DFB4u
#define communication_failed_text_ADDRESS 0x00DFCEu
#define stand_alone_machine_text_ADDRESS 0x00DFEAu
#define master_machine_text_ADDRESS 0x00E00Cu
#define slave_machine_text_ADDRESS 0x00E034u
#define isdn_machine_text_ADDRESS 0x00E055u
#define unused_race_copy_addresses_ADDRESS 0x00E07Cu
#define network_check_master_text_ADDRESS 0x00E10Cu
#define network_check_slave_text_ADDRESS 0x00E128u
#define network_check_isdn_text_ADDRESS 0x00E143u
#define board_check_text_ADDRESS 0x00E15Eu
#define wait_text_ADDRESS 0x00E170u
#define check_dualport_ram_characters_ADDRESS 0x00E17Au
#define check_network_status_text_ADDRESS 0x00E1B6u
#define check_trans_end_text_ADDRESS 0x00E1D6u
#define network_error_text_ADDRESS 0x00E40Eu
#define connection_error_texts_ADDRESS 0x00E422u
#define communication_board_error_texts_ADDRESS 0x00E464u
#define communication_time_out_texts_ADDRESS 0x00E484u
#define link_board_ic_text_records_ADDRESS 0x00E4F8u
#define segment_projection_pattern_ADDRESS 0x010509u
#define mixer_register_defaults_ADDRESS 0x010AB6u
#define sprite_control_rest_ADDRESS 0x010B0Cu
#define video_register_defaults_ADDRESS 0x010B12u
#define sprite_ram_start_entries_ADDRESS 0x010BF4u
#define last_colour_tables_ADDRESS 0x010C44u
#define bookkeeping_defaults_ADDRESS 0x011458u
#define seat1_first_chute_entry_ADDRESS 0x0114D4u
#define seat1_second_chute_entry_ADDRESS 0x011504u
#define seat2_first_chute_entry_ADDRESS 0x011534u
#define seat2_second_chute_entry_ADDRESS 0x011564u
#define screen_fade_from_black_ADDRESS 0x01173Eu
#define screen_fade_to_white_ADDRESS 0x011742u
#define screen_fade_from_white_ADDRESS 0x011746u
#define screen_fade_to_red_tint_ADDRESS 0x01174Au
#define screen_fade_from_red_tint_ADDRESS 0x01174Eu
#define car_hit_box_sizes_ADDRESS 0x0126BCu
#define radio_song_titles_ADDRESS 0x019478u
#define dj_mouth_shapes_ADDRESS 0x0196F8u
#define coin_prompt_lcd_strings_ADDRESS 0x01A9C8u
#define credit_free_play_text_ADDRESS 0x01AB62u
#define credit_singular_text_ADDRESS 0x01AB72u
#define credit_plural_text_ADDRESS 0x01AB80u
#define title_logo_flash_palettes_ADDRESS 0x01AC54u
#define title_logo_copy_flash_palettes_ADDRESS 0x01AC62u
#define timeline_1AFC0_ADDRESS 0x01AFC0u
#define test_mode_menu_text_ADDRESS 0x01C490u
#define test_mode_menu_heading_texts_ADDRESS 0x01C568u
#define test_mode_write_count_text_ADDRESS 0x01C5A2u
#define memory_test_page_text_ADDRESS 0x01CC9Cu
#define memory_test_now_testing_text_ADDRESS 0x01CD87u
#define memory_test_result_cells_ADDRESS 0x01CD96u
#define volume_adjustment_page_text_ADDRESS 0x01CF71u
#define volume_adjustment_controls_text_ADDRESS 0x01CFA0u
#define volume_adjusting_text_ADDRESS 0x01D021u
#define volume_adjustment_bar_cells_ADDRESS 0x01D030u
#define volume_adjustment_bars_text_ADDRESS 0x01D03Cu
#define input_test_page_text_ADDRESS 0x01D26Cu
#define input_test_bars_text_ADDRESS 0x01D30Au
#define input_test_items_ADDRESS 0x01D380u
#define input_test_bar_cells_ADDRESS 0x01D3D8u
#define output_test_page_text_ADDRESS 0x01D4F8u
#define sound_test_page_text_ADDRESS 0x01D788u
#define crt_test_first_page_text_ADDRESS 0x01D9C7u
#define crt_test_second_page_text_ADDRESS 0x01D9DCu
#define game_assignments_page_text_ADDRESS 0x01DBF8u
#define game_assignments_setting_places_ADDRESS 0x01DD14u
#define coin_assignments_page_text_ADDRESS 0x01E0F4u
#define coin_manual_page_text_ADDRESS 0x01E13Fu
#define coin_assignments_line_text_ADDRESS 0x01E20Eu
#define coin_setting_description_records_ADDRESS 0x01E28Cu
#define no_bonus_adder_text_ADDRESS 0x01E39Eu
#define bonus_free_play_text_ADDRESS 0x01E3BCu
#define bonus_adder_text_ADDRESS 0x01E3CBu
#define coin_assignments_setting_places_ADDRESS 0x01E44Du
#define network_assignments_page_text_ADDRESS 0x01E62Cu
#define privilege_mode_text_lists_ADDRESS 0x01E698u
#define network_assignments_setting_places_ADDRESS 0x01E6E0u
#define time_digit_steps_ADDRESS 0x01E970u
#define bookkeeping_first_page_text_ADDRESS 0x01EA5Cu
#define bookkeeping_second_page_text_ADDRESS 0x01EBB9u
#define car_colour_names_ADDRESS 0x01ED94u
#define backup_data_clear_page_text_ADDRESS 0x01EE79u
#define backup_data_clear_completed_text_ADDRESS 0x01EEB2u
#define eeprom_caution_text_ADDRESS 0x01EF12u
#define exit_prompt_text_ADDRESS 0x01F2BCu
#define continue_prompt_text_ADDRESS 0x01F2DEu
#define test_mode_palettes_1_ADDRESS 0x01F304u
#define test_mode_palettes_2_ADDRESS 0x01F506u
#define unused_words_01F706_ADDRESS 0x01F706u
#define unused_fill_01F714_ADDRESS 0x01F714u
#define traffic_records_ADDRESS 0x030B60u
#define boot_palette_9e00_ADDRESS 0x03153Cu
#define black_background_palette_ADDRESS 0x031740u
#define vector_table_ADDRESS 0x03F000u
#define zero_block_ADDRESS 0x100800u
#define one_bit_font_ADDRESS 0x103C20u
#define text_font_ADDRESS 0x104020u
#define indirect_colour_tables_ADDRESS 0x107020u
#define solid_colour_blocks_ADDRESS 0x12297Cu
#define boot_palette_8000_ADDRESS 0x153904u

/* numbers the code uses, by name (symbols.txt constant lines) */
#define TIME_DIGITS 0x3u
#define PLACE_NUMBER 0x4u
#define PLACE_COUNT 0x5u
#define REV_METER 0x6u
#define SHIFT_UP_HINT 0xEu
#define SPEED_UNIT_KMH 0x10u
#define SPEED_DIGITS 0x12u
#define CHOICE_FLAG 0x1Bu
#define LAP_TIMER 0x2Du
#define STAGE_INDICATOR 0x2Eu
#define CRASH_TEXT 0x2Fu
#define EXTEND_TIME 0x33u
#define BRANCH_ARROW_LEFT 0x34u
#define BRANCH_ARROW_RIGHT 0x35u
#define CHECKPOINT_BOARD_ROW 0x36u
#define WEST_EAST_PANEL 0x39u
#define SECONDS_PER_MINUTE 0x3Cu
#define ACCELERATION_SMOKE 0x44u
#define PALM_TREE 0x59u
#define SWORDFISH 0x165u
#define CYCLIST 0x185u
#define ELEPHANT 0x258u
#define TITLE_BOARD_PALMS 0x25Au
#define TITLE_SEAGULL 0x25Bu
#define TITLE_LOGO 0x25Du
#define TITLE_LOGO_COPY 0x25Eu
#define TITLE_LOGO_STRIPE 0x25Fu
#define ATTENDANT_WALKING 0x266u
#define AUTOMATIC_POSE 0x267u
#define MANUAL_POSE 0x268u
#define STANDING 0x269u
#define CAR_CAPTION_LETTERS 0x26Eu
#define ENTRY_COUNTDOWN_DIGITS 0x26Fu
#define WAITING_BANNER 0x28Bu
#define WAIT_A_MINUTE_BANNER 0x28Du
#define SELECT_CAR_HEADING 0x2B5u
#define AUTOMATIC_PANEL 0x2BDu
#define MANUAL_PANEL 0x2BEu
#define NOVICE_LABEL 0x2C1u
#define GEARS_LABEL 0x2C9u
#define CAR_GEARS_CAPTION 0x2D1u
#define RANKING_TITLE_LEFT 0x2D2u
#define RANKING_TITLE_RIGHT 0x2D3u
#define NAME_ENTRY_FONT 0x2DEu
#define GAME_OVER_BANNER 0x2F4u
#define CONGRATULATIONS 0x2F5u
#define WINNER 0x2F6u
#define LOSER 0x2F7u
#define RACE_PLACES 0x2F8u
#define ROUTE_MAP_LEG 0x306u
#define CAR_NAME_FONT 0x346u
#define SANTA_SLEIGH 0x3A0u
#define ORCA 0x3A2u
#define SECONDS_PER_HOUR 0xE10u
#define EEPROM_WRITE_LIMIT 0x2710u
#define SECONDS_PER_DAY 0x15180u
#define BOARD_OFF_LOOPS 0x16000u
#define LOW_ROM_TESTED_BYTES 0x3FFF8u
#define HIGH_ROM_TESTED_BYTES 0xFF000u

/* the game's structures in work RAM */
#pragma pack(push, 1)
struct object
{
	uint8_t flags;
	uint8_t flags_high;
	union
	{
		struct
		{
			uint16_t flags_upper;
			uint32_t handler;
			uint8_t slot;
			uint8_t number;
			uint8_t car;
			uint8_t car_colour;
			uint8_t state;
			uint8_t pad_0D[0x7];
			uint32_t parent;
			uint32_t parameter;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x2];
			uint16_t position_fraction;
			uint8_t pad_2A[0x6];
			uint16_t x;
			uint16_t y;
			uint16_t width_scale;
			uint16_t height_scale;
			uint16_t animation;
			uint16_t palette;
			uint16_t depth_layer;
			uint8_t draw_mode;
			int8_t side;
			uint8_t frame;
			uint8_t frame_time;
			int8_t frame_countdown;
			uint8_t size_step;
			uint8_t scale_shift;
			uint8_t pad_45[0xB];
			uint32_t previous_place;
			uint8_t pad_54[0x2C];
			uint32_t list_data;
			uint8_t pad_84[0x7C];
		};
		struct
		{
			uint8_t pad_02[0xE];
			int16_t end_position;
			uint8_t countdown;
			uint8_t pad_13[0x5];
			uint32_t length;
		} ambient_sound;
		struct
		{
			uint8_t pad_02[0x3F];
			uint8_t frame_time;
			int8_t frame_countdown;
		} animation_view;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t animation_frames;
			uint8_t pad_0A[0x6];
			uint8_t countdown;
			uint8_t pad_11[0x7];
			uint32_t animation_list;
		} aurora;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t choice;
			uint32_t palette_list;
			uint8_t pad_1C[0x4];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
		} automatic_panel;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t leaving;
			uint8_t pad_0E[0x12];
			uint16_t panorama_x;
			uint8_t pad_22[0x2];
			uint16_t height;
			uint8_t pad_26[0x2];
			uint16_t half_width;
			uint8_t pad_2A[0x56];
			uint32_t data;
		} backdrop;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t leaving;
			uint8_t pad_0E[0x2];
			uint16_t other_palette;
			uint8_t pad_12[0xE];
			uint16_t panorama_x;
			uint8_t pad_22[0x2];
			uint16_t height;
			uint8_t pad_26[0x2];
			uint16_t half_width;
			uint8_t pad_2A[0x56];
			uint32_t data;
		} backdrop_flashing;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t leaving;
			uint8_t pad_0E[0x2];
			uint16_t approach_start;
			uint8_t pad_12[0xE];
			uint16_t panorama_x;
			uint8_t pad_22[0x2];
			uint16_t height;
			uint8_t pad_26[0x2];
			uint16_t half_width;
			uint8_t pad_2A[0x56];
			uint32_t data;
		} backdrop_landmark;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t leaving;
			uint8_t pad_0E[0x2];
			uint8_t frame_count;
			uint8_t pad_11[0xF];
			uint16_t panorama_x;
			uint8_t pad_22[0x2];
			uint16_t height;
			uint8_t pad_26[0x2];
			uint16_t half_width;
			uint8_t pad_2A[0x56];
			uint32_t data;
		} backdrop_translucent;
		struct
		{
			uint8_t pad_02[0xE];
			int16_t speed_x;
			int16_t speed_y;
			uint8_t pad_14[0x6C];
			int16_t target_x;
			int16_t target_y;
			uint32_t scales;
		} bar_picture;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t leave_segment;
			uint8_t pad_12[0x6];
			uint32_t palette_list;
		} branch_arrow;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t car_select;
			uint8_t pad_18[0x4];
			uint16_t size;
			uint8_t pad_1E[0x2];
			int16_t side_offset;
			uint8_t pad_22[0xA];
			uint16_t angle;
			uint16_t radius;
		} car_attendant;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t shown_car;
			uint8_t pad_0A[0x6];
			uint16_t timer;
			uint8_t pad_12[0x2];
			uint32_t car_select;
		} car_caption;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t shown_car;
			uint8_t pad_0A[0x6];
			uint16_t timer;
			uint8_t pad_12[0x2];
			uint32_t car_select;
		} car_gears_caption;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t character;
			uint8_t pad_0A[0x6];
			uint8_t delay;
			uint8_t slide_frames;
			uint16_t frames_left;
			uint8_t pad_14[0x4];
			uint32_t palette_list;
		} car_name_letter;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t timer;
			uint8_t palette_step;
			uint8_t pad_13[0x1];
			uint32_t staff_credits_code;
		} car_presentation;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t car_number;
			uint8_t stage;
			uint8_t queued_message;
			uint8_t pad_0C[0x1];
			uint8_t talk_step;
			uint8_t pad_0E[0x2];
			uint8_t timer;
			uint8_t pad_11[0x7];
			uint32_t title;
			uint8_t pad_1C[0x10];
			uint16_t buttons;
			uint8_t pad_2E[0xA];
			uint32_t remark_list;
			uint8_t pad_3C[0x44];
			uint8_t car_models[0x8];
			uint8_t cars_to_introduce;
			uint8_t next_car_number;
		} car_radio;
		struct
		{
			uint8_t pad_02[0x7];
			int8_t turn;
			int8_t turn_delay;
			uint8_t confirm_delay;
			uint8_t pad_0C[0x1];
			uint8_t gas_down;
			uint8_t rowscroll_copies;
			uint8_t choice_turn;
			uint16_t timer;
			uint8_t pad_12[0xA];
			uint16_t time_left;
			uint8_t pad_1E[0x6];
			uint16_t angle;
			uint8_t pad_26[0x2];
			uint16_t top_scroll;
			uint8_t pad_2A[0x2];
			uint8_t car_chosen;
			uint8_t transmission_chosen;
			uint8_t pad_2E[0x6A];
			uint32_t scroll_x_address;
			uint32_t scroll_y_address;
			uint32_t rowscroll_address;
			uint32_t rowselect_address;
			uint32_t second_scroll_x_address;
			uint32_t second_scroll_y_address;
			uint8_t entry_over;
		} car_select;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t own_number;
			uint8_t digit_toggle;
			uint8_t blink_timer;
			uint8_t pad_0C[0x4];
			uint8_t frames;
			uint8_t pad_11[0x1];
			uint8_t flags_visible;
			uint8_t pad_13[0x1];
			uint32_t car_select;
			uint32_t race_order;
			uint8_t pad_1C[0x24];
			uint32_t shown_players;
			uint8_t pad_44[0x3C];
			uint32_t car_flags[0x8];
		} car_select_entries;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t checkpoints_seen;
			uint8_t checkpoint;
			uint8_t pad_0C[0x1];
			uint8_t blink_timer;
			uint8_t blink_on;
			uint8_t rows_shown;
			uint16_t leave_segment;
			uint8_t row;
			uint8_t rows_left;
			uint32_t passing_order;
			uint32_t checkpoint_count;
			uint16_t leader_time;
			uint8_t pad_1E[0x62];
			uint32_t car_sprites[0x8];
			uint16_t row_offsets[0x8];
			uint8_t pad_B0[0x40];
			uint8_t row_steps[0x8];
		} checkpoint_board;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t player;
			uint8_t car;
			uint8_t pad_0B[0x7];
			uint8_t visible;
			uint8_t pad_13[0x1];
			uint32_t entries;
			uint32_t top_flag;
			uint8_t pad_1C[0x8];
			uint16_t angle;
		} choice_flag;
		struct
		{
			uint8_t pad_02[0x10];
			uint8_t visible;
			uint8_t pad_13[0x1];
			uint32_t flag;
		} choice_flag_number;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t message;
			uint8_t next_car;
			uint8_t car;
			uint8_t pad_0C[0x4];
			uint8_t round_frames;
		} coin_prompt;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t car;
			uint8_t car_colour;
		} coin_prompt_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t message;
			uint8_t pad_0A[0x1];
			uint8_t car_colour;
			uint8_t pad_0C[0x4];
			uint8_t frames;
			uint8_t pad_11[0x3];
			uint32_t prompt;
		} coin_prompt_message;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t digit_kind;
			uint8_t digit;
		} credit_display;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t crest;
			uint8_t pad_11[0x27];
			uint32_t clip_entry;
			uint8_t pad_3C[0x44];
			uint16_t clip_command;
			uint16_t clip_bottom;
			uint16_t clip_left;
			uint16_t clip_right;
		} crest_clip;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t record;
			uint8_t pad_0A[0x6];
			uint16_t frames_left;
		} demo_caption;
		struct
		{
			uint8_t pad_02[0xE];
			uint32_t frames;
			uint8_t pad_14[0x6C];
			uint32_t timeline;
		} demo_captions;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t left_side;
		} driver_couple;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t blink;
		} entry_banner;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t copies;
			uint8_t pad_11[0x1];
			uint8_t car;
		} entry_car_ring;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t car_ring;
		} entry_countdown;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t delay;
		} entry_traffic;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t car;
			uint8_t pad_0A[0x6];
			uint16_t path_step;
			uint16_t spin_delay;
		} entry_traffic_car;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
			uint8_t pad_11[0x1];
			uint8_t blink;
		} extend_time_message;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t pad_12[0x6];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			uint16_t in_view_animation;
		} flagman;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t spacing;
			uint16_t count;
			uint8_t pad_14[0x4];
			uint32_t palette_cycle;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
			uint8_t pad_40[0x5];
			uint8_t size_table;
			uint8_t pad_46[0x10];
			int16_t first_position;
			int16_t end_position;
			uint8_t pad_5A[0x6];
			uint32_t hit_box;
		} flashing_scenery;
		struct
		{
			uint8_t pad_02[0x1A];
			int16_t position;
			uint8_t pad_1E[0x38];
			int16_t start_position;
			int16_t end_position;
			uint8_t pad_5A[0x26];
			int32_t direction;
		} fork_branch;
		struct
		{
			uint8_t pad_02[0x1A];
			int16_t position;
			uint8_t pad_1E[0x62];
			uint16_t lift_step;
			uint16_t tilt_step;
			uint16_t lift_height;
			uint16_t lower_position;
		} fork_camera;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t timer;
		} game_over_banner;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t redline_frames;
			uint8_t pad_12[0x6E];
			uint8_t hint_started;
		} gear_indicator;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t timer;
			uint8_t pad_12[0xA];
			int16_t position;
			uint8_t pad_1E[0x62];
			uint16_t text_x;
			uint16_t text_y;
			uint16_t text_attribute;
		} goal;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t counter;
			uint8_t pad_12[0x2];
			uint32_t scene;
			uint8_t pad_18[0x29];
			uint8_t frame_time;
			int8_t frame_countdown;
			uint8_t pad_43[0x3D];
			uint32_t script;
			uint32_t step;
		} goal_actor;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t pad_12[0x6];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0xA];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			int16_t x_speed;
		} goal_parade;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t piece;
			uint8_t pad_0A[0x6];
			uint16_t frames_left;
		} ignition_key_prompt;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t blink_counter;
		} isdn_status;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t pad_12[0x6];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x6];
			uint16_t position_fraction;
			uint8_t pad_2A[0x6];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			uint16_t hop_animation;
			uint16_t trigger_distance;
			int16_t x_speed;
		} kangaroo;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t blink_count;
		} kanji_text_viewer;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t checkpoints;
			uint8_t pad_0B[0x5];
			uint16_t split_frames;
			uint8_t pad_12[0x6E];
			uint16_t text_x;
			uint16_t text_y;
			uint16_t text_attribute;
		} lap_timer;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t racer_count;
			uint8_t pad_0A[0x3];
			uint8_t page;
			uint8_t page_step;
			uint8_t pad_0F[0x1];
			uint16_t frames;
			uint16_t page_frames;
			uint8_t pad_14[0x4];
			uint32_t order_list;
		} lcd_race_info;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t choice;
			uint32_t palette_list;
			uint8_t pad_1C[0x4];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
		} manual_panel;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t pad_12[0x6];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0xA];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			uint16_t trigger_distance;
			int16_t x_speed;
			int16_t y_speed;
			int16_t road_speed;
		} moving_scenery;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t open;
			uint8_t letter;
			uint8_t place;
			uint8_t pad_0C[0x1];
			uint8_t gas_step;
			uint8_t pad_0E[0x2];
			uint16_t timer;
			uint16_t spin_frames;
			uint8_t pad_14[0x18];
			uint16_t wheel_angle;
			uint8_t pad_2E[0x6A];
			int8_t turn_steps;
			int8_t turn_delay;
			uint16_t name_x;
			uint16_t name_y;
			uint16_t name_attribute;
			uint8_t name[0x4];
			uint16_t time_x;
			uint16_t time_y;
			uint16_t time_attribute;
			uint8_t time_text;
		} name_entry;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t name_entry;
		} name_entry_lcd;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x8];
			uint16_t spin_frames;
			uint32_t name_entry;
			uint8_t pad_18[0x4];
			int32_t width_factor;
			int32_t x_offset;
			int32_t y_offset;
			int32_t depth;
			uint16_t angle;
		} name_entry_letter;
		struct
		{
			uint8_t pad_02[0x5A];
			uint8_t screen_centered;
		} overhead_scenery;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t running;
			uint8_t pad_0E[0x2];
			uint16_t delay;
			uint16_t frames_left;
			uint16_t step_frames;
			uint16_t step_countdown;
			uint8_t pad_18[0x20];
			uint32_t palette_list;
			uint8_t pad_3C[0x44];
			uint32_t data;
		} palette_cycle;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t delay;
			uint8_t pad_11[0x1];
			uint8_t period;
			uint8_t colours_left;
			uint8_t pad_14[0x4];
			uint32_t from;
			uint32_t to;
			uint32_t colour_place;
			uint32_t difference_place;
			uint16_t upload_length;
			uint8_t pad_2A[0x2];
			uint32_t upload_colours;
			uint16_t palette_offset;
			uint16_t blocks;
		} palette_fade;
		struct
		{
			uint8_t pad_02[0xE];
			int16_t step;
		} passing_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t path_steps;
			uint8_t pad_0A[0x6];
			uint16_t wait;
		} passing_traffic;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t place;
			uint8_t pad_0A[0xA];
			uint32_t number;
		} place_count;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t place;
			uint8_t pad_0A[0x6];
			uint8_t frames;
			uint8_t pad_11[0x1];
			uint8_t blink_timer;
		} place_number;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t step;
			uint16_t state_high;
			uint16_t timer;
			uint8_t pad_12[0xE];
			int32_t world_x;
			int32_t world_y;
			int32_t world_z;
			uint16_t speed;
			int16_t sideways_speed;
			uint8_t pad_30[0x11];
			uint8_t frame_time;
			int8_t frame_countdown;
			uint8_t pad_43[0x25];
			uint16_t hit_other;
			uint8_t pad_6A[0x4];
			uint8_t hit_kind;
			uint8_t hit_guard;
			uint8_t pad_70[0x1C];
			uint16_t pitch;
			uint16_t heading;
			uint16_t previous_heading;
			uint8_t pad_92[0x2];
			int16_t acceleration;
			int16_t steering;
			uint8_t pad_98[0x8];
			int32_t view_target_x;
			int32_t view_target_y;
			int32_t view_target_z;
			uint8_t pad_AC[0x4];
			uint16_t brake_force;
			uint16_t last_negated_speed;
			int32_t previous_world_y;
			int16_t slide;
			uint8_t pad_BA[0x6];
			uint32_t impact_speed;
			int32_t speed_change;
			int32_t bounce_height;
			int16_t bounce_speed;
			int16_t sideways_push;
			uint8_t gas;
			uint8_t brake;
			uint8_t wheel;
			uint8_t pad_D3[0x6];
			uint8_t view_height;
			uint8_t place;
			uint8_t steer_frame;
			uint8_t pad_DC[0x4];
			uint8_t rival_step;
			uint8_t rival_near;
			int16_t nod;
			uint8_t rival_hold;
			uint8_t pad_E5[0xB];
			uint16_t stop_position;
			int16_t stop_x;
			uint16_t stop_speed;
			uint8_t stop_wheel;
			uint8_t stop_gas;
			uint8_t finished;
		} player_car;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t leader;
			uint32_t record;
			uint8_t pad_1C[0x68];
			uint16_t stop_distance;
		} pulled_vehicle;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t change_step;
			uint8_t page_half;
			uint8_t background;
			uint8_t pad_10[0x8];
			uint32_t scroll_x_address;
			uint32_t scroll_y_address;
			uint32_t page_word_address;
			uint32_t tile_bank_address;
			uint32_t next_page_words;
			uint32_t page_list;
		} race_background;
		struct
		{
			uint8_t pad_02[0xE];
			uint32_t count;
			uint32_t background;
			uint8_t pad_18[0x68];
			uint32_t fades;
		} race_background_fade_watch;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t delay;
			uint8_t pad_11[0x1];
			uint8_t step;
			uint8_t pad_13[0xD];
			uint32_t page_word_address;
		} race_background_lower_animation;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t delay;
			uint8_t pad_11[0x1];
			uint8_t step;
			uint8_t pad_13[0xD];
			uint32_t page_word_address;
		} race_background_upper_animation;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t player;
			uint8_t model;
			uint8_t pad_0B[0x23];
			uint8_t special_body;
			uint8_t pad_2F[0x51];
			uint32_t record;
			uint16_t body_attributes;
			uint8_t pad_86[0x2];
			uint32_t wheel_turn;
			uint16_t first_body_piece;
			uint16_t first_body_animation;
			uint32_t driver_places;
			uint16_t driver_animation;
			uint16_t driver_palette;
			uint8_t driver_gesture_mask;
			uint8_t pad_99[0x1];
			uint8_t driver_frame;
			uint8_t driver_frames;
			uint8_t pad_9C[0x4];
			uint32_t passenger_places;
			uint16_t passenger_animation;
			uint16_t passenger_palette;
			uint8_t passenger_gesture_mask;
			uint8_t pad_A9[0x1];
			uint8_t passenger_frame;
			uint8_t passenger_frames;
			uint8_t pad_AC[0x4];
			int16_t crash_text_y;
			uint8_t crash_text_frames;
			uint8_t crash_text_rise;
			uint16_t crash_text_animation;
			uint16_t crash_text_palette;
			uint16_t tag_palette;
			uint16_t tag_animation;
			uint8_t blink_counter;
			uint8_t rival;
			uint8_t pad_BE[0x1];
			uint8_t hard_acceleration_frame;
			uint32_t spray_2_animations;
			uint32_t spray_2_places;
			uint32_t spray_2_effect;
			uint8_t spray_2_frame;
			uint8_t spray_2_side;
			uint8_t spray_2_kind;
			uint8_t pad_CF[0x1];
			uint32_t spray_1_animations;
			uint32_t spray_1_places;
			uint32_t spray_1_effect;
			uint8_t spray_1_frame;
			uint8_t spray_1_side;
			uint8_t spray_1_kind;
			uint8_t pad_DF[0x2];
			uint8_t gesture_shown;
			uint8_t last_place;
			uint8_t view;
		} race_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t occupant;
			uint8_t body_back;
			uint8_t pad_0B[0x9];
			uint16_t car;
		} race_car_occupant;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t sound_step;
			uint8_t camera_step;
			uint8_t pad_0F[0x1];
			uint8_t count;
			uint8_t pad_11[0x1];
			uint8_t next_count;
			uint8_t pad_13[0x5];
			uint32_t sound_list;
		} race_opening;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t frames;
		} race_result;
		struct
		{
			uint16_t course_over;
			uint8_t pad_04[0xC];
			uint8_t courses_shown;
			uint8_t pad_11[0x1];
			uint16_t frames_left;
		} ranking;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t courses_shown;
			uint8_t pad_11[0x3];
			uint32_t ranking;
			uint8_t pad_18[0x68];
			uint32_t tile_bank_address;
			uint32_t pages_address;
			uint32_t scroll_x_address;
			uint32_t scroll_y_address;
			uint32_t zoom_x_address;
			uint32_t zoom_y_address;
			uint32_t zoom_center_x_address;
			uint32_t zoom_center_y_address;
			uint32_t cleared_scroll_x_address;
			uint32_t cleared_scroll_y_address;
			uint32_t second_scroll_x_address;
			uint32_t second_scroll_y_address;
			uint32_t page_list;
		} ranking_background;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t course;
			uint8_t pad_0A[0x6];
			uint16_t frames_left;
			uint8_t pad_12[0x2];
			uint32_t ranking;
		} ranking_course_banner;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t course;
			uint8_t pad_0A[0x76];
			uint8_t text[0x10];
		} ranking_record;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t course;
			uint8_t pad_0A[0x6];
			uint16_t wait_frames;
		} ranking_record_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t course;
			uint8_t pad_0B[0x5];
			uint8_t slide_steps;
			uint8_t pad_11[0x1];
			uint8_t colour_delay;
			uint8_t pad_13[0x1];
			uint32_t ranking_record;
			uint8_t pad_18[0x6A];
			uint16_t frames_left;
		} ranking_record_character;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t rider;
			uint8_t model;
			uint8_t player;
			uint8_t pad_0C[0x8];
			uint16_t car;
		} rider_gesture;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
		} rival_alarm;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
			uint8_t pad_11[0x1];
			uint8_t blinks;
		} rival_warning;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t pad_12[0x6];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0xA];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			uint16_t crossing_animation;
			uint16_t trigger_distance;
			int16_t x_speed;
		} road_crosser;
		struct
		{
			uint8_t pad_02[0x7E];
			uint32_t stream;
			uint16_t stage_start;
			uint8_t stage;
		} road_object_stream;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t spacing;
			uint16_t count;
			uint8_t pad_14[0x4];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x2];
			uint16_t position_fraction;
			uint8_t pad_2A[0x15];
			int8_t side;
			uint8_t pad_40[0x5];
			uint8_t size_table;
			uint8_t pad_46[0xA];
			uint32_t previous_sprites;
			uint8_t pad_54[0x2];
			int16_t first_position;
			int16_t end_position;
			uint8_t pad_5A[0x2];
			uint8_t screen_centered;
			uint8_t pad_5D[0x3];
			uint32_t hit_box;
		} road_row;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t first_tiles_flag;
			uint8_t second_tiles_flag;
			uint8_t pad_0C[0x1];
			uint8_t page_set;
			uint8_t segment_page_set;
			uint8_t points_buffer;
			uint16_t near_scroll;
			uint8_t event;
			uint8_t last_event;
			uint16_t segment;
			uint16_t last_segment;
			uint32_t page_table[0xF];
			uint32_t page_register;
			uint32_t line_scroll_table;
			uint32_t line_select_table;
			uint32_t skipped_line;
			uint32_t skipped_x;
			uint32_t skipped_depth;
			uint32_t skipped_x_step;
			uint32_t skipped_depth_step;
			uint32_t skipped_scroll_place;
			uint32_t skipped_select_place;
			uint32_t skipped_stripe;
			uint8_t event_delay;
		} road_surface;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t table_half;
			uint8_t page_set;
			uint8_t pad_0B[0x5];
			uint8_t step;
			uint8_t pad_11[0x1];
			uint8_t pages_left;
			uint8_t pad_13[0x1];
			uint32_t road;
			uint32_t source_pages[0x4];
			uint32_t target_pages[0x4];
			uint32_t data;
			uint32_t flag;
		} road_tiles_change;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t first_branch;
			uint8_t pad_0A[0x1];
			uint8_t last_leg_progress;
			uint8_t pad_0C[0x4];
			uint8_t timer;
			int8_t leg_frames;
			uint8_t first_legs_left;
			uint8_t checkpoint_legs_left;
			uint8_t pad_14[0x4];
			uint32_t sea_colour_address;
			uint8_t pad_1C[0xC];
			uint32_t stage;
			uint8_t pad_2C[0x54];
			uint32_t pages_address;
			uint32_t scroll_x_address;
			uint32_t scroll_y_address;
			uint32_t zoom_x_address;
			uint32_t zoom_y_address;
			uint32_t zoom_center_x_address;
			uint32_t zoom_center_y_address;
			uint32_t register_scroll_x_address;
			uint32_t register_scroll_y_address;
			uint32_t tile_bank_address;
			uint32_t second_pages_address;
			uint32_t second_scroll_x_address;
			uint32_t second_scroll_y_address;
			uint32_t second_register_scroll_x_address;
			uint32_t second_register_scroll_y_address;
			uint32_t second_tile_bank_address;
		} route_map;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t model;
			uint8_t pad_0A[0xA];
			uint32_t route_map;
		} route_map_car;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t kind;
			uint8_t pad_0B[0x5];
			uint16_t blink;
			uint8_t pad_12[0x2];
			uint32_t leg;
		} route_map_checkpoint;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t colour_timer;
			uint8_t pad_12[0xAE];
			uint16_t palette_base;
		} route_map_direction_sign;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index_then_half_rate;
			uint8_t pad_0A[0x6];
			uint8_t toggle;
			uint8_t pad_11[0x3];
			uint32_t route_map;
			uint8_t pad_18[0x6C];
			int16_t x_offset;
			uint8_t pad_86[0x2];
			int16_t y_offset;
		} route_map_landmark;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t leg;
			uint8_t progress;
			uint8_t pad_0B[0x9];
			uint32_t map;
		} route_map_leg;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t leg;
		} route_map_leg_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t kind;
			uint8_t pad_0A[0x6];
			uint16_t blink;
			uint8_t pad_12[0x2];
			uint32_t route_map;
			uint8_t pad_18[0x6C];
			int16_t x_offset;
			uint8_t pad_86[0x2];
			int16_t y_offset;
		} route_map_sign;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t timer;
		} route_map_wait_banner;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t sound_countdown;
			uint8_t pad_13[0x5];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0xA];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x28];
			int16_t x_speed;
			uint16_t sound;
		} santa_sleigh;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t timer;
			uint8_t pad_12[0x2];
			uint32_t car_select;
		} select_car_heading;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t timer;
			uint8_t pad_12[0x2];
			uint32_t car_select;
			uint16_t piece_animation;
		} select_instruction;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t gear;
			uint8_t pad_0A[0x6];
			uint8_t blink;
			uint8_t pad_11[0x3];
			uint32_t indicator;
		} shift_up_hint;
		struct
		{
			uint8_t pad_02[0xB];
			uint8_t colour_step;
			uint8_t pad_0E[0x2];
			int16_t time_left;
			uint16_t speed;
			uint8_t pad_14[0x4];
			uint32_t text;
			uint8_t pad_1C[0x4];
			int16_t slide;
			uint16_t y;
		} song_title;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t spacing;
			uint16_t count;
			uint8_t pad_14[0x4];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
			uint8_t pad_40[0x5];
			uint8_t size_table;
			uint8_t pad_46[0x10];
			int16_t first_position;
			int16_t end_position;
			uint8_t pad_5A[0x6];
			uint32_t sound_entry;
			uint8_t pad_64[0x1C];
			uint16_t sound;
			uint8_t sound_countdown;
		} sounding_scenery;
		struct
		{
			uint8_t pad_02[0x1E];
			uint16_t units_x;
			uint8_t pad_22[0x2];
			uint16_t digits_y;
		} speedometer;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t player;
			uint8_t car;
			uint8_t pad_0B[0x7];
			uint8_t visible;
			uint8_t pad_13[0x1];
			uint32_t below;
		} stacked_choice_flag;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t controls_held;
			uint8_t pad_0A[0x6];
			uint16_t frames_left;
			uint8_t pad_12[0x2];
			uint32_t presentation;
			uint32_t code_place;
		} staff_credits_code;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t stage;
			uint8_t pad_0A[0x6];
			uint8_t countdown;
			uint8_t pad_11[0x7];
			uint32_t sound_list;
		} stage_end_sounds;
		struct
		{
			uint8_t pad_02[0x1A];
			int16_t position;
		} stage_entry;
		struct
		{
			uint8_t pad_02[0x8];
			uint8_t last;
			uint8_t pad_0B[0x15];
			uint32_t place_address;
			uint8_t pad_24[0x4];
			uint32_t stage;
		} stage_result_line;
		struct
		{
			uint8_t pad_02[0x10];
			uint16_t rise_speed;
			uint8_t pad_14[0x4];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
		} start_balloon;
		struct
		{
			uint8_t pad_02[0x16];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
			uint8_t pad_40[0x40];
			uint16_t go_animation;
		} start_flagman;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t spacing;
			uint16_t count;
			uint8_t pad_14[0x4];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x19];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t first_position;
			int16_t end_position;
			uint8_t pad_5A[0x6];
			uint32_t hit_box;
		} start_gate;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
		} start_message;
		struct
		{
			uint8_t pad_02[0x16];
			uint32_t palette_list;
		} step_palette;
		struct
		{
			uint8_t pad_02[0xE];
			uint32_t count;
			uint8_t pad_14[0x4];
			uint32_t fades;
			uint8_t pad_1C[0x1C];
			uint32_t fade_list;
			uint8_t pad_3C[0x44];
			uint32_t data;
		} sunset_fade;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t draw_distance;
			uint8_t dive_countdown;
			uint8_t pad_13[0x5];
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0xA];
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x16];
			int16_t appear_position;
			int16_t trigger_position;
			uint8_t pad_5A[0x26];
			uint16_t trigger_distance;
			int16_t x_speed;
			int16_t y_speed;
			int16_t road_speed;
			uint8_t dive_frames;
			uint8_t pad_89[0x1];
			int16_t dive_y_speed;
		} swooping_helicopter;
		struct
		{
			uint8_t pad_02[0x1E];
			uint16_t units_x;
			uint8_t pad_22[0x5E];
			int8_t units_flip;
			uint8_t units_digit;
			uint16_t units_width;
			uint32_t units_remaining;
			int8_t tens_flip;
			uint8_t tens_digit;
			uint16_t tens_width;
			uint32_t tens_remaining;
			int8_t hundreds_flip;
			uint8_t hundreds_digit;
			uint16_t hundreds_width;
			uint32_t hundreds_remaining;
		} time_digits;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
		} title_board_palms;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x6];
			uint16_t frames;
			uint8_t pad_12[0xE];
			int16_t position;
			uint8_t pad_22[0x2];
			uint16_t rest_y;
			uint8_t pad_26[0x6];
			uint16_t speed;
		} title_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t model;
			uint8_t pad_0A[0x6];
			int16_t x_offset;
			int16_t y_offset;
			uint32_t car;
		} title_car_driver;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t model;
			uint8_t pad_0A[0x6];
			int16_t x_offset;
			int16_t y_offset;
			uint32_t car;
		} title_car_passenger;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
			uint8_t pad_11[0x6F];
			uint32_t palette_list;
		} title_logo;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x6];
			uint8_t delay;
		} title_logo_car;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t frames;
			uint8_t pad_11[0x6F];
			uint32_t palette_list;
		} title_logo_copy;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x6];
			uint8_t frames;
		} title_logo_stripe;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t near_or_far;
		} title_palm_tree;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
		} title_picture;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t model;
			uint8_t pad_0A[0x6];
			int16_t x_offset;
			int16_t y_offset;
			uint32_t car;
		} title_rider;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t frames;
			uint16_t palm_frames;
			uint8_t pad_14[0x6C];
			uint32_t wave_times;
			uint32_t tile_bank_address;
			uint32_t pages_address;
			uint32_t scroll_x_address;
			uint32_t scroll_y_address;
			uint32_t second_tile_bank_address;
			uint32_t second_pages_address;
			uint32_t second_scroll_x_address;
			uint32_t second_scroll_y_address;
		} title_screen;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x6];
			uint8_t frames;
			uint8_t pad_11[0x77];
			int16_t x_angle;
			uint8_t pad_8A[0x2];
			int16_t centre_x;
			uint8_t pad_8E[0x2];
			int16_t centre_y;
			uint8_t pad_92[0x6];
			int16_t y_angle;
		} title_seagull;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t leader;
			uint32_t record;
			int16_t position;
			uint8_t pad_1E[0x2];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x2];
			uint16_t position_fraction;
			uint8_t pad_2A[0x2];
			int16_t speed;
			int16_t sideways_speed;
			int16_t screen_x;
			uint8_t pad_32[0xD];
			int8_t side;
			uint8_t pad_40[0x20];
			uint32_t hit_box;
			uint8_t pad_64[0x6];
			uint16_t weight;
			uint8_t pad_6C[0x14];
			uint8_t passed;
			uint8_t passing_sound;
			uint8_t hit_sound;
			uint8_t pad_83[0x1];
			uint16_t stop_distance;
			uint8_t stopping;
			uint8_t shake;
		} traffic;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t leader;
			uint32_t record;
			uint8_t pad_1C[0x68];
			uint16_t stop_distance;
		} traffic_follower;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t delay;
			uint8_t pad_11[0x1];
			uint8_t sound;
		} traffic_hit_sound;
		struct
		{
			uint8_t pad_02[0x16];
			uint32_t list;
			int16_t stage_start;
			uint8_t stage;
			uint8_t pad_1F[0xD];
			uint16_t traffic_level;
		} traffic_spawner;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t shown_transmission;
			uint8_t pad_0A[0x6];
			uint8_t manual_cycle;
			uint8_t pad_11[0x1];
			uint8_t automatic_cycle;
			uint8_t slide_frames;
			uint32_t car_select;
			uint8_t pad_18[0x14];
			int16_t slide_step;
		} transmission_choice;
		struct
		{
			uint8_t pad_02[0x12];
			uint32_t panel;
			uint8_t pad_18[0x8];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
		} transmission_label;
		struct
		{
			uint8_t pad_02[0x1A];
			uint16_t size;
		} turntable;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t car;
			uint8_t pad_0A[0xA];
			uint32_t car_select;
			uint8_t pad_18[0x4];
			uint16_t size;
		} turntable_car;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t group;
			uint8_t pad_0A[0xA];
			uint32_t car_select;
			uint32_t item_list;
			uint16_t size;
			uint8_t pad_1E[0xE];
			uint16_t angle;
			uint8_t pad_2E[0x22];
			uint32_t previous_place;
			uint8_t pad_54[0x2C];
			uint16_t item_places;
		} turntable_scenery;
		struct
		{
			uint8_t pad_02[0xE];
			uint8_t timer;
			uint8_t pad_11[0x3];
			uint32_t car_select;
		} waiting_banner;
		struct
		{
			uint8_t pad_02[0x7];
			uint8_t index;
			uint8_t pad_0A[0x6];
			int16_t delay;
			uint16_t frames_left;
			uint8_t pad_14[0x4];
			uint32_t palette_list;
		} welcome_letter;
		struct
		{
			uint8_t pad_02[0xE];
			uint16_t colour_count;
			uint8_t pad_12[0x6E];
			uint16_t base_palette;
		} west_east_panel;
	};
};

struct car
{
	uint32_t object;
	uint32_t race_time;
	uint32_t checkpoint_time;
	uint8_t pad_0C[0x6];
	uint16_t time_left;
	uint8_t song;
	uint8_t pad_15[0xB];
	int32_t view_x;
	int32_t view_y;
	int32_t view_z;
	uint16_t view_pitch;
	uint16_t view_yaw;
	uint16_t road_pitch;
	uint16_t segment_fraction;
	uint32_t segment_progress;
	int16_t road_x;
	uint16_t segment_remaining;
	uint32_t window_record;
	uint16_t segment_offset;
	uint16_t window_segment;
	uint16_t segment;
	uint16_t stage_segments;
	uint16_t stage_start;
	uint16_t reached_stage_start;
	uint16_t surface_next_start;
	uint16_t surface_start;
	uint32_t torque_curves;
	uint32_t gear_ratios;
	uint32_t handling;
	uint8_t pad_5C[0x4];
	uint8_t model;
	uint8_t transmission;
	uint8_t gear;
	uint8_t top_gear;
	uint8_t gear_ratio;
	uint8_t wheel_slip;
	uint8_t moving;
	uint8_t skidding;
	uint16_t catch_up_target;
	uint16_t catch_up;
	uint8_t pad_6C[0x4];
	uint16_t revs;
	uint8_t pad_72[0x2];
	uint16_t speed;
	uint8_t pad_76[0x6];
	uint8_t off_road;
	uint8_t pad_7D[0x1];
	uint8_t brake_lights;
	uint8_t pad_7F[0x1];
	uint32_t next_x;
	uint32_t next_y;
	uint32_t next_z;
	uint8_t pad_8C[0x4];
	uint32_t next_x_fine;
	uint32_t next_y_fine;
	uint32_t next_z_fine;
	uint8_t width_code;
	uint8_t width_changing;
	uint8_t pad_9E[0x2];
	uint32_t course;
	uint16_t window_offset;
	uint16_t build_offset;
	uint16_t build_end_offset;
	uint16_t built_segments;
	uint16_t prebuilt_segments;
	uint16_t run_left;
	uint32_t pitch_angle;
	uint32_t heading_angle;
	uint16_t pitch_step;
	uint16_t heading_step;
	uint16_t road_width;
	int16_t road_width_step;
	uint16_t road_width_target;
	uint16_t course_records_read;
	uint8_t pad_C4[0x5];
	uint8_t branch;
	uint8_t new_branch;
	uint8_t at_fork;
	uint8_t forks_reached;
	uint8_t extensions_shown;
	uint8_t road_event;
	uint8_t pad_CF[0x1];
	uint8_t forks_taken;
	uint8_t stage_history[0x6];
	uint8_t branch_history[0x5];
	int32_t view_matrix_xx;
	int32_t view_matrix_xy;
	int32_t view_matrix_xz;
	int32_t view_matrix_yx;
	int32_t view_matrix_yy;
	int32_t view_matrix_yz;
	int32_t view_matrix_zx;
	int32_t view_matrix_zy;
	int32_t view_matrix_zz;
	uint16_t view_shift_nod;
	uint16_t view_pitch_nod;
	uint16_t view_rise;
	uint16_t view_side;
	uint16_t view_shift_lift;
	uint16_t view_pitch_lift;
	uint16_t view_shift_intro;
	uint16_t view_pitch_intro;
	uint16_t view_shift_rival;
	uint16_t view_pitch_rival;
	uint8_t pad_114[0xC];
	uint16_t hit_across;
	uint16_t hit_along;
	uint8_t pad_124[0x2];
	uint16_t bounce_damping;
	uint16_t push_decay;
	uint16_t gravity;
	uint16_t speed_change_decay;
	uint8_t pad_12E[0x2];
	uint32_t crash_table;
	uint8_t crash_sound;
	uint8_t crash_sound_delay;
	uint8_t crash_text;
	uint8_t crash_body_shown;
	uint8_t pad_138[0x8];
	uint8_t wheel_effect_sides;
	uint8_t wheel_effect_1;
	uint8_t wheel_effect_2;
	uint8_t pad_143[0x1];
	uint8_t shift_spin;
	uint8_t hard_acceleration;
	uint8_t surface_stage;
	uint8_t slowed_frames;
	uint32_t surface_entry_1;
	uint32_t surface_entry_2;
	uint8_t pad_150[0x30];
	uint8_t time_up;
	uint8_t at_goal;
	uint8_t race_over;
	uint8_t pad_183[0x7D];
};

struct view
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t window_offset;
	uint16_t horizon_line;
	uint16_t segment_remaining;
	uint8_t pad_16[0xA];
};

struct road_segment
{
	uint16_t heading;
	uint16_t pitch;
	uint32_t x;
	uint32_t z;
	uint32_t y;
	uint16_t width;
	uint16_t half_width;
	union
	{
		uint16_t width_code_and_event;
		uint8_t width_code;
		struct
		{
			uint8_t pad_14[0x1];
			uint8_t road_event;
		};
	};
	uint8_t pad_16[0xA];
};

struct projected_segment
{
	uint16_t scale;
	uint16_t width;
	uint16_t x;
	uint16_t y;
	uint16_t depth_layer;
	uint8_t pad_0A[0x2];
	uint8_t width_code;
	uint8_t road_event;
	uint8_t pad_0E[0x2];
};

struct depth_list
{
	uint16_t head;
	uint16_t tail;
};

struct screen
{
	uint8_t index;
	uint8_t player;
	uint8_t car_number;
	uint8_t pad_03[0x1];
	uint32_t palette_ram;
	uint32_t mixer;
	uint32_t io_chip;
	uint16_t eye_distance;
	uint16_t draw_distance;
	uint16_t sprite_origin_x;
	uint16_t sprite_origin_y;
	uint16_t view_pitch_base;
	uint16_t sprite_screen_bit;
	uint8_t pad_1C[0x4];
	uint32_t race_copy;
	uint32_t race;
	uint8_t pad_28[0x18];
	uint8_t link_event;
	uint8_t link_event_data_1;
	uint8_t link_event_data_2;
	uint8_t link_event_data_3;
	uint8_t link_event_data_4;
	uint8_t link_status;
	uint8_t link_status_set;
	uint8_t link_status_cleared;
	uint8_t link_status_changed;
	uint8_t pad_49[0x17];
	uint32_t leader_record;
	uint32_t last_record;
	uint32_t ahead_record;
	uint32_t behind_record;
	uint8_t leader_player;
	uint8_t last_player;
	uint8_t ahead_player;
	uint8_t behind_player;
	uint8_t pad_74[0xC];
	uint8_t game_mode;
	uint8_t mode_step;
	uint8_t game_step;
	uint8_t pad_83[0x5];
	uint16_t mode_timer;
	uint8_t pad_8A[0x6];
	uint32_t demo_flags;
	uint32_t flags;
	uint32_t game_start_time;
	uint8_t attract_car;
	uint8_t record_course;
	uint8_t next_attract_car;
	uint8_t pad_9F[0x1];
	uint8_t first_stage;
	uint8_t route_stage;
	uint8_t built_stage_level;
	uint8_t built_stage;
	uint8_t current_stage_level;
	uint8_t route_stage_level;
	uint8_t current_stage;
	uint8_t skip_scroll_update;
	uint8_t pad_A8[0x8];
	uint16_t sprite_groups;
	uint8_t pad_B2[0x2];
	uint32_t next_sprite_place;
	uint16_t horizon_line;
	uint8_t pad_BA[0x6];
	uint32_t sprite_list_last;
	uint32_t sprite_list_limit;
	uint8_t pad_C8[0x8];
	uint32_t shown_view;
	uint8_t view_ring_slot;
	uint8_t pad_D5[0xC];
	uint8_t start_lamp_blink;
	uint8_t entry_countdown_shown;
	uint8_t pad_E3[0x1D];
	uint8_t wheel;
	uint8_t gas;
	uint8_t brake;
	uint8_t pad_103[0x5];
	uint8_t input_write_place;
	uint8_t input_read_place;
	uint8_t pad_10A[0x6];
	uint32_t buttons;
	uint32_t previous_buttons;
	uint32_t buttons_pressed;
	uint32_t buttons_released;
	uint8_t pad_120[0x10];
	uint32_t input_ring[0x8];
	uint8_t fade_level;
	int8_t fade_step;
	uint8_t fade_end;
	uint8_t fade_kind;
	uint8_t fade_channels;
	uint8_t pad_155[0xB];
	uint16_t mixer_values[0x1F];
	uint8_t pad_19E[0x1];
	uint8_t colour_offset_mode;
	uint8_t pad_1A0[0x10];
	uint16_t mixer_defaults[0x1F];
	uint8_t pad_1EE[0x12];
	uint16_t road_top_line;
	uint8_t background_changing;
	uint8_t stage_background_change;
	uint8_t stage_background_branch;
	uint8_t backdrops_leaving;
	uint8_t background_fading;
	uint8_t background_at_start;
	uint32_t road_palette_fades;
	uint16_t background_scroll_y;
	uint8_t pad_20E[0xF2];
	uint8_t accepted_players[0x8];
	uint32_t host_record;
	uint8_t host_player;
	uint8_t pad_30D[0x2];
	uint8_t entrant_count;
	uint8_t entrants[0x8];
	uint8_t is_host;
	uint8_t entry_accepted;
	uint8_t in_game;
	uint8_t race_running;
	uint8_t race_started;
	uint8_t pad_31D[0x2];
	uint8_t place;
	uint8_t places[0x8];
	uint8_t name_entries_seen;
	uint8_t pad_329[0x7];
	uint8_t counted_stages[0x8];
	uint8_t counted_passes[0x6];
	uint8_t pad_33E[0x4];
	uint8_t pass_seen;
	uint8_t pad_343[0x1];
	uint8_t route_stages[0x5];
	uint8_t forks_decided;
	uint8_t route_branches[0x6];
	uint16_t leader_window_segment;
	uint8_t leader_stage;
	uint8_t pad_353[0x7];
	uint8_t checkpoint_passes[0x6];
	uint8_t checkpoint_orders[0x30];
	uint8_t checkpoint_order[0x30];
	uint16_t checkpoint_times_by_player[0x30];
	uint16_t checkpoint_times[0x30];
	uint8_t checkpoints_passed[0x8];
	uint8_t pad_488[0x27];
	uint8_t demo_player_count;
	uint8_t demo_players;
	uint8_t pad_4B1[0x41];
	uint8_t demo_stage;
	uint8_t pad_4F3[0xD];
	uint32_t own_hit_box;
	uint8_t pad_504[0xC];
	uint32_t hit_box_count;
	uint8_t pad_514[0xEC];
	struct view views[0x8];
	uint16_t stage_lengths[0x1F];
	uint16_t branch_road_length;
	uint8_t pad_740[0xC0];
	uint8_t sound_queue_read;
	uint8_t sound_queue_write;
	uint8_t sound_queue[0x20];
	uint8_t sound_start_cleared;
	uint8_t pad_823[0x1D];
	uint8_t second_sound_queue_read;
	uint8_t second_sound_queue_write;
	uint8_t second_sound_queue[0x20];
	uint8_t pad_862[0x9E];
	uint32_t palette_upload_count;
	uint32_t palette_uploads[0x7F];
	uint16_t unpack_state;
	uint8_t unpack_read_place;
	uint8_t unpack_write_place;
	uint32_t unpack_source;
	uint32_t unpack_destination;
	uint32_t unpack_copy_source;
	uint8_t unpack_copy_count;
	uint8_t unpack_flags;
	uint8_t unpack_flags_left;
	uint8_t pad_B13[0xED];
	uint32_t unpack_queue;
	uint8_t pad_C04[0x2FC];
	uint32_t random_state;
	int32_t turntable_depth_offset;
	int32_t turntable_radius_offset;
	uint8_t pad_F0C[0x2];
	uint8_t palette_fades_started;
	uint8_t nearing_fork;
	uint8_t lcd_text[0x10];
	uint16_t button_lamps;
	uint8_t music_lamp_mode;
	uint8_t arrows_lamp_mode;
	uint8_t music_lamp_timer;
	uint8_t arrows_lamp_timer;
	uint8_t pad_F26[0xA];
	uint32_t dj_mouth_shapes;
	uint8_t dj_mouth_frames;
	uint8_t dj_face;
	uint8_t pad_F36[0xA];
	uint32_t lcd_scroll_request;
	uint32_t lcd_scroll;
	uint8_t lcd_scroll_step;
	uint8_t lcd_scroll_frames;
	uint8_t pad_F4A[0x6];
	uint16_t shown_speed;
	int16_t maximum_speed;
	uint32_t speed_sum;
	uint16_t speed_samples;
	uint8_t pad_F5A[0x26];
	uint32_t traffic_racer_list;
	uint16_t visible_stretch_count;
	uint8_t pad_F86[0x20];
	uint16_t traffic_draw_distance;
	uint8_t traffic_count;
	uint8_t pad_FA9[0x57];
	struct car car;
	uint8_t pad_1200[0xD00];
	uint8_t crest_count;
	uint8_t pad_1F01[0xF];
	uint16_t crest_layers[0x10];
	uint16_t crest_heights[0x10];
	uint8_t pad_1F50[0xB0];
	struct road_segment road_segments[0x80];
	struct projected_segment projected_segments[0x80];
	uint8_t pad_3800[0x1800];
	union
	{
		struct depth_list depth_layers[0x1A0];
		struct
		{
			uint16_t depth_lists;
			uint8_t pad_5002[0x676];
			uint16_t text_depth_list_head;
			uint16_t text_depth_list_tail;
		};
	};
	uint8_t pad_5680[0x180];
	union
	{
		uint32_t route_map_signals;
		uint16_t kanji_text_number;
		struct
		{
			uint8_t pad_5800[0x1];
			uint8_t title_logo_shine;
			int16_t kanji_text_units;
		};
	};
	int16_t kanji_text_tens;
	int16_t kanji_text_hundreds;
	uint8_t pad_5808[0x7F8];
};

struct object_list
{
	uint16_t count;
	uint16_t objects[0x7F];
};

struct object_area
{
	uint32_t list;
	uint32_t list_first;
	uint32_t next_list;
	uint32_t list_end;
	uint8_t slots[0x7C];
	uint8_t pad_8C[0x74];
	struct object spare_object;
	struct object_list object_lists[0x2];
	struct object objects[0x7C];
};

struct race
{
	uint8_t machine;
	uint8_t player;
	uint8_t car_number;
	uint8_t master_advertise_sound;
	uint8_t master_steering_wheel_reaction;
	uint8_t master_coins_per_credit;
	uint8_t master_first_chute_multiplier;
	uint8_t master_bonus_adder;
	uint8_t master_coin_setting;
	uint8_t master_free_play;
	uint8_t master_game_difficulty;
	int8_t master_time_difficulty_0;
	uint8_t master_time_difficulty_1;
	uint32_t master_time_difficulty_2_5;
	uint8_t record_serial;
	uint8_t record_model;
	uint8_t record_name;
	uint16_t record_name_rest;
	uint16_t finish_time;
	uint8_t record_course;
	uint8_t entry_request;
	uint16_t entry_frames;
	uint8_t entry_serial_seen;
	uint8_t entry_serial;
	uint8_t entrant;
	uint8_t racer_count;
	uint8_t race_order;
	uint8_t race_order_1;
	uint8_t race_order_2;
	uint8_t race_order_3;
	uint32_t race_order_4_7;
	uint8_t game_over;
	uint8_t players_this_side;
	uint8_t players_other_side;
	uint8_t link_status;
	uint8_t link_buttons;
	uint8_t link_wheel;
	uint8_t link_gas;
	uint8_t link_brake;
	int32_t world_x;
	int32_t world_y;
	int32_t world_z;
	uint16_t animation;
	uint16_t palette;
	uint16_t segment_fraction;
	uint16_t segment;
	int16_t road_x;
	uint16_t window_segment;
	uint8_t car_choice;
	uint8_t model;
	uint8_t frame;
	uint8_t view_slot;
	uint16_t speed;
	int16_t sideways_speed;
	uint8_t wheel_effect_sides;
	uint8_t wheel_effect_1;
	uint8_t wheel_effect_2;
	uint8_t hard_acceleration;
	uint16_t segment_pitch;
	int16_t bounce_height;
	uint16_t car_flags;
	uint8_t link_pad_5A;
	uint8_t link_pad_5B;
	uint8_t ready;
	uint8_t started;
	uint8_t finished;
	uint8_t published;
	uint8_t stage_count;
	uint8_t pass_seen;
	uint8_t stage;
	uint8_t pass_count;
	uint8_t pass_player;
	uint8_t pass_checkpoint;
	uint8_t pass_order;
	uint8_t pass_branch;
	uint16_t pass_time;
	uint16_t time_left;
	uint32_t race_time;
	uint8_t pad_70[0x10];
};

struct object_list_block
{
	union
	{
		uint32_t low;
		uint16_t count;
	};
	uint32_t high;
};

struct road_row_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	uint8_t size_table;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t spacing;
	uint16_t count;
	uint32_t hit_box_along;
	uint32_t hit_box_across;
};

struct road_object_stream_entry
{
	int16_t position;
	uint16_t command;
	uint32_t handler;
	uint32_t parameter;
};

struct flashing_scenery_record
{
	uint16_t animation;
	uint32_t palettes;
	uint8_t flags;
	uint8_t size_table;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t spacing;
	uint16_t count;
	uint32_t hit_box_along;
	uint32_t hit_box_across;
};

struct aurora_record
{
	uint32_t animations;
	uint16_t palette;
	uint8_t flags;
	uint8_t animation_frames;
};

struct road_crosser_record
{
	uint16_t animation;
	uint16_t crossing_animation;
	uint16_t palette;
	uint8_t flags;
	int8_t side;
	int16_t x_offset;
	uint16_t trigger_distance;
	int16_t x_speed;
	uint8_t draw_mode;
	uint8_t scale_shift;
};

struct moving_scenery_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	uint8_t draw_mode;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t trigger_distance;
	int16_t x_speed;
	int16_t y_speed;
	int16_t road_speed;
};

struct swooping_helicopter_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	uint8_t draw_mode;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t trigger_distance;
	int16_t x_speed;
	int16_t y_speed;
	int16_t road_speed;
	uint8_t dive_frames;
	int16_t dive_y_speed;
};

struct start_gate_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	uint8_t draw_mode;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t spacing;
	uint16_t count;
	uint32_t hit_box_along;
	uint32_t hit_box_across;
};

struct flagman_record
{
	uint16_t animation;
	uint16_t waving_animation;
	uint16_t palette;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
};

struct start_balloon_record
{
	uint16_t animation;
	uint16_t palette;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
};

struct santa_sleigh_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	int8_t side;
	int16_t x_offset;
	int16_t x_speed;
	uint16_t sound;
	uint8_t draw_mode;
	uint8_t scale_shift;
};

struct goal_parade_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	int8_t side;
	int16_t x_offset;
	int16_t x_speed;
	uint8_t draw_mode;
	uint8_t scale_shift;
};

struct sounding_scenery_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t flags;
	uint8_t size_table;
	uint8_t scale_shift;
	int8_t side;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t spacing;
	uint16_t count;
	uint16_t sound;
};

struct traffic_record
{
	uint32_t sprite;
	int16_t side;
	int16_t x_offset;
	int16_t speed;
};

struct traffic_sprite_record
{
	uint16_t animation;
	uint16_t palette;
	uint8_t passing_sound;
	uint8_t hit_sound;
	uint16_t weight;
	uint16_t hit_box[0x4];
};

struct traffic_spawn_entry
{
	int16_t position;
	uint16_t level;
	uint32_t handler;
	uint32_t stop_distance;
	uint32_t record;
};

struct traffic_line_record
{
	uint32_t spacing;
};

struct backdrop_record
{
	uint32_t animation;
	uint16_t palette;
	uint16_t panorama_x;
	uint16_t height;
	uint16_t half_width;
	uint32_t approach_distance;
};

struct car_select_tilemap_setup
{
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t second_scroll_x_address;
	uint32_t second_scroll_y_address;
	uint32_t rowscroll_address;
	uint32_t rowselect_address;
	uint32_t background;
	uint16_t video_register_bits;
	uint8_t pad_1E[0x2];
};

struct animation_palette
{
	uint16_t animation;
	uint16_t palette;
};

struct turntable_scenery_group
{
	uint16_t animation;
	uint16_t palette;
	uint16_t size;
	uint16_t item_list_offset;
};

struct welcome_letter_place
{
	uint16_t x;
	uint8_t letter;
	uint8_t pad_03[0x1];
};

struct select_instruction_piece
{
	uint16_t animation;
	uint16_t palette;
	uint16_t x;
	uint16_t y;
};

struct traffic_path_step
{
	uint16_t size;
	uint16_t x;
	uint16_t y;
};

struct palette_cycle_record
{
	uint16_t delay;
	uint16_t frames_left;
	uint32_t palette_list;
	uint32_t step_frames;
};

struct sunset_fade_record
{
	uint32_t delay;
	uint32_t fade_list;
};

struct palette_fade_list
{
	uint32_t length;
};

struct palette_fade_entry
{
	uint32_t from;
	uint32_t to;
	uint16_t palette_offset;
	uint8_t blocks;
	uint8_t period;
};

struct goal_stop
{
	uint16_t position;
	int16_t x;
	uint16_t speed;
};

struct crash_physics
{
	uint16_t bounce_damping;
	uint16_t speed_change_decay;
	uint16_t push_decay;
	uint16_t gravity;
};

struct car_handling
{
	uint32_t steering;
	uint8_t pad_04[0x4];
	uint32_t off_road_drag;
	int32_t spin_start_force;
	int32_t spin_end_force;
	int32_t spin_drive_force;
	uint8_t pad_18[0x2];
	uint16_t half_width;
	int32_t brake_factor;
	int32_t grip;
	int32_t slide_factor;
};

struct gear_list
{
	uint8_t top_gear;
	uint8_t first_ratio;
};

struct torque_point
{
	int16_t speed;
	uint16_t force;
};

struct roadside_surface
{
	int16_t end;
	uint16_t inner_edge;
	uint16_t outer_edge;
	uint8_t inner_effect;
	uint8_t outer_effect;
};

struct stage_roadside_surfaces
{
	uint32_t first;
	uint32_t second;
};

struct sprite_animation
{
	uint8_t frame_time;
	uint8_t pad_01[0x3];
	int32_t frames[];
};

struct sprite_frame
{
	uint16_t piece_count;
	uint16_t piece_words[];
};

struct sprite_piece
{
	uint16_t attributes;
	uint16_t sprite;
	int16_t y_offset;
	int16_t x_offset;
};

struct sprite_size_step
{
	uint16_t source_size;
	uint16_t rom_offset;
	uint16_t height;
	uint16_t height_bits;
	uint16_t width;
	uint16_t width_bits;
};

struct checkpoint_board_place
{
	uint16_t x;
	uint16_t y;
};

struct sprite_list_switch
{
	uint32_t list;
	uint16_t end_word;
	uint16_t entry_2_jump;
	uint16_t entry_3_jump;
	uint16_t next_buffers;
	uint32_t next_list;
	uint32_t next_limit;
};

struct item_hit_box
{
	uint16_t along_start;
	uint16_t along_end;
	uint16_t across_start;
	uint16_t across_end;
};

struct course_fork
{
	uint8_t branch_stages[0x2];
	uint32_t branch_courses[0x2];
};

struct screen_hardware
{
	uint16_t sprite_screen_bit;
	uint32_t palette_ram;
	uint32_t mixer;
	uint32_t io_chip;
	uint8_t pad_0E[0x2];
};

struct screen_view_default
{
	uint32_t draw_distances;
	uint16_t eye_distance;
	uint16_t sprite_origin_x;
	uint16_t sprite_origin_y;
	uint16_t view_pitch_bases[0x8];
};

struct hidden_layer_pair
{
	uint8_t first_layer;
	uint8_t second_layer;
};

struct screen_fade_start
{
	uint8_t level;
	int8_t step;
	uint8_t end;
	uint8_t channels;
};

struct stored_duration
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint16_t days;
};

struct background_registers
{
	uint32_t pages_address;
	uint32_t tile_banks_address;
	uint32_t zoom_x_address;
	uint32_t zoom_y_address;
	uint32_t zoom_centre_x_address;
	uint32_t zoom_centre_y_address;
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t second_scroll_x_address;
	uint32_t second_scroll_y_address;
	uint32_t video_mask;
	uint32_t mixer_address;
};

struct background_setup
{
	uint32_t pages;
	uint32_t second_pages;
	uint8_t tile_bank;
	uint8_t second_tile_bank;
	uint8_t priority;
	uint8_t second_priority;
	uint32_t tile_pages;
	uint32_t palette_list;
};

struct bar_picture_bar
{
	uint32_t target;
	uint32_t scales;
};

struct course_record
{
	union
	{
		uint32_t model_and_name;
		uint8_t model;
		struct
		{
			uint8_t pad_00[0x1];
			uint8_t name[0x3];
		};
	};
	uint16_t time;
};

struct ranking_background_registers
{
	uint32_t tile_bank_address;
	uint32_t pages_address;
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t zoom_x_address;
	uint32_t zoom_y_address;
	uint32_t zoom_center_x_address;
	uint32_t zoom_center_y_address;
	uint32_t cleared_scroll_x_address;
	uint32_t cleared_scroll_y_address;
	uint32_t second_scroll_x_address;
	uint32_t second_scroll_y_address;
};

struct demo_race_choice
{
	uint8_t stage;
	uint8_t model;
};

struct attract_lamp_step
{
	uint8_t checkpoint;
	uint8_t leader;
};

struct route_map_registers
{
	uint32_t pages_address;
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t zoom_x_address;
	uint32_t zoom_y_address;
	uint32_t zoom_center_x_address;
	uint32_t zoom_center_y_address;
	uint32_t register_scroll_x_address;
	uint32_t register_scroll_y_address;
	uint32_t tile_bank_address;
	uint32_t second_pages_address;
	uint32_t second_scroll_x_address;
	uint32_t second_scroll_y_address;
	uint32_t second_register_scroll_x_address;
	uint32_t second_register_scroll_y_address;
	uint32_t second_tile_bank_address;
};

struct route_map_leg_car_place
{
	uint16_t x;
	uint16_t y;
	uint16_t frame;
	uint8_t pad_06[0x2];
};

struct stage_result_text
{
	uint16_t attribute;
	uint8_t characters[0x7];
};

struct stage_result_place_text
{
	uint16_t attribute;
	uint8_t characters[0x4];
};

struct route_map_landmark_record
{
	uint16_t animation;
	uint16_t palette;
	uint16_t width_scale;
	uint16_t height_scale;
	int16_t x_offset;
	int16_t y_offset;
	uint16_t half_rate;
	uint8_t pad_0E[0x2];
};

struct sound_cue
{
	uint8_t time;
	uint8_t sound;
};

struct road_surface_layer_setup
{
	uint16_t video_mask;
	uint16_t video_bits;
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t tile_bank_address;
	uint32_t page_register;
	uint32_t mixer_address;
	uint32_t line_scroll_table;
	uint32_t line_select_table;
};

struct race_background_layer_setup
{
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t tile_bank_address;
	uint32_t page_word_address;
	uint32_t mixer_address;
};

struct song_title_record
{
	uint16_t x;
};

struct title_screen_layer_setup
{
	uint32_t tile_bank_address;
	uint32_t pages_address;
	uint32_t scroll_x_address;
	uint32_t scroll_y_address;
	uint32_t second_tile_bank_address;
	uint32_t second_pages_address;
	uint32_t second_scroll_x_address;
	uint32_t second_scroll_y_address;
};

struct title_car_record
{
	uint16_t model;
	uint16_t palette;
	uint16_t width_scale;
	uint16_t height_scale;
	uint16_t position;
	uint16_t y;
	uint16_t speed;
	uint16_t depth_layer;
};

struct lcd_text_line
{
	uint32_t text_0_to_3;
	uint32_t text_4_to_7;
	uint32_t text_8_to_11;
	uint32_t text_12_to_15;
};

struct title_place
{
	uint16_t x;
	uint16_t y;
};

struct picture_record
{
	uint16_t animation;
	uint16_t palette;
	uint16_t width_scale;
	uint16_t height_scale;
	uint16_t x;
	uint16_t y;
	uint16_t depth_layer;
	uint16_t frames_left;
};

struct demo_caption_cue
{
	uint32_t frame;
	uint32_t routine;
};

struct memory_test_result_text
{
	uint8_t colour;
	uint8_t character_0;
	uint8_t character_1;
	uint8_t character_2;
	uint8_t character_3;
	uint8_t pad_05[0x1];
};

struct output_test_output
{
	uint16_t cell;
	uint8_t variable;
	uint8_t bit;
};

struct coin_setting_record
{
	uint8_t coins;
	uint8_t bonus;
	uint8_t first_multiplier;
	uint8_t second_multiplier;
};

struct privilege_mode_setting
{
	uint8_t master;
	uint8_t isdn;
};

struct time_digit_step
{
	uint32_t divisor;
	uint32_t cell_step;
};

struct screen_sound_page
{
	uint8_t command;
	uint8_t pad_01[0x7];
	uint8_t second_command;
	uint8_t pad_09[0x7];
	uint8_t engine_revs;
	uint8_t pad_11[0x1];
	uint8_t engine_requests;
	uint8_t engine_sample;
	uint8_t engine_loudness;
	uint8_t pad_15[0x3];
	uint8_t tyre_noise;
	uint8_t pad_19[0xE7];
};

struct seat_credits
{
	uint8_t credits;
	uint8_t coin_units;
	uint8_t bonus_units;
	uint8_t pad_03[0x1];
	uint8_t bookkeeping_byte;
	uint8_t pad_05[0x3];
};

struct coin_meter
{
	union
	{
		uint16_t frames;
		uint8_t frames_on;
		struct
		{
			uint8_t pad_00[0x1];
			uint8_t frames_off;
		};
	};
	uint8_t pulses_owed;
};

struct coin_chute
{
	uint32_t switch_bit;
	uint32_t coin_sounds;
	uint8_t pad_08[0x28];
};

struct coin_chute_variant
{
	uint32_t credits;
	uint32_t pulses_owed;
	uint32_t counter;
	uint32_t coin_units;
	uint8_t event_code;
	uint8_t pad_11[0x3];
};

struct settings_copy
{
	uint16_t checksum;
	uint16_t write_count;
	uint8_t pad_04[0x38];
};

struct seat_inputs
{
	uint8_t buttons;
	uint8_t wheel;
	uint8_t gas;
	uint8_t brake;
};

struct unpack_request
{
	uint32_t source;
	uint32_t destination;
};

struct packed_block_entry
{
	uint32_t source;
	uint32_t destination;
};

struct text_layer_record
{
	uint16_t cell;
	uint8_t attribute;
	uint8_t characters[];
};

struct sprite_text_record
{
	union
	{
		uint32_t place;
		uint16_t x;
	};
	uint16_t attribute;
	uint8_t characters[];
};

struct palette_block
{
	uint16_t place;
	uint16_t length;
	uint16_t colours[];
};

struct palette_block_list
{
	uint32_t length;
};

struct palette_block_entry
{
	uint32_t colours;
	uint16_t place;
	uint16_t blocks;
};

struct object_list_pair
{
	uint32_t handler;
	uint32_t data;
};

struct volume_range
{
	uint8_t low;
	uint8_t high;
};

struct turntable_scenery_item
{
	uint16_t angle;
	uint16_t radius;
	int16_t height;
};

struct screen_lamps
{
	uint8_t pad_00[0xC0];
	union
	{
		uint32_t checkpoint_all;
		uint8_t checkpoint;
		struct
		{
			uint8_t pad_C0[0x1];
			uint8_t checkpoint_blinks;
			uint8_t checkpoint_period;
			uint8_t checkpoint_timer;
		};
	};
	uint8_t pad_C4[0x4];
	union
	{
		uint32_t leader_all;
		uint8_t leader;
		struct
		{
			uint8_t pad_C8[0x1];
			uint8_t leader_blinks;
			uint8_t leader_period;
			uint8_t leader_timer;
		};
	};
};

struct car_hit_box
{
	uint16_t player;
	uint32_t record;
	uint16_t first_segment;
	uint16_t last_segment;
	uint16_t along_start;
	uint16_t along_end;
	uint16_t across_start;
	uint16_t across_end;
};

struct crash_entry
{
	uint16_t animation[0x2];
	uint16_t speed_factor;
	uint16_t push[0x2];
	uint16_t bounce_speed;
	uint8_t hit_sound;
	uint8_t crash_sound;
	uint16_t car_speed_factor;
};

struct course_start
{
	int16_t stage_command;
	uint8_t stage_level;
	uint8_t stage;
	int16_t run_length;
	int8_t pitch_step;
	int8_t heading_step;
	uint8_t width_code;
	uint8_t road_event;
};

struct car_part_place
{
	uint16_t x;
	uint16_t y;
};

struct board_car_sprite
{
	uint16_t animation;
	uint16_t palette;
};

struct checkpoint_pass
{
	uint8_t player;
};

struct sprite_place
{
	union
	{
		uint32_t size;
		uint16_t height;
		struct
		{
			uint8_t pad_00[0x2];
			uint16_t width;
		};
	};
	union
	{
		uint32_t place;
		uint16_t y;
		struct
		{
			uint8_t pad_04[0x2];
			uint16_t x;
		};
	};
	uint16_t rom_offset;
	uint16_t mirror_x;
	uint8_t pad_0C[0x4];
};

struct object_start_list
{
	uint32_t count;
};

struct tilemap_pages
{
	union
	{
		uint32_t pages;
		uint16_t upper_pages;
		struct
		{
			uint8_t pad_00[0x2];
			uint16_t lower_pages;
		};
	};
	uint8_t pad_04[0x4];
	uint32_t second_pages;
};

struct tilemap_banks
{
	uint8_t tile_bank;
	uint8_t pad_01[0x1];
	uint8_t second_tile_bank;
};

struct road_palette_record
{
	uint32_t road_fades;
};

struct backup_course_record
{
	uint8_t model;
	uint8_t name[0x3];
	uint16_t time;
};

struct race_car_rider
{
	uint32_t places;
	uint16_t animation;
	uint16_t palette;
	uint8_t gesture_mask;
	uint8_t pad_09[0x1];
	uint8_t frame;
	uint8_t frames;
	uint8_t pad_0C[0x4];
};

struct race_car_spray
{
	uint32_t animations;
	uint32_t places;
	uint32_t effect;
	uint8_t frame;
	uint8_t side;
	uint8_t kind;
	uint8_t pad_0F[0x1];
};

struct bookkeeping_count16
{
	uint16_t value;
	uint8_t pad_02[0x7A];
	uint16_t backup;
};

struct bookkeeping_count32
{
	uint32_t value;
	uint8_t pad_04[0x78];
	uint32_t backup;
};

struct bookkeeping_count8
{
	uint8_t value;
	uint8_t pad_01[0x7B];
	uint8_t backup;
};

struct time_digit
{
	int8_t flip;
	uint8_t digit;
	uint16_t width;
	uint32_t remaining;
};

struct backed_up_setting
{
	uint8_t value;
	uint8_t pad_01[0x3B];
	uint8_t backup;
};

struct bookkeeping_block
{
	uint16_t checksum;
	uint16_t coin_credits;
	uint16_t service_credits;
	uint16_t chute_a_coins;
	uint8_t pad_08[0x2];
	uint16_t chute_b_coins;
	uint8_t pad_0C[0x4];
	uint32_t total_time;
	uint16_t games_a;
	uint16_t games_b;
	uint32_t play_time_a;
	uint32_t play_time_b;
	uint16_t longest_play;
	uint16_t shortest_play;
	uint8_t pad_24[0x5A];
};

struct course_entry
{
	uint16_t length;
	uint32_t jump;
};

struct course_record_entry
{
	uint8_t model;
	uint8_t name;
	uint16_t name_rest;
	uint16_t time;
};

struct course_record_copy
{
	union
	{
		uint32_t model_and_name;
		uint8_t model;
		struct
		{
			uint8_t pad_00[0x1];
			uint8_t name[0x3];
		};
	};
	uint16_t time;
};

#pragma pack(pop)

_Static_assert(sizeof(struct object) == 0x100, "struct object has the size of the game's blocks");
_Static_assert(sizeof(struct car) == 0x200, "struct car has the size of the game's blocks");
_Static_assert(sizeof(struct screen) == 0x6000, "struct screen has the size of the game's blocks");
_Static_assert(offsetof(struct screen, views) == 0x600, "views at its place in struct screen");
_Static_assert(offsetof(struct screen, car) == 0x1000, "car at its place in struct screen");
_Static_assert(offsetof(struct screen, road_segments) == 0x2000, "road_segments at its place in struct screen");
_Static_assert(offsetof(struct screen, projected_segments) == 0x3000, "projected_segments at its place in struct screen");
_Static_assert(offsetof(struct screen, depth_layers) == 0x5000, "depth_layers at its place in struct screen");
_Static_assert(sizeof(struct object_area) == 0x8000, "struct object_area has the size of the game's blocks");
_Static_assert(offsetof(struct object_area, spare_object) == 0x100, "spare_object at its place in struct object_area");
_Static_assert(offsetof(struct object_area, object_lists) == 0x200, "object_lists at its place in struct object_area");
_Static_assert(offsetof(struct object_area, objects) == 0x400, "objects at its place in struct object_area");
_Static_assert(sizeof(struct race) == 0x80, "struct race has the size of the game's blocks");
_Static_assert(sizeof(struct object_list) == 0x100, "struct object_list has the size of the game's blocks");
_Static_assert(sizeof(struct object_list_block) == 0x8, "struct object_list_block has the size of the game's blocks");
_Static_assert(sizeof(struct road_row_record) == 0x18, "struct road_row_record has the size of the program ROM's records");
_Static_assert(sizeof(struct road_object_stream_entry) == 0xC, "struct road_object_stream_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct flashing_scenery_record) == 0x1A, "struct flashing_scenery_record has the size of the program ROM's records");
_Static_assert(sizeof(struct aurora_record) == 0x8, "struct aurora_record has the size of the program ROM's records");
_Static_assert(sizeof(struct road_crosser_record) == 0x10, "struct road_crosser_record has the size of the program ROM's records");
_Static_assert(sizeof(struct moving_scenery_record) == 0x14, "struct moving_scenery_record has the size of the program ROM's records");
_Static_assert(sizeof(struct swooping_helicopter_record) == 0x17, "struct swooping_helicopter_record has the size of the program ROM's records");
_Static_assert(sizeof(struct start_gate_record) == 0x18, "struct start_gate_record has the size of the program ROM's records");
_Static_assert(sizeof(struct flagman_record) == 0xB, "struct flagman_record has the size of the program ROM's records");
_Static_assert(sizeof(struct start_balloon_record) == 0x9, "struct start_balloon_record has the size of the program ROM's records");
_Static_assert(sizeof(struct santa_sleigh_record) == 0xE, "struct santa_sleigh_record has the size of the program ROM's records");
_Static_assert(sizeof(struct goal_parade_record) == 0xC, "struct goal_parade_record has the size of the program ROM's records");
_Static_assert(sizeof(struct sounding_scenery_record) == 0x12, "struct sounding_scenery_record has the size of the program ROM's records");
_Static_assert(sizeof(struct traffic_record) == 0xA, "struct traffic_record has the size of the program ROM's records");
_Static_assert(sizeof(struct traffic_sprite_record) == 0x10, "struct traffic_sprite_record has the size of the program ROM's records");
_Static_assert(sizeof(struct traffic_spawn_entry) == 0x10, "struct traffic_spawn_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct traffic_line_record) == 0x4, "struct traffic_line_record has the size of the program ROM's records");
_Static_assert(sizeof(struct backdrop_record) == 0x10, "struct backdrop_record has the size of the program ROM's records");
_Static_assert(sizeof(struct car_select_tilemap_setup) == 0x20, "struct car_select_tilemap_setup has the size of the program ROM's records");
_Static_assert(sizeof(struct animation_palette) == 0x4, "struct animation_palette has the size of the program ROM's records");
_Static_assert(sizeof(struct turntable_scenery_group) == 0x8, "struct turntable_scenery_group has the size of the program ROM's records");
_Static_assert(sizeof(struct welcome_letter_place) == 0x4, "struct welcome_letter_place has the size of the program ROM's records");
_Static_assert(sizeof(struct select_instruction_piece) == 0x8, "struct select_instruction_piece has the size of the program ROM's records");
_Static_assert(sizeof(struct traffic_path_step) == 0x6, "struct traffic_path_step has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_cycle_record) == 0xC, "struct palette_cycle_record has the size of the program ROM's records");
_Static_assert(sizeof(struct sunset_fade_record) == 0x8, "struct sunset_fade_record has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_fade_list) == 0x4, "struct palette_fade_list has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_fade_entry) == 0xC, "struct palette_fade_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct goal_stop) == 0x6, "struct goal_stop has the size of the program ROM's records");
_Static_assert(sizeof(struct crash_physics) == 0x8, "struct crash_physics has the size of the program ROM's records");
_Static_assert(sizeof(struct car_handling) == 0x28, "struct car_handling has the size of the program ROM's records");
_Static_assert(sizeof(struct gear_list) == 0x2, "struct gear_list has the size of the program ROM's records");
_Static_assert(sizeof(struct torque_point) == 0x4, "struct torque_point has the size of the program ROM's records");
_Static_assert(sizeof(struct roadside_surface) == 0x8, "struct roadside_surface has the size of the program ROM's records");
_Static_assert(sizeof(struct stage_roadside_surfaces) == 0x8, "struct stage_roadside_surfaces has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_animation) == 0x4, "struct sprite_animation has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_frame) == 0x2, "struct sprite_frame has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_piece) == 0x8, "struct sprite_piece has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_size_step) == 0xC, "struct sprite_size_step has the size of the program ROM's records");
_Static_assert(sizeof(struct checkpoint_board_place) == 0x4, "struct checkpoint_board_place has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_list_switch) == 0x14, "struct sprite_list_switch has the size of the program ROM's records");
_Static_assert(sizeof(struct item_hit_box) == 0x8, "struct item_hit_box has the size of the program ROM's records");
_Static_assert(sizeof(struct course_fork) == 0xA, "struct course_fork has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_hardware) == 0x10, "struct screen_hardware has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_view_default) == 0x1A, "struct screen_view_default has the size of the program ROM's records");
_Static_assert(sizeof(struct hidden_layer_pair) == 0x2, "struct hidden_layer_pair has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_fade_start) == 0x4, "struct screen_fade_start has the size of the program ROM's records");
_Static_assert(sizeof(struct stored_duration) == 0x5, "struct stored_duration has the size of the game's blocks");
_Static_assert(sizeof(struct background_registers) == 0x30, "struct background_registers has the size of the program ROM's records");
_Static_assert(sizeof(struct background_setup) == 0x14, "struct background_setup has the size of the program ROM's records");
_Static_assert(sizeof(struct bar_picture_bar) == 0x8, "struct bar_picture_bar has the size of the program ROM's records");
_Static_assert(sizeof(struct course_record) == 0x6, "struct course_record has the size of the program ROM's records");
_Static_assert(sizeof(struct ranking_background_registers) == 0x30, "struct ranking_background_registers has the size of the program ROM's records");
_Static_assert(sizeof(struct demo_race_choice) == 0x2, "struct demo_race_choice has the size of the program ROM's records");
_Static_assert(sizeof(struct attract_lamp_step) == 0x2, "struct attract_lamp_step has the size of the program ROM's records");
_Static_assert(sizeof(struct route_map_registers) == 0x40, "struct route_map_registers has the size of the program ROM's records");
_Static_assert(sizeof(struct route_map_leg_car_place) == 0x8, "struct route_map_leg_car_place has the size of the program ROM's records");
_Static_assert(sizeof(struct stage_result_text) == 0x9, "struct stage_result_text has the size of the program ROM's records");
_Static_assert(sizeof(struct stage_result_place_text) == 0x6, "struct stage_result_place_text has the size of the program ROM's records");
_Static_assert(sizeof(struct route_map_landmark_record) == 0x10, "struct route_map_landmark_record has the size of the program ROM's records");
_Static_assert(sizeof(struct sound_cue) == 0x2, "struct sound_cue has the size of the program ROM's records");
_Static_assert(sizeof(struct road_surface_layer_setup) == 0x20, "struct road_surface_layer_setup has the size of the program ROM's records");
_Static_assert(sizeof(struct race_background_layer_setup) == 0x14, "struct race_background_layer_setup has the size of the program ROM's records");
_Static_assert(sizeof(struct song_title_record) == 0x2, "struct song_title_record has the size of the program ROM's records");
_Static_assert(sizeof(struct title_screen_layer_setup) == 0x20, "struct title_screen_layer_setup has the size of the program ROM's records");
_Static_assert(sizeof(struct title_car_record) == 0x10, "struct title_car_record has the size of the program ROM's records");
_Static_assert(sizeof(struct lcd_text_line) == 0x10, "struct lcd_text_line has the size of the program ROM's records");
_Static_assert(sizeof(struct title_place) == 0x4, "struct title_place has the size of the program ROM's records");
_Static_assert(sizeof(struct picture_record) == 0x10, "struct picture_record has the size of the program ROM's records");
_Static_assert(sizeof(struct demo_caption_cue) == 0x8, "struct demo_caption_cue has the size of the program ROM's records");
_Static_assert(sizeof(struct memory_test_result_text) == 0x6, "struct memory_test_result_text has the size of the program ROM's records");
_Static_assert(sizeof(struct output_test_output) == 0x4, "struct output_test_output has the size of the program ROM's records");
_Static_assert(sizeof(struct coin_setting_record) == 0x4, "struct coin_setting_record has the size of the program ROM's records");
_Static_assert(sizeof(struct privilege_mode_setting) == 0x2, "struct privilege_mode_setting has the size of the program ROM's records");
_Static_assert(sizeof(struct time_digit_step) == 0x8, "struct time_digit_step has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_sound_page) == 0x100, "struct screen_sound_page has the size of the board's memory");
_Static_assert(sizeof(struct road_segment) == 0x20, "struct road_segment has the size of the game's blocks");
_Static_assert(sizeof(struct view) == 0x20, "struct view has the size of the game's blocks");
_Static_assert(sizeof(struct projected_segment) == 0x10, "struct projected_segment has the size of the game's blocks");
_Static_assert(sizeof(struct seat_credits) == 0x8, "struct seat_credits has the size of the game's blocks");
_Static_assert(sizeof(struct coin_meter) == 0x3, "struct coin_meter has the size of the game's blocks");
_Static_assert(sizeof(struct coin_chute) == 0x30, "struct coin_chute has the size of the program ROM's records");
_Static_assert(sizeof(struct coin_chute_variant) == 0x14, "struct coin_chute_variant has the size of the program ROM's records");
_Static_assert(sizeof(struct settings_copy) == 0x3C, "struct settings_copy has the size of the game's blocks");
_Static_assert(sizeof(struct seat_inputs) == 0x4, "struct seat_inputs has the size of the game's blocks");
_Static_assert(sizeof(struct unpack_request) == 0x8, "struct unpack_request has the size of the game's blocks");
_Static_assert(sizeof(struct packed_block_entry) == 0x8, "struct packed_block_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct text_layer_record) == 0x3, "struct text_layer_record has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_text_record) == 0x6, "struct sprite_text_record has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_block) == 0x4, "struct palette_block has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_block_list) == 0x4, "struct palette_block_list has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_block_entry) == 0x8, "struct palette_block_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct depth_list) == 0x4, "struct depth_list has the size of the game's blocks");
_Static_assert(sizeof(struct object_list_pair) == 0x8, "struct object_list_pair has the size of the program ROM's records");
_Static_assert(sizeof(struct volume_range) == 0x2, "struct volume_range has the size of the game's blocks");
_Static_assert(sizeof(struct turntable_scenery_item) == 0x6, "struct turntable_scenery_item has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_lamps) == 0xCC, "struct screen_lamps has the size of the game's blocks");
_Static_assert(sizeof(struct car_hit_box) == 0x12, "struct car_hit_box has the size of the game's blocks");
_Static_assert(sizeof(struct crash_entry) == 0x10, "struct crash_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct course_start) == 0xA, "struct course_start has the size of the program ROM's records");
_Static_assert(sizeof(struct car_part_place) == 0x4, "struct car_part_place has the size of the program ROM's records");
_Static_assert(sizeof(struct board_car_sprite) == 0x4, "struct board_car_sprite has the size of the game's blocks");
_Static_assert(sizeof(struct checkpoint_pass) == 0x1, "struct checkpoint_pass has the size of the game's blocks");
_Static_assert(sizeof(struct sprite_place) == 0x10, "struct sprite_place has the size of the game's blocks");
_Static_assert(sizeof(struct object_start_list) == 0x4, "struct object_start_list has the size of the program ROM's records");
_Static_assert(sizeof(struct tilemap_pages) == 0xC, "struct tilemap_pages has the size of the game's blocks");
_Static_assert(sizeof(struct tilemap_banks) == 0x3, "struct tilemap_banks has the size of the game's blocks");
_Static_assert(sizeof(struct road_palette_record) == 0x4, "struct road_palette_record has the size of the program ROM's records");
_Static_assert(sizeof(struct backup_course_record) == 0x6, "struct backup_course_record has the size of the board's memory");
_Static_assert(sizeof(struct race_car_rider) == 0x10, "struct race_car_rider has the size of the game's blocks");
_Static_assert(sizeof(struct race_car_spray) == 0x10, "struct race_car_spray has the size of the game's blocks");
_Static_assert(sizeof(struct bookkeeping_count16) == 0x7E, "struct bookkeeping_count16 has the size of the board's memory");
_Static_assert(sizeof(struct bookkeeping_count32) == 0x80, "struct bookkeeping_count32 has the size of the board's memory");
_Static_assert(sizeof(struct bookkeeping_count8) == 0x7D, "struct bookkeeping_count8 has the size of the board's memory");
_Static_assert(sizeof(struct time_digit) == 0x8, "struct time_digit has the size of the game's blocks");
_Static_assert(sizeof(struct backed_up_setting) == 0x3D, "struct backed_up_setting has the size of the game's blocks");
_Static_assert(sizeof(struct bookkeeping_block) == 0x7E, "struct bookkeeping_block has the size of the board's memory");
_Static_assert(sizeof(struct course_entry) == 0x6, "struct course_entry has the size of the program ROM's records");
_Static_assert(sizeof(struct course_record_entry) == 0x6, "struct course_record_entry has the size of the game's blocks");
_Static_assert(sizeof(struct course_record_copy) == 0x6, "struct course_record_copy has the size of the game's blocks");

#define OBJECT(address) ((struct object *)ram(address))
#define CAR(address) ((struct car *)ram(address))
#define SCREEN(address) ((struct screen *)ram(address))
#define OBJECT_AREA(address) ((struct object_area *)ram(address))
#define RACE(address) ((struct race *)ram(address))
#define OBJECT_LIST(address) ((struct object_list *)ram(address))
#define OBJECT_LIST_BLOCK(address) ((struct object_list_block *)ram(address))
#define ROAD_ROW_RECORD(address) ((const struct road_row_record *)data_at(address))
#define ROAD_OBJECT_STREAM_ENTRY(address) ((const struct road_object_stream_entry *)data_at(address))
#define ROAD_OBJECT_STREAM_FORKS(address) ((const uint32_t *)data_at(address))
#define FLASHING_SCENERY_RECORD(address) ((const struct flashing_scenery_record *)data_at(address))
#define FLASHING_SCENERY_PALETTES(address) ((const int16_t *)data_at(address))
#define AURORA_RECORD(address) ((const struct aurora_record *)data_at(address))
#define AURORA_ANIMATIONS(address) ((const int16_t *)data_at(address))
#define ROAD_CROSSER_RECORD(address) ((const struct road_crosser_record *)data_at(address))
#define MOVING_SCENERY_RECORD(address) ((const struct moving_scenery_record *)data_at(address))
#define SWOOPING_HELICOPTER_RECORD(address) ((const struct swooping_helicopter_record *)data_at(address))
#define START_GATE_RECORD(address) ((const struct start_gate_record *)data_at(address))
#define FLAGMAN_RECORD(address) ((const struct flagman_record *)data_at(address))
#define START_BALLOON_RECORD(address) ((const struct start_balloon_record *)data_at(address))
#define SANTA_SLEIGH_RECORD(address) ((const struct santa_sleigh_record *)data_at(address))
#define GOAL_PARADE_RECORD(address) ((const struct goal_parade_record *)data_at(address))
#define SOUNDING_SCENERY_RECORD(address) ((const struct sounding_scenery_record *)data_at(address))
#define TRAFFIC_RECORD(address) ((const struct traffic_record *)data_at(address))
#define TRAFFIC_SPRITE_RECORD(address) ((const struct traffic_sprite_record *)data_at(address))
#define TRAFFIC_SPAWN_ENTRY(address) ((const struct traffic_spawn_entry *)data_at(address))
#define TRAFFIC_LINE_RECORD(address) ((const struct traffic_line_record *)data_at(address))
#define TRAFFIC_LINE_FOLLOWERS(address) ((const uint32_t *)data_at(address))
#define BACKDROP_RECORD(address) ((const struct backdrop_record *)data_at(address))
#define CAR_SELECT_TILEMAP_SETUP(address) ((const struct car_select_tilemap_setup *)data_at(address))
#define ANIMATION_PALETTE(address) ((const struct animation_palette *)data_at(address))
#define TURNTABLE_SCENERY_GROUP(address) ((const struct turntable_scenery_group *)data_at(address))
#define WELCOME_LETTER_PLACE(address) ((const struct welcome_letter_place *)data_at(address))
#define SELECT_INSTRUCTION_PIECE(address) ((const struct select_instruction_piece *)data_at(address))
#define TRAFFIC_PATH_STEP(address) ((const struct traffic_path_step *)data_at(address))
#define GOAL_ACTOR_SCRIPT_WORDS(address) ((const uint16_t *)data_at(address))
#define PALETTE_CYCLE_RECORD(address) ((const struct palette_cycle_record *)data_at(address))
#define PALETTE_CYCLE_STEPS(address) ((const uint32_t *)data_at(address))
#define SUNSET_FADE_RECORD(address) ((const struct sunset_fade_record *)data_at(address))
#define PALETTE_FADE_LIST(address) ((const struct palette_fade_list *)data_at(address))
#define PALETTE_FADE_ENTRY(address) ((const struct palette_fade_entry *)data_at(address))
#define GRID_SEGMENTS(address) ((const uint16_t *)data_at(address))
#define GRID_ROAD_PLACES(address) ((const uint32_t *)data_at(address))
#define GOAL_STOP(address) ((const struct goal_stop *)data_at(address))
#define CRASH_PHYSICS(address) ((const struct crash_physics *)data_at(address))
#define WHEEL_STEERING(address) ((const int8_t *)data_at(address))
#define CAR_HANDLING(address) ((const struct car_handling *)data_at(address))
#define GEAR_LIST(address) ((const struct gear_list *)data_at(address))
#define TORQUE_POINT(address) ((const struct torque_point *)data_at(address))
#define ROADSIDE_SURFACE(address) ((const struct roadside_surface *)data_at(address))
#define STAGE_ROADSIDE_SURFACES(address) ((const struct stage_roadside_surfaces *)data_at(address))
#define OBJECT_PALETTE_STEPS(address) ((const int16_t *)data_at(address))
#define SPRITE_ANIMATION(address) ((const struct sprite_animation *)data_at(address))
#define SPRITE_ANIMATION_POINTER(address) ((const uint32_t *)data_at(address))
#define SPRITE_FRAME(address) ((const struct sprite_frame *)data_at(address))
#define SPRITE_PIECE(address) ((const struct sprite_piece *)data_at(address))
#define SPRITE_SIZE_STEP(address) ((const struct sprite_size_step *)data_at(address))
#define SPRITE_PALETTE_WORDS(address) ((const uint16_t *)data_at(address))
#define SPRITE_PALETTE_POINTER(address) ((const uint32_t *)data_at(address))
#define CAR_GEAR_RATIOS(address) ((const uint8_t *)data_at(address))
#define CHECKPOINT_BOARD_PLACE(address) ((const struct checkpoint_board_place *)data_at(address))
#define SPRITE_LIST_SWITCH(address) ((const struct sprite_list_switch *)data_at(address))
#define SPRITE_LIST_SWITCH_POINTER(address) ((const uint32_t *)data_at(address))
#define ITEM_HIT_BOX(address) ((const struct item_hit_box *)data_at(address))
#define COURSE_POINTER(address) ((const uint32_t *)data_at(address))
#define COURSE_FORK(address) ((const struct course_fork *)data_at(address))
#define ROAD_PROJECTION_PATTERN(address) ((const uint8_t *)data_at(address))
#define ROAD_ROW_SIZE_STEPS(address) ((const uint8_t *)data_at(address))
#define ROAD_ROW_SIZE_STEPS_POINTER(address) ((const uint32_t *)data_at(address))
#define ROAD_ROW_SIZE_SHIFTS(address) ((const int8_t *)data_at(address))
#define ROAD_ROW_SIZE_SHIFTS_POINTER(address) ((const uint32_t *)data_at(address))
#define SCREEN_HARDWARE(address) ((const struct screen_hardware *)data_at(address))
#define SCREEN_HARDWARE_POINTER(address) ((const uint32_t *)data_at(address))
#define DRAW_DISTANCE_LIST(address) ((const uint16_t *)data_at(address))
#define SCREEN_VIEW_DEFAULT(address) ((const struct screen_view_default *)data_at(address))
#define SPRITE_CONTROL_VALUES(address) ((const uint8_t *)data_at(address))
#define HIDDEN_LAYER_PAIR(address) ((const struct hidden_layer_pair *)data_at(address))
#define SCREEN_FADE_START(address) ((const struct screen_fade_start *)data_at(address))
#define TEXT_FONT_ROWS(address) ((const uint8_t *)data_at(address))
#define TEXT_CHARACTERS(address) ((const uint8_t *)data_at(address))
#define COURSE_BYTES(address) ((const uint8_t *)data_at(address))
#define INPUT_TEST_BYTES(address) ((const uint8_t *)data_at(address))
#define TEST_MODE_TEXT_BYTES(address) ((const uint8_t *)data_at(address))
#define STORED_DURATION(address) ((struct stored_duration *)ram(address))
#define BACKGROUND_REGISTERS(address) ((const struct background_registers *)data_at(address))
#define BACKGROUND_SETUP(address) ((const struct background_setup *)data_at(address))
#define FIRST_ATTRACT_CARS(address) ((const uint8_t *)data_at(address))
#define BAR_PICTURE_BAR(address) ((const struct bar_picture_bar *)data_at(address))
#define STAFF_CREDITS_CODE_CONTROLS(address) ((const uint8_t *)data_at(address))
#define CAR_NAME_LETTERS(address) ((const uint8_t *)data_at(address))
#define COURSE_RECORD(address) ((const struct course_record *)data_at(address))
#define REJECTED_NAMES(address) ((const uint32_t *)data_at(address))
#define RANKING_BACKGROUND_REGISTERS(address) ((const struct ranking_background_registers *)data_at(address))
#define DEMO_RACE_CHOICE(address) ((const struct demo_race_choice *)data_at(address))
#define ATTRACT_LAMP_STEP(address) ((const struct attract_lamp_step *)data_at(address))
#define ROUTE_MAP_REGISTERS(address) ((const struct route_map_registers *)data_at(address))
#define ROUTE_MAP_LEG_CAR_PLACE(address) ((const struct route_map_leg_car_place *)data_at(address))
#define STAGE_RESULT_TEXT(address) ((const struct stage_result_text *)data_at(address))
#define STAGE_RESULT_PLACE_TEXT(address) ((const struct stage_result_place_text *)data_at(address))
#define ROUTE_MAP_LANDMARK_RECORD(address) ((const struct route_map_landmark_record *)data_at(address))
#define SOUND_CUE(address) ((const struct sound_cue *)data_at(address))
#define ROAD_SURFACE_LAYER_SETUP(address) ((const struct road_surface_layer_setup *)data_at(address))
#define ROAD_TILES_CHANGE_BYTES(address) ((const uint8_t *)data_at(address))
#define RACE_BACKGROUND_LAYER_SETUP(address) ((const struct race_background_layer_setup *)data_at(address))
#define BACKGROUND_PAGE_WORD_STEPS(address) ((const uint32_t *)data_at(address))
#define PALETTE_COLOURS(address) ((const uint16_t *)data_at(address))
#define SONG_TITLE_RECORD(address) ((const struct song_title_record *)data_at(address))
#define TITLE_SCREEN_LAYER_SETUP(address) ((const struct title_screen_layer_setup *)data_at(address))
#define TITLE_WAVE_FRAMES(address) ((const uint16_t *)data_at(address))
#define TITLE_CAR_RECORD(address) ((const struct title_car_record *)data_at(address))
#define LCD_TEXT_LINE(address) ((const struct lcd_text_line *)data_at(address))
#define LCD_CHARACTER_ROWS(address) ((const uint8_t *)data_at(address))
#define DJ_MOUTH_SHAPE_NUMBERS(address) ((const uint8_t *)data_at(address))
#define TITLE_PLACE(address) ((const struct title_place *)data_at(address))
#define PICTURE_RECORD(address) ((const struct picture_record *)data_at(address))
#define TITLE_LOGO_PALETTES(address) ((const uint16_t *)data_at(address))
#define DEMO_CAPTION_CUE(address) ((const struct demo_caption_cue *)data_at(address))
#define KANJI_TEXT(address) ((const int16_t *)data_at(address))
#define KANJI_CHARACTER_ROWS(address) ((const uint8_t *)data_at(address))
#define TEST_MODE_CELLS(address) ((const uint16_t *)data_at(address))
#define MEMORY_TEST_RESULT_TEXT(address) ((const struct memory_test_result_text *)data_at(address))
#define TEST_MODE_CHARACTERS(address) ((const uint8_t *)data_at(address))
#define OUTPUT_TEST_OUTPUT(address) ((const struct output_test_output *)data_at(address))
#define TEST_MODE_SETTING_PLACES(address) ((const uint8_t *)data_at(address))
#define COIN_SETTING_RECORD(address) ((const struct coin_setting_record *)data_at(address))
#define PRIVILEGE_MODE_SETTING(address) ((const struct privilege_mode_setting *)data_at(address))
#define TIME_DIGIT_STEP(address) ((const struct time_digit_step *)data_at(address))
#define SCREEN_SOUND_PAGE(address) ((struct screen_sound_page *)board(address))
#define ROAD_SEGMENT(address) ((struct road_segment *)ram(address))
#define VIEW(address) ((struct view *)ram(address))
#define PROJECTED_SEGMENT(address) ((struct projected_segment *)ram(address))
#define SEAT_CREDITS(address) ((struct seat_credits *)ram(address))
#define COIN_METER(address) ((struct coin_meter *)ram(address))
#define COIN_CHUTE(address) ((const struct coin_chute *)data_at(address))
#define COIN_CHUTE_VARIANT(address) ((const struct coin_chute_variant *)data_at(address))
#define SETTINGS_COPY(address) ((struct settings_copy *)ram(address))
#define SEAT_INPUTS(address) ((struct seat_inputs *)ram(address))
#define UNPACK_REQUEST(address) ((struct unpack_request *)ram(address))
#define PACKED_BLOCK_ENTRY(address) ((const struct packed_block_entry *)data_at(address))
#define PACKED_BYTES(address) ((const uint8_t *)data_at(address))
#define TILE_PAGE_ENTRIES(address) ((const uint32_t *)data_at(address))
#define TEXT_LAYER_RECORD(address) ((const struct text_layer_record *)data_at(address))
#define TEXT_HALFWORD_CHARACTERS(address) ((const uint16_t *)data_at(address))
#define SPRITE_TEXT_RECORD(address) ((const struct sprite_text_record *)data_at(address))
#define PALETTE_BLOCK(address) ((const struct palette_block *)data_at(address))
#define PALETTE_BLOCK_LIST(address) ((const struct palette_block_list *)data_at(address))
#define PALETTE_BLOCK_ENTRY(address) ((const struct palette_block_entry *)data_at(address))
#define DEPTH_LIST(address) ((struct depth_list *)ram(address))
#define REMARK_ROUTINES(address) ((const uint32_t *)data_at(address))
#define OBJECT_LIST_PAIR(address) ((const struct object_list_pair *)data_at(address))
#define OBJECT_HANDLERS(address) ((const uint32_t *)data_at(address))
#define VOLUME_RANGE(address) ((struct volume_range *)ram(address))
#define COIN_SETTING_DESCRIPTION(address) ((const uint8_t *)data_at(address))
#define TURNTABLE_SCENERY_ITEM(address) ((const struct turntable_scenery_item *)data_at(address))
#define SCREEN_LAMPS(address) ((struct screen_lamps *)ram(address))
#define CAR_HIT_BOX(address) ((struct car_hit_box *)ram(address))
#define CRASH_ENTRY(address) ((const struct crash_entry *)data_at(address))
#define COURSE_START(address) ((const struct course_start *)data_at(address))
#define CAR_PART_PLACE(address) ((const struct car_part_place *)data_at(address))
#define BOARD_CAR_SPRITE(address) ((struct board_car_sprite *)ram(address))
#define CHECKPOINT_PASS(address) ((struct checkpoint_pass *)ram(address))
#define OBJECT_WORDS(address) ((uint32_t *)ram(address))
#define OBJECT_SLOT_FLAGS(address) ((uint8_t *)ram(address))
#define PLAYER_CAR_NUMBERS(address) ((uint8_t *)ram(address))
#define SEEN_RECORD_SERIALS(address) ((uint8_t *)ram(address))
#define RACE_RECORD_WORDS(address) ((uint32_t *)ram(address))
#define RACE_RECORD_BYTES(address) ((uint8_t *)ram(address))
#define STAGE_LENGTH_LIST(address) ((uint16_t *)ram(address))
#define PALETTE_UPLOAD_QUEUE(address) ((uint32_t *)ram(address))
#define MIXER_COLOUR_OFFSETS(address) ((uint16_t *)ram(address))
#define PALETTE_FADE_COLOURS(address) ((uint16_t *)ram(address))
#define PALETTE_FADE_DIFFERENCES(address) ((int8_t *)ram(address))
#define BACKGROUND_FADES(address) ((uint32_t *)ram(address))
#define UNPACKED_BYTES(address) ((uint8_t *)board(address))
#define TEXT_LAYER_CELLS(address) ((uint16_t *)board(address))
#define TEXT_LAYER_FONT_ROWS(address) ((uint32_t *)board(address))
#define BOOT_REGISTER_VALUES(address) ((const uint8_t *)data_at(address))
#define CLEARED_RAM_WORDS(address) ((uint32_t *)ram(address))
#define PORT_OUTPUT_BITS(address) ((uint8_t *)ram(address))
#define VBLANK_COUNT(address) ((uint8_t *)ram(address))
#define SPRITE_PLACE(address) ((struct sprite_place *)ram(address))
#define SPRITE_PLACE_WORDS(address) ((uint32_t *)ram(address))
#define OBJECT_LINKS(address) ((uint16_t *)ram(address))
#define OBJECT_START_LIST(address) ((const struct object_start_list *)data_at(address))
#define TILEMAP_PAGES(address) ((struct tilemap_pages *)ram(address))
#define TILEMAP_BANKS(address) ((struct tilemap_banks *)ram(address))
#define ROAD_PALETTE_RECORD(address) ((const struct road_palette_record *)data_at(address))
#define FADE_OBJECTS(address) ((uint32_t *)ram(address))
#define STRETCH_ENDS(address) ((uint16_t *)ram(address))
#define LINE_SELECTS(address) ((uint16_t *)board(address))
#define CHIP_TEST_RESULTS(address) ((uint8_t *)ram(address))
#define PROGRAM_HALFWORDS(address) ((const uint16_t *)data_at(address))
#define ANALOG_POSITIONS(address) ((uint8_t *)ram(address))
#define OUTPUT_BITS(address) ((uint32_t *)ram(address))
#define STAGE_GAME_COUNTS(address) ((uint16_t *)board(address))
#define BACKUP_COURSE_RECORD(address) ((struct backup_course_record *)board(address))
#define TILE_PIXEL_ROWS(address) ((uint32_t *)board(address))
#define PLAYER_PLACE(address) ((uint8_t *)ram(address))
#define RACER_NUMBERS(address) ((uint8_t *)ram(address))
#define OBJECT_TILE_PAGES(address) ((uint32_t *)ram(address))
#define COURSE_RECORD_BYTES(address) ((uint8_t *)ram(address))
#define FLOOR_LINE_OFFSETS(address) ((uint16_t *)ram(address))
#define NAME_LETTERS(address) ((uint8_t *)ram(address))
#define ITEM_PLACE_WORDS(address) ((uint16_t *)ram(address))
#define ROAD_POINTS(address) ((uint16_t *)ram(address))
#define LINE_SCROLL_VALUES(address) ((uint16_t *)board(address))
#define TILE_PAGE_PLACES(address) ((uint32_t *)ram(address))
#define ROAD_PAGE_TABLE_BYTES(address) ((uint8_t *)ram(address))
#define TILES_CHANGE_FLAGS(address) ((uint8_t *)ram(address))
#define PAGE_SELECT(address) ((uint32_t *)ram(address))
#define VIDEO_REGISTER_WORDS(address) ((uint16_t *)ram(address))
#define TILE_BANK_VALUES(address) ((uint8_t *)ram(address))
#define MIXER_VALUE_PAIRS(address) ((uint32_t *)ram(address))
#define ROAD_PALETTE_LISTS(address) ((const uint32_t *)data_at(address))
#define RACE_CAR_RIDER(address) ((struct race_car_rider *)ram(address))
#define RACE_CAR_SPRAY(address) ((struct race_car_spray *)ram(address))
#define SPRAY_ANIMATION_NUMBERS(address) ((const uint16_t *)data_at(address))
#define WHEEL_EFFECT_VALUES(address) ((uint8_t *)ram(address))
#define SPRITE_WORDS(address) ((uint16_t *)board(address))
#define RACE_ORDER_PLAYERS(address) ((uint8_t *)ram(address))
#define OBJECT_TEXT_CHARACTERS(address) ((uint8_t *)ram(address))
#define CHECKPOINT_COUNTS(address) ((uint8_t *)ram(address))
#define TORQUE_CURVE_FORCES(address) ((uint16_t *)ram(address))
#define BOOKKEEPING_COUNT16(address) ((struct bookkeeping_count16 *)board(address))
#define BOOKKEEPING_COUNT32(address) ((struct bookkeeping_count32 *)board(address))
#define BOOKKEEPING_COUNT8(address) ((struct bookkeeping_count8 *)board(address))
#define BOOKKEEPING_HALFWORDS(address) ((uint16_t *)board(address))
#define SEAT_SOUND_COUNTS(address) ((uint8_t *)ram(address))
#define CHUTE_UNIT_SETTINGS(address) ((uint8_t *)ram(address))
#define ANALOG_VALUES(address) ((uint8_t *)ram(address))
#define ANALOG_CALIBRATION_BYTES(address) ((uint8_t *)ram(address))
#define MUSIC_FADE_DELAYS(address) ((uint8_t *)board(address))
#define LCD_CHARACTERS(address) ((uint8_t *)ram(address))
#define SETTINGS_WORDS(address) ((uint16_t *)ram(address))
#define TIME_DIGIT(address) ((struct time_digit *)ram(address))
#define TESTED_SOUND_RAM_BYTES(address) ((uint8_t *)board(address))
#define TIME_DIFFICULTY_SECONDS(address) ((uint8_t *)ram(address))
#define BACKED_UP_SETTING(address) ((struct backed_up_setting *)ram(address))
#define BOOKKEEPING_BLOCK(address) ((struct bookkeeping_block *)board(address))
#define COURSE_ENTRY(address) ((const struct course_entry *)data_at(address))
#define COURSE_RECORD_ENTRY(address) ((struct course_record_entry *)ram(address))
#define LINE_SELECT_PAIRS(address) ((uint32_t *)board(address))
#define COURSE_RECORD_COPY(address) ((struct course_record_copy *)ram(address))
#define BITMAP_LAYER_WORDS(address) ((uint32_t *)board(address))

/* the program ROM's tables of records, by name: name[i].field */
#define bar_picture_bars_ADDRESS 0x0011B1u
extern const void *bar_picture_bars_data;
#define bar_picture_bars ((const struct bar_picture_bar *)(bar_picture_bars_data))
#define attract_lamp_steps_ADDRESS 0x001607u
extern const void *attract_lamp_steps_data;
#define attract_lamp_steps ((const struct attract_lamp_step *)(attract_lamp_steps_data))
#define staff_credits_code_sequence_ADDRESS 0x001ADCu
extern const void *staff_credits_code_sequence_data;
#define staff_credits_code_sequence ((const uint8_t *)(staff_credits_code_sequence_data))
#define ranking_background_register_sets_ADDRESS 0x002C44u
extern const void *ranking_background_register_sets_data;
#define ranking_background_register_sets ((const struct ranking_background_registers *)(ranking_background_register_sets_data))
#define default_course_records_ADDRESS 0x002CC4u
extern const void *default_course_records_data;
#define default_course_records ((const struct course_record *)(default_course_records_data))
#define name_entry_rejected_names_ADDRESS 0x002D7Cu
extern const void *name_entry_rejected_names_data;
#define name_entry_rejected_names ((const uint32_t *)(name_entry_rejected_names_data))
#define attract_demo_races_ADDRESS 0x0034F4u
extern const void *attract_demo_races_data;
#define attract_demo_races ((const struct demo_race_choice *)(attract_demo_races_data))
#define route_map_register_sets_ADDRESS 0x0047E8u
extern const void *route_map_register_sets_data;
#define route_map_register_sets ((const struct route_map_registers *)(route_map_register_sets_data))
#define route_map_start_leg_car_places_ADDRESS 0x004964u
extern const void *route_map_start_leg_car_places_data;
#define route_map_start_leg_car_places ((const struct route_map_leg_car_place *)(route_map_start_leg_car_places_data))
#define route_map_first_fork_leg_car_places_ADDRESS 0x0049A4u
extern const void *route_map_first_fork_leg_car_places_data;
#define route_map_first_fork_leg_car_places ((const struct route_map_leg_car_place *)(route_map_first_fork_leg_car_places_data))
#define route_map_fork_leg_car_places_ADDRESS 0x004A24u
extern const void *route_map_fork_leg_car_places_data;
#define route_map_fork_leg_car_places ((const struct route_map_leg_car_place *)(route_map_fork_leg_car_places_data))
#define stage_result_labels_ADDRESS 0x004BA4u
extern const void *stage_result_labels_data;
#define stage_result_labels ((const struct stage_result_text *)(stage_result_labels_data))
#define stage_result_places_ADDRESS 0x004BDAu
extern const void *stage_result_places_data;
#define stage_result_places ((const struct stage_result_place_text *)(stage_result_places_data))
#define stage_result_retire_ADDRESS 0x004C0Au
extern const void *stage_result_retire_data;
#define stage_result_retire ((const struct stage_result_text *)(stage_result_retire_data))
#define route_map_landmarks_ADDRESS 0x004C14u
extern const void *route_map_landmarks_data;
#define route_map_landmarks ((const struct route_map_landmark_record *)(route_map_landmarks_data))
#define traffic_path_ADDRESS 0x0067B2u
extern const void *traffic_path_data;
#define traffic_path ((const struct traffic_path_step *)(traffic_path_data))
#define car_select_tilemap_setups_ADDRESS 0x006920u
extern const void *car_select_tilemap_setups_data;
#define car_select_tilemap_setups ((const struct car_select_tilemap_setup *)(car_select_tilemap_setups_data))
#define turntable_car_looks_ADDRESS 0x0069B8u
extern const void *turntable_car_looks_data;
#define turntable_car_looks ((const struct animation_palette *)(turntable_car_looks_data))
#define turntable_scenery_groups_ADDRESS 0x006A18u
extern const void *turntable_scenery_groups_data;
#define turntable_scenery_groups ((const struct turntable_scenery_group *)(turntable_scenery_groups_data))
#define welcome_letter_places_ADDRESS 0x006DACu
extern const void *welcome_letter_places_data;
#define welcome_letter_places ((const struct welcome_letter_place *)(welcome_letter_places_data))
#define select_instruction_pieces_ADDRESS 0x006DF6u
extern const void *select_instruction_pieces_data;
#define select_instruction_pieces ((const struct select_instruction_piece *)(select_instruction_pieces_data))
#define driver_couple_looks_ADDRESS 0x006E58u
extern const void *driver_couple_looks_data;
#define driver_couple_looks ((const struct animation_palette *)(driver_couple_looks_data))
#define stage_roadside_surface_lists_ADDRESS 0x00B78Cu
extern const void *stage_roadside_surface_lists_data;
#define stage_roadside_surface_lists ((const struct stage_roadside_surfaces *)(stage_roadside_surface_lists_data))
#define checkpoint_board_car_places_ADDRESS 0x00CF68u
extern const void *checkpoint_board_car_places_data;
#define checkpoint_board_car_places ((const struct checkpoint_board_place *)(checkpoint_board_car_places_data))
#define checkpoint_board_number_places_ADDRESS 0x00CF88u
extern const void *checkpoint_board_number_places_data;
#define checkpoint_board_number_places ((const struct checkpoint_board_place *)(checkpoint_board_number_places_data))
#define checkpoint_board_behind_places_ADDRESS 0x00CFA8u
extern const void *checkpoint_board_behind_places_data;
#define checkpoint_board_behind_places ((const struct checkpoint_board_place *)(checkpoint_board_behind_places_data))
#define sprite_list_switches_ADDRESS 0x00F508u
extern const void *sprite_list_switches_data;
#define sprite_list_switches ((const uint32_t *)(sprite_list_switches_data))
#define hidden_layer_pairs_ADDRESS 0x010B06u
extern const void *hidden_layer_pairs_data;
#define hidden_layer_pairs ((const struct hidden_layer_pair *)(hidden_layer_pairs_data))
#define screen_hardware_records_ADDRESS 0x010BA0u
extern const void *screen_hardware_records_data;
#define screen_hardware_records ((const uint32_t *)(screen_hardware_records_data))
#define screen_view_defaults_ADDRESS 0x010BC8u
extern const void *screen_view_defaults_data;
#define screen_view_defaults ((const struct screen_view_default *)(screen_view_defaults_data))
#define screen_fade_starts_ADDRESS 0x01173Au
extern const void *screen_fade_starts_data;
#define screen_fade_starts ((const struct screen_fade_start *)(screen_fade_starts_data))
#define race_opening_sound_cues_ADDRESS 0x014660u
extern const void *race_opening_sound_cues_data;
#define race_opening_sound_cues ((const struct sound_cue *)(race_opening_sound_cues_data))
#define stage_end_sound_cues_ADDRESS 0x01466Fu
extern const void *stage_end_sound_cues_data;
#define stage_end_sound_cues ((const struct sound_cue *)(stage_end_sound_cues_data))
#define road_surface_layer_setups_ADDRESS 0x01710Cu
extern const void *road_surface_layer_setups_data;
#define road_surface_layer_setups ((const struct road_surface_layer_setup *)(road_surface_layer_setups_data))
#define race_background_layer_setups_ADDRESS 0x017A50u
extern const void *race_background_layer_setups_data;
#define race_background_layer_setups ((const struct race_background_layer_setup *)(race_background_layer_setups_data))
#define lcd_custom_characters_ADDRESS 0x018B4Eu
extern const void *lcd_custom_characters_data;
#define lcd_custom_characters ((const uint8_t *)(lcd_custom_characters_data))
#define lcd_boot_text_ADDRESS 0x018B90u
extern const void *lcd_boot_text_data;
#define lcd_boot_text ((const struct lcd_text_line *)(lcd_boot_text_data))
#define radio_station_title_ADDRESS 0x01954Cu
extern const void *radio_station_title_data;
#define radio_station_title ((const struct song_title_record *)(radio_station_title_data))
#define ignition_key_prompt_pieces_ADDRESS 0x01A95Bu
extern const void *ignition_key_prompt_pieces_data;
#define ignition_key_prompt_pieces ((const struct picture_record *)(ignition_key_prompt_pieces_data))
#define title_car_records_ADDRESS 0x01ABB4u
extern const void *title_car_records_data;
#define title_car_records ((const struct title_car_record *)(title_car_records_data))
#define title_pictures_ADDRESS 0x01AC70u
extern const void *title_pictures_data;
#define title_pictures ((const struct picture_record *)(title_pictures_data))
#define title_seagull_places_ADDRESS 0x01ACA0u
extern const void *title_seagull_places_data;
#define title_seagull_places ((const struct title_place *)(title_seagull_places_data))
#define title_logo_stripe_places_ADDRESS 0x01ACA8u
extern const void *title_logo_stripe_places_data;
#define title_logo_stripe_places ((const struct title_place *)(title_logo_stripe_places_data))
#define title_logo_car_places_ADDRESS 0x01ACB4u
extern const void *title_logo_car_places_data;
#define title_logo_car_places ((const struct title_place *)(title_logo_car_places_data))
#define title_screen_layer_setups_ADDRESS 0x01ACCCu
extern const void *title_screen_layer_setups_data;
#define title_screen_layer_setups ((const struct title_screen_layer_setup *)(title_screen_layer_setups_data))
#define title_background_setups_ADDRESS 0x01AD0Cu
extern const void *title_background_setups_data;
#define title_background_setups ((const struct background_setup *)(title_background_setups_data))
#define demo_caption_pictures_ADDRESS 0x01AF00u
extern const void *demo_caption_pictures_data;
#define demo_caption_pictures ((const struct picture_record *)(demo_caption_pictures_data))
#define memory_test_result_texts_ADDRESS 0x01CDC8u
extern const void *memory_test_result_texts_data;
#define memory_test_result_texts ((const struct memory_test_result_text *)(memory_test_result_texts_data))
#define output_test_outputs_ADDRESS 0x01D5C0u
extern const void *output_test_outputs_data;
#define output_test_outputs ((const struct output_test_output *)(output_test_outputs_data))
#define coin_setting_records_ADDRESS 0x01E3E8u
extern const void *coin_setting_records_data;
#define coin_setting_records ((const struct coin_setting_record *)(coin_setting_records_data))
#define privilege_mode_settings_ADDRESS 0x01E6E5u
extern const void *privilege_mode_settings_data;
#define privilege_mode_settings ((const struct privilege_mode_setting *)(privilege_mode_settings_data))
#define sprite_palettes_ADDRESS 0x032BA4u
extern const void *sprite_palettes_data;
#define sprite_palettes ((const uint32_t *)(sprite_palettes_data))
#define sprite_animations_ADDRESS 0x034138u
extern const void *sprite_animations_data;
#define sprite_animations ((const uint32_t *)(sprite_animations_data))
#define fork_branch_piece_animation_ADDRESS 0x034308u
extern const void *fork_branch_piece_animation_data;
#define fork_branch_piece_animation ((const uint32_t *)(fork_branch_piece_animation_data))
#define stage_courses_ADDRESS 0x03CC00u
extern const void *stage_courses_data;
#define stage_courses ((const uint32_t *)(stage_courses_data))
#define sprite_size_steps_ADDRESS 0x122B74u
extern const void *sprite_size_steps_data;
#define sprite_size_steps ((const struct sprite_size_step *)(sprite_size_steps_data))
#define road_row_size_step_lists_ADDRESS 0x1598CCu
extern const void *road_row_size_step_lists_data;
#define road_row_size_step_lists ((const uint32_t *)(road_row_size_step_lists_data))
#define road_row_size_shift_lists_ADDRESS 0x159AFCu
extern const void *road_row_size_shift_lists_data;
#define road_row_size_shift_lists ((const uint32_t *)(road_row_size_shift_lists_data))

/* The game's state: the work RAM (128 KB at 200000 on the board), laid out as the original had it,
   so that each frame's state compares byte for byte with the original's. The code reaches it
   through game: game->game_frame_count; the runtime keeps one (runtime/src/game.c). The stacks are
   the V70's, which the decompiled program does not use */
#pragma pack(push, 1)
struct game
{
	struct object_area object_area_1;
	struct screen screen_1;
	uint32_t waiting_task_frame;
	uint32_t waiting_task_stack;
	uint8_t pad_20E008[0x2];
	uint16_t vblanks_since_start;
	uint8_t pad_20E00C[0x2];
	uint8_t debug_timer_rounds;
	uint8_t pad_20E00F[0x11];
	uint16_t sprite_list_buffers;
	uint8_t sprite_lists_set_up;
	uint8_t frame_ready;
	uint8_t single_task;
	uint8_t pad_20E025[0x1];
	uint8_t game_frame_count;
	uint8_t send_video_registers;
	uint8_t task_swapping;
	uint8_t pad_20E029[0x2];
	uint8_t second_task_waiting;
	uint8_t timer_interrupts_left;
	uint8_t vblank_counter;
	uint8_t cleared_on_race_over;
	uint8_t pad_20E02F[0x1];
	uint8_t car_numbers[0x8];
	uint16_t link_wait_frames_start;
	uint16_t total_time_count;
	uint8_t pad_20E03C[0x4];
	uint8_t machine_count;
	uint8_t player_count;
	uint8_t link_off;
	uint8_t link_failed;
	uint8_t pad_20E044[0x2];
	uint16_t link_wait_frames;
	uint16_t network_check_blink_counter;
	uint8_t isdn_players_this_side;
	uint8_t isdn_players_other_side;
	uint8_t pad_20E04C[0x14];
	uint32_t link_wait_count;
	uint8_t pad_20E064[0x1C];
	uint8_t coin_meters;
	uint8_t pad_20E081[0xB];
	uint8_t seat1_coin_sounds;
	uint8_t seat2_coin_sounds;
	uint8_t seat1_service_sounds;
	uint8_t seat2_service_sounds;
	uint8_t pad_20E090[0x1];
	uint8_t chute_entry_offset;
	uint8_t port_d_outputs[0x2];
	uint8_t port_g_outputs[0x2];
	uint8_t pad_20E096[0xA];
	uint8_t seat1_credits;
	uint8_t seat1_coin_units;
	uint8_t pad_20E0A2[0x2];
	uint8_t seat1_bookkeeping_byte;
	uint8_t pad_20E0A5[0x3];
	uint8_t seat2_credits;
	uint8_t seat2_coin_units;
	uint8_t pad_20E0AA[0x2];
	uint8_t seat2_bookkeeping_byte;
	uint8_t pad_20E0AD[0x13];
	union
	{
		uint8_t checkpoint_lamp[0x8];
		struct
		{
			uint8_t pad_to_checkpoint_lamp_blinks[0x1];
			uint8_t checkpoint_lamp_blinks;
			uint8_t checkpoint_lamp_period;
			uint8_t checkpoint_lamp_timer;
			uint8_t checkpoint_lamp_2;
			uint8_t checkpoint_lamp_2_blinks;
			uint8_t checkpoint_lamp_2_period;
			uint8_t checkpoint_lamp_2_timer;
		};
	};
	union
	{
		uint8_t leader_lamp[0x8];
		struct
		{
			uint8_t pad_to_leader_lamp_blinks[0x1];
			uint8_t leader_lamp_blinks;
			uint8_t leader_lamp_period;
			uint8_t leader_lamp_timer;
			uint8_t leader_lamp_2;
			uint8_t leader_lamp_2_blinks;
			uint8_t leader_lamp_2_period;
			uint8_t leader_lamp_2_timer;
		};
	};
	uint8_t wheel_motor[0x2];
	uint8_t pad_20E0D2[0x2A];
	uint8_t screen_2_vblanks;
	uint8_t screen_2_vblanks_high;
	uint8_t screen_1_vblanks;
	uint8_t screen_1_vblanks_high;
	uint8_t switches;
	uint8_t previous_switches;
	uint8_t switches_pressed;
	uint8_t switches_released;
	uint8_t second_io_chip_port_f;
	uint8_t pad_20E105[0xB];
	uint8_t analog_raw;
	uint8_t pad_20E111[0x5];
	uint8_t analog_inputs;
	uint8_t analog_gas_1;
	uint8_t analog_brake_1;
	uint8_t analog_wheel_2;
	uint8_t analog_gas_2;
	uint8_t analog_brake_2;
	uint8_t pad_20E11C[0x4];
	uint8_t seat_1_player;
	uint8_t seat_1_buttons;
	uint8_t seat_1_wheel;
	uint8_t seat_1_gas;
	uint8_t seat_1_brake;
	uint8_t seat_2_player;
	uint8_t seat_2_buttons;
	uint8_t seat_2_wheel;
	uint8_t seat_2_gas;
	uint8_t seat_2_brake;
	uint8_t pad_20E12A[0xD6];
	uint16_t video_registers[0x48];
	uint32_t scroll_targets[0x8];
	uint8_t tile_banks[0x4];
	uint8_t pad_20E2B4[0x4C];
	union
	{
		uint8_t course_records[0x3C];
		struct
		{
			uint8_t pad_to_course_record_times[0x4];
			uint16_t course_record_times[0x1C];
		};
	};
	uint8_t pad_20E33C[0x84];
	uint8_t lcd_send_buffer_1[0x10];
	uint8_t lcd_send_buffer_2[0x10];
	uint8_t pad_20E3E0[0x10];
	uint16_t lcd_restart_frames;
	uint8_t lcd_send_step;
	uint8_t lcd_busy;
	uint8_t lcd_init_step;
	uint8_t pad_20E3F5[0x20B];
	uint32_t settings_magic;
	uint16_t settings_signature;
	uint8_t pad_20E606[0x2];
	uint16_t settings_checksum;
	uint16_t eeprom_write_count;
	uint8_t flip_screen;
	uint8_t advertise_sound;
	uint8_t pad_20E60E[0x2];
	uint8_t coins_per_credit;
	uint8_t first_chute_multiplier;
	uint8_t second_chute_multiplier;
	uint8_t bonus_adder;
	uint8_t coin_setting;
	uint8_t free_play;
	uint8_t pad_20E616[0xA];
	uint8_t stand_alone;
	uint8_t link_master;
	uint8_t cabinet_id;
	uint8_t isdn_mode;
	uint8_t steering_wheel_reaction;
	uint8_t pad_20E625[0xB];
	uint8_t game_difficulty;
	uint8_t pad_20E631[0x1];
	int8_t time_difficulty[0x6];
	uint8_t volume_ranges;
	uint8_t pad_20E639[0xB];
	uint16_t backup_settings_checksum;
	uint16_t backup_eeprom_write_count;
	uint8_t pad_20E648[0xB8];
	uint8_t test_mode_on;
	uint8_t test_mode_flags;
	uint8_t test_mode_cursor;
	uint8_t test_mode_page_cursor;
	uint32_t test_mode_routine;
	union
	{
		uint32_t test_mode_blink_address;
		uint32_t test_mode_prompt_text;
	};
	uint8_t test_mode_buttons;
	uint8_t test_mode_buttons_previous;
	uint8_t test_mode_buttons_pressed;
	uint8_t test_mode_buttons_released;
	uint8_t test_mode_analog_inputs;
	uint8_t pad_20E711[0x5];
	uint8_t memory_test_results[0x19];
	int8_t test_mode_blink_timer;
	uint8_t sound_test_number_a;
	uint8_t sound_test_number_b;
	uint8_t test_mode_sound_queue_write;
	uint8_t test_mode_sound_queue_read;
	uint16_t test_mode_sound_queue[0x10];
	uint8_t test_mode_privilege_mode;
	uint8_t pad_20E755[0x13AB];
	struct race races[2];
	union
	{
		struct race race_copies[8];
		struct
		{
			uint8_t pad_to_master_time_difficulty[0xB];
			int8_t master_time_difficulty[0x6];
			uint8_t pad_to_race_copy_window_segments[0x35];
			uint16_t race_copy_window_segments[0x1DD];
		};
	};
	struct object_area object_area_2;
	struct screen screen_2;
	struct race isdn_races[8];
	uint8_t pad_21E400[0xC00];
	uint8_t stacks[0x1000];
};
#pragma pack(pop)
_Static_assert(sizeof(struct game) == 0x20000, "struct game is the work RAM");
_Static_assert(offsetof(struct game, object_area_1) == 0x0, "object_area_1 at its place in the work RAM");
_Static_assert(offsetof(struct game, screen_1) == 0x8000, "screen_1 at its place in the work RAM");
_Static_assert(offsetof(struct game, races) == 0xFB00, "races at its place in the work RAM");
_Static_assert(offsetof(struct game, race_copies) == 0xFC00, "race_copies at its place in the work RAM");
_Static_assert(offsetof(struct game, object_area_2) == 0x10000, "object_area_2 at its place in the work RAM");
_Static_assert(offsetof(struct game, screen_2) == 0x18000, "screen_2 at its place in the work RAM");
_Static_assert(offsetof(struct game, isdn_races) == 0x1E000, "isdn_races at its place in the work RAM");
_Static_assert(offsetof(struct game, stacks) == 0x1F000, "stacks at its place in the work RAM");
extern struct game *game;

#endif
