"""
Name:        orunners_rom.py
Description: MAME 'orunners' ROM set (OutRunners, World) and memory region builders
Version:     0.1.0
Created:     19-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

Files are identified by CRC32 + SHA1, not by name: dumps circulate with the current names
(epr-15620.ic37) and older ones without the hyphen (epr15620.ic37). Accepts a folder or the .zip as is.
Region layouts follow ROM_START( orunners ) in MAME src/mame/sega/segas32.cpp (BSD-3-Clause).
"""
import hashlib
import zipfile
import zlib
from pathlib import Path

SET_NAME = "orunners"

# MAME name -> (crc32, sha1, size)
ROM_FILES = {
    "epr-15620.ic37": ("84f5ad92", "1f9cb04b42b2d450be93400d9979a7910eaf05d1", 0x020000),
    "epr-15621.ic40": ("d98b765a", "b58567e976228267a86af53de2135bc0b247a44a", 0x020000),
    "mpr-15538.ic36": ("93958820", "e19b6f18a5707dbb64ae009d63c05eac5bac4a81", 0x080000),
    "mpr-15539.ic39": ("219760fa", "bd62a83de9c9542f6da454a87dc4947492f65c52", 0x080000),
    "epr-15550.ic31": ("0205d2ed", "3475479e1a45fe96eefbe53842758898db7accbf", 0x080000),
    "mpr-15548.ic3": ("b6470a66", "e1544590c02d41f62f82a4d771b893fb0f2734c7", 0x200000),
    "mpr-15549.ic11": ("81d12520", "1555893941e832f00ad3d0b3ad0c34a0d3a1c58a", 0x200000),
    "mpr-15540.ic14": ("a10d72b4", "6d9d5e20be6721b53ce49df4d5a1bbd91f5b3aed", 0x200000),
    "mpr-15542.ic15": ("40952374", "c669ef52508bc2f49cf812dc86ac98fb535471fa", 0x200000),
    "mpr-15544.ic10": ("39e3df45", "38a7b21617b45613b05509dda388f8f7770b186c", 0x200000),
    "mpr-15546.ic38": ("e3fcc12c", "1cf7e05c7873f68789a27a91cddf471df40d7907", 0x200000),
    "mpr-15541.ic22": ("a2003c2d", "200a2c7d78d3f5f28909267fdcdbddd58c5f5fa2", 0x200000),
    "mpr-15543.ic23": ("933e8e7b", "0d53286f524f47851a483569dc37e9f6d34cc5f4", 0x200000),
    "mpr-15545.ic18": ("53dd0235", "4aee5ae1820ff933b6bd8a54bdbf989c0bc95c1a", 0x200000),
    "mpr-15547.ic41": ("edcb2a43", "f0bcfcc749ca0267f85bf9838164869912944d00", 0x200000),
    "mpr-15551.ic1": ("4894bc73", "351f5c03fb430fd87df915dfe3a377b5ada622c4", 0x200000),
    "mpr-15552.ic2": ("1c4b5e73", "50a8e9a200575a3522a51bf094aa0e87b90bb0a3", 0x200000),
}

# Windows of the V70 program image with unique content; the rest are mirrors. The two code chips make
# 256 KB that ROM_LOAD32_WORD_x4 repeats four times over 0x000000-0x0FFFFF, and the bus shows the first
# MB again at 0xF00000 (the reset vector, 0xFFFFF0). The code refers to the first copy: boot jumps to
# 0x000006 and points the vector table (SBR) at 0x03F000.
PROGRAM_AREAS = [
    (0x000000, 0x040000),  # epr-15620.ic37 + epr-15621.ic40, a 16-bit word of each in every 32-bit word
    (0x100000, 0x200000),  # mpr-15538.ic36 + mpr-15539.ic39, the same way
]


def read_files(source):
    """Return {original_name: bytes} from a folder or a .zip."""
    source = Path(source)
    contents = {}
    if source.is_file() and source.suffix.lower() == ".zip":
        with zipfile.ZipFile(source) as archive:
            for entry in archive.infolist():
                if not entry.is_dir():
                    contents[Path(entry.filename).name] = archive.read(entry)
        return contents
    for path in sorted(source.iterdir()):
        if path.is_file():
            contents[path.name] = path.read_bytes()
    return contents


def identify_set(source):
    """Return ({mame_name: bytes}, [report lines]), matching every file by its hashes. A folder that holds
    none of the set's files loose is read through the first .zip in it that does."""
    found, report = _identify(source)
    if not found and Path(source).is_dir():
        for archive in sorted(Path(source).glob("*.zip")):
            found, report = _identify(archive)
            if found:
                break
    return found, report


def _identify(source):
    by_crc = {crc: name for name, (crc, _, _) in ROM_FILES.items()}
    found = {}
    report = []
    for original_name, data in read_files(source).items():
        crc = format(zlib.crc32(data) & 0xFFFFFFFF, "08x")
        mame_name = by_crc.get(crc)
        if mame_name and ROM_FILES[mame_name][1] == hashlib.sha1(data).hexdigest():
            found[mame_name] = data
            report.append(f"OK      {original_name:20} = {mame_name}")
        else:
            report.append(f"--      {original_name:20} not part of the set (crc {crc})")
    for mame_name in sorted(set(ROM_FILES) - set(found)):
        report.append(f"MISSING {mame_name}")
    return found, report


def load_set(source):
    """Like identify_set, but stops when a file is missing."""
    found, _ = identify_set(source)
    missing = sorted(set(ROM_FILES) - set(found))
    if missing:
        raise SystemExit(f"The '{SET_NAME}' ROM set is incomplete. Missing: " + ", ".join(missing))
    return found


