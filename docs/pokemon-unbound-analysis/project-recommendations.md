# Recommandations pour notre projet

## Constat de départ, honnête

Cette analyse statique n'a identifié **aucun système de gameplay, de contenu narratif ou de structure de
données spécifique à Unbound avec une confiance suffisante pour être "repris" ou "adapté" concrètement**.
Ce n'est pas un échec de méthode — c'est la limite structurelle d'un pack de métadonnées sans contenu
(voir `confidence.md`). Les recommandations ci-dessous s'appuient donc sur ce qui EST solide : la
confirmation de l'architecture de base d'Unbound, et les leçons méthodologiques de cet exercice
lui-même — pas sur une prétendue rétro-ingénierie de ses mécaniques.

## Tableau de recommandations

| Système | Constat sur Unbound | Intérêt pour nous | Complexité | Recommandation |
|---|---|---|---|---|
| Base moteur (FireRed/CFRU) | CONFIRMÉ : Unbound tourne sur FireRed, pas Émeraude | Notre projet est déjà bâti sur `pokeemerald-expansion` (Émeraude). Copier le code d'Unbound est impossible (architecture différente) et non souhaitable (ROM protégée). | — | **NE PAS UTILISER.** Si des mécaniques FireRed-only nous intéressent un jour, la référence légale est `pret/pokefirered`, pas Unbound. |
| Réputation "contenu volumineux, rythme soigné" | Fait déjà noté en Session 1 (`PROJECT_ANALYSIS.md`) comme motivation du choix d'architecture, indépendant de cette analyse | Notre cahier des charges vise déjà "la trempe d'Unbound" en ambition (GAME_DESIGN_DOCUMENT.md) | Élevée (c'est un choix de contenu/rythme, pas de code) | **ADAPTER L'ESPRIT**, pas le contenu : continuer notre approche déjà en place (un arc à la fois, build compilable à chaque étape, cf. piliers du cahier des charges), qui est structurellement la même discipline que celle nécessaire pour produire un hack de cette ampleur. |
| Outillage d'analyse statique local (scripts créés cette session) | N/A — nos propres outils, pas ceux d'Unbound | Utile pour déboguer NOS PROPRES builds (ex. localiser un problème de compilation, auditer la taille d'occupation ROM) sans dépendre d'outils externes | Faible (déjà écrit) | **REPRENDRE LE CONCEPT**, usage légitime : `tools/unbound_analysis/*.py` peuvent être généralisés en `tools/rom_analysis/*.py` pour analyser NOTRE PROPRE `pokeemerald.gba` compilé (pas une ROM tierce) — utile par exemple pour visualiser la répartition d'occupation ROM déjà trackée dans nos CHANGELOG (`79,01 %` etc.) de façon plus fine que le seul résumé `--print-memory-usage` du linker. |
| Détection de tables par corrélation stride+entropie+pointeurs | Méthode développée pour cette analyse, validée empiriquement (calibration en deux passes documentée) | Transposable à l'audit de nos propres données (`src/data/*.h`, `trainers.party`) si on veut un jour vérifier automatiquement des incohérences de taille de struct ou des doublons | Moyenne | **ADAPTER** : la méthode (pas le résultat) est réutilisable comme outil de QA interne. |

## Ce qu'on NE recommande PAS de faire, explicitement

- Chercher à obtenir une ROM d'Unbound (patchée ou non) pour "aller plus loin" dans cette analyse — refus
  maintenu quatre fois cette session pour des raisons de droit d'auteur, une analyse statistique plus
  poussée ne change pas ce refus.
- Présenter les hypothèses de `pokemon-data.md`/`pointer-analysis.md` (tables candidates d'espèces/
  attaques) comme une base fiable pour dimensionner nos propres structures de données — elles restent au
  niveau HYPOTHÈSE et ne sont pas plus fiables qu'une estimation directe depuis les conventions publiques
  pret, que nous utilisons déjà.
- Répliquer la structure de fichiers ou l'organisation ROM déduite ici — notre projet a déjà sa propre
  organisation (`data/maps/*/map.json`, `data/scripts/*.inc`), validée et fonctionnelle depuis 17 builds.

## Prochaine étape suggérée, si le sujet reste d'intérêt

Si l'objectif réel est de s'inspirer du **game design** d'Unbound (rythme, difficulté, structure narrative)
plutôt que de sa technique : ce sont des choses qu'on peut discuter directement à partir de ce qui est
public (retours de joueurs, changelogs officiels, vidéos de gameplay que Thomas aurait regardées) — pas
par rétro-ingénierie binaire. Je suis disponible pour ça si c'est utile.
