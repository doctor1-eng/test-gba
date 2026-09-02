# Heart & Soul — Cartographie technique de Kanto

Correspondance entre les zones Heart & Soul (`docs/histoire.md`) et les maps réelles du
fork. Voir `implementation_notes.md` pour le constat complet.

**Cible retenue (décision utilisateur du 2026-08-29) : `_hns`.**

Ce dépôt contient deux jeux de maps Kanto (`_Frlg` et `_hns`), pas interchangeables. Audit de
connectivité (`connections` de `map.json`) confirmant que **`_hns` est bien le Kanto complet
et vivant de cette variante** : Pallet ↔ Route 1 ↔ Viridian ↔ Route 2 ↔ Pewter ↔ Route 3 →
Mt Moon → Route 4 ↔ Cerulean ↔ Routes 5/6/9/24 ↔ ... ↔ Vermilion ↔ Route 11 ↔ ... ↔ Fuchsia
↔ Routes 15/18/19 ↔ ... ↔ Saffron ↔ Routes 5/6/7/8 ↔ Celadon ↔ Routes 7/16 ↔ ... — un seul
graphe overworld cohérent, toutes les villes testées interconnectées.

`_Frlg` (Kanto original FireRed/LeafGreen, avec Mansion/Gym/Lab/Silph Co) coexiste dans le
dépôt mais son contenu (objets cachés, états de zone) est en grande partie câblé à `0` dans
`flags_hns.h` — signe qu'il est traité comme inerte dans cette variante, pas comme Kanto
jouable.

**⚠️ Correction majeure (2026-08-30) — `_Frlg` n'est PAS une réserve de géométrie
réutilisable par warp pour un build `POKEMON_HNS`, contrairement à ce que cette section
affirmait jusque-là.** Un warp externe vers un intérieur `_Frlg` **compile sans erreur** mais
**fait planter le jeu (reboot immédiat)** à l'entrée. Cause : `src/data/tilesets/headers.h`
sépare les tilesets par `#if !IS_FRLG && !IS_HNS` / `#elif IS_FRLG` / `#elif IS_HNS` — ce sont
des branches **mutuellement exclusives**, pas un contenu qui coexiste dans le même binaire.
Pour un build `POKEMON_HNS`, toute la branche `#elif IS_FRLG` (tilesets ET scripts des
intérieurs `_Frlg`) est absente du binaire ; les layouts correspondants (`layouts_table.inc`
généré) pointent vers `NULL`. Un simple retag `game_version: frlg → hns` sur les fichiers
JSON débloque le layout mais casse le lien (tilesets/scripts introuvables à l'édition de
liens). Ce piège a été découvert concrètement sur Gym/Manoir/Labo de Cinnabar (voir
`MAP_TEST_README.md` section 5quater pour le détail complet) — la doc précédente de cette
section, qui décrivait ces 3 bâtiments comme "réutilisation `_Frlg` auditée et fonctionnelle",
était **fausse** : l'audit en question (connectivité `warp_events` + collision `map.bin`)
était correct sur ses propres critères mais ne pouvait pas détecter ce type d'exclusion de
compilation, invisible tant qu'on ne regarde pas `headers.h` ou qu'on ne teste pas en jeu.
**Leçon pour toute réutilisation `_Frlg` future (Silph Co, etc.)** : vérifier d'abord que le
tileset primaire/secondaire de la map visée est bien compilé sous `IS_HNS` dans
`src/data/tilesets/headers.h`, avant de considérer une géométrie `_Frlg` comme "prête à
réutiliser" — une correspondance `map.json`/`map.bin` propre ne suffit pas.

