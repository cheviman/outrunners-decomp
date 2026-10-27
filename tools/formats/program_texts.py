"""
Name:        program_texts.py
Description: the program's texts as quoted strings: the test mode's pages, the text layer's records, the
             sprite texts, the song titles, the LCD's texts and the DJ's messages
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

Every format writes the typed format of tools/core/formats/typed.py (from_text is typed.from_text): 'record'
lines name the kinds of line a piece holds, each line after the keyword of its kind; a string is quoted and
goes back closed by a 0 byte (ASCII as it is, any other byte as \\xNN). What a format cannot read as its
records stays as 'bytes' lines, so every piece goes back whole.

The formats (one dictionary for every module of tools/formats, so their names start with program_):
- program_test_mode_texts: test_mode_draw_texts' lists: 'text' a cell (the low half of the text layer
  cell's address, 31F000 + (row * 64 + column) * 2) and its characters, closed by 0; a character from 80 up
  sets the attribute (the cells' high byte) of the ones after it to its complement (\\x97: attribute 68);
  'end', a 0 halfword, closes a list; 'padding' a 0 byte between two lists.
- program_text_layer_texts: print_text_layer's records: 'text' a cell (its address less 310000), the
  attribute byte and the characters, closed by 0; 'end', FFFF, closes a list (print_text_layer_list);
  'characters' a string alone (network_check_toggle_memory_text's); a 0 byte between them is padding.
- program_sprite_texts: draw_text's records, a sprite a character: 'text' x, y, the attribute (the
  palette word's low bits) and the characters, closed by 0; 'end', an x of FFFF (negative), closes a list
  (staff_credits_code_show's).
- program_song_titles: song_title_show's records: 'title' x and the text, closed by 0 (` is the note,
  custom character 7 on the LCD); a 0 byte before one at an odd offset is padding.
- program_strings: strings closed by 0 (the LCD's scrolling texts, lcd_scroll_request).
- program_dj_messages: start_dj_message's records by message number: the mouth shapes' string (the low
  24 bits: an address, by name) and the sound (the top byte).
- program_dj_mouth_shapes: the DJ's mouth shapes for lcd_update, a string a message: the shapes 0-2
  (dj_face_characters by number), one every third frame, closed by a negative byte.
- program_radio_songs: car_radio_song's songs by number: the title (song_title_record: the low 24 bits, an
  address, by name) and the sound that plays it (the top byte).
- program_coin_setting_descriptions: the coin settings' descriptions (test_mode_coin_assignments_draw):
  'pair' COINS CREDITS, up to five, then 'end' (0) or 'text' (FF, then the text closed by 0).
- program_lcd_entry, program_car_colour_names: characters with no end, a record each of as many as the code
  copies (6): written as their codes, the string beside them as a comment (the format has no column for a
  string of a fixed length).
"""
import struct

from formats import typed

TEXT_LAYER_CELL = 0xF000        # the text layer's first cell (31F000), the low half of its address
TEXT_LAYER_END = 0x10000        # 64 x 32 cells of a halfword
SHAPES_PER_LINE = 32            # the DJ's mouth shapes a line (a long string goes on on the next)
PADDING = 3                     # the most zero bytes between two test mode lists (to the next 4)


def _string_end(data, at):
    """The index of the 0 that closes the string at 'at', or None."""
    end = data.find(b"\0", at)
    return None if end < 0 else end


def _cell_comment(cell):
    index = (cell - TEXT_LAYER_CELL) // 2
    return f"row {index // 64}, column {index % 64}"


def _is_cell(data, at):
    if at + 2 > len(data):
        return False
    cell = struct.unpack_from("<H", data, at)[0]
    return TEXT_LAYER_CELL <= cell < TEXT_LAYER_END and not cell & 1


def _printable(raw):
    return all(0x20 <= byte < 0x7F for byte in raw)


