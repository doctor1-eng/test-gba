import { UNIT_ICONS } from './icons';
import './ui.css';

export function Portrait({ iconKey, accent, size = 56, glyphFallback, dead }: { iconKey: string; accent: string; size?: number; glyphFallback?: string; dead?: boolean }) {
  const Icon = UNIT_ICONS[iconKey];
  return (
    <div
      className={`portrait-frame${dead ? ' portrait-dead' : ''}`}
      style={{ width: size, height: size, color: accent, borderColor: accent }}
    >
      {Icon ? <Icon className="portrait-icon" /> : <span className="portrait-fallback">{glyphFallback}</span>}
    </div>
  );
}
