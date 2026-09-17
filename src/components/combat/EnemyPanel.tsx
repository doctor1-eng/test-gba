import type { EnemyRuntimeState } from '../../engine/types';
import { getEnemyById } from '../../data/enemies';
import { HPBar } from '../ui/Bars';
import { Portrait } from '../ui/Portrait';
import './combat.css';

function enemyAccent(zoneId: string, isBoss: boolean): string {
  if (isBoss) return '#c9a24b';
  return zoneId === 'catacombes' ? '#5a8a4a' : '#a3444a';
}

export function EnemyPanel({ enemy, targetable, onClick }: { enemy: EnemyRuntimeState; targetable?: boolean; onClick?: () => void }) {
  const def = getEnemyById(enemy.enemyId);
  const classes = ['unit-panel', enemy.dead ? 'dead' : '', targetable ? 'targetable' : ''].filter(Boolean).join(' ');
  return (
    <div className={classes} onClick={targetable ? onClick : undefined}>
      {enemy.block > 0 && <span className="unit-block">🛡 {enemy.block}</span>}
      <Portrait iconKey={def.id} accent={enemyAccent(def.zoneId, def.isBoss)} glyphFallback={def.portraitGlyph} dead={enemy.dead} size={52} />
      <div className="unit-name">{def.name}</div>
      <div className="unit-bars">
        <HPBar current={enemy.currentHp} max={enemy.maxHp} />
      </div>
      <div className="unit-statuses">
        {enemy.statuses.filter((s) => s.kind === 'bleed' || s.kind === 'poison').map((s, i) => (
          <span key={i} className={`status-icon ${s.kind}`}>{s.kind === 'bleed' ? '🩸' : '☠'} {s.amount}</span>
        ))}
        {enemy.statuses.filter((s) => s.kind === 'debuff').map((s, i) => (
          <span key={i} className="status-icon debuff">-{s.stat}</span>
        ))}
      </div>
    </div>
  );
}
