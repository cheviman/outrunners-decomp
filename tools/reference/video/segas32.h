/**
 *    Name:        segas32.h
 *    Description: the part of MAME's segas32_state that segas32_v.cpp uses, over the board model's memory
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    19-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The members, types and functions of MAME's sega/segas32.h (Aaron Giles) that the video code reads,
 *    with the same names, so that segas32_v.cpp builds as MAME has it. Video RAM, sprite RAM, palette RAM and
 *    the mixer are the host's (s32video.cpp points them at the board model); the tile and sprite ROMs too.
 */
#pragma once

// a share of 16-bit words the host owns
class word_share
{
public:
	uint16_t *pointer = nullptr;
	size_t count = 0;

	operator uint16_t *() const { return pointer; }
	uint16_t &operator[](size_t index) const { return pointer[index]; }
	size_t bytes() const { return count * 2; }
};

class byte_share
{
public:
	uint8_t *pointer = nullptr;
	size_t count = 0;

	explicit operator bool() const { return pointer != nullptr; }
	uint8_t &operator[](size_t index) const { return pointer[index]; }
	size_t bytes() const { return count; }
};

class long_region
{
public:
	const uint32_t *pointer = nullptr;
	size_t count = 0;

	size_t length() const { return count; }
	const uint32_t &operator[](size_t index) const { return pointer[index]; }
};

class segas32_state
{
public:
	segas32_state() = default;

	running_machine &machine() { return m_machine; }

	uint32_t screen_update_system32(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_multi32_left(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	uint32_t screen_update_multi32_right(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	void device_start();
	void system32_set_vblank(int state);

	uint16_t videoram_r(offs_t offset);
	void videoram_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);
	uint8_t sprite_control_r(offs_t offset);
	void sprite_control_w(offs_t offset, uint8_t data);
	uint16_t spriteram_r(offs_t offset);
	void spriteram_w(offs_t offset, uint16_t data, uint16_t mem_mask = ~0);

	TIMER_CALLBACK_MEMBER(end_of_vblank_int) {}
	TIMER_CALLBACK_MEMBER(update_sprites);

	struct layer_info
	{
		bitmap_ind16 bitmap;
		std::unique_ptr<uint8_t[]> transparent;
		int num = 0;
	};

	struct extents_list
	{
		uint8_t                   scan_extent[256]{};
		uint16_t                  extent[256][16]{};
	};

	struct cache_entry
	{
		cache_entry *             next = nullptr;
		tilemap_t *               tmap = nullptr;
		uint8_t                   page = 0;
		uint8_t                   bank = 0;
	};

	TILE_GET_INFO_MEMBER(get_tile_info);
	TILE_GET_INFO_MEMBER(get_text_tile_info);

	tilemap_t *find_cache_entry(int page, int bank);
	inline void get_tilemaps(int bgnum, tilemap_t **tilemaps);
	uint8_t update_tilemaps(screen_device &screen, const rectangle &cliprect);
	void sprite_erase_buffer();
	void sprite_swap_buffers();
	int draw_one_sprite(uint16_t const *data, int xoffs, int yoffs, const rectangle &clipin, const rectangle &clipout);
	void sprite_render_list();
	inline uint8_t compute_color_offsets(int which, int layerbit, int layerflag);
	inline uint16_t compute_sprite_blend(uint8_t encoding);
	inline uint16_t *get_layer_scanline(int layer, int scanline);
	void mix_all_layers(int which, int xoffs, bitmap_rgb32 &bitmap, const rectangle &cliprect, uint8_t enablemask);
	void print_mixer_data(int which);
	uint32_t multi32_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect, int index);
	bool compute_clipping_extents(screen_device &screen, bool enable, bool clipout, int clipmask, const rectangle &cliprect, extents_list *list);
	void compute_tilemap_flips(int bgnum, bool &flipx, bool &flipy);
	void update_tilemap_zoom(screen_device &screen, layer_info &layer, const rectangle &cliprect, int bgnum);
	void update_tilemap_rowscroll(screen_device &screen, layer_info &layer, const rectangle &cliprect, int bgnum);
	void update_tilemap_text(screen_device &screen, layer_info &layer, const rectangle &cliprect);
	void update_bitmap(screen_device &screen, layer_info &layer, const rectangle &cliprect);
	void update_background(layer_info &layer, const rectangle &cliprect);

	emu_timer *timer_alloc(void (segas32_state::*method)(int32_t), const char *, segas32_state *self)
	{
		m_timers.push_back(std::make_unique<emu_timer>());
		emu_timer *timer = m_timers.back().get();
		timer->callback = [self, method](int32_t param) { (self->*method)(param); };
		return timer;
	}
	template <typename... Args> void save_item(Args &&...) {}
	template <typename... Args> void save_pointer(Args &&...) {}

	// memory the host owns
	word_share m_videoram;
	word_share m_spriteram;
	byte_share m_soundram;
	word_share m_paletteram[2];
	long_region m_sprite_region;

	gfxdecode_device m_gfxdecode_device;
	gfxdecode_device *m_gfxdecode = &m_gfxdecode_device;
	palette_device m_palette_device;
	palette_device *m_palette = &m_palette_device;

	const bool m_is_multi32 = true;      // OutRunners' board, the Multi 32

	uint8_t m_v60_irq_control[0x10]{};
	uint8_t m_sound_irq_control[4]{};
	uint8_t m_sound_irq_input = 0;
	uint8_t m_sound_dummy_value = 0;
	uint16_t m_sound_bank = 0;

	// video-related
	uint16_t m_system32_displayenable[2]{};
	uint16_t m_system32_tilebank_external = 0;
	tilemap_t *m_text_tilemap = nullptr;
	std::unique_ptr<cache_entry[]> m_tmap_cache;
	cache_entry *m_cache_head = nullptr;
	layer_info m_layer_data[11];
	uint16_t m_mixer_control[2][0x40]{};
	std::unique_ptr<uint16_t[]> m_solid_0000;
	std::unique_ptr<uint16_t[]> m_solid_ffff;
	uint8_t m_sprite_render_count = 0;
	uint8_t m_sprite_control_latched[8]{};
	uint8_t m_sprite_control[8]{};
	std::unique_ptr<uint32_t[]> m_spriteram_32bit;
	std::unique_ptr<int32_t[]> m_prev_bgstartx;
	std::unique_ptr<int32_t[]> m_prev_bgendx;
	std::unique_ptr<int32_t[]> m_bgcolor_line;
	int m_print_count = 0;
	emu_timer *m_vblank_end_int_timer = nullptr;
	emu_timer *m_update_sprites_timer = nullptr;

private:
	running_machine m_machine;
	std::vector<std::unique_ptr<emu_timer>> m_timers;
};
