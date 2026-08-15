import type { GameMap } from "../schema/types.js";
import { checkTechnical } from "./checks/technical.js";
import { checkGameplay } from "./checks/gameplay.js";
import { checkDesign } from "./checks/design.js";
import { loadTemplate, isOverworldType } from "../data/loader.js";
import type { MapTemplate } from "../schema/template.js";
import { checkConnectionsConsistency } from "../world/worldGraph.js";

export type ChecklistKey = "Dimensions" | "Tileset" | "Collisions" | "Warps" | "NPCs" | "Objects" | "Encounters";

export interface ValidationReport {
  mapId: string;
  mapName: string;
  checklist: Record<ChecklistKey, boolean>;
  warnings: string[];
  errors: string[];
  passed: boolean;
}

function has(msgs: string[], ...needles: string[]): boolean {
  return msgs.some((m) => needles.some((n) => m.includes(n)));
}

function checkEncounters(map: GameMap): { errors: string[]; warnings: string[] } {
  const errors: string[] = [];
  const warnings: string[] = [];
  if (!isOverworldType(map.metadata.kind)) return { errors, warnings };
  let template: MapTemplate;
  try {
    template = loadTemplate(map.metadata.kind) as MapTemplate;
  } catch {
    return { errors, warnings };
  }
  const methodCount = Object.keys(map.encounters).length;
  if (template.encounters.enabled && methodCount === 0) {
    errors.push(`Rencontres attendues (biome "${map.metadata.kind}") mais table de rencontres vide.`);
  }
  if (!template.encounters.enabled && methodCount > 0) {
    errors.push(`Table de rencontres non vide sur un biome qui ne devrait pas en avoir ("${map.metadata.kind}").`);
  }
  for (const [method, slots] of Object.entries(map.encounters)) {
    for (const slot of slots ?? []) {
      if (slot.levelMin > slot.levelMax) {
        errors.push(`Rencontre ${method}/${slot.pokemon}: levelMin (${slot.levelMin}) > levelMax (${slot.levelMax}).`);
      }
    }
  }
  return { errors, warnings };
}

/**
 * Exécute l'ensemble des vérifications (section 13) et produit un rapport
 * structuré. `worldMaps` (optionnel) permet en plus de vérifier la
 * cohérence des connexions inter-maps (section 11).
 */
export function validateMap(map: GameMap, worldMaps?: GameMap[]): ValidationReport {
  const technical = checkTechnical(map);
  const gameplay = checkGameplay(map);
  const design = checkDesign(map);
  const encounters = checkEncounters(map);

  const errors = [...technical.errors, ...gameplay.errors, ...encounters.errors];
  const warnings = [...technical.warnings, ...gameplay.warnings, ...design.warnings, ...encounters.warnings];

  if (worldMaps && worldMaps.length > 0) {
    const consistency = checkConnectionsConsistency(worldMaps.some((m) => m.metadata.id === map.metadata.id) ? worldMaps : [map, ...worldMaps]);
    for (const e of consistency.errors) {
      if (e.startsWith(map.metadata.id)) errors.push(e);
    }
  }

  const checklist: Record<ChecklistKey, boolean> = {
    Dimensions: !has(errors, "Terrain:", "Terrain ligne", "Collision:", "Collision ligne"),
    Tileset: !has(errors, "Tileset", "référencée dans le terrain"),
    Collisions: !has(errors, "Collision:", "Collision ligne") && !has(warnings, "collision ne correspond pas"),
    Warps: !has(errors, "Warp ", "connexion"),
    NPCs: !has(errors, "PNJ "),
    Objects: !has(errors, "Objet ", "Meuble "),
    Encounters: !has(errors, "Rencontre"),
  };

  return {
    mapId: map.metadata.id,
    mapName: map.metadata.name,
    checklist,
    warnings,
    errors,
    passed: errors.length === 0,
  };
}
