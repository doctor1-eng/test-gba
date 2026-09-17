import { useGameStore } from '../../store/gameStore';
import { Button } from '../ui/Button';
import './screens.css';

export function EventScreen() {
  const currentEvent = useGameStore((s) => s.currentEvent);
  const infoText = useGameStore((s) => s.infoText);
  const chooseEventChoice = useGameStore((s) => s.chooseEventChoice);
  const confirmEncounterContinue = useGameStore((s) => s.confirmEncounterContinue);
  if (!currentEvent) return null;

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">{currentEvent.title}</h1>
      <p className="event-text">{currentEvent.text}</p>

      {infoText ? (
        <>
          <p className="event-text" style={{ color: 'var(--color-gold-bright)' }}>{infoText}</p>
          <Button variant="primary" block onClick={confirmEncounterContinue}>Continuer</Button>
        </>
      ) : (
        <div className="choice-list">
          {currentEvent.choices.map((choice) => (
            <Button key={choice.id} block onClick={() => chooseEventChoice(choice.id)}>{choice.label}</Button>
          ))}
        </div>
      )}
    </div>
  );
}
