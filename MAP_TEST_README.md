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
- `CinnabarIsland_Gym_Frlg` (Arène)
- `PokemonMansion_1F_Frlg` (+ 2F/3F/B1F si vous poussez plus loin)
- `CinnabarIsland_PokemonLab_Entrance_Frlg` (+ Lounge/Research Room/Experiment Room)

## 4. Comment tester chaque bâtiment

Pour chacun des trois bâtiments :

1. Marcher jusqu'à la porte (coordonnées ci-dessus) et vérifier l'affichage de la façade
   (murs, toit, porte) et l'absence de superposition avec le décor existant.
2. Entrer : vérifier l'atterrissage correct à l'intérieur, sans blocage dans l'encadrement de
   la porte.
3. Vérifier les collisions intérieures (murs, meubles, PNJ) et les éventuels étages/salles
   (Manoir : 2F/3F/B1F ; Labo : Lounge/Research Room/Experiment Room).
4. Ressortir par la même porte (ou une sortie équivalente) et vérifier l'atterrissage exact
   devant le bon bâtiment sur `CinnabarIsland_hns` — **pas** de téléportation vers un autre
   Kanto.

## 5. Warps à vérifier (déjà corrigés, à confirmer visuellement)

| Bâtiment | Warp entrée (CinnabarIsland_hns → intérieur) | Warp sortie (intérieur → CinnabarIsland_hns) |
|---|---|---|
| Arène | `(49,16)` → `MAP_CINNABAR_ISLAND_GYM`, warp id 1 | `(24-26,23)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 2 |
| Manoir | `(30,16)` → `MAP_POKEMON_MANSION_1F`, warp id 1 | `(7-9,33)` / `(34,33)` / `(35,34)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 3 |
| Labo | `(23,21)` → `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE`, warp id 1 | `(3-5,9)` → `MAP_CINNABAR_ISLAND_HNS`, warp id 4 |

Ces trois sorties pointaient auparavant vers `MAP_CINNABAR_ISLAND` (l'ancien Kanto FRLG inerte)
— déjà corrigé vers `MAP_CINNABAR_ISLAND_HNS` dans un commit précédent
(`feat(hs-map): blockout des entrées Gym/Manoir/Labo sur CinnabarIsland_hns`), revérifié ici en
relisant directement les `map.json` des 3 intérieurs : plus aucun warp lié à ces 3 bâtiments ne
cible l'ancien Kanto.

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

## 7. Ce qui n'a pas été touché

Aucune autre map, aucun autre script, aucun dialogue, aucun dresseur, aucun objet, aucun
Pokémon n'a été modifié pour ce chantier. Le mécanisme de test (`MAPTEST=1`,
`HNS_MAP_TEST_BUILD`) est entièrement conditionnel et n'a aucun effet sur le build normal
`make hns`.
