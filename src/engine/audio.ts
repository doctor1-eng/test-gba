// Moteur audio synthétisé (Web Audio API) : aucune dépendance à des fichiers audio externes,
// donc rien à précacher et zéro risque hors-ligne. Doit être démarré depuis un geste utilisateur (iOS).

let ctx: AudioContext | null = null;
let ambianceNodes: { osc1: OscillatorNode; osc2: OscillatorNode; noise: AudioBufferSourceNode; gain: GainNode } | null = null;
let masterGain: GainNode | null = null;

function ensureContext(): AudioContext {
  if (!ctx) {
    ctx = new (window.AudioContext || (window as unknown as { webkitAudioContext: typeof AudioContext }).webkitAudioContext)();
    masterGain = ctx.createGain();
    masterGain.gain.value = 0.5;
    masterGain.connect(ctx.destination);
  }
  if (ctx.state === 'suspended') ctx.resume();
  return ctx;
}

function noiseBuffer(context: AudioContext, seconds: number): AudioBuffer {
  const buffer = context.createBuffer(1, context.sampleRate * seconds, context.sampleRate);
  const data = buffer.getChannelData(0);
  for (let i = 0; i < data.length; i++) data[i] = Math.random() * 2 - 1;
  return buffer;
}

export function initAudio(): void {
  ensureContext();
}

export function startAmbiance(): void {
  const context = ensureContext();
  if (ambianceNodes) return;

  const gain = context.createGain();
  gain.gain.value = 0;
  gain.connect(masterGain!);
  gain.gain.linearRampToValueAtTime(0.06, context.currentTime + 2);

  const osc1 = context.createOscillator();
  osc1.type = 'sine';
  osc1.frequency.value = 55;
  const osc2 = context.createOscillator();
  osc2.type = 'sine';
  osc2.frequency.value = 82.4;
  const oscGain = context.createGain();
  oscGain.gain.value = 0.7;
  osc1.connect(oscGain);
  osc2.connect(oscGain);
  oscGain.connect(gain);

  const noiseSrc = context.createBufferSource();
  noiseSrc.buffer = noiseBuffer(context, 4);
  noiseSrc.loop = true;
  const noiseFilter = context.createBiquadFilter();
  noiseFilter.type = 'lowpass';
  noiseFilter.frequency.value = 300;
  const noiseGain = context.createGain();
  noiseGain.gain.value = 0.25;
  noiseSrc.connect(noiseFilter);
  noiseFilter.connect(noiseGain);
  noiseGain.connect(gain);

  osc1.start();
  osc2.start();
  noiseSrc.start();

  ambianceNodes = { osc1, osc2, noise: noiseSrc, gain };
}

export function stopAmbiance(): void {
  if (!ambianceNodes || !ctx) return;
  const { osc1, osc2, noise, gain } = ambianceNodes;
  const context = ctx;
  gain.gain.linearRampToValueAtTime(0, context.currentTime + 0.6);
  setTimeout(() => {
    osc1.stop(); osc2.stop(); noise.stop();
  }, 700);
  ambianceNodes = null;
}

type SfxName = 'cardPlay' | 'hit' | 'crit' | 'death' | 'victory' | 'stress';

export function playSfx(name: SfxName): void {
  const context = ensureContext();
  const now = context.currentTime;
  const gain = context.createGain();
  gain.connect(masterGain!);

  switch (name) {
    case 'cardPlay': {
      const osc = context.createOscillator();
      osc.type = 'triangle';
      osc.frequency.setValueAtTime(340, now);
      osc.frequency.exponentialRampToValueAtTime(180, now + 0.12);
      gain.gain.setValueAtTime(0.18, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + 0.15);
      osc.connect(gain);
      osc.start(now); osc.stop(now + 0.16);
      break;
    }
    case 'hit': {
      const osc = context.createOscillator();
      osc.type = 'square';
      osc.frequency.setValueAtTime(110, now);
      osc.frequency.exponentialRampToValueAtTime(50, now + 0.18);
      gain.gain.setValueAtTime(0.22, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + 0.2);
      osc.connect(gain);
      osc.start(now); osc.stop(now + 0.2);
      break;
    }
    case 'crit': {
      const osc = context.createOscillator();
      osc.type = 'sawtooth';
      osc.frequency.setValueAtTime(220, now);
      osc.frequency.exponentialRampToValueAtTime(40, now + 0.3);
      gain.gain.setValueAtTime(0.28, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + 0.32);
      osc.connect(gain);
      osc.start(now); osc.stop(now + 0.32);
      break;
    }
    case 'death': {
      const osc = context.createOscillator();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(220, now);
      osc.frequency.exponentialRampToValueAtTime(55, now + 1.4);
      gain.gain.setValueAtTime(0.2, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + 1.5);
      osc.connect(gain);
      osc.start(now); osc.stop(now + 1.5);
      break;
    }
    case 'victory': {
      [261.6, 329.6, 392.0].forEach((freq, i) => {
        const osc = context.createOscillator();
        osc.type = 'triangle';
        osc.frequency.value = freq;
        const g = context.createGain();
        g.gain.setValueAtTime(0.001, now + i * 0.12);
        g.gain.linearRampToValueAtTime(0.15, now + i * 0.12 + 0.05);
        g.gain.exponentialRampToValueAtTime(0.001, now + i * 0.12 + 0.9);
        osc.connect(g);
        g.connect(masterGain!);
        osc.start(now + i * 0.12);
        osc.stop(now + i * 0.12 + 1);
      });
      break;
    }
    case 'stress': {
      const osc = context.createOscillator();
      osc.type = 'sine';
      osc.frequency.setValueAtTime(80, now);
      const lfo = context.createOscillator();
      lfo.frequency.value = 6;
      const lfoGain = context.createGain();
      lfoGain.gain.value = 20;
      lfo.connect(lfoGain);
      lfoGain.connect(osc.frequency);
      gain.gain.setValueAtTime(0.15, now);
      gain.gain.exponentialRampToValueAtTime(0.001, now + 0.5);
      osc.connect(gain);
      lfo.start(now); osc.start(now);
      lfo.stop(now + 0.5); osc.stop(now + 0.5);
      break;
    }
  }
}
