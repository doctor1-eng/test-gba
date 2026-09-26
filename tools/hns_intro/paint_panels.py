#!/usr/bin/env python3
"""Heart & Soul - opening cinematic panels ("La chute de Cinnabar").

Paints the 5 full-screen pixel-art panels procedurally (every pixel is placed by code, with a
fixed seed, so the art is reproducible and reviewable), then converts each one to GBA data:
  graphics/hns_intro/panelN.png        256x160 preview / source of truth
  graphics/hns_intro/panelN_tiles.bin  8bpp tiles, deduplicated
  graphics/hns_intro/panelN_map.bin    32x32 u16 tilemap (only the first 20 rows used)
  graphics/hns_intro/panelN_pal.bin    BGR555 palette, at most MAX_COLORS entries
Palette slots 224..255 stay free for the caption window (src/hns_intro_cinematic.c).
Run from the repo root: python3 tools/hns_intro/paint_panels.py
"""
import math, os, random, struct
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'graphics/hns_intro')
W, H = 256, 160
MAX_COLORS = 224
MAX_TILES = 640

# Shared night palette (hand-picked; the GBA shows 5 bits per channel, so values are
# chosen on that grid to avoid banding surprises after conversion).
C = {
    'void': (8, 8, 16), 'sky0': (16, 16, 40), 'sky1': (24, 24, 64), 'sky2': (40, 32, 88),
    'sky3': (64, 48, 112), 'haze': (96, 64, 120), 'star': (248, 248, 232), 'star2': (168, 176, 216),
    'moon': (240, 232, 200), 'moon2': (208, 200, 168), 'moon3': (176, 168, 144),
    'sea0': (8, 16, 40), 'sea1': (16, 32, 64), 'sea2': (32, 56, 96), 'glint': (200, 208, 224),
    'rock0': (16, 12, 24), 'rock1': (32, 24, 40), 'rock2': (56, 40, 56),
    'lava0': (120, 24, 16), 'lava1': (216, 72, 24), 'lava2': (248, 168, 56),
    'win': (248, 216, 112), 'win2': (200, 144, 64), 'pcroof': (200, 48, 48),
    'smoke0': (48, 40, 56), 'smoke1': (72, 64, 80), 'red': (224, 40, 40), 'red2': (160, 16, 24),
    'beam': (120, 128, 152), 'fire0': (248, 232, 144), 'fire1': (248, 152, 40), 'fire2': (200, 64, 24),
    'orange0': (136, 48, 40), 'orange1': (184, 80, 48), 'bolt': (248, 248, 255), 'bolt2': (176, 200, 248),
    'coat': (24, 24, 40), 'coat2': (40, 40, 64), 'gold': (240, 192, 64), 'gold2': (184, 128, 32),
    'badge0': (200, 56, 40), 'badge1': (248, 120, 64), 'badge2': (120, 24, 24),
}


class Canvas:
    def __init__(self):
        self.im = Image.new('RGB', (W, H), C['void'])
        self.px = self.im.load()

    def put(self, x, y, col):
        if 0 <= x < W and 0 <= y < H:
            self.px[x, y] = C[col] if isinstance(col, str) else col

    def rect(self, x0, y0, x1, y1, col):
        for y in range(max(0, y0), min(H, y1 + 1)):
            for x in range(max(0, x0), min(W, x1 + 1)):
                self.px[x, y] = C[col]

    def disc(self, cx, cy, r, col):
        for y in range(int(cy - r) - 1, int(cy + r) + 2):
            for x in range(int(cx - r) - 1, int(cx + r) + 2):
                if (x - cx) ** 2 + (y - cy) ** 2 <= r * r:
                    self.put(x, y, col)

    def poly(self, pts, col):
        ys = [p[1] for p in pts]
        for y in range(max(0, min(ys)), min(H, max(ys) + 1)):
            xs = []
            n = len(pts)
            for i in range(n):
                (x1, y1), (x2, y2) = pts[i], pts[(i + 1) % n]
                if (y1 <= y < y2) or (y2 <= y < y1):
                    xs.append(x1 + (y - y1) * (x2 - x1) / (y2 - y1))
            xs.sort()
            for i in range(0, len(xs) - 1, 2):
                for x in range(int(math.ceil(xs[i])), int(xs[i + 1]) + 1):
                    self.put(x, y, col)


