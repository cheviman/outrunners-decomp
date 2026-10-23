/**
 *    Name:        ram.h
 *    Description: the game's variables, tables, board registers and structures
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    23-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#ifndef ORUNNERS_RAM_H
#define ORUNNERS_RAM_H

/* the game's variables in work RAM; SCREEN_1 and SCREEN_2 are the blocks of the two screens,
   which the code reaches through screen (R25 of the task that runs each) */
#define SCREEN_1 0x208000u
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
#define unk_20E000_ADDRESS 0x20E000u
#define unk_20E000 RAM32(unk_20E000_ADDRESS)
#define unk_20E004_ADDRESS 0x20E004u
#define unk_20E004 RAM32(unk_20E004_ADDRESS)
#define unk_20E00A_ADDRESS 0x20E00Au
#define unk_20E00A RAM16(unk_20E00A_ADDRESS)
#define unk_20E00E_ADDRESS 0x20E00Eu
#define unk_20E00E RAM8(unk_20E00E_ADDRESS)
#define unk_20E020_ADDRESS 0x20E020u
#define unk_20E020 RAM16(unk_20E020_ADDRESS)
#define unk_20E022_ADDRESS 0x20E022u
#define unk_20E022 RAM8(unk_20E022_ADDRESS)
#define unk_20E023_ADDRESS 0x20E023u
#define unk_20E023 RAM8(unk_20E023_ADDRESS)
#define unk_20E024_ADDRESS 0x20E024u
#define unk_20E024 RAM8(unk_20E024_ADDRESS)
#define unk_20E026_ADDRESS 0x20E026u
#define unk_20E026 RAM8(unk_20E026_ADDRESS)
#define unk_20E027_ADDRESS 0x20E027u
#define unk_20E027 RAM8(unk_20E027_ADDRESS)
#define unk_20E028_ADDRESS 0x20E028u
#define unk_20E028 RAM8(unk_20E028_ADDRESS)
#define unk_20E02B_ADDRESS 0x20E02Bu
#define unk_20E02B RAM8(unk_20E02B_ADDRESS)
#define unk_20E02C_ADDRESS 0x20E02Cu
#define unk_20E02C RAM8(unk_20E02C_ADDRESS)
#define unk_20E02D_ADDRESS 0x20E02Du
#define unk_20E02D RAM8(unk_20E02D_ADDRESS)
#define unk_20E02E_ADDRESS 0x20E02Eu
#define unk_20E02E RAM8(unk_20E02E_ADDRESS)
#define unk_20E030_ADDRESS 0x20E030u
#define unk_20E030 ((uint8_t *)ram(unk_20E030_ADDRESS))
#define unk_20E038_ADDRESS 0x20E038u
#define unk_20E038 RAM16(unk_20E038_ADDRESS)
#define unk_20E03A_ADDRESS 0x20E03Au
#define unk_20E03A RAM16(unk_20E03A_ADDRESS)
#define unk_20E040_ADDRESS 0x20E040u
#define unk_20E040 RAM8(unk_20E040_ADDRESS)
#define unk_20E041_ADDRESS 0x20E041u
#define unk_20E041 RAM8(unk_20E041_ADDRESS)
#define unk_20E042_ADDRESS 0x20E042u
#define unk_20E042 RAM8(unk_20E042_ADDRESS)
#define unk_20E043_ADDRESS 0x20E043u
#define unk_20E043 RAM8(unk_20E043_ADDRESS)
#define unk_20E046_ADDRESS 0x20E046u
#define unk_20E046 RAM16(unk_20E046_ADDRESS)
#define unk_20E048_ADDRESS 0x20E048u
#define unk_20E048 RAM16(unk_20E048_ADDRESS)
#define unk_20E04A_ADDRESS 0x20E04Au
#define unk_20E04A RAM8(unk_20E04A_ADDRESS)
#define unk_20E04B_ADDRESS 0x20E04Bu
#define unk_20E04B RAM8(unk_20E04B_ADDRESS)
#define unk_20E060_ADDRESS 0x20E060u
#define unk_20E060 RAM32(unk_20E060_ADDRESS)
#define unk_20E08C_ADDRESS 0x20E08Cu
#define unk_20E08C RAM8(unk_20E08C_ADDRESS)
#define unk_20E08D_ADDRESS 0x20E08Du
#define unk_20E08D RAM8(unk_20E08D_ADDRESS)
#define unk_20E08E_ADDRESS 0x20E08Eu
#define unk_20E08E RAM8(unk_20E08E_ADDRESS)
#define unk_20E08F_ADDRESS 0x20E08Fu
#define unk_20E08F RAM8(unk_20E08F_ADDRESS)
#define unk_20E091_ADDRESS 0x20E091u
#define unk_20E091 RAM8(unk_20E091_ADDRESS)
#define unk_20E092_ADDRESS 0x20E092u
#define unk_20E092 ((uint16_t *)ram(unk_20E092_ADDRESS))
#define unk_20E094_ADDRESS 0x20E094u
#define unk_20E094 RAM16(unk_20E094_ADDRESS)
#define unk_20E0A0_ADDRESS 0x20E0A0u
#define unk_20E0A0 RAM8(unk_20E0A0_ADDRESS)
#define unk_20E0A1_ADDRESS 0x20E0A1u
#define unk_20E0A1 RAM8(unk_20E0A1_ADDRESS)
#define unk_20E0A4_ADDRESS 0x20E0A4u
#define unk_20E0A4 RAM8(unk_20E0A4_ADDRESS)
#define unk_20E0A8_ADDRESS 0x20E0A8u
#define unk_20E0A8 RAM8(unk_20E0A8_ADDRESS)
#define unk_20E0A9_ADDRESS 0x20E0A9u
#define unk_20E0A9 RAM8(unk_20E0A9_ADDRESS)
#define unk_20E0AC_ADDRESS 0x20E0ACu
#define unk_20E0AC RAM8(unk_20E0AC_ADDRESS)
#define unk_20E0C0_ADDRESS 0x20E0C0u
#define unk_20E0C0 ((uint8_t *)ram(unk_20E0C0_ADDRESS))
#define unk_20E0C1_ADDRESS 0x20E0C1u
#define unk_20E0C1 RAM8(unk_20E0C1_ADDRESS)
#define unk_20E0C2_ADDRESS 0x20E0C2u
#define unk_20E0C2 RAM8(unk_20E0C2_ADDRESS)
#define unk_20E0C3_ADDRESS 0x20E0C3u
#define unk_20E0C3 RAM8(unk_20E0C3_ADDRESS)
#define unk_20E0C4_ADDRESS 0x20E0C4u
#define unk_20E0C4 RAM8(unk_20E0C4_ADDRESS)
#define unk_20E0C5_ADDRESS 0x20E0C5u
#define unk_20E0C5 RAM8(unk_20E0C5_ADDRESS)
#define unk_20E0C6_ADDRESS 0x20E0C6u
#define unk_20E0C6 RAM8(unk_20E0C6_ADDRESS)
#define unk_20E0C7_ADDRESS 0x20E0C7u
#define unk_20E0C7 RAM8(unk_20E0C7_ADDRESS)
#define unk_20E0C8_ADDRESS 0x20E0C8u
#define unk_20E0C8 ((uint8_t *)ram(unk_20E0C8_ADDRESS))
#define unk_20E0C9_ADDRESS 0x20E0C9u
#define unk_20E0C9 RAM8(unk_20E0C9_ADDRESS)
#define unk_20E0CA_ADDRESS 0x20E0CAu
#define unk_20E0CA RAM8(unk_20E0CA_ADDRESS)
#define unk_20E0CB_ADDRESS 0x20E0CBu
#define unk_20E0CB RAM8(unk_20E0CB_ADDRESS)
#define unk_20E0CC_ADDRESS 0x20E0CCu
#define unk_20E0CC RAM8(unk_20E0CC_ADDRESS)
#define unk_20E0CD_ADDRESS 0x20E0CDu
#define unk_20E0CD RAM8(unk_20E0CD_ADDRESS)
#define unk_20E0CE_ADDRESS 0x20E0CEu
#define unk_20E0CE RAM8(unk_20E0CE_ADDRESS)
#define unk_20E0CF_ADDRESS 0x20E0CFu
#define unk_20E0CF RAM8(unk_20E0CF_ADDRESS)
#define unk_20E0D0_ADDRESS 0x20E0D0u
#define unk_20E0D0 ((uint8_t *)ram(unk_20E0D0_ADDRESS))
#define unk_20E0D1_ADDRESS 0x20E0D1u
#define unk_20E0D1 RAM8(unk_20E0D1_ADDRESS)
#define unk_20E0FC_ADDRESS 0x20E0FCu
#define unk_20E0FC RAM8(unk_20E0FC_ADDRESS)
#define unk_20E0FD_ADDRESS 0x20E0FDu
#define unk_20E0FD RAM8(unk_20E0FD_ADDRESS)
#define unk_20E0FE_ADDRESS 0x20E0FEu
#define unk_20E0FE RAM8(unk_20E0FE_ADDRESS)
#define unk_20E0FF_ADDRESS 0x20E0FFu
#define unk_20E0FF RAM8(unk_20E0FF_ADDRESS)
#define unk_20E100_ADDRESS 0x20E100u
#define unk_20E100 RAM8(unk_20E100_ADDRESS)
#define unk_20E101_ADDRESS 0x20E101u
#define unk_20E101 RAM8(unk_20E101_ADDRESS)
#define unk_20E102_ADDRESS 0x20E102u
#define unk_20E102 RAM8(unk_20E102_ADDRESS)
#define unk_20E103_ADDRESS 0x20E103u
#define unk_20E103 RAM8(unk_20E103_ADDRESS)
#define unk_20E104_ADDRESS 0x20E104u
#define unk_20E104 RAM8(unk_20E104_ADDRESS)
#define unk_20E116_ADDRESS 0x20E116u
#define unk_20E116 RAM8(unk_20E116_ADDRESS)
#define unk_20E117_ADDRESS 0x20E117u
#define unk_20E117 RAM8(unk_20E117_ADDRESS)
#define unk_20E118_ADDRESS 0x20E118u
#define unk_20E118 RAM8(unk_20E118_ADDRESS)
#define unk_20E119_ADDRESS 0x20E119u
#define unk_20E119 RAM8(unk_20E119_ADDRESS)
#define unk_20E11A_ADDRESS 0x20E11Au
#define unk_20E11A RAM8(unk_20E11A_ADDRESS)
#define unk_20E11B_ADDRESS 0x20E11Bu
#define unk_20E11B RAM8(unk_20E11B_ADDRESS)
#define unk_20E120_ADDRESS 0x20E120u
#define unk_20E120 RAM8(unk_20E120_ADDRESS)
#define unk_20E121_ADDRESS 0x20E121u
#define unk_20E121 RAM8(unk_20E121_ADDRESS)
#define unk_20E122_ADDRESS 0x20E122u
#define unk_20E122 RAM8(unk_20E122_ADDRESS)
#define unk_20E123_ADDRESS 0x20E123u
#define unk_20E123 RAM8(unk_20E123_ADDRESS)
#define unk_20E124_ADDRESS 0x20E124u
#define unk_20E124 RAM8(unk_20E124_ADDRESS)
#define unk_20E125_ADDRESS 0x20E125u
#define unk_20E125 RAM8(unk_20E125_ADDRESS)
#define unk_20E126_ADDRESS 0x20E126u
#define unk_20E126 RAM8(unk_20E126_ADDRESS)
#define unk_20E127_ADDRESS 0x20E127u
#define unk_20E127 RAM8(unk_20E127_ADDRESS)
#define unk_20E128_ADDRESS 0x20E128u
#define unk_20E128 RAM8(unk_20E128_ADDRESS)
#define unk_20E129_ADDRESS 0x20E129u
#define unk_20E129 RAM8(unk_20E129_ADDRESS)
#define unk_20E200_ADDRESS 0x20E200u
#define unk_20E200 RAM16(unk_20E200_ADDRESS)
#define unk_20E202_ADDRESS 0x20E202u
#define unk_20E202 RAM16(unk_20E202_ADDRESS)
#define unk_20E204_ADDRESS 0x20E204u
#define unk_20E204 RAM16(unk_20E204_ADDRESS)
#define unk_20E210_ADDRESS 0x20E210u
#define unk_20E210 ((uint32_t *)ram(unk_20E210_ADDRESS))
#define unk_20E214_ADDRESS 0x20E214u
#define unk_20E214 ((uint32_t *)ram(unk_20E214_ADDRESS))
#define unk_20E220_ADDRESS 0x20E220u
#define unk_20E220 ((uint32_t *)ram(unk_20E220_ADDRESS))
#define unk_20E224_ADDRESS 0x20E224u
#define unk_20E224 ((uint32_t *)ram(unk_20E224_ADDRESS))
#define unk_20E240_ADDRESS 0x20E240u
#define unk_20E240 ((uint32_t *)ram(unk_20E240_ADDRESS))
#define unk_20E248_ADDRESS 0x20E248u
#define unk_20E248 ((uint32_t *)ram(unk_20E248_ADDRESS))
#define unk_20E288_ADDRESS 0x20E288u
#define unk_20E288 RAM16(unk_20E288_ADDRESS)
#define unk_20E28A_ADDRESS 0x20E28Au
#define unk_20E28A RAM16(unk_20E28A_ADDRESS)
#define unk_20E28C_ADDRESS 0x20E28Cu
#define unk_20E28C RAM16(unk_20E28C_ADDRESS)
#define unk_20E290_ADDRESS 0x20E290u
#define unk_20E290 ((uint32_t *)ram(unk_20E290_ADDRESS))
#define unk_20E294_ADDRESS 0x20E294u
#define unk_20E294 ((uint32_t *)ram(unk_20E294_ADDRESS))
#define unk_20E2A0_ADDRESS 0x20E2A0u
#define unk_20E2A0 ((uint32_t *)ram(unk_20E2A0_ADDRESS))
#define unk_20E2A4_ADDRESS 0x20E2A4u
#define unk_20E2A4 ((uint32_t *)ram(unk_20E2A4_ADDRESS))
#define unk_20E2B0_ADDRESS 0x20E2B0u
#define unk_20E2B0 RAM8(unk_20E2B0_ADDRESS)
#define unk_20E2B1_ADDRESS 0x20E2B1u
#define unk_20E2B1 RAM8(unk_20E2B1_ADDRESS)
#define unk_20E2B2_ADDRESS 0x20E2B2u
#define unk_20E2B2 RAM8(unk_20E2B2_ADDRESS)
#define unk_20E2B3_ADDRESS 0x20E2B3u
#define unk_20E2B3 RAM8(unk_20E2B3_ADDRESS)
#define unk_20E300_ADDRESS 0x20E300u
#define unk_20E300 ((uint8_t *)ram(unk_20E300_ADDRESS))
#define unk_20E304_ADDRESS 0x20E304u
#define unk_20E304 ((uint16_t *)ram(unk_20E304_ADDRESS))
#define unk_20E3C0_ADDRESS 0x20E3C0u
#define unk_20E3C0 ((uint8_t *)ram(unk_20E3C0_ADDRESS))
#define unk_20E3C4_ADDRESS 0x20E3C4u
#define unk_20E3C4 RAM32(unk_20E3C4_ADDRESS)
#define unk_20E3C8_ADDRESS 0x20E3C8u
#define unk_20E3C8 RAM32(unk_20E3C8_ADDRESS)
#define unk_20E3CC_ADDRESS 0x20E3CCu
#define unk_20E3CC RAM32(unk_20E3CC_ADDRESS)
#define unk_20E3D0_ADDRESS 0x20E3D0u
#define unk_20E3D0 ((uint8_t *)ram(unk_20E3D0_ADDRESS))
#define unk_20E3D4_ADDRESS 0x20E3D4u
#define unk_20E3D4 RAM32(unk_20E3D4_ADDRESS)
#define unk_20E3D8_ADDRESS 0x20E3D8u
#define unk_20E3D8 RAM32(unk_20E3D8_ADDRESS)
#define unk_20E3DC_ADDRESS 0x20E3DCu
#define unk_20E3DC RAM32(unk_20E3DC_ADDRESS)
#define unk_20E3F0_ADDRESS 0x20E3F0u
#define unk_20E3F0 RAM16(unk_20E3F0_ADDRESS)
#define unk_20E3F2_ADDRESS 0x20E3F2u
#define unk_20E3F2 RAM8(unk_20E3F2_ADDRESS)
#define unk_20E3F3_ADDRESS 0x20E3F3u
#define unk_20E3F3 RAM8(unk_20E3F3_ADDRESS)
#define unk_20E3F4_ADDRESS 0x20E3F4u
#define unk_20E3F4 RAM8(unk_20E3F4_ADDRESS)
#define unk_20E600_ADDRESS 0x20E600u
#define unk_20E600 RAM32(unk_20E600_ADDRESS)
#define unk_20E604_ADDRESS 0x20E604u
#define unk_20E604 RAM16(unk_20E604_ADDRESS)
#define unk_20E608_ADDRESS 0x20E608u
#define unk_20E608 RAM16(unk_20E608_ADDRESS)
#define unk_20E60A_ADDRESS 0x20E60Au
#define unk_20E60A RAM16(unk_20E60A_ADDRESS)
#define unk_20E60C_ADDRESS 0x20E60Cu
#define unk_20E60C RAM8(unk_20E60C_ADDRESS)
#define unk_20E60D_ADDRESS 0x20E60Du
#define unk_20E60D RAM8(unk_20E60D_ADDRESS)
#define unk_20E610_ADDRESS 0x20E610u
#define unk_20E610 RAM8(unk_20E610_ADDRESS)
#define unk_20E611_ADDRESS 0x20E611u
#define unk_20E611 RAM8(unk_20E611_ADDRESS)
#define unk_20E612_ADDRESS 0x20E612u
#define unk_20E612 RAM8(unk_20E612_ADDRESS)
#define unk_20E613_ADDRESS 0x20E613u
#define unk_20E613 RAM8(unk_20E613_ADDRESS)
#define unk_20E614_ADDRESS 0x20E614u
#define unk_20E614 RAM8(unk_20E614_ADDRESS)
#define unk_20E615_ADDRESS 0x20E615u
#define unk_20E615 RAM8(unk_20E615_ADDRESS)
#define unk_20E620_ADDRESS 0x20E620u
#define unk_20E620 RAM8(unk_20E620_ADDRESS)
#define unk_20E621_ADDRESS 0x20E621u
#define unk_20E621 RAM8(unk_20E621_ADDRESS)
#define unk_20E622_ADDRESS 0x20E622u
#define unk_20E622 RAM8(unk_20E622_ADDRESS)
#define unk_20E623_ADDRESS 0x20E623u
#define unk_20E623 RAM8(unk_20E623_ADDRESS)
#define unk_20E624_ADDRESS 0x20E624u
#define unk_20E624 RAM8(unk_20E624_ADDRESS)
#define unk_20E630_ADDRESS 0x20E630u
#define unk_20E630 RAM8(unk_20E630_ADDRESS)
#define unk_20E632_ADDRESS 0x20E632u
#define unk_20E632 RAM16(unk_20E632_ADDRESS)
#define unk_20E634_ADDRESS 0x20E634u
#define unk_20E634 RAM32(unk_20E634_ADDRESS)
#define unk_20E644_ADDRESS 0x20E644u
#define unk_20E644 RAM16(unk_20E644_ADDRESS)
#define unk_20E646_ADDRESS 0x20E646u
#define unk_20E646 RAM16(unk_20E646_ADDRESS)
#define unk_20E700_ADDRESS 0x20E700u
#define unk_20E700 RAM8(unk_20E700_ADDRESS)
#define unk_20E701_ADDRESS 0x20E701u
#define unk_20E701 RAM32(unk_20E701_ADDRESS)
#define unk_20E708_ADDRESS 0x20E708u
#define unk_20E708 RAM32(unk_20E708_ADDRESS)
#define unk_20E70C_ADDRESS 0x20E70Cu
#define unk_20E70C RAM8(unk_20E70C_ADDRESS)
#define unk_20E70D_ADDRESS 0x20E70Du
#define unk_20E70D RAM8(unk_20E70D_ADDRESS)
#define unk_20E70E_ADDRESS 0x20E70Eu
#define unk_20E70E RAM32(unk_20E70E_ADDRESS)
#define unk_20E72F_ADDRESS 0x20E72Fu
#define unk_20E72F RAM8(unk_20E72F_ADDRESS)
#define unk_20E730_ADDRESS 0x20E730u
#define unk_20E730 RAM8(unk_20E730_ADDRESS)
#define unk_20E731_ADDRESS 0x20E731u
#define unk_20E731 RAM8(unk_20E731_ADDRESS)
#define unk_20E732_ADDRESS 0x20E732u
#define unk_20E732 RAM8(unk_20E732_ADDRESS)
#define unk_20E733_ADDRESS 0x20E733u
#define unk_20E733 RAM8(unk_20E733_ADDRESS)
#define unk_20E734_ADDRESS 0x20E734u
#define unk_20E734 ((uint16_t *)ram(unk_20E734_ADDRESS))
#define unk_20E754_ADDRESS 0x20E754u
#define unk_20E754 RAM8(unk_20E754_ADDRESS)
#define unk_20FB00_ADDRESS 0x20FB00u
#define unk_20FB00 RAM8(unk_20FB00_ADDRESS)
#define unk_20FB01_ADDRESS 0x20FB01u
#define unk_20FB01 RAM8(unk_20FB01_ADDRESS)
#define unk_20FB02_ADDRESS 0x20FB02u
#define unk_20FB02 RAM8(unk_20FB02_ADDRESS)
#define unk_20FB03_ADDRESS 0x20FB03u
#define unk_20FB03 RAM8(unk_20FB03_ADDRESS)
#define unk_20FB04_ADDRESS 0x20FB04u
#define unk_20FB04 RAM8(unk_20FB04_ADDRESS)
#define unk_20FB05_ADDRESS 0x20FB05u
#define unk_20FB05 RAM8(unk_20FB05_ADDRESS)
#define unk_20FB06_ADDRESS 0x20FB06u
#define unk_20FB06 RAM8(unk_20FB06_ADDRESS)
#define unk_20FB07_ADDRESS 0x20FB07u
#define unk_20FB07 RAM8(unk_20FB07_ADDRESS)
#define unk_20FB08_ADDRESS 0x20FB08u
#define unk_20FB08 RAM8(unk_20FB08_ADDRESS)
#define unk_20FB09_ADDRESS 0x20FB09u
#define unk_20FB09 RAM8(unk_20FB09_ADDRESS)
#define unk_20FB0A_ADDRESS 0x20FB0Au
#define unk_20FB0A RAM8(unk_20FB0A_ADDRESS)
#define unk_20FB0B_ADDRESS 0x20FB0Bu
#define unk_20FB0B RAM16(unk_20FB0B_ADDRESS)
#define unk_20FB0D_ADDRESS 0x20FB0Du
#define unk_20FB0D RAM32(unk_20FB0D_ADDRESS)
#define unk_20FB80_ADDRESS 0x20FB80u
#define unk_20FB80 RAM8(unk_20FB80_ADDRESS)
#define unk_20FB81_ADDRESS 0x20FB81u
#define unk_20FB81 RAM8(unk_20FB81_ADDRESS)
#define unk_20FB82_ADDRESS 0x20FB82u
#define unk_20FB82 RAM8(unk_20FB82_ADDRESS)
#define unk_20FC03_ADDRESS 0x20FC03u
#define unk_20FC03 RAM8(unk_20FC03_ADDRESS)
#define unk_20FC04_ADDRESS 0x20FC04u
#define unk_20FC04 RAM8(unk_20FC04_ADDRESS)
#define unk_20FC05_ADDRESS 0x20FC05u
#define unk_20FC05 RAM8(unk_20FC05_ADDRESS)
#define unk_20FC06_ADDRESS 0x20FC06u
#define unk_20FC06 RAM8(unk_20FC06_ADDRESS)
#define unk_20FC07_ADDRESS 0x20FC07u
#define unk_20FC07 RAM8(unk_20FC07_ADDRESS)
#define unk_20FC08_ADDRESS 0x20FC08u
#define unk_20FC08 RAM8(unk_20FC08_ADDRESS)
#define unk_20FC09_ADDRESS 0x20FC09u
#define unk_20FC09 RAM8(unk_20FC09_ADDRESS)
#define unk_20FC0A_ADDRESS 0x20FC0Au
#define unk_20FC0A RAM8(unk_20FC0A_ADDRESS)
#define unk_20FC0B_ADDRESS 0x20FC0Bu
#define unk_20FC0B ((uint8_t *)ram(unk_20FC0B_ADDRESS))
#define unk_20FC0D_ADDRESS 0x20FC0Du
#define unk_20FC0D RAM32(unk_20FC0D_ADDRESS)
#define unk_20FC1A_ADDRESS 0x20FC1Au
#define unk_20FC1A RAM16(unk_20FC1A_ADDRESS)
#define unk_20FC1C_ADDRESS 0x20FC1Cu
#define unk_20FC1C RAM8(unk_20FC1C_ADDRESS)
#define unk_20FC1D_ADDRESS 0x20FC1Du
#define unk_20FC1D RAM8(unk_20FC1D_ADDRESS)
#define unk_20FC1E_ADDRESS 0x20FC1Eu
#define unk_20FC1E RAM8(unk_20FC1E_ADDRESS)
#define unk_20FC46_ADDRESS 0x20FC46u
#define unk_20FC46 ((uint16_t *)ram(unk_20FC46_ADDRESS))
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
#define rom_00E0A0_ADDRESS 0x00E0A0u
#define rom_00E0A0 ((const uint32_t *)rom(rom_00E0A0_ADDRESS))
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
#define typed_messages_ADDRESS 0x019576u
#define typed_messages ((const uint32_t *)rom(typed_messages_ADDRESS))
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
#define SOUND_700700 0x700700u
#define SOUND_700710 0x700710u
#define SOUND_700712 0x700712u
#define SOUND_700800 0x700800u
#define SOUND_700810 0x700810u
#define SOUND_700812 0x700812u
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
	uint8_t unk_00;
	uint8_t unk_01;
	uint16_t unk_02;
	uint32_t unk_04;
	uint32_t unk_08;
	uint8_t unk_0C;
	uint8_t unk_0D;
	uint8_t unk_0E;
	uint8_t unk_0F;
	uint8_t unk_10;
	uint8_t unk_11;
	uint8_t unk_12;
	uint8_t unk_13;
	uint32_t unk_14;
	uint32_t unk_18;
	uint16_t unk_1C;
	uint8_t unk_1E;
	uint8_t pad_1F[0x1];
	uint16_t unk_20;
	uint8_t unk_22;
	uint8_t unk_23;
	uint16_t unk_24;
	uint8_t pad_26[0x2];
	uint32_t unk_28;
	uint16_t unk_2C;
	uint16_t unk_2E;
	uint16_t unk_30;
	uint16_t unk_32;
	uint16_t unk_34;
	uint16_t unk_36;
	uint16_t unk_38;
	uint16_t unk_3A;
	uint16_t unk_3C;
	uint8_t unk_3E;
	uint8_t unk_3F;
	uint8_t unk_40;
	uint8_t unk_41;
	uint8_t unk_42;
	uint8_t unk_43;
	uint8_t unk_44;
	uint8_t unk_45;
	uint8_t pad_46[0xA];
	uint32_t unk_50;
	uint32_t unk_54;
	uint16_t unk_58;
	uint8_t pad_5A[0x2];
	uint32_t unk_5C;
	uint32_t unk_60;
	uint32_t unk_64;
	uint32_t unk_68;
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
	uint8_t pad_D3[0x6];
	uint8_t unk_D9;
	uint8_t unk_DA;
	uint8_t unk_DB;
	uint8_t unk_DC;
	uint8_t pad_DD[0x3];
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

