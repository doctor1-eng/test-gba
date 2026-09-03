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

## Pipeline structuré de génération de cartes (suite 34)

Refonte demandée après retour de Thomas sur le premier rendu de Bourg Palette (§ci-dessus, "suite 33") :
le résultat ressemblait à des sprites posés au hasard plutôt qu'à une vraie carte Pokémon construite
par un level designer. Cette section documente le diagnostic et le système qui remplace l'approche
précédente — à réutiliser telle quelle pour les 13 autres villes.

### Diagnostic : pourquoi l'ancienne approche produisait un résultat incohérent

Trois causes racines identifiées dans le code de génération de "suite 33" :

1. **Aucun bit de collision jamais posé.** Seule l'élévation (`3<<12`) était écrite dans `map.bin` ;
   le bit de collision (`MAPGRID_COLLISION_MASK`, `0x0C00`) restait à 0 partout. Or les tuiles de
   mur/toit/arbre/colline du tileset Kanto portent toutes le comportement `MB_FRLG_NORMAL` (aucun
   blocage intrinsèque) — sans le bit de collision explicite, un joueur traversait murs, arbres et
   collines sans obstacle. Ce bug était invisible en preview PNG statique (qui ne rend que les
   tuiles, pas la collision) et ne se voyait qu'en jouant.
2. **Pose de tuiles ad hoc, sans notion de "bâtiment".** Chaque maison était composée à la main,
   ligne par ligne, sans structure réutilisable garantissant qu'un toit correspondait bien au mur
   du dessous, qu'une porte existait et étai vraiment accessible, ou qu'aucune construction ne
   débordait de la carte. Rien n'empêchait techniquement une porte "orpheline" (non desservie par un
   chemin) ou un bâtiment partiellement recouvert par la végétation posée après coup.
3. **Aucune vérification automatisée.** Le seul contrôle qualité était une relecture visuelle de la
   preview PNG — suffisant pour repérer une tuile d'eau qui strie, pas pour garantir que les 7-8
   bâtiments d'une ville sont tous accessibles et cohérents entre eux.

### Architecture de correction (implémentée, `tools/kanto_tileset_port/`)

Trois modules remplacent le script ad hoc unique de suite 33 :

- **`tile_catalog.py`** — catalogue de metatiles vérifiés individuellement (jamais par comptage de
  colonnes sur une planche, cf. §3e). Regroupe le terrain (`TERRAIN`), les portes walkable
  confirmées (`DOORS`), et quatre **archétypes de bâtiment** prêts à l'emploi : `PC_STYLE` (toit
  bleu + dortoir à emblème Poké Ball), `MART_STYLE` (toit rouge brique, porte à emblème rouge),
  `GYM_STYLE` (brique claire, tuiles `337`/`320` explicitement exclues — texte "GYM" incrusté et
  comportement de warp incrusté, inadaptés à un mur générique), `LAB_STYLE` (façade brique/fenêtres
  bleues arquées, 7 cases de large). Chaque archétype définit ses rangées toit/mur/porte comme des
  listes de tile IDs, avec `"DOOR"` comme placeholder résolu au moment de la construction.
- **`map_builder.py`** — classe `MapGrid` qui remplace la simple grille de tuiles par **deux grilles
  parallèles** (`self.tiles` et `self.collision`), corrigeant la cause racine n°1 : chaque primitive
  de dessin (`rect`, `hline`, `vline`, `blob`, `thin_line`) accepte un paramètre `collision` optionnel,
  et les fonctions `build_pc_style`/`build_mart_style`/`build_gym_style`/`build_lab` peignent le
  bâtiment complet (toutes rangées toit+mur marquées `IMPASSABLE`), calculent la position de la
  porte automatiquement, la remarquent explicitement `PASSABLE`, puis **enregistrent le bâtiment
  dans `self.buildings`** pour le validateur. `thin_line()` utilise un vrai algorithme de Bresenham
  (un seul tampon par point de ligne) — l'ancienne fonction de rivière empilait un rectangle à
  chaque micro-pas d'interpolation, produisant une bande d'eau bien plus large que prévu.
