# GAME_DESIGN_DOCUMENT.md

## Pitch
Vivre la Saison 1 de l'anime Pokémon comme un vrai RPG Pokémon — pas Émeraude reskinné, une adaptation
jouable de l'aventure de Kanto, avec la structure et la profondeur d'un hack de la trempe d'Unbound.

## Le joueur = équivalent de Sacha
- Starter reçu (pas choisi) : **Pikachu**, en résistance au départ (fidélité à l'anime — "je ne rentre pas dans une Poké Ball").
- Un starter classique optionnel pourra être ajouté plus tard en complément narratif (ex. offert par un
  personnage secondaire), à discuter — non prioritaire pour le prototype.

## Piliers de gameplay (ordre de priorité, cf. PROJECT_ANALYSIS.md §33)
1. Projet compilable à chaque étape (aucun commit qui casse le build)
2. Stabilité (pas de softlock, pas d'événement bloquant)
3. Progression jouable de bout en bout
4. Scénario fidèle à l'esprit de la saison 1
5. Gameplay (équilibrage, rythme, mécaniques modernes activées)
6. Contenu (volume d'arcs/quêtes)
7. Fidélité visuelle/textuelle à l'anime
8. Polish

## Découpage macro (arcs — cf. cahier des charges)
| Arc | Zone(s) | Statut |
|---|---|---|
| 1 — Bourg Palette | Bourg Palette, Route 1 | **En cours (vertical slice)** |
| 2 — Premières routes | Jadielle, forêt de Jade, Route 2-3 | Non commencé |
| 3 — Ondine | Azuria | Non commencé |
| 4 — Pierre | Argenta | Non commencé |
| 5 — Kanto (suite) | Carmin-sur-Mer → Plateau Indigo | Non commencé |

Chaque arc suivra le même processus que défini dans le cahier des charges (§24, Phase 3) :
map → PNJ → scripts → dialogues → combats → Pokémon → objets → événements → tests, un arc à la fois,
jamais en parallèle, pour garder un projet toujours compilable et testable.

## Systèmes techniques activés (mécaniques modernes, décision Session 2)
Confirmé : on assume pleinement les mécaniques modernes (type Fée, split Physique/Spécial, Pokémon/objets
jusqu'à Gen 9, TM réutilisables, EXP Share configurable). Le moteur `pokeemerald-expansion` les a déjà
actives par défaut (`GEN_LATEST = GEN_9`) — aucune configuration supplémentaire requise à ce stade.

## Systèmes narratifs propres au projet
- **Pikachu-personnage** : flags dédiés pour des réactions/dialogues contextuels de Pikachu (refus de Poké
  Ball, réactions à certains événements) — mécanisme à concevoir en Phase 2.
- **Team Rocket récurrente** (Jessie/James/Miaouss) : rencontres scriptées à des points clés de chaque arc,
  indépendantes de la progression de badge, pour créer une continuité comique et narrative.
- **Régis (rival)** : combats à chaque ville importante, équipe qui évolue avec celle du joueur, quelques
  victoires de sa part pour rester crédible (cf. cahier des charges §17).

## Renommage Hoenn → Kanto — méthode
Le moteur repose sur la géographie de Hoenn (32 sections de région map). Plutôt que renommer point par point
au hasard, chaque zone renommée devra être actée dans `MAPS.md` (à créer) avec : nom Hoenn d'origine → nom
Kanto cible → statut (juste renommée / reconstruite visuellement). Fait à ce jour : Littleroot Town → Bourg
Palette (nom affiché uniquement — la map physique n'est pas encore retravaillée, cf. Phase 2).

## Prochaine étape technique concrète
Remplacement du système de choix de starter (`src/starter_choose.c`) par une remise directe de Pikachu —
première brique du vertical slice Bourg Palette.
