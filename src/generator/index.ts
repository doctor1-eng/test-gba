import { Rng } from "./rng.js";
import type {
  Connection,
  Direction,
  GameEvent,
  GameMap,
  ProgressionTier,
} from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import {
  loadNpcArchetypes,
  loadObjectsCatalog,
  loadSpeciesEncounters,
  loadTemplate,
  loadTileset,
  tilesetLegend,
  isOverworldType,
} from "../data/loader.js";
import { computeReachableSet } from "./grid.js";
import type { TerrainResult } from "./terrain/common.js";
import { generateTownTerrain } from "./terrain/town.js";
import { generateVillageTerrain } from "./terrain/village.js";
import { generateRouteTerrain } from "./terrain/route.js";
import { generateForestTerrain } from "./terrain/forest.js";
import { generateCaveTerrain } from "./terrain/cave.js";
import { generateMountainTerrain } from "./terrain/mountain.js";
import { generateBeachTerrain } from "./terrain/beach.js";
import { generateSpecialTerrain } from "./terrain/special.js";
import { placeBuildings } from "./buildings.js";
import { placeDecorations, decorationTileId } from "./decorations.js";
import { placeNpcs } from "./npcs.js";
import { placeObjects } from "./objects.js";
import { placeLandmarks } from "./landmarks.js";
import { buildEncounterTable } from "./encounters.js";

const TERRAIN_GENERATORS: Record<
  string,
  (rng: Rng, w: number, h: number, t: MapTemplate, c: Direction[], isSolid: (tile: string) => boolean) => TerrainResult
> = {
  town: generateTownTerrain,
  village: generateVillageTerrain,
  route: generateRouteTerrain,
  forest: generateForestTerrain,
  cave: generateCaveTerrain,
  mountain: generateMountainTerrain,
  beach: generateBeachTerrain,
  special: generateSpecialTerrain,
};

const DEFAULT_CONNECTIONS: Record<string, Direction[]> = {
  town: ["south"],
  village: ["south"],
  route: ["north", "south"],
  forest: ["north", "south"],
  cave: ["north", "south"],
  mountain: ["north", "south"],
  beach: ["west"],
  special: ["south"],
};

export interface ConnectionSpec {
  direction: Direction;
  /** Map voisine déjà connue (posée par le graphe monde). Laisser vide = sortie non câblée, signalée par le validateur. */
  mapId?: string;
  offset?: number;
}

export interface GenerateMapOptions {
  type: string;
  name: string;
  id?: string;
  seed?: number;
  region?: string;
  progression?: ProgressionTier;
  connections?: ConnectionSpec[];
  width?: number;
  height?: number;
}

export interface GenerateMapResult {
  map: GameMap;
  interiorMaps: GameMap[];
}

function slugify(name: string): string {
  return name
    .toLowerCase()
    .normalize("NFD")
    .replace(/[̀-ͯ]/g, "")
    .replace(/[^a-z0-9]+/g, "_")
    .replace(/^_+|_+$/g, "");
}

