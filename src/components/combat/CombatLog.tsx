import { useEffect, useRef } from 'react';
import type { CombatLogEntry } from '../../engine/types';
import './combat.css';

export function CombatLog({ entries }: { entries: CombatLogEntry[] }) {
  const ref = useRef<HTMLDivElement>(null);
  useEffect(() => {
    if (ref.current) ref.current.scrollTop = ref.current.scrollHeight;
  }, [entries.length]);
  return (
    <div className="combat-log" ref={ref}>
      {entries.map((e) => <p key={e.id}>{e.text}</p>)}
    </div>
  );
}
