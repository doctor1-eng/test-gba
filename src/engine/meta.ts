import type { MetaState } from './types';
import { getMetaUpgradeById } from '../data/metaUpgrades';

export function createDefaultMeta(): MetaState {
  return {
    ancientRelics: 0,
    unlockedHeroIds: ['croise', 'heretique', 'peste'],
    unlockedUpgradeIds: [],
    bestFloorReached: 0,
    totalRuns: 0,
    totalVictories: 0,
    runHistory: [],
  };
}

export function purchaseUpgrade(meta: MetaState, upgradeId: string): MetaState {
  const upgrade = getMetaUpgradeById(upgradeId);
  if (meta.ancientRelics < upgrade.cost) return meta;
  if (meta.unlockedUpgradeIds.includes(upgradeId)) return meta;

  const next: MetaState = {
    ...meta,
    ancientRelics: meta.ancientRelics - upgrade.cost,
    unlockedUpgradeIds: [...meta.unlockedUpgradeIds, upgradeId],
    unlockedHeroIds: [...meta.unlockedHeroIds],
  };
  if (upgrade.kind === 'unlockHero' && upgrade.heroId && !next.unlockedHeroIds.includes(upgrade.heroId)) {
    next.unlockedHeroIds.push(upgrade.heroId);
  }
  return next;
}

// Récompense en monnaie meta calculée à la fin d'un run (victoire ou mort).
export function computeAncientRelicsReward(floorReached: number, victory: boolean): number {
  const base = Math.floor(floorReached / 2);
  return victory ? base + 5 : base;
}

export function recordRunResult(meta: MetaState, params: { floorReached: number; zoneId: string; gold: number; victory: boolean }): MetaState {
  const reward = computeAncientRelicsReward(params.floorReached, params.victory);
  return {
    ...meta,
    ancientRelics: meta.ancientRelics + reward,
    bestFloorReached: Math.max(meta.bestFloorReached, params.floorReached),
    totalRuns: meta.totalRuns + 1,
    totalVictories: meta.totalVictories + (params.victory ? 1 : 0),
    runHistory: [{ date: Date.now(), floorReached: params.floorReached, zoneId: params.zoneId, gold: params.gold, victory: params.victory }, ...meta.runHistory].slice(0, 20),
  };
}
