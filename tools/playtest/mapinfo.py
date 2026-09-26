#!/usr/bin/env python3
"""Map analysis helper: mapinfo.py MAP_DIR_NAME [--doors] [--grid]
Prints layout info, warps, objects, and (with --doors) every door/warp-behaviour tile with its warp status.
--grid prints the collision grid ('.' walkable, '#' blocked, 'D' door, 'W' warp, 'O' object)."""
import json, os, re, struct, sys
root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
def behaviors():
    txt = open(os.path.join(root, 'include/constants/metatile_behaviors.h')).read()
    body = txt[txt.index('enum'):]
    body = body[body.index('{') + 1: body.index('}')]
    names, v = {}, 0
    for line in body.split('\n'):
        line = re.sub(r'//.*', '', line).strip().rstrip(',')
        if not line: continue
        if '=' in line:
            n, val = [x.strip() for x in line.split('=')]
            v = int(val, 0)
        else: n = line
        names[v] = n; v += 1
    return names
BEH = behaviors()
lay = json.load(open(os.path.join(root, 'data/layouts/layouts.json')))['layouts']
def tsdir(sym, kind):
    base = re.sub(r'^gTileset_', '', sym)
    snake = re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '_', base).lower()
    for cand in (snake, base.lower()):
        p = os.path.join(root, 'data/tilesets', kind, cand)
        if os.path.isdir(p): return p
def attrs(d):
    b = open(os.path.join(d, 'metatile_attributes.bin'), 'rb').read()
    return list(struct.unpack('<%dH' % (len(b) // 2), b))
def load(mapname):
    m = json.load(open(os.path.join(root, 'data/maps', mapname, 'map.json')))
    L = next(l for l in lay if l.get('id') == m['layout'])
    raw = open(os.path.join(root, L['blockdata_filepath']), 'rb').read()
    cells = struct.unpack('<%dH' % (len(raw) // 2), raw)
    pa = attrs(tsdir(L['primary_tileset'], 'primary'))
    sa = attrs(tsdir(L['secondary_tileset'], 'secondary'))
    npri = int(re.search(r'#define NUM_METATILES_IN_PRIMARY\s+(\d+)', open(os.path.join(root, 'include/fieldmap.h')).read()).group(1))
    def beh(mt):
        a = pa[mt] if mt < npri else (sa[mt - npri] if mt - npri < len(sa) else 0)
        return BEH.get(a & 0xFF, str(a & 0xFF))
    return m, L, cells, beh
if __name__ == '__main__':
    name = sys.argv[1]
    m, L, cells, beh = load(name)
    W, H = L['width'], L['height']
    print(name, L['id'], W, 'x', H, L['primary_tileset'], L['secondary_tileset'])
    warps = {(w['x'], w['y']): w for w in m.get('warp_events', [])}
    for i, w in enumerate(m.get('warp_events', [])): print(' warp', i, (w['x'], w['y']), w['dest_map'], w['dest_warp_id'])
    for o in m.get('object_events', []): print(' obj', o.get('local_id', ''), (o['x'], o['y']), o['graphics_id'], o.get('script'), o.get('flag'))
    for c in m.get('coord_events', []): print(' coord', (c['x'], c['y']), c.get('script'), c.get('var'), c.get('var_value'))
    for b in m.get('bg_events', []): print(' bg', (b['x'], b['y']), b.get('type'), b.get('script'))
    if '--doors' in sys.argv:
        for y in range(H):
            for x in range(W):
                b = beh(cells[y * W + x] & 0x3FF)
                if 'DOOR' in b or 'LADDER' in b or 'WARP' in b or 'STAIRS' in b or 'CAVE' in b and 'ENTRANCE' in b:
                    print(' door-tile', (x, y), b, 'WARP->' + warps[(x, y)]['dest_map'] if (x, y) in warps else 'NO WARP')
    if '--grid' in sys.argv:
        objs = {(o['x'], o['y']) for o in m.get('object_events', [])}
        for y in range(H):
            row = ''
            for x in range(W):
                c = cells[y * W + x]
                b = beh(c & 0x3FF)
                ch = '#' if (c >> 10) & 3 else '.'
                if 'DOOR' in b: ch = 'D'
                if (x, y) in warps: ch = 'W'
                if (x, y) in objs: ch = 'O'
                row += ch
            print('%3d %s' % (y, row))
