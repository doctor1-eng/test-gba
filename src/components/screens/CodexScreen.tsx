import { useState } from 'react';
import { useGameStore } from '../../store/gameStore';
import { HEROES } from '../../data/heroes';
import { CARDS } from '../../data/cards';
import { ENEMIES } from '../../data/enemies';
import { Button } from '../ui/Button';
import './screens.css';

type Tab = 'heroes' | 'cards' | 'enemies';

export function CodexScreen() {
  const goTo = useGameStore((s) => s.goTo);
  const [tab, setTab] = useState<Tab>('heroes');

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">Codex</h1>

      <div className="tabs-row">
        <button className={`tab-btn${tab === 'heroes' ? ' active' : ''}`} onClick={() => setTab('heroes')}>Héros</button>
        <button className={`tab-btn${tab === 'cards' ? ' active' : ''}`} onClick={() => setTab('cards')}>Cartes</button>
        <button className={`tab-btn${tab === 'enemies' ? ' active' : ''}`} onClick={() => setTab('enemies')}>Ennemis</button>
      </div>

      <div className="choice-list">
        {tab === 'heroes' && HEROES.map((h) => (
          <div key={h.id} className="codex-entry">
            <div className="codex-entry-head">
              <span className="codex-name" style={{ color: h.colorAccent }}>{h.portraitGlyph} {h.name}</span>
              <span className="screen-subtitle" style={{ margin: 0 }}>{h.baseHp} PV</span>
            </div>
            <p className="codex-desc">{h.archetype}</p>
            <p className="codex-desc" style={{ fontStyle: 'italic' }}>{h.quote}</p>
          </div>
        ))}
        {tab === 'cards' && CARDS.map((c) => (
          <div key={c.id} className="codex-entry">
            <div className="codex-entry-head">
              <span className="codex-name">{c.name}</span>
              <span className="screen-subtitle" style={{ margin: 0 }}>Coût {c.cost}</span>
            </div>
            <p className="codex-desc">{c.description}</p>
          </div>
        ))}
        {tab === 'enemies' && ENEMIES.map((e) => (
          <div key={e.id} className="codex-entry">
            <div className="codex-entry-head">
              <span className="codex-name">{e.portraitGlyph} {e.name}{e.isBoss ? ' · Boss' : ''}</span>
              <span className="screen-subtitle" style={{ margin: 0 }}>{e.hp} PV</span>
            </div>
            <p className="codex-desc">{e.description}</p>
          </div>
        ))}
      </div>

      <Button block onClick={() => goTo('mainMenu')}>Retour</Button>
    </div>
  );
}
