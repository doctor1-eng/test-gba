# Heart & Soul — ROM de test des maps Cinnabar

ROM de QA isolée, dédiée exclusivement aux 3 bâtiments ajoutés sur `CinnabarIsland_hns`
(Arène, Manoir, Labo). Ne remplace pas et ne modifie pas la ROM principale `pokehns.gba`.

## 1. Comment lancer la ROM

1. `make hns MAPTEST=1 -j$(nproc)` depuis la racine du dépôt (branche
   `claude/pokemon-heart-soul-audit-qv0f4n`).
2. Ouvrir le fichier produit `heart-and-soul-map-test.gba` dans un émulateur GBA (mGBA, VBA-M,
   etc.).
3. Démarrer une **nouvelle partie** (le mécanisme de test s'active à la création de la
   sauvegarde, dans `NewGameInitData`/`WarpToTruck`). Une sauvegarde existante d'une autre ROM
   ne doit pas être réutilisée ici — commencer une partie neuve dans cette ROM.

Aucune configuration supplémentaire n'est nécessaire : pas de sélection de personnage à faire
en dehors du strict minimum du moteur (nom du joueur), pas de dialogue d'intro à traverser.

## 2. Où commence le joueur

Directement en extérieur sur **CinnabarIsland_hns**, en `(30,17)` — la case piétonne juste au
sud de la porte du Manoir. Depuis ce point :

- **Manoir** : porte immédiatement au nord, en `(30,16)`.
- **Labo** : porte en `(23,21)`, à l'ouest-sud-ouest.
- **Arène** : porte en `(49,16)`, à l'est.
- Le Pokémon Center existant reste accessible (warp en `(45,29)`), mais **évitez d'y entrer**
  dans cette ROM de test (voir Limitations, section 6).

La position de spawn a été choisie par lecture directe des données (jamais devinée) : décodage
de `data/layouts/CinnabarIsland_hns/map.bin` (grille collision/élévation) puis vérification par
flood fill 4-directions que `(30,17)` est bien relié, via des tuiles de collision 0, aux trois
portes et au warp du Pokémon Center. Le détail de cette vérification est dans le commit
`feat(hs-map): build de test isole pour les portes Cinnabar`.

## 3. Maps à tester

- `CinnabarIsland_hns` (extérieur, 72×44, tileset `Kanto_General_Hns` / `Lavaridge_Hns`)
- `CinnabarIsland_Gym_Hns` (Arène — intérieur neuf, voir section 5quater)
- `CinnabarIsland_Mansion_Hns` (Manoir — intérieur neuf)
- `CinnabarIsland_PokemonLab_Hns` (Labo — intérieur neuf)

Les anciens intérieurs `_Frlg` (`CinnabarIsland_Gym_Frlg`, `PokemonMansion_1F_Frlg`,
`CinnabarIsland_PokemonLab_Entrance_Frlg`, etc.) ne sont **plus utilisés** par ces 3 portes —
voir section 5quater pour pourquoi.

## 4. Comment tester chaque bâtiment

Pour chacun des trois bâtiments :

1. Marcher jusqu'à la porte (coordonnées ci-dessus) et vérifier l'affichage de la façade
   (murs, toit, porte) et l'absence de superposition avec le décor existant.
2. Entrer : vérifier l'atterrissage correct à l'intérieur, sans blocage dans l'encadrement de
   la porte.
3. Vérifier les collisions intérieures (murs, sortie).
4. Ressortir par la même porte et vérifier l'atterrissage exact devant le bon bâtiment sur
   `CinnabarIsland_hns` — **pas** de téléportation vers un autre Kanto, pas de reboot.

Chaque intérieur est une seule pièce (pas d'étages/salles annexes pour l'instant — voir
section 5quater, "limite assumée").

## 5. Warps (nouveaux intérieurs, voir section 5quater pour le contexte)

