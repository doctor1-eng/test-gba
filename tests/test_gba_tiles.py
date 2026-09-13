"""Self-tests for tools/gba_tiles. Most cases use synthetic data so they run
without the real game assets; CliPrimarySecondaryResolutionTests specifically
locks in two bugs found and fixed by rendering real maps from this project
(data/tilesets/..., data/layouts/...) after the tool was rebased onto the
actual pokeemerald-expansion "Heart & Soul" branch:

1. Palette assembly for a primary+secondary tileset pair is NOT "all of
   primary's palette files then all of secondary's from 0" -- verified
   against src/fieldmap.c:LoadSecondaryTilesetPalette, the secondary
   tileset's *own* files [num_pals_primary, num_pals_total) are the ones
   actually used; its files [0, num_pals_primary) are unused placeholders.
2. A map's metatile ids span the combined primary+secondary range, so
   render-map must concatenate both tileset's own metatiles.bin, not just
   read the primary one -- otherwise every metatile the map places from its
   secondary tileset silently renders as blank.

Run with:

    python3 -m unittest tests.test_gba_tiles -v

from the repo root.
"""
from __future__ import annotations

import os
import struct
import sys
import tempfile
import unittest
from types import SimpleNamespace

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from tools.gba_tiles import cli, fallback_gen, importer, search, sources_ledger  # noqa: E402
from tools.gba_tiles.gba_decode import (  # noqa: E402
    decode_tiles_4bpp,
    encode_tiles_4bpp,
    tiles_from_indexed_grid,
    tiles_to_indexed_grid,
)
from tools.gba_tiles.metatiles import (  # noqa: E402
    build_combined_palette,
    compose_metatile_indexed,
    parse_map_blocks,
    parse_metatiles,
)
from tools.gba_tiles.palette import (  # noqa: E402
    median_cut_quantize,
    nearest_index,
    read_jasc_pal,
    write_jasc_pal,
)
from tools.gba_tiles.png_codec import read_png, write_indexed_png, write_rgb_png  # noqa: E402


class PngRoundTripTests(unittest.TestCase):
    def test_indexed_png_round_trip(self):
        palette = [(0, 0, 0), (255, 0, 0), (0, 255, 0), (0, 0, 255)]
        pixels = [[0, 1, 2, 3], [3, 2, 1, 0]]
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "test.png")
            write_indexed_png(path, 4, 2, pixels, palette)
            img = read_png(path)
            self.assertEqual(img.width, 4)
            self.assertEqual(img.height, 2)
            self.assertEqual(img.color_type, 3)
            self.assertEqual(img.rows, pixels)
            self.assertEqual(img.palette[:4], palette)

    def test_4bit_indexed_png_round_trip(self):
        # The majority of this real project's tiles.png files (139 of 238 at
        # last count) are 4-bit indexed PNGs, not 8-bit -- render-map failed
        # outright on them (NotImplementedError) until the reader learned to
        # unpack sub-byte pixel depths.
        palette = [(i * 16, i * 16, i * 16) for i in range(16)]
        pixels = [[0, 1, 2, 3, 4], [15, 14, 13, 12, 11]]  # odd width: tests row padding
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "test4.png")
            write_indexed_png(path, 5, 2, pixels, palette, bit_depth=4)
            img = read_png(path)
            self.assertEqual((img.width, img.height, img.color_type), (5, 2, 3))
            self.assertEqual(img.rows, pixels)
            self.assertEqual(img.palette[:16], palette)

    def test_rgb_png_round_trip(self):
        rows = [[(10, 20, 30), (40, 50, 60)], [(70, 80, 90), (100, 110, 120)]]
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "rgb.png")
            write_rgb_png(path, 2, 2, rows)
            img = read_png(path)
            self.assertEqual(img.color_type, 2)
            self.assertEqual(img.to_rgb_rows(), rows)


class GbaTileCodecTests(unittest.TestCase):
    def test_4bpp_round_trip(self):
        tile = [[i % 16 for i in range(8)] for _ in range(8)]
        data = encode_tiles_4bpp([tile])
        self.assertEqual(len(data), 32)
        decoded = decode_tiles_4bpp(data)
        self.assertEqual(decoded, [tile])

    def test_grid_slicing_round_trip(self):
        tiles = [[[(t * 8 + r) % 16 for _ in range(8)] for r in range(8)] for t in range(6)]
        grid = tiles_to_indexed_grid(tiles, tiles_per_row=3)
        self.assertEqual(len(grid), 16)  # 2 rows of tiles * 8px
        self.assertEqual(len(grid[0]), 24)  # 3 tiles/row * 8px
        recovered = tiles_from_indexed_grid(grid, 24, 16)
        self.assertEqual(recovered, tiles)


