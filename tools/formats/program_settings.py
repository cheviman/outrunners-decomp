"""
Name:        program_settings.py
Description: the program's defaults and small tables: the settings' and the bookkeeping's defaults, the
             second task's first frame, the video, mixer and sprite chips' registers, the coin chutes, the
             test mode's cells and items, the time's digits, the signature
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Every format writes the typed format of tools/core/formats/typed.py (from_text is typed.from_text): a
'layout' line says how to read the lines under it, 'record' lines name the kinds of line a piece holds.
Unsigned values are hex, signed ones decimal; an address is written as the name of what is there (a
variable of symbols.txt, a function). What a format cannot read as its records stays as 'bytes' lines, so
every piece goes back whole.

The formats (one dictionary for every module of tools/formats, so their names start with program_):
- program_settings_defaults: what load_settings copies over the settings (20E600-20E67F), and
  program_bookkeeping_defaults what bookkeeping_reset copies over the bookkeeping (701F02-701F7D): a line
  for each variable symbols.txt names there (its name, then its value or values), the bytes no variable
  names as 'bytes' lines.
- program_task_frame: the interrupt frame the second task starts from (copied to 21FF84): R0-R28, PC, PSW.
- program_register_halfwords: halfwords a function copies to a chip's registers, 8 a line (# after a line:
  the first one's offset from the piece's start).
- program_sprite_entries: sprite RAM entries, 8 halfwords each (init_sprite_lists).
- program_coin_chutes: insert_coin's chute entries (coin_chute and its two coin_chute_variant, alone and
  shared), a line each.
- program_input_test_items: test_mode_input_test_draw's items: 'number' the cell of each analog input's
  number, 'switch' an input switch (its port, its bit, the cells of its state on monitor A and B), 'end'
  (FFFF), 'test' the cells of TEST's.
- program_text_cells: text layer cells (the low half of their address, 31F000 + (row * 64 + column) * 2).
- program_setting_places: the settings a page's EXIT checks for changes: the low byte of each one's address
  in settings_magic's page (20E600), closed by 0.
- program_time_digit_steps: the bookkeeping's course record times: 'step' the divisor of each digit and the
  cell step after it, 'end' (FFFFFFFF).
- program_signature: the program's 48-byte signature (crc16'd by signature_checksum), 16 characters a line
  (their codes: the format has no column for a string of a fixed length).
- program_segment_pattern: a byte a segment out from the window (segment_projection_pattern), 16 a line in
  decimal.
- program_colour_tables: sprite RAM's indirect colour tables (load_sprite_ram_tables), 16 halfwords a table,
  a line each; program_solid_colour_blocks its blocks of one pen, 32 bytes (8 words) a line.
"""
import struct
from pathlib import Path

from formats import common, typed
from formats.program_walk import record_columns, value_text

TEXT_LAYER_CELL = 0xF000


def _names():
    """symbols.txt's variables and hardware registers: {address: (name, type)} and {address: count}."""
    if not _NAMES:
        import disassemble
        names = disassemble.load_names(Path(common.ROOT) / "symbols.txt")
        _NAMES["at"] = dict(names.hardware)
        _NAMES["at"].update(names.variables)
        _NAMES["counts"] = dict(names.counts)
    return _NAMES


_NAMES = {}


# ---- copies over variables ----

def _places(base, size, extra):
    """{offset: (name, kind, count)} of the places in base..base+size: symbols.txt's variables and registers
    there, then 'extra' ([(address, name, kind, count)]: kind a value type or a structure of symbols.txt,
    count how many one after another), which go before them (and before what they cover)."""
    names, counts = _names()["at"], _names()["counts"]
    places = {}
    for address, (name, kind) in names.items():
        if base <= address < base + size and kind in typed.VALUE_TYPES:
            places[address - base] = (name, kind, counts.get(address, 1) or 1)
    for address, name, kind, count in extra:
        for inside in range(address + 1, address + _place_size(kind) * count):
            places.pop(inside - base, None)
        places[address - base] = (name, kind, count)
    return places


