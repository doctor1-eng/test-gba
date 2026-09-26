#!/usr/bin/env python3
"""Heart & Soul headless playtest driver (see tools/playtest/README.md).

Usage: pt.py ROM SCENARIO [--out DIR]
A scenario is a text file of harness commands (see harness.c) plus these extensions:
  goto MAP_CONSTANT X Y [FLAG_... VAR_...=N ...]   arm the in-game playtest hook (warp + flags/vars)
  setflag FLAG_...  | clearflag FLAG_... | setvar VAR_... N   poke save data directly
  checkflag FLAG_... | checkvar VAR_...             print a flag/var value
  shot NAME                                         screenshot saved as DIR/NAME.png (PNG, x2)
  walk DIR N                                        walk N tiles (UP/DOWN/LEFT/RIGHT), 16 frames each
  talk [N]                                          press A N times with message-scroll delays
Constants are resolved with the C preprocessor against the repo headers, and symbols from the ELF.
"""
import os, re, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
HARNESS = os.path.join(ROOT, 'tools/playtest/harness')
_const_cache = {}


def const(name):
    if re.fullmatch(r'-?(0x[0-9a-fA-F]+|\d+)', name):
        return int(name, 0)
    if name in _const_cache:
        return _const_cache[name]
    if name.startswith('MAP_'):
        m = re.search(r'\b%s\s*=\s*\((\d+)\s*\|\s*\((\d+)\s*<<\s*8\)\)' % name,
                      open(os.path.join(ROOT, 'include/constants/map_groups.h')).read())
        if m:
            return int(m.group(1)) | (int(m.group(2)) << 8)
    src = ('#include "global.h"\n#include "constants/flags.h"\n#include "constants/vars.h"\n'
           '#include "constants/map_groups.h"\n#include "constants/species.h"\n#include "constants/items.h"\n'
           '@@@ %s\n' % name)
    out = subprocess.run(['arm-none-eabi-cpp', '-P', '-iquote', 'include', '-DMODERN=1', '-DTESTING=0',
                          '-DPOKEMON_HNS', '-std=gnu17', '-x', 'c', '-'],
                         input=src, capture_output=True, text=True, cwd=ROOT)
    m = re.search(r'@@@ (.*)', out.stdout)
    if not m:
        raise SystemExit('cannot resolve %s\n%s' % (name, out.stderr[-2000:]))
    expr = m.group(1).strip()
    try:
        val = eval(re.sub(r'\b(0x[0-9a-fA-F]+|\d+)[uUlL]+\b', r'\1', expr), {}, {})
    except Exception:
        raise SystemExit('cannot evaluate %s = %s' % (name, expr))
    _const_cache[name] = val
    return val


def symbols(elf):
    out = subprocess.run(['arm-none-eabi-nm', elf], capture_output=True, text=True).stdout
    syms = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3:
            syms[parts[2]] = int(parts[0], 16)
    return syms


def probe(exprs):
    """Evaluate integer C expressions (sizeof/offsetof...) with the real cross compiler."""
    src = '#include "global.h"\n#include "battle.h"\n#include "pokemon.h"\n#include <stddef.h>\n'
    for i, e in enumerate(exprs):
        src += 'const int kProbe%d = %s;\n' % (i, e)
    with tempfile.TemporaryDirectory() as d:
        c = os.path.join(d, 'p.c')
        open(c, 'w').write(src)
        cmd = ('arm-none-eabi-cpp -iquote include -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -std=gnu17 %s | '
               'tools/preproc/preproc -i %s charmap.txt | /usr/lib/gcc/arm-none-eabi/13.2.1/cc1 -quiet -mthumb '
               '-mthumb-interwork -O2 -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t -std=gnu17 -o - -' % (c, c))
        r = subprocess.run(['bash', '-c', cmd], capture_output=True, text=True, cwd=ROOT)
        out = []
        for i in range(len(exprs)):
            m = re.search(r'kProbe%d:\s*\.word\s+(-?\d+)' % i, r.stdout)
            if not m:
                raise SystemExit('probe failed for %s\n%s' % (exprs[i], r.stderr[-2000:]))
            out.append(int(m.group(1)))
        return out


