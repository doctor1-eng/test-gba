// Harness de QA headless : charge la ROM, exécute une séquence de touches, capture des PNG.
#include <mgba/core/core.h>
#include <mgba/core/blip_buf.h>
#include <mgba/gba/core.h>
#include <mgba-util/vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// Format du script d'entrée : lignes "FRAMES KEYMASK" ou "SHOT nom.png"
// KEYMASK bits : A=1 B=2 SELECT=4 START=8 RIGHT=16 LEFT=32 UP=64 DOWN=128 R=256 L=512

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s rom.gba script.txt [outdir]\n", argv[0]);
        return 1;
    }
    const char* romPath = argv[1];
    const char* scriptPath = argv[2];
    const char* outDir = argc > 3 ? argv[3] : ".";

    struct mCore* core = mCoreFind(romPath);
    if (!core) { fprintf(stderr, "Impossible de charger le coeur pour %s\n", romPath); return 1; }
    core->init(core);

    unsigned width, height;
    core->desiredVideoDimensions(core, &width, &height);
    color_t* buffer = malloc(width * height * BYTES_PER_PIXEL);
    core->setVideoBuffer(core, buffer, width);

    mCoreLoadFile(core, romPath);
    mCoreConfigInit(&core->config, "qa_runner");
    mCoreConfigLoad(&core->config);

    struct mCoreOptions opts = {0};
    mCoreConfigMap(&core->config, &opts);
    opts.audioSync = false;
    opts.videoSync = false;
    mCoreConfigLoadDefaults(&core->config, &opts);
    mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "detect");
    mCoreLoadConfig(core);

    core->reset(core);

    FILE* script = fopen(scriptPath, "r");
    if (!script) { fprintf(stderr, "Script introuvable: %s\n", scriptPath); return 1; }

    char line[256];
    while (fgets(line, sizeof(line), script)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (strncmp(line, "SHOT", 4) == 0) {
            char name[200];
            sscanf(line + 5, "%199s", name);
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", outDir, name);
            struct VFile* out = VFileOpen(path, O_CREAT | O_TRUNC | O_WRONLY);
            if (out) {
                mCoreTakeScreenshotVF(core, out);
                out->close(out);
                printf("Capture: %s\n", path);
            }
        } else {
            int frames, keys;
            if (sscanf(line, "%d %d", &frames, &keys) == 2) {
                core->setKeys(core, keys);
                for (int i = 0; i < frames; i++) {
                    core->runFrame(core);
                }
                core->setKeys(core, 0);
            }
        }
    }
    fclose(script);
    core->deinit(core);
    free(buffer);
    return 0;
}
