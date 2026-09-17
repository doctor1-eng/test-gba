import { create } from 'zustand';
import type { Card, CombatState, MetaState, NarrativeEvent, RunState } from '../engine/types';
import type { RngFn } from '../engine/rng';
import { mulberry32, randInt, randomSeed } from '../engine/rng';
import { playCard as enginePlayCard, endPlayerTurn as engineEndPlayerTurn } from '../engine/combat';
import { startCombatForFloor, resolveEventChoice, resolveCamp } from '../engine/encounter';
import {
  createRun, getCurrentFloor, syncHeroesFromCombat, advanceFloor, addGold,
  generateCardRewardOptions, addCardToHeroDeck, livingHeroCount, computeGlobalFloorNumber,
} from '../engine/run';
import { createDefaultMeta, purchaseUpgrade as metaPurchaseUpgrade, recordRunResult } from '../engine/meta';
import { EVENTS } from '../data/events';
import { getCardById, CARDS } from '../data/cards';
import { loadMeta, saveMeta, loadSave, saveSave, clearSave, loadSettings, saveSettings } from './persistence';

export type Screen = 'mainMenu' | 'heroSelect' | 'dungeonMap' | 'combat' | 'event' | 'camp' | 'cardReward' | 'gameOver' | 'codex' | 'upgrades';

let engineRng: RngFn = mulberry32(randomSeed());

interface GameState {
  meta: MetaState;
  run: RunState | null;
  combat: CombatState | null;
  screen: Screen;
  currentEvent: NarrativeEvent | null;
  infoText: string | null;
  cardRewardOptions: Card[] | null;
  lastCombatRewardGold: number;
  audioEnabled: boolean;
  hasSave: boolean;

  init: () => void;
  startNewRun: (partyHeroIds: string[]) => void;
  goTo: (screen: Screen) => void;
  enterFloor: () => void;
  playCard: (instanceId: string, targetId?: string) => void;
  endPlayerTurn: () => void;
  proceedAfterCombat: () => void;
  chooseCardReward: (cardId: string | null) => void;
  chooseEventChoice: (choiceId: string) => void;
  chooseCampAction: (action: 'rest' | 'watch') => void;
  confirmEncounterContinue: () => void;
  purchaseUpgrade: (upgradeId: string) => void;
  abandonRun: () => void;
  toggleAudio: () => void;
}

function persist(state: Pick<GameState, 'run' | 'combat' | 'screen' | 'currentEvent' | 'infoText' | 'cardRewardOptions' | 'lastCombatRewardGold'>) {
  if (!state.run) {
    clearSave();
    return;
  }
  saveSave({
    run: state.run,
    combat: state.combat,
    screen: state.screen,
    currentEvent: state.currentEvent,
    infoText: state.infoText,
    cardRewardOptionIds: state.cardRewardOptions ? state.cardRewardOptions.map((c) => c.id) : null,
    lastCombatRewardGold: state.lastCombatRewardGold,
  });
}

function finalizeRunEnd(run: RunState, victory: boolean, meta: MetaState): { run: RunState; meta: MetaState } {
  const floorReached = computeGlobalFloorNumber(run);
  const updatedRun: RunState = { ...run, status: victory ? 'victory' : 'defeat' };
  const updatedMeta = recordRunResult(meta, { floorReached, zoneId: run.zoneId, gold: run.gold, victory });
  saveMeta(updatedMeta);
  clearSave();
  return { run: updatedRun, meta: updatedMeta };
}

