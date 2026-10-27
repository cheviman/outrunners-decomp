"""
Name:        sound_tables.py
Description: the sound driver's tables in the typed format: the chips' set-up, the voices' channels, the FM
             patch registers, the pitch tables, the note codes, the velocity curve, the bend ranges, the
             MIDI part templates, the instruments, the FM patches, the drum kits, the engine and tyre noises
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

The tables the Z80 sound driver (src/sound/*.c) reads below A000 of the sound program's chip, little endian,
written in the core's typed format (tools/core/formats/typed.py), which one reader reads back for every
piece: 'layout TYPE' and rows of values, 'layout NAME SIZE column:type ...' and a record a row, 'record
KEYWORD SIZE column:type ...' and rows that start with the keyword, a column:type:N cell written HIGH:LOW
in hex (split at bit N), names for places (sound_names: NAME or NAME+0xNN, names.txt gives their offsets)
and 'bytes XX ...' for what no row explains. Comments ('#') say what the values are: which indices point
at a record, what a register is. Every format's from_text is typed.from_text: to_text(data) gives the very
bytes back for any data.

Where a table is pointers and records (part_templates, instruments, fm_patches, drum_kits, the pitch
tables, bend_range_tables), the pointers come first, by index, as the places they point at (the piece's
own name and +0xNN for its records); then the records in the order they lie, each under a comment that
starts with its place (+0xNN) and says which indices point at it. A record ends where the next one starts.

The formats (FORMATS is one dictionary for every module, so their names start with sound_):
- sound_register_pairs: YM3438 register, value pairs (fm_global_init, fm_operator_init).
- sound_voice_channels: each of the 30 voices' chip channel (voice_chip_channels).
- sound_register_list: YM3438 registers the channel within the part is added to (fm_patch_registers,
  fm_level_registers).
- sound_semitone_pointers: 12 pointers by semitone alone (the pitch pointer tables nothing reads).
- sound_fm_fnum_tables: by semitone, the run of YM3438 pitches (one a pitch bend step, 1/16 semitone)
  start_fm_note reads at the run + 2 x the bend; the runs run on below and above the octave. A value is
  BLOCK:FNUM split at bit 11: the block offset in 5 bits (1F is -1, 1 is +1) and the F-number.
- sound_pcm_pitch_tables: the same for the MultiPCM, split at bit 12: OCTAVE:LOW, the octave offset in 4 bits
  (F is -1) and register 2's bits above register 3's low nibble: the F-number times 4.
- sound_note_codes: by MIDI note number (+ the part's transpose), the note code: octave << 4 | semitone.
- sound_velocity_levels: by note-on velocity, the note's level.
- sound_bend_range_tables: by the part's bend_range, the run of pitch steps by the pitch bend's distance
  from the centre.
- sound_part_templates: by sound set (CC 20), the parts' settings.
- sound_instruments: by program 00-7F, the instrument: MultiPCM key splits, or an FM patch.
- sound_fm_patches: by patch number, the 25 bytes of a YM3438 voice, a row a register group.
- sound_drum_kits: by program F0-FF, the kit's notes: pitch, time-out and sample.
- sound_revs_levels, sound_revs_pitches, sound_revs_list: the engine's level and pitch by revs, the revs of
  the engine sweep.
- sound_tyre_noises: the 8 tyre noises: sample, pitch, level, duration.
"""
import struct

from formats import sound_names
from formats import typed

SEMITONES = ("C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B")
RUN_STEPS = 16                      # pitch table steps a semitone (1/16 semitone)
ROW = 16                            # values a row in the long lists
OPERATOR_OF_SLOT = (1, 3, 2, 4)     # a YM3438 channel's registers +0, +4, +8, +C are operators 1, 3, 2, 4


# ---- helpers ----

def read(text):
    """Every format's from_text: the typed format, its names the sound program's."""
    return typed.from_text(text, resolve=sound_names.resolve)


def _hex(value, digits=2):
    return f"0x{value:0{digits}X}"


def _signed(value, bits):
    """A field of 'bits' bits as a signed number: +3, -1, 0."""
    if value & 1 << (bits - 1):
        value -= 1 << bits
    return f"{value:+d}" if value else "0"


def _u16(data, at):
    return data[at] | data[at + 1] << 8


def _base(name):
    """Where the piece 'name' is (its pointers into itself are places of it)."""
    return sound_names.place_of(name)


