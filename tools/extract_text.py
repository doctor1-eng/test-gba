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


def main():
    with open(ROM_PATH, "rb") as f:
        data = f.read()
    found = scan_pointers(data)
    print(f"{len(found)} unique text strings found via {sum(len(v['refs']) for v in found.values())} valid pointers")

    rows = []
    for idx, (target, entry) in enumerate(sorted(found.items()), start=1):
        codes, n_lines, n_chars = analyze(entry)
        text_id = f"ODYSSEY-TXT-{idx:06d}"
        refs = ",".join(f"0x{r:07X}" for r in sorted(entry["refs"]))
        rows.append({
            "id": text_id,
            "offset": f"0x{target:07X}",
            "orig_size_bytes": entry["length"],
            "english": entry["text"],
            "control_codes": ";".join(codes),
            "n_chars": n_chars,
            "n_lines": n_lines,
            "references": refs,
            "context": "UNKNOWN",
            "french": "",
            "status": "UNTRANSLATED",
        })

    with open(OUT_PATH, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=[
            "id", "offset", "orig_size_bytes", "english", "control_codes",
            "n_chars", "n_lines", "references", "context", "french", "status",
        ], delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)

    print(f"wrote {len(rows)} rows to {OUT_PATH}")


if __name__ == "__main__":
    main()
