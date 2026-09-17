import type { EncounterTier, MapEdge, MapNode, Zone, ZoneMap } from './types';
import type { RngFn } from './rng';
import { randInt } from './rng';

function tierForRow(row: number, totalRows: number): EncounterTier {
  if (row <= 1) return 'early';
  if (row >= totalRows - 2) return 'late';
  return 'mid';
}

// Génère un labyrinthe de rangées de nœuds (2-3 choix par rangée) pour une zone,
// avec un boss unique en dernière rangée. Chaque rangée est reliée à la suivante
// par des arêtes en "treillis" (chaque nœud rejoint 1-2 nœuds voisins de la rangée
// suivante), garantissant qu'aucun nœud n'est isolé.
export function generateZoneMap(zone: Zone, rng: RngFn): ZoneMap {
  const nodes: MapNode[] = [];
  const rowIds: string[][] = [];
  const totalRows = zone.rowTemplates.length;

  zone.rowTemplates.forEach((types, row) => {
    const ids: string[] = [];
    types.forEach((type, col) => {
      const id = `r${row}n${col}`;
      let enemyIds: string[] | undefined;
      if (type === 'boss') {
        enemyIds = [zone.bossId];
      } else if (type === 'combat') {
        const tier = tierForRow(row, totalRows);
        const pool = zone.encounterPool[tier];
        enemyIds = pool[randInt(rng, 0, pool.length - 1)].enemyIds;
      }
      nodes.push({ id, row, col, type, enemyIds });
      ids.push(id);
    });
    rowIds.push(ids);
  });

  const edges: MapEdge[] = [];
  for (let row = 0; row < rowIds.length - 1; row++) {
    const current = rowIds[row];
    const next = rowIds[row + 1];
    const ratio = next.length / current.length;

    current.forEach((fromId, j) => {
      const center = Math.max(0, Math.min(next.length - 1, Math.round(j * ratio)));
      const targets = new Set<number>([center]);
      if (next.length > 1) {
        const neighbor = center + (j * ratio < center || center === next.length - 1 ? -1 : 1);
        targets.add(Math.max(0, Math.min(next.length - 1, neighbor)));
      }
      targets.forEach((k) => edges.push({ from: fromId, to: next[k] }));
    });

    // garantit qu'aucun nœud de la rangée suivante ne reste inaccessible
    next.forEach((toId, k) => {
      if (edges.some((e) => e.to === toId)) return;
      const fallbackFrom = current[Math.max(0, Math.min(current.length - 1, Math.round(k / ratio)))];
      edges.push({ from: fallbackFrom, to: toId });
    });
  }

  return { nodes, edges };
}
