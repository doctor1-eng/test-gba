#!/usr/bin/env python3
"""Turn a harness recording (PPM frames) into a compact animated GIF: consecutive identical
frames are merged into one longer frame (so static dialogue pages stay on screen as long as
they did in the game, up to a cap). make_gif.py FRAME_DIR OUT.gif [ms_per_frame] [scale]"""
import glob, sys
from PIL import Image
d, out = sys.argv[1], sys.argv[2]
ms = int(sys.argv[3]) if len(sys.argv) > 3 else 50
scale = int(sys.argv[4]) if len(sys.argv) > 4 else 2
frames, durs, prev = [], [], None
for f in sorted(glob.glob(d + '/f*.ppm')):
    im = Image.open(f).convert('RGB')
    b = im.tobytes()
    if b == prev:
        durs[-1] = min(durs[-1] + ms, 2600)
        continue
    prev = b
    # quantize right away: keeps memory flat on long recordings
    frames.append(im.resize((im.width * scale, im.height * scale), Image.NEAREST).quantize(colors=255, method=Image.Quantize.MEDIANCUT))
    durs.append(ms)
pal = frames
pal[0].save(out, save_all=True, append_images=pal[1:], duration=durs, loop=0, optimize=True, disposal=1)
print(out, len(frames), 'frames', sum(durs) / 1000, 's')
