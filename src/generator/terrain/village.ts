import type { Rng } from "../rng.js";
import type { Direction } from "../../schema/types.js";
import type { MapTemplate } from "../../schema/template.js";
import { type Point } from "../grid.js";
import {
  carveMainPath,
  edgePoint,
  frameBorder,
  inwardOf,
  makeTerrain,
  makeZones,
  type TerrainResult,
} from "./common.js";

/** Village : un unique chemin reliant les bords connectés, sans place centrale formelle. */
export function generateVillageTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
): TerrainResult {
  const terrain = makeTerrain(width, height, "grass");
  const zones = makeZones(width, height, "open");
  frameBorder(terrain, width, height, "tree");

  const anchors = connections.map((dir) => ({ dir, point: edgePoint(rng, width, height, dir) }));
  const mainPath = new Set<string>();
  const edgeAnchors: Partial<Record<Direction, Point>> = {};

  const hub: Point =
    anchors.length > 0
      ? anchors[0].point
      : { x: Math.floor(width / 2), y: Math.floor(height / 2) };

  for (const { dir, point } of anchors) {
    edgeAnchors[dir] = point;
    terrain[point.y][point.x] = "path";
    const approach = inwardOf(point, dir);
    for (const t of carveMainPath(rng, width, height, point, approach, template.path.mainWidth, 0)) mainPath.add(t);
    for (const t of carveMainPath(rng, width, height, approach, hub, template.path.mainWidth, template.path.windiness))
      mainPath.add(t);
  }
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    terrain[y][x] = "path";
    zones[y][x] = "path_edge";
  }
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    for (let dy = -1; dy <= 1; dy++)
      for (let dx = -1; dx <= 1; dx++) {
        const nx = x + dx;
        const ny = y + dy;
        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1) zones[ny][nx] = "path_edge";
      }
  }

  const landmarkSpot: Point = { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  zones[landmarkSpot.y][landmarkSpot.x] = "open";

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot };
}
