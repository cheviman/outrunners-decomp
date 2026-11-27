/**
 *    Name:        projection.c
 *    Description: where the game's state is put back into the original's memory for the state's hash: the code addresses the C keeps as function numbers, and the variables kept in the host's memory
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    26-11-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */

#include "game.h"

/* the code calls through memory slots and interrupts can reach, by address */
const game_code_entry game_code[] =
{
	{ 0x0006B0u, call_second_task },
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
	{ 0x00EAE4u, call_draw_object },
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
	{ 0x01C928u, call_test_mode_memory_test_run },
	{ 0x01CDE0u, test_mode_volume_adjustment },
	{ 0x01CE15u, call_test_mode_volume_adjustment_select },
	{ 0x01CE97u, test_mode_volume_adjustment_adjust },
	{ 0x01D111u, test_mode_input_test },
	{ 0x01D126u, call_test_mode_input_test_run },
	{ 0x01D13Bu, test_mode_input_test_release },
	{ 0x01D3E4u, test_mode_output_test },
	{ 0x01D419u, test_mode_output_test_run },
	{ 0x01D620u, test_mode_sound_test },
	{ 0x01D661u, test_mode_sound_test_run },
	{ 0x01D840u, test_mode_crt_test },
	{ 0x01D868u, call_test_mode_crt_test_bars_run },
	{ 0x01D87Au, test_mode_crt_test_grid },
	{ 0x01D88Fu, call_test_mode_crt_test_grid_run },
	{ 0x01D9F3u, test_mode_game_assignments },
	{ 0x01DA28u, test_mode_game_assignments_run },
	{ 0x01DD1Eu, test_mode_coin_assignments },
	{ 0x01DD2Bu, test_mode_coin_assignments_show },
	{ 0x01DD58u, call_test_mode_coin_assignments_run },
	{ 0x01DEFDu, test_mode_coin_assignments_manual_run },
	{ 0x01E453u, test_mode_network_assignments_start },
	{ 0x01E4BAu, test_mode_network_assignments_run },
	{ 0x01E6EBu, test_mode_bookkeeping_start },
	{ 0x01E703u, call_test_mode_bookkeeping_wait },
	{ 0x01E715u, test_mode_bookkeeping_second_page_start },
	{ 0x01E72Du, call_test_mode_bookkeeping_second_page_wait },
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

/* the number of the function at a V70 address (its row of game_code, which is in address order, + 1):
   what the C keeps for an address read out of the program ROM's tables; NO_FUNCTION for an address
   no code was found at, which call_function refuses */
uint32_t function_number(uint32_t address)
{
	const uint32_t rows = FUNCTION_COUNT - 1;
	uint32_t low = 0, high = rows;
	while (low < high)
	{
		const uint32_t middle = (low + high) / 2;
		if (game_code[middle].address < address)
			low = middle + 1;
		else
			high = middle;
	}
	if (low < rows && game_code[low].address == address)
		return low + 1;
	return NO_FUNCTION;
}

/* a function's number as the original's address, for the state's hash; anything else (NO_FUNCTION
   and what records not in use or other union members hold there) as it is */
static uint32_t function_address(uint32_t number)
{
	const uint32_t rows = FUNCTION_COUNT - 1;
	if (number != NO_FUNCTION && number <= rows)
		return game_code[number - 1].address;
	return number;
}

/* and back: the address of a function as its number, anything else as it is */
static uint32_t address_function(uint32_t address)
{
	const uint32_t number = function_number(address);
	return number != NO_FUNCTION ? number : address;
}

/* the families of the objects' fields that hold an address in the program ROM (memory.ROM_FIELDS), and the one
   of each function of game_functions[]: the family its name starts with (an object's handler says which
   family's fields the object holds) */
enum object_family
{
	OBJECT_FAMILY_NONE,
	OBJECT_FAMILY_automatic_panel,
	OBJECT_FAMILY_branch_arrow,
	OBJECT_FAMILY_car_name_letter,
	OBJECT_FAMILY_manual_panel,
	OBJECT_FAMILY_race_background,
	OBJECT_FAMILY_race_opening,
	OBJECT_FAMILY_staff_credits_code,
	OBJECT_FAMILY_stage_end_sounds,
	OBJECT_FAMILY_step_palette,
	OBJECT_FAMILY_title_logo,
	OBJECT_FAMILY_title_logo_copy,
	OBJECT_FAMILY_title_screen,
	OBJECT_FAMILY_welcome_letter,
};

static const uint8_t function_families[FUNCTION_COUNT] =
{
	[FUNCTION_staff_credits_code_start] = OBJECT_FAMILY_staff_credits_code,
	[FUNCTION_staff_credits_code_watch] = OBJECT_FAMILY_staff_credits_code,
	[FUNCTION_staff_credits_code_show] = OBJECT_FAMILY_staff_credits_code,
	[FUNCTION_staff_credits_code_done] = OBJECT_FAMILY_staff_credits_code,
	[FUNCTION_car_name_letter_update] = OBJECT_FAMILY_car_name_letter,
	[FUNCTION_welcome_letter_start] = OBJECT_FAMILY_welcome_letter,
	[FUNCTION_welcome_letter_show] = OBJECT_FAMILY_welcome_letter,
	[FUNCTION_automatic_panel_show] = OBJECT_FAMILY_automatic_panel,
	[FUNCTION_manual_panel_show] = OBJECT_FAMILY_manual_panel,
	[FUNCTION_branch_arrow_update] = OBJECT_FAMILY_branch_arrow,
	[FUNCTION_race_opening_update] = OBJECT_FAMILY_race_opening,
	[FUNCTION_stage_end_sounds_update] = OBJECT_FAMILY_stage_end_sounds,
	[FUNCTION_race_background_update] = OBJECT_FAMILY_race_background,
	[FUNCTION_title_screen_update] = OBJECT_FAMILY_title_screen,
	[FUNCTION_title_logo_update] = OBJECT_FAMILY_title_logo,
	[FUNCTION_title_logo_copy_update] = OBJECT_FAMILY_title_logo_copy,
};

/* the address the original holds for a pointer into the program's data, 'bias' bytes into the record it
   points at (0 for none) */
static uint32_t rom_address(const void *pointer, uint32_t bias)
{
	return pointer == NULL ? 0u : address_of(pointer) + bias;
}

/* the pointer for such an address: the record 'bias' bytes before it (none for 0) */
static const void *rom_place(uint32_t address, uint32_t bias)
{
	return address == 0u ? NULL : rom_pointer(address - bias);
}

/* the family of an object's handler (OBJECT_FAMILY_NONE for a number that is no function's) */
static unsigned object_family(const struct object *object)
{
	return object->handler < FUNCTION_COUNT ? function_families[object->handler] : OBJECT_FAMILY_NONE;
}

/* an object's ROM fields into its bytes, by its handler's family: the addresses the original holds there */
static void object_project(struct object *object, const union object_host *host)
{
	switch (object_family(object))
	{
	case OBJECT_FAMILY_automatic_panel:
		object->automatic_panel.palette_list = rom_address(host->automatic_panel.palette_list, 0u);
		break;
	case OBJECT_FAMILY_branch_arrow:
		object->branch_arrow.palette_list = rom_address(host->branch_arrow.palette_list, 0u);
		break;
	case OBJECT_FAMILY_car_name_letter:
		object->car_name_letter.palette_list = rom_address(host->car_name_letter.palette_list, 0u);
		break;
	case OBJECT_FAMILY_manual_panel:
		object->manual_panel.palette_list = rom_address(host->manual_panel.palette_list, 0u);
		break;
	case OBJECT_FAMILY_race_background:
		object->race_background.next_page_words = rom_address(host->race_background.next_page_words, 0u);
		break;
	case OBJECT_FAMILY_race_opening:
		object->race_opening.sound_list = rom_address(host->race_opening.sound_list, 1u);
		break;
	case OBJECT_FAMILY_staff_credits_code:
		object->staff_credits_code.code_place = rom_address(host->staff_credits_code.code_place, 0u);
		break;
	case OBJECT_FAMILY_stage_end_sounds:
		object->stage_end_sounds.sound_list = rom_address(host->stage_end_sounds.sound_list, 1u);
		break;
	case OBJECT_FAMILY_step_palette:
		object->step_palette.palette_list = rom_address(host->step_palette.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_logo:
		object->title_logo.palette_list = rom_address(host->title_logo.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_logo_copy:
		object->title_logo_copy.palette_list = rom_address(host->title_logo_copy.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_screen:
		object->title_screen.wave_times = rom_address(host->title_screen.wave_times, 0u);
		break;
	case OBJECT_FAMILY_welcome_letter:
		object->welcome_letter.palette_list = rom_address(host->welcome_letter.palette_list, 0u);
		break;
	default:
		break;
	}
}

/* and back: the pointers of its family's fields from its bytes, every other slot none (an object started
   again by its bytes keeps nothing of the one before) */
static void object_unproject(const struct object *object, union object_host *host)
{
	memset(host, 0, sizeof *host);
	switch (object_family(object))
	{
	case OBJECT_FAMILY_automatic_panel:
		host->automatic_panel.palette_list = rom_place(object->automatic_panel.palette_list, 0u);
		break;
	case OBJECT_FAMILY_branch_arrow:
		host->branch_arrow.palette_list = rom_place(object->branch_arrow.palette_list, 0u);
		break;
	case OBJECT_FAMILY_car_name_letter:
		host->car_name_letter.palette_list = rom_place(object->car_name_letter.palette_list, 0u);
		break;
	case OBJECT_FAMILY_manual_panel:
		host->manual_panel.palette_list = rom_place(object->manual_panel.palette_list, 0u);
		break;
	case OBJECT_FAMILY_race_background:
		host->race_background.next_page_words = rom_place(object->race_background.next_page_words, 0u);
		break;
	case OBJECT_FAMILY_race_opening:
		host->race_opening.sound_list = rom_place(object->race_opening.sound_list, 1u);
		break;
	case OBJECT_FAMILY_staff_credits_code:
		host->staff_credits_code.code_place = rom_place(object->staff_credits_code.code_place, 0u);
		break;
	case OBJECT_FAMILY_stage_end_sounds:
		host->stage_end_sounds.sound_list = rom_place(object->stage_end_sounds.sound_list, 1u);
		break;
	case OBJECT_FAMILY_step_palette:
		host->step_palette.palette_list = rom_place(object->step_palette.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_logo:
		host->title_logo.palette_list = rom_place(object->title_logo.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_logo_copy:
		host->title_logo_copy.palette_list = rom_place(object->title_logo_copy.palette_list, 0u);
		break;
	case OBJECT_FAMILY_title_screen:
		host->title_screen.wave_times = rom_place(object->title_screen.wave_times, 0u);
		break;
	case OBJECT_FAMILY_welcome_letter:
		host->welcome_letter.palette_list = rom_place(object->welcome_letter.palette_list, 0u);
		break;
	default:
		break;
	}
}

/* the host part of an object of the state's (host_of): its union object_host in struct world, by its place */
union object_host *object_host(const struct object *object)
{
	const uintptr_t place = (uintptr_t)object;

	if (object == &game->object_area_1.spare_object)
		return &world->object_area_1_spare_object;
	if (place - (uintptr_t)game->object_area_1.objects < sizeof game->object_area_1.objects)
		return &world->object_area_1_objects[(place - (uintptr_t)game->object_area_1.objects) / sizeof (struct object)];
	if (object == &game->object_area_2.spare_object)
		return &world->object_area_2_spare_object;
	if (place - (uintptr_t)game->object_area_2.objects < sizeof game->object_area_2.objects)
		return &world->object_area_2_objects[(place - (uintptr_t)game->object_area_2.objects) / sizeof (struct object)];
	board_outside(address_of(object));
}

/* an object's host part made again from its bytes, after code wrote them without its family's names: a link
   a spawner stores (its parameter, its list_data) or the clear of a new object's (start_object's) */
void object_sync(const struct object *object)
{
	object_unproject(object, object_host(object));
}

/* the same for the object at an address, after a clear of its bytes by address (memory.ROM_FIELD_SYNCS) */
void object_sync_at(uint32_t address)
{
	object_sync((const struct object *)ram(address));
}

/* the game's variables the C keeps in the host's memory (struct world), the runtime's one world's */
static struct world first_world;
struct world *world = &first_world;

/* cleared with the work RAM */
void world_clear(void)
{
	memset(world, 0, sizeof *world);
}

/* into the work RAM at their places, where the state's hash and the code that reaches memory by its
   address read them */
void world_project(void)
{
	game->waiting_task_stack = world->waiting_task_stack;
	game->vblanks_since_start = world->vblanks_since_start;
	game->debug_timer_rounds = world->debug_timer_rounds;
	game->sprite_list_buffers = world->sprite_list_buffers;
	game->sprite_lists_set_up = world->sprite_lists_set_up;
	game->single_task = world->single_task;
	game->game_frame_count = world->game_frame_count;
	game->send_video_registers = world->send_video_registers;
	game->task_swapping = world->task_swapping;
	game->second_task_waiting = world->second_task_waiting;
	game->timer_interrupts_left = world->timer_interrupts_left;
	game->vblank_counter = world->vblank_counter;
	game->cleared_on_race_over = world->cleared_on_race_over;
	for (size_t index = 0; index < 0x8; index++)
		game->car_numbers[index] = world->car_numbers[index];
	game->link_wait_frames_start = world->link_wait_frames_start;
	game->total_time_count = world->total_time_count;
	game->player_count = world->player_count;
	game->link_off = world->link_off;
	game->link_failed = world->link_failed;
	game->link_wait_frames = world->link_wait_frames;
	game->network_check_blink_counter = world->network_check_blink_counter;
	game->isdn_players_this_side = world->isdn_players_this_side;
	game->isdn_players_other_side = world->isdn_players_other_side;
	game->link_wait_count = world->link_wait_count;
	game->chute_entry_offset = world->chute_entry_offset;
	game->seat1_coin_units = world->seat1_coin_units;
	game->seat2_coin_units = world->seat2_coin_units;
	for (size_t index = 0; index < 0x2; index++)
		game->wheel_motor[index] = world->wheel_motor[index];
	game->switches = world->switches;
	game->previous_switches = world->previous_switches;
	game->switches_pressed = world->switches_pressed;
	game->switches_released = world->switches_released;
	game->second_io_chip_port_f = world->second_io_chip_port_f;
	game->seat_1_player = world->seat_1_player;
	game->seat_1_wheel = world->seat_1_wheel;
	game->seat_1_gas = world->seat_1_gas;
	game->seat_1_brake = world->seat_1_brake;
	game->seat_2_player = world->seat_2_player;
	game->seat_2_wheel = world->seat_2_wheel;
	game->seat_2_gas = world->seat_2_gas;
	game->seat_2_brake = world->seat_2_brake;
	for (size_t index = 0; index < 0x10; index++)
		game->lcd_send_buffer_1[index] = world->lcd_send_buffer_1[index];
	for (size_t index = 0; index < 0x10; index++)
		game->lcd_send_buffer_2[index] = world->lcd_send_buffer_2[index];
	game->lcd_restart_frames = world->lcd_restart_frames;
	game->lcd_send_step = world->lcd_send_step;
	game->lcd_busy = world->lcd_busy;
	game->lcd_init_step = world->lcd_init_step;
	game->test_mode_on = world->test_mode_on;
	game->test_mode_flags = world->test_mode_flags;
	game->test_mode_cursor = world->test_mode_cursor;
	game->test_mode_page_cursor = world->test_mode_page_cursor;
	game->test_mode_routine = function_address(world->test_mode_routine);
	game->test_mode_buttons = world->test_mode_buttons;
	game->test_mode_buttons_previous = world->test_mode_buttons_previous;
	game->test_mode_buttons_pressed = world->test_mode_buttons_pressed;
	game->test_mode_buttons_released = world->test_mode_buttons_released;
	game->test_mode_blink_timer = world->test_mode_blink_timer;
	game->sound_test_number_a = world->sound_test_number_a;
	game->sound_test_number_b = world->sound_test_number_b;
	game->test_mode_sound_queue_write = world->test_mode_sound_queue_write;
	game->test_mode_sound_queue_read = world->test_mode_sound_queue_read;
	for (size_t index = 0; index < 0x10; index++)
		game->test_mode_sound_queue[index] = world->test_mode_sound_queue[index];
	game->test_mode_privilege_mode = world->test_mode_privilege_mode;
	object_project(&game->object_area_1.spare_object, &world->object_area_1_spare_object);
	for (size_t index = 0; index < 0x7C; index++)
		object_project(&game->object_area_1.objects[index], &world->object_area_1_objects[index]);
	object_project(&game->object_area_2.spare_object, &world->object_area_2_spare_object);
	for (size_t index = 0; index < 0x7C; index++)
		object_project(&game->object_area_2.objects[index], &world->object_area_2_objects[index]);
	game->object_area_1.spare_object.handler = function_address(game->object_area_1.spare_object.handler);
	for (size_t index = 0; index < 0x7C; index++)
		game->object_area_1.objects[index].handler = function_address(game->object_area_1.objects[index].handler);
	game->object_area_2.spare_object.handler = function_address(game->object_area_2.spare_object.handler);
	for (size_t index = 0; index < 0x7C; index++)
		game->object_area_2.objects[index].handler = function_address(game->object_area_2.objects[index].handler);
}

/* back from the work RAM, after code that reaches memory by its address */
void world_unproject(void)
{
	game->object_area_1.spare_object.handler = address_function(game->object_area_1.spare_object.handler);
	for (size_t index = 0; index < 0x7C; index++)
		game->object_area_1.objects[index].handler = address_function(game->object_area_1.objects[index].handler);
	game->object_area_2.spare_object.handler = address_function(game->object_area_2.spare_object.handler);
	for (size_t index = 0; index < 0x7C; index++)
		game->object_area_2.objects[index].handler = address_function(game->object_area_2.objects[index].handler);
	object_unproject(&game->object_area_1.spare_object, &world->object_area_1_spare_object);
	for (size_t index = 0; index < 0x7C; index++)
		object_unproject(&game->object_area_1.objects[index], &world->object_area_1_objects[index]);
	object_unproject(&game->object_area_2.spare_object, &world->object_area_2_spare_object);
	for (size_t index = 0; index < 0x7C; index++)
		object_unproject(&game->object_area_2.objects[index], &world->object_area_2_objects[index]);
	world->waiting_task_stack = game->waiting_task_stack;
	world->vblanks_since_start = game->vblanks_since_start;
	world->debug_timer_rounds = game->debug_timer_rounds;
	world->sprite_list_buffers = game->sprite_list_buffers;
	world->sprite_lists_set_up = game->sprite_lists_set_up;
	world->single_task = game->single_task;
	world->game_frame_count = game->game_frame_count;
	world->send_video_registers = game->send_video_registers;
	world->task_swapping = game->task_swapping;
	world->second_task_waiting = game->second_task_waiting;
	world->timer_interrupts_left = game->timer_interrupts_left;
	world->vblank_counter = game->vblank_counter;
	world->cleared_on_race_over = game->cleared_on_race_over;
	for (size_t index = 0; index < 0x8; index++)
		world->car_numbers[index] = game->car_numbers[index];
	world->link_wait_frames_start = game->link_wait_frames_start;
	world->total_time_count = game->total_time_count;
	world->player_count = game->player_count;
	world->link_off = game->link_off;
	world->link_failed = game->link_failed;
	world->link_wait_frames = game->link_wait_frames;
	world->network_check_blink_counter = game->network_check_blink_counter;
	world->isdn_players_this_side = game->isdn_players_this_side;
	world->isdn_players_other_side = game->isdn_players_other_side;
	world->link_wait_count = game->link_wait_count;
	world->chute_entry_offset = game->chute_entry_offset;
	world->seat1_coin_units = game->seat1_coin_units;
	world->seat2_coin_units = game->seat2_coin_units;
	for (size_t index = 0; index < 0x2; index++)
		world->wheel_motor[index] = game->wheel_motor[index];
	world->switches = game->switches;
	world->previous_switches = game->previous_switches;
	world->switches_pressed = game->switches_pressed;
	world->switches_released = game->switches_released;
	world->second_io_chip_port_f = game->second_io_chip_port_f;
	world->seat_1_player = game->seat_1_player;
	world->seat_1_wheel = game->seat_1_wheel;
	world->seat_1_gas = game->seat_1_gas;
	world->seat_1_brake = game->seat_1_brake;
	world->seat_2_player = game->seat_2_player;
	world->seat_2_wheel = game->seat_2_wheel;
	world->seat_2_gas = game->seat_2_gas;
	world->seat_2_brake = game->seat_2_brake;
	for (size_t index = 0; index < 0x10; index++)
		world->lcd_send_buffer_1[index] = game->lcd_send_buffer_1[index];
	for (size_t index = 0; index < 0x10; index++)
		world->lcd_send_buffer_2[index] = game->lcd_send_buffer_2[index];
	world->lcd_restart_frames = game->lcd_restart_frames;
	world->lcd_send_step = game->lcd_send_step;
	world->lcd_busy = game->lcd_busy;
	world->lcd_init_step = game->lcd_init_step;
	world->test_mode_on = game->test_mode_on;
	world->test_mode_flags = game->test_mode_flags;
	world->test_mode_cursor = game->test_mode_cursor;
	world->test_mode_page_cursor = game->test_mode_page_cursor;
	world->test_mode_routine = address_function(game->test_mode_routine);
	world->test_mode_buttons = game->test_mode_buttons;
	world->test_mode_buttons_previous = game->test_mode_buttons_previous;
	world->test_mode_buttons_pressed = game->test_mode_buttons_pressed;
	world->test_mode_buttons_released = game->test_mode_buttons_released;
	world->test_mode_blink_timer = game->test_mode_blink_timer;
	world->sound_test_number_a = game->sound_test_number_a;
	world->sound_test_number_b = game->sound_test_number_b;
	world->test_mode_sound_queue_write = game->test_mode_sound_queue_write;
	world->test_mode_sound_queue_read = game->test_mode_sound_queue_read;
	for (size_t index = 0; index < 0x10; index++)
		world->test_mode_sound_queue[index] = game->test_mode_sound_queue[index];
	world->test_mode_privilege_mode = game->test_mode_privilege_mode;
}

