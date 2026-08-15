import type { Rng } from "../rng.js";
import type { InteriorTemplate } from "../../schema/template.js";
import { frameBorder, makeTerrain, makeZones } from "./common.js";
import type { Point } from "../grid.js";

export interface InteriorTerrainResult {
  width: number;
  height: number;
  terrain: string[][];
  zones: string[][];
  door: Point;
  /** Emplacements de mobilier posés le long des murs, dans l'ordre du template. */
  furnitureSpots: Array<{ x: number; y: number; tile: string }>;
  npcSpot: Point;
}

/**
 * Génère une pièce rectangulaire simple : sol, murs, une porte au centre du
 * mur sud (aligne avec l'entrée du bâtiment extérieur), mobilier réparti le
 * long des murs selon la liste du template, un emplacement PNJ au fond.
 */
export function generateInteriorTerrain(rng: Rng, template: InteriorTemplate): InteriorTerrainResult {
  const width = rng.int(template.dimensions.minWidth, template.dimensions.maxWidth);
  const height = rng.int(template.dimensions.minHeight, template.dimensions.maxHeight);
  const floorTile = template.type === "pokemon_center" || template.type === "pokemart" ? "floor_tile" : "floor_wood";

  const terrain = makeTerrain(width, height, floorTile);
  const zones = makeZones(width, height, "room");
  frameBorder(terrain, width, height, "wall");

  const door: Point = { x: Math.floor(width / 2), y: height - 1 };
  terrain[door.y][door.x] = "exit_mat";

  const furnitureSpots: Array<{ x: number; y: number; tile: string }> = [];
  let fx = 2;
  for (const tile of template.furniture) {
    if (fx >= width - 2) break;
    const spot = { x: fx, y: 1, tile };
    terrain[spot.y][spot.x] = tile;
    furnitureSpots.push(spot);
    fx += 2;
  }

  const npcSpot: Point = { x: Math.floor(width / 2), y: 2 };

  return { width, height, terrain, zones, door, furnitureSpots, npcSpot };
}
