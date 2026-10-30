/**
 *    Name:        sound.h
 *    Description: the sound program's ports, variables, tables, structures and functions
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#ifndef ORUNNERS_SOUND_H
#define ORUNNERS_SOUND_H

#include "sound/support.h"

/* the board's I/O ports the program uses */
#define FM_ADDRESS 0x80u
#define FM_DATA 0x81u
#define FM_PART2_ADDRESS 0x82u
#define FM_PART2_DATA 0x83u
#define ROM_BANK 0xA0u
#define PCM_BANKS 0xB0u
#define IRQ_CONTROL 0xC0u
#define IRQ_ACKNOWLEDGE 0xC1u
#define IRQ_VECTOR0_INPUT 0xD0u
#define IRQ_VECTOR1_INPUT 0xD1u
#define IRQ_VECTOR2_INPUT 0xD2u
#define IRQ_MASK 0xD3u
#define IRQ_CONTROL_D8 0xD8u

/* the sound program's variables, in the RAM it shares with the V70 (E000-FFFF) */
#define shared_ram_start_ADDRESS 0xE000u
#define shared_ram_start SOUND_RAM8(shared_ram_start_ADDRESS)
#define screen_1_midi_read_ADDRESS 0xE002u
#define screen_1_midi_read SOUND_RAM8(screen_1_midi_read_ADDRESS)
#define screen_1_midi_status_ADDRESS 0xE008u
#define screen_1_midi_status SOUND_RAM8(screen_1_midi_status_ADDRESS)
#define sample_or_patch_ADDRESS 0xE00Du
#define sample_or_patch SOUND_RAM8(sample_or_patch_ADDRESS)
#define drum_timeout_wait_ADDRESS 0xE010u
#define drum_timeout_wait SOUND_RAM8(drum_timeout_wait_ADDRESS)
#define note_code_ADDRESS 0xE012u
#define note_code SOUND_RAM8(note_code_ADDRESS)
#define note_level_ADDRESS 0xE013u
#define note_level SOUND_RAM8(note_level_ADDRESS)
#define pitch_bend_ADDRESS 0xE014u
#define pitch_bend SOUND_RAM16S(pitch_bend_ADDRESS)
#define drum_timeout_pass_ADDRESS 0xE017u
#define drum_timeout_pass SOUND_RAM8(drum_timeout_pass_ADDRESS)
#define drum_timeout_cursor_ADDRESS 0xE018u
#define drum_timeout_cursor SOUND_RAM16(drum_timeout_cursor_ADDRESS)
#define drum_timeout_voices_left_ADDRESS 0xE01Au
#define drum_timeout_voices_left SOUND_RAM8(drum_timeout_voices_left_ADDRESS)
#define voice_modulation_ADDRESS 0xE01Bu
#define voice_modulation SOUND_RAM8(voice_modulation_ADDRESS)
#define screen_1_music_volume_ADDRESS 0xE01Cu
#define screen_1_music_volume SOUND_RAM8(screen_1_music_volume_ADDRESS)
#define drum_pitch_ADDRESS 0xE01Du
#define drum_pitch SOUND_RAM8(drum_pitch_ADDRESS)
#define fm_channel_ADDRESS 0xE01Eu
#define fm_channel SOUND_RAM8(fm_channel_ADDRESS)
#define pcm_voices_used_ADDRESS 0xE01Fu
#define pcm_voices_used SOUND_RAM8(pcm_voices_used_ADDRESS)
#define next_voice_ADDRESS 0xE028u
#define next_voice SOUND_RAM16(next_voice_ADDRESS)
#define pcm_banks_ADDRESS 0xE02Au
#define pcm_banks SOUND_RAM8(pcm_banks_ADDRESS)
#define split_tune_ADDRESS 0xE02Bu
#define split_tune SOUND_RAM8(split_tune_ADDRESS)
#define split_octave_ADDRESS 0xE02Cu
#define split_octave SOUND_RAM8(split_octave_ADDRESS)
#define note_number_ADDRESS 0xE02Du
#define note_number SOUND_RAM8(note_number_ADDRESS)
#define screen_1_pitch_bends_ADDRESS 0xE040u
#define screen_1_pitch_bends SOUND_RAM16(screen_1_pitch_bends_ADDRESS)
#define screen_1_channel_controls_ADDRESS 0xE060u
#define screen_1_channel_controls ((uint8_t *)sound_ram_at(screen_1_channel_controls_ADDRESS))
#define screen_2_midi_read_ADDRESS 0xE082u
#define screen_2_midi_read SOUND_RAM8(screen_2_midi_read_ADDRESS)
#define screen_2_midi_status_ADDRESS 0xE088u
#define screen_2_midi_status SOUND_RAM8(screen_2_midi_status_ADDRESS)
#define screen_2_music_volume_ADDRESS 0xE09Cu
#define screen_2_music_volume SOUND_RAM8(screen_2_music_volume_ADDRESS)
#define screen_2_pitch_bends_ADDRESS 0xE0C0u
#define screen_2_pitch_bends SOUND_RAM16(screen_2_pitch_bends_ADDRESS)
#define screen_2_channel_controls_ADDRESS 0xE0E0u
#define screen_2_channel_controls ((uint8_t *)sound_ram_at(screen_2_channel_controls_ADDRESS))
#define screen_1_midi_queue_ADDRESS 0xE100u
#define screen_1_midi_queue SOUND_RAM8(screen_1_midi_queue_ADDRESS)
#define screen_2_midi_queue_ADDRESS 0xE200u
#define screen_2_midi_queue SOUND_RAM8(screen_2_midi_queue_ADDRESS)
#define voice_0_chip_channel_ADDRESS 0xE411u
#define voice_0_chip_channel SOUND_RAM8(voice_0_chip_channel_ADDRESS)
#define voice_0_note_ADDRESS 0xE412u
#define voice_0_note SOUND_RAM8(voice_0_note_ADDRESS)
#define voice_0_midi_channel_ADDRESS 0xE413u
#define voice_0_midi_channel SOUND_RAM8(voice_0_midi_channel_ADDRESS)
#define voice_0_drum_timeout_ADDRESS 0xE414u
#define voice_0_drum_timeout SOUND_RAM8(voice_0_drum_timeout_ADDRESS)
#define voice_0_allocation_order_ADDRESS 0xE416u
#define voice_0_allocation_order SOUND_RAM8(voice_0_allocation_order_ADDRESS)
#define screen_1_command_ADDRESS 0xE700u
#define screen_1_command SOUND_RAM8(screen_1_command_ADDRESS)
#define screen_1_engine_revs_ADDRESS 0xE710u
#define screen_1_engine_revs SOUND_RAM8(screen_1_engine_revs_ADDRESS)
#define screen_1_engine_state_ADDRESS 0xE711u
#define screen_1_engine_state SOUND_RAM8(screen_1_engine_state_ADDRESS)
#define screen_1_engine_requests_ADDRESS 0xE712u
#define screen_1_engine_requests SOUND_RAM8(screen_1_engine_requests_ADDRESS)
#define screen_1_engine_sample_ADDRESS 0xE713u
#define screen_1_engine_sample SOUND_RAM8(screen_1_engine_sample_ADDRESS)
#define screen_1_engine_loudness_ADDRESS 0xE714u
#define screen_1_engine_loudness SOUND_RAM8(screen_1_engine_loudness_ADDRESS)
#define screen_1_tyre_noise_ADDRESS 0xE718u
#define screen_1_tyre_noise SOUND_RAM8(screen_1_tyre_noise_ADDRESS)
#define screen_1_tyre_noise_sample_ADDRESS 0xE719u
#define screen_1_tyre_noise_sample SOUND_RAM8(screen_1_tyre_noise_sample_ADDRESS)
#define screen_1_tyre_noise_time_ADDRESS 0xE71Cu
#define screen_1_tyre_noise_time SOUND_RAM8(screen_1_tyre_noise_time_ADDRESS)
#define screen_1_tyre_noise_off_ADDRESS 0xE71Du
#define screen_1_tyre_noise_off SOUND_RAM8(screen_1_tyre_noise_off_ADDRESS)
#define screen_1_tick_countdown_ADDRESS 0xE740u
#define screen_1_tick_countdown SOUND_RAM8(screen_1_tick_countdown_ADDRESS)
#define screen_1_midi_write_ADDRESS 0xE758u
#define screen_1_midi_write SOUND_RAM8(screen_1_midi_write_ADDRESS)
#define screen_1_midi_count_ADDRESS 0xE759u
#define screen_1_midi_count SOUND_RAM8(screen_1_midi_count_ADDRESS)
#define engine_sweep_on_ADDRESS 0xE780u
#define engine_sweep_on SOUND_RAM8(engine_sweep_on_ADDRESS)
#define engine_sweep_wait_ADDRESS 0xE781u
#define engine_sweep_wait SOUND_RAM8(engine_sweep_wait_ADDRESS)
#define engine_sweep_position_low_ADDRESS 0xE784u
#define engine_sweep_position_low SOUND_RAM8(engine_sweep_position_low_ADDRESS)
#define engine_sweep_position_high_ADDRESS 0xE785u
#define engine_sweep_position_high SOUND_RAM8(engine_sweep_position_high_ADDRESS)
#define engine_sweep_down_ADDRESS 0xE786u
#define engine_sweep_down SOUND_RAM8(engine_sweep_down_ADDRESS)
#define screen_2_command_ADDRESS 0xE800u
#define screen_2_command SOUND_RAM8(screen_2_command_ADDRESS)
#define screen_2_engine_revs_ADDRESS 0xE810u
#define screen_2_engine_revs SOUND_RAM8(screen_2_engine_revs_ADDRESS)
#define screen_2_engine_state_ADDRESS 0xE811u
#define screen_2_engine_state SOUND_RAM8(screen_2_engine_state_ADDRESS)
#define screen_2_engine_requests_ADDRESS 0xE812u
#define screen_2_engine_requests SOUND_RAM8(screen_2_engine_requests_ADDRESS)
#define screen_2_engine_sample_ADDRESS 0xE813u
#define screen_2_engine_sample SOUND_RAM8(screen_2_engine_sample_ADDRESS)
#define screen_2_engine_loudness_ADDRESS 0xE814u
#define screen_2_engine_loudness SOUND_RAM8(screen_2_engine_loudness_ADDRESS)
#define screen_2_tyre_noise_ADDRESS 0xE818u
#define screen_2_tyre_noise SOUND_RAM8(screen_2_tyre_noise_ADDRESS)
#define screen_2_tyre_noise_sample_ADDRESS 0xE819u
#define screen_2_tyre_noise_sample SOUND_RAM8(screen_2_tyre_noise_sample_ADDRESS)
#define screen_2_tyre_noise_time_ADDRESS 0xE81Cu
#define screen_2_tyre_noise_time SOUND_RAM8(screen_2_tyre_noise_time_ADDRESS)
#define screen_2_tyre_noise_off_ADDRESS 0xE81Du
#define screen_2_tyre_noise_off SOUND_RAM8(screen_2_tyre_noise_off_ADDRESS)
#define screen_2_midi_write_ADDRESS 0xE858u
#define screen_2_midi_write SOUND_RAM8(screen_2_midi_write_ADDRESS)
#define screen_2_midi_count_ADDRESS 0xE859u
#define screen_2_midi_count SOUND_RAM8(screen_2_midi_count_ADDRESS)
#define screen_1_music_flags_ADDRESS 0xE900u
#define screen_1_music_flags SOUND_RAM8(screen_1_music_flags_ADDRESS)
#define screen_2_music_flags_ADDRESS 0xF100u
#define screen_2_music_flags SOUND_RAM8(screen_2_music_flags_ADDRESS)
#define sound_cpu_hold_ADDRESS 0xFFFFu
#define sound_cpu_hold SOUND_RAM8(sound_cpu_hold_ADDRESS)

