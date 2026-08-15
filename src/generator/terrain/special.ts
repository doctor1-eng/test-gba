import type { Rng } from "../rng.js";
import type { Direction } from "../../schema/types.js";
import type { MapTemplate } from "../../schema/template.js";
import { generateVillageTerrain } from "./village.js";
import type { TerrainResult } from "./common.js";

/**
 * Zone spéciale/narrative : réutilise la topologie ouverte du village
 * (un point central, chemin unique) — la spécificité vient des events/NPC
 * narratifs posés dessus, pas d'une géométrie différente.
 */
export function generateSpecialTerrain(
  rng: Rng,
  width: number,
  height: number,
  template: MapTemplate,
  connections: Direction[],
): TerrainResult {
  return generateVillageTerrain(rng, width, height, template, connections);
}
