"""Palette I/O and 16-color quantization.

Supports the two palette formats you'll actually meet in a pokeemerald-family
project:

- JASC-PAL (plain text, used by porymap/Tilemap Studio and most tile editors)
- GBA native binary palette (16 x little-endian uint16, BGR555)

Also provides a dependency-free median-cut quantizer to reduce an arbitrary
imported image down to <=16 colors (GBA hardware palette size per tileset
palette slot).
"""
from __future__ import annotations

import struct
from typing import List, Sequence, Tuple

RGB = Tuple[int, int, int]

GBA_MAX_COMPONENT = 31  # 5 bits per channel


def _round5(x: int) -> int:
    return round(x * GBA_MAX_COMPONENT / 255)


def _expand5(x: int) -> int:
    return round(x * 255 / GBA_MAX_COMPONENT)


def snap_to_gba_15bit(color: RGB) -> RGB:
    """Round an 8-bit RGB color to what it would actually look like on GBA
    hardware (5 bits per channel), so previews match in-game colors."""
    r, g, b = color
    return (_expand5(_round5(r)), _expand5(_round5(g)), _expand5(_round5(b)))


def read_jasc_pal(path: str) -> List[RGB]:
    with open(path, "r", encoding="utf-8") as f:
        lines = [l.strip() for l in f if l.strip()]
    if not lines or lines[0] != "JASC-PAL":
        raise ValueError(f"{path}: not a JASC-PAL file")
    count = int(lines[2])
    colors = []
    for line in lines[3 : 3 + count]:
        r, g, b = (int(v) for v in line.split())
        colors.append((r, g, b))
    return colors


def write_jasc_pal(path: str, colors: Sequence[RGB], pad_to: int = 16) -> None:
    colors = list(colors)
    if len(colors) > pad_to:
        raise ValueError(f"{len(colors)} colors exceeds pad_to={pad_to}")
    while len(colors) < pad_to:
        colors.append((0, 0, 0))
    with open(path, "w", encoding="utf-8") as f:
        f.write("JASC-PAL\n0100\n")
        f.write(f"{len(colors)}\n")
        for (r, g, b) in colors:
            f.write(f"{r} {g} {b}\n")


def read_gba_bin_pal(path: str) -> List[RGB]:
    with open(path, "rb") as f:
        data = f.read()
    if len(data) % 2 != 0:
        raise ValueError(f"{path}: odd byte count, not a valid GBA palette")
    colors = []
    for (word,) in struct.iter_unpack("<H", data):
        r5 = word & 0x1F
        g5 = (word >> 5) & 0x1F
        b5 = (word >> 10) & 0x1F
        colors.append((_expand5(r5), _expand5(g5), _expand5(b5)))
    return colors


def write_gba_bin_pal(path: str, colors: Sequence[RGB]) -> None:
    with open(path, "wb") as f:
        for (r, g, b) in colors:
            r5, g5, b5 = _round5(r), _round5(g), _round5(b)
            word = r5 | (g5 << 5) | (b5 << 10)
            f.write(struct.pack("<H", word))


def load_palette(path: str) -> List[RGB]:
    """Auto-detect JASC-PAL (text) vs GBA binary (.pal/.gbapal) by content."""
    with open(path, "rb") as f:
        head = f.read(8)
    if head.startswith(b"JASC-PAL"):
        return read_jasc_pal(path)
    return read_gba_bin_pal(path)


# ---------------------------------------------------------------------------
# Median-cut quantization: reduce an arbitrary set of colors to <=16 entries.
# ---------------------------------------------------------------------------


def _box_range(colors: Sequence[RGB]) -> Tuple[int, int]:
    """Return (channel_index, range_size) of the widest channel in this box."""
    best_ch, best_range = 0, -1
    for ch in range(3):
        vals = [c[ch] for c in colors]
        r = max(vals) - min(vals)
        if r > best_range:
            best_ch, best_range = ch, r
    return best_ch, best_range


def median_cut_quantize(colors: Sequence[RGB], max_colors: int = 16) -> List[RGB]:
    """Median-cut quantization down to at most max_colors representative colors.

    Pure stdlib implementation (no numpy/Pillow). Deterministic and adequate
    for tile art (small, low-noise images) -- not meant to compete with a
    real image editor's quantizer for photographic input.
    """
    unique = list({c for c in colors})
    if len(unique) <= max_colors:
        return unique

    boxes = [unique]
    while len(boxes) < max_colors:
        # split the box with the largest channel range
        idx = max(range(len(boxes)), key=lambda i: _box_range(boxes[i])[1] if len(boxes[i]) > 1 else -1)
        box = boxes[idx]
        if len(box) <= 1:
            break
        ch, _ = _box_range(box)
        box_sorted = sorted(box, key=lambda c: c[ch])
        mid = len(box_sorted) // 2
        boxes[idx] = box_sorted[:mid]
        boxes.insert(idx + 1, box_sorted[mid:])

    result = []
    for box in boxes:
        if not box:
            continue
        n = len(box)
        avg = (
            sum(c[0] for c in box) // n,
            sum(c[1] for c in box) // n,
            sum(c[2] for c in box) // n,
        )
        result.append(avg)
    return result[:max_colors]


def nearest_index(color: RGB, palette: Sequence[RGB]) -> int:
    best_i, best_d = 0, None
    for i, p in enumerate(palette):
        d = (color[0] - p[0]) ** 2 + (color[1] - p[1]) ** 2 + (color[2] - p[2]) ** 2
        if best_d is None or d < best_d:
            best_i, best_d = i, d
    return best_i
