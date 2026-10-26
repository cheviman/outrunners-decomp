/**
 *    Name:        ram.h
 *    Description: the game's variables, tables, board registers and structures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#ifndef ORUNNERS_RAM_H
#define ORUNNERS_RAM_H

/* the game's variables in work RAM; SCREEN_1 and SCREEN_2 are the blocks of the two screens,
   which the code reaches through screen (R25 of the task that runs each) */
#define GAME_VARIABLES 0x20E000u
#define SCREEN_1 0x208000u
#define RACE_1 0x20FB00u
#define RACE_2 0x20FB80u
#define RACE_COPY_1 0x20FC00u
#define RACE_COPY_2 0x20FC80u
#define SCREEN_2 0x218000u
#define unk_208001_ADDRESS 0x208001u
#define unk_208001 RAM8(unk_208001_ADDRESS)
#define unk_208002_ADDRESS 0x208002u
#define unk_208002 RAM8(unk_208002_ADDRESS)
#define unk_208020_ADDRESS 0x208020u
#define unk_208020 RAM32(unk_208020_ADDRESS)
#define unk_208024_ADDRESS 0x208024u
#define unk_208024 RAM32(unk_208024_ADDRESS)
#define unk_208080_ADDRESS 0x208080u
#define unk_208080 RAM16(unk_208080_ADDRESS)
#define unk_208090_ADDRESS 0x208090u
#define unk_208090 RAM32(unk_208090_ADDRESS)
#define unk_208094_ADDRESS 0x208094u
#define unk_208094 RAM32(unk_208094_ADDRESS)
#define unk_20809E_ADDRESS 0x20809Eu
#define unk_20809E RAM8(unk_20809E_ADDRESS)
#define unk_2080B0_ADDRESS 0x2080B0u
#define unk_2080B0 RAM8(unk_2080B0_ADDRESS)
#define unk_2080B4_ADDRESS 0x2080B4u
#define unk_2080B4 RAM32(unk_2080B4_ADDRESS)
#define unk_2080C0_ADDRESS 0x2080C0u
#define unk_2080C0 RAM32(unk_2080C0_ADDRESS)
#define unk_208153_ADDRESS 0x208153u
#define unk_208153 RAM8(unk_208153_ADDRESS)
#define unk_208B00_ADDRESS 0x208B00u
#define unk_208B00 RAM32(unk_208B00_ADDRESS)
#define unk_208F10_ADDRESS 0x208F10u
#define unk_208F10 RAM32(unk_208F10_ADDRESS)
#define unk_208F14_ADDRESS 0x208F14u
#define unk_208F14 RAM32(unk_208F14_ADDRESS)
#define unk_208F18_ADDRESS 0x208F18u
#define unk_208F18 RAM32(unk_208F18_ADDRESS)
#define unk_208F1C_ADDRESS 0x208F1Cu
#define unk_208F1C RAM32(unk_208F1C_ADDRESS)
#define unk_208F20_ADDRESS 0x208F20u
#define unk_208F20 RAM16(unk_208F20_ADDRESS)
#define waiting_task_frame_ADDRESS 0x20E000u
#define waiting_task_frame RAM32(waiting_task_frame_ADDRESS)
#define waiting_task_stack_ADDRESS 0x20E004u
#define waiting_task_stack RAM32(waiting_task_stack_ADDRESS)
#define vblanks_since_start_ADDRESS 0x20E00Au
#define vblanks_since_start RAM16(vblanks_since_start_ADDRESS)
#define debug_timer_rounds_ADDRESS 0x20E00Eu
#define debug_timer_rounds RAM8(debug_timer_rounds_ADDRESS)
#define sprite_list_buffers_ADDRESS 0x20E020u
#define sprite_list_buffers RAM16(sprite_list_buffers_ADDRESS)
#define sprite_lists_set_up_ADDRESS 0x20E022u
#define sprite_lists_set_up RAM8(sprite_lists_set_up_ADDRESS)
#define frame_ready_ADDRESS 0x20E023u
#define frame_ready RAM8(frame_ready_ADDRESS)
#define single_task_ADDRESS 0x20E024u
#define single_task RAM8(single_task_ADDRESS)
#define game_frame_count_ADDRESS 0x20E026u
#define game_frame_count RAM8(game_frame_count_ADDRESS)
#define send_video_registers_ADDRESS 0x20E027u
#define send_video_registers RAM8(send_video_registers_ADDRESS)
#define task_swapping_ADDRESS 0x20E028u
#define task_swapping RAM8(task_swapping_ADDRESS)
#define second_task_waiting_ADDRESS 0x20E02Bu
#define second_task_waiting RAM8(second_task_waiting_ADDRESS)
#define timer_interrupts_left_ADDRESS 0x20E02Cu
#define timer_interrupts_left RAM8(timer_interrupts_left_ADDRESS)
#define vblank_counter_ADDRESS 0x20E02Du
#define vblank_counter RAM8(vblank_counter_ADDRESS)
#define cleared_on_race_over_ADDRESS 0x20E02Eu
#define cleared_on_race_over RAM8(cleared_on_race_over_ADDRESS)
#define car_numbers_ADDRESS 0x20E030u
#define car_numbers ((uint8_t *)ram(car_numbers_ADDRESS))
#define link_wait_frames_start_ADDRESS 0x20E038u
#define link_wait_frames_start RAM16(link_wait_frames_start_ADDRESS)
#define total_time_count_ADDRESS 0x20E03Au
#define total_time_count RAM16(total_time_count_ADDRESS)
#define machine_count_ADDRESS 0x20E040u
#define machine_count RAM8(machine_count_ADDRESS)
#define player_count_ADDRESS 0x20E041u
#define player_count RAM8(player_count_ADDRESS)
#define link_off_ADDRESS 0x20E042u
#define link_off RAM8(link_off_ADDRESS)
#define link_failed_ADDRESS 0x20E043u
#define link_failed RAM8(link_failed_ADDRESS)
#define link_wait_frames_ADDRESS 0x20E046u
#define link_wait_frames RAM16(link_wait_frames_ADDRESS)
#define network_check_blink_counter_ADDRESS 0x20E048u
#define network_check_blink_counter RAM16(network_check_blink_counter_ADDRESS)
#define isdn_players_this_side_ADDRESS 0x20E04Au
#define isdn_players_this_side RAM8(isdn_players_this_side_ADDRESS)
#define isdn_players_other_side_ADDRESS 0x20E04Bu
#define isdn_players_other_side RAM8(isdn_players_other_side_ADDRESS)
#define link_wait_count_ADDRESS 0x20E060u
#define link_wait_count RAM32(link_wait_count_ADDRESS)
#define coin_meters_ADDRESS 0x20E080u
#define coin_meters RAM8(coin_meters_ADDRESS)
#define seat1_coin_sounds_ADDRESS 0x20E08Cu
#define seat1_coin_sounds RAM8(seat1_coin_sounds_ADDRESS)
#define seat2_coin_sounds_ADDRESS 0x20E08Du
#define seat2_coin_sounds RAM8(seat2_coin_sounds_ADDRESS)
#define seat1_service_sounds_ADDRESS 0x20E08Eu
#define seat1_service_sounds RAM8(seat1_service_sounds_ADDRESS)
#define seat2_service_sounds_ADDRESS 0x20E08Fu
#define seat2_service_sounds RAM8(seat2_service_sounds_ADDRESS)
#define chute_entry_offset_ADDRESS 0x20E091u
#define chute_entry_offset RAM8(chute_entry_offset_ADDRESS)
#define port_d_outputs_ADDRESS 0x20E092u
#define port_d_outputs ((uint8_t *)ram(port_d_outputs_ADDRESS))
#define port_g_outputs_ADDRESS 0x20E094u
#define port_g_outputs ((uint8_t *)ram(port_g_outputs_ADDRESS))
#define seat1_credits_ADDRESS 0x20E0A0u
#define seat1_credits RAM8(seat1_credits_ADDRESS)
#define seat1_coin_units_ADDRESS 0x20E0A1u
#define seat1_coin_units RAM8(seat1_coin_units_ADDRESS)
#define seat1_bookkeeping_byte_ADDRESS 0x20E0A4u
#define seat1_bookkeeping_byte RAM8(seat1_bookkeeping_byte_ADDRESS)
#define seat2_credits_ADDRESS 0x20E0A8u
#define seat2_credits RAM8(seat2_credits_ADDRESS)
#define seat2_coin_units_ADDRESS 0x20E0A9u
#define seat2_coin_units RAM8(seat2_coin_units_ADDRESS)
#define seat2_bookkeeping_byte_ADDRESS 0x20E0ACu
#define seat2_bookkeeping_byte RAM8(seat2_bookkeeping_byte_ADDRESS)
#define checkpoint_lamp_ADDRESS 0x20E0C0u
#define checkpoint_lamp ((uint8_t *)ram(checkpoint_lamp_ADDRESS))
#define checkpoint_lamp_blinks_ADDRESS 0x20E0C1u
#define checkpoint_lamp_blinks RAM8(checkpoint_lamp_blinks_ADDRESS)
#define checkpoint_lamp_period_ADDRESS 0x20E0C2u
#define checkpoint_lamp_period RAM8(checkpoint_lamp_period_ADDRESS)
#define checkpoint_lamp_timer_ADDRESS 0x20E0C3u
#define checkpoint_lamp_timer RAM8(checkpoint_lamp_timer_ADDRESS)
#define checkpoint_lamp_2_ADDRESS 0x20E0C4u
#define checkpoint_lamp_2 RAM8(checkpoint_lamp_2_ADDRESS)
#define checkpoint_lamp_2_blinks_ADDRESS 0x20E0C5u
#define checkpoint_lamp_2_blinks RAM8(checkpoint_lamp_2_blinks_ADDRESS)
#define checkpoint_lamp_2_period_ADDRESS 0x20E0C6u
#define checkpoint_lamp_2_period RAM8(checkpoint_lamp_2_period_ADDRESS)
#define checkpoint_lamp_2_timer_ADDRESS 0x20E0C7u
#define checkpoint_lamp_2_timer RAM8(checkpoint_lamp_2_timer_ADDRESS)
#define leader_lamp_ADDRESS 0x20E0C8u
#define leader_lamp ((uint8_t *)ram(leader_lamp_ADDRESS))
#define leader_lamp_blinks_ADDRESS 0x20E0C9u
#define leader_lamp_blinks RAM8(leader_lamp_blinks_ADDRESS)
#define leader_lamp_period_ADDRESS 0x20E0CAu
#define leader_lamp_period RAM8(leader_lamp_period_ADDRESS)
#define leader_lamp_timer_ADDRESS 0x20E0CBu
#define leader_lamp_timer RAM8(leader_lamp_timer_ADDRESS)
#define leader_lamp_2_ADDRESS 0x20E0CCu
#define leader_lamp_2 RAM8(leader_lamp_2_ADDRESS)
#define leader_lamp_2_blinks_ADDRESS 0x20E0CDu
#define leader_lamp_2_blinks RAM8(leader_lamp_2_blinks_ADDRESS)
#define leader_lamp_2_period_ADDRESS 0x20E0CEu
#define leader_lamp_2_period RAM8(leader_lamp_2_period_ADDRESS)
#define leader_lamp_2_timer_ADDRESS 0x20E0CFu
#define leader_lamp_2_timer RAM8(leader_lamp_2_timer_ADDRESS)
#define wheel_motor_ADDRESS 0x20E0D0u
#define wheel_motor ((uint8_t *)ram(wheel_motor_ADDRESS))
#define screen_2_vblanks_ADDRESS 0x20E0FCu
#define screen_2_vblanks RAM8(screen_2_vblanks_ADDRESS)
#define screen_2_vblanks_high_ADDRESS 0x20E0FDu
#define screen_2_vblanks_high RAM8(screen_2_vblanks_high_ADDRESS)
#define screen_1_vblanks_ADDRESS 0x20E0FEu
#define screen_1_vblanks RAM8(screen_1_vblanks_ADDRESS)
#define screen_1_vblanks_high_ADDRESS 0x20E0FFu
#define screen_1_vblanks_high RAM8(screen_1_vblanks_high_ADDRESS)
#define switches_ADDRESS 0x20E100u
#define switches RAM8(switches_ADDRESS)
#define previous_switches_ADDRESS 0x20E101u
#define previous_switches RAM8(previous_switches_ADDRESS)
#define switches_pressed_ADDRESS 0x20E102u
#define switches_pressed RAM8(switches_pressed_ADDRESS)
#define switches_released_ADDRESS 0x20E103u
#define switches_released RAM8(switches_released_ADDRESS)
#define second_io_chip_port_f_ADDRESS 0x20E104u
#define second_io_chip_port_f RAM8(second_io_chip_port_f_ADDRESS)
#define analog_raw_ADDRESS 0x20E110u
#define analog_raw RAM8(analog_raw_ADDRESS)
#define analog_inputs_ADDRESS 0x20E116u
#define analog_inputs RAM8(analog_inputs_ADDRESS)
#define analog_gas_1_ADDRESS 0x20E117u
#define analog_gas_1 RAM8(analog_gas_1_ADDRESS)
#define analog_brake_1_ADDRESS 0x20E118u
#define analog_brake_1 RAM8(analog_brake_1_ADDRESS)
#define analog_wheel_2_ADDRESS 0x20E119u
#define analog_wheel_2 RAM8(analog_wheel_2_ADDRESS)
#define analog_gas_2_ADDRESS 0x20E11Au
#define analog_gas_2 RAM8(analog_gas_2_ADDRESS)
#define analog_brake_2_ADDRESS 0x20E11Bu
#define analog_brake_2 RAM8(analog_brake_2_ADDRESS)
#define seat_1_player_ADDRESS 0x20E120u
#define seat_1_player RAM8(seat_1_player_ADDRESS)
#define seat_1_buttons_ADDRESS 0x20E121u
#define seat_1_buttons RAM8(seat_1_buttons_ADDRESS)
#define seat_1_wheel_ADDRESS 0x20E122u
#define seat_1_wheel RAM8(seat_1_wheel_ADDRESS)
#define seat_1_gas_ADDRESS 0x20E123u
#define seat_1_gas RAM8(seat_1_gas_ADDRESS)
#define seat_1_brake_ADDRESS 0x20E124u
#define seat_1_brake RAM8(seat_1_brake_ADDRESS)
#define seat_2_player_ADDRESS 0x20E125u
#define seat_2_player RAM8(seat_2_player_ADDRESS)
#define seat_2_buttons_ADDRESS 0x20E126u
#define seat_2_buttons RAM8(seat_2_buttons_ADDRESS)
#define seat_2_wheel_ADDRESS 0x20E127u
#define seat_2_wheel RAM8(seat_2_wheel_ADDRESS)
#define seat_2_gas_ADDRESS 0x20E128u
#define seat_2_gas RAM8(seat_2_gas_ADDRESS)
#define seat_2_brake_ADDRESS 0x20E129u
#define seat_2_brake RAM8(seat_2_brake_ADDRESS)
#define video_registers_ADDRESS 0x20E200u
#define video_registers ((uint16_t *)ram(video_registers_ADDRESS))
#define scroll_targets_ADDRESS 0x20E290u
#define scroll_targets ((uint32_t *)ram(scroll_targets_ADDRESS))
#define tile_banks_ADDRESS 0x20E2B0u
#define tile_banks ((uint8_t *)ram(tile_banks_ADDRESS))
#define course_records_ADDRESS 0x20E300u
#define course_records ((uint8_t *)ram(course_records_ADDRESS))
#define course_record_times_ADDRESS 0x20E304u
#define course_record_times ((uint16_t *)ram(course_record_times_ADDRESS))
#define lcd_send_buffer_1_ADDRESS 0x20E3C0u
#define lcd_send_buffer_1 ((uint8_t *)ram(lcd_send_buffer_1_ADDRESS))
#define lcd_send_buffer_2_ADDRESS 0x20E3D0u
#define lcd_send_buffer_2 ((uint8_t *)ram(lcd_send_buffer_2_ADDRESS))
#define lcd_restart_frames_ADDRESS 0x20E3F0u
#define lcd_restart_frames RAM16(lcd_restart_frames_ADDRESS)
#define lcd_send_step_ADDRESS 0x20E3F2u
#define lcd_send_step RAM8(lcd_send_step_ADDRESS)
#define lcd_busy_ADDRESS 0x20E3F3u
#define lcd_busy RAM8(lcd_busy_ADDRESS)
#define lcd_init_step_ADDRESS 0x20E3F4u
#define lcd_init_step RAM8(lcd_init_step_ADDRESS)
#define settings_magic_ADDRESS 0x20E600u
#define settings_magic RAM32(settings_magic_ADDRESS)
#define settings_signature_ADDRESS 0x20E604u
#define settings_signature RAM16(settings_signature_ADDRESS)
#define settings_checksum_ADDRESS 0x20E608u
#define settings_checksum RAM16(settings_checksum_ADDRESS)
#define eeprom_write_count_ADDRESS 0x20E60Au
#define eeprom_write_count RAM16(eeprom_write_count_ADDRESS)
#define flip_screen_ADDRESS 0x20E60Cu
#define flip_screen RAM8(flip_screen_ADDRESS)
#define advertise_sound_ADDRESS 0x20E60Du
#define advertise_sound RAM8(advertise_sound_ADDRESS)
#define coins_per_credit_ADDRESS 0x20E610u
#define coins_per_credit RAM8(coins_per_credit_ADDRESS)
#define first_chute_multiplier_ADDRESS 0x20E611u
#define first_chute_multiplier RAM8(first_chute_multiplier_ADDRESS)
#define second_chute_multiplier_ADDRESS 0x20E612u
#define second_chute_multiplier RAM8(second_chute_multiplier_ADDRESS)
#define bonus_adder_ADDRESS 0x20E613u
#define bonus_adder RAM8(bonus_adder_ADDRESS)
#define coin_setting_ADDRESS 0x20E614u
#define coin_setting RAM8(coin_setting_ADDRESS)
#define free_play_ADDRESS 0x20E615u
#define free_play RAM8(free_play_ADDRESS)
#define stand_alone_ADDRESS 0x20E620u
#define stand_alone RAM8(stand_alone_ADDRESS)
#define link_master_ADDRESS 0x20E621u
#define link_master RAM8(link_master_ADDRESS)
#define cabinet_id_ADDRESS 0x20E622u
#define cabinet_id RAM8(cabinet_id_ADDRESS)
#define isdn_mode_ADDRESS 0x20E623u
#define isdn_mode RAM8(isdn_mode_ADDRESS)
#define steering_wheel_reaction_ADDRESS 0x20E624u
#define steering_wheel_reaction RAM8(steering_wheel_reaction_ADDRESS)
#define game_difficulty_ADDRESS 0x20E630u
#define game_difficulty RAM8(game_difficulty_ADDRESS)
#define time_difficulty_ADDRESS 0x20E632u
#define time_difficulty ((int8_t *)ram(time_difficulty_ADDRESS))
#define volume_ranges_ADDRESS 0x20E638u
#define volume_ranges RAM8(volume_ranges_ADDRESS)
#define backup_settings_checksum_ADDRESS 0x20E644u
#define backup_settings_checksum RAM16(backup_settings_checksum_ADDRESS)
#define backup_eeprom_write_count_ADDRESS 0x20E646u
#define backup_eeprom_write_count RAM16(backup_eeprom_write_count_ADDRESS)
#define test_mode_on_ADDRESS 0x20E700u
#define test_mode_on RAM8(test_mode_on_ADDRESS)
#define test_mode_flags_ADDRESS 0x20E701u
#define test_mode_flags RAM8(test_mode_flags_ADDRESS)
#define test_mode_cursor_ADDRESS 0x20E702u
#define test_mode_cursor RAM8(test_mode_cursor_ADDRESS)
#define test_mode_page_cursor_ADDRESS 0x20E703u
#define test_mode_page_cursor RAM8(test_mode_page_cursor_ADDRESS)
#define test_mode_routine_ADDRESS 0x20E704u
#define test_mode_routine RAM32(test_mode_routine_ADDRESS)
#define test_mode_blink_address_ADDRESS 0x20E708u
#define test_mode_blink_address RAM32(test_mode_blink_address_ADDRESS)
#define test_mode_buttons_ADDRESS 0x20E70Cu
#define test_mode_buttons RAM8(test_mode_buttons_ADDRESS)
#define test_mode_buttons_previous_ADDRESS 0x20E70Du
#define test_mode_buttons_previous RAM8(test_mode_buttons_previous_ADDRESS)
#define test_mode_buttons_pressed_ADDRESS 0x20E70Eu
#define test_mode_buttons_pressed RAM8(test_mode_buttons_pressed_ADDRESS)
#define test_mode_buttons_released_ADDRESS 0x20E70Fu
#define test_mode_buttons_released RAM8(test_mode_buttons_released_ADDRESS)
#define test_mode_analog_inputs_ADDRESS 0x20E710u
#define test_mode_analog_inputs RAM8(test_mode_analog_inputs_ADDRESS)
#define memory_test_results_ADDRESS 0x20E716u
#define memory_test_results ((uint8_t *)ram(memory_test_results_ADDRESS))
#define test_mode_blink_timer_ADDRESS 0x20E72Fu
#define test_mode_blink_timer RAM8S(test_mode_blink_timer_ADDRESS)
#define sound_test_number_a_ADDRESS 0x20E730u
#define sound_test_number_a RAM8(sound_test_number_a_ADDRESS)
#define sound_test_number_b_ADDRESS 0x20E731u
#define sound_test_number_b RAM8(sound_test_number_b_ADDRESS)
#define test_mode_sound_queue_write_ADDRESS 0x20E732u
#define test_mode_sound_queue_write RAM8(test_mode_sound_queue_write_ADDRESS)
#define test_mode_sound_queue_read_ADDRESS 0x20E733u
#define test_mode_sound_queue_read RAM8(test_mode_sound_queue_read_ADDRESS)
#define test_mode_sound_queue_ADDRESS 0x20E734u
#define test_mode_sound_queue ((uint16_t *)ram(test_mode_sound_queue_ADDRESS))
#define test_mode_privilege_mode_ADDRESS 0x20E754u
#define test_mode_privilege_mode RAM8(test_mode_privilege_mode_ADDRESS)
#define unk_20FB00_ADDRESS 0x20FB00u
#define unk_20FB00 RAM8(unk_20FB00_ADDRESS)
#define unk_20FB01_ADDRESS 0x20FB01u
#define unk_20FB01 RAM8(unk_20FB01_ADDRESS)
#define unk_20FB02_ADDRESS 0x20FB02u
#define unk_20FB02 RAM8(unk_20FB02_ADDRESS)
#define unk_20FB80_ADDRESS 0x20FB80u
#define unk_20FB80 RAM8(unk_20FB80_ADDRESS)
#define unk_20FB81_ADDRESS 0x20FB81u
#define unk_20FB81 RAM8(unk_20FB81_ADDRESS)
#define unk_20FB82_ADDRESS 0x20FB82u
#define unk_20FB82 RAM8(unk_20FB82_ADDRESS)
#define unk_20FC08_ADDRESS 0x20FC08u
#define unk_20FC08 RAM8(unk_20FC08_ADDRESS)
#define master_time_difficulty_ADDRESS 0x20FC0Bu
#define master_time_difficulty ((int8_t *)ram(master_time_difficulty_ADDRESS))
#define unk_20FC1A_ADDRESS 0x20FC1Au
#define unk_20FC1A RAM16(unk_20FC1A_ADDRESS)
#define race_copy_window_segments_ADDRESS 0x20FC46u
#define race_copy_window_segments ((uint16_t *)ram(race_copy_window_segments_ADDRESS))
#define unk_218001_ADDRESS 0x218001u
#define unk_218001 RAM8(unk_218001_ADDRESS)
#define unk_218002_ADDRESS 0x218002u
#define unk_218002 RAM8(unk_218002_ADDRESS)
#define unk_218020_ADDRESS 0x218020u
#define unk_218020 RAM32(unk_218020_ADDRESS)
#define unk_218024_ADDRESS 0x218024u
#define unk_218024 RAM32(unk_218024_ADDRESS)
#define unk_218080_ADDRESS 0x218080u
#define unk_218080 RAM16(unk_218080_ADDRESS)
#define unk_218090_ADDRESS 0x218090u
#define unk_218090 RAM32(unk_218090_ADDRESS)
#define unk_218094_ADDRESS 0x218094u
#define unk_218094 RAM32(unk_218094_ADDRESS)
#define unk_21809E_ADDRESS 0x21809Eu
#define unk_21809E RAM8(unk_21809E_ADDRESS)
#define unk_2180B0_ADDRESS 0x2180B0u
#define unk_2180B0 RAM8(unk_2180B0_ADDRESS)
#define unk_2180B4_ADDRESS 0x2180B4u
#define unk_2180B4 RAM32(unk_2180B4_ADDRESS)
#define unk_2180C0_ADDRESS 0x2180C0u
#define unk_2180C0 RAM32(unk_2180C0_ADDRESS)
#define unk_218153_ADDRESS 0x218153u
#define unk_218153 RAM8(unk_218153_ADDRESS)
#define unk_218B00_ADDRESS 0x218B00u
#define unk_218B00 RAM32(unk_218B00_ADDRESS)
#define unk_218F10_ADDRESS 0x218F10u
#define unk_218F10 RAM32(unk_218F10_ADDRESS)
#define unk_218F14_ADDRESS 0x218F14u
#define unk_218F14 RAM32(unk_218F14_ADDRESS)
#define unk_218F18_ADDRESS 0x218F18u
#define unk_218F18 RAM32(unk_218F18_ADDRESS)
#define unk_218F1C_ADDRESS 0x218F1Cu
#define unk_218F1C RAM32(unk_218F1C_ADDRESS)
#define unk_218F20_ADDRESS 0x218F20u
#define unk_218F20 RAM16(unk_218F20_ADDRESS)

