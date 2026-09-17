import { useGameStore } from '../../store/gameStore';
import { Button } from '../ui/Button';
import './screens.css';

export function CampScreen() {
  const infoText = useGameStore((s) => s.infoText);
  const chooseCampAction = useGameStore((s) => s.chooseCampAction);
  const confirmEncounterContinue = useGameStore((s) => s.confirmEncounterContinue);

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">Campement</h1>
      <p className="camp-text">
        Un renfoncement à l'abri des regards. Le temps d'une pause, avant de replonger dans les ténèbres.
      </p>

      {infoText ? (
        <>
          <p className="camp-text" style={{ color: 'var(--color-gold-bright)' }}>{infoText}</p>
          <Button variant="primary" block onClick={confirmEncounterContinue}>Continuer</Button>
        </>
      ) : (
        <div className="choice-list">
          <Button block onClick={() => chooseCampAction('rest')}>Se reposer (soigne davantage)</Button>
          <Button block onClick={() => chooseCampAction('watch')}>Veiller (apaise l'esprit)</Button>
        </div>
      )}
    </div>
  );
}