| Bâtiment | Warp entrée (CinnabarIsland_hns → intérieur) | Warp sortie (intérieur → CinnabarIsland_hns) |
|---|---|---|
| Arène | `(49,16)` → `MAP_CINNABAR_ISLAND_GYM_HNS`, warp id 0 | `(4,8)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 2 |
| Manoir | `(30,16)` → `MAP_CINNABAR_ISLAND_MANSION_HNS`, warp id 0 | `(4,8)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 3 |
| Labo | `(23,21)` → `MAP_CINNABAR_ISLAND_POKEMON_LAB_HNS`, warp id 0 | `(4,8)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 4 |

Vérifié directement dans les fichiers compilés (`data/maps/*/events.inc`, générés par l'outil
`mapjson`) après build : les 3 `warp_def` de `CinnabarIsland_hns` ciblent bien ces 3 nouvelles
maps, et chacune d'elles a un unique warp de sortie vers `MAP_CINNABAR_ISLAND_HNS` avec le bon
`dest_warp_id` (2/3/4, correspondant à l'index de la porte dans le tableau de warps de
`CinnabarIsland_hns`).

## 5bis. Bug corrigé : blocage total des contrôles sur CinnabarIsland_hns

Découvert en testant cette ROM (déplacement et bouton START tous les deux sans effet dès
l'arrivée sur la carte, perçu comme un freeze). Cause réelle, confirmée par lecture du moteur :
`CinnabarIsland_hns_MapScripts` déclenche `HeartSoul_EventScript_CinnabarVerrouilleeCheck` à
chaque frame tant que `VAR_TEMP_0 == 0` (`MAP_SCRIPT_ON_FRAME_TABLE`). `TryRunOnFrameMapScript()`
(`src/script.c`) renvoie alors `TRUE`, et `ProcessPlayerFieldInput()`
(`src/field_control_avatar.c`) s'arrête avant de traiter le déplacement ou le bouton START pour
cette frame — même si le script appelé ne fait ensuite qu'un `end` immédiat. Le script ne
remettait jamais `VAR_TEMP_0` à une valeur non nulle : la condition restait donc vraie à chaque
frame, indéfiniment, tant que le joueur restait sur la carte. Motif déjà correctement utilisé
ailleurs dans le même fichier (`CeruleanCity_Gym_EventScript_TryMachinePart`, qui pose bien
`setvar VAR_TEMP_0, 1` sur chaque sortie) — c'est cet appel qui manquait ici.

Corrigé (`data/scripts/heart_and_soul_act1.inc`) en ajoutant `setvar VAR_TEMP_0, 1` sur les deux
sorties du script. `VAR_TEMP_0` est remis à 0 par le moteur à chaque nouveau chargement de carte
(`ClearTempFieldEventData`, `src/event_data.c`), donc ce `setvar` désarme seulement la
vérification jusqu'au prochain chargement, sans changer son résultat ni le verrou scénaristique
Acte I/V. Ce bug touchait déjà potentiellement le jeu normal (pas seulement cette ROM de test) :
tout séjour prolongé sur `CinnabarIsland_hns` avec ce script actif aurait figé les contrôles de
la même façon.

**Si votre ROM date d'avant ce correctif, retéléchargez-la et recommencez une nouvelle
partie** — une sauvegarde faite sur l'ancienne ROM figée reproduira le même blocage.

## 5ter. Bug corrigé : porte du Labo infranchissable

Signalé en testant cette ROM : la grotte du Labo `(23,21)` était visible mais impossible à
franchir, aucune réaction en marchant dessus. Cause confirmée par comparaison directe avec des
portes du même type déjà fonctionnelles ailleurs dans le jeu (`CeruleanCity_hns (7,10)` →
Grotte Céladopole, `VermilionCity_hns (61,10)` → Grotte Digda, même tileset primaire
`Kanto_General_Hns`) : la tuile avait le bon metatile et le bon comportement
(`MB_NON_ANIMATED_DOOR`), mais avec le bit de collision à **1** au lieu de **0** dans
`data/layouts/CinnabarIsland_hns/map.bin`. `MapGridGetCollisionAt()` (`src/fieldmap.c`) est
utilisé sans exception pour ce type de porte par `event_object_movement.c` : collision ≠ 0
bloque physiquement le pas, donc le joueur ne pouvait jamais se tenir sur la case et le warp
n'était jamais évalué.

Vérifié en même temps que l'**Arène** `(49,16)` et le **Manoir** `(30,16)` n'ont **pas** ce
problème : leur type de porte (`MB_ANIMATED_DOOR`, porte à deux battants) a une collision=1 qui
est cette fois normale (entrée gérée par une animation forcée qui contourne la collision
standard) — valeurs identiques bit pour bit à la porte d'Arène de `CeruleanCity_hns (34,31)`
qui fonctionne déjà. Aucune modification nécessaire de leur côté ; à confirmer quand même en
jeu.

Corrigé : une seule case modifiée dans `map.bin` (collision 1→0, élévation 0→3, alignée sur les
portes de grotte déjà fonctionnelles). Metatile et `warp_events` inchangés.

**Si votre ROM date d'avant ce correctif, retéléchargez-la** — la grotte du Labo restera
bloquée sur l'ancienne version.

## 5quater. Changement d'architecture : pourquoi les intérieurs `_Frlg` ont été abandonnés

Signalé en testant cette ROM : entrer dans l'Arène/le Manoir/le Labo faisait **redémarrer le
jeu** (sauf le Pokémon Center, sans rapport avec ce chantier — voir section 6). Cause identifiée
en lisant directement les fichiers compilés : le plan initial (réutiliser la géométrie des
intérieurs `_Frlg` existants via un warp externe depuis `CinnabarIsland_hns`) repose sur une
hypothèse fausse. `_Frlg` et `_Hns` ne sont pas deux contenus qui coexistent dans le même
binaire : dans `src/data/tilesets/headers.h`,

```c
#if !IS_FRLG && !IS_HNS
    // tilesets Emerald
#elif IS_FRLG
    // tilesets FRLG (gTileset_BuildingFrlg, gTileset_CinnabarGym, gTileset_PokemonMansion...)
#elif IS_HNS
    // tilesets HNS
#endif
```

Pour un build `POKEMON_HNS`, la branche `#elif IS_FRLG` est **totalement absente du binaire** —
pas seulement inerte au niveau du scénario, réellement non compilée. Confirmé en trois temps :
1. `data/layouts/layouts_table.inc` (généré) : le slot de `LAYOUT_CINNABAR_ISLAND_GYM` valait
   `.4byte NULL` — d'où le crash au chargement (déréférencement NULL).
2. Un essai de correctif « retag `game_version: frlg → hns` sur les 9 map.json/layouts.json
   concernés » a été tenté puis **annulé** : ça débloque bien les layouts, mais fait échouer le
   lien avec des dizaines de `undefined reference` (tilesets et scripts de ces intérieurs,
   toujours absents du binaire pour ce type de build).
3. Conclusion : impossible à corriger par un simple tag JSON. Remonter plus loin (modifier
   `headers.h` pour compiler ces tilesets FRLG aussi en HNS) était risqué et hors du périmètre
   décidé — la ROM est déjà à 94,5 % de ses 32 Mo, et c'est un fichier partagé par tout le jeu,
   pas seulement Cinnabar.

**Décision retenue** : construire 3 intérieurs neufs (`CinnabarIsland_Gym_Hns`,
`CinnabarIsland_Mansion_Hns`, `CinnabarIsland_PokemonLab_Hns`), avec des tilesets déjà compilés
côté HNS (`gTileset_Johto_Building_Hns` + `gTileset_House_Lab_Hns`), le même combo déjà utilisé
avec succès par plusieurs maisons existantes (`CeruleanCity_House1_hns`,
`VermilionCity_House1_hns`...). Chaque intérieur est une pièce simple (13×10, copiée depuis
`VermilionCity_House1_hns`, sans PNJ ni panneau ajoutés) avec une seule sortie, vérifiée après
compilation (section 5) — plus de dépendance à du contenu `_Frlg` non compilé pour ce build.

**Limite assumée** : les 3 intérieurs sont actuellement des **blockouts identiques** (même
pièce vide, même tileset) — pas de mobilier thématique (Blaine dans l'Arène, statues du Manoir,
machines du Labo), pas d'étages. Suffisant pour valider warps/collisions/transitions (l'objet
de cette ROM de test) ; une passe de contenu (mobilier, PNJ, plusieurs pièces) reste à faire
séparément, hors du périmètre de ce chantier.

## 6. Limitations connues

- **N'entrez pas dans le Pokémon Center** (`CinnabarIsland_hns` warp `(45,29)`) avec cette ROM
  de test. `CinnabarIsland_PokemonCenter_hns` déclenche, dès le premier affichage de la carte,
  le script d'intro de l'Acte I (`HeartSoul_EventScript_ChooseType` →
  `HeartSoul_EventScript_CinnabarAttack`) qui verrouille le contrôle du joueur et le renvoie
  scénaristiquement hors de Cinnabar. Ce n'est pas un bug de ce build : c'est le comportement
  normal du jeu, simplement hors du périmètre de cette ROM de test (qui ne teste que les 3
  nouveaux bâtiments). Un filet de sécurité (`FLAG_ACTE_5_DEBLOQUE` forcé) empêche que le
  verrou de retour bloque définitivement l'accès à l'île si l'attaque se déclenche quand même,
  mais la séquence scriptée elle-même n'est pas neutralisée.
- **Le Manoir réutilise la même silhouette extérieure que l'Arène** (même tampon de bâtiment 7×5
  du tileset `Kanto_General_Hns`) — limite déjà documentée et assumée dans
  `docs/map_design.md` section 5.4.2 : aucun tampon "grand bâtiment" entièrement portable
  n'a été trouvé ailleurs dans le jeu. Distinguer les deux bâtiments à l'œil se fera par leur
  position, pas par leur façade.
- **Le panneau existant devant l'Arène** (`CinnabarIsland_Text_GymSign`, texte hérité de
  FRLG : « L'ARENE de CINNABAR a déménagé aux SEAFOAM ISLANDS ») **contredit maintenant la
  porte fonctionnelle juste à côté**. C'est un texte préexistant, sans rapport avec ce chantier
  (modifier des dialogues est hors du périmètre de cette tâche) — signalé ici pour information,
  pas corrigé.
- **Aucune vérification visuelle n'a été faite dans cet environnement** (pas d'émulateur avec
  rendu graphique disponible côté agent). Toute la validation ci-dessus est **technique** :
  compilation (`make hns MAPTEST=1`, 0 erreur), lecture directe des `map.json` (warps), et
  décodage binaire de `map.bin` + désassemblage du code compilé (confirmation que
  `SetWarpDestination` est bien appelé avec `MAP_CINNABAR_ISLAND_HNS, (30,17)` dans le binaire
  final). La confirmation visuelle du rendu, des collisions ressenties en jeu et de la
  cohérence esthétique reste à faire par vous, dans un émulateur.

## 8. Équipe et inventaire de test complets (menu debug, aucun code modifié)

Le jeu embarque déjà un menu de debug complet (`include/config/debug.h`,
`DEBUG_OVERWORLD_MENU = TRUE` par défaut) : **maintenir R puis appuyer sur START en extérieur**
(pas dans un menu) l'ouvre. Il permet de créer des Pokémon niveau 100 avec IVs parfaites et de
remplir entièrement le sac — pas besoin de modifier le jeu pour ça, ni de recompiler la ROM.

### 8.1 Six Pokémon (meilleures stats totales réellement disponibles)

Les formes Méga/Primal/Gigamax/Téracristal sont désactivées dans ce fork
(`include/config/species_enabled.h`) — ce top 6 exclut donc ces formes et a été vérifié
directement dans `src/data/pokemon/species_info/*.h` (BST = somme des 6 stats de base) :

| # | Pokémon | ID espèce | BST | PV/Atq/Déf/AtqS/DéfS/Vit |
|---|---|---|---|---|
| 1 | Arceus | 493 | 720 | 120/120/120/120/120/120 |
| 2 | Zacian (Couronné) | 1227 | 700 | 92/150/115/80/115/148 |
| 3 | Zamazenta (Couronné) | 1228 | 700 | 92/120/140/80/140/128 |
| 4 | Eternatus | 890 | 690 | 140/85/95/145/95/130 |
| 5 | Dialga (Origine) | 1069 | 680 | 100/100/120/150/120/90 |
| 6 | Palkia (Origine) | 1070 | 680 | 90/100/100/150/120/120 |

**Pour les obtenir** : menu debug → `Give X…` → `Pokémon (Complex)` → entrer l'ID espèce →
niveau **100** → puis Shiny/Nature/Ability/Tera/Dynamax/Gigantamax (au choix) → **IVs : 31 sur
les 6 stats** (pour de vraies stats maximales) → EVs/Moves au choix. Répéter pour les 6 IDs.
Version rapide sans réglage IV/EV : `Pokémon (Basic)` (juste ID + niveau).

### 8.2 Tout l'inventaire d'un coup

Menu debug → `PC/Bag…` :

- `Fill Pocket TMHM` — toutes les CT/CS
- `Fill Pocket Items` — tous les objets
- `Fill Pocket Poké Balls` — toutes les Poké Balls
- `Fill Pocket Key Items` — tous les objets clés
- `Fill PC Items` — au cas où le sac déborde

Menu debug → `Give X…` → `Max Money` / `Max Coins` si besoin d'acheter en boutique.

## 8bis. Checklist de test (à cocher à chaque session)

**Avant de commencer**
- [ ] Nouvelle partie sur la dernière version de `heart-and-soul-map-test.gba`
- [ ] Équipe de test donnée (section 8.1) et sac rempli (section 8.2)
- [ ] Position de départ confirmée : `CinnabarIsland_hns (30,17)`, aucun blocage d'input

**Par bâtiment (Arène, Manoir, Labo) — répéter 3 fois**
- [ ] Façade visible et cohérente depuis l'extérieur (pas de tuiles manquantes/mal alignées)
- [ ] Pas de superposition avec le décor existant (arbres, PNJ, faune) autour de la porte
- [ ] Approche à pied depuis le point de spawn sans collision anormale
- [ ] Entrée par la porte : pas de blocage dans l'encadrement, atterrissage correct à l'intérieur,
      **pas de reboot/crash**
- [ ] Collisions intérieures : murs de la pièce infranchissables comme attendu
- [ ] Sortie par la porte : atterrissage exact devant le bon bâtiment sur `CinnabarIsland_hns`
- [ ] Pas de téléportation vers un autre Kanto ni vers un autre bâtiment en sortant

**Général sur CinnabarIsland_hns**
- [ ] Déplacement libre dans toutes les directions autour des 3 portes, aucun freeze
- [ ] Bouton START ouvre bien le menu (Pokémon/Sac/Sauvegarde/Options)
- [ ] Pas d'entrée accidentelle dans le Pokémon Center déclenchant l'intro Acte I (sauf test volontaire)
- [ ] Limites de la carte (bords) ne laissent pas sortir de la zone jouable

**À noter pour chaque anomalie trouvée**
- [ ] Bâtiment/zone concerné, coordonnées approximatives, capture d'écran si possible
- [ ] Reproductible ou ponctuel
- [ ] Bloquant (freeze/soft-lock) ou cosmétique

## 9. Ce qui n'a pas été touché

Aucune autre map, aucun autre script, aucun dialogue, aucun dresseur, aucun objet, aucun
Pokémon n'a été modifié pour ce chantier. Le mécanisme de test (`MAPTEST=1`,
`HNS_MAP_TEST_BUILD`) est entièrement conditionnel et n'a aucun effet sur le build normal
`make hns`.
