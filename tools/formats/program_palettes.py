"""
Name:        program_palettes.py
Description: the program's colours and palette numbers: upload_palette's colour blocks, the test mode's
             palettes, the lists of palettes objects step through, the screen fades' starts
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Every format writes the typed format of tools/core/formats/typed.py (from_text is typed.from_text). A colour
is #RRGGBB (five bits a component, scaled to eight), '+' after it when bit 15 is set: rgb555 as the palette
RAM holds it, rgb555alt in the palette window's alternate format (the low bit of blue, green and red in bits
14-12, the rest 4:4:4), which is what the window's upper half (8000-FFFF of 600000 and 680000) takes. What a
format cannot read as its records stays as 'bytes' lines, so every piece goes back whole.

The formats (one dictionary for every module of tools/formats, so their names start with program_):
- program_colour_blocks: upload_palette's (and queue_palette's) blocks: 'block' the place in the palette
  window (a byte offset: 8000 on is the alternate format's half) and the length in bytes, then its colours,
  16 a line, in the format the place takes.
- program_alternate_colours: colours in the alternate format, 16 a line (test_mode_set_up_colours copies
  them to 609E00 and 689E00).
- program_palette_steps: the palettes an object steps through, a frame each (step_palette, branch_arrow_update,
  title_logo_update): 'palette' a palette number (sprite_palettes), 'back' (negative) the bytes the list
  goes back to loop.
- program_screen_fade: step_screen_fade's start of a fade (screen_fade_start: level, step, end, channels).
"""
import struct

from formats import typed
from formats.program_walk import record_columns, record_size, value_text

COLOURS_PER_LINE = 16
ALTERNATE_HALF = 0x8000         # palette window offsets from here show the alternate format


def _colour_rows(out, data, kind):
    count = len(data) // 2
    for at in range(0, count, COLOURS_PER_LINE):
        values = struct.unpack_from(f"<{min(COLOURS_PER_LINE, count - at)}H", data, at * 2)
        out.row([typed.color_text(value, kind) for value in values], f"{at:X}")


# ---- upload_palette's colour blocks ----

def colour_blocks_to_text(data):
    out = typed.Writer()
    out.comment("colour blocks: 'block' PLACE LENGTH (the place in the palette window, the length in bytes), then")
    out.comment("its colours, 16 a line (# after a line: its first colour's number in the block, hex)")
    out.record("block", 4, [typed.column("place", "u16"), typed.column("length", "u16")])
    at, kind = 0, None
    while at + 4 <= len(data):
        place, length = struct.unpack_from("<2H", data, at)
        if not length or length % 2 or at + 4 + length > len(data):
            break
        block_kind = "rgb555alt" if place & ALTERNATE_HALF else "rgb555"
        if block_kind != kind:
            out.layout(block_kind)
            kind = block_kind
        out.row(["block", f"0x{place:04X}", f"0x{length:04X}"], f"+0x{at:X}: {length // 2} colours")
        _colour_rows(out, data[at + 4:at + 4 + length], kind)
        at += 4 + length
    out.raw(data[at:])
    return out.text()


def alternate_colours_to_text(data):
    out = typed.Writer()
    out.comment("colours in the palette window's alternate format, 16 a line (# after a line: its first colour's")
    out.comment("number, hex)")
    out.layout("rgb555alt")
    _colour_rows(out, data[:len(data) // 2 * 2], "rgb555alt")
    out.raw(data[len(data) // 2 * 2:])
    return out.text()


# ---- the palettes objects step through ----

def palette_steps_to_text(data):
    out = typed.Writer()
    out.comment("the palettes an object steps through, a frame each: 'palette' a palette number (sprite_palettes),")
    out.comment("'back' the bytes the list goes back to loop (negative)")
    out.record("palette", 2, [typed.column("palette", "u16")])
    out.record("back", 2, [typed.column("back", "s16")])
    at = 0
    while at + 2 <= len(data):
        step = struct.unpack_from("<h", data, at)[0]
        at += 2
        if step < 0:
            out.row(["back", str(step)], f"to +0x{at + step:X}")
            break
        out.row(["palette", f"0x{step:04X}"])
    out.raw(data[at:])
    return out.text()


# ---- the fades' starts ----

def screen_fade_to_text(data):
    out = typed.Writer()
    out.comment("step_screen_fade's start of a fade: the level, the step a frame, the level it ends at, the channels")
    out.comment("(E0 red, green and blue; 80 red)")
    columns = record_columns("screen_fade_start")
    size = record_size("screen_fade_start")
    out.layout(None, "screen_fade_start", size, [typed.column(name, kind) for name, _, kind in columns])
    if len(data) < size:
        out.raw(data)
        return out.text()
    out.row([value_text(struct.unpack_from("<" + typed.VALUE_TYPES[kind], data, offset)[0], kind)
             for _, offset, kind in columns])
    out.raw(data[size:])
    return out.text()


FORMATS = {
    "program_colour_blocks": (".txt", colour_blocks_to_text, typed.from_text),
    "program_alternate_colours": (".txt", alternate_colours_to_text, typed.from_text),
    "program_palette_steps": (".txt", palette_steps_to_text, typed.from_text),
    "program_screen_fade": (".txt", screen_fade_to_text, typed.from_text),
}
