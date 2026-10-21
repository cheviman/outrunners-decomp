"""
Name:        signatures.py
Description: the C signature of every function: its parameters and the results it hands back
Version:     0.1.0
Created:     11-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

The game passes values in registers, as hand-written assembly does. In C:
  - Context registers the whole program shares (CONTEXT) are global variables. In OutRunners: R25, the
    block of the screen being run (0x208000 or 0x218000), which the interrupt handlers read too; R23, a
    place in it (1000[R25]); R20, the object the object loop (011CC0) runs, and R22, the loop's place in
    the list; R26, R27 and R28, which the loop does not set between handlers, so a handler may find what
    the one before left there. R12 is one too, although the game uses it as scratch: an object handler,
    009590, sets only the low halfword of R12 ("mov.h R21, R12") and reads through it, so the high one is
    the work RAM page some code before it left there (the screen's, 0x0020 or 0x0021); only that halfword
    outlives the function that sets it (SCRATCH_CONTEXT), unless the callers read more, and its assignments
    fold into their uses like a local's. R14 likewise: 016D00 sets only its low halfword and stores all of
    it in the object (7C), and other handlers read it whole as they start. R24 never changes after boot (the base of the game's variables)
    and disappears. Each of the game's two tasks has its own copy of them, as each has its own registers:
    the runtime swaps them with the task.
  - Any other register a function reads before writing is a parameter when some caller sets it before
    the call. When none does, the value would be whatever an earlier piece of code left there, which only
    happens on paths the game never takes: the function starts the register at 0 instead.
  - The registers and flags some caller reads after the call, out of those the function may write, are
    its results: each one a pointer parameter the function writes through (and reads, when it may leave
    it unchanged).
A function nothing in the program reaches (no call, jump, table or stored pointer: OPEN) has no caller to
say what it takes or hands back: every register it reads before writing is a parameter, and its results
are the registers symbols.txt says it hands back ("result" lines: dataflow.Program.declared), so that
the C keeps the work that makes them.
A call through a memory slot (an object's handler, an interrupt hook) passes nothing and takes nothing
back: the code that makes it runs whatever is stored there. A function such a slot can hold gets its
parameters from its direct callers only, and the slot call reaches it with zeros and ignores its
results. The entries of a table keep their own signatures: the C calls them from a switch on the
index, one call each, and an entry that does not write a register leaves the caller's variable as it
was, which is what the original does. A tail call needs no more: what the callee hands back that the
caller's own callers want is part of the caller's results already (dataflow.outputs), and the rest goes
to local variables of the caller.
"""
from v60 import ir

from . import dataflow

CONTEXT = (12, 14, 20, 22, 23, 25, 26, 27, 28, 32)
# what a context register holds when the program starts, where it is not 0 (the V70's SP after boot,
# 000072: functions whose stack stays in memory, stack.MEMORY_SP, push from there)
INITIAL = {32: 0x21F400}
# context registers the game uses as scratch: the bits code that runs later may read without setting them
SCRATCH_CONTEXT = {12: 0xFFFF0000, 14: 0xFFFFFFFF}
CONSTANT = {24: 0x20E000}
# the functions nothing reaches (decompile.py)
OPEN = set()


def register_of(variable):
    return variable[1] if variable[0] == "r" else None


def scratch(variables):
    """The C values in a set of data flow variables: registers (any part) and flags, without the context ones."""
    found = set()
    for variable in variables:
        if variable[0] == "r":
            if variable[1] not in CONTEXT and variable[1] not in CONSTANT:
                found.add(("r", variable[1]))
        elif variable[0] == "f":
            found.add(variable)
    return found


class Signature:
    def __init__(self):
        self.params = set()       # ("r", n) or ("f", name)
        self.results = set()
        self.zeroed = set()       # inputs no caller sets: start at 0

    def ordered(self, values):
        return sorted(values, key=lambda value: (value[0] != "r", value[1] if value[0] == "r" else value[1]))


def written_before_calls(flow, entry):
    """For each call of a function: the variables possibly written between the function's start and the call."""
    function = flow.functions[entry]
    blocks = function.blocks
    written_in = {label: set() for label in blocks}
    found = {}
    changed = True
    while changed:
        changed = False
        for label, block in blocks.items():
            written = set(written_in[label])
            for statement in block.statements:
                if isinstance(statement, ir.Call):
                    previous = found.get(id(statement), (statement, set()))[1]
                    found[id(statement)] = (statement, previous | written)
                may, _ = flow.transfer(statement, set(), {})
                written |= may
            if isinstance(block.end, ir.TailCall):
                call = ir.Call(target=block.end.target)
                key = ("tail", label)
                previous = found.get(key, (call, set()))[1]
                found[key] = (call, previous | written)
            for successor in block.successors():
                if not written <= written_in[successor]:
                    written_in[successor] |= written
                    changed = True
    return list(found.values())


def provided_inputs(flow, written):
    """Parameters of every function: the inputs some caller sets before calling it, or holds as a
    parameter of its own (a value handed down through a function that does not touch it)."""
    inputs = {entry: scratch(summary.reads) for entry, summary in flow.summaries.items()}
    params = {entry: set() for entry in flow.functions}
    changed = True
    while changed:
        changed = False
        provided = {entry: set() for entry in flow.functions}
        for entry, calls in written.items():
            for call, before in calls:
                if call.slot is not None:
                    continue    # a slot call passes nothing
                available = scratch(before) | params[entry]
                for target in flow.call_targets(call):
                    if target in provided:
                        provided[target] |= available
        for entry in flow.functions:
            new = inputs[entry] if entry in OPEN else inputs[entry] & provided[entry]
            if new != params[entry]:
                params[entry] = new
                changed = True
    return inputs, params


def compute(flow):
    """Signatures of every function. Re-solves 'flow' with the inputs that start at 0 left out, until
    those settle."""
    while True:
        written = {entry: written_before_calls(flow, entry) for entry in flow.functions}
        inputs, params = provided_inputs(flow, written)
        zeroed = {entry: inputs[entry] - params[entry] for entry in flow.functions}
        if all(zeroed[entry] <= flow.zeroed.get(entry, set()) for entry in flow.functions):
            break
        for entry in flow.functions:
            flow.zeroed[entry] = flow.zeroed.get(entry, set()) | zeroed[entry]
        flow.solve()
    outputs = flow.outputs()

    signatures = {}
    for entry, summary in flow.summaries.items():
        signature = Signature()
        signature.params = set(params[entry])
        signature.zeroed = set(flow.zeroed.get(entry, set()))
        signature.results = scratch(outputs[entry] & summary.writes)
        signatures[entry] = signature
    return signatures
