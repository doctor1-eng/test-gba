"""Scanne toute la ROM à la recherche de blocs LZ77 valides (voir lz77.py), et classe les
candidats plausibles : blocs "graphismes" (taille multiple de 32 octets = un tile 4bpp 8x8) et
blocs "palette" (taille multiple de 32 octets aussi, mais dans la plage 32-512 = 1 à 16 palettes
de 16 couleurs). Écrit les résultats en JSON pour inspection ultérieure - ne décide de rien tout
seul, juste réduit ~16 Mo de ROM à une liste de candidats qu'on peut ensuite rendre en image.
"""
import sys
import json
import time
from lz77 import try_decompress

ROM_PATH = sys.argv[1] if len(sys.argv) > 1 else None
if not ROM_PATH:
    print("Usage: scan_rom.py <rom.gba> [out.json]")
    sys.exit(1)
OUT_PATH = sys.argv[2] if len(sys.argv) > 2 else "candidates.json"

with open(ROM_PATH, "rb") as f:
    data = f.read()

print(f"ROM: {len(data)} octets")
t0 = time.time()

candidates = []
n = len(data)
checked = 0
i = 0
while i < n - 4:
    if data[i] == 0x10:
        checked += 1
        size = data[i + 1] | (data[i + 2] << 8) | (data[i + 3] << 16)
        # Rejet rapide avant meme de tenter la decompression complete : taille nulle, absurde,
        # ou non multiple de 32 (aucun tileset/palette GBA n'a une taille decompressee arbitraire).
        if 32 <= size <= 200_000 and size % 32 == 0:
            result = try_decompress(data, i, max_size=200_000)
            if result is not None and len(result) == size:
                candidates.append({"offset": i, "size": size})
    i += 1
    if i % 2_000_000 == 0:
        print(f"  ... {i}/{n} ({time.time()-t0:.1f}s, {len(candidates)} candidats)")

print(f"Termine en {time.time()-t0:.1f}s : {checked} en-tetes 0x10 vus, {len(candidates)} blocs LZ77 valides")

with open(OUT_PATH, "w") as f:
    json.dump(candidates, f)
print(f"-> {OUT_PATH}")
