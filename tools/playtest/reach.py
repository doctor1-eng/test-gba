#!/usr/bin/env python3
"""Flood-fill reachability on a map's collision layer: reach.py MAP_DIR START_X START_Y X Y [X Y ...]"""
import sys
sys.path.insert(0, __import__('os').path.dirname(__file__))
import mapinfo as M
m, L, c, beh = M.load(sys.argv[1])
W, H = L['width'], L['height']
objs = {(o['x'], o['y']) for o in m.get('object_events', []) if 'CUTTABLE' not in o['graphics_id'] or '--solid-trees' in sys.argv}
def free(x, y):
    return 0 <= x < W and 0 <= y < H and not ((c[y * W + x] >> 10) & 3) and (x, y) not in objs
sx, sy = int(sys.argv[2]), int(sys.argv[3])
seen, st = {(sx, sy)}, [(sx, sy)]
while st:
    x, y = st.pop()
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        n = (x + dx, y + dy)
        if n not in seen and free(*n):
            seen.add(n); st.append(n)
t = sys.argv[4:]
for i in range(0, len(t), 2):
    p = (int(t[i]), int(t[i + 1]))
    print(p, 'REACHABLE' if p in seen else 'unreachable', 'free' if free(*p) else 'blocked')