| Zone Heart & Soul | Map réelle (`_hns`) | Connexions confirmées | Réutilisable | Modification nécessaire |
|---|---|---|---|---|
| Cinnabar (île) | `CinnabarIsland_hns` | `Route20_hns`, `Route21_hns` (→ Johto ; **entièrement en eau, Surf requis, non praticable en tout début de partie** — cause du bug bloquant du retour de test n°5), warp Pokémon Center, **+ 3 warps bâtiments (Gym/Manoir/Labo), voir ci-dessous** | Oui — **complétée** | Fuite/retour gérés par `warp` scripté (voir implementation_notes.md). Warps vers Gym/Manoir/Labo **posés et fonctionnels**, pointant vers 3 nouvelles maps `_Hns` natives (voir "Bâtiments de Cinnabar" ci-dessous). PNJ Blaine/Blue existants réécrits (voir conflit narratif). |
| Pokémon Mansion | `CinnabarIsland_Mansion_Hns` — **nouvelle map native**, pièce unique 13×10 | — | Warp externe depuis `CinnabarIsland_hns` (fait) | Entrée `(30,16)` → `MAP_CINNABAR_ISLAND_MANSION_HNS` **faite et fonctionnelle** (l'ancienne cible `MAP_POKEMON_MANSION_1F`, réutilisation `_Frlg`, faisait planter le jeu — voir avertissement en tête de fichier). Pas d'étages/sous-sol dans cette variante ; zone bonus Kaïn post-game à repenser séparément. Blockout identique aux 2 autres bâtiments (même pièce vide), pas encore meublée. |
| Cinnabar Gym | `CinnabarIsland_Gym_Hns` — **nouvelle map native**, pièce unique 13×10 | — | Warp externe (fait) | Entrée `(49,16)` → `MAP_CINNABAR_ISLAND_GYM_HNS` **faite et fonctionnelle** (même correction que le Manoir). Cohérent avec le dialogue existant de Blaine ("Gym pas encore reconstruit, je suis au Dojo") — mais voir limitation panneau ci-dessous. Blockout, pas encore meublé. |
| Pokémon Lab (carnets de Blaine) | `CinnabarIsland_PokemonLab_Hns` — **nouvelle map native**, pièce unique 13×10 | — | Warp externe (fait) | Entrée `(23,21)` → `MAP_CINNABAR_ISLAND_POKEMON_LAB_HNS` **faite et fonctionnelle** (même correction). Plus de salles séparées (Lounge/Research/Experiment) dans cette variante — une seule pièce. Support de la sous-intrigue "carnets de Blaine" (section 8) — pas encore scripté, ni la subdivision en salles si elle s'avère nécessaire au contenu. |
| Route 21 | `Route21_hns` | Cinnabar ↔ Route 20 | Oui | Séquence de fuite/sauvetage en mer à scripter. |
| Pallet Town | `PalletTown_hns` | `Route1_hns`, `Route21_hns` | Oui | PNJ "dresseur local" (section 8) à ajouter. |
| Route 1 | `Route1_hns` | Pallet ↔ Viridian | Oui | Quête objet perdu / milice de Viridian faite (voir implementation_notes.md) : Quinn reflavorée en milice, `HeartSoul_EventScript_MiliceRoute1`. |
| Viridian City | `ViridianCity_hns` (+ `_Gym_hns`) | Route 1, Route 2, Route 22 | Oui | Arène : Blue (`TRAINER_BLUE_HNS`, Champion d'Arène dans le jeu de base) masqué jusqu'à l'Acte V — conflit narratif avec l'antagoniste résolu (voir implementation_notes.md). Débat commerçant/résistance (section 8) toujours à ajouter. |
| Viridian Forest | `ViridianForest_hns` | Accès via `Gate_Route2_ViridianForest_hns` / `Gate_ViridianForest_Route2_hns` (portes dédiées, pas de connexion `map.json` directe) | Oui | Repaire de Lyre (lieutenant). |
| Pewter City | `PewterCity_hns` (+ `_Gym_hns`) | Route 2, Route 3 | Oui | Scène de doute de Pierre ajoutée (`HeartSoul_EventScript_PierreDoute`, `heart_and_soul_act2.inc`), câblée dans `EventScript_Brock` avant le combat de badge existant. Sous-intrigue "vivres cachées par Pierre" (section 8) toujours à ajouter. |
| Mt Moon | `MtMoon_Outside_hns`, `MtMoon_Cave_hns`, `MtMoon_Shop_hns` | Route 4 | Oui | Repaire de Selen. Chercheuse rationaliste (section 8) à ajouter. |
| Cerulean City | `CeruleanCity_hns` (+ `_Gym_hns`) | Routes 4, 5, 9, 24 | Oui | Scène de doute d'Ondine ajoutée (`HeartSoul_EventScript_OndineDoute`, `heart_and_soul_act2.inc`), câblée dans `EventScript_Misty` avant le combat de badge existant. Quête canalisations/inondation — état "endommagé" de la Gym (section 9) toujours à vérifier/créer. |
| Rock Tunnel | `RockTunnel_1F_hns`, `_B1F_hns` | Accès via portes dédiées (à vérifier, pas de connexion `map.json` directe trouvée) | Oui | Repaire de Terrence (conviction). Mineurs piégés (section 8) à créer. |
| Vermilion City | `VermilionCity_hns` (+ `_Gym_hns`) | Route 6, Route 11, `VermilionCity_PortOutside_hns` | Oui | Major Bob / interception radio à scripter. Port existant = bon point d'ancrage pour la traversée maritime Cinnabar↔continent. |
| Zone Safari | `SafariZone1/2/3_hns`, `_Indoor_hns`, zones `_TopLeft/TopMid/...` | `SafariZoneGate_hns` ↔ `Route48_hns` | Oui | Repaire de Kess (conviction). Pokémon braconnés à libérer. |
| Fuchsia City | `FuchsiaCity_hns` (+ `_Gym_hns`) | Routes 15, 18, 19 | Oui | Débat clan de Koga à scripter. |
| Céladopole | `CeladonCity_hns` (+ `_Gym_hns`, `_DepartmentStore_*_hns`, `_GameCorner_hns`) | Routes 7, 16 | Oui | Game Corner `_hns` existant = bon point d'ancrage pour "financement Rocket exposé". |
| Saffron City | `SaffronCity_hns` (+ `_Gym_hns`) | Routes 5, 6, 7, 8 — carrefour central | Oui | — |
| Silph Co | `SaffronCity_SilphCo_hns` | Warps internes uniquement (pas de `connections` externe, normal pour un intérieur) | Oui | Repaire de Mira Voss (lieutenant final) + documents à lire (section 8). À vérifier : nombre d'étages réellement modélisés côté `_hns` (la version `_Frlg` en a 11 ; la version `_hns` est un seul fichier de map, structure interne à explorer avant de répartir le contenu). |

## Bâtiments de Cinnabar — statut actuel (mis à jour 2026-09-02)

**Historique court** (détail complet dans `MAP_TEST_README.md` section 5quater et
`implementation_notes.md`) : une première approche (warp externe vers les intérieurs `_Frlg`
existants — `PokemonMansion_1F/2F/3F/B1F_Frlg`, `CinnabarIsland_Gym_Frlg`,
`CinnabarIsland_PokemonLab_Entrance_Frlg` + 3 salles) a été construite, auditée
techniquement (connectivité `warp_events` + collision `map.bin`, tous les warps internes
cohérents) et déclarée saine sur cette base — **mais faisait planter le jeu en pratique**
(reboot à l'entrée), pour une raison invisible à ce type d'audit : les tilesets/scripts
`_Frlg` de ces intérieurs sont exclus à la compilation pour un build `POKEMON_HNS` (voir
l'avertissement en tête de ce fichier). Cette approche a été **entièrement abandonnée**.

**Approche actuelle** : 3 nouvelles maps natives `_Hns`, chacune une pièce unique 13×10
copiée depuis `VermilionCity_House1_hns` (tileset `gTileset_Johto_Building_Hns` +
`gTileset_House_Lab_Hns`, déjà compilé et déjà utilisé avec succès ailleurs dans le jeu) :

| Bâtiment | Map | Warp entrée (`CinnabarIsland_hns` →) | Warp sortie (→ `CinnabarIsland_hns`) |
|---|---|---|---|
| Arène | `CinnabarIsland_Gym_Hns` | `(49,16)` → warp id 0 | `(4,8)` → warp id 2 |
| Manoir | `CinnabarIsland_Mansion_Hns` | `(30,16)` → warp id 0 | `(4,8)` → warp id 3 |
| Labo | `CinnabarIsland_PokemonLab_Hns` | `(23,21)` → warp id 0 | `(4,8)` → warp id 4 |

Vérifié après compilation dans les `events.inc` générés (pas seulement les `map.json` source) :
chaque paire de warps se référence correctement dans les deux sens. Un bug de collision
séparé sur la porte du Labo (case `(23,21)` avec collision 1 au lieu de 0, donc
infranchissable) a été trouvé et corrigé — voir `MAP_TEST_README.md` section 5ter.

**LIMITATION ASSUMÉE (documentée, pas un oubli)** : les 3 intérieurs sont actuellement des
**blockouts identiques** (même pièce vide, même tileset, aucun PNJ ni mobilier thématique) —
pas d'étages (le sous-sol B1F du Manoir, prévu comme zone bonus Kaïn post-game, et les 3
salles séparées du Labo n'existent plus dans cette architecture ; à repenser séparément si ce
contenu reste souhaité). Suffisant pour valider warps/collisions/transitions ; une passe de
contenu (mobilier, PNJ, subdivision en salles si nécessaire) reste à faire.

**Ce qui reste à faire** : validation visuelle en émulateur (aucun rendu graphique
disponible côté agent, voir `MAP_TEST_README.md`), contenu scénaristique interne (carnets de
Blaine au Labo, etc., section 8 de `histoire.md`), et décision sur la zone bonus Kaïn
(nécessite un sous-sol qui n'existe plus dans l'architecture actuelle).

## Point vérifié

`_hns` est confirmé comme Kanto complet, connecté et fonctionnel dans cette variante.
Cinnabar est complète elle aussi : les 3 bâtiments (Gym/Manoir/Labo) ont des warps
fonctionnels vers des maps natives `_Hns` (plus de dépendance à une géométrie `_Frlg`
inutilisable pour ce type de build). Seule la validation visuelle en émulateur reste
ouverte, ainsi que le contenu interne (mobilier/PNJ/étages) de ces 3 bâtiments.
