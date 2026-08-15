import type { Rng } from "../rng.js";
import type { Direction } from "../../schema/types.js";
import type { MapTemplate } from "../../schema/template.js";
import { type Point } from "../grid.js";
import {
  carveMainPath,
  frameBorder,
  growBlob,
  inwardOf,
  makeTerrain,
  makeZones,
  markPathEdgeRing,
  resolveEntryExit,
  type TerrainResult,
} from "./common.js";

/**
 * Forêt : masse d'arbres denses avec un chemin sinueux carvé dedans et 1-2
 * clairières (zones "clearing") servant de points d'intérêt / objets cachés.
 * Le landmark (grand arbre) est posé au bord d'une clairière, visible sans
 * bloquer le passage.
 */
export function generateForestTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
  isSolid: (tile: string) => boolean,
): TerrainResult {
  const terrain = makeTerrain(width, height, "dense_tree");
  const zones = makeZones(width, height, "canopy");
  frameBorder(terrain, width, height, "tree");

  const { entry, exit, dirA, dirB, edgeAnchors } = resolveEntryExit(rng, width, height, connections);

  const approachEntry = inwardOf(entry, dirA);
  const approachExit = dirB ? inwardOf(exit, dirB) : exit;
  const mainPath = new Set<string>();
  for (const t of carveMainPath(rng, width, height, entry, approachEntry, template.path.mainWidth, 0)) mainPath.add(t);
  for (const t of carveMainPath(
    rng,
    width,
    height,
    approachEntry,
    approachExit,
    template.path.mainWidth,
    template.path.windiness,
  ))
    mainPath.add(t);
  if (dirB) {
    for (const t of carveMainPath(rng, width, height, approachExit, exit, template.path.mainWidth, 0)) mainPath.add(t);
  }

  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    terrain[y][x] = "path";
  }
  markPathEdgeRing(zones, width, height, mainPath, "path_edge");

  // Clairières : taches de sol dégagé creusées dans le fourré. Chaque
  // clairière démarre sur une case directement adjacente au chemin (comme
  // les cavernes de grotte.ts) — sans cette ancre, une marche aléatoire qui
  // évite le chemin peut former une poche totalement isolée du reste de la
  // carte, un cul-de-sac involontaire (section 13, détecté par le
  // validateur : "n'est atteignable depuis aucun point d'entrée").
  const pathTiles: Point[] = [...mainPath].map((k) => {
    const [x, y] = k.split(",").map(Number);
    return { x, y };
  });
  const neighborOffsets = [
    { x: 1, y: 0 },
    { x: -1, y: 0 },
    { x: 0, y: 1 },
    { x: 0, y: -1 },
  ];

  const clearingCount = rng.int(1, 2);
  // Repli garanti non-solide si aucune clairière ne peut être ancrée : une
  // case du chemin lui-même, jamais le centre géométrique (souvent un arbre).
  let landmarkSpot: Point = pathTiles[Math.floor(pathTiles.length / 2)] ?? { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  for (let i = 0; i < clearingCount && pathTiles.length > 0; i++) {
    const attach = pathTiles[rng.int(0, pathTiles.length - 1)];
    const seedCandidates = neighborOffsets
      .map((o) => ({ x: attach.x + o.x, y: attach.y + o.y }))
      .filter((p) => p.x > 0 && p.y > 0 && p.x < width - 1 && p.y < height - 1 && !mainPath.has(`${p.x},${p.y}`));
    if (seedCandidates.length === 0) continue;
    const seed = rng.pick(seedCandidates);
    const blob = growBlob(rng, width, height, seed, rng.int(10, 20), mainPath);
    for (const k of blob) {
      const [x, y] = k.split(",").map(Number);
      if (mainPath.has(k)) continue;
      terrain[y][x] = "grass";
      zones[y][x] = "clearing";
    }
    if (i === 0) landmarkSpot = seed;
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
