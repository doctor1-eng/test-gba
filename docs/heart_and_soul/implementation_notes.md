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
- **Les 18 types mènent désormais à une sélection réelle** (mise à jour du retour de test
  n°3 — voir section dédiée plus bas). Chaque type a sa propre paire de scripts
  `HeartSoul_EventScript_PickOne_{type}` / `HeartSoul_EventScript_ChooseTeam_{type}`, générée
  avec exactement la même structure que Feu, y compris les formes spéciales qui utilisent le
  suffixe réel `_ALOLA` et non `_ALOLAN` comme on aurait pu le deviner —
  `SPECIES_NINETALES_ALOLA`, `SPECIES_SANDSLASH_ALOLA`, `SPECIES_MAROWAK_ALOLA`.
- Sélection de 4 Pokémon distincts par type : sous-script `HeartSoul_EventScript_PickOne_{type}`
  réutilisé 4 fois via `call`/`return`, avec un garde anti-doublon par `compare`/`goto_if_eq`
  qui rouvre la liste si le joueur choisit deux fois le même Pokémon (pas de filtrage
  dynamique de la liste — plus simple, zéro risque, suffisant pour la contrainte "aucun
  doublon").
- `givemon` niveau 34 pour les 4 choix, mêmes paramètres que le starter picker `_hns` existant
  (`NewBarkTown_Lab_hns/scripts.inc`) — convention réutilisée à l'identique, pour les 18 types.

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
- ~~La fuite vers Route 21/Pallet/Route 1 ne fait l'objet d'aucun `warp` scripté...~~ **Erreur
  corrigée au retour de test n°5** (voir section dédiée plus bas) : cette hypothèse initiale
  était fausse et bloquait réellement la progression. `Route21_hns`/`Route20_hns` sont des
  connexions à 100 % en eau (comme la Route 21 canonique) — sans CS Surf, impossible à
  traverser à pied en tout début de partie. Remplacé par un `warp` scripté.
- ~~`FLAG_CINNABAR_VERROUILLEE` est posé mais le blocage effectif du retour n'est pas encore
  implémenté...~~ **Fait au retour de test n°5** (voir plus bas).
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

## Retour de test n°3 : duplication du mécanisme Feu aux 17 autres types

Suite à "continue le process", extension de `heart_and_soul_intro.inc` du seul type Feu
(vertical slice initiale) aux 18 types complets.

- **Génération** : script Python (`gen_intro_full.py`, scratchpad de session) qui produit,
  pour chacun des 18 types, la même structure exacte que Feu (`PickOne_{type}` /
  `ChooseTeam_{type}`, garde anti-doublon, `givemon` niveau 34, chaînage vers le sélecteur
  d'attaques puis vers `HeartSoul_EventScript_CinnabarAttack`), à partir des listes d'espèces
  par type de la section 6 de `histoire.md` vérifiées contre `include/constants/species.h`.
  Le menu `HeartSoul_EventScript_ChooseType` (`switch VAR_TYPE_CHOISI`) pointe maintenant vers
  les 18 `ChooseTeam_{type}` réels ; il n'y a plus de branche "type pas encore disponible".
- **Noms français** : les ~200 espèces couvrant les 17 nouveaux types ont été vérifiées une
  par une par recherche web (pas de traduction par lot ni par mémoire), après l'expérience du
  retour n°2 où 4 noms mémorisés sur 20 s'étaient révélés être ceux de la pré-évolution. Aucun
  nom manquant en sortie du générateur (`missing french names: []`).
- Aucune nouvelle mécanique introduite dans cette passe : uniquement de la duplication
  structurelle du modèle Feu déjà validé, plus la traduction. `data/scripts/heart_and_soul_act1.inc`,
  les hooks de map et le reste de la logique Acte I sont inchangés.

`make hns -j4` : PASS, 0 erreur. ROM à 94.44 % (contre 94.39 % avant l'extension — la marge
avant le plafond de 32 Mo reste confortable malgré ~18x plus de contenu d'espèces/texte).
Toujours **non testé en émulateur avec écran** (même limite que les retours précédents) :
vérifié par compilation propre et relecture du fichier généré (nombre de scripts,
disparition du placeholder), pas par une partie jouée.

## Statut

Phase 0 (build baseline) : **terminée, PASS**.
Phase audit (section 3 du brief) : **terminée**.
Phase 1 (registre flags/vars) : **terminée, PASS**.
Phase 2 (vertical slice, type Feu uniquement) : **compilée, PASS ; non testée en jeu**.
Phase 2bis (18 types complets, noms français, choix des attaques, chaussures de course) :
**compilée, PASS ; non testée en jeu**.
Phase 2ter (correctif menu Pokémon, Acte II - doute de Pierre/Ondine) : **compilée, PASS ;
non testée en jeu**.
Phase 2quater (correctif bug bloquant de fuite de Cinnabar, blocage du retour) : **compilée,
PASS ; corrige un bug bloquant confirmé par l'utilisateur, fuite confirmée fonctionnelle**.
Phase 2quinquies (Blue masqué à Argenta, dresseurs Cinnabar→Azuria relevés niveau 34,
sous-intrigues Route 1/Viridian/Forêt de Jade/Azuria, intro Chen sautée, 5 Poké Balls de
départ) : **compilée, PASS ; fuite confirmée, dialogue Route 1 remonté en échec**.
Phase 2sexies (correctif dialogues post-combat Quinn/Doug/Pierre/Ondine, Pokémon sauvages
Cinnabar→Azuria relevés niveau 34, Pokégear/carte dès le départ, capacités de terrain
utilisables sans badge) : **compilée, PASS ; corrige les bugs confirmés Quinn/Pierre**.
Phase 2septies (CT + bicyclette dès le départ, histoire avancée jusqu'à la 4e arène/Major Bob
à Carmin-sur-Mer, traduction française complète de tous les dialogues PNJ de Cinnabar à
Vermilion) : **compilée, PASS ; non testée en jeu**.
Phase 2octies (cannes à pêche de départ, vrais magasins à Viridian/Pewter/Cerulean/
Vermilion) : **compilée, PASS ; non testée en jeu**. Voir sections dédiées ci-dessous.
**Warps des bâtiments de Cinnabar (Arène/Manoir/Labo)** : fait entre-temps par une session
parallèle sur cette même branche (commits `d524e40`-`7ba52e2`, fusionnés sans conflit dans
`2c7f519`) — voir `docs/map_design.md` et `docs/world_map.md` pour le détail de cette autre
piste de travail. Point auparavant bloqué de notre côté (pas de rendu visuel disponible),
débloqué par une méthode différente (décodage direct des comportements de metatile +
flood-fill sur la grille de collision, plutôt qu'une capture d'écran) — statut : blockout
compilé, PASS, **audit technique de connectivité/collision étendu aux étages intérieurs
complété et PASS** (2026-08-30, voir section dédiée plus bas et `technical_map.md`),
**non testé en jeu** (validation visuelle toujours ouverte), limites documentées dans
`MAP_TEST_README.md` (notamment : ne pas entrer dans le Pokémon Center dans le build de test
dédié `MAPTEST=1`, car ça déclenche le script d'attaque de l'Acte I) et dans
`technical_map.md` (2 warps orphelins inatteignables, sans impact joueur).
Prochaine étape : playtest complet du chemin Cinnabar → Vermilion (priorité : confirmer que
la traduction ne casse rien visuellement, texte trop long pour une fenêtre par exemple ; et
que les magasins vendent bien les bons objets), puis validation en jeu des portes Cinnabar
ci-dessus, puis suite de l'histoire après la 4e arène (Acte III : Forêt de Jade/Mont
Sélénite/Route de la Centrale en monde ouvert avec les
lieutenants).

## Retour de test n°4 : entrée "Pokémon" absente du menu START

Bug remonté par l'utilisateur : après avoir choisi son équipe de 4, aucun moyen de la
consulter depuis le menu START (pas d'entrée "Pokémon").

**Cause** : `BuildNormalStartMenu` (`src/start_menu.c:363`) n'ajoute l'entrée
`MENU_ACTION_POKEMON` que si `FLAG_SYS_POKEMON_GET` est posé. Ce flag est mis par le flow
normal du starter picker (`NewBarkTown_Lab_hns/scripts.inc:244`), mais jamais par le flow
Heart & Soul, qui donne les 4 Pokémon directement via `givemon` sans passer par ce script.

**Correctif** : ajout de `setflag FLAG_SYS_POKEMON_GET` juste après les 4 `givemon`, dans
les 18 `HeartSoul_EventScript_ChooseTeam_{type}` de `heart_and_soul_intro.inc` (même endroit
que `FLAG_RECEIVED_RUNNING_SHOES`). `make hns -j4` : PASS, 0 erreur.

## Acte II — premier contact avec Pierre et Ondine (`data/scripts/heart_and_soul_act2.inc`)

Section 2 de `histoire.md` : "rejoindre le continent, convaincre les premiers Champions
(Pierre, Ondine) de l'ampleur de la menace. Ils doutent : 'tu n'as pas su protéger ta propre
ville.'"

- Deux sous-scripts `call`/`return` (`HeartSoul_EventScript_PierreDoute`,
  `HeartSoul_EventScript_OndineDoute`), insérés par un simple `call` dans les scripts
  existants de `PewterCity_Gym_hns/scripts.inc` (`EventScript_Brock`) et
  `CeruleanCity_Gym_hns/scripts.inc` (`EventScript_Misty`), juste après le
  `goto_if_set FLAG_DEFEATED_*` de garde et avant le `msgbox` d'intro normal. Le combat de
  badge lui-même n'est pas modifié : le joueur enchaîne directement dessus après la scène de
  doute.
- Chaque scène ne se déclenche qu'une fois (`FLAG_PIERRE_CONVAINCU` /
  `FLAG_ONDINE_CONVAINCUE`), et seulement une fois `FLAG_ACTE_1_TERMINE` posé — sinon le
  combat de badge se déroule normalement, sans texte H&S (couvre le cas où quelqu'un joue une
  sauvegarde sans être passé par Cinnabar).
- `FLAG_ACTE_2_TERMINE` est posé dès que les deux flags de conviction sont actifs, peu
  importe l'ordre (Pewter et Cerulean sont accessibles dans n'importe quel ordre depuis
  Viridian) — vérifié via `call_if_set` symétrique dans les deux sous-scripts.
- **Aucun point de réputation ajouté ici** : le barème de la section 9 de `histoire.md` ne
  liste pas ce moment (contrairement aux choix d'Acte I déjà câblés) — pas d'invention de
  règle non spécifiée par le brief.
- Simplification volontaire : les deux scènes sont de la narration à sens unique (pas de
  choix multiple pour le joueur), le combat qui suit sert de "preuve" implicite. Le brief ne
  spécifie pas de mécanique de dialogue à choix pour cette scène précise (contrairement à
  Terrence/Kess, section 3) — rien n'a donc été inventé au-delà de ce que demande le texte.

`make hns -j4` : PASS, 0 erreur après ces deux changements. ROM à 94.44 %.

## Retour de test n°5 : impossible de quitter Cinnabar (bug bloquant) + blocage du retour

Remonté par l'utilisateur : après l'attaque de Cinnabar, plus aucun moyen de continuer
l'histoire, au point de devoir envisager un CS Fly de debug pour sortir de l'île.

**Cause réelle** : hypothèse fausse documentée (et maintenant corrigée) dans la section
Acte I ci-dessus. `HeartSoul_CinnabarAttack_Verrouillage` affichait le message de fuite puis
relâchait simplement le joueur (`releaseall`/`end`) en supposant qu'il pourrait sortir par le
bord nord de la carte via la connexion `CinnabarIsland_hns` → `Route21_hns` (`map.json`). Or
cette connexion, comme `Route20_hns`, est entièrement entourée d'eau (vérifié via
`data/maps/Route21_hns/map.json` : aucun `warp_events`, uniquement des `connections` ; la
Route 21 canonique est intégralement maritime) — sans CS Surf, impossible à traverser en tout
début de partie. Le joueur restait donc réellement bloqué sur l'île malgré le texte annonçant
un départ en bateau.

**Correctif** :
- `HeartSoul_CinnabarAttack_Verrouillage` fait maintenant un `warp` scripté direct vers
  `MAP_PALLET_TOWN_HNS, 6, 8` après le message de fuite (fade to black inclus), au lieu de
  compter sur un déplacement du joueur. Coordonnée **vérifiée, pas devinée** : lue directement
  dans `data/layouts/PalletTown_hns/map.bin` (décodage manuel du format blockdata pokeemerald,
  2 octets/case) — collision `0` (case franchissable) sur toute la rangée `y=8` autour de
  `x=6`, élévation cohérente avec le reste de la rue extérieure ; à comparer aux cases
  `y=5-7` juste au-dessus (collision `1`, la porte de la maison de Red et ses abords) qui,
  elles, auraient été risquées. Aucun rendu visuel disponible dans cet environnement, mais
  cette vérification par les données brutes du layout est une preuve, pas une supposition.
- Texte `HeartSoul_Text_FuiteRoute21` ajusté en conséquence (« un dernier bateau attend au
  port » plutôt que « fuir par la Route 21 », qui laissait croire à tort à un trajet à pied).
- **Blocage physique du retour à Cinnabar**, maintenant réellement implémenté :
  `CinnabarIsland_hns_MapScripts` gagne un `MAP_SCRIPT_ON_FRAME_TABLE` (même motif que
  `CinnabarIsland_PokemonCenter_hns` et `CeruleanCity_Gym_hns_OnFrame`, `VAR_TEMP_0, 0` comme
  condition toujours vraie) qui appelle `HeartSoul_EventScript_CinnabarVerrouilleeCheck`
  (`heart_and_soul_act1.inc`) : si `FLAG_CINNABAR_VERROUILLEE` est posé et
  `FLAG_ACTE_5_DEBLOQUE` ne l'est pas encore, le joueur est renvoyé au même point d'arrivée
  sur le continent avec un message expliquant pourquoi, plutôt que laissé face à un mur d'eau
  incompréhensible. Le garde sur `FLAG_ACTE_5_DEBLOQUE` (déjà réservé dans le registre de
  flags) anticipe le retour à Cinnabar prévu à l'Acte V sans qu'il faille revenir modifier ce
  script plus tard.

**Limite honnête, toujours d'actualité** : les warps vers les bâtiments de Cinnabar
(Arène/Manoir/Labo) ne sont **toujours pas faits**. Contrairement au bug ci-dessus, ce n'est
pas une question de logique de script mais d'édition de tilemap (ajouter `warp_events` sur
`CinnabarIsland_hns` à des coordonnées qui doivent correspondre à une porte de bâtiment
dessinée sur la carte) — un rendu visuel de la carte est nécessaire pour ne pas placer un
warp invisible sur une case incohérente avec le décor. Je n'ai pas cette capacité dans
l'environnement actuel (pas de porymap, pas de rendu déjà exporté). Piste proposée à
l'utilisateur : une capture d'écran de la carte extérieure de Cinnabar depuis son émulateur
donnerait une vérité de terrain suffisante pour faire cette dernière passe sans deviner.

