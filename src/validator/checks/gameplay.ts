import type { GameMap } from "../../schema/types.js";
import { computeReachableSet, floodFillReachable } from "../../generator/grid.js";
import type { CheckResult } from "./technical.js";

/**
 * Vérifications gameplay (section 13) : accessibilité de toutes les
 * sorties/objets/PNJ/bâtiments depuis les points d'entrée de la map, via
 * flood-fill sur la grille de collision réelle. Utilise EXACTEMENT le même
 * calcul d'atteignabilité (computeReachableSet) que le générateur au moment
 * du placement — sans quoi une case jugée "sûre" pendant la génération
 * pourrait être jugée injoignable ici, pour deux définitions différentes
 * du même concept.
 */
export function checkGameplay(map: GameMap): CheckResult {
  const errors: string[] = [];
  const warnings: string[] = [];
  const { width, height } = map.dimensions;
  const solid = (x: number, y: number) => map.collision[y]?.[x] ?? true;

  const reachableUnion = computeReachableSet(
    width,
    height,
    solid,
    map.warps.map((w) => ({ x: w.x, y: w.y })),
  );

  const checkReachable = (x: number, y: number, label: string) => {
    if (solid(x, y)) {
      errors.push(`${label} est posé sur une tuile solide (inaccessible par construction).`);
      return;
    }
    if (!reachableUnion.has(`${x},${y}`)) {
      errors.push(`${label} en (${x},${y}) n'est atteignable depuis aucun point d'entrée de la map.`);
    }
  };

  // Un meuble fixe (comptoir, PC, machine) est volontairement posé sur une
  // tuile solide — comme dans les jeux Pokémon, on l'utilise depuis une
  // case adjacente, on ne marche jamais dessus.
  const FIXTURE_TYPES = new Set(["pc", "counter", "machine"]);
  const checkAdjacentReachable = (x: number, y: number, label: string) => {
    const neighbors = [
      { x: x + 1, y },
      { x: x - 1, y },
      { x, y: y + 1 },
      { x, y: y - 1 },
    ];
    const ok = neighbors.some((n) => !solid(n.x, n.y) && reachableUnion.has(`${n.x},${n.y}`));
    if (!ok) errors.push(`${label} en (${x},${y}) n'a aucune case adjacente accessible pour l'utiliser.`);
  };

  for (const w of map.warps) checkReachable(w.x, w.y, `Warp "${w.id}"`);
  for (const n of map.npcs) checkReachable(n.x, n.y, `PNJ "${n.id}"`);
  for (const o of map.objects) {
    if (FIXTURE_TYPES.has(o.type)) {
      checkAdjacentReachable(o.x, o.y, `Meuble "${o.id}" (${o.type})`);
      continue;
    }
    // Un objet caché peut être derrière un obstacle mineur, mais jamais totalement isolé.
    checkReachable(o.x, o.y, `Objet "${o.id}"${o.hidden ? " (caché)" : ""}`);
  }
  for (const b of map.buildings) checkReachable(b.entrance.x, b.entrance.y, `Entrée du bâtiment "${b.id}"`);
  for (const l of map.landmarks) checkReachable(l.x, l.y, `Landmark "${l.id}"`);

  // Détection de poches isolées : composantes connexes de tuiles non-solides
  // qui ne contiennent ni warp ni connexion — signe d'une zone créée mais
  // jamais reliée au reste de la carte (cul-de-sac involontaire à grande échelle).
  const visitedGlobal = new Set<string>();
  let pocketCount = 0;
  for (let y = 1; y < height - 1; y++) {
    for (let x = 1; x < width - 1; x++) {
      const k = `${x},${y}`;
      if (solid(x, y) || visitedGlobal.has(k) || reachableUnion.has(k)) continue;
      const pocket = floodFillReachable(width, height, solid, { x, y });
      for (const p of pocket) visitedGlobal.add(p);
      if (pocket.size >= 4) pocketCount++;
    }
  }
  if (pocketCount > 0) {
    warnings.push(`${pocketCount} poche(s) de terrain praticable isolée(s) du reste de la carte détectée(s) (zone potentiellement inaccessible).`);
  }

  return { errors, warnings };
}
