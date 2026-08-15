import type { Rng } from "../rng.js";
import { carvePath, inBounds, key, type Point } from "../grid.js";
import type { Direction } from "../../schema/types.js";

export interface TerrainResult {
  width: number;
  height: number;
  terrain: string[][];
  /** Zone sémantique par tuile, utilisée par les règles de décoration/densité du template. */
  zones: string[][];
  /** Tuiles appartenant au chemin principal — protégées de tout obstacle ultérieur. */
  mainPath: Set<string>;
  /** Points d'ancrage sur les bords, un par direction utilisée pour une connexion. */
  edgeAnchors: Partial<Record<Direction, Point>>;
  /** Point suggéré pour le landmark central (zone dégagée, visible). */
  landmarkSpot: Point;
}

export function makeTerrain(width: number, height: number, ground: string): string[][] {
  return Array.from({ length: height }, () => Array.from({ length: width }, () => ground));
}

export function makeZones(width: number, height: number, defaultZone: string): string[][] {
  return Array.from({ length: height }, () => Array.from({ length: width }, () => defaultZone));
}

/** Point sur le bord indiqué, à une position aléatoire mais pas dans un coin (lisibilité). */
export function edgePoint(rng: Rng, width: number, height: number, dir: Direction): Point {
  const margin = 2;
  switch (dir) {
    case "north":
      return { x: rng.int(margin, width - 1 - margin), y: 0 };
    case "south":
      return { x: rng.int(margin, width - 1 - margin), y: height - 1 };
    case "west":
      return { x: 0, y: rng.int(margin, height - 1 - margin) };
    case "east":
      return { x: width - 1, y: rng.int(margin, height - 1 - margin) };
  }
}

/** Point juste à l'intérieur du bord (pour que le chemin ait un pas de recul avant la bordure). */
export function inwardOf(p: Point, dir: Direction): Point {
  switch (dir) {
    case "north":
      return { x: p.x, y: p.y + 2 };
    case "south":
      return { x: p.x, y: p.y - 2 };
    case "west":
      return { x: p.x + 2, y: p.y };
    case "east":
      return { x: p.x - 2, y: p.y };
  }
}

export function frameBorder(terrain: string[][], width: number, height: number, borderTile: string) {
  for (let x = 0; x < width; x++) {
    terrain[0][x] = borderTile;
    terrain[height - 1][x] = borderTile;
  }
  for (let y = 0; y < height; y++) {
    terrain[y][0] = borderTile;
    terrain[y][width - 1] = borderTile;
  }
}

export function stampTile(terrain: string[][], width: number, height: number, p: Point, tile: string) {
  if (inBounds(width, height, p.x, p.y)) terrain[p.y][p.x] = tile;
}

/**
 * Fait pousser une "tache" organique (clairière, mare, caverne) à partir
 * d'un point de départ par marche aléatoire bornée — jamais un simple
 * rectangle, jamais un bruit non contraint : le nombre de pas fixe une
 * taille cible et la marche reste connexe.
 */
export function growBlob(
  rng: Rng,
  width: number,
  height: number,
  start: Point,
  steps: number,
  avoid: Set<string>,
): Set<string> {
  const blob = new Set<string>();
  let cx = start.x;
  let cy = start.y;
  blob.add(key(cx, cy));
  const dirs = [
    [1, 0],
    [-1, 0],
    [0, 1],
    [0, -1],
  ];
  for (let i = 0; i < steps; i++) {
    const [dx, dy] = dirs[rng.int(0, 3)];
    const nx = Math.min(Math.max(cx + dx, 1), width - 2);
    const ny = Math.min(Math.max(cy + dy, 1), height - 2);
    if (!avoid.has(key(nx, ny))) {
      cx = nx;
      cy = ny;
      blob.add(key(cx, cy));
    }
  }
  return blob;
}

export interface EntryExit {
  entry: Point;
  exit: Point;
  dirA: Direction;
  /** Absent si la map n'a qu'une seule connexion (zone terminale/cul-de-sac volontaire). */
  dirB?: Direction;
  edgeAnchors: Partial<Record<Direction, Point>>;
}

/**
 * Détermine les points d'entrée/sortie d'un biome "corridor" (route, forêt,
 * grotte, montagne, plage) à partir de ses connexions déclarées :
 * - 2 connexions ou plus → relie les deux premiers bords (traversée) ;
 * - 1 seule connexion → la carte est une zone terminale volontaire : le
 *   chemin va du bord jusqu'à un point intérieur (jamais vers un second
 *   bord fictif), ce qui produit un vrai cul-de-sac exploitable plutôt
 *   qu'un aller-retour dégénéré sur le même bord.
 */
export function resolveEntryExit(rng: Rng, width: number, height: number, connections: Direction[]): EntryExit {
  const dirA = connections[0] ?? "south";
  const entry = edgePoint(rng, width, height, dirA);
  const edgeAnchors: Partial<Record<Direction, Point>> = { [dirA]: entry };

  if (connections.length >= 2) {
    const dirB = connections[1];
    const exit = edgePoint(rng, width, height, dirB);
    edgeAnchors[dirB] = exit;
    return { entry, exit, dirA, dirB, edgeAnchors };
  }

  const exit: Point = {
    x: Math.min(Math.max(Math.floor(width / 2) + rng.int(-3, 3), 2), width - 3),
    y: Math.min(Math.max(Math.floor(height / 2) + rng.int(-3, 3), 2), height - 3),
  };
  return { entry, exit, dirA, edgeAnchors };
}

/**
 * Zone en couronne autour du chemin (jamais sur le chemin lui-même, qui est
 * de toute façon toujours exclu de la décoration) — c'est là que les
 * décorations "path_edge" doivent pouvoir apparaître pour border le
 * chemin sans jamais le recouvrir.
 */
export function markPathEdgeRing(
  zones: string[][],
  width: number,
  height: number,
  mainPath: Set<string>,
  zoneName: string,
  preserveZones: string[] = [],
) {
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    for (let dy = -1; dy <= 1; dy++) {
      for (let dx = -1; dx <= 1; dx++) {
        const nx = x + dx;
        const ny = y + dy;
        if (nx <= 0 || ny <= 0 || nx >= width - 1 || ny >= height - 1) continue;
        if (mainPath.has(`${nx},${ny}`)) continue;
        if (preserveZones.includes(zones[ny][nx])) continue;
        zones[ny][nx] = zoneName;
      }
    }
  }
}

export function carveMainPath(
  rng: Rng,
  width: number,
  height: number,
  entry: Point,
  exit: Point,
  pathWidth: number,
  windiness: number,
): Set<string> {
  return carvePath(rng, entry, exit, width, height, pathWidth, windiness);
}