`make hns -j4` : PASS, 0 erreur. ROM à 94.44 %.

## Résolution : Blue masqué à l'Arène d'Argenta (Viridian)

Décision utilisateur sur le conflit signalé au retour de test n°6 : **cacher Blue, Arène
fermée** (plutôt que garder le combat avec un texte réécrit, ou laisser tel quel).

- `ViridianCity_Gym_hns_MapScripts` gagne un `MAP_SCRIPT_ON_TRANSITION` (`setflag` simple,
  pas de `msgbox` — même règle que `CinnabarIsland_OnTransition`) qui pose
  `FLAG_HIDE_VIRIDIAN_BLUE` tant que `FLAG_ACTE_5_DEBLOQUE` n'est pas posé. Ce flag existait
  déjà, câblé nativement sur l'objet-événement de Blue dans
  `ViridianCity_Gym_hns/map.json` — aucune nouvelle géométrie, juste une condition de garde
  supplémentaire en amont.
- Texte du PNJ guide de l'Arène (`ViridianCity_Gym_Text_Guide`) réécrit en français : annonce
  la fermeture de l'Arène plutôt que le combat, cohérent avec l'absence du Champion.
- Le combat, le badge et le reste du script `ViridianCity_Gym_EventScript_Blue` restent
  intacts dans le code, simplement inaccessibles tant que `FLAG_ACTE_5_DEBLOQUE` n'est pas
  posé (flag déjà réservé dans le registre, pas encore posé nulle part — Acte V pas encore
  écrit).