/* data in the program ROM the code reads */
#define rom_000001_ADDRESS 0x000001u
#define rom_000001 ((const uint8_t *)rom(rom_000001_ADDRESS))
#define rom_000002_ADDRESS 0x000002u
#define rom_000002 ((const uint16_t *)rom(rom_000002_ADDRESS))
#define boot_e80000_bytes_ADDRESS 0x0000B4u
#define boot_e80000_bytes ROM32(boot_e80000_bytes_ADDRESS)
#define seat_credits_addresses_ADDRESS 0x0008C4u
#define seat_credits_addresses ((const uint32_t *)rom(seat_credits_addresses_ADDRESS))
#define game_mode_jumps_ADDRESS 0x0008CCu
#define game_mode_jumps ((const uint32_t *)rom(game_mode_jumps_ADDRESS))
#define analog_calibration_addresses_ADDRESS 0x000B0Cu
#define analog_calibration_addresses ((const uint32_t *)rom(analog_calibration_addresses_ADDRESS))
#define analog_input_counts_ADDRESS 0x000B14u
#define analog_input_counts ((const uint8_t *)rom(analog_input_counts_ADDRESS))
#define first_attract_car_lists_ADDRESS 0x000D14u
#define first_attract_car_lists ((const uint32_t *)rom(first_attract_car_lists_ADDRESS))
#define isdn_connection_step_jumps_ADDRESS 0x000DACu
#define isdn_connection_step_jumps ((const uint32_t *)rom(isdn_connection_step_jumps_ADDRESS))
#define isdn_status_state_jumps_ADDRESS 0x000EB0u
#define isdn_status_state_jumps ((const uint32_t *)rom(isdn_status_state_jumps_ADDRESS))
#define isdn_connecting_texts_ADDRESS 0x000F78u
#define isdn_connecting_texts ((const uint32_t *)rom(isdn_connecting_texts_ADDRESS))
#define isdn_failed_text_ADDRESS 0x000F80u
#define isdn_failed_text ROM32(isdn_failed_text_ADDRESS)
#define isdn_success_text_ADDRESS 0x000F84u
#define isdn_success_text ROM32(isdn_success_text_ADDRESS)
#define isdn_this_side_text_ADDRESS 0x000F88u
#define isdn_this_side_text ROM32(isdn_this_side_text_ADDRESS)
#define isdn_other_side_text_ADDRESS 0x000F8Cu
#define isdn_other_side_text ROM32(isdn_other_side_text_ADDRESS)
#define isdn_total_text_ADDRESS 0x000F90u
#define isdn_total_text ROM32(isdn_total_text_ADDRESS)
#define title_step_jumps_ADDRESS 0x001014u
#define title_step_jumps ((const uint32_t *)rom(title_step_jumps_ADDRESS))
#define car_presentation_step_jumps_ADDRESS 0x00146Cu
#define car_presentation_step_jumps ((const uint32_t *)rom(car_presentation_step_jumps_ADDRESS))
#define demo_race_step_jumps_ADDRESS 0x001530u
#define demo_race_step_jumps ((const uint32_t *)rom(demo_race_step_jumps_ADDRESS))
#define car_presentation_state_jumps_ADDRESS 0x001694u
#define car_presentation_state_jumps ((const uint32_t *)rom(car_presentation_state_jumps_ADDRESS))
#define car_name_letter_state_jumps_ADDRESS 0x0019CCu
#define car_name_letter_state_jumps ((const uint32_t *)rom(car_name_letter_state_jumps_ADDRESS))
#define attract_car_picture_animations_ADDRESS 0x001A88u
#define attract_car_picture_animations ((const uint16_t *)rom(attract_car_picture_animations_ADDRESS))
#define attract_car_scene_palettes_ADDRESS 0x001A98u
#define attract_car_scene_palettes ((const uint8_t *)rom(attract_car_scene_palettes_ADDRESS))
#define car_presentation_palette_cycle_ADDRESS 0x001AC8u
#define car_presentation_palette_cycle ((const uint32_t *)rom(car_presentation_palette_cycle_ADDRESS))
#define attract_car_names_ADDRESS 0x001CECu
#define attract_car_names ((const uint32_t *)rom(attract_car_names_ADDRESS))
#define ranking_step_jumps_ADDRESS 0x001DE0u
#define ranking_step_jumps ((const uint32_t *)rom(ranking_step_jumps_ADDRESS))
#define name_entry_state_jumps_ADDRESS 0x001ED0u
#define name_entry_state_jumps ((const uint32_t *)rom(name_entry_state_jumps_ADDRESS))
#define name_entry_gas_step_jumps_ADDRESS 0x002088u
#define name_entry_gas_step_jumps ((const uint32_t *)rom(name_entry_gas_step_jumps_ADDRESS))
#define name_entry_place_jumps_ADDRESS 0x0020C8u
#define name_entry_place_jumps ((const uint32_t *)rom(name_entry_place_jumps_ADDRESS))
#define name_entry_letter_state_jumps_ADDRESS 0x00221Cu
#define name_entry_letter_state_jumps ((const uint32_t *)rom(name_entry_letter_state_jumps_ADDRESS))
#define name_entry_lcd_state_jumps_ADDRESS 0x002304u
#define name_entry_lcd_state_jumps ((const uint32_t *)rom(name_entry_lcd_state_jumps_ADDRESS))
#define name_entry_car_state_jumps_ADDRESS 0x002384u
#define name_entry_car_state_jumps ((const uint32_t *)rom(name_entry_car_state_jumps_ADDRESS))
#define name_entry_drivers_state_jumps_ADDRESS 0x0023D4u
#define name_entry_drivers_state_jumps ((const uint32_t *)rom(name_entry_drivers_state_jumps_ADDRESS))
#define name_entry_course_name_state_jumps_ADDRESS 0x002424u
#define name_entry_course_name_state_jumps ((const uint32_t *)rom(name_entry_course_name_state_jumps_ADDRESS))
#define ranking_state_jumps_ADDRESS 0x002488u
#define ranking_state_jumps ((const uint32_t *)rom(ranking_state_jumps_ADDRESS))
#define ranking_background_state_jumps_ADDRESS 0x002564u
#define ranking_background_state_jumps ((const uint32_t *)rom(ranking_background_state_jumps_ADDRESS))
#define ranking_title_left_state_jumps_ADDRESS 0x002620u
#define ranking_title_left_state_jumps ((const uint32_t *)rom(ranking_title_left_state_jumps_ADDRESS))
#define ranking_title_right_state_jumps_ADDRESS 0x00266Cu
#define ranking_title_right_state_jumps ((const uint32_t *)rom(ranking_title_right_state_jumps_ADDRESS))
#define ranking_course_banner_state_jumps_ADDRESS 0x0026B8u
#define ranking_course_banner_state_jumps ((const uint32_t *)rom(ranking_course_banner_state_jumps_ADDRESS))
#define ranking_record_character_state_jumps_ADDRESS 0x00279Cu
#define ranking_record_character_state_jumps ((const uint32_t *)rom(ranking_record_character_state_jumps_ADDRESS))
#define ranking_record_car_state_jumps_ADDRESS 0x00287Cu
#define ranking_record_car_state_jumps ((const uint32_t *)rom(ranking_record_car_state_jumps_ADDRESS))
#define passing_traffic_state_jumps_ADDRESS 0x002960u
#define passing_traffic_state_jumps ((const uint32_t *)rom(passing_traffic_state_jumps_ADDRESS))
#define passing_car_state_jumps_ADDRESS 0x0029A4u
#define passing_car_state_jumps ((const uint32_t *)rom(passing_car_state_jumps_ADDRESS))
#define ranking_background_tile_banks_ADDRESS 0x002C18u
#define ranking_background_tile_banks ((const uint8_t *)rom(ranking_background_tile_banks_ADDRESS))
#define ranking_mixer_addresses_ADDRESS 0x002CA4u
#define ranking_mixer_addresses ((const uint32_t *)rom(ranking_mixer_addresses_ADDRESS))
#define ranking_background_page_numbers_ADDRESS 0x002CACu
#define ranking_background_page_numbers ((const uint32_t *)rom(ranking_background_page_numbers_ADDRESS))
#define ranking_background_scroll_xs_ADDRESS 0x002CB4u
#define ranking_background_scroll_xs ((const uint16_t *)rom(ranking_background_scroll_xs_ADDRESS))
#define name_entry_letters_ADDRESS 0x002D00u
#define name_entry_letters ((const uint8_t *)rom(name_entry_letters_ADDRESS))
#define course_backgrounds_ADDRESS 0x002D20u
#define course_backgrounds ((const uint8_t *)rom(course_backgrounds_ADDRESS))
#define ranking_record_character_xs_ADDRESS 0x002D2Au
#define ranking_record_character_xs ((const uint16_t *)rom(ranking_record_character_xs_ADDRESS))
#define ranking_record_character_ys_ADDRESS 0x002D40u
#define ranking_record_character_ys ((const uint16_t *)rom(ranking_record_character_ys_ADDRESS))
#define name_entry_turn_rates_ADDRESS 0x002D6Cu
#define name_entry_turn_rates ((const uint8_t *)rom(name_entry_turn_rates_ADDRESS))
#define name_entry_place_sounds_ADDRESS 0x002D98u
#define name_entry_place_sounds ((const uint8_t *)rom(name_entry_place_sounds_ADDRESS))
#define certificate_car_animations_ADDRESS 0x002D9Cu
#define certificate_car_animations ((const uint16_t *)rom(certificate_car_animations_ADDRESS))
#define car_model_palettes_ADDRESS 0x002DACu
#define car_model_palettes ((const uint16_t *)rom(car_model_palettes_ADDRESS))
#define certificate_driver_animations_ADDRESS 0x002DBCu
#define certificate_driver_animations ((const uint16_t *)rom(certificate_driver_animations_ADDRESS))
#define certificate_driver_palettes_ADDRESS 0x002DCCu
#define certificate_driver_palettes ((const uint16_t *)rom(certificate_driver_palettes_ADDRESS))
#define car_model_turn_animations_ADDRESS 0x002DDCu
#define car_model_turn_animations ((const uint16_t *)rom(car_model_turn_animations_ADDRESS))
#define ranking_record_character_palettes_ADDRESS 0x002DECu
#define ranking_record_character_palettes ((const uint16_t *)rom(ranking_record_character_palettes_ADDRESS))
#define ranking_course_banner_animations_ADDRESS 0x002DFCu
#define ranking_course_banner_animations ((const uint16_t *)rom(ranking_course_banner_animations_ADDRESS))
#define ranking_course_banner_palettes_ADDRESS 0x002E10u
#define ranking_course_banner_palettes ((const uint16_t *)rom(ranking_course_banner_palettes_ADDRESS))
#define certificate_course_animations_ADDRESS 0x002E24u
#define certificate_course_animations ((const uint16_t *)rom(certificate_course_animations_ADDRESS))
#define certificate_course_palettes_ADDRESS 0x002E38u
#define certificate_course_palettes ((const uint16_t *)rom(certificate_course_palettes_ADDRESS))
#define game_step_jumps_ADDRESS 0x002E80u
#define game_step_jumps ((const uint32_t *)rom(game_step_jumps_ADDRESS))
#define game_start_c00074_bytes_ADDRESS 0x003463u
#define game_start_c00074_bytes ((const uint8_t *)rom(game_start_c00074_bytes_ADDRESS))
#define game_end_c00074_bytes_ADDRESS 0x003465u
#define game_end_c00074_bytes ((const uint8_t *)rom(game_end_c00074_bytes_ADDRESS))
#define game_end_c00074_next_bytes_ADDRESS 0x003467u
#define game_end_c00074_next_bytes ((const uint8_t *)rom(game_end_c00074_next_bytes_ADDRESS))
#define route_map_state_jumps_ADDRESS 0x003540u
#define route_map_state_jumps ((const uint32_t *)rom(route_map_state_jumps_ADDRESS))
#define route_map_car_state_jumps_ADDRESS 0x003E28u
#define route_map_car_state_jumps ((const uint32_t *)rom(route_map_car_state_jumps_ADDRESS))
#define route_map_sign_state_jumps_ADDRESS 0x003F34u
#define route_map_sign_state_jumps ((const uint32_t *)rom(route_map_sign_state_jumps_ADDRESS))
#define route_map_sign_animations_ADDRESS 0x00403Au
#define route_map_sign_animations ((const uint16_t *)rom(route_map_sign_animations_ADDRESS))
#define route_map_sign_palettes_ADDRESS 0x00403Eu
#define route_map_sign_palettes ((const uint16_t *)rom(route_map_sign_palettes_ADDRESS))
#define route_map_goal_xs_ADDRESS 0x004042u
#define route_map_goal_xs ((const int16_t *)rom(route_map_goal_xs_ADDRESS))
#define route_map_landmark_state_jumps_ADDRESS 0x004064u
#define route_map_landmark_state_jumps ((const uint32_t *)rom(route_map_landmark_state_jumps_ADDRESS))
#define route_map_leg_state_jumps_ADDRESS 0x00424Cu
#define route_map_leg_state_jumps ((const uint32_t *)rom(route_map_leg_state_jumps_ADDRESS))
#define route_map_leg_jumps_ADDRESS 0x004280u
#define route_map_leg_jumps ((const uint32_t *)rom(route_map_leg_jumps_ADDRESS))
#define route_map_leg_car_state_jumps_ADDRESS 0x00442Cu
#define route_map_leg_car_state_jumps ((const uint32_t *)rom(route_map_leg_car_state_jumps_ADDRESS))
#define route_map_leg_car_leg_jumps_ADDRESS 0x004474u
#define route_map_leg_car_leg_jumps ((const uint32_t *)rom(route_map_leg_car_leg_jumps_ADDRESS))
#define route_map_checkpoint_state_jumps_ADDRESS 0x004588u
#define route_map_checkpoint_state_jumps ((const uint32_t *)rom(route_map_checkpoint_state_jumps_ADDRESS))
#define route_map_finish_x_offsets_ADDRESS 0x00462Cu
#define route_map_finish_x_offsets ((const uint16_t *)rom(route_map_finish_x_offsets_ADDRESS))
#define route_map_finish_y_offsets_ADDRESS 0x004634u
#define route_map_finish_y_offsets ((const uint16_t *)rom(route_map_finish_y_offsets_ADDRESS))
#define route_map_direction_sign_state_jumps_ADDRESS 0x004648u
#define route_map_direction_sign_state_jumps ((const uint32_t *)rom(route_map_direction_sign_state_jumps_ADDRESS))
#define route_map_wait_banner_state_jumps_ADDRESS 0x0046C8u
#define route_map_wait_banner_state_jumps ((const uint32_t *)rom(route_map_wait_banner_state_jumps_ADDRESS))
#define game_over_banner_state_jumps_ADDRESS 0x00477Cu
#define game_over_banner_state_jumps ((const uint32_t *)rom(game_over_banner_state_jumps_ADDRESS))
#define route_map_close_up_pages_ADDRESS 0x0048B8u
#define route_map_close_up_pages ((const uint32_t *)rom(route_map_close_up_pages_ADDRESS))
#define route_map_routes_pages_ADDRESS 0x0048C0u
#define route_map_routes_pages ((const uint32_t *)rom(route_map_routes_pages_ADDRESS))
#define route_map_world_pages_ADDRESS 0x0048C8u
#define route_map_world_pages ((const uint32_t *)rom(route_map_world_pages_ADDRESS))
#define route_map_start_scroll_xs_ADDRESS 0x0048D0u
#define route_map_start_scroll_xs ((const uint16_t *)rom(route_map_start_scroll_xs_ADDRESS))
#define route_map_whole_scroll_xs_ADDRESS 0x0048D4u
#define route_map_whole_scroll_xs ((const uint16_t *)rom(route_map_whole_scroll_xs_ADDRESS))
#define route_map_sea_colour_addresses_ADDRESS 0x0048D8u
#define route_map_sea_colour_addresses ((const uint32_t *)rom(route_map_sea_colour_addresses_ADDRESS))
#define route_map_car_animations_ADDRESS 0x0048E0u
#define route_map_car_animations ((const uint16_t *)rom(route_map_car_animations_ADDRESS))
#define route_map_car_palettes_ADDRESS 0x0048F0u
#define route_map_car_palettes ((const uint16_t *)rom(route_map_car_palettes_ADDRESS))
#define route_map_stage_xs_ADDRESS 0x004900u
#define route_map_stage_xs ((const uint16_t *)rom(route_map_stage_xs_ADDRESS))
#define route_map_stage_ys_ADDRESS 0x00492Au
#define route_map_stage_ys ((const uint16_t *)rom(route_map_stage_ys_ADDRESS))
#define route_map_direction_x_shifts_ADDRESS 0x004954u
#define route_map_direction_x_shifts ((const uint16_t *)rom(route_map_direction_x_shifts_ADDRESS))
#define route_map_first_fork_leg_animations_ADDRESS 0x004958u
#define route_map_first_fork_leg_animations ((const uint16_t *)rom(route_map_first_fork_leg_animations_ADDRESS))
#define route_map_fork_leg_animations_ADDRESS 0x00495Cu
#define route_map_fork_leg_animations ((const uint16_t *)rom(route_map_fork_leg_animations_ADDRESS))
#define car_select_state_jumps_ADDRESS 0x005058u
#define car_select_state_jumps ((const uint32_t *)rom(car_select_state_jumps_ADDRESS))
#define car_select_choice_turn_jumps_ADDRESS 0x0053ECu
#define car_select_choice_turn_jumps ((const uint32_t *)rom(car_select_choice_turn_jumps_ADDRESS))
#define car_select_rowscroll_copies_jumps_ADDRESS 0x00548Cu
#define car_select_rowscroll_copies_jumps ((const uint32_t *)rom(car_select_rowscroll_copies_jumps_ADDRESS))
#define car_attendant_state_jumps_ADDRESS 0x005534u
#define car_attendant_state_jumps ((const uint32_t *)rom(car_attendant_state_jumps_ADDRESS))
#define turntable_car_state_jumps_ADDRESS 0x0056C0u
#define turntable_car_state_jumps ((const uint32_t *)rom(turntable_car_state_jumps_ADDRESS))
#define turntable_scenery_state_jumps_ADDRESS 0x005770u
#define turntable_scenery_state_jumps ((const uint32_t *)rom(turntable_scenery_state_jumps_ADDRESS))
#define welcome_letter_state_jumps_ADDRESS 0x005890u
#define welcome_letter_state_jumps ((const uint32_t *)rom(welcome_letter_state_jumps_ADDRESS))
#define select_car_heading_state_jumps_ADDRESS 0x005914u
#define select_car_heading_state_jumps ((const uint32_t *)rom(select_car_heading_state_jumps_ADDRESS))
#define select_instruction_state_jumps_ADDRESS 0x0059C4u
#define select_instruction_state_jumps ((const uint32_t *)rom(select_instruction_state_jumps_ADDRESS))
#define transmission_choice_state_jumps_ADDRESS 0x005A28u
#define transmission_choice_state_jumps ((const uint32_t *)rom(transmission_choice_state_jumps_ADDRESS))
#define automatic_panel_state_jumps_ADDRESS 0x005B50u
#define automatic_panel_state_jumps ((const uint32_t *)rom(automatic_panel_state_jumps_ADDRESS))
#define manual_panel_state_jumps_ADDRESS 0x005BE4u
#define manual_panel_state_jumps ((const uint32_t *)rom(manual_panel_state_jumps_ADDRESS))
#define car_caption_state_jumps_ADDRESS 0x005D68u
#define car_caption_state_jumps ((const uint32_t *)rom(car_caption_state_jumps_ADDRESS))
#define car_gears_caption_state_jumps_ADDRESS 0x005E1Cu
#define car_gears_caption_state_jumps ((const uint32_t *)rom(car_gears_caption_state_jumps_ADDRESS))
#define waiting_banner_state_jumps_ADDRESS 0x005EC0u
#define waiting_banner_state_jumps ((const uint32_t *)rom(waiting_banner_state_jumps_ADDRESS))
#define car_select_entries_state_jumps_ADDRESS 0x005F5Cu
#define car_select_entries_state_jumps ((const uint32_t *)rom(car_select_entries_state_jumps_ADDRESS))
#define car_select_gas_down_jumps_ADDRESS 0x0062B0u
#define car_select_gas_down_jumps ((const uint32_t *)rom(car_select_gas_down_jumps_ADDRESS))
#define entry_countdown_lcd_texts_ADDRESS 0x006464u
#define entry_countdown_lcd_texts ((const uint32_t *)rom(entry_countdown_lcd_texts_ADDRESS))
#define entry_traffic_state_jumps_ADDRESS 0x00661Cu
#define entry_traffic_state_jumps ((const uint32_t *)rom(entry_traffic_state_jumps_ADDRESS))
#define entry_traffic_car_state_jumps_ADDRESS 0x00665Cu
#define entry_traffic_car_state_jumps ((const uint32_t *)rom(entry_traffic_car_state_jumps_ADDRESS))
#define car_pass_sounds_ADDRESS 0x0067A9u
#define car_pass_sounds ((const uint8_t *)rom(car_pass_sounds_ADDRESS))
#define car_select_default_cars_ADDRESS 0x0069B0u
#define car_select_default_cars ((const uint8_t *)rom(car_select_default_cars_ADDRESS))
#define turntable_car_frames_ADDRESS 0x0069D8u
#define turntable_car_frames ((const uint8_t *)rom(turntable_car_frames_ADDRESS))
#define car_select_turn_delays_ADDRESS 0x006D9Cu
#define car_select_turn_delays ((const uint8_t *)rom(car_select_turn_delays_ADDRESS))
#define car_select_wheel_transmissions_ADDRESS 0x006DA4u
#define car_select_wheel_transmissions ((const uint8_t *)rom(car_select_wheel_transmissions_ADDRESS))
#define car_caption_palettes_ADDRESS 0x006DDEu
#define car_caption_palettes ((const uint16_t *)rom(car_caption_palettes_ADDRESS))
#define car_select_car_sounds_ADDRESS 0x006DEEu
#define car_select_car_sounds ((const uint8_t *)rom(car_select_car_sounds_ADDRESS))
#define automatic_panel_palette_lists_ADDRESS 0x006E30u
#define automatic_panel_palette_lists ((const uint32_t *)rom(automatic_panel_palette_lists_ADDRESS))
#define manual_panel_palette_lists_ADDRESS 0x006E44u
#define manual_panel_palette_lists ((const uint32_t *)rom(manual_panel_palette_lists_ADDRESS))
#define car_gears_caption_frames_ADDRESS 0x006E78u
#define car_gears_caption_frames ((const uint8_t *)rom(car_gears_caption_frames_ADDRESS))
#define choice_flag_palettes_ADDRESS 0x006E80u
#define choice_flag_palettes ((const uint16_t *)rom(choice_flag_palettes_ADDRESS))
#define choice_flag_number_animations_ADDRESS 0x006E90u
#define choice_flag_number_animations ((const uint16_t *)rom(choice_flag_number_animations_ADDRESS))
#define car_select_entries_flag_blink_ADDRESS 0x006EA0u
#define car_select_entries_flag_blink ((const uint8_t *)rom(car_select_entries_flag_blink_ADDRESS))
#define player_car_state_jumps_ADDRESS 0x006F18u
#define player_car_state_jumps ((const uint32_t *)rom(player_car_state_jumps_ADDRESS))
#define player_car_time_up_step_jumps_ADDRESS 0x007164u
#define player_car_time_up_step_jumps ((const uint32_t *)rom(player_car_time_up_step_jumps_ADDRESS))
#define player_car_goal_step_jumps_ADDRESS 0x00720Cu
#define player_car_goal_step_jumps ((const uint32_t *)rom(player_car_goal_step_jumps_ADDRESS))
#define player_car_rival_step_jumps_ADDRESS 0x007917u
#define player_car_rival_step_jumps ((const uint32_t *)rom(player_car_rival_step_jumps_ADDRESS))
#define rival_warning_state_jumps_ADDRESS 0x007C44u
#define rival_warning_state_jumps ((const uint32_t *)rom(rival_warning_state_jumps_ADDRESS))
#define rival_alarm_state_jumps_ADDRESS 0x007CACu
#define rival_alarm_state_jumps ((const uint32_t *)rom(rival_alarm_state_jumps_ADDRESS))
#define player_car_hit_kind_jumps_ADDRESS 0x007D28u
#define player_car_hit_kind_jumps ((const uint32_t *)rom(player_car_hit_kind_jumps_ADDRESS))
#define crash_place_offsets_ADDRESS 0x008026u
#define crash_place_offsets ((const uint16_t *)rom(crash_place_offsets_ADDRESS))
#define car_weights_ADDRESS 0x008038u
#define car_weights ((const uint16_t *)rom(car_weights_ADDRESS))
#define car_crash_physics_ADDRESS 0x00804Au
#define car_crash_physics ((const uint32_t *)rom(car_crash_physics_ADDRESS))
#define grid_road_place_lists_ADDRESS 0x0090ACu
#define grid_road_place_lists ((const uint32_t *)rom(grid_road_place_lists_ADDRESS))
#define grid_segment_lists_ADDRESS 0x0091CCu
#define grid_segment_lists ((const uint32_t *)rom(grid_segment_lists_ADDRESS))
#define player_car_steer_animations_ADDRESS 0x00926Cu
#define player_car_steer_animations ((const uint16_t *)rom(player_car_steer_animations_ADDRESS))
#define player_car_wheelspin_animations_ADDRESS 0x009300u
#define player_car_wheelspin_animations ((const uint16_t *)rom(player_car_wheelspin_animations_ADDRESS))
#define player_car_skid_animations_wheel_high_ADDRESS 0x009310u
#define player_car_skid_animations_wheel_high ((const uint16_t *)rom(player_car_skid_animations_wheel_high_ADDRESS))
#define player_car_skid_animations_wheel_low_ADDRESS 0x009410u
#define player_car_skid_animations_wheel_low ((const uint16_t *)rom(player_car_skid_animations_wheel_low_ADDRESS))
#define player_car_palettes_ADDRESS 0x009510u
#define player_car_palettes ((const uint16_t *)rom(player_car_palettes_ADDRESS))
#define player_car_arrival_animations_ADDRESS 0x009520u
#define player_car_arrival_animations ((const uint16_t *)rom(player_car_arrival_animations_ADDRESS))
#define player_car_time_up_brakes_ADDRESS 0x009530u
#define player_car_time_up_brakes ((const uint8_t *)rom(player_car_time_up_brakes_ADDRESS))
#define goal_stops_ADDRESS 0x009540u
#define goal_stops ((const uint32_t *)rom(goal_stops_ADDRESS))
#define race_car_first_body_pieces_ADDRESS 0x009DCCu
#define race_car_first_body_pieces ((const uint16_t *)rom(race_car_first_body_pieces_ADDRESS))
#define race_car_first_body_animations_ADDRESS 0x009DDCu
#define race_car_first_body_animations ((const uint16_t *)rom(race_car_first_body_animations_ADDRESS))
#define race_car_driver_animations_ADDRESS 0x009DECu
#define race_car_driver_animations ((const uint16_t *)rom(race_car_driver_animations_ADDRESS))
#define race_car_passenger_animations_ADDRESS 0x009DFCu
#define race_car_passenger_animations ((const uint16_t *)rom(race_car_passenger_animations_ADDRESS))
#define race_car_driver_gesture_masks_ADDRESS 0x009E0Cu
#define race_car_driver_gesture_masks ((const uint8_t *)rom(race_car_driver_gesture_masks_ADDRESS))
#define race_car_passenger_gesture_masks_ADDRESS 0x009E14u
#define race_car_passenger_gesture_masks ((const uint8_t *)rom(race_car_passenger_gesture_masks_ADDRESS))
#define race_car_driver_palettes_ADDRESS 0x009E1Cu
#define race_car_driver_palettes ((const uint16_t *)rom(race_car_driver_palettes_ADDRESS))
#define race_car_passenger_palettes_ADDRESS 0x009E2Cu
#define race_car_passenger_palettes ((const uint16_t *)rom(race_car_passenger_palettes_ADDRESS))
#define race_car_driver_frame_counts_ADDRESS 0x009E3Cu
#define race_car_driver_frame_counts ((const uint8_t *)rom(race_car_driver_frame_counts_ADDRESS))
#define race_car_passenger_frame_counts_ADDRESS 0x009E44u
#define race_car_passenger_frame_counts ((const uint8_t *)rom(race_car_passenger_frame_counts_ADDRESS))
#define race_car_body_wheel_kinds_ADDRESS 0x009E4Cu
#define race_car_body_wheel_kinds ((const uint8_t *)rom(race_car_body_wheel_kinds_ADDRESS))
#define race_car_driver_handlers_ADDRESS 0x009EBCu
#define race_car_driver_handlers ((const uint32_t *)rom(race_car_driver_handlers_ADDRESS))
#define race_car_passenger_handlers_ADDRESS 0x009EDCu
#define race_car_passenger_handlers ((const uint32_t *)rom(race_car_passenger_handlers_ADDRESS))
#define race_car_spray_palettes_ADDRESS 0x009F0Cu
#define race_car_spray_palettes ((const uint16_t *)rom(race_car_spray_palettes_ADDRESS))
#define race_car_spray_2_animations_ADDRESS 0x009F2Cu
#define race_car_spray_2_animations ROM16(race_car_spray_2_animations_ADDRESS)
#define race_car_spray_1_animations_ADDRESS 0x009F4Cu
#define race_car_spray_1_animations ROM16(race_car_spray_1_animations_ADDRESS)
#define race_car_spray_frame_counts_ADDRESS 0x009F6Cu
#define race_car_spray_frame_counts ((const uint8_t *)rom(race_car_spray_frame_counts_ADDRESS))
#define race_car_tag_palettes_ADDRESS 0x009F7Cu
#define race_car_tag_palettes ((const uint16_t *)rom(race_car_tag_palettes_ADDRESS))
#define race_car_tag_animations_ADDRESS 0x009F8Cu
#define race_car_tag_animations ((const uint16_t *)rom(race_car_tag_animations_ADDRESS))
#define race_car_blink_starts_ADDRESS 0x009F9Cu
#define race_car_blink_starts ((const uint8_t *)rom(race_car_blink_starts_ADDRESS))
#define race_car_tag_blink_ADDRESS 0x009FA4u
#define race_car_tag_blink ((const uint8_t *)rom(race_car_tag_blink_ADDRESS))
#define race_car_ending_handlers_ADDRESS 0x009FC4u
#define race_car_ending_handlers ((const uint32_t *)rom(race_car_ending_handlers_ADDRESS))
#define player_car_heading_quadrant_jumps_ADDRESS 0x00A5F4u
#define player_car_heading_quadrant_jumps ((const uint32_t *)rom(player_car_heading_quadrant_jumps_ADDRESS))
#define player_car_view_yaw_quadrant_jumps_ADDRESS 0x00A798u
#define player_car_view_yaw_quadrant_jumps ((const uint32_t *)rom(player_car_view_yaw_quadrant_jumps_ADDRESS))
#define car_skid_noises_ADDRESS 0x00ABC3u
#define car_skid_noises ((const uint8_t *)rom(car_skid_noises_ADDRESS))
#define gas_pedal_curve_ADDRESS 0x00ABCBu
#define gas_pedal_curve ((const int8_t *)rom(gas_pedal_curve_ADDRESS))
#define brake_pedal_curve_ADDRESS 0x00ACCBu
#define brake_pedal_curve ((const uint8_t *)rom(brake_pedal_curve_ADDRESS))
#define car_handlings_ADDRESS 0x00ADCCu
#define car_handlings ((const uint32_t *)rom(car_handlings_ADDRESS))
#define car_gear_lists_ADDRESS 0x00B04Cu
#define car_gear_lists ((const uint32_t *)rom(car_gear_lists_ADDRESS))
#define car_torque_curves_ADDRESS 0x00B0C0u
#define car_torque_curves ((const uint32_t *)rom(car_torque_curves_ADDRESS))
#define car_model_off_road_noises_ADDRESS 0x00B6FCu
#define car_model_off_road_noises ((const uint8_t *)rom(car_model_off_road_noises_ADDRESS))
#define car_model_off_road_shakes_ADDRESS 0x00B704u
#define car_model_off_road_shakes ((const uint8_t *)rom(car_model_off_road_shakes_ADDRESS))
#define wheel_effect_1_jumps_ADDRESS 0x00B70Cu
#define wheel_effect_1_jumps ((const uint32_t *)rom(wheel_effect_1_jumps_ADDRESS))
#define wheel_effect_2_jumps_ADDRESS 0x00B74Cu
#define wheel_effect_2_jumps ((const uint32_t *)rom(wheel_effect_2_jumps_ADDRESS))
#define rev_meter_state_jumps_ADDRESS 0x00BFCCu
#define rev_meter_state_jumps ((const uint32_t *)rom(rev_meter_state_jumps_ADDRESS))
#define speedometer_state_jumps_ADDRESS 0x00C024u
#define speedometer_state_jumps ((const uint32_t *)rom(speedometer_state_jumps_ADDRESS))
#define speed_unit_state_jumps_ADDRESS 0x00C0D4u
#define speed_unit_state_jumps ((const uint32_t *)rom(speed_unit_state_jumps_ADDRESS))
#define gear_indicator_state_jumps_ADDRESS 0x00C118u
#define gear_indicator_state_jumps ((const uint32_t *)rom(gear_indicator_state_jumps_ADDRESS))
#define shift_up_hint_state_jumps_ADDRESS 0x00C23Cu
#define shift_up_hint_state_jumps ((const uint32_t *)rom(shift_up_hint_state_jumps_ADDRESS))
#define place_count_state_jumps_ADDRESS 0x00C2B8u
#define place_count_state_jumps ((const uint32_t *)rom(place_count_state_jumps_ADDRESS))
#define place_number_state_jumps_ADDRESS 0x00C350u
#define place_number_state_jumps ((const uint32_t *)rom(place_number_state_jumps_ADDRESS))
#define lap_timer_state_jumps_ADDRESS 0x00C44Cu
#define lap_timer_state_jumps ((const uint32_t *)rom(lap_timer_state_jumps_ADDRESS))
#define start_message_state_jumps_ADDRESS 0x00C5A8u
#define start_message_state_jumps ((const uint32_t *)rom(start_message_state_jumps_ADDRESS))
#define time_left_display_state_jumps_ADDRESS 0x00C66Cu
#define time_left_display_state_jumps ((const uint32_t *)rom(time_left_display_state_jumps_ADDRESS))
#define stage_entry_state_jumps_ADDRESS 0x00C880u
#define stage_entry_state_jumps ((const uint32_t *)rom(stage_entry_state_jumps_ADDRESS))
#define goal_state_jumps_ADDRESS 0x00C8C4u
#define goal_state_jumps ((const uint32_t *)rom(goal_state_jumps_ADDRESS))
#define branch_arrow_state_jumps_ADDRESS 0x00CAC8u
#define branch_arrow_state_jumps ((const uint32_t *)rom(branch_arrow_state_jumps_ADDRESS))
#define checkpoint_board_state_jumps_ADDRESS 0x00CBCCu
#define checkpoint_board_state_jumps ((const uint32_t *)rom(checkpoint_board_state_jumps_ADDRESS))
#define checkpoint_board_row_state_jumps_ADDRESS 0x00CC70u
#define checkpoint_board_row_state_jumps ((const uint32_t *)rom(checkpoint_board_row_state_jumps_ADDRESS))
#define checkpoint_board_car_sprites_ADDRESS 0x00CFC8u
#define checkpoint_board_car_sprites ((const uint32_t *)rom(checkpoint_board_car_sprites_ADDRESS))
#define checkpoint_board_number_animations_ADDRESS 0x00CFE8u
#define checkpoint_board_number_animations ((const uint16_t *)rom(checkpoint_board_number_animations_ADDRESS))
#define checkpoint_board_time_places_ADDRESS 0x00CFF8u
#define checkpoint_board_time_places ((const uint32_t *)rom(checkpoint_board_time_places_ADDRESS))
#define gear_indicator_animations_ADDRESS 0x00D018u
#define gear_indicator_animations ((const uint16_t *)rom(gear_indicator_animations_ADDRESS))
#define stage_level_texts_ADDRESS 0x00D034u
#define stage_level_texts ((const uint32_t *)rom(stage_level_texts_ADDRESS))
#define lap_timer_checkpoint_states_ADDRESS 0x00D0C0u
#define lap_timer_checkpoint_states ((const uint8_t *)rom(lap_timer_checkpoint_states_ADDRESS))
#define entry_invitation_step_jumps_ADDRESS 0x00DDB4u
#define entry_invitation_step_jumps ((const uint32_t *)rom(entry_invitation_step_jumps_ADDRESS))
#define race_copy_offsets_ADDRESS 0x00E0A0u
#define race_copy_offsets ((const uint32_t *)rom(race_copy_offsets_ADDRESS))
#define race_copy_player_offsets_ADDRESS 0x00E0C4u
#define race_copy_player_offsets ((const uint32_t *)rom(race_copy_player_offsets_ADDRESS))
#define link_board_ic_texts_ADDRESS 0x00E4D0u
#define link_board_ic_texts ((const uint32_t *)rom(link_board_ic_texts_ADDRESS))
#define stage_start_times_ADDRESS 0x00E5B3u
#define stage_start_times ((const uint8_t *)rom(stage_start_times_ADDRESS))
#define fork_next_stages_ADDRESS 0x00E5D3u
#define fork_next_stages ((const uint8_t *)rom(fork_next_stages_ADDRESS))
#define entry_frames_by_machines_ADDRESS 0x00E614u
#define entry_frames_by_machines ((const uint16_t *)rom(entry_frames_by_machines_ADDRESS))
#define draw_object_mode_jumps_ADDRESS 0x00EB44u
#define draw_object_mode_jumps ((const uint32_t *)rom(draw_object_mode_jumps_ADDRESS))
#define sprite_flip_x_signs_ADDRESS 0x00F13Cu
#define sprite_flip_x_signs ((const uint32_t *)rom(sprite_flip_x_signs_ADDRESS))
#define sprite_flip_y_signs_ADDRESS 0x00F140u
#define sprite_flip_y_signs ((const uint32_t *)rom(sprite_flip_y_signs_ADDRESS))
#define animation_order_jumps_ADDRESS 0x00F34Cu
#define animation_order_jumps ((const uint32_t *)rom(animation_order_jumps_ADDRESS))
#define smoothed_flip_signs_ADDRESS 0x00F4ECu
#define smoothed_flip_signs ((const uint32_t *)rom(smoothed_flip_signs_ADDRESS))
#define course_command_jumps_ADDRESS 0x00FF40u
#define course_command_jumps ((const uint32_t *)rom(course_command_jumps_ADDRESS))
#define sprite_control_mode_ADDRESS 0x010B0Au
#define sprite_control_mode ROM8(sprite_control_mode_ADDRESS)
#define sprite_control_flip_ADDRESS 0x010B0Bu
#define sprite_control_flip ROM8(sprite_control_flip_ADDRESS)
#define video_control_default_ADDRESS 0x010B10u
#define video_control_default ROM16(video_control_default_ADDRESS)
#define model_view_pitch_bases_ADDRESS 0x010BD2u
#define model_view_pitch_bases ((const uint16_t *)rom(model_view_pitch_bases_ADDRESS))
#define screen_fade_kind_jumps_ADDRESS 0x01168Cu
#define screen_fade_kind_jumps ((const uint32_t *)rom(screen_fade_kind_jumps_ADDRESS))
#define music_fade_addresses_ADDRESS 0x011FFCu
#define music_fade_addresses ((const uint32_t *)rom(music_fade_addresses_ADDRESS))
#define car_model_engine_samples_ADDRESS 0x012026u
#define car_model_engine_samples ((const uint8_t *)rom(car_model_engine_samples_ADDRESS))
#define demo_sounds_ADDRESS 0x01202Eu
#define demo_sounds ((const uint32_t *)rom(demo_sounds_ADDRESS))
#define demo_second_sounds_ADDRESS 0x01204Eu
#define demo_second_sounds ((const uint32_t *)rom(demo_second_sounds_ADDRESS))
#define octant_base_angles_ADDRESS 0x0121D8u
#define octant_base_angles ((const uint16_t *)rom(octant_base_angles_ADDRESS))
#define octant_angle_factors_ADDRESS 0x0121E8u
#define octant_angle_factors ((const uint16_t *)rom(octant_angle_factors_ADDRESS))
#define road_object_stream_state_jumps_ADDRESS 0x012708u
#define road_object_stream_state_jumps ((const uint32_t *)rom(road_object_stream_state_jumps_ADDRESS))
#define road_object_stream_command_jumps_ADDRESS 0x012748u
#define road_object_stream_command_jumps ((const uint32_t *)rom(road_object_stream_command_jumps_ADDRESS))
#define crest_clip_state_jumps_ADDRESS 0x0127FCu
#define crest_clip_state_jumps ((const uint32_t *)rom(crest_clip_state_jumps_ADDRESS))
#define scenery_state_jumps_ADDRESS 0x012888u
#define scenery_state_jumps ((const uint32_t *)rom(scenery_state_jumps_ADDRESS))
#define flashing_scenery_state_jumps_ADDRESS 0x012914u
#define flashing_scenery_state_jumps ((const uint32_t *)rom(flashing_scenery_state_jumps_ADDRESS))
#define scenery_pair_state_jumps_ADDRESS 0x0129B8u
#define scenery_pair_state_jumps ((const uint32_t *)rom(scenery_pair_state_jumps_ADDRESS))
#define overhead_scenery_state_jumps_ADDRESS 0x012A00u
#define overhead_scenery_state_jumps ((const uint32_t *)rom(overhead_scenery_state_jumps_ADDRESS))
#define aurora_state_jumps_ADDRESS 0x012A2Cu
#define aurora_state_jumps ((const uint32_t *)rom(aurora_state_jumps_ADDRESS))
#define fork_sign_state_jumps_ADDRESS 0x012ADCu
#define fork_sign_state_jumps ((const uint32_t *)rom(fork_sign_state_jumps_ADDRESS))
#define animated_scenery_state_jumps_ADDRESS 0x012B00u
#define animated_scenery_state_jumps ((const uint32_t *)rom(animated_scenery_state_jumps_ADDRESS))
#define animated_scenery_pair_state_jumps_ADDRESS 0x012B90u
#define animated_scenery_pair_state_jumps ((const uint32_t *)rom(animated_scenery_pair_state_jumps_ADDRESS))
#define road_crosser_state_jumps_ADDRESS 0x012BBCu
#define road_crosser_state_jumps ((const uint32_t *)rom(road_crosser_state_jumps_ADDRESS))
#define kangaroo_state_jumps_ADDRESS 0x012CB8u
#define kangaroo_state_jumps ((const uint32_t *)rom(kangaroo_state_jumps_ADDRESS))
#define moving_scenery_state_jumps_ADDRESS 0x012DC4u
#define moving_scenery_state_jumps ((const uint32_t *)rom(moving_scenery_state_jumps_ADDRESS))
#define swooping_helicopter_state_jumps_ADDRESS 0x012EB0u
#define swooping_helicopter_state_jumps ((const uint32_t *)rom(swooping_helicopter_state_jumps_ADDRESS))
#define start_gate_state_jumps_ADDRESS 0x012FC8u
#define start_gate_state_jumps ((const uint32_t *)rom(start_gate_state_jumps_ADDRESS))
#define start_gate_light_palettes_ADDRESS 0x01306Eu
#define start_gate_light_palettes ((const uint16_t *)rom(start_gate_light_palettes_ADDRESS))
#define start_flagman_state_jumps_ADDRESS 0x01308Cu
#define start_flagman_state_jumps ((const uint32_t *)rom(start_flagman_state_jumps_ADDRESS))
#define flagman_state_jumps_ADDRESS 0x013110u
#define flagman_state_jumps ((const uint32_t *)rom(flagman_state_jumps_ADDRESS))
#define start_balloon_state_jumps_ADDRESS 0x0131B0u
#define start_balloon_state_jumps ((const uint32_t *)rom(start_balloon_state_jumps_ADDRESS))
#define ambient_sound_state_jumps_ADDRESS 0x013238u
#define ambient_sound_state_jumps ((const uint32_t *)rom(ambient_sound_state_jumps_ADDRESS))
#define west_east_panel_state_jumps_ADDRESS 0x0132A4u
#define west_east_panel_state_jumps ((const uint32_t *)rom(west_east_panel_state_jumps_ADDRESS))
#define santa_sleigh_state_jumps_ADDRESS 0x013348u
#define santa_sleigh_state_jumps ((const uint32_t *)rom(santa_sleigh_state_jumps_ADDRESS))
#define goal_parade_rightward_state_jumps_ADDRESS 0x01345Cu
#define goal_parade_rightward_state_jumps ((const uint32_t *)rom(goal_parade_rightward_state_jumps_ADDRESS))
#define sounding_scenery_state_jumps_ADDRESS 0x01352Cu
#define sounding_scenery_state_jumps ((const uint32_t *)rom(sounding_scenery_state_jumps_ADDRESS))
#define goal_parade_leftward_state_jumps_ADDRESS 0x0135D8u
#define goal_parade_leftward_state_jumps ((const uint32_t *)rom(goal_parade_leftward_state_jumps_ADDRESS))
#define goal_parade_dragon_state_jumps_ADDRESS 0x0136A8u
#define goal_parade_dragon_state_jumps ((const uint32_t *)rom(goal_parade_dragon_state_jumps_ADDRESS))
#define traffic_spawner_state_jumps_ADDRESS 0x013778u
#define traffic_spawner_state_jumps ((const uint32_t *)rom(traffic_spawner_state_jumps_ADDRESS))
#define traffic_car_state_jumps_ADDRESS 0x0138F8u
#define traffic_car_state_jumps ((const uint32_t *)rom(traffic_car_state_jumps_ADDRESS))
#define traffic_stopping_car_state_jumps_ADDRESS 0x013B04u
#define traffic_stopping_car_state_jumps ((const uint32_t *)rom(traffic_stopping_car_state_jumps_ADDRESS))
#define traffic_figure_state_jumps_ADDRESS 0x013BB0u
#define traffic_figure_state_jumps ((const uint32_t *)rom(traffic_figure_state_jumps_ADDRESS))
#define traffic_shake_frame_offsets_ADDRESS 0x013D45u
#define traffic_shake_frame_offsets ((const uint8_t *)rom(traffic_shake_frame_offsets_ADDRESS))
#define course_length_command_jumps_ADDRESS 0x013D94u
#define course_length_command_jumps ((const uint32_t *)rom(course_length_command_jumps_ADDRESS))
#define fork_branch_state_jumps_ADDRESS 0x013DF0u
#define fork_branch_state_jumps ((const uint32_t *)rom(fork_branch_state_jumps_ADDRESS))
#define fork_branch_stage_palettes_ADDRESS 0x0140C8u
#define fork_branch_stage_palettes ((const uint16_t *)rom(fork_branch_stage_palettes_ADDRESS))
#define race_car_occupant_state_jumps_ADDRESS 0x014114u
#define race_car_occupant_state_jumps ((const uint32_t *)rom(race_car_occupant_state_jumps_ADDRESS))
#define rider_gesture_state_jumps_ADDRESS 0x014200u
#define rider_gesture_state_jumps ((const uint32_t *)rom(rider_gesture_state_jumps_ADDRESS))
#define rider_gesture_x_offsets_ADDRESS 0x014360u
#define rider_gesture_x_offsets ((const uint16_t *)rom(rider_gesture_x_offsets_ADDRESS))
#define rider_gesture_y_offsets_ADDRESS 0x014380u
#define rider_gesture_y_offsets ((const uint16_t *)rom(rider_gesture_y_offsets_ADDRESS))
#define rider_gesture_car_states_ADDRESS 0x0143A0u
#define rider_gesture_car_states ((const uint8_t *)rom(rider_gesture_car_states_ADDRESS))
#define rider_gesture_views_ADDRESS 0x0143A6u
#define rider_gesture_views ((const uint8_t *)rom(rider_gesture_views_ADDRESS))
#define race_car_occupant_animations_ADDRESS 0x01440Eu
#define race_car_occupant_animations ((const uint16_t *)rom(race_car_occupant_animations_ADDRESS))
#define race_car_rider_palettes_ADDRESS 0x01442Eu
#define race_car_rider_palettes ((const uint16_t *)rom(race_car_rider_palettes_ADDRESS))
#define rider_gesture_animations_ADDRESS 0x01444Eu
#define rider_gesture_animations ((const uint16_t *)rom(rider_gesture_animations_ADDRESS))
#define race_opening_state_jumps_ADDRESS 0x01447Cu
#define race_opening_state_jumps ((const uint32_t *)rom(race_opening_state_jumps_ADDRESS))
#define race_opening_sound_step_jumps_ADDRESS 0x0144A9u
#define race_opening_sound_step_jumps ((const uint32_t *)rom(race_opening_sound_step_jumps_ADDRESS))
#define race_opening_camera_step_jumps_ADDRESS 0x0144ECu
#define race_opening_camera_step_jumps ((const uint32_t *)rom(race_opening_camera_step_jumps_ADDRESS))
#define stage_end_sounds_state_jumps_ADDRESS 0x014520u
#define stage_end_sounds_state_jumps ((const uint32_t *)rom(stage_end_sounds_state_jumps_ADDRESS))
#define fork_camera_state_jumps_ADDRESS 0x0145A0u
#define fork_camera_state_jumps ((const uint32_t *)rom(fork_camera_state_jumps_ADDRESS))
#define fork_camera_lift_steps_ADDRESS 0x014674u
#define fork_camera_lift_steps ((const uint16_t *)rom(fork_camera_lift_steps_ADDRESS))
#define fork_camera_tilt_steps_ADDRESS 0x014684u
#define fork_camera_tilt_steps ((const uint16_t *)rom(fork_camera_tilt_steps_ADDRESS))
#define race_car_ending_state_jumps_ADDRESS 0x0146A0u
#define race_car_ending_state_jumps ((const uint32_t *)rom(race_car_ending_state_jumps_ADDRESS))
#define goal_actor_command_jumps_ADDRESS 0x014994u
#define goal_actor_command_jumps ((const uint32_t *)rom(goal_actor_command_jumps_ADDRESS))
#define goal_actor_place_waits_ADDRESS 0x014A16u
#define goal_actor_place_waits ((const uint8_t *)rom(goal_actor_place_waits_ADDRESS))
#define object_lists_14A20_ADDRESS 0x014A20u
#define object_lists_14A20 ((const uint32_t *)rom(object_lists_14A20_ADDRESS))
#define object_list_14A60_ADDRESS 0x014A60u
#define object_list_14A60 ROM32(object_list_14A60_ADDRESS)
#define unfinished_ending_actors_ADDRESS 0x014A64u
#define unfinished_ending_actors ROM32(unfinished_ending_actors_ADDRESS)
#define road_surface_state_jumps_ADDRESS 0x016B0Cu
#define road_surface_state_jumps ((const uint32_t *)rom(road_surface_state_jumps_ADDRESS))
#define road_surface_event_jumps_ADDRESS 0x016BBCu
#define road_surface_event_jumps ((const uint32_t *)rom(road_surface_event_jumps_ADDRESS))
#define road_surface_page_words_ADDRESS 0x01714Cu
#define road_surface_page_words ROM32(road_surface_page_words_ADDRESS)
#define road_tiles_change_state_jumps_ADDRESS 0x0171E0u
#define road_tiles_change_state_jumps ((const uint32_t *)rom(road_tiles_change_state_jumps_ADDRESS))
#define road_tiles_change_first_data_ADDRESS 0x017444u
#define road_tiles_change_first_data ROM8(road_tiles_change_first_data_ADDRESS)
#define road_tiles_change_second_data_ADDRESS 0x0174B4u
#define road_tiles_change_second_data ROM8(road_tiles_change_second_data_ADDRESS)
#define race_background_state_jumps_ADDRESS 0x017530u
#define race_background_state_jumps ((const uint32_t *)rom(race_background_state_jumps_ADDRESS))
#define race_background_change_step_jumps_ADDRESS 0x017738u
#define race_background_change_step_jumps ((const uint32_t *)rom(race_background_change_step_jumps_ADDRESS))
#define background_lower_animation_tile_pages_ADDRESS 0x017904u
#define background_lower_animation_tile_pages ((const uint32_t *)rom(background_lower_animation_tile_pages_ADDRESS))
#define background_lower_animation_page_pairs_ADDRESS 0x01790Cu
#define background_lower_animation_page_pairs ((const uint16_t *)rom(background_lower_animation_page_pairs_ADDRESS))
#define background_upper_animation_tile_pages_ADDRESS 0x017988u
#define background_upper_animation_tile_pages ((const uint32_t *)rom(background_upper_animation_tile_pages_ADDRESS))
#define background_upper_animation_page_pairs_ADDRESS 0x0179A0u
#define background_upper_animation_page_pairs ((const uint16_t *)rom(background_upper_animation_page_pairs_ADDRESS))
#define background_register_lists_ADDRESS 0x017A78u
#define background_register_lists ((const uint32_t *)rom(background_register_lists_ADDRESS))
#define background_page_words_ADDRESS 0x017AE0u
#define background_page_words ((const uint32_t *)rom(background_page_words_ADDRESS))
#define stage_backgrounds_ADDRESS 0x017B60u
#define stage_backgrounds ((const uint8_t *)rom(stage_backgrounds_ADDRESS))
#define background_tile_banks_ADDRESS 0x017B7Fu
#define background_tile_banks ((const uint8_t *)rom(background_tile_banks_ADDRESS))
#define stage_tile_banks_ADDRESS 0x017BA9u
#define stage_tile_banks ((const uint8_t *)rom(stage_tile_banks_ADDRESS))
#define palette_cycle_state_jumps_ADDRESS 0x017C08u
#define palette_cycle_state_jumps ((const uint32_t *)rom(palette_cycle_state_jumps_ADDRESS))
#define palette_cycle_running_jumps_ADDRESS 0x017C68u
#define palette_cycle_running_jumps ((const uint32_t *)rom(palette_cycle_running_jumps_ADDRESS))
#define sunset_fade_state_jumps_ADDRESS 0x017CB8u
#define sunset_fade_state_jumps ((const uint32_t *)rom(sunset_fade_state_jumps_ADDRESS))
#define backdrop_far_state_jumps_ADDRESS 0x017DE4u
#define backdrop_far_state_jumps ((const uint32_t *)rom(backdrop_far_state_jumps_ADDRESS))
#define backdrop_near_state_jumps_ADDRESS 0x017E98u
#define backdrop_near_state_jumps ((const uint32_t *)rom(backdrop_near_state_jumps_ADDRESS))
#define backdrop_landmark_state_jumps_ADDRESS 0x017F7Cu
#define backdrop_landmark_state_jumps ((const uint32_t *)rom(backdrop_landmark_state_jumps_ADDRESS))
#define backdrop_flashing_state_jumps_ADDRESS 0x018094u
#define backdrop_flashing_state_jumps ((const uint32_t *)rom(backdrop_flashing_state_jumps_ADDRESS))
#define backdrop_translucent_state_jumps_ADDRESS 0x01814Cu
#define backdrop_translucent_state_jumps ((const uint32_t *)rom(backdrop_translucent_state_jumps_ADDRESS))
#define object_lists_181B8_ADDRESS 0x0181B8u
#define object_lists_181B8 ((const uint32_t *)rom(object_lists_181B8_ADDRESS))
#define lcd_scroll_step_jumps_ADDRESS 0x018A70u
#define lcd_scroll_step_jumps ((const uint32_t *)rom(lcd_scroll_step_jumps_ADDRESS))
#define dj_face_characters_ADDRESS 0x018AE8u
#define dj_face_characters ((const uint16_t *)rom(dj_face_characters_ADDRESS))
#define lcd_restart_nibbles_ADDRESS 0x018B21u
#define lcd_restart_nibbles ((const uint8_t *)rom(lcd_restart_nibbles_ADDRESS))
#define lcd_race_info_state_jumps_ADDRESS 0x018BD4u
#define lcd_race_info_state_jumps ((const uint32_t *)rom(lcd_race_info_state_jumps_ADDRESS))
#define lcd_race_info_page_jumps_ADDRESS 0x018C64u
#define lcd_race_info_page_jumps ((const uint32_t *)rom(lcd_race_info_page_jumps_ADDRESS))
#define lcd_race_info_time_step_jumps_ADDRESS 0x018C84u
#define lcd_race_info_time_step_jumps ((const uint32_t *)rom(lcd_race_info_time_step_jumps_ADDRESS))
#define lcd_race_info_order_step_jumps_ADDRESS 0x018CF0u
#define lcd_race_info_order_step_jumps ((const uint32_t *)rom(lcd_race_info_order_step_jumps_ADDRESS))
#define lcd_race_info_speed_step_jumps_ADDRESS 0x018D5Cu
#define lcd_race_info_speed_step_jumps ((const uint32_t *)rom(lcd_race_info_speed_step_jumps_ADDRESS))
#define lcd_race_info_maximum_step_jumps_ADDRESS 0x018D88u
#define lcd_race_info_maximum_step_jumps ((const uint32_t *)rom(lcd_race_info_maximum_step_jumps_ADDRESS))
#define lcd_race_info_average_step_jumps_ADDRESS 0x018DB4u
#define lcd_race_info_average_step_jumps ((const uint32_t *)rom(lcd_race_info_average_step_jumps_ADDRESS))
#define lcd_race_info_finish_state_jumps_ADDRESS 0x018E40u
#define lcd_race_info_finish_state_jumps ((const uint32_t *)rom(lcd_race_info_finish_state_jumps_ADDRESS))
#define lcd_countdown_bars_ADDRESS 0x018EA0u
#define lcd_countdown_bars ((const uint32_t *)rom(lcd_countdown_bars_ADDRESS))
#define lcd_time_label_ADDRESS 0x018F1Eu
#define lcd_time_label ROM8(lcd_time_label_ADDRESS)
#define lcd_order_label_ADDRESS 0x018F2Cu
#define lcd_order_label ROM8(lcd_order_label_ADDRESS)
#define lcd_speed_label_ADDRESS 0x018F3Au
#define lcd_speed_label ROM8(lcd_speed_label_ADDRESS)
#define lcd_maximum_label_ADDRESS 0x018F48u
#define lcd_maximum_label ROM8(lcd_maximum_label_ADDRESS)
#define lcd_average_label_ADDRESS 0x018F56u
#define lcd_average_label ROM8(lcd_average_label_ADDRESS)
#define lcd_finish_label_ADDRESS 0x018F64u
#define lcd_finish_label ROM8(lcd_finish_label_ADDRESS)
#define car_radio_state_jumps_ADDRESS 0x019044u
#define car_radio_state_jumps ((const uint32_t *)rom(car_radio_state_jumps_ADDRESS))
#define car_radio_talk_step_jumps_ADDRESS 0x0191E0u
#define car_radio_talk_step_jumps ((const uint32_t *)rom(car_radio_talk_step_jumps_ADDRESS))
#define song_title_state_jumps_ADDRESS 0x019330u
#define song_title_state_jumps ((const uint32_t *)rom(song_title_state_jumps_ADDRESS))
#define car_model_songs_ADDRESS 0x019421u
#define car_model_songs ((const uint8_t *)rom(car_model_songs_ADDRESS))
#define radio_songs_ADDRESS 0x01942Cu
#define radio_songs ((const uint32_t *)rom(radio_songs_ADDRESS))
#define route_stage_radio_sounds_ADDRESS 0x019458u
#define route_stage_radio_sounds ((const uint8_t *)rom(route_stage_radio_sounds_ADDRESS))
#define song_title_colours_ADDRESS 0x019561u
#define song_title_colours ((const uint16_t *)rom(song_title_colours_ADDRESS))
#define stage_level_radio_messages_ADDRESS 0x019571u
#define stage_level_radio_messages ((const uint8_t *)rom(stage_level_radio_messages_ADDRESS))
#define dj_messages_ADDRESS 0x019576u
#define dj_messages ((const uint32_t *)rom(dj_messages_ADDRESS))
#define routine_lists_1966C_ADDRESS 0x01966Cu
#define routine_lists_1966C ((const uint32_t *)rom(routine_lists_1966C_ADDRESS))
#define title_screen_state_jumps_ADDRESS 0x019CB8u
#define title_screen_state_jumps ((const uint32_t *)rom(title_screen_state_jumps_ADDRESS))
#define title_car_state_jumps_ADDRESS 0x019E78u
#define title_car_state_jumps ((const uint32_t *)rom(title_car_state_jumps_ADDRESS))
#define title_car_driver_state_jumps_ADDRESS 0x019F58u
#define title_car_driver_state_jumps ((const uint32_t *)rom(title_car_driver_state_jumps_ADDRESS))
#define title_car_passenger_state_jumps_ADDRESS 0x019F6Cu
#define title_car_passenger_state_jumps ((const uint32_t *)rom(title_car_passenger_state_jumps_ADDRESS))
#define title_driver_animations_ADDRESS 0x01A012u
#define title_driver_animations ((const uint16_t *)rom(title_driver_animations_ADDRESS))
#define title_passenger_animations_ADDRESS 0x01A022u
#define title_passenger_animations ((const uint16_t *)rom(title_passenger_animations_ADDRESS))
#define title_driver_palettes_ADDRESS 0x01A032u
#define title_driver_palettes ((const uint16_t *)rom(title_driver_palettes_ADDRESS))
#define title_passenger_palettes_ADDRESS 0x01A042u
#define title_passenger_palettes ((const uint16_t *)rom(title_passenger_palettes_ADDRESS))
#define title_rider_scales_ADDRESS 0x01A052u
#define title_rider_scales ((const uint16_t *)rom(title_rider_scales_ADDRESS))
#define title_driver_x_offsets_ADDRESS 0x01A062u
#define title_driver_x_offsets ((const uint16_t *)rom(title_driver_x_offsets_ADDRESS))
#define title_driver_y_offsets_ADDRESS 0x01A072u
#define title_driver_y_offsets ((const uint16_t *)rom(title_driver_y_offsets_ADDRESS))
#define title_passenger_x_offsets_ADDRESS 0x01A082u
#define title_passenger_x_offsets ((const uint16_t *)rom(title_passenger_x_offsets_ADDRESS))
#define title_passenger_y_offsets_ADDRESS 0x01A092u
#define title_passenger_y_offsets ((const uint16_t *)rom(title_passenger_y_offsets_ADDRESS))
#define title_palm_tree_state_jumps_ADDRESS 0x01A0F0u
#define title_palm_tree_state_jumps ((const uint32_t *)rom(title_palm_tree_state_jumps_ADDRESS))
#define title_logo_state_jumps_ADDRESS 0x01A198u
#define title_logo_state_jumps ((const uint32_t *)rom(title_logo_state_jumps_ADDRESS))
#define title_logo_copy_state_jumps_ADDRESS 0x01A2A8u
#define title_logo_copy_state_jumps ((const uint32_t *)rom(title_logo_copy_state_jumps_ADDRESS))
#define title_logo_car_state_jumps_ADDRESS 0x01A334u
#define title_logo_car_state_jumps ((const uint32_t *)rom(title_logo_car_state_jumps_ADDRESS))
#define title_logo_stripe_state_jumps_ADDRESS 0x01A3F4u
#define title_logo_stripe_state_jumps ((const uint32_t *)rom(title_logo_stripe_state_jumps_ADDRESS))
#define title_board_palms_state_jumps_ADDRESS 0x01A4A4u
#define title_board_palms_state_jumps ((const uint32_t *)rom(title_board_palms_state_jumps_ADDRESS))
#define title_seagull_state_jumps_ADDRESS 0x01A544u
#define title_seagull_state_jumps ((const uint32_t *)rom(title_seagull_state_jumps_ADDRESS))
#define title_picture_state_jumps_ADDRESS 0x01A680u
#define title_picture_state_jumps ((const uint32_t *)rom(title_picture_state_jumps_ADDRESS))
#define coin_prompt_state_jumps_ADDRESS 0x01A6DCu
#define coin_prompt_state_jumps ((const uint32_t *)rom(coin_prompt_state_jumps_ADDRESS))
#define coin_prompt_screen_jumps_ADDRESS 0x01A744u
#define coin_prompt_screen_jumps ((const uint32_t *)rom(coin_prompt_screen_jumps_ADDRESS))
#define coin_prompt_car_state_jumps_ADDRESS 0x01A7D0u
#define coin_prompt_car_state_jumps ((const uint32_t *)rom(coin_prompt_car_state_jumps_ADDRESS))
#define coin_prompt_message_state_jumps_ADDRESS 0x01A84Cu
#define coin_prompt_message_state_jumps ((const uint32_t *)rom(coin_prompt_message_state_jumps_ADDRESS))
#define ignition_key_prompt_state_jumps_ADDRESS 0x01A8F0u
#define ignition_key_prompt_state_jumps ((const uint32_t *)rom(ignition_key_prompt_state_jumps_ADDRESS))
#define coin_prompt_car_animations_ADDRESS 0x01A986u
#define coin_prompt_car_animations ((const uint16_t *)rom(coin_prompt_car_animations_ADDRESS))
#define coin_prompt_car_palettes_ADDRESS 0x01A996u
#define coin_prompt_car_palettes ((const uint16_t *)rom(coin_prompt_car_palettes_ADDRESS))
#define coin_prompt_message_animations_ADDRESS 0x01A9A6u
#define coin_prompt_message_animations ((const uint16_t *)rom(coin_prompt_message_animations_ADDRESS))
#define coin_prompt_message_palettes_ADDRESS 0x01A9ACu
#define coin_prompt_message_palettes ((const uint16_t *)rom(coin_prompt_message_palettes_ADDRESS))
#define coin_prompt_lcd_texts_ADDRESS 0x01A9BCu
#define coin_prompt_lcd_texts ((const uint32_t *)rom(coin_prompt_lcd_texts_ADDRESS))
#define credit_display_state_jumps_ADDRESS 0x01AA0Cu
#define credit_display_state_jumps ((const uint32_t *)rom(credit_display_state_jumps_ADDRESS))
#define credit_display_screen_jumps_ADDRESS 0x01AA4Cu
#define credit_display_screen_jumps ((const uint32_t *)rom(credit_display_screen_jumps_ADDRESS))
#define credit_digit_characters_ADDRESS 0x01AB8Eu
#define credit_digit_characters ((const uint8_t *)rom(credit_digit_characters_ADDRESS))
#define credit_display_animations_ADDRESS 0x01AB98u
#define credit_display_animations ((const uint16_t *)rom(credit_display_animations_ADDRESS))
#define credit_display_palettes_ADDRESS 0x01AB9Eu
#define credit_display_palettes ((const uint16_t *)rom(credit_display_palettes_ADDRESS))
#define title_car_screen_offsets_ADDRESS 0x01ABA4u
#define title_car_screen_offsets ((const uint16_t *)rom(title_car_screen_offsets_ADDRESS))
#define title_car_animations_ADDRESS 0x01AC34u
#define title_car_animations ((const uint16_t *)rom(title_car_animations_ADDRESS))
#define title_logo_car_palettes_ADDRESS 0x01AC44u
#define title_logo_car_palettes ((const uint16_t *)rom(title_logo_car_palettes_ADDRESS))
#define title_logo_car_wave_frames_ADDRESS 0x01ACC0u
#define title_logo_car_wave_frames ROM16(title_logo_car_wave_frames_ADDRESS)
#define demo_captions_state_jumps_ADDRESS 0x01ADF4u
#define demo_captions_state_jumps ((const uint32_t *)rom(demo_captions_state_jumps_ADDRESS))
#define demo_caption_state_jumps_ADDRESS 0x01AE98u
#define demo_caption_state_jumps ((const uint32_t *)rom(demo_caption_state_jumps_ADDRESS))
#define kanji_text_viewer_state_jumps_ADDRESS 0x01AFE8u
#define kanji_text_viewer_state_jumps ((const uint32_t *)rom(kanji_text_viewer_state_jumps_ADDRESS))
#define kanji_texts_ADDRESS 0x01B254u
#define kanji_texts ((const uint32_t *)rom(kanji_texts_ADDRESS))
#define test_mode_lcd_text_ADDRESS 0x01C378u
#define test_mode_lcd_text ROM32(test_mode_lcd_text_ADDRESS)
#define test_mode_lcd_text_4_to_7_ADDRESS 0x01C37Cu
#define test_mode_lcd_text_4_to_7 ROM32(test_mode_lcd_text_4_to_7_ADDRESS)
#define test_mode_lcd_text_8_to_11_ADDRESS 0x01C380u
#define test_mode_lcd_text_8_to_11 ROM32(test_mode_lcd_text_8_to_11_ADDRESS)
#define test_mode_lcd_text_12_to_15_ADDRESS 0x01C384u
#define test_mode_lcd_text_12_to_15 ROM32(test_mode_lcd_text_12_to_15_ADDRESS)
#define test_mode_page_jumps_ADDRESS 0x01C45Cu
#define test_mode_page_jumps ((const uint32_t *)rom(test_mode_page_jumps_ADDRESS))
#define test_mode_menu_headings_ADDRESS 0x01C560u
#define test_mode_menu_headings ((const uint32_t *)rom(test_mode_menu_headings_ADDRESS))
#define test_mode_menu_cursor_cells_ADDRESS 0x01C588u
#define test_mode_menu_cursor_cells ((const uint16_t *)rom(test_mode_menu_cursor_cells_ADDRESS))
#define volume_adjustment_cursor_cells_ADDRESS 0x01CF9Cu
#define volume_adjustment_cursor_cells ((const uint16_t *)rom(volume_adjustment_cursor_cells_ADDRESS))
#define volume_adjustment_cancel_cell_ADDRESS 0x01CF9Eu
#define volume_adjustment_cancel_cell ROM16(volume_adjustment_cancel_cell_ADDRESS)
#define input_test_on_off_cells_ADDRESS 0x01D3C0u
#define input_test_on_off_cells ((const uint32_t *)rom(input_test_on_off_cells_ADDRESS))
#define output_test_cursor_cells_ADDRESS 0x01D5AAu
#define output_test_cursor_cells ((const uint16_t *)rom(output_test_cursor_cells_ADDRESS))
#define output_test_exit_cell_ADDRESS 0x01D5BEu
#define output_test_exit_cell ROM16(output_test_exit_cell_ADDRESS)
#define test_mode_output_test_item_jumps_ADDRESS 0x01D5ECu
#define test_mode_output_test_item_jumps ((const uint32_t *)rom(test_mode_output_test_item_jumps_ADDRESS))
#define output_test_on_off_cells_ADDRESS 0x01D618u
#define output_test_on_off_cells ((const uint32_t *)rom(output_test_on_off_cells_ADDRESS))
#define test_mode_sound_test_item_jumps_ADDRESS 0x01D77Cu
#define test_mode_sound_test_item_jumps ((const uint32_t *)rom(test_mode_sound_test_item_jumps_ADDRESS))
#define sound_test_cursor_cells_ADDRESS 0x01D7C0u
#define sound_test_cursor_cells ((const uint16_t *)rom(sound_test_cursor_cells_ADDRESS))
#define sound_test_exit_cell_ADDRESS 0x01D7C4u
#define sound_test_exit_cell ROM16(sound_test_exit_cell_ADDRESS)
#define sound_test_codes_ADDRESS 0x01D7C6u
#define sound_test_codes ROM8(sound_test_codes_ADDRESS)
#define test_mode_game_assignments_item_jumps_ADDRESS 0x01DBD0u
#define test_mode_game_assignments_item_jumps ((const uint32_t *)rom(test_mode_game_assignments_item_jumps_ADDRESS))
#define game_assignments_cursor_cells_ADDRESS 0x01DCCEu
#define game_assignments_cursor_cells ((const uint16_t *)rom(game_assignments_cursor_cells_ADDRESS))
#define game_assignments_exit_cell_ADDRESS 0x01DCE0u
#define game_assignments_exit_cell ROM16(game_assignments_exit_cell_ADDRESS)
#define steering_wheel_reaction_texts_ADDRESS 0x01DCE4u
#define steering_wheel_reaction_texts ((const uint32_t *)rom(steering_wheel_reaction_texts_ADDRESS))
#define advertise_sound_texts_ADDRESS 0x01DCECu
#define advertise_sound_texts ((const uint32_t *)rom(advertise_sound_texts_ADDRESS))
#define test_mode_coin_assignments_item_jumps_ADDRESS 0x01E0D8u
#define test_mode_coin_assignments_item_jumps ((const uint32_t *)rom(test_mode_coin_assignments_item_jumps_ADDRESS))
#define test_mode_coin_assignments_manual_item_jumps_ADDRESS 0x01E0E4u
#define test_mode_coin_assignments_manual_item_jumps ((const uint32_t *)rom(test_mode_coin_assignments_manual_item_jumps_ADDRESS))
#define coin_assignments_cursor_cells_ADDRESS 0x01E200u
#define coin_assignments_cursor_cells ((const uint16_t *)rom(coin_assignments_cursor_cells_ADDRESS))
#define coin_manual_cursor_cells_ADDRESS 0x01E206u
#define coin_manual_cursor_cells ((const uint16_t *)rom(coin_manual_cursor_cells_ADDRESS))
#define coin_manual_exit_cell_ADDRESS 0x01E20Cu
#define coin_manual_exit_cell ROM16(coin_manual_exit_cell_ADDRESS)
#define coin_setting_descriptions_ADDRESS 0x01E220u
#define coin_setting_descriptions ((const uint32_t *)rom(coin_setting_descriptions_ADDRESS))
#define test_mode_network_assignments_item_jumps_ADDRESS 0x01E61Cu
#define test_mode_network_assignments_item_jumps ((const uint32_t *)rom(test_mode_network_assignments_item_jumps_ADDRESS))
#define network_assignments_cursor_cells_ADDRESS 0x01E67Cu
#define network_assignments_cursor_cells ((const uint16_t *)rom(network_assignments_cursor_cells_ADDRESS))
#define network_assignments_exit_cell_ADDRESS 0x01E682u
#define network_assignments_exit_cell ROM16(network_assignments_exit_cell_ADDRESS)
#define communication_texts_ADDRESS 0x01E684u
#define communication_texts ((const uint32_t *)rom(communication_texts_ADDRESS))
#define privilege_mode_texts_ADDRESS 0x01E68Cu
#define privilege_mode_texts ((const uint32_t *)rom(privilege_mode_texts_ADDRESS))
#define backup_data_clear_cursor_cells_ADDRESS 0x01EEAEu
#define backup_data_clear_cursor_cells ((const uint16_t *)rom(backup_data_clear_cursor_cells_ADDRESS))
#define backup_data_clear_no_cell_ADDRESS 0x01EEB0u
#define backup_data_clear_no_cell ROM16(backup_data_clear_no_cell_ADDRESS)
#define test_mode_instructions_text_ADDRESS 0x01F282u
#define test_mode_instructions_text ROM16(test_mode_instructions_text_ADDRESS)
#define test_mode_palettes_1_bytes_ADDRESS 0x01F302u
#define test_mode_palettes_1_bytes ROM16(test_mode_palettes_1_bytes_ADDRESS)
#define test_mode_palettes_2_bytes_ADDRESS 0x01F504u
#define test_mode_palettes_2_bytes ROM16(test_mode_palettes_2_bytes_ADDRESS)
#define road_object_streams_ADDRESS 0x020800u
#define road_object_streams ((const uint32_t *)rom(road_object_streams_ADDRESS))
#define object_lists_2E1F0_ADDRESS 0x02E1F0u
#define object_lists_2E1F0 ((const uint32_t *)rom(object_lists_2E1F0_ADDRESS))
#define background_palettes_ADDRESS 0x0318CCu
#define background_palettes ((const uint32_t *)rom(background_palettes_ADDRESS))
#define background_leaving_fades_ADDRESS 0x0319F0u
#define background_leaving_fades ((const uint32_t *)rom(background_leaving_fades_ADDRESS))
#define background_palette_fades_ADDRESS 0x031B14u
#define background_palette_fades ((const uint32_t *)rom(background_palette_fades_ADDRESS))
#define stage_road_palettes_ADDRESS 0x031C38u
#define stage_road_palettes ((const uint32_t *)rom(stage_road_palettes_ADDRESS))
#define background_road_palettes_ADDRESS 0x031CB8u
#define background_road_palettes ((const uint32_t *)rom(background_road_palettes_ADDRESS))
#define scene_palettes_ADDRESS 0x031DDCu
#define scene_palettes ((const uint32_t *)rom(scene_palettes_ADDRESS))
#define car_select_palette_ADDRESS 0x031E7Cu
#define car_select_palette ROM32(car_select_palette_ADDRESS)
#define ranking_palette_ADDRESS 0x031E80u
#define ranking_palette ROM32(ranking_palette_ADDRESS)
#define route_map_palette_ADDRESS 0x031E84u
#define route_map_palette ROM32(route_map_palette_ADDRESS)
#define title_palette_ADDRESS 0x031E88u
#define title_palette ROM32(title_palette_ADDRESS)
#define route_map_background_palette_ADDRESS 0x031E8Cu
#define route_map_background_palette ROM32(route_map_background_palette_ADDRESS)
#define stage_road_page_sets_ADDRESS 0x03CC80u
#define stage_road_page_sets ((const uint8_t *)rom(stage_road_page_sets_ADDRESS))
#define vblank_timer_period_ADDRESS 0x03F82Au
#define vblank_timer_period ROM16(vblank_timer_period_ADDRESS)
#define lcd_timer_period_ADDRESS 0x03F82Cu
#define lcd_timer_period ROM16(lcd_timer_period_ADDRESS)
#define link_message_kind_jumps_ADDRESS 0x03F85Cu
#define link_message_kind_jumps ((const uint32_t *)rom(link_message_kind_jumps_ADDRESS))
#define vblank_seat_inputs_jumps_ADDRESS 0x03FAC4u
#define vblank_seat_inputs_jumps ((const uint32_t *)rom(vblank_seat_inputs_jumps_ADDRESS))
#define screen_vblank_counters_ADDRESS 0x03FACCu
#define screen_vblank_counters ((const uint32_t *)rom(screen_vblank_counters_ADDRESS))
#define ic37_checksum_ADDRESS 0x03FFF8u
#define ic37_checksum ROM16(ic37_checksum_ADDRESS)
#define ic40_checksum_ADDRESS 0x03FFFAu
#define ic40_checksum ROM16(ic40_checksum_ADDRESS)
#define ic36_checksum_ADDRESS 0x03FFFCu
#define ic36_checksum ROM16(ic36_checksum_ADDRESS)
#define ic39_checksum_ADDRESS 0x03FFFEu
#define ic39_checksum ROM16(ic39_checksum_ADDRESS)
#define blank_sprite_places_ADDRESS 0x100000u
#define blank_sprite_places ROM32(blank_sprite_places_ADDRESS)
#define sine_table_ADDRESS 0x100C00u
#define sine_table ((const int16_t *)rom(sine_table_ADDRESS))
#define cosine_table_ADDRESS 0x101400u
#define cosine_table ((const int16_t *)rom(cosine_table_ADDRESS))
#define arctangents_ADDRESS 0x103400u
#define arctangents ((const uint16_t *)rom(arctangents_ADDRESS))
#define boot_tile_pages_5d_ADDRESS 0x109420u
#define boot_tile_pages_5d ROM32(boot_tile_pages_5d_ADDRESS)
#define boot_tile_pages_5e_ADDRESS 0x109428u
#define boot_tile_pages_5e ROM32(boot_tile_pages_5e_ADDRESS)
#define boot_tile_pages_29_to_4e_ADDRESS 0x109578u
#define boot_tile_pages_29_to_4e ROM32(boot_tile_pages_29_to_4e_ADDRESS)
#define no_background_tile_pages_ADDRESS 0x109684u
#define no_background_tile_pages ROM32(no_background_tile_pages_ADDRESS)
#define background_tile_page_lists_ADDRESS 0x109694u
#define background_tile_page_lists ((const uint32_t *)rom(background_tile_page_lists_ADDRESS))
#define text_sprite_words_0_1_ADDRESS 0x122B5Cu
#define text_sprite_words_0_1 ROM32(text_sprite_words_0_1_ADDRESS)
#define text_sprite_size_ADDRESS 0x122B60u
#define text_sprite_size ROM32(text_sprite_size_ADDRESS)
#define text_sprite_font_ADDRESS 0x122B64u
#define text_sprite_font ROM16(text_sprite_font_ADDRESS)
#define text_sprite_palette_ADDRESS 0x122B66u
#define text_sprite_palette ROM16(text_sprite_palette_ADDRESS)
#define entry_countdown_palette_fades_ADDRESS 0x13F66Cu
#define entry_countdown_palette_fades ROM32(entry_countdown_palette_fades_ADDRESS)
#define name_entry_palette_fades_ADDRESS 0x13F6C0u
#define name_entry_palette_fades ROM32(name_entry_palette_fades_ADDRESS)
#define manual_picked_palettes_ADDRESS 0x13F95Cu
#define manual_picked_palettes ROM32(manual_picked_palettes_ADDRESS)
#define automatic_picked_palettes_ADDRESS 0x13F998u
#define automatic_picked_palettes ROM32(automatic_picked_palettes_ADDRESS)
#define no_background_palettes_ADDRESS 0x13F9D4u
#define no_background_palettes ROM32(no_background_palettes_ADDRESS)
#define race_car_rider_places_ADDRESS 0x18D25Cu
#define race_car_rider_places ((const uint32_t *)rom(race_car_rider_places_ADDRESS))
#define perspective_scales_ADDRESS 0x19A340u
#define perspective_scales ((const uint16_t *)rom(perspective_scales_ADDRESS))
#define depth_layers_by_depth_ADDRESS 0x1A0340u
#define depth_layers_by_depth ((const uint16_t *)rom(depth_layers_by_depth_ADDRESS))
#define kanji_row_pixels_ADDRESS 0x1A6340u
#define kanji_row_pixels ((const uint32_t *)rom(kanji_row_pixels_ADDRESS))
#define kanji_font_ADDRESS 0x1CF000u
#define kanji_font ROM8(kanji_font_ADDRESS)

