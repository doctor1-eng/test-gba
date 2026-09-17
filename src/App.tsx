import { useEffect, useRef } from 'react';
import { useGameStore } from './store/gameStore';
import { initAudio, startAmbiance, stopAmbiance } from './engine/audio';
import { AppShell } from './components/layout/AppShell';
import { MainMenu } from './components/screens/MainMenu';
import { HeroSelect } from './components/screens/HeroSelect';
import { DungeonMap } from './components/screens/DungeonMap';
import { CombatScreen } from './components/screens/CombatScreen';
import { EventScreen } from './components/screens/EventScreen';
import { CampScreen } from './components/screens/CampScreen';
import { CardRewardScreen } from './components/screens/CardRewardScreen';
import { GameOverScreen } from './components/screens/GameOverScreen';
import { CodexScreen } from './components/screens/CodexScreen';
import { UpgradesScreen } from './components/screens/UpgradesScreen';

const AMBIANCE_SCREENS = new Set(['dungeonMap', 'combat', 'event', 'camp', 'cardReward']);

function CurrentScreen() {
  const screen = useGameStore((s) => s.screen);
  switch (screen) {
    case 'mainMenu': return <MainMenu />;
    case 'heroSelect': return <HeroSelect />;
    case 'dungeonMap': return <DungeonMap />;
    case 'combat': return <CombatScreen />;
    case 'event': return <EventScreen />;
    case 'camp': return <CampScreen />;
    case 'cardReward': return <CardRewardScreen />;
    case 'gameOver': return <GameOverScreen />;
    case 'codex': return <CodexScreen />;
    case 'upgrades': return <UpgradesScreen />;
    default: return null;
  }
}

function App() {
  const init = useGameStore((s) => s.init);
  const screen = useGameStore((s) => s.screen);
  const audioEnabled = useGameStore((s) => s.audioEnabled);
  const audioUnlocked = useRef(false);

  useEffect(() => {
    init();
  }, [init]);

  useEffect(() => {
    function unlock() {
      if (audioUnlocked.current) return;
      audioUnlocked.current = true;
      initAudio();
    }
    window.addEventListener('pointerdown', unlock, { once: true });
    return () => window.removeEventListener('pointerdown', unlock);
  }, []);

  useEffect(() => {
    if (audioEnabled && AMBIANCE_SCREENS.has(screen)) {
      startAmbiance();
    } else {
      stopAmbiance();
    }
  }, [screen, audioEnabled]);

  return (
    <AppShell>
      <CurrentScreen />
    </AppShell>
  );
}

export default App;
