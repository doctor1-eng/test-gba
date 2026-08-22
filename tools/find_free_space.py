#!/usr/bin/env python3
"""Find large contiguous runs of a single repeated byte (0xFF or 0x00),
the two typical GBA "erased/unused" fill values, as candidate free
space for inserting relocated/expanded strings. Read-only."""
import sys


def find_runs(data, fill, min_len=256):
    runs = []
    n = len(data)
    i = 0
    while i < n:
        if data[i] == fill:
            j = i
            while j < n and data[j] == fill:
                j += 1
            if j - i >= min_len:
                runs.append((i, j - i))
            i = j
        else:
            i += 1
    return runs


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "roms/original/odyssey_en_v4.1.1.gba"
    with open(path, "rb") as f:
        data = f.read()
    for fill in (0xFF, 0x00):
        runs = find_runs(data, fill, min_len=1024)
        total = sum(l for _, l in runs)
        print(f"fill=0x{fill:02X}: {len(runs)} runs >=1024 bytes, total {total} bytes ({total/1024/1024:.2f} MiB)")
        for off, length in sorted(runs, key=lambda r: -r[1])[:15]:
            print(f"  0x{off:07X} - 0x{off+length:07X}  ({length} bytes)")
