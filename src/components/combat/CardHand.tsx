import type { CombatState } from '../../engine/types';
import { parseInstanceId } from '../../engine/deck';
import { getCardById } from '../../data/cards';
import { CardView } from './CardView';
import './combat.css';

export function CardHand({ combat, selectedInstanceId, onSelect }: { combat: CombatState; selectedInstanceId: string | null; onSelect: (instanceId: string) => void }) {
  return (
    <div className="hand-row">
      {combat.hand.map((instanceId) => {
        const { heroId, cardId } = parseInstanceId(instanceId);
        const card = getCardById(cardId);
        const hero = combat.heroes.find((h) => h.heroId === heroId);
        const disabled = !hero || hero.dead || combat.energy < card.cost || combat.phase !== 'playerTurn';
        return (
          <CardView
            key={instanceId}
            card={card}
            ownerHeroId={heroId}
            selected={selectedInstanceId === instanceId}
            disabled={disabled}
            onClick={() => onSelect(instanceId)}
          />
        );
      })}
    </div>
  );
}