def dither_band(c, y0, y1, top, bottom):
    """Vertical gradient between two palette colours with an ordered (Bayer 2x2) dither."""
    bayer = [[0, 2], [3, 1]]
    for y in range(y0, y1):
        t = (y - y0) / max(1, (y1 - y0 - 1))
        for x in range(W):
            c.put(x, y, bottom if t * 4 > bayer[y % 2][x % 2] + 0.5 else top)


def sky(c, rnd, horizon=100):
    bands = ['sky0', 'sky1', 'sky2', 'sky3', 'haze']
    step = horizon // (len(bands) - 1)
    for i in range(len(bands) - 1):
        dither_band(c, i * step, (i + 1) * step, bands[i], bands[i + 1])
    c.rect(0, (len(bands) - 1) * step, W - 1, horizon, 'haze')
    for _ in range(90):
        x, y = rnd.randrange(W), rnd.randrange(horizon - 25)
        c.put(x, y, 'star2' if rnd.random() < 0.7 else 'star')
    for _ in range(6):  # a few twinkling crosses
        x, y = rnd.randrange(8, W - 8), rnd.randrange(4, horizon - 40)
        c.put(x, y, 'star')
        for d in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            c.put(x + d[0], y + d[1], 'star2')


def moon(c, cx, cy, r):
    c.disc(cx, cy, r + 2, 'haze')
    c.disc(cx, cy, r, 'moon')
    for (dx, dy, rr) in ((-r // 3, -r // 4, r // 4), (r // 3, r // 5, r // 5), (-r // 6, r // 2, r // 6)):
        c.disc(cx + dx, cy + dy, rr, 'moon2')
    for y in range(cy - r, cy + r + 1):  # terminator shading on the right edge
        for x in range(cx + r // 3, cx + r + 1):
            if (x - cx) ** 2 + (y - cy) ** 2 <= r * r and (x + y) % 2 == 0:
                c.put(x, y, 'moon3')


def sea(c, rnd, horizon, glint_x=None):
    for y in range(horizon, H):
        t = (y - horizon) / (H - horizon)
        for x in range(W):
            c.put(x, y, 'sea1' if t < 0.35 else 'sea0')
    for y in range(horizon + 2, H, 3):  # wave strokes
        for _ in range(10 + (y - horizon) // 4):
            x = rnd.randrange(W)
            ln = rnd.randrange(3, 9 + (y - horizon) // 6)
            for i in range(ln):
                c.put(x + i, y, 'sea2')
    if glint_x is not None:  # moon reflection column
        for y in range(horizon + 1, H):
            wob = int(3 * math.sin(y * 0.9))
            half = 2 + (y - horizon) // 10
            for x in range(glint_x - half + wob, glint_x + half + wob):
                if (x + y) % 3 != 0:
                    c.put(x, y, 'glint')


def island(c, rnd, x0, x1, base, peak_x, peak_h, lights=True, glow=True):
    """Volcanic island silhouette: a cone with a crater, a shoreline shelf and a town."""
    pts = [(x0, base), (peak_x - 14, base - peak_h + 6), (peak_x - 6, base - peak_h),
           (peak_x + 6, base - peak_h), (peak_x + 16, base - peak_h + 8), (x1, base)]
    c.poly(pts, 'rock1')
    c.poly([(x0 + 6, base), (peak_x - 10, base - peak_h + 10), (peak_x - 4, base - peak_h + 2),
            (peak_x - 1, base)], 'rock2')  # lit left flank
    c.rect(x0 - 10, base - 4, x1 + 12, base, 'rock0')  # shore shelf
    if glow:
        for i in range(-5, 6):
            c.put(peak_x + i, base - peak_h, 'lava1' if abs(i) < 4 else 'lava0')
        c.put(peak_x, base - peak_h + 1, 'lava2')
        for k in range(26):  # smoke plume drifting right
            sx = peak_x + int(k * 1.6 + 3 * math.sin(k * 0.5))
            sy = base - peak_h - 3 - k
            c.disc(sx, sy, 2 + k // 7, 'smoke0' if k % 3 else 'smoke1')
    if lights:
        for i in range(14):
            x = rnd.randrange(x0 + 2, x1 - 2)
            c.rect(x, base - 3, x + 1, base - 2, 'win' if i % 3 else 'win2')
        c.rect(x1 - 26, base - 7, x1 - 18, base - 4, 'pcroof')  # Pokemon Center roof
        c.rect(x1 - 25, base - 3, x1 - 19, base - 1, 'win')


def ship(c, x, y, s, flag=True):
    hull = [(x - 18 * s, y), (x + 20 * s, y), (x + 15 * s, y + 6 * s), (x - 14 * s, y + 6 * s)]
    c.poly(hull, 'rock0')
    c.rect(x - 8 * s, y - 6 * s, x + 6 * s, y - 1, 'rock1')
    c.rect(x - 1, y - 20 * s, x, y - 6 * s, 'rock0')  # mast
    for wx in range(x - 6 * s, x + 5 * s, 3):
        c.put(wx, y - 4 * s, 'win2')
    if flag:  # the red R flag, lit
        fx, fy = x + 1, y - 20 * s
        c.rect(fx, fy, fx + 9 * s, fy + 6 * s, 'red2')
        R = ['###.', '#..#', '###.', '#.#.', '#..#']
        for j, row in enumerate(R):
            for i, ch in enumerate(row):
                if ch == '#':
                    c.rect(fx + 2 * s + i * s, fy + 1 + j * s // 1, fx + 2 * s + i * s + s - 1, fy + 1 + j * s + s - 1, 'red')


def beam(c, x, y, ang, length, spread):
    for t in range(length):
        half = int(t * spread)
        cx = x + int(t * math.cos(ang))
        cy = y - int(t * math.sin(ang))
        for d in range(-half, half + 1):
            px, py = cx, cy + d
            if 0 <= px < W and 0 <= py < H and (px + py + t) % 2 == 0:
                cur = c.px[px, py]
                if cur in (C['sky2'], C['sky3'], C['haze'], C['sky1'], C['sea1'], C['sea2']):
                    c.put(px, py, 'beam')


def panel1():
    rnd = random.Random(1)
    c = Canvas()
    sky(c, rnd, 104)
    moon(c, 200, 34, 16)
    sea(c, rnd, 104, glint_x=200)
    island(c, rnd, 20, 150, 106, 70, 46)
    return c


def panel2():
    rnd = random.Random(2)
    c = Canvas()
    sky(c, rnd, 96)
    moon(c, 40, 30, 11)
    sea(c, rnd, 96, glint_x=40)
    island(c, rnd, 196, 262, 97, 232, 22, lights=True, glow=True)
    ship(c, 60, 112, 2)
    ship(c, 150, 104, 1)
    ship(c, 118, 116, 2)
    beam(c, 70, 90, math.radians(28), 120, 0.12)
    beam(c, 128, 102, math.radians(12), 90, 0.10)
    return c


def panel3():
    """Blue on the prow, backlit by a huge moon; Cinnabar small and lit below."""
    rnd = random.Random(3)
    c = Canvas()
    sky(c, rnd, 118)
    moon(c, 150, 62, 44)
    sea(c, rnd, 118, glint_x=150)
    island(c, rnd, 20, 80, 120, 44, 16)
    # prow of the ship, bottom-right
    c.poly([(90, 150), (256, 124), (256, 160), (90, 160)], 'rock0')
    c.poly([(118, 150), (256, 129), (256, 133), (130, 150)], 'rock1')
    # Blue: silhouette standing on the prow, coat blowing left, spiky hair (drawn at 2x scale)
    bx, by, k = 200, 112, 2

    def P(pts):
        return [(bx + x * k, by + y * k) for (x, y) in pts]
    c.poly(P([(-5, 0), (5, 0), (4, -14), (-4, -14)]), 'coat')             # coat body
    c.poly(P([(-4, -13), (-15, -3), (-12, 0), (-3, -6)]), 'coat')          # coat tail in the wind
    c.poly(P([(-4, 0), (-1, 0), (-1, 8), (-4, 8)]), 'coat')               # legs
    c.poly(P([(1, 0), (4, 0), (5, 8), (2, 8)]), 'coat')
    c.poly(P([(-3, -14), (3, -14), (2, -17), (-2, -17)]), 'coat')         # neck/collar
    c.disc(bx, by - 20 * k, 4 * k, 'coat')                                 # head
    for (dx, dy) in ((-5, -23), (-3, -26), (0, -27), (3, -26), (5, -23), (-6, -20)):  # spikes
        c.poly(P([(dx, dy), (dx + 3, dy + 4), (dx - 1, dy + 4)]), 'coat')
    c.poly(P([(3, -13), (13, -16), (13, -14), (3, -11)]), 'coat')          # arm pointing at the island
    # rim light where the moon hits the silhouette's left edges
    for y in range(by - 60, by + 18):
        for x in range(bx - 34, bx + 30):
            if 0 < x < W and 0 <= y < H and c.px[x, y] == C['coat'] and c.px[x - 1, y] not in (C['coat'], C['coat2']):
                c.put(x, y, 'coat2')
                c.put(x + 1, y, 'coat2')
    return c


def panel4():
    """The port falls: burning town, Pokemon Center, and the lightning that takes Blaine."""
    rnd = random.Random(4)
    c = Canvas()
    for y in range(0, 110):  # sky lit from below by fire
        for x in range(W):
            t = y / 110
            col = 'sky1' if t < 0.3 else ('orange0' if t < 0.62 else 'orange1')
            if 0.25 < t < 0.35 and (x + y) % 2:
                col = 'orange0'
            if 0.57 < t < 0.67 and (x + y) % 2:
                col = 'orange1' if t > 0.62 else 'orange0'
            c.put(x, y, col)
    for k, sx in enumerate((22, 70, 150, 212, 244)):  # a few smoke columns, drifting left
        top = rnd.randrange(8, 30)
        for j in range((96 - top) // 2):
            r = 3 + j // 5
            c.disc(sx - j + int(3 * math.sin(j * 0.25 + k)), 96 - j * 2, r, 'smoke1' if (j // 3 + k) % 4 else 'smoke0')
    # houses
    x = 0
    while x < W:
        w = rnd.randrange(18, 32)
        h = rnd.randrange(16, 30)
        base = 132
        c.rect(x, base - h, x + w, base, 'rock0')
        c.poly([(x - 2, base - h), (x + w // 2, base - h - 10), (x + w + 2, base - h)], 'rock1')
        for wx in range(x + 3, x + w - 3, 6):
            if rnd.random() < 0.6:
                c.rect(wx, base - h + 6, wx + 2, base - h + 9, 'fire1' if rnd.random() < 0.5 else 'win')
        x += w + rnd.randrange(2, 8)
    # the Pokemon Center, centre stage
    c.rect(96, 98, 160, 132, 'rock1')
    c.poly([(92, 98), (128, 84), (164, 98)], 'pcroof')
    c.rect(118, 104, 138, 116, 'win')
    c.disc(128, 92, 5, 'star')
    c.rect(123, 91, 133, 92, 'red')
    # flames licking the rooftops
    for fx in range(0, W, 7):
        fh = rnd.randrange(4, 16)
        for j in range(fh):
            col = 'fire0' if j > fh - 3 else ('fire1' if j > fh // 3 else 'fire2')
            c.put(fx + int(math.sin(j) * 2), 128 - j - rnd.randrange(0, 20), col)
    c.rect(0, 132, W - 1, H - 1, 'rock0')
    # the lightning bolt
    bx, by = 178, 0
    pts = []
    while by < 96:
        pts.append((bx, by))
        bx += rnd.randrange(-9, 10)
        by += rnd.randrange(8, 14)
    for (x1, y1), (x2, y2) in zip(pts, pts[1:]):
        steps = max(abs(x2 - x1), abs(y2 - y1))
        for i in range(steps + 1):
            x = x1 + (x2 - x1) * i // max(1, steps)
            y = y1 + (y2 - y1) * i // max(1, steps)
            c.put(x - 1, y, 'bolt2')
            c.put(x, y, 'bolt')
            c.put(x + 1, y, 'bolt2')
    return c


def panel5():
    """Title card: the Volcano Badge of Cinnabar, cracked."""
    rnd = random.Random(5)
    c = Canvas()
    for _ in range(60):
        c.put(rnd.randrange(W), rnd.randrange(110), 'star2')
    cx, cy = 128, 54
    # flame-shaped badge (Volcano Badge): stacked rounded lobes
    c.disc(cx, cy + 6, 30, 'badge2')
    c.poly([(cx - 30, cy + 6), (cx - 12, cy - 38), (cx - 2, cy - 14), (cx + 6, cy - 44), (cx + 18, cy - 12),
            (cx + 30, cy + 6)], 'badge2')
    c.disc(cx, cy + 6, 26, 'badge0')
    c.poly([(cx - 26, cy + 6), (cx - 10, cy - 32), (cx - 1, cy - 10), (cx + 6, cy - 38), (cx + 16, cy - 10),
            (cx + 26, cy + 6)], 'badge0')
    c.disc(cx - 4, cy + 10, 14, 'badge1')
    c.poly([(cx - 16, cy + 10), (cx - 6, cy - 18), (cx + 2, cy - 2), (cx + 8, cy - 22), (cx + 14, cy + 10)], 'badge1')
    c.disc(cx - 2, cy + 14, 7, 'lava2')
    # the crack, left to right, with a dark gap
    x, y = cx - 30, cy - 2
    while x < cx + 30:
        c.put(x, y, 'void')
        c.put(x, y + 1, 'rock0')
        x += 1
        y += rnd.choice((-1, 0, 0, 1))
    # gold rim highlights
    for a in range(0, 360, 6):
        r = 29
        c.put(int(cx + r * math.cos(math.radians(a))), int(cy + 6 + r * math.sin(math.radians(a))),
              'gold' if a < 180 else 'gold2')
    return c


def to_gba(im, name):
    """Quantize to <= MAX_COLORS, dedupe 8x8 tiles, write tiles/map/palette binaries."""
    q = im.quantize(colors=MAX_COLORS, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    pal = q.getpalette()[:MAX_COLORS * 3]
    used = sorted(set(q.get_flattened_data()))
    # palette index 0 must be the darkest colour: it is also the GBA backdrop
    darkest = min(used, key=lambda i: sum(pal[i * 3:i * 3 + 3]))
    order = [darkest] + [i for i in used if i != darkest]
    remap = {old: new for new, old in enumerate(order)}
    data = [remap[p] for p in q.get_flattened_data()]
    newpal = []
    for old in order:
        r, g, b = pal[old * 3:old * 3 + 3]
        newpal.append((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))
    tiles, index, tmap = [], {}, []
    for ty in range(H // 8):
        row = []
        for tx in range(W // 8):
            t = bytes(data[(ty * 8 + y) * W + tx * 8 + x] for y in range(8) for x in range(8))
            if t not in index:
                index[t] = len(tiles)
                tiles.append(t)
            row.append(index[t])
        tmap.append(row)
    assert len(tiles) <= MAX_TILES, (name, len(tiles))
    os.makedirs(OUT, exist_ok=True)
    open(os.path.join(OUT, name + '_tiles.bin'), 'wb').write(b''.join(tiles))
    full = []
    for ty in range(32):
        for tx in range(32):
            full.append(tmap[ty][tx] if ty < len(tmap) else 0)
    open(os.path.join(OUT, name + '_map.bin'), 'wb').write(struct.pack('<1024H', *full))
    open(os.path.join(OUT, name + '_pal.bin'), 'wb').write(struct.pack('<%dH' % len(newpal), *newpal))
    im.save(os.path.join(OUT, name + '.png'))
    return len(tiles), len(newpal)


if __name__ == '__main__':
    for i, f in enumerate((panel1, panel2, panel3, panel4, panel5), 1):
        nt, nc = to_gba(f().im, 'panel%d' % i)
        print('panel%d: %d tiles, %d colours' % (i, nt, nc))
