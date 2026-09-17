import type { HeroRuntimeState, CombatLogEntry } from './types';
import type { RngFn } from './rng';
import { rollRandomQuirk } from '../data/quirks';
import { HEROES } from '../data/heroes';

const VIRTUE_CHANCE = 0.35;

function heroName(heroId: string): string {
  return HEROES.find((h) => h.id === heroId)?.name ?? heroId;
}

// Applique un changement de stress et déclenche, si le seuil est atteint, une résolution
// (affliction ou vertu) façon Darkest Dungeon. Mute directement le draft immer fourni.
export function applyStressDelta(hero: HeroRuntimeState, amount: number, rng: RngFn, log: CombatLogEntry[]) {
  if (hero.dead) return;
  if (amount === 0) return;

  if (amount > 0) {
    const stressResist = hero.statuses
      .filter((s) => s.kind === 'buff' && s.stat === 'stressResist')
      .reduce((sum, s) => sum + s.amount, 0);
    amount = Math.max(0, amount - stressResist);
  }

  hero.stress = Math.max(0, Math.min(200, hero.stress + amount));

  if (hero.stress >= hero.stressThreshold && !hero.temporaryQuirk) {
    resolveStress(hero, rng, log);
  }
}

function resolveStress(hero: HeroRuntimeState, rng: RngFn, log: CombatLogEntry[]) {
  const isVirtue = rng() < VIRTUE_CHANCE;
  const quirk = rollRandomQuirk(isVirtue ? 'virtue' : 'affliction', rng);
  hero.temporaryQuirk = { quirkId: quirk.id, roundsLeft: 99 };
  hero.stress = 50;

  const name = heroName(hero.heroId);
  if (isVirtue) {
    log.push({ id: `${Date.now()}-${Math.random()}`, text: `${name} trouve une force insoupçonnée : ${quirk.name}.` });
  } else {
    log.push({ id: `${Date.now()}-${Math.random()}`, text: `${name} craque sous la pression : ${quirk.name}.` });
  }

  if (quirk.effect.kind === 'heal') {
    hero.currentHp = Math.min(hero.maxHp, hero.currentHp + quirk.effect.amount);
  } else if (quirk.effect.kind === 'buff' || quirk.effect.kind === 'debuff') {
    hero.statuses.push({ kind: quirk.effect.kind, stat: quirk.effect.stat, amount: quirk.effect.amount, duration: quirk.effect.duration });
  }
}
