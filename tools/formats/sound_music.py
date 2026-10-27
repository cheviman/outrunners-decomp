"""
Name:        sound_music.py
Description: the sound program's music data: the sound commands, the playlists, the songs table and its
             records as text (the core's typed format), and the sequences the players read as Standard MIDI
             Files
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

How the driver reads them (src/sound/sequences.c), and how the text has them. The text is the core's typed
format (tools/core/formats/typed.py): every text format's from_text is typed.from_text with the sound
program's names (sound_names.resolve; assets/sound/data/names.txt gives their offsets in the chip).

- sound_commands (44DB): start_sound takes the command byte the V70 writes for a screen (screen_1_command,
  screen_2_command) as the index of a 16-bit pointer to the sound's header. Its first byte, the flags, says
  what the header is:
      bit 7 set:   an effect: bits 0-3 its priority (a player that plays a sound of a higher one keeps it);
                   bit 6 clear: the screen's effect player (EB00, screen 2 F300), bit 6 set: the player of
                   the channel of the sequence's first event (A-E: EC00-F000, screen 2 F400-F800); the
                   sequence follows the flags byte.
      bit 6 set:   the music player (E900, F100), the sequence follows the flags byte (no command has it).
      bit 5 set:   a song record: the bank, then the 16-bit address of the sequence (start_sequence).
      else:        a playlist: the first song's number, then the playlist.
  Text: 'layout u16', a row a command: "[1]  playlist_magical_sound_shower  # command 0x01".
- songs (439B): start_song: by song number, the address of a song record. Text: 'layout u16', a row a song:
  "[4]  song_records+0x8  # song 0x04".
- song records (song_records): start_sequence copies the bank to the player's bank and the address to its
  position. An address in A000-BFFF is in the bank's window (chip offset = bank * 2000h + address - A000h);
  below A000 the start of the chip, the bank not read. Text: 'layout song_record 4 flags:u8 bank:u8
  address:u16': a sequence below A000 by its name ("[0]  0x20  0x00  stop_music_sequence"), one in the banked
  part as the numbers, its name after '#' ("[2]  0x20  0x08  0xA000  # magical_sound_shower_04, +0x8": a
  name gives the chip's offset, which is not what the record holds there).
- playlists (a piece each, or several in a row): when a sequence ends (step_player) a player with a playlist
  (flags bit 5) reads its next byte: 00 stops it, 01-EF is the next song (start_song), F0-FF a command of
  playlist_command_table: F0 (and F3-FD) jumps to the 16-bit address after it and plays the song there,
  F1/F2 play song 90h + playlist_choice_1/playlist_choice_2 & 7, FE stops this player, FF clears all eight
  of the screen's players. Its header (flags, the first song) is what start_sound reads; it starts the first
  song without looking at it. Text: a 'record' line for each kind of row the piece has (playlist: flags and
  first song; song; jump: F0 and the place; jump_command: F3-FD and the place; chosen_song_1, chosen_song_2,
  stop_player, stop_all_players, end: their byte alone), then a row a byte of the piece ("playlist  0x10
  0x04", "song  0x05", "jump  playlist_magical_sound_shower+0x1A", "end"), so what follows a jump (never
  read) is there too; a row a jump lands on says its place (+0xNN) after '#'.
- playlist_command_table (401E): by the low nibble of a playlist command, where the driver jumps: the code
  is the C's, only its label goes back. Text: 'layout u16', "[0]  playlist_jump  # playlist byte 0xF0".

- The sequences (step_player, next_sequence_byte, chain_next_event): an event stream much like the track
  of a Standard MIDI File, read once every four of the YM3438's timer A interrupts (1/873.9 s, a tick):
      event, delta time, event, delta time, ..., FF 2F
  The delta time comes after the event, the ticks to the next one: one byte 00-7F, or two bytes 1hhhhhhh
  0lllllll ((h << 7) | l; 80 00 waits 65536 ticks). An event is a status byte (or none: the running status,
  the last status byte read, FF and F0-FE too) and its data bytes:
      8n kk         note off: one data byte, the driver queues velocity 7F
      9n kk vv, An kk vv, Bn cc vv, En ll mm   two data bytes; Cn pp, Dn vv   one
      FF 2F         the end (the byte after it, 00 in the game's sequences, is not read)
      FF 51 ll vv   four bytes (meta_51_value); FF tt ll ...: ll bytes (00: 256); F0 ... F7; F7 ll ...
                    (ll counts itself); F1-F6, F8-FE end the sequence too
  Bit 7 of an event's last data byte (the only data byte whose bit 7 the driver takes off) chains: the next
  event follows at once, with no delta time byte; a delta time 00 does the same. The game's sequences use
  both, with no rule that tells which: both have to come back. A sequence in the banked part runs on through
  the banks (the chip's bytes one after another).

The sequences as MIDI files: sound_sequence (a song: the sequence alone) and sound_effect (a sound header:
its flags byte, then the sequence).

Writing (sequence_to_midi): "MThd", length 6, format 0, 1 track, 384 ticks a quarter note; then one "MTrk"
chunk. One tick of the file is one tick of the driver; the tempo event makes a player play them at the
driver's speed, 136.5 quarter notes a minute (the music's bars do not fall on the file's: its delta times
come from real time). The track, each event after its delta time (a variable-length quantity):
  1. 00 FF 51 03 06 B4 6B: the tempo, 439403 microseconds a quarter note.
  2. (sound_effect) 00 FF 7F 03 7D 48 FL: the flags byte FL; then 00 FF 01 LL and LL bytes of ASCII text,
     what the flags say (for whoever reads the file: the way back ignores it).
  3. When the bytes after the driver's FF 2F are not one 00: 00 FF 7F LL 7D 41 and those bytes.
  4. The driver's events in order. The delta time before the first is 00; before each other one, the delta
     time the driver has after the event before it: its bytes as they are (one byte 01-7F, or two bytes), a
     chain (bit 7 on the event before) as 00, the one-byte 00 after a channel event as 80 00 (the same zero,
     two bytes) and after any other event as 00, the two-byte 80 00 (65536 ticks) as 84 80 00. Each event:
     - a channel event (80-EF) whose data bytes are all below 80h once the chain bit is off, written with
       its status byte when the driver has it, or without one (running status) when the driver has none and
       the file's running status is the same: as itself, its last data byte without the chain bit, and a
       note off with the velocity 7F the driver queues (8n kk 7F);
     - another channel event (a data byte of 80h or more besides the chain bit, or the driver's running
       status where the file's is not the same): FF 7F LL 7D 43 and its bytes as the driver has them (with
       or without the status byte), the last one without the chain bit;
     - any other event of the driver (FF meta events but the end, F0 and F7 system exclusive): FF 7F LL
       7D 45 and its bytes as the driver has them.
     After an FF 7F event the writer counts no running status (the next channel event has its status byte).
  5. The end: the delta time after the last event, then FF 2F 00. When the stream does not read to an FF 2F
     with its own status byte (another end, or bytes that are no event), the delta time after the last event
     that reads, FF 7F LL 7D 54 and every byte of the piece from there, then 00 FF 2F 00.
  LL is a variable-length quantity: how many bytes follow it.

Reading (midi_to_sequence; effect_from_mid puts the flags byte before what it gives):
  1. The file: "MThd" and its length (the header chunk skipped by it), then every chunk: an "MTrk" is a
     track, any other chunk is skipped. A track's events, each after its delta time (a variable-length
     quantity, any length; its bytes are kept): FF TYPE LEN DATA (meta; LEN a variable-length quantity; FF
     2F ends the track), F0 LEN DATA and F7 LEN DATA (system exclusive), a status byte 80-EF and its data
     bytes (one for C0-DF, two for the others), or data bytes alone under the track's last status byte
     (running status; meta and system exclusive events do not change it). A status byte F1-FE other than
     F7, or running status before any status byte: not a file this reads.
  2. The tags: an FF 7F event whose data starts 7D and a tag byte. The attributes: the data after 7D 48 of
     the first 'H' event of the tracks (in their order), and after 7D 41 of the first 'A' event.
  3. The driver's events: the channel events, the 'C' (43), 'E' (45) and 'T' (54) events, and the end (FF
     2F). The track that has driver's events (a channel event or a 'C', 'E', 'T' one) is read, all its
     events; when several have, their events but the ends go into one list in the order of their times
     (the order of the tracks, then their own, among events at the same time), with an end at the time of
     the latest end or event; each event's delta time is then the difference to the one before it, as the
     shortest variable-length quantity. When no track has them: the first track's events.
  4. The sequence, event by event of that list, keeping the driver's running status (none at the start),
     the place of the byte a chain bit goes on (none at the start), and the delta times of the events that
     are not the driver's added up (pending: 0 at the start; plain: no such event since the last driver's
     event):
     - an event that is not the driver's (any other meta event, the 'H' and 'A' events, system exclusive):
       its delta time's value added to pending; plain no longer;
     - a driver's event: unless it is the first one (whose delta time, and pending, are dropped: the driver
       plays the first event at once), first the driver's delta time for the event before it, from this
       event's delta time (its bytes RAW and value V; T = pending + V):
         plain and RAW is 80 00:             the byte 00
         plain and RAW is 84 80 00:          the bytes 80 00
         plain, RAW one or two bytes, V > 0: RAW as it is
         else T = 0:                         a chain (bit 7 set on the byte a chain bit goes on) when there
                                             is one, else the byte 00
         else T < 80h:                       the byte T
         else T < 4000h:                     80h | T >> 7, then T & 7Fh
         else T = 10000h:                    the bytes 80 00
         else:                               no delta time the driver has: not a file this reads
       then pending 0, plain again, and the event:
         the end (FF 2F):  FF 2F, then the 'A' attribute's bytes, or 00 without one; the sequence ends;
         'T':              the bytes after 7D 54 as they are; the sequence ends (no FF 2F is added);
         a channel event:  its status byte when the file has it, or when it is not the driver's running
                           status (which it becomes); then its data bytes, but only the first one of a
                           note off (8n: the driver queues velocity 7F); a chain bit goes on its last byte;
         'C' or 'E':       the bytes after the tag as they are; when the first is 80h or more it becomes the
                           driver's running status; a chain bit goes on the last byte of a 'C' event (none
                           when it has no bytes), and on nothing after an 'E' one.
  5. A list with no end and no 'T': after its last event, the delta time for T = pending (as above, never
     plain), then FF 2F and the 'A' bytes (or 00).
  6. sound_effect: the 'H' attribute's first byte, then the sequence; a file without one gives no bytes when
     the sequence is empty, and is no effect this reads otherwise.
A sequencer that saves the file again writes its own delta times and running status: the bytes change (and
their count with them), and the driver plays them the same way.
"""
import struct

