"""Cherche des palettes GBA brutes (non compressées) dans toute la ROM : une vraie palette GBA
est un tableau de 16 couleurs 15-bit BGR555 (2 octets/couleur), et le bit de poids fort de chaque
couleur est TOUJOURS à 0 (format 0BBBBBGGGGGRRRRR). Sur des données aléatoires ce bit vaut 0 une
fois sur deux, donc exiger les 16 à la fois est un filtre très sélectif (~1/65536), largement
suffisant pour distinguer une vraie palette du bruit sans connaître son adresse à l'avance.
"""
import sys
import struct

ROM_PATH = sys.argv[1]
with open(ROM_PATH, "rb") as f:
    rom = f.read()

n = len(rom)
found = []
for off in range(0, n - 32, 2):
    ok = True
    for i in range(16):
        color = rom[off + i * 2] | (rom[off + i * 2 + 1] << 8)
        if color & 0x8000:
            ok = False
            break
    if ok:
        colors = struct.unpack("<16H", rom[off:off + 32])
        if len(set(colors)) > 3:  # rejette les blocs degeneres (tout a zero, etc.)
            found.append(off)

print(f"{len(found)} palettes candidates")
with open("palette_candidates.txt", "w") as f:
    for off in found:
        f.write(f"0x{off:X}\n")
