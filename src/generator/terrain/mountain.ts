import type { Rng } from "../rng.js";
import type { Direction } from "../../schema/types.js";
import type { MapTemplate } from "../../schema/template.js";
import { type Point } from "../grid.js";
import {
  carveMainPath,
  frameBorder,
  inwardOf,
  makeTerrain,
  makeZones,
  markPathEdgeRing,
  resolveEntryExit,
  type TerrainResult,
} from "./common.js";

/**
 * Montagne : bandes de falaises horizontales (paliers d'altitude) que le
 * chemin en lacets doit traverser par des brèches — le chemin est carvé
 * en dernier et perce systématiquement les bandes qu'il croise, donc la
 * traversée reste garantie.
 */
export function generateMountainTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
  isSolid: (tile: string) => boolean,
): TerrainResult {
  const terrain = makeTerrain(width, height, "grass");
  const zones = makeZones(width, height, "terrace");
  frameBorder(terrain, width, height, "cliff");

  const bandSpacing = Math.max(5, Math.floor(height / 6));
  for (let y = bandSpacing; y < height - bandSpacing; y += bandSpacing) {
    for (let x = 1; x < width - 1; x++) terrain[y][x] = "cliff";
  }

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
    // Le chemin perce toujours les falaises qu'il traverse : lisibilité garantie.
    terrain[y][x] = "stairs";
  }
  markPathEdgeRing(zones, width, height, mainPath, "path_edge");

  // Le point de vue du sommet est posé sur le point le plus haut du chemin
  // lui-même (jamais un point arbitraire qui risquerait de tomber sur une
  // falaise solide) : c'est là que le chemin franchit la bande la plus haute.
  let landmarkSpot: Point = { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  let minY = Infinity;
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    if (y < minY) {
      minY = y;
      landmarkSpot = { x, y };
    }
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
