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
jouable. Il sert néanmoins de **réserve de géométrie de bâtiments** réutilisable par warp
(voir "Bâtiments manquants" plus bas), conformément à la règle « ne pas créer de nouvelle
map si une réutilisable existe ».

| Zone Heart & Soul | Map réelle (`_hns`) | Connexions confirmées | Réutilisable | Modification nécessaire |
|---|---|---|---|---|
| Cinnabar (île) | `CinnabarIsland_hns` | `Route20_hns`, `Route21_hns` (→ Johto ; **entièrement en eau, Surf requis, non praticable en tout début de partie** — cause du bug bloquant du retour de test n°5), warp Pokémon Center, **+ 3 warps bâtiments (Gym/Manoir/Labo), voir ci-dessous** | Oui — **complétée** | Fuite/retour gérés par `warp` scripté (voir implementation_notes.md). Warps vers Gym/Mansion/Lab **ajoutés et audités techniquement** (blockout, commits `d524e40`-`7ba52e2`/`2c7f519`) — voir "Bâtiments de Cinnabar" ci-dessous pour le détail de l'audit et son statut. PNJ Blaine/Blue existants réécrits (voir conflit narratif). |
| Pokémon Mansion | *(aucune version `_hns`)* → réutilise `PokemonMansion_1F/2F/3F/B1F_Frlg` (`MAP_POKEMON_MANSION_1F` etc., sans suffixe dans les constantes malgré le nom de dossier `_Frlg`) | — | Oui, par warp externe depuis `CinnabarIsland_hns` (fait) | Entrée bâtiment sur `CinnabarIsland_hns` (30,16) → `MAP_POKEMON_MANSION_1F` **faite**. Sous-sol (`B1F`) = zone bonus Kaïn post-game, accessible en interne (escalier `(25,27)`@1F ↔ `(34,29)`@B1F, audité). |
| Cinnabar Gym | *(aucune version `_hns`)* → réutilise `MAP_CINNABAR_ISLAND_GYM` | — | Oui, par warp externe (fait) | Entrée bâtiment sur `CinnabarIsland_hns` (49,16) → `MAP_CINNABAR_ISLAND_GYM` **faite**. Cohérent avec le dialogue existant de Blaine ("Gym pas encore reconstruit, je suis au Dojo") — mais voir limitation panneau ci-dessous. |
| Pokémon Lab (carnets de Blaine) | *(aucune version `_hns`)* → réutilise `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE` + Lounge/Research Room/Experiment Room | — | Oui, par warp externe (fait) | Entrée bâtiment sur `CinnabarIsland_hns` (23,21) → `LAB_ENTRANCE` **faite**, les 3 salles internes accessibles et auditées (warps cohérents). Support de la sous-intrigue "carnets de Blaine" (section 8) — pas encore scripté. |
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

## Bâtiments de Cinnabar — statut de l'audit technique (2026-08-30)

`CinnabarIsland_hns` a désormais 5 warps (Pokémon Center, sortie Johto, Gym `(49,16)`,
Manoir `(30,16)`, Labo `(23,21)`), ajoutés par édition de tilemap + `warp_events` dans
`map.json` (commits `d524e40`-`7ba52e2`, fusionnés dans `2c7f519`). Audit de connectivité et
de collision refait indépendamment ce jour (lecture directe de tous les `map.json` +
décodage binaire de tous les `map.bin` concernés, format `MAPGRID_*` de
`include/global.fieldmap.h`), sur l'ensemble de la chaîne de warps, pas seulement les portes
d'entrée :

- **Manoir** : `CinnabarIsland_hns(30,16)` ↔ `PokemonMansion_1F` warp 1 `(8,33)` — confirmé
  passable (collision 0) des deux côtés. Cage d'escalier 1F↔2F↔3F et 1F↔B1F entièrement
  auditée (10 warps sur 1F, 5 sur 2F, 8 sur 3F, 1 sur B1F) : chaque paire pointe vers l'index
  correct dans le tableau `warp_events` de la map cible, aucun warp orphelin vers une mauvaise
  map, aucune incohérence d'élévation déclarée vs élévation réelle de la tuile.
- **Labo** : `CinnabarIsland_hns(23,21)` ↔ `LAB_ENTRANCE` warp 1 `(4,9)` — confirmé passable.
  Les 3 salles (Lounge/Research Room/Experiment Room) bouclent proprement sur leurs warps
  dédiés (3/4/5) de l'Entrance, tuiles d'arrivée passables des deux côtés.
- **Gym** : `CinnabarIsland_hns(49,16)` ↔ `MAP_CINNABAR_ISLAND_GYM` warp 1 `(25,23)` —
  confirmé passable. Pas d'étage supplémentaire dans cette variante (single-room, réutilisée
  telle quelle).

**LIMITATION CONNUE (non bloquante, non corrigée)** : 2 warps hérités de la géométrie
vanilla FRLG du Manoir sont posés sur des tuiles à collision non nulle, donc physiquement
inatteignables par le joueur : `PokemonMansion_1F` warp 6 `(35,34)` (jumeau d'élévation d'une
des anciennes sorties sud du manoir d'origine) et warp 9 `(11,13)` (jumeau d'élévation de
l'escalier 1F→2F). Aucun impact fonctionnel — les warps réellement empruntables (1, 3, etc.)
sont sains — mais ce sont des entrées mortes dans `warp_events`, pas nettoyées ici : les
supprimer demanderait de renuméroter tous les `dest_warp_id` qui référencent ces tableaux par
index, un risque de régression plus grand que le bénéfice pour du contenu jamais atteint en
jeu. À corriger uniquement si une vraie raison de toucher ce fichier se présente.

**Ce qui reste à faire** (hors périmètre de cet audit, technique pur) : validation visuelle
en émulateur (aucun rendu graphique disponible côté agent, voir `MAP_TEST_README.md`) et
contenu scénaristique interne (carnets de Blaine au Labo, etc., section 8 de `histoire.md`).

## Point vérifié

`_hns` est confirmé comme Kanto complet, connecté et fonctionnel dans cette variante.
Cinnabar est désormais complète elle aussi : les 3 bâtiments (Gym/Manoir/Labo) ont leurs
warps posés et audités techniquement (connectivité + collision), cf. section ci-dessus.
Seule la validation visuelle en émulateur reste ouverte.
