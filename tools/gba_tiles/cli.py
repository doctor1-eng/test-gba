"""CLI entrypoint. Run via `python gba_tiles.py <command> ...` from the repo
root (see gba_tiles.py) or `python -m tools.gba_tiles.cli <command> ...`.
"""
from __future__ import annotations

import argparse
import os
import sys

from . import fallback_gen, importer, search, sources_ledger
from .gba_decode import decode_tiles_4bpp, tiles_to_indexed_grid
from .metatiles import (
    build_combined_palette,
    compose_metatile_indexed,
    parse_map_blocks,
    parse_metatiles,
)
from .palette import load_palette
from .png_codec import read_png, write_indexed_png

DEFAULT_LEDGER = "ASSETS_SOURCES.md"


def _load_tiles_from_source(raw_path, indexed_png_path):
    """Shared helper: load a tileset either from a raw .4bpp binary or from
    an already-indexed tiles.png (the layout modern pokeemerald-expansion
    keeps in-repo)."""
    from .gba_decode import tiles_from_indexed_grid

    if raw_path:
        with open(raw_path, "rb") as f:
            return decode_tiles_4bpp(f.read())
    img = read_png(indexed_png_path)
    if img.color_type != 3:
        raise SystemExit(f"{indexed_png_path}: attendu un PNG indexé (color type 3), reçu type {img.color_type}")
    return tiles_from_indexed_grid(img.rows, img.width, img.height)


def _resolve_tiles_and_palettes(args):
    """Resolve primary/secondary tile pools and the combined palette table.

    Two ways in:
    - `--primary-dir`/`--secondary-dir` (recommended): point at a real
      `data/tilesets/{primary,secondary}/<name>/` directory and this derives
      `tiles.png` + the correct palette slice automatically, using the exact
      rule the game engine itself uses (verified against
      src/fieldmap.c:LoadSecondaryTilesetPalette): primary contributes its
      own palette files [0, num_pals_primary), and secondary contributes
      *its own files named [num_pals_primary, num_pals_total)* -- NOT
      secondary's files renumbered from 0. Concatenating "all of primary's
      files then all of secondary's files from 0" (an earlier, wrong version
      of this tool did exactly that) silently shifts every secondary-tileset
      color by `num_pals_primary` slots and produces a broken-looking render
      for any map that actually uses a secondary tileset.
    - `--primary-raw`/`--primary-indexed-png` + `--palettes` (manual/legacy):
      full control, but you are responsible for passing palettes already in
      correct global-slot order.
    """
    if args.primary_dir:
        primary_tiles = _load_tiles_from_source(None, os.path.join(args.primary_dir, "tiles.png"))
        primary_pals = [
            load_palette(os.path.join(args.primary_dir, "palettes", f"{i:02d}.pal"))
            for i in range(args.num_pals_primary)
        ]
        secondary_tiles: list = []
        secondary_pals: list = []
        if args.secondary_dir:
            secondary_tiles = _load_tiles_from_source(None, os.path.join(args.secondary_dir, "tiles.png"))
            secondary_pals = [
                load_palette(os.path.join(args.secondary_dir, "palettes", f"{i:02d}.pal"))
                for i in range(args.num_pals_primary, args.num_pals_total)
            ]
        combined_palette = build_combined_palette(primary_pals + secondary_pals)
        metatiles_path = args.metatiles or os.path.join(args.primary_dir, "metatiles.bin")
        secondary_metatiles_path = (
            None if args.metatiles else (
                os.path.join(args.secondary_dir, "metatiles.bin") if args.secondary_dir else None
            )
        )
        return primary_tiles, secondary_tiles, combined_palette, metatiles_path, secondary_metatiles_path

    primary_tiles = _load_tiles_from_source(args.primary_raw, args.primary_indexed_png)
    secondary_tiles = (
        _load_tiles_from_source(args.secondary_raw, args.secondary_indexed_png)
        if (args.secondary_raw or args.secondary_indexed_png)
        else []
    )
    if not args.palettes:
        raise SystemExit("Fournis --primary-dir (recommandé) ou --palettes en mode manuel.")
    combined_palette = build_combined_palette([load_palette(p) for p in args.palettes])
    if not args.metatiles:
        raise SystemExit("--metatiles requis en mode manuel (ou utilise --primary-dir pour le déduire).")
    return primary_tiles, secondary_tiles, combined_palette, args.metatiles, None