def interleave16(even, odd):
    """ROM_LOAD16_BYTE: even bytes from one chip, odd bytes from the other."""
    result = bytearray(len(even) * 2)
    result[0::2] = even
    result[1::2] = odd
    return result


def interleave32(low, high):
    """ROM_LOAD32_WORD: every 32-bit word takes a 16-bit word of each chip, the first chip's first."""
    result = bytearray(len(low) * 2)
    for lane, chip in enumerate((low, high)):
        result[lane * 2::4] = chip[0::2]
        result[lane * 2 + 1::4] = chip[1::2]
    return result


def build_program(rom):
    """mainpcb:maincpu region (2 MB, little endian) as the V70 sees it at 0x000000-0x1FFFFF."""
    image = bytearray(0x200000)
    code = interleave32(rom["epr-15620.ic37"], rom["epr-15621.ic40"])
    image[0x000000:0x100000] = code * 4
    image[0x100000:0x200000] = interleave32(rom["mpr-15538.ic36"], rom["mpr-15539.ic39"])
    return bytes(image)


def build_tiles(rom):
    """mainpcb:tiles region (4 MB): 32768 tiles of 16x16 pixels at 4 bpp, 128 bytes each."""
    return bytes(interleave16(rom["mpr-15548.ic3"], rom["mpr-15549.ic11"]))


# the sprite chips: four 16-bit lanes of 8-byte groups from each base
SPRITE_CHIPS = [
    (0x000000, ["mpr-15540.ic14", "mpr-15542.ic15", "mpr-15544.ic10", "mpr-15546.ic38"]),
    (0x800000, ["mpr-15541.ic22", "mpr-15543.ic23", "mpr-15545.ic18", "mpr-15547.ic41"]),
]


def build_sprites(rom):
    """mainpcb:sprites region (16 MB) in file byte order: every 32-bit word is big endian.

    ROM_LOAD64_WORD spreads 16-bit words from four chips over 8-byte groups.
    """
    image = bytearray(b"\xff" * 0x1000000)
    for base, chips in SPRITE_CHIPS:
        for lane, name in enumerate(chips):
            data = rom[name]
            end = base + (len(data) // 2) * 8
            image[base + lane * 2:end:8] = data[0::2]
            image[base + lane * 2 + 1:end:8] = data[1::2]
    return bytes(image)


def build_sound(rom):
    """mainpcb:soundcpu region (512 KB): the Z80 program, 0x0000-0x9FFF fixed and 8 KB pages at 0xA000."""
    return bytes(rom["epr-15550.ic31"])


def build_pcm(rom):
    """mainpcb:sega region (4 MB): the MultiPCM's samples, with its sample table at the start."""
    return bytes(rom["mpr-15551.ic1"] + rom["mpr-15552.ic2"])


def chips_from_regions(regions):
    """The set's chips back from the regions the build_* functions lay out ({region name: bytes} for
    program, tiles, sprites, sound and pcm): {mame_name: bytes}. Each chip is read from the copy its
    region's builder writes first; the mirrors are not read."""
    program, tiles, sprites, sound, pcm = (regions[name] for name in ("program", "tiles", "sprites", "sound", "pcm"))

    def lanes32(image, start, size):
        low, high = bytearray(size), bytearray(size)
        for lane, chip in enumerate((low, high)):
            chip[0::2] = image[start + lane * 2:start + size * 2:4]
            chip[1::2] = image[start + lane * 2 + 1:start + size * 2:4]
        return low, high

    chips = {}
    chips["epr-15620.ic37"], chips["epr-15621.ic40"] = lanes32(program, 0x000000, 0x020000)
    chips["mpr-15538.ic36"], chips["mpr-15539.ic39"] = lanes32(program, 0x100000, 0x080000)
    chips["mpr-15548.ic3"] = tiles[0::2]
    chips["mpr-15549.ic11"] = tiles[1::2]
    chips["epr-15550.ic31"] = sound
    chips["mpr-15551.ic1"] = pcm[:0x200000]
    chips["mpr-15552.ic2"] = pcm[0x200000:]
    for base, names in SPRITE_CHIPS:
        for lane, name in enumerate(names):
            size = ROM_FILES[name][2]
            end = base + (size // 2) * 8
            chip = bytearray(size)
            chip[0::2] = sprites[base + lane * 2:end:8]
            chip[1::2] = sprites[base + lane * 2 + 1:end:8]
            chips[name] = chip
    return {name: bytes(chip) for name, chip in chips.items()}


def canonical_address(address):
    """Map a V70 address to its copy inside PROGRAM_AREAS, or None if it is not ROM.

    The bus decodes 24 bits; 0xF00000-0xFFFFFF mirrors the first MB of the region (MAME's map).
    """
    address &= 0xFFFFFF
    if address >= 0xF00000:
        address -= 0xF00000
    elif address >= 0x200000:
        return None
    if address < 0x100000:
        return address & 0x03FFFF
    return address


class ProgramMemory:
    """V70 view of the program ROM for the disassembler: mirrors folded, anything else reads 0xFF."""

    def __init__(self, image):
        self.image = image

    def r8(self, address):
        offset = canonical_address(address)
        return self.image[offset] if offset is not None else 0xFF

    def r16(self, address):
        return self.r8(address) | (self.r8(address + 1) << 8)

    def r32(self, address):
        return self.r16(address) | (self.r16(address + 2) << 16)
