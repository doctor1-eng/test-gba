# Heart & Soul — Notes d'implémentation

Ce fichier journalise l'audit technique et les décisions prises pendant l'intégration
de la spec Heart & Soul (`docs/heart_and_soul/docs/histoire.md`, source de vérité narrative)
dans ce dépôt. Il est mis à jour à chaque étape, jamais réécrit rétroactivement.

## Fork et build (Phase 0)

- **Dépôt** : `PokemonHnS-Development/pokehns-expansion`, tag `Release-v2.0.4`
  (commit `98574d291b5f82526e12de1800d50616d2238dc2`).
- **Lignée** : pret/pokeemerald → resetes12/Modern Emerald → rh-hideout/pokeemerald-expansion
  → PokemonHnS-Development/pokehns-expansion. Remote `upstream` = pokehns-expansion,
  `origin` = ce dépôt (`doctor1-eng/test-gba`), historique complet poussé (496 Mo, ~21 000 commits)
  pour permettre de futurs `git pull upstream` propres.
- **Nature du jeu de base** : ce n'est PAS un fork nu de pokeemerald-expansion, c'est un
  remake GSC / demake HGSS complet (Johto), qui embarque en plus tout le contenu vanilla
  Emerald/FRLG (Hoenn + Kanto), un randomiseur gen1-9, et un système de rendez-vous/rematch.
  Kanto y existe déjà, mais comme contenu **post-game** de l'aventure Johto, pas comme
  point de départ.
- **Toolchain** : `gcc-arm-none-eabi` 13.2.1, `binutils-arm-none-eabi`, `libnewlib-arm-none-eabi`,
  `libpng-dev` (paquets Ubuntu 24.04 standards, aucune version custom nécessaire).
- **Build** : `make hns -j<nproc>` → cible `pokehns.gba` / `pokehns.elf` / `pokehns.map`.
- **BASELINE BUILD: PASS** — 0 erreur, 430 warnings (pré-existants, non liés à Heart & Soul),
  ROM 32 Mo (94.38 % de la ROM utilisée, 94.47 % EWRAM, 78.37 % IWRAM — marge de manœuvre
  réduite, à surveiller à chaque ajout de contenu).

## Convention de scripting réelle : PAS de Poryscript actif

Le template fourni (`docs/heart_and_soul/scripts/*.pory`) suppose du Poryscript. Ce n'est
**pas** ce que ce fork utilise en pratique :

- La règle de compilation `data/%.inc: data/%.pory` est **commentée** dans le `Makefile`
  (section `# poryscript`), tout comme `%.pory: ;` et la variable `$(SCRIPT)`.
- `tools/poryscript/` ne contient que les fichiers de config (`command_config.json`,
  `font_config.json`) — pas le binaire ni les sources du compilateur.
- Sur ~1436 fichiers `scripts.inc` dans `data/maps/`, un seul fichier `.pory` résiduel
  existe (`data/maps/NewSinjoh_hns/scripts.pory`), qui n'est plus utilisé par la chaîne de
  build.

**Conséquence** : tous les scripts Heart & Soul seront écrits directement en macros natives
pokeemerald (`.inc`), pas en syntaxe Poryscript. Les templates du zip serviront de base
logique (structure des dialogues, embranchements) mais seront réécrits à la main dans la
syntaxe réelle. Exemple de convention confirmée (`data/maps/CinnabarIsland_hns/scripts.inc`,
`data/maps/PewterCity_Gym_hns/scripts.inc`, `data/maps/NewBarkTown_Lab_hns/scripts.inc`) :

```
CinnabarIsland_EventScript_Blaine::
    lock
    faceplayer
    msgbox CinnabarIsland_Text_Blaine, MSGBOX_DEFAULT
    closemessage
    fadescreenswapbuffers FADE_TO_BLACK
    setflag FLAG_HIDE_CINNABAR_BLAINE
    removeobject LOCALID_CINNABAR_BLAINE
    fadescreenswapbuffers FADE_FROM_BLACK
    release
    end
```

Combat dresseur :
```
trainerbattle_single TRAINER_JERRY_HNS, PewterCity_Gym_Text_CamperJerry_Seen, PewterCity_Gym_Text_CamperJerry_Beaten
```
```
trainerbattle_no_intro TRAINER_BROCK_HNS, PewterCity_Gym_Text_Brock_WinLoss
```

