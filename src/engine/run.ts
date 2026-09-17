import type { CombatState, MapNode, MetaState, RunHeroState, RunState } from './types';
import type { RngFn } from './rng';
import { mulberry32, shuffle } from './rng';
import { generateZoneMap } from './mapgen';
import { getHeroById } from '../data/heroes';
import { getRelicById } from '../data/relics';
import { CARDS } from '../data/cards';
import { ZONES, getZoneById, getNextZoneId } from '../data/zones';
import type { CombatBonuses } from './combat';

export function createRun(partyHeroIds: string[], meta: MetaState, seed: number): RunState {
  const bonusHp = meta.unlockedUpgradeIds.includes('bonus_hp') ? 5 : 0;
  const startGold = meta.unlockedUpgradeIds.includes('bonus_gold') ? 20 : 0;
  const startRelics = meta.unlockedUpgradeIds.includes('bonus_heal') ? ['onguent_ancien'] : [];

  const heroes: RunHeroState[] = partyHeroIds.map((id) => {
    const def = getHeroById(id);
    const maxHp = def.baseHp + bonusHp;
    return {
      heroId: id,
      currentHp: maxHp,
      maxHp,
      stress: 0,
      activeQuirks: [],
      deckCardIds: [...def.startingCardIds],
      dead: false,
    };
  });

  const firstZone = ZONES[0];
  const { nodes, edges } = generateZoneMap(firstZone, mulberry32(seed));

  return {
    seed,
    zoneId: firstZone.id,
    mapNodes: nodes,
    mapEdges: edges,
    currentNodeId: null,
    visitedNodeIds: [],
    partyHeroIds: [...partyHeroIds],
    heroes,
    gold: startGold,
    relicIds: startRelics,
    runLog: [],
    status: 'inProgress',
    startedAt: Date.now(),
  };
}

export function getCurrentZone(run: RunState) {
  return getZoneById(run.zoneId);
}

export function getCurrentNode(run: RunState): MapNode | null {
  if (!run.currentNodeId) return null;
  return run.mapNodes.find((n) => n.id === run.currentNodeId) ?? null;
}

// Nœuds que le joueur peut choisir maintenant : la rangée 0 s'il n'a pas encore
// commencé la zone, sinon les nœuds reliés par une arête depuis le nœud courant.
export function getAvailableNodes(run: RunState): MapNode[] {
  if (!run.currentNodeId) {
    return run.mapNodes.filter((n) => n.row === 0);
  }
  const targetIds = new Set(run.mapEdges.filter((e) => e.from === run.currentNodeId).map((e) => e.to));
  return run.mapNodes.filter((n) => targetIds.has(n.id));
}

export function chooseMapNode(run: RunState, nodeId: string): RunState {
  return { ...run, currentNodeId: nodeId, visitedNodeIds: [...run.visitedNodeIds, nodeId] };
}

// À appeler quand l'encounter du nœud courant est résolue (combat gagné, event/camp
// terminé). Applique le bonus d'or par étage puis, si c'était le boss, fait
// transitionner vers la zone suivante (ou déclare la victoire de la run).
export function completeCurrentNode(run: RunState): RunState {
  const bonuses = computeRunBonuses(run);
  let next: RunState = { ...run, gold: run.gold + bonuses.goldOnFloor };
  const node = getCurrentNode(next);

  if (node?.type === 'boss') {
    const nextZoneId = getNextZoneId(next.zoneId);
    if (!nextZoneId) {
      return { ...next, status: 'victory' };
    }
    const nextZone = getZoneById(nextZoneId);
    const zoneIndex = ZONES.findIndex((z) => z.id === nextZoneId);
    const { nodes, edges } = generateZoneMap(nextZone, mulberry32(next.seed + zoneIndex * 7919 + 1));
    next = { ...next, zoneId: nextZoneId, mapNodes: nodes, mapEdges: edges, currentNodeId: null, visitedNodeIds: [] };
  }

  return next;
}

