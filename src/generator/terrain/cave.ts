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
  resolveEntryExit,
  type TerrainResult,
} from "./common.js";

/**
 * Grotte : parois pleines par défaut, un corridor principal carvé entre
 * les deux entrées, et quelques cavernes (salles) qui poussent depuis des
 * points du corridor — garantit la connectivité par construction (chaque
 * salle est rattachée au chemin).
 */
export function generateCaveTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
): TerrainResult {
  const terrain = makeTerrain(width, height, "cave_wall");
  const zones = makeZones(width, height, "corridor");
  frameBorder(terrain, width, height, "cave_wall");

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

  const pathTiles: Point[] = [];
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    terrain[y][x] = "cave_floor";
    pathTiles.push({ x, y });
  }
  terrain[entry.y][entry.x] = "cave_exit";
  if (dirB) terrain[exit.y][exit.x] = "cave_exit";

  // Cavernes rattachées au corridor : chaque salle pousse depuis un point du chemin.
  const roomCount = rng.int(2, 3);
  let landmarkSpot: Point = pathTiles[Math.floor(pathTiles.length / 2)] ?? { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  let biggestRoom = 0;
  for (let i = 0; i < roomCount; i++) {
    const attach = pathTiles[rng.int(0, pathTiles.length - 1)];
    const blob = growBlob(rng, width, height, attach, rng.int(14, 26), new Set());
    for (const k of blob) {
      const [x, y] = k.split(",").map(Number);
      terrain[y][x] = "cave_floor";
      zones[y][x] = "cavern";
    }
    if (blob.size > biggestRoom) {
      biggestRoom = blob.size;
      landmarkSpot = attach;
    }
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