def _load_metatiles_combined(metatiles_path, secondary_metatiles_path):
    """Load a tileset's metatiles.bin, concatenated with a secondary
    tileset's own metatiles.bin when given -- required for render-map, since
    a map's metatile ids span the combined [0, NUM_METATILES_TOTAL) range:
    ids < NUM_METATILES_IN_PRIMARY index the primary tileset's own
    metatiles.bin, and everything from there on indexes the secondary
    tileset's own (separately 0-indexed) metatiles.bin. Loading only the
    primary file silently drops every metatile the map places from its
    secondary tileset -- exactly the bug that made a first pass at rendering
    a real indoor map come out as a flat, near-empty color instead of a
    room."""
    with open(metatiles_path, "rb") as f:
        metatiles = parse_metatiles(f.read())
    if secondary_metatiles_path:
        with open(secondary_metatiles_path, "rb") as f:
            metatiles = metatiles + parse_metatiles(f.read())
    return metatiles


def cmd_decode_tileset(args):
    tiles = _load_tiles_from_source(args.raw, args.indexed_png)
    palette = load_palette(args.palette) if args.palette else None
    if palette is None and args.indexed_png:
        img = read_png(args.indexed_png)
        palette = img.palette
    if palette is None:
        raise SystemExit("Fournis --palette (fichier .pal) ou une source --indexed-png avec sa propre PLTE.")

    tiles_per_row = args.tiles_per_row
    grid = tiles_to_indexed_grid(tiles, tiles_per_row)
    height, width = len(grid), len(grid[0])
    write_indexed_png(args.out, width, height, grid, palette, transparent_index=0)
    print(f"OK: {len(tiles)} tuiles -> {args.out} ({width}x{height}px, {tiles_per_row} tuiles/ligne)")