/* data in the program ROM the code reads */
#define part_templates_ADDRESS 0x1847u
extern const void *part_templates_data;
#define part_templates (*(const uint16_t *)part_templates_data)
#define instruments_ADDRESS 0x1BC2u
extern const void *instruments_data;
#define instruments (*(const uint16_t *)instruments_data)
#define note_codes_ADDRESS 0x1F62u
extern const void *note_codes_data;
#define note_codes ((const uint8_t *)note_codes_data)
#define bend_range_tables_ADDRESS 0x2546u
extern const void *bend_range_tables_data;
#define bend_range_tables (*(const uint16_t *)bend_range_tables_data)
#define fm_patches_ADDRESS 0x2A5Eu
extern const void *fm_patches_data;
#define fm_patches (*(const uint16_t *)fm_patches_data)
#define drum_kits_ADDRESS 0x2DA5u
extern const void *drum_kits_data;
#define drum_kits (*(const uint16_t *)drum_kits_data)
#define engine_levels_ADDRESS 0x37F3u
extern const void *engine_levels_data;
#define engine_levels ((const uint8_t *)engine_levels_data)
#define engine_pitches_ADDRESS 0x38F3u
extern const void *engine_pitches_data;
#define engine_pitches (*(const uint16_t *)engine_pitches_data)
#define tyre_noises_ADDRESS 0x3DC6u
extern const void *tyre_noises_data;
#define tyre_noises (*(const uint8_t *)tyre_noises_data)
#define songs_ADDRESS 0x439Bu
extern const void *songs_data;
#define songs (*(const uint16_t *)songs_data)
#define sound_commands_ADDRESS 0x44DBu
extern const void *sound_commands_data;
#define sound_commands (*(const uint16_t *)sound_commands_data)

