import type { Card } from '../../engine/types';
import { getHeroById } from '../../data/heroes';
import { CARD_TYPE_ICONS } from '../ui/icons';
import './combat.css';

export function CardView({ card, ownerHeroId, selected, disabled, onClick }: { card: Card; ownerHeroId: string; selected?: boolean; disabled?: boolean; onClick?: () => void }) {
  const owner = ownerHeroId !== 'neutral' ? getHeroById(ownerHeroId) : null;
  const accent = owner?.colorAccent ?? '#a89a86';
  const TypeIcon = CARD_TYPE_ICONS[card.type];
  const classes = ['card-view', selected ? 'selected' : '', disabled ? 'disabled' : ''].filter(Boolean).join(' ');
  return (
    <div className={classes} onClick={disabled ? undefined : onClick} style={{ borderColor: accent }}>
      <div className="card-top-row">
        <span className="card-cost">{card.cost}</span>
        {TypeIcon && <TypeIcon className="card-type-icon" style={{ color: accent }} />}
      </div>
      <div className="card-name">{card.name}</div>
      <div className="card-owner" style={{ color: accent }}>{owner ? owner.name : 'Butin'}</div>
      <div className="card-desc">{card.description}</div>
    </div>
  );
}
