// Headless mGBA playtest harness for Heart & Soul.
// Reads commands (one per line) from a script file and drives the ROM with no display.
//   frames N              run N frames with no input
//   press KEYS N [W]      hold KEYS (e.g. A, B, UP, A+B) for N frames, then release W frames (default 8)
//   tap KEYS [COUNT]      press/release KEYS COUNT times (6 frames on, 10 off)
//   shot FILE.ppm         dump the current frame (240x160) as a PPM image
//   save FILE / load FILE savestate to / from file (tied to the exact ROM build)
//   battery FILE.sav      attach an in-game save file and reboot (survives rebuilds)
//   w8 ADDR VAL | w16 ADDR VAL | w32 ADDR VAL   write memory (hex or decimal)
//   r8 ADDR | r16 ADDR | r32 ADDR               print memory
//   echo TEXT             print TEXT
// Usage: harness ROM SCRIPT
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba/core/log.h>
#include <mgba-util/vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct mCore *core;
static color_t *fb;

static void quietLog(struct mLogger *l, int c, enum mLogLevel lv, const char *f, va_list a) {
    (void)l; (void)c; (void)lv; (void)f; (void)a;
}

static uint32_t parseKeys(const char *s) {
    static const char *names[] = {"A","B","SELECT","START","RIGHT","LEFT","UP","DOWN","R","L"};
    uint32_t k = 0;
    char buf[128];
    strncpy(buf, s, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = 0;
    for (char *t = strtok(buf, "+"); t; t = strtok(NULL, "+"))
        for (int i = 0; i < 10; i++)
            if (!strcmp(t, names[i])) k |= 1u << i;
    return k;
}

// Address syntax: plain number, or "*PTR+OFF" = (u32 at PTR) + OFF (for gSaveBlock1Ptr etc.).
static uint32_t addrOf(const char *s) {
    if (s[0] == '*') {
        char *end;
        uint32_t ptr = strtoul(s + 1, &end, 0);
        uint32_t off = *end == '+' ? strtoul(end + 1, NULL, 0) : 0;
        return core->busRead32(core, ptr) + off;
    }
    return strtoul(s, NULL, 0);
}

static char recPrefix[256];
static int recEvery = 0, recCount = 0, recFrame = 0;
static void shot(const char *path);
static void run(int n) {
    while (n-- > 0) {
        core->runFrame(core);
        if (recEvery && (recFrame++ % recEvery) == 0) {
            char p[300];
            snprintf(p, sizeof(p), "%s%05d.ppm", recPrefix, recCount++);
            shot(p);
        }
    }
}

static void shot(const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) { perror(path); return; }
    fprintf(f, "P6\n240 160\n255\n");
    for (int i = 0; i < 240 * 160; i++) {
        uint32_t c = fb[i];
        unsigned char px[3] = { c & 0xFF, (c >> 8) & 0xFF, (c >> 16) & 0xFF };
        fwrite(px, 1, 3, f);
    }
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s ROM SCRIPT\n", argv[0]); return 2; }
    static struct mLogger logger = { .log = quietLog };
    mLogSetDefaultLogger(&logger);
    core = mCoreFind(argv[1]);
    if (!core || !core->init(core)) { fprintf(stderr, "no core\n"); return 1; }
    mCoreInitConfig(core, NULL);
    unsigned w, h;
    core->desiredVideoDimensions(core, &w, &h);
    fb = calloc(w * h, sizeof(color_t));
    core->setVideoBuffer(core, fb, w);
    if (!mCoreLoadFile(core, argv[1])) { fprintf(stderr, "cannot load ROM\n"); return 1; }
    core->reset(core);

    FILE *s = strcmp(argv[2], "-") ? fopen(argv[2], "r") : stdin;
    if (!s) { perror(argv[2]); return 1; }
    // Whole script in memory so "until8 ADDR MASK" ... "end" blocks can repeat: the block runs
    // again and again until (u8 at ADDR) & MASK is non-zero (max 600 iterations).
    static char lines[20000][512];
    int nlines = 0;
    while (nlines < 20000 && fgets(lines[nlines], 512, s)) nlines++;
    int loopStart = -1, loopIter = 0;
    char loopAddr[256] = {0};
    uint32_t loopMask = 0;
    char line[512];
    for (int pc = 0; pc < nlines; pc++) {
        strcpy(line, lines[pc]);
        if (!strncmp(line, "until8 ", 7)) {
            char ad[256]; unsigned long mk;
            sscanf(line + 7, "%255s %lu", ad, &mk);
            strcpy(loopAddr, ad); loopMask = mk; loopStart = pc; loopIter = 0;
            continue;
        }
        if (!strncmp(line, "end", 3) && (line[3] == '\n' || line[3] == 0) && loopStart >= 0) {
            if (!(core->busRead8(core, addrOf(loopAddr)) & loopMask) && ++loopIter < 600) { pc = loopStart; continue; }
            printf("until8 %s done after %d iterations\n", loopAddr, loopIter);
            loopStart = -1;
            continue;
        }
        char cmd[32] = {0}, a1[256] = {0}, a2[64] = {0}, a3[64] = {0};
        if (sscanf(line, "%31s %255s %63s %63s", cmd, a1, a2, a3) < 1 || cmd[0] == '#') continue;
        if (!strcmp(cmd, "frames")) run(atoi(a1));
        else if (!strcmp(cmd, "press")) {
            core->setKeys(core, parseKeys(a1)); run(atoi(a2));
            core->setKeys(core, 0); run(a3[0] ? atoi(a3) : 8);
        } else if (!strcmp(cmd, "tap")) {
            int n = a2[0] ? atoi(a2) : 1;
            while (n-- > 0) { core->setKeys(core, parseKeys(a1)); run(6); core->setKeys(core, 0); run(10); }
        } else if (!strcmp(cmd, "shot")) shot(a1);
        else if (!strcmp(cmd, "record")) { strcpy(recPrefix, a1); recEvery = atoi(a2); recFrame = 0; }
        else if (!strcmp(cmd, "stoprecord")) recEvery = 0;
        else if (!strcmp(cmd, "waitmem")) {
            // waitmem ADDR VALUE MAXFRAMES: run frames until the u32 at ADDR equals VALUE
            uint32_t addr = addrOf(a1), want = strtoul(a2, NULL, 0);
            int max = a3[0] ? atoi(a3) : 3000, n = 0;
            while (core->busRead32(core, addr) != want && n < max) { core->runFrame(core); n++; }
            if (n >= max) printf("waitmem timeout at %s\n", a1);
        }
        else if (!strcmp(cmd, "battery")) {
            // Attach a battery save file (read/write) and reboot, like a cartridge that already
            // holds a save. Survives ROM rebuilds, unlike savestates.
            FILE *probe = fopen(a1, "rb");
            if (!probe) { // blank 128 KiB flash, as on a fresh cartridge
                FILE *nf = fopen(a1, "wb");
                for (int i = 0; nf && i < 131072; i++) fputc(0xFF, nf);
                if (nf) fclose(nf);
            } else fclose(probe);
            struct VFile *vf = VFileOpen(a1, O_RDWR);
            if (!vf || !core->loadSave(core, vf)) { fprintf(stderr, "cannot attach %s\n", a1); return 1; }
            core->reset(core);
        }
        else if (!strcmp(cmd, "save")) {
            struct VFile *vf = VFileOpen(a1, O_CREAT | O_TRUNC | O_RDWR);
            mCoreSaveStateNamed(core, vf, SAVESTATE_SAVEDATA); vf->close(vf);
        } else if (!strcmp(cmd, "load")) {
            struct VFile *vf = VFileOpen(a1, O_RDONLY);
            if (!vf) { fprintf(stderr, "no state %s\n", a1); return 1; }
            mCoreLoadStateNamed(core, vf, SAVESTATE_SAVEDATA); vf->close(vf);
        } else if (!strcmp(cmd, "or8") || !strcmp(cmd, "and8")) {
            uint32_t addr = addrOf(a1), v = core->busRead8(core, addr), m = strtoul(a2, NULL, 0);
            core->busWrite8(core, addr, cmd[0] == 'o' ? (v | m) : (v & m));
        } else if (!strcmp(cmd, "copy16")) {
            // copy only onto a non-zero value: never revive a fainted battler (that desyncs the
            // battle engine's switch-in flow and hangs the battle)
            uint32_t from = addrOf(a1), to = addrOf(a2);
            if (core->busRead16(core, to) != 0)
                core->busWrite16(core, to, core->busRead16(core, from));
        } else if (!strcmp(cmd, "set16nz")) {
            uint32_t addr = addrOf(a1);
            if (core->busRead16(core, addr) != 0) core->busWrite16(core, addr, strtoul(a2, NULL, 0));
        } else if (!strcmp(cmd, "min16")) {
            // clamp a non-zero u16 down to VAL (used to weaken opposing Pokemon without reviving fainted ones)
            uint32_t addr = addrOf(a1), v = core->busRead16(core, addr), m = strtoul(a2, NULL, 0);
            if (v > m) core->busWrite16(core, addr, m);
        } else if (!strcmp(cmd, "bit8")) {
            uint32_t addr = addrOf(a1), v = core->busRead8(core, addr), m = strtoul(a2, NULL, 0);
            printf("%s = %d\n", a3, (v & m) ? 1 : 0);
        } else if (cmd[0] == 'w') {
            uint32_t addr = addrOf(a1), v = strtoul(a2, NULL, 0);
            if (!strcmp(cmd, "w8")) core->busWrite8(core, addr, v);
            else if (!strcmp(cmd, "w16")) core->busWrite16(core, addr, v);
            else core->busWrite32(core, addr, v);
        } else if (cmd[0] == 'r') {
            uint32_t addr = addrOf(a1), v;
            if (!strcmp(cmd, "r8")) v = core->busRead8(core, addr);
            else if (!strcmp(cmd, "r16")) v = core->busRead16(core, addr);
            else v = core->busRead32(core, addr);
            printf("%s %s = %u (0x%X)\n", cmd, a2[0] ? a2 : a1, v, v);
        } else if (!strcmp(cmd, "echo")) printf("%s", line + 5);
        fflush(stdout);
    }
    core->deinit(core);
    return 0;
}
