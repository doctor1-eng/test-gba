import type { Zone } from '../engine/types';

export const ZONES: Zone[] = [
  {
    id: 'hameau',
    name: 'Le Hameau Oublié',
    description: "Des chaumières éventrées, des cloches qui sonnent seules. Personne n'a survécu ici — ou alors, plus vraiment.",
    bossId: 'la_faucheuse',
    ambiancePalette: { bg: '#1b1210', accent: '#8a2e12' },
    rowTemplates: [
      ['combat', 'event'],
      ['combat', 'combat', 'event'],
      ['event', 'combat', 'combat'],
      ['combat', 'camp', 'combat'],
      ['combat', 'event', 'combat'],
      ['combat', 'combat', 'event'],
      ['boss'],
    ],
    encounterPool: {
      early: [
        { type: 'combat', enemyIds: ['fossoyeur', 'corbeaux_nuee'] },
        { type: 'combat', enemyIds: ['corbeaux_nuee', 'corbeaux_nuee'] },
      ],
      mid: [
        { type: 'combat', enemyIds: ['villageois_corrompu', 'fossoyeur', 'corbeaux_nuee'] },
        { type: 'combat', enemyIds: ['fossoyeur', 'fossoyeur', 'corbeaux_nuee'] },
      ],
      late: [
        { type: 'combat', enemyIds: ['pretre_renegat', 'villageois_corrompu', 'fossoyeur'] },
        { type: 'combat', enemyIds: ['pretre_renegat', 'corbeaux_nuee', 'corbeaux_nuee'] },
      ],
    },
  },
  {
    id: 'catacombes',
    name: 'Les Catacombes',
    description: "Sous le hameau, les tunnels s'enfoncent plus profond que la raison ne le devrait. L'air y est immobile depuis des siècles.",
    bossId: 'charnier_vivant',
    ambiancePalette: { bg: '#0f1410', accent: '#3f6b3a' },
    rowTemplates: [
      ['combat', 'event'],
      ['combat', 'combat', 'event'],
      ['event', 'combat', 'combat'],
      ['combat', 'camp', 'combat'],
      ['combat', 'event', 'combat'],
      ['combat', 'combat', 'event'],
      ['boss'],
    ],
    encounterPool: {
      early: [
        { type: 'combat', enemyIds: ['squelette_guerrier', 'squelette_guerrier'] },
        { type: 'combat', enemyIds: ['squelette_guerrier', 'vampire_mineur'] },
      ],
      mid: [
        { type: 'combat', enemyIds: ['vampire_mineur', 'spectre_hurleur', 'goule_putride'] },
        { type: 'combat', enemyIds: ['squelette_guerrier', 'squelette_guerrier', 'vampire_mineur'] },
      ],
      late: [
        { type: 'combat', enemyIds: ['squelette_guerrier', 'squelette_guerrier', 'vampire_mineur', 'goule_putride'] },
        { type: 'combat', enemyIds: ['spectre_hurleur', 'goule_putride', 'vampire_mineur'] },
      ],
    },
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