/* the banked page of the sound ROM and the MultiPCM (read and written through the bus) */
#define pcm_data 0xC000u
#define pcm_voice_select 0xC001u
#define pcm_register_select 0xC002u
#define MULTIPCM_C009 0xC009u

/* data in the program ROM sound_symbols.txt labels */
#define unused_restart_entries_ADDRESS 0x0004u
#define unused_nmi_entry_ADDRESS 0x005Fu
#define unused_music_demo_loop_ADDRESS 0x0068u
#define unused_demo_commands_ADDRESS 0x00ACu
#define unused_zeros_00CF_ADDRESS 0x00CFu
#define unused_return_072F_ADDRESS 0x072Fu
#define unused_midi_uart_ADDRESS 0x0A9Bu
#define unused_loop_0BA2_ADDRESS 0x0BA2u
#define unused_return_0F6B_ADDRESS 0x0F6Bu
#define unused_loop_1285_ADDRESS 0x1285u
#define unused_fm_fnum_pointers_ADDRESS 0x154Du
#define unused_pcm_pitch_pointers_ADDRESS 0x2096u
#define engine_sweep_revs_ADDRESS 0x3B5Du
#define playlist_command_table_ADDRESS 0x401Eu
#define unused_lowest_flags_player_ADDRESS 0x42B2u
#define unused_return_4382_ADDRESS 0x4382u
#define playlist_stop_music_2_ADDRESS 0x46DBu
#define playlist_magical_sound_shower_ADDRESS 0x46DFu
#define playlist_passing_breeze_ADDRESS 0x46FDu
#define unused_playlist_songs_1a_1e_ADDRESS 0x4712u
#define playlist_picture_the_rivers_ADDRESS 0x4720u
#define playlist_blow_your_cool_ADDRESS 0x4736u
#define playlist_looking_for_the_rainbow_ADDRESS 0x474Au
#define playlist_speed_king_ADDRESS 0x4757u
#define playlist_adventure_ADDRESS 0x476Au
#define playlist_staff_credits_ADDRESS 0x4786u
#define playlist_race_opening_ADDRESS 0x478Du
#define playlist_meaning_of_the_light_ADDRESS 0x4790u
#define playlist_music_0c_ADDRESS 0x4795u
#define playlist_jingle_bell_ADDRESS 0x4798u
#define playlist_stop_music_ADDRESS 0x479Fu
#define playlist_silence_screen_ADDRESS 0x47A3u
#define playlist_sonic_control_ADDRESS 0x47A7u
#define playlist_mega_driver_ADDRESS 0x47BEu
#define playlist_dream_flying_ADDRESS 0x47C4u
#define playlist_splash_wave_ADDRESS 0x47CBu
#define playlist_last_wave_ADDRESS 0x47DBu
#define unused_silence_playlists_ADDRESS 0x47E4u
#define playlist_station_fork_jingle_ADDRESS 0x4810u
#define playlist_station_stage_09_ADDRESS 0x4813u
#define playlist_station_stages_15_18_ADDRESS 0x481Bu
#define playlist_station_stages_16_17_ADDRESS 0x4823u
#define playlist_station_stage_11_ADDRESS 0x4829u
#define playlist_station_stages_13_20_ADDRESS 0x482Fu
#define playlist_station_stage_12_ADDRESS 0x4834u
#define playlist_station_stage_07_ADDRESS 0x4839u
#define playlist_station_stage_03_ADDRESS 0x483Eu
#define playlist_station_stage_06_ADDRESS 0x4843u
#define playlist_station_stage_10_ADDRESS 0x4849u
#define playlist_unplayed_station_eb_ADDRESS 0x484Eu
#define playlist_station_stage_01_ADDRESS 0x4853u
#define playlist_station_stage_05_ADDRESS 0x4858u
#define playlist_station_fork_ADDRESS 0x485Eu
#define playlist_station_stages_14_19_ADDRESS 0x4863u
#define playlist_station_stage_08_ADDRESS 0x4868u
#define playlist_station_stage_02_ADDRESS 0x486Eu
#define playlist_station_stage_04_ADDRESS 0x4876u
#define playlist_station_stage_00_ADDRESS 0x487Du
#define playlist_station_stages_21_30_ADDRESS 0x4884u
#define effect_20_ADDRESS 0x488Cu
#define effect_21_game_start_ADDRESS 0x4894u
#define effect_22_ADDRESS 0x489Cu
#define effect_23_stage_30_start_ADDRESS 0x48A4u
#define effect_24_checkpoint_leading_ADDRESS 0x48ACu
#define effect_25_ADDRESS 0x48B4u
#define effect_26_after_crash_ADDRESS 0x48BCu
#define effect_27_after_crash_ADDRESS 0x48C4u
#define effect_28_checkpoint_ADDRESS 0x48CCu
#define effect_29_ADDRESS 0x48D4u
#define effect_2a_ADDRESS 0x48DCu
#define effect_2b_rival_alarm_ADDRESS 0x48E4u
#define effect_2c_name_entry_letter_ADDRESS 0x48ECu
#define effect_2d_race_opening_ADDRESS 0x48F4u
#define effect_2e_race_opening_2_ADDRESS 0x48FCu
#define effect_2f_ADDRESS 0x4904u
#define effect_30_ADDRESS 0x490Cu
#define effect_31_crash_ADDRESS 0x4914u
#define effect_32_crash_hit_ADDRESS 0x491Cu
#define effect_33_crash_hit_ADDRESS 0x4924u
#define effect_34_crash_hit_ADDRESS 0x492Cu
#define effect_35_ADDRESS 0x4934u
#define effect_36_car_pass_ADDRESS 0x493Cu
#define effect_37_car_pass_ADDRESS 0x4944u
#define effect_38_car_pass_ADDRESS 0x494Cu
#define effect_39_car_pass_ADDRESS 0x4954u
#define effect_3a_car_pass_ADDRESS 0x495Cu
#define effect_3b_ADDRESS 0x4964u
#define effect_3c_ADDRESS 0x496Cu
#define dj_message_31_ADDRESS 0x4978u
#define dj_message_32_ADDRESS 0x4984u
#define dj_message_33_ADDRESS 0x4990u
#define dj_goal_first_ADDRESS 0x499Cu
#define dj_goal_ADDRESS 0x49A8u
#define dj_time_up_36_ADDRESS 0x49B4u
#define dj_time_up_37_ADDRESS 0x49C0u
#define effect_44_dragon_dance_ADDRESS 0x49CCu
#define effect_45_ADDRESS 0x49D4u
#define effect_46_matterhorn_crowd_ADDRESS 0x49DCu
#define effect_47_pandas_ADDRESS 0x49E4u
#define effect_48_ADDRESS 0x49ECu
#define effect_49_santa_sleigh_ADDRESS 0x49F4u
#define effect_4a_cheering_crowd_ADDRESS 0x49FCu
#define effect_4b_ADDRESS 0x4A04u
#define effect_4c_ADDRESS 0x4A0Cu
#define effect_4d_ADDRESS 0x4A14u
#define effect_4e_ADDRESS 0x4A1Cu
#define effect_4f_ending_ADDRESS 0x4A24u
#define effect_50_ADDRESS 0x4A2Cu
#define effect_51_jungle_ADDRESS 0x4A34u
#define effect_52_ADDRESS 0x4A3Cu
#define effect_53_stage_end_ADDRESS 0x4A44u
#define effect_54_ADDRESS 0x4A4Cu
#define effect_55_ADDRESS 0x4A54u
#define effect_56_ADDRESS 0x4A5Cu
#define effect_57_ADDRESS 0x4A64u
#define dj_waiting_ADDRESS 0x4A6Cu
#define effect_59_ADDRESS 0x4A74u
#define effect_5a_ADDRESS 0x4A7Cu
#define effect_5b_ADDRESS 0x4A84u
#define effect_5c_ADDRESS 0x4A8Cu
#define effect_5d_ADDRESS 0x4A94u
#define dj_message_38_ADDRESS 0x4A9Cu
#define dj_message_39_ADDRESS 0x4AA4u
#define effect_60_car_select_model_4_ADDRESS 0x4AACu
#define effect_61_car_select_model_2_ADDRESS 0x4AB8u
#define effect_62_car_select_model_0_ADDRESS 0x4AC4u
#define effect_63_car_select_model_6_ADDRESS 0x4AD0u
#define effect_64_car_select_model_5_ADDRESS 0x4ADCu
#define effect_65_car_select_model_3_ADDRESS 0x4AE8u
#define effect_66_car_select_model_1_ADDRESS 0x4AF4u
#define effect_67_car_select_model_7_ADDRESS 0x4B00u
#define effect_68_ADDRESS 0x4B0Cu
#define effect_69_ADDRESS 0x4B14u
#define effect_6a_ADDRESS 0x4B1Cu
#define effect_6b_ADDRESS 0x4B24u
#define effect_6c_ADDRESS 0x4B2Cu
#define effect_6d_ADDRESS 0x4B34u
#define effect_6e_ADDRESS 0x4B3Cu
#define effect_6f_ADDRESS 0x4B44u
#define effect_70_ADDRESS 0x4B4Cu
#define effect_71_ADDRESS 0x4B54u
#define dj_message_3a_ADDRESS 0x4B5Cu
#define effect_73_ADDRESS 0x4B68u
#define dj_car_model_4_ADDRESS 0x4B70u
#define dj_car_model_2_ADDRESS 0x4B78u
#define dj_car_model_0_ADDRESS 0x4B80u
#define dj_car_model_6_ADDRESS 0x4B88u
#define dj_car_model_5_ADDRESS 0x4B90u
#define dj_car_model_3_ADDRESS 0x4B98u
#define dj_car_model_1_ADDRESS 0x4BA0u
#define dj_car_model_7_ADDRESS 0x4BA8u
#define effect_7c_ADDRESS 0x4BB0u
#define effect_7d_ADDRESS 0x4BB8u
#define effect_7e_ADDRESS 0x4BC0u
#define effect_7f_ADDRESS 0x4BC8u
#define effect_80_ADDRESS 0x4BD0u
#define effect_81_ADDRESS 0x4BD8u
#define dj_station_ADDRESS 0x4BE0u
#define dj_cars_introduced_ADDRESS 0x4BE8u
#define effect_84_ADDRESS 0x4BF0u
#define effect_85_ADDRESS 0x4BF8u
#define dj_remark_2b_ADDRESS 0x4C00u
#define effect_87_ADDRESS 0x4C08u
#define dj_remark_2c_ADDRESS 0x4C10u
#define effect_89_ADDRESS 0x4C18u
#define dj_remark_2d_ADDRESS 0x4C20u
#define dj_checkpoint_final_ADDRESS 0x4C28u
#define dj_remark_2f_ADDRESS 0x4C30u
#define dj_checkpoint_ADDRESS 0x4C38u
#define effect_8e_ADDRESS 0x4C40u
#define effect_8f_ADDRESS 0x4C48u
#define select_sound_set_0_ADDRESS 0x4C50u
#define select_sound_set_1_ADDRESS 0x4C58u
#define select_sound_set_2_ADDRESS 0x4C60u
#define select_sound_set_3_ADDRESS 0x4C68u
#define select_sound_set_4_ADDRESS 0x4C70u
#define select_sound_set_5_ADDRESS 0x4C78u
#define select_sound_set_6_ADDRESS 0x4C80u
#define select_sound_set_7_ADDRESS 0x4C88u
#define select_sound_set_8_ADDRESS 0x4C90u
#define select_sound_set_9_ADDRESS 0x4C98u
#define select_sound_set_a_ADDRESS 0x4CA0u
#define select_sound_set_b_ADDRESS 0x4CA8u
#define channel_a_notes_off_ADDRESS 0x4CB0u
#define channel_a_full_volume_ADDRESS 0x4CB8u
#define stop_all_voices_7d_ADDRESS 0x4CC0u
#define stop_all_voices_7f_ADDRESS 0x4CC8u
#define effect_b2_ADDRESS 0x4CD0u
#define effect_b3_ADDRESS 0x4D9Au
#define effect_b6_ADDRESS 0x4EBCu
#define effect_b7_ADDRESS 0x4FDEu
#define unused_effect_program_7d_ADDRESS 0x5100u
#define unused_effect_program_7e_ADDRESS 0x5222u
#define unused_effect_program_7f_ADDRESS 0x5344u
#define dj_message_21_ADDRESS 0x5466u
#define dj_message_22_ADDRESS 0x5479u
#define dj_message_23_ADDRESS 0x548Cu
#define dj_message_24_ADDRESS 0x549Fu
#define dj_message_25_ADDRESS 0x54B2u
#define dj_message_26_ADDRESS 0x54C5u
#define dj_message_27_ADDRESS 0x54D8u
#define dj_message_28_ADDRESS 0x54EBu
#define dj_car_number_1_ADDRESS 0x54FEu
#define dj_car_number_2_ADDRESS 0x550Cu
#define dj_car_number_3_ADDRESS 0x551Au
#define dj_car_number_4_ADDRESS 0x5528u
#define dj_car_number_5_ADDRESS 0x5536u
#define dj_car_number_6_ADDRESS 0x5544u
#define dj_car_number_7_ADDRESS 0x5552u
#define dj_car_number_8_ADDRESS 0x5560u
#define dj_leader_car_1_ADDRESS 0x556Eu
#define dj_leader_car_2_ADDRESS 0x5581u
#define dj_leader_car_3_ADDRESS 0x5594u
#define dj_leader_car_4_ADDRESS 0x55A7u
#define dj_leader_car_5_ADDRESS 0x55BAu
#define dj_leader_car_6_ADDRESS 0x55CDu
#define dj_leader_car_7_ADDRESS 0x55E0u
#define dj_leader_car_8_ADDRESS 0x55F3u
#define dj_own_car_1_ADDRESS 0x5606u
#define dj_own_car_2_ADDRESS 0x561Eu
#define dj_own_car_3_ADDRESS 0x5636u
#define dj_own_car_4_ADDRESS 0x564Eu
#define dj_own_car_5_ADDRESS 0x5666u
#define dj_own_car_6_ADDRESS 0x567Eu
#define dj_own_car_7_ADDRESS 0x5696u
#define dj_own_car_8_ADDRESS 0x56AEu
#define dj_station_hello_ADDRESS 0x56C6u
#define dj_entry_ADDRESS 0x56D9u
#define song_records_ADDRESS 0x56F1u
#define stop_music_sequence_ADDRESS 0x595Du
#define stop_screen_voices_sequence_ADDRESS 0x596Eu
#define unused_fill_005974_ADDRESS 0x5974u

