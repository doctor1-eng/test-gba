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

## Session 4 (suite 2) — Maison du rival (Régis) : correction des références père/déménagement
- Découverte importante en creusant : la scène "Poké Ball" de la chambre de Régis n'est pas du code mort
  comme supposé initialement — elle pose `setvar VAR_LITTLEROOT_TOWN_STATE, 1`, la variable qui débloque
  physiquement l'accès à la Route 1 (le PNJ qui garde la sortie du village vérifie cette variable). La
  supprimer comme proposé initialement aurait bloqué le jeu après l'Épisode 1.1. Proposition corrigée
  validée par Thomas avant implémentation.
- Découverte d'une deuxième scène équivalente (Régis vient voir le joueur chez lui, `MeetRival0/1/2`),
  menant au même déblocage, avec le même texte incohérent — traitée en même temps
- Décision de généalogie actée avec Thomas : Régis est le petit-fils de Chen via l'un de ses enfants (donc
  le PNJ "mère du rival" est la fille de Chen, pas son épouse — le texte "my husband spends his days at
  the lab" sous-entendant que son mari est le professeur ne pouvait pas être traduit tel quel)
- Textes réécrits (mise en scène/choréographie et logique de déblocage entièrement conservées, seul le
  contenu parlé change) :
  - `RivalsHouse_1F_Text_LikeChildLikeFather` → `CommeSonGrandPere` (PNJ mère de Régis, dialogue par défaut)
  - `RivalsHouse_1F_Text_WentOutToRoute103` → `PartiRoute103` ("comme son grand-père" au lieu de "comme
    son père", écho avec le texte déjà existant sur Chen qui alterne labo/terrain)
  - `RivalsHouse_1F_Text_DoYouHavePokemon` → `TuAsDejaUnPokemon` (PNJ enfant du quartier, retrait du
    cadrage "nouveau voisin")
  - `RivalsHouse_1F_Text_BrendanWhoAreYou` / `MayWhoAreYou` (scène "Régis vient te voir") et
    `RivalsHouse_2F_Text_BrendanWhoAreYou` / `MayWhoAreYou` (scène "Poké Ball" dans sa chambre) : les 4
    remplacés par un texte cohérent avec une rivalité déjà établie (plus de "qui es-tu, tu viens
    d'emménager, mon père le champion d'arène..."), ton "sûr de lui, compétitif" conforme à CHARACTERS.md
  - Contenu identique entre les variantes Brendan/May (le sprite du rival n'est pas encore fixé sur Régis
    dans cette zone contrairement à Route 103 — limitation technique connue, non traitée cette session,
    n'affecte pas le texte qui reste cohérent dans les deux cas)
- `RivalsHouse_1F_Text_OhYoureTheNewNeighbor` : toujours orpheline (scène "la mère de Régis visite ta
  maison", morte depuis la suppression du camion), non traitée — signalée, hors périmètre validé cette fois
- Build validé (14e build propre), ROM 79,01 % (quasi inchangé)

## Session 4 (suite 3) — Début Arc 2 : Jadielle et Route 2
- Découpage Arc 2 proposé et validé par Thomas : 2.1 Jadielle/Route 2, 2.2 forêt de Jade (Team Rocket
  remplace Team Aqua/Devon Corp), 3.1 rencontre avec Ondine — les deux dernières restent à écrire et
  soumettre avant implémentation, trop centrales pour y aller à l'aveugle
- Jadielle : panneau physique de la ville corrigé (affichait encore "OLDALE TOWN" en anglais malgré le
  renommage déjà fait sur la carte région depuis la Session 3) ; PNJ boutique/potion/chercheur
  d'empreintes traduits (contenu neutre, aucune incohérence Hoenn)
- Caméo de Régis à Jadielle ("MAY/BRENDAN : je rentre au labo de mon père") : même travers "papa" que la
  maison du rival, corrigé avec la même formule ("je file aider mon grand-père") — caméo isolé, sans
  effet de bord sur la progression (juste `VAR_OLDALE_RIVAL_STATE` local à la scène), donc traité
  directement sans round de validation séparé
- Route 2 (ex-Route102) : contenu neutre (dresseurs génériques, PNJ, arbres à baies), panneaux traduits
  et renommés (Jadielle/Argenta) ; textes de combat génériques (Calvin/Rick/Tiana/Allen) laissés pour le
  balayage de traduction général, pas de contenu narratif dedans
- Repéré en chemin : `PetalburgCity/scripts.inc` référence des textes "WALLY" partagés depuis ce fichier
  Route102 (scène "attraper un Pokémon" + retour à l'Arène) — hors périmètre Route 2/forêt, concerne
  Argenta (Arc 4 Pierre), à traiter le moment venu
- Build validé (15e build propre), ROM 79,01 %

## Session 4 (suite 4) — Épisode 2.2 : Team Rocket en forêt de Jade
- Texte validé par Thomas avant implémentation. Combat obligatoire "Grunt Team Aqua vole des documents
  Devon Corp" (bloquait le passage) remplacé par une réapparition de Jessie/James/Miaouss — cohérent avec
  GAME_DESIGN_DOCUMENT.md ("rencontres scriptées à des points clés de chaque arc")
- Inversion des rôles par rapport à Route 1 : cette fois Jessie combat (Ekans niveau 12, nouveau dresseur
  `TRAINER_ROCKET_JESSIE_2`), James l'encourage — garde le duo vivant sur la durée sans répéter Route 1
  à l'identique
- Mise en scène simplifiée par rapport à l'original (choréographie "chercheur fuit / grunt poursuit"
  abandonnée au profit d'un format court façon Route 1 : sursaut, dialogue, combat, fuite comique) — les
  deux PNJ (sprites Team Aqua/employé Devon) recyclés en Jessie/James (`OBJ_EVENT_GFX_ROCKET_F/M`) plutôt
  que recréés, `map.json` édité directement (source de vérité, `events.inc` se régénère automatiquement —
  confirmé en vérifiant les règles Make, ne jamais éditer `events.inc` à la main)
- Budget dresseurs : 858/864 utilisés (6 de marge restante)
- Repéré au passage, non traité : un dresseur générique "James" (chasseur d'insectes vanilla, sans lien
  avec notre Team Rocket) existe déjà dans cette forêt — coïncidence de nom à corriger lors du balayage
  de traduction général
- Build validé (16e build propre), ROM 79,01 %

## Session 4 (suite 5) — Arc 3, début : première rencontre avec Ondine
- Texte validé par Thomas, ainsi que le choix de conception : Ondine reste une **rivale récurrente**
  (recroisée ponctuellement plus tard), pas de mécanique de suivi façon Pikachu — plus simple, aucun
  risque technique nouveau
- Scène placée sur Route 2, en réutilisant le PNJ générique "Boy" déjà existant (position déjà validée
  praticable) plutôt qu'en inventant de nouvelles coordonnées à l'aveugle (pas d'outil de preview de map
  disponible pour vérifier visuellement)
- Nouveau dresseur `TRAINER_ONDINE_1` (Poliwag niveau 13, légèrement au-dessus de Jessie/Route 2 pour
  rester crédible comme "test" de la part d'Ondine), classe de combat `Swimmer F` / sprite overworld
  `OBJ_EVENT_GFX_SWIMMER_F` (pas de sprite dédié Ondine existant, réutilisation d'un sprite vanilla)
  cohérente avec le thème "passionnée de POKéMON EAU"
- Deux nouveaux flags custom ajoutés (`FLAG_HIDE_ROUTE_2_ONDINE_ENCOUNTER`, `FLAG_ONDINE_ROUTE2_DEFEATED`,
  0x22/0x23) dans la même plage libre que les flags Team Rocket de la Session 2 — attention en ajoutant
  un flag : bien remplacer la ligne `FLAG_UNUSED_0x0xx` existante, pas juste insérer avant (erreur commise
  puis corrigée pendant cette étape, cf. duplication de valeur détectée et nettoyée)
- Budget dresseurs : 859/864 utilisés (5 de marge restante)
- Un premier build a échoué (`Class: Swimmer` n'est pas un nom de classe valide, corrigé en `Swimmer F`
  — leçon : toujours vérifier le nom exact de `Class:` par grep sur une entrée existante utilisant le
  même `Pic:` avant d'écrire une nouvelle entrée `trainers.party`)
- Build validé (17e build propre), ROM 79,01 %
- **Arc 3 amorcé** : la rencontre avec Ondine annoncée en fin de proposition Arc 2 est faite. Son
  développement complet (ville d'Azuria, arène, etc.) reste à concevoir plus tard, hors périmètre de
  cette session

## Session 4 (suite 6) — Aparté : analyse statique Pokémon Unbound (hors développement du jeu)
- Quatre fichiers ROM/paquets fournis par Thomas pour analyse : trois refusés (ROMs de jeux commerciaux
  complètes, hackées ou non — Unbound v2.1.1.1, un hack FireRed "Squirrels", une FireRed vanilla), un
  accepté après vérification de son contenu réel (un pack de métadonnées d'analyse statique sans aucune
  ROM/image/son à l'intérieur — CSV d'offsets, entropie, compteurs de références)
- Analyse complète documentée dans `docs/pokemon-unbound-analysis/` (14 documents + FINAL-REPORT.md) et
  outillage réutilisable dans `tools/unbound_analysis/` (3 scripts Python)
- Conclusion principale : peu d'éléments concrets exploitables pour notre projet (les données ne
  contiennent aucun contenu réel, seulement des statistiques), mais confirmation croisée qu'Unbound est
  bâti sur FireRed (cohérent avec nos notes de Session 1), et une découverte méthodologique utile (deux
  signaux statistiques sur sept se sont avérés dominés par des faux positifs à l'échelle de la ROM
  entière — détaillé dans `code-analysis.md`/`compression.md`)
- Sans rapport avec l'avancement du jeu — aucun fichier `engine/`/`changed_files/` touché cette étape

## Session 4 (suite 7) — Arc 4 : Argenta/Pierre (ex-Norman), retrait de l'intrigue "papa champion d'arène"
- Priorité fixée par Thomas ("Traite Argenta d'abord") : traiter la plus grosse zone "papa" restante avant
  de poursuivre l'Arc 3 (Ondine/Azuria)
- Proposition validée par Thomas ("Je valide") : Norman → PIERRE, aucun lien de parenté avec le joueur,
  équipe recentrée Roche (au lieu de Normal), suppression du palier "reviens avec 4 badges", intrigue
  Wally conservée telle quelle (elle ne référence jamais la famille du joueur), badge renommé
- `TRAINER_NORMAN_1` (constante interne conservée pour limiter le risque sur les fichiers qui la
  référencent) : Nom PIERRE, objets Potion/Potion, équipe remplacée par du Roche — Geodude niv. 12
  (Tacle/Amorce/Jet-Pierres) et Onix niv. 14 (Tacle/Cri Perçant/Étreinte/Jet-Pierres) — remplace Spinda
  niv. 27/Vigoroth niv. 27/Linoone niv. 29/Slaking niv. 31 @Baie Sitrus (Normal, bien trop fort pour une
  première arène)
- 7 dresseurs du gauntlet (Randall, Parker, George, Berke, Mary, Alexia, Jody — tous Cooltrainer) baissés
  de niveau 26 à niveau 12 et objet Hyper Potion → Potion, espèces/attaques inchangées (les 7 salles à
  thème Vitesse/Précision/Confusion/Défense/Soin/Force/K.O. restent en l'état, jugées neutres
  narrativement)
- `PetalburgCity_Gym/scripts.inc` : switch `VAR_PETALBURG_GYM_STATE` simplifié — les cas 2 à 5 (paliers
  "reviens avec 1/2/3/4 badges", textes `NormanGoToRustboro`/`NormanGoToDewford`/`YouHaveGottenStronger`
  supprimés avec leurs scripts) mènent désormais tous directement au combat contre Pierre, cohérent avec
  Argenta = première arène (MAPS.md)
- ~30 blocs de texte réécrits/traduits dans `PetalburgCity_Gym/scripts.inc` : toute la séquence Wally
  (accueil, prêt du Zigzagoon, retour de tutoriel) traduite en français et débarrassée du cadre
  père-fils ; intro/défaite de Pierre, remise du badge, explication CT Répercussion, réaction post-combat,
  textes de re-provocation post-badge ; scène "PleaseComeWithMe/LetMeBorrowPlayer" du père de Wally
  reformulée (n'emprunte plus "le fils du champion" mais simplement le dresseur qui a aidé Wally) ; dans le
  gauntlet, les répliques évoquant "le gamin du champion"/"ton père" (Berke, Jody, George, guide de la
  salle) neutralisées (restent en anglais, traduction complète de ces 7 salles différée — hors scope de
  cette étape) ; panneaux de l'arène traduits (`ARÈNE POKéMON D'ARGENTA`)
- `PetalburgCity_Gym_Text_ReceivedBalanceBadge` (seule occurrence de "BALANCE BADGE" dans tout le dépôt) →
  "BADGE ROCHE", effet mécanique (HM Surf, +DÉF) inchangé
- `PetalburgCity/scripts.inc` vérifié : aucune référence au père du joueur (les seules mentions "DAD"
  restantes concernent le père de Wally, un personnage distinct, non affecté)
- Hors scope, sciemment différé : les 4 textes de rematch post-Ligue (`NormanPreRematch`,
  `NormanRematchDefeat`, `NormanPostRematch`, `NormanRematchNeedTwoMons`) contiennent encore un thème
  "parent et enfant" complet — accessibles uniquement en post-Ligue, non bloquant pour l'instant
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`src/data/trainers.party`, `data/maps/PetalburgCity_Gym/scripts.inc`)

## Session 4 (suite 8) — Corrections suite au premier test de Thomas sur Bourg Palette
- Retour de test (4 points) : écran qui tremble/joueur bloqué à l'ouverture, deux camions visibles en
  permanence dans la ville, deux "mamans" superposées dans chaque maison, une PNJ (la Jumelle) qui bloque
  physiquement la sortie nord de la ville
- Cause racine identifiée pour 3 des 4 points : `InsideOfTruck_EventScript_SetIntroFlagsMale/Female`
  posait, en plus de l'animation du camion elle-même, toute une série de `setflag` de nettoyage (cacher
  les deux camions, cacher la "maman du rival"/le "frère ou sœur du rival"/la Poké Ball du rival dans la
  maison du joueur). En supprimant l'entrée dans `MAP_INSIDE_OF_TRUCK` (cf. Épisode 1.1), ce nettoyage
  n'était plus jamais exécuté — d'où les doublons et les camions résiduels
- `src/new_game.c` (`WarpToPlayerBedroom`) : les `FlagSet()` correspondants sont maintenant posés
  directement en C, par genre, au moment du warp direct vers la chambre — sans passer par le camion ni son
  animation. Les deux camions de Bourg Palette sont cachés dans tous les cas
- `data/maps/LittlerootTown/scripts.inc` + `map.json` : suppression du verrou "pas de sortie sans
  POKéMON" (déclencheurs `NeedPokemonTriggerLeft`/`Right` + scène `DangerousWithoutPokemon` qui faisait
  physiquement reculer le joueur) — obsolète dans notre histoire puisque Pikachu est donné dès le départ
  par le Professeur Chen. La Jumelle reste en PNJ d'accueil près du passage nord, sans plus jamais bloquer
  le passage ; le code mort associé (scripts, mouvements, texte) a été retiré
- Le tremblement d'écran/blocage total à l'ouverture correspond au comportement de
  `Task_HandleTruckSequence`/`ExecuteTruckSequence`, qui n'est déclenché que par les coord_events internes
  à `MAP_INSIDE_OF_TRUCK` — or `WarpToPlayerBedroom` ne warpe plus jamais vers cette carte (vérifié dans
  `src/new_game.c` et via recherche exhaustive des appelants d'`ExecuteTruckSequence`). Aucun chemin de
  code ne peut plus déclencher cette scène sur une nouvelle partie avec ce build ; le plus probable est que
  Thomas testait avec une sauvegarde créée sur une build antérieure à la réécriture de l'Épisode 1.1 (le
  camion n'a jamais existé dans les commits d'après Session 4). À vérifier avec une sauvegarde neuve sur ce
  build
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`src/new_game.c`, `data/maps/LittlerootTown/scripts.inc`, `data/maps/LittlerootTown/map.json`)

## Session 4 (suite 9) — Arc 3 : Azuria, l'arène d'Ondine (Rustboro/Roxanne repurposée)
- Proposition validée : `TRAINER_ROXANNE_1` devient ONDINE, déjà annoncée comme rivale récurrente
  (`CHARACTERS.md`) et déjà rencontrée sur la Route 2. Contrairement à Argenta/Pierre, aucune intrigue
  familiale à retirer ici — Roxanne n'a jamais eu de lien de parenté avec le joueur en vanilla
- `src/data/trainers.party` : équipe passée de Roche (Geodude x2 + Nosepass) à Eau (Poliwag niv. 12,
  Goldeen niv. 12, Staryu niv. 15 @Baie Oran), même structure de niveaux que l'original. Le nom interne
  `TRAINER_ROXANNE_1` est conservé (même logique que `TRAINER_NORMAN_1`→Pierre) pour ne pas toucher aux
  nombreux fichiers qui le référencent (`battle_setup.c` REMATCH_ROXANNE, `trainers.h`, `opponents.h`)
- 3 dresseurs du gauntlet reconvertis en Eau : Josh (Geodude→Horsea), Tommy (2x Geodude→2x Goldeen), Marc
  (2x Geodude→2x Tentacool, classe Hiker→Fisherman puisque sa réplique vantait littéralement "mes POKéMON
  ROCHE")
- `data/maps/RustboroCity_Gym/scripts.inc` : tous les textes Roxanne→Ondine réécrits en français (intro,
  défaite, badge, PostBattle, GymGuide, statue, appel PokéNav). L'intro d'Ondine référence explicitement
  leur rencontre de la Route 2 ("On se retrouve, {PLAYER}… Je t'avais bien dit qu'on n'en resterait pas
  là"). Badge renommé STONE BADGE → **BADGE CASCADE** (nom canon d'Ondine/Misty dans les jeux originaux)
- Retrait d'un bug latent : `RoxanneDefeated` faisait `addvar VAR_PETALBURG_GYM_STATE, 1` +
  `call_if_eq VAR_PETALBURG_GYM_STATE, 6, ...ReadyPetalburgGymForBattle` — un vestige de l'ordre vanilla où
  Rustboro est le 1er badge et Petalburg le 5e (gate à 4 badges). Comme on a inversé cet ordre (Argenta = 1er
  badge désormais), ce code aurait fait sauter Pierre en mode "revanche" (`VAR_PETALBURG_GYM_STATE = 8`) dès
  qu'Ondine est battue, même sans revanche légitime déclenchée. Supprimé
- Décisions de scope, mêmes principes que pour Argenta : la TM offerte (Éboulement/Rock Tomb) reste
  inchangée mécaniquement (texte traduit mais neutre, pas de redesign d'objet) ; les textes de revanche
  post-Ligue (`RoxannePreRematch`/`RoxanneRematchDefeat`/`RoxannePostRematch`/`RoxanneRematchNeedTwoMons`,
  `TRAINER_ROXANNE_2`–`_5`) restent non traités, différés comme pour Pierre
- Panneau de ville (`RustboroCity_Text_GymSign`) et un PNJ neutre (`RustboroCity_House2`) qui nommaient
  Roxanne corrigés pour cohérence
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`src/data/trainers.party`, `data/maps/RustboroCity_Gym/scripts.inc`, `data/maps/RustboroCity/scripts.inc`,
  `data/maps/RustboroCity_House2/scripts.inc`)

## Session 4 (suite 10) — Correction d'une régression : le blocage de la Jumelle était intentionnel
- Retour de test de Thomas : "Ça relance le jeu dès que je passe le village" — crash/redémarrage en
  quittant Bourg Palette par le nord
- Cause identifiée après relecture du CHANGELOG (Session 4 suite 2) : le retrait du blocage de la Jumelle
  fait en suite 8 était une **erreur**. Ce blocage n'est pas un reliquat vanilla obsolète — il pose
  `VAR_LITTLEROOT_TOWN_STATE` à une valeur non nulle uniquement après la scène de la maison du rival, et
  c'est ce verrou qui garantissait que le joueur passe par cette scène (mise en place, choréographie
  validées par Thomas) avant d'atteindre la Route 1 et la scène de sauvetage du Professeur Chen
  (`Route101_EventScript_StartBirchRescue`/`BirchsBag`, qui donne Pikachu). En retirant le verrou, il
  devenait possible d'atteindre la Route 1 sans être passé par la maison du rival — combinaison d'états
  jamais testée ni prévue, très probablement la cause du crash
- Correctif : restauration intégrale du verrou tel qu'il existait avant la suite 8
  (`LittlerootTown_EventScript_SetTwinPos`/`SetTwinGuardingRoutePos`, les triggers
  `NeedPokemonTriggerLeft`/`Right`, la scène `DangerousWithoutPokemon` et son texte) dans
  `data/maps/LittlerootTown/scripts.inc` et `map.json`. Les corrections trucs/mamans de la suite 8
  (`src/new_game.c`) restent en place, elles ne sont pas concernées par cette régression
- Leçon retenue : avant de qualifier un mécanisme de "reliquat vanilla obsolète" et de le supprimer,
  vérifier d'abord s'il pose une variable de progression consultée ailleurs (ici, la note de la suite 2
  documentait déjà explicitement ce rôle — elle avait été relue trop vite)
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/LittlerootTown/scripts.inc`, `data/maps/LittlerootTown/map.json`)

## Session 4 (suite 11) — Traduction complète du gauntlet d'Argenta
- Les 7 dresseurs du gauntlet (Randall/Vitesse, Parker/Confusion, George/Soin, Berke/K.O., Mary/Précision,
  Alexia/Défense, Jody/Force), leurs panneaux de porte et le guide d'arène (`GymGuideAdvice`/
  `GymGuidePostVictory`) traduits en français — jusqu'ici laissés en anglais le temps de neutraliser
  d'abord les répliques "gamin du champion" (fait en suite 7)
- Références à PIERRE et "champion d'arène d'Argenta" gardées cohérentes avec le reste du fichier
- Restent volontairement non traduits : les 4 textes de revanche post-Ligue de Pierre
  (`NormanPreRematch`/`NormanRematchDefeat`/`NormanPostRematch`/`NormanRematchNeedTwoMons`, thème "parent
  et enfant" toujours présent) — différé comme déjà noté, non bloquant (post-Ligue uniquement)
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/PetalburgCity_Gym/scripts.inc`)

## Session 4 (suite 12) — Vrai correctif du crash à la sortie de Bourg Palette
- Retour de test : "Même dans la V0.5 ça redémarre automatiquement quand j'essaie de sortir de Bourg
  Palette" — la restauration du verrou de la Jumelle (suite 10) n'a pas suffi, le jeu redémarrait
  toujours en traversant vers la Route 1
- Root cause trouvée par test headless (mGBA/libmgba installé, `tools/qa_harness/qa_runner` recompilé —
  l'ancien blocage "input non détecté" documenté en Session 3 n'existait plus une fois `libmgba-dev`
  correctement installé). Séquence de diagnostic : build `DEBUG=1` avec le menu debug embarqué du moteur
  (`Utilities > Warp to map warp`, `Scripts > Script N`), scripts de test temporaires dans
  `data/scripts/debug.inc` (jamais committés) pour isoler la variable en cause par bissection
- Chaîne de tests qui a permis d'isoler le bug : varier texte (accents/plein texte), position de warp,
  délai avant l'action, dresseurs visibles un par un (Team Rocket, Zigzagoon, Birch/Chen, PNJ générique) —
  jusqu'à isoler `LOCALID_ROUTE101_BIRCH` (le Professeur Chen) comme seul déclencheur, reproductible aussi
  bien par script que par marche normale dans le jeu
- **Cause exacte** : `LOCALID_ROUTE101_BIRCH` utilise `movement_type: MOVEMENT_TYPE_JOG_IN_PLACE_RIGHT`
  (hérité tel quel du Professeur Chen/Birch d'origine). Or son sprite a été remplacé plus tôt cette session
  de développement (`OBJ_EVENT_GFX_PROF_BIRCH` → `OBJ_EVENT_GFX_PROF_OAK`, repris de FRLG, cf. Session 3)
  sans jamais être testé en jeu jusqu'ici — la Jumelle bloquait l'accès à la Route 1 depuis le début, donc
  personne n'avait jamais atteint physiquement ce PNJ avant cette session. Le sprite FRLG de Chen n'a pas
  le même jeu d'animation "jogging sur place" que l'ancien sprite Birch : après quelques secondes d'inactivité,
  le moteur tente de lire une frame d'animation hors des bornes du sprite, corrompant la mémoire et
  provoquant un redémarrage matériel (confirmé dans les logs mGBA : lectures mémoire à des adresses
  invalides comme `0xE3A02024`, juste après un `CpuSet` à adresse source non alignée)
- Vérifié : `LOCALID_ROUTE101_BIRCH` est la SEULE occurrence de `OBJ_EVENT_GFX_PROF_OAK` dans tout le
  dépôt utilisant un movement_type animé (`JOG_IN_PLACE`) — toutes les autres (Bourg Palette, labo,
  Route 103, Route 110, salle du champion, générique FRLG) utilisent des types statiques
  (`FACE_UP`/`FACE_DOWN`/`LOOK_AROUND`), jamais problématiques
- Correctif : `data/maps/Route101/map.json`, `LOCALID_ROUTE101_BIRCH` et `LOCALID_ROUTE101_ZIGZAGOON`
  passés à `MOVEMENT_TYPE_LOOK_AROUND` (type sûr déjà utilisé par le second PNJ Chen du même plan et par
  Route 110). Aucun impact sur la cinématique elle-même : la scène scriptée (fuite, poursuite) pilote leurs
  déplacements explicitement via `applymovement`, ce réglage ne concernait que leur animation d'attente
  par défaut
- Testé et confirmé résolu : traversée complète Bourg Palette → Route 1 → déclenchement de la scène de
  sauvetage du Professeur Chen → dialogue → remise de Pikachu, sans plus aucun redémarrage, reproduit par
  script ET par marche normale dans l'émulateur headless
- Outillage : `tools/qa_harness/qa_runner` remis en état de marche (dépendance `libmgba.so.0.10`
  installée via `apt-get install libmgba-dev mgba-sdl`, recompilation propre). Le blocage d'input
  documenté en Session 3 était bien un faux problème d'environnement, pas un bug du harness lui-même
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/Route101/map.json`)

## Session 4 (suite 13) — Azuria : traduction neutre + décision sur les fils narratifs restants
- Exploration complète de `RustboroCity/scripts.inc` (1290 lignes) : identifié 3 fils narratifs vanilla
  encore non traités, tous interconnectés et s'étendant sur plusieurs cartes (Rustboro/Azuria, Tunnel
  Rusturf, maison de Briney) :
  1. Vol des affaires DEVON CORP par un sbire Team Aqua (musique, sprite, texte inchangés)
  2. Le marin MR. BRINEY / son PEEKO (transport en bateau vers Dewford)
  3. Un combat de rival scripté (May/Brendan = Régis chez nous), conditionné à avoir croisé Briney
- Proposition faite à Thomas avant d'implémenter quoi que ce soit sur ces trois fils, vu leur portée
  multi-cartes — décision : remplacer le sbire par la Team Rocket (même traitement que la forêt de Jade)
- En attendant la conception de ce remplacement (à faire dans une prochaine étape, cf. PROCHAINES ÉTAPES),
  traduit en français tout le contenu neutre et sans dépendance à ces trois fils : panneaux de ville
  (`CitySign`→AZURIA, `TrainersSchoolSign`, `CuttersHouse`), PNJ de discussion générale (arène, école des
  dresseurs, POKéNAV/MATCH CALL, combat 2v2, changement d'apparence par l'XP)
- `RustboroCity_Text_GymLeaderIsntEasyWithFire` corrigé au passage : référençait encore "FEU contre
  ROCHE" (ancien typage de Roxanne) — mis à jour en "FEU contre EAU" pour refléter le typage EAU d'Ondine
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/RustboroCity/scripts.inc`)

## Session 4 (suite 14) — Team Rocket remplace le sbire du vol DEVON CORP (Azuria + Tunnel Rusturf)
- Implémentation du remplacement narratif validé par Thomas ("Je valide continue") : le sbire anonyme
  qui vole les affaires DEVON CORP à Azuria puis se réfugie dans le Tunnel Rusturf devient un membre de
  la Team Rocket, avec une réplique du PNJ DEVON rappelant explicitement la forêt de Jade (cohérence avec
  le fil Team Rocket déjà établi sur cette carte)
- `data/maps/RustboroCity/scripts.inc` traduit et réécrit en français : `WeShortenItToDevon`,
  `OutOfTheWay`, `WaitDontTakeMyGoods`, `HelpMeIWasRobbed` (référence à la forêt de Jade),
  `ShadyCharacterTookOffTowardsTunnel`, `YouGotItThankYou`, `YoureLoadedWithItems`, `PleaseComeWithMe`
- `data/maps/RusturfTunnel/scripts.inc` traduit et réécrit en français : `ComeAndGetSome`, `Peeko`,
  `GruntIntro`, `GruntDefeat`, `GruntTakePackage`, `PeekoGladToSeeYouSafe`, et le long monologue de
  remerciement de MR. BRINEY (`ThankYouLetsGoHomePeeko`)
- Les identifiants internes (noms de symboles `TRAINER_GRUNT_RUSTURF_TUNNEL`, flags, `MUS_ENCOUNTER_AQUA`)
  sont volontairement laissés inchangés — seul le texte visible par le joueur change, conformément à la
  convention déjà suivie pour le remplacement de la forêt de Jade
- Périmètre restant hors de ce correctif, non lié à la Team Rocket : panneaux `DevonCorpSign`/
  `DevonCorpBranchOfficeSign` (encore en anglais), le combat de rival scripté (Régis) et son texte
  d'enregistrement, le sous-fil Wanda/petit ami dans le Tunnel Rusturf, et `Route104_MrBrineysHouse` —
  tous purement neutres (pas de Team Aqua), traduction reportée à une prochaine étape
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/RustboroCity/scripts.inc`, `data/maps/RusturfTunnel/scripts.inc`)

## Session 4 (suite 15) — Combat de rival d'Azuria : bug de branchement corrigé + traduction
- Régression latente trouvée en reprenant le combat de rival scripté d'Azuria (`RustboroCity_EventScript_
  RivalEncounter`) : la carte utilisait encore l'ancien mécanisme vanilla `Common_EventScript_
  SetupRivalGfxId` et un `checkplayergender` pour choisir entre les branches "MAY" et "BRENDAN" — alors
  que Régis a été fixé ailleurs dans le projet (Route 103, `Custom_EventScript_SetupRegisGfxId`) comme
  personnage unique (petit-fils du Professeur Chen) affichant toujours le même sprite, quel que soit le
  genre choisi par le joueur. Sans ce correctif, un joueur masculin aurait vu le sprite "MAY" et
  déclenché la branche de texte "MAY" à Azuria — incohérent avec Régis partout ailleurs dans le jeu
- Corrigé : `RustboroCity_OnTransition` appelle désormais `Custom_EventScript_SetupRegisGfxId` ;
  `RustboroCity_EventScript_RivalEncounter` et `RustboroCity_EventScript_PlayRivalMusic` vont
  directement vers la branche "Brendan" (celle qui utilise le sprite et la musique de Régis), sans plus
  jamais tester le genre du joueur
- Traduit en français toute la branche de texte désormais utilisée (`BrendanHiLetsRegister`,
  `RegisteredBrendan`, `BrendanPassedBrineyWantToBattle`, `BrendanNoConfidenceInPokemon`,
  `BrendanWantToBattle`, `BrendanIWontGoEasy`, `BrendanDefeat`, `BrendanMrBrineyHint`) avec le label
  "RÉGIS :" ; référence à "PETALBURG WOODS" adaptée en "forêt de Jade"
- La branche "MAY" (`RustboroCity_EventScript_MayEncounter` et ses textes) devient du code mort
  inatteignable — volontairement laissée en l'état (non supprimée) par prudence, conformément à la
  leçon retenue en suite 10 : ne pas retirer un mécanisme sans être sûr qu'il n'est réellement plus
  sollicité nulle part ; elle ne sera plus jamais atteinte en jeu
- Traduit au passage : `RustboroCity_Text_DevonCorpSign`, `DevonCorpBranchOfficeSign`,
  `TunnelNearingCompletion` (panneaux neutres, sans lien avec la Team Rocket)
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/RustboroCity/scripts.inc`)

## Session 4 (suite 16) — Le bug de branchement Régis touchait 9 cartes, pas seulement Azuria
- En creusant le correctif de suite 15, découverte que le même bug (branchement vanilla May/Brendan
  selon `checkplayergender` au lieu du personnage fixe Régis) touchait `Common_EventScript_
  SetupRivalGfxId` — donc potentiellement le sprite de Régis — sur 9 cartes au total : `LittlerootTown`,
  `LittlerootTown_ProfessorBirchsLab`, `OldaleTown`, `Route104`, `Route110`, `Route119`, `LavaridgeTown`,
  `LilycoveCity`, `EverGrandeCity_ChampionsRoom`
- Corrigé partout : les 9 cartes appellent désormais `Custom_EventScript_SetupRegisGfxId` au lieu de
  `Common_EventScript_SetupRivalGfxId` — le sprite de Régis est maintenant cohérent sur l'ensemble du
  jeu, quel que soit le genre choisi par le joueur (avant ce correctif, un joueur masculin aurait vu le
  sprite "MAY" comme rival dans toutes ces scènes)
- Corrigé aussi le routage de texte (le `checkplayergender` qui choisissait entre la branche "MAY" et la
  branche "BRENDAN" pour le dialogue) dans les 7 cartes qui avaient un vrai combat/scène de rival
  scriptée avec du texte dupliqué : `LittlerootTown_ProfessorBirchsLab` (5 points de branchement),
  `Route104`, `Route110`, `LavaridgeTown`, `LilycoveCity`, `EverGrandeCity_ChampionsRoom` (en plus
  d'Azuria, déjà fait en suite 15) — toutes redirigent maintenant directement vers la branche "Brendan"
  (celle qui porte l'identité de Régis)
- Traduit en français le texte de `LittlerootTown_ProfessorBirchsLab` réellement atteignable à court/
  moyen terme : `BirchRivalGoneHome`, `HeardYouBeatRivalTakePokedex`, `ExplainPokedex`,
  `CountlessPokemonAwait` (texte du Professeur Chen), et toute la branche Brendan/Régis désormais
  utilisée (`BrendanGotPokedexTooTakeThese`, `CatchCoolPokemonWithPokeBalls`, `HeyYourBagsFull`,
  `BrendanWhereShouldIGoNext`, `BrendanTakeBreakFromFieldwork`, `BrendanYouCanThankMe`,
  `BrendanPreferCollectingSlowly`, `BrendanHaveYouGoneToBattleFrontier`)
- Les branches "MAY" (`LittlerootTown_ProfessorBirchsLab_EventScript_May*`) dans ce fichier deviennent du
  code mort inatteignable, volontairement laissées en l'état (même prudence qu'en suite 15)
- **Périmètre non traité, identifié mais reporté** :
  - `Route104`, `Route110`, `LavaridgeTown`, `LilycoveCity`, `EverGrandeCity_ChampionsRoom` : le texte de
    la branche Brendan/Régis reste en anglais (seul le routage a été corrigé) — traduction à faire dans
    une prochaine étape
  - Contenu très tardif de `LittlerootTown_ProfessorBirchsLab` (post-Ligue/Elite 4) resté en anglais :
    cérémonie de mise à niveau du POKéDEX NATIONAL, choix du starter Johto, appel de Scott (S.S. Tidal)
  - D'autres occurrences isolées du même schéma de branchement `checkplayergender`/MAY trouvées par grep
    (`LittlerootTown_MaysHouse_2F`, `MossdeepCity_SpaceCenter_2F`, `OldaleTown`, `Route101`, `Route119`)
    n'ont pas été auditées cette fois — à vérifier une par une avant de conclure s'il s'agit du même bug
    ou d'un usage différent (ex. logement du joueur selon son propre genre, qui lui est légitime)
- Build validé (compilation propre, exit code 0) après chaque étape, ROM 79,01 %, `changed_files/`
  synchronisé pour les 9 cartes touchées

## Session 4 (suite 17) — Traduction de la branche Régis sur Route 104 et Route 110
- Traduit en français le texte de la branche Régis (routage déjà corrigé en suite 16, seul le texte
  restait en anglais) sur `Route104` (enregistrement POKéNAV, combat, conseil post-combat sur
  l'amitié avec les POKéMON — référence à MR. BRINEY conservée, cohérente avec le fil Tunnel Rusturf) et
  `Route110` (combat sur CYCLING ROAD, remise du DÉTECTEUR d'objets)
- Le DÉTECTEUR (`ITEM_DOWSING_MACHINE`) est traduit uniquement dans le dialogue, comme la SUPER BALL
  ailleurs dans le projet — le nom d'objet lui-même dans `items.h` reste en anglais ("Dowsing Machine"),
  la traduction complète de la base d'objets (873 entrées) est un chantier séparé, non commencé
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/Route104/scripts.inc`, `data/maps/Route110/scripts.inc`)

## Session 4 (suite 18) — Fin de la traduction de la branche Régis + 2 reliquats "papa" trouvés
- Traduit en français le texte de la branche Régis (routage déjà corrigé en suite 16) sur `LavaridgeTown`
  (remise des GO-GOGGLES), `LilycoveCity` (combat, discussion sur la suite du jeu), et
  `EverGrandeCity_ChampionsRoom` (scène finale post-victoire contre le MAÎTRE DE LA LIGUE)
- Deux nouveaux reliquats "papa champion d'arène" trouvés et corrigés au passage (même travers que Pierre/
  Norman traité en suite 7, jamais audité sur ces cartes tardives) :
  - `LavaridgeTown_Text_BrendanExplainGoGogglesChallengeDad` : "je vais défier NORMAN, champion d'arène
    de PETALBURG, ton père" → réécrit sans lien de parenté ("PIERRE, le champion de l'arène d'Argenta")
  - `EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex` (discours du Professeur Chen à l'arrivée
    dans la salle du Champion) : "tu as battu ton propre père à l'arène de PETALBURG" → réécrit
    ("tu as battu PIERRE, le champion d'Argenta")
- Également corrigé : Régis appelait le Professeur Chen "mon père" dans deux textes de `LilycoveCity`
  (`BrendanShoppingLetsBattle`, `BrendanGoingBackToLittleroot`) — incohérent avec la relation
  grand-père/petit-fils établie ailleurs dans le projet (Chen est le grand-père de Régis, pas son père) ;
  remplacé par "mon grand-père"
- `EverGrandeCity_ChampionsRoom_Text_BirchArriveRatePokedex`/`BirchCongratulations`/
  `WallaceWaitOutside` : label "PROF. BIRCH" remplacé par "PROFESSEUR CHEN", cohérent avec le reste du
  jeu ; "CHAMPION"/le titre du protagoniste victorieux traduit en "MAÎTRE DE LA LIGUE"
  ("WALLACE", le nom du champion final, laissé tel quel — personnage de fin de jeu jamais encore traité
  dans les docs, pas de nom français décidé)
- Bug de compilation trouvé et corrigé en cours de route : deux tirets cadratins (—) introduits par
  erreur dans `LilycoveCity/scripts.inc` — le charmap du jeu ne les supporte pas (`unknown character
  U+2014`), remplacés par une simple virgule, cohérent avec la règle du projet ("pas de tiret cadratin")
- Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé
  (`data/maps/LavaridgeTown/scripts.inc`, `data/maps/LilycoveCity/scripts.inc`,
  `data/maps/EverGrandeCity_ChampionsRoom/scripts.inc`)

## Session 4 (suite 19) — Retour de test v0.7 : camion corrigé, crash Route 1 confirmé et diagnostiqué
Retour de Thomas sur la v0.7 : (1) l'animation de tremblement du camion est toujours présente au tout
début d'une nouvelle partie ; (2) le jeu redémarre à la 2e/3e phrase du dialogue de rencontre avec le
Professeur Chen sur la Route 1.

**Bug 1 — tremblement du camion : corrigé.**
- Cause trouvée dans `src/overworld.c`, `CB2_NewGame()` (le point d'entrée de toute nouvelle partie) :
  pour les builds non-FRLG (donc notre base Émeraude), le moteur fixait inconditionnellement
  `gFieldCallback = ExecuteTruckSequence` — la cinématique de caméra/tremblement du camion vanilla —
  quelle que soit la carte de destination réelle. `WarpToPlayerBedroom()` (`src/new_game.c`, déjà
  adapté en Session 2/3 pour faire apparaître le joueur directement dans sa chambre, sans camion) change
  la DESTINATION du warp mais ne touche jamais à ce champ de rappel, qui continuait donc à tourner en
  arrière-plan sur toute nouvelle partie, indépendamment de la carte réellement chargée
- Corrigé : `gFieldCallback` utilise désormais `FieldCB_WarpExitFadeFromBlack` (le fondu de sortie de
  warp standard, déjà utilisé par les builds FRLG et à de nombreux autres endroits du moteur) au lieu de
  `ExecuteTruckSequence`, pour toutes les nouvelles parties
- `ExecuteTruckSequence`/`Task_Truck1`/`2`/`3` (dans `field_special_scene.c`) ne sont plus jamais
  appelés mais volontairement laissés intacts (code mort, pas de risque à les garder)
- Build validé, testé en isolation (pas de régression sur le warp standard vers la chambre)

**Bug 2 — crash au dialogue de rencontre avec le Professeur Chen : confirmé, cause profonde non
identifiée malgré un diagnostic approfondi.**
- Reproduit de façon fiable en headless (`tools/qa_harness/qa_runner`, build `DEBUG=1` + fonctionnalité
  `Quickstart` du moteur pour atteindre l'overworld sans avoir à scripter l'écran de saisie du nom) :
  chaque `warp` vers `MAP_ROUTE101` suivi de la présence active de l'objet `LOCALID_ROUTE101_BIRCH`
  (le Professeur Chen, sprite `OBJ_EVENT_GFX_PROF_OAK`) déclenche la même signature de corruption
  mémoire que celle documentée en suite 12 (lectures `GBA Memory: Bad memory Load...` avec des valeurs
  qui ressemblent à des opcodes ARM, précédées d'appels BIOS `SWI 0x0C` CpuFastSet) — cohérent avec un
  crash matériel réel, pas un artefact de l'émulateur
- Plus de 30 builds de test ont permis d'ISOLER le comportement mais pas sa cause exacte. Cause
  RETENUE comme fausse piste, testée et éliminée individuellement (le crash persiste identique dans
  tous les cas) :
  - Le sprite substitué (`OBJ_EVENT_GFX_PROF_OAK`, porté depuis FRLG) — remplacé par `OBJ_EVENT_GFX_MAN`
    (sprite natif, sain, utilisé partout ailleurs sans problème) : crash identique
  - Le mécanisme d'animation "jog in place" déjà corrigé en suite 12 (`movement_type`) — testé
    `LOOK_AROUND` (valeur actuelle) et `FACE_DOWN` : crash identique dans les deux cas
  - La position de l'objet sur la carte (testé à 3 positions différentes, dont une à l'autre bout de
    la carte) : crash identique partout SAUF quand le joueur apparaît assez loin pour que l'objet ne
    soit pas chargé/actif (auquel cas, logiquement, aucun crash)
  - L'élévation de l'objet (0 vs 3, correspondant à l'instance qui fonctionne sans problème dans le
    labo du Professeur Chen) : crash identique
  - Le numéro d'ID local de l'objet (`LOCALID_ROUTE101_BIRCH`, testé avec sa valeur d'origine et avec
    une valeur arbitraire différente) : crash identique
  - Le script attaché à l'objet (`0x0` vs un script valide) : crash identique
  - Le nom/l'ID du flag qui contrôle sa visibilité : crash identique
  - Sa position dans le tableau `object_events` de la carte (déplacé en dernière position) : crash
    identique
  - Le mécanisme d'apparition (déclaré directement dans `map.json` vs `addobject` dynamique au moment
    du déclenchement scripté) : crash identique
  - La présence d'objets voisins (Zigzagoon, trio Team Rocket) — masqués un par un ou ensemble : aucune
    combinaison ne change le résultat, sauf masquer `LOCALID_ROUTE101_BIRCH` lui-même
  - Le réglage moteur `OW_GFX_COMPRESS` (compression des graphismes overworld, recommandé à `FALSE` en
    cas de pression VRAM par la doc du moteur) : testé à `FALSE`, crash quasi identique (grossit
    fortement la ROM sans résoudre le problème — revert immédiat)
- **Seul levier qui élimine le crash à coup sûr** : que `LOCALID_ROUTE101_BIRCH` ne soit tout simplement
  jamais actif près du joueur (masqué en permanence, ou joueur trop loin). Inutilisable tel quel comme
  correctif définitif puisque cette scène (arrivée du Professeur Chen, remise de PIKACHU) est centrale
  au tout début du jeu
- Confirmé par élimination que ce n'est PAS le même bug que celui corrigé en suite 12 (qui portait sur
  le `movement_type` par défaut d'IDLE) : ce nouveau crash apparaît dès l'activation de l'objet, avant
  toute animation ou inactivité prolongée, et persiste même avec un `movement_type` sûr
- **Conclusion honnête** : je n'ai pas trouvé la cause exacte malgré une élimination quasi exhaustive de
  toutes les hypothèses testables en boîte noire (config JSON, scripts, réglages moteur). Il me manque
  un outil de débogage bas niveau (registres CPU/désassemblage au moment du crash) pour aller plus loin
  — `mgba-sdl` est installé mais sans interface graphique ni serveur GDB accessibles dans cet
  environnement. Toutes les modifications de diagnostic (map.json, debug.inc, overworld.h) ont été
  intégralement annulées avant de livrer cette build ; seul le correctif du camion (Bug 1, confirmé et
  sûr) est inclus
- Prochaine étape recommandée : reprendre ce diagnostic avec un accès à un débogueur bas niveau
  (GDB via mgba, ou test sur un environnement avec interface graphique), ou envisager une refonte
  structurelle de la scène de sauvetage (ex. déplacer la scène sur une "carte de cinématique" dédiée,
  technique classique des jeux Pokémon pour les scènes scriptées à risque)

## Session 4 (suite 20) — Cause racine trouvée et corrigée : bug moteur dans `applymovement`, scène
## de sauvetage du Professeur Chen déplacée sur Bourg Palette

Thomas a envoyé une vidéo de l'écran de son téléphone montrant le crash en conditions réelles
(émulateur "Manic EMU") : le dialogue "À-à l'aide !" s'affiche correctement en français, puis
l'écran devient blanc et le jeu revient à l'écran de copyright Nintendo/Game Freak — moins d'une
seconde après le début du dialogue, dès que le script commence à animer le Professeur Chen. Ceci
confirme exactement la reproduction obtenue en headless depuis la suite 19. Puis consigne explicite
de Thomas : « changeons la rencontre avec Chen alors pour supprimer tout problème ».

**Cause racine identifiée** (après un nouveau cycle de diagnostic très approfondi, plus de 50 builds
de test supplémentaires) : ce n'est **pas** un problème propre à la Route 1, ni au sprite du
Professeur Chen, ni à un réglage de carte. C'est un **bug du moteur pokeemerald-expansion** dans
`ScrCmd_applymovement` (`src/scrcmd.c`) :

- `applymovement` résout l'objet cible via `GetObjectEventIdByLocalId(localId)`
  (`src/event_object_movement.c`), une fonction qui parcourt `gObjectEvents[]` (le pool des objets
  actuellement chargés, 16 emplacements) et retourne le premier objet dont l'ID local correspond —
  **sans jamais vérifier à quelle carte cet objet appartient**
- À l'inverse, `setobjectxy` utilise `TryGetObjectEventIdByLocalIdAndMap`, la variante qui filtre
  correctement par carte (numéro de carte + groupe de carte)
- Près d'une connexion entre deux cartes (ex. la bordure nord de Bourg Palette, connectée à la
  Route 1), les objets des DEUX cartes sont simultanément actifs dans `gObjectEvents[]` pour les
  besoins de l'affichage/défilement. Si les ID locaux se recoupent entre les deux cartes,
  `applymovement` peut donc cibler l'objet de la MAUVAISE carte
- Preuve : les objets de test avec ID local 2, 3, 4, 5 ou 6 sur Bourg Palette plantaient
  systématiquement dès `applymovement` — exactement les ID locaux déjà utilisés par les propres
  objets de la Route 1 (`1`=Dresseur Jeunot, `2`=le Professeur Chen "post-jeu", `3`=Garçon,
  `4`/`5`/`6`=le trio Team Rocket). Seul l'ID local `1` (la Jumelle, sans équivalent sur la Route 1)
  était épargné. Objet de test avec un ID local libre (`7`+, n'existant sur aucune des deux cartes) :
  plus aucun plantage lié à la collision — mais un second facteur est apparu (voir ci-dessous)
- **Second facteur, indépendant** : même avec un ID local sans collision, `applymovement` plantait
  encore si l'objet se trouvait physiquement près de la bordure nord (essentiellement partout où le
  jeu pourrait charger les objets de la Route 1 en mémoire). Décalage précis établi par dichotomie :
  sûr à partir de `y=13` (sur une carte de 20 cases de haut), dangereux en dessous. Cause exacte non
  identifiée avec certitude (probablement liée au même mélange objets Route 1 / Bourg Palette dans
  `gObjectEvents[]`, via un mécanisme différent non totalement isolé), mais le comportement est
  parfaitement reproductible et stable
- Ceci explique aussi, rétrospectivement, le crash original sur la Route 1 lui-même : Route101 est
  connectée à plusieurs cartes (Bourg Palette au sud, Jadielle à l'ouest), donc le même bug de
  collision d'ID locaux ou de proximité de bordure s'y appliquait tout aussi bien

**Correctif appliqué** (conformément à la demande de Thomas de changer la rencontre pour éliminer le
problème) : toute la scène (course-poursuite Chen/Ramoloss puis remise du PIKACHU) est déplacée de la
Route 1 vers la zone sud de Bourg Palette, près du Labo du Professeur Chen — la zone la plus éloignée
possible de la connexion Route 1, jamais mise en défaut dans les tests :
- 3 nouveaux objets sur `LittlerootTown/map.json` : `LOCALID_LITTLEROOT_CHEN_RESCUE` (Professeur Chen,
  départ 8,18), `LOCALID_LITTLEROOT_ZIGZAGOON_RESCUE` (Ramoloss sauvage, départ 8,19),
  `LOCALID_LITTLEROOT_BIRCHS_BAG` (le sac avec la POKé BALL, position fixe 9,16) — leurs ID locaux
  (7, 8, 9) sont garantis sans collision avec la Route 1 (qui plafonne à 6), et leur zone de
  déplacement pendant la course-poursuite reste entièrement au sud de `y=13`
- `LittlerootTown_EventScript_GoSaveBirchTrigger` (déclenchée par la Jumelle près de l'entrée nord,
  zone déjà validée sûre) enchaîne désormais sur `LittlerootTown_EventScript_StartBirchRescue`
  (nouveau, reprend le texte et les gabarits de mouvement approuvés, réadaptés à la nouvelle zone) puis
  `LittlerootTown_EventScript_BirchsBag` (remise du PIKACHU, texte inchangé)
- Route101 est nettoyée des 3 anciens objets (Chen, Ramoloss, sac) et de leurs déclencheurs de zone —
  les gabarits de mouvement et textes associés restent dans `Route101/scripts.inc` (réutilisés depuis
  Bourg Palette, les labels `::` sont visibles globalement) avec un commentaire expliquant la
  relocalisation
- Vérifié en headless (`qa_runner`, build `DEBUG=1`, `Quickstart` + menu debug) : chaîne complète
  `GoSaveBirchTrigger` → `StartBirchRescue` → `BirchsBag` (avertisseurs de dialogue compris, boutons A
  simulés), **0 instance de "Bad memory" sur plusieurs exécutions consécutives**, contre 136 avant le
  correctif de position/ID
- Build release validé (`make MODERN=1`, exit code 0, ROM 79,01 %, taille identique à la référence)

**Reste en marge (non corrigé, risque latent documenté)** : l'objet "Professeur Chen post-jeu" sur
Route101 (position 5,11, script `ProfBirch_EventScript_RatePokedexOrRegister`, ID local 2) utilise le
même sprite `OBJ_EVENT_GFX_PROF_OAK` que celui qui posait problème, et son ID local (2) collisionne
avec Bourg Palette (Villageois gros ventre). Il n'est actuellement ciblé par aucun `applymovement`
donc ne devrait pas déclencher le bug — mais toute future modification qui ajouterait un mouvement
scripté sur cet objet devra d'abord vérifier l'absence de collision d'ID local avec les cartes
connectées (Bourg Palette au sud, Jadielle à l'ouest).

## Session 4 (suite 21) — Combat de rival de la Route 119 (CS Vol) converti en Régis

Suite de l'audit des branches `checkplayergender` May/Brendan commencé en suite 20. Trois occurrences
supplémentaires identifiées comme du vrai contenu à corriger (pas de simples doublons comme Jadielle) :
Route 119 (deuxième combat de rival, remise de la CS Vol), la scène de rencontre du rival dans sa
chambre à Bourg Palette (2F), et un appel post-Ligue à Mossdeep (contenu jamais traduit). Priorité
donnée à la Route 119, sur consigne de Thomas, contenu actif tôt dans le jeu.

- `Route119_EventScript_RivalEncounter` simplifiée : suppression de la branche
  `checkplayergender`/`PlayMayMusic`/`PlayBrendanMusic` (musique unique, celle déjà utilisée pour
  Régis ailleurs), fusion de `BattleMay`/`BattleBrendan` en une seule `Route119_EventScript_BattleRegis`
- Le `switch VAR_STARTER_MON` (Treecko/Torchic/Mudkip) a été supprimé : cette variable vaut toujours 1
  (index "Torchic") dans Kanto Saison 1 puisque le starter unique est Pikachu (cf.
  `LittlerootTown_EventScript_BirchsBag`), les branches Treecko/Mudkip n'étaient donc jamais atteignables
- Équipe de dresseur reprise telle quelle de `TRAINER_BRENDAN_ROUTE_119_TORCHIC` (Lombre/Slugma/
  Marshtomp), cohérent avec le choix déjà fait pour le premier combat de rival (Route 103) — pas de
  rééquilibrage de l'équipe pour cette suite, à revoir si Thomas le juge trop/pas assez difficile
- 4 textes traduits en français dans la voix déjà établie de Régis (cf. Route 103 : tutoiement, ton
  compétitif mais jamais malveillant) : intro du combat, texte de défaite, remise de la CS Vol,
  explication de VOL. La référence à "FORTREE" (arène qui débloque VOL) est laissée telle quelle,
  cette ville n'a pas encore été renommée dans le projet — hors périmètre de cette suite
- **Bug annexe trouvé et corrigé en le croisant** : `Common_EventScript_SetupRivalOnBikeGfxId`
  (graphisme du rival à vélo, utilisé uniquement dans les scènes où il arrive en vélo avant de
  descendre pour le combat) n'avait jamais été converti en variante Régis fixe, contrairement au
  graphisme standard (`Custom_EventScript_SetupRegisGfxId`, déjà en place depuis plusieurs suites).
  Trouvé sur 3 cartes : Route 119, Route 110 (Cycling Road), LavaridgeTown (Arc post-Ligue). Nouvelle
  fonction `Custom_EventScript_SetupRegisOnBikeGfxId` ajoutée dans `data/scripts/rival_graphics.inc`
  (réutilise le sprite vélo de Brendan, cohérent avec le sprite debout) et substituée aux 3 endroits
- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)
- Reste dans le backlog (non traité cette suite) : la scène de rencontre du rival à Bourg Palette
  (maison 2F, cutscene complète avec mouvements et PC) et l'appel post-Ligue de Mossdeep (contenu
  encore en anglais)

## Session 4 (suite 22) — Scène de rencontre du rival à Bourg Palette (maison 2F) traduite

Deuxième gros morceau du même audit May/Brendan. Point important vérifié avant de toucher au code :
le genre du joueur reste bien sélectionnable à la création de partie (`src/oak_speech.c`, écran de
choix garçon/fille intact), donc la structure à deux maisons miroir (`LittlerootTown_MaysHouse_2F` /
`LittlerootTown_BrendansHouse_2F`, qui détermine laquelle est "chez toi" et laquelle est "chez le
rival" selon le genre choisi) reste un mécanisme légitime et n'a pas été touchée. Seul le contenu du
PERSONNAGE rival (son texte, sa personnalité) a été unifié : quelle que soit la maison où on le
rencontre, c'est toujours Régis, avec une seule voix.
- Les 8 textes du rival (accueil surprise dans sa chambre, phrase courte en le recroisant, dialogue
  post-Lilycove sur le POKéDEX, phrase de "où aller ensuite") vivaient tous dans un seul fichier
  (`LittlerootTown_MaysHouse_2F/scripts.inc`, référencés depuis les deux maisons via labels `::`
  globaux — convention déjà en place dans le fichier). Les variantes "May" et "Brendan" de chaque
  texte sont maintenant identiques mot pour mot (voix de Régis, cf. Route 103/Route 119), les 5 qui
  étaient encore en anglais ont été traduites
- Aucune restructuration de script (pas de fusion des `EventScript_TryUpdateMayPos`/
  `CheckSetReadyToMeetMay`/`CheckInitDecor`/`EventScript_PC` : ces fonctions décident laquelle des deux
  maisons est celle du rival selon le genre du joueur, toujours nécessaire)
- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)
- Reste dans le backlog : l'appel post-Ligue de Mossdeep (`MossdeepCity_SpaceCenter_2F`, contenu
  entièrement en anglais, hors priorité)

## Session 4 (suite 23) — Changement de programme : suppression complète de la scène de sauvetage,
## remise directe de PIKACHU au labo, Quickstart automatique pour les builds de test

Nouvelle consigne de Thomas : au lieu de contourner le crash de la Route 1 en déplaçant la scène de
sauvetage à Bourg Palette (suite 20-22), la supprimer intégralement et remettre PIKACHU directement
en discutant avec le Professeur Chen dans son laboratoire. Plus une carte de Bourg Palette avec 5
maisons supplémentaires (reporté — pas d'éditeur de carte visuel dans cet environnement, voir
ci-dessous) et un démarrage automatique pour les builds de test.

**1. Suppression complète de la scène de sauvetage**
- Bourg Palette (`LittlerootTown/map.json`) : les 3 objets de la scène (Professeur Chen, Ramoloss
  sauvage, sac de PIKACHU) supprimés, ainsi que le déclencheur `GoSaveBirchTrigger`
- Route 1 (`Route101/scripts.inc`) : tout le code mort restant de l'ancienne scène supprimé
  (gabarits de mouvement de la course-poursuite, textes "à l'aide"/"tu m'as sauvé", scripts
  `PreventExit{South,West,North}` qui bloquaient la sortie pendant la scène — plus aucun `coord_event`
  ne les référençait déjà depuis la suite 20)
- Bourg Palette (`LittlerootTown/scripts.inc`) : la Jumelle ne parle plus d'un "vacarme" sur la
  Route 1 — elle indique simplement le chemin du labo tant qu'on n'a pas de POKéMON. Les textes
  encore en anglais de cette scène (avertissement "attention aux hautes herbes", "bonne chance") ont
  été traduits au passage
- Le verrou "impossible de sortir sans POKéMON" (la Jumelle qui repousse le joueur) fonctionnait
  seulement à l'état `VAR_LITTLEROOT_TOWN_STATE == 0` ; comme ce nouvel enchaînement peut laisser le
  joueur à l'état 1 (a rencontré son rival) sans PIKACHU, le déclencheur a été dupliqué pour couvrir
  aussi l'état 1, sans quoi le joueur aurait pu sortir sur la Route 1 sans POKéMON

**2. Nouvel événement : remise directe de PIKACHU au labo**
- Le Professeur Chen (`LittlerootTown_ProfessorBirchsLab_EventScript_Birch`) donne maintenant
  directement PIKACHU si le joueur n'a pas encore de POKéMON (`FLAG_SYS_POKEMON_GET`), avant de
  retomber sur son ancien comportement (post-jeu, appels PokéNav, etc.)
- Réutilise presque entièrement une mécanique déjà présente et jusque-là inexploitée :
  `GiveStarterEvent` (commentaire d'origine : "the starter is technically given prior to this...
  this is just where the game tells you it's yours") gérait déjà tout l'enchaînement surnom + "va
  voir mon petit-fils" — il ne manquait que la remise du POKéMON elle-même, ajoutée dans une nouvelle
  fonction `GiveDirectPikachu` qui fait le `givemon` puis enchaîne dessus
- Texte de Chen (`LittlerootTown_ProfessorBirchsLab_Text_LikeYouToHavePokemon`, déjà écrit en
  français en suite 9) réécrit pour retirer les références à la scène de sauvetage ("vu comment tu
  t'es débrouillé tout à l'heure…", "en remerciement de m'avoir aidé…"), remplacées par une
  explication plus simple (Chen a été prévenu par la mère du joueur, il ne restait que PIKACHU)
- **Bug moteur non résolu trouvé et contourné** : déclarer le Professeur Chen visible par défaut
  dans `map.json` (`flag: "0"`) — ou le rendre visible via un `clearflag` précoce — fait planter le
  jeu au chargement du labo (720+ instances de corruption mémoire en headless, reproduit sur
  plusieurs variantes de test, y compris avec un graphisme de remplacement neutre : donc pas
  spécifique au sprite du Professeur Chen). L'ajouter dynamiquement par script via `addobject`,
  comme le fait déjà le reste de ce fichier pour ses autres apparitions tardives (upgrade Pokédex
  national, starters Johto), est sûr : 0 crash en isolation. Nouvelle fonction
  `ShowBirchBeforeStarter` appelée depuis `OnTransition`, qui l'ajoute par script dès la première
  visite tant que le joueur n'a pas encore son POKéMON
  - Point d'honnêteté : un résidu très réduit (8 instances de "Bad memory" sur des dizaines de
    milliers de frames testées, ne grossit jamais quel que soit le scénario de test, jamais de
    corruption visible ni de redémarrage observé dans aucun test, y compris via interaction réelle
    au bouton A) persiste dans certains enchaînements combinant `addobject` du Professeur Chen et
    l'affichage d'un message juste après. Non résolu faute de piste supplémentaire et de temps ; le
    jeu continue de fonctionner normalement dans tous les tests effectués. À surveiller lors du
    playtest réel — prévenir immédiatement si un plantage survient en parlant au Professeur Chen au
    labo
- `checkplayergender` + `Route101_EventScript_HideMayInBedroom`/`HideBrendanInBedroom` conservés
  (cachent la scène "PokéBall du rival" dans sa chambre, maintenant résolue au même moment que la
  remise du PIKACHU du joueur)

**3. Quickstart automatique pour les builds de test**
- Nouveau réglage `QUICKSTART_AUTO` (`include/config/quickstart.h`, `TRUE` par défaut) : les builds
  de test envoyées à Thomas sautent maintenant directement en jeu au démarrage, sans qu'il ait besoin
  d'appuyer sur SELECT ni de repasser par la création de personnage à chaque test. Implémenté dans
  `Task_TitleScreenPhase3` (`src/title_screen.c`), déclenche `Quickstart()` automatiquement dès que
  l'écran-titre atteint sa phase de saisie (les écrans de copyright Nintendo/GAME FREAK restent
  visibles quelques secondes, incompressibles sans toucher au tout début du boot — le reste
  (écran-titre, création de personnage) est entièrement sauté)
  - **Commentaire dans le code, à ne pas oublier** : ce réglage doit repasser à `FALSE` avant toute
    build destinée à un vrai playtest narratif (elle sauterait alors systématiquement l'écran-titre)
  - Vérifié en headless : démarrage à zéro appui, sans aucune entrée, le jeu atterrit directement dans
    la chambre du joueur
  - Ce réglage est indépendant de `ENABLE_QUICKSTART`/`QUICKSTART`, qui reste désactivé sur les
    builds RELEASE (`RELEASE=1`) — nos builds de test ne définissent pas ce flag donc Quickstart (et
    son déclenchement automatique) y reste actif

**4. Carte de Bourg Palette avec 5 maisons supplémentaires : reporté**
- Cet environnement ne dispose d'aucun éditeur de carte visuel (pas de Porymap, pas d'interface
  graphique) ; ajouter des bâtiments signifie éditer à l'aveugle les données de tuiles/collision
  brutes (`map.bin`) et créer une carte d'intérieur complète par maison. Vu le risque d'erreurs
  difficiles à repérer sans retour visuel, Thomas a préféré reporter ce chantier en attendant un
  moyen d'édition visuelle (ex. Porymap en local)

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 24) — Bourg Palette : extension sud avec 5 nouvelles maisons

Thomas a fourni un plan détaillé (carnet de cartographie avec grille 24×18 et esquisses d'intérieur)
pour 5 maisons supplémentaires : Mme Chen (voisine), le Vieux Dresseur, le Gardien de Route, une
maison aux volets fermés (accroche narrative future) et la Dame aux Baies. Comme annoncé en suite 23,
aucun éditeur visuel n'est disponible ; ce chantier a finalement été fait en éditant directement les
données JSON/binaires, avec vérification visuelle via captures d'écran `qa_runner` (pas de Porymap,
mais pas nécessaire non plus : `tools/mapjson` régénère tous les en-têtes/événements C à partir des
`.json` au moment du `make`, donc éditer le JSON à la main suffit — seule la grille de tuiles brute
(`map.bin`) demandait un script Python).

**1. Stratégie retenue : extension additive plutôt que relocalisation complète**
- Le plan de Thomas prévoyait une carte 24×18 réorganisée en profondeur. Risque identifié :
  `LittlerootTown` et `MAP_ROUTE101` font toutes deux exactement 20×20 avec une connexion nord à
  l'offset 0 — élargir la largeur aurait décalé cette connexion et pu laisser un "trou" visuel au
  raccord. A la place : toutes les maisons/PNJ/panneaux existants restent à leur place exacte, et les
  5 nouvelles maisons sont ajoutées dans une extension purement additive au sud (hauteur de la carte
  20 → 34 lignes). Aucun risque sur la connexion Route 1, aucune maison existante déplacée
- Extension générée par un script Python (`struct.pack`/`unpack` sur `data/layouts/LittlerootTown/map.bin`,
  format `u16` par tuile : `metatileId = v & 0x3FF`, `collision = (v>>10)&0x3`, `elevation = (v>>12)&0xF`) :
  décodage de la grille existante pour en extraire tel quel (valeurs brutes copiées, pas
  réinterprétées) le bloc de maison 5×5 déjà utilisé par la maison de Régis/du joueur, puis
  réutilisation de ce bloc à 5 nouveaux emplacements (3 maisons en rangée haute lignes 20-24,
  2 maisons en rangée basse lignes 27-31), et de la ligne de bordure d'arbustes déjà utilisée en bas
  de carte, redupliquée comme nouvelle bordure sud (ligne 33). `data/layouts/layouts.json` :
  `LAYOUT_LITTLEROOT_TOWN` hauteur 20 → 34
- `LittlerootTown/map.json` : 5 nouveaux `warp_events` (un par porte) et 5 nouveaux `bg_events` de
  type panneau (`MAISON DE MME CHEN`, etc., textes ajoutés dans `LittlerootTown/scripts.inc`)

**2. Cinq nouvelles cartes d'intérieur, réutilisant des layouts génériques existants**
- Plutôt que de générer 5 nouvelles grilles de tuiles d'intérieur à l'aveugle, réutilisation de
  layouts d'intérieur génériques déjà présents dans le jeu de base et utilisés par des dizaines de
  maisons à travers Hoenn (`LAYOUT_HOUSE1`-`LAYOUT_HOUSE4`, `LAYOUT_FORTREE_CITY_HOUSE1`) — contenu
  de tuiles déjà prouvé fiable, seule la couche `map.json`/`scripts.inc` est nouvelle par maison :
  - `LittlerootTown_MmeChenHouse` (`LAYOUT_HOUSE1`) — conseils de voisinage
  - `LittlerootTown_VieuxDresseurHouse` (`LAYOUT_HOUSE2`) — conseils de combat, mur de trophées
  - `LittlerootTown_GardienRouteHouse` (`LAYOUT_HOUSE3`) — entretien de la Route 1
  - `LittlerootTown_MaisonVoletsFermes` (`LAYOUT_HOUSE4`) — presque vide, un mot énigmatique sur la
    table (accroche pour un futur arc), pas de PNJ intentionnellement
  - `LittlerootTown_DameBaiesHouse` (`LAYOUT_FORTREE_CITY_HOUSE1`) — culture de Baies
- Chaque maison enregistrée dans `data/maps/map_groups.json` (`gMapGroup_IndoorLittleroot`) et son
  `scripts.inc` ajouté à la liste d'inclusion `data/event_scripts.s` (nécessaire : contrairement aux
  fichiers `.json`, les `.inc` de script ne sont pas auto-découverts, il faut les lister explicitement
  — première tentative de build a échoué avec des `undefined reference` jusqu'à cet ajout)
- Portes/sorties calées sur les coordonnées exactes déjà utilisées par les mêmes layouts ailleurs dans
  le jeu (ex. `LAYOUT_HOUSE1` → porte à x=4,y=8), vérifiées au préalable en inspectant plusieurs
  cartes existantes qui réutilisent ces layouts (`FallarborTown_CozmosHouse`, `LilycoveCity_House1`,
  etc.) plutôt que devinées

**3. Vérification visuelle (headless, sans Porymap)**
- `tools/qa_harness/qa_runner` utilisé pour naviguer réellement dans le jeu compilé (démarrage via
  `QUICKSTART_AUTO`, puis navigation + menu debug overworld `Debug_EventScript_Script_1/2` — retirés
  avant commit, `debug.inc` revérifié identique à l'état neutre) et capturer des captures d'écran :
  - Nouveau quartier sud : 3 maisons en rangée haute visibles correctement (toits, façades, chemin
    entre elles), 2e rangée avec bordure d'arbustes sud propre, aucune corruption visuelle
  - Entrée dans `LittlerootTown_MmeChenHouse` via la porte correcte : intérieur généré correctement
    (mobilier, PNJ visible)
  - **0 instance de "Bad memory"** sur l'ensemble des tests (contre 720+ lors du bug Professeur Chen
    de la suite 23) — signe que la copie de blocs de tuiles bruts existants, sans réinterprétation,
    est une stratégie nettement plus sûre que la génération de contenu de novo
- Les 4 autres maisons n'ont pas été vérifiées individuellement à l'écran (même mécanisme exact que
  `MmeChenHouse`, layouts génériques vanilla non modifiés) — à surveiller lors du playtest réel

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 25) — Bug critique résolu : le Professeur Chen n'apparaissait pas au labo

Retour de test de Thomas : "Toujours cette petite fille qui bloque le passage" (la Jumelle/Twin
bloque toujours la sortie nord tant qu'on n'a pas de POKéMON). En creusant : "Comment avoir pikachu au
labo ? Je ne l'avais pas prit" — le joueur était bien allé au labo, mais le Professeur Chen n'était
tout simplement jamais apparu pour le lui donner.

**Root cause enfin identifiée** : le résidu "8 instances de Bad memory" documenté en suite 23 comme
"mineur, jamais reproduit comme plantage réel" avait en fait un effet bien réel — juste invisible dans
les tests précédents, qui ne vérifiaient que le nombre de plantages en log, jamais si le Professeur
Chen apparaissait effectivement à l'écran. Un test headless plus poussé cette suite (marche naturelle
jusqu'au labo + captures d'écran, plutôt qu'un simple comptage de plantages) a montré que :
- Avec `addobject` déclenché à l'entrée de la pièce (via `OnTransition` en suite 23, ou même via un
  `coord_event` après le premier pas du joueur, testé cette suite) : Chen n'apparaît jamais à l'écran
- En marchant vers l'endroit où Chen est censé se trouver (même sans jamais l'avoir vu apparaître) :
  **plantage massif de 616 instances "Bad memory"**, signature identique au tout premier bug de la
  suite 23 (720+ instances) — donc le même bug moteur non élucidé, pas un phénomène différent
- Conclusion : l'objet `LOCALID_BIRCHS_LAB_BIRCH` est fondamentalement instable dans ce labo tant que
  le joueur ne l'atteint pas par les chemins vanilla habituels (bien plus tard dans le jeu) — aucune
  variante testée (déclaration statique, `clearflag`, `addobject` à la transition, `addobject` via
  trigger) ne le rend fiable en avance

**Correctif retenu** : abandon complet de la tentative de rendre Chen visible/interactif avant que le
joueur ait son POKéMON. La remise de PIKACHU se déclenche maintenant automatiquement dès que le joueur
fait un pas dans la pièce (nouveaux `coord_events` juste après les deux cases d'entrée, `x=6/7,y=11`),
sans jamais faire apparaître l'objet de Chen pour cette interaction :
- Nouvelle fonction `LittlerootTown_ProfessorBirchsLab_EventScript_TriggerGivePikachu` (`lockall` puis
  `goto GiveDirectPikachu`, sans passer par `addobject`)
- `LittlerootTown_ProfessorBirchsLab_EventScript_ShowBirchBeforeStarter` conservée mais plus jamais
  appelée pour ce chemin — l'ancien mécanisme est documenté en commentaire comme non fiable
- `EventScript_Birch` (script d'interaction directe avec le PNJ) garde son ancienne branche
  `goto_if_unset FLAG_SYS_POKEMON_GET` en code mort défensif — elle ne peut plus jamais se déclencher
  puisque Chen n'est plus jamais spawné avant que le joueur ait déjà son PIKACHU, mais elle ne gêne pas
- Chen ne sera donc plus jamais visible à l'écran pour cette scène précise (juste une voix qui
  s'exprime en entrant), mais le reste de la mise en scène est identique (texte, remise du POKéMON,
  surnom, aller voir le rival)
- **Vérifié en profondeur en headless** : marche naturelle jusqu'au labo (via la porte, pas de
  téléportation par script), déclenchement du dialogue confirmé à l'écran (capture montrant
  "PROFESSEUR C..." puis le texte complet défiler), enchaînement confirmé jusque dans
  `GiveStarterEvent` (texte "En vérité, il ne me reste..." bien affiché, preuve que `givemon`/
  `setflag`/`setvar` se sont exécutés avant), **0 instance de "Bad memory"** sur l'intégralité du test
  (~30 pressions A à travers tout le dialogue, plus la marche jusqu'à l'ancien emplacement de Chen)
- La suite de la conversation (choix du surnom, écran de saisie, "aller voir le rival") n'a pas été
  testée jusqu'au bout caractère par caractère car c'est une mécanique préexistante du jeu de base,
  non modifiée cette session — seul le déclenchement en amont (la partie réellement à risque) a été
  vérifié

**Point important** : cette session confirme une leçon méthodologique — compter les plantages en log
ne suffit pas à valider qu'une fonctionnalité marche réellement ; il faut aussi vérifier visuellement
(captures d'écran) que le contenu attendu apparaît bien à l'écran. Le "résidu mineur" de la suite 23
n'était pas mineur, juste mal caractérisé.

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 26) — Retour de test négatif : le correctif de suite 25 ne marchait toujours pas

Retour de test de Thomas sur la build de suite 25 : "toujours pas de remise de pikachu", "tu n'as
toujours pas supprimer la fillette à la sortie du village", et "la map de bourg palette n'a rien à voir
avec le fichier html que je t'ai fourni".

**1. PIKACHU toujours pas remis : le déclencheur par coord_event n'était pas assez robuste**
- Le correctif de suite 25 (message déclenché par un `coord_event` une case après l'entrée de la
  pièce) fonctionnait en test headless, mais pas de façon fiable en jeu réel. Hypothèse la plus
  probable : le Professeur Chen étant invisible (aucun sprite, cf. suite 25), un joueur qui entre,
  ne voit personne, et ressort directement sans avoir marché précisément sur la case du trigger ne
  déclenche jamais rien
- Tentative intermédiaire (déclenchement direct depuis `OnTransition`, avant même la fin du fondu
  d'entrée) : **provoque un écran figé en test headless** (`lockall`+`msgbox` trop tôt dans la
  transition) — immédiatement abandonnée avant d'être livrée, aurait été un bug bien pire que celui
  qu'elle cherchait à corriger
- **Solution retenue** : réutilisation du mécanisme `MAP_SCRIPT_ON_FRAME_TABLE`/`map_script_2`, déjà
  utilisé de façon fiable par ce même fichier pour `GiveStarterEvent`/`GivePokedexEvent` (contenu
  post-Ligue) — vérifie en continu, à chaque frame où le joueur a le contrôle (donc jamais pendant une
  transition), l'état de `VAR_DEX_UPGRADE_JOHTO_STARTER_STATE` (reste à 0 tout le début du jeu) et
  déclenche la remise du PIKACHU dès que le joueur a le contrôle dans la pièce, sans dépendre d'un
  déplacement précis. Un garde-fou interne (`goto_if_set FLAG_SYS_POKEMON_GET`) empêche toute
  répétition une fois le POKéMON reçu
- Revérifié en headless avec une entrée 100 % naturelle par la porte (aucun raccourci de script) :
  le dialogue s'affiche dès l'entrée dans la pièce sans qu'aucun pas supplémentaire ne soit
  nécessaire, confirmé jusqu'à mi-parcours du texte de remise, **0 instance de "Bad memory"**

**2. La Jumelle bloquait toujours la sortie nord : supprimée cette fois pour de bon**
- Sur nouvelle demande explicite de Thomas, l'ancien verrou (`LittlerootTown_EventScript_
  NeedPokemonTrigger{Left,Right}`, qui faisait accourir la Jumelle pour repousser le joueur tant qu'il
  n'avait pas de POKéMON) est entièrement supprimé : les 4 `coord_events` qui le déclenchaient dans
  `map.json`, les deux scripts, les 6 blocs de mouvement associés, et le texte devenu orphelin
  (`Text_DangerousIfYouDontHavePokemon`). La Jumelle reste présente ailleurs en ville comme PNJ normal
  mais ne bloque plus aucun passage, avec ou sans POKéMON
- Vérifié en headless : un joueur sans aucun POKéMON peut désormais marcher directement de Bourg
  Palette jusqu'à la Route 1 sans aucune interruption

**3. Carte de Bourg Palette : décalage avec le plan HTML — question ouverte, pas encore traitée**
- Thomas signale que le nouveau quartier sud (suite 24) ne correspond pas à son plan détaillé fourni
  en carnet de cartographie HTML (grille 24×18, bâtiments repositionnés). C'est exact et assumé : le
  choix de suite 24 avait été d'étendre la carte existante vers le sud plutôt que de relocaliser tous
  les bâtiments selon le plan, pour ne pas risquer de désaligner la connexion vers la Route 1 (les
  deux cartes faisaient exactement 20×20). Ce choix n'a pas été validé explicitement par Thomas avant
  d'être implémenté. **Non retraité cette suite** — nécessite de clarifier avec Thomas s'il veut une
  refonte plus fidèle au plan original (avec le risque technique expliqué) ou des ajustements ciblés
  sur la disposition actuelle

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 27) — Bourg Palette refaite fidèlement au plan HTML de Thomas

Thomas a redemandé la refonte fidèle ("Réécrit proprement la scène pour déplacer ma maison comme
prévu"), après que la suite 26 ait identifié un risque : la position de la maison du joueur était
codée en dur dans la scène des Chaussures de Course (remise par Maman plus tard dans le jeu). Cette
suite réécrit cette scène pour la rendre indépendante de la position des maisons, ce qui débloque enfin
une refonte complète et fidèle de la carte.

**1. Réécriture de la scène des Chaussures de Course**
- L'ancienne version codait en dur une douzaine de trajets de marche (`applymovement` avec un nombre
  de pas précis par variante, 6 positions de déclenchement × 2 genres) calibrés sur les anciennes
  coordonnées de porte — cassait dès qu'une maison bougeait
- Remplacée par une version simple et indépendante de la position : Maman apparaît directement
  (`setobjectxy`) à un point fixe du chemin d'entrée (indépendant de sa propre maison), donne les
  Chaussures, disparaît. Les deux seules valeurs encore liées à la position des maisons
  (`SetHomeDoorCoordsMale/Female`, utilisées pour l'animation d'ouverture de porte quand on parle
  directement à Maman) sont maintenant de simples constantes faciles à mettre à jour
- Vérifié en headless (déclenchement réel du trigger, pas un raccourci scripté) : dialogue complet
  affiché correctement ("Maman ! Tu as pensé à te présenter au Pr. Chen ?" → "Tiens, chérie ! Pour
  ton aventure, mets ces..."), **0 plantage**

**2. Carte de Bourg Palette entièrement refaite selon le plan HTML**
- Nouvelle carte 24×19 recalculée directement à partir du code JavaScript du plan fourni par Thomas
  (mêmes fonctions `building()`/`makeGrid` réimplémentées en Python pour extraire les coordonnées
  exactes), avec les 8 bâtiments réorganisés en 3 rangées fidèles à l'agencement du plan :
  - Rangée 1 : Maison du joueur, Laboratoire du Pr. Chen
  - Rangée 2 : Mme Chen, maison aux volets fermés, Régis
  - Rangée 3 : Gardien de Route, Vieux Dresseur, Dame aux Baies
  - Coordonnées de porte très proches du plan original (écart d'au plus 1 case sur la plupart des
    bâtiments, plusieurs correspondances exactes) — légèrement adaptées pour utiliser un bloc de
    maison unique de taille fixe (5×5, 7×5 pour le labo), déjà vérifié fiable en suite 24, plutôt que
    de générer des tailles de bâtiment variables sans retour visuel
  - **La connexion vers la Route 1 n'a pas bougé** (sortie toujours aux mêmes colonnes qu'avant,
    `offset` de connexion inchangé) — tous les bâtiments ont été agencés autour de ce couloir d'entrée
    fixe pour ne courir aucun risque de désalignement avec la Route 1
  - Mare et herbes hautes décoratives du plan non reproduites (auraient nécessité de nouvelles tuiles
    dont l'identifiant n'est pas garanti sûr sans retour visuel direct ; les herbes hautes créeraient
    en plus des rencontres sauvages en pleine ville) — simplification assumée
- Tous les warps, PNJ et panneaux redéplacés vers les nouvelles positions ; les 5 maisons de la
  suite 24 (intérieurs déjà créés) et les warps `MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F`/
  `..._BRENDANS_HOUSE_1F`/`..._PROFESSOR_BIRCHS_LAB` (déjà existants) réutilisés tels quels — seules
  leurs coordonnées côté Bourg Palette changent, aucun fichier de carte intérieure modifié
- **Vérifié en profondeur en headless** : rendu visuel des 3 rangées de bâtiments correct, **raccord
  avec la Route 1 sans aucun décalage visible** (bordure d'arbres parfaitement alignée), entrée dans le
  Labo à sa nouvelle position (déclencheur PIKACHU toujours fonctionnel), entrée dans la maison du
  joueur à sa nouvelle position (Maman bien présente à l'intérieur), scène des Chaussures de Course
  fonctionnelle à la nouvelle position — **0 instance de "Bad memory"** sur l'ensemble des tests

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 28) — Correction Jessie/James (Team Rocket) sur la Route 1

Thomas a signalé qu'à la sortie de Bourg Palette, sur la Route 1, seul Miaouss était visible : Jessie
était invisible et James inaccessible pour lancer le combat.

**1. Diagnostic**
- Reproduit en isolation en headless : les graphismes d'origine `OBJ_EVENT_GFX_ROCKET_F`/
  `OBJ_EVENT_GFX_ROCKET_M` provoquent une corruption mémoire importante (des centaines d'instances de
  "Bad memory"/"Bad BIOS Load" dans les logs) dès qu'ils sont affichés sur cette carte précise, que ce
  soit par déclaration statique dans `map.json` ou via `addobject` — contrairement au cas du
  Professeur Chen (suite 23/25) où `addobject` avait réglé un problème similaire, aucun des deux
  mécanismes n'est fiable ici. Miaouss (graphisme d'espèce, mécanisme différent) n'a jamais posé de
  problème
- Cause profonde non identifiée avec certitude malgré une investigation poussée (ces mêmes graphismes
  fonctionnent sans souci ailleurs dans ce hack, par exemple en forêt de Jade pour la scène Team
  Rocket/Devon) — corrigé par un contournement pragmatique plutôt que de risquer une régression sur
  un mécanisme mal compris
- James se trouvait en plus sur une case décorative de buisson (différente de celle de Jessie et de
  Miaouss), ce qui expliquait probablement en partie la difficulté à l'atteindre pour engager le
  combat

**2. Correctif appliqué (`data/maps/Route101/map.json` et `scripts.inc`)**
- Jessie et James utilisent maintenant des graphismes PNJ génériques déjà utilisés sans problème
  ailleurs dans le jeu (`OBJ_EVENT_GFX_WOMAN_2` et `OBJ_EVENT_GFX_MAN_3`), avec des emplacements de
  palette différents de celui utilisé par le Pr. Chen sur cette même carte
- James repositionné de (9,17) à (10,17), sur la même case dégagée (herbe, sans décor) que Jessie et
  directement au-dessus de Miaouss (10,18), au lieu de la case de buisson décorative d'origine
- Mécanisme de déclenchement (apparition conditionnée à `VAR_BIRCH_LAB_STATE` et à la défaite de
  l'équipe) inchangé, revenu à sa version d'origine après l'abandon de la piste `addobject`

**3. Vérification headless**
- Testé en isolation (les trois PNJ toujours visibles, indépendamment de la progression) avec les
  nouveaux graphismes et positions : **0 instance de "Bad memory"**, Jessie et James bien visibles et
  distincts à l'écran
- Combat de James déclenché avec succès par une interaction directe (case juste au-dessus, touche A) :
  le texte de la devise de la Team Rocket s'affiche correctement ("Prépare-toi aux ennuis, et fais que
  ce soit double !"), confirmant que James est bien accessible pour lancer le combat
- Note méthodologique : un warp déclenché depuis le menu de débogage "Scripts" laisse le personnage
  bloqué après le chargement de la carte (bug de l'outil de test, pas du jeu) — contourné en utilisant
  l'utilitaire dédié "Warp to map warp…" du menu de débogage, qui restaure correctement le contrôle du
  joueur

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,01 %)

## Session 4 (suite 29) — Nouvelle zone : le Mont Sélénite

Thomas a fourni un carnet de cartographie complet (plan HTML) pour une nouvelle zone montagneuse à
système souterrain sur 3 niveaux, à construire entre Argenta et Azuria. Demande explicite :
implémenter la carte complète depuis le plan, avec une sortie nord d'Argenta, en construisant les 4
cartes de zéro (plutôt que de réutiliser des cartes existantes) pour rester fidèle au plan fourni.

**1. Clarification géographique préalable**
- Vérifié que dans ce hack, `PetalburgCity` = Argenta (1ère arène) et `RustboroCity` = Azuria (2e
  arène, Ondine) — l'inverse de ce qu'on pourrait supposer par analogie avec les noms Hoenn/Kanto
- `Route104` relie déjà Argenta et Azuria directement et s'appelle "ROUTE 4" en jeu — cohérent avec le
  Kanto canon (Argenta → Mont Sélénite → Route 4 → Azuria)
- Argenta n'avait alors aucune sortie nord : une nouvelle connexion `up` a été ajoutée sur
  `PetalburgCity/map.json`, avec une brèche de 3 cases percée dans la bordure nord de la carte (zone
  déjà dégagée, place déjà ouverte juste en dessous, aucun bâtiment perturbé)

**2. Quatre nouvelles cartes construites depuis le plan HTML**
- `MtSelenite` (extérieur, 26×20) : re-génération en Python du même algorithme de tracé
  (`carve`/`blob`) que le JavaScript du plan fourni, avec une palette de tuiles sûres extraite de
  `Route115` (route montagneuse déjà existante au nord d'Azuria) : herbe, sentier, blocs rocheux,
  entrées de grotte — toutes des tuiles déjà utilisées ailleurs dans ce hack ou dans le jeu de base,
  aucune tuile inédite
- `MtSelenite_1F`/`_B1F`/`_B2F` (tunnels, salles, cavernes, 26×22/28×22/28×24) : même principe de
  génération, palette extraite de `GraniteCave` (sol, parois, variantes décoratives)
- Tentative initiale de réutiliser le tileset `cave_frlg` (rendu Kanto authentique, choix initial de
  Thomas) — **abandonnée** : ce tileset et les layouts au format `frlg` ne sont compilés que pour un
  build FireRed/LeafGreen (`MAP_VERSION=firered`), pas pour ce hack qui reste un build Emerald sous le
  capot malgré son habillage Kanto. Remplacé par le tileset `cave` standard (Granite Cave/Victory
  Road), seul choix réellement disponible dans ce build
- Rivière souterraine et cristaux du plan simplifiés en variantes de sol décoratives praticables
  (aucune tuile "eau" ou "cristal" fiable disponible dans ce tileset) — simplification assumée, comme
  pour la mare de Bourg Palette en suite 27

**3. Contenu**
- 2 fossiles au choix exclusif (FOSSILE DÔME / FOSSILE SPIRALE, un seul flag partagé) dans les
  cavernes profondes, sur le modèle du script `MtMoon_B2F_Frlg` déjà présent dans le jeu de base
- Une zone secrète accessible par un chemin détourné, avec un combat sauvage statique unique
  (Mélofée) sur le modèle des combats statiques déjà présents (`AquaHideout_B1F`)
- Tables de rencontres sauvages ajoutées pour les 4 cartes (Nosferapti/Racaillou/Coco en surface et
  dans les niveaux supérieurs, Ténéfix/Grolem en profondeur)
- Panneau d'entrée ("MONT SÉLÉNITE ↓ ARGENTA")

**4. Câblage complet**
- Argenta → Mont Sélénite (nouvelle connexion `up`) → 2 entrées de grotte menant au niveau 1 → 2
  échelles vers le niveau 2 → 2 échelles vers le niveau 3 → sortie nord vers un nouveau point d'entrée
  sur `Route104` (déjà reliée à Azuria) — toutes les références croisées entre cartes
  (`dest_map`/`dest_warp_id`) vérifiées programmatiquement avant compilation
- Nouvelle section `MAPSEC_MT_SELENITE` ajoutée à la mini-carte (région Hoenn active, pas la carte
  Kanto FRLG dormante)

**5. Bug trouvé et corrigé pendant les tests**
- Les deux entrées de grotte de la carte extérieure étaient placées puis immédiatement écrasées par
  les blobs décoratifs de falaise générés après coup (même défaut d'ordre que dans l'algorithme
  JavaScript d'origine) — corrigé en générant les blobs décoratifs avant les marqueurs d'entrée

**6. Vérification headless**
- **0 instance de "Bad memory"** sur l'ensemble des tests (extérieur, entrée en grotte, niveau
  profond, fossiles visibles et distincts)
- Carte extérieure et niveau profond accessibles et rendus correctement en jeu, connexion Argenta ⇄
  Mont Sélénite traversée dans les deux sens sans plantage ni décalage visuel
- Note méthodologique supplémentaire : la navigation dans le sous-menu "Group" de l'utilitaire de
  warp de débogage utilise les touches gauche/droite pour changer le pas d'incrémentation (1/10/100)
  et haut/bas pour appliquer ce pas — piège à éviter pour les futurs tests headless de cartes situées
  dans des groupes autres que le groupe 0

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,03 %)

## Session 4 (suite 30) — Bourg Palette refaite (village côtier), cadeaux de Maman, Multi-Exp, soin instantané

Thomas a fourni un nouveau plan HTML plus abouti pour Bourg Palette (collines, ruisseau, côte rocheuse,
plage, bâtiments mieux espacés) et demandé plusieurs changements de confort de jeu. Les 4 autres plans
HTML fournis dans le même message (Route 1, Jadielle, Forêt de Jade, Argenta) sont volontairement
laissés de côté pour des sessions dédiées — Thomas a confirmé vouloir se concentrer sur Bourg Palette
cette fois-ci, chacune de ces cartes représentant un chantier de l'ampleur du Mont Sélénite à elle seule.

**1. Bourg Palette entièrement refaite en 34×26 (village côtier)**
- Régénérée en Python depuis l'algorithme du plan HTML (`carve`/`blob`/tracé de rivière réimplémentés),
  avec une palette de tuiles vérifiées directement dans le jeu de base : herbe/chemin/collines/panneau
  du tileset déjà utilisé par Bourg Palette (`General`+`Petalburg`), complétées par de vraies tuiles de
  mer/plage puisées dans le tileset `General` primaire (partagé par toutes les cartes, y compris celles
  qui n'utilisent pas de tileset secondaire côtier) — pas de nouveau tileset nécessaire
- Collines à l'ouest, ruisseau du nord au sud, côte rocheuse et plage complète au sud (contrairement à
  la mare simplifiée de suite 27, l'eau ici est une vraie mer avec son comportement de jeu standard :
  infranchissable sans Surf, pas de mur artificiel nécessaire)
- Les 8 bâtiments (Labo + 7 maisons, tous déjà existants depuis les suites 24/27) repositionnés selon
  le nouveau plan, en réutilisant tels quels les blocs de tuiles bruts déjà éprouvés (aucune nouvelle
  carte d'intérieur nécessaire — seules les coordonnées de porte côté Bourg Palette changent)
- Connexion vers la Route 1 recalculée (nouvel `offset` sur les deux cartes) pour que le nouveau couloir
  d'entrée (recentré) s'aligne avec le couloir existant sur la Route 1, inchangé
- PNJ, panneaux et le point de garde de la Jumelle repositionnés en conséquence

**2. Nouvelle scène : Maman remet Chaussures de Course + Vélo + Canne à Pêche**
- Se déclenche désormais juste après la visite complète au Labo (`VAR_LITTLEROOT_TOWN_STATE == 3`,
  déjà utilisé par le jeu de base à ce moment précis) plutôt qu'en sortant du village — Maman apparaît
  directement à droite de la porte du Labo et remet les trois objets en une seule conversation
  (`ITEM_MACH_BIKE`, `ITEM_OLD_ROD`, en plus des Chaussures de Course qui restent un déblocage de
  capacité via `FLAG_SYS_B_DASH`, pas un objet d'inventaire)
- Ancien mécanisme (déclenchement automatique en marchant vers la sortie nord, calibré sur des
  positions de porte désormais obsolètes) entièrement retiré ; les textes de cette scène, restés en
  anglais depuis l'origine du projet, traduits en français au passage

**3. Autres corrections de confort**
- Animation de soin au Centre Pokémon (Poké Balls clignotantes sur la machine) supprimée — le soin
  reste complet et instantané, juste sans l'attente visuelle, dans `data/scripts/pkmn_center_nurse.inc`
  (fichier partagé par tous les Centres Pokémon actifs du hack)
- Multi-Exp activé en permanence dès une nouvelle partie (toute l'équipe gagne de l'XP même sans avoir
  combattu, sans avoir besoin de l'objet) via `I_EXP_SHARE_FLAG` (mécanisme déjà prévu par
  pokeemerald-expansion) pointé vers un nouveau flag toujours actif dès `new_game.inc`
- Audit de traduction : le texte de revanche d'Ondine dans `RustboroCity_Gym/scripts.inc` (resté au nom
  et aux répliques anglaises d'origine malgré la conversion Roxanne → Ondine de suite 32) corrigé.
  **Audit non exhaustif** : une recherche large a fait remonter un volume important de dialogues encore
  en anglais dans des PNJ secondaires à travers tout le jeu (Rustboro/Azuria, Petalburg/Argenta,
  Route104, forêt de Jade, etc.) — un passage de traduction complet du jeu est un chantier à part
  entière, largement hors de portée de cette seule suite, et non traité ici au-delà du cas Ondine

**4. Vérification headless**
- **0 instance de "Bad memory"** sur l'ensemble des tests (extérieur, côte/mer, entrée au labo,
  scène complète des cadeaux de Maman avec dialogue en français confirmé à l'écran)
- Compilation propre avec les 4 changements de confort (Multi-Exp, soin instantané, textes Ondine)

- Build release validée (`make MODERN=1`, exit code 0, ROM 79,03 %)

## Session 5 (suite 31) — Correctif retour de test : plantage à la 2e rencontre Team Rocket (forêt de Jade)

**1. Diagnostic**
- Retour de test : message d'erreur pendant/après le combat de la 2e rencontre Team Rocket (forêt de
  Jade, `PetalburgWoods`), puis retour à l'écran-titre une fois le combat gagné, avec des graphismes
  "Team Rocket" corrompus sur Jessie et James
- Cause identifiée : `PetalburgWoods/map.json` utilisait encore `OBJ_EVENT_GFX_ROCKET_F`/
  `OBJ_EVENT_GFX_ROCKET_M` pour Jessie/James — exactement les deux graphismes déjà identifiés en
  suite 28 comme provoquant une corruption mémoire sur les cartes au format Emerald de ce hack. Le
  correctif de suite 28 n'avait été appliqué qu'à Route 1 ; cette deuxième rencontre (ajoutée en
  Session 3, avant la découverte du bug) n'avait jamais été mise à jour
- Confirmé par relecture de code (assertion secondaire indépendante trouvée au passage dans
  `SetTrainerFacingDirection`, bénigne/résumable en debug, silencieuse en release — non liée à la
  cause principale) et par test headless : combat rejoué de bout en bout via le raccourci "Instant
  Win" du menu de debug batailles (`SELECT` en combat), 0 instance de "Bad memory" sur l'ensemble de
  la séquence

**2. Correctif**
- `PetalburgWoods/map.json` : `OBJ_EVENT_GFX_ROCKET_F` → `OBJ_EVENT_GFX_WOMAN_2` (Jessie),
  `OBJ_EVENT_GFX_ROCKET_M` → `OBJ_EVENT_GFX_MAN_3` (James) — mêmes graphismes de remplacement que sur
  Route 1, pour une apparence cohérente entre les deux rencontres. Aucun conflit de palette sur cette
  carte (ces graphismes n'y étaient pas déjà utilisés ailleurs)

**3. Vérification headless**
- Warp direct vers `PetalburgWoods` (groupe 24, index 11) puis traversée complète de la carte à pied :
  **0 instance de "Bad memory"**
- Build release validée (`make MODERN=1`, exit code 0)

**4. Investigation Porymap + script de preview PNG des maps**
- Vérifié : Porymap (github.com/huderlem/porymap) est installable dans cet environnement
  (Qt6 disponible via apt, Xvfb disponible pour un serveur X headless) mais **inutilisable en
  pratique** — c'est un éditeur purement interactif, `main.cpp` ne fait que transmettre `argv` à
  `QApplication` sans aucun traitement d'arguments (pas de mode CLI, pas de mode batch, pas
  d'export scriptable). Le piloter à l'aveugle par captures d'écran/clics simulés serait
  impraticable pour un vrai travail de cartographie sans un humain devant l'écran
- Créé `tools/map_preview/map_preview.py` : lit directement `tiles.png` + `metatiles.bin` +
  `palettes/*.pal` + `layouts.json` + `map.json` (les mêmes fichiers source que compile le
  moteur GBA) et reproduit l'algorithme de composition des metatiles (`engine/src/fieldmap.c`,
  `engine/include/global.fieldmap.h`) pour générer un PNG fidèle de n'importe quelle carte
- Piège détecté et corrigé en cours de route : les layouts `"layout_version": "frlg"` utilisent
  des constantes de répartition primaire/secondaire différentes des layouts `"emerald"` (640
  tuiles/metatiles et 7 palettes en primaire contre 512/512/6) — `NUM_*_FRLG` dans
  `engine/include/fieldmap.h`. Sans ça, les cartes FRLG (dont les futures cartes Kanto visées par
  la directive de Thomas) s'affichaient avec des metatiles corrompus (damier magenta)
- Mode supplémentaire `--tileset <primaire> <secondaire>` : affiche en planche toutes les
  metatiles d'une paire de tilesets, y compris un tileset FRLG dormant qu'aucune carte n'utilise
  encore (ex: `pallet_town_frlg`) — utile pour prévisualiser un tileset avant de bâtir une carte
- Testé avec succès sur Bourg Palette (tileset actuel), Pewter City FRLG (validation du fix de
  constantes sur un layout `frlg`), Mont Sélénite, et une planche du tileset `pallet_town_frlg`
  (Poké Mart, Centre Pokémon, plage, jetée, eau — rendu fidèle)
- **Point de vigilance confirmé** (celui qu'on soupçonnait déjà en suite 30) : `PewterCity_Frlg`,
  malgré son dossier dans `data/maps/`, n'est PAS une carte active du hack — `IS_FRLG` vaut `0`
  pour ce build Emerald (`include/constants/global.h`), et tous les tilesets `*_frlg` de villes
  (dont `gTileset_PalletTown`) sont dans la branche `#else` (IS_FRLG uniquement) du même garde
  `#if !IS_FRLG` de `headers.h` qui avait bloqué `Cave_Frlg`/`General_Frlg` en suite 29 ; leurs
  layouts `"layout_version": "frlg"` sont en plus filtrés hors compilation par `MAP_VERSION`
  (`map_data_rules.mk`). Le script de preview a pu quand même les lire et les rendre correctement
  car il lit les fichiers source directement, sans passer par la chaîne de build
- **Rappel de portée** : ce script ne fait que la preview (lecture seule). Reconstruire
  effectivement une carte active (Bourg Palette en premier) avec un vrai tileset Kanto FRLG
  nécessitera d'abord un contournement du garde `#if !IS_FRLG` (dupliquer le struct de tileset
  concerné sous un nouveau nom compilé pour Emerald, même fichiers source) — chantier séparé,
  non commencé, à trancher avec Thomas avant de s'y lancer
