import sys
import struct
from PIL import Image
from lz77 import try_decompress

ROM_PATH = sys.argv[1]
TILE_OFFSET = int(sys.argv[2], 16)
TILE_SIZE = int(sys.argv[3])
PAL_OFFSET = int(sys.argv[4], 16)
OUT_PATH = sys.argv[5]
N_PALETTES = int(sys.argv[6]) if len(sys.argv) > 6 else 1

with open(ROM_PATH, "rb") as f:
    rom = f.read()

data = try_decompress(rom, TILE_OFFSET, max_size=TILE_SIZE)
assert data is not None and len(data) == TILE_SIZE, "decompression echouee ou taille inattendue"


def bgr555_to_rgb(color):
    r = (color & 0x1F) * 8
    g = ((color >> 5) & 0x1F) * 8
    b = ((color >> 10) & 0x1F) * 8
    return (r, g, b)


palettes = []
for p in range(N_PALETTES):
    raw = rom[PAL_OFFSET + p * 32: PAL_OFFSET + p * 32 + 32]
    colors = struct.unpack("<16H", raw)
    palettes.append([bgr555_to_rgb(c) for c in colors])


def decode_4bpp_tile(raw):
    px = [[0] * 8 for _ in range(8)]
    for row in range(8):
        for col_pair in range(4):
            byte = raw[row * 4 + col_pair]
            px[row][col_pair * 2] = byte & 0xF
            px[row][col_pair * 2 + 1] = (byte >> 4) & 0xF
    return px


tiles_per_block = TILE_SIZE // 32
cols = 32
rows = (tiles_per_block + cols - 1) // cols
pal = palettes[0]

img = Image.new("RGB", (cols * 8, rows * 8), pal[0])
for t in range(tiles_per_block):
    raw = data[t * 32:(t + 1) * 32]
    px = decode_4bpp_tile(raw)
    tx = (t % cols) * 8
    ty = (t // cols) * 8
    for yy in range(8):
        for xx in range(8):
            img.putpixel((tx + xx, ty + yy), pal[px[yy][xx]])

img = img.resize((img.width * 3, img.height * 3), Image.NEAREST)
img.save(OUT_PATH)
print("saved", OUT_PATH)
