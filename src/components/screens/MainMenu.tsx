import { useGameStore } from '../../store/gameStore';
import { Button } from '../ui/Button';
import './screens.css';

export function MainMenu() {
  const meta = useGameStore((s) => s.meta);
  const hasSave = useGameStore((s) => s.hasSave);
  const goTo = useGameStore((s) => s.goTo);

  return (
    <div className="screen menu-screen">
      <div className="menu-logo">☠</div>
      <h1 className="gothic-title screen-title">Contes du Donjon</h1>
      <p className="screen-subtitle">« Descendez. Peu en reviennent entiers. »</p>

      <div className="menu-actions">
        {hasSave && (
          <Button variant="primary" block onClick={() => goTo('dungeonMap')}>Continuer l'expédition</Button>
        )}
        <Button variant={hasSave ? 'secondary' : 'primary'} block onClick={() => goTo('heroSelect')}>Nouvelle Expédition</Button>
        <Button block onClick={() => goTo('upgrades')}>Améliorations ({meta.ancientRelics} 🜏)</Button>
        <Button block onClick={() => goTo('codex')}>Codex</Button>
      </div>

      <p className="screen-subtitle" style={{ marginTop: '1.2rem' }}>
        Étage le plus profond atteint : {meta.bestFloorReached} · Expéditions : {meta.totalRuns} · Victoires : {meta.totalVictories}
      </p>
    </div>
  );
}