struct car
{
	uint32_t unk_00;
	uint32_t unk_04;
	uint32_t unk_08;
	uint8_t pad_0C[0x6];
	uint16_t unk_12;
	uint8_t unk_14;
	uint8_t pad_15[0xB];
	uint32_t unk_20;
	uint32_t unk_24;
	uint32_t unk_28;
	uint16_t unk_2C;
	uint16_t unk_2E;
	uint16_t unk_30;
	uint16_t unk_32;
	uint32_t unk_34;
	uint16_t unk_38;
	uint16_t unk_3A;
	uint32_t unk_3C;
	uint16_t unk_40;
	uint16_t unk_42;
	uint16_t unk_44;
	uint16_t unk_46;
	uint16_t unk_48;
	uint16_t unk_4A;
	uint16_t unk_4C;
	uint16_t unk_4E;
	uint32_t unk_50;
	uint32_t unk_54;
	uint32_t unk_58;
	uint8_t pad_5C[0x4];
	uint8_t unk_60;
	uint8_t unk_61;
	uint8_t unk_62;
	uint8_t unk_63;
	uint8_t unk_64;
	uint8_t unk_65;
	uint8_t unk_66;
	uint8_t unk_67;
	uint16_t unk_68;
	uint16_t unk_6A;
	uint8_t pad_6C[0x4];
	uint16_t unk_70;
	uint8_t pad_72[0x2];
	uint16_t unk_74;
	uint8_t pad_76[0x6];
	uint8_t unk_7C;
	uint8_t pad_7D[0x1];
	uint16_t unk_7E;
	uint32_t unk_80;
	uint32_t unk_84;
	uint32_t unk_88;
	uint8_t pad_8C[0x4];
	uint32_t unk_90;
	uint32_t unk_94;
	uint32_t unk_98;
	uint8_t unk_9C;
	uint8_t unk_9D;
	uint8_t pad_9E[0x2];
	uint32_t unk_A0;
	uint16_t unk_A4;
	uint16_t unk_A6;
	uint16_t unk_A8;
	uint16_t unk_AA;
	uint16_t unk_AC;
	uint16_t unk_AE;
	uint32_t unk_B0;
	uint32_t unk_B4;
	uint16_t unk_B8;
	uint16_t unk_BA;
	uint16_t unk_BC;
	uint16_t unk_BE;
	uint16_t unk_C0;
	uint16_t unk_C2;
	uint8_t pad_C4[0x5];
	uint8_t unk_C9;
	uint8_t unk_CA;
	uint8_t unk_CB;
	uint8_t unk_CC;
	uint8_t unk_CD;
	uint8_t unk_CE;
	uint8_t pad_CF[0x1];
	uint8_t unk_D0;
	uint8_t unk_D1;
	uint8_t pad_D2[0xA];
	uint32_t unk_DC;
	uint32_t unk_E0;
	uint32_t unk_E4;
	uint32_t unk_E8;
	uint32_t unk_EC;
	uint32_t unk_F0;
	uint32_t unk_F4;
	uint32_t unk_F8;
	uint32_t unk_FC;
	uint16_t unk_100;
	uint16_t unk_102;
	uint16_t unk_104;
	uint16_t unk_106;
	uint16_t unk_108;
	uint16_t unk_10A;
	uint16_t unk_10C;
	uint16_t unk_10E;
	uint16_t unk_110;
	uint16_t unk_112;
	uint8_t pad_114[0xC];
	uint16_t unk_120;
	uint16_t unk_122;
	uint8_t pad_124[0x2];
	uint16_t unk_126;
	uint16_t unk_128;
	uint16_t unk_12A;
	uint16_t unk_12C;
	uint8_t pad_12E[0x2];
	uint32_t unk_130;
	uint8_t unk_134;
	uint8_t unk_135;
	uint8_t unk_136;
	uint8_t unk_137;
	uint8_t pad_138[0x8];
	uint8_t unk_140;
	uint8_t unk_141;
	uint8_t unk_142;
	uint8_t pad_143[0x1];
	uint8_t unk_144;
	uint8_t unk_145;
	uint8_t unk_146;
	uint8_t unk_147;
	uint32_t unk_148;
	uint32_t unk_14C;
	uint8_t pad_150[0x30];
	uint8_t unk_180;
	uint8_t unk_181;
	uint8_t unk_182;
	uint8_t pad_183[0x7D];
};

