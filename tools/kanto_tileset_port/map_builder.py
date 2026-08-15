"""Pipeline de construction de carte structuree (relief -> eau -> routes -> batiments -> portes ->
vegetation -> collision -> validation), remplace l'approche precedente ("poser des tuiles au petit
bonheur"). Voir docs/TECHNICAL_ARCHITECTURE.md pour le contexte complet.

Difference cle avec la version precedente (suite 33) : chaque tuile placee a maintenant un bit de
collision explicite (mur/toit/arbre/colline = infranchissable, sol/chemin/porte = franchissable) -
jusqu'ici seule l'elevation etait geree, ce qui rendait murs et arbres traversables en jeu.
"""
import math
import random
import struct

from tile_catalog import TERRAIN, DOORS, PC_STYLE, MART_STYLE, GYM_STYLE, LAB_STYLE

IMPASSABLE = 3
PASSABLE = 0


class MapGrid:
    def __init__(self, w, h, fill=TERRAIN["grass"]):
        self.w, self.h = w, h
        self.tiles = [[fill for _ in range(w)] for _ in range(h)]
        self.collision = [[PASSABLE for _ in range(w)] for _ in range(h)]
        self.buildings = []  # [(name, door_x, door_y)] for the validator

    def in_bounds(self, x, y):
        return 0 <= x < self.w and 0 <= y < self.h

    def put(self, x, y, tile, collision=None):
        if not self.in_bounds(x, y):
            return
        self.tiles[y][x] = tile
        if collision is not None:
            self.collision[y][x] = collision

    def rect(self, x, y, w, h, tile, collision=None):
        for j in range(y, y + h):
            for i in range(x, x + w):
                self.put(i, j, tile, collision)

    def hline(self, x1, x2, y, tile, collision=None):
        for i in range(min(x1, x2), max(x1, x2) + 1):
            self.put(i, y, tile, collision)

    def vline(self, y1, y2, x, tile, collision=None):
        for j in range(min(y1, y2), max(y1, y2) + 1):
            self.put(x, j, tile, collision)

    def blob(self, cx, cy, r, tile, collision=None, rng=random):
        for j in range(-r, r + 1):
            for i in range(-r, r + 1):
                if i * i + j * j <= r * r + rng.random() * r:
                    self.put(cx + i, cy + j, tile, collision)

    def thin_line(self, x1, y1, x2, y2, width, tile, collision=None):
        """Bresenham : un seul tampon par point de la ligne, pas de cumul de rectangles
        (l'ancienne version suite 33 empilait des rect() a chaque micro-pas d'interpolation,
        produisant une bande bien plus large que prevu)."""
        dx, dy = abs(x2 - x1), abs(y2 - y1)
        sx = 1 if x1 < x2 else -1
        sy = 1 if y1 < y2 else -1
        err = dx - dy
        x, y = x1, y1
        half = width // 2
        while True:
            self.rect(x - half, y - half, width, width, tile, collision)
            if x == x2 and y == y2:
                break
            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x += sx
            if e2 < dx:
                err += dx
                y += sy

    def river(self, points, width, tile):
        for p in range(len(points) - 1):
            x1, y1 = points[p]
            x2, y2 = points[p + 1]
            self.thin_line(x1, y1, x2, y2, width, tile)  # water: no collision bit, blocked by behavior

    # --- Batiments : chaque fonction peint le batiment ENTIER (toit+murs+porte), marque les
    #     murs/toits IMPASSABLE, la porte PASSABLE, et enregistre la porte pour le validateur. ---

    def _paint_row(self, x, y, tiles, collision):
        for i, t in enumerate(tiles):
            self.put(x + i, y, t, collision)

    def build_pc_style(self, x, y, name, door_tile=DOORS["pokeball_door_grey"]):
        w = len(PC_STYLE["roof_top"])
        self._paint_row(x, y, PC_STYLE["roof_top"], IMPASSABLE)
        self._paint_row(x, y + 1, PC_STYLE["roof_dormer"], IMPASSABLE)
        self._paint_row(x, y + 2, PC_STYLE["wall"], IMPASSABLE)
        row = [door_tile if t == "DOOR" else t for t in PC_STYLE["wall_door_row"]]
        self._paint_row(x, y + 3, row, IMPASSABLE)
        door_x = x + row.index(door_tile)
        door_y = y + 3
        self.put(door_x, door_y, door_tile, PASSABLE)
        self.buildings.append((name, door_x, door_y))
        return door_x, door_y

    def build_mart_style(self, x, y, name):
        self._paint_row(x, y, MART_STYLE["roof_top"], IMPASSABLE)
        self._paint_row(x, y + 1, MART_STYLE["roof_bottom"], IMPASSABLE)
        self._paint_row(x, y + 2, [68, 69, 70, 71], IMPASSABLE)
        row = MART_STYLE["wall_door_row"]
        self._paint_row(x, y + 3, row, IMPASSABLE)
        door_tile = 90
        door_x = x + row.index(door_tile)
        door_y = y + 3
        self.put(door_x, door_y, door_tile, PASSABLE)
        self.buildings.append((name, door_x, door_y))
        return door_x, door_y

    def build_gym_style(self, x, y, name, door_tile=DOORS["pokeball_door_grey"]):
        self._paint_row(x, y, GYM_STYLE["roof_top"], IMPASSABLE)
        self._paint_row(x, y + 1, GYM_STYLE["wall"], IMPASSABLE)
        self._paint_row(x, y + 2, GYM_STYLE["wall"], IMPASSABLE)
        row = [door_tile if t == "DOOR" else t for t in GYM_STYLE["wall_door_row"]]
        self._paint_row(x, y + 3, row, IMPASSABLE)
        door_x = x + row.index(door_tile)
        door_y = y + 3
        self.put(door_x, door_y, door_tile, PASSABLE)
        self.buildings.append((name, door_x, door_y))
        return door_x, door_y

    def build_lab(self, x, y, name, door_tile=DOORS["pokeball_door_grey"]):
        # 5 rangees : toit, mur, mur, mur, porte (DERNIERE rangee = niveau du sol).
        self._paint_row(x, y, LAB_STYLE["top"], IMPASSABLE)
        self._paint_row(x, y + 1, LAB_STYLE["wall"], IMPASSABLE)
        self._paint_row(x, y + 2, LAB_STYLE["wall"], IMPASSABLE)
        self._paint_row(x, y + 3, LAB_STYLE["wall"], IMPASSABLE)
        row = [door_tile if t == "DOOR" else t for t in LAB_STYLE["wall_door_row"]]
        self._paint_row(x, y + 4, row, IMPASSABLE)
        door_x = x + row.index(door_tile)
        door_y = y + 4
        self.put(door_x, door_y, door_tile, PASSABLE)
        self.buildings.append((name, door_x, door_y))
        return door_x, door_y

    def add_tree(self, cx, cy, tile=TERRAIN["tree_bush"]):
        self.put(cx, cy, tile, IMPASSABLE)

    def tree_blob(self, cx, cy, r, tile=TERRAIN["tree_bush"], rng=random):
        for j in range(-r, r + 1):
            for i in range(-r, r + 1):
                if i * i + j * j <= r * r + rng.random() * r:
                    self.put(cx + i, cy + j, tile, IMPASSABLE)

    def hill_row(self, y, depth, tile=TERRAIN["hill"]):
        self.rect(0, y, depth, 1, tile, IMPASSABLE)

    # --- Validateur ---
    def validate(self):
        errors = []
        path_like = {TERRAIN["path"], TERRAIN["sand"]}
        for name, dx, dy in self.buildings:
            neighbors = [(dx, dy + 1), (dx - 1, dy), (dx + 1, dy), (dx, dy - 1)]
            ok = any(
                self.in_bounds(nx, ny) and self.tiles[ny][nx] in path_like
                for nx, ny in neighbors
            )
            if not ok:
                errors.append(f"batiment '{name}': porte ({dx},{dy}) non desservie par un chemin adjacent")
            if self.in_bounds(dx, dy) and self.collision[dy][dx] != PASSABLE:
                errors.append(f"batiment '{name}': porte ({dx},{dy}) marquee infranchissable")
        return errors

    def ensure_door_path(self, door_x, door_y):
        """Etend un chemin jusqu'a la porte si le validateur la trouve isolee - garantit qu'aucun
        batiment ne reste desservi par rien, plutot que de livrer une carte avec une porte orpheline."""
        ny = door_y + 1
        if self.in_bounds(door_x, ny) and self.tiles[ny][door_x] not in (TERRAIN["path"], TERRAIN["sand"]):
            self.put(door_x, ny, TERRAIN["path"], PASSABLE)

    def to_bin(self):
        data = bytearray()
        for y in range(self.h):
            for x in range(self.w):
                val = self.tiles[y][x] & 0x3FF
                val |= (self.collision[y][x] & 0x3) << 10
                val |= (3 << 12)  # elevation normale, convention deja utilisee sur les autres cartes actives
                data += struct.pack("<H", val)
        return bytes(data)