from formats import sound_names
from formats import typed

WINDOW = sound_names.WINDOW
BANK_SIZE = sound_names.BANK_SIZE


# ---- the text formats: the core's typed format (tools/core/formats/typed.py) ----

def read(text):
    """Every text format's from_text: the typed format, its names the sound program's."""
    return typed.from_text(text, resolve=sound_names.resolve)


def _u16(data, at):
    return data[at] | data[at + 1] << 8


def _pointer_list(header, label):
    """16-bit pointers by index, a row each, as the places they point at; label(index) after '#'."""
    def to_text(data):
        writer = typed.Writer()
        for line in header:
            writer.comment(line)
        writer.layout("u16")
        count = len(data) // 2
        for index in range(count):
            writer.row([f"[{index}]", sound_names.pointer_text(_u16(data, index * 2))], label(index))
        writer.raw(data[count * 2:])
        return writer.text()
    return to_text


COMMANDS_HEADER = (
    "sound_commands (start_sound, 4211): by the command byte the V70 writes (screen_1_command, screen_2_command),",
    "the sound's header by its name: an effect (flags 8x: the sequence follows the flags), a song record (20: the",
    "bank and the address of a sequence) or a playlist (10: the first song, then the playlist)")
SONGS_HEADER = (
    "songs (start_song, 4238): by song number, its record in song_records (flags 20, the bank and the address of",
    "the sequence); playlists name songs by these numbers")