def cmd_render_tileset(args):
    primary_tiles, secondary_tiles, combined_palette, metatiles_path, _ = _resolve_tiles_and_palettes(args)

    with open(metatiles_path, "rb") as f:
        metatiles = parse_metatiles(f.read())

    cols = args.metatiles_per_row
    rows_of_mt = (len(metatiles) + cols - 1) // cols
    sheet_w, sheet_h = cols * 16, rows_of_mt * 16
    sheet = [[0] * sheet_w for _ in range(sheet_h)]

    for i, mt in enumerate(metatiles):
        mt_grid = compose_metatile_indexed(mt, primary_tiles, secondary_tiles, args.num_tiles_primary)
        ox, oy = (i % cols) * 16, (i // cols) * 16
        for ry in range(16):
            sheet[oy + ry][ox : ox + 16] = mt_grid[ry]

    write_indexed_png(args.out, sheet_w, sheet_h, sheet, combined_palette, transparent_index=0)
    print(
        f"OK: {len(metatiles)} metatiles -> {args.out} "
        f"({sheet_w}x{sheet_h}px avant upscale, {cols} metatiles/ligne)"
    )
    if args.scale > 1:
        _upscale_indexed_png_inplace(args.out, sheet, combined_palette, args.scale)


def cmd_render_map(args):
    primary_tiles, secondary_tiles, combined_palette, metatiles_path, secondary_metatiles_path = (
        _resolve_tiles_and_palettes(args)
    )
    metatiles = _load_metatiles_combined(metatiles_path, secondary_metatiles_path)
    with open(args.map, "rb") as f:
        map_data = f.read()
    blocks = parse_map_blocks(map_data, args.width, args.height)

    sheet_w, sheet_h = args.width * 16, args.height * 16
    sheet = [[0] * sheet_w for _ in range(sheet_h)]
    for by, row in enumerate(blocks):
        for bx, block in enumerate(row):
            if block.metatile_id >= len(metatiles):
                continue
            mt_grid = compose_metatile_indexed(
                metatiles[block.metatile_id], primary_tiles, secondary_tiles, args.num_tiles_primary
            )
            ox, oy = bx * 16, by * 16
            for ry in range(16):
                sheet[oy + ry][ox : ox + 16] = mt_grid[ry]

    write_indexed_png(args.out, sheet_w, sheet_h, sheet, combined_palette, transparent_index=0)
    print(f"OK: carte {args.width}x{args.height} blocs -> {args.out} ({sheet_w}x{sheet_h}px avant upscale)")
    if args.scale > 1:
        _upscale_indexed_png_inplace(args.out, sheet, combined_palette, args.scale)


def _upscale_indexed_png_inplace(out_path, sheet, palette, scale):
    from .png_codec import write_rgb_png

    h, w = len(sheet), len(sheet[0])
    up_rows = []
    for y in range(h * scale):
        src_y = y // scale
        row = []
        for x in range(w * scale):
            src_x = x // scale
            row.append(palette[sheet[src_y][src_x]])
        up_rows.append(row)
    root, ext = os.path.splitext(out_path)
    scaled_path = f"{root}_x{scale}{ext}"
    write_rgb_png(scaled_path, w * scale, h * scale, up_rows)
    print(f"OK: version upscalee x{scale} -> {scaled_path}")


def cmd_search_tiles(args):
    results = search.search(
        args.keyword,
        project_reference_png=args.project_tileset,
        preview_cache_dir=args.preview_cache_dir,
    )
    print(search.format_results(results, args.project_tileset))


def cmd_import_tile(args):
    result = importer.import_tile(
        src_png_path=args.input,
        dest_dir=args.dest_dir,
        name=args.name,
        source=args.source,
        license_=args.license,
        keyword=args.keyword,
        ledger_path=args.ledger,
        reuse_palette_path=args.reuse_palette,
        dedupe_tiles=args.dedupe_tiles,
    )
    print(f"OK: importé et journalisé dans {args.ledger}")
    print(f"  -> {result.dest_png} ({result.width}x{result.height}px, {result.num_colors} couleurs)")
    if result.dest_pal:
        print(f"  -> {result.dest_pal} (nouvelle palette générée)")
    if result.tiles_deduped_from:
        print(f"  -> tuiles dédupliquées: {result.tiles_deduped_from} -> feuille compactée")


def cmd_generate_fallback(args):
    tile = fallback_gen.generate_fallback_tile(args.reference, seed=args.seed)
    from .palette import median_cut_quantize, snap_to_gba_15bit
    from .png_codec import write_indexed_png
    from .palette import nearest_index

    flat = [c for row in tile for c in row]
    palette = [snap_to_gba_15bit(c) for c in median_cut_quantize(flat, max_colors=16)]
    indexed = [[nearest_index(c, palette) for c in row] for row in tile]
    write_indexed_png(args.out, 8, 8, indexed, palette)
    print(f"PROPOSITION (fallback, non issue d'une recherche) -> {args.out}")
    print(
        "Rappel : cette tile est générée procéduralement à partir de la palette/texture "
        "du fichier de référence, faute d'asset communautaire adapté trouvé. "
        "À revoir par un humain avant intégration."
    )


def cmd_verify_sources(args):
    untracked = sources_ledger.find_untracked_assets(args.graphics_dir, args.ledger)
    if not untracked:
        print(f"OK: tous les fichiers sous {args.graphics_dir} sont couverts par {args.ledger}")
        return
    print(f"BLOQUANT: {len(untracked)} fichier(s) sous {args.graphics_dir} sans entrée dans {args.ledger} :")
    for path in untracked:
        print(f"  - {path}")
    sys.exit(1)


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="gba_tiles",
        description="Visualisation de tilesets GBA + recherche/import d'assets communautaires.",
    )
    sub = p.add_subparsers(dest="command", required=True)

    # ---- Mode 1: visualisation ----
    d = sub.add_parser("decode-tileset", help="Décoder un tileset (raw 4bpp ou PNG indexé) en PNG lisible")
    src = d.add_mutually_exclusive_group(required=True)
    src.add_argument("--raw", help="fichier .4bpp brut (format natif GBA)")
    src.add_argument("--indexed-png", help="PNG déjà indexé (format repo pokeemerald moderne)")
    d.add_argument("--palette", help=".pal (JASC ou binaire GBA); sinon on réutilise la PLTE du PNG d'entrée")
    d.add_argument("--out", required=True)
    d.add_argument("--tiles-per-row", type=int, default=16)
    d.set_defaults(func=cmd_decode_tileset)

    r = sub.add_parser("render-tileset", help="Assembler tous les metatiles d'un tileset en planche PNG")
    r.add_argument(
        "--primary-dir",
        help="dossier data/tilesets/primary/<nom>/ (recommandé -- déduit tiles.png, palettes/ et metatiles.bin)",
    )
    r.add_argument("--secondary-dir", help="dossier data/tilesets/secondary/<nom>/ (si la carte en utilise un)")
    r.add_argument("--metatiles", help="metatiles.bin (déduit de --primary-dir si omis)")
    r.add_argument("--primary-raw", help="mode manuel : .4bpp brut au lieu de --primary-dir")
    r.add_argument("--primary-indexed-png", help="mode manuel : tiles.png au lieu de --primary-dir")
    r.add_argument("--secondary-raw")
    r.add_argument("--secondary-indexed-png")
    r.add_argument(
        "--palettes",
        nargs="+",
        help="mode manuel : fichiers .pal déjà dans l'ordre des slots globaux (0..N) -- préférer --primary-dir",
    )
    r.add_argument(
        "--num-pals-primary",
        type=int,
        default=7,
        help="NUM_PALS_IN_PRIMARY (voir include/fieldmap.h -- 7 pour layout HNS/FRLG actif, 6 si swap Emerald)",
    )
    r.add_argument("--num-pals-total", type=int, default=13, help="NUM_PALS_TOTAL (include/fieldmap.h)")
    r.add_argument(
        "--num-tiles-primary",
        type=int,
        default=640,
        help="NUM_TILES_IN_PRIMARY (include/fieldmap.h -- 640 pour layout HNS/FRLG actif, 512 si swap Emerald)",
    )
    r.add_argument("--metatiles-per-row", type=int, default=8)
    r.add_argument("--out", required=True)
    r.add_argument("--scale", type=int, default=1, help="ex: 4 ou 8 pour une version upscalée")
    r.set_defaults(func=cmd_render_tileset)

    m = sub.add_parser("render-map", help="Assembler une carte complète (map.bin + metatiles) en planche PNG")
    m.add_argument("--map", required=True, help="map.bin (layout de la carte, ex: data/layouts/<Nom>/map.bin)")
    m.add_argument("--width", type=int, required=True, help="largeur de la carte en blocs (voir layouts.json)")
    m.add_argument("--height", type=int, required=True, help="hauteur de la carte en blocs (voir layouts.json)")
    m.add_argument(
        "--primary-dir",
        help="dossier data/tilesets/primary/<nom>/ (recommandé -- déduit tiles.png, palettes/ et metatiles.bin)",
    )
    m.add_argument("--secondary-dir", help="dossier data/tilesets/secondary/<nom>/ (si la carte en utilise un)")
    m.add_argument("--metatiles", help="metatiles.bin (déduit de --primary-dir si omis)")
    m.add_argument("--primary-raw", help="mode manuel : .4bpp brut au lieu de --primary-dir")
    m.add_argument("--primary-indexed-png", help="mode manuel : tiles.png au lieu de --primary-dir")
    m.add_argument("--secondary-raw")
    m.add_argument("--secondary-indexed-png")
    m.add_argument(
        "--palettes",
        nargs="+",
        help="mode manuel : fichiers .pal déjà dans l'ordre des slots globaux (0..N) -- préférer --primary-dir",
    )
    m.add_argument("--num-pals-primary", type=int, default=7)
    m.add_argument("--num-pals-total", type=int, default=13)
    m.add_argument("--num-tiles-primary", type=int, default=640)
    m.add_argument("--out", required=True)
    m.add_argument("--scale", type=int, default=1)
    m.set_defaults(func=cmd_render_map)

    # ---- Mode 2: recherche & import ----
    s = sub.add_parser("search-tiles", help="Chercher des tiles communautaires par mot-clé")
    s.add_argument("keyword")
    s.add_argument("--project-tileset", help="PNG de référence du projet, pour comparer le style/palette")
    s.add_argument(
        "--preview-cache-dir",
        help="dossier où placer les aperçus PNG téléchargés manuellement (nommés <id>.png) pour le scoring de style",
    )
    s.set_defaults(func=cmd_search_tiles)

    i = sub.add_parser(
        "import-tile",
        help="Convertir une image candidate au format projet et journaliser dans ASSETS_SOURCES.md (obligatoire)",
    )
    i.add_argument("input", help="PNG source (déjà téléchargé localement)")
    i.add_argument("--dest-dir", required=True)
    i.add_argument("--name", required=True, help="nom de base pour les fichiers de sortie (cohérent avec l'existant)")
    i.add_argument("--source", required=True, help="URL ou référence de la source")
    i.add_argument("--license", required=True, help="licence déclarée (ex: CC0, CC-BY 4.0)")
    i.add_argument("--keyword", required=True, help="mot-clé de recherche ayant mené à cet asset")
    i.add_argument("--ledger", default=DEFAULT_LEDGER)
    i.add_argument("--reuse-palette", help="réutiliser une palette .pal existante du projet au lieu d'en générer une")
    i.add_argument("--dedupe-tiles", action="store_true")
    i.set_defaults(func=cmd_import_tile)

    g = sub.add_parser(
        "generate-fallback",
        help="Fallback : proposer une tile inédite dans le style d'un tileset existant (à utiliser seulement si search-tiles n'a rien donné)",
    )
    g.add_argument("--reference", required=True, help="PNG du tileset existant dont on imite palette/texture")
    g.add_argument("--out", required=True)
    g.add_argument("--seed", type=int, default=None)
    g.set_defaults(func=cmd_generate_fallback)

    v = sub.add_parser(
        "verify-sources",
        help="Vérifier qu'aucun asset sous un dossier graphics n'a été ajouté sans entrée ASSETS_SOURCES.md",
    )
    v.add_argument("graphics_dir")
    v.add_argument("--ledger", default=DEFAULT_LEDGER)
    v.set_defaults(func=cmd_verify_sources)

    return p


def main(argv=None):
    parser = build_parser()
    args = parser.parse_args(argv)
    args.func(args)


if __name__ == "__main__":
    main()
