/**
 *    Name:        game.h
 *    Description: prototypes of the game's functions and the addresses of the code it keeps in memory slots
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    20-10-2026
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
extern uint32_t r12, r14, r20, r22, r23, r25, r26, r27, r28, r32;

/* where the code the program keeps in memory slots (objects' handlers, hooks) is, as the game's own
   addresses: what it stores and compares, and what call_code() runs */
#define sub_001687_ADDRESS 0x001687u
#define sub_001894_ADDRESS 0x001894u
#define sub_0018A4_ADDRESS 0x0018A4u
#define sub_0018E5_ADDRESS 0x0018E5u
#define sub_0018FE_ADDRESS 0x0018FEu
#define sub_001980_ADDRESS 0x001980u
#define sub_0019C0_ADDRESS 0x0019C0u
#define sub_001EC2_ADDRESS 0x001EC2u
#define sub_002204_ADDRESS 0x002204u
#define sub_0022F7_ADDRESS 0x0022F7u
#define sub_002379_ADDRESS 0x002379u
#define sub_0023C6_ADDRESS 0x0023C6u
#define sub_002416_ADDRESS 0x002416u
#define sub_00247A_ADDRESS 0x00247Au
#define sub_00254D_ADDRESS 0x00254Du
#define sub_002612_ADDRESS 0x002612u
#define sub_00265F_ADDRESS 0x00265Fu
#define sub_0026AD_ADDRESS 0x0026ADu
#define sub_00270C_ADDRESS 0x00270Cu
#define sub_00278E_ADDRESS 0x00278Eu
#define sub_002870_ADDRESS 0x002870u
#define sub_002933_ADDRESS 0x002933u
#define sub_002952_ADDRESS 0x002952u
#define sub_002998_ADDRESS 0x002998u
#define sub_003520_ADDRESS 0x003520u
#define sub_003E1B_ADDRESS 0x003E1Bu
#define sub_003F27_ADDRESS 0x003F27u
#define sub_004056_ADDRESS 0x004056u
#define sub_004152_ADDRESS 0x004152u
#define sub_004240_ADDRESS 0x004240u
#define sub_004421_ADDRESS 0x004421u
#define sub_00457D_ADDRESS 0x00457Du
#define sub_00463C_ADDRESS 0x00463Cu
#define sub_0046BB_ADDRESS 0x0046BBu
#define sub_004771_ADDRESS 0x004771u
#define sub_005030_ADDRESS 0x005030u
#define sub_00503D_ADDRESS 0x00503Du
#define sub_005526_ADDRESS 0x005526u
#define sub_0056B4_ADDRESS 0x0056B4u
#define sub_005762_ADDRESS 0x005762u
#define sub_005862_ADDRESS 0x005862u
#define sub_005883_ADDRESS 0x005883u
#define sub_005908_ADDRESS 0x005908u
#define sub_0059B8_ADDRESS 0x0059B8u
#define sub_005A1D_ADDRESS 0x005A1Du
#define sub_005B45_ADDRESS 0x005B45u
#define sub_005BD6_ADDRESS 0x005BD6u
#define sub_005C71_ADDRESS 0x005C71u
#define sub_005CA4_ADDRESS 0x005CA4u
#define sub_005CE5_ADDRESS 0x005CE5u
#define sub_005D14_ADDRESS 0x005D14u
#define sub_005D5C_ADDRESS 0x005D5Cu
#define sub_005E10_ADDRESS 0x005E10u
#define sub_005EB4_ADDRESS 0x005EB4u
#define sub_005F51_ADDRESS 0x005F51u
#define sub_006070_ADDRESS 0x006070u
#define sub_0060B1_ADDRESS 0x0060B1u
#define sub_006105_ADDRESS 0x006105u
#define sub_00613F_ADDRESS 0x00613Fu
#define sub_006173_ADDRESS 0x006173u
#define sub_0061A2_ADDRESS 0x0061A2u
#define sub_006356_ADDRESS 0x006356u
#define sub_0063F9_ADDRESS 0x0063F9u
#define sub_006463_ADDRESS 0x006463u
#define sub_0064E2_ADDRESS 0x0064E2u
#define sub_006514_ADDRESS 0x006514u
#define sub_00652A_ADDRESS 0x00652Au
#define sub_006560_ADDRESS 0x006560u
#define sub_00660F_ADDRESS 0x00660Fu
#define sub_00664E_ADDRESS 0x00664Eu
#define sub_006EF0_ADDRESS 0x006EF0u
#define sub_007C36_ADDRESS 0x007C36u
#define sub_007C9E_ADDRESS 0x007C9Eu
#define sub_009590_ADDRESS 0x009590u
#define sub_00974C_ADDRESS 0x00974Cu
#define sub_00BFC1_ADDRESS 0x00BFC1u
#define sub_00C017_ADDRESS 0x00C017u
#define sub_00C0C6_ADDRESS 0x00C0C6u
#define sub_00C10D_ADDRESS 0x00C10Du
#define sub_00C231_ADDRESS 0x00C231u
#define sub_00C2AC_ADDRESS 0x00C2ACu
#define sub_00C342_ADDRESS 0x00C342u
#define sub_00C43E_ADDRESS 0x00C43Eu
#define sub_00C553_ADDRESS 0x00C553u
#define sub_00C585_ADDRESS 0x00C585u
#define sub_00C59C_ADDRESS 0x00C59Cu
#define sub_00C65F_ADDRESS 0x00C65Fu
#define sub_00C7EC_ADDRESS 0x00C7ECu
#define sub_00C857_ADDRESS 0x00C857u
#define sub_00C873_ADDRESS 0x00C873u
#define sub_00C8B6_ADDRESS 0x00C8B6u
#define sub_00C9ED_ADDRESS 0x00C9EDu
#define sub_00CA44_ADDRESS 0x00CA44u
#define sub_00CA50_ADDRESS 0x00CA50u
#define sub_00CAA8_ADDRESS 0x00CAA8u
#define sub_00CABC_ADDRESS 0x00CABCu
#define sub_00CBC0_ADDRESS 0x00CBC0u
#define sub_00EAE4_ADDRESS 0x00EAE4u
#define sub_0126FC_ADDRESS 0x0126FCu
#define sub_0127EE_ADDRESS 0x0127EEu
#define sub_01287C_ADDRESS 0x01287Cu
#define sub_012906_ADDRESS 0x012906u
#define sub_0129AB_ADDRESS 0x0129ABu
#define sub_0129F4_ADDRESS 0x0129F4u
#define sub_012A20_ADDRESS 0x012A20u
#define sub_012ACF_ADDRESS 0x012ACFu
#define sub_012AF4_ADDRESS 0x012AF4u
#define sub_012B84_ADDRESS 0x012B84u
#define sub_012BAE_ADDRESS 0x012BAEu
#define sub_012CAA_ADDRESS 0x012CAAu
#define sub_012DB7_ADDRESS 0x012DB7u
#define sub_012EA2_ADDRESS 0x012EA2u
#define sub_012FBC_ADDRESS 0x012FBCu
#define sub_01307E_ADDRESS 0x01307Eu
#define sub_013104_ADDRESS 0x013104u
#define sub_0131A5_ADDRESS 0x0131A5u
#define sub_01322D_ADDRESS 0x01322Du
#define sub_013288_ADDRESS 0x013288u
#define sub_013296_ADDRESS 0x013296u
#define sub_01333C_ADDRESS 0x01333Cu
#define sub_01344F_ADDRESS 0x01344Fu
#define sub_01351E_ADDRESS 0x01351Eu
#define sub_0135CC_ADDRESS 0x0135CCu
#define sub_01369A_ADDRESS 0x01369Au
#define sub_01376C_ADDRESS 0x01376Cu
#define sub_01389E_ADDRESS 0x01389Eu
#define sub_0138EB_ADDRESS 0x0138EBu
#define sub_013AB7_ADDRESS 0x013AB7u
#define sub_013AD8_ADDRESS 0x013AD8u
#define sub_013AF9_ADDRESS 0x013AF9u
#define sub_013B56_ADDRESS 0x013B56u
#define sub_013BA3_ADDRESS 0x013BA3u
#define sub_013D15_ADDRESS 0x013D15u
#define sub_013DC4_ADDRESS 0x013DC4u
#define sub_013DD5_ADDRESS 0x013DD5u
#define sub_013DE4_ADDRESS 0x013DE4u
#define sub_014108_ADDRESS 0x014108u
#define sub_0141F4_ADDRESS 0x0141F4u
#define sub_014470_ADDRESS 0x014470u
#define sub_014514_ADDRESS 0x014514u
#define sub_01458D_ADDRESS 0x01458Du
#define sub_014694_ADDRESS 0x014694u
#define sub_014735_ADDRESS 0x014735u
#define sub_014777_ADDRESS 0x014777u
#define sub_014782_ADDRESS 0x014782u
#define sub_014838_ADDRESS 0x014838u
#define sub_014846_ADDRESS 0x014846u
#define sub_016B00_ADDRESS 0x016B00u
#define sub_0171CE_ADDRESS 0x0171CEu
#define sub_017524_ADDRESS 0x017524u
#define sub_0178C9_ADDRESS 0x0178C9u
#define sub_0178D5_ADDRESS 0x0178D5u
#define sub_01794C_ADDRESS 0x01794Cu
#define sub_017958_ADDRESS 0x017958u
#define sub_017BFB_ADDRESS 0x017BFBu
#define sub_017CAA_ADDRESS 0x017CAAu
#define sub_017D1F_ADDRESS 0x017D1Fu
#define sub_017E1B_ADDRESS 0x017E1Bu
#define sub_017E5B_ADDRESS 0x017E5Bu
#define sub_017EF2_ADDRESS 0x017EF2u
#define sub_017F3D_ADDRESS 0x017F3Du
#define sub_0180CB_ADDRESS 0x0180CBu
#define sub_018110_ADDRESS 0x018110u
#define sub_01854A_ADDRESS 0x01854Au
#define sub_0185D2_ADDRESS 0x0185D2u
#define sub_01864A_ADDRESS 0x01864Au
#define sub_018BA0_ADDRESS 0x018BA0u
#define sub_018BB2_ADDRESS 0x018BB2u
#define sub_018E32_ADDRESS 0x018E32u
#define sub_018E98_ADDRESS 0x018E98u
#define sub_018F74_ADDRESS 0x018F74u
#define sub_018FD3_ADDRESS 0x018FD3u
#define sub_018FE0_ADDRESS 0x018FE0u
#define sub_01900A_ADDRESS 0x01900Au
#define sub_019322_ADDRESS 0x019322u
#define sub_019CAC_ADDRESS 0x019CACu
#define sub_019E6B_ADDRESS 0x019E6Bu
#define sub_019F4C_ADDRESS 0x019F4Cu
#define sub_019F60_ADDRESS 0x019F60u
#define sub_01A0E2_ADDRESS 0x01A0E2u
#define sub_01A18C_ADDRESS 0x01A18Cu
#define sub_01A29C_ADDRESS 0x01A29Cu
#define sub_01A329_ADDRESS 0x01A329u
#define sub_01A3E9_ADDRESS 0x01A3E9u
#define sub_01A499_ADDRESS 0x01A499u
#define sub_01A536_ADDRESS 0x01A536u
#define sub_01A672_ADDRESS 0x01A672u
#define sub_01A6CF_ADDRESS 0x01A6CFu
#define sub_01A7C5_ADDRESS 0x01A7C5u
#define sub_01A834_ADDRESS 0x01A834u
#define sub_01A8E2_ADDRESS 0x01A8E2u
#define sub_01AA01_ADDRESS 0x01AA01u
#define sub_01ADE8_ADDRESS 0x01ADE8u
#define sub_01AE8D_ADDRESS 0x01AE8Du
#define sub_01C392_ADDRESS 0x01C392u
#define sub_01C3E6_ADDRESS 0x01C3E6u
#define sub_01C5BA_ADDRESS 0x01C5BAu
#define sub_01C5DB_ADDRESS 0x01C5DBu
#define sub_01C928_ADDRESS 0x01C928u
#define sub_01CDE0_ADDRESS 0x01CDE0u
#define sub_01CE15_ADDRESS 0x01CE15u
#define sub_01CE97_ADDRESS 0x01CE97u
#define sub_01D111_ADDRESS 0x01D111u
#define sub_01D126_ADDRESS 0x01D126u
#define sub_01D13B_ADDRESS 0x01D13Bu
#define sub_01D3E4_ADDRESS 0x01D3E4u
#define sub_01D419_ADDRESS 0x01D419u
#define sub_01D620_ADDRESS 0x01D620u
#define sub_01D661_ADDRESS 0x01D661u
#define sub_01D840_ADDRESS 0x01D840u
#define sub_01D868_ADDRESS 0x01D868u
#define sub_01D87A_ADDRESS 0x01D87Au
#define sub_01D88F_ADDRESS 0x01D88Fu
#define sub_01D9F3_ADDRESS 0x01D9F3u
#define sub_01DA28_ADDRESS 0x01DA28u
#define sub_01DD1E_ADDRESS 0x01DD1Eu
#define sub_01DD2B_ADDRESS 0x01DD2Bu
#define sub_01DD58_ADDRESS 0x01DD58u
#define sub_01DEFD_ADDRESS 0x01DEFDu
#define sub_01E453_ADDRESS 0x01E453u
#define sub_01E4BA_ADDRESS 0x01E4BAu
#define sub_01E6EB_ADDRESS 0x01E6EBu
#define sub_01E703_ADDRESS 0x01E703u
#define sub_01E715_ADDRESS 0x01E715u
#define sub_01E72D_ADDRESS 0x01E72Du
#define sub_01EDC4_ADDRESS 0x01EDC4u
#define sub_01EDF1_ADDRESS 0x01EDF1u
#define sub_01EE66_ADDRESS 0x01EE66u
#define sub_01EEC1_ADDRESS 0x01EEC1u
#define sub_01EEEF_ADDRESS 0x01EEEFu
#define int_vector_40_ADDRESS 0x03F500u
#define int_vector_41_ADDRESS 0x03F794u
#define int_vector_42_ADDRESS 0x03F7A0u
#define int_vector_43_ADDRESS 0x03F7ACu
#define int_vector_44_ADDRESS 0x03F830u

