# TECHNICAL_ARCHITECTURE.md

## Moteur retenu
`rh-hideout/pokeemerald-expansion` (fork enrichi de `pret/pokeemerald`, maintenu activement par la
communauté ROM Hacking Hideout), compilé en mode `MODERN=1` (toolchain `gcc-arm-none-eabi`).

**Changement de base depuis la session 1** : on utilisait initialement `pret/pokeemerald` vanilla. Après
analyse des 4 ROMs de référence fournies par Thomas (Unbound, Sword/Shield Ultimate Plus, Heart and Soul,
Squirrels), constat que les hacks de référence "qualité pro" (notamment Unbound, unanimement salué) reposent
sur des moteurs enrichis (CFRU pour Unbound, non compatible decomp — équivalent decomp = pokeemerald-expansion).
Migration effectuée et validée par un 2e build complet (ROM 32 Mo, 79 % d'occupation).

**Licence** : usage libre, attribution "RHH (Rom Hacking Hideout)" requise dans les crédits du jeu final.
Pas de restriction commerciale bloquante (contrairement à CFRU, qui interdit toute monétisation) — sujet
sans impact pour un projet non-commercial mais à garder en tête si le projet évoluait.

## Mécaniques directement disponibles (config activable/désactivable dans `include/config/`)
Cela répond à l'objectif "impression d'un vrai jeu, pas juste Émeraude reskinné" du cahier des charges :
- Type Fée, split Physique/Spécial/Statut (mécaniques post-Gen3, désactivables si on veut rester fidèle Gen1-2)
- Objets, capacités et attaques jusqu'à Écarlate/Violet (à utiliser avec parcimonie — rester crédible dans le cadre "saison 1 / Kanto")
- TM réutilisables, EXP Share moderne configurable, HM sans obligation de les enseigner
- Pokédex façon HeartGold/SoulSilver (interface, tri, recherche)
- Structures dresseurs personnalisables (nature, EVs/IVs, objet tenu, etc. par dresseur — utile pour Team Rocket, champions, Régis)
- Compatible Porymap (versions récentes) pour l'édition de maps si Thomas l'utilise en local

## Toolchain validée dans cet environnement
```
apt-get install build-essential binutils-arm-none-eabi libpng-dev gcc-arm-none-eabi
cd engine && make MODERN=1 -j$(nproc)
```
Sortie : `engine/pokeemerald_modern.gba`. Build testé et fonctionnel (2 compilations réussies à ce jour).

## Encodage des caractères français
Bonne nouvelle confirmée par audit : `engine/charmap.txt` contient déjà nativement
à, â, ç, è, é, ê, î, ï, ô, œ, ù, û ainsi que les majuscules À, Ç, È, É.
→ **La limitation de police rencontrée sur le projet FireRed Rocket Edition (patch binaire) ne s'applique pas ici.**
Seule limite identifiée à ce jour : les guillemets français « » sont absents (seuls “ ” existent) → utiliser “ ” dans tous les dialogues.

## Structure du projet
```
kanto_saison1_project/
  docs/                  ← documentation vivante (ce dossier)
  engine/                ← fork de pret/pokeemerald (code source du jeu)
    data/text/           ← dialogues (fichiers .inc, éditables directement)
    data/scripts/        ← logique d'événements
    data/maps/<NomMap>/  ← scripts, connexions, événements par map
    src/data/region_map/ ← noms de villes affichés à l'écran (JSON)
    graphics/, sound/    ← assets (sprites, tiles, musique)
  tools/                 ← scripts Python maison (rendu de preview de maps, etc.)
```

## Répartition des rôles
- **Claude** : scénario, dialogues FR, données (dresseurs/Pokémon/objets/flags), scripts d'événements,
  structure des maps (fichiers), compilation, QA automatisée, documentation.
- **Thomas** : validation visuelle des maps (via Porymap en local si souhaité) et playtest réel sous émulateur —
  je n'ai ni interface graphique ni émulateur dans ce sandbox.

## Ce qui n'est PAS récupéré du projet TrashMan
Le hack TrashMan reste un patch binaire scellé sur une ROM figée. On repart d'Émeraude vanilla en tant que
moteur source. TrashMan peut servir de référence de contenu/ton si besoin, jamais de base de code.

## Limites connues
- Pas de rendu visuel en direct → prévoir un script de preview PNG (composition des tuiles) avant validation finale d'une map.
- Pas d'émulateur → validation = compilation propre + tests automatisés du framework pokeemerald (`make check` — à explorer en Phase 1).
- Réseau sortant limité à GitHub / PyPI / npm / dépôts Ubuntu — pas d'accès aux forums/outils communautaires hors GitHub.

## Tilesets Kanto FRLG dans un hack Emerald (suite 32)

Ce hack est compilé avec `MAP_VERSION=emerald` (target `make MODERN=1` utilisée partout dans ce
projet). Le dépôt pokeemerald-expansion contient aussi, en données dormantes, l'intégralité des
tilesets et cartes FireRed/LeafGreen d'origine (dossiers `*_frlg`) — mais ils ne sont **pas**
compilés dans ce build par défaut, pour deux raisons indépendantes, toutes deux contournables sans
toucher au moteur partagé au-delà d'un seul patch mineur et bien isolé (`tools/mapjson/mapjson.cpp`).
Méthode validée une fois (suite 32, `pallet_town_frlg` + `general_frlg` → carte de test isolée
`LittlerootTownKantoTest`, groupe 75 index 0, warp via le menu debug) — à répéter à l'identique
pour chacun des 13 autres tilesets de villes Kanto avant de les utiliser sur une vraie carte.

### 1. Pourquoi ces tilesets ne sont pas compilés

**a) Garde de préprocesseur `#if !IS_FRLG` / `#if IS_FRLG`.** `IS_FRLG` vaut `0` pour ce build
(`include/constants/global.h:67-78`, aucun `FIRERED`/`LEAFGREEN` défini). Or dans
`engine/src/data/tilesets/headers.h`, `engine/src/data/tilesets/graphics.h` et
`engine/src/data/tilesets/metatiles.h`, **tout** le contenu FRLG (structs `Tileset`, tuiles,
palettes, metatiles, attributs de metatile) vit dans la branche `#else` (FRLG uniquement) d'un
garde `#if !IS_FRLG ... #else ... #endif` — donc rien de tout ça n'est réellement lié dans le ROM
Emerald, même si le code source existe et compile pour une target FRLG.