/* the sound program's structures in the shared RAM */
#pragma pack(push, 1)
struct midi_part
{
	uint8_t flags;
	uint8_t midi_channel;
	uint8_t volume;
	uint8_t program;
	uint8_t transpose;
	uint8_t pad_05[0x1];
	uint8_t lfo_frequency;
	uint8_t pad_07[0x1];
	uint8_t effective_volume;
	uint8_t bend_range;
};

struct voice
{
	uint8_t flags;
	uint8_t chip_channel;
	uint8_t note;
	uint8_t midi_channel;
	uint8_t drum_timeout;
	uint8_t level;
	uint8_t allocation_order;
	uint8_t tune;
	uint8_t octave_shift;
};

struct sequence_player
{
	uint8_t flags;
	uint8_t pad_01[0x1];
	union
	{
		uint16_t position;
		uint8_t position_low;
		struct
		{
			uint8_t pad_02[0x1];
			uint8_t position_high;
		};
	};
	uint8_t pad_04[0x2];
	uint8_t delta_time_high;
	uint8_t delta_time_low;
	uint8_t pad_08[0x2];
	uint8_t meta_51_value;
	uint8_t pad_0B[0x9];
	uint8_t running_status;
	uint8_t fade_volume;
	uint8_t pad_16[0x2];
	uint8_t fade_speed;
	uint8_t fade_wait;
	uint8_t fade_channel_volume;
	uint8_t bank;
	uint8_t pad_1C[0x4];
	union
	{
		uint16_t playlist;
		uint8_t playlist_low;
		struct
		{
			uint8_t pad_20[0x1];
			uint8_t playlist_high;
		};
	};
	uint8_t pad_22[0xDE];
};

