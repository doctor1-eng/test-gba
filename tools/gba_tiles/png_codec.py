"""Minimal, dependency-free PNG reader/writer.

Written from scratch against the PNG spec (no Pillow/numpy available in this
environment). Scope is deliberately bounded to what GBA tile pipelines need:

Writer: 8-bit indexed-color (palette) PNGs, optionally with a tRNS entry for
index 0 (used to render transparent gaps in assembled metatile maps).

Reader: non-interlaced PNGs, 8-bit depth, color types 0 (grayscale), 2 (RGB),
3 (indexed, requires PLTE), 6 (RGBA). This covers essentially all tileset/
tile-pack PNGs encountered in the wild. Interlaced or 16-bit PNGs raise a
clear NotImplementedError rather than silently producing garbage.
"""
from __future__ import annotations

import struct
import zlib
from dataclasses import dataclass
from typing import List, Optional, Sequence, Tuple

PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"

RGB = Tuple[int, int, int]


def _chunk(tag: bytes, data: bytes) -> bytes:
    return (
        struct.pack(">I", len(data))
        + tag
        + data
        + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    )


def write_indexed_png(
    path: str,
    width: int,
    height: int,
    pixels: Sequence[Sequence[int]],
    palette: Sequence[RGB],
    transparent_index: Optional[int] = None,
    bit_depth: int = 8,
) -> None:
    """Write an indexed-color PNG (8-bit by default, or 4-bit -- the depth
    most tiles.png files in a real pokeemerald-family repo actually use, two
    16-color-or-fewer pixels packed per byte).

    pixels: row-major list of rows, each row a sequence of palette indices.
    palette: list of up to 256 (r, g, b) tuples (16 max when bit_depth=4).
    """
    if len(palette) > 256:
        raise ValueError(f"palette has {len(palette)} entries, PNG PLTE allows <=256")
    if bit_depth not in (4, 8):
        raise ValueError(f"unsupported bit_depth {bit_depth} (use 4 or 8)")
    if bit_depth == 4 and len(palette) > 16:
        raise ValueError(f"bit_depth=4 allows <=16 palette entries, got {len(palette)}")
    if len(pixels) != height:
        raise ValueError(f"expected {height} rows, got {len(pixels)}")

    plte = b"".join(struct.pack(">BBB", r, g, b) for (r, g, b) in palette)

    raw = bytearray()
    for row in pixels:
        if len(row) != width:
            raise ValueError(f"row has {len(row)} pixels, expected {width}")
        raw.append(0)  # filter type 0 (None) per scanline
        if bit_depth == 8:
            raw.extend(row)
        else:
            for i in range(0, width, 2):
                hi = row[i] & 0xF
                lo = row[i + 1] & 0xF if i + 1 < width else 0
                raw.append((hi << 4) | lo)

    ihdr = struct.pack(">IIBBBBB", width, height, bit_depth, 3, 0, 0, 0)
    idat = zlib.compress(bytes(raw), 9)

    out = bytearray(PNG_SIGNATURE)
    out += _chunk(b"IHDR", ihdr)
    out += _chunk(b"PLTE", plte)
    if transparent_index is not None:
        trns = bytes(
            [255] * transparent_index + [0] + [255] * (len(palette) - transparent_index - 1)
        )
        out += _chunk(b"tRNS", trns)
    out += _chunk(b"IDAT", idat)
    out += _chunk(b"IEND", b"")

    with open(path, "wb") as f:
        f.write(bytes(out))


def write_rgb_png(path: str, width: int, height: int, rgb_rows: Sequence[Sequence[RGB]]) -> None:
    """Write a truecolor 8-bit RGB PNG (no palette). Used for upscaled preview sheets."""
    raw = bytearray()
    for row in rgb_rows:
        if len(row) != width:
            raise ValueError(f"row has {len(row)} pixels, expected {width}")
        raw.append(0)
        for (r, g, b) in row:
            raw.extend((r, g, b))

    ihdr = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)
    idat = zlib.compress(bytes(raw), 9)

    out = bytearray(PNG_SIGNATURE)
    out += _chunk(b"IHDR", ihdr)
    out += _chunk(b"IDAT", idat)
    out += _chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(bytes(out))


@dataclass
class DecodedPNG:
    width: int
    height: int
    color_type: int
    palette: Optional[List[RGB]]
    # row-major list of rows; each pixel is either an int (palette index /
    # grayscale) or an (r, g, b) / (r, g, b, a) tuple, depending on color_type
    rows: List[List]

    def to_rgb_rows(self) -> List[List[RGB]]:
        if self.color_type == 3:
            assert self.palette is not None
            return [[self.palette[idx] for idx in row] for row in self.rows]
        if self.color_type == 0:
            return [[(v, v, v) for v in row] for row in self.rows]
        if self.color_type == 2:
            return self.rows  # already (r,g,b)
        if self.color_type == 6:
            return [[(r, g, b) for (r, g, b, a) in row] for row in self.rows]
        raise NotImplementedError(f"color_type {self.color_type}")


