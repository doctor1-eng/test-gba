import fs from "node:fs";
import path from "node:path";
import { WORLD_DIR } from "../paths.js";
import type { Direction, GameMap } from "../schema/types.js";

export interface WorldRegistryEntry {
  id: string;
  name: string;
  kind: string;
  region: string;
  width: number;
  height: number;
}

export interface WorldRegistry {
  maps: Record<string, WorldRegistryEntry>;
}

const REGISTRY_PATH = path.join(WORLD_DIR, "world.json");

export function loadWorldRegistry(): WorldRegistry {
  if (!fs.existsSync(REGISTRY_PATH)) return { maps: {} };
  return JSON.parse(fs.readFileSync(REGISTRY_PATH, "utf-8"));
}

export function saveWorldRegistry(registry: WorldRegistry) {
  fs.mkdirSync(WORLD_DIR, { recursive: true });
  fs.writeFileSync(REGISTRY_PATH, JSON.stringify(registry, null, 2), "utf-8");
}

export function registerMap(registry: WorldRegistry, map: GameMap): WorldRegistry {
  registry.maps[map.metadata.id] = {
    id: map.metadata.id,
    name: map.metadata.name,
    kind: map.metadata.kind,
    region: map.metadata.region,
    width: map.dimensions.width,
    height: map.dimensions.height,
  };
  return registry;
}

export function opposite(dir: Direction): Direction {
  switch (dir) {
    case "north":
      return "south";
    case "south":
      return "north";
    case "east":
      return "west";
    case "west":
      return "east";
  }
}

/**
 * Vérifie que chaque connexion entre deux maps (section 11) est
 * bidirectionnelle et cohérente : la map voisine doit exister et pointer
 * en retour vers la map d'origine, dans la direction opposée.
 */
export function checkConnectionsConsistency(maps: GameMap[]): { errors: string[]; warnings: string[] } {
  const byId = new Map(maps.map((m) => [m.metadata.id, m]));
  const errors: string[] = [];
  const warnings: string[] = [];

  for (const map of maps) {
    for (const conn of map.connections) {
      const neighbor = byId.get(conn.mapId);
      if (!neighbor) {
        errors.push(`${map.metadata.id}: connexion ${conn.direction} → "${conn.mapId}" introuvable dans le monde chargé.`);
        continue;
      }
      const back = neighbor.connections.find((c) => c.mapId === map.metadata.id);
      if (!back) {
        errors.push(
          `${map.metadata.id} → ${neighbor.metadata.id} (${conn.direction}) : connexion non réciproque (rien ne pointe en retour).`,
        );
        continue;
      }
      if (back.direction !== opposite(conn.direction)) {
        errors.push(
          `${map.metadata.id} ↔ ${neighbor.metadata.id} : directions incohérentes (${conn.direction} vs ${back.direction}, attendu ${opposite(conn.direction)}).`,
        );
      }
    }
  }
  return { errors, warnings };
}