def _header(writer, text):
    """A comment of several lines, wrapped at 110 columns."""
    line = ""
    for word in text.split():
        if line and len(line) + len(word) + 3 > 110:
            writer.comment(line)
            line = ""
        line = f"{line} {word}" if line else word
    if line:
        writer.comment(line)


def _values(writer, data, kind, per_row, comment=lambda first, count: None):
    """Rows of plain values (writer.layout(kind) before): '[FIRST]' then up to per_row values; what is left
    of a value as bytes."""
    size = typed.SIZES[kind]
    count = len(data) // size
    for first in range(0, count, per_row):
        values = [int.from_bytes(data[at * size:at * size + size], "little") for at in range(first, min(count, first + per_row))]
        writer.row([f"[{first}]"] + [_hex(value, size * 2) for value in values], comment(first, len(values)))
    if len(data) % size:
        writer.raw(data[count * size:])


def _pointer_table(writer, data, base, count, label):
    """The table's pointers as the places they point at, by index (label(index) after '#'); returns
    (count, the record starts: offsets in the piece the pointers give after the pointers, sorted)."""
    count = min(count, len(data) // 2)
    writer.layout("u16")
    for index in range(count):
        writer.row([f"[{index}]", sound_names.pointer_text(_u16(data, index * 2))], label(index))
    starts = sorted({_u16(data, index * 2) - base for index in range(count)} & set(range(count * 2, len(data))))
    return count, starts


def _records(writer, data, count, base, starts, label):
    """[(start, end, comment)] of the records after the pointers; writes the bytes before the first record
    as bytes. The comment: the record's place and the indices that point at it (label(index))."""
    first = starts[0] if starts else len(data)
    if first > count * 2:
        writer.comment("bytes no pointer reaches")
        writer.raw(data[count * 2:first])
    spans = []
    for start, end in zip(starts, starts[1:] + [len(data)]):
        users = [label(index) for index in range(count) if _u16(data, index * 2) == base + start]
        spans.append((start, end, f"+0x{start:X}" + (f": {', '.join(users)}" if users else "")))
    return spans


# ---- YM3438 register pairs and lists ----

FM_REGISTERS = {
    0x22: "LFO", 0x24: "timer A, high bits", 0x25: "timer A, low bits", 0x26: "timer B", 0x27: "timers, channel 3 mode",
    0x28: "key on/off", 0x2A: "DAC data", 0x2B: "DAC on",
}
FM_GROUPS = {0x30: "detune, multiple", 0x40: "total level", 0x50: "key scale, attack rate", 0x60: "AM, decay rate",
             0x70: "sustain rate", 0x80: "sustain level, release rate", 0x90: "SSG-EG"}


def _fm_register_name(register, channel=True):
    if register in FM_REGISTERS:
        return FM_REGISTERS[register]
    group = register & 0xF0
    if group in FM_GROUPS and register & 3 != 3:
        where = f"channel {register & 3}, " if channel else ""
        return f"{FM_GROUPS[group]}: {where}operator {OPERATOR_OF_SLOT[(register >> 2) & 3]}"
    if group == 0xA0:
        return {0xA0: "F-number, low bits", 0xA4: "block, F-number high bits"}.get(register & 0xFC)
    if group == 0xB0:
        return {0xB0: "feedback, algorithm", 0xB4: "output, LFO sensitivity"}.get(register & 0xFC)
    return None


def register_pairs_to_text(data):
    writer = typed.Writer()
    writer.comment("YM3438 register, value pairs, written in this order on part I (init_fm_chip; fm_operator_init on")
    writer.comment("both parts); after '#' the register's name")
    writer.layout(None, "register_value_pair", 2, [typed.column("register_number", "u8"), typed.column("value", "u8")])
    for index in range(len(data) // 2):
        writer.row([f"[{index}]", _hex(data[index * 2]), _hex(data[index * 2 + 1])], _fm_register_name(data[index * 2]))
    if len(data) % 2:
        writer.raw(data[-1:])
    return writer.text()


def register_list_to_text(data):
    writer = typed.Writer()
    writer.comment("YM3438 registers, the channel within its part added to each (load_fm_patch, apply_fm_*_volume):")
    writer.comment("registers +0, +4, +8, +C of a group are operators 1, 3, 2, 4")
    writer.layout("u8")
    for index, register in enumerate(data):
        writer.row([f"[{index}]", _hex(register)], _fm_register_name(register, channel=False))
    return writer.text()


# ---- the voices' chip channels ----

def voice_channels_to_text(data):
    writer = typed.Writer()
    writer.comment("by voice (the 30 voice records, init_voice_channels), its chip channel: a MultiPCM voice's code,")
    writer.comment("or 80 + a YM3438 channel (bit 2 part II); init_pcm_chip also selects the first 28 as MultiPCM voices")
    writer.layout("u8")
    for voice, code in enumerate(data):
        if code < 0x80:
            what = "MultiPCM voice"
        elif code & 0x78 == 0 and code & 3 != 3:
            what = f"YM3438 part {'II' if code & 4 else 'I'}, channel {code & 3}"
        else:
            what = None
        writer.row([f"[{voice}]", _hex(code)], f"voice {voice}" + (f": {what}" if what else ""))
    return writer.text()


# ---- pitch tables ----

def semitone_pointers_to_text(data):
    writer = typed.Writer()
    writer.comment("12 pointers by semitone (C..B) in the shape of a pitch table's: nothing reads them (the driver")
    writer.comment("reads the table after them by the note's semitone)")
    count = len(data) // 2
    writer.layout("u16")
    for index in range(count):
        writer.row([f"[{index}]", sound_names.pointer_text(_u16(data, index * 2))],
                   SEMITONES[index] if index < 12 else None)
    if len(data) % 2:
        writer.raw(data[-1:])
    return writer.text()


def _pitch_tables(name, header, split):
    """A pitch table: 12 pointers by semitone into its runs, then the runs, a row of 16 values (32 bytes)
    split at bit 'split', each with its place and the semitones that point at it after '#'."""
    def to_text(data):
        writer = typed.Writer()
        _header(writer, header)
        count = min(12, len(data) // 2)
        writer.layout("u16")
        for index in range(count):
            writer.row([f"[{index}]", sound_names.pointer_text(_u16(data, index * 2))], SEMITONES[index])
        base = _base(name)
        pointers = {_u16(data, index * 2) - base: SEMITONES[index] for index in range(count)}
        runs = (len(data) - count * 2) // (RUN_STEPS * 2)
        writer.layout(None, "pitch_run", RUN_STEPS * 2, [typed.column(f"step_{step}", "u16", split) for step in range(RUN_STEPS)])
        for run in range(runs):
            at = count * 2 + run * RUN_STEPS * 2
            words = [_u16(data, at + step * 2) for step in range(RUN_STEPS)]
            writer.row([f"[{run}]"] + [typed.split_text(word, split) for word in words],
                       f"+0x{at:X}" + (f": {pointers[at]}" if at in pointers else ""))
        writer.raw(data[count * 2 + runs * RUN_STEPS * 2:])
        return writer.text()
    return to_text


FNUM_HEADER = ("fm_fnum_tables (start_fm_note 12E0, retune_fm_voice 13D8): by the note's semitone, a pointer to its "
               "run of YM3438 pitches, one a pitch bend step (1/16 semitone), read at the run + 2 x the bend: the runs "
               "go on below and above, so a bend reaches the neighbouring semitones. A pitch is BLOCK:FNUM: the block "
               "offset in 5 bits (1F is -1, 1 is +1; the note's octave is added to it) and the F-number, hex. A row "
               "is a run of 16 steps; after '#' its place and the semitone that points at it.")
PCM_PITCH_HEADER = ("pcm_pitch_tables (play_midi_message 0599, retune_voice 0705): by the note's semitone, a pointer to "
                    "its run of MultiPCM pitches, one a pitch bend step (1/16 semitone), read at the run + 2 x (the bend "
                    "+ the split's tune): the runs go on below and above. A pitch is OCTAVE:LOW: the octave offset in "
                    "4 bits (F is -1; the note's octave and the split's are added) and the F-number times 4 (registers "
                    "3 and 2), hex. A row is a run of 16 steps; after '#' its place and the semitone that points at it.")


# ---- note codes, the velocity curve, the lists by revs ----

def note_codes_to_text(data):
    writer = typed.Writer()
    writer.comment("note_codes (play_midi_message 0410): by MIDI note number (+ the part's transpose), the note code")
    writer.comment("the voices keep: its octave (signed, high nibble) << 4 | its semitone (C = 0 .. B = B); 12 a row")
    writer.layout("u8")

    def octave(first, count):
        octaves = {byte >> 4 for byte in data[first:first + count]}
        return f"notes 0x{first:02X}-0x{first + count - 1:02X}" + (f": octave {_signed(data[first] >> 4, 4)}"
                                                                    if len(octaves) == 1 else "")
    _values(writer, data, "u8", 12, octave)
    return writer.text()


def _value_list(kind, header, index_name):
    def to_text(data):
        writer = typed.Writer()
        _header(writer, header)
        writer.layout(kind)
        _values(writer, data, kind, ROW, lambda first, count: f"{index_name} 0x{first:02X}-0x{first + count - 1:02X}")
        return writer.text()
    return to_text


VELOCITY_HEADER = "velocity_levels (play_midi_message 0425): by note-on velocity 00-7F, the note's level (00-FF), 16 a row."
ENGINE_LEVELS_HEADER = ("engine_levels (set_engine_pitch 376D): by revs 00-FF, the engine's level before the loudness is "
                        "taken off (register 5: 0 loudest), 16 a row.")
SWEEP_HEADER = ("engine_sweep_revs (engine_sweep_tick 3AF3): the revs the engine sweep plays one after another every 65 "
                "ticks, up the list and back down; 0 at its ends turns it. 16 a row.")


def engine_pitches_to_text(data):
    writer = typed.Writer()
    _header(writer, "engine_pitches (set_engine_pitch 376D): by revs 00-FF, the engine sample's MultiPCM pitch, OCTAVE:LOW "
                    "(the octave in 4 bits, F is -1, E is -2; the F-number times 4: registers 3 and 2), hex; a random "
                    "amount is added where the octave is negative. 16 a row.")
    rows = len(data) // (ROW * 2)
    writer.layout(None, "pitch_row", ROW * 2, [typed.column(f"revs_{step:X}", "u16", 12) for step in range(ROW)])
    for row in range(rows):
        words = [_u16(data, (row * ROW + step) * 2) for step in range(ROW)]
        writer.row([f"[{row}]"] + [typed.split_text(word, 12) for word in words],
                   f"revs 0x{row * ROW:02X}-0x{row * ROW + ROW - 1:02X}")
    writer.raw(data[rows * ROW * 2:])
    return writer.text()


# ---- bend ranges ----

BEND_HEADER = ("bend_range_tables (play_midi_message 0692): by the part's bend_range, a pointer to its table: by the "
               "pitch bend's distance from the centre (bits 5-12 of the 14-bit bend, negated below the centre), the "
               "pitch steps (1/16 semitone) the voices move; a table is 256 steps, 16 a row, after its place.")


def bend_tables_to_text(data):
    writer = typed.Writer()
    _header(writer, BEND_HEADER)
    base = _base("bend_range_tables")
    count, starts = _pointer_table(writer, data, base, 12, lambda index: f"bend_range {index}")
    spans = _records(writer, data, count, base, starts, lambda index: f"bend_range {index}")
    for start, end, comment in spans:
        writer.comment(comment)
        writer.layout("u8")
        _values(writer, data[start:end], "u8", ROW)
    return writer.text()


# ---- part templates ----

PART_COLUMNS = [typed.column(name, kind) for name, kind in (
    ("flags", "u8"), ("midi_channel", "u8"), ("volume", "u8"), ("program", "u8"), ("transpose", "s8"), ("at_05", "u8"),
    ("lfo_frequency", "u8"), ("at_07", "u8"), ("effective_volume", "u8"), ("bend_range", "u8"))]
PART_SIZE = 10
MUSIC_PARTS = 8
TEMPLATES_HEADER = ("part_templates (CC 20, select_sound_set 098C; init_parts 0316 for sound set 0): by sound set, a "
                    "pointer to its template: the 8 music parts (midi_part: flags (bit 7 plays, bit 0 at full volume), "
                    "MIDI channel, volume, program (F0-FF: drum kit 0-F), transpose (added to the note), at_05 "
                    "(unread), lfo_frequency (MultiPCM register 6), at_07 (unread), effective_volume (worked out "
                    "again), bend_range), then the sound set's sample bank (PCM_BANKS bits 0-2), then for sound set 0 "
                    "the 5 parts of channels A-E (the effects'), which no CC 20 changes.")


def part_templates_to_text(data):
    writer = typed.Writer()
    _header(writer, TEMPLATES_HEADER)
    base = _base("part_templates")
    count, starts = _pointer_table(writer, data, base, 15, lambda index: f"sound set {index:X}")
    writer.layout(None, "midi_part", PART_SIZE, PART_COLUMNS)
    writer.record("bank", 1, [typed.column("bank", "u8")])
    for start, end, comment in _records(writer, data, count, base, starts, lambda index: f"sound set {index:X}"):
        writer.comment(comment)
        at = start

        def part(at):
            cells = [_hex(byte) for byte in data[at:at + PART_SIZE]]
            cells[4] = str(struct.unpack("b", data[at + 4:at + 5])[0])
            writer.row(cells, f"channel {data[at + 1]:X}")
        if end - at >= MUSIC_PARTS * PART_SIZE + 1:
            for _ in range(MUSIC_PARTS):
                part(at)
                at += PART_SIZE
            writer.row(["bank", _hex(data[at])])
            at += 1
        while end - at >= PART_SIZE:
            part(at)
            at += PART_SIZE
        if at < end:
            writer.comment("after the parts: nothing reads it")
            writer.raw(data[at:end])
    return writer.text()


# ---- instruments ----

INSTRUMENTS_HEADER = ("instruments (play_midi_message 03D3): by program 00-7F, a pointer to its instrument: its flags "
                      "(pcm 01, fm 02: bit 1), then key splits of 4 bytes, the last one for the highest notes: top "
                      "(the split takes the notes up to it, after the transpose), then for a MultiPCM instrument the "
                      "sample (register 1), tune (pitch steps added to the bend) and octave (its high nibble added to "
                      "the note's octave); for an FM instrument the note on reads only the patch (fm_patches; "
                      "apply_fm_*_volume read the top byte as the patch).")
SPLIT_COLUMNS = {"pcm": ("top", "sample", "tune", "octave"), "fm": ("top", "patch", "at_2", "at_3")}


def instruments_to_text(data):
    writer = typed.Writer()
    _header(writer, INSTRUMENTS_HEADER)
    base = _base("instruments")
    count, starts = _pointer_table(writer, data, base, 128, lambda index: f"program 0x{index:02X}")
    writer.record("pcm", 1, [typed.column("flags", "u8", fixed=0x01)])
    writer.record("fm", 1, [typed.column("flags", "u8", fixed=0x02)])
    writer.record("flags", 1, [typed.column("flags", "u8")])
    shape = None
    for start, end, comment in _records(writer, data, count, base, starts, lambda index: f"program 0x{index:02X}"):
        flags = data[start]
        kind = {0x01: "pcm", 0x02: "fm"}.get(flags)
        if (kind or "pcm") != shape and end - start >= 5:
            shape = kind or "pcm"
            writer.layout(None, f"{shape}_split", 4, [typed.column(name, "u8") for name in SPLIT_COLUMNS[shape]])
        writer.comment(comment)
        writer.row([kind] if kind else ["flags", _hex(flags)])
        at = start + 1
        while end - at >= 4:
            top, sample, tune, octave = data[at:at + 4]
            note = f"octave {_signed(octave >> 4, 4)}" if shape == "pcm" and octave and not octave & 0x0F else None
            writer.row([_hex(top), _hex(sample), _hex(tune), _hex(octave)], note)
            at += 4
        if at < end:
            writer.raw(data[at:end])
    return writer.text()


# ---- FM patches ----

PATCH_SIZE = 25
PATCH_ROWS = (     # (keyword, its registers' offset in the patch, how many, split)
    ("detune_multiple", 0, 4, 4), ("key_scale_attack", 4, 4, 6), ("am_decay", 8, 4, 7), ("sustain_rate", 12, 4, None),
    ("sustain_level_release", 16, 4, 4), ("feedback_algorithm", 20, 1, 3), ("level", 21, 4, 7))
KEYWORD_WIDTH = max(len(keyword) for keyword, _, _, _ in PATCH_ROWS)
PATCHES_HEADER = ("fm_patches (load_fm_patch 14F6): by patch number, a pointer to its 25 bytes: the 21 registers of "
                  "fm_patch_registers, then the four operators' total levels (fm_level_registers), a row a register "
                  "group, its operators in the registers' order (+0, +4, +8, +C: operators 1, 3, 2, 4): "
                  "detune_multiple DT:MUL, key_scale_attack KS:AR, am_decay AM:D1R, sustain_rate D2R, "
                  "sustain_level_release D1L:RR, feedback_algorithm FB:ALG, level FIXED:TL (TL the attenuation, 0 "
                  "loudest; FIXED, bit 7: the note's level is not added to it).")


def fm_patches_to_text(data):
    writer = typed.Writer()
    _header(writer, PATCHES_HEADER)
    base = _base("fm_patches")
    count, starts = _pointer_table(writer, data, base, 32, lambda index: f"patch 0x{index:02X}")
    for keyword, _, many, split in PATCH_ROWS:
        names = [f"operator_{OPERATOR_OF_SLOT[slot]}" for slot in range(many)] if many > 1 else ["value"]
        writer.record(keyword, many, [typed.column(name, "u8", split) for name in names])
    for start, end, comment in _records(writer, data, count, base, starts, lambda index: f"patch 0x{index:02X}"):
        writer.comment(comment)
        at = start
        while end - at >= PATCH_SIZE:
            for keyword, offset, many, split in PATCH_ROWS:
                values = data[at + offset:at + offset + many]
                # (the rows of a patch lined up as one table: a row each keyword, the operators in columns)
                writer.row([keyword.ljust(KEYWORD_WIDTH)] + [(typed.split_text(value, split) if split is not None
                                                             else _hex(value)).rjust(4) for value in values])
            at += PATCH_SIZE
        if at < end:
            writer.raw(data[at:end])
    return writer.text()


# ---- drum kits ----

KITS_HEADER = ("drum_kits (play_midi_message 0372): by program F0-FF (kit 0-F), a pointer to its notes, 3 bytes a "
               "MIDI note from 0 (the row's [N]): the pitch (MultiPCM register 3: the octave and the F-number's top "
               "bits; register 2 is 0), the time-out (drum_timeout: drum_timeout_tick stops the note when it runs "
               "out) and the sample (register 1; FF: the note plays nothing). A kit ends where the next one starts: "
               "one shorter than 128 notes reads its last ones from the next.")


def drum_kits_to_text(data):
    writer = typed.Writer()
    _header(writer, KITS_HEADER)
    base = _base("drum_kits")
    count, starts = _pointer_table(writer, data, base, 16, lambda index: f"kit {index:X} (program F{index:X})")
    writer.layout(None, "drum_note", 3, [typed.column("pitch", "u8"), typed.column("timeout", "u8"),
                                          typed.column("sample", "u8")])
    for start, end, comment in _records(writer, data, count, base, starts, lambda index: f"kit {index:X}"):
        writer.comment(comment)
        notes = (end - start) // 3
        for note in range(notes):
            pitch, timeout, sample = data[start + note * 3:start + note * 3 + 3]
            writer.row([f"[{note}]", _hex(pitch), _hex(timeout), _hex(sample)], "no sound" if sample == 0xFF else None)
        if start + notes * 3 < end:
            writer.raw(data[start + notes * 3:end])
    return writer.text()


# ---- tyre noises ----

def tyre_noises_to_text(data):
    writer = typed.Writer()
    writer.comment("tyre_noises (update_tyre_noise 3DE6, start_tyre_noise 3E86): by the V70's request 1-8 (the screen")
    writer.comment("page's tyre_noise), the noise: its sample (register 1), pitch (register 3), level (register 5) and")
    writer.comment("duration (main loop passes without a request before it is keyed off)")
    writer.layout(None, "tyre_noise", 4, [typed.column(name, "u8") for name in ("sample", "pitch", "level", "duration")])
    count = len(data) // 4
    for index in range(count):
        writer.row([f"[{index}]"] + [_hex(byte) for byte in data[index * 4:index * 4 + 4]], f"request {index + 1}")
    writer.raw(data[count * 4:])
    return writer.text()


FORMATS = {name: (".txt", to_text, read) for name, to_text in (
    ("sound_register_pairs", register_pairs_to_text),
    ("sound_voice_channels", voice_channels_to_text),
    ("sound_register_list", register_list_to_text),
    ("sound_semitone_pointers", semitone_pointers_to_text),
    ("sound_fm_fnum_tables", _pitch_tables("fm_fnum_tables", FNUM_HEADER, 11)),
    ("sound_pcm_pitch_tables", _pitch_tables("pcm_pitch_tables", PCM_PITCH_HEADER, 12)),
    ("sound_note_codes", note_codes_to_text),
    ("sound_velocity_levels", _value_list("u8", VELOCITY_HEADER, "velocity")),
    ("sound_bend_range_tables", bend_tables_to_text),
    ("sound_part_templates", part_templates_to_text),
    ("sound_instruments", instruments_to_text),
    ("sound_fm_patches", fm_patches_to_text),
    ("sound_drum_kits", drum_kits_to_text),
    ("sound_revs_levels", _value_list("u8", ENGINE_LEVELS_HEADER, "revs")),
    ("sound_revs_pitches", engine_pitches_to_text),
    ("sound_revs_list", _value_list("u8", SWEEP_HEADER, "step")),
    ("sound_tyre_noises", tyre_noises_to_text),
)}
