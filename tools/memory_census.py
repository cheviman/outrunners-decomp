"""
Name:        memory_census.py
Description: what memory each mem_read and mem_write of the game's C reaches, over the scenarios
Version:     0.1.0
Created:     15-10-2026
Modified:    15-10-2026
Author:      Sebastian Toyos

Usage: python tools/memory_census.py run [--jobs N]    run every script of tests/controls on the headless game
                                                       built with SONIC_MEMORY_CENSUS (build/bin-census)
       python tools/memory_census.py report            sum the counts: build/census/memory.txt

A development aid for typing the C's pointers: the census says, for each line of src/ that reads or writes
memory through a pointer (mem_read, mem_write), which regions of the board it reaches in the scenarios
(the program ROM, the work RAM, video RAM, sprite RAM, palette RAM, the mixer, the RAM shared with the
sound board, the I/O chip...), how often, and the lowest and highest address. A line no scenario runs is
not in it. Build it with: cmake -S . -B build/cmake-census -DSONIC_MEMORY_CENSUS=ON
-DSEGASONIC_REFERENCE_TOOLS=OFF -DSONIC_BIN_DIR=build/bin-census, then build sonic_game_headless.
"""
import os
import subprocess
import sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GAME = ROOT / "build" / "bin-census" / ("sonic_game_headless.exe" if os.name == "nt" else "sonic_game_headless")
OUT = ROOT / "build" / "census"


def run_one(script):
    target = OUT / "scenarios" / f"{script.stem}.txt"
    if not GAME.exists():
        return f"{GAME}: missing"
    result = subprocess.run([str(GAME), "--script", str(script), "--memory-census", str(target)],
                            capture_output=True, text=True, timeout=1800)
    return None if result.returncode == 0 else f"{script.stem}: {result.stderr.strip()[:200]}"


def run(jobs):
    (OUT / "scenarios").mkdir(parents=True, exist_ok=True)
    scripts = sorted((ROOT / "tests" / "controls").glob("*.txt"))
    with ThreadPoolExecutor(max_workers=jobs) as pool:
        for problem in pool.map(run_one, scripts):
            if problem:
                print(problem)
    print(f"{len(scripts)} scenarios: {OUT / 'scenarios'}")


def report():
    totals = defaultdict(lambda: [0, 0xFFFFFFFF, 0, set()])
    for path in sorted((OUT / "scenarios").glob("*.txt")):
        for line in path.read_text(encoding="ascii").splitlines():
            if line.startswith("#"):
                continue
            where, bits, kind, count, span, *regions = line.split()
            low, high = (int(value, 16) for value in span.split("-"))
            key = (where.replace("\\", "/"), int(bits), kind)
            entry = totals[key]
            entry[0] += int(count)
            entry[1] = min(entry[1], low)
            entry[2] = max(entry[2], high)
            entry[3] |= set(regions)
    lines = ["# FILE:LINE BITS read|write COUNT LOW-HIGH REGIONS, summed over the scenarios"]
    for (where, bits, kind), (count, low, high, regions) in sorted(totals.items()):
        lines.append(f"{where} {bits} {kind} {count} {low:06X}-{high:06X} {' '.join(sorted(regions))}")
    (OUT / "memory.txt").write_text("\n".join(lines) + "\n", encoding="ascii")
    print(f"{len(totals)} lines of C: {OUT / 'memory.txt'}")


def main():
    arguments = sys.argv[1:]
    if arguments[:1] == ["run"]:
        jobs = int(arguments[arguments.index("--jobs") + 1]) if "--jobs" in arguments else 4
        run(jobs)
    elif arguments[:1] == ["report"]:
        report()
    else:
        print(__doc__.split("\n\n")[1])
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
