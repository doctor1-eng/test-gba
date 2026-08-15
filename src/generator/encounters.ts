import type { Rng } from "./rng.js";
import type { EncounterMethod, EncounterSlot, EncounterTable, ProgressionTier } from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import type { SpeciesEncounters } from "../data/loader.js";

const LEVEL_RANGE_BY_TIER: Record<ProgressionTier, [number, number]> = {
  early: [2, 9],
  mid: [14, 27],
  late: [30, 48],
};

/**
 * Construit une table de rencontres cohérente avec le biome et le palier
 * de progression du joueur (section 10) — jamais un tirage arbitraire
 * d'espèces hors-contexte.
 */
export function buildEncounterTable(
  rng: Rng,
  template: MapTemplate,
  progression: ProgressionTier,
  species: SpeciesEncounters,
): EncounterTable {
  if (!template.encounters.enabled || !template.encounters.biomeKey) return {};

  const biomeData = species[template.encounters.biomeKey];
  if (!biomeData) return {};
  const tierData = biomeData[progression] ?? biomeData.early;
  if (!tierData) return {};

  const [tierMin, tierMax] = LEVEL_RANGE_BY_TIER[progression];
  const table: EncounterTable = {};

  for (const method of template.encounters.methods ?? []) {
    const slots = tierData[method as EncounterMethod];
    if (!slots || slots.length === 0) continue;
    table[method as EncounterMethod] = slots.map((slot): EncounterSlot => {
      const spread = rng.int(1, 3);
      const levelMin = Math.max(1, tierMin + rng.int(-1, 1));
      const levelMax = Math.min(100, Math.max(levelMin + spread, tierMax));
      return {
        pokemon: slot.pokemon,
        rarity: slot.rarity,
        weight: slot.weight,
        levelMin,
        levelMax: Math.min(levelMax, tierMax + 4),
      };
    });
  }
  return table;
}