def _place_size(kind):
    return typed.SIZES[kind] if kind in typed.SIZES else typed.element_size(kind)


def variables_format(base, description, extra=lambda: ()):
    """The bytes a function copies to 'base': a line for each variable there (the line's keyword is its name;
    an array's values on the line, a structure's records a line each); extra() more places (_places)."""
    def to_text(data):
        out = typed.Writer()
        for line in _wrap(description):
            out.comment(line)
        out.comment("a line a variable (its name, then its value or values), a record a line for a structure's;")
        out.comment("the bytes no variable names as 'bytes' lines")
        places = _places(base, len(data), extra())
        items, at, loose = [], 0, bytearray()
        while at < len(data):
            place = places.get(at)
            if place is not None:
                name, kind, count = place
                while count > 1 and at + _place_size(kind) * count > len(data):
                    count -= 1
            if place is None or at + _place_size(kind) > len(data):
                loose.append(data[at])
                at += 1
                continue
            if loose:
                items.append(None)
                items.append(bytes(loose))
                loose = bytearray()
            if kind in typed.VALUE_TYPES:
                values = struct.unpack_from(f"<{count}{typed.VALUE_TYPES[kind]}", data, at)
                columns = [typed.column("value" if count == 1 else f"value[{index}]", kind) for index in range(count)]
                items.append((name, _place_size(kind) * count, columns,
                              [[value_text(value, kind) for value in values]], at))
            else:
                columns = record_columns(kind)
                rows = [[value_text(struct.unpack_from("<" + typed.VALUE_TYPES[column_kind], data,
                                                       at + index * _place_size(kind) + offset)[0], column_kind)
                         for _, offset, column_kind in columns] for index in range(count)]
                items.append((name, _place_size(kind), [typed.column(column, column_kind)
                                                         for column, _, column_kind in columns], rows, at))
            at += _place_size(kind) * count
        if loose:
            items += [None, bytes(loose)]
        declared = set()
        for item in items:
            if isinstance(item, tuple) and item[0] not in declared:
                declared.add(item[0])
                out.record(item[0], item[1], item[2])
        for item in items:
            if item is None:
                continue
            if isinstance(item, bytes):
                out.raw(item)
                continue
            name, size, _, rows, at = item
            for index, cells in enumerate(rows):
                out.row([name] + cells, f"{base + at + index * size:06X}")
        return out.text()
    return (".txt", to_text, typed.from_text)


def _bookkeeping_places():
    """The bookkeeping block's fields (701F00 on), its histogram and its course records."""
    columns = record_columns("bookkeeping_block") or []
    places = [(BOOKKEEPING + offset, name, kind, 1) for name, offset, kind in columns if not name.startswith("at_")]
    return places + [(BOOKKEEPING + 0x24, "histogram", "u16", 7),
                     (BOOKKEEPING + 0x32, "course_record", "backup_course_record", 10)]


BOOKKEEPING = 0x701F00
SETTINGS_BACKUP = 0x3C          # the backup copy of the settings, from backup_settings_checksum (20E644)


def _settings_places():
    """The controls' calibration ranges, and the backup copy's places by the settings they copy."""
    names, counts = _names()["at"], _names()["counts"]
    places = [(0x20E638, "volume_ranges", "u8", 12)]
    for address, (name, kind) in sorted(names.items()):
        if 0x20E60C <= address < 0x20E644 and kind in typed.VALUE_TYPES and address + SETTINGS_BACKUP not in names:
            places.append((address + SETTINGS_BACKUP, "backup_" + name, kind, counts.get(address, 1) or 1))
    return places + [(0x20E638 + SETTINGS_BACKUP, "backup_volume_ranges", "u8", 12)]


def _wrap(text, width=106):
    lines, line = [], ""
    for word in text.split():
        if line and len(line) + len(word) + 1 > width:
            lines.append(line)
            line = ""
        line = f"{line} {word}" if line else word
    return lines + [line]


# ---- the second task's first frame ----