def _characters(raw):
    """Characters of a font (the LCD's 7E and 7F are arrows): no control codes."""
    return all(byte >= 0x20 for byte in raw)


# ---- test_mode_draw_texts' lists ----

def _test_mode_list(data, at):
    """[(cells, comment)] of the list at 'at' and where it ends, or None when no list is there."""
    rows = []
    while True:
        if at + 2 > len(data):
            return None
        cell = struct.unpack_from("<H", data, at)[0]
        if cell == 0:
            return (rows + [(["end"], None)], at + 2) if rows else None
        end = _string_end(data, at + 2)
        if not _is_cell(data, at) or end is None:
            return None
        characters = data[at + 2:end]
        attributes = [f"{~byte & 0xFF:02X}" for byte in characters if byte & 0x80]
        comment = _cell_comment(cell) + (f"; attribute {', '.join(attributes)}" if attributes else "")
        rows.append((["text", f"0x{cell:04X}", typed.string_text(characters)], comment))
        at = end + 1


def test_mode_texts_to_text(data):
    out = typed.Writer()
    out.comment("test_mode_draw_texts' lists: 'text' CELL \"CHARACTERS\", CELL the low half of the text layer cell's")
    out.comment("address (F000 + (row * 64 + column) * 2); a character from 80 up (\\xNN) sets the attribute of the")
    out.comment("ones after it to its complement; 'end' closes a list")
    out.record("text", 0, [typed.column("cell", "u16"), typed.column("characters", "text")])
    out.record("end", 2, [typed.column("end", "u16", fixed=0)])
    out.record("padding", 1, [typed.column("padding", "u8", fixed=0)])
    at = 0
    while at < len(data):
        found = _test_mode_list(data, at)
        if found is None:
            # zero bytes before the next list: padding
            skip = next((count for count in range(1, PADDING + 1) if not any(data[at:at + count])
                         and _test_mode_list(data, at + count) is not None), None)
            if skip is None:
                break
            for _ in range(skip):
                out.row(["padding"])
            found = _test_mode_list(data, at + skip)
        rows, at = found
        for cells, comment in rows:
            out.row(cells, comment)
    out.raw(data[at:])
    return out.text()


# ---- print_text_layer's records ----

def text_layer_texts_to_text(data):
    out = typed.Writer()
    out.comment("print_text_layer's records: 'text' CELL ATTRIBUTE \"CHARACTERS\", CELL the cell's address less 310000")
    out.comment("(F000 + (row * 64 + column) * 2), ATTRIBUTE the cells' high byte; 'end' (FFFF) closes a list;")
    out.comment("'characters' a string alone; a 0 byte between them is padding")
    out.record("text", 0, [typed.column("cell", "u16"), typed.column("attribute", "u8"),
                           typed.column("characters", "text")])
    out.record("characters", 0, [typed.column("characters", "text")])
    out.record("end", 2, [typed.column("end", "u16", fixed=0xFFFF)])
    out.record("padding", 1, [typed.column("padding", "u8", fixed=0)])
    at = 0
    while at < len(data):
        if data[at:at + 2] == b"\xff\xff":
            out.row(["end"])
            at += 2
            continue
        end = _string_end(data, at + 3)
        if _is_cell(data, at) and end is not None:
            cell = struct.unpack_from("<H", data, at)[0]
            out.row(["text", f"0x{cell:04X}", f"0x{data[at + 2]:02X}", typed.string_text(data[at + 3:end])],
                    _cell_comment(cell))
            at = end + 1
            continue
        if data[at] == 0 and (_is_cell(data, at + 1) or data[at + 1:at + 3] == b"\xff\xff"):
            out.row(["padding"])
            at += 1
            continue
        end = _string_end(data, at)
        if end is not None and end > at and _printable(data[at:end]) and data[end + 1:end + 3] == b"\xff\xff":
            out.row(["characters", typed.string_text(data[at:end])])
            at = end + 1
            continue
        break
    out.raw(data[at:])
    return out.text()


