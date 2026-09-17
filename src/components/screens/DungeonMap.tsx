import { useGameStore } from '../../store/gameStore';
import { getCurrentZone, getAvailableNodes } from '../../engine/run';
import { getHeroById } from '../../data/heroes';
import { getRelicById } from '../../data/relics';
import { HPBar, StressBar } from '../ui/Bars';
import { Button } from '../ui/Button';
import { Portrait } from '../ui/Portrait';
import './screens.css';

const NODE_ICONS: Record<string, string> = { combat: '⚔', event: '❖', camp: '🔥', boss: '☠' };
const NODE_LABELS: Record<string, string> = { combat: 'Combat', event: 'Événement', camp: 'Campement', boss: 'Boss' };

export function DungeonMap() {
  const run = useGameStore((s) => s.run);
  const chooseNode = useGameStore((s) => s.chooseNode);
  const abandonRun = useGameStore((s) => s.abandonRun);
  if (!run) return null;

  const zone = getCurrentZone(run);
  const available = new Set(getAvailableNodes(run).map((n) => n.id));
  const visited = new Set(run.visitedNodeIds);

  return (
    <div className="screen dungeon-map">
      <h1 className="gothic-title screen-title">{zone.name}</h1>
      <p className="screen-subtitle">{zone.description}</p>

      <p className="gold-badge">🜏 {run.gold} or · {run.relicIds.length} relique(s)</p>
      {run.relicIds.length > 0 && (
        <p className="screen-subtitle" style={{ margin: 0 }}>
          {run.relicIds.map((id) => getRelicById(id).name).join(' · ')}
        </p>
      )}

      <div className="labyrinth">
        {zone.rowTemplates.map((_, row) => {
          const rowNodes = run.mapNodes.filter((n) => n.row === row);
          return (
            <div key={row} className="labyrinth-row">
              {rowNodes.map((node) => {
                const isAvailable = available.has(node.id);
                const isVisited = visited.has(node.id);
                const classes = ['labyrinth-node', isAvailable ? 'available' : '', isVisited ? 'visited' : '', !isAvailable && !isVisited ? 'locked' : ''].filter(Boolean).join(' ');
                return (
                  <button
                    key={node.id}
                    className={classes}
                    disabled={!isAvailable}
                    onClick={() => chooseNode(node.id)}
                    aria-label={NODE_LABELS[node.type]}
                  >
                    <span className="labyrinth-node-icon">{NODE_ICONS[node.type]}</span>
                  </button>
                );
              })}
            </div>
          );
        })}
      </div>

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

      <Button variant="danger" block onClick={() => { if (confirm('Abandonner cette expédition ?')) abandonRun(); }}>
        Abandonner l'expédition
      </Button>
    </div>
  );
}