def _make_metatile_word(tile_id, xflip=False, yflip=False, pal=0):
    return (tile_id & 0x3FF) | ((1 if xflip else 0) << 10) | ((1 if yflip else 0) << 11) | ((pal & 0xF) << 12)


class MetatileTests(unittest.TestCase):
    def test_parse_and_compose(self):
        # One metatile: bottom layer all tile 1 (pal 0), top layer all tile 0
        # (which must render as transparent, letting the bottom show through).
        words = [_make_metatile_word(1)] * 4 + [_make_metatile_word(0)] * 4
        data = struct.pack(f"<{len(words)}H", *words)
        metatiles = parse_metatiles(data)
        self.assertEqual(len(metatiles), 1)

        solid_tile = [[5] * 8 for _ in range(8)]  # every pixel = palette index 5
        blank_tile = [[0] * 8 for _ in range(8)]  # index 0 = transparent on top layer
        primary_tiles = [blank_tile, solid_tile]  # id 0, id 1

        grid = compose_metatile_indexed(metatiles[0], primary_tiles, [], num_tiles_primary=512)
        self.assertEqual(len(grid), 16)
        self.assertEqual(len(grid[0]), 16)
        # bottom layer (tile id 1, pal 0) should show through everywhere,
        # since the top layer is tile id 0 (transparent)
        for row in grid:
            for combined_idx in row:
                self.assertEqual(combined_idx, 0 * 16 + 5)

    def test_map_blocks(self):
        # 2x1 map: block0 metatile 3, collision 1, elevation 2 ; block1 metatile 0
        w0 = 3 | (1 << 10) | (2 << 12)
        w1 = 0
        data = struct.pack("<2H", w0, w1)
        grid = parse_map_blocks(data, width=2, height=1)
        self.assertEqual(grid[0][0].metatile_id, 3)
        self.assertEqual(grid[0][0].collision, 1)
        self.assertEqual(grid[0][0].elevation, 2)
        self.assertEqual(grid[0][1].metatile_id, 0)

    def test_combined_palette_padding(self):
        combined = build_combined_palette([[(1, 2, 3)] * 16])
        self.assertEqual(len(combined), 256)
        self.assertEqual(combined[0], (1, 2, 3))
        self.assertEqual(combined[16], (0, 0, 0))


class PaletteTests(unittest.TestCase):
    def test_jasc_round_trip(self):
        colors = [(0, 0, 0), (255, 255, 255), (128, 64, 32)]
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "p.pal")
            write_jasc_pal(path, colors, pad_to=4)
            back = read_jasc_pal(path)
            self.assertEqual(back[:3], colors)
            self.assertEqual(len(back), 4)

    def test_median_cut_bounds_and_nearest(self):
        colors = [(r, g, b) for r in (0, 50, 100, 150, 200, 250) for g in (0, 128, 255) for b in (0, 255)]
        reduced = median_cut_quantize(colors, max_colors=16)
        self.assertLessEqual(len(reduced), 16)
        idx = nearest_index((250, 128, 255), reduced)
        self.assertTrue(0 <= idx < len(reduced))


class LedgerEnforcementTests(unittest.TestCase):
    def test_require_import_args_blocks(self):
        with self.assertRaises(SystemExit):
            sources_ledger.require_import_args(None, "CC0", "foret")
        with self.assertRaises(SystemExit):
            sources_ledger.require_import_args("https://x", "", "foret")
        sources_ledger.require_import_args("https://x", "CC0", "foret")  # should not raise

    def test_ledger_round_trip_and_untracked_detection(self):
        with tempfile.TemporaryDirectory() as d:
            ledger = os.path.join(d, "ASSETS_SOURCES.md")
            graphics = os.path.join(d, "graphics")
            os.makedirs(graphics)
            tracked_path = os.path.join(graphics, "tracked.png")
            untracked_path = os.path.join(graphics, "untracked.png")
            open(tracked_path, "wb").close()
            open(untracked_path, "wb").close()

            entry = sources_ledger.LedgerEntry(
                date_str="2026-09-13",
                keyword="foret",
                source="https://example.invalid/pack",
                license="CC0",
                dest_paths=[os.path.normpath(tracked_path)],
                sha256="deadbeef",
            )
            sources_ledger.append_entry(ledger, entry)

            entries = sources_ledger.read_entries(ledger)
            self.assertEqual(len(entries), 1)
            self.assertEqual(entries[0].keyword, "foret")
            self.assertEqual(entries[0].license, "CC0")

            untracked = sources_ledger.find_untracked_assets(graphics, ledger)
            self.assertEqual([os.path.normpath(p) for p in untracked], [os.path.normpath(untracked_path)])


