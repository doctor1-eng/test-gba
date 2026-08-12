# PROJECT_STATUS.md

Dernière mise à jour : 2026-08-12 (Session 4, suite 4)

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

## PROCHAINES ÉTAPES (mise à jour Session 4 suite 4)
1. Trancher puis implémenter la première rencontre avec Ondine (Arc 3) — mécanique de suivi façon Pikachu,
   ou simple rivale récurrente ? En attente de la décision de Thomas
2. Renommage complet des maps Hoenn → Kanto (level design, au-delà du texte)
3. Script Python de preview PNG des maps
4. Recompiler `qa_runner` depuis la source et reprendre le débogage de l'input (`core->getKeys()`)
5. Balayage plus large des dialogues restants en anglais (dresseurs génériques Route 2, "James" chasseur
  d'insectes de la forêt à renommer pour éviter la confusion avec Team Rocket)
6. `RivalsHouse_1F_Text_OhYoureTheNewNeighbor` (scène orpheline restante, mère de Régis visite le joueur) —
   non bloquant, jamais déclenché
7. Quête SS Ticket/Latios (livrée par "papa") et cadeau Amulet Coin post-badge 5 — toujours en attente
   d'un autre messager que "papa", non bloquant (atteignable seulement en post-Ligue)
8. Textes "WALLY" (Route102/PetalburgCity) — à traiter avec l'Arc 4 (Argenta/Pierre)
