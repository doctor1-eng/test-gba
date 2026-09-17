import type { Relic } from '../engine/types';

export const RELICS: Relic[] = [
  {
    id: 'reliquaire_os',
    name: "Reliquaire d'Os",
    description: 'Les PV maximums de tout le groupe augmentent de 8.',
    effect: { kind: 'maxHp', amount: 8 },
  },
  {
    id: 'amulette_devote',
    name: 'Amulette Dévote',
    description: 'Le groupe résiste mieux à la peur (+5 résistance au stress).',
    effect: { kind: 'stressResist', amount: 5 },
  },
  {
    id: 'bourse_maudite',
    name: 'Bourse Maudite',
    description: "Elle tinte d'un or qui ne s'épuise jamais tout à fait. +5 or par étage.",
    effect: { kind: 'goldOnFloor', amount: 5 },
  },
  {
    id: 'onguent_ancien',
    name: 'Onguent Ancien',
    description: 'Les soins au camp sont plus efficaces (+6 PV).',
    effect: { kind: 'healOnCamp', amount: 6 },
  },
  {
    id: 'fiole_energie',
    name: "Fiole d'Énergie",
    description: 'Le groupe démarre chaque combat avec +1 énergie.',
    effect: { kind: 'startEnergy', amount: 1 },
  },
];

export function getRelicById(id: string): Relic {
  const relic = RELICS.find((r) => r.id === id);
  if (!relic) throw new Error(`Relique inconnue: ${id}`);
  return relic;
}
