"""Rend en planche (avec une palette provisoire en niveaux de gris) les blocs candidats d'une
taille donnee, pour un premier tri visuel : ce script ne sait PAS quelle est la vraie palette
GBA associee a chaque bloc de tuiles (elle est stockee separement et referencee par un pointeur
ailleurs dans la ROM, pas retrouve ici) - le niveau de gris sert seulement a distinguer les
formes (mur, fenetre, toit) du bruit avant d'aller chercher la bonne palette pour les blocs
prometteurs.
"""
import sys
import json
from PIL import Image
from lz77 import try_decompress

ROM_PATH = sys.argv[1]
CANDIDATES_PATH = sys.argv[2]
TARGET_SIZE = int(sys.argv[3])
OUT_PATH = sys.argv[4]

with open(ROM_PATH, "rb") as f:
    rom = f.read()

candidates = json.load(open(CANDIDATES_PATH))
matches = [c for c in candidates if c["size"] == TARGET_SIZE]
print(f"{len(matches)} bloc(s) de taille {TARGET_SIZE} ({TARGET_SIZE // 32} tiles chacun)")

GRAY = [(i * 17, i * 17, i * 17) for i in range(16)]  # 16 niveaux de gris, index 0 = noir/transparent


def decode_4bpp_tile(raw):
    """32 octets -> grille 8x8 d'index de palette (4 bits/pixel, GBA little-endian par octet)."""
    px = [[0] * 8 for _ in range(8)]
    for row in range(8):
        for col_pair in range(4):
            byte = raw[row * 4 + col_pair]
            px[row][col_pair * 2] = byte & 0xF
            px[row][col_pair * 2 + 1] = (byte >> 4) & 0xF
    return px


TILE_PX = 8
cols = 32
rows_per_block = 8  # tiles empilees verticalement par bloc, avant de passer au bloc suivant a droite
gap = 1

n_blocks = len(matches)
if n_blocks == 0:
    print("Aucun bloc a ce format.")
    sys.exit(0)

tiles_per_block = TARGET_SIZE // 32
block_cols = cols
block_rows = (tiles_per_block + block_cols - 1) // block_cols

sheet_w = block_cols * TILE_PX
sheet_h = block_rows * TILE_PX
total_h = n_blocks * (sheet_h + gap * 4)

img = Image.new("RGB", (sheet_w, total_h), (255, 0, 255))

for bi, cand in enumerate(matches):
    data = try_decompress(rom, cand["offset"], max_size=TARGET_SIZE)
    if data is None or len(data) != TARGET_SIZE:
        continue
    y_base = bi * (sheet_h + gap * 4)
    for t in range(tiles_per_block):
        raw = data[t * 32:(t + 1) * 32]
        px = decode_4bpp_tile(raw)
        tx = (t % block_cols) * TILE_PX
        ty = y_base + (t // block_cols) * TILE_PX
        for yy in range(8):
            for xx in range(8):
                img.putpixel((tx + xx, ty + yy), GRAY[px[yy][xx]])

img.save(OUT_PATH)
print("saved", OUT_PATH, img.size)

# fichier d'index pour retrouver l'offset ROM de chaque bloc numerote dans l'image
with open(OUT_PATH + ".offsets.txt", "w") as f:
    for bi, cand in enumerate(matches):
        f.write(f"{bi}: offset=0x{cand['offset']:X} size={cand['size']}\n")
