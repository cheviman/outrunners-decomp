"""
Name:        make_routes.py
Description: writes control scripts that race a one-player game to the goal by a given route, turning the
             wheel before each fork the route takes off the way the wheel in the centre goes
Version:     0.1.0
Created:     20-10-2026
Modified:    20-10-2026
Author:      Sebastian Toyos

Usage: python tools/make_routes.py NAME CHOICES [NAME CHOICES ...]
  NAME     the script to write, tests/controls/NAME.txt
  CHOICES  five letters, the way taken at each fork: R (right), C (centre) or L (left)

The race is the one of tests/controls/race_to_goal.txt: a coin, start, the selections, the gas down and the
race time held by its poke. The map is a lattice: the race starts at node 0, stage s has 2s nodes, from
s * s - s + 1 on, and a fork goes from a stage's node i to the next stage's i (right), i + 1 (centre) or
i + 2 (left); the first one, to stage 1's node 0 or 1. The program keeps the node it is on at 20FB62 and
the stage, from 1 at the start to 6 at the goal, at 20FB60.

Each fork is found by running the script on build/bin/s32ref and watching 20FB62. Where the wheel in the
centre does not take the way wanted, the wheel is turned for a while before that fork, longer or harder
until it does, and the next fork is looked for from there. The script ends some seconds after the goal,
when the ending, the map and the rankings have gone by.
"""
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONTROLS = ROOT / "tests" / "controls"
NODE = 0x20FB62
STAGE = 0x20FB60
GOAL_STAGE = 6
STEER_FROM = 2450               # the race is on by then
AFTER_GOAL = 5000               # frames from the goal to the end of the script: ending, map, rankings
POSITIONS = {"R": (184, 216), "C": (128, 112, 144, 96, 160, 80, 176), "L": (72, 40)}
WINDOWS = (600, 1200, 2400)     # how long before a fork the wheel turns


def executable():
    path = ROOT / "build" / "bin" / "s32ref.exe"
    return path if path.exists() else ROOT / "build" / "bin" / "s32ref"


def expected_nodes(choices):
    """The node each fork leads to"""
    nodes = []
    index = 0
    for stage, choice in enumerate(choices, start=1):
        if stage == 1:
            index = 0 if choice == "R" else 1
        else:
            index += "RCL".index(choice)
        nodes.append(stage * stage - stage + 1 + index)
    return nodes


def script(windows, poke_until, end, description):
    lines = [f"# {line}" for line in description]
    lines += ["600 press coin1", "660 press start1"]
    lines += [f"{frame} press start1" for frame in range(760, 2401, 120)]
    lines += ["900 gas 1 255"]
    for start, stop, position in windows:
        lines += [f"{start} wheel 1 {position}", f"{stop} wheel 1 128"]
    lines += [f"{frame} poke 20FB6B 0A" for frame in range(2500, poke_until + 1, 60)]
    lines += [f"{end} end"]
    return "\n".join(lines) + "\n"


def watch(windows, frames):
    """(frame, address, value) of every change of the node and the stage over a run of 'frames'"""
    with tempfile.TemporaryDirectory() as folder:
        path = Path(folder) / "route.txt"
        path.write_text(script(windows, frames, frames, []), encoding="ascii")
        result = subprocess.run([str(executable()), "--script", str(path), "--frames", str(frames),
                                 "--watch", f"{NODE:06X}", "--watch", f"{STAGE:06X}"],
                                capture_output=True, text=True)
    changes = []
    for line in result.stdout.splitlines():
        found = re.match(r"frame (\d+): ([0-9A-F]{6}) = ([0-9A-F]{2})", line)
        if found:
            changes.append((int(found.group(1)), int(found.group(2), 16), int(found.group(3), 16)))
    return changes


def forks(changes):
    """The node the race goes on to at each fork, with its frame"""
    return [(frame, value) for frame, address, value in changes
            if address == NODE and frame > STEER_FROM and value != 0]


def goal(changes):
    return next((frame for frame, address, value in changes if address == STAGE and value == GOAL_STAGE), None)


def make_route(name, choices):
    wanted = expected_nodes(choices)
    windows = []
    last = STEER_FROM
    found_frames = []
    for number, node in enumerate(wanted, start=1):
        taken = forks(watch(windows, last + 12000))
        if len(taken) < number:
            raise SystemExit(f"{name}: fork {number} does not come within 12000 frames of the one before")
        frame, value = taken[number - 1]
        if value != node:
            choice = choices[number - 1]
            for length in WINDOWS:
                tried = [(max(frame - length, last + 60, STEER_FROM), frame + 30, position)
                         for position in POSITIONS[choice]]
                for window in tried:
                    taken = forks(watch(windows + [window], frame + 3000))
                    if len(taken) >= number and taken[number - 1][1] == node:
                        windows.append(window)
                        frame = taken[number - 1][0]
                        break
                else:
                    continue
                break
            else:
                raise SystemExit(f"{name}: fork {number} goes to node {value}, not {node}, whatever the wheel does")
        found_frames.append(frame)
        last = frame
        print(f"  {name}: fork {number} at frame {frame} to node {node}", flush=True)
    finish = goal(watch(windows, last + 15000))
    if finish is None:
        raise SystemExit(f"{name}: the goal does not come within 15000 frames of the last fork")
    description = [
        f"A one-player game run to the goal by the route {choices} (R right, C centre, L left at each fork),",
        f"through the nodes {', '.join(str(node) for node in wanted)} of the map, the race time held as in",
        "race_to_goal.txt; the wheel turns before the forks where the centre would take another way.",
        "Written by tools/make_routes.py",
    ]
    (CONTROLS / f"{name}.txt").write_text(script(windows, finish, finish + AFTER_GOAL, description), encoding="ascii")
    print(f"{name}: goal at frame {finish}, written {CONTROLS / (name + '.txt')}", flush=True)


def main():
    arguments = sys.argv[1:]
    if not arguments or len(arguments) % 2:
        raise SystemExit(__doc__.split("Usage:")[1].split("The race")[0])
    for name, choices in zip(arguments[0::2], arguments[1::2]):
        if not re.fullmatch(r"[RCL]{5}", choices):
            raise SystemExit(f"{choices}: five letters, R, C or L")
        make_route(name, choices)
    return 0


if __name__ == "__main__":
    sys.exit(main())