/* the board's registers (read and written through the bus) */
#define VIDEO_31F736 0x31F736u
#define VIDEO_31FF00 0x31FF00u
#define VIDEO_31FF05 0x31FF05u
#define SPRITE_RAM_400020 0x400020u
#define SPRITE_RAM_400030 0x400030u
#define SPRITE_RAM_404000 0x404000u
#define SPRITE_RAM_407000 0x407000u
#define SPRITE_RAM_40A000 0x40A000u
#define SPRITE_RAM_40D000 0x40D000u
#define SPRITE_RAM_414000 0x414000u
#define SPRITE_RAM_417000 0x417000u
#define SPRITE_RAM_41A000 0x41A000u
#define SPRITE_RAM_41D000 0x41D000u
#define SPRITE_CONTROL_500000 0x500000u
#define PALETTE_609C00 0x609C00u
#define MIXER_610040 0x610040u
#define MIXER_610044 0x610044u
#define MIXER_610048 0x610048u
#define SOUND_COMMAND_1 0x700700u
#define SOUND_SECOND_COMMAND_1 0x700708u
#define SOUND_ENGINE_REVS_1 0x700710u
#define SOUND_ENGINE_REQUESTS_1 0x700712u
#define SOUND_ENGINE_SAMPLE_1 0x700713u
#define SOUND_ENGINE_LOUDNESS_1 0x700714u
#define SOUND_TYRE_NOISE_1 0x700718u
#define SOUND_COMMAND_2 0x700800u
#define SOUND_ENGINE_REVS_2 0x700810u
#define SOUND_ENGINE_REQUESTS_2 0x700812u
#define SOUND_701F00 0x701F00u
#define SOUND_701F06 0x701F06u
#define SOUND_701F10 0x701F10u
#define SOUND_701F20 0x701F20u
#define SOUND_701F22 0x701F22u
#define SOUND_701F74 0x701F74u
#define SOUND_701F76 0x701F76u
#define SOUND_701F78 0x701F78u
#define SOUND_701F7A 0x701F7Au
#define SOUND_701FF0 0x701FF0u
#define SOUND_701FF4 0x701FF4u
#define SOUND_701FFF 0x701FFFu
#define IO_C00000 0xC00000u
#define IO_C00006 0xC00006u
#define IO_C00008 0xC00008u
#define IO_C0000A 0xC0000Au
#define IO_C0000C 0xC0000Cu
#define IO_C0000E 0xC0000Eu
#define IO_C0001C 0xC0001Cu
#define IO_C0001E 0xC0001Eu
#define IO_C00060 0xC00060u
#define IO_C00074 0xC00074u
#define IRQ_D00005 0xD00005u
#define IRQ_D00007 0xD00007u
#define IRQ_D00008 0xD00008u
#define IRQ_D0000A 0xD0000Au
#define RANDOM_D80000 0xD80000u