JUMPS_HEADER = (
    "playlist_command_table (playlist_command, 3FC2): by the low nibble of a playlist byte F0-FF, the code the",
    "driver jumps to, by its label (the code is the C's: only the address goes back)")


# ---- song records ----

RECORDS_HEADER = (
    "song records (start_sequence, 4256; songs points at them, so do the commands C0-CF): the flags of a sound",
    "header (20: a song record), the bank and the Z80 address of the sequence: below A000 the start of the chip",
    "(the bank not read: the address is the sequence's place, by its name), else the bank's window A000-BFFF (chip",
    "offset = bank * 2000h + address - A000h: numbers, the sequence's name after '#'); and the record's place in",
    "the piece, which songs reads (song_records+0xNN)")


def _chip_offset(bank, address):
    return address if address < WINDOW else bank * BANK_SIZE + address - WINDOW


def song_records_to_text(data):
    writer = typed.Writer()
    for line in RECORDS_HEADER:
        writer.comment(line)
    writer.layout(None, "song_record", 4, [typed.column("flags", "u8"), typed.column("bank", "u8"),
                                           typed.column("address", "u16")])
    count = len(data) // 4
    for index in range(count):
        at = index * 4
        flags, bank, address = data[at], data[at + 1], _u16(data, at + 2)
        target = sound_names.name_of(_chip_offset(bank, address))
        cell = sound_names.pointer_text(address) if address < WINDOW else f"0x{address:04X}"
        writer.row([f"[{index}]", f"0x{flags:02X}", f"0x{bank:02X}", cell],
                   (f"{target}, " if target and address >= WINDOW else "") + f"+0x{at:X}")
    writer.raw(data[count * 4:])
    return writer.text()


