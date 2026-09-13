"""Pokeemerald-family metatile format: metatiles.bin (+ optional
metatile_attributes.bin) and map layout (map.bin) parsing, plus compositing
metatiles into a full-color pixel grid for preview PNGs.

Format reminder (standard across pokeemerald / pokeemerald-expansion, vanilla
constants -- override via CLI flags if your project's include/constants.h
differs, e.g. an expansion build with extended tile/metatile counts):

  metatiles.bin: array of metatiles. Each metatile is 8 x uint16 (little-
  endian) "tile" entries: bottom layer as a 2x2 block of 8x8 tiles (TL, TR,
  BL, BR), then top layer as another 2x2 block, in the same order. Each u16:
    bits 0-9:  local tile id (0-1023) into the combined primary+secondary
               tileset (primary occupies ids [0, NUM_TILES_PRIMARY), the
               secondary tileset continues from there)
    bit 10:    horizontal flip
    bit 11:    vertical flip
    bits 12-15: palette number (0-15)

  map.bin (a map's layout blob): array of uint16 "map blocks":
    bits 0-9:   metatile id (primary tileset ids first, then secondary)
    bits 10-11: collision
    bits 12-15: elevation

  BG rendering convention: palette index 0 within a tile's 16-color bank is
  transparent for that layer. We use this to let the bottom layer's pixel
  show through wherever the top layer is index 0 -- this is the standard
  convention porymap itself relies on for a legible preview; it is a
  simplification (it ignores per-metatile layer-type/encounter-type from
  metatile_attributes, which change compositing in a running game for a
  handful of special metatile types).
"""
from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import List, Sequence, Tuple

from .gba_decode import TILE_SIZE, Tile, flip_tile

TILE_ENTRIES_PER_METATILE = 8  # 4 bottom + 4 top


@dataclass
class TileRef:
    tile_id: int
    xflip: bool
    yflip: bool
    pal_num: int

    @staticmethod
    def from_u16(word: int) -> "TileRef":
        return TileRef(
            tile_id=word & 0x3FF,
            xflip=bool(word & 0x400),
            yflip=bool(word & 0x800),
            pal_num=(word >> 12) & 0xF,
        )


@dataclass
class Metatile:
    bottom: List[TileRef]  # 4 entries, 2x2
    top: List[TileRef]  # 4 entries, 2x2


def parse_metatiles(data: bytes) -> List[Metatile]:
    entry_size = TILE_ENTRIES_PER_METATILE * 2
    if len(data) % entry_size != 0:
        raise ValueError(
            f"metatiles.bin length {len(data)} is not a multiple of {entry_size} bytes"
        )
    metatiles = []
    count = len(data) // entry_size
    words = struct.unpack(f"<{count * TILE_ENTRIES_PER_METATILE}H", data)
    for m in range(count):
        base = m * TILE_ENTRIES_PER_METATILE
        refs = [TileRef.from_u16(words[base + i]) for i in range(TILE_ENTRIES_PER_METATILE)]
        metatiles.append(Metatile(bottom=refs[0:4], top=refs[4:8]))
    return metatiles


@dataclass
class MapBlock:
    metatile_id: int
    collision: int
    elevation: int


def parse_map_blocks(data: bytes, width: int, height: int) -> List[List[MapBlock]]:
    expected = width * height * 2
    if len(data) < expected:
        raise ValueError(
            f"map.bin is {len(data)} bytes, need at least {expected} for a {width}x{height} map"
        )
    words = struct.unpack(f"<{width * height}H", data[:expected])
    grid = []
    for y in range(height):
        row = []
        for x in range(width):
            w = words[y * width + x]
            row.append(
                MapBlock(
                    metatile_id=w & 0x3FF,
                    collision=(w >> 10) & 0x3,
                    elevation=(w >> 12) & 0xF,
                )
            )
        grid.append(row)
    return grid


def resolve_tile(
    ref: TileRef,
    primary_tiles: Sequence[Tile],
    secondary_tiles: Sequence[Tile],
    num_tiles_primary: int,
) -> Tile:
    if ref.tile_id < num_tiles_primary:
        pool, local_id = primary_tiles, ref.tile_id
    else:
        pool, local_id = secondary_tiles, ref.tile_id - num_tiles_primary
    if local_id >= len(pool):
        # Out-of-range tile references show up in real projects (unused
        # slots); render as blank rather than crashing the whole sheet.
        return [[0] * TILE_SIZE for _ in range(TILE_SIZE)]
    return flip_tile(pool[local_id], ref.xflip, ref.yflip)


def compose_metatile_indexed(
    metatile: Metatile,
    primary_tiles: Sequence[Tile],
    secondary_tiles: Sequence[Tile],
    num_tiles_primary: int,
) -> List[List[int]]:
    """Return a 16x16 grid of (pal_num*16 + pixel) combined-palette indices,
    ready to look up in a full 256-entry combined RGB palette table."""
    grid = [[0] * 16 for _ in range(16)]

    def blit(refs: List[TileRef], transparent_on_zero: bool):
        for slot, ref in enumerate(refs):
            tile_px = resolve_tile(ref, primary_tiles, secondary_tiles, num_tiles_primary)
            ox, oy = (slot % 2) * TILE_SIZE, (slot // 2) * TILE_SIZE
            for ry in range(TILE_SIZE):
                for rx in range(TILE_SIZE):
                    px = tile_px[ry][rx]
                    if transparent_on_zero and px == 0:
                        continue
                    grid[oy + ry][ox + rx] = ref.pal_num * 16 + px

    blit(metatile.bottom, transparent_on_zero=False)
    blit(metatile.top, transparent_on_zero=True)
    return grid


def build_combined_palette(palettes: Sequence[Sequence[Tuple[int, int, int]]]) -> List[Tuple[int, int, int]]:
    """Concatenate up to 16 sub-palettes of 16 colors each into one 256-entry
    RGB table indexed by pal_num*16 + pixel, padding short palettes/lists."""
    combined: List[Tuple[int, int, int]] = []
    for pal in palettes:
        pal = list(pal)[:16]
        pal += [(0, 0, 0)] * (16 - len(pal))
        combined.extend(pal)
    combined += [(0, 0, 0)] * (256 - len(combined))
    return combined[:256]