struct screen_page
{
	uint8_t command;
	uint8_t pad_01[0x8];
	uint8_t playlist_choice_1;
	uint8_t playlist_choice_2;
	uint8_t player_end_flag;
	uint8_t pad_0C[0x4];
	uint8_t engine_revs;
	uint8_t engine_state;
	uint8_t engine_requests;
	uint8_t engine_sample;
	uint8_t engine_loudness;
	uint8_t pad_15[0x3];
	uint8_t tyre_noise;
	uint8_t tyre_noise_sample;
	uint8_t pad_1A[0x2];
	uint8_t tyre_noise_time;
	uint8_t tyre_noise_off;
	uint8_t pad_1E[0x22];
	uint8_t tick_countdown;
	uint8_t pad_41[0x17];
	uint8_t midi_write;
	uint8_t midi_count;
	uint8_t pad_5A[0xA6];
};

struct tyre_noise
{
	uint8_t sample;
	uint8_t pitch;
	uint8_t level;
	uint8_t duration;
};

struct register_value_pair
{
	uint8_t register_number;
	uint8_t value;
};

#pragma pack(pop)

_Static_assert(sizeof(struct midi_part) == 0xA, "struct midi_part has the size of the game's blocks");
_Static_assert(sizeof(struct voice) == 0x9, "struct voice has the size of the game's blocks");
_Static_assert(sizeof(struct sequence_player) == 0x100, "struct sequence_player has the size of the game's blocks");
_Static_assert(sizeof(struct screen_page) == 0x100, "struct screen_page has the size of the game's blocks");
_Static_assert(sizeof(struct tyre_noise) == 0x4, "struct tyre_noise has the size of the program ROM's records");
_Static_assert(sizeof(struct register_value_pair) == 0x2, "struct register_value_pair has the size of the program ROM's records");

