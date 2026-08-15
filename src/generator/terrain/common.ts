import type { Rng } from "../rng.js";
import { carvePath, floodFillReachable, inBounds, key, type Point } from "../grid.js";
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
  /**
   * Landmarks additionnels suggérés par le générateur de terrain lui-même
   * (ex: un étang qu'il vient de creuser) — préférés par placeLandmarks aux
   * décalages génériques puisqu'ils correspondent à un vrai élément du
   * terrain plutôt qu'à une case arbitraire.
   */
  extraLandmarkSpots?: Array<{ point: Point; type: string }>;
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

/**
 * Ajoute une frange d'arbres à profondeur irrégulière juste à l'intérieur
 * du cadre solide déjà posé par frameBorder (jamais retiré : la bordure
 * réelle reste garantie scellée). Rompt la silhouette parfaitement
 * rectangulaire d'une carte à cadre uniforme — section 4 de la mission :
 * "évite absolument les cartes parfaitement symétriques ou artificielles".
 * La profondeur varie par marche aléatoire bornée (jamais un bruit non
 * contraint). Appelée après le tracé du chemin/de la place : ne repeint
 * que les cases encore au sol de base (`groundTile`), donc ne touche
 * jamais un chemin, une place ou toute autre feature déjà posée.
 *
 * Sur une petite carte (ex: un village à connexion unique, où l'essentiel
 * de la zone n'est praticable que parce que c'est un grand champ ouvert
 * sans obstacle), une frange trop profonde peut suffire à couper un pan
 * entier du terrain du reste de la carte — et pas seulement engloutir sa
 * propre surface : comparer la perte à la seule taille de la frange ne
 * suffit pas (une frange qui mange presque tout l'intérieur "explique"
 * mathématiquement sa propre perte sans qu'on détecte le problème). On
 * exige donc qu'au moins la moitié de la surface intérieure (hors cadre)
 * reste atteignable depuis une tuile de chemin connue ; sinon, on annule.
 */
export function addOrganicFringe(
  rng: Rng,
  terrain: string[][],
  width: number,
  height: number,
  fringeTile: string,
  groundTile: string,
  mainPath: Set<string>,
  isSolid: (tile: string) => boolean,
  maxDepth = 3,
) {
  const referenceKey = mainPath.values().next().value as string | undefined;
  if (!referenceKey) return;
  const [refX, refY] = referenceKey.split(",").map(Number);
  const solidAt = (x: number, y: number) => isSolid(terrain[y]?.[x] ?? "");
  const interiorArea = (width - 2) * (height - 2);
  const minAcceptable = Math.floor(interiorArea * 0.5);

  const painted: Point[] = [];
  const paintEdge = (getCell: (i: number, depth: number) => Point, length: number) => {
    let depth = rng.int(1, maxDepth);
    for (let i = 0; i < length; i++) {
      depth = Math.min(maxDepth, Math.max(1, depth + rng.int(-1, 1)));
      for (let d = 1; d <= depth; d++) {
        const p = getCell(i, d);
        if (!inBounds(width, height, p.x, p.y)) continue;
        if (terrain[p.y][p.x] !== groundTile) continue;
        terrain[p.y][p.x] = fringeTile;
        painted.push(p);
      }
    }
  };

  paintEdge((i, d) => ({ x: i, y: d }), width); // nord
  paintEdge((i, d) => ({ x: i, y: height - 1 - d }), width); // sud
  paintEdge((i, d) => ({ x: d, y: i }), height); // ouest
  paintEdge((i, d) => ({ x: width - 1 - d, y: i }), height); // est

  const after = floodFillReachable(width, height, solidAt, { x: refX, y: refY }).size;
  if (after < minAcceptable) {
    for (const p of painted) terrain[p.y][p.x] = groundTile; // a mangé trop de l'intérieur praticable : annulé
  }
}

/**
 * Creuse un point d'eau (étang) uniquement si ça ne coupe le terrain
 * praticable en deux : contrairement à une clairière ou une salle de
 * grotte, un étang est délibérément à l'écart du chemin et fait de tuiles
 * *solides* — sur une petite carte, une simple tache peut suffire à
 * cloisonner une partie du terrain. On simule la pose, on compare la
 * taille de la zone atteignable avant/après (en ne perdant jamais plus que
 * la surface de l'étang lui-même), et on annule sinon plutôt que de
 * produire une poche inaccessible.
 */
export function carveSafePond(
  rng: Rng,
  terrain: string[][],
  zones: string[][],
  width: number,
  height: number,
  mainPath: Set<string>,
  isSolid: (tile: string) => boolean,
  waterTile: string,
  zoneName: string,
  sizeRange: [number, number] = [6, 12],
  attempts = 15,
): { point: Point; type: string } | null {
  const referenceKey = mainPath.values().next().value as string | undefined;
  if (!referenceKey) return null;
  const [refX, refY] = referenceKey.split(",").map(Number);
  const solidAt = (x: number, y: number) => isSolid(terrain[y]?.[x] ?? "");

  for (let attempt = 0; attempt < attempts; attempt++) {
    const seed: Point = { x: rng.int(3, width - 4), y: rng.int(3, height - 4) };
    const seedKey = key(seed.x, seed.y);
    if (mainPath.has(seedKey) || zones[seed.y][seed.x] === "plaza") continue;

    const blob = growBlob(rng, width, height, seed, rng.int(sizeRange[0], sizeRange[1]), mainPath);
    const cells = [...blob]
      .filter((k) => k !== seedKey && !mainPath.has(k))
      .map((k) => {
        const [x, y] = k.split(",").map(Number);
        return { x, y };
      })
      .filter(({ x, y }) => zones[y][x] !== "plaza");
    if (cells.length === 0) continue;

    const before = floodFillReachable(width, height, solidAt, { x: refX, y: refY }).size;
    const saved = cells.map(({ x, y }) => ({ x, y, tile: terrain[y][x] }));
    for (const { x, y } of cells) terrain[y][x] = waterTile;
    const after = floodFillReachable(width, height, solidAt, { x: refX, y: refY }).size;

    if (before - after > cells.length + 2) {
      for (const { x, y, tile } of saved) terrain[y][x] = tile; // revert : ça cloisonnait autre chose que l'étang lui-même
      continue;
    }
    for (const { x, y } of cells) zones[y][x] = zoneName;
    return { point: seed, type: "pond" };
  }
  return null;
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
