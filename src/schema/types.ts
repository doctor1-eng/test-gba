/**
 * Format de carte du pipeline. Modélisé sur le format interne de
 * Pokémon Gen 3 (pokeemerald) : grille de tuiles 16x16, écran 240x160
 * (15x10 tuiles visibles), layers séparés terrain/collision/élévation,
 * "object events" pour NPCs/objets, warps, connections inter-maps.
 */

export type Direction = "north" | "south" | "east" | "west";

export type Biome =
  | "town"
  | "village"
  | "route"
  | "forest"
  | "cave"
  | "mountain"
  | "beach"
  | "special";

export type InteriorType =
  | "pokemon_center"
  | "pokemart"
  | "gym"
  | "house"
  | "lab";

export type MapKind = Biome | InteriorType;

/** Étape de progression du joueur, utilisée pour calibrer niveaux/difficulté. */
export type ProgressionTier = "early" | "mid" | "late";

export const GBA_SCREEN_WIDTH_PX = 240;
export const GBA_SCREEN_HEIGHT_PX = 160;
export const TILE_SIZE_PX = 16;
export const VIEWPORT_TILES_W = GBA_SCREEN_WIDTH_PX / TILE_SIZE_PX; // 15
export const VIEWPORT_TILES_H = GBA_SCREEN_HEIGHT_PX / TILE_SIZE_PX; // 10

export interface MapMetadata {
  id: string;
  name: string;
  kind: MapKind;
  region: string;
  /** Tier utilisé pour calibrer rencontres/dresseurs. */
  progression: ProgressionTier;
  tileset: string;
  music?: string;
  weather?: "clear" | "rain" | "sand" | "fog";
  /** Graine utilisée pour la génération — garantit la reproductibilité. */
  seed: number;
  generatedAt: string;
  description: string;
}

export interface MapDimensions {
  width: number;
  height: number;
}

export interface Connection {
  direction: Direction;
  mapId: string;
  /** Décalage en tuiles entre le bord local et le bord de la map voisine. */
  offset: number;
}

export interface Warp {
  id: string;
  x: number;
  y: number;
  direction: Direction;
  destinationMap: string;
  destinationWarpId: string;
  /** true si ce warp est une sortie de map par bord (auto-généré par connection). */
  isEdgeConnection?: boolean;
}

export type NpcRole =
  | "resident"
  | "merchant"
  | "trainer"
  | "guide"
  | "narrative"
  | "guardian"
  | "researcher"
  | "child"
  | "tourist"
  | "nurse"
  | "clerk"
  | "gym_leader";

export interface NpcMovement {
  type: "static" | "wander" | "patrol" | "face_only";
  radius?: number;
  path?: Array<{ x: number; y: number }>;
}

export interface Npc {
  id: string;
  archetype: string;
  role: NpcRole;
  name: string;
  x: number;
  y: number;
  direction: Direction;
  movement: NpcMovement;
  dialogue: string[];
  /** Identifiant d'événement associé (quête, combat, don d'objet), optionnel. */
  eventId?: string;
}

export type ObjectType =
  | "item_visible"
  | "item_hidden"
  | "berry_tree"
  | "sign"
  | "machine"
  | "npc_object"
  | "cuttable_tree"
  | "pushable_rock"
  | "pc"
  | "counter";

export interface MapObject {
  id: string;
  type: ObjectType;
  x: number;
  y: number;
  itemId?: string;
  /** Objets cachés : trouvables uniquement par interaction/recherche. */
  hidden?: boolean;
  label?: string;
  /** Flag de progression requis pour interagir (ex: badge, item-clé). */
  requiresFlag?: string;
}

export interface Building {
  id: string;
  type: InteriorType | "generic";
  label: string;
  x: number;
  y: number;
  width: number;
  height: number;
  entrance: { x: number; y: number };
  interiorMapId?: string;
}

export interface Decoration {
  x: number;
  y: number;
  type: string;
}

export interface Landmark {
  id: string;
  label: string;
  x: number;
  y: number;
  description: string;
}

export type EncounterMethod = "grass" | "water" | "cave" | "fishing";

export interface EncounterSlot {
  pokemon: string;
  levelMin: number;
  levelMax: number;
  rarity: "common" | "uncommon" | "rare";
  /** Poids relatif dans la table (somme documentée dans le générateur). */
  weight: number;
}

export type EncounterTable = Partial<Record<EncounterMethod, EncounterSlot[]>>;

export interface GameEvent {
  id: string;
  trigger: "flag_set" | "item_obtained" | "npc_defeated" | "enter_zone";
  condition: string;
  action: "unblock_path" | "spawn_npc" | "despawn_npc" | "reveal_object" | "narrative";
  description: string;
  /** Zone affectée si action = unblock_path (coordonnées tuile). */
  targetTiles?: Array<{ x: number; y: number }>;
}

export interface GameMap {
  metadata: MapMetadata;
  dimensions: MapDimensions;
  /** Grille [y][x] d'ids de tuiles sémantiques (voir assets/tilesets). */
  terrain: string[][];
  /** Grille [y][x] : true = tuile solide (non franchissable). */
  collision: boolean[][];
  connections: Connection[];
  warps: Warp[];
  npcs: Npc[];
  objects: MapObject[];
  buildings: Building[];
  decorations: Decoration[];
  landmarks: Landmark[];
  encounters: EncounterTable;
  events: GameEvent[];
}

export interface TilesetTile {
  id: string;
  category: string;
  color: string;
  solid: boolean;
  label: string;
}

export interface Tileset {
  id: string;
  label: string;
  description: string;
  tiles: TilesetTile[];
}
