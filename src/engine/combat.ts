import { produce } from 'immer';
import type {
  Card, CombatState, CombatLogEntry, Effect, EnemyRuntimeState, HeroRuntimeState,
  RunHeroState, StatusEffectInstance,
} from './types';
import type { RngFn } from './rng';
import { mulberry32, randInt, pickWeighted } from './rng';
import { buildShuffledDeck, drawUpTo, parseInstanceId } from './deck';
import { applyStressDelta } from './stress';
import { getCardById } from '../data/cards';
import { getEnemyById } from '../data/enemies';
import { getQuirkById } from '../data/quirks';
import { HEROES } from '../data/heroes';

type Unit = HeroRuntimeState | EnemyRuntimeState;

function isHeroUnit(u: Unit): u is HeroRuntimeState {
  return (u as HeroRuntimeState).heroId !== undefined;
}

function heroName(heroId: string): string {
  return HEROES.find((h) => h.id === heroId)?.name ?? heroId;
}

function unitName(u: Unit): string {
  return isHeroUnit(u) ? heroName(u.heroId) : getEnemyById(u.enemyId).name;
}

function pushLog(draft: CombatState, text: string) {
  draft.log.push({ id: `${draft.log.length}-${Math.random().toString(36).slice(2, 8)}`, text });
}

export function createRng(seed: number): RngFn {
  return mulberry32(seed);
}

// ---- Initialisation ----

export interface CombatBonuses {
  startEnergy: number;
  stressResist: number;
}

export function initCombat(party: RunHeroState[], enemyIds: string[], isBoss: boolean, rng: RngFn, bonuses: CombatBonuses): CombatState {
  const livingParty = party.filter((h) => !h.dead);
  const heroes: HeroRuntimeState[] = livingParty.map((h) => ({
    heroId: h.heroId,
    currentHp: h.currentHp,
    maxHp: h.maxHp,
    stress: h.stress,
    stressThreshold: 100,
    block: 0,
    statuses: [],
    atDeathsDoor: h.currentHp <= 0,
    dead: false,
    activeQuirks: [...h.activeQuirks],
    temporaryQuirk: undefined,
  }));

  for (const hero of heroes) {
    for (const quirkId of hero.activeQuirks) {
      const quirk = getQuirkById(quirkId);
      if (quirk.effect.kind === 'buff' || quirk.effect.kind === 'debuff') {
        hero.statuses.push({ kind: quirk.effect.kind, stat: quirk.effect.stat, amount: quirk.effect.amount, duration: 99 });
      }
    }
    if (bonuses.stressResist > 0) {
      hero.statuses.push({ kind: 'buff', stat: 'stressResist', amount: bonuses.stressResist, duration: 99 });
    }
  }

  const enemies: EnemyRuntimeState[] = enemyIds.map((id, idx) => {
    const def = getEnemyById(id);
    return { instanceId: `${id}__${idx}`, enemyId: id, currentHp: def.hp, maxHp: def.hp, block: 0, statuses: [], dead: false };
  });

  const drawPile = buildShuffledDeck(livingParty, rng);
  const draw = drawUpTo(drawPile, [], [], 5, rng);
  const maxEnergy = 3 + bonuses.startEnergy;

  const log: CombatLogEntry[] = [{ id: 'start', text: isBoss ? 'Une présence bien plus grande vous fait face.' : 'Le combat commence.' }];

  return {
    heroes,
    enemies,
    drawPile: draw.drawPile,
    hand: draw.hand,
    discardPile: draw.discardPile,
    exhaustPile: [],
    energy: maxEnergy,
    maxEnergy,
    round: 1,
    phase: 'playerTurn',
    log,
    isBossFight: isBoss,
  };
}

// ---- Dégâts & modificateurs ----

function statModifier(u: Unit, stat: 'damage' | 'defense' | 'stressResist', kind: 'buff' | 'debuff'): number {
  return u.statuses.filter((s) => s.kind === kind && s.stat === stat).reduce((sum, s) => sum + s.amount, 0);
}

