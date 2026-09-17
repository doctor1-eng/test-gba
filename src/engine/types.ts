// Types partagés du moteur de jeu. Aucun import React ici : cette couche reste pure.

export type StatKey = 'damage' | 'defense' | 'stressResist';

export type EffectTarget = 'self' | 'cardTarget';

export type Effect =
  | { kind: 'damage'; amount: number; variance: number; target?: EffectTarget; executeBelowPercent?: number; executeBonus?: number }
  | { kind: 'heal'; amount: number; target?: EffectTarget }
  | { kind: 'stress'; amount: number; target?: EffectTarget }
  | { kind: 'block'; amount: number; target?: EffectTarget }
  | { kind: 'buff'; stat: StatKey; amount: number; duration: number; target?: EffectTarget }
  | { kind: 'debuff'; stat: StatKey; amount: number; duration: number; target?: EffectTarget }
  | { kind: 'bleed'; amount: number; duration: number; target?: EffectTarget }
  | { kind: 'poison'; amount: number; duration: number; target?: EffectTarget }
  | { kind: 'draw'; amount: number; target?: EffectTarget }
  | { kind: 'energy'; amount: number; target?: EffectTarget };

export type EffectKind = Effect['kind'];

export type CardTarget = 'singleEnemy' | 'allEnemies' | 'singleAlly' | 'allAllies' | 'self' | 'randomEnemy';

export type CardType = 'attack' | 'skill' | 'guard' | 'heal' | 'curse';

export interface Card {
  id: string;
  heroId: string | 'neutral';
  name: string;
  cost: number;
  type: CardType;
  target: CardTarget;
  effects: Effect[];
  description: string;
  tier: 0 | 1 | 2;
  upgradeOf?: string; // id de la carte de base si carte améliorée (tier 1/2)
}

export interface Hero {
  id: string;
  name: string;
  archetype: string;
  baseHp: number;
  baseStressThreshold: number;
  startingCardIds: string[];
  portraitGlyph: string; // caractère/emoji utilisé pour le portrait stylisé CSS
  quote: string;
  colorAccent: string;
  unlockedByDefault: boolean;
  unlockCost?: number; // coût en reliques anciennes si non débloqué par défaut
}

export type EnemyMoveEffectTarget = 'singleHero' | 'allHeroes' | 'randomHero' | 'self';

export interface EnemyMove {
  id: string;
  name: string;
  weight: number;
  minHpPercent?: number; // ce move n'est utilisable qu'au-dessus de ce % de vie
  maxHpPercent?: number; // ce move n'est utilisable qu'en dessous de ce % de vie (phases de boss)
  target: EnemyMoveEffectTarget;
  effects: Effect[];
  summonEnemyId?: string;
}

export interface Enemy {
  id: string;
  name: string;
  hp: number;
  stressDamageOnHit: number;
  moves: EnemyMove[];
  zoneId: string;
  isBoss: boolean;
  portraitGlyph: string;
  description: string;
  resistances?: Partial<Record<EffectKind, number>>; // 0-1, réduction en %
}

export interface EventChoice {
  id: string;
  label: string;
  outcomes: { weight: number; resultText: string; goldDelta?: number; hpDeltaAll?: number; hpDeltaRandom?: number; stressDeltaAll?: number; stressDeltaRandom?: number; relicId?: string; quirkId?: 'random-negative' | 'random-positive' }[];
}

export interface NarrativeEvent {
  id: string;
  title: string;
  text: string;
  choices: EventChoice[];
}

export type EncounterType = 'combat' | 'event' | 'camp' | 'boss';

export interface CombatEncounterDef {
  type: 'combat' | 'boss';
  enemyIds: string[];
}

export type EncounterTier = 'early' | 'mid' | 'late';

export interface EncounterPool {
  early: CombatEncounterDef[];
  mid: CombatEncounterDef[];
  late: CombatEncounterDef[];
}

