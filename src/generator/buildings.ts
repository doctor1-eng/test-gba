import type { Rng } from "./rng.js";
import type { Building, GameMap, Warp } from "../schema/types.js";
import type { MapTemplate, InteriorTemplate } from "../schema/template.js";
import type { TerrainResult } from "./terrain/common.js";
import { loadTemplate } from "../data/loader.js";
import { buildInteriorMap } from "./interiors.js";
import { floodFillReachable, type Point } from "./grid.js";

// Un bâtiment "required" doit pouvoir se placer de façon quasi certaine —
// une emprise trop grande par rapport à l'espace non-plaza disponible près
// du chemin le condamne à échouer presque systématiquement. Tailles
// resserrées pour rester dans l'ordre de grandeur des autres bâtiments.
const FOOTPRINT: Record<string, { w: number; h: number; label: string }> = {
  pokemon_center: { w: 5, h: 4, label: "Centre Pokémon" },
  pokemart: { w: 5, h: 4, label: "Boutique" },
  gym: { w: 6, h: 5, label: "Arène" },
  house: { w: 4, h: 4, label: "Maison" },
  lab: { w: 5, h: 4, label: "Laboratoire" },
};

export interface BuildingsResult {
  buildings: Building[];
  interiorMaps: GameMap[];
  exteriorWarps: Warp[];
}

/**
 * Place les bâtiments en bordure du réseau de chemins, porte tournée vers
 * le chemin (jamais un bâtiment isolé sans accès). Chaque bâtiment avec
 * intérieur génère sa propre map + la paire de warps qui les relie.
 */
export function placeBuildings(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  mapId: string,
  region: string,
  progression: GameMap["metadata"]["progression"],
  occupied: Set<string>,
  isSolid: (tile: string) => boolean,
): BuildingsResult {
  const { width, height, terrain, zones, mainPath } = terrainResult;
  const buildings: Building[] = [];
  const interiorMaps: GameMap[] = [];
  const exteriorWarps: Warp[] = [];

  const typeQueue = [...template.buildings.required];
  const targetCount = rng.int(
    Math.max(template.buildings.minCount, typeQueue.length),
    Math.max(template.buildings.maxCount, typeQueue.length),
  );
  const pool = rng.shuffle(template.buildings.optionalPool);
  let poolIdx = 0;
  while (typeQueue.length < targetCount && poolIdx < pool.length) {
    typeQueue.push(pool[poolIdx]);
    poolIdx++;
  }

  // Ancrer uniquement sur les tuiles du chemin lui-même laisse trop peu de
  // place non-plaza pour un bâtiment près d'une carte à connexion unique
  // (le chemin y est un simple corridor étroit) : la couronne "path_edge"
  // (déjà réservée à la circulation piétonne autour du chemin) offre
  // beaucoup plus de points d'ancrage valides, toujours à proximité
  // immédiate d'une tuile praticable.
  // Le zonage "path_edge" est posé pendant la génération du terrain et peut
  // devenir obsolète : une frange d'arbres ajoutée après coup mute le
  // terrain sans jamais retoucher `zones` (elle n'a pas à le faire pour
  // son propre usage). Un ancrage doit donc revérifier la solidité réelle
  // de la tuile elle-même, pas seulement son étiquette de zone.
  const pathEdgeTiles: string[] = [];
  for (let y = 1; y < height - 1; y++) {
    for (let x = 1; x < width - 1; x++) {
      if (zones[y][x] === "path_edge" && !mainPath.has(`${x},${y}`) && !isSolid(terrain[y][x])) {
        pathEdgeTiles.push(`${x},${y}`);
      }
    }
  }
  const pathTiles = rng.shuffle([...mainPath, ...pathEdgeTiles]);
  let buildingIndex = 0;

  // Référence de connectivité UNIQUE pour tous les bâtiments de cette carte
  // (jamais l'ancre propre à chaque bâtiment) : deux bâtiments individuellement
  // "sûrs" au sens de leur propre ancre peuvent, ensemble, pincer une zone
  // tierce entre eux. Vérifier chaque pose par rapport au même point de
  // référence rend les vérifications cumulatives : la perte totale reste
  // bornée par la somme des emprises, jamais par un effet de bord caché.
  const referenceKey = mainPath.values().next().value as string | undefined;
  const referencePoint = referenceKey
    ? { x: Number(referenceKey.split(",")[0]), y: Number(referenceKey.split(",")[1]) }
    : null;

  for (const type of typeQueue) {
    const footprint = FOOTPRINT[type];
    if (!footprint) continue;

    const spot = findFootprintSpot(
      pathTiles,
      footprint.w,
      footprint.h,
      width,
      height,
      terrain,
      zones,
      mainPath,
      occupied,
      isSolid,
      referencePoint,
    );
    if (!spot) continue; // pas d'emplacement valide : on ne force jamais un bâtiment sans accès au chemin

    buildingIndex++;
    const buildingId = `${mapId}_b${buildingIndex}_${type}`;

    for (let dy = 0; dy < footprint.h; dy++) {
      for (let dx = 0; dx < footprint.w; dx++) {
        const x = spot.x + dx;
        const y = spot.y + dy;
        terrain[y][x] = dy === footprint.h - 1 ? "building_wall" : "building_roof";
        occupied.add(`${x},${y}`);
      }
    }
    const entrance: Point = { x: spot.x + Math.floor(footprint.w / 2), y: spot.y + footprint.h - 1 };
    terrain[entrance.y][entrance.x] = "door";

    const building: Building = {
      id: buildingId,
      type: type as Building["type"],
      label: footprint.label,
      x: spot.x,
      y: spot.y,
      width: footprint.w,
      height: footprint.h,
      entrance,
    };

    const interiorTemplate = loadTemplate(type) as InteriorTemplate;
    const exteriorWarpId = `w_${buildingId}`;
    const built = buildInteriorMap(
      rng,
      interiorTemplate,
      mapId,
      exteriorWarpId,
      buildingId,
      footprint.label,
      region,
      progression,
    );
    building.interiorMapId = built.map.metadata.id;
    interiorMaps.push(built.map);
    exteriorWarps.push({
      id: exteriorWarpId,
      x: entrance.x,
      y: entrance.y,
      direction: "north",
      destinationMap: built.map.metadata.id,
      destinationWarpId: built.entryWarpId,
    });

    buildings.push(building);
  }

  return { buildings, interiorMaps, exteriorWarps };
}

