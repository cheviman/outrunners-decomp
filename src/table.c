/**
 *    Name:        table.c
 *    Description: the registers the whole program shares, the code calls through memory slots reach (by address), and the reset entry
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
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

/* How call_code() runs sub_00C857 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_00C857(void)
{
	sub_00C857(0);
}

/* How call_code() runs draw_object from a slot that holds its address: with 0 for what it takes, and
   what it hands back dropped. */
static void call_00EAE4(void)
{
	draw_object(0);
}

/* How call_code() runs sub_0138EB from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_0138EB(void)
{
	sub_0138EB(0);
}

/* How call_code() runs sub_01864A from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01864A(void)
{
	sub_01864A(0);
}

/* How call_code() runs sub_01C928 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01C928(void)
{
	sub_01C928(0);
}

/* How call_code() runs sub_01CE15 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01CE15(void)
{
	sub_01CE15(0);
}

/* How call_code() runs sub_01D126 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01D126(void)
{
	sub_01D126(0);
}

/* How call_code() runs sub_01D868 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01D868(void)
{
	sub_01D868(0);
}

/* How call_code() runs sub_01D88F from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01D88F(void)
{
	sub_01D88F(0);
}

/* How call_code() runs sub_01DD58 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01DD58(void)
{
	sub_01DD58(0);
}

/* How call_code() runs sub_01E703 from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01E703(void)
{
	sub_01E703(0);
}

/* How call_code() runs sub_01E72D from a slot that holds its address: with 0 for what it takes, and what
   it hands back dropped. */
static void call_01E72D(void)
{
	sub_01E72D(0);
}

