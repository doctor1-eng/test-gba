# Contexte projet — Pokémon Heart & Soul (Kanto overhaul)

Document de reprise à coller/joindre en début d'une nouvelle conversation pour continuer
ce projet sans repartir de zéro. Rédigé le 2026-09-26, mis à jour le 2026-09-26 (session
« Ceux que la Ligue a oubliés », voir §11).

## 1. Le projet en une phrase

Refonte narrative "Heart & Soul" d'un fork Pokémon GBA (`pokehns-expansion`, lui-même un
fork de `pokeemerald-expansion`, demake HGSS avec Johto en trame principale et Kanto en
post-game) : l'histoire commence directement à **Cinnabar** (Kanto), qui tombe sous
l'attaque de la Team Rocket, et suit un joueur déjà Championne qui traverse Kanto pour
affronter des lieutenants régionaux de la Team Rocket. L'utilisateur est le testeur : il
reçoit des ROM compilées, joue en émulateur, et renvoie des retours numérotés en français
que je dois diagnostiquer (root cause, pas de rustine) avant de corriger.

## 2. Dépôt, branches, remotes

- Repo GitHub : `doctor1-eng/test-gba` (fork de `PokemonHnS-Development/pokehns-expansion`).
- Clone local : `/home/user/pokehns-expansion` (remote `origin` = `doctor1-eng/test-gba`,
  `upstream` = le dépôt HnS officiel, non utilisé pour push).
- **Deux branches actives, pas interchangeables** :
  - `claude/pokemon-heart-soul-audit-qv0f4n` — branche "historique" du projet, où le gros
    du travail narratif Acte I/II a été fait (Pierre/Ondine, canalisations, etc.).
  - `claude/gba-tiles-search-import-cli-0kd07s` — branche **actuellement checkout**, plus
    récente, qui contient en plus l'outil `tools/gba_tiles` (rendu de cartes/sprites sans
    émulateur) et une réécriture complète de la chorégraphie de l'Acte I (voir §5).
  - Une **session Claude parallèle** pousse aussi sur ces branches de temps en temps :
    toujours `git fetch` + comparer les commits avant de push, merger (jamais rebase, jamais
    force-push), rebuild après merge pour vérifier que rien n'est cassé.
- **HEAD actuel** (`claude/gba-tiles-search-import-cli-0kd07s`) : commit `8454c24d9c`
  ("Build MAP_CINNABAR_ISLAND_CAVE_HNS..."), working tree propre.

## 3. Build

```bash
cd /home/user/pokehns-expansion
make hns -j$(nproc)
```
Produit `pokehns.gba` (32 Mo) à la racine. Dernier build confirmé : **PASS, 0 erreur**,
ROM ~94.5 % pleine (marge réduite, à surveiller). Seul warning résiduel connu et sans
rapport avec Heart & Soul : `UlaUla_Forest_hns` (`setvar`/`copyvar`, carte hors-Kanto).

Livraison à l'utilisateur : gzip obligatoire (`gzip -f pokehns.gba`) avant `SendUserFile`,
la ROM brute dépasse la limite de taille normale.

## 4. Cartographie des fichiers importants

| Rôle | Chemin |
|---|---|
| **Scénario / bible narrative** | `docs/heart_and_soul/docs/histoire.md` (le "fichier du scénario") |
| Cartographie technique zones H&S ↔ maps réelles | `docs/heart_and_soul/technical_map.md` |
| Cartographie géographique/connectivité Kanto | `docs/world_map.md` |
| Journal d'implémentation (root causes, décisions) | `docs/heart_and_soul/implementation_notes.md` |
| Checklist de test rapide (à donner au testeur) | `docs/heart_and_soul/quick_test_checklist.md` |
| Suivi de couverture de test | `docs/heart_and_soul/testing.md` |
| Scripts narratifs (macros `.inc` natives, **pas de Poryscript actif**) | `data/scripts/heart_and_soul_{intro,act1,act2,act3,act4,act5,aspirants,cinnabar_cave,cinnabar_interiors,evenements,histoires_secondaires}.inc` |
| Registre des flags/vars H&S | `include/constants/flags_hns.h` (+ `HNS_EXTENDED_CONTENT_COUNT` à incrémenter à chaque ajout) |
| Outil de rendu carte/sprite sans émulateur | `tools/gba_tiles/` (README dédié complet), wrapper `python3 gba_tiles.py <commande>` depuis la racine |
| Build de test isolé (portes Cinnabar) | `MAP_TEST_README.md` (`make hns MAPTEST=1`) |
| Registre des assets tiers importés | `ASSETS_SOURCES.md` |

