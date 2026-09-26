#!/usr/bin/env python3
"""Render a layout by id or name with tools/gba_tiles: render_layout.py LAYOUT_ID_OR_NAME OUT.png [scale]"""
import json, os, re, subprocess, sys
root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
lay = json.load(open(os.path.join(root, 'data/layouts/layouts.json')))['layouts']
key = sys.argv[1]
L = next(l for l in lay if l.get('id') == key or l.get('name') == key or l.get('name') == key + '_Layout')
def tsdir(sym, kind):
    base = re.sub(r'^gTileset_', '', sym)
    snake = re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '_', base).lower()
    for cand in (snake, base.lower()):
        p = os.path.join(root, 'data/tilesets', kind, cand)
        if os.path.isdir(p):
            return p
    raise SystemExit(f'tileset dir not found for {sym}')
cmd = ['python3', os.path.join(root, 'gba_tiles.py'), 'render-map', '--map', os.path.join(root, L['blockdata_filepath']),
       '--width', str(L['width']), '--height', str(L['height']),
       '--primary-dir', tsdir(L['primary_tileset'], 'primary'), '--secondary-dir', tsdir(L['secondary_tileset'], 'secondary'),
       '--out', sys.argv[2], '--scale', sys.argv[3] if len(sys.argv) > 3 else '2']
print(L['id'], L['width'], 'x', L['height'], L['primary_tileset'], L['secondary_tileset'])
subprocess.run(cmd, check=True)
