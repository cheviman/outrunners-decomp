"""
Name:        program_objects.py
Description: the program's lists of routines: start_object_list's handler lists, the DJ's remarks and the
             demo's captions
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Every format writes the typed format of tools/core/formats/typed.py (from_text is typed.from_text): 'record'
lines name the kinds of line a piece holds, each line after the keyword of its kind. An address is written
as the name of the place it reaches (a handler's function, NAME+0xNN inside a named piece), a count in
decimal. What a format cannot read as its records stays as 'bytes' lines, so every piece goes back whole.

The formats (one dictionary for every module of tools/formats, so their names start with program_):
- program_object_list: start_object_list's lists: 'count' the number of objects (a 32-bit word), then
  'object' a handler each, the new object's field 4 (a piece may hold the count, the handlers or both).
- program_dj_remark_lists (dj_remark_lists): car_radio_update's lists of DJ remarks: 'list' the address of
  each list by number (random_number picks one), then the lists: 'routine' each remark's routine
  (routine_list_jump runs the next one each time), 'end' (0).
- program_timeline: demo_captions_run's cues: 'cue' the frame (of the demo race's) and the routine it runs
  then; 'end', a frame of FFFF that no count reaches before the demo ends.
"""
import struct

from formats import typed
from formats.program_walk import Walk, address_table


def _address(value):
    """An address as the name of the place it reaches, else in hex."""
    return typed.name_for(value) or f"0x{value:08X}"


def _is_address(value):
    return typed.name_for(value) is not None


# ---- start_object_list's lists ----

def object_list_to_text(data):
    out = typed.Writer()
    out.comment("start_object_list's list: 'count' the number of objects, then 'object' the handler of each")
    out.record("count", 4, [typed.column("count", "u32")])
    out.record("object", 4, [typed.column("handler", "u32")])
    count = len(data) // 4
    for index in range(count):
        value = struct.unpack_from("<I", data, index * 4)[0]
        if _is_address(value):
            out.row(["object", _address(value)])
        else:
            out.row(["count", str(value)])
    out.raw(data[count * 4:])
    return out.text()


# ---- demo_captions_run's timeline ----

def timeline_to_text(data):
    out = typed.Writer()
    out.comment("demo_captions_run's cues: 'cue' FRAME ROUTINE, the routine run when the demo's frame count reaches")
    out.comment("FRAME (decimal); 'end', the frame FFFF")
    out.record("cue", 8, [typed.column("frame", "u32"), typed.column("routine", "u32")])
    out.record("end", 4, [typed.column("end", "u32", fixed=0xFFFF)])
    at = 0
    while at + 4 <= len(data):
        frame = struct.unpack_from("<I", data, at)[0]
        if frame == 0xFFFF:
            out.row(["end"])
            at += 4
            continue
        if at + 8 > len(data) or frame > 0xFFFF:
            break
        out.row(["cue", str(frame), _address(struct.unpack_from("<I", data, at + 4)[0])])
        at += 8
    out.raw(data[at:])
    return out.text()


# ---- the DJ's remarks ----

def _remark_list(walk, offset):
    rows, at = [], offset
    while walk.fits(at, 4):
        routine = walk.u32(at)
        at += 4
        if routine == 0:
            rows.append((["end"], None))
            return at - offset, rows
        rows.append((["routine", walk.place(routine)], None))
    return None


def dj_remark_lists_to_text(data, piece="dj_remark_lists"):
    walk = Walk(data, piece)
    out = typed.Writer()
    out.comment("car_radio_update's lists of DJ remarks: 'list' each list by number (+0xNN: a list's place in the")
    out.comment("piece), then the lists: 'routine' a remark's routine, 'end' (0)")
    out.record("list", 4, [typed.column("list", "u32")])
    out.record("routine", 4, [typed.column("routine", "u32")])
    out.record("end", 4, [typed.column("end", "u32", fixed=0)])
    walk.want_offset(0, "remark_lists")
    walk.run({"remark_lists": address_table("list", "remark_list"), "remark_list": _remark_list})
    walk.write(out)
    return out.text()


FORMATS = {
    "program_dj_remark_lists": (".txt", dj_remark_lists_to_text, typed.from_text),
    "program_object_list": (".txt", object_list_to_text, typed.from_text),
    "program_timeline": (".txt", timeline_to_text, typed.from_text),
}
