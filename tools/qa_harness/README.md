# QA Harness — Test headless du jeu (sans émulateur graphique)

## Ce que c'est
Un petit programme C (`qa_runner.c`) qui utilise `libmgba` pour faire tourner la ROM en headless,
injecter des séquences de touches, et capturer des captures d'écran PNG. Ça permet de **jouer au jeu et
vérifier visuellement le contenu sans interface graphique** — exactement ce qui manquait pour attraper
des bugs comme celui de l'élévation Team Rocket avant de livrer une ROM à Thomas.

## Statut : calibrage partiel réalisé (Session 3)
Timings confirmés par capture d'écran :
- **~1800 frames** après le reset : écran titre atteint (animation Latios/Latias + vélo)
- L'entrée **Start est bien prise en compte** (transition vers le logo "Pokémon" blanc flashé) mais semble redéclencher une animation de transition à chaque pression plutôt que d'aller direct au menu Nouvelle Partie/Continuer — sécable en plusieurs pressions espacées (10 frames ON / 5 frames OFF a fonctionné pour faire réagir le jeu)
- Reste à calibrer : la séquence exacte Start → sélection New Game → confirmation, qui n'a pas encore été atteinte de façon fiable dans nos tests

## Blocage identifié (Session 3, suite) : les entrées clavier ne semblent pas prises en compte
Tests supplémentaires : plusieurs séquences de pression Start (courtes, longues, répétées) ramènent
systématiquement à un état identique (logo "Pokémon" en flash ↔ écran titre "PRESS START"). Hypothèse la
plus probable : ce flash fait partie de l'animation d'ambiance du titre elle-même (cycle automatique),
indépendante de nos pressions — c'est-à-dire que **nos appuis sur Start ne sont probablement pas pris en
compte du tout**, malgré une implémentation a priori conforme à l'API (`setKeys` avec le bitmask standard
GBA_KEY_*). Cause racine non identifiée : possible qu'un driver d'entrée/`keySource` doive être enregistré
explicitement auprès du core au lieu du simple `setKeys` direct.

**Ne pas re-essayer à l'aveugle la prochaine fois** — commencer par vérifier `core->getKeys()` juste après
`setKeys()` pour confirmer si l'état est bien mémorisé côté core avant de creuser plus loin.

## Compilation
```
cd tools/qa_harness
gcc -o qa_runner qa_runner.c -I/usr/include -L/usr/lib/x86_64-linux-gnu -lmgba -lpng
```
Dépendances système : `libmgba-dev`, `mgba-sdl` (via `apt-get install`).

## Utilisation
```
LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu ./qa_runner <rom.gba> <script.txt> <dossier_sortie>
```

## Format du script
Fichier texte, une instruction par ligne :
- `<frames> <keymask>` : maintenir les touches `keymask` pendant `frames` frames
- `SHOT nom.png` : capturer l'écran actuel

Table des bits de touches (GBA) :
```
A=1  B=2  SELECT=4  START=8  RIGHT=16  LEFT=32  UP=64  DOWN=128  R=256  L=512
```

## Prochaine étape
Calibrer un script de référence "Arc 1 complet" (boot → nouvelle partie → nom → labo → Pikachu →
Route 1 → combat James) pour en faire un test de non-régression automatique à chaque session, à lancer
avant de livrer une ROM à Thomas.