**b) Filtrage par `layout_version` dans le compilateur de maps.** `tools/mapjson/mapjson.cpp`
(`generate_layout_headers_text`, ligne ~783) exclut de la compilation tout layout dont
`"layout_version"` ne correspond pas à `MAP_VERSION` (`emerald` ici) — donc même une carte FRLG
existante comme `PewterCity_Frlg` (dossier bien présent dans `data/maps/`) n'est **pas** une carte
active du hack : c'est une donnée source dormante, jamais atteignable en jeu.

Le champ `isFrlg` d'un `MapLayout` compilé (`struct MapLayout` dans `include/global.fieldmap.h`)
dépend directement de `layout_version == "frlg"` (même fichier mapjson.cpp, ligne ~801) — et ce
flag gouverne à l'exécution :
- la frontière tuiles/metatiles/palettes primaire↔secondaire (`GetNumTilesInPrimary`/
  `GetNumMetatilesInPrimary`/`GetNumPalsInPrimary`, `src/fieldmap.c` ~ligne 412-422) :
  **512 tuiles / 512 metatiles / 6 palettes en Emerald, contre 640/640/7 en FRLG** —
  une carte Kanto FRLG utilise réellement ces 640/640/7 (verifié visuellement : les tuiles
  d'eau/jetée de `general_frlg` situées entre les IDs 512 et 639 sont perdues si on force la
  frontière Emerald)
- le format des attributs de metatile (`ExtractMetatileAttribute`/`GetAttributeByMetatileIdAndMapLayoutFrlg`,
  `src/fieldmap.c` ~ligne 469-500) : **4 octets/metatile en FRLG** (Behavior 9 bits + TerrainType 5
  bits + EncounterType 3 bits + LayerType 2 bits) **contre 2 octets/metatile en Emerald**
  (Behavior 8 bits + LayerType 4 bits, pas de TerrainType/EncounterType)

Point positif vérifié : le champ `struct Tileset.metatileAttributes` est un simple `const u16 *`
(`include/global.fieldmap.h:107`) — le format des octets qu'il pointe n'est interprété que via
`isFrlg`, donc rien à changer côté struct elle-même.

### 2. Les comportements de metatile (`Behavior`) ne sont PAS directement compatibles

**Ce n'est pas un simple copier-coller.** Les octets bruts de `metatile_attributes.bin` d'un
tileset FRLG encodent le champ Behavior selon la numérotation **propre à FireRed/LeafGreen**
(`include/constants/metatile_behaviors_frlg.h`, constantes `MB_FRLG_*`), qui est **différente** de
la numérotation unifiée `MB_*` utilisée par ce moteur (`include/constants/metatile_behaviors.h`,
enum unique 0-240 partagé par toutes les cartes actives du hack, Emerald comme Kanto). Exemple
concret rencontré : l'octet brut `0x69` décode en `MB_LAVARIDGE_GYM_1F_WARP` (une référence Hoenn)
si on le lit à tort avec la table unifiée au lieu de la table FRLG — alors qu'il s'agit en réalité
de `MB_FRLG_WARP_DOOR` (une porte de bâtiment ordinaire), une fois correctement traduit.

