#!/usr/bin/env python3
"""Generateur de previews PNG fideles des maps, a partir des vraies tuiles sources
(engine/data/tilesets/), sans passer par un editeur graphique (Porymap n'a pas de
mode CLI/batch - voir docs/PROJECT_STATUS.md, suite 31).

Lit directement les memes fichiers source que le moteur GBA (tiles.png, metatiles.bin,
palettes/*.pal, layouts.json, map.json) et reproduit l'algorithme de composition des
metatiles (2 couches de 2x2 tuiles de 8x8px) tel qu'implemente dans engine/src/fieldmap.c
et engine/include/global.fieldmap.h.

Usage :
    python3 map_preview.py <NomDeMap> [<NomDeMap2> ...]
    python3 map_preview.py --all
    python3 map_preview.py --list
    python3 map_preview.py --tileset <TilesetPrimaire> <TilesetSecondaire> [emerald|frlg]

Le mode --tileset affiche en planche toutes les metatiles d'une paire de tilesets (utile
pour previsualiser un tileset FRLG comme pallet_town_frlg avant de l'utiliser dans une map,
sans avoir a d'abord construire une carte complete). Version par defaut : frlg pour un
tileset secondaire dont le nom finit par "Frlg", emerald sinon.

Les PNG sont ecrits dans tools/map_preview/out/<NomDeMap>.png (ou out/tileset_<...>.png)
"""
import json
import re
import struct
import sys
from pathlib import Path

ENGINE_DIR = Path(__file__).resolve().parents[2] / "engine"
MAPS_DIR = ENGINE_DIR / "data" / "maps"
LAYOUTS_JSON = ENGINE_DIR / "data" / "layouts" / "layouts.json"
TILESETS_GRAPHICS_SOURCES = [
    ENGINE_DIR / "src" / "data" / "tilesets" / "graphics.h",
    ENGINE_DIR / "src" / "graphics.c",
    ENGINE_DIR / "src" / "data" / "tilesets" / "graphics_kanto.h",
]
OUT_DIR = Path(__file__).resolve().parent / "out"

TILE_PX = 8
METATILE_PX = 16

# engine/include/fieldmap.h : ces constantes different entre les layouts "emerald" et "frlg"
# (une carte FRLG a un tileset primaire plus grand : 640 tuiles/metatiles et 7 palettes au
# lieu de 512/512/6 pour Emerald). Le champ "layout_version" du layout indique lequel utiliser.
SPLIT_CONSTANTS = {
    "emerald": dict(num_tiles_primary=512, num_metatiles_primary=512, num_pals_primary=6, num_pals_total=13),
    "frlg": dict(num_tiles_primary=640, num_metatiles_primary=640, num_pals_primary=7, num_pals_total=13),
}

MAPGRID_METATILE_ID_MASK = 0x03FF


def load_symbol_to_folder():
    """Parse the tileset graphics sources to map gTilesetTiles_<Name> -> its source folder."""
    mapping = {}
    for src in TILESETS_GRAPHICS_SOURCES:
        text = src.read_text()
        for m in re.finditer(
            r'const u32 gTilesetTiles_(\w+)\[\]\s*=\s*INCGFX_U32\("(data/tilesets/[^"]+)/tiles\.png"',
            text,
        ):
            name, folder = m.group(1), m.group(2)
            mapping[name] = ENGINE_DIR / folder
    return mapping


SYMBOL_TO_FOLDER = load_symbol_to_folder()


def resolve_tileset_dir(symbol):
    """'gTileset_General' -> Path to engine/data/tilesets/primary/general"""
    name = symbol.replace("gTileset_", "")
    if name not in SYMBOL_TO_FOLDER:
        raise KeyError(f"Tileset inconnu (absent de graphics.h) : {symbol}")
    return SYMBOL_TO_FOLDER[name]


def read_jasc_pal(path):
    lines = path.read_text().splitlines()
    # lines[0] = "JASC-PAL", lines[1] = "0100", lines[2] = count
    count = int(lines[2])
    colors = []
    for i in range(count):
        r, g, b = map(int, lines[3 + i].split())
        colors.append((r, g, b))
    return colors