class ImporterGateTests(unittest.TestCase):
    def _make_source_png(self, path):
        # 8x8 RGB image, two colors, so quantization is trivial and predictable.
        rows = [[(200, 30, 30) if (x + y) % 2 == 0 else (30, 30, 200) for x in range(8)] for y in range(8)]
        write_rgb_png(path, 8, 8, rows)

    def test_import_without_args_writes_nothing(self):
        with tempfile.TemporaryDirectory() as d:
            src = os.path.join(d, "src.png")
            self._make_source_png(src)
            ledger = os.path.join(d, "ASSETS_SOURCES.md")
            dest_dir = os.path.join(d, "out")
            with self.assertRaises(SystemExit):
                importer.import_tile(
                    src_png_path=src,
                    dest_dir=dest_dir,
                    name="route_test",
                    source="",
                    license_="",
                    keyword="",
                    ledger_path=ledger,
                )
            self.assertFalse(os.path.exists(ledger))
            self.assertFalse(os.path.exists(dest_dir))

    def test_import_with_args_writes_ledger_and_files(self):
        with tempfile.TemporaryDirectory() as d:
            src = os.path.join(d, "src.png")
            self._make_source_png(src)
            ledger = os.path.join(d, "ASSETS_SOURCES.md")
            dest_dir = os.path.join(d, "out")

            result = importer.import_tile(
                src_png_path=src,
                dest_dir=dest_dir,
                name="route_test",
                source="https://opengameart.org/content/example",
                license_="CC0",
                keyword="route forestiere",
                ledger_path=ledger,
            )
            self.assertTrue(os.path.exists(ledger))
            self.assertTrue(os.path.exists(result.dest_png))
            self.assertTrue(os.path.exists(result.dest_pal))
            self.assertLessEqual(result.num_colors, 16)

            entries = sources_ledger.read_entries(ledger)
            self.assertEqual(len(entries), 1)
            self.assertEqual(entries[0].source, "https://opengameart.org/content/example")
            self.assertEqual(entries[0].sha256, sources_ledger.sha256_of_file(src))

            # Round trip the written PNG back through our own reader.
            img = read_png(result.dest_png)
            self.assertEqual(img.color_type, 3)
            self.assertEqual((img.width, img.height), (8, 8))

    def test_import_rejects_non_multiple_of_8(self):
        with tempfile.TemporaryDirectory() as d:
            src = os.path.join(d, "src.png")
            write_rgb_png(src, 5, 8, [[(0, 0, 0)] * 5 for _ in range(8)])
            with self.assertRaises(ValueError):
                importer.convert_to_project_format(src)


class SearchTests(unittest.TestCase):
    def test_french_keyword_matches_forest_tags(self):
        results = search.search("route forestière")
        self.assertTrue(len(results) > 0)
        ids = [r.entry["id"] for r in results]
        self.assertIn("oga-forest-tileset-sample", ids)

    def test_french_keyword_matches_interior_tags(self):
        results = search.search("intérieur pokécenter")
        self.assertTrue(len(results) > 0)
        ids = [r.entry["id"] for r in results]
        self.assertTrue(any("indoor" in r.entry["tags"] or "interior" in r.entry["tags"] for r in results))
        self.assertIn("oga-16x16-indoor-rpg-tileset", ids)

    def test_unmatched_keyword_returns_empty(self):
        results = search.search("zzz_totalement_hors_sujet_zzz")
        self.assertEqual(results, [])


class FallbackGenTests(unittest.TestCase):
    def test_deterministic_with_seed(self):
        with tempfile.TemporaryDirectory() as d:
            ref = os.path.join(d, "ref.png")
            rows = [[(10, 10, 10) if (x + y) % 3 else (200, 180, 50) for x in range(16)] for y in range(16)]
            write_rgb_png(ref, 16, 16, rows)

            tile_a = fallback_gen.generate_fallback_tile(ref, seed=42)
            tile_b = fallback_gen.generate_fallback_tile(ref, seed=42)
            self.assertEqual(tile_a, tile_b)
            self.assertEqual(len(tile_a), 8)
            self.assertEqual(len(tile_a[0]), 8)