export function generateMap(opts: GenerateMapOptions): GenerateMapResult {
  if (!isOverworldType(opts.type)) {
    throw new Error(
      `generateMap() ne gère que les biomes extérieurs (${Object.keys(TERRAIN_GENERATORS).join(", ")}). ` +
        `Les intérieurs (${opts.type}) sont générés automatiquement via les bâtiments d'une ville/village.`,
    );
  }
  const generatorFn = TERRAIN_GENERATORS[opts.type];
  if (!generatorFn) throw new Error(`Type de biome inconnu: ${opts.type}`);

  const template = loadTemplate(opts.type) as MapTemplate;
  const seed = opts.seed ?? Math.floor(Math.random() * 2 ** 31);
  const rng = new Rng(seed);
  const id = opts.id ?? slugify(opts.name);
  const region = opts.region ?? "Région Démo";
  const progression: ProgressionTier = opts.progression ?? "early";

  const width = opts.width ?? rng.int(template.dimensions.minWidth, template.dimensions.maxWidth);
  const height = opts.height ?? rng.int(template.dimensions.minHeight, template.dimensions.maxHeight);

  const connectionSpecs: ConnectionSpec[] =
    opts.connections ?? DEFAULT_CONNECTIONS[opts.type].map((direction): ConnectionSpec => ({ direction }));
  const directions = connectionSpecs.map((c) => c.direction);

  const tileset = loadTileset(template.tileset);
  const legend = tilesetLegend(tileset);
  const isSolid = (tile: string) => legend.get(tile)?.solid ?? false;

  const terrainResult = generatorFn(rng, width, height, template, directions, isSolid);

  const occupied = new Set<string>();
  // Le(s) landmark(s) réservent leur case AVANT le placement des bâtiments :
  // sans ça, un bâtiment peut recouvrir la tuile choisie par le générateur
  // de terrain pour un landmark (choisie sans connaître les bâtiments à
  // venir — ex: la berge d'un étang, restée non-solide exprès), et le
  // landmark finirait posé sous un mur. Les réservations sont retirées
  // juste après (leur rôle était d'écarter les bâtiments) pour ne pas
  // fausser la recherche de repli de placeLandmarks juste en dessous.
  const reservedLandmarkKeys = [
    `${terrainResult.landmarkSpot.x},${terrainResult.landmarkSpot.y}`,
    ...(terrainResult.extraLandmarkSpots ?? []).map((s) => `${s.point.x},${s.point.y}`),
  ];
  for (const k of reservedLandmarkKeys) occupied.add(k);

  // 1) Bâtiments : mutent le terrain (footprint + porte) avant tout le reste.
  const { buildings, interiorMaps, exteriorWarps } = placeBuildings(
    rng,
    terrainResult,
    template,
    id,
    region,
    progression,
    occupied,
    isSolid,
  );
  for (const k of reservedLandmarkKeys) occupied.delete(k);

  // Ensemble des tuiles réellement atteignables, calculé une seule fois
  // après la pose des bâtiments (dernière étape qui ajoute des obstacles
  // majeurs). Sert de garde-fou pour toutes les passes de placement
  // suivantes : "non-solide" ne suffit pas, une case cernée par une frange
  // d'arbres, un étang ou un bâtiment reste inutilisable même si elle n'est
  // techniquement pas un mur. Calculé EXACTEMENT comme le validateur le
  // recalculera (mêmes points d'entrée : les warps de bâtiment) — sans
  // quoi une case jugée "sûre" ici pourrait être jugée injoignable
  // ensuite, pour deux définitions différentes de la même notion.
  const isSolidAt = (x: number, y: number) => isSolid(terrainResult.terrain[y]?.[x] ?? "");
  const reachable = computeReachableSet(
    terrainResult.width,
    terrainResult.height,
    isSolidAt,
    exteriorWarps.map((w) => ({ x: w.x, y: w.y })),
  );

  // 2) Landmarks : réservent leur case avant la passe décoration.
  const landmarks = placeLandmarks(rng, terrainResult, template, isSolidAt, occupied, reachable);

  // 3) Décorations : remplissent le reste selon les densités de zone.
  const decorations = placeDecorations(rng, terrainResult, template, isSolid, occupied);

  // 4) PNJ, positionnés par rôle/archétype.
  const archetypes = loadNpcArchetypes();
  const npcs = placeNpcs(rng, terrainResult, template, archetypes, buildings, isSolid, occupied, reachable);

  // 5) Objets interactifs.
  const catalog = loadObjectsCatalog();
  const objects = placeObjects(rng, terrainResult, template, catalog, isSolid, occupied, reachable);

  // 6) Rencontres sauvages.
  const species = loadSpeciesEncounters();
  const encounters = buildEncounterTable(rng, template, progression, species);

  // 7) Événements de progression (gardien bloquant le passage jusqu'à un flag).
  const events: GameEvent[] = [];
  for (const npc of npcs) {
    if (npc.eventId) {
      events.push({
        id: npc.eventId,
        trigger: "flag_set",
        condition: `${id}_access_granted`,
        action: "despawn_npc",
        description: `${npc.name} bloque le passage jusqu'à l'obtention du flag "${id}_access_granted".`,
        targetTiles: [{ x: npc.x, y: npc.y }],
      });
    }
  }

  // Collision calculée après toute mutation du terrain (bâtiments compris).
  const collision = terrainResult.terrain.map((row) => row.map((t) => isSolid(t)));

  const connections: Connection[] = connectionSpecs
    .filter((c): c is ConnectionSpec & { mapId: string } => !!c.mapId)
    .map((c) => ({ direction: c.direction, mapId: c.mapId, offset: c.offset ?? 0 }));

  const map: GameMap = {
    metadata: {
      id,
      name: opts.name,
      kind: opts.type as GameMap["metadata"]["kind"],
      region,
      progression,
      tileset: template.tileset,
      seed,
      generatedAt: new Date().toISOString(),
      description: template.designIntent.playerMotivation,
    },
    dimensions: { width, height },
    terrain: terrainResult.terrain,
    collision,
    connections,
    warps: exteriorWarps,
    npcs,
    objects,
    buildings,
    decorations,
    landmarks,
    encounters,
    events,
  };

  return { map, interiorMaps };
}

export { decorationTileId };