struct screen
{
	uint8_t unk_00;
	uint8_t unk_01;
	uint8_t unk_02;
	uint8_t pad_03[0x1];
	uint32_t unk_04;
	uint32_t unk_08;
	uint32_t unk_0C;
	uint16_t unk_10;
	uint16_t unk_12;
	uint16_t unk_14;
	uint16_t unk_16;
	uint16_t unk_18;
	uint16_t unk_1A;
	uint8_t pad_1C[0x4];
	uint32_t unk_20;
	uint32_t unk_24;
	uint8_t pad_28[0x18];
	uint8_t unk_40;
	uint8_t unk_41;
	uint8_t pad_42[0x3];
	uint8_t unk_45;
	uint8_t unk_46;
	uint8_t unk_47;
	uint8_t unk_48;
	uint8_t pad_49[0x17];
	uint32_t unk_60;
	uint32_t unk_64;
	uint32_t unk_68;
	uint32_t unk_6C;
	uint8_t unk_70;
	uint8_t unk_71;
	uint8_t unk_72;
	uint8_t unk_73;
	uint8_t pad_74[0xC];
	uint32_t unk_80;
	uint8_t pad_84[0x4];
	uint16_t unk_88;
	uint8_t pad_8A[0x6];
	uint32_t unk_90;
	uint32_t unk_94;
	uint32_t unk_98;
	uint8_t unk_9C;
	uint8_t unk_9D;
	uint8_t unk_9E;
	uint8_t pad_9F[0x1];
	uint8_t unk_A0;
	uint8_t unk_A1;
	uint8_t unk_A2;
	uint8_t unk_A3;
	uint8_t unk_A4;
	uint8_t unk_A5;
	uint8_t unk_A6;
	uint8_t unk_A7;
	uint8_t pad_A8[0x8];
	uint16_t unk_B0;
	uint8_t pad_B2[0x2];
	uint32_t unk_B4;
	uint16_t unk_B8;
	uint8_t pad_BA[0x6];
	uint32_t unk_C0;
	uint32_t unk_C4;
	uint8_t pad_C8[0x8];
	uint32_t unk_D0;
	uint8_t unk_D4;
	uint8_t pad_D5[0xC];
	uint8_t unk_E1;
	uint8_t unk_E2;
	uint8_t pad_E3[0x1D];
	uint8_t unk_100;
	uint8_t unk_101;
	uint8_t unk_102;
	uint8_t pad_103[0x5];
	uint8_t unk_108;
	uint8_t unk_109;
	uint8_t pad_10A[0x6];
	uint32_t unk_110;
	uint32_t unk_114;
	uint32_t unk_118;
	uint32_t unk_11C;
	uint8_t pad_120[0x30];
	uint8_t unk_150;
	uint8_t unk_151;
	uint8_t unk_152;
	uint8_t unk_153;
	uint8_t unk_154;
	uint8_t pad_155[0x4A];
	uint8_t unk_19F;
	uint8_t pad_1A0[0x60];
	uint16_t unk_200;
	uint8_t unk_202;
	uint8_t unk_203;
	uint8_t unk_204;
	uint8_t unk_205;
	uint8_t unk_206;
	uint8_t unk_207;
	uint32_t unk_208;
	uint16_t unk_20C;
	uint8_t pad_20E[0xF2];
	uint32_t unk_300;
	uint32_t unk_304;
	uint32_t unk_308;
	uint8_t unk_30C;
	uint8_t pad_30D[0x2];
	uint8_t unk_30F;
	uint32_t unk_310;
	uint32_t unk_314;
	uint8_t unk_318;
	uint8_t unk_319;
	uint8_t unk_31A;
	uint8_t unk_31B;
	uint8_t unk_31C;
	uint8_t pad_31D[0x2];
	uint8_t unk_31F;
	uint8_t pad_320[0x22];
	uint8_t unk_342;
	uint8_t pad_343[0x6];
	uint8_t unk_349;
	uint8_t unk_34A;
	uint8_t pad_34B[0x5];
	uint16_t unk_350;
	uint8_t unk_352;
	uint8_t pad_353[0x15C];
	uint8_t unk_4AF;
	uint8_t unk_4B0;
	uint8_t pad_4B1[0x41];
	uint8_t unk_4F2;
	uint8_t pad_4F3[0xD];
	uint32_t unk_500;
	uint8_t pad_504[0xC];
	uint32_t unk_510;
	uint8_t pad_514[0x1EC];
	uint16_t unk_700;
	uint8_t pad_702[0x3C];
	uint16_t unk_73E;
	uint8_t pad_740[0xC0];
	uint8_t unk_800;
	uint8_t unk_801;
	uint8_t pad_802[0x20];
	uint8_t unk_822;
	uint8_t pad_823[0x1D];
	uint8_t unk_840;
	uint8_t unk_841;
	uint8_t pad_842[0xBE];
	uint32_t unk_900;
	uint8_t pad_904[0x1FC];
	uint16_t unk_B00;
	uint8_t unk_B02;
	uint8_t unk_B03;
	uint32_t unk_B04;
	uint32_t unk_B08;
	uint32_t unk_B0C;
	uint8_t unk_B10;
	uint8_t unk_B11;
	uint8_t unk_B12;
	uint8_t pad_B13[0x3ED];
	uint32_t unk_F00;
	uint32_t unk_F04;
	uint32_t unk_F08;
	uint8_t pad_F0C[0x2];
	uint8_t unk_F0E;
	uint8_t unk_F0F;
	uint16_t unk_F10;
	uint8_t pad_F12[0x2];
	uint32_t unk_F14;
	uint32_t unk_F18;
	uint32_t unk_F1C;
	uint16_t unk_F20;
	uint8_t unk_F22;
	uint8_t unk_F23;
	uint8_t unk_F24;
	uint8_t unk_F25;
	uint8_t pad_F26[0xA];
	uint32_t unk_F30;
	uint8_t unk_F34;
	uint8_t unk_F35;
	uint8_t pad_F36[0xA];
	uint32_t unk_F40;
	uint32_t unk_F44;
	uint8_t unk_F48;
	uint8_t unk_F49;
	uint8_t pad_F4A[0x6];
	uint16_t unk_F50;
	uint16_t unk_F52;
	uint32_t unk_F54;
	uint16_t unk_F58;
	uint8_t pad_F5A[0x26];
	uint32_t unk_F80;
	uint16_t unk_F84;
	uint8_t pad_F86[0x20];
	uint16_t unk_FA6;
	uint8_t unk_FA8;
	uint8_t pad_FA9[0xF57];
	uint8_t unk_1F00;
	uint8_t pad_1F01[0x2F];
	uint16_t unk_1F30;
	uint8_t pad_1F32[0x3746];
	uint16_t unk_5678;
	uint16_t unk_567A;
	uint8_t pad_567C[0x184];
	uint32_t unk_5800;
	uint16_t unk_5804;
	uint16_t unk_5806;
	uint8_t pad_5808[0x27F8];
};

#pragma pack(pop)

_Static_assert(sizeof(struct object) == 0x100, "struct object has the size of the game's blocks");
_Static_assert(sizeof(struct car) == 0x200, "struct car has the size of the game's blocks");
_Static_assert(sizeof(struct screen) == 0x8000, "struct screen has the size of the game's blocks");

#define OBJECT(address) ((struct object *)ram(address))
#define CAR(address) ((struct car *)ram(address))
#define SCREEN(address) ((struct screen *)ram(address))

#endif
