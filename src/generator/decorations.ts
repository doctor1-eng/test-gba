import type { Rng } from "./rng.js";
import type { Decoration } from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import type { TerrainResult } from "./terrain/common.js";

const DECORATION_TILE: Record<string, string> = {
  flower: "flower",
  fence: "fence",
  sign: "sign",
  bridge: "bridge",
  rock_deco: "rock",
  mushroom: "flower",
  berry_tree_deco: "flower",
  crystal_deco: "crystal",
  rubble_deco: "cave_rubble",
  ore_deco: "ore_vein",
  snow_patch: "snow",
  driftwood: "rock",
  shell_deco: "flower",
  statue_deco: "rock",
};

/**
 * Place des décorations par zone selon la densité du template. Une tuile
 * n'est jamais décorée si elle appartient au chemin principal (lisibilité,
 * section 3) ni si elle est déjà solide (arbre, eau, mur...).
 */
export function placeDecorations(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  isSolidTerrainTile: (tile: string) => boolean,
  occupied: Set<string>,
): Decoration[] {
  const { width, height, terrain, zones, mainPath } = terrainResult;
  const decorations: Decoration[] = [];

  for (let y = 1; y < height - 1; y++) {
    for (let x = 1; x < width - 1; x++) {
      const k = `${x},${y}`;
      if (mainPath.has(k) || occupied.has(k)) continue;
      if (isSolidTerrainTile(terrain[y][x])) continue;
      const zone = zones[y][x];
      const density = template.decorations.densityByZone[zone] ?? 0;
      if (density <= 0 || !rng.bool(density)) continue;
      const allowed = template.decorations.allowed.filter((t) => DECORATION_TILE[t]);
      if (allowed.length === 0) continue;
      const type = rng.pick(allowed);
      decorations.push({ x, y, type });
      occupied.add(k);
    }
  }
  return decorations;
}

export function decorationTileId(type: string): string {
  return DECORATION_TILE[type] ?? "flower";
}
