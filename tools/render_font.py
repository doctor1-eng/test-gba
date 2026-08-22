#!/usr/bin/env python3
"""Decompress a candidate LZ77 font block and render it as a PNG grid
of 8x16 4bpp tiles (16 columns x 16 rows = 256 glyphs), using the
Pokemon Gen3 default text palette (white on transparent/black bg with
dark outline). Read-only against the ROM."""
import sys
sys.path.insert(0, "tools")
from find_font import lz77_decompress
from PIL import Image

# Standard Gen3 font palette approximation: index0=transparent,
# index1=black outline, index2=white fill, index3=light gray shadow.
PALETTE = [
    (0, 0, 0, 0),
    (40, 40, 40, 255),
    (255, 255, 255, 255),
    (160, 160, 160, 255),
] + [(200, 200, 200, 255)] * 12


def render(rom_path, offset, out_path, cols=16):
    with open(rom_path, "rb") as f:
        data = f.read()
    decompressed, consumed = lz77_decompress(data, offset)
    n_glyphs = len(decompressed) // 64
    rows = (n_glyphs + cols - 1) // cols
    img = Image.new("RGBA", (cols * 8, rows * 16), (30, 30, 30, 255))
    for g in range(n_glyphs):
        gx = (g % cols) * 8
        gy = (g // cols) * 16
        glyph = decompressed[g*64:(g+1)*64]
        # two stacked 8x8 4bpp tiles
        for t in range(2):
            tile = glyph[t*32:(t+1)*32]
            for row in range(8):
                rowbytes = tile[row*4:(row+1)*4]
                for b_i, byte in enumerate(rowbytes):
                    px0 = byte & 0xF
                    px1 = (byte >> 4) & 0xF
                    x = gx + b_i * 2
                    y = gy + t * 8 + row
                    img.putpixel((x, y), PALETTE[px0 % len(PALETTE)])
                    img.putpixel((x + 1, y), PALETTE[px1 % len(PALETTE)])
    img = img.resize((img.width * 3, img.height * 3), Image.NEAREST)
    img.save(out_path)
    print(f"saved {out_path}: {n_glyphs} glyphs, {consumed} compressed bytes consumed")


if __name__ == "__main__":
    rom_path = sys.argv[1]
    offset = int(sys.argv[2], 16)
    out_path = sys.argv[3]
    render(rom_path, offset, out_path)
