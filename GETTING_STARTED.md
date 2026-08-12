# GETTING_STARTED.md — Reprise du projet sous Claude Code

## Avant toute chose
Lis `docs/PROJECT_STATUS.md` (état actuel, prochaines étapes) et `docs/CHANGELOG.md` (historique complet
des 3 sessions précédentes). Tout le reste de `docs/` complète ce panorama (architecture technique,
game design, personnages, épisodes, correspondance des maps).

## Installer la toolchain (une seule fois)
```bash
sudo apt-get update
sudo apt-get install -y build-essential binutils-arm-none-eabi libpng-dev gcc-arm-none-eabi
```

## Compiler
```bash
cd engine
make MODERN=1 -j$(nproc)
```
Sortie : `engine/pokeemerald.gba`

## État du projet (résumé — voir PROJECT_STATUS.md pour le détail à jour)
- **Arc 1 fonctionnellement complet** : réception de Pikachu, suivi overworld (hors POKé BALL), première
  rencontre Team Rocket (vrai combat contre James), premier combat de rival contre Régis
- **Renommage régional** : 11 villes + 34 routes Hoenn renommées avec les noms français officiels Kanto
  (voir `docs/MAPS.md`)
- **Non commencé** : réécriture narrative de la maison du joueur (Épisode 1.1 — actuellement toute la
  map référence un scénario Hoenn incompatible, à ne pas traduire littéralement, voir CHANGELOG)
- **QA harness** (`tools/qa_harness/`) : outil de test headless basé sur libmgba, fonctionnel pour le
  boot et la capture d'écran, mais **bloqué sur la prise en compte des entrées clavier** — dernière piste
  non essayée : vérifier `core->getKeys()` juste après `setKeys()` pour voir si le core mémorise bien l'état

## Pourquoi ce transfert vers Claude Code
Ce projet vivait jusqu'ici dans un environnement bac-à-sable éphémère (une conversation claude.ai) —
tout aurait été perdu à la moindre réinitialisation. Sous Claude Code, le projet vit dans un vrai dépôt
git sur ta machine : historique versionné, pas de reconstruction de toolchain à chaque session, et surtout
**tu peux tester la ROM toi-même avec un vrai émulateur graphique** (mGBA, VBA) au lieu de dépendre
uniquement de mes retours en aveugle — ce qui aurait évité l'aller-retour sur le bug d'élévation Team
Rocket de la session 3, par exemple.

## Conventions établies (à respecter pour la suite)
- Tutoiement, noms de lieux/personnages officiels français (voir CHARACTERS.md, MAPS.md)
- Toujours rebuild après une modification de script/texte avant de considérer une tâche terminée
- Ne jamais traduire un texte sans vérifier qu'il ne référence pas une intrigue Hoenn incompatible
  (cf. l'exemple de la maison du joueur)
- Documenter chaque session dans CHANGELOG.md + PROJECT_STATUS.md, comme fait jusqu'ici