function damageModifierFor(u: Unit): number {
  return statModifier(u, 'damage', 'buff') - statModifier(u, 'damage', 'debuff');
}

function defenseModifierFor(u: Unit): number {
  return statModifier(u, 'defense', 'debuff') - statModifier(u, 'defense', 'buff');
}

function computeDamage(effect: Extract<Effect, { kind: 'damage' }>, source: Unit | null, target: Unit, rng: RngFn): number {
  let dmg = effect.amount + randInt(rng, -effect.variance, effect.variance);
  if (source) dmg += damageModifierFor(source);
  if (effect.executeBelowPercent !== undefined && target.maxHp > 0 && target.currentHp / target.maxHp <= effect.executeBelowPercent) {
    dmg += effect.executeBonus ?? 0;
  }
  dmg += defenseModifierFor(target);
  if (!isHeroUnit(target)) {
    const def = getEnemyById(target.enemyId);
    const resist = def.resistances?.damage ?? 0;
    dmg *= 1 - resist;
  }
  return Math.max(0, Math.round(dmg));
}

function resolveDamageToHero(draft: CombatState, hero: HeroRuntimeState, amount: number, rng: RngFn) {
  const name = heroName(hero.heroId);
  const absorbed = Math.min(hero.block, amount);
  hero.block -= absorbed;
  const remaining = amount - absorbed;
  if (remaining <= 0) return;

  if (hero.atDeathsDoor) {
    const survives = rng() > 0.35;
    if (!survives) {
      hero.dead = true;
      hero.currentHp = 0;
      pushLog(draft, `${name} succombe à ses blessures.`);
    } else {
      pushLog(draft, `${name} frôle la mort, mais s'accroche encore à la vie.`);
      for (const other of draft.heroes) {
        if (other !== hero && !other.dead) applyStressDelta(other, 15, rng, draft.log);
      }
    }
  } else {
    hero.currentHp = Math.max(0, hero.currentHp - remaining);
    if (hero.currentHp <= 0) {
      hero.currentHp = 0;
      hero.atDeathsDoor = true;
      pushLog(draft, `${name} est à l'article de la mort.`);
    }
  }
}

function resolveDamageToEnemy(draft: CombatState, enemy: EnemyRuntimeState, amount: number) {
  const absorbed = Math.min(enemy.block, amount);
  enemy.block -= absorbed;
  const remaining = amount - absorbed;
  if (remaining <= 0) return;
  enemy.currentHp = Math.max(0, enemy.currentHp - remaining);
  if (enemy.currentHp <= 0) {
    enemy.dead = true;
    pushLog(draft, `${getEnemyById(enemy.enemyId).name} s'effondre.`);
  }
}

function applyEffects(draft: CombatState, effects: Effect[], source: Unit | null, cardTargets: Unit[], rng: RngFn) {
  for (const effect of effects) {
    const targets = effect.target === 'self' && source ? [source] : cardTargets;
    for (const target of targets) {
      switch (effect.kind) {
        case 'damage': {
          const dmg = computeDamage(effect, source, target, rng);
          if (isHeroUnit(target)) resolveDamageToHero(draft, target, dmg, rng);
          else resolveDamageToEnemy(draft, target, dmg);
          break;
        }
        case 'heal': {
          target.currentHp = Math.min(target.maxHp, target.currentHp + effect.amount);
          if (isHeroUnit(target) && target.currentHp > 0) target.atDeathsDoor = false;
          pushLog(draft, `${unitName(target)} récupère ${effect.amount} PV.`);
          break;
        }
        case 'stress': {
          if (isHeroUnit(target)) applyStressDelta(target, effect.amount, rng, draft.log);
          break;
        }
        case 'block': {
          target.block += effect.amount;
          break;
        }
        case 'buff':
        case 'debuff': {
          const status: StatusEffectInstance = { kind: effect.kind, stat: effect.stat, amount: effect.amount, duration: effect.duration };
          target.statuses.push(status);
          break;
        }
        case 'bleed':
        case 'poison': {
          target.statuses.push({ kind: effect.kind, amount: effect.amount, duration: effect.duration });
          break;
        }
        case 'draw': {
          const result = drawUpTo(draft.drawPile, draft.discardPile, draft.hand, effect.amount, rng);
          draft.drawPile = result.drawPile;
          draft.discardPile = result.discardPile;
          draft.hand = result.hand;
          break;
        }
        case 'energy': {
          draft.energy += effect.amount;
          break;
        }
      }
    }
  }
}

