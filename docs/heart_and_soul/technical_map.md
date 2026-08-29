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
| Cinnabar (île) | `CinnabarIsland_hns` | `Route20_hns`, `Route21_hns` (→ Johto ; **entièrement en eau, Surf requis, non praticable en tout début de partie** — cause du bug bloquant du retour de test n°5), warp Pokémon Center | Oui, mais **incomplète** | Fuite/retour désormais gérés par `warp` scripté (voir implementation_notes.md) plutôt que par la connexion piétonne. Pas de warp vers Gym/Mansion/Lab — à ajouter (voir "Bâtiments manquants"), bloqué par l'absence de vérification visuelle. PNJ Blaine/Blue existants réécrits (voir conflit narratif). |
| Pokémon Mansion | *(aucune version `_hns`)* → réutiliser `PokemonMansion_1F/2F/3F/B1F_Frlg` (`MAP_POKEMON_MANSION_1F` etc., sans suffixe dans les constantes malgré le nom de dossier `_Frlg`) | — | Oui, par warp externe depuis `CinnabarIsland_hns` | Créer une entrée bâtiment sur la carte `CinnabarIsland_hns` (tuiles + warp) menant à `MAP_POKEMON_MANSION_1F`. Sous-sol (`B1F`) = zone bonus Kaïn post-game. |
| Cinnabar Gym | *(aucune version `_hns`)* → réutiliser `MAP_CINNABAR_ISLAND_GYM` | — | Oui, par warp externe | Idem : entrée bâtiment sur `CinnabarIsland_hns` + warp. Cohérent avec le dialogue existant de Blaine ("Gym pas encore reconstruit, je suis au Dojo"). |
| Pokémon Lab (carnets de Blaine) | *(aucune version `_hns`)* → réutiliser `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE`+ | — | Oui, par warp externe | Idem. Support de la sous-intrigue "carnets de Blaine" (section 8). |
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

## Bâtiments manquants à Cinnabar — plan de réutilisation

`CinnabarIsland_hns` (`data/maps/CinnabarIsland_hns/`) n'a actuellement que 2 warps (Pokémon
Center, sortie vers Johto) et 12 object events. Pour Gym/Mansion/Lab, la règle "pas de
nouvelle map si réutilisable" s'applique à la géométrie intérieure (déjà entièrement
modélisée côté `_Frlg`) mais pas au point d'entrée, qui doit être ajouté sur la carte
extérieure `_hns` elle-même (tuiles de bâtiment + `warp_events` dans `map.json`/`events.inc`
vers `MAP_POKEMON_MANSION_1F`, `MAP_CINNABAR_ISLAND_GYM`,
`MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE`). C'est une édition de tilemap (pas seulement de
script), à faire et vérifier visuellement avant de scripter le contenu à l'intérieur —
prochaine étape technique, pas encore réalisée.

## Point vérifié

`_hns` est confirmé comme Kanto complet, connecté et fonctionnel dans cette variante —
seule Cinnabar nécessite un ajout de warps pour ses bâtiments intérieurs.
