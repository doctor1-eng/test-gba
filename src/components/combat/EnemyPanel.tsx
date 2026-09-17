import type { EnemyRuntimeState } from '../../engine/types';
import { getEnemyById } from '../../data/enemies';
import { HPBar } from '../ui/Bars';
import './combat.css';

export function EnemyPanel({ enemy, targetable, onClick }: { enemy: EnemyRuntimeState; targetable?: boolean; onClick?: () => void }) {
  const def = getEnemyById(enemy.enemyId);
  const classes = ['unit-panel', enemy.dead ? 'dead' : '', targetable ? 'targetable' : ''].filter(Boolean).join(' ');
  return (
    <div className={classes} onClick={targetable ? onClick : undefined}>
      {enemy.block > 0 && <span className="unit-block">🛡 {enemy.block}</span>}
      <div className="unit-glyph">{def.portraitGlyph}</div>
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
