"""
Name:        pcm_samples.py
Description: the MultiPCM's samples (the pcm region, 4 MB) as a WAV a sample, named and grouped by subject,
             and its 512 sample headers as text that names the sample each one plays; and the region
             back from them (extract_assets.py: the pcm_samples type)
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

The region holds the sample headers (12 bytes each, the first 0x1800 bytes), then the samples, each
followed by 16 empty bytes (0xFF), and empty bytes to the end of each 512 KB. The chip sees 2 MB of it:
000000-0FFFFF as they are (the fixed part), and two windows, 100000-17FFFF (screen 1's) and 180000-1FFFFF
(screen 2's), each on the 512 KB page its screen's sound set picks (runtime/src/sound.c
s32_pcm_sample_read). Screen 1's notes play headers 000-0FF, screen 2's 100-1FF: the same headers, with
screen 2's window.

pcm_samples.txt (the repository's root) says where each sample is, its length and its name: the WAV is
assets/sound/samples/NAME.wav, the chip's signed bytes + 128, 8 bits, and a 'smpl' chunk with the loop of
the header that plays it from its start when that header loops (the rate is nominal: the sound program
sets the pitch of every note). It also says which sound sets put each page in a window ('page'), and
which sets play each header that starts in a window ('uses'). assets/sound/samples/headers.txt has the
headers, a line each: the envelope and the LFOs by name, the loop and the length in samples, and the
sample it plays (see HEADERS_NOTES), by sound set for a header in a window, since each set's page puts a
different sample there.

Samples that share bytes:
- a byte-identical sample at several places (the same drum on the pages of several songs) is one WAV,
  its name on a line for each place;
- a header that starts inside another sample's data plays NAME+X, X samples into it: 032, a tom on the
  page of Magical Sound Shower's set and, for Splash Wave's set (its drum note 79), the middle of that
  page's open hi-hat; and E2-E5, which no sound set plays. One may play on past that sample's end, into
  the empty bytes and the next sample, as the region lays them out (E2 and E3);
- headers that start at the same place name the same sample;
- a sample's WAV holds its bytes up to the empty bytes that follow it: some keep a last byte their
  headers stop short of.
Everything else in the region is empty bytes; a byte anywhere else is an error.

extract() writes the files, build() lays the region out again over empty bytes from pieces(): the
headers' table and each sample at each of its places, by name (what a pack of the assets would hold).
"""
import struct
from bisect import bisect_right
from pathlib import Path

HEADER = 12                 # the bytes of a sample header
HEADERS = 0x200             # the headers at the start of the region
FIXED = 0x100000            # the chip's addresses below it are the region's own
WINDOW = 0x80000            # a page of samples, and each window of the chip
WINDOWS = {"window_a": 0x100000, "window_b": 0x180000}
EMPTY = 0xFF                # the bytes no sample holds
HEADERS_FILE = "headers.txt"
ENVELOPE = (("lfo_vibrato", 7, 0, 8), ("attack", 8, 4, 4), ("decay1", 8, 0, 4), ("decay_level", 9, 4, 4),
            ("decay2", 9, 0, 4), ("key_rate_scale", 10, 4, 4), ("release", 10, 0, 4), ("lfo_amplitude", 11, 0, 8))
HEADERS_NOTES = """\
# The MultiPCM's 512 sample headers (tools/pcm_samples.py), a line each by number (hex): 000-0FF play
# screen 1's notes, 100-1FF the same notes of screen 2, in screen 2's window.
#   NNN WHERE loop=L length=N lfo_vibrato=V attack=A decay1=D decay_level=DL decay2=D2 key_rate_scale=K
#       release=R lfo_amplitude=M [format=F] SAMPLE...
# WHERE: fixed (the chip's first MB, the same for every sound set), window_a or window_b (screen 1's or
# screen 2's window, on the page the screen's sound set picks: pcm_samples.txt). loop and length are in
# samples from where the header starts: the chip plays length samples, then goes back to loop. The
# envelope and the LFOs are as the chip takes them (runtime/src/multipcm.c); format, the start's top bits,
# only when not 0.
# SAMPLE: sample=NAME in the fixed part; sets=S,S:NAME for the sound sets that play it from their page
# (one for each page); unplayed_sets=S:NAME for a header no set plays, at the sample that starts there on
# those sets' page; NAME+X: X samples into NAME (the header starts inside its data, and may play on past
# its end); at=X: no sample starts there, X the place in the fixed part or in the window."""


