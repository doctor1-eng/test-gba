import type { Rng } from "./rng.js";
import type { Building, GameMap, Warp } from "../schema/types.js";
import type { MapTemplate, InteriorTemplate } from "../schema/template.js";
import type { TerrainResult } from "./terrain/common.js";
import { loadTemplate } from "../data/loader.js";
import { buildInteriorMap } from "./interiors.js";
import type { Point } from "./grid.js";

const FOOTPRINT: Record<string, { w: number; h: number; label: string }> = {
  pokemon_center: { w: 6, h: 5, label: "Centre Pokémon" },
  pokemart: { w: 5, h: 4, label: "Boutique" },
  gym: { w: 7, h: 6, label: "Arène" },
  house: { w: 4, h: 4, label: "Maison" },
  lab: { w: 6, h: 5, label: "Laboratoire" },
};

export interface BuildingsResult {
  buildings: Building[];
  interiorMaps: GameMap[];
  exteriorWarps: Warp[];
}

/**
 * Place les bâtiments en bordure du réseau de chemins, porte tournée vers
 * le chemin (jamais un bâtiment isolé sans accès). Chaque bâtiment avec
 * intérieur génère sa propre map + la paire de warps qui les relie.
 */
export function placeBuildings(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  mapId: string,
  region: string,
  progression: GameMap["metadata"]["progression"],
  occupied: Set<string>,
): BuildingsResult {
  const { width, height, terrain, zones, mainPath } = terrainResult;
  const buildings: Building[] = [];
  const interiorMaps: GameMap[] = [];
  const exteriorWarps: Warp[] = [];

  const typeQueue = [...template.buildings.required];
  const targetCount = rng.int(
    Math.max(template.buildings.minCount, typeQueue.length),
    Math.max(template.buildings.maxCount, typeQueue.length),
  );
  const pool = rng.shuffle(template.buildings.optionalPool);
  let poolIdx = 0;
  while (typeQueue.length < targetCount && poolIdx < pool.length) {
    typeQueue.push(pool[poolIdx]);
    poolIdx++;
  }

  const pathTiles = rng.shuffle([...mainPath]);
  let buildingIndex = 0;

  for (const type of typeQueue) {
    const footprint = FOOTPRINT[type];
    if (!footprint) continue;

    const spot = findFootprintSpot(pathTiles, footprint.w, footprint.h, width, height, zones, mainPath, occupied);
    if (!spot) continue; // pas d'emplacement valide : on ne force jamais un bâtiment sans accès au chemin

    buildingIndex++;
    const buildingId = `${mapId}_b${buildingIndex}_${type}`;

    for (let dy = 0; dy < footprint.h; dy++) {
      for (let dx = 0; dx < footprint.w; dx++) {
        const x = spot.x + dx;
        const y = spot.y + dy;
        terrain[y][x] = dy === footprint.h - 1 ? "building_wall" : "building_roof";
        occupied.add(`${x},${y}`);
      }
    }
    const entrance: Point = { x: spot.x + Math.floor(footprint.w / 2), y: spot.y + footprint.h - 1 };
    terrain[entrance.y][entrance.x] = "door";

    const building: Building = {
      id: buildingId,
      type: type as Building["type"],
      label: footprint.label,
      x: spot.x,
      y: spot.y,
      width: footprint.w,
      height: footprint.h,
      entrance,
    };

    const interiorTemplate = loadTemplate(type) as InteriorTemplate;
    const exteriorWarpId = `w_${buildingId}`;
    const built = buildInteriorMap(
      rng,
      interiorTemplate,
      mapId,
      exteriorWarpId,
      buildingId,
      footprint.label,
      region,
      progression,
    );
    building.interiorMapId = built.map.metadata.id;
    interiorMaps.push(built.map);
    exteriorWarps.push({
      id: exteriorWarpId,
      x: entrance.x,
      y: entrance.y,
      direction: "north",
      destinationMap: built.map.metadata.id,
      destinationWarpId: built.entryWarpId,
    });

    buildings.push(building);
  }

  return { buildings, interiorMaps, exteriorWarps };
}

function findFootprintSpot(
  pathTiles: Array<{ x: number; y: number } | string>,
  w: number,
  h: number,
  width: number,
  height: number,
  zones: string[][],
  mainPath: Set<string>,
  occupied: Set<string>,
): Point | null {
  for (const t of pathTiles) {
    const [px, py] = typeof t === "string" ? t.split(",").map(Number) : [t.x, t.y];
    // Le bâtiment est ancré juste au-dessus d'un point du chemin (porte au sud, sur le chemin).
    const spotX = px - Math.floor(w / 2);
    const spotY = py - h;
    if (spotX < 1 || spotY < 1 || spotX + w >= width - 1 || spotY + h >= height - 1) continue;

    let free = true;
    for (let dy = 0; dy < h && free; dy++) {
      for (let dx = 0; dx < w && free; dx++) {
        const x = spotX + dx;
        const y = spotY + dy;
        const k = `${x},${y}`;
        if (occupied.has(k) || mainPath.has(k) || zones[y][x] === "plaza") free = false;
      }
    }
    if (free) return { x: spotX, y: spotY };
  }
  return null;
}