**Bonne nouvelle vérifiée empiriquement sur `pallet_town_frlg`/`general_frlg` (suite 32)** :
- Tous les comportements réellement utilisés ont un équivalent fonctionnel direct dans la table
  unifiée : `MB_FRLG_TALL_GRASS → MB_TALL_GRASS`, `MB_FRLG_POND_WATER → MB_POND_WATER`,
  `MB_FRLG_OCEAN_WATER → MB_OCEAN_WATER`, `MB_FRLG_DEEP_WATER → MB_DEEP_WATER`, etc. — testé en
  jeu réel (headless) : le joueur est bloqué net à la lisière de l'eau, comportement identique à
  n'importe quelle eau Hoenn du hack.
- Aucune valeur Behavior FRLG rencontrée ne dépasse 8 bits (255) → aucune perte lors du passage au
  champ 8 bits Emerald.
- Le `LayerType` (Normal/Covered/Split, `include/global.fieldmap.h:54-56`) est un enum partagé,
  identique des deux côtés — aucune conversion nécessaire, copie directe.
- Seuls le `TerrainType` et l'`EncounterType` (spécifiques au format FRLG 32 bits) n'ont pas
  d'équivalent dans le format Emerald 16 bits — perte assumée, cohérente avec le fait qu'aucune
  autre carte Emerald de ce hack ne les utilise non plus (le format Emerald n'a tout simplement pas
  la place pour ces deux champs).
- **La table officielle de correspondance existe déjà** dans le dépôt :
  `engine/migration_scripts/frlg_metatile_behavior_converter.py` (fournie par
  pokeemerald-expansion, dictionnaires `FRLG_BEHAVIORS`/`EMERALD_BEHAVIORS`/`FRLG_TO_EMERALD`) —
  ne pas la réinventer, la réutiliser telle quelle (voir §4 ci-dessous).
- Quelques valeurs Behavior brutes n'existent dans AUCUNE des deux tables (ex. `0x1D`, `0x1E`,
  `0x1F`, `0x2C`, `0x4F` sur `general_frlg`/`pallet_town_frlg`) : ce sont des trous de numérotation
  FRLG jamais documentés. Vérifié visuellement (panneaux MART/CENTRE POKéMON, jetées/ponts en bois
  sur l'eau) : aucun ne correspond à un mécanisme de jeu critique → convertis en `MB_NORMAL` par
  défaut plutôt que de garder la valeur brute inchangée (qui retomberait sur une entrée arbitraire
  et sans rapport de la table unifiée).

### 3. Méthode de duplication (validée sur `pallet_town_frlg`)

**a) Dupliquer les données graphiques hors du garde `#if IS_FRLG`, sans copier les fichiers
source.** Trois nouveaux fichiers, inclus sans condition depuis `src/tilesets.c` (après les
`#include` existants) :
- `src/data/tilesets/graphics_kanto.h` : `INCGFX_U32`/`INCGFX_U16` pour les tuiles et les 16
  palettes de chaque tileset, sous de **nouveaux noms de symboles** (suffixe `Kanto`, ex.
  `gTilesetTiles_GeneralFrlgKanto`) mais pointant vers les **mêmes fichiers source**
  (`data/tilesets/primary/general_frlg/tiles.png` etc.) — aucune duplication de données binaires,
  juste de nouvelles déclarations C compilées sans condition.
- `src/data/tilesets/metatiles_kanto.h` : `INCBIN_U16` pour `metatiles.bin` (identique à l'original,
  aucune conversion nécessaire) et pour `metatile_attributes_emerald_behaviors.bin` (voir §b).
- `src/data/tilesets/headers_kanto.h` : les nouveaux structs `Tileset` eux-mêmes. Pour un tileset
  primaire avec animation (ex. `general_frlg` a une animation d'eau/fleurs), le callback
  `InitTilesetAnim_General_Frlg` (`src/tileset_anims.c`) est **déjà compilé sans condition** dans ce
  fichier (aucune fonction de `tileset_anims.c` n'est gardée par `#if IS_FRLG`) — donc réutilisable
  tel quel, pas besoin de le dupliquer.

**b) Convertir la numérotation des comportements de metatile.**
`tools/kanto_tileset_port/convert_frlg_behaviors.py` lit un `metatile_attributes.bin` FRLG (32
bits/metatile), ne touche QUE le champ Behavior (reste du mot inchangé — TerrainType/EncounterType/
LayerType), et réencode sa valeur de `MB_FRLG_*` vers `MB_*` via les tables officielles de
`engine/migration_scripts/frlg_metatile_behavior_converter.py`, en tombant sur `MB_NORMAL` pour les
valeurs non documentées (voir §2). Écrit un nouveau fichier `..._emerald_behaviors.bin` à côté de
l'original (celui-ci n'est jamais modifié — reste disponible tel quel pour un usage FRLG réel
futur). **Le format reste le format FRLG 32 bits** — voir §c, on ne repasse PAS au format Emerald
16 bits, ça reviendrait à perdre la frontière 640/640/7 (§1b).