def load(path):
    """pcm_samples.txt: {"pages": {page: [sets]}, "set_page": {set: page}, "samples": [(offset, length,
    name)] by offset, "uses": {header below 100: [sets]}}."""
    layout = {"pages": {}, "set_page": {}, "samples": [], "uses": {}}
    for number, line in enumerate(Path(path).read_text(encoding="ascii").splitlines(), 1):
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        try:
            if words[0] == "page" and words[2] == "sets":
                page = int(words[1])
                layout["pages"][page] = [int(word) for word in words[3:]]
                for sound_set in layout["pages"][page]:
                    layout["set_page"][sound_set] = page
            elif words[0] == "uses":
                layout["uses"][int(words[1], 16)] = [int(word) for word in words[2:]]
            else:
                layout["samples"].append((int(words[0], 16), int(words[1], 16), words[2]))
        except (IndexError, ValueError):
            raise SystemExit(f"{path}: line {number}: not a page, uses or sample line")
    layout["samples"].sort()
    end = HEADERS * HEADER
    sizes = {}
    for offset, length, name in layout["samples"]:
        if offset < end:
            raise SystemExit(f"{path}: {name} at 0x{offset:06X} overlaps what comes before it")
        if sizes.setdefault(name, length) != length:
            raise SystemExit(f"{path}: {name} has two lengths")
        end = offset + length
    return layout


def sample_at(layout, place):
    """(name, offset into it) of the sample that holds region place 'place', or None."""
    samples = layout["samples"]
    index = bisect_right(samples, (place, 1 << 30, "")) - 1
    if index >= 0:
        offset, length, name = samples[index]
        if place < offset + length:
            return name, place - offset
    return None


def sample_reference(found):
    name, into = found
    return name + (f"+{into:X}" if into else "")


def header_fields(raw):
    """A header's 12 bytes as {field: value} (start, loop, length, the envelope's)."""
    fields = {"start": int.from_bytes(raw[0:3], "big"), "loop": int.from_bytes(raw[3:5], "big"),
              "length": 0x10000 - int.from_bytes(raw[5:7], "big")}
    for name, at, shift, bits in ENVELOPE:
        fields[name] = (raw[at] >> shift) & ((1 << bits) - 1)
    return fields


def header_line(number, raw, layout, path):
    """A header's line of headers.txt, and the samples it names: [(name, offset into it)]."""
    fields = header_fields(raw)
    address = fields["start"] & 0x1FFFFF
    where = "fixed" if address < FIXED else ("window_a" if address < WINDOWS["window_b"] else "window_b")
    words = [f"{number:03X}", where, f"loop={fields['loop']:04X}", f"length={fields['length']:05X}"]
    words += [f"{name}={fields[name]:0{bits // 4}X}" for name, _, _, bits in ENVELOPE]
    if fields["start"] >> 21:
        words.append(f"format={fields['start'] >> 21:X}")
    named = []
    if where == "fixed":
        found = sample_at(layout, address)
        if found:
            named.append(found)
            words.append(f"sample={sample_reference(found)}")
        else:
            words.append(f"at={address:06X}")
        return " ".join(words), named
    offset = address & (WINDOW - 1)
    by_page = {}
    for sound_set in layout["uses"].get(number & 0xFF, []):
        if sound_set not in layout["set_page"]:
            raise SystemExit(f"{path}: header {number & 0xFF:03X}: sound set {sound_set} has no page")
        by_page.setdefault(layout["set_page"][sound_set], []).append(sound_set)
    for page, page_sets in sorted(by_page.items()):
        found = sample_at(layout, page * WINDOW + offset)
        if not found:
            raise SystemExit(f"{path}: header {number & 0xFF:03X}: no sample at 0x{page * WINDOW + offset:06X}")
        named.append(found)
        words.append("sets=" + ",".join(str(s) for s in page_sets) + ":" + sample_reference(found))
    if not by_page:
        # no set plays it: the sample that starts there on a page, when one page has one
        starting = [page for page in sorted(layout["pages"])
                    if (sample_at(layout, page * WINDOW + offset) or ("", 1))[1] == 0]
        if len(starting) == 1:
            found = sample_at(layout, starting[0] * WINDOW + offset)
            named.append(found)
            words.append("unplayed_sets=" + ",".join(str(s) for s in layout["pages"][starting[0]]) + ":" + found[0])
        else:
            words.append(f"at={offset:05X}")
    return " ".join(words), named


