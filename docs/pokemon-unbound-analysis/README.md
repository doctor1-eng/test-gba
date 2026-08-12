# Analyse statique — Pokémon Unbound v2.1.1.1

## Ce que c'est, et ce que ce n'est pas

Cette analyse ne porte **pas** sur une ROM. Elle porte sur un pack de 11 fichiers CSV/JSON/Markdown
fourni par Thomas (`Pokemon_Unbound_MAX_Analysis_29Mo.zip`), décrit comme dérivé d'une analyse statique
binaire d'une ROM Pokémon Unbound v2.1.1.1, réalisée localement en dehors de cet environnement. Vérifié
avant toute analyse : le zip ne contient aucun `.gba`, aucune image, aucun son — uniquement des tableaux
de métadonnées (offsets, tailles, compteurs de références, entropie). Voir `manifest.json` et `README.md`
du pack source, tous deux repris dans `data/`.

**Refus maintenu sur trois fichiers distincts avant celui-ci** : deux ROMs complètes (`Pokemon Unbound
v2.1.1.1.gba` fourni en zip, `Pokemon Fire Red USquirrels.gba`, `Pokemon Fire Red V1.1.gba` vanilla) —
toutes trois des binaires de jeu Nintendo/Game Freak/Creatures, hackés ou non. Ce pack-ci est différent
par nature : ce sont des statistiques dérivées, pas le binaire lui-même, et ne reproduisent aucun contenu
protégé (texte, graphismes, audio) — confirmé en inspectant la structure réelle des CSV avant de
commencer, pas seulement en se fiant à l'étiquette du fichier.

## Ce qui a été confirmé

- **CONFIRMÉ** (depuis `manifest.json`, information déclarative sur la source, pas dérivée par nous) :
  ROM source de 33 554 432 octets (32 MiB), en-tête GBA `POKEMON FIRE` / code jeu `BPRE` / mainteneur `01`.
  `BPRE` est le code jeu standard publiquement documenté de Pokémon FireRed (US) — ceci confirme
  qu'Unbound est bâti sur une base FireRed, cohérent avec `docs/PROJECT_ANALYSIS.md` (Session 1 de notre
  propre projet, qui notait déjà "Unbound tourne sur CFRU, Complete FireRed Upgrade").
- **CONFIRMÉ** : somme de contrôle d'en-tête déclarée valide (`0x68` == `0x68`).
- **CONFIRMÉ** (comptages bruts fournis) : 307 109 séquences ASCII indexées, 197 543 pointeurs
  candidats, 307 380 cibles BL ARM candidates, 129 483 cibles BL Thumb candidates, 308 777 signatures de
  compression candidates, 200 000 blocs de 16 octets répétés enregistrés.

Tout le reste — classification par zone, tables candidates, hypothèses de systèmes — est dérivé par nous
à partir de ces comptages, donc au mieux **PROBABLE**, généralement **HYPOTHÈSE**. Voir `confidence.md`.

## Découverte méthodologique importante

Deux des sept signaux fournis se sont révélés **probablement dominés par des faux positifs** à l'échelle
de la ROM entière :
- `arm-bl-targets.csv` : 100 % des 2048 régions de 16 KiB contiennent au moins 69 cibles ARM candidates,
  y compris dans des zones à très haute entropie où du vrai code ARM ne devrait pas se trouver.
- `compression-signatures.csv` : même la région 0 (0x0–0x3FFF), qui par convention GBA contient l'en-tête
  et le tout début du code, affiche 359 signatures de compression candidates.

Voir `code-analysis.md` et `compression.md` pour le détail. Ceci a changé la méthode de classification en
cours de route (voir `tools/unbound_analysis/correlate_regions.py`, deux itérations de seuils).

## Documents

| Fichier | Contenu |
|---|---|
| `technical-structure.md` | En-tête, taille, format, ce qui est confirmé vs déduit |
| `rom-map.md` | Cartographie par région de 16 KiB, classification heuristique |
| `code-analysis.md` | Cibles ARM/Thumb, fiabilité des signaux, fonctions "hub" candidates |
| `pointer-analysis.md` | Tables à espacement régulier détectées |
| `compression.md` | Signatures de compression, zones à haute entropie |
| `maps.md` | Ce qui est déductible sur le système de cartes (très limité avec ces données) |
| `scripts.md` | Ce qui est déductible sur le système de scripts (très limité) |
| `dialogues.md` | Structure des tables de texte candidates |
| `pokemon-data.md` | Candidats structurels pour espèces/attaques/objets/dresseurs |
| `battle-system.md` | Ce qui est déductible sur le moteur de combat |
| `gameplay-systems.md` | Vue d'ensemble des systèmes, très majoritairement INCONNU |
| `confidence.md` | Registre de toutes les affirmations avec leur niveau de confiance |
| `project-recommendations.md` | Ce qui est exploitable pour notre projet, et comment |
| `FINAL-REPORT.md` | Synthèse finale |

## Outils créés

- `tools/unbound_analysis/correlate_regions.py` — corrèle les 8 CSV en un profil par région de 16 KiB
  avec classification heuristique (`data/region-profile.csv`)
- `tools/unbound_analysis/analyze_branches.py` — classe les cibles BL par nombre de références
  (`data/top-branch-targets.md`)
- `tools/unbound_analysis/detect_tables.py` — détecte séquences à pas constant et clusters de blocs
  répétés (`data/table-candidates.md`)

Les trois sont relançables (`python3 tools/unbound_analysis/<script>.py <dossier_pack> <dossier_sortie>`),
ne modifient aucune entrée, et ne lisent/n'écrivent jamais de ROM.

## Limites de l'analyse (honnêtes)

- Aucun désassemblage réel n'a été effectué — tout ce qui touche au code (ARM/Thumb) reste au niveau
  "candidat statistique", jamais confirmé.
- Le pack ne contient aucun contenu (texte, graphismes, audio) — seulement des métadonnées. On ne peut
  donc **rien** dire sur le contenu narratif, les noms de Pokémon custom, les dialogues, ou l'apparence
  des graphismes d'Unbound à partir de ces données seules.
- Deux des sept signaux (ARM BL, signatures de compression) sont probablement peu fiables à l'échelle
  globale — utilisés avec prudence, en pondération secondaire, dans la classification par région.
- Aucune corrélation avec le code source réel de CFRU ou d'Unbound n'a été possible (nous n'avons pas
  accès à leur dépôt).

## Prochaines étapes recommandées

Voir `project-recommendations.md` pour la réponse complète. En bref : cette analyse confirme surtout des
faits déjà connus indépendamment (base FireRed/CFRU, taille 32 Mo) et fournit une cartographie
approximative par densité de signal — utile comme point de départ mais insuffisante pour extraire ou
reproduire un système précis d'Unbound. Les décisions de conception utiles pour notre projet devraient
plutôt s'appuyer sur `pret/pokefirered` (décompilation FireRed publique et légale) comme référence
architecturale directe, puisque Unbound et notre projet visent la même famille de moteur.
