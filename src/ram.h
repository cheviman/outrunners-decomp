/**
 *    Name:        ram.h
 *    Description: the game's variables, tables, board registers and structures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    25-10-2026
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
#define memory_test_results RAM8(memory_test_results_ADDRESS)
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
#define rom_0000B4_ADDRESS 0x0000B4u
#define rom_0000B4 ROM8(rom_0000B4_ADDRESS)
#define rom_0000B5_ADDRESS 0x0000B5u
#define rom_0000B5 ROM8(rom_0000B5_ADDRESS)
#define rom_0000B6_ADDRESS 0x0000B6u
#define rom_0000B6 ROM8(rom_0000B6_ADDRESS)
#define rom_0000B7_ADDRESS 0x0000B7u
#define rom_0000B7 ROM8(rom_0000B7_ADDRESS)
#define rom_0008C4_ADDRESS 0x0008C4u
#define rom_0008C4 ((const uint32_t *)rom(rom_0008C4_ADDRESS))
#define rom_0008CC_ADDRESS 0x0008CCu
#define rom_0008CC ((const uint32_t *)rom(rom_0008CC_ADDRESS))
#define rom_000B0C_ADDRESS 0x000B0Cu
#define rom_000B0C ((const uint32_t *)rom(rom_000B0C_ADDRESS))
#define rom_000B14_ADDRESS 0x000B14u
#define rom_000B14 ((const uint8_t *)rom(rom_000B14_ADDRESS))
#define rom_000D14_ADDRESS 0x000D14u
#define rom_000D14 ((const uint32_t *)rom(rom_000D14_ADDRESS))
#define rom_000DAC_ADDRESS 0x000DACu
#define rom_000DAC ((const uint32_t *)rom(rom_000DAC_ADDRESS))
#define rom_000EB0_ADDRESS 0x000EB0u
#define rom_000EB0 ((const uint32_t *)rom(rom_000EB0_ADDRESS))
#define rom_000F78_ADDRESS 0x000F78u
#define rom_000F78 ((const uint32_t *)rom(rom_000F78_ADDRESS))
#define rom_000F80_ADDRESS 0x000F80u
#define rom_000F80 ROM32(rom_000F80_ADDRESS)
#define rom_000F84_ADDRESS 0x000F84u
#define rom_000F84 ROM32(rom_000F84_ADDRESS)
#define rom_000F88_ADDRESS 0x000F88u
#define rom_000F88 ROM32(rom_000F88_ADDRESS)
#define rom_000F8C_ADDRESS 0x000F8Cu
#define rom_000F8C ROM32(rom_000F8C_ADDRESS)
#define rom_000F90_ADDRESS 0x000F90u
#define rom_000F90 ROM32(rom_000F90_ADDRESS)
#define rom_001014_ADDRESS 0x001014u
#define rom_001014 ((const uint32_t *)rom(rom_001014_ADDRESS))
#define rom_00146C_ADDRESS 0x00146Cu
#define rom_00146C ((const uint32_t *)rom(rom_00146C_ADDRESS))
#define rom_001530_ADDRESS 0x001530u
#define rom_001530 ((const uint32_t *)rom(rom_001530_ADDRESS))
#define rom_001694_ADDRESS 0x001694u
#define rom_001694 ((const uint32_t *)rom(rom_001694_ADDRESS))
#define rom_0019CC_ADDRESS 0x0019CCu
#define rom_0019CC ((const uint32_t *)rom(rom_0019CC_ADDRESS))
#define rom_001A88_ADDRESS 0x001A88u
#define rom_001A88 ((const uint16_t *)rom(rom_001A88_ADDRESS))
#define rom_001A98_ADDRESS 0x001A98u
#define rom_001A98 ((const uint8_t *)rom(rom_001A98_ADDRESS))
#define rom_001AC8_ADDRESS 0x001AC8u
#define rom_001AC8 ((const uint32_t *)rom(rom_001AC8_ADDRESS))
#define rom_001CEC_ADDRESS 0x001CECu
#define rom_001CEC ((const uint32_t *)rom(rom_001CEC_ADDRESS))
#define rom_001DE0_ADDRESS 0x001DE0u
#define rom_001DE0 ((const uint32_t *)rom(rom_001DE0_ADDRESS))
#define rom_001ED0_ADDRESS 0x001ED0u
#define rom_001ED0 ((const uint32_t *)rom(rom_001ED0_ADDRESS))
#define rom_002088_ADDRESS 0x002088u
#define rom_002088 ((const uint32_t *)rom(rom_002088_ADDRESS))
#define rom_0020C8_ADDRESS 0x0020C8u
#define rom_0020C8 ((const uint32_t *)rom(rom_0020C8_ADDRESS))
#define rom_00221C_ADDRESS 0x00221Cu
#define rom_00221C ((const uint32_t *)rom(rom_00221C_ADDRESS))
#define rom_002304_ADDRESS 0x002304u
#define rom_002304 ((const uint32_t *)rom(rom_002304_ADDRESS))
#define rom_002384_ADDRESS 0x002384u
#define rom_002384 ((const uint32_t *)rom(rom_002384_ADDRESS))
#define rom_0023D4_ADDRESS 0x0023D4u
#define rom_0023D4 ((const uint32_t *)rom(rom_0023D4_ADDRESS))
#define rom_002424_ADDRESS 0x002424u
#define rom_002424 ((const uint32_t *)rom(rom_002424_ADDRESS))
#define rom_002488_ADDRESS 0x002488u
#define rom_002488 ((const uint32_t *)rom(rom_002488_ADDRESS))
#define rom_002564_ADDRESS 0x002564u
#define rom_002564 ((const uint32_t *)rom(rom_002564_ADDRESS))
#define rom_002620_ADDRESS 0x002620u
#define rom_002620 ((const uint32_t *)rom(rom_002620_ADDRESS))
#define rom_00266C_ADDRESS 0x00266Cu
#define rom_00266C ((const uint32_t *)rom(rom_00266C_ADDRESS))
#define rom_0026B8_ADDRESS 0x0026B8u
#define rom_0026B8 ((const uint32_t *)rom(rom_0026B8_ADDRESS))
#define rom_00279C_ADDRESS 0x00279Cu
#define rom_00279C ((const uint32_t *)rom(rom_00279C_ADDRESS))
#define rom_00287C_ADDRESS 0x00287Cu
#define rom_00287C ((const uint32_t *)rom(rom_00287C_ADDRESS))
#define rom_002960_ADDRESS 0x002960u
#define rom_002960 ((const uint32_t *)rom(rom_002960_ADDRESS))
#define rom_0029A4_ADDRESS 0x0029A4u
#define rom_0029A4 ((const uint32_t *)rom(rom_0029A4_ADDRESS))
#define rom_002C18_ADDRESS 0x002C18u
#define rom_002C18 ((const uint8_t *)rom(rom_002C18_ADDRESS))
#define rom_002CA4_ADDRESS 0x002CA4u
#define rom_002CA4 ((const uint32_t *)rom(rom_002CA4_ADDRESS))
#define rom_002CAC_ADDRESS 0x002CACu
#define rom_002CAC ((const uint32_t *)rom(rom_002CAC_ADDRESS))
#define rom_002CB4_ADDRESS 0x002CB4u
#define rom_002CB4 ((const uint16_t *)rom(rom_002CB4_ADDRESS))
#define rom_002D00_ADDRESS 0x002D00u
#define rom_002D00 ((const uint8_t *)rom(rom_002D00_ADDRESS))
#define rom_002D20_ADDRESS 0x002D20u
#define rom_002D20 ((const uint8_t *)rom(rom_002D20_ADDRESS))
#define rom_002D2A_ADDRESS 0x002D2Au
#define rom_002D2A ((const uint16_t *)rom(rom_002D2A_ADDRESS))
#define rom_002D40_ADDRESS 0x002D40u
#define rom_002D40 ((const uint16_t *)rom(rom_002D40_ADDRESS))
#define rom_002D6C_ADDRESS 0x002D6Cu
#define rom_002D6C ((const uint8_t *)rom(rom_002D6C_ADDRESS))
#define rom_002D98_ADDRESS 0x002D98u
#define rom_002D98 ((const uint8_t *)rom(rom_002D98_ADDRESS))
#define rom_002D9C_ADDRESS 0x002D9Cu
#define rom_002D9C ((const uint16_t *)rom(rom_002D9C_ADDRESS))
#define rom_002DAC_ADDRESS 0x002DACu
#define rom_002DAC ((const uint16_t *)rom(rom_002DAC_ADDRESS))
#define rom_002DBC_ADDRESS 0x002DBCu
#define rom_002DBC ((const uint16_t *)rom(rom_002DBC_ADDRESS))
#define rom_002DCC_ADDRESS 0x002DCCu
#define rom_002DCC ((const uint16_t *)rom(rom_002DCC_ADDRESS))
#define rom_002DDC_ADDRESS 0x002DDCu
#define rom_002DDC ((const uint16_t *)rom(rom_002DDC_ADDRESS))
#define rom_002DEC_ADDRESS 0x002DECu
#define rom_002DEC ((const uint16_t *)rom(rom_002DEC_ADDRESS))
#define rom_002DFC_ADDRESS 0x002DFCu
#define rom_002DFC ((const uint16_t *)rom(rom_002DFC_ADDRESS))
#define rom_002E10_ADDRESS 0x002E10u
#define rom_002E10 ((const uint16_t *)rom(rom_002E10_ADDRESS))
#define rom_002E24_ADDRESS 0x002E24u
#define rom_002E24 ((const uint16_t *)rom(rom_002E24_ADDRESS))
#define rom_002E38_ADDRESS 0x002E38u
#define rom_002E38 ((const uint16_t *)rom(rom_002E38_ADDRESS))
#define rom_002E80_ADDRESS 0x002E80u
#define rom_002E80 ((const uint32_t *)rom(rom_002E80_ADDRESS))
#define rom_003463_ADDRESS 0x003463u
#define rom_003463 ((const uint8_t *)rom(rom_003463_ADDRESS))
#define rom_003465_ADDRESS 0x003465u
#define rom_003465 ((const uint8_t *)rom(rom_003465_ADDRESS))
#define rom_003467_ADDRESS 0x003467u
#define rom_003467 ((const uint8_t *)rom(rom_003467_ADDRESS))
#define rom_003540_ADDRESS 0x003540u
#define rom_003540 ((const uint32_t *)rom(rom_003540_ADDRESS))
#define rom_003E28_ADDRESS 0x003E28u
#define rom_003E28 ((const uint32_t *)rom(rom_003E28_ADDRESS))
#define rom_003F34_ADDRESS 0x003F34u
#define rom_003F34 ((const uint32_t *)rom(rom_003F34_ADDRESS))
#define rom_00403A_ADDRESS 0x00403Au
#define rom_00403A ((const uint16_t *)rom(rom_00403A_ADDRESS))
#define rom_00403E_ADDRESS 0x00403Eu
#define rom_00403E ((const uint16_t *)rom(rom_00403E_ADDRESS))
#define rom_004042_ADDRESS 0x004042u
#define rom_004042 ((const uint16_t *)rom(rom_004042_ADDRESS))
#define rom_004064_ADDRESS 0x004064u
#define rom_004064 ((const uint32_t *)rom(rom_004064_ADDRESS))
#define rom_00424C_ADDRESS 0x00424Cu
#define rom_00424C ((const uint32_t *)rom(rom_00424C_ADDRESS))
#define rom_004280_ADDRESS 0x004280u
#define rom_004280 ((const uint32_t *)rom(rom_004280_ADDRESS))
#define rom_00442C_ADDRESS 0x00442Cu
#define rom_00442C ((const uint32_t *)rom(rom_00442C_ADDRESS))
#define rom_004474_ADDRESS 0x004474u
#define rom_004474 ((const uint32_t *)rom(rom_004474_ADDRESS))
#define rom_004588_ADDRESS 0x004588u
#define rom_004588 ((const uint32_t *)rom(rom_004588_ADDRESS))
#define rom_00462C_ADDRESS 0x00462Cu
#define rom_00462C ((const uint16_t *)rom(rom_00462C_ADDRESS))
#define rom_004634_ADDRESS 0x004634u
#define rom_004634 ((const uint16_t *)rom(rom_004634_ADDRESS))
#define rom_004648_ADDRESS 0x004648u
#define rom_004648 ((const uint32_t *)rom(rom_004648_ADDRESS))
#define rom_0046C8_ADDRESS 0x0046C8u
#define rom_0046C8 ((const uint32_t *)rom(rom_0046C8_ADDRESS))
#define rom_00477C_ADDRESS 0x00477Cu
#define rom_00477C ((const uint32_t *)rom(rom_00477C_ADDRESS))
#define rom_0048B8_ADDRESS 0x0048B8u
#define rom_0048B8 ((const uint32_t *)rom(rom_0048B8_ADDRESS))
#define rom_0048C0_ADDRESS 0x0048C0u
#define rom_0048C0 ((const uint32_t *)rom(rom_0048C0_ADDRESS))
#define rom_0048C8_ADDRESS 0x0048C8u
#define rom_0048C8 ((const uint32_t *)rom(rom_0048C8_ADDRESS))
#define rom_0048D0_ADDRESS 0x0048D0u
#define rom_0048D0 ((const uint16_t *)rom(rom_0048D0_ADDRESS))
#define rom_0048D4_ADDRESS 0x0048D4u
#define rom_0048D4 ((const uint16_t *)rom(rom_0048D4_ADDRESS))
#define rom_0048D8_ADDRESS 0x0048D8u
#define rom_0048D8 ((const uint32_t *)rom(rom_0048D8_ADDRESS))
#define rom_0048E0_ADDRESS 0x0048E0u
#define rom_0048E0 ((const uint16_t *)rom(rom_0048E0_ADDRESS))
#define rom_0048F0_ADDRESS 0x0048F0u
#define rom_0048F0 ((const uint16_t *)rom(rom_0048F0_ADDRESS))
#define rom_004900_ADDRESS 0x004900u
#define rom_004900 ((const uint16_t *)rom(rom_004900_ADDRESS))
#define rom_00492A_ADDRESS 0x00492Au
#define rom_00492A ((const uint16_t *)rom(rom_00492A_ADDRESS))
#define rom_004954_ADDRESS 0x004954u
#define rom_004954 ((const uint16_t *)rom(rom_004954_ADDRESS))
#define rom_004958_ADDRESS 0x004958u
#define rom_004958 ((const uint16_t *)rom(rom_004958_ADDRESS))
#define rom_00495C_ADDRESS 0x00495Cu
#define rom_00495C ((const uint16_t *)rom(rom_00495C_ADDRESS))
#define rom_004C0A_ADDRESS 0x004C0Au
#define rom_004C0A ROM16(rom_004C0A_ADDRESS)
#define rom_005058_ADDRESS 0x005058u
#define rom_005058 ((const uint32_t *)rom(rom_005058_ADDRESS))
#define rom_0053EC_ADDRESS 0x0053ECu
#define rom_0053EC ((const uint32_t *)rom(rom_0053EC_ADDRESS))
#define rom_00548C_ADDRESS 0x00548Cu
#define rom_00548C ((const uint32_t *)rom(rom_00548C_ADDRESS))
#define rom_005534_ADDRESS 0x005534u
#define rom_005534 ((const uint32_t *)rom(rom_005534_ADDRESS))
#define rom_0056C0_ADDRESS 0x0056C0u
#define rom_0056C0 ((const uint32_t *)rom(rom_0056C0_ADDRESS))
#define rom_005770_ADDRESS 0x005770u
#define rom_005770 ((const uint32_t *)rom(rom_005770_ADDRESS))
#define rom_005890_ADDRESS 0x005890u
#define rom_005890 ((const uint32_t *)rom(rom_005890_ADDRESS))
#define rom_005914_ADDRESS 0x005914u
#define rom_005914 ((const uint32_t *)rom(rom_005914_ADDRESS))
#define rom_0059C4_ADDRESS 0x0059C4u
#define rom_0059C4 ((const uint32_t *)rom(rom_0059C4_ADDRESS))
#define rom_005A28_ADDRESS 0x005A28u
#define rom_005A28 ((const uint32_t *)rom(rom_005A28_ADDRESS))
#define rom_005B50_ADDRESS 0x005B50u
#define rom_005B50 ((const uint32_t *)rom(rom_005B50_ADDRESS))
#define rom_005BE4_ADDRESS 0x005BE4u
#define rom_005BE4 ((const uint32_t *)rom(rom_005BE4_ADDRESS))
#define rom_005D68_ADDRESS 0x005D68u
#define rom_005D68 ((const uint32_t *)rom(rom_005D68_ADDRESS))
#define rom_005E1C_ADDRESS 0x005E1Cu
#define rom_005E1C ((const uint32_t *)rom(rom_005E1C_ADDRESS))
#define rom_005EC0_ADDRESS 0x005EC0u
#define rom_005EC0 ((const uint32_t *)rom(rom_005EC0_ADDRESS))
#define rom_005F5C_ADDRESS 0x005F5Cu
#define rom_005F5C ((const uint32_t *)rom(rom_005F5C_ADDRESS))
#define rom_0062B0_ADDRESS 0x0062B0u
#define rom_0062B0 ((const uint32_t *)rom(rom_0062B0_ADDRESS))
#define rom_006464_ADDRESS 0x006464u
#define rom_006464 ((const uint32_t *)rom(rom_006464_ADDRESS))
#define rom_00661C_ADDRESS 0x00661Cu
#define rom_00661C ((const uint32_t *)rom(rom_00661C_ADDRESS))
#define rom_00665C_ADDRESS 0x00665Cu
#define rom_00665C ((const uint32_t *)rom(rom_00665C_ADDRESS))
#define rom_0067A9_ADDRESS 0x0067A9u
#define rom_0067A9 ((const uint8_t *)rom(rom_0067A9_ADDRESS))
#define rom_0069B0_ADDRESS 0x0069B0u
#define rom_0069B0 ((const uint8_t *)rom(rom_0069B0_ADDRESS))
#define rom_0069B8_ADDRESS 0x0069B8u
#define rom_0069B8 ROM16(rom_0069B8_ADDRESS)
#define rom_0069BA_ADDRESS 0x0069BAu
#define rom_0069BA ROM16(rom_0069BA_ADDRESS)
#define rom_0069D8_ADDRESS 0x0069D8u
#define rom_0069D8 ((const uint8_t *)rom(rom_0069D8_ADDRESS))
#define rom_006D9C_ADDRESS 0x006D9Cu
#define rom_006D9C ((const uint8_t *)rom(rom_006D9C_ADDRESS))
#define rom_006DA4_ADDRESS 0x006DA4u
#define rom_006DA4 ((const uint8_t *)rom(rom_006DA4_ADDRESS))
#define rom_006DDE_ADDRESS 0x006DDEu
#define rom_006DDE ((const uint16_t *)rom(rom_006DDE_ADDRESS))
#define rom_006DEE_ADDRESS 0x006DEEu
#define rom_006DEE ((const uint8_t *)rom(rom_006DEE_ADDRESS))
#define rom_006E30_ADDRESS 0x006E30u
#define rom_006E30 ((const uint32_t *)rom(rom_006E30_ADDRESS))
#define rom_006E44_ADDRESS 0x006E44u
#define rom_006E44 ((const uint32_t *)rom(rom_006E44_ADDRESS))
#define rom_006E78_ADDRESS 0x006E78u
#define rom_006E78 ((const uint8_t *)rom(rom_006E78_ADDRESS))
#define rom_006E80_ADDRESS 0x006E80u
#define rom_006E80 ((const uint16_t *)rom(rom_006E80_ADDRESS))
#define rom_006E90_ADDRESS 0x006E90u
#define rom_006E90 ((const uint16_t *)rom(rom_006E90_ADDRESS))
#define rom_006EA0_ADDRESS 0x006EA0u
#define rom_006EA0 ((const uint8_t *)rom(rom_006EA0_ADDRESS))
#define rom_006F18_ADDRESS 0x006F18u
#define rom_006F18 ((const uint32_t *)rom(rom_006F18_ADDRESS))
#define rom_007164_ADDRESS 0x007164u
#define rom_007164 ((const uint32_t *)rom(rom_007164_ADDRESS))
#define rom_00720C_ADDRESS 0x00720Cu
#define rom_00720C ((const uint32_t *)rom(rom_00720C_ADDRESS))
#define rom_007917_ADDRESS 0x007917u
#define rom_007917 ((const uint32_t *)rom(rom_007917_ADDRESS))
#define rom_007C44_ADDRESS 0x007C44u
#define rom_007C44 ((const uint32_t *)rom(rom_007C44_ADDRESS))
#define rom_007CAC_ADDRESS 0x007CACu
#define rom_007CAC ((const uint32_t *)rom(rom_007CAC_ADDRESS))
#define rom_007D28_ADDRESS 0x007D28u
#define rom_007D28 ((const uint32_t *)rom(rom_007D28_ADDRESS))
#define rom_008026_ADDRESS 0x008026u
#define rom_008026 ((const uint16_t *)rom(rom_008026_ADDRESS))
#define rom_008038_ADDRESS 0x008038u
#define rom_008038 ((const uint16_t *)rom(rom_008038_ADDRESS))
#define rom_00804A_ADDRESS 0x00804Au
#define rom_00804A ((const uint32_t *)rom(rom_00804A_ADDRESS))
#define rom_0090AC_ADDRESS 0x0090ACu
#define rom_0090AC ((const uint32_t *)rom(rom_0090AC_ADDRESS))
#define rom_0091CC_ADDRESS 0x0091CCu
#define rom_0091CC ((const uint32_t *)rom(rom_0091CC_ADDRESS))
#define rom_00926C_ADDRESS 0x00926Cu
#define rom_00926C ((const uint16_t *)rom(rom_00926C_ADDRESS))
#define rom_009300_ADDRESS 0x009300u
#define rom_009300 ((const uint16_t *)rom(rom_009300_ADDRESS))
#define rom_009310_ADDRESS 0x009310u
#define rom_009310 ((const uint16_t *)rom(rom_009310_ADDRESS))
#define rom_009410_ADDRESS 0x009410u
#define rom_009410 ((const uint16_t *)rom(rom_009410_ADDRESS))
#define rom_009510_ADDRESS 0x009510u
#define rom_009510 ((const uint16_t *)rom(rom_009510_ADDRESS))
#define rom_009520_ADDRESS 0x009520u
#define rom_009520 ((const uint16_t *)rom(rom_009520_ADDRESS))
#define rom_009530_ADDRESS 0x009530u
#define rom_009530 ((const uint8_t *)rom(rom_009530_ADDRESS))
#define rom_009540_ADDRESS 0x009540u
#define rom_009540 ((const uint32_t *)rom(rom_009540_ADDRESS))
#define rom_009DCC_ADDRESS 0x009DCCu
#define rom_009DCC ((const uint16_t *)rom(rom_009DCC_ADDRESS))
#define rom_009DDC_ADDRESS 0x009DDCu
#define rom_009DDC ((const uint16_t *)rom(rom_009DDC_ADDRESS))
#define rom_009DEC_ADDRESS 0x009DECu
#define rom_009DEC ((const uint16_t *)rom(rom_009DEC_ADDRESS))
#define rom_009DFC_ADDRESS 0x009DFCu
#define rom_009DFC ((const uint16_t *)rom(rom_009DFC_ADDRESS))
#define rom_009E0C_ADDRESS 0x009E0Cu
#define rom_009E0C ((const uint8_t *)rom(rom_009E0C_ADDRESS))
#define rom_009E14_ADDRESS 0x009E14u
#define rom_009E14 ((const uint8_t *)rom(rom_009E14_ADDRESS))
#define rom_009E1C_ADDRESS 0x009E1Cu
#define rom_009E1C ((const uint16_t *)rom(rom_009E1C_ADDRESS))
#define rom_009E2C_ADDRESS 0x009E2Cu
#define rom_009E2C ((const uint16_t *)rom(rom_009E2C_ADDRESS))
#define rom_009E3C_ADDRESS 0x009E3Cu
#define rom_009E3C ((const uint8_t *)rom(rom_009E3C_ADDRESS))
#define rom_009E44_ADDRESS 0x009E44u
#define rom_009E44 ((const uint8_t *)rom(rom_009E44_ADDRESS))
#define rom_009E4C_ADDRESS 0x009E4Cu
#define rom_009E4C ((const uint8_t *)rom(rom_009E4C_ADDRESS))
#define rom_009EBC_ADDRESS 0x009EBCu
#define rom_009EBC ((const uint32_t *)rom(rom_009EBC_ADDRESS))
#define rom_009EDC_ADDRESS 0x009EDCu
#define rom_009EDC ((const uint32_t *)rom(rom_009EDC_ADDRESS))
#define rom_009F0C_ADDRESS 0x009F0Cu
#define rom_009F0C ((const uint16_t *)rom(rom_009F0C_ADDRESS))
#define rom_009F6C_ADDRESS 0x009F6Cu
#define rom_009F6C ((const uint8_t *)rom(rom_009F6C_ADDRESS))
#define rom_009F7C_ADDRESS 0x009F7Cu
#define rom_009F7C ((const uint16_t *)rom(rom_009F7C_ADDRESS))
#define rom_009F8C_ADDRESS 0x009F8Cu
#define rom_009F8C ((const uint16_t *)rom(rom_009F8C_ADDRESS))
#define rom_009F9C_ADDRESS 0x009F9Cu
#define rom_009F9C ((const uint8_t *)rom(rom_009F9C_ADDRESS))
#define rom_009FA4_ADDRESS 0x009FA4u
#define rom_009FA4 ((const uint8_t *)rom(rom_009FA4_ADDRESS))
#define rom_009FC4_ADDRESS 0x009FC4u
#define rom_009FC4 ((const uint32_t *)rom(rom_009FC4_ADDRESS))
#define rom_00A5F4_ADDRESS 0x00A5F4u
#define rom_00A5F4 ((const uint32_t *)rom(rom_00A5F4_ADDRESS))
#define rom_00A798_ADDRESS 0x00A798u
#define rom_00A798 ((const uint32_t *)rom(rom_00A798_ADDRESS))
#define rom_00ABC3_ADDRESS 0x00ABC3u
#define rom_00ABC3 ((const uint8_t *)rom(rom_00ABC3_ADDRESS))
#define rom_00ABCB_ADDRESS 0x00ABCBu
#define rom_00ABCB ((const uint8_t *)rom(rom_00ABCB_ADDRESS))
#define rom_00ACCB_ADDRESS 0x00ACCBu
#define rom_00ACCB ((const uint8_t *)rom(rom_00ACCB_ADDRESS))
#define rom_00ADCC_ADDRESS 0x00ADCCu
#define rom_00ADCC ((const uint32_t *)rom(rom_00ADCC_ADDRESS))
#define rom_00B04C_ADDRESS 0x00B04Cu
#define rom_00B04C ((const uint32_t *)rom(rom_00B04C_ADDRESS))
#define rom_00B0C0_ADDRESS 0x00B0C0u
#define rom_00B0C0 ((const uint32_t *)rom(rom_00B0C0_ADDRESS))
#define rom_00B6FC_ADDRESS 0x00B6FCu
#define rom_00B6FC ((const uint8_t *)rom(rom_00B6FC_ADDRESS))
#define rom_00B704_ADDRESS 0x00B704u
#define rom_00B704 ((const uint8_t *)rom(rom_00B704_ADDRESS))
#define rom_00B70C_ADDRESS 0x00B70Cu
#define rom_00B70C ((const uint32_t *)rom(rom_00B70C_ADDRESS))
#define rom_00B74C_ADDRESS 0x00B74Cu
#define rom_00B74C ((const uint32_t *)rom(rom_00B74C_ADDRESS))
#define rom_00BFCC_ADDRESS 0x00BFCCu
#define rom_00BFCC ((const uint32_t *)rom(rom_00BFCC_ADDRESS))
#define rom_00C024_ADDRESS 0x00C024u
#define rom_00C024 ((const uint32_t *)rom(rom_00C024_ADDRESS))
#define rom_00C0D4_ADDRESS 0x00C0D4u
#define rom_00C0D4 ((const uint32_t *)rom(rom_00C0D4_ADDRESS))
#define rom_00C118_ADDRESS 0x00C118u
#define rom_00C118 ((const uint32_t *)rom(rom_00C118_ADDRESS))
#define rom_00C23C_ADDRESS 0x00C23Cu
#define rom_00C23C ((const uint32_t *)rom(rom_00C23C_ADDRESS))
#define rom_00C2B8_ADDRESS 0x00C2B8u
#define rom_00C2B8 ((const uint32_t *)rom(rom_00C2B8_ADDRESS))
#define rom_00C350_ADDRESS 0x00C350u
#define rom_00C350 ((const uint32_t *)rom(rom_00C350_ADDRESS))
#define rom_00C44C_ADDRESS 0x00C44Cu
#define rom_00C44C ((const uint32_t *)rom(rom_00C44C_ADDRESS))
#define rom_00C5A8_ADDRESS 0x00C5A8u
#define rom_00C5A8 ((const uint32_t *)rom(rom_00C5A8_ADDRESS))
#define rom_00C66C_ADDRESS 0x00C66Cu
#define rom_00C66C ((const uint32_t *)rom(rom_00C66C_ADDRESS))
#define rom_00C880_ADDRESS 0x00C880u
#define rom_00C880 ((const uint32_t *)rom(rom_00C880_ADDRESS))
#define rom_00C8C4_ADDRESS 0x00C8C4u
#define rom_00C8C4 ((const uint32_t *)rom(rom_00C8C4_ADDRESS))
#define rom_00CAC8_ADDRESS 0x00CAC8u
#define rom_00CAC8 ((const uint32_t *)rom(rom_00CAC8_ADDRESS))
#define rom_00CBCC_ADDRESS 0x00CBCCu
#define rom_00CBCC ((const uint32_t *)rom(rom_00CBCC_ADDRESS))
#define rom_00CC70_ADDRESS 0x00CC70u
#define rom_00CC70 ((const uint32_t *)rom(rom_00CC70_ADDRESS))
#define rom_00CFC8_ADDRESS 0x00CFC8u
#define rom_00CFC8 ((const uint32_t *)rom(rom_00CFC8_ADDRESS))
#define rom_00CFE8_ADDRESS 0x00CFE8u
#define rom_00CFE8 ((const uint16_t *)rom(rom_00CFE8_ADDRESS))
#define rom_00CFF8_ADDRESS 0x00CFF8u
#define rom_00CFF8 ((const uint32_t *)rom(rom_00CFF8_ADDRESS))
#define rom_00D018_ADDRESS 0x00D018u
#define rom_00D018 ((const uint16_t *)rom(rom_00D018_ADDRESS))
#define rom_00D034_ADDRESS 0x00D034u
#define rom_00D034 ((const uint32_t *)rom(rom_00D034_ADDRESS))
#define rom_00D0C0_ADDRESS 0x00D0C0u
#define rom_00D0C0 ((const uint8_t *)rom(rom_00D0C0_ADDRESS))
#define rom_00DDB4_ADDRESS 0x00DDB4u
#define rom_00DDB4 ((const uint32_t *)rom(rom_00DDB4_ADDRESS))
#define race_copy_offsets_ADDRESS 0x00E0A0u
#define race_copy_offsets ((const uint32_t *)rom(race_copy_offsets_ADDRESS))
#define rom_00E0C4_ADDRESS 0x00E0C4u
#define rom_00E0C4 ((const uint32_t *)rom(rom_00E0C4_ADDRESS))
#define rom_00E4D0_ADDRESS 0x00E4D0u
#define rom_00E4D0 ((const uint32_t *)rom(rom_00E4D0_ADDRESS))
#define rom_00E5B3_ADDRESS 0x00E5B3u
#define rom_00E5B3 ((const uint8_t *)rom(rom_00E5B3_ADDRESS))
#define rom_00E5D3_ADDRESS 0x00E5D3u
#define rom_00E5D3 ((const uint8_t *)rom(rom_00E5D3_ADDRESS))
#define rom_00E614_ADDRESS 0x00E614u
#define rom_00E614 ((const uint16_t *)rom(rom_00E614_ADDRESS))
#define rom_00EB44_ADDRESS 0x00EB44u
#define rom_00EB44 ((const uint32_t *)rom(rom_00EB44_ADDRESS))
#define rom_00F13C_ADDRESS 0x00F13Cu
#define rom_00F13C ((const uint32_t *)rom(rom_00F13C_ADDRESS))
#define rom_00F140_ADDRESS 0x00F140u
#define rom_00F140 ((const uint32_t *)rom(rom_00F140_ADDRESS))
#define rom_00F34C_ADDRESS 0x00F34Cu
#define rom_00F34C ((const uint32_t *)rom(rom_00F34C_ADDRESS))
#define rom_00F4EC_ADDRESS 0x00F4ECu
#define rom_00F4EC ((const uint32_t *)rom(rom_00F4EC_ADDRESS))
#define rom_00F508_ADDRESS 0x00F508u
#define rom_00F508 ((const uint32_t *)rom(rom_00F508_ADDRESS))
#define rom_00FF40_ADDRESS 0x00FF40u
#define rom_00FF40 ((const uint32_t *)rom(rom_00FF40_ADDRESS))
#define rom_010B0A_ADDRESS 0x010B0Au
#define rom_010B0A ROM8(rom_010B0A_ADDRESS)
#define rom_010B0B_ADDRESS 0x010B0Bu
#define rom_010B0B ROM8(rom_010B0B_ADDRESS)
#define rom_010B10_ADDRESS 0x010B10u
#define rom_010B10 ROM16(rom_010B10_ADDRESS)
#define rom_010BA0_ADDRESS 0x010BA0u
#define rom_010BA0 ((const uint32_t *)rom(rom_010BA0_ADDRESS))
#define rom_010BC8_ADDRESS 0x010BC8u
#define rom_010BC8 ROM32(rom_010BC8_ADDRESS)
#define rom_010BCC_ADDRESS 0x010BCCu
#define rom_010BCC ROM16(rom_010BCC_ADDRESS)
#define rom_010BCE_ADDRESS 0x010BCEu
#define rom_010BCE ROM16(rom_010BCE_ADDRESS)
#define rom_010BD0_ADDRESS 0x010BD0u
#define rom_010BD0 ROM16(rom_010BD0_ADDRESS)
#define rom_010BD2_ADDRESS 0x010BD2u
#define rom_010BD2 ((const uint16_t *)rom(rom_010BD2_ADDRESS))
#define rom_01168C_ADDRESS 0x01168Cu
#define rom_01168C ((const uint32_t *)rom(rom_01168C_ADDRESS))
#define rom_011FFC_ADDRESS 0x011FFCu
#define rom_011FFC ((const uint32_t *)rom(rom_011FFC_ADDRESS))
#define rom_012026_ADDRESS 0x012026u
#define rom_012026 ((const uint8_t *)rom(rom_012026_ADDRESS))
#define rom_01202E_ADDRESS 0x01202Eu
#define rom_01202E ((const uint32_t *)rom(rom_01202E_ADDRESS))
#define rom_01204E_ADDRESS 0x01204Eu
#define rom_01204E ((const uint32_t *)rom(rom_01204E_ADDRESS))
#define rom_0121D8_ADDRESS 0x0121D8u
#define rom_0121D8 ((const uint16_t *)rom(rom_0121D8_ADDRESS))
#define rom_0121E8_ADDRESS 0x0121E8u
#define rom_0121E8 ((const uint16_t *)rom(rom_0121E8_ADDRESS))
#define rom_012708_ADDRESS 0x012708u
#define rom_012708 ((const uint32_t *)rom(rom_012708_ADDRESS))
#define rom_012748_ADDRESS 0x012748u
#define rom_012748 ((const uint32_t *)rom(rom_012748_ADDRESS))
#define rom_0127FC_ADDRESS 0x0127FCu
#define rom_0127FC ((const uint32_t *)rom(rom_0127FC_ADDRESS))
#define rom_012888_ADDRESS 0x012888u
#define rom_012888 ((const uint32_t *)rom(rom_012888_ADDRESS))
#define rom_012914_ADDRESS 0x012914u
#define rom_012914 ((const uint32_t *)rom(rom_012914_ADDRESS))
#define rom_0129B8_ADDRESS 0x0129B8u
#define rom_0129B8 ((const uint32_t *)rom(rom_0129B8_ADDRESS))
#define rom_012A00_ADDRESS 0x012A00u
#define rom_012A00 ((const uint32_t *)rom(rom_012A00_ADDRESS))
#define rom_012A2C_ADDRESS 0x012A2Cu
#define rom_012A2C ((const uint32_t *)rom(rom_012A2C_ADDRESS))
#define rom_012ADC_ADDRESS 0x012ADCu
#define rom_012ADC ((const uint32_t *)rom(rom_012ADC_ADDRESS))
#define rom_012B00_ADDRESS 0x012B00u
#define rom_012B00 ((const uint32_t *)rom(rom_012B00_ADDRESS))
#define rom_012B90_ADDRESS 0x012B90u
#define rom_012B90 ((const uint32_t *)rom(rom_012B90_ADDRESS))
#define rom_012BBC_ADDRESS 0x012BBCu
#define rom_012BBC ((const uint32_t *)rom(rom_012BBC_ADDRESS))
#define rom_012CB8_ADDRESS 0x012CB8u
#define rom_012CB8 ((const uint32_t *)rom(rom_012CB8_ADDRESS))
#define rom_012DC4_ADDRESS 0x012DC4u
#define rom_012DC4 ((const uint32_t *)rom(rom_012DC4_ADDRESS))
#define rom_012EB0_ADDRESS 0x012EB0u
#define rom_012EB0 ((const uint32_t *)rom(rom_012EB0_ADDRESS))
#define rom_012FC8_ADDRESS 0x012FC8u
#define rom_012FC8 ((const uint32_t *)rom(rom_012FC8_ADDRESS))
#define rom_01306E_ADDRESS 0x01306Eu
#define rom_01306E ((const uint16_t *)rom(rom_01306E_ADDRESS))
#define rom_01308C_ADDRESS 0x01308Cu
#define rom_01308C ((const uint32_t *)rom(rom_01308C_ADDRESS))
#define rom_013110_ADDRESS 0x013110u
#define rom_013110 ((const uint32_t *)rom(rom_013110_ADDRESS))
#define rom_0131B0_ADDRESS 0x0131B0u
#define rom_0131B0 ((const uint32_t *)rom(rom_0131B0_ADDRESS))
#define rom_013238_ADDRESS 0x013238u
#define rom_013238 ((const uint32_t *)rom(rom_013238_ADDRESS))
#define rom_0132A4_ADDRESS 0x0132A4u
#define rom_0132A4 ((const uint32_t *)rom(rom_0132A4_ADDRESS))
#define rom_013348_ADDRESS 0x013348u
#define rom_013348 ((const uint32_t *)rom(rom_013348_ADDRESS))
#define rom_01345C_ADDRESS 0x01345Cu
#define rom_01345C ((const uint32_t *)rom(rom_01345C_ADDRESS))
#define rom_01352C_ADDRESS 0x01352Cu
#define rom_01352C ((const uint32_t *)rom(rom_01352C_ADDRESS))
#define rom_0135D8_ADDRESS 0x0135D8u
#define rom_0135D8 ((const uint32_t *)rom(rom_0135D8_ADDRESS))
#define rom_0136A8_ADDRESS 0x0136A8u
#define rom_0136A8 ((const uint32_t *)rom(rom_0136A8_ADDRESS))
#define rom_013778_ADDRESS 0x013778u
#define rom_013778 ((const uint32_t *)rom(rom_013778_ADDRESS))
#define rom_0138F8_ADDRESS 0x0138F8u
#define rom_0138F8 ((const uint32_t *)rom(rom_0138F8_ADDRESS))
#define rom_013B04_ADDRESS 0x013B04u
#define rom_013B04 ((const uint32_t *)rom(rom_013B04_ADDRESS))
#define rom_013BB0_ADDRESS 0x013BB0u
#define rom_013BB0 ((const uint32_t *)rom(rom_013BB0_ADDRESS))
#define rom_013D45_ADDRESS 0x013D45u
#define rom_013D45 ((const uint8_t *)rom(rom_013D45_ADDRESS))
#define rom_013D94_ADDRESS 0x013D94u
#define rom_013D94 ((const uint32_t *)rom(rom_013D94_ADDRESS))
#define rom_013DF0_ADDRESS 0x013DF0u
#define rom_013DF0 ((const uint32_t *)rom(rom_013DF0_ADDRESS))
#define rom_0140C8_ADDRESS 0x0140C8u
#define rom_0140C8 ((const uint16_t *)rom(rom_0140C8_ADDRESS))
#define rom_014114_ADDRESS 0x014114u
#define rom_014114 ((const uint32_t *)rom(rom_014114_ADDRESS))
#define rom_014200_ADDRESS 0x014200u
#define rom_014200 ((const uint32_t *)rom(rom_014200_ADDRESS))
#define rom_014360_ADDRESS 0x014360u
#define rom_014360 ((const uint16_t *)rom(rom_014360_ADDRESS))
#define rom_014380_ADDRESS 0x014380u
#define rom_014380 ((const uint16_t *)rom(rom_014380_ADDRESS))
#define rom_0143A0_ADDRESS 0x0143A0u
#define rom_0143A0 ((const uint8_t *)rom(rom_0143A0_ADDRESS))
#define rom_0143A6_ADDRESS 0x0143A6u
#define rom_0143A6 ((const uint8_t *)rom(rom_0143A6_ADDRESS))
#define rom_01440E_ADDRESS 0x01440Eu
#define rom_01440E ((const uint16_t *)rom(rom_01440E_ADDRESS))
#define rom_01442E_ADDRESS 0x01442Eu
#define rom_01442E ((const uint16_t *)rom(rom_01442E_ADDRESS))
#define rom_01444E_ADDRESS 0x01444Eu
#define rom_01444E ((const uint16_t *)rom(rom_01444E_ADDRESS))
#define rom_01447C_ADDRESS 0x01447Cu
#define rom_01447C ((const uint32_t *)rom(rom_01447C_ADDRESS))
#define rom_0144A9_ADDRESS 0x0144A9u
#define rom_0144A9 ((const uint32_t *)rom(rom_0144A9_ADDRESS))
#define rom_0144EC_ADDRESS 0x0144ECu
#define rom_0144EC ((const uint32_t *)rom(rom_0144EC_ADDRESS))
#define rom_014520_ADDRESS 0x014520u
#define rom_014520 ((const uint32_t *)rom(rom_014520_ADDRESS))
#define rom_0145A0_ADDRESS 0x0145A0u
#define rom_0145A0 ((const uint32_t *)rom(rom_0145A0_ADDRESS))
#define rom_014660_ADDRESS 0x014660u
#define rom_014660 ROM8(rom_014660_ADDRESS)
#define rom_014674_ADDRESS 0x014674u
#define rom_014674 ((const uint16_t *)rom(rom_014674_ADDRESS))
#define rom_014684_ADDRESS 0x014684u
#define rom_014684 ((const uint16_t *)rom(rom_014684_ADDRESS))
#define rom_0146A0_ADDRESS 0x0146A0u
#define rom_0146A0 ((const uint32_t *)rom(rom_0146A0_ADDRESS))
#define rom_014994_ADDRESS 0x014994u
#define rom_014994 ((const uint32_t *)rom(rom_014994_ADDRESS))
#define rom_014A16_ADDRESS 0x014A16u
#define rom_014A16 ((const uint8_t *)rom(rom_014A16_ADDRESS))
#define object_lists_14A20_ADDRESS 0x014A20u
#define object_lists_14A20 ((const uint32_t *)rom(object_lists_14A20_ADDRESS))
#define object_list_14A60_ADDRESS 0x014A60u
#define object_list_14A60 ROM32(object_list_14A60_ADDRESS)
#define rom_016B0C_ADDRESS 0x016B0Cu
#define rom_016B0C ((const uint32_t *)rom(rom_016B0C_ADDRESS))
#define rom_016BBC_ADDRESS 0x016BBCu
#define rom_016BBC ((const uint32_t *)rom(rom_016BBC_ADDRESS))
#define rom_0171E0_ADDRESS 0x0171E0u
#define rom_0171E0 ((const uint32_t *)rom(rom_0171E0_ADDRESS))
#define rom_017530_ADDRESS 0x017530u
#define rom_017530 ((const uint32_t *)rom(rom_017530_ADDRESS))
#define rom_017738_ADDRESS 0x017738u
#define rom_017738 ((const uint32_t *)rom(rom_017738_ADDRESS))
#define rom_017904_ADDRESS 0x017904u
#define rom_017904 ((const uint32_t *)rom(rom_017904_ADDRESS))
#define rom_01790C_ADDRESS 0x01790Cu
#define rom_01790C ((const uint16_t *)rom(rom_01790C_ADDRESS))
#define rom_017988_ADDRESS 0x017988u
#define rom_017988 ((const uint32_t *)rom(rom_017988_ADDRESS))
#define rom_0179A0_ADDRESS 0x0179A0u
#define rom_0179A0 ((const uint16_t *)rom(rom_0179A0_ADDRESS))
#define rom_017A78_ADDRESS 0x017A78u
#define rom_017A78 ((const uint32_t *)rom(rom_017A78_ADDRESS))
#define rom_017AE0_ADDRESS 0x017AE0u
#define rom_017AE0 ((const uint32_t *)rom(rom_017AE0_ADDRESS))
#define rom_017B60_ADDRESS 0x017B60u
#define rom_017B60 ((const uint8_t *)rom(rom_017B60_ADDRESS))
#define rom_017B7F_ADDRESS 0x017B7Fu
#define rom_017B7F ((const uint8_t *)rom(rom_017B7F_ADDRESS))
#define rom_017BA9_ADDRESS 0x017BA9u
#define rom_017BA9 ((const uint8_t *)rom(rom_017BA9_ADDRESS))
#define rom_017C08_ADDRESS 0x017C08u
#define rom_017C08 ((const uint32_t *)rom(rom_017C08_ADDRESS))
#define rom_017C68_ADDRESS 0x017C68u
#define rom_017C68 ((const uint32_t *)rom(rom_017C68_ADDRESS))
#define rom_017CB8_ADDRESS 0x017CB8u
#define rom_017CB8 ((const uint32_t *)rom(rom_017CB8_ADDRESS))
#define rom_017DE4_ADDRESS 0x017DE4u
#define rom_017DE4 ((const uint32_t *)rom(rom_017DE4_ADDRESS))
#define rom_017E98_ADDRESS 0x017E98u
#define rom_017E98 ((const uint32_t *)rom(rom_017E98_ADDRESS))
#define rom_017F7C_ADDRESS 0x017F7Cu
#define rom_017F7C ((const uint32_t *)rom(rom_017F7C_ADDRESS))
#define rom_018094_ADDRESS 0x018094u
#define rom_018094 ((const uint32_t *)rom(rom_018094_ADDRESS))
#define rom_01814C_ADDRESS 0x01814Cu
#define rom_01814C ((const uint32_t *)rom(rom_01814C_ADDRESS))
#define object_lists_181B8_ADDRESS 0x0181B8u
#define object_lists_181B8 ((const uint32_t *)rom(object_lists_181B8_ADDRESS))
#define rom_018A70_ADDRESS 0x018A70u
#define rom_018A70 ((const uint32_t *)rom(rom_018A70_ADDRESS))
#define rom_018AE8_ADDRESS 0x018AE8u
#define rom_018AE8 ((const uint16_t *)rom(rom_018AE8_ADDRESS))
#define rom_018B21_ADDRESS 0x018B21u
#define rom_018B21 ((const uint8_t *)rom(rom_018B21_ADDRESS))
#define rom_018B90_ADDRESS 0x018B90u
#define rom_018B90 ROM32(rom_018B90_ADDRESS)
#define rom_018B94_ADDRESS 0x018B94u
#define rom_018B94 ROM32(rom_018B94_ADDRESS)
#define rom_018B98_ADDRESS 0x018B98u
#define rom_018B98 ROM32(rom_018B98_ADDRESS)
#define rom_018B9C_ADDRESS 0x018B9Cu
#define rom_018B9C ROM32(rom_018B9C_ADDRESS)
#define rom_018BD4_ADDRESS 0x018BD4u
#define rom_018BD4 ((const uint32_t *)rom(rom_018BD4_ADDRESS))
#define rom_018C64_ADDRESS 0x018C64u
#define rom_018C64 ((const uint32_t *)rom(rom_018C64_ADDRESS))
#define rom_018C84_ADDRESS 0x018C84u
#define rom_018C84 ((const uint32_t *)rom(rom_018C84_ADDRESS))
#define rom_018CF0_ADDRESS 0x018CF0u
#define rom_018CF0 ((const uint32_t *)rom(rom_018CF0_ADDRESS))
#define rom_018D5C_ADDRESS 0x018D5Cu
#define rom_018D5C ((const uint32_t *)rom(rom_018D5C_ADDRESS))
#define rom_018D88_ADDRESS 0x018D88u
#define rom_018D88 ((const uint32_t *)rom(rom_018D88_ADDRESS))
#define rom_018DB4_ADDRESS 0x018DB4u
#define rom_018DB4 ((const uint32_t *)rom(rom_018DB4_ADDRESS))
#define rom_018E40_ADDRESS 0x018E40u
#define rom_018E40 ((const uint32_t *)rom(rom_018E40_ADDRESS))
#define rom_018EA0_ADDRESS 0x018EA0u
#define rom_018EA0 ((const uint32_t *)rom(rom_018EA0_ADDRESS))
#define rom_019044_ADDRESS 0x019044u
#define rom_019044 ((const uint32_t *)rom(rom_019044_ADDRESS))
#define rom_0191E0_ADDRESS 0x0191E0u
#define rom_0191E0 ((const uint32_t *)rom(rom_0191E0_ADDRESS))
#define rom_019330_ADDRESS 0x019330u
#define rom_019330 ((const uint32_t *)rom(rom_019330_ADDRESS))
#define rom_019421_ADDRESS 0x019421u
#define rom_019421 ((const uint8_t *)rom(rom_019421_ADDRESS))
#define rom_01942C_ADDRESS 0x01942Cu
#define rom_01942C ((const uint32_t *)rom(rom_01942C_ADDRESS))
#define rom_019458_ADDRESS 0x019458u
#define rom_019458 ((const uint8_t *)rom(rom_019458_ADDRESS))
#define rom_019561_ADDRESS 0x019561u
#define rom_019561 ((const uint16_t *)rom(rom_019561_ADDRESS))
#define rom_019571_ADDRESS 0x019571u
#define rom_019571 ((const uint8_t *)rom(rom_019571_ADDRESS))
#define dj_messages_ADDRESS 0x019576u
#define dj_messages ((const uint32_t *)rom(dj_messages_ADDRESS))
#define routine_lists_1966C_ADDRESS 0x01966Cu
#define routine_lists_1966C ((const uint32_t *)rom(routine_lists_1966C_ADDRESS))
#define rom_019CB8_ADDRESS 0x019CB8u
#define rom_019CB8 ((const uint32_t *)rom(rom_019CB8_ADDRESS))
#define rom_019E78_ADDRESS 0x019E78u
#define rom_019E78 ((const uint32_t *)rom(rom_019E78_ADDRESS))
#define rom_019F58_ADDRESS 0x019F58u
#define rom_019F58 ((const uint32_t *)rom(rom_019F58_ADDRESS))
#define rom_019F6C_ADDRESS 0x019F6Cu
#define rom_019F6C ((const uint32_t *)rom(rom_019F6C_ADDRESS))
#define rom_01A012_ADDRESS 0x01A012u
#define rom_01A012 ((const uint16_t *)rom(rom_01A012_ADDRESS))
#define rom_01A022_ADDRESS 0x01A022u
#define rom_01A022 ((const uint16_t *)rom(rom_01A022_ADDRESS))
#define rom_01A032_ADDRESS 0x01A032u
#define rom_01A032 ((const uint16_t *)rom(rom_01A032_ADDRESS))
#define rom_01A042_ADDRESS 0x01A042u
#define rom_01A042 ((const uint16_t *)rom(rom_01A042_ADDRESS))
#define rom_01A052_ADDRESS 0x01A052u
#define rom_01A052 ((const uint16_t *)rom(rom_01A052_ADDRESS))
#define rom_01A062_ADDRESS 0x01A062u
#define rom_01A062 ((const uint16_t *)rom(rom_01A062_ADDRESS))
#define rom_01A072_ADDRESS 0x01A072u
#define rom_01A072 ((const uint16_t *)rom(rom_01A072_ADDRESS))
#define rom_01A082_ADDRESS 0x01A082u
#define rom_01A082 ((const uint16_t *)rom(rom_01A082_ADDRESS))
#define rom_01A092_ADDRESS 0x01A092u
#define rom_01A092 ((const uint16_t *)rom(rom_01A092_ADDRESS))
#define rom_01A0F0_ADDRESS 0x01A0F0u
#define rom_01A0F0 ((const uint32_t *)rom(rom_01A0F0_ADDRESS))
#define rom_01A198_ADDRESS 0x01A198u
#define rom_01A198 ((const uint32_t *)rom(rom_01A198_ADDRESS))
#define rom_01A2A8_ADDRESS 0x01A2A8u
#define rom_01A2A8 ((const uint32_t *)rom(rom_01A2A8_ADDRESS))
#define rom_01A334_ADDRESS 0x01A334u
#define rom_01A334 ((const uint32_t *)rom(rom_01A334_ADDRESS))
#define rom_01A3F4_ADDRESS 0x01A3F4u
#define rom_01A3F4 ((const uint32_t *)rom(rom_01A3F4_ADDRESS))
#define rom_01A4A4_ADDRESS 0x01A4A4u
#define rom_01A4A4 ((const uint32_t *)rom(rom_01A4A4_ADDRESS))
#define rom_01A544_ADDRESS 0x01A544u
#define rom_01A544 ((const uint32_t *)rom(rom_01A544_ADDRESS))
#define rom_01A680_ADDRESS 0x01A680u
#define rom_01A680 ((const uint32_t *)rom(rom_01A680_ADDRESS))
#define rom_01A6DC_ADDRESS 0x01A6DCu
#define rom_01A6DC ((const uint32_t *)rom(rom_01A6DC_ADDRESS))
#define rom_01A744_ADDRESS 0x01A744u
#define rom_01A744 ((const uint32_t *)rom(rom_01A744_ADDRESS))
#define rom_01A7D0_ADDRESS 0x01A7D0u
#define rom_01A7D0 ((const uint32_t *)rom(rom_01A7D0_ADDRESS))
#define rom_01A84C_ADDRESS 0x01A84Cu
#define rom_01A84C ((const uint32_t *)rom(rom_01A84C_ADDRESS))
#define rom_01A8F0_ADDRESS 0x01A8F0u
#define rom_01A8F0 ((const uint32_t *)rom(rom_01A8F0_ADDRESS))
#define rom_01A986_ADDRESS 0x01A986u
#define rom_01A986 ((const uint16_t *)rom(rom_01A986_ADDRESS))
#define rom_01A996_ADDRESS 0x01A996u
#define rom_01A996 ((const uint16_t *)rom(rom_01A996_ADDRESS))
#define rom_01A9A6_ADDRESS 0x01A9A6u
#define rom_01A9A6 ((const uint16_t *)rom(rom_01A9A6_ADDRESS))
#define rom_01A9AC_ADDRESS 0x01A9ACu
#define rom_01A9AC ((const uint16_t *)rom(rom_01A9AC_ADDRESS))
#define rom_01A9BC_ADDRESS 0x01A9BCu
#define rom_01A9BC ((const uint32_t *)rom(rom_01A9BC_ADDRESS))
#define rom_01AA0C_ADDRESS 0x01AA0Cu
#define rom_01AA0C ((const uint32_t *)rom(rom_01AA0C_ADDRESS))
#define rom_01AA4C_ADDRESS 0x01AA4Cu
#define rom_01AA4C ((const uint32_t *)rom(rom_01AA4C_ADDRESS))
#define rom_01AB8E_ADDRESS 0x01AB8Eu
#define rom_01AB8E ((const uint8_t *)rom(rom_01AB8E_ADDRESS))
#define rom_01AB98_ADDRESS 0x01AB98u
#define rom_01AB98 ((const uint16_t *)rom(rom_01AB98_ADDRESS))
#define rom_01AB9E_ADDRESS 0x01AB9Eu
#define rom_01AB9E ((const uint16_t *)rom(rom_01AB9E_ADDRESS))
#define rom_01ABA4_ADDRESS 0x01ABA4u
#define rom_01ABA4 ((const uint16_t *)rom(rom_01ABA4_ADDRESS))
#define rom_01AC34_ADDRESS 0x01AC34u
#define rom_01AC34 ((const uint16_t *)rom(rom_01AC34_ADDRESS))
#define rom_01AC44_ADDRESS 0x01AC44u
#define rom_01AC44 ((const uint16_t *)rom(rom_01AC44_ADDRESS))
#define rom_01ADF4_ADDRESS 0x01ADF4u
#define rom_01ADF4 ((const uint32_t *)rom(rom_01ADF4_ADDRESS))
#define rom_01AE98_ADDRESS 0x01AE98u
#define rom_01AE98 ((const uint32_t *)rom(rom_01AE98_ADDRESS))
#define rom_01AFE8_ADDRESS 0x01AFE8u
#define rom_01AFE8 ((const uint32_t *)rom(rom_01AFE8_ADDRESS))
#define rom_01B254_ADDRESS 0x01B254u
#define rom_01B254 ((const uint32_t *)rom(rom_01B254_ADDRESS))
#define rom_01C378_ADDRESS 0x01C378u
#define rom_01C378 ROM32(rom_01C378_ADDRESS)
#define rom_01C37C_ADDRESS 0x01C37Cu
#define rom_01C37C ROM32(rom_01C37C_ADDRESS)
#define rom_01C380_ADDRESS 0x01C380u
#define rom_01C380 ROM32(rom_01C380_ADDRESS)
#define rom_01C384_ADDRESS 0x01C384u
#define rom_01C384 ROM32(rom_01C384_ADDRESS)
#define rom_01C45C_ADDRESS 0x01C45Cu
#define rom_01C45C ((const uint32_t *)rom(rom_01C45C_ADDRESS))
#define rom_01C560_ADDRESS 0x01C560u
#define rom_01C560 ((const uint32_t *)rom(rom_01C560_ADDRESS))
#define rom_01C588_ADDRESS 0x01C588u
#define rom_01C588 ((const uint16_t *)rom(rom_01C588_ADDRESS))
#define rom_01CF9C_ADDRESS 0x01CF9Cu
#define rom_01CF9C ((const uint16_t *)rom(rom_01CF9C_ADDRESS))
#define rom_01CF9E_ADDRESS 0x01CF9Eu
#define rom_01CF9E ROM16(rom_01CF9E_ADDRESS)
#define rom_01D3C0_ADDRESS 0x01D3C0u
#define rom_01D3C0 ((const uint32_t *)rom(rom_01D3C0_ADDRESS))
#define rom_01D5AA_ADDRESS 0x01D5AAu
#define rom_01D5AA ((const uint16_t *)rom(rom_01D5AA_ADDRESS))
#define rom_01D5BE_ADDRESS 0x01D5BEu
#define rom_01D5BE ROM16(rom_01D5BE_ADDRESS)
#define rom_01D5EC_ADDRESS 0x01D5ECu
#define rom_01D5EC ((const uint32_t *)rom(rom_01D5EC_ADDRESS))
#define rom_01D618_ADDRESS 0x01D618u
#define rom_01D618 ((const uint32_t *)rom(rom_01D618_ADDRESS))
#define rom_01D77C_ADDRESS 0x01D77Cu
#define rom_01D77C ((const uint32_t *)rom(rom_01D77C_ADDRESS))
#define rom_01D7C0_ADDRESS 0x01D7C0u
#define rom_01D7C0 ((const uint16_t *)rom(rom_01D7C0_ADDRESS))
#define rom_01D7C4_ADDRESS 0x01D7C4u
#define rom_01D7C4 ROM16(rom_01D7C4_ADDRESS)
#define rom_01DBD0_ADDRESS 0x01DBD0u
#define rom_01DBD0 ((const uint32_t *)rom(rom_01DBD0_ADDRESS))
#define rom_01DCCE_ADDRESS 0x01DCCEu
#define rom_01DCCE ((const uint16_t *)rom(rom_01DCCE_ADDRESS))
#define rom_01DCE0_ADDRESS 0x01DCE0u
#define rom_01DCE0 ROM16(rom_01DCE0_ADDRESS)
#define rom_01DCE4_ADDRESS 0x01DCE4u
#define rom_01DCE4 ((const uint32_t *)rom(rom_01DCE4_ADDRESS))
#define rom_01DCEC_ADDRESS 0x01DCECu
#define rom_01DCEC ((const uint32_t *)rom(rom_01DCEC_ADDRESS))
#define rom_01E0D8_ADDRESS 0x01E0D8u
#define rom_01E0D8 ((const uint32_t *)rom(rom_01E0D8_ADDRESS))
#define rom_01E0E4_ADDRESS 0x01E0E4u
#define rom_01E0E4 ((const uint32_t *)rom(rom_01E0E4_ADDRESS))
#define rom_01E200_ADDRESS 0x01E200u
#define rom_01E200 ((const uint16_t *)rom(rom_01E200_ADDRESS))
#define rom_01E206_ADDRESS 0x01E206u
#define rom_01E206 ((const uint16_t *)rom(rom_01E206_ADDRESS))
#define rom_01E20C_ADDRESS 0x01E20Cu
#define rom_01E20C ROM16(rom_01E20C_ADDRESS)
#define rom_01E220_ADDRESS 0x01E220u
#define rom_01E220 ((const uint32_t *)rom(rom_01E220_ADDRESS))
#define rom_01E61C_ADDRESS 0x01E61Cu
#define rom_01E61C ((const uint32_t *)rom(rom_01E61C_ADDRESS))
#define rom_01E67C_ADDRESS 0x01E67Cu
#define rom_01E67C ((const uint16_t *)rom(rom_01E67C_ADDRESS))
#define rom_01E682_ADDRESS 0x01E682u
#define rom_01E682 ROM16(rom_01E682_ADDRESS)
#define rom_01E684_ADDRESS 0x01E684u
#define rom_01E684 ((const uint32_t *)rom(rom_01E684_ADDRESS))
#define rom_01E68C_ADDRESS 0x01E68Cu
#define rom_01E68C ((const uint32_t *)rom(rom_01E68C_ADDRESS))
#define rom_01EEAE_ADDRESS 0x01EEAEu
#define rom_01EEAE ((const uint16_t *)rom(rom_01EEAE_ADDRESS))
#define rom_01EEB0_ADDRESS 0x01EEB0u
#define rom_01EEB0 ROM16(rom_01EEB0_ADDRESS)
#define rom_01F282_ADDRESS 0x01F282u
#define rom_01F282 ROM16(rom_01F282_ADDRESS)
#define rom_01F302_ADDRESS 0x01F302u
#define rom_01F302 ROM16(rom_01F302_ADDRESS)
#define rom_01F504_ADDRESS 0x01F504u
#define rom_01F504 ROM16(rom_01F504_ADDRESS)
#define road_object_streams_ADDRESS 0x020800u
#define road_object_streams ((const uint32_t *)rom(road_object_streams_ADDRESS))
#define object_lists_2E1F0_ADDRESS 0x02E1F0u
#define object_lists_2E1F0 ((const uint32_t *)rom(object_lists_2E1F0_ADDRESS))
#define rom_0318CC_ADDRESS 0x0318CCu
#define rom_0318CC ((const uint32_t *)rom(rom_0318CC_ADDRESS))
#define rom_0319F0_ADDRESS 0x0319F0u
#define rom_0319F0 ((const uint32_t *)rom(rom_0319F0_ADDRESS))
#define rom_031B14_ADDRESS 0x031B14u
#define rom_031B14 ((const uint32_t *)rom(rom_031B14_ADDRESS))
#define rom_031C38_ADDRESS 0x031C38u
#define rom_031C38 ((const uint32_t *)rom(rom_031C38_ADDRESS))
#define rom_031CB8_ADDRESS 0x031CB8u
#define rom_031CB8 ((const uint32_t *)rom(rom_031CB8_ADDRESS))
#define rom_031DDC_ADDRESS 0x031DDCu
#define rom_031DDC ((const uint32_t *)rom(rom_031DDC_ADDRESS))
#define rom_031E7C_ADDRESS 0x031E7Cu
#define rom_031E7C ROM32(rom_031E7C_ADDRESS)
#define rom_031E80_ADDRESS 0x031E80u
#define rom_031E80 ROM32(rom_031E80_ADDRESS)
#define rom_031E84_ADDRESS 0x031E84u
#define rom_031E84 ROM32(rom_031E84_ADDRESS)
#define rom_031E88_ADDRESS 0x031E88u
#define rom_031E88 ROM32(rom_031E88_ADDRESS)
#define rom_031E8C_ADDRESS 0x031E8Cu
#define rom_031E8C ROM32(rom_031E8C_ADDRESS)
#define rom_032BA4_ADDRESS 0x032BA4u
#define rom_032BA4 ((const uint32_t *)rom(rom_032BA4_ADDRESS))
#define rom_034138_ADDRESS 0x034138u
#define rom_034138 ((const uint32_t *)rom(rom_034138_ADDRESS))
#define rom_034308_ADDRESS 0x034308u
#define rom_034308 ROM32(rom_034308_ADDRESS)
#define rom_03CC00_ADDRESS 0x03CC00u
#define rom_03CC00 ((const uint32_t *)rom(rom_03CC00_ADDRESS))
#define rom_03CC80_ADDRESS 0x03CC80u
#define rom_03CC80 ((const uint8_t *)rom(rom_03CC80_ADDRESS))
#define rom_03F82A_ADDRESS 0x03F82Au
#define rom_03F82A ROM16(rom_03F82A_ADDRESS)
#define rom_03F82C_ADDRESS 0x03F82Cu
#define rom_03F82C ROM16(rom_03F82C_ADDRESS)
#define rom_03F85C_ADDRESS 0x03F85Cu
#define rom_03F85C ((const uint32_t *)rom(rom_03F85C_ADDRESS))
#define rom_03FAC4_ADDRESS 0x03FAC4u
#define rom_03FAC4 ((const uint32_t *)rom(rom_03FAC4_ADDRESS))
#define rom_03FACC_ADDRESS 0x03FACCu
#define rom_03FACC ((const uint32_t *)rom(rom_03FACC_ADDRESS))
#define rom_03FFF8_ADDRESS 0x03FFF8u
#define rom_03FFF8 ROM16(rom_03FFF8_ADDRESS)
#define rom_03FFFA_ADDRESS 0x03FFFAu
#define rom_03FFFA ROM16(rom_03FFFA_ADDRESS)
#define rom_03FFFC_ADDRESS 0x03FFFCu
#define rom_03FFFC ROM16(rom_03FFFC_ADDRESS)
#define rom_03FFFE_ADDRESS 0x03FFFEu
#define rom_03FFFE ROM16(rom_03FFFE_ADDRESS)
#define rom_100000_ADDRESS 0x100000u
#define rom_100000 ROM32(rom_100000_ADDRESS)
#define rom_100C00_ADDRESS 0x100C00u
#define rom_100C00 ((const uint16_t *)rom(rom_100C00_ADDRESS))
#define rom_101400_ADDRESS 0x101400u
#define rom_101400 ((const uint16_t *)rom(rom_101400_ADDRESS))
#define rom_103400_ADDRESS 0x103400u
#define rom_103400 ((const uint16_t *)rom(rom_103400_ADDRESS))
#define rom_109694_ADDRESS 0x109694u
#define rom_109694 ((const uint32_t *)rom(rom_109694_ADDRESS))
#define rom_122B5C_ADDRESS 0x122B5Cu
#define rom_122B5C ROM32(rom_122B5C_ADDRESS)
#define rom_122B60_ADDRESS 0x122B60u
#define rom_122B60 ROM32(rom_122B60_ADDRESS)
#define rom_122B64_ADDRESS 0x122B64u
#define rom_122B64 ROM16(rom_122B64_ADDRESS)
#define rom_122B66_ADDRESS 0x122B66u
#define rom_122B66 ROM16(rom_122B66_ADDRESS)
#define rom_1598CC_ADDRESS 0x1598CCu
#define rom_1598CC ((const uint32_t *)rom(rom_1598CC_ADDRESS))
#define rom_159AFC_ADDRESS 0x159AFCu
#define rom_159AFC ((const uint32_t *)rom(rom_159AFC_ADDRESS))
#define rom_18D25C_ADDRESS 0x18D25Cu
#define rom_18D25C ((const uint32_t *)rom(rom_18D25C_ADDRESS))
#define rom_19A340_ADDRESS 0x19A340u
#define rom_19A340 ((const uint16_t *)rom(rom_19A340_ADDRESS))
#define rom_1A0340_ADDRESS 0x1A0340u
#define rom_1A0340 ((const uint16_t *)rom(rom_1A0340_ADDRESS))
#define rom_1A6340_ADDRESS 0x1A6340u
#define rom_1A6340 ((const uint32_t *)rom(rom_1A6340_ADDRESS))

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
#define object_handlers_F70_ADDRESS 0x000F74u
#define unk_00183B_ADDRESS 0x00183Bu
#define object_handlers_1A78_ADDRESS 0x001A7Cu
#define object_handlers_2D58_ADDRESS 0x002D5Cu
#define object_handlers_346C_ADDRESS 0x003470u
#define object_handlers_3474_ADDRESS 0x003478u
#define object_handlers_34C4_ADDRESS 0x0034C8u
#define object_handlers_34EC_ADDRESS 0x0034F0u
#define unk_009EFC_ADDRESS 0x009EFCu
#define unk_009FE4_ADDRESS 0x009FE4u
#define unk_00AB04_ADDRESS 0x00AB04u
#define object_handlers_DF74_ADDRESS 0x00DF78u
#define unk_010309_ADDRESS 0x010309u
#define unk_010425_ADDRESS 0x010425u
#define timeline_1AFC0_ADDRESS 0x01AFC0u
#define unk_01B248_ADDRESS 0x01B248u
#define unk_030B60_ADDRESS 0x030B60u
#define vector_table_ADDRESS 0x03F000u

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
			uint16_t unk_02;
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
			uint32_t unk_14;
			uint32_t unk_18;
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
			uint32_t unk_80;
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
			uint8_t name;
			uint8_t pad_A1[0x3];
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
	uint8_t unk_0C;
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
	uint8_t unk_5A;
	uint8_t unk_5B;
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

#pragma pack(pop)

_Static_assert(sizeof(struct object) == 0x100, "struct object has the size of the game's blocks");
_Static_assert(sizeof(struct car) == 0x200, "struct car has the size of the game's blocks");
_Static_assert(sizeof(struct screen) == 0x8000, "struct screen has the size of the game's blocks");
_Static_assert(sizeof(struct object_area) == 0x8000, "struct object_area has the size of the game's blocks");
_Static_assert(sizeof(struct race) == 0x80, "struct race has the size of the game's blocks");
_Static_assert(sizeof(struct object_list) == 0x100, "struct object_list has the size of the game's blocks");

#define OBJECT(address) ((struct object *)ram(address))
#define CAR(address) ((struct car *)ram(address))
#define SCREEN(address) ((struct screen *)ram(address))
#define OBJECT_AREA(address) ((struct object_area *)ram(address))
#define RACE(address) ((struct race *)ram(address))
#define OBJECT_LIST(address) ((struct object_list *)ram(address))

#endif
