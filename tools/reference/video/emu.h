/**
 *    Name:        emu.h
 *    Description: minimal stand-in for MAME's emu.h, enough to build MAME's System 32 video (segas32_v.cpp) alone
 *    Version:     0.1.0
 *    Created:     13-10-2026
 *    Modified:    13-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Only what segas32_v.cpp uses, with MAME's meaning: rectangles, 16- and 32-bit bitmaps, tilemaps
 *    (a page of 16x16 tiles from the tile ROM, or the 8x8 text cells decoded from video RAM, drawn into
 *    a pixmap of palette-base + pen values the way MAME's tilemap.cpp does), the screen's visible area,
 *    timers the host fires, and no-ops for saving state, profiling and logging. s32video.cpp includes
 *    this inside its own namespace after the standard headers; it includes none itself.
 */
#pragma once

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using offs_t = uint32_t;
using pen_t = uint32_t;

#define ATTR_COLD
#define NAME(x) x, #x
#define FUNC(x) &x, #x
#define BIT(x, n) (((x) >> (n)) & 1)
#define COMBINE_DATA(varptr) (*(varptr) = (*(varptr) & ~mem_mask) | (data & mem_mask))

template <typename... Args> inline void osd_printf_debug(const char *, Args...) {}
template <typename... Args> inline void logerror(const char *, Args...) {}
template <typename... Args> inline void popmessage(const char *, Args...) {}

template <typename T> inline std::unique_ptr<T> make_unique_clear(size_t count)
{
	using element = typename std::remove_extent<T>::type;
	std::unique_ptr<T> found(new element[count]);
	std::memset(found.get(), 0, sizeof(element) * count);
	return found;
}

namespace util
{
	// sign-extends the low 'bits' bits of a value, into the signed type of its size (as MAME's coretmpl.h)
	template <typename T> constexpr typename std::make_signed<T>::type sext(T value, unsigned bits)
	{
		using wide = typename std::make_signed<T>::type;
		const unsigned shift = sizeof(T) * 8 - bits;
		return wide(wide(value << shift) >> shift);
	}

	// the host is little endian, like the V60's bus: the bytes are where they are
	template <typename T, typename U> inline T *little_endian_cast(U *pointer)
	{
		return reinterpret_cast<T *>(pointer);
	}
}

// ---- profiling, input, save states: nothing ----

enum { PROFILER_USER1, PROFILER_USER2, PROFILER_USER3 };
struct profiler_scope {};
struct profiler_stub
{
	profiler_scope start(int) { return profiler_scope(); }
};
inline profiler_stub g_profiler;

enum { KEYCODE_Q, KEYCODE_W, KEYCODE_E, KEYCODE_R, KEYCODE_T, KEYCODE_Y, KEYCODE_L, KEYCODE_Z, KEYCODE_X,
       KEYCODE_V, KEYCODE_B, KEYCODE_N, KEYCODE_M };
struct input_manager
{
	bool code_pressed(int) const { return false; }
};

struct device_missing_dependencies {};

// ---- geometry ----

class rectangle
{
public:
	int min_x = 0, max_x = 0, min_y = 0, max_y = 0;

	rectangle() = default;
	rectangle(int left, int right, int top, int bottom) : min_x(left), max_x(right), min_y(top), max_y(bottom) {}

	int left() const { return min_x; }
	int right() const { return max_x; }
	int top() const { return min_y; }
	int bottom() const { return max_y; }
	int width() const { return max_x + 1 - min_x; }
	int height() const { return max_y + 1 - min_y; }
	bool empty() const { return min_x > max_x || min_y > max_y; }
	bool contains(int x, int y) const { return x >= min_x && x <= max_x && y >= min_y && y <= max_y; }
	bool containsx(int x) const { return x >= min_x && x <= max_x; }
	bool containsy(int y) const { return y >= min_y && y <= max_y; }
	void set(int left, int right, int top, int bottom) { min_x = left; max_x = right; min_y = top; max_y = bottom; }
	void setx(int left, int right) { min_x = left; max_x = right; }
	void inset(int left, int right, int top, int bottom) { min_x += left; max_x -= right; min_y += top; max_y -= bottom; }
	rectangle &operator&=(const rectangle &other)
	{
		if (other.min_x > min_x) min_x = other.min_x;
		if (other.max_x < max_x) max_x = other.max_x;
		if (other.min_y > min_y) min_y = other.min_y;
		if (other.max_y < max_y) max_y = other.max_y;
		return *this;
	}
};

inline void sect_rect(rectangle *target, const rectangle *other) { *target &= *other; }