Don de Pokémon (paramètres réels, tous positionnels et obligatoires) :
```
givemon PLAYER_STARTER_SPECIES, 5, ITEM_NONE, BALL_POKE, NATURE_RANDOM, NUM_ABILITY_PERSONALITY, \
    MON_GENDER_RANDOM, 0, 0, 0, 0, 0, 0, USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, \
    USE_RANDOM_IVS, USE_RANDOM_IVS, USE_RANDOM_IVS, MOVE_DEFAULT, MOVE_DEFAULT, MOVE_DEFAULT, \
    MOVE_DEFAULT, SHINY_MODE_NEVER
```

Données de dresseur (format `.party`, lisible, compilé via `trainerproc`) — `src/data/trainers_hns.party` :
```
=== TRAINER_BROCK_HNS ===
Name: BROCK
Class: Leader Kanto Hns
Pic: Leader Brock Hns
Gender: Male
Music: Hg Boy 1
Double Battle: No
AI: Basic Trainer
Mugshot: Orange
Items: Full Restore / Full Restore

Golem @ Quick Claw
Level: 66
IVs: 24 HP / 24 Atk / 24 Def / 24 SpA / 24 SpD / 24 Spe
- Curse
- Stone Edge
- Body Slam
- Earthquake
```
→ Les lieutenants et boss Heart & Soul (Lyre, Selen, Terrence, Kess, Mira Voss, Blue, Kaïn)
seront déclarés dans `src/data/trainers_hns.party`, pas en `.h` à la main.

Le sélecteur de starter Johto (`NewBarkTown_Lab_hns/scripts.inc`) utilise un objet-événement
par Pokémon posé sur la carte (le joueur marche jusqu'au bon Pokéball), pas un menu.
Ce motif ne passe pas à l'échelle pour ~20 choix par type (section 8 du brief) : il faudra
un vrai menu paginé. Reste à identifier l'équivalent natif du fork pour une liste scrollable
(`multichoicegrid` / liste PC / relearner de capacités) avant d'écrire `intro_choix_type`.
**Point ouvert, à trancher avant d'implémenter la sélection de type.**

## Flags / Vars — convention réelle

- `include/constants/flags.h` inclut conditionnellement `flags_hns.h` (`#if IS_HNS`) —
  c'est le fichier où vivent tous les flags scénaristiques.
- Convention du fork : les flags ne s'ajoutent **pas** en fin de fichier avec de nouvelles
  valeurs — ils réutilisent les créneaux `FLAG_UNUSED_0xNNN` déjà réservés dans la table
  (`include/constants/flags_hns.h`, ~3522 lignes, dont une plage libre autour de
  `0x920`–`0x95F`). Chaque nouveau flag Heart & Soul doit remplacer un
  `FLAG_UNUSED_0xNNN` existant, à la même valeur numérique, pas être inventé à une adresse
  arbitraire.
- Même logique pour les vars : `include/constants/vars_hns.h` (260 lignes) est le fichier
  vars spécifique HnS, séparé de `vars.h`/`vars_frlg.h`.
- **Aucun flag/var Heart & Soul n'a encore été déclaré.** La matrice de la section 6 du
  brief sera mappée sur des créneaux `UNUSED` réels au moment de l'implémentation, avec la
  liste exacte des créneaux consommés consignée ici.

## Pokédex

Confirmé : Pokédex national complet jusqu'à Gen 8 inclus (`SPECIES_DRAGAPULT` présent,
`SPECIES_MIMIKYU` présent). L'hypothèse de la section 6 de `histoire.md`
("le fork utilise le Pokédex national complet") est validée — les 18 tableaux de Pokémon
niveau 34 par type sont utilisables sans retrait d'espèce.

## ⚠️ Conflit narratif — Blue et Blaine sont déjà des PNJ canoniques à Cinnabar

`data/maps/CinnabarIsland_hns/scripts.inc` contient déjà :
- `CinnabarIsland_EventScript_Blaine` : Blaine, présent et vivant, promet de reconstruire
  l'Arène de Cinnabar, renvoie vers le Dojo. Flags `FLAG_HIDE_CINNABAR_BLAINE`,
  `FLAG_HIDE_DOJO_BLAINE`.
- `CinnabarIsland_EventScript_Blue` / `_BlueActive` : Blue, ex-Champion de Johto (battu par
  "RED"), personnage neutre/amical, gated par `VAR_NUM_BADGES == 15`. Flags
  `FLAG_HIDE_CINNABAR_BLUE`, `FLAG_HIDE_VIRIDIAN_BLUE`.

