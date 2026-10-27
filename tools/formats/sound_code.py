"""
Name:        sound_code.py
Description: Z80 code no code reaches, as assembly: the sound program's pieces of code the analysis does
             not reach (the C has no part of them), and the list of commands one of them reads
Version:     0.1.0
Created:     27-10-2026
Modified:    27-10-2026
Author:      Sebastian Toyos

The bytes of a piece of unreached code read as Z80 instructions, one a line, in MAME's syntax (the core's
z80/disassembler.py: the same text MAME prints), with the instruction's place in the piece after '#' (and
the name of a jump's or a call's target, sound_symbols.txt's). A
relative jump (JR, DJNZ) gives its target as $+N or $-N, from the instruction's own place, so the text
does not depend on where the piece is. On the way back each line is assembled: of the encodings whose
text has the line's shape, the first that decodes to the very same text (the Z80 has a few with the same
text: the first of the table is taken). 'db $XX' is a byte that is no instruction (or the start of one the
piece cuts).

- sound_z80_code: the instructions.
- sound_command_list: sound command bytes in the core's typed format ('layout u8', 16 a row).
"""
import re

from formats import sound_names
from formats import typed
from z80 import disassembler

_SHAPES = {}
NUMBER = re.compile(r"\$[0-9A-Fa-f]+")
RELATIVE = ("jr", "djnz")


def _decode(code, at=0):
    """The instruction at 'at' of 'code' (bytes past the end read as 0), decoded at Z80 address 'at'."""
    padded = bytes(code) + bytes(4)
    return disassembler.decode(disassembler.FlatMemory(padded, 0xFFFF), at)


def _normal(text):
    """An instruction's text with single spaces: 'ld   a,b' and 'ld a,b' are the same."""
    return " ".join(text.split())


def _shape(text):
    """The text with its numbers out (and a displacement's sign): what the encodings of one form share."""
    return NUMBER.sub("$", _normal(text)).replace("-$", "+$")


def _encodings():
    """{shape: [opcode bytes, the operand bytes zero]} of every instruction the decoder knows."""
    if _SHAPES:
        return _SHAPES
    prefixes = [[op] for op in range(256) if op not in (0xCB, 0xDD, 0xED, 0xFD)]
    prefixes += [[0xCB, op] for op in range(256)] + [[0xED, op] for op in range(256)]
    for index in (0xDD, 0xFD):
        prefixes += [[index, op] for op in range(256) if op != 0xCB]
        prefixes += [[index, 0xCB, 0, op] for op in range(256)]
    for prefix in prefixes:
        instruction = _decode(prefix)
        if instruction.mnemonic == "db":
            continue
        encoding = bytes(prefix) + bytes(max(0, instruction.length - len(prefix)))
        _SHAPES.setdefault(_shape(instruction.text), []).append(encoding)
    return _SHAPES


def _absolute(text, at):
    """A line's relative target ($+N) as the address the decoder prints at 'at'."""
    mnemonic = text.split()[0]
    if mnemonic not in RELATIVE:
        return text
    match = re.search(r"\$([+-]\d+)$", text)
    if not match:
        return text
    return text[:match.start()] + f"${(at + int(match.group(1))) & 0xFFFF:04X}"


def assemble(text, at):
    """The bytes of instruction 'text' at place 'at' of its piece."""
    words = text.split()
    if words[0] == "db" and len(words) == 2:
        return bytes([int(words[1].lstrip("$"), 16)])
    wanted = _normal(_absolute(text, at))
    numbers = [match.group()[1:] for match in NUMBER.finditer(wanted)]
    for encoding in _encodings().get(_shape(wanted), []):
        for candidate in _fills(encoding, numbers, at):
            instruction = _decode(bytes(at) + candidate, at)
            if _normal(instruction.text) == wanted and instruction.length == len(candidate):
                return candidate
    raise ValueError(f"not a Z80 instruction: {text}")


def _fills(encoding, numbers, at):
    """Ways to put the line's numbers in the encoding's operand bytes, in the order they are printed (the
    decoding checks which one is right): as they are; a jump target as a relative offset; a displacement
    negated (it is printed without its sign's bits)."""
    values = [int(number, 16).to_bytes(2, "little") if len(number) > 2 else bytes([int(number, 16)])
              for number in numbers]
    if not values:
        yield encoding
        return
    if encoding[:2] in (bytes((0xDD, 0xCB)), bytes((0xFD, 0xCB))):
        for value in values:
            yield encoding[:2] + value + encoding[3:]
            yield encoding[:2] + bytes([-value[0] & 0xFF]) + encoding[3:]
        return
    length = len(encoding)
    for joined in (b"".join(values), b"".join(bytes([-value[0] & 0xFF]) if len(value) == 1 else value for value in values)):
        if len(joined) < length:
            yield encoding[:length - len(joined)] + joined
    if len(values) == 1 and len(values[0]) == 2:
        target = int.from_bytes(values[0], "little")
        yield encoding[:length - 1] + bytes([(target - (at + length)) & 0xFF])
    yield encoding      # a number that is part of the opcode (RST's vector)


def code_to_text(data):
    lines = []
    at = 0
    while at < len(data):
        instruction = _decode(data, at)
        text, length = instruction.text, instruction.length
        if instruction.mnemonic in RELATIVE and instruction.target is not None:
            text = text[:text.rindex("$")] + f"${instruction.target - at:+d}"
        if instruction.mnemonic == "db" or at + length > len(data):
            text, length = f"db   ${data[at]:02X}", 1
        else:
            try:
                if assemble(text, at) != data[at:at + length]:
                    text, length = f"db   ${data[at]:02X}", 1
            except ValueError:
                text, length = f"db   ${data[at]:02X}", 1
        target = instruction.target if length == instruction.length and instruction.mnemonic not in RELATIVE else None
        name = sound_names.name_of(target) if target is not None and target < sound_names.WINDOW else None
        lines.append(f"{text:<24}# +0x{at:X}" + (f"  {name}" if name else ""))
        at += length
    return "\n".join(lines) + "\n"


def code_from_text(text):
    out = bytearray()
    for line in text.split("\n"):
        line = line.split("#", 1)[0].strip()
        if line:
            out += assemble(line, len(out))
    return bytes(out)


def command_list_to_text(data):
    """Sound command bytes in the typed format (tools/core/formats/typed.py): 16 a row."""
    writer = typed.Writer()
    writer.comment("sound command bytes (sound_commands' indices), 16 a row")
    writer.layout("u8")
    for first in range(0, len(data), 16):
        writer.row([f"[{first}]"] + [f"0x{byte:02X}" for byte in data[first:first + 16]])
    return writer.text()


def command_list_from_text(text):
    return typed.from_text(text, resolve=sound_names.resolve)


FORMATS = {
    "sound_z80_code": (".txt", code_to_text, code_from_text),
    "sound_command_list": (".txt", command_list_to_text, command_list_from_text),
}
