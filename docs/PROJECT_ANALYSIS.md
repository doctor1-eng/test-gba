# PROJECT_ANALYSIS.md — Pokémon Kanto Saison 1 (Fangame)

Date : 2026-08-11
Statut : Phase 0 — Audit terminé, aucune modification de contenu effectuée.

---

## 1. Identification du fichier fourni

| Champ | Valeur |
|---|---|
| Fichier | `1986_-_Pokemon_Emerald__U__TrashMan_.gba` |
| Taille | 16 777 216 octets (16 Mo) — taille standard GBA, non étendue |
| Titre interne | `POKEMON EMER` |
| Code jeu | `BPEE` (Emerald, région U/Europe-US) |
| Maker code | `01` |
| MD5 | `605b89b67018abcea91e693a4dd25be3`* |
| SHA1 | `f3ae088181bf583e55daf962a92bb46f4f1d07b7`* |

*(hash tronqués dans la sortie console — recalculables à la demande)*

**Constat clé : ce fichier est une ROM Pokémon Émeraude déjà patchée ("TrashMan"), pas une base vierge.** C'est un hack binaire existant, avec ses propres modifications de scripts/texte/mécaniques déjà injectées dans le binaire d'origine. Sa structure interne (pointeurs, offsets libres, tables déplacées) est probablement différente de celle d'un Émeraude vanilla — elle n'est pas documentée et nécessiterait une rétro-ingénierie dédiée avant toute modification fiable.

---

## 2. Deux architectures techniques possibles — analyse comparative

C'est la décision la plus structurante du projet. Je ne tranche pas seul (règle de confirmation sur les décisions fondamentales) — voici l'analyse.

### Option A — Patch binaire direct sur la ROM TrashMan fournie
Méthode déjà maîtrisée dans les projets précédents (FireRed Rocket Edition, TrashMan 2) : extraction de chaînes, charmap, réinsertion, repointing, byte-à-byte en Python.

**Avantages**
- Méthode connue, pipeline déjà rodé et productif (~2 300 lignes traduites en FireRed).
- Rapide pour du texte, des tables de données (dresseurs, objets, Pokémon rencontrables).

**Limites sérieuses pour CE projet précis**
- Créer de **nouvelles maps**, de **nouveaux sprites**, de **nouveaux scripts d'événements complexes** en patch binaire pur est extrêmement lourd et risqué sans outils graphiques dédiés (Advance Map, Porymap, XSE) — or ces outils sont des applications GUI Windows/Qt non exécutables dans cet environnement (sandbox headless, pas d'affichage, accès réseau restreint à GitHub/PyPI/npm — pas d'accès à romhacking.net ou autres dépôts d'outils).
- Le contenu narratif visé (arcs complets, Team Rocket récurrente, arènes scénarisées, Ligue) implique des dizaines de nouvelles maps et scripts — quasi impossible à produire de façon fiable en édition binaire pure sans éditeur visuel de maps.
- On hérite des choix de design déjà faits par TrashMan, potentiellement incompatibles avec la vision "anime".

### Option B — Reconstruction sur le moteur décompilé `pokeemerald` (pret/pokeemerald)
**Test réalisé pendant cet audit** : j'ai cloné le dépôt et **compilé avec succès une ROM fonctionnelle** dans ce sandbox (toolchain `gcc-arm-none-eabi` + `binutils-arm-none-eabi`, installables via les dépôts Ubuntu autorisés). Résultat : `pokeemerald_modern.gba`, build propre, usage mémoire ROM 39,68 %.

**Ce que ça change concrètement**
- Le jeu devient un projet **en code source C + assembleur + données JSON/texte**, versionnable, modifiable par scripts Python/édition de fichiers — exactement le type de travail que je peux faire de façon fiable dans ce sandbox.
- Les maps, événements, dresseurs, Pokémon, dialogues sont des fichiers texte/JSON structurés (pas des offsets binaires opaques) → création de nouvelles maps et de nouveaux scripts devient réellement faisable, pas seulement de la retouche.
- Aligné avec l'ambition du cahier des charges (arcs narratifs complets, personnages avec sprites et comportements propres, arènes scénarisées).

**Limites honnêtes**
- **On ne repart pas du contenu TrashMan.** On repart d'Émeraude vanilla en tant que moteur ; le travail de TrashMan (déjà fait sur la ROM binaire) n'est pas récupérable dans cette architecture. Il peut servir d'inspiration/référence, pas de base de code.
- **Porymap** (éditeur de maps visuel officiel de l'écosystème pret) est une appli Qt graphique — inutilisable en headless ici. Deux sous-options : (1) éditer directement les fichiers JSON de maps par script (viable mais fastidieux et plus risqué visuellement sans rendu), ou (2) toi (Thomas) fais l'édition visuelle des maps en local avec Porymap et je prends en charge tout le reste (scripts, dialogues, données, compilation, doc) — **recommandé**.
- Pas d'émulateur graphique disponible ici pour du playtest visuel — seuls la compilation et des tests automatisés (le framework de tests intégré à pokeemerald) sont vérifiables sans toi.
- **Volume de travail réel** : une adaptation complète et fidèle de la saison 1 (12+ villes stylées, arènes scénarisées, Team Rocket récurrente avec dialogues propres, rival évolutif, Ligue avec Elite Four, doublage textuel intégral en français) représente, à titre de comparaison, un travail que des équipes de hack communautaires mènent sur plusieurs **années**. Je peux produire un volume élevé par session, mais annoncer un jeu "complet" à court terme serait malhonnête. Je fonctionnerai en incréments documentés (cf. PROJECT_STATUS.md), zone par zone.

---

## 3. Recommandation

**Option B (moteur pokeemerald décompilé)**, avec split de responsabilité :
- Moi : scénario, dialogues FR, structure narrative, données (dresseurs/Pokémon/objets/flags/variables), scripts d'événements, intégration, compilation, QA automatisée, documentation.
- Toi : édition visuelle des maps sous Porymap en local (je fournis les specs détaillées de chaque map à créer), et tests de jeu réels sous émulateur (je n'ai pas d'émulateur graphique ici).

C'est la seule option compatible à la fois avec l'ambition du cahier des charges et avec les contraintes réelles de cet environnement.

---

## 4. Contraintes d'environnement confirmées

- Réseau sortant limité à : GitHub (dépôts pret/*, outils), PyPI, npm — **pas** d'accès à romhacking.net, forums, Discord, assets communautaires hors GitHub.
- Pas d'interface graphique / pas d'émulateur GBA jouable ici.
- Toolchain de compilation C/ASM ARM : **validée et fonctionnelle** (gcc-arm-none-eabi, binutils-arm-none-eabi).
- Aucune mémoire persistante entre sessions → PROJECT_STATUS.md et CHANGELOG.md indispensables dès la Phase 1.

---

## 5. Décisions requises avant Phase 1 (Game Design)

Voir questions posées dans le chat.
