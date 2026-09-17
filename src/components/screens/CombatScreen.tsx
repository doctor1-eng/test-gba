import { useEffect, useRef, useState } from 'react';
import { useGameStore } from '../../store/gameStore';
import { getCardById } from '../../data/cards';
import { parseInstanceId } from '../../engine/deck';
import { cardNeedsTarget, getValidTargetIds } from '../../engine/combat';
import { playSfx } from '../../engine/audio';
import { HeroPanel } from '../combat/HeroPanel';
import { EnemyPanel } from '../combat/EnemyPanel';
import { CardHand } from '../combat/CardHand';
import { CombatLog } from '../combat/CombatLog';
import { EnergyPips } from '../ui/Bars';
import { Button } from '../ui/Button';
import '../combat/combat.css';

export function CombatScreen() {
  const combat = useGameStore((s) => s.combat);
  const playCard = useGameStore((s) => s.playCard);
  const endPlayerTurn = useGameStore((s) => s.endPlayerTurn);
  const proceedAfterCombat = useGameStore((s) => s.proceedAfterCombat);
  const [selectedInstanceId, setSelectedInstanceId] = useState<string | null>(null);
  const lastPhase = useRef<string | null>(null);
  const phase = combat?.phase ?? null;

  useEffect(() => {
    if (!phase || phase === lastPhase.current) return;
    if (phase === 'victory') playSfx('victory');
    if (phase === 'defeat') playSfx('death');
    lastPhase.current = phase;
  }, [phase]);

  if (!combat) return null;

  const selectedCard = selectedInstanceId ? getCardById(parseInstanceId(selectedInstanceId).cardId) : null;
  const validTargets = selectedCard ? getValidTargetIds(selectedCard, combat) : [];

  function handleSelectCard(instanceId: string) {
    if (instanceId === selectedInstanceId) {
      setSelectedInstanceId(null);
      return;
    }
    const { cardId } = parseInstanceId(instanceId);
    const card = getCardById(cardId);
    if (cardNeedsTarget(card)) {
      setSelectedInstanceId(instanceId);
    } else {
      playCard(instanceId, undefined);
      playSfx('cardPlay');
      setSelectedInstanceId(null);
    }
  }

  function handleTargetClick(targetId: string) {
    if (!selectedInstanceId) return;
    playCard(selectedInstanceId, targetId);
    playSfx('cardPlay');
    setSelectedInstanceId(null);
  }

  const isOver = combat.phase === 'victory' || combat.phase === 'defeat';

  return (
    <div className="combat-screen">
      <div className="combat-top">
        <div className="enemy-row">
          {combat.enemies.map((e) => (
            <EnemyPanel
              key={e.instanceId}
              enemy={e}
              targetable={!!selectedInstanceId && validTargets.includes(e.instanceId)}
              onClick={() => handleTargetClick(e.instanceId)}
            />
          ))}
        </div>
        <div className="hero-row">
          {combat.heroes.map((h) => (
            <HeroPanel
              key={h.heroId}
              hero={h}
              targetable={!!selectedInstanceId && validTargets.includes(h.heroId)}
              onClick={() => handleTargetClick(h.heroId)}
            />
          ))}
        </div>
      </div>

      <CombatLog entries={combat.log} />

      <div className="combat-bottom">
        {isOver ? (
          <>
            <p className="gothic-title" style={{ textAlign: 'center', margin: '0.4rem 0' }}>
              {combat.phase === 'victory' ? 'Victoire' : 'Défaite'}
            </p>
            <Button variant="primary" block onClick={proceedAfterCombat}>Continuer</Button>
          </>
        ) : (
          <>
            <div className="energy-row">
              <EnergyPips current={combat.energy} max={combat.maxEnergy} />
              <Button variant="primary" small onClick={endPlayerTurn}>Fin du tour</Button>
            </div>
            <CardHand combat={combat} selectedInstanceId={selectedInstanceId} onSelect={handleSelectCard} />
          </>
        )}
      </div>
    </div>
  );
}
