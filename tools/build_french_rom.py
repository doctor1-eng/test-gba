#!/usr/bin/env python3
"""
Reinsertion tool. Applies translated & control-code-validated rows
from translation/text_database.tsv onto a COPY of the original ROM.

Never touches roms/original/*.gba. Strategy per docs/TECHNICAL_AUDIT.md
section 8:
  - French encodes to <= original byte length -> write in place at the
    same offset (terminator just lands earlier; pointer(s) unchanged).
  - French encodes longer -> allocate from free space (large 0xFF
    runs found by tools/find_free_space.py, allocated with a simple
    bump allocator, largest runs first, 4-byte aligned), write there,
    and rewrite every referencing pointer (row's "references" column)
    to the new address. Logged to build/patch_log.tsv.

Only rows with status in TRANSLATED/REVIEW/VALIDATED are inserted
(anything still UNTRANSLATED or SKIP_NOISE is left as English, on
purpose -- a half-translated build is still a truthful build).
"""
import csv
import hashlib
import sys

sys.path.insert(0, "tools")
from gen3_charmap import encode_string
from find_free_space import find_runs

ROM_PATH = "roms/original/odyssey_en_v4.1.1.gba"
DB_PATH = "translation/text_database.tsv"
OUT_ROM_PATH = "build/Pokemon_Odyssey_FR.gba"
OUT_SHA_PATH = "build/Pokemon_Odyssey_FR.sha256"
LOG_PATH = "build/patch_log.tsv"

INSERTABLE_STATUSES = {"TRANSLATED", "REVIEW", "VALIDATED", "INSERTED", "TESTED"}


class FreeSpaceAllocator:
    def __init__(self, data, min_run=64, reserve_margin=16):
        runs = find_runs(data, 0xFF, min_len=min_run)
        # largest first: minimizes fragmentation waste for big strings first
        self.runs = sorted(runs, key=lambda r: -r[1])
        self.cursor_idx = 0
        self.cursor_off = self.runs[0][0] if self.runs else None
        self.reserve_margin = reserve_margin

    def alloc(self, size):
        size_aligned = (size + 3) & ~3
        while self.cursor_idx < len(self.runs):
            run_off, run_len = self.runs[self.cursor_idx]
            used = self.cursor_off - run_off
            remaining = run_len - used
            if remaining >= size_aligned + self.reserve_margin:
                addr = self.cursor_off
                self.cursor_off += size_aligned
                return addr
            self.cursor_idx += 1
            if self.cursor_idx < len(self.runs):
                self.cursor_off = self.runs[self.cursor_idx][0]
        raise RuntimeError(f"out of free space allocating {size} bytes")


def main():
    with open(ROM_PATH, "rb") as f:
        original = f.read()
    rom = bytearray(original)

    rows = list(csv.DictReader(open(DB_PATH, encoding="utf-8"), delimiter="\t"))
    to_insert = [r for r in rows if r["status"] in INSERTABLE_STATUSES and r["french"].strip()]
    print(f"{len(to_insert)} / {len(rows)} rows queued for insertion")

    allocator = FreeSpaceAllocator(rom)
    log_rows = []
    n_inplace = 0
    n_relocated = 0
    n_skipped_errors = 0

    for row in to_insert:
        target = int(row["offset"], 16)
        orig_size = int(row["orig_size_bytes"])
        encoded, warnings = encode_string(row["french"])
        hard_errors = [w for w in warnings if w[0] == "unencodable"]
        if hard_errors:
            n_skipped_errors += 1
            print(f"SKIP {row['id']}: unencodable chars {hard_errors}")
            continue

        refs = [int(r, 16) for r in row["references"].split(",") if r]

        if len(encoded) <= orig_size:
            rom[target:target + len(encoded)] = encoded
            n_inplace += 1
            log_rows.append({
                "id": row["id"], "mode": "inplace",
                "old_offset": f"0x{target:07X}", "new_offset": f"0x{target:07X}",
                "old_size": orig_size, "new_size": len(encoded),
                "refs_updated": "",
            })
        else:
            new_addr = allocator.alloc(len(encoded))
            rom[new_addr:new_addr + len(encoded)] = encoded
            new_ptr = (0x08000000 + new_addr).to_bytes(4, "little")
            for ref_off in refs:
                rom[ref_off:ref_off + 4] = new_ptr
            n_relocated += 1
            log_rows.append({
                "id": row["id"], "mode": "relocated",
                "old_offset": f"0x{target:07X}", "new_offset": f"0x{new_addr:07X}",
                "old_size": orig_size, "new_size": len(encoded),
                "refs_updated": ",".join(f"0x{r:07X}" for r in refs),
            })

    assert len(rom) == len(original), "ROM size must not change"

    import os
    os.makedirs("build", exist_ok=True)
    with open(OUT_ROM_PATH, "wb") as f:
        f.write(rom)
    digest = hashlib.sha256(rom).hexdigest()
    with open(OUT_SHA_PATH, "w") as f:
        f.write(f"{digest}  {OUT_ROM_PATH.split('/')[-1]}\n")

    with open(LOG_PATH, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["id", "mode", "old_offset", "new_offset", "old_size", "new_size", "refs_updated"], delimiter="\t")
        w.writeheader()
        w.writerows(log_rows)

    print(f"in-place: {n_inplace}, relocated: {n_relocated}, skipped (unencodable): {n_skipped_errors}")
    print(f"wrote {OUT_ROM_PATH} sha256={digest}")


if __name__ == "__main__":
    main()