FRAME_REGISTERS = [f"r{index}" for index in range(29)] + ["pc", "psw"]


def task_frame_to_text(data):
    out = typed.Writer()
    out.comment("the interrupt frame the second task starts from (boot and second_task copy it to 21FF84): R0-R28,")
    out.comment("then the PC and the PSW it returns to; a line a register")
    for name in FRAME_REGISTERS:
        out.record(name, 4, [typed.column("value", "u32")])
    count = min(len(data) // 4, len(FRAME_REGISTERS))
    for index in range(count):
        value = struct.unpack_from("<I", data, index * 4)[0]
        out.row([FRAME_REGISTERS[index], typed.name_for(value) or f"0x{value:08X}"])
    out.raw(data[count * 4:])
    return out.text()


# ---- registers ----

def register_halfwords_to_text(data):
    out = typed.Writer()
    out.comment("halfwords copied to a chip's registers in order, 8 a line (# after a line: the first one's offset")
    out.comment("from the piece's start, hex)")
    out.layout("u16")
    count = len(data) // 2
    for at in range(0, count, 8):
        values = struct.unpack_from(f"<{min(8, count - at)}H", data, at * 2)
        out.row([f"0x{value:04X}" for value in values], f"+{at * 2:02X}")
    out.raw(data[count * 2:])
    return out.text()


def sprite_entries_to_text(data):
    out = typed.Writer()
    out.comment("sprite RAM entries, 8 halfwords each (16 bytes), a line each")
    out.layout("u16")
    count = len(data) // 16
    for index in range(count):
        values = struct.unpack_from("<8H", data, index * 16)
        out.row([f"0x{value:04X}" for value in values], f"entry {index}")
    out.raw(data[count * 16:])
    return out.text()


def sprite_control_to_text(data):
    out = typed.Writer()
    out.comment("init_sprite_chip's values for the sprite chip's control registers 500006, 500008, 50000A, 50000C")
    out.comment("(a byte each)")
    out.layout("u8")
    for index, value in enumerate(data):
        out.row([f"0x{value:02X}"], f"{0x500006 + index * 2:06X}")
    return out.text()


# ---- the coin chutes ----

def _chute_columns():
    head = [(name, offset, kind) for name, offset, kind in record_columns("coin_chute") if offset < 8]
    variant = record_columns("coin_chute_variant")
    size = 8 + 2 * typed.element_size("coin_chute_variant")
    columns = list(head)
    for prefix, base in (("alone.", 8), ("shared.", 8 + typed.element_size("coin_chute_variant"))):
        columns += [(prefix + name, base + offset, kind) for name, offset, kind in variant]
    return size, columns


def coin_chutes_to_text(data):
    out = typed.Writer()
    out.comment("insert_coin's chute entries: the switch's bit, the seat's coin sounds, then what the chute does")
    out.comment("alone (chute_entry_offset 0) and with the chutes shared (14): the seat's credits, its coin meter's")
    out.comment("pulses owed, the bookkeeping's count, the chute's coin units setting, the byte for C00074")
    size, columns = _chute_columns()
    out.layout(None, "coin_chute", size, [typed.column(name, kind) for name, _, kind in columns])
    count = len(data) // size
    for index in range(count):
        cells = []
        for name, offset, kind in columns:
            value = struct.unpack_from("<" + typed.VALUE_TYPES[kind], data, index * size + offset)[0]
            named = typed.name_for(value) if kind == "u32" else None
            cells.append(named or value_text(value, kind))
        out.row(cells)
    out.raw(data[count * size:])
    return out.text()


# ---- the test mode ----

def _cell_comment(cell):
    if not TEXT_LAYER_CELL <= cell or cell & 1:
        return None
    index = (cell - TEXT_LAYER_CELL) // 2
    return f"row {index // 64}, column {index % 64}"


def text_cells_to_text(data):
    out = typed.Writer()
    out.comment("text layer cells: the low half of each one's address (F000 + (row * 64 + column) * 2)")
    out.layout("u16")
    count = len(data) // 2
    for index in range(count):
        cell = struct.unpack_from("<H", data, index * 2)[0]
        out.row([f"0x{cell:04X}"], _cell_comment(cell))
    out.raw(data[count * 2:])
    return out.text()


INPUT_NUMBERS = 6       # test_mode_input_test_draw's analog inputs


def input_test_items_to_text(data):
    out = typed.Writer()
    out.comment("test_mode_input_test_draw's items: 'number' the cell of each analog input's number; 'switch' PORT BIT")
    out.comment("CELL_A CELL_B, an input (the I/O port's offset, its bit in decimal, its On/Off cells on monitor A and")
    out.comment("B); 'end'")
    out.comment("(FFFF); then 'test' the cells of TEST's state")
    out.record("number", 2, [typed.column("cell", "u16")])
    out.record("switch", 6, [typed.column("port", "u8"), typed.column("bit", "u8"), typed.column("cell_a", "u16"),
                             typed.column("cell_b", "u16")])
    out.record("end", 2, [typed.column("end", "u16", fixed=0xFFFF)])
    out.record("test", 2, [typed.column("cell", "u16")])
    at = 0
    for _ in range(INPUT_NUMBERS):
        if at + 2 > len(data):
            break
        cell = struct.unpack_from("<H", data, at)[0]
        out.row(["number", f"0x{cell:04X}"], _cell_comment(cell))
        at += 2
    while at + 2 <= len(data) and struct.unpack_from("<H", data, at)[0] != 0xFFFF and at + 6 <= len(data):
        port, bit, cell_a, cell_b = struct.unpack_from("<BBHH", data, at)
        out.row(["switch", f"0x{port:02X}", str(bit), f"0x{cell_a:04X}",
                 f"0x{cell_b:04X}"], _cell_comment(cell_a))
        at += 6
    if data[at:at + 2] == b"\xff\xff":
        out.row(["end"])
        at += 2
        while at + 2 <= len(data) and struct.unpack_from("<H", data, at)[0] >= TEXT_LAYER_CELL:
            cell = struct.unpack_from("<H", data, at)[0]
            out.row(["test", f"0x{cell:04X}"], _cell_comment(cell))
            at += 2
    out.raw(data[at:])
    return out.text()


SETTINGS_PAGE = 0x20E600


def _setting_name(address):
    """The variable at 'address', NAME[N] inside an array, or None."""
    names, counts = _names()["at"], _names()["counts"]
    for start in range(address, address - 0x20, -1):
        found = names.get(start)
        if found is None:
            continue
        width = typed.SIZES.get(found[1], 1)
        index, extra = divmod(address - start, width)
        if index == 0 and not extra:
            return found[0]
        return f"{found[0]}[{index}]" if not extra and index < (counts.get(start, 1) or 1) else None
    return None


def setting_places_to_text(data):
    out = typed.Writer()
    out.comment("the settings a page's EXIT compares with their backup copy: 'setting' the low byte of the")
    out.comment("setting's address in settings_magic's page (20E600), 'end' (0)")
    out.record("setting", 1, [typed.column("place", "u8")])
    out.record("end", 1, [typed.column("end", "u8", fixed=0)])
    at = 0
    while at < len(data):
        if data[at] == 0:
            out.row(["end"])
            at += 1
            break
        out.row(["setting", f"0x{data[at]:02X}"], _setting_name(SETTINGS_PAGE + data[at]))
        at += 1
    out.raw(data[at:])
    return out.text()


def time_digit_steps_to_text(data):
    out = typed.Writer()
    out.comment("a course record's time, digit by digit: 'step' the divisor and the cells to the next digit (each")
    out.comment("cell 2 bytes), decimal; 'end' (FFFFFFFF, negative)")
    out.record("step", 8, [typed.column("divisor", "u32"), typed.column("cell_step", "u32")])
    out.record("end", 4, [typed.column("end", "u32", fixed=0xFFFFFFFF)])
    at = 0
    while at + 4 <= len(data):
        divisor = struct.unpack_from("<I", data, at)[0]
        if divisor == 0xFFFFFFFF:
            out.row(["end"])
            at += 4
            break
        if at + 8 > len(data):
            break
        out.row(["step", str(divisor), str(struct.unpack_from("<I", data, at + 4)[0])])
        at += 8
    out.raw(data[at:])
    return out.text()


def signature_to_text(data):
    out = typed.Writer()
    out.comment("the program's signature, 48 characters with no end (signature_checksum: its crc16): 16 a line,")
    out.comment("their codes, the string beside them")
    out.layout(None, "characters", 16, [typed.column(f"c{index}", "u8") for index in range(16)])
    count = len(data) // 16
    for index in range(count):
        raw = data[index * 16:(index + 1) * 16]
        out.row([f"0x{byte:02X}" for byte in raw], typed.string_text(raw))
    out.raw(data[count * 16:])
    return out.text()


def segment_pattern_to_text(data):
    out = typed.Writer()
    out.comment("a byte a segment out from the window, 16 a line, decimal (# after a line: its first segment)")
    out.layout("u8")
    for at in range(0, len(data), 16):
        out.row([str(byte) for byte in data[at:at + 16]], str(at))
    return out.text()


def colour_tables_to_text(data):
    out = typed.Writer()
    out.comment("sprite RAM's indirect colour tables: 16 halfwords a table (by pen), a line each (# after a line:")
    out.comment("the table's number in the piece)")
    out.layout("u16")
    count = len(data) // 32
    for index in range(count):
        out.row([f"0x{value:04X}" for value in struct.unpack_from("<16H", data, index * 32)], str(index))
    out.raw(data[count * 32:])
    return out.text()


def solid_colour_blocks_to_text(data):
    out = typed.Writer()
    out.comment("sprite RAM's blocks of one pen: 32 bytes (8 words) a block, a line each")
    out.layout("u32")
    count = len(data) // 32
    for index in range(count):
        out.row([f"0x{value:08X}" for value in struct.unpack_from("<8I", data, index * 32)], f"pen {index + 1:X}")
    out.raw(data[count * 32:])
    return out.text()


FORMATS = {
    "program_settings_defaults": variables_format(
        0x20E600, "the defaults load_settings copies over the settings (move_halfwords: 40 halfwords to 20E600), "
        "and their backup copy from backup_settings_checksum (3C bytes on); volume_ranges: each control's "
        "calibration, a low and a high byte", _settings_places),
    "program_bookkeeping_defaults": variables_format(
        BOOKKEEPING + 2, "the defaults bookkeeping_reset copies over the bookkeeping (bookkeeping_copy: 3E halfwords to "
        "701F02): bookkeeping_block's fields, the time histogram (games by the stage they ended at: PRE-STAGE, "
        "STAGE 1-5, COMPLETE), the courses' records (backup_course_record: none with a negative model)",
        _bookkeeping_places),
    "program_task_frame": (".txt", task_frame_to_text, typed.from_text),
    "program_register_halfwords": (".txt", register_halfwords_to_text, typed.from_text),
    "program_sprite_entries": (".txt", sprite_entries_to_text, typed.from_text),
    "program_sprite_control": (".txt", sprite_control_to_text, typed.from_text),
    "program_coin_chutes": (".txt", coin_chutes_to_text, typed.from_text),
    "program_text_cells": (".txt", text_cells_to_text, typed.from_text),
    "program_input_test_items": (".txt", input_test_items_to_text, typed.from_text),
    "program_setting_places": (".txt", setting_places_to_text, typed.from_text),
    "program_time_digit_steps": (".txt", time_digit_steps_to_text, typed.from_text),
    "program_signature": (".txt", signature_to_text, typed.from_text),
    "program_segment_pattern": (".txt", segment_pattern_to_text, typed.from_text),
    "program_colour_tables": (".txt", colour_tables_to_text, typed.from_text),
    "program_solid_colour_blocks": (".txt", solid_colour_blocks_to_text, typed.from_text),
}
