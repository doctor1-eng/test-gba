# INSTALL.md — Reconstruction du projet (paquet léger)

Ce paquet ne contient PAS le moteur complet (trop volumineux — plusieurs centaines de Mo d'assets
graphiques/sonores communs à tous les hacks basés sur pokeemerald-expansion). Il contient uniquement :
- `changed_files/` — les fichiers qu'on a réellement modifiés ou créés (28 au 2026-08-12, Session 4 suite —
  liste amenée à grandir à chaque session, toujours resynchronisée depuis `engine/` avant chaque commit)
- `docs/` — toute la documentation du projet
- `tools/qa_harness/` — l'outil de QA headless
- `GETTING_STARTED.md` — contexte général du projet

## Étape 1 — Cloner le moteur de base
```bash
git clone https://github.com/rh-hideout/pokeemerald-expansion.git engine
```

## Étape 2 — Copier nos fichiers modifiés par-dessus
```bash
cp -r changed_files/* engine/
```

## Étape 3 — Installer la toolchain et compiler
```bash
sudo apt-get update
sudo apt-get install -y build-essential binutils-arm-none-eabi libpng-dev gcc-arm-none-eabi
cd engine
make MODERN=1 -j$(nproc)
```

## Étape 4 — Vérifier
La compilation doit se terminer sans erreur et produire `engine/pokeemerald.gba`. Si c'est le cas,
tout notre travail (Sessions 1 à 3) est bien restauré à l'identique.

## Ensuite
Lis `GETTING_STARTED.md` puis `docs/PROJECT_STATUS.md` pour la suite.

## Note technique
Ces 21 fichiers ont été identifiés par un `diff -rq` entre notre copie de travail et un clone frais de
`pokeemerald-expansion`. Quelques fichiers auto-générés (`trainers.h`, `region_map_entries.h`, etc.) ne
sont pas inclus — ils se régénèrent automatiquement au moment du build à partir des sources `.json`/`.party`
incluses.