#define MIDI_PART(address) ((struct midi_part *)sound_ram_at(address))
#define VOICE(address) ((struct voice *)sound_ram_at(address))
#define SEQUENCE_PLAYER(address) ((struct sequence_player *)sound_ram_at(address))
#define SCREEN_PAGE(address) ((struct screen_page *)sound_ram_at(address))
#define TYRE_NOISE(address) ((const struct tyre_noise *)sound_rom_at(address))
#define REGISTER_VALUE_PAIR(address) ((const struct register_value_pair *)sound_rom_at(address))
#define VOICE_CHIP_CODES(address) ((const uint8_t *)sound_rom_at(address))
#define NOTE_LEVELS(address) ((const uint8_t *)sound_rom_at(address))
#define PCM_PITCH_STEPS(address) ((const uint16_t *)sound_rom_at(address))
#define PCM_PITCH_RUNS(address) ((const uint16_t *)sound_rom_at(address))
#define FM_FNUMBERS(address) ((const uint16_t *)sound_rom_at(address))
#define FM_FNUMBER_RUNS(address) ((const uint16_t *)sound_rom_at(address))
#define FM_REGISTER_NUMBERS(address) ((const uint8_t *)sound_rom_at(address))

/* the program ROM's tables of records, by name: name[i].field */
#define fm_global_init_ADDRESS 0x0234u
extern const void *fm_global_init_data;
#define fm_global_init ((const struct register_value_pair *)(fm_global_init_data))
#define fm_operator_init_ADDRESS 0x024Au
extern const void *fm_operator_init_data;
#define fm_operator_init ((const struct register_value_pair *)(fm_operator_init_data))
#define voice_chip_channels_ADDRESS 0x02E4u
extern const void *voice_chip_channels_data;
#define voice_chip_channels ((const uint8_t *)(voice_chip_channels_data))
#define fm_patch_registers_ADDRESS 0x1534u
extern const void *fm_patch_registers_data;
#define fm_patch_registers ((const uint8_t *)(fm_patch_registers_data))
#define fm_level_registers_ADDRESS 0x1549u
extern const void *fm_level_registers_data;
#define fm_level_registers ((const uint8_t *)(fm_level_registers_data))
#define fm_fnum_tables_ADDRESS 0x1565u
extern const void *fm_fnum_tables_data;
#define fm_fnum_tables ((const uint16_t *)(fm_fnum_tables_data))
#define velocity_levels_ADDRESS 0x2016u
extern const void *velocity_levels_data;
#define velocity_levels ((const uint8_t *)(velocity_levels_data))
#define pcm_pitch_tables_ADDRESS 0x20AEu
extern const void *pcm_pitch_tables_data;
#define pcm_pitch_tables ((const uint16_t *)(pcm_pitch_tables_data))
#define MIDI_PART_ADDRESS(n) (0xE300u + (n) * 0xAu)    /* the 26 of them, from 0 */
#define VOICE_ADDRESS(n) (0xE410u + (n) * 0x9u)    /* the 30 of them, from 0 */
#define SEQUENCE_PLAYER_ADDRESS(n) (0xE900u + (n) * 0x100u)    /* the 16 of them, from 0 */
#define SCREEN_PAGE_ADDRESS(n) (0xE700u + (n) * 0x100u)    /* the 2 of them, from 0 */