`make hns -j4` : PASS, 0 erreur. ROM à 94.44 %.

## Sous-intrigue Route 1 — l'objet perdu (section 8 de `histoire.md`)

Point de barème chiffré en section 9 (« Route 1, objet perdu | Le rendre à la famille | +1 |
Ignorer | 0 ») implémenté en réutilisant des éléments existants plutôt qu'en devinant de la
géométrie :

- Le joueur trouve un bracelet en fuyant Cinnabar (`FLAG_OBJET_PERDU_TROUVE`, narré au même
  endroit que le reste de la séquence de fuite dans `heart_and_soul_act1.inc`).
- `Route1_hns/scripts.inc` : `Quinn` (dresseuse « Cooltrainer F » déjà présente sur Route 1,
  déjà relevée au niveau 34) est reflavorée en milice locale plutôt que d'ajouter un nouveau
  PNJ à des coordonnées devinées. Après son combat existant (message de post-combat déjà
  fonctionnel, inchangé), un nouveau sous-script `call`/`return`
  (`HeartSoul_EventScript_MiliceRoute1`, `heart_and_soul_act2.inc`) propose de rendre le
  bracelet (+1 réputation, `FLAG_OBJET_PERDU_RENDU`) ou de le garder (0).
- **Choix de placement volontaire** : inséré après le message de post-combat existant
  (`msgbox ... AfterBattle`), jamais avant `trainerbattle_single`. Aucun exemple trouvé dans
  ce fork d'un `msgbox`/`dynmultichoice` précédant un `trainerbattle_single` déclenché par la
  vue du joueur (contrairement à `trainerbattle_no_intro`, prévu pour ça) — plutôt que de
  parier sur un comportement non vérifié après le bug de blocage déjà rencontré une fois
  cette session, le choix a été placé à un endroit dont le fonctionnement est déjà prouvé.