export interface Zone {
  id: string;
  name: string;
  description: string;
  bossId: string;
  ambiancePalette: { bg: string; accent: string };
  // Gabarit du labyrinthe : une entrée par étage (rangée), la liste donnant les
  // types de nœuds proposés au joueur à cet étage (2-3 choix par rangée).
  rowTemplates: EncounterType[][];
  encounterPool: EncounterPool;
}

// ---- Carte de donjon (labyrinthe à embranchements, générée par run) ----

export interface MapNode {
  id: string;
  row: number;
  col: number;
  type: EncounterType;
  enemyIds?: string[]; // pour type 'combat' | 'boss'
}

export interface MapEdge {
  from: string;
  to: string;
}

export interface ZoneMap {
  nodes: MapNode[];
  edges: MapEdge[];
}

export type QuirkType = 'affliction' | 'virtue';

export interface Quirk {
  id: string;
  name: string;
  type: QuirkType;
  description: string;
  effect: Effect;
  selfDamageChance?: number; // certaines afflictions font agir le héros contre lui-même
  refuseOrderChance?: number; // risque de refuser d'exécuter la carte choisie
}

export interface Relic {
  id: string;
  name: string;
  description: string;
  effect:
    | { kind: 'maxHp'; amount: number }
    | { kind: 'startEnergy'; amount: number }
    | { kind: 'stressResist'; amount: number }
    | { kind: 'goldOnFloor'; amount: number }
    | { kind: 'healOnCamp'; amount: number };
}

// ---- État de runtime ----

export interface StatusEffectInstance {
  stat?: StatKey;
  kind: 'buff' | 'debuff' | 'bleed' | 'poison';
  amount: number;
  duration: number;
}

export interface HeroRuntimeState {
  heroId: string;
  currentHp: number;
  maxHp: number;
  stress: number;
  stressThreshold: number;
  block: number;
  statuses: StatusEffectInstance[];
  atDeathsDoor: boolean;
  dead: boolean;
  activeQuirks: string[]; // ids de quirks permanents pour la run
  temporaryQuirk?: { quirkId: string; roundsLeft: number }; // affliction/vertu active en combat
}

export interface EnemyRuntimeState {
  instanceId: string;
  enemyId: string;
  currentHp: number;
  maxHp: number;
  block: number;
  statuses: StatusEffectInstance[];
  dead: boolean;
}

export interface CombatLogEntry {
  id: string;
  text: string;
}

export type CombatPhase = 'playerTurn' | 'enemyTurn' | 'victory' | 'defeat';

export interface CombatState {
  heroes: HeroRuntimeState[];
  enemies: EnemyRuntimeState[];
  drawPile: string[];
  hand: string[];
  discardPile: string[];
  exhaustPile: string[];
  energy: number;
  maxEnergy: number;
  round: number;
  phase: CombatPhase;
  log: CombatLogEntry[];
  isBossFight: boolean;
}

export interface RunHeroState {
  heroId: string;
  currentHp: number;
  maxHp: number;
  stress: number;
  activeQuirks: string[];
  deckCardIds: string[]; // deck complet de ce héros (base + cartes gagnées attribuées à lui)
  dead: boolean;
}

export interface RunState {
  seed: number;
  zoneId: string;
  mapNodes: MapNode[];
  mapEdges: MapEdge[];
  currentNodeId: string | null; // null = pas encore entré dans la zone, choix parmi la rangée 0
  visitedNodeIds: string[];
  partyHeroIds: string[];
  heroes: RunHeroState[];
  gold: number;
  relicIds: string[];
  runLog: string[];
  status: 'inProgress' | 'victory' | 'defeat';
  startedAt: number;
}

export interface MetaState {
  ancientRelics: number; // monnaie meta persistante
  unlockedHeroIds: string[];
  unlockedUpgradeIds: string[];
  bestFloorReached: number;
  totalRuns: number;
  totalVictories: number;
  runHistory: { date: number; floorReached: number; zoneId: string; gold: number; victory: boolean }[];
}