Ceci **contredit frontalement** la prémisse Heart & Soul (Blue chef de la Team Rocket,
attaque de Cinnabar, disparition de Blaine). Ce n'est pas un système large et interconnecté
— c'est localisé à ce seul fichier de script, donc réécrivable proprement — mais je ne le
modifie pas silencieusement.

Un système "Team Rocket au Kanto" existe déjà par ailleurs dans le fork, mais à une tout
autre échelle : `VAR_KANTO_ROCKET_STORY_STATE` ne pilote qu'une **micro-scène** sur Route 24
Johto (un Rocket importune un couple, état 4 → 5, ~10 lignes de script). Sans rapport avec
la prise de contrôle de Kanto par Blue décrite dans `histoire.md`. Pas un obstacle, juste à
ne pas confondre avec le système à construire.

**Décision requise avant script (voir message de session) :** réécrire directement ces
scripts Cinnabar pour la trame Heart & Soul, ou les gater derrière un flag de mode pour
préserver le contenu post-game Johto d'origine en parallèle ?

## ⚠️ Deux jeux de cartes Kanto distincts dans ce dépôt

Le fork contient **deux ensembles de maps Kanto qui ne sont pas le même Kanto** :

- **`*_Frlg`** (ex. `CinnabarIsland_Frlg`, `PewterCity_Frlg`, `Route1_Frlg`...) : Kanto complet
  et connecté sur la grille overworld (villes ↔ routes ↔ donjons), avec Arènes, Pokemon
  Mansion, Pokemon Lab, Silph Co., Zone Safari — tout y est. `CinnabarIsland_Frlg` a des
  warps directs vers `MAP_POKEMON_MANSION_1F`, `MAP_CINNABAR_ISLAND_GYM`,
  `MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE`.
- **`*_hns`** (ex. `CinnabarIsland_hns`, `PewterCity_hns`...) : versions retravaillées pour
  la structure du jeu HnS, mais **beaucoup plus minces** sur Cinnabar — `CinnabarIsland_hns`
  ne se connecte qu'à `Route20_hns`/`Route21_hns` (qui mènent vers Johto/New Bark Town) et
  à son Pokémon Center ; **pas de warp vers un Pokemon Mansion, une Arène ou un Lab en
  version `_hns`** (ces bâtiments n'existent qu'en `_Frlg`).

Autrement dit : le Kanto jouable et complet pour la prémisse Heart & Soul (Champion d'Arène
de Cinnabar, Pokemon Mansion comme ancien labo Rocket, etc.) est très probablement le jeu de
maps **`_Frlg`**, pas `_hns`. Reste à déterminer comment ce Kanto `_Frlg` est actuellement
relié à la progression Johto du jeu de base (rejoint-on ce Kanto en jouant normalement, ou
est-ce un contenu largement indépendant/désactivé ?) avant de bâtir la structure open world
dessus.

**Décision requise avant Phase 2 (voir message de session).**

## Décisions utilisateur (2026-08-29)

1. **Conflit Blue/Blaine** : réécriture directe des scripts `CinnabarIsland_hns` pour la
   trame Heart & Soul (le contenu Johto d'origine à cet endroit disparaît, reste dans
   l'historique `upstream`). Pas de gating par flag de mode.
2. **Cible Kanto** : `_hns`, malgré son incomplétude à Cinnabar (pas de Mansion/Gym/Lab).
   Corroboré après coup par un indice technique : dans `flags_hns.h`, tout le contenu
   d'objets cachés/état propre à `_Frlg` (des centaines de `FLAG_HIDE_SILPH_CO_*`,
   `FLAG_HIDE_SEVEN_ISLAND_*`, etc.) est câblé à la valeur littérale `0` — signe que `_Frlg`
   est traité comme inerte dans cette variante. Audit de connectivité complémentaire :
   `_hns` s'avère être un Kanto complet et connecté (Pallet↔Route1↔Viridian↔...↔Saffron↔
   Celadon, graphe overworld cohérent sur toutes les villes testées) — seule Cinnabar
   manque de warps vers des bâtiments. Voir `technical_map.md` pour le plan de réutilisation
   de la géométrie `_Frlg` (Mansion/Gym/Lab) par nouveaux warps depuis `CinnabarIsland_hns`.

