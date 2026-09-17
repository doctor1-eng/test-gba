export interface MetaUpgrade {
  id: string;
  name: string;
  description: string;
  cost: number;
  kind: 'unlockHero' | 'bonusHp' | 'bonusHeal' | 'bonusGold';
  heroId?: string;
}

export const META_UPGRADES: MetaUpgrade[] = [
  {
    id: 'unlock_bourreau',
    name: 'Le Bourreau',
    description: 'Débloque un quatrième héros jouable, spécialiste du burst et de l\'exécution.',
    cost: 3,
    kind: 'unlockHero',
    heroId: 'bourreau',
  },
  {
    id: 'bonus_hp',
    name: 'Bénédiction du Foyer',
    description: 'Tous les héros commencent chaque run avec +5 PV maximum.',
    cost: 2,
    kind: 'bonusHp',
  },
  {
    id: 'bonus_heal',
    name: 'Trousse de Secours',
    description: 'Chaque run commence avec un Onguent Ancien en poche.',
    cost: 2,
    kind: 'bonusHeal',
  },
  {
    id: 'bonus_gold',
    name: 'Bourse de Départ',
    description: 'Chaque run commence avec 20 pièces d\'or supplémentaires.',
    cost: 1,
    kind: 'bonusGold',
  },
];

export function getMetaUpgradeById(id: string): MetaUpgrade {
  const upgrade = META_UPGRADES.find((u) => u.id === id);
  if (!upgrade) throw new Error(`Amélioration meta inconnue: ${id}`);
  return upgrade;
}
