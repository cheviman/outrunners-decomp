"""
Name:        select_scenarios.py
Description: runs the shared core's select_scenarios.py (tools/core) for this game: which scenarios a change
             has to run again, by what each one executes and reads
Version:     0.1.0
Created:     11-11-2026
Modified:    11-11-2026
Author:      Sebastian Toyos

The script is the core's (arcade-decomp, which tools/core holds); tools/game_profile.py tells it about this game.
Its usage is the core's.
"""
import runpy
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
runpy.run_path(str(TOOLS / "core" / "select_scenarios.py"), run_name="__main__")