def offsets():
    """Byte offsets of SaveBlock1.flags / .vars, computed by the cross compiler itself."""
    src = ('#include "global.h"\n#include <stddef.h>\n'
           'const int kFlags = offsetof(struct SaveBlock1, flags);\n'
           'const int kVars = offsetof(struct SaveBlock1, vars);\n')
    with tempfile.TemporaryDirectory() as d:
        c = os.path.join(d, 'o.c')
        open(c, 'w').write(src)
        cmd = ('arm-none-eabi-cpp -iquote include -DMODERN=1 -DTESTING=0 -DPOKEMON_HNS -std=gnu17 %s | '
               'tools/preproc/preproc -i %s charmap.txt | /usr/lib/gcc/arm-none-eabi/13.2.1/cc1 -quiet -mthumb -mthumb-interwork -O2 -mabi=apcs-gnu '
               '-mtune=arm7tdmi -march=armv4t -std=gnu17 -o - -' % (c, c))
        r = subprocess.run(['bash', '-c', cmd], capture_output=True, text=True, cwd=ROOT)
        f = re.search(r'kFlags:\s*\.word\s+(\d+)', r.stdout)
        v = re.search(r'kVars:\s*\.word\s+(\d+)', r.stdout)
        if not f or not v:
            raise SystemExit('offset probe failed\n' + r.stderr[-3000:])
        return int(f.group(1)), int(v.group(1))