/* game_00.c: the game's functions, part 00 (in the order of their V60 addresses) */
void boot(void);
uint32_t sub_0000C4(uint32_t r10, uint32_t r11);
uint32_t sub_0000FC(void);
void sub_00013C(uint32_t r0);
void sub_0002CA(void);
void sub_0002E1(void);
void sub_000337(uint32_t r0);
void sub_00036B(void);
void sub_000372(void);
void sub_000375(void);
void sub_0003BA(uint8_t r4, uint32_t r13, uint32_t *p_r0, uint32_t *p_r2);
uint32_t sub_0003ED(uint32_t r0, uint32_t r2, uint8_t r4, uint32_t r13);
uint32_t sub_000451(uint32_t r2, uint8_t r4, uint32_t r13);
void sub_0004A1(uint32_t *p_r2, uint32_t *p_r13);
uint32_t sub_0004B8(uint32_t r2, uint32_t r13);
void second_task(uint32_t entry_point);
uint32_t sub_000848(uint32_t r0, uint32_t r2);
uint32_t sub_0008F0(uint32_t r2);
void sub_000913(void);
void sub_000976(void);
void sub_000995(uint32_t r0);
uint32_t sub_0009F9(void);
void sub_000B16(void);
void sub_000BBC(uint8_t r0, uint32_t r2, uint32_t *p_r10, uint32_t *p_r11);
uint32_t sub_000BF3(void);
uint32_t sub_000C1C(uint32_t r10, uint32_t r11);
uint32_t sub_000C5A(void);
void sub_000D7D(void);
void sub_000D9F(void);
void sub_001000(void);
void sub_001440(void);
void sub_00144C(void);
void sub_001458(void);
void sub_001516(void);
uint32_t sub_001544(void);
void sub_001577(void);
void sub_001594(void);
void sub_0015A9(void);
uint32_t sub_0015C0(uint32_t left_caller);
void sub_0015D8(void);
void sub_001687(void);
void sub_001894(void);
void sub_0018A4(uint32_t r0, uint32_t r2);
void sub_0018E5(void);
void sub_0018FE(void);
void sub_00190B(uint32_t r2, uint32_t *p_r0, uint32_t *p_zero);
uint32_t sub_001969(uint32_t r0, uint32_t r1, uint8_t r2, uint32_t r3);
void sub_001980(void);
void sub_0019C0(void);
void sub_001D84(void);
void sub_001DCC(void);
uint32_t sub_001E89(void);
void sub_001EC2(void);
void sub_002204(void);
void sub_0022F7(void);
void sub_002379(void);
void sub_0023C6(void);
void sub_002416(void);
void sub_00247A(void);
void sub_00254D(void);
void sub_002612(void);
void sub_00265F(void);
void sub_0026AD(void);
void sub_00270C(void);
void sub_00278E(void);
void sub_002870(void);
void sub_002933(void);
void sub_002952(void);
void sub_002998(void);
void sub_002AC8(void);
void sub_002E74(void);
void sub_003407(void);
void sub_00340D(void);
void sub_003425(void);
void sub_003520(void);
void sub_003E1B(void);
void sub_003F27(void);
void sub_004056(void);
void sub_004122(void);
void sub_004152(void);
void sub_004240(void);
void sub_004421(void);
void sub_00457D(void);
void sub_00463C(void);
void sub_0046BB(void);
void sub_004771(void);
void sub_005030(void);
void sub_00503D(void);
void sub_0053DF(void);
void sub_005443(void);
void sub_0054BC(void);
void sub_0054C1(void);
void sub_0054C4(uint16_t r0);
void sub_005526(void);
void sub_0056B4(void);
void sub_005762(void);
void sub_005862(void);
void sub_005883(void);
void sub_005908(void);
void sub_0059B8(void);
void sub_005A1D(void);
void sub_005B45(void);
void sub_005BD6(void);
void sub_005C71(void);
void sub_005CA4(void);
void sub_005CE5(void);
void sub_005D14(void);
void sub_005D5C(void);
void sub_005E10(void);
void sub_005EB4(void);
void sub_005F51(void);
void sub_006070(void);
void sub_0060B1(void);
void sub_006105(void);
void sub_00613F(void);
void sub_006173(void);
void sub_0061A2(void);
uint32_t sub_0061DB(uint32_t r1, uint32_t r2, uint32_t r3);
uint32_t sub_006294(void);
void sub_006356(void);