# ---- draw_text's records ----

def sprite_texts_to_text(data):
    out = typed.Writer()
    out.comment("draw_text's records, a sprite a character: 'text' X Y ATTRIBUTE \"CHARACTERS\"; 'end' (an x of")
    out.comment("FFFF, negative) closes a list")
    out.record("text", 0, [typed.column("x", "u16"), typed.column("y", "u16"), typed.column("attribute", "u16"),
                           typed.column("characters", "text")])
    out.record("end", 2, [typed.column("end", "u16", fixed=0xFFFF)])
    at = 0
    while at + 2 <= len(data):
        if data[at:at + 2] == b"\xff\xff":
            out.row(["end"])
            at += 2
            continue
        end = _string_end(data, at + 6)
        if at + 6 > len(data) or end is None or end == at + 6 or not _printable(data[at + 6:end]):
            break
        x, y, attribute = struct.unpack_from("<3H", data, at)
        if x & 0x8000:
            break
        out.row(["text", f"0x{x:04X}", f"0x{y:04X}", f"0x{attribute:04X}", typed.string_text(data[at + 6:end])])
        at = end + 1
    out.raw(data[at:])
    return out.text()


# ---- song titles ----

def song_titles_to_text(data):
    out = typed.Writer()
    out.comment("song_title_show's records: 'title' X \"TEXT\" (` is the note; the LCD scrolls the text too)")
    out.record("title", 0, [typed.column("x", "u16"), typed.column("text", "text")])
    out.record("padding", 1, [typed.column("padding", "u8", fixed=0)])

    def title_end(at):
        end = _string_end(data, at + 2) if at + 2 < len(data) else None
        return None if end is None or end == at + 2 or not _printable(data[at + 2:end]) else end
    at = 0
    while at + 2 < len(data):
        if at % 2 and data[at] == 0 and title_end(at + 1) is not None:
            out.row(["padding"])
            at += 1
            continue
        end = title_end(at)
        if end is None:
            break
        out.row(["title", f"0x{struct.unpack_from('<H', data, at)[0]:04X}", typed.string_text(data[at + 2:end])])
        at = end + 1
    out.raw(data[at:])
    return out.text()


# ---- strings ----

def strings_to_text(data):
    out = typed.Writer()
    out.comment("strings, each closed by 0")
    out.layout("string", "string", 0, [typed.column("characters", "text")])
    at = 0
    while at < len(data):
        end = _string_end(data, at)
        if end is None or end == at or not _characters(data[at:end]):
            break
        out.row([typed.string_text(data[at:end])])
        at = end + 1
    out.raw(data[at:])
    return out.text()


# ---- the DJ ----

def _address_byte_records(data, comments, keyword, address, byte, numbered=None):
    """Records of a 32-bit word: an address in its low 24 bits (by name) and a byte above it."""
    out = typed.Writer()
    for comment in comments:
        out.comment(comment)
    count = len(data) // 4
    out.layout(None, keyword, 4, [typed.column(address, "u24"), typed.column(byte, "u8")])
    for index in range(count):
        value = struct.unpack_from("<I", data, index * 4)[0]
        place = value & 0xFFFFFF
        out.row([f"[{index}]", typed.name_for(place) or f"0x{place:06X}", f"0x{value >> 24:02X}"],
                numbered(index) if numbered else None)
    out.raw(data[count * 4:])
    return out.text()


def dj_messages_to_text(data):
    return _address_byte_records(data, ["start_dj_message's records by message number: the mouth shapes' string "
                                        "(dj_mouth_shapes)", "and the sound the message plays"],
                                 "dj_message", "mouth_shapes", "sound", lambda index: f"message {index:02X}")


def radio_songs_to_text(data):
    return _address_byte_records(data, ["car_radio_song's songs by number: the title (a song_title_record) and the "
                                        "sound that plays", "the song"], "radio_song", "title", "sound")


