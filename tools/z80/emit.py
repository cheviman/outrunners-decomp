# license:BSD-3-Clause
# copyright-holders:Sebastian Toyos
"""
Name:        emit.py
Description: writes the sound program's functions as C, through the decompiler's writer
Version:     0.1.0
Created:     13-10-2026
Modified:    16-10-2026
Author:      Sebastian Toyos

The decompiler's FunctionWriter does the work (decompiler/emit.py); this gives it the Z80's memory and
effects. MEMORY_MAP is the Z80's for decompiler/memory.py and decompiler/pointers.py: the RAM it shares
with the V60 (E000-FFFF) holds the variables and the structures sound_symbols.txt names, read and written
as lvalues over those bytes (SOUND_RAM8, CHANNEL(p)->field); the program ROM (0000-9FFF) the tables; the
sample bank's window (A000-BFFF) and the RF5C68 (C000-DFFF) are the board's, reached through its bus by
name. A computed address no structure explains goes over the bus, z80_read8/16() and z80_write8/16()
(runtime/include/sound/support.h). The timing model's Effects (z80_block, z80_cycles), DI and EI, OUT and
the block moves print as calls of that header, the ports by the names sound_symbols.txt gives them.
"""
from decompiler import emit
from v60 import ir

SHARED_RAM = (0xE000, 0x10000)
PROGRAM_ROM = (0x0000, 0xA000)


def use_memory_map(names, arrays):
    """Makes the Z80's memory the one decompiler/memory.py and decompiler/pointers.py work on: 'names'
    (disassemble.Names) gives the structures, 'arrays' ({struct: (first, count)}) where they stand one
    after another."""
    from decompiler import memory
    from decompiler import pointers
    memory.WORK_RAM = SHARED_RAM
    memory.ROM_AREAS = (PROGRAM_ROM,)
    memory.ADDRESS_MASK = 0xFFFF
    memory.ADDRESS_DIGITS = 4
    memory.RAM_MACRO, memory.ROM_MACRO = "SOUND_RAM", "SOUND_ROM"
    memory.RAM_FUNCTION, memory.ROM_FUNCTION = "sound_ram_at", "sound_rom_at"
    memory.PAGED = False
    memory.PLAYER_FIELDS = None
    memory.CANONICAL = lambda address: address
    memory.ROM_NAMES_FROM = 0x0100    # the restarts and the interrupt's code are below
    memory.VARIABLES = None
    memory.HARDWARE = [(0xA000, 0xC000, "SAMPLE_BANK"), (0xC000, 0xD000, "PCM"), (0xD000, 0xE000, "PCM_RAM")]
    memory.BUS_ONLY = []
    memory.BYTE_INDEXES = True
    memory.NOTES = {
        "variables": ["/* the sound program's variables, in the RAM it shares with the V60 (E000-FFFF) */"],
        "scoped": ["/* shared RAM as one family of functions uses it */"],
        "tables": ["/* data in the program ROM the code reads */"],
        "hardware": ["/* the sample bank's window and the RF5C68 (read and written through the bus) */"],
    }
    pointers.STRUCTS = dict(names.structs)
    pointers.PLAYERS = (0, 0, 0)
    pointers.VARIABLES = None
    pointers.CONTEXT_KINDS = {}
    pointers.OBJECT_REGISTERS = ()
    pointers.WORK_RAM = SHARED_RAM
    pointers.ADDRESS_MASK = 0xFFFF
    pointers.POINTER_BITS = 16
    pointers.PAGE_LINKS = False
    pointers.ROM_DATA = (0x0100, PROGRAM_ROM[1])
    pointers.ARRAYS = dict(arrays)
    pointers.STRUCT_FUNCTION = "sound_ram_at"
    pointers.ROM_STRUCTS = set(names.rom_structs)    # records of the sound program's ROM: read only
    pointers.ROM_FUNCTION = "sound_rom_at"
    pointers.CONSTANTS = True
    pointers.PAGED = True    # the sequence players, one per page from E900


class SoundNames(emit.Names):
    def memory(self, load):
        found = super().memory(load)
        if found is not None:
            return found
        # a computed address: over the board's bus
        return f"z80_read{load.bits}({self.writer.exact(load.address)})", False


class SoundPrinter(emit.Printer):
    def __init__(self, writer):
        super().__init__(writer)
        self.names = SoundNames(writer)


class SoundWriter(emit.FunctionWriter):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.printer = SoundPrinter(self)

    def statement(self, statement):
        if isinstance(statement, ir.Store):
            target = self.memory(statement.address, statement.bits, id(statement))
            if target is not None and target[2]:
                return [self.assignment(target[0], statement, target[1])]
            if target is not None:
                raise emit.EmitError(f"{self.function.name}: writes the program ROM")
            return [f"z80_write{statement.bits}({self.exact(statement.address)}, {self.stored(statement.value, statement.bits)});"]
        return super().statement(statement)

    def port(self, node):
        name = self.emitter.ports.get(node.value) if isinstance(node, ir.Const) else None
        return name or self.exact(node)

    def effect(self, statement):
        name = statement.name
        args = statement.args
        if name == "z80_block":
            return [f"z80_block({args[0].value});"]
        if name == "z80_cycles":
            return [f"z80_cycles({args[0].value});"]
        if name == "interrupts_off":
            return ["interrupts_off();"]
        if name == "interrupts_on":
            # EI as a block's last instruction: the interrupt waits for the block after the next start
            return ["interrupts_on_after_block();" if args[0].value else "interrupts_on();"]
        if name == "port_out":
            return [f"port_out({self.port(args[0])}, {self.exact(args[1])});"]
        if name == "move_block":
            source, destination, count, upward = (self.exact(arg) for arg in args)
            return [f"move_block({source}, {destination}, {count}, {upward});"]
        raise emit.EmitError(f"{self.function.name}: effect {name}")


class SoundEmitter(emit.Emitter):
    """emit.Emitter with nothing of the V60 program's: no context registers; the sound program's
    structures (pointers.Layouts) and ports."""

    def __init__(self, functions, signatures, names, memory_, always, layouts, ports):
        super().__init__(functions, signatures, names, (), set(), memory_, always, lambda address: 0)
        self.layouts = layouts
        self.ports = dict(ports)
        self.saved_locals = True
        self.reroll = True    # copy_sample_page's 256 LDI in a row
        self.flow = None

    def restores_current(self, entry):
        return None, None

    def port_declarations(self):
        return [f"#define {name} 0x{port:02X}u" for port, name in sorted(self.ports.items())]