- Les dialogues de combat déjà existants de Quinn (Seen/Beaten) restent en anglais,
  conformément à la limite déjà posée au retour de test n°2 : seuls les textes écrits pour
  Heart & Soul sont traduits, pas l'ensemble des dialogues du jeu de base.

`make hns -j4` : PASS, 0 erreur. ROM à 94.45 %.

## Retour de test n°6 : dresseurs surclassés (Brock niveau 66, etc.)

Fuite confirmée fonctionnelle par l'utilisateur. Demande suivante : remettre les dresseurs au
niveau de l'équipe du joueur (34).

**Constat** : les dresseurs `_hns` de Kanto sont conçus pour le postgame façon HGSS (après un
run complet de Johto), pas pour un début de partie. Exemples relevés dans
`src/data/trainers_hns.party` avant correctif : `TRAINER_BROCK_HNS` = 6 Pokémon niveau **66**
avec objets et mouvements compétitifs ; `TRAINER_ARNOLD_HNS` (Route 21, rencontré juste après
la fuite de Cinnabar) = niveau **61**. Totalement injouable face à une équipe de 4 Pokémon
niveau 34 fraîchement obtenue.

**Correctif** : script Python identifiant, via les appels `trainerbattle` réels (pas une
supposition) dans les `scripts.inc` de toutes les cartes du chemin actuellement construit
(Route 20/21, Pallet, Route 1, Viridian + Arène, Route 2, Forêt de Jade, Argenta + Arène,
Route 3, Mont Sélénite, Route 4, Azuria + Arène), les 28 dresseurs réellement placés sur ce
chemin. Chaque `Level:` de chacun de leurs Pokémon mis à `34` dans
`src/data/trainers_hns.party` (85 lignes modifiées), sans toucher à la taille des équipes, aux
objets, mouvements ou IVs.

**Trouvaille signalée pour suite, résolue juste après** (voir section dédiée juste au-dessus,
« Blue masqué à l'Arène d'Argenta ») : `TRAINER_BLUE_HNS` était le Champion d'Arène d'Argenta
(`ViridianCity_Gym_hns`) dans le jeu de base — un PNJ amical qui donne un badge, en conflit
direct avec Blue antagoniste de Heart & Soul.

## Retour de test n°7 : dialogue Route 1 non déclenché + confort de test + suite des sous-intrigues

Retour utilisateur : « Le dialogue ne se lance pas automatiquement après le combat » (Quinn,
Route 1). Demandes associées : sauter le discours du Professeur Chen pour les prochains
tests, donner 5 Poké Balls de départ, puis continuer les sous-intrigues avant le prochain
envoi de ROM.