// ---- Ciblage ----

function resolveCardTargets(draft: CombatState, card: Card, caster: HeroRuntimeState, targetId: string | undefined, rng: RngFn): Unit[] {
  switch (card.target) {
    case 'self':
      return [caster];
    case 'singleAlly': {
      const h = targetId ? draft.heroes.find((h) => h.heroId === targetId && !h.dead) : caster;
      return h ? [h] : [];
    }
    case 'allAllies':
      return draft.heroes.filter((h) => !h.dead);
    case 'singleEnemy': {
      const e = draft.enemies.find((e) => e.instanceId === targetId && !e.dead);
      return e ? [e] : [];
    }
    case 'allEnemies':
      return draft.enemies.filter((e) => !e.dead);
    case 'randomEnemy': {
      const alive = draft.enemies.filter((e) => !e.dead);
      if (!alive.length) return [];
      return [alive[randInt(rng, 0, alive.length - 1)]];
    }
  }
}

export function cardNeedsTarget(card: Card): boolean {
  return card.target === 'singleEnemy' || card.target === 'singleAlly';
}

export function getValidTargetIds(card: Card, state: CombatState): string[] {
  if (card.target === 'singleEnemy') return state.enemies.filter((e) => !e.dead).map((e) => e.instanceId);
  if (card.target === 'singleAlly') return state.heroes.filter((h) => !h.dead).map((h) => h.heroId);
  return [];
}

// ---- Fin de combat ----

function checkCombatEndInPlace(draft: CombatState): boolean {
  if (draft.enemies.length > 0 && draft.enemies.every((e) => e.dead)) {
    draft.phase = 'victory';
    pushLog(draft, 'Le silence retombe. Vous avez survécu.');
    return true;
  }
  if (draft.heroes.every((h) => h.dead)) {
    draft.phase = 'defeat';
    pushLog(draft, "Les ténèbres l'emportent.");
    return true;
  }
  return false;
}

// ---- Actions du joueur ----

export function playCard(state: CombatState, instanceId: string, targetId: string | undefined, rng: RngFn): CombatState {
  return produce(state, (draft) => {
    if (draft.phase !== 'playerTurn') return;
    const handIdx = draft.hand.indexOf(instanceId);
    if (handIdx === -1) return;
    const { heroId, cardId } = parseInstanceId(instanceId);
    const card = getCardById(cardId);
    const hero = draft.heroes.find((h) => h.heroId === heroId);
    if (!hero || hero.dead) return;
    if (draft.energy < card.cost) return;

    draft.hand.splice(handIdx, 1);
    draft.discardPile.push(instanceId);
    draft.energy -= card.cost;

    if (hero.temporaryQuirk) {
      const quirk = getQuirkById(hero.temporaryQuirk.quirkId);
      if (quirk.refuseOrderChance && rng() < quirk.refuseOrderChance) {
        pushLog(draft, `${heroName(heroId)} refuse d'obéir, paralysé par la terreur.`);
        checkCombatEndInPlace(draft);
        return;
      }
      if (quirk.selfDamageChance && rng() < quirk.selfDamageChance) {
        pushLog(draft, `${heroName(heroId)} se retourne contre lui-même !`);
        if (quirk.effect.kind === 'damage') {
          const dmg = computeDamage(quirk.effect, null, hero, rng);
          resolveDamageToHero(draft, hero, dmg, rng);
        }
        checkCombatEndInPlace(draft);
        return;
      }
    }

    const targets = resolveCardTargets(draft, card, hero, targetId, rng);
    if (targets.length === 0) { checkCombatEndInPlace(draft); return; }
    pushLog(draft, `${heroName(heroId)} joue ${card.name}.`);
    applyEffects(draft, card.effects, hero, targets, rng);
    checkCombatEndInPlace(draft);
  });
}

