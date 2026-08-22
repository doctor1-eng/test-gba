#!/usr/bin/env python3
"""
Validate the Gen3 charmap hypothesis against the actual ROM bytes, and
give a rough map of where readable text lives. Read-only.

Strategy: scan the whole ROM; at every byte offset, try to decode a
Gen3 string. Keep it only if it terminates cleanly (0xFF) within a
sane length, is mostly letters/space/punctuation, and has at least a
few letters. Report a sample of decoded strings and a histogram of
hits per 0x10000-byte region, so we can see where the text banks are.
"""
import sys
import string
sys.path.insert(0, "tools")
from gen3_charmap import decode_bytes

PRINTABLE = set(string.ascii_letters + string.digits + " .,!?'-:;")


def is_plausible(text):
    if len(text) < 4:
        return False
    letters = sum(1 for c in text if c.isalpha())
    if letters < 3:
        return False
    ratio_letters = letters / max(1, len(text))
    return ratio_letters > 0.5


def scan(path, limit_samples=40):
    with open(path, "rb") as f:
        data = f.read()
    hits = []
    region_hist = {}
    i = 0
    n = len(data)
    while i < n:
        b = data[i]
        # only attempt a decode where the first byte is plausibly a letter/space
        if b == 0x00 or (0xA1 <= b <= 0xF7):
            text, length, ok = decode_bytes(data, i, max_len=400)
            if ok and is_plausible(text):
                hits.append((i, text))
                region = i // 0x10000
                region_hist[region] = region_hist.get(region, 0) + 1
                i += length
                continue
        i += 1
    return hits, region_hist


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "roms/original/odyssey_en_v4.1.1.gba"
    hits, hist = scan(path)
    print(f"total plausible decoded strings: {len(hits)}")
    print("sample:")
    for off, text in hits[:60]:
        print(f"  0x{off:07X}: {text!r}")
    print()
    print("top regions (0x10000 bank -> hit count):")
    for region, count in sorted(hist.items(), key=lambda kv: -kv[1])[:30]:
        print(f"  0x{region*0x10000:07X}-0x{region*0x10000+0xFFFF:07X}: {count}")
