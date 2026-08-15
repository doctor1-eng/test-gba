/**
 * Forme d'un template de la bibliothèque `templates/`. Un template ne
 * contient jamais de placement final — seulement des règles/contraintes
 * que le générateur doit respecter (section 12 : biome + topologie +
 * règles de gameplay + progression + narration + décoration = map).
 */

export interface DesignIntent {
  /** Pourquoi le joueur est ici / ce qu'il doit ressentir (section 18). */
  playerMotivation: string;
  attraction: string;
  emotion: string;
}

export interface DimensionRules {
  minWidth: number;
  maxWidth: number;
  minHeight: number;
  maxHeight: number;
}

export interface BuildingRules {
  /** Types obligatoires (ex: pokemon_center dans toute ville). */
  required: string[];
  /** Types tirés pour compléter jusqu'à densité cible. */
  optionalPool: string[];
  minCount: number;
  maxCount: number;
}

export interface DecorationRules {
  /** Densité (0-1) par zone sémantique. */
  densityByZone: Record<string, number>;
  allowed: string[];
}

export interface NpcRules {
  countMin: number;
  countMax: number;
  archetypePool: string[];
}

export interface ObjectRules {
  hiddenItemChance: number;
  visibleItemChance: number;
  signChance: number;
}

export interface LandmarkRules {
  minCount: number;
  maxCount: number;
  pool: string[];
}

export interface EncounterRules {
  enabled: boolean;
  /** Clé dans src/data/species-encounters.json */
  biomeKey?: string;
  methods?: string[];
}

export interface PathRules {
  mainWidth: number;
  secondaryPathChance: number;
  windiness: number;
}

export interface MapTemplate {
  type: string;
  label: string;
  tileset: string;
  designIntent: DesignIntent;
  dimensions: DimensionRules;
  path: PathRules;
  buildings: BuildingRules;
  decorations: DecorationRules;
  npcs: NpcRules;
  objects: ObjectRules;
  landmarks: LandmarkRules;
  encounters: EncounterRules;
}

export interface InteriorTemplate {
  type: string;
  label: string;
  tileset: string;
  designIntent: DesignIntent;
  dimensions: DimensionRules;
  npcs: NpcRules;
  objects: ObjectRules;
  furniture: string[];
}
