# Heart & Soul — Map Design (méthode, philosophie, feuille de route)

Ce document fixe la méthode de travail du Map Director pour Heart & Soul et propose la
feuille de route de construction du monde, à partir des constats de `docs/world_map.md`.
**Aucune map n'a été modifiée pour produire ce document** — c'est une proposition, pas un
blockout déjà réalisé.

## 1. Contrainte technique n°1 : pas de rendu visuel dans cet environnement

Confirmé à l'audit (`docs/world_map.md` section 1) : Porymap n'est pas utilisable ici (pas
d'affichage), et `tools/mgba/mgba-rom-test` est un harnais de tests de script, pas un émulateur
graphique. **Toute la philosophie « vérifier visuellement avant de placer un warp » (règle
26/31 du brief) doit donc être adaptée** : elle reste vraie dans l'esprit (ne jamais deviner),
mais la vérification passe par les données plutôt que par une capture d'écran, avec la capture
d'écran comme confirmation finale plutôt que comme préalable bloquant.

### Méthode de vérification sans rendu visuel

Pour toute édition de tilemap (ajout de porte de bâtiment, modification de terrain) :

1. **Décoder `map.bin` en clair.** Format confirmé (`include/global.fieldmap.h`) : 2 octets par
   case, little-endian — bits 0-9 = id de metatile, bits 10-11 = collision, bits 12-15 =
   élévation. Un script Python de ~15 lignes suffit à produire une grille lisible
   (id-metatile/collision) pour toute la carte, comme déjà fait avec succès sur `PalletTown_hns`
   pour vérifier une case de warp (`implementation_notes.md`, retour de test n°5).
2. **Identifier les metatiles de bâtiment déjà utilisés sur la carte.** Les ids de metatile
   « mur », « porte », « toit » du tileset `gTileset_Kanto_General_Hns` /
   `gTileset_Lavaridge_Hns` sont nommés dans les en-têtes générés (`src/data/tilesets/
   headers_kanto.h` ou équivalent `_hns`) — les repérer par nom plutôt que deviner un numéro.
3. **Croiser avec la collision/élévation** pour ne jamais poser une porte sur une case
   infranchissable ou en pleine mer (même méthode que la vérification du warp Pallet Town).
4. **Ne construire une façade que sur une zone de terrain plat, cohérente avec le décor
   existant** (pas au milieu de l'eau, pas en superposition d'un decor déjà posé) — vérifié par
   la grille décodée, pas par supposition.
5. **Demander une capture d'écran à l'utilisateur avant le commit final** quand la zone
   concernée est petite et critique (ex. porte de bâtiment) — pas comme préalable bloquant (on
   ne reste pas immobile faute de capture), mais comme dernière validation avant de considérer
   le travail « FINAL » plutôt que « BLOCKOUT/TEST ». Cohérent avec la Règle des 8 étapes de
   création (section 31 du brief) : blockout d'abord, validation ensuite.

Cette méthode n'est pas un contournement de la règle « ne jamais deviner » — elle en est une
application littérale avec les outils réellement disponibles ici.

## 2. Philosophie de level design retenue pour ce projet

Reprise du brief, condensée aux points qui pèsent réellement sur les décisions à venir :

- **5 questions par map** : où suis-je / pourquoi ce lieu existe / où puis-je aller / qu'est-ce
  qui m'intéresse / pourquoi revenir. Appliquées explicitly à chaque nouvelle zone dans les
  fiches de map (section 6).
- **Ne pas recréer ce qui existe** : la règle a déjà été respectée par la session précédente
  (réutilisation systématique de `_hns`, réserve `_Frlg` pour Cinnabar). Ce document la
  poursuit : les fiches de map (section 6) commencent toujours par « peut-on réutiliser ? »
  avant « faut-il construire ? ».
- **Priorité gameplay > esthétique** (règle 33 du brief) : particulièrement vrai ici puisque le
  scripting narratif est déjà avancé (jusqu'à la 4ᵉ arène, Carmin-sur-Mer) — casser un chemin
  scripté existant par une modification de map serait un régression, pas une amélioration.
  **Toute édition de tilemap sur une carte déjà scriptée doit être revérifiée contre
  `scripts.inc` et `implementation_notes.md`** avant modification (coordonnées d'object events,
  déclencheurs `MAP_SCRIPT_ON_FRAME_TABLE`, etc.).
- **Storytelling environnemental** (section 17-18 du brief) : c'est le vrai apport attendu de ce
  rôle par rapport au travail déjà fait. Le scripting a posé les flags et les dialogues ; aucune
  map ne montre encore visuellement l'attaque de Cinnabar, la présence Rocket, la reconstruction
  progressive. C'est le manque le plus net identifié par l'audit.
- **Cohérence régionale des tilesets** (section 13 du brief) : `Kanto_General_Hns` +
  `Lavaridge_Hns` pour Cinnabar confirme déjà une identité volcanique cohérente (Lavaridge =
  ville volcanique d'Hoenn, réutilisée comme tileset secondaire — choix pertinent, pas à
  changer).

## 3. Contrôle qualité — check-list active

Reprise et resserrée sur ce qui est vérifiable dans cet environnement (barré = non vérifiable
sans rendu, à reporter à la revue utilisateur) :

**Avant tout commit de map :**
- [ ] Warps : chaque nouveau `warp_events` a une destination existante et un `dest_warp_id`
      cohérent avec la carte cible (vérifié en lisant le `map.json` de la destination).
- [ ] Collision/élévation : aucune porte/passage posé sur une case de collision `1` ou en
      élévation incohérente avec les abords (vérifié par décodage `map.bin`).
- [ ] Connexions : toute nouvelle `connections` référence une carte qui existe réellement dans
      `layouts.json` et `map_groups.json`.
- [ ] Scripts existants non cassés : `scripts.inc` de la carte relu après modification de
      géométrie, coordonnées des `object_events` toujours cohérentes.
- [ ] Build : `make hns` PASS, 0 erreur, taille ROM revérifiée (marge 94-95 % déjà tendue).
- [ ] ~~Rendu visuel~~ : à défaut, capture d'écran demandée à l'utilisateur avant statut FINAL.

## 4. Feuille de route — réajustée à l'état réel du projet

L'ordre du brief (section 37) suppose un monde vierge de tout scripting. Ce n'est pas le cas
ici : Actes I et II sont déjà scriptés et partiellement testés jusqu'à Carmin-sur-Mer (4ᵉ
arène). **La feuille de route MAP ne réécrit pas ce travail** — elle comble ce qui manque
dessus, dans l'ordre narratif d'origine :

| Phase | Zones | Travail de map identifié | Statut |
|---|---|---|---|
| **1** | Cinnabar Island | Portes Gym/Mansion/Lab sur la carte extérieure `_hns` (aucune actuellement) | **Non commencé — chantier prioritaire, voir section 5** |
| 2 | Route 21 → Pallet → Route 1 | Rien côté géométrie (warp scripté déjà fonctionnel) ; mise en scène "contraste avant/après" à évaluer | À évaluer après Phase 1 |
| 3 | Viridian, Route 2, Forêt de Jade, Pewter | Repaire visuel de Lyre (Forêt de Jade) à concevoir ; le reste est scripté sans besoin de map | Non commencé |
| 4 | Route 3, Mont Sélénite, Route 4, Azuria | État "endommagé" de l'Arène d'Ondine (inondation, histoire.md section 8) — **actuellement aucune trace visuelle**, seulement narrée | Non commencé, écart identifié |
| 5 | Route 10/Rock Tunnel (Centrale), Carmin-sur-Mer | Repaire de Terrence, mineurs piégés — non scénographiés | Non commencé |
| 6 | Zone Safari, Céladopole, Safrania/Silph Co | Repaires de Kess et Mira Voss, structure interne de Silph Co à explorer (nombre d'étages `_hns` inconnu) | Non commencé |
| 7 | Retour à Cinnabar (Acte V), sous-sols du Mansion (post-game, Kaïn) | Cinnabar « après » (destruction visible), zone bonus | Dépend de la Phase 1 |

**Recommandation** : traiter la Phase 1 (Cinnabar) en premier, comme le demande le brief — c'est
aussi, indépendamment de l'ordre suggéré par le brief, le seul chantier de map déjà identifié et
documenté comme bloquant par la session précédente.

## 5. Chantier prioritaire — Cinnabar Island, blockout des bâtiments

### 5.1 Objectif narratif

Acte I : le joueur doit pouvoir accéder à son Arène, au Pokémon Mansion (objectif de Blue) et au
Lab (carnets de Blaine) **avant** l'attaque, pour que la chute de la ville ait un poids (le
joueur doit d'abord habiter ces lieux). Acte V : retour sur la même carte, dans un état dégradé.

### 5.2 Ce qui existe déjà (à ne pas recréer)

- Géométrie intérieure complète et réutilisable par warp externe : `MAP_POKEMON_MANSION_1F/2F/
  3F/B1F`, `MAP_CINNABAR_ISLAND_GYM`, `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE` (`_Frlg`,
  section 4 de `world_map.md`).
- Référence de composition (pas de coordonnées à copier telles quelles — la carte `_hns` est
  72×44, très différente de la `_Frlg` 24×20) : sur la version `_Frlg`, le Mansion est au
  nord-ouest, le Gym à l'est, le Lab au nord (proche du Mansion), le Pokémon Center et le Mart au
  sud-centre. Cet agencement relatif (Mansion+Lab groupés, Gym à part, PC central) est une bonne
  base de lecture pour composer la carte `_hns`, à confirmer contre le terrain réel décodé.
- Sur `_hns` : Pokémon Center déjà en place (warp existant, sud-est de la carte selon les
  coordonnées de warp 45,29), Blaine et la faune volcanique positionnés sur la moitié nord/est
  de la carte (coordonnées object_events entre x=25-61, y=9-32).

### 5.3 Plan d'action (blockout, pas encore exécuté)

1. Décoder `data/layouts/CinnabarIsland_hns/map.bin` (72×44) en grille metatile/collision.
2. Repérer les metatiles « bâtiment » déjà catalogués dans le tileset `Kanto_General_Hns` /
   `Lavaridge_Hns` (murs, toits, portes) par leurs noms dans les en-têtes générés.
3. Choisir 3 emplacements sur terrain plat et libre (pas de decor existant, pas d'eau), cohérents
   avec la disposition déjà en place (Blaine/PC à l'est-sud-est → Gym proche du PC, Mansion/Lab
   isolés côté nord/ouest en écho au canon).
4. Poser la géométrie de façade (tuiles) + un `warp_events` par bâtiment vers la bonne carte
   `_Frlg`, avec le `dest_warp_id` correspondant (déjà listés dans `world_map.md` section 5).
5. Revérifier que les nouveaux warps ne recouvrent aucun `object_events` existant (Blaine, Blue,
   faune) ni les coordonnées attendues par `scripts.inc` (`CinnabarIsland_hns/scripts.inc`).
6. `make hns`, PASS obligatoire avant tout envoi pour test.
7. Statut **BLOCKOUT** tant qu'aucune capture d'écran n'a confirmé le rendu ; **FINAL** ensuite.

### 5.4 Ce dont j'ai besoin avant d'exécuter

Rien de bloquant au sens strict (la méthode de la section 1 permet d'avancer sans capture
d'écran). Mais une capture d'écran de l'état actuel de `CinnabarIsland_hns` en jeu
accélérerait fortement la fiabilité du placement (zéro incertitude sur le décor déjà en place),
et reste la seule façon de faire passer ce chantier de BLOCKOUT à FINAL.

## 6. Format de fiche de map (pour chaque zone traitée à partir de maintenant)

```
# MAP REPORT
## MAP
## OBJECTIF NARRATIF (histoire.md)
## RÉUTILISATION (existe déjà / à réutiliser / à construire)
## DIMENSIONS / TILESET (vérifiés, pas estimés)
## CONNEXIONS / WARPS
## BÂTIMENTS
## POINTS D'INTÉRÊT
## STORYTELLING ENVIRONNEMENTAL (avant / pendant / après, si applicable)
## SCRIPTS EXISTANTS À NE PAS CASSER
## BUILD : PASS / FAIL
## VÉRIFICATION VISUELLE : capture reçue / en attente
## STATUT : BLOCKOUT / TEST / FINAL
```

## 7. Prochaine étape

En attente de validation avant d'exécuter le blockout Cinnabar (section 5) : confirmer que
l'ordre (Cinnabar d'abord), la méthode (décodage blockdata + capture d'écran en confirmation
finale, pas en préalable), et l'emplacement approximatif des 3 bâtiments conviennent avant
modification réelle de `CinnabarIsland_hns`.