/* engine.c: the engine and tyre noise of each screen's car */
void update_engine_screen_1(void);
void update_engine_screen_2(void);
void key_on_with_pan(uint8_t pan);
void set_engine_pitch(uint16_t revs_and_loudness);
void set_engine_sample(uint8_t sample);
void engine_sweep_tick(void);
void update_tyre_noise_screen_1(void);
void update_tyre_noise_screen_2(void);
void start_tyre_noise(uint8_t pan, uint16_t page, uint16_t noise);

/* main.c: the sound program's frame: the reset, the timer interrupt, sound_init and the chips set up and written */
void sound_reset(void);
void sound_irq(void);
void sound_main_loop(void);
void drum_timeout_tick(void);
void sound_init(void);
void init_fm_chip(void);
uint16_t write_fm_channel(uint16_t fm_register, uint8_t value);
void write_fm(uint8_t fm_register, uint8_t value);
void write_fm_part2(uint8_t fm_register, uint8_t value);
void init_pcm_chip(void);
void write_pcm_register(uint8_t pcm_register, uint8_t value);
void init_voice_channels(void);
void init_parts(void);
uint16_t read_table_word(uint16_t table, uint8_t entry);

/* midi.c: the MIDI messages of each screen's queue for its parts: note on and off, pitch bend, controllers, programs; the voices they take */
void handle_midi_message_screen_1(void);
void skip_midi_data_screen_1(uint8_t first_byte);
void play_midi_message_screen_1(uint8_t message_byte, uint16_t part);
void cut_open_hi_hat_screen_1(uint8_t velocity_then_timeout, uint16_t part);
void take_voice_screen_1(uint8_t velocity_then_timeout, uint16_t part);
void release_note_screen_1(uint16_t part);
void retune_voice_screen_1(uint16_t voice);
void set_voice_modulation_screen_1(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice);
void read_midi_byte_screen_1(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte);
void stop_flagged_voices(void);
uint16_t multiply_bytes(uint16_t factor, uint16_t value);
uint16_t reset_next_voice(void);
void handle_midi_message_screen_2(void);
void skip_midi_data_screen_2(uint8_t first_byte);
void play_midi_message_screen_2(uint8_t message_byte, uint16_t part);
void cut_open_hi_hat_screen_2(uint8_t velocity_then_timeout, uint16_t part);
void take_voice_screen_2(uint8_t velocity_then_timeout, uint16_t part);
void release_note_screen_2(uint16_t part);
void retune_voice_screen_2(uint16_t voice);
void set_voice_modulation_screen_2(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice);
void read_midi_byte_screen_2(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte);

