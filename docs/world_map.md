# Heart & Soul — World Map (Kanto, cartographie de niveau)

Ce document est la cartographie **géographique et technique** de Kanto pour Heart & Soul, du
point de vue Map Director / Level Design. Il complète — sans le dupliquer —
`docs/heart_and_soul/technical_map.md` (qui documente la correspondance narration ↔ map, écrit
côté script/intégration) : ici, l'angle est la construction du monde lui-même (connectivité,
géométrie, tilesets, lisibilité, contenu manquant à concevoir).

Source de vérité narrative : `docs/heart_and_soul/docs/histoire.md`.
Source de vérité technique : ce dépôt, fork de `PokemonHnS-Development/pokehns-expansion`
(`Release-v2.0.4`), branche `claude/pokemon-heart-soul-audit-qv0f4n`.

## 0. Ce que ce document N'EST PAS

Aucune map n'a été modifiée pour produire ce document. C'est un audit de lecture (fichiers
`data/maps/*/map.json`, `data/layouts/layouts.json`, `data/layouts/*/map.bin`), pas une
proposition déjà construite. Voir `docs/map_design.md` pour le plan d'action et le premier
chantier concret (Cinnabar).

## 1. Moteur et données — constat d'audit

- **Fork** : pokeemerald-expansion → lignée `pret/pokeemerald → Modern Emerald →
  pokeemerald-expansion → pokehns-expansion`. Ce n'est pas un fork nu : c'est un demake
  GSC/HGSS complet (Johto = trame principale), qui embarque tout le contenu vanilla Emerald/FRLG
  (Hoenn + Kanto) comme **post-game**.
- **Deux jeux de maps Kanto coexistent dans le dépôt, non interchangeables** :
  - `*_Frlg` : Kanto FireRed/LeafGreen canonique, complet (Arènes, Pokemon Mansion, Lab, Silph
    Co, Zone Safari), mais son état runtime (`flags_hns.h`) est câblé à `0` presque partout —
    **traité comme inerte** dans cette variante. Sert de **réserve de géométrie** réutilisable.
  - `*_hns` : Kanto retravaillé pour la structure HGSS-like du fork. **Confirmé par audit de
    connectivité (ci-dessous, section 3) comme le Kanto complet, connecté et jouable de cette
    variante.** C'est la cible retenue (décision utilisateur du 2026-08-29, déjà actée dans
    `technical_map.md`) — ce document la reprend et l'étend sans la remettre en cause.
- **Format de map** : `data/maps/<NomDeMap>/map.json` (métadonnées : id, layout, connections,
  warp_events, object_events, bg_events) + `data/maps/<NomDeMap>/scripts.inc` (logique). Géométrie
  séparée dans `data/layouts/<NomDeLayout>/{map.bin, border.bin}`, indexée par
  `data/layouts/layouts.json` (dimensions en tiles, tileset primaire/secondaire).