class Tileset:
    """Charge tiles.png + metatiles.bin + palettes/ d'un tileset (primaire ou secondaire)."""

    def __init__(self, symbol):
        from PIL import Image

        self.symbol = symbol
        self.dir = resolve_tileset_dir(symbol)

        img = Image.open(self.dir / "tiles.png").convert("P")
        w, h = img.size
        self.tiles_wide = w // TILE_PX
        pixels = img.load()
        self.tile_indices = {}  # local tile id -> 8x8 list of palette indices (0-15)
        n_tiles = (w // TILE_PX) * (h // TILE_PX)
        for tid in range(n_tiles):
            tx = (tid % self.tiles_wide) * TILE_PX
            ty = (tid // self.tiles_wide) * TILE_PX
            grid = [[pixels[tx + x, ty + y] & 0xF for x in range(TILE_PX)] for y in range(TILE_PX)]
            self.tile_indices[tid] = grid

        self.metatiles = self._load_metatiles()
        self.palettes = [
            read_jasc_pal(self.dir / "palettes" / f"{i:02d}.pal") for i in range(16)
        ]

    def _load_metatiles(self):
        data = (self.dir / "metatiles.bin").read_bytes()
        bytes_per_metatile = 2 * 8  # 8 tiles (2 layers x 2x2), u16 each
        n = len(data) // bytes_per_metatile
        metatiles = []
        for i in range(n):
            raw = struct.unpack_from("<8H", data, i * bytes_per_metatile)
            tiles = []
            for v in raw:
                tile_id = v & 0x03FF
                xflip = bool(v & 0x0400)
                yflip = bool(v & 0x0800)
                palette = (v & 0xF000) >> 12
                tiles.append((tile_id, xflip, yflip, palette))
            metatiles.append(tiles)
        return metatiles


class MapRenderer:
    def __init__(self):
        self._tileset_cache = {}

    def get_tileset(self, symbol):
        if symbol not in self._tileset_cache:
            self._tileset_cache[symbol] = Tileset(symbol)
        return self._tileset_cache[symbol]

    def get_tile_pixels(self, primary, secondary, tile_id, split):
        if tile_id < split["num_tiles_primary"]:
            return primary.tile_indices.get(tile_id)
        return secondary.tile_indices.get(tile_id - split["num_tiles_primary"])

    def get_palette_rgb(self, primary, secondary, pal_num, split):
        if pal_num < split["num_pals_primary"]:
            return primary.palettes[pal_num]
        return secondary.palettes[pal_num]

    def get_metatile(self, primary, secondary, metatile_id, split):
        if metatile_id < split["num_metatiles_primary"]:
            return primary.metatiles[metatile_id] if metatile_id < len(primary.metatiles) else None
        idx = metatile_id - split["num_metatiles_primary"]
        return secondary.metatiles[idx] if idx < len(secondary.metatiles) else None

    def render_metatile_image(self, primary, secondary, metatile_id, split):
        from PIL import Image

        canvas = Image.new("RGBA", (METATILE_PX, METATILE_PX), (0, 0, 0, 0))
        tiles = self.get_metatile(primary, secondary, metatile_id, split)
        if tiles is None:
            # Metatile inconnu/hors limites : damier magenta pour le signaler visuellement.
            px = canvas.load()
            for y in range(METATILE_PX):
                for x in range(METATILE_PX):
                    if (x // 4 + y // 4) % 2 == 0:
                        px[x, y] = (255, 0, 255, 255)
            return canvas

        # tiles[0..3] = couche du bas (TL,TR,BL,BR), tiles[4..7] = couche du haut.
        for layer in (0, 1):
            layer_tiles = tiles[layer * 4 : layer * 4 + 4]
            for slot, (tile_id, xflip, yflip, pal_num) in enumerate(layer_tiles):
                grid = self.get_tile_pixels(primary, secondary, tile_id, split)
                if grid is None:
                    continue
                colors = self.get_palette_rgb(primary, secondary, pal_num, split)
                ox = (slot % 2) * TILE_PX
                oy = (slot // 2) * TILE_PX
                for y in range(TILE_PX):
                    sy = (TILE_PX - 1 - y) if yflip else y
                    for x in range(TILE_PX):
                        sx = (TILE_PX - 1 - x) if xflip else x
                        idx = grid[sy][sx]
                        if idx == 0:
                            continue  # index de palette 0 = transparent (fond GBA)
                        r, g, b = colors[idx]
                        canvas.putpixel((ox + x, oy + y), (r, g, b, 255))
        return canvas


def load_layouts():
    data = json.loads(LAYOUTS_JSON.read_text())
    return {l["id"]: l for l in data["layouts"]}


def render_map(map_name, renderer, layouts, metatile_cache):
    from PIL import Image

    map_json_path = MAPS_DIR / map_name / "map.json"
    if not map_json_path.exists():
        print(f"[!] Map introuvable : {map_name}", file=sys.stderr)
        return None

    map_json = json.loads(map_json_path.read_text())
    layout_id = map_json["layout"]
    layout = layouts.get(layout_id)
    if not layout:
        print(f"[!] Layout introuvable pour {map_name} ({layout_id})", file=sys.stderr)
        return None

    width, height = layout["width"], layout["height"]
    primary = renderer.get_tileset(layout["primary_tileset"])
    secondary = renderer.get_tileset(layout["secondary_tileset"])
    # "metatile_format": "frlg" (suite 32) fait passer isFrlg=TRUE cote moteur (mapjson.cpp) meme
    # pour un layout_version "emerald" - donc la meme frontiere 640/640/7 doit etre utilisee ici.
    split_key = layout.get("metatile_format") or layout.get("layout_version", "emerald")
    split = SPLIT_CONSTANTS[split_key]

    blockdata_path = ENGINE_DIR / layout["blockdata_filepath"]
    raw = blockdata_path.read_bytes()
    cells = struct.unpack(f"<{len(raw)//2}H", raw)
    if len(cells) < width * height:
        print(f"[!] {map_name}: map.bin trop court ({len(cells)} < {width*height})", file=sys.stderr)

    out_img = Image.new("RGBA", (width * METATILE_PX, height * METATILE_PX), (32, 32, 32, 255))

    cache_key = (layout["primary_tileset"], layout["secondary_tileset"], split_key)
    local_cache = metatile_cache.setdefault(cache_key, {})

    for i in range(min(len(cells), width * height)):
        x, y = i % width, i // width
        metatile_id = cells[i] & MAPGRID_METATILE_ID_MASK
        if metatile_id not in local_cache:
            local_cache[metatile_id] = renderer.render_metatile_image(primary, secondary, metatile_id, split)
        out_img.paste(local_cache[metatile_id], (x * METATILE_PX, y * METATILE_PX))

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    out_path = OUT_DIR / f"{map_name}.png"
    out_img.save(out_path)
    print(f"[ok] {map_name} -> {out_path} ({width}x{height} metatiles, {out_img.size[0]}x{out_img.size[1]}px)")
    return out_path


def render_tileset_swatch(primary_symbol, secondary_symbol, version, renderer):
    """Affiche en planche toutes les metatiles d'une paire de tilesets, sans carte reelle.
    Utile pour previsualiser un tileset FRLG dormant (ex: pallet_town_frlg) avant de l'utiliser."""
    from PIL import Image

    primary = renderer.get_tileset(primary_symbol)
    secondary = renderer.get_tileset(secondary_symbol)
    split = SPLIT_CONSTANTS[version]

    total_metatiles = len(primary.metatiles) + len(secondary.metatiles)
    cols = 16
    rows = (total_metatiles + cols - 1) // cols
    out_img = Image.new("RGBA", (cols * METATILE_PX, rows * METATILE_PX), (32, 32, 32, 255))

    for metatile_id in range(total_metatiles):
        img = renderer.render_metatile_image(primary, secondary, metatile_id, split)
        x, y = metatile_id % cols, metatile_id // cols
        out_img.paste(img, (x * METATILE_PX, y * METATILE_PX))

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    p_name = primary_symbol.replace("gTileset_", "")
    s_name = secondary_symbol.replace("gTileset_", "")
    out_path = OUT_DIR / f"tileset_{p_name}_{s_name}.png"
    out_img.save(out_path)
    print(f"[ok] tileset {primary_symbol}+{secondary_symbol} ({version}) -> {out_path} "
          f"({total_metatiles} metatiles, {out_img.size[0]}x{out_img.size[1]}px)")
    return out_path


def main():
    args = sys.argv[1:]
    layouts = load_layouts()
    renderer = MapRenderer()
    metatile_cache = {}

    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        return

    if args[0] == "--list":
        for name in sorted(p.name for p in MAPS_DIR.iterdir() if p.is_dir()):
            print(name)
        return

    if args[0] == "--tileset":
        if len(args) < 3:
            print("Usage: map_preview.py --tileset <TilesetPrimaire> <TilesetSecondaire> [emerald|frlg]", file=sys.stderr)
            sys.exit(1)
        primary_symbol, secondary_symbol = args[1], args[2]
        version = args[3] if len(args) > 3 else ("frlg" if secondary_symbol.endswith("Frlg") else "emerald")
        try:
            render_tileset_swatch(primary_symbol, secondary_symbol, version, renderer)
        except Exception as e:
            print(f"[!] {primary_symbol}+{secondary_symbol}: erreur - {e}", file=sys.stderr)
        return

    if args[0] == "--all":
        names = sorted(p.name for p in MAPS_DIR.iterdir() if p.is_dir())
    else:
        names = args

    for name in names:
        try:
            render_map(name, renderer, layouts, metatile_cache)
        except Exception as e:
            print(f"[!] {name}: erreur - {e}", file=sys.stderr)


if __name__ == "__main__":
    main()
