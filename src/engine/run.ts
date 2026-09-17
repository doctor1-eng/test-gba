import type { CombatState, MetaState, RunHeroState, RunState } from './types';
import type { RngFn } from './rng';
import { shuffle } from './rng';
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

  return {
    seed,
    zoneId: ZONES[0].id,
    floorIndex: 0,
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

export function getCurrentFloor(run: RunState) {
  return getCurrentZone(run).floors[run.floorIndex];
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

export function advanceFloor(run: RunState): RunState {
  const bonuses = computeRunBonuses(run);
  const gold = run.gold + bonuses.goldOnFloor;
  const zone = getCurrentZone(run);
  const nextIndex = run.floorIndex + 1;

  if (nextIndex >= zone.floors.length) {
    const nextZoneId = getNextZoneId(run.zoneId);
    if (!nextZoneId) {
      return { ...run, gold, status: 'victory' };
    }
    return { ...run, gold, zoneId: nextZoneId, floorIndex: 0 };
  }
  return { ...run, gold, floorIndex: nextIndex };
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
  for (let i = 0; i < zoneIdx; i++) total += ZONES[i].floors.length;
  return total + run.floorIndex + 1;
}

export function isRunOver(run: RunState): boolean {
  return run.status !== 'inProgress';
}

export function livingHeroCount(run: RunState): number {
  return run.heroes.filter((h) => !h.dead).length;
}
