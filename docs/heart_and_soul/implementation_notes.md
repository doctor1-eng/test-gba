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

## Statut

Phase 0 (build baseline) : **terminée, PASS**.
Phase "audit obligatoire" (section 3 du brief) : **en cours**, voir `technical_map.md` pour
la cartographie Kanto détaillée. Aucun script Heart & Soul n'a encore été écrit — en attente
des deux décisions ci-dessus avant de commencer la Phase 1 (infrastructure : déclaration des
flags/vars réels).