def main():
    rom, scen = sys.argv[1], sys.argv[2]
    outdir = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else '/tmp/claude-0/pt/out'
    os.makedirs(outdir, exist_ok=True)
    elf = rom[:-4] + '.elf'
    syms = symbols(elf)
    sb1ptr = syms['gSaveBlock1Ptr']
    req = syms['gHnsPlaytest']
    fl_off, var_off = offsets()
    vars_start = const('VARS_START')
    lines, shots = [], []

    def sb1(off):  # address expression resolved at runtime by the harness: pointer deref
        return '*%d+%d' % (sb1ptr, off)

    for raw in open(scen):
        line = raw.split('#', 1)[0].strip()
        if not line:
            continue
        t = line.split()
        c = t[0]
        if c == 'goto':
            g = const(t[1])
            lines.append('w8 %d %d' % (req + 4, (g >> 8) & 0xFF))  # mapGroup
            lines.append('w8 %d %d' % (req + 5, g & 0xFF))  # mapNum
            lines.append('w8 %d %d' % (req + 6, int(t[2]) & 0xFF))
            lines.append('w8 %d %d' % (req + 7, int(t[3]) & 0xFF))
            fi = vi = 0
            for extra in t[4:]:
                if '=' in extra:
                    n, v = extra.split('=')
                    lines.append('w16 %d %d' % (req + 8 + 48 + vi * 4, const(n)))
                    lines.append('w16 %d %d' % (req + 8 + 48 + vi * 4 + 2, const(v)))
                    vi += 1
                else:
                    lines.append('w16 %d %d' % (req + 8 + fi * 2, const(extra)))
                    fi += 1
            lines.append('w32 %d %d' % (req, 0x54504E48))
            lines.append('waitmem %d 0 6000' % req)  # hook consumed = player had field control
            lines.append('frames 90')
        elif c in ('setflag', 'clearflag', 'checkflag'):
            f = const(t[1])
            addr = sb1(fl_off + f // 8)
            bit = 1 << (f % 8)
            lines.append({'setflag': 'or8', 'clearflag': 'and8', 'checkflag': 'r8'}[c] + ' %s %d' %
                         (addr, bit if c != 'clearflag' else (~bit) & 0xFF) + ('' if c != 'checkflag' else ''))
            if c == 'checkflag':
                lines[-1] = 'bit8 %s %d %s' % (addr, bit, t[1])
        elif c == 'untilflag':
            # repeat the following block (up to 'end') until FLAG is set; FLAG may be FLAG_X or
            # TRAINER:TRAINER_X (that trainer's defeated flag)
            if t[1].startswith('TRAINER:'):
                f = const('TRAINER_FLAGS_START') + const(t[1][8:])
            else:
                f = const(t[1])
            lines.append('until8 %s %d' % (sb1(fl_off + f // 8), 1 << (f % 8)))
        elif c == 'setvar':
            v = const(t[1]) - vars_start
            lines.append('w16 %s %d' % (sb1(var_off + v * 2), const(t[2])))
        elif c == 'checkvar':
            v = const(t[1]) - vars_start
            lines.append('r16 %s %s' % (sb1(var_off + v * 2), t[1]))
        elif c == 'battle':
            # Deterministic battle autopilot: every step, set every opposing battler (and the
            # enemy party) to 1 HP and every player battler to full HP, then press A.
            # Validates the SCRIPT around a battle, not the battle balance.
            if 'bm' not in locals():
                bm_size, bm_hp, bm_maxhp, mon_size, mon_hp, bm_pp = probe([
                    'sizeof(struct BattlePokemon)', 'offsetof(struct BattlePokemon, hp)',
                    'offsetof(struct BattlePokemon, maxHP)', 'sizeof(struct Pokemon)', 'offsetof(struct Pokemon, hp)',
                    'offsetof(struct BattlePokemon, pp)'])
                bm = syms['gBattleMons']; ep = syms['gEnemyParty']
            for step in range(int(t[1])):
                for b in (1, 3):
                    lines.append('min16 %d 1' % (bm + b * bm_size + bm_hp))
                for b in (0, 2):  # player side: 999 HP, so nothing can one-shot it
                    lines.append('set16nz %d 999' % (bm + b * bm_size + bm_maxhp))
                    lines.append('set16nz %d 999' % (bm + b * bm_size + bm_hp))
                    lines.append('w8 %d 10' % (bm + b * bm_size + bm_pp))  # first move never runs out
                for k in range(6):
                    lines.append('min16 %d 1' % (ep + k * mon_size + mon_hp))
                lines.append('tap A 1')
                lines.append('frames 20')
                if step % 7 == 6:  # backs out of the "switch Pokemon?" party screen (SHIFT style)
                    lines.append('tap B 1')
        elif c == 'fastbattles':
            # Options: battle style SET + battle animations off (SaveBlock2 0x14 bitfield,
            # include/global.h: bit 9 = optionsBattleStyle, bit 10 = optionsBattleSceneOff)
            lines.append('or8 *%d+%d 6' % (syms['gSaveBlock2Ptr'], 0x15))
        elif c == 'shot':
            p = os.path.join(outdir, t[1] + '.ppm')
            shots.append(p)
            lines.append('shot ' + p)
        elif c == 'walk':
            n = int(t[2])
            lines.append('press %s %d 2' % (t[1], 16 * n - 2))
        elif c == 'talk':
            n = int(t[1]) if len(t) > 1 else 1
            for _ in range(n):
                lines.append('frames 40')
                lines.append('tap A 1')
        else:
            lines.append(line)
    script = os.path.join(outdir, '_script.txt')
    open(script, 'w').write('\n'.join(lines) + '\n')
    r = subprocess.run([HARNESS, rom, script], capture_output=True, text=True)
    sys.stdout.write(r.stdout)
    sys.stderr.write(r.stderr)
    try:
        from PIL import Image
        for p in shots:
            if os.path.exists(p):
                im = Image.open(p)
                im.resize((480, 320), Image.NEAREST).save(p[:-4] + '.png')
                os.remove(p)
    except ImportError:
        pass
    sys.exit(r.returncode)


if __name__ == '__main__':
    main()
