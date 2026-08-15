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

/**
 * Ville : place centrale (landmark) reliée par un réseau de chemins à
 * chaque bord connecté. Zone "plaza" au centre, "residential" ailleurs,
 * "path_edge" en bordure de chemin (pour la densité de décoration).
 */
export function generateTownTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
): TerrainResult {
  const terrain = makeTerrain(width, height, "grass");
  const zones = makeZones(width, height, "residential");
  frameBorder(terrain, width, height, "tree");

  const center: Point = { x: Math.floor(width / 2), y: Math.floor(height / 2) };
  const plazaRadius = Math.max(2, Math.floor(Math.min(width, height) * 0.12));

  const mainPath = new Set<string>();
  const edgeAnchors: Partial<Record<Direction, Point>> = {};

  for (const dir of connections) {
    const anchor = edgePoint(rng, width, height, dir);
    edgeAnchors[dir] = anchor;
    terrain[anchor.y][anchor.x] = "path";
    const approach = inwardOf(anchor, dir);
    const segment = carveMainPath(rng, width, height, approach, center, template.path.mainWidth, template.path.windiness);
    for (const t of segment) mainPath.add(t);
    const straight = carveMainPath(rng, width, height, anchor, approach, template.path.mainWidth, 0);
    for (const t of straight) mainPath.add(t);
  }

  // Trace le sol pour les tuiles de chemin puis la place centrale.
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    if (terrain[y][x] !== "tree") terrain[y][x] = "path";
  }
  for (let dy = -plazaRadius; dy <= plazaRadius; dy++) {
    for (let dx = -plazaRadius; dx <= plazaRadius; dx++) {
      const x = center.x + dx;
      const y = center.y + dy;
      if (x <= 0 || y <= 0 || x >= width - 1 || y >= height - 1) continue;
      if (dx * dx + dy * dy <= plazaRadius * plazaRadius) {
        terrain[y][x] = "plaza";
        zones[y][x] = "plaza";
      }
    }
  }

  // Zone "path_edge" : couronne autour du réseau de chemins.
  for (const k of mainPath) {
    const [x, y] = k.split(",").map(Number);
    for (let dy = -1; dy <= 1; dy++) {
      for (let dx = -1; dx <= 1; dx++) {
        const nx = x + dx;
        const ny = y + dy;
        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && zones[ny][nx] !== "plaza") {
          zones[ny][nx] = "path_edge";
        }
      }
    }
  }

  return { width, height, terrain, zones, mainPath, edgeAnchors, landmarkSpot: center };
}
