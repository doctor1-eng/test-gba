import type { Rng } from "./rng.js";
import type { Landmark } from "../schema/types.js";
import type { MapTemplate } from "../schema/template.js";
import type { TerrainResult } from "./terrain/common.js";

const LANDMARK_LABELS: Record<string, string> = {
  fountain: "Fontaine centrale",
  statue: "Statue commémorative",
  clock_tower: "Tour de l'horloge",
  old_tree: "Vieil arbre",
  well: "Puits du village",
  rest_spot: "Aire de repos",
  big_rock: "Rocher massif",
  giant_tree: "Arbre géant de la forêt",
  crystal_formation: "Formation de cristaux",
  summit_view: "Point de vue du sommet",
  shipwreck: "Épave échouée",
  monument: "Monument",
  pond: "Étang",
};

const LANDMARK_DESC: Record<string, string> = {
  fountain: "Visible depuis presque toute la ville, sert de repère pour s'orienter.",
  statue: "Marque un événement important de l'histoire locale.",
  clock_tower: "Le plus haut bâtiment, visible de loin.",
  old_tree: "Le village s'est construit autour, symbole du lieu.",
  well: "Ancien point d'eau, toujours utilisé par les habitants.",
  rest_spot: "Un endroit dégagé pour souffler avant de continuer la route.",
  big_rock: "Un bloc rocheux impossible à manquer.",
  giant_tree: "Domine la canopée, visible depuis la clairière.",
  crystal_formation: "Un amas de cristaux qui scintille à la lumière des torches.",
  summit_view: "Vue dégagée sur toute la région en contrebas.",
  shipwreck: "Vestige d'un naufrage ancien, échoué sur le sable.",
  monument: "Point central de la zone, chargé de sens narratif.",
  pond: "Un petit point d'eau qui rompt la monotonie du terrain environnant.",
};

/**
 * Cherche la case non-solide, non occupée ET atteignable la plus proche par
 * expansion en anneaux — filet de sécurité si la case visée a été
 * recouverte entre-temps (ex: par un bâtiment placé après le choix du
 * point de landmark), ou si elle est techniquement praticable mais isolée
 * du reste de la carte (une case "libre" ne suffit pas si le joueur ne
 * peut jamais l'atteindre).
 */
function nearestFreeSpot(
  start: { x: number; y: number },
  width: number,
  height: number,
  isSolid: (x: number, y: number) => boolean,
  occupied: Set<string>,
  reachable: Set<string>,
): { x: number; y: number } {
  const ok = (x: number, y: number) =>
    !isSolid(x, y) && !occupied.has(`${x},${y}`) && (reachable.size === 0 || reachable.has(`${x},${y}`));
  if (ok(start.x, start.y)) return start;
  for (let radius = 1; radius < Math.max(width, height); radius++) {
    for (let dy = -radius; dy <= radius; dy++) {
      for (let dx = -radius; dx <= radius; dx++) {
        if (Math.max(Math.abs(dx), Math.abs(dy)) !== radius) continue;
        const x = start.x + dx;
        const y = start.y + dy;
        if (x <= 0 || y <= 0 || x >= width - 1 || y >= height - 1) continue;
        if (ok(x, y)) return { x, y };
      }
    }
  }
  return start; // aucune case libre trouvée : le validateur le signalera explicitement
}

/**
 * Pose un ou plusieurs landmarks mémorables, toujours en zone dégagée et
 * non-solide. Priorité aux landmarks suggérés par le générateur de terrain
 * lui-même (`extraLandmarkSpots`, ex: un étang tout juste creusé) : ils
 * correspondent à un vrai élément du terrain plutôt qu'à une case
 * arbitraire, mais ne sont retenus que si leur type figure dans le pool du
 * template (le template garde la main sur ce qui compte comme landmark).
 */
export function placeLandmarks(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  isSolid: (x: number, y: number) => boolean,
  occupied: Set<string>,
  reachable: Set<string>,
): Landmark[] {
  const count = Math.max(template.landmarks.minCount, rng.int(template.landmarks.minCount, template.landmarks.maxCount));
  const landmarks: Landmark[] = [];
  const usedTypes = new Set<string>();

  const placeAt = (type: string, intended: { x: number; y: number }) => {
    const spot = nearestFreeSpot(intended, terrainResult.width, terrainResult.height, isSolid, occupied, reachable);
    occupied.add(`${spot.x},${spot.y}`);
    usedTypes.add(type);
    landmarks.push({
      id: `landmark_${landmarks.length + 1}`,
      label: LANDMARK_LABELS[type] ?? type,
      x: spot.x,
      y: spot.y,
      description: LANDMARK_DESC[type] ?? "Point de repère notable de la zone.",
    });
  };

  for (const extra of terrainResult.extraLandmarkSpots ?? []) {
    if (landmarks.length >= count) break;
    if (!template.landmarks.pool.includes(extra.type) || usedTypes.has(extra.type)) continue;
    placeAt(extra.type, extra.point);
  }

  const remainingPool = rng.shuffle(template.landmarks.pool.filter((t) => !usedTypes.has(t)));
  for (let i = 0; landmarks.length < count && i < remainingPool.length; i++) {
    const intended =
      landmarks.length === 0
        ? terrainResult.landmarkSpot
        : { x: terrainResult.landmarkSpot.x + landmarks.length * 3, y: terrainResult.landmarkSpot.y };
    placeAt(remainingPool[i], intended);
  }

  return landmarks;
}