// ---- bitmaps: rows of 'width' pixels ----

template <typename T> class bitmap_t
{
public:
	void allocate(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_pixels.assign(size_t(width) * height, T(0));
	}
	int width() const { return m_width; }
	int height() const { return m_height; }
	rectangle cliprect() const { return rectangle(0, m_width - 1, 0, m_height - 1); }
	T &pix(int y, int x = 0) { return m_pixels[size_t(y) * m_width + x]; }
	const T &pix(int y, int x = 0) const { return m_pixels[size_t(y) * m_width + x]; }
	void fill(T value) { std::fill(m_pixels.begin(), m_pixels.end(), value); }
	void fill(T value, const rectangle &area)
	{
		rectangle clip = area;
		clip &= cliprect();
		for (int y = clip.top(); y <= clip.bottom(); y++)
			for (int x = clip.left(); x <= clip.right(); x++)
				pix(y, x) = value;
	}
	void plot(bitmap_t &, int x, int y, T value) { if (cliprect().contains(x, y)) pix(y, x) = value; }
	T *raw() { return m_pixels.data(); }

private:
	int m_width = 0;
	int m_height = 0;
	std::vector<T> m_pixels;
};

using bitmap_ind16 = bitmap_t<uint16_t>;
using bitmap_rgb32 = bitmap_t<uint32_t>;

// ---- time and timers: the host says when a timer fires ----

struct attotime
{
	int64_t usec = 0;
	static attotime from_usec(int64_t count) { attotime found; found.usec = count; return found; }
};

class emu_timer
{
public:
	std::function<void(int32_t)> callback;
	bool armed = false;
	int32_t param = 0;
	attotime delay;

	void adjust(attotime after, int32_t value = 0) { armed = true; param = value; delay = after; }
	void fire() { if (armed) { armed = false; callback(param); } }
};

#define TIMER_CALLBACK_MEMBER(name) void name(int32_t param)

// ---- graphics: the tile ROM's 16x16 tiles and the text layer's 8x8 cells ----

struct gfx_source
{
	const uint8_t *data = nullptr;    // the tile ROM, or video RAM for the text cells
	size_t size = 0;
};

class gfx_element
{
public:
	int width = 16, height = 16;
	const uint8_t *data = nullptr;
	size_t size = 0;
	bool text = false;                // 8x8 cells packed high nibble first (gfx_8x8x4_packed_msb)

	void mark_dirty(offs_t) {}        // decoded on demand: nothing is kept

	// the pen (0-15) of a tile's pixel, as MAME's gfx decoding lays it out; the code wraps around the
	// tiles there are, as tile_data::set takes it
	int pen(uint32_t code, int x, int y) const
	{
		if (text)
		{
			const size_t at = size_t(code % (size / 32)) * 32 + y * 4 + x / 2;
			const uint8_t value = data[at];
			return (x & 1) ? (value & 0x0F) : (value >> 4);
		}
		// bgcharlayout: rows of 8 bytes; x offsets 0,4,16,20,8,12,24,28,32,36,48,52,40,44,56,60 bits
		static const int byte_of[16] = { 0, 0, 2, 2, 1, 1, 3, 3, 4, 4, 6, 6, 5, 5, 7, 7 };
		const size_t at = size_t(code % (size / 128)) * 128 + y * 8 + byte_of[x];
		const uint8_t value = data[at];
		return (x & 1) ? (value & 0x0F) : (value >> 4);
	}
};

class gfxdecode_device
{
public:
	gfx_element elements[2];
	bool started() const { return true; }
	gfx_element *gfx(int index) { return &elements[index]; }
};

class palette_device
{
public:
	pen_t black_pen() const { return 0; }
};

// ---- tilemaps ----

class tilemap_t;

struct tile_data
{
	int gfx = 0;
	uint32_t code = 0;
	uint32_t color = 0;
	uint8_t flags = 0;
	void set(int gfxnum, uint32_t tilecode, uint32_t tilecolor, uint8_t tileflags)
	{
		gfx = gfxnum; code = tilecode; color = tilecolor; flags = tileflags;
	}
};

#define TILE_FLIPX 0x01
#define TILE_FLIPY 0x02
#define TILEMAP_FLIPX TILE_FLIPX
#define TILEMAP_FLIPY TILE_FLIPY
enum tilemap_mapper { TILEMAP_SCAN_ROWS };

#define TILE_GET_INFO_MEMBER(name) void name(tilemap_t &tilemap, tile_data &tileinfo, uint32_t tile_index)

class tilemap_get_info_delegate
{
public:
	std::function<void(tilemap_t &, tile_data &, uint32_t)> call;

