import './ui.css';

export function HPBar({ current, max, deathsDoor }: { current: number; max: number; deathsDoor?: boolean }) {
  const pct = max > 0 ? Math.max(0, Math.min(100, (current / max) * 100)) : 0;
  const low = pct <= 25;
  return (
    <div>
      <div className="bar-label">
        <span>{deathsDoor ? 'Article de la mort' : 'PV'}</span>
        <span>{current}/{max}</span>
      </div>
      <div className="bar">
        <div className={`bar-fill hp${low ? ' low' : ''}`} style={{ width: `${pct}%` }} />
      </div>
    </div>
  );
}

export function StressBar({ value, threshold = 100 }: { value: number; threshold?: number }) {
  const pct = Math.max(0, Math.min(100, (value / threshold) * 100));
  return (
    <div>
      <div className="bar-label">
        <span>Stress</span>
        <span>{value}/{threshold}</span>
      </div>
      <div className="bar">
        <div className="bar-fill stress" style={{ width: `${pct}%` }} />
      </div>
    </div>
  );
}

export function EnergyPips({ current, max }: { current: number; max: number }) {
  return (
    <div className="pip-row">
      {Array.from({ length: max }).map((_, i) => (
        <div key={i} className={`pip${i < current ? ' filled' : ''}`} />
      ))}
    </div>
  );
}
