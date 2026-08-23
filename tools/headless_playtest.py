#!/usr/bin/env python3
"""
Headless, real-emulation visual test harness.

Requires: pip install mgba (the official mGBA Python core bindings --
a genuine libmgba wrapper, NOT the SDL/Qt frontend. Runs the actual GBA
CPU/PPU core in-process and exposes the raw framebuffer, so it works
in a bare environment with no X server/display at all).

Found and used 2026-08-23 after `mgba-sdl` under Xvfb proved unusable in
this environment (window created, SDL reported focus, but the core
never actually rendered a frame -- even the pristine original ROM
stayed solid black; not a ROM bug, just an unrelated environment
limitation of that particular frontend/setup). This tool sidesteps the
whole problem: no SDL, no X server, no window -- just the core and a
framebuffer.

Usage:
    python3 tools/headless_playtest.py <rom.gba> <out_dir> [n_presses]

Boots the ROM, waits for the intro logos, then repeatedly presses A
(mashing through dialogue, mimicking a player advancing text boxes) and
periodically Start, saving a screenshot + printing an image-variance
("std") number at each checkpoint. Two independent runs (e.g. the
original English ROM and a translated build) can be diffed frame-by-
frame -- either automatically (std tracks structurally similar content
even when the text itself differs) or visually (open the PNGs) -- to
confirm a fix without ever needing a real device or display.

This is how the reference-corruption bugs documented in docs/PROGRESS.md
were bisected and confirmed fixed: build_partial-style incremental ROM
reconstruction from build/patch_log.tsv, combined with this harness, let
a specific corrupting patch be located automatically rather than guessed
at from static analysis alone.
"""
import sys
import logging
logging.disable(logging.CRITICAL)
import mgba.core
import mgba.image
import mgba.log
from mgba._pylib import lib
import numpy as np
mgba.log.silence()


def main():
    rom_path = sys.argv[1]
    out_dir = sys.argv[2]
    n_presses = int(sys.argv[3]) if len(sys.argv) > 3 else 150

    core = mgba.core.load_path(rom_path)
    core.autoload_save()
    w, h = core.desired_video_dimensions()
    img = mgba.image.Image(w, h)
    core.set_video_buffer(img)
    core.reset()

    def run(n):
        for _ in range(n):
            core.run_frame()

    def press(key, hold=4, release=14):
        core.add_keys(key)
        run(hold)
        core.clear_keys(key)
        run(release)

    def snapshot(name):
        png_path = f"{out_dir}/{name}.png"
        with open(png_path, "wb") as f:
            img.save_png(f)
        from PIL import Image
        arr = np.asarray(Image.open(png_path).convert("RGB"))
        return arr.std()

    run(300)  # let boot/logo sequence play out
    stds = [(0, snapshot("s0000"))]
    for i in range(1, n_presses):
        press(lib.GBA_KEY_A, hold=4, release=10)
        if i % 3 == 0:
            press(lib.GBA_KEY_START, hold=3, release=6)
        if i % 2 == 0:
            stds.append((i, snapshot(f"s{i:04d}")))

    for i, s in stds:
        print(f"{i}\t{s:.2f}")


if __name__ == "__main__":
    main()
