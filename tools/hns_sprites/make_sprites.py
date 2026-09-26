#!/usr/bin/env python3
"""Heart & Soul custom overworld sprites.

Each sprite is derived from an existing HNS sheet (same 16x32 x 9-frame layout, same pixel
style) with a new dedicated 16-colour palette, plus optional hand-placed pixel edits.
Outputs, per sprite: graphics/object_events/pics/people/hns_custom/<name>.png (indexed) and
graphics/object_events/palettes/<name>.pal (JASC). Run from the repo root.
"""
import os
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PEOPLE = os.path.join(ROOT, 'graphics/object_events/pics/people')
OUT_PIC = os.path.join(PEOPLE, 'hns_custom')
OUT_PAL = os.path.join(ROOT, 'graphics/object_events/palettes')


def hexrgb(h):
    h = h.lstrip('#')
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))


SPRITES = {
    # Melanie, gardienne du Village Cache (anime EP010): cheveux chatains, tenue verte, accents roses.
    'melanie_hns': {
        'base': 'lass_hns.png',
        'recolor': {6: '#c08a4c', 7: '#8a5530', 8: '#8ccf64', 9: '#3f8f3f', 10: '#1f4a24',
                    11: '#f29ab8', 12: '#b84c7c', 13: '#4a2a3a'},
        # little white apron pocket on the front-facing frames (frame 0 and walk frames 3/4)
        'pixels': [],
    },
    # Kaz, chef de l'arene libre "Kaz" (anime Dark City): kimono cramoisi, ceinture or.
    'kaz_hns': {
        'base': 'black_belt_hns.png',
        'recolor': {11: '#e0523e', 12: '#a02c26', 13: '#4c1414', 6: '#4a3426', 7: '#1c120c'},
        'pixels': [],
        'shades': True,  # hand-drawn black sunglasses, his trademark in the free-gym brawls
    },
    # Yas, chef de l'arene libre rivale: cheveux argent, blouson jaune electrique.
    'yas_hns': {
        'base': 'cooltrainer_m_hns.png',
        'recolor': {8: '#eeeef6', 9: '#a8a8bc', 10: '#565670', 11: '#f4d444', 12: '#b88c14', 13: '#3a3020'},
        'pixels': [],
    },
    # Vesper, recruteuse de Blue: cheveux violet nuit, tailleur anthracite.
    'vesper_hns': {
        'base': 'rockets/ariana_hns.png',
        'recolor': {1: '#24163e', 2: '#46287a', 3: '#6e40b4', 10: '#5c5c6c', 11: '#a0a0b0', 15: '#7c7c8c',
                    13: '#d8a828'},
        'pixels': [],
    },
}


def draw_shades(px):
    """Sunglasses drawn by hand on the eye row of every frame (coordinates read off the base
    black_belt_hns sheet: front frames 0/3/4, side frames 2/7/8; walk frames bob one pixel)."""
    front = {0: 19, 3: 20, 4: 20}
    side = {2: 19, 7: 20, 8: 20}
    for f, y in front.items():
        for x in range(4, 12):
            px[f * 16 + x, y] = 15
    for f, y in side.items():
        for x in range(4, 8):
            px[f * 16 + x, y] = 15


def build(name, spec):
    im = Image.open(os.path.join(PEOPLE, spec['base']))
    assert im.mode == 'P'
    im = im.crop((0, 0, 144, 32))  # 9 frames of 16x32, the layout every HNS pic table uses
    pal = im.getpalette()[:48]
    for idx, col in spec['recolor'].items():
        pal[idx * 3: idx * 3 + 3] = hexrgb(col)
    im.putpalette(pal + [0] * (768 - 48))
    px = im.load()
    for (x, y, idx) in spec['pixels']:
        px[x, y] = idx
    if spec.get('shades'):
        draw_shades(px)
    os.makedirs(OUT_PIC, exist_ok=True)
    im.save(os.path.join(OUT_PIC, name + '.png'), transparency=0)
    with open(os.path.join(OUT_PAL, name + '.pal'), 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for i in range(16):
            f.write('%d %d %d\n' % tuple(pal[i * 3:i * 3 + 3]))
    return im


if __name__ == '__main__':
    for n, s in SPRITES.items():
        build(n, s)
        print('built', n)


# ---------------------------------------------------------------------------------------
# Trainer battle portraits (64x64), recoloured from existing pics so that the battle
# portrait matches the overworld sprite. Output: graphics/trainers/front_pics/<name>.png and
# graphics/trainers/palettes/<name>.pal.
# ---------------------------------------------------------------------------------------
TRAINERS = {
    'kaz_hns': {'base': 'black_belt.png',
                'recolor': {9: '#e0523e', 10: '#b8402f', 11: '#8a2a22'}},
    'yas_hns': {'base': 'cooltrainer_m.png',
                'recolor': {9: '#565670', 10: '#e6e6f0', 11: '#a8a8bc',
                            5: '#f8e070', 6: '#f0c830', 7: '#b08a10', 8: '#3a3020'}},
    'vesper_hns': {'base': 'ariana_hns.png',
                   'recolor': {1: '#46287a', 3: '#6e40b4', 4: '#24163e', 15: '#8a58d0',
                               9: '#7c7c8c', 10: '#a0a0b0', 8: '#5c5c6c'}},
}


def build_trainer(name, spec):
    fp = os.path.join(ROOT, 'graphics/trainers/front_pics')
    im = Image.open(os.path.join(fp, spec['base']))
    assert im.mode == 'P' and im.size == (64, 64)
    pal = im.getpalette()[:48]
    for idx, col in spec['recolor'].items():
        pal[idx * 3: idx * 3 + 3] = hexrgb(col)
    im.putpalette(pal + [0] * (768 - 48))
    im.save(os.path.join(fp, name + '.png'), transparency=0)
    with open(os.path.join(ROOT, 'graphics/trainers/palettes', name + '.pal'), 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n')
        for i in range(16):
            f.write('%d %d %d\n' % tuple(pal[i * 3:i * 3 + 3]))


if __name__ == '__main__':
    for n, s in TRAINERS.items():
        build_trainer(n, s)
        print('built trainer pic', n)
