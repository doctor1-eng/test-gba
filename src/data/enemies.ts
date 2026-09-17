import type { Enemy } from '../engine/types';

export const ENEMIES: Enemy[] = [
  // --- Zone 1 : Le Hameau Oublié ---
  {
    id: 'fossoyeur',
    name: 'Fossoyeur',
    hp: 18,
    stressDamageOnHit: 4,
    zoneId: 'hameau',
    isBoss: false,
    portraitGlyph: '⛏',
    description: 'Il creuse encore, par habitude. Les tombes ne manquent jamais de clients.',
    moves: [
      { id: 'pelle', name: 'Coup de Pelle', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 5, variance: 2 }] },
      { id: 'fosse_hative', name: 'Fosse Hâtive', weight: 2, target: 'singleHero', effects: [{ kind: 'damage', amount: 3, variance: 1 }, { kind: 'stress', amount: 6 }] },
    ],
  },
  {
    id: 'corbeaux_nuee',
    name: 'Nuée de Corbeaux',
    hp: 10,
    stressDamageOnHit: 2,
    zoneId: 'hameau',
    isBoss: false,
    portraitGlyph: '🜏',
    description: "Ils tournent au-dessus des mourants. Ils savent toujours avant vous.",
    moves: [
      { id: 'bec', name: 'Bec', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 3, variance: 1 }] },
      { id: 'tourbillon', name: 'Tourbillon Noir', weight: 1, target: 'allHeroes', effects: [{ kind: 'damage', amount: 2, variance: 1 }] },
    ],
  },
  {
    id: 'villageois_corrompu',
    name: 'Villageois Corrompu',
    hp: 22,
    stressDamageOnHit: 5,
    zoneId: 'hameau',
    isBoss: false,
    portraitGlyph: '☾',
    description: 'Vous le reconnaissez presque. Ce qu\'il est devenu ne vous reconnaît pas.',
    moves: [
      { id: 'griffe', name: 'Griffe', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 6, variance: 2 }] },
      { id: 'hurlement', name: 'Hurlement', weight: 1, target: 'allHeroes', effects: [{ kind: 'stress', amount: 8 }] },
    ],
  },
  {
    id: 'pretre_renegat',
    name: 'Prêtre Renégat',
    hp: 20,
    stressDamageOnHit: 9,
    zoneId: 'hameau',
    isBoss: false,
    portraitGlyph: '✟',
    description: 'Il prêche encore. Mais plus le même dieu.',
    moves: [
      { id: 'sermon_funeste', name: 'Sermon Funeste', weight: 2, target: 'singleHero', effects: [{ kind: 'stress', amount: 14 }] },
      { id: 'coup_de_crosse', name: 'Coup de Crosse', weight: 2, target: 'singleHero', effects: [{ kind: 'damage', amount: 5, variance: 1 }] },
    ],
  },
  {
    id: 'la_faucheuse',
    name: 'La Faucheuse du Hameau',
    hp: 70,
    stressDamageOnHit: 10,
    zoneId: 'hameau',
    isBoss: true,
    portraitGlyph: '☠',
    description: "Elle a moissonné le village entier. Vous n'êtes qu'une brindille de plus.",
    moves: [
      { id: 'fauche_large', name: 'Fauche Large', weight: 3, target: 'allHeroes', minHpPercent: 0.5, effects: [{ kind: 'damage', amount: 6, variance: 2 }] },
      { id: 'moisson', name: 'Moisson', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 12, variance: 3 }] },
      { id: 'regard_mort', name: 'Regard de la Mort', weight: 2, target: 'allHeroes', maxHpPercent: 0.5, effects: [{ kind: 'stress', amount: 16 }] },
    ],
  },

  // --- Zone 2 : Les Catacombes ---
  {
    id: 'squelette_guerrier',
    name: 'Squelette Guerrier',
    hp: 16,
    stressDamageOnHit: 3,
    zoneId: 'catacombes',
    isBoss: false,
    portraitGlyph: '🗡',
    description: 'Ses os cliquettent en rythme. Il a oublié pourquoi il se bat, mais pas comment.',
    moves: [
      { id: 'estoc', name: 'Estoc', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 7, variance: 2 }] },
      { id: 'position', name: 'Position', weight: 1, target: 'self', effects: [{ kind: 'block', amount: 8, target: 'self' }] },
    ],
  },
  {
    id: 'vampire_mineur',
    name: 'Vampire Mineur',
    hp: 20,
    stressDamageOnHit: 4,
    zoneId: 'catacombes',
    isBoss: false,
    portraitGlyph: '🦇',
    description: 'Trop jeune pour la puissance, assez affamé pour le désespoir.',
    moves: [
      { id: 'morsure', name: 'Morsure', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 6, variance: 1 }, { kind: 'bleed', amount: 3, duration: 2 }] },
      { id: 'vol_de_vie', name: 'Vol de Vie', weight: 2, target: 'singleHero', effects: [{ kind: 'damage', amount: 5, variance: 1 }, { kind: 'heal', amount: 5, target: 'self' }] },
    ],
  },
  {
    id: 'spectre_hurleur',
    name: 'Spectre Hurleur',
    hp: 18,
    stressDamageOnHit: 12,
    zoneId: 'catacombes',
    isBoss: false,
    portraitGlyph: '👻',
    description: 'Son cri ne blesse pas la chair. Il s\'attaque à ce qu\'il en reste, à l\'intérieur.',
    moves: [
      { id: 'hurlement_glacial', name: 'Hurlement Glacial', weight: 2, target: 'allHeroes', effects: [{ kind: 'stress', amount: 14 }] },
      { id: 'griffe_spectrale', name: 'Griffe Spectrale', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 6, variance: 2 }] },
    ],
  },
  {
    id: 'goule_putride',
    name: 'Goule Putride',
    hp: 24,
    stressDamageOnHit: 5,
    zoneId: 'catacombes',
    isBoss: false,
    portraitGlyph: '🪱',
    description: "L'odeur seule suffit presque à vous faire fuir. Presque.",
    moves: [
      { id: 'morsure_infecte', name: 'Morsure Infecte', weight: 3, target: 'singleHero', effects: [{ kind: 'damage', amount: 5, variance: 1 }, { kind: 'poison', amount: 5, duration: 3 }] },
      { id: 'vomissure', name: 'Vomissure', weight: 1, target: 'allHeroes', effects: [{ kind: 'damage', amount: 3, variance: 1 }] },
    ],
  },
  {
    id: 'charnier_vivant',
    name: 'Le Charnier Vivant',
    hp: 90,
    stressDamageOnHit: 12,
    zoneId: 'catacombes',
    isBoss: true,
    portraitGlyph: '🕸',
    description: 'Un amas d\'ossements et de chair recousue, qui respire encore, quelque part sous les côtes empilées.',
    moves: [
      { id: 'invocation_ossements', name: "Invocation d'Ossements", weight: 2, target: 'self', effects: [], summonEnemyId: 'squelette_guerrier' },
      { id: 'effondrement', name: 'Effondrement', weight: 3, target: 'allHeroes', effects: [{ kind: 'damage', amount: 8, variance: 2 }] },
      { id: 'etreinte_charnier', name: 'Étreinte du Charnier', weight: 2, target: 'singleHero', effects: [{ kind: 'damage', amount: 14, variance: 3 }, { kind: 'stress', amount: 10 }] },
    ],
  },
];

export function getEnemyById(id: string): Enemy {
  const enemy = ENEMIES.find((e) => e.id === id);
  if (!enemy) throw new Error(`Ennemi inconnu: ${id}`);
  return enemy;
}
