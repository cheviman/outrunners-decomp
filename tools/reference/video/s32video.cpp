/**
 *    Name:        s32video.cpp
 *    Description: reference Multi 32 video: MAME's segas32_v.cpp drawing the board model's two screens
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    MAME's file (one line fixed, see tools/reference/mame/LICENSE) is compiled here inside a namespace of
 *    its own, over the stand-ins of this folder (emu.h, segas32.h), so that its names keep away from the
 *    V60 core's stand-ins in the same program.
 *    The board model is the only state: before the sprite list is drawn and before each frame, the
 *    memories it reads are copied from s32_board into the video's own.
 */
#include "s32video.h"

#include "s32/machine.h"

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace s32v
{
#include "../mame/sega/segas32_v.cpp"
}

namespace
{

s32v::segas32_state *g_video = nullptr;
s32v::screen_device g_screen[S32V_SCREENS];
s32v::bitmap_rgb32 g_frame[S32V_SCREENS];
std::vector<uint16_t> g_video_ram(S32_VIDEO_RAM_SIZE / 2);
std::vector<uint16_t> g_sprite_ram(S32_SPRITE_RAM_SIZE / 2);
std::vector<uint16_t> g_palette[S32V_SCREENS] = {
	std::vector<uint16_t>(S32_PALETTE_ENTRIES), std::vector<uint16_t>(S32_PALETTE_ENTRIES) };
std::vector<uint32_t> g_sprite_rom;
int g_width[S32V_SCREENS] = { 320, 320 };
bool g_draw_screens = true;

void load_sprite_ram()
{
	std::memcpy(g_sprite_ram.data(), s32_board.sprite_ram, S32_SPRITE_RAM_SIZE);
	// the 32-bit copy spriteram_w keeps up to date as the CPU writes
	for (size_t offset = 0; offset < g_sprite_ram.size(); offset += 2)
		g_video->m_spriteram_32bit[offset >> 1] =
			((g_sprite_ram[offset | 1] >> 8) & 0x000000ff) |
			((g_sprite_ram[offset | 1] << 8) & 0x0000ff00) |
			((uint32_t(g_sprite_ram[offset]) << 8) & 0x00ff0000) |
			((uint32_t(g_sprite_ram[offset]) << 24) & 0xff000000);
}

// the board sends the sprite list: MAME's update_sprites past its automatic mode, which the board ran
// (the controls it latched are the ones sprite_swap_buffers latches)
void on_sprites_due(bool erase, bool draw)
{
	load_sprite_ram();
	std::memcpy(g_video->m_sprite_control, s32_board.sprite_control_latched, sizeof(g_video->m_sprite_control));
	if (erase)
		g_video->sprite_erase_buffer();
	if (draw)
	{
		g_video->sprite_swap_buffers();
		g_video->sprite_render_list();
	}
}

// VBLANK starts: the frames on the two screens are drawn from the memories as they are (MAME's screen
// updates, the left screen's first)
void on_frame_shown()
{
	if (!g_draw_screens)
		return;
	std::memcpy(g_video_ram.data(), s32_board.video_ram, S32_VIDEO_RAM_SIZE);
	s32v::video_memory_generation++;
	for (int screen = 0; screen < S32V_SCREENS; screen++)
	{
		std::memcpy(g_palette[screen].data(), s32_board.palette[screen], sizeof(s32_board.palette[screen]));
		for (int index = 0; index < S32_MIXER_REGISTERS; index++)
			g_video->m_mixer_control[screen][index] = s32_board.mixer[screen][index];
		g_video->m_system32_displayenable[screen] = s32_display_enabled(screen);    // its I/O chip's CNT1
	}
	g_video->m_system32_tilebank_external = s32_board.tilebank_external;
	load_sprite_ram();
	for (int screen = 0; screen < S32V_SCREENS; screen++)
	{
		g_width[screen] = (g_video_ram[0x1ff00 / 2] & 0x8000) ? 416 : 320;
		g_screen[screen].set_visible_area(0, g_width[screen] - 1, 0, S32V_HEIGHT - 1);
		if (screen == 0)
			g_video->screen_update_multi32_left(g_screen[0], g_frame[0], g_screen[0].visible_area());
		else
			g_video->screen_update_multi32_right(g_screen[1], g_frame[1], g_screen[1].visible_area());
	}
}

}    // namespace

extern "C" void s32v_attach(const uint8_t *tiles, size_t tiles_size, const uint8_t *sprites, size_t sprites_size)
{
	g_video = new s32v::segas32_state();
	g_video->m_videoram.pointer = g_video_ram.data();
	g_video->m_videoram.count = g_video_ram.size();
	g_video->m_spriteram.pointer = g_sprite_ram.data();
	g_video->m_spriteram.count = g_sprite_ram.size();
	for (int screen = 0; screen < S32V_SCREENS; screen++)
	{
		g_video->m_paletteram[screen].pointer = g_palette[screen].data();
		g_video->m_paletteram[screen].count = g_palette[screen].size();
	}
	// the sprite ROMs as 32-bit words in the host's order
	g_sprite_rom.resize(sprites_size / 4);
	for (size_t index = 0; index < g_sprite_rom.size(); index++)
		g_sprite_rom[index] = uint32_t(sprites[index * 4]) << 24 | uint32_t(sprites[index * 4 + 1]) << 16 |
			uint32_t(sprites[index * 4 + 2]) << 8 | sprites[index * 4 + 3];
	g_video->m_sprite_region.pointer = g_sprite_rom.data();
	g_video->m_sprite_region.count = g_sprite_rom.size();
	s32v::gfx_element &tile_gfx = *g_video->m_gfxdecode->gfx(0);
	tile_gfx.data = tiles;
	tile_gfx.size = tiles_size;
	s32v::gfx_element &text_gfx = *g_video->m_gfxdecode->gfx(1);
	text_gfx.width = text_gfx.height = 8;
	text_gfx.text = true;
	text_gfx.data = reinterpret_cast<const uint8_t *>(g_video_ram.data());
	text_gfx.size = S32_VIDEO_RAM_SIZE;
	g_video->device_start();
	for (int screen = 0; screen < S32V_SCREENS; screen++)
	{
		g_screen[screen].owner_machine = &g_video->machine();
		g_frame[screen].allocate(S32V_MAX_WIDTH, S32V_HEIGHT);
	}
	s32_board.sprites_due = on_sprites_due;
	s32_board.frame_shown = on_frame_shown;
}

extern "C" void s32v_draw_screens(bool on)
{
	g_draw_screens = on;
}

extern "C" const uint32_t *s32v_frame(int screen, int *width)
{
	if (width)
		*width = g_width[screen];
	return g_frame[screen].raw();
}

extern "C" uint64_t s32v_frame_hash(void)
{
	uint64_t hash = 0xcbf29ce484222325ull;
	for (int screen = 0; screen < S32V_SCREENS; screen++)
		for (int y = 0; y < S32V_HEIGHT; y++)
			for (int x = 0; x < g_width[screen]; x++)
			{
				const uint32_t pixel = g_frame[screen].pix(y, x);
				for (int shift = 0; shift < 24; shift += 8)
					hash = (hash ^ ((pixel >> shift) & 0xff)) * 0x100000001b3ull;
			}
	return hash;
}