# ---- playlists ----

PLAYLIST_RECORDS = {    # keyword: (its size, its columns)
    "playlist": (2, [typed.column("flags", "u8"), typed.column("first_song", "u8")]),
    "song": (1, [typed.column("song", "u8")]),
    "jump": (3, [typed.column("code", "u8", fixed=0xF0), typed.column("place", "u16")]),
    "jump_command": (3, [typed.column("code", "u8"), typed.column("place", "u16")]),
    "chosen_song_1": (1, [typed.column("code", "u8", fixed=0xF1)]),
    "chosen_song_2": (1, [typed.column("code", "u8", fixed=0xF2)]),
    "stop_player": (1, [typed.column("code", "u8", fixed=0xFE)]),
    "stop_all_players": (1, [typed.column("code", "u8", fixed=0xFF)]),
    "end": (1, [typed.column("code", "u8", fixed=0x00)]),
}
PLAYLIST_COMMANDS = {0xF1: "chosen_song_1", 0xF2: "chosen_song_2", 0xFE: "stop_player", 0xFF: "stop_all_players"}


def playlists_to_text(data):
    """A row a byte of the piece (the header two, a jump three), each kind declared by a record line before
    them; a row a jump lands on says its place (+0xNN) after '#'."""
    rows, targets = [], set()
    at, header = 0, True
    while at < len(data):
        byte = data[at]
        if header and at + 2 <= len(data) and not byte & 0xE0:
            rows.append((at, ["playlist", f"0x{byte:02X}", f"0x{data[at + 1]:02X}"]))
            at += 2
            header = False
            continue
        if byte == 0:
            rows.append((at, ["end"]))
            at += 1
            header = True
        elif byte < 0xF0:
            rows.append((at, ["song", f"0x{byte:02X}"]))
            at += 1
        elif byte in PLAYLIST_COMMANDS:
            rows.append((at, [PLAYLIST_COMMANDS[byte]]))
            at += 1
        elif at + 3 <= len(data):
            target = sound_names.pointer_text(_u16(data, at + 1))
            rows.append((at, ["jump", target] if byte == 0xF0 else ["jump_command", f"0x{byte:02X}", target]))
            if "+" in target:
                targets.add(int(target.split("+")[1], 16))
            at += 3
            header = True
        else:
            rows.append((at, None))
            at = len(data)
    writer = typed.Writer()
    for keyword in dict.fromkeys(cells[0] for _, cells in rows if cells):
        writer.record(keyword, *PLAYLIST_RECORDS[keyword])
    for offset, cells in rows:
        if cells is None:
            writer.raw(data[offset:])
            continue
        writer.row(cells, f"+0x{offset:X}" if offset in targets else None)
    return writer.text()


