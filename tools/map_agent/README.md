# Map Agent — Heart & Soul

Génère les fichiers de données d'une map à partir d'une spec YAML
lisible, et les valide avant de te laisser les copier dans le repo.
Calé directement sur le vrai schéma du dépôt `doctor1-eng/test-gba`
(branche `claude/pokemon-heart-soul-audit-qv0f4n`) — `map.json`,
`layouts.json` et `scripts.inc` réels inspectés, pas seulement l'upstream
`pokeemerald-expansion`.

## Le garde-fou le plus important : tilesets FRLG vs HNS

`docs/heart_and_soul/technical_map.md` documente un bug déjà rencontré :
les 3 premiers bâtiments de Cinnabar (Gym/Manoir/Labo) ont d'abord réutilisé
des intérieurs `_Frlg` existants. Le `map.json` et les warps étaient
corrects, la compilation passait — mais **le jeu plantait (reboot
immédiat) à l'entrée**. Cause : `src/data/tilesets/headers.h` compile les
tilesets dans 3 branches `#if/#elif` **mutuellement exclusives** selon la
variante (générique / `IS_FRLG` / `IS_HNS`). Un tileset FRLG n'existe tout
simplement pas dans le binaire d'un build HNS.

**Le Map Agent bloque maintenant ce cas précis** : `generate` vérifie que
`primary_tileset`/`secondary_tileset` figurent bien dans la liste réelle
des 106 tilesets compilés pour HNS (extraite de `headers.h`), et refuse de
générer la map sinon — avec le message d'erreur qui explique pourquoi.
Il avertit aussi si un warp ou une connexion cible un nom de map contenant
`_Frlg`. C'est le seul type de bug de cette classe qu'un audit JSON pouvait
manquer ; c'est maintenant couvert.

## Installation

```bash
pip install pyyaml
```

## Utilisation

```bash
# 1. Générer une map à partir d'une spec
python3 map_agent.py generate ma_map.yaml --out ./output

# 2. Valider un map.json déjà existant dans le repo (avant un commit, en CI...)
python3 map_agent.py validate data/maps/CinnabarLab
```

`generate` refuse d'écrire les fichiers si la spec contient une erreur
bloquante (coordonnée hors carte, warp mal formé, `local_id` dupliqué...).
Les avertissements (ex: NPC et item sur la même case) n'empêchent pas la
génération mais sont listés dans `VALIDATION.md`.

## Ce qui est généré

Pour une map nommée `Name` :

```
output/Name/
├── map.json            → à copier dans data/maps/Name/map.json
├── scripts.inc          → squelette de scripts NPC, à compléter puis copier dans data/maps/Name/scripts.inc
├── layout_snippet.json  → entrée à fusionner dans data/layouts/layouts.json
├── VALIDATION.md        → rapport de validation
└── NEXT_STEPS.md        → checklist des étapes manuelles restantes
```

## Ce qui reste manuel (et pourquoi)

Le placement des tiles (`border.bin`, `map.bin`) n'est **pas généré**.
Peindre une map (terrain, bâtiments, collisions visuelles) est une tâche
de composition graphique dans Porymap — la déléguer à un script produirait
des maps vides ou géométriquement absurdes. Le Map Agent automatise la
partie qui EST purement structurée : les données de map.json, la
cohérence warps/NPC/items, et l'entrée de layout. C'est le goulot
d'étranglement réel du pipeline décrit dans ton document d'architecture ;
le réduire à "peindre le terrain dans Porymap, tout le reste est généré et
validé" fait déjà gagner l'essentiel du temps.

De même, `map_groups.json` et `maps.s` (registres globaux du projet) ne
sont pas modifiés automatiquement : une erreur là casse la compilation de
tout le projet, ça reste une étape à revue humaine (1 ligne à ajouter à
chaque fois, indiquée dans `NEXT_STEPS.md`).

## Format de la spec YAML

Voir `example_spec.yaml` pour un exemple complet. Champs principaux :

| Champ | Obligatoire | Description |
|---|---|---|
| `map.name` | oui | Nom PascalCase (ex: `CinnabarLab`), sert de préfixe partout |
| `map.layout.width/height` | oui | Dimensions en tiles |
| `map.layout.primary_tileset/secondary_tileset` | oui | Tilesets Porymap déjà existants |
| `map.region`, `map.region_map_section` | non | Constantes du jeu (défaut Kanto) |
| `map.npcs[]` | non | `local_id`, `graphics`, `x`, `y`, `movement_type`, `dialogue` |
| `map.items[]` | non | `x`, `y`, `item`, `flag` (le flag doit être unique dans tout le projet) |
| `map.warps[]` | non mais recommandé | `x`, `y`, `dest_map`, `dest_warp_id` |
| `map.requirements` | non | Compteurs attendus (`entrances`, `npcs`, `items`) — génère un avertissement si ça ne colle pas, utile pour respecter un brief de game design |

## Note sur Poryscript

Le dépôt contient des fichiers `.pory` (ex: `docs/heart_and_soul/scripts/
cinnabar_acte1.pory`), mais ce sont des **templates narratifs de conception**,
explicitement marqués "à adapter" — pas la source compilée. Les vraies maps
(`data/maps/*/scripts.inc`) utilisent le format assembleur direct
(`lock`/`msgbox`/`release`/`end`), pas Poryscript compilé. Le Map Agent
génère donc du `.inc`, cohérent avec ce qui est réellement utilisé.

## Prochaine itération possible

- **Rafraîchir `HNS_COMPILED_TILESETS`** périodiquement si de nouveaux
  tilesets sont ajoutés à `headers.h` (commande de refresh documentée en
  commentaire dans `map_agent.py`) — sinon un vrai nouveau tileset HNS
  déclencherait un faux positif.
- Ajout d'un mode `batch` pour générer plusieurs maps d'un coup depuis un
  dossier de specs (utile pour un acte entier).
- Vérification croisée entre deux maps : si `CinnabarLab` a un warp vers
  `CinnabarIsland`, vérifier qu'un warp retour existe bien côté
  `CinnabarIsland` (actuellement seulement signalé en avertissement
  manuel dans `NEXT_STEPS.md`).
- Étendre le même principe de garde-fou (compilation conditionnelle par
  `game_version`) aux graphics `OBJ_EVENT_GFX_*` si le même genre
  d'exclusion existe côté sprites — à vérifier dans le code source des
  objets événements avant de considérer que ce risque n'existe que pour
  les tilesets.
