import type { Direction, Warp } from "../schema/types.js";

/**
 * Paire de warps réciproques génériques (utilisée pour les entrées de
 * grotte : contrairement aux transitions ville/route/forêt qui sont
 * continues via `connections[]`, entrer dans une grotte déclenche un vrai
 * changement de map, fidèle à la convention Pokémon Gen 3.
 */
export function createReciprocalWarpPair(
  mapAId: string,
  warpAId: string,
  pointA: { x: number; y: number; direction: Direction },
  mapBId: string,
  warpBId: string,
  pointB: { x: number; y: number; direction: Direction },
): { warpA: Warp; warpB: Warp } {
  return {
    warpA: {
      id: warpAId,
      x: pointA.x,
      y: pointA.y,
      direction: pointA.direction,
      destinationMap: mapBId,
      destinationWarpId: warpBId,
    },
    warpB: {
      id: warpBId,
      x: pointB.x,
      y: pointB.y,
      direction: pointB.direction,
      destinationMap: mapAId,
      destinationWarpId: warpAId,
    },
  };
}
