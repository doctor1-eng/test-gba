#!/usr/bin/env python3
"""
Find text by validating the GBA pointer structure, not raw byte
guessing: scan every 4-byte-aligned word for a valid ROM pointer
(0x08000000 + offset, high byte 0x08 or 0x09 for a 32MB ROM), then
try to Gen3-decode the byte string it points to. A pointer whose
target decodes cleanly (proper 0xFF terminator, mostly letters/space,
plausible English-ish token) is strong joint evidence that both the
pointer table AND the charmap hypothesis are correct, because a
coincidental match on both structural constraints at once is unlikely.
Read-only.
"""
import sys
import string
sys.path.insert(0, "tools")
from gen3_charmap import decode_bytes

COMMON_WORDS = {
    "the", "you", "your", "is", "and", "a", "to", "of", "in", "it",
    "me", "my", "i", "not", "no", "yes", "what", "this", "that",
    "are", "have", "will", "can", "with", "for", "on", "at", "be",
    "pokemon", "trainer", "gym", "town", "route", "here", "there",
    "go", "get", "want", "need", "know", "see", "let", "come",
}


def is_plausible_dialogue(text):
    if not (3 <= len(text) <= 300):
        return False
    letters = sum(1 for c in text if c.isalpha())
    if letters < 3:
        return False
    non_printable_junk = sum(1 for c in text if c == '<')
    words = [w.strip(string.punctuation).lower() for w in text.replace('<', ' <').split()]
    common_hits = sum(1 for w in words if w in COMMON_WORDS)
    ratio_letters = letters / max(1, len(text))
    return ratio_letters > 0.55 and (common_hits > 0 or len(text) < 12)


def scan(path, rom_size_hint=0x02000000):
    with open(path, "rb") as f:
        data = f.read()
    n = len(data)
    high_bytes = set()
    b = 0x08000000 + n
    for hb in (0x08, 0x09, 0x0A):
        if hb <= (0x08000000 + n - 1) >> 24:
            high_bytes.add(hb)
    high_bytes = {0x08, 0x09} if n > 0x1000000 else {0x08}

    hits = []
    i = 0
    while i + 4 <= n:
        b3 = data[i + 3]
        if b3 in high_bytes:
            word = int.from_bytes(data[i:i+4], "little")
            target = word - 0x08000000
            if 0 <= target < n:
                text, length, ok = decode_bytes(data, target, max_len=300)
                if ok and is_plausible_dialogue(text):
                    hits.append((i, target, text))
        i += 4
    return hits


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "roms/original/odyssey_en_v4.1.1.gba"
    hits = scan(path)
    print(f"valid pointer->text hits: {len(hits)}")
    for ptr_off, target, text in hits[:80]:
        print(f"  ptr@0x{ptr_off:07X} -> 0x{target:07X}: {text!r}")
