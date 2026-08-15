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
};

/**
 * Cherche la case non-solide, non occupée la plus proche par expansion en
 * anneaux — filet de sécurité si la case visée a été recouverte entre-temps
 * (ex: par un bâtiment placé après le choix du point de landmark).
 */
function nearestFreeSpot(
  start: { x: number; y: number },
  width: number,
  height: number,
  isSolid: (x: number, y: number) => boolean,
  occupied: Set<string>,
): { x: number; y: number } {
  if (!isSolid(start.x, start.y) && !occupied.has(`${start.x},${start.y}`)) return start;
  for (let radius = 1; radius < Math.max(width, height); radius++) {
    for (let dy = -radius; dy <= radius; dy++) {
      for (let dx = -radius; dx <= radius; dx++) {
        if (Math.max(Math.abs(dx), Math.abs(dy)) !== radius) continue;
        const x = start.x + dx;
        const y = start.y + dy;
        if (x <= 0 || y <= 0 || x >= width - 1 || y >= height - 1) continue;
        if (!isSolid(x, y) && !occupied.has(`${x},${y}`)) return { x, y };
      }
    }
  }
  return start; // aucune case libre trouvée : le validateur le signalera explicitement
}

/** Pose un ou plusieurs landmarks mémorables, toujours en zone dégagée et non-solide. */
export function placeLandmarks(
  rng: Rng,
  terrainResult: TerrainResult,
  template: MapTemplate,
  isSolid: (x: number, y: number) => boolean,
  occupied: Set<string>,
): Landmark[] {
  const count = Math.max(template.landmarks.minCount, rng.int(template.landmarks.minCount, template.landmarks.maxCount));
  const landmarks: Landmark[] = [];
  const pool = rng.shuffle(template.landmarks.pool);

  for (let i = 0; i < count && i < pool.length; i++) {
    const type = pool[i];
    const intended =
      i === 0 ? terrainResult.landmarkSpot : { x: terrainResult.landmarkSpot.x + i * 3, y: terrainResult.landmarkSpot.y };
    const spot = nearestFreeSpot(intended, terrainResult.width, terrainResult.height, isSolid, occupied);
    occupied.add(`${spot.x},${spot.y}`);
    landmarks.push({
      id: `landmark_${i + 1}`,
      label: LANDMARK_LABELS[type] ?? type,
      x: spot.x,
      y: spot.y,
      description: LANDMARK_DESC[type] ?? "Point de repère notable de la zone.",
    });
  }
  return landmarks;
}
