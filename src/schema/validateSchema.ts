import { z } from "zod";

/**
 * Validation structurelle stricte (section 13 "Technique") : la génération
 * elle-même ne devrait jamais produire de données non conformes, mais tout
 * map.json chargé depuis le disque (édité à la main, importé) passe par ici
 * avant d'entrer dans le validateur gameplay/design.
 */

const direction = z.enum(["north", "south", "east", "west"]);

const mapKind = z.enum([
  "town",
  "village",
  "route",
  "forest",
  "cave",
  "mountain",
  "beach",
  "special",
  "pokemon_center",
  "pokemart",
  "gym",
  "house",
  "lab",
]);

const metadataSchema = z.object({
  id: z.string().min(1),
  name: z.string().min(1),
  kind: mapKind,
  region: z.string().min(1),
  progression: z.enum(["early", "mid", "late"]),
  tileset: z.string().min(1),
  music: z.string().optional(),
  weather: z.enum(["clear", "rain", "sand", "fog"]).optional(),
  seed: z.number().int(),
  generatedAt: z.string(),
  description: z.string(),
});

const dimensionsSchema = z.object({
  width: z.number().int().min(5).max(200),
  height: z.number().int().min(5).max(200),
});

const connectionSchema = z.object({
  direction,
  mapId: z.string().min(1),
  offset: z.number().int(),
});

const warpSchema = z.object({
  id: z.string().min(1),
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  direction,
  destinationMap: z.string().min(1),
  destinationWarpId: z.string().min(1),
  isEdgeConnection: z.boolean().optional(),
});

const npcMovementSchema = z.object({
  type: z.enum(["static", "wander", "patrol", "face_only"]),
  radius: z.number().int().min(0).optional(),
  path: z.array(z.object({ x: z.number().int(), y: z.number().int() })).optional(),
});

const npcRole = z.enum([
  "resident",
  "merchant",
  "trainer",
  "guide",
  "narrative",
  "guardian",
  "researcher",
  "child",
  "tourist",
  "nurse",
  "clerk",
  "gym_leader",
]);

const npcSchema = z.object({
  id: z.string().min(1),
  archetype: z.string().min(1),
  role: npcRole,
  name: z.string().min(1),
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  direction,
  movement: npcMovementSchema,
  dialogue: z.array(z.string()).min(1),
  eventId: z.string().optional(),
});

const objectType = z.enum([
  "item_visible",
  "item_hidden",
  "berry_tree",
  "sign",
  "machine",
  "npc_object",
  "cuttable_tree",
  "pushable_rock",
  "pc",
  "counter",
]);

const objectSchema = z.object({
  id: z.string().min(1),
  type: objectType,
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  itemId: z.string().optional(),
  hidden: z.boolean().optional(),
  label: z.string().optional(),
  requiresFlag: z.string().optional(),
});

const buildingSchema = z.object({
  id: z.string().min(1),
  type: z.string().min(1),
  label: z.string().min(1),
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  width: z.number().int().min(1),
  height: z.number().int().min(1),
  entrance: z.object({ x: z.number().int(), y: z.number().int() }),
  interiorMapId: z.string().optional(),
});

const decorationSchema = z.object({
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  type: z.string().min(1),
});

const landmarkSchema = z.object({
  id: z.string().min(1),
  label: z.string().min(1),
  x: z.number().int().min(0),
  y: z.number().int().min(0),
  description: z.string().min(1),
});

const encounterSlotSchema = z.object({
  pokemon: z.string().min(1),
  levelMin: z.number().int().min(1).max(100),
  levelMax: z.number().int().min(1).max(100),
  rarity: z.enum(["common", "uncommon", "rare"]),
  weight: z.number().min(0),
});

const encounterTableSchema = z.object({
  grass: z.array(encounterSlotSchema).optional(),
  water: z.array(encounterSlotSchema).optional(),
  cave: z.array(encounterSlotSchema).optional(),
  fishing: z.array(encounterSlotSchema).optional(),
});

const eventSchema = z.object({
  id: z.string().min(1),
  trigger: z.enum(["flag_set", "item_obtained", "npc_defeated", "enter_zone"]),
  condition: z.string().min(1),
  action: z.enum(["unblock_path", "spawn_npc", "despawn_npc", "reveal_object", "narrative"]),
  description: z.string().min(1),
  targetTiles: z.array(z.object({ x: z.number().int(), y: z.number().int() })).optional(),
});

export const gameMapSchema = z.object({
  metadata: metadataSchema,
  dimensions: dimensionsSchema,
  terrain: z.array(z.array(z.string())),
  collision: z.array(z.array(z.boolean())),
  connections: z.array(connectionSchema),
  warps: z.array(warpSchema),
  npcs: z.array(npcSchema),
  objects: z.array(objectSchema),
  buildings: z.array(buildingSchema),
  decorations: z.array(decorationSchema),
  landmarks: z.array(landmarkSchema),
  encounters: encounterTableSchema,
  events: z.array(eventSchema),
});

export type GameMapParsed = z.infer<typeof gameMapSchema>;

export function parseGameMap(data: unknown) {
  return gameMapSchema.safeParse(data);
}

export const tilesetSchema = z.object({
  id: z.string().min(1),
  label: z.string().min(1),
  description: z.string().min(1),
  tiles: z.array(
    z.object({
      id: z.string().min(1),
      category: z.string().min(1),
      color: z.string().regex(/^#[0-9a-fA-F]{6}$/),
      solid: z.boolean(),
      label: z.string().min(1),
    }),
  ),
});