function findFootprintSpot(
  pathTiles: Array<{ x: number; y: number } | string>,
  w: number,
  h: number,
  width: number,
  height: number,
  terrain: string[][],
  zones: string[][],
  mainPath: Set<string>,
  occupied: Set<string>,
  isSolid: (tile: string) => boolean,
  referencePoint: Point | null,
): Point | null {
  const solidNow = (x: number, y: number) => isSolid(terrain[y]?.[x] ?? "");
  // Calculée une seule fois : le terrain ne change pas entre deux essais
  // d'emplacement pour un même bâtiment.
  const before = referencePoint ? floodFillReachable(width, height, solidNow, referencePoint).size : 0;

  for (const t of pathTiles) {
    const [px, py] = typeof t === "string" ? t.split(",").map(Number) : [t.x, t.y];
    // Le bâtiment est ancré juste au-dessus d'un point du chemin (porte au sud, sur le chemin).
    const spotX = px - Math.floor(w / 2);
    const spotY = py - h;
    if (spotX < 1 || spotY < 1 || spotX + w >= width - 1 || spotY + h >= height - 1) continue;
    // L'ancre elle-même (juste sous la porte) doit être praticable : filet
    // de sécurité si la liste de candidats contenait une tuile devenue
    // solide entre-temps (frange d'arbres, étang...) malgré son étiquette
    // de zone encore "path_edge".
    if (isSolid(terrain[py]?.[px] ?? "")) continue;

    let free = true;
    for (let dy = 0; dy < h && free; dy++) {
      for (let dx = 0; dx < w && free; dx++) {
        const x = spotX + dx;
        const y = spotY + dy;
        const k = `${x},${y}`;
        // Un bâtiment ne doit jamais recouvrir un obstacle naturel déjà en
        // place (arbre de bordure, étang, falaise...) : seul du terrain
        // plat et dégagé peut accueillir une fondation.
        if (occupied.has(k) || mainPath.has(k) || zones[y][x] === "plaza" || isSolid(terrain[y][x])) free = false;
      }
    }
    if (!free) continue;

    if (referencePoint) {
      // Vérifiée par rapport à LA MÊME référence pour tous les bâtiments de
      // la carte (jamais l'ancre propre à ce candidat) : deux bâtiments
      // individuellement "sûrs" pourraient sinon, ensemble, pincer une zone
      // tierce entre eux sans que ni l'un ni l'autre ne le détecte seul.
      const solidWithFootprint = (x: number, y: number) =>
        (x >= spotX && x < spotX + w && y >= spotY && y < spotY + h) || solidNow(x, y);
      const after = floodFillReachable(width, height, solidWithFootprint, referencePoint).size;
      if (before - after > w * h + 4) continue; // cloisonne plus que sa propre emprise : on cherche un autre point
    }

    return { x: spotX, y: spotY };
  }
  return null;
}
