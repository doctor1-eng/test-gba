import type { ReactNode } from 'react';
import './ui.css';

export function Modal({ onClose, children }: { onClose: () => void; children: ReactNode }) {
  return (
    <div className="modal-backdrop" onClick={onClose}>
      <div className="modal-panel vignette-frame" onClick={(e) => e.stopPropagation()}>
        {children}
      </div>
    </div>
  );
}
