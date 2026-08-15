import type { Rng } from "./rng.js";
import type { MapObject } from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import type { ObjectsCatalog } from "../data/loader.js";
import type { TerrainResult } from "./terrain/common.js";

/**
 * Place des objets interactifs avec une justification de position :
 * - objets visibles : en bordure de chemin (le joueur les voit en passant) ;
 * - objets cachés : dans les zones à l'écart (clairières, cavernes) —
 *   jamais sur le chemin principal, jamais sans zone secondaire qui les
 *   justifie ;
 * - panneaux : aux jonctions du chemin (chemin secondaire ou entrée de zone).
 */
export function placeObjects(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  catalog: ObjectsCatalog,
  isSolid: (tile: string) => boolean,
  occupied: Set<string>,
  reachable: Set<string>,
): MapObject[] {
  const { width, height, terrain, zones, mainPath } = terrainResult;
  const objects: MapObject[] = [];

  const offPathTiles: Array<{ x: number; y: number }> = [];
  const pathEdgeTiles: Array<{ x: number; y: number }> = [];
  for (let y = 1; y < height - 1; y++) {
    for (let x = 1; x < width - 1; x++) {
      const k = `${x},${y}`;
      if (occupied.has(k) || isSolid(terrain[y][x])) continue;
      if (mainPath.has(k)) continue;
      // "À l'écart du chemin" ne veut pas dire "inaccessible" : un objet
      // caché doit rester atteignable, seulement moins visible au premier coup d'œil.
      if (reachable.size > 0 && !reachable.has(k)) continue;
      if (zones[y][x] === "path_edge") pathEdgeTiles.push({ x, y });
      else offPathTiles.push({ x, y });
    }
  }

  let objectCounter = 0;
  const place = (x: number, y: number, obj: Omit<MapObject, "id" | "x" | "y">) => {
    objectCounter++;
    objects.push({ id: `obj_${objectCounter}`, x, y, ...obj });
    occupied.add(`${x},${y}`);
  };

  if (rng.bool(template.objects.visibleItemChance) && pathEdgeTiles.length > 0) {
    const spot = rng.pick(pathEdgeTiles);
    const tier = rng.pick(["common", "uncommon"] as const);
    place(spot.x, spot.y, {
      type: "item_visible",
      itemId: rng.pick(catalog.items[tier] ?? catalog.items.common),
      label: "Objet posé au sol",
    });
  }

  if (rng.bool(template.objects.hiddenItemChance) && offPathTiles.length > 0) {
    const spot = rng.pick(offPathTiles);
    place(spot.x, spot.y, {
      type: "item_hidden",
      itemId: rng.pick(catalog.items.uncommon.concat(catalog.items.rare)),
      hidden: true,
      label: "Objet caché (zone à l'écart du chemin)",
    });
  }

  if (rng.bool(template.objects.signChance) && pathEdgeTiles.length > 0) {
    const spot = rng.pick(pathEdgeTiles);
    place(spot.x, spot.y, {
      type: "sign",
      label: rng.pick(Object.values(catalog.signs)),
    });
  }

  return objects;
}