function tickStatuses(draft: CombatState, u: Unit, rng: RngFn) {
  let dotDamage = 0;
  const remaining: StatusEffectInstance[] = [];
  for (const s of u.statuses) {
    if (s.kind === 'bleed' || s.kind === 'poison') dotDamage += s.amount;
    const dur = s.duration - 1;
    if (dur > 0) remaining.push({ ...s, duration: dur });
  }
  u.statuses = remaining;
  if (dotDamage > 0) {
    pushLog(draft, `${unitName(u)} subit ${dotDamage} dégâts de saignement/poison.`);
    if (isHeroUnit(u)) resolveDamageToHero(draft, u, dotDamage, rng);
    else resolveDamageToEnemy(draft, u, dotDamage);
  }
}

function startNewRound(draft: CombatState, rng: RngFn) {
  for (const hero of draft.heroes) if (!hero.dead) tickStatuses(draft, hero, rng);
  for (const enemy of draft.enemies) if (!enemy.dead) tickStatuses(draft, enemy, rng);
  if (checkCombatEndInPlace(draft)) return;

  for (const h of draft.heroes) h.block = 0;
  for (const e of draft.enemies) e.block = 0;

  draft.round += 1;
  draft.energy = draft.maxEnergy;
  draft.discardPile.push(...draft.hand);
  draft.hand = [];
  const drawResult = drawUpTo(draft.drawPile, draft.discardPile, draft.hand, 5, rng);
  draft.drawPile = drawResult.drawPile;
  draft.discardPile = drawResult.discardPile;
  draft.hand = drawResult.hand;
  draft.phase = 'playerTurn';
  pushLog(draft, `--- Tour ${draft.round} ---`);
}

export function endPlayerTurn(state: CombatState, rng: RngFn): CombatState {
  return produce(state, (draft) => {
    if (draft.phase !== 'playerTurn') return;
    draft.phase = 'enemyTurn';
    pushLog(draft, '--- Les ombres se retournent contre vous ---');

    let ended = false;
    for (const enemy of draft.enemies) {
      if (enemy.dead) continue;
      const def = getEnemyById(enemy.enemyId);
      const hpPercent = enemy.maxHp > 0 ? enemy.currentHp / enemy.maxHp : 0;
      const available = def.moves.filter(
        (m) => (m.minHpPercent === undefined || hpPercent >= m.minHpPercent) && (m.maxHpPercent === undefined || hpPercent <= m.maxHpPercent),
      );
      const pool = available.length ? available : def.moves;
      const move = pickWeighted(pool, rng);
      pushLog(draft, `${def.name} utilise ${move.name}.`);

      if (move.summonEnemyId) {
        const summonDef = getEnemyById(move.summonEnemyId);
        draft.enemies.push({
          instanceId: `${move.summonEnemyId}__${draft.enemies.length}__${draft.round}`,
          enemyId: move.summonEnemyId,
          currentHp: summonDef.hp,
          maxHp: summonDef.hp,
          block: 0,
          statuses: [],
          dead: false,
        });
        pushLog(draft, `${summonDef.name} rejoint le combat !`);
      }

      const aliveHeroes = draft.heroes.filter((h) => !h.dead);
      let targets: Unit[] = [];
      if (move.target === 'self') targets = [enemy];
      else if (move.target === 'allHeroes') targets = aliveHeroes;
      else if ((move.target === 'singleHero' || move.target === 'randomHero') && aliveHeroes.length) {
        targets = [aliveHeroes[randInt(rng, 0, aliveHeroes.length - 1)]];
      }

      if (targets.length && move.effects.length) {
        applyEffects(draft, move.effects, enemy, targets, rng);
        if (def.stressDamageOnHit > 0) {
          for (const t of targets) {
            if (isHeroUnit(t) && !t.dead) applyStressDelta(t, def.stressDamageOnHit, rng, draft.log);
          }
        }
      }

      if (checkCombatEndInPlace(draft)) { ended = true; break; }
    }

    if (ended) return;
    startNewRound(draft, rng);
  });
}
