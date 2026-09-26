#!/usr/bin/env python3
"""Heart & Soul - Act I "La fuite" cinematic panels (after the battle with the 2nd Grunt, up to
the arrival in Pallet Town). Same pipeline and style as paint_panels.py (side views, night and
fire palettes), plus real game sprites composited in (Growlithe, Chansey, the Sailor, the
Rocket Grunt) so the characters are the game's own pixel art.

Branch panels (one per player choice) are named fNN<variant>. Run from the repo root:
    python3 tools/hns_intro/paint_fuite.py
"""
import math, os, random, sys
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from paint_panels import Canvas, C, W, H, ROOT, sky, moon, sea, island, dither_band, to_gba  # noqa: E402

C.update({
    'plank0': (56, 36, 32), 'plank1': (88, 56, 40), 'plank2': (120, 80, 56),
    'wall0': (40, 32, 48), 'wall1': (64, 52, 72), 'gymw': (176, 136, 88), 'gymr': (120, 64, 40),
    'light0': (248, 240, 200), 'light1': (232, 200, 120), 'eye': (248, 232, 120),
    'silver': (208, 216, 232), 'silver2': (136, 144, 168), 'paper': (232, 224, 200), 'ink': (72, 64, 88),
    'storm0': (16, 20, 32), 'storm1': (32, 40, 56), 'storm2': (56, 68, 88), 'foam': (192, 208, 224),
    'rain': (120, 136, 168), 'dawn0': (72, 56, 104), 'dawn1': (168, 96, 120), 'dawn2': (240, 152, 120),
    'dawn3': (248, 208, 152), 'sand0': (152, 120, 96), 'sand1': (200, 168, 128), 'roofp': (184, 72, 64),
    'house': (96, 80, 96), 'house2': (136, 112, 128), 'bandage': (240, 240, 240), 'skin': (40, 32, 48),
    'green': (88, 168, 88), 'cross': (232, 72, 80),
})


def sprite(path, frame=0, size=64):
    """Load a game sprite sheet frame as RGBA with palette index 0 transparent."""
    im = Image.open(os.path.join(ROOT, path))
    im = im.crop((0, frame * size, size, frame * size + size)) if im.height > size else im
    idx = im.load()
    rgba = im.convert('RGBA')
    px = rgba.load()
    for y in range(rgba.height):
        for x in range(rgba.width):
            if idx[x, y] == 0:
                px[x, y] = (0, 0, 0, 0)
    return rgba


def paste(c, spr, x, y, grade=None, silhouette=None, flip=False, scale=1):
    if flip:
        spr = spr.transpose(Image.FLIP_LEFT_RIGHT)
    if scale != 1:
        spr = spr.resize((int(spr.width * scale), int(spr.height * scale)), Image.NEAREST)
    p = spr.load()
    for j in range(spr.height):
        for i in range(spr.width):
            r, g, b, a = p[i, j]
            if a == 0:
                continue
            if silhouette:
                col = C[silhouette]
            elif grade:
                col = tuple(max(0, min(255, int(v * m + o))) for v, (m, o) in zip((r, g, b), grade))
            else:
                col = (r, g, b)
            c.put(x + i, y + j, col)


NIGHT = ((0.55, 8), (0.55, 10), (0.75, 24))
FIRE = ((0.95, 30), (0.65, 6), (0.45, 0))
STORM = ((0.45, 6), (0.55, 10), (0.75, 22))
DAWN = ((1.0, 16), (0.88, 8), (0.82, 8))


