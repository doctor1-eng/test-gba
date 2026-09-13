"""import-tile: convert a candidate community image into the project's tile
format (16-color indexed PNG, 8x8-aligned) and log it to ASSETS_SOURCES.md.

Hard gate: the ledger entry is written as part of the same in-memory-then-
commit operation as the converted files. If conversion fails for any reason,
nothing is written -- neither the ledger row nor the image files. There is no
code path that produces an image file under the project without a matching
ledger row, short of a user manually copying a file outside this tool (which
is exactly what `verify-sources` is for).
"""
from __future__ import annotations

import os
from dataclasses import dataclass
from typing import List, Optional

from . import sources_ledger
from .gba_decode import TILE_SIZE, tiles_from_indexed_grid, tiles_to_indexed_grid
from .palette import RGB, load_palette, median_cut_quantize, nearest_index, snap_to_gba_15bit, write_jasc_pal
from .png_codec import read_png, write_indexed_png


@dataclass
class ImportResult:
    dest_png: str
    dest_pal: Optional[str]
    width: int
    height: int
    num_colors: int
    tiles_deduped_from: Optional[int] = None


def _tile_key(tile, allow_flip_dedupe: bool):
    if not allow_flip_dedupe:
        return tuple(tuple(r) for r in tile)
    # canonical form for flip-aware dedupe: the lexicographically smallest
    # of the tile's 4 flip variants, so mirrored copies collapse together.
    from .gba_decode import flip_tile

    forms = [
        flip_tile(tile, False, False),
        flip_tile(tile, True, False),
        flip_tile(tile, False, True),
        flip_tile(tile, True, True),
    ]
    canon = min((tuple(tuple(r) for r in f) for f in forms))
    return canon


def convert_to_project_format(
    src_png_path: str,
    reuse_palette_path: Optional[str] = None,
    dedupe_tiles: bool = False,
    max_colors: int = 16,
):
    img = read_png(src_png_path)
    if img.width % TILE_SIZE or img.height % TILE_SIZE:
        raise ValueError(
            f"{src_png_path}: dimensions {img.width}x{img.height} ne sont pas un multiple de "
            f"{TILE_SIZE}px -- redimensionne/recadre l'image avant import."
        )

    rgb_rows = img.to_rgb_rows()

    if reuse_palette_path:
        palette: List[RGB] = load_palette(reuse_palette_path)
        if len(palette) > max_colors:
            raise ValueError(
                f"{reuse_palette_path}: {len(palette)} couleurs, plus que la limite {max_colors}"
            )
    else:
        all_colors = [c for row in rgb_rows for c in row]
        palette = median_cut_quantize(all_colors, max_colors=max_colors)
        palette = [snap_to_gba_15bit(c) for c in palette]

    indexed_grid = [[nearest_index(c, palette) for c in row] for row in rgb_rows]

    tiles_deduped_from = None
    if dedupe_tiles:
        tiles = tiles_from_indexed_grid(indexed_grid, img.width, img.height)
        seen = {}
        unique_tiles = []
        for tile in tiles:
            key = _tile_key(tile, allow_flip_dedupe=False)
            if key not in seen:
                seen[key] = len(unique_tiles)
                unique_tiles.append(tile)
        tiles_deduped_from = len(tiles)
        tiles_per_row = min(16, len(unique_tiles)) or 1
        indexed_grid = tiles_to_indexed_grid(unique_tiles, tiles_per_row)

    height = len(indexed_grid)
    width = len(indexed_grid[0]) if height else 0
    return indexed_grid, palette, width, height, tiles_deduped_from


def import_tile(
    src_png_path: str,
    dest_dir: str,
    name: str,
    source: str,
    license_: str,
    keyword: str,
    ledger_path: str = "ASSETS_SOURCES.md",
    reuse_palette_path: Optional[str] = None,
    dedupe_tiles: bool = False,
) -> ImportResult:
    sources_ledger.require_import_args(source, license_, keyword)

    if not os.path.exists(src_png_path):
        raise SystemExit(f"Import bloqué : fichier source introuvable : {src_png_path}")

    # 1) Conversion in memory first -- any failure here writes nothing at all.
    indexed_grid, palette, width, height, deduped_from = convert_to_project_format(
        src_png_path, reuse_palette_path=reuse_palette_path, dedupe_tiles=dedupe_tiles
    )

    os.makedirs(dest_dir, exist_ok=True)
    dest_png = os.path.join(dest_dir, f"{name}.png")
    dest_pal = None if reuse_palette_path else os.path.join(dest_dir, f"{name}.pal")

    # 2) Commit the ledger row *before* any project file is written, using the
    #    hash of the ORIGINAL source file (the thing that was actually found
    #    and licensed) so the entry stays valid even if conversion parameters
    #    change later.
    entry = sources_ledger.LedgerEntry(
        date_str=sources_ledger.today_str(),
        keyword=keyword,
        source=source,
        license=license_,
        dest_paths=[os.path.normpath(dest_png)] + ([os.path.normpath(dest_pal)] if dest_pal else []),
        sha256=sources_ledger.sha256_of_file(src_png_path),
    )
    sources_ledger.append_entry(ledger_path, entry)

    # 3) Only now write the actual project asset files.
    write_indexed_png(dest_png, width, height, indexed_grid, palette, transparent_index=0)
    if dest_pal:
        write_jasc_pal(dest_pal, palette, pad_to=16)

    return ImportResult(
        dest_png=dest_png,
        dest_pal=dest_pal,
        width=width,
        height=height,
        num_colors=len(palette),
        tiles_deduped_from=deduped_from,
    )
