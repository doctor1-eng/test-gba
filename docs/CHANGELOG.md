# CHANGELOG.md

## Session 1 — 2026-08-11
- Audit technique complet (PROJECT_ANALYSIS.md)
- Décision d'architecture : moteur pokeemerald décompilé (Option B), validée par Thomas
- Setup toolchain (gcc-arm-none-eabi, binutils-arm-none-eabi) + validation build (2x réussi)
- Confirmation charmap français natif complet
- Traduction FR du discours d'ouverture (Professeur Chen)
- Renommage Littleroot Town → Bourg Palette
- Documentation initiale : PROJECT_ANALYSIS.md, TECHNICAL_ARCHITECTURE.md, PROJECT_STATUS.md, CHANGELOG.md

## Session 2 — 2026-08-11
- Analyse des 4 ROMs de référence fournies (Unbound, Sword/Shield Ultimate Plus, Heart and Soul, Squirrels)
- Recherche : Unbound tourne sur CFRU (Complete FireRed Upgrade, binaire, licence non-commerciale stricte) — équivalent decomp identifié : pokeemerald-expansion (RHH)
- Décision : bascule du moteur de pret/pokeemerald (vanilla) vers rh-hideout/pokeemerald-expansion
- Migration des 2 modifications de la session 1 (Bourg Palette, discours Chen) sur la nouvelle base
- Build complet validé (32 Mo, 79% d'occupation ROM)
- TECHNICAL_ARCHITECTURE.md mis à jour avec liste des mécaniques disponibles et note de licence (attribution RHH)
- Remplacement technique validé : `special ChooseStarter` → `givemon SPECIES_PIKACHU, 5, ITEM_NONE` (Route101_EventScript_BirchsBag) — build propre confirmé
- Réécriture narrative complète de la scène d'ouverture (Épisode 1.2) : Route 1 (sauvetage du Professeur Chen) + Labo (remise de Pikachu, discours adapté, plus de "père" hors-lore Hoenn) — entièrement en français, cohérent avec la personnalité indépendante de Pikachu voulue en amont (CHARACTERS.md)
- Renommage Route 101 → Route 1, Oldale Town → Jadielle (panneau + dialogues), CENTRE POKéMON déplacé narrativement à Jadielle
- Nouvelle limite de police identifiée : le tiret cadratin (—, U+2014) n'est pas dans la charmap → utiliser la virgule ou le point à la place
- Build validé après chaque étape (3 builds propres cette session)
- Épisode 1.3 (Pikachu hors POKé BALL) : activation de `OW_FOLLOWERS_ENABLED` (système HGSS natif du moteur) → le Pokémon en tête d'équipe suit désormais le joueur à pied sur la carte au lieu de rester invisible dans son SAC. Build complet revalidé (4e build propre de la session).
- Vérifié : Pikachu dispose nativement de son sprite de suivi overworld (P_FAMILY_PIKACHU actif) — aucun travail graphique supplémentaire requis.
- Découverte utile : le système inclut déjà des réactions contextuelles automatiques du suiveur près de certains décors (Ligue Pokémon, vélos, bateau, etc.) — réutilisable pour renforcer la personnalité de Pikachu sans script maison.
- Commande `hidefollower` disponible pour masquer ponctuellement le suiveur pendant des cinématiques scriptées si besoin.
- Épisode 1.4 implémenté : première rencontre Team Rocket sur Route 1
  - Ajout de 3 nouveaux événements objets (Jessie = OBJ_EVENT_GFX_ROCKET_F, James = OBJ_EVENT_GFX_ROCKET_M, Miaouss = sprite overworld de l'espèce Meowth) — sprites déjà présents dans les assets du moteur, aucun graphisme custom nécessaire
  - Scène complète en français : présentation façon "devise" Jessie/James/Miaouss, tentative de vol au filet, Pikachu contre-attaque (décharge), fuite comique scriptée
  - Gating narratif propre : la Team Rocket n'apparaît sur la carte qu'après réception de Pikachu (VAR_BIRCH_LAB_STATE >= 3), et disparaît définitivement après la scène (2 flags dédiés : FLAG_HIDE_ROUTE_1_TEAM_ROCKET_ENCOUNTER pour la visibilité, FLAG_TEAM_ROCKET_ROUTE1_DEFEATED pour la résolution permanente)
  - Limite assumée : scène scriptée en dialogue, pas un vrai combat de dresseur (aucune classe de dresseur "Team Rocket" n'existe encore dans le moteur — Émeraude n'a que Team Aqua/Magma nativement). Créer une vraie classe de dresseur Team Rocket réutilisable (nom, sprite de combat déjà présent, musique de combat) est nécessaire avant d'ajouter des affrontements réels avec Jessie/James récurrents dans les arcs suivants — tâche notée, pas traitée dans l'urgence pour ne pas fragiliser une base de données partagée par tout le jeu
- Build validé (5e build propre de la session)
- Vraie classe de dresseur créée : TRAINER_CLASS_ROCKET ("TEAM ROCKET", récompense en argent x12, Poké Ball standard). Musique de combat : réutilisation de la musique Team Aqua en attendant l'import d'un thème dédié (pas de nouvel asset audio pour l'instant — item de polish futur)
- Deux dresseurs réels créés et prêts à l'emploi : TRAINER_ROCKET_JESSIE_1 (Ekans niv. 8) et TRAINER_ROCKET_JAMES_1 (Koffing niv. 8) — fidèles aux Pokémon emblématiques du duo dans l'anime, sprites de combat déjà présents (rocket_grunt_f_frlg / rocket_grunt_m_frlg)
- Ajustements structurels nécessaires : TRAINERS_COUNT_EMERALD 855→857 (2 nouveaux ID), vérifié : reste sous la limite MAX_TRAINERS_COUNT_EMERALD (864) donc pas de dépassement de l'espace de flags dédié aux dresseurs vaincus
- Build validé (6e build propre de la session)
- Épisode 1.4 basculé en vrai combat : parler à James déclenche `trainerbattle_single` contre TRAINER_ROCKET_JAMES_1 (Koffing), avec la "devise" Jessie/James comme texte d'intro et une réplique de défaite dédiée
- Niveau de Koffing ajusté à 4 (contre 8 initialement) pour rester un premier combat accessible avec un Pikachu niveau 5 tout juste reçu — cohérent avec le principe "éviter les murs de difficulté artificiels" du cahier des charges
- Jessie et Miaouss restent des PNJ de circonstance (répliques courtes, redirigent vers James) ; fuite scénarisée après la victoire (déplacement + suppression des 3 objets + flags de résolution)
- Build validé (7e build propre de la session)
- Nom du rival fixé sur "Régis" : nouveau choix de nom `gNameChoice_Regis` ajouté à `sRivalNameChoices`, placé en première position (choix par défaut à l'écran de nommage du rival). Solution volontairement non-intrusive : pas de réécriture du flux `oak_speech.c` (partagé avec le nommage du joueur, trop risqué à modifier en profondeur) — juste un contenu de liste changé.
- Build validé (8e build propre de la session)

## Session 3 — Corrections suite au premier retour de test de Thomas
- Sprite Professeur Chen : remplacement de OBJ_EVENT_GFX_PROF_BIRCH → OBJ_EVENT_GFX_PROF_OAK (déjà présent nativement dans le moteur, repris de FRLG) sur toutes les maps concernées (labo, Route 1, LittlerootTown, Route 103, Route 110, EverGrandeCity_ChampionsRoom) — cohérence globale
- Traduction de l'assistant du labo (BirchAwayOnFieldwork, BirchIsntOneForDeskWork, BirchEnjoysRivalsHelpToo)
- Bug élévation Team Rocket corrigé (0 → 3) : cause du sprite invisible + blocage fantôme signalé par Thomas. Vérifié via lecture directe de la couche de collision/élévation du layout (positions confirmées praticables)
- Clarifié : le combat contre le Zigzagoon après réception de Pikachu est une rencontre sauvage standard (herbes hautes, taux 20%), pas un bug
- Build validé (9e build propre) — ROM v0.2 fournie à Thomas pour re-test

## Session 3 (suite) — Épisode 1.5 : premier combat de Régis
- Découverte/correctif important : nos scènes ultérieures (combat de rival, etc.) reposent sur VAR_STARTER_MON pour savoir quel POKéMON adverse envoyer — variable jamais initialisée depuis qu'on a remplacé ChooseStarter par un givemon direct. Fixée une bonne fois à une valeur constante (1) juste après la remise de Pikachu, pour que toutes les branches du jeu qui en dépendent (il y en a dans une dizaine de fichiers) restent cohérentes sans avoir à toutes les réécrire une par une
- Scène du premier combat de rival (Route 103) entièrement réécrite : suppression du branchement Mai/Brendan selon le genre du joueur (Régis est un personnage fixe, comme Bleu dans les jeux originaux) — nouveau script `Custom_EventScript_SetupRegisGfxId` (sprite Brendan fixe), un seul combat (plus de switch sur 3 starters)
- Textes traduits et adaptés : "mon père" → "mon grand-père" (cohérent avec le lien Chen/Régis établi précédemment)
- Build validé (10e build propre de la session)

## Session 3 (suite 2) — Renommage régional Hoenn → Kanto
- Nouveau document MAPS.md créé : table de correspondance officielle, avec statut par zone (renommée / en attente / reconstruite)
- 11 villes/villages Hoenn renommées avec les noms français officiels Kanto (Bourg Palette, Jadielle, Argenta, Azuria, Carmin-sur-Mer, Lavanville, Céladopole, Parmanie, Safrania, Île Cramoisie, Plateau Indigo) — correspondance choisie par cohérence géographique/fonctionnelle (ports↔ports, îles↔îles, Ligue↔Ligue) quand c'était possible
- Les 34 routes renumérotées en style Kanto (Route 101→Route 1, etc.) sur la carte régionale
- 5 villes Hoenn sans équivalent Kanto direct laissées en attente (candidates pour du contenu post-game)
- Build validé (11e build propre de la session)

## Session 3 (suite 3) — Outil de QA headless
- Construction d'un harness C (`tools/qa_harness/qa_runner.c`) basé sur libmgba : fait tourner la ROM sans interface graphique, injecte des séquences de touches, capture des PNG
- Débogage réussi (segfault d'initialisation résolu en suivant l'exemple officiel `perf-main.c` de mgba) — pipeline validé de bout en bout (capture du tout premier écran de boot confirmée)
- Statut : fonctionnel mais timings non calibrés — le jeu reste dans les écrans de logos plus longtemps que prévu (>250 frames). Calibrage précis reporté à la prochaine session.
- Documentation créée : `tools/qa_harness/README.md`
- Impact attendu : pouvoir jouer et vérifier moi-même le contenu avant de livrer une ROM à Thomas, plutôt que de découvrir les bugs seulement via ses retours de test

## Session 3 (suite 4) — Calibrage QA harness
- Calibrage par captures d'écran successives : confirmé que l'écran titre (animation Latios/Latias) est atteint après ~1800 frames de boot
- Confirmé que l'entrée Start est bien prise en compte par le harness (transition visible vers le logo "Pokémon")
- Reste à calibrer : séquence précise pour passer du logo au menu New Game/Continue de façon fiable
- Outil documenté avec les timings connus dans tools/qa_harness/README.md pour reprise directe la prochaine session

## Session 3 (suite 5) — Investigation maison du joueur (Épisode 1.1)
- Examen de LittlerootTown_BrendansHouse_1F (= maison du joueur) : contrairement au labo (où un simple
  nettoyage de références suffisait), TOUT le texte de cette map est structurellement couplé à l'intrigue
  "jour d'emménagement" de Hoenn (PAPA = champion d'arène de Petalburg, POKENAV, badges, DEVON). Une
  traduction littérale produirait un scénario cassé (notre histoire n'a pas de "papa champion d'arène", le
  joueur n'emménage pas le jour de l'aventure).
- Décision : ne PAS traduire à la légère. Cette map a besoin d'une réécriture narrative complète (comme la
  scène Route 1/Labo), pas d'une simple traduction — tâche notée pour une session dédiée à l'Épisode 1.1.
- QA harness : blocage input documenté (voir tools/qa_harness/README.md) — les pressions de touches ne
  semblent pas prises en compte par le core, cause racine non identifiée, ne pas retenter à l'aveugle.

## Session 4 — 2026-08-12 — Migration vers un vrai dépôt git (Claude Code)
- Transfert du projet depuis l'environnement bac-à-sable éphémère vers un vrai dépôt git
  (`doctor1-eng/test-gba`, branche `claude/pokeemerald-setup-context-wayy1t`), conformément au plan annoncé
  dans GETTING_STARTED.md
- Reconstruction complète depuis le paquet léger (`kanto_saison1_leger.tar`) : clone frais de
  `rh-hideout/pokeemerald-expansion` dans `engine/`, copie des 21 fichiers modifiés (`changed_files/`)
  par-dessus
- Toolchain installée (`build-essential`, `binutils-arm-none-eabi`, `libpng-dev`, `gcc-arm-none-eabi`)
- Build validé (12e build propre au global) : `engine/pokeemerald.gba`, 32 Mo, 79,02 % d'occupation ROM —
  strictement identique au taux d'occupation constaté en session 2/3, confirmant que la reconstruction est
  fidèle
- Décision de structure de dépôt : `engine/` (moteur cloné, ~800 Mo dont ~500 Mo de son propre `.git`) reste
  **hors suivi git** (`.gitignore`), conformément à la logique déjà actée dans INSTALL.md ("paquet léger").
  Le contenu réellement versionné est `changed_files/` (à la racine, copié depuis le paquet) + `docs/` +
  `tools/qa_harness/` (sans le binaire précompilé `qa_runner`, ignoré — seule sa source `qa_runner.c` est
  suivie). `engine/` se régénère à chaque session via `INSTALL.md`.
- Le binaire `qa_runner` fourni dans le paquet n'a pas été exécuté (provenance non vérifiable dans ce
  nouvel environnement) ; à recompiler depuis la source si besoin de reprendre le QA harness
- Aucune modification de contenu/scénario cette session — session d'infrastructure uniquement

## Session 4 (suite) — Épisode 1.1 : réécriture de la maison du joueur
- Proposition de scène (dialogues + séquence) validée par Thomas avant implémentation, comme demandé
- **Suppression complète de l'intrigue "emménagement"**, jusqu'à sa racine :
  - `src/new_game.c` : la nouvelle partie ne warp plus vers `InsideOfTruck` (camion) mais directement dans
    la chambre du joueur (`LittlerootTown_BrendansHouse_2F` ou `MaysHouse_2F` selon le genre), fonction
    renommée `WarpToPlayerBedroom`. Le point de respawn (`SetLastHealLocationWarp`) est fixé au même
    endroit puisque `InsideOfTruck` ne s'exécute plus pour le faire.
  - `LittlerootTown/scripts.inc` : retrait de la scène "sortie du camion" (Maman accueille le joueur
    devant la maison, dialogue "we're here, honey"), des mouvements et du texte associés
  - `LittlerootTown_BrendansHouse_1F` et `MaysHouse_1F` : retrait des cartons de déménagement, du blocage
    scénarisé de l'escalier, du bulletin télé "Arène d'Argenta / p Papa sera peut-être à l'écran"
  - `LittlerootTown_BrendansHouse_2F` et `MaysHouse_2F` : retrait du blocage d'escalier lié à l'horloge
  - `data/scripts/players_house.inc` : nouveau dialogue unique de Maman
    (`PlayersHouse_1F_EventScript_BonjourMaman`) — accueil + annonce que le professeur Chen attend au
    labo + rappel de prudence sur la route (fusion des deux beats "accueil"/"au revoir" proposés,
    simplification volontaire pour rester sur des "scripts minimes" comme demandé dans EPISODES.md)
  - Nouvelle sémantique simplifiée de `VAR_LITTLEROOT_INTRO_STATE` : 0 = pas encore parlé à Maman,
    1 = fait (contre 8 états auparavant)
- **Contenu non traité, signalé pour une session dédiée future** (découvert en creusant les dépendances,
  pas dans le périmètre validé par Thomas cette fois) :
  - La maison du RIVAL (Régis) contient une scène "nouveau voisin" symétrique
    (`RivalsHouse_1F_Text_MayWhoAreYou` / `BrendanWhoAreYou`, "so your move was today", "mon père le
    champion d'arène") — toujours accessible indépendamment de ce qui vient d'être corrigé, car son
    déclenchement (`VAR_LITTLEROOT_RIVAL_STATE`) ne dépend pas de la scène du camion
  - Vérifié avant de laisser en l'état : ce contenu n'est référencé par aucun fichier déjà validé
    (`Route103/scripts.inc`, `oak_speech.c`, `rival_graphics.inc`) — aucun risque de casser le combat de
    rival existant en le laissant de côté pour l'instant
  - La quête SS Ticket/Latios-Latias livrée par "papa" (`PlayersHouse_1F_EventScript_GetSSTicketAndSeeLatiTV`)
    et le cadeau post-badge 5 ("Amulet Coin... did DAD give you that badge?") restent également non traités,
    comme déjà noté avant implémentation — toujours atteignables via le Hall of Fame, donc pas de
    régression de compilabilité
- Build validé (13e build propre au global) : `engine/pokeemerald.gba`, 32 Mo, toujours 79,02 % —
  cohérent (code retiré ≈ code ajouté)
