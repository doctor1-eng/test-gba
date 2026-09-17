import { useGameStore } from '../../store/gameStore';
import { computeGlobalFloorNumber } from '../../engine/run';
import { Button } from '../ui/Button';
import './screens.css';

export function GameOverScreen() {
  const run = useGameStore((s) => s.run);
  const meta = useGameStore((s) => s.meta);
  const goTo = useGameStore((s) => s.goTo);
  if (!run) return null;

  const victory = run.status === 'victory';
  const floorReached = computeGlobalFloorNumber(run);

  return (
    <div className="screen menu-screen">
      <div className="menu-logo">{victory ? '✝' : '☠'}</div>
      <h1 className="gothic-title screen-title">{victory ? 'Le Donjon est Vaincu' : 'L\'Expédition S\'achève Ici'}</h1>
      <p className="screen-subtitle">
        {victory
          ? 'Vous émergez des ténèbres, changés à jamais.'
          : 'Les ténèbres réclament un nouveau tribut. D\'autres suivront vos traces.'}
      </p>

      <p className="event-text" style={{ textAlign: 'center' }}>
        Étage atteint : {floorReached} · Or amassé : {run.gold}
      </p>
      <p className="gold-badge">Reliques anciennes gagnées : 🜏 {meta.ancientRelics}</p>

      <div className="menu-actions">
        <Button variant="primary" block onClick={() => goTo('mainMenu')}>Retour au menu</Button>
      </div>
    </div>
  );
}
