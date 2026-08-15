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
 * Route : corridor reliant deux bords (généralement opposés), avec des
 * poches d'herbes hautes (rencontres) à l'écart immédiat du chemin — jamais
 * sur le chemin lui-même, pour garder la circulation lisible.
 */
export function generateRouteTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
  isSolid: (tile: string) => boolean,
): TerrainResult {
  const terrain = makeTerrain(width, height, "grass");
  const zones = makeZones(width, height, "grass_field");
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
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    for (let dy = -1; dy <= 1; dy++)
      for (let dx = -1; dx <= 1; dx++) {
        const nx = x + dx;
        const ny = y + dy;
        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && zones[ny][nx] !== "grass_field") continue;
        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1) zones[ny][nx] = "path_edge";
      }
  }

  // Poches d'herbes hautes : plusieurs taches organiques, jamais sur le chemin.
  const patchCount = Math.max(2, Math.floor((width * height) / 90));
  for (let i = 0; i < patchCount; i++) {
    let seed: Point;
    let tries = 0;
    do {
      seed = { x: rng.int(2, width - 3), y: rng.int(2, height - 3) };
      tries++;
    } while (mainPath.has(`${seed.x},${seed.y}`) && tries < 20);
    const blob = growBlob(rng, width, height, seed, rng.int(6, 14), mainPath);
    for (const k of blob) {
      const [x, y] = k.split(",").map(Number);
      if (mainPath.has(k)) continue;
      terrain[y][x] = "tall_grass";
    }
  }

  // Landmark de repos : point ouvert proche du chemin mais pas dessus.
  let landmarkSpot: Point = { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  for (let tries = 0; tries < 30; tries++) {
    const cand = { x: rng.int(2, width - 3), y: rng.int(2, height - 3) };
    const k = `${cand.x},${cand.y}`;
    if (!mainPath.has(k) && terrain[cand.y][cand.x] === "grass") {
      landmarkSpot = cand;
      break;
    }
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