**Non lus en détail dans cette conversation** (existent sur la branche, contenu réel à
relire avant de s'y fier) : `heart_and_soul_act3.inc` (Lyre/Forêt de Jade, Acte III en
cours), `act4.inc`, `act5.inc` (épilogues, dont la conséquence du choix réfugiés de
l'Acte I y a été câblée), `aspirants.inc`, `cinnabar_interiors.inc`,
`histoires_secondaires.inc`. Ce sont probablement le fruit de la session parallèle.

## 5. Où en est l'histoire (état confirmé)

- **Acte I (Cinnabar → Bourg Palet)** entièrement réécrit et mis en scène avec de vrais
  sprites/déplacements (plus une simple suite de `msgbox`) : alarme → chorégraphie
  Blaine/Grunt → éclair (Blaine capturé) → combat forcé contre un 2e Grunt → choix
  Centre Pokémon/Arène → carnet de codes radio → choix réfugiés (guider/cacher, PNJ +
  enfant visibles) → choix Pokémon blessé (Growlithe) → fuite vers une **cabine de bateau**
  dédiée (nouvelle carte, pas d'extérieur Route 21 en Surf) → tempête/marin → **téléportation
  automatique à Bourg Palet**. Détail complet et coordonnées exactes : lire
  `data/scripts/heart_and_soul_act1.inc` (très commenté, explique chaque bug rencontré).
- **Grotte du Mont Cinnabar** (`MAP_CINNABAR_ISLAND_CAVE_HNS`, nouvelle carte, réutilise la
  géométrie de `CliffEdgeCave_hns`) : accessible après l'Acte I, contient les 2 "derniers
  habitants" si le joueur avait choisi de les cacher plutôt que de les guider vers le
  bateau (`FLAG_REFUGIES_CACHES`). Script : `heart_and_soul_cinnabar_cave.inc`.
- **Bâtiments de Cinnabar** (Gym/Manoir/Labo) : warps posés et audités sur
  `CinnabarIsland_hns`, réutilisent la géométrie `_Frlg` existante (Manoir/Labo) ou une
  duplication de layout déjà `_hns` (Gym, cabine du bateau).
- **Acte II (Pierre/Ondine)** : scène de doute avant combat, texte anglais générique
  retiré chez Pierre, texte post-victoire H&S ajouté. Arène d'Ondine **verrouillée** tant
  que `FLAG_CANALISATIONS_REPAREES` n'est pas posé (sous-intrigue canalisations à Azuria) :
  un PNJ Rocket saboteur, réutilisant un object_event déjà présent mais inerte dans le jeu
  de base, est interactif avant réparation puis disparaît après.
- **Bug de la carte de Vol corrigé** : ce fork (base HGSS) ne bascule l'écran Vol sur les
  données Kanto que si `FLAG_VISITED_KANTO` est posé (normalement via `Route27_hns`, jamais
  traversée dans une partie H&S qui reste à Kanto) — sans ce flag, l'écran affichait la
  grille **Johto** avec le bon nom de ville mais la mauvaise géographie. Corrigé en posant
  ce flag dès la création du personnage (les 18 `ChooseTeam_{type}`).
- **Acte III** (Viridian Forest, Mont Sélénite, Rock Tunnel — lieutenants régionaux) :
  Lyre scriptée (Forêt de Jade), Selen et Terrence pas encore faits d'après le dernier état
  connu — **à vérifier**, du travail a pu être fait par la session parallèle depuis.

## 6. Pièges techniques déjà rencontrés (ne pas les redécouvrir)

1. **`call` vs `goto`** : un sous-script appelé par `call` doit impérativement se terminer
   par `return`, jamais par `end` — sinon la pile de retour du moteur de script est
   corrompue. Repéré deux fois dans ce projet.
2. **Freeze total via `map_script_2 VAR_TEMP_X, 0, ...`** utilisé comme déclencheur
   "toujours vrai" (condition = valeur par défaut de la var) : si le script invoqué ne
   remet pas `VAR_TEMP_X` à une valeur non nulle sur **chaque** chemin de sortie,
   `TryRunOnFrameMapScript()` re-déclenche à chaque frame et
   `ProcessPlayerFieldInput()` cesse de traiter tout déplacement/START — freeze total tant
   que le joueur reste sur la carte. `VAR_TEMP_0`/`VAR_TEMP_1` sont remis à 0 par le moteur
   uniquement au **prochain chargement de carte**. Repéré/corrigé au moins 3 fois.
3. **`MAP_SCRIPT_ON_FRAME_TABLE` vs `ON_TRANSITION`** : un menu/msgbox interactif ne peut
   pas tourner de façon fiable depuis `ON_TRANSITION` (l'écran n'est pas encore prêt) —
   utiliser `ON_FRAME_TABLE` + `map_script_2` à la place, et ne JAMAIS continuer une
   chorégraphie (`applymovement`/`msgbox`) directement après un `warp` scripté dans le même
   script : il faut `end` juste après le `warp`/`waitstate`, et relancer la suite depuis
   `ON_FRAME_TABLE` de la carte de destination une fois l'écran réellement prêt.
4. **`trainerbattle_single` à 3 arguments** (`TRAINER_BATTLE_SINGLE`, sans script de
   victoire) ne rend pas la main de façon fiable pour un combat forcé sans object_event
   approché normalement (walk-up) — utiliser la forme à 4 arguments
   (`TRAINER_BATTLE_CONTINUE_SCRIPT`) avec un script de victoire explicite.
5. **Commentaires `@` vs `//`** dans les `.inc` : `cpp` (qui tourne avant l'assembleur)
   reconnaît et retire les commentaires `//`, mais pas les `@` (uniquement compris par
   `as`) — un `@`-commentaire contenant une apostrophe isolée sur une ligne fait croire à
   `cpp` à un littéral caractère non terminé (warning bénin mais à corriger : reformuler
   pour retirer l'apostrophe seule).
6. **`object_event` avec `script: null`** dans un `map.json` = PNJ visible mais totalement
   inerte (appuyer sur A ne fait rien) — pas un bug, juste un placeholder du jeu de base
   jamais câblé. Bon candidat à réemployer plutôt que d'inventer un nouveau PNJ.
7. **`tools/gba_tiles`** : palette d'un tileset secondaire = fichiers `[num_pals_primary,
   num_pals_total)`, pas `00.pal..`; metatiles d'une carte avec tileset secondaire =
   concaténation metatiles primaire + secondaire. `--primary-dir`/`--secondary-dir`
   gèrent ça automatiquement, à préférer à l'assemblage manuel. Vérifier
   `include/fieldmap.h` (`NUM_TILES_IN_PRIMARY`) avant un rendu : ce fork bascule cette
   constante manuellement entre cartes HNS/FRLG (640) et cartes Emerald (512).

## 7. Discipline établie (à respecter dans la suite)

- **Réemploi avant invention** : chercher un object_event/PNJ/map déjà présent avant de
  créer quoi que ce soit de nouveau (sprite, map, flag).
- **Root cause, pas de rustine** : chaque bug remonté par l'utilisateur est diagnostiqué en
  lisant le code réel (moteur `src/*.c` si besoin), jamais deviné.
- **Toujours compiler avant d'annoncer un succès** (`make hns -j$(nproc)`, 0 erreur exigé).
- **`git fetch` avant tout push** sur les branches partagées avec la session parallèle ;
  merge, jamais rebase/force-push ; rebuild après merge.
- **Documenter au fil de l'eau** : chaque changement notable → entrée dans
  `implementation_notes.md`, et mise à jour de `quick_test_checklist.md`/`testing.md` si le
  flux de jeu testable change.
- **Toujours tester sur une partie neuve**, jamais une sauvegarde antérieure au contenu
  testé (règle explicite en tête de `quick_test_checklist.md`).
- **Format de compte-rendu attendu par l'utilisateur** (réponses en français) :
  ÉTAPE / OBJECTIF / FICHIERS INSPECTÉS / FICHIERS MODIFIÉS / CHANGEMENTS / BUILD / TESTS /
  PROBLÈMES RESTANTS / PROCHAINE ÉTAPE.

## 8. Artifact publié

Récapitulatif animé de l'Acte I (vraies cartes + vrais sprites du jeu, rendu via
`tools/gba_tiles`) jusqu'à la téléportation à Bourg Palet :
`https://claude.ai/code/artifact/581db66d-3ebb-4876-99ae-1f86296887d3`
(le watch/wake automatique n'a pas pu s'enregistrer sur cet artifact — infra, sans
impact sur sa consultation/publication).

## 9. Points ouverts / à reprendre en premier

1. **Retours de test n°10 non résolus** : deux observations de l'utilisateur non
   localisées par recherche de mots-clés dans le code — (a) un évènement "agrandissement
   du parc Safari" dans un bâtiment d'entrée de Carmin-sur-Mer, (b) un tunnel bloqué
   Route 5 avec un PNJ mentionnant un problème "power plant". Nécessite de redemander des
   précisions à l'utilisateur (capture d'écran idéalement) avant d'investiguer plus loin.
2. Vérifier l'état réel d'Acte III (Selen/Terrence) et Acte IV/V — pas relus dans cette
   conversation, potentiellement avancés par la session parallèle.
3. Continuer à répondre aux retours de test numérotés de l'utilisateur au fur et à mesure
   qu'il rejoue la ROM.
4. Envisager d'étendre le récapitulatif animé au-delà de Bourg Palet si demandé
   (Route 1, Viridian, etc. — même méthode : `tools/gba_tiles` + sprites réels).

## 10. Pour démarrer la prochaine conversation

Donner ce fichier + dire explicitly : "reprends le projet Heart & Soul, branche
`claude/gba-tiles-search-import-cli-0kd07s` (ou l'autre si précisé), voir le fichier de
contexte joint." Le prochain agent doit commencer par `git status`/`git log` pour
confirmer qu'aucun changement n'a eu lieu depuis ce document, puis relire les fichiers
"non lus en détail" du §4 si l'action porte sur l'Acte III/IV/V.

## 11. Mise à jour 2026-09-26 : session « Ceux que la Ligue a oubliés »

### Environnement (à refaire dans chaque nouveau conteneur)
- Clone : `/home/user/test-gba`. Il peut être positionné sur une autre branche (un autre
  projet vit sur `main`) : faire `git fetch` puis checkout de la branche H&S.
- Toolchain : `apt-get install -y gcc-arm-none-eabi binutils-arm-none-eabi libpng-dev libmgba-dev`
  (faire `apt-get update` si « Unable to fetch »), puis `pip install pillow`.
- Build complet : environ 4 min (`make hns -j$(nproc)`).
- **Tests en jeu autonomes** : `tools/playtest/` (émulateur mGBA headless, scénarios,
  pilote automatique de combat, captures, GIF). Mode d'emploi : `tools/playtest/README.md`.
  C'est ce qui permet de livrer une ROM déjà testée sans renvoyer le testeur à chaque étape.
  Les savestates sont liés à un build : utiliser une sauvegarde « batterie » (`battery`)
  obtenue en rejouant la nouvelle partie scriptée.

### Branches
- Cette session a travaillé sur `claude/new-session-f5pakx`, créée à partir de
  `claude/gba-tiles-search-import-cli-0kd07s` (`8454c24d9c`). Elle contient toute l'histoire
  de gba-tiles plus ce lot. **À fusionner dans gba-tiles** (fast-forward possible si
  gba-tiles n'a pas bougé entre-temps).

### Contenu ajouté (détail dans implementation_notes.md, entrée du 2026-09-26)
- **Village Caché** (Route 25 → `MAP_HIDDEN_VILLAGE_HNS` + cabane) : Mélanie, Pokémon
  abandonnés de Cinnabar, collecteurs Rocket (combat à 2 dresseurs), choix Bulbizarre.
- **Arènes Libres** (porte verrouillée de Céladopole → `MAP_CELADON_CITY_ARENE_LIBRE_HNS`) :
  Kaz et Yas, Vesper la recruteuse, jugement de la Championne.
- 4 sprites et 3 portraits de combat inédits (`tools/hns_sprites/make_sprites.py`), une carte
  générée par script (`tools/hns_maps/hidden_village.py`), des lignes d'épilogue.
- Documents : `docs/heart_and_soul/docs/anime-exclusive-locations-*.md` (source OneDrive et
  audit), `docs/heart_and_soul/prompts/brief_autonome_*.md` (prompt de décision réutilisable).

### Pièges nouveaux (s'ajoutent au §6)
8. Nom de dresseur limité à 10 caractères (sinon `excess elements in array initializer`).
9. Charmap : pas de `«»` ni de `—`. Utiliser `“ ”` et `-`.
10. Les objets placés pendant une cinématique restent à leur position de scène jusqu'au
    rechargement de la carte : les repositionner (`setobjectxy`) à la fin de la scène.
11. Les Pokémon suiveurs (follower) sont actifs : le premier de l'équipe suit le joueur,
    y compris pendant les cinématiques.

### Points ouverts
- Retours de test n°10 (Safari/Carmin, tunnel Route 5) : toujours en attente de précisions.
- Prochain lot anime possible : Gringey City (Centrale, lien avec Terrence), Pokémon Tech,
  habillage du hall de concours de Jadielle.