# ---- the sequences: the driver's reading ----

CHAIN = None    # a delta time that is the chain bit of the event before (no delta time bytes)


class Event:
    """One event of the driver: its bytes as the stream has them (the chain bit of a channel event's last
    data byte cleared), and the delta time after it (bytes, or CHAIN)."""

    def __init__(self, raw, status, explicit, channel):
        self.raw = raw
        self.status = status
        self.explicit = explicit
        self.channel = channel    # a channel event (80-EF): the next delta time may be its chain bit
        self.delta = b""


def _read_event(data, at, running):
    """The event at 'at' as step_player reads it: (Event, end) or ('end', at) for an FF 2F with its status
    byte, None where the stream does not read as an event of its own (another end, or past the data)."""
    if at >= len(data):
        return None
    if data[at] & 0x80:
        status, first, explicit = data[at], at + 1, True
    elif running is not None:
        status, first, explicit = running, at, False
    else:
        return None
    if status == 0xFF:
        if first >= len(data):
            return None
        if data[first] == 0x2F:
            return ("end", at) if explicit else None
        if data[first] == 0x51:
            end = first + 3                                # meta_51: type, length, one value byte
        elif first + 1 < len(data):
            end = first + 2 + (data[first + 1] or 256)     # a meta event: its length byte (0: 256)
        else:
            return None
    elif status == 0xF0:
        end = first
        while end < len(data) and data[end] != 0xF7:
            end += 1
        end += 1
    elif status == 0xF7:
        if first >= len(data):
            return None
        end = first + (data[first] or 256)                 # the length byte counts itself
    elif status > 0xF0:
        return None                                        # F1-FE: the end of the sequence
    else:
        end = first + (1 if status & 0xF0 in (0x80, 0xC0, 0xD0) else 2)
    if end > len(data):
        return None
    raw = bytearray(data[at:end])
    channel = status < 0xF0
    if channel:
        raw[-1] &= 0x7F
    return Event(bytes(raw), status, explicit, channel), end


def _read_delta(data, event, end):
    """The delta time after the event: (bytes or CHAIN, where the next event starts), or None."""
    if event.channel and data[end - 1] & 0x80:
        return CHAIN, end
    if end >= len(data):
        return None
    if data[end] < 0x80:
        return data[end:end + 1], end + 1
    if end + 1 >= len(data) or data[end + 1] & 0x80:
        return None
    return data[end:end + 2], end + 2


