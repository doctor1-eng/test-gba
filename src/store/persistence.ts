import type { CombatState, MetaState, NarrativeEvent, RunState } from '../engine/types';
import { createDefaultMeta } from '../engine/meta';

const META_KEY = 'dungeon_tales_meta_v1';
const SAVE_KEY = 'dungeon_tales_save_v1';
const SETTINGS_KEY = 'dungeon_tales_settings_v1';

export interface SaveGame {
  run: RunState;
  combat: CombatState | null;
  screen: string;
  currentEvent: NarrativeEvent | null;
  infoText: string | null;
  cardRewardOptionIds: string[] | null;
  lastCombatRewardGold: number;
}

export interface Settings {
  audioEnabled: boolean;
}

function safeGet(key: string): string | null {
  try {
    return localStorage.getItem(key);
  } catch {
    return null;
  }
}

function safeSet(key: string, value: string): void {
  try {
    localStorage.setItem(key, value);
  } catch {
    // localStorage indisponible (navigation privée, quota) : la partie continue en mémoire.
  }
}

function safeRemove(key: string): void {
  try {
    localStorage.removeItem(key);
  } catch {
    // ignore
  }
}

export function loadMeta(): MetaState {
  const raw = safeGet(META_KEY);
  if (!raw) return createDefaultMeta();
  try {
    return { ...createDefaultMeta(), ...JSON.parse(raw) } as MetaState;
  } catch {
    return createDefaultMeta();
  }
}

export function saveMeta(meta: MetaState): void {
  safeSet(META_KEY, JSON.stringify(meta));
}

export function loadSave(): SaveGame | null {
  const raw = safeGet(SAVE_KEY);
  if (!raw) return null;
  try {
    return JSON.parse(raw) as SaveGame;
  } catch {
    return null;
  }
}

export function saveSave(save: SaveGame): void {
  safeSet(SAVE_KEY, JSON.stringify(save));
}

export function clearSave(): void {
  safeRemove(SAVE_KEY);
}

export function loadSettings(): Settings {
  const raw = safeGet(SETTINGS_KEY);
  if (!raw) return { audioEnabled: true };
  try {
    return { audioEnabled: true, ...JSON.parse(raw) } as Settings;
  } catch {
    return { audioEnabled: true };
  }
}

export function saveSettings(settings: Settings): void {
  safeSet(SETTINGS_KEY, JSON.stringify(settings));
}
