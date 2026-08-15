import type { Rng } from "./rng.js";

export interface Point {
  x: number;
  y: number;
}

export function makeGrid<T>(width: number, height: number, fill: T): T[][] {
  return Array.from({ length: height }, () => Array.from({ length: width }, () => fill));
}

export function inBounds(width: number, height: number, x: number, y: number): boolean {
  return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * Trace un chemin sinueux entre deux points, contraint (pas un random walk
 * libre) : avance globalement vers la cible, avec une probabilité de
 * déviation latérale pilotée par `windiness`. Retourne les tuiles couvertes
 * (avec la largeur `width` appliquée perpendiculairement à la direction).
 */
export function carvePath(
  rng: Rng,
  from: Point,
  to: Point,
  gridW: number,
  gridH: number,
  width: number,
  windiness: number,
): Set<string> {
  const covered = new Set<string>();
  let cx = from.x;
  let cy = from.y;
  const maxSteps = (gridW + gridH) * 4;
  let steps = 0;

  const stamp = (x: number, y: number) => {
    const half = Math.floor(width / 2);
    for (let dy = -half; dy <= half; dy++) {
      for (let dx = -half; dx <= half; dx++) {
        const nx = x + dx;
        const ny = y + dy;
        if (inBounds(gridW, gridH, nx, ny)) covered.add(`${nx},${ny}`);
      }
    }
  };

  stamp(cx, cy);
  while ((cx !== to.x || cy !== to.y) && steps < maxSteps) {
    steps++;
    const dx = to.x - cx;
    const dy = to.y - cy;
    const preferHorizontal = Math.abs(dx) > Math.abs(dy);
    let moveX = 0;
    let moveY = 0;

    if (rng.bool(windiness) && (dx !== 0 || dy !== 0)) {
      // Déviation contrôlée : mouvement perpendiculaire à la direction dominante.
      if (preferHorizontal) moveY = rng.bool(0.5) ? 1 : -1;
      else moveX = rng.bool(0.5) ? 1 : -1;
    } else {
      if (dx !== 0 && (preferHorizontal || dy === 0)) moveX = dx > 0 ? 1 : -1;
      else if (dy !== 0) moveY = dy > 0 ? 1 : -1;
    }

    const nx = Math.min(Math.max(cx + moveX, 0), gridW - 1);
    const ny = Math.min(Math.max(cy + moveY, 0), gridH - 1);
    cx = nx;
    cy = ny;
    stamp(cx, cy);
  }
  // Garantit l'arrivée exacte même si le budget de pas est épuisé.
  stamp(to.x, to.y);
  return covered;
}

/** Flood-fill de connectivité : retourne l'ensemble des cases atteignables depuis `start` sans traverser `solid`. */
export function floodFillReachable(
  width: number,
  height: number,
  solid: (x: number, y: number) => boolean,
  start: Point,
): Set<string> {
  const visited = new Set<string>();
  if (solid(start.x, start.y)) return visited;
  const queue: Point[] = [start];
  visited.add(`${start.x},${start.y}`);
  const dirs = [
    [1, 0],
    [-1, 0],
    [0, 1],
    [0, -1],
  ];
  while (queue.length > 0) {
    const p = queue.pop()!;
    for (const [dx, dy] of dirs) {
      const nx = p.x + dx;
      const ny = p.y + dy;
      const key = `${nx},${ny}`;
      if (!inBounds(width, height, nx, ny) || visited.has(key) || solid(nx, ny)) continue;
      visited.add(key);
      queue.push({ x: nx, y: ny });
    }
  }
  return visited;
}

export function key(x: number, y: number): string {
  return `${x},${y}`;
}

/**
 * Case de repli quand aucun point d'entrée n'est connu : la plus grande
 * composante connexe de terrain praticable, jamais le centre géométrique
 * (souvent un mur, une falaise, une paroi de grotte).
 */
export function largestWalkableComponentSeed(
  width: number,
  height: number,
  solid: (x: number, y: number) => boolean,
): Point | null {
  const visited = new Set<string>();
  let best: Point | null = null;
  let bestSize = 0;
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const k = key(x, y);
      if (solid(x, y) || visited.has(k)) continue;
      const comp = floodFillReachable(width, height, solid, { x, y });
      for (const c of comp) visited.add(c);
      if (comp.size > bestSize) {
        bestSize = comp.size;
        best = { x, y };
      }
    }
  }
  return best;
}

/**
 * Ensemble des tuiles atteignables depuis un jeu de points d'entrée (warps,
 * en général). Sans point d'entrée, retombe sur la plus grande composante
 * connexe praticable. Utilisée à la fois par le générateur (pour ne jamais
 * placer PNJ/objet/landmark hors de portée) et par le validateur (pour
 * vérifier exactement la même chose après coup) — les deux DOIVENT
 * calculer l'atteignabilité de la même façon, sans quoi une case jugée
 * "sûre" pendant la génération peut être jugée injoignable à la validation.
 */
export function computeReachableSet(
  width: number,
  height: number,
  solid: (x: number, y: number) => boolean,
  entryPoints: Point[],
): Set<string> {
  const union = new Set<string>();
  for (const p of entryPoints) {
    for (const k of floodFillReachable(width, height, solid, p)) union.add(k);
  }
  if (union.size > 0) return union;
  const seed = largestWalkableComponentSeed(width, height, solid);
  return seed ? floodFillReachable(width, height, solid, seed) : union;
}
