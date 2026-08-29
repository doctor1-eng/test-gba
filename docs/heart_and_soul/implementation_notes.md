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
PASS ; corrige un bug bloquant confirmé par l'utilisateur, fuite non testée en jeu après
correctif**. Voir sections dédiées ci-dessus/dessous.
Prochaine étape : playtest réel en émulateur du correctif de fuite (priorité, pour confirmer
que le blocage est bien levé), warps réels des bâtiments de Cinnabar (Arène/Manoir/Labo,
bloqué par l'absence de rendu visuel — capture d'écran utilisateur utile ici), puis suite de
l'Acte II (Route 1/Viridian, choix section 8) et Acte III.

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

**Trouvaille non traitée ici, signalée pour suite** : `TRAINER_BLUE_HNS` est le Champion
d'Arène d'Argenta (`ViridianCity_Gym_hns`) dans le jeu de base — un PNJ amical qui donne un
badge. Conflit direct avec Blue antagoniste de Heart & Soul. Fait notable : le dialogue
existant du jeu de base dit déjà « I wasn't in the mood at CINNABAR, but now I'm ready to
battle you » — la trame de base semble déjà anticiper un passage par Cinnabar avant ce combat,
ce qui pourrait faciliter une réconciliation narrative plutôt qu'une réécriture complète. Pas
touché dans cette passe (décision de structure, pas juste un correctif de niveau) : à trancher
avec l'utilisateur (cache-t-on Blue ici comme à Cinnabar en bloquant l'accès à l'Arène jusqu'à
l'Acte V, ou garde-t-on ce combat avec un nouveau texte cohérent ?).

`make hns -j4` : PASS, 0 erreur. ROM à 94.44 %.
