import { useState } from 'react';
import { useGameStore } from '../../store/gameStore';
import { HEROES, PARTY_SIZE } from '../../data/heroes';
import { Button } from '../ui/Button';
import './screens.css';

export function HeroSelect() {
  const meta = useGameStore((s) => s.meta);
  const startNewRun = useGameStore((s) => s.startNewRun);
  const goTo = useGameStore((s) => s.goTo);
  const [selected, setSelected] = useState<string[]>([]);

  function toggle(heroId: string, unlocked: boolean) {
    if (!unlocked) return;
    setSelected((prev) => {
      if (prev.includes(heroId)) return prev.filter((id) => id !== heroId);
      if (prev.length >= PARTY_SIZE) return prev;
      return [...prev, heroId];
    });
  }

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">Choisissez Votre Groupe</h1>
      <p className="screen-subtitle">{selected.length}/{PARTY_SIZE} héros sélectionnés</p>

      <div className="hero-select-grid">
        {HEROES.map((hero) => {
          const unlocked = meta.unlockedHeroIds.includes(hero.id);
          const isSelected = selected.includes(hero.id);
          return (
            <div
              key={hero.id}
              className={`hero-select-card${isSelected ? ' selected' : ''}${!unlocked ? ' locked' : ''}`}
              onClick={() => toggle(hero.id, unlocked)}
            >
              <div className="hero-select-glyph" style={{ color: hero.colorAccent }}>{unlocked ? hero.portraitGlyph : '🔒'}</div>
              <div className="hero-select-name">{hero.name}</div>
              <div className="hero-select-archetype">{unlocked ? hero.archetype : `Verrouillé (${hero.unlockCost} 🜏)`}</div>
            </div>
          );
        })}
      </div>

      <Button
        variant="primary"
        block
        disabled={selected.length !== PARTY_SIZE}
        onClick={() => startNewRun(selected)}
      >
        Lancer l'expédition
      </Button>
      <Button block onClick={() => goTo('mainMenu')}>Retour</Button>
    </div>
  );
}
