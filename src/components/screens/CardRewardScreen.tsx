import { useGameStore } from '../../store/gameStore';
import { CardView } from '../combat/CardView';
import { Button } from '../ui/Button';
import './screens.css';

export function CardRewardScreen() {
  const lastCombatRewardGold = useGameStore((s) => s.lastCombatRewardGold);
  const cardRewardOptions = useGameStore((s) => s.cardRewardOptions);
  const chooseCardReward = useGameStore((s) => s.chooseCardReward);

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">Butin</h1>
      <p className="screen-subtitle">Vous récupérez {lastCombatRewardGold} pièces d'or sur les corps.</p>
      <p className="event-text">Une carte attire votre regard. Une seule survivra au trajet.</p>

      <div className="reward-grid">
        {(cardRewardOptions ?? []).map((card) => (
          <CardView key={card.id} card={card} ownerHeroId={card.heroId} onClick={() => chooseCardReward(card.id)} />
        ))}
      </div>

      <Button block onClick={() => chooseCardReward(null)}>Ne rien prendre</Button>
    </div>
  );
}
