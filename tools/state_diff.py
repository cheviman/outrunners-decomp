"""
Name:        state_diff.py
Description: runs the shared core's state_diff.py (tools/core) for this game: the board's state of a run on
             the PC and of one on a big-endian processor, compared as the values each one holds
Version:     0.1.0
Created:     06-11-2026
Modified:    06-11-2026
Author:      Sebastian Toyos

The script is the core's (arcade-decomp, which tools/core holds); tools/game_profile.py tells it about this game.
Its usage is the core's.
"""
import runpy
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
runpy.run_path(str(TOOLS / "core" / "state_diff.py"), run_name="__main__")
