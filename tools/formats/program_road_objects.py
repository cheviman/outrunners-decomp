"""
Name:        program_road_objects.py
Description: the objects along the roads: the courses' object streams and the records their objects start
             with, the stages' traffic lists and the traffic's records, the stages' background objects
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Every format writes the typed format of tools/core/formats/typed.py (from_text is typed.from_text), read by
program_walk: each piece starts with a table of addresses, of lists, whose entries point at records in the
same piece; each part goes under the line of its kind, in the order of their places, its first line after a
comment with its place (+0xNN). A place inside the piece is written NAME+0xNN (NAME the piece's), a handler
by its function's name. The records' columns are symbols.txt's structures (road_row_record, traffic_record...),
in the order of their bytes; at_NN columns are bytes no field names. Signed values are decimal, the others
hex. What no part covers stays as 'bytes' lines.

The formats (one dictionary for every module of tools/formats, so their names start with program_):
- program_road_object_streams (road_object_streams): 'stream' the stream of each course by number
  (road_object_stream_read: the stage's course); a stream's entries (road_object_stream_entry), by their
  position (segments from the stage's start): 'object' starts an object (its handler, and the parameter
  that goes into its field 18: most often the address of its record), 'stage_end' (command 1), 'end'
  (command 2), 'fork' (command 3) and after it 'branch' the stream of each branch; then the records, by
  the handler that reads them: 'road_row' (road_row_start: scenery_update, scenery_pair_update,
  fork_sign_update, overhead_scenery_update; road_row_start_copy: animated_scenery_update,
  animated_scenery_pair_update), 'flashing', 'moving', 'crosser' (road_crosser_update, kangaroo_update),
  'helicopter', 'start_gate', 'flagman' (start_flagman_update, flagman_update), 'balloon', 'sleigh',
  'parade' (goal_parade_*), 'sounding', 'aurora'; and the lists two of them cycle through, a step a frame:
  'palette' (flashing_scenery_palettes) and 'animation' (aurora_animations), each list closed by 'back' the
  bytes it goes back (negative) to its start.
- program_stage_road_object_lists (stage_road_object_lists): 'list' the traffic list of each stage, then
  the lists: 'traffic' a traffic_spawn_entry (position, level, handler, stop distance, record: the
  traffic_record in traffic_records), 'end' (8000 FFFF) after the last.
- program_traffic_records (traffic_records): what the traffic lists' entries point at: 'sprite' a
  traffic_sprite_record (animation, palette, the sounds, weight, hit box), 'traffic' a traffic_record (its
  sprite record, side, x offset, speed, and two bytes nothing reads), 'line' a traffic leader's
  traffic_line_record (the spacing), then 'follower' the traffic_record of each vehicle behind it and
  'followers_end' (0).
- program_stage_background_objects (stage_background_object_lists): 'list' the background objects of each
  stage (race_background_start_objects; 31 on, the ones of a change within the stage), then the lists:
  'length' the length of its pairs in bytes, 'object' a handler and its data; then the data by handler:
  'backdrop' (backdrop_near_start, backdrop_translucent_start, backdrop_far_start, backdrop_flashing_start:
  12 bytes of backdrop_record), 'landmark' (backdrop_landmark_start: 16), 'palette_cycle'
  (palette_cycle_update), 'sunset_fade' (sunset_fade_start).
"""
from formats import typed
from formats.program_walk import Walk, address_table, declare, record_cells, record_columns, record_size, \
    value_text

# ---- road_object_streams ----

