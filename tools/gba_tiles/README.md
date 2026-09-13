# gba_tiles — Heart & Soul

Deux modes, un seul script : `python3 gba_tiles.py <commande> ...` depuis la
racine du dépôt (voir `../../gba_tiles.py`). Aucune dépendance externe (pas
de Pillow/numpy) : décodage PNG et format GBA écrits en Python stdlib pur,
testés dans `../../tests/test_gba_tiles.py`.

Calé directement sur le vrai schéma de ce dépôt (branche
`claude/pokemon-heart-soul-audit-qv0f4n`) : `data/tilesets/{primary,secondary}/*/`,
`data/layouts/*/map.bin`, `include/fieldmap.h` réels inspectés et testés en
rendu — pas seulement le format `pokeemerald-expansion` upstream générique.

## ⚠️ Deux pièges déjà rencontrés (et corrigés) dans ce dépôt précis

Ce tool a d'abord été écrit contre le format pokeemerald générique, avant
que ce dépôt réel ne soit disponible pour le tester. Deux bugs concrets sont
apparus au premier rendu d'une vraie carte (`VioletCity_Gym_hns`, qui
utilise un tileset secondaire) — corrigés, verrouillés par des tests de
non-régression (`CliPrimarySecondaryResolutionTests`), mais à connaître si tu
modifies ce code :

1. **Palette d'un tileset secondaire** : ce n'est PAS "tous les fichiers du
   primaire puis tous ceux du secondaire à partir de 0". Vérifié directement
   dans `src/fieldmap.c` (`LoadSecondaryTilesetPalette`) : le secondaire
   utilise **ses propres fichiers indexés `[num_pals_primary, num_pals_total)`**
   (ex. `07.pal`..`12.pal`), pas ses fichiers `00.pal`..`06.pal` (qui sont des
   placeholders inutilisés dans ce contexte). Se tromper ici ne plante rien
   mais décale toutes les couleurs du tileset secondaire — rendu qui a
   l'air "presque bon" mais faux.
2. **Metatiles d'une carte avec tileset secondaire** : les IDs de metatile
   d'une carte couvrent la plage combinée `[0, NUM_METATILES_TOTAL)`. Il faut
   concaténer le `metatiles.bin` du primaire ET celui du secondaire (chacun
   indexé localement à partir de 0), pas lire uniquement celui du primaire.
   Oublier ça fait disparaître silencieusement (rendu en case vide) tout
   metatile placé depuis le tileset secondaire — dans notre premier test ça
   a rendu une salle de Gym entière comme un simple aplat vert.

`--primary-dir`/`--secondary-dir` (voir plus bas) implémentent ces deux
règles automatiquement — préfère-les à l'assemblage manuel `--palettes`.

## ⚠️ Constante à vérifier avant chaque rendu : `include/fieldmap.h`

Ce fork bascule manuellement `NUM_TILES_IN_PRIMARY` / `NUM_PALS_IN_PRIMARY`
selon le type de carte édité (commentaire explicite en tête du fichier) :

- Cartes **HNS/FRLG** (valeur actuellement active) : 640 / 640 / 7
- Cartes **Emerald** : 512 / 512 / 6

Les valeurs par défaut du CLI (`--num-tiles-primary 640 --num-pals-primary 7
--num-pals-total 13`) reflètent le réglage actif au moment où ce tool a été
écrit. Si `include/fieldmap.h` a été re-basculé entre-temps, ou si tu rends
une carte Emerald, passe `--num-tiles-primary 512 --num-pals-primary 6`.

## Mode 1 — Visualisation

### Décoder un tileset en PNG lisible

```bash
python3 gba_tiles.py decode-tileset \
  --indexed-png data/tilesets/primary/alola_island/tiles.png \
  --palette data/tilesets/primary/alola_island/palettes/00.pal \
  --out tiles_repalette.png
```