	template <typename Owner, typename Method>
	tilemap_get_info_delegate(Owner &owner, Method method, const char *)
	{
		Owner *self = &owner;
		call = [self, method](tilemap_t &map, tile_data &info, uint32_t index) { (self->*method)(map, info, index); };
	}
};

class screen_device;

// what the host changes before each frame: a tilemap draws its pixmap again once per change
inline uint64_t video_memory_generation = 1;

class tilemap_t
{
public:
	tilemap_t(gfxdecode_device &decode, tilemap_get_info_delegate info, int tile_width, int tile_height,
			int columns, int rows)
		: m_decode(&decode), m_info(std::move(info)), m_tile_width(tile_width), m_tile_height(tile_height),
		  m_columns(columns), m_rows(rows)
	{
		m_pixmap.allocate(columns * tile_width, rows * tile_height);
	}

	void set_user_data(void *data) { m_user = data; }
	void *user_data() const { return m_user; }
	void set_transparent_pen(int pen) { m_transparent_pen = pen; }
	void set_flip(int flags) { m_flip = flags; }
	void mark_tile_dirty(offs_t) {}
	void mark_all_dirty() { m_generation = 0; }

	// every tile drawn again from what the callback says now (palette base + pen per pixel), once for each
	// state of the memories
	bitmap_ind16 &pixmap()
	{
		if (m_generation == video_memory_generation)
			return m_pixmap;
		m_generation = video_memory_generation;
		for (int row = 0; row < m_rows; row++)
			for (int column = 0; column < m_columns; column++)
			{
				tile_data info;
				m_info.call(*this, info, uint32_t(row * m_columns + column));
				const gfx_element &gfx = *m_decode->gfx(info.gfx);
				const uint32_t base = (info.color % 0x400) * 16;
				for (int y = 0; y < m_tile_height; y++)
					for (int x = 0; x < m_tile_width; x++)
					{
						const int sx = (info.flags & TILE_FLIPX) ? m_tile_width - 1 - x : x;
						const int sy = (info.flags & TILE_FLIPY) ? m_tile_height - 1 - y : y;
						m_pixmap.pix(row * m_tile_height + y, column * m_tile_width + x) =
							uint16_t(base + gfx.pen(info.code, sx, sy));
					}
			}
		return m_pixmap;
	}

	// the tilemap over a bitmap with no scrolling: its pixels whose pen is not the transparent one
	void draw(screen_device &, bitmap_ind16 &bitmap, const rectangle &cliprect, int, int)
	{
		bitmap_ind16 &source = pixmap();
		rectangle clip = cliprect;
		clip &= bitmap.cliprect();
		for (int y = clip.top(); y <= clip.bottom(); y++)
			for (int x = clip.left(); x <= clip.right(); x++)
			{
				const int sx = (m_flip & TILEMAP_FLIPX) ? source.width() - 1 - x : x;
				const int sy = (m_flip & TILEMAP_FLIPY) ? source.height() - 1 - y : y;
				if (sx < 0 || sy < 0 || sx >= source.width() || sy >= source.height())
					continue;
				const uint16_t value = source.pix(sy, sx);
				if ((value & 0x0F) != m_transparent_pen)
					bitmap.pix(y, x) = value;
			}
	}

private:
	gfxdecode_device *m_decode;
	tilemap_get_info_delegate m_info;
	int m_tile_width, m_tile_height, m_columns, m_rows;
	void *m_user = nullptr;
	int m_transparent_pen = -1;
	int m_flip = 0;
	uint64_t m_generation = 0;
	bitmap_ind16 m_pixmap;
};

class tilemap_manager
{
public:
	tilemap_t &create(gfxdecode_device &decode, tilemap_get_info_delegate info, tilemap_mapper, int tile_width,
			int tile_height, int columns, int rows)
	{
		m_maps.push_back(std::make_unique<tilemap_t>(decode, std::move(info), tile_width, tile_height, columns, rows));
		return *m_maps.back();
	}

private:
	std::vector<std::unique_ptr<tilemap_t>> m_maps;
};

class running_machine
{
public:
	tilemap_manager &tilemap() { return m_tilemaps; }
	input_manager &input() { return m_input; }

private:
	tilemap_manager m_tilemaps;
	input_manager m_input;
};

class screen_device
{
public:
	running_machine *owner_machine = nullptr;
	rectangle area = rectangle(0, 319, 0, 223);

	running_machine &machine() const { return *owner_machine; }
	const rectangle &visible_area() const { return area; }
	void set_visible_area(int left, int right, int top, int bottom) { area.set(left, right, top, bottom); }
};