STREAM_RECORDS = {      # handler -> (keyword, structure, columns that hold addresses: their kind)
    "scenery_update": ("road_row", "road_row_record", {}),
    "scenery_pair_update": ("road_row", "road_row_record", {}),
    "fork_sign_update": ("road_row", "road_row_record", {}),
    "overhead_scenery_update": ("road_row", "road_row_record", {}),
    "animated_scenery_update": ("road_row", "road_row_record", {}),
    "animated_scenery_pair_update": ("road_row", "road_row_record", {}),
    "flashing_scenery_update": ("flashing", "flashing_scenery_record", {"palettes": "palettes"}),
    "moving_scenery_update": ("moving", "moving_scenery_record", {}),
    "road_crosser_update": ("crosser", "road_crosser_record", {}),
    "kangaroo_update": ("crosser", "road_crosser_record", {}),
    "swooping_helicopter_update": ("helicopter", "swooping_helicopter_record", {}),
    "start_gate_update": ("start_gate", "start_gate_record", {}),
    "start_flagman_update": ("flagman", "flagman_record", {}),
    "flagman_update": ("flagman", "flagman_record", {}),
    "start_balloon_update": ("balloon", "start_balloon_record", {}),
    "santa_sleigh_update": ("sleigh", "santa_sleigh_record", {}),
    "goal_parade_rightward_update": ("parade", "goal_parade_record", {}),
    "goal_parade_leftward_update": ("parade", "goal_parade_record", {}),
    "goal_parade_dragon_update": ("parade", "goal_parade_record", {}),
    "sounding_scenery_update": ("sounding", "sounding_scenery_record", {}),
    "aurora_update": ("aurora", "aurora_record", {"animations": "animations"}),
}
STEP_LISTS = {"palettes": "palette", "animations": "animation"}     # kind -> keyword of its steps
STREAM_COMMANDS = {1: "stage_end", 2: "end", 3: "fork"}


def _stream(walk, offset):
    rows, at = [], offset
    while walk.fits(at, 4):
        position, command = walk.s16(at), walk.u16(at + 2)
        if command == 0:
            if not walk.fits(at, 12):
                return None
            handler, parameter = walk.u32(at + 4), walk.u32(at + 8)
            name = typed.name_for(handler)
            if name in STREAM_RECORDS:
                walk.want(parameter, name)      # (its record: read when it is in this piece)
                shown = walk.place(parameter)
            else:
                shown = f"0x{parameter:08X}"
            rows.append((["object", str(position), walk.place(handler), shown], None))
            at += 12
            continue
        if command not in STREAM_COMMANDS:
            return None
        rows.append(([STREAM_COMMANDS[command], str(position)], None))
        at += 4
        if command == 3:
            # the branches' streams: as many addresses as fit before the first of them
            first = None
            while walk.fits(at, 4) and (first is None or at < first):
                target = walk.offset_of(walk.u32(at))
                if target is None:
                    return None
                walk.want_offset(target, "stream")
                first = target if first is None else min(first, target)
                rows.append((["branch", walk.place(walk.u32(at))], None))
                at += 4
        return at - offset, rows
    return None


def _record_reader(handler):
    keyword, structure, pointers = STREAM_RECORDS[handler]

    def read(walk, offset):
        size = record_size(structure)
        if not walk.fits(offset, size):
            return None
        places = {name: at for name, at, _ in record_columns(structure)}
        for column, kind in pointers.items():
            walk.want(walk.u32(offset + places[column]), kind)
        return size, [([keyword] + record_cells(walk, offset, structure, pointers), None)]
    return read


def _steps_reader(kind):
    keyword = STEP_LISTS[kind]

    def read(walk, offset):
        rows, at = [], offset
        while walk.fits(at, 2):
            step = walk.s16(at)
            at += 2
            if step < 0:
                rows.append((["back", str(step)], None))
                return at - offset, rows
            rows.append(([keyword, f"0x{step:04X}"], None))
        return None
    return read


def road_object_streams_to_text(data, piece="road_object_streams"):
    walk = Walk(data, piece)
    out = typed.Writer()
    out.comment("the courses' object streams (road_object_stream_read), then the records their objects start with;")
    out.comment("positions in segments from the stage's start, decimal; +0xNN: a part's place in the piece")
    out.record("stream", 4, [typed.column("stream", "u32")])
    out.record("object", 12, [typed.column("position", "s16"), typed.column("command", "u16", fixed=0),
                              typed.column("handler", "u32"), typed.column("parameter", "u32")])
    for command, keyword in STREAM_COMMANDS.items():
        out.record(keyword, 4, [typed.column("position", "s16"), typed.column("command", "u16", fixed=command)])
    out.record("branch", 4, [typed.column("stream", "u32")])
    declared = set()
    for keyword, structure, _ in STREAM_RECORDS.values():
        if keyword not in declared:
            declared.add(keyword)
            declare(out, keyword, structure)
    for keyword in STEP_LISTS.values():
        out.record(keyword, 2, [typed.column(keyword, "u16")])
    out.record("back", 2, [typed.column("back", "s16")])
    readers = {"streams": address_table("stream", "stream"), "stream": _stream}
    readers.update({handler: _record_reader(handler) for handler in STREAM_RECORDS})
    readers.update({kind: _steps_reader(kind) for kind in STEP_LISTS})
    walk.want_offset(0, "streams")
    walk.run(readers)
    walk.write(out, _unstarted_records)
    return out.text()


