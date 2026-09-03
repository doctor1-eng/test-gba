# Dialogues

## Constat le plus important de cette section

`string-index.csv` (307 109 entrées, colonnes `offset, gba, length`, contenu explicitement omis par le
pack) indexe des séquences d'octets ASCII imprimables. Distribution des longueurs :

| Percentile | Longueur (octets) |
|---|---|
| min | 4 |
| p25 | 4 |
| médiane (p50) | 5 |
| p75 | 7 |
| p90 | 11 |
| p99 | 41 |
| max | 6016 |

**PROBABLE, confiance élevée : ceci n'est très majoritairement PAS un index de dialogues du jeu.**
Raisonnement, en deux points indépendants et convergents :

1. **Convention publique bien établie sur les jeux Pokémon GBA** (CONFIRMÉ — fait que nous appliquons
   nous-mêmes tout au long de ce projet, voir `engine/charmap.txt` dans notre propre dépôt) : le texte de
   dialogue en jeu n'est **pas** encodé en ASCII standard. Ces jeux utilisent une table de caractères
   propriétaire (charmap) où la plupart des octets ne correspondent pas aux mêmes lettres qu'en ASCII. Un
   scanner de chaînes ASCII générique (comme celui qui a probablement produit ce CSV, vu le seuil minimal
   de 4 octets typique de l'outil `strings` standard) **manque presque entièrement le texte réel du jeu**,
   puisque ce texte ne "ressemble" pas à de l'ASCII au niveau des octets.
2. **La distribution des longueurs elle-même le confirme** : une médiane de 5 octets est incompatible
   avec des phrases de dialogue (même une réplique courte fait généralement plusieurs dizaines de
   caractères). Ce profil est plutôt celui de courtes séquences alphanumériques coïncidentes — identifiants
   internes, fragments de symboles de débogage résiduels, ou simple coïncidence statistique dans des
   données binaires (avec 32 Mo de données, des runs de 4-5 octets imprimables apparaissent par hasard des
   dizaines de milliers de fois).

## Conséquence pour la classification par région

Les régions marquées `table_texte_probable` dans `rom-map.md` (208 régions, ex. 0x11A0000, 0x117C000)
doivent être relues à la lumière de ce constat : une forte densité de "chaînes ASCII courtes" dans une
région à haute entropie (7.7-7.8, proche du maximum théorique) est **plus probablement le signe d'une
donnée binaire dense et variée (compressée ou non) qui génère par hasard beaucoup de runs de 4-5 octets
imprimables**, que le signe d'une vraie table de texte. **Reclassification suggérée : ces régions sont
des candidats pour des données compressées ou des tables binaires denses, pas prioritairement pour du
texte de dialogue.** Ceci corrige une lecture trop littérale du nom de la classification produite par
`correlate_regions.py`.

## Ce qu'on ne peut donc pas dire

- Où se trouve la vraie table de texte du jeu (charmap-encodée) — ce pack ne peut structurellement pas la
  révéler, quel que soit le niveau d'analyse statistique appliqué dessus.
- Le nombre réel de chaînes de dialogue, leur encodage exact, leur système de pointeurs, la présence de
  variables dynamiques (`{PLAYER}`, `{RIVAL}`...) ou de contrôles de mise en forme.

## Référence méthodologique publique

Notre propre projet utilise déjà, de façon confirmée et fonctionnelle (validée par nos builds de session),
les conventions FireRed/Emerald standard pour l'encodage de texte : table de caractères (`charmap.txt`),
codes de contrôle (`\n`, `\l`, `\p`), variables (`{PLAYER}`, `{RIVAL}`), limites connues (ex. tiret
cadratin absent de la charmap, documenté dans notre `docs/TECHNICAL_ARCHITECTURE.md`). C'est la référence
directement exploitable pour notre travail — pas une inférence depuis ce pack.