def read_sequence(data):
    """The driver's events of a sequence: (events, end, rest). With end True the stream ends with FF 2F and
    rest is what follows it; else rest is what follows the last event's delta time and does not read."""
    events = []
    at, running = 0, None
    while True:
        found = _read_event(data, at, running)
        if found is None:
            return events, False, data[at:]
        if found[0] == "end":
            return events, True, data[at + 2:]
        event, end = found
        after = _read_delta(data, event, end)
        if after is None:
            return events, False, data[at:]
        event.delta, at = after
        if event.explicit:
            running = event.status
        events.append(event)


# ---- MIDI files ----

PPQN = 384                                       # ticks a quarter note in the files
TICK_SECONDS = 4 * 16 * 144 / 8053975            # four timer A periods: (1024 - 3F0h) x 144 YM3438 clocks
TEMPO = round(PPQN * TICK_SECONDS * 1e6)         # microseconds a quarter note (439403: 136.5 a minute)
PRIVATE = 0x7D                                   # the non-commercial manufacturer id
TAG_CHANNEL, TAG_EVENT, TAG_REST, TAG_AFTER_END, TAG_HEADER = b"C", b"E", b"T", b"A", b"H"


def _vlq(value):
    out = [value & 0x7F]
    value >>= 7
    while value:
        out.append(0x80 | value & 0x7F)
        value >>= 7
    return bytes(reversed(out))


def _meta(kind, payload):
    return bytes([0xFF, kind]) + _vlq(len(payload)) + payload


def _private(tag, payload):
    return _meta(0x7F, bytes([PRIVATE]) + tag + payload)


def _file_delta(delta, channel):
    """The delta time of the file for the driver's delta time after an event."""
    if delta is CHAIN:
        return b"\x00"
    if delta == b"\x00":
        return b"\x80\x00" if channel else b"\x00"
    if delta == b"\x80\x00":
        return b"\x84\x80\x00"                   # a delta time of 0 in two bytes: 65536 ticks
    return delta


def _header_text(flags):
    """What the sound header's flags say (start_sound), for the file's text event."""
    if flags & 0x80:
        player = ("the player of the first event's channel (A-E: EC00-F000, screen 2 F400-F800)" if flags & 0x40
                  else "the effect player (EB00, screen 2 F300)")
        return f"sound header 0x{flags:02X}: an effect on {player}, priority {flags & 0x0F}"
    if flags & 0x40:
        return f"sound header 0x{flags:02X}: the music player (E900, screen 2 F100)"
    return f"sound header 0x{flags:02X}"


def sequence_to_midi(data, flags=None):
    """A sequence (with the flags byte of its sound header before it, given apart) as a MIDI file."""
    events, ended, rest = read_sequence(data)
    track = bytearray(b"\x00" + _meta(0x51, TEMPO.to_bytes(3, "big")))
    if flags is not None:
        track += b"\x00" + _private(TAG_HEADER, bytes([flags]))
        track += b"\x00" + _meta(0x01, _header_text(flags).encode("ascii"))
    if ended and rest != b"\x00":
        track += b"\x00" + _private(TAG_AFTER_END, rest)
    running = None                               # the file's running status
    delta = b"\x00"                              # the first event's
    for event in events:
        track += delta
        data_bytes = event.raw[1:] if event.explicit else event.raw
        fits = event.channel and all(byte < 0x80 for byte in data_bytes) and (event.explicit or running == event.status)
        if fits:
            track += event.raw + (b"\x7F" if event.status & 0xF0 == 0x80 else b"")
            running = event.status
        else:
            track += _private(TAG_CHANNEL if event.channel else TAG_EVENT, event.raw)
            running = None
        delta = _file_delta(event.delta, event.channel)
    if ended:
        track += delta + b"\xFF\x2F\x00"
    else:
        track += delta + _private(TAG_REST, rest) + b"\x00\xFF\x2F\x00"
    header = b"MThd" + struct.pack(">IHHH", 6, 0, 1, PPQN)
    return header + b"MTrk" + struct.pack(">I", len(track)) + bytes(track)