- **Éditeur prévu** : `porymap.project.json` présent à la racine → **Porymap** est l'éditeur
  visuel du projet. **Non disponible dans cet environnement** (pas d'affichage, session headless).
- **Aucun outil de rendu/preview** trouvé dans ce dépôt pour visualiser une map sans Porymap ni
  émulateur graphique. `tools/mgba/mgba-rom-test` est un harnais de **tests de commandes de
  script** (`-S SWI`, pas d'option de capture d'écran) — confirmé par lecture de son `--help`, pas
  un moyen de voir le rendu d'une carte. **Conséquence directe pour tout futur travail de
  géométrie (placement de bâtiments, tuiles) : voir la méthode de vérification sans rendu visuel
  décrite dans `docs/map_design.md` section « Méthode de vérification sans rendu visuel ».**
- **Scripting** : pas de Poryscript actif dans la chaîne de build de ce fork (règle commentée
  dans le `Makefile`). Tout le contenu Heart & Soul déjà écrit l'est en macros `.inc` natives —
  sans impact sur le travail de map (géométrie + `warp_events` + `object_events` en JSON/binaire),
  mais à savoir si un script doit accompagner une nouvelle map.
- **Build** : `make hns -j<nproc>` → `pokehns.gba`. Baseline confirmée PASS (0 erreur), ROM à
  ~94.4 % de capacité — **marge réduite**, à surveiller à chaque ajout de tileset/graphismes.

## 2. État d'avancement narratif déjà en place (pour contexte, pas un livrable de ce doc)

Une session précédente a déjà scripté, testé (par compilation, pas en jeu avec écran) et *en
partie* confirmé en jeu par l'utilisateur : le choix de type/équipe, l'attaque de Cinnabar (Acte
I), la fuite scriptée vers Pallet, le verrouillage du retour à Cinnabar, les scènes de doute de
Pierre et Ondine (Acte II), plusieurs sous-intrigues (Route 1, Viridian, Forêt de Jade, Azuria),
et le rééquilibrage des dresseurs/rencontres sauvages jusqu'à Carmin-sur-Mer (4ᵉ arène). Détail
complet : `docs/heart_and_soul/implementation_notes.md`.

**Point important pour ce document** : tout ce travail réutilise la géométrie **existante** des
maps `_hns` sans la modifier. **Aucune édition de tilemap n'a encore eu lieu.** Le travail de
Map Director décrit ici (ajout de bâtiments, mise en scène de la destruction, etc.) est donc un
chantier neuf, pas une suite de ce qui existe déjà.

## 3. Graphe de connectivité — vérifié zone par zone

Vérifié par lecture directe de `connections` (et `warp_events` pour les portes dédiées/
intérieurs) dans chaque `map.json`, pas repris de mémoire. Toutes les zones listées dans le
brief Heart & Soul sont présentes et connectées dans `_hns`.

```
                         (mer, Route 20 — connexion Cinnabar/Fuchsia)
                                      │
CINNABAR ISLAND (île) ───Route 21────PALLET TOWN
  │  connexions confirmées :               │ Route 1
  │  ↑ Route21_hns → Pallet                ↓
  │  → Route20_hns → Route19 (Fuchsia)  VIRIDIAN CITY ──Route 22──→ (Ligue Indigo,
  │  (les deux entièrement en mer :         │  Route 2                zone post-Acte V)
  │   fuite/retour Cinnabar gérés par       ↓
  │   warp scripté, pas par la connexion)  Gate → VIRIDIAN FOREST → Gate → (Route 2, suite)
  │                                         │  Route 2
[Pokémon Mansion / Gym / Lab :              ↓
 géométrie _Frlg réutilisable,          PEWTER CITY
 PAS ENCORE de porte sur la carte           │  Route 3
 extérieure _hns — voir section 5]          ↓
                                        ROUTE 3 → MT MOON (extérieur → grotte → boutique)
                                             │  Route 4
                                             ↓
                                        CERULEAN CITY
                                        │Route5      │Route9         │Route24
                                        ↓            ↓                ↓
                                   SAFFRON CITY   ROUTE 9→10      ROUTE 24→25
                                   (carrefour)    → ROCK TUNNEL   (impasse nord,
                                   │Route6 │Route7│Route8          zone annexe)
                                   ↓       ↓      ↓
                              VERMILION  CELADON  ROUTE 10 (suite,
                              CITY       CITY     Power Plant / "Centrale")
                              │Route11            → Rock Tunnel B1F
                              ↓                   → Lavender Town
                              ROUTE 11→12→13→14
                                             │Route15/18/19
                                             ↓
                                       FUCHSIA CITY
                                             │ Gate
                                             ↓
                                       SAFARI ZONE GATE → ROUTE 48 → ROUTE 47 → (Cianwood,
                                       (entrée Zone Safari / PC)                 Johto)

SAFFRON CITY → SILPH CO (intérieur, warps internes uniquement, pas de connexion externe)
```

### Table de connectivité (résumé par zone, direction → voisin)

| Zone | Connexions confirmées | Type de lien |
|---|---|---|
| Cinnabar Island | ↑ Route 21 (Pallet), → Route 20 (Route 19/Fuchsia) | `connections` (100 % mer, non praticable à pied en début de partie) |
| Route 21 | ↓ Cinnabar, ↑ Pallet | `connections` (mer) |
| Pallet Town | ↑ Route 1, ↓ Route 21 | `connections` |
| Route 1 | ↑ Viridian, ↓ Pallet | `connections` |
| Viridian City | ← Route 22, ↓ Route 1, ↑ Route 2 | `connections` |
| Route 2 | ↓ Viridian, ↑ Pewter | `connections` + porte dédiée vers Forêt de Jade |
| Viridian Forest | (accès uniquement par portes `Gate_Route2_ViridianForest_hns` / `Gate_ViridianForest_Route2_hns`) | `warp_events` (porte) |
| Pewter City | ↓ Route 2, → Route 3 | `connections` |
| Route 3 | ← Pewter, ↑ Route 4 | `connections` |
| Mt Moon (extérieur) | ↓ Route 4 | `connections` |
| Route 4 | → Cerulean, ↓ Route 3, ↑ Mt Moon | `connections` |
| Cerulean City | ← Route 4, ↓ Route 5, → Route 9, ↑ Route 24 | `connections` (carrefour à 4 branches) |
| Route 5 | ↑ Cerulean, ↓ Saffron | `connections` |
| Route 9 | ← Cerulean, → Route 10 | `connections` |
| Route 10 | ↓ Lavender Town, ← Route 9, + 2 warps vers Rock Tunnel B1F, + Power Plant | `connections` + `warp_events` |
| Rock Tunnel (1F/B1F) | Accès **uniquement** depuis Route 10 (2 warps), aucune porte côté Route 9 | `warp_events` |
| Route 24 / 25 | Impasse nord depuis Cerulean | `connections` |
| Saffron City | ↑ Route 5, ↓ Route 6, ← Route 7, → Route 8 | `connections` (carrefour central, 4 branches) |
| Silph Co | Intérieur de Saffron, warps internes uniquement | `warp_events` |
| Route 6 | ↑ Saffron, ↓ Vermilion | `connections` |
| Vermilion City | ↑ Route 6, → Route 11, ↓ Vermilion Port | `connections` |
| Route 11→12→13→14 | Chaîne linéaire vers le sud | `connections` |
| Route 7 | ← Celadon, → Saffron | `connections` |
| Celadon City | → Route 7, ← Route 16 | `connections` |
| Route 16→17→18 | Chaîne vers Fuchsia | `connections` |
| Fuchsia City | ← Route 18, → Route 15, ↓ Route 19 | `connections` + portes dédiées Route 15/18 |
| Route 19 | ↑ Fuchsia, ← Route 20 | `connections` (relie Fuchsia à Cinnabar par la mer) |
| Zone Safari | `SafariZoneGate_hns` ↔ `Route48_hns` ↔ `Route47_hns` | `warp_events` en cascade (porte → PC / entrée zone) |

**Confirmation** : le graphe est un seul ensemble connexe, sans île orpheline (hormis Cinnabar,
isolée **volontairement** par la narration et reliée par un warp scripté plutôt que par la
marche). Toutes les villes listées dans le brief Heart & Soul existent et sont atteignables.

### Clarification apportée par cet audit : « Route de la Centrale »

`docs/histoire.md` situe le lieutenant **Terrence** sur la « Route de la Centrale (Rock
Tunnel) ». L'audit de connectivité montre que **Rock Tunnel n'est accessible que depuis
Route 10**, et que Route 10 contient aussi `MAP_ROUTE10_POWER_PLANT_ENTRANCE_HNS` — la
**Centrale électrique** au sens propre (French pour *Power Plant*), à quelques cases des warps
Rock Tunnel. Les deux lieux sont donc la même zone de jeu au sens Heart & Soul : le complexe
Route 10 / Rock Tunnel / Centrale électrique. Ceci n'est pas une contradiction avec
`technical_map.md`, mais une précision géographique utile pour la mise en scène de Terrence
(ex-mineur *et* zone industrielle abandonnée dans un seul et même espace, cohérent avec son
profil « Rock/Ground, terrain-control »).

## 4. Inventaire par zone — dimensions, tilesets, contenu

Dimensions et tilesets lus directement dans `data/layouts/layouts.json` (pas d'estimation).

| Zone | Map(s) `_hns` | Dimensions (tuiles) | Tileset primaire / secondaire | Bâtiments/warps existants | Statut Heart & Soul |
|---|---|---|---|---|---|
| Cinnabar Island | `CinnabarIsland_hns` | 72×44 | `Kanto_General_Hns` / `Lavaridge_Hns` (volcanique) | 2 warps (Pallet-side legacy vers New Bark Town — probablement un warp de continuité Johto à ne pas casser ; Pokémon Center). 12 object events (Blaine, Blue [à masquer], faune sauvage volcanique : Magcargo, Slugma, Koffing, Weezing, Magmar, Magby, Misdreavus, Torkoal, Wailmer). | Scripté (attaque, fuite, verrouillage). **Aucune entrée Gym/Mansion/Lab sur la carte.** Voir section 5. |
| Pokémon Mansion | *(aucune version `_hns`)* → `PokemonMansion_1F/2F/3F/B1F` (`_Frlg`) | 38×35 (1F/3F/B1F), 38×38 (2F) | `BuildingFrlg` / `PokemonMansion` | Géométrie complète (canon FRLG), inerte dans `_hns` | Réserve à réutiliser par warp externe, contenu narratif ("carnets de Blaine") pas encore scripté |
| Cinnabar Gym | *(aucune version `_hns`)* → `MAP_CINNABAR_ISLAND_GYM` | 30×25 | `BuildingFrlg` / `CinnabarGym` | idem | idem |
| Pokémon Lab (Cinnabar) | *(aucune version `_hns`)* → `..._POKEMON_LAB_ENTRANCE/LOUNGE/RESEARCH_ROOM/EXPERIMENT_ROOM` | 28×11 (entrée), 15×11 (autres) | `BuildingFrlg` / `Lab_Frlg` | idem | idem |
| Route 21 | `Route21_hns` | — | mer (canon) | Connexion Cinnabar↔Pallet, 100 % eau | Séquence de fuite scriptée (warp, pas de traversée réelle) |
| Pallet Town | `PalletTown_hns` | — | — | Warp d'arrivée vérifié (7,8→6,8 raw blockdata, collision 0) | PNJ dresseur local (section 8 histoire) à ajouter |
| Route 1 | `Route1_hns` | — | — | Quinn (milice) reflavorée | Fait côté script, pas de map à modifier |
| Viridian City | `ViridianCity_hns` + `_Gym_hns` | — | — | Blue masqué (Gym) jusqu'à Acte V | Débat commerçant/résistance (section 8) — script, pas de map |
| Viridian Forest | `ViridianForest_hns` | — | — | Accès par portes dédiées | Repaire de Lyre — non scénographié |
| Pewter City | `PewterCity_hns` + `_Gym_hns` | — | — | Scène de doute Pierre câblée | Vivres cachées (section 8) — script |
| Mt Moon | `MtMoon_Outside_hns`, `MtMoon_Cave_hns`, `MtMoon_Shop_hns` | — | — | — | Repaire de Selen — non scénographié |
| Cerulean City | `CeruleanCity_hns` + `_Gym_hns` | — | — | Scène de doute Ondine câblée | État "endommagé" (inondation, section 9 histoire) — **map non modifiée**, à concevoir |
| Rock Tunnel / Route 10 | `RockTunnel_1F_hns`, `_B1F_hns`, `Route10_PowerPlant*` | — | — | Accès uniquement par Route 10 | Repaire de Terrence, mineurs piégés — non scénographié |
| Vermilion City | `VermilionCity_hns` + `_Gym_hns` + `_PortOutside_hns` | — | — | Port existant | Major Bob / interception radio — script, pas de map |
| Zone Safari | `SafariZone1/2/3_hns`, `_Indoor_hns`, zones `_TopLeft/TopMid/...` | — | — | `SafariZoneGate_hns` en cascade | Repaire de Kess — non scénographié |
| Fuchsia City | `FuchsiaCity_hns` + `_Gym_hns` | — | — | — | Débat clan Koga — script |
| Céladopole | `CeladonCity_hns` + `_Gym_hns` + `_DepartmentStore_*` + `_GameCorner_hns` | — | — | Game Corner existant | Financement Rocket exposé — script |
| Saffron City | `SaffronCity_hns` + `_Gym_hns` | — | — | Carrefour central | — |
| Silph Co | `SaffronCity_SilphCo_hns` | — | — | Warps internes uniquement, structure interne à explorer | Repaire de Mira Voss — non scénographié |

*(Dimensions non renseignées pour les zones non encore auditées en détail — à compléter zone par
zone au moment de leur prise en charge, plutôt que devinées ici.)*

## 5. Écart principal identifié : Cinnabar sans bâtiments

**Constat central de cet audit** : `CinnabarIsland_hns` (72×44, tileset volcanique) n'a que 2
warps (Pokémon Center + un warp legacy) et aucune entrée vers Gym, Mansion ou Lab — alors que ces
trois bâtiments sont **narrativement essentiels** à l'Acte I et V (le Mansion est l'objectif de
Blue, le Gym est le lieu du joueur, le Lab porte la sous-intrigue des carnets de Blaine).

La géométrie intérieure existe déjà (réserve `_Frlg`, section 4) — la règle "ne pas recréer une
map réutilisable" s'applique pleinement ici : **le travail n'est pas de construire les
intérieurs, mais de construire les façades/portes sur la carte extérieure `_hns`** et de poser les
`warp_events` correspondants. C'est un travail de tilemap (pas de script), qui a été identifié
comme bloqué par l'absence de rendu visuel dans les sessions précédentes.

C'est la **priorité n°1 de `docs/map_design.md`** (Phase 1, premier blockout).

## 6. Limites de cet audit

- Dimensions/tilesets non vérifiés zone par zone au-delà de Cinnabar (fait par priorité : c'est
  la zone du premier chantier). À compléter avant chaque nouvelle phase.
- Aucun rendu visuel possible dans cet environnement (pas de Porymap, pas d'émulateur graphique).
  Toute affirmation sur l'agencement fin d'une carte (où sont les arbres, la plage, la route)
  vient de la lecture brute de `map.bin`/`border.bin` (décodage blockdata), jamais d'une capture
  d'écran, sauf mention contraire.
- La numérotation des routes suit celle du fork (HGSS Kanto post-game), pas nécessairement celle
  de FRLG — vérifiée mais à garder en tête si le brief narratif utilise une numérotation FRLG par
  endroits.