def _paeth(a: int, b: int, c: int) -> int:
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def read_png(path: str) -> DecodedPNG:
    with open(path, "rb") as f:
        data = f.read()

    if data[:8] != PNG_SIGNATURE:
        raise ValueError(f"{path}: not a PNG file (bad signature)")

    pos = 8
    width = height = bit_depth = color_type = interlace = None
    palette: Optional[List[RGB]] = None
    idat = bytearray()

    while pos < len(data):
        (length,) = struct.unpack(">I", data[pos : pos + 4])
        tag = data[pos + 4 : pos + 8]
        chunk_data = data[pos + 8 : pos + 8 + length]
        pos += 8 + length + 4  # skip CRC

        if tag == b"IHDR":
            width, height, bit_depth, color_type, _comp, _filt, interlace = struct.unpack(
                ">IIBBBBB", chunk_data
            )
        elif tag == b"PLTE":
            palette = [
                tuple(chunk_data[i : i + 3]) for i in range(0, len(chunk_data), 3)
            ]
        elif tag == b"IDAT":
            idat += chunk_data
        elif tag == b"IEND":
            break

    if width is None:
        raise ValueError(f"{path}: missing IHDR")
    if interlace != 0:
        raise NotImplementedError(f"{path}: interlaced PNGs are not supported")
    if color_type == 3 and palette is None:
        raise ValueError(f"{path}: indexed PNG missing PLTE")

    channels = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}.get(color_type)
    if channels is None:
        raise NotImplementedError(f"{path}: unsupported color type {color_type}")
    if bit_depth != 8 and color_type not in (0, 3):
        # RGB/RGBA/grayscale+alpha are always >=8 bits per the PNG spec;
        # only grayscale and indexed color ever use sub-byte depths.
        raise NotImplementedError(
            f"{path}: bit depth {bit_depth} with color type {color_type} is not supported"
        )
    if bit_depth not in (1, 2, 4, 8):
        raise NotImplementedError(f"{path}: unsupported bit depth {bit_depth}")

    # Per the PNG spec, "bpp" for filtering purposes is bytes per *complete*
    # pixel, minimum 1 -- for sub-byte indexed/grayscale depths this is 1
    # regardless of bit_depth, since a pixel never spans a byte boundary.
    bpp = max(1, (bit_depth * channels) // 8)
    stride = (width * bit_depth * channels + 7) // 8

    raw = zlib.decompress(bytes(idat))

    rows_bytes: List[bytearray] = []
    prev = bytearray(stride)
    offset = 0
    for _ in range(height):
        filt = raw[offset]
        offset += 1
        cur = bytearray(raw[offset : offset + stride])
        offset += stride

        if filt == 0:
            pass
        elif filt == 1:  # Sub
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                cur[i] = (cur[i] + a) & 0xFF
        elif filt == 2:  # Up
            for i in range(stride):
                cur[i] = (cur[i] + prev[i]) & 0xFF
        elif filt == 3:  # Average
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                b = prev[i]
                cur[i] = (cur[i] + ((a + b) // 2)) & 0xFF
        elif filt == 4:  # Paeth
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                b = prev[i]
                c = prev[i - bpp] if i >= bpp else 0
                cur[i] = (cur[i] + _paeth(a, b, c)) & 0xFF
        else:
            raise ValueError(f"{path}: unknown filter type {filt}")

        rows_bytes.append(cur)
        prev = cur

    def unpack_subbyte_row(rb: bytearray) -> List[int]:
        pixels_per_byte = 8 // bit_depth
        mask = (1 << bit_depth) - 1
        out = []
        for byte in rb:
            for shift in range(8 - bit_depth, -1, -bit_depth):
                out.append((byte >> shift) & mask)
                if len(out) == width:
                    break
            if len(out) == width:
                break
        return out

    rows: List[List] = []
    for rb in rows_bytes:
        if color_type in (3, 0) and bit_depth < 8:
            rows.append(unpack_subbyte_row(rb))
        elif color_type == 3 or color_type == 0:
            rows.append(list(rb))
        elif color_type == 2:
            rows.append([tuple(rb[i : i + 3]) for i in range(0, stride, 3)])
        elif color_type == 6:
            rows.append([tuple(rb[i : i + 4]) for i in range(0, stride, 4)])

    return DecodedPNG(
        width=width, height=height, color_type=color_type, palette=palette, rows=rows
    )
