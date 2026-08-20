"""Demo d'une "grande ville" Kanto avec le tileset secondaire viridian_city_frlg (suite 35),
pour montrer la richesse disponible avant de choisir quelle ville industrialiser en premier parmi
les 13 restantes. Carte isolee (MAP_VIRIDIAN_CITY_KANTO_DEMO, groupe 76), non connectee au monde,
accessible uniquement par le menu debug - aucune ville deja livree n'est touchee.

Batiments (PC/MART/GYM_STYLE) inchanges : ce sont des tuiles general_frlg PRIMAIRES, donc valables
quel que soit le tileset secondaire de la carte. Seul le decor (arbres, haies, falaises) vient du
nouveau secondaire viridian_city_frlg - voir tile_catalog.VIRIDIAN_DECOR pour le detail/verification.
"""
import random
import sys

from map_builder import MapGrid, PASSABLE, IMPASSABLE
from tile_catalog import TERRAIN, VIRIDIAN_DECOR

random.seed(11)
W, H = 32, 28
g = MapGrid(W, H, fill=TERRAIN["grass"])

# --- 1. Relief : falaise rocheuse Viridian en fond nord/ouest (montagne bordant la ville) ---
g.rect(0, 0, W, 3, VIRIDIAN_DECOR["cliff_face"], collision=IMPASSABLE)
g.rect(0, 3, 4, H - 3, VIRIDIAN_DECOR["cliff_face"], collision=IMPASSABLE)

# --- 2. Eau (petit etang decoratif au sud-est, pas une riviere traversante ici) ---
g.blob(27, 23, 3, TERRAIN["water"])
g.blob(6, 24, 2, TERRAIN["sand"])

# --- 3. Routes (avant les batiments) ---
g.vline(4, 24, 16, TERRAIN["path"])
g.hline(6, 26, 10, TERRAIN["path"])
g.hline(8, 24, 18, TERRAIN["path"])
g.vline(10, 18, 24, TERRAIN["path"])

# --- 4. Batiments (archetypes deja valides, tuiles general_frlg primaires) ---
buildings = []
buildings.append(("centre", g.build_pc_style(12, 5, "centre")))
buildings.append(("arene", g.build_gym_style(20, 4, "arene")))
buildings.append(("mart", g.build_mart_style(7, 12, "mart")))
buildings.append(("maison1", g.build_pc_style(20, 12, "maison1")))
buildings.append(("maison2", g.build_gym_style(13, 18, "maison2")))

doors = {name: pos for name, pos in buildings}
print("doors:", doors)


def connect_door_to_path(door_x, door_y, target_x, target_y):
    front_x, front_y = door_x, door_y + 1
    g.put(front_x, front_y, TERRAIN["path"], PASSABLE)
    g.hline(min(front_x, target_x), max(front_x, target_x), front_y, TERRAIN["path"])
    g.vline(min(front_y, target_y), max(front_y, target_y), target_x, TERRAIN["path"])


connect_door_to_path(*doors["centre"], 16, 8)
connect_door_to_path(*doors["arene"], 24, 8)
connect_door_to_path(*doors["mart"], 8, 10)
connect_door_to_path(*doors["maison1"], 24, 12)
connect_door_to_path(*doors["maison2"], 16, 18)

# --- 5. Vegetation Viridian (groupee) : arbres denses + haies en bordure de route ---
for (cx, cy, r) in [(27, 6, 3), (28, 15, 3), (26, 21, 2), (9, 22, 2)]:
    g.tree_blob(cx, cy, r, TERRAIN["tree_bush"])
for (cx, cy, r) in [(29, 2, 2), (30, 10, 2), (2, 20, 2)]:
    g.tree_blob(cx, cy, r, VIRIDIAN_DECOR["tree_big"])
# Segments de haie (PAS de bande continue) : on evite explicitement les colonnes occupees par un
# batiment a cette rangee (piege deja rencontre sur Bourg Palette : un decor pose apres les
# batiments peut corrompre leur facade s'il chevauche leur empreinte - cf. TECHNICAL_ARCHITECTURE.md).
g.hline(6, 11, 9, VIRIDIAN_DECOR["hedge_row"], collision=IMPASSABLE)   # evite centre (x12-15)
g.hline(16, 26, 9, VIRIDIAN_DECOR["hedge_row"], collision=IMPASSABLE)
g.hline(6, 12, 19, VIRIDIAN_DECOR["hedge_row"], collision=IMPASSABLE)  # evite maison2 (x13-16)
g.hline(17, 26, 19, VIRIDIAN_DECOR["hedge_row"], collision=IMPASSABLE)
# filet de securite : rouvre les cases de porte/chemin devant si un decor les a recouvertes
for name, (dx, dy) in doors.items():
    g.put(dx, dy + 1, TERRAIN["path"], PASSABLE)
g.rect(5, 6, 4, 2, TERRAIN["tall_grass"])
g.rect(22, 20, 3, 2, TERRAIN["tall_grass"])
g.put(10, 7, TERRAIN["flower"])
g.put(25, 11, TERRAIN["flower"])

# --- 6. Validation ---
errors = g.validate()
if errors:
    print(f"[!] {len(errors)} erreur(s) de validation, correction automatique :")
    for e in errors:
        print("   -", e)
    for name, (dx, dy) in doors.items():
        g.ensure_door_path(dx, dy)
    errors = g.validate()
    if errors:
        print("[!] ERREURS PERSISTANTES apres correction automatique :")
        for e in errors:
            print("   -", e)
        sys.exit(1)
    else:
        print("[ok] toutes les erreurs corrigees automatiquement")
else:
    print("[ok] validation propre : chaque porte est desservie par un chemin, aucune porte infranchissable")

# --- 7. Ecriture ---
with open("/home/user/test-gba/engine/data/layouts/ViridianCityKantoDemo/map.bin", "wb") as f:
    f.write(g.to_bin())

import struct
border = bytearray()
for _ in range(4):
    val = TERRAIN["grass"] | (3 << 12)
    border += struct.pack("<H", val)
with open("/home/user/test-gba/engine/data/layouts/ViridianCityKantoDemo/border.bin", "wb") as f:
    f.write(border)

print(f"map.bin: {g.w*g.h*2} bytes ({g.w}x{g.h})")
