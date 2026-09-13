"""generate-fallback: last-resort procedural tile proposal, used only when
search-tiles turns up nothing usable.

This intentionally does NOT try to be a generative-art model -- it builds a
new tile by recombining statistics (the actual color palette, and the edge/
noise density) taken from the project's own existing tileset, so the
proposal at least sits in the right palette and roughly the right level of
detail instead of being pulled from nowhere. It is a placeholder for a human
(or a real art pass) to refine, not a finished asset.
"""
from __future__ import annotations

import random
from typing import List, Optional

from .gba_decode import TILE_SIZE
from .palette import RGB
from .png_codec import read_png


def _reference_palette_and_texture(reference_png_path: str):
    img = read_png(reference_png_path)
    rgb_rows = img.to_rgb_rows()
    colors = [c for row in rgb_rows for c in row]
    # crude "texture" signal: how often a pixel differs from its right
    # neighbor, used to decide how noisy/detailed the generated tile should be
    diffs = 0
    total = 0
    for row in rgb_rows:
        for i in range(len(row) - 1):
            total += 1
            if row[i] != row[i + 1]:
                diffs += 1
    detail_ratio = diffs / total if total else 0.15
    return colors, detail_ratio


def generate_fallback_tile(
    reference_png_path: str,
    seed: Optional[int] = None,
) -> List[List[RGB]]:
    """Return an 8x8 grid of RGB colors sampled from the reference image's
    own palette, with a detail level matched to the reference's edge density.
    Deterministic when `seed` is given, for reproducible proposals."""
    rng = random.Random(seed)
    colors, detail_ratio = _reference_palette_and_texture(reference_png_path)
    if not colors:
        raise ValueError(f"{reference_png_path}: image vide, impossible d'en tirer une palette")

    from collections import Counter

    ranked = [c for c, _ in Counter(colors).most_common(16)]
    base = ranked[0]
    accents = ranked[1:] or [base]

    tile = [[base for _ in range(TILE_SIZE)] for _ in range(TILE_SIZE)]
    num_accent_pixels = max(1, round(detail_ratio * TILE_SIZE * TILE_SIZE))
    placed = set()
    attempts = 0
    while len(placed) < num_accent_pixels and attempts < num_accent_pixels * 10:
        attempts += 1
        x, y = rng.randrange(TILE_SIZE), rng.randrange(TILE_SIZE)
        if (x, y) in placed:
            continue
        placed.add((x, y))
        tile[y][x] = rng.choice(accents)

    return tile