def _read_vlq(track, at):
    value, start = 0, at
    while True:
        if at >= len(track):
            raise ValueError("a MIDI track ends inside a variable-length number")
        byte = track[at]
        at += 1
        value = value << 7 | byte & 0x7F
        if not byte & 0x80:
            return value, track[start:at], at


def _read_track(track):
    """[(time, delta bytes, kind, status, explicit, payload)] of a MIDI track: kind 'channel' (payload the
    data bytes), 'meta' (status the meta type), 'sysex'."""
    events, at, time, running = [], 0, 0, None
    while at < len(track):
        delta, raw_delta, at = _read_vlq(track, at)
        time += delta
        if at >= len(track):
            raise ValueError("a MIDI track ends after a delta time")
        byte = track[at]
        if byte == 0xFF:
            kind = track[at + 1]
            length, _, at = _read_vlq(track, at + 2)
            events.append((time, raw_delta, "meta", kind, True, track[at:at + length]))
            at += length
            if kind == 0x2F:
                break
            continue
        if byte in (0xF0, 0xF7):
            length, _, at = _read_vlq(track, at + 1)
            events.append((time, raw_delta, "sysex", byte, True, track[at:at + length]))
            at += length
            continue
        if byte & 0x80:
            if byte > 0xEF:
                raise ValueError(f"status byte {byte:02X} in a MIDI track")
            status, explicit = byte, True
            at += 1
        elif running is None:
            raise ValueError("a running status with no status byte before it in a MIDI track")
        else:
            status, explicit = running, False
        running = status
        count = 1 if status & 0xF0 in (0xC0, 0xD0) else 2
        events.append((time, raw_delta, "channel", status, explicit, track[at:at + count]))
        at += count
    return events


DRIVER_TAGS = (TAG_CHANNEL, TAG_EVENT, TAG_REST)


def _tag(event):
    """The tag of one of these files' sequencer-specific meta events (FF 7F 7D tag ...), or None."""
    if event[2] == "meta" and event[3] == 0x7F and len(event[5]) >= 2 and event[5][0] == PRIVATE:
        return event[5][1:2]
    return None


def _is_end(event):
    return event[2] == "meta" and event[3] == 0x2F


def _read_midi(blob):
    """The events of a MIDI file the driver's stream comes from, [(time, delta bytes, kind, status,
    explicit, payload, tag, the driver's)], and the file's private attributes {tag: bytes}. The track with
    the channel events (several: merged in time order, with the delta times a sequencer would write)."""
    if blob[:4] != b"MThd":
        raise ValueError("not a MIDI file")
    at, tracks = 8 + struct.unpack(">I", blob[4:8])[0], []
    while at + 8 <= len(blob):
        kind, length = blob[at:at + 4], struct.unpack(">I", blob[at + 4:at + 8])[0]
        if kind == b"MTrk":
            tracks.append(_read_track(blob[at + 8:at + 8 + length]))
        at += 8 + length
    attributes = {}
    for track in tracks:
        for event in track:
            if _tag(event) in (TAG_HEADER, TAG_AFTER_END):
                attributes.setdefault(_tag(event), event[5][2:])
    playing = [track for track in tracks if any(event[2] == "channel" or _tag(event) in DRIVER_TAGS for event in track)]
    if len(playing) > 1:
        merged = sorted((event for track in playing for event in track if not _is_end(event)), key=lambda event: event[0])
        end = max([event[0] for track in playing for event in track if _is_end(event)] + [merged[-1][0]])
        merged.append((end, b"", "meta", 0x2F, True, b""))
        events, time = [], 0
        for event in merged:
            events.append((event[0], _vlq(event[0] - time)) + tuple(event[2:]))
            time = event[0]
    else:
        events = playing[0] if playing else (tracks[0] if tracks else [])
    return [tuple(event) + (_tag(event), event[2] == "channel" or _tag(event) in DRIVER_TAGS or _is_end(event))
            for event in events], attributes