## ⚠️ Piège découvert : les noms `FLAG_UNUSED_0xNNN` de `flags_hns.h` ne sont PAS tous des créneaux libres

Avant d'allouer les flags Heart & Soul, un piège a été identifié et évité : une grande partie
des `#define FLAG_UNUSED_0xNNN` dispersés dans `flags_hns.h` (plusieurs centaines, sections
`0x022`–`0x0FF`, `0x1AA`+, `0x2xx`, `0x4xx`, `0x8xx`, `0x9xx`) ont pour valeur **la constante
littérale `0`**, pas l'adresse indiquée par leur propre nom. Ce ne sont pas des créneaux
disponibles — écrire un `setflag` dessus reviendrait à cibler le flag système 0 pour tous,
un bug garanti. Ce sont des restes de la table de flags FRLG d'origine, neutralisés en bloc
et conservés uniquement pour la lisibilité des diffs avec l'amont. **Ne jamais leur faire
confiance sans vérifier leur valeur réelle.**

Le seul registre fiable pour ajouter du contenu neuf est documenté explicitement dans le
fichier lui-même : bloc "Extended content flags" (`HNS_EXTENDED_CONTENT_START = 0x36A`,
300 créneaux consommés jusqu'à l'ajout ci-dessous, plage `0x496–0x4FF` explicitement
réservée pour extension future par un commentaire des mainteneurs). Vérifié par comptage des
usages réels de la macro `(HNS_EXTENDED_CONTENT_START + N)` avant toute allocation.

## Registre Heart & Soul alloué (Phase 1)

Déclaré dans `include/constants/flags_hns.h`, à la suite du bloc "extended content"
existant (`HNS_EXTENDED_CONTENT_START + 300` à `+323`, adresses `0x496`–`0x4AD`) ;
`HNS_EXTENDED_CONTENT_COUNT` mis à jour de `300` à `324`. Build revérifié après ajout :
**PASS, 0 erreur**.

| Flag | Valeur |
|---|---|
| `FLAG_ATTAQUE_CINNABAR_LANCEE` | `HNS_EXTENDED_CONTENT_START + 300` |
| `FLAG_BLAINE_DISPARU` | `+301` |
| `FLAG_BLAINE_SAUVE` | `+302` |
| `FLAG_REFUGIES_GUIDES` | `+303` |
| `FLAG_REFUGIES_CACHES` | `+304` |
| `FLAG_ACTE_1_TERMINE` | `+305` |
| `FLAG_CINNABAR_VERROUILLEE` | `+306` |
| `FLAG_LYRE_VAINCUE` | `+307` |
| `FLAG_SELEN_VAINCUE` | `+308` |
| `FLAG_MIRA_VOSS_VAINCUE` | `+309` |
| `FLAG_TERRENCE_RESOLU` | `+310` |
| `FLAG_TERRENCE_CONVAINCU` | `+311` |
| `FLAG_TERRENCE_COMBAT_ALLEGE` | `+312` |
| `FLAG_TERRENCE_VAINCU` | `+313` |
| `FLAG_KESS_RESOLUE` | `+314` |
| `FLAG_KESS_CONVAINCUE` | `+315` |
| `FLAG_KESS_COMBAT_ALLEGE` | `+316` |
| `FLAG_KESS_VAINCUE` | `+317` |
| `FLAG_DOCUMENTS_MIRA_VOSS_LUS` | `+318` |
| `FLAG_ACTE_5_DEBLOQUE` | `+319` |
| `FLAG_BLUE_VAINCU` | `+320` |
| `FLAG_EPILOGUE_JOUE` | `+321` |
| `FLAG_ZONE_BONUS_DEBLOQUEE` | `+322` |
| `FLAG_KAIN_VAINCU` | `+323` |

Vars déclarées dans `include/constants/vars_hns.h`, créneaux `VAR_UNUSED_HNS_0x40D8`–
`0x40DE` (valeurs réelles, pas le piège ci-dessus — `vars_hns.h` encode bien l'adresse en
littéral) :

| Var | Valeur |
|---|---|
| `VAR_TYPE_CHOISI` | `0x40D8` |
| `VAR_TEAM_SLOT` | `0x40D9` |
| `VAR_TEMP_SPECIES` | `0x40DA` |
| `VAR_REPUTATION` | `0x40DB` |
| `VAR_PERSUASION_TERRENCE` | `0x40DC` |
| `VAR_PERSUASION_KESS` | `0x40DD` |
| `VAR_DIALOGUE_BLUE` | `0x40DE` |