**Sur le dialogue Route 1** : relecture complète de `Route1_EventScript_Quinn` et
`HeartSoul_EventScript_MiliceRoute1` — structure identique au motif déjà prouvé fonctionnel
dans `heart_and_soul_act1.inc` (deux `msgbox ..., MSGBOX_DEFAULT` enchaînés, puis
`dynmultipush`/`dynmultistack`), rien d'anormal trouvé côté script. **Hypothèse la plus
probable, non a confirmé** : le test a été fait sur une sauvegarde qui avait déjà dépassé la
scène de fuite de Cinnabar *avant* que `FLAG_OBJET_PERDU_TROUVE` n'existe dans le ROM testé —
ce flag n'est posé qu'une fois, au moment précis de la scène de fuite
(`HeartSoul_CinnabarAttack_Verrouillage`), donc une sauvegarde qui a déjà dépassé ce point sur
une version antérieure du ROM ne peut plus jamais l'obtenir rétroactivement. Résultat observé :
le combat et le message de post-combat s'affichent (inchangés), mais
`HeartSoul_EventScript_MiliceRoute1` se termine immédiatement sur son garde
`goto_if_unset FLAG_OBJET_PERDU_TROUVE` sans rien afficher — silencieux, donc perçu comme
« le dialogue ne se lance pas ». Pas de correctif de code appliqué sur cette hypothèse (rien
d'anormal identifié à corriger) ; **à confirmer par un test sur une sauvegarde neuve**, ce que
les deux changements suivants doivent justement faciliter.

**Confort de test** :
- `src/oak_speech_hns.c` : le discours du Professeur Chen (« This is a Pokemon » / « And you
  are... ») est sauté ; la partie démarre directement sur l'écran de choix du genre. Voir le
  commentaire dans le code pour le détail technique (nouvel état
  `Task_NewGameHnsSpeech_SkipToGender` qui reproduit exactement les affectations que l'état
  sauté aurait faites, sans animation ni texte).
- `heart_and_soul_intro.inc` : `giveitem ITEM_POKE_BALL, 5` ajouté aux 18
  `ChooseTeam_{type}`, même quantité que le flow starter Johto normal
  (`NewBarkTown_Lab_hns/scripts.inc:497`).

**Suite des sous-intrigues** (section 8/9 de `histoire.md`), toutes chiffrées au barème et
toutes sur le chemin déjà construit (Cinnabar → Azuria) :
- Viridian, commerçant résistant (+1) : PNJ « Cooltrainer M » du Mart (`ViridianCity_Mart_hns`,
  texte déjà existant mentionnant Cinnabar) reflavoré.
- Forêt de Jade, bûcherons déplacés (+1) : Doug (« Bug Catcher », déjà relevé niveau 34)
  reflavoré, même motif que Quinn (inséré après le message de post-combat existant).
- Azuria, canalisations sabotées (+1) : le « Boy » de Cerulean (flavor « SuperNerd »)
  reflavoré.

Chaque sous-intrigue suit exactement le même squelette : flag « résolu » posé dans les deux
issues (aider/ignorer) pour éviter un déclenchement répété, `+1` réputation seulement sur
l'issue positive, sous-script `call`/`return` dans `heart_and_soul_act2.inc`. Les PNJ à
interaction classique (`MSGBOX_NPC`, pas de `trainerbattle`) n'ont aucune contrainte de
placement particulière ; ceux à `trainerbattle_single` (Quinn, Doug) gardent la règle établie
au retour de test n°6 : le nouveau contenu est inséré après le message de post-combat
existant, jamais avant.

`make hns -j4` : PASS, 0 erreur, à chaque étape. ROM à 94.44-94.45 %.

## Retour de test n°8 : dialogues post-combat non déclenchés — cause réelle identifiée

Confirmation utilisateur du retour n°7 : le dialogue de Quinn ne se lance toujours pas
automatiquement (il faut lui reparler après le combat, l'ancien dialogue anglais s'affichant
en premier avant le nôtre), et Pierre a un problème symétrique (notre dialogue s'affiche bien
la première fois, suivi du dialogue anglais d'origine ; mais après une défaite, seul le
dialogue anglais se relance).

**Cause réelle, identifiée dans `data/scripts/trainer_battle.inc` (pas une supposition)** :
`trainerbattle_single`, sur une **première victoire**, passe par `EventScript_EndTrainerBattle`
→ `gotobeatenscript` — qui saute directement au **4ᵉ argument `event_script`** de
`trainerbattle_single` s'il est fourni, sinon à un script générique silencieux
(`EventScript_TryGetTrainerScript`), puis `releaseall`/`end`. **`gotopostbattlescript`** (qui
reprend la ligne suivant directement `trainerbattle_single` dans le script — ce que j'avais
supposé à tort) n'est utilisé que sur le chemin « dresseur déjà vaincu, on lui reparle »
(`EventScript_NoNormalTrainerBattle`). Autrement dit : sans le 4ᵉ argument, tout ce qui suit
`trainerbattle_single` dans un script (mon `msgbox`+`call` inclus) est **inatteignable à la
première victoire**, et ne s'exécute qu'au rappel — exactement le symptôme remonté.

**Correctif** : `Route1_EventScript_Quinn` et `Route2_EventScript_Doug` passent maintenant
leur logique de suite (message de post-combat + sous-intrigue) en 4ᵉ argument
(`event_script`) de `trainerbattle_single`, exactement le motif déjà présent dans ce fork pour
`Route2_EventScript_RegisterRob` (enregistrement du numéro de Rob, qui lui fonctionnait déjà
car câblé de cette façon dès le départ — la preuve que ce motif est correct était donc déjà
dans le dépôt, il suffisait de le suivre).

**Pierre/Ondine, cause différente** : `trainerbattle_no_intro` (utilisé par Brock/Misty,
combat déclenché par interaction directe, pas par la vue du joueur) enchaîne bien
automatiquement sur la suite du script après une victoire
(`EventScript_DoNoIntroTrainerBattle` → `dotrainerbattle` → `gotopostbattlescript`
inconditionnel). Le vrai problème : `HeartSoul_EventScript_PierreDoute` posait
`FLAG_PIERRE_CONVAINCU` dès l'affichage du dialogue, **avant** le combat qui suit — donc dès
la première tentative, que le combat soit gagné ou perdu. Sur une défaite (le joueur est
white-out, pas de suite de script possible, mais le flag reste posé), une nouvelle tentative
sautait directement la scène de doute (déjà « vue ») pour aller droit au dialogue anglais
d'origine. Corrigé en déplaçant la pose du flag hors du sous-script de dialogue, vers deux
nouveaux sous-scripts (`HeartSoul_EventScript_PierreConvaincu` /
`_OndineConvaincue`) appelés uniquement dans la branche de victoire du combat de badge (aux
côtés de `FLAG_DEFEATED_PEWTER_GYM`/`FLAG_DEFEATED_CERULEAN_GYM`) : la scène de doute se
rejoue désormais à chaque tentative tant que le combat n'est pas gagné, ce qui colle au texte
(« Prouve-le sur le terrain »).

## Confort de jeu supplémentaire (retour de test n°8)

Trois demandes complémentaires, toutes livrées dans le même lot :

- **Pokémon sauvages relevés niveau 34** : même méthode que pour les dresseurs (retour de
  test n°6) — script Python ciblant les 13 cartes du chemin déjà construit dans
  `src/data/wild_encounters.json` (12 avaient une table de rencontres ; `MAP_MT_MOON_OUTSIDE_HNS`
  n'en a aucune, rien à faire). Un exemple de l'ampleur du problème avant correctif : Route 21
  proposait des Spearow/Rattata niveau 45-47 et des Ekans niveau 48-50, alors que le joueur
  vient d'obtenir une équipe de niveau 34 quelques minutes plus tôt. `min_level`/`max_level`
  de chaque emplacement (herbe, eau, pêche, Rock Smash) mis à `34` — 606 emplacements sur les
  12 cartes concernées. Format JSON revérifié par un aller-retour parse/dump sans diff avant
  la vraie modification, pour ne changer que les valeurs, jamais la mise en forme.
- **Pokégear (« la carte ») dès le départ** : ce fork (base HGSS) n'a pas d'objet « Carte »
  indépendant distribué par un PNJ sur les cartes `_hns` — seule la variante `_Frlg` inerte
  (déjà documentée comme non jouable, voir audit initial) en donne un. La carte régionale fait
  partie du Pokégear/Pokénav dans ce jeu. Les 3 flags que
  `NewBarkTown_PlayersHouse_1F_hns` pose normalement pour le donner
  (`FLAG_SYS_POKENAV_GET`, `FLAG_HAS_MATCH_CALL`, `FLAG_RECEIVED_POKENAV`) sont désormais posés
  avec le reste du kit de départ (Poké Balls, chaussures de course) dans les 18
  `ChooseTeam_{type}`.
- **Capacités de terrain utilisables sans badge** : `src/field_move.c` centralise la
  vérification de déblocage de chaque capacité de terrain (Coupe, Flash, Force, Surf, Vol,
  Plongée, Cascade, Rock Smash) via `gFieldMoveInfo[].isUnlockedFunc` — les 8 fonctions
  `IsFieldMoveUnlocked_X` retournent maintenant `TRUE` sans condition pour `IS_HNS` au lieu de
  vérifier `FLAG_BADGEnn_GET`. Côté script, `data/scripts/field_move_scripts_hns.inc` avait
  aussi ses propres vérifications de badge redondantes lors de l'interaction avec un objet de
  la carte (arbre à couper, rocher à fracasser/pousser, cascade, tourbillon) — les 5
  `goto_if_unset FLAG_BADGEnn_GET` retirés. Dans les deux cas, seule la condition du badge saute
  : posséder la CT et l'avoir enseignée à un Pokémon reste requis (mécanique normale
  conservée).

`make hns -j4` : PASS, 0 erreur à chaque étape. ROM à 94.45 %.

## Retour de test n°9 : CT/bicyclette, histoire jusqu'à la 4e arène, traduction complète

Suite à la confirmation que les correctifs Quinn/Pierre/Doug fonctionnent (« Perdre exprès
une fois → reparler à Pierre : (Fonctionne) »), trois demandes : des événements avant Pierre,
la bicyclette activée directement, l'histoire avancée jusqu'à la 4e Arène, et la traduction
française de toutes les lignes de dialogue de PNJ avant cette 4e Arène.

**CT et bicyclette dès le départ** : les 8 CT de capacités de terrain (Coupe, Vol, Surf,
Force, Flash, Rock Smash, Cascade, Tourbillon) et la Bicyclette (`ITEM_BICYCLE`, l'objet du
Magasin de Vélos de ce fork HGSS) ajoutées au kit de départ dans les 18 `ChooseTeam_{type}`.
Root cause du signalement « la carte pour voler ne s'active pas correctement » : le badge
n'était déjà plus requis (retour n°8), mais aucune CT n'avait jamais été donnée, donc aucun
Pokémon ne pouvait connaître Vol. Deux flags supplémentaires posés
(`FLAG_RECEIVED_HM_CUT`/`FLAG_RECEIVED_HM_ROCK_SMASH`, réels dans `flags_hns.h` mais jamais
posés par aucun script `_hns` puisque le PNJ qui les pose normalement n'existe qu'en Hoenn).

**Histoire jusqu'à la 4e Arène** : chemin vérifié sans blocage (`map.json`, aucun garde/flag) :
Azuria → Route 5 → Safrania (simple passage) → Route 6 → Carmin-sur-Mer. 6 dresseurs et 211
emplacements de rencontres sauvages sur ces 4 cartes relevés niveau 34 (Major Bob/Lt. Surge
était niveau 57). Major Bob reçoit le même traitement que Pierre/Ondine (scène de doute,
`HeartSoul_EventScript_MajorBobDoute`/`_Convaincu`, flag posé uniquement à la victoire).
Sous-intrigue « interception radio » (section 8/9, +1 réputation) implémentée en réutilisant
le PNJ « Nerd » existant de Carmin-sur-Mer — simplification documentée : le brief décrit un
mini-jeu d'écoute, remplacé par un choix aider/ignorer faute de mini-jeu existant à réutiliser
sans risque. Petit événement ajouté à Pallet Town (« le monde d'avant », section 8, sans point
de barème) sur le PNJ « Woman » existant.

**Traduction française complète (Cinnabar → Vermilion)** : chantier de grande ampleur (~45
fichiers de cartes, environ 1300 lignes `.string` au total) délégué à 4 agents en parallèle,
chacun sur un groupe de cartes disjoint (Cinnabar/Pallet/Route1/Viridian ;
Route2/Forêt de Jade/Pewter/Route3 ; Mont Sélénite/Route4/Azuria ;
Route5/Safrania/Route6/Vermilion), avec des règles strictes communes : ne toucher que le
texte entre guillemets, préserver exactement les codes de contrôle (`\n`/`\l`/`\p`/`$`),
jamais traduire les noms d'espèces/capacités/objets (aucune table française n'existe pour
eux dans ce fork), garder les noms de villes/routes en anglais (cohérence avec les bannières
de carte, non retouchées), et remplacer BROCK/MISTY/SURGE par leurs noms officiels français
PIERRE/ONDINE/MAJOR BOB (pas une invention : ce sont les vraies traductions Pokémon
officielles), cohérent avec les scènes de doute déjà écrites sous ces noms. Un balayage final
(recherche de mots anglais courants sur les ~45 fichiers) ne remonte plus aucune ligne
suspecte, à une exception documentée et volontaire près : le bloc de dialogue de Blue à
l'Arène d'Argenta (`ViridianCity_Gym_Text_LeaderBlue_*`) reste en anglais, car ce contenu est
actuellement inaccessible en jeu (Blue masqué jusqu'à l'Acte V) — pas de valeur à le traduire
maintenant, sera fait avec le reste du contenu de l'Acte V.

