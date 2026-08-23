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


def trustworthy_refs(refs, strict=False, max_gap=0x4000, big_list_threshold=40, min_retained_frac=0.60):
    """Filter a row's raw 'references' list before it is used to blindly
    overwrite pointer bytes elsewhere in the ROM.

    Root cause of a real bug found via in-game testing: extract_text.py's
    scan_pointers() records ANY 4-byte-aligned word matching a discovered
    text's address as a "reference", with no check that the word is truly
    a pointer-table entry rather than a coincidental byte match inside
    unrelated binary data (graphics/audio/padding). This is usually
    harmless (median row has exactly 1 real reference), but a handful of
    text offsets happen to be "attractive nuisance" addresses -- e.g.
    physical offset 0x1090909 encodes as pointer word 09 09 09 09, a
    4-byte repdigit that recurs by pure coincidence throughout large
    low-entropy binary regions (confirmed by inspecting the referrer
    positions in the original ROM: most have neighbouring words with
    invalid/out-of-range high bytes, and the match count peaks exactly
    at the most repetitive byte pattern in a family of 1-byte-shifted
    overlapping string fragments -- see docs/PROGRESS.md).

    Spatial clustering (always applied): a genuine pointer table
    referencing a shared string lives in one bounded region of the ROM.
    Refs are grouped into clusters (gap <= max_gap between consecutive
    sorted refs) and only the single largest cluster is kept -- this
    discards refs scattered many MB away as coincidental noise.

    Two trust modes for what happens after clustering:

    - Normal (strict=False): trust the largest cluster unless the raw
      list was already large (> big_list_threshold) and clustering only
      recovered a small minority of it (< min_retained_frac) -- e.g. a
      list of 178 that clusters down to 25 is still untrustworthy even
      at 25. This is permissive enough to keep large, internally
      consistent multi-reference patterns that are entirely normal in
      this ROM (e.g. ~230 ability-description rows each legitimately
      referenced from exactly 2 separate UI screens -- verified these
      are NOT part of any overlapping-fragment family, unlike the rows
      below).
    - Strict (strict=True): only trust a cluster that is unanimous (every
      raw ref agrees) or an overwhelming, clearly-tabular majority
      (cluster size >= 20 AND >= 90% retained). A found-in-the-wild
      example of why: two refs over 1MB apart is a coin flip for the
      clustering heuristic above, and for a normal standalone string
      that coin flip is low-stakes (the vast majority of such cases,
      like the ability descriptions, are genuine dual references) -- but
      for a row physically overlapping a neighbour (build_french_rom's
      overlapping_ids, forced to relocate regardless of length), the
      SAME "attractive nuisance address" mechanism documented above is
      already known to be in play, so an ambiguous cluster there is far
      more likely to be coincidence than structure. Confirmed against
      the intro-narration overlap family (IDs 006145/146/149/150): each
      has 2-27 raw refs spread across MB-scale gaps with no dominant
      cluster, and relocating them was very likely still corrupting a
      handful of unrelated ROM locations even after the general fix
      above -- consistent with the in-game corruption the user reported
      persisting after that first fix. The caller should pass
      strict=True whenever the row is in overlapping_ids.

    Either mode returns None (untrusted -- caller should skip inserting
    this row rather than risk corrupting unrelated ROM data) when the
    trust bar isn't met.
    """
    if len(refs) <= 1:
        return refs
    srefs = sorted(refs)
    clusters = []
    cur = [srefs[0]]
    for r in srefs[1:]:
        if r - cur[-1] <= max_gap:
            cur.append(r)
        else:
            clusters.append(cur)
            cur = [r]
    clusters.append(cur)
    clusters.sort(key=len, reverse=True)
    biggest = clusters[0]
    if strict:
        if len(biggest) == len(refs):
            return biggest
        if len(biggest) >= 20 and len(biggest) / len(refs) >= 0.90:
            return biggest
        return None
    if len(refs) > big_list_threshold and len(biggest) / len(refs) < min_retained_frac:
        return None
    return biggest


