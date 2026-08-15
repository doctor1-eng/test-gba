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
 * Plage : dégradé sable → eau peu profonde → eau, le rivage occupe le bord
 * qui n'est utilisé par aucune connexion terrestre (sinon "south" par
 * défaut). Le chemin longe le rivage en restant sur le sable.
 */
export function generateBeachTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
  isSolid: (tile: string) => boolean,
): TerrainResult {
  const terrain = makeTerrain(width, height, "sand");
  const zones = makeZones(width, height, "sand");
  frameBorder(terrain, width, height, "tree");

  const shoreDir: Direction = (["south", "east", "north", "west"] as Direction[]).find(
    (d) => !connections.includes(d),
  )!;
  const shoreDepth = Math.max(3, Math.floor(Math.min(width, height) * 0.25));

  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      let distToShore = Infinity;
      if (shoreDir === "south") distToShore = height - 1 - y;
      if (shoreDir === "north") distToShore = y;
      if (shoreDir === "east") distToShore = width - 1 - x;
      if (shoreDir === "west") distToShore = x;
      if (distToShore < shoreDepth * 0.4) {
        terrain[y][x] = "water";
      } else if (distToShore < shoreDepth) {
        terrain[y][x] = "shallow_water";
        zones[y][x] = "shoreline";
      } else if (distToShore < shoreDepth + 2) {
        zones[y][x] = "shoreline";
      }
    }
  }
  const landConnections = connections.filter((d) => d !== shoreDir);
  const { entry, exit, dirA, dirB, edgeAnchors } = resolveEntryExit(rng, width, height, landConnections);

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
    if (terrain[y][x] === "water" || terrain[y][x] === "shallow_water") continue;
    terrain[y][x] = "path";
  }
  markPathEdgeRing(zones, width, height, mainPath, "path_edge", ["shoreline"]);

  let landmarkSpot: Point = { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  for (let tries = 0; tries < 30; tries++) {
    const cand = { x: rng.int(2, width - 3), y: rng.int(2, height - 3) };
    if (terrain[cand.y][cand.x] === "sand" && !mainPath.has(`${cand.x},${cand.y}`)) {
      landmarkSpot = cand;
      break;
    }
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