`make hns -j4` : PASS, 0 erreur, vérifié après chaque lot puis sur l'état final consolidé.
ROM à 94.46 %.

## Retour de test n°10 : cannes à pêche et magasins vides

Demande : cannes à pêche et objets achetables dans chaque magasin.

**Cannes à pêche** : les 3 cannes (`ITEM_OLD_ROD`/`ITEM_GOOD_ROD`/`ITEM_SUPER_ROD`) ajoutées
au kit de départ des 18 `ChooseTeam_{type}`. Aucun script `_hns` de Kanto n'en donne (la seule
occurrence de `ITEM_OLD_ROD` dans tout le dépôt est à `Route32_PokemonCenter_hns`, en Johto —
jamais atteignable dans une partie Heart & Soul qui reste entièrement à Kanto).

**Magasins vides, cause réelle trouvée** : aucun des 4 Marts de Viridian/Pewter/Cerulean/
Vermilion ne vendait quoi que ce soit. Cause identifiée dans `map.json` (pas une supposition)
: le PNJ « Clerk » déjà placé sur chacune des 4 cartes pointait vers
`Cherrygrove_Pokemart_EventScript_Clerk` — le vendeur de Cherrygrove City, une ville de
**Johto**, dont le stock de démarrage ne contient que Potion/Antidote, sans lien avec la
ville visitée. Vraisemblablement un placeholder resté de la génération initiale des cartes
Kanto `_hns`, jamais complété (cohérent avec le reste de l'audit initial : ce Kanto est traité
comme contenu de postgame dans le jeu de base, où le joueur arrive déjà équipé depuis Johto et
n'a normalement pas besoin de ces magasins).

**Correctif** : chaque PNJ Clerk repointé (même objet-événement, seul le script visé change
dans `map.json`) vers un nouveau script de vendeur propre à la carte, avec l'inventaire réel
de cette ville — porté tel quel depuis la variante `_Frlg` inerte de ce même dépôt
(`PewterCity_Mart_Frlg`, etc., déjà conçue avec les bons objets par ville, jamais du contenu
inventé) :
- Argenta (Viridian) : Poké Ball, Potion, Antidote, Anti-Paralysie.
- Jadielle (Pewter) : + Réveil, Anti-Brûlure, Corde Sortie, Repousse.
- Azuria (Cerulean) : + Super Potion (en tête de liste, avant Potion).
- Carmin-sur-Mer (Vermilion) : Poké Ball, Super Potion, Antidote, Anti-Paralysie, Réveil,
  Anti-Gel, Repousse.

`make hns -j4` : PASS, 0 erreur. ROM à 94.47 %.

## Note : travail parallèle fusionné (warps Cinnabar)

En parallèle de ce retour n°10, une autre session a poussé sur cette même branche le
blockout des portes Arène/Manoir/Labo sur `CinnabarIsland_hns` (voir `docs/map_design.md`,
`docs/world_map.md`, `MAP_TEST_README.md`) — le point que ce journal documentait comme
bloqué faute de rendu visuel. Fusionné sans conflit (fichiers disjoints des miens) dans le
commit `2c7f519e2f`. Détail technique et statut de test : voir ces documents dédiés plutôt que
dupliqué ici.

## Audit technique approfondi des bâtiments de Cinnabar (2026-08-30)

Chantier demandé explicitement : pousser l'audit des portes Gym/Manoir/Labo au-delà des
seules portes d'entrée (2F/3F/B1F du Manoir, 3 salles du Labo) avant de considérer ce
blockout comme complet. Méthode : lecture de tous les `warp_events` des `map.json` concernés
(9 maps : `PokemonMansion_{1F,2F,3F,B1F}_Frlg`, `CinnabarIsland_PokemonLab_{Entrance,Lounge,
ResearchRoom,ExperimentRoom}_Frlg`, `CinnabarIsland_Gym_Frlg`, `CinnabarIsland_hns`) +
décodage binaire de chaque `map.bin` (format `MAPGRID_METATILE_ID_MASK`/`COLLISION_MASK`/
`ELEVATION_MASK` de `include/global.fieldmap.h`) pour vérifier la collision réelle sous
chaque tuile de warp.

**Résultat** : les 3 chaînes de warps (Manoir 1F↔2F↔3F↔B1F, Labo Entrance↔3 salles, Gym) sont
internement cohérentes — chaque `dest_warp_id` pointe vers l'index correct dans le tableau
de la map cible, aucune map de destination erronée, aucune tuile d'arrivée effectivement
utilisée par le joueur qui soit bloquée. Confirme et complète (sans le contredire)
`MAP_TEST_README.md`.

**2 warps orphelins trouvés** (tuile de collision non nulle → physiquement inatteignables,
donc sans impact joueur) : `PokemonMansion_1F` warp 6 `(35,34)` et warp 9 `(11,13)`, hérités
tels quels de la géométrie vanilla FRLG du Manoir (portes/jumeaux d'élévation qui servaient
à d'autres agencements dans le jeu d'origine). Documentés comme LIMITATION CONNUE dans
`technical_map.md`, non corrigés : les supprimer demanderait de renuméroter tous les
`dest_warp_id` qui référencent ces tableaux par index dans plusieurs fichiers, un risque de
régression plus grand que le bénéfice pour du contenu jamais atteint en jeu (règle « ne pas
sur-corriger »).

`docs/heart_and_soul/technical_map.md` mis à jour en conséquence (les lignes Cinnabar/
Manoir/Gym/Labo indiquaient encore par erreur ce travail comme non fait).

Build de contrôle sur cette session (environnement neuf, toolchain installée à l'identique
de la Phase 0 : `gcc-arm-none-eabi` 13.2.1 via `apt`) : `make hns -j$(nproc)` → **PASS, 0
erreur**, ROM 33 554 432 octets, EWRAM 94.47 %, IWRAM 78.37 %, ROM 94.47 % — chiffres
identiques à la baseline Phase 0, confirmant que le build reste reproductible et que rien ne
s'est dégradé depuis.

Aucune donnée de map modifiée dans cet audit (lecture seule) ; seule la documentation a été
mise à jour.

## Acte III — premier lieutenant scripté : Lyre (Forêt de Jade)

Chantier ouvert (docs/heart_and_soul/docs/histoire.md section 3 : Viridian Forest/Mont
Sélénite/Rock Tunnel, open world, ordre libre). Décision de méthode : les 3 lieutenants
traités un par un (« travail par lots »), pas en bloc — Lyre d'abord, la plus simple des
trois (combat direct, pas de mécanique de conviction contrairement à Terrence).

**Constat avant script** : contrairement à Route1/Route2 (Quinn, Doug, Ed — tous des
Bug Catcher génériques déjà présents, reflavorés), `ViridianForest_hns` ne contient aucun
PNJ dresseur "boss" réutilisable pour incarner une lieutenante régionale de la TEAM ROCKET.
Décision : nouvel `object_event`, sprite `OBJ_EVENT_GFX_ROCKET_F_HNS` (déjà présent dans les
graphismes du jeu de base, jamais assigné à un PNJ nommé dans ce fork — pas de graphisme
inventé). Position `(46,40)` choisie par le même procédé que l'audit Cinnabar : décodage de
`data/layouts/ViridianForest_hns/map.bin`, recherche d'une case de collision 0 avec ses 8
voisines également à 0, non occupée par un autre `object_event`/`warp_event`.

**Trainer** : `TRAINER_LYRE_HNS` ajouté en fin de liste (`include/constants/opponents_hns.h`,
id `631`, `TRAINERS_COUNT_HNS` `631`→`632`) — jamais inséré au milieu, le fichier documente
lui-même pourquoi (décalerait les flags de victoire de tous les dresseurs suivants). Équipe
dans `src/data/trainers_hns.party` : Venomoth/Beedrill/Ariados/Parasect niveau 34 (même
convention plate que Brock/tout le reste du jeu), thème Bug/Poison + statut-piège
(Stun Spore/Sleep Powder/Spider Web/Spore) cohérent avec "guérilla, pièges et embuscades"
(histoire.md section 3). Classe `Rocket Admin Hns` / pic `Rocket Grunt F Hns`, déjà utilisés
ailleurs dans ce fork pour d'autres PNJ Rocket — pas de nouvelle classe inventée.

**Script** (`data/scripts/heart_and_soul_act3.inc`, nouveau fichier, ajouté à
`data/event_scripts.s`) : suit exactement le patron `Route1_EventScript_Quinn` /
`Route1_EventScript_QuinnPostBattle` déjà validé (`trainerbattle_single` avec le 4e argument
`event_script`, `special PlayerFaceTrainerAfterBattle` + `waitmovement 0` avant le message de
victoire, flag posé uniquement dans la branche de victoire). **Décision assumée** : le combat
n'est pas conditionné par un flag d'acte, comme aucun combat de ce fork ne l'est (seul le
contenu narratif qui suit un combat l'est ailleurs, ex. `HeartSoul_EventScript_MiliceRoute1`
après Quinn) — Lyre est donc combattable dès que le joueur atteint la Forêt de Jade, y
compris avant la fin officielle de l'Acte II. Simplification délibérée, pas un oubli.

Build de contrôle : `make hns -j$(nproc)` → **PASS, 0 erreur**, symbole
`ViridianForest_EventScript_Lyre` confirmé présent dans `pokehns.map`. ROM 33 554 432 octets
(inchangé), 94.47 % ROM/EWRAM, 78.37 % IWRAM.

**Non testé en jeu** (validation visuelle toujours indisponible côté agent) : positionnement
réel de Lyre sur la carte, dialogue à l'écran, équilibrage du combat.

**Reste à faire pour clore Acte III** : Selen (Mont Sélénite) et Terrence (Rock Tunnel, avec
sa mécanique de conviction à 3 choix, `VAR_PERSUASION_TERRENCE` déjà réservée en Phase 1) —
même méthode (nouvel object_event, sprite Rocket, position vérifiée par collision), à traiter
en chantiers séparés.