export const useGameStore = create<GameState>((set, get) => ({
  meta: createDefaultMeta(),
  run: null,
  combat: null,
  screen: 'mainMenu',
  currentEvent: null,
  infoText: null,
  cardRewardOptions: null,
  lastCombatRewardGold: 0,
  audioEnabled: true,
  hasSave: false,

  init: () => {
    const meta = loadMeta();
    const settings = loadSettings();
    const save = loadSave();
    if (save && save.run.status === 'inProgress') {
      engineRng = mulberry32(save.run.seed + save.run.floorIndex + save.run.gold + 1);
      set({
        meta,
        run: save.run,
        combat: save.combat,
        screen: save.screen as Screen,
        currentEvent: save.currentEvent,
        infoText: save.infoText,
        cardRewardOptions: save.cardRewardOptionIds ? save.cardRewardOptionIds.map((id) => getCardById(id)) : null,
        lastCombatRewardGold: save.lastCombatRewardGold,
        audioEnabled: settings.audioEnabled,
        hasSave: true,
      });
    } else {
      set({ meta, audioEnabled: settings.audioEnabled, hasSave: false });
    }
  },

  startNewRun: (partyHeroIds) => {
    const seed = randomSeed();
    engineRng = mulberry32(seed);
    const meta = get().meta;
    const run = createRun(partyHeroIds, meta, seed);
    const next = { run, combat: null, screen: 'dungeonMap' as Screen, currentEvent: null, infoText: null, cardRewardOptions: null, lastCombatRewardGold: 0, hasSave: true };
    set(next);
    persist(next);
  },

  goTo: (screen) => set({ screen }),

  enterFloor: () => {
    const { run } = get();
    if (!run) return;
    const floor = getCurrentFloor(run);
    if (floor.type === 'combat' || floor.type === 'boss') {
      const combat = startCombatForFloor(run, engineRng);
      const next = { combat, screen: 'combat' as Screen };
      set(next);
      persist({ ...get(), ...next });
    } else if (floor.type === 'event') {
      const event = EVENTS[randInt(engineRng, 0, EVENTS.length - 1)];
      const next = { currentEvent: event, infoText: null, screen: 'event' as Screen };
      set(next);
      persist({ ...get(), ...next });
    } else if (floor.type === 'camp') {
      const next = { screen: 'camp' as Screen, infoText: null };
      set(next);
      persist({ ...get(), ...next });
    }
  },

  playCard: (instanceId, targetId) => {
    const { combat } = get();
    if (!combat) return;
    const next = enginePlayCard(combat, instanceId, targetId, engineRng);
    set({ combat: next });
    persist({ ...get(), combat: next });
  },

  endPlayerTurn: () => {
    const { combat } = get();
    if (!combat) return;
    const next = engineEndPlayerTurn(combat, engineRng);
    set({ combat: next });
    persist({ ...get(), combat: next });
  },

  proceedAfterCombat: () => {
    const { combat, run, meta } = get();
    if (!combat || !run) return;

    let updatedRun = syncHeroesFromCombat(run, combat);

    if (combat.phase === 'defeat' || livingHeroCount(updatedRun) === 0) {
      const { run: finalRun, meta: finalMeta } = finalizeRunEnd(updatedRun, false, meta);
      set({ run: finalRun, meta: finalMeta, combat: null, screen: 'gameOver' });
      return;
    }

    const goldReward = randInt(engineRng, 8, 16) * (combat.isBossFight ? 3 : 1);
    updatedRun = addGold(updatedRun, goldReward);
    const rewardOptions = generateCardRewardOptions(updatedRun.partyHeroIds, engineRng, 3);
    const next = { run: updatedRun, combat: null, lastCombatRewardGold: goldReward, cardRewardOptions: rewardOptions, screen: 'cardReward' as Screen };
    set(next);
    persist({ ...get(), ...next });
  },

  chooseCardReward: (cardId) => {
    const { run, meta } = get();
    if (!run) return;
    let updatedRun = run;
    if (cardId) {
      const card = CARDS.find((c) => c.id === cardId);
      if (card) {
        let heroId = card.heroId;
        if (heroId === 'neutral') {
          const aliveHeroes = updatedRun.heroes.filter((h) => !h.dead);
          if (aliveHeroes.length) {
            heroId = aliveHeroes.reduce((min, h) => (h.deckCardIds.length < min.deckCardIds.length ? h : min), aliveHeroes[0]).heroId;
          }
        }
        updatedRun = addCardToHeroDeck(updatedRun, heroId, cardId);
      }
    }
    updatedRun = advanceFloor(updatedRun);

    if (updatedRun.status === 'victory') {
      const { run: finalRun, meta: finalMeta } = finalizeRunEnd(updatedRun, true, meta);
      set({ run: finalRun, meta: finalMeta, cardRewardOptions: null, screen: 'gameOver' });
      return;
    }
    const next = { run: updatedRun, cardRewardOptions: null, lastCombatRewardGold: 0, screen: 'dungeonMap' as Screen };
    set(next);
    persist({ ...get(), ...next });
  },

  chooseEventChoice: (choiceId) => {
    const { run, currentEvent } = get();
    if (!run || !currentEvent) return;
    const { run: updatedRun, resultText } = resolveEventChoice(run, currentEvent, choiceId, engineRng);
    const next = { run: updatedRun, infoText: resultText };
    set(next);
    persist({ ...get(), ...next });
  },

  chooseCampAction: (action) => {
    const { run } = get();
    if (!run) return;
    const { run: updatedRun, resultText } = resolveCamp(run, action, engineRng);
    const next = { run: updatedRun, infoText: resultText };
    set(next);
    persist({ ...get(), ...next });
  },

  confirmEncounterContinue: () => {
    const { run, meta } = get();
    if (!run) return;
    let updatedRun = advanceFloor(run);
    if (updatedRun.status === 'victory') {
      const { run: finalRun, meta: finalMeta } = finalizeRunEnd(updatedRun, true, meta);
      set({ run: finalRun, meta: finalMeta, currentEvent: null, infoText: null, screen: 'gameOver' });
      return;
    }
    const next = { run: updatedRun, currentEvent: null, infoText: null, screen: 'dungeonMap' as Screen };
    set(next);
    persist({ ...get(), ...next });
  },

  purchaseUpgrade: (upgradeId) => {
    const meta = get().meta;
    const updatedMeta = metaPurchaseUpgrade(meta, upgradeId);
    set({ meta: updatedMeta });
    saveMeta(updatedMeta);
  },

  abandonRun: () => {
    clearSave();
    set({ run: null, combat: null, currentEvent: null, infoText: null, cardRewardOptions: null, screen: 'mainMenu', hasSave: false });
  },

  toggleAudio: () => {
    const audioEnabled = !get().audioEnabled;
    set({ audioEnabled });
    saveSettings({ audioEnabled });
  },
}));
