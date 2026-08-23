#!/usr/bin/env python3
"""
Rebuilds a ROM by replaying a prefix of build/patch_log.tsv onto a fresh
copy of the original ROM, optionally excluding specific row IDs.

This exists for bisecting a corruption regression: apply the first N
patches (or all patches minus a suspect row) and re-test with
tools/headless_playtest.py, binary-searching for the exact patch
responsible rather than guessing from static analysis. This is how
ODYSSEY-TXT-004785's single implausible reference (0x0000D90, inside
the ROM's boot code) was pinned down as the title-screen corruption
source on 2026-08-23 -- see docs/PROGRESS.md.

Usage:
    python3 tools/replay_patch_log.py <n> <out.gba> [exclude_id ...]
"""
import csv
import sys

ORIG = "roms/original/odyssey_en_v4.1.1.gba"
FR = "build/Pokemon_Odyssey_FR.gba"
LOG = "build/patch_log.tsv"


def load():
    with open(ORIG, "rb") as f:
        orig = f.read()
    with open(FR, "rb") as f:
        fr = f.read()
    rows = list(csv.DictReader(open(LOG, encoding="utf-8"), delimiter="\t"))
    return orig, fr, rows


def build_partial(n, out_path, exclude_ids=frozenset(), orig=None, fr=None, rows=None):
    if orig is None:
        orig, fr, rows = load()
    rom = bytearray(orig)
    for row in rows[:n]:
        if row["id"] in exclude_ids:
            continue
        old_off = int(row["old_offset"], 16)
        new_size = int(row["new_size"]) if row["new_size"] else None
        if row["mode"] == "inplace":
            rom[old_off:old_off + new_size] = fr[old_off:old_off + new_size]
        elif row["mode"] == "relocated":
            new_off = int(row["new_offset"], 16)
            rom[new_off:new_off + new_size] = fr[new_off:new_off + new_size]
            new_ptr = (0x08000000 + new_off).to_bytes(4, "little")
            for ref in row["refs_updated"].split(","):
                if not ref:
                    continue
                ref_off = int(ref, 16)
                rom[ref_off:ref_off + 4] = new_ptr
        elif row["mode"] == "skipped_untrusted_refs":
            pass
        else:
            raise ValueError(row["mode"])
    with open(out_path, "wb") as f:
        f.write(bytes(rom))


if __name__ == "__main__":
    n = int(sys.argv[1])
    out = sys.argv[2]
    exclude = frozenset(sys.argv[3:])
    orig, fr, rows = load()
    print(f"{len(rows)} patch rows total")
    build_partial(n, out, exclude_ids=exclude, orig=orig, fr=fr, rows=rows)
    print(f"wrote {out} with first {n} patches applied" + (f", excluding {sorted(exclude)}" if exclude else ""))
