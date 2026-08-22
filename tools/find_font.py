#!/usr/bin/env python3
"""
Locate the GBA font tile graphics by scanning for LZ77-compressed
blocks (the standard BIOS LZ77UnCompVram signature: byte 0x10, then a
3-byte little-endian decompressed size) whose decompressed size is a
plausible font-sheet size (multiple of 32 bytes = one 4bpp 8x8 tile;
Gen3 glyphs are two stacked 8x8 tiles = 64 bytes each). Read-only.
"""
import sys


def lz77_decompress(data, start, max_out=1 << 20):
    if data[start] != 0x10:
        return None
    size = data[start+1] | (data[start+2] << 8) | (data[start+3] << 16)
    if size == 0 or size > max_out:
        return None
    out = bytearray()
    pos = start + 4
    n = len(data)
    try:
        while len(out) < size:
            if pos >= n:
                return None
            flags = data[pos]
            pos += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if pos >= n:
                    return None
                if flags & (0x80 >> bit):
                    if pos + 1 >= n:
                        return None
                    b1 = data[pos]
                    b2 = data[pos+1]
                    pos += 2
                    length = (b1 >> 4) + 3
                    disp = ((b1 & 0x0F) << 8) | b2
                    disp += 1
                    if disp > len(out):
                        return None
                    for _ in range(length):
                        out.append(out[-disp])
                else:
                    out.append(data[pos])
                    pos += 1
    except IndexError:
        return None
    return bytes(out), pos - start


def scan(path, min_tiles=150, max_tiles=600):
    with open(path, "rb") as f:
        data = f.read()
    n = len(data)
    candidates = []
    i = 0
    while i < n - 4:
        if data[i] == 0x10:
            size = data[i+1] | (data[i+2] << 8) | (data[i+3] << 16)
            n_tiles64 = size / 64.0
            if size > 0 and size % 32 == 0 and min_tiles <= n_tiles64 <= max_tiles:
                result = lz77_decompress(data, i)
                if result:
                    decompressed, consumed = result
                    if len(decompressed) == size:
                        candidates.append((i, size, size // 64))
        i += 1
    return candidates


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "roms/original/odyssey_en_v4.1.1.gba"
    cands = scan(path)
    print(f"{len(cands)} candidate font-sheet blocks found")
    for off, size, ntiles in cands[:60]:
        print(f"  0x{off:07X}: decompressed {size} bytes (~{ntiles} 8x16 glyph tiles)")