def coin_setting_descriptions_to_text(data):
    out = typed.Writer()
    out.comment("the coin settings' descriptions (test_mode_coin_assignments_draw), one after another: 'pair' COINS")
    out.comment("CREDITS (decimal), then 'end' (0) or 'text' (FF, then the text)")
    out.record("pair", 2, [typed.column("coins", "u8"), typed.column("credits", "u8")])
    out.record("end", 1, [typed.column("end", "u8", fixed=0)])
    out.record("text", 0, [typed.column("marker", "u8", fixed=0xFF), typed.column("characters", "text")])
    at, first = 0, True
    while at < len(data):
        if data[at] == 0:
            out.row(["end"])
            at, first = at + 1, True
            continue
        if data[at] == 0xFF:
            end = _string_end(data, at + 1)
            if end is None or not _printable(data[at + 1:end]):
                break
            out.row(["text", typed.string_text(data[at + 1:end])], f"+0x{at:X}" if first else None)
            at, first = end + 1, True
            continue
        if data[at] & 0x80 or at + 2 > len(data):
            break
        out.row(["pair", str(data[at]), str(data[at + 1])], f"+0x{at:X}" if first else None)
        at, first = at + 2, False
    out.raw(data[at:])
    return out.text()


def dj_mouth_shapes_to_text(data):
    out = typed.Writer()
    out.comment("the DJ's mouth shapes, a string a message (dj_messages names each by its place, +0xNN): the")
    out.comment("shapes 0-2 (dj_face_characters by number), one every third frame, closed by a negative byte")
    out.layout("s8")
    at = 0
    while at < len(data):
        end = next((index for index in range(at, len(data)) if data[index] & 0x80), None)
        if end is None:
            break
        values = [str(struct.unpack_from("b", data, index)[0]) for index in range(at, end + 1)]
        for line in range(0, len(values), SHAPES_PER_LINE):
            out.row(values[line:line + SHAPES_PER_LINE], f"+0x{at:X}" if line == 0 else None)
        at = end + 1
    out.raw(data[at:])
    return out.text()


# ---- characters with no end ----

def characters_format(length, description):
    """The format of records of 'length' characters each, with no end (the code copies that many)."""
    def to_text(data):
        out = typed.Writer()
        out.comment(description)
        out.comment(f"{length} characters a record, no end: their codes, the string beside them")
        count = len(data) // length
        out.layout("characters", "characters", length, [typed.column(f"c{index}", "u8") for index in range(length)])
        for index in range(count):
            raw = data[index * length:(index + 1) * length]
            out.row([f"[{index}]"] + [f"0x{byte:02X}" for byte in raw], typed.string_text(raw))
        out.raw(data[count * length:])
        return out.text()
    return (".txt", to_text, typed.from_text)


FORMATS = {
    "program_test_mode_texts": (".txt", test_mode_texts_to_text, typed.from_text),
    "program_text_layer_texts": (".txt", text_layer_texts_to_text, typed.from_text),
    "program_sprite_texts": (".txt", sprite_texts_to_text, typed.from_text),
    "program_song_titles": (".txt", song_titles_to_text, typed.from_text),
    "program_strings": (".txt", strings_to_text, typed.from_text),
    "program_dj_messages": (".txt", dj_messages_to_text, typed.from_text),
    "program_dj_mouth_shapes": (".txt", dj_mouth_shapes_to_text, typed.from_text),
    "program_radio_songs": (".txt", radio_songs_to_text, typed.from_text),
    "program_coin_setting_descriptions": (".txt", coin_setting_descriptions_to_text, typed.from_text),
    "program_lcd_entry": characters_format(6, "the LCD's Entry: (car_select_entries_run copies its 6 characters)"),
    "program_car_colour_names": characters_format(
        6, "the cars by colour (a course record's car: test_mode_bookkeeping_draw_histogram shows its 6 characters)"),
}