Aucune de ces valeurs ne doit être modifiée sans mettre à jour ce tableau et
`HNS_EXTENDED_CONTENT_COUNT` en conséquence.

## Phase 2 — Vertical slice (choix du type → Acte I → fuite)

Implémenté, compilé (`make hns` : **PASS, 0 erreur**), non testé visuellement (voir
"Limite de test" plus bas).

### Point d'entrée de nouvelle partie

`src/new_game.c`, fonction `WarpToTruck()`, branche `IS_HNS` : modifiée pour faire démarrer
toute nouvelle sauvegarde dans `CinnabarIsland_PokemonCenter_hns` (coordonnées `7,8`, reprises
du warp existant qui relie déjà ce Pokémon Center à l'extérieur — donc garanties praticables,
pas de nouvelle géométrie devinée) au lieu de la maison du joueur à New Bark Town. **C'est le
changement au plus grand rayon d'impact de cette session** : il redéfinit ce qui se passe pour
toute nouvelle partie sur cette ROM. Documenté ici en évidence plutôt que noyé dans le diff.

### Choix du type et de l'équipe (`data/scripts/heart_and_soul_intro.inc`, généré puis relu)

- `CinnabarIsland_PokemonCenter_hns_MapScripts` (`OnTransition`) lance
  `HeartSoul_EventScript_ChooseType` la toute première fois (`VAR_TYPE_CHOISI == 0`, vars à 0
  par défaut sur une sauvegarde neuve) — pas de nouveau flag nécessaire pour ce garde-fou.
- Menu des 18 types via le système `dynmultipush`/`dynmultistack` (liste déroulante réelle,
  scroll natif inclus — voir découverte technique ci-dessous), pas un `multichoice` statique
  qui ne supporte pas plus de quelques lignes sans le casser visuellement.
- **Seul le type Feu mène à une sélection réelle** (les 20 Pokémon de la section 6 de
  `histoire.md`, vérifiés un par un contre `include/constants/species.h`, y compris les 3
  formes d'Alola qui utilisent le suffixe réel `_ALOLA` et non `_ALOLAN` comme on aurait pu le
  deviner — `SPECIES_NINETALES_ALOLA`, `SPECIES_SANDSLASH_ALOLA`, `SPECIES_MAROWAK_ALOLA`).
  Les 17 autres types affichent "Ce type n'est pas encore disponible" et renvoient au choix du
  type — jamais de blocage, mais pas non plus une fausse liste. C'est exactement la portée que
  le zip lui-même annonçait ("Feu en exemple complet, 17 types à dupliquer") : le mécanisme est
  prouvé et prêt à dupliquer, la duplication réelle reste à faire.
- Sélection de 4 Pokémon distincts : sous-script `HeartSoul_EventScript_PickOne_Feu` réutilisé
  4 fois via `call`/`return`, avec un garde anti-doublon par `compare`/`goto_if_eq` qui rouvre
  la liste si le joueur choisit deux fois le même Pokémon (pas de filtrage dynamique de la
  liste — plus simple, zéro risque, suffisant pour la contrainte "aucun doublon").
- `givemon` niveau 34 pour les 4 choix, mêmes paramètres que le starter picker `_hns` existant
  (`NewBarkTown_Lab_hns/scripts.inc`) — convention réutilisée à l'identique.

### Découverte technique : système de liste déroulante réelle déjà présent dans le moteur

`ScrCmd_dynmultichoice`/`dynmultipush` (`src/scrcmd.c`, macro `dynmultistack` dans
`asm/macros/event.inc`) construit un menu à défilement (`ListMenu` + flèches de scroll) à
partir d'éléments poussés un par un depuis le script, avec un id numérique par entrée récupéré
dans `VAR_RESULT`. Déjà utilisé en production dans `data/scripts/debug.inc`. C'est la bonne
brique pour tout menu de plus de ~8 entrées dans ce fork — le `multichoice` statique classique
ne scroll pas et casse visuellement au-delà d'une poignée d'options (fenêtre dimensionnée à
`count * 2` tuiles sans limite). Aucune modification native (C) n'a été nécessaire : tout est
scriptable avec les commandes existantes.

### Acte I — attaque de Cinnabar (`data/scripts/heart_and_soul_act1.inc`)