/* sequences.c: the sequence players and the playlist: their tick, their events into the MIDI queues, the sound commands, the music's fade */
void sequence_tick(void);
void playlist_command(uint16_t queue_pointer, uint16_t player, uint16_t page, uint16_t *p_playlist, uint8_t *p_playlist_byte);
uint16_t step_player(uint16_t queue_pointer, uint16_t player_address, uint16_t page);
uint8_t chain_next_event(uint16_t player, uint8_t data_byte);
void clear_player_end_flag(uint16_t page);
void poll_sound_commands(void);
void take_sound_command(uint16_t player_address, uint16_t page, uint8_t command);
void start_sound(uint16_t player_address, uint8_t command);
void start_song(uint16_t player, uint8_t song);
void start_sequence(uint16_t header, uint16_t player);
void select_channel_player(uint16_t *p_header, uint16_t *p_player_address);
uint16_t clear_player(uint16_t player);
uint16_t fade_music_volume(uint16_t queue_pointer, uint16_t player, uint16_t page);
uint16_t fade_player_volume(uint16_t queue_pointer, uint16_t player, uint16_t page);
uint16_t next_sequence_byte(uint16_t position, uint16_t player);

/* voices.c: the YM3438's FM voices: starting a note, retuning, stopping, their volume and patches */
void start_fm_note(uint8_t note, uint16_t midi_channel, uint16_t pitch_bends);
void retune_fm_voice(uint16_t voice, uint8_t chip_channel);
void stop_fm_voice(uint16_t voice);
void apply_fm_music_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part);
void apply_fm_channel_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part);
void load_fm_patch(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data);
void load_fm_patch_part2(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data);

#endif
