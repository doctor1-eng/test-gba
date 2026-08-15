import type { Rng } from "./rng.js";
import type { Direction, Npc } from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import type { NpcArchetype } from "../data/loader.js";
import type { TerrainResult } from "./terrain/common.js";
import type { Point } from "./grid.js";

function candidateSpots(
  terrainResult: TerrainResult,
  placement: string,
  buildings: Array<{ x: number; y: number; width: number; height: number }>,
  isSolid: (tile: string) => boolean,
  occupied: Set<string>,
): Point[] {
  const { width, height, terrain, mainPath, landmarkSpot, edgeAnchors } = terrainResult;
  const spots: Point[] = [];
  const push = (x: number, y: number) => {
    const k = `${x},${y}`;
    if (x <= 0 || y <= 0 || x >= width - 1 || y >= height - 1) return;
    if (occupied.has(k) || mainPath.has(k)) return;
    if (isSolid(terrain[y][x])) return;
    spots.push({ x, y });
  };

  if (placement === "near_houses" && buildings.length > 0) {
    for (const b of buildings) {
      push(b.x + b.width + 1, b.y + Math.floor(b.height / 2));
      push(b.x - 1, b.y + Math.floor(b.height / 2));
    }
  } else if (placement === "on_route") {
    for (const k of mainPath) {
      const [x, y] = k.split(",").map(Number);
      push(x + 1, y);
      push(x - 1, y);
    }
  } else if (placement === "near_landmark" || placement === "landmark_adjacent") {
    for (let dy = -2; dy <= 2; dy++) for (let dx = -2; dx <= 2; dx++) push(landmarkSpot.x + dx, landmarkSpot.y + dy);
  } else if (placement === "near_entrance") {
    for (const dir of Object.keys(edgeAnchors) as Direction[]) {
      const a = edgeAnchors[dir];
      if (a) {
        push(a.x + 1, a.y);
        push(a.x - 1, a.y);
        push(a.x, a.y + 1);
      }
    }
  } else if (placement === "blocking_path") {
    for (const k of mainPath) {
      const [x, y] = k.split(",").map(Number);
      spots.push({ x, y }); // volontairement sur le chemin (garde de progression)
    }
  }

  if (spots.length === 0) {
    for (let y = 1; y < height - 1; y++)
      for (let x = 1; x < width - 1; x++) push(x, y);
  }
  return spots;
}

export function placeNpcs(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  archetypes: Record<string, NpcArchetype>,
  buildings: Array<{ x: number; y: number; width: number; height: number }>,
  isSolid: (tile: string) => boolean,
  occupied: Set<string>,
): Npc[] {
  const count = rng.int(template.npcs.countMin, template.npcs.countMax);
  const npcs: Npc[] = [];
  const pool = template.npcs.archetypePool.filter((a) => archetypes[a]);
  if (pool.length === 0) return npcs;

  let npcCounter = 0;
  for (let i = 0; i < count; i++) {
    const archetypeName = rng.pick(pool);
    const archetype = archetypes[archetypeName];
    const spots = candidateSpots(terrainResult, archetype.placement, buildings, isSolid, occupied);
    if (spots.length === 0) continue;
    const spot = rng.pick(spots);
    occupied.add(`${spot.x},${spot.y}`);

    const dialogueCount = rng.int(1, Math.min(2, archetype.dialoguePool.length));
    const dialogue = rng.shuffle(archetype.dialoguePool).slice(0, dialogueCount);

    npcCounter++;
    npcs.push({
      id: `npc_${npcCounter}`,
      archetype: archetypeName,
      role: archetype.role as Npc["role"],
      name: `${archetype.role}_${npcCounter}`,
      x: spot.x,
      y: spot.y,
      direction: rng.pick(["north", "south", "east", "west"] as Direction[]),
      movement: { type: archetype.movement, radius: archetype.movement === "wander" ? 2 : undefined },
      dialogue,
      eventId: archetype.placement === "blocking_path" ? `event_guard_${npcCounter}` : undefined,
    });
  }
  return npcs;
}