`CinnabarIsland_hns_MapScripts` (`OnTransition`) déclenche `HeartSoul_EventScript_CinnabarAttack`
au premier passage sur la map extérieure une fois l'équipe choisie
(`VAR_TYPE_CHOISI != 0` et `FLAG_ATTAQUE_CINNABAR_LANCEE` non posé).

Séquence : narration de l'attaque → Blaine disparaît (`removeobject LOCALID_CINNABAR_BLAINE`,
`FLAG_BLAINE_DISPARU`) → choix réfugiés (guider = `+2` réputation /
cacher = `+1`, via `dynmultipush`/`dynmultistack`, `FLAG_REFUGIES_GUIDES` ou
`FLAG_REFUGIES_CACHES`) → `FLAG_CINNABAR_VERROUILLEE` + `FLAG_ACTE_1_TERMINE` → message de
fuite vers Route 21.

**Simplifications volontaires de cette passe, documentées plutôt que cachées :**
- Le choix des réfugiés est présenté par narration directe (pas de PNJ dédié placé sur la
  carte) : ça évite de deviner des coordonnées d'objet-événement sans pouvoir vérifier
  visuellement le résultat dans un émulateur. Une vraie rencontre de PNJ reste une amélioration
  naturelle, pas un mensonge sur ce qui existe.
- La fuite vers Route 21/Pallet/Route 1 ne fait l'objet d'aucun `warp` scripté : la carte
  `CinnabarIsland_hns` se connecte déjà nativement à `Route21_hns` (`map.json`, connexion non
  modifiée) — le joueur sort simplement par le bord nord de la carte, comme dans le jeu de
  base. Aucune coordonnée inventée.
