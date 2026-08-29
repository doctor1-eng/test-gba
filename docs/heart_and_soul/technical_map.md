# Heart & Soul — Cartographie technique de Kanto

Correspondance entre les zones Heart & Soul (`docs/histoire.md`) et les maps réelles du
fork. Voir `implementation_notes.md` pour le constat complet : ce dépôt contient **deux**
jeux de maps Kanto (`_Frlg` et `_hns`) qui ne sont pas interchangeables. Le jeu `_Frlg` est
le Kanto complet et connecté (Arènes, donjons, Mansion, Silph Co tous présents et reliés) ;
le jeu `_hns` est une version très allégée, sans la plupart de ces bâtiments, orientée
post-game Johto. **Toutes les recommandations ci-dessous ciblent `_Frlg`**, sous réserve de
la décision utilisateur sur la manière dont ce Kanto est actuellement rattaché à la
progression du jeu de base.

| Zone Heart & Soul | Map réelle | Groupe/ID | Réutilisable | Modification nécessaire |
|---|---|---|---|---|
| Cinnabar (île) | `CinnabarIsland_Frlg` | `(40\|(0<<8))` group 0 | Oui | Reprendre `CinnabarIsland_EventScript_Blaine`/`_Blue` — voir conflit narratif dans `implementation_notes.md`. Warps existants vers Gym/Mansion/Lab/Mart/PokeCenter à conserver. |
| Pokémon Mansion | `PokemonMansion_1F/2F/3F/B1F_Frlg` | group 66 | Oui | Contenu narratif "carnets de Blaine" à ajouter en objets/scripts sur les étages existants. Sous-sols (`B1F`) = zone bonus Kaïn post-game, à vérifier qu'elle a la place pour un combat de boss. |
| Route 21 | `Route21_North_Frlg` / `Route21_South_Frlg` | — | Oui | Connecte Pallet ↔ Cinnabar. Séquence de fuite/sauvetage en mer à scripter sur la portion Sud (adjacente à Cinnabar). |
| Pallet Town | `PalletTown_Frlg` | group 0, id 31 (`_hns`) | Oui | Connecte Route 1 et Route 21 Nord. PNJ "dresseur local" de la section 8 à ajouter. |
| Route 1 | `Route1_Frlg` | — | Oui | Relie Pallet ↔ Viridian. Quête "objet perdu, milice de Viridian" à scripter. |
| Viridian City | `ViridianCity_Frlg` (+ `_Gym_Frlg`) | id 32 | Oui | Connecte Route 1, Route 2, Route 22. Débat commerçant/résistance à ajouter. Vérifier statut du Gym (Giovanni/vacant) avant de scripter l'écho volontaire de la section 4. |
| Viridian Forest | `ViridianForest_Frlg` | group 24 (`_hns`: id 56) | Oui | Repaire de Lyre (lieutenant). Aucune connexion déclarée dans `map.json` au-delà des warps internes — à vérifier avec `data/maps/Gate_*` avant de placer le combat. |
| Pewter City | `PewterCity_Frlg` (+ `_Gym_Frlg`) | — | Oui | Connecte Route 2 et Route 3 (→ Mt Moon). Trainerbattle Brock déjà scripté comme référence de syntaxe (`PewterCity_Gym_hns/scripts.inc`, à adapter côté `_Frlg`). Sous-intrigue "vivres cachées par Pierre" à ajouter. |
| Mt Moon | `MtMoon_1F/B1F/B2F_Frlg` | group 23/66 | Oui | Repaire de Selen (lieutenant). Chercheuse rationaliste (section 8) à ajouter. |
| Cerulean City | `CeruleanCity_Frlg` (+ `_Gym_Frlg`) | — | Oui | Connecte Route 4, 5, 9, 24. Quête canalisations/inondation à scripter — vérifier si la Gym `_Frlg` a déjà un état "endommagé" exploitable ou si un état alternatif est à créer. |
| Rock Tunnel | `RockTunnel_1F/B1F_Frlg` | — | Oui | Repaire de Terrence (lieutenant, conviction). Mineurs piégés (section 8) à ajouter — aucun événement de ce type détecté dans le script actuel, à créer. |
| Vermilion City | `VermilionCity_Frlg` (+ `_Gym_Frlg`) | — | Oui | Connecte Route 6 et 11. Major Bob / interception radio à scripter. SS Aqua (`SSAqua_1F_hns` existe déjà côté Johto — vérifier s'il y a un équivalent FRLG pour la traversée maritime évoquée dans `histoire.md`). |
| Zone Safari | `SafariZone_Center/East/North/West_Frlg` + `_RestHouse` | group 57 (`_Frlg`), 30 (`_hns`) | Oui | Repaire de Kess (lieutenant, conviction). Pokémon braconnés à libérer — à scripter. |
| Fuchsia City | `FuchsiaCity_Frlg` (+ `_Gym_Frlg`) | — | Oui | Connecte Route 15, 18, 19. Débat clan de Koga à scripter. |
| Céladopole | `CeladonCity_Frlg` (+ `_Gym_Frlg`, `_DepartmentStore_*_Frlg`, `_GameCorner_Frlg`) | — | Oui | Connecte Route 7 et 16. Le Game Corner `_Frlg` existe déjà (écho canonique direct) — quête "financement Rocket exposé" à y greffer plutôt que réinventer un lieu. |
| Saffron City | `SaffronCity_Frlg` (+ `_Gym_Frlg`) | — | Oui | Connecte Route 5, 6, 7, 8 — carrefour central de Kanto. |
| Silph Co | `SilphCo_1F` à `_11F_Frlg` + `_Elevator_Frlg` | — | Oui | 11 étages déjà modélisés. Repaire de Mira Voss (lieutenant final avant Blue) et documents à lire (section 8) — à répartir sur les étages existants plutôt qu'en créer de nouveaux. |

## Zones sans correspondance directe dans `histoire.md` mais présentes et à connaître

- **Route 22 / Route 23 / Victory Road Kanto** (`FLAG_HIDDEN_ITEM_VICTORYROADKANTO_*` confirme
  leur existence) : non mentionnées dans le découpage en actes, à statuer si utilisées.
- **`CinnabarIsland_hns`** : existe en parallèle, connectée à Johto (`Route20_hns`/`Route21_hns`
  → `NewBarkTown_hns`). À ne pas toucher si on cible `_Frlg` pour Heart & Soul — deux Cinnabar
  distinctes dans le même jeu, ne pas les confondre en écrivant les scripts.

## Point ouvert

Comment le Kanto `_Frlg` est-il actuellement rattaché à la trame Johto du jeu de base
(accessible en cours de partie normale, ou contenu largement séparé/inactif) ? Détermine si
la structure "monde ouvert dès le début" de Heart & Soul peut s'appuyer directement dessus
ou nécessite un point d'entrée dédié. Voir décision demandée dans `implementation_notes.md`.