Fonctionne aussi depuis un `.4bpp` brut compilé (`--raw` au lieu de
`--indexed-png`) si tu travailles à partir d'une ROM/binaire plutôt que des
sources du repo.

### Assembler tous les metatiles d'un tileset en planche

```bash
python3 gba_tiles.py render-tileset \
  --primary-dir data/tilesets/primary/alola_island \
  --metatiles-per-row 16 \
  --out planche_alola.png --scale 4
```

Avec tileset secondaire (pour résoudre les tuiles qu'un metatile du
primaire peut référencer dedans) :

```bash
python3 gba_tiles.py render-tileset \
  --primary-dir data/tilesets/primary/johto_building_hns \
  --secondary-dir data/tilesets/secondary/ecruteak_theater_hns \
  --metatiles-per-row 16 \
  --out planche_johto_building.png --scale 4
```

Produit la planche en résolution native et une version `_x4`/`_x8` upscalée
plus lisible pour une revue visuelle.

### Assembler une carte complète

Trouve `width`/`height`/`primary_tileset`/`secondary_tileset` dans
`data/layouts/layouts.json` (cherche l'`id` de ta `LAYOUT_*`), résous les
noms de tileset (`gTileset_Johto_Building_Hns` → dossier
`data/tilesets/primary/johto_building_hns`, minuscules/underscores — pas de
résolveur automatique, vérifie via `grep -rn "gTileset_XXX" include/tilesets.h`
si le nom de dossier n'est pas évident), puis :

```bash
python3 gba_tiles.py render-map \
  --map data/layouts/VioletCity_Gym_hns/map.bin --width 19 --height 21 \
  --primary-dir data/tilesets/primary/johto_building_hns \
  --secondary-dir data/tilesets/secondary/ecruteak_theater_hns \
  --out violet_gym.png --scale 4
```

**Cas d'usage direct pour ce projet** : `docs/heart_and_soul/technical_map.md`
liste plusieurs maps comme "à valider visuellement en émulateur (aucun rendu
graphique disponible côté agent)" — c'est précisément ce que cette commande
fournit sans émulateur. Exemple sur un des 3 blockouts de Cinnabar :

```bash
python3 gba_tiles.py render-map \
  --map data/layouts/CinnabarIsland_Gym_Hns/map.bin --width 13 --height 10 \
  --primary-dir data/tilesets/primary/johto_building_hns \
  --secondary-dir data/tilesets/secondary/house_lab_hns \
  --out cinnabar_gym.png --scale 8
```
→ Au rendu, cette pièce n'est **pas vide** : elle affiche le mobilier complet
du template `VermilionCity_House1_hns` d'où elle a été copiée (bibliothèque,
télé, tableau, table + 4 chaises, plantes, buffet, aquarium). La doc
existante la décrit comme "blockout identique, aucun mobilier thématique" —
à confirmer si c'est toujours l'état voulu ou si la doc est restée en retard
sur le `map.bin` réellement copié.

**Simplification assumée** : la composition couche haute/basse traite tout
pixel d'indice 0 sur la couche du dessus comme transparent (convention GBA
standard), mais ignore le `layer type` de `metatile_attributes.bin`
(covered/split/normal) — suffisant pour une revue visuelle, pas
pixel-perfect pour de rares metatiles à comportement spécial.

## Mode 2 — Recherche & import d'assets communautaires

Principe produit : chercher d'abord un asset réutilisable, ne générer qu'en
dernier recours. **Rien n'est jamais importé automatiquement.**

> **⚠️ À trancher avant d'utiliser `generate-fallback` sur ce projet** : la
> racine du dépôt (`README.md`, section "AI Disclosure") déclare
> explicitement *"AI has not been used for: Generating assets of any kind;
> Art or Music"*. `generate-fallback` ne fait pas appel à un modèle
> génératif (il recombine algorithmiquement des pixels déjà présents dans un
> tileset existant), mais reste une forme de génération d'asset par un
> outil plutôt qu'à la main par un membre de l'équipe. À valider avec
> l'équipe HnS avant d'utiliser cette commande sur des assets destinés au
> jeu, indépendamment de ce que dit ce README.