- `FLAG_CINNABAR_VERROUILLEE` est posé mais **le blocage effectif du retour n'est pas encore
  implémenté** (bloquer une connexion de bord de carte demande une modification de collision
  ou un script de garde que je n'ai pas pu vérifier visuellement dans cet environnement sans
  écran). Le critère de réussite de la section 7 du brief ("atteindre le continent sans
  softlock") est rempli ; "empêcher physiquement de revenir" reste un travail futur signalé
  ici, pas silencieusement oublié.
- Blue est masqué dès `CinnabarIsland_OnTransition` (n'apparaît plus jamais comme PNJ amical à
  Cinnabar) plutôt que déplacé ou re-scripté ailleurs dans cette passe — son rôle de chef de la
  Team Rocket (Acte V) reste à écrire.

### Bug corrigé après premier retour de test : blocage au tout premier lancement

Retour utilisateur : le jeu se bloquait juste après la création de personnage (choix du
sexe, nom, options), au moment du transport dans le jeu.

**Cause réelle** : `HeartSoul_EventScript_ChooseType` (menu interactif `dynmultichoice`,
`lockall`) était déclenché depuis `MAP_SCRIPT_ON_TRANSITION` sur
`CinnabarIsland_PokemonCenter_hns`. Or `include/constants/map_scripts.h` documente
explicitement ce point d'accroche comme tournant *pendant* le chargement de la carte, avant
l'affichage — "Used to set map-specific flags/vars... update object positions", pas pour
ouvrir des fenêtres de menu. Un vrai événement interactif au premier chargement doit passer
par `MAP_SCRIPT_ON_FRAME_TABLE` ("Run every frame after the map has faded in, before player
input is processed... trigger an event"), confirmé par un exemple réel du fork
(`AzaleaTown_hns/scripts.inc`, `AzaleaTown_EventScript_GSBall`, qui fait `lock`/`msgbox`/
`giveitem` depuis ce point d'accroche sans problème).

**Correctif** :
- `CinnabarIsland_PokemonCenter_hns/scripts.inc` : le déclenchement passe par
  `map_script MAP_SCRIPT_ON_FRAME_TABLE` + `map_script_2 VAR_TYPE_CHOISI, 0, ...` (même
  motif que l'exemple ci-dessus) au lieu de `ON_TRANSITION`.
- `CinnabarIsland_hns/scripts.inc` : le déclenchement redondant de l'attaque depuis
  `ON_TRANSITION` (même bug) a été retiré — inutile de toute façon, la chaîne
  `ChooseTeam_Feu → CinnabarAttack` s'enchaîne déjà dans la même exécution de script
  (`goto`, pas de rechargement de carte entre les deux).
- `HeartSoul_EventScript_CinnabarAttack` se termine maintenant par `end` et non `return` :
  il est atteint par `goto`, pas `call`, depuis `heart_and_soul_intro.inc` — un `return` sans
  `call` correspondant aurait dépilé une adresse de retour invalide et provoqué un second
  blocage/comportement erratique juste après la séquence d'attaque.

`make hns` : PASS, 0 erreur après correctif. Toujours non testé en émulateur de mon côté.

### Limite de test importante

Cette session n'a pas d'accès à un émulateur avec écran (mGBA headless présent
via `tools/mgba/mgba-rom-test`, mais c'est un framework de tests unitaires de commandes de
script, pas un moyen de jouer la séquence à l'écran). **Tout ce qui précède est vérifié par
compilation (0 erreur, ROM générée) et relecture attentive du script, pas par une partie
jouée.** Aucun softlock connu dans la logique relue, mais une vraie session de jeu (section 19
du brief) reste à faire avant de considérer l'Acte I "terminé" au sens de la règle anti-bugs.

## Retour de test n°2 : noms français, choix des attaques, chaussures de course

Trois demandes suite au premier test réussi (les 4 Feu se choisissent bien) :

- **Noms français des Pokémon** : les 20 noms de la liste Feu (`HeartSoul_Text_Mon_Feu_*`)
  étaient en anglais. Remplacés par les noms officiels français, vérifiés un par un par
  recherche web plutôt que de mémoire (4 des 20 noms que j'avais initialement en tête
  étaient en fait ceux de la pré-évolution, pas de l'espèce demandée — Infernape n'est pas
  Ouisticram [Chimchar] mais Simiabraz, Simisear n'est pas Flamajou [Pansear] mais Flamoutan,
  Pyroar n'est pas resté "Pyroar" mais Némélios, Magcargo n'est pas Limagma [Slugma] mais
  Volcaropod — bon rappel que "je pense m'en souvenir" n'est pas une vérification).
  **Limite honnête** : seuls les noms de Pokémon et les textes que j'ai écrits sont en
  français. Les noms d'attaques affichés par le sélecteur de capacités (voir ci-dessous)
  restent en anglais — ce fork ne contient aucune table de texte française pour les
  attaques/objets/etc. (vérifié à l'audit initial), donc les traduire dépasserait largement
  cette tâche (il faudrait importer une table de traduction complète, absente du projet).
- **Choix des attaques** : réutilise le Move Relearner déjà présent dans le moteur
  (`src/move_relearner.c`, primitives `setmoverelearnerstate` / `chooseboxmon
  SELECT_PC_MON_MOVE_RELEARNER` / `special HasMovesToRelearn` / `special
  TeachMoveRelearnerMove`), sur le même modèle que le vrai Tuteur de Capacités de
  Blackthorn City (`BlackthornCity_House3_hns/scripts.inc`) — sans le coût en Écaille Cœur.
  Après les 4 `givemon`, le joueur choisit un de ses 4 nouveaux Pokémon, remplace une
  attaque par une autre apprise par niveau, répète pour n'importe lequel de ses 4 Pokémon
  autant de fois qu'il veut, puis quitte (bouton B / "annuler" sur l'écran de choix du
  Pokémon) pour enchaîner sur l'attaque de Cinnabar. Pas un flow "exactement 4 attaques une
  fois" figé : le joueur peut ajuster librement chacun des 4 emplacements de chacun de ses 4
  Pokémon avant de continuer — couvre la demande sans réinventer une UI (réutilisation totale
  du moteur existant).
- **Chaussures de course** : `setflag FLAG_RECEIVED_RUNNING_SHOES` + `setflag
  FLAG_SYS_B_DASH` juste après les `givemon`, même paire de flags que celle utilisée pour
  cette fonctionnalité ailleurs dans le code (`data/scripts/debug.inc`).

`make hns` : PASS, 0 erreur après ces changements.

## Statut

Phase 0 (build baseline) : **terminée, PASS**.
Phase audit (section 3 du brief) : **terminée**.
Phase 1 (registre flags/vars) : **terminée, PASS**.
Phase 2 (vertical slice, type Feu uniquement) : **compilée, PASS ; non testée en jeu**.
Prochaine étape : dupliquer le mécanisme Feu vers les 17 autres types (mécanique, la
structure est prouvée), puis playtest réel en émulateur, puis Actes II+.
