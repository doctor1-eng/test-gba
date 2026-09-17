import type { RunHeroState } from './types';
import type { RngFn } from './rng';
import { shuffle } from './rng';

export function makeInstanceId(heroId: string, cardId: string, idx: number): string {
  return `${heroId}::${cardId}::${idx}`;
}

export function parseInstanceId(instanceId: string): { heroId: string; cardId: string } {
  const [heroId, cardId] = instanceId.split('::');
  return { heroId, cardId };
}

export function buildShuffledDeck(heroes: RunHeroState[], rng: RngFn): string[] {
  const instances: string[] = [];
  let counter = 0;
  for (const hero of heroes) {
    for (const cardId of hero.deckCardIds) {
      instances.push(makeInstanceId(hero.heroId, cardId, counter++));
    }
  }
  return shuffle(instances, rng);
}

export interface DrawResult {
  drawPile: string[];
  discardPile: string[];
  hand: string[];
}

export function drawUpTo(drawPile: string[], discardPile: string[], hand: string[], count: number, rng: RngFn): DrawResult {
  let draw = [...drawPile];
  let discard = [...discardPile];
  const nextHand = [...hand];
  for (let i = 0; i < count; i++) {
    if (draw.length === 0) {
      if (discard.length === 0) break;
      draw = shuffle(discard, rng);
      discard = [];
    }
    const card = draw.shift();
    if (card) nextHand.push(card);
  }
  return { drawPile: draw, discardPile: discard, hand: nextHand };
}
