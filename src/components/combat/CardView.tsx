import type { Card } from '../../engine/types';
import { getHeroById } from '../../data/heroes';
import './combat.css';

export function CardView({ card, ownerHeroId, selected, disabled, onClick }: { card: Card; ownerHeroId: string; selected?: boolean; disabled?: boolean; onClick?: () => void }) {
  const owner = ownerHeroId !== 'neutral' ? getHeroById(ownerHeroId) : null;
  const classes = ['card-view', selected ? 'selected' : '', disabled ? 'disabled' : ''].filter(Boolean).join(' ');
  return (
    <div className={classes} onClick={disabled ? undefined : onClick} style={{ borderColor: owner?.colorAccent }}>
      <span className="card-cost">{card.cost}</span>
      <div className="card-name">{card.name}</div>
      <div className="card-owner" style={{ color: owner?.colorAccent }}>{owner ? owner.name : 'Butin'}</div>
      <div className="card-desc">{card.description}</div>
    </div>
  );
}
