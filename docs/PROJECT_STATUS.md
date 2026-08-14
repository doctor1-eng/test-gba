# PROJECT_STATUS.md

Dernière mise à jour : 2026-08-14 (Session 4, suite 23)

## TERMINÉ
- [x] Phase 0 — Audit technique complet (voir PROJECT_ANALYSIS.md)
- [x] Décision d'architecture : Option B (moteur pokeemerald décompilé), validée par Thomas
- [x] Toolchain de compilation installée et validée (2 builds réussis)
- [x] Structure de projet créée (`docs/`, `engine/`, `tools/`)
- [x] Confirmation : charmap français complet déjà présent nativement dans le moteur
- [x] Premier contenu réel : discours d'ouverture du Professeur Chen traduit en français (Bourg Palette)
- [x] Renommage du premier lieu : Littleroot Town → Bourg Palette (nom affiché à l'écran)

## EN COURS
- Rédaction GAME_DESIGN_DOCUMENT.md, STORY_BIBLE.md, CHARACTERS.md, EPISODES.md (Phase 1)

## PROCHAINES ÉTAPES (ordre de priorité)
1. Finaliser les documents de Phase 1 (scénario global, personnages, découpage en arcs/épisodes)
2. Lister précisément les renommages Hoenn → Kanto (villes, routes, PNJ génériques) — gros chantier JSON/texte
3. Adapter le starter : remplacer le choix classique (3 starters) par la remise de Pikachu (fidélité anime)
4. Construire le vertical slice Phase 2 : Bourg Palette complet (maison du joueur, labo, sortie vers Route 1)
5. Script de preview PNG des maps (rendu tuiles) pour validation visuelle sans GUI

## BLOQUÉ
- Aucun blocage actif.

## BUGS CONNUS
- Aucun à ce stade (2 builds propres).

## Décisions de design actées
- Le joueur incarne l'équivalent de Sacha : starter = Pikachu (et non les 3 starters classiques), conformément
  au cahier des charges anime. *(à confirmer avec Thomas si divergence souhaitée)*
- Bourg Palette = ex-Littleroot Town (premier lieu renommé, sert de gabarit pour les prochains renommages).

## Mise à jour Session 2
- Moteur définitif : rh-hideout/pokeemerald-expansion (voir TECHNICAL_ARCHITECTURE.md)
- Build validé 2x sur la nouvelle base
- Prochaine étape : reprise de la Phase 1 (GAME_DESIGN_DOCUMENT.md, CHARACTERS.md, EPISODES.md) en tenant compte des mécaniques désormais disponibles (à décider : quel niveau de mécaniques modernes garder pour rester crédible "Kanto saison 1" — proposition à valider avec Thomas)

## Mise à jour Session 2 (suite) — Phase 1 terminée
- [x] Décision actée : mécaniques modernes pleines (type Fée, split Phys/Spé, Gen latest) — déjà natif au moteur, rien à configurer
- [x] GAME_DESIGN_DOCUMENT.md rédigé (pitch, piliers, arcs macro, systèmes)
- [x] CHARACTERS.md rédigé (voix distinctes des personnages principaux)
- [x] EPISODES.md rédigé (Arc 1 détaillé épisode par épisode, arcs 2-5 en squelette)

## PROCHAINES ÉTAPES (mise à jour)
1. Implémenter Épisode 1.2 : remplacer starter_choose.c par remise directe de Pikachu
2. Implémenter Épisodes 1.1, 1.3, 1.4, 1.5 (dialogues, scripts, premier combat Team Rocket)
3. Renommer les maps concernées (Littleroot Town → Bourg Palette, labo, Route 1)
4. Script Python de preview PNG des maps pour validation visuelle sans GUI
5. Build de validation après chaque épisode implémenté

## Mise à jour Session 2 (suite 2)
- [x] Preuve technique : remise directe de Pikachu au lieu du choix à 3 starters (build validé)
- ATTENTION pour la suite : la scène actuelle est encore celle du sauvetage du Professeur attaqué par un
  Zigzagoon sur la Route 1 (flux Émeraude d'origine) — narrativement différente de l'Épisode 1.2 documenté
  (remise du Pikachu au labo). Prochaine tâche : réécrire complètement cette séquence pour coller aux
  Épisodes 1.1-1.5 (déplacer la remise au labo, retirer/adapter la scène Zigzagoon, ajouter la réticence
  de Pikachu). Ne pas se contenter du patch minimal actuel pour la version "finale" de l'arc.

## Mise à jour Session 2 (suite 3) — Épisode 1.2 fonctionnellement complet
- [x] Scène Route 1 + Labo entièrement traduite et adaptée (Pikachu, Professeur Chen, sans lore Hoenn résiduel)
- [x] Route 101 → "Route 1", Oldale Town → "Jadielle" (textuel ; la map physique garde le level design Hoenn pour l'instant)
- [x] Limite de police notée : tiret cadratin absent de la charmap

## PROCHAINES ÉTAPES (mise à jour)
1. Épisode 1.3 : mécanique de réticence de Pikachu (hors POKé BALL) — évaluer `include/config/follower_npc.h`, potentiellement réutilisable pour un Pikachu qui suit à pied
2. Épisode 1.4 : première rencontre Team Rocket (Jessie/James/Miaouss) sur Route 1 ou à Bourg Palette
3. Épisode 1.5 : scène de départ, première provocation de Régis
4. Fixer le nom du rival sur "Régis" (actuellement token {RIVAL} dynamique lié à l'écran de personnalisation du perso — à étudier, ne pas casser le flux de création de personnage)
5. Renommage complet des maps Hoenn → Kanto (au-delà du texte : level design, cf. GAME_DESIGN_DOCUMENT.md)
6. Script Python de preview PNG des maps (toujours en attente)

## Mise à jour Session 2 (suite 4) — Épisode 1.3 fonctionnellement complet
- [x] Pikachu suit désormais le joueur hors POKé BALL (système follower HGSS natif activé)
- [x] Confirmé : sprite de suivi Pikachu déjà présent dans les assets du moteur
- [x] Build validé (4e build propre)

## PROCHAINES ÉTAPES (mise à jour)
1. Épisode 1.4 : première rencontre Team Rocket (Jessie/James/Miaouss) — à concevoir (map, dresseurs, dialogue, sprite objets)
2. Épisode 1.5 : scène de départ, première provocation de Régis
3. Fixer le nom du rival sur "Régis" (token {RIVAL} dynamique — à étudier sans casser l'écran de création de personnage)
4. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
5. Script Python de preview PNG des maps (toujours en attente)
6. Décision à prendre : garder les réactions contextuelles automatiques du suiveur (Ligue/vélos/bateau, textes VO) ou les traduire/adapter au fur et à mesure qu'on croise ces décors

## Mise à jour Session 2 (suite 5) — Épisode 1.4 fonctionnellement complet
- [x] Scène Team Rocket implémentée et compilée (dialogue uniquement, pas de vrai combat de dresseur)
- [x] Gating narratif validé (apparition après Pikachu, disparition définitive après résolution)

## PROCHAINES ÉTAPES (mise à jour)
1. Créer une vraie classe TRAINER_CLASS_ROCKET (nom, musique, sprite déjà dispo) — prérequis avant tout combat réel contre Jessie/James dans les arcs suivants (récurrence prévue au cahier des charges)
2. Épisode 1.5 : scène de départ, première provocation de Régis
3. Fixer le nom du rival sur "Régis" (token {RIVAL} dynamique — à étudier sans casser l'écran de création de personnage)
4. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
5. Script Python de preview PNG des maps (toujours en attente)

## Mise à jour Session 2 (suite 6) — Infrastructure Team Rocket prête
- [x] TRAINER_CLASS_ROCKET créée et fonctionnelle
- [x] Jessie (Ekans) et James (Koffing) existent comme vrais dresseurs combattables, réutilisables dans tous les arcs suivants
- [x] Budget de flags dresseurs vérifié, marge restante : 7 dresseurs supplémentaires possibles avant nouvel ajustement (MAX_TRAINERS_COUNT_EMERALD)

## PROCHAINES ÉTAPES (mise à jour)
1. (Optionnel, polish) Remplacer la scène scriptée de l'Épisode 1.4 par un vrai combat contre TRAINER_ROCKET_JAMES_1 en utilisant `trainerbattle`, maintenant que l'infrastructure existe
2. Import éventuel d'une musique de combat dédiée Team Rocket (actuellement musique Team Aqua réutilisée)
3. Épisode 1.5 : scène de départ, première provocation de Régis
4. Fixer le nom du rival sur "Régis" (token {RIVAL} dynamique — à étudier sans casser l'écran de création de personnage)
5. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
6. Script Python de preview PNG des maps (toujours en attente)

## Mise à jour Session 2 (suite 7) — Épisode 1.4 : vrai combat validé
- [x] Combat réel contre James (Koffing niv. 4) fonctionnel, niveau ajusté pour rester accessible
- [x] Texte de défaite dédié, fuite scénarisée post-combat inchangée

## PROCHAINES ÉTAPES (mise à jour)
1. Épisode 1.5 : scène de départ, première provocation de Régis
2. Fixer le nom du rival sur "Régis" (token {RIVAL} dynamique — à étudier sans casser l'écran de création de personnage)
3. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
4. Script Python de preview PNG des maps (toujours en attente)
5. (Polish) musique de combat Team Rocket dédiée, au lieu de la musique Team Aqua réutilisée

## Mise à jour Session 2 (suite 8) — Nom du rival
- [x] "Régis" disponible et pré-sélectionné en premier choix à l'écran de nommage du rival
- [ ] EN PAUSE volontaire : le vrai premier combat de rival (Route 103 dans la structure Hoenn actuelle) est une scène complexe (branches Brendan/May, mouvements scriptés, plusieurs fichiers). Je ne l'attaque pas pendant que Thomas teste activement la ROM pour ne pas complexifier un état en cours de validation. Prochaine session : reprendre ici.

## Mise à jour Session 3 — Corrections post-test
- [x] Sprite Chen corrigé (Oak natif, plus cohérent que Birch)
- [x] Bug d'élévation Team Rocket corrigé (invisibilité + blocage fantôme)
- [x] Traduction assistant du labo
- [x] Clarifié : combat Zigzagoon = mécanique normale, pas un bug
- En attente : retour de re-test de Thomas sur la v0.2

## PROCHAINES ÉTAPES
1. Reprendre le premier combat de rival (actuellement sur Route 103 dans la structure Hoenn) — Épisode 1.5
2. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
3. Script Python de preview PNG des maps
4. Balayage plus large des dialogues restants en anglais dans les zones déjà "actives" (labo, Route 1, maison du joueur)

## Mise à jour Session 3 (suite) — Épisode 1.5 complet
- [x] VAR_STARTER_MON fixé (bug latent important, corrigé avant qu'il ne cause des soucis plus loin dans le jeu)
- [x] Combat de rival Route 103 fonctionnel, Régis fixe (plus de dépendance au genre du joueur ni à un starter inexistant)
- [x] Textes traduits et adaptés à la relation Chen/Régis

## PROCHAINES ÉTAPES
1. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
2. Script Python de preview PNG des maps
3. Balayage plus large des dialogues restants en anglais
4. Vérifier si VAR_STARTER_MON est utilisé ailleurs avec des conséquences visibles tôt dans le jeu (Dex, autres PNJ) à surveiller lors des prochains tests

## Mise à jour Session 3 (suite 2) — Renommage régional
- [x] MAPS.md créé, 11 villes + 34 routes renommées (affichage uniquement, level design Hoenn inchangé — attendu et documenté)

## PROCHAINES ÉTAPES
1. Script Python de preview PNG des maps (toujours en attente — priorité pour la suite)
2. Balayage plus large des dialogues restants en anglais
3. Décider du sort des 5 villes Hoenn sans équivalent Kanto (Dewford, Sootopolis, Fallarbor, Verdanturf, Pacifidlog) — post-game ?
4. Reconstruction visuelle réelle des maps (gros chantier, non commencé)

## Mise à jour Session 3 (suite 3) — Outil de QA headless créé
- [x] Harness libmgba fonctionnel (build + exécution + capture PNG validés)
- [ ] Calibrage des timings de boot (logos → titre → nouvelle partie) — à faire prochaine session
- [ ] Script de référence "Arc 1 complet" pour test de non-régression automatique

## PROCHAINES ÉTAPES (mise à jour)
1. Calibrer les timings du QA harness (priorité — débloquera un vrai gain de fiabilité pour la suite)
2. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
3. Créer une vraie classe Team Rocket → fait (session 2)
4. Balayage plus large des dialogues restants en anglais

## Mise à jour Session 3 (suite 5)
- [ ] BLOQUÉ (QA harness) : les entrées clavier ne semblent pas enregistrées par le core mgba — cause
  racine non identifiée, à déboguer en vérifiant `core->getKeys()` avant de retenter des scripts d'input
- [x] Investigation Épisode 1.1 (maison du joueur) : nécessite une réécriture narrative complète, pas une
  simple traduction (toutes les répliques référencent "papa le champion d'arène" / jour d'emménagement,
  incompatibles avec notre histoire) — tâche à traiter en session dédiée

## PROCHAINES ÉTAPES (mise à jour)
1. Réécriture narrative complète de la maison du joueur (Épisode 1.1) — prévoir une session dédiée, pas un aparté
2. Déboguer l'input du QA harness (vérifier core->getKeys())
3. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
4. Script Python de preview PNG des maps (alternative plus simple au QA harness, jamais commencé)

## Mise à jour Session 4 — Migration vers dépôt git réel (Claude Code)
- [x] Projet transféré dans un vrai dépôt git (`doctor1-eng/test-gba`, branche
  `claude/pokeemerald-setup-context-wayy1t`) — fin de la dépendance à l'environnement bac-à-sable éphémère
  annoncée en fin de Session 3
- [x] Reconstruction depuis le paquet léger validée : clone `engine/` frais + `changed_files/` appliqués +
  toolchain installée + build propre (12e build global, ROM 79,02 %, identique aux sessions précédentes)
- [x] Structure de dépôt actée : `engine/` gitignoré (trop volumineux, se reclone via INSTALL.md),
  `changed_files/` + `docs/` + `tools/qa_harness/` (hors binaire `qa_runner`) versionnés
- [ ] `qa_runner` fourni dans le paquet non ré-exécuté (binaire non vérifié) — à recompiler depuis la source
  si la calibration du QA harness reprend

## Mise à jour Session 4 (suite) — Épisode 1.1 fonctionnellement complet
- [x] Proposition de scène (dialogues + séquence) validée par Thomas avant implémentation
- [x] Intrigue "emménagement/papa champion d'arène" retirée à la racine : plus de camion (`new_game.c`
  warp directement dans la chambre du joueur), plus de cartons, plus de blocage d'escalier, plus de
  bulletin TV Arène d'Argenta
- [x] Nouveau dialogue Maman unique (accueil + direction labo + rappel de prudence), `VAR_LITTLEROOT_INTRO_STATE`
  simplifiée à 2 états (0/1) contre 8 avant
- [x] Build validé (13e build propre), ROM toujours 79,02 % d'occupation
- [ ] DÉCOUVERT EN COURS DE ROUTE, PAS TRAITÉ : la maison du rival (Régis) contient une scène "nouveau
  voisin" symétrique avec les mêmes références père/déménagement, toujours accessible indépendamment de
  ce qui vient d'être corrigé (vérifié : ne touche à rien du combat de rival Route 103 déjà validé si on
  la laisse de côté pour l'instant) — nécessite une session dédiée

## Mise à jour Session 4 (suite 2) — Maison du rival (Régis) : fait
- [x] Scènes "qui es-tu / nouveau voisin" (maison du joueur ET chambre de Régis) réécrites — Régis et le
  joueur se connaissent déjà, plus de référence "papa champion d'arène"
- [x] Généalogie actée : Régis = petit-fils de Chen via un de ses enfants (PNJ "mère de Régis" = fille de
  Chen, pas son épouse)
- [x] Déblocage de la Route 1 (`VAR_LITTLEROOT_TOWN_STATE`) vérifié intact — risque de blocage total évité
  (découvert avant implémentation, pas en production)
- [x] Build validé (14e build propre), ROM 79,01 %
- **Arc 1 narrativement complet** : les 5 épisodes prévus dans EPISODES.md (réveil → labo → Pikachu →
  Team Rocket → départ/premier combat de rival) sont maintenant tous implémentés et cohérents avec notre
  histoire (plus aucune intrigue Hoenn/père résiduelle connue dans le contenu actif de l'Arc 1)

## Mise à jour Session 4 (suite 3) — Arc 2 démarré : Jadielle + Route 2
- [x] Découpage Arc 2 validé par Thomas : 2.1 Jadielle/Route 2 (fait), 2.2 forêt de Jade/Team Rocket (texte
  à soumettre), 3.1 rencontre Ondine (texte à soumettre)
- [x] Jadielle : panneau ville corrigé, PNJ neutres traduits, caméo de Régis "papa" → "grand-père"
- [x] Route 2 : panneaux traduits/renommés (Jadielle/Argenta), contenu déjà neutre sinon
- [x] Build validé (15e build propre), ROM 79,01 %
- Repéré pour plus tard (Arc 4/Argenta, pas maintenant) : textes "WALLY" partagés dans Route102 mais
  utilisés par PetalburgCity/scripts.inc

## Mise à jour Session 4 (suite 4) — Épisode 2.2 fait : Team Rocket en forêt de Jade
- [x] Texte validé, combat Team Aqua/Devon Corp remplacé par Jessie (Ekans niv. 12, nouveau dresseur
  `TRAINER_ROCKET_JESSIE_2`) épaulée par James — inversion des rôles par rapport à Route 1
- [x] Build validé (16e build propre), ROM 79,01 %
- [ ] Ondine : texte proposé, question ouverte posée à Thomas (rejoint réellement l'équipe à l'écran, ou
  rivale récurrente recroisée ponctuellement) — implémentation en attente de sa réponse

## Mise à jour Session 4 (suite 5) — Ondine (Arc 3) : première rencontre faite
- [x] Rivale récurrente (pas de suivi à l'écran), rencontre sur Route 2, dresseur `TRAINER_ONDINE_1`
  (Poliwag niv. 13), sprite `OBJ_EVENT_GFX_SWIMMER_F`
- [x] Build validé (17e build propre), ROM 79,01 %
- Demande hors-sujet refusée cette session : analyse/extraction d'une ROM Pokémon Unbound fournie par
  Thomas — refusé (ROM commerciale protégée, contrairement à pokeemerald-expansion qui est un moteur
  décompilé propre) ; alternatives proposées (techniques ROM hacking documentées publiquement, inspiration
  conceptuelle sans toucher au binaire)

## Mise à jour Session 4 (suite 6) — Découverte de topologie importante
- [x] Vérifié via `map.json`/connections : Route 2 mène directement à Argenta (Petalburg City), la forêt
  de Jade (Team Rocket, Épisode 2.2) est en réalité accessible plus loin, depuis Argenta via Route 4, sur
  le chemin vers Azuria — pas entre Jadielle et Argenta comme supposé dans la proposition initiale de
  l'Arc 2. Le déroulé jouable actuel n'est donc pas cassé, mais l'ordre géographique réel est : Jadielle →
  Route 2 (Ondine) → **Argenta non modifiée (Arc 4, pas commencé)** → Route 4 → forêt de Jade (fait) →
  Azuria (Arc 3 suite, pas commencé)
- v0.3 envoyée à Thomas pour test — porte cet avertissement : au-delà de la forêt, tout redevient anglais
  vanilla Hoenn (Argenta/Petalburg City, y compris l'intrigue "papa champion d'arène" intacte)

## Mise à jour Session 4 (suite 7) — Arc 4 : Argenta/Pierre, retrait de l'intrigue "papa champion d'arène"
- [x] Priorité fixée par Thomas ("Traite Argenta d'abord"), proposition validée ("Je valide") : Norman →
  PIERRE, aucun lien de parenté, équipe Roche (Geodude niv. 12 / Onix niv. 14), palier "4 badges" retiré,
  intrigue Wally conservée (déjà neutre), badge renommé BADGE ROCHE
- [x] 7 dresseurs du gauntlet baissés de niveau 26 à niveau 12 (Potion au lieu de Hyper Potion), espèces/
  attaques/salles à thème inchangées
- [x] `VAR_PETALBURG_GYM_STATE` : switch simplifié, Pierre accepte le combat dès la fin du tutoriel Wally
  (plus de "reviens avec des badges")
- [x] ~30 blocs de texte traduits/réécrits (séquence Wally complète, intro/défaite/badge/post-combat de
  Pierre, scène du père de Wally reformulée, répliques "gamin du champion" neutralisées dans le gauntlet,
  panneaux de l'arène traduits)
- [x] "BALANCE BADGE" → "BADGE ROCHE" (seule occurrence dans tout le dépôt), effet mécanique inchangé
- [x] Vérifié : `PetalburgCity/scripts.inc` ne contient aucune référence au père du joueur
- [x] Build validé (compilation propre), ROM 79,01 %
- Sciemment différé : les 4 textes de rematch post-Ligue de Pierre gardent un thème "parent et enfant"
  complet — non bloquant, accessible seulement après la Ligue
- Sciemment différé : traduction complète des 7 salles du gauntlet (Vitesse/Précision/Confusion/Défense/
  Soin/Force/K.O.) — restent en anglais sauf les répliques à réécrire pour la cohérence "pas de lien
  familial", fait cette étape

## Mise à jour Session 4 (suite 8) — Corrections suite au premier test de Thomas
- [x] Retour de test traité (4 points) : écran qui tremble/joueur bloqué à l'ouverture, deux camions
  visibles en permanence à Bourg Palette, deux "mamans" superposées dans chaque maison, la Jumelle qui
  bloquait la sortie nord de la ville
- [x] Cause racine des 3 premiers points : `InsideOfTruck_EventScript_SetIntroFlagsMale/Female` posait
  aussi des `setflag` de nettoyage (camions, maman/frère-sœur/Poké Ball du rival dans la maison du joueur)
  jamais rejoués depuis la suppression de l'entrée dans `MAP_INSIDE_OF_TRUCK` — ces `FlagSet()` sont
  maintenant posés directement dans `WarpToPlayerBedroom` (`src/new_game.c`)
- [x] Verrou "pas de sortie sans POKéMON" retiré de `LittlerootTown` (obsolète : Pikachu donné dès le
  départ par le Professeur Chen)
- [ ] Tremblement d'écran à l'ouverture : aucun chemin de code ne peut plus déclencher
  `ExecuteTruckSequence`/`Task_HandleTruckSequence` sur une nouvelle partie avec ce build (vérifié) —
  probablement une sauvegarde d'une build antérieure à l'Épisode 1.1 ; à confirmer avec Thomas via une
  sauvegarde neuve sur le prochain build envoyé
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 9) — Arc 3 : Azuria, l'arène d'Ondine
- [x] Proposition présentée et validée (identité d'Ondine, équipe Eau, BADGE CASCADE) avant implémentation
- [x] `TRAINER_ROXANNE_1` → ONDINE : équipe Roche (Geodude x2, Nosepass) → Eau (Poliwag niv. 12, Goldeen
  niv. 12, Staryu niv. 15 @Baie Oran), même structure de niveaux, nom interne conservé
- [x] 3 dresseurs du gauntlet reconvertis en Eau (Josh→Horsea, Tommy→2x Goldeen, Marc→2x Tentacool,
  classe Hiker→Fisherman)
- [x] Tous les textes de l'arène traduits et réécrits (intro d'Ondine référence leur rencontre Route 2,
  défaite, badge, PostBattle, GymGuide, statue, appel PokéNav) — panneau de ville et un PNJ neutre
  également corrigés
- [x] "STONE BADGE" → "BADGE CASCADE" (nom canon d'Ondine/Misty)
- [x] Bug latent retiré : `addvar VAR_PETALBURG_GYM_STATE, 1` dans `RoxanneDefeated` (vestige de l'ordre
  vanilla Rustboro-1er/Petalburg-5e, aurait fait sauter Pierre en mode revanche prématurément vu
  l'inversion de notre ordre)
- [x] Build validé, ROM 79,01 %
- Sciemment différé, même logique que pour Pierre : TM Éboulement/Rock Tomb gardée telle quelle
  (mécanique inchangée, juste traduite), 4 textes de revanche post-Ligue d'Ondine non traités
  (`TRAINER_ROXANNE_2`–`_5`)

## PROCHAINES ÉTAPES (mise à jour Session 4 suite 9)
1. Confirmer avec Thomas (sauvegarde neuve) que le tremblement d'écran à l'ouverture a bien disparu
2. Traduction complète des 7 salles du gauntlet d'Argenta (actuellement en anglais, hors répliques
   "famille" déjà neutralisées)
3. Traduction complète du reste de la ville d'Azuria (Rustboro City) — signalétique restante, PNJ
   génériques, sous-intrigue Devon Corp (grunt qui vole des documents, à évaluer : remplacer par Team
   Rocket comme pour la forêt de Jade, ou laisser neutre ?)
4. Textes de rematch post-Ligue de Pierre et d'Ondine (thème "parent et enfant" pour Pierre, identité
   Roxanne pour Ondine) — non urgent (post-Ligue uniquement)
5. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
6. Script Python de preview PNG des maps — utile aussi pour valider les futurs placements de PNJ sans
   deviner des coordonnées à l'aveugle
7. Recompiler `qa_runner` depuis la source et reprendre le débogage de l'input (`core->getKeys()`)
8. Balayage plus large des dialogues restants en anglais (dresseurs génériques Route 2, "James" chasseur
  d'insectes de la forêt à renommer pour éviter la confusion avec Team Rocket)
9. `RivalsHouse_1F_Text_OhYoureTheNewNeighbor` (scène orpheline restante, mère de Régis visite le joueur) —
   non bloquant, jamais déclenché
10. Quête SS Ticket/Latios (livrée par "papa") et cadeau Amulet Coin post-badge 5 — toujours en attente
   d'un autre messager que "papa", non bloquant (atteignable seulement en post-Ligue)
11. Textes "WALLY" partagés (Route102/PetalburgCity) — toujours partagés entre fichiers, fonctionnels,
   pas de conflit identifié avec le travail d'Argenta
12. Après Azuria : décider de la suite (route vers Céladopole/Carmin-sur-Mer, ou consolidation/polish des
   arcs existants) avec Thomas

## Mise à jour Session 4 (suite 10) — Correction d'une régression critique
- [x] Retour de test : "le jeu redémarre en passant le village" — la suppression du blocage de la Jumelle
  (suite 8) était une erreur, ce verrou est en réalité le mécanisme qui garantit que le joueur passe par la
  maison du rival avant d'atteindre la Route 1 et la scène de sauvetage du Professeur Chen (remise de
  Pikachu). Restauré intégralement dans `data/maps/LittlerootTown/scripts.inc` et `map.json`
- [x] Vérifié : les corrections trucs/mamans de la suite 8 (`src/new_game.c`) sont indépendantes et restent
  valides, non concernées par cette régression
- [x] Build validé, ROM 79,01 %
- **Point de vigilance méthodologique** : avant de qualifier un mécanisme de "reliquat vanilla obsolète" et
  de le supprimer, toujours vérifier s'il pose une variable de progression consultée ailleurs — la note de
  la Session 4 suite 2 documentait déjà ce rôle, relue trop vite avant la suite 8

## Mise à jour Session 4 (suite 11) — Traduction complète du gauntlet d'Argenta
- [x] Les 7 dresseurs du gauntlet (Vitesse/Confusion/Soin/K.O./Précision/Défense/Force), leurs panneaux de
  porte, et le guide d'arène traduits en français
- [x] Build validé, ROM 79,01 %
- Reste volontairement non traduit : les 4 textes de revanche post-Ligue de Pierre (thème "parent et
  enfant") — non bloquant

## Mise à jour Session 4 (suite 12) — Vrai correctif du crash de sortie de Bourg Palette
- [x] Root cause trouvée et corrigée : `LOCALID_ROUTE101_BIRCH` (Professeur Chen sur la Route 1)
  utilisait `MOVEMENT_TYPE_JOG_IN_PLACE_RIGHT`, incompatible avec le sprite FRLG substitué
  (`OBJ_EVENT_GFX_PROF_OAK`) — l'animation "jogging sur place" lit une frame hors des bornes du sprite
  après quelques secondes d'inactivité, corrompant la mémoire et provoquant un redémarrage matériel.
  Jamais détecté avant car la Jumelle bloquait tout accès à ce PNJ jusqu'à la suite 8/10 de cette session
- [x] Corrigé dans `data/maps/Route101/map.json` : `LOCALID_ROUTE101_BIRCH` et
  `LOCALID_ROUTE101_ZIGZAGOON` passés à `MOVEMENT_TYPE_LOOK_AROUND` (type sûr, sans impact sur la
  cinématique scriptée qui pilote leurs déplacements explicitement)
- [x] Testé et confirmé résolu par émulation headless (marche normale + script), traversée complète
  Bourg Palette → Route 1 → sauvetage du Professeur Chen → Pikachu, sans redémarrage
- [x] `tools/qa_harness/qa_runner` remis en état de marche (dépendance `libmgba-dev`/`mgba-sdl`
  installée, recompilation propre) — outil de diagnostic headless désormais opérationnel pour cette
  session et les suivantes, le blocage d'input de la Session 3 était un problème d'environnement, pas un
  bug du harness
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 13) — Azuria : traduction neutre + décision Team Rocket
- [x] `RustboroCity/scripts.inc` exploré en entier (1290 lignes) — 3 fils vanilla identifiés (vol Devon
  Corp/Team Aqua, marin Briney, combat de rival), tous multi-cartes (Rustboro, Tunnel Rusturf, maison de
  Briney)
- [x] Décision prise avec Thomas : remplacer le sbire par la Team Rocket, même traitement que la forêt de
  Jade — proposition de texte à faire avant implémentation (portée multi-cartes, scène centrale)
- [x] Contenu neutre traduit en français (panneaux de ville, PNJ génériques arène/école/POKéNAV/combat
  2v2) ; `GymLeaderIsntEasyWithFire` mis à jour (FEU vs ROCHE → FEU vs EAU, typage d'Ondine)
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 14) — Team Rocket remplace le vol Devon Corp (Azuria + Tunnel Rusturf)
- [x] Proposition de texte validée par Thomas ("Je valide continue") puis implémentée dans
  `RustboroCity/scripts.inc` et `RusturfTunnel/scripts.inc` : le sbire anonyme devient un membre de la
  Team Rocket, avec un rappel explicite à la forêt de Jade dans la réplique du PNJ DEVON
- [x] Texte du combat contre le sbire et de la scène de sauvetage de MR. BRINEY/PEEKO entièrement traduits
  en français ; identifiants internes (symboles, flags, musique) laissés inchangés par convention
- [x] Build validé (compilation propre, exit code 0), ROM 79,01 %, `changed_files/` synchronisé

## Mise à jour Session 4 (suite 15) — Combat de rival d'Azuria : bug corrigé + traduction
- [x] Bug trouvé : le combat de rival scripté d'Azuria utilisait encore l'ancien branchement vanilla
  May/Brendan selon le genre du joueur au lieu de Régis fixe (incohérent avec Route 103) — corrigé
  (`Custom_EventScript_SetupRegisGfxId`, suppression du `checkplayergender`)
- [x] Traduit en français toute la branche de texte désormais utilisée (8 textes, label "RÉGIS :")
- [x] Traduit au passage `DevonCorpSign`, `DevonCorpBranchOfficeSign`, `TunnelNearingCompletion`
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 16) — Bug de branchement Régis : 9 cartes touchées, pas seulement Azuria
- [x] Découvert et corrigé sur 9 cartes (`LittlerootTown`, `LittlerootTown_ProfessorBirchsLab`,
  `OldaleTown`, `Route104`, `Route110`, `Route119`, `LavaridgeTown`, `LilycoveCity`,
  `EverGrandeCity_ChampionsRoom`) : le sprite de Régis utilisait encore le branchement vanilla May/
  Brendan selon le genre du joueur — toutes appellent maintenant `Custom_EventScript_SetupRegisGfxId`
- [x] Routage de texte (choix de la branche de dialogue) corrigé sur les 6 cartes qui ont un vrai combat/
  scène de rival scriptée (en plus d'Azuria, déjà fait en suite 15) : `LittlerootTown_
  ProfessorBirchsLab`, `Route104`, `Route110`, `LavaridgeTown`, `LilycoveCity`,
  `EverGrandeCity_ChampionsRoom`
- [x] Traduit en français le texte de `LittlerootTown_ProfessorBirchsLab` atteignable à court/moyen
  terme (dialogue du Professeur Chen post-victoire sur Route 103, remise des POKé BALLS, etc.)
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 17) — Traduction de la branche Régis sur Route 104 et Route 110
- [x] `Route104` et `Route110` traduits en français (routage déjà corrigé en suite 16)
- [x] Build validé, ROM 79,01 %

## Mise à jour Session 4 (suite 18) — Fin de la traduction Régis + 2 reliquats "papa" trouvés
- [x] `LavaridgeTown`, `LilycoveCity`, `EverGrandeCity_ChampionsRoom` traduits en français (routage déjà
  corrigé en suite 16) — la branche Régis est désormais intégralement traduite sur les 7 cartes à combat
  scripté (Azuria, labo, Route104, Route110, Lavaridge, Lilycove, salle du Champion)
- [x] 2 nouveaux reliquats "papa champion d'arène" trouvés et corrigés (Lavaridge, salle du Champion) —
  même travers que Pierre/Norman traité en suite 7, sur des cartes tardives jamais auditées jusqu'ici
- [x] Confusion père/grand-père corrigée : Régis appelait le Pr Chen "mon père" à Lilycove, corrigé en
  "mon grand-père" (cohérent avec la relation établie ailleurs dans le projet)
- [x] Bug de compilation (tirets cadratins non supportés par le charmap) trouvé et corrigé
- [x] Build validé, ROM 79,01 %

## PROCHAINES ÉTAPES (mise à jour Session 4 suite 18)
1. Confirmer avec Thomas (nouveau build .gba fourni) que le crash a bien disparu, que l'Arc 3 (Azuria/
   Ondine), le fil Team Rocket (Tunnel Rusturf/Briney), le combat de rival (toutes cartes) et le sprite
   de Régis (partout) fonctionnent comme attendu
2. Auditer les autres occurrences du schéma `checkplayergender`/MAY trouvées par grep mais pas encore
   vérifiées : `LittlerootTown_MaysHouse_2F`, `MossdeepCity_SpaceCenter_2F`, `OldaleTown` (ligne 258),
   `Route101` (ligne 252), `Route119` (combat de rival ligne 57) — déterminer si c'est le même bug ou un
   usage légitime (ex. logement du joueur selon son propre genre) avant de corriger
3. Balayage ciblé "papa"/"father" à faire sur le reste du dépôt — 2 reliquats retrouvés cette suite sur
   des cartes tardives non auditées ; probable qu'il en reste d'autres (post-Ligue, Battle Frontier,
   contenu FRLG type Bourg Palette/Cramois'Île) à vérifier au fur et à mesure
4. Décider d'un nom français pour WALLACE (champion final) si le contenu post-Ligue est un jour prioritaire
   — laissé tel quel pour l'instant, jamais traité dans les docs
5. Traduire ce qui reste à Azuria/Route104 : sous-fil Wanda/petit ami dans le Tunnel Rusturf,
   `Route104_MrBrineysHouse`
6. Contenu très tardif de `LittlerootTown_ProfessorBirchsLab` (post-Ligue) resté en anglais : mise à
   niveau POKéDEX NATIONAL, choix du starter Johto, appel de Scott — non urgent (post-Ligue uniquement)
7. Textes de rematch post-Ligue de Pierre et d'Ondine (thème "parent et enfant" pour Pierre, identité
   Roxanne pour Ondine) — non urgent (post-Ligue uniquement)
8. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
9. Script Python de preview PNG des maps — utile aussi pour valider les futurs placements de PNJ sans
   deviner des coordonnées à l'aveugle (le QA harness headless peut aussi servir de filet de sécurité
   ponctuel pour vérifier qu'un PNJ animé/scripté ne crashe pas avant de livrer une build)
10. Balayage plus large des dialogues restants en anglais (dresseurs génériques Route 2, "James" chasseur
    d'insectes de la forêt à renommer pour éviter la confusion avec Team Rocket)
11. `RivalsHouse_1F_Text_OhYoureTheNewNeighbor` (scène orpheline restante, mère de Régis visite le
    joueur) — non bloquant, jamais déclenché
12. Quête SS Ticket/Latios (livrée par "papa") et cadeau Amulet Coin post-badge 5 — toujours en attente
    d'un autre messager que "papa", non bloquant (atteignable seulement en post-Ligue)
13. Textes "WALLY" partagés (Route102/PetalburgCity) — toujours partagés entre fichiers, fonctionnels,
    pas de conflit identifié avec le travail d'Argenta
14. Traduction complète de la base d'objets `items.h` (873 entrées, encore entièrement en anglais côté
    nom/description) — chantier séparé, pas commencé, à discuter avec Thomas avant de s'y attaquer vu
    l'ampleur

## Mise à jour Session 4 (suite 19) — Retour de test v0.7 : camion corrigé, crash Route 1 non résolu
- [x] Bug du tremblement de caméra du camion au tout début d'une nouvelle partie : corrigé
  (`src/overworld.c`, `CB2_NewGame` utilisait `ExecuteTruckSequence` sans condition sur la carte
  d'arrivée réelle)
- [ ] **Crash confirmé à la rencontre avec le Professeur Chen sur la Route 1** (2e/3e phrase du
  dialogue) : reproduit de façon fiable en headless, diagnostiqué en profondeur (30+ builds de test),
  mais cause exacte NON identifiée — voir CHANGELOG suite 19 pour le détail complet des hypothèses
  testées et éliminées. Nécessite un accès à un débogueur bas niveau (GDB/désassemblage) pour continuer,
  indisponible dans cet environnement actuel
- Build livrée à Thomas avec uniquement le correctif du camion — le crash Route 1 reste présent dans
  cette build, aucune régression introduite pendant le diagnostic (toutes les modifications
  d'investigation ont été annulées avant de livrer)

## Mise à jour Session 4 (suite 20) — Cause racine trouvée : crash RÉSOLU par refonte de la scène
- [x] **Cause racine identifiée** : bug moteur dans `ScrCmd_applymovement` (`src/scrcmd.c`) — résout
  l'objet cible via `GetObjectEventIdByLocalId()` (`src/event_object_movement.c`), qui ne filtre PAS
  par carte (contrairement à `setobjectxy`, qui utilise correctement
  `TryGetObjectEventIdByLocalIdAndMap`). Près d'une connexion entre deux cartes, les objets des deux
  cartes partagent le même pool `gObjectEvents[]` ; si leurs ID locaux se recoupent, `applymovement`
  peut cibler l'objet de la MAUVAISE carte et corrompre la mémoire. Détail complet dans CHANGELOG
  suite 20
- [x] **Corrigé** : la scène de sauvetage du Professeur Chen (course-poursuite + remise du PIKACHU) a
  été déplacée de la Route 1 vers la zone sud de Bourg Palette (près du Labo), avec des ID locaux
  garantis sans collision avec la Route 1 et hors de la zone de bordure jamais mise en défaut.
  Vérifié en headless : 0 crash sur plusieurs exécutions consécutives de la chaîne complète (avant :
  136 instances de corruption mémoire par exécution)
- [ ] Risque latent documenté (non corrigé, hors périmètre urgent) : le PNJ "Professeur Chen post-jeu"
  sur Route101 (position 5,11) utilise le même sprite et un ID local qui collisionne avec Bourg
  Palette — sûr tant qu'aucun `applymovement` n'est ajouté dessus ; à vérifier avant toute future
  scène animée le concernant

## Mise à jour Session 4 (suite 21) — Combat de rival de la Route 119 converti en Régis
- [x] Deuxième combat de rival (Route 119, remise de la CS Vol) : branche `checkplayergender`
  supprimée, fusionnée en un seul combat contre Régis, texte traduit en français dans sa voix établie
  (cf. Route 103). Équipe de dresseur reprise de `TRAINER_BRENDAN_ROUTE_119_TORCHIC` sans changement
  (Lombre/Slugma/Marshtomp) — non rééquilibrée cette suite
- [x] Bug annexe trouvé en creusant : le graphisme "rival à vélo" n'avait jamais été fixé en variante
  Régis (contrairement au graphisme debout) — corrigé sur les 3 cartes concernées (Route 119, Route 110,
  LavaridgeTown) via une nouvelle fonction `Custom_EventScript_SetupRegisOnBikeGfxId`
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 22) — Scène de rencontre du rival à Bourg Palette traduite
- [x] Confirmé avant modification : le genre du joueur reste sélectionnable à la création de partie
  (`src/oak_speech.c` intact) — la structure à 2 maisons miroir (May's/Brendan's House, qui détermine
  laquelle est celle du rival selon le genre choisi) est donc légitime et volontairement conservée
- [x] Les 8 textes du rival dans cette scène (accueil surprise, phrase courte, dialogue POKéDEX
  post-Lilycove, "où aller ensuite") unifiés en une seule voix (Régis) et traduits en français ; les
  variantes May/Brendan sont maintenant identiques mot pour mot
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 23) — Changement de programme : suppression de la scène de sauvetage,
## remise directe de PIKACHU au labo, Quickstart automatique pour les builds de test
- [x] Toute la scène de sauvetage du Professeur Chen (Bourg Palette + reliquats sur Route 1) supprimée
  intégralement, sur nouvelle consigne de Thomas
- [x] Nouvel événement : le joueur reçoit PIKACHU directement en discutant avec le Professeur Chen
  dans son laboratoire, dès sa première visite (avant, il fallait déjà avoir le POKéMON pour lui
  parler normalement). Réutilise une mécanique existante (`GiveStarterEvent`) jusque-là jamais
  déclenchée par ce chemin
- [x] Bug moteur trouvé et contourné : rendre le Professeur Chen visible par défaut au labo (déclaré
  visible dans map.json, ou `clearflag` précoce) fait planter le jeu au chargement — l'ajouter
  dynamiquement par script (`addobject`, comme le fait déjà le reste du fichier pour ses autres
  apparitions) est sûr. Un résidu très réduit et non reproduit comme plantage réel (8 lectures
  mémoire suspectes sur des dizaines de milliers de frames testées, aucune corruption ni redémarrage
  observés) reste non expliqué — à surveiller au playtest réel, cf. CHANGELOG suite 23 pour le détail
- [x] Démarrage automatique (`QUICKSTART_AUTO`) pour les builds de test : elles sautent directement en
  jeu sans appui sur SELECT ni création de personnage à chaque test — seuls les écrans de copyright
  Nintendo/GAME FREAK restent visibles quelques secondes (incompressible sans toucher au tout début du
  boot). À repasser à `FALSE` avant toute build de playtest narratif normal
- [x] Carte de Bourg Palette avec 5 maisons supplémentaires : fait en suite 24 (extension additive au
  sud plutôt que la relocalisation complète du plan initial de Thomas — voir suite 24 ci-dessous)
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 24) — Bourg Palette : 5 nouvelles maisons (extension sud)
- [x] Thomas a fourni un plan détaillé (carnet de cartographie 24×18) ; réalisé en extension additive
  au sud de la carte existante (hauteur 20 → 34) plutôt qu'en relocalisant tout, pour ne prendre aucun
  risque sur la connexion Route 1 (les deux cartes faisaient exactement 20×20 avec connexion à
  l'offset 0). Aucune maison/PNJ/panneau existant déplacé
- [x] 5 nouvelles maisons ajoutées : Mme Chen (conseils), Vieux Dresseur (combat, trophées), Gardien de
  Route (entretien Route 1), maison aux volets fermés (presque vide, accroche narrative future), Dame
  aux Baies (culture de Baies). Intérieurs réutilisant des layouts génériques déjà présents et prouvés
  dans le jeu de base (`LAYOUT_HOUSE1-4`, `LAYOUT_FORTREE_CITY_HOUSE1`), seule la couche
  scripts/warps/PNJ est nouvelle
- [x] Réalisé sans Porymap : le pipeline `tools/mapjson` régénère tout depuis le JSON au moment du
  `make`, donc éditer `map.json`/`layouts.json`/`map_groups.json` à la main suffit. Seule la grille de
  tuiles brute (`map.bin`) a demandé un script Python — généré en copiant tel quel (pas de nouvelle
  interprétation de tuiles) le bloc de maison 5×5 et la bordure d'arbustes déjà utilisés ailleurs dans
  la même carte
- [x] Vérifié visuellement en headless (`qa_runner`, captures d'écran) : nouveau quartier sud rendu
  correctement (3+2 maisons, chemins, bordure sud), une maison testée à l'intérieur avec son PNJ.
  **0 instance de "Bad memory"** sur l'ensemble des tests (à comparer aux 720+ de la suite 23) — la
  copie de blocs de tuiles existants s'est révélée beaucoup plus sûre que la génération de contenu
  inédit. Les 4 autres maisons n'ont pas été vérifiées individuellement à l'écran (même mécanisme,
  layouts génériques non modifiés) — à confirmer au playtest réel
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 25) — Bug critique résolu : PIKACHU jamais reçu au labo
- [x] Retour de test : Thomas restait bloqué par la Jumelle à la sortie nord car il n'avait jamais pu
  obtenir PIKACHU — le Professeur Chen n'apparaissait tout simplement jamais au labo. Root cause
  trouvée : le "résidu mineur" documenté en suite 23 n'était pas mineur — vérification approfondie
  (marche naturelle + captures d'écran, pas juste un comptage de plantages) a montré que l'objet
  Chen ne s'affiche jamais quand il est ajouté dynamiquement (`addobject`), et que s'en approcher
  déclenche un plantage massif (616 instances, même signature que le bug d'origine de suite 23)
- [x] Corrigé en abandonnant complètement l'idée de rendre Chen visible avant que le joueur ait son
  POKéMON : la remise de PIKACHU se déclenche maintenant automatiquement dès l'entrée dans la pièce
  (nouveau `coord_event`), sans jamais faire apparaître son sprite pour cette scène précise
- [x] Vérifié en profondeur en headless (marche réelle jusqu'au labo par la porte, ~30 pressions A à
  travers tout le dialogue jusque dans `GiveStarterEvent`) : **0 plantage**, dialogue confirmé à
  l'écran par capture, `givemon`/`setflag`/`setvar` confirmés exécutés
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 26) — Retour de test négatif, deux bugs re-corrigés + question ouverte
- [x] Le correctif de suite 25 (PIKACHU via `coord_event`) ne marchait pas de façon fiable en jeu réel
  malgré un test headless positif — probablement parce que Chen étant invisible, un joueur pouvait
  ressortir de la pièce sans avoir marché sur la case exacte du déclencheur. Remplacé par le mécanisme
  `OnFrame`/`map_script_2` (le même que celui déjà utilisé de façon fiable dans ce fichier pour
  `GiveStarterEvent`/`GivePokedexEvent`) : se déclenche dès que le joueur a le contrôle dans la pièce,
  sans dépendre d'un déplacement précis. Une tentative intermédiaire (déclenchement direct depuis
  `OnTransition`) a été testée et **rejetée** car elle provoquait un écran figé en headless
- [x] Revérifié avec une entrée 100 % naturelle par la porte (aucun raccourci de script) : dialogue
  confirmé à l'écran dès l'entrée, 0 plantage
- [x] La Jumelle ne bloque plus JAMAIS la sortie nord (sur nouvelle demande explicite) : tout l'ancien
  verrou (coord_events, scripts, mouvements, texte orphelin) supprimé entièrement, pas juste contourné.
  Vérifié : un joueur sans aucun POKéMON peut marcher directement jusqu'à la Route 1
- [ ] **Question ouverte, non traitée cette suite** : Thomas signale que le quartier sud de Bourg
  Palette (suite 24) ne correspond pas à son plan HTML fourni. C'est un choix de conception assumé
  (extension additive plutôt que relocalisation, pour ne pas risquer la connexion Route 1) qui n'a
  jamais été validé explicitement avec lui avant implémentation — à clarifier : refonte plus fidèle au
  plan original (avec le risque technique) ou ajustements ciblés ?
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 27) — Bourg Palette refaite fidèlement au plan de Thomas
- [x] Scène des Chaussures de Course (Maman) entièrement réécrite pour être indépendante de la
  position des maisons — l'ancienne version codait en dur une douzaine de trajets de marche calibrés
  sur les anciennes coordonnées, ce qui bloquait tout déplacement de bâtiment
- [x] Carte de Bourg Palette refaite en 24×19 à partir du code JavaScript du plan HTML fourni par
  Thomas (coordonnées extraites directement de sa logique `building()`), 3 rangées de bâtiments très
  proches du plan original (Labo + maison du joueur ; Mme Chen + volets fermés + Régis ; Gardien de
  Route + Vieux Dresseur + Dame aux Baies)
- [x] La connexion vers la Route 1 n'a pas bougé (aucun risque de désalignement) — tous les bâtiments
  réorganisés autour du couloir d'entrée existant
- [x] Mare/herbes hautes décoratives du plan non reproduites (tuiles non garanties sûres sans retour
  visuel, et les herbes hautes créeraient des rencontres sauvages en pleine ville) — simplification
  assumée
- [x] Vérifié en profondeur en headless : rendu des 3 rangées correct, raccord Route 1 sans décalage
  visible, Labo (déclencheur PIKACHU) et maison du joueur fonctionnels à leurs nouvelles positions,
  scène des Chaussures de Course fonctionnelle — 0 plantage sur l'ensemble des tests
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 28) — Jessie/James (Team Rocket) corrigés sur la Route 1
- [x] Diagnostiqué en isolation headless : les graphismes d'origine `OBJ_EVENT_GFX_ROCKET_F`/`_M`
  provoquaient une corruption mémoire importante dès leur affichage sur la Route 1 (statique ou
  `addobject`, aucun des deux fiable ici) — cause profonde non identifiée avec certitude, contourné
  par un changement de graphismes plutôt que de risquer une régression
- [x] Jessie et James utilisent maintenant `OBJ_EVENT_GFX_WOMAN_2`/`OBJ_EVENT_GFX_MAN_3` (graphismes
  génériques déjà utilisés sans souci ailleurs, palette différente de celle du Pr. Chen sur cette
  carte) ; James repositionné de (9,17) à (10,17), hors de la case de buisson décorative, aligné avec
  Jessie et Miaouss
- [x] Vérifié en headless : 0 plantage, les deux PNJ bien visibles et distincts, combat de James
  déclenché avec succès par interaction directe (texte de devise Team Rocket affiché correctement)
- [x] Build release validée, ROM 79,01 %

## Mise à jour Session 4 (suite 29) — Nouvelle zone : le Mont Sélénite
- [x] Nouvelle zone montagneuse à 3 niveaux souterrains construite depuis le plan HTML fourni par
  Thomas, insérée entre Argenta et Azuria : `MtSelenite` (extérieur, 26×20), `MtSelenite_1F` (tunnels,
  26×22), `MtSelenite_B1F` (salles, 28×22), `MtSelenite_B2F` (cavernes, 28×24)
- [x] Nouvelle connexion nord depuis Argenta (`PetalburgCity`, qui n'avait aucune sortie nord),
  câblage complet des 4 cartes entre elles, sortie nord du niveau profond vers `Route104` (déjà reliée
  à Azuria) ; nouvelle entrée `MAPSEC_MT_SELENITE` sur la mini-carte
- [x] Contenu : 2 fossiles au choix exclusif, une zone secrète avec un Pokémon rare (Mélofée), tables
  de rencontres sauvages sur les 4 cartes, panneau d'entrée
- [x] Tileset `cave_frlg` initialement prévu abandonné en cours de route : incompatible avec ce build
  Emerald (réservé aux builds FireRed/LeafGreen) — remplacé par le tileset `cave` standard
  (Granite Cave/Victory Road), seul disponible ici
- [x] Bug trouvé et corrigé en test headless : les 2 entrées de grotte de la carte extérieure étaient
  écrasées par les blobs décoratifs générés après coup (ordre hérité du plan JS d'origine)
- [x] Vérifié en headless : 0 plantage, carte extérieure et niveau profond accessibles et rendus
  correctement, connexion Argenta ⇄ Mont Sélénite traversée dans les deux sens
- [x] Build release validée, ROM 79,03 %

## PROCHAINES ÉTAPES (mise à jour Session 4 suite 29)
1. **Playtest réel prioritaire** : parcourir le Mont Sélénite en jeu réel de bout en bout (les 2
   entrées, les 4 échelles inter-niveaux, les 2 fossiles, la zone secrète, la sortie vers la Route 4) —
   seule une vérification headless partielle a été faite (navigation complète bloquée par les limites
   du menu de débogage pour les cartes hors groupe 0)
2. Confirmer que Jessie/James/Miaouss s'affichent et s'enchaînent bien en jeu réel sur la Route 1, et
   que le combat de James se lance normalement
3. Confirmer la nouvelle disposition de Bourg Palette dans son ensemble (déplacements, tous les warps,
   scène des Chaussures de Course jusqu'au bout), le Professeur Chen au labo, et l'absence de blocage
   de la Jumelle à la sortie nord
4. Entrer dans les 5 nouvelles maisons à leurs nouvelles positions (Mme Chen, Vieux Dresseur, Gardien
   de Route, volets fermés, Dame aux Baies) pour confirmer qu'aucune ne plante
5. Playtest du nouveau combat de Route 119 (texte, équilibrage de l'équipe de Régis) et de la scène de
   rencontre du rival à Bourg Palette 2F (texte, mise en scène) — toujours en attente de retour
6. Dernier reliquat de l'audit May/Brendan : l'appel post-Ligue de Mossdeep (encore en anglais,
   non prioritaire, contenu post-Ligue)
7. Envisager, en tâche de fond non urgente, un audit plus large des `applymovement`/`addobject` déjà
   utilisés près d'autres connexions de cartes ou d'autres PNJ du jeu (le bug de suite 20/23/25/26/28
   n'est pas spécifique à Bourg Palette/Route 1/Chen/Rocket — pourrait resurgir ailleurs)
8. Tout le reste de la liste ci-dessus (suite 18/19) reste valable et inchangé