**c) Patch minimal de `tools/mapjson/mapjson.cpp` : découpler `isFrlg` du filtre de compilation.**
Sans ce patch, on est bloqué : un layout `"layout_version": "frlg"` est le seul moyen d'obtenir
`isFrlg=TRUE` (frontière 640/640/7 + lecture 32 bits des attributs) — mais c'est aussi précisément
ce qui le fait exclure de la compilation quand `MAP_VERSION=emerald`. Un layout `"emerald"` (donc
compilé) est lui verrouillé sur `isFrlg=FALSE`. Nouveau champ optionnel `"metatile_format": "frlg"`
sur un layout `"layout_version": "emerald"` : le layout passe le filtre de compilation normalement,
mais `isFrlg=TRUE` est quand même émis dans le struct compilé, avec `border_width`/`border_height`
lus depuis le JSON comme pour un vrai layout FRLG. Changement additif et rétrocompatible : aucun
layout existant n'est affecté tant qu'il n'ajoute pas ce champ. Le outil `mapjson` est recompilé
automatiquement par `make MODERN=1` (règle Makefile standard, mtime du `.cpp`).

**d) Déclarer le layout.** Dans `data/layouts/layouts.json` :
```json
{
  "id": "LAYOUT_...",
  "primary_tileset": "gTileset_GeneralFrlgKanto",
  "secondary_tileset": "gTileset_PalletTownKanto",
  "border_width": 2,
  "border_height": 2,
  "layout_version": "emerald",
  "metatile_format": "frlg"
}
```

**e) IDs de metatile : bien vérifier chaque tuile individuellement avant de composer une carte.**
Piège rencontré pendant ce test : une identification visuelle "à l'œil" sur une planche de tuiles
compressée peut décaler l'index d'une ligne entière (16 tuiles) sans que ce soit évident au premier
coup d'œil — la métatuile d'herbe attendue à l'ID 0 était en réalité une métatuile vide/non définie
(rendue en noir), la vraie herbe étant à l'ID 1. Méthode fiable utilisée pour corriger : appeler
directement `MapRenderer.render_metatile_image()` de `tools/map_preview/map_preview.py` pour
chaque ID candidat un par un, avec le numéro incrusté sur l'image par le code Python lui-même
(jamais en comptant des colonnes à la main sur une planche) — élimine toute erreur d'indexation.

### 4. Reproduire pour les 13 autres tilesets de villes

1. Repérer le tileset primaire compagnon exact dans `src/data/tilesets/headers.h` (le plus souvent
   `gTileset_General_Frlg`/`gTileset_GeneralFrlgKanto`, déjà dupliqué — sauf exceptions comme les
   gymnases qui utilisent `gTileset_BuildingFrlg`, à dupliquer une seule fois lui aussi le cas
   échéant)
2. `python3 tools/kanto_tileset_port/convert_frlg_behaviors.py <secondary>/metatile_attributes.bin <secondary>/metatile_attributes_emerald_behaviors.bin`
   — vérifier le nombre de "valeurs de behavior inconnues" affiché ; si non nul, refaire la
   vérification visuelle du §2 (les tuiles concernées, pas juste supposer que `MB_NORMAL` convient)
3. Ajouter les nouvelles déclarations dans `graphics_kanto.h`/`metatiles_kanto.h`/`headers_kanto.h`
   (suivre exactement le patron `PalletTown`/`GeneralFrlgKanto` déjà en place)
4. Ajouter le layout dans `layouts.json` (`metatile_format: "frlg"`, bonnes dimensions/bordure)
5. Recompiler, `tools/map_preview/map_preview.py --tileset <primaire> <secondaire> frlg` pour
   prévisualiser la planche complète du tileset avant de composer quoi que ce soit
6. Composer la carte réelle metatile par metatile (§3e), rebuild, preview PNG de la carte complète,
   test headless (marche, collision eau, `grep -c "Bad memory"` = 0) avant de considérer la ville
   comme prête