- **`MapGrid.validate()`** — le validateur automatisé demandé : pour chaque bâtiment enregistré,
  vérifie qu'au moins une case adjacente à la porte est un chemin/sable (`errors.append(...)` sinon)
  ET que la porte elle-même est bien marquée `PASSABLE`. `ensure_door_path()` fournit la correction
  automatique (pose un chemin devant une porte isolée) si le premier passage échoue. Le script de
  génération d'une ville (`build_littleroot.py`) refuse d'écrire `map.bin` (`sys.exit(1)`) si des
  erreurs persistent après correction automatique — aucune carte avec porte orpheline ne peut être
  livrée.

### Ordre du pipeline (`build_littleroot.py`, à dupliquer par ville)

Ordre strict, chaque étape ne redessinant jamais ce qu'une étape précédente a posé sans raison :
1. Relief (collines) → 2. Eau (rivière + mer + berges) → 3. Routes principales (AVANT les bâtiments,
qui viennent ensuite s'y raccorder, jamais l'inverse) → 4. Bâtiments (archétypes, empreinte + porte
complètes) → 5. Segments de raccordement porte→route explicites (`connect_door_to_path`, un appel par
bâtiment, pas de raccordement laissé au hasard) → 6. Végétation (groupée en amas via `tree_blob`/`blob`,
jamais tuile-par-tuile aléatoire) → 7. Validation (§ci-dessus) → 8. Écriture `map.bin`/`border.bin`.

**Piège découvert et corrigé pendant la construction de Bourg Palette** : la végétation posée à
l'étape 6 peut recouvrir un bâtiment de l'étape 4 si les coordonnées ne sont pas vérifiées à la main
(un amas de sapins centré près d'un mur de bâtiment corrompt visuellement sa façade). Le validateur
actuel ne détecte QUE les portes orphelines, pas les collisions décor/bâtiment — en pratique, on
vérifie ce cas à l'œil sur la preview PNG avant de considérer une carte prête, et on choisit les
centres d'amas de végétation à distance de sécurité (rayon + 1) de toute empreinte de bâtiment.

**Piège découvert et corrigé sur les PNJ à position codée en dur** : un `object_event` (PNJ) peut
avoir sa position réécrite au runtime par un script (`setobjectxyperm` dans `scripts.inc`, indépendant
de la position de départ dans `map.json`) — sur Bourg Palette, `LittlerootTown_EventScript_
SetTwinGuardingRoutePos` plaçait la Jumelle à (17,2), qui tombait dans l'empreinte du premier
emplacement du bâtiment "mystery" (gym-style, x14-17). Corrigé en déplaçant le bâtiment (x=10 au lieu
de x=14) plutôt que le script de jeu — plus sûr que de toucher une position codée en dur liée à
l'état de progression. **Avant de placer un bâtiment, grep `setobjectxyperm` dans le `scripts.inc` de
la carte pour repérer toute position de PNJ non visible dans `map.json`.**

### Reproduire pour une nouvelle ville

1. Dupliquer `build_littleroot.py` en `build_<ville>.py`, ajuster `W`/`H` et les points de départ
   (relief/rivière/routes) au plan de la ville.
2. Choisir un archétype par bâtiment (varier PC/Mart/Gym/Lab pour éviter que toutes les maisons se
   ressemblent) et ses coordonnées ; laisser `build_*_style()` calculer la porte.
3. Grep `setobjectxyperm` dans `scripts.inc` de la carte cible AVANT de fixer les coordonnées des
   bâtiments (cf. piège Twin ci-dessus).
4. Appeler `connect_door_to_path()` une fois par bâtiment.
5. Lancer le script : si `[!] erreur(s) de validation` persiste après correction automatique, le
   script s'arrête (`exit(1)`) — ajuster les coordonnées plutôt que d'ignorer l'erreur.
6. Mettre à jour `warp_events`/`bg_events` (signs) dans `map.json` avec les coordonnées de porte
   réellement produites (imprimées par le script, aussi dumpées en JSON) — ne jamais deviner ces
   coordonnées à l'avance.
7. Rebuild, `tools/map_preview/map_preview.py` pour la preview PNG, vérifier à l'œil qu'aucun amas de
   végétation ne recouvre une façade, test headless collision (marcher dans un mur ne doit pas
   bouger le joueur) avant de livrer.
