"""GBA 4bpp tile <-> 8x8 indexed pixel grid conversion, and sheet assembly.

GBA native tile format ("4bpp"): each 8x8 tile is 32 bytes, 2 pixels per
byte, low nibble = left pixel, high nibble = right pixel, rows top to bottom.
Modern pokeemerald-expansion stores tiles as indexed PNGs in the repo and
lets gbagfx compile them to this format at build time -- this module covers
both directions so the CLI can work from whichever the user has on hand
(raw compiled .4bpp, or an already-indexed tiles.png).
"""
from __future__ import annotations

from typing import List, Sequence

TILE_SIZE = 8
BYTES_PER_TILE_4BPP = 32

Tile = List[List[int]]  # 8x8 grid of palette indices (0-15)


def decode_tiles_4bpp(data: bytes) -> List[Tile]:
    if len(data) % BYTES_PER_TILE_4BPP != 0:
        raise ValueError(
            f"data length {len(data)} is not a multiple of {BYTES_PER_TILE_4BPP} "
            "(not a valid raw 4bpp GBA tileset)"
        )
    tiles = []
    for t in range(len(data) // BYTES_PER_TILE_4BPP):
        block = data[t * BYTES_PER_TILE_4BPP : (t + 1) * BYTES_PER_TILE_4BPP]
        tile: Tile = []
        for row in range(TILE_SIZE):
            row_bytes = block[row * 4 : row * 4 + 4]
            pixels = []
            for byte in row_bytes:
                pixels.append(byte & 0x0F)
                pixels.append((byte >> 4) & 0x0F)
            tile.append(pixels)
        tiles.append(tile)
    return tiles


def encode_tiles_4bpp(tiles: Sequence[Tile]) -> bytes:
    out = bytearray()
    for tile in tiles:
        if len(tile) != TILE_SIZE or any(len(r) != TILE_SIZE for r in tile):
            raise ValueError("each tile must be an 8x8 grid")
        for row in tile:
            for i in range(0, TILE_SIZE, 2):
                lo = row[i] & 0x0F
                hi = row[i + 1] & 0x0F
                out.append(lo | (hi << 4))
    return bytes(out)


def tiles_from_indexed_grid(rows: Sequence[Sequence[int]], width_px: int, height_px: int) -> List[Tile]:
    """Slice a full indexed pixel grid (as read from an indexed PNG) into
    8x8 tiles, in the standard left-to-right, top-to-bottom tile order."""
    if width_px % TILE_SIZE or height_px % TILE_SIZE:
        raise ValueError(
            f"image size {width_px}x{height_px} is not a multiple of {TILE_SIZE}px"
        )
    tiles: List[Tile] = []
    tiles_x = width_px // TILE_SIZE
    tiles_y = height_px // TILE_SIZE
    for ty in range(tiles_y):
        for tx in range(tiles_x):
            tile = [
                list(rows[ty * TILE_SIZE + ry][tx * TILE_SIZE : tx * TILE_SIZE + TILE_SIZE])
                for ry in range(TILE_SIZE)
            ]
            tiles.append(tile)
    return tiles


def tiles_to_indexed_grid(tiles: Sequence[Tile], tiles_per_row: int) -> List[List[int]]:
    """Lay out a flat tile list into a single indexed pixel grid, wrapping at
    tiles_per_row tiles -- i.e. build the classic tileset sheet PNG."""
    n = len(tiles)
    rows_of_tiles = (n + tiles_per_row - 1) // tiles_per_row
    width_px = tiles_per_row * TILE_SIZE
    height_px = rows_of_tiles * TILE_SIZE
    grid = [[0] * width_px for _ in range(height_px)]
    for i, tile in enumerate(tiles):
        tx, ty = i % tiles_per_row, i // tiles_per_row
        for ry in range(TILE_SIZE):
            grid[ty * TILE_SIZE + ry][tx * TILE_SIZE : tx * TILE_SIZE + TILE_SIZE] = tile[ry]
    return grid


def flip_tile(tile: Tile, xflip: bool, yflip: bool) -> Tile:
    result = tile
    if xflip:
        result = [list(reversed(r)) for r in result]
    if yflip:
        result = list(reversed(result))
    return result
