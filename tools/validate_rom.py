#!/usr/bin/env python3
"""
Post-build integrity checks on a produced FR ROM against the original.
Read-only on both files.
"""
import sys
import hashlib

sys.path.insert(0, "tools")
from gba_header import read_header

ORIG_PATH = "roms/original/odyssey_en_v4.1.1.gba"
FR_PATH = "build/Pokemon_Odyssey_FR.gba"


def main():
    orig_header, orig_data = read_header(ORIG_PATH)
    fr_header, fr_data = read_header(FR_PATH)

    errors = []
    warnings = []

    if len(fr_data) != len(orig_data):
        errors.append(f"size changed: {len(orig_data)} -> {len(fr_data)}")

    for field in ("title", "game_code", "maker_code", "fixed_96", "main_unit_code", "device_type"):
        if orig_header[field] != fr_header[field]:
            errors.append(f"header field '{field}' changed: {orig_header[field]!r} -> {fr_header[field]!r}")

    if not fr_header["complement_check_valid"]:
        errors.append("complement checksum invalid in FR ROM")

    # every byte outside the diff should be identical; count diffs as a sanity signal
    n_diff_bytes = sum(1 for a, b in zip(orig_data, fr_data) if a != b)
    print(f"bytes changed vs original: {n_diff_bytes} / {len(orig_data)} ({100*n_diff_bytes/len(orig_data):.3f}%)")

    if errors:
        print(f"\n{len(errors)} ERROR(S):")
        for e in errors:
            print(f"  - {e}")
    else:
        print("\nNo structural errors found (size, header, checksum all OK).")

    print(f"\nFR ROM sha256: {hashlib.sha256(fr_data).hexdigest()}")
    sys.exit(1 if errors else 0)


if __name__ == "__main__":
    main()