def find_overlapping_ids(all_rows):
    """Some extracted strings share physical ROM bytes: a second pointer
    can reference a few bytes INTO another string's already-captured
    span (a real, intentional Gen3-hack space-saving trick -- the tail
    of one string doubles as a shorter standalone message elsewhere).
    Writing such rows independently in place would let whichever is
    processed last silently corrupt the other. Any row whose span
    [target, target+orig_size) overlaps another row's span must always
    be relocated (never written in place), so each ends up as its own
    independent copy with no shared bytes."""
    spans = sorted(
        ((int(r["offset"], 16), int(r["orig_size_bytes"]), r["id"]) for r in all_rows),
        key=lambda s: s[0],
    )
    overlapping = set()
    for i in range(len(spans) - 1):
        off_a, size_a, id_a = spans[i]
        off_b, size_b, id_b = spans[i + 1]
        if off_b < off_a + size_a:
            overlapping.add(id_a)
            overlapping.add(id_b)
    return overlapping


def main():
    with open(ROM_PATH, "rb") as f:
        original = f.read()
    rom = bytearray(original)

    rows = list(csv.DictReader(open(DB_PATH, encoding="utf-8"), delimiter="\t"))
    to_insert = [r for r in rows if r["status"] in INSERTABLE_STATUSES and r["french"].strip()]
    print(f"{len(to_insert)} / {len(rows)} rows queued for insertion")

    overlapping_ids = find_overlapping_ids(rows)
    if overlapping_ids:
        print(f"{len(overlapping_ids)} rows share ROM bytes with a neighbor -- forcing relocation for those")

    allocator = FreeSpaceAllocator(rom)
    log_rows = []
    n_inplace = 0
    n_relocated = 0
    n_skipped_errors = 0
    n_skipped_untrusted_refs = 0

    for row in to_insert:
        target = int(row["offset"], 16)
        orig_size = int(row["orig_size_bytes"])
        encoded, warnings = encode_string(row["french"])
        hard_errors = [w for w in warnings if w[0] == "unencodable"]
        if hard_errors:
            n_skipped_errors += 1
            print(f"SKIP {row['id']}: unencodable chars {hard_errors}")
            continue

        raw_refs = [int(r, 16) for r in row["references"].split(",") if r]
        must_relocate = row["id"] in overlapping_ids

        if len(encoded) <= orig_size and not must_relocate:
            rom[target:target + len(encoded)] = encoded
            n_inplace += 1
            log_rows.append({
                "id": row["id"], "mode": "inplace",
                "old_offset": f"0x{target:07X}", "new_offset": f"0x{target:07X}",
                "old_size": orig_size, "new_size": len(encoded),
                "refs_updated": "",
            })
        else:
            refs = trustworthy_refs(raw_refs, strict=must_relocate)
            if refs is None:
                # Relocation requires rewriting every referencing pointer,
                # but this target's reference list could not be trusted
                # (see trustworthy_refs docstring) -- rewriting it would
                # risk corrupting unrelated ROM data. Leaving the original
                # English bytes untouched is always safe.
                n_skipped_untrusted_refs += 1
                print(f"SKIP {row['id']}: {len(raw_refs)} raw references, could not be trusted for relocation -- left in English")
                log_rows.append({
                    "id": row["id"], "mode": "skipped_untrusted_refs",
                    "old_offset": f"0x{target:07X}", "new_offset": "",
                    "old_size": orig_size, "new_size": "",
                    "refs_updated": "",
                })
                continue
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
            if len(refs) < len(raw_refs):
                print(f"NOTE {row['id']}: {len(raw_refs)} raw references -> {len(refs)} trusted (clustering discarded {len(raw_refs)-len(refs)} likely-coincidental matches)")

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

    print(f"in-place: {n_inplace}, relocated: {n_relocated}, skipped (unencodable): {n_skipped_errors}, skipped (untrusted references): {n_skipped_untrusted_refs}")
    print(f"wrote {OUT_ROM_PATH} sha256={digest}")


if __name__ == "__main__":
    main()
