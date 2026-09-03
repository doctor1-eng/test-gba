# map_preview.py

Generateur de previews PNG fideles des maps, a partir des vraies tuiles source
(`engine/data/tilesets/`) — sans passer par un editeur graphique.

## Pourquoi ce script existe

Porymap (github.com/huderlem/porymap) a ete evalue pour ce role. Verdict : **installable
mais inutilisable ici**. Qt6 et un serveur X headless (Xvfb) sont bien disponibles dans
cet environnement, mais Porymap est un editeur purement interactif — son `main.cpp` ne
fait que transmettre `argv` a `QApplication` sans aucun traitement d'arguments : pas de
mode CLI, pas de mode batch, pas d'export scriptable. Meme installe et lance sous Xvfb, il
faudrait piloter la souris/le clavier a l'aveugle via des captures d'ecran repetees pour la
moindre action — impraticable pour un vrai travail de cartographie sans un humain devant
l'ecran. D'ou ce script Python comme alternative directement scriptable.

## Usage

```
python3 tools/map_preview/map_preview.py <NomDeMap> [<NomDeMap2> ...]
python3 tools/map_preview/map_preview.py --all
python3 tools/map_preview/map_preview.py --list
python3 tools/map_preview/map_preview.py --tileset <TilesetPrimaire> <TilesetSecondaire> [emerald|frlg]
```

Le mode `--tileset` affiche en planche toutes les metatiles d'une paire de tilesets, y
compris des tilesets FRLG dormants (ex: `pallet_town_frlg`) qu'aucune carte n'utilise
encore — utile pour previsualiser un tileset avant de batir une carte avec.

Les PNG sont ecrits dans `tools/map_preview/out/`.

## Format lu

- `data/layouts/layouts.json` : dimensions, tileset primaire/secondaire, `layout_version`
  de chaque layout
- `data/maps/<Map>/map.json` : quel layout une carte utilise
- `data/tilesets/{primary,secondary}/<nom>/tiles.png` : planche de tuiles 8x8 indexees
- `data/tilesets/{primary,secondary}/<nom>/metatiles.bin` : 8 tuiles (2 couches de 2x2)
  par metatile, u16 = tileId(10 bits) | xflip | yflip | palette(4 bits)
- `data/tilesets/{primary,secondary}/<nom>/palettes/NN.pal` : palettes JASC-PAL (16
  couleurs), NN etant l'indice global de palette (0-5 = primaire, 6-12 = secondaire pour
  un layout "emerald" ; 0-6 = primaire, 7-12 = secondaire pour un layout "frlg" — voir
  `NUM_PALS_IN_PRIMARY[_FRLG]` dans `engine/include/fieldmap.h`)
- `data/layouts/<Map>/map.bin` : grille de la carte, u16 = metatileId(10 bits) |
  collision(2 bits) | elevation(4 bits)

Les attributs de metatile (`metatile_attributes.bin`) ne sont pas lus : sans sprite
joueur a superposer, une preview statique n'a pas besoin de LayerType pour l'affichage
(couche du bas puis couche du haut, dans cet ordre, suffit dans tous les cas).

## Limites connues

- Pas de rendu des animations de tuiles (eau, drapeaux...) — image statique uniquement
- Pas de rendu des sprites d'object_events (PNJ, objets) ni des bg_events (panneaux) —
  seul le terrain (metatiles) est affiche
- Pas de rendu de la bordure (`border.bin`) au-dela des limites de la carte