/* the code calls through memory slots and interrupts can reach, by address */
const game_code_entry game_code[] =
{
	{ 0x0006B0u, call_0006B0 },
	{ 0x0010EBu, sub_0010EB },
	{ 0x001141u, sub_001141 },
	{ 0x0011ABu, sub_0011AB },
	{ 0x001687u, sub_001687 },
	{ 0x001894u, staff_credits_code_start },
	{ 0x0018A4u, call_0018A4 },
	{ 0x0018E5u, staff_credits_code_show },
	{ 0x0018FEu, staff_credits_code_done },
	{ 0x001980u, sub_001980 },
	{ 0x0019C0u, sub_0019C0 },
	{ 0x001EC2u, sub_001EC2 },
	{ 0x002204u, sub_002204 },
	{ 0x0022F7u, sub_0022F7 },
	{ 0x002379u, sub_002379 },
	{ 0x0023C6u, sub_0023C6 },
	{ 0x002416u, sub_002416 },
	{ 0x00247Au, sub_00247A },
	{ 0x00254Du, sub_00254D },
	{ 0x002612u, sub_002612 },
	{ 0x00265Fu, sub_00265F },
	{ 0x0026ADu, sub_0026AD },
	{ 0x00270Cu, sub_00270C },
	{ 0x00278Eu, sub_00278E },
	{ 0x002870u, sub_002870 },
	{ 0x002933u, sub_002933 },
	{ 0x002952u, sub_002952 },
	{ 0x002998u, sub_002998 },
	{ 0x003520u, sub_003520 },
	{ 0x003E1Bu, sub_003E1B },
	{ 0x003F27u, sub_003F27 },
	{ 0x004056u, sub_004056 },
	{ 0x004152u, sub_004152 },
	{ 0x004240u, sub_004240 },
	{ 0x004421u, sub_004421 },
	{ 0x00457Du, sub_00457D },
	{ 0x00463Cu, sub_00463C },
	{ 0x0046BBu, sub_0046BB },
	{ 0x004771u, sub_004771 },
	{ 0x005030u, car_select_start },
	{ 0x00503Du, car_select_run },
	{ 0x005526u, sub_005526 },
	{ 0x0056B4u, sub_0056B4 },
	{ 0x005762u, sub_005762 },
	{ 0x005862u, welcome_letter_start },
	{ 0x005883u, welcome_letter_show },
	{ 0x005908u, sub_005908 },
	{ 0x0059B8u, sub_0059B8 },
	{ 0x005A1Du, sub_005A1D },
	{ 0x005B45u, sub_005B45 },
	{ 0x005BD6u, sub_005BD6 },
	{ 0x005C71u, transmission_label_start_automatic },
	{ 0x005CA4u, transmission_label_start_manual },
	{ 0x005CE5u, transmission_label_follow },
	{ 0x005D14u, sub_005D14 },
	{ 0x005D5Cu, sub_005D5C },
	{ 0x005E10u, sub_005E10 },
	{ 0x005EB4u, sub_005EB4 },
	{ 0x005F51u, sub_005F51 },
	{ 0x006070u, sub_006070 },
	{ 0x0060B1u, sub_0060B1 },
	{ 0x006105u, sub_006105 },
	{ 0x00613Fu, sub_00613F },
	{ 0x006173u, sub_006173 },
	{ 0x0061A2u, sub_0061A2 },
	{ 0x006356u, entry_countdown_start },
	{ 0x0063F9u, entry_countdown_count },
	{ 0x006463u, entry_countdown_done },
	{ 0x0064E2u, entry_banner_start },
	{ 0x006514u, entry_banner_blink },
	{ 0x00652Au, entry_car_ring_start },
	{ 0x006560u, entry_car_ring_draw },
	{ 0x00660Fu, sub_00660F },
	{ 0x00664Eu, sub_00664E },
	{ 0x006EF0u, sub_006EF0 },
	{ 0x007C36u, sub_007C36 },
	{ 0x007C9Eu, sub_007C9E },
	{ 0x009590u, sub_009590 },
	{ 0x00974Cu, sub_00974C },
	{ 0x00AB24u, sub_00AB24 },
	{ 0x00AB36u, sub_00AB36 },
	{ 0x00BFC1u, sub_00BFC1 },
	{ 0x00C017u, sub_00C017 },
	{ 0x00C0C6u, sub_00C0C6 },
	{ 0x00C10Du, sub_00C10D },
	{ 0x00C231u, sub_00C231 },
	{ 0x00C2ACu, sub_00C2AC },
	{ 0x00C342u, sub_00C342 },
	{ 0x00C43Eu, sub_00C43E },
	{ 0x00C553u, stage_indicator_start },
	{ 0x00C585u, stage_indicator_draw },
	{ 0x00C59Cu, sub_00C59C },
	{ 0x00C65Fu, sub_00C65F },
	{ 0x00C7ECu, sub_00C7EC },
	{ 0x00C857u, call_00C857 },
	{ 0x00C873u, sub_00C873 },
	{ 0x00C8B6u, sub_00C8B6 },
	{ 0x00C9EDu, sub_00C9ED },
	{ 0x00CA44u, sub_00CA44 },
	{ 0x00CA50u, sub_00CA50 },
	{ 0x00CAA8u, sub_00CAA8 },
	{ 0x00CABCu, sub_00CABC },
	{ 0x00CBC0u, sub_00CBC0 },
	{ 0x00EAE4u, call_00EAE4 },
	{ 0x0126FCu, sub_0126FC },
	{ 0x0127EEu, sub_0127EE },
	{ 0x01287Cu, scenery_update },
	{ 0x012906u, flashing_scenery_update },
	{ 0x0129ABu, scenery_pair_update },
	{ 0x0129F4u, overhead_scenery_update },
	{ 0x012A20u, sub_012A20 },
	{ 0x012ACFu, fork_sign_update },
	{ 0x012AF4u, animated_scenery_update },
	{ 0x012B84u, animated_scenery_pair_update },
	{ 0x012BAEu, road_crosser_update },
	{ 0x012CAAu, sub_012CAA },
	{ 0x012DB7u, moving_scenery_update },
	{ 0x012EA2u, sub_012EA2 },
	{ 0x012FBCu, sub_012FBC },
	{ 0x01307Eu, sub_01307E },
	{ 0x013104u, flagman_update },
	{ 0x0131A5u, sub_0131A5 },
	{ 0x01322Du, sub_01322D },
	{ 0x013288u, sub_013288 },
	{ 0x013296u, sub_013296 },
	{ 0x01333Cu, sub_01333C },
	{ 0x01344Fu, sub_01344F },
	{ 0x01351Eu, sub_01351E },
	{ 0x0135CCu, sub_0135CC },
	{ 0x01369Au, sub_01369A },
	{ 0x01376Cu, sub_01376C },
	{ 0x01389Eu, sub_01389E },
	{ 0x0138EBu, call_0138EB },
	{ 0x013AB7u, sub_013AB7 },
	{ 0x013AD8u, sub_013AD8 },
	{ 0x013AF9u, sub_013AF9 },
	{ 0x013B56u, sub_013B56 },
	{ 0x013BA3u, sub_013BA3 },
	{ 0x013D15u, sub_013D15 },
	{ 0x013DC4u, sub_013DC4 },
	{ 0x013DD5u, sub_013DD5 },
	{ 0x013DE4u, sub_013DE4 },
	{ 0x014108u, sub_014108 },
	{ 0x0141F4u, sub_0141F4 },
	{ 0x014470u, sub_014470 },
	{ 0x014514u, sub_014514 },
	{ 0x01458Du, sub_01458D },
	{ 0x014694u, sub_014694 },
	{ 0x014735u, sub_014735 },
	{ 0x014777u, sub_014777 },
	{ 0x014782u, sub_014782 },
	{ 0x014838u, sub_014838 },
	{ 0x014846u, sub_014846 },
	{ 0x016B00u, sub_016B00 },
	{ 0x0171CEu, sub_0171CE },
	{ 0x017524u, sub_017524 },
	{ 0x0178C9u, sub_0178C9 },
	{ 0x0178D5u, sub_0178D5 },
	{ 0x01794Cu, sub_01794C },
	{ 0x017958u, sub_017958 },
	{ 0x017BFBu, sub_017BFB },
	{ 0x017CAAu, sub_017CAA },
	{ 0x017D1Fu, sub_017D1F },
	{ 0x017DB0u, sub_017DB0 },
	{ 0x017E1Bu, sub_017E1B },
	{ 0x017E5Bu, sub_017E5B },
	{ 0x017EF2u, sub_017EF2 },
	{ 0x017F3Du, sub_017F3D },
	{ 0x018050u, sub_018050 },
	{ 0x0180CBu, sub_0180CB },
	{ 0x018110u, sub_018110 },
	{ 0x01854Au, sub_01854A },
	{ 0x0185D2u, sub_0185D2 },
	{ 0x01864Au, call_01864A },
	{ 0x018BA0u, lcd_race_info_start },
	{ 0x018BB2u, lcd_race_info_update },
	{ 0x018E32u, lcd_race_info_finish },
	{ 0x018E98u, lcd_race_info_paused },
	{ 0x018F74u, car_radio_start },
	{ 0x018FD3u, car_radio_wait_start },
	{ 0x018FE0u, car_radio_read_buttons },
	{ 0x01900Au, car_radio_update },
	{ 0x019322u, sub_019322 },
	{ 0x019CACu, sub_019CAC },
	{ 0x019E6Bu, sub_019E6B },
	{ 0x019F4Cu, sub_019F4C },
	{ 0x019F60u, sub_019F60 },
	{ 0x01A0E2u, sub_01A0E2 },
	{ 0x01A18Cu, sub_01A18C },
	{ 0x01A29Cu, sub_01A29C },
	{ 0x01A329u, sub_01A329 },
	{ 0x01A3E9u, sub_01A3E9 },
	{ 0x01A499u, sub_01A499 },
	{ 0x01A536u, sub_01A536 },
	{ 0x01A672u, sub_01A672 },
	{ 0x01A6CFu, sub_01A6CF },
	{ 0x01A7C5u, sub_01A7C5 },
	{ 0x01A834u, sub_01A834 },
	{ 0x01A8E2u, sub_01A8E2 },
	{ 0x01AA01u, sub_01AA01 },
	{ 0x01ADE8u, sub_01ADE8 },
	{ 0x01AE8Du, sub_01AE8D },
	{ 0x01C392u, sub_01C392 },
	{ 0x01C3E6u, sub_01C3E6 },
	{ 0x01C5BAu, sub_01C5BA },
	{ 0x01C5DBu, sub_01C5DB },
	{ 0x01C928u, call_01C928 },
	{ 0x01CDE0u, sub_01CDE0 },
	{ 0x01CE15u, call_01CE15 },
	{ 0x01CE97u, sub_01CE97 },
	{ 0x01D111u, sub_01D111 },
	{ 0x01D126u, call_01D126 },
	{ 0x01D13Bu, sub_01D13B },
	{ 0x01D3E4u, sub_01D3E4 },
	{ 0x01D419u, sub_01D419 },
	{ 0x01D620u, sub_01D620 },
	{ 0x01D661u, sub_01D661 },
	{ 0x01D840u, sub_01D840 },
	{ 0x01D868u, call_01D868 },
	{ 0x01D87Au, sub_01D87A },
	{ 0x01D88Fu, call_01D88F },
	{ 0x01D9F3u, sub_01D9F3 },
	{ 0x01DA28u, sub_01DA28 },
	{ 0x01DD1Eu, sub_01DD1E },
	{ 0x01DD2Bu, sub_01DD2B },
	{ 0x01DD58u, call_01DD58 },
	{ 0x01DEFDu, sub_01DEFD },
	{ 0x01E453u, sub_01E453 },
	{ 0x01E4BAu, sub_01E4BA },
	{ 0x01E6EBu, sub_01E6EB },
	{ 0x01E703u, call_01E703 },
	{ 0x01E715u, sub_01E715 },
	{ 0x01E72Du, call_01E72D },
	{ 0x01EDC4u, sub_01EDC4 },
	{ 0x01EDF1u, sub_01EDF1 },
	{ 0x01EE66u, sub_01EE66 },
	{ 0x01EEC1u, sub_01EEC1 },
	{ 0x01EEEFu, sub_01EEEF },
	{ 0x03F500u, int_vector_40 },
	{ 0x03F794u, int_vector_41 },
	{ 0x03F7A0u, int_vector_42 },
	{ 0x03F7ACu, int_vector_43 },
	{ 0x03F830u, int_vector_44 },
};

const size_t game_code_count = 243;

/* the code at the reset vector */
void game_entry(void)
{
	reset();
}