### 1. Chercher

```bash
python3 gba_tiles.py search-tiles "route forestière"
python3 gba_tiles.py search-tiles "intérieur pokécenter"
```

Interroge un catalogue local curé (`data/community_index.json` dans ce
dossier, constitué par recherche web le 2026-09-13 — packs Kenney.nl en CC0
site-wide + sélection OpenGameArt). Chaque résultat affiche titre, source,
**licence déclarée avec un niveau de confiance** (`high` = vérifié
site-wide, `medium`/`low` = à reconfirmer sur la page produit), tags, et
score de pertinence (lexique FR→EN intégré : "forêt", "intérieur",
"pokécenter", "grotte", "ville"...).

Pour classer par compatibilité de style (palette/détail) avec un tileset
existant du projet :

```bash
mkdir -p .tiles_preview_cache
# télécharge l'aperçu d'un candidat qui t'intéresse, nomme-le <id>.png
# (id affiché entre crochets par search-tiles)
python3 gba_tiles.py search-tiles "route forestière" \
  --project-tileset data/tilesets/primary/alola_island/tiles.png \
  --preview-cache-dir .tiles_preview_cache
```

Sans aperçu téléchargé, le score de style est affiché comme "non
calculable" plutôt que deviné.

### 2. Importer (validation explicite obligatoire)

```bash
python3 gba_tiles.py import-tile /chemin/vers/candidat_telecharge.png \
  --dest-dir data/tilesets/secondary/route_foret_nouveau \
  --name tiles \
  --source "https://opengameart.org/content/..." \
  --license "CC0" \
  --keyword "route forestière" \
  [--reuse-palette data/tilesets/primary/alola_island/palettes/00.pal] \
  [--dedupe-tiles]
```

- Convertit vers le format projet : PNG indexé, grille 8x8, ≤16 couleurs
  (quantification median-cut maison, ou mappage sur une palette existante
  via `--reuse-palette` — recommandé pour cohérence avec un tileset en place).
- **Règle bloquante** : `--source`, `--license`, `--keyword` obligatoires.
  Sans eux, la commande s'arrête et **n'écrit strictement aucun fichier**
  (ni la ligne de registre ni les images) — la conversion se fait en
  mémoire avant toute écriture disque.
- Ajoute une ligne à `ASSETS_SOURCES.md` (racine du dépôt) : date, mot-clé,
  source, licence, fichiers produits, hash SHA-256 du fichier source.

### 3. Vérifier qu'aucun import n'a contourné l'outil

```bash
python3 gba_tiles.py verify-sources data/tilesets
```

Échoue (`exit 1`) si un `.png` sous `data/tilesets` n'a pas de ligne
correspondante dans `ASSETS_SOURCES.md`. Utile en pré-commit/CI.

### 4. Fallback : génération seulement si rien de convenable trouvé

Voir l'avertissement AI Disclosure ci-dessus avant utilisation sur cet
projet.

```bash
python3 gba_tiles.py generate-fallback \
  --reference data/tilesets/primary/alola_island/tiles.png \
  --out proposition.png --seed 42
```

Reconstruit une tuile 8x8 à partir de la palette réelle et de la densité de
détail mesurée du tileset de référence — une proposition à revoir par un
humain, jamais un import automatique. Non journalisée dans
`ASSETS_SOURCES.md` (ce n'est pas un asset tiers).

## Tests

```bash
python3 -m unittest tests.test_gba_tiles -v
```

20 tests depuis la racine du dépôt : round-trip PNG, round-trip 4bpp GBA,
découpe/assemblage de tuiles, parsing `metatiles.bin`/`map.bin`, palette,
verrou du registre, **et les deux régressions primaire/secondaire décrites
plus haut**, construites sur de vraies structures de dossier synthétiques
pour ne pas dépendre d'assets du jeu dans le test lui-même.