def _unstarted_records(walk, start, end, kind):
    """Between the records the streams start, records of the same kind that none starts (road_row's: the
    road rows' records lie one after another)."""
    if kind not in STREAM_RECORDS:
        return None
    keyword, structure, pointers = STREAM_RECORDS[kind]
    size = record_size(structure)
    if pointers or (end - start) % size or not any(walk.data[start:end]):
        return None
    return [([keyword] + record_cells(walk, at, structure), "no stream starts it" if at == start else None)
            for at in range(start, end, size)]


# ---- stage_road_object_lists and traffic_records ----

TRAFFIC_END = 0xFFFF8000        # a list's end: position 8000, level FFFF


def _traffic_list(walk, offset):
    rows, at = [], offset
    while walk.fits(at, 4):
        if walk.u32(at) == TRAFFIC_END:
            rows.append((["end"], None))
            return at + 4 - offset, rows
        if not walk.fits(at, 16):
            return None
        rows.append((["traffic"] + record_cells(walk, at, "traffic_spawn_entry", ("handler", "record")), None))
        at += 16
    return None


def stage_road_object_lists_to_text(data, piece="stage_road_object_lists"):
    walk = Walk(data, piece)
    out = typed.Writer()
    out.comment("the stages' traffic (traffic_spawner_update): the list of each stage, then the lists, by")
    out.comment("position (segments from the stage's start, decimal) and level; +0xNN: a part's place in the piece")
    out.record("list", 4, [typed.column("list", "u32")])
    declare(out, "traffic", "traffic_spawn_entry")
    out.record("end", 4, [typed.column("end", "u32", fixed=TRAFFIC_END)])
    walk.want_offset(0, "traffic_lists")
    walk.run({"traffic_lists": address_table("list", "traffic_list"), "traffic_list": _traffic_list})
    walk.write(out)
    return out.text()


TRAFFIC_RECORD_SIZE = 12        # a traffic_record and two bytes nothing reads


def traffic_records_to_text(data, piece="traffic_records"):
    """The records one after another: a traffic_record where the first word is a place in the piece (its sprite
    record), a traffic_line_record where it is a small number and a place follows it, else a sprite record."""
    walk = Walk(data, piece)
    out = typed.Writer()
    out.comment("the traffic's records, which the stages' traffic lists (stage_road_object_lists) point at; +0xNN: a")
    out.comment("record's place in the piece")
    declare(out, "sprite", "traffic_sprite_record")
    declare(out, "traffic", "traffic_record", [typed.column("unread", "u16")])
    declare(out, "line", "traffic_line_record")
    out.record("follower", 4, [typed.column("record", "u32")])
    out.record("followers_end", 4, [typed.column("end", "u32", fixed=0)])
    sprite_size = record_size("traffic_sprite_record")
    at = 0
    while at < len(data):
        if walk.fits(at, TRAFFIC_RECORD_SIZE) and walk.offset_of(walk.u32(at)) is not None:
            cells = record_cells(walk, at, "traffic_record", ("sprite",))
            walk.parts[at] = (TRAFFIC_RECORD_SIZE, [(["traffic"] + cells + [value_text(walk.u16(at + 10), "u16")],
                                                     None)])
            at += TRAFFIC_RECORD_SIZE
            continue
        if walk.fits(at, 8) and 0 < walk.u32(at) < 0x100 and walk.offset_of(walk.u32(at + 4)) is not None:
            rows = [(["line"] + record_cells(walk, at, "traffic_line_record"), None)]
            end = at + 4
            while walk.fits(end, 4) and walk.u32(end) != 0 and walk.offset_of(walk.u32(end)) is not None:
                rows.append((["follower", walk.place(walk.u32(end))], None))
                end += 4
            if not walk.fits(end, 4) or walk.u32(end) != 0:
                break
            rows.append((["followers_end"], None))
            walk.parts[at] = (end + 4 - at, rows)
            at = end + 4
            continue
        if not walk.fits(at, sprite_size):
            break
        walk.parts[at] = (sprite_size, [(["sprite"] + record_cells(walk, at, "traffic_sprite_record"), None)])
        at += sprite_size
    walk.write(out)
    return out.text()


