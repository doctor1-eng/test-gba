import sharp from 'sharp';
import { mkdirSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import { dirname, join } from 'node:path';

const __dirname = dirname(fileURLToPath(import.meta.url));
const outDir = join(__dirname, '..', 'public', 'icons');
mkdirSync(outDir, { recursive: true });

function iconSvg(size, maskablePadding = 0) {
  const pad = maskablePadding;
  const inner = size - pad * 2;
  return `
<svg xmlns="http://www.w3.org/2000/svg" width="${size}" height="${size}" viewBox="0 0 ${size} ${size}">
  <defs>
    <radialGradient id="bg" cx="50%" cy="38%" r="75%">
      <stop offset="0%" stop-color="#2a1712"/>
      <stop offset="60%" stop-color="#160d0a"/>
      <stop offset="100%" stop-color="#0b0807"/>
    </radialGradient>
    <linearGradient id="gold" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" stop-color="#f0d38a"/>
      <stop offset="100%" stop-color="#a3792f"/>
    </linearGradient>
  </defs>
  <rect width="${size}" height="${size}" fill="url(#bg)"/>
  <rect x="${pad}" y="${pad}" width="${inner}" height="${inner}" fill="none" stroke="#5c3f2a" stroke-width="${Math.max(1, size * 0.01)}"/>
  <g transform="translate(${size / 2}, ${size / 2})">
    <circle r="${inner * 0.34}" fill="none" stroke="#7a1620" stroke-width="${Math.max(2, size * 0.012)}"/>
    <path d="M0 ${-inner * 0.30} L0 ${inner * 0.30} M${-inner * 0.16} ${-inner * 0.10} L${inner * 0.16} ${-inner * 0.10}"
          stroke="url(#gold)" stroke-width="${Math.max(4, size * 0.045)}" stroke-linecap="round"/>
    <circle cy="${-inner * 0.30}" r="${size * 0.02}" fill="url(#gold)"/>
  </g>
</svg>`;
}

const targets = [
  { name: 'icon-16.png', size: 16 },
  { name: 'icon-32.png', size: 32 },
  { name: 'icon-180.png', size: 180 },
  { name: 'icon-192.png', size: 192 },
  { name: 'icon-512.png', size: 512 },
  { name: 'icon-512-maskable.png', size: 512, maskable: true },
];

for (const t of targets) {
  const svg = iconSvg(t.size, t.maskable ? Math.round(t.size * 0.08) : 0);
  await sharp(Buffer.from(svg)).png().toFile(join(outDir, t.name));
  console.log('generated', t.name);
}