def write_wav(path, samples, rate, loop=None):
    """Mono 8-bit PCM (unsigned), with a 'smpl' chunk for loop = (first, last sample) when given."""
    chunks = b"fmt " + struct.pack("<IHHIIHH", 16, 1, 1, rate, rate, 1, 8)
    chunks += b"data" + struct.pack("<I", len(samples)) + samples + (b"\x00" if len(samples) & 1 else b"")
    if loop:
        chunks += b"smpl" + struct.pack("<I9I", 60, 0, 0, 1000000000 // rate, 60, 0, 0, 0, 1, 0)
        chunks += struct.pack("<6I", 0, 0, loop[0], loop[1], 0, 0)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b"RIFF" + struct.pack("<I", 4 + len(chunks)) + b"WAVE" + chunks)


def read_wav(path):
    """The samples of a mono 8 or 16-bit PCM WAV as 8-bit unsigned bytes."""
    data = Path(path).read_bytes()
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise SystemExit(f"{path}: not a WAV file")
    at, form, samples = 12, None, None
    while at + 8 <= len(data):
        kind, size = data[at:at + 4], struct.unpack("<I", data[at + 4:at + 8])[0]
        body = data[at + 8:at + 8 + size]
        if kind == b"fmt ":
            form = struct.unpack("<HH", body[:4]) + struct.unpack("<H", body[14:16])
        elif kind == b"data":
            samples = body
        at += 8 + size + (size & 1)
    if form is None or samples is None or form[0] != 1 or form[1] != 1 or form[2] not in (8, 16):
        raise SystemExit(f"{path}: not a mono 8 or 16-bit PCM WAV")
    if form[2] == 16:
        samples = bytes((value >> 8) + 0x80 for (value,) in struct.iter_unpack("<h", samples[:len(samples) // 2 * 2]))
    return samples


def extract(region, layout_path, folder, rate):
    """The region's samples as WAVs in 'folder', its headers as headers.txt there, and the layout beside them
    (samples.txt: where each sample is, which the game reads them by); the number of files."""
    layout = load(layout_path)
    folder = Path(folder)
    covered = HEADERS * HEADER
    for offset, length, name in layout["samples"]:
        if any(byte != EMPTY for byte in region[covered:offset]):
            raise SystemExit(f"pcm region: 0x{covered:06X}-0x{offset:06X}: bytes in no sample of {layout_path.name}")
        covered = offset + length
    if any(byte != EMPTY for byte in region[covered:]):
        raise SystemExit(f"pcm region: 0x{covered:06X}-0x{len(region):06X}: bytes in no sample of {layout_path.name}")
    lines = [HEADERS_NOTES]
    loops = {}      # a sample's loop: the first header's that plays it from its start
    for number in range(HEADERS):
        raw = region[number * HEADER:(number + 1) * HEADER]
        line, named = header_line(number, raw, layout, layout_path)
        lines.append(line)
        fields = header_fields(raw)
        for name, into in named:
            if not into:
                loops.setdefault(name, (fields["loop"], fields["length"]))
    (folder / HEADERS_FILE).parent.mkdir(parents=True, exist_ok=True)
    (folder / HEADERS_FILE).write_text("\n".join(lines) + "\n", encoding="ascii")
    files = 1
    first = {}
    for offset, length, name in layout["samples"]:
        data = region[offset:offset + length]
        if name in first:
            if region[first[name]:first[name] + length] != data:
                raise SystemExit(f"pcm region: {name} at 0x{offset:06X} is not the same as at 0x{first[name]:06X}")
            continue
        first[name] = offset
        loop, played = loops.get(name, (None, None))
        smpl = (loop, played - 1) if loop is not None and loop < played - 1 else None
        write_wav(folder / f"{name}.wav", bytes(byte ^ 0x80 for byte in data), rate, smpl)
        files += 1
    (folder / "samples.txt").write_text(Path(layout_path).read_text(encoding="ascii"), encoding="ascii")
    return files + 1


def sample_offsets(layout):
    offsets = {}
    for offset, length, name in layout["samples"]:
        offsets.setdefault(name, []).append(offset)
    return offsets


def header_bytes(words, layout, offsets, where_from):
    """A headers.txt line's words as the header's 12 bytes."""
    fields = dict(word.split("=", 1) for word in words[2:] if "=" in word)
    where = words[1]
    if where != "fixed" and where not in WINDOWS:
        raise SystemExit(f"{where_from}: '{where}' is no place of the chip")
    address = None
    for key, value in ((word.split("=", 1)) for word in words[2:]):
        if key not in ("sample", "sets", "unplayed_sets", "at"):
            continue
        if key == "at":
            place = int(value, 16)
        else:
            sets, _, reference = value.rpartition(":")
            name, _, into = reference.partition("+")
            if name not in offsets:
                raise SystemExit(f"{where_from}: no sample {name} in pcm_samples.txt")
            if key == "sample":
                places = [offset for offset in offsets[name] if offset < FIXED]
            else:
                pages = {layout["set_page"].get(int(s)) for s in sets.split(",")}
                places = [offset for offset in offsets[name] if offset // WINDOW in pages]
            if not places:
                raise SystemExit(f"{where_from}: {name} is not where its sound sets read it")
            place = places[0] + int(into or "0", 16)
            if where != "fixed":
                place &= WINDOW - 1
        if address is None:
            address = place
        elif address != place:
            raise SystemExit(f"{where_from}: its samples start at different places of the window")
    if address is None:
        raise SystemExit(f"{where_from}: no sample")
    start = (address if where == "fixed" else WINDOWS[where] + address) | int(fields.get("format", "0"), 16) << 21
    raw = bytearray(start.to_bytes(3, "big"))
    raw += int(fields["loop"], 16).to_bytes(2, "big")
    raw += ((0x10000 - int(fields["length"], 16)) & 0xFFFF).to_bytes(2, "big")
    raw += bytes(5)
    for name, at, shift, bits in ENVELOPE:
        raw[at] |= (int(fields[name], 16) & ((1 << bits) - 1)) << shift
    return bytes(raw)


def pieces(folder, layout_path):
    """The region's pieces from the WAVs and headers.txt in 'folder', by pcm_samples.txt: [(name, offset,
    bytes)], the headers ("headers") and each sample at each of its places; the rest is EMPTY."""
    layout = load(layout_path)
    folder = Path(folder)
    offsets = sample_offsets(layout)
    table = bytearray(HEADERS * HEADER)
    headers_path = folder / HEADERS_FILE
    numbers = []
    for number, line in enumerate(headers_path.read_text(encoding="ascii").splitlines(), 1):
        words = line.split()
        if not words or words[0].startswith("#"):
            continue
        where_from = f"{headers_path}: line {number}"
        try:
            header = int(words[0], 16)
            raw = header_bytes(words, layout, offsets, where_from)
        except (IndexError, KeyError, ValueError):
            raise SystemExit(f"{where_from}: not a header line")
        if not 0 <= header < HEADERS:
            raise SystemExit(f"{where_from}: no header {words[0]}")
        table[header * HEADER:(header + 1) * HEADER] = raw
        numbers.append(header)
    if sorted(numbers) != list(range(HEADERS)):
        raise SystemExit(f"{headers_path}: not the {HEADERS} headers, each once")
    found = [("headers", 0, bytes(table))]
    cache = {}
    for offset, length, name in layout["samples"]:
        if name not in cache:
            cache[name] = bytes(byte ^ 0x80 for byte in read_wav(folder / f"{name}.wav"))
        if len(cache[name]) != length:
            raise SystemExit(f"{folder / name}.wav: {len(cache[name])} samples, pcm_samples.txt gives {length}")
        found.append((name, offset, cache[name]))
    return found


def build(folder, layout_path, size):
    """The region back from the WAVs and headers.txt in 'folder', by pcm_samples.txt."""
    region = bytearray([EMPTY]) * size
    for _, offset, data in pieces(folder, layout_path):
        if offset + len(data) > size:
            raise SystemExit(f"pcm_samples.txt: a sample at 0x{offset:06X} goes past the region's end")
        region[offset:offset + len(data)] = data
    return bytes(region)
