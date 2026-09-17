import type { Hero } from '../engine/types';

export const HEROES: Hero[] = [
  {
    id: 'croise',
    name: 'Le Croisé',
    archetype: 'Tank sacré',
    baseHp: 34,
    baseStressThreshold: 100,
    portraitGlyph: '✝',
    colorAccent: '#c9a24b',
    quote: '« La foi ne me sauvera pas. Mais elle me fera tenir un tour de plus. »',
    unlockedByDefault: true,
    startingCardIds: [
      'croise_frappe_sacree', 'croise_frappe_sacree',
      'croise_bouclier', 'croise_bouclier',
      'croise_jugement',
      'croise_priere', 'croise_priere',
      'neutral_second_souffle', 'neutral_position_defensive',
    ],
  },
  {
    id: 'heretique',
    name: "L'Hérétique",
    archetype: 'Magie noire, sacrifice',
    baseHp: 22,
    baseStressThreshold: 100,
    portraitGlyph: '☠',
    colorAccent: '#7a1f2b',
    quote: '« Chaque sort a un prix. Le mien, c\'est ce qu\'il me reste d\'esprit. »',
    unlockedByDefault: true,
    startingCardIds: [
      'heretique_rituel_sang', 'heretique_rituel_sang',
      'heretique_malediction', 'heretique_malediction',
      'heretique_nuee_corbeaux',
      'heretique_pacte_interdit', 'heretique_pacte_interdit',
      'neutral_second_souffle', 'neutral_sursaut',
    ],
  },
  {
    id: 'peste',
    name: 'La Peste',
    archetype: 'Poisons, soutien',
    baseHp: 24,
    baseStressThreshold: 100,
    portraitGlyph: '⚗',
    colorAccent: '#3f6b3a',
    quote: '« Je porte le masque non pour me protéger d\'eux. Mais pour qu\'ils ne me voient pas sourire. »',
    unlockedByDefault: true,
    startingCardIds: [
      'peste_fiole_poison', 'peste_fiole_poison',
      'peste_scalpel', 'peste_scalpel',
      'peste_tonique', 'peste_tonique',
      'peste_vapeurs',
      'neutral_reconfort', 'neutral_sursaut',
    ],
  },
  {
    id: 'bourreau',
    name: 'Le Bourreau',
    archetype: 'Burst, exécution',
    baseHp: 27,
    baseStressThreshold: 100,
    portraitGlyph: '⚔',
    colorAccent: '#8a2e12',
    quote: '« On ne me demande pas de juger. Seulement de finir le travail. »',
    unlockedByDefault: false,
    unlockCost: 3,
    startingCardIds: [
      'bourreau_coup_precis', 'bourreau_coup_precis',
      'bourreau_fauche',
      'bourreau_esquive', 'bourreau_esquive',
      'bourreau_execution', 'bourreau_execution',
      'neutral_second_souffle', 'neutral_position_defensive',
    ],
  },
];

export const PARTY_SIZE = 3;

export function getHeroById(id: string): Hero {
  const hero = HEROES.find((h) => h.id === id);
  if (!hero) throw new Error(`Héros inconnu: ${id}`);
  return hero;
}