/* game_01.c: the game's functions, part 01 (in the order of their V60 addresses) */
void sub_0063F9(void);
void sub_006463(void);
void sub_0064E2(void);
void sub_006514(void);
void sub_00652A(void);
void sub_006560(void);
void sub_00660F(void);
void sub_00664E(void);
void sub_0068FE(void);
void sub_006EF0(void);
void sub_0070EC(void);
void sub_007396(void);
void sub_007443(void);
void sub_007551(void);
void sub_007677(void);
void sub_007734(void);
void sub_0077A4(void);
void sub_0077DF(void);
void sub_00780E(void);
void sub_007829(uint32_t r9);
uint32_t sub_0078D3(void);
void sub_007908(void);
void sub_007B28(void);
void sub_007C36(void);
void sub_007C9E(void);
void sub_007CF1(void);
void sub_009590(void);
void sub_00974C(void);
uint32_t sub_009C1E(uint32_t r4, uint16_t r5, uint16_t r6, uint8_t r7, uint32_t r8, uint32_t r10, uint16_t r15, uint16_t r16);
uint32_t sub_009C71(uint32_t r4, uint16_t r5, uint16_t r6, uint32_t r8, uint32_t r10, uint32_t r11, uint32_t r13, uint16_t r15, uint16_t r16);
void sub_009C9E(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13);
void sub_009CB6(uint32_t r8, uint16_t r17, uint16_t r18, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_r13);
uint32_t sub_009D54(uint32_t r10, uint16_t r13, uint16_t r17, uint16_t r18);
void sub_00A00C(void);
void sub_00A04A(void);
void sub_00A098(void);
void sub_00A130(uint32_t *p_r11, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r17);
void sub_00A537(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17);
void sub_00A632(uint32_t r11, uint32_t r13, uint32_t r15, uint32_t r17);
void sub_00A85D(uint32_t r7, uint32_t r8, uint32_t r9, uint32_t r19);
void sub_00AABC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t *p_r3, uint32_t *p_r4, uint32_t *p_r5);
void sub_00B4F8(void);
uint32_t sub_00B52F(uint32_t r0, uint16_t r1, uint16_t r2, uint16_t r3);
void sub_00B65F(void);
void sub_00B660(uint32_t r13);
void sub_00B66E(uint32_t r13);
void sub_00B67C(void);
void sub_00B68A(void);
void sub_00B698(void);
void sub_00B699(uint32_t r13);
void sub_00B6A7(uint32_t r13);
void sub_00B6B5(void);
void sub_00B6C3(void);
void sub_00B6D1(void);
void sub_00BFC1(void);
void sub_00C017(void);
void sub_00C0C6(void);
void sub_00C10D(void);
void sub_00C231(void);
void sub_00C2AC(void);
void sub_00C342(void);
void sub_00C43E(void);
void sub_00C553(void);
void sub_00C585(void);
void sub_00C59C(void);
void sub_00C65F(void);
void sub_00C68E(void);
void sub_00C70C(uint32_t r0);
void sub_00C7CD(void);
void sub_00C7EC(void);
void sub_00C857(uint32_t r0);
void sub_00C873(void);
void sub_00C8B6(void);
void sub_00C9ED(void);
void sub_00CA44(void);
void sub_00CA50(void);
void sub_00CAA8(void);
void sub_00CABC(void);
void sub_00CBC0(void);
void sub_00CD8B(uint32_t r21);
void sub_00CE99(uint32_t r0, uint32_t r1);
void sub_00CEA7(uint32_t r0, uint32_t r10);
void sub_00CEF3(uint32_t r0, uint32_t r1);
void sub_00D0C8(void);
void sub_00D13A(void);
void sub_00D298(void);
void sub_00D2BE(void);
void sub_00D2EB(void);
void sub_00D2F0(void);
void sub_00D341(uint32_t r0);
uint32_t sub_00D524(uint32_t left_caller);
uint32_t sub_00D544(uint32_t r0, uint32_t r10);
void sub_00D5E0(uint32_t *p_r2, uint32_t *p_left_caller);
uint32_t sub_00D612(uint32_t left_caller);
void sub_00D664(uint32_t *p_r2, uint32_t *p_left_caller);
uint32_t sub_00D6AD(uint32_t r2, uint32_t left_caller);
void sub_00D6BB(uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D6C7(uint32_t r0, uint32_t r2);
void sub_00D6EC(void);
void sub_00D716(void);
void sub_00D72D(void);
void sub_00D7CB(uint32_t r19, uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D884(uint32_t *p_r2, uint32_t *p_left_caller);
void sub_00D8A5(void);
void sub_00D9F3(void);
void sub_00DB6D(void);
void sub_00DDA8(void);
void sub_00DEC2(void);
void sub_00DED2(void);
uint32_t sub_00DED9(void);
void sub_00DEF8(void);
void sub_00E620(void);
void sub_00E638(void);
void sub_00E67C(uint32_t r1, uint32_t r10, uint32_t r11);
void sub_00E8DF(void);
uint32_t sub_00EAE4(uint32_t r0);
void sub_00EB74(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EC1B(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00ECCD(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00ED82(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);

/* game_02.c: the game's functions, part 02 (in the order of their V60 addresses) */
void sub_00EE37(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EE75(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEBF(uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEF0(uint16_t r1, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEF4(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EEFF(uint16_t r1, uint32_t r2, uint32_t r11, uint32_t *p_r0, uint32_t *p_r10, uint32_t *p_left_caller);
void sub_00EF32(uint32_t *p_r0, uint32_t *p_r10);
void sub_00EF4A(uint16_t r1, uint32_t r2, uint32_t r3, uint32_t *p_r0, uint32_t *p_r10);
void sub_00F04E(uint32_t *p_r0, uint32_t *p_r10);
uint32_t sub_00F104(uint32_t r10);
uint32_t sub_00F126(uint32_t r10);
void sub_00F15C(uint32_t *p_r0, uint32_t *p_r10);
uint32_t sub_00F30D(uint32_t r7);
void sub_00F366(uint32_t r9);
void sub_00F396(void);
void sub_00F5C8(uint32_t r7, uint32_t r15);
void sub_00F911(uint32_t r7, uint32_t r15);
void sub_00FCBC(void);
void sub_00FD22(void);
void sub_00FD78(uint16_t r5);
void sub_010062(void);
void sub_010243(void);
void sub_01028A(uint32_t r0, uint32_t r3, uint32_t r4, uint32_t *p_r1, uint32_t *p_r2);
uint32_t sub_010634(void);
void sub_010688(void);
uint32_t sub_0106AF(void);
uint32_t sub_01070C(void);
void sub_010741(void);
void sub_01076A(void);
void sub_0107BC(void);
void sub_0107F0(void);
void sub_010889(void);
void sub_010890(void);
void sub_0108EA(void);
void sub_010936(void);
void sub_010DC4(uint32_t r10, uint32_t r11);
uint32_t sub_010E42(uint32_t r2);
void sub_010FA1(uint32_t r10);
void sub_011011(uint32_t r10);
void sub_011018(void);
void sub_01106C(uint32_t r6);
void sub_0111CB(uint8_t r1, uint32_t r7);
void sub_011213(void);
uint32_t sub_01122C(uint32_t r9, uint32_t r10, uint32_t r11);
uint32_t sub_011252(uint32_t r6, uint32_t r9, uint32_t r19);
void sub_0112D9(void);
void sub_0112F0(uint32_t r7);
void sub_011324(uint32_t r7, uint16_t r9);
void sub_011341(uint32_t r7, uint32_t r9);
void sub_01137D(uint32_t r7, uint16_t r9);
void sub_01139E(void);
void sub_0113C0(void);
uint32_t sub_0113E5(uint32_t r0);
uint32_t sub_01140E(uint32_t r0, uint32_t r2, uint32_t r3);
uint32_t sub_011418(uint32_t r0);
uint32_t sub_01143E(uint32_t r0, uint32_t r2);
void sub_011594(void);
void sub_0115B1(void);
void sub_0115C0(uint32_t r19);
void sub_0115C5(uint32_t r19);
void sub_0115CC(uint32_t r8, uint32_t r19);
void sub_0115EB(void);
void sub_011615(uint32_t r19);
void sub_011643(uint32_t r19);
void sub_01167E(void);
void sub_0117E4(void);
void sub_011868(uint32_t r19);
uint32_t sub_011874(uint32_t r19);
uint32_t sub_011975(uint32_t r0);
void sub_0119FA(uint32_t r3, uint16_t r5, uint32_t r18);
uint32_t sub_011A32(uint32_t r19);
uint32_t sub_011A72(uint32_t r1, uint32_t r2, uint32_t r3, uint16_t r4, uint16_t r5, uint32_t r10, uint32_t r11, uint32_t r18);
void sub_011C60(void);
void sub_011CC0(void);
uint32_t sub_011D43(void);
uint32_t sub_011E2C(uint32_t r10);
uint32_t sub_011E45(void);
void sub_011E88(void);
void sub_011EB8(uint8_t r0);
void sub_011EDE(uint8_t r0);
void sub_011F30(void);
void sub_011F7C(void);
void sub_011FC3(uint8_t r0);
void sub_011FC9(uint8_t r0);
void sub_011FE6(void);
void sub_011FEA(uint8_t r0);
void sub_012004(void);
uint32_t sub_0120AC(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);
void sub_012109(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t *p_r0, uint32_t *p_r1);
void sub_012238(void);
uint32_t sub_0122AD(uint16_t r1, uint32_t r2, uint32_t r11, uint16_t r15, uint16_t r17);
void sub_0122E3(uint32_t *p_r7, uint32_t *p_r15);
void sub_0122E8(uint32_t *p_r7, uint32_t *p_r15);
void sub_0122EB(uint32_t *p_r7, uint32_t *p_r15);
void sub_0123AF(uint32_t *p_r7, uint32_t *p_r15);
void sub_01256F(void);
uint32_t sub_01262A(void);
void sub_0126FC(void);
uint32_t sub_012758(uint16_t r0, uint32_t r10);
uint32_t sub_012777(void);
uint32_t sub_012786(uint32_t r10);
uint32_t sub_01279C(void);
void sub_0127EE(void);
void sub_01287C(void);
void sub_012894(void);
void sub_012906(void);
void sub_0129AB(void);
void sub_0129F4(void);
void sub_012A20(void);
void sub_012AA5(void);
void sub_012AAB(void);
void sub_012ACF(void);
void sub_012AF4(void);
void sub_012B0C(void);
void sub_012B84(void);
void sub_012BAE(void);
void sub_012CAA(void);
void sub_012DB7(void);
void sub_012EA2(void);
void sub_012FBC(void);

/* game_03.c: the game's functions, part 03 (in the order of their V60 addresses) */
void sub_01307E(void);
void sub_013104(void);
void sub_0131A5(void);
void sub_01322D(void);
void sub_013288(void);
void sub_013296(void);
void sub_01333C(void);
void sub_01344F(void);
void sub_01351E(void);
void sub_0135CC(void);
void sub_01369A(void);
void sub_01376C(void);
void sub_01389E(void);
void sub_0138EB(uint32_t r1);
void sub_013900(void);
void sub_01397E(uint32_t r0, uint32_t r1);
void sub_013AB7(void);
void sub_013AD8(void);
void sub_013AF9(void);
void sub_013B56(void);
void sub_013BA3(void);
void sub_013CF6(void);
void sub_013D15(void);
uint32_t sub_013D29(uint16_t r0);
void sub_013DC4(void);
void sub_013DD5(void);
void sub_013DE4(void);
void sub_01405F(uint32_t *p_r13, uint32_t *p_r15);
void sub_014108(void);
void sub_0141F4(void);
uint32_t sub_0142B4(uint32_t r10);
uint32_t sub_0142D2(uint8_t r9, uint32_t r10);
void sub_0142F7(uint16_t r0, uint32_t r10);
void sub_014470(void);
void sub_0144B5(void);
void sub_0144C9(void);
void sub_0144EB(void);
void sub_014514(void);
void sub_01458D(void);
void sub_014694(void);
void sub_014735(void);
void sub_014777(void);
void sub_014782(void);
void sub_01478B(uint32_t r10);
void sub_014838(void);
void sub_014846(void);
void sub_016B00(void);
void sub_016C23(void);
void sub_016D00(uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r13, uint32_t *p_r15, uint32_t *p_r16, uint32_t *p_r18);
void sub_016E8C(uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r13, uint8_t r15, uint32_t r16, uint16_t r18);
void sub_016FEB(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3, uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r4, uint32_t *p_r5, uint32_t *p_r6, uint32_t *p_r7, uint32_t *p_r8, uint32_t *p_r11, uint32_t *p_zero);
void sub_0170A6(uint8_t r15, uint32_t r16, uint16_t r18, uint32_t *p_r5, uint32_t *p_r6);
void sub_017188(void);
void sub_0171CE(void);
void sub_017524(void);
void sub_01789C(void);
void sub_0178C9(void);
void sub_0178D5(void);
void sub_01791C(uint32_t r0);
void sub_01794C(void);
void sub_017958(void);
void sub_0179AC(uint32_t r19);
void sub_017BC8(uint32_t r0);
void sub_017BFB(void);
void sub_017CAA(void);
void sub_017D1F(void);
void sub_017E1B(void);
void sub_017E5B(void);
void sub_017EF2(void);
void sub_017F3D(void);
void sub_0180CB(void);
void sub_018110(void);
uint32_t sub_01818A(void);
void sub_0184D0(uint32_t r19);
void sub_0184EC(uint32_t r19);
void sub_01850B(uint32_t r19);
void sub_01854A(void);
uint32_t sub_018581(uint8_t r0, uint8_t r1, uint32_t r10, uint32_t r11);
void sub_0185D2(void);
void sub_01864A(uint32_t r0);
void sub_0186FC(void);
uint32_t sub_0187A3(uint32_t r0);
void sub_0187C1(uint8_t r0);
void sub_0187F0(uint8_t r0);
void sub_0187F8(uint8_t r0);
void sub_018827(void);
void sub_018863(void);
void sub_0188BE(void);
void sub_0189D9(void);
void sub_018AF0(void);
void sub_018BA0(void);
void sub_018BB2(void);
void sub_018C8C(void);
void sub_018C99(void);
void sub_018DBC(void);
void sub_018DC9(void);
void sub_018DDF(uint32_t r0, uint32_t r10);
void sub_018E32(void);
void sub_018E98(void);
void sub_018F74(void);
void sub_018FD3(void);
void sub_018FE0(void);
void sub_01900A(void);
void sub_01929E(void);
void sub_0192B1(void);
void sub_0192BC(void);
void sub_0192C3(void);
void sub_0192CA(void);
void sub_0192D1(void);
void sub_0192D8(void);
uint32_t sub_0192E5(uint32_t r0);
void sub_0192F5(uint32_t r10);
uint32_t sub_01930A(uint32_t r10);
void sub_019322(void);
void sub_0193FC(uint8_t r0);
void sub_019402(uint8_t r0);
void sub_019CAC(void);
void sub_019E6B(void);
void sub_019F4C(void);
void sub_019F60(void);

/* game_04.c: the game's functions, part 04 (in the order of their V60 addresses) */
void sub_019FD3(void);
void sub_01A0A2(void);
void sub_01A0E2(void);
void sub_01A18C(void);
void sub_01A29C(void);
void sub_01A329(void);
void sub_01A3E9(void);
void sub_01A499(void);
void sub_01A536(void);
void sub_01A672(void);
void sub_01A6CF(void);
void sub_01A7C5(void);
void sub_01A834(void);
uint32_t sub_01A8CA(uint32_t r1, uint8_t r2);
void sub_01A8E2(void);
void sub_01AA01(void);
void sub_01AB20(void);
void sub_01ADE8(void);
void sub_01AE2D(void);
void sub_01AE61(void);
void sub_01AE6B(void);
void sub_01AE75(uint8_t r0, uint32_t r1);
void sub_01AE8D(void);
void sub_01C241(void);
void sub_01C388(void);
void sub_01C392(void);
void sub_01C3E6(void);
uint32_t sub_01C435(uint32_t r0);
void sub_01C5BA(void);
void sub_01C5DB(void);
void sub_01C928(uint16_t r0);
uint32_t sub_01C93B(uint32_t r0);
void sub_01C98E(uint32_t r10, uint32_t *p_r0, uint32_t *p_r1);
uint32_t sub_01C9BE(uint32_t r0, uint32_t r10);
uint32_t sub_01CA5D(uint32_t r0, uint32_t r10);
uint32_t sub_01CAF8(uint32_t r0, uint32_t r10);
uint32_t sub_01CB8F(uint32_t r0, uint32_t r10);
void sub_01CC80(void);
void sub_01CDE0(void);
void sub_01CE15(uint32_t r0);
void sub_01CE97(void);
uint32_t sub_01CEDB(uint32_t r0);
void sub_01D111(void);
void sub_01D126(uint32_t r0);
void sub_01D13B(void);
uint32_t sub_01D151(uint32_t r0);
void sub_01D3E4(void);
void sub_01D419(void);
void sub_01D49A(void);
void sub_01D4C7(void);
void sub_01D4DF(void);
void sub_01D620(void);
void sub_01D661(void);
void sub_01D6BD(void);
void sub_01D6E7(void);
void sub_01D72C(void);
void sub_01D771(void);
void sub_01D840(void);
void sub_01D868(uint16_t r0);
void sub_01D87A(void);
void sub_01D88F(uint32_t r0);
uint32_t sub_01D8B9(uint32_t r0);
uint32_t sub_01D927(uint16_t r0, uint32_t r10);
uint32_t sub_01D957(uint32_t r0);
void sub_01D9F3(void);
void sub_01DA28(void);
uint32_t sub_01DA7C(void);
void sub_01DB58(void);
void sub_01DB68(void);
void sub_01DB85(void);
void sub_01DB97(void);
void sub_01DBA9(void);
void sub_01DD1E(void);
void sub_01DD2B(void);
void sub_01DD58(uint32_t r1);
void sub_01DDAC(uint32_t *p_r0, uint32_t *p_r1);
void sub_01DE56(void);
void sub_01DEC7(uint32_t r1);
void sub_01DEFD(void);
uint32_t sub_01DF51(uint32_t r1);
uint32_t sub_01E02E(uint32_t r5, uint32_t r10);
void sub_01E04F(uint32_t r1);
void sub_01E053(uint32_t r0, uint32_t r1);
void sub_01E073(uint32_t r1);
void sub_01E09B(uint32_t r1);
void sub_01E0A1(void);
void sub_01E0B0(void);
void sub_01E453(void);
void sub_01E4BA(void);
uint32_t sub_01E523(void);
void sub_01E57F(uint32_t r0);
void sub_01E5C3(void);
void sub_01E5E7(void);
void sub_01E5F7(void);
void sub_01E6EB(void);
void sub_01E703(uint16_t r0);
void sub_01E715(void);
void sub_01E72D(uint16_t r0);
uint32_t sub_01E740(void);
uint32_t sub_01E862(void);
uint32_t sub_01E9A4(uint32_t r0, uint32_t r11);
void sub_01E9E6(uint32_t *p_r0, uint32_t *p_r11);
void sub_01EDC4(void);
void sub_01EDF1(void);
void sub_01EE66(void);
void sub_01EEC1(void);
void sub_01EEEF(void);
uint32_t sub_01EF6E(void);
void sub_01F02C(uint32_t r0);
void sub_01F034(void);
void sub_01F043(void);
void sub_01F052(uint32_t r0);
uint32_t sub_01F085(uint32_t r0, uint32_t r10);
void sub_01F09A(void);
uint32_t sub_01F0CA(uint32_t r0, uint32_t r10);
void sub_01F0ED(uint32_t r0, uint32_t r2, uint32_t r3, uint32_t *p_r1, uint32_t *p_r10);
void sub_01F103(uint32_t r2, uint32_t *p_r0, uint32_t *p_r1, uint32_t *p_r10);
uint32_t sub_01F131(uint32_t r0, uint8_t r1, uint32_t r2, uint32_t r10);
uint32_t sub_01F16C(uint32_t r0, uint32_t r10);
uint32_t sub_01F17D(void);

/* game_05.c: the game's functions, part 05 (in the order of their V60 addresses) */
void sub_01F190(void);
void sub_01F237(void);
void sub_01F266(uint16_t r0);
void sub_01F26B(uint16_t r0);
void int_vector_40(void);
void sub_03F62A(void);
void sub_03F6A0(void);
void int_vector_41(void);
void int_vector_42(void);
void int_vector_43(void);
void int_vector_44(void);
void int_default(void);
void int_vector_02(void);
void sub_03F8C9(uint32_t r0);
void sub_03F8DA(void);
void sub_03F922(void);
void sub_03F96B(void);
void sub_03FA54(uint32_t r10, uint32_t r11);
void reset(void);

#endif
