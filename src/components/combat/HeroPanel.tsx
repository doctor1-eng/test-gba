import type { HeroRuntimeState } from '../../engine/types';
import { getHeroById } from '../../data/heroes';
import { getQuirkById } from '../../data/quirks';
import { HPBar, StressBar } from '../ui/Bars';
import './combat.css';

export function HeroPanel({ hero, targetable, onClick }: { hero: HeroRuntimeState; targetable?: boolean; onClick?: () => void }) {
  const def = getHeroById(hero.heroId);
  const classes = ['unit-panel', hero.dead ? 'dead' : '', hero.atDeathsDoor && !hero.dead ? 'deaths-door' : '', targetable ? 'targetable' : ''].filter(Boolean).join(' ');
  return (
    <div className={classes} onClick={targetable ? onClick : undefined}>
      {hero.block > 0 && <span className="unit-block">🛡 {hero.block}</span>}
      <div className="unit-glyph" style={{ color: def.colorAccent }}>{def.portraitGlyph}</div>
      <div className="unit-name">{def.name}</div>
      <div className="unit-bars">
        <HPBar current={hero.currentHp} max={hero.maxHp} deathsDoor={hero.atDeathsDoor && !hero.dead} />
        <StressBar value={hero.stress} threshold={hero.stressThreshold} />
      </div>
      <div className="unit-statuses">
        {hero.temporaryQuirk && (
          <span className="status-icon buff" title={getQuirkById(hero.temporaryQuirk.quirkId).description}>
            {getQuirkById(hero.temporaryQuirk.quirkId).name}
          </span>
        )}
        {hero.statuses.filter((s) => s.kind === 'bleed' || s.kind === 'poison').map((s, i) => (
          <span key={i} className={`status-icon ${s.kind}`}>{s.kind === 'bleed' ? '🩸' : '☠'} {s.amount}</span>
        ))}
      </div>
    </div>
  );
}