/* data in the program ROM symbols.txt labels */
#define signature_text_ADDRESS 0x00010Cu
#define default_settings_ADDRESS 0x0004C0u
#define second_task_start_frame_ADDRESS 0x000C98u
#define isdn_connection_objects_ADDRESS 0x000F70u
#define object_handlers_F70_ADDRESS 0x000F74u
#define unk_00183B_ADDRESS 0x00183Bu
#define car_presentation_objects_ADDRESS 0x001A78u
#define object_handlers_1A78_ADDRESS 0x001A7Cu
#define staff_credits_texts_ADDRESS 0x001AE8u
#define car_name_letter_palette_steps_ADDRESS 0x001D70u
#define ranking_objects_ADDRESS 0x002CB8u
#define name_entry_objects_ADDRESS 0x002D58u
#define object_handlers_2D58_ADDRESS 0x002D5Cu
#define player_car_objects_ADDRESS 0x00346Cu
#define object_handlers_346C_ADDRESS 0x003470u
#define race_objects_ADDRESS 0x003474u
#define object_handlers_3474_ADDRESS 0x003478u
#define demo_race_objects_ADDRESS 0x0034C4u
#define object_handlers_34C4_ADDRESS 0x0034C8u
#define route_map_objects_ADDRESS 0x0034ECu
#define object_handlers_34EC_ADDRESS 0x0034F0u
#define meaning_of_the_light_title_ADDRESS 0x003504u
#define thank_you_lcd_text_ADDRESS 0x005014u
#define welcome_letter_palette_steps_ADDRESS 0x006DCCu
#define transmission_panel_palette_steps_ADDRESS 0x006E18u
#define entry_lcd_text_ADDRESS 0x006EB0u
#define mega_driver_title_ADDRESS 0x006EB6u
#define rival_warning_text_ADDRESS 0x007CDBu
#define steer_frames_by_steering_ADDRESS 0x0092BCu
#define unk_009EFC_ADDRESS 0x009EFCu
#define unk_009FE4_ADDRESS 0x009FE4u
#define unk_00AB04_ADDRESS 0x00AB04u
#define check_point_lcd_text_branch_0_ADDRESS 0x00CB98u
#define check_point_lcd_text_branch_1_ADDRESS 0x00CBACu
#define total_time_text_ADDRESS 0x00D08Eu
#define branch_arrow_palette_steps_ADDRESS 0x00D0A0u
#define dream_flying_song_title_ADDRESS 0x00D0AEu
#define entry_invitation_objects_ADDRESS 0x00DF74u
#define object_handlers_DF74_ADDRESS 0x00DF78u
#define network_check_text_ADDRESS 0x00DF84u
#define communication_success_text_ADDRESS 0x00DF98u
#define machine_id_text_ADDRESS 0x00DFB4u
#define communication_failed_text_ADDRESS 0x00DFCEu
#define stand_alone_machine_text_ADDRESS 0x00DFEAu
#define master_machine_text_ADDRESS 0x00E00Cu
#define slave_machine_text_ADDRESS 0x00E034u
#define isdn_machine_text_ADDRESS 0x00E055u
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
#define unk_010309_ADDRESS 0x010309u
#define unk_010425_ADDRESS 0x010425u
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
#define credit_free_play_text_ADDRESS 0x01AB62u
#define credit_singular_text_ADDRESS 0x01AB72u
#define credit_plural_text_ADDRESS 0x01AB80u
#define title_logo_flash_palettes_ADDRESS 0x01AC54u
#define title_logo_copy_flash_palettes_ADDRESS 0x01AC62u
#define timeline_1AFC0_ADDRESS 0x01AFC0u
#define unk_01B248_ADDRESS 0x01B248u
#define test_mode_menu_text_ADDRESS 0x01C490u
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
#define no_bonus_adder_text_ADDRESS 0x01E39Eu
#define bonus_free_play_text_ADDRESS 0x01E3BCu
#define bonus_adder_text_ADDRESS 0x01E3CBu
#define coin_assignments_setting_places_ADDRESS 0x01E44Du
#define network_assignments_page_text_ADDRESS 0x01E62Cu
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
#define unk_030B60_ADDRESS 0x030B60u
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
#define SECONDS_PER_MINUTE 0x3Cu
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
			uint8_t unk_09;
			uint8_t unk_0A;
			uint8_t unk_0B;
			uint8_t state;
			uint8_t unk_0D;
			uint8_t unk_0E;
			uint8_t unk_0F;
			uint16_t unk_10;
			uint8_t unk_12;
			uint8_t unk_13;
			uint32_t parent;
			uint32_t parameter;
			int16_t position;
			uint8_t unk_1E;
			uint8_t pad_1F[0x1];
			int16_t x_offset;
			uint8_t pad_22[0x2];
			int16_t y_offset;
			uint8_t pad_26[0x2];
			uint16_t position_fraction;
			uint8_t pad_2A[0x2];
			uint16_t unk_2C;
			uint16_t unk_2E;
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
			uint8_t unk_45;
			uint8_t pad_46[0xA];
			uint32_t previous_place;
			uint32_t unk_54;
			uint16_t unk_58;
			uint8_t pad_5A[0x2];
			uint32_t unk_5C;
			uint32_t unk_60;
			uint32_t unk_64;
			uint16_t unk_68;
			uint16_t unk_6A;
			uint32_t unk_6C;
			uint32_t unk_70;
			uint32_t unk_74;
			uint32_t unk_78;
			uint32_t unk_7C;
			uint32_t list_data;
			uint16_t unk_84;
			uint8_t unk_86;
			uint8_t unk_87;
			uint32_t unk_88;
			uint32_t unk_8C;
			uint32_t unk_90;
			uint32_t unk_94;
			uint32_t unk_98;
			uint32_t unk_9C;
			uint32_t unk_A0;
			uint32_t unk_A4;
			uint32_t unk_A8;
			uint32_t unk_AC;
			uint32_t unk_B0;
			uint32_t unk_B4;
			uint16_t unk_B8;
			uint16_t unk_BA;
			uint8_t unk_BC;
			uint8_t unk_BD;
			uint8_t pad_BE[0x1];
			uint8_t unk_BF;
			uint32_t unk_C0;
			uint32_t unk_C4;
			uint32_t unk_C8;
			uint16_t unk_CC;
			uint16_t unk_CE;
			uint8_t unk_D0;
			uint8_t unk_D1;
			uint8_t unk_D2;
			uint8_t pad_D3[0x1];
			uint32_t unk_D4;
			uint32_t unk_D8;
			uint8_t unk_DC;
			uint8_t unk_DD;
			uint8_t pad_DE[0x2];
			uint8_t unk_E0;
			uint8_t unk_E1;
			uint16_t unk_E2;
			uint8_t unk_E4;
			uint8_t pad_E5[0xB];
			uint16_t unk_F0;
			uint16_t unk_F2;
			uint16_t unk_F4;
			uint8_t unk_F6;
			uint8_t unk_F7;
			uint8_t unk_F8;
			uint8_t pad_F9[0x7];
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
			uint8_t pad_02[0x10];
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
			uint8_t car_models;
			uint8_t pad_81[0x7];
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
			uint32_t car_flags;
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
			uint8_t pad_1E[0xD2];
			uint8_t row_steps;
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
			union
			{
				uint32_t name_word;
				uint8_t name;
			};
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
			uint8_t text;
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
			uint8_t page_table;
			uint8_t pad_19[0x3B];
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
			uint32_t source_pages;
			uint8_t pad_1C[0xC];
			uint32_t target_pages;
			uint8_t pad_2C[0xC];
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
	uint8_t stage_history;
	uint8_t pad_D2[0x5];
	uint8_t branch_history;
	uint8_t pad_D8[0x4];
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
	uint8_t pad_120[0x30];
	uint8_t fade_level;
	int8_t fade_step;
	uint8_t fade_end;
	uint8_t fade_kind;
	uint8_t fade_channels;
	uint8_t pad_155[0xB];
	uint16_t mixer_values;
	uint8_t pad_162[0x3D];
	uint8_t colour_offset_mode;
	uint8_t pad_1A0[0x10];
	uint16_t mixer_defaults;
	uint8_t pad_1B2[0x4E];
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
	uint8_t accepted_players;
	uint8_t pad_301[0x3];
	uint32_t last_four_accepted_players;
	uint32_t host_record;
	uint8_t host_player;
	uint8_t pad_30D[0x2];
	uint8_t entrant_count;
	uint8_t entrants;
	uint8_t pad_311[0x3];
	uint32_t last_four_entrants;
	uint8_t is_host;
	uint8_t entry_accepted;
	uint8_t in_game;
	uint8_t race_running;
	uint8_t race_started;
	uint8_t pad_31D[0x2];
	uint8_t place;
	uint8_t places;
	uint8_t pad_321[0x7];
	uint8_t name_entries_seen;
	uint8_t pad_329[0x7];
	uint8_t counted_stages;
	uint8_t pad_331[0x7];
	uint8_t counted_passes;
	uint8_t pad_339[0x9];
	uint8_t pass_seen;
	uint8_t pad_343[0x1];
	uint8_t route_stages;
	uint8_t pad_345[0x4];
	uint8_t forks_decided;
	uint8_t route_branches;
	uint8_t pad_34B[0x5];
	uint16_t leader_window_segment;
	uint8_t leader_stage;
	uint8_t pad_353[0x7];
	uint8_t checkpoint_passes;
	uint8_t pad_35B[0x5];
	uint8_t checkpoint_orders;
	uint8_t pad_361[0x2F];
	uint8_t checkpoint_order;
	uint8_t pad_391[0x2F];
	uint16_t checkpoint_times_by_player;
	uint8_t pad_3C2[0x5E];
	uint16_t checkpoint_times;
	uint8_t pad_422[0x5E];
	uint8_t checkpoints_passed;
	uint8_t pad_481[0x2E];
	uint8_t demo_player_count;
	uint8_t demo_players;
	uint8_t pad_4B1[0x41];
	uint8_t demo_stage;
	uint8_t pad_4F3[0xD];
	uint32_t own_hit_box;
	uint8_t pad_504[0xC];
	uint32_t hit_box_count;
	uint8_t pad_514[0x1EC];
	uint16_t stage_lengths;
	uint8_t pad_702[0x3C];
	uint16_t branch_road_length;
	uint8_t pad_740[0xC0];
	uint8_t sound_queue_read;
	uint8_t sound_queue_write;
	uint8_t pad_802[0x20];
	uint8_t sound_start_cleared;
	uint8_t pad_823[0x1D];
	uint8_t second_sound_queue_read;
	uint8_t second_sound_queue_write;
	uint8_t pad_842[0xBE];
	uint32_t palette_upload_count;
	uint8_t pad_904[0x1FC];
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
	uint8_t lcd_text;
	uint8_t pad_F11[0x3];
	uint32_t lcd_text_4_to_7;
	uint32_t lcd_text_8_to_11;
	uint32_t lcd_text_12_to_15;
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
	uint8_t pad_FA9[0xF57];
	uint8_t crest_count;
	uint8_t pad_1F01[0xF];
	uint16_t crest_layers;
	uint8_t pad_1F12[0x1E];
	uint16_t crest_heights;
	uint8_t pad_1F32[0x30CE];
	uint16_t depth_lists;
	uint8_t pad_5002[0x676];
	uint16_t text_depth_list_head;
	uint16_t text_depth_list_tail;
	uint8_t pad_567C[0x184];
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
	uint8_t pad_5808[0x27F8];
};

