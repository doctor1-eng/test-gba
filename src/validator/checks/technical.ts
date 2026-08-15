import type { GameMap } from "../../schema/types.js";
import { loadTileset, tilesetLegend, isOverworldType } from "../../data/loader.js";

export interface CheckResult {
  errors: string[];
  warnings: string[];
}

/** Vérifications techniques (section 13) : dimensions, tileset, collisions, références. */
export function checkTechnical(map: GameMap): CheckResult {
  const errors: string[] = [];
  const warnings: string[] = [];
  const { width, height } = map.dimensions;

  if (map.terrain.length !== height) {
    errors.push(`Terrain: ${map.terrain.length} lignes, attendu ${height} (dimensions.height).`);
  }
  map.terrain.forEach((row, y) => {
    if (row.length !== width) errors.push(`Terrain ligne ${y}: ${row.length} colonnes, attendu ${width}.`);
  });
  if (map.collision.length !== height) {
    errors.push(`Collision: ${map.collision.length} lignes, attendu ${height}.`);
  }
  map.collision.forEach((row, y) => {
    if (row.length !== width) errors.push(`Collision ligne ${y}: ${row.length} colonnes, attendu ${width}.`);
  });

  let tileset;
  try {
    tileset = loadTileset(map.metadata.tileset);
  } catch (e) {
    errors.push(`Tileset "${map.metadata.tileset}" introuvable ou invalide: ${(e as Error).message}`);
  }
  if (tileset) {
    const legend = tilesetLegend(tileset);
    const unknownTiles = new Set<string>();
    for (const row of map.terrain) {
      for (const tileId of row) {
        if (!legend.has(tileId)) unknownTiles.add(tileId);
      }
    }
    for (const t of unknownTiles) errors.push(`Tuile "${t}" référencée dans le terrain mais absente du tileset "${tileset.id}".`);

    // Cohérence collision ↔ tileset : une tuile marquée solide dans le tileset
    // doit avoir collision=true, et inversement pour les tuiles non-solides.
    let mismatches = 0;
    for (let y = 0; y < map.terrain.length; y++) {
      for (let x = 0; x < (map.terrain[y]?.length ?? 0); x++) {
        const tile = legend.get(map.terrain[y][x]);
        const collisionHere = map.collision[y]?.[x];
        if (tile && collisionHere !== undefined && tile.solid !== collisionHere) mismatches++;
      }
    }
    if (mismatches > 0) warnings.push(`${mismatches} tuile(s) où la collision ne correspond pas à la solidité déclarée dans le tileset.`);
  }

  const inBounds = (x: number, y: number) => x >= 0 && x < width && y >= 0 && y < height;

  for (const w of map.warps) {
    if (!inBounds(w.x, w.y)) errors.push(`Warp "${w.id}" hors limites (${w.x},${w.y}).`);
    if (!w.destinationMap) errors.push(`Warp "${w.id}" sans destinationMap.`);
  }
  for (const n of map.npcs) {
    if (!inBounds(n.x, n.y)) errors.push(`PNJ "${n.id}" hors limites (${n.x},${n.y}).`);
  }
  for (const o of map.objects) {
    if (!inBounds(o.x, o.y)) errors.push(`Objet "${o.id}" hors limites (${o.x},${o.y}).`);
  }
  for (const b of map.buildings) {
    if (!inBounds(b.entrance.x, b.entrance.y)) errors.push(`Entrée du bâtiment "${b.id}" hors limites.`);
    if (b.type !== "generic" && isOverworldType(map.metadata.kind) && !b.interiorMapId) {
      warnings.push(`Bâtiment "${b.id}" (${b.type}) n'a pas d'intérieur généré.`);
    }
  }
  for (const l of map.landmarks) {
    if (!inBounds(l.x, l.y)) errors.push(`Landmark "${l.id}" hors limites (${l.x},${l.y}).`);
  }

  return { errors, warnings };
}
