import type { GameMap } from "../../schema/types.js";
import { loadTemplate, loadTileset, tilesetLegend, isOverworldType } from "../../data/loader.js";
import type { MapTemplate } from "../../schema/template.js";
import type { CheckResult } from "./technical.js";

/**
 * Vérifications de design (section 13) : densité de décoration, présence
 * de landmarks, répétitions excessives — des heuristiques, pas des règles
 * absolues, d'où des WARNINGS plutôt que des ERRORS.
 */
export function checkDesign(map: GameMap): CheckResult {
  const errors: string[] = [];
  const warnings: string[] = [];

  if (!isOverworldType(map.metadata.kind)) return { errors, warnings };

  let template: MapTemplate;
  try {
    template = loadTemplate(map.metadata.kind) as MapTemplate;
  } catch {
    return { errors, warnings };
  }

  const area = map.dimensions.width * map.dimensions.height;

  // Densité comparée uniquement sur les tuiles réellement éligibles à une
  // décoration (non-solides) : un canopy de forêt est presque entièrement
  // constitué d'arbres solides, comparer sa densité brute à la surface
  // totale de la zone produirait un faux positif "zone trop vide".
  let tileset;
  try {
    tileset = loadTileset(map.metadata.tileset);
  } catch {
    tileset = null;
  }
  const legend = tileset ? tilesetLegend(tileset) : null;
  const isSolid = (tileId: string) => legend?.get(tileId)?.solid ?? false;

  // Les zones sémantiques ne sont pas persistées dans le GameMap final
  // (internes au générateur) : on retombe sur la surface non-solide totale
  // de la carte, point de comparaison plus honnête que la surface brute.
  let eligibleTiles = 0;
  for (const row of map.terrain) for (const t of row) if (!isSolid(t)) eligibleTiles++;

  const decorationDensity = map.decorations.length / Math.max(1, eligibleTiles);
  const avgTargetDensity =
    Object.values(template.decorations.densityByZone).reduce((s, v) => s + v, 0) /
    Math.max(1, Object.values(template.decorations.densityByZone).length);

  if (avgTargetDensity > 0 && decorationDensity < avgTargetDensity * 0.25) {
    warnings.push(
      `Densité de décoration très faible sur la surface praticable (${(decorationDensity * 100).toFixed(1)}% vs ~${(avgTargetDensity * 100).toFixed(1)}% attendu) — la zone risque de paraître vide.`,
    );
  }
  if (avgTargetDensity > 0 && decorationDensity > avgTargetDensity * 2.5) {
    warnings.push(
      `Densité de décoration très élevée sur la surface praticable (${(decorationDensity * 100).toFixed(1)}% vs ~${(avgTargetDensity * 100).toFixed(1)}% attendu) — risque de "mur de décoration".`,
    );
  }

  if (area >= 200 && map.landmarks.length < template.landmarks.minCount) {
    warnings.push(`Aucun landmark mémorable sur une carte de ${area} tuiles — le joueur risque de perdre ses repères.`);
  }

  // Répétition excessive : 5 décorations identiques alignées consécutivement (horizontal ou vertical).
  const byPos = new Map(map.decorations.map((d) => [`${d.x},${d.y}`, d.type]));
  const checkLine = (getPoint: (i: number) => { x: number; y: number }, length: number) => {
    let run = 1;
    let lastType: string | null = null;
    for (let i = 0; i < length; i++) {
      const p = getPoint(i);
      const type = byPos.get(`${p.x},${p.y}`) ?? null;
      if (type && type === lastType) {
        run++;
        if (run === 5) {
          warnings.push(`Répétition de décoration "${type}" alignée sur au moins 5 tuiles près de (${p.x},${p.y}) — motif artificiel probable.`);
        }
      } else {
        run = type ? 1 : 0;
      }
      lastType = type;
    }
  };
  for (let y = 0; y < map.dimensions.height; y++) checkLine((i) => ({ x: i, y }), map.dimensions.width);
  for (let x = 0; x < map.dimensions.width; x++) checkLine((i) => ({ x, y: i }), map.dimensions.height);

  // Routes/forêts trop longues sans landmark intermédiaire (section "warnings" exemple).
  const longAxis = Math.max(map.dimensions.width, map.dimensions.height);
  if ((map.metadata.kind === "route" || map.metadata.kind === "forest") && longAxis > 45 && map.landmarks.length < 2) {
    warnings.push(`Zone longue (${longAxis} tuiles sur l'axe principal) avec un seul landmark — envisager un point de repère intermédiaire.`);
  }

  return { errors, warnings };
}