# ---- stage_background_object_lists ----

BACKGROUND_RECORDS = {      # handler -> kind of its data
    "backdrop_near_start": "backdrop",
    "backdrop_translucent_start": "backdrop",
    "backdrop_far_start": "backdrop",
    "backdrop_flashing_start": "backdrop",
    "backdrop_landmark_start": "landmark",
    "palette_cycle_update": "palette_cycle",
    "sunset_fade_start": "sunset_fade",
}
BACKDROP_SIZE = 12          # the backdrops' records but the landmarks': no approach_distance


def _background_table(walk, offset):
    """The lists by stage: as many addresses as come before the first list (0: none)."""
    rows, at, first = [], offset, len(walk.data)
    while at < first and walk.fits(at, 4):
        target = walk.u32(at)
        if target and walk.offset_of(target) is None:
            break
        if target:
            walk.want(target, "background_list")
            first = min(first, walk.offset_of(target))
        rows.append((["list", walk.place(target)], f"[{(at - offset) // 4}]"))
        at += 4
    return (at - offset, rows) if rows else None


def _background_list(walk, offset):
    if not walk.fits(offset, 4) or walk.u32(offset) % 8 or not walk.fits(offset, 4 + walk.u32(offset)):
        return None
    length = walk.u32(offset)
    rows = [(["length", str(length)], None)]
    for at in range(offset + 4, offset + 4 + length, 8):
        handler, item = walk.u32(at), walk.u32(at + 4)
        kind = BACKGROUND_RECORDS.get(typed.name_for(handler))
        if kind is not None:
            walk.want(item, kind)
        rows.append((["object", walk.place(handler), walk.place(item)], None))
    return 4 + length, rows


def _backdrop(walk, offset, keyword, size):
    if not walk.fits(offset, size):
        return None
    cells = record_cells(walk, offset, "backdrop_record")
    kept = sum(1 for _, at, _ in record_columns("backdrop_record") if at < size)
    return size, [([keyword] + cells[:kept], None)]


def _plain_record(keyword, structure, pointers=()):
    def read(walk, offset):
        if not walk.fits(offset, record_size(structure)):
            return None
        return record_size(structure), [([keyword] + record_cells(walk, offset, structure, pointers), None)]
    return read


def stage_background_objects_to_text(data, piece="stage_background_object_lists"):
    walk = Walk(data, piece)
    out = typed.Writer()
    out.comment("the stages' background objects (race_background_start_objects): the list of each stage, then the")
    out.comment("lists ('length' in bytes, 8 a pair), then the objects' data; +0xNN: a part's place in the piece")
    out.record("list", 4, [typed.column("list", "u32")])
    out.record("length", 4, [typed.column("length", "u32")])
    out.record("object", 8, [typed.column("handler", "u32"), typed.column("data", "u32")])
    out.record("backdrop", BACKDROP_SIZE, [typed.column(name, kind) for name, offset, kind
                                           in record_columns("backdrop_record") if offset < BACKDROP_SIZE])
    declare(out, "landmark", "backdrop_record")
    declare(out, "palette_cycle", "palette_cycle_record")
    declare(out, "sunset_fade", "sunset_fade_record")
    walk.want_offset(0, "background_lists")
    walk.run({"background_lists": _background_table, "background_list": _background_list,
              "backdrop": lambda walk, offset: _backdrop(walk, offset, "backdrop", BACKDROP_SIZE),
              "landmark": lambda walk, offset: _backdrop(walk, offset, "landmark", record_size("backdrop_record")),
              "palette_cycle": _plain_record("palette_cycle", "palette_cycle_record", ("palette_list",)),
              "sunset_fade": _plain_record("sunset_fade", "sunset_fade_record", ("fade_list",))})
    walk.write(out)
    return out.text()


FORMATS = {
    "program_road_object_streams": (".txt", road_object_streams_to_text, typed.from_text),
    "program_stage_road_object_lists": (".txt", stage_road_object_lists_to_text, typed.from_text),
    "program_traffic_records": (".txt", traffic_records_to_text, typed.from_text),
    "program_stage_background_objects": (".txt", stage_background_objects_to_text, typed.from_text),
}