def fire_sky(c, rnd, horizon=110):
    for y in range(horizon):
        t = y / horizon
        for x in range(W):
            if t < 0.3:
                col = 'sky1'
            elif t < 0.6:
                col = 'orange0' if (t < 0.42 or (x + y) % 2) else 'orange1'
            else:
                col = 'orange1'
            if 0.26 < t < 0.32 and (x + y) % 2:
                col = 'orange0'
            c.put(x, y, col)
    for k, sx in enumerate(rnd.sample(range(10, W - 10, 20), 4)):
        for j in range(40):
            c.disc(sx - j + int(3 * math.sin(j * 0.25 + k)), horizon - 10 - j * 2, 3 + j // 6,
                   'smoke1' if (j // 3 + k) % 4 else 'smoke0')


def embers(c, rnd, n=40, ymax=H):
    for _ in range(n):
        c.put(rnd.randrange(W), rnd.randrange(ymax), 'fire0' if rnd.random() < 0.4 else 'fire1')


def flames(c, rnd, x0, x1, base, hmin=6, hmax=18):
    for fx in range(x0, x1, 3):
        fh = rnd.randrange(hmin, hmax)
        for j in range(fh):
            col = 'fire0' if j > fh - 4 else ('fire1' if j > fh // 3 else 'fire2')
            wob = int(math.sin(j * 0.6 + fx) * 2)
            c.rect(fx + wob, base - j, fx + wob + 1, base - j, col)


def pokecenter(c, x, base, burning=False, rnd=None, door_lit=True):
    c.rect(x, base - 34, x + 56, base, 'wall1')
    c.poly([(x - 5, base - 34), (x + 28, base - 48), (x + 61, base - 34)], 'pcroof')
    c.rect(x + 22, base - 46, x + 34, base - 40, 'star')          # Poke Ball sign
    c.rect(x + 22, base - 43, x + 34, base - 43, 'red')
    for wx in (x + 5, x + 40):
        c.rect(wx, base - 26, wx + 10, base - 18, 'light1' if door_lit else 'wall0')
    c.rect(x + 20, base - 20, x + 36, base, 'light0' if door_lit else 'wall0')  # door
    if burning and rnd:
        flames(c, rnd, x - 4, x + 62, base - 34, 4, 14)


def gym(c, x, base, rnd=None, burning=False):
    c.rect(x, base - 44, x + 64, base, 'gymr')
    c.rect(x - 4, base - 50, x + 68, base - 44, 'gymw')
    c.rect(x + 24, base - 20, x + 40, base, 'wall0')
    # Volcano emblem over the door
    c.poly([(x + 26, base - 24), (x + 32, base - 36), (x + 38, base - 24)], 'lava1')
    c.rect(x + 30, base - 36, x + 34, base - 34, 'lava2')
    for wx in (x + 6, x + 48):
        c.rect(wx, base - 36, wx + 8, base - 26, 'fire1')
    if burning and rnd:
        flames(c, rnd, x - 6, x + 70, base - 50, 6, 20)


def person(c, x, base, h=26, col='coat', child=False, facing=0):
    """Simple standing silhouette (back view when facing=0)."""
    s = h / 26
    head = max(2, int(4 * s))
    c.disc(x, base - h + head, head, col)
    c.poly([(x - int(5 * s), base - h + 2 * head), (x + int(5 * s), base - h + 2 * head),
            (x + int(6 * s), base - int(9 * s)), (x - int(6 * s), base - int(9 * s))], col)
    c.rect(x - int(4 * s), base - int(9 * s), x - 1, base, col)
    c.rect(x + 1, base - int(9 * s), x + int(4 * s), base, col)
    if child:
        c.rect(x + int(5 * s), base - int(16 * s), x + int(8 * s), base - int(13 * s), col)


def rim(c, x0, y0, x1, y1, body='coat', light='coat2'):
    """Light the left edge of every silhouette pixel in the box (fire/moon rim light)."""
    snap = c.im.copy().load()  # read from a snapshot so the light does not cascade rightwards
    for y in range(max(0, y0), min(H, y1)):
        for x in range(max(1, x0), min(W - 1, x1)):
            if snap[x, y] == C[body] and snap[x - 1, y] != C[body]:
                c.put(x, y, light)


def lantern(c, x, y, r=18):
    for yy in range(y - r, y + r + 1):
        for xx in range(x - r, x + r + 1):
            d = math.hypot(xx - x, yy - y)
            if d <= r and 0 <= xx < W and 0 <= yy < H:
                cur = c.px[xx, yy]
                if d < r * 0.45 or (d < r and (xx + yy) % 2 == 0):
                    lum = sum(cur) / 3
                    c.put(xx, yy, 'light1' if lum > 60 else 'win2')
    c.rect(x - 2, y - 3, x + 2, y + 3, 'light0')
    c.rect(x - 1, y - 6, x + 1, y - 4, 'rock0')


def barque(c, x, y, people=3, lantern_on=True):
    c.poly([(x - 20, y), (x + 20, y), (x + 14, y + 7), (x - 14, y + 7)], 'rock0')
    for i in range(people):
        person(c, x - 12 + i * 11, y, 12, 'rock0', child=(i == people - 1))
    if lantern_on:
        lantern(c, x + 16, y - 10, 10)


def planks(c, y0, rnd):
    for y in range(y0, H):
        for x in range(W):
            c.put(x, y, 'plank1' if ((y - y0) // 6) % 2 == 0 else 'plank0')
        if (y - y0) % 6 == 0:
            for x in range(W):
                c.put(x, y, 'plank0')
    for _ in range(18):
        x, y = rnd.randrange(W), rnd.randrange(y0, H)
        c.rect(x, y, x + 1, y, 'plank2')


def rain(c, rnd, n=260, col='rain'):
    for _ in range(n):
        x, y = rnd.randrange(W), rnd.randrange(H)
        for k in range(5):
            c.put(x - k, y + k * 2, col)


def storm_sea(c, rnd, horizon):
    for y in range(horizon, H):
        for x in range(W):
            c.put(x, y, 'storm1')
    for row in range(5):  # rows of rolling swells, nearer ones taller
        base = horizon + 12 + row * 18
        amp = 4 + row * 3
        phase = rnd.random() * 6
        for x in range(W):
            top = base - int(amp * (0.5 + 0.5 * math.sin(x * 0.06 + phase + row)))
            for y in range(top, min(H, base + 18)):
                c.put(x, y, 'storm2' if y < base + 6 else 'storm1')
            if math.sin(x * 0.06 + phase + row) > 0.8:
                c.put(x, top, 'foam')
                c.put(x, top + 1, 'foam')


def growlithe_sprite():
    return sprite('graphics/pokemon/growlithe/anim_front.png', 0)


# ---------------------------------------------------------------------------------------------
# Panels
# ---------------------------------------------------------------------------------------------
def f01_choix():
    rnd = random.Random(11)
    c = Canvas()
    fire_sky(c, rnd, 112)
    c.rect(0, 112, W - 1, H - 1, 'rock0')
    pokecenter(c, 18, 124, burning=True, rnd=rnd)
    gym(c, 170, 124, rnd=rnd, burning=True)
    c.rect(0, 124, W - 1, H - 1, 'rock1')
    person(c, 128, 152, 40, 'coat')
    rim(c, 100, 100, 160, 160, 'coat', 'fire1')
    for (tx, ty) in ((60, 136), (206, 136)):  # two paths
        c.rect(tx - 1, ty, tx + 1, ty + 1, 'fire0')
    embers(c, rnd, 50, 120)
    return c


def f02a_centre():
    rnd = random.Random(12)
    c = Canvas()
    fire_sky(c, rnd, 100)
    c.rect(0, 100, W - 1, H - 1, 'rock1')
    pokecenter(c, 30, 128, burning=False, door_lit=True)
    c.rect(0, 128, W - 1, H - 1, 'rock0')
    paste(c, sprite('graphics/pokemon/chansey/anim_front.png', 0), 42, 100, grade=FIRE, scale=0.6)
    # stretcher carried by two silhouettes toward the right
    person(c, 150, 150, 34, 'coat')
    person(c, 214, 150, 34, 'coat')
    rim(c, 130, 100, 240, 160, 'coat', 'fire1')
    c.rect(142, 126, 222, 129, 'paper')
    c.rect(166, 121, 196, 125, 'coat2')
    embers(c, rnd, 30, 110)
    return c


def f02b_arene():
    rnd = random.Random(13)
    c = Canvas()
    for y in range(H):
        for x in range(W):
            c.put(x, y, 'wall0' if y < 96 else 'rock1')
    for x in range(0, W, 32):  # pillars
        c.rect(x, 0, x + 5, 96, 'wall1')
    # trophy shelf with badges
    c.rect(56, 36, 200, 70, 'gymr')
    c.rect(60, 40, 196, 66, 'plank0')
    colors = ['badge0', 'lava1', 'gold', 'green', 'silver', 'cross', 'star2', 'lava2']
    for i, col in enumerate(colors):
        c.disc(72 + i * 16, 53, 5, col)
    c.disc(72 + 7 * 16, 53, 7, 'gold')  # the Volcano badge case, larger
    flames(c, rnd, 0, W, 112, 10, 40)
    flames(c, rnd, 0, 50, 96, 10, 30)
    person(c, 190, 150, 44, 'coat')
    c.rect(198, 108, 214, 112, 'coat')  # arm reaching to the case
    embers(c, rnd, 60)
    return c


def f03_carnet():
    rnd = random.Random(14)
    c = Canvas()
    fire_sky(c, rnd, 70)
    c.rect(0, 70, W - 1, H - 1, 'rock1')
    for _ in range(80):
        x, y = rnd.randrange(W), rnd.randrange(72, H)
        c.put(x, y, 'rock2')
    grunt = sprite('graphics/trainers/front_pics/rocket_grunt_m_hns.png')
    paste(c, grunt, 176, 20, silhouette='rock0', flip=True)
    c.rect(200, 34, 206, 40, 'red')  # the R on the chest, lit by fire
    # the notebook in the foreground: two pages, lines of code
    c.poly([(40, 150), (120, 136), (124, 104), (48, 116)], 'paper')
    c.poly([(124, 104), (200, 94), (206, 126), (120, 136)], 'paper')
    c.poly([(120, 136), (124, 104), (122, 104), (118, 136)], 'ink')
    for i in range(6):
        y = 120 + i * 4
        for x in range(56, 112, 3):
            if rnd.random() < 0.7:
                c.put(x, y - (x - 56) // 7, 'ink')
        y2 = 106 + i * 4
        for x in range(132, 196, 3):
            if rnd.random() < 0.7:
                c.put(x, y2 - (x - 132) // 8, 'ink')
    return c


def f04_refugies():
    rnd = random.Random(15)
    c = Canvas()
    sky(c, rnd, 60)
    c.poly([(0, 60), (70, 18), (150, 10), (256, 40), (256, 160), (0, 160)], 'rock1')
    for _ in range(120):
        x, y = rnd.randrange(W), rnd.randrange(30, H)
        if c.px[x, y] == C['rock1']:
            c.put(x, y, 'rock2')
    c.disc(128, 118, 46, 'rock0')              # cave mouth
    c.rect(82, 118, 174, 160, 'rock0')
    lantern(c, 104, 110, 22)
    person(c, 116, 150, 34, 'coat')            # adult
    person(c, 136, 150, 20, 'coat', child=True)
    rim(c, 90, 100, 150, 152, 'coat', 'win2')
    c.rect(0, 150, W - 1, H - 1, 'rock0')
    return c


def f05a_guides():
    rnd = random.Random(16)
    c = Canvas()
    sky(c, rnd, 92)
    sea(c, rnd, 92)
    island(c, rnd, -30, 90, 94, 20, 30, lights=False, glow=True)
    flames(c, rnd, 0, 90, 94, 3, 10)
    barque(c, 150, 112)
    for t in range(110):  # searchlight sweeping just behind them, on the water only
        half = t // 18
        cx, cy = 40 + t, 96 + t // 5
        for d in range(-half, half + 1):
            if (cx + cy + d) % 3 == 0:
                c.put(cx, cy + d, 'beam')
    c.rect(0, 146, 90, H - 1, 'plank0')        # the pier they left
    return c


def f05b_caches():
    rnd = random.Random(17)
    c = Canvas()
    for y in range(H):
        for x in range(W):
            c.put(x, y, 'rock0')
    for _ in range(300):
        x, y = rnd.randrange(W), rnd.randrange(H)
        c.put(x, y, 'rock1')
    c.poly([(0, 0), (256, 0), (256, 30), (180, 44), (90, 38), (0, 50)], 'void')
    for _ in range(500):                       # wall catching the lantern light
        a, d = rnd.random() * 6.283, rnd.random() ** 0.6 * 60
        c.put(int(190 + d * math.cos(a)), int(120 + d * 0.7 * math.sin(a)), 'rock2' if d < 40 else 'rock1')
    person(c, 76, 140, 30, 'rock1')            # the family, barely visible, pressed to the wall
    person(c, 108, 140, 18, 'rock1', child=True)
    lantern(c, 190, 120, 10)                   # the last lantern, turned low
    for (ex, ey) in ((74, 116), (78, 116), (106, 126), (110, 126)):  # eyes in the dark
        c.rect(ex, ey, ex + 1, ey, 'eye')
    return c


def f06_blesse():
    rnd = random.Random(18)
    c = Canvas()
    fire_sky(c, rnd, 80)
    sea(c, rnd, 80)
    planks(c, 116, rnd)
    paste(c, growlithe_sprite(), 96, 66, grade=((0.6, 10), (0.45, 6), (0.45, 6)))
    for (sx, sy) in ((116, 100), (140, 92), (126, 110)):  # scratches
        c.rect(sx, sy, sx + 4, sy, 'red2')
    embers(c, rnd, 30, 100)
    return c


def f07a_soigne():
    rnd = random.Random(19)
    c = Canvas()
    fire_sky(c, rnd, 80)
    sea(c, rnd, 80)
    planks(c, 116, rnd)
    paste(c, growlithe_sprite(), 112, 60, grade=FIRE)
    c.rect(128, 108, 144, 111, 'bandage')      # bandage on the foreleg
    c.poly([(0, 160), (30, 118), (60, 112), (84, 120), (60, 132), (40, 160)], 'skin')  # the player's hand
    for (hx, hy) in ((150, 58), (156, 52), (162, 58)):  # little heart
        c.rect(hx, hy, hx + 3, hy + 3, 'cross')
    c.rect(153, 61, 159, 64, 'cross')
    return c


def f07b_ignore():
    rnd = random.Random(20)
    c = Canvas()
    fire_sky(c, rnd, 80)
    sea(c, rnd, 80)
    planks(c, 110, rnd)
    paste(c, growlithe_sprite(), 184, 70, grade=((0.5, 6), (0.4, 6), (0.45, 10)), scale=0.5)
    person(c, 70, 160, 60, 'coat')             # the player walking away, close to camera
    rim(c, 30, 90, 110, 160, 'coat', 'fire1')
    return c


def f08_bracelet():
    rnd = random.Random(21)
    c = Canvas()
    planks(c, 0, rnd)
    for y in range(H):  # firelight from the left
        for x in range(0, 60):
            if (x + y) % 3 == 0:
                c.put(x, y, 'orange1')
    cx, cy = 128, 80
    for a in range(360):
        for r in (30, 31, 32, 33):
            x = int(cx + r * 1.4 * math.cos(math.radians(a)))
            y = int(cy + r * 0.8 * math.sin(math.radians(a)))
            c.put(x, y, 'silver' if a < 200 else 'silver2')
    for i in range(8):  # engraving
        c.rect(cx - 20 + i * 5, cy + 24, cx - 18 + i * 5, cy + 25, 'silver2')
    c.rect(cx + 38, cy - 22, cx + 40, cy - 20, 'star')  # glint
    c.put(cx + 39, cy - 25, 'star')
    c.put(cx + 39, cy - 17, 'star')
    return c


def f09_bateau(guides, growlithe):
    rnd = random.Random(22)
    c = Canvas()
    sky(c, rnd, 88)
    moon(c, 214, 26, 9)
    sea(c, rnd, 88)
    island(c, rnd, 10, 130, 90, 60, 36, lights=False, glow=True)
    flames(c, rnd, 10, 130, 90, 3, 12)
    if guides:
        barque(c, 196, 100, 3, True)           # the refugees' boat, ahead of us
    # stern railing in the foreground
    c.rect(0, 136, W - 1, H - 1, 'plank0')
    c.rect(0, 126, W - 1, 128, 'plank2')
    for x in range(4, W, 16):
        c.rect(x, 126, x + 2, 138, 'plank2')
    person(c, 150, 160, 44, 'coat')
    if growlithe:
        paste(c, growlithe_sprite(), 176, 100, grade=NIGHT, scale=0.75)
    return c


def f10_tempete():
    rnd = random.Random(23)
    c = Canvas()
    for y in range(H):
        for x in range(W):
            c.put(x, y, 'storm0' if y < 50 else 'storm1')
    storm_sea(c, rnd, 70)
    # our boat, tilted on a wave
    c.poly([(70, 110), (170, 94), (164, 110), (82, 122)], 'rock0')
    c.rect(118, 70, 120, 100, 'rock0')
    # lightning
    x, y = 200, 0
    while y < 80:
        nx, ny = x + rnd.randrange(-8, 9), y + rnd.randrange(6, 12)
        for i in range(12):
            c.put(x + (nx - x) * i // 12, y + (ny - y) * i // 12, 'bolt')
        x, y = nx, ny
    rain(c, rnd)
    return c


def f11_marin():
    rnd = random.Random(24)
    c = Canvas()
    for y in range(H):
        for x in range(W):
            c.put(x, y, 'storm0' if y < 80 else 'storm1')
    storm_sea(c, rnd, 96)
    # ship's wheel behind the sailor
    for a in range(0, 360, 45):
        for r in range(0, 34):
            c.put(int(96 + r * math.cos(math.radians(a))), int(96 + r * math.sin(math.radians(a))), 'plank1')
    for a in range(360):
        for r in (30, 31):
            c.put(int(96 + r * math.cos(math.radians(a))), int(96 + r * math.sin(math.radians(a))), 'plank2')
    paste(c, sprite('graphics/trainers/front_pics/sailor.png'), 120, 48, grade=STORM, scale=1.5)
    lantern(c, 40, 60, 14)
    rain(c, rnd, 320)
    return c


def f12_palette(growlithe):
    rnd = random.Random(25)
    c = Canvas()
    bands = ['dawn0', 'dawn1', 'dawn2', 'dawn3']
    for i in range(3):
        dither_band(c, i * 26, (i + 1) * 26, bands[i], bands[i + 1])
    c.rect(0, 78, W - 1, 92, 'dawn3')
    c.disc(200, 92, 16, 'light0')              # the rising sun on the horizon
    for y in range(92, 118):  # calm sea
        for x in range(W):
            c.put(x, y, 'sea2' if (y % 4) else 'dawn2')
    # Pallet Town on the shore: two houses and the lab, backlit
    for (hx, hw, hh) in ((20, 40, 22), (70, 40, 22), (150, 70, 26)):
        c.rect(hx, 92 - hh, hx + hw, 92, 'house')
        c.poly([(hx - 4, 92 - hh), (hx + hw // 2, 92 - hh - 12), (hx + hw + 4, 92 - hh)], 'roofp')
        c.rect(hx + 8, 92 - hh + 8, hx + 14, 92 - hh + 13, 'win')
    c.rect(0, 118, W - 1, H - 1, 'sand1')
    for _ in range(80):
        c.put(rnd.randrange(W), rnd.randrange(118, H), 'sand0')
    c.poly([(10, 126), (70, 120), (64, 134), (18, 136)], 'rock1')  # the beached boat
    person(c, 128, 156, 34, 'coat2')
    if growlithe:
        paste(c, growlithe_sprite(), 140, 108, grade=DAWN, scale=0.75)
    return c


PANELS = [
    ('f01', f01_choix), ('f02a', f02a_centre), ('f02b', f02b_arene), ('f03', f03_carnet),
    ('f04', f04_refugies), ('f05a', f05a_guides), ('f05b', f05b_caches), ('f06', f06_blesse),
    ('f07a', f07a_soigne), ('f07b', f07b_ignore), ('f08', f08_bracelet),
    ('f09_00', lambda: f09_bateau(False, False)), ('f09_01', lambda: f09_bateau(True, False)),
    ('f09_10', lambda: f09_bateau(False, True)), ('f09_11', lambda: f09_bateau(True, True)),
    ('f10', f10_tempete), ('f11', f11_marin),
    ('f12_0', lambda: f12_palette(False)), ('f12_1', lambda: f12_palette(True)),
]

if __name__ == '__main__':
    for name, fn in PANELS:
        nt, nc = to_gba(fn().im, name)
        print('%s: %d tiles, %d colours' % (name, nt, nc))
