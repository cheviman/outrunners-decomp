"""
Name:        noise_report.py
Description: counts what is left of the CPU and of the decompiler in the C (src/, src/sound/) and compares it
             with the counts last saved
Version:     0.1.0
Created:     17-10-2026
Modified:    17-10-2026
Author:      Sebastian Toyos

Every kind of leftover the C can still have, counted in one pass over the generated C, so a round of work
shows what it took away and nothing comes back unseen: gotos, functions and locals named after an address
or a register, fields without a name, memory reached through computed addresses, results copied back at
every return, jumps out of a table, ROM addresses printed as numbers, functions without a comment. The
counts are compared with tools/noise_baseline.txt; a count that went up makes the run fail.

Usage: python tools/noise_report.py [--save]
  --save   write the counts as the new baseline (after a round that took something away)
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BASELINE = Path(__file__).resolve().parent / "noise_baseline.txt"
UNNAMED_ADDRESSES = ROOT / "build" / "decompile" / "unnamed_addresses.txt"

FUNCTION = re.compile(r"^(?:static )?\w[\w ]*?\b(\w+)\((.*)\)$")
# kind -> (what it is, pattern counted in the code lines; None: counted below)
KINDS = {
    "goto": ("goto statements", re.compile(r"\bgoto\b")),
    "control_variables": ("functions with bool control variables", None),
    "sub_functions": ("functions named after their address (sub_XXXXXX)", None),
    "unk_names": ("memory named after its address (unk_XXXXXX)", re.compile(r"\bunk_[0-9A-F]{4,6}\b")),
    "register_names": ("parameters and locals named after a register (r1, p_r4, r0_2)", None),
    "var_fields": ("object fields without a name (var_XX)", re.compile(r"\bvar_[0-9A-F]{2}\b")),
    "mem_access": ("memory reached through a computed address (mem_read/mem_write)",
                   re.compile(r"\bmem_(?:read|write)\d+\(")),
    "wide_access": ("several fields read or written at once (AS/AT)", re.compile(r"\bA[ST]\d+\(")),
    "copy_backs": ("results copied back before a return (*p_x = x;)", re.compile(r"^\t+\*p_\w+ = [^;]*;$")),
    "invalid_jump": ("jumps through a table with an index it does not have", re.compile(r"\binvalid_jump\(")),
    "unknown_code": ("calls to code the analysis does not know", re.compile(r"\bunknown_code\(")),
    "raw_rom_addresses": ("ROM addresses printed as numbers (build/decompile/unnamed_addresses.txt)", None),
    "uncommented": ("functions without a comment above them", None),
}
INFORMATION = {
    "functions": "functions",
    "lines": "lines of C",
    "casts": "casts",
    "partial_writes": "partial writes of a variable ((x & ~0xFFFFu) | ...)",
}


def count(paths):
    counts = {kind: 0 for kind in list(KINDS) + list(INFORMATION)}
    for path in paths:
        lines = path.read_text(encoding="ascii", errors="replace").splitlines()
        counts["lines"] += len(lines)
        in_function = False
        for index, line in enumerate(lines):
            if line.startswith(("/*", "   ", " *")):
                continue    # (comments)
            found = FUNCTION.match(line)
            if found and index + 1 < len(lines) and lines[index + 1] == "{":
                in_function = True
                counts["functions"] += 1
                if not lines[index - 1].endswith("*/"):
                    counts["uncommented"] += 1
                if re.fullmatch(r"sub_[0-9A-F]{6}", found.group(1)):
                    counts["sub_functions"] += 1
                counts["register_names"] += len(re.findall(r"(?<!\w)(?:p_)?r\d+(?:_\d+)?(?=[,)])", found.group(2)))
                continue
            if not in_function:
                continue
            for kind, (_, pattern) in KINDS.items():
                if pattern is not None:
                    counts[kind] += len(pattern.findall(line))
            if re.match(r"^\t(?:uint\d+_t|int\d+_t) ", line):
                counts["register_names"] += len(re.findall(r"\br\d+(?:_\d+)? = ", line))
            if line.startswith("\tbool "):
                counts["control_variables"] += 1
            counts["casts"] += len(re.findall(r"\((?:u?int\d+_t)\)", line))
            counts["partial_writes"] += len(re.findall(r"& ~0xF+u\) \|", line))
    return counts


def main():
    sources = sorted(path for path in (ROOT / "src").rglob("*.c"))
    counts = count(sources)
    if UNNAMED_ADDRESSES.exists():
        counts["raw_rom_addresses"] = sum(1 for line in UNNAMED_ADDRESSES.read_text().splitlines()
                                          if line and not line.startswith("#"))
    saved = {}
    if BASELINE.exists():
        for line in BASELINE.read_text().splitlines():
            if line and not line.startswith("#"):
                kind, value = line.split()[:2]
                saved[kind] = int(value)
    rose = []
    print(f"{'':20s} {'now':>7s} {'saved':>7s}")
    for kind, (what, _) in list(KINDS.items()) + [(kind, (what, None)) for kind, what in INFORMATION.items()]:
        before = saved.get(kind)
        change = "" if before is None or before == counts[kind] else f"{counts[kind] - before:+d}"
        print(f"{kind:20s} {counts[kind]:7d} {'' if before is None else before:>7} {change:>6s}  {what}")
        if kind in KINDS and before is not None and counts[kind] > before:
            rose.append(kind)
    if "--save" in sys.argv[1:]:
        BASELINE.write_text("# KIND COUNT: tools/noise_report.py --save writes it\n" +
                            "".join(f"{kind} {value}\n" for kind, value in counts.items()), encoding="ascii")
        print(f"saved in {BASELINE.relative_to(ROOT)}")
    if rose:
        print("went up: " + ", ".join(rose))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
