"""Genere data/layouts/LittlerootTown/{map.bin,border.bin} avec le pipeline structure
(relief -> eau -> routes -> batiments -> portes -> vegetation -> collision -> validation).
Remplace le script ad hoc de suite 33. Voir docs/TECHNICAL_ARCHITECTURE.md.
"""
import random
import sys

from map_builder import MapGrid, PASSABLE, IMPASSABLE
from tile_catalog import TERRAIN

random.seed(7)
W, H = 34, 26
g = MapGrid(W, H, fill=TERRAIN["grass"])

# --- 1. Limites / relief (collines a l'ouest, degrade doux) ---
import math
for y in range(H):
    depth = 1 + round(math.sin(y / 4) * 1.3 + 1.3)
    g.hill_row(y, depth)

# --- 2. Eau (ruisseau des collines vers la mer au sud + mer) ---
river_points = [[5, 4], [9, 7], [13, 6], [16, 10], [15, 15], [18, 19], [17, 24]]
g.river(river_points, 2, TERRAIN["water"])
g.rect(0, 23, W, 3, TERRAIN["water"])
g.blob(20, 22, 4, TERRAIN["sand"])
g.blob(12, 22, 3, TERRAIN["sand"])
for (cx, cy, r) in [(6, 22, 2), (26, 22, 2), (30, 21, 2)]:
    g.blob(cx, cy, r, TERRAIN["hill"], collision=IMPASSABLE)

# --- 3. Routes (reseau principal, AVANT les batiments : les batiments viennent ensuite se
#     raccorder dessus, jamais l'inverse) ---
# Colonne vertebrale nord-sud (sortie vers Route 1 en haut) + traversee est-ouest du centre-ville.
g.vline(4, 20, 17, TERRAIN["path"])
g.hline(8, 26, 8, TERRAIN["path"])
g.hline(10, 30, 19, TERRAIN["path"])
g.vline(8, 19, 26, TERRAIN["path"])

# --- 4. Batiments (empreintes + portes) : positions du plan de Thomas, archetypes varies
#     (PC/Mart/Gym-style) pour eviter que 7 maisons se ressemblent toutes, chacune avec une
#     porte visible et fonctionnelle. ---
# "mystery" (MaisonVoletsFermees) est volontairement a x=10, pas x=14 : scripts.inc code en dur
# la position de repos de la Jumelle (LOCALID_LITTLEROOT_TWIN) a (17,2) et (18,1) selon l'etat du
# jeu (LittlerootTown_EventScript_SetTwinGuardingRoutePos/SetTwinPos) - a x=14 le batiment
# gym-style (largeur 4, x14-17) l'aurait fait apparaitre plantee dans le toit/mur.
buildings = []
buildings.append(("lab", g.build_lab(22, 3, "lab")))
buildings.append(("player", g.build_pc_style(6, 4, "player")))
buildings.append(("mystery", g.build_gym_style(10, 2, "mystery")))
buildings.append(("regis", g.build_mart_style(24, 11, "regis")))
buildings.append(("mmechen", g.build_pc_style(5, 10, "mmechen")))
buildings.append(("vieuxdresseur", g.build_gym_style(11, 16, "vieuxdresseur")))
buildings.append(("gardien", g.build_mart_style(28, 17, "gardien")))
buildings.append(("damebaies", g.build_pc_style(19, 15, "damebaies")))

doors = {name: pos for name, pos in buildings}
print("doors:", doors)

# --- 5. Chemins de raccordement porte -> route principale (chaque porte a explicitement un
#     segment de chemin qui la relie a la colonne vertebrale, pas de porte orpheline). ---
def connect_door_to_path(door_x, door_y, target_x, target_y):
    front_x, front_y = door_x, door_y + 1
    g.put(front_x, front_y, TERRAIN["path"], PASSABLE)
    g.hline(min(front_x, target_x), max(front_x, target_x), front_y, TERRAIN["path"])
    g.vline(min(front_y, target_y), max(front_y, target_y), target_x, TERRAIN["path"])

connect_door_to_path(*doors["lab"], 26, 8)
connect_door_to_path(*doors["player"], 8, 8)
connect_door_to_path(*doors["mystery"], 17, 5)
connect_door_to_path(*doors["regis"], 26, 19)
connect_door_to_path(*doors["mmechen"], 17, 13)
connect_door_to_path(*doors["vieuxdresseur"], 17, 19)
connect_door_to_path(*doors["gardien"], 30, 19)
connect_door_to_path(*doors["damebaies"], 20, 19)

# --- 6. Vegetation (regroupee, pas au hasard tuile par tuile) ---
for (cx, cy, r) in [(3, 5, 3), (2, 11, 3), (4, 17, 3), (3, 22, 2)]:
    g.tree_blob(cx, cy, r, TERRAIN["tree_bush"])
for (cx, cy, r) in [(31, 3, 3), (32, 10, 2), (30, 16, 3), (24, 0, 2)]:
    g.tree_blob(cx, cy, r, TERRAIN["tree_pine"])
g.rect(26, 6, 3, 2, TERRAIN["tall_grass"])
g.rect(7, 13, 3, 2, TERRAIN["tall_grass"])
g.rect(22, 17, 3, 2, TERRAIN["tall_grass"])
g.rect(4, 20, 2, 2, TERRAIN["tall_grass"])
g.put(28, 8, TERRAIN["flower"])
g.put(9, 15, TERRAIN["flower"])
# haies decoratives devant quelques maisons
for name in ("player", "mmechen", "damebaies"):
    dx, dy = doors[name]
    if g.tiles[dy + 1][dx - 2] == TERRAIN["grass"]:
        g.put(dx - 2, dy + 1, TERRAIN["hedge"], IMPASSABLE)
    if g.tiles[dy + 1][dx + 2] == TERRAIN["grass"] if dx + 2 < W else False:
        g.put(dx + 2, dy + 1, TERRAIN["hedge"], IMPASSABLE)

# --- 7. Validation ---
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

# --- 8. Ecriture ---
with open("/home/user/test-gba/engine/data/layouts/LittlerootTown/map.bin", "wb") as f:
    f.write(g.to_bin())

border = bytearray()
import struct
for _ in range(4):
    val = TERRAIN["grass"] | (3 << 12)
    border += struct.pack("<H", val)
with open("/home/user/test-gba/engine/data/layouts/LittlerootTown/border.bin", "wb") as f:
    f.write(border)

import json
with open("/tmp/doors_v2.json", "w") as f:
    json.dump(doors, f)

print(f"map.bin: {g.w*g.h*2} bytes ({g.w}x{g.h})")