def _make_tileset_dir(base, name, num_tiles, palette_colors_by_index, metatile_words):
    """Build a minimal on-disk tileset directory (tiles.png + palettes/*.pal
    + metatiles.bin) matching the real project's layout, for CLI-level tests."""
    d = os.path.join(base, name)
    os.makedirs(os.path.join(d, "palettes"))
    tiles = [[[i % 16] * 8 for _ in range(8)] for i in range(num_tiles)]
    grid = tiles_to_indexed_grid(tiles, tiles_per_row=num_tiles)
    palette16 = [(0, 0, 0)] * 16
    write_indexed_png(os.path.join(d, "tiles.png"), len(grid[0]), len(grid), grid, palette16)
    for idx, color in palette_colors_by_index.items():
        write_jasc_pal(os.path.join(d, "palettes", f"{idx:02d}.pal"), [color] * 16, pad_to=16)
    words = []
    for w in metatile_words:
        words.extend(w)
    with open(os.path.join(d, "metatiles.bin"), "wb") as f:
        f.write(struct.pack(f"<{len(words)}H", *words))
    return d


class CliPrimarySecondaryResolutionTests(unittest.TestCase):
    """Regression coverage for the two real-project bugs described in the
    module docstring: wrong secondary-palette slice, and missing secondary
    metatiles.bin concatenation."""

    def test_secondary_palette_uses_its_own_high_slots_not_slot_zero(self):
        with tempfile.TemporaryDirectory() as d:
            junk = (1, 1, 1)
            real_a, real_b = (10, 20, 30), (40, 50, 60)
            primary_dir = _make_tileset_dir(
                d, "primary", num_tiles=1,
                palette_colors_by_index={0: (100, 0, 0), 1: (0, 100, 0)},
                metatile_words=[[_make_metatile_word(0)] * 8],
            )
            secondary_dir = _make_tileset_dir(
                d, "secondary", num_tiles=1,
                # files 0/1 are the "not mine" placeholders a real secondary
                # tileset folder carries; files 2/3 are its real, owned slots.
                palette_colors_by_index={0: junk, 1: junk, 2: real_a, 3: real_b},
                metatile_words=[[_make_metatile_word(0)] * 8],
            )
            args = SimpleNamespace(
                primary_dir=primary_dir,
                secondary_dir=secondary_dir,
                num_pals_primary=2,
                num_pals_total=4,
                metatiles=None,
            )
            _p, _s, combined_palette, _mt, _smt = cli._resolve_tiles_and_palettes(args)
            self.assertEqual(combined_palette[2 * 16], real_a)
            self.assertEqual(combined_palette[3 * 16], real_b)
            self.assertNotIn(junk, (combined_palette[2 * 16], combined_palette[3 * 16]))

    def test_render_map_concatenates_secondary_metatiles(self):
        with tempfile.TemporaryDirectory() as d:
            primary_dir = _make_tileset_dir(
                d, "primary", num_tiles=1,
                palette_colors_by_index={0: (1, 2, 3)},
                metatile_words=[[_make_metatile_word(0, pal=0)] * 8],  # metatile id 0
            )
            secondary_dir = _make_tileset_dir(
                d, "secondary", num_tiles=1,
                # num_pals_primary=1 below, so global slot 1 is secondary's
                # own file 01.pal (its file 00.pal would be the unused one).
                palette_colors_by_index={0: (0, 0, 0), 1: (4, 5, 6)},
                metatile_words=[[_make_metatile_word(0, pal=1)] * 8],  # metatile id 1 (primary has 1 metatile)
            )
            args = SimpleNamespace(
                primary_dir=primary_dir,
                secondary_dir=secondary_dir,
                num_pals_primary=1,
                num_pals_total=2,
                metatiles=None,
            )
            _p, _s, _pal, metatiles_path, secondary_metatiles_path = cli._resolve_tiles_and_palettes(args)
            self.assertIsNotNone(secondary_metatiles_path)
            metatiles = cli._load_metatiles_combined(metatiles_path, secondary_metatiles_path)
            self.assertEqual(len(metatiles), 2)
            self.assertEqual(metatiles[0].bottom[0].pal_num, 0)  # from primary's own file
            self.assertEqual(metatiles[1].bottom[0].pal_num, 1)  # from secondary's own file, at global id 1


if __name__ == "__main__":
    unittest.main()
