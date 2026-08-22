#!/usr/bin/env python3
"""
Exhaustive text extraction for Pokémon Odyssey (BPRE-based hack).

Method: scan every 4-byte-aligned word in the ROM for a structurally
valid GBA pointer (see docs/TECHNICAL_AUDIT.md section 2). For each
valid pointer, try to Gen3-decode the string at its target. Keep it if
it decodes cleanly (proper 0xFF terminator) and passes a plausibility
filter. Group by target offset (many pointers -- e.g. tables + script
references -- can point at the same string) and assign a stable ID.

Output: translation/text_database.tsv

This is a first exhaustive pass over the *pointer-referenced* text.
Known limitation (documented, not hidden): a small number of engine
text strings may be referenced through pointer arithmetic or indirect
tables this scan doesn't resolve; docs/PROGRESS.md tracks this.
"""
import re
import sys
import csv

sys.path.insert(0, "tools")
from gen3_charmap import decode_bytes, CONTROL_CODES, TERMINATOR

ROM_PATH = "roms/original/odyssey_en_v4.1.1.gba"
OUT_PATH = "translation/text_database.tsv"

CONTROL_TAG_RE = re.compile(r"<([0-9A-F]{2})(?::[0-9A-F]{2})?>")


def is_plausible(text, min_letters=3):
    if not (1 <= len(text) <= 500):
        return False
    letters = sum(1 for c in text if c.isalpha())
    if letters < min_letters and len(text.strip()) > 0:
        # allow short pure-symbol/number strings (e.g. "100%", "Lv.")
        if not any(c.isalnum() for c in text):
            return False
        if letters == 0 and len(text) > 6:
            return False
    ratio_letters_or_space = sum(1 for c in text if c.isalpha() or c in " '’-.,!?:;%&") / max(1, len(text))
    return ratio_letters_or_space > 0.6


def high_byte_set(rom_size):
    return {0x08, 0x09} if rom_size > 0x01000000 else {0x08}


def scan_pointers(data):
    n = len(data)
    hb = high_byte_set(n)
    found = {}  # target_offset -> {"text":..., "refs": set()}
    i = 0
    while i + 4 <= n:
        if data[i + 3] in hb:
            word = int.from_bytes(data[i:i + 4], "little")
            target = word - 0x08000000
            if 0 <= target < n and target not in found:
                text, length, ok = decode_bytes(data, target, max_len=500)
                if ok and is_plausible(text):
                    found[target] = {"text": text, "length": length, "refs": {i}}
                elif ok and target in found:
                    pass
            elif 0 <= target < n and target in found:
                found[target]["refs"].add(i)
        i += 4
    return found


def analyze(entry):
    text = entry["text"]
    codes = sorted(set(CONTROL_TAG_RE.findall(text)))
    n_lines = 1 + text.count("<FE>") + text.count("<01>")
    n_chars = len(CONTROL_TAG_RE.sub("", text))
    return codes, n_lines, n_chars


def load_existing():
    """Load the current TSV (if any) so re-extraction never renumbers or
    loses translation work already done: existing offset -> row is kept
    as-is (id, french, context, status untouched), only its english/
    control_codes/etc. get refreshed from a fresh decode. Brand-new
    offsets get the next free ID."""
    import os
    if not os.path.exists(OUT_PATH):
        return {}, 0
    rows = list(csv.DictReader(open(OUT_PATH, encoding="utf-8"), delimiter="\t"))
    by_offset = {r["offset"]: r for r in rows}
    max_idx = max((int(r["id"].rsplit("-", 1)[1]) for r in rows), default=0)
    return by_offset, max_idx


def main():
    with open(ROM_PATH, "rb") as f:
        data = f.read()
    found = scan_pointers(data)
    print(f"{len(found)} unique text strings found via {sum(len(v['refs']) for v in found.values())} valid pointers")

    existing_by_offset, max_idx = load_existing()
    next_idx = max_idx + 1

    rows = []
    n_new = 0
    n_refreshed = 0
    for target, entry in sorted(found.items()):
        offset_str = f"0x{target:07X}"
        codes, n_lines, n_chars = analyze(entry)
        refs = ",".join(f"0x{r:07X}" for r in sorted(entry["refs"]))

        prior = existing_by_offset.get(offset_str)
        if prior is not None:
            text_id = prior["id"]
            context = prior["context"]
            french = prior["french"]
            status = prior["status"]
            if prior["english"] != entry["text"]:
                n_refreshed += 1
        else:
            text_id = f"ODYSSEY-TXT-{next_idx:06d}"
            next_idx += 1
            context = "UNKNOWN"
            french = ""
            status = "UNTRANSLATED"
            n_new += 1

        rows.append({
            "id": text_id,
            "offset": offset_str,
            "orig_size_bytes": entry["length"],
            "english": entry["text"],
            "control_codes": ";".join(codes),
            "n_chars": n_chars,
            "n_lines": n_lines,
            "references": refs,
            "context": context,
            "french": french,
            "status": status,
        })

    rows.sort(key=lambda r: int(r["id"].rsplit("-", 1)[1]))

    with open(OUT_PATH, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=[
            "id", "offset", "orig_size_bytes", "english", "control_codes",
            "n_chars", "n_lines", "references", "context", "french", "status",
        ], delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)

    print(f"wrote {len(rows)} rows to {OUT_PATH} ({n_new} new, {n_refreshed} re-decoded differently, {len(rows)-n_new-n_refreshed} unchanged)")


if __name__ == "__main__":
    main()