def _driver_delta(raw, value, plain, channel):
    """The driver's delta time for the file's (CHAIN or bytes); 'plain': no event of the file's own came
    between the two events (their raw delta time is the driver's)."""
    if plain:
        if raw == b"\x80\x00":
            return b"\x00"
        if raw == b"\x84\x80\x00":
            return b"\x80\x00"
        if len(raw) in (1, 2) and value:
            return raw
    if value == 0:
        return CHAIN if channel else b"\x00"
    if value < 0x80:
        return bytes([value])
    if value < 0x4000:
        return bytes([0x80 | value >> 7, value & 0x7F])
    if value == 0x10000:
        return b"\x80\x00"
    raise ValueError(f"a delta time of {value} ticks: the driver waits at most 16383 between events")


def midi_to_sequence(blob):
    """A MIDI file back to the driver's bytes: (flags of the sound header or None, the sequence)."""
    events, attributes = _read_midi(blob)
    out = bytearray()
    running = None                    # the driver's running status
    last = None                       # where the last event's chain bit goes, None when it cannot chain
    pending, plain, started = 0, True, False
    after = attributes.get(TAG_AFTER_END, b"\x00")
    for time, raw_delta, kind, status, explicit, payload, tag, drives in events:
        value, _, _ = _read_vlq(raw_delta, 0)
        if not drives:
            pending += value
            plain = False
            continue
        if started:
            delta = _driver_delta(raw_delta, pending + value, plain, last is not None)
            if delta is CHAIN:
                out[last] |= 0x80
            else:
                out += delta
        started, pending, plain = True, 0, True
        if kind == "meta" and status == 0x2F:
            out += b"\xFF\x2F" + after
            break
        if tag == TAG_REST:
            out += payload[2:]
            break
        if kind == "channel":
            if explicit or status != running:
                out.append(status)
                running = status
            out += payload[:1] if status & 0xF0 == 0x80 else payload
            last = len(out) - 1
            continue
        raw = payload[2:]
        out += raw
        if raw and raw[0] & 0x80:
            running = raw[0]
        last = len(out) - 1 if tag == TAG_CHANNEL and raw else None
    else:
        # no end event in the file: the sequence ends right after its last event
        if started:
            delta = _driver_delta(b"", pending, False, last is not None)
            if delta is CHAIN:
                out[last] |= 0x80
            else:
                out += delta
        out += b"\xFF\x2F" + after
    flags = attributes.get(TAG_HEADER)
    return (flags[0] if flags else None), bytes(out)


def sequence_to_mid(data):
    return sequence_to_midi(data)


def sequence_from_mid(blob):
    return midi_to_sequence(blob)[1]


def effect_to_mid(data):
    """A sound header with its sequence after the flags byte (the effects)."""
    if not data:
        return sequence_to_midi(b"")
    return sequence_to_midi(data[1:], flags=data[0])


def effect_from_mid(blob):
    flags, sequence = midi_to_sequence(blob)
    if flags is None:
        if sequence:
            raise ValueError("the MIDI file has no sound header flags")
        return b""
    return bytes([flags]) + sequence


FORMATS = {
    "sound_commands": (".txt", _pointer_list(COMMANDS_HEADER, lambda index: f"command 0x{index:02X}"), read),
    "sound_songs": (".txt", _pointer_list(SONGS_HEADER, lambda index: f"song 0x{index:02X}"), read),
    "sound_jump_table": (".txt", _pointer_list(JUMPS_HEADER, lambda index: f"playlist byte 0x{0xF0 + index:02X}"), read),
    "sound_song_records": (".txt", song_records_to_text, read),
    "sound_playlists": (".txt", playlists_to_text, read),
    "sound_sequence": (".mid", sequence_to_mid, sequence_from_mid, "binary"),
    "sound_effect": (".mid", effect_to_mid, effect_from_mid, "binary"),
}
