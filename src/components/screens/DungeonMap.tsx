import { useGameStore } from '../../store/gameStore';
import { getCurrentZone } from '../../engine/run';
import { getHeroById } from '../../data/heroes';
import { getRelicById } from '../../data/relics';
import { HPBar, StressBar } from '../ui/Bars';
import { Button } from '../ui/Button';
import { Portrait } from '../ui/Portrait';
import './screens.css';

const FLOOR_ICONS: Record<string, string> = { combat: '⚔', event: '❖', camp: '🔥', boss: '☠' };

export function DungeonMap() {
  const run = useGameStore((s) => s.run);
  const enterFloor = useGameStore((s) => s.enterFloor);
  const abandonRun = useGameStore((s) => s.abandonRun);
  if (!run) return null;

  const zone = getCurrentZone(run);
  const currentFloor = zone.floors[run.floorIndex];

  return (
    <div className="screen dungeon-map">
      <h1 className="gothic-title screen-title">{zone.name}</h1>
      <p className="screen-subtitle">{zone.description}</p>

      <div className="floor-track">
        {zone.floors.map((f) => (
          <div key={f.index} className={`floor-node${f.index - 1 < run.floorIndex ? ' done' : ''}${f.index - 1 === run.floorIndex ? ' current' : ''}`}>
            {FLOOR_ICONS[f.type]}
          </div>
        ))}
      </div>

      <p className="gold-badge">🜏 {run.gold} or · {run.relicIds.length} relique(s)</p>
      {run.relicIds.length > 0 && (
        <p className="screen-subtitle" style={{ margin: 0 }}>
          {run.relicIds.map((id) => getRelicById(id).name).join(' · ')}
        </p>
      )}

      <div className="party-status-row">
        {run.heroes.map((h) => {
          const def = getHeroById(h.heroId);
          return (
            <div key={h.heroId} className="party-status-card">
              <Portrait iconKey={def.id} accent={def.colorAccent} glyphFallback={def.portraitGlyph} dead={h.dead} size={44} />
              <div className="party-status-bars">
                <span style={{ fontFamily: 'var(--font-display)', fontSize: '0.78rem' }}>{def.name}{h.dead ? ' (mort)' : ''}</span>
                {!h.dead && (
                  <>
                    <HPBar current={h.currentHp} max={h.maxHp} deathsDoor={h.currentHp <= 0} />
                    <StressBar value={h.stress} />
                  </>
                )}
              </div>
            </div>
          );
        })}
      </div>

      <Button variant="primary" block onClick={enterFloor}>
        {currentFloor.type === 'boss' ? 'Affronter le boss' : 'Avancer'}
      </Button>
      <Button variant="danger" block onClick={() => { if (confirm('Abandonner cette expédition ?')) abandonRun(); }}>
        Abandonner l'expédition
      </Button>
    </div>
  );
}
