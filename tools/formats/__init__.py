"""
Name:        __init__.py
Description: the formats of the program data, one module a family (palettes, scripts, animations...)
Version:     0.1.0
Created:     14-10-2026
Modified:    14-10-2026
Author:      Sebastian Toyos

Each module here has FORMATS: {name: (file suffix, to_text(bytes) -> text, from_text(text) -> bytes)}, with
from_text(to_text(data)) == data for every piece data_formats.json gives that format. tools/data_formats.py
gathers them.
"""
