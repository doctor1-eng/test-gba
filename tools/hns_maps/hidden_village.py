#!/usr/bin/env python3
"""Builds data/layouts/HiddenVillage_Hns/map.bin (Heart & Soul, Village Cache) by editing a copy of
PalletTown_hns's layout with metatiles of the same tilesets: one cabin kept, a flower meadow and a
fenced pen where the rescued Pokemon live, the pond framed by trees, south edge sealed.
Run from the repo root: python3 tools/hns_maps/hidden_village.py data/layouts/HiddenVillage_Hns/map.bin"""
import struct, sys
W,H=24,20
src=open('data/layouts/PalletTown_hns/map.bin','rb').read()
g=[list(struct.unpack('<%dH'%W, src[y*W*2:(y+1)*W*2])) for y in range(H)]
def put(x,y,mt,col=0): g[y][x]=(mt&0x3ff)|(col<<10)|(g[y][x]&0xF000)
def tree(x,y):  # 2x2 tree block like the border
    put(x,y,0x1c,1); put(x+1,y,0x1d,1); put(x,y+1,0x14,1); put(x+1,y+1,0x15,1)
def rect(x0,y0,x1,y1,mt,col=0):
    for y in range(y0,y1+1):
        for x in range(x0,x1+1): put(x,y,mt,col)
# 1) right house block (x12..21, y2..8) -> ground, then a copse of trees
rect(12,3,20,8,0x296)
rect(13,4,19,7,0x4)                # open flower meadow where the rescued Pokemon roam
# 2) lab block (x13..20, y9..16) -> flower meadow fenced like the flower bed
rect(12,9,20,16,0x296)
rect(14,11,19,11,0x284,1)          # fence top
rect(14,12,19,14,0x4)              # flowers (the pen)
put(16,11,0x2,0)                   # gap in fence (walkable post tile used in Pallet)
# 3) seal the south edge with trees
for x in list(range(0,6,2))+list(range(12,24,2)):
    tree(x,18)
rect(6,18,6,19,0x1b,1)             # tree edge next to the pond
rect(11,18,11,19,0x1a,1)
out=b''.join(struct.pack('<%dH'%W,*row) for row in g)
open(sys.argv[1],'wb').write(out)
