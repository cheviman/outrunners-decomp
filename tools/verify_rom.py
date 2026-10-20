"""
Name:        verify_rom.py
Description: checks that the user-provided ROM is the complete 'orunners' set
Version:     0.1.0
Created:     11-10-2026
Modified:    19-10-2026
Author:      Sebastian Toyos

Usage: python tools/verify_rom.py [folder or .zip]   (default: rom/; a folder that holds the set's .zip
       is read through the .zip)
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import orunners_rom  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent


def main():
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "rom"
    if not source.exists():
        raise SystemExit(f"{source} does not exist")
    found, report = orunners_rom.identify_set(source)
    if not found and source.is_dir():
        for archive in sorted(source.glob("*.zip")):
            found, report = orunners_rom.identify_set(archive)
            if found:
                break
    print("\n".join(report))
    total = len(orunners_rom.ROM_FILES)
    print(f"\n{len(found)}/{total} files of the '{orunners_rom.SET_NAME}' set")
    return 0 if len(found) == total else 1


if __name__ == "__main__":
    sys.exit(main())
