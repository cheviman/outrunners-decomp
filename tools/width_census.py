"""
Name:        width_census.py
Description: runs the shared core's width_census.py (tools/core) for this game: the places the original program
             reaches at more than one width, and its accesses out of line with their width, over the scenarios
Version:     0.1.0
Created:     05-11-2026
Modified:    05-11-2026
Author:      Sebastian Toyos

The script is the core's (arcade-decomp, which tools/core holds); tools/game_profile.py tells it about this game.
Its usage is the core's.
"""
import runpy
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
runpy.run_path(str(TOOLS / "core" / "width_census.py"), run_name="__main__")