export function computeRunBonuses(run: RunState): CombatBonuses & { healOnCamp: number; goldOnFloor: number } {
  let startEnergy = 0;
  let stressResist = 0;
  let healOnCamp = 0;
  let goldOnFloor = 0;
  for (const relicId of run.relicIds) {
    const relic = getRelicById(relicId);
    switch (relic.effect.kind) {
      case 'startEnergy': startEnergy += relic.effect.amount; break;
      case 'stressResist': stressResist += relic.effect.amount; break;
      case 'healOnCamp': healOnCamp += relic.effect.amount; break;
      case 'goldOnFloor': goldOnFloor += relic.effect.amount; break;
    }
  }
  return { startEnergy, stressResist, healOnCamp, goldOnFloor };
}

export function syncHeroesFromCombat(run: RunState, combat: CombatState): RunState {
  const heroes = run.heroes.map((rh) => {
    const ch = combat.heroes.find((h) => h.heroId === rh.heroId);
    if (!ch) return rh;
    return { ...rh, currentHp: ch.currentHp, maxHp: ch.maxHp, stress: ch.stress, dead: ch.dead || rh.dead };
  });
  return { ...run, heroes };
}

export function addGold(run: RunState, amount: number): RunState {
  return { ...run, gold: Math.max(0, run.gold + amount) };
}

export function healHero(run: RunState, heroId: string, amount: number): RunState {
  const heroes = run.heroes.map((h) => (h.heroId === heroId && !h.dead ? { ...h, currentHp: Math.min(h.maxHp, Math.max(0, h.currentHp) + amount) } : h));
  return { ...run, heroes };
}

export function healAll(run: RunState, amount: number): RunState {
  const heroes = run.heroes.map((h) => (!h.dead ? { ...h, currentHp: Math.min(h.maxHp, Math.max(0, h.currentHp) + amount) } : h));
  return { ...run, heroes };
}

export function adjustStress(run: RunState, heroId: string, delta: number): RunState {
  const heroes = run.heroes.map((h) => (h.heroId === heroId && !h.dead ? { ...h, stress: Math.max(0, Math.min(200, h.stress + delta)) } : h));
  return { ...run, heroes };
}

export function adjustStressAll(run: RunState, delta: number): RunState {
  const heroes = run.heroes.map((h) => (!h.dead ? { ...h, stress: Math.max(0, Math.min(200, h.stress + delta)) } : h));
  return { ...run, heroes };
}

export function addQuirkToHero(run: RunState, heroId: string, quirkId: string): RunState {
  const heroes = run.heroes.map((h) => (h.heroId === heroId && !h.activeQuirks.includes(quirkId) ? { ...h, activeQuirks: [...h.activeQuirks, quirkId] } : h));
  return { ...run, heroes };
}

export function removeQuirkFromHero(run: RunState, heroId: string, quirkId: string): RunState {
  const heroes = run.heroes.map((h) => (h.heroId === heroId ? { ...h, activeQuirks: h.activeQuirks.filter((q) => q !== quirkId) } : h));
  return { ...run, heroes };
}

export function applyRelicGain(run: RunState, relicId: string): RunState {
  const relic = getRelicById(relicId);
  let heroes = run.heroes;
  if (relic.effect.kind === 'maxHp') {
    const amount = relic.effect.amount;
    heroes = run.heroes.map((h) => (h.dead ? h : { ...h, maxHp: h.maxHp + amount, currentHp: h.currentHp > 0 ? h.currentHp + amount : h.currentHp }));
  }
  return { ...run, relicIds: [...run.relicIds, relicId], heroes };
}

export function addCardToHeroDeck(run: RunState, heroId: string, cardId: string): RunState {
  const heroes = run.heroes.map((h) => (h.heroId === heroId ? { ...h, deckCardIds: [...h.deckCardIds, cardId] } : h));
  return { ...run, heroes };
}

export function generateCardRewardOptions(partyHeroIds: string[], rng: RngFn, count = 3) {
  const pool = CARDS.filter((c) => c.heroId === 'neutral' || partyHeroIds.includes(c.heroId));
  return shuffle(pool, rng).slice(0, count);
}

export function computeGlobalFloorNumber(run: RunState): number {
  const zoneIdx = ZONES.findIndex((z) => z.id === run.zoneId);
  let total = 0;
  for (let i = 0; i < zoneIdx; i++) total += ZONES[i].rowTemplates.length;
  return total + run.visitedNodeIds.length;
}

export function isRunOver(run: RunState): boolean {
  return run.status !== 'inProgress';
}

export function livingHeroCount(run: RunState): number {
  return run.heroes.filter((h) => !h.dead).length;
}
