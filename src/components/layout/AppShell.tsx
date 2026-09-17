import type { ReactNode } from 'react';
import { useGameStore } from '../../store/gameStore';
import '../screens/screens.css';

const SCREEN_TITLES: Record<string, string> = {
  mainMenu: 'Contes du Donjon',
  heroSelect: 'Choix du Groupe',
  dungeonMap: 'Le Donjon',
  combat: 'Combat',
  event: 'Événement',
  camp: 'Campement',
  cardReward: 'Butin',
  gameOver: 'Fin de l\'Expédition',
  codex: 'Codex',
  upgrades: 'Améliorations',
};

export function AppShell({ children }: { children: ReactNode }) {
  const screen = useGameStore((s) => s.screen);
  const audioEnabled = useGameStore((s) => s.audioEnabled);
  const toggleAudio = useGameStore((s) => s.toggleAudio);

  return (
    <div style={{ display: 'flex', flexDirection: 'column', flex: 1, minHeight: 0 }}>
      <div className="topbar">
        <span className="topbar-title gothic-title">{SCREEN_TITLES[screen] ?? ''}</span>
        <div className="topbar-actions">
          <button
            aria-label="Basculer le son"
            onClick={toggleAudio}
            style={{ background: 'none', border: 'none', color: 'var(--color-text-dim)', fontSize: '1.1rem', cursor: 'pointer' }}
          >
            {audioEnabled ? '🔊' : '🔇'}
          </button>
        </div>
      </div>
      <div style={{ flex: 1, minHeight: 0, display: 'flex', flexDirection: 'column' }}>
        {children}
      </div>
    </div>
  );
}
