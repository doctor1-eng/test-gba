import type { Quirk } from '../engine/types';

export const QUIRKS: Quirk[] = [
  {
    id: 'main_tremblante',
    name: 'Main Tremblante',
    type: 'affliction',
    description: 'Ses mains tremblent sans répit. -3 dégâts infligés.',
    effect: { kind: 'debuff', stat: 'damage', amount: 3, duration: 99, target: 'self' },
  },
  {
    id: 'paranoia',
    name: 'Paranoïa',
    type: 'affliction',
    description: 'Il sursaute à chaque ombre, chaque souffle. +3 dégâts subis.',
    effect: { kind: 'debuff', stat: 'defense', amount: 3, duration: 99, target: 'self' },
  },
  {
    id: 'terreur_muette',
    name: 'Terreur Muette',
    type: 'affliction',
    description: 'Parfois, la terreur le paralyse et il ignore les ordres.',
    effect: { kind: 'debuff', stat: 'damage', amount: 0, duration: 99, target: 'self' },
    refuseOrderChance: 0.2,
  },
  {
    id: 'auto_mutilation',
    name: 'Auto-Mutilation',
    type: 'affliction',
    description: 'Pris de folie, il se blesse parfois lui-même au lieu d\'agir.',
    effect: { kind: 'damage', amount: 4, variance: 1, target: 'self' },
    selfDamageChance: 0.15,
  },
  {
    id: 'inspire',
    name: 'Inspiré',
    type: 'virtue',
    description: "Une clarté soudaine traverse son esprit. +3 dégâts infligés.",
    effect: { kind: 'buff', stat: 'damage', amount: 3, duration: 99, target: 'self' },
  },
  {
    id: 'stoique',
    name: 'Stoïque',
    type: 'virtue',
    description: "Rien ne semble plus l'atteindre. Résiste mieux au stress.",
    effect: { kind: 'buff', stat: 'stressResist', amount: 5, duration: 99, target: 'self' },
  },
  {
    id: 'berserk',
    name: 'Rage Froide',
    type: 'virtue',
    description: 'Une rage glaciale le saisit. +6 dégâts infligés.',
    effect: { kind: 'buff', stat: 'damage', amount: 6, duration: 99, target: 'self' },
  },
  {
    id: 'lucidite',
    name: 'Lucidité',
    type: 'virtue',
    description: 'Un instant de paix retrouvée au cœur de l\'horreur. Soigne 10 PV.',
    effect: { kind: 'heal', amount: 10, target: 'self' },
  },
];

export function getQuirkById(id: string): Quirk {
  const quirk = QUIRKS.find((q) => q.id === id);
  if (!quirk) throw new Error(`Quirk inconnu: ${id}`);
  return quirk;
}

export function rollRandomQuirk(type: 'affliction' | 'virtue', rng: () => number): Quirk {
  const pool = QUIRKS.filter((q) => q.type === type);
  const idx = Math.floor(rng() * pool.length);
  return pool[Math.min(idx, pool.length - 1)];
}
