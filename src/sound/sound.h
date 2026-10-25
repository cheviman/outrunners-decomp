/**
 *    Name:        sound.h
 *    Description: the sound program's ports, variables, tables, structures and functions
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    25-10-2026
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
#define SCREEN_1 0x208000u
#define SCREEN_2 0x218000u
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
#define fm_global_init_ADDRESS 0x0234u
#define fm_global_init SOUND_ROM8(fm_global_init_ADDRESS)
#define fm_operator_init_ADDRESS 0x024Au
#define fm_operator_init SOUND_ROM8(fm_operator_init_ADDRESS)
#define voice_chip_channels_ADDRESS 0x02E4u
#define voice_chip_channels SOUND_ROM8(voice_chip_channels_ADDRESS)
#define fm_patch_registers_ADDRESS 0x1534u
#define fm_patch_registers SOUND_ROM8(fm_patch_registers_ADDRESS)
#define fm_level_registers_ADDRESS 0x1549u
#define fm_level_registers SOUND_ROM8(fm_level_registers_ADDRESS)
#define fm_fnum_tables_ADDRESS 0x1565u
#define fm_fnum_tables SOUND_ROM16(fm_fnum_tables_ADDRESS)
#define part_templates_ADDRESS 0x1847u
#define part_templates SOUND_ROM16(part_templates_ADDRESS)
#define instruments_ADDRESS 0x1BC2u
#define instruments SOUND_ROM16(instruments_ADDRESS)
#define note_codes_ADDRESS 0x1F62u
#define note_codes ((const uint8_t *)sound_rom_at(note_codes_ADDRESS))
#define velocity_levels_ADDRESS 0x2016u
#define velocity_levels SOUND_ROM8(velocity_levels_ADDRESS)
#define pcm_pitch_tables_ADDRESS 0x20AEu
#define pcm_pitch_tables SOUND_ROM16(pcm_pitch_tables_ADDRESS)
#define bend_range_tables_ADDRESS 0x2546u
#define bend_range_tables SOUND_ROM16(bend_range_tables_ADDRESS)
#define fm_patches_ADDRESS 0x2A5Eu
#define fm_patches SOUND_ROM16(fm_patches_ADDRESS)
#define drum_kits_ADDRESS 0x2DA5u
#define drum_kits SOUND_ROM16(drum_kits_ADDRESS)
#define engine_levels_ADDRESS 0x37F3u
#define engine_levels ((const uint8_t *)sound_rom_at(engine_levels_ADDRESS))
#define engine_pitches_ADDRESS 0x38F3u
#define engine_pitches SOUND_ROM16(engine_pitches_ADDRESS)
#define tyre_noises_ADDRESS 0x3DC6u
#define tyre_noises SOUND_ROM8(tyre_noises_ADDRESS)
#define songs_ADDRESS 0x439Bu
#define songs SOUND_ROM16(songs_ADDRESS)
#define sound_commands_ADDRESS 0x44DBu
#define sound_commands SOUND_ROM16(sound_commands_ADDRESS)

/* the banked page of the sound ROM and the MultiPCM (read and written through the bus) */
#define pcm_data 0xC000u
#define pcm_voice_select 0xC001u
#define pcm_register_select 0xC002u
#define MULTIPCM_C009 0xC009u

/* data in the program ROM sound_symbols.txt labels */
#define engine_sweep_revs_ADDRESS 0x3B5Du
#define playlist_command_table_ADDRESS 0x401Eu
#define playlist_stop_music_ADDRESS 0x479Fu
#define playlist_silence_screen_ADDRESS 0x47A3u
#define stop_music_sequence_ADDRESS 0x595Du
#define stop_screen_voices_sequence_ADDRESS 0x596Eu

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

#pragma pack(pop)

