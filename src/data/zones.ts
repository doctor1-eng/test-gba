import type { Zone } from '../engine/types';

export const ZONES: Zone[] = [
  {
    id: 'hameau',
    name: 'Le Hameau Oublié',
    description: "Des chaumières éventrées, des cloches qui sonnent seules. Personne n'a survécu ici — ou alors, plus vraiment.",
    bossId: 'la_faucheuse',
    ambiancePalette: { bg: '#1b1210', accent: '#8a2e12' },
    floors: [
      { index: 1, type: 'combat', combat: { type: 'combat', enemyIds: ['fossoyeur', 'corbeaux_nuee'] } },
      { index: 2, type: 'event' },
      { index: 3, type: 'combat', combat: { type: 'combat', enemyIds: ['villageois_corrompu', 'fossoyeur', 'corbeaux_nuee'] } },
      { index: 4, type: 'camp' },
      { index: 5, type: 'combat', combat: { type: 'combat', enemyIds: ['pretre_renegat', 'villageois_corrompu', 'fossoyeur'] } },
      { index: 6, type: 'event' },
      { index: 7, type: 'boss', combat: { type: 'boss', enemyIds: ['la_faucheuse'] } },
    ],
  },
  {
    id: 'catacombes',
    name: 'Les Catacombes',
    description: "Sous le hameau, les tunnels s'enfoncent plus profond que la raison ne le devrait. L'air y est immobile depuis des siècles.",
    bossId: 'charnier_vivant',
    ambiancePalette: { bg: '#0f1410', accent: '#3f6b3a' },
    floors: [
      { index: 1, type: 'combat', combat: { type: 'combat', enemyIds: ['squelette_guerrier', 'squelette_guerrier', 'vampire_mineur'] } },
      { index: 2, type: 'event' },
      { index: 3, type: 'combat', combat: { type: 'combat', enemyIds: ['vampire_mineur', 'spectre_hurleur', 'goule_putride'] } },
      { index: 4, type: 'camp' },
      { index: 5, type: 'combat', combat: { type: 'combat', enemyIds: ['squelette_guerrier', 'squelette_guerrier', 'vampire_mineur', 'goule_putride'] } },
      { index: 6, type: 'event' },
      { index: 7, type: 'boss', combat: { type: 'boss', enemyIds: ['charnier_vivant'] } },
    ],
  },
];

export function getZoneById(id: string): Zone {
  const zone = ZONES.find((z) => z.id === id);
  if (!zone) throw new Error(`Zone inconnue: ${id}`);
  return zone;
}

export function getNextZoneId(currentZoneId: string): string | null {
  const idx = ZONES.findIndex((z) => z.id === currentZoneId);
  if (idx === -1 || idx + 1 >= ZONES.length) return null;
  return ZONES[idx + 1].id;
}
