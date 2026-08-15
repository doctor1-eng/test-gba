import { Rng, deriveSeed } from "./rng.js";
import type { GameMap, Npc, Direction } from "../schema/types.js";
import type { InteriorTemplate } from "../schema/template.js";
import { generateInteriorTerrain } from "./terrain/interior.js";
import { loadNpcArchetypes, loadTileset, tilesetLegend } from "../data/loader.js";

export interface BuiltInterior {
  map: GameMap;
  /** Warp d'entrée côté intérieur (là où le joueur apparaît en entrant). */
  entryWarpId: string;
}

/**
 * Assemble une map d'intérieur complète (pièce + mobilier + PNJ) à partir
 * d'un template d'intérieur. Toujours une seule pièce avec une porte au
 * sud reliée au warp extérieur du bâtiment.
 */
export function buildInteriorMap(
  rng: Rng,
  interiorTemplate: InteriorTemplate,
  parentMapId: string,
  parentWarpId: string,
  buildingId: string,
  buildingLabel: string,
  region: string,
  progression: GameMap["metadata"]["progression"],
): BuiltInterior {
  const interiorSeed = deriveSeed(rng.int(0, 2 ** 31), `${buildingId}_interior`);
  const interiorRng = new Rng(interiorSeed);

  const terrain = generateInteriorTerrain(interiorRng, interiorTemplate);
  const tileset = loadTileset(interiorTemplate.tileset);
  const legend = tilesetLegend(tileset);
  const isSolid = (tile: string) => legend.get(tile)?.solid ?? false;

  const collision = terrain.terrain.map((row) => row.map((t) => isSolid(t)));

  const mapId = `${buildingId}_interior`;
  const entryWarpId = "w_entry";

  const npcs: Npc[] = [];
  const archetypes = loadNpcArchetypes();
  const pool = interiorTemplate.npcs.archetypePool.filter((a) => archetypes[a]);
  const npcCount = interiorRng.int(interiorTemplate.npcs.countMin, interiorTemplate.npcs.countMax);
  for (let i = 0; i < npcCount && pool.length > 0; i++) {
    const archetypeName = interiorRng.pick(pool);
    const archetype = archetypes[archetypeName];
    const spot = i === 0 ? terrain.npcSpot : { x: terrain.npcSpot.x + i, y: terrain.npcSpot.y };
    npcs.push({
      id: `npc_${mapId}_${i + 1}`,
      archetype: archetypeName,
      role: archetype.role as Npc["role"],
      name: `${archetype.role}_${i + 1}`,
      x: spot.x,
      y: spot.y,
      direction: "south" as Direction,
      movement: { type: archetype.movement },
      dialogue: interiorRng.shuffle(archetype.dialoguePool).slice(0, 1),
    });
  }

  const map: GameMap = {
    metadata: {
      id: mapId,
      name: buildingLabel,
      kind: interiorTemplate.type as GameMap["metadata"]["kind"],
      region,
      progression,
      tileset: interiorTemplate.tileset,
      seed: interiorSeed,
      generatedAt: new Date().toISOString(),
      description: interiorTemplate.designIntent.playerMotivation,
    },
    dimensions: { width: terrain.width, height: terrain.height },
    terrain: terrain.terrain,
    collision,
    connections: [],
    warps: [
      {
        id: entryWarpId,
        x: terrain.door.x,
        y: terrain.door.y,
        direction: "south",
        destinationMap: parentMapId,
        destinationWarpId: parentWarpId,
      },
    ],
    npcs,
    objects: terrain.furnitureSpots.map((f, i) => ({
      id: `furn_${mapId}_${i + 1}`,
      type: (f.tile === "pc_console" ? "pc" : f.tile === "counter" ? "counter" : "machine") as GameMap["objects"][number]["type"],
      x: f.x,
      y: f.y,
      label: f.tile,
    })),
    buildings: [],
    decorations: [],
    landmarks: [],
    encounters: {},
    events: [],
  };

  return { map, entryWarpId };
}
