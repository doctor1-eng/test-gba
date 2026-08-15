import fs from "node:fs";
import path from "node:path";
import { ASSETS_DIR, DATA_DIR, TEMPLATES_DIR } from "../paths.js";
import type { Tileset } from "../schema/types.js";
import { tilesetSchema } from "../schema/validateSchema.js";
import type { InteriorTemplate, MapTemplate } from "../schema/template.js";

const jsonCache = new Map<string, unknown>();

function readJson<T>(filePath: string): T {
  const cached = jsonCache.get(filePath);
  if (cached) return cached as T;
  const raw = fs.readFileSync(filePath, "utf-8");
  const parsed = JSON.parse(raw) as T;
  jsonCache.set(filePath, parsed);
  return parsed;
}

const OVERWORLD_TYPES = new Set([
  "town",
  "village",
  "route",
  "forest",
  "cave",
  "mountain",
  "beach",
  "special",
]);
const INTERIOR_TYPES = new Set(["pokemon_center", "pokemart", "gym", "house", "lab"]);

export function loadTemplate(type: string): MapTemplate | InteriorTemplate {
  const dir = path.join(TEMPLATES_DIR, type);
  const filePath = path.join(dir, "template.json");
  if (!fs.existsSync(filePath)) {
    throw new Error(`Template introuvable pour le type "${type}" (attendu: ${filePath})`);
  }
  return readJson(filePath);
}

export function isOverworldType(type: string): boolean {
  return OVERWORLD_TYPES.has(type);
}

export function isInteriorType(type: string): boolean {
  return INTERIOR_TYPES.has(type);
}

export function listAvailableTypes(): string[] {
  return fs
    .readdirSync(TEMPLATES_DIR, { withFileTypes: true })
    .filter((d) => d.isDirectory())
    .map((d) => d.name)
    .filter((name) => fs.existsSync(path.join(TEMPLATES_DIR, name, "template.json")));
}

export function loadTileset(id: string): Tileset {
  const filePath = path.join(ASSETS_DIR, "tilesets", `${id}.json`);
  const raw = readJson(filePath);
  const result = tilesetSchema.safeParse(raw);
  if (!result.success) {
    throw new Error(`Tileset "${id}" invalide: ${result.error.message}`);
  }
  return result.data;
}

export function tilesetLegend(tileset: Tileset): Map<string, Tileset["tiles"][number]> {
  return new Map(tileset.tiles.map((t) => [t.id, t]));
}

export interface NpcArchetype {
  role: string;
  spriteColor: string;
  movement: "static" | "wander" | "patrol" | "face_only";
  placement: string;
  dialoguePool: string[];
}

export function loadNpcArchetypes(): Record<string, NpcArchetype> {
  return readJson(path.join(DATA_DIR, "npc-archetypes.json"));
}

export interface ObjectsCatalog {
  items: Record<string, string[]>;
  signs: Record<string, string>;
  berryTrees: string[];
}

export function loadObjectsCatalog(): ObjectsCatalog {
  return readJson(path.join(DATA_DIR, "objects-catalog.json"));
}

export interface SpeciesSlot {
  pokemon: string;
  rarity: "common" | "uncommon" | "rare";
  weight: number;
}

export type SpeciesEncounters = Record<
  string,
  Record<string, Record<string, SpeciesSlot[]>>
>;

export function loadSpeciesEncounters(): SpeciesEncounters {
  return readJson(path.join(DATA_DIR, "species-encounters.json"));
}