struct object_area
{
	uint32_t list;
	uint32_t list_first;
	uint32_t next_list;
	uint32_t list_end;
	uint8_t slots;
	uint8_t pad_11[0x7FEF];
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

struct object_list
{
	uint16_t count;
	uint8_t pad_02[0xFE];
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
	uint8_t pad_14[0x4];
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
	uint8_t pad_16[0x4];
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
	uint8_t pad_14[0x4];
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
	uint8_t model;
	uint8_t name[0x3];
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

struct road_segment
{
	uint16_t heading;
	uint16_t pitch;
	uint32_t x;
	uint32_t z;
	uint32_t y;
	uint16_t width;
	uint16_t half_width;
	uint8_t width_code;
	uint8_t road_event;
	uint8_t pad_16[0xA];
};

#pragma pack(pop)

_Static_assert(sizeof(struct object) == 0x100, "struct object has the size of the game's blocks");
_Static_assert(sizeof(struct car) == 0x200, "struct car has the size of the game's blocks");
_Static_assert(sizeof(struct screen) == 0x8000, "struct screen has the size of the game's blocks");
_Static_assert(sizeof(struct object_area) == 0x8000, "struct object_area has the size of the game's blocks");
_Static_assert(sizeof(struct race) == 0x80, "struct race has the size of the game's blocks");
_Static_assert(sizeof(struct object_list) == 0x100, "struct object_list has the size of the game's blocks");
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
_Static_assert(sizeof(struct road_segment) == 0x20, "struct road_segment has the size of the game's blocks");

#define OBJECT(address) ((struct object *)ram(address))
#define CAR(address) ((struct car *)ram(address))
#define SCREEN(address) ((struct screen *)ram(address))
#define OBJECT_AREA(address) ((struct object_area *)ram(address))
#define RACE(address) ((struct race *)ram(address))
#define OBJECT_LIST(address) ((struct object_list *)ram(address))
#define ROAD_ROW_RECORD(address) ((const struct road_row_record *)rom(address))
#define ROAD_OBJECT_STREAM_ENTRY(address) ((const struct road_object_stream_entry *)rom(address))
#define FLASHING_SCENERY_RECORD(address) ((const struct flashing_scenery_record *)rom(address))
#define FLASHING_SCENERY_PALETTES(address) ((const int16_t *)rom(address))
#define AURORA_RECORD(address) ((const struct aurora_record *)rom(address))
#define AURORA_ANIMATIONS(address) ((const int16_t *)rom(address))
#define ROAD_CROSSER_RECORD(address) ((const struct road_crosser_record *)rom(address))
#define MOVING_SCENERY_RECORD(address) ((const struct moving_scenery_record *)rom(address))
#define SWOOPING_HELICOPTER_RECORD(address) ((const struct swooping_helicopter_record *)rom(address))
#define START_GATE_RECORD(address) ((const struct start_gate_record *)rom(address))
#define FLAGMAN_RECORD(address) ((const struct flagman_record *)rom(address))
#define START_BALLOON_RECORD(address) ((const struct start_balloon_record *)rom(address))
#define SANTA_SLEIGH_RECORD(address) ((const struct santa_sleigh_record *)rom(address))
#define GOAL_PARADE_RECORD(address) ((const struct goal_parade_record *)rom(address))
#define SOUNDING_SCENERY_RECORD(address) ((const struct sounding_scenery_record *)rom(address))
#define TRAFFIC_RECORD(address) ((const struct traffic_record *)rom(address))
#define TRAFFIC_SPRITE_RECORD(address) ((const struct traffic_sprite_record *)rom(address))
#define TRAFFIC_SPAWN_ENTRY(address) ((const struct traffic_spawn_entry *)rom(address))
#define TRAFFIC_LINE_RECORD(address) ((const struct traffic_line_record *)rom(address))
#define TRAFFIC_LINE_FOLLOWERS(address) ((const uint32_t *)rom(address))
#define BACKDROP_RECORD(address) ((const struct backdrop_record *)rom(address))
#define CAR_SELECT_TILEMAP_SETUP(address) ((const struct car_select_tilemap_setup *)rom(address))
#define ANIMATION_PALETTE(address) ((const struct animation_palette *)rom(address))
#define TURNTABLE_SCENERY_GROUP(address) ((const struct turntable_scenery_group *)rom(address))
#define WELCOME_LETTER_PLACE(address) ((const struct welcome_letter_place *)rom(address))
#define SELECT_INSTRUCTION_PIECE(address) ((const struct select_instruction_piece *)rom(address))
#define TRAFFIC_PATH_STEP(address) ((const struct traffic_path_step *)rom(address))
#define GOAL_ACTOR_SCRIPT_WORDS(address) ((const uint16_t *)rom(address))
#define PALETTE_CYCLE_RECORD(address) ((const struct palette_cycle_record *)rom(address))
#define PALETTE_CYCLE_STEPS(address) ((const uint32_t *)rom(address))
#define SUNSET_FADE_RECORD(address) ((const struct sunset_fade_record *)rom(address))
#define PALETTE_FADE_LIST(address) ((const struct palette_fade_list *)rom(address))
#define PALETTE_FADE_ENTRY(address) ((const struct palette_fade_entry *)rom(address))
#define GRID_SEGMENTS(address) ((const uint16_t *)rom(address))
#define GRID_ROAD_PLACES(address) ((const uint32_t *)rom(address))
#define GOAL_STOP(address) ((const struct goal_stop *)rom(address))
#define CRASH_PHYSICS(address) ((const struct crash_physics *)rom(address))
#define WHEEL_STEERING(address) ((const int8_t *)rom(address))
#define CAR_HANDLING(address) ((const struct car_handling *)rom(address))
#define GEAR_LIST(address) ((const struct gear_list *)rom(address))
#define TORQUE_POINT(address) ((const struct torque_point *)rom(address))
#define ROADSIDE_SURFACE(address) ((const struct roadside_surface *)rom(address))
#define STAGE_ROADSIDE_SURFACES(address) ((const struct stage_roadside_surfaces *)rom(address))
#define OBJECT_PALETTE_STEPS(address) ((const int16_t *)rom(address))
#define SPRITE_ANIMATION(address) ((const struct sprite_animation *)rom(address))
#define SPRITE_ANIMATION_POINTER(address) ((const uint32_t *)rom(address))
#define SPRITE_FRAME(address) ((const struct sprite_frame *)rom(address))
#define SPRITE_PIECE(address) ((const struct sprite_piece *)rom(address))
#define SPRITE_SIZE_STEP(address) ((const struct sprite_size_step *)rom(address))
#define SPRITE_PALETTE_WORDS(address) ((const uint16_t *)rom(address))
#define SPRITE_PALETTE_POINTER(address) ((const uint32_t *)rom(address))
#define CAR_GEAR_RATIOS(address) ((const uint8_t *)rom(address))
#define CHECKPOINT_BOARD_PLACE(address) ((const struct checkpoint_board_place *)rom(address))
#define SPRITE_LIST_SWITCH(address) ((const struct sprite_list_switch *)rom(address))
#define SPRITE_LIST_SWITCH_POINTER(address) ((const uint32_t *)rom(address))
#define ITEM_HIT_BOX(address) ((const struct item_hit_box *)rom(address))
#define COURSE_POINTER(address) ((const uint32_t *)rom(address))
#define COURSE_FORK(address) ((const struct course_fork *)rom(address))
#define ROAD_PROJECTION_PATTERN(address) ((const uint8_t *)rom(address))
#define ROAD_ROW_SIZE_STEPS(address) ((const uint8_t *)rom(address))
#define ROAD_ROW_SIZE_STEPS_POINTER(address) ((const uint32_t *)rom(address))
#define ROAD_ROW_SIZE_SHIFTS(address) ((const int8_t *)rom(address))
#define ROAD_ROW_SIZE_SHIFTS_POINTER(address) ((const uint32_t *)rom(address))
#define SCREEN_HARDWARE(address) ((const struct screen_hardware *)rom(address))
#define SCREEN_HARDWARE_POINTER(address) ((const uint32_t *)rom(address))
#define DRAW_DISTANCE_LIST(address) ((const uint16_t *)rom(address))
#define SCREEN_VIEW_DEFAULT(address) ((const struct screen_view_default *)rom(address))
#define SPRITE_CONTROL_VALUES(address) ((const uint8_t *)rom(address))
#define HIDDEN_LAYER_PAIR(address) ((const struct hidden_layer_pair *)rom(address))
#define SCREEN_FADE_START(address) ((const struct screen_fade_start *)rom(address))
#define TEXT_FONT_ROWS(address) ((const uint8_t *)rom(address))
#define TEXT_CHARACTERS(address) ((const uint8_t *)rom(address))
#define BACKGROUND_REGISTERS(address) ((const struct background_registers *)rom(address))
#define BACKGROUND_SETUP(address) ((const struct background_setup *)rom(address))
#define FIRST_ATTRACT_CARS(address) ((const uint8_t *)rom(address))
#define BAR_PICTURE_BAR(address) ((const struct bar_picture_bar *)rom(address))
#define STAFF_CREDITS_CODE_CONTROLS(address) ((const uint8_t *)rom(address))
#define CAR_NAME_LETTERS(address) ((const uint8_t *)rom(address))
#define COURSE_RECORD(address) ((const struct course_record *)rom(address))
#define REJECTED_NAMES(address) ((const uint32_t *)rom(address))
#define RANKING_BACKGROUND_REGISTERS(address) ((const struct ranking_background_registers *)rom(address))
#define DEMO_RACE_CHOICE(address) ((const struct demo_race_choice *)rom(address))
#define ATTRACT_LAMP_STEP(address) ((const struct attract_lamp_step *)rom(address))
#define ROUTE_MAP_REGISTERS(address) ((const struct route_map_registers *)rom(address))
#define ROUTE_MAP_LEG_CAR_PLACE(address) ((const struct route_map_leg_car_place *)rom(address))
#define STAGE_RESULT_TEXT(address) ((const struct stage_result_text *)rom(address))
#define STAGE_RESULT_PLACE_TEXT(address) ((const struct stage_result_place_text *)rom(address))
#define ROUTE_MAP_LANDMARK_RECORD(address) ((const struct route_map_landmark_record *)rom(address))
#define SOUND_CUE(address) ((const struct sound_cue *)rom(address))
#define ROAD_SURFACE_LAYER_SETUP(address) ((const struct road_surface_layer_setup *)rom(address))
#define ROAD_TILES_CHANGE_BYTES(address) ((const uint8_t *)rom(address))
#define RACE_BACKGROUND_LAYER_SETUP(address) ((const struct race_background_layer_setup *)rom(address))
#define BACKGROUND_PAGE_WORD_STEPS(address) ((const uint32_t *)rom(address))
#define PALETTE_COLOURS(address) ((const uint16_t *)rom(address))
#define SONG_TITLE_RECORD(address) ((const struct song_title_record *)rom(address))
#define TITLE_SCREEN_LAYER_SETUP(address) ((const struct title_screen_layer_setup *)rom(address))
#define TITLE_WAVE_FRAMES(address) ((const uint16_t *)rom(address))
#define TITLE_CAR_RECORD(address) ((const struct title_car_record *)rom(address))
#define LCD_TEXT_LINE(address) ((const struct lcd_text_line *)rom(address))
#define LCD_CHARACTER_ROWS(address) ((const uint8_t *)rom(address))
#define DJ_MOUTH_SHAPE_NUMBERS(address) ((const uint8_t *)rom(address))
#define TITLE_PLACE(address) ((const struct title_place *)rom(address))
#define PICTURE_RECORD(address) ((const struct picture_record *)rom(address))
#define TITLE_LOGO_PALETTES(address) ((const uint16_t *)rom(address))
#define DEMO_CAPTION_CUE(address) ((const struct demo_caption_cue *)rom(address))
#define KANJI_TEXT(address) ((const int16_t *)rom(address))
#define KANJI_CHARACTER_ROWS(address) ((const uint8_t *)rom(address))
#define TEST_MODE_CELLS(address) ((const uint16_t *)rom(address))
#define MEMORY_TEST_RESULT_TEXT(address) ((const struct memory_test_result_text *)rom(address))
#define TEST_MODE_CHARACTERS(address) ((const uint8_t *)rom(address))
#define OUTPUT_TEST_OUTPUT(address) ((const struct output_test_output *)rom(address))
#define TEST_MODE_SETTING_PLACES(address) ((const uint8_t *)rom(address))
#define COIN_SETTING_RECORD(address) ((const struct coin_setting_record *)rom(address))
#define PRIVILEGE_MODE_SETTING(address) ((const struct privilege_mode_setting *)rom(address))
#define TIME_DIGIT_STEP(address) ((const struct time_digit_step *)rom(address))
#define ROAD_SEGMENT(address) ((struct road_segment *)ram(address))

/* the program ROM's tables of records, by name: name[i].field */
#define bar_picture_bars_ADDRESS 0x0011B1u
#define bar_picture_bars ((const struct bar_picture_bar *)rom(bar_picture_bars_ADDRESS))
#define attract_lamp_steps_ADDRESS 0x001607u
#define attract_lamp_steps ((const struct attract_lamp_step *)rom(attract_lamp_steps_ADDRESS))
#define staff_credits_code_sequence_ADDRESS 0x001ADCu
#define staff_credits_code_sequence ((const uint8_t *)rom(staff_credits_code_sequence_ADDRESS))
#define ranking_background_register_sets_ADDRESS 0x002C44u
#define ranking_background_register_sets ((const struct ranking_background_registers *)rom(ranking_background_register_sets_ADDRESS))
#define default_course_records_ADDRESS 0x002CC4u
#define default_course_records ((const struct course_record *)rom(default_course_records_ADDRESS))
#define name_entry_rejected_names_ADDRESS 0x002D7Cu
#define name_entry_rejected_names ((const uint32_t *)rom(name_entry_rejected_names_ADDRESS))
#define attract_demo_races_ADDRESS 0x0034F4u
#define attract_demo_races ((const struct demo_race_choice *)rom(attract_demo_races_ADDRESS))
#define route_map_register_sets_ADDRESS 0x0047E8u
#define route_map_register_sets ((const struct route_map_registers *)rom(route_map_register_sets_ADDRESS))
#define route_map_start_leg_car_places_ADDRESS 0x004964u
#define route_map_start_leg_car_places ((const struct route_map_leg_car_place *)rom(route_map_start_leg_car_places_ADDRESS))
#define route_map_first_fork_leg_car_places_ADDRESS 0x0049A4u
#define route_map_first_fork_leg_car_places ((const struct route_map_leg_car_place *)rom(route_map_first_fork_leg_car_places_ADDRESS))
#define route_map_fork_leg_car_places_ADDRESS 0x004A24u
#define route_map_fork_leg_car_places ((const struct route_map_leg_car_place *)rom(route_map_fork_leg_car_places_ADDRESS))
#define stage_result_labels_ADDRESS 0x004BA4u
#define stage_result_labels ((const struct stage_result_text *)rom(stage_result_labels_ADDRESS))
#define stage_result_places_ADDRESS 0x004BDAu
#define stage_result_places ((const struct stage_result_place_text *)rom(stage_result_places_ADDRESS))
#define stage_result_retire_ADDRESS 0x004C0Au
#define stage_result_retire ((const struct stage_result_text *)rom(stage_result_retire_ADDRESS))
#define route_map_landmarks_ADDRESS 0x004C14u
#define route_map_landmarks ((const struct route_map_landmark_record *)rom(route_map_landmarks_ADDRESS))
#define traffic_path_ADDRESS 0x0067B2u
#define traffic_path ((const struct traffic_path_step *)rom(traffic_path_ADDRESS))
#define car_select_tilemap_setups_ADDRESS 0x006920u
#define car_select_tilemap_setups ((const struct car_select_tilemap_setup *)rom(car_select_tilemap_setups_ADDRESS))
#define turntable_car_looks_ADDRESS 0x0069B8u
#define turntable_car_looks ((const struct animation_palette *)rom(turntable_car_looks_ADDRESS))
#define turntable_scenery_groups_ADDRESS 0x006A18u
#define turntable_scenery_groups ((const struct turntable_scenery_group *)rom(turntable_scenery_groups_ADDRESS))
#define welcome_letter_places_ADDRESS 0x006DACu
#define welcome_letter_places ((const struct welcome_letter_place *)rom(welcome_letter_places_ADDRESS))
#define select_instruction_pieces_ADDRESS 0x006DF6u
#define select_instruction_pieces ((const struct select_instruction_piece *)rom(select_instruction_pieces_ADDRESS))
#define driver_couple_looks_ADDRESS 0x006E58u
#define driver_couple_looks ((const struct animation_palette *)rom(driver_couple_looks_ADDRESS))
#define stage_roadside_surface_lists_ADDRESS 0x00B78Cu
#define stage_roadside_surface_lists ((const struct stage_roadside_surfaces *)rom(stage_roadside_surface_lists_ADDRESS))
#define checkpoint_board_car_places_ADDRESS 0x00CF68u
#define checkpoint_board_car_places ((const struct checkpoint_board_place *)rom(checkpoint_board_car_places_ADDRESS))
#define checkpoint_board_number_places_ADDRESS 0x00CF88u
#define checkpoint_board_number_places ((const struct checkpoint_board_place *)rom(checkpoint_board_number_places_ADDRESS))
#define checkpoint_board_behind_places_ADDRESS 0x00CFA8u
#define checkpoint_board_behind_places ((const struct checkpoint_board_place *)rom(checkpoint_board_behind_places_ADDRESS))
#define sprite_list_switches_ADDRESS 0x00F508u
#define sprite_list_switches ((const uint32_t *)rom(sprite_list_switches_ADDRESS))
#define hidden_layer_pairs_ADDRESS 0x010B06u
#define hidden_layer_pairs ((const struct hidden_layer_pair *)rom(hidden_layer_pairs_ADDRESS))
#define screen_hardware_records_ADDRESS 0x010BA0u
#define screen_hardware_records ((const uint32_t *)rom(screen_hardware_records_ADDRESS))
#define screen_view_defaults_ADDRESS 0x010BC8u
#define screen_view_defaults ((const struct screen_view_default *)rom(screen_view_defaults_ADDRESS))
#define screen_fade_starts_ADDRESS 0x01173Au
#define screen_fade_starts ((const struct screen_fade_start *)rom(screen_fade_starts_ADDRESS))
#define race_opening_sound_cues_ADDRESS 0x014660u
#define race_opening_sound_cues ((const struct sound_cue *)rom(race_opening_sound_cues_ADDRESS))
#define stage_end_sound_cues_ADDRESS 0x01466Fu
#define stage_end_sound_cues ((const struct sound_cue *)rom(stage_end_sound_cues_ADDRESS))
#define road_surface_layer_setups_ADDRESS 0x01710Cu
#define road_surface_layer_setups ((const struct road_surface_layer_setup *)rom(road_surface_layer_setups_ADDRESS))
#define race_background_layer_setups_ADDRESS 0x017A50u
#define race_background_layer_setups ((const struct race_background_layer_setup *)rom(race_background_layer_setups_ADDRESS))
#define lcd_custom_characters_ADDRESS 0x018B4Eu
#define lcd_custom_characters ((const uint8_t *)rom(lcd_custom_characters_ADDRESS))
#define lcd_boot_text_ADDRESS 0x018B90u
#define lcd_boot_text ((const struct lcd_text_line *)rom(lcd_boot_text_ADDRESS))
#define radio_station_title_ADDRESS 0x01954Cu
#define radio_station_title ((const struct song_title_record *)rom(radio_station_title_ADDRESS))
#define ignition_key_prompt_pieces_ADDRESS 0x01A95Bu
#define ignition_key_prompt_pieces ((const struct picture_record *)rom(ignition_key_prompt_pieces_ADDRESS))
#define title_car_records_ADDRESS 0x01ABB4u
#define title_car_records ((const struct title_car_record *)rom(title_car_records_ADDRESS))
#define title_pictures_ADDRESS 0x01AC70u
#define title_pictures ((const struct picture_record *)rom(title_pictures_ADDRESS))
#define title_seagull_places_ADDRESS 0x01ACA0u
#define title_seagull_places ((const struct title_place *)rom(title_seagull_places_ADDRESS))
#define title_logo_stripe_places_ADDRESS 0x01ACA8u
#define title_logo_stripe_places ((const struct title_place *)rom(title_logo_stripe_places_ADDRESS))
#define title_logo_car_places_ADDRESS 0x01ACB4u
#define title_logo_car_places ((const struct title_place *)rom(title_logo_car_places_ADDRESS))
#define title_screen_layer_setups_ADDRESS 0x01ACCCu
#define title_screen_layer_setups ((const struct title_screen_layer_setup *)rom(title_screen_layer_setups_ADDRESS))
#define title_background_setups_ADDRESS 0x01AD0Cu
#define title_background_setups ((const struct background_setup *)rom(title_background_setups_ADDRESS))
#define demo_caption_pictures_ADDRESS 0x01AF00u
#define demo_caption_pictures ((const struct picture_record *)rom(demo_caption_pictures_ADDRESS))
#define memory_test_result_texts_ADDRESS 0x01CDC8u
#define memory_test_result_texts ((const struct memory_test_result_text *)rom(memory_test_result_texts_ADDRESS))
#define output_test_outputs_ADDRESS 0x01D5C0u
#define output_test_outputs ((const struct output_test_output *)rom(output_test_outputs_ADDRESS))
#define coin_setting_records_ADDRESS 0x01E3E8u
#define coin_setting_records ((const struct coin_setting_record *)rom(coin_setting_records_ADDRESS))
#define privilege_mode_settings_ADDRESS 0x01E6E5u
#define privilege_mode_settings ((const struct privilege_mode_setting *)rom(privilege_mode_settings_ADDRESS))
#define sprite_palettes_ADDRESS 0x032BA4u
#define sprite_palettes ((const uint32_t *)rom(sprite_palettes_ADDRESS))
#define sprite_animations_ADDRESS 0x034138u
#define sprite_animations ((const uint32_t *)rom(sprite_animations_ADDRESS))
#define fork_branch_piece_animation_ADDRESS 0x034308u
#define fork_branch_piece_animation ((const uint32_t *)rom(fork_branch_piece_animation_ADDRESS))
#define stage_courses_ADDRESS 0x03CC00u
#define stage_courses ((const uint32_t *)rom(stage_courses_ADDRESS))
#define sprite_size_steps_ADDRESS 0x122B74u
#define sprite_size_steps ((const struct sprite_size_step *)rom(sprite_size_steps_ADDRESS))
#define road_row_size_step_lists_ADDRESS 0x1598CCu
#define road_row_size_step_lists ((const uint32_t *)rom(road_row_size_step_lists_ADDRESS))
#define road_row_size_shift_lists_ADDRESS 0x159AFCu
#define road_row_size_shift_lists ((const uint32_t *)rom(road_row_size_shift_lists_ADDRESS))

#endif
