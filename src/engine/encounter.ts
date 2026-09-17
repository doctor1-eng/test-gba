import type { CombatState, NarrativeEvent, RunState } from './types';
import type { RngFn } from './rng';
import { pickWeighted, randInt } from './rng';
import { initCombat } from './combat';
import { getCurrentFloor, computeRunBonuses, addGold, healHero, healAll, adjustStress, adjustStressAll, addQuirkToHero, applyRelicGain, removeQuirkFromHero } from './run';
import { QUIRKS, rollRandomQuirk } from '../data/quirks';
import { getHeroById } from '../data/heroes';

export function startCombatForFloor(run: RunState, rng: RngFn): CombatState {
  const floor = getCurrentFloor(run);
  if (!floor.combat) throw new Error('Cet étage ne contient pas de combat.');
  const bonuses = computeRunBonuses(run);
  return initCombat(run.heroes, floor.combat.enemyIds, floor.type === 'boss', rng, bonuses);
}

export function resolveEventChoice(run: RunState, event: NarrativeEvent, choiceId: string, rng: RngFn): { run: RunState; resultText: string } {
  const choice = event.choices.find((c) => c.id === choiceId);
  if (!choice) return { run, resultText: '' };
  const outcome = pickWeighted(choice.outcomes, rng);

  let next = run;
  if (outcome.goldDelta) next = addGold(next, outcome.goldDelta);

  const aliveHeroIds = next.heroes.filter((h) => !h.dead).map((h) => h.heroId);

  if (outcome.hpDeltaAll) next = healAll(next, outcome.hpDeltaAll);
  if (outcome.hpDeltaRandom && aliveHeroIds.length) {
    const heroId = aliveHeroIds[randInt(rng, 0, aliveHeroIds.length - 1)];
    next = healHero(next, heroId, outcome.hpDeltaRandom);
  }
  if (outcome.stressDeltaAll) next = adjustStressAll(next, outcome.stressDeltaAll);
  if (outcome.stressDeltaRandom && aliveHeroIds.length) {
    const heroId = aliveHeroIds[randInt(rng, 0, aliveHeroIds.length - 1)];
    next = adjustStress(next, heroId, outcome.stressDeltaRandom);
  }
  if (outcome.relicId) next = applyRelicGain(next, outcome.relicId);
  if (outcome.quirkId && aliveHeroIds.length) {
    const type = outcome.quirkId === 'random-negative' ? 'affliction' : 'virtue';
    const quirk = rollRandomQuirk(type, rng);
    const heroId = aliveHeroIds[randInt(rng, 0, aliveHeroIds.length - 1)];
    next = addQuirkToHero(next, heroId, quirk.id);
  }

  return { run: next, resultText: outcome.resultText };
}

export function resolveCamp(run: RunState, action: 'rest' | 'watch', rng: RngFn): { run: RunState; resultText: string } {
  const bonuses = computeRunBonuses(run);
  let next = run;

  if (action === 'rest') {
    for (const h of run.heroes.filter((h) => !h.dead)) {
      const amount = Math.round(h.maxHp * 0.3) + bonuses.healOnCamp;
      next = healHero(next, h.heroId, amount);
    }
    next = adjustStressAll(next, -20);
    return { run: next, resultText: 'Le groupe se repose autour d\'un feu de fortune. Les blessures se referment, l\'esprit s\'apaise un peu.' };
  }

  for (const h of run.heroes.filter((h) => !h.dead)) {
    const amount = Math.round(h.maxHp * 0.15) + bonuses.healOnCamp;
    next = healHero(next, h.heroId, amount);
  }
  const candidates = next.heroes.filter((h) => !h.dead && h.activeQuirks.some((q) => QUIRKS.find((qu) => qu.id === q)?.type === 'affliction'));
  let resultText = 'Le groupe veille, à tour de rôle, l\'oreille tendue vers les ténèbres.';
  if (candidates.length) {
    const target = candidates[randInt(rng, 0, candidates.length - 1)];
    const afflictionId = target.activeQuirks.find((q) => QUIRKS.find((qu) => qu.id === q)?.type === 'affliction')!;
    next = removeQuirkFromHero(next, target.heroId, afflictionId);
    resultText += ` ${getHeroById(target.heroId).name} semble retrouver un peu de calme.`;
  }
  return { run: next, resultText };
}