_Static_assert(sizeof(struct midi_part) == 0xA, "struct midi_part has the size of the game's blocks");
_Static_assert(sizeof(struct voice) == 0x9, "struct voice has the size of the game's blocks");
_Static_assert(sizeof(struct sequence_player) == 0x100, "struct sequence_player has the size of the game's blocks");
_Static_assert(sizeof(struct screen_page) == 0x100, "struct screen_page has the size of the game's blocks");
_Static_assert(sizeof(struct tyre_noise) == 0x4, "struct tyre_noise has the size of the program ROM's records");

#define MIDI_PART(address) ((struct midi_part *)sound_ram_at(address))
#define VOICE(address) ((struct voice *)sound_ram_at(address))
#define SEQUENCE_PLAYER(address) ((struct sequence_player *)sound_ram_at(address))
#define SCREEN_PAGE(address) ((struct screen_page *)sound_ram_at(address))
#define TYRE_NOISE(address) ((const struct tyre_noise *)sound_rom_at(address))
#define MIDI_PART_ADDRESS(n) (0xE300u + (n) * 0xAu)    /* the 26 of them, from 0 */
#define VOICE_ADDRESS(n) (0xE410u + (n) * 0x9u)    /* the 30 of them, from 0 */
#define SEQUENCE_PLAYER_ADDRESS(n) (0xE900u + (n) * 0x100u)    /* the 16 of them, from 0 */
#define SCREEN_PAGE_ADDRESS(n) (0xE700u + (n) * 0x100u)    /* the 2 of them, from 0 */

/* sound_00.c: the sound program's functions, part 00 (in the order of their Z80 addresses) */
void sound_reset(void);
void sound_irq(void);
void sound_main_loop(void);
void drum_timeout_tick(void);
void sound_init(void);
uint16_t init_fm_chip(void);
uint16_t write_fm_channel(uint8_t fm_register, uint8_t value);
void write_fm(uint8_t fm_register, uint8_t value);
void write_fm_part2(uint8_t fm_register, uint8_t value);
void init_pcm_chip(void);
void write_pcm_register(uint8_t pcm_register, uint8_t value);
void init_voice_channels(uint32_t unused_c);
void init_parts(void);
uint16_t read_table_word(uint16_t table, uint8_t entry);
void handle_midi_message_screen_1(void);
void skip_midi_data_screen_1(uint8_t first_byte);
void play_midi_message_screen_1(uint8_t message_byte, uint16_t part);
void retune_voice_screen_1(uint16_t voice);
void set_voice_modulation_screen_1(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice);
void read_midi_byte_screen_1(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte);
void stop_flagged_voices(void);
uint16_t multiply_bytes(uint16_t factor, uint16_t value);
uint16_t reset_next_voice(void);
void handle_midi_message_screen_2(void);
void skip_midi_data_screen_2(uint8_t first_byte);
void play_midi_message_screen_2(uint8_t message_byte, uint16_t part);
void retune_voice_screen_2(uint16_t voice);
void set_voice_modulation_screen_2(uint16_t part, uint16_t *p_voices_left, uint16_t *p_voice);
void read_midi_byte_screen_2(uint16_t *p_midi_byte_copy, uint8_t *p_midi_byte);
void start_fm_note(uint8_t note, uint16_t midi_channel, uint16_t pitch_bends);
void retune_fm_voice(uint16_t voice, uint8_t chip_channel);
void stop_fm_voice(uint16_t voice);
void apply_fm_music_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part);
void apply_fm_channel_volume(uint8_t chip_channel, uint16_t voice_field, uint16_t part);
void load_fm_patch(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data);
void load_fm_patch_part2(uint8_t channel_code, uint16_t *p_channel_in_part, uint16_t *p_register_list, uint16_t *p_patch_data);
void update_engine_screen_1(void);
void update_engine_screen_2(void);
void key_on_with_pan(uint8_t pan);
void set_engine_pitch(uint16_t revs_and_loudness);
void set_engine_sample(uint8_t sample);
void engine_sweep_tick(void);
void update_tyre_noise_screen_1(void);
void update_tyre_noise_screen_2(void);
void start_tyre_noise(uint8_t pan, uint16_t page, uint16_t noise);
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

#endif
